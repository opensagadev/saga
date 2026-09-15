#!/usr/bin/env python3
"""Inventory original allocated symbols and evidence for translation-unit ownership.

This is an evidence ledger, not a claimed recovery of absent STT_FILE records.
Every named, defined, allocated symbol is retained, including aliases and
zero-sized symbols. Current-build comparison is optional and takes explicit
file paths; this tool never invokes a build system.
"""

from __future__ import annotations

import argparse
from collections import Counter, defaultdict
import json
from pathlib import Path
import re
import struct
import subprocess

from scripts.restructure.elf32 import SHF_ALLOC, SHN_COMMON, read_elf32, workspace_root
from scripts.restructure.inputs import read_units_manifest
from scripts.restructure.text_adjacency import text_adjacency_edges

EXCLUDED_TYPES = {3, 4}  # STT_SECTION, STT_FILE
CONSTRUCTOR = re.compile(r"^_GLOBAL__sub_I_(.+)$")
EMBEDDED_PATH = re.compile(rb"([A-Za-z]:/[A-Za-z0-9_./-]+\.(?:cpp|c))(?::\d+)?\x00")
STRONG_LOCAL_XREF = "strong same-TU constraint; not an assignment"


def allocated_symbols(sections: list[dict], symbols: list[dict]) -> list[dict]:
    """Keep original symbol-table identity, even for aliases at one address."""
    return [
        symbol
        for symbol in symbols
        if symbol["section_index"] < len(sections)
        and sections[symbol["section_index"]]["flags"] & SHF_ALLOC
        and symbol["type"] not in EXCLUDED_TYPES
    ]


def current_unit_symbols(sections: list[dict], symbols: list[dict]) -> list[dict]:
    """Include tentative COMMON definitions when attributing a current object."""
    return [
        symbol for symbol in symbols
        if symbol["type"] not in EXCLUDED_TYPES
        and (symbol["section_index"] == SHN_COMMON
             or (symbol["section_index"] < len(sections)
                 and sections[symbol["section_index"]]["flags"] & SHF_ALLOC))
    ]


def alias_groups(symbols: list[dict]) -> tuple[list[dict], dict[int, int]]:
    """Identify same-location/same-size/type entries without collapsing them."""
    by_location: dict[tuple[int, int, int, int], list[int]] = defaultdict(list)
    for symbol in symbols:
        key = (
            symbol["section_index"],
            symbol["address"],
            symbol["size"],
            symbol["type"],
        )
        by_location[key].append(symbol["symbol_index"])
    groups = []
    assignments = {}
    for key, members in by_location.items():
        if len(members) < 2:
            continue
        group_id = len(groups)
        groups.append({"id": group_id, "location": list(key), "symbol_indices": members})
        for symbol_index in members:
            assignments[symbol_index] = group_id
    return groups, assignments


def local_initializer_blocks(symbols: list[dict]) -> tuple[list[dict], dict[int, int]]:
    """Partition local symtab order at named initializer sentinels.

    A block can contain symbols from initializer-free units. Its basename is
    evidence for its own locals, not a proven owner for the entire block.
    """
    local = [symbol for symbol in symbols if symbol["binding"] == 0]
    blocks = []
    assignments = {}
    start = 0
    for offset, symbol in enumerate(local):
        match = CONSTRUCTOR.match(symbol["name"])
        if match is None:
            continue
        block_id = len(blocks)
        members = local[start : offset + 1]
        blocks.append(
            {
                "id": block_id,
                "initializer": symbol["name"],
                "basename": match.group(1),
                "first_symbol_index": members[0]["symbol_index"],
                "last_symbol_index": symbol["symbol_index"],
                "symbol_count": len(members),
                "certainty": "initializer-delimited; may contain other units",
            }
        )
        for member in members:
            assignments[member["symbol_index"]] = block_id
        start = offset + 1
    if start < len(local):
        block_id = len(blocks)
        members = local[start:]
        blocks.append(
            {
                "id": block_id,
                "initializer": None,
                "basename": None,
                "first_symbol_index": members[0]["symbol_index"],
                "last_symbol_index": members[-1]["symbol_index"],
                "symbol_count": len(members),
                "certainty": "undelimited tail",
            }
        )
        for member in members:
            assignments[member["symbol_index"]] = block_id
    return blocks, assignments


def original_build_clues(
    original: Path, sections: list[dict], symbols: list[dict]
) -> tuple[list[dict], list[str]]:
    """Read constructor order and literal source paths without guessing owners."""
    data = original.read_bytes()
    byte_order = {1: "<", 2: ">"}[data[5]]
    by_address = defaultdict(list)
    for symbol in symbols:
        by_address[symbol["address"]].append(symbol)
    initializers = []
    for section in sections:
        if section["name"] != ".init_array":
            continue
        contents = data[section["offset"] : section["offset"] + section["size"]]
        for ordinal, (address,) in enumerate(struct.iter_unpack(byte_order + "I", contents)):
            initializers.append(
                {
                    "ordinal": ordinal,
                    "address": address,
                    "symbols": [
                        {"name": symbol["name"], "symbol_index": symbol["symbol_index"]}
                        for symbol in by_address[address]
                    ],
                }
            )
    paths = sorted(
        {
            match.group(1).decode("ascii")
            for match in EMBEDDED_PATH.finditer(data)
        }
    )
    return initializers, paths


def function_local_anchors(symbols: list[dict]) -> dict[int, list[int]]:
    """Link Itanium function-local symbols to parent function names, if unique.

    This is a lexical relationship in symbol names. It does not by itself
    assign either symbol to a translation unit.
    """
    relevant = [
        symbol
        for symbol in symbols
        if symbol["type"] == 2 or symbol["name"].startswith("_ZZ")
    ]
    result = subprocess.run(
        ["c++filt"],
        input="\n".join(symbol["name"] for symbol in relevant) + "\n",
        text=True,
        capture_output=True,
        check=True,
    )
    names = result.stdout.splitlines()
    if len(names) != len(relevant):
        raise RuntimeError("c++filt returned an unexpected number of symbol names")
    functions: dict[str, list[int]] = defaultdict(list)
    for symbol, demangled in zip(relevant, names):
        if symbol["type"] == 2:
            functions[demangled].append(symbol["symbol_index"])
    anchors = {}
    for symbol, demangled in zip(relevant, names):
        if symbol["name"].startswith("_ZZ") and "::" in demangled:
            parent = demangled.rsplit("::", 1)[0]
            if parent in functions:
                anchors[symbol["symbol_index"]] = functions[parent]
    return anchors


def annotate_local_xrefs(
    edges: list[dict], mapped: list[dict], current_units: list[dict] | None = None
) -> tuple[list[dict], dict]:
    """Add ledger metadata; current candidates remain diagnostics, never owners."""
    by_index = {symbol["symbol_index"]: symbol for symbol in mapped}
    units_by_id = {unit["id"]: unit for unit in current_units or []}
    annotated = []
    strengths = Counter()
    assessable = discordant = size_concordant_discordant = 0
    for edge in edges:
        targets = [by_index[index] for index in edge["object_symbol_indices"]]
        sections = sorted({target["section"] for target in targets})
        bindings = sorted({target["binding"] for target in targets})
        if len(sections) == 1 and sections[0] in (".data", ".bss", ".tdata", ".tbss"):
            strength = STRONG_LOCAL_XREF
        elif sections == [".rodata"]:
            strength = "medium same-TU evidence; read-only constants may be pooled"
        else:
            strength = "unclassified reference; no TU constraint"
        source = by_index[edge["function_symbol_index"]]
        source_candidates = source.get("current_owner_candidates", [])
        target_candidates = targets[0].get("current_owner_candidates", []) if len(targets) == 1 else []
        target_size_concordant = None
        if len(source_candidates) == len(target_candidates) == 1:
            assessable += 1
            is_discordant = source_candidates[0] != target_candidates[0]
            discordant += is_discordant
            candidate_unit = units_by_id.get(target_candidates[0])
            if candidate_unit is not None:
                original_target = targets[0]
                target_size_concordant = any(
                    symbol["name"] == original_target["name"]
                    and symbol["type"] == original_target["type"]
                    and symbol["size"] == original_target["size"]
                    and (symbol["binding"] == 0) == (original_target["binding"] == 0)
                    for symbol in candidate_unit["symbols"]
                )
                size_concordant_discordant += is_discordant and target_size_concordant
        annotated.append({**edge,
                          "id": f"original-local-xref:{edge['function_symbol_index']}:{edge['instruction_address']:08x}:{edge['object_address']:08x}",
                          "target_sections": sections, "target_bindings": bindings,
                          "same_tu_evidence": strength,
                          "current_target_object_size_concordant": target_size_concordant})
        strengths[strength] += 1
    return annotated, {
        "original_local_xrefs": len(annotated),
        "original_local_xrefs_by_strength": dict(sorted(strengths.items())),
        "original_local_xrefs_alias_ambiguous": sum(edge["object_alias_ambiguous"] for edge in annotated),
        "current_cross_owner_diagnostic_assessable": assessable,
        "current_cross_owner_diagnostic_discordant": discordant,
        "current_cross_owner_diagnostic_discordant_object_size_concordant": size_concordant_discordant,
    }


def local_xref_components(edges: list[dict], mapped: list[dict]) -> tuple[list[dict], dict]:
    """Find minimum same-TU groups implied by verified writable LOCAL refs.

    These groups contain only reached symbols, not the complete owning TUs.
    Read-only references and unresolved address aliases cannot join groups.
    """
    by_index = {symbol["symbol_index"]: symbol for symbol in mapped}
    parent: dict[int, int] = {}
    participating_edges = []

    def root(index: int) -> int:
        parent.setdefault(index, index)
        while parent[index] != index:
            parent[index] = parent[parent[index]]
            index = parent[index]
        return index

    for edge in edges:
        targets = edge["object_symbol_indices"]
        if edge["same_tu_evidence"] != STRONG_LOCAL_XREF or len(targets) != 1:
            continue
        function_index = edge["function_symbol_index"]
        object_index = targets[0]
        if function_index not in by_index or object_index not in by_index:
            continue
        parent[root(object_index)] = root(function_index)
        participating_edges.append(edge)

    members: dict[int, set[int]] = defaultdict(set)
    edge_ids: dict[int, set[str]] = defaultdict(set)
    for index in parent:
        members[root(index)].add(index)
    for edge in participating_edges:
        edge_ids[root(edge["function_symbol_index"])].add(edge["id"])

    components = []
    for indices in sorted(members.values(), key=lambda group: min(group)):
        ordered = sorted(indices)
        blocks = sorted({by_index[index]["local_initializer_block"] for index in ordered
                         if by_index[index].get("local_initializer_block") is not None})
        component_root = root(ordered[0])
        components.append({
            "id": len(components),
            "symbol_indices": ordered,
            "function_symbol_indices": [index for index in ordered if by_index[index]["type"] == 2],
            "object_symbol_indices": [index for index in ordered if by_index[index]["type"] == 1],
            "xref_ids": sorted(edge_ids[component_root]),
            "local_initializer_blocks": blocks,
            "certainty": "minimum same-TU constraint; not a complete TU or source assignment",
        })
    return components, {
        "strong_local_xref_components": len(components),
        "strong_local_xref_component_symbols": sum(len(group["symbol_indices"]) for group in components),
        "strong_local_xref_components_crossing_initializer_blocks": sum(
            len(group["local_initializer_blocks"]) > 1 for group in components
        ),
    }


def build_map(
    original: Path, current: Path | None = None, units: list[dict] | None = None,
    *, with_local_xrefs: bool = False, with_relocations: bool = False,
) -> dict:
    if (current is None) != (units is None):
        raise ValueError("current ELF and unit manifest must be supplied together")
    original_sections, original_all = read_elf32(original)
    original_symbols = allocated_symbols(original_sections, original_all)
    aliases, alias_assignment = alias_groups(original_symbols)
    current_symbols = []
    if current is not None:
        current_sections, current_all = read_elf32(current)
        current_symbols = allocated_symbols(current_sections, current_all)
    blocks, local_blocks = local_initializer_blocks(original_all)
    local_anchors = function_local_anchors(original_symbols)
    initializers, embedded_paths = original_build_clues(
        original, original_sections, original_all
    )

    unit_records = []
    name_owners: dict[tuple[str, int, int], set[int]] = defaultdict(set)
    for unit_id, unit in enumerate(units or []):
        sections, all_symbols = read_elf32(unit["object_path"], include_common_symbols=True)
        object_symbols = current_unit_symbols(sections, all_symbols)
        entries = []
        for symbol in object_symbols:
            entries.append(
                {
                    **symbol,
                    "section": "COMMON" if symbol["section_index"] == SHN_COMMON
                    else sections[symbol["section_index"]]["name"],
                }
            )
            # A local symbol never establishes cross-TU ownership by name.
            binding_class = 0 if symbol["binding"] == 0 else 1
            name_owners[(symbol["name"], binding_class, symbol["type"])].add(unit_id)
        unit_records.append(
            {
                "id": unit_id,
                "source": unit["source"],
                "object": unit["object"],
                "optimization": unit["optimization"],
                "symbols": entries,
            }
        )

    mapped = []
    candidate_counts = Counter()
    candidate_by_section: dict[str, Counter] = defaultdict(Counter)
    for symbol in original_symbols:
        entry = {
            **symbol,
            "section": original_sections[symbol["section_index"]]["name"],
            "local_initializer_block": local_blocks.get(symbol["symbol_index"])
            if symbol["binding"] == 0
            else None,
            "alias_group": alias_assignment.get(symbol["symbol_index"]),
            "containing_function_symbols": local_anchors.get(
                symbol["symbol_index"], []
            ),
        }
        if current is not None:
            binding_class = 0 if symbol["binding"] == 0 else 1
            candidates = sorted(
                name_owners.get((symbol["name"], binding_class, symbol["type"]), ())
            )
            if symbol["binding"] == 0:
                evidence = "local-name-only; requires independent corroboration"
            else:
                evidence = "global-name-match; current owner, not original TU proof"
            candidate_class = (
                "unique" if len(candidates) == 1 else "ambiguous" if candidates else "none"
            )
            candidate_counts[candidate_class] += 1
            candidate_by_section[entry["section"]][candidate_class] += 1
            entry["current_owner_candidates"] = candidates
            entry["candidate_evidence"] = (
                evidence if candidates else "no same-name current symbol"
            )
        mapped.append(entry)

    original_section_counts = Counter(symbol["section"] for symbol in mapped)
    text_edges = text_adjacency_edges(mapped)
    summary = {
        "original_allocated_symbols": len(mapped),
        "initializer_delimited_blocks": len(blocks),
        "init_array_entries": len(initializers),
        "embedded_source_paths": len(embedded_paths),
        "function_local_static_anchors": len(local_anchors),
        "alias_groups": len(aliases),
        "original_text_adjacency_edges": len(text_edges),
        "original_by_section": dict(sorted(original_section_counts.items())),
    }
    if current is not None:
        summary.update(
            {
                "current_allocated_symbols": len(current_symbols),
                "current_target_units": len(unit_records),
                "current_candidate_counts": dict(sorted(candidate_counts.items())),
                "current_candidates_by_original_section": {
                    section: dict(sorted(counts.items()))
                    for section, counts in sorted(candidate_by_section.items())
                },
            }
        )
    local_xrefs = None
    xref_components = None
    if with_local_xrefs:
        # Keep baseline inventory dependency-free; Capstone is only needed for
        # this explicitly requested original-binary disassembly pass.
        from scripts.restructure.original_local_xrefs import extract

        local_xrefs, xref_summary = annotate_local_xrefs(extract(original), mapped, unit_records)
        summary.update(xref_summary)
        xref_components, component_summary = local_xref_components(local_xrefs, mapped)
        summary.update(component_summary)
    inventory = {
        "schema_version": 1,
        "rules": {
            "inclusion": "named defined SHT_SYMTAB symbols in SHF_ALLOC sections, excluding STT_SECTION and STT_FILE",
            "local_blocks": "symtab-local-order segments ending at _GLOBAL__sub_I_; not necessarily complete TUs",
            "current_candidates": "same-name, same-type object symbols of the same local/nonlocal binding class; not original TU assignment",
            "aliases": "preserved as separate entries by symbol_table and symbol_index",
            "text_adjacency_edges": "individual original .text function-site gaps of 0..16 bytes; weak layout evidence only, never TU assignments or clusters",
        },
        "original": str(original),
        "current": str(current) if current is not None else None,
        "summary": summary,
        "original_local_blocks": blocks,
        "original_alias_groups": aliases,
        "original_text_adjacency_edges": text_edges,
        "original_initializers": initializers,
        "original_embedded_paths": embedded_paths,
        "original_symbols": mapped,
        "current_units": unit_records,
    }
    if with_local_xrefs:
        inventory["rules"]["original_local_xrefs"] = (
            "exact original ELF i386 PIC instruction/REL evidence to LOCAL object addresses; "
            "non-const state is a same-TU constraint, .rodata is weaker due to pooling; "
            "current-owner disagreement is diagnostic only, never original TU assignment"
        )
        inventory["original_local_xrefs"] = local_xrefs
        inventory["original_strong_local_xref_components"] = xref_components
    if with_relocations:
        from scripts.restructure.original_relocations import extract as extract_relocations

        relocations, relocation_summary = extract_relocations(original)
        inventory["rules"]["original_dynamic_relocations"] = (
            "exact original ELF dynamic relocation sites and static pointer targets; "
            "containing/exact symbols are positional evidence only, never TU ownership"
        )
        inventory["original_dynamic_relocations"] = relocations
        summary.update(relocation_summary)
    return inventory


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--original", type=Path)
    parser.add_argument("--current", type=Path)
    parser.add_argument("--units", type=Path, help="JSON source/object manifest")
    parser.add_argument("--output", type=Path)
    parser.add_argument("--with-local-xrefs", action="store_true", help="add verified original i386 LOCAL references (requires Capstone)")
    parser.add_argument("--with-relocations", action="store_true", help="add original GOT/data dynamic relocation sites and static pointer targets")
    args = parser.parse_args()
    if (args.current is None) != (args.units is None):
        parser.error("--current and --units must be supplied together")
    root = workspace_root()
    original = args.original or root / "res/libTTapp.so"
    output = args.output or root / ".work/original-tu-map.json"
    units = read_units_manifest(args.units, root) if args.units else None
    inventory = build_map(
        original.resolve(), args.current.resolve() if args.current else None, units,
        with_local_xrefs=args.with_local_xrefs,
        with_relocations=args.with_relocations,
    )
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(inventory, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(inventory["summary"], indent=2))
    print(f"Wrote {output}")


if __name__ == "__main__":
    main()
