#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/screen.h"
#include "legoapi/characters/motion.h"
#include "legoapi/menus/core/text.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/characters/motion/gameanim.h"
#include "gamelib/util/gamelib_util_types.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nuandroid/ios_graphics.h"

#include <GLES2/gl2.h>
#include <stdio.h>
#include <string.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void ClearScreen() {
    STUBBED();
}

void RenderQuads(i16 *) {
    STUBBED();
}

void InitAlphaList() {
    char name[16];

    for (i32 i = 0; i < 26; ++i) {
        memset(name, 0, sizeof(name));
        sprintf(name, "shop_%c", i + 'a');
        NuSpecialFind(WORLD->current_gscn, &atoz0to9icon[i], name, 1);
    }

#define FIND_SHOP_DIGIT(index_, digit_)                                                                                \
    memset(name, 0, sizeof(name));                                                                                     \
    sprintf(name, "shop_%c", digit_);                                                                                  \
    NuSpecialFind(WORLD->current_gscn, &atoz0to9icon[index_], name, 1)

    FIND_SHOP_DIGIT(26, '0');
    FIND_SHOP_DIGIT(27, '1');
    FIND_SHOP_DIGIT(28, '2');
    FIND_SHOP_DIGIT(29, '3');
    FIND_SHOP_DIGIT(30, '4');
    FIND_SHOP_DIGIT(31, '5');
    FIND_SHOP_DIGIT(32, '6');
    FIND_SHOP_DIGIT(33, '7');
    FIND_SHOP_DIGIT(34, '8');
    FIND_SHOP_DIGIT(35, '9');

#undef FIND_SHOP_DIGIT
}

f32 GetAspectRatio() {
    return static_cast<f32>(g_backingHeight) / static_cast<f32>(g_backingWidth);
}

static u8 ScreenGrabNeeded;
static i32 pause_rt;
static NUMTL *pause_rndr_mtl;
extern i32 pause_rndr_on;
extern f32 pause_fade;
static i32 old_pause_state;
i32 (*PauseRenderOffFn)(void);
i32 cut_waiting_for_new_level;
u32 animSetVisibilityHack[6];
i32 specVisibilityFlashHack;

void GameAnimSet_Draw(GAMEANIMSET_s &set);
extern f32 hackFlashTimer;
extern GAMEANIMSET_s *hackFlashingGameAnimSet;
extern nuhspecial_s *hackFlashingSpecial;

extern FadeSystem FadeSys;
extern i32 Paused;
extern volatile i32 waiting_for_level;
extern i32 GAMEDEMO;
extern "C" {
    extern f32 MainRenderTime;
    extern i32 back_rgba[2];
    extern i32 clear_screen_onstill;
    extern i32 screendump;
}
void BackDrop_Draw(f32, i32);

extern "C" i32 NuRndrBeginScene(i32);
extern "C" void NuRndrEndScene(void);
extern "C" void NuBackbufferCopy(i32);

extern f32 CameraZoom;
extern "C" f32 NuIOS_GetAspectRatio(void);

void WidescreenCode(i32) {
    pNuCam->aspect = 1.0f / NuIOS_GetAspectRatio();
    pNuCam->fov = (1.0f / NuIOS_GetAspectRatio() + 0.75f) * 0.5f * (1.0f / CameraZoom);
    SmartTextSetWidescreen(1.3333334f / NuIOS_GetAspectRatio(), 1.0f);
}

SAGA_HOST_WEAK void InitStillRender(variptr_u *, variptr_u) {
    static NUNATIVETEX nativePauseTex;

    pause_rt = NuTexGenTexture(&nativePauseTex);
    nativePauseTex.ref_count = 1;
    memset(nativePauseTex.checksum, 0, sizeof(nativePauseTex.checksum));
    nativePauseTex.width = g_backingWidth;
    nativePauseTex.height = g_backingHeight;
    nativePauseTex.image_data = NULL;
    nativePauseTex.size = 0;

    BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/legoapi.saga/screen.cpp", 0x561);
    glGenTextures(1, &nativePauseTex.platform.gl_tex);
    glActiveTexture(GL_TEXTURE0);
    g_currentTexUnit = 0;
    glBindTexture(GL_TEXTURE_2D, nativePauseTex.platform.gl_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g_backingWidth, g_backingHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/legoapi.saga/screen.cpp", 0x56d);

    pause_rndr_mtl = NuMtlCreate3D(1);
    pause_rndr_mtl->sort_pri = 0x7fff;
    pause_rndr_mtl->shader_desc.flags = 0x1000;
    pause_rndr_mtl->opacity = 1.0f;
    u8 *attrib = reinterpret_cast<u8 *>(&pause_rndr_mtl->attribs);
    attrib[1] = static_cast<u8>((attrib[1] & 0x30) | 0xc5);
    attrib[2] = static_cast<u8>((attrib[2] & 0xfc) | 6);
    attrib[0] = static_cast<u8>((attrib[0] & 0xc0) | 0x11);
    attrib[1] = 0xe5;
    pause_rndr_mtl->tex_id = static_cast<i16>(pause_rt);
    pause_rndr_mtl->shader_desc.diffuse_color[0] = -1;
    pause_rndr_mtl->shader_desc.unknown_a8 = 1;
    pause_rndr_mtl->shader_desc.vtx_desc.flags |= 0x40800;
    NuMtlUpdate(pause_rndr_mtl);
    pause_rndr_on = 0;
}

void DrawPauseScreenWipe() {
    NuRndrBeginScene(-1);

    const f32 fade = FadeSys.fade;
    i32 x = 0;
    i32 y = 0;
    i32 width = 0x2800;
    i32 height = 0xe00;
    f32 u0 = 0.0f;
    f32 v0 = 1.0f;
    f32 u1 = 1.0f;
    f32 v1 = 0.0f;
    u32 colours[4];

    if ((FadeSys.direction & 3) != 0) {
        if ((FadeSys.direction & 1) == 0) {
            width = static_cast<i32>(fade * 10240.0f);
            colours[0] = 0x80808080u;
            colours[1] = 0x00808080u;
            colours[2] = 0x80808080u;
            colours[3] = 0x00808080u;
            NuRndrGradRectUV2di(width, 0, 0x400, 0xe00, fade, 1.0f, fade + 0.1f, 0.0f, colours, pause_rndr_mtl);
            u1 = fade;
        } else {
            u0 = 1.0f - fade;
            x = static_cast<i32>(u0 * 10240.0f);
            width = 0x2800 - x;
            colours[0] = 0x00808080u;
            colours[1] = 0x80808080u;
            colours[2] = 0x00808080u;
            colours[3] = 0x80808080u;
            NuRndrGradRectUV2di(x - 0x400, 0, 0x400, 0xe00, u0 - 0.1f, 1.0f, u0, 0.0f, colours, pause_rndr_mtl);
        }
    } else if ((FadeSys.direction & 0xc) != 0) {
        if ((FadeSys.direction & 4) == 0) {
            height = static_cast<i32>(fade * 3584.0f);
            colours[0] = 0x80808080u;
            colours[1] = 0x80808080u;
            colours[2] = 0x00808080u;
            colours[3] = 0x00808080u;
            NuRndrGradRectUV2di(0, height, 0x2800, 0x166, 0.0f, 1.0f - fade, 1.0f, 1.0f - (fade + 0.1f), colours,
                                pause_rndr_mtl);
            v1 = 1.0f - fade;
        } else {
            const f32 edge = 1.0f - fade;
            y = static_cast<i32>(edge * 3584.0f);
            height = 0xe00 - y;
            colours[0] = 0x00808080u;
            colours[1] = 0x00808080u;
            colours[2] = 0x80808080u;
            colours[3] = 0x80808080u;
            NuRndrGradRectUV2di(0, y - 0x166, 0x2800, 0x166, 0.0f, 1.0f - (edge - 0.1f), 1.0f, 1.0f - edge, colours,
                                pause_rndr_mtl);
            v0 = 1.0f - edge;
        }
    }

    NuRndrRectUV2di(x, y, width, height, u0, v0, u1, v1, 0x80808080u, pause_rndr_mtl);
    NuRndrEndScene();
}

void GrabStillScreen() {
    if (ScreenGrabNeeded != 0) {
        ScreenGrabNeeded = 0;
        NuRndrBeginScene(-1);
        NuBackbufferCopy(pause_rt);
        NuRndrEndScene();
        pause_fade = 0;
    }
}

void NeedScreenGrab(i32 needed) {
    ScreenGrabNeeded = needed != 0;
}

i8 IsGrabbingScreen() {
    return ScreenGrabNeeded;
}

void DrawStillScreen(i32 clear) {
    NuRndrBeginScene(-1);
    NuVpGetCurrentViewport();
    if (clear != 0) {
        NuRndrClear(0x500, 0, 1.0f);
    }
    if (MainRenderTime >= 1.0f) {
        NuRndrRectUV2di(0, 0, 0x2800, 0xe00, 0.0f, 1.0f, 1.0f, 0.0f, 0x80808080u, pause_rndr_mtl);
    } else {
        const u32 colour = (static_cast<i32>(MainRenderTime * 128.0f) << 24) | 0x00808080u;
        u32 colours[4] = {colour, colour, colour, colour};
        NuRndrGradRectUV2di(0, 0, 0x2800, 0xe00, 0.0f, 1.0f, 1.0f, 0.0f, colours, pause_rndr_mtl);
    }
    NuRndrEndScene();
}

void ClearStill() {
    old_pause_state = 0;
    Paused = 0;
}

void UpdateCutBorders() {
    f32 target_scale = 1.0f;

    if (CUTSTOPGAME == 0 && (LEGOCAMMODE_DOORCUT == -1 || GameCam->mode != LEGOCAMMODE_DOORCUT)) {
        if (LEGOCAMMODE_OBSTACLE == -1 || GameCam->mode != LEGOCAMMODE_OBSTACLE || ObstacleCamBorders == 0) {
            target_scale = 0.0f;
        }
    }

    CutBorderScale = SeekLinearF(CutBorderScale, target_scale, FRAMETIME * 2.0f);
}

void HandleStillRender() {
    GetMenuID();
    if (pause_rndr_on != 0 && FadeSys.pending_type == FADE_TYPE_NONE) {
        NuRndrBeginScene(-1);
        if (MainRenderTime < 1.0f) {
            if (back_rgba[0] == back_rgba[1]) {
                NuRndrClear(0xf00, back_rgba[0], 1.0f);
            } else {
                NuRndrGradClear(0xf00, back_rgba[0], back_rgba[1], 1.0f);
            }
            BackDrop_Draw(1.0f - MainRenderTime, 0);
        } else {
            NuRndrClear(0x300, 0, 1.0f);
        }
        NuRndrEndScene();
    }

    if (grab_screen_image != 2 && pause_rndr_on != 0 && pause_rt != 0 && FadeSys.pending_type == FADE_TYPE_NONE) {
        DrawStillScreen(clear_screen_onstill);
    }

    if (Paused != 0) {
        if (old_pause_state == 0 && FadeSys.pending_type == FADE_TYPE_NONE) {
            NeedScreenGrab(1);
        }
    }
    if (Paused == 0 && old_pause_state != 0) {
        pause_rndr_on = 0;
    }
    old_pause_state = Paused;

    if ((waiting_for_level == -1 ||
         (gone_through_door_to_new_level == 0 && cut_waiting_for_new_level == 0 && waiting_for_new_level == 0)) &&
        (NewLData != CREDITS_LDATA || LastLData != STATUS_LDATA) &&
        (NewLData != STATUS_LDATA ||
         ((WORLD->level_sub_id == -1 || (WORLD->area[WORLD->level_sub_id].flags & 2) == 0) && GAMEDEMO == 0)) &&
        NewLData != CREDITS_LDATA &&
        (NewLData != HUB_LDATA || WORLD->level_sub_id == -1 || (WORLD->area[WORLD->level_sub_id].flags & 2) == 0) &&
        !(MainRenderTime > 0.0f && MainRenderTime < 1.0f)) {
        if (Paused == 0 || IsGrabbingScreen() != 0) {
            pause_rndr_on = static_cast<u32>(grab_screen_image) > 1;
        } else if (PauseRenderOffFn != NULL && PauseRenderOffFn() != 0) {
            pause_rndr_on = 0;
        } else {
            pause_rndr_on = 1;
        }
    } else {
        pause_rndr_on = 1;
    }

    if (screendump != 0) {
        pause_rndr_on = 0;
    }
    clear_screen_onstill = 1;
    grab_screen_image = 0;
}

void PreRenderFlashHack() {
    if (hackFlashingGameAnimSet == NULL && hackFlashingSpecial == NULL)
        return;

    TouchHacks::TintStack tint;
    hackFlashTimer -= FRAMETIME;
    if (TouchHacks::ShouldFlash(hackFlashTimer)) {
        NUCOLOUR3 *colour = TouchHacks::GetFlashColour();
        NuRndrLightingStateCurrent.ambient = *colour;
        NuRndrSetAmbientLightPS(colour);
    }

    if (hackFlashingGameAnimSet != NULL) {
        GameAnimSet_Draw(*hackFlashingGameAnimSet);
        animSetVisibilityHack[1] = 0;
        GAMEANIMOBJ_s *object = hackFlashingGameAnimSet->objects;
        animSetVisibilityHack[0] = 0;
        u32 index = 0;
        while (object != NULL) {
            if (NuSpecialGetVisibilityFn(&object->special) != 0)
                animSetVisibilityHack[index >> 5] |= 1u << (index & 31);
            ++index;
            NuSpecialSetVisibility(&object->special, 0);
            if (index == 0xc0)
                break;
            object = object->next;
        }
    }
    if (hackFlashingSpecial != NULL) {
        NuSpecialDrawAt(hackFlashingSpecial, NuSpecialGetDrawMtx(hackFlashingSpecial));
        specVisibilityFlashHack = NuSpecialGetVisibilityFn(hackFlashingSpecial);
        NuSpecialSetVisibility(hackFlashingSpecial, 0);
    }
}

void UCStretchToCorners(i16 *, i16 *) {
    STUBBED();
}

void PostRenderFlashHack() {
    if (hackFlashingGameAnimSet != NULL && hackFlashingGameAnimSet->objects != NULL) {
        GAMEANIMOBJ_s *object = hackFlashingGameAnimSet->objects;
        NuSpecialSetVisibility(&object->special, animSetVisibilityHack[0] & 1);
        u32 index = 1;
        while ((object = object->next) != NULL) {
            NuSpecialSetVisibility(&object->special,
                                   (static_cast<i32>(animSetVisibilityHack[index >> 5]) >> (index & 31)) & 1);
            if (++index == 0xc1)
                break;
        }
    }
    if (hackFlashingSpecial != NULL)
        NuSpecialSetVisibility(hackFlashingSpecial, specVisibilityFlashHack);
    if (hackFlashTimer <= 0.0f && hackFlashTimer != 0.0f) {
        hackFlashingSpecial = NULL;
        hackFlashingGameAnimSet = NULL;
    }
}
