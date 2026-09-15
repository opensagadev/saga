#pragma once

#include "nu2api/nucore/fixed_width.h"
#include "nu2api/numath/numtx.h"

struct nuvec_s;
struct TERRSET;
struct TERRAIN_TRACK_SLOT;
struct tertype;
struct terrsitu_s;
struct PLATSKININFO;
typedef tertype TERRAIN_SHAPE;

extern "C" i32 IgnoreWallSplines;
extern "C" void PlatOnOff(i32 index, i32 enabled);
extern "C" void TerrainSetImpactData(void *impact_data, i32 *impact_count, i32 maximum_impacts);
extern "C" void TerrainPlatGetMtx(i32 index, NUMTX **previous, NUMTX **current);
extern "C" void TerrainSetPlatConnectTol(f32 tolerance);
TERRAIN_TRACK_SLOT *AllocTerrId(void);
void NewTerrStoreAnyInfo(void);
void DerotateMovementVector(void);
void RotateVec(nuvec_s *source, nuvec_s *destination);
nuvec_s TerCrossProduct(nuvec_s *a, nuvec_s *b);
void DeRotateTerrain(tertype *surface);
void DeRotatePoint(nuvec_s *point);
extern "C" f32 NewShadowEx(nuvec_s *position, i32 handle, f32 height_above, f32 height_below, i32 terrain_mask);
extern "C" TERRSET *TerrainGetCur(void);
extern "C" void TerrainSetCur(void *terrain);
extern "C" void noterraininit(void);
extern "C" void TerrSetPlatScanDist(f32 dist);
extern "C" void TerrainPlatformOldUpdate(void);
extern "C" void TerrainPlatformNewUpdate(void);
extern "C" void TerrainSetWallDeflectYScale(f32 scale);
extern "C" void FullDeflect(nuvec_s *normal, nuvec_s *movement, nuvec_s *result);
void FullDeflectSmallY(nuvec_s *normal, nuvec_s *movement, nuvec_s *result);
extern "C" void FullReflect(nuvec_s *normal, nuvec_s *movement, nuvec_s *result);
extern "C" void NewTerrainScaleYMask(nuvec_s *position, nuvec_s *movement, u8 *hit_flags, i32 object_index, f32 radius,
                                     f32 collision_radius, f32 object_scale, i32 embedded_retry, i32 scan_flags,
                                     i32 terrain_mask);
extern "C" void *TerrainInitEx(i32 level_num, void *buffer, void *buffer_end, i32 options, char *path, void *scene,
                               i32 group, u32 flags, u32 terrain_flags, u32 mode);
extern "C" i32 FindPlatInst(i32 instance_index);
extern "C" i32 DeletePlatinst(i32 platform_index);
extern "C" i32 NewPlatInst(void *object, i32 instance);
extern "C" i16 NewPlatPickupInst(void *object, i32 object_type);
extern "C" void PlatInstRotate(i32 platform_index, i32 enabled);
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
extern "C" i32 EShadowInfo(void);
extern "C" i32 EShadowRoofInfo(void);
extern "C" i32 ShadowIntensityInfo(void);
extern "C" void TerrainTrackFlush(void);
void NewScan(nuvec_s *, i32, i32);
void NewScanHandelFull(nuvec_s *, nuvec_s *, f32, i32, i32);
void NewScanHandelSubset(i16 *, nuvec_s *, nuvec_s *, f32, i32);
void DrawWallSpline(float alpha);
i32 CheckCylinder(i32 first_vertex, i32 second_vertex, i32 *vertex_mask, i32 remaining_vertex_mask);
i32 CheckSphere(i32 vertex_index);
i32 CheckSphereTer(nuvec_s *position, f32 radius);
i32 HitPoly(f32 primary_start, f32 primary_end, f32 secondary_start, f32 secondary_end, tertype *surface);
i32 HitTerrain(void);
i32 HitTerrPoly(tertype *surface, i32 group_index);
void RayImpact(nuvec_s *movement);
void PlatformConnect(char *track_id, nuvec_s *position_delta, nuvec_s *movement_delta, i32 platform_index);
void TerrainSkinAllocate(terrsitu_s *terrain_group);
void SkinPlatform(terrsitu_s *terrain_group, unsigned char *buffer, PLATSKININFO *info);
void SkinPlatformSize(i32 group_index, unsigned char *buffer, PLATSKININFO *info);
nuvec_s TerrainSkin(PLATSKININFO *info, nuvec_s *position, f32 scale, i32 flags);
extern i32 SkinFlipTab[8];
