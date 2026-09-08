#pragma once
#include "decomp.h"

#include "legoapi/gizmo/base/gizmo.h"

#ifdef __cplusplus

struct GameObject_s;
typedef struct SHARD_s {
    char name[0x10];
    union {
        char reserved_10[0x44];
        struct {
            NUVEC position; // 0x10
            union {
                u8 reserved_1c[0xc];
                NUVEC current_position;
            };
            NUVEC screen_position; // 0x28
            union {
                u8 reserved_34[2];
                i16 model_index;
            };
            u16 angle_x; // 0x36
            u16 angle_z; // 0x38
            union {
                u8 reserved_3a[2];
                u16 spin_angle;
            };
            u8 state_flags; // 0x3c
            u8 reserved_3d[3];
            f32 collection_time;       // 0x40
            GameObject_s *collector;   // 0x44
            NUVEC collection_velocity; // 0x48
        };
    };
} SHARD;

DECOMP_ASSERT(sizeof(SHARD_s) == 0x54, "SHARD ABI");
DECOMP_ASSERT(offsetof(SHARD_s, state_flags) == 0x3c, "SHARD state offset");
DECOMP_ASSERT(offsetof(SHARD_s, current_position) == 0x1c, "SHARD moving position offset");
DECOMP_ASSERT(offsetof(SHARD_s, model_index) == 0x34, "SHARD model offset");
DECOMP_ASSERT(offsetof(SHARD_s, spin_angle) == 0x3a, "SHARD spin offset");

ADDGIZMOTYPE *Shards_RegisterGizmo(i32 type_id);
struct WORLDINFO_s;
SHARD *Shard_FindNearest(WORLDINFO_s *, NUVEC *, GameObject_s *, f32 *);

extern "C" {
#endif

#ifdef __cplusplus
}
#endif
