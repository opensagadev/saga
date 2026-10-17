#pragma once

#include "decomp.h"

// Types shared by legoapi/audio/sfx.cpp and legoapi/render/core/gameliball.cpp.

enum RepeatSfxState : u8 {
    REPEAT_SFX_INACTIVE = 0,
    REPEAT_SFX_INITIAL_DELAY = 1,
    REPEAT_SFX_PLAY = 2,
    REPEAT_SFX_INTERVAL = 3,
};
struct RepeatSfx {
    i16 sfx_id;
    RepeatSfxState state;
    i8 plays_remaining;
    f32 timer;
    f32 interval;
    nuvec_s *position;
};
struct SoundTrackData {
    u8 reserved_00[0x88];
    u8 flags_88;
};

DECOMP_ASSERT(sizeof(RepeatSfx) == 0x10, "RepeatSfx size");
