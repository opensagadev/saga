#include "decomp.h"
#include "legoapi/legoapi_types.h"

extern "C" {

    void StreamCache(void) {
    }

    i32 StreamCacheCheckComplete(void) {
        return 0;
    }

    i32 StreamRead(void) {
        return 0;
    }

    void StreamSeek(void) {
    }

} // extern "C"
