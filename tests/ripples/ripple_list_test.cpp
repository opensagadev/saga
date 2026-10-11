#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/numath/nuvec.h"

#include <cstdio>
#include <cstdlib>

void AddRipple(ripple_set_s *set, NUMTX *matrix, f32 size, f32 growth, f32 lifetime, f32 delay, RGBA start_color,
               RGBA end_color, i32 flags, numtl_s *material, NUVEC *velocity);

// The production default-velocity provider. Rendering and asset loading are not used.
NUVEC v000 = {};

static void Require(bool condition, const char *message) {
    if (!condition) {
        std::fprintf(stderr, "Ripple list regression: %s\n", message);
        std::exit(1);
    }
}

static void CheckRing(ripple_node_s *head, i32 count) {
    Require((head == NULL) == (count == 0), "empty ring identity");
    ripple_node_s *node = head;
    for (i32 i = 0; i < count; ++i) {
        Require(node != NULL && node->next != NULL && node->previous != NULL, "non-null links");
        Require(node->next->previous == node && node->previous->next == node, "bidirectional links");
        node = node->next;
        Require(i == count - 1 || node != head, "ring must not close early");
    }
    Require(node == head, "ring length");
}

int main() {
    static_assert(sizeof(void *) == 4, "Actual Linux i686 layouts");
    i32 insertions = 0;
    for (i32 capacity = 1; capacity <= 8; ++capacity) {
        ripple_node_s nodes[8] = {};
        ripple_set_s storage = {};
        ripple_set_s *set = &storage;
        set->count = capacity;
        set->nodes = set->free_head = nodes;
        for (i32 i = 0; i < capacity; ++i) {
            nodes[i].next = &nodes[(i + 1) % capacity];
            nodes[i].previous = &nodes[(i + capacity - 1) % capacity];
        }
        CheckRing(set->free_head, capacity);
        NUMTX matrix = {};
        numtl_s material = {};
        NUVEC velocity = {1.0f, 2.0f, 3.0f};
        RGBA color = {};
        for (i32 i = 0; i < capacity * 3; ++i) {
            ripple_node_s *expected = i < capacity ? set->free_head : set->oldest;
            AddRipple(set, &matrix, 1.0f + i, 2.0f, 3.0f, 4.0f, color, color, 5, &material,
                      i % 2 == 0 ? &velocity : NULL);
            Require(set->newest == expected, "consume free head or recycle oldest");
            Require(set->active_count == (i < capacity ? i + 1 : capacity), "active count saturates");
            Require(set->newest->next == set->oldest, "oldest follows newest");
            Require(expected->size == 1.0f + i && expected->material == &material, "new ripple payload");
            Require(expected->velocity.x == (i % 2 == 0 ? 1.0f : 0.0f), "velocity provider");
            CheckRing(set->oldest, set->active_count);
            CheckRing(set->free_head, capacity - set->active_count);
            ++insertions;
        }
    }
    std::printf("Ripple list regression: PASS (%d insertions; capacities 1..8, fill and recycle)\n", insertions);
    return 0;
}
