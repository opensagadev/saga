// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/world/area.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/world/levels/levels.h"
#include "globals.h"
#include <stdlib.h>
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/cutscenes/minicamcut.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/menus/core/gamehint.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "gameframework/saveload.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include <new>
#include "legoapi/props/doors/door.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nuvec4.h"
#include <limits.h>
#include "legoapi/lib/camera.h"

// Original data symbol testmovef is float0.005 at 0x617cb0.



// Retail only keeps the IPA-SRA clone, which drops the unused fourth argument.
#if defined(__i386__)
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone))
#endif

#define ALPHA_GRID_VERTEX(vx, vy)                                                                                      \
    do {                                                                                                               \
        direction.x = (vx);                                                                                            \
        direction.y = -(vy) * camera.aspect;                                                                           \
        direction.z = adjacent;                                                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        NuVecMtxTransform(&direction, &direction, &camera.mtx);                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        brightness = direction.y * 0.5f + 0.5f;                                                                        \
        if (brightness <= near_angle)                                                                                  \
            brightness = near_scale * parameters->intensity;                                                           \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale * parameters->intensity;                                                            \
        else                                                                                                           \
            brightness = ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity; \
        if (parameters->directional) {                                                                                 \
            f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;       \
            if (angle < parameters->direction_near_angle) {                                                            \
                brightness += 128.0f * parameters->direction_far_scale;                                                \
            } else if (!(angle > parameters->direction_far_angle)) {                                                   \
                f32 blend = NuPowFast((angle - parameters->direction_near_angle) /                                     \
                                          (parameters->direction_far_angle - parameters->direction_near_angle),        \
                                      parameters->direction_bias);                                                     \
                brightness +=                                                                                          \
                    128.0f * ((1.0f - blend) * (parameters->direction_far_scale - parameters->direction_near_scale) +  \
                              parameters->direction_near_scale);                                                       \
            }                                                                                                          \
        }                                                                                                              \
        u32 colour = (static_cast<u32>(static_cast<u8>(128)) << 24) |                                                  \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 16) |                   \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 8) |                    \
                     static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness))));                            \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] =                                                                     \
                ((static_cast<i32>(colour) >> 1) & 0x7f7f7f) | (colour & 0xff000000);                                  \
        else                                                                                                           \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = colour;                                                             \
        NuPrim2DAddXYZ((vx), (vy), 0.0f);                                                                              \
    } while (0)

#define SWIPE_PRIM_UV(U, V)                                                                                            \
    do {                                                                                                               \
        PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);                      \
        if (!g_NuPrim_NeedsHalfUVs) {                                                                                  \
            vertex->float_uv[0] = (U);                                                                                 \
            vertex->float_uv[1] = (V);                                                                                 \
        } else {                                                                                                       \
            vertex->half_uv[0] = NuRndrFloatToHalf(U);                                                                 \
            vertex->half_uv[1] = NuRndrFloatToHalf(V);                                                                 \
        }                                                                                                              \
    } while (false)

struct GAMECAMERA_s;

namespace {




} // namespace

namespace {

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

// Constructs the fixed terrain header and its variable-sized group, index and
// platform arrays in the caller's arena. The four bounds arrays and the cell
// assignment array are temporary storage carved backwards from buf_end.
#define IS_SNAN(x) (*(u32 *)&(x) == 0xffc00000)

namespace {


} // namespace

// Retail only keeps the IPA-SRA clone, which takes &normal->y.
#if defined(__i386__)
#define TERRAIN_KILL_PLAYER_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define TERRAIN_KILL_PLAYER_ATTR __attribute__((noinline, noclone))
#endif

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


namespace {

    // Target NewScanRot 0x378fb0 uses 0.1f on both horizontal axes when
    // selecting the terrain shapes that can contain the cast point.

} // namespace

namespace {

} // namespace

// Original 0x378fb0: the optimized rotating scan shares the terrain engine state.

namespace {

} // namespace

namespace {

} // namespace

static NARROWSOCKEXCEPTION *NarrowSockException;

static inline i32 NarrowSockPositionAllowed(GameObject_s *object, SOCKSYS *system, i32 level_index) {
    if (system == NULL || object->sock_position.location.sock == -1)
        return 0;
    if ((system->sock[object->sock_position.location.sock].flags & 0x800) == 0)
        return 0;
    if (NarrowSockException != NULL) {
        for (NARROWSOCKEXCEPTION *exception = NarrowSockException; exception->level_name != NULL; ++exception) {
            if (exception->level_index == -1 || exception->level_index != level_index ||
                exception->sock_index != object->sock_position.location.sock)
                continue;
            if (object->sock_position.distance >= exception->start_distance &&
                object->sock_position.distance <= exception->end_distance)
                return 0;
        }
    }
    return 1;
}

void GameCam_ResetLookRot(GAMECAMERA_s *camera) {
    if (camera == NULL) {
        camera = GameCam;
    }

    camera->field_0x214 = 0.0f;
    camera->field_0x218 = 0.0f;
    camera->field_0x20c = 0.0f;
    camera->field_0x210 = 0.0f;
    camera->field_0x204 = 0.0f;
    camera->field_0x208 = 0.0f;
}

void GameCam_UpdateLookRot(GAMECAMERA_s *camera) {
    // Retail handles each player separately, with directional buttons only for
    // the left-stick mode and seek speeds proportional to the angular ranges.
    if (camera == NULL)
        camera = GameCam;
    f32 look_pitch = 1820.0f, look_yaw = 2730.0f;
    camera->field_0x204 = 0.0f;
    camera->field_0x208 = 0.0f;
    f32 contributing_players = 0.0f;
    if (MiniCutCam == 0) {
        if (Player[0] != NULL && static_cast<i8>(Player[0]->apiobj.flags_low) < 0 &&
            Player[0]->pad_gamepad->pad != NULL) {
            i32 look_source;
            if (__builtin_expect(GameCam_ObjLookingWithLeftStick == NULL, 0))
                goto player0_right_look;
            look_source = GameCam_ObjLookingWithLeftStick(Player[0]);
            if (look_source != 1 && look_source != 2)
                goto player0_look_done;
            if (look_source == 2) {
                f32 horizontal = 0.0f, vertical = 0.0f;
                if (Player[0]->pad_gamepad->input_mode == 1) {
                    u32 buttons = Player[0]->pad_gamepad->buttons_held;
                    u32 horizontal_buttons = buttons & (GAMEPAD_DLEFT | GAMEPAD_DRIGHT);
                    if (horizontal_buttons == GAMEPAD_DLEFT)
                        horizontal = -look_yaw;
                    else if (horizontal_buttons == GAMEPAD_DRIGHT)
                        horizontal = look_yaw;
                    u32 vertical_buttons = buttons & (GAMEPAD_DUP | GAMEPAD_DDOWN);
                    if (vertical_buttons == GAMEPAD_DUP)
                        vertical = -look_pitch;
                    else if (__builtin_expect(vertical_buttons == GAMEPAD_DDOWN, 0))
                        vertical = look_pitch;
                } else {
                    nupad_s *pad = Player[0]->pad_gamepad->pad;
                    vertical = (static_cast<f32>(static_cast<u32>(pad->analog_left_y)) - 127.5f) * (1.0f / 127.5f) *
                               look_pitch;
                    horizontal =
                        (static_cast<f32>(static_cast<u32>(pad->analog_left_x)) - 127.5f) * (1.0f / 127.5f) * look_yaw;
                }
                camera->field_0x204 += vertical;
                camera->field_0x208 += horizontal;
            } else if (look_source == 1) {
            player0_right_look:
                nupad_s *pad = Player[0]->pad_gamepad->pad;
                f32 horizontal = static_cast<f32>(static_cast<u32>(pad->analog_right_x));
                f32 vertical = static_cast<f32>(static_cast<u32>(pad->analog_right_y));
                camera->field_0x208 += (horizontal - 127.5f) * (1.0f / 127.5f) * look_yaw;
                camera->field_0x204 += (vertical - 127.5f) * (1.0f / 127.5f) * look_pitch;
            }
            contributing_players += 1.0f;
        player0_look_done:;
        }
        if (Player[1] != NULL && static_cast<i8>(Player[1]->apiobj.flags_low) < 0 &&
            Player[1]->pad_gamepad->pad != NULL) {
            i32 look_source;
            if (GameCam_ObjLookingWithLeftStick == NULL)
                goto player1_right_look;
            look_source = GameCam_ObjLookingWithLeftStick(Player[1]);
            if (look_source != 1 && look_source != 2)
                goto player1_look_done;
            if (look_source == 2) {
                f32 horizontal = 0.0f, vertical = 0.0f;
                if (Player[1]->pad_gamepad->input_mode == 1) {
                    u32 buttons = Player[1]->pad_gamepad->buttons_held;
                    u32 horizontal_buttons = buttons & (GAMEPAD_DLEFT | GAMEPAD_DRIGHT);
                    if (horizontal_buttons == GAMEPAD_DLEFT)
                        horizontal = -look_yaw;
                    else if (horizontal_buttons == GAMEPAD_DRIGHT)
                        horizontal = look_yaw;
                    u32 vertical_buttons = buttons & (GAMEPAD_DUP | GAMEPAD_DDOWN);
                    if (vertical_buttons == GAMEPAD_DUP)
                        vertical = -look_pitch;
                    else if (vertical_buttons == GAMEPAD_DDOWN)
                        vertical = look_pitch;
                } else {
                    nupad_s *pad = Player[1]->pad_gamepad->pad;
                    vertical = (static_cast<f32>(static_cast<u32>(pad->analog_left_y)) - 127.5f) * (1.0f / 127.5f) *
                               look_pitch;
                    horizontal =
                        (static_cast<f32>(static_cast<u32>(pad->analog_left_x)) - 127.5f) * (1.0f / 127.5f) * look_yaw;
                }
                camera->field_0x204 += vertical;
                camera->field_0x208 += horizontal;
            } else if (look_source == 1) {
            player1_right_look:
                nupad_s *pad = Player[1]->pad_gamepad->pad;
                f32 horizontal = static_cast<f32>(static_cast<u32>(pad->analog_right_x));
                f32 vertical = static_cast<f32>(static_cast<u32>(pad->analog_right_y));
                camera->field_0x208 += (horizontal - 127.5f) * (1.0f / 127.5f) * look_yaw;
                camera->field_0x204 += (vertical - 127.5f) * (1.0f / 127.5f) * look_pitch;
            }
            contributing_players += 1.0f;
        player1_look_done:;
        }
    }
    if (contributing_players > 1.0f) {
        f32 inverse_count = 1.0f / contributing_players;
        camera->field_0x204 *= inverse_count;
        camera->field_0x208 *= inverse_count;
    }
    camera->field_0x20c = SeekLinearF(camera->field_0x20c, camera->field_0x204, (FRAMETIME * look_pitch) * 2.0f);
    camera->field_0x210 = SeekLinearF(camera->field_0x210, camera->field_0x208, (FRAMETIME * look_yaw) * 2.0f);
    camera->field_0x214 = SeekValF(camera->field_0x214, camera->field_0x20c, 3.0f);
    camera->field_0x218 = SeekValF(camera->field_0x218, camera->field_0x210, 3.0f);
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

void GameCam_NewShake(GAMECAMERA_s *camera, float amount, float duration, float speed) {
    if (camera == NULL) {
        camera = GameCam;
    }
    camera->shake_target_amount = amount;
    camera->shake_time = duration;
    camera->shake_speed = speed;
}

void GameCam_UpdateShake(GAMECAMERA_s *camera, float ambient_amount) {
    if (camera == NULL) {
        camera = GameCam;
    }

    if (camera->shake_time > 0.0f) {
        camera->shake_time -= FRAMETIME;
    }
    f32 target_amount = 0.0f;
    if (camera->shake_time > 0.0f)
        target_amount = camera->shake_target_amount;
    else if (ambient_amount > 0.0f)
        target_amount = ambient_amount;
    camera->shake_amplitude = SeekLinearF(camera->shake_amplitude, target_amount, FRAMETIME * 2.0f);

    constexpr f32 kShakeRadius = 0.1f;
    constexpr f32 kShakeRetargetDistanceSquared = 0.025f * 0.025f;
    if (NuVecDistSqr(&camera->shake_direction, &camera->shake_target, NULL) < kShakeRetargetDistanceSquared) {
        camera->shake_target.x = 0.0f;
        camera->shake_target.y = static_cast<f32>(qrand()) * (1.0f / 65535.0f) * kShakeRadius;
        camera->shake_target.z = 0.0f;
        NuVecRotateZ(&camera->shake_target, &camera->shake_target, static_cast<NUANG>(qrand()));
        camera->shake_target.x *= 4.0f / 3.0f;
    }

    SeekVec(&camera->shake_direction, &camera->shake_direction, &camera->shake_target,
            camera->shake_time > 0.0f ? camera->shake_speed * 10.0f : 10.0f);
    SeekVec(&camera->shake_offset, &camera->shake_offset, &camera->shake_direction,
            camera->shake_time > 0.0f ? camera->shake_speed * 2.0f : 2.0f);

    NUVEC scaled_offset;
    NuVecScale(&scaled_offset, &camera->shake_offset, camera->shake_amplitude);
    constexpr f32 kShakeAngleScale = 910.0f;
    const i32 pitch = static_cast<i32>(kShakeAngleScale * camera->shake_amplitude * (scaled_offset.y / kShakeRadius));
    NuMtxPreRotateX(&camera->render_mtx, pitch);
    const i32 yaw = static_cast<i32>(kShakeAngleScale * camera->shake_amplitude * (scaled_offset.x / kShakeRadius));
    NuMtxPreRotateY(&camera->render_mtx, yaw);
}

void GameCam_Judder(GAMECAMERA_s *camera, float amount, i32 axis, nuvec_s *source) {
    if (camera == NULL)
        camera = GameCam;
    const f32 absolute_amount = NuFabs(amount);
    if (!(absolute_amount > camera->judder_time))
        return;
    if (source != NULL) {
        const f32 distance = NuVecDist(&camera->pos, source, NULL);
        const f32 maximum_distance = static_cast<f32>(static_cast<u8>(WORLD->current_level->camera_judder_distance));
        if (maximum_distance > distance) {
            const f32 attenuated_amount = ((maximum_distance - distance) / maximum_distance) * absolute_amount;
            if (attenuated_amount > camera->judder_time) {
                camera->judder_reverse = amount < 0.0f;
                camera->judder_duration = attenuated_amount;
                camera->judder_time = attenuated_amount;
            }
        }
    } else {
        camera->judder_duration = absolute_amount;
        camera->judder_time = absolute_amount;
        camera->judder_reverse = amount < 0.0f;
    }
    camera->judder_axis = static_cast<u8>(axis);
}

void GameCam_HitJudder() {
    f32 amount = VehicleArea != 0 ? -0.3f : -0.2f;
    if (qrand() <= 0x7fff) {
        amount = -amount;
    }
    GameCam_Judder(GameCam, amount, qrand() / 0x5556, NULL);
}

void GameCam_HitRoll() {
    const f32 amount = qrand() <= 0x7fff ? -0.25f : 0.25f;
    GameCam_Judder(GameCam, amount, 2, NULL);
}

void SetCameraMatrices() {
    NuRndrLightingStateCurrent.field_0x60 = 1;
    NuRndrLightingStateCurrent.field_0x74 = 0;
    NuRndrSetSpecularLightPS(NULL, NULL);

    NUMTX effect_matrix;
    NUVEC scale = {1.0f, 1.0f, 1.0f};
    NuMtxInvR(&effect_matrix, &global_camera.mtx);
    NuMtxScale(&effect_matrix, &scale);
    effect_matrix.m30 = 1.0f;
    effect_matrix.m31 = 1.0f;
    effect_matrix.m33 = 1.0f;
    effect_matrix.m32 = 0.0f;
    effect_matrix.m23 = 0.0f;
    effect_matrix.m13 = 0.0f;
    effect_matrix.m03 = 0.0f;
    NuRndrSetFxMtx(&effect_matrix);
}

void CentreTwoPlayerCamera(nuvec_s *center, nuvec_s *player_a, nuvec_s *player_b, nuvec_s *reference) {
    const f32 distance_a = NuVecDist(reference, player_a, NULL);
    const f32 distance_b = NuVecDist(reference, player_b, NULL);
    const f32 blend = distance_a / (distance_a + distance_b);
    center->x = player_a->x + (player_b->x - player_a->x) * blend;
    center->y = player_a->y + (player_b->y - player_a->y) * blend;
    center->z = player_a->z + (player_b->z - player_a->z) * blend;
}

void NarrowSockExceptions_Init(NARROWSOCKEXCEPTION *exceptions) {
    i32 level_index __attribute__((aligned(16)));
    if (exceptions == NULL)
        return;
    NarrowSockException = exceptions;
    for (; exceptions->level_name != NULL; ++exceptions) {
        Level_FindByName(exceptions->level_name, &level_index);
        exceptions->level_index = level_index;
    }
}

i32 ObjInNarrowSock(GameObject_s *object, SOCKSYS *system, i32 level_index) {
    if ((WORLD->current_level->flags & LEVEL_NARROW_SOCKS) == 0 && !VehicleArea)
        return 0;
    if (disable_narrow_socks || object->apiobj.field_0x27c == -1)
        return 0;
    if (object->apiobj.player_controlled) {
        GameObject_s *other = NULL;
        if (object == Player[0])
            other = Player[1];
        else if (object == Player[1])
            other = Player[0];
        if (other != NULL && other->apiobj.player_controlled && !NarrowSockPositionAllowed(other, system, level_index))
            return 0;
    }
    return NarrowSockPositionAllowed(object, system, level_index);
}

void GameCam_Reset(GAMECAMERA_s *camera) {
    if (camera == NULL) {
        camera = GameCam;
    }

    camera->sock_position.location.sock = -1;
    camera->sock_position.location.segment = -1;
    camera->previous_mode = -1;
    camera->judder_time = 0.0f;
    ObstacleCamSpl = NULL;
    camera->shake_amplitude = 0.0f;
    MiniCutCam = 0;
    camera->shake_target_amount = 0.0f;
    camera->mode = -1;
    camera->shake_time = 0.0f;
    camera->shake_speed = 1.0f;
    camera->blend_duration = 0.0f;
    camera->blend_time = 0.0f;
    camera->position_seek = static_cast<f32>(static_cast<u8>(WORLD->current_level->cam_pos_seek));
    camera->field_0x1b8 = 0.0f;
    camera->angle_seek = static_cast<f32>(static_cast<u8>(WORLD->current_level->cam_angle_seek));
    camera->field_0x1ec = 0.0f;
    camera->field_0x1e8 = 0.0f;
    camera->field_0x1f4 = 0.0f;
    camera->field_0x1f0 = 0.0f;
    GameCam_ResetLookRot(camera);
    Minicam_InitSystem();
}

void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode) {
    if (camera == NULL) {
        camera = GameCam;
    }
    if (!(duration > 0.0f) || camera->mode == -1) {
        return;
    }

    camera->blend_start_pitch = camera->desired_pitch;
    camera->blend_start_yaw = camera->desired_yaw;
    camera->blend_start_roll = camera->desired_roll;
    camera->blend_mode = mode == 0 ? 1 : 2;
    camera->previous_camera_mode = camera->previous_mode;
    camera->reset_blend = 1;
    camera->blend_curve = curve;
    camera->blend_time = 0.0f;
    camera->blend_duration = duration;
    camera->blend_start_position = camera->desired_position;
    camera->blend_start_target = camera->target;
    camera->blend_end_position = camera->desired_position;
    camera->blend_end_target = camera->target;
}

u16 GameCam_GetAdjustedYRot(GAMECAMERA_s *camera) {
    if (camera == NULL) {
        camera = GameCam;
    }
    return static_cast<u16>(camera->input_yaw + static_cast<i32>(camera->field_0x218));
}

