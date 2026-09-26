#include "decomp.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "gameapi/edtools/edfile.h"
#include "globals.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numusic/sfx.h"

#include <float.h>
#include <string.h>

void EvalAnim(nuhspecial_s *special, f32 frame, numtx_s *matrix, i32 include_instance_translation);
i32 UseFallAnim(GameObject_s *object);
i32 GetDefaultIdle(GameObject_s *object);
i32 SetProtocolDroidFallAnim(GameObject_s *object);
// TODO: Restore target-local linkage once the four remaining animation-mode
// callers are decompiled; their references naturally prevent inlining.
static void MoveAnim_Manage(GameObject_s *object, f32 movement_speed, i32 allow_tiptoe, i32 weapon_variant);
static void MoveAnim_Check(GameObject_s *object);
static void JumpAnimCode(GameObject_s *object);
static bool JumpAnim_HasAction(const GameObject_s *object, i16 action);
static GAMECHARACTERDATA *GetGameCharacterData(GameObject_s *object);
void UpdateCharacterIdle(GameObject_s *object);
void AutoWeaponOnOff(GameObject_s *object);
void AddFootSteps(GameObject_s *object);
extern "C" void PlaySfxByIdAndSetVolume(i32 sfx_id, NUVEC *position, f32 volume);
i32 MatrixReflection(NUMTX *matrix, i32 axis, f32 plane, f32 height, NUMTX *result);

extern i16 id_BODYGUARD;
extern i16 id_IMPERIALGUARD;
extern i16 id_YODA;
extern i16 id_YODAGHOST;
extern i16 id_GONKDROID;
extern i16 id_JUMBOHOMINGDROID;

enum CHARACTER_ANIMATION : i16 {
    CHARACTER_ANIMATION_WALK = 0,
    CHARACTER_ANIMATION_IDLE = 1,
    CHARACTER_ANIMATION_RUN = 3,
    CHARACTER_ANIMATION_TIPTOE = 4,
    CHARACTER_ANIMATION_FALL = 5,
    CHARACTER_ANIMATION_WEAPON_IDLE = 11,
    CHARACTER_ANIMATION_ALT_IDLE = 25,
    CHARACTER_ANIMATION_SABER_RUN = 23,
    CHARACTER_ANIMATION_ALT_WEAPON_IDLE = 39,
    CHARACTER_ANIMATION_FALL_VARIANT_40 = 40,
    CHARACTER_ANIMATION_SABER_TIPTOE = 63,
    CHARACTER_ANIMATION_SABER_WALK = 64,
    CHARACTER_ANIMATION_FALL_VARIANT_75 = 75,
    CHARACTER_ANIMATION_FALL_VARIANT_76 = 76,
    CHARACTER_ANIMATION_BACKWARDS = 80,
    CHARACTER_ANIMATION_EXTRA_TIPTOE = 113,
    CHARACTER_ANIMATION_EXTRA_WALK = 114,
    CHARACTER_ANIMATION_EXTRA_RUN = 115,
    CHARACTER_ANIMATION_EXTRA_FALL = 116,
    CHARACTER_ANIMATION_EXTRA_IDLE = 117,
    CHARACTER_ANIMATION_SUIT_TIPTOE = 198,
    CHARACTER_ANIMATION_SUIT_WALK = 199,
    CHARACTER_ANIMATION_SUIT_RUN = 200,
};

static void MoveAnim_Check(GameObject_s *object) {
    if (GetAnimBlendMode() == 1) {
        return;
    }

    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    i16 requested = packet.requested_animation;
    const i16 previous = packet.previous_animation;

    if (object->released_movement_animation != -1) {
        object->movement_animation_hold_timer = 0.1f;
        object->held_movement_animation = -1;
    } else {
        if (requested == previous) {
            object->movement_animation_hold_timer = 0.1f;
            object->held_movement_animation = -1;
            object->movement_animation_release_timer = 0.1f;
            object->released_movement_animation = -1;
            return;
        }

        const u32 requested_flags = ActionInfo[requested].flags;
        if (((requested_flags & 7) == 0 && requested != CHARACTER_ANIMATION_ALT_IDLE &&
             requested != CHARACTER_ANIMATION_IDLE) ||
            (ActionInfo[previous].flags & 7) == 0 || (requested_flags & 4) != 0) {
            object->movement_animation_hold_timer = 0.1f;
            object->held_movement_animation = -1;
            object->movement_animation_release_timer = 0.1f;
            object->released_movement_animation = -1;
            return;
        }

        if (object->held_movement_animation != -1) {
            object->movement_animation_hold_timer -= FRAMETIME;
            if (object->movement_animation_hold_timer > 0.0f) {
                packet.requested_animation = object->held_movement_animation;
                object->movement_animation_release_timer = 0.1f;
                object->released_movement_animation = -1;
                return;
            }
            object->held_movement_animation = -1;
        } else {
            object->movement_animation_hold_timer = 0.1f;
            if (packet.blending == 0) {
                bool retain_previous = false;
                if (previous == CHARACTER_ANIMATION_RUN) {
                    retain_previous = requested == CHARACTER_ANIMATION_WALK ||
                                      requested == CHARACTER_ANIMATION_TIPTOE || requested == CHARACTER_ANIMATION_IDLE;
                } else if (previous == CHARACTER_ANIMATION_SABER_RUN) {
                    const GAMECHARACTERDATA *game_character =
                        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                    const i16 alternate_idle =
                        game_character->field275_0x116 != 0
                            ? CHARACTER_ANIMATION_ALT_IDLE
                            : static_cast<i16>((object->apiobj.character_data->model_flags & 0x80) != 0 ? 118 : 25);
                    retain_previous = requested == CHARACTER_ANIMATION_SABER_TIPTOE ||
                                      requested == CHARACTER_ANIMATION_SABER_WALK || requested == alternate_idle;
                }

                if (retain_previous) {
                    packet.requested_animation = previous;
                    object->held_movement_animation = previous;
                    object->movement_animation_release_timer = 0.1f;
                    object->released_movement_animation = -1;
                    return;
                }
            }

            object->movement_animation_release_timer = 0.1f;
            object->released_movement_animation = -1;
            return;
        }
    }

    requested = packet.requested_animation;
    const u32 requested_flags = ActionInfo[requested].flags;
    if (requested == previous || (requested_flags & 7) == 0 ||
        (previous != CHARACTER_ANIMATION_ALT_IDLE && previous != CHARACTER_ANIMATION_IDLE) ||
        (requested_flags & 4) != 0) {
        object->movement_animation_release_timer = 0.1f;
        object->released_movement_animation = -1;
        return;
    }

    if (object->released_movement_animation != -1) {
        object->movement_animation_release_timer -= FRAMETIME;
        if (object->movement_animation_release_timer <= 0.0f) {
            object->movement_animation_release_timer = -1.0f;
        } else {
            packet.requested_animation = object->released_movement_animation;
        }
        return;
    }

    object->movement_animation_hold_timer = 0.1f;
    if (packet.blending == 0 && (requested_flags & 3) != 0) {
        packet.requested_animation = previous;
        object->released_movement_animation = previous;
    }
}

static void JumpAnimCode(GameObject_s *object) {
    if (object->context_variant_flags >= 0) {
        ANIMPACKET_s *packet = &object->apiobj.anim_packet;
        packet->requested_animation = object->context_animation;
        const u8 state = object->action_movement_state;
        if (object->context_animation != 0x49 && (state == 6 || state < 2 || state == 7 || state == 9)) {
            if (packet->blending == 0 && object->context_animation == packet->animation_index &&
                (packet->flags & 1) != 0) {
                object->airborne_input_timer += FRAMETIME;
                if (object->airborne_input_timer >= 0.1f &&
                    (object->nearby_floor_distance == 2000000.0f || object->nearby_floor_distance > 0.35f)) {
                    object->context_variant_flags |= 0x80;
                }
            } else {
                object->airborne_input_timer = 0.0f;
            }
        }
        return;
    }

    void **animations = object->apiobj.character_model->model_data_b;
    if (object->action_movement_state == PLAYER_JUMP_MOVEMENT_COMBAT_ROLL &&
        animations[PLAYER_JUMP_ACTION_COMBAT_ROLL_FALL] != NULL) {
        object->apiobj.anim_packet.requested_animation = PLAYER_JUMP_ACTION_COMBAT_ROLL_FALL;
        return;
    }
    if (animations[PLAYER_JUMP_ACTION_FALL] != NULL) {
        object->apiobj.anim_packet.requested_animation = PLAYER_JUMP_ACTION_FALL;
        return;
    }
    object->apiobj.anim_packet.requested_animation = object->context_animation;
}

static CHARACTERANIM_s *GetAnimationInfo(const CHARACTERMODEL_s *model, i32 animation) {
    if (model == NULL || animation < 0 || model->model_data_a == NULL) {
        return NULL;
    }
    return static_cast<CHARACTERANIM_s *>(model->model_data_a[animation]);
}

static GAMECHARACTERDATA *GetGameCharacterData(GameObject_s *object) {
    if (object == NULL || object->apiobj.character_data == NULL) {
        return NULL;
    }
    return static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
}

static void StartAnimation(CHARACTERMODEL_s *model, ANIMPACKET_s *packet, i16 animation, bool backwards) {
    packet->animation_index = animation;
    CHARACTERANIM_s *info = GetAnimationInfo(model, animation);
    const bool reverse =
        backwards && info != NULL && (info->flags & CHARACTER_ANIMATION_FLAG_REVERSE_WITH_MOVEMENT) != 0;
    packet->current_reversed = reverse ? 1 : 0;
    packet->current_time = reverse ? NuAnimEndFrame(model->model_data_b[animation]) : 1.0f;
    packet->previous_time = packet->current_time;
    packet->blending = 0;
}

void Animate_POD(GameObject_s *) {
    STUBBED();
}

void Animate_ATAT(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        packet.requested_animation = object->context_animation;
    } else {
        packet.requested_animation = CHARACTER_ANIMATION_IDLE;
        if (static_cast<i16>(object->apiobj.field_0x1f8) < 0 &&
            object->apiobj.character_model->model_data_b[15] != NULL) {
            packet.requested_animation = 15;
        }
        if ((object->pad_gamepad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 &&
            object->pad_gamepad->input_magnitude > 0.0f) {
            packet.requested_animation = CHARACTER_ANIMATION_WALK;
        }
    }

    if (packet.requested_animation != CHARACTER_ANIMATION_IDLE) {
        return;
    }

    const i32 turn = RotDiff(object->previous_movement_angle, object->apiobj.field_0x276);
    if (turn > 0 && object->apiobj.character_model->model_data_b[79] != NULL) {
        packet.requested_animation = 79;
    } else if (turn < 0 && object->apiobj.character_model->model_data_b[38] != NULL) {
        packet.requested_animation = 38;
    }
}

void Animate_JEDI(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    bool check_movement_animation = false;

    if ((object->field_0xe23 & GAMEOBJECT_E23_FLAG_FORCE_WEAPON_IDLE) != 0) {
        if (((object->field_0xe22 & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) != 0 || object->field_0xe32 == 1) &&
            object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_ALT_WEAPON_IDLE] != NULL) {
            packet.requested_animation = CHARACTER_ANIMATION_ALT_WEAPON_IDLE;
        } else {
            packet.requested_animation = CHARACTER_ANIMATION_WEAPON_IDLE;
        }
    } else if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        packet.requested_animation = object->context_animation;
    } else {
        packet.requested_animation = CHARACTER_ANIMATION_FALL;

        if (object->field_0x7a5 != CHARACTER_CONTEXT_DOOMED) {
            bool use_default_idle = object->apiobj.field_0x27d != 0;
            if (!use_default_idle) {
                const bool has_fall_animation =
                    object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] != NULL;
                // Original 0x16c978/0x16c9ce joins the character-data check
                // at 0x16c7bc even when the ground-contact timer has expired.
                if (object->ground_contact_grace_timer > 0.0f || !has_fall_animation ||
                    (object->fall_animation_timer < 0.2f && object->nearby_floor_distance != 2000000.0f &&
                     object->nearby_floor_distance < 0.25f && object->apiobj.velocity.y < 0.0f)) {
                    const GAMECHARACTERDATA *game_character =
                        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                    use_default_idle = !(game_character->field_0x28 > 0.0f) || !has_fall_animation;
                }
            }
            if (use_default_idle) {
                packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
            }
        }

        if (object->field_0x7a5 == CHARACTER_CONTEXT_JUMP) {
            JumpAnimCode(object);
        } else if (UseFallAnim(object)) {
            packet.requested_animation = CHARACTER_ANIMATION_FALL;
        } else if (object->field_0x7a5 == CHARACTER_CONTEXT_FORCE_PUSH) {
            if ((object->action_flags & GAMEOBJECT_ACTION_FLAG_FORCE_PUSH_WEAPON_IDLE_MASK) != 0) {
                if (((object->field_0xe22 & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) != 0 || object->field_0xe32 == 1) &&
                    object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_ALT_WEAPON_IDLE] != NULL) {
                    packet.requested_animation = CHARACTER_ANIMATION_ALT_WEAPON_IDLE;
                } else {
                    packet.requested_animation = CHARACTER_ANIMATION_WEAPON_IDLE;
                }
            } else {
                packet.requested_animation =
                    object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_ALT_WEAPON_IDLE] != NULL
                        ? CHARACTER_ANIMATION_ALT_WEAPON_IDLE
                        : CHARACTER_ANIMATION_WEAPON_IDLE;
            }
        } else if (object->field_0x7a5 == CHARACTER_CONTEXT_FORCE_DEFLECT ||
                   object->field_0x7a5 == CHARACTER_CONTEXT_FORCE_THROW ||
                   object->field_0x7a5 == CHARACTER_CONTEXT_FORCE) {
            if (((object->field_0xe22 & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) != 0 || object->field_0xe32 == 1) &&
                object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_ALT_WEAPON_IDLE] != NULL) {
                packet.requested_animation = CHARACTER_ANIMATION_ALT_WEAPON_IDLE;
            } else {
                packet.requested_animation = CHARACTER_ANIMATION_WEAPON_IDLE;
            }
        } else if (packet.requested_animation != CHARACTER_ANIMATION_FALL) {
            GAMEPAD_s *pad = object->pad_gamepad;
            if ((pad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 && pad->input_magnitude > 0.0f) {
                if (object->id == id_YODA || object->id == id_YODAGHOST) {
                    packet.requested_animation = CHARACTER_ANIMATION_SABER_TIPTOE;
                } else {
                    const GAMECHARACTERDATA *game_character =
                        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                    const i32 allow_tiptoe =
                        (game_character->flags_090 & GAMECHARACTER_FLAG_DISABLE_TIPTOE) == 0 ? 1 : 0;
                    MoveAnim_Manage(object, pad->input_magnitude, allow_tiptoe, 1);
                }
            } else if (((object->field_0xe22 & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) != 0 ||
                        object->field_0xe32 == 1) &&
                       object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_ALT_IDLE] != NULL) {
                packet.requested_animation = CHARACTER_ANIMATION_ALT_IDLE;
            }
        }

        if (UsingExtraActionsFn != NULL && UsingExtraActionsFn(object) != 0 &&
            packet.requested_animation <= CHARACTER_ANIMATION_FALL) {
            switch (packet.requested_animation) {
                case CHARACTER_ANIMATION_WALK:
                    packet.requested_animation = CHARACTER_ANIMATION_EXTRA_WALK;
                    break;
                case CHARACTER_ANIMATION_IDLE:
                    packet.requested_animation = CHARACTER_ANIMATION_EXTRA_IDLE;
                    break;
                case 2:
                    break;
                case CHARACTER_ANIMATION_RUN:
                    packet.requested_animation = CHARACTER_ANIMATION_EXTRA_RUN;
                    break;
                case CHARACTER_ANIMATION_TIPTOE:
                    packet.requested_animation = CHARACTER_ANIMATION_EXTRA_TIPTOE;
                    break;
                case CHARACTER_ANIMATION_FALL:
                    packet.requested_animation = CHARACTER_ANIMATION_EXTRA_FALL;
                    break;
            }
        }
        check_movement_animation = true;
    }

    if (check_movement_animation) {
        MoveAnim_Check(object);
    }
    UpdateCharacterIdle(object);

    const i16 animation = packet.requested_animation;
    if (animation == CHARACTER_ANIMATION_FALL ||
        ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_HIGH_JUMP) != 0 &&
         (animation == CHARACTER_ANIMATION_FALL_VARIANT_75 || animation == CHARACTER_ANIMATION_FALL_VARIANT_40 ||
          animation == CHARACTER_ANIMATION_FALL_VARIANT_76))) {
        object->fall_animation_timer += FRAMETIME;
    } else {
        object->fall_animation_timer = 0.0f;
    }

    if (object->id == id_IMPERIALGUARD || object->weapon_scale <= 0.0f) {
        return;
    }

    const char *loop_sfx;
    if (object->id == id_BODYGUARD) {
        loop_sfx = "Grv_GuardWeaponLp";
    } else if ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_JEDI_BADDIE) != 0) {
        loop_sfx = "SaberLoopB";
    } else {
        loop_sfx = "SaberLoopJ";
    }
    PlaySfxByIdAndSetVolume(GetSfxId(loop_sfx), &object->apiobj.collision_position, object->weapon_scale);
}

static void MoveAnim_Manage(GameObject_s *object, f32 movement_speed, i32 allow_tiptoe, i32 weapon_variant) {
    GAMECHARACTERDATA *game_character = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    CHARACTERMODEL_s *model = object->apiobj.character_model;

    const f32 walk_run_threshold = (game_character->walk_speed + game_character->run_speed) * 0.5f;
    const bool use_weapon_locomotion =
        weapon_variant != 0 && (object->weapon_scale == 0.0f || object->weapon_scale_state == WEAPON_SCALE_EXTENDING);

    CHARACTER_ANIMATION animation;
    if (allow_tiptoe != 0 && movement_speed <= (game_character->tiptoe_speed + game_character->walk_speed) * 0.5f) {
        animation = use_weapon_locomotion && model->model_data_b[CHARACTER_ANIMATION_SABER_TIPTOE] != NULL
                        ? CHARACTER_ANIMATION_SABER_TIPTOE
                        : CHARACTER_ANIMATION_TIPTOE;
    } else if (movement_speed <= walk_run_threshold) {
        if (use_weapon_locomotion && model->model_data_b[CHARACTER_ANIMATION_SABER_WALK] != NULL) {
            animation = CHARACTER_ANIMATION_SABER_WALK;
        } else if (model->model_data_b[CHARACTER_ANIMATION_BACKWARDS] != NULL &&
                   (object->field_0xefd & GAMEOBJECT_MOVEMENT_FLAG_BACKWARDS) != 0) {
            animation = CHARACTER_ANIMATION_BACKWARDS;
        } else {
            animation = CHARACTER_ANIMATION_WALK;
        }
    } else {
        animation = use_weapon_locomotion && model->model_data_b[CHARACTER_ANIMATION_SABER_RUN] != NULL
                        ? CHARACTER_ANIMATION_SABER_RUN
                        : CHARACTER_ANIMATION_RUN;
    }
    object->apiobj.anim_packet.requested_animation = animation;

    const SUIT_s *suit = static_cast<const SUIT_s *>(object->suit);
    if (suit != NULL && (suit->store_flag & SUIT_STORE_FLAG_EXTRA_MOVEMENT_ANIMATIONS) != 0 &&
        (object->movement_context_state & 0x00ffff00) != 0x00054300) {
        if (animation == CHARACTER_ANIMATION_TIPTOE && model->model_data_b[CHARACTER_ANIMATION_SUIT_TIPTOE] != NULL) {
            animation = CHARACTER_ANIMATION_SUIT_TIPTOE;
        } else if (animation == CHARACTER_ANIMATION_WALK &&
                   model->model_data_b[CHARACTER_ANIMATION_SUIT_WALK] != NULL) {
            animation = CHARACTER_ANIMATION_SUIT_WALK;
        } else if (animation == CHARACTER_ANIMATION_RUN && model->model_data_b[CHARACTER_ANIMATION_SUIT_RUN] != NULL) {
            animation = CHARACTER_ANIMATION_SUIT_RUN;
        }
        object->apiobj.anim_packet.requested_animation = animation;
    }

    // The target applies this bounded fallback exactly three times. Keeping
    // the passes explicit preserves its finite walk/run alternation when a
    // model supplies none of the ordinary locomotion clips.
    if (model->model_data_b[animation] == NULL) {
        if (animation == CHARACTER_ANIMATION_TIPTOE) {
            animation = CHARACTER_ANIMATION_WALK;
        } else if (animation == CHARACTER_ANIMATION_WALK) {
            animation = CHARACTER_ANIMATION_RUN;
        } else if (animation == CHARACTER_ANIMATION_RUN) {
            animation = CHARACTER_ANIMATION_WALK;
        } else {
            return;
        }
        object->apiobj.anim_packet.requested_animation = animation;
    }
    if (model->model_data_b[animation] == NULL) {
        if (animation == CHARACTER_ANIMATION_TIPTOE) {
            animation = CHARACTER_ANIMATION_WALK;
        } else if (animation == CHARACTER_ANIMATION_WALK) {
            animation = CHARACTER_ANIMATION_RUN;
        } else if (animation == CHARACTER_ANIMATION_RUN) {
            animation = CHARACTER_ANIMATION_WALK;
        } else {
            return;
        }
        object->apiobj.anim_packet.requested_animation = animation;
    }
    if (model->model_data_b[animation] == NULL) {
        if (animation == CHARACTER_ANIMATION_TIPTOE) {
            animation = CHARACTER_ANIMATION_WALK;
        } else if (animation == CHARACTER_ANIMATION_WALK) {
            animation = CHARACTER_ANIMATION_RUN;
        } else if (animation == CHARACTER_ANIMATION_RUN) {
            animation = CHARACTER_ANIMATION_WALK;
        } else {
            return;
        }
        object->apiobj.anim_packet.requested_animation = animation;
    }
}

void AnimatePlayer(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    packet.previous_animation = packet.animation_index;
    object->mini_animation.previous_animation = object->mini_animation.animation_index;
    GAMEPAD_s *pad = object->pad_gamepad;
    CHARACTERMODEL_s *model = object->apiobj.character_model;

    if ((object->apiobj.field_0x1f4 & APIOBJECT_STATE_FLAG_IGNORE_DOORS) == 0) {
        object->apiobj.character_data->animate_fn(object);
    }

    const i16 override_from = object->ai.animation_override_from;
    if (override_from != -1 &&
        ((override_from == 0xe9 && object->character_context != 0x1c) || override_from == packet.requested_animation)) {
        packet.requested_animation = object->ai.animation_override_to;
    }

    if (model == NULL ||
        (object->apiobj.field_0x287 != 0 && (object->field_0x1018 == 0.0f || object->apiobj.field_0x287 == 1))) {
        return;
    }

    const f32 direction = (object->field_0xefd & GAMEOBJECT_MOVEMENT_FLAG_BACKWARDS) != 0 ? -1.0f : 1.0f;
    f32 movement_speed;
    if (object->apiobj.character_model->model_data_b[1] != NULL) {
        movement_speed = pad->animation_input_magnitude;
        if (movement_speed > 0.0f && object->id == id_GONKDROID && Cheat_IsOn(8) == 0)
            movement_speed = object->apiobj.character_data->game_character->walk_speed;
    } else if ((object->apiobj.character_data->game_character->flags_090 &
                GAMECHARACTER_FLAG_ANIMATION_SPEED_FROM_VELOCITY) != 0) {
        const f32 forward_speed = object->apiobj.velocity.x * object->facing_direction.x +
                                  object->apiobj.velocity.z * object->facing_direction.z;
        movement_speed = forward_speed < 0.0f ? 0.0f : forward_speed;
    } else {
        movement_speed = pad->input_magnitude;
    }

    // The original passes signed movement into UpdateAnimPacket.  Clips with
    // CHARACTER_ANIMATION_FLAG_REVERSE_WITH_MOVEMENT (including the acrobatic
    // jump clips) start at their end frame and play backwards while the
    // character is travelling backwards.
    f32 time_multiplier = 1.0f;
    if (object->character_context == 0x2d && object->field_0x788 != NULL)
        time_multiplier = GizBuildItMul(object);
    UpdateAnimPacket(model, &packet, (FRAMETIME * 30.0f) * time_multiplier, movement_speed * direction,
                     time_multiplier * FRAMETIME,
                     object->apiobj.character_data->game_character->backwards_speed_multiplier);
    if ((packet.flags & ANIMPACKET_FLAG_ANIMATION_CHANGED) != 0 &&
        (packet.blending != 0 ? packet.blend_animation_b : packet.animation_index) == 0x5f) {
        f32 *time = packet.blending != 0 ? &packet.blend_target_time : &packet.current_time;
        const f32 duration = AnimDuration(object->id, 0x5f, 0.0f, 0.0f, 0);
        i32 count = static_cast<i32>(duration / 0.3f);
        if (NuFmod(duration, 3.0f) > 0.15f)
            ++count;
        const i32 phase = qrand() / (0xffff / count + 1);
        CHARACTERMODEL_s *current_model = object->apiobj.character_model;
        const f32 start =
            static_cast<f32>(phase) *
                (0.3f * static_cast<CHARACTERANIM_s *>(current_model->model_data_a[0x5f])->playback_rate) +
            1.0f;
        if (NuAnimEndFrame(current_model->model_data_b[0x5f]) > start)
            *time = start;
        object->field_0xe21 = (object->field_0xe21 & ~0x40) | ((phase & 1) << 6);
    }
    AutoWeaponOnOff(object);
    AddFootSteps(object);
}

void Animate_BEAST(GameObject_s *) {
    STUBBED();
}

void Animate_BARMAN(GameObject_s *object) {
    GAMEPAD_s *pad = object->pad_gamepad;
    object->apiobj.anim_packet.requested_animation = CHARACTER_ANIMATION_IDLE;
    if ((pad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 && pad->input_magnitude > 0.0f) {
        object->apiobj.anim_packet.requested_animation = CHARACTER_ANIMATION_WALK;
    }
    UpdateCharacterIdle(object);
}

void Animate_CANNON(GameObject_s *object) {
    object->apiobj.anim_packet.requested_animation =
        (CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0
            ? object->context_animation
            : CHARACTER_ANIMATION_IDLE;
}

void Animate_WALKER(GameObject_s *object) {
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        object->apiobj.anim_packet.requested_animation = object->context_animation;
        return;
    }

    object->apiobj.anim_packet.requested_animation = CHARACTER_ANIMATION_IDLE;
    if ((object->pad_gamepad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 &&
        object->pad_gamepad->input_magnitude > 0.0f) {
        object->apiobj.anim_packet.requested_animation = CHARACTER_ANIMATION_WALK;
    }
}

void Animate_WEIRDO(GameObject_s *) {
    STUBBED();
}

void Animate_CRITTER(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        packet.requested_animation = object->context_animation;
        return;
    }

    if (object->character_context == 30) {
        packet.requested_animation = CHARACTER_ANIMATION_FALL;
        if (object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] != NULL) {
            return;
        }
    } else {
        packet.requested_animation = CHARACTER_ANIMATION_FALL;
    }

    if (object->character_context != CHARACTER_CONTEXT_DOOMED) {
        bool use_default_idle = object->apiobj.field_0x27d != 0;
        if (!use_default_idle) {
            if (object->ground_contact_grace_timer > 0.0f) {
                const GAMECHARACTERDATA *game_character = GetGameCharacterData(object);
                use_default_idle = game_character->field_0x28 <= 0.0f ||
                                   object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL;
            } else if (object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL) {
                use_default_idle = true;
            } else if (object->fall_animation_timer < 0.2f && object->nearby_floor_distance != 2000000.0f &&
                       object->nearby_floor_distance < 0.25f && object->apiobj.velocity.y < 0.0f) {
                use_default_idle = true;
            }
        }
        if (use_default_idle) {
            packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
        }
    }

    if (UseFallAnim(object)) {
        packet.requested_animation = CHARACTER_ANIMATION_FALL;
    } else if (packet.requested_animation != CHARACTER_ANIMATION_FALL &&
               (object->pad_gamepad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 &&
               object->pad_gamepad->input_magnitude > 0.0f) {
        const bool has_walk = object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_WALK] != NULL;
        const bool has_run = object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_RUN] != NULL;
        if (has_run && has_walk) {
            const GAMECHARACTERDATA *game_character = GetGameCharacterData(object);
            const f32 run_threshold = (game_character->walk_speed + game_character->run_speed) * 0.5f;
            packet.requested_animation = run_threshold < object->pad_gamepad->input_magnitude
                                             ? CHARACTER_ANIMATION_RUN
                                             : CHARACTER_ANIMATION_WALK;
        } else if (has_run) {
            packet.requested_animation = CHARACTER_ANIMATION_RUN;
        } else if (has_walk) {
            packet.requested_animation = CHARACTER_ANIMATION_WALK;
        }
    }
    MoveAnim_Check(object);
}

void Animate_DEFAULT(GameObject_s *object) {
    object->apiobj.anim_packet.requested_animation = CHARACTER_ANIMATION_IDLE;
}

extern i32 NeedsPretendAnim(GameObject_s *object);

void Animate_VEHICLE(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        packet.requested_animation = object->context_animation;
    } else {
        GAMECHARACTERDATA_s *character = object->apiobj.character_data->game_character;
        if ((reinterpret_cast<u8 *>(character)[0x92] & 0x10) != 0) {
            packet.requested_animation = 3;
        } else if ((object->id == id_STAP || object->id == id_STAP2 || object->id == id_JUMBOHOMINGDROID) &&
                   object->pad_gamepad->input_magnitude > 0.0f &&
                   object->apiobj.character_model->model_data_b[0] != NULL) {
            packet.requested_animation = 0;
        } else {
            packet.requested_animation = 1;
        }
    }

    if (WORLD->current_level == PLATFORM_LDATA && NeedsPretendAnim(object) != 0) {
        const u16 phase = static_cast<u16>(NuFmod(GameTimer.time_elapsed, 2.1f) / 2.1f * 65536.0f);
        object->render_offset.x = 0.1f * NU_SIN_LUT(phase);
        object->render_offset.y = 0.05f * NU_SIN_LUT(static_cast<u16>(phase + 0x2000));
        object->render_offset.z = 0.025f * NU_SIN_LUT(static_cast<u16>(phase ^ 0x8000));
        NuVecMtxRotate(&object->render_offset, &object->render_offset, &object->apiobj.field_0xb8);
    }
}

void Animate_DROIDEKA(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        packet.requested_animation = object->context_animation;
    } else {
        packet.requested_animation = CHARACTER_ANIMATION_FALL;
        if (object->character_context != CHARACTER_CONTEXT_DOOMED) {
            if (object->apiobj.field_0x27d != 0) {
                packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
            } else if (object->ground_contact_grace_timer > 0.0f) {
                const GAMECHARACTERDATA *game_character =
                    static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                if (game_character->field_0x28 <= 0.0f ||
                    object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL) {
                    packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
                }
            } else if (object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL ||
                       (object->fall_animation_timer < 0.2f && object->nearby_floor_distance != 2000000.0f &&
                        object->nearby_floor_distance < 0.25f && object->apiobj.velocity.y < 0.0f)) {
                const GAMECHARACTERDATA *game_character =
                    static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                if (game_character->field_0x28 <= 0.0f ||
                    object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL) {
                    packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
                }
            }
        }

        if (UseFallAnim(object) || (object->character_context == -1 && object->apiobj.field_0x27d == 0)) {
            packet.requested_animation = CHARACTER_ANIMATION_FALL;
        } else if ((object->pad_gamepad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 &&
                   object->pad_gamepad->input_magnitude > 0.0f) {
            const GAMECHARACTERDATA *game_character = GetGameCharacterData(object);
            const f32 walk_threshold = (game_character->tiptoe_speed + game_character->walk_speed) * 0.5f;
            packet.requested_animation = object->pad_gamepad->input_magnitude > walk_threshold
                                             ? CHARACTER_ANIMATION_WALK
                                             : CHARACTER_ANIMATION_TIPTOE;
        }
        MoveAnim_Check(object);
    }

    const i16 animation = packet.requested_animation;
    if (animation == CHARACTER_ANIMATION_FALL ||
        ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_HIGH_JUMP) != 0 &&
         (animation == CHARACTER_ANIMATION_FALL_VARIANT_75 || animation == CHARACTER_ANIMATION_FALL_VARIANT_40 ||
          animation == CHARACTER_ANIMATION_FALL_VARIANT_76))) {
        object->fall_animation_timer += FRAMETIME;
    } else {
        object->fall_animation_timer = 0.0f;
    }
}

void Animate_PROTOCOL(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        packet.requested_animation = object->context_animation;
        goto final_animation;
    }
    if (object->context_target_position != NULL) {
        packet.requested_animation = static_cast<i16>(SetProtocolDroidFallAnim(object));
        goto final_animation;
    }
    packet.requested_animation = CHARACTER_ANIMATION_FALL;
    if (object->character_context == CHARACTER_CONTEXT_DOOMED)
        goto falling;
    if (object->apiobj.field_0x27d != 0)
        goto idle;
    if (object->ground_contact_grace_timer > 0.0f)
        goto check_fall;
    if (object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL)
        goto check_fall;
    if (!(object->fall_animation_timer < 0.2f) || object->nearby_floor_distance == 2000000.0f ||
        !(object->nearby_floor_distance < 0.25f) || !(object->apiobj.velocity.y < 0.0f))
        goto falling;
check_fall:
    if (!(GetGameCharacterData(object)->field_0x28 <= 0.0f) &&
        object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] != NULL)
        goto falling;
idle:
    packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
    if (packet.requested_animation == CHARACTER_ANIMATION_FALL)
        goto falling;
    switch (object->field_0xe38) {
        case 1:
            packet.requested_animation = 8;
            break;
        case 2:
            packet.requested_animation = 20;
            break;
        case 3:
            packet.requested_animation = 15;
            break;
        default:
            packet.requested_animation = 1;
            break;
    }
    if (UseFallAnim(object)) {
        packet.requested_animation = CHARACTER_ANIMATION_FALL;
    } else if ((object->pad_gamepad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 &&
               object->pad_gamepad->input_magnitude > 0.0f) {
        switch (object->field_0xe38) {
            case 1:
                packet.requested_animation = 29;
                break;
            case 2:
                packet.requested_animation = 23;
                break;
            case 3:
                packet.requested_animation = 3;
                break;
            default:
                packet.requested_animation = 0;
                break;
        }
    }
    goto final_animation;
falling:
    packet.requested_animation = static_cast<i16>(SetProtocolDroidFallAnim(object));
    if (UseFallAnim(object))
        packet.requested_animation = CHARACTER_ANIMATION_FALL;
final_animation:
    if (packet.requested_animation == CHARACTER_ANIMATION_FALL)
        packet.requested_animation = static_cast<i16>(SetProtocolDroidFallAnim(object));
    UpdateCharacterIdle(object);
    const i16 animation = packet.requested_animation;
    if (animation == CHARACTER_ANIMATION_FALL ||
        ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_HIGH_JUMP) != 0 &&
         (animation == CHARACTER_ANIMATION_FALL_VARIANT_75 || animation == CHARACTER_ANIMATION_FALL_VARIANT_40 ||
          animation == CHARACTER_ANIMATION_FALL_VARIANT_76))) {
        object->fall_animation_timer += FRAMETIME;
    } else {
        object->fall_animation_timer = 0.0f;
    }
}

// Original @0x4a9900.
void GameAnimSet_Draw(GAMEANIMSET_s &set) {
    for (GAMEANIMOBJ_s *object = set.objects; object != NULL; object = object->next) {
        if ((object->flags & 2) == 0 && NuSpecialGetVisibilityFn(&object->special) != 0) {
            NuSpecialDrawAt(&object->special, NuSpecialGetDrawMtx(&object->special));
        }
    }
}

i32 GameAnimSet_Play(GAMEANIMSET_s *set, float speed, i32 evaluate_state) {
    if (set == NULL) {
        return 1;
    }

    if (evaluate_state != 0) {
        GameAnimSet_EvaluateState(set);
    }
    set->flags = static_cast<GAMEANIMSET_FLAGS>(set->flags & ~GAMEANIMSET_FLAG_STOP_REQUESTED);

    if (speed >= 0.0f) {
        if (set->state == GAMEANIMSET_STATE_AT_END) {
            return 1;
        }
    } else if (speed < 0.0f) {
        if (set->state == GAMEANIMSET_STATE_AT_START) {
            return 1;
        }
    }

    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        nuinstanim_s *animation = object->instance_animation;
        if (animation == NULL) {
            continue;
        }

        animation->playing = 1;
        animation->waiting = 0;

        f32 direction;
        if (object->end_frame < object->start_frame) {
            direction = -1.0f;
        } else {
            direction = 1.0f;
        }
        f32 current_frame = animation->ltime * direction;
        if (object->start_frame * direction > current_frame) {
            animation->ltime = object->start_frame;
        } else if (current_frame > object->end_frame * direction) {
            animation->ltime = object->end_frame;
        }

        if (animation->fparam1 != 0.0f) {
            animation->tfactor = animation->fparam1 * speed * direction;
        } else {
            animation->tfactor = direction * speed;
        }
    }

    if ((set->flags & GAMEANIMSET_FLAG_IN_SYSTEM_LIST) == 0) {
        GameAnimSet_AddToSystemList(set);
    }
    return 1;
}

i32 GameAnimSet_Stop(GAMEANIMSET_s *set) {
    if (set == NULL) {
        return 1;
    }

    GAMEANIMSET_STATE state = static_cast<GAMEANIMSET_STATE>(set->state & ~GAMEANIMSET_STATE_AT_END);
    if (state != GAMEANIMSET_STATE_ACTIVE_FORWARD) {
        return 1;
    }

    set->flags = static_cast<GAMEANIMSET_FLAGS>(set->flags | GAMEANIMSET_FLAG_STOP_REQUESTED);
    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        if (object->instance_animation != NULL) {
            object->instance_animation->playing = 0;
        }
    }
    return 1;
}

void Animate_ASTROMECH(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) == 0) {
        if (object->context_target_position != NULL) {
            if (object->apiobj.character_model->model_data_b[43] != NULL) {
                packet.requested_animation = 43;
            } else {
                const i32 target_state =
                    *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(object->context_target_position) + 0x14);
                packet.requested_animation = target_state == 0 ? CHARACTER_ANIMATION_IDLE : CHARACTER_ANIMATION_FALL;
            }
        } else {
            packet.requested_animation = CHARACTER_ANIMATION_FALL;
            if (object->character_context != CHARACTER_CONTEXT_DOOMED) {
                if (object->apiobj.field_0x27d != 0) {
                    packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
                } else if (object->ground_contact_grace_timer > 0.0f) {
                    const GAMECHARACTERDATA *game_character =
                        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                    if (game_character->field_0x28 <= 0.0f ||
                        object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL) {
                        packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
                    }
                } else if (object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL ||
                           (object->fall_animation_timer < 0.2f && object->nearby_floor_distance != 2000000.0f &&
                            object->nearby_floor_distance < 0.25f && object->apiobj.velocity.y < 0.0f)) {
                    const GAMECHARACTERDATA *game_character =
                        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                    if (game_character->field_0x28 <= 0.0f ||
                        object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL) {
                        packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
                    }
                }
            }

            if (object->character_context == 0) {
                packet.requested_animation = object->pad_gamepad->input_magnitude > 0.0f ? 37 : 36;
            } else if (UseFallAnim(object)) {
                packet.requested_animation = CHARACTER_ANIMATION_FALL;
            } else if (packet.requested_animation != CHARACTER_ANIMATION_FALL &&
                       (object->pad_gamepad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 &&
                       object->pad_gamepad->input_magnitude > 0.0f) {
                packet.requested_animation = CHARACTER_ANIMATION_WALK;
            }
        }
    } else {
        packet.requested_animation = object->context_animation;
    }

    UpdateCharacterIdle(object);
    const i16 animation = packet.requested_animation;
    if (animation == CHARACTER_ANIMATION_FALL ||
        ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_HIGH_JUMP) != 0 &&
         (animation == CHARACTER_ANIMATION_FALL_VARIANT_75 || animation == CHARACTER_ANIMATION_FALL_VARIANT_40 ||
          animation == CHARACTER_ANIMATION_FALL_VARIANT_76))) {
        object->fall_animation_timer += FRAMETIME;
    } else {
        object->fall_animation_timer = 0.0f;
    }
}

void Animate_CHARACTER(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    bool check_movement_animation = false;

    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        packet.requested_animation = object->context_animation;
    } else if (object->context_target_position != NULL) {
        if (object->apiobj.character_model->model_data_b[43] != NULL) {
            packet.requested_animation = 43;
        } else {
            const i32 target_state =
                *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(object->context_target_position) + 0x14);
            packet.requested_animation = target_state == 0 ? CHARACTER_ANIMATION_IDLE : CHARACTER_ANIMATION_FALL;
        }
    } else if (object->character_context == CHARACTER_CONTEXT_FORCE) {
        packet.requested_animation = CHARACTER_ANIMATION_WEAPON_IDLE;
    } else {
        packet.requested_animation = CHARACTER_ANIMATION_FALL;

        if (object->character_context != CHARACTER_CONTEXT_DOOMED) {
            bool use_default_idle = object->apiobj.field_0x27d != 0;
            if (!use_default_idle) {
                const bool has_fall_animation =
                    object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] != NULL;
                if (object->ground_contact_grace_timer > 0.0f) {
                    const GAMECHARACTERDATA *game_character =
                        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                    use_default_idle = game_character->field_0x28 <= 0.0f || !has_fall_animation;
                } else if (!has_fall_animation) {
                    use_default_idle = true;
                } else if (object->fall_animation_timer < 0.2f && object->nearby_floor_distance != 2000000.0f &&
                           object->nearby_floor_distance < 0.25f && object->apiobj.velocity.y < 0.0f) {
                    use_default_idle = true;
                }
            }
            if (use_default_idle) {
                packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
            }
        }

        if (object->character_context == CHARACTER_CONTEXT_JUMP) {
            JumpAnimCode(object);
        } else if (UseFallAnim(object)) {
            packet.requested_animation = CHARACTER_ANIMATION_FALL;
        } else if (object->character_context == CHARACTER_CONTEXT_DOOMED) {
            // The doomed context retains the fall choice unless the model's
            // context handler supplied another action above.
        } else if (packet.requested_animation != CHARACTER_ANIMATION_FALL) {
            GAMEPAD_s *pad = object->pad_gamepad;
            if ((pad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 && pad->input_magnitude > 0.0f) {
                const GAMECHARACTERDATA *game_character =
                    static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                if (!object->apiobj.player_controlled && game_character->field275_0x116 == 1) {
                    MoveAnim_Manage(object, pad->input_magnitude, 0, 1);
                } else {
                    const i32 allow_tiptoe =
                        (game_character->flags_090 & GAMECHARACTER_FLAG_DISABLE_TIPTOE) == 0 ? 1 : 0;
                    MoveAnim_Manage(object, pad->input_magnitude, allow_tiptoe, 1);
                }
            } else if (((object->field_0xe22 & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) != 0 ||
                        object->field_0xe32 == 1) &&
                       object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_ALT_IDLE] != NULL) {
                packet.requested_animation = CHARACTER_ANIMATION_ALT_IDLE;
            }
        }
        check_movement_animation = true;
    }

    if (check_movement_animation) {
        MoveAnim_Check(object);
    }
    UpdateCharacterIdle(object);

    const i16 animation = packet.requested_animation;
    if (animation == CHARACTER_ANIMATION_FALL ||
        ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_HIGH_JUMP) != 0 &&
         (animation == CHARACTER_ANIMATION_FALL_VARIANT_75 || animation == CHARACTER_ANIMATION_FALL_VARIANT_40 ||
          animation == CHARACTER_ANIMATION_FALL_VARIANT_76))) {
        object->fall_animation_timer += FRAMETIME;
    } else {
        object->fall_animation_timer = 0.0f;
    }
}

void Animate_GEONOSIAN(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        packet.requested_animation = object->context_animation;
    } else {
        packet.requested_animation = CHARACTER_ANIMATION_FALL;
        if (object->character_context != CHARACTER_CONTEXT_DOOMED &&
            (object->apiobj.field_0x27d != 0 ||
             ((object->ground_contact_grace_timer > 0.0f ||
               object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL ||
               (object->fall_animation_timer < 0.2f && object->nearby_floor_distance != 2000000.0f &&
                object->nearby_floor_distance < 0.25f && object->apiobj.velocity.y < 0.0f)) &&
              !(static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->field_0x28 > 0.0f &&
                object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] != NULL)))) {
            packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
        }

        if (UseFallAnim(object)) {
            packet.requested_animation = CHARACTER_ANIMATION_FALL;
        } else if (object->character_context == -1 && object->field_0xe31 == 1) {
            packet.requested_animation = object->pad_gamepad->input_magnitude > 0.0f ? 37 : 15;
        } else if (packet.requested_animation != CHARACTER_ANIMATION_FALL &&
                   (object->pad_gamepad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 &&
                   object->pad_gamepad->input_magnitude > 0.0f) {
            packet.requested_animation = CHARACTER_ANIMATION_WALK;
        }
        MoveAnim_Check(object);
    }

    UpdateCharacterIdle(object);
    const i16 animation = packet.requested_animation;
    if (animation == CHARACTER_ANIMATION_FALL ||
        ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_HIGH_JUMP) != 0 &&
         (animation == CHARACTER_ANIMATION_FALL_VARIANT_40 || animation == CHARACTER_ANIMATION_FALL_VARIANT_75 ||
          animation == CHARACTER_ANIMATION_FALL_VARIANT_76))) {
        object->fall_animation_timer += FRAMETIME;
    } else {
        object->fall_animation_timer = 0.0f;
    }
}

i32 GameAnimSet_Reset(GAMEANIMSET_s *set) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            nuinstanim_s *animation = object->instance_animation;
            if (animation != NULL) {
                animation->playing = 0;
                animation->waiting = 0;
                animation->tfactor = 1.0f;
                animation->ltime = object->start_frame;
            }
            GameAnimSet_RemoveFromSystemList(set);
        }
    }
    return 1;
}

void Animate_HOVERDROID(GameObject_s *object) {
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        object->apiobj.anim_packet.requested_animation = object->context_animation;
    } else if (object->character_context == 0x1e &&
               object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] != NULL) {
        object->apiobj.anim_packet.requested_animation = CHARACTER_ANIMATION_FALL;
    } else {
        object->apiobj.anim_packet.requested_animation = CHARACTER_ANIMATION_IDLE;
    }
}

GAMEANIMSET_s *GameAnimSet_Create(variptr_u *buf, variptr_u *buf_end, GAMEANIMOBJPOOL_s *object_pool,
                                  GAMEANIMSYS_s *system) {
    GAMEANIMSET_s *set = NULL;
    if (object_pool != NULL) {
        set = static_cast<GAMEANIMSET_s *>(GameBufferAlloc(buf, buf_end, sizeof(GAMEANIMSET_s)));
        if (set != NULL) {
            set->object_pool = object_pool;
            set->system = system;
            i32 index = system->set_count;
            if (index < gameanimsysprogress.entry_size) {
                system->sets[index] = set;
                system->set_count = index + 1;
            }
        }
    }
    return set;
}

void Animate_BATTLEDROID(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        packet.requested_animation = object->context_animation;
    } else if (object->context_target_position != NULL) {
        if (object->apiobj.character_model->model_data_b[43] != NULL) {
            packet.requested_animation = 43;
        } else {
            packet.requested_animation = object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL
                                             ? CHARACTER_ANIMATION_IDLE
                                             : CHARACTER_ANIMATION_FALL;
        }
    } else {
        packet.requested_animation = CHARACTER_ANIMATION_FALL;
        if (object->character_context != CHARACTER_CONTEXT_DOOMED) {
            bool use_default_idle = object->apiobj.field_0x27d != 0;
            if (!use_default_idle) {
                const bool has_fall = object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] != NULL;
                if (object->ground_contact_grace_timer > 0.0f || !has_fall ||
                    (object->fall_animation_timer < 0.2f && object->nearby_floor_distance != 2000000.0f &&
                     object->nearby_floor_distance < 0.25f && object->apiobj.velocity.y < 0.0f)) {
                    use_default_idle = GetGameCharacterData(object)->field_0x28 <= 0.0f || !has_fall;
                }
            }
            if (use_default_idle) {
                packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
            }
        }

        if (UseFallAnim(object)) {
            packet.requested_animation = CHARACTER_ANIMATION_FALL;
        } else if ((object->pad_gamepad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 &&
                   object->pad_gamepad->input_magnitude > 0.0f) {
            const GAMECHARACTERDATA *game_character = GetGameCharacterData(object);
            MoveAnim_Manage(object, object->pad_gamepad->input_magnitude,
                            (game_character->flags_090 & GAMECHARACTER_FLAG_DISABLE_TIPTOE) == 0 ? 1 : 0, 0);
        }
        MoveAnim_Check(object);
    }

    const i16 animation = packet.requested_animation;
    if (animation == CHARACTER_ANIMATION_FALL ||
        ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_HIGH_JUMP) != 0 &&
         (animation == CHARACTER_ANIMATION_FALL_VARIANT_75 || animation == CHARACTER_ANIMATION_FALL_VARIANT_40 ||
          animation == CHARACTER_ANIMATION_FALL_VARIANT_76))) {
        object->fall_animation_timer += FRAMETIME;
    } else {
        object->fall_animation_timer = 0.0f;
    }
}

void Animate_SPEEDERBIKE(GameObject_s *object) {
    object->apiobj.anim_packet.requested_animation =
        (CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0
            ? object->context_animation
            : CHARACTER_ANIMATION_IDLE;
}

i32 GameAnimSet_Playing(GAMEANIMSET_s *set) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            if (object->instance_animation != NULL && object->instance_animation->playing == 0) {
                return 0;
            }
        }
    }
    return 1;
}

void GameAnimSet_EvalAnim(GAMEANIMSET_s *set) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            EvalAnim2(&object->special, object->instance_animation->ltime);
        }
    }
}

GAMEANIMOBJ_s *GameAnimSet_AddObject(GAMEANIMSET_s *set, nuhspecial_s *special, float start_frame, float end_frame,
                                     i32 append) {
    if (set == NULL || set->object_pool == NULL || set->object_pool->free_objects == NULL || special == NULL ||
        NuSpecialExistsFn(special) == 0) {
        return NULL;
    }

    GAMEANIMOBJPOOL_s *pool = set->object_pool;
    ++set->object_count;
    GAMEANIMOBJ_s *object = pool->free_objects;
    ++pool->active_count;
    pool->free_objects = object->next;

    if (append != 0) {
        object->next = NULL;
        if (set->objects == NULL) {
            set->objects = object;
        } else {
            GAMEANIMOBJ_s *tail = set->objects;
            while (tail->next != NULL) {
                tail = tail->next;
            }
            tail->next = object;
        }
    } else {
        object->next = set->objects;
        set->objects = object;
    }

    const i32 object_index = object - pool->objects;
    object->object_data = static_cast<u8 *>(pool->object_data) + pool->object_data_size * object_index;
    object->special = *special;
    object->instance_animation = NuSpecialGetInstAnim(&object->special);
    if (object->instance_animation == NULL) {
        return object;
    }

    object->animation = object->special.scene->instance_animation_data[object->instance_animation->anim_ix];
    const f32 last_frame = NuSpecialGetAnimEndFrame(&object->special);
    if (last_frame > 0.0f) {
        if (last_frame < end_frame) {
            object->end_frame = last_frame;
        } else {
            object->end_frame = end_frame;
            if (object->end_frame < 1.0f) {
                object->end_frame = 1.0f;
            }
        }
        if (start_frame > last_frame) {
            object->start_frame = last_frame;
        } else {
            object->start_frame = start_frame;
            if (object->start_frame < 1.0f) {
                object->start_frame = 1.0f;
            }
        }
        ++set->animated_object_count;
    } else {
        NuSpecialGetName(&object->special);
    }
    return object;
}

i32 GameAnimSet_JumpToEnd(GAMEANIMSET_s *set) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            if (object->instance_animation != NULL) {
                object->instance_animation->playing = 0;
                object->instance_animation->ltime = object->end_frame;
            }
        }
    }
    return 1;
}

void GameAnimSet_SetOffset(GAMEANIMSET_s *set, NUVEC *offset) {
    if (set == NULL) {
        return;
    }

    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        if ((object->flags & 4) != 0) {
            continue;
        }
        NUMTX *source = NuSpecialGetMtx(&object->special);
        if (source == NULL) {
            continue;
        }

        NUMTX matrix = *source;
        matrix.m30 += offset->x;
        matrix.m31 += offset->y;
        matrix.m32 += offset->z;
        NuSpecialSetDrawMtx(&object->special, &matrix);
    }
}

f32 GameAnimSet_GetAnimPos(GAMEANIMOBJ_s *object) {
    if (object == NULL || object->instance_animation == NULL || object->animation == NULL) {
        return 0.0f;
    }

    if (object->start_frame == object->end_frame) {
        return 1.0f;
    }

    f32 position =
        (object->instance_animation->ltime - object->start_frame) / (object->end_frame - object->start_frame);
    if (position < 0.0f) {
        return 0.0f;
    }
    if (position > 1.0f) {
        position = 1.0f;
    }
    return position;
}

void GameAnimSet_SetAnimPos(GAMEANIMOBJ_s *object, float position) {
    if (object == NULL || object->instance_animation == NULL || object->animation == NULL) {
        return;
    }

    if (position < 0.0f) {
        position = 0.0f;
    }
    if (position > 1.0f) {
        position = 1.0f;
    }
    object->instance_animation->ltime = (object->end_frame - object->start_frame) * position + object->start_frame;
}

void GameAnimSet_SetTFactor(GAMEANIMSET_s *set, float factor) {
    if (set == NULL) {
        return;
    }

    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        nuinstanim_s *animation = object->instance_animation;
        if (animation != NULL) {
            f32 direction = object->end_frame < object->start_frame ? -1.0f : 1.0f;
            if (animation->fparam1 != 0.0f) {
                animation->tfactor = animation->fparam1 * factor * direction;
            } else {
                animation->tfactor = direction * factor;
            }
        }
    }
}

void Animate_REPUBLICGUNSHIP(GameObject_s *object) {
    object->apiobj.anim_packet.requested_animation = CHARACTER_ANIMATION_IDLE;
    if (object->character_context == 0x23) {
        object->apiobj.anim_packet.requested_animation = 0x2c;
    } else if (object->character_context == 0x24) {
        object->apiobj.anim_packet.requested_animation = 0x2d;
    }
    UpdateCharacterIdle(object);
}

i32 GameAnimSet_JumpToStart(GAMEANIMSET_s *set) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            if (object->instance_animation != NULL) {
                object->instance_animation->playing = 0;
                object->instance_animation->ltime = object->start_frame;
            }
        }
    }
    return 1;
}

void Animate_SUPERBATTLEDROID(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        packet.requested_animation = object->context_animation;
    } else if (object->context_target_position != NULL) {
        if (object->apiobj.character_model->model_data_b[43] != NULL) {
            packet.requested_animation = 43;
        } else {
            const i32 target_state =
                *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(object->context_target_position) + 0x14);
            packet.requested_animation = target_state == 0 ? CHARACTER_ANIMATION_IDLE : CHARACTER_ANIMATION_FALL;
        }
    } else {
        packet.requested_animation = CHARACTER_ANIMATION_FALL;
        if (object->character_context != CHARACTER_CONTEXT_DOOMED &&
            object->character_context != CHARACTER_CONTEXT_JUMP) {
            bool use_default_idle = object->apiobj.field_0x27d != 0;
            if (!use_default_idle) {
                const bool has_fall = object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] != NULL;
                if (object->ground_contact_grace_timer > 0.0f) {
                    const GAMECHARACTERDATA *game_character = GetGameCharacterData(object);
                    use_default_idle = game_character->field_0x28 <= 0.0f || !has_fall;
                } else if (!has_fall) {
                    use_default_idle = true;
                } else if (object->fall_animation_timer < 0.2f && object->nearby_floor_distance != 2000000.0f &&
                           object->nearby_floor_distance < 0.25f && object->apiobj.velocity.y < 0.0f) {
                    use_default_idle = true;
                }
            }
            if (use_default_idle) {
                packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
            }
        }

        if (UseFallAnim(object)) {
            packet.requested_animation = CHARACTER_ANIMATION_FALL;
        } else if (packet.requested_animation != CHARACTER_ANIMATION_FALL) {
            const GAMECHARACTERDATA *game_character = GetGameCharacterData(object);
            const bool weapon_out =
                (object->field_0xe22 & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) != 0 || object->weapon_scale > 0.0f;
            if ((object->pad_gamepad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 &&
                object->pad_gamepad->input_magnitude > 0.0f) {
                const f32 run_threshold = (game_character->walk_speed + game_character->run_speed) * 0.5f;
                if (object->pad_gamepad->input_magnitude > run_threshold) {
                    packet.requested_animation = weapon_out ? CHARACTER_ANIMATION_SABER_RUN : CHARACTER_ANIMATION_RUN;
                } else {
                    packet.requested_animation = weapon_out ? CHARACTER_ANIMATION_SABER_WALK : CHARACTER_ANIMATION_WALK;
                }
            } else if (object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_ALT_IDLE] != NULL &&
                       object->weapon_scale >= 0.5f) {
                packet.requested_animation = CHARACTER_ANIMATION_ALT_IDLE;
            }
        }
        MoveAnim_Check(object);
    }

    UpdateCharacterIdle(object);
    const i16 animation = packet.requested_animation;
    if (animation == CHARACTER_ANIMATION_FALL ||
        ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_HIGH_JUMP) != 0 &&
         (animation == CHARACTER_ANIMATION_FALL_VARIANT_75 || animation == CHARACTER_ANIMATION_FALL_VARIANT_40 ||
          animation == CHARACTER_ANIMATION_FALL_VARIANT_76))) {
        object->fall_animation_timer += FRAMETIME;
    } else {
        object->fall_animation_timer = 0.0f;
    }
}

void GameAnimSet_RemoveObject(GAMEANIMSET_s *set, GAMEANIMOBJ_s *object) {
    if (object == NULL || set == NULL)
        return;
    if (set->objects == object) {
        set->objects = object->next;
    } else {
        GAMEANIMOBJ_s *previous = set->objects;
        while (previous != NULL && previous->next != object) {
            previous = previous->next;
        }
        if (previous != NULL)
            previous->next = object->next;
    }
    object->next = NULL;
    --set->object_count;
    GAMEANIMOBJPOOL_s *pool = set->object_pool;
    --pool->active_count;
    object->next = pool->free_objects;
    pool->free_objects = object;
}

void GameAnimSet_ScaleFParam1(GAMEANIMSET_s *set, float scale) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            if (object->instance_animation != NULL) {
                object->instance_animation->fparam1 *= scale;
            }
        }
    }
}

i32 GameAnimSet_SetRepeating(GAMEANIMSET_s *set, i32 repeating) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            if (object->instance_animation != NULL) {
                object->instance_animation->repeating = repeating & 1;
            }
        }
    }
    return 1;
}

void GameAnimSet_EvaluateState(GAMEANIMSET_s *set) {
    if ((set->flags & GAMEANIMSET_FLAG_IN_SYSTEM_LIST) != 0) {
        return;
    }

    GAMEANIMOBJ_s *object = set->objects;
    i32 all_at_end = 1;
    i32 all_at_start = 1;

    while (object != NULL) {
        nuinstanim_s *animation = object->instance_animation;
        if (animation != NULL) {
            f32 direction = object->end_frame < object->start_frame ? -1.0f : 1.0f;
            f32 current_frame = animation->ltime * direction;
            if (object->end_frame * direction > current_frame) {
                all_at_end = 0;
            }
            if (current_frame > object->start_frame * direction) {
                all_at_start = 0;
            }
        }
        object = object->next;
    }

    set->state = GAMEANIMSET_STATE_AT_START;
    if (all_at_end != 0) {
        set->state = GAMEANIMSET_STATE_AT_END;
    } else if (all_at_start == 0) {
        set->state = GAMEANIMSET_STATE_BETWEEN_ENDPOINTS;
    }
}

i32 GameAnimSet_GetAveragePos(GAMEANIMSET_s *set, NUVEC *position, i32 frame_selection, i32 include_animated,
                              i32 include_static) {
    NUVEC sum = {0.0f, 0.0f, 0.0f};
    if (position == NULL || set == NULL || set->object_count == 0 || set->objects == NULL)
        return 0;
    i32 count = 0;
    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        if ((object->flags & 1) != 0)
            continue;
        if (object->instance_animation != NULL) {
            if (include_animated == 0)
                continue;
            f32 frame;
            if (frame_selection == 0)
                frame = object->start_frame;
            else if (frame_selection == 1)
                frame = object->end_frame;
            else
                frame = object->instance_animation->ltime;
            NUMTX matrix;
            EvalAnim(&object->special, frame, &matrix, 1);
            NuVecAdd(&sum, &sum, NUMTX_GET_ROW_VEC(&matrix, 3));
            ++count;
        } else if (include_static != 0) {
            NuVecAdd(&sum, &sum, NuSpecialGetDrawPos(&object->special));
            ++count;
        }
    }
    if (count == 0)
        return 0;
    NuVecScale(position, &sum, 1.0f / static_cast<f32>(count));
    return 1;
}

GAMEANIMSET_VISIBILITY GameAnimSet_GetVisibility(GAMEANIMSET_s *set) {
    if (set == NULL) {
        return GAMEANIMSET_VISIBILITY_NONE;
    }

    i32 visible_count = 0;
    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        if (NuSpecialGetVisibilityFn(&object->special) == 1) {
            ++visible_count;
        }
    }

    if (visible_count == set->object_count) {
        return GAMEANIMSET_VISIBILITY_ALL;
    }
    if (visible_count > 0) {
        return GAMEANIMSET_VISIBILITY_PARTIAL;
    }
    return GAMEANIMSET_VISIBILITY_NONE;
}

void GameAnimSet_JumpToAnimPos(GAMEANIMSET_s *set, float position) {
    if (set == NULL) {
        return;
    }

    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        nuinstanim_s *animation = object->instance_animation;
        if (animation == NULL || object->animation == NULL) {
            continue;
        }

        animation->playing = 0;
        f32 frame = (object->end_frame - object->start_frame) * position + object->start_frame;
        f32 direction = object->end_frame < object->start_frame ? -1.0f : 1.0f;
        f32 scaled_frame = frame * direction;
        f32 scaled_end = object->end_frame * direction;
        if (scaled_frame <= scaled_end) {
            animation->ltime = frame;
        } else {
            animation->ltime = object->end_frame;
            scaled_frame = scaled_end;
        }
        if (object->start_frame * direction > scaled_frame) {
            animation->ltime = object->start_frame;
        }
    }
}

void GameAnimSet_RemoveSpecial(GAMEANIMSET_s *set, nuhspecial_s *special) {
    if (special == NULL || set == NULL)
        return;
    GAMEANIMOBJ_s *object = set->objects;
    while (object != NULL && NuSpecialCompare(&object->special, special) == 0) {
        object = object->next;
    }
    if (object != NULL)
        GameAnimSet_RemoveObject(set, object);
}

void GameAnimSet_SetVisibility(GAMEANIMSET_s *set, i32 visibility) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            NuSpecialSetVisibility(&object->special, visibility);
        }
    }
}

void GameAnimSet_DrawReflection(GAMEANIMSET_s *set, i32 axis, float offset, numtx_s *matrix) {
    if (set == NULL || set->objects == NULL) {
        return;
    }
    if (matrix == NULL) {
        matrix = NuSpecialGetMtx(&set->objects->special);
    }
    f32 plane = offset + reinterpret_cast<f32 *>(matrix)[11 + axis];
    NuRndrStartReflectionRender(0);
    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        if ((object->flags & 2) == 0 && NuSpecialGetVisibilityFn(&object->special) != 0) {
            NUMTX_ALIGNED16 reflection;
            extern i32 MatrixReflection(NUMTX *, i32, f32, f32, NUMTX *);
            NUMTX *draw_matrix = NuSpecialGetDrawMtx(&object->special);
            if (MatrixReflection(draw_matrix, axis, plane, WORLD->current_level->unknown_0cc, &reflection) != 0) {
                NuSpecialDrawAt(&object->special, &reflection);
            }
        }
    }
    NuRndrEndReflectionRender();
}

GAMEANIMOBJ_s *GameAnimSet_AddObjectByName(GAMEANIMSET_s *set, nugscn_s *scene, char *name, float start_frame,
                                           float end_frame, i32 append, GIZMOSYS_s *gizmo_sys, char *prefix,
                                           char *suffix) {
    if (set == NULL) {
        return NULL;
    }

    nuhspecial_s special;
    if (Gizmo_FindNuSpecial(scene, &special, name, 1, gizmo_sys, prefix, suffix) == 0) {
        return NULL;
    }
    return GameAnimSet_AddObject(set, &special, start_frame, end_frame, append);
}

void GameAnimSet_AddToSystemList(GAMEANIMSET_s *set) {
    if (set != NULL && set->system != NULL && (set->flags & GAMEANIMSET_FLAG_IN_SYSTEM_LIST) == 0) {
        NuLinkedListAppend(&set->system->active_sets, &set->links);
        set->flags = static_cast<GAMEANIMSET_FLAGS>(set->flags | GAMEANIMSET_FLAG_IN_SYSTEM_LIST);
    }
}

f32 GameAnimSet_AutoSetReflectY(GAMEANIMSET_s *set, nuvec_s *position, numtx_s *matrix) {
    if (set != NULL && set->objects != NULL) {
        if (matrix == NULL) {
            matrix = NuSpecialGetMtx(&set->objects->special);
        }
        f32 height = matrix->m31;
        f32 ground = GameShadow(NULL, position, 5.0f, -1);
        if (ground != 2000000.0f) {
            return ground - height;
        }
    }
    return 0.0f;
}

f32 GameAnimSet_GetCurrentFrame(GAMEANIMSET_s *set) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            if (object->instance_animation != NULL) {
                return object->instance_animation->ltime;
            }
        }
    }
    return 0.0f;
}

GAMEANIMOBJPOOL_s *GameAnimSet_CreateObjectPool(variptr_u *buf, variptr_u *buf_end, i32 object_data_size,
                                                i32 capacity) {
    GAMEANIMOBJPOOL_s *pool = NULL;
    if (capacity != 0) {
        pool = static_cast<GAMEANIMOBJPOOL_s *>(GameBufferAlloc(buf, buf_end, sizeof(GAMEANIMOBJPOOL_s)));
        if (pool != NULL) {
            pool->capacity = static_cast<u16>(capacity);
            pool->object_data_size = static_cast<u16>(object_data_size);
            pool->objects = static_cast<GAMEANIMOBJ_s *>(
                GameBufferAlloc(buf, buf_end, static_cast<u16>(capacity) * sizeof(GAMEANIMOBJ_s)));
            if (object_data_size != 0) {
                pool->object_data = GameBufferAlloc(
                    buf, buf_end, static_cast<u16>(pool->object_data_size) * static_cast<u16>(pool->capacity));
            }

            for (i32 i = 0; i < pool->capacity; ++i) {
                GAMEANIMOBJ_s *object = &pool->objects[i];
                object->next = pool->free_objects;
                pool->free_objects = object;
            }
        }
    }
    return pool;
}

i32 GameAnimSet_IsAnimationReset(GAMEANIMSET_s *set) {
    if (set == NULL || set->objects == NULL) {
        return 1;
    }

    i32 repeating_count = 0;
    i32 animated_count = 0;
    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        nuinstanim_s *animation = object->instance_animation;
        if (animation == NULL) {
            continue;
        }

        ++animated_count;
        if (animation->repeating != 0) {
            ++repeating_count;
            continue;
        }
        if (animation->playing != 0) {
            return 0;
        }

        const f32 direction = object->end_frame < object->start_frame ? -1.0f : 1.0f;
        if (animation->ltime * direction > object->start_frame * direction) {
            return 0;
        }
    }

    if (animated_count == 0) {
        return 1;
    }
    return animated_count != repeating_count;
}

void GameAnimSet_RemoveAllObjects(GAMEANIMSET_s *set) {
    if (set != NULL) {
        while (set->objects != NULL)
            GameAnimSet_RemoveObject(set, set->objects);
    }
}

i32 GameAnimSet_GetCentreAndRadius(GAMEANIMSET_s *set, NUVEC *centre, f32 *radius, i32 frame_selection,
                                   i32 include_animated, i32 include_static) {
    if (centre == NULL || set == NULL || set->object_count == 0 || set->objects == NULL) {
        return 0;
    }

    NUVEC minimum = {1.0e9f, 1.0e9f, 1.0e9f};
    NUVEC maximum = {-1.0e9f, -1.0e9f, -1.0e9f};
    bool found_object = false;

    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        if ((object->flags & 1) != 0) {
            continue;
        }

        NUMTX matrix;
        if (object->instance_animation == NULL) {
            if (include_static == 0) {
                continue;
            }
            NUMTX *draw_matrix = NuSpecialGetDrawMtx(&object->special);
            if (draw_matrix == NULL) {
                continue;
            }
            matrix = *draw_matrix;
        } else {
            if (include_animated == 0) {
                continue;
            }
            f32 frame;
            if (frame_selection == 0) {
                frame = object->start_frame;
            } else if (frame_selection == 1) {
                frame = object->end_frame;
            } else {
                frame = object->instance_animation->ltime;
            }
            EvalAnim(&object->special, frame, &matrix, 1);
        }

        NUVEC object_centre;
        f32 object_radius;
        NuSpecialGetRadius(&object->special, &object_centre, &object_radius);
        object_radius *= 0.75f;
        NuVecMtxTransform(&object_centre, &object_centre, &matrix);

        const NUVEC object_minimum = {
            object_centre.x - object_radius,
            object_centre.y - object_radius,
            object_centre.z - object_radius,
        };
        const NUVEC object_maximum = {
            object_centre.x + object_radius,
            object_centre.y + object_radius,
            object_centre.z + object_radius,
        };
        if (object_minimum.x < minimum.x) {
            minimum.x = object_minimum.x;
        }
        if (object_minimum.y < minimum.y) {
            minimum.y = object_minimum.y;
        }
        if (object_minimum.z < minimum.z) {
            minimum.z = object_minimum.z;
        }
        if (object_maximum.x > maximum.x) {
            maximum.x = object_maximum.x;
        }
        if (object_maximum.y > maximum.y) {
            maximum.y = object_maximum.y;
        }
        if (object_maximum.z > maximum.z) {
            maximum.z = object_maximum.z;
        }
        found_object = true;
    }

    if (!found_object) {
        return 0;
    }

    centre->x = (minimum.x + maximum.x) * 0.5f;
    centre->y = (minimum.y + maximum.y) * 0.5f;
    centre->z = (minimum.z + maximum.z) * 0.5f;
    if (radius != NULL) {
        const f32 half_x = (maximum.x - minimum.x) * 0.5f;
        const f32 half_y = (maximum.y - minimum.y) * 0.5f;
        const f32 half_z = (maximum.z - minimum.z) * 0.5f;
        *radius = NuFsqrt(half_x * half_x + half_y * half_y + half_z * half_z);
    }
    return 1;
}

f32 GameAnimSet_GetCompletionRatio(GAMEANIMSET_s *set) {
    if (set == NULL) {
        return 0.0f;
    }
    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        if (object->instance_animation == NULL) {
            continue;
        }
        if (object->start_frame == object->end_frame) {
            return 1.0f;
        }
        f32 ratio =
            (object->instance_animation->ltime - object->start_frame) / (object->end_frame - object->start_frame);
        if (ratio > 1.0f) {
            ratio = 1.0f;
        }
        if (ratio < 0.0f) {
            ratio = 0.0f;
        }
        return ratio;
    }
    return 0.0f;
}

void GameAnimSet_RemoveFromSystemList(GAMEANIMSET_s *set) {
    if (set != NULL && set->system != NULL) {
        if ((set->flags & GAMEANIMSET_FLAG_IN_SYSTEM_LIST) != 0) {
            NuLinkedListRemove(&set->system->active_sets, &set->links);
            set->flags = static_cast<GAMEANIMSET_FLAGS>(set->flags & ~GAMEANIMSET_FLAG_IN_SYSTEM_LIST);
        }
        GameAnimSet_EvaluateState(set);
    }
}

i32 ANI_SimpleAni3PlayerV4Joint_Blend_Quat3(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, f32 blend,
                                            i32 joint_count, i32 first_joint, NUVEC *root_translation);
i32 ANI_SimpleAni3PlayerV4Joint_Blend_Quat3W(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, f32 blend,
                                             i32 joint_count, i32 first_joint, NUVEC *root_translation);

extern "C" {

    i32 ANI_SimpleAni3PlayerV4Joint_EulerQuat(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, i32 joint_count,
                                              i32 first_joint);
    void ANI_SimpleAni3PlayerV4Joint_Blend_EulerQuat(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer,
                                                     f32 blend, i32 joint_count, i32 first_joint,
                                                     NUVEC *root_translation);

    void ANI_FixUpAddrs(ani3_animheader_s *anim, isize delta, i32) {
        if (anim->magic != 0x414e4934) {
            return;
        }
        while (true) {
            if (anim->constants != NULL) {
                anim->constants = reinterpret_cast<i16 *>(reinterpret_cast<usize>(anim->constants) + (usize)delta);
            }
            if (anim->scale_min != NULL) {
                anim->scale_min =
                    reinterpret_cast<ani3_scalemin_s *>(reinterpret_cast<usize>(anim->scale_min) + (usize)delta);
            }
            if (anim->keys != NULL) {
                anim->keys = reinterpret_cast<u8 *>(reinterpret_cast<usize>(anim->keys) + (usize)delta);
            }
            if (anim->curve_types != NULL) {
                anim->curve_types = reinterpret_cast<u16 *>(reinterpret_cast<usize>(anim->curve_types) + (usize)delta);
            }
            if (anim->node_flags != NULL) {
                anim->node_flags = reinterpret_cast<u8 *>(reinterpret_cast<usize>(anim->node_flags) + (usize)delta);
            }
            if (anim->field_38 != NULL) {
                anim->field_38 = reinterpret_cast<void *>(reinterpret_cast<usize>(anim->field_38) + (usize)delta);
            }
            u16 next = anim->next_block;
            if (next == 0) {
                break;
            }
            delta += next;
            anim = reinterpret_cast<ani3_animheader_s *>(reinterpret_cast<usize>(anim) + next);
        }
    }

    // Original @0x2c17d0. The non-quaternion ANI4 player uses a compact
    // four-samples-per-word curve stream. Only groups enabled by the node's
    // translation/rotation/scale flags occupy space in that stream.

    f32 GetInstAnimEndFrame(nugscn_s *scene, nuinstanim_s *instance_animation) {
        if (instance_animation == NULL) {
            return 0.0f;
        }

        void *animation = scene->instance_animation_data[instance_animation->anim_ix];
        if (animation != NULL) {
            return NuAnimEndFrameOld(animation);
        }
        if ((instance_animation->end_frame_lookup_bits & NUINSTANIM_END_FRAME_LOOKUP_MASK) == 0 ||
            scene->animation_end_frames == NULL) {
            return 0.0f;
        }

        return static_cast<f32>(scene->animation_end_frames[instance_animation->end_frame_lookup_index - 1].end_frame);
    }

    NUJOINTPROCANIMFN JointProcAnimFn;

    void SetProceduralAnimationFn(void *function) {
        JointProcAnimFn = reinterpret_cast<NUJOINTPROCANIMFN>(function);
    }

    i32 StateAnimEvaluate(StateAnim *state, u8 *index, u8 *value, f32 frame) {
        u8 next = *index;
        if (next < state->count) {
            bool changed = false;
            do {
                if (frame < state->times[next]) {
                    if (changed) {
                        return 1;
                    }
                    break;
                }
                changed = true;
                *value = state->values[next];
                next = static_cast<u8>(*index + 1);
                *index = next;
            } while (next < state->count);
            if (next >= state->count) {
                return 1;
            }
        }

        i32 changed = 0;
        if (next == 0) {
            return 0;
        }
        do {
            if (state->times[next - 1] <= frame) {
                return changed;
            }
            next--;
            *index = next;
            *value = next == 0 ? state->values[0] : state->values[next - 1];
            changed = 1;
        } while (next != 0);
        return 1;
    }

    bool StateAnimEvaluate2(StateAnim *state, u8 *index, char *value, f32 frame) {
        i32 current = *index;
        i32 count = state->count;
        if (current >= count) {
            current = count - 1;
        }
        if (current < 0) {
            current = 0;
        }
        char old_value = state->values[current];
        if (frame < state->times[current]) {
            while (current != 0 && frame < state->times[current - 1]) {
                --current;
            }
        } else {
            while (current < count - 1 && state->times[current + 1] <= frame) {
                ++current;
            }
        }
        char new_value = state->values[current];
        *value = new_value;
        *index = static_cast<u8>(current);
        return old_value != new_value;
    }

    void NuSpecialSetInstAnimTime(nuhspecial_s *special, f32 frame) {
        NUGSCN *scene = special->scene;
        if (scene == NULL) {
            return;
        }
        nuinstanim_s *animation = NuSpecialGetInstAnim(special);
        if (animation == NULL) {
            return;
        }
        animation->ltime = frame;
        if ((animation->end_frame_lookup_bits & NUINSTANIM_END_FRAME_LOOKUP_MASK) != 0 &&
            scene->animation_end_frames != NULL) {
            StateAnim *state =
                reinterpret_cast<StateAnim *>(&scene->animation_end_frames[animation->end_frame_lookup_index - 1]);
            u8 index = static_cast<u8>(static_cast<u32>(animation->flags) >> NUINSTANIM_STATE_INDEX_SHIFT);
            char value;
            StateAnimEvaluate2(state, &index, &value, frame);
            animation->flags =
                static_cast<NUINSTANIM_FLAGS>((static_cast<u32>(animation->flags) & ~NUINSTANIM_STATE_INDEX_MASK) |
                                              (static_cast<u32>(index) << NUINSTANIM_STATE_INDEX_SHIFT));
        }
    }

    StateAnim *StateAnimFixPtrs(StateAnim *state, isize delta) {
        if (state == NULL) {
            return NULL;
        }
        state = reinterpret_cast<StateAnim *>(reinterpret_cast<usize>(state) + delta);
        if (state == NULL) {
            return NULL;
        }
        state->times =
            state->times != NULL ? reinterpret_cast<f32 *>(reinterpret_cast<usize>(state->times) + delta) : NULL;
        state->values =
            state->values != NULL ? reinterpret_cast<u8 *>(reinterpret_cast<usize>(state->values) + delta) : NULL;
        return state;
    }

} // extern "C"

void SetAnimFrame(nuhspecial_s *special, float frame) {
    if (NuSpecialExistsFn(special) == 0)
        return;
    NUMTX_ALIGNED16 matrix;
    NuMtxSetIdentity(&matrix);
    nuinstanim_s *instance_animation = NuSpecialGetInstAnim(special);
    if (instance_animation == NULL)
        return;
    nuanimdata_s *animation = special->scene->instance_animation_data[instance_animation->anim_ix];
    if (animation == NULL)
        return;
    // The animation header begins with its final frame; the remaining header is opaque here.
    f32 end_frame;
    memcpy(&end_frame, animation, sizeof(end_frame));
    if (frame == 1.0e9f)
        frame = end_frame;
    if (!(frame >= 1.0f && frame <= end_frame))
        return;
    NuAnimData2CalcMatrix(animation, 0, frame, &matrix);
    instance_animation->mtx = matrix;
    NUMTX *instance_matrix = NuSpecialGetInstanceMtx(special);
    instance_animation->mtx.m30 = instance_matrix->m30;
    instance_animation->mtx.m31 = instance_matrix->m31;
    instance_animation->mtx.m32 = instance_matrix->m32;
    instance_animation->ltime = frame;
}

i32 GetAnimDirection(nuinstanim_s *animation) {
    if (animation == NULL || animation->tfactor == 0.0f) {
        return -1;
    }
    return animation->tfactor < 0.0f ? 1 : 0;
}

i32 FindTexAnimFromMtl(nugscn_s *scene, numtl_s *material) {
    nutexanim_s *animations = static_cast<nutexanim_s *>(scene->texture_anims);
    for (i32 animation_index = 0; animation_index < scene->num_texture_anims; ++animation_index) {
        if (animations[animation_index].material == material) {
            return animation_index + 1;
        }
    }
    return 0;
}

static char **TexAnimList;

void InitTexAnimScripts(char **names) {
    TexAnimList = names;
    if (names == NULL)
        return;
    while (*names != NULL) {
        permbuffer_ptr.addr = ALIGN(permbuffer_ptr.addr, 4);
        char path[72];
        NuStrCpy(path, "stuff\\ats\\");
        NuStrCat(path, *names++);
        NuStrCat(path, ".ats");
        NuTexAnimProgReadScript(path, &permbuffer_ptr);
    }
    permbuffer_ptr.addr = ALIGN(permbuffer_ptr.addr, 16);
}

i32 GizmoFileReadGameAnimSet(GAMEANIMSET_s *set, void *world_ptr,
                             void (*read_object_data)(GAMEANIMOBJ_s *, unsigned char), unsigned char version,
                             char *prefix, char *suffix) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    const unsigned char file_version = static_cast<unsigned char>(EdFileReadChar());
    const unsigned char object_count = static_cast<unsigned char>(EdFileReadChar());
    i32 success = 1;

    for (i32 object_index = 0; object_index < object_count; ++object_index) {
        char object_name[64];
        const i32 name_length = static_cast<signed char>(EdFileReadChar());
        if (name_length != 0) {
            EdFileRead(object_name, name_length);
        }

        const f32 start_frame = EdFileReadFloat();
        const f32 end_frame = EdFileReadFloat();
        u32 flags = 0;
        if (file_version > 1) {
            flags = EdFileReadInt();
        }

        if (name_length == 0) {
            continue;
        }

        if (set->object_pool == NULL || set->object_pool->free_objects == NULL) {
            success = 0;
        }

        GAMEANIMOBJ_s *object = GameAnimSet_AddObjectByName(set, world->current_gscn, object_name, start_frame,
                                                            end_frame, 0, world->gizmo_sys, prefix, suffix);
        GAMEANIMOBJ_s missing_object;
        if (object == NULL) {
            memset(&missing_object, 0, sizeof(missing_object));
            object = &missing_object;
        }

        object->flags = flags;
        if (read_object_data != NULL) {
            read_object_data(object, version);
        }
        if (file_version <= 2) {
            object->flags &= ~2u;
        }
    }

    return success;
}

void EvalAnim(nuhspecial_s *special, float frame, numtx_s *matrix, i32 include_instance_translation) {
    if (matrix == NULL || special == NULL) {
        return;
    }

    nuinstanim_s *instance_animation = NuSpecialGetInstAnim(special);
    if (instance_animation == NULL) {
        if (include_instance_translation != 0) {
            NUMTX *instance_matrix = NuSpecialGetInstanceMtx(special);
            if (instance_matrix != NULL) {
                memcpy(matrix, instance_matrix, sizeof(NUMTX));
            }
        }
        return;
    }

    NUGSCN *scene = special->scene;
    nuanimdata_s *animation = scene->instance_animation_data[instance_animation->anim_ix];
    if (animation == NULL) {
        return;
    }

    NuAnimData2CalcMatrix(animation, 0, frame, matrix);
    if (include_instance_translation != 0) {
        NUMTX *instance_matrix = NuSpecialGetInstanceMtx(special);
        if (instance_matrix != NULL) {
            NUVEC *translation = NUMTX_GET_ROW_VEC(matrix, 3);
            const NUVEC *instance_translation = NUMTX_GET_ROW_VEC(instance_matrix, 3);
            translation->x += instance_translation->x;
            translation->y += instance_translation->y;
            translation->z += instance_translation->z;
        }
    }
}

void EvalAnim2(nuhspecial_s *special, float frame) {
    nuinstanim_s *instance_animation = NuSpecialGetInstAnim(special);
    if (instance_animation == NULL) {
        return;
    }

    NUGSCN *scene = special->scene;
    nuanimdata_s *animation = scene->instance_animation_data[instance_animation->anim_ix];
    if (animation == NULL || frame == instance_animation->prev_eval_time) {
        return;
    }

    NUMTX *instance_matrix = NuSpecialGetInstanceMtx(special);
    NuAnimData2CalcMatrix(animation, 0, frame, &instance_animation->mtx);

    instance_animation->prev_eval_time = frame;
    const i32 instance_index = instance_animation - scene->instance_animations;
    NUVEC &animation_translation = *NUMTX_GET_ROW_VEC(&instance_animation->mtx, 3);
    NUVEC &instance_translation = *NUMTX_GET_ROW_VEC(instance_matrix, 3);
    NUMTX *evaluated_matrix = &scene->instance_animation_matrices[instance_index];
    memcpy(evaluated_matrix, &instance_animation->mtx, sizeof(NUMTX));

    animation_translation.x += instance_translation.x;
    animation_translation.y += instance_translation.y;
    animation_translation.z += instance_translation.z;
    NuSpecialUpdate(special);
}

void GameAnimSys_AllocateLevelProgressData(variptr_u *buf, variptr_u *buf_end, i32 capacity, i32 level_count) {
    if (buf_end == NULL || buf == NULL)
        return;
    gameanimsysprogress.count = level_count;
    gameanimsysprogress.entry_size = capacity;
    gameanimsysprogress.entries = static_cast<u8 **>(GameBufferAlloc(buf, buf_end, level_count * sizeof(u8 *)));
    if (gameanimsysprogress.entries != NULL) {
        for (i32 i = 0; i < level_count; ++i)
            gameanimsysprogress.entries[i] = static_cast<u8 *>(GameBufferAlloc(buf, buf_end, capacity));
    }
}

u8 *GameAnimSys_GetProgressData(i32 index) {
    if (index < 0 || index >= gameanimsysprogress.count)
        return NULL;
    return gameanimsysprogress.entries[index];
}

void GameAnimSys_StoreProgress(GAMEANIMSYS_s *system, i32 index) {
    if (system == NULL || system->sets == NULL || index < 0 || index >= gameanimsysprogress.count)
        return;
    u8 *progress = gameanimsysprogress.entries[index];
    for (i32 i = 0; i < gameanimsysprogress.entry_size && system->sets[i] != NULL; ++i)
        progress[i] = system->sets[i]->state;
}

void GameAnimSys_ReStoreProgress(GAMEANIMSYS_s *system, i32 index) {
    if (system == NULL || system->sets == NULL || index < 0 || index >= gameanimsysprogress.count)
        return;
    u8 *progress = gameanimsysprogress.entries[index];
    for (i32 i = 0; i < gameanimsysprogress.entry_size && system->sets[i] != NULL; ++i) {
        GAMEANIMSET_s *set = system->sets[i];
        set->state = static_cast<GAMEANIMSET_STATE>(static_cast<i8>(progress[i]));
        if (set->state == GAMEANIMSET_STATE_ACTIVE_FORWARD || set->state == GAMEANIMSET_STATE_ACTIVE_BACKWARD) {
            if ((set->flags & GAMEANIMSET_FLAG_IN_SYSTEM_LIST) == 0)
                GameAnimSet_AddToSystemList(set);
        } else if ((set->flags & GAMEANIMSET_FLAG_IN_SYSTEM_LIST) != 0)
            GameAnimSet_RemoveFromSystemList(set);
    }
}

GAMEANIMSYS_s *GameAnimSys_Create(variptr_u *buf, variptr_u *buf_end) {
    GAMEANIMSYS_s *system = static_cast<GAMEANIMSYS_s *>(GameBufferAlloc(buf, buf_end, sizeof(GAMEANIMSYS_s)));
    if (system != NULL && gameanimsysprogress.entry_size != 0) {
        system->sets = static_cast<GAMEANIMSET_s **>(
            GameBufferAlloc(buf, buf_end, gameanimsysprogress.entry_size * sizeof(GAMEANIMSET_s *)));
    }
    return system;
}
