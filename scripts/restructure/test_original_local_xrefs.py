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
    # mov ecx,[ebx+0x200] (GOT RELATIVE); branch over an unreachable
    # candidate, which must not be credited.
    code = (b"\xe8" + struct.pack("<i", 0x180 - 0x105) + b"\x81\xc3" +
            struct.pack("<I", 0x1000 - 0x105) + b"\x8b\x83\x00\x01\x00\x00" +
            b"\x8b\x8b\x00\x02\x00\x00" + b"\xeb\x06" +
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

    def test_reachable_branch_preserves_base(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "small.so"
            blob, sections, symbols = fixture(path)
            # The jump now lands on the final direct reference.
            blob[0x100 + symbols[0]["size"] - 7] = 0
            path.write_bytes(blob)
            with patch("scripts.restructure.original_local_xrefs.read_elf32", return_value=(sections, symbols)):
                edges = extract(path)
            self.assertEqual([edge["object_names"] for edge in edges],
                             [["direct"], ["via_got"], ["direct"]])

    def test_conflicting_join_drops_base(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "small.so"
            blob, sections, symbols = fixture(path)
            code = (b"\xe8" + struct.pack("<i", 0x180 - 0x105) + b"\x81\xc3" +
                    struct.pack("<I", 0x1000 - 0x105) + b"\x74\x02" +
                    b"\x31\xdb" +
                    b"\x8b\x83\x00\x01\x00\x00")
            blob[0x100:0x100 + len(code)] = code
            symbols[0]["size"] = len(code)
            path.write_bytes(blob)
            with patch("scripts.restructure.original_local_xrefs.read_elf32", return_value=(sections, symbols)):
                self.assertEqual(extract(path), [])

    def test_abi_preserved_ebx_survives_ordinary_call(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "small.so"
            blob, sections, symbols = fixture(path)
            code = (b"\xe8" + struct.pack("<i", 0x180 - 0x105) + b"\x81\xc3" +
                    struct.pack("<I", 0x1000 - 0x105) + b"\xe8" +
                    struct.pack("<i", 0x190 - 0x110) +
                    b"\x8b\x83\x00\x01\x00\x00")
            blob[0x100:0x100 + len(code)] = code
            symbols[0]["size"] = len(code)
            path.write_bytes(blob)
            with patch("scripts.restructure.original_local_xrefs.read_elf32", return_value=(sections, symbols)):
                edges = extract(path)
            self.assertEqual([edge["object_names"] for edge in edges], [["direct"]])

    def test_partial_pic_register_write_invalidates_base(self):
        for name, thunk, add_opcode, partial_write, memory_opcode in (
            ("bx", 0x180, b"\x81\xc3", b"\xb3\x00", b"\x8b\x83"),
            ("cx", 0x184, b"\x81\xc1", b"\xb5\x00", b"\x8b\x81"),
            ("dx", 0x188, b"\x81\xc2", b"\xb6\x00", b"\x8b\x82"),
        ):
            with self.subTest(name=name), tempfile.TemporaryDirectory() as directory:
                path = Path(directory) / "small.so"
                blob, sections, symbols = fixture(path)
                symbols.append(dict(name=f"__x86.get_pc_thunk.{name}", address=thunk, size=4,
                                    type=2, binding=0, section_index=1, symbol_index=5))
                code = (b"\xe8" + struct.pack("<i", thunk - 0x105) + add_opcode +
                        struct.pack("<I", 0x1000 - 0x105) + partial_write +
                        memory_opcode + b"\x00\x01\x00\x00")
                blob[0x100:0x100 + len(code)] = code
                symbols[0]["size"] = len(code)
                path.write_bytes(blob)
                with patch("scripts.restructure.original_local_xrefs.read_elf32", return_value=(sections, symbols)):
                    self.assertEqual(extract(path), [])

    def test_caller_saved_pic_registers(self):
        for name, thunk, add_opcode, memory_opcode in (
            ("cx", 0x184, b"\x81\xc1", b"\x8b\x81"),
            ("dx", 0x188, b"\x81\xc2", b"\x8b\x82"),
        ):
            with self.subTest(name=name), tempfile.TemporaryDirectory() as directory:
                path = Path(directory) / "small.so"
                blob, sections, symbols = fixture(path)
                symbols.append(dict(name=f"__x86.get_pc_thunk.{name}", address=thunk, size=4,
                                    type=2, binding=0, section_index=1, symbol_index=5))
                prefix = (b"\xe8" + struct.pack("<i", thunk - 0x105) + add_opcode +
                          struct.pack("<I", 0x1000 - 0x105))
                reference = memory_opcode + b"\x00\x01\x00\x00"
                code = prefix + reference
                blob[0x100:0x100 + len(code)] = code
                symbols[0]["size"] = len(code)
                path.write_bytes(blob)
                with patch("scripts.restructure.original_local_xrefs.read_elf32", return_value=(sections, symbols)):
                    edges = extract(path)
                self.assertEqual([edge["kind"] for edge in edges], [f"direct-e{name}-relative"])

                # An ordinary ABI call may clobber ECX/EDX, unlike EBX.
                code = prefix + b"\xe8" + struct.pack("<i", 0x190 - 0x110) + reference
                blob[0x100:0x100 + len(code)] = code
                symbols[0]["size"] = len(code)
                path.write_bytes(blob)
                with patch("scripts.restructure.original_local_xrefs.read_elf32", return_value=(sections, symbols)):
                    self.assertEqual(extract(path), [])

    def test_loop_backedge_retains_agreed_base(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "small.so"
            blob, sections, symbols = fixture(path)
            code = (b"\xe8" + struct.pack("<i", 0x180 - 0x105) + b"\x81\xc3" +
                    struct.pack("<I", 0x1000 - 0x105) + b"\xeb\x06" +
                    b"\x8b\x83\x00\x01\x00\x00" + b"\x83\xf9\x01" +
                    b"\x75\xf5" + b"\xc3")
            blob[0x100:0x100 + len(code)] = code
            symbols[0]["size"] = len(code)
            path.write_bytes(blob)
            with patch("scripts.restructure.original_local_xrefs.read_elf32", return_value=(sections, symbols)):
                edges = extract(path)
            self.assertEqual([edge["object_names"] for edge in edges], [["direct"]])


if __name__ == "__main__":
    unittest.main()
