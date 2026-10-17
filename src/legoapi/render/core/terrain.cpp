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
extern i32 NOTERRAINSTOP;
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

extern i32 testshadowfix;

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
static i32 TerrainKillPlayer(GameObject_s *object, i32 surface, const f32 *normal_y);

void TerrainPlayer(GameObject_s *object);
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
