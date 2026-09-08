#pragma once

#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/gizmo/base/gizmo.h"

#ifdef __cplusplus

typedef struct LEVELSCRIPTPROCESS_s {
    char name[0x10];
    AISCRIPTPROCESS processor;

    u32 unknown_d8;
} LEVELSCRIPTPROCESS;
DECOMP_ASSERT(sizeof(LEVELSCRIPTPROCESS) == 0xdc, "Level script processor size");
DECOMP_ASSERT(offsetof(LEVELSCRIPTPROCESS, processor) == 0x10, "Level script processor state offset");

typedef struct AI_s {
} AI;

ADDGIZMOTYPE *AI_RegisterGizmo(i32 type_id);

extern "C" {
#endif

#ifdef __cplusplus
}
#endif
