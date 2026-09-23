#pragma once

#include "decomp.h"
#include "nu2api/nucore/nulist.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nuhspecial.h"

struct EDANTINODE_s {
    NULISTLNK link;
    nuvec_s position;
    f32 radius;
    f32 lower_height;
    f32 upper_height;
    nuhspecial_s special;
    nuvec_s special_position;
    i32 rotation_offset;
    i32 flags;
    f32 base_radius;
    f32 base_height;
    u8 game_flags;
    u8 type;
    u8 unknown_4a[2];
};
DECOMP_ASSERT(sizeof(EDANTINODE_s) == 0x4c, "editor antinode stride");
DECOMP_ASSERT(offsetof(EDANTINODE_s, rotation_offset) == 0x38, "editor antinode rotation offset");
DECOMP_ASSERT(offsetof(EDANTINODE_s, flags) == 0x3c, "editor antinode flags offset");
