#pragma once

#include "decomp.h"
#include "editor/edpath_types.h"
#include "nu2api/nucore/nulist.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nuhspecial.h"

struct EDAIPATH_s;
struct EDAIPATHNODE_s;
struct EDAIPATHWALL_s;
struct eduimenu_s;
struct AISYS_s;
struct AIPATH_s;
struct AIPATHSYS_s;
struct EDLOCATOR_s;
struct EDLOCATORSET_s;

struct EditorNamedEntry {
    NULISTLNK link;
    char name[0x10];
};

struct EDLOCATOR_s {
    NULISTLNK link;
    char name[0x10];
    NUVEC position;
    i32 direction;
    union {
        u8 path_check[0x1c];
        struct {
            i32 on_path;
            EDAIPATH_s *path;
            EDAIPATHNODE_s *first_node;
            EDAIPATHNODE_s *second_node;
            f32 path_fraction;
            f32 path_width;
            i32 path_angle;
        };
    };
    u16 runtime_index;
    u8 drawn;
    u8 unknown_47;
};

struct EDLOCATORSET_s {
    NULISTLNK link;
    char name[0x10];
    EDLOCATOR_s *locators[64];
};

struct EDCREATURELOCATOR_s {
    NULISTLNK link;
    u8 unknown_08[0x84 - 0x08];
    EDLOCATOR_s *locator;
};

DECOMP_ASSERT(sizeof(EDLOCATOR_s) == 0x48, "editor locator stride");
DECOMP_ASSERT(offsetof(EDLOCATOR_s, path) == 0x2c, "editor locator path offset");
DECOMP_ASSERT(offsetof(EDLOCATOR_s, first_node) == 0x30, "editor locator first path node offset");
DECOMP_ASSERT(offsetof(EDLOCATOR_s, second_node) == 0x34, "editor locator second path node offset");
DECOMP_ASSERT(offsetof(EDLOCATOR_s, path_fraction) == 0x38, "editor locator path fraction offset");
DECOMP_ASSERT(offsetof(EDLOCATOR_s, path_width) == 0x3c, "editor locator path width offset");
DECOMP_ASSERT(offsetof(EDLOCATOR_s, path_angle) == 0x40, "editor locator path angle offset");
DECOMP_ASSERT(offsetof(EDLOCATOR_s, runtime_index) == 0x44, "editor locator runtime index offset");
DECOMP_ASSERT(sizeof(EDLOCATORSET_s) == 0x118, "editor locator set stride");

struct AIEDITOR_RENDER_STATE {
    u8 unknown_00[0x04];
    eduimenu_s *main_menu;
    void *enter_context;
    void *enter_owner;
    union {
        NUVEC selection_position;
        NUVEC cursor_position;
    };
    NUVEC cursor_movement;
    NUVEC camera_position;
    union {
        u8 unknown_34[0x64 - 0x34];
        struct {
            i32 camera_pitch;
            i32 camera_yaw;
            nuhspecial_s cursor_platform;
            u8 unknown_48_to_64[0x64 - 0x48];
        };
    };
    AISYS_s *ai_system;
    EDAIPATH_s path_storage[32];
    EDAIPATH_s *current_path;
    union {
        NULISTHDR free_paths;
        u8 unknown_3fec[8];
    };
    NULISTHDR paths;
    union {
        u8 unknown_3ffc[0x31304 - 0x3ffc];
        struct {
            EDAIPATHNODE_s node_storage[1024];
            NULISTHDR free_path_nodes;
            EDAISHAREDPATHNODE_s shared_node_storage[64];
        };
    };
    union {
        NULISTHDR free_shared_nodes;
        NULISTHDR free_shared_path_nodes;
    };
    union {
        NULISTHDR path_walls;
        NULISTHDR shared_path_nodes;
    };
    union {
        u8 unknown_31314[0x36924 - 0x31314];
        struct {
            AIPATHSYS_s *cached_path_system;
            AIPATH_s *runtime_path;
            u8 unknown_3131c_to_36924[0x36924 - 0x3131c];
        };
    };
    NULISTHDR creatures;
    u8 unknown_3692c[4];
    EditorNamedEntry *mode_selection_36930;
    u8 unknown_36934[0x37a48 - 0x36934];
    void *mode_selection_37a48;
    u8 unknown_37a4c[4];
    EDLOCATOR_s locator_pool[256];
    NULISTHDR free_locators;
    NULISTHDR locators;
    union {
        void *mode_selection_3c260;
        EDLOCATOR_s *current_locator;
    };
    union {
        u8 unknown_3c264[4];
        EDLOCATOR_s *nearest_locator;
    };
    EDLOCATORSET_s locator_set_pool[64];
    char pending_locator_name[0x10];
    NULISTHDR free_locator_sets;
    NULISTHDR locator_sets;
    union {
        EditorNamedEntry *current_route;
        EDLOCATORSET_s *current_locator_set;
    };
    u8 unknown_4088c[0x42e9c - 0x4088c];
    void *mode_selection_42e9c;
    u8 unknown_42ea0[0x42ea8 - 0x42ea0];
    u8 flags;
    u8 unknown_42ea9[3];
    void *enter_extra_1;
    void *enter_extra_2;
    void *enter_extra_3;
    u8 save_scratch[0x80000];
    char warning[0x100];
    u32 pad_buttons;
    u32 pad_pressed;
};

DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, current_path) == 0x3fe8, "editor current path offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, selection_position) == 0x10, "editor selection position offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, free_paths) == 0x3fec, "editor free path list offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, main_menu) == 0x04, "editor main menu offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, ai_system) == 0x64, "editor AI system offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, enter_context) == 0x08, "editor enter context offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, enter_owner) == 0x0c, "editor enter owner offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, paths) == 0x3ff4, "editor path list offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, path_storage) == 0x68, "editor path storage offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, node_storage) == 0x3ffc, "editor path node storage offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, shared_node_storage) == 0x31004,
              "editor shared path node storage offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, free_shared_nodes) == 0x31304, "editor free shared node list offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, path_walls) == 0x3130c, "editor path wall list offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, mode_selection_36930) == 0x36930, "editor mode selection offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, creatures) == 0x36924, "editor creature list offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, mode_selection_37a48) == 0x37a48, "editor mode selection offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, mode_selection_3c260) == 0x3c260, "editor mode selection offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, locator_pool) == 0x37a50, "editor locator pool offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, free_locators) == 0x3c250, "editor free locator list offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, locators) == 0x3c258, "editor locator list offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, locator_set_pool) == 0x3c268, "editor locator set pool offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, free_locator_sets) == 0x40878, "editor free locator set list offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, locator_sets) == 0x40880, "editor locator set list offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, current_route) == 0x40888, "editor current route offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, mode_selection_42e9c) == 0x42e9c, "editor mode selection offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, flags) == 0x42ea8, "editor flags offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, enter_extra_1) == 0x42eac, "editor enter extra offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, enter_extra_2) == 0x42eb0, "editor enter extra offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, enter_extra_3) == 0x42eb4, "editor enter extra offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, cursor_movement) == 0x1c, "editor cursor movement offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, camera_pitch) == 0x34, "editor camera pitch offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, cursor_platform) == 0x3c, "editor cursor platform offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, save_scratch) == 0x42eb8, "editor save scratch offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, warning) == 0xc2eb8, "editor warning offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, pad_buttons) == 0xc2fb8, "editor pad buttons offset");
DECOMP_ASSERT(offsetof(AIEDITOR_RENDER_STATE, pad_pressed) == 0xc2fbc, "editor pad pressed offset");

extern "C" AIEDITOR_RENDER_STATE *aieditor;
