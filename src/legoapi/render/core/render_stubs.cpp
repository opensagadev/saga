#include "legoapi/world/world_shared.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "decomp.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "globals.h"

#include <string.h>

struct numtl_s;
typedef struct numtl_s NUMTL;

extern "C" {
    extern nurenderscene_s currentScene;
} // extern "C"
