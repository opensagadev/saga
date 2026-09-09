#include "gamelib/util/gamelib_util_types.h"
#include "decomp.h"
#include <cstddef>

DECOMP_ASSERT(sizeof(VirtualStackAllocator) == 0x10, "VirtualStackAllocator size");
DECOMP_ASSERT(offsetof(VirtualStackAllocator, cursor) == 4, "Allocator cursor offset");
DECOMP_ASSERT(offsetof(VirtualStackAllocator, end) == 8, "Allocator end offset");
DECOMP_ASSERT(offsetof(VirtualStackAllocator, base) == 12, "Allocator base offset");

VirtualStackAllocator::VirtualStackAllocator() {
    owns_memory = 0;
    cursor = nullptr;
    base = nullptr;
    end = nullptr;
}

VirtualStackAllocator::VirtualStackAllocator(VirtualStackAllocator &parent, u32 size) {
    cursor = nullptr;
    base = nullptr;
    end = nullptr;
    owns_memory = 0;
    u8 *memory = parent.cursor;
    parent.cursor += size;
    setExternalMemoryPool(memory, size);
}

VirtualStackAllocator::VirtualStackAllocator(i32 size) {
    // The original leaves this flag clear even for this allocation.
    owns_memory = 0;
    cursor = new u8[size];
    base = cursor;
    end = cursor + size;
}

VirtualStackAllocator::VirtualStackAllocator(void *memory, u32 size) {
    owns_memory = 0;
    cursor = nullptr;
    base = nullptr;
    end = nullptr;
    setExternalMemoryPool(memory, size);
}

void VirtualStackAllocator::setExternalMemoryPool(void *memory, u32 size) {
    if (owns_memory != 0 && base != nullptr) {
        delete[] base;
    }
    cursor = static_cast<u8 *>(memory);
    base = cursor;
    owns_memory = 0;
    end = cursor + size;
}

VirtualStackAllocator::~VirtualStackAllocator() {
    if (owns_memory != 0 && base != nullptr) {
        delete[] base;
    }
}
