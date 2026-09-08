#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numath/nufloat.h"

#include <stddef.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern TERRSET *CurTerr;
extern TerrainQuery_s *TerI;
extern i16 NuTerrPlatsOff;
extern TERRAIN_SHAPE *ScaleTerrain;
extern "C" void *NuScratchAlloc32(i32);
extern "C" void NuScratchRelease();
NUVEC TerCrossProduct(NUVEC *, NUVEC *);

void TerrainSkinAllocate(terrsitu_s *terrain_group);

namespace {

    // Target NewScanRot 0x378fb0 uses 0.1f on both horizontal axes when
    // selecting the terrain shapes that can contain the cast point.
    const f32 SHADOW_SCAN_HALF_EXTENT = 0.1f;

    struct ShadowScanWriter {
        u8 *group_header;
        TERRAIN_SHAPE **cursor;
        u8 *limit;
        i32 shape_count;
    };

    static bool ShadowBoundsOverlap(f32 min_x, f32 min_z, f32 max_x, f32 max_z, const NUVEC &minimum,
                                    const NUVEC &maximum) {
        return max_x >= minimum.x && maximum.x >= min_x && max_z >= minimum.z && maximum.z > min_z;
    }

    static void ShadowFinishGroup(ShadowScanWriter *writer, i32 group_index) {
        if (writer->shape_count == 0) {
            return;
        }

        i16 *header = reinterpret_cast<i16 *>(writer->group_header);
        header[0] = static_cast<i16>(writer->shape_count);
        header[1] = static_cast<i16>(group_index);
        writer->group_header = reinterpret_cast<u8 *>(writer->cursor);
        writer->cursor = reinterpret_cast<TERRAIN_SHAPE **>(writer->group_header + sizeof(TERRAIN_SHAPE *));
        writer->shape_count = 0;
    }

    static void ShadowScanGroup(i32 group_index, f32 world_min_x, f32 world_min_z, f32 world_max_x, f32 world_max_z,
                                i32 terrain_mask, bool bounds_are_local, ShadowScanWriter *writer) {
        TERRAIN_GROUP &group = CurTerr->groups[group_index];
        if (group.chunk_type == -1) {
            return;
        }

        f32 local_min_x = world_min_x - group.origin.x;
        f32 local_max_x = world_max_x - group.origin.x;
        f32 local_min_z = world_min_z - group.origin.z;
        f32 local_max_z = world_max_z - group.origin.z;
        if (bounds_are_local) {
            if (!ShadowBoundsOverlap(local_min_x, local_min_z, local_max_x, local_max_z, group.bounds_min,
                                     group.bounds_max)) {
                return;
            }
        } else if (!ShadowBoundsOverlap(world_min_x, world_min_z, world_max_x, world_max_z, group.bounds_min,
                                        group.bounds_max)) {
            return;
        }

        if (group.scene_index < 0) {
            TerrainSkinAllocate(reinterpret_cast<terrsitu_s *>(&group));
        }

        TERRAIN_SHAPE_BATCH *batch = static_cast<TERRAIN_SHAPE_BATCH *>(group.data);
        while (batch->marker >= 0) {
            TERRAIN_SHAPE *shapes = reinterpret_cast<TERRAIN_SHAPE *>(batch + 1);
            if (local_max_x >= batch->min_x && batch->max_x > local_min_x && local_max_z >= batch->min_z &&
                batch->max_z > local_min_z) {
                for (i32 shape_index = 0; shape_index < batch->shape_count; ++shape_index) {
                    TERRAIN_SHAPE *shape = &shapes[shape_index];
                    if (local_max_x < shape->min_x || shape->max_x <= local_min_x || local_max_z < shape->min_z ||
                        shape->max_z <= local_min_z) {
                        continue;
                    }
                    if (shape->material[1] != 0 && (shape->material[1] & terrain_mask) == 0) {
                        continue;
                    }
                    if (reinterpret_cast<u8 *>(writer->cursor + 1) > writer->limit) {
                        continue;
                    }
                    *writer->cursor++ = shape;
                    ++writer->shape_count;
                }
            }
            batch = reinterpret_cast<TERRAIN_SHAPE_BATCH *>(shapes + batch->shape_count);
        }
        ShadowFinishGroup(writer, group_index);
    }

} // namespace

void NewScanRot(nuvec_s *position, i32 terrain_mask) {
    void *scratch = NuScratchAlloc32(0xd0);
    i32 cache_index = 0;
    bool cache_hit = false;
    for (i32 i = 0; i < 16; ++i) {
        TERRAIN_INDEX_LEVEL &cache = CurTerr->index_levels[i];
        if (cache.cache_age > 0) {
            f32 dx = (position->x + 1.0f) - cache.center_x;
            f32 dz = (position->z + 1.0f) - cache.center_z;
            if (dx > 0.0f && dx < 2.0f && dz > 0.0f && dz < 2.0f) {
                cache_index = i;
                cache_hit = true;
                break;
            }
        }
        if (cache.cache_age < CurTerr->index_levels[cache_index].cache_age)
            cache_index = i;
    }
    TERRAIN_INDEX_LEVEL &cache = CurTerr->index_levels[cache_index];
    bool fill_cache = !cache_hit && cache.cache_age <= 1;
    ShadowScanWriter writer;
    writer.group_header = fill_cache ? cache.scan_list : TerI->scan_list_storage;
    writer.cursor = reinterpret_cast<TERRAIN_SHAPE **>(writer.group_header + sizeof(TERRAIN_SHAPE *));
    // Target stops at TerI + 0x93c, leaving the final header slot available.
    writer.limit = writer.group_header + 0x7f4;
    writer.shape_count = 0;

    const f32 extent = fill_cache ? 1.0f : SHADOW_SCAN_HALF_EXTENT;
    f32 min_x = position->x - extent;
    f32 max_x = position->x + extent;
    f32 min_z = position->z - extent;
    f32 max_z = position->z + extent;

    if (!cache_hit) {
        for (i32 cell_index = 0; cell_index < CurTerr->used_cell_count; ++cell_index) {
            const TERRAIN_CELL &cell = CurTerr->cells[cell_index];
            if (max_x < cell.min_x || cell.max_x < min_x || max_z < cell.min_z || cell.max_z < min_z) {
                continue;
            }
            const i16 *group_indices = CurTerr->group_indices + cell.first_group;
            for (i32 cell_group = 0; cell_group < static_cast<i16>(cell.group_count); ++cell_group) {
                const i32 group_index = group_indices[cell_group];
                ShadowScanGroup(group_index, min_x, min_z, max_x, max_z, terrain_mask, false, &writer);
            }
        }
        // Skin allocation can invalidate the cache while scanning groups.
        fill_cache = cache.cache_age <= 1;
    }
    if (fill_cache) {
        reinterpret_cast<i16 *>(writer.group_header)[0] = 0;
        reinterpret_cast<i16 *>(writer.group_header)[1] = 0;
        cache.center_x = position->x;
        cache.center_z = position->z;
    }
    if (fill_cache || cache_hit) {
        cache.cache_age = 8;
        writer.group_header = TerI->scan_list_storage;
        writer.cursor = reinterpret_cast<TERRAIN_SHAPE **>(writer.group_header + 4);
        writer.limit = reinterpret_cast<u8 *>(TerI) + 0x93c;
        min_x = position->x - SHADOW_SCAN_HALF_EXTENT;
        max_x = position->x + SHADOW_SCAN_HALF_EXTENT;
        min_z = position->z - SHADOW_SCAN_HALF_EXTENT;
        max_z = position->z + SHADOW_SCAN_HALF_EXTENT;
        u8 *entry = cache.scan_list;
        for (;;) {
            i16 count = reinterpret_cast<i16 *>(entry)[0];
            if (count <= 0)
                break;
            i16 group_index = reinterpret_cast<i16 *>(entry)[1];
            TERRAIN_SHAPE **shapes = reinterpret_cast<TERRAIN_SHAPE **>(entry + 4);
            entry = reinterpret_cast<u8 *>(shapes + count);
            TERRAIN_GROUP &group = CurTerr->groups[group_index];
            if (!ShadowBoundsOverlap(min_x, min_z, max_x, max_z, group.bounds_min, group.bounds_max) ||
                group.chunk_type == -1)
                continue;
            f32 local_min_x = min_x - group.origin.x, local_max_x = max_x - group.origin.x;
            f32 local_min_z = min_z - group.origin.z, local_max_z = max_z - group.origin.z;
            for (i32 i = 0; i < count; ++i) {
                TERRAIN_SHAPE *shape = shapes[i];
                if (local_max_x < shape->min_x || shape->max_x <= local_min_x || local_max_z < shape->min_z ||
                    shape->max_z <= local_min_z)
                    continue;
                if (shape->material[1] != 0 && (shape->material[1] & terrain_mask) == 0)
                    continue;
                if (reinterpret_cast<u8 *>(writer.cursor) >= writer.limit)
                    continue;
                *writer.cursor++ = shape;
                ++writer.shape_count;
            }
            ShadowFinishGroup(&writer, group_index);
        }
    }

    if (NuTerrPlatsOff == 0) {
        min_x -= 0.05f;
        min_z -= 0.05f;
        max_x += 0.05f;
        max_z += 0.05f;
        i16 *platform_groups = CurTerr->active_platform_groups;
        i32 platform_count = CurTerr->active_platform_count;
        if (!(min_x > CurTerr->platform_scan_min.x && max_x < CurTerr->platform_scan_max.x &&
              min_z > CurTerr->platform_scan_min.z && max_z < CurTerr->platform_scan_max.z)) {
            TERRAIN_CELL &cell = CurTerr->cells[TERRAIN_PLATFORM_CELL];
            platform_groups = CurTerr->group_indices + cell.first_group;
            platform_count = static_cast<i16>(cell.group_count);
        }
        NUVEC4 *vertices = reinterpret_cast<NUVEC4 *>((reinterpret_cast<uintptr_t>(scratch) + 31) & ~uintptr_t(15));
        i32 transformed_count = 0;
        for (i32 p = 0; p < platform_count; ++p) {
            i32 group_index = platform_groups[p];
            TERRAIN_GROUP &group = CurTerr->groups[group_index];
            if (group.origin.x - group.radius > max_x || min_x > group.origin.x + group.radius ||
                group.origin.z - group.radius > max_z || min_z > group.origin.z + group.radius)
                continue;
            TERRAIN_PLATFORM &platform = CurTerr->platforms[group.scene_index];
            f32 local_min_x = min_x - group.origin.x, local_max_x = max_x - group.origin.x;
            f32 local_min_z = min_z - group.origin.z, local_max_z = max_z - group.origin.z;
            NUMTX *matrix = static_cast<NUMTX *>(platform.scene_object);
            if (matrix != NULL) {
                if (matrix->m30 > platform.previous_matrix.m30)
                    local_max_x = (matrix->m30 - platform.previous_matrix.m30) * 1.5f + max_x - group.origin.x;
                else
                    local_min_x = (matrix->m30 - platform.previous_matrix.m30) * 1.5f + min_x - group.origin.x;
                if (matrix->m32 > platform.previous_matrix.m32)
                    local_max_z = (matrix->m32 - platform.previous_matrix.m32) * 1.5f + max_z - group.origin.z;
                else
                    local_min_z = (matrix->m32 - platform.previous_matrix.m32) * 1.5f + min_z - group.origin.z;
            }
            bool rotating = (platform.flags & 1) != 0;
            if (rotating) {
                if (group.chunk_type == -1)
                    continue;
                f32 dx = (max_x + min_x) * 0.5f - group.origin.x;
                f32 dz = (min_z + max_z) * 0.5f - group.origin.z;
                f32 radius = group.radius + 0.00001f;
                if (!(radius * radius > dx * dx + dz * dz))
                    continue;
            } else if (!ShadowBoundsOverlap(local_min_x, local_min_z, local_max_x, local_max_z, group.bounds_min,
                                            group.bounds_max) ||
                       group.chunk_type == -1) {
                continue;
            }
            if (platform.scene_transform != NULL &&
                (*static_cast<u8 *>(platform.scene_transform) & ((platform.flags & 4) != 0 ? 2 : 1)) == 0)
                continue;
            TERRAIN_SHAPE_BATCH *batch = static_cast<TERRAIN_SHAPE_BATCH *>(group.data);
            while (batch->marker >= 0) {
                TERRAIN_SHAPE *shapes = reinterpret_cast<TERRAIN_SHAPE *>(batch + 1);
                bool batch_overlaps = rotating || (local_max_x >= batch->min_x && batch->max_x > local_min_x &&
                                                   local_max_z >= batch->min_z && batch->max_z > local_min_z);
                for (i32 i = 0; batch_overlaps && i < batch->shape_count; ++i) {
                    TERRAIN_SHAPE *shape = &shapes[i];
                    if (shape->material[1] != 0 && (shape->material[1] & terrain_mask) == 0)
                        continue;
                    if (reinterpret_cast<u8 *>(writer.cursor) >= writer.limit)
                        continue;
                    if (!rotating) {
                        if (local_max_x < shape->min_x || shape->max_x <= local_min_x || local_max_z < shape->min_z ||
                            shape->max_z <= local_min_z)
                            continue;
                    } else {
                        for (i32 v = 0; v < 3; ++v) {
                            vertices[v] = {shape->vectors[v].x, shape->vectors[v].y, shape->vectors[v].z, 0.0f};
                        }
                        NuVec4MtxTransformVU0x3(vertices, vertices, matrix);
                        bool quad = shape->normals[1].y < 65535.0f;
                        if (quad) {
                            vertices[3] = {shape->vectors[3].x, shape->vectors[3].y, shape->vectors[3].z, 0.0f};
                            NuVec4MtxTransformVU0(&vertices[3], &vertices[3], matrix);
                        } else
                            vertices[3] = vertices[2];
                        bool above_x = false, below_x = false, above_z = false, below_z = false;
                        for (i32 v = 0; v < 4; ++v) {
                            above_x |= vertices[v].x > local_min_x;
                            below_x |= local_max_x > vertices[v].x;
                            above_z |= vertices[v].z > local_min_z;
                            below_z |= local_max_z > vertices[v].z;
                        }
                        if (!above_x || !below_x || !above_z || !below_z)
                            continue;
                        TERRAIN_SHAPE *transformed = &ScaleTerrain[transformed_count];
                        *reinterpret_cast<u32 *>(transformed->material) = *reinterpret_cast<u32 *>(shape->material);
                        for (i32 v = 0; v < (quad ? 4 : 3); ++v)
                            transformed->vectors[v] = {vertices[v].x, vertices[v].y, vertices[v].z};
                        if (!quad)
                            transformed->normals[1].y = 65536.0f;
                        for (i32 n = quad ? 1 : 0; n >= 0; --n) {
                            i32 origin = n ? 3 : 0, first = n ? 1 : 2, second = n ? 2 : 1;
                            NUVEC a = {vertices[first].x - vertices[origin].x, vertices[first].y - vertices[origin].y,
                                       vertices[first].z - vertices[origin].z};
                            NUVEC b = {vertices[second].x - vertices[origin].x, vertices[second].y - vertices[origin].y,
                                       vertices[second].z - vertices[origin].z};
                            NUVEC &normal = transformed->normals[n];
                            normal = TerCrossProduct(&a, &b);
                            f32 length = NuFsqrt((normal.x * normal.x + normal.y * normal.y) + normal.z * normal.z);
                            f32 inverse = length == 0.0f ? 0.0f : 1.0f / length;
                            normal.x *= inverse;
                            normal.y *= inverse;
                            normal.z *= inverse;
                        }
                        shape = transformed;
                        ++transformed_count;
                    }
                    *writer.cursor++ = shape;
                    ++writer.shape_count;
                }
                batch = reinterpret_cast<TERRAIN_SHAPE_BATCH *>(shapes + batch->shape_count);
            }
            ShadowFinishGroup(&writer, group_index);
        }
    }
    NuScratchRelease();
    i16 *terminator = reinterpret_cast<i16 *>(writer.group_header);
    terminator[0] = 0;
    terminator[1] = 0;
    TerI->scan_list = TerI->scan_list_storage;
}

void ObjZappedBlue(GameObject_s *) {
}

void PeriscodeCode(GameObject_s *) {
}

void NewScanHandelFull(nuvec_s *, nuvec_s *, float, i32, i32) {
}

void NewScanHandelSubset(i16 *, nuvec_s *, nuvec_s *, float, i32) {
}

void RegisterGizmoTypes_Batman(variptr_u *, variptr_u *) {
}

void NewScan(nuvec_s *, i32, i32) {
}
