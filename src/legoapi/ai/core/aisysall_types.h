#pragma once

#include "decomp.h"

// Types shared by legoapi/ai/core/aisysall.cpp and legoapi/ai/core/gameai.cpp.

enum { EQ = 0, LT = 1, GT = 2, LT_EQ = 3, GT_EQ = 4, NEQ = 5 };
// The editor keeps 64 editable area records ahead of its two lists.
struct EDAIAREA_s {
    NULISTLNK link;
    char name[16];
    NUVEC position;
    NUVEC size;
    i16 rotation;
    u8 unknown_32;
    u8 flags;
    u8 unknown_34[16];
};
struct AIPATHCNXTYPE_s {
    u32 connection_flag;
    void *context;
    char name[0x40];
    u32 flags;
};
struct EdUiNameInputItem : eduiitem_s {
    u32 unknown_48;
    char name[0x40];
    u8 unknown_08c[0x15a - 0x8c];
    i16 max_name_length;
};
struct EditorItemColours {
    u32 normal;
    u32 selected;
    u32 disabled;
    u32 background;
};
// Private view of the fields accessed by creature editor callbacks.
struct CreatureEditorRecord {
    NULISTLNK link;
    char name[0x10];
    char script_name[0x10];
    NUVEC position;
    i32 angle;
    union {
        u8 path_check[0x1c];
        struct {
            u8 unknown_38[0x3c - 0x38];
            void *path;
            u8 unknown_40[0x54 - 0x40];
        };
    };
    u32 valid_positions;
    i16 character_type;
    u8 set;
    u8 group_count;
    u8 across_count;
    u8 unknown_5d[3];
    f32 x_spacing;
    f32 z_spacing;
    u32 flags;
    void *activation_area;
    f32 script_params[4];
    void *trigger_area;
    void *locator;
    void *respawn_locator;
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

DECOMP_ASSERT(offsetof(AIPATHCNXTYPE_s, flags) == 0x48, "editor path connection options offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, activation) == 0x8f, "creature activation offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, activation_area) == 0x6c, "creature activation area offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, angle) == 0x34, "creature angle offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, character_type) == 0x58, "creature character type offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, flags) == 0x68, "creature flags offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, negative_min_view_height) == 0xa8, "creature min view height offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, path) == 0x3c, "creature path offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, position) == 0x28, "creature position offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, script_name) == 0x18, "creature script name offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, script_params) == 0x70, "creature script params offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, set) == 0x5a, "creature set offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, trigger_area) == 0x80, "creature trigger area offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, valid_positions) == 0x54, "creature valid position mask offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, view_distance) == 0x9c, "creature view distance offset");
DECOMP_ASSERT(offsetof(EdUiNameInputItem, max_name_length) == 0x15a, "editor name input limit offset");
DECOMP_ASSERT(offsetof(EdUiNameInputItem, name) == 0x4c, "editor name input offset");
DECOMP_ASSERT(sizeof(AIPATHCNXTYPE_s) == 0x4c, "editor path connection type size");
DECOMP_ASSERT(sizeof(CreatureEditorRecord) == 0xac, "editor creature record size");
DECOMP_ASSERT(sizeof(EDAIAREA_s) == 0x44, "area editor record size");
