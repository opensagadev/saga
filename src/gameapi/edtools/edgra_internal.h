#pragma once

#include "gameapi/edtools/edgra.h"
#include "legoapi/legoapi_types.h"

// Grass-editor state shared by the reconstructed editor and runtime owners.
extern "C" {
    extern char edgra_filter_string[16];
    extern i32 edgra_ind_clumps_used;
    extern i32 *IndGrassClumpsUsed;
    extern edgra_individual_s *IndGrassClumps;
    extern edgra_clump_s *GrassClumps;
    extern i32 EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP;
    extern i32 EDGRA_MAX_INDIVIDUAL_CLUMPS;
    extern i32 EDGRA_MAX_CLUMPS;

    extern i32 edgra_page_calculate_done[8];
    extern i32 edgra_page_vectors_valid[8];
    extern NUMTX *edgra_page_matrix_stack[8];
    extern void *edgra_page_terrain[8];
    extern NUGSCN *edgra_page_scene[8];
    extern NUVEC *edgra_vecbuffer;
    extern NUMTX *edgra_mtxbuffer;
    extern void *edgra_free_vecbuffer;
    extern i32 edgra_editormode;
    extern i32 edgra_dpadmode;
    extern i32 edgra_roty;
    extern i32 edgra_rotz;
    extern i32 edgra_pageid;
    extern i32 edgra_page_on[8];
    extern i32 edgra_page_used[8];
    extern i32 edgra_clump_size;
    extern i32 edgra_fadeclumps_used;
    extern i32 edgra_clumps_used;
    extern i32 edgra_fadeunits_used;
    extern i32 edgra_units_used;
    extern i32 edgra_instance_type;
    extern i32 edgra_nearest_instance;
    extern i32 edgra_nearest;
    extern f32 edgra_size;
    extern i32 edgra_cam_ay;
    extern i32 edgra_cam_ax;
    extern f32 edgra_cam_dist;
    extern NUVEC edgra_cam_pos;
    extern f32 edgra_mtl_zoff;
    extern struct numtl_s *edgra_mtl;
    extern i32 edgra_filter;

    extern eduimenu_s *edgra_globals_menu;
    extern eduimenu_s *edgra_sscale_menu;
    extern eduimenu_s *edgra_dpadmode_menu;
    extern eduimenu_s *edgra_clumpmode_menu;
    extern eduimenu_s *edgra_clumpterrain_menu;
    extern eduimenu_s *edgra_clumpfade_menu;
    extern eduimenu_s *edgra_clumpdist_menu;
    extern eduimenu_s *edgra_clumparea_menu;
    extern eduimenu_s *edgra_clumpsizes_menu;
    extern eduimenu_s *edgra_clumpproperties_menu;
    extern eduimenu_s *edgra_changeinstance_menu;
    extern eduimenu_s *edgra_instance_menu;
    extern eduimenu_s *edgra_options_menu;
    extern eduimenu_s *edgra_active_menu;
}
