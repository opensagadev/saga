#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nutex.h"

#include <new>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

i32 FindMtlInHGObj(nugscn_s *scene, i32 material_type) {
    for (i32 index = 0; index < scene->nummtl; ++index) {
        if (scene->mtls[index]->unknown_9a[0] == material_type) {
            return index + 1;
        }
    }
    return 0;
}

// CreateThingManager @0x4e8b50: allocate a 0x24-byte GameThingManager from the
// MemoryManager pool (zeroed) and construct it with room for 4 things. The
// ctor stores the object in theGameThings.
void CreateThingManager() {
    void *obj = theMemoryManager.AllocPool(sizeof(GameThingManager), 1);
    new (obj) GameThingManager(4);
}
