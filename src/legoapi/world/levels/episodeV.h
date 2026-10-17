#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct AILOCATOR_s;
struct GameObject_s;
struct HOTHBATTLE_MELEE_s;
struct PART_s;
struct WORLDINFO_s;
struct nuvec_s;
extern AILOCATOR_s * locator;
i32 HothBattle_StartNewWave();
i32 TakeOverYodaSeekDistanceHack(GameObject_s *, GameObject_s *, nuvec_s *);
void Asteroid_PartKill(PART_s *, i32);
void HothBattleE_UpdateWave();
void HothBattle_ManageBackgroundCreatures();
void HothBattle_Melee_init(HOTHBATTLE_MELEE_s *);
void InitTrooperCannons(WORLDINFO_s *world);
void ResetTrooperCannons(WORLDINFO_s *world, i32 trooper_id);
void UpdateTrooperCannons(WORLDINFO_s *world);
extern "C" {
    extern HOTHBATTLE_MELEE_s melee;
}
