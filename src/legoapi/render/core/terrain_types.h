#pragma once

#include "decomp.h"

// Types shared by legoapi/render/core/terrain.cpp and legoapi/render/core/gameliball.cpp.

namespace {
    struct TERRAIN_SCENE_OBJECT {
        u8 pad_0x00[0x44];
        u8 transform[4];
        void *object;
        u8 pad_0x4c[4];
    };
    struct TERRAIN_DISPLAY_OBJECT {
        u8 pad_0x00[0x40];
        u8 embedded_object[0x78];
        u8 transform[8];
        i32 scene_object_index;
        void *object;
        u8 pad_0xc8[8];
    };
    struct TERRAIN_DISPLAY_SCENE {
        u8 pad_0x00[0x6c];
        i32 object_count;
        TERRAIN_DISPLAY_OBJECT *objects;
    };
    struct TERRAIN_GSCENE {
        u8 pad_0x00[0x1c];
        i32 object_count;
        TERRAIN_SCENE_OBJECT *objects;
        u8 pad_0x24[0x34];
        void *object_map_present;
        u16 *object_map;
        u8 pad_0x60[0xb0];
        TERRAIN_DISPLAY_SCENE *display_scene;
    };
    struct TerrainScanBounds {
        f32 min_y;
        f32 min_x;
        f32 min_z;
        f32 max_y;
        f32 max_x;
        f32 max_z;
    };
    struct TerrainScanWriter {
        u8 *group_header;
        TERRAIN_SHAPE **cursor;
        u8 *limit;
        i32 group_shape_count;
        i32 scaled_shape_count;
    };
    struct TERRAIN_IMPACT_RECORD {
        NUVEC position;
        NUVEC normal;
        u8 material[2];
        u8 flags;
        u8 normal_flags;
    };
    enum TERRAIN_IMPACT_RESULT_FLAGS {
        TERRAIN_IMPACT_RESULT_WALL = 0x01,
        TERRAIN_IMPACT_RESULT_GROUND = 0x02,
        TERRAIN_IMPACT_RESULT_STATIC_WALL = 0x10,
    };
}

// The retained debug renderers read the older 100-byte terrain record.
struct TERRAIN_DEBUG_RECORD {
    u8 unknown_00[0x18];
    NUVEC vertices[4];
    NUVEC normals[2];
    u8 unknown_60[4];
};
namespace {
    struct ShadowScanWriter {
        u8 *group_header;
        TERRAIN_SHAPE **cursor;
        u8 *limit;
        i32 shape_count;
    };
    struct ShadowSurfaceCandidate {
        f32 height;
        TERRAIN_SHAPE *surface;
        NUVEC normal;
        i16 terrain_group;
    };
}

DECOMP_ASSERT(offsetof(TERRAIN_DISPLAY_OBJECT, scene_object_index) == 0xc0, "TERRAIN_DISPLAY_OBJECT scene_object_index offset");
DECOMP_ASSERT(offsetof(TERRAIN_DISPLAY_OBJECT, transform) == 0xb8, "TERRAIN_DISPLAY_OBJECT transform offset");
DECOMP_ASSERT(offsetof(TERRAIN_DISPLAY_SCENE, object_count) == 0x6c, "TERRAIN_DISPLAY_SCENE object_count offset");
DECOMP_ASSERT(offsetof(TERRAIN_GSCENE, display_scene) == 0x110, "TERRAIN_GSCENE display_scene offset");
DECOMP_ASSERT(offsetof(TERRAIN_GSCENE, object_count) == 0x1c, "TERRAIN_GSCENE object_count offset");
DECOMP_ASSERT(offsetof(TERRAIN_GSCENE, object_map_present) == 0x58, "TERRAIN_GSCENE object_map_present offset");
DECOMP_ASSERT(sizeof(TERRAIN_DEBUG_RECORD) == 100, "Terrain debug record ABI");
DECOMP_ASSERT(sizeof(TERRAIN_DISPLAY_OBJECT) == 0xd0, "TERRAIN_DISPLAY_OBJECT ABI");
DECOMP_ASSERT(sizeof(TERRAIN_IMPACT_RECORD) == 0x1c, "TERRAIN_IMPACT_RECORD ABI");
DECOMP_ASSERT(sizeof(TERRAIN_SCENE_OBJECT) == 0x50, "TERRAIN_SCENE_OBJECT ABI");
