"""Conservative tests for the linked .got order diagnostic."""

import unittest

from scripts.restructure.compare_got_order import got_target_order, order_agreement
from scripts.restructure.elf32 import SHF_ALLOC, SHT_SYMTAB


SECTIONS = [
    {"name": "", "type": 0, "address": 0, "size": 0, "flags": 0},
    {"name": ".got", "type": 1, "address": 0x2000, "size": 24, "flags": SHF_ALLOC},
    {"name": ".data", "type": 1, "address": 0x3000, "size": 0x100, "flags": SHF_ALLOC},
    {"name": ".symtab", "type": SHT_SYMTAB, "address": 0, "size": 0, "flags": 0},
]


def obj(index, name, address, *, size=4, section_index=2):
    return {"symbol_index": index, "name": name, "address": address,
            "size": size, "type": 1, "binding": 0, "section_index": section_index}


def reloc(slot, target, *, kind="R_386_RELATIVE", targets=None, offset=None):
    return {"site_section": ".got", "site_address": 0x2000 + slot * 4,
            "site_section_offset": slot * 4 if offset is None else offset,
            "kind": kind, "target_exact_symbol_indices": targets if targets is not None else [target],
            "target_address": 0x3000 + target * 4}


class CompareGotOrderTests(unittest.TestCase):
    def test_only_unique_exact_static_relative_targets_count(self):
        symbols = [obj(1, "a", 0x3004), obj(2, "duplicate", 0x3008),
                   obj(3, "duplicate", 0x300c), obj(4, "import", 0x3010)]
        records = [reloc(0, 1), reloc(1, 2), reloc(2, 3),
                   reloc(3, 4, kind="R_386_GLOB_DAT"),
                   reloc(4, 1, targets=[]), reloc(5, 1, targets=[1, 2])]
        result = got_target_order(SECTIONS, symbols, records)
        self.assertEqual(result["slots"], 6)
        self.assertEqual(result["relocations"], 6)
        self.assertEqual(result["relative_relocations"], 5)
        self.assertEqual(result["unique_relative_targets"], [("a", 1, 0, ".data", 4)])

    def test_repeated_target_or_site_is_not_a_unique_slot(self):
        symbols = [obj(1, "a", 0x3004), obj(2, "b", 0x3008)]
        records = [reloc(0, 1), reloc(1, 1), reloc(2, 2), reloc(2, 2)]
        result = got_target_order(SECTIONS, symbols, records)
        self.assertEqual(result["unique_relative_targets"], [])

    def test_reject_interior_misaligned_and_outside_slots(self):
        symbols = [obj(1, "a", 0x3004), obj(2, "b", 0x3008)]
        records = [reloc(0, 1, offset=2), reloc(1, 2, targets=[2]),
                   reloc(7, 1), {**reloc(3, 1), "target_address": 0x3005}]
        result = got_target_order(SECTIONS, symbols, records)
        self.assertEqual(result["unique_relative_targets"], [("b", 1, 0, ".data", 4)])

    def test_order_and_coverage_are_separate(self):
        def info(keys):
            return {"slots": 4, "relocations": 4, "relative_relocations": 4,
                    "unique_relative_targets": keys}

        result = order_agreement(info(["a", "b", "c", "d"]), info(["b", "a", "c"]))
        self.assertEqual(result["shared_targets"], 3)
        self.assertEqual(result["ordered_shared_targets"], 2)
        self.assertEqual(result["coverage_percent"], 75.0)
        self.assertAlmostEqual(result["order_percent"], 200 / 3)

    def test_missing_got_or_non_word_size_is_invalid(self):
        with self.assertRaises(ValueError):
            got_target_order(SECTIONS[:1], [], [])
        with self.assertRaises(ValueError):
            got_target_order([{**section, "size": 3} if section["name"] == ".got" else section
                              for section in SECTIONS], [], [])

    def test_multiple_symbol_tables_are_rejected(self):
        with self.assertRaisesRegex(ValueError, "exactly one static symbol table"):
            got_target_order(SECTIONS + [{**SECTIONS[-1], "name": ".second_symtab"}], [], [])

    def test_malformed_offset_is_skipped_before_sorting(self):
        result = got_target_order(SECTIONS, [obj(1, "a", 0x3004)],
                                  [{**reloc(0, 1), "site_section_offset": None}, reloc(1, 1)])
        self.assertEqual(result["unique_relative_targets"], [("a", 1, 0, ".data", 4)])


if __name__ == "__main__":
    unittest.main()
