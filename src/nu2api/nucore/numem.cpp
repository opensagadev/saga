#include "decomp.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/nuheap.h"
#include "nu2api/nucore/numemory.h"
#include <string.h>
#include <stdlib.h>

// The original Android implementation does not change page protection.
extern "C" i32 NuPhysicalProtect(void) {
    STUBBED();
    return 0;
}

#include "nu2api/nucore/numem_types.h"


void NuMemDumpFn(i32 mode);

void *NuMemValidateFn(void);

void NuMemFlushFn(void);


extern "C" {
extern void * NuMem_Heap;
    extern i32 highallocaddr;
}

void *NuMemAlloc(i32 size);

void NuMemSetHeap(void *heap);

void NuMemFree(void *ptr);

isize NuMemGetPeakAllocAddr(void);

NUMEMDISCARDABLE *NuMemSetDiscardable(NUMEMDISCARDABLE *buffer);

void NuMemFlushDiscardable(NUMEMDISCARDABLE *buffer);

NUMEMDISCARDABLE *NuMemCreateDiscardable(i32 size);

void NuMemDestroyDiscardable(NUMEMDISCARDABLE *buffer);

void NuMemSetExternal(VARIPTR *cursor, VARIPTR *end);

void memmove(void *dest, const void *source, i32 size);

i32 numeminit();

NUMEMEXTERNAL *NuMemGetExternal(void);

void NuMemCopy128(void *dest, const void *source, i32 count);

void NuMemBlkCheckFreeList(NUMEMBLK *pool);

NUMEMBLK *NuMemBlkCreate(i32 element_size, i32 count, u32 alignment_mask);

NUMEMBLK *NuMemBlkCreateVari(i32 element_size, i32 count, u32 alignment_mask, VARIPTR *buffer);

NUMEMBLK *NuMemBlkCreateEx(i32 element_size, i32 count, u32 alignment_mask, void *storage);

void *NuMemBlkAlloc(NUMEMBLK *pool);

void NuMemBlkFree(NUMEMBLK *pool, void *block);

i32 NuMemBlkSize(i32 element_size, i32 count, i32 alignment_mask);

void NuMemBlkDestroy(NUMEMBLK *pool);
