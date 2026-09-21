#include "decomp.h"
#include "gameapi/gui/apimenu.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include <math.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern void *CutStopInfo;
extern "C" {
    extern i32 Paused;
    extern i32 NewMode;
    extern i32 CutSceneWaiting;
    extern i32 editor_active;
    extern i32 (*GamePads_IgnoreInputFn)(void);
    extern GAMEPAD_s GamePad[64];
    extern FadeSystem FadeSys;
}

// Original: 43 bytes.
i32 CircleLevel(LEVELDATA_s *level) {
    return BONUS_GUNSHIPB_LDATA != NULL && level == BONUS_GUNSHIPB_LDATA;
}

extern i32 TwistLevel(LEVELDATA_s *level);

void CurrentStart(GameObject_s *object, i32 require_twist_level, i32 use_socket_rotation) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    object->field_0xc3c = 0;
    if (object->field_0x661 == 0xff || world->sock_sys == NULL)
        return;

    SOCK &socket = world->sock_sys->sock[static_cast<i8>(object->field_0x661)];
    if (socket.current_speed == 0.0f || (require_twist_level == 0 && TwistLevel(world->current_level) == 0))
        return;

    NUVEC current = {0.0f, 0.0f, socket.current_speed};
    *reinterpret_cast<f32 *>(&object->field_0xc3c) = socket.current_speed;
    const f32 multiplier =
        (object->field_0xf02 & 0x20) == 0 ? object->current_speed_mul : object->current_speed_multiplier;
    current.z *= multiplier;
    if (use_socket_rotation == 0) {
        NuVecRotateY(&object->apiobj.velocity, &object->apiobj.velocity, object->apiobj.movement_facing_angle);
    } else {
        NuVecRotateX(&object->apiobj.velocity, &current, object->sock_position.midpoint_rotation.x);
        NuVecRotateY(&object->apiobj.velocity, &object->apiobj.velocity, object->sock_position.midpoint_rotation.y);
    }
    reinterpret_cast<u8 *>(object)[0x4a6] &= static_cast<u8>(~4u);
    object->target_velocity = object->apiobj.velocity;
}

void NewRumbleAllPlayers(f32, f32, i32, i32);

void ConstantRumble(GameObject_s *object, float strength, float phase) {
    phase = NuFmod(phase + GameTimer.time_elapsed, 1.25f);
    f32 weight = 0.0f;
    if (phase < 1.0f) {
        i32 angle = static_cast<i32>(phase * 32768.0f + 16384.0f);
        weight = 1.0f - fabsf(NuTrigTable[(angle >> 1) & 0x7fff]);
    }
    strength = weight * strength;
    if (object == NULL) {
        NewRumbleAllPlayers(strength, 0.0f, 0, 0);
    } else if ((object->apiobj.flags_low & 0x80) != 0) {
        NewRumble(object->pad_gamepad->pad, strength, 0);
    }
}

void ClearLastSafeTakeOver(GameObject_s *object) {
    if (object == NULL || (object->field_0xefa & 0x10) != 0) {
        return;
    }
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
        GameObject_s *candidate = &Obj[i];
        if (candidate != NULL && (candidate->apiobj.field_0x1f8 & 0x1001) == 0x1001 &&
            candidate->takeover_source == object) {
            candidate->takeover_source = NULL;
        }
    }
}

void GetNativeTextureFormatName(NUTEXFORMAT) {
    STUBBED();
}

void CatIToX(char *, i32) {
    STUBBED();
}

void DoInput(WORLDINFO_s *world) {
    if (world == NULL) {
        world = WorldInfo_CurrentlyActive();
    }

    const i32 player_0_input = ReadPad(0);
    const i32 player_1_input = ReadPad(1);
    const i32 player_state_changed = PlayersDropInOut();

    for (i32 player_index = 0; player_index < 2; ++player_index) {
        const i32 input_result = player_index == 0 ? player_0_input : player_1_input;
        if (GamePads_IgnoreInputFn != NULL && GamePads_IgnoreInputFn() != 0) {
            continue;
        }
        if (input_result <= 1 || player_state_changed != 0) {
            continue;
        }

        GameObject_s *player = Player[player_index];
        if (player == NULL || static_cast<i8>(player->apiobj.field_0x1f8) >= 0 ||
            (LEGOCONTEXT_DROPIN != -1 && static_cast<i8>(player->field_0x7a5) == LEGOCONTEXT_DROPIN) ||
            (GamePad[player_index].buttons_pressed & GAMEPAD_START) == 0) {
            continue;
        }
        if (NewMode != 0 || NewLData != NULL || FadeSys.fade != 0.0f || editor_active != 0 ||
            GameTimer.time_elapsed <= 0.0f || world == NULL || world->current_level == NULL ||
            world->current_level == TITLES_LDATA) {
            continue;
        }

        const bool player_can_resume = pause_i_pad == -1 || pause_i_pad == player_index;
        if (Paused != 0 || (GameMenu[GameMenuLevel].menu != -1 && NetPaused != 0)) {
            if (player_can_resume) {
                ResumeGame(1, 1);
                RestoreOptions();
            }
            continue;
        }
        if (GameMenu[GameMenuLevel].menu != -1 || CutSceneWaiting != 0 || MiniCutCam != 0 ||
            memcard_autosavestarted != 0 || memcard_autosavepostdelay > 0.0f || memcard_autosavepredelay > 0.0f ||
            GameTimer.update_count == 0) {
            continue;
        }
        if (CUTSTOPGAME != 0 && !CutScene_IsSkippable(static_cast<CUTINFO *>(CutStopInfo))) {
            continue;
        }

        PauseGame(static_cast<i32>(player->pad_gamepad - GamePad));
    }
}

void CatI64ToX(char *, i64) {
    STUBBED();
}

void DieRumble(GameObject_s *) {
    STUBBED();
}

void charToInt(char const *) {
    STUBBED();
}

static __used__ i32 _fseek64_wrap(__sFILE *, i64, i32) {
    STUBBED();
    return 0;
}
