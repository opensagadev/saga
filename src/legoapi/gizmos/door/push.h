#pragma once

#include "legoapi/gizmo/base/gizmo.h"

extern i32 pushblock_gizmotype_id;

#ifdef __cplusplus

struct PUSHPROGRESS {
    u32 visible_mask;
    u32 state_mask;
    u32 position_mask;
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
