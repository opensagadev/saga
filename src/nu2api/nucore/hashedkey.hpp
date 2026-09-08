#pragma once

#include "decomp_assert.h"
#include "nu2api/nucore/fixed_width.h"
#include <stddef.h>

struct HashedKey {
    u32 value;

    explicit HashedKey(const char *name = NULL) {
        Set(name);
    }
    void Set(const char *name);
};
DECOMP_ASSERT(sizeof(HashedKey) == 4, "HashedKey ABI");
