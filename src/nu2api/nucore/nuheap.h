#pragma once

#include "nu2api/nucore/common.h"

#ifdef __cplusplus
extern "C" {
#endif
    void *NuHeapCreate(VARIPTR *buffer, void *buffer_end, u32 size);
    void NuHeapFree(void *heap, void *ptr);
    void NuHeapDestroy(void *heap);
    void *NuHeapDefragAllocation(void *heap, void *ptr);
    void *NuHeapAlloc(void *heap, u32 size);
    void *NuHeapAllocNamed(void *heap, u32 size, char *name);
    void *NuHeapAllocAligned(void *heap, u32 size, u32 alignment);
    void *NuHeapAllocAlignedNamed(void *heap, u32 size, u32 alignment, char *name);
    u32 NuHeapGetAllocatedBlockCount(void *heap);
    u32 NuHeapGetFreeBlockCount(void *heap);
    u32 NuHeapGetTotalAllocated(void *heap);
    u32 NuHeapGetTotalFree(void *heap);
#ifdef __cplusplus
}
#endif
