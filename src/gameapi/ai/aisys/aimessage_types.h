#pragma once

#include "decomp.h"
#include "nu2api/nucore/nulist.h"

struct AIMESSAGE_s;

struct AIMESSAGESYS_s {
    i32 count;
    AIMESSAGE_s *messages;
    NULISTHDR free_list;
    NULISTHDR active_list;
};

struct AIMESSAGE_s {
    NULISTLNK links;
    char name[0x20];
    f32 value;
};

DECOMP_ASSERT(sizeof(AIMESSAGE_s) == 0x2c, "AIMESSAGE_s size");
DECOMP_ASSERT(offsetof(AIMESSAGE_s, name) == 0x8, "AIMESSAGE name offset");
DECOMP_ASSERT(offsetof(AIMESSAGE_s, value) == 0x28, "AIMESSAGE value offset");
DECOMP_ASSERT(sizeof(AIMESSAGESYS_s) == 0x18, "AIMESSAGESYS_s size");

struct AILOCALMESSAGE_s {
    AIMESSAGE_s message;
    AILOCALMESSAGE_s *next;
};

DECOMP_ASSERT(sizeof(AILOCALMESSAGE_s) == 0x30, "AILOCALMESSAGE size");
DECOMP_ASSERT(offsetof(AILOCALMESSAGE_s, next) == 0x2c, "AILOCALMESSAGE next offset");
