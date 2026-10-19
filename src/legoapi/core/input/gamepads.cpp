// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/misc/androidbatman.h"
#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/numusic/numusic.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nupad.h"

#include <string.h>
#include "gameframework/saveload.h"
#include "legoapi/menus/screens/store.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include <math.h>
#include "legoapi/core/config/cheat.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/items/base/apiobject.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/numath/nuang.h"
#include "legoapi/audio/sfx.h"
#include <stdio.h>
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "nu2api/nusound/nusound.h"
#include "nu2api/nusound/nusound3_include.h"

extern i32 (*GamePads_IgnoreInputFn)(void);
extern "C" {
    extern "C++" FadeSystem FadeSys;
    extern i32 CutSceneWaiting;
}

// Original bss @0x127a500: 64 pads x 0x60 bytes.
GAMEPAD_s GamePad[64];
// Original bss @0x127a4e0.
i32 readpads_always = 0;

u32 GAMEPAD_DRIGHT = 0x2000;
u32 GAMEPAD_DLEFT = 0x8000;
u32 GAMEPAD_DDOWN = 0x4000;
u32 GAMEPAD_DUP = 0x1000;
u32 GAMEPAD_TOGGLERIGHT = 10;
u32 GAMEPAD_TOGGLELEFT = 5;
u32 GAMEPAD_LIFT = 8;
u32 GAMEPAD_TAG = 16;
u32 GAMEPAD_SPECIAL = 32;
u32 GAMEPAD_ACTION = 128;
u32 GAMEPAD_JUMP = 64;
u32 GAMEPAD_START = 2048;
u32 GAMEPAD_SELECT = 256;
u32 GAMEPAD_R3 = 1024;
u32 GAMEPAD_MENUSELECT = 64;
u32 GAMEPAD_MENUCANCEL = 16;

// Two frames of direction history for each local player.  MovePlayer shifts
// these before accepting the new input; GamePad_Rotate then reports a stable
// angular velocity only when both consecutive turns agree.
f32 PadOldSpeed2[2] = {};
f32 PadOldSpeed[2] = {};
u16 PadOldAngle2[2] = {};
u16 PadOldAngle[2] = {};

static i32 RotationDistance(i32 difference) {
    return difference < 0 ? -difference : difference;
}

extern "C" {
    extern i32 CutSceneWaiting;
    extern i32 editor_active;
    extern "C++" i32 (*GamePads_IgnoreInputFn)(void);
    extern "C++" FadeSystem FadeSys;
}

GAMEPAD_s *GamePad_Allocate() {
    for (i32 i = 0; i < 64; i++) {
        GAMEPAD_s *pad = &GamePad[i];
        if ((pad->allocated_5a & 1) == 0) {
            pad->allocated_5a |= 1;
            return pad;
        }
    }
    return NULL;
}

void GamePads_Init() {
    Game_NuPad[0] = NuPadOpen(0, 0);
    Game_NuPad[1] = NuPadOpen(1, 0);

    for (i32 i = 0; i < 64; ++i) {
        memset(&GamePad[i], 0, sizeof(GamePad[i]));
    }
    GamePad[0].pad = Game_NuPad[0];
    GamePad[1].pad = Game_NuPad[1];
}

void GamePads_NetClient() {
}

void GamePads_NetHost() {
}

void GamePads_NetReset(i32) {
}

f32 GamePad_Rotate(GameObject_s *object) {
    i32 player_index;
    if (Player[0] == object) {
        player_index = 0;
    } else if (Player[1] == object) {
        player_index = 1;
    } else {
        return 0.0f;
    }

    GAMEPAD_s *pad = object->pad_gamepad;
    if (pad->input_magnitude == 0.0f || PadOldSpeed[player_index] == 0.0f || PadOldSpeed2[player_index] == 0.0f) {
        return 0.0f;
    }

    const i32 current_delta = RotDiff(PadOldAngle[player_index], pad->input_angle);
    const i32 previous_delta = RotDiff(PadOldAngle2[player_index], PadOldAngle[player_index]);
    if ((current_delta < 0 && previous_delta < 0 && current_delta > -0x2000 && previous_delta > -0x2000) ||
        (current_delta > 0 && previous_delta > 0 && current_delta < 0x2000 && previous_delta < 0x2000)) {
        return (static_cast<f32>(current_delta) / 65536.0f) / FRAMETIME;
    }
    return 0.0f;
}

i32 GamePad_Waggle(GAMEPAD_s *pad) {
    if (pad->input_magnitude != 0.0f &&
        RotationDistance(RotDiff(pad->previous_input_angle, pad->input_angle)) > 0x2000) {
        return 1;
    }
    if (pad->input_magnitude == 0.0f && pad->previous_input_magnitude != 0.0f) {
        return 1;
    }
    if (pad->input_magnitude != 0.0f && pad->previous_input_magnitude == 0.0f) {
        return 1;
    }
    return 0;
}

void ResetRumble(RUMBLEPACKET *packet) {
    packet->buzz_time = 0.0f;
    packet->time = 0.0f;
    packet->priority = 0;
}

void UpdateRumble(RUMBLEPACKET *packet) {
    if (packet->buzz_time > 0.0f)
        packet->buzz_time -= FRAMETIME;
    if (packet->time > 0.0f)
        packet->time -= FRAMETIME;
    if (packet->priority != 0)
        --packet->priority;
}

void NewRumble(nupad_s *pad, float strength, i32) {
    if (pad != NULL) {
        i32 amount = static_cast<i32>(strength * 255.0f);
        if (amount > 255)
            amount = 255;
        NuSound3AddRumble(pad, 0.0f, amount, 0, strength);
    }
}

void NewBuzz(nupad_s *pad, float duration, i32) {
    if (pad != NULL)
        NuSound3AddRumble(pad, duration, 0, 0, 0.0f);
}

void NewBuzzFrames(nupad_s *pad, i32 frames, i32) {
    if (pad != NULL)
        NuSound3AddRumble(pad, static_cast<f32>(frames) / DEFAULTFPS, 0, 0, 0.0f);
}

void NewRumbleAllPlayers(float strength, float duration, i32 frames, i32) {
    if (frames > 0) {
        f32 frame_duration = static_cast<f32>(frames) / DEFAULTFPS;
        if (frame_duration > duration)
            duration = frame_duration;
    }
    const f32 amount = strength * 255.0f;
    for (i32 i = 0; i < 8; ++i) {
        GameObject_s *object = Player[i];
        if (object != NULL && object->apiobj.player_controlled && object->pad_gamepad->pad != NULL) {
            NuSound3AddRumble(object->pad_gamepad->pad, duration, static_cast<i32>(amount), 0, strength);
        }
    }
}

void DieRumble(GameObject_s *object) {
    if (object != NULL && object->apiobj.player_controlled) {
        NewRumble(object->pad_gamepad->pad, 1.0f, 0);
        NewBuzz(object->pad_gamepad->pad, 0.3f, 0);
    }
}

void KillRumble(GameObject_s *object) {
    if (object != NULL) {
        if (object->apiobj.player_controlled) {
            NewRumble(object->pad_gamepad->pad, 0.7f, 0);
            NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
        }
    }
}

void HitRumble(GameObject_s *object) {
    if (object != NULL) {
        if (object->apiobj.player_controlled) {
            NewRumble(object->pad_gamepad->pad, 0.5f, 0);
            NewBuzzFrames(object->pad_gamepad->pad, 2, 0);
        }
    }
}

void TakeHitRumble(GameObject_s *object, float strength) {
    if (object != NULL && object->apiobj.player_controlled) {
        NewRumble(object->pad_gamepad->pad, strength, 0);
        NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
    }
}

void FloatRumble(GameObject_s *object) {
    if (object != NULL && object->apiobj.player_controlled) {
        const f32 strength = MIN(1.0f, NuFabs(object->apiobj.velocity.y) /
                                               object->apiobj.character_data->game_character->movement_speed * 0.15f +
                                           0.25f);
        NewRumble(object->pad_gamepad->pad, strength, 0);
        NewBuzzFrames(object->pad_gamepad->pad, 1, 0);
    }
}

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

i32 NoPad(i32 port, i32 require_game_input) {
    const i32 menu_id = GetMenuID();
    if ((require_game_input != 0 && GamePad[port].input_state == 0) || GamePad[port].pad->is_valid != 0) {
        return 0;
    }
    if (LEGOMENU_NEWGAME != -1 && menu_id == LEGOMENU_NEWGAME) {
        return 0;
    }
    if (LEGOMENU_CREDITS != -1 && menu_id == LEGOMENU_CREDITS) {
        return 0;
    }
    return 1;
}

void PadOutPause(i32 port, WORLDINFO_s *world) {
    if (Paused != 0 || NoPad(port, 1) == 0 || NewMode != 0 || NewLData != NULL || FadeSys.fade != 0.0f) {
        return;
    }
    if (GamePads_IgnoreInputFn != NULL && GamePads_IgnoreInputFn() != 0) {
        return;
    }
    if (MiniCutCam != 0 || (world->area != NULL && (world->area->flags & AREAFLAG_ENDING_AREA) != 0) ||
        (world->current_level->flags & (LEVEL_INTRO | LEVEL_MIDTRO | LEVEL_OUTRO)) != 0 || GetMenuID() != -1) {
        return;
    }
    if (CUTSTOPGAME != 0) {
        if (!CutScene_IsSkippable(static_cast<CUTINFO *>(CutStopInfo))) {
            return;
        }
    }
    if (CutSceneWaiting == 0) {
        PauseGame(port);
    }
}

i32 ReadPad(i32 port) {
    GAMEPAD_s *gamepad = &GamePad[port];
    NUPAD *pad = gamepad->pad;
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();

    i32 input_mode;
    i32 result;
    if (GamePads_IgnoreInputFn != NULL && GamePads_IgnoreInputFn() != 0) {
        result = 1;
        input_mode = 1;
    } else if (readpads_always != 0 || world == NULL || world->current_level == NULL ||
               world->current_level == TITLES_LDATA || world->current_level == CREDITS_LDATA ||
               (world->current_level->flags & LEVEL_STATUS) != 0) {
        result = 0;
        input_mode = 2;
    } else if (port >= PLAYERCOUNT || Player[port] == NULL) {
        result = 1;
        input_mode = 1;
    } else if (Player[port]->apiobj.player_controlled) {
        result = 0;
        input_mode = 2;
    } else {
        result = 0;
        input_mode = 3;
    }

    // These are the six fields cleared by the original before attempting a
    // low-level read. Offsets 0x0c..0x20 contain the two sticks' synthesized
    // directional held/pressed/previous masks (some legacy member names are
    // misleading, so use their offset aliases here).
    gamepad->unknown_0c = 0;
    gamepad->unknown_10 = 0;
    gamepad->unknown_18 = 0;
    gamepad->unknown_1c = 0;
    gamepad->unknown_04 = 0;
    gamepad->buttons_down_08 = 0;

    if (pad == NULL || NuPadRead(pad) == 0 || input_mode == 1) {
        return result;
    }

    if (input_mode == 3) {
        gamepad->buttons_down_08 |= pad->digital_buttons_pressed & GAMEPAD_START;
        return 3;
    }

    gamepad->unknown_0c = 0;
    const f32 left_x = static_cast<f32>(pad->analog_left_x) - 127.5f;
    if (left_x < -85.0f) {
        gamepad->unknown_0c |= GAMEPAD_DLEFT;
    } else if (left_x > 85.0f) {
        gamepad->unknown_0c |= GAMEPAD_DRIGHT;
    }
    const f32 left_y = static_cast<f32>(pad->analog_left_y) - 127.5f;
    if (left_y < -85.0f) {
        gamepad->unknown_0c |= GAMEPAD_DUP;
    } else if (left_y > 85.0f) {
        gamepad->unknown_0c |= GAMEPAD_DDOWN;
    }
    gamepad->unknown_10 = gamepad->unknown_0c & ~gamepad->unknown_14;
    gamepad->unknown_14 = gamepad->unknown_0c;

    gamepad->unknown_18 = 0;
    const f32 right_x = static_cast<f32>(pad->analog_right_x) - 127.5f;
    if (right_x < -85.0f) {
        gamepad->unknown_18 |= GAMEPAD_DLEFT;
    } else if (right_x > 85.0f) {
        gamepad->unknown_18 |= GAMEPAD_DRIGHT;
    }
    const f32 right_y = static_cast<f32>(pad->analog_right_y) - 127.5f;
    if (!(right_y < -85.0f)) {
        if (right_y > 85.0f) {
            gamepad->unknown_18 |= GAMEPAD_DDOWN;
        }
    } else {
        gamepad->unknown_18 |= GAMEPAD_DUP;
    }
    gamepad->unknown_1c = gamepad->unknown_18 & ~gamepad->unknown_20;
    gamepad->unknown_20 = gamepad->unknown_18;
    gamepad->input_state = 1;

    gamepad->buttons_down_08 = pad->digital_buttons_pressed;
    gamepad->unknown_04 = pad->digital_buttons;
    pActivePad = gamepad->pad;
    return 2;
}

void ReadPads() {
    ReadPad(0);
    ReadPad(1);
    readpads_always = 0;
}

void DoInput(WORLDINFO_s *world) {
    if (world == NULL) {
        world = WorldInfo_CurrentlyActive();
    }

    i32 input_result[2];
    input_result[0] = ReadPad(0);
    input_result[1] = ReadPad(1);
    i32 player_state_changed = PlayersDropInOut();

    for (i32 player_index = 0; player_index < 2; ++player_index) {
        nupad_s *pad = GamePad[player_index].pad;
        GameObject_s *player = Player[player_index];
        if (GamePads_IgnoreInputFn != NULL) {
            if (GamePads_IgnoreInputFn() != 0) {
                continue;
            }
        }
        if (static_cast<u32>(input_result[player_index]) <= 1) {
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
            (LEGOCONTEXT_DROPIN != -1 && static_cast<i8>(player->field_0x7a5) == LEGOCONTEXT_DROPIN) ||
            (pad->digital_buttons_pressed & GAMEPAD_START) == 0) {
            continue;
        }
        if (NewMode != 0 || NewLData != NULL || FadeSys.fade != 0.0f || editor_active != 0 ||
            !(GameTimer.time_elapsed > 0.0f) || world == NULL || world->current_level == TITLES_LDATA) {
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

u16 GamePad_InputAngle(GameObject_s *object, GAMEPAD_s *pad) {
    if (object->apiobj.player_controlled && object->field_0x661 != 0xff) {
        SOCK &socket = WORLD->sock_sys->sock[static_cast<i8>(object->field_0x661)];
        if ((socket.flags & 0x40) != 0) {
            return static_cast<u16>(pad->input_angle + object->yrot + socket.input_yaw);
        }
    }
    return static_cast<u16>(GameCam->input_yaw + pad->input_angle);
}

i32 GamePads_SkipMovie() {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    ReadPads();
    if ((GamePad[0].buttons_held & GAMEPAD_SKIP) != 0) {
        if (world != NULL && world->current_level == TITLES_LDATA && GAMEDEMO == 0) {
            PlayerProgress[0].active = 1;
        }
        return 1;
    } else if ((GamePad[1].buttons_held & GAMEPAD_SKIP) != 0) {
        if (world != NULL && world->current_level == TITLES_LDATA && GAMEDEMO == 0) {
            PlayerProgress[1].active = 1;
        }
        return 1;
    }
    return 0;
}

void FinishLoop_Network() {
    STUBBED();
}

