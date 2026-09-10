#include "nu2api/nucore/hashedkey.hpp"

// The original exports Set as a weak out-of-line function (0x446b20, 94 bytes).
__attribute__((weak)) void HashedKey::Set(const char *name) {
    if (name == NULL) {
        value = 0;
        return;
    }
    u32 hash = 0x811c9dc5;
    for (; *name != '\0'; ++name) {
        u32 character = static_cast<i8>(*name);
        const u32 multiplied_hash = hash * 0x1000193;
        hash = (character - 'a' < 26 ? character - ('a' - 'A') : character) ^ multiplied_hash;
    }
    value = hash;
}
