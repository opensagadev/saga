#include "decomp.h"
#include "legoapi/gizmos/traps/attractos.h"
#include "legoapi/world/world.h"
#include "nu2api/numath/nuvec.h"
#include "globals.h"
#include "legoapi/gizmos/traps/shards.h"
#include "legoapi/characters/motion.h"
#include "legoapi/render/fx.h"
#include "nu2api/numath/nutrig.h"
#include <stdio.h>
#include <math.h>
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void *AddGameMessage(char *, NUVEC *, f32, NUVEC *, f32, u8, u8, u8, u32, f32);
f32 SeekValF(f32, f32, f32);
i32 GameRayCast(NUVEC *, NUVEC *, f32, i32);
void Shard_Collect(SHARD_s *, GameObject_s *);
void KeepPointOnScreen(NUVEC *, NUVEC *);
void Batarang_GetSightInfo(i32, i32 *, i32 *, i32 *, char *);
i32 qrand();
void NewRumble(nupad_s *, f32, i32);

void Attracto_MoveCode(WORLDINFO_s *world, GameObject_s *object) {
    bool available = false;
    if ((object->apiobj.flags_low & 0x80) != 0 && object->suit != NULL &&
        (static_cast<SUIT_s *>(object->suit)->flags & 0x80) != 0) {
        static NUVEC counter_offset = {0.0f, 0.45f, 0.15f};
        char text[32];
        sprintf(text, "%i", object->field_0x106e);
        NUVEC position;
        NuVecMtxRotate(&position, &counter_offset, &object->apiobj.field_0xb8);
        NuVecAdd(&position, &position, &object->apiobj.position);
        GAMEMESSAGE_s *message = static_cast<GAMEMESSAGE_s *>(
            AddGameMessage(text, &position, 1.3f, NULL, 0.0f, 255, 255, 255, 0x1087, 0.0f));
        if (message != NULL)
            message->alpha = static_cast<i32>((0.2f * game_pulse + 0.6f) * 128.0f);
        available = true;
    }
    if (object->character_context == 0x53) {
        if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL &&
            AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0) == NULL)
            return;
        if ((object->pad_gamepad->buttons_held & GAMEPAD_SPECIAL) != 0) {
            object->context_animation_timer += FRAMETIME;
            object->airborne_action_duration -= FRAMETIME;
            if (!(object->airborne_action_duration <= 0.0f))
                return;
            if (object->field_0x106e != 0) {
                ATTRACTO_s *attracto = static_cast<ATTRACTO_s *>(object->field_0x788);
                if (attracto->collected_count < attracto->capacity) {
                    --object->field_0x106e;
                    ++attracto->collected_count;
                    object->airborne_action_duration = 0.2f;
                    attracto = static_cast<ATTRACTO_s *>(object->field_0x788);
                    if (attracto->collected_count == attracto->capacity)
                        attracto->state_flags |= 4;
                    else if (object->field_0x106e != 0 && object->character_context != -1)
                        return;
                }
            }
            goto begin_suction;
        }
        object->airborne_action_duration -= FRAMETIME;
        if (object->airborne_action_duration <= 0.0f)
            object->character_context = -1;
        return;
    }
    if (object->character_context == 0x52) {
        if ((object->pad_gamepad->buttons_held & GAMEPAD_SPECIAL) == 0 && !(object->context_animation_timer < 0.3f)) {
            object->character_context = -1;
            return;
        }
        if (object->apiobj.character_model->model_data_b[object->context_animation] == NULL ||
            AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0) != NULL) {
            object->context_animation_timer += FRAMETIME;
            f32 x = object->pad_gamepad->input_direction_z * 1.25f;
            f32 y = object->pad_gamepad->input_direction_x * 1.25f;
            if (object->context_animation_timer < 0.25f) {
                f32 ramp = object->context_animation_timer * 4.0f;
                x *= ramp;
                y *= ramp;
            }
            object->launch_origin.x = SeekValF(object->launch_origin.x, x, 10.0f);
            object->launch_origin.y = SeekValF(object->launch_origin.y, y, 10.0f);
            object->external_force.x += object->launch_origin.x * FRAMETIME;
            object->external_force.y += object->launch_origin.y * FRAMETIME;
            SHARD_s *nearest = NULL;
            if (world->shards != NULL) {
                f32 best = 64.0f;
                SHARD_s *shard = static_cast<SHARD_s *>(world->shards);
                for (i32 i = 0; i < world->shard_count; ++i, ++shard) {
                    if ((shard->state_flags & 0x1f) != 0x13 ||
                        !(fabsf(shard->screen_position.x - object->external_force.x) < 0.2f) ||
                        !(fabsf(shard->screen_position.y - object->external_force.y) < 0.2f))
                        continue;
                    NUVEC delta;
                    f32 distance = NuVecDistSqr(&shard->position, &object->apiobj.collision_position, &delta);
                    if (distance < best) {
                        best = distance;
                        nearest = shard;
                    }
                }
            }
            if (nearest != NULL) {
                NUVEC origin = {object->apiobj.pos_x, (object->apiobj.pos_y + object->apiobj.field_0x194) * 0.5f,
                                object->apiobj.pos_z};
                NUVEC target = {0.0f, 0.05f, 0.0f};
                if (nearest->angle_z != 0)
                    NuVecRotateZ(&target, &target, nearest->angle_z);
                if (nearest->angle_x != 0)
                    NuVecRotateX(&target, &target, nearest->angle_x);
                NuVecAdd(&target, &target, &nearest->position);
                NUVEC delta;
                NuVecSub(&delta, &target, &origin);
                if (GameRayCast(&origin, &delta, 0.0f, 0x1f) == 0) {
                    object->apiobj.movement_facing_angle = NuAtan2D(nearest->position.x - object->apiobj.pos_x,
                                                                    nearest->position.z - object->apiobj.pos_z);
                    if (ParticlesPerSecond(3.0f, FRAMETIME) > 0)
                        Shard_Collect(nearest, object);
                    else {
                        nearest->state_flags |= 0x20;
                        NewRumble(object->pad_gamepad->pad, static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 0.4f, 0);
                    }
                }
            }
        }
        KeepPointOnScreen(&object->external_force, &object->launch_origin);
        i32 red, green, blue;
        Batarang_GetSightInfo(object->id, &red, &green, &blue, NULL);
        AddGameMessage("[  ]", &object->external_force, 1.0f, NULL, 0.0f, red, green, blue, 0x1080, 0.0f);
        return;
    }
    if (!available || object->apiobj.field_0x27d == 0 || !ObjLandReady(object))
        return;
    {
        f32 radius = object->apiobj.field_0x1dc + 0.25f;
        f32 distance;
        ATTRACTO_s *attracto = Attracto_FindNearest(world, &object->apiobj.lower_position, object, &distance);
        if (attracto != NULL && radius * radius > distance && object->field_0x106e != 0 &&
            (object->pad_gamepad->buttons_pressed & GAMEPAD_SPECIAL) != 0) {
            object->character_context = 0x53;
            object->field_0x788 = attracto;
            object->context_animation = 0x9b;
            object->context_animation_timer = 0.0f;
            object->apiobj.movement_facing_angle = attracto->angle;
            return;
        }
    }
    if (object->apiobj.model_draw_result == 0 || !(object->camera_screen_position.z > 0.0f) ||
        (object->pad_gamepad->buttons_held & GAMEPAD_SPECIAL) == 0)
        return;
begin_suction:
    object->character_context = 0x52;
    object->context_animation = 0x9a;
    object->context_animation_timer = 0.0f;
    object->external_force.x = object->camera_screen_position.x;
    object->external_force.y = object->camera_screen_position.y;
    object->external_force.z = 1.0f;
    object->launch_origin = v000;
    object->airborne_action_duration = 0.2f;
}

void Attracto_GetPos_Top(ATTRACTO_s *attracto, NUVEC *position) {
    if (position != NULL && attracto != NULL) {
        *position = attracto->position;
        position->y = 1.0f + position->y;
    }
}

ATTRACTO_s *Attracto_FindNearest(WORLDINFO_s *world, NUVEC *position, GameObject_s *object, f32 *distance) {
    ATTRACTO_s *attracto = static_cast<ATTRACTO_s *>(world->attractos);
    ATTRACTO_s *nearest = NULL;
    f32 best = 1.0e9f;
    for (i32 i = 0; i < world->attracto_count; ++i, ++attracto) {
        f32 candidate;
        if (object != NULL) {
            if ((attracto->state_flags & 7) != 3)
                continue;
            candidate = NuVecDistSqr(position, &attracto->active_position, NULL);
        } else {
            candidate = NuVecDistSqr(position, &attracto->position, NULL);
        }
        if (candidate < best) {
            best = candidate;
            nearest = attracto;
        }
    }
    if (distance != NULL)
        *distance = best;
    return nearest;
}

extern "C" i16 NewPlatPickupInst(void *, i32);
void Attractos_InitTerrain(WORLDINFO_s *world) {
    for (i32 i = 0; i < world->attracto_count; ++i) {
        ATTRACTO_s *attracto = &static_cast<ATTRACTO_s *>(world->attractos)[i];
        attracto->platform_id = NewPlatPickupInst(&attracto->transform, 4);
    }
}

void Attracto_GetSuctionPos(GameObject_s *object, NUVEC *position) {
    *position = object->apiobj.collision_position;
}
