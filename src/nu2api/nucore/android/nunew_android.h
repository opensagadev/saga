#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
void *operator new(std::size_t size);
void *operator new(std::size_t size, u32 alignment);
void *operator new[](std::size_t size);
void *operator new[](std::size_t size, u32 alignment);
void operator delete(void *ptr) noexcept;
void operator delete[](void *ptr) noexcept;
