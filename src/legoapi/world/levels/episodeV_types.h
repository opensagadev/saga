#pragma once

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
