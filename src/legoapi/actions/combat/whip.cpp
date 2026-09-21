#include "decomp.h"
#include "globals.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/items/base/animpacket.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void SetWeaponIn(GameObject_s *object);

void Whip_Release(GameObject_s *object) {
    if (LEGOCONTEXT_WHIP != -1 && object->character_context == LEGOCONTEXT_WHIP && object->field_0x788 != NULL) {
        object->carried_object_basis[0] = v100;
        object->carried_object_basis[1] = v010;
        object->carried_object_basis[2] = v001;
        SuperCarry_Throw(object, 1);
    }
}

void Whip_MoveCode(GameObject_s *object) {
    if (LEGOCONTEXT_WHIP == -1) {
        return;
    }

    if (object->character_context != LEGOCONTEXT_WHIP) {
        if ((object->pad_gamepad->buttons_pressed & GAMEPAD_SPECIAL) == 0 || object->apiobj.field_0x27d == 0 ||
            ObjLandReady(object) == 0 || LEGOACT_WHIP_START == -1 ||
            object->apiobj.character_model->model_data_b[LEGOACT_WHIP_START] == NULL) {
            return;
        }

        object->field_0x7aa = 0xff;
        GIZMOBLOWUP_s *target = NULL;
        u8 action = 1;
        if (LEGOACT_WHIP_GRAB != -1 && object->apiobj.character_model->model_data_b[LEGOACT_WHIP_GRAB] != NULL &&
            SuperCarry_Possible(object, 0) != 0) {
            target = GizmoBlowUpOpponent(object, 2.0f, 0.75f, 0.25f, 5, 0, 0, 1);
            if (target != NULL) {
                action = 3;
            }
        }
        if (target == NULL && LEGOACT_WHIP_BREAK != -1 &&
            object->apiobj.character_model->model_data_b[LEGOACT_WHIP_BREAK] != NULL) {
            target = GizmoBlowUpOpponent(object, 2.0f, 0.75f, 0.25f, 5, 0xc080, 0, 0);
            if (target != NULL) {
                action = 2;
            }
        }

        object->field_0x7aa = action;
        object->field_0x7a3 = 0;
        object->context_animation_timer = 0.0f;
        object->character_context = LEGOCONTEXT_WHIP;
        object->context_animation = LEGOACT_WHIP_START;
        object->context_variant_flags &= ~1;
        object->airborne_action_duration = AnimDuration(object->id, LEGOACT_WHIP_START, 0.0f, 0.0f, 0);
        GameAudio_PlaySfx(76, &object->apiobj.collision_position, GameAudio_GetPlrSfxBits(object), 0);
        SetWeaponIn(object);
        object->field_0x788 = target;
        object->force_target = NULL;
        if (target != NULL) {
            object->apiobj.movement_facing_angle = NuAtan2D(target->position.x - object->apiobj.position.x,
                                                            target->position.z - object->apiobj.position.z);
        }
        return;
    }

    if (object->field_0x7a3 == 0) {
        if (AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0) == NULL) {
            return;
        }

        object->context_animation_timer += FRAMETIME;
        if (object->context_animation_timer < object->airborne_action_duration) {
            return;
        }

        GIZMOBLOWUP_s *target = static_cast<GIZMOBLOWUP_s *>(object->field_0x788);
        i16 animation;
        if (object->field_0x7aa == 2 && target != NULL && (target->status_flags & 0x804000) == 0x804000) {
            object->field_0x7a3 = 2;
            animation = LEGOACT_WHIP_BREAK;
        } else if (object->field_0x7aa == 3 && target != NULL && (target->status_flags & 0x804000) == 0x804000) {
            object->field_0x7a3 = 3;
            animation = LEGOACT_WHIP_GRAB;
        } else {
            object->field_0x7aa = 1;
            object->field_0x7a3 = 1;
            animation = LEGOACT_WHIP_CRACK;
        }

        object->context_animation_timer = 0.0f;
        object->context_animation = animation;
        object->airborne_action_duration = AnimDuration(object->id, animation, 0.0f, 0.0f, 1);
        object->context_flags &= ~0x40;
        if (object->field_0x7a3 != 1) {
            object->apiobj.movement_facing_angle = NuAtan2D(target->position.x - object->apiobj.position.x,
                                                            target->position.z - object->apiobj.position.z);
        }
        return;
    }

    if ((object->context_variant_flags & 1) != 0) {
        NUVEC *target_position = NULL;
        if (object->field_0x788 != NULL) {
            target_position = &static_cast<GIZMOBLOWUP_s *>(object->field_0x788)->position;
        } else if (object->force_target != NULL) {
            target_position = &object->force_target->apiobj.collision_position;
        }
        if (target_position != NULL) {
            object->apiobj.movement_facing_angle = NuAtan2D(target_position->x - object->apiobj.position.x,
                                                            target_position->z - object->apiobj.position.z);
        }
    }

    f32 *frame = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
    if (frame != NULL) {
        bool activate = false;
        object->context_animation_timer += FRAMETIME;
        if ((object->context_flags & 0x40) == 0) {
            f32 action_frame = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
            if (action_frame >= 1.0f && *frame >= action_frame) {
                object->context_flags |= 0x40;
                activate = true;
            }
        }
        if (object->context_animation_timer >= object->airborne_action_duration) {
            object->character_context = -1;
            if ((object->context_flags & 0x40) == 0) {
                activate = true;
            }
        }

        if (activate) {
            GIZMOBLOWUP_s *target = static_cast<GIZMOBLOWUP_s *>(object->field_0x788);
            if (object->field_0x7a3 == 2) {
                if (GizmoBlowupBlowup(target, 1, 1, 1, NULL, 1) != 0) {
                    NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
                }
                NewRumble(object->pad_gamepad->pad, 0.5f, 0);
                GameAudio_PlaySfx(77, &object->apiobj.collision_position, GameAudio_GetPlrSfxBits(object), 0);
            } else if (object->field_0x7a3 == 3) {
                GizmoBlowupBlowup(target, 0, 7, -1, NULL, 1);
                GameAudio_PlaySfx(77, &object->apiobj.collision_position, GameAudio_GetPlrSfxBits(object), 0);
            } else {
                GameAudio_PlaySfx(78, &object->apiobj.collision_position, GameAudio_GetPlrSfxBits(object), 0);
            }
        }
    }

    if (object->character_context == -1 && object->field_0x7a3 == 3 && object->field_0x788 != NULL) {
        SuperCarry_Start(object, static_cast<GIZMOBLOWUP_s *>(object->field_0x788), 1);
    }
}
