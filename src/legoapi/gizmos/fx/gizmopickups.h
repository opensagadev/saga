#pragma once

#include "legoapi/gizmo/base/gizmo.h"

extern i32 gizmopickup_typeid;
extern f32 AreaPickupScale;
extern f32 COINMSGTIME;
extern i32 PickUpFlickerTest;
extern i32 PickUpFlickerFrames;
extern i32 PickupFlickerFrame;

#ifdef __cplusplus

typedef struct GIZMOPICKUP_s GIZMOPICKUP;
struct GIZMOPICKUPSYS_s;

ADDGIZMOTYPE *GizmoPickups_RegisterGizmo(i32 type_id);
void GizmoPickups_InitSys(GIZMOPICKUPSYS_s *pickup_sys);
void SpecialMiniKits_Configure(WORLDINFO_s *world, char *config);
void SpecialMiniKits_Reset(WORLDINFO_s *world);
void SpecialMiniKits_Draw(WORLDINFO_s *world);
void GizmoPickup_CollectCoin(WORLDINFO_s *world, NUVEC *position, i32 type_index, i32 model_variant,
                             GameObject_s *object, i32 flags);
GIZMOPICKUP_s *GizmoPickup_FindByName(WORLDINFO_s *world, char *name);
i32 GizmoPickup_BeenTurnedOn(GIZMOPICKUP_s *pickup);
GIZMOPICKUP_s *GizmoPickup_InBox(WORLDINFO_s *world, i32 type_index, NUVEC *minimum, NUVEC *maximum);
GIZMOPICKUP_s *GizmoPickup_FindNearest(WORLDINFO_s *world, NUVEC *position, f32 *distance);
i32 GizmoPickup_NumberOfType(WORLDINFO_s *world, i32 type_index, char type_code);
u32 GizmoPickups_TotalScore(void *world);

#endif
