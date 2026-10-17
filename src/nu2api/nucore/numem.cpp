#include "decomp.h"
#include "nu2api/nucore/numemory.h"
#include <string.h>
#include <stdlib.h>

// The original Android implementation does not change page protection.
extern "C" i32 NuPhysicalProtect(void) {
    STUBBED();
    return 0;
}

extern "C" {
    extern i32 highallocaddr;
}

void memmove(void *dest, const void *source, i32 size);

