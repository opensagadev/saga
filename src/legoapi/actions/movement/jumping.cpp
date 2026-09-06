#include "decomp.h"
#include "globals.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;
void StartLunge(GameObject_s *, f32, f32);
i32 Slam_Start(GameObject_s *, f32);
void StartHold(GameObject_s *);
void ComboHitFrame(GameObject_s *, i32);
extern "C" f32 AnimDuration(i32, i32, f32, f32, i32);
void PlaySabreSfx(char *, GameObject_s *, NUVEC *, i32);
i32 DoubleJump_JediSlam = 0;
f32 SLAMJUMPSPEED = 3.0f;
bool (*IsWearingBackPackFn)(GameObject_s *) = NULL;
i32 LEGOCONTEXT_LAND_JUMP = -1;
i16 LEGOACT_LAND = -1;
i16 LEGOACT_LAND2 = -1;
i16 LEGOACT_FALLLAND = -1;
i16 LEGOACT_BACKPACKFALLLAND = -1;
i16 LEGOACT_EXTRA_LAND2 = -1;

enum PLAYER_JUMP_RUNTIME_FLAGS : u8 {
    PLAYER_JUMP_RUNTIME_BUTTON_HELD = 0x10,
};

enum PLAYER_JUMP_INPUT_FLAGS : u8 {
    PLAYER_JUMP_INPUT_BUFFERED = 0x10,
};

enum PLAYER_JUMP_FLAGS : u8 {
    PLAYER_JUMP_FLAG_SPECIAL_TAKEOFF = 0x01,
};

enum PLAYER_JUMP_CONTEXT_FLAGS : u32 {
    PLAYER_JUMP_CONTEXT_ALLOW_START = 0x00001000,
};

enum PLAYER_JUMP_VARIANT_FLAGS : u8 {
    PLAYER_JUMP_VARIANT_BUTTON_RELEASED = 0x10,
    PLAYER_JUMP_VARIANT_SECOND_JUMP = 0x40,
    PLAYER_JUMP_VARIANT_FALLING = 0x80,
    PLAYER_JUMP_VARIANT_START_CLEAR = 0x90,
    PLAYER_JUMP_VARIANT_END_CLEAR = 0x50,
};

enum PLAYER_MOVEMENT_RUNTIME_FLAGS : u8 {
    PLAYER_MOVEMENT_RUNTIME_DISABLE_JUMP_CODE = 0x10,
};

enum PLAYER_JUMP_ANIMATION_FLAGS : u32 {
    PLAYER_JUMP_ANIMATION_ALLOW_DOUBLE_JUMP = 0x0008,
    PLAYER_JUMP_ANIMATION_USE_THIRD_JUMP = 0x0010,
};

static const f32 PLAYER_JUMP_MINIMUM_AIR_TIME = 0.1f;
static const f32 PLAYER_JUMP_REENTRY_DELAY = 0.2f;

void PlayJumpSfx(GameObject_s *object, i32 variant);
void PlayLandSfx(GameObject_s *object, i32 variant, i32 force);

void StartJump(GameObject_s *object, i32 movement_state);

static GAMECHARACTERDATA *Jump_GetCharacterData(GameObject_s *object) {
    if (object == NULL || object->apiobj.character_data == NULL) {
        return NULL;
    }
    return static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
}

static bool Jump_HasAction(const GameObject_s *object, PLAYER_JUMP_ACTION action) {
    return object != NULL && object->apiobj.character_model != NULL &&
           object->apiobj.character_model->model_data_b != NULL &&
           object->apiobj.character_model->model_data_b[action] != NULL;
}

void BigJumpCode(GameObject_s *) {
}

bool UseFallAnim(GameObject_s *object) {
    const CHARACTER_CONTEXT_INFO_s &context = CInfo[object->character_context];
    return (context.flags & CHARACTER_CONTEXT_INFO_FLAG_USE_FALL_ANIMATION) != 0 &&
           Jump_HasAction(object, PLAYER_JUMP_ACTION_FALL);
}

void StartBigJump(GameObject_s *, nuvec_s *, i32, float, float, i32, signed char) {
}

i32 StartFallLand(GameObject_s *object, i32 action) {
    PlayLandSfx(object, 0, 0);
    if (LEGOCONTEXT_LAND_JUMP == -1) {
        object->movement_runtime_flags &= ~4;
        return 0;
    }
    void **animations = object->apiobj.character_model->model_data_b;
    if (action == -1 || animations[action] == NULL) {
        if (IsWearingBackPackFn != NULL && IsWearingBackPackFn(object) &&
            LEGOACT_BACKPACKFALLLAND != -1 && animations[LEGOACT_BACKPACKFALLLAND] != NULL) {
            object->context_animation = LEGOACT_BACKPACKFALLLAND;
        } else if (UsingExtraActionsFn != NULL && UsingExtraActionsFn(object) &&
                   LEGOACT_EXTRA_LAND2 != -1 && animations[LEGOACT_EXTRA_LAND2] != NULL) {
            object->context_animation = LEGOACT_EXTRA_LAND2;
        } else if (LEGOACT_FALLLAND != -1 && animations[LEGOACT_FALLLAND] != NULL) {
            object->context_animation = LEGOACT_FALLLAND;
        } else if (LEGOACT_LAND2 != -1 && animations[LEGOACT_LAND2] != NULL) {
            object->context_animation = LEGOACT_LAND2;
        } else {
            object->context_animation = LEGOACT_LAND;
            if (animations[object->context_animation] == NULL) {
                object->movement_runtime_flags &= ~4;
                return 0;
            }
        }
    } else {
        object->context_animation = action;
        if (animations[object->context_animation] == NULL) {
            object->movement_runtime_flags &= ~4;
            return 0;
        }
    }
    object->character_context = LEGOCONTEXT_LAND_JUMP;
    ResetAnimPacket(&object->apiobj.anim_packet, -1);
    ResetMiniAnimPacket(&object->mini_animation, -1);
    object->fall_animation_timer = 0.0f;
    object->context_animation_timer = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1);
    object->movement_runtime_flags =
        static_cast<u8>((object->movement_runtime_flags & ~0x08) | ((object->movement_runtime_flags & 0x04) << 1));
    return 1;
}

void StartEndOfJump(GameObject_s *object) {
    if (object == NULL) {
        return;
    }

    object->character_context = CHARACTER_CONTEXT_JUMP;
    object->action_movement_state = PLAYER_JUMP_MOVEMENT_BASIC;
    object->jump_sequence = 2;
    object->jump_flags &= ~PLAYER_JUMP_FLAG_SPECIAL_TAKEOFF;
    object->context_animation = PLAYER_JUMP_ACTION_FALL;
    object->context_variant_flags =
        static_cast<i8>((static_cast<u8>(object->context_variant_flags) | PLAYER_JUMP_VARIANT_FALLING) &
                        ~PLAYER_JUMP_VARIANT_END_CLEAR);
    object->airborne_reset_timer = 0.0f;
}

void StartBallooning(GameObject_s *, i32) {
}

void StartJetPackFall(GameObject_s *, i32) {
}

void MakeJumpReachHeight(GameObject_s *object, float height, i32 force) {
    const f32 remaining_height = height - (object->apiobj.position.y - object->jump_start_height);
    if (remaining_height > 0.0f) {
        const GAMECHARACTERDATA *game_character =
            static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
        const f32 vertical_speed = NuFsqrt(-2.0f * game_character->gravity * remaining_height);
        if (force != 0 || vertical_speed > object->apiobj.velocity.y) {
            object->apiobj.velocity.y = vertical_speed;
        }
    } else if (force != 0) {
        object->apiobj.velocity.y = 0.0f;
    }
}

void SetBallooningHeight(GameObject_s *, float) {
}

void JumpCode(GameObject_s *object, i32 jump_pressed, i32 jump_held, u32 animation_set, i32 action_pressed, i32 action_held, i32) {
    if (object == NULL || (object->movement_runtime_flags & PLAYER_MOVEMENT_RUNTIME_DISABLE_JUMP_CODE) != 0) {
        return;
    }

    if (object->character_context != CHARACTER_CONTEXT_JUMP) {
        const bool has_ground_contact = object->apiobj.field_0x27d != 0 || object->ground_contact_grace_timer > 0.0f;
        const bool context_allows_jump =
            (CInfo[object->character_context].flags & PLAYER_JUMP_CONTEXT_ALLOW_START) != 0;
        if (jump_pressed != 0 && has_ground_contact && context_allows_jump) {
            const PLAYER_JUMP_MOVEMENT_STATE movement_state =
                (animation_set & 0x04) != 0 ? PLAYER_JUMP_MOVEMENT_ORDINARY : PLAYER_JUMP_MOVEMENT_BASIC;
            StartJump(object, movement_state);
        }
    } else {
        if (jump_held == 0) {
            object->field_0xe22 &= ~PLAYER_JUMP_RUNTIME_BUTTON_HELD;
        }

        if (object->apiobj.velocity.y > 0.0f) {
            object->airborne_reset_timer = 0.0f;
        }
        object->context_animation_timer += FRAMETIME;

        GAMECHARACTERDATA *game_character = Jump_GetCharacterData(object);
        if (action_pressed != 0 && object->apiobj.velocity.y > -1.25f && object->context_variant_flags >= 0) {
            if (object->action_movement_state == 0 && object->jump_sequence < 2 &&
                (game_character->field275_0x116 != 0 || (object->apiobj.character_data->model_flags & 8) != 0) &&
                Jump_HasAction(object, static_cast<PLAYER_JUMP_ACTION>(0x1f))) {
                object->field_0x780 = NULL;
                object->blowup_target = NULL;
                StartLunge(object, 0.0f, object->apiobj.collision_height);
                return;
            }
            if (DoubleJump_JediSlam != 0 && (animation_set & 0x10) != 0 &&
                ((object->action_movement_state == 0 &&
                  (object->jump_sequence == 2 || (game_character->field_0x98 & 0x20) != 0)) ||
                 object->action_movement_state == 1 || object->action_movement_state == 2) &&
                Jump_HasAction(object, static_cast<PLAYER_JUMP_ACTION>(0x21))) {
                if (Slam_Start(object, SLAMJUMPSPEED) != 0) {
                    PlaySabreSfx(NULL, object, NULL, 0);
                    return;
                }
            }
        }
        const bool buffered_second_jump = (object->jump_input_flags & PLAYER_JUMP_INPUT_BUFFERED) != 0;
        const bool can_start_second_jump = (jump_pressed != 0 || buffered_second_jump) &&
                                           (animation_set & PLAYER_JUMP_ANIMATION_ALLOW_DOUBLE_JUMP) != 0 &&
                                           object->action_movement_state == PLAYER_JUMP_MOVEMENT_BASIC &&
                                           object->jump_sequence <= 1 &&
                                           (object->apiobj.velocity.y > -1.25f || buffered_second_jump);
        if (can_start_second_jump && game_character != NULL) {
            MakeJumpReachHeight(object, game_character->second_jump_height, 0);
            object->jump_sequence++;
            object->context_variant_flags |= PLAYER_JUMP_VARIANT_SECOND_JUMP;
            object->action_movement_state = PLAYER_JUMP_MOVEMENT_BASIC;
            PLAYER_JUMP_ACTION second_jump_action = PLAYER_JUMP_ACTION_JUMP;
            if ((animation_set & PLAYER_JUMP_ANIMATION_USE_THIRD_JUMP) != 0 &&
                Jump_HasAction(object, PLAYER_JUMP_ACTION_THIRD_JUMP)) {
                second_jump_action = PLAYER_JUMP_ACTION_THIRD_JUMP;
            } else if (Jump_HasAction(object, PLAYER_JUMP_ACTION_SECOND_JUMP)) {
                second_jump_action = PLAYER_JUMP_ACTION_SECOND_JUMP;
            }
            object->context_animation = second_jump_action;
            object->context_animation_timer = 0.0f;
            object->field_0xe22 |= PLAYER_JUMP_RUNTIME_BUTTON_HELD;
            PlayJumpSfx(object, 1);
            if ((animation_set & PLAYER_JUMP_ANIMATION_USE_THIRD_JUMP) == 0) {
                object->context_variant_flags |= PLAYER_JUMP_VARIANT_BUTTON_RELEASED;
            }
            return;
        }

        if ((object->apiobj.field_0x27d != 0 && object->context_animation_timer >= PLAYER_JUMP_MINIMUM_AIR_TIME) ||
            ((object->action_movement_state == 3 || object->action_movement_state == 4) && object->context_animation_timer >= 2.5f)) {
            if (object->action_movement_state == 3 || object->action_movement_state == 4) {
                const bool slam = object->action_movement_state == 4;
                const i32 action = slam ? 0x22 : 0x20;
                if (Jump_HasAction(object, static_cast<PLAYER_JUMP_ACTION>(action))) {
                    object->character_context = slam ? 14 : 13;
                    object->context_animation = action;
                    object->context_animation_timer = AnimDuration(object->id, action, 0.0f, 0.0f, 1);
                    if (slam && object->context_animation_timer <= 0.0f) object->context_animation_timer = 0.75f;
                    object->context_flags &= ~0x40;
                    object->jump_reentry_timer = 0.0f;
                    ResetAnimPacket(&object->apiobj.anim_packet, -1);
                } else {
                    object->character_context = -1;
                    object->jump_reentry_timer = 0.2f;
                }
                PlayLandSfx(object, slam ? 2 : 1, 0);
            } else if (object->action_movement_state == 2 &&
                       Jump_HasAction(object, static_cast<PLAYER_JUMP_ACTION>(0x13))) {
                if ((object->context_variant_flags & 0x20) == 0) {
                    object->apiobj.field_0x276 += 0x8000;
                    object->apiobj.facing_angle += 0x8000;
                    object->apiobj.movement_facing_angle += 0x8000;
                }
                object->jump_reentry_timer = 0.0f;
                if (object->pad_gamepad->input_magnitude == 0.0f) {
                    object->character_context = 4;
                    object->context_animation = (object->context_variant_flags & 0x20) != 0 &&
                        Jump_HasAction(object, static_cast<PLAYER_JUMP_ACTION>(0x0d)) ? 0x0d : 0x13;
                    object->context_animation_timer = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1);
                } else {
                    object->character_context = -1;
                }
                PlayLandSfx(object, 0, 0);
            } else if (object->action_movement_state == PLAYER_JUMP_MOVEMENT_ORDINARY) {
                object->character_context = CHARACTER_CONTEXT_NONE;
                object->jump_reentry_timer = PLAYER_JUMP_REENTRY_DELAY;
                object->jump_chain_timer = 0.0f;
                PlayLandSfx(object, 0, 0);
            } else {
                StartFallLand(object, -1);
            }
        }
        return;
    }

    if (object->character_context == CHARACTER_CONTEXT_LAND_JUMP || object->character_context == 4 ||
        object->character_context == 13 || object->character_context == 14) {
        if (object->character_context == 13 && (object->context_flags & 0x40) == 0) {
            const f32 frame = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
            f32 *time = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
            if (frame > 0.0f && time != NULL && *time >= frame) ComboHitFrame(object, 1);
        }
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer <= 0.0f) {
            const bool attack_landing = object->character_context == 13 || object->character_context == 14;
            if (object->character_context == 13 && (object->context_flags & 0x40) == 0) ComboHitFrame(object, 1);
            object->character_context = CHARACTER_CONTEXT_NONE;
            if (attack_landing && action_held != 0) StartHold(object);
        }
    }
}

void StartJump(GameObject_s *object, i32 movement_state) {
    GAMECHARACTERDATA *game_character = Jump_GetCharacterData(object);
    if (object == NULL || game_character == NULL) {
        return;
    }

    object->character_context = CHARACTER_CONTEXT_JUMP;
    object->action_movement_state = static_cast<u8>(movement_state);
    object->jump_sequence = 1;
    object->jump_start_height = object->apiobj.position.y;
    object->context_animation_timer = 0.0f;
    object->context_animation = PLAYER_JUMP_ACTION_JUMP;
    object->jump_flags =
        static_cast<u8>((object->jump_flags & ~PLAYER_JUMP_FLAG_SPECIAL_TAKEOFF) |
                        ((movement_state == 3 || movement_state == 4) ? PLAYER_JUMP_FLAG_SPECIAL_TAKEOFF : 0));
    object->context_variant_flags =
        static_cast<i8>(static_cast<u8>(object->context_variant_flags) & ~PLAYER_JUMP_VARIANT_START_CLEAR);
    object->airborne_action_timer = 0.0f;
    if (movement_state != 6 && movement_state != 7) {
        PlayJumpSfx(object, 0);
    }
    object->apiobj.field_0x27d = 0;
    object->field_0x105c = 0;
    object->field_0xe22 |= PLAYER_JUMP_RUNTIME_BUTTON_HELD;
    object->delayed_turn_timer = 0.0f;
    object->airborne_input_timer = 0.0f;
    object->airborne_reset_timer = 0.0f;
    object->apiobj.velocity.y = game_character->jump_speed;
    ResetAnimPacket(&object->apiobj.anim_packet, -1);
    object->apiobj.pitch_angle = 0;
    object->apiobj.roll_angle = 0;
    object->field_0x1086 = 2;
}
