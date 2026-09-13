"""Conservative original-ELF function -> local-object instruction evidence.

Only straight-line i386 EBX PIC blocks with an exact thunk/add setup are
accepted. This reports references, never translation-unit ownership.
"""

from __future__ import annotations

import argparse
from collections import defaultdict
import json
from pathlib import Path
import struct

from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_OP_IMM, CS_OP_MEM, CS_GRP_JUMP, CS_GRP_RET
from capstone.x86_const import X86_REG_EBX, X86_REG_INVALID

from scripts.restructure.elf32 import read_elf32, workspace_root


def _section_bytes(blob: bytes, section: dict) -> bytes:
    return blob[section["offset"]:section["offset"] + section["size"]]


def _relative_addends(blob: bytes, sections: list[dict]) -> dict[int, int]:
    """Read ELF RELATIVE relocations; the stored word is the REL addend."""
    slots = {}
    for relsec in sections:
        if relsec["type"] != 9 or relsec["name"] != ".rel.dyn":
            continue
        raw = _section_bytes(blob, relsec)
        for offset in range(0, len(raw), 8):
            place, info = struct.unpack_from("<II", raw, offset)
            if info & 0xff != 8 or info >> 8:
                continue
            owners = [s for s in sections if s["address"] <= place < s["address"] + s["size"]
                      and s["type"] != 8 and s["flags"] & 2]
            if len(owners) != 1 or place + 4 > owners[0]["address"] + owners[0]["size"]:
                continue
            sec = owners[0]
            slots[place] = struct.unpack_from("<I", blob, sec["offset"] + place - sec["address"])[0]
    return slots


def extract(path: Path) -> list[dict]:
    blob = path.read_bytes()
    if blob[:6] != b"\x7fELF\x01\x01" or struct.unpack_from("<H", blob, 18)[0] != 3:
        raise ValueError("expected little-endian ELF32 i386")
    sections, symbols = read_elf32(path)
    text = next(s for s in sections if s["name"] == ".text")
    got = [s for s in sections if s["name"] in (".got", ".got.plt")]
    local_objects = defaultdict(list)
    for symbol in symbols:
        if (symbol["type"] == 1 and symbol["binding"] == 0 and symbol["size"] > 0
                and sections[symbol["section_index"]]["flags"] & 2
                and sections[symbol["section_index"]]["name"] not in (".got", ".got.plt")):
            local_objects[symbol["address"]].append(symbol)
    thunks = {s["address"] for s in symbols if s["type"] == 2
              and s["name"] == "__x86.get_pc_thunk.bx"}
    relative = _relative_addends(blob, sections)
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    result = []
    functions = [s for s in symbols if s["type"] == 2 and s["size"] > 0
                 and s["section_index"] == text["index"]]
    for function in functions:
        start, size = function["address"], function["size"]
        if start < text["address"] or start + size > text["address"] + text["size"]:
            continue
        raw = blob[text["offset"] + start - text["address"]:text["offset"] + start - text["address"] + size]
        instructions = list(decoder.disasm(raw, start))
        if sum(i.size for i in instructions) != size:
            continue
        branch_targets = {op.imm for ins in instructions if ins.group(CS_GRP_JUMP)
                          for op in ins.operands if op.type == CS_OP_IMM}
        pending = None
        base = None
        for ins in instructions:
            code = bytes(ins.bytes)
            if ins.address in branch_targets:
                pending = None
                base = None
            if pending is not None:
                if ins.address == pending and len(code) == 6 and code[:2] == b"\x81\xc3":
                    base = (pending + struct.unpack_from("<I", code, 2)[0]) & 0xffffffff
                    pending = None
                    continue
                pending = None
            if ins.mnemonic == "call":
                base = None
                if len(code) == 5 and code[0] == 0xe8:
                    target = ins.address + 5 + struct.unpack_from("<i", code, 1)[0]
                    if target in thunks:
                        pending = ins.address + 5
                continue
            if ins.group(CS_GRP_JUMP) or ins.group(CS_GRP_RET):
                base = None
                continue
            if base is None:
                continue
            for operand in ins.operands:
                if operand.type != CS_OP_MEM or operand.mem.base != X86_REG_EBX or operand.mem.index != X86_REG_INVALID:
                    continue
                address = (base + operand.mem.disp) & 0xffffffff
                slot = next((s for s in got if s["address"] <= address < s["address"] + s["size"]), None)
                if slot is not None:
                    target = relative.get(address) if address % 4 == 0 else None
                    kind = "got-relative-relocation"
                else:
                    target = address
                    kind = "direct-ebx-relative"
                candidates = sorted(local_objects.get(target, []), key=lambda s: s["symbol_index"])
                if not candidates:
                    continue
                result.append({"kind": kind, "function_symbol_index": function["symbol_index"],
                               "function": function["name"], "instruction_address": ins.address,
                               "instruction_bytes": code.hex(),
                               "object_symbol_indices": [s["symbol_index"] for s in candidates],
                               "object_names": [s["name"] for s in candidates],
                               "object_alias_ambiguous": len(candidates) > 1,
                               "object_address": target,
                               "got_slot": address if slot is not None else None,
                               "status": "verified address reference; aliases unresolved; not TU ownership"
                               if len(candidates) > 1 else "verified reference; not TU ownership"})
            _, writes = ins.regs_access()
            if X86_REG_EBX in writes:
                base = None
    return result


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("elf", nargs="?", type=Path, default=workspace_root() / "res/libTTapp.so")
    args = parser.parse_args()
    print(json.dumps(extract(args.elf), indent=2))


if __name__ == "__main__":
    main()
