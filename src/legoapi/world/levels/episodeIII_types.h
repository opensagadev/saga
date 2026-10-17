#pragma once

#include "decomp.h"

// Types shared by legoapi/world/levels/episodeIII.cpp and legoapi/ai/game/creature.cpp.

struct VADERA_s {
    GIZAIMESSAGE_s *in_control_room_message;
    GIZAIMESSAGE_s *ceiling_collapse_message;
    GIZAIMESSAGE_s *timer_message;
    f32 timer;
    u16 count;
    i16 subtitle;
    GIZFORCE_s *forces[4];
    AILOCATOR_s *big_jump_locator;
    i32 collapse_started;
    u8 reset_flag;
    u8 reserved_2d[0x03];
};
struct CRUISERCNETPACKET_s {
    i16 free_palpatine;
    i16 dooku_fight;
};
struct CRUISERC_s {
    GIZAIMESSAGE_s *dooku_fight;
    GameObject_s *count_dooku;
    GIZAIMESSAGE_s *free_palpatine;
    GameObject_s *palpatine;
    AILOCATOR_s *palpatine_locator;
};
struct CRUISERDNETPACKET_s {
    f32 frame;
    f32 speed;
    u32 flags;
};
struct VADERANETPACKET_s {
    u16 count;
    i16 subtitle;
    f32 timer;
};

DECOMP_ASSERT(offsetof(CRUISERC_s, count_dooku) == 4, "CRUISERC_s Dooku offset");
DECOMP_ASSERT(offsetof(VADERA_s, big_jump_locator) == 0x24, "VADERA_s locator offset");
DECOMP_ASSERT(sizeof(CRUISERDNETPACKET_s) == 12, "Cruiser D packet size");
DECOMP_ASSERT(sizeof(VADERA_s) == 0x30, "VADERA_s size");
