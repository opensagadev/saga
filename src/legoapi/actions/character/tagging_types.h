#pragma once

#include "decomp.h"

// Types shared by legoapi/actions/character/tagging.cpp and legoapi/core/startup/text.cpp.

struct TAGTRANSFER_s {
    GameObject_s *source;
    NUVEC position[3];
    f32 height[3];
    f32 time;
};

DECOMP_ASSERT(sizeof(TAGTRANSFER_s) == 0x38, "tag transfer size");
