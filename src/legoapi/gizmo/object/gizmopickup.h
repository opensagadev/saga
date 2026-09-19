#pragma once

#include "nu2api/nucore/common.h"

struct GIZMO_PICKUP_TYPE;
struct GIZMOPICKUPSYS_s;
struct WORLDINFO_s;
struct GIZMOPICKUP_s;
struct GameObject_s;
struct nuvec_s;

extern u8 CoinTab[4];
extern GIZMO_PICKUP_TYPE GizmoPickupType[10];
extern GIZMOPICKUPSYS_s GizmoPickupSys_Game;

void Pup_CollectCoin(WORLDINFO_s *world, GIZMOPICKUP_s *pickup, i32 type, GameObject_s *object, i32 arg);
i32 GetRandomCoinType(void);
void AddCoinsToPanel(i32 coins, nuvec_s *position, i32 player, f32 speed, GameObject_s *owner, i32 random_types);
void AddPickups(i32 coin_count, i32 heart_count, i32 pickup_count, i32 unknown, nuvec_s *position, nuvec_s *direction,
                f32 speed, i32 model, f32 radius, f32 duration, GameObject_s *owner, i32 flags, i32 extra,
                bool visible);
void AddMiscPickups(nuvec_s *position, i32 type, i32 count, i32 flags);
i32 IsACoinType(i32 type);
i32 LoseCoins(GameObject_s *object, i32 cause);
void GizmoPickups_SetOnOff(void);
