#include "nu2api_nusound_types.h"

static_assert(sizeof(void *) != 4 || offsetof(NuSoundEffect, reference_head) == 0x38,
              "effect reference-list head must retain its Android offset");

NuSoundEffect::~NuSoundEffect() {
    ReferenceNode *node = reference_head->next;
    while (node != reference_tail) {
        --reference_count;
        ReferenceNode *previous = node->prev;
        ReferenceNode *next = node->next;
        if (previous != NULL) previous->next = next;
        if (next != NULL) next->prev = previous;

        ManagedReference &ref = node->reference;
        if (ref.object != NULL) {
            if (ref.next == &ref) {
                ref.object->references = NULL;
            } else {
                ManagedReference *next_reference = ref.next;
                ManagedReference *previous_reference = ref.previous;
                next_reference->previous = previous_reference;
                previous_reference->next = next_reference;
                if (ref.object->references == &ref) ref.object->references = next_reference;
            }
            ref.object = NULL;
            ref.next = NULL;
            ref.previous = NULL;
        }
        NuMemoryGet()->GetThreadMem()->BlockFree(node, 0);
        node = reference_head->next;
    }

    ManagedReference *head = references;
    if (head != NULL) {
        while (head->next != head) {
            ManagedReference *ref = head->next;
            ref->object = NULL;
            head->next = ref->next;
            ref->previous = NULL;
            ref->next = NULL;
        }
        head->next = NULL;
        head->object = NULL;
        head->previous = NULL;
        references = NULL;
    }
}

bool NuSoundEffect::Initialise() {
    return true;
}
