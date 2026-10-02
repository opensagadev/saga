#!/usr/bin/env python3
"""Compare functions by what their PIC-relative operands refer to.

The project's objdiff fork recovers the GOT-base ``add``, GOT-slot loads and
``lea`` of a uniquely named symbol in linked i386 code, but compares other
``GOTOFF`` displacements numerically: loads and stores of globals and statics,
constants, unlabeled literals, indexed operands, and everything in a function
with a jump table. A displacement is the distance from the GOT to a `.rodata`,
`.data` or `.bss` item, so it only matches when the data layout of the whole
binary matches. This experimental report resolves every such operand in both
binaries instead and treats two instructions as equal when they refer to the
same thing:

- the same named symbol at the same offset, defined in the same section
  (globals, file statics, statics inside functions; `.data` and `.bss` differ,
  as an initialized variable is not a zeroed one);
- a GOT slot for the same symbol, defined in the same section;
- an anonymous constant with identical bytes (width taken from the operand);
- an identical C string (for ``lea`` of a literal);
- a compiler-generated table (``CSWTCH.n``, ``.LCn``) with identical contents;
- a PIC jump table whose entries land on the same offsets in the function.

Branches and calls compare their targets by symbol (ignoring the numbering of
``.isra``/``.part``/``.constprop`` clones and ``@plt``), and the GOT-base
``add`` after a PC thunk compares the address it produces. Everything else must
be textually identical, and both functions must have the same number of
instructions.

The report prints how many functions would be exact under this comparison,
validates it against objdiff's exact functions, and lists near-exact functions
whose resolved data genuinely differs (wrong constants, strings, globals or
field offsets that raw displacements hide).

Usage:  bazel run //scripts:resolved_match_report -- [--json OUT] [--hidden N]
"""

from __future__ import annotations

import argparse
import bisect
from collections import Counter, defaultdict
from dataclasses import dataclass, field
import json
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys

SHT_NOBITS = 8
SHT_SYMTAB = 2
SHT_DYNSYM = 11
SHT_REL = 9
STT_FUNC = 2

R_386_32 = 1
R_386_GLOB_DAT = 6
R_386_JUMP_SLOT = 7
R_386_RELATIVE = 8

WIDTH = {"BYTE": 1, "WORD": 2, "DWORD": 4, "QWORD": 8, "TBYTE": 10, "XMMWORD": 16}
NUMBER = re.compile(r"([+-]?)0x([0-9a-f]+)")
CLONE = re.compile(r"\.(isra|part|constprop|clone)\.\d+")
INSN = re.compile(r"^\s+([0-9a-f]+):\t(.*)$")
BRANCH = re.compile(r"^\S+ (?:short )?(?:0x)?([0-9a-f]+)(?: <([^>+]*)(\+0x[0-9a-f]+)?>)?$")
THUNK = re.compile(r"^call \S+ <__x86\.get_pc_thunk\.(\w\w)>")
ANONYMOUS_TABLE = ("CSWTCH.", ".L")


def workspace_root() -> Path:
    """Return the source workspace both under `bazel run` and direct Python."""
    bazel_workspace = os.environ.get("BUILD_WORKSPACE_DIRECTORY")
    if bazel_workspace:
        return Path(bazel_workspace).resolve()
    return Path(__file__).resolve().parents[1]


def bazel_target_output(root: Path, bazel: str, target: str) -> Path:
    """Resolve a target-config output without depending on the bazel-bin link."""
    result = subprocess.run(
        [bazel, "cquery", "--config=target", target, "--output=files", "--noshow_progress"],
        cwd=root,
        text=True,
        capture_output=True,
        check=False,
    )
    if result.returncode:
        sys.stderr.write(result.stderr)
        raise RuntimeError(f"bazel cquery failed for {target}")
    files = [root / line for line in result.stdout.splitlines() if (root / line).is_file()]
    if len(files) != 1:
        raise RuntimeError(
            f"expected one built output for {target}; build it with --config=target first"
        )
    return files[0].resolve()


def find_objdump() -> str:
    """GNU objdump (any recent version) prints the Intel syntax parsed here."""
    env = os.environ.get("OBJDUMP")
    if env:
        return env
    for candidate in ("i686-linux-android-objdump", "objdump"):
        path = shutil.which(candidate)
        if path:
            return path
    raise RuntimeError("GNU objdump not found; set OBJDUMP")


def fname(name: str) -> str:
    return CLONE.sub(r".\1", name.replace("@plt", ""))


@dataclass
class Section:
    name: str
    address: int
    size: int
    offset: int | None


class Binary:
    """The parts of a 32-bit little-endian ELF shared object needed here."""

    def __init__(self, path: Path):
        self.path = path
        self.data = path.read_bytes()
        if self.data[:4] != b"\x7fELF" or self.data[4] != 1 or self.data[5] != 1:
            raise ValueError(f"{path}: expected a 32-bit little-endian ELF file")
        header = struct.unpack_from("<HHIIIIIHHHHHH", self.data, 16)
        shoff, shentsize, shnum, shstrndx = header[5], header[10], header[11], header[12]
        raw = [
            struct.unpack_from("<IIIIIIIIII", self.data, shoff + i * shentsize)
            for i in range(shnum)
        ]
        names = raw[shstrndx]

        def cstr(offset: int) -> str:
            end = self.data.index(b"\0", offset)
            return self.data[offset:end].decode("utf-8", errors="replace")

        self.sections: list[Section] = []
        by_name = {}
        for sh in raw:
            name = cstr(names[4] + sh[0])
            by_name[name] = sh
            if sh[3]:
                self.sections.append(
                    Section(name, sh[3], sh[5], None if sh[1] == SHT_NOBITS else sh[4])
                )
        self.got = by_name[".got.plt"][3]

        symbols = set()
        self.functions: dict[str, tuple[int, int]] = {}
        function_counts: Counter[str] = Counter()
        dynamic_names: list[str] = []
        self.definitions: dict[str, int] = {}
        for sh in raw:
            if sh[1] not in (SHT_SYMTAB, SHT_DYNSYM):
                continue
            strtab = raw[sh[6]]
            for i in range(sh[5] // 16):
                st_name, value, size, info, _, _ = struct.unpack_from(
                    "<IIIBBH", self.data, sh[4] + i * 16
                )
                name = cstr(strtab[4] + st_name)
                if sh[1] == SHT_DYNSYM:
                    dynamic_names.append(name)
                if not value or not name:
                    continue
                symbols.add((value, size, name))
                self.definitions.setdefault(name, value)
                if sh[1] == SHT_SYMTAB and info & 0xF == STT_FUNC:
                    function_counts[name] += 1
                    self.functions[name] = (value, size)
        self.duplicate_functions = {name for name, count in function_counts.items() if count > 1}
        ordered = sorted(symbols)
        self.symbol_addresses = [s[0] for s in ordered]
        self.symbols = ordered

        # GOT slot -> what it refers to.
        self.slots: dict[int, tuple] = {}
        for sh in raw:
            if sh[1] != SHT_REL:
                continue
            for i in range(sh[5] // 8):
                offset, info = struct.unpack_from("<II", self.data, sh[4] + i * 8)
                kind, index = info & 0xFF, info >> 8
                if kind in (R_386_32, R_386_GLOB_DAT, R_386_JUMP_SLOT) and index < len(
                    dynamic_names
                ):
                    self.slots[offset] = ("sym", dynamic_names[index])
                elif kind == R_386_RELATIVE:
                    self.slots[offset] = ("rel", self.u32(offset))

    def section(self, address: int) -> Section | None:
        for section in self.sections:
            if section.address <= address < section.address + section.size:
                return section
        return None

    def read(self, address: int, size: int) -> bytes | None:
        section = self.section(address)
        if section is None or section.offset is None:
            return None
        start = section.offset + address - section.address
        return self.data[start : start + size]

    def u32(self, address: int) -> int | None:
        raw = self.read(address, 4)
        return struct.unpack("<I", raw)[0] if raw is not None and len(raw) == 4 else None

    def containing(self, address: int) -> tuple[int, int, str] | None:
        """Smallest symbol covering address (a sized-0 symbol covers its first byte)."""
        best = None
        index = bisect.bisect_right(self.symbol_addresses, address) - 1
        stop = max(-1, index - 64)
        while index > stop:
            value, size, name = self.symbols[index]
            if value <= address < value + max(size, 1) and (best is None or size < best[1]):
                best = (value, size, name)
            index -= 1
        return best

    def placement(self, address: int | None) -> str:
        """Section of a definition: initialized and zeroed data are not interchangeable."""
        section = None if address is None else self.section(address)
        return section.name if section else "undefined"

    def identity(self, address: int, width: int | None, mnemonic: str) -> tuple:
        """What the operand at address refers to, comparable across binaries."""
        section = self.section(address)
        if section is None:
            return ("unmapped",)
        if section.name in (".got", ".got.plt"):
            slot = self.slots.get(address - address % 4)
            if slot is None:
                return ("got-unknown",)
            if slot[0] == "sym":
                definition = self.definitions.get(slot[1])
                return ("got", "sym", slot[1], 0, self.placement(definition))
            return ("got",) + self.identity(slot[1], width, "ptr")
        symbol = self.containing(address)
        if symbol and symbol[2].startswith(ANONYMOUS_TABLE) and symbol[1] > 0:
            return ("table", self.read(symbol[0], symbol[1]), address - symbol[0])
        if symbol and not symbol[2].startswith(".L"):
            return ("sym", symbol[2], address - symbol[0], section.name)
        if section.name == ".rodata":
            if mnemonic == "lea" or width is None:
                raw = self.read(address, 256) or b""
                end = raw.find(b"\0")
                if end >= 0 and all(32 <= c < 127 or c in (9, 10, 13) for c in raw[:end]):
                    return ("str", raw[: end + 1])
                return ("data", None)  # unknown extent: never equal
            return ("const", self.read(address, width))
        return ("anon", section.name)


def disassemble(objdump: str, path: Path) -> tuple[list[int], list[str]]:
    output = subprocess.run(
        [objdump, "-d", "-M", "intel", "--no-show-raw-insn", "-j", ".text", str(path)],
        capture_output=True,
        text=True,
        check=True,
    ).stdout
    addresses, texts = [], []
    for line in output.splitlines():
        match = INSN.match(line)
        if match:
            addresses.append(int(match.group(1), 16))
            texts.append(" ".join(match.group(2).split()))
    return addresses, texts


@dataclass
class Function:
    name: str
    address: int
    size: int
    source: str | None
    objdiff: float
    status: str = "unpaired"  # same | resolved | differs | unpaired
    kinds: Counter = field(default_factory=Counter)
    difference: tuple | None = None


class Comparator:
    def __init__(self, original: Binary, current: Binary):
        self.a, self.b = original, current
        self.failure: tuple | None = None

    def compare(self, fn: Function, ia: list, ib: list, fa: tuple, fb: tuple) -> None:
        if len(ia) != len(ib):
            fn.status, fn.difference = "differs", ("instruction count", len(ia), len(ib))
            return
        pic_a = {f"e{m.group(1)}" for _, t in ia if (m := THUNK.match(t))}
        pic_b = {f"e{m.group(1)}" for _, t in ib if (m := THUNK.match(t))}
        for (aa, ta), (ab, tb) in zip(ia, ib):
            self.failure = None
            kind = self.instruction(aa, ta, ab, tb, fa, fb, pic_a | pic_b)
            if kind is False:
                fn.status = "differs"
                fn.difference = (hex(aa), ta, hex(ab), tb, self.failure)
                return
            if kind:
                fn.kinds[kind] += 1
        fn.status = "resolved" if fn.kinds else "same"

    def instruction(self, aa, ta, ab, tb, fa, fb, pic) -> str | None | bool:
        mnemonic = ta.split(" ")[0]
        if mnemonic != tb.split(" ")[0]:
            return False
        if mnemonic.startswith("j") or mnemonic in ("call", "loop"):
            return self.branch(ta, tb, fa, fb)
        if ta == tb:
            return None
        pa, pb = NUMBER.split(ta), NUMBER.split(tb)
        if len(pa) != len(pb) or any(pa[i] != pb[i] for i in range(0, len(pa), 3)):
            return False
        kind = None
        for i in range(1, len(pa), 3):
            va = int(pa[i + 1], 16) * (-1 if pa[i] == "-" else 1)
            vb = int(pb[i + 1], 16) * (-1 if pb[i] == "-" else 1)
            if va == vb:
                continue
            before = pa[i - 1]
            gotpc = re.match(r"add (e\w\w),$", before.strip())
            if gotpc and gotpc.group(1) in pic:
                if (aa + va) & 0xFFFFFFFF == self.a.got and (ab + vb) & 0xFFFFFFFF == self.b.got:
                    kind = kind or "gotpc"
                    continue
                return False
            operand = re.search(r"\[([^\]]*)$", before)
            if not operand:
                return False
            registers = re.findall(r"\b(e[a-z]{2})\b", operand.group(1))
            if not set(registers) & pic:
                return False
            size = re.search(r"(BYTE|WORD|DWORD|QWORD|TBYTE|XMMWORD) PTR", ta)
            width = WIDTH[size.group(1)] if size else None
            target_a = (self.a.got + va) & 0xFFFFFFFF
            target_b = (self.b.got + vb) & 0xFFFFFFFF
            indexed = len(registers) >= 2 or "*" in operand.group(1)
            if indexed and mnemonic == "mov" and self.jump_table(target_a, target_b, fa, fb):
                kind = kind or "jump table"
                continue
            ida = self.a.identity(target_a, width, mnemonic)
            idb = self.b.identity(target_b, width, mnemonic)
            comparable = ("sym", "table") if indexed else ("sym", "table", "got", "str", "const")
            if ida[0] not in comparable or ida != idb:
                self.failure = (ida, idb, ida[0] in comparable and idb[0] in comparable)
                return False
            kind = (
                kind
                or {
                    "sym": "data symbol",
                    "table": "constant",
                    "got": "GOT slot",
                    "str": "string",
                    "const": "constant",
                }[ida[0]]
            )
        return kind

    def branch(self, ta, tb, fa, fb) -> str | None | bool:
        ma, mb = BRANCH.match(ta), BRANCH.match(tb)
        if not ma or not mb:
            return None if ta == tb else False
        xa, xb = int(ma.group(1), 16), int(mb.group(1), 16)
        inside_a = fa[0] <= xa < fa[0] + fa[1]
        inside_b = fb[0] <= xb < fb[0] + fb[1]
        if inside_a or inside_b:
            return None if inside_a and inside_b and xa - fa[0] == xb - fb[0] else False
        if (
            ma.group(2)
            and mb.group(2)
            and fname(ma.group(2)) == fname(mb.group(2))
            and ma.group(3) == mb.group(3)
        ):
            return "branch"
        return False

    def jump_table(self, ta, tb, fa, fb) -> bool:
        entries = 0
        for i in range(1024):
            ea, eb = self.a.u32(ta + 4 * i), self.b.u32(tb + 4 * i)
            if ea is None or eb is None:
                break
            la, lb = (self.a.got + ea) & 0xFFFFFFFF, (self.b.got + eb) & 0xFFFFFFFF
            inside_a = fa[0] <= la < fa[0] + fa[1]
            inside_b = fb[0] <= lb < fb[0] + fb[1]
            if not inside_a and not inside_b:
                break
            if inside_a != inside_b or la - fa[0] != lb - fb[0]:
                return False
            entries += 1
        return entries > 0


def load_functions(report: dict) -> list[Function]:
    functions, seen = [], set()
    groups = [(u["functions"], u["source"]) for u in report["units"]]
    groups += [(report["unassigned_functions"], None), (report["ambiguous_functions"], None)]
    for entries, source in groups:
        for f in entries:
            key = (f["name"], f["address"])
            if key not in seen:
                seen.add(key)
                functions.append(
                    Function(
                        f["name"], f["address"], f["size"], source, f.get("match_percent", 0.0)
                    )
                )
    return functions


def describe(identity: tuple) -> str:
    kind = identity[0]
    if kind == "const" and identity[1] and len(identity[1]) == 4:
        return f"f32 {struct.unpack('<f', identity[1])[0]:g}"
    if kind == "str":
        return repr(identity[1][:-1].decode("latin-1"))
    if kind == "sym":
        name = f"{identity[1]}+{identity[2]}" if identity[2] else identity[1]
        return f"{name} in {identity[3]}"
    if kind == "table":
        return "compiler table"
    if kind == "got":
        return "GOT(" + describe(identity[1:]) + ")" if len(identity) > 1 else "GOT(?)"
    return kind


def main() -> int:
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument("--original", default="res/libTTapp.so")
    parser.add_argument(
        "--current", help="current binary (default: target-config output of //src:libTTapp.so)"
    )
    parser.add_argument(
        "--report", default="matching.json", help="objdiff report for the current binary"
    )
    parser.add_argument("--json", help="write per-function results to this file")
    parser.add_argument(
        "--hidden", type=int, default=40, help="list at most N hidden data differences"
    )
    parser.add_argument("--bazel", default="bazel")
    args = parser.parse_args()

    root = workspace_root()
    original_path = (root / args.original).resolve()
    current_path = (
        (root / args.current).resolve()
        if args.current
        else bazel_target_output(root, args.bazel, "//src:libTTapp.so")
    )
    report = json.loads((root / args.report).read_text())
    objdump = find_objdump()

    original, current = Binary(original_path), Binary(current_path)
    da, db = disassemble(objdump, original_path), disassemble(objdump, current_path)
    comparator = Comparator(original, current)

    def instructions(listing, start, size):
        lo = bisect.bisect_left(listing[0], start)
        hi = bisect.bisect_left(listing[0], start + size)
        return list(zip(listing[0][lo:hi], listing[1][lo:hi]))

    functions = load_functions(report)
    for fn in functions:
        if fn.name not in current.functions or fn.name in current.duplicate_functions:
            continue
        fa, fb = (fn.address, fn.size), current.functions[fn.name]
        comparator.compare(fn, instructions(da, *fa), instructions(db, *fb), fa, fb)

    paired = [f for f in functions if f.status != "unpaired"]
    objdiff_exact = [f for f in functions if f.objdiff >= 100.0]
    resolved_exact = [f for f in paired if f.status in ("same", "resolved")]
    newly = [f for f in resolved_exact if f.objdiff < 100.0]
    lost = [f for f in objdiff_exact if f.status == "differs"]
    total_code = sum(f.size for f in functions)
    code_objdiff = sum(f.size for f in objdiff_exact)
    code_resolved = code_objdiff + sum(f.size for f in newly) - sum(f.size for f in lost)
    exact_resolved = len(objdiff_exact) + len(newly) - len(lost)

    print(f"original {original_path}\ncurrent  {current_path}\n")

    def row(label: str, count: int, code: int) -> str:
        functions_cell = f"{count:>8} ({count / len(functions):6.2%})"
        code_cell = f"{code:>12} ({code / total_code:6.2%})"
        return f"{label:28}{functions_cell}{code_cell}"

    print(f"{'':28}{'functions':>18}{'code bytes':>22}")
    print(row("objdiff exact", len(objdiff_exact), code_objdiff))
    print(row("resolved exact", exact_resolved, code_resolved))
    print(f"\npaired {len(paired)} of {len(functions)} original functions by name")
    print(f"newly exact under resolution: {len(newly)}")
    print(f"objdiff-exact functions that differ when resolved: {len(lost)}")
    for f in lost:
        print(f"  {f.name}: {f.difference[1]}  vs  {f.difference[3]}")

    kinds = Counter()
    for f in newly:
        for kind in f.kinds:
            if kind not in ("branch", "gotpc"):
                kinds[kind] += 1
    print(
        "\nnewly exact functions needing each resolution: "
        + ", ".join(f"{k} {v}" for k, v in kinds.most_common())
    )

    units = defaultdict(list)
    for f in newly:
        units[f.source or "(unassigned)"].append(f)
    print("\nlargest gains by unit:")
    for source, entries in sorted(units.items(), key=lambda item: -len(item[1]))[:10]:
        print(f"  {len(entries):4}  {source}")

    near = [
        f
        for f in paired
        if f.status == "differs"
        and f.objdiff >= 99.0
        and f.difference
        and len(f.difference) > 4
        and f.difference[4]
    ]
    hidden = sorted((f for f in near if f.difference[4][2]), key=lambda f: -f.objdiff)
    print(f"\nnear-exact functions (objdiff >= 99%) whose first difference is an operand")
    print(f"  that could not be resolved (indexed or unsized data): {len(near) - len(hidden)}")
    print(f"  that resolves to different data in each binary: {len(hidden)}")
    for f in hidden[: args.hidden]:
        ida, idb, _ = f.difference[4]
        name = f.name[:48]
        print(
            f"  {f.objdiff:7.3f}  {name:48}  original {describe(ida):32}  current {describe(idb)}"
        )

    if args.json:
        out = [
            {
                "name": f.name,
                "address": f.address,
                "size": f.size,
                "source": f.source,
                "objdiff": f.objdiff,
                "status": f.status,
                "kinds": dict(f.kinds),
                "difference": None if f.difference is None else [repr(x) for x in f.difference],
            }
            for f in functions
        ]
        Path(args.json).write_text(json.dumps(out, indent=1) + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
