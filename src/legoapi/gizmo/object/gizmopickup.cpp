#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"
#include "legoapi/world/level.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nutex.h"
#include "globals.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include <string.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void AICreatureResumeScript(GameObject_s *);
void GetTakeOverPos(GameObject_s *, NUVEC *);
void GameObjectOrigin(GameObject_s *);
i32 TagCode(GameObject_s *, GameObject_s *, i32, i32, i32);
void Buck_StartRiderJump(GameObject_s *, GameObject_s *);
void Buck_Start(GameObject_s *, GameObject_s *);
f32 SpeederChaseATATInOutMul(NUVEC *, NUVEC *);
void GameCam_Blend(GAMECAMERA_s *, f32, f32, i32);
extern i16 id_ATAT;
static NUVEC SpeederChaseATATExitLandPos = {-159.0f, 6.869999885559082f, -17.600000381469727f};

void ReleaseTakeOver(GameObject_s *object, i32) {
    GameObject_s *rider = object->field_0xcc0;
    if (rider == NULL)
        return;
    if (object->character_context == 0x3b) {
        GameObject_s *vehicle = rider;
        rider = object;
        object = vehicle;
    }
    struct ScriptSnapshot {
        AISCRIPTPROCESS process;
        void *field_c8;
    } rider_script, object_script;
    DECOMP_ASSERT(sizeof(ScriptSnapshot) == 0xcc, "Takeover script snapshot ABI");
    memcpy(&rider_script.process, rider->ai.script_process, sizeof(rider_script.process));
    rider_script.field_c8 = rider->ai.field_0xc8;
    u8 rider_set = rider->ai.creature_set;
    memcpy(&object_script.process, object->ai.script_process, sizeof(object_script.process));
    object_script.field_c8 = object->ai.field_0xc8;
    u8 object_set = object->ai.creature_set;
    u16 rider_flag = rider->apiobj.field_0x1f8 & 0x100;
    u16 object_flag = object->apiobj.field_0x1f8 & 0x100;
    if ((rider->field_0xf00 & 2) == 0 && TagCode(rider, object, 1, 0, 0) == 0)
        return;
    memcpy(rider->ai.script_process, &object_script.process, sizeof(object_script.process));
    rider->ai.field_0xc8 = object_script.field_c8;
    memcpy(object->ai.script_process, &rider_script.process, sizeof(rider_script.process));
    object->ai.field_0xc8 = rider_script.field_c8;
    rider->ai.creature_set = rider_set;
    object->ai.creature_set = object_set;
    AICreatureResumeScript(object);
    rider->apiobj.field_0x1f8 = (rider->apiobj.field_0x1f8 & ~0x100) | object_flag;
    object->apiobj.field_0x1f8 = (object->apiobj.field_0x1f8 & ~0x100) | rider_flag;
    if (rider->field_0xcc0 != NULL) {
        GetTakeOverPos(rider->field_0xcc0, &rider->apiobj.position);
        rider->apiobj.pitch_angle = 0;
        rider->apiobj.roll_angle = 0;
        rider->apiobj.facing_angle = rider->field_0xcc0->apiobj.facing_angle;
        rider->apiobj.field_0x276 = rider->apiobj.facing_angle;
        rider->apiobj.movement_facing_angle = rider->field_0xcc0->apiobj.facing_angle;
    }
    GameObjectOrigin(rider);
    rider->character_context = -1;
    rider->field_0xcc0 = NULL;
    if (object->id == id_ATAT && WORLD->current_level == SPEEDERCHASEA_LDATA) {
        f32 multiplier = SpeederChaseATATInOutMul(&rider->apiobj.position, &SpeederChaseATATExitLandPos);
        StartBigJump(rider, &SpeederChaseATATExitLandPos, 0, multiplier * 4.0f, 2.5f * multiplier, 0, 0);
        GameCam_Blend(GameCam, 2.0f, 0.0f, 1);
    } else {
        Buck_StartRiderJump(rider, object);
    }
    GAMECHARACTERDATA *character = object->apiobj.character_data->game_character;
    if (character->field_0x28 > 0.0f) {
        object->apiobj.velocity.y -= 0.5f * rider->apiobj.velocity.y;
    } else if ((character->flags_094[2] & 2) != 0) {
        Buck_Start(object, rider);
    }
    object->field_0xcc0 = NULL;
    rider->field_0xe23 &= 0x7f;
    object->field_0xe23 &= 0x7f;
    rider->ai.path_info.flags &= ~1;
    object->saved_position = object->apiobj.position;
    if (rider->apiobj.field_0x27c != -1 || object->apiobj.field_0x27c != -1) {
        GameCam_Blend(GameCam, 0.3f, 0.0f, 1);
    }
    AISCRIPTPROCESS *rider_process = reinterpret_cast<AISCRIPTPROCESS *>(&rider->ai);
    AISCRIPTPROCESS *object_process = reinterpret_cast<AISCRIPTPROCESS *>(&object->ai);
    if (AIScriptSetBaseScriptStateByName(rider_process, const_cast<char *>("ReleasedTakeOver")) != 0) {
        AIScriptProcess(WORLD->ai_sys, &rider->apiobj, &rider->ai, rider_process, FRAMETIME);
    }
    if (AIScriptSetBaseScriptStateByName(object_process, const_cast<char *>("ReleasedTakeOver")) != 0) {
        AIScriptProcess(WORLD->ai_sys, &object->apiobj, &object->ai, object_process, FRAMETIME);
    }
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

struct TAKEOVER_OBJECT_RECORD {
    GameObject_s *object;
    u8 saved_state[0x10];
    char script_name[0x10];
    i16 model;
    u8 creature_index;
    u8 original_level;
    u8 current_level;
    u8 reserved_29[3];
};
DECOMP_ASSERT(sizeof(TAKEOVER_OBJECT_RECORD) == 0x2c, "Takeover record size");
DECOMP_ASSERT(offsetof(TAKEOVER_OBJECT_RECORD, script_name) == 0x14, "Takeover script offset");
DECOMP_ASSERT(offsetof(TAKEOVER_OBJECT_RECORD, model) == 0x24, "Takeover model offset");
TAKEOVER_OBJECT_RECORD takeoverobjects[8];
i32 num_takeoverobjects;

void RegisterTakeOverObject(GameObject_s *object) {
    if ((object->apiobj.character_data->game_character->flags_090 & 0x80) != 0 ||
        (object->apiobj.field_0x1f4 & 0x4000) == 0 || (WORLD->current_level->flags & LEVEL_FORGET_TAKEOVERS) != 0) {
        return;
    }
    const u8 level = static_cast<u8>(WORLD->current_level->area_level_index);
    if (num_takeoverobjects > 7 || (object->apiobj.field_0x1f4 & 0x400) == 0) {
        return;
    }
    i32 index = 0;
    while (index < num_takeoverobjects) {
        if (takeoverobjects[index].object == object)
            return;
        ++index;
    }
    TAKEOVER_OBJECT_RECORD &record = takeoverobjects[index];
    record.object = object;
    AISCRIPTPROCESS *process = reinterpret_cast<AISCRIPTPROCESS *>(object->ai.script_process);
    if (process->script != NULL && process->script->name != NULL) {
        NuStrNCpy(record.script_name, process->script->name, 16);
    }
    record.model = object->id;
    record.creature_index = object->ai.field_0x134;
    record.original_level = level;
    record.current_level = level;
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
