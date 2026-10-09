#include "decomp.h"
#include "legoapi/items/collect/batarang.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/actions/movement/jumping.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nucore/nustring.h"
#include <math.h>
#include <string.h>

extern BATARANG_s Batarang[8];
extern "C" i16 id_ROBIN;
extern i16 id_CATWOMAN;

void NewRumble(nupad_s *, f32, i32);
void NewBuzzFrames(nupad_s *, i32, i32);
void GameCam_HitJudder();
i32 StunGameObject(GameObject_s *, GameObject_s *, f32, i32);
void Detonator_Detonate(DETONATOR_s *);
i32 GizmoBlowupBlowup(GIZMOBLOWUP_s *, i32, i32, i32, GameObject_s *, i32);
i32 TerrainPlatId();
f32 SeekValF(f32, f32, f32);

i32 Batarang_SeekToTarget(BATARANG_s *);
i32 Batarang_StartTargetting(GameObject_s *);
i32 Batarang_GetObjectFromCharID(i32);
static i32 Batarang_FindTarget(WORLDINFO_s *world, GameObject_s *object, i32 automatic);

u16 GetShootDirection_Batman(GameObject_s *object, nuvec_s *direction) {
    NUVEC temporary;
    if (direction == NULL) {
        direction = &temporary;
    }
    u16 angle;
    if (object->field_0x1086 == 4) {
        NUMTX matrix = object->apiobj.field_0xb8;
        NuMtxPreRotateY(&matrix, 0x8000);
        NuVecMtxRotate(direction, &v001, &matrix);
        angle = object->apiobj.facing_angle;
    } else {
        if ((object->apiobj.character_data->model_flags & 0x2000) == 0 &&
            (object->apiobj.character_data->game_character->flags_090 & 0x80) == 0) {
            angle = object->apiobj.movement_facing_angle;
        } else {
            angle = object->apiobj.facing_angle;
            if (object->character_context == 0x2a &&
                0.25f <= 1.0f - object->context_animation_timer / object->airborne_action_duration) {
                angle -= 0x8000;
            }
        }
        direction->x = NU_SIN_LUT(angle);
        direction->y = 0.0f;
        direction->z = NU_COS_LUT(angle);
    }
    return angle;
}

static unsigned int Batarang_GetTargetPos(BATARANG_s *batarang, int index, nuvec_s *position) {
    if (batarang->field_0x7d != 0 && batarang->active != 0 && batarang->active == index) {
        GameObject_s *owner = batarang->owner;
        *position = owner->apiobj.collision_position;
        if ((owner->field_0xe24 & 8) != 0) {
            i32 joint = owner->apiobj.character_data->player_config->batarang_joint;
            if (joint != -1 && owner->apiobj.character_model->points_of_interest[joint] != NULL)
                *position = *NUMTX_GET_ROW_VEC(&owner->joint_matrices[joint], 3);
        }
        return 1;
    }
    if (index < 0 || index >= 5 || index >= batarang->active)
        return 0;
    BATARANG_TARGET_s *target = &batarang->targets[index];
    switch (target->type) {
        case 0:
            *position = static_cast<GameObject_s *>(target->object)->apiobj.collision_position;
            return 1;
        case 1:
            *position = static_cast<DETONATOR_s *>(target->object)->field_0x0c;
            return 1;
        case 2:
            *position = static_cast<GIZMOBLOWUP_s *>(target->object)->mid_position;
            return 1;
        case 3:
            *position = *static_cast<NUVEC *>(target->object);
            return 1;
        default:
            return 0;
    }
}

static i32 Batarang_FindTarget(WORLDINFO_s *world, GameObject_s *object, i32 automatic) {
    BATARANG_s *batarang = static_cast<BATARANG_s *>(object->batarang);
    if (batarang == NULL || batarang->active > 5) {
        return 0;
    }
    NUVEC forward;
    NuVecRotateY(&forward, &v001, object->apiobj.movement_facing_angle);
    void *selected = NULL;
    i32 selected_type = -1;
    NUVEC selected_position;
    f32 selected_distance = 64.0f;
    GameObject_s *nearest_object = NULL;
    f32 nearest_distance = 16.0f;
    GameObject_s *first_object = NULL;
    GameObject_s *candidate = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++candidate) {
        const i8 context = candidate->character_context;
        if ((candidate->apiobj.object_flags & 0x1001) != 0x1001 || candidate->apiobj.field_0x287 != 0 ||
            (CInfo[context].flags & 0x8000) != 0 || candidate->apiobj.model_draw_result == 0 || candidate == object ||
            candidate->apiobj.field_0x27c != -1 || (static_cast<u8>(context) & 0xfd) == 0x39 || context == 0x3c ||
            (object->apiobj.field_0x1f4 & 0x10004) != 0 ||
            ((object->apiobj.field_0x1f4 ^ candidate->apiobj.field_0x1f4) & 1) == 0) {
            continue;
        }
        i32 target;
        for (target = 0; target < batarang->active; ++target) {
            if (batarang->targets[target].object == candidate) {
                break;
            }
        }
        if (target != batarang->active) {
            continue;
        }
        NUVEC delta;
        if (automatic != 0) {
            f32 distance =
                NuVecDistSqr(&candidate->apiobj.collision_position, &object->apiobj.collision_position, &delta);
            if (!(delta.x * forward.x + delta.z * forward.z > 0.0f)) {
                continue;
            }
            if (distance < nearest_distance) {
                nearest_distance = distance;
                nearest_object = candidate;
            }
        } else {
            if (fabsf(candidate->camera_screen_position.x - batarang->sight_position.x) < 0.1f &&
                fabsf(candidate->camera_screen_position.y - batarang->sight_position.y) < 0.2f &&
                NuVecDistSqr(&candidate->apiobj.collision_position, &object->apiobj.collision_position, &delta) <
                    selected_distance) {
                first_object = candidate;
                break;
            }
        }
    }
    if (nearest_object != NULL && nearest_distance < selected_distance) {
        selected = nearest_object;
        selected_distance = nearest_distance;
    } else if (first_object != NULL) {
        selected = first_object;
    }
    if (selected != NULL) {
        GameObject_s *candidate = static_cast<GameObject_s *>(selected);
        selected_position = candidate->apiobj.collision_position;
        selected_position.y = (candidate->apiobj.collision_position.y + candidate->apiobj.upper_position.y) * 0.5f;
        selected_type = 0;
    }
    if (automatic == 0) {
        for (DETONATOR_s *candidate = Detonator; candidate != Detonator + 10; ++candidate) {
            if (candidate->active == 0 || candidate->draw_result == 0 || candidate->timer == 0.0f) {
                continue;
            }
            i32 target;
            for (target = 0; target < batarang->active; ++target) {
                if (batarang->targets[target].object == candidate) {
                    break;
                }
            }
            if (target != batarang->active || !(fabsf(candidate->field_0x18.x - batarang->sight_position.x) < 0.1f) ||
                !(fabsf(candidate->field_0x18.y - batarang->sight_position.y) < 0.1f)) {
                continue;
            }
            f32 distance = NuVecDistSqr(&candidate->field_0x0c, &object->apiobj.collision_position, NULL);
            if (distance < selected_distance) {
                selected_position = candidate->field_0x0c;
                selected_type = 1;
                selected_distance = distance;
                selected = candidate;
            }
        }
    }
    GIZMOBLOWUP_s *nearest_blowup = NULL;
    GIZMOBLOWUP_s *blowup = NULL;
    nearest_distance = 16.0f;
    GIZMOBLOWUP_s *candidate_blowup = world->gizmo_blowups;
    for (i32 i = 0; i < world->gizmo_blowup_count; ++i, ++candidate_blowup) {
        GIZMOBLOWUP_s *candidate = candidate_blowup;
        if ((candidate->status_flags & 0x80c001) != 0x80c000 || (candidate->draw_flags & 1) == 0 ||
            ((candidate->draw_flags & 0x20) != 0 && ShadowMode == 0)) {
            continue;
        }
        i32 target;
        for (target = 0; target < batarang->active; ++target) {
            if (batarang->targets[target].object == candidate) {
                break;
            }
        }
        if (target != batarang->active) {
            continue;
        }
        NUVEC delta;
        if (automatic == 0) {
            if (!(fabsf(candidate->screen_position.x - batarang->sight_position.x) < 0.1f) ||
                !(fabsf(candidate->screen_position.y - batarang->sight_position.y) < 0.1f)) {
                continue;
            }
            if (NuVecDistSqr(&candidate->mid_position, &object->apiobj.collision_position, &delta) <
                selected_distance) {
                blowup = candidate;
                break;
            }
        } else {
            f32 distance = NuVecDistSqr(&candidate->mid_position, &object->apiobj.collision_position, &delta);
            if (!(delta.x * forward.x + delta.z * forward.z > 0.0f)) {
                continue;
            }
            if (distance < nearest_distance) {
                nearest_distance = distance;
                nearest_blowup = candidate;
            }
        }
    }
    if (nearest_blowup != NULL && nearest_distance < selected_distance) {
        blowup = nearest_blowup;
    }
    i16 platform = -1;
    if (blowup != NULL) {
        selected_position = blowup->mid_position;
        platform = blowup->platform_id;
        selected = blowup;
        selected_type = 2;
    }
    if (selected == NULL) {
        return 0;
    }
    NUVEC origin = object->apiobj.collision_position;
    origin.y = (origin.y + object->apiobj.upper_position.y) * 0.5f;
    NUVEC ray;
    NuVecSub(&ray, &selected_position, &origin);
    if (GameRayCast(&origin, &ray, 0.0f, 0x1f) != 0) {
        if (selected_type != 2 || platform == -1 || TerrainPlatId() != platform) {
            return 0;
        }
    }
    u8 count = batarang->active;
    if (count == 5) {
        if (static_cast<i32>(GameTimer.time_elapsed / 0.2f) == static_cast<i32>(GameTimer.last_time_elapsed / 0.2f)) {
            return 0;
        }
        for (i32 i = 0; i < count - 1; ++i) {
            batarang->targets[i] = batarang->targets[i + 1];
        }
        --count;
    }
    batarang->targets[count].object = selected;
    batarang->targets[count].type = static_cast<u8>(selected_type);
    batarang->targets[count].lost = 0;
    batarang->active = count + 1;
    return 1;
}

void Batarang_Release(GameObject_s *object, i32 mode) {
    BATARANG_s *batarang = static_cast<BATARANG_s *>(object->batarang);
    if (batarang == NULL) {
        return;
    }
    batarang->field_0x7d = 1;
    batarang->owner = object;
    static_cast<BATARANG_s *>(object->batarang)->current_target = 0;
    batarang = static_cast<BATARANG_s *>(object->batarang);
    batarang->flight_time = 0.0f;
    batarang->velocity.x = NU_SIN_LUT(object->apiobj.movement_facing_angle) * 4.0f;
    batarang->velocity.y = object->apiobj.velocity.y * 0.5f;
    batarang->velocity.z = 4.0f * NU_COS_LUT(object->apiobj.movement_facing_angle);
    NewRumble(object->pad_gamepad->pad, 0.5f, 0);
    batarang = static_cast<BATARANG_s *>(object->batarang);
    batarang->position = object->apiobj.collision_position;
    if ((object->field_0xe24 & 8) != 0) {
        i32 joint = object->apiobj.character_data->player_config->batarang_joint;
        if (joint != -1 && object->apiobj.character_model->points_of_interest[joint] != NULL) {
            batarang->position = *NUMTX_GET_ROW_VEC(&object->joint_matrices[joint], 3);
        }
    }

    if (batarang->active == 0 && mode == 2) {
        if (WORLD != NULL) {
            Batarang_FindTarget(WORLD, object, 1);
        }
        batarang = static_cast<BATARANG_s *>(object->batarang);
        if (batarang->active == 0) {
            const u16 angle = object->apiobj.movement_facing_angle;
            const f32 x = NU_SIN_LUT(angle);
            const f32 z = NU_COS_LUT(angle);
            batarang->targets[0].fallback_position = {
                x + x + object->apiobj.collision_position.x,
                object->apiobj.collision_position.y,
                z + z + object->apiobj.collision_position.z,
            };
            batarang->targets[0].object = &batarang->targets[0].fallback_position;
            batarang->targets[0].type = 3;
            static_cast<BATARANG_s *>(object->batarang)->targets[0].lost = 0;
            ++static_cast<BATARANG_s *>(object->batarang)->active;
        }
    }
}

void Batarang_StartThrowQuick(GameObject_s *object) {
    ResetAnimPacket(&object->apiobj.anim_packet, -1);
    object->character_context = 0x5e;
    object->context_animation = 0xb2;
    object->context_animation_timer = AnimDuration(object->id, 0xb2, 0.0f, 0.0f, 1);
}

void Batarangs_Reset() {
    for (i32 i = 0; i < 8; ++i) {
        Batarang[i].field_0x7d = 0;
        Batarang[i].active = 0;
        Batarang[i].owner = NULL;
        Batarang[i].cooldown = 50;
    }
}

i32 Batarang_InitRicochet(BATARANG_s *batarang, nuvec_s *normal) {
    if (batarang == NULL || normal == NULL) {
        return 0;
    }
    if (batarang->ricochet_count >= 5) {
        --batarang->ricochet_count;
        batarang->ricochet_flags &= ~1u;
        batarang->ricochet_normal = v000;
        return 0;
    }
    batarang->ricochet_flags |= 1;
    batarang->ricochet_normal = *normal;
    ++batarang->ricochet_count;
    batarang->ricochet_timer = 0.0f;
    NUVEC original_velocity = batarang->velocity;
    NUVEC reflected_velocity = batarang->velocity;
    NUVEC axis;
    NuVecCross(&axis, &batarang->ricochet_normal, &reflected_velocity);
    NuVecNorm(&axis, &axis);
    NUMTX rotation;
    NuMtxSetIdentity(&rotation);
    f32 magnitudes = NuVecMag(&batarang->ricochet_normal) * NuVecMag(&reflected_velocity);
    f32 dot = NuVecDot(&batarang->ricochet_normal, &reflected_velocity);
    f32 cosine = magnitudes == 0.0f || dot == 0.0f ? 0.0f : dot / magnitudes;
    i16 angle = 0x4000 - NuASin(cosine);
    NuMtxSetRotationAxis(&rotation, 0x8000 - angle * 2, &axis);
    NuVecMtxRotate(&reflected_velocity, &reflected_velocity, &rotation);
    f32 speed = NuVecNorm(&original_velocity, &original_velocity);
    NuVecNorm(&reflected_velocity, &reflected_velocity);
    NuVecAdd(&reflected_velocity, &reflected_velocity, &original_velocity);
    NuVecNorm(&reflected_velocity, &reflected_velocity);
    NuVecScale(&reflected_velocity, &reflected_velocity, speed * 0.8f);
    batarang->velocity.x = SeekLinearF(batarang->velocity.x, reflected_velocity.x, 10.0f);
    batarang->velocity.y = SeekLinearF(batarang->velocity.y, reflected_velocity.y, 10.0f);
    batarang->velocity.z = SeekLinearF(batarang->velocity.z, reflected_velocity.z, 10.0f);
    return 1;
}

void Batarang_Ricochet(BATARANG_s *batarang) {
    if (batarang == NULL || (batarang->ricochet_flags & 1) == 0) {
        return;
    }
    if (batarang->ricochet_timer < 0.2f) {
        batarang->ricochet_timer += FRAMETIME;
    } else {
        batarang->ricochet_timer = 0.0f;
        batarang->ricochet_flags &= ~1u;
        batarang->ricochet_normal = v000;
    }
}

i32 Batarang_SeekToTarget(BATARANG_s *batarang) {
    if (batarang == NULL) {
        return 0;
    }
    NUVEC target;
    if (batarang->current_target > batarang->active || batarang->targets[batarang->current_target].lost != 0) {
        target = batarang->owner->apiobj.collision_position;
    } else if (!Batarang_GetTargetPos(batarang, batarang->current_target, &target)) {
        return 0;
    }

    const f32 rate = batarang->flight_time > 1.0f ? (batarang->flight_time - 1.0f) * 5.0f + 5.0f : 5.0f;
    if (batarang->flight_time < 4.0f) {
        NUVEC travel, destination;
        NuVecScale(&travel, &batarang->velocity, FRAMETIME * 2.0f);
        NuVecAdd(&destination, &batarang->position, &travel);
        if (GameRayCast(&batarang->position, &travel, 0.1f, 0x1f) != 0) {
            i32 platform = TerrainPlatId();
            bool ricochet = batarang->field_0x7d != 0 && batarang->current_target >= batarang->active;
            if (!ricochet) {
                BATARANG_TARGET_s *current = &batarang->targets[batarang->current_target];
                switch (current->type) {
                    case 0:
                    case 1:
                    case 3:
                        ricochet = true;
                        break;
                    case 2:
                        ricochet = platform != static_cast<GIZMOBLOWUP_s *>(current->object)->platform_id;
                        break;
                    default:
                        break;
                }
            }
            if (ricochet) {
                NewRayCastGetImpactNormal(&travel);
                Batarang_InitRicochet(batarang, &travel);
            }
        }
    }
    if ((batarang->ricochet_flags & 1) != 0) {
        Batarang_Ricochet(batarang);
    } else {
        NUVEC desired;
        NuVecSub(&desired, &target, &batarang->position);
        NuVecNorm(&desired, &desired);
        NuVecScale(&desired, &desired, 5.0f);
        batarang->velocity.x = SeekValF(batarang->velocity.x, desired.x, rate);
        batarang->velocity.y = SeekValF(batarang->velocity.y, desired.y, rate);
        batarang->velocity.z = SeekValF(batarang->velocity.z, desired.z, rate);
        batarang->ricochet_timer += FRAMETIME;
        if (batarang->ricochet_timer > 0.2f && batarang->ricochet_count > 0) {
            --batarang->ricochet_count;
            batarang->ricochet_timer = 0.0f;
        }
    }
    batarang->position.x += batarang->velocity.x * FRAMETIME;
    batarang->position.y += batarang->velocity.y * FRAMETIME;
    batarang->position.z += batarang->velocity.z * FRAMETIME;
    return NuVecDistSqr(&batarang->position, &target, NULL) < 0.0625f;
}

void Batarangs_Draw() {
    for (i32 i = 0; i < 8; ++i) {
        BATARANG_s *batarang = &Batarang[i];
        if (batarang->field_0x7d == 0 || WORLD->lev_objs[batarang->cooldown].active == 0) {
            continue;
        }
        NUMTX_ALIGNED16 matrix;
        NuMtxSetTranslation(&matrix, &batarang->position);
        NuSpecialDrawAt(&WORLD->lev_objs[batarang->cooldown].special, &matrix);
    }
}

void Batarangs_CheckLostData(void *data) {
    BATARANG_s *batarang = Batarang;
    BATARANG_s *end = Batarang + 8;
    do {
        if (batarang->owner == data) {
        reset:
            batarang->field_0x7d = 0;
            batarang->active = 0;
            batarang->owner = NULL;
            batarang->cooldown = 0x32;
        } else {
            BATARANG_TARGET_s *target_data = batarang->targets;
            for (i32 target = 0; target < batarang->active; ++target, ++target_data) {
                if (target_data->object != data) {
                    continue;
                }
                if (batarang->owner == data) {
                    goto reset;
                }
                if (batarang->field_0x7d != 0) {
                    target_data->lost = 1;
                    continue;
                }
                for (i32 move = target + 1; move < batarang->active; ++move) {
                    batarang->targets[move - 1] = batarang->targets[move];
                }
                --batarang->active;
            }
        }
        ++batarang;
    } while (batarang != end);
}

i32 Batarang_GetObjectFromCharID(i32 character) {
    return 0x32 + (character == id_ROBIN);
}

i32 Batarang_StartTargetting(GameObject_s *object) {
    GAMECHARACTERDATA *runtime = object->apiobj.character_data->game_character;
    if ((runtime->flags_090 & 0x20000000) == 0) {
        return 0;
    }
    BATARANG_s *batarang = static_cast<BATARANG_s *>(object->batarang);
    batarang->cooldown = Batarang_GetObjectFromCharID(object->id);
    batarang = static_cast<BATARANG_s *>(object->batarang);
    if (WORLD->lev_objs[batarang->cooldown].active == 0) {
        return 0;
    }
    object->character_context = 0x4d;
    object->context_animation = 0x90;
    object->context_animation_timer = 0.0f;
    object->field_0x7a3 = 0;
    object->jump_flags |= 2;
    object->landing_followup = 0;
    batarang->active = 0;
    const f32 sight_x = object->camera_screen_position.x;
    batarang->owner = object;
    batarang = static_cast<BATARANG_s *>(object->batarang);
    batarang->sight_position.x = sight_x;
    batarang->sight_position.y = object->camera_screen_position.y;
    batarang->sight_position.z = 1.0f;
    batarang->sight_velocity.x = batarang->sight_velocity.y = batarang->sight_velocity.z = 0.0f;
    KeepPointOnScreen(&batarang->sight_position, &batarang->sight_velocity);
    return 1;
}

void Batarangs_Update() {
    for (i32 i = 0; i < 8; ++i) {
        BATARANG_s *batarang = &Batarang[i];
        if (batarang->field_0x7d == 0 || batarang->owner == NULL) {
            continue;
        }
        batarang->flight_time += FRAMETIME;
        if (!(batarang->flight_time < 2.0f) && batarang->flight_time > 4.0f)
            batarang->flight_time = 4.0f;
        if (!Batarang_SeekToTarget(batarang)) {
            continue;
        }

        if (batarang->current_target >= batarang->active) {
            GameObject_s *owner = batarang->owner;
            owner->hold_timer = 0.0f;
            NewBuzzFrames(owner->pad_gamepad->pad, 1, 0);
            owner = batarang->owner;
            if ((owner->pad_gamepad->buttons_held & GAMEPAD_ACTION) != 0) {
                Batarang_StartTargetting(owner);
            } else if (owner->pad_gamepad->input_magnitude == 0.0f && owner->character_context == -1 &&
                       owner->apiobj.character_model->model_data_b[0x98] != NULL) {
                owner->character_context = 0x50;
                owner->context_animation = 0x98;
                owner->context_animation_timer = AnimDuration(owner->id, 0x98, 0.0f, 0.0f, 1);
            }
            batarang->field_0x7d = 0;
            batarang->active = 0;
            batarang->owner = NULL;
            batarang->cooldown = 0x32;
            continue;
        }

        BATARANG_TARGET_s &target = batarang->targets[batarang->current_target++];
        batarang->flight_time = 0.0f;
        if (target.lost != 0 || target.object == NULL || target.type == 3) {
            continue;
        }
        NewRumble(batarang->owner->pad_gamepad->pad, 0.5f, 0);
        NewBuzzFrames(batarang->owner->pad_gamepad->pad, 1, 0);
        GameCam_HitJudder();
        if (target.type == 0) {
            GameObject_s *victim = static_cast<GameObject_s *>(target.object);
            if (victim->id == id_CATWOMAN && victim->apiobj.field_0x27c == -1) {
                victim->field_0xef8 |= 1;
                StunGameObject(victim, batarang->owner, catwoman_stun_time, 0);
            } else if ((victim->field_0xefb & 8) == 0) {
                ObjHitObj(batarang->owner, victim, 1, 0, 0, 1);
            }
        } else if (target.type == 1) {
            Detonator_Detonate(static_cast<DETONATOR_s *>(target.object));
        } else if (target.type == 2) {
            GizmoBlowupBlowup(static_cast<GIZMOBLOWUP_s *>(target.object), 1, 2, 2, NULL, 1);
        }
    }
}

void Batarang_GetSightInfo(i32 character, i32 *red, i32 *green, i32 *blue, char *text) {
    *red = 255;
    if (character == id_ROBIN) {
        *green = 0;
        *blue = 31;
        if (text != NULL)
            NuStrCpy(text, "\xc2\xb1");
    } else {
        *green = 223;
        *blue = 0;
        if (text != NULL)
            NuStrCpy(text, "\xc2\xa7");
    }
}

void Batarang_MoveCode(GameObject_s *object) {
    BATARANG_s *batarang;
    if (object->character_context == 0x50) {
        object->field_0xe22 |= 0x40;
        if (object->hold_timer >= 0.25f) {
            Batarang_StartTargetting(object);
            return;
        }
        if (AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 0, 0) == NULL) {
            return;
        }
        if (!(object->pad_gamepad->input_magnitude > 0.0f)) {
            object->context_animation_timer -= FRAMETIME;
            if (0.0f < object->context_animation_timer) {
                return;
            }
            if ((object->pad_gamepad->buttons_held & GAMEPAD_ACTION) != 0) {
                if (Batarang_StartTargetting(object) != 0) {
                    return;
                }
                object->hold_timer = 0.0f;
            }
        }
        object->character_context = -1;
        return;
    }
    if (object->character_context != 0x4d) {
        if (!object->apiobj.player_controlled ||
            (object->apiobj.character_data->game_character->flags_090 & 0x20000000) == 0 || object->batarang == NULL ||
            static_cast<BATARANG_s *>(object->batarang)->field_0x7d != 0 || object->use_model_origin == 0 ||
            (object->field_0xe24 & 8) == 0 || object->apiobj.model_draw_result == 0 || !(object->field_0xc54 > 0.0f) ||
            !(object->hold_timer >= 0.25f) || object->apiobj.field_0x27d == 0 || ObjLandReady(object) == 0) {
            return;
        }
        object->field_0xe22 |= 0x40;
        if (!(0.25f > object->hold_timer - FRAMETIME)) {
            return;
        }
        Batarang_StartTargetting(object);
        return;
    }
    if (object->field_0x7a3 == 0) {
        object->field_0xe22 |= 0x40;
        object->jump_flags |= 2;
        if ((object->pad_gamepad->buttons_pressed & GAMEPAD_JUMP) != 0) {
            StartJump(object, 0);
            return;
        }
        if ((object->pad_gamepad->buttons_held & GAMEPAD_ACTION) != 0 || 0.3f > object->context_animation_timer) {
            if (object->apiobj.character_model->model_data_b[object->context_animation] == NULL ||
                AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0) != NULL) {
                GAMEPAD_s *pad = object->pad_gamepad;
                object->context_animation_timer += FRAMETIME;
                f32 x = pad->input_direction_z * 1.25f;
                f32 y = 1.25f * pad->input_direction_x;
                if (0.25f > object->context_animation_timer) {
                    x = x * object->context_animation_timer * 4.0f;
                    y = y * object->context_animation_timer * 4.0f;
                }
                BATARANG_s *aim = static_cast<BATARANG_s *>(object->batarang);
                aim->sight_velocity.x = SeekValF(aim->sight_velocity.x, x, 10.0f);
                aim = static_cast<BATARANG_s *>(object->batarang);
                aim->sight_velocity.y = SeekValF(aim->sight_velocity.y, y, 10.0f);
                aim = static_cast<BATARANG_s *>(object->batarang);
                aim->sight_position.x = aim->sight_velocity.x * FRAMETIME + aim->sight_position.x;
                aim->sight_position.y = FRAMETIME * aim->sight_velocity.y + aim->sight_position.y;
            }
            batarang = static_cast<BATARANG_s *>(object->batarang);
            KeepPointOnScreen(&batarang->sight_position, &batarang->sight_velocity);
            if (Batarang_FindTarget(WORLD, object, 0) != 0) {
                NewBuzzFrames(object->pad_gamepad->pad, 1, 0);
            }
            i32 colour[3];
            char text[32];
            Batarang_GetSightInfo(object->id, &colour[0], &colour[1], &colour[2], text);
            ADDGAMEMSG message;
            memcpy(&message, &AddGameMsg_Default, sizeof(message));
            message.position = &static_cast<BATARANG_s *>(object->batarang)->sight_position;
            message.flags = 0x1080;
            message.scale = 1.0f;
            message.red = static_cast<u8>(colour[0]);
            message.green = static_cast<u8>(colour[1]);
            message.blue = static_cast<u8>(colour[2]);
            message.alpha = static_cast<i32>((0.2f * game_pulse + 0.8f) * 128.0f);
            message.text = text;
            AddGameMsg(&message);
            message.flags = 0x87;
            message.field_0x4f = 4;
            message.scale = 3.0f;
            message.text = ASCII_DOWN;
            message.alpha = static_cast<i32>(128.0f * (0.8f - 0.2f * game_pulse));
            batarang = static_cast<BATARANG_s *>(object->batarang);
            for (i32 i = 0; i < batarang->active; ++i) {
                NUVEC position;
                if (Batarang_GetTargetPos(batarang, i, &position) != 0) {
                    message.position = &position;
                    AddGameMsg(&message);
                }
                batarang = static_cast<BATARANG_s *>(object->batarang);
            }
        } else if (static_cast<BATARANG_s *>(object->batarang)->active != 0) {
            object->jump_flags &= ~2U;
            object->field_0x7a3 = 1;
            object->context_animation = 0x65;
            f32 duration = AnimDuration(object->id, 0x65, 0.0f, 0.0f, 1);
            object->context_flags &= ~0x40U;
            object->context_animation_timer = 0.0f;
            object->airborne_action_duration = duration <= 0.0f ? 0.5f : duration;
            if (object->apiobj.character_model->model_data_b[object->context_animation] == NULL ||
                AnimListFrame(object->apiobj.character_model, object->context_animation, 0) != 0.0f) {
                object->pad_e3c[0] = 1;
                object->context_flags |= 0x40;
                return;
            }
            object->context_flags &= ~0x40U;
        } else {
            object->character_context = -1;
        }
    } else if (object->field_0x7a3 == 1) {
        object->jump_flags &= ~2U;
        if ((object->pad_gamepad->buttons_pressed & GAMEPAD_JUMP) != 0) {
            object->landing_followup = 1;
        }
        if ((object->context_flags & 0x40) != 0) {
            if (object->landing_followup == 1 && 0.2f <= object->context_animation_timer) {
                StartJump(object, 0);
                object->movement_runtime_flags |= 0x10;
                goto finish;
            }
            if (0.0f < object->pad_gamepad->input_magnitude && 0.3f <= object->context_animation_timer) {
                object->character_context = -1;
                return;
            }
        }
        f32 *frame = NULL;
        if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL) {
            frame = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
            if (frame == NULL) {
                goto finish;
            }
        }
        object->context_animation_timer += FRAMETIME;
        if (object->airborne_action_duration <= object->context_animation_timer) {
            object->character_context = -1;
            if ((object->context_flags & 0x40) != 0) {
                return;
            }
        } else {
            if ((object->context_flags & 0x40) != 0) {
                return;
            }
            if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL) {
                f32 marker = AnimListFrame(object->apiobj.character_model, object->context_animation, 2);
                if (frame == NULL || !(0.0f < *frame) || !(marker <= *frame)) {
                    goto finish;
                }
            }
        }
        object->pad_e3c[0] = 1;
        object->context_flags |= 0x40;
        return;
    }
finish:
    if ((object->context_flags & 0x40) == 0 && object->pad_e3c[0] == 0) {
        object->field_0xe22 |= 0x40;
    }
}
