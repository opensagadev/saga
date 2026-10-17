#pragma once

// Types shared by legoapi/world/levels/episode.cpp and legoapi/characters/motion/move.cpp.

struct TROOPERCANNON_s {
    GIZMO *base;
    GIZBUILDIT_s *buildit;
    GameObject_s *object;
    char character_name[32];
    u8 rebuilding;
    u8 reserved_0x2d[3];
};
