#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    // Preserve the POSIX API types; Windows long is 32 bits.
    long lrand48(void);      // NOLINT(google-runtime-int)
    void srand48(long seed); // NOLINT(google-runtime-int)

#ifdef __cplusplus
}
#endif
