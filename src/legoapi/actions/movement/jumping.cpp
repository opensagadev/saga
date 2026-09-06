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
i32 LEGOCONTEXT_BACKFLIP = -1;
i16 LEGOACT_JUMP = -1;
i16 LEGOACT_JUMP2 = -1;
i16 LEGOACT_FLIP = -1;
i16 LEGOACT_BACKFLIP = -1;
i16 LEGOACT_EXTRA_JUMP = -1;
i16 LEGOACT_EXTRA_JUMP2 = -1;
i16 LEGOACT_MAGNET_JUMP = -1;
i16 LEGOACT_FALL = -1;
i32 (*Jump_PreventJumpFn)(GameObject_s *) = NULL;
i32 (*CanMagnetClimbFn)(GameObject_s *) = NULL;
i32 (*CanGlideFn)(GameObject_s *) = NULL;
i32 DoubleJump_AlwaysReachJump2Height = 0;
i32 LEGOCONTEXT_GLIDE = -1;
i16 LEGOACT_JUMP3 = -1;
i16 LEGOACT_COMBATROLL_JUMP = -1;
i16 LEGOACT_COMBATROLL_FALL = -1;
i16 LEGOACT_COMBATROLL_LAND = -1;
i16 LEGOACT_COMBATROLL_FIRE = -1;
i16 LEGOACT_LUNGE = 1;
i16 LEGOACT_LAND3 = -1;
i16 LEGOACT_EXTRA_LAND = -1;
i16 LEGOACT_FLIPLAND = -1;
i16 LEGOACT_COMBOLAND = -1;
i32 LEGOCONTEXT_LAND_JUMP2 = -1;
i32 LEGOCONTEXT_LAND_FLIP = -1;
i32 LEGOCONTEXT_LAND_COMBOJUMP = -1;
i32 LEGOCONTEXT_LAND_LUNGE = -1;
i16 LEGOACT_LUNGELAND = -1;
i32 LEGOCONTEXT_LAND_SLAM = -1;
i16 LEGOACT_SLAMLAND = -1;
i32 (*Slam_GetDebrisFn)(GameObject_s *, i32) = NULL;

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
        if (IsWearingBackPackFn != NULL && IsWearingBackPackFn(object) && LEGOACT_BACKPACKFALLLAND != -1 &&
            animations[LEGOACT_BACKPACKFALLLAND] != NULL) {
            object->context_animation = LEGOACT_BACKPACKFALLLAND;
        } else if (UsingExtraActionsFn != NULL && UsingExtraActionsFn(object) && LEGOACT_EXTRA_LAND2 != -1 &&
                   animations[LEGOACT_EXTRA_LAND2] != NULL) {
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
    object->airborne_collision_target = NULL;
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
    object->airborne_collision_target = NULL;
    object->apiobj.velocity.y = game_character->jump_speed;
    ResetAnimPacket(&object->apiobj.anim_packet, -1);
    object->apiobj.pitch_angle = 0;
    object->apiobj.roll_angle = 0;
    object->field_0x1086 = 2;
}
