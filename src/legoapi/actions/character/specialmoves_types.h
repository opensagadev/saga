#pragma once

#include "decomp.h"

// Types shared by legoapi/actions/character/specialmoves.cpp and legoapi/actions/movement/jumping.cpp.

struct SPECIALMOVE_s {
    i16 attacker_animation;
    i16 attacker_id;
    i8 attacker_action_type;
    i8 victim_action_type;
    i16 victim_id;
    i16 victim_animation;
    u16 flags;
    f32 distance;
};

DECOMP_ASSERT(offsetof(SPECIALMOVE_s, flags) == 10, "SPECIALMOVE flags offset");
DECOMP_ASSERT(offsetof(SPECIALMOVE_s, victim_animation) == 8, "SPECIALMOVE victim animation offset");
DECOMP_ASSERT(sizeof(SPECIALMOVE_s) == 0x10, "SPECIALMOVE size");
