#!/usr/bin/env python3
"""Compare linked ELF symbol positions without assuming original TU ownership.

Only a unique name/type/binding match on both sides is compared. In
particular, duplicate file-local names are reported as ambiguous rather than
assigned by proximity. Absolute virtual addresses, section offsets, and
offsets from the first selected same-section symbol are distinct measurements;
none proves that the two binaries have the same linker layout.
"""

from __future__ import annotations

import argparse
from bisect import bisect_left
from collections import Counter, defaultdict
from difflib import unified_diff
import json
from pathlib import Path
import re

from scripts.restructure.elf32 import read_elf32, workspace_root
from scripts.restructure.calibrate_tu_map import address_contiguity
from scripts.restructure.generate_original_tu_map import STRONG_LOCAL_XREF, allocated_symbols, build_map
from scripts.restructure.inputs import read_units_manifest


def symbol_key(symbol: dict) -> tuple[str, int, int]:
    return symbol["name"], symbol["type"], symbol["binding"]


def indexed_symbols(sections: list[dict], symbols: list[dict]) -> list[dict]:
    return [
        {
            **symbol,
            "section": sections[symbol["section_index"]]["name"],
            "section_offset": symbol["address"] - sections[symbol["section_index"]]["address"],
        }
        for symbol in allocated_symbols(sections, symbols)
    ]


def pair_symbols(
    original: list[dict], current: list[dict], selected_keys: set | None = None,
    original_predicate=None,
) -> tuple[list[dict], dict]:
    """Pair only unambiguous linked symbols; retain ambiguity counts."""
    original_by_key: dict[tuple, list[dict]] = defaultdict(list)
    current_by_key: dict[tuple, list[dict]] = defaultdict(list)
    for symbol in original:
        original_by_key[symbol_key(symbol)].append(symbol)
    for symbol in current:
        current_by_key[symbol_key(symbol)].append(symbol)

    rows = []
    counts = Counter()
    for key, originals in original_by_key.items():
        if selected_keys is not None and key not in selected_keys:
            continue
        selected = [symbol for symbol in originals if original_predicate is None or original_predicate(symbol)]
        if not selected:
            continue
        currents = current_by_key.get(key, [])
        if not currents:
            counts["unmatched_original"] += len(selected)
        elif len(originals) != 1 or len(currents) != 1:
            counts["ambiguous_original"] += len(selected)
        else:
            before, after = originals[0], currents[0]
            rows.append({
                "name": before["name"],
                "type": before["type"],
                "binding": before["binding"],
                "size_original": before["size"],
                "size_current": after["size"],
                "section_original": before["section"],
                "section_current": after["section"],
                "address_original": before["address"],
                "address_current": after["address"],
                "absolute_delta": after["address"] - before["address"],
                "section_offset_original": before["section_offset"],
                "section_offset_current": after["section_offset"],
                "section_offset_delta": after["section_offset"] - before["section_offset"],
            })
            counts["paired"] += 1
    return rows, dict(counts)


def relative_positions(rows: list[dict]) -> tuple[list[dict], dict]:
    """Compare order and spacing inside each selected section pair."""
    groups: dict[tuple[str, str], list[dict]] = defaultdict(list)
    for row in rows:
        groups[(row["section_original"], row["section_current"])].append(row)
    order_breaks = 0
    inversions = 0
    comparable_pairs = 0
    rank_displaced = 0
    section_moves = 0
    for (original_section, current_section), group in groups.items():
        section_moves += len(group) if original_section != current_section else 0
        group.sort(key=lambda row: (row["address_original"], row["name"]))
        for rank, row in enumerate(sorted(group, key=lambda row: (row["address_current"], row["name"]))):
            row["current_rank"] = rank
        tree = [0] * (len(group) + 1)

        def prefix(count: int) -> int:
            total = 0
            while count:
                total += tree[count]
                count -= count & -count
            return total

        first = group[0]
        previous = None
        comparable_pairs += len(group) * (len(group) - 1) // 2
        for rank, row in enumerate(group):
            row["original_rank"] = rank
            row["rank_delta"] = row["current_rank"] - rank
            rank_displaced += row["rank_delta"] != 0
            inversions += rank - prefix(row["current_rank"] + 1)
            cursor = row["current_rank"] + 1
            while cursor < len(tree):
                tree[cursor] += 1
                cursor += cursor & -cursor
            row["relative_original"] = row["address_original"] - first["address_original"]
            row["relative_current"] = row["address_current"] - first["address_current"]
            row["relative_delta"] = row["relative_current"] - row["relative_original"]
            if previous is not None:
                row["original_gap"] = row["address_original"] - previous["address_original"]
                row["current_gap"] = row["address_current"] - previous["address_current"]
                row["gap_delta"] = row["current_gap"] - row["original_gap"]
                if row["current_gap"] < 0:
                    order_breaks += 1
            previous = row
    return sorted(rows, key=lambda row: (row["address_original"], row["name"])), {
        "section_pairs": len(groups),
        "section_moves": section_moves,
        "adjacent_order_breaks": order_breaks,
        "pairwise_inversions": inversions,
        "comparable_pairs": comparable_pairs,
        "rank_displaced": rank_displaced,
    }


def parse_address(value: str) -> int:
    return int(value, 0)


def source_symbols(units_path: Path, source: str, root: Path) -> list[dict]:
    matches = [unit for unit in read_units_manifest(units_path, root) if unit["source"] == source]
    if len(matches) != 1:
        raise ValueError(f"expected one current unit for {source!r}; found {len(matches)}")
    sections, symbols = read_elf32(matches[0]["object_path"])
    return indexed_symbols(sections, symbols)


def ordered_symbols(symbols: list[dict]) -> list[dict]:
    return sorted(symbols, key=lambda symbol: (symbol["address"], symbol["name"], symbol["symbol_index"]))


def current_order_symbols(current: list[dict], keys: set[tuple], sections: set[str]) -> list[dict]:
    """Only attribute unique linked names to a selected object or original range."""
    counts = Counter(symbol_key(symbol) for symbol in current)
    return ordered_symbols([symbol for symbol in current if symbol["section"] in sections and
                            symbol_key(symbol) in keys and
                            counts[symbol_key(symbol)] == 1])


def order_token(symbol: dict) -> str:
    return symbol["name"]


def display_symbol(symbol: dict) -> str:
    binding = {0: "LOCAL", 1: "GLOBAL", 2: "WEAK"}.get(symbol["binding"], str(symbol["binding"]))
    kind = {1: "OBJECT", 2: "FUNC"}.get(symbol["type"], str(symbol["type"]))
    return f"{binding:<6} {kind:<6} {symbol['name']}"


def is_assembler_label(symbol: dict) -> bool:
    return bool(re.fullmatch(r"\.L\d+", symbol["name"]))


def order_diff_lines(original: list[dict], current: list[dict]) -> list[str]:
    return list(unified_diff(
        [order_token(symbol) + "\n" for symbol in original],
        [order_token(symbol) + "\n" for symbol in current],
        fromfile="original-address-order", tofile="current-address-order",
        n=3,
    ))


def lcs_length(original: list[str], current: list[str]) -> int:
    """Exact LCS length using symbol-position lists, including repeated names."""
    positions: dict[str, list[int]] = defaultdict(list)
    for index, name in enumerate(current):
        positions[name].append(index)
    tails = []
    for name in original:
        for position in reversed(positions.get(name, [])):
            slot = bisect_left(tails, position)
            if slot == len(tails):
                tails.append(position)
            else:
                tails[slot] = position
    return len(tails)


def order_alignment(original: list[dict], current: list[dict], *, unique_only: bool = False) -> dict:
    """Ordered-symbol F1: 2*LCS/(original count + current count)."""
    before = [f"{symbol['type']}:{symbol['name']}" for symbol in original]
    after = [f"{symbol['type']}:{symbol['name']}" for symbol in current]
    counts_before, counts_after = Counter(before), Counter(after)
    common = sum((counts_before & counts_after).values())
    if unique_only:
        eligible = {name for name in counts_before.keys() & counts_after.keys()
                    if counts_before[name] == counts_after[name] == 1}
        matched_before = [name for name in before if name in eligible]
        matched_after = [name for name in after if name in eligible]
    else:
        eligible = None
        matched_before, matched_after = before, after
    ordered = lcs_length(matched_before, matched_after)
    denominator = len(before) + len(after)
    return {
        "original": len(before),
        "current": len(after),
        "common": common,
        "eligible_unique": len(eligible) if eligible is not None else None,
        "ordered": ordered,
        "percent": 100.0 * (2 * ordered / denominator if denominator else 1.0),
    }


def source_order_alignment(original: list[dict], current: list[dict], paired_count: int) -> dict:
    """Avoid a false zero when input sections differ from linked output sections."""
    metric = order_alignment(original, current)
    metric["comparable"] = metric["common"] >= paired_count
    return metric


def substantive(symbols: list[dict]) -> list[dict]:
    return [symbol for symbol in symbols if symbol["type"] in {1, 2} and not is_assembler_label(symbol)]


def overall_order_alignment(original: list[dict], current: list[dict]) -> dict:
    """Conservative whole-image proxy for text, data, and BSS sequences."""
    sections = {}
    for section in (".text", ".data", ".bss"):
        before = substantive(ordered_symbols([symbol for symbol in original if symbol["section"] == section]))
        after = substantive(ordered_symbols([symbol for symbol in current if symbol["section"] == section]))
        sections[section] = order_alignment(before, after, unique_only=True)
    original_count = sum(item["original"] for item in sections.values())
    current_count = sum(item["current"] for item in sections.values())
    ordered_count = sum(item["ordered"] for item in sections.values())
    return {
        "sections": sections,
        "original": original_count,
        "current": current_count,
        "ordered": ordered_count,
        "percent": 100.0 * (2 * ordered_count / (original_count + current_count)
                            if original_count + current_count else 1.0),
    }


def original_identity_key(symbol: dict) -> tuple | None:
    fields = ("name", "type", "binding")
    if not all(field in symbol for field in fields):
        return None
    return symbol["name"], symbol["type"], 0 if symbol["binding"] == 0 else 1


def original_identity_counts(symbols: dict[int, dict]) -> Counter:
    """Use the same name/type/binding-class key as current owner lookup."""
    return Counter(key for symbol in symbols.values() if (key := original_identity_key(symbol)) is not None)


def uniquely_identified_original(symbol: dict, identities: Counter) -> bool:
    key = original_identity_key(symbol)
    return key is None or identities[key] == 1


def same_tu_constraints(ledger: dict, start: int | None = None, end: int | None = None) -> dict:
    """Measure distinct strong original function-to-local-state owner pairs."""
    symbols = {symbol["symbol_index"]: symbol for symbol in ledger["original_symbols"]}
    identities = original_identity_counts(symbols)
    relationships = {}
    for edge in ledger["original_local_xrefs"]:
        if edge["same_tu_evidence"] != STRONG_LOCAL_XREF or len(edge["object_symbol_indices"]) != 1:
            continue
        source = symbols[edge["function_symbol_index"]]
        if start is not None and source["address"] < start:
            continue
        if end is not None and source["address"] >= end:
            continue
        relationship = (edge["function_symbol_index"], edge["object_symbol_indices"][0])
        relationships[relationship] = bool(edge.get("current_target_object_size_concordant"))
    assessable = satisfied = 0
    for (source_index, target_index), size_concordant in relationships.items():
        source = symbols[source_index]
        target = symbols[target_index]
        if (not uniquely_identified_original(source, identities)
                or not uniquely_identified_original(target, identities)):
            continue
        source_owners = source.get("current_owner_candidates", [])
        target_owners = target.get("current_owner_candidates", [])
        if len(source_owners) != 1 or len(target_owners) != 1 or not size_concordant:
            continue
        assessable += 1
        satisfied += source_owners[0] == target_owners[0]
    return {
        "total": len(relationships),
        "assessable": assessable,
        "satisfied": satisfied,
        "percent": 100.0 * satisfied / assessable if assessable else None,
        "coverage_percent": 100.0 * assessable / len(relationships) if relationships else None,
    }


def same_tu_owner_splits(ledger: dict, limit: int = 5) -> list[dict]:
    """Rank verifiable source/object splits without treating names as TU proof."""
    symbols = {symbol["symbol_index"]: symbol for symbol in ledger["original_symbols"]}
    identities = original_identity_counts(symbols)
    units = {unit["id"]: unit["source"] for unit in ledger.get("current_units", [])}
    groups: dict[tuple[int, int], set[tuple[int, int]]] = defaultdict(set)
    for edge in ledger["original_local_xrefs"]:
        if (edge["same_tu_evidence"] != STRONG_LOCAL_XREF
                or len(edge["object_symbol_indices"]) != 1
                or not edge.get("current_target_object_size_concordant")):
            continue
        function_index = edge["function_symbol_index"]
        object_index = edge["object_symbol_indices"][0]
        function_owners = symbols[function_index].get("current_owner_candidates", [])
        object_owners = symbols[object_index].get("current_owner_candidates", [])
        if (not uniquely_identified_original(symbols[function_index], identities)
                or not uniquely_identified_original(symbols[object_index], identities)):
            continue
        if len(function_owners) != 1 or len(object_owners) != 1:
            continue
        if function_owners[0] != object_owners[0]:
            groups[(function_owners[0], object_owners[0])].add((function_index, object_index))
    ranked = sorted(groups.items(), key=lambda item: (-len(item[1]), item[0]))[:limit]
    return [{"function_source": units.get(owners[0], f"unit {owners[0]}"),
             "object_source": units.get(owners[1], f"unit {owners[1]}"),
             "pairs": len(pairs)} for owners, pairs in ranked]


def original_component(ledger: dict, component_id: int) -> dict:
    """Resolve a strong-local component through ELF symbol IDs, not list offsets."""
    components = ledger.get("original_strong_local_xref_components", [])
    component = next((item for item in components if item["id"] == component_id), None)
    if component is None:
        raise ValueError(f"no original strong-local component with ID {component_id}")
    by_id = {symbol["symbol_index"]: symbol for symbol in ledger["original_symbols"]}
    identities = original_identity_counts(by_id)
    units = {unit["id"]: unit["source"] for unit in ledger.get("current_units", [])}
    global_objects = defaultdict(set)
    for unit in ledger.get("current_units", []):
        for symbol in unit.get("symbols", []):
            if symbol["type"] == 1 and symbol["binding"] == 1:
                global_objects[(symbol["name"], symbol["size"], symbol["section"])].add(unit["source"])

    def global_counterpart(symbol: dict, owners: list[int]) -> str | None:
        if owners or symbol["type"] != 1:
            return None
        name = re.fullmatch(r"_ZL(\d+)([A-Za-z_]\w*)", symbol["name"])
        if name is None or len(name.group(2)) != int(name.group(1)):
            return None
        candidates = global_objects.get((name.group(2), symbol["size"], symbol["section"]), set())
        return next(iter(candidates)) if len(candidates) == 1 else None

    def entries(indices: list[int]) -> list[dict]:
        result = []
        for symbol_id in indices:
            symbol = by_id[symbol_id]
            owners = symbol.get("current_owner_candidates", [])
            result.append({
                "symbol_index": symbol_id,
                "name": symbol["name"],
                "address": symbol["address"],
                "section": symbol["section"],
                "size": symbol["size"],
                "owner": units.get(owners[0], f"unit {owners[0]}") if len(owners) == 1 else None,
                "candidate_count": len(owners),
                "original_identity_unique": uniquely_identified_original(symbol, identities),
                "global_counterpart": global_counterpart(symbol, owners),
            })
        return sorted(result, key=lambda item: (item["address"], item["symbol_index"]))

    functions = entries(component["function_symbol_indices"])
    objects = entries(component["object_symbol_indices"])
    function_by_id = {symbol["symbol_index"]: symbol for symbol in functions}
    object_by_id = {symbol["symbol_index"]: symbol for symbol in objects}
    xrefs = {xref["id"]: xref for xref in ledger.get("original_local_xrefs", [])}
    state_pairs = {}
    for xref_id in component.get("xref_ids", []):
        xref = xrefs.get(xref_id)
        if xref is None or xref.get("object_alias_ambiguous") or not xref.get("same_tu_evidence", "").startswith("strong"):
            continue
        function = function_by_id.get(xref["function_symbol_index"])
        for object_id in xref["object_symbol_indices"]:
            obj = object_by_id.get(object_id)
            if function is None or obj is None:
                continue
            if (not uniquely_identified_original(by_id[function["symbol_index"]], identities)
                    or not uniquely_identified_original(by_id[object_id], identities)):
                continue
            object_owner = obj["owner"] or obj["global_counterpart"]
            state_pairs[(function["symbol_index"], object_id)] = {
                "resolved": function["owner"] is not None and object_owner is not None,
                "split": function["owner"] is not None and object_owner is not None
                         and function["owner"] != object_owner,
                "provisional": obj["owner"] is None and obj["global_counterpart"] is not None,
                "function_name": function["name"],
                "function_owner": function["owner"],
                "object_owner": object_owner,
            }

    split_functions = Counter((pair["function_name"], pair["function_owner"], pair["object_owner"])
                              for pair in state_pairs.values() if pair["split"])

    return {
        "id": component_id,
        "certainty": component["certainty"],
        "initializer_blocks": component["local_initializer_blocks"],
        "functions": functions,
        "objects": objects,
        "local_state_pairs": {
            "total": len(state_pairs),
            "resolved": sum(pair["resolved"] for pair in state_pairs.values()),
            "split": sum(pair["split"] for pair in state_pairs.values()),
            "provisional": sum(pair["resolved"] and pair["provisional"] for pair in state_pairs.values()),
            "top_split_functions": [
                {"name": name, "function_owner": function_owner, "object_owner": object_owner, "pairs": count}
                for (name, function_owner, object_owner), count in
                sorted(split_functions.items(), key=lambda item: (-item[1], item[0]))[:5]
            ],
        },
    }


def print_original_component(component: dict, limit: int = 0) -> None:
    functions = component["functions"]
    objects = component["objects"]
    first = min((symbol["address"] for symbol in functions), default=0)
    last = max((symbol["address"] for symbol in functions), default=0)
    print(f"Original strong-local component {component['id']}: "
          f"{len(functions)} functions, {len(objects)} LOCAL objects; "
          f"text symbols 0x{first:x}–0x{last:x}")
    print(f"  Initializer blocks: {component['initializer_blocks']}; {component['certainty']}")
    duplicate_count = sum(not symbol["original_identity_unique"] for symbol in functions + objects)
    if duplicate_count:
        print(f"  {duplicate_count} symbols reuse an original name/type/binding identity; "
              "their local-state links are excluded from split counts")
    pairs = component.get("local_state_pairs", {})
    if pairs.get("total", 0):
        print(f"  Writable-state candidate links: {pairs['resolved']}/{pairs['total']} source-assigned, "
              f"{pairs['split']} cross-source; {pairs['provisional']} use same-name/size/section "
              "GLOBAL counterparts (verify identities before migration)")
        for item in pairs.get("top_split_functions", []):
            print(f"    {item['pairs']} split pairs: {item['name']} "
                  f"({item['function_owner']} -> {item['object_owner']})")
    for label, symbols in (("Function", functions), ("Object", objects)):
        owners = Counter(symbol["owner"] if symbol["owner"] is not None
                         else "ambiguous" if symbol["candidate_count"] else "no same-name owner"
                         for symbol in symbols)
        print(f"  {label} current-source candidates:")
        for owner, count in sorted(owners.items(), key=lambda pair: (-pair[1], pair[0])):
            print(f"    {count:>3}  {owner}")
        if label == "Function":
            assigned = [symbol["owner"] for symbol in symbols if symbol["owner"] is not None]
            runs = 0
            longest = 0
            current_run = 0
            previous = None
            for owner in assigned:
                current_run = current_run + 1 if owner == previous else 1
                runs += owner != previous
                longest = max(longest, current_run)
                previous = owner
            print(f"    Original-address owner runs: {runs} among {len(assigned)} uniquely assigned functions "
                  f"({max(runs - 1, 0)} switches; longest run {longest})")
        if label == "Object":
            counterparts = [symbol for symbol in symbols if symbol.get("global_counterpart")]
            print(f"    {len(counterparts)}/{sum(not symbol['candidate_count'] for symbol in symbols)} "
                  "unpaired LOCAL objects have one same-sized, same-section GLOBAL name counterpart")
        shown = symbols if limit == 0 else symbols[:limit]
        for symbol in shown:
            owner = symbol["owner"] or ("ambiguous" if symbol["candidate_count"] else "not paired")
            if symbol.get("global_counterpart"):
                owner += f"; GLOBAL counterpart in {symbol['global_counterpart']}"
            print(f"    0x{symbol['address']:08x}  {symbol['size']:>5}  "
                  f"{symbol['section']:<12}  {symbol['name']}  -> {owner}")
        if len(shown) < len(symbols):
            print(f"    ... {len(symbols) - len(shown)} more; use --limit 0 for all")


def ranked_component_splits(ledger: dict) -> list[dict]:
    """Rank unresolved minimum same-TU constraints, including provisional GLOBAL counterparts."""
    ranked = []
    for raw in ledger.get("original_strong_local_xref_components", []):
        component = original_component(ledger, raw["id"])
        pairs = component["local_state_pairs"]
        if pairs["split"] == 0:
            continue
        owners = {symbol["owner"] for symbol in component["functions"] if symbol["owner"] is not None}
        ranked.append({
            "id": component["id"], "functions": len(component["functions"]),
            "objects": len(component["objects"]), "sources": len(owners),
            **{key: pairs[key] for key in ("total", "resolved", "split", "provisional")},
        })
    return sorted(ranked, key=lambda item: (-item["split"], -item["sources"], -item["functions"], item["id"]))


def print_ranked_component_splits(ranked: list[dict], limit: int) -> None:
    print("Candidate cross-source LOCAL-state links by original component "
          "(name/size/section attribution; verify symbol identities before migration):")
    print("  ID   splits  resolved/total  provisional  funcs  objects  sources")
    shown = ranked if limit == 0 else ranked[:limit]
    for item in shown:
        print(f"  {item['id']:>3}  {item['split']:>6}  {item['resolved']:>4}/{item['total']:<5}  "
              f"{item['provisional']:>11}  {item['functions']:>5}  {item['objects']:>7}  {item['sources']:>7}")
    if len(shown) < len(ranked):
        print(f"  ... {len(ranked) - len(shown)} more; use --top-components 0 for all")


def overall_progress(ledger: dict, matching: dict | None = None) -> dict:
    """Separate observable source coverage, order grouping, and constraints."""
    # Compiler-generated numeric labels are not stable source identities. A
    # harmless recompile can renumber them and falsely move source coverage.
    originals = [symbol for symbol in ledger["original_symbols"]
                 if not is_assembler_label(symbol)]
    by_section = defaultdict(Counter)
    for symbol in originals:
        candidates = symbol.get("current_owner_candidates", [])
        state = "unique" if len(candidates) == 1 else "ambiguous" if candidates else "missing"
        by_section[symbol["section"]][state] += 1
    attributed = [{**symbol, "owners": symbol.get("current_owner_candidates", [])}
                  for symbol in originals]
    owner_counts = Counter(symbol["owners"][0] for symbol in attributed
                           if symbol["type"] == 2 and symbol["section"] == ".text"
                           and len(symbol["owners"]) == 1)
    grouping = address_contiguity(attributed)
    grouping["symbols_in_nontrivial_sources"] = sum(count for count in owner_counts.values()
                                                     if count >= 5)
    return {
        "original_symbols": len(originals),
        "assembler_labels_excluded": len(ledger["original_symbols"]) - len(originals),
        "current_units": len(ledger.get("current_units", [])),
        "by_section": {section: dict(counts) for section, counts in by_section.items()},
        "grouping": grouping,
        "constraints": same_tu_constraints(ledger) if "original_local_xrefs" in ledger else None,
        "splits": same_tu_owner_splits(ledger) if "original_local_xrefs" in ledger else [],
        "matching": (matching or {}).get("measures", {}),
    }


def print_overall_progress(progress: dict) -> None:
    """Human-readable dashboard; none of these measures is a TU completion score."""
    def coverage(section: str) -> str:
        counts = progress["by_section"].get(section, {})
        total = sum(counts.values())
        unique = counts.get("unique", 0)
        percent = 100.0 * unique / total if total else 0.0
        return (f"{unique:,}/{total:,} ({percent:.1f}%) unique source candidates; "
                f"{counts.get('ambiguous', 0):,} ambiguous, {counts.get('missing', 0):,} absent")

    print("Overall structural progress (independent indicators, not a completion score):")
    print(f"  Current compile units: {progress['current_units']:,}")
    print(f"  Original named allocated symbols: {progress['original_symbols']:,} "
          f"({progress['assembler_labels_excluded']:,} unstable numeric assembler labels excluded)")
    for section in (".text", ".rodata", ".data", ".bss"):
        print(f"  {section} source coverage: {coverage(section)}")
    grouping = progress["grouping"]
    if grouping["symbols_in_nontrivial_sources"]:
        print(f"  Original text grouping: {100 * grouping['fraction_in_owner_largest_run']:.1f}% "
              f"of {grouping['symbols_in_nontrivial_sources']:,} attributed functions in "
              "sources with >=5 lie in that source's largest original address run; "
              f"{grouping['owners_in_one_run']:,}/{grouping['owners_with_at_least_five_functions']:,} "
              "such sources form one run")
    else:
        print("  Original text grouping: too few attributed functions per source to measure")
    constraints = progress["constraints"]
    if constraints is not None and constraints["percent"] is not None:
        unsatisfied = constraints["assessable"] - constraints["satisfied"]
        unassessable = constraints["total"] - constraints["assessable"]
        print(f"  Original writable-state TU constraints: "
              f"{constraints['satisfied']:,}/{constraints['assessable']:,} satisfied "
              f"({constraints['percent']:.1f}% of assessable; "
              f"{constraints['coverage_percent']:.1f}% of original pairs assessable); "
              f"{unsatisfied:,} current source splits, "
              f"{unassessable:,} original pairs not yet assessable")
    elif constraints is None:
        print("  Original writable-state TU constraints: not computed")
    if progress["splits"]:
        print("  Largest verifiable source splits (function source -> local-state source):")
        for split in progress["splits"]:
            print(f"    {split['pairs']:>3} pairs  {split['function_source']} -> {split['object_source']}")
    matching = progress["matching"]
    if "matched_functions" in matching and "total_functions" in matching:
        print(f"  Body matching in matching.json: {matching['matched_functions']:,}/"
              f"{matching['total_functions']:,} exact functions "
              f"({matching['matched_functions_percent']:.1f}%); "
              f"fuzzy {matching['fuzzy_match_percent']:.4f}%")
    print("Name-only source candidates are not proven original TU owners; repeated local "
          "constant labels make .rodata especially ambiguous. Verified TU boundaries, "
          "relocations and GOT layout remain outside these indicators.")


def read_fresh_ledger(path: Path, original: Path, current: Path, units: Path | None) -> dict:
    dependencies = [original, current]
    if units is not None:
        dependencies.append(units)
    if path.stat().st_mtime < max(dependency.stat().st_mtime for dependency in dependencies):
        raise ValueError(f"{path} is older than an input; regenerate it before measuring TU constraints")
    ledger = json.loads(path.read_text(encoding="utf-8"))
    if "original_local_xrefs" not in ledger or "original_symbols" not in ledger:
        raise ValueError(f"{path} lacks the local-xref inventory")
    stored_original = ledger.get("original")
    stored_current = ledger.get("current")
    if (not isinstance(stored_original, str) or not isinstance(stored_current, str) or
            Path(stored_original).resolve() != original.resolve() or
            Path(stored_current).resolve() != current.resolve()):
        raise ValueError(f"{path} was generated for different ELF inputs")
    if units is not None:
        manifest = json.loads(units.read_text(encoding="utf-8"))
        entries = manifest["units"] if isinstance(manifest, dict) else manifest
        current_units = ledger.get("current_units")
        if not isinstance(current_units, list) or [
            (unit["source"], unit["object"], unit.get("optimization")) for unit in current_units
        ] != [
            (unit["source"], unit["object"], unit.get("optimization")) for unit in entries
        ]:
            raise ValueError(f"{path} was generated for a different unit manifest")
    return ledger


def main() -> None:
    root = workspace_root()
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--original", type=Path, default=root / "res/libTTapp.so")
    parser.add_argument("--current", type=Path, default=root / "bazel-out/k8-fastbuild/bin/src/libTTapp.so",
                        help="linked target-config ELF (default: Bazel target output)")
    parser.add_argument("--units", type=Path, default=root / "matching.json",
                        help="matching.json source/object manifest")
    parser.add_argument("--ledger", type=Path, help="fresh original TU map with local xrefs for same-TU constraint metrics")
    parser.add_argument("--no-xrefs", action="store_true", help="skip same-TU evidence calculation")
    parser.add_argument("--component", type=int, help="inspect an original strong-local component by its ID")
    parser.add_argument("--top-components", type=int, help="rank this many original components by unresolved "
                        "same-TU state splits; 0 prints all")
    parser.add_argument("--global-order", action="store_true", help="also show whole-linked text/data/BSS order proxy")
    parser.add_argument("--source", help="restrict to symbols in one current source object")
    parser.add_argument("--start", type=parse_address, help="inclusive original virtual address")
    parser.add_argument("--end", type=parse_address, help="exclusive original virtual address")
    parser.add_argument("--section", help="restrict to an original section, e.g. .text or .bss")
    parser.add_argument("--limit", type=int, default=0, help="maximum rank-detail rows; 0 prints all")
    parser.add_argument("--diff", action="store_true", help="print both address-sorted symbol lists and their unified name-order diff")
    parser.add_argument("--ranks", action="store_true", help="show rank displacements instead of the unified diff")
    parser.add_argument("--show-addresses", action="store_true", help="with --ranks, include linked addresses and byte-offset deltas")
    parser.add_argument("--include-labels", action="store_true", help="include assembler .L<number> labels in the lists")
    args = parser.parse_args()
    if args.start is not None and args.end is not None and args.end <= args.start:
        parser.error("--end must be greater than --start")
    if args.limit < 0:
        parser.error("--limit must be nonnegative")
    if args.top_components is not None and args.top_components < 0:
        parser.error("--top-components must be nonnegative")
    if args.top_components is not None and args.component is not None:
        parser.error("--top-components and --component are mutually exclusive")
    if args.component is not None and args.no_xrefs:
        parser.error("--component requires local-xref evidence")

    original_sections, original_symbols = read_elf32(args.original)
    current_sections, current_symbols = read_elf32(args.current)
    original = indexed_symbols(original_sections, original_symbols)
    current = indexed_symbols(current_sections, current_symbols)
    def in_scope(symbol: dict) -> bool:
        return ((args.start is None or symbol["address"] >= args.start) and
                (args.end is None or symbol["address"] < args.end) and
                (args.section is None or symbol["section"] == args.section))

    object_symbols = source_symbols(args.units, args.source, root) if args.source else None
    selected_keys = {symbol_key(symbol) for symbol in object_symbols} if object_symbols is not None else None
    rows, counts = pair_symbols(original, current, selected_keys, in_scope)
    rows, spacing = relative_positions(rows)
    original_order = substantive(ordered_symbols([symbol for symbol in original if in_scope(symbol)]))
    sections = {symbol["section"] for symbol in original_order}
    if object_symbols is not None:
        current_order = substantive(ordered_symbols([symbol for symbol in object_symbols if symbol["section"] in sections]))
    else:
        keys = {symbol_key(symbol) for symbol in original_order}
        current_order = substantive(current_order_symbols(current, keys, sections))
    if object_symbols is not None:
        unit_metric = source_order_alignment(original_order, current_order, counts.get("paired", 0))
        if unit_metric["comparable"]:
            print(f"TU ordered-symbol alignment: {unit_metric['percent']:.1f}% "
                  f"({unit_metric['ordered']} in order; {unit_metric['original']} original, "
                  f"{unit_metric['current']} current; {unit_metric['common']} shared)")
        else:
            print("TU ordered-symbol alignment: n/a (some paired linked symbols use different "
                  "input sections; inspect paired linked-order ranks)")
    overall_view = (args.source is None and args.start is None and args.end is None
                    and args.section is None)
    ledger = None
    if args.ledger:
        try:
            ledger = read_fresh_ledger(args.ledger, args.original, args.current, args.units)
        except (OSError, ValueError) as error:
            parser.error(str(error))
    elif not args.no_xrefs:
        cached = root / ".work/original-tu-map-current.json"
        if cached.is_file():
            try:
                ledger = read_fresh_ledger(cached, args.original, args.current, args.units)
            except (OSError, ValueError):
                pass
        if ledger is None:
            print("Computing fresh original local-state evidence...")
            ledger = build_map(args.original, args.current,
                               read_units_manifest(args.units, root), with_local_xrefs=True)
    if ledger is None and overall_view:
        ledger = build_map(args.original, args.current,
                           read_units_manifest(args.units, root), with_local_xrefs=False)
    if args.component is not None:
        if ledger is None or "original_strong_local_xref_components" not in ledger:
            parser.error("--component requires a ledger with strong-local components")
        try:
            component = original_component(ledger, args.component)
        except ValueError as error:
            parser.error(str(error))
        print_original_component(component, args.limit)
        return
    if args.top_components is not None:
        if ledger is None or "original_strong_local_xref_components" not in ledger:
            parser.error("--top-components requires a ledger with strong-local components")
        print_ranked_component_splits(ranked_component_splits(ledger), args.top_components)
        return
    if ledger is not None:
        if overall_view:
            matching = json.loads(args.units.read_text(encoding="utf-8"))
            print_overall_progress(overall_progress(ledger, matching))
        elif "original_local_xrefs" in ledger:
            constraints = same_tu_constraints(ledger)
            if constraints["percent"] is not None:
                print(f"Overall same-TU evidence: {constraints['percent']:.1f}% "
                      f"({constraints['satisfied']}/{constraints['assessable']} verifiable "
                      f"function-to-local-state pairs; {constraints['coverage_percent']:.1f}% of "
                      f"{constraints['total']} original pairs assessable)")
        if args.start is not None or args.end is not None:
            if "original_local_xrefs" in ledger:
                selected = same_tu_constraints(ledger, args.start, args.end)
                if selected["percent"] is not None:
                    print(f"Selected-run same-TU evidence: {selected['percent']:.1f}% "
                          f"({selected['satisfied']}/{selected['assessable']} verifiable pairs; "
                          f"{selected['coverage_percent']:.1f}% of {selected['total']} assessable)")
    if args.global_order:
        overall = overall_order_alignment(original, current)
        print(f"Whole-binary order proxy: {overall['percent']:.1f}% "
              f"({overall['ordered']} unambiguous symbols in order; "
              f"{overall['original']} original, {overall['current']} current)")
        for section, metric in overall["sections"].items():
            print(f"  {section}: {metric['percent']:.1f}% "
                  f"({metric['ordered']} in order; {metric['original']} original, {metric['current']} current)")
    if not overall_view:
        print("These metrics track structure, not matching bodies or unexamined TU boundaries and relocations.")
    if not (args.diff or args.ranks):
        return
    print(f"paired={counts.get('paired', 0)} ambiguous-original={counts.get('ambiguous_original', 0)} "
          f"unmatched-original={counts.get('unmatched_original', 0)} "
          f"section-moves={spacing['section_moves']} "
          f"order-breaks={spacing['adjacent_order_breaks']} "
          f"inversions={spacing['pairwise_inversions']}/{spacing['comparable_pairs']} "
          f"rank-displaced={spacing['rank_displaced']}")
    print("The order summary uses uniquely paired symbols within each section pair; "
          "it is placement evidence, not original TU ownership.")
    if not args.ranks:
        if args.include_labels:
            original_order = ordered_symbols([symbol for symbol in original if in_scope(symbol)])
            if object_symbols is not None:
                current_order = ordered_symbols([symbol for symbol in object_symbols if symbol["section"] in sections])
            else:
                keys = {symbol_key(symbol) for symbol in original_order}
                current_order = current_order_symbols(current, keys, sections)
        print(f"Original by address ({len(original_order)} symbols):")
        for symbol in original_order:
            print(f"  {symbol['address']:08x}  {symbol['size']:>7}  {symbol['section']:<16}  {display_symbol(symbol)}")
        current_label = "current object section offset" if object_symbols is not None else "current linked address"
        print(f"Current by {current_label} ({len(current_order)} symbols):")
        for symbol in current_order:
            print(f"  {symbol['address']:08x}  {symbol['size']:>7}  {symbol['section']:<16}  {display_symbol(symbol)}")
        print("Order diff (addresses excluded from comparison; '-' original, '+' current):")
        diff = order_diff_lines(original_order, current_order)
        if diff:
            for line in diff:
                print(line, end="")
        else:
            print("  identical symbol order")
        return
    if args.show_addresses:
        print("original  current   abs-delta  section-offset-delta  anchor-delta  gap-delta  rank o/c  size o/c  section o/c  name")
    else:
        print("rank o/c  rank-delta  size o/c  section o/c  name")
    shown = rows if args.limit == 0 else rows[:args.limit]
    for row in shown:
        order = f"{row['original_rank'] + 1}/{row['current_rank'] + 1}"
        tail = (f"{row['size_original']}/{row['size_current']}  "
                f"{row['section_original']}/{row['section_current']}  {row['name']}")
        if args.show_addresses:
            gap = str(row.get("gap_delta", "-"))
            print(f"{row['address_original']:08x}  {row['address_current']:08x}  "
                  f"{row['absolute_delta']:+10d}  {row['section_offset_delta']:+20d}  "
                  f"{row['relative_delta']:+12d}  {gap:>9}  {order:>8}  {tail}")
        else:
            print(f"{order:>8}  {row['rank_delta']:+10d}  {tail}")
    if len(shown) < len(rows):
        print(f"... {len(rows) - len(shown)} more paired symbols; use --limit 0 for all")


if __name__ == "__main__":
    main()
