#include "host/platform/windows/posix_random.h"

#include <stdint.h>

namespace {
    uint64_t random_state = UINT64_C(0x1234abcd330e);
}

// These signatures implement the POSIX API using Windows' 32-bit long.
extern "C" long lrand48(void) { // NOLINT(google-runtime-int)
    random_state = (random_state * UINT64_C(0x5deece66d) + 0xb) & UINT64_C(0xffffffffffff);
    return static_cast<int32_t>(random_state >> 17);
}

extern "C" void srand48(long seed) { // NOLINT(google-runtime-int)
    random_state = (static_cast<uint64_t>(static_cast<uint32_t>(seed)) << 16) | 0x330e;
}
