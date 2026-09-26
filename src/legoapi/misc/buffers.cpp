#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern NUMEMFILE memfiles[20];

void StillMemRestore() {
    // No operation in the Android build.
}

void MemFileBoundsCheck(i32 file) {
    i32 remaining = memfiles[file].end - memfiles[file].ptr + 1;
}
