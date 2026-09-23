#pragma once

#include "decomp.h"
#include "nu2api/nu3d/nuhspecial.h"
#include "nu2api/nucore/nulist.h"
#include "nu2api/numath/nuvec.h"

struct EDAIPATHNODE_s;
struct EDAIPATHCNX_s {
    EDAIPATHNODE_s *node;
    u32 flags;
    u16 route_mask;
    u16 unknown_0a;
};
struct EDAISHAREDPATHNODE_s {
    NULISTLNK link;
    i8 reference_count;
    union {
        i8 runtime_index;
        u8 draw_flags;
    };
};
struct EDAIPATHROUTE_s {
    char name[0x10];
    u64 user_mask;
    u8 flags;
    u8 unknown_19[3];
};
struct EDAIPATH_s {
    NULISTLNK link;
    char name[0x10];
    NULISTHDR nodes;
    EDAIPATHNODE_s *current_node;
    union {
        EDAIPATHNODE_s *other_node;
        EDAIPATHNODE_s *nearest_node;
    };
    union {
        EDAIPATHROUTE_s *current_route;
        u32 unknown_28;
    };
    u8 flags;
    u8 unknown_02d;
    i16 runtime_start;
    i16 runtime_end;
    i16 runtime_nearest;
    i16 node_count;
    i8 draw_index;
    u8 unknown_037[0x3c - 0x37];
    EDAIPATHROUTE_s routes[16];
};
struct EDAIPATHWALL_s {
    NULISTLNK link;
    u8 unknown_08[2];
    u8 flags;
};
struct EDAIPATHNODE_s {
    NULISTLNK link;
    char name[0x10];
    NUVEC position;
    f32 radius;
    union {
        f32 lower_height;
        f32 height_min;
    };
    union {
        f32 upper_height;
        f32 height_max;
    };
    union {
        u8 unknown_030[0x90 - 0x30];
        EDAIPATHCNX_s connections[8];
    };
    i16 index;
    u8 flags;
    u8 unknown_093[0x96 - 0x93];
    u16 route_mask;
    union {
        nuhspecial_s special;
        nuhspecial_s platform;
    };
    union {
        NUVEC special_position;
        NUVEC platform_position;
    };
    EDAISHAREDPATHNODE_s *shared_node;
};

DECOMP_ASSERT(offsetof(EDAIPATH_s, nodes) == 0x18, "editor path node list offset");
DECOMP_ASSERT(offsetof(EDAIPATHCNX_s, route_mask) == 0x08, "editor connection route mask offset");
DECOMP_ASSERT(sizeof(EDAIPATHCNX_s) == 0x0c, "editor connection stride");
DECOMP_ASSERT(offsetof(EDAIPATHROUTE_s, flags) == 0x18, "editor route flags offset");
DECOMP_ASSERT(offsetof(EDAIPATHNODE_s, route_mask) == 0x96, "editor node route mask offset");
DECOMP_ASSERT(offsetof(EDAISHAREDPATHNODE_s, reference_count) == 0x08, "editor shared node reference count offset");
DECOMP_ASSERT(offsetof(EDAIPATH_s, current_node) == 0x20, "editor current node offset");
DECOMP_ASSERT(offsetof(EDAIPATH_s, current_route) == 0x28, "editor current route offset");
DECOMP_ASSERT(offsetof(EDAIPATH_s, flags) == 0x2c, "editor path flags offset");
DECOMP_ASSERT(offsetof(EDAIPATHROUTE_s, user_mask) == 0x10, "editor route user mask offset");
DECOMP_ASSERT(sizeof(EDAIPATHROUTE_s) == 0x1c, "editor route stride");
DECOMP_ASSERT(offsetof(EDAIPATH_s, routes) == 0x3c, "editor route array offset");
DECOMP_ASSERT(offsetof(EDAIPATH_s, draw_index) == 0x36, "editor path draw index offset");
DECOMP_ASSERT(offsetof(EDAIPATHWALL_s, flags) == 0x0a, "editor path wall flags offset");
DECOMP_ASSERT(offsetof(EDAIPATHNODE_s, index) == 0x90, "editor path node index offset");
DECOMP_ASSERT(offsetof(EDAIPATHNODE_s, name) == 0x08, "editor path node name offset");
DECOMP_ASSERT(offsetof(EDAIPATHNODE_s, radius) == 0x24, "editor node radius offset");
DECOMP_ASSERT(offsetof(EDAIPATHNODE_s, lower_height) == 0x28, "editor node lower height offset");
DECOMP_ASSERT(offsetof(EDAIPATHNODE_s, upper_height) == 0x2c, "editor node upper height offset");
DECOMP_ASSERT(offsetof(EDAIPATHNODE_s, flags) == 0x92, "editor path node flags offset");
DECOMP_ASSERT(offsetof(EDAIPATHNODE_s, special) == 0x98, "editor path node special offset");
DECOMP_ASSERT(offsetof(EDAIPATHNODE_s, special_position) == 0xa4, "editor path node special position offset");
DECOMP_ASSERT(offsetof(EDAIPATHNODE_s, shared_node) == 0xb0, "editor shared path node offset");
