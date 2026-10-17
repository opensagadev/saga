#pragma once

#include "decomp.h"

// Types shared by legoapi/ai/core/ai_sys_stubs.cpp and legoapi/ai/core/aisysall.cpp.

extern "C" {
    struct RebuildEditorCreature {
        NULISTLNK link;
        char name[0x10];
        char script_name[0x10];
        NUVEC position;
        i32 angle;
        EDAIPATHCHECK_s path_check;
        u32 valid_positions;
        i16 character_type;
        u8 set;
        u8 group_count;
        u8 across_count;
        u8 padding_5d[3];
        f32 x_spacing;
        f32 z_spacing;
        i32 flags;
        EditorNamedEntry *activation_area;
        f32 script_params[4];
        EditorNamedEntry *trigger_area;
        EDLOCATOR_s *locator;
        EDLOCATOR_s *respawn_locator;
        u8 difficulty;
        u8 min_respawns;
        u8 max_respawns;
        u8 activation;
        f32 min_respawn_time;
        f32 max_respawn_time;
        f32 stagger_start;
        f32 view_distance;
        f32 hear_distance;
        f32 max_view_height;
        f32 negative_min_view_height;
    };
}

DECOMP_ASSERT(offsetof(RebuildEditorCreature, activation) == 0x8f, "editor creature activation offset");
DECOMP_ASSERT(offsetof(RebuildEditorCreature, activation_area) == 0x6c, "editor creature area offset");
DECOMP_ASSERT(offsetof(RebuildEditorCreature, path_check) == 0x38, "editor creature path offset");
DECOMP_ASSERT(sizeof(RebuildEditorCreature) == 0xac, "editor creature rebuild stride");
