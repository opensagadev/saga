#!/usr/bin/env python3
"""Compare linked .got order using uniquely named static relocation targets.

The score is intentionally conservative. It measures layout agreement among
comparable slots, not translation-unit ownership or function-body matching.
"""

from __future__ import annotations

import argparse
from bisect import bisect_left
from collections import Counter
import json
from pathlib import Path

from scripts.restructure.elf32 import SHF_ALLOC, SHT_SYMTAB, read_elf32, workspace_root
from scripts.restructure.original_relocations import extract


def got_target_order(sections: list[dict], symbols: list[dict], relocations: list[dict]) -> dict:
    """Select unambiguous R_386_RELATIVE targets in .got address order."""
    # Relocation target indices are table-local in the extraction ledger.
    # Refuse an ELF where separate SYMTABs could collide rather than pair
    # the wrong targets or silently discard a valid target.
    if sum(section["type"] == SHT_SYMTAB for section in sections) != 1:
        raise ValueError("expected exactly one static symbol table")
    got_sections = [section for section in sections if section["name"] == ".got"]
    if (len(got_sections) != 1 or got_sections[0]["size"] % 4
            or not got_sections[0]["flags"] & SHF_ALLOC):
        raise ValueError("expected one word-aligned .got section")
    got = got_sections[0]
    allocated = [
        symbol for symbol in symbols
        if symbol["section_index"] < len(sections)
        and sections[symbol["section_index"]]["flags"] & SHF_ALLOC
        and symbol["type"] in (1, 2)  # OBJECT or FUNC, not section/NOTYPE labels
    ]
    by_index = {symbol["symbol_index"]: symbol for symbol in allocated}
    name_counts = Counter(symbol["name"] for symbol in allocated)
    got_relocations = [record for record in relocations if record["site_section"] == ".got"]
    site_counts = Counter(record["site_address"] for record in got_relocations)
    keys = []
    valid_offsets = (record for record in got_relocations
                     if isinstance(record["site_section_offset"], int))
    for record in sorted(valid_offsets, key=lambda item: item["site_section_offset"]):
        site = record["site_address"]
        offset = record["site_section_offset"]
        if (record["kind"] != "R_386_RELATIVE" or site_counts[site] != 1
                or offset % 4
                or site != got["address"] + offset or offset < 0 or offset + 4 > got["size"]):
            continue
        targets = record["target_exact_symbol_indices"]
        if len(targets) != 1:
            continue
        target = by_index.get(targets[0])
        if (target is None or target["address"] != record["target_address"]
                or name_counts[target["name"]] != 1):
            continue
        section = sections[target["section_index"]]["name"]
        keys.append((target["name"], target["type"], target["binding"], section, target["size"]))
    key_counts = Counter(keys)
    unique = [key for key in keys if key_counts[key] == 1]
    return {
        "slots": got["size"] // 4,
        "relocations": len(got_relocations),
        "relative_relocations": sum(record["kind"] == "R_386_RELATIVE" for record in got_relocations),
        "unique_relative_targets": unique,
    }


def order_agreement(original: dict, current: dict) -> dict:
    """Report shared coverage separately from the longest in-order subset."""
    before = original["unique_relative_targets"]
    after = current["unique_relative_targets"]
    shared = set(before) & set(after)
    positions = {key: index for index, key in enumerate(after) if key in shared}
    tails: list[int] = []
    for key in before:
        position = positions.get(key)
        if position is None:
            continue
        slot = bisect_left(tails, position)
        if slot == len(tails):
            tails.append(position)
        else:
            tails[slot] = position
    return {
        "original_slots": original["slots"],
        "current_slots": current["slots"],
        "original_relocations": original["relocations"],
        "current_relocations": current["relocations"],
        "original_relative_relocations": original["relative_relocations"],
        "current_relative_relocations": current["relative_relocations"],
        "original_unique_targets": len(before),
        "current_unique_targets": len(after),
        "shared_targets": len(shared),
        "ordered_shared_targets": len(tails),
        "coverage_percent": 100.0 * len(shared) / original["slots"] if original["slots"] else 0.0,
        "order_percent": 100.0 * len(tails) / len(shared) if shared else 0.0,
    }


def compare(original: Path, current: Path) -> dict:
    original_sections, original_symbols = read_elf32(original)
    current_sections, current_symbols = read_elf32(current)
    original_relocations, _ = extract(original)
    current_relocations, _ = extract(current)
    return order_agreement(
        got_target_order(original_sections, original_symbols, original_relocations),
        got_target_order(current_sections, current_symbols, current_relocations),
    )


def main() -> None:
    root = workspace_root()
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--original", type=Path, default=root / "res/libTTapp.so")
    parser.add_argument("--current", type=Path, default=root / "bazel-out/k8-fastbuild/bin/src/libTTapp.so")
    parser.add_argument("--json", action="store_true", help="print machine-readable metrics")
    args = parser.parse_args()
    result = compare(args.original, args.current)
    if args.json:
        print(json.dumps(result, indent=2))
        return
    print(f".got slots: original {result['original_slots']:,}, current {result['current_slots']:,}")
    print(f"  Relocations in .got: {result['original_relocations']:,} original, "
          f"{result['current_relocations']:,} current "
          f"({result['original_relative_relocations']:,} and "
          f"{result['current_relative_relocations']:,} RELATIVE)")
    print(f"  Strict unique RELATIVE targets: {result['original_unique_targets']:,} original, "
          f"{result['current_unique_targets']:,} current")
    print(f"  Comparable target coverage: {result['shared_targets']:,}/"
          f"{result['original_slots']:,} original slots ({result['coverage_percent']:.1f}%)")
    print(f"  Shared-target order agreement: {result['ordered_shared_targets']:,}/"
          f"{result['shared_targets']:,} ({result['order_percent']:.1f}%)")
    print("This is a linked-layout diagnostic, not a TU boundary or body-matching score; "
          "imports and PLT slots are excluded.")


if __name__ == "__main__":
    main()
