// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/misc/androidbatman.h"

#include <GLES2/gl2.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>

#include "batman.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legogame/game.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nuandroid/nuphoneos.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nusound/nusound.h"
#include "nu2api/numath/nuvec4.h"

extern "C" {
    void *DVD = NULL;
    nupad_s *Game_NuPad_Store[2];
    nupad_s **Game_NuPad;
}

i32 app_tbgameset;
i32 app_tbplayerset;
i32 app_tbaiset;
i32 app_tbdrawset;

static u8 s_SystemPausedTracks[2];

static void SystemDidBecomeActiveCallback(const NuPhoneOSMessageData *) {
    for (i32 track = 0; track < 2; ++track) {
        if (s_SystemPausedTracks[track] != 0) {
            NuSound3ResumeStereoStream(track);
            s_SystemPausedTracks[track] = 0;
        }
    }
    NuPad_Interface_ResetAllTouches();
}

static void SystemPauseCallback(const NuPhoneOSMessageData *) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();

    for (i32 player_index = 0; player_index < 2; ++player_index) {
        GameObject_s *player = Player[player_index];
        if (__builtin_expect(
                !(Paused == 0 && GameTimer.update_count != 0 && !(memcard_autosavepredelay > 0.0f) && NewMode == 0 &&
                  player != NULL && static_cast<i8>(player->apiobj.field_0x1f8) < 0 &&
                  (LEGOCONTEXT_DROPIN == -1 || LEGOCONTEXT_DROPIN != static_cast<i8>(player->field_0x7a5)) &&
                  NewLData == NULL && FadeSys.fade == 0.0f && editor_active == 0 && GameTimer.time_elapsed > 0.0f &&
                  world != NULL && world->current_level != TITLES_LDATA && GameMenu[GameMenuLevel].menu == -1 &&
                  CutSceneWaiting == 0 && CUTSTOPGAME == 0 && MiniCutCam == 0 && memcard_autosavestarted == 0 &&
                  !(memcard_autosavepostdelay > 0.0f)),
                0))
            continue;
        PauseGame(player->pad_gamepad - GamePad);
        break;
    }

    for (i32 track = 0; track < 2; ++track) {
        if (__builtin_expect(NuSound3GetStereoStreamStatus(track) == NUSOUND_STEREO_STREAM_PLAYING, 1)) {
            s_SystemPausedTracks[track] = 1;
            NuSound3PauseStereoStream(track);
        }
    }
}

static void TouchCallback(const NuPhoneOSMessageData *message) {
    NuPad_Interface_TouchScreenInput(
        message->touch_id, message->x, message->y, message->pressure, message->touch_action == NUPHONE_TOUCH_DOWN,
        message->touch_action == NUPHONE_TOUCH_UP, message->touch_action == NUPHONE_TOUCH_MOVE,
        message->touch_action == NUPHONE_TOUCH_CANCEL);
}

i32 Text_ExpandButtonString(char *input, char *output) {
    if (NuStrICmp(input, "[TAG]") == 0 || NuStrICmp(input, "[TRIANGLE]") == 0 || NuStrICmp(input, "[T]") == 0) {
        strcpy(output, "[[y]]");
        return 1;
    }
    if (NuStrICmp(input, "[ACTION]") == 0 || NuStrICmp(input, "[SQUARE]") == 0 || NuStrICmp(input, "[S]") == 0) {
        strcpy(output, "[[x]]");
        return 1;
    }
    if (NuStrICmp(input, "[SPECIAL]") == 0 || NuStrICmp(input, "[CIRCLE]") == 0 || NuStrICmp(input, "[O]") == 0) {
        strcpy(output, "[[b]]");
        return 1;
    }
    if (NuStrICmp(input, "[JUMP]") == 0 || NuStrICmp(input, "[CROSS]") == 0 || NuStrICmp(input, "[X]") == 0) {
        strcpy(output, "[[a]]");
        return 1;
    }
    if (NuStrICmp(input, "[TOGGLELEFT]") == 0) {
        strcpy(output, "[[lb]]");
        return 1;
    }
    if (NuStrICmp(input, "[TOGGLERIGHT]") == 0) {
        strcpy(output, "[[rb]]");
        return 1;
    }
    return 0;
}

void GameFog_Set() {
    if (NuIOS_IsLowEndDevice()) {
        NuLightFogX(GameFog.low_quality_start, GameFog.low_quality_end, GameFog.colour, 0.0f, 0.0f, 0, 0.0f);
        return;
    }

    NuLightFogX(GameFog.high_quality_start, GameFog.high_quality_end, GameFog.colour, 0.0f, 0.0f, 1,
                GameFog.high_quality_density);
}

void GamePad_InitButtons() {
    GAMEPAD_JUMP = 0x40;
    GAMEPAD_ACTION = 0x80;
    GAMEPAD_SPECIAL = 0x20;
    GAMEPAD_TAG = 0x10;
    GAMEPAD_MENUSELECT = 0x40;
    GAMEPAD_MENUCANCEL = 0x20;
    GAMEPAD_SKIP = 0x840;
    GAMEPAD_TOGGLELEFT = 5;
    GAMEPAD_TOGGLERIGHT = 10;
}

extern "C" {
extern "C" void NuIOS_FreeMemoryForSuspend(void) {
    const char *source_path = "i:/SagaTouch-Android_9176564/saga/androidbatman.cpp";
    BeginCriticalSectionGL(source_path, 270);
    NuIOS_DeallocateSystemFramebuffers();
    glReleaseShaderCompiler();
    glFinish();
    EndCriticalSectionGL(source_path, 279);
    NuThreadCriticalSectionBegin(g_performingBgProcWorkCritSec);
    NuThreadCriticalSectionBegin(g_writingSaveCriticalSection);
}

}

void InitOnce(i32 argc, char **param_2) {
    NuPhoneOSRegisterEventCallback(PHONE_EVENT_TOUCH, TouchCallback);
    NuPhoneOSRegisterEventCallback(PHONE_EVENT_PAUSE, SystemPauseCallback);
    NuPhoneOSRegisterEventCallback(PHONE_EVENT_BECOME_ACTIVE, SystemDidBecomeActiveCallback);

    if (NuIOS_IsLowEndDevice()) {
        SUPERBUFFERSIZE -= 0x38370;
    }

    i32 size = SUPERBUFFERSIZE;

    permbuffer_base.void_ptr = NU_ALLOC(size, 4, 1, "", NUMEMORY_CATEGORY_NONE);
    superbuffer_end.void_ptr = (void *)(SUPERBUFFERSIZE + (usize)permbuffer_base.void_ptr);
    original_permbuffer_base.void_ptr = permbuffer_base.void_ptr;
    InitGameBeforeConfig();

    Game_NuPad = &Game_NuPad_Store[0];

#define SETUP(cmd, ...) cmd, ##__VA_ARGS__

    NuInitHardware(&permbuffer_base, &superbuffer_end, 0,                      //
                   SETUP(NUAPI_SETUP_HOSTFS, 0),                               //
                   SETUP(NUAPI_SETUP_SWAPMODE, NUVIDEO_SWAPMODE_ASYNC),        //
                   SETUP(NUAPI_SETUP_STREAMSIZE, 0x200000),                    //
                   SETUP(NUAPI_SETUP_VIDEOMODE, (PAL == 0) ? 0xdeadbeef : 8),  //
                   SETUP(NUAPI_SETUP_RESOLUTION, 512, (PAL == 0) ? 224 : 256), //
                   SETUP(NUAPI_SETUP_GLASSRPLANE, 1),                          //
                   SETUP(NUAPI_SETUP_CDDVDMODE, &DVD),                         //
                   SETUP((NOSOUND == 0) ? NUAPI_SETUP_AUDIO : NUAPI_SETUP_AUDIO_DISABLED, 0, 0x640, 0, 0),
                   SETUP(NUAPI_SETUP_PAD0, &Game_NuPad_Store[0]), //
                   SETUP(NUAPI_SETUP_PAD1, &Game_NuPad_Store[1]), //
                   SETUP(NUAPI_SETUP_0x46, 1),                    //
                   SETUP(NUAPI_SETUP_0x47, 1),                    //
                   SETUP(NUAPI_SETUP_0x49, 1),                    //
                   SETUP(NUAPI_SETUP_0x4b, 1),                    //
                   NUAPI_SETUP_END                                //
    );

    pNuCam = NuCameraCreate();
    Game.options_save.field11_0xb = static_cast<u8>(NuIOS_IsWidescreen());
    WidescreenCode(Game.options_save.field11_0xb);
    InitPanel(Game.options_save.field11_0xb);

    app_tbgameset = NuTimeBarCreateSet(0);
    app_tbplayerset = NuTimeBarCreateSet(0);
    app_tbaiset = NuTimeBarCreateSet(0);
    app_tbdrawset = NuTimeBarCreateSet(0);
}

