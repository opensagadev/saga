#pragma once

#include "nu2api/nucore/common.h"

struct GIZMO_PICKUP_TYPE;
struct GIZMOPICKUPSYS_s;
struct WORLDINFO_s;
struct GIZMOPICKUP_s;
struct GameObject_s;

extern u8 CoinTab[4];
extern GIZMO_PICKUP_TYPE GizmoPickupType[10];
extern GIZMOPICKUPSYS_s GizmoPickupSys_Game;

void Pup_CollectCoin(WORLDINFO_s *world, GIZMOPICKUP_s *pickup, i32 type, GameObject_s *object, i32 arg);
