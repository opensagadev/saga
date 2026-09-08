#pragma once

#include "decomp.h"
#include "nu2api/nucore/common.h"
#include "nu2api/numath/nuvec.h"

struct AIPATH_s;
struct AIPATHCNX_s;

// Per-character route cursor. This type is shared by the AI runtime packet,
// creature spawn records, and formation rows.
typedef struct AIPATHINFO_s {
    AIPATH_s *path;
    AIPATHCNX_s *connection;
    u8 direction;
    u8 path_index;

    u16 game_flags;
    u16 next_check;

    union {
        struct {
            u8 on_path : 1;
            u8 was_on_path : 1;
            u8 route_checked : 1;
            u8 narrow_path : 1;
        };
        u8 flags;
    };
    u8 padding_0x0f;

    f32 dist;
    f32 width;
} AIPATHINFO;

enum AIPATHINFO_FLAGS : u8 {
    AIPATHINFO_FLAG_ON_PATH = 0x01,
    AIPATHINFO_FLAG_WAS_ON_PATH = 0x02,
    AIPATHINFO_FLAG_ROUTE_CHECKED = 0x04,
    AIPATHINFO_FLAG_NARROW_PATH = 0x08,
};

DECOMP_ASSERT(sizeof(AIPATHINFO) == 0x18, "AIPATHINFO size");

typedef struct AILOCATOR_s {
    char name[0x10];
    NUVEC position;
    union {
        i32 flags;
        i32 direction;
    };
    union {
        AIPATHINFO path_info;
        AIPATHINFO path;
    };
    i32 locator_flags;
} AILOCATOR;
DECOMP_ASSERT(sizeof(AILOCATOR) == 0x3c, "AILOCATOR size");
DECOMP_ASSERT(offsetof(AILOCATOR, direction) == 0x1c, "AILOCATOR direction offset");
DECOMP_ASSERT(offsetof(AILOCATOR, path_info) == 0x20, "AILOCATOR path info offset");
