#pragma once

#include "decomp.h"

// Types shared by legoapi/world/levels/episodeV.cpp and legoapi/characters/motion/move.cpp.

struct HOTHBATTLEE_NETPACKET_s {
    i16 targets[12];
    char defeated[12];
    f32 alpha[12];
    i32 count;
};
struct ASTEROID_s {
    nuhspecial_s special;
    GIZMOBLOWUP_s *blowup;
    i16 rotation_speed_x;
    i16 rotation_speed_y;
    i16 rotation_speed_z;
    u8 activated;
    u8 reserved_17;
};
struct FINALASTEROID_s {
    nuhspecial_s special;
    GIZMOBLOWUP_s *blowups[8];
    i16 blowup_count;
    i16 rotation_speed_x;
    i16 rotation_speed_y;
    i16 rotation_speed_z;
    i16 rotation_x;
    i16 rotation_y;
    i16 rotation_z;
};
struct ASTEROIDCNETPACKET_s {
    u16 rotation_x;
    u16 rotation_y;
    u16 rotation_z;
    u16 reserved;
};

DECOMP_ASSERT(offsetof(FINALASTEROID_s, blowup_count) == 0x2c, "final asteroid blowup count offset");
DECOMP_ASSERT(offsetof(FINALASTEROID_s, rotation_x) == 0x34, "final asteroid rotation offset");
DECOMP_ASSERT(offsetof(HOTHBATTLEE_NETPACKET_s, alpha) == 0x24, "Hoth panel packet alpha offset");
DECOMP_ASSERT(offsetof(HOTHBATTLEE_NETPACKET_s, count) == 0x54, "Hoth panel packet count offset");
DECOMP_ASSERT(sizeof(ASTEROIDCNETPACKET_s) == 8, "asteroid C network packet size");
DECOMP_ASSERT(sizeof(ASTEROID_s) == 0x18, "ASTEROID_s size");
DECOMP_ASSERT(sizeof(FINALASTEROID_s) == 0x3c, "final asteroid size");
DECOMP_ASSERT(sizeof(HOTHBATTLEE_NETPACKET_s) == 0x58, "Hoth panel packet size");
