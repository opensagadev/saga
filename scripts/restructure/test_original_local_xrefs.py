"""Synthetic exact-byte and REL-addend checks for original local references."""

import struct
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from scripts.restructure.original_local_xrefs import extract


def fixture(path):
    blob = bytearray(0x400)
    blob[:6] = b"\x7fELF\x01\x01"
    struct.pack_into("<H", blob, 18, 3)
    # call thunk; add ebx, 0x1000 - next EIP; mov eax,[ebx+0x100]
    # mov ecx,[ebx+0x200] (GOT RELATIVE); branch; unreachable-straight-line
    # candidate after branch must not be credited.
    code = (b"\xe8" + struct.pack("<i", 0x180 - 0x105) + b"\x81\xc3" +
            struct.pack("<I", 0x1000 - 0x105) + b"\x8b\x83\x00\x01\x00\x00" +
            b"\x8b\x8b\x00\x02\x00\x00" + b"\xeb\x00" +
            b"\x8b\x83\x00\x01\x00\x00")
    blob[0x100:0x100 + len(code)] = code
    struct.pack_into("<I", blob, 0x300, 0x1300)
    struct.pack_into("<II", blob, 0x380, 0x1200, 8)
    path.write_bytes(blob)
    sections = [
        dict(index=0, name="", type=0, address=0, offset=0, size=0, flags=0),
        dict(index=1, name=".text", type=1, address=0x100, offset=0x100, size=0x100, flags=6),
        dict(index=2, name=".data", type=1, address=0x1100, offset=0x200, size=0x100, flags=3),
        dict(index=3, name=".got", type=1, address=0x1200, offset=0x300, size=4, flags=3),
        dict(index=4, name=".rel.dyn", type=9, address=0, offset=0x380, size=8, flags=0),
        dict(index=5, name=".bss", type=8, address=0x1300, offset=0, size=0x100, flags=3),
    ]
    symbols = [
        dict(name="func", address=0x100, size=len(code), type=2, binding=0, section_index=1, symbol_index=1),
        dict(name="__x86.get_pc_thunk.bx", address=0x180, size=4, type=2, binding=0, section_index=1, symbol_index=2),
        dict(name="direct", address=0x1100, size=4, type=1, binding=0, section_index=2, symbol_index=3),
        dict(name="via_got", address=0x1300, size=4, type=1, binding=0, section_index=5, symbol_index=4),
    ]
    return blob, sections, symbols


class OriginalLocalXrefsTest(unittest.TestCase):
    def test_pic_direct_got_and_rejections(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "small.so"
            blob, sections, symbols = fixture(path)
            with patch("scripts.restructure.original_local_xrefs.read_elf32", return_value=(sections, symbols)):
                edges = extract(path)
            self.assertEqual([edge["kind"] for edge in edges],
                             ["direct-ebx-relative", "got-relative-relocation"])
            self.assertEqual([edge["object_names"] for edge in edges], [["direct"], ["via_got"]])

            # A GOT slot with a non-RELATIVE relocation is not a local target.
            struct.pack_into("<II", blob, 0x380, 0x1200, 6)
            path.write_bytes(blob)
            with patch("scripts.restructure.original_local_xrefs.read_elf32", return_value=(sections, symbols)):
                self.assertEqual([edge["object_names"] for edge in extract(path)], [["direct"]])

            # The exact PC thunk/add instruction pair is mandatory.
            blob[0x105] = 0x90
            path.write_bytes(blob)
            with patch("scripts.restructure.original_local_xrefs.read_elf32", return_value=(sections, symbols)):
                self.assertEqual(extract(path), [])

    def test_same_address_local_aliases_remain_unresolved(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "small.so"
            _, sections, symbols = fixture(path)
            symbols.append(dict(name="direct_alias", address=0x1100, size=4, type=1,
                                binding=0, section_index=2, symbol_index=5))
            with patch("scripts.restructure.original_local_xrefs.read_elf32", return_value=(sections, symbols)):
                edges = extract(path)
            self.assertEqual(edges[0]["object_symbol_indices"], [3, 5])
            self.assertEqual(edges[0]["object_names"], ["direct", "direct_alias"])
            self.assertTrue(edges[0]["object_alias_ambiguous"])
            self.assertFalse(edges[1]["object_alias_ambiguous"])


if __name__ == "__main__":
    unittest.main()
