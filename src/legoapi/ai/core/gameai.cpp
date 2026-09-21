#include "decomp.h"
#include "globals.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/numtx.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/props/objects/tightrope.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

struct ADAPTIVEDIFFICULTY_s {
    i32 difficulty;
    f32 multiplier;
    f32 elapsed;
};

extern i32 adaptivedifficulty[3];

i8 adtabentries[9][4] = {{-1, -1, -1, -1}, {-1, -1, -1, 0}, {-1, -1, 0, 0}, {-1, 0, 0, 0}, {0, 0, 0, 0},
                         {1, 0, 0, 0},     {1, 1, 0, 0},    {1, 1, 1, 0},   {1, 1, 1, 1}};
i8 (*adtab)[4] = &adtabentries[4];

extern void InitSurfaceInfo(GameObject_s *);
extern i32 SetObjOnSurface(GameObject_s *, i32);
extern void Player_ClearContext(GameObject_s *, i32);
extern void Player_ResetContexts(PLAYERPACKET_s *);
extern f32 LOOPTIME;
extern void StartTurn(GameObject_s *object);
extern i32 TwistLevel(LEVELDATA_s *level);
extern GameObject_s *GetOtherActivePlayer(GameObject_s *object);
extern i32 GoingForwardsAlongNarrowSock(GameObject_s *object);

i32 CheckPosAIArea(AIAREA_s *area, nuvec_s *position, float tolerance) {
    if (position == NULL || area == NULL) {
        return 0;
    }

    NUVEC local_position;
    NuVecSub(&local_position, position, &area->position);
    NuVecRotateY(&local_position, &local_position, -area->rotation);

    return local_position.x + tolerance >= -area->half_width && local_position.y + tolerance >= -0.1f &&
           local_position.z + tolerance >= -area->half_depth && local_position.x - tolerance <= area->half_width &&
           local_position.y - tolerance <= area->height && local_position.z - tolerance <= area->half_depth;
}

void ResetAdaptiveDifficulty() {
    ADAPTIVEDIFFICULTY_s *difficulty = (ADAPTIVEDIFFICULTY_s *)adaptivedifficulty;
    difficulty->multiplier = 0.5f;
    difficulty->elapsed = 0.0f;
    difficulty->difficulty = -4;
}

void LoopCode(GameObject_s *object, i32 jump_pressed, i32, GAMEPAD_s *pad, i32 allow_loop) {
    if (WORLD->area != NULL) {
        if (WORLD->area == DOGFIGHT_ADATA) {
            allow_loop = 0;
        } else if (WORLD->area == PODSPRINT_ADATA) {
            return;
        }
    }

    if (object->character_context == 0x3a) {
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer <= 0.0f) {
            object->character_context = -1;
        }
        return;
    }

    if (object->character_context == 0x36) {
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer <= 0.0f) {
            object->character_context = -1;
            object->secondary_lean_angle = 0;
            object->delayed_turn_timer = 0.3f;
        } else {
            const f32 phase = object->context_animation_timer / object->airborne_action_duration;
            const i32 index = static_cast<i32>((1.0f - phase) * -65536.0f + 16384.0f);
            const f32 curve = (NuTrigTable[(index >> 1) & 0x7fff] + 1.0f) * 0.5f;
            object->secondary_lean_angle = static_cast<i16>((1.0f - curve) * 65536.0f);
        }
        return;
    }

    if (object->apiobj.character_data->game_character->field_0x88 <= 0.0f || object->character_context != -1) {
        return;
    }

    GameObject_s *other = GetOtherActivePlayer(object);
    if (allow_loop != 0 && object->in_narrow_socket != 0 && other != NULL && other->in_narrow_socket != 0 &&
        other->character_context == 0x36 && other->context_animation_timer < other->airborne_action_duration - 0.2f &&
        other->context_animation_timer > 0.3f &&
        GoingForwardsAlongNarrowSock(other) == GoingForwardsAlongNarrowSock(object)) {
        goto start_loop;
    }

    if (object->delayed_turn_timer > 0.0f) {
        return;
    }

    i32 angle_difference;
    if (TwistLevel(WORLD->current_level) != 0) {
        angle_difference = RotDiff(0, pad->input_angle);
    } else {
        angle_difference = RotDiff(object->apiobj.field_0x276, GamePad_InputAngle(object, pad));
    }

    if (object->movement_spline == NULL && pad->input_magnitude > 0.0f) {
        i32 roll_direction = 0;
        if (static_cast<u32>(angle_difference + 0x5fff) <= 0x58e2) {
            roll_direction = 2;
        } else if (static_cast<u32>(angle_difference - 0x71d) <= 0x58e2) {
            roll_direction = 1;
        }

        if (roll_direction != 0) {
            if (jump_pressed == 0) {
                return;
            }
            if (object->in_narrow_socket != 0 && other != NULL &&
                (other->character_context == 0x36 || other->character_context == 0x2a)) {
                return;
            }

            object->airborne_action_duration = 0.75f;
            object->context_animation_timer = 0.75f;
            object->character_context = 0x3a;
            object->field_0x7a3 = static_cast<u8>(roll_direction - 1);
            object->context_animation = 1;
            PlaySfx("XWing_LoopDeLoop", &object->apiobj.collision_position);
            if (static_cast<i8>(object->apiobj.flags_low) < 0) {
                Hint_SetComplete(0x617);
            }
            return;
        }

        const i32 absolute_difference = angle_difference < 0 ? -angle_difference : angle_difference;
        if (absolute_difference > 0x3fff) {
            if (allow_loop == 0 || jump_pressed == 0) {
                return;
            }
            if (object->in_narrow_socket != 0 && other != NULL &&
                (other->character_context == 0x36 || other->character_context == 0x2a ||
                 other->character_context == 0x3a)) {
                return;
            }
            StartTurn(object);
            if (static_cast<i8>(object->apiobj.flags_low) < 0) {
                Hint_SetComplete(0x617);
            }
            return;
        }
    }

    if (allow_loop == 0 || jump_pressed == 0) {
        return;
    }
    if (object->in_narrow_socket != 0 && other != NULL &&
        (other->character_context == 0x36 || other->character_context == 0x2a || other->character_context == 0x3a)) {
        return;
    }

start_loop:
    object->character_context = 0x36;
    object->context_animation = 1;
    object->airborne_action_duration = LOOPTIME;
    object->context_animation_timer = LOOPTIME;
    PlaySfx("XWing_LoopDeLoop", &object->apiobj.collision_position);
    Hint_SetComplete(0x287);
    if (static_cast<i8>(object->apiobj.flags_low) < 0) {
        Hint_SetComplete(0x617);
    }
}
