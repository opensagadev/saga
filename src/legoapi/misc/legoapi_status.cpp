#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

#if defined(__GNUC__) && !defined(__clang__) && defined(__i386__)
#define STATUS_OMIT_FRAME_POINTER __attribute__((optimize("omit-frame-pointer")))
#else
#define STATUS_OMIT_FRAME_POINTER
#endif

extern "C" {

    STATUS_OMIT_FRAME_POINTER i32 DEVCDDVDROM_Interrogate(NUFILE_DEVICE *device) {
        device->status = 1;
        return 1;
    }

    STATUS_OMIT_FRAME_POINTER i32 DEVMEMORYCARD_Interrogate(NUFILE_DEVICE *device) {
        device->status = 1;
        return 1;
    }

    void DebugLog(void) {
    }

    void Debug_Print(void) {
    }

} // extern "C"
