"""Inventory original ELF32 i386 dynamic relocation sites and pointer targets.

Relocations describe linker-visible data layout. They do not, by themselves,
identify the translation unit that emitted either side of a reference.
"""

from __future__ import annotations

import argparse
from collections import Counter, defaultdict
import json
from pathlib import Path
import struct

from scripts.restructure.elf32 import SHF_ALLOC, read_elf32, workspace_root

SHT_NOBITS = 8
SHT_REL = 9
R_386_32 = 1
R_386_GLOB_DAT = 6
R_386_JUMP_SLOT = 7
R_386_RELATIVE = 8
RELOCATION_NAMES = {
    R_386_32: "R_386_32",
    R_386_GLOB_DAT: "R_386_GLOB_DAT",
    R_386_JUMP_SLOT: "R_386_JUMP_SLOT",
    R_386_RELATIVE: "R_386_RELATIVE",
}


def _cstring(blob: bytes, start: int) -> str:
    end = blob.find(b"\0", start)
    return blob[start:end if end >= 0 else len(blob)].decode("utf-8", errors="replace")


def _dynamic_symbols(blob: bytes, sections: list[dict], symbol_table_index: int) -> list[dict]:
    table = sections[symbol_table_index]
    strings = sections[table["link"]]
    if table["entry_size"] < 16:
        raise ValueError("dynamic symbol table has invalid entry size")
    result = []
    for ordinal, offset in enumerate(range(table["offset"], table["offset"] + table["size"], table["entry_size"])):
        name_offset, value, size, info, other, section_index = struct.unpack_from("<IIIBBH", blob, offset)
        result.append({
            "index": ordinal,
            "name": _cstring(blob, strings["offset"] + name_offset) if name_offset else "",
            "address": value,
            "size": size,
            "type": info & 0xf,
            "binding": info >> 4,
            "section_index": section_index,
        })
    return result


def _allocated_section_at(sections: list[dict], address: int, width: int = 1) -> dict | None:
    matches = [section for section in sections if section["flags"] & SHF_ALLOC
               and section["address"] <= address
               and address + width <= section["address"] + section["size"]]
    return matches[0] if len(matches) == 1 else None


def _stored_word(blob: bytes, section: dict | None, address: int) -> int | None:
    if section is None or section["type"] == SHT_NOBITS:
        return None
    return struct.unpack_from("<I", blob, section["offset"] + address - section["address"])[0]


def relocation_target(kind: int, word: int | None, dynamic_symbol: dict | None) -> int | None:
    """Compute only targets fixed in the original file, never runtime imports."""
    if kind == R_386_RELATIVE:
        return word
    if dynamic_symbol is None or dynamic_symbol["section_index"] == 0:
        return None
    if kind == R_386_32 and word is not None:
        return (dynamic_symbol["address"] + word) & 0xffffffff
    if kind in (R_386_GLOB_DAT, R_386_JUMP_SLOT):
        return dynamic_symbol["address"]
    return None


def extract(path: Path) -> tuple[list[dict], dict]:
    blob = path.read_bytes()
    if blob[:6] != b"\x7fELF\x01\x01" or struct.unpack_from("<H", blob, 18)[0] != 3:
        raise ValueError("expected little-endian ELF32 i386")
    sections, symbols = read_elf32(path)
    exact_symbols: dict[int, list[int]] = defaultdict(list)
    objects_by_section: dict[int, list[dict]] = defaultdict(list)
    for symbol in symbols:
        section_index = symbol["section_index"]
        if not sections[section_index]["flags"] & SHF_ALLOC:
            continue
        exact_symbols[symbol["address"]].append(symbol["symbol_index"])
        if symbol["type"] == 1 and symbol["size"] > 0:
            objects_by_section[section_index].append(symbol)

    dynamic_tables = {}
    records = []
    by_type = Counter()
    by_site_section = Counter()
    for relsec in sections:
        if relsec["type"] != SHT_REL:
            continue
        if relsec["entry_size"] < 8:
            raise ValueError("relocation section has invalid entry size")
        if relsec["link"] not in dynamic_tables:
            dynamic_tables[relsec["link"]] = _dynamic_symbols(blob, sections, relsec["link"])
        dynamic = dynamic_tables[relsec["link"]]
        for ordinal, offset in enumerate(range(relsec["offset"], relsec["offset"] + relsec["size"], relsec["entry_size"])):
            site, info = struct.unpack_from("<II", blob, offset)
            kind, dynamic_index = info & 0xff, info >> 8
            site_section = _allocated_section_at(sections, site, 4)
            word = _stored_word(blob, site_section, site)
            dynamic_symbol = dynamic[dynamic_index] if dynamic_index < len(dynamic) else None
            target = relocation_target(kind, word, dynamic_symbol)
            target_section = _allocated_section_at(sections, target) if target is not None else None
            site_objects = [symbol["symbol_index"] for symbol in objects_by_section.get(site_section["index"], ())
                            if symbol["address"] <= site < symbol["address"] + symbol["size"]] if site_section else []
            name = RELOCATION_NAMES.get(kind, f"R_386_{kind}")
            section_name = site_section["name"] if site_section else None
            records.append({
                "id": f"{relsec['name']}:{ordinal}",
                "relocation_section": relsec["name"],
                "site_address": site,
                "site_section": section_name,
                "site_section_offset": site - site_section["address"] if site_section else None,
                "site_object_symbol_indices": site_objects,
                "kind": name,
                "dynamic_symbol_index": dynamic_index,
                "dynamic_symbol_name": dynamic_symbol["name"] if dynamic_symbol else None,
                "stored_addend": word if kind in (R_386_RELATIVE, R_386_32) else None,
                "target_address": target,
                "target_section": target_section["name"] if target_section else None,
                "target_exact_symbol_indices": sorted(exact_symbols.get(target, ())) if target is not None else [],
                "certainty": "relocation site and static addend; not TU ownership",
            })
            by_type[name] += 1
            by_site_section[section_name or "<unknown>"] += 1
    return records, {
        "original_dynamic_relocations": len(records),
        "original_relocations_by_type": dict(sorted(by_type.items())),
        "original_relocations_by_site_section": dict(sorted(by_site_section.items())),
        "original_relocations_with_containing_object": sum(bool(record["site_object_symbol_indices"]) for record in records),
        "original_relocations_with_exact_target_symbol": sum(bool(record["target_exact_symbol_indices"]) for record in records),
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("elf", nargs="?", type=Path, default=workspace_root() / "res/libTTapp.so")
    args = parser.parse_args()
    records, summary = extract(args.elf)
    print(json.dumps({"summary": summary, "relocations": records}, indent=2))


if __name__ == "__main__":
    main()
