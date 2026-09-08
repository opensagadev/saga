#include "legoapi/legoapi_types.h"

#include "legoapi/gizmos/object/lever.h"
#include "legoapi/world/world.h"
#include "nu2api/numath/nuvec.h"

extern "C" {
    i16 NewPlatPickupInst(void *object, i32 object_type);
    void PlatInstRotate(i32 platform_id, i32 enabled);
}

void Lever_MoveCode(WORLDINFO_s *, GameObject_s *) {
}

void Levers_InitTerrain(WORLDINFO_s *world) {
    for (i32 index = 0; index < world->nlevers; ++index) {
        LEVER_s &lever = world->levers[index];
        lever.platform_id = NewPlatPickupInst(&lever, 3);
        PlatInstRotate(lever.platform_id, 1);
    }
}

void LEVER_s::ClearMechObjectInterface() {
}

void LEVER_s::GetMechObjectInterface() {
}
