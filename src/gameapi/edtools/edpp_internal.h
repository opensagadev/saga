#pragma once

#include "legoapi/legoapi_types.h"

// Particle-editor state shared with the debris runtime and the remaining
// editor implementation files. The definitions belong to the original
// edtoolsall.cpp translation unit.
extern "C" {
    extern i32 edpp_types_used;
    extern usize edpp_page_scene[8];
    extern i32 edpp_page_on[8];
    extern i32 edpp_page_used[8];
    extern edpp_particle_s edpp_ptls[512];
    extern i32 edpp_nearest;
    extern NUVEC edpp_cam_pos;
    extern i32 edpp_instances_used;

    extern i32 edpp_dpad_mode;
    extern f32 edpp_scale_factor;
    extern f32 edpp_copy_size;
    extern i32 edpp_create_type;
    extern i32 edptl_clipboard_entry;
    extern i32 edptl_repeatboxxzlock;
    extern u8 edpp_effect_list;
    extern i32 edpp_num_orphans;
    extern eduimenu_s *edptl_switchtype_menu;
    extern eduimenu_s *edptl_page_menu;
    extern eduimenu_s *edptl_star_menu;
    extern eduimenu_s *edptl_soundid_menu;
}
