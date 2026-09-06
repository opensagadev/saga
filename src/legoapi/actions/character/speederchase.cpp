#include "decomp.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/world/world.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nutex.h"
#include <string.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

static i32 PodRaceKey[8] __attribute__((aligned(16))) = {-1, -1, -1, -1, -1, -1, -1, -1};

u8 troopercannons_beenReset = 0;
i32 players_going_forward = 0;

MechObjectInterface *forceNextAttackOpponent;
MechObjectInterface *nextShootTarget;
i32 objopponent_ignoreaiopponent;
i32 test_ai_combo;
i32 CanPunchGirls;
extern i16 id_GAMORREANGUARD;
i32 ComboOpponent_Behind;
f32 ComboOpponent_Range2;
f32 PlayerOpponent_Range2;
i16 LEGOACT_PUNCH_BEHIND = -1;
i32 SpecialMove_Check(GameObject_s *, GameObject_s *);
u32 SpecialMove_GetFlags(i32, u32);

GameObject_s *ObjOpponent(GameObject_s *object, f32 range, f32 extra_radius, i32 allow_untargeted, i32 mode,
                          i32 player_filter) {
    i32 ignore_ai = objopponent_ignoreaiopponent;
    if (forceNextAttackOpponent != NULL && (object->apiobj.flags_low & 0x80) != 0) {
        return forceNextAttackOpponent->GetCharacterObject();
    }
    objopponent_ignoreaiopponent = 0;
    GAMEPAD_s *pad = object->pad_gamepad;
    u16 heading = pad->input_magnitude > 0.0f ? GamePad_InputAngle(object, pad) : object->apiobj.movement_facing_angle;
    NUVEC forward;
    NuVecRotateY(&forward, &v001, heading);
    ComboOpponent_Behind = 0;
    GameObject_s *nearest = NULL;
    f32 nearest_distance = 1.0e8f;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
        GameObject_s *target = &Obj[i];
        if ((object->apiobj.flags_low & 0x80) != 0) {
            if (nextShootTarget != NULL && nextShootTarget->GetCharacterObject() != target)
                continue;
        } else if (test_ai_combo != 0 && object->ai.action_target_ref != NULL &&
                   *object->ai.action_target_ref != NULL) {
            target = *object->ai.action_target_ref;
            i = HIGHGAMEOBJECT;
        }
        if (target == object || (target->apiobj.field_0x1f8 & 0x1001) != 0x1001 || target->apiobj.field_0x287 != 0 ||
            target->apiobj.collision_min.y > object->apiobj.collision_max.y)
            continue;
        if ((player_filter == 1 && target->apiobj.field_0x27c != -1) ||
            (player_filter == 2 && target->apiobj.field_0x27c == -1))
            continue;
        i8 context = target->character_context;
        GAMECHARACTERDATA *runtime = static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24);
        if ((context == 0x17 && (target->current_hp == 0 || (runtime->flags_090 & 0x40) != 0)) ||
            (CInfo[context].flags & 0x8000) != 0 || (context & 0xfd) == 0x39 || context == 0x3c)
            continue;
        if (context == 0) {
            if (static_cast<u8>(target->action_movement_state - 3) < 2)
                continue;
        } else if (static_cast<u8>(context - 0xd) < 2 || context == 0x35 || context == 0x47 || context == 0x46)
            continue;
        if ((runtime->flags_090 & 0x8000) != 0 || target == object->field_0xcc0)
            continue;
        u32 object_flags = object->apiobj.field_0x1f4;
        u32 target_flags = target->apiobj.field_0x1f4;
        if ((object_flags & 0x10000) == 0 && (target_flags & 0x10000) == 0 && (object->apiobj.flags_low & 0x80) == 0 &&
            ((object_flags ^ target_flags) & 5) == 0)
            continue;
        if (mode != 0) {
            if (static_cast<u8>(context + 0xa1) < 2 ||
                (context == 0x5a && (target->field_0x7a3 == 0 || (object->apiobj.flags_low & 0x80) == 0)) ||
                (context == 0x17 && target->hitpoints == 0))
                continue;
            if (object->apiobj.field_0x27c != -1) {
                if ((object->apiobj.flags_low & 0x80) != 0) {
                    if (target->apiobj.field_0x27c == -1) {
                        if (WORLD->current_level != HUB_LDATA && (target_flags & 0x10005) == 0)
                            continue;
                    } else {
                        if (((object_flags ^ target_flags) & 1) == 0 &&
                            (object->ai.action_target_ref != NULL || target->ai.action_target_ref != NULL))
                            continue;
                        if (CanPunchGirls == 0 && (object->field_0xf01 & 8) == 0 && (target->field_0xf01 & 8) != 0 &&
                            object->id != id_GAMORREANGUARD) {
                            i32 move = SpecialMove_Check(object, target);
                            if (move == -1 || SpecialMove_GetFlags(move, 1) == 0)
                                continue;
                        }
                    }
                } else if (target->apiobj.field_0x27c != -1 ||
                           (WORLD->current_level != HUB_LDATA && (target_flags & 0x10005) == 0))
                    continue;
            }
            if ((target->apiobj.character_data->model_flags & 0x2000) != 0 ||
                ((runtime->flags_090 & 0x40) != 0 && (target->apiobj.flags_low & 0x80) == 0))
                continue;
        }
        if (!(allow_untargeted != 0 && (ignore_ai != 0 || object->ai.action_target_ref == NULL || mode != 0)) &&
            (target_flags & 1) == 0)
            continue;
        NUVEC delta;
        f32 distance = NuVecDistSqr(&target->apiobj.position, &object->apiobj.position, &delta);
        if (distance >= range * range)
            continue;
        i32 behind = 0;
        if (forward.x * delta.x + forward.z * delta.z < 0.0f) {
            if (mode != 1)
                continue;
            if (object->combo_stage == 0) {
                if (object->apiobj.character_model->model_data_b[0x94] == NULL)
                    continue;
                behind = 1;
            }
        }
        if (extra_radius > 0.0f) {
            f32 radius = object->apiobj.field_0x1dc + target->apiobj.field_0x1dc + extra_radius;
            if (distance >= radius * radius)
                continue;
        }
        if (distance < nearest_distance) {
            nearest = target;
            nearest_distance = distance;
            ComboOpponent_Behind = behind;
        }
    }
    ComboOpponent_Range2 = nearest_distance;
    return nearest;
}

void PodKeyReset() {
    for (i32 i = 0; i < 8; i++) {
        PodRaceKey[i] = -1;
    }
}

void PodLoseSpeed(GameObject_s *, i32, i32) {
}

void InitBikeParts() {
}

void SpeederBlowupHack(GIZMOBLOWUP_s *, i32) {
}

void FindPodHoverHeight(GameObject_s *) {
}

void GetVehicleSpeedMul(GameObject_s *, float) {
}

void ObjIsTargetSpeeder(GameObject_s *) {
}

void PodSeekSubCutSound() {
}

void SpeederChaseA_Init(WORLDINFO_s *) {
}

void PodSeekMushCutSound() {
}

void ProcessCurrentSpeed(WORLDINFO_s *, speedup_s *) {
}

void SpeederChaseA_Panel(WORLDINFO_s *) {
}

void SpeederChaseA_Reset(WORLDINFO_s *) {
    troopercannons_beenReset = 0;
    InitBikeParts();
    LevAIMessage[0] = CheckGizAIMessage(gizaimessagesys, "SpeedersToKill", NULL);
    LevAIMessage[1] = CheckGizAIMessage(gizaimessagesys, "BeenKilled", NULL);
    LevAIMessage[2] = CheckGizAIMessage(gizaimessagesys, "Stage", NULL);
    LevAIMessage[3] = CheckGizAIMessage(gizaimessagesys, "SpeedersKilled", NULL);
    LevAIMessage[4] = CheckGizAIMessage(gizaimessagesys, "KeepPlayersOnSpeeders", NULL);
    LevAIMessage[5] = CheckGizAIMessage(gizaimessagesys, "SpeederMode", NULL);
    players_going_forward = 1;
}

void SpeedersDroppedBack() {
}

void SpeederChaseA_Update(WORLDINFO_s *) {
}

void KillParts_SpeederBike(ADDPART_s *, i32, i32, GameObject_s *) {
}

void ObjOpponentStillThere(GameObject_s *, GameObject_s *, float) {
}

void PodSeekTuskanCutSound() {
}

void SpeederChaseATATInOutMul(nuvec_s *, nuvec_s *) {
}

f32 GetVehicleAreaRememberSpeed() {
    if (bonusmodearcade == 0) {
        return 0.0f;
    }

    f32 speed = 0.0f;
    f32 player_count = 0.0f;
    if (Player[0] != NULL && (Player[0]->apiobj.field_0x1f8 & 0x80) != 0) {
        speed += Player[0]->field_0xdc8;
        player_count = 1.0f;
    }
    if (Player[1] != NULL && (Player[1]->apiobj.field_0x1f8 & 0x80) != 0) {
        speed += Player[1]->field_0xdc8;
        player_count = 2.0f;
    }
    if (player_count > 1.0f) {
        speed /= player_count;
    }
    if (speed < 0.25f) {
        speed = 0.25f;
    }
    return speed;
}

void SpeederChase_DrawMeleeTargets(i16 *, char *, i32) {
}

void SpeederChase_ObjIsAGroundTroop(GameObject_s *) {
}

extern "C" {

    void cbSetAutoSpeed(void) {
    }

} // extern "C"
