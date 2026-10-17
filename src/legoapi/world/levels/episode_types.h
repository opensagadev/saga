#pragma once

#include "decomp.h"

// Types shared by legoapi/world/levels/episode.cpp and legoapi/characters/motion/move.cpp.

struct TROOPERCANNON_s {
    GIZMO *base;
    GIZBUILDIT_s *buildit;
    GameObject_s *object;
    char character_name[32];
    u8 rebuilding;
    u8 reserved_0x2d[3];
};

DECOMP_ASSERT(offsetof(TROOPERCANNON_s, character_name) == 0x0c, "Trooper cannon name offset");
DECOMP_ASSERT(offsetof(TROOPERCANNON_s, rebuilding) == 0x2c, "Trooper cannon rebuilding offset");
DECOMP_ASSERT(sizeof(TROOPERCANNON_s) == 0x30, "Trooper cannon state size");
