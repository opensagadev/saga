#pragma once

#include "legoapi/gizmo/base/gizmo.h"
#include "decomp_assert.h"

#ifdef __cplusplus

typedef struct LEDGE_s {
    char name[8];
    NUVEC position; // 0x08
    u16 y_rotation; // 0x14
    i8 type_code;
    u8 state_flags; // 0x17, active and visible in bits 0 and 1
    u8 type_index;
    u8 flags;
    u8 pad_1a[2];
    i16 field_0x1c;
    i16 field_0x1e;
    NUVEC bounds_min; // 0x20
    NUVEC bounds_max; // 0x2c
} LEDGE;

DECOMP_ASSERT(sizeof(LEDGE) == 0x38, "LEDGE ABI");
DECOMP_ASSERT(offsetof(LEDGE, position) == 0x08, "LEDGE position offset");
DECOMP_ASSERT(offsetof(LEDGE, state_flags) == 0x17, "LEDGE state offset");
DECOMP_ASSERT(offsetof(LEDGE, bounds_min) == 0x20, "LEDGE bounds offset");
struct WORLDINFO_s;
struct GameObject_s;
LEDGE *Ledge_FindNearest(WORLDINFO_s *, NUVEC *, GameObject_s *, f32 *);

ADDGIZMOTYPE *Ledges_RegisterGizmo(i32 type_id);

extern "C" {
#endif

#ifdef __cplusplus
}
#endif
