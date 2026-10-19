#pragma once

#include "legoapi/legoapi_types.h"

extern "C" TERRAIN_SURFACE_s TerSurface[32];

// Declared here for the other units that use them.
struct GameObject_s;
struct nuvec_s;
i32 IntersectWater(GameObject_s *object);
i32 UnderWater(GameObject_s *object);
void GetSurfaceInfo(GameObject_s *object, i32 update_surface, f32 shadow_height);
void SurfaceMaskOff(u32 *surface_mask);
void SurfaceMaskOn(u32 *surface_mask);
void Surface_Deflect(nuvec_s *normal, nuvec_s *movement, nuvec_s *result, i32 mode);
void Surfaces_Reset(void);
extern "C" {
    extern u16 SURFACEBITS_DUST;
}
