"""Guard the original relocation ledger without inferring source ownership."""

import unittest

from scripts.restructure.elf32 import workspace_root
from scripts.restructure.original_relocations import (
    R_386_32,
    R_386_GLOB_DAT,
    R_386_JUMP_SLOT,
    R_386_RELATIVE,
    extract,
    relocation_target,
)


class OriginalRelocationsTest(unittest.TestCase):
    def test_only_defined_static_targets_are_resolved(self):
        defined = {"address": 0x1000, "section_index": 3}
        imported = {"address": 0, "section_index": 0}
        self.assertEqual(relocation_target(R_386_RELATIVE, 0x1234, None), 0x1234)
        self.assertEqual(relocation_target(R_386_32, 0x24, defined), 0x1024)
        self.assertEqual(relocation_target(R_386_GLOB_DAT, None, defined), 0x1000)
        self.assertEqual(relocation_target(R_386_JUMP_SLOT, None, defined), 0x1000)
        self.assertIsNone(relocation_target(R_386_32, 0x24, imported))
        self.assertIsNone(relocation_target(R_386_JUMP_SLOT, None, imported))
        self.assertIsNone(relocation_target(99, 0x24, defined))

    def test_original_relocation_surface_and_got_positions(self):
        records, summary = extract(workspace_root() / "res/libTTapp.so")
        self.assertEqual(len(records), 14751)
        self.assertEqual(summary["original_relocations_by_type"], {
            "R_386_32": 114,
            "R_386_GLOB_DAT": 14,
            "R_386_JUMP_SLOT": 190,
            "R_386_RELATIVE": 14433,
        })
        got = [record for record in records if record["site_section"] == ".got"]
        self.assertEqual(len(got), 4758)
        self.assertTrue(all(record["site_section_offset"] is not None for record in got))
        self.assertTrue(all("not TU ownership" in record["certainty"] for record in records))
        self.assertEqual(summary["original_relocations_with_containing_object"], 9568)
        self.assertEqual(summary["original_relocations_with_exact_target_symbol"], 11114)


if __name__ == "__main__":
    unittest.main()
