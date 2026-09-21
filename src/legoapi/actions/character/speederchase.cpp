#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/world/levels/levels.h"
#include "legogame/game.h"
#include "decomp.h"
#include "legoapi/actions/character/speederchase.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/items/collect/spacelevel.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edui.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nu3d/nutex.h"
#include <string.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.
static i32 PodRaceKey[8] __attribute__((aligned(16))) = {-1, -1, -1, -1, -1, -1, -1, -1};
static i32 snaphack[2];
static i32 snaphacktimer[2];

struct SPEEDERCHASEANETPACKET_s {
    u8 field_0x0;
    u8 speeder_count;
    u8 speeders_killed;
    u8 field_0x3;
    u8 panel_state;
    u8 field_0x5;
    u8 field_0x6;
};

SPEEDERCHASEANETPACKET_s *speederchasea_netpacket;

u8 troopercannons_beenReset = 0;
i32 players_going_forward = 0;
extern i32 players_cannot_exit_speeder;

NuMechPtr<MechObjectInterface, 4> lungeTarget;
NuMechPtr<MechObjectInterface, 4> forceNextAttackOpponent;
NuMechPtr<MechObjectInterface, 4> nextShootTarget;
i32 objopponent_ignoreaiopponent;
i32 test_ai_combo;
i32 CanPunchGirls;
extern i16 id_GAMORREANGUARD;
extern i16 id_STORMTROOPER;
i32 ComboOpponent_Behind;
f32 ComboOpponent_Range2;
f32 PlayerOpponent_Range2;
i32 SpecialMove_Check(GameObject_s *, GameObject_s *);
u32 SpecialMove_GetFlags(i32, u32);

GameObject_s *ObjOpponent(GameObject_s *object, f32 range, f32 extra_radius, i32 allow_untargeted, i32 mode,
                          i32 player_filter) {
    i32 ignore_ai = objopponent_ignoreaiopponent;
    if (forceNextAttackOpponent.Get() != NULL && (object->apiobj.flags_low & 0x80) != 0) {
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
            if (nextShootTarget.Get() != NULL && nextShootTarget->GetCharacterObject() != target)
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

void TakeHitRumble(GameObject_s *, f32);

void PodLoseSpeed(GameObject_s *object, i32 hit, i32 rumble) {
    if (hit != 0) {
        ObjHitObj(NULL, object, 1, 0, 0, 1);
    }
    if (hit != 0 || rumble != 0) {
        TakeHitRumble(object, 0.7f);
    }
    object->current_speed_mul *= 0.5f;
    if (object->current_speed_mul < 0.333f) {
        object->current_speed_mul = 0.333f;
    }
}

i32 SpeederBlowupHack(GIZMOBLOWUP_s *blowup, i32) {
    if (blowup == NULL) {
        return 1;
    }
    return NuStrCmp(blowup->name, "thermocrate_011") != 0 && NuStrCmp(blowup->name, "thermocrate_021") != 0 &&
           NuStrCmp(blowup->name, "thermocrate_031") != 0 && NuStrCmp(blowup->name, "minikit101") != 0;
}

f32 FindPodHoverHeight(GameObject_s *object) {
    static NUVEC podsprintlifthackpos[3] = {
        {-395.26f, -5.48f, -165.08f},
        {-375.38f, -5.04f, -286.5f},
        {-354.91f, -4.85f, -299.32f},
    };

    f32 height = 0.15f;
    if (object == Player[1] && (Player[0]->apiobj.flags_low & 0x80) != 0) {
        f32 distance_squared = NuVecDistSqr(&Player[0]->apiobj.position, &object->apiobj.position, NULL);
        if (distance_squared < 4.0f) {
            f32 distance = NuFsqrt(distance_squared);
            i32 angle = static_cast<i32>((1.0f - distance * 0.5f) * 16384.0f);
            height += NuTrigTable[(angle >> 1) & 0x7fff] * 0.5f;
        }
    }

    if (PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA && (object->apiobj.flags_low & 0x80) != 0) {
        for (i32 i = 0; i < 3; ++i) {
            if (NuVecXZDistSqr(&object->apiobj.position, &podsprintlifthackpos[i], NULL) < 2.0f) {
                height += 1.5f;
                break;
            }
        }
    }
    return height;
}

extern i32 ObjInNarrowSock(GameObject_s *, SOCKSYS *, i32);
i32 IDLESPEEDINNARROWSOCKSONLY = 0;
f32 GetVehicleSpeedMul(GameObject_s *object, f32 speed) {
    f32 effective;
    if (object->character_context == 0x36 || object->character_context == 0x2a || object->character_context == 0x3a)
        effective = ((GAMECHARACTERDATA_s *)object->apiobj.character_data->field11_0x24)->run_speed;
    else if ((object->apiobj.flags_low & 0x80) != 0 && WORLD->current_level == DEATHSTARBATTLED_LDATA &&
             ObjInNarrowSock(object, WORLD->sock_sys, WORLD->level_idx)) {
        GAMECHARACTERDATA_s *data = (GAMECHARACTERDATA_s *)object->apiobj.character_data->field11_0x24;
        f32 fraction = (speed - data->field_0x10) / (data->run_speed - data->field_0x10);
        if (fraction < 0.0f)
            fraction = 0.0f;
        effective = (0.5f + fraction * 0.5f) * data->run_speed;
    } else if ((object->apiobj.flags_low & 0x80) != 0 && (!IDLESPEEDINNARROWSOCKSONLY || object->in_narrow_socket) &&
               (object->field_0xf03 & 2) == 0) {
        f32 idle = ((GAMECHARACTERDATA_s *)object->apiobj.character_data->field11_0x24)->field_0x10;
        effective = idle > speed ? idle : speed;
    } else {
        GAMECHARACTERDATA_s *data = (GAMECHARACTERDATA_s *)object->apiobj.character_data->field11_0x24;
        effective = (speed - data->field_0x10) / (data->run_speed - data->field_0x10) * data->run_speed;
    }
    if (effective < 0.0f)
        effective = 0.0f;
    return effective / ((GAMECHARACTERDATA_s *)object->apiobj.character_data->field11_0x24)->run_speed;
}

i32 ObjIsTargetSpeeder(GameObject_s *object) {
    if (object->id != id_SPEEDERBIKE) {
        return 0;
    }
    if (object->field_0xcc0 == NULL) {
        return 0;
    }
    return object->apiobj.field_0x27c == -1;
}

void SetLevelExBlowupFunc(i32 (*callback)(GIZMOBLOWUP_s *, i32));
void InitTrooperCannons(WORLDINFO_s *world);
void ResetTrooperCannons(WORLDINFO_s *world, i32 trooper_id);
void UpdateTrooperCannons(WORLDINFO_s *world);
i32 GoingForwardsAlongNarrowSock(GameObject_s *object);

void SpeederChaseA_Init(WORLDINFO_s *world) {
    speederchasea_netpacket = static_cast<SPEEDERCHASEANETPACKET_s *>(SetLevelHack(7));
    SetLevelExBlowupFlags(3);
    SetLevelExBlowupFunc(SpeederBlowupHack);
    InitTrooperCannons(world);
    players_cannot_exit_speeder = 0;
    InitBikeParts();

    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "clear1_ffield1", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[1], "clear1_ffield2", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[2], "clear2_ffield1", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[3], "clear2_ffield2", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[4], "clear1_ffield1", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[5], "clear3_ffield1", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[6], "clear3_ffield2", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[7], "clear4_ffield1", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[8], "clear4_ffield2", 1);

    snaphack[1] = 0;
    snaphack[0] = 0;
    snaphacktimer[1] = 0;
    reinterpret_cast<u8 *>(LevSfxFlag)[0] = 0;
    reinterpret_cast<u8 *>(LevSfxFlag)[1] = 0;
    reinterpret_cast<u8 *>(LevSfxFlag)[2] = 0;
    reinterpret_cast<u8 *>(LevSfxFlag)[3] = 0;
    reinterpret_cast<u8 *>(LevSfxFlag)[4] = 0;
    reinterpret_cast<u8 *>(LevSfxFlag)[5] = 0;
    reinterpret_cast<u8 *>(LevSfxFlag)[6] = 0;
    reinterpret_cast<u8 *>(LevSfxFlag)[7] = 0;
    reinterpret_cast<u8 *>(LevSfxFlag)[8] = 0;
    snaphacktimer[0] = 0;

    GIZOBSTACLE_s *obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "RAISEPLAT");
    if (obstacle != NULL) {
        obstacle->field_a1_0xa1 |= 1;
    }

    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "bridge_01_13b1");
    if (blowup != NULL) {
        blowup->field_0x124 = 1;
        blowup->field_0x128 = blowup->target_scale * 1.5f;
    }
    blowup = GizmoBlowUp_FindByName(world, "bridge_01_14b1");
    if (blowup != NULL) {
        blowup->field_0x124 = 1;
        blowup->field_0x128 = blowup->target_scale * 1.5f;
    }
    blowup = GizmoBlowUp_FindByName(world, "tow1_tar1");
    if (blowup != NULL) {
        blowup->field_0x124 = 1;
    }
    blowup = GizmoBlowUp_FindByName(world, "tow2_tar1");
    if (blowup != NULL) {
        blowup->field_0x124 = 1;
    }
    blowup = GizmoBlowUp_FindByName(world, "tow3_tar1");
    if (blowup != NULL) {
        blowup->field_0x124 = 1;
    }
    blowup = GizmoBlowUp_FindByName(world, "tow4_tar1");
    if (blowup != NULL) {
        blowup->field_0x124 = 1;
    }
}

void ProcessCurrentSpeed(WORLDINFO_s *world, speedup_s *speedup) {
    spacelevel_s *space = world->space_level;
    while (speedup->distance != 0.0f) {
        if (speedup->distance > space->door_elapsed && speedup->distance <= space->door_countdown) {
            space->value_one_a = speedup->speed;
        }
        ++speedup;
    }

    f32 target_speed = space->value_one_a;
    if (static_cast<u16>(GameCam->sock_position.next_segment - 120) <= 30) {
        if (static_cast<i8>(LevBlowUp[0]->state_flags) < 0) {
            target_speed *= 0.25f;
        } else if (static_cast<i8>(LevBlowUp[1]->state_flags) < 0) {
            target_speed *= 0.25f;
        } else if (static_cast<i8>(LevBlowUp[2]->state_flags) < 0) {
            target_speed *= 0.25f;
        } else if (static_cast<i8>(LevBlowUp[3]->state_flags) < 0) {
            target_speed *= 0.25f;
        } else if (static_cast<i8>(LevBlowUp[4]->state_flags) < 0) {
            target_speed *= 0.25f;
        }
    }

    if (target_speed > space->value_one_b) {
        space->value_one_b += FRAMETIME * 0.25f;
        if (space->value_one_b > target_speed) {
            space->value_one_b = target_speed;
        }
    } else if (space->value_one_b > target_speed) {
        space->value_one_b -= FRAMETIME * 0.25f;
        if (space->value_one_b < target_speed) {
            space->value_one_b = target_speed;
        }
    }
    f32 current_speed = space->value_one_b;
    f32 vehicle_speed = current_speed * 20.0f;
    world->sock_sys->sock[0].current_speed = vehicle_speed;

    if (world->current_level == DOGFIGHTA_LDATA && vehicle_speed != 0.0f) {
        *reinterpret_cast<f32 *>(space->unknown_62ee4) = vehicle_speed / 11.0f;
    }
}

void SpeederChaseA_Panel(WORLDINFO_s *) {
    i16 character_ids[10];
    char dimmed[10] = {0};

    if (netclient != 0) {
        SPEEDERCHASEANETPACKET_s *packet = speederchasea_netpacket;
        if (packet == NULL || packet->panel_state != 1 || packet->speeder_count == 0) {
            return;
        }

        for (i32 i = 0; i < packet->speeder_count; ++i) {
            character_ids[i] = id_SPEEDERBIKE;
            if (packet->speeder_count - packet->speeders_killed <= i) {
                dimmed[i] = 1;
            }
        }
        SpeederChase_DrawMeleeTargets(character_ids, dimmed, packet->speeder_count);
        return;
    }

    if (LevAIMessage[0] == NULL || LevAIMessage[1] == NULL || LevAIMessage[3] == NULL ||
        LevAIMessage[3]->value != 0.0f || !(0.0f < LevAIMessage[0]->value)) {
        return;
    }

    i32 count = static_cast<i32>(LevAIMessage[0]->value);
    if (count > 10) {
        count = 10;
    }
    i32 killed = static_cast<i32>(LevAIMessage[1]->value);
    for (i32 i = 0; i < count; ++i) {
        character_ids[i] = id_SPEEDERBIKE;
        if (count - killed <= i) {
            dimmed[i] = 1;
        }
    }
    SpeederChase_DrawMeleeTargets(character_ids, dimmed, count);
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

i32 SpeedersDroppedBack() {
    return WORLD->current_level == SPEEDERCHASEA_LDATA && disable_narrow_socks == 0 && set_speedermode == 2;
}

void SpeederChaseA_Update(WORLDINFO_s *world) {
    ResetTrooperCannons(world, id_STORMTROOPER);
    UpdateTrooperCannons(world);

    if (netclient == 0) {
        players_cannot_exit_speeder = LevAIMessage[4] != NULL && LevAIMessage[4]->value == 1.0f;
    }

    if (disable_narrow_socks != 0) {
        if (Player[0] != NULL) {
            Player[0]->field_0xf03 &= ~1;
        }
        if (Player[1] != NULL) {
            Player[1]->field_0xf03 &= ~1;
        }
    } else {
        for (i32 i = 0; i < 2; ++i) {
            GameObject_s *object = Player[i];
            if (object != NULL && WORLD->current_level == SPEEDERCHASEA_LDATA && object->id == id_SPEEDERBIKE &&
                object->field_0xcc0 != NULL && static_cast<i8>(object->field_0xcc0->apiobj.flags_low) < 0) {
                object->field_0xf03 |= 1;
            } else if (object != NULL) {
                object->field_0xf03 &= ~1;
            }
        }
    }

    players_going_forward = 1;
    for (i32 i = 0; i < 2; ++i) {
        GameObject_s *object = Player[i];
        if (object != NULL && static_cast<i8>(object->apiobj.flags_low) < 0 &&
            (object->apiobj.field_0x1f4 & 0x40000) == 0 &&
            (GoingForwardsAlongNarrowSock(object) == 0 || object->field_0x7a5 == 0x2a)) {
            players_going_forward = 0;
        }
    }

    u8 *sound_flags = reinterpret_cast<u8 *>(LevSfxFlag);

#define UPDATE_FORCE_FIELD_SOUND(index)                                                                                \
    do {                                                                                                               \
        nuinstanim_s *animation = NuSpecialGetInstAnim(&LevHSpecial[index]);                                           \
        NUVEC *position = NuSpecialGetDrawPos(&LevHSpecial[index]);                                                    \
        if (animation != NULL) {                                                                                       \
            void *animation_data = WORLD->current_gscn->instance_animation_data[animation->anim_ix];                   \
            if (sound_flags[index] == 0) {                                                                             \
                if (animation->ltime > 0.0f && animation->ltime != NuAnimEndFrameOld(animation_data)) {                \
                    PlaySfx("env_laser_gate_on", position);                                                            \
                    sound_flags[index] = 1;                                                                            \
                }                                                                                                      \
            } else if ((animation->flags & NUINSTANIM_FLAG_PLAYING) != 0 ||                                            \
                       animation->ltime != NuAnimEndFrameOld(animation_data)) {                                        \
                PlaySfx("env_laser_gate_lp", position);                                                                \
            } else {                                                                                                   \
                PlaySfx("env_laser_gate_off", position);                                                               \
                sound_flags[index] = 0;                                                                                \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)

    UPDATE_FORCE_FIELD_SOUND(0);
    UPDATE_FORCE_FIELD_SOUND(1);
    UPDATE_FORCE_FIELD_SOUND(2);
    UPDATE_FORCE_FIELD_SOUND(3);
    UPDATE_FORCE_FIELD_SOUND(4);
    UPDATE_FORCE_FIELD_SOUND(5);
    UPDATE_FORCE_FIELD_SOUND(6);
    UPDATE_FORCE_FIELD_SOUND(7);
    UPDATE_FORCE_FIELD_SOUND(8);

#undef UPDATE_FORCE_FIELD_SOUND
}

// Original 0x4f2e50, 258 bytes. The original returns an integer.
i32 ObjOpponentStillThere(GameObject_s *object, GameObject_s *opponent, f32 gap) {
    i32 result = 0;
    if (object->force_target != NULL) {
        NUVEC forward, difference;
        NuVecRotateY(&forward, &v001, object->apiobj.field_0x276);
        f32 distance = NuVecDistSqr(&opponent->apiobj.position, &object->apiobj.position, &difference);
        f32 dot = forward.x * difference.x + forward.z * difference.z;
        if (static_cast<i8>(object->context_flags) < 0 ? dot <= 0.0f : dot >= 0.0f) {
            f32 radius = object->apiobj.field_0x1dc + opponent->apiobj.field_0x1dc + gap;
            result = distance < radius * radius;
        }
    }
    return result;
}

f32 SpeederChaseATATInOutMul(nuvec_s *start, nuvec_s *end) {
    f32 distance = NuVecXZDist(start, end, NULL) * 0.125f;
    if (distance > 1.0f)
        return 1.0f;
    return MAX(0.1f, distance);
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

i32 SpeederChase_ObjIsAGroundTroop(GameObject_s *object) {
    if (object == NULL) {
        return 1;
    }
    return (object->apiobj.character_data->model_flags & 0x2000) == 0;
}

extern "C" {

    i32 edmain_auto_speed = 1;

    void cbSetAutoSpeed(eduimenu_s *, eduiitem_s *item, u32) {
        edmain_auto_speed = item->highlighted;
        if (edmain_auto_speed != 0) {
            edcamSetAutoSpeed(0.15f, 0.2f, 0.01f, 0.1f);
        } else {
            edcamSetAutoSpeed(0.0f, 0.0f, 0.0f, 0.0f);
        }
    }

} // extern "C"

extern "C" void AddDebrisEffect(i32 *, i32, f32, f32, f32);
extern "C" void DebrisPosOrientationMtx(i32, NUMTX *);
extern "C" void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

void PodDust(WORLDINFO_s *world, GameObject_s *object) {
    if (object->apiobj.field_0x218 == 2000000.0f)
        return;
    if (world->debris_sys->entries[25].effect == -1)
        return;
    NUVEC positions[2];
    NUVEC previous[2];
    i32 count = 0;
    for (i32 locator = 1; locator <= 2; ++locator) {
        if (object->apiobj.field_0x288 == 0 || object->apiobj.character_model->points_of_interest[locator] == NULL)
            continue;
        NUMTX *joint = &object->joint_matrices[locator];
        positions[count].x = joint->m30;
        positions[count].y = object->apiobj.field_0x218 + 0.1f;
        positions[count].z = joint->m32;
        ++count;
        if ((object->apiobj.flags_low & 0x80) == 0)
            continue;
        i32 key;
        if (object == Player[0])
            key = locator - 1;
        else if (object == Player[1])
            key = locator + 3;
        else
            continue;
        NUMTX matrix;
        matrix.m00 = joint->m00;
        matrix.m01 = joint->m01;
        matrix.m02 = joint->m02;
        matrix.m03 = joint->m03;
        matrix.m10 = joint->m20;
        matrix.m11 = joint->m21;
        matrix.m12 = joint->m22;
        matrix.m13 = joint->m23;
        matrix.m20 = -joint->m10;
        matrix.m21 = -joint->m11;
        matrix.m22 = -joint->m12;
        matrix.m23 = -joint->m13;
        matrix.m30 = joint->m30;
        matrix.m31 = joint->m31;
        matrix.m32 = joint->m32;
        matrix.m33 = joint->m33;
        if (PodRaceKey[key] == -1) {
            AddDebrisEffect(&PodRaceKey[key], world->debris_sys->entries[53].effect, 0.0f, 0.0f, 0.0f);
        } else {
            DebrisPosOrientationMtx(PodRaceKey[key], &matrix);
        }
        key += 2;
        if (PodRaceKey[key] == -1) {
            AddDebrisEffect(&PodRaceKey[key], world->debris_sys->entries[54].effect, 0.0f, 0.0f, 0.0f);
        } else {
            u16 angle = object->apiobj.field_0x276;
            f32 sine = NuTrigTable[angle >> 1];
            f32 cosine = NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff];
            matrix.m00 = -cosine;
            matrix.m01 = 0.0f;
            matrix.m02 = sine;
            matrix.m10 = -sine;
            matrix.m11 = 0.0f;
            matrix.m12 = -cosine;
            matrix.m20 = 0.0f;
            matrix.m21 = 1.0f;
            matrix.m22 = 0.0f;
            matrix.m31 = object->apiobj.field_0x218 + 0.1f;
            DebrisPosOrientationMtx(PodRaceKey[key], &matrix);
        }
    }
    if (count == 0) {
        positions[0].x = object->apiobj.collision_position.x;
        positions[0].y = object->apiobj.field_0x218;
        positions[0].z = object->apiobj.collision_position.z;
        count = 1;
    }
    NUVEC delta;
    NuVecSub(&delta, &object->apiobj.start_position, &object->apiobj.position);
    f32 rate = 100.0f * avg_currentspeed_mul;
    rate *= 1.0f - (object->apiobj.collision_min.y - object->apiobj.field_0x218) * 0.5f;
    for (i32 i = 0; i < count; ++i) {
        NuVecAdd(&previous[i], &positions[i], &delta);
        i32 particles = ParticlesPerSecond(rate, FRAMETIME);
        for (i32 j = 0; j < particles; ++j) {
            f32 fraction = qrand() * (1.0f / 65535.0f);
            NUVEC position;
            position.x = previous[i].x + (positions[i].x - previous[i].x) * fraction;
            position.y = previous[i].y + (positions[i].y - previous[i].y) * fraction;
            position.z = previous[i].z + (positions[i].z - previous[i].z) * fraction;
            AddVariableShotDebrisEffect(world->debris_sys->entries[25].effect, &position, 1, 0, 0);
        }
    }
}

extern "C" f32 pod_roll[2];
f32 getPodRoll(i32 index) {
    return pod_roll[index];
}
