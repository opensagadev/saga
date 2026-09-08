#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/gizmo/object/takeoverobjects.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/world/level.h"
#include "globals.h"
#include "nu2api/nucore/nustring.h"
#include <string.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void ReleaseTakeOver(GameObject_s *, i32) {
}

void SuperCounters_Reset(i32 area_index) {
    if (area_index != -1) {
        AREADATA *area = &ADataList[area_index];
        SUPERCOUNTER *super_counters = area->super_counters;
        if (super_counters != NULL && area->super_counter_count != 0) {
            for (i32 i = 0; i < area->super_counter_count; ++i) {
                super_counters[i].reset_value = 0;
            }
        }
    }
}

void UpdatePickupFlicker() {
}

TAKEOVEROBJECT_s takeoverobjects[8];
i32 num_takeoverobjects;

void ClearTakeOverObjectSys() {
    memset(takeoverobjects, 0, sizeof(takeoverobjects));
    num_takeoverobjects = 0;
}

void RegisterTakeOverObject(GameObject_s *object) {
    if ((object->apiobj.character_data->game_character->flags_090 & 0x80) != 0 ||
        (object->apiobj.field_0x1f4 & 0x4000) == 0 ||
        (WORLD->current_level->flags & LEVEL_FORGET_TAKEOVERS) != 0) {
        return;
    }
    u8 level = static_cast<u8>(WORLD->current_level->area_level_index);
    if (num_takeoverobjects > 7 || (object->apiobj.field_0x1f4 & 0x400) == 0) {
        return;
    }
    i32 index;
    for (index = 0; index < num_takeoverobjects; ++index) {
        if (takeoverobjects[index].object == object) {
            return;
        }
    }
    takeoverobjects[index].object = object;
    AISCRIPT *script = object->ai.script_process.script;
    if (script != NULL && script->name != NULL) {
        NuStrNCpy(takeoverobjects[index].script_name, script->name, 0x10);
    }
    takeoverobjects[index].character_id = object->id;
    takeoverobjects[index].registered_level = level;
    takeoverobjects[index].current_level = level;
    takeoverobjects[index].source_creature = object->ai.field_0x134;
    ++num_takeoverobjects;
}

void SuperCounters_FindPickup(WORLDINFO_s *, GIZMO_s *, nuvec_s *, SUPERCOUNTERPICKUP **) {
}

void SuperCounter_AnyCollected(SUPERCOUNTER *, WORLDINFO_s *) {
}

void SuperCounters_FixUpGizmos(WORLDINFO_s *) {
}

void SuperCounters_ResetProcessed(WORLDINFO_s *world) {
    if (world->area != NULL && world->area->super_counters != NULL && world->area->super_counter_count != 0) {
        for (i32 i = 0; i < world->area->super_counter_count; ++i) {
            world->area->super_counters[i].processed_flags &= ~2;
        }
    }
}

void SuperCounter_ActivateGizmoPickup(GIZMO_s *, GIZMOPICKUP_s *) {
}

void SuperCounter_FindFromNameAndLevel(char *, WORLDINFO_s *, SUPERCOUNTERPICKUP **) {
}
