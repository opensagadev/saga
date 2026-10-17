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
#include "legoapi/misc/utilities.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include <math.h>
#include <string.h>

struct AIROW_s;
struct nunativegscene_s;

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

void NewRumbleAllPlayers(f32, f32, i32, i32);

void ConstantRumble(GameObject_s *object, float strength, float phase) {
    phase = NuFmod(phase + GameTimer.time_elapsed, 1.25f);
    f32 weight = 0.0f;
    if (phase < 1.0f) {
        i32 angle = static_cast<i32>(phase * 32768.0f + 16384.0f);
        weight = 1.0f - fabsf(NuTrigTable[(angle >> 1) & 0x7fff]);
    }
    strength = weight * strength;
    if (object != NULL) {
        if (object->apiobj.player_controlled) {
            NewRumble(object->pad_gamepad->pad, strength, 0);
        }
    } else {
        NewRumbleAllPlayers(strength, 0.0f, 0, 0);
    }
}

void CatIToX(char *output, i32 value) {
    while (*output != '\0')
        ++output;
    *IToX(output, value) = '\0';
}

void DoInput(WORLDINFO_s *world) {
    if (world == NULL) {
        world = WorldInfo_CurrentlyActive();
    }

    const i32 player_0_input = ReadPad(0);
    const i32 player_1_input = ReadPad(1);
    i32 player_state_changed = PlayersDropInOut();

    for (i32 player_index = 0; player_index < 2; ++player_index) {
        nupad_s *pad = GamePad[player_index].pad;
        GameObject_s *player = Player[player_index];
        const i32 input_result = player_index == 0 ? player_0_input : player_1_input;
        if (GamePads_IgnoreInputFn != NULL && GamePads_IgnoreInputFn() != 0) {
            continue;
        }
        if (static_cast<u32>(input_result) <= 1) {
            if (player != NULL && player->apiobj.player_controlled && pad != NULL && MiniCutCam == 0 && world != NULL &&
                world->current_level != NULL) {
                PadOutPause(player_index, world);
            }
            continue;
        }
        if (player_state_changed != 0) {
            continue;
        }

        if (player == NULL || !player->apiobj.player_controlled ||
            (LEGOCONTEXT_DROPIN != -1 && static_cast<i8>(player->field_0x7a5) == LEGOCONTEXT_DROPIN) || pad == NULL ||
            (pad->digital_buttons_pressed & GAMEPAD_START) == 0) {
            continue;
        }
        if (NewMode != 0 || NewLData != NULL || FadeSys.fade != 0.0f || editor_active != 0 ||
            !(GameTimer.time_elapsed > 0.0f) || world == NULL || world->current_level == NULL ||
            world->current_level == TITLES_LDATA) {
            continue;
        }

        if (Paused == 0) {
            if (GameMenu[GameMenuLevel].menu == -1 && CutSceneWaiting == 0) {
                if ((CUTSTOPGAME == 0 || CutScene_IsSkippable(static_cast<CUTINFO *>(CutStopInfo))) &&
                    MiniCutCam == 0 && memcard_autosavestarted == 0 && !(memcard_autosavepostdelay > 0.0f) &&
                    !(memcard_autosavepredelay > 0.0f) && GameTimer.update_count != 0) {
                    PauseGame(static_cast<i32>(player->pad_gamepad - GamePad));
                    player_state_changed = 1;
                    continue;
                }
                if (Paused != 0) {
                    goto resume;
                }
            }
            if (NetPaused == 0) {
                continue;
            }
        }
    resume:
        if (pause_i_pad == -1 || pause_i_pad == player_index) {
            ResumeGame(1, 1);
            RestoreOptions();
        }
    }
}

void CatI64ToX(char *output, i64 value) {
    while (*output != '\0')
        ++output;
    *I64ToX(output, value) = '\0';
}

void DieRumble(GameObject_s *object) {
    if (object != NULL && object->apiobj.player_controlled) {
        NewRumble(object->pad_gamepad->pad, 1.0f, 0);
        NewBuzz(object->pad_gamepad->pad, 0.3f, 0);
    }
}

i32 charToInt(char const *text) {
    const i32 length = static_cast<i32>(strlen(text));
    u32 sign = 1;
    i32 first = 0;
    if (length > 0 && text[0] == '-') {
        first = 1;
        sign = static_cast<u32>(-1);
    }
    // The original accepts signed non-digit bytes and wraps at 32 bits.
    u32 result = 0;
    u32 place = 1;
    for (i32 index = static_cast<i32>(static_cast<u32>(length) - 1u); index >= first; --index) {
        result += static_cast<u32>(static_cast<i8>(text[index]) - '0') * place;
        place *= 10;
    }
    return static_cast<i32>(result * sign);
}
