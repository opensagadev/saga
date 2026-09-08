#include "legoapi/legoapi_types.h"

#include "legoapi/gizmos/object/lever.h"
#include "legoapi/world/world.h"
#include "nu2api/numath/nuvec.h"

extern "C" {
    i16 NewPlatPickupInst(void *object, i32 object_type);
    void PlatInstRotate(i32 platform_id, i32 enabled);
}

void LEVER_s::ClearMechObjectInterface() {
}

void LEVER_s::GetMechObjectInterface() {
}
