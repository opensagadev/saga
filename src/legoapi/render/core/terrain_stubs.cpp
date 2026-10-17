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
#include "nu2api/numath/numtx_inline.h"
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
extern TERRSET * CurTerr;
// Runtime-selected groups appended after the fixed terrain allocation.
i32 WallSplinesOnly;
extern "C" {
    i32 IgnoreWallSplines;
}
extern f32 TerrPlatScanDist;
extern i32 cntrots;
#include "legoapi/render/core/terrain_stubs_types.h"
void ScanTerrIDRemovePlat(i32 platform_index);

void TerrainSkinAllocate(terrsitu_s *terrain_group);

void SkinPlatformSize(i32 group_index, unsigned char *buffer, PLATSKININFO *info);

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
void TerrDrawPlat(tertype *terrain, i16 index);
void RemoveChunkControlFromStack(debris_chunk_control_s *, debris_chunk_control_s **);
i32 DebrisSingleCollisionCheckScaleYFlag(i32, NUVEC *, f32, f32, u8);
i32 DebrisSingleTorusCollisionCheckScaleYFlag(i32, NUVEC *, f32, f32, u8);

extern "C" {
    i32 DebrisCollisionCheckScaleYFlag(NUVEC *, f32, f32, u8);
    i32 DebrisCollisionCheckScaleY(NUVEC *, f32, f32);
    i32 DebrisTorusCollisionCheckScaleYFlag(NUVEC *, f32, f32, u8);
    i32 DebrisTorusCollisionCheckScaleY(NUVEC *, f32, f32);
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
    extern debkeydatatype_s *debris_keystack;
    extern u32 *spherecolldata;
    extern u32 *toruscolldata;
    extern i32 numspherecolldata;
    extern i32 numtoruscolldata;
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

void DebrisFreeOldestDmaDebTypeTable();

extern "C" i32 edppLoadPage(char *path, i32 flag, usize scene);
// NuFileExists @nufile (nucore_plain.cpp).
extern "C" i32 NuFileExists(char *name);

extern "C" void TerrainSetCur(void *terrain);
extern "C" void TerrSetPlatScanDist(f32 dist);

extern "C" void TerrainPlatformNewUpdate(void);
extern "C" i32 PARTLookupTypePageOnly(char *, i32);
extern "C" part_type_s part_types[128];
extern "C" i32 part_types_used;
extern "C" f32 partglobaltime;
void edpartScaleType(i32 index, f32 scale);
void *InitPartDebris(VARIPTR *buf, VARIPTR *, i32 capacity, i32 named_count, char **names, i32 page) {
    PARTDEBSYS_s *system = static_cast<PARTDEBSYS_s *>(BUFFER_ALLOC(buf, sizeof(PARTDEBSYS_s), 16));
    if (__builtin_expect(system == NULL, 0))
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
        if (__builtin_expect(part_types[type].variant_count > 0, 1)) {
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

    world->debris_sys = (APIDEBRISSYS_s *)InitGameDebris(&world->giz_buffer, &world->unknown_0108, count, flags,
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

i32 CheckForPlatInst(i32 instance);


i32 CreateScaledEffect(i32 effect_index, f32 requested_scale);

    i32 CreateScaledPARTEffect(i32 effect_index, f32 requested_scale) {
        // Retail uses this inclusive upper check; loaded type IDs are 0..127.
        if (effect_index <= 0) {
            return -1;
        }
        if (effect_index > 128 || part_types[effect_index].effect_ids[0] == -1) {
            return -1;
        }
        if (part_types[effect_index].scale != 1.0f) {
            // The final word identifies the original, unscaled type.
            effect_index = part_types[effect_index].field_174;
            if (part_types[effect_index].effect_ids[0] == -1) {
                return -1;
            }
        }
        if (requested_scale == 1.0f && effect_index != 0) {
            return effect_index;
        }
        if (requested_scale < 0.01f) {
            requested_scale = 0.01f;
        }

        i32 closest_index = effect_index;
        f32 ratio = requested_scale / part_types[effect_index].scale;
        f32 closest_distance = ratio <= 1.0f ? 1.0f - ratio : ratio - 1.0f;
        for (i32 i = 0; i < 128; ++i) {
            if (__builtin_expect(part_types[i].effect_ids[0] != -1 && part_types[i].field_174 == static_cast<u32>(effect_index), 1)) {
                ratio = requested_scale / part_types[i].scale;
                f32 distance = ratio <= 1.0f ? 1.0f - ratio : ratio - 1.0f;
                if (__builtin_expect(distance < closest_distance, 1)) {
                    closest_index = i;
                    closest_distance = distance;
                }
            }
        }
        if (closest_index != 0 && closest_distance < 1.1f) {
            return closest_index;
        }

        for (i32 i = 0; i < 128; ++i) {
            if (part_types[i].effect_ids[0] != -1) {
                continue;
            }
            part_type_s *scaled = &part_types[i];
            part_type_s *source = &part_types[effect_index];
            *scaled = *source;
            edpartScaleType(i, requested_scale);
            scaled->last_used_time = partglobaltime;
            scaled->scale = requested_scale;
            scaled->field_174 = effect_index;
            char shortened[16];
            const char *base_name = source->name;
            const size_t name_length = strlen(base_name);
            if (__builtin_expect(name_length > 12, 1)) {
                memcpy(shortened, base_name, name_length + 1);
                shortened[12] = '\0';
                base_name = shortened;
            }
            sprintf(scaled->name, "%s%03d", base_name, i);
            ++part_types_used;
            return i != 0 ? i : closest_index;
        }
        return closest_index;
    }

void DebFreeAllCreatedEffects(void);

void DebFreeAllDMADebTablesInstantly(void);

void DebFreeAllPanelEffects(void);

void Debris(i32 update_panel_time);

i32 DebrisAllCollisionCheckScaleYFlag(NUVEC *position, f32 radius, f32 y_scale, u8 flags);

i32 DebrisCollisionCheck(NUVEC *position, f32 radius);

i32 DebrisCollisionCheckFlag(NUVEC *position, f32 radius, u8 flags);

i32 DebrisCollisionCheckScaleY(NUVEC *position, f32 radius, f32 y_scale);

i32 DebrisCollisionCheckScaleYFlag(NUVEC *position, f32 radius, f32 y_scale, u8 flags);

} // extern "C"

void DebrisTimeSlip(i32 group);


extern "C" {

void DebrisEmitterMomentum(i32 handle, f32 x, f32 y, f32 z);

void DebrisEmitterOrientation(i32 handle, i16 z, i16 y, i16 x);

void DebrisEmitterOrientationMtx(i32 handle, NUMTX *source);

void DebrisEmitterPos(i32 handle, f32 x, f32 y, f32 z);

i32 DebrisFindAllOfType(i32 effect_index, NUVEC *positions, i32 *handles, i32 capacity, i32 skip, i32 active_only);

i32 DebrisGetConeProperties(i32 handle, NUVEC *origin, NUVEC *end, f32 *start_radius, f32 *end_radius,
                                NUVEC *direction, f32 *length, f32 *speed);

f32 DebrisGetDuration(i32 effect_index);

char *DebrisGetName(i32 effect_index);

i32 DebrisGetParticleCount(i32 handle);

i32 DebrisGetRingProperties(i32 handle, NUVEC *origin, NUVEC *direction, f32 *radius);

void DebrisOrientation(i32 handle, i16 z, i16 y);

void DebrisOrientationMtx(i32 handle, NUMTX *source);

void DebrisParticleMomentum(i32 handle, f32 x, f32 y, f32 z);

void DebrisPopulateInstance(i32 handle, f32 duration);

void DebrisPosOrientationMtx(i32 handle, NUMTX *source);

i32 DebrisPreCheckCollisions(NUVEC *position, f32 radius);

void DebrisProcessTimeSlip(void);

i32 DebrisQueryPriority(i32 effect_index);

void DebrisReScale(i32 effect_index, f32 scale);

void DebrisReflectionOrientation(i32 handle, i16 x, i16 y, f32 plane, f32 scale);

extern NUVEC * CutoffCameraVec;

void DebrisRegisterCutoffCameraVec(void *position);

void DebrisReserveTrashableSpace(VARIPTR *buffer, VARIPTR, i32 chunk_count, i32 glass_chunk_count);

void DebrisResetTimers(void);

void DebrisSetDetailLevels(i32 handle, i32 detail_levels);

void DebrisSetDrawFlag(i32 handle, i8 draw_flag);

void DebrisSetFacing(i32 handle, u8 enabled, i16 x_angle, i16 y_angle);

void DebrisSetGroupID(i32 handle, i16 group);

void DebrisSetPriority(i32 handle, u16 priority, u8 automatic);

extern i32 debris_render_group;
void DebrisSetRenderGroup(i32 group);

void DebrisSetRoomID(i32 handle, nugscn_s *scene);

void DebrisSetTrigger(i32 handle, i32 first, i32 second, f32 third);

void DebrisSetUserData(i32 handle, void *user_data);

void DebrisShift(NUMTX *transform);

void DebrisStartOffset(i32 handle, f32 offset);

void DebrisStatusAlwaysOff(i32 *handle);

void DebrisStatusAlwaysOn(i32 *handle);

void DebrisStatusNormal(i32 *handle);

i32 DebrisTorusCollisionCheck(NUVEC *position, f32 radius);

i32 DebrisTorusCollisionCheckFlag(NUVEC *position, f32 radius, u8 flags);

i32 DebrisTorusCollisionCheckScaleY(NUVEC *position, f32 radius, f32 y_scale);

i32 DebrisTorusCollisionCheckScaleYFlag(NUVEC *position, f32 radius, f32 y_scale, u8 flags);

void DebrisTrashableSetup(VARIPTR *buffer, VARIPTR *);

void DebrisTypeStatusAlwaysOff(i32 type);

void DebrisTypeStatusAlwaysOn(i32 type);

void DebrisTypeStatusNormal(i32 type);

i32 DeletePlatinst(i32 index);

void DrawPlatform(i32 platform_index);

i32 FindPlatInst(i32 instance);

i32 NewMSituTerrEx(NUVEC *position, void *data, NUVEC *minimum, NUVEC *maximum, f32 radius);

i32 NewPlatInst(void *object, i32 instance);

i32 NewPlatInstMSitu(void *object, i32 source);

i32 NewRayCastEx(NUVEC *position, NUVEC *movement, f32 radius, i32 scan_flags);

i32 NewRayCastScaleY(NUVEC *position, NUVEC *movement, f32 radius, f32 scale_y, i32 scan_flags);

    f32 NewShadowEx(NUVEC *position, i32 handle, f32 height_above, f32 height_below, i32 terrain_mask);

f32 NewShadow(NUVEC *position, f32 height_above, f32 height_below, i32 terrain_mask);

    void NewTerrainScaleY(NUVEC *position, NUVEC *movement, u8 *hit_flags, i32 object_index, f32 radius,
                          f32 collision_radius, f32 object_scale, i32 embedded_retry, i32 scan_flags);

void NewTerrain(NUVEC *position, NUVEC *movement, u8 *hit_flags, i32 object_index, f32 radius, f32 collision_radius,
                    i32 scan_flags);

void NewTerrainScaleY(NUVEC *position, NUVEC *movement, u8 *hit_flags, i32 object_index, f32 radius,
                          f32 collision_radius, f32 object_scale, i32 embedded_retry, i32 scan_flags);

    void PartTerrInit(void) {
    }

void PlatInstBounce(i32 index, f32 impulse, f32 spring, f32 damping);

void PlatInstCenter(i32 index, NUVEC *position);

i32 PlatInstGetHit(i32 index);

void PlatInstRotate(i32 index, i32 rotate);

void PlatSkinMemRigister(void *start, void *end, i32 maximum);

void PlatSkinMemReset(void);

i32 PlatInstSkinRegisterEx(NUMTX *matrix, void *skin_data, void *matrix_data, f32 scale, i32 instance, i32 flags,
                               TERRSET *source);

i32 PlatInstSkinRegister(NUMTX *matrix, void *skin_data, void *matrix_data, f32 scale, i32 instance, i32 flags);

void PlatOnOff(i32 index, i32 enabled);

void *PlatSkinEndReigster(i32 cache_slots);

void PlatformRemoveCallback(void (*function)(void *));

void PlatformUpdateCallback(void (*function)(void *), void *argument);

void SortDebrisRenderStack(void);

const char *TerrErrorString(i32 error);

extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId();

i32 TerrainFreeId(void *id);

TERRSET *TerrainGetCur(void);

TERRAIN_GROUP *TerrainGetModelByInst(i32 instance);

void TerrainPlatGetMtx(i32 index, NUMTX **previous, NUMTX **current);

void UpdateDebrisRenderStackPriority(void);

void *terraininit(i32 level_num, void *buffer, void *buffer_end, i32 options, char *path, void *scene, i32 group);

void UpdatePlatinst(i32 index, void *object);

} // extern "C"
