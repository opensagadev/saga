#pragma once

#include "decomp.h"

struct GameObject_s;
struct numtx_s;
struct WORLDINFO_s;
struct speedup_s;
struct starfighter_s;
struct BOLT_s;
struct nuvec_s;

void SpaceResetAudioPoint();
void ProcessCurrentSpeed(WORLDINFO_s *world, speedup_s *speed_list);
extern speedup_s DogFightSpeedList[];
i32 ShipDropCoins(starfighter_s *fighter);
i32 ChrisExtraBoltCollision(BOLT_s *bolt, nuvec_s *points);

void ChrisGetSpaceShipMatrix(GameObject_s *object, numtx_s *matrix);
void ChrisGetTargetedSpaceShipMatrix(GameObject_s *object, numtx_s *matrix);
