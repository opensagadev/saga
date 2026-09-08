#pragma once
#include "decomp.h"
#include "nu2api/numath/numtx.h"

#include "legoapi/gizmo/base/gizmo.h"

#ifdef __cplusplus

typedef struct ATTRACTO_s {
    char name[0x10];
    union {
        char reserved_10[0x64];
        struct {
            NUVEC position;     // 0x10
            NUMTX transform;    // 0x1c
            u16 angle;          // 0x5c
            u8 capacity;        // 0x5e
            u8 collected_count; // 0x5f
            i16 platform_id;    // 0x60
            u8 state_flags;     // 0x62
            u8 reserved_63;
            NUVEC active_position; // 0x64
            u16 ground_angle_z;    // 0x70
            u16 ground_angle_x;    // 0x72
        };
    };
} ATTRACTO;

DECOMP_ASSERT(sizeof(ATTRACTO_s) == 0x74, "ATTRACTO ABI");
DECOMP_ASSERT(offsetof(ATTRACTO_s, angle) == 0x5c, "ATTRACTO angle offset");
DECOMP_ASSERT(offsetof(ATTRACTO_s, active_position) == 0x64, "ATTRACTO active position offset");
struct GameObject_s;
ATTRACTO_s *Attracto_FindNearest(WORLDINFO_s *, NUVEC *, GameObject_s *, f32 *);
void Attracto_GetPos_Top(ATTRACTO_s *, NUVEC *);

ADDGIZMOTYPE *Attractos_RegisterGizmo(i32 type_id);

extern "C" {
#endif

#ifdef __cplusplus
}
#endif
