"""Conservative original-ELF function -> local-object instruction evidence.

Only i386 PIC paths with an exact register-specific thunk/add setup are accepted. A
control-flow join retains the base only when every reachable predecessor
agrees. This reports references, never translation-unit ownership.
"""

from __future__ import annotations

import argparse
from collections import defaultdict, deque
import json
from pathlib import Path
import struct

from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_OP_IMM, CS_OP_MEM, CS_GRP_JUMP, CS_GRP_RET
from capstone.x86_const import (
    X86_REG_BH, X86_REG_BL, X86_REG_BX, X86_REG_EBX,
    X86_REG_CH, X86_REG_CL, X86_REG_CX, X86_REG_ECX,
    X86_REG_DH, X86_REG_DL, X86_REG_DX, X86_REG_EDX,
    X86_REG_INVALID,
)

from scripts.restructure.elf32 import read_elf32, workspace_root


PIC_REGISTERS = {
    X86_REG_EBX: ("bx", b"\x81\xc3", True),
    X86_REG_ECX: ("cx", b"\x81\xc1", False),
    X86_REG_EDX: ("dx", b"\x81\xc2", False),
}
PIC_REGISTER_ALIASES = {
    X86_REG_EBX: {X86_REG_EBX, X86_REG_BX, X86_REG_BH, X86_REG_BL},
    X86_REG_ECX: {X86_REG_ECX, X86_REG_CX, X86_REG_CH, X86_REG_CL},
    X86_REG_EDX: {X86_REG_EDX, X86_REG_DX, X86_REG_DH, X86_REG_DL},
}


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


def _pic_states(
    instructions: list, thunks: set[int], register: int, add_opcode: bytes,
    callee_saved: bool,
) -> dict[int, tuple[int | None, int | None]]:
    """Track a proven PIC base through branches without assuming it at joins.

    States are ``(base, pending_thunk_return_address)``. Unknown and conflicting
    paths both use ``(None, None)``; an absent state is unreachable. Ordinary
    calls preserve EBX under the i386 SysV ABI but invalidate ECX/EDX. The
    exact register-specific PC thunk establishes a new pending base.
    """
    if not instructions:
        return {}
    by_address = {ins.address: ins for ins in instructions}
    incoming = {instructions[0].address: (None, None)}
    work = deque([instructions[0].address])
    while work:
        address = work.popleft()
        ins = by_address[address]
        code = bytes(ins.bytes)
        following = address + ins.size
        base, pending = incoming[address]
        established_base = False
        if pending is not None:
            if address == pending and len(code) == 6 and code[:2] == add_opcode:
                base = (pending + struct.unpack_from("<I", code, 2)[0]) & 0xffffffff
                established_base = True
            pending = None
        if ins.mnemonic == "call":
            target = None
            if len(code) == 5 and code[0] == 0xe8:
                target = following + struct.unpack_from("<i", code, 1)[0]
            if target in thunks:
                base = None
                pending = following
            elif not callee_saved:
                base = None
                pending = None
        else:
            _, writes = ins.regs_access()
            if PIC_REGISTER_ALIASES[register].intersection(writes) and not established_base:
                base = None
        outgoing = (base, pending)
        successors = []
        if ins.group(CS_GRP_JUMP):
            successors.extend(op.imm for op in ins.operands if op.type == CS_OP_IMM)
            if ins.mnemonic != "jmp":
                successors.append(following)
        elif not ins.group(CS_GRP_RET):
            successors.append(following)
        for successor in successors:
            if successor not in by_address:
                continue
            previous = incoming.get(successor)
            joined = outgoing if previous is None else outgoing if previous == outgoing else (None, None)
            if previous != joined:
                incoming[successor] = joined
                work.append(successor)
    return incoming


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
    thunks = {
        register: {s["address"] for s in symbols if s["type"] == 2
                   and s["name"] == f"__x86.get_pc_thunk.{name}"}
        for register, (name, _, _) in PIC_REGISTERS.items()
    }
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
        call_targets = {
            ins.address + ins.size + struct.unpack_from("<i", bytes(ins.bytes), 1)[0]
            for ins in instructions if ins.mnemonic == "call" and ins.size == 5 and ins.bytes[0] == 0xe8
        }
        for register, (name, add_opcode, callee_saved) in PIC_REGISTERS.items():
            if not call_targets.intersection(thunks[register]):
                continue
            states = _pic_states(instructions, thunks[register], register, add_opcode, callee_saved)
            for ins in instructions:
                if ins.address not in states:
                    continue
                code = bytes(ins.bytes)
                base, pending = states[ins.address]
                if pending is not None:
                    if ins.address == pending and len(code) == 6 and code[:2] == add_opcode:
                        base = (pending + struct.unpack_from("<I", code, 2)[0]) & 0xffffffff
                if base is None:
                    continue
                for operand in ins.operands:
                    if operand.type != CS_OP_MEM or operand.mem.base != register or operand.mem.index != X86_REG_INVALID:
                        continue
                    address = (base + operand.mem.disp) & 0xffffffff
                    slot = next((s for s in got if s["address"] <= address < s["address"] + s["size"]), None)
                    if slot is not None:
                        target = relative.get(address) if address % 4 == 0 else None
                        kind = "got-relative-relocation"
                    else:
                        target = address
                        kind = f"direct-e{name}-relative"
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
    return result


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("elf", nargs="?", type=Path, default=workspace_root() / "res/libTTapp.so")
    args = parser.parse_args()
    print(json.dumps(extract(args.elf), indent=2))


if __name__ == "__main__":
    main()
