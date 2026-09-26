#include "decomp.h"
#include "legoapi/legoapi_types.h"

extern "C" {

    __attribute__((optimize("O2", "omit-frame-pointer"))) void StreamCache(void) {
    }

    __attribute__((optimize("O2", "omit-frame-pointer"))) i32 StreamCacheCheckComplete(void) {
        return 0;
    }

    __attribute__((optimize("O2", "omit-frame-pointer"))) i32 StreamRead(void) {
        return 0;
    }

    __attribute__((optimize("O2", "omit-frame-pointer"))) void StreamSeek(void) {
    }

} // extern "C"
