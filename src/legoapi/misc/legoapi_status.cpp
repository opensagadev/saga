#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" {

    __attribute__((optimize("O2", "omit-frame-pointer"))) i32 DEVCDDVDROM_Interrogate(NUFILE_DEVICE *device) {
        device->status = 1;
        return 1;
    }

    __attribute__((optimize("O2", "omit-frame-pointer"))) i32 DEVMEMORYCARD_Interrogate(NUFILE_DEVICE *device) {
        device->status = 1;
        return 1;
    }

    __attribute__((optimize("O2", "omit-frame-pointer"))) void DebugLog(void) {
    }

    __attribute__((optimize("O2", "omit-frame-pointer"))) void Debug_Print(void) {
    }

} // extern "C"
