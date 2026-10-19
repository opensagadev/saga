#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain_internal.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stddef.h>

i16 TerrImpact;
i32 terrhitflags;
i32 TERRAINMASK_NONWEAPON = 0x20;
i32 TERRAINMASK_NONDROID;
NUVEC TerrImpactPos;
NUVEC TerrImpactNormal;
NUVEC ShadNorm;
NUVEC ShadRoofNorm;
NUVEC EShadNorm;
NUVEC EShadRoofNorm;
f32 ShadRoofY;
f32 EShadRoofY;
TerrainLastImpact_s TerrLastImpact;
i32 WallSplinesOnly;
extern "C" {
    i32 IgnoreWallSplines;
}
