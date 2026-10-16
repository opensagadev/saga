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

// CreateThingManager @0x4e8b50: allocate a 0x24-byte GameThingManager from the
// MemoryManager pool (zeroed) and construct it with room for 4 things. Both the
// ctor and this function store the object in theGameThings.
void CreateThingManager() {
    void *obj = theMemoryManager.AllocPool(sizeof(GameThingManager), 1);
    theGameThings = new (obj) GameThingManager(4);
}
