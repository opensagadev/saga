#include "decomp.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include <string.h>

struct NuFadeObjGType {
    i16 active;
    i16 flags;
    NUGSCN *scene;
    NUDISPLAYSPECIAL *special;
    void *terrain;
    i16 instance_index;
    u8 unknown_12[2];
    NUMTX *matrices;
    i16 *platforms;
    NUVEC centre;
    u8 drawn;
    u8 in_range;
    i16 count;
    f32 radius_squared, near_distance, far_distance, cull_distance_squared, fade_range;
};
DECOMP_ASSERT(sizeof(NuFadeObjGType) == 0x40, "fade group size");
static i32 maxwindmats;
static i32 maxgroups;
extern NUVEC ShadNorm;
extern "C" f32 NewShadow(NUVEC *, f32, f32, i32);

NUVEC NuFadeObjGetAngleTerrainValues(NUVEC *position) {
    NUVEC values;
    values.y = NewShadow(position, 0.0f, 5.0f, 0);
    values.x = NuAtan2D(ShadNorm.z, ShadNorm.y);
    values.z = -NuAtan2D(ShadNorm.x, NuFsqrt(ShadNorm.y * ShadNorm.y + ShadNorm.z * ShadNorm.z));
    return values;
}
static void (*NuFadeObjGlobalSetLightsFn)(void);
void NuFadeObjAngleTerrain(NUMTX *matrix) {
    NUVEC position;
    NUVEC angles;
    matrix->m31 = NewShadow(reinterpret_cast<NUVEC *>(&matrix->m30), 0.0f, 5.0f, 0);
    angles.x = NuAtan2D(ShadNorm.z, ShadNorm.y);
    angles.z = -NuAtan2D(ShadNorm.x, NuFsqrt(ShadNorm.y * ShadNorm.y + ShadNorm.z * ShadNorm.z));
    position.x = matrix->m30;
    position.y = matrix->m31;
    position.z = matrix->m32;
    matrix->m30 = matrix->m31 = matrix->m32 = 0.0f;
    NuMtxRotateZ(matrix, angles.z);
    NuMtxRotateX(matrix, angles.x);
    matrix->m30 = position.x;
    matrix->m31 = position.y;
    matrix->m32 = position.z;
}
static void (*NuFadeObjSetLightsFn)(void);
NuFadeObjGType *NuFadeObjAllocateGrp(void);
void NuFadeObjFreeGrp(NuFadeObjGType *);
i16 *NuFadeObjAllocData(i32);

extern "C" {
    NuFadeObjGType *NuFadeObjGroup;
    NuFadeObjGType *NuFadeObjCurGrp;
    void NuFadeObjSet_SetLightsFn(void (*global_lights)(void), void (*object_lights)(void)) {
        NuFadeObjGlobalSetLightsFn = global_lights;
        NuFadeObjSetLightsFn = object_lights;
    }
    void NuFadeObjDraw(void) {
        i32 i, g;
        NUMTX *matrix;
        NuFadeObjGType *group;
        f32 near_squared, far_squared, distance_squared;
        nuhspecial_s special;
        special.special = NULL;
        group = NuFadeObjGroup;
        if (NuFadeObjGlobalSetLightsFn)
            NuFadeObjGlobalSetLightsFn();
        for (g = 0; g < maxgroups; ++g, ++group) {
            if (group->active && group->in_range) {
                NuFadeObjCurGrp = group;
                group->drawn = 0;
                matrix = group->matrices;
                near_squared = group->near_distance * group->near_distance;
                far_squared = group->far_distance * group->far_distance;
                for (i = 0; i < group->count; ++i, ++matrix) {
                    distance_squared = (matrix->m30 - global_camera.mtx.m30) * (matrix->m30 - global_camera.mtx.m30) +
                                       (matrix->m31 - global_camera.mtx.m31) * (matrix->m31 - global_camera.mtx.m31) +
                                       (matrix->m32 - global_camera.mtx.m32) * (matrix->m32 - global_camera.mtx.m32);
                    if (distance_squared < far_squared) {
                        if (distance_squared < near_squared)
                            matrix->m23 = 0.0f;
                        else
                            matrix->m23 = 1.0e-11f;
                        special.scene = group->scene;
                        special.display_special = group->special;
                        if (NuSpecialDrawAt(&special, matrix))
                            group->drawn = 1;
                    }
                }
            }
        }
    }
    i16 *NuFadeData;
    NUMTX *NuFadeObjMtxs;
    i32 NuFadeObjDir, NuFadeObjDir2, NuFadeObjWave;
    i32 NuFadeObjMtxIndex, NuFadeObjDataIndex;
    void *TerrainGetCur(void);
    void TerrainSetCur(void *);
    i32 DeletePlatinst(i32);
    i32 NewPlatInst(NUMTX *, i32);
    void PlatInstRotate(i32, i32);
    i32 stopfadeup;
    void NuFadeObjUpdateArray(NUVEC **positions) {
        i32 i, g, p, nearby;
        NUMTX *matrix;
        i16 *platform;
        NuFadeObjGType *group = NuFadeObjGroup;
        void *terrain;
        NuFadeObjWave += 501;
        NuFadeObjDir += 133;
        NuFadeObjDir2 += 377;
        if (stopfadeup)
            return;
        terrain = TerrainGetCur();
        for (g = 0; g < maxgroups; ++g, ++group) {
            if (group->active) {
                if ((group->centre.x - global_camera.mtx.m30) * (group->centre.x - global_camera.mtx.m30) +
                        (group->centre.y - global_camera.mtx.m31) * (group->centre.y - global_camera.mtx.m31) +
                        (group->centre.z - global_camera.mtx.m32) * (group->centre.z - global_camera.mtx.m32) <
                    group->cull_distance_squared) {
                    group->in_range = 1;
                    if (group->flags) {
                        TerrainSetCur(group->terrain);
                        matrix = group->matrices;
                        platform = group->platforms;
                        for (i = 0; i < group->count; ++i, ++matrix, ++platform) {
                            nearby = 0;
                            for (p = 0; p < 8; ++p) {
                                if (positions[p]) {
                                    if ((matrix->m30 - positions[p]->x) * (matrix->m30 - positions[p]->x) +
                                            (matrix->m31 - positions[p]->y) * (matrix->m31 - positions[p]->y) +
                                            (matrix->m32 - positions[p]->z) * (matrix->m32 - positions[p]->z) <
                                        4.0f) {
                                        nearby = 1;
                                        break;
                                    }
                                }
                            }
                            if (nearby) {
                                if (*platform == -1) {
                                    *platform = NewPlatInst(matrix, group->instance_index);
                                    PlatInstRotate(*platform, 1);
                                }
                            } else if (*platform != -1) {
                                DeletePlatinst(*platform);
                                *platform = -1;
                            }
                        }
                    }
                } else {
                    group->in_range = 0;
                }
            }
        }
        TerrainSetCur(terrain);
    }
    void NuFadeObjUpdateArray(NUVEC **positions);

    void NuFadeObjUpdate(NUVEC *position) {
        i32 i;
        NUVEC *positions[8];
        for (i = 1; i < 8; ++i)
            positions[i] = NULL;
        positions[0] = position;
        NuFadeObjUpdateArray(positions);
    }

    NuFadeObjGType *NuFadeObjCreateMtx(nuhspecial_s *source, NUMTX *matrices, i16 count, f32 near_distance,
                                       f32 far_distance, i32 flags) {
        NUMTX *matrix;
        i16 *platform;
        i32 i;
        f32 min_x, max_x, min_y, max_y, min_z, max_z;
        NuFadeObjGType *group;
        if (!matrices)
            return NULL;
        matrix = matrices;
        group = NuFadeObjAllocateGrp();
        if (group) {
            platform = NuFadeObjAllocData(count);
            if (platform) {
                group->matrices = matrices;
                group->platforms = platform;
                group->scene = source->scene;
                group->special = source->display_special;
                group->instance_index = source->display_special->instance_ix;
                group->terrain = TerrainGetCur();
                group->count = count;
                group->flags = flags;
                group->near_distance = near_distance;
                group->far_distance = far_distance;
                group->cull_distance_squared = far_distance * far_distance;
                group->fade_range = group->far_distance - group->near_distance;
                min_x = min_y = min_z = 10000000.0f;
                max_x = max_y = max_z = -10000000.0f;
                for (i = 0; i < count; ++i, ++matrix, ++platform) {
                    *platform = -1;
                    if (matrix->m30 < min_x)
                        min_x = matrix->m30;
                    if (matrix->m31 < min_y)
                        min_y = matrix->m31;
                    if (matrix->m32 < min_z)
                        min_z = matrix->m32;
                    if (matrix->m30 > max_x)
                        max_x = matrix->m30;
                    if (matrix->m31 > max_y && matrix->m31 < 2000000.0f)
                        max_y = matrix->m31;
                    if (matrix->m32 > max_z)
                        max_z = matrix->m32;
                }
                group->centre.x = (max_x + min_x) * 0.5f;
                group->centre.y = (max_y + min_y) * 0.5f;
                group->centre.z = (max_z + min_z) * 0.5f;
                group->radius_squared = ((max_x - min_x) * 0.5f) * ((max_x - min_x) * 0.5f) +
                                        ((max_y - min_y) * 0.5f) * ((max_y - min_y) * 0.5f) +
                                        ((max_z - min_z) * 0.5f) * ((max_z - min_z) * 0.5f) + 1.0f;
                group->cull_distance_squared = group->cull_distance_squared + group->radius_squared;
                return group;
            }
            NuFadeObjFreeGrp(group);
            return NULL;
        }
        return NULL;
    }

    void NuFadeObjInit(void) {
        i16 *platform;
        i32 group, i;
        void *terrain;
        NuFadeObjDir = 0;
        NuFadeObjDir2 = 0;
        NuFadeObjWave = 0;
        NuFadeObjMtxIndex = 0;
        NuFadeObjDataIndex = 0;
        terrain = TerrainGetCur();
        for (group = 0; group < maxgroups; ++group) {
            if (NuFadeObjGroup[group].active) {
                TerrainSetCur(NuFadeObjGroup[group].terrain);
                platform = NuFadeObjGroup[group].platforms;
                for (i = 0; i < NuFadeObjGroup[group].count; ++i, ++platform) {
                    if (*platform != -1)
                        DeletePlatinst(*platform);
                }
                NuFadeObjGroup[group].active = 0;
            }
        }
        TerrainSetCur(terrain);
    }

    void NuFadeObjSetup(VARIPTR *buffer, VARIPTR, i32 matrix_count, i32 group_count) {
        maxwindmats = matrix_count;
        maxgroups = group_count;
        buffer->addr = (buffer->addr + 15) & ~static_cast<usize>(15);
        NuFadeObjGroup = static_cast<NuFadeObjGType *>(buffer->void_ptr);
        buffer->addr += maxgroups * sizeof(NuFadeObjGType);
        memset(NuFadeObjGroup, 0, maxgroups * sizeof(NuFadeObjGType));
        buffer->addr = (buffer->addr + 15) & ~static_cast<usize>(15);
        NuFadeData = static_cast<i16 *>(buffer->void_ptr);
        buffer->addr += maxwindmats * sizeof(i16);
        memset(NuFadeData, 0, maxwindmats * sizeof(i16));
        buffer->addr = (buffer->addr + 15) & ~static_cast<usize>(15);
        NuFadeObjMtxs = static_cast<NUMTX *>(buffer->void_ptr);
        buffer->addr += maxwindmats * sizeof(NUMTX);
        memset(NuFadeObjMtxs, 0, maxwindmats * sizeof(NUMTX));
    }
}

NuFadeObjGType *NuFadeObjAllocateGrp(void) {
    for (i32 i = 0; i < maxgroups; ++i) {
        if (!NuFadeObjGroup[i].active) {
            NuFadeObjGroup[i].active = 1;
            return &NuFadeObjGroup[i];
        }
    }
    return NULL;
}

void NuFadeObjFreeGrp(NuFadeObjGType *group) {
    group->active = 0;
}

i16 *NuFadeObjAllocData(i32 count) {
    if (NuFadeObjDataIndex + count < maxwindmats) {
        NuFadeObjDataIndex += count;
        return &NuFadeData[NuFadeObjDataIndex - count];
    }
    return NULL;
}

NUMTX *NuFadeObjAllocMtxs(i32 count) {
    if (NuFadeObjMtxIndex + count < maxwindmats) {
        NuFadeObjMtxIndex += count;
        return &NuFadeObjMtxs[NuFadeObjMtxIndex - count];
    }
    return NULL;
}

void NuFadeObjFreeMtxs(NUMTX *matrices, i32 count) {
    if (matrices)
        NuFadeObjMtxIndex -= count;
}
