#pragma once

#include "decomp.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "nu2api/nu3d/nuhspecial.h"

extern i32 grapple_gizmotype_id;

#ifdef __cplusplus

enum GRAPPLE_FLAGS {
    GRAPPLE_FLAG_ACTIVE = 1 << 0,
    GRAPPLE_FLAG_VISIBLE = 1 << 1,
    GRAPPLE_FLAG_DISABLED = 1 << 2,
    GRAPPLE_FLAG_REVERSED = 1 << 3,
};

typedef struct GRAPPLE_s {
    char name[16];           // 0x00
    NUVEC position;          // 0x10
    u8 has_terrain_platform; // 0x1c
    u8 retain_attachment;    // 0x1d
    union {
        u8 flags;
        struct {
            u8 active : 1;
            u8 visible : 1;
            u8 disabled : 1;
            u8 reversed : 1;
            u8 reserved_flags : 4;
        };
    };
    u8 field_0x1f;
    u16 y_rotation;              // 0x20
    u16 x_rotation;              // 0x22
    NUVEC ground_position;       // 0x24
    NUVEC hook_position;         // 0x30
    NUVEC shadow_probe_position; // 0x3c
    u16 ground_x_rotation;       // 0x48
    u16 ground_z_rotation;       // 0x4a
    f32 field_0x4c;
    f32 rope_length;                      // 0x50
    struct GameObject_s *attached_object; // 0x54
    f32 activation_progress;              // 0x58
    NUVEC rope_points[6];                 // 0x5c
    NUVEC target_rope_points[6];          // 0xa4
    nuhspecial_s display_special;         // 0xec
} GRAPPLE;

DECOMP_ASSERT(sizeof(GRAPPLE) == 0xf8, "GRAPPLE ABI");
DECOMP_ASSERT(offsetof(GRAPPLE, has_terrain_platform) == 0x1c, "GRAPPLE platform-state offset");
DECOMP_ASSERT(offsetof(GRAPPLE, flags) == 0x1e, "GRAPPLE flags offset");
DECOMP_ASSERT(offsetof(GRAPPLE, ground_position) == 0x24, "GRAPPLE ground-position offset");
DECOMP_ASSERT(offsetof(GRAPPLE, hook_position) == 0x30, "GRAPPLE hook-position offset");
DECOMP_ASSERT(offsetof(GRAPPLE, attached_object) == 0x54, "GRAPPLE attached-object offset");
DECOMP_ASSERT(offsetof(GRAPPLE, activation_progress) == 0x58, "GRAPPLE activation progress offset");
DECOMP_ASSERT(offsetof(GRAPPLE, target_rope_points) == 0xa4, "GRAPPLE target-points offset");
DECOMP_ASSERT(offsetof(GRAPPLE, display_special) == 0xec, "GRAPPLE display-special offset");

ADDGIZMOTYPE *Grapples_RegisterGizmo(i32 type_id);
GRAPPLE *Grapple_FindNearest(struct WORLDINFO_s *world, NUVEC *position, GameObject_s *object, f32 *distance_squared);
GRAPPLE *Grapple_FindNearestToPos(struct WORLDINFO_s *world, NUVEC *position);
void Grapple_DrawLine(GameObject_s *object);
i32 Grapple_ReachedTop(GameObject_s *object);
i32 Grapple_LookAtPos(GameObject_s *object, NUVEC *position);
i32 Grapple_SetTargetMom(GameObject_s *object);
void Grapple_SetRotOrder(GameObject_s *object);
void Grapple_SetPlayerTargetPoint(GameObject_s *object, NUVEC *target);

extern "C" {
#endif

#ifdef __cplusplus
}
#endif

// Declared here for the other units that use them.
struct AIPATHCNX_s;
struct GRAPPLE_s;
struct GameObject_s;
GRAPPLE_s *Grapple_AddDynamic(void *, i32);
GameObject_s *Grapple_Occupied(GRAPPLE_s *, GameObject_s *, AIPATHCNX_s *);
void Grapple_MoveCode(GameObject_s *);
void Grapple_RemoveDynamic(void *);
