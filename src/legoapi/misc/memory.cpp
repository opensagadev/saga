#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/android/nuptl_android.h"

#include <string.h>

// MemoryManager::AllocPool @0x425740.
//
// The manager is initialized by LoadPermData. Its pointer-bearing fields use
// native pointer width while retaining the original layout in the 32-bit build.
// Pools are 32-byte classes; a freed block is pushed on its class list and
// recycled by the pop path below (memset of the requested size only), while a
// fresh carve rounds up to the full 32-byte block and updates the counters.
void *MemoryManager::AllocPool(u32 size, i32 zero) {
    const i32 adjusted_size =
        static_cast<i32>(size - 1) < 0 ? static_cast<i32>(size + 0x1e) : static_cast<i32>(size - 1);
    const i32 size_class = adjusted_size >> 5;

    void **free_head = &free_lists[size_class];
    void *item = *free_head;
    if (item != NULL) {
        *free_head = *reinterpret_cast<void **>(item);
        if (zero != 0) {
            memset(item, 0, size);
        }
        return item;
    }

    const usize block = static_cast<usize>(size_class + 1) << 5;
    if (*end_cell - *cursor_cell <= block) {
        return NULL;
    }
    const usize aligned = ALIGN(*cursor_cell, 0x10);
    *cursor_cell = aligned;
    item = reinterpret_cast<void *>(aligned);
    *cursor_cell = aligned + block;
    memset(item, 0, block);
    allocated += block;
    remaining -= block;
    high_water = *cursor_cell;
    return item;
}

// MemoryManager::FreePool @0x425810: pushes the block onto its size class's
// free list (the next AllocPool of that class pops it, first word = next).
void MemoryManager::FreePool(void *item, u32 size) {
    const i32 adjusted_size =
        static_cast<i32>(size - 1) < 0 ? static_cast<i32>(size + 0x1e) : static_cast<i32>(size - 1);
    const i32 size_class = adjusted_size >> 5;

    void **free_head = &free_lists[size_class];
    *reinterpret_cast<void **>(item) = *free_head;
    *free_head = item;
}

void DebFreeChunksInstantly(i32 *);
void RemoveDebrisEffectFromStack(debkeydatatype_s *);
void RemoveAnyChunkControls(i32 *);
void AddChunkControlToStack(debris_chunk_control_s *, debris_chunk_control_s **);
void DebrisGetControlStackLock();
void DebrisReleaseControlStackLock();
void RemoveChunkFromRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);

extern "C" {

    extern debkeydatatype_s *debkeydata;
    extern i16 *freedebkeys;
    extern i32 freedebkeyptr;
    extern i32 maxdebkeys;
    extern debinftype **debtab;
    extern i32 debrischunks;
    extern i32 debrischunksglass;
    extern i32 freedebchkptr;
    extern i32 freedebchkptrg;
    extern dma_particle_chunk_s **freedebchunks;
    extern dma_particle_chunk_s **freedebchunksglass;
    extern particlechunkrendertype_s *ParticleChunkToRender;
    extern particlechunkrendertype_s *ParticleChunkRenderStack[5];

i32 DebAlloc(void);

    extern debris_chunk_control_s **freechunkcontrols;
    extern i32 freechunkcontrolsptr;
    extern debris_chunk_control_s *debris_chunk_control_stack[2];
    extern f32 globaltime, panelglobaltime;

void DebFree(i32 *handle);

void DebFreeAllChunksInstantly(void);

void DebFreeInstantly(i32 *handle);

void DebFreeOrphansInstantly(debinftype *effect);

    void DebReAlloc2(debkeydatatype_s *key);

void DebReAlloc(debkeydatatype_s *key, i32 particle_count);

    extern "C++" void AddChunkToRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);

void DebReAlloc2(debkeydatatype_s *key);

} // extern "C"
