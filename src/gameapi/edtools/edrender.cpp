#include "gameapi/edtools/edrender.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nutrig.h"

extern "C" void edbitsDrawBasicCube(f32 x, f32 y, f32 z, f32 half_x, f32 half_y, f32 half_z, i32 rotation_x,
                                    i32 rotation_y, i32 rotation_z, i32 colour, numtl_s *material) {
    NUVEC outlines[4][5] = {{{-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}, {-1, -1, 1}},
                            {{-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1}, {-1, -1, -1}},
                            {{-1, -1, 1}, {-1, -1, -1}, {-1, 1, -1}, {-1, 1, 1}, {-1, -1, 1}},
                            {{1, -1, 1}, {1, -1, -1}, {1, 1, -1}, {1, 1, 1}, {1, -1, 1}}};
    NUVEC endpoints[2];
    NURND_VERTEX3D vertices[2];
    for (i32 face = 0; face < 4; ++face) {
        for (i32 edge = 0; edge < 4; ++edge) {
            endpoints[0].x = half_x * outlines[face][edge].x;
            endpoints[0].y = half_y * outlines[face][edge].y;
            endpoints[0].z = half_z * outlines[face][edge].z;
            endpoints[1].x = half_x * outlines[face][edge + 1].x;
            endpoints[1].y = half_y * outlines[face][edge + 1].y;
            endpoints[1].z = half_z * outlines[face][edge + 1].z;
            NuVecRotateX(&endpoints[0], &endpoints[0], rotation_x);
            NuVecRotateY(&endpoints[0], &endpoints[0], rotation_y);
            NuVecRotateZ(&endpoints[0], &endpoints[0], rotation_z);
            NuVecRotateX(&endpoints[1], &endpoints[1], rotation_x);
            NuVecRotateY(&endpoints[1], &endpoints[1], rotation_y);
            NuVecRotateZ(&endpoints[1], &endpoints[1], rotation_z);
            vertices[0].colour = colour;
            vertices[1].colour = colour;
            vertices[0].position.x = x + endpoints[0].x;
            vertices[0].position.y = y + endpoints[0].y;
            vertices[0].position.z = z + endpoints[0].z;
            vertices[1].position.x = x + endpoints[1].x;
            vertices[1].position.y = y + endpoints[1].y;
            vertices[1].position.z = z + endpoints[1].z;
            NuRndrLine3d(vertices, material, NULL);
        }
    }
}

extern "C" void edbitsDrawCube(f32 x, f32 y, f32 z, f32 half_x, f32 half_y, f32 half_z, i32 rotation_z, i32 rotation_y,
                               i32 rotation_x, i32 outer_z, i32 outer_y, i32 colour, numtl_s *material) {
    NUVEC outlines[4][5] = {{{-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}, {-1, -1, 1}},
                            {{-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1}, {-1, -1, -1}},
                            {{-1, -1, 1}, {-1, -1, -1}, {-1, 1, -1}, {-1, 1, 1}, {-1, -1, 1}},
                            {{1, -1, 1}, {1, -1, -1}, {1, 1, -1}, {1, 1, 1}, {1, -1, 1}}};
    NUVEC endpoints[2];
    NURND_VERTEX3D vertices[2];
    for (i32 face = 0; face < 4; ++face) {
        for (i32 edge = 0; edge < 4; ++edge) {
            endpoints[0].x = half_x * outlines[face][edge].x;
            endpoints[0].y = half_y * outlines[face][edge].y;
            endpoints[0].z = half_z * outlines[face][edge].z;
            endpoints[1].x = half_x * outlines[face][edge + 1].x;
            endpoints[1].y = half_y * outlines[face][edge + 1].y;
            endpoints[1].z = half_z * outlines[face][edge + 1].z;
            NuVecRotateZ(&endpoints[0], &endpoints[0], rotation_z);
            NuVecRotateY(&endpoints[0], &endpoints[0], rotation_y);
            NuVecRotateX(&endpoints[0], &endpoints[0], rotation_x);
            NuVecRotateZ(&endpoints[0], &endpoints[0], outer_z);
            NuVecRotateY(&endpoints[0], &endpoints[0], outer_y);
            NuVecRotateZ(&endpoints[1], &endpoints[1], rotation_z);
            NuVecRotateY(&endpoints[1], &endpoints[1], rotation_y);
            NuVecRotateX(&endpoints[1], &endpoints[1], rotation_x);
            NuVecRotateZ(&endpoints[1], &endpoints[1], outer_z);
            NuVecRotateY(&endpoints[1], &endpoints[1], outer_y);
            vertices[0].colour = colour;
            vertices[1].colour = colour;
            vertices[0].position.x = x + endpoints[0].x;
            vertices[0].position.y = y + endpoints[0].y;
            vertices[0].position.z = z + endpoints[0].z;
            vertices[1].position.x = x + endpoints[1].x;
            vertices[1].position.y = y + endpoints[1].y;
            vertices[1].position.z = z + endpoints[1].z;
            NuRndrLine3d(vertices, material, NULL);
        }
    }
}

extern "C" void edbitsDrawDiagonalCross(f32 x, f32 y, f32 z, f32 radius, i32 colour, numtl_s *material) {
    NUVEC axes[4] = {{1.0f, 1.0f, 1.0f}, {-1.0f, 1.0f, 1.0f}, {1.0f, -1.0f, 1.0f}, {-1.0f, -1.0f, 1.0f}};
    NURND_VERTEX3D vertices[2];
    for (i32 i = 0; i < 4; ++i) {
        vertices[0].colour = colour;
        vertices[1].colour = colour;
        vertices[0].position.x = x + radius * axes[i].x;
        vertices[0].position.y = y + radius * axes[i].y;
        vertices[0].position.z = z + radius * axes[i].z;
        vertices[1].position.x = x - radius * axes[i].x;
        vertices[1].position.y = y - radius * axes[i].y;
        vertices[1].position.z = z - radius * axes[i].z;
        NuRndrLine3d(vertices, material, NULL);
    }
}

extern "C" void edbitsDrawCross(f32 x, f32 y, f32 z, f32 radius, i32 colour, numtl_s *material) {
    NUVEC axes[3] = {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};
    NURND_VERTEX3D vertices[2];
    for (i32 i = 0; i < 3; ++i) {
        vertices[0].colour = colour;
        vertices[1].colour = colour;
        vertices[0].position.x = x + radius * axes[i].x;
        vertices[0].position.y = y + radius * axes[i].y;
        vertices[0].position.z = z + radius * axes[i].z;
        vertices[1].position.x = x - radius * axes[i].x;
        vertices[1].position.y = y - radius * axes[i].y;
        vertices[1].position.z = z - radius * axes[i].z;
        NuRndrLine3d(vertices, material, NULL);
    }
}

extern "C" void edbitsDrawOvalTilted(NUVEC *centre, f32 radius_x, f32 radius_z, i32 colour, i32, i32 rotation_z,
                                     i32 rotation_y) {
    NUVEC endpoints[2];
    NUVEC &previous = endpoints[0];
    NUVEC &point = endpoints[1];
    point.x = 0.0f;
    point.y = 0.0f;
    point.z = radius_z;
    if (rotation_z)
        NuVecRotateZ(&point, &point, rotation_z);
    if (rotation_y)
        NuVecRotateY(&point, &point, rotation_y);
    point.x += centre->x;
    point.y += centre->y;
    point.z += centre->z;
    for (i32 i = 1; i <= 10; ++i) {
        previous = point;
        i32 angle = i * 65536 / 10;
        point.x = radius_x * NU_SIN_LUT(angle);
        point.y = 0.0f;
        point.z = radius_z * NU_COS_LUT(angle);
        if (rotation_z)
            NuVecRotateZ(&point, &point, rotation_z);
        if (rotation_y)
            NuVecRotateY(&point, &point, rotation_y);
        point.x += centre->x;
        point.y += centre->y;
        point.z += centre->z;
        NuRndrLine3dDbg(previous.x, previous.y, previous.z, point.x, point.y, point.z, colour);
    }
}

extern "C" void edbitsDrawTorus(NUVEC *centre, f32 radius, f32 radial_extent, f32 vertical_extent, i32 colour,
                                i32 unused) {
    edbitsDrawCircleXY(centre, radius - radial_extent, colour, unused);
    edbitsDrawCircleXY(centre, radius + radial_extent, colour, unused);
    NUVEC offset = *centre;
    offset.y -= vertical_extent;
    edbitsDrawCircleXY(&offset, radius, colour, unused);
    offset = *centre;
    offset.y += vertical_extent;
    edbitsDrawCircleXY(&offset, radius, colour, unused);
    for (i32 i = 0; i <= 10; ++i) {
        i32 angle = i * 65536 / 10;
        NUVEC point;
        point.x = centre->x;
        point.y = centre->y;
        point.z = centre->z;
        point.x += radius * NU_SIN_LUT(angle);
        point.z += radius * NU_COS_LUT(angle);
        edbitsDrawOvalTilted(&point, vertical_extent, radial_extent, colour, unused, 0x4000, angle);
    }
}
