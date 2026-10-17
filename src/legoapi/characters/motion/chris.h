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

// Declared here for the other units that use them.
struct BOLT_s;
struct WORLDINFO_s;
struct _vum_s;
struct _vuv_s;
struct flightspline_s;
struct nuvec_s;
i32 DidBoltHitChrisJobby(WORLDINFO_s *, BOLT_s *);
void CalcSplinePointFromDist(flightspline_s *spline, _vuv_s *result, float distance);
void ChrisAfterBurnerCam(nuvec_s *, nuvec_s *camera);
void ChrisAnakinADraw();
void ChrisAnakinAInit(WORLDINFO_s *world);
void ChrisAnakinAPanel(WORLDINFO_s *);
void ChrisAnakinAReset(WORLDINFO_s *world);
void ChrisAnakinAUpdate(WORLDINFO_s *);
void ChrisAnakinBDraw();
void ChrisAnakinBInit();
void ChrisAnakinBReset();
void ChrisAnakinBUpdate();
void ChrisAnakinCInit();
void ChrisAnakinCReset();
void ChrisAnakinCUpdate();
void ChrisAnakinDDraw();
void ChrisAnakinDInit(WORLDINFO_s *world);
void ChrisAnakinDReset(WORLDINFO_s *world);
void ChrisAnakinDUpdate(WORLDINFO_s *);
void ChrisDogFightAPanel(WORLDINFO_s *);
void ChrisRadialCam(nuvec_s *position, nuvec_s *target);
void ChrisUnallocLevelStuff(WORLDINFO_s *world);
void DogFightARestart();
void DrawArrow_Now(_vum_s *, float, i32, i32);
void DrawBoxMtx_Now(_vum_s *, _vuv_s *, i32, i32);
void DrawBox_Now(_vuv_s *, _vuv_s *, i32, i32);
void DrawCross_Now(_vuv_s *position, f32 size, i32 colour, i32 mode);
void DrawLine_Now(_vuv_s *, _vuv_s *, i32, i32);
void SpaceAudioPoint();
