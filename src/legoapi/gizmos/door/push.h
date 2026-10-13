#pragma once

#include "legoapi/gizmo/base/gizmo.h"

extern i32 pushblock_gizmotype_id;

#ifdef __cplusplus

struct PUSHPROGRESS {
    // One bit per push block; 16 blocks fit in a single word.
    u32 visible_mask[1];
    u32 state_mask[1];
    u32 position_mask[1];
    NUVEC positions[16];
    NUVEC end_positions[2][16];
};

typedef struct PUSHBLOCK_s {
    char unknown_00[0x44];
    char name[0x40];
    char unknown_084[0x4c];
} PUSHBLOCK;

ADDGIZMOTYPE *Push_RegisterGizmo(i32 type_id);

extern "C" {
#endif

#ifdef __cplusplus
}
#endif
