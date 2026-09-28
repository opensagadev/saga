#include "decomp.h"
#include "batman.h"
#include "globals.h"
#include "gameapi/edtools/edgra.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/light/fade.h"
#include "legoapi/render/core/screen.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
extern f32 FRAMETIME;
extern i32 pause_rndr_on;
extern i32 wait_till_next_frame;
extern void DrawFadeScreenWipe(void);
extern "C" void NuRndrRect2di(i32, i32, i32, i32, i32, numtl_s *);

static u32 FRAMES_TO_WAIT = 1;

// The four global fade effect objects handed to FadeSystem::AddFade() during
// LoadPermData (original bss @0x127bf08 / 0x127bf00 / 0x127bef8 / 0x127bef0).
Fade fade;
FadeWipe fadeWipe;
FadeStillWipe fadeStillWipe;
FadeStill fadeStill;

// Material handles used by the original 2-D fade paths.
NUMTL *ScreenFadeMtl;
NUMTL *FadeMtl2;
NUMTL *FadeMtl;

void FadeStillWipe::DrawFade() {
    if (wait_till_next_frame != 0)
        return;
    if (info->stage & 2)
        DrawStillScreen(1);
    else
        DrawPauseScreenWipe();
}

void FadeStillWipe::InitFade() {
    const u32 old_direction = info->direction;
    if ((info->stage & 1) == 0) {
        info->fade = 1.0f;
        info->rate = 2.0f;
        pause_rndr_on = 0;
        if (info->field_28 == 0)
            info->field_28 = 1;
        else
            NeedScreenGrab(1);
        wait_till_next_frame = 1;
    } else {
        info->fade = 1.0f;
        info->rate = -1.3333334f;
    }
    do {
        i32 value = qrand();
        if (value < 0)
            value += 0x3fff;
        info->direction = 1u << (value >> 14);
    } while (info->direction == old_direction);
}

void FadeStillWipe::UpdateFade() {
    if (wait_till_next_frame > 0)
        --wait_till_next_frame;
    if ((info->stage & 2) && wait_till_next_frame == 0)
        pause_rndr_on = 1;
}

void Fade::DrawFade() {
    if (info->fade > 0.0f && FadeMtl != NULL) {
        const i32 index = (static_cast<i32>(info->fade * 16384.0f + 65536.0f) >> 1) & 0x7fff;
        NuRndrRect2di(0, 0, 0x2800, 0xe00, static_cast<i32>(NuTrigTable[index] * 128.0f) << 24, ScreenFadeMtl);
    }
}

void Fade::Init(FADEINFO_s *state) {
    info = state;
}

void Fade::InitFade() {
    f32 fade;
    f32 rate;
    if (__builtin_expect((info->stage & 1) == 0, 1)) {
        fade = 0.0f;
        rate = 2.0f;
    } else {
        fade = 1.0f;
        rate = -1.3333334f;
    }
    info->rate = rate;
    info->fade = fade;
}

void Fade::UpdateFade() {
}

void FadeWipe::DrawFade() {
    DrawFadeScreenWipe();
}

void FadeWipe::Init(FADEINFO_s *state) {
    info = state;
}

void FadeWipe::InitFade() {
    const u32 old = info->direction;
    if ((info->stage & 1) == 0) {
        info->fade = 0.0f;
        info->rate = 2.0f;
    } else {
        info->fade = 1.0f;
        info->rate = -1.3333334f;
    }
    do {
        i32 value = qrand();
        if (value < 0)
            value += 0x3fff;
        info->direction = 1u << (value >> 14);
    } while (info->direction == old);
}

void FadeWipe::UpdateFade() {
}

void FadeStillWipe::Init(FADEINFO_s *state) {
    info = state;
}

void FadeStill::DrawFade() {
    if (wait_till_next_frame != 0) {
        --wait_till_next_frame;
        return;
    }
    if (__builtin_expect((info->stage & 2) != 0, 1))
        DrawStillScreen(1);
    else
        DrawPauseScreenWipe();
}

void FadeStill::Init(FADEINFO_s *state) {
    info = state;
}

void FadeStill::InitFade() {
    volatile u32 &direction = info->direction;
    const u32 old_direction = direction;
    if ((info->stage & 1) == 0) {
        info->fade = 1.0f;
        info->rate = 2.0f;
        NeedScreenGrab(1);
        wait_till_next_frame = 1;
    } else {
        info->fade = 0.0f;
        info->rate = 0.0f;
    }
    do {
        i32 value = qrand();
        if (value < 0)
            value += 0x3fff;
        direction = 1u << (value >> 14);
    } while (direction == old_direction);
}

void FadeStill::UpdateFade() {
    if (info->stage & 2)
        pause_rndr_on = 1;
}

void SetFramesToWait(u32 frames) {
    FRAMES_TO_WAIT = frames;
}

void FadeSystem::Init() {
    fades[0] = NULL;
    fades[1] = NULL;
    fades[2] = NULL;
    fades[3] = NULL;
    field_28 = 1;
    pending_type = FADE_TYPE_NONE;
}

void FadeSystem::Update() {
    f32 old_fade = fade;
    FADETYPE_VALUE type = pending_type;
    if (type == FADE_TYPE_NONE)
        return;

    f32 next = fade + rate * FRAMETIME;
    if (next > 1.0f)
        fade = 1.0f;
    else
        fade = next < 0.0f ? 0.0f : next;

    if (old_fade < 1.0f && fade == 1.0f)
        busy = 1;
    else if (busy != 0)
        --busy;

    if (fades[type] != NULL)
        fades[type]->UpdateFade();
    if (fade == 0.0f || fade == 1.0f)
        rate = 0.0f;
    if (fade == 0.0f)
        pending_type = FADE_TYPE_NONE;
}

void FadeSystem::Draw() {
    if (pending_type != FADE_TYPE_NONE && fades[pending_type] != NULL)
        fades[pending_type]->DrawFade();
}

void FadeSystem::SetStage(char stage) {
    this->stage = stage;
    busy = 0;
    if (pending_type != FADE_TYPE_NONE && fades[pending_type] != NULL)
        fades[pending_type]->InitFade();
}

i32 FadeSystem::AddFade(FadeBase *o) {
    if (o == NULL)
        return 0;
    FADETYPE_VALUE type = o->GetFadeType();
    o->Init(this);
    fades[type] = o;
    return 1;
}

i32 FadeSystem::SetFade(FADETYPE const &t, u32 frames) {
    FADETYPE_VALUE type = t.type;
    if (type != FADE_TYPE_NONE && fades[type] != NULL) {
        pending_type = type;
        direction = frames;
        return 1;
    }
    pending_type = FADE_TYPE_NONE;
    return 0;
}

nugscn_s *FadeLoop_ObjScene;
nuhspecial_s FadeLoop_ObjHSpecial;

void FadeLoop_SetObj(nugscn_s *scene, char *name) {
    FadeLoop_ObjScene = scene;
    if (scene != NULL && NuSpecialFind(scene, &FadeLoop_ObjHSpecial, name, 1) == 0) {
        FadeLoop_ObjScene = NULL;
    }
}

void FadeLoop_DrawObj(float alpha) {
    if (FadeLoop_ObjScene != NULL && NuSpecialExistsFn(&FadeLoop_ObjHSpecial)) {
        NUVEC scale = {0.125f, 0.125f, 0.125f};
        NUMTX matrix;
        NuMtxSetScale(&matrix, &scale);
        matrix.m32 = 1.0f;
        NuSpecialDrawAtAlpha(&FadeLoop_ObjHSpecial, &matrix, alpha);
    }
}

i32 FadeLoop_UsingObj() {
    return FadeLoop_ObjScene != NULL;
}

void CreateFadeMaterials() {
    ScreenFadeMtl = NuMtlCreate(1);
    u8 *attrib = reinterpret_cast<u8 *>(&ScreenFadeMtl->attribs);
    attrib[1] = static_cast<u8>((attrib[1] & 0x30) | 0xc5);
    attrib[0] = static_cast<u8>((attrib[0] & 0xc0) | 0x13);
    attrib[2] = static_cast<u8>((attrib[2] & 0xfc) | 0x06);
    NuMtlUpdate(ScreenFadeMtl);

    FadeMtl2 = NuMtlCreate(1);
    attrib = reinterpret_cast<u8 *>(&FadeMtl2->attribs);
    attrib[1] = static_cast<u8>((attrib[1] & 0x30) | 0xc5);
    attrib[0] = static_cast<u8>((attrib[0] & 0xc0) | 0x11);
    attrib[2] = static_cast<u8>((attrib[2] & 0xfc) | 0x06);
    NuMtlUpdate(FadeMtl2);

    FadeMtl = NuMtlCreate(1);
    FadeMtl->opacity = 1.0f;
    FadeMtl->tex_id = 0;
    FadeMtl->sort_pri = 0x7ffe;
    attrib = reinterpret_cast<u8 *>(&FadeMtl->attribs);
    attrib[1] = static_cast<u8>((attrib[1] & 0x30) | 0xc5);
    attrib[2] = static_cast<u8>((attrib[2] & 0xfc) | 0x06);
    attrib[0] = static_cast<u8>((attrib[0] & 0xc0) | 0x11);
    NuMtlUpdate(FadeMtl);
}

void FadeLoop(char *text, i32 direction, float duration, void (*draw)(float)) {
    f32 alpha = 0.0f;
    f32 target = 1.0f;
    if (direction != 0) {
        alpha = 1.0f;
        target = 0.0f;
    }
    f32 rate = 10.0f;
    if (duration != 0.0f) {
        rate = 1.0f / duration;
    }
    FRAMETIME = DEFAULTFRAMETIME;
    while (alpha != target) {
        NuFrameBegin();
        alpha = SeekLinearF(alpha, target, rate * FRAMETIME);
        NuRndrBeginScene(-1);
        NuRndrClear(0xb00, 0, 1.0f);
        if (FadeLoop_ObjScene != NULL) {
            FadeLoop_DrawObj(alpha);
        }
        if (text != NULL) {
            SetQFont2D();
            Text3D(text, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0, static_cast<u8>(alpha * 0.0f),
                   static_cast<u8>(alpha * 191.0f), static_cast<u8>(alpha * 255.0f));
        }
        if (draw != NULL) {
            draw(alpha);
        }
        NuRndrEndScene();
        edGraEnableTerrainSwap();
        FRAMETIME = NuFrameEnd();
        edGraDisableTerrainSwap();
    }
    if (direction == 1) {
        FadeLoop_SetObj(NULL, NULL);
        FinishLoop(2);
    }
}
