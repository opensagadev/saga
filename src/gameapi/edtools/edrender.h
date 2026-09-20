#pragma once

#include "decomp.h"
#include "nu2api/numath/nuvec.h"

struct numtl_s;

extern "C" {
    void edbitsDrawBasicCube(f32 x, f32 y, f32 z, f32 half_x, f32 half_y, f32 half_z, i32 rotation_x, i32 rotation_y,
                             i32 rotation_z, i32 colour, numtl_s *material);
    void edbitsDrawCircleXY(NUVEC *centre, f32 radius, i32 colour, i32 unused);
    void edbitsDrawCube(f32 x, f32 y, f32 z, f32 half_x, f32 half_y, f32 half_z, i32 rotation_z, i32 rotation_y,
                        i32 rotation_x, i32 outer_z, i32 outer_y, i32 colour, numtl_s *material);
    void edbitsDrawCross(f32 x, f32 y, f32 z, f32 radius, i32 colour, numtl_s *material);
    void edbitsDrawDiagonalCross(f32 x, f32 y, f32 z, f32 radius, i32 colour, numtl_s *material);
    void edbitsDrawOvalTilted(NUVEC *centre, f32 radius_x, f32 radius_z, i32 colour, i32 unused, i32 rotation_z,
                              i32 rotation_y);
    void edbitsDrawTorus(NUVEC *centre, f32 radius, f32 radial_extent, f32 vertical_extent, i32 colour, i32 unused);
}
