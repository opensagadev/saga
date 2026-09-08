#pragma once

#include "legoapi/gizmo/base/gizmo.h"
#include "decomp_assert.h"

#ifdef __cplusplus

struct GameObject_s;
struct WORLDINFO_s;

typedef struct TIGHTROPE_s {
    u8 reserved_00[0x10];
    NUVEC start;
    NUVEC end;
    u8 reserved_28[0x0c];
    NUVEC direction;
    f32 horizontal_length;
    u16 rotation;
    u8 enabled;
    u8 available;
} TIGHTROPE;

DECOMP_ASSERT(sizeof(TIGHTROPE) == 0x48, "TIGHTROPE size");
DECOMP_ASSERT(offsetof(TIGHTROPE, start) == 0x10, "TIGHTROPE start offset");
DECOMP_ASSERT(offsetof(TIGHTROPE, end) == 0x1c, "TIGHTROPE end offset");
DECOMP_ASSERT(offsetof(TIGHTROPE, direction) == 0x34, "TIGHTROPE direction offset");
DECOMP_ASSERT(offsetof(TIGHTROPE, rotation) == 0x44, "TIGHTROPE rotation offset");

TIGHTROPE *TightRope_InRange(GameObject_s *object, WORLDINFO_s *world, NUVEC *position);
i32 TightRope_SnapTo(GameObject_s *object, NUVEC *position);
i32 TightRope_SetTargetMom(GameObject_s *object);
void TightRope_MoveCode(GameObject_s *object, i32 jump_pressed);
TIGHTROPE *TightRope_FindNearest(NUVEC *position, WORLDINFO_s *world, i32 *endpoint, f32 *distance_squared);

ADDGIZMOTYPE *TightRopes_RegisterGizmo(i32 type_id);

extern "C" {
#endif

#ifdef __cplusplus
}
#endif
