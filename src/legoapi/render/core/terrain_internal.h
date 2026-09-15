#pragma once

#include "legoapi/render/core/terrain.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/numath/nuvec.h"

struct TerrainLastImpact_s {
    NUVEC position;
    f32 hit_type;
};

extern TerrainLastImpact_s TerrLastImpact;
extern u8 TerrainHitInfo[4];
extern TERRSET *CurTerr;
extern i32 WallSplinesOnly;
extern TERRAIN_SPHERE SphereData[16];
extern NUVEC ShadNorm;
extern NUVEC ShadRoofNorm;
extern NUVEC EShadNorm;
extern NUVEC EShadRoofNorm;
extern f32 ShadRoofY;
extern f32 EShadRoofY;
extern i32 terrhitflags;
extern i32 TERRAINMASK_NONWEAPON;
extern i32 TERRAINMASK_NONDROID;
