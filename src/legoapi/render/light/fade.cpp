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
#include "legoapi/audio/sfx.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nu3d/nu3d_includes.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
// The file is built with -ftree-vectorize for the VuVec initializers; the
// functions below were compiled without it.
#define SCALAR_FN __attribute__((optimize("omit-frame-pointer", "no-tree-vectorize")))
extern f32 FRAMETIME;
extern i32 pause_rndr_on;
extern i32 wait_till_next_frame;

extern NUMTL *ScreenFadeMtl;
extern NUMTL *FadeMtl2;
extern NUMTL *FadeMtl;

static u32 FRAMES_TO_WAIT = 1;

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
        pause_rndr_on = 0;
        info->fade = 1.0f;
        info->rate = 2.0f;
        if (FadeSys.field_28 == 0) {
            FadeSys.field_28 = 1;
        } else {
            NeedScreenGrab(1);
        }
        wait_till_next_frame = FRAMES_TO_WAIT;
    } else {
        info->fade = 1.0f;
        info->rate = -1.3333334f;
        if (CUTSTOPGAME == 0 && WORLD->current_level != NULL && (WORLD->current_level->flags & 0xe0) == 0 &&
            WORLD->current_level != TITLES_LDATA) {
            PlaySfx(const_cast<char *>("WipeScreen"), NULL);
        }
    }
    do {
        info->direction = 1u << (qrand() / 0x4000);
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
        const f32 angle = info->fade * 16384.0f + 49152.0f;
        const i32 index = (static_cast<i32>(angle + 16384.0f) >> 1) & 0x7fff;
        NuRndrRect2di(0, 0, 0x2800, 0xe00, static_cast<i32>(NuTrigTable[index] * 128.0f) << 24, ScreenFadeMtl);
    }
}

void Fade::Init(FADEINFO_s *state) {
    info = state;
}

void Fade::InitFade() {
    f32 fade_value;
    f32 rate;
    if ((info->stage & 1) != 0) {
        fade_value = 1.0f;
        rate = -1.3333334f;
    } else {
        fade_value = 0.0f;
        rate = 2.0f;
    }
    info->rate = rate;
    info->fade = fade_value;
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
    f32 fade_value;
    f32 rate;
    if ((info->stage & 1) != 0) {
        fade_value = 1.0f;
        rate = -1.3333334f;
    } else {
        fade_value = 0.0f;
        rate = 2.0f;
    }
    info->rate = rate;
    info->fade = fade_value;
    do {
        info->direction = 1u << (qrand() / 0x4000);
    } while (info->direction == old);
}

void FadeWipe::UpdateFade() {
}

void FadeStillWipe::Init(FADEINFO_s *state) {
    info = state;
}

void FadeStill::DrawFade() {
    if (wait_till_next_frame == 0) {
        if (info->stage & 2)
            DrawStillScreen(1);
        else
            DrawPauseScreenWipe();
    } else {
        --wait_till_next_frame;
    }
}

void FadeStill::Init(FADEINFO_s *state) {
    info = state;
}

void FadeStill::InitFade() {
    const u32 old_direction = info->direction;
    if ((info->stage & 1) == 0) {
        info->fade = 1.0f;
        info->rate = 2.0f;
        NeedScreenGrab(1);
        wait_till_next_frame = FRAMES_TO_WAIT;
    } else {
        info->fade = 0.0f;
        info->rate = 0.0f;
    }
    do {
        info->direction = 1u << (qrand() / 0x4000);
    } while (info->direction == old_direction);
}

void FadeStill::UpdateFade() {
    if (info->stage & 2)
        pause_rndr_on = 1;
}

void SetFramesToWait(u32 frames) {
    FRAMES_TO_WAIT = frames;
}

void FadeSystem::Init() {
    memset(fades, 0, sizeof(fades));
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
    if (fade == 0.0f)
        pending_type = FADE_TYPE_NONE;
    if (fade == 0.0f || fade == 1.0f)
        rate = 0.0f;
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

SCALAR_FN i32 FadeSystem::SetFade(FADETYPE const &t, u32 frames) {
    FADETYPE_VALUE type = t.type;
    if (type != FADE_TYPE_NONE && fades[type] != NULL) {
        direction = frames;
        pending_type = type;
        return 1;
    }
    pending_type = FADE_TYPE_NONE;
    return 0;
}
