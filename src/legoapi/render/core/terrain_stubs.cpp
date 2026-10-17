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
#include "legoapi/render/core/terrain_stubs_types.h"

void TerrainSkinAllocate(terrsitu_s *terrain_group);

void SkinPlatformSize(i32 group_index, unsigned char *buffer, PLATSKININFO *info);

void ScanTerrain(i32 scan_type, i32 terrain_mask, i32 scan_flags);
i32 HitTerrain(void);
void RayImpact(NUVEC *);

extern "C" {
    extern debinftype **debtab;
    void DebReAlloc(debkeydatatype_s *, i32);
    void DebFreeInstantly(i32 *);
    extern "C++" void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
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

void Debris(i32 update_panel_time);

} // extern "C"

extern "C" {

void DebrisPosOrientationMtx(i32 handle, NUMTX *source);

extern i32 debris_render_group;

void DebrisTypeStatusAlwaysOff(i32 type);

void DebrisTypeStatusAlwaysOn(i32 type);

void DebrisTypeStatusNormal(i32 type);

i32 DeletePlatinst(i32 index);

i32 FindPlatInst(i32 instance);

i32 NewPlatInst(void *object, i32 instance);

i32 NewRayCastEx(NUVEC *position, NUVEC *movement, f32 radius, i32 scan_flags);

i32 NewRayCastScaleY(NUVEC *position, NUVEC *movement, f32 radius, f32 scale_y, i32 scan_flags);

    f32 NewShadowEx(NUVEC *position, i32 handle, f32 height_above, f32 height_below, i32 terrain_mask);

f32 NewShadow(NUVEC *position, f32 height_above, f32 height_below, i32 terrain_mask);

void PlatInstRotate(i32 index, i32 rotate);

void PlatOnOff(i32 index, i32 enabled);

extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId();

TERRSET *TerrainGetCur(void);

void TerrainPlatGetMtx(i32 index, NUMTX **previous, NUMTX **current);

} // extern "C"
