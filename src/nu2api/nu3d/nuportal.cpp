#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/android/nuportal_android.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"

#include <string.h>

static VARIPTR fstack;
static i8 fstackmem[12288];
static VARIPTR fstack_limit;
static VARIPTR fstack_top;

static NUCAMERA *cam;
static NUMTX local_inv_view_mtx;
static NUVEC world_campos;
static i16 camera_roomid;
static NUFRUSTRUM **frustra;
static i32 *nfrustra;
static i32 draw_portals;

NUPLANE cam_plane;
i16 rooms_visited[18];

static NUFRUSTRUM *allocateFrustrum(i16 plane_count, i16 room_id) {
    NUFRUSTRUM *frustum = reinterpret_cast<NUFRUSTRUM *>(ALIGN(fstack_top.addr, 16));
    frustum->planes = reinterpret_cast<NUPLANE *>(reinterpret_cast<u8 *>(frustum) + sizeof(NUFRUSTRUM));
    fstack_top.addr = reinterpret_cast<usize>(frustum->planes + plane_count);
    frustum->plane_count = plane_count;
    frustum->room_id = room_id;
    frustra[*nfrustra] = frustum;
    ++*nfrustra;
    return frustum;
}

static __used__ void transposeClipPlanes(NUFRUSTRUM *frustum) {
    f32 cam_a = cam_plane.a;
    frustum->transposed_planes[3] = cam_a;
    f32 cam_d = cam_plane.d;
    f32 cam_b = cam_plane.b;
    f32 cam_c = cam_plane.c;
    NUPLANE *planes = frustum->planes;
    frustum->transposed_planes[7] = cam_b;
    frustum->transposed_planes[11] = cam_c;
    frustum->transposed_planes[15] = cam_d;
    frustum->transposed_planes[16] = cam_a;
    frustum->transposed_planes[20] = cam_b;
    frustum->transposed_planes[24] = cam_c;
    frustum->transposed_planes[28] = cam_d;
    frustum->transposed_planes[17] = cam_a;
    frustum->transposed_planes[21] = cam_b;
    frustum->transposed_planes[25] = cam_c;
    frustum->transposed_planes[29] = cam_d;
    frustum->transposed_planes[18] = cam_a;
    frustum->transposed_planes[22] = cam_b;
    frustum->transposed_planes[26] = cam_c;
    frustum->transposed_planes[30] = cam_d;
    frustum->transposed_planes[19] = cam_a;
    frustum->transposed_planes[23] = cam_b;
    frustum->transposed_planes[27] = cam_c;
    frustum->transposed_planes[31] = cam_d;

    frustum->transposed_planes[0] = planes[0].a;
    frustum->transposed_planes[4] = planes[0].b;
    frustum->transposed_planes[8] = planes[0].c;
    frustum->transposed_planes[12] = planes[0].d;
    frustum->transposed_planes[1] = planes[1].a;
    frustum->transposed_planes[5] = planes[1].b;
    frustum->transposed_planes[9] = planes[1].c;
    frustum->transposed_planes[13] = planes[1].d;
    frustum->transposed_planes[2] = planes[2].a;
    frustum->transposed_planes[6] = planes[2].b;
    frustum->transposed_planes[10] = planes[2].c;
    frustum->transposed_planes[14] = planes[2].d;
    if (frustum->plane_count <= 3) {
        return;
    }
    frustum->transposed_planes[3] = planes[3].a;
    frustum->transposed_planes[7] = planes[3].b;
    frustum->transposed_planes[11] = planes[3].c;
    frustum->transposed_planes[15] = planes[3].d;
    if (frustum->plane_count == 4) {
        return;
    }
    frustum->transposed_planes[16] = planes[4].a;
    frustum->transposed_planes[20] = planes[4].b;
    frustum->transposed_planes[24] = planes[4].c;
    frustum->transposed_planes[28] = planes[4].d;
    if (frustum->plane_count == 5) {
        return;
    }
    frustum->transposed_planes[17] = planes[5].a;
    frustum->transposed_planes[21] = planes[5].b;
    frustum->transposed_planes[25] = planes[5].c;
    frustum->transposed_planes[29] = planes[5].d;
    if (frustum->plane_count == 6) {
        return;
    }
    frustum->transposed_planes[18] = planes[6].a;
    frustum->transposed_planes[22] = planes[6].b;
    frustum->transposed_planes[26] = planes[6].c;
    frustum->transposed_planes[30] = planes[6].d;
    if (frustum->plane_count == 7) {
        return;
    }
    frustum->transposed_planes[19] = planes[7].a;
    frustum->transposed_planes[23] = planes[7].b;
    frustum->transposed_planes[27] = planes[7].c;
    frustum->transposed_planes[31] = planes[7].d;
}

static void transformFrustrumPlane(NUPLANE *plane) {
    NuVec4MtxTransformH(reinterpret_cast<NUVEC4 *>(plane), reinterpret_cast<NUVEC4 *>(plane), &local_inv_view_mtx);
    plane->d = -(local_inv_view_mtx.m30 * plane->a + local_inv_view_mtx.m31 * plane->b +
                 local_inv_view_mtx.m32 * plane->c);
}

static __used__ NUFRUSTRUM *buildFrustrum(NUVEC *minimum, NUVEC *maximum, i16 room_id) {
    NUFRUSTRUM *frustum = allocateFrustrum(4, room_id);
    frustum->minimum = *minimum;
    frustum->maximum = *maximum;

    const f32 tangent = NU_TAN_LUT(cam->fov * 0.5f * 10430.378f);
    const f32 horizontal_tangent = tangent / cam->aspect;

    f32 edge = maximum->x * horizontal_tangent;
    f32 inverse_length = 1.0f / NuFsqrt(edge * edge + 1.0f);
    frustum->planes[0] = {-inverse_length, 0.0f, edge * inverse_length, 0.0f};

    edge = minimum->x * horizontal_tangent;
    inverse_length = 1.0f / NuFsqrt(edge * edge + 1.0f);
    frustum->planes[1] = {inverse_length, 0.0f, -edge * inverse_length, 0.0f};

    edge = minimum->y * tangent;
    inverse_length = 1.0f / NuFsqrt(edge * edge + 1.0f);
    frustum->planes[2] = {0.0f, inverse_length, -edge * inverse_length, 0.0f};

    edge = maximum->y * tangent;
    inverse_length = 1.0f / NuFsqrt(edge * edge + 1.0f);
    frustum->planes[3] = {0.0f, -inverse_length, edge * inverse_length, 0.0f};

    for (i32 i = 0; i < 4; ++i) {
        transformFrustrumPlane(&frustum->planes[i]);
    }
    transposeClipPlanes(frustum);
    return frustum;
}

static void computePortalExtents(NUPORTAL *portal, NUVEC *minimum, NUVEC *maximum) {
    NUMTX clip;
    NuCameraGetClipMtx(&clip, NULL);
    NuVecMtxTransformH(minimum, &portal->vertices[0], &clip);
    *maximum = *minimum;
    for (i32 i = 1; i < portal->vertex_count; ++i) {
        NUVEC point;
        NuVecMtxTransformH(&point, &portal->vertices[i], &clip);
        NuVecMax(maximum, maximum, &point);
        NuVecMin(minimum, minimum, &point);
    }
    const NUVEC minus_one = {-1.0f, -1.0f, -1.0f};
    const NUVEC one = {1.0f, 1.0f, 1.0f};
    NuVecMin(maximum, maximum, const_cast<NUVEC *>(&one));
    NuVecMax(maximum, maximum, const_cast<NUVEC *>(&minus_one));
    NuVecMin(minimum, minimum, const_cast<NUVEC *>(&one));
    NuVecMax(minimum, minimum, const_cast<NUVEC *>(&minus_one));
}

static f32 planeDistance(const NUPLANE &plane, const NUVEC &point) {
    return plane.a * point.x + plane.b * point.y + plane.c * point.z + plane.d;
}

static NUFRUSTRUM *buildPortalFrustrum(NUPORTAL *portal, i16 room_id) {
    NUFRUSTRUM *frustum = allocateFrustrum(portal->vertex_count, room_id);
    const f32 camera_side = planeDistance(portal->plane, world_campos);
    NUVEC camera_position = {
        local_inv_view_mtx.m30,
        local_inv_view_mtx.m31,
        local_inv_view_mtx.m32,
    };
    for (i32 i = 0; i < portal->vertex_count; ++i) {
        NUVEC first;
        NUVEC second;
        NUVEC normal;
        NuVecSub(&first, &camera_position, &portal->vertices[i]);
        NuVecSub(&second, &camera_position, &portal->vertices[(i + 1) % portal->vertex_count]);
        NuVecCross(&normal, &first, &second);
        NuVecNorm(&normal, &normal);
        if (camera_side >= 0.0f) {
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }
        frustum->planes[i].a = normal.x;
        frustum->planes[i].b = normal.y;
        frustum->planes[i].c = normal.z;
        frustum->planes[i].d =
            -(normal.x * camera_position.x + normal.y * camera_position.y + normal.z * camera_position.z);
    }
    computePortalExtents(portal, &frustum->minimum, &frustum->maximum);
    transposeClipPlanes(frustum);
    return frustum;
}

static NUFRUSTRUM *copyFrustrum(NUFRUSTRUM *source, i16 room_id) {
    NUFRUSTRUM *copy = allocateFrustrum(source->plane_count, room_id);
    memcpy(copy->transposed_planes, source->transposed_planes, sizeof(copy->transposed_planes));
    copy->minimum = source->minimum;
    copy->maximum = source->maximum;
    memcpy(copy->planes, source->planes, sizeof(NUPLANE) * source->plane_count);
    return copy;
}

static __used__ void roomRecursive(NUGSCN *scene, NUFRUSTRUM *frustum, i16 room_id, i16 previous_room, i32 depth) {
    if (depth >= 2) {
        for (i32 i = 0; i < depth - 1; ++i) {
            if (rooms_visited[i] == room_id) {
                return;
            }
        }
    }
    if (depth >= 16) {
        return;
    }

    NUROOM *room = &scene->rooms[room_id];
    rooms_visited[depth] = room_id;
    if (room_id == camera_roomid) {
        FlagRoomInstancesAsVisible(room, scene);
    } else {
        clipRoomAgainstFrustrum(scene, room, frustum);
    }
    room->flags |= NUROOM_FLAG_VISITED;
    if (depth >= scene->portal_depth || room->portal_count == 0) {
        return;
    }

    for (i32 portal_index = 0; portal_index < room->portal_count; ++portal_index) {
        NUPORTAL *portal = &scene->portals[room->portal_indices[portal_index]];
        i16 next_room = portal->front_room == room_id ? portal->back_room : portal->front_room;
        if (next_room == previous_room || (portal->is_active & NUPORTAL_FLAG_ACTIVE) == 0) {
            continue;
        }

        NUROOM *next = &scene->rooms[next_room];
        const f32 camera_side = planeDistance(portal->plane, world_campos);
        if (next->priority > room->priority && ((room_id == portal->back_room && camera_side < 0.0f) ||
                                                (room_id == portal->front_room && camera_side > 0.0f))) {
            continue;
        }

        i32 inside_tests = 0;
        bool rejected = false;
        for (i32 plane_index = 0; plane_index < frustum->plane_count; ++plane_index) {
            i32 outside_vertices = 0;
            for (i32 vertex_index = 0; vertex_index < portal->vertex_count; ++vertex_index) {
                if (planeDistance(frustum->planes[plane_index], portal->vertices[vertex_index]) < 0.0f) {
                    ++outside_vertices;
                } else {
                    ++inside_tests;
                }
            }
            if (outside_vertices == portal->vertex_count) {
                rejected = true;
                break;
            }
        }
        if (rejected) {
            continue;
        }

        NUFRUSTRUM *next_frustum = NULL;
        if (inside_tests == frustum->plane_count * portal->vertex_count) {
            next_frustum = buildPortalFrustrum(portal, next_room);
        } else {
            i32 in_front = 0;
            for (i32 i = 0; i < portal->vertex_count; ++i) {
                if (planeDistance(cam_plane, portal->vertices[i]) > 0.0f) {
                    ++in_front;
                }
            }
            if (in_front == 0) {
                continue;
            }
            if (in_front == portal->vertex_count) {
                NUVEC portal_minimum;
                NUVEC portal_maximum;
                computePortalExtents(portal, &portal_minimum, &portal_maximum);
                if (frustum->minimum.x >= portal_maximum.x || frustum->minimum.y >= portal_maximum.y ||
                    portal_minimum.x >= frustum->maximum.x || portal_minimum.y >= frustum->maximum.y) {
                    continue;
                }
                portal_minimum.x = MAX(portal_minimum.x, frustum->minimum.x);
                portal_minimum.y = MAX(portal_minimum.y, frustum->minimum.y);
                portal_maximum.x = MIN(portal_maximum.x, frustum->maximum.x);
                portal_maximum.y = MIN(portal_maximum.y, frustum->maximum.y);
                next_frustum = buildFrustrum(&portal_minimum, &portal_maximum, next_room);
            } else {
                next_frustum = copyFrustrum(frustum, next_room);
            }
        }
        roomRecursive(scene, next_frustum, next_room, room_id, depth + 1);
    }
}

void NuPortalInit(void) {
    fstack.void_ptr = fstackmem;
    fstack_top.addr = ALIGN((usize)fstackmem, 16);
    fstack_limit.void_ptr = fstackmem + 0x3000;
}

void NuPortalSetActiveDirect(NUPORTAL *portal, i32 active) {
    if (portal == NULL) {
        return;
    }

    if (active) {
        portal->is_active |= NUPORTAL_FLAG_ACTIVE;
    } else {
        portal->is_active = 0;
    }
}

void NuPortalSetActive(NUGSCN *scene, i32 portal_id, i32 active) {
    if (scene->max_portals != 0) {
        NUPORTAL *portal = scene->portals;
        if (active == 0) {
            for (u32 index = 0; index < scene->max_portals; index++, portal++) {
                if (portal->id == portal_id) {
                    portal->is_active = 0;
                }
            }
        } else {
            for (u32 index = 0; index < scene->max_portals; index++, portal++) {
                if (portal->id == portal_id) {
                    portal->is_active |= NUPORTAL_FLAG_ACTIVE;
                }
            }
        }
    }
}

void NuPortalMaxDepth(struct nugscn_s *scene, i32 depth) {
    scene->portal_depth = depth;
}

static NUVEC *override_campos;
extern i32 portals_enabled;
static NUPLANE near_clip_plane;
extern "C" i32 NuPortalVisibility(NUGSCN *scene) {
    Initialise_PS(scene);
    NUVEC *camera_position = override_campos != NULL ? override_campos : &world_campos;
    if (portals_enabled == 0) {
        SetAllInstancesVisible(scene);
        return 0;
    }
    if (scene->max_portals == 0 || scene->num_rooms == 0 || scene->portal_instance_count == 0) {
        return 0;
    }

    scene->num_portal_frusta = 0;
    nfrustra = &scene->num_portal_frusta;
    frustra = scene->portal_frusta;
    SetAllInstancesHidden(scene);
    cam = NuCameraGetCam();
    local_inv_view_mtx = *NuCameraGetMtx();
    world_campos = {
        global_camera.mtx.m30,
        global_camera.mtx.m31,
        global_camera.mtx.m32,
    };

    camera_roomid = static_cast<i16>(NuPortalWhichRoom(scene, camera_position));
    scene->camera_room = camera_roomid;
    if (camera_roomid == -1) {
        return 0;
    }

    const f32 forward_x = local_inv_view_mtx.m20;
    const f32 forward_y = local_inv_view_mtx.m21;
    const f32 forward_z = local_inv_view_mtx.m22;
    cam_plane.a = forward_x;
    cam_plane.b = forward_y;
    cam_plane.c = forward_z;
    cam_plane.d = -(world_campos.x * forward_x + world_campos.y * forward_y + world_campos.z * forward_z);

    near_clip_plane.a = forward_x;
    near_clip_plane.b = forward_y;
    near_clip_plane.c = forward_z;
    near_clip_plane.d = -((world_campos.x + cam->near_clip * forward_x) * forward_x +
                          (world_campos.y + cam->near_clip * forward_y) * forward_y +
                          (world_campos.z + cam->near_clip * forward_z) * forward_z);

    NUVEC minimum = {-1.0f, -1.0f, -1.0f};
    NUVEC maximum = {1.0f, 1.0f, 1.0f};
    NUFRUSTRUM *frustum = buildFrustrum(&minimum, &maximum, -2);
    for (i32 i = 0; i < scene->num_rooms; ++i) {
        scene->rooms[i].flags &= ~NUROOM_FLAG_VISITED;
    }
    roomRecursive(scene, frustum, camera_roomid, -1, 0);
    return 1;
}

void NuPortalSetOverride(NUVEC *position) {
    override_campos = position;
}

void NuPortalEnableDebugDraw(i32 enabled) {
    draw_portals = enabled;
}

extern "C" i32 clipTestSphere(NUPORTALSPHERE *sphere, NUFRUSTRUM *frustum) {
    i32 fully_inside = 0;
    for (i32 i = 0; i < frustum->plane_count; ++i) {
        const NUPLANE &plane = frustum->planes[i];
        const f32 distance =
            plane.a * sphere->center.x + plane.b * sphere->center.y + plane.c * sphere->center.z + plane.d;
        if (distance < -sphere->radius) {
            return 0;
        }
        if (distance > sphere->radius) {
            ++fully_inside;
        }
    }
    const f32 camera_distance =
        cam_plane.a * sphere->center.x + cam_plane.b * sphere->center.y + cam_plane.c * sphere->center.z + cam_plane.d;
    if (camera_distance < -sphere->radius) {
        return 0;
    }
    if (camera_distance > sphere->radius) {
        ++fully_inside;
    }
    return fully_inside == frustum->plane_count + 1 ? 1 : 2;
}

extern "C" i32 clipTestBox(NUVEC *minimum, NUVEC *maximum, NUPLANE *planes, i32 plane_count) {
    NUVEC min = *minimum;
    NUVEC max = *maximum;
    i32 inside_vertices = 0;
    for (i32 plane_index = 0; plane_index < plane_count; ++plane_index) {
        const NUPLANE &plane = planes[plane_index];
        i32 inside_plane = 0;
        if (!(plane.a * min.x + plane.b * min.y + plane.c * min.z + plane.d < 0.0f))
            ++inside_plane, ++inside_vertices;
        if (!(plane.a * max.x + plane.b * min.y + plane.c * min.z + plane.d < 0.0f))
            ++inside_plane, ++inside_vertices;
        if (!(plane.a * max.x + plane.b * max.y + plane.c * min.z + plane.d < 0.0f))
            ++inside_plane, ++inside_vertices;
        if (!(plane.a * min.x + plane.b * max.y + plane.c * min.z + plane.d < 0.0f))
            ++inside_plane, ++inside_vertices;
        if (!(plane.a * min.x + plane.b * min.y + plane.c * max.z + plane.d < 0.0f))
            ++inside_plane, ++inside_vertices;
        if (!(plane.a * max.x + plane.b * min.y + plane.c * max.z + plane.d < 0.0f))
            ++inside_plane, ++inside_vertices;
        if (!(plane.a * max.x + plane.b * max.y + plane.c * max.z + plane.d < 0.0f))
            ++inside_plane, ++inside_vertices;
        if (!(plane.a * min.x + plane.b * max.y + plane.c * max.z + plane.d < 0.0f))
            ++inside_plane, ++inside_vertices;
        if (inside_plane == 0) {
            return 0;
        }
    }
    return inside_vertices == plane_count * 8 ? 1 : 2;
}

extern "C" i32 NuPortalClipTestBox(NUVEC *center, NUVEC *extent, NUFRUSTRUM *frustum) {
    for (i32 i = 0; i < frustum->plane_count; ++i) {
        const NUPLANE &plane = frustum->planes[i];
        const f32 distance = plane.a * center->x + plane.b * center->y + plane.c * center->z + plane.d;
        const f32 radius = NuFabs(plane.a) * extent->x + NuFabs(plane.b) * extent->y + NuFabs(plane.c) * extent->z;
        if (distance < -radius) {
            return 0;
        }
    }
    return 1;
}
