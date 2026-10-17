#pragma once

#include "decomp.h"

// Types shared by legoapi/characters/core/players.cpp and legoapi/ai/game/creature.cpp.

// Single-bit view of the AI flag bytes; the original resets them one field at a time.
struct PlayerAIFlagBits {
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};
struct SPECIAL_LAYER_s {
    i16 *character_id;
    char *name;
    u32 mask;
};

DECOMP_ASSERT(sizeof(SPECIAL_LAYER_s) == 0xc, "SPECIAL_LAYER_s size");
