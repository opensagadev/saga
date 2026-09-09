#pragma once

#include "decomp.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/numtx.h"

extern i32 plug_gizmotype_id;

#ifdef __cplusplus

enum PLUG_FLAGS : u8 {
    PLUG_FLAG_ACTIVE = 1 << 0,
    PLUG_FLAG_VISIBLE = 1 << 1,
    PLUG_FLAG_PLUGGED = 1 << 2,
};

typedef struct PLUG_s {
    char name[16];
    NUVEC position;
    u16 x_rotation;
    u16 y_rotation;
    u16 z_rotation;
    u8 enabled;
    union {
        u8 flags;
        struct {
            u8 active : 1;
            u8 visible : 1;
            u8 reserved_flags : 6;
        };
    };
    u8 reserved[0x10];
} PLUG;

typedef struct PLUGSYS_s {
    PLUG *plugs;
    i32 count;
} PLUGSYS;

DECOMP_ASSERT(sizeof(PLUG) == 0x34, "PLUG size");
DECOMP_ASSERT(offsetof(PLUG, position) == 0x10, "PLUG position offset");
DECOMP_ASSERT(offsetof(PLUG, flags) == 0x23, "PLUG flags offset");
DECOMP_ASSERT(offsetof(PLUG, z_rotation) == 0x20, "PLUG Z rotation offset");
DECOMP_ASSERT(sizeof(PLUGSYS) == 8, "PLUGSYS size");

ADDGIZMOTYPE *Plugs_RegisterGizmo(i32 type_id);
PLUG *Plug_FindNearest(PLUGSYS *system, NUVEC *position, f32 *distance_squared, i32 only_unplugged);
void Plug_MakeDrawMtx(PLUG *plug, NUMTX *matrix);

extern "C" {
#endif

#ifdef __cplusplus
}
#endif
