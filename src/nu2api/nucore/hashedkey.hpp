#pragma once

#include "decomp_assert.h"
#include "nu2api/nucore/fixed_width.h"
#include <stddef.h>

struct HashedKey {
    u32 value;

    explicit HashedKey(const char *name = NULL) {
        Set(name);
    }
    void Set(const char *name) {
        if (name == NULL) {
            value = 0;
            return;
        }
        u32 hash = 0x811c9dc5;
        for (; *name != '\0'; ++name) {
            u32 character = static_cast<i8>(*name);
            if (character - 'a' < 26)
                character -= 'a' - 'A';
            hash = character ^ hash * 0x1000193;
        }
        value = hash;
    }
};
DECOMP_ASSERT(sizeof(HashedKey) == 4, "HashedKey ABI");
