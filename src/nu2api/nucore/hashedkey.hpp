#pragma once

#include "decomp.h"
#include "nu2api/nucore/fixed_width.h"
#include <stddef.h>

struct HashedKey {
    u32 value;

    explicit HashedKey(const char *name = NULL) {
        Set(name);
    }
    void Set(const char *name);

    // Units that define a single key had the hash expanded in their static
    // initialiser instead of calling Set.
    enum InlineHashTag { kInlineHash };

    HashedKey(const char *name, InlineHashTag) : value(Hash(name)) {
    }

    static u32 Hash(const char *name) {
        if (name == NULL) {
            return 0;
        }
        u32 hash = 0x811c9dc5;
        for (; *name != '\0'; ++name) {
            u32 character = static_cast<i8>(*name);
            if (character - 'a' < 26) {
                character -= 'a' - 'A';
            }
            hash = (hash * 0x1000193) ^ character;
        }
        return hash;
    }
};
DECOMP_ASSERT(sizeof(HashedKey) == 4, "HashedKey ABI");
