"""Guard the lossless symbol inventory and conservative TU evidence rules."""

import unittest
from pathlib import Path
import json
import struct
import tempfile

from scripts.restructure.generate_original_tu_map import (
    allocated_symbols,
    current_unit_symbols,
    alias_groups,
    annotate_local_xrefs,
    local_xref_components,
    STRONG_LOCAL_XREF,
    function_local_anchors,
    local_initializer_blocks,
    original_build_clues,
)
from scripts.restructure.elf32 import SHN_COMMON
from scripts.restructure.inputs import read_units_manifest


class OriginalTuMapTest(unittest.TestCase):
    def test_strong_local_xrefs_form_minimum_components_only(self):
        mapped = [
            {"symbol_index": 1, "type": 2, "local_initializer_block": 4},
            {"symbol_index": 2, "type": 1, "local_initializer_block": 4},
            {"symbol_index": 3, "type": 2, "local_initializer_block": None},
            {"symbol_index": 4, "type": 1, "local_initializer_block": 5},
            {"symbol_index": 5, "type": 1, "local_initializer_block": 5},
        ]
        def edge(source, target, evidence, suffix):
            return {"function_symbol_index": source, "object_symbol_indices": target,
                    "same_tu_evidence": evidence, "id": f"xref:{suffix}"}
        edges = [edge(1, [2], STRONG_LOCAL_XREF, "a"),
                 edge(3, [2], STRONG_LOCAL_XREF, "b"),
                 edge(3, [4], STRONG_LOCAL_XREF, "c"),
                 edge(1, [5], "medium same-TU evidence", "d"),
                 edge(1, [4, 5], STRONG_LOCAL_XREF, "ambiguous")]
        groups, summary = local_xref_components(edges, mapped)
        self.assertEqual(len(groups), 1)
        self.assertEqual(groups[0]["symbol_indices"], [1, 2, 3, 4])
        self.assertEqual(groups[0]["xref_ids"], ["xref:a", "xref:b", "xref:c"])
        self.assertEqual(groups[0]["local_initializer_blocks"], [4, 5])
        self.assertEqual(summary["strong_local_xref_components_crossing_initializer_blocks"], 1)

    def test_local_xref_annotations_and_current_disagreement_are_diagnostic(self):
        mapped = [
            {"symbol_index": 1, "section": ".text", "binding": 0, "current_owner_candidates": [2]},
            {"symbol_index": 2, "section": ".data", "binding": 0, "current_owner_candidates": [3]},
            {"symbol_index": 3, "section": ".rodata", "binding": 0, "current_owner_candidates": [2]},
            {"symbol_index": 4, "section": ".data", "binding": 0, "current_owner_candidates": [4]},
        ]
        base = {"function_symbol_index": 1, "instruction_address": 0x100,
                "object_address": 0x200, "object_alias_ambiguous": False}
        edges = [{**base, "object_symbol_indices": [2]},
                 {**base, "instruction_address": 0x106, "object_address": 0x300,
                  "object_symbol_indices": [3]},
                 {**base, "instruction_address": 0x10c, "object_address": 0x400,
                  "object_symbol_indices": [2, 4], "object_alias_ambiguous": True}]
        units = [
            {"id": 2, "symbols": []},
            {"id": 3, "symbols": [{"name": "state", "type": 1, "size": 4, "binding": 0}]},
            {"id": 4, "symbols": [{"name": "alias", "type": 1, "size": 4, "binding": 0}]},
        ]
        mapped[1].update(name="state", type=1, size=4)
        mapped[2].update(name="constant", type=1, size=4)
        mapped[3].update(name="alias", type=1, size=4)
        annotated, summary = annotate_local_xrefs(edges, mapped, units)
        self.assertEqual(annotated[0]["id"], "original-local-xref:1:00000100:00000200")
        self.assertEqual(annotated[0]["target_sections"], [".data"])
        self.assertEqual(annotated[0]["target_bindings"], [0])
        self.assertIn("strong", annotated[0]["same_tu_evidence"])
        self.assertIn("medium", annotated[1]["same_tu_evidence"])
        self.assertEqual(summary["current_cross_owner_diagnostic_assessable"], 2)
        self.assertEqual(summary["current_cross_owner_diagnostic_discordant"], 1)
        self.assertTrue(annotated[0]["current_target_object_size_concordant"])
        self.assertEqual(summary["current_cross_owner_diagnostic_discordant_object_size_concordant"], 1)
        self.assertEqual(summary["original_local_xrefs_alias_ambiguous"], 1)

    def test_allocated_symbols_keep_aliases_and_zero_size(self):
        sections = [{"flags": 0}, {"flags": 2}, {"flags": 0}]
        symbols = [
            {"symbol_index": 1, "section_index": 1, "type": 2, "size": 0},
            {"symbol_index": 2, "section_index": 1, "type": 2, "size": 0},
            {"symbol_index": 3, "section_index": 1, "type": 1, "size": 4},
            {"symbol_index": 4, "section_index": 1, "type": 3, "size": 0},
            {"symbol_index": 5, "section_index": 2, "type": 2, "size": 4},
        ]
        self.assertEqual(
            [symbol["symbol_index"] for symbol in allocated_symbols(sections, symbols)],
            [1, 2, 3],
        )

    def test_current_unit_symbols_include_common_without_changing_original_inventory(self):
        sections = [{"flags": 0}, {"flags": 2}]
        symbols = [
            {"symbol_index": 2, "section_index": SHN_COMMON, "type": 1},
            {"symbol_index": 1, "section_index": 1, "type": 1},
            {"symbol_index": 3, "section_index": SHN_COMMON, "type": 3},
        ]
        self.assertEqual([item["symbol_index"] for item in allocated_symbols(sections, symbols)], [1])
        self.assertEqual([item["symbol_index"] for item in current_unit_symbols(sections, symbols)], [2, 1])

    def test_alias_group_preserves_every_symbol_identity(self):
        base = {"section_index": 1, "address": 0x1234, "size": 4, "type": 1}
        symbols = [
            {**base, "symbol_index": 1},
            {**base, "symbol_index": 2},
            {**base, "symbol_index": 3, "size": 8},
        ]
        groups, assignments = alias_groups(symbols)
        self.assertEqual(groups[0]["symbol_indices"], [1, 2])
        self.assertEqual(assignments, {1: 0, 2: 0})

    def test_initializer_segments_are_not_claimed_as_tus(self):
        symbols = [
            {"symbol_index": 1, "binding": 0, "name": "local_from_another_unit"},
            {"symbol_index": 2, "binding": 0, "name": "_GLOBAL__sub_I_Foo.cpp"},
            {"symbol_index": 3, "binding": 0, "name": "unbounded_local"},
            {"symbol_index": 4, "binding": 1, "name": "global"},
        ]
        blocks, assignments = local_initializer_blocks(symbols)
        self.assertEqual([block["basename"] for block in blocks], ["Foo.cpp", None])
        self.assertEqual(assignments, {1: 0, 2: 0, 3: 1})
        self.assertIn("may contain other units", blocks[0]["certainty"])

    def test_initializers_and_embedded_paths_are_kept_separate(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "binary"
            contents = bytearray(100)
            contents[5] = 1
            contents[16:24] = struct.pack("<II", 0x1000, 0x2000)
            contents[40:58] = b"i:/src/example.cpp\0"
            path.write_bytes(contents)
            sections = [{"name": ".init_array", "offset": 16, "size": 8}]
            symbols = [
                {"address": 0x1000, "name": "_GLOBAL__sub_I_example.cpp", "symbol_index": 1}
            ]
            initializers, paths = original_build_clues(path, sections, symbols)
            self.assertEqual([entry["address"] for entry in initializers], [0x1000, 0x2000])
            self.assertEqual(initializers[0]["symbols"][0]["symbol_index"], 1)
            self.assertEqual(paths, ["i:/src/example.cpp"])

    def test_function_local_static_anchors_parent_function(self):
        symbols = [
            {"symbol_index": 1, "name": "NuIOS_YieldThread", "type": 2},
            {"symbol_index": 2, "name": "_ZZ17NuIOS_YieldThreadE5count", "type": 1},
        ]
        self.assertEqual(function_local_anchors(symbols), {2: [1]})

    def test_explicit_unit_manifest_needs_no_build_tool(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / "unit.o").write_bytes(b"object")
            manifest = root / "units.json"
            manifest.write_text(
                json.dumps({"units": [{"source": "src/unit.cpp", "object": "unit.o"}]}),
                encoding="utf-8",
            )
            units = read_units_manifest(manifest, root)
            self.assertEqual(units[0]["source"], "src/unit.cpp")
            self.assertEqual(units[0]["object_path"], root / "unit.o")
            self.assertIsNone(units[0]["optimization"])


if __name__ == "__main__":
    unittest.main()
