// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/world/levels/levels.h"
#include "decomp.h"
#include "legoapi/actions/character/speederchase.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "gameapi/edtools/edui.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nurand.h"
#include "legoapi/items/objects/gameobjects.h"
#include "nu2api/nucore/nuanim3.h"
#include <string.h>
#include "legoapi/actions/character/speederchase_types.h"
#include "legoapi/actions/character/legoapi_tag.h"
#include "legoapi/characters/motion/camera.h"
#include "legoapi/world/levels/episodeV.h"
#include "legoapi/gizmo/object/gizmoblowups.h"

static i32 snaphack[2];
static i32 snaphacktimer[2];

SPEEDERCHASEANETPACKET_s *speederchasea_netpacket;

u8 troopercannons_beenReset = 0;
i32 players_going_forward = 1;

f32 zoom_ahead_extra = 3.0f;
f32 zoom_ahead_extra_time = 5.0f;
f32 speeder_level_height_lerpf = 5.0f;
f32 speeder_level_height = 1.0f;
SPEEDERCHASEVALUES speedervals[4] = {
    {9.0f, -3.0f, 1.5f, 5.0f, 10.0f, 0.5f, 5.0f},
    {-3.0f, -3.0f, 1.5f, 5.0f, 5.0f, 0.3f, 5.0f},
    {-3.0f, -3.0f, 1.5f, 5.0f, 2.0f, 0.3f, 5.0f},
    {4.0f, -3.0f, 1.5f, 5.0f, 10.0f, 0.5f, 5.0f},
};
f32 speederfirerange = -1.0f;
f32 speeder_shootrate = 1.0f;
f32 speeder_kill_dist = 75.0f;
f32 speeder_midrange_time = 10.0f;
f32 speeder_ahead_time = 15.0f;
f32 speeder_mode_ahead_timer;
DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

i32 objopponent_ignoreaiopponent;
i32 test_ai_combo = 1;
i32 CanPunchGirls = 1;
extern i16 id_STORMTROOPER;

f32 getPodRoll(i32 index) {
    return pod_roll[index];
}

i32 SpeederBlowupHack(GIZMOBLOWUP_s *blowup, i32) {
    if (blowup == NULL) {
        return 1;
    }
    return NuStrCmp(blowup->name, "thermocrate_011") != 0 && NuStrCmp(blowup->name, "thermocrate_021") != 0 &&
           NuStrCmp(blowup->name, "thermocrate_031") != 0 && NuStrCmp(blowup->name, "minikit101") != 0;
}

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

void SpeederChaseA_Reset(WORLDINFO_s *) {
    troopercannons_beenReset = 0;
    InitBikeParts();
    LevAIMessage[0] = CheckGizAIMessage(gizaimessagesys, "SpeedersToKill", NULL);
    LevAIMessage[1] = CheckGizAIMessage(gizaimessagesys, "SpeedersKilled", NULL);
    LevAIMessage[2] = CheckGizAIMessage(gizaimessagesys, "Stage", NULL);
    LevAIMessage[3] = CheckGizAIMessage(gizaimessagesys, "Phase", NULL);
    LevAIMessage[4] = CheckGizAIMessage(gizaimessagesys, "KeepPlayersOnSpeeders", NULL);
    LevAIMessage[5] = CheckGizAIMessage(gizaimessagesys, "SpeederMode", NULL);
    players_going_forward = 1;
}

void SpeederChaseA_Update(WORLDINFO_s *world) {
    ResetTrooperCannons(world, id_STORMTROOPER);
    UpdateTrooperCannons(world);

    if (netclient == 0) {
        if (LevAIMessage[4] != NULL && LevAIMessage[4]->value == 1.0f) {
            players_cannot_exit_speeder = 1;
        } else {
            players_cannot_exit_speeder = 0;
        }
    }

    WORLDINFO_s *current_world = WORLD;
    LEVELDATA *chase_level = SPEEDERCHASEA_LDATA;
    const i16 bike_id = id_SPEEDERBIKE;
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
            if (object != NULL && current_world->current_level == chase_level && object->id == bike_id &&
                object->field_0xcc0 != NULL && object->field_0xcc0->apiobj.player_controlled) {
                object->field_0xf03 |= 1;
            } else if (object != NULL) {
                object->field_0xf03 &= ~1;
            }
        }
    }

    players_going_forward = 1;
    for (i32 i = 0; i < 2; ++i) {
        GameObject_s *object = Player[i];
        if (!(object != NULL && static_cast<i8>(object->apiobj.flags_low) < 0 &&
              (object->apiobj.field_0x1f4 & 0x40000) == 0 &&
              (GoingForwardsAlongNarrowSock(object) == 0 || Player[i]->field_0x7a5 == 0x2a))) {
            continue;
        }
        players_going_forward = 0;
        break;
    }

    u8 *sound_flags = reinterpret_cast<u8 *>(LevSfxFlag);

    for (i32 i = 0; i < 9; i++) {
        nuinstanim_s *animation = NuSpecialGetInstAnim(&LevHSpecial[i]);
        NUVEC *position = NuSpecialGetDrawPos(&LevHSpecial[i]);
        if (animation != NULL) {
            void *animation_data = WORLD->current_gscn->instance_animation_data[animation->anim_ix];
            if (sound_flags[i] == 0) {
                if (animation->ltime > 0.0f && animation->ltime != NuAnimEndFrameOld(animation_data)) {
                    PlaySfx("env_laser_gate_on", position);
                    sound_flags[i] = 1;
                }
            } else if ((animation->flags & NUINSTANIM_FLAG_PLAYING) != 0 ||
                       animation->ltime != NuAnimEndFrameOld(animation_data)) {
                PlaySfx("env_laser_gate_lp", position);
            } else {
                PlaySfx("env_laser_gate_off", position);
                sound_flags[i] = 0;
            }
        }
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

    if (LevAIMessage[0] != NULL && LevAIMessage[1] != NULL && LevAIMessage[3] != NULL &&
        LevAIMessage[3]->value == 1.0f && 0.0f < LevAIMessage[0]->value) {

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

i32 SpeederChase_ObjIsAGroundTroop(GameObject_s *object) {
    if (object == NULL) {
        return 1;
    }
    return (object->apiobj.character_data->model_flags & 0x2000) == 0;
}

i32 SpeedersDroppedBack() {
    return WORLD->current_level == SPEEDERCHASEA_LDATA && disable_narrow_socks == 0 && set_speedermode == 2;
}

i32 Action_SpeederBeingChased(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **, i32, i32 first_time,
                              f32 elapsed) {
    SOCK *socks = WORLD->sock_sys->sock;
    if (__builtin_expect(packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL, 0))
        return 0;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (first_time != 0) {
        object->run_speed_override = object->apiobj.character_data->game_character->run_speed;
        processor->action_data_1 = 0;
        processor->action_timer = 0.0f;
        set_speedermode = 0;
        processor->action_data_2 = object->current_hp;
        speeder_hitpoints_lost = 0;
        speeder_mode_ahead_timer = 0.0f;
    } else if (__builtin_expect(processor->action_data_1 > 4, 0)) {
        processor->action_data_1 = 0;
    }
    const f32 base_speed = object->apiobj.character_data->game_character->run_speed;
    f32 distance = 0.0f;
    f32 error = 0.0f;
    i32 can_fire = 0;
    if (player->sock_position.location.sock == 1) {
        if (player2 != NULL) {
            f32 player_distance = MidDistanceFromSockStart(WORLD->sock_sys, &player2->sock_position);
            player_distance -= MidDistanceFromSockStart(WORLD->sock_sys, &player->sock_position);
            const f32 length = socks[1].unknown_98;
            if (player_distance > length * 0.5f)
                player_distance -= length;
            else if (player_distance < length * -0.5f)
                player_distance += length;
            GameObject_s *reference = player;
            if (__builtin_expect(speedervals[processor->action_data_1].distance < 0.0f, 0)) {
                if (__builtin_expect(!(player_distance > 0.0f), 1))
                    reference = player2;
            } else if (__builtin_expect(player_distance > 0.0f, 0)) {
                reference = player2;
            }
            distance = MidDistanceFromSockStart(WORLD->sock_sys, &object->sock_position);
            distance -= MidDistanceFromSockStart(WORLD->sock_sys, &reference->sock_position);
        } else {
            distance = MidDistanceFromSockStart(WORLD->sock_sys, &object->sock_position);
            distance -= MidDistanceFromSockStart(WORLD->sock_sys, &player->sock_position);
        }
        const f32 length = socks[1].unknown_98;
        if (distance > length * 0.5f)
            distance -= length;
        else if (distance < length * -0.5f)
            distance += length;
        can_fire = distance < speederfirerange;
        if (__builtin_expect(distance > speeder_kill_dist, 0)) {
            KillGameObject(object, 4, 0);
            return 1;
        }
        error = distance - speedervals[processor->action_data_1].distance;
        if (processor->action_data_1 == 0) {
            if (__builtin_expect(processor->action_timer < zoom_ahead_extra_time, 1))
                error -= (zoom_ahead_extra_time - processor->action_timer) / zoom_ahead_extra_time * zoom_ahead_extra;
            if (__builtin_expect(speeder_mode_ahead_timer < 2.0f, 0))
                speeder_hitpoints_lost = 0;
        }
    }
    // Retail has four tuning rows and only resets state bytes greater than 4.
    // State 4 therefore remains an invalid-input table overrun, not a fifth mode.
    f32 ratio, speed, seek;
    if (error > 0.0f) {
        SPEEDERCHASEVALUES *values = &speedervals[processor->action_data_1];
        ratio = error / values->behind_range;
        if (__builtin_expect(ratio > 1.0f, 0))
            ratio = 1.0f;
        speed = values->behind_speed * ratio + (1.0f - ratio);
        seek = values->behind_seek;
    } else {
        ratio = error / speedervals[processor->action_data_1].ahead_range;
        if (ratio > 1.0f)
            ratio = 1.0f;
        speed = speedervals[processor->action_data_1].ahead_speed * ratio + (1.0f - ratio);
        seek = speedervals[processor->action_data_1].ahead_seek;
    }
    object->run_speed_override = SeekValF(object->run_speed_override, speed * base_speed, seek);
    if (processor->speeder_ahead_latched != 0)
        speeder_mode_ahead_timer += elapsed;
    else if (__builtin_expect(distance > 0.0f, 1))
        processor->speeder_ahead_latched = 1;
    else
        processor->action_timer = 0.0f;
    if (__builtin_expect(processor->action_data_1 == 1 || processor->speeder_ahead_latched == 0, 0))
        object->movement_spline_offset.y =
            SeekValF(object->movement_spline_offset.y, speeder_level_height, speeder_level_height_lerpf);
    else
        object->movement_spline_offset.y = SeekValF(object->movement_spline_offset.y, 0.0f, speeder_level_height_lerpf);
    if (processor->action_data_2 > object->current_hp) {
        processor->action_data_2 = object->current_hp;
        ++speeder_hitpoints_lost;
    }
    if (object->character_context != -1)
        return 0;
    if (processor->action_data_1 != static_cast<u8>(set_speedermode)) {
        processor->action_data_1 = set_speedermode;
        if (set_speedermode == 1) {
            processor->action_timer = 0.25f;
        } else {
            processor->action_timer = 0.0f;
        }
    }
    switch (processor->action_data_1) {
        case 0:
            if (players_going_forward == 0) {
                processor->action_timer = 0.0f;
                break;
            }
            if (distance > speedervals[0].distance * 0.8f || processor->action_timer > 0.0f) {
                if (speeder_hitpoints_lost <= 1) {
                    processor->action_timer += elapsed;
                    if (__builtin_expect(!(processor->action_timer > speeder_ahead_time), 1))
                        break;
                }
                set_speedermode = 1;
                processor->action_data_1 = 1;
                processor->action_timer = 0.0f;
                speeder_hitpoints_lost = 0;
                speeder_mode_ahead_timer = 0.0f;
                object->pad_gamepad->buttons_down_08 |= GAMEPAD_JUMP;
            }
            break;
        case 1:
            if (__builtin_expect(players_going_forward == 0, 0))
                goto reset_mode;
            if (distance < 0.0f) {
                set_speedermode = 2;
                processor->action_data_1 = 2;
                goto reset_timers;
            }
            processor->action_timer -= elapsed;
            if (__builtin_expect(!(processor->action_timer > 0.0f), 0)) {
                if (processor->action_timer < -1.0f && player->character_context == 0x36) {
                    set_speedermode = 3;
                    processor->action_data_1 = 3;
                    goto reset_timers;
                }
            } else if (processor->action_timer < 0.0f) {
                processor->action_timer = 0.0f;
                object->pad_gamepad->buttons_down_08 |= GAMEPAD_JUMP;
            }
            break;
        case 2:
            if (players_going_forward == 0)
                goto reset_mode;
            if (player->character_context == 0x36) {
                set_speedermode = 3;
                processor->action_data_1 = 3;
                processor->action_timer = 0.0f;
                speeder_hitpoints_lost = 0;
                speeder_mode_ahead_timer = 0.0f;
            }
            if (speeder_shootrate > 0.0f) {
                const f32 shoot_interval = speeder_shootrate;
                processor->action_timer -= elapsed;
                if (processor->action_timer <= 0.0f) {
                    const f32 random = NuRandFloat();
                    processor->action_timer = shoot_interval * 0.5f + random * speeder_shootrate;
                    if (can_fire != 0)
                        object->pad_gamepad->buttons_down_08 |= GAMEPAD_ACTION;
                }
            }
            break;
        case 3:
            if (__builtin_expect(speeder_hitpoints_lost <= 2, 0)) {
                processor->action_timer += elapsed;
                if (!(processor->action_timer > speeder_midrange_time))
                    break;
            }
        reset_mode:
            set_speedermode = 0;
            processor->action_data_1 = 0;
        reset_timers:
            processor->action_timer = 0.0f;
            speeder_hitpoints_lost = 0;
            speeder_mode_ahead_timer = 0.0f;
            break;
    }
    return 0;
}

f32 SpeederChaseATATInOutMul(nuvec_s *start, nuvec_s *end) {
    f32 distance = NuVecXZDist(start, end, NULL) * 0.125f;
    if (distance > 1.0f)
        return 1.0f;
    return MAX(0.1f, distance);
}

