#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" {

    i32 DEVCDDVDROM_Interrogate(NUFILE_DEVICE *device) {
        device->status = 1;
        return 1;
    }

    i32 DEVMEMORYCARD_Interrogate(NUFILE_DEVICE *device) {
        device->status = 1;
        return 1;
    }

    void DebugLog(void) {
    }

    void Debug_Print(void) {
    }

} // extern "C"
