#include "nu2api/nucore/numem.h"

struct NUMEMHIGHBLOCK {
    NUMEMHIGHBLOCK *next;
    u32 units;
    u8 reserved[8];
};

static NUMEMHIGHBLOCK *freep;

void NuAllocHighInit(usize buffer, u32 size) {
    // Preserve the low-bit masks emitted by the original, including its
    // unusual treatment of unaligned inputs.
    if (buffer & 15) {
        buffer = (buffer + 15) & 15;
        size -= 15;
    }
    if (size & 15) {
        size = (size - 15) & 15;
    }
    freep = (NUMEMHIGHBLOCK *)buffer;
    freep->next = freep;
    freep->units = size;
}

void *NuAllocHigh(u32 size) {
    NUMEMHIGHBLOCK *block;
    NUMEMHIGHBLOCK *previous;
    u32 units;
    units = (size + sizeof(NUMEMHIGHBLOCK) - 1) / sizeof(NUMEMHIGHBLOCK) + 1;
    previous = freep;
    block = previous->next;
    while (true) {
        if (block->units >= units) {
            if (block->units == units) {
                previous->next = block->next;
            } else {
                block->units -= units;
                block += block->units;
                block->units = units;
            }
            freep = previous;
            return block + 1;
        }
        if (block == freep)
            return NULL;
        previous = block;
        block = block->next;
    }
}

void NuFreeHigh(void *ptr) {
    NUMEMHIGHBLOCK *previous;
    NUMEMHIGHBLOCK *block = (NUMEMHIGHBLOCK *)ptr - 1;
    previous = freep;
    while (!(block > previous && block < previous->next)) {
        if (previous >= previous->next && (block > previous || block < previous->next))
            break;
        previous = previous->next;
    }
    if (block + block->units == previous->next) {
        block->units += previous->next->units;
        block->next = previous->next->next;
    } else {
        block->next = previous->next;
    }
    if (previous + previous->units == block) {
        previous->units += block->units;
        previous->next = block->next;
    } else {
        previous->next = block;
    }
    freep = previous;
}
