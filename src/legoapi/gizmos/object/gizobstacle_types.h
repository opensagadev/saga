#pragma once

#include "decomp.h"

// Types shared by legoapi/gizmos/object/gizobstacle.cpp and legogame/game.cpp.

namespace {
    enum : i32 {
        GIZOBSTACLE_PROGRESS_CAPACITY = 128,
        GIZOBSTACLE_PROGRESS_WORDS = GIZOBSTACLE_PROGRESS_CAPACITY / 32,
    };
    enum GIZOBSTACLE_ACTIVATE_REVERSE_FLAGS : u32 {
        GIZOBSTACLE_ACTIVATE_REVERSE_VALIDATE = 0x01,
        GIZOBSTACLE_ACTIVATE_REVERSE_ALLOW_MATCHING_STATE = 0x04,
    };
    struct GIZOBSTACLEPROGRESS_s {
        u32 progress_flag_0[GIZOBSTACLE_PROGRESS_WORDS];
        u32 progress_flag_1[GIZOBSTACLE_PROGRESS_WORDS];
        u32 runtime_flag_0[GIZOBSTACLE_PROGRESS_WORDS];
        u32 runtime_flag_2[GIZOBSTACLE_PROGRESS_WORDS];
        u32 runtime_flag_3[GIZOBSTACLE_PROGRESS_WORDS];
        u32 runtime_flag_6[GIZOBSTACLE_PROGRESS_WORDS];
        u32 runtime_flag_7[GIZOBSTACLE_PROGRESS_WORDS];
    };
    struct OBSTACLE_CHARACTER_DATA_FLAGS_s {
        u32 flags_0x00;
        u8 flags_0x04;
        u8 flags_0x05;
    };
}

DECOMP_ASSERT(sizeof(GIZOBSTACLEPROGRESS_s) == 0x70, "GIZOBSTACLE progress ABI");
