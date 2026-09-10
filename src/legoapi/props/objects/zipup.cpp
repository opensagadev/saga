#include "decomp.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/characters/core/players.h"
#include "nu2api/nu3d/nurndr.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/world/world.h"
#include "nu2api/numath/nuvec.h"
#include "globals.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

i16 LEGOACT_WHIP_SWING_START = -1;
i16 LEGOACT_WHIP_SWING_SWING = -1;
i16 LEGOACT_WHIP_SWING_JUMP = -1;
i32 ObjLandReady(GameObject_s *);
i32 SuperWeirdo(GameObject_s *);
i32 Cheat_IsOn(i32);
void SetHeadTarget(GameObject_s *, NUVEC *, i8, f32, f32, f32);
void StartJump(GameObject_s *, i32);
i32 StartFallLand(GameObject_s *, i32);
void SetWeaponIn(GameObject_s *);
void FastWeaponOut(GameObject_s *, i32);
void Hint_SetComplete(i32);
i32 GameAudio_GetPlrSfxBits(void *);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);
void PlayJumpSfx(GameObject_s *, i32);
i32 RotDiff(u16, u16);
extern "C" f32 AnimDuration(i32, i32, f32, f32, i32);

void ZipUp_MoveCode(GameObject_s *object, i32 special_pressed) {
    APIOBJECT &api = object->apiobj;
    if (object->character_context == 0x47) {
        if (object->action_movement_state == 1) {
            void *info = api.character_model->model_data_b[object->context_animation];
            f32 *playing = NULL;
            if (info != NULL)
                playing = AnimPlaying(&api.anim_packet, object->context_animation, 1, 0);
            if (info == NULL || playing != NULL) {
                object->context_animation_timer += FRAMETIME;
                if (object->context_animation_timer >= object->airborne_action_duration)
                    object->context_animation_timer = object->airborne_action_duration;
            }
            api.velocity = v000;
            NUVEC delta;
            if (object->field_0x7a3 == 0) {
                f32 fraction;
                bool animation_fraction = false;
                if (playing != NULL && *playing > 0.0f) {
                    f32 frame = AnimListFrame(api.character_model, object->context_animation, 0);
                    if (frame > 1.0f && AnimEndFrame(api.character_model, object->context_animation) > frame) {
                        f32 progress = (*playing - 1.0f) / (frame - 1.0f);
                        fraction = progress < 1.0f ? progress : 1.0f;
                        animation_fraction = true;
                    }
                }
                if (!animation_fraction)
                    fraction = object->context_animation_timer / object->airborne_action_duration;
                NuVecSub(&delta, &object->zipup_start_position, &object->zipup_entry_position);
                NuVecScale(&delta, &delta, fraction);
                NuVecAdd(&api.position, &object->zipup_entry_position, &delta);
                if (object->context_animation_timer >= object->airborne_action_duration) {
                    object->field_0x7a3 = 1;
                    object->context_animation_timer = 0.0f;
                    object->context_animation = LEGOACT_WHIP_SWING_SWING;
                    f32 duration = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 0);
                    object->airborne_action_duration = duration <= 0.0f ? 1.0f : duration;
                    GameAudio_PlaySfx(0x4d, &api.collision_position, GameAudio_GetPlrSfxBits(object), 0);
                }
            } else if (object->field_0x7a3 == 1) {
                f32 fraction = object->context_animation_timer / object->airborne_action_duration;
                NuVecSub(&delta, &object->zipup_swing_position, &object->zipup_start_position);
                NuVecScale(&delta, &delta, fraction);
                NuVecAdd(&api.position, &object->zipup_start_position, &delta);
                if (object->context_animation_timer >= object->airborne_action_duration) {
                    object->field_0x7a3 = 2;
                    object->context_animation_timer = 0.0f;
                    object->context_animation = LEGOACT_WHIP_SWING_JUMP;
                    f32 duration = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 0);
                    object->airborne_action_duration = duration <= 0.0f ? 1.0f : duration;
                    PlayJumpSfx(object, 0);
                }
            } else if (object->field_0x7a3 == 2) {
                f32 fraction = object->context_animation_timer / object->airborne_action_duration;
                NuVecSub(&delta, &object->zipup_landing_position, &object->zipup_swing_position);
                NuVecScale(&delta, &delta, fraction);
                NuVecAdd(&api.position, &object->zipup_swing_position, &delta);
                if (object->context_animation_timer >= object->airborne_action_duration) {
                    api.velocity.x = api.velocity.z = 0.0f;
                    api.velocity.y = -2.0f;
                    object->character_context = -1;
                    StartFallLand(object, LEGOACT_LAND);
                }
            }
            return;
        }
        ZIPUP *zipup = static_cast<ZIPUP *>(object->field_0x788);
        NUVEC *hook = &zipup->hook_origin;
        NUVEC *destination = object->field_0x7a3 == 0 ? &zipup->upper_position : &zipup->lower_position;
        if ((zipup->flags & 1) == 0) {
            if (NuVecDistSqr(&api.position, hook, NULL) < 0.01f || object->context_animation_timer >= 5.0f) {
                StartJump(object, 6);
                PlaySfx("GrapDetach", &api.collision_position);
                if ((api.flags_low & 0x80) != 0)
                    Hint_SetComplete(0x262);
                u16 angle = NuAtan2D(destination->x - api.position.x, destination->z - api.position.z);
                api.field_0x276 = api.facing_angle = api.movement_facing_angle = angle;
                f32 speed =
                    NuVecXZDist(&api.position, destination, NULL) / api.character_data->game_character->jump_duration;
                object->airborne_action_timer = speed;
                api.velocity.x = NU_SIN_LUT(angle) * speed;
                api.velocity.z = NU_COS_LUT(angle) * speed;
                object->context_flags &= ~0x20;
                static_cast<ZIPUP *>(object->field_0x788)->runtime_flags &= ~1;
                object->field_0x788 = NULL;
                return;
            }
            zipup = static_cast<ZIPUP *>(object->field_0x788);
        }
        if ((zipup->flags & 0xc0) != 0xc0 || (zipup->runtime_flags & 1) == 0 || (object->context_flags & 0x20) == 0) {
            object->character_context = -1;
            object->context_flags &= ~0x20;
            zipup->runtime_flags &= ~1;
            object->field_0x788 = NULL;
            return;
        }
        if ((zipup->flags & 1) == 0) {
            object->context_animation_timer += FRAMETIME;
            PlaySfx("GrapWindLp", &api.collision_position);
            return;
        }
        if (object->context_animation_timer >= 1.5f) {
            object->context_animation_timer = 0.0f;
            object->character_context = -1;
            zipup->occupant = NULL;
            object->context_flags &= ~0x20;
            static_cast<ZIPUP *>(object->field_0x788)->runtime_flags &= ~1;
            object->fall_animation_timer = 0.2f;
            api.velocity.x = object->target_velocity.x = v000.x;
            api.velocity.y = object->target_velocity.y = -1.0f;
            api.velocity.z = object->target_velocity.z = v000.z;
            object->field_0x788 = NULL;
            object->magnet_surface_angle = 0;
            PlaySfx("GrapDetach", &api.collision_position);
            if ((api.flags_low & 0x80) != 0)
                Hint_SetComplete(0x262);
            return;
        }
        zipup->runtime_flags |= 2;
        u16 start_angle = NuAtan2D(zipup->rider_start_offset.x, zipup->rider_start_offset.z);
        NUVEC end_offset;
        NuVecSub(&end_offset, destination, hook);
        end_offset.y += 0.5f;
        u16 end_angle = NuAtan2D(end_offset.x, end_offset.z);
        i32 difference = 0x8000 - static_cast<u16>(RotDiff(start_angle, end_angle));
        if (difference < 0)
            difference = -difference;
        i32 half = static_cast<i32>(difference * 0.5f);
        u16 axis = start_angle > 0x8000 ? start_angle + half : start_angle - half;
        NUVEC offset = zipup->rider_start_offset;
        NuVecRotateY(&offset, &offset, static_cast<u16>(-axis));
        f32 phase = object->context_animation_timer / 1.5f * 32768.0f;
        i32 pitch = static_cast<i32>((1.0f - (NU_SIN_LUT(static_cast<i32>(16384.0f + phase)) + 1.0f) * 0.5f) *
                                     zipup->pitch_adjustment);
        NuVecRotateX(&zipup->rider_target_position, &offset, pitch);
        NuVecRotateY(&zipup->rider_target_position, &zipup->rider_target_position, axis);
        zipup->rider_target_position.y *= 1.0f - (1.0f - NuTrigTable[0x1000]) * NU_SIN_LUT(static_cast<i32>(phase));
        NuVecAdd(&zipup->rider_target_position, hook, &zipup->rider_target_position);
        f32 old_time = object->context_animation_timer;
        object->context_animation_timer += FRAMETIME;
        if (old_time < 0.55f && object->context_animation_timer >= 0.55f)
            PlaySfx("GrapSwing", &api.collision_position);
        return;
    }
    if (!ObjLandReady(object) && !objInNetWaitContext(object, 0x47))
        return;
    if ((api.flags_low & 0x80) != 0) {
        if ((api.character_data->model_flags & 0x100000) == 0 && !SuperWeirdo(object)) {
            if ((api.character_data->model_flags & 8) == 0 ||
                (api.character_data->game_character->flags_094[1] & 0x80) != 0 || !Cheat_IsOn(13))
                return;
        }
    } else if (object->use_action != 4 || (api.character_data->model_flags & 0x100000) == 0) {
        return;
    }
    i32 endpoint;
    ZIPUP *zipup = ZipUp_FindNearest(WORLD, &api.lower_position, api.collision_radius, NULL, &endpoint, object, false);
    if (objInNetWaitContext(object, 0x47)) {
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer <= 0.0f)
            object->character_context = -1;
    }
    if (zipup == NULL)
        return;
    if ((special_pressed == 0 && !objInNetWaitContext(object, 0x47)) ||
        ((zipup->flags & 1) != 0 && (zipup->runtime_flags & 1) != 0)) {
        SetHeadTarget(object, &zipup->hook_origin, 4, 2.0f, 1.0f, 2.0f);
        return;
    }
    if ((api.character_data->model_flags & 0x100000) == 0) {
        NUVEC *destination = endpoint == 0 ? &zipup->upper_position : &zipup->lower_position;
        if (StartBigJump(object, destination, 0, 0.5f, 1.0f, 0, 0))
            object->context_variant_flags |= 1;
        return;
    }
    if ((api.flags_low & 0x80) != 0)
        Hint_SetComplete(0x262);
    object->field_0x788 = zipup;
    object->character_context = 0x47;
    zipup->occupant = object;
    object->context_flags |= 0x20;
    static_cast<ZIPUP *>(object->field_0x788)->runtime_flags |= 1;
    if ((api.character_data->game_character->flags_094[3] & 0x20) != 0) {
        object->action_movement_state = 1;
        SetWeaponIn(object);
        GameAudio_PlaySfx(0x4c, &api.collision_position, GameAudio_GetPlrSfxBits(object), 0);
        object->field_0x7a3 = 0;
        object->context_animation_timer = 0.0f;
        object->context_animation = LEGOACT_WHIP_SWING_START;
        f32 duration = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 0);
        object->airborne_action_duration = duration <= 0.0f ? 0.5f : duration;
        object->zipup_entry_position = api.position;
        ZIPUP *current = static_cast<ZIPUP *>(object->field_0x788);
        object->zipup_start_position = endpoint == 0 ? current->lower_position : current->upper_position;
        object->zipup_landing_position = endpoint == 0 ? current->upper_position : current->lower_position;
        object->zipup_landing_position.y = endpoint == 0 ? zipup->upper_ground_height : zipup->lower_ground_height;
        NUVEC offset;
        NuVecSub(&offset, &object->zipup_landing_position, &object->zipup_start_position);
        offset.y = 0.0f;
        NuVecNorm(&offset, &offset);
        NuVecScale(&offset, &offset, 1.923615932464599609375f);
        NuVecAdd(&object->zipup_swing_position, &object->zipup_start_position, &offset);
        object->zipup_swing_position.y += 0.25150299072265625f;
        api.movement_facing_angle = NuAtan2D(object->zipup_landing_position.x - object->zipup_start_position.x,
                                             object->zipup_landing_position.z - object->zipup_start_position.z);
        api.field_0x27d = 0;
        return;
    }
    zipup = static_cast<ZIPUP *>(object->field_0x788);
    object->action_movement_state = 0;
    object->context_animation = 0x2a;
    NUVEC *hook = &zipup->hook_origin;
    NUVEC *start = endpoint == 0 ? &zipup->lower_position : &zipup->upper_position;
    NUVEC *destination = endpoint == 0 ? &zipup->upper_position : &zipup->lower_position;
    object->field_0x7a3 = endpoint == 0 ? 0 : 1;
    NUVEC *heading = start->x == hook->x && start->z == hook->z ? destination : hook;
    api.movement_facing_angle = NuAtan2D(heading->x - start->x, heading->z - start->z);
    api.velocity.y = 0.0f;
    if (api.character_data->game_character->field275_0x116 == 2)
        FastWeaponOut(object, 1);
    object->context_animation_timer = 0.0f;
    PlaySfx("GrapAttach", &api.upper_position);
    FastWeaponOut(object, 0);
    object->field_0xe31 = 0;
    f32 dx = hook->x - start->x, dz = hook->z - start->z;
    i32 angle = NuAtan2D(hook->y - start->y, NuFsqrt(dx * dx + dz * dz));
    i32 pitch = 0x4000 - (angle < 0 ? -angle : angle);
    object->magnet_surface_angle = angle < 0 ? -pitch : pitch;
    zipup = static_cast<ZIPUP *>(object->field_0x788);
    if ((zipup->flags & 1) != 0) {
        NuVecSub(&zipup->rider_start_offset, start, hook);
        NUVEC end_offset;
        NuVecSub(&end_offset, destination, hook);
        end_offset.y += 0.5f;
        f32 rider_height = 0.5f * api.scaled_height;
        zipup->rider_start_offset.x *= 0.9f;
        zipup->rider_start_offset.y *= 0.9f;
        zipup->rider_start_offset.y = rider_height + zipup->rider_start_offset.y;
        zipup->rider_start_offset.z *= 0.9f;
        i32 yaw = -static_cast<u16>(NuAtan2D(zipup->rider_start_offset.x, zipup->rider_start_offset.z));
        NUVEC start_offset = zipup->rider_start_offset;
        NuVecRotateY(&start_offset, &start_offset, yaw);
        NuVecRotateY(&end_offset, &end_offset, yaw);
        NuVecNorm(&start_offset, &start_offset);
        NuVecNorm(&end_offset, &end_offset);
        static_cast<ZIPUP *>(object->field_0x788)->pitch_adjustment =
            NuACos(start_offset.y * end_offset.y + start_offset.z * end_offset.z);
        zipup = static_cast<ZIPUP *>(object->field_0x788);
        zipup->rider_target_position = zipup->rider_start_offset;
        NuVecAdd(&zipup->rider_target_position, hook, &zipup->rider_target_position);
    }
}

extern numtl_s *ropemtl;
void DrawRopeSingle(NUVEC *, NUVEC *, f32, numtl_s *, f32, f32, f32, f32);

static __used__ void ZipUp_GetStartPoint(GameObject_s *object, NUVEC *position) {
    GAMECHARACTERDATA *character = object->apiobj.character_data->game_character;
    i32 joint = character->grapple_locators[0];
    if (joint != -1 && object->apiobj.character_model->points_of_interest[joint] != NULL &&
        object->apiobj.field_0x288 != 0) {
        position->x = object->joint_matrices[joint].m30;
        position->y = object->joint_matrices[joint].m31;
        position->z = object->joint_matrices[joint].m32;
        return;
    }
    joint = character->weapon_shoot_joints[0];
    if (joint != -1 && object->apiobj.character_model->points_of_interest[joint] != NULL &&
        object->apiobj.field_0x288 != 0) {
        position->x = object->joint_matrices[joint].m30;
        position->y = object->joint_matrices[joint].m31;
        position->z = object->joint_matrices[joint].m32;
        return;
    }
    joint = character->weapon_joints[0];
    if (joint != -1 && object->apiobj.character_model->points_of_interest[joint] != NULL &&
        object->apiobj.field_0x288 != 0) {
        position->x = object->joint_matrices[joint].m30;
        position->y = object->joint_matrices[joint].m31;
        position->z = object->joint_matrices[joint].m32;
        return;
    }
    position->x = object->apiobj.collision_position.x;
    position->y = object->apiobj.collision_position.y + object->apiobj.collision_height;
    position->z = object->apiobj.collision_position.z;
}

void ZipUps_DrawLines() {
    GameObject_s *object = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++object) {
        NURND_VERTEX3D start, end;
        if (object->character_context == 0x47) {
            if (object->action_movement_state != 0)
                continue;
            ZipUp_GetStartPoint(object, &start.position);
            f32 time = object->context_animation_timer;
            start.colour = 0xffffffff;
            end.position = static_cast<ZIPUP *>(object->field_0x788)->hook_position;
            if (time < 0.2f) {
                f32 fraction = time / 0.2f;
                end.position.x = (end.position.x - start.position.x) * fraction + start.position.x;
                end.position.y = (end.position.y - start.position.y) * fraction + start.position.y;
                end.position.z = (end.position.z - start.position.z) * fraction + start.position.z;
            }
            end.colour = 0xff808080;
            DrawRopeSingle(&start.position, &end.position, 1.0f, ropemtl, time, 0.2f, 3.5f, 1.0f);
        } else if (object->character_context == 0x35) {
            end.position = object->external_force;
            start.colour = 0xffffffff;
            end.colour = 0xff808080;
            ZipUp_GetStartPoint(object, &start.position);
            DrawRopeSingle(&start.position, &end.position, 1.0f, ropemtl, object->context_animation_timer, 0.2f, 3.5f,
                           1.0f);
        }
    }
}

ZIPUP *ZipUp_FindNearest(WORLDINFO_s *world, nuvec_s *position, float radius, float *distance, i32 *endpoint,
                         GameObject_s *object, bool touch) {
    if (world == NULL || object == NULL || object->apiobj.character_data == NULL ||
        object->apiobj.character_data->game_character == NULL)
        return NULL;
    if (touch && !TouchHacks::CanUseZipup(*object))
        return NULL;
    if (world->zipups == NULL)
        return NULL;
    float nearest_distance = (radius + 0.25f) * (radius + 0.25f);
    ZIPUP *nearest = NULL;
    ZIPUP *zipup = world->zipups;
    for (i32 i = 0; i < world->zipup_count; ++i, ++zipup) {
        if (zipup != NULL) {
            if ((zipup->flags & 0xc0) != 0xc0)
                continue;
            if ((zipup->flags & 0x20) != 0) {
                if ((object->apiobj.character_data->game_character->flags_094[3] & 0x20) == 0)
                    continue;
            } else {
                if ((object->apiobj.character_data->game_character->flags_094[3] & 0x20) != 0)
                    continue;
                if ((zipup->flags & 2) == 0 && object->apiobj.field_0x27c != -1)
                    continue;
            }
        }
        float candidate = NuVecDistSqr(position, &zipup->lower_position, NULL);
        if (candidate < nearest_distance) {
            nearest = zipup;
            nearest_distance = candidate;
            if (endpoint != NULL)
                *endpoint = 0;
        }
        if ((zipup->flags & 8) != 0) {
            candidate = NuVecDistSqr(position, &zipup->upper_position, NULL);
            if (candidate < nearest_distance) {
                if (endpoint != NULL)
                    *endpoint = 2;
                nearest = zipup;
                nearest_distance = candidate;
            }
        }
    }
    if (distance != NULL)
        *distance = nearest_distance;
    return nearest;
}

i32 ZipUps_UpdateHint(HINT_s *hint) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if (world == NULL || world->zipups == NULL)
        return 0;
    for (i32 i = 0; i < world->zipup_count; ++i) {
        ZIPUP *zipup = &world->zipups[i];
        if (hint->completion_flags[MechInputTouchSystem::s_baseControlMode] == 0 && CanDrawZipUpSwirls != 0 &&
            (zipup->flags & ZIPUP_FLAG_ACTIVE) != 0 && ActivePlayerInRange(&zipup->lower_position, 2.0f, NULL))
            return 1;
    }
    return 0;
}
