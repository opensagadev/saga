#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec4.h"
#include <stdio.h>

i32 NuBridgeAlloc(void);
void ropesegment(numtl_s *, NUVEC *, i32, i32) {
}

void NuBrdigeDrawRope(numtl_s *material, NUVEC *first, NUVEC *second, i32, i32 *boundaries, i32 colour) {
    for (i32 i = 0; boundaries[i] < boundaries[i + 1]; ++i) {
        ropesegment(material, &first[boundaries[i]], boundaries[i + 1] - boundaries[i], colour);
        ropesegment(material, &second[boundaries[i]], boundaries[i + 1] - boundaries[i], colour);
    }
}

extern "C" {
    struct NUBRIDGE {
        u8 active;
        u8 unknown_01;
        i16 field_02;
        NUGSCN *scene;
        void *terrain;
        NUVEC edge_positions[24][2];
        NUVEC edge_motion[24][2];
        NUMTX platform_matrices[24];
        nuhspecial_s special;
        nuhspecial_s second_special;
        NUVEC centre;
        f32 cull_radius_squared;
        i16 platforms[24];
        u8 field_ae4;
        u8 field_ae5;
        i8 platform_count;
        i8 field_ae7;
        u32 colour;
        u16 field_aec;
        i16 rotation_y;
        f32 field_af0, field_af4, field_af8, field_afc;
        f32 field_b00, field_b04, field_b08;
    };
    DECOMP_ASSERT(sizeof(NUBRIDGE) == 0xb0c, "NUBRIDGE size");
    NUBRIDGE Bridges[8];
    i32 NuBridgeProc;
    void NuBridgeDraw(numtl_s *material) {
        i32 b, i;
        NUBRIDGE *bridge;
        i32 n, group;
        NUVEC4 point;
        NUVEC first[512], second[512];
        NUMTX matrix;
        i32 boundaries[8];
        NUVEC4 offsets[2];
        if (NuBridgeProc) {
            bridge = Bridges;
            n = 0;
            group = 0;
            boundaries[group] = 0;
            for (b = 0; b < 8; ++b, ++bridge) {
                if (bridge->active && bridge->field_ae4) {
                    bridge->field_ae5 = 0;
                    NuMtxSetRotationY(&matrix, bridge->rotation_y);
                    offsets[0].x = 0.0f;
                    offsets[0].y = 0.0f;
                    offsets[0].z = -bridge->field_b04;
                    offsets[0].w = 1.0f;
                    offsets[1].x = 0.0f;
                    offsets[1].y = 0.0f;
                    offsets[1].z = bridge->field_b04;
                    offsets[1].w = 1.0f;
                    for (i = 0; i < bridge->platform_count; ++i) {
                        if (NuSpecialExistsFn(&bridge->special)) {
                            if (NuSpecialDrawAt(&bridge->special, &bridge->platform_matrices[i]))
                                bridge->field_ae5 = 1;
                        }
                        if (i % bridge->field_ae7 == 0 && NuSpecialExistsFn(&bridge->second_special)) {
                            NuVec4MtxTransformVU0(&point, &offsets[0], &bridge->platform_matrices[i]);
                            if (n < 512) {
                                first[n].x = point.x;
                                first[n].y = point.y + bridge->field_b08;
                                first[n].z = point.z;
                            }
                            matrix.m30 = point.x;
                            matrix.m31 = point.y;
                            matrix.m32 = point.z;
                            NuSpecialDrawAt(&bridge->second_special, &matrix);
                            point.x = 0.0f;
                            point.y = 0.0f;
                            point.z = 1.0f;
                            point.w = 1.0f;
                            NuVec4MtxTransformVU0(&point, &offsets[1], &bridge->platform_matrices[i]);
                            if (n < 512) {
                                second[n].x = point.x;
                                second[n].y = point.y + bridge->field_b08;
                                second[n].z = point.z;
                                ++n;
                            }
                            matrix.m30 = point.x;
                            matrix.m31 = point.y;
                            matrix.m32 = point.z;
                            NuSpecialDrawAt(&bridge->second_special, &matrix);
                        }
                    }
                    if (boundaries[group] != n) {
                        ++group;
                        boundaries[group] = n;
                    }
                }
            }
            boundaries[group + 1] = n;
            if (n > 1)
                NuBrdigeDrawRope(material, first, second, n, boundaries, Bridges[0].colour);
        }
    }
    void NuBridgeOn(i32 enabled) {
        NuBridgeProc = enabled;
    }
    void *TerrainGetCur(void);
    void TerrainSetCur(void *terrain);
    i32 DeletePlatinst(i32 index);
    i32 NewPlatInst(NUMTX *matrix, i32 instance);
    i32 NuBridgeCreate(NUGSCN *scene, nuhspecial_s *first, nuhspecial_s *second, NUVEC *start, NUVEC *end, f32 width,
                       i16 rotation, f32 af4, f32 afc, f32 af8, f32 b00, i32 count, f32 b04, f32 b08, i32 ae7,
                       u32 colour) {
        if (count > 24)
            printf("Too many sections/n");
        NuBridgeOn(1);
        i32 index = NuBridgeAlloc();
        if (index != -1) {
            NUBRIDGE *bridge = &Bridges[index];
            bridge->special = *first;
            bridge->second_special = *second;
            bridge->scene = scene;
            bridge->active = 1;
            bridge->field_02 = 0;
            bridge->terrain = TerrainGetCur();
            bridge->platform_count = count;
            bridge->field_af4 = af4;
            bridge->field_afc = afc;
            bridge->field_af0 = width;
            bridge->field_af8 = af8;
            bridge->field_b00 = b00;
            bridge->field_b04 = b04;
            bridge->field_b08 = b08;
            bridge->field_ae7 = ae7;
            bridge->colour = colour;
            bridge->centre.x = (end->x + start->x) * 0.5f;
            bridge->centre.y = (end->y + start->y) * 0.5f;
            bridge->centre.z = (end->z + start->z) * 0.5f;
            NUVEC delta = {end->x - start->x, end->y - start->y, end->z - start->z};
            bridge->cull_radius_squared = (delta.x * 0.5f) * (delta.x * 0.5f) + (delta.y * 0.5f) * (delta.y * 0.5f) +
                                          (delta.z * 0.5f) * (delta.z * 0.5f) + 1.0f;
            bridge->field_ae4 = 0;
            f32 inverse_length = 1.0f / NuFsqrt(delta.x * delta.x + delta.z * delta.z);
            NUVEC side;
            side.x = -delta.z * inverse_length;
            side.y = 0.0f;
            side.z = delta.x * inverse_length;
            bridge->rotation_y = rotation;
            for (i32 i = 0; i < count; ++i) {
                if (NuSpecialExistsFn(&bridge->special)) {
                    bridge->platforms[i] =
                        NewPlatInst(&bridge->platform_matrices[i], NuSpecialGetInstanceix(&bridge->special));
                } else {
                    bridge->platforms[i] = -1;
                }
                NuMtxSetIdentity(&bridge->platform_matrices[i]);
                NuMtxPreRotateY(&bridge->platform_matrices[i], bridge->rotation_y);
                bridge->platform_matrices[i].m30 = delta.x * i / (count - 1) + start->x;
                bridge->platform_matrices[i].m31 = delta.y * i / (count - 1) + start->y;
                bridge->platform_matrices[i].m32 = delta.z * i / (count - 1) + start->z;
                bridge->edge_positions[i][0].x = delta.x * i / (count - 1) + start->x - side.x * width * 0.5f;
                bridge->edge_positions[i][0].y = delta.y * i / (count - 1) + start->y;
                bridge->edge_positions[i][0].z = delta.z * i / (count - 1) + start->z - side.z * width * 0.5f;
                bridge->edge_positions[i][1].x = delta.x * i / (count - 1) + start->x + side.x * width * 0.5f;
                bridge->edge_positions[i][1].y = delta.y * i / (count - 1) + start->y;
                bridge->edge_positions[i][1].z = delta.z * i / (count - 1) + start->z + side.z * width * 0.5f;
                bridge->edge_motion[i][0].x = 0.0f;
                bridge->edge_motion[i][0].y = 0.0f;
                bridge->edge_motion[i][0].z = 0.0f;
                bridge->edge_motion[i][1].x = 0.0f;
                bridge->edge_motion[i][1].y = 0.0f;
                bridge->edge_motion[i][1].z = 0.0f;
                bridge->field_aec = 0;
            }
        }
        return index;
    }
    void NuBridgeInit(void) {
        i32 index;
        i32 i;
        NUBRIDGE *bridge = Bridges;
        void *terrain = TerrainGetCur();
        for (index = 0; index < 8; ++index, ++bridge) {
            if (bridge->active) {
                TerrainSetCur(bridge->terrain);
                for (i = 0; i < bridge->platform_count; ++i) {
                    if (NuSpecialExistsFn(&bridge->special))
                        DeletePlatinst(bridge->platforms[i]);
                }
                bridge->active = 0;
            }
        }
        TerrainSetCur(terrain);
    }
    void NuBridgeRemove(i32 index) {
        if (index >= 0 && index < 8) {
            void *terrain = TerrainGetCur();
            NUBRIDGE *bridge = &Bridges[index];
            if (bridge->active) {
                TerrainSetCur(bridge->terrain);
                for (i32 i = 0; i < bridge->platform_count; ++i) {
                    if (NuSpecialExistsFn(&bridge->special))
                        DeletePlatinst(bridge->platforms[i]);
                }
                bridge->active = 0;
            }
            TerrainSetCur(terrain);
        }
    }
}

i32 NuBridgeAlloc(void) {
    i32 index;
    NUBRIDGE *bridge = Bridges;
    for (index = 0; index < 8; ++index, ++bridge) {
        if (!bridge->active)
            return index;
    }
    return -1;
}
