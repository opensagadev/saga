#pragma once

#include "legoapi/gizmo/base/gizmo.h"
#include "decomp.h"
#include "nu2api/numath/numtx.h"
#include <stddef.h>

#ifdef __cplusplus

typedef struct SECURITYDOOR_s {
    NUMTX leaf_matrix[2];
    char name[0x10];
    NUVEC position;
    i16 platform_id[2];
    u16 yaw;
    union {
        u16 flags;
        struct {
            u16 active : 1;
            u16 visible : 1;
            u16 opened : 1;
            u16 flag_08 : 1;
            u16 reserved_flags : 4;
            u16 state : 8;
        };
    };
    NUVEC player_position;
    u16 terrain_angle_z;
    u16 terrain_angle_x;
    f32 opening;
    char unknown_b8[8];
} SECURITYDOOR;

DECOMP_ASSERT(sizeof(SECURITYDOOR) == 0xc0, "SECURITYDOOR size");
DECOMP_ASSERT(offsetof(SECURITYDOOR, name) == 0x80, "SECURITYDOOR name");
DECOMP_ASSERT(offsetof(SECURITYDOOR, position) == 0x90, "SECURITYDOOR position");
DECOMP_ASSERT(offsetof(SECURITYDOOR, flags) == 0xa2, "SECURITYDOOR packed flags and state");
DECOMP_ASSERT(offsetof(SECURITYDOOR, player_position) == 0xa4, "SECURITYDOOR player position");

ADDGIZMOTYPE *SecurityDoors_RegisterGizmo(i32 type_id);
void SecurityDoors_InitTerrain(WORLDINFO_s *);
SECURITYDOOR *SecurityDoor_FindNearest(WORLDINFO_s *, NUVEC *, GameObject_s *, f32 *);
void SecurityDoor_MoveCode(WORLDINFO_s *, GameObject_s *);

extern "C" {
#endif

#ifdef __cplusplus
}
#endif
