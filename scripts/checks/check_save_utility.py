#!/usr/bin/env python3
"""Integration checks: python3 scripts/checks/check_save_utility.py /absolute/saga_native."""

import os
from pathlib import Path
import random
import re
import struct
import subprocess
import sys
import tempfile
import unittest


BINARY = str(Path(sys.argv.pop(1)).resolve())
HEADER = 0x2028
PAYLOAD = 0x7E58


class SaveUtilityTest(unittest.TestCase):
    def setUp(self):
        self.directory = tempfile.TemporaryDirectory(prefix="saga-save-test-")
        self.addCleanup(self.directory.cleanup)
        self.root = Path(self.directory.name)
        self.save = self.root / "game.sav"

    def run_save(self, *args, success=True):
        env = dict(os.environ, ASAN_OPTIONS="detect_leaks=0")
        result = subprocess.run([BINARY, "save", *map(str, args)], capture_output=True, env=env, cwd=self.root)
        self.assertEqual(result.returncode == 0, success, result.stderr.decode(errors="replace"))
        return result.stdout

    def assert_checksum(self, data, size=PAYLOAD, prefix=0):
        start = HEADER + prefix
        expected = (0x5C0999 + sum(struct.unpack_from(f"<{size // 4}I", data, start))) & 0xFFFFFFFF
        self.assertEqual(struct.unpack_from("<I", data, start + size)[0], expected)

    def test_creation_and_typed_edits(self):
        self.run_save("create", self.save, "coins=12345", "completion=32769",
                      "area_save[2].minikit_count=10", "episode_save[1].superstory_time_limit=12.5",
                      "customizer.primary_name=text:TEST", "customizer.secondary_pieces[8]=-123",
                      "character_save[339]=3")
        data = self.save.read_bytes()
        self.assertEqual(len(data), HEADER + PAYLOAD + 8)
        self.assertEqual(struct.unpack_from("<III", data), (0x52474D48, 1, HEADER))
        self.assertEqual(data[HEADER + 1], 5)
        self.assertEqual(struct.unpack_from("<I", data, HEADER + 0x7C20)[0], 12345)
        self.assertEqual(data[HEADER + 0x782C + 2 * 12 + 5], 10)
        self.assertEqual(struct.unpack_from("<f", data, HEADER + 0x7B8C + 12)[0], 12.5)
        self.assertEqual(data[HEADER + 0x7C44:HEADER + 0x7C49], b"TEST\0")
        self.assertEqual(struct.unpack_from("<h", data, HEADER + 0x7C30 + 0x48)[0], -123)
        self.assertEqual(data[HEADER + 0x7D04 + 339], 3)
        self.assertEqual(struct.unpack_from("<I", data, len(data) - 4)[0], 0xFFFF8001)
        self.assert_checksum(data)

    def test_lossless_export_and_template(self):
        self.run_save("create", self.save)
        data = bytearray(self.save.read_bytes())
        rng = random.Random(20260908)
        data[0x18:] = bytes(rng.randrange(256) for _ in data[0x18:])
        # Unknown fields, a non-zero extra-data prefix, and a NaN with payload
        # must survive a full listing/import, even with intentionally bad checksum.
        data[0x2A:0x82A] = bytes(range(256)) * 8
        struct.pack_into("<I", data, HEADER + 0x7C2C, 0x7FC01234)
        struct.pack_into("<I", data, HEADER + 0x7B8C, 1)  # smallest subnormal float
        data[HEADER:HEADER] = b"odd"
        struct.pack_into("<I", data, 0x14, 3)
        self.save.write_bytes(data)
        params = self.root / "properties.txt"
        params.write_bytes(self.run_save("list", self.save))
        recreated = self.root / "recreated.sav"
        self.run_save("create", recreated, "--from", self.save, "--params", params, "--keep-derived")
        self.assertEqual(recreated.read_bytes(), data)
        self.run_save("edit", recreated, "coins=999")
        changed = recreated.read_bytes()
        allowed = set(range(HEADER + 3 + 0x7C20, HEADER + 3 + 0x7C24)) | set(range(len(data) - 8, len(data)))
        self.assertTrue(all(a == b or i in allowed for i, (a, b) in enumerate(zip(data, changed))))
        self.assert_checksum(changed, prefix=3)

    def test_invalid_requests_preserve_file(self):
        self.run_save("create", self.save)
        original = self.save.read_bytes()
        for assignment in ("coins=-1", "coins=4294967296", "save_version=256", "missing=1",
                           "character_save[340]=1", "byte[999999]=1", "coins=1oops",
                           "customizer.secondary_pieces[0]=-32769", "header.extradata_offset=1",
                           "header.size=1", "customizer.primary_name=hex:gg", "coins="):
            self.run_save("edit", self.save, "coins=2", assignment, success=False)
            self.assertEqual(self.save.read_bytes(), original)
        self.run_save("create", self.save, success=False)
        self.assertEqual(self.save.read_bytes(), original)
        self.assertEqual(list(self.root.glob("*.incomplete.*")), [])

    def test_options_and_output(self):
        self.run_save("create", self.save, "--options", "options.left_control_x=0.25", "options.music_enabled=1")
        data = self.save.read_bytes()
        self.assertEqual(len(data), HEADER + 24 + 8)
        self.assert_checksum(data, size=24)
        self.assertEqual(data[-4:], b"\xff" * 4)
        other = self.root / "other.sav"
        self.run_save("edit", self.save, "--output", other, "options.music_enabled=0")
        self.assertEqual(self.save.read_bytes(), data)
        self.run_save("edit", self.save, "--output", other, success=False)

    def test_malformed_input(self):
        self.run_save("create", self.save)
        data = self.save.read_bytes()
        for bad in (b"", data[:HEADER - 1], data[:-1], b"BAD!" + data[4:],
                    data[:20] + b"\xff" * 4 + data[24:]):
            self.save.write_bytes(bad)
            self.run_save("list", self.save, success=False)
            self.run_save("edit", self.save, "coins=1", success=False)
            self.assertEqual(self.save.read_bytes(), bad)

    def test_default_path(self):
        default = self.root / "res/SavedGames/SaveGame0.LEGO Star Wars - The Complete Saga_SavedGame"
        default.parent.mkdir(parents=True)
        self.run_save("create", "coins=123")
        self.assertTrue(default.exists())
        self.assertEqual(self.run_save(), self.run_save("list", default))
        self.assertIn(b"coins=123\n", self.run_save("list", "--filter", "coins"))
        self.run_save("edit", "coins=456")
        self.assertIn(b"coins=456\n", self.run_save("list", "--filter", "coins"))
        original = default.read_bytes()
        self.run_save("create", "coins=789", success=False)
        self.assertEqual(default.read_bytes(), original)
        self.assertIn(b"res/SavedGames/SaveGame0.", self.run_save("--help"))
        self.run_save("create", "--path", "--save=test.sav", "coins=789")
        self.assertIn(b"coins=789\n", self.run_save("list", "--path", "--save=test.sav", "--filter", "coins"))
        self.run_save("list", "--path", success=False)

    def test_schema(self):
        # A schema is available without any save or game assets on disk.
        table = self.run_save("schema", "--filter", "coins").decode()
        self.assertNotIn("\t", table)
        table_lines = [line for line in table.splitlines() if line.startswith(("|", "+"))]
        self.assertTrue(table_lines)
        self.assertEqual(len({len(line) for line in table_lines}), 1)
        self.assertIn("| Property", table)
        self.assertIn("| coins", table)
        self.assertIn("Stored stud/coin balance", table)
        schema = self.run_save("schema", "--tsv").decode()
        self.assertIn("Completion points, not a percentage", schema)
        self.assertIn("Unknown/reserved field", schema)
        rows = [line.split("\t") for line in schema.splitlines() if line and not line.startswith("#")][1:]
        self.assertTrue(all(len(row) == 8 and row[7] for row in rows))
        self.assertLess(len(rows), 120)
        self.assertIn("area_save[0..71].complete\tu8", schema)
        self.assertIn("episode_save[0..5].flags\tu32", schema)
        self.assertIn("hint_completion_bits\tbits192", schema)
        self.assertIn("customizer.secondary_pieces[0..8]\ti16", schema)
        covered = set()
        expanded = set()
        for name, _, offset, size, count, stride, _, _ in rows:
            match = re.search(r"\[(\d+)\.\.(\d+)\]", name)
            self.assertEqual(int(count), int(match[2]) - int(match[1]) + 1 if match else 1)
            for i in range(int(count)):
                start = int(offset, 16) + i * int(stride)
                region = set(range(start, start + int(size)))
                self.assertFalse(covered & region)
                covered |= region
                expanded.add(name[:match.start()] + f"[{int(match[1]) + i}]" + name[match.end():] if match else name)
        self.assertEqual(covered, set(range(HEADER + PAYLOAD + 8)))
        self.run_save("create", self.save)
        listed = self.run_save("list", self.save).decode()
        self.assertEqual(expanded, {line.split("=", 1)[0] for line in listed.splitlines() if not line.startswith("#")})
        filtered = self.run_save("schema", "--filter", "coins", "--tsv")
        self.assertIn(b"coins\tu32\t0x9c48\t4\t1\t0\t0..4294967295", filtered)
        self.assertNotIn(b"completion\tu16", filtered)
        options = self.run_save("schema", "--options", "--tsv")
        self.assertIn(b"SUPEROPTIONS_s", options)
        self.assertIn(b"options.music_enabled\tu8", options)
        data = bytearray(self.save.read_bytes())
        data[HEADER:HEADER] = b"abc"
        struct.pack_into("<I", data, 20, 3)
        self.save.write_bytes(data)
        self.assertIn(b"coins\tu32\t0x9c4b", self.run_save("schema", self.save, "--filter", "coins", "--tsv"))
        self.run_save("schema", self.save, "--options", success=False)
        self.run_save("list", "--filter", success=False)

    def test_verified_offsets_and_symbolic_flags(self):
        self.run_save("create", self.save, "reward_flags=100_PERCENT|ALL_GOLD_BRICKS|128",
                      "character_save[17]=AVAILABLE|UNLOCKED", "area_save[1].minikit_complete=COMPLETE",
                      "area_save[1].red_brick_collected=COMPLETE", "level_save[365].arcade_flags=BATTLE|HUNT",
                      "level_save[365].minikit_names[9]=text:kit9", "mission_save.best_times[19]=19.5",
                      "mission_save.completed[19]=COMPLETE", "extra_purchased_bits[1]=scorex10|fastbuild",
                      "hint_completion_bits[3]=BIT_0|BIT_31", "options_save.player1_rumble=ON")
        data = self.save.read_bytes()
        self.assertEqual(data[HEADER + 0x7C27], 131)
        self.assertEqual(data[HEADER + 0x7D04 + 17], 3)
        self.assertEqual(data[HEADER + 0x782C + 12 + 4], 1)
        self.assertEqual(data[HEADER + 0x782C + 12 + 6], 1)
        self.assertEqual(data[HEADER + 0x11 + 365 * 84 + 83], 5)
        self.assertEqual(struct.unpack_from("<f", data, HEADER + 0x7CA0 + 19 * 4)[0], 19.5)
        self.assertEqual(data[HEADER + 0x7CF0 + 19], 1)
        self.assertEqual(struct.unpack_from("<I", data, HEADER + 0x7C04)[0], (1 << 11) | 2)
        self.assertEqual(struct.unpack_from("<I", data, HEADER + 0x7C14)[0], 0x80000001)
        self.assertIn(b"reward_flags=100_PERCENT|ALL_GOLD_BRICKS|128", self.run_save("list", self.save))
        for value in ("BOGUS", "100_PERCENT|", "100_PERCENT|4294967296", "100_PERCENT|-1"):
            self.run_save("edit", self.save, "reward_flags=" + value, success=False)
            self.assertEqual(self.save.read_bytes(), data)
        other = self.root / "options.sav"
        self.run_save("create", other, "--options", "options.store_pack_flags=EPISODE_II|SITH",
                      "options.store_bundle_flags=PREQUEL|COMPLETE", "options.dpad_locked=ON")
        options = other.read_bytes()
        self.assertEqual(struct.unpack_from("<H", options, HEADER)[0], 1025)
        self.assertEqual(options[HEADER + 21], 5)
        self.assertEqual(options[HEADER + 3], 1)

    def test_logical_masks_and_named_bit_listing(self):
        self.run_save("create", self.save,
                      "shop_character_purchased_bits=gonkdroid|wookie|slave1|BIT_127",
                      "extra_purchased_bits=scorex2|scorex10",
                      "hint_completion_bits=console.AutoJump_1568|touch.AutoJump_1568|BIT_191")
        data = self.save.read_bytes()
        self.assertEqual(int.from_bytes(data[HEADER + 0x7BE0:HEADER + 0x7BF0], "little"),
                         1 | (1 << 32) | (1 << 89) | (1 << 127))
        self.assertEqual(int.from_bytes(data[HEADER + 0x7C00:HEADER + 0x7C08], "little"), (1 << 31) | (1 << 43))
        self.assertEqual(int.from_bytes(data[HEADER + 0x7C08:HEADER + 0x7C20], "little"),
                         (1 << 3) | (1 << 99) | (1 << 191))
        listed = self.run_save("list", self.save, "--filter", "shop_character").decode()
        self.assertIn("0=gonkdroid", listed)
        self.assertIn("32=wookie", listed)
        self.assertIn("89=slave1", listed)
        self.assertIn("127=BIT_127", listed)
        self.assertIn("shop_character_purchased_bits=gonkdroid|wookie|slave1|BIT_127", listed)
        self.assertNotIn("shop_character_purchased_bits[", listed)
        schema = self.run_save("schema", "--tsv").decode()
        for name, width in (("shop_hint_purchased_bits", 96), ("shop_character_purchased_bits", 128),
                            ("extra_unlocked_bits", 64), ("extra_purchased_bits", 64), ("hint_completion_bits", 192)):
            self.assertIn(f"{name}\tbits{width}", schema)
            self.run_save("edit", self.save, f"{name}={(1 << width) - 1}")
            original = self.save.read_bytes()
            for value in (str(1 << width), "0x1" + "0" * (width // 4), f"BIT_{width}", "gonkdroid|", "-1"):
                self.run_save("edit", self.save, f"{name}={value}", success=False)
                self.assertEqual(self.save.read_bytes(), original)
            self.run_save("edit", self.save, f"{name}=0x" + "f" * (width // 4))
            self.assertEqual(self.save.read_bytes(), original)
        params = self.root / "masks.params"
        params.write_bytes(self.run_save("list", self.save))
        other = self.root / "masks.sav"
        self.run_save("create", other, "--from", self.save, "--params", params, "--keep-derived")
        self.assertEqual(other.read_bytes(), self.save.read_bytes())


if __name__ == "__main__":
    unittest.main()
