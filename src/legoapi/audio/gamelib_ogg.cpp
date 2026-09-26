#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include <stdlib.h>
#include <string.h>
extern "C" {

    void *OggAllocMem(u32 bytes) {
        void *memory = malloc(bytes);
        memset(memory, 0, bytes);
        return memory;
    }

    void OggFreeMem(void *memory) {
        free(memory);
    }

    void *OggReAllocMem(void *memory, u32 bytes) {
        return realloc(memory, bytes);
    }

} // extern "C"
