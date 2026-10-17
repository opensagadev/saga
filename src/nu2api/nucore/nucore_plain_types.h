#pragma once

// Types shared by nu2api/nucore/nucore_plain.cpp and gameapi/edtools/edtoolsall.cpp.

extern "C" {
    struct NuLegacyDwaChunk {
        i32 node_count;
        i32 reserved_04;
        nuanimcurveset_s **curve_sets;
    };
    struct NUKEYCHAR {
        u32 key;
        u8 normal;
        u8 shifted;
        u8 padding[2];
    };
    struct NuInstanceTreeNode {
        u16 bounds_index;
        u16 subtree_instance_count;
        u16 child_count;
        u16 first_child;
        u16 instance_count;
        u16 first_instance;
    };
    struct NuInstanceTreeBounds {
        NUVEC center;
        f32 center_w;
        NUVEC extent;
        f32 extent_w;
    };
    struct NuInstanceTree_s {
        u32 reserved_00;
        u32 reserved_04;
        u16 *instance_indices;
        NuInstanceTreeBounds *instance_bounds;
        u32 reserved_10;
        NuInstanceTreeNode *nodes;
        NuInstanceTreeBounds *node_bounds;
        u16 tree_count;
        u16 reserved_1e;
        u16 *roots;
        f32 *far_clips;
    };
    struct NuVisibilityOcclusionGrid {
        u32 reserved_00;
        i32 width;
        f32 min_x;
        f32 max_x;
        f32 min_z;
        f32 max_z;
        f32 scale_x;
        f32 scale_z;
        i32 mode;
        u32 output_size;
        u32 reserved_28;
        u8 **cells;
    };
}
