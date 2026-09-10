#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/nuheap.h"
#include "nu2api/nucore/numemory.h"
#include <string.h>
#include <stdlib.h>

// The original Android implementation does not change page protection.
extern "C" i32 NuPhysicalProtect(void) {
    return 0;
}

struct NUMEMALLOCATION {
    NUMEMALLOCATION *next;
    u8 unknown_04[12];
    u32 size;
    char guard[12];
};

static NUMEMALLOCATION *alloc_list;

void NuMemDumpFn(i32 mode) {
    NUMEMALLOCATION *block = alloc_list;
    VARIPTR tail;
    void *tail_data;
    // The Android binary retains the traversal and metadata calculations,
    // but contains no calls to print the allocation records.
    while (block) {
        tail.void_ptr = block + 1;
        tail.addr += block->size;
        tail_data = (u8 *)tail.void_ptr + 12;
        block = block->next;
    }
}

void *NuMemValidateFn(void) {
    i32 result;
    NUMEMALLOCATION *block;
    VARIPTR tail;
    void *tail_data;
    block = alloc_list;
    while (block) {
        tail.void_ptr = block + 1;
        tail.addr += block->size;
        // The original also computes the data following the tail guard;
        // its diagnostic reporting body is absent in this build.
        tail_data = (u8 *)tail.void_ptr + 12;
        result = memcmp(block->guard, "MEMBLKHEADER", 12);
        if (result != 0) {
            return block;
        }
        result = memcmp(tail.void_ptr, "MEMBLKTAIL01", 12);
        if (result != 0) {
            return tail.void_ptr;
        }
        block = block->next;
    }
    return NULL;
}

void NuMemFlushFn(void) {
    NUMEMALLOCATION *block = alloc_list;
    while (block) {
        NUMEMALLOCATION *next = block->next;
        free(block);
        block = next;
    }
}

static NUMEMEXTERNAL *memexternal;
static NUMEMEXTERNAL memext;
static NUMEMDISCARDABLE *discardbuff;
static isize peakallocaddr;
static i32 totalloc;

extern "C" {
    void *NuMem_Heap;
    extern i32 highallocaddr;
}

void *NuMemAlloc(i32 size) {
    void *block;
    isize end;
    if (memexternal != NULL) {
        memexternal->cursor->addr = (memexternal->cursor->addr + 15) & ~(usize)15;
        if (memexternal->end.addr - memexternal->cursor->addr >= (usize)size) {
            block = memexternal->cursor->void_ptr;
            memexternal->cursor->addr += size;
            return block;
        }
        return NULL;
    }
    if (discardbuff != NULL) {
        size = (size + 15) & ~15;
        if (discardbuff->remaining > size) {
            block = discardbuff->cursor;
            discardbuff->cursor += size;
            discardbuff->remaining -= size;
            return block;
        }
        return NULL;
    }
    totalloc += size;
    if (NuMem_Heap != NULL) {
        block = NuHeapAllocAligned(NuMem_Heap, size, 4);
    } else {
        block = malloc(size);
    }
    memset(block, 0, size);
    end = (isize)block + size;
    if (end > highallocaddr)
        highallocaddr = end;
    if (end > peakallocaddr)
        peakallocaddr = end;
    return block;
}

void NuMemSetHeap(void *heap) {
    NuMem_Heap = heap;
}

void NuMemFree(void *ptr) {
    if (NuMem_Heap != NULL) {
        NuHeapFree(NuMem_Heap, ptr);
    } else {
        free(ptr);
    }
}

isize NuMemGetPeakAllocAddr(void) {
    return peakallocaddr;
}

NUMEMDISCARDABLE *NuMemSetDiscardable(NUMEMDISCARDABLE *buffer) {
    NUMEMDISCARDABLE *previous = discardbuff;
    discardbuff = buffer;
    return previous;
}

void NuMemFlushDiscardable(NUMEMDISCARDABLE *buffer) {
    if (buffer != NULL) {
        NUMEMDISCARDABLE *pool = buffer;
        pool->cursor = (u8 *)(pool + 1);
        pool->remaining = pool->capacity;
    }
}

NUMEMDISCARDABLE *NuMemCreateDiscardable(i32 size) {
    NUMEMDISCARDABLE *previous = discardbuff;
    discardbuff = NULL;
    NUMEMDISCARDABLE *pool =
        (NUMEMDISCARDABLE *)NuMemoryGet()->GetThreadMem()->_BlockAlloc(size + sizeof(NUMEMDISCARDABLE), 4, 1, "", 0);
    if (pool != NULL) {
        pool->capacity = size;
        pool->remaining = size;
        pool->cursor = (u8 *)(pool + 1);
    }
    discardbuff = previous;
    return pool;
}

void NuMemDestroyDiscardable(NUMEMDISCARDABLE *buffer) {
    NU_FREE(buffer);
}

void NuMemSetExternal(VARIPTR *cursor, VARIPTR *end) {
    if (cursor != NULL) {
        memexternal = &memext;
        memexternal->cursor = cursor;
        if (end != NULL) {
            memexternal->end = *end;
        }
    } else {
        memexternal = NULL;
    }
}

NUMEMEXTERNAL *NuMemGetExternal(void) {
    return memexternal;
}

void memmove(void *dest, const void *source, i32 size) {
    u8 *out = (u8 *)dest;
    const u8 *in = (const u8 *)source;
    if (out < in) {
        while (size != 0) {
            size--;
            *out = *in;
            out++;
            in++;
        }
    } else {
        out += size;
        in += size;
        while (size != 0) {
            size--;
            out--;
            in--;
            *out = *in;
        }
    }
}

void NuMemCopy128(void *dest, const void *source, i32 count) {
    memmove(dest, source, count << 4);
}

void NuMemBlkCheckFreeList(NUMEMBLK *pool) {
    NUMEMBLKLINK *link = pool->free_list;
    i32 count = 0;
    while (link != NULL) {
        link = link->next;
        count++;
    }
}

NUMEMBLK *NuMemBlkCreate(u32 element_size, i32 count, u32 alignment_mask) {
    NUMEMBLKLINK *link;
    i32 i;
    u32 header_size;
    u32 stride;
    NUMEMBLK *pool;
    usize data;
    u32 stride_words;

    if (element_size < sizeof(NUMEMBLKLINK)) {
        element_size = sizeof(NUMEMBLKLINK);
    }
    header_size = (sizeof(NUMEMBLK) + alignment_mask) & ~alignment_mask;
    stride = (element_size + alignment_mask) & ~alignment_mask;
    pool = (NUMEMBLK *)NuMemoryGet()->GetThreadMem()->_BlockAlloc(header_size + stride * count, 4, 1, "", 0);
    data = (usize)pool;
    data = (usize)((NUMEMBLK *)data + 1);
    data += alignment_mask;
    data &= ~(usize)alignment_mask;
    pool->free_list = (NUMEMBLKLINK *)data;
    pool->stride = stride;
    pool->capacity = count;
    pool->free_count = count;
    pool->flags = 0;
    stride_words = stride / sizeof(u32);
    link = pool->free_list;
    for (i = 0; i < count - 1; i++) {
        link->next = (NUMEMBLKLINK *)((u32 *)link + stride_words);
        link = link->next;
    }
    link->next = NULL;
    return pool;
}

NUMEMBLK *NuMemBlkCreateVari(u32 element_size, i32 count, u32 alignment_mask, VARIPTR *buffer) {
    i32 size = NuMemBlkSize(element_size, count, alignment_mask);
    buffer->addr += alignment_mask;
    buffer->addr &= ~(usize)alignment_mask;
    NUMEMBLK *pool = NuMemBlkCreateEx(element_size, count, alignment_mask, buffer->void_ptr);
    buffer->addr += size;
    return pool;
}

NUMEMBLK *NuMemBlkCreateEx(u32 element_size, i32 count, u32 alignment_mask, void *storage) {
    NUMEMBLKLINK *link;
    i32 i;
    u32 stride;
    NUMEMBLK *pool;
    u8 *data;
    u32 stride_words;

    if (element_size < sizeof(NUMEMBLKLINK)) {
        element_size = sizeof(NUMEMBLKLINK);
    }
    stride = (element_size + alignment_mask) & ~alignment_mask;
    pool = (NUMEMBLK *)storage;
    data = (u8 *)pool;
    data += (sizeof(NUMEMBLK) + alignment_mask) & ~alignment_mask;
    pool->stride = stride;
    pool->capacity = count;
    pool->free_count = count;
    pool->flags = NUMEMBLK_EXTERNAL_STORAGE;
    if (count != 0) {
        pool->free_list = (NUMEMBLKLINK *)data;
        stride_words = stride / sizeof(u32);
        link = pool->free_list;
        for (i = 0; i < count - 1; i++) {
            link->next = (NUMEMBLKLINK *)((u32 *)link + stride_words);
            link = link->next;
        }
        link->next = NULL;
    } else {
        pool->free_list = NULL;
    }
    return pool;
}

void *NuMemBlkAlloc(NUMEMBLK *pool) {
    NUMEMBLKLINK *block = pool->free_list;
    if (block != NULL) {
        pool->free_list = pool->free_list->next;
        pool->free_count--;
        memset(block, -1, pool->stride);
    }
    return block;
}

void NuMemBlkFree(NUMEMBLK *pool, void *block) {
    NUMEMBLKLINK *link = (NUMEMBLKLINK *)block;
    pool->free_count++;
    memset(block, -2, pool->stride);
    link->next = pool->free_list;
    pool->free_list = link;
}

i32 NuMemBlkSize(i32 element_size, i32 count, i32 alignment_mask) {
    i32 header_size = (16 + alignment_mask) & ~alignment_mask;
    i32 stride = (element_size + alignment_mask) & ~alignment_mask;
    i32 size = header_size + stride * count;
    return size;
}

void NuMemBlkDestroy(NUMEMBLK *pool) {
    if (!(pool->flags & NUMEMBLK_EXTERNAL_STORAGE)) {
        NU_FREE(pool);
    }
}
