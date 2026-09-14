#pragma once

#include "legoapi/gizmo/base/gizmo.h"

extern i32 hatmachine_gizmotype_id;

#ifdef __cplusplus

typedef struct HATMACHINE_s HATMACHINE;

ADDGIZMOTYPE *HatMachine_RegisterGizmo(i32 type_id);
void Hat_GetAbsTargetPos(HATMACHINE *machine, NUVEC *position);
void HatMachines_InitTerrain(WORLDINFO_s *world);
void HatMachine_MoveCode(WORLDINFO_s *world, GameObject_s *object, i32 special_pressed);
void HatMachine_FindNearest(WORLDINFO_s *world, nuvec_s *position, GameObject_s *object, f32 *distance);
i32 HatMachine_BeingUsed(HATMACHINE *machine);

#endif
