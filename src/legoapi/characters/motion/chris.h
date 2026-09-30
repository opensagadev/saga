#pragma once

struct GameObject_s;
struct numtx_s;
struct WORLDINFO_s;
struct speedup_s;

void SpaceResetAudioPoint();
void ProcessCurrentSpeed(WORLDINFO_s *world, speedup_s *speed_list);
extern speedup_s DogFightSpeedList[];

void ChrisGetSpaceShipMatrix(GameObject_s *object, numtx_s *matrix);
void ChrisGetTargetedSpaceShipMatrix(GameObject_s *object, numtx_s *matrix);
