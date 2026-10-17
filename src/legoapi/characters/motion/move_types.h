#pragma once

#include "decomp.h"

// Types shared by legoapi/characters/motion/move.cpp and legoapi/ai/game/creature.cpp.

enum JEDI_ACTION : i16 {
    JEDI_ACTION_COMBO_1_1 = 46,
    JEDI_ACTION_COMBO_2_1 = 53,
};
struct AWKWARDSHAPESPHERE_s {
    i16 *character_id;
    i16 joint;
    u16 flags;
    f32 radius;
};

DECOMP_ASSERT(offsetof(AWKWARDSHAPESPHERE_s, radius) == 8, "Awkward shape sphere radius offset");
DECOMP_ASSERT(sizeof(AWKWARDSHAPESPHERE_s) == 12, "Awkward shape sphere size");
