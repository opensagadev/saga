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
extern "C" i32 TerrainIntensityInfo(void);
extern "C" i32 TerrainInfo(void);
extern "C" i32 TerrainInfoExtra(void);
extern "C" i32 ShadowInfo(void);
extern "C" i32 ShadowIntensityInfo(void);
extern "C" void TerrainTrackFlush(void);
void NewScan(nuvec_s *, i32, i32);
void NewScanHandelFull(nuvec_s *, nuvec_s *, f32, i32, i32);
void NewScanHandelSubset(i16 *, nuvec_s *, nuvec_s *, f32, i32);
void DrawWallSpline(float alpha);
