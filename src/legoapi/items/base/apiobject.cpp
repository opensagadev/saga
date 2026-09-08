#include "decomp.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/characters/core/character.h"
#include "nu2api/nucore/nuhgobj.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

#include <string.h>

static f32 MINFORCEAPART = 0.333f;
static f32 MAXFORCEAPART = 0.666f;

static void GameObjectForceApart2D(APIOBJECT *first, APIOBJECT *second) {
    const u16 angle = static_cast<u16>(static_cast<i32>(NuRandFloat() * 65536.0f));

    if ((first->flags_low & 2) != 0 && (second->flags_low & 2) == 0) {
        second->velocity.x = NuTrigTable[static_cast<u16>(angle >> 1)] * 4.0f;
        second->velocity.z = NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff] * 4.0f;
        second->movement_direction.x = second->velocity.x;
        second->movement_direction.z = second->velocity.z;
        first->movement_direction.x = first->velocity.x;
        first->movement_direction.z = first->velocity.z;
    } else if ((first->flags_low & 2) == 0 && (second->flags_low & 2) != 0) {
        first->velocity.x = NuTrigTable[static_cast<u16>(angle >> 1)] * 4.0f;
        first->velocity.z = NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff] * 4.0f;
        first->movement_direction.x = first->velocity.x;
        first->movement_direction.z = first->velocity.z;
        second->movement_direction.x = second->velocity.x;
        second->movement_direction.z = second->velocity.z;
    } else {
        first->velocity.x = NuTrigTable[static_cast<u16>(angle >> 1)] * 2.0f;
        first->velocity.z = NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff] * 2.0f;
        first->movement_direction.x = first->velocity.x;
        first->movement_direction.z = first->velocity.z;
        second->velocity.x = -first->velocity.x;
        second->velocity.z = -first->velocity.z;
        second->movement_direction.x = second->velocity.x;
        second->movement_direction.z = second->velocity.z;
    }
}

extern "C" {

    i32 APIObjectCollision(APIOBJECT *first, APIOBJECT *second) {
        f32 value;
        f32 first_weight;
        f32 second_weight;
        i32 already_touching;
        NUVEC *first_position = &first->initial_position;
        NUVEC *second_position = &second->initial_position;
        f32 first_speed;
        f32 second_speed;
        f32 first_dot;
        f32 second_dot;
        f32 force;
        NUVEC normal;
        NUVEC first_component;
        NUVEC second_component;
        NUVEC average;
        already_touching =
            ((first->field_0x1ec & second->field_0x1e4) | (first->field_0x1f0 & second->field_0x1e8) |
             (second->field_0x1ec & first->field_0x1e4) | (second->field_0x1f0 & first->field_0x1e8)) != 0;
        first->field_0x1ec |= second->field_0x1e4;
        first->field_0x1f0 |= second->field_0x1e8;
        second->field_0x1ec |= first->field_0x1e4;
        second->field_0x1f0 |= first->field_0x1e8;
        value = first->scaled_radius + second->scaled_radius;
        if (value > 0.0f) {
            first_weight = first->scaled_radius / value * 2.0f;
            second_weight = 2.0f - first_weight;
        } else {
            first_weight = second_weight = 1.0f;
        }
        if (already_touching == 0) {
            if ((first->field_0x1f4 & 0x80) != 0)
                return 2;
            if ((second->field_0x1f4 & 0x80) != 0)
                return 2;
            NuVecSub(&normal, second_position, first_position);
            NuVecNorm(&normal, &normal);
            first_speed = NuFsqrt(first->velocity.x * first->velocity.x + first->velocity.y * first->velocity.y +
                                  first->velocity.z * first->velocity.z);
            value = first_speed == 0.0f ? 0.0f : 1.0f / first_speed;
            first_component.x = first->velocity.x * value;
            first_component.y = first->velocity.y * value;
            first_component.z = first->velocity.z * value;
            second_speed = NuFsqrt(second->velocity.x * second->velocity.x + second->velocity.y * second->velocity.y +
                                   second->velocity.z * second->velocity.z);
            value = second_speed == 0.0f ? 0.0f : 1.0f / second_speed;
            second_component.x = second->velocity.x * value;
            second_component.y = second->velocity.y * value;
            second_component.z = second->velocity.z * value;
            first_dot = NuVecDot(&first_component, &normal);
            second_dot = NuVecDot(&second_component, &normal);
            value = first_dot * first_speed;
            first_component.x = normal.x * value;
            first_component.y = normal.y * value;
            first_component.z = normal.z * value;
            value = second_dot * second_speed;
            second_component.x = normal.x * value;
            second_component.y = normal.y * value;
            second_component.z = normal.z * value;
            if ((first->flags_low & 2) != 0 && (second->flags_low & 2) == 0) {
                second->velocity.x += (first_component.x - second_component.x) * first_weight * 2.0f;
                second->velocity.y += (first_component.y - second_component.y) * first_weight * 2.0f;
                second->velocity.z += (first_component.z - second_component.z) * first_weight * 2.0f;
            } else if ((first->flags_low & 2) == 0 && (second->flags_low & 2) != 0) {
                first->velocity.x += (second_component.x - first_component.x) * second_weight * 2.0f;
                first->velocity.y += (second_component.y - first_component.y) * second_weight * 2.0f;
                first->velocity.z += (second_component.z - first_component.z) * second_weight * 2.0f;
            } else {
                first->velocity.x += (second_component.x - first_component.x) * second_weight;
                first->velocity.y += (second_component.y - first_component.y) * second_weight;
                first->velocity.z += (second_component.z - first_component.z) * second_weight;
                second->velocity.x += (first_component.x - second_component.x) * first_weight;
                second->velocity.y += (first_component.y - second_component.y) * first_weight;
                second->velocity.z += (first_component.z - second_component.z) * first_weight;
            }
            return 2;
        }
        if ((first->field_0x1f4 & 0x80) != 0)
            return 1;
        if ((second->field_0x1f4 & 0x80) != 0)
            return 1;
        average.x = 0.5f * (first->velocity.x + second->velocity.x);
        average.y = 0.5f * (first->velocity.y + second->velocity.y);
        average.z = 0.5f * (first->velocity.z + second->velocity.z);
        if (first_position->x != second_position->x || first_position->y != second_position->y ||
            first_position->z != second_position->z) {
            NuVecSub(&normal, second_position, first_position);
            NuVecNorm(&normal, &normal);
            value = NuFsqrt(average.x * average.x + average.y * average.y + average.z * average.z);
            if (value < MINFORCEAPART)
                value = MINFORCEAPART;
            else if (value > MAXFORCEAPART)
                value = MAXFORCEAPART;
            force = 3.0f;
            value *= force / 2.1f;
            NuVecScale(&normal, &normal, value);
            if ((first->flags_low & 2) != 0 && (second->flags_low & 2) == 0) {
                NuVecAdd(&second->velocity, &average, &normal);
                NuVecAdd(&second->velocity, &average, &normal);
            } else if ((first->flags_low & 2) == 0 && (second->flags_low & 2) != 0) {
                NuVecSub(&first->velocity, &average, &normal);
                NuVecSub(&first->velocity, &average, &normal);
            } else {
                NuVecSub(&first->velocity, &average, &normal);
                NuVecAdd(&second->velocity, &average, &normal);
            }
        }
        return 1;
    }

    i32 APIObjectCollision2D(APIOBJECT *first, APIOBJECT *second) {
        f32 value;
        f32 first_weight;
        f32 second_weight;
        i32 already_touching;
        NUVEC *first_position = &first->initial_position;
        NUVEC *second_position = &second->initial_position;
        f32 distance;
        f32 first_speed;
        f32 second_speed;
        f32 first_dot;
        f32 second_dot;
        f32 force;
        NUVEC normal;
        NUVEC first_component;
        NUVEC second_component;
        NUVEC average;
        if ((first->collision_contact_mask & second->collision_identity_mask) != 0 ||
            (second->collision_contact_mask & first->collision_identity_mask) != 0)
            already_touching = 1;
        else
            already_touching = 0;
        first->collision_contact_mask |= second->collision_identity_mask;
        second->collision_contact_mask |= first->collision_identity_mask;
        value = first->scaled_radius + second->scaled_radius;
        if (value > 0.0f) {
            first_weight = first->scaled_radius / value * 2.0f;
            second_weight = 2.0f - first_weight;
        } else {
            first_weight = second_weight = 1.0f;
        }
        if (already_touching == 0) {
            if ((first->field_0x1f4 & 0x80) != 0)
                return 2;
            if ((second->field_0x1f4 & 0x80) != 0)
                return 2;
            if (NuFabs(second_position->x - first_position->x) < 0.0001f &&
                NuFabs(second_position->z - first_position->z) < 0.0001f &&
                NuFabs(first->previous_velocity.x) < 0.0001f && NuFabs(first->previous_velocity.z) < 0.0001f &&
                NuFabs(second->previous_velocity.x) < 0.0001f && NuFabs(second->previous_velocity.z) < 0.0001f) {
                GameObjectForceApart2D(first, second);
                return 2;
            }
            normal.x = second_position->x - first_position->x;
            normal.z = second_position->z - first_position->z;
            distance = NuFsqrt(normal.x * normal.x + normal.z * normal.z);
            if (distance == 0.0f)
                value = 0.0f;
            else
                value = 1.0f / distance;
            normal.x *= value;
            normal.z *= value;
            first_speed = NuFsqrt(first->previous_velocity.x * first->previous_velocity.x +
                                  first->previous_velocity.z * first->previous_velocity.z);
            if (first_speed == 0.0f)
                value = 0.0f;
            else
                value = 1.0f / first_speed;
            first_component.x = first->previous_velocity.x * value;
            first_component.z = first->previous_velocity.z * value;
            second_speed = NuFsqrt(second->previous_velocity.x * second->previous_velocity.x +
                                   second->previous_velocity.z * second->previous_velocity.z);
            if (second_speed == 0.0f)
                value = 0.0f;
            else
                value = 1.0f / second_speed;
            second_component.x = second->previous_velocity.x * value;
            second_component.z = second->previous_velocity.z * value;
            first_dot = first_component.x * normal.x + first_component.z * normal.z;
            second_dot = second_component.x * normal.x + second_component.z * normal.z;
            value = first_dot * first_speed;
            first_component.x = normal.x * value;
            first_component.z = normal.z * value;
            value = second_dot * second_speed;
            second_component.x = normal.x * value;
            second_component.z = normal.z * value;
            if ((first->flags_low & 2) != 0 && (second->flags_low & 2) == 0) {
                second->velocity.x += (first_component.x - second_component.x) * first_weight * 2.0f;
                second->velocity.z += (first_component.z - second_component.z) * first_weight * 2.0f;
            } else if ((first->flags_low & 2) == 0 && (second->flags_low & 2) != 0) {
                first->velocity.x += (second_component.x - first_component.x) * second_weight * 2.0f;
                first->velocity.z += (second_component.z - first_component.z) * second_weight * 2.0f;
            } else {
                first->velocity.x += (second_component.x - first_component.x) * second_weight;
                first->velocity.z += (second_component.z - first_component.z) * second_weight;
                second->velocity.x += (first_component.x - second_component.x) * first_weight;
                second->velocity.z += (first_component.z - second_component.z) * first_weight;
            }
            first->movement_direction.x = first->velocity.x;
            first->movement_direction.z = first->velocity.z;
            second->movement_direction.x = second->velocity.x;
            second->movement_direction.z = second->velocity.z;
            return 2;
        }
        if ((first->field_0x1f4 & 0x80) != 0)
            return 1;
        if ((second->field_0x1f4 & 0x80) != 0)
            return 1;
        if (NuFabs(second_position->x - first_position->x) < 0.0001f &&
            NuFabs(second_position->z - first_position->z) < 0.0001f && NuFabs(first->previous_velocity.x) < 0.0001f &&
            NuFabs(first->previous_velocity.z) < 0.0001f && NuFabs(second->previous_velocity.x) < 0.0001f &&
            NuFabs(second->previous_velocity.z) < 0.0001f) {
            GameObjectForceApart2D(first, second);
            return 1;
        }
        average.x = 0.5f * (first->previous_velocity.x + second->previous_velocity.x);
        average.z = 0.5f * (first->previous_velocity.z + second->previous_velocity.z);
        if (first_position->x != second_position->x || first_position->z != second_position->z) {
            normal.x = second_position->x - first_position->x;
            normal.z = second_position->z - first_position->z;
            value = NuFdiv(1.0f, NuFsqrt(normal.x * normal.x + normal.z * normal.z));
            normal.x *= value;
            normal.z *= value;
            if (normal.x == 0.0f && normal.z == 0.0f) {
                GameObjectForceApart2D(first, second);
                return 1;
            }
            value = NuFsqrt(average.x * average.x + average.z * average.z);
            if (value < MINFORCEAPART)
                value = MINFORCEAPART;
            else if (value > MAXFORCEAPART)
                value = MAXFORCEAPART;
            force = 3.0f;
            value *= force / 2.1f;
            normal.x *= value;
            normal.z *= value;
            if ((first->flags_low & 2) != 0 && (second->flags_low & 2) == 0) {
                second->velocity.x = average.x + normal.x * 2.0f;
                second->velocity.z = average.z + normal.z * 2.0f;
            } else if ((first->flags_low & 2) == 0 && (second->flags_low & 2) != 0) {
                first->velocity.x = average.x - normal.x * 2.0f;
                first->velocity.z = average.z - normal.z * 2.0f;
            } else {
                first->velocity.x = average.x - normal.x;
                first->velocity.z = average.z - normal.z;
                second->velocity.x = average.x + normal.x;
                second->velocity.z = average.z + normal.z;
            }
            first->movement_direction.x = first->velocity.x;
            first->movement_direction.z = first->velocity.z;
            second->movement_direction.x = second->velocity.x;
            second->movement_direction.z = second->velocity.z;
        }
        return 1;
    }

    void APIObjectCollisions(i32 count, APIOBJECT **objects, NUVEC *minimums, NUVEC *maximums,
                             i32 (*collide)(APIOBJECT *, APIOBJECT *)) {
        if (collide == NULL)
            return;
        i32 outer = 1;
        APIOBJECT **second = objects + outer;
        NUVEC *second_min = minimums + outer;
        NUVEC *second_max = maximums + outer;
        for (; outer < count; ++outer, ++second, ++second_min, ++second_max) {
            NUVEC *second_position = &(*second)->collision_position;
            i32 inner = 0;
            APIOBJECT **first = objects + inner;
            NUVEC *first_min = minimums + inner;
            NUVEC *first_max = maximums + inner;
            for (; inner < outer; ++inner, ++first, ++first_min, ++first_max) {
                if ((*first)->collision_excluded_object == *second)
                    goto no_collision;
                if ((*second)->collision_excluded_object == *first)
                    goto no_collision;
                if ((((static_cast<u64>((*first)->collision_exclusion_mask_high) << 32) |
                      (*first)->collision_exclusion_mask_low) >>
                     (*second)->field_0x289) &
                    1)
                    goto no_collision;
                if ((((static_cast<u64>((*second)->collision_exclusion_mask_high) << 32) |
                      (*second)->collision_exclusion_mask_low) >>
                     (*first)->field_0x289) &
                    1)
                    goto no_collision;
                if (first_min->x > second_max->x || second_min->x > first_max->x || first_min->z > second_max->z ||
                    second_min->z > first_max->z || first_min->y > second_max->y || second_min->y > first_max->y)
                    goto no_collision;
                {
                    NUVEC *first_position = &(*first)->collision_position;
                    f32 dx = second_position->x - first_position->x;
                    f32 dy = second_position->y - first_position->y;
                    f32 dz = second_position->z - first_position->z;
                    if ((*first)->field_0x1dc != (*first)->field_0x1e0 ||
                        (*second)->field_0x1dc != (*second)->field_0x1e0) {
                        dy *= ((*first)->field_0x1dc + (*second)->field_0x1dc) /
                              ((*first)->field_0x1e0 + (*second)->field_0x1e0);
                    }
                    f32 distance_squared = dx * dx + dy * dy + dz * dz;
                    f32 radius = (*first)->field_0x1dc + (*second)->field_0x1dc;
                    if (distance_squared < radius * radius) {
                        collide(*first, *second);
                        continue;
                    }
                }
            no_collision:
                (*first)->field_0x1ec &= ~(*second)->field_0x1e4;
                (*first)->field_0x1f0 &= ~(*second)->field_0x1e8;
                (*second)->field_0x1ec &= ~(*first)->field_0x1e4;
                (*second)->field_0x1f0 &= ~(*first)->field_0x1e8;
            }
        }
    }

    void APIObjectSetUsed(APIOBJECT *object, i32 index, i32 used) {
        if (used != 0) {
            object->in_use = 1;
            object->field_0x289 = index;
        } else {
            object->in_use = 0;
        }
    }

    APIOBJECT *APIObjectCreate(APIOBJECTSYS_s *system) {
        i32 index;
        APIOBJECT *object;
        if (system != NULL && system->object_size != 0) {
            object = system->objects;
            for (index = 0; index < 64; ++index,
                object = reinterpret_cast<APIOBJECT *>(reinterpret_cast<u8 *>(object) + system->object_size)) {
                if ((object->flags_low & 1) == 0) {
                    memset(object, 0, system->object_size);
                    APIObjectSetUsed(object, index, 1);
                    return object;
                }
            }
        }
        return NULL;
    }

    void APIObjectDestroyAll(APIOBJECTSYS_s *system) {
        if (system != NULL) {
            memset(system->objects, 0, system->object_size * 64);
        }
    }

    void APIObjectRemoveFromLOSTable(APIOBJECTSYS_s *, APIOBJECT *, APIOBJECT *) {
    }

    void APIObjectDestroy(APIOBJECTSYS_s *system, APIOBJECT *object) {
        if (system == NULL || object == NULL || system->object_size == 0) {
            return;
        }
        for (i32 i = 0; i < 64; i++) {
            APIOBJECT *other =
                reinterpret_cast<APIOBJECT *>(reinterpret_cast<u8 *>(system->objects) + system->object_size * i);
            if ((other->field_0x1f8 & APIOBJECT_FLAG_IN_USE) != 0) {
                APIObjectRemoveFromLOSTable(system, other, object);
            }
        }
        APIObjectRemoveFromLOSTable(system, NULL, object);
        memset(object, 0, system->object_size);
    }

    void APIObjectLOSChecks(void) {
    }

    void APIObjectVelocities(GameObject_s *object) {
        APIOBJECT &api = object->apiobj;
        if (api.velocity.x == 0.0f && api.velocity.z == 0.0f) {
            api.horizontal_velocity_magnitude = 0.0f;
            api.velocity_magnitude = NuFabs(api.velocity.y);
            return;
        }

        const f32 horizontal_squared = api.velocity.x * api.velocity.x + api.velocity.z * api.velocity.z;
        api.horizontal_velocity_magnitude = NuFsqrt(horizontal_squared);

        if (api.velocity.y == 0.0f) {
            api.velocity_magnitude = api.horizontal_velocity_magnitude;
        } else {
            api.velocity_magnitude = NuFsqrt(api.velocity.y * api.velocity.y + horizontal_squared);
        }
    }

    void AddCollisionSphere(void) {
    }

    void FlagRoomInstancesAsVisible(NUROOM *room, NUGSCN *) {
        for (i32 i = 0; i < room->instance_count; ++i) {
            PortalVisiFlags[room->instance_indices[i] >> 3] =
                PortalVisiFlags[room->instance_indices[i] >> 3] | static_cast<u8>(1 << (room->instance_indices[i] & 7));
        }
    }

    void StoreLocatorCoordinates(CHARACTERMODEL_s *model, NUMTX *world_matrix, NUMTX *joint_matrices, NUVEC *positions,
                                 NUMTX *matrices) {
        if (positions != NULL || matrices != NULL) {
            for (i32 index = 0; index < 16; ++index) {
                if (model->points_of_interest[index] != NULL) {
                    NUMTX matrix;
                    NuHGobjPOIMtx(model->hierarchy, static_cast<u8>(index), world_matrix, joint_matrices, &matrix);
                    if (positions != NULL) {
                        positions[index].x = matrix.m30;
                        positions[index].y = matrix.m31;
                        positions[index].z = matrix.m32;
                    }
                    if (matrices != NULL) {
                        matrices[index] = matrix;
                    }
                }
            }
        }
    }

    void WindShear(void) {
    }

} // extern "C"
