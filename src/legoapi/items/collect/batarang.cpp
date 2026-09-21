#include "decomp.h"
#include "legoapi/items/collect/batarang.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/actions/movement/jumping.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nucore/nustring.h"
#include <math.h>

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

static inline bool Batarang_TargetPosition(BATARANG_s *batarang, i32 index, NUVEC *position) {
    if (index < 0 || index >= batarang->active || batarang->targets[index].lost != 0 ||
        batarang->targets[index].object == NULL) {
        return false;
    }
    BATARANG_TARGET_s &target = batarang->targets[index];
    switch (target.type) {
        case 0:
            *position = static_cast<GameObject_s *>(target.object)->apiobj.collision_position;
            return true;
        case 1:
            *position = static_cast<DETONATOR_s *>(target.object)->field_0x0c;
            return true;
        case 2:
            *position = static_cast<GIZMOBLOWUP_s *>(target.object)->mid_position;
            return true;
        case 3:
            *position = *static_cast<NUVEC *>(target.object);
            return true;
        default:
            return false;
    }
}

void Batarangs_Draw() {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if (world == NULL || world->lev_objs == NULL) {
        return;
    }
    for (i32 i = 0; i < 8; ++i) {
        BATARANG_s *batarang = &Batarang[i];
        if (batarang->field_0x7d == 0 || batarang->cooldown >= 0x1000 ||
            world->lev_objs[batarang->cooldown].active == 0) {
            continue;
        }
        NUMTX matrix;
        NuMtxSetTranslation(&matrix, &batarang->position);
        NuSpecialDrawAt(&world->lev_objs[batarang->cooldown].special, &matrix);
    }
}

void Batarangs_Reset() {
    for (i32 i = 0; i < 8; ++i) {
        Batarang[i].field_0x7d = 0;
        Batarang[i].active = 0;
        Batarang[i].owner = NULL;
        Batarang[i].cooldown = 50;
    }
}

void Batarang_Release(GameObject_s *object, i32 mode) {
    BATARANG_s *batarang = static_cast<BATARANG_s *>(object->batarang);
    if (batarang == NULL) {
        return;
    }
    batarang->field_0x7d = 1;
    batarang->owner = object;
    batarang->current_target = 0;
    batarang->flight_time = 0.0f;
    batarang->ricochet_count = 0;
    batarang->ricochet_flags = 0;
    batarang->velocity.x = NU_SIN_LUT(object->apiobj.movement_facing_angle) * 5.0f;
    batarang->velocity.y = object->apiobj.velocity.y * 0.5f;
    batarang->velocity.z = NU_COS_LUT(object->apiobj.movement_facing_angle) * 5.0f;
    batarang->position = object->apiobj.collision_position;
    NewRumble(object->pad_gamepad->pad, 0.5f, 0);

    if (batarang->active == 0 && mode == 2) {
        batarang->targets[0].object = object;
        batarang->targets[0].type = 0;
        batarang->targets[0].lost = 0;
        batarang->active = 1;
    }
}

void Batarangs_Update() {
    for (i32 i = 0; i < 8; ++i) {
        BATARANG_s *batarang = &Batarang[i];
        if (batarang->field_0x7d == 0 || batarang->owner == NULL) {
            continue;
        }
        batarang->flight_time += FRAMETIME;
        if (!Batarang_SeekToTarget(batarang)) {
            continue;
        }

        if (batarang->current_target >= batarang->active) {
            GameObject_s *owner = batarang->owner;
            owner->hold_timer = 0.0f;
            NewBuzzFrames(owner->pad_gamepad->pad, 1, 0);
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

void Batarang_MoveCode(GameObject_s *object) {
    BATARANG_s *batarang = static_cast<BATARANG_s *>(object->batarang);
    if (batarang == NULL || batarang->field_0x7d != 0) {
        return;
    }

    if (object->character_context == 0x50) {
        object->field_0xe22 |= 0x40;
        if (AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 0, 0) == NULL) {
            return;
        }
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer <= 0.0f) {
            if ((object->pad_gamepad->buttons_held & GAMEPAD_ACTION) == 0 || !Batarang_StartTargetting(object)) {
                object->hold_timer = 0.0f;
                object->character_context = -1;
            }
        }
        return;
    }

    if (object->character_context != 0x4d) {
        GAMECHARACTERDATA *runtime = object->apiobj.character_data->game_character;
        if (static_cast<i8>(object->apiobj.flags_low) < 0 && (runtime->flags_090 & 0x20000000) != 0 &&
            object->use_model_origin != 0 && (object->field_0xe24 & 8) != 0 && object->apiobj.model_draw_result != 0 &&
            object->apiobj.field_0x27d != 0 && ObjLandReady(object) != 0 && object->hold_timer >= 0.25f &&
            (object->pad_gamepad->buttons_held & GAMEPAD_ACTION) != 0) {
            object->field_0xe22 |= 0x40;
            Batarang_StartTargetting(object);
        }
        return;
    }

    object->field_0xe22 |= 0x40;
    object->jump_flags |= 2;
    if ((object->pad_gamepad->buttons_pressed & GAMEPAD_JUMP) != 0) {
        StartJump(object, 0);
        return;
    }

    if (object->field_0x7a3 == 0) {
        object->context_animation_timer += FRAMETIME;
        batarang->sight_velocity.x =
            SeekValF(batarang->sight_velocity.x, object->pad_gamepad->input_direction_x * 0.5f, 10.0f);
        batarang->sight_velocity.y =
            SeekValF(batarang->sight_velocity.y, object->pad_gamepad->input_direction_z * 0.5f, 10.0f);
        batarang->sight_position.x += batarang->sight_velocity.x * FRAMETIME;
        batarang->sight_position.y += batarang->sight_velocity.y * FRAMETIME;
        KeepPointOnScreen(&batarang->sight_position, &batarang->sight_velocity);

        if ((object->pad_gamepad->buttons_pressed & GAMEPAD_ACTION) != 0) {
            if (batarang->active == 0) {
                object->character_context = -1;
                return;
            }
            object->field_0x7a3 = 1;
            object->jump_flags &= ~2u;
            object->context_animation = 0x65;
            object->context_animation_timer = 0.0f;
            object->airborne_action_duration = AnimDuration(object->id, 0x65, 0.0f, 0.0f, 1);
            if (object->airborne_action_duration <= 0.0f) {
                object->airborne_action_duration = 0.5f;
            }
            object->pad_e3c[0] = 1;
            object->context_flags |= 0x40;
        }
        return;
    }

    object->jump_flags &= ~2u;
    object->context_animation_timer += FRAMETIME;
    if (object->context_animation_timer >= object->airborne_action_duration) {
        object->character_context = -1;
    }
}

void Batarang_Ricochet(BATARANG_s *batarang) {
    if (batarang == NULL || (batarang->ricochet_flags & 1) == 0) {
        return;
    }
    batarang->ricochet_timer += FRAMETIME;
    if (batarang->ricochet_timer >= 0.2f) {
        batarang->ricochet_timer = 0.0f;
        batarang->ricochet_flags &= ~1u;
        batarang->ricochet_normal = v000;
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
    batarang->ricochet_normal = *normal;
    ++batarang->ricochet_count;
    batarang->ricochet_flags |= 1;
    batarang->ricochet_timer = 0.0f;
    NUVEC unit = *normal;
    NuVecNorm(&unit, &unit);
    const f32 projection = NuVecDot(&batarang->velocity, &unit);
    batarang->velocity.x = (batarang->velocity.x - 2.0f * projection * unit.x) * 0.75f;
    batarang->velocity.y = (batarang->velocity.y - 2.0f * projection * unit.y) * 0.75f;
    batarang->velocity.z = (batarang->velocity.z - 2.0f * projection * unit.z) * 0.75f;
    return 1;
}

i32 Batarang_SeekToTarget(BATARANG_s *batarang) {
    if (batarang == NULL || batarang->owner == NULL) {
        return 0;
    }
    NUVEC target;
    if (batarang->current_target >= batarang->active ||
        !Batarang_TargetPosition(batarang, batarang->current_target, &target)) {
        target = batarang->owner->apiobj.collision_position;
    }

    NUVEC travel;
    NuVecScale(&travel, &batarang->velocity, FRAMETIME * 2.0f);
    if (GameRayCast(&batarang->position, &travel, 0.1f, 0x1f) != 0) {
        NUVEC normal;
        NewRayCastGetImpactNormal(&normal);
        Batarang_InitRicochet(batarang, &normal);
    }
    if ((batarang->ricochet_flags & 1) != 0) {
        Batarang_Ricochet(batarang);
    } else {
        NUVEC desired;
        NuVecSub(&desired, &target, &batarang->position);
        NuVecNorm(&desired, &desired);
        NuVecScale(&desired, &desired, 5.0f);
        const f32 rate = batarang->flight_time > 1.0f ? batarang->flight_time * 2.0f : 2.0f;
        batarang->velocity.x = SeekValF(batarang->velocity.x, desired.x, rate);
        batarang->velocity.y = SeekValF(batarang->velocity.y, desired.y, rate);
        batarang->velocity.z = SeekValF(batarang->velocity.z, desired.z, rate);
    }
    batarang->position.x += batarang->velocity.x * FRAMETIME;
    batarang->position.y += batarang->velocity.y * FRAMETIME;
    batarang->position.z += batarang->velocity.z * FRAMETIME;
    return NuVecDistSqr(&batarang->position, &target, NULL) < 0.25f;
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

i32 Batarang_StartTargetting(GameObject_s *object) {
    GAMECHARACTERDATA *runtime = object->apiobj.character_data->game_character;
    BATARANG_s *batarang = static_cast<BATARANG_s *>(object->batarang);
    if (batarang == NULL || (runtime->flags_090 & 0x20000000) == 0) {
        return 0;
    }
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    const i32 object_id = Batarang_GetObjectFromCharID(object->id);
    if (world == NULL || world->lev_objs == NULL || world->lev_objs[object_id].active == 0) {
        return 0;
    }
    batarang->cooldown = object_id;
    object->character_context = 0x4d;
    object->context_animation = 0x90;
    object->context_animation_timer = 0.0f;
    object->field_0x7a3 = 0;
    object->jump_flags |= 2;
    object->landing_followup = 0;
    batarang->active = 0;
    batarang->owner = object;
    batarang->sight_position = object->camera_screen_position;
    batarang->sight_position.z = 1.0f;
    batarang->sight_velocity = v000;
    KeepPointOnScreen(&batarang->sight_position, &batarang->sight_velocity);

    GameObject_s *nearest = NULL;
    f32 best = 1000000000.0f;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
        GameObject_s *candidate = &Obj[i];
        if (candidate == object || (candidate->apiobj.field_0x1f8 & 0x1001) != 0x1001 ||
            candidate->apiobj.field_0x287 != 0) {
            continue;
        }
        const f32 distance =
            NuVecDistSqr(&object->apiobj.collision_position, &candidate->apiobj.collision_position, NULL);
        if (distance < best && distance < 225.0f) {
            best = distance;
            nearest = candidate;
        }
    }
    if (nearest != NULL) {
        batarang->targets[0].object = nearest;
        batarang->targets[0].type = 0;
        batarang->targets[0].lost = 0;
        batarang->active = 1;
    }
    return 1;
}

void Batarang_StartThrowQuick(GameObject_s *object) {
    ResetAnimPacket(&object->apiobj.anim_packet, -1);
    object->character_context = 0x5e;
    object->context_animation = 0xb2;
    object->context_animation_timer = AnimDuration(object->id, 0xb2, 0.0f, 0.0f, 1);
}

i32 Batarang_GetObjectFromCharID(i32 character) {
    return 0x32 + (character == id_ROBIN);
}

i32 GetShootDirection_Batman(GameObject_s *object, nuvec_s *direction) {
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
        GAMECHARACTERDATA *runtime = object->apiobj.character_data->game_character;
        if ((object->apiobj.character_data->model_flags & 0x2000) == 0 && (runtime->flags_090 & 0x80000000) == 0) {
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
