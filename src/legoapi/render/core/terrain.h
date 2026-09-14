#pragma once

#include "nu2api/nucore/fixed_width.h"

struct nuvec_s;

extern "C" i32 FindPlatInst(i32 instance_index);
extern "C" i32 TerrainPlatId();
extern "C" void NewTerrPlatformsOff(void);
extern "C" i32 NewRayCast(nuvec_s *origin, nuvec_s *direction, f32 distance, i32 flags);
extern "C" void NewRayCastGetImpactNormal(nuvec_s *normal);
extern "C" i32 NewRayCastGetImpactTerrainType(void);
extern "C" f32 NewRayCastGetTOFI(void);
extern "C" f32 NewRayCastGetEmbedDist(void);
extern "C" i32 NewRayCastHitWallSpline(void);
void DrawWallSpline(float alpha);
