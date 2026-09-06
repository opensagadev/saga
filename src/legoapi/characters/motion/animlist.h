#pragma once

#include "decomp_assert.h"
#include "nu2api/nucore/nuanim3.h"

struct ANIMLIST_s {
    char path[0x30];
    nuanimdata2_s *animation;
};

DECOMP_ASSERT(sizeof(ANIMLIST_s) == 0x34, "ANIMLIST_s size");

struct CHARACTERMODEL_s;
extern "C" f32 AnimListFrame(CHARACTERMODEL_s *model, i32 animation, i32 frame);
extern "C" f32 *AnimListFrameArray(CHARACTERMODEL_s *model, i32 animation);
