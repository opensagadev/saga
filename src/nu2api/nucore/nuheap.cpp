#include "nu2api/nucore/nuheap.h"

struct NUHEAPBLOCK {
    u32 size_and_flags;
    NUHEAPBLOCK *next_free;
    NUHEAPBLOCK *previous_free;
};

struct NUHEAP {
    void *base;
    u32 size;
    u32 allocated_blocks;
    u32 allocated_bytes;
};

enum NUHEAPBLOCKFLAGS { NUHEAPBLOCK_IN_USE = 0x80000000u, NUHEAPBLOCK_SIZE_MASK = 0x7fffffffu };

static u32 NuHeapBlock_GetSize(NUHEAPBLOCK *block) {
    return block->size_and_flags & NUHEAPBLOCK_SIZE_MASK;
}

static NUHEAPBLOCK *NuHeapBlock_GetNextFree(NUHEAPBLOCK *block) {
    return block->next_free;
}

static void NuHeapBlock_SetInUse(NUHEAPBLOCK *block, u32 in_use) {
    if (in_use) {
        block->size_and_flags |= NUHEAPBLOCK_IN_USE;
    } else {
        block->size_and_flags &= NUHEAPBLOCK_SIZE_MASK;
    }
}

static void NuHeapBlock_SetSize(NUHEAPBLOCK *block, u32 size) {
    block->size_and_flags &= NUHEAPBLOCK_IN_USE;
    block->size_and_flags |= size;
}

static void NuHeapBlock_SetNextFree(NUHEAPBLOCK *block, NUHEAPBLOCK *next) {
    block->next_free = next;
}

static void NuHeapBlock_SetPrevFree(NUHEAPBLOCK *block, NUHEAPBLOCK *previous) {
    block->previous_free = previous;
}

static void NuHeapBlock_WriteFooter(NUHEAPBLOCK *block) {
    *(NUHEAPBLOCK **)((u8 *)block + sizeof(NUHEAPBLOCK) + NuHeapBlock_GetSize(block)) = block;
}

static void NuHeapBlock_SetName(NUHEAPBLOCK *block, char *name) {
    // Block names are disabled in the original Android build.
}

static u32 NuHeapBlock_GetInUse(NUHEAPBLOCK *block) {
    if (block->size_and_flags & NUHEAPBLOCK_IN_USE)
        return 1;
    return 0;
}

static NUHEAPBLOCK *NuHeapBlock_GetNext(NUHEAPBLOCK *block) {
    u8 *next = (u8 *)block;
    next += NuHeapBlock_GetSize(block) + sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *);
    return (NUHEAPBLOCK *)next;
}

static NUHEAPBLOCK *NuHeapBlock_GetPrev(NUHEAPBLOCK *block) {
    u8 *footer = (u8 *)block;
    footer -= sizeof(NUHEAPBLOCK *);
    return *(NUHEAPBLOCK **)footer;
}

static void NuHeapBlock_RemoveFromFreeList(NUHEAPBLOCK *block) {
    if (block->previous_free != NULL) {
        block->previous_free->next_free = block->next_free;
    }
    if (block->next_free != NULL) {
        block->next_free->previous_free = block->previous_free;
    }
}

static void *NuHeapBlock_GetUsableMemory(NUHEAPBLOCK *block) {
    return block + 1;
}

static void NuHeapBlock_SplitFreeBlock(NUHEAPBLOCK *block, u32 *size, NUHEAPBLOCK **allocated, NUHEAPBLOCK **remainder,
                                       u32 alignment) {
    u32 remaining_size;
    u8 *base = (u8 *)block;
    u32 total_size = NuHeapBlock_GetSize(block) + sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *);
    usize address;
    usize aligned_address;
    remaining_size = total_size - *size - 2 * (sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *));
    *remainder = (NUHEAPBLOCK *)base;
    *allocated = (NUHEAPBLOCK *)(base + remaining_size + sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *));
    if (alignment != 0) {
        address = (usize)(*allocated + 1);
        aligned_address = (alignment + address - 1) & -(usize)alignment;
        *allocated = (NUHEAPBLOCK *)((u8 *)*allocated + (aligned_address - address));
        remaining_size += aligned_address - address;
        *size += address - aligned_address;
    }
    NuHeapBlock_SetInUse(*allocated, 1);
    NuHeapBlock_SetSize(*allocated, *size);
    NuHeapBlock_SetNextFree(*allocated, NULL);
    NuHeapBlock_SetPrevFree(*allocated, NULL);
    NuHeapBlock_WriteFooter(*allocated);
    NuHeapBlock_SetName(*allocated, NULL);
    NuHeapBlock_SetInUse(*remainder, 0);
    NuHeapBlock_SetSize(*remainder, remaining_size);
    NuHeapBlock_SetNextFree(*remainder, NULL);
    NuHeapBlock_SetPrevFree(*remainder, NULL);
    NuHeapBlock_WriteFooter(*remainder);
    NuHeapBlock_SetName(*remainder, NULL);
}

void *NuHeapAllocAlignedNamed(void *heap, u32 size, u32 alignment, char *name) {
    NUHEAPBLOCK *block;
    NUHEAP *control = (NUHEAP *)heap;
    NUHEAPBLOCK *free_head = (NUHEAPBLOCK *)((u8 *)control->base + sizeof(NUHEAP));
    u32 required_size;
    void *result;
    NUHEAPBLOCK *remainder;
    NUHEAPBLOCK *allocated;
    if (size == 0)
        return NULL;
    size += alignment;
    required_size = size + sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *);
    block = free_head;
    while (true) {
        if (block == NULL)
            return NULL;
        if (NuHeapBlock_GetSize(block) > required_size)
            break;
        block = NuHeapBlock_GetNextFree(block);
    }
    NuHeapBlock_RemoveFromFreeList(block);
    NuHeapBlock_SplitFreeBlock(block, &size, &allocated, &remainder, alignment);
    NuHeapBlock_SetName(allocated, name);
    if (free_head->next_free != NULL) {
        NuHeapBlock_SetPrevFree(free_head->next_free, remainder);
    }
    NuHeapBlock_SetNextFree(remainder, free_head->next_free);
    NuHeapBlock_SetPrevFree(remainder, free_head);
    NuHeapBlock_SetNextFree(free_head, remainder);
    control->allocated_blocks++;
    control->allocated_bytes += NuHeapBlock_GetSize(allocated) + sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *);
    result = NuHeapBlock_GetUsableMemory(allocated);
    return result;
}

void *NuHeapAlloc(void *heap, u32 size) {
    return NuHeapAllocAlignedNamed(heap, size, 4, NULL);
}

void *NuHeapAllocNamed(void *heap, u32 size, char *name) {
    return NuHeapAllocAlignedNamed(heap, size, 4, name);
}

void *NuHeapAllocAligned(void *heap, u32 size, u32 alignment) {
    return NuHeapAllocAlignedNamed(heap, size, alignment, NULL);
}

void NuHeapFree(void *heap, void *ptr) {
    NUHEAP *control = (NUHEAP *)heap;
    NUHEAPBLOCK *free_head = (NUHEAPBLOCK *)((u8 *)control->base + sizeof(NUHEAP));
    u8 *address = (u8 *)ptr;
    NUHEAPBLOCK *block;
    NUHEAPBLOCK *next;
    NUHEAPBLOCK *previous;

    if (ptr == NULL)
        return;
    if (address < (u8 *)control->base || address > (u8 *)control->base + control->size)
        return;
    block = (NUHEAPBLOCK *)(address - sizeof(NUHEAPBLOCK));
    NuHeapBlock_SetInUse(block, 0);
    control->allocated_blocks--;
    control->allocated_bytes -= NuHeapBlock_GetSize(block);
    control->allocated_bytes -= sizeof(NUHEAPBLOCK);
    control->allocated_bytes -= sizeof(NUHEAPBLOCK *);

    if (free_head->next_free != NULL) {
        NuHeapBlock_SetPrevFree(free_head->next_free, block);
    }
    NuHeapBlock_SetNextFree(block, free_head->next_free);
    NuHeapBlock_SetPrevFree(block, free_head);
    NuHeapBlock_SetNextFree(free_head, block);

    next = NuHeapBlock_GetNext(block);
    if (!NuHeapBlock_GetInUse(next)) {
        NuHeapBlock_RemoveFromFreeList(next);
        NuHeapBlock_SetSize(block, NuHeapBlock_GetSize(block) + NuHeapBlock_GetSize(next) + sizeof(NUHEAPBLOCK) +
                                       sizeof(NUHEAPBLOCK *));
        NuHeapBlock_WriteFooter(block);
    }
    previous = NuHeapBlock_GetPrev(block);
    if (!NuHeapBlock_GetInUse(previous)) {
        NuHeapBlock_RemoveFromFreeList(block);
        NuHeapBlock_SetSize(previous, NuHeapBlock_GetSize(previous) + NuHeapBlock_GetSize(block) + sizeof(NUHEAPBLOCK) +
                                          sizeof(NUHEAPBLOCK *));
        NuHeapBlock_WriteFooter(previous);
    }
}

void *NuHeapCreate(VARIPTR *buffer, void *buffer_end, u32 size) {
    u8 *base;
    NUHEAP *control;
    NUHEAPBLOCK *free_head;
    NUHEAPBLOCK *start;
    NUHEAPBLOCK *first;
    NUHEAPBLOCK *end;
    enum { block_overhead = sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *) };

    if (size < sizeof(NUHEAP) + 4 * block_overhead)
        return NULL;
    base = (u8 *)buffer->void_ptr;
    buffer->addr += size;
    control = (NUHEAP *)base;
    free_head = (NUHEAPBLOCK *)(base + sizeof(NUHEAP));
    start = (NUHEAPBLOCK *)(base + sizeof(NUHEAP) + block_overhead);
    first = (NUHEAPBLOCK *)(base + sizeof(NUHEAP) + 2 * block_overhead);
    end = (NUHEAPBLOCK *)(base + size - block_overhead);

    NuHeapBlock_SetInUse(free_head, 0);
    NuHeapBlock_SetSize(free_head, 0);
    NuHeapBlock_SetNextFree(free_head, first);
    NuHeapBlock_SetPrevFree(free_head, NULL);
    NuHeapBlock_WriteFooter(free_head);
    NuHeapBlock_SetName(free_head, NULL);

    NuHeapBlock_SetInUse(start, 1);
    NuHeapBlock_SetSize(start, 0);
    NuHeapBlock_SetNextFree(start, NULL);
    NuHeapBlock_SetPrevFree(start, NULL);
    NuHeapBlock_WriteFooter(start);
    NuHeapBlock_SetName(start, NULL);

    NuHeapBlock_SetInUse(end, 1);
    NuHeapBlock_SetSize(end, 0);
    NuHeapBlock_SetNextFree(end, NULL);
    NuHeapBlock_SetPrevFree(end, NULL);
    NuHeapBlock_WriteFooter(end);
    NuHeapBlock_SetName(end, NULL);

    NuHeapBlock_SetInUse(first, 0);
    NuHeapBlock_SetSize(first, size - sizeof(NUHEAP) - 4 * block_overhead);
    NuHeapBlock_SetNextFree(first, NULL);
    NuHeapBlock_SetPrevFree(first, free_head);
    NuHeapBlock_WriteFooter(first);
    NuHeapBlock_SetName(first, NULL);

    control->base = base;
    control->size = size;
    control->allocated_blocks = 0;
    control->allocated_bytes = 0;
    return control;
}

u32 NuHeapGetAllocatedBlockCount(void *heap) {
    NUHEAP *control = (NUHEAP *)heap;
    return control->allocated_blocks;
}

static void NuHeap_PrintAllocations(void *heap) {
    // Allocation reporting is disabled in the original Android build.
}

void NuHeapDestroy(void *heap) {
    NuHeap_PrintAllocations(heap);
}

void *NuHeapDefragAllocation(void *heap, void *ptr) {
    i32 i;
    NUHEAP *control = (NUHEAP *)heap;
    u8 *address = (u8 *)ptr;
    NUHEAPBLOCK *block;
    NUHEAPBLOCK *next;
    u32 size;
    u32 next_size;
    u8 *source;
    u8 *destination;
    void *result;
    NUHEAPBLOCK *free_head;
    if (ptr == NULL)
        return NULL;
    block = (NUHEAPBLOCK *)(address - sizeof(NUHEAPBLOCK));
    next = NuHeapBlock_GetNext(block);
    if (NuHeapBlock_GetInUse(next))
        return ptr;
    size = NuHeapBlock_GetSize(block);
    next_size = NuHeapBlock_GetSize(next);
    NuHeapBlock_RemoveFromFreeList(next);
    source = (u8 *)NuHeapBlock_GetUsableMemory(block);
    destination = address + next_size + 2 * sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *);
    for (i = size - 1; i >= 0; i--) {
        destination[i] = source[i];
    }
    // These offsets are relative to the supplied payload pointer in the
    // original, including the position of the replacement free header.
    block = (NUHEAPBLOCK *)(address + next_size + sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *));
    NuHeapBlock_SetInUse(block, 1);
    NuHeapBlock_SetSize(block, size);
    NuHeapBlock_SetNextFree(block, NULL);
    NuHeapBlock_SetPrevFree(block, NULL);
    NuHeapBlock_WriteFooter(block);
    result = NuHeapBlock_GetUsableMemory(block);
    block = (NUHEAPBLOCK *)address;
    NuHeapBlock_SetInUse(block, 0);
    NuHeapBlock_SetSize(block, next_size);
    NuHeapBlock_SetNextFree(block, NULL);
    NuHeapBlock_SetPrevFree(block, NULL);
    NuHeapBlock_WriteFooter(block);
    free_head = (NUHEAPBLOCK *)((u8 *)control->base + sizeof(NUHEAP));
    if (free_head->next_free != NULL) {
        NuHeapBlock_SetPrevFree(free_head->next_free, block);
    }
    NuHeapBlock_SetNextFree(block, free_head->next_free);
    NuHeapBlock_SetPrevFree(block, free_head);
    NuHeapBlock_SetNextFree(free_head, block);
    return result;
}

u32 NuHeapGetFreeBlockCount(void *heap) {
    NUHEAPBLOCK *block;
    u32 count;
    NUHEAP *control = (NUHEAP *)heap;
    block = (NUHEAPBLOCK *)((u8 *)control->base + sizeof(NUHEAP));
    count = 0;
    while (true) {
        if (block == NULL)
            break;
        count++;
        block = NuHeapBlock_GetNextFree(block);
    }
    return count;
}

u32 NuHeapGetTotalAllocated(void *heap) {
    NUHEAP *control = (NUHEAP *)heap;
    return control->allocated_bytes;
}

u32 NuHeapGetTotalFree(void *heap) {
    NUHEAPBLOCK *block;
    u32 size;
    NUHEAP *control = (NUHEAP *)heap;
    block = (NUHEAPBLOCK *)((u8 *)control->base + sizeof(NUHEAP));
    size = 0;
    while (true) {
        if (block == NULL)
            break;
        size += NuHeapBlock_GetSize(block);
        block = NuHeapBlock_GetNextFree(block);
    }
    return size;
}
