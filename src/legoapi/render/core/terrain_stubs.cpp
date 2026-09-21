#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/terrain_internal.h"
#include "legoapi/render/core/gameliball.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "gameapi/edtools/edpp_internal.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/android/nurain_android.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "gameapi/edtools/edstubs.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nustring.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stddef.h>

i16 TerrImpact;
i32 terrhitflags;
i32 TERRAINMASK_NONWEAPON = 0x20;
i32 TERRAINMASK_NONDROID;
i32 geom;
NUVEC TerrImpactPos;
NUVEC TerrImpactNormal;
NUVEC ShadNorm;
NUVEC ShadRoofNorm;
NUVEC EShadNorm;
NUVEC EShadRoofNorm;
f32 ShadRoofY;
f32 EShadRoofY;
TerrainLastImpact_s TerrLastImpact;
TERRSET *CurTerr;
static PLATSKININFO *PlatSkinInfo;
static PLATSKINMEMINFO *SkinMemInfo;
static i32 PlatSkinMaxStore;
static i32 TerrainUpadteCnt;
static u8 *PlatSkinMem;
static u8 *PlatSkinMemEnd;
static i32 PlatSkinMax;
static i32 PlatSkinMaxSize;
static i32 PlatSkinCnt;
static i32 PlatSkinResetTotal = -1;
static i32 debris_sort_delay = 6;
static i32 debris_update_delay = 3;
static i32 debris_sort_this_stack;
static i32 debris_update_this_stack;
// Runtime-selected groups appended after the fixed terrain allocation.
i32 WallSplinesOnly;
extern "C" {
    i32 IgnoreWallSplines;
}
f32 TerrPlatScanDist = 10.0f;
i32 cntrots;
struct TERRAIN_PLATFORM_CALLBACK {
    void (*function)(void *);
    void *argument;
};
static i32 PlatCodeCallback;
static TERRAIN_PLATFORM_CALLBACK PlatCallback[8];
void ScanTerrIDRemovePlat(i32 platform_index);

u8 TerrainHitInfo[4];
TERRAIN_SPHERE SphereData[16];

void TerrainSkinAllocate(terrsitu_s *terrain_group) {
    TERRAIN_GROUP *group = reinterpret_cast<TERRAIN_GROUP *>(terrain_group);
    i32 skin_index = ~static_cast<i32>(group->scene_index);
    if (skin_index >= PlatSkinCnt)
        return;
    ++TerrainUpadteCnt;
    PLATSKININFO *info = &PlatSkinInfo[skin_index];
    if (group->data != NULL) {
        SkinMemInfo[info->cache_slot].last_used = TerrainUpadteCnt;
        return;
    }
    i32 slot = 0;
    if (SkinMemInfo[0].skin_index != -1) {
        i32 oldest = SkinMemInfo[0].last_used;
        for (i32 i = 1; i < PlatSkinMaxStore; ++i) {
            if (SkinMemInfo[i].skin_index == -1) {
                slot = i;
                break;
            }
            if (SkinMemInfo[i].last_used < oldest) {
                oldest = SkinMemInfo[i].last_used;
                slot = i;
            }
        }
    }
    PLATSKINMEMINFO *cache = &SkinMemInfo[slot];
    if (cache->skin_index >= 0) {
        CurTerr->groups[PlatSkinInfo[cache->skin_index].terrain_group].data = NULL;
        for (i32 i = 0; i < 16; ++i)
            CurTerr->index_levels[i].entry_count = 0;
    }
    group->data = info->terrain_data;
    info->cache_slot = slot;
    cache->skin_index = skin_index;
    SkinPlatform(terrain_group, PlatSkinMem + slot * PlatSkinMaxSize, info);
    SkinMemInfo[slot].last_used = TerrainUpadteCnt;
}

void SkinPlatformSize(i32 group_index, unsigned char *buffer, PLATSKININFO *info) {
    if (CurTerr == NULL || group_index < 0)
        return;
    TERRAIN_GROUP *group = &CurTerr->groups[group_index];
    NUVEC minimum = {123456792.0f, 123456792.0f, 123456792.0f};
    NUVEC maximum = {-123456792.0f, -123456792.0f, -123456792.0f};
    f32 radius_squared = 0.0f;
    i32 bytes = 0;
    if (static_cast<u32>(group->chunk_type) <= 1) {
        TERRAIN_SHAPE_BATCH *input = static_cast<TERRAIN_SHAPE_BATCH *>(group->data);
        TERRAIN_SHAPE_BATCH *output = reinterpret_cast<TERRAIN_SHAPE_BATCH *>(buffer);
        while (input->marker >= 0) {
            output->marker = input->marker;
            output->shape_count = input->shape_count;
            TERRAIN_SHAPE *source = reinterpret_cast<TERRAIN_SHAPE *>(input + 1);
            TERRAIN_SHAPE *destination = reinterpret_cast<TERRAIN_SHAPE *>(output + 1);
            for (i32 i = 0; i < input->shape_count; ++i, ++source, ++destination) {
                i32 last_vertex = source->normals[1].y > 65535.0f ? 2 : 3;
                memcpy(destination, source, sizeof(TERRAIN_SHAPE));
                for (i32 vertex = last_vertex; vertex >= 0; --vertex) {
                    NUVEC &v = destination->vectors[vertex];
                    v = TerrainSkin(info, &source->vectors[SkinFlipTab[vertex + info->mirrored * 4]], -1.0f,
                                    info->flags);
                    v.x -= info->matrix->m30;
                    v.y -= info->matrix->m31;
                    v.z -= info->matrix->m32;
                    minimum.x = MIN(v.x, minimum.x);
                    minimum.y = MIN(v.y, minimum.y);
                    minimum.z = MIN(v.z, minimum.z);
                    maximum.x = MAX(v.x, maximum.x);
                    maximum.y = MAX(v.y, maximum.y);
                    maximum.z = MAX(v.z, maximum.z);
                    radius_squared = MAX((v.x * v.x + v.y * v.y) + v.z * v.z, radius_squared);
                }
                for (i32 normal = source->normals[1].y < 65535.0f ? 1 : 0; normal >= 0; --normal) {
                    i32 origin = normal != 0 ? 3 : 0;
                    i32 first = normal != 0 ? 1 : 2;
                    i32 second = normal != 0 ? 2 : 1;
                    NUVEC a, b;
                    a.x = destination->vectors[first].x - destination->vectors[origin].x;
                    a.y = destination->vectors[first].y - destination->vectors[origin].y;
                    a.z = destination->vectors[first].z - destination->vectors[origin].z;
                    b.x = destination->vectors[second].x - destination->vectors[origin].x;
                    b.y = destination->vectors[second].y - destination->vectors[origin].y;
                    b.z = destination->vectors[second].z - destination->vectors[origin].z;
                    NUVEC &n = destination->normals[normal];
                    n = TerCrossProduct(&a, &b);
                    f32 squared = (n.x * n.x + n.y * n.y) + n.z * n.z;
                    f32 inverse = squared == 0.0f ? 0.0f : 1.0f / NuFsqrt(squared);
                    n.x *= inverse;
                    n.y *= inverse;
                    n.z *= inverse;
                }
            }
            input = reinterpret_cast<TERRAIN_SHAPE_BATCH *>(source);
            output = reinterpret_cast<TERRAIN_SHAPE_BATCH *>(destination);
        }
        output->marker = -1;
        output->shape_count = -1;
        bytes = reinterpret_cast<unsigned char *>(input) + 4 - static_cast<unsigned char *>(group->data);
    }
    group->origin.x = info->matrix->m30;
    group->origin.y = info->matrix->m31;
    group->origin.z = info->matrix->m32;
    group->bounds_min.x = (minimum.x - 0.1f) + info->matrix->m30;
    group->bounds_min.y = (minimum.y - 0.1f) + info->matrix->m31;
    group->bounds_min.z = (minimum.z - 0.1f) + info->matrix->m32;
    group->bounds_max.x = (maximum.x + 0.1f) + info->matrix->m30;
    group->bounds_max.y = (maximum.y + 0.1f) + info->matrix->m31;
    group->bounds_max.z = (maximum.z + 0.1f) + info->matrix->m32;
    group->radius = NuFsqrt(radius_squared);
    if (bytes > PlatSkinMaxSize)
        PlatSkinMaxSize = bytes;
}

extern "C" {
    void *NuScratchAlloc32(i32 size);
    void NuScratchRelease(void);
}

TERRAIN_TRACK_SLOT *ScanTerrId(void *hit_flags);
void ScanTerrain(i32 scan_type, i32 terrain_mask, i32 scan_flags);
i32 PlatformChecks(i32 count, NUVEC *movement);
i32 HitTerrain(void);
void TerrainImpactNorm(void);
void RayImpact(NUVEC *);
void StorePlatImpact(void);
i32 TerrainPlatformEmbedded(NUVEC *movement);
i32 TerrShapeSideStep(NUVEC *position, NUVEC *movement, u8 *hit_flags);
void TerrainImpact(NUVEC *position, NUVEC *movement, u8 *hit_flags);
void TerrFlush(void);
void NewScanRot(NUVEC *position, i32 terrain_mask);
f32 NewCast(NUVEC *position, f32 height_above, f32 height_below);
void RemoveChunkControlFromStack(debris_chunk_control_s *, debris_chunk_control_s **);

extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern i32 maxdebkeys;
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern i32 EDPP_SCALE_TYPES;
    extern debinftype *effecttypes;
    extern debscale_s *debscale;
    extern f32 panelglobaltime;
    extern f32 globaltime;
    extern f32 renderpanelglobaltime;
    extern f32 renderglobaltime;
    extern f32 timeincrement;
    extern i32 globalframes;
    extern i32 update_debris_enabled;
    extern i32 debris_setup_called;
    extern usize debris_trash_space;
    extern usize debris_trash_size;
    extern i32 debrischunks;
    extern i32 debrischunksglass;
    extern i32 freedebchkptr;
    extern i32 freedebchkptrg;
    extern debris_chunk_control_s *debris_chunk_controls;
    extern debris_chunk_control_s **freechunkcontrols;
    extern i32 freechunkcontrolsptr;
    extern dma_particle_chunk_s **freedebchunks;
    extern dma_particle_chunk_s **freedebchunksglass;
    extern particlechunkrendertype_s *ParticleChunkToRender;
    extern particlechunkrendertype_s *ParticleChunkRenderStack[5];
    extern debris_chunk_control_s *debris_chunk_control_stack[2];
    extern NUMTL *DebMat[10];
    extern i32 debris_suspended;
    void DebrisReScale(i32, f32);
    void DebReAlloc(debkeydatatype_s *, i32);
    void DebFreeInstantly(i32 *);
    extern "C++" void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
    extern "C++" {
        void AddChunkToRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);
        void DebrisCleanUpDmaDebTypeTables(void);
        void DebrisProcessAllocation(void);
        void DebrisProcessControlChunks(i32);
        void RemoveChunkFromRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);
    }
}

// InitGameDebris @0x3ca2d0 (game_deb.cpp).
// edppLoadPage @0x36c630 (edtoolsall_plain.cpp) — deferred parts-page loader.
void DebrisFreeOldestDmaDebTypeTable() {
    i32 oldest = 0;
    f32 oldest_age = 0.0f;
    for (i32 i = 1; i < EDPP_MAX_TYPES; ++i) {
        debinftype *effect = debtab[i];
        if (effect == NULL || effect->native_data == NULL)
            continue;
        f32 age = (effect->time_group == 4 ? panelglobaltime : globaltime) - effect->last_render_time;
        if (age > oldest_age) {
            oldest_age = age;
            oldest = i;
        }
    }
    if (oldest != 0) {
        DmaDebTypes[--freeDmaDebType] = debtab[oldest]->native_data;
        debtab[oldest]->native_data = NULL;
    }
}

extern "C" i32 edppLoadPage(char *path, i32 flag, usize scene);
// NuFileExists @nufile (nucore_plain.cpp).
extern "C" i32 NuFileExists(char *name);

extern "C" void TerrainSetCur(void *terrain) {
    CurTerr = static_cast<TERRSET *>(terrain);
}
extern "C" void TerrSetPlatScanDist(f32 dist) {
    TerrPlatScanDist = dist;
}

extern "C" void TerrainPlatformNewUpdate(void) {
    if (CurTerr != NULL) {
        for (i32 i = 0; i < 64; ++i) {
            TERRAIN_TRACK_SLOT &slot = CurTerr->track_slots[i];
            if (slot.id != NULL) {
                if (slot.platform_contact_state > 0)
                    --slot.platform_contact_state;
                if (slot.wall_contact_state > 0)
                    --slot.wall_contact_state;
            }
        }
        for (i32 i = 0; i < 16; ++i) {
            if (static_cast<i16>(CurTerr->index_levels[i].entry_count) > 0)
                --CurTerr->index_levels[i].entry_count;
        }
    }
    for (i32 i = 0; i < PlatCodeCallback; ++i)
        PlatCallback[i].function(PlatCallback[i].argument);
    PlatCodeCallback = 0;
    cntrots = 0;
    if (CurTerr == NULL)
        return;

    const NUMTX &camera = global_camera.mtx;
    const f32 negative_distance = -TerrPlatScanDist;
    const f32 back_x = negative_distance * camera.m20 * 0.25f;
    const f32 back_z = negative_distance * camera.m22 * 0.25f;
    f32 min_x = camera.m00 * negative_distance * 0.25f + back_x;
    f32 min_z = camera.m02 * negative_distance * 0.25f + back_z;
    f32 max_x = camera.m00 * negative_distance * 0.35f + camera.m20 * TerrPlatScanDist;
    f32 max_z = camera.m02 * negative_distance * 0.35f + camera.m22 * TerrPlatScanDist;
    f32 x = min_x;
    f32 z = min_z;
    min_x = MIN(min_x, max_x);
    min_z = MIN(min_z, max_z);
    max_x = MAX(x, max_x);
    max_z = MAX(z, max_z);
    x = camera.m00 * TerrPlatScanDist * 0.35f + camera.m20 * TerrPlatScanDist;
    z = TerrPlatScanDist * camera.m02 * 0.35f + camera.m22 * TerrPlatScanDist;
    min_x = MIN(min_x, x);
    max_x = MAX(max_x, x);
    min_z = MIN(min_z, z);
    max_z = MAX(max_z, z);
    x = camera.m00 * TerrPlatScanDist * 0.25f + back_x;
    z = TerrPlatScanDist * camera.m02 * 0.25f + back_z;
    min_x = (MIN(min_x, x)) + (camera.m30 - 1.0f);
    max_x = (MAX(max_x, x)) + (camera.m30 + 1.0f);
    min_z = (MIN(min_z, z)) + (camera.m32 - 1.0f);
    max_z = (MAX(max_z, z)) + (camera.m32 + 1.0f);
    CurTerr->platform_scan_min.x = min_x;
    CurTerr->platform_scan_min.z = min_z;
    CurTerr->platform_scan_max.x = max_x;
    CurTerr->platform_scan_max.z = max_z;
    CurTerr->active_platform_count = 0;
    TERRAIN_CELL &cell = CurTerr->cells[TERRAIN_PLATFORM_CELL];
    i16 *group_index = CurTerr->group_indices + cell.first_group;
    i16 *end = group_index + cell.group_count;
    for (; group_index != end; ++group_index) {
        TERRAIN_GROUP &group = CurTerr->groups[*group_index];
        TERRAIN_PLATFORM &platform = CurTerr->platforms[group.scene_index];
        if (platform.scene_transform != NULL) {
            const u8 visibility = *static_cast<u8 *>(platform.scene_transform);
            if ((platform.flags & TERRAIN_PLATFORM_FLAG_DISPLAY_LIST_BACKED) != 0) {
                if ((visibility & 2) == 0)
                    continue;
            } else if ((visibility & 1) == 0)
                continue;
        }
        if (group.chunk_type == 0)
            continue;
        NUMTX *matrix = static_cast<NUMTX *>(platform.scene_object);
        if (matrix != NULL) {
            if (platform.bounce_frames != 0) {
                f32 velocity = platform.bounce_velocity - platform.bounce_damping * platform.bounce_velocity;
                if (platform.bounce_frames >= 125)
                    velocity = (platform.bounce_impulse - platform.bounce_damping * platform.bounce_velocity) +
                               platform.bounce_velocity;
                const f32 offset = platform.bounce_offset;
                --platform.bounce_frames;
                velocity += -offset * fabsf(offset) * platform.bounce_spring * 0.5f - platform.bounce_spring * offset;
                platform.bounce_velocity = velocity;
                platform.bounce_offset = velocity + offset;
                matrix->m31 += platform.bounce_offset;
            }
            group.origin = *NUMTX_GET_ROW_VEC(matrix, 3);
        }
        const u8 flags = platform.flags;
        if ((flags & TERRAIN_PLATFORM_FLAG_COLLIDED) != 0)
            platform.bounce_frames = 128;
        platform.field_0x44 = 0;
        platform.flags &= ~TERRAIN_PLATFORM_FLAG_COLLIDED;
        if ((flags & TERRAIN_PLATFORM_FLAG_ROTATING) == 0) {
            if (group.bounds_min.x + group.origin.x > max_x || group.origin.x + group.bounds_max.x < min_x ||
                group.bounds_min.z + group.origin.z > max_z || group.origin.z + group.bounds_max.z < min_z)
                continue;
        } else {
            if (group.origin.x - group.radius > max_x || group.origin.x + group.radius < min_x ||
                group.origin.z - group.radius > max_z || group.origin.z + group.radius < min_z)
                continue;
        }
        if (CurTerr->active_platform_count < 96)
            CurTerr->active_platform_groups[CurTerr->active_platform_count++] = *group_index;
    }
}
extern "C" i32 PARTLookupTypePageOnly(char *, i32);
extern "C" part_type_s part_types[128];
void *InitPartDebris(VARIPTR *buf, VARIPTR *, i32 capacity, i32 named_count, char **names, i32 page) {
    PARTDEBSYS_s *system = static_cast<PARTDEBSYS_s *>(BUFFER_ALLOC(buf, sizeof(PARTDEBSYS_s), 16));
    if (system == NULL)
        return system;
    system->entries = NULL;
    system->capacity = capacity;
    system->named_count = named_count;
    system->entries = static_cast<PARTDEBENTRY_s *>(BUFFER_ALLOC(buf, capacity * sizeof(PARTDEBENTRY_s), 16));
    if (system->entries == NULL)
        return NULL;
    memset(system->entries, -1, capacity * sizeof(PARTDEBENTRY_s));
    i32 i = 0;
    if (names != NULL) {
        for (; i < system->named_count; ++i) {
            NuStrCpy(system->entries[i].name, names[i]);
            system->entries[i].type_id = -1;
            system->entries[i].type_id = PARTLookupTypePageOnly(system->entries[i].name, page);
        }
    }
    for (i32 type = 0; type < 128 && i < system->capacity; ++type) {
        system->entries[i].type_id = -1;
        if (part_types[type].variant_count > 0) {
            NuStrCpy(system->entries[i].name, part_types[type].name);
            system->entries[i].type_id = PARTLookupTypePageOnly(system->entries[i].name, page);
            ++i;
        }
    }
    for (; i < system->capacity; ++i)
        system->entries[i].type_id = -1;
    return system;
}

// Particles_Load @0x4a2a50.
void Particles_Load(WORLDINFO *world, char **debris_name, i32 count, i32 flags) {
    char path[0x100];

    world->page_pp = -1;
    sprintf(path, "%s.ptl", world->config_file);
    if (NuFileExists(path) != 0) {
        world->page_pp = edppLoadPage(path, 1, reinterpret_cast<usize>(world->current_gscn));
    }

    world->debris_sys = (APIDEBRISSYS_s *)InitGameDebris(&world->giz_buffer, world->unknown_0108, count, flags,
                                                         debris_name, (char)world->page_pp);
}

extern "C" {

    void AITerrInit(void) {
    }

    f32 AITerrShadow(NUVEC *position, f32 height_above, f32 height_below, i32 terrain_mask) {
        f32 height = 0.0f;
        if (geom == 0)
            height = NewShadow(position, height_above, height_below, terrain_mask);
        return height;
    }

    i32 NewShadowOnPlatform(void);

    i32 AITerrShadowOnPlatform(void) {
        i32 platform = -1;
        if (geom == 0)
            platform = NewShadowOnPlatform();
        return platform;
    }

    i32 CheckForPlatInst(i32 instance) {
        if (CurTerr->max_platforms <= 0) {
            return 0;
        }
        TERRAIN_PLATFORM *platform = CurTerr->platforms;
        for (i32 i = 0; i < CurTerr->max_platforms; ++i, ++platform) {
            if (platform->scene_object != NULL && static_cast<i16>(platform->scene_object_index) == instance) {
                return 1;
            }
        }
        return 0;
    }

    i32 CreateScaledEffect(i32 effect_index, f32 requested_scale) {
        if (effect_index < 1 || EDPP_MAX_TYPES < effect_index || debtab[effect_index] == NULL) {
            return -1;
        }

        debinftype *effect = debtab[effect_index];
        if (effect->scale != 1.0f) {
            effect_index = effect->unscaled_effect_index;
            effect = debtab[effect_index];
            if (effect == NULL) {
                return -1;
            }
        }
        if (requested_scale == 1.0f && effect_index != 0) {
            return effect_index;
        }

        if (requested_scale < 0.01f) {
            requested_scale = 0.01f;
        }

        for (i32 i = 0; i < EDPP_SCALE_TYPES; ++i) {
            debscale_s &entry = debscale[i];
            if (entry.unscaled_effect_index == effect_index && entry.scale == requested_scale &&
                entry.scaled_effect_index != 0) {
                debinftype *scaled = debtab[entry.scaled_effect_index];
                if (scaled != NULL && scaled->unscaled_effect_index == effect_index) {
                    return entry.scaled_effect_index;
                }
                entry.scale = 0.0f;
            }
        }

        i32 closest_index = effect_index;
        f32 closest_distance = fabsf(requested_scale / effect->scale - 1.0f);
        for (i32 i = 1; i < EDPP_MAX_TYPES; ++i) {
            debinftype *candidate = debtab[i];
            if (candidate != NULL && candidate->unscaled_effect_index == effect_index) {
                f32 distance = 1.0f;
                if (requested_scale != 0.0f && candidate->scale != 0.0f) {
                    distance = fabsf(requested_scale / candidate->scale - 1.0f);
                }
                if (distance < closest_distance) {
                    closest_distance = distance;
                    closest_index = i;
                }
            }
        }
        if (closest_index != 0 && closest_distance < 0.1f) {
            return closest_index;
        }

        i32 scaled_index = 1;
        while (scaled_index < EDPP_MAX_TYPES && debtab[scaled_index] != NULL) {
            ++scaled_index;
        }
        if (scaled_index == EDPP_MAX_TYPES) {
            return closest_index;
        }

        debinftype *scaled = &effecttypes[scaled_index];
        debtab[scaled_index] = scaled;
        *scaled = *effect;
        scaled->native_data = NULL;
        for (i32 i = 0; i < 8; ++i) {
            scaled->particle_keys[i] = -1;
        }
        DebrisReScale(scaled_index, requested_scale);
        scaled->scale = requested_scale;
        scaled->last_render_time = scaled->time_group == 4 ? panelglobaltime : globaltime;
        scaled->unscaled_effect_index = effect_index;

        if (strlen(effect->name) < 13) {
            sprintf(scaled->name, "%s%03d", effect->name, scaled_index);
        } else {
            char shortened[13];
            memcpy(shortened, effect->name, 12);
            shortened[12] = '\0';
            sprintf(scaled->name, "%s%03d", shortened, scaled_index);
        }
        ++edpp_types_used;

        for (i32 i = 0; i < EDPP_SCALE_TYPES; ++i) {
            if (debscale[i].scale == 0.0f) {
                debscale[i].unscaled_effect_index = effect_index;
                debscale[i].scaled_effect_index = scaled_index;
                debscale[i].scale = requested_scale;
                break;
            }
        }
        return scaled_index;
    }

    void CreateScaledPARTEffect(void) {
        STUBBED();
    }

    void CubeImpact(void) {
        STUBBED();
    }

    void DebFreeAllCreatedEffects(void) {
        for (i32 i = 0; i < maxdebkeys; ++i) {
            if (debkeydata[i].effect_index != 0 && debkeydata[i].field_2f9 != 0) {
                i32 handle = i;
                DebFreeInstantly(&handle);
            }
        }
    }

    void DebFreeAllDMADebTablesInstantly(void) {
        for (i32 i = 1; i < EDPP_MAX_TYPES; ++i) {
            debinftype *effect = debtab[i];
            if (effect != NULL && effect->native_data != NULL) {
                DmaDebTypes[--freeDmaDebType] = effect->native_data;
                effect->native_data = NULL;
            }
        }
    }

    void DebFreeAllPanelEffects(void) {
        for (i32 i = 0; i < maxdebkeys; ++i) {
            const i16 effect_index = debkeydata[i].effect_index;
            if (effect_index != 0 && debtab[effect_index]->time_group == 4) {
                i32 handle = i;
                DebFreeInstantly(&handle);
            }
        }
    }

    void Debris(i32 update_panel_time) {
        if (update_debris_enabled == 0 || debris_suspended != 0) {
            return;
        }

        panelglobaltime += timeincrement;
        renderpanelglobaltime += timeincrement;
        DebrisProcessControlChunks(1);
        if (update_panel_time == 0) {
            globaltime += timeincrement;
            renderglobaltime += timeincrement;
            ++globalframes;
            DebrisCleanUpDmaDebTypeTables();
            DebrisProcessTriggers();
            DebrisProcessAllocation();
            DebrisProcessGeneration();
            DebrisProcessControlChunks(0);
        }
    }

    void DebrisAllCollisionCheckScaleYFlag(void) {
        STUBBED();
    }

    void DebrisCollisionCheck(void) {
        STUBBED();
    }

    void DebrisCollisionCheckFlag(void) {
        STUBBED();
    }

    void DebrisCollisionCheckScaleY(void) {
        STUBBED();
    }

    void DebrisCollisionCheckScaleYFlag(void) {
        STUBBED();
    }

} // extern "C"

void DebrisTimeSlip(i32 group) {
    if (group == 0) {
        globaltime = renderglobaltime;
        for (debris_chunk_control_s *control = debris_chunk_control_stack[0]; control != NULL; control = control->next)
            control->expiry_time -= 800.0f;
    } else if (group == 1) {
        panelglobaltime = renderpanelglobaltime;
        for (debris_chunk_control_s *control = debris_chunk_control_stack[1]; control != NULL; control = control->next)
            control->expiry_time -= 800.0f;
    }
    for (i32 i = 0; i < maxdebkeys; ++i) {
        debkeydatatype_s *key = &debkeydata[i];
        debinftype *effect = debtab[key->effect_index];
        if (effect->time_group == 4 ? group != 1 : group != 0)
            continue;
        key->last_update_time -= 800.0f;
        key->emission_epoch -= 800.0f;
        key->emission_time -= 800.0f;
        key->previous_emission_time -= 800.0f;
        key->field_1e4 -= 800.0f;
        for (i32 chunk = 0; chunk < 32; ++chunk) {
            if (key->particle_chunks[chunk] == NULL)
                continue;
            dma_particle_chunk_s *particles = key->particle_chunks[chunk];
            i32 count = effect->particle_type == 7 ? 12 : 32;
            for (i32 particle = 0; particle < count; ++particle)
                particles->particles[particle].start_time -= 800.0f;
        }
    }
    for (i32 i = 1; i < EDPP_MAX_TYPES; ++i) {
        debinftype *effect = debtab[i];
        if (effect == NULL)
            continue;
        if (effect->time_group == 4 ? group != 1 : group != 0)
            continue;
        effect->last_render_time -= 800.0f;
        if (effect->native_data != NULL)
            effect->native_data->last_render_time -= 800.0f;
    }
}

extern "C" {

    void DebrisEmitterMomentum(i32 handle, f32 x, f32 y, f32 z) {
        if (handle != -1) {
            debkeydata[handle].emitter_momentum = NUVEC{x, y, z};
        }
    }

    void DebrisEmitterOrientation(i32 handle, i16 z, i16 y, i16 x) {
        if (handle == -1) {
            return;
        }
        debkeydatatype_s &key = debkeydata[handle];
        NUMTX *mtx = &key.emitter_orientation;
        NuMtxSetIdentity(mtx);
        NuMtxRotateZ(mtx, z);
        NuMtxRotateY(mtx, y);
        NuMtxRotateX(mtx, x);
        mtx->m30 = 0.0f;
        mtx->m31 = 0.0f;
        mtx->m32 = 0.0f;
        key.orientation_dirty = 0.0f;
    }

    void DebrisEmitterOrientationMtx(i32 handle, NUMTX *source) {
        if (handle != -1) {
            debkeydatatype_s &key = debkeydata[handle];
            NUMTX *mtx = &key.emitter_orientation;
            memcpy(mtx, source, sizeof(NUMTX));
            mtx->m30 = 0.0f;
            mtx->m31 = 0.0f;
            mtx->m32 = 0.0f;
            key.orientation_dirty = 0.0f;
        }
    }

    void DebrisEmitterPos(i32 handle, f32 x, f32 y, f32 z) {
        if (handle != -1) {
            debkeydatatype_s &key = debkeydata[handle];
            key.orientation_dirty = 0.0f;
            key.position.x = x;
            key.position.y = y;
            key.position.z = z;
        }
    }

    i32 DebrisFindAllOfType(i32 effect_index, NUVEC *positions, i32 *handles, i32 capacity, i32 skip, i32 active_only) {
        i32 count = 0;
        debkeydatatype_s *key = debkeydata;
        for (i32 i = 0; i < maxdebkeys; ++i, ++key) {
            if ((active_only == 0 || key->allocated_chunk_count != 0) && key->effect_index == effect_index) {
                i32 output_index = count - skip;
                if (output_index >= 0 && output_index < capacity) {
                    if (positions != NULL) {
                        positions[output_index].x = key->position.x;
                        positions[output_index].y = key->position.y;
                        positions[output_index].z = key->position.z;
                    }
                    if (handles != NULL)
                        handles[output_index] = i;
                }
                ++count;
            }
        }
        return count - skip;
    }

    i32 DebrisGetConeProperties(i32 handle, NUVEC *origin, NUVEC *end, f32 *start_radius, f32 *end_radius,
                                NUVEC *direction, f32 *length, f32 *speed) {
        debkeydatatype_s *key = &debkeydata[handle];
        debinftype *effect = debtab[key->effect_index];
        if (effect == NULL)
            return 0;
        origin->x = key->position.x;
        origin->y = key->position.y;
        origin->z = key->position.z;
        if (effect->generator_type == 8) {
            NUVEC axis = {0.0f, 1.0f, 0.0f};
            NuVecMtxTransform(&axis, &axis, &key->emitter_orientation);
            direction->x = axis.x;
            direction->y = axis.y;
            direction->z = axis.z;
            *speed = effect->field_048;
            *length = effect->field_048 * effect->particle_lifetime;
            end->x = *length * direction->x + origin->x;
            end->y = direction->y * *length + origin->y;
            end->z = direction->z * *length + origin->z;
            *start_radius = (effect->field_058 + effect->field_060) * 0.5f;
            *end_radius = (effect->field_04c + effect->field_054) / 2.8f * effect->particle_lifetime + *start_radius;
            return 1;
        }
        end->x = end->y = end->z = 0.0f;
        *direction = *end;
        *start_radius = *end_radius = *length = *speed = 0.0f;
        return 0;
    }

    f32 DebrisGetDuration(i32 effect_index) {
        if (effect_index < 0 || effect_index >= EDPP_MAX_TYPES || debtab[effect_index] == NULL)
            return 0.0f;
        debinftype *effect = debtab[effect_index];
        return effect->emission_period_random + effect->emission_pause + effect->emission_pause_random +
               effect->start_offset_random;
    }

    char *DebrisGetName(i32 effect_index) {
        if (effect_index < 0 || effect_index >= EDPP_MAX_TYPES)
            return NULL;
        debinftype *effect = debtab[effect_index];
        return effect == NULL ? NULL : effect->name;
    }

    i32 DebrisGetParticleCount(i32 handle) {
        if (handle == -1)
            return 0;
        return debkeydata[handle].allocated_chunk_count;
    }

    i32 DebrisGetRingProperties(i32 handle, NUVEC *origin, NUVEC *direction, f32 *radius) {
        debkeydatatype_s *key = &debkeydata[handle];
        debinftype *effect = debtab[key->effect_index];
        if (effect == NULL)
            return 0;
        origin->x = key->position.x;
        origin->y = key->position.y;
        origin->z = key->position.z;
        NUVEC axis;
        if (effect->generator_type == 11)
            axis = NUVEC{0.0f, 1.0f, 0.0f};
        else
            axis = NUVEC{0.0f, 0.0f, 1.0f};
        NuVecMtxTransform(&axis, &axis, &key->emitter_orientation);
        direction->x = axis.x;
        direction->y = axis.y;
        direction->z = axis.z;
        if (effect->generator_type == 6 || effect->generator_type == 7 || effect->generator_type == 11) {
            *radius = effect->field_058;
            return 1;
        }
        *radius = 0.0f;
        return 0;
    }

    void DebrisOrientation(i32 handle, i16 z, i16 y) {
        if (handle != -1) {
            NuMtxSetIdentity(&debkeydata[handle].effect_orientation);
            NUMTX *mtx = &debkeydata[handle].effect_orientation;

            const f32 cos_z = NU_COS_LUT(z);
            const f32 sin_z = NU_SIN_LUT(z);
            const f32 z_m00 = mtx->m00;
            const f32 z_m10 = mtx->m10;
            const f32 z_m20 = mtx->m20;
            const f32 z_m30 = mtx->m30;
            mtx->m00 = z_m00 * cos_z - mtx->m01 * sin_z;
            mtx->m01 = z_m00 * sin_z + mtx->m01 * cos_z;
            mtx->m10 = z_m10 * cos_z - mtx->m11 * sin_z;
            mtx->m11 = z_m10 * sin_z + mtx->m11 * cos_z;
            mtx->m20 = z_m20 * cos_z - mtx->m21 * sin_z;
            mtx->m21 = z_m20 * sin_z + mtx->m21 * cos_z;
            mtx->m30 = z_m30 * cos_z - mtx->m31 * sin_z;
            mtx->m31 = z_m30 * sin_z + mtx->m31 * cos_z;

            const f32 cos_y = NU_COS_LUT(y);
            const f32 sin_y = NU_SIN_LUT(y);
            const f32 y_m00 = mtx->m00;
            const f32 y_m10 = mtx->m10;
            const f32 y_m20 = mtx->m20;
            const f32 y_m30 = mtx->m30;
            mtx->m00 = y_m00 * cos_y + mtx->m02 * sin_y;
            mtx->m02 = mtx->m02 * cos_y - y_m00 * sin_y;
            mtx->m10 = y_m10 * cos_y + mtx->m12 * sin_y;
            mtx->m12 = mtx->m12 * cos_y - y_m10 * sin_y;
            mtx->m20 = y_m20 * cos_y + mtx->m22 * sin_y;
            mtx->m22 = mtx->m22 * cos_y - y_m20 * sin_y;
            mtx->m30 = y_m30 * cos_y + mtx->m32 * sin_y;
            mtx->m32 = mtx->m32 * cos_y - y_m30 * sin_y;
            debkeydata[handle].orientation_dirty = 0.0f;
        }
    }

    void DebrisOrientationMtx(i32 handle, NUMTX *source) {
        if (handle != -1) {
            debkeydatatype_s &key = debkeydata[handle];
            NUMTX *mtx = &key.effect_orientation;
            memcpy(mtx, source, sizeof(NUMTX));
            mtx->m30 = 0.0f;
            mtx->m31 = 0.0f;
            mtx->m32 = 0.0f;
            key.orientation_dirty = 0.0f;
        }
    }

    void DebrisParticleMomentum(i32 handle, f32 x, f32 y, f32 z) {
        if (handle != -1) {
            debkeydata[handle].momentum = NUVEC{x, y, z};
        }
    }

    void DebrisPopulateInstance(i32 handle, f32 duration) {
        if (duration < 0.0f || handle == -1) {
            return;
        }

        debkeydatatype_s *key = &debkeydata[handle];
        debinftype *effect = debtab[key->effect_index];
        if (effect->frequency == 0) {
            return;
        }
        if (duration == 0.0f) {
            duration = effect->particle_lifetime;
        }

        const f32 now = effect->time_group == 4 ? panelglobaltime : globaltime;
        const f32 end_time = now + timeincrement;
        f32 emission_time = now - duration;
        if (key->previous_allocated_chunk_count == 0) {
            const f32 thinning =
                forced_debris_thinning == 0
                    ? (debris_thinning_level <= effect->thinning ? debris_thinning_level : effect->thinning)
                    : debris_thinning_level;
            DebReAlloc(key, static_cast<i32>(static_cast<f32>(effect->max_particles) / thinning));
        }

        key->field_184 = 1;
        if (key->allocated_chunk_count <= 0) {
            return;
        }
        const f32 thinning =
            forced_debris_thinning == 0
                ? (debris_thinning_level <= effect->thinning ? debris_thinning_level : effect->thinning)
                : debris_thinning_level;
        const f32 emission_interval = 1.0f / (static_cast<f32>(effect->frequency) / thinning);
        key->emission_epoch = emission_time;
        emission_time += emission_interval;
        for (i32 remaining = 999; emission_time < end_time && remaining != 0; --remaining) {
            uv1deb *particle = key->generator(key, effect, emission_time);
            if (effect->process_spheres != 0 && particle != NULL) {
                DebrisProcessSpheres(particle, emission_time, effect, key, 0);
            }
            emission_time = key->emission_epoch + emission_interval;
        }
    }

    void DebrisPosOrientationMtx(i32 handle, NUMTX *source) {
        if (handle != -1) {
            debkeydatatype_s &key = debkeydata[handle];
            NUMTX *mtx = &key.effect_orientation;
            memcpy(mtx, source, sizeof(NUMTX));
            key.position.x = source->m30;
            key.position.y = source->m31;
            key.position.z = source->m32;
            mtx->m30 = 0.0f;
            mtx->m31 = 0.0f;
            mtx->m32 = 0.0f;
            key.orientation_dirty = 0.0f;
        }
    }

    void DebrisPreCheckCollisions(void) {
        STUBBED();
    }

    void DebrisProcessTimeSlip(void) {
        if (globaltime - 1.0f > renderglobaltime)
            DebrisTimeSlip(0);
        else if (globaltime > 900.0f)
            renderglobaltime -= 800.0f;
        if (panelglobaltime - 1.0f > renderpanelglobaltime)
            DebrisTimeSlip(1);
        else if (panelglobaltime > 900.0f)
            renderpanelglobaltime -= 800.0f;
    }

    i32 DebrisQueryPriority(i32 effect_index) {
        if (effect_index < 0 || effect_index >= EDPP_MAX_TYPES || debtab[effect_index] == NULL)
            return 0;
        i16 priority = 0;
        switch (static_cast<i8>(debtab[effect_index]->particle_type)) {
            case 0:
                priority = 20000;
                break;
            case 2:
                priority = -25536;
                break;
            case 3:
                priority = 30000;
                break;
            case 7:
                priority = 10000;
                break;
            default:
                break;
        }
        return priority;
    }

    void DebrisReScale(i32 effect_index, f32 scale) {
        if (effect_index < 0 || EDPP_MAX_TYPES <= effect_index || debtab[effect_index] == NULL) {
            return;
        }

        debinftype *effect = debtab[effect_index];
        for (i32 i = 0; i < 8; ++i) {
            effect->width_keys[i].value *= scale;
            effect->height_keys[i].value *= scale;
        }
        effect->min_size *= scale;
        effect->max_size *= scale;
        effect->field_048 *= scale;
        effect->field_0a0 *= scale;

        const u8 generator_type = effect->generator_type;
        if ((generator_type & 0xf7) == 0 || generator_type == 9 || generator_type == 10) {
            effect->field_058 *= scale;
            effect->field_05c *= scale;
            effect->field_060 *= scale;
            effect->field_04c *= scale;
            effect->field_050 *= scale;
            effect->field_054 *= scale;
        } else if (generator_type == 6 || generator_type == 7 || generator_type == 11 || generator_type == 12) {
            effect->field_058 *= scale;
            effect->field_04c *= scale;
        }
        effect->jib_x_amplitude *= scale;
        effect->jib_y_amplitude *= scale;
        GenericDebinfoDmaTypeUpdate(effect);
    }

    void DebrisReflectionOrientation(i32 handle, i16 x, i16 y, f32 plane, f32 scale) {
        if (handle != -1) {
            debkeydatatype_s &key = debkeydata[handle];
            key.collision_plane = plane;
            key.reflection_x = x;
            key.reflection_y = y;
            key.reflection_scale = scale;
        }
    }

    NUVEC *CutoffCameraVec = NULL;

    void DebrisRegisterCutoffCameraVec(void *position) {
        CutoffCameraVec = static_cast<NUVEC *>(position);
    }

    void DebrisReserveTrashableSpace(void) {
        STUBBED();
    }

    void DebrisResetTimers(void) {
        panelglobaltime = 0.0f;
        renderpanelglobaltime = 0.0f;
        globaltime = 0.0f;
        renderglobaltime = 0.0f;
        globalframes = 0;
    }

    void DebrisSetDetailLevels(i32 handle, i32 detail_levels) {
        debkeydata[handle].field_1da = static_cast<u8>(detail_levels);
    }

    void DebrisSetDrawFlag(i32 handle, i8 draw_flag) {
        if (handle != -1) {
            debkeydata[handle].field_2f7 = draw_flag;
        }
    }

    void DebrisSetFacing(i32 handle, u8 enabled, i16 x_angle, i16 y_angle) {
        if (handle == -1)
            return;
        debkeydata[handle].field_2fa = enabled;
        NuMtxSetIdentity(&debkeydata[handle].particle_orientation);
        NUMTX *matrix = &debkeydata[handle].particle_orientation;
        f32 sine = NU_SIN_LUT(x_angle);
        f32 cosine = NU_COS_LUT(x_angle);
        f32 y0 = matrix->m01;
        f32 z0 = matrix->m02;
        matrix->m01 = cosine * y0 - z0 * sine;
        matrix->m02 = sine * y0 + z0 * cosine;
        f32 y1 = matrix->m11;
        f32 z1 = matrix->m12;
        matrix->m11 = cosine * y1 - z1 * sine;
        matrix->m12 = sine * y1 + z1 * cosine;
        f32 y2 = matrix->m21;
        f32 z2 = matrix->m22;
        matrix->m21 = cosine * y2 - z2 * sine;
        matrix->m22 = sine * y2 + z2 * cosine;
        f32 y3 = matrix->m31;
        f32 z3 = matrix->m32;
        matrix->m31 = cosine * y3 - z3 * sine;
        matrix->m32 = sine * y3 + z3 * cosine;
        sine = NU_SIN_LUT(y_angle);
        cosine = NU_COS_LUT(y_angle);
        f32 x0 = matrix->m00;
        z0 = matrix->m02;
        matrix->m00 = x0 * cosine + z0 * sine;
        matrix->m02 = z0 * cosine - x0 * sine;
        f32 x1 = matrix->m10;
        z1 = matrix->m12;
        matrix->m10 = x1 * cosine + z1 * sine;
        matrix->m12 = z1 * cosine - x1 * sine;
        f32 x2 = matrix->m20;
        z2 = matrix->m22;
        matrix->m20 = x2 * cosine + z2 * sine;
        matrix->m22 = z2 * cosine - x2 * sine;
        f32 x3 = matrix->m30;
        z3 = matrix->m32;
        matrix->m30 = x3 * cosine + z3 * sine;
        matrix->m32 = z3 * cosine - x3 * sine;
    }

    void DebrisSetGroupID(i32 handle, i16 group) {
        if (handle != -1)
            debkeydata[handle].render_group = group;
    }

    void DebrisSetPriority(i32 handle, i16 priority, u8 automatic) {
        if (handle != -1) {
            debkeydatatype_s *key = &debkeydata[handle];
            key->timed_flags = automatic;
            if (automatic == 0)
                key->render_priority = priority;
        }
    }

    i32 debris_render_group;
    void DebrisSetRenderGroup(i32 group) {
        debris_render_group = group;
    }

    void DebrisSetRoomID(i32 handle, nugscn_s *scene) {
        if (scene != NULL) {
            debkeydata[handle].field_2f2 = NuPortalWhichRoom(scene, &debkeydata[handle].position);
            if (debkeydata[handle].field_2f2 != -1)
                debkeydata[handle].gscene = scene;
            else
                debkeydata[handle].gscene = NULL;
        } else {
            debkeydata[handle].field_2f2 = -1;
            debkeydata[handle].gscene = NULL;
        }
    }

    void DebrisSetTrigger(i32 handle, i32 first, i32 second, f32 third) {
        if (handle != -1) {
            debkeydatatype_s &key = debkeydata[handle];
            key.switch_variable = third;
            key.trigger_first = first;
            key.trigger_second = second;
        }
    }

    void DebrisSetUserData(i32 handle, void *user_data) {
        if (handle != -1)
            debkeydata[handle].user_data = user_data;
    }

    void DebrisShift(NUMTX *transform) {
        for (i32 i = 0; i < maxdebkeys; ++i) {
            debkeydatatype_s *key = &debkeydata[i];
            if (key->effect_index == 0)
                continue;
            debinftype *effect = debtab[key->effect_index];
            i32 slot;
            for (slot = 0; slot < 8; ++slot) {
                if (effect->particle_keys[slot] == i)
                    break;
            }
            if (slot != 8) {
                i32 group = effect->time_group == 4;
                if (key->controlled_chunk_count != 0) {
                    for (i32 chunk = 0; chunk < key->controlled_chunk_count; ++chunk) {
                        for (debris_chunk_control_s *control = debris_chunk_control_stack[group]; control != NULL;
                             control = control->next) {
                            if (control->particle_chunk == key->particle_chunks[chunk]) {
                                RemoveChunkControlFromStack(control, &debris_chunk_control_stack[group]);
                                freechunkcontrols[--freechunkcontrolsptr] = control;
                                break;
                            }
                        }
                    }
                    key->controlled_chunk_count = 0;
                }
                DebReAlloc(&debkeydata[i], 0);
            } else {
                NuMtxMulR(&key->effect_orientation, transform, &key->effect_orientation);
                NuVecMtxTransform(&debkeydata[i].position, &debkeydata[i].position, transform);
                debkeydata[i].orientation_dirty = 0.0f;
            }
        }
        for (i32 i = 0; i < debrischunks + debrischunksglass; ++i) {
            particlechunkrendertype_s *chunk = &ParticleChunkToRender[i];
            if (chunk->particle_chunk != NULL && chunk->key == NULL) {
                NuMtxMulR(&chunk->effect_orientation, transform, &chunk->effect_orientation);
                NuVecMtxTransform(&ParticleChunkToRender[i].position, &ParticleChunkToRender[i].position, transform);
            }
        }
    }

    void DebrisStartOffset(i32 handle, f32 offset) {
        if (handle != -1) {
            DebrisStartOffsetEx(debkeydata + handle, offset);
        }
    }

    void DebrisStatusAlwaysOff(i32 *handle) {
        if (*handle != -1)
            debkeydata[*handle].field_2f4 = 0;
    }

    void DebrisStatusAlwaysOn(i32 *handle) {
        if (*handle != -1)
            debkeydata[*handle].field_2f4 = 2;
    }

    void DebrisStatusNormal(i32 *handle) {
        if (*handle != -1)
            debkeydata[*handle].field_2f4 = 1;
    }

    void DebrisTorusCollisionCheck(void) {
        STUBBED();
    }

    void DebrisTorusCollisionCheckFlag(void) {
        STUBBED();
    }

    void DebrisTorusCollisionCheckScaleY(void) {
        STUBBED();
    }

    void DebrisTorusCollisionCheckScaleYFlag(void) {
        STUBBED();
    }

    void DebrisTrashableSetup(VARIPTR *buffer, VARIPTR *) {
        if (debris_setup_called == 0) {
            return;
        }

        usize cursor = debris_trash_space;
        if (cursor == 0) {
            if (buffer == NULL) {
                return;
            }
            buffer->addr = ALIGN(buffer->addr, 0x10);
            cursor = buffer->addr;
        }

        const i32 chunk_count = debrischunks + debrischunksglass;
        memset(debris_chunk_controls, 0, static_cast<usize>(chunk_count) * 2 * sizeof(debris_chunk_control_s));
        memset(freechunkcontrols, 0, static_cast<usize>(chunk_count) * 2 * sizeof(debris_chunk_control_s *));
        memset(freedebchunks, 0, static_cast<usize>(debrischunks) * sizeof(dma_particle_chunk_s *));
        memset(freedebchunksglass, 0, static_cast<usize>(debrischunksglass) * sizeof(dma_particle_chunk_s *));
        memset(ParticleChunkToRender, 0, static_cast<usize>(chunk_count) * sizeof(particlechunkrendertype_s));

        for (i32 i = 0; i < debrischunks; ++i) {
            i32 size;
            freedebchunks[i] = CreateDmaParticleSet(reinterpret_cast<void *>(cursor), &size);
            cursor += static_cast<usize>(size);
        }
        for (i32 i = 0; i < debrischunksglass; ++i) {
            i32 size;
            freedebchunksglass[i] = CreateDmaParticleSetGlass(reinterpret_cast<void *>(cursor), &size);
            cursor += static_cast<usize>(size);
        }

        freedebchkptr = 0;
        freedebchkptrg = 0;
        for (i32 i = 0; i < EDPP_MAX_DMADEBTYPES; ++i) {
            i32 size;
            DmaDebTypes[i] = CreateDmaPartEffectList(reinterpret_cast<void *>(cursor), &size);
            cursor += static_cast<usize>(size);
        }

        if (debris_trash_space == 0) {
            debris_trash_space = buffer->addr;
            debris_trash_size = cursor - buffer->addr;
            buffer->addr = cursor;
        }

        debris_chunk_control_stack[0] = NULL;
        debris_chunk_control_stack[1] = NULL;
        for (i32 i = 0; i < chunk_count * 2; ++i) {
            freechunkcontrols[i] = &debris_chunk_controls[i];
        }
        freechunkcontrolsptr = 0;
        for (i32 i = 0; i < chunk_count; ++i) {
            ParticleChunkToRender[i].particle_chunk = NULL;
            ParticleChunkToRender[i].effect = NULL;
            ParticleChunkToRender[i].key = NULL;
            ParticleChunkToRender[i].previous = NULL;
            ParticleChunkToRender[i].next = NULL;
        }
        ParticleChunkRenderStack[0] = NULL;
        ParticleChunkRenderStack[1] = NULL;
        ParticleChunkRenderStack[2] = NULL;
        ParticleChunkRenderStack[3] = NULL;
        ParticleChunkRenderStack[4] = NULL;
    }

    void DebrisTypeStatusAlwaysOff(i32 type) {
        if (type != -1)
            debtab[type]->status = 0;
    }

    void DebrisTypeStatusAlwaysOn(i32 type) {
        if (type != -1)
            debtab[type]->status = 2;
    }

    void DebrisTypeStatusNormal(i32 type) {
        if (type != -1)
            debtab[type]->status = 1;
    }

    i32 DeletePlatinst(i32 index) {
        if (index < 0 || index >= CurTerr->max_platforms)
            return 0;
        TERRAIN_PLATFORM &platform = CurTerr->platforms[index];
        if (platform.scene_object == NULL)
            return 0;
        const i16 last_group = --CurTerr->group_count;
        --CurTerr->group_index_count;
        TERRAIN_GROUP &last = CurTerr->groups[last_group];
        CurTerr->platforms[last.scene_index].terrain_group_index = platform.terrain_group_index;
        CurTerr->groups[platform.terrain_group_index] = last;
        last.chunk_type = -1;
        platform.scene_object = NULL;
        TERRAIN_CELL &cell = CurTerr->cells[TERRAIN_PLATFORM_CELL];
        i16 *indices = CurTerr->group_indices + cell.first_group;
        for (i32 i = 0; i < cell.group_count; ++i) {
            if (indices[i] == last_group) {
                indices[i] = CurTerr->group_indices[CurTerr->group_index_count];
                break;
            }
        }
        --cell.group_count;
        ScanTerrIDRemovePlat(index);
        return 1;
    }

    void DrawPlatform(void) {
        STUBBED();
    }

    i32 FindPlatInst(i32 instance) {
        if (instance != -1) {
            for (i32 i = 0; i < CurTerr->max_platforms; ++i) {
                TERRAIN_PLATFORM *platform = &CurTerr->platforms[i];
                if (platform->scene_object != NULL && static_cast<i16>(platform->scene_object_index) == instance)
                    return i;
            }
        }
        return -1;
    }

    i32 NewMSituTerrEx(NUVEC *position, void *data, NUVEC *minimum, NUVEC *maximum, f32 radius) {
        TERRSET *terrain = CurTerr;
        if (terrain == NULL || terrain->group_index_count >= terrain->max_group_indices ||
            terrain->group_count >= terrain->max_groups)
            return -1;
        i16 group_index = terrain->group_count;
        TERRAIN_GROUP &group = terrain->groups[group_index];
        group.platform_flags &= ~1;
        group.origin = *position;
        group.data = data;
        group.bounds_min = *minimum;
        group.bounds_max = *maximum;
        group.field_0x32 = -1;
        group.scene_index = -1;
        group.chunk_type = 0;
        group.radius = radius;
        terrain->group_indices[terrain->group_index_count++] = group_index;
        ++terrain->cells[TERRAIN_PLATFORM_CELL].group_count;
        ++terrain->group_count;
        return terrain->group_count - 1;
    }

    i32 NewPlatInst(void *object, i32 instance) {
        if (CurTerr == NULL || CurTerr->group_index_count >= CurTerr->max_group_indices ||
            CurTerr->group_count >= CurTerr->max_groups || object == NULL || CurTerr->max_platforms <= 0)
            return -1;
        i32 index = 0;
        while (CurTerr->platforms[index].scene_object != NULL) {
            if (++index == CurTerr->max_platforms)
                return -1;
        }
        for (i32 source = 0; source < CurTerr->max_platforms; ++source) {
            TERRAIN_PLATFORM &original = CurTerr->platforms[source];
            if (original.scene_object == NULL || static_cast<i16>(original.scene_object_index) != instance)
                continue;
            const i16 group_index = CurTerr->group_count;
            TERRAIN_GROUP &group = CurTerr->groups[group_index];
            group = CurTerr->groups[original.terrain_group_index];
            group.scene_index = index;
            group.chunk_type = 1;
            TERRAIN_PLATFORM &platform = CurTerr->platforms[index];
            platform.scene_object = object;
            platform.terrain_group_index = group_index;
            platform.scene_object_index = instance;
            platform.scene_transform = NULL;
            platform.flags = (platform.flags & ~1) | (original.flags & 1);
            CurTerr->group_indices[CurTerr->group_index_count++] = group_index;
            platform.bounce_impulse = 0.0f;
            platform.bounce_offset = 0.0f;
            platform.bounce_velocity = 0.0f;
            platform.bounce_damping = 0.0f;
            platform.bounce_spring = 0.0f;
            ++CurTerr->group_count;
            ++CurTerr->cells[TERRAIN_PLATFORM_CELL].group_count;
            return index;
        }
        return -1;
    }

    i32 NewPlatInstMSitu(void *object, i32 source) {
        TERRSET *terrain = CurTerr;
        if (terrain == NULL || terrain->group_index_count >= terrain->max_group_indices ||
            terrain->group_count >= terrain->max_groups || object == NULL || terrain->max_platforms <= 0)
            return -1;
        i32 index = 0;
        while (terrain->platforms[index].scene_object != NULL) {
            if (++index == terrain->max_platforms)
                return -1;
        }
        if (index == -1 || source == -1)
            return -1;
        i16 group_index = terrain->group_count;
        TERRAIN_GROUP &group = terrain->groups[group_index];
        group = terrain->groups[source];
        group.scene_index = index;
        group.chunk_type = 1;
        TERRAIN_PLATFORM &platform = terrain->platforms[index];
        platform.scene_object = object;
        platform.terrain_group_index = group_index;
        platform.scene_object_index = source;
        platform.flags &= ~TERRAIN_PLATFORM_FLAG_ROTATING;
        platform.scene_transform = NULL;
        platform.bounce_impulse = 0.0f;
        platform.bounce_offset = 0.0f;
        platform.bounce_velocity = 0.0f;
        platform.bounce_damping = 0.0f;
        platform.bounce_spring = 0.0f;
        terrain->group_indices[terrain->group_index_count++] = group_index;
        ++terrain->cells[TERRAIN_PLATFORM_CELL].group_count;
        ++terrain->group_count;
        return index;
    }

    i32 NewRayCastEx(NUVEC *position, NUVEC *movement, f32 radius, i32 scan_flags) {
        return NewRayCast(position, movement, radius, scan_flags);
    }

    i32 NewRayCastScaleY(NUVEC *position, NUVEC *movement, f32 radius, f32 scale_y, i32 scan_flags) {
        return NewRayCastScaleYMask(position, movement, radius, scale_y, scan_flags, 0);
    }

    f32 NewShadowEx(NUVEC *position, i32 handle, f32 height_above, f32 height_below, i32 terrain_mask);

    f32 NewShadow(NUVEC *position, f32 height_above, f32 height_below, i32 terrain_mask) {
        return NewShadowEx(position, 0, height_above, height_below, terrain_mask);
    }

    void NewTerrHitInfo(u8 *info) {
        info[0] = TerrainHitInfo[0];
        info[1] = TerrainHitInfo[1];
        info[2] = TerrainHitInfo[2];
        info[3] = TerrainHitInfo[3];
    }

    void NewTerrainScaleY(NUVEC *position, NUVEC *movement, u8 *hit_flags, i32 object_index, f32 radius,
                          f32 collision_radius, f32 object_scale, i32 embedded_retry, i32 scan_flags);

    void NewTerrain(NUVEC *position, NUVEC *movement, u8 *hit_flags, i32 object_index, f32 radius, f32 collision_radius,
                    i32 scan_flags) {
        if (CurTerr != NULL)
            NewTerrainScaleY(position, movement, hit_flags, object_index, radius, collision_radius, 1.0f, 0,
                             scan_flags);
    }

    void NewTerrainScaleY(NUVEC *position, NUVEC *movement, u8 *hit_flags, i32 object_index, f32 radius,
                          f32 collision_radius, f32 object_scale, i32 embedded_retry, i32 scan_flags) {
        NewTerrainScaleYMask(position, movement, hit_flags, object_index, radius, collision_radius, object_scale,
                             embedded_retry, scan_flags, 0);
    }

    void PartTerrInit(void) {
    }

    void PlatInstBounce(i32 index, f32 impulse, f32 spring, f32 damping) {
        if (index >= 0 && index < CurTerr->max_platforms) {
            TERRAIN_PLATFORM &platform = CurTerr->platforms[index];
            if (platform.bounce_impulse == 0.0f) {
                platform.bounce_offset = 0.0f;
                platform.bounce_velocity = 0.0f;
            }
            platform.bounce_impulse = impulse;
            platform.bounce_spring = spring;
            platform.bounce_damping = damping;
        }
    }

    void PlatInstCenter(i32 index, NUVEC *position) {
        if (CurTerr != NULL && index >= 0 && index < CurTerr->max_platforms)
            *position = CurTerr->groups[CurTerr->platforms[index].terrain_group_index].origin;
    }

    i32 PlatInstGetHit(i32 index) {
        if (CurTerr != NULL && index >= 0 && index < CurTerr->max_platforms)
            return (CurTerr->platforms[index].flags >> 1) & 1;
        return 0;
    }

    void PlatInstRotate(i32 index, i32 rotate) {
        if (index >= 0 && index < CurTerr->max_platforms) {
            TERRAIN_PLATFORM &platform = CurTerr->platforms[index];
            platform.flags = (platform.flags & ~1) | (rotate & 1);
        }
    }

    void PlatSkinMemRigister(void *start, void *end, i32 maximum) {
        PlatSkinMax = maximum;
        PlatSkinInfo = static_cast<PLATSKININFO *>(start);
        PlatSkinMem = reinterpret_cast<u8 *>(PlatSkinInfo + maximum);
        PlatSkinMemEnd = static_cast<u8 *>(end);
        PlatSkinMaxSize = 0;
        PlatSkinCnt = 0;
        PlatSkinResetTotal = CurTerr->group_count;
    }

    void PlatSkinMemReset(void) {
        if (PlatSkinResetTotal >= 0) {
            CurTerr->group_count = PlatSkinResetTotal;
            for (i32 i = 0; i < 16; ++i)
                CurTerr->index_levels[i].entry_count = 0;
        }
    }

    i32 PlatInstSkinRegisterEx(NUMTX *matrix, void *skin_data, void *matrix_data, f32 scale, i32 instance, i32 flags,
                               TERRSET *source) {
        TERRSET *terrain = CurTerr;
        if (terrain == NULL || source == NULL)
            return -1;
        i32 group_index = terrain->group_count;
        if (group_index >= terrain->max_groups)
            return -3;
        if (skin_data == NULL || matrix == NULL)
            return -4;
        if (PlatSkinCnt >= PlatSkinMax)
            return -5;
        TERRAIN_PLATFORM *platform = source->platforms;
        i32 i;
        for (i = 0; i < source->max_platforms; ++i, ++platform) {
            if (platform->scene_object != NULL && static_cast<i16>(platform->scene_object_index) == instance)
                break;
        }
        if (i == source->max_platforms || source->max_platforms <= 0)
            return -6;
        TERRAIN_GROUP &group = terrain->groups[group_index];
        group = source->groups[platform->terrain_group_index];
        group.chunk_type = 0;
        PLATSKININFO *info = &PlatSkinInfo[PlatSkinCnt];
        info->matrix = matrix;
        info->skin_data = skin_data;
        info->matrix_data = matrix_data;
        info->scale = scale;
        info->flags = flags;
        info->mirrored = matrix->m02 * matrix->m20 - matrix->m00 * matrix->m22 > 0.0f;
        SkinPlatformSize(group_index, PlatSkinMem, info);
        info = &PlatSkinInfo[PlatSkinCnt];
        info->terrain_group = CurTerr->group_count;
        TERRAIN_GROUP &registered_group = CurTerr->groups[CurTerr->group_count];
        info->terrain_data = registered_group.data;
        registered_group.data = NULL;
        registered_group.scene_index = ~PlatSkinCnt;
        ++CurTerr->group_count;
        ++PlatSkinCnt;
        return CurTerr->group_count - 1;
    }

    i32 PlatInstSkinRegister(NUMTX *matrix, void *skin_data, void *matrix_data, f32 scale, i32 instance, i32 flags) {
        return PlatInstSkinRegisterEx(matrix, skin_data, matrix_data, scale, instance, flags, CurTerr);
    }

    void PlatOnOff(i32 index, i32 enabled) {
        if (CurTerr != NULL && index >= 0 && index < CurTerr->max_platforms) {
            TERRAIN_GROUP &group = CurTerr->groups[CurTerr->platforms[index].terrain_group_index];
            if (enabled != 0) {
                group.chunk_type = 1;
                return;
            }
            group.chunk_type = -1;
        }
    }

    void *PlatSkinEndReigster(i32 cache_slots) {
        TERRSET *terrain = CurTerr;
        i16 *group_cells = reinterpret_cast<i16 *>(reinterpret_cast<uintptr_t>(PlatSkinMemEnd) & ~uintptr_t(1));
        group_cells -= terrain->max_groups;
        f32 *group_min_x = reinterpret_cast<f32 *>(reinterpret_cast<uintptr_t>(group_cells) & ~uintptr_t(3));
        group_min_x -= terrain->max_groups;
        f32 *group_max_x = group_min_x - terrain->max_groups;
        f32 *group_min_z = group_max_x - terrain->max_groups;
        f32 *group_max_z = group_min_z - terrain->max_groups;
        TERRAIN_GROUP *groups = terrain->groups;
        f32 minimum_x = 200000000.0f, minimum_z = 200000000.0f;
        f32 maximum_x = -200000000.0f, maximum_z = -200000000.0f;
        for (i32 i = 0; i < terrain->group_count; ++i) {
            TERRAIN_GROUP &group = groups[i];
            if (group.chunk_type == 0) {
                minimum_x = MIN(group.bounds_min.x, minimum_x);
                minimum_z = MIN(group.bounds_min.z, minimum_z);
                maximum_x = MAX(group.bounds_max.x, maximum_x);
                maximum_z = MAX(group.bounds_max.z, maximum_z);
            } else if (group.chunk_type == 1) {
                if (minimum_x > group.bounds_min.x)
                    minimum_x = group.origin.x + group.bounds_min.x;
                if (minimum_z > group.bounds_min.z)
                    minimum_z = group.origin.z + group.bounds_min.z;
                if (maximum_x < group.bounds_max.x)
                    maximum_x = group.origin.x + group.bounds_max.x;
                if (maximum_z < group.bounds_max.z)
                    maximum_z = group.origin.z + group.bounds_max.z;
            }
        }
        terrain->group_index_count = 0;
        for (i32 i = 0; i < terrain->group_count; ++i) {
            TERRAIN_GROUP &group = groups[i];
            if (group.chunk_type != 0)
                continue;
            group_min_x[i] = MIN(group.bounds_min.x, 200000000.0f);
            group_min_z[i] = MIN(group.bounds_min.z, 200000000.0f);
            group_max_x[i] = MAX(group.bounds_max.x, -200000000.0f);
            group_max_z[i] = MAX(group.bounds_max.z, -200000000.0f);
            i32 x_distance = static_cast<i32>((group_min_x[i] + group_max_x[i]) * 0.5f - minimum_x);
            i32 x_cell = static_cast<i32>(static_cast<f32>(x_distance * 7) / (maximum_x - minimum_x));
            if (x_cell < 0)
                x_cell = 0;
            else if (x_cell > 6)
                x_cell = 6;
            i32 z_distance = static_cast<i32>((group_min_z[i] + group_max_z[i]) * 0.5f - minimum_z);
            i32 z_cell = static_cast<i32>(static_cast<f32>(z_distance * 7) / (maximum_z - minimum_z));
            if (z_cell < 0)
                z_cell = 0;
            else if (z_cell > 6)
                z_cell = 6;
            group_cells[i] = x_cell + z_cell * 7;
        }
        for (i32 i = 0; i < TERRAIN_CELL_RECORD_COUNT; ++i)
            terrain->cells[i].group_count = 0;
        terrain->used_cell_count = 0;
        for (i32 cell_index = 0; cell_index < TERRAIN_GRID_CELL_COUNT; ++cell_index) {
            TERRAIN_CELL &cell = terrain->cells[terrain->used_cell_count];
            cell.first_group = terrain->group_index_count;
            f32 min_x = 200000000.0f, min_z = 200000000.0f;
            f32 max_x = -200000000.0f, max_z = -200000000.0f;
            for (i32 i = 0; i < terrain->group_count; ++i) {
                if (group_cells[i] != cell_index || groups[i].chunk_type != 0)
                    continue;
                min_x = MIN(group_min_x[i], min_x);
                min_z = MIN(group_min_z[i], min_z);
                max_x = MAX(group_max_x[i], max_x);
                max_z = MAX(group_max_z[i], max_z);
                ++cell.group_count;
                terrain->group_indices[terrain->group_index_count++] = i;
            }
            if (cell.group_count != 0) {
                cell.min_x = min_x;
                cell.min_z = min_z;
                cell.max_x = max_x;
                cell.max_z = max_z;
                ++terrain->used_cell_count;
            }
        }
        TERRAIN_CELL &platform_cell = terrain->cells[TERRAIN_PLATFORM_CELL];
        platform_cell.first_group = terrain->group_index_count;
        TERRAIN_GROUP *group = groups;
        for (i32 i = 0; i < terrain->group_count; ++i) {
            // The original advances this pointer only for a secondary group.
            if (group->chunk_type != 1)
                continue;
            terrain->group_indices[terrain->group_index_count++] = i;
            ++platform_cell.group_count;
            terrain->platforms[group->scene_index].terrain_group_index = i;
            ++group;
        }
        SkinMemInfo = reinterpret_cast<PLATSKINMEMINFO *>(PlatSkinMem + cache_slots * PlatSkinMaxSize);
        for (i32 i = 0; i < cache_slots; ++i) {
            SkinMemInfo[i].last_used = 0;
            SkinMemInfo[i].skin_index = -1;
        }
        PlatSkinMaxStore = cache_slots;
        return SkinMemInfo + cache_slots;
    }

    void PlatformRemoveCallback(void (*function)(void *)) {
        for (i32 i = 0; i < PlatCodeCallback; ++i) {
            if (PlatCallback[i].function == function) {
                --PlatCodeCallback;
                PlatCallback[i].function = PlatCallback[PlatCodeCallback].function;
                PlatCallback[i].argument = PlatCallback[PlatCodeCallback].argument;
                --i;
            }
        }
    }

    void PlatformUpdateCallback(void (*function)(void *), void *argument) {
        if (PlatCodeCallback < 8) {
            PlatCallback[PlatCodeCallback].function = function;
            PlatCallback[PlatCodeCallback].argument = argument;
            ++PlatCodeCallback;
        }
    }

    void SortDebrisRenderStack(void) {
        if (--debris_sort_delay > 0) {
            return;
        }
        debris_sort_delay = 6;

        i32 stack_index = debris_sort_this_stack;
        while (ParticleChunkRenderStack[stack_index] == NULL) {
            if (++stack_index >= 5) {
                stack_index = 0;
            }
            if (stack_index == debris_sort_this_stack) {
                debris_sort_this_stack = stack_index;
                if (ParticleChunkRenderStack[stack_index] == NULL) {
                    return;
                }
                break;
            }
        }
        debris_sort_this_stack = stack_index;

        particlechunkrendertype_s *chunk = ParticleChunkRenderStack[stack_index];
        while (chunk->next != NULL) {
            particlechunkrendertype_s *next = chunk->next;
            const u16 priority = static_cast<u16>(chunk->render_priority);
            const u16 next_priority = static_cast<u16>(next->render_priority);
            if (priority < next_priority ||
                (priority == next_priority && chunk->effect->particle_type > next->effect->particle_type)) {
                RemoveChunkFromRenderStack(next, &ParticleChunkRenderStack[stack_index]);
                AddChunkToRenderStack(next, &ParticleChunkRenderStack[debris_sort_this_stack]);
                return;
            }
            chunk = next;
        }
    }

    const char *TerrErrorString(i32 error) {
        static const char *const errors[] = {"ERR_UNKNOWN", "ERR_NOTERR",      "ERR_MAXTERLIST", "ERR_MAXTERR",
                                             "ERR_INOUT",   "ERR_PLATSKINMAX", "ERR_NOINSTANCE"};
        i32 index = -error;
        if (index >= 7)
            index = 0;
        return errors[index];
    }

    extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId() {
        TERRSET *terrain = CurTerr;
        if (terrain == NULL) {
            return NULL;
        }

        for (i32 slot_index = 0; slot_index < TERRAIN_TRACK_SLOT_COUNT; ++slot_index) {
            if (terrain->track_slots[slot_index].id == NULL) {
                return &terrain->track_slots[slot_index];
            }
        }

        return NULL;
    }

    i32 TerrainFreeId(void *id) {
        if (CurTerr != NULL) {
            for (i32 i = 0; i < 64; ++i) {
                if (CurTerr->track_slots[i].id == id) {
                    CurTerr->track_slots[i].id = NULL;
                    return 1;
                }
            }
        }
        return 0;
    }

    TERRSET *TerrainGetCur(void) {
        return CurTerr;
    }

    TERRAIN_GROUP *TerrainGetModelByInst(i32 instance) {
        if (CurTerr != NULL && CurTerr->max_platforms > 0) {
            TERRAIN_PLATFORM *platform = CurTerr->platforms;
            for (i32 i = 0; i < CurTerr->max_platforms; ++i, ++platform) {
                if (platform->scene_object != NULL && static_cast<i16>(platform->scene_object_index) == instance)
                    return &CurTerr->groups[platform->terrain_group_index];
            }
        }
        return NULL;
    }

    void TerrainPlatGetMtx(i32 index, NUMTX **previous, NUMTX **current) {
        if (index >= 0) {
            *previous = &CurTerr->platforms[index].previous_matrix;
            *current = static_cast<NUMTX *>(CurTerr->platforms[index].scene_object);
        }
    }

    void UpdateDebrisRenderStackPriority(void) {
        if (--debris_update_delay > 0) {
            return;
        }
        debris_update_delay = 6;

        i32 stack_index = debris_update_this_stack;
        while (ParticleChunkRenderStack[stack_index] == NULL) {
            if (++stack_index >= 5) {
                stack_index = 0;
            }
            if (stack_index == debris_update_this_stack) {
                debris_update_this_stack = stack_index;
                if (ParticleChunkRenderStack[stack_index] == NULL) {
                    return;
                }
                break;
            }
        }
        debris_update_this_stack = stack_index;

        particlechunkrendertype_s *chunk = ParticleChunkRenderStack[debris_sort_this_stack];
        while (chunk != NULL) {
            debkeydatatype_s *key = chunk->key;
            if (key != NULL) {
                if (key->timed_flags != 0) {
                    const f32 priority = static_cast<u16>(key->render_priority) + key->cutoff_distance * 25.0f;
                    i32 updated_priority = -1;
                    if (!(priority > 65535.0f)) {
                        updated_priority = static_cast<i32>(priority);
                    }
                    chunk->render_priority = static_cast<i16>(updated_priority);
                } else {
                    chunk->render_priority = key->render_priority;
                }
            }
            chunk = chunk->next;
        }
    }

    void *terraininit(i32 level_num, void *buffer, void *buffer_end, i32 options, char *path, void *scene, i32 group) {
        return TerrainInitEx(level_num, buffer, buffer_end, options, path, scene, group, 0x1000, 0xc00, 0x100);
    }

    void UpdatePlatinst(i32 index, void *object) {
        if (index >= 0 && index < CurTerr->max_platforms)
            CurTerr->platforms[index].scene_object = object;
    }

} // extern "C"
