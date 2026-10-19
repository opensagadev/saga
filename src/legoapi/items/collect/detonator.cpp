#include "decomp.h"
#include "globals.h"
#include "legoapi/ai/game/gameantinode.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nutrig.h"

#include <string.h>
#include "legoapi/items/collect/detonator_types.h"
#include "legoapi/actions/movement/climbing.h"
#include "legoapi/items/collect/batarang.h"
#include "legoapi/items/collect/detonator.h"
#include "legoapi/props/objects/zipup.h"

void Detonators_Draw() {
    if (!WORLD->lev_objs[0xec].active) {
        return;
    }

    for (DETONATOR_s *cursor = Detonator; cursor != Detonator + 10; ++cursor) {
        DETONATOR_s &detonator = *cursor;
        detonator.draw_result = 0;
        if (!detonator.active) {
            continue;
        }

        NUMTX_ALIGNED16 matrix;
        NuMtxSetRotationY(&matrix, detonator.rotation_y);
        NuMtxRotateZ(&matrix, detonator.rotation_z);
        NuMtxRotateX(&matrix, detonator.rotation_x);
        NuMtxTranslate(&matrix, &detonator.position);
        NuSpecialDrawAt(&WORLD->lev_objs[0xec].special, &matrix);

        const bool flicker_on = PickUpFlickerTest <= PickupFlickerFrame % PickUpFlickerFrames;
        if (!flicker_on &&
            (detonator.timer >= 10.0f || (detonator.object != NULL && detonator.object->apiobj.field_0x287 == 0 &&
                                          detonator.object->field_0xde0 >= 0.3f))) {
            if (WORLD->lev_objs[0xef].active)
                detonator.draw_result = NuSpecialDrawAt(&WORLD->lev_objs[0xef].special, &matrix);
        } else {
            if (WORLD->lev_objs[0xee].active)
                detonator.draw_result = NuSpecialDrawAt(&WORLD->lev_objs[0xee].special, &matrix);
        }
    }
}

static inline void Detonator_Update(DETONATOR_s *detonator) {
    if (detonator->active == 0) {
        return;
    }
    detonator->timer += FRAMETIME;
    NuCameraTransformScreenClip(&detonator->field_0x18, &detonator->field_0x0c, 1, NULL);
    if (detonator->timer >= 10.7f) {
        Detonator_Detonate(detonator);
    } else if (detonator->field_0x34 != NULL) {
        DetonatorHitData *hit_data = static_cast<DetonatorHitData *>(detonator->field_0x34);
        if (!(detonator->timer >= 10.0f) &&
            (detonator->object == NULL || detonator->object->apiobj.field_0x287 != 0 ||
             !(detonator->object->field_0xde0 >= 0.3f))) {
            hit_data->field_0x0c = 0.08f;
        } else {
            hit_data->field_0x0c = 0.75f;
        }
    }
}

void Detonators_Update() {
    Detonator_Update(&Detonator[0]);
    Detonator_Update(&Detonator[1]);
    Detonator_Update(&Detonator[2]);
    Detonator_Update(&Detonator[3]);
    Detonator_Update(&Detonator[4]);
    Detonator_Update(&Detonator[5]);
    Detonator_Update(&Detonator[6]);
    Detonator_Update(&Detonator[7]);
    Detonator_Update(&Detonator[8]);
    Detonator_Update(&Detonator[9]);
}

void Detonator_Detonate(DETONATOR_s *detonator) {
    Batarangs_CheckLostData(detonator);
    Detonate(reinterpret_cast<nuvec_s *>(&detonator->field_0x0c), 0);
    DetonatorHitData *hit_data = static_cast<DetonatorHitData *>(detonator->field_0x34);
    detonator->active = 0;
    if (hit_data != NULL) {
        hit_data->field_0x20 = 2.0f;
        detonator->field_0x34 = NULL;
    }
}

static inline void DetonatorConsiderOldest(DETONATOR_s *detonator, f32 &oldest_time, DETONATOR_s *&oldest) {
    if (detonator->active != 0 && detonator->timer > oldest_time) {
        oldest_time = detonator->timer;
        oldest = detonator;
    }
}

static inline void DetonatorConsiderPlacementSlot(i32 index, GameObject_s *object, i32 &count, i32 &available) {
    if (!(Detonator[index].active != 0 && Detonator[index].object == object)) {
        if (available == -1) {
            // Retail also permits replacing a detonator owned by another object.
            available = index;
        }
    } else {
        ++count;
    }
}

static inline i32 DetonatorPlacementSlot(GameObject_s *object) {
    i32 count = 0;
    i32 available = -1;
    DetonatorConsiderPlacementSlot(0, object, count, available);
    DetonatorConsiderPlacementSlot(1, object, count, available);
    DetonatorConsiderPlacementSlot(2, object, count, available);
    if (count == 3)
        return -1;
    DetonatorConsiderPlacementSlot(3, object, count, available);
    if (count == 3)
        return -1;
    DetonatorConsiderPlacementSlot(4, object, count, available);
    if (count == 3)
        return -1;
    DetonatorConsiderPlacementSlot(5, object, count, available);
    if (count == 3)
        return -1;
    DetonatorConsiderPlacementSlot(6, object, count, available);
    if (count == 3)
        return -1;
    DetonatorConsiderPlacementSlot(7, object, count, available);
    if (count == 3)
        return -1;
    DetonatorConsiderPlacementSlot(8, object, count, available);
    if (count == 3)
        return -1;
    DetonatorConsiderPlacementSlot(9, object, count, available);
    if (count == 3)
        return -1;
    return available;
}

void Detonator_MoveCode(GameObject_s *object) {
    f32 oldest_time;
    if (__builtin_expect(object->character_context != 0x48 && object->character_context != 0x49, 1)) {
        if (WORLD->lev_objs[0xec].active == 0 || static_cast<i8>(object->apiobj.flags_low) >= 0 ||
            (object->apiobj.field_0x27d == 0 && object->field_0xe31 != 1) ||
            (ObjLandReady(object) == 0 && (CInfo[object->character_context].flags & 0x2000) == 0)) {
            object->field_0xde0 = 0.0f;
            object->field_0xe21 |= 0x80;
            return;
        }
        if ((object->pad_gamepad->buttons_held & GAMEPAD_SPECIAL) != 0) {
            if (!(object->field_0xde0 > 0.0f) && static_cast<i8>(object->field_0xe21) < 0) {
                return;
            }
            object->field_0xe21 |= 0x80;
            object->field_0xde0 += FRAMETIME;
            if (object->field_0xde0 >= 0.3f && object->field_0xde0 >= 1.0f) {
                oldest_time = -1.0f;
                DETONATOR_s *oldest = NULL;
                if (Detonator[0].active != 0) {
                    oldest = &Detonator[0];
                    oldest_time = Detonator[0].timer;
                    if (!(oldest_time > -1.0f)) {
                        oldest = NULL;
                        oldest_time = -1.0f;
                    }
                }
                DetonatorConsiderOldest(&Detonator[1], oldest_time, oldest);
                DetonatorConsiderOldest(&Detonator[2], oldest_time, oldest);
                DetonatorConsiderOldest(&Detonator[3], oldest_time, oldest);
                DetonatorConsiderOldest(&Detonator[4], oldest_time, oldest);
                DetonatorConsiderOldest(&Detonator[5], oldest_time, oldest);
                DetonatorConsiderOldest(&Detonator[6], oldest_time, oldest);
                DetonatorConsiderOldest(&Detonator[7], oldest_time, oldest);
                DetonatorConsiderOldest(&Detonator[8], oldest_time, oldest);
                DetonatorConsiderOldest(&Detonator[9], oldest_time, oldest);
                if (oldest != NULL) {
                    Detonator_Detonate(oldest);
                    object->field_0xde0 = 0.7f;
                } else {
                    object->field_0xde0 = 0.0f;
                }
            }
            return;
        }
        if (object->field_0xde0 > 0.0f && object->field_0xde0 < 0.3f) {
            DETONATOR_s *nearest =
                Detonator_FindNearest(&object->apiobj.lower_position, 0.0775f + object->apiobj.field_0x1dc, object);
            if (nearest != NULL) {
                object->field_0x7a3 = 0;
                object->character_context = 0x49;
                FastWeaponIn(object, 0);
                object->field_0x788 = nearest;
                object->context_animation = 0x66;
                const u16 angle = NuAtan2D(nearest->position.x - object->apiobj.position.x,
                                           nearest->position.z - object->apiobj.position.z);
                nearest = static_cast<DETONATOR_s *>(object->field_0x788);
                object->apiobj.movement_facing_angle = angle;
                object->context_destination.x = nearest->position.x - NU_SIN_LUT(angle) * 0.1684f;
                object->context_destination.z = nearest->position.z - NU_COS_LUT(angle) * 0.1684f;
                object->carried_object_angle = RotDiff(angle, nearest->rotation_y);
            } else if (__builtin_expect(DetonatorPlacementSlot(object) != -1 && object->apiobj.field_0x218 != 2000000.0f &&
                       object->apiobj.lower_position.y - object->apiobj.field_0x218 < 0.155f, 1)) {
                object->context_destination.x = object->apiobj.lower_position.x;
                object->context_destination.y = object->apiobj.field_0x218;
                object->context_destination.z = object->apiobj.lower_position.z;
                if (__builtin_expect(Detonator_FindNearest(&object->context_destination, 0.155f, NULL) == NULL, 1)) {
                    object->field_0x7a3 = 0;
                    object->character_context = 0x48;
                    FastWeaponIn(object, 0);
                    object->context_animation = 0x8d;
                    object->context_x_rotation = object->field_0x1062;
                    object->carried_object_angle = qrand();
                    object->context_z_rotation = object->field_0x1064;
                } else {
                    GameAudio_PlaySfx(0x32, NULL, 0, 0);
                }
            } else {
                GameAudio_PlaySfx(0x32, NULL, 0, 0);
            }
            if (object->character_context == 0x48 || object->character_context == 0x49) {
                object->apiobj.velocity.z = 0.0f;
                object->apiobj.velocity.x = 0.0f;
                if (AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 1) != NULL) {
                    ResetAnimPacket(&object->apiobj.anim_packet, -1);
                }
                object->context_animation_timer =
                    object->apiobj.character_model->model_data_b[object->context_animation] != NULL
                        ? AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1)
                        : 1.0f;
            }
        }
        object->field_0xde0 = 0.0f;
        object->field_0xe21 &= 0x7f;
        return;
    }

    object->field_0xe21 |= 0x80;
    object->field_0xde0 = 0.0f;
    f32 *frame = NULL;
    if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL) {
        frame = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
        if (frame == NULL) {
            return;
        }
    }
    object->context_animation_timer -= FRAMETIME;
    if (object->context_animation_timer <= 0.0f) {
        object->character_context = -1;
        if (object->field_0x7a3 != 0) {
            return;
        }
        // Expiry falls through to placement, including an unfinished pickup.
        object->field_0x7a3 = 1;
    } else {
        if (object->field_0x7a3 != 0) {
            return;
        }
        if (__builtin_expect(object->apiobj.character_model->model_data_b[object->context_animation] != NULL, 1)) {
            const f32 marker = AnimListFrame(object->apiobj.character_model, object->context_animation,
                                             object->character_context == 0x49 ? 0 : 2);
            if (frame == NULL || !(*frame > 0.0f) || *frame < marker) {
                return;
            }
        } else if (__builtin_expect(object->context_animation_timer >= 0.5f, 1)) {
            return;
        }
        object->field_0x7a3 = 1;
        if (object->character_context == 0x49) {
            DETONATOR_s *detonator = static_cast<DETONATOR_s *>(object->field_0x788);
            if (detonator != NULL) {
                detonator->active = 0;
                detonator = static_cast<DETONATOR_s *>(object->field_0x788);
                if (detonator->field_0x34 != NULL) {
                    GameAntinode_UnregisterAntiNode(WORLD->game_antinode_sys,
                                                    static_cast<GAMEANTINODE_s *>(detonator->field_0x34));
                    static_cast<DETONATOR_s *>(object->field_0x788)->field_0x34 = NULL;
                }
            }
            return;
        }
    }
    const i32 slot = DetonatorPlacementSlot(object);
    if (slot == -1) {
        GameAudio_PlaySfx(0x32, NULL, 0, 0);
        return;
    }
    DETONATOR_s *detonator = &Detonator[slot];
    detonator->position.x = object->context_destination.x;
    detonator->position.y = object->context_destination.y;
    detonator->position.z = object->context_destination.z;
    detonator->active = 1;
    detonator->object = object;
    detonator->rotation_x = object->context_x_rotation;
    detonator->rotation_y = object->carried_object_angle + object->apiobj.field_0x276;
    detonator->rotation_z = object->context_z_rotation;
    detonator->field_0x0c.z = 0.0f;
    detonator->field_0x0c.x = 0.0f;
    detonator->field_0x0c.y = 0.0775f;
    NuVecRotateZ(&detonator->field_0x0c, &detonator->field_0x0c, detonator->rotation_z);
    NuVecRotateX(&detonator->field_0x0c, &detonator->field_0x0c, detonator->rotation_x);
    NuVecAdd(&detonator->field_0x0c, &detonator->field_0x0c, &detonator->position);
    detonator->timer = 0.0f;
    detonator->field_0x34 =
        GameAntinode_RegisterAntiNode(WORLD->game_antinode_sys, &detonator->field_0x0c, 1.0f, 1.0f, 1.0f, 0, 0, 0.0f);
    PlaySfx(const_cast<char *>("imp_thermalDet_attach"), &detonator->position);
}

