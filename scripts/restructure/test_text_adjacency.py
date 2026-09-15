"""Original-only text adjacency and withheld-label calibration."""

import unittest

from scripts.restructure.text_adjacency import text_adjacency_edges, calibrate_text_edges


def func(index, address, size=8, section=".text"):
    return {"symbol_index": index, "address": address, "size": size, "type": 2, "section": section}


class TextEdgesTest(unittest.TestCase):
    def test_flat_edges_aliases_and_gap(self):
        symbols = [func(3, 0x130), func(2, 0x108), func(1, 0x100),
                   func(4, 0x130), func(5, 0x138), func(6, 0x140, section=".plt")]
        edges = text_adjacency_edges(symbols)
        self.assertEqual([edge["id"] for edge in edges],
                         ["text-adj:00000100:00000108", "text-adj:00000130:00000138"])
        self.assertEqual(edges[1]["from_symbol_indices"], [3, 4])
        self.assertEqual(edges[0]["gap_bytes"], 0)
        self.assertIn("not TU ownership", edges[0]["status"])

    def test_zero_size_and_overlap_never_link(self):
        self.assertEqual(text_adjacency_edges([func(1, 0x100, 0), func(2, 0x108)]), [])
        self.assertEqual(text_adjacency_edges([func(1, 0x100, 16), func(2, 0x108)]), [])

    def test_long_chain_is_only_individual_edges(self):
        edges = text_adjacency_edges([func(i + 1, 0x100 + i * 8) for i in range(17)])
        self.assertEqual(len(edges), 16)
        self.assertTrue(all("site_count" not in edge for edge in edges))

    def test_edge_precision_and_same_file_recall(self):
        symbols = [func(1, 0x100), func(2, 0x108), func(3, 0x110),
                   func(4, 0x140), func(5, 0x148)]
        score = calibrate_text_edges(symbols, text_adjacency_edges(symbols),
                                    {1: "a", 2: "a", 3: "b", 4: "b", 5: "b"})
        self.assertEqual(score["predicted_edges_total"], 3)
        self.assertEqual(score["assessable_adjacent_site_pairs"], 4)
        self.assertEqual(score["edge_precision"], 2 / 3)
        self.assertEqual(score["same_file_edge_recall"], 2 / 3)


if __name__ == "__main__":
    unittest.main()
