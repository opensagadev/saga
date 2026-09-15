"""Weak original .text adjacency evidence, never translation-unit ownership."""

from collections import defaultdict

MAX_GAP = 16


def text_adjacency_edges(symbols: list[dict], *, max_gap: int = MAX_GAP) -> list[dict]:
    """Report bounded gaps between neighboring distinct function sites.

    Same-address aliases form one site. A zero-sized or conflicting-size site
    cannot establish an end address, so it supplies no outgoing edge.
    """
    if max_gap < 0:
        raise ValueError("max_gap must be nonnegative")
    by_address: dict[int, list[dict]] = defaultdict(list)
    for symbol in symbols:
        if symbol.get("section", ".text") == ".text" and symbol["type"] == 2:
            by_address[symbol["address"]].append(symbol)
    sites = []
    for address, members in sorted(by_address.items()):
        sizes = {member["size"] for member in members if member.get("size", 0) > 0}
        sites.append((address, sorted(member["symbol_index"] for member in members),
                      next(iter(sizes)) if len(sizes) == 1 else None))
    edges = []
    for (left, left_indices, size), (right, right_indices, _) in zip(sites, sites[1:]):
        gap = right - left - size if size is not None else None
        if gap is not None and 0 <= gap <= max_gap:
            edges.append({
                "id": f"text-adj:{left:08x}:{right:08x}",
                "status": "weak layout evidence; not TU ownership",
                "kind": "bounded-function-address-gap",
                "from_address": left,
                "to_address": right,
                "from_symbol_indices": left_indices,
                "to_symbol_indices": right_indices,
                "gap_bytes": gap,
                "max_gap_bytes": max_gap,
            })
    return edges


def calibrate_text_edges(symbols: list[dict], edges: list[dict], file_labels: dict[int, str]) -> dict:
    """Compare edges to withheld current ELF local FILE labels only."""
    by_address: dict[int, list[int]] = defaultdict(list)
    for symbol in symbols:
        if symbol.get("section", ".text") == ".text" and symbol["type"] == 2:
            by_address[symbol["address"]].append(symbol["symbol_index"])
    labeled_sites = []
    for address, indices in sorted(by_address.items()):
        labels = {file_labels[index] for index in indices if index in file_labels}
        labeled_sites.append((address, next(iter(labels)) if len(labels) == 1 else None))
    predicted = {edge["id"] for edge in edges}
    assessable = predicted_assessable = true_edges = correct = 0
    for (left, left_label), (right, right_label) in zip(labeled_sites, labeled_sites[1:]):
        if left_label is None or right_label is None:
            continue
        assessable += 1
        same_file = left_label == right_label
        true_edges += same_file
        selected = f"text-adj:{left:08x}:{right:08x}" in predicted
        predicted_assessable += selected
        correct += selected and same_file
    return {
        "predicted_edges_total": len(predicted),
        "assessable_adjacent_site_pairs": assessable,
        "predicted_assessable_edges": predicted_assessable,
        "predicted_edge_label_coverage": predicted_assessable / len(predicted) if predicted else None,
        "same_file_adjacent_pairs": true_edges,
        "predicted_same_file_edges": correct,
        "edge_precision": correct / predicted_assessable if predicted_assessable else None,
        "same_file_edge_recall": correct / true_edges if true_edges else None,
        "limitation": "STT_FILE labels cover local symbols only; edge precision is insufficient for TU ownership",
    }
