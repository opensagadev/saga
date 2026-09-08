#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "legoapi/items/base/apiobject.h"
#include "gameapi/ai/aisys/aisys.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"

#include <string.h>
struct nuhspecial_s;
struct minitrooperteam_s;
struct nuvec_s;

extern "C" void *AISysBufferAlloc(VARIPTR *cursor, VARIPTR *buf_end, u32 size);
void AIPathCnxControlSysReset(AIPATHCNXCONTROLSYS_s *system);
extern "C" void *AISysLoadEx(void *buf, void *buf_end, i32 size, void *gscn, char *dir, char *name, char *param,
                             char *load_dir);

extern "C" void *AISysLoad(void *buf, void *buf_end, i32 size, void *gscn, char *dir, char *name, char *param) {
    return AISysLoadEx(buf, buf_end, size, gscn, dir, name, param, dir);
}
void *AIPathCnxControlSysCreate(VARIPTR *buf, VARIPTR *buf_end, i32 count) {
    AIPATHCNXCONTROLSYS_s *system =
        static_cast<AIPATHCNXCONTROLSYS_s *>(AISysBufferAlloc(buf, buf_end, sizeof(AIPATHCNXCONTROLSYS_s)));
    if (system != NULL) {
        memset(system, 0, sizeof(*system));
        system->controllers = static_cast<AIPATHCNXCONTROLLER_s *>(
            AISysBufferAlloc(buf, buf_end, static_cast<u32>(count) * sizeof(AIPATHCNXCONTROLLER_s)));
        if (system->controllers != NULL) {
            system->controller_count = count;
            AIPathCnxControlSysReset(system);
        }
    }
    return system;
}
void *AIPathCnxHelperSysCreate(VARIPTR *buf, VARIPTR *buf_end, i32 count) {
    if (count == 0) {
        return NULL;
    }
    AIPATHCNXHELPERSYS_s *system =
        static_cast<AIPATHCNXHELPERSYS_s *>(AISysBufferAlloc(buf, buf_end, sizeof(AIPATHCNXHELPERSYS_s)));
    if (system != NULL) {
        system->helpers = static_cast<AIPATHCNXHELPER_s *>(
            AISysBufferAlloc(buf, buf_end, static_cast<u32>(count) * sizeof(AIPATHCNXHELPER_s)));
        if (system->helpers != NULL) {
            system->field_0x00 = static_cast<i16>(count);
        }
    }
    return system;
}
void *AITriggerSetSysCreate(VARIPTR *buf, VARIPTR *buf_end) {
    (void)buf;
    (void)buf_end;
    return NULL;
}
void GameAIScriptAddLevelSfx(WORLDINFO *world, NULISTHDR *scripts) {
    (void)world;
    (void)scripts;
}
void *CreateClimbObjectSys(VARIPTR *buf, VARIPTR *buf_end, i32 count) {
    (void)buf;
    (void)buf_end;
    (void)count;
    return NULL;
}
extern "C" APIOBJECTSYS_s *APIObjectSysInit(i32 size, VARIPTR *buf, VARIPTR *buf_end) {
    APIOBJECTSYS_s *system = static_cast<APIOBJECTSYS_s *>(AISysBufferAlloc(buf, buf_end, sizeof(APIOBJECTSYS_s)));
    if (system == NULL) {
        return NULL;
    }

    memset(system, 0, sizeof(*system));
    if (size != 0) {
        system->objects = static_cast<APIOBJECT *>(AISysBufferAlloc(buf, buf_end, static_cast<u32>(size) * 64));
        if (system->objects != NULL) {
            system->object_size = static_cast<u32>(size);
            memset(system->objects, 0, static_cast<u32>(size) * 64);
        }
    }
    return system;
}

static __used__ unsigned int CalculateIntersection(AISYS_s *, AIPACKET_s *, APIOBJECT_s *, AIPATHCNX_s *,
                                                   AIPATHCNX_s *) {
    return {};
}

static __used__ unsigned int AISysCharacterTestPathCnx(AISYS_s *, APIOBJECT_s *, AIPACKET_s *, AIPATHCNX_s *, int,
                                                       float *) {
    return {};
}

extern "C" {
    f32 antinode_time = 1.0f;
    f32 antinode_reverse_time = 1.0f;
    f32 ai_moveradius = 0.1f;
}

static __used__ void AISysCheckAntinode_Circle(APIOBJECT_s *object, AIANTINODE_s *node, NUVEC *offset, f32 radius) {
    f32 distance_squared = offset->x * offset->x + offset->z * offset->z;
    if (!(radius * radius > distance_squared))
        return;
    offset->x = object->position.x - node->position.x;
    offset->z = object->position.z - node->position.z;
    i32 start_angle = NuAtan2D(offset->x, offset->z);
    offset->x = object->ai->movement_destination.x - node->position.x;
    offset->z = object->ai->movement_destination.z - node->position.z;
    i32 delta = NuAngSub(NuAtan2D(offset->x, offset->z), start_angle);
    if (object->ai->antinode_timer > 0.0f) {
        if (((object->ai->path_info.flags & 3) == 2 || object->respawn_timer > 1.0f) &&
            antinode_time > object->ai->antinode_timer) {
            object->ai->field_0x1e5 ^= 0x80;
            object->ai->antinode_timer = antinode_time + antinode_reverse_time;
        }
        if ((object->ai->field_0x1e5 & 0x80) != 0) {
            if (delta < 0)
                delta += 0x10000;
        } else if (delta > 0)
            delta -= 0x10000;
    } else {
        if (delta > 0)
            object->ai->field_0x1e5 |= 0x80;
        else
            object->ai->field_0x1e5 &= ~0x80;
    }
    if ((node->game_flags & 2) == 0 && antinode_time > object->ai->antinode_timer)
        object->ai->antinode_timer = antinode_time;
    i32 turn_limit = static_cast<i32>((ai_moveradius / radius) * 10430.3779296875f);
    if (delta > turn_limit)
        delta = turn_limit;
    else if (delta < -turn_limit)
        delta = -turn_limit;
    i32 angle = NuAngAdd(start_angle, delta);
    offset->x = 0.0f;
    offset->y = 0.0f;
    offset->z = radius;
    NuVecRotateY(offset, offset, angle);
    object->ai->movement_position.x = offset->x + node->position.x;
    object->ai->movement_position.y = object->ai->movement_destination.y;
    object->ai->movement_position.z = offset->z + node->position.z;
    object->field_0x1fa |= 4;
}

static __used__ void AISysCheckAntinode_Ellipse(APIOBJECT_s *object, AIANTINODE_s *node, NUVEC *offset, f32 radius) {
    f32 distance_squared = offset->x * offset->x + offset->z * offset->z;
    if (!(radius * radius > distance_squared))
        return;
    f32 ratio = node->base_height / node->base_radius;
    NUVEC local, boundary, start, start_boundary, destination;
    offset->y = 0.0f;
    NuVecRotateY(&local, offset, -node->flags);
    i32 angle = NuAtan2D(ratio * local.x, local.z);
    boundary.x = NU_SIN_LUT(angle) * node->base_radius;
    boundary.y = 0.0f;
    boundary.z = NU_COS_LUT(angle) * node->base_height;
    NuVecNorm(offset, &boundary);
    NuVecScale(offset, offset, object->ai->mover_height);
    NuVecAdd(&boundary, &boundary, offset);
    if (!(boundary.x * boundary.x + boundary.z * boundary.z > distance_squared))
        return;
    offset->x = object->position.x - node->position.x;
    offset->y = 0.0f;
    offset->z = object->position.z - node->position.z;
    NuVecRotateY(&start, offset, -node->flags);
    i32 start_angle = NuAtan2D(ratio * start.x, start.z);
    start_boundary.x = NU_SIN_LUT(start_angle) * node->base_radius;
    start_boundary.y = 0.0f;
    start_boundary.z = NU_COS_LUT(start_angle) * node->base_height;
    NuVecNorm(offset, &start_boundary);
    NuVecScale(offset, offset, object->ai->mover_height);
    NuVecAdd(&start_boundary, &start_boundary, offset);
    offset->x = object->ai->movement_destination.x - node->position.x;
    offset->y = 0.0f;
    offset->z = object->ai->movement_destination.z - node->position.z;
    NuVecRotateY(&destination, offset, -node->flags);
    i32 delta = NuAngSub(NuAtan2D(ratio * destination.x, destination.z), start_angle);
    if (object->ai->antinode_timer > 0.0f) {
        if (((object->ai->path_info.flags & 3) == 2 || object->respawn_timer > 1.0f) &&
            antinode_time > object->ai->antinode_timer) {
            object->ai->field_0x1e5 ^= 0x80;
            object->ai->antinode_timer = antinode_time + antinode_reverse_time;
        }
        if ((object->ai->field_0x1e5 & 0x80) != 0) {
            if (delta < 0)
                delta += 0x10000;
        } else if (delta > 0)
            delta -= 0x10000;
    } else {
        if (delta > 0)
            object->ai->field_0x1e5 |= 0x80;
        else
            object->ai->field_0x1e5 &= ~0x80;
    }
    if ((node->game_flags & 2) == 0 && antinode_time > object->ai->antinode_timer)
        object->ai->antinode_timer = antinode_time;
    f32 tangent_x = NU_COS_LUT(start_angle) * node->base_radius;
    f32 tangent_z = -NU_SIN_LUT(start_angle) * node->base_height;
    if (delta < 0) {
        tangent_x = -tangent_x;
        tangent_z = -tangent_z;
    }
    i32 magnitude = delta < 0 ? -delta : delta;
    if (magnitude < 1820) {
        f32 scale = static_cast<f32>(magnitude) / 1820.0f;
        tangent_x *= scale;
        tangent_z *= scale;
    }
    object->ai->movement_position.x = tangent_x + start_boundary.x;
    object->ai->movement_position.y = 0.0f;
    object->ai->movement_position.z = tangent_z + start_boundary.z;
    NuVecRotateY(&object->ai->movement_position, &object->ai->movement_position, node->flags);
    NuVecAdd(&object->ai->movement_position, &object->ai->movement_position, &node->position);
    object->field_0x1fa |= 4;
}

static __used__ void AISysCheckAntinode_Rectangle(APIOBJECT_s *object, AIANTINODE_s *node, NUVEC *offset, f32 radius) {
    offset->x = object->ai->movement_position.x - node->position.x;
    if (offset->x > radius || -radius > offset->x)
        return;
    offset->z = object->ai->movement_position.z - node->position.z;
    if (offset->z > radius || -radius > offset->z || object->collision_min.y > node->max_height ||
        node->height > object->collision_max.y)
        return;
    NuVecRotateY(offset, offset, -node->flags);
    if (!(node->base_radius + object->ai->mover_height > fabsf(offset->x)) ||
        !(object->ai->mover_height + node->base_height > fabsf(offset->z)))
        return;
    NUVEC start, destination;
    offset->x = object->position.x - node->position.x;
    offset->y = 0.0f;
    offset->z = object->position.z - node->position.z;
    NuVecRotateY(&start, offset, -node->flags);
    i32 start_angle = NuAtan2D(start.x, start.z);
    offset->x = object->ai->movement_destination.x - node->position.x;
    offset->y = 0.0f;
    offset->z = object->ai->movement_destination.z - node->position.z;
    NuVecRotateY(&destination, offset, -node->flags);
    i32 delta = NuAngSub(NuAtan2D(destination.x, destination.z), start_angle);
    i32 turn_wrap = 0;
    if (object->ai->antinode_timer > 0.0f) {
        if (((object->ai->path_info.flags & 3) == 2 || object->respawn_timer > 1.0f) &&
            antinode_time > object->ai->antinode_timer) {
            object->ai->field_0x1e5 ^= 0x80;
            object->ai->antinode_timer = antinode_time + antinode_reverse_time;
        }
        if ((object->ai->field_0x1e5 & 0x80) != 0) {
            if (delta < 0)
                turn_wrap = 0x10000;
        } else if (delta > 0)
            turn_wrap = -0x10000;
    } else {
        if (delta > 0)
            object->ai->field_0x1e5 |= 0x80;
        else
            object->ai->field_0x1e5 &= ~0x80;
    }
    if ((node->game_flags & 2) == 0 && antinode_time > object->ai->antinode_timer)
        object->ai->antinode_timer = antinode_time;
    i32 corner_angle = NuAtan2D(node->base_radius, node->base_height);
    i32 angle = NuAngAdd(start_angle, 0);
    f32 width = node->base_radius + object->ai->mover_height;
    f32 depth = object->ai->mover_height + node->base_height;
    f32 tangent_x, tangent_z;
    if (angle > 0x8000 - corner_angle || angle < corner_angle - 0x7fff) {
        object->ai->movement_position.x = width * NU_SIN_LUT(angle);
        object->ai->movement_position.z = -depth;
        tangent_x = -1.0f;
        tangent_z = 0.0f;
    } else if (angle > corner_angle) {
        object->ai->movement_position.x = width;
        object->ai->movement_position.z = depth * NU_COS_LUT(angle);
        tangent_x = 0.0f;
        tangent_z = -1.0f;
    } else if (angle > -corner_angle) {
        object->ai->movement_position.x = width * NU_SIN_LUT(angle);
        object->ai->movement_position.z = depth;
        tangent_x = 1.0f;
        tangent_z = 0.0f;
    } else {
        object->ai->movement_position.x = -width;
        object->ai->movement_position.z = depth * NU_COS_LUT(angle);
        tangent_x = 0.0f;
        tangent_z = 1.0f;
    }
    if (delta + turn_wrap < 0) {
        tangent_x = -tangent_x;
        tangent_z = -tangent_z;
    }
    i32 magnitude = delta < 0 ? -delta : delta;
    if (magnitude < 1820) {
        f32 scale = static_cast<f32>(magnitude) / 1820.0f;
        tangent_x *= scale;
        tangent_z *= scale;
    }
    object->ai->movement_position.x += tangent_x;
    object->ai->movement_position.y = 0.0f;
    object->ai->movement_position.z += tangent_z;
    NuVecRotateY(&object->ai->movement_position, &object->ai->movement_position, node->flags);
    NuVecAdd(&object->ai->movement_position, &object->ai->movement_position, &node->position);
    object->field_0x1fa |= 4;
}

extern "C" {
    void (*checkantinodefns[3])(APIOBJECT_s *, AIANTINODE_s *, NUVEC *, f32) = {
        AISysCheckAntinode_Circle, AISysCheckAntinode_Ellipse, AISysCheckAntinode_Rectangle};
}

static __used__ void FormationMove(AIGROUP_s *, int (*)(AIGROUP_s *, AIROW_s *, AIROW_s *, APIOBJECT_s *)) {
}

static __used__ void *GetNextConnection(AIPACKET_s const *, int *) {
    return nullptr;
}

static __used__ void GenerateTrooperTeamShape(minitrooperteam_s *, int) {
}
