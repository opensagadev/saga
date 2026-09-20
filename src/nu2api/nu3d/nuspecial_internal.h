#ifndef NU2API_NU3D_NUSPECIAL_INTERNAL_H
#define NU2API_NU3D_NUSPECIAL_INTERNAL_H

#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"

// Shared legacy/display-list views used by the base NuSpecial unit and the
// remaining platform draw entry points. Keep these provisional layouts in one
// internal header until their canonical runtime structures are fully named.
struct NuSpecialHandleLayout {
    NUGSCN *scene;
    void *special;
    void *display_special;
};

struct NuSpecialLegacyRuntimeLayout {
    u8 pad_00[0x40];
    u8 *instance;
    char *name;
    u32 flags;
};

struct NuSpecialBoundsDisplayLayout {
    NUMTX mtx;
    NUMTX draw_mtx;
    NUVEC min;
    f32 min_w;
    NUVEC max;
    f32 max_w;
    NUVEC center;
    f32 radius;
    NUCLIPOBJECT *clip_objects;
    char *name;
    u32 flags;
    f32 *clip_range;
    i32 instance_ix;
    nuinstanim_s *instance_animation;
    i16 wind_speed;
    i16 wind_scale;
    u32 pad_cc;
};

struct NuSpecialLegacySceneLayout {
    u8 pad_00[0x18];
    void **objects;
    i32 instance_count;
    u8 *instances;
};

struct NuSpecialLegacyInstanceBoundsLayout {
    u8 pad_00[0x40];
    i16 object_index;
};

struct NuSpecialLegacyMaterialLink {
    NuSpecialLegacyMaterialLink *next;
    NUMTL *material;
};

struct NuSpecialLegacyObjectBoundsLayout {
    NuSpecialLegacyMaterialLink *materials;
    f32 origin_radius;
    u8 pad_08[4];
    NUVEC minimum;
    NUVEC maximum;
    NUVEC center;
    f32 radius;
    u8 pad_34[4];
    NuSpecialLegacyObjectBoundsLayout *next;
};

// Renderer state shared with the display-list implementation. These are
// implementation details of the original NuSpecial unit, not public API.
extern "C" {
    extern i32 nuspecial_reflection;
    extern i32 nuspecial_const_tint_enabled;
    extern NUCOLOUR3 nuspecial_const_tint;
    extern i32 nuspecial_const_alpha_enabled;
    extern f32 nuspecial_const_alpha;
    extern i32 nuspecial_clip_state;
}

#endif
