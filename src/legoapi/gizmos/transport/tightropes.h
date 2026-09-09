#pragma once

#include "decomp.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "decomp.h"

#ifdef __cplusplus

struct GameObject_s;
struct WORLDINFO_s;

typedef struct TIGHTROPE_s {
    char name[16];
    union {
        NUVEC start_position;
        NUVEC start;
    }; // 0x10
    union {
        NUVEC end_position;
        NUVEC end;
    }; // 0x1c
    u16 field_0x28;
    u16 field_0x2a;
    u16 field_0x2c;
    u16 field_0x2e;
    u8 field_0x30;
    u8 field_0x31;
    i8 field_0x32;
    u8 pad_33;
    NUVEC direction; // 0x34, normalized end minus start
    union {
        f32 length;
        f32 horizontal_length;
    }; // 0x40
    union {
        u16 rotation;
        u16 y_rotation;
        u16 angle;
    }; // 0x44
    union {
        u8 visible;
        u8 enabled;
    };
    union {
        u8 active;
        u8 available;
    };
} TIGHTROPE;

DECOMP_ASSERT(sizeof(TIGHTROPE) == 0x48, "TIGHTROPE ABI");
DECOMP_ASSERT(offsetof(TIGHTROPE, direction) == 0x34, "TIGHTROPE direction offset");
DECOMP_ASSERT(offsetof(TIGHTROPE, length) == 0x40, "TIGHTROPE length offset");
DECOMP_ASSERT(offsetof(TIGHTROPE, y_rotation) == 0x44, "TIGHTROPE yaw offset");
DECOMP_ASSERT(offsetof(TIGHTROPE, active) == 0x47, "TIGHTROPE active offset");

ADDGIZMOTYPE *TightRopes_RegisterGizmo(i32 type_id);
struct GameObject_s;
struct WORLDINFO_s;
TIGHTROPE *TightRope_InRange(GameObject_s *object, WORLDINFO_s *world, NUVEC *target);
TIGHTROPE *TightRope_FindNearest(NUVEC *position, WORLDINFO_s *world, i32 *endpoint, f32 *distance_squared);
i32 TightRope_SnapTo(GameObject_s *object, NUVEC *position);

extern "C" {
#endif

#ifdef __cplusplus
}
#endif

void TightRope_MoveCode(GameObject_s *, i32);
