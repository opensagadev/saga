"""Conservative linked-symbol placement comparison tests."""

from contextlib import redirect_stdout
from io import StringIO
import json
import os
from pathlib import Path
import tempfile
import time
import unittest

from scripts.restructure.compare_symbol_placement import (
    current_order_symbols,
    lcs_length,
    order_alignment,
    order_diff_lines,
    ordered_symbols,
    original_component,
    overall_progress,
    pair_symbols,
    print_overall_progress,
    print_original_component,
    read_fresh_ledger,
    relative_positions,
    same_tu_constraints,
    same_tu_owner_splits,
    symbol_key,
)


def symbol(name, address, *, section=".text", section_base=0x1000, binding=0, size=8):
    return {
        "name": name,
        "address": address,
        "section": section,
        "section_offset": address - section_base,
        "size": size,
        "type": 2 if section == ".text" else 1,
        "binding": binding,
        "symbol_index": address,
    }


class CompareSymbolPlacementTests(unittest.TestCase):
    def test_component_uses_elf_symbol_ids_not_json_positions(self):
        function = {**symbol("TerrainScan", 0x2000, binding=1), "symbol_index": 91,
                    "current_owner_candidates": [2]}
        state = {**symbol("_ZL4TerI", 0x4000, section=".bss", size=4),
                 "symbol_index": 48, "current_owner_candidates": []}
        ledger = {
            "original_symbols": [state, function],
            "current_units": [{"id": 2, "source": "terrain.cpp"}],
            "original_strong_local_xref_components": [{
                "id": 7, "certainty": "minimum same-TU constraint",
                "local_initializer_blocks": [3],
                "function_symbol_indices": [91], "object_symbol_indices": [48],
            }],
        }
        component = original_component(ledger, 7)
        self.assertEqual(component["functions"][0]["name"], "TerrainScan")
        self.assertEqual(component["functions"][0]["owner"], "terrain.cpp")
        self.assertEqual(component["objects"][0]["name"], "_ZL4TerI")
        stream = StringIO()
        with redirect_stdout(stream):
            print_original_component(component)
        self.assertIn("1 functions, 1 LOCAL objects", stream.getvalue())
        with self.assertRaisesRegex(ValueError, "no original strong-local component"):
            original_component(ledger, 8)

    def test_cached_ledger_must_match_elf_paths_and_units(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            original = root / "original.so"
            current = root / "current.so"
            units = root / "matching.json"
            ledger = root / "ledger.json"
            original.write_bytes(b"original")
            current.write_bytes(b"current")
            manifest = [{"source": "one.c", "object": "one.o"}]
            units.write_text(json.dumps({"units": manifest}), encoding="utf-8")
            contents = {
                "original": str(original), "current": str(current),
                "original_local_xrefs": [], "original_symbols": [],
                "current_units": manifest,
            }

            def save_ledger():
                ledger.write_text(json.dumps(contents), encoding="utf-8")
                future = time.time() + 10
                os.utime(ledger, (future, future))

            save_ledger()
            self.assertEqual(read_fresh_ledger(ledger, original, current, units), contents)
            contents["current"] = str(root / "other.so")
            save_ledger()
            with self.assertRaisesRegex(ValueError, "different ELF inputs"):
                read_fresh_ledger(ledger, original, current, units)
            contents["current"] = str(current)
            contents["current_units"] = [{"source": "other.c", "object": "one.o"}]
            save_ledger()
            with self.assertRaisesRegex(ValueError, "different unit manifest"):
                read_fresh_ledger(ledger, original, current, units)

    def test_absolute_section_and_relative_deltas_are_distinct(self):
        original = [symbol("a", 0x1010), symbol("b", 0x1020)]
        current = [symbol("a", 0x2020, section_base=0x2000),
                   symbol("b", 0x2034, section_base=0x2000)]
        rows, counts = pair_symbols(original, current)
        rows, spacing = relative_positions(rows)
        self.assertEqual(counts["paired"], 2)
        self.assertEqual(rows[0]["absolute_delta"], 0x1010)
        self.assertEqual(rows[0]["section_offset_delta"], 0x10)
        self.assertEqual(rows[0]["relative_delta"], 0)
        self.assertEqual(rows[1]["relative_delta"], 4)
        self.assertEqual(rows[1]["gap_delta"], 4)
        self.assertEqual(spacing["adjacent_order_breaks"], 0)
        self.assertEqual(spacing["pairwise_inversions"], 0)
        self.assertEqual([row["rank_delta"] for row in rows], [0, 0])

    def test_duplicate_static_is_ambiguous_even_outside_selected_range(self):
        original = [symbol("local", 0x1010), symbol("local", 0x9010)]
        current = [symbol("local", 0x2020, section_base=0x2000)]
        rows, counts = pair_symbols(original, current, original_predicate=lambda s: s["address"] < 0x2000)
        self.assertEqual(rows, [])
        self.assertEqual(counts["ambiguous_original"], 1)

    def test_duplicate_current_and_missing_remain_unpaired(self):
        original = [symbol("duplicate", 0x1010), symbol("missing", 0x1020)]
        current = [symbol("duplicate", 0x2010), symbol("duplicate", 0x3010)]
        rows, counts = pair_symbols(original, current)
        self.assertEqual(rows, [])
        self.assertEqual(counts["ambiguous_original"], 1)
        self.assertEqual(counts["unmatched_original"], 1)

    def test_order_break_is_reported_not_repaired(self):
        original = [symbol("a", 0x1010), symbol("b", 0x1020)]
        current = [symbol("a", 0x2040, section_base=0x2000),
                   symbol("b", 0x2020, section_base=0x2000)]
        rows, _ = pair_symbols(original, current)
        rows, spacing = relative_positions(rows)
        self.assertEqual(rows[1]["current_gap"], -0x20)
        self.assertEqual(spacing["adjacent_order_breaks"], 1)
        self.assertEqual(spacing["pairwise_inversions"], 1)
        self.assertEqual([row["rank_delta"] for row in rows], [1, -1])

    def test_address_sorted_lists_diff_symbol_order_not_addresses(self):
        original = ordered_symbols([symbol("b", 0x1020), symbol("a", 0x1010)])
        current = ordered_symbols([symbol("a", 0x2040), symbol("b", 0x2020)])
        diff = "".join(order_diff_lines(original, current))
        self.assertIn("+b\n", diff)
        self.assertIn("-b\n", diff)
        same_order_shifted = ordered_symbols([symbol("a", 0x2010), symbol("b", 0x2030)])
        self.assertEqual(order_diff_lines(original, same_order_shifted), [])

    def test_current_source_list_skips_ambiguous_and_wrong_section(self):
        current = [symbol("a", 0x2010), symbol("a", 0x3010),
                   symbol("data", 0x4010, section=".bss")]
        keys = {symbol_key(item) for item in current}
        self.assertEqual(current_order_symbols(current, keys, {".text"}), [])

    def test_ordered_symbol_alignment_penalizes_missing_extra_and_reordering(self):
        original = [symbol("a", 0x1010), symbol("b", 0x1020), symbol("c", 0x1030)]
        current = [symbol("b", 0x2010), symbol("a", 0x2020), symbol("extra", 0x2030)]
        self.assertEqual(lcs_length(["a", "b", "c"], ["b", "a", "extra"]), 1)
        metric = order_alignment(original, current)
        self.assertEqual(metric["common"], 2)
        self.assertEqual(metric["ordered"], 1)
        self.assertAlmostEqual(metric["percent"], 100 / 3)

    def test_global_unique_only_does_not_guess_duplicate_local_identity(self):
        original = [symbol("same", 0x1010), symbol("same", 0x1020), symbol("unique", 0x1030)]
        current = [symbol("same", 0x2010), symbol("same", 0x2020), symbol("unique", 0x2030)]
        metric = order_alignment(original, current, unique_only=True)
        self.assertEqual(metric["eligible_unique"], 1)
        self.assertEqual(metric["ordered"], 1)
        self.assertAlmostEqual(metric["percent"], 100 / 3)

    def test_same_tu_metric_counts_distinct_size_concordant_strong_pairs(self):
        ledger = {
            "original_symbols": [
                {"symbol_index": 1, "address": 0x100, "current_owner_candidates": [2]},
                {"symbol_index": 2, "address": 0x200, "current_owner_candidates": [2]},
                {"symbol_index": 3, "address": 0x300, "current_owner_candidates": [3]},
            ],
            "original_local_xrefs": [
                {"same_tu_evidence": "strong same-TU constraint; not an assignment",
                 "function_symbol_index": 1, "object_symbol_indices": [2],
                 "current_target_object_size_concordant": True},
                {"same_tu_evidence": "strong same-TU constraint; not an assignment",
                 "function_symbol_index": 1, "object_symbol_indices": [2],
                 "current_target_object_size_concordant": True},
                {"same_tu_evidence": "strong same-TU constraint; not an assignment",
                 "function_symbol_index": 1, "object_symbol_indices": [3],
                 "current_target_object_size_concordant": True},
            ],
        }
        metric = same_tu_constraints(ledger, 0x100, 0x200)
        self.assertEqual((metric["total"], metric["assessable"], metric["satisfied"]), (2, 2, 1))
        self.assertEqual(metric["percent"], 50.0)

    def test_same_tu_metric_does_not_assign_duplicate_original_local_state(self):
        ledger = {
            "original_symbols": [
                {**symbol("NuTimeStartFrame", 0x100, binding=1), "current_owner_candidates": [0]},
                {**symbol("frameStartTime", 0x200, section=".bss"), "current_owner_candidates": [1]},
                {**symbol("frameStartTime", 0x300, section=".bss", size=4), "current_owner_candidates": [1]},
            ],
            "current_units": [{"id": 0, "source": "time.cpp"}, {"id": 1, "source": "utility.cpp"}],
            "original_local_xrefs": [
                {"same_tu_evidence": "strong same-TU constraint; not an assignment",
                 "function_symbol_index": 0x100, "object_symbol_indices": [0x200],
                 "current_target_object_size_concordant": True},
            ],
        }
        metric = same_tu_constraints(ledger)
        self.assertEqual((metric["total"], metric["assessable"], metric["satisfied"]), (1, 0, 0))
        self.assertEqual(same_tu_owner_splits(ledger), [])

    def test_overall_report_separates_coverage_grouping_and_verified_splits(self):
        ledger = {
            "original_symbols": [
                {**symbol("first", 0x100), "current_owner_candidates": [0]},
                {**symbol("second", 0x110), "current_owner_candidates": [0]},
                {**symbol("third", 0x120), "current_owner_candidates": [1]},
                {**symbol("state", 0x200, section=".bss"), "current_owner_candidates": [1]},
                {**symbol("unknown", 0x210, section=".data"), "current_owner_candidates": []},
            ],
            "current_units": [{"id": 0, "source": "render.c"},
                              {"id": 1, "source": "state.c"}],
            "original_local_xrefs": [
                {"same_tu_evidence": "strong same-TU constraint; not an assignment",
                 "function_symbol_index": 0x100, "object_symbol_indices": [0x200],
                 "current_target_object_size_concordant": True},
                {"same_tu_evidence": "strong same-TU constraint; not an assignment",
                 "function_symbol_index": 0x100, "object_symbol_indices": [0x200],
                 "current_target_object_size_concordant": True},
            ],
        }
        report = overall_progress(ledger, {"measures": {
            "matched_functions": 1, "total_functions": 3,
            "matched_functions_percent": 33.3, "fuzzy_match_percent": 42.0,
        }})
        self.assertEqual(report["by_section"][".text"]["unique"], 3)
        self.assertEqual(report["by_section"][".data"]["missing"], 1)
        self.assertEqual(report["constraints"]["assessable"], 1)
        self.assertEqual(same_tu_owner_splits(ledger), [{
            "function_source": "render.c", "object_source": "state.c", "pairs": 1,
        }])
        output = StringIO()
        with redirect_stdout(output):
            print_overall_progress(report)
        self.assertIn("Original text grouping", output.getvalue())
        self.assertIn("1 current source splits", output.getvalue())
        self.assertIn("render.c -> state.c", output.getvalue())
        self.assertIn("Body matching in matching.json", output.getvalue())

    def test_overall_report_without_xrefs_omits_constraint_claim(self):
        ledger = {"original_symbols": [{**symbol("first", 0x100),
                                        "current_owner_candidates": [0]}],
                  "current_units": [{"id": 0, "source": "first.c"}]}
        report = overall_progress(ledger)
        self.assertIsNone(report["constraints"])
        output = StringIO()
        with redirect_stdout(output):
            print_overall_progress(report)
        self.assertIn("TU constraints: not computed", output.getvalue())
        self.assertIn("too few attributed functions", output.getvalue())


if __name__ == "__main__":
    unittest.main()
