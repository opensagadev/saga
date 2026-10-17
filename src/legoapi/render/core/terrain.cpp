#include "decomp.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/terrain_internal.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/ai/core/legoai.h"
#include "legoapi/render/core/gameliball.h"

#include <stdio.h>
#include <new>

#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edgra.h"
#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/nuplatform/nudevicespecs.hpp"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nuvec4.h"

#include <limits.h>
#include <math.h>
#include <string.h>

struct tertype;
struct terrsitu_s;
struct PLATSKININFO;
struct GAMECAMERA_s;
struct pushblock_s;
void Surface_Deflect(NUVEC *normal, NUVEC *movement, NUVEC *result, i32 mode);
extern i16 id_ATST;
extern i16 id_SPEEDERBIKE;
void GetSurfaceInfo(GameObject_s *object, i32 update_surface, f32 shadow_height);
f32 FindReflectionNoPlatforms(NUVEC *position);
extern i32 TimingBarSet;
void TBOPENFN(char *, i32);
void TBCLOSEFN(char *, i32);
i32 NOTERRAINSTOP;
extern GameObject_s *CarWashHack;
GameObject_s *CharPlatform_FindObjFromPlatID(CHARPLATFORMSYS_s *system, i32 platform_id);
void StartFlatten(GameObject_s *source, GameObject_s *target);
void AddWaterSplash(GameObject_s *object, NUVEC *position);
i32 CannotKill(GameObject_s *object);
extern AREADATA_s *PODSPRINT_ADATA;
extern AREADATA_s *PODRACE_ADATA;
void NewRumble(nupad_s *pad, f32 strength, i32 mode);
void NewBuzz(nupad_s *pad, f32 duration, i32 mode);
void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 delay, i32 mode);
i32 NoLayerKill(GameObject_s *object);
void ClearLastSafeTakeOver(GameObject_s *object);
void Player_ClearContext(GameObject_s *object, i32 mode);
void PlayDieSfx(GameObject_s *object);
i32 StartBigJump(GameObject_s *, NUVEC *, i32, f32, f32, i32, i8);
extern i16 id_SNAKE, id_TRACTOR;
extern AREADATA_s *DAGOBAH_ADATA;
extern f32 MiscTime;
void TakeOverCode(GameObject_s *object, i32 tag_pressed);
void ApplyGravity(GameObject_s *object, f32 *gravity, f32 hover_height, f32 seek_rate, f32 *ground_height);
void InstantKillParts(GameObject_s *object, i32 mode, f32 delay);
i32 IntersectWater(GameObject_s *object);
extern "C" void NewTerrPlatformsOff();

i32 testshadowfix = 1;

namespace {
#include "legoapi/render/core/terrain_types.h"
    DECOMP_ASSERT(sizeof(TERRAIN_SCENE_OBJECT) == 0x50, "TERRAIN_SCENE_OBJECT ABI");

    DECOMP_ASSERT(sizeof(TERRAIN_DISPLAY_OBJECT) == 0xd0, "TERRAIN_DISPLAY_OBJECT ABI");
    DECOMP_ASSERT(offsetof(TERRAIN_DISPLAY_OBJECT, transform) == 0xb8, "TERRAIN_DISPLAY_OBJECT transform offset");
    DECOMP_ASSERT(offsetof(TERRAIN_DISPLAY_OBJECT, scene_object_index) == 0xc0,
                  "TERRAIN_DISPLAY_OBJECT scene_object_index offset");

    DECOMP_ASSERT(offsetof(TERRAIN_DISPLAY_SCENE, object_count) == 0x6c, "TERRAIN_DISPLAY_SCENE object_count offset");

    DECOMP_ASSERT(offsetof(TERRAIN_GSCENE, object_count) == 0x1c, "TERRAIN_GSCENE object_count offset");
    DECOMP_ASSERT(offsetof(TERRAIN_GSCENE, object_map_present) == 0x58, "TERRAIN_GSCENE object_map_present offset");
    DECOMP_ASSERT(offsetof(TERRAIN_GSCENE, display_scene) == 0x110, "TERRAIN_GSCENE display_scene offset");

} // namespace

extern "C" void NuRndrLine3dDbg(f32, f32, f32, f32, f32, f32, i32);

extern "C" void SphereDrawEx(NUVEC *centre, f32 radius, f32 vertical_scale, i32 colour);

extern "C" void SphereDraw(NUVEC *centre, f32 radius, f32 vertical_scale);
extern NUVEC TerrWallNorm;

i16 InsidePolLines(f32 point_x, f32 point_y, f32 point_z, f32 edge_a_x, f32 edge_a_y, f32 edge_a_z, f32 edge_b_x,
                   f32 edge_b_y, f32 edge_b_z, NUVEC *normal);

void ScanWallSplineTerrain(i32 scan_type, i32 terrain_mask, i32 scan_flags);
void TerrainMoveImpactData();
void TerrainImpactNorm();
void Tag_Check(GameObject_s *object);
void BigJumpCode(GameObject_s *object);
f32 GameShadow(GameObject_s *object, NUVEC *position, f32 probe_height, i32 terrain_mask);
void GetSurfaceInfo(GameObject_s *object, i32 update_surface, f32 shadow_height);
extern "C" i32 NewShadowOnPlatform();
extern i32 LEGO_AIPATHCNX_BLOCKAGE;
extern i32 LEGO_AIPATHCNX_FULLTERRAIN;
extern i32 LEGO_AIPATHCNX_WALLSHUFFLE;

void StorePlatImpact();

extern "C" void PlatImpactInfo(NUVEC *normal, i32 *material0, i32 *material1);

namespace {



    static TerrainScanBounds TerrainHandleBounds(const NUVEC &position, const NUVEC &movement, f32 radius);


    static bool TerrainShapeOverlaps(const TerrainScanBounds &bounds, const TERRAIN_SHAPE &shape) {
        return bounds.max_x >= shape.min_x && shape.max_x > bounds.min_x && bounds.max_y >= shape.min_y &&
               shape.max_y > bounds.min_y && bounds.max_z >= shape.min_z && shape.max_z > bounds.min_z;
    }


    // Retail scan callers contain the scale-and-normal reconstruction directly.
#define TERRAIN_SCALE_SCAN_SHAPE(candidate_arg, group_arg, writer_arg)                                                 \
    do {                                                                                                               \
        TERRAIN_SHAPE *&scale_candidate = (candidate_arg);                                                             \
        const TERRAIN_GROUP &scale_group = (group_arg);                                                                \
        TerrainScanWriter *scale_writer = (writer_arg);                                                                \
        if (TerI->object_scale != 1.0f) {                                                                              \
            TERRAIN_SHAPE *source = scale_candidate;                                                                   \
            scale_candidate = &ScaleTerrain[scale_writer->scaled_shape_count];                                         \
            scale_candidate->material[0] = source->material[0];                                                        \
            scale_candidate->material[1] = source->material[1];                                                        \
            scale_candidate->flags = source->flags;                                                                    \
            scale_candidate->normal_flags = source->normal_flags;                                                      \
                                                                                                                       \
            for (i32 vector_index = 0; vector_index < 3; ++vector_index) {                                             \
                scale_candidate->vectors[vector_index].x = source->vectors[vector_index].x;                            \
                scale_candidate->vectors[vector_index].z = source->vectors[vector_index].z;                            \
                scale_candidate->vectors[vector_index].y =                                                             \
                    (source->vectors[vector_index].y + scale_group.origin.y) * TerI->inverse_object_scale -            \
                    scale_group.origin.y;                                                                              \
            }                                                                                                          \
                                                                                                                       \
            if (source->normals[1].y < 65535.0f) {                                                                     \
                scale_candidate->vectors[3].x = source->vectors[3].x;                                                  \
                scale_candidate->vectors[3].z = source->vectors[3].z;                                                  \
                scale_candidate->vectors[3].y =                                                                        \
                    (source->vectors[3].y + scale_group.origin.y) * TerI->inverse_object_scale - scale_group.origin.y; \
                                                                                                                       \
                const f32 length = NuFsqrt(source->normals[1].x * source->normals[1].x +                               \
                                           source->normals[1].y * source->normals[1].y * TerI->object_scale_sq +       \
                                           source->normals[1].z * source->normals[1].z);                               \
                const f32 inverse_length = length == 0.0f ? 0.0f : 1.0f / length;                                      \
                scale_candidate->normals[1].x = source->normals[1].x * inverse_length;                                 \
                scale_candidate->normals[1].y = source->normals[1].y * TerI->object_scale * inverse_length;            \
                scale_candidate->normals[1].z = source->normals[1].z * inverse_length;                                 \
            } else {                                                                                                   \
                scale_candidate->normals[1].y = 65536.0f;                                                              \
            }                                                                                                          \
                                                                                                                       \
            const f32 length = NuFsqrt(source->normals[0].x * source->normals[0].x +                                   \
                                       source->normals[0].y * source->normals[0].y * TerI->object_scale_sq +           \
                                       source->normals[0].z * source->normals[0].z);                                   \
            const f32 inverse_length = length == 0.0f ? 0.0f : 1.0f / length;                                          \
            scale_candidate->normals[0].x = source->normals[0].x * inverse_length;                                     \
            scale_candidate->normals[0].y = source->normals[0].y * TerI->object_scale * inverse_length;                \
            scale_candidate->normals[0].z = source->normals[0].z * inverse_length;                                     \
            ++scale_writer->scaled_shape_count;                                                                        \
        }                                                                                                              \
    } while (0)


    // Preserve the retail arithmetic ordering and the sweep-diagonal service
    // call, even in the wall-only caller where its result is unused.
#define TERRAIN_GET_SCAN_BOUNDS(bounds_arg, radius_arg, wall_arg)                                                      \
    do {                                                                                                               \
        TerrainScanBounds &scan_bounds = (bounds_arg);                                                                 \
        f32 &bounds_radius = (radius_arg);                                                                             \
        const TerrainQuery_s &scan_query = *TerI;                                                                      \
        if (scan_query.scan_result != 1) {                                                                             \
            const f32 move_length = NuFsqrt(scan_query.movement.x * scan_query.movement.x +                            \
                                            scan_query.movement.y * scan_query.movement.y +                            \
                                            scan_query.movement.z * scan_query.movement.z);                            \
            const TerrainQuery_s &sphere_query = *TerI;                                                                \
            const f32 reach = (wall_arg) ? 0.1f + sphere_query.collision_radius + move_length                          \
                                         : move_length + 0.1f + sphere_query.collision_radius;                         \
            const f32 vertical = reach * sphere_query.object_scale;                                                    \
            scan_bounds.max_x = sphere_query.position.x + 0.05f + reach;                                               \
            scan_bounds.max_z = sphere_query.position.z + 0.05f + reach;                                               \
            scan_bounds.min_x = sphere_query.position.x - 0.05f - reach;                                               \
            scan_bounds.min_z = sphere_query.position.z - 0.05f - reach;                                               \
            scan_bounds.min_y = sphere_query.position.y - 0.05f - vertical;                                            \
            scan_bounds.max_y = sphere_query.position.y + 0.05f + vertical;                                            \
            bounds_radius = reach;                                                                                     \
        } else {                                                                                                       \
            const f32 radius = scan_query.collision_radius;                                                            \
            if (!(scan_query.movement.x > 0.0f)) {                                                                     \
                scan_bounds.min_x = scan_query.position.x + scan_query.movement.x - 0.02f - radius;                    \
                scan_bounds.max_x = scan_query.position.x + 0.02f + radius;                                            \
            } else {                                                                                                   \
                scan_bounds.min_x = scan_query.position.x - 0.02f - radius;                                            \
                scan_bounds.max_x = scan_query.position.x + scan_query.movement.x + 0.02f + radius;                    \
            }                                                                                                          \
            if (!(scan_query.movement.y > 0.0f)) {                                                                     \
                scan_bounds.min_y = scan_query.position.y + scan_query.movement.y - 0.02f - radius;                    \
                scan_bounds.max_y = scan_query.position.y + 0.02f + radius;                                            \
            } else {                                                                                                   \
                scan_bounds.min_y = scan_query.position.y - 0.02f - radius;                                            \
                scan_bounds.max_y = scan_query.position.y + scan_query.movement.y + 0.02f + radius;                    \
            }                                                                                                          \
            if (!(scan_query.movement.z > 0.0f)) {                                                                     \
                scan_bounds.min_z = scan_query.position.z + scan_query.movement.z - 0.02f - radius;                    \
                scan_bounds.max_z = scan_query.position.z + 0.02f + radius;                                            \
            } else {                                                                                                   \
                scan_bounds.min_z = scan_query.position.z - 0.02f - radius;                                            \
                scan_bounds.max_z = scan_query.position.z + scan_query.movement.z + 0.02f + radius;                    \
            }                                                                                                          \
            const f32 dx = scan_bounds.min_x - scan_bounds.max_x;                                                      \
            const f32 dy = scan_bounds.min_y - scan_bounds.max_y;                                                      \
            const f32 dz = scan_bounds.min_z - scan_bounds.max_z;                                                      \
            bounds_radius = NuFsqrt(dx * dx + dy * dy + dz * dz);                                                      \
        }                                                                                                              \
        if (!(wall_arg) && scan_query.object_scale > 1.0f)                                                             \
            bounds_radius += (scan_query.object_scale - 1.0f) * scan_query.collision_radius;                           \
    } while (0)

    // These wall-spline traversals are part of all three retail scan bodies.
#define TERRAIN_COLLECT_WALL_SPLINES(bounds_arg, mask_arg, clear_arg, reset_empty_arg)                                 \
    do {                                                                                                               \
        TerrainScanBounds wall_bounds = (bounds_arg);                                                                  \
        const i32 wall_mask = (mask_arg);                                                                              \
        const bool wall_clear_override = (clear_arg);                                                                  \
        wall_bounds.min_x -= 0.02f;                                                                                    \
        wall_bounds.min_z -= 0.02f;                                                                                    \
        wall_bounds.max_x += 0.02f;                                                                                    \
        wall_bounds.max_z += 0.02f;                                                                                    \
        if ((reset_empty_arg) || (CurTerr->spatial_nodes != NULL && WallSplCount < 64))                                \
            WallSplCount = 0;                                                                                          \
        for (TERRAIN_SPATIAL_NODE *node = CurTerr->spatial_nodes; node != NULL;                                        \
             node = *reinterpret_cast<TERRAIN_SPATIAL_NODE **>(reinterpret_cast<u8 *>(node) - sizeof(void *))) {       \
            const u8 mask = node->field_0x02 >> 8;                                                                     \
            if (mask != 0 && (mask & wall_mask) == 0)                                                                  \
                continue;                                                                                              \
            for (i32 first = 0; first < node->point_count; first += 16) {                                              \
                NUVEC *points = node->points + first;                                                                  \
                i32 end = node->point_count;                                                                           \
                if (points[0].y != 2147483648.0f) {                                                                    \
                    if (!(points[1].y >= wall_bounds.min_x && wall_bounds.max_x >= points[0].y &&                      \
                          points[3].y >= wall_bounds.min_z && wall_bounds.max_z >= points[2].y))                       \
                        continue;                                                                                      \
                    end = MIN(end, first + 16);                                                                        \
                }                                                                                                      \
                for (i32 i = first; i < end; ++i) {                                                                    \
                    NUVEC &a = node->points[i];                                                                        \
                    NUVEC &b = node->points[i + 1];                                                                    \
                    if (!((a.x >= wall_bounds.min_x && wall_bounds.max_x >= b.x) ||                                    \
                          (b.x >= wall_bounds.min_x && wall_bounds.max_x >= a.x)))                                     \
                        continue;                                                                                      \
                    if (!((a.z >= wall_bounds.min_z && wall_bounds.max_z >= b.z) ||                                    \
                          (b.z >= wall_bounds.min_z && wall_bounds.max_z >= a.z)))                                     \
                        continue;                                                                                      \
                    if (WallSplCount < 64) {                                                                           \
                        WallSplList[WallSplCount].position = a;                                                        \
                        WallSplList[WallSplCount + 1].position = b;                                                    \
                        for (i32 p = 0; p < 2; ++p) {                                                                  \
                            u8 *material = WallSplList[WallSplCount + p].material;                                     \
                            material[0] = static_cast<u8>(node->field_0x02);                                           \
                            material[1] = mask;                                                                        \
                            material[2] = 0;                                                                           \
                            material[3] = 0;                                                                           \
                        }                                                                                              \
                        WallSplCount += 2;                                                                             \
                    }                                                                                                  \
                }                                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
        if (wall_clear_override)                                                                                       \
            TerrOverRideScan = NULL;                                                                                   \
    } while (0)

} // namespace

extern i16 debug_index;

// Original 400-pointer table at 0x61e860: 147 named effects, then empty slots.
char *debris_name[400] = {
    "BULLET_SPARK",   "SABER_RED",     "SABER_GREEN",   "SABER_BLUE",    "SABER_PURPLE",   "EXPLD_1A",
    "JS_THRUST",      "S1_THRUST",     "COIN_BLUE",     "COIN_GOLD",     "COIN_SILVER",    "R2D2THRUSTER",
    "R2Q5THRUSTER",   "SPLASH",        "RIPPLE",        "HEART",         "HEART_BIG",      "EXPLOSION",
    "MINI_01",        "MINI_02",       "MINI_03",       "TER_SPARK",     "TER_SPARK2",     "EXPOL_02",
    "SPEEDERDUST",    "POD_DUST",      "CAVE_DUST",     "JANGOTHRUSTER", "EXPLO_DROID",    "EXPLO_NAB",
    "NAB_SMOKE",      "NAB_BOOST",     "MIKESMOKE",     "EXPLO_05",      "EXPLO_06",       "EXPLO_07",
    "EXPLO_11",       "MIKESTALL",     "TRAINING_01",   "TAG_BLUE",      "TAG_GREEN",      "ZIP_TARGET",
    "DOOKU_BALL",     "WALKER_01",     "WALKER_02",     "ENG_BLOW",      "DogImpact",      "DogEngine",
    "DogSmoke",       "DogTrail",      "YODA_HOVER_01", "K_Bolt",        "DUK_P2",         "PodHaze",
    "PodDust10",      "rock_pop1",     "BlueTrail",     "PurpleTrail",   "LavaDie",        "ComboRed",
    "ComboGreen",     "ComboBlue",     "ComboPurple",   "REPAIR",        "SABER_RED1",     "SABER_GREEN1",
    "SABER_BLUE1",    "SABER_PURPLE1", "XWING_1",       "MOUSE_POP",     "PHOTON",         "PHOTON_EXPLO",
    "PHOTON_EXPLO_S", "EXPLO_ORAN_2A", "EXPLO_ORAN_2B", "EXPLO_ORAN_2C", "BUILD_IT1",      "BUILD_IT2",
    "BUILD_IT3",      "BUILD_IT4",     "BUILD_IT5",     "BUILD_IT6",     "DRAG_POP_1",     "DRAG_POP_2",
    "DRAG_POP_4",     "GenoGun",       "SNOW_SPEEDER1", "LEVER_SPARK",   "ZAPPER_1",       "V_BOLT_RED",
    "V_BOLT_GREEN",   "CHAR_SMOKE1",   "CHAR_SMOKE2",   "POWER_P_1",     "POWER_P_2",      "POWER_P_3",
    "TIE_HIT1",       "BRICK_01",      "BRICK_02",      "BRICK_03",      "BUMP_01",        "BUMP_02",
    "LAND_S_DUST1",   "LAND_S_DUST2",  "LAND_S_THRUST", "LAND_S_HAZE",   "EXPLO_06",       "EXPLO_06b",
    "BUMP_03",        "FORCE_BLUE",    "FORCE_RED",     "SCAN3",         "REPULSOR",       "WEE_POP",
    "EXIT_FIRE_1",    "EXIT_FIRE_2",   "EWOK_POP1",     "EXPLO_02",      "ATAT_POP_1",     "ATAT_POP_2",
    "ATAT_POP_3",     "FIRE_1",        "RAIN_1",        "CHAR_BUBBLE",   "CHAR_GHOST",     "DEATH_1",
    "DEATH_2",        "BOULDER_2",     "DISH_1",        "DISH_2",        "SNOW_POP_1",     "SNOW_POP_2",
    "TREE_POP_1",     "TREE_POP_4",    "SPEEDER_HIT",   "FALCONGLOW",    "FALCONTHRUSTER", "SPEED_SPARK",
    "PUNCH_1",        "PUNCH_2",       "GUNSHIP_01",    "GUNSHIP_02",    "SandBlast",      "BULLET1",
    "BULLET2",        "BULLET_HIT",    "POD_SPARK",
};
extern "C" {
i32 Grass_Available;
}

extern "C" void TerrainSetImpactData(void *impact_data, i32 *impact_count, i32 maximum_impacts);

void TerrFlush();

extern "C" void TerrTempMemory(void **buffer);

// Constructs the fixed terrain header and its variable-sized group, index and
// platform arrays in the caller's arena. The four bounds arrays and the cell
// assignment array are temporary storage carved backwards from buf_end.
#define IS_SNAN(x) (*(u32 *)&(x) == 0xffc00000)

void NuVecCheckForSNANs(NUVEC *v);

#undef IS_SNAN

extern "C" void *TerrainInitEx(i32 level_num, void *buf, void *buf_end, i32 options, char *path, void *gscn,
                               i32 scene_id, u32 max_group_indices, u32 max_groups, u32 max_platforms);
void LoadTerrainFile(WORLDINFO *world) {
    char path[256];
    world->terrain = NULL;
    if ((world->current_level->flags & LEVEL_TERRAIN) != 0) {
        NuStrCpy(path, world->config_file);
        LEVELDATA *level = world->current_level;
        if (level == (LEVELDATA *)PLATFORM_LDATA) {
            NuStrCpy(path, "levels\\episode_v\\cloudcityescape\\cloudcityescape_b\\cloudcityescape_b");
            level = world->current_level;
        }
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
        void *terrain = TerrainInitEx(world->level_idx, &world->giz_buffer, world->unknown_0108.void_ptr, 0, path,
                                      world->current_gscn, 0, (u32)(u16)level->max_ter_groups,
                                      (u32)(u16)level->max_ter_groups, (u32)(u16)level->max_ter_platforms);
        world->terrain = terrain;
    }
}
void LoadGrassFile(WORLDINFO *world) {
    char path[256];
    world->page_grass = -1;
    Grass_Available = 1;
    char *config = world->config_file;

    if (g_isLowEndDevice != 0) {
        char *found = NuStrIStr(config, "Gungan");
        if (found == NULL)
            found = NuStrIStr(config, "SpeederChase");
        if (found == NULL)
            found = NuStrIStr(config, "EndorBattle");
        if (found == NULL)
            found = NuStrIStr(config, "Retake");

        if (found != NULL) {
            Grass_Available = 0;
        } else {
            Grass_Available = 1;
        }
    }

    if (NuStrIStr(config, "SpeederChase") != NULL) {
        Grass_Available = 0;
    }

    if (NuDeviceSpecs::ms_instance->specs < 3 && NuStrIStr(config, "GunGan_A") != NULL) {
        Grass_Available = 0;
        return;
    }

    if (__builtin_expect(Grass_Available != 0, 1)) {
        sprintf(path, "%s.gra", config);
        if (NuFileExists(path)) {
            i32 page = edgraLoadPage(path, world->current_gscn, *(i32 *)&world->terrain, &world->giz_buffer,
                                     &world->unknown_0108);
            world->page_grass = page;
        }
    }
}
void LoadBridgeFile(WORLDINFO *world) {
    char path[256];
    world->page_bridge = -1;
    sprintf(path, "%s.bri", world->config_file);
    if (NuFileExists(path)) {
        i32 page = edbriLoadPage(path, world->current_gscn);
        world->page_bridge = page;
    }
}
TERRAIN_TRACK_SLOT *ScanTerrId(void *id);
i32 UnderWater(GameObject_s *object) {
    if ((object->apiobj.field_0x27f & static_cast<u8>(~8u)) == 1) {
        return object->apiobj.water_height >= object->apiobj.pos_y;
    }
    return 0;
}
i32 CheckCylinder(i32 first_vertex, i32 second_vertex, i32 *vertex_mask, i32 remaining_vertex_mask);

i32 CheckSphere(i32 vertex_index);

i32 CheckSphereTer(NUVEC *position, f32 radius);

i32 HitPoly(f32 primary_start, f32 primary_end, f32 secondary_start, f32 secondary_end, tertype *surface);

void ScanTerrain(i32 scan_type, i32 terrain_mask, i32 scan_flags);
#undef TERRAIN_SCALE_SCAN_SHAPE

NUVEC TerrainSkin(PLATSKININFO *info, nuvec_s *position, float weight, i32 mode);
void DerotateMovementVector();

void RotateVec(NUVEC *source, NUVEC *destination);

void RotateTerrain(tertype *surface);

void DeRotateTerrain(tertype *surface);

void DeRotatePoint(nuvec_s *point);

namespace {

    DECOMP_ASSERT(sizeof(TERRAIN_IMPACT_RECORD) == 0x1c, "TERRAIN_IMPACT_RECORD ABI");


} // namespace

void TerrainImpact(NUVEC *position, NUVEC *movement, u8 *hit_flags);

// Retail only keeps the IPA-SRA clone, which takes &normal->y.
#if defined(__i386__)
#define TERRAIN_KILL_PLAYER_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define TERRAIN_KILL_PLAYER_ATTR __attribute__((noinline, noclone))
#endif
static TERRAIN_KILL_PLAYER_ATTR i32 TerrainKillPlayer(GameObject_s *object, i32 surface, const f32 *normal_y)
    __asm__("_ZL17TerrainKillPlayerP12GameObject_siP7nuvec_s.isra.4");
static i32 TerrainKillPlayer(GameObject_s *object, i32 surface, const f32 *normal_y) {
    const u32 flags = TerSurface[surface].flags;
    if ((flags & 1) != 0 ||
        ((flags & 0x4000) != 0 && (object->apiobj.character_data->model_flags & 0x10) == 0 &&
         (object->apiobj.character_data->game_character->flags_090 & 0x8000) == 0) ||
        ((flags & 0x8000) != 0 && (object->apiobj.field_0x27c == -1 || *normal_y > 0.0f))) {
        InstantKillParts(object, 1, 0.0f);
        if (object->apiobj.player_controlled && BonusWinner == -1) {
            const i32 coins = LoseCoins(object, 1);
            AddPickups(coins, 0, 0, 0, &object->apiobj.collision_position, NULL, 2.0f, -1, 1.0f, 2000000.0f, object, 1,
                       0, false);
        }
        KillPlayer(object, 2, 1, NULL);
        return 1;
    }
    return 0;
}

void TerrainPlayer(GameObject_s *object) {
    NUVEC next_position;
    GAMEPAD_s *const entry_pad = object->pad_gamepad;
    const u8 entry_is_underwater = object->apiobj.is_underwater;
    const u8 entry_intersects_water = object->apiobj.intersects_water;
    const i32 entry_menu_id = GetMenuID();
    const i16 entry_platform = object->apiobj.supporting_platform_id;

    i32 impact_count = -1;
    // Original 0x102720 skips integration and callbacks for inactive objects.
    if (object->apiobj.field_0x287 == 0 && object->field_0xcc0 != NULL && object->character_context == 0x3b) {
        TakeOverCode(object, object->pad_gamepad->buttons_pressed & GAMEPAD_TAG);
    } else if (object->apiobj.field_0x287 == 0) {
        TERRAIN_IMPACT_RECORD impact_records[8];

        APIOBJECT &api = object->apiobj;
        i32 terrain_mask = TERRAINMASK_NONWEAPON;
        if ((api.character_data->model_flags & 0x10) == 0 &&
            (api.character_data->game_character->flags_090 & 0x8000) == 0) {
            terrain_mask |= TERRAINMASK_NONDROID;
        }
        if ((api.field_0x1f4 & APIOBJECT_MOTION_FLAG_AI_CONTROLLED) != 0 && object->id != id_DRAGBOMB &&
            (WORLD->area != HOTHBATTLE_ADATA || api.character_data->game_character->field_0x28 != 0.0f)) {
            terrain_mask |= 0x40;
        }
        object->field_0xe20 &= static_cast<u8>(~8u);
        const f32 entry_vertical_velocity = api.velocity.y;
        const AIPATHINFO &path_info = object->ai.path_info;
        i32 special_path_endpoints = false;
        if (path_info.path != NULL && path_info.connection != NULL) {
            const AIPATHCNX *path_connection = path_info.connection;
            const AIPATHNODE *nodes = path_info.path->nodes;
            special_path_endpoints = ((nodes[path_connection->node_indices[path_info.direction]].runtime_flags |
                                       nodes[path_connection->node_indices[path_info.direction == 0]].runtime_flags) &
                                      0x82) != 0;
        }
        i32 shadow_grounding = false;
        i32 retain_floor = false;
        if (object == CarWashHack) {
            shadow_grounding = true;
        } else if ((api.character_data->game_character->flags_090 & 0x80) != 0) {
            api.velocity.z = 0.0f;
            api.velocity.x = 0.0f;
            shadow_grounding = true;
        } else if ((api.field_0x1f8 & 0x40) == 0 && (CInfo[object->character_context].flags & 0x8000) == 0) {
            const bool ai_controlled = (api.field_0x1f4 & APIOBJECT_MOTION_FLAG_AI_CONTROLLED) != 0;
            const bool ordinary_ai_path =
                ai_controlled && object->movement_spline == NULL && object->character_context != 0x1c &&
                object->context_target_position == NULL && !special_path_endpoints &&
                (path_info.flags & (AIPATHINFO_FLAG_ON_PATH | AIPATHINFO_FLAG_NARROW_PATH)) ==
                    AIPATHINFO_FLAG_ON_PATH &&
                Technos_FindControllingTechno(object) == NULL && (api.field_0x1fa & 4) == 0 &&
                (path_info.connection == NULL ||
                 ((path_info.connection->original_traversal_flags[0] & static_cast<u32>(LEGO_AIPATHCNX_BLOCKAGE)) ==
                      0 &&
                  (path_info.connection->traversal_flags[0] & static_cast<u32>(LEGO_AIPATHCNX_WALLSHUFFLE)) == 0 &&
                  (path_info.connection->traversal_flags[0] & static_cast<u32>(LEGO_AIPATHCNX_FULLTERRAIN)) == 0)) &&
                (object->character_context != 0x5a || object->field_0x7a3 != 0);
            if (ordinary_ai_path) {
                shadow_grounding = true;
                retain_floor = (api.field_0x1f8 & 0x10) != 0;
            } else {
                retain_floor = !ai_controlled && (api.field_0x1f8 & 0x10) != 0;
                if ((object->field_0xf00 & 0x40) != 0) {
                    object->field_0xf00 &= static_cast<u8>(~0x40u);
                    shadow_grounding = true;
                } else {
                    shadow_grounding = (api.field_0x1f8 & 8) != 0;
                }
            }
        }

        // TerrainPlayer selects either the inexpensive shadow-grounding path used
        // by ordinary AI or the swept capsule resolver required by players and
        // special traversal connections, then refreshes floor/contact state before
        // dispatching the character movement callback.
        const f32 lower_bound = object->character_bottom * api.field_0xa8;
        // Original 0x102856..0x10316f consumes the previous edge-stop request
        // and probes the next horizontal position before integrating movement.
        const bool player_edge_probe = static_cast<i8>(api.flags_low) < 0 && object->spawn_protection_timer > 1.25f;
        const bool requested_edge_stop = static_cast<i8>(object->edge_stop_requests) > 0;
        object->field_0xf04 &= static_cast<u8>(~0x40u);
        object->edge_stop_requests = 0;
        if (VehicleArea == 0 && api.field_0x287 == 0 &&
            (api.field_0x27d != 0 || requested_edge_stop ||
             (player_edge_probe && object->character_context == -1 && object->field_0xe31 != 1))) {
            const bool walkable_floor = api.field_0x218 != 2000000.0f && object->surface_normal.y > NuTrigTable[0x238e];
            const bool probe_edge = walkable_floor ? (player_edge_probe || requested_edge_stop ||
                                                      (CInfo[object->character_context].parameter & 1) != 0)
                                                   : requested_edge_stop;
            if (probe_edge && api.movement_direction.x == 0.0f && api.movement_direction.z == 0.0f &&
                (api.velocity.x != 0.0f || api.velocity.z != 0.0f)) {

                next_position.x = api.position.x + api.velocity.x * FRAMETIME;
                next_position.z = api.position.z + api.velocity.z * FRAMETIME;
                next_position.y = api.position.y;
                const f32 next_floor = GameShadow(object, &next_position, 5.0f, terrain_mask | 0x1f);
                if (next_floor != 2000000.0f && ShadNorm.y > NuTrigTable[0x238e] &&
                    (player_edge_probe ? api.collision_min.y : api.field_0x218) - next_floor > 0.2f) {
                    object->field_0xf04 |= 0x40;
                    api.velocity.x = 0.0f;
                    api.velocity.z = 0.0f;
                }
            }
        }
        f32 movement_threshold = 0.0f;
        if (!(object->pad_gamepad->input_magnitude > 0.0f) && object->character_context != 0x33) {
            const i32 surface = static_cast<i8>(api.field_0x281);
            const bool slippery =
                api.field_0x27d != 0 && surface >= 0 && surface <= 31 && 1.0f > TerSurface[surface].movement_scale;
            if (!slippery && AnimSpeed(api.character_model, CurrentAnim(&api.anim_packet)) == 0.0f &&
                (api.character_data->model_flags & 0x800) == 0) {
                movement_threshold = api.character_data->game_character->field_0x0c * api.field_0xa8;
            }
        }

        const f32 speed_squared =
            (api.velocity.x * api.velocity.x + api.velocity.y * api.velocity.y) + api.velocity.z * api.velocity.z;
        const bool perform_motion =
            !((object->field_0xefc & 0x20) == 0 && (object->field_0xf02 & 0x40) != 0 && !special_path_endpoints &&
              object->ai.field_0x180 == NULL && static_cast<i8>(api.flags_low) >= 0 &&
              (object->field_0xe23 & 0x10) == 0 && object->field_0xe31 == 0 && object->character_context != 0 &&
              object->character_context != 0x1c && api.field_0x281 != 8 &&
              (static_cast<u32>(GameTimer.update_count) > 1 || (api.field_0x1f4 & 5) != 0) &&
              (api.field_0x27d & 3) != 0 && api.supporting_platform_id == -1 &&
              movement_threshold * movement_threshold >= speed_squared && object->character_context != 0x0f &&
              object->character_context != 0x0b && object->character_context != 0x1e);
        api.field_0x1f8 = (api.field_0x1f8 & ~4u) | (perform_motion ? 0u : 4u);
        object->field_0x1084 = 0;
        const u8 integration_flags = object->field_0xe20;
        // Original 0x102952/0x102bfb carries this byte through the call-free
        // integration selection before setting its terrain-update bit.
        const i32 previous_platform = entry_platform;
        const bool direct_integration = (api.flags_low & 0x20) != 0 || (integration_flags & 0x20) != 0 ||
                                        object->movement_spline != NULL || object->move_override != NULL ||
                                        (object->character_context == 0x0f && object->field_0x7a3 <= 1) ||
                                        (object->character_context == 0x2c && object->field_0x7a3 == 0) ||
                                        (object->character_context == 0x47 && object->action_movement_state == 1);
        if (direct_integration) {
            // Original 0x102940/0x102bf8 selects integration without a terrain
            // query for these motion owners and action states.
            api.respawn_timer = 0.0f;
            object->field_0xe20 = integration_flags | 2;
            if (TimingBarSet == 2)
                TBOPENFN("Ter", 2);
            api.field_0x27d = 0;
            api.supporting_platform_id = -1;
            object->field_0x6b0 = 0;
            api.position.x += api.velocity.x * FRAMETIME;
            api.position.y += api.velocity.y * FRAMETIME;
            api.position.z += api.velocity.z * FRAMETIME;
            if (TimingBarSet == 2)
                TBCLOSEFN("Ter", 2);
        } else if (shadow_grounding || retain_floor || (WORLD->current_level->flags & 0x10) != 0) {
            // Ordinary path-following AI uses the target's inexpensive shadow
            // grounding path.  Full swept collision is reserved for path
            // connections whose traversal flags require special collision.
            api.respawn_timer = 0.0f;
            if (perform_motion) {
                if (TimingBarSet == 2)
                    TBOPENFN("Ter", 2);
                object->field_0xe20 |= 2;
                api.supporting_platform_id = -1;
                api.position.x += api.velocity.x * FRAMETIME;
                api.position.y += api.velocity.y * FRAMETIME;
                api.position.z += api.velocity.z * FRAMETIME;

                if (shadow_grounding) {
                    if (object == CarWashHack) {
                        NewTerrPlatformsOff();
                    }
                    if (testshadowfix == 0) {
                        api.field_0x218 = GameShadow(object, &api.collision_position, 5.0f, terrain_mask | 0x1f);
                    } else {
                        NUVEC shadow_position = api.collision_position;
                        if (api.initial_position.y > api.collision_position.y) {
                            shadow_position.y = api.initial_position.y;
                        } else {
                            shadow_position.y = api.collision_position.y;
                        }
                        api.field_0x218 = GameShadow(object, &shadow_position, 5.0f, terrain_mask | 0x1f);
                    }
                    api.supporting_platform_id = static_cast<i16>(NewShadowOnPlatform());
                    GetSurfaceInfo(object, api.field_0x218 != 2000000.0f ? 1 : 0, api.field_0x218);
                    if (api.field_0x218 == 2000000.0f) {
                        api.field_0x281 = 0;
                        object->field_0xe41 = 0;
                        object->surface_normal = NUVEC{0.0f, 1.0f, 0.0f};
                        api.water_height = 2000000.0f;
                        api.field_0x220 = 2000000.0f;
                        api.field_0x27f = static_cast<u8>(-1);
                        api.field_0x280 = static_cast<u8>(-1);
                        object->field_0x1078 = -1;
                    }
                    api.is_underwater = static_cast<u8>(UnderWater(object));
                    api.intersects_water = static_cast<u8>(IntersectWater(object));
                    if (api.supporting_platform_id == -1 && api.field_0x27d != 0 && object->field_0x1078 != -1) {
                        api.supporting_platform_id = object->field_0x1078;
                    }
                }
                const f32 grounded_lower_bound = object->character_bottom * api.field_0xa8;
                if (api.field_0x218 != 2000000.0f && api.position.y + grounded_lower_bound < api.field_0x218) {
                    api.velocity.y = 0.0f;
                    api.position.y = api.field_0x218 - grounded_lower_bound;
                }

                api.field_0x27d = 0;
                if (GameObjectNearFloor(object, 1.0f, NULL) != 0) {
                    api.field_0x27d |= APIOBJECT_TERRAIN_CONTACT_NEAR_FLOOR;
                }
                if (api.collision_min.y <= api.field_0x218) {
                    api.field_0x27d |= APIOBJECT_TERRAIN_CONTACT_FLOOR;
                }
                if ((api.field_0x27d & APIOBJECT_TERRAIN_CONTACT_FLOOR) != 0) {
                    object->field_0x1084 = 1;
                    object->contact_position.x = api.position.x;
                    object->contact_position.y = api.field_0x218;
                    object->contact_position.z = api.position.z;
                    object->contact_normal = shadow_grounding ? object->surface_normal : NUVEC{0.0f, 1.0f, 0.0f};
                    object->field_0x6b0 = api.field_0x281;
                    object->terrain_impact_speed = -api.velocity.y;
                } else {
                    object->field_0x6b0 = 0;
                }
                if (TimingBarSet == 2)
                    TBCLOSEFN("Ter", 2);
            }
        } else {
            if (!perform_motion) {
                api.velocity.x = 0.0f;
                api.velocity.y = 0.0f;
                api.velocity.z = 0.0f;
            } else {
                // UpdateGameObjects supplies objects from the initialized Obj
                // array. Original 0x104830 computes its unsigned index before
                // invoking any full-collision callbacks.
                const i32 object_index =
                    static_cast<u32>(reinterpret_cast<uintptr_t>(object) - reinterpret_cast<uintptr_t>(Obj)) /
                    sizeof(GameObject_s);
                NUVEC collision_position = api.position;
                collision_position.y += lower_bound;

                NUVEC movement;
                NUVEC incoming_velocity = api.velocity;
                NuVecScale(&movement, &api.velocity, FRAMETIME);

                // Original 0x1048e9 clears the complete contact metadata word.
                object->field_0x6b0 = 0;
                memset(object->pad_6b1, 0, sizeof(object->pad_6b1));
                if (TimingBarSet == 2)
                    TBOPENFN("Ter", 2);
                // Original 0x1048fc..0x10497c and 0x10545f..0x105544.
                if (object->use_model_origin != 0 && api.field_0x27d != 0 &&
                    (movement.x != 0.0f || movement.z != 0.0f) &&
                    !(WORLD->current_level == ENDORBATTLEC_LDATA && object->id == id_ATST)) {
                    i32 deflection_mode = 0;
                    if (VehicleArea != 0 && (api.character_data->model_flags & 0x2000) != 0 &&
                        api.character_data->game_character->field_0x28 != 0.0f) {
                        deflection_mode = 2;
                    } else if (object->character_context == -1 && object->pad_gamepad->input_magnitude > 0.0f) {
                        deflection_mode = 1;
                    }
                    if (deflection_mode != 0) {
                        NUVEC deflected;
                        Surface_Deflect(&object->surface_normal, &movement, &deflected, deflection_mode);
                        if (deflected.y > movement.y) {
                            movement = deflected;
                            if (deflection_mode == 1) {
                                object->field_0xe20 |= 8;
                            }
                        }
                    }
                }
                TerrainSetPlatConnectTol((api.character_data->model_flags & 0x800) != 0 ? 0.0f : 0.01f);
                if ((object->ai.field_0x1e4 & 0x80) != 0) {
                    IgnoreWallSplines = 1;
                }
                TerrainSetImpactData(impact_records, &impact_count, 8);
                f32 movement_scale = 1.0f;
                if ((object->field_0xefd & 4) != 0) {
                    GAMECHARACTERDATA *character = api.character_data->game_character;
                    if (object->id == id_SPEEDERBIKE) {
                        if (object->ai.goal_speed_mode == 1) {
                            movement_scale = 15.0f / character->walk_speed;
                        } else if (object->ai.goal_speed_mode == 2) {
                            movement_scale = 15.0f / character->tiptoe_speed;
                        } else {
                            movement_scale = 15.0f / character->run_speed;
                        }
                    } else {
                        movement_scale = 2.5f / character->run_speed;
                    }
                    movement.x *= movement_scale;
                    movement.z *= movement_scale;
                }

                // Original 0x104a8f..0x104bdb excludes owned and selected character
                // platforms for this query, then re-enables the same list.
                i32 excluded_platforms[32];
                i32 excluded_count = 0;
                if (object->field_0x107a != -1) {
                    excluded_platforms[excluded_count++] = object->field_0x107a;
                }
                if (object->field_0x107c != -1) {
                    excluded_platforms[excluded_count++] = object->field_0x107c;
                }
                if (object->character_context != 0) {
                    if (object->character_context == 0x58 && object->field_0x7a3 == 1) {
                        GIZMOBLOWUP_s *blowup = static_cast<GIZMOBLOWUP_s *>(object->field_0x788);
                        if (blowup->platform_id != -1) {
                            excluded_platforms[excluded_count++] = blowup->platform_id;
                        }
                    }
                } else {
                    GameObject_s *target = object->airborne_collision_target;
                    if (target != NULL && target->field_0x107c != -1) {
                        excluded_platforms[excluded_count++] = target->field_0x107c;
                    }
                }
                CHARPLATFORMSYS_s *platforms = WORLD->char_platform_sys;
                if (platforms != NULL && VehicleArea == 0) {
                    const bool player = static_cast<i8>(api.flags_low) < 0;
                    if (!player || object->character_context == 0x3d || object->field_0xcc0 != NULL ||
                        object->character_context == 0x3b ||
                        (api.character_data->game_character->flags_090 & 0x8040) != 0) {
                        for (i32 i = 0; i < platforms->platform_count; ++i) {
                            GameObject_s *platform_object = platforms->platforms[i].object;
                            if (platform_object != NULL && ((object->field_0xf02 & 8) == 0 || player)) {
                                excluded_platforms[excluded_count++] = platform_object->field_0x107c;
                            }
                        }
                    }
                }
                for (i32 i = 0; i < excluded_count; ++i) {
                    PlatOnOff(excluded_platforms[i], 0);
                }

                NewTerrainScaleYMask(&collision_position, &movement, reinterpret_cast<u8 *>(&object->field_0x105c),
                                     object_index,
                                     entry_pad->input_magnitude > 0.0f ? 0.0f : movement_threshold * FRAMETIME,
                                     api.collision_radius, object->collision_y_scale, 0, 0, terrain_mask);
                IgnoreWallSplines = 0;
                for (i32 i = 0; i < excluded_count; ++i) {
                    PlatOnOff(excluded_platforms[i], 1);
                }
                if ((object->field_0xefd & 4) != 0) {
                    movement.x /= movement_scale;
                    movement.z /= movement_scale;
                }
                extern i32 TERRAINCALLS;
                ++TERRAINCALLS;
                if (TimingBarSet == 2)
                    TBCLOSEFN("Ter", 2);
                api.supporting_platform_id = static_cast<i16>(NewShadowOnPlatform());

                object->field_0x1084 = static_cast<u8>(TerrImpact);
                if (object->field_0x1084 != 0 && impact_count > 0) {
                    // Original copies the last complete 0x1c-byte impact record,
                    // including material and flags, before deriving impact speed.
                    memcpy(&object->contact_position, &impact_records[impact_count - 1], sizeof(TERRAIN_IMPACT_RECORD));
                    object->terrain_impact_speed = -NuVecDot(&object->contact_normal, &incoming_velocity);
                } else {
                    object->field_0x1084 = 0;
                }
                NuVecScale(&api.velocity, &movement, 1.0f / FRAMETIME);
                api.position.x = collision_position.x;
                const f32 updated_y = collision_position.y - object->character_bottom * api.field_0xa8;
                api.position.z = collision_position.z;
                api.position.y = updated_y;
                // Original 0x104eb0..0x10501a carries all three headings with the
                // rotation of the supporting platform.
                if (api.field_0x287 == 0 && api.supporting_platform_id != -1) {
                    NUMTX *current = NULL;
                    NUMTX *previous = NULL;
                    TerrainPlatGetMtx(api.supporting_platform_id, &previous, &current);
                    if (previous != NULL && current != NULL) {
                        NUVEC forward = {0.0f, 0.0f, 1.0f};
                        NUVEC previous_forward, current_forward;
                        NuVecMtxRotate(&previous_forward, &forward, previous);
                        const NUANG previous_heading = NuAtan2D(previous_forward.x, previous_forward.z);
                        NuVecMtxRotate(&current_forward, &forward, current);
                        const NUANG current_heading = NuAtan2D(current_forward.x, current_forward.z);
                        const NUANG rotation = NuAngSub(current_heading, previous_heading);
                        if (rotation != 0) {
                            api.field_0x276 = NuAngAdd(rotation, api.field_0x276);
                            api.facing_angle = NuAngAdd(rotation, api.facing_angle);
                            api.movement_facing_angle = NuAngAdd(rotation, api.movement_facing_angle);
                        }
                    }
                }
                if (api.field_0x287 == 0) {
                    bool wall_impact = false;
                    if (object->field_0x1084 != 0 && (api.flags_low & 0x84) == 0 && object->character_context != 0 &&
                        object->character_context != 0x43 && object->character_context != 0x45 &&
                        (object->field_0xf02 & 1) == 0) {
                        for (i32 i = 0; i < impact_count; ++i) {
                            if (impact_records[i].normal.y < 0.707f) {
                                wall_impact = true;
                                break;
                            }
                        }
                    }
                    if (!wall_impact) {
                        api.respawn_timer -= FRAMETIME;
                        if (api.respawn_timer < 0.0f) {
                            api.respawn_timer = 0.0f;
                        }
                    } else {
                        api.respawn_timer += FRAMETIME;
                    }
                }
            }
            const f32 floor_height = (api.flags_low & 4) != 0
                                         ? api.field_0x218
                                         : GameShadow(object, &api.position, 5.0f, terrain_mask | 0x1f);
            bool stopped_without_terrain = false;
            if (floor_height != 2000000.0f) {
                object->field_0xe20 |= 2;
                if (perform_motion) {
                    const f32 bottom_offset = api.field_0xa8 * object->character_bottom;
                    const f32 top = object->character_top * api.field_0xa8 + api.position.y;
                    const f32 bottom = api.position.y + bottom_offset;
                    const f32 height = top - bottom;
                    if (floor_height > 0.1f * height + bottom && height + 0.01f >= (floor_height + height) - top) {
                        api.position.y = floor_height - bottom_offset;
                    }
                }
            } else {
                object->field_0xe20 &= static_cast<u8>(~2u);
                if (NOTERRAINSTOP != 0) {
                    api.velocity.z = 0.0f;
                    api.velocity.x = 0.0f;
                    api.position.x = api.start_position.x;
                    api.position.z = api.start_position.z;
                    stopped_without_terrain = true;
                }
            }
            if (!stopped_without_terrain && (api.flags_low & 4) == 0) {
                api.field_0x218 = floor_height;
                GetSurfaceInfo(object, floor_height != 2000000.0f ? 1 : 0, floor_height);
                if (api.supporting_platform_id == -1 && api.field_0x27d != 0 && object->field_0x1078 != -1) {
                    api.supporting_platform_id = object->field_0x1078;
                }
                if ((api.character_data->game_character->flags_090 & 0x8000) == 0 &&
                    object->field_0x1020 == 2000000.0f && object->field_0x1078 != -1) {
                    const f32 reflection_height = FindReflectionNoPlatforms(&api.position);
                    if (reflection_height != 2000000.0f) {
                        object->field_0x1020 = reflection_height;
                        object->field_0x1087 = 2;
                    }
                }
            }
            // Contact reconstruction joins the near-floor test at 0x102d26.
            api.field_0x27d = object->field_0x105c != 0 ? APIOBJECT_TERRAIN_CONTACT_FLOOR : 0;
            if (GameObjectNearFloor(object, 1.0f, NULL) != 0) {
                api.field_0x27d |= APIOBJECT_TERRAIN_CONTACT_NEAR_FLOOR;
            }
            if (api.field_0x27d != 0) {
                object->field_0xeff |= 2;
            }
        }

        // Original 0x1029e6/0x103cb4 transfers vertical momentum only when
        // leaving or landing on the object's recorded character platform.
        if (api.field_0x27d != 0) {
            if (previous_platform == -1 && api.supporting_platform_id != -1 &&
                api.supporting_platform_id == object->field_0x1078 && entry_vertical_velocity < 0.0f) {
                const i32 platform_id = api.supporting_platform_id;
                GRABBER_s *grabber = WORLD->grabber;
                if (grabber != NULL && grabber->platform_id == platform_id) {
                    if (grabber->platform_contact_timer <= 0.0f) {
                        grabber->platform_contact_timer = 0.25f;
                    }
                } else {
                    GameObject_s *platform_owner =
                        CharPlatform_FindObjFromPlatID(WORLD->char_platform_sys, platform_id);
                    if (platform_owner != NULL &&
                        platform_owner->apiobj.character_data->game_character->field_0x28 > 0.0f) {
                        platform_owner->apiobj.velocity.y =
                            entry_vertical_velocity * 0.5f + platform_owner->apiobj.velocity.y;
                    }
                }
            }
        } else {
            if (previous_platform != -1 && api.supporting_platform_id == -1 &&
                object->field_0x1078 == previous_platform) {
                const i32 platform_id = previous_platform;
                GRABBER_s *grabber = WORLD->grabber;
                if (grabber != NULL && grabber->platform_id == platform_id) {
                    if (grabber->platform_contact_timer <= 0.0f) {
                        grabber->platform_contact_timer = 0.25f;
                    }
                } else {
                    GameObject_s *platform_owner =
                        CharPlatform_FindObjFromPlatID(WORLD->char_platform_sys, platform_id);
                    if (platform_owner != NULL &&
                        platform_owner->apiobj.character_data->game_character->field_0x28 > 0.0f) {
                        platform_owner->apiobj.velocity.y -= entry_vertical_velocity * 0.5f;
                    }
                }
            }
        }

        // Original 0x102a01..0x102a9a handles a moving character platform
        // above the player before dispatching the normal movement callback.
        if (VehicleArea == 0 && static_cast<i8>(api.flags_low) < 0 && api.supporting_platform_id != -1) {
            GameObject_s *platform_owner =
                CharPlatform_FindObjFromPlatID(WORLD->char_platform_sys, api.supporting_platform_id);
            if (platform_owner != NULL) {
                const GAMECHARACTERDATA *data = platform_owner->apiobj.character_data->game_character;
                if ((data->flags_090 & 0x1000) != 0 && platform_owner->apiobj.position.y > api.position.y &&
                    platform_owner->apiobj.horizontal_velocity_magnitude > 0.5f * data->run_speed) {
                    StartFlatten(platform_owner, object);
                }
            }
        }

        if (api.field_0x287 != 0) {
            if (api.field_0x27d != 0) {
                api.velocity.y = 0.0f;
                api.flags_low |= 4;
            }
            if ((api.flags_low & 4) == 0) {
                ApplyGravity(object, NULL, 0.0f, 0.0f, NULL);
            }
        } else {
            // Original 0x1032a0 checks swept contacts or the standing surface.
            const i32 check_terrain_hazards =
                object->character_context != 0x2b && (api.flags_low & 4) == 0 && gone_through_door_to_new_level == 0;
            // Original 0x1038b0 completes the doomed state on contact, timeout,
            // or a collision during the falling variant.
            if (object->character_context == 0x2b &&
                (api.field_0x27d != 0 || api.model_draw_result == 0 || object->turn_braking >= 5.0f ||
                 (object->field_0xe36 == 3 && object->field_0x1084 != 0))) {
                if (api.model_draw_result != 0) {
                    InstantKillParts(object, 0, 0.0f);
                }
                KillPlayer(object, 3, 1, NULL);
                if (WORLD->current_level == CRUISERD_LDATA && object->field_0xe36 == 3) {
                    MiscTime = 0.1f;
                }
            }
            if (check_terrain_hazards && CannotKill(object) == 0 && api.field_0x287 == 0 &&
                (VehicleArea == 0 || static_cast<i8>(api.flags_low) < 0) && (object->field_0xefe & 4) != 0 &&
                (object->field_0xefa & 4) == 0 && object->character_context != 0x5d) {
                if (object->field_0x1084 != 0) {
                    const i32 count = impact_count == -1 ? 1 : impact_count;
                    for (i32 i = 0; i < count; ++i) {
                        const i32 surface =
                            static_cast<i8>(impact_count == -1 ? object->field_0x6b0 : impact_records[i].material[0]);
                        NUVEC *normal = impact_count == -1 ? &object->contact_normal : &impact_records[i].normal;
                        if (static_cast<u32>(surface) > 31) {
                            continue;
                        }
                        if (WORLD->current_level == BONUS_GUNSHIPA_LDATA ||
                            (PODRACE_ADATA != NULL && WORLD->area == PODRACE_ADATA && GameTimer.time_elapsed >= 1.0f)) {
                            NewRumble(object->pad_gamepad->pad, (static_cast<f32>(qrand()) * (1.0f / 65535.0f)) * 0.5f,
                                      0);
                            if (api.model_draw_result == 0 && object->character_context != 0x23) {
                                if (static_cast<i8>(api.flags_low) < 0) {
                                    LoseCoins(object, 2);
                                }
                                KillPlayer(object, 2, 1, NULL);
                                break;
                            }
                        } else if (((api.character_data->model_flags & 0x2000) != 0 ||
                                    (api.character_data->game_character->flags_090 & 0x04000000) != 0) &&
                                   api.field_0x27f == 6) {
                            break;
                        }
                        if ((WORLD->area == NULL || WORLD->area != PODSPRINT_ADATA) &&
                            TerrainKillPlayer(object, surface, &normal->y) != 0) {
                            if (WORLD->current_level == ASTEROIDCHASEC_LDATA) {
                                GameCam_Blend(GameCam, 1.0f, 0.0f, 1);
                            }
                            break;
                        }
                        if ((TerSurface[surface].flags & 0x80) != 0 && WORLD->current_level != BONUS_GUNSHIPA_LDATA) {
                            NewRumble(object->pad_gamepad->pad, 0.6f, 0);
                            NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
                            if (object->field_0xd24 != 1.0f && CannotKill(object) == 0) {
                                ObjHitObj(NULL, object, 1, 0, 0, 1);
                            }
                            break;
                        }
                    }
                } else if (api.field_0x27d != 0 && (WORLD->area == NULL || WORLD->area != PODSPRINT_ADATA)) {
                    const i32 surface = static_cast<i8>(api.field_0x281);
                    if (static_cast<u32>(surface) <= 31) {
                        TerrainKillPlayer(object, surface, &object->surface_normal.y);
                    }
                }
            }
            if (check_terrain_hazards && CannotKill(object) == 0 && (object->field_0xefa & 4) == 0 &&
                api.field_0x287 == 0 && api.water_height != 2000000.0f) {
                const i32 layer = static_cast<i8>(api.field_0x27f);
                const bool tractor_swamp =
                    layer == 9 && object->id == id_TRACTOR && WORLD->area != NULL && WORLD->area == DAGOBAH_ADATA;
                if ((layer == 3 || tractor_swamp || (TerLayer[layer].flags & 0x20) != 0 ||
                     ((api.character_data->model_flags & 0x2000) == 0 &&
                      (api.character_data->game_character->flags_090 & 0x04000000) == 0)) &&
                    NoLayerKill(object) == 0 && object->character_context != 0x5d) {
                    const u32 layer_flags = TerLayer[static_cast<i8>(api.field_0x27f)].flags;
                    const bool exempt_context =
                        static_cast<i8>(api.flags_low) >= 0 &&
                        (object->character_context == 0x46 || object->character_context == 0x47);
                    const bool rescue_below =
                        ((layer_flags & 1) != 0 || tractor_swamp) && api.water_height > api.position.y &&
                        !(VehicleArea != 0 && BonusArea != 0 && api.character_data->game_character->field_0x28 > 0.0f);
                    const bool rescue_above = (layer_flags & 0x20) != 0 && api.collision_max.y > api.water_height;
                    if (!((rescue_below || rescue_above) && !exempt_context)) {
                        if ((api.character_data->model_flags & 0x40) == 0 && object->id != id_SNAKE &&
                            api.field_0x27f == 9 && api.water_height > api.collision_max.y) {
                            if (BonusWinner == -1) {
                                const i32 coins = LoseCoins(object, 2);
                                if (coins > 0) {
                                    AddPickups(coins, 0, 0, 0, &api.upper_position, NULL, 2.0f,
                                               static_cast<i8>(api.field_0x27c), 1.0f, 2000000.0f, NULL, 1, 0, false);
                                }
                            }
                            InstantKillParts(object, 1, 2.0f);
                            KillPlayer(object, 3, 1, NULL);
                        }
                    } else {
                        if (object->doomed_escape_locator != NULL && static_cast<i8>(api.flags_low) >= 0) {
                            if ((object->field_0xefd & 8) != 0 || TouchHacks::AiPlayerTakeDamageOnKillRescue(*object)) {
                                ObjHitObj(NULL, object, 1, 0, 0, 1);
                            }
                            if (api.field_0x287 == 0) {
                                StartBigJump(object, &object->doomed_escape_locator->position, 0, 1.0f, 1.0f, 0, 0);
                            }
                        } else {
                            ClearLastSafeTakeOver(object);
                            Player_ClearContext(object, 1);
                            object->context_animation = 5;
                            if (object->character_context != -1 &&
                                api.character_model->model_data_b[api.anim_packet.requested_animation] != NULL) {
                                object->context_animation = api.anim_packet.requested_animation;
                            }
                            object->character_context = 0x2b;
                            object->field_0xe31 = 0;
                            if ((TerLayer[static_cast<i8>(api.field_0x27f)].flags & 0x20) == 0) {
                                if (api.field_0x27f == 6) {
                                    api.velocity.y = -0.25f;
                                    object->field_0xe36 = 2;
                                    NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
                                } else if (WORLD->current_level == VADERC_LDATA) {
                                    api.velocity.y = -0.5f;
                                    object->field_0xe36 = 4;
                                    NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
                                } else {
                                    object->field_0xe36 = 1;
                                }
                            } else {
                                object->field_0xe36 = 3;
                            }
                            object->turn_braking = 0.0f;
                            PlayDieSfx(object);
                            object->current_hp = 0;
                            if (static_cast<i8>(api.flags_low) < 0 && object->coinpacket != NULL &&
                                object->coinpacket->coins != 0 && BonusWinner == -1) {
                                const i32 coins = LoseCoins(object, 2);
                                if (coins > 0) {
                                    f32 height = api.water_height;
                                    if (height != 2000000.0f &&
                                        (TerLayer[static_cast<i8>(api.field_0x27f)].flags & 1) == 0) {
                                        height = 2000000.0f;
                                    }
                                    AddPickups(coins, 0, 0, 0, &api.collision_position, NULL, 2.0f,
                                               static_cast<i8>(api.field_0x27c), 1.0f, height, NULL, 1, 0, false);
                                }
                                object->coinpacket->scale = 1.5f;
                            }
                        }
                    }
                }
            }
            // Original 0x1036a0 refreshes these even if the hazard handler killed
            // the object. The bypass and level-transition routes retain old state.
            if (check_terrain_hazards) {
                api.is_underwater = static_cast<u8>(UnderWater(object));
                api.intersects_water = static_cast<u8>(IntersectWater(object));
            }
            // Original 0x102ddb emits the splash only on a dry-to-water transition.
            if (api.intersects_water != 0 && (entry_is_underwater | entry_intersects_water) == 0) {
                bool splash = object->character_context == 0x1f || object->character_context == 0 ||
                              (CInfo[object->character_context].flags & 4) != 0 || object->character_context == 0x0e ||
                              object->character_context == 0x0d;
                if (!splash) {
                    const i32 animation = CurrentAnim(&api.anim_packet);
                    splash = animation == 0x28 || animation == 5 || static_cast<u32>(animation - 0x4b) <= 1;
                }
                if (splash) {
                    AddWaterSplash(object, &api.collision_position);
                }
            }
            if (api.model_draw_result != 0 && object->character_context != 0x40 && object->character_context != 0x2c) {
                extern void GameObjectRotation(GameObject_s *, i32);
                GameObjectRotation(object, 2);
            }
            // The jump callback returns to the common saved-button restoration
            // at 0x102ed4, including when it changes the object's gamepad.
            const u32 held_buttons = object->pad_gamepad->buttons_held;
            const u32 pressed_buttons = object->pad_gamepad->buttons_pressed;
            if ((api.field_0x1f8 & 0x180) == 0x80 && (entry_menu_id != -1 || MiniCutCam == 2)) {
                object->pad_gamepad->buttons_held = 0;
                object->pad_gamepad->buttons_pressed = 0;
            }
            Tag_Check(object);
            PreResetCode(object);
            api.character_data->move_fn(object);
            PostResetCode(object);
            if ((api.character_data->model_flags & 0x00200000) == 0) {
                BigJumpCode(object);
            }
            object->pad_gamepad->buttons_held = held_buttons;
            object->pad_gamepad->buttons_pressed = pressed_buttons;
        }

        if ((object->field_0xefa & 4) == 0 && CurTerr != NULL && api.field_0x287 == 0 &&
            (WORLD->current_level->flags & 0xe0) == 0 &&
            static_cast<f32>(CurTerr->minimum_height) - 5.0f > api.position.y) {
            if (api.model_draw_result != 0) {
                InstantKillParts(object, 0, 0.0f);
            }
            KillPlayer(object, 3, 1, NULL);
        }
    }

    // Common epilogue at 0x102b60.
    entry_pad->allocated_5a &= 0xe9;
    object->field_0xd14 = 0;
    entry_pad->operator_data = NULL;
    NuVecRotateYValZ(&object->facing_direction, 1.0f, object->apiobj.field_0x276);
}
static inline void MakePlayCorner(GAMECAMERA_s *camera, f32 screen_x, f32 screen_y, NUVEC *near_corner,
                                  NUVEC *far_corner) {
    NUVEC ray = {screen_x, screen_y, 1.0f};
    NuVecMtxRotate(&ray, &ray, &camera->target_mtx);
    NuVecAdd(near_corner, &ray, &camera->pos);
    NuVecNorm(&ray, &ray);
    far_corner->y = near_corner->y + ray.y * 5.0f;
    far_corner->z = near_corner->z + ray.z * 5.0f;
    far_corner->x = near_corner->x + ray.x * 5.0f;
}

static inline void SetPlayPlane(PLAYPLANE_s *plane, NUVEC *a, NUVEC *b, NUVEC *c, NUVEC *d, NUVEC *edge_a0,
                                NUVEC *edge_a1, NUVEC *edge_b0, NUVEC *edge_b1) {
    plane->point.x = (a->x + b->x + c->x + d->x) * 0.25f;
    plane->point.y = (a->y + b->y + c->y + d->y) * 0.25f;
    plane->point.z = (a->z + b->z + c->z + d->z) * 0.25f;
    NUVEC second_edge;
    NUVEC first_edge;
    NuVecSub(&first_edge, edge_a0, edge_a1);
    NuVecSub(&second_edge, edge_b0, edge_b1);
    NuVecCross(&plane->normal, &first_edge, &second_edge);
    NuVecNorm(&plane->normal, &plane->normal);
}

void MakePlayPlanes(GAMECAMERA_s *camera) {
    NUVEC far_corner[4];
    NUVEC near_corner[4];
    MakePlayCorner(camera, -0.85f * PANEL3DMULX, 0.85f * PANEL3DMULY, &near_corner[0], &far_corner[0]);
    MakePlayCorner(camera, 0.85f * PANEL3DMULX, 0.85f * PANEL3DMULY, &near_corner[1], &far_corner[1]);
    MakePlayCorner(camera, -0.85f * PANEL3DMULX, -0.85f * PANEL3DMULY, &near_corner[2], &far_corner[2]);
    MakePlayCorner(camera, 0.85f * PANEL3DMULX, -0.85f * PANEL3DMULY, &near_corner[3], &far_corner[3]);

    // Plane order is near, left, right, top, bottom, far.
    SetPlayPlane(&PlayPlane[1], &near_corner[0], &far_corner[0], &far_corner[2], &near_corner[2], &far_corner[0],
                 &near_corner[0], &far_corner[2], &near_corner[0]);
    SetPlayPlane(&PlayPlane[2], &far_corner[1], &near_corner[1], &near_corner[3], &far_corner[3], &near_corner[1],
                 &far_corner[1], &near_corner[3], &far_corner[1]);
    SetPlayPlane(&PlayPlane[3], &near_corner[0], &near_corner[1], &far_corner[1], &far_corner[0], &near_corner[1],
                 &near_corner[0], &far_corner[1], &near_corner[0]);
    SetPlayPlane(&PlayPlane[4], &near_corner[2], &far_corner[2], &far_corner[3], &near_corner[3], &far_corner[2],
                 &near_corner[2], &far_corner[3], &near_corner[2]);
    SetPlayPlane(&PlayPlane[0], &near_corner[0], &near_corner[2], &near_corner[3], &near_corner[1], &near_corner[2],
                 &near_corner[0], &near_corner[3], &near_corner[0]);
    SetPlayPlane(&PlayPlane[5], &far_corner[0], &far_corner[1], &far_corner[3], &far_corner[2], &far_corner[1],
                 &far_corner[0], &far_corner[3], &far_corner[0]);
}

DECOMP_ASSERT(sizeof(TERRAIN_DEBUG_RECORD) == 100, "Terrain debug record ABI");

void TerrDrawPlatCol(tertype *terrain, i16 index, i32 colour);




void ScanTerrainPlatform(i32 group_index, i32 terrain_mask);
void ScanTerrainHandel(i32 scan_type, i16 *handle);

extern "C" i32 NewRayCastPlatForm(NUVEC *position, NUVEC *movement, f32 radius, f32 separation_epsilon,
                                  i32 platform_index, i32 terrain_mask);

extern "C" void *NuScratchAlloc32(i32 size);
extern "C" void NuScratchRelease();
void TerrDraw(tertype *terrain, i16 index);
void TerrDrawPlat(tertype *terrain, i16 index);

extern "C" void DrawHitTerrain(void);

void TerrShowCamTerr();
NUVEC TerrainStaticMtx(PLATSKININFO *info, nuvec_s *position, i32);
extern "C" void NewShapeInit(NUVEC *offset);

void TerrainImpactNorm();
void ScanTerrainPlatform(i32 group_index, i32 terrain_mask);
i32 TerrainBlockOnBlock(WORLDINFO_s *world, pushblock_s *block, nuvec_s *points, float *heights) {
    if (!heights || !points)
        return 0;
    heights[0] = heights[1] = heights[2] = heights[3] = -10000.0f;
    for (i32 i = 0; i < world->push_block_count; ++i) {
        pushblock_s *other = &world->push_blocks[i];
        if (other == block)
            continue;
        f32 xmin = other->position->x - fabsf(other->bounds_min.x),
            xmax = other->position->x + fabsf(other->bounds_max.x);
        f32 zmin = other->position->z - fabsf(other->bounds_min.z),
            zmax = other->position->z + fabsf(other->bounds_max.z);
        f32 height = fabsf(other->bounds_max.y) + other->position->y;
        for (i32 j = 0; j < 4; ++j)
            if (points[j].x > xmin && points[j].x < xmax && points[j].z > zmin && points[j].z < zmax)
                heights[j] = height;
    }
    return 1;
}

void ScanTerrIDRemovePlat(i32 platform_index);
i32 HitWallSpline();

void ScanWallSplineTerrain(i32, i32 terrain_mask, i32);

#undef TERRAIN_GET_SCAN_BOUNDS

i32 TerrainImpactPlatform(unsigned char *hit_flags);

void NewTerrStoreAnyInfo();

void TerrainMoveImpactData();
extern "C" void *NuScratchAlloc32(i32 size);
extern "C" void NuScratchRelease();


i32 TerrainPlatformMoveCheck(nuvec_s *, nuvec_s *, i32, i32, i32);

extern "C" void TerrainScanWallSpline(TERRAIN_SPATIAL_NODE *node);

extern "C" void TerrainAddWallSpline(TERRAIN_SPATIAL_NODE *node, TERRSET *terrain);

extern "C" void TerrainRemoveWallSpline(TERRAIN_SPATIAL_NODE *node, TERRSET *terrain);

extern "C" void FullDeflect(NUVEC *normal, NUVEC *movement, NUVEC *result);

void FullDeflectSmallY(NUVEC *normal, NUVEC *movement, NUVEC *result);

i32 FullDeflectTest(nuvec_s *normal, nuvec_s *movement, nuvec_s *result);

void FullDeflectSize(nuvec_s *normal, nuvec_s *movement, nuvec_s *result);

extern "C" void FullReflect(NUVEC *normal, NUVEC *movement, NUVEC *result);

void FullReflectTest(nuvec_s *normal, nuvec_s *movement, nuvec_s *result);


extern "C" void TerrainSetPlatConnectTol(f32 tolerance);

extern "C" void TerrainSetWallDeflectYScale(f32 scale);

i32 PlatformChecks(i32 count, nuvec_s *movement);



i32 TerrainPlatformEmbedded(nuvec_s *movement_delta);
i32 TerrainPlatformMoveCheck(nuvec_s *position, nuvec_s *normal, i32 platform_index, i32 resolve_impact,
                             i32 keep_embedded_disabled);
NUVEC TerCrossProduct(NUVEC *a, NUVEC *b);

void TerrDrawSitu(tertype *terrain, terrsitu_s *situation);

void TerrDrawPlat(tertype *terrain, i16 index);

void TerrDraw(tertype *terrain, i16 index);

void DrawMSitu(i32 index);

extern "C" void terrainpickupinit(char *path, void **buffer);

extern "C" void noterraininit(void);

extern "C" void TerrDrawImpactPol(void);

void DrawWallSpline(float height);

namespace {

    // Target NewScanRot 0x378fb0 uses 0.1f on both horizontal axes when
    // selecting the terrain shapes that can contain the cast point.





} // namespace

void NewScan(nuvec_s *position, i32 terrain_mask, i32 scan_platforms);

void NewScanRot(nuvec_s *position, i32 terrain_mask);

i16 InsideLineF(f32, f32, f32, f32, f32, f32);

namespace {

    const f32 NO_TERRAIN_HEIGHT = 2000000.0f;


    static void ConsiderShadowTriangle(const NUVEC &local_position, const TERRAIN_GROUP &group, TERRAIN_SHAPE *surface,
                                       const NUVEC &origin, const NUVEC &edge_a, const NUVEC &edge_b,
                                       const NUVEC &normal, f32 winding_y, f32 query_height, i16 terrain_group,
                                       ShadowSurfaceCandidate *floor_candidate,
                                       ShadowSurfaceCandidate *above_candidate) {
        const f32 point_x = local_position.x, point_z = local_position.z;
        if (winding_y > 0.0f) {
            if (InsideLineF(point_x, point_z, edge_a.x, edge_a.z, origin.x, origin.z) == 0 ||
                InsideLineF(point_x, point_z, edge_b.x, edge_b.z, edge_a.x, edge_a.z) == 0 ||
                InsideLineF(point_x, point_z, origin.x, origin.z, edge_b.x, edge_b.z) == 0)
                return;
        } else if (winding_y < 0.0f) {
            if (InsideLineF(point_x, point_z, origin.x, origin.z, edge_a.x, edge_a.z) == 0 ||
                InsideLineF(point_x, point_z, edge_a.x, edge_a.z, edge_b.x, edge_b.z) == 0 ||
                InsideLineF(point_x, point_z, edge_b.x, edge_b.z, origin.x, origin.z) == 0)
                return;
        } else
            return;

        const f32 plane_height =
            (origin.y + group.origin.y) +
            ((local_position.x - origin.x) * normal.x + (local_position.z - origin.z) * normal.z) / -normal.y;
        if (plane_height <= query_height) {
            if (winding_y > 0.0f && plane_height > floor_candidate->height) {
                floor_candidate->height = plane_height;
                floor_candidate->surface = surface;
                floor_candidate->normal = normal;
                floor_candidate->terrain_group = terrain_group;
            }
        } else if (plane_height < above_candidate->height ||
                   (plane_height == above_candidate->height && normal.y < above_candidate->normal.y)) {
            // The target keeps the nearest ordinary surface above the cast
            // origin as well as the conventional floor below it.  NewCast's
            // final range check decides whether this surface is reachable.
            above_candidate->height = plane_height;
            above_candidate->surface = surface;
            above_candidate->normal = normal;
            above_candidate->terrain_group = terrain_group;
        }
    }

} // namespace

f32 NewCast(nuvec_s *position, f32 height_above, f32 roof_range);

extern "C" void TerrainWallAng(f32 slope);

extern "C" void ShadowDir(NUVEC *direction);

extern "C" i32 EShadowInfo(void);

extern "C" i32 ShadowRoofInfo(void);

extern "C" i32 EShadowRoofInfo(void);

extern "C" void NewTerrPlatformsOff(void);

extern "C" i32 NewShadowOnPlatform(void);

extern "C" i32 NewShadowOnMSitu(void);

extern "C" void AddCollisionSphere(NUVEC *position, f32 radius);

extern "C" void AddPickupTerr(i32 type, NUVEC *position);



extern "C" i32 AddPickupTerrRot(i32 type, NUMTX *matrix, NUMTX *previous, i32 rotating);

void TerrainSideClamp(NUVEC *axis, NUVEC *position);

i32 TerrShapeSideStep(NUVEC *, NUVEC *, u8 *);

extern "C" void CubeImpact(NUMTX *current, NUMTX *previous, NUVEC *normal, f32 scale, NUVEC *impact);

extern "C" i32 NewTerrainOnAPlatform(void *id);

extern "C" void NewTerrHitInfo(u8 *info);

extern "C" void NewTerrainScaleYMask(NUVEC *position, NUVEC *movement, u8 *hit_flags, i32 object_index, f32 radius,
                                     f32 collision_radius, f32 object_scale, i32 embedded_retry, i32 scan_flags,
                                     i32 terrain_mask);

extern "C" f32 NewRayCastGetTOFI(void);

extern "C" i32 NewRayCastGetImpactTerrainType(void);

extern "C" void NewRayCastGetImpactNormal(NUVEC *normal);

extern "C" f32 NewRayCastGetEmbedDist(void);

extern "C" i32 NewRayCastHitWallSpline(void);

extern "C" i32 NewRayCast(NUVEC *position, NUVEC *movement, f32 radius, i32 scan_flags);

extern "C" i32 NewRayCastScaleYMask(NUVEC *position, NUVEC *movement, f32 radius, f32 scale_y, i32 scan_flags,
                                    u32 terrain_mask);

extern "C" void TerrainPolyEdge(NUVEC *first, NUVEC *second);

extern "C" i32 NewRayCastMask(NUVEC *position, NUVEC *movement, f32 radius, i32 terrain_mask, i32 scan_flags);

extern "C" i32 NewRayCastSet(NUVEC *position, NUVEC *movement, f32 radius, f32 separation_epsilon, f32 compare_epsilon,
                             i32 scan_type, i32 scan_flags);

extern "C" i32 NewRayCastSetMask(NUVEC *position, NUVEC *movement, f32 radius, f32 separation_epsilon,
                                 f32 compare_epsilon, i32 scan_type, i32 terrain_mask, i32 scan_flags);

extern "C" void NewTerrAxisFreedom(TERRAIN_AXIS_FREEDOM_SHAPE *shape, NUVEC *position);

extern "C" i16 NewPlatPickupInst(void *object, i32 type);

extern "C" void ReassignPickupInst(i32 index, i32 type);

extern "C" i32 AddMSituExtraTerrRot(i32 source_index, NUMTX *matrix, NUMTX *previous, i32 rotating, TERRSET *source);

extern "C" void TerrainWallSideSlide(NUVEC *movement, void *id, f32 speed, f32 upward_scale, NUANG angle);

extern "C" i32 NewRayCastSetHandel(NUVEC *position, NUVEC *movement, f32 radius, f32 separation_epsilon,
                                   f32 compare_epsilon, i16 *handle, i32 scan_type);

void ScanTerrainHandel(i32 terrain_mask, i16 *handle);
f32 NewCast(nuvec_s *position, f32 height_above, f32 roof_range);

extern "C" f32 NewShadowHandelEx(NUVEC *position, i32, f32 height_above, f32 height_below, i32 terrain_mask,
                                 i16 *handle);

extern "C" i32 TerrainPlatId(void);

extern "C" i32 TerrainTrackBack(NUVEC *position, NUVEC *direction, f32 radius, f32 backoff, NUVEC *result);

extern "C" i32 TerrainIntensityInfo();

extern "C" i32 TerrainInfo();

extern "C" i32 TerrainInfoExtra();

extern "C" i32 ShadowInfo(void);

extern "C" i32 ShadowIntensityInfo(void);

// Original 0x378fb0: the optimized rotating scan shares the terrain engine state.
extern TERRSET *CurTerr;
extern "C" void *NuScratchAlloc32(i32);
extern "C" void NuScratchRelease();

extern "C" void TerrainPlatformOldUpdate(void);

extern "C" i32 PlatformCrush(void);

extern "C" void NewScanInit(void);

namespace {
    static TerrainScanBounds TerrainHandleBounds(const NUVEC &position, const NUVEC &movement, f32 radius);
    static bool TerrainHandleVerticesOverlap(const TerrainScanBounds &bounds, const TERRAIN_SHAPE &shape);
    static void TerrainHandleFilterShapes(TerrainScanWriter *writer, TERRAIN_SHAPE **shapes, i32 count,
                                          const TerrainScanBounds &bounds, i32 terrain_mask, bool rotating);
    static bool TerrainHandleWallOverlap(const TerrainScanBounds &bounds, const TERRAIN_WALL_POINT *wall);


} // namespace

i16 *NewScanHandelFull(nuvec_s *position, nuvec_s *movement, f32 radius, i32 scan_type, i32 terrain_mask);

#undef TERRAIN_COLLECT_WALL_SPLINES

i16 *NewScanHandelSubset(i16 *subset, nuvec_s *position, nuvec_s *movement, f32 radius, i32 terrain_mask);

extern "C" i16 *NewScanHandel(nuvec_s *position, nuvec_s *movement, f32 radius, i32 scan_type, i16 *subset);

extern "C" void NewRaySetDisablePalt(i32 disabled);

namespace {



} // namespace

void ScanTerrainHandel(i32 terrain_mask, i16 *handle);

extern "C" void TerrainTrackFlush(void);

i32 HitTerrain();

extern "C" f32 NewShadowEx(NUVEC *position, i32, f32 height_above, f32 height_below, i32 terrain_mask);

i32 HitTerrPoly(tertype *surface, i32 group_index);

void PlatformConnect(char *track_id, NUVEC *position_delta, NUVEC *movement_delta, i32 platform_index);

void RayImpact(NUVEC *movement);
