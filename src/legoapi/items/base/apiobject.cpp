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

extern "C" {

    i32 APIObjectCollision(APIOBJECT *first, APIOBJECT *second) {
        const bool already_colliding =
            (first->field_0x1f0 & second->field_0x1e8) != 0 || (first->field_0x1ec & second->field_0x1e4) != 0 ||
            (second->field_0x1f0 & first->field_0x1e8) != 0 || (second->field_0x1ec & first->field_0x1e4) != 0;

        first->field_0x1ec |= second->field_0x1e4;
        first->field_0x1f0 |= second->field_0x1e8;
        second->field_0x1ec |= first->field_0x1e4;
        second->field_0x1f0 |= first->field_0x1e8;

        f32 first_weight;
        f32 second_weight;
        const f32 combined_radius = first->scaled_radius + second->scaled_radius;
        if (combined_radius <= 0.0f) {
            first_weight = 1.0f;
            second_weight = 1.0f;
        } else {
            first_weight = first->scaled_radius / combined_radius;
            first_weight += first_weight;
            second_weight = 2.0f - first_weight;
        }

        if (!already_colliding) {
            if ((first->field_0x1f4 & 0x80) == 0 && (second->field_0x1f4 & 0x80) == 0) {
                NUVEC separation;
                NuVecSub(&separation, &second->initial_position, &first->initial_position);
                NuVecNorm(&separation, &separation);

                const f32 first_speed =
                    NuFsqrt(first->velocity.x * first->velocity.x + first->velocity.y * first->velocity.y +
                            first->velocity.z * first->velocity.z);
                const f32 first_inverse_speed = first_speed == 0.0f ? 0.0f : 1.0f / first_speed;
                NUVEC first_direction = {first->velocity.x * first_inverse_speed,
                                         first->velocity.y * first_inverse_speed,
                                         first->velocity.z * first_inverse_speed};

                const f32 second_speed =
                    NuFsqrt(second->velocity.x * second->velocity.x + second->velocity.y * second->velocity.y +
                            second->velocity.z * second->velocity.z);
                const f32 second_inverse_speed = second_speed == 0.0f ? 0.0f : 1.0f / second_speed;
                NUVEC second_direction = {second->velocity.x * second_inverse_speed,
                                          second->velocity.y * second_inverse_speed,
                                          second->velocity.z * second_inverse_speed};

                const f32 first_projection = NuVecDot(&first_direction, &separation) * first_speed;
                const f32 second_projection = NuVecDot(&second_direction, &separation) * second_speed;
                NUVEC first_component = {separation.x * first_projection, separation.y * first_projection,
                                         separation.z * first_projection};
                NUVEC second_component = {separation.x * second_projection, separation.y * second_projection,
                                          separation.z * second_projection};

                if ((first->field_0x1f8 & 2) != 0 && (second->field_0x1f8 & 2) == 0) {
                    second->velocity.x += (first_component.x - second_component.x) * first_weight * 2.0f;
                    second->velocity.y += (first_component.y - second_component.y) * first_weight * 2.0f;
                    second->velocity.z += (first_component.z - second_component.z) * first_weight * 2.0f;
                } else if ((first->field_0x1f8 & 2) == 0 && (second->field_0x1f8 & 2) != 0) {
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
            }
            return 2;
        }

        if ((first->field_0x1f4 & 0x80) == 0 && (second->field_0x1f4 & 0x80) == 0) {
            NUVEC mean_velocity = {(first->velocity.x + second->velocity.x) * 0.5f,
                                   (first->velocity.y + second->velocity.y) * 0.5f,
                                   (first->velocity.z + second->velocity.z) * 0.5f};
            if (first->initial_position.x != second->initial_position.x ||
                first->initial_position.y != second->initial_position.y ||
                first->initial_position.z != second->initial_position.z) {
                NUVEC separation;
                NuVecSub(&separation, &second->initial_position, &first->initial_position);
                NuVecNorm(&separation, &separation);
                f32 force = NuFsqrt(mean_velocity.x * mean_velocity.x + mean_velocity.y * mean_velocity.y +
                                    mean_velocity.z * mean_velocity.z);
                if (force < MINFORCEAPART) {
                    force = MINFORCEAPART;
                } else if (force > MAXFORCEAPART) {
                    force = MAXFORCEAPART;
                }
                NuVecScale(&separation, &separation, force * 1.4285715f);
                if ((first->field_0x1f8 & 2) != 0 && (second->field_0x1f8 & 2) == 0) {
                    NuVecAdd(&second->velocity, &mean_velocity, &separation);
                    NuVecAdd(&second->velocity, &mean_velocity, &separation);
                } else if ((first->field_0x1f8 & 2) == 0 && (second->field_0x1f8 & 2) != 0) {
                    NuVecSub(&first->velocity, &mean_velocity, &separation);
                    NuVecSub(&first->velocity, &mean_velocity, &separation);
                } else {
                    NuVecSub(&first->velocity, &mean_velocity, &separation);
                    NuVecAdd(&second->velocity, &mean_velocity, &separation);
                }
            }
        }
        return 1;
    }

} // extern "C"

static void GameObjectForceApart2D(APIOBJECT *first, APIOBJECT *second) {
    const u32 angle = static_cast<u32>(NuRandFloat() * 65536.0f);
    const f32 direction_x = NuTrigTable[static_cast<u16>(angle) >> 1];
    const f32 direction_z = NuTrigTable[((angle & 0xffff) + 0x4000) >> 1 & 0x7fff];

    if ((first->field_0x1f8 & 2) != 0 && (second->field_0x1f8 & 2) == 0) {
        second->velocity.x = direction_x * 4.0f;
        second->velocity.z = direction_z * 4.0f;
    } else if ((first->field_0x1f8 & 2) == 0 && (second->field_0x1f8 & 2) != 0) {
        first->velocity.x = direction_x * 4.0f;
        first->velocity.z = direction_z * 4.0f;
    } else {
        first->velocity.x = direction_x + direction_x;
        first->velocity.z = direction_z + direction_z;
        second->velocity.x = -first->velocity.x;
        second->velocity.z = -first->velocity.z;
    }
    first->movement_direction.x = first->velocity.x;
    first->movement_direction.z = first->velocity.z;
    second->movement_direction.x = second->velocity.x;
    second->movement_direction.z = second->velocity.z;
}

extern "C" {

    i32 APIObjectCollision2D(APIOBJECT *first, APIOBJECT *second) {
        const bool already_colliding =
            (first->field_0x1f0 & second->field_0x1e8) != 0 || (first->field_0x1ec & second->field_0x1e4) != 0 ||
            (second->field_0x1f0 & first->field_0x1e8) != 0 || (second->field_0x1ec & first->field_0x1e4) != 0;

        first->field_0x1ec |= second->field_0x1e4;
        first->field_0x1f0 |= second->field_0x1e8;
        second->field_0x1ec |= first->field_0x1e4;
        second->field_0x1f0 |= first->field_0x1e8;

        f32 first_weight;
        f32 second_weight;
        const f32 combined_radius = first->scaled_radius + second->scaled_radius;
        if (combined_radius <= 0.0f) {
            first_weight = 1.0f;
            second_weight = 1.0f;
        } else {
            first_weight = first->scaled_radius / combined_radius;
            first_weight += first_weight;
            second_weight = 2.0f - first_weight;
        }

        const bool stationary_overlap =
            NuFabs(second->initial_position.x - first->initial_position.x) < 0.0001f &&
            NuFabs(second->initial_position.z - first->initial_position.z) < 0.0001f &&
            NuFabs(first->previous_velocity.x) < 0.0001f && NuFabs(first->previous_velocity.z) < 0.0001f &&
            NuFabs(second->previous_velocity.x) < 0.0001f && NuFabs(second->previous_velocity.z) < 0.0001f;

        if (already_colliding) {
            if ((first->field_0x1f4 & 0x80) == 0 && (second->field_0x1f4 & 0x80) == 0) {
                if (stationary_overlap) {
                    GameObjectForceApart2D(first, second);
                    return 1;
                }

                const f32 mean_x = (first->previous_velocity.x + second->previous_velocity.x) * 0.5f;
                const f32 mean_z = (first->previous_velocity.z + second->previous_velocity.z) * 0.5f;
                if (first->initial_position.x != second->initial_position.x ||
                    first->initial_position.z != second->initial_position.z) {
                    f32 separation_x = second->initial_position.x - first->initial_position.x;
                    f32 separation_z = second->initial_position.z - first->initial_position.z;
                    const f32 inverse_distance =
                        NuFdiv(1.0f, NuFsqrt(separation_x * separation_x + separation_z * separation_z));
                    separation_x *= inverse_distance;
                    separation_z *= inverse_distance;
                    if (separation_x == 0.0f && separation_z == 0.0f) {
                        GameObjectForceApart2D(first, second);
                        return 1;
                    }
                    f32 force = NuFsqrt(mean_x * mean_x + mean_z * mean_z);
                    if (force < MINFORCEAPART) {
                        force = MINFORCEAPART;
                    } else if (force > MAXFORCEAPART) {
                        force = MAXFORCEAPART;
                    }
                    separation_x *= force * 1.4285715f;
                    separation_z *= force * 1.4285715f;
                    if ((first->field_0x1f8 & 2) != 0 && (second->field_0x1f8 & 2) == 0) {
                        second->velocity.x = mean_x + separation_x + separation_x;
                        second->velocity.z = mean_z + separation_z + separation_z;
                    } else if ((first->field_0x1f8 & 2) == 0 && (second->field_0x1f8 & 2) != 0) {
                        first->velocity.x = mean_x - (separation_x + separation_x);
                        first->velocity.z = mean_z - (separation_z + separation_z);
                    } else {
                        first->velocity.x = mean_x - separation_x;
                        first->velocity.z = mean_z - separation_z;
                        second->velocity.x = mean_x + separation_x;
                        second->velocity.z = mean_z + separation_z;
                    }
                    first->movement_direction.x = first->velocity.x;
                    first->movement_direction.z = first->velocity.z;
                    second->movement_direction.x = second->velocity.x;
                    second->movement_direction.z = second->velocity.z;
                }
            }
            return 1;
        }

        if ((first->field_0x1f4 & 0x80) == 0 && (second->field_0x1f4 & 0x80) == 0) {
            if (stationary_overlap) {
                GameObjectForceApart2D(first, second);
                return 2;
            }
            f32 separation_x = second->initial_position.x - first->initial_position.x;
            f32 separation_z = second->initial_position.z - first->initial_position.z;
            const f32 distance = NuFsqrt(separation_x * separation_x + separation_z * separation_z);
            const f32 inverse_distance = distance == 0.0f ? 0.0f : 1.0f / distance;
            separation_x *= inverse_distance;
            separation_z *= inverse_distance;

            const f32 first_speed = NuFsqrt(first->previous_velocity.x * first->previous_velocity.x +
                                            first->previous_velocity.z * first->previous_velocity.z);
            const f32 first_inverse_speed = first_speed == 0.0f ? 0.0f : 1.0f / first_speed;
            const f32 first_projection = (first->previous_velocity.x * first_inverse_speed * separation_x +
                                          first->previous_velocity.z * first_inverse_speed * separation_z) *
                                         first_speed;
            const f32 first_component_x = separation_x * first_projection;
            const f32 first_component_z = separation_z * first_projection;

            const f32 second_speed = NuFsqrt(second->previous_velocity.x * second->previous_velocity.x +
                                             second->previous_velocity.z * second->previous_velocity.z);
            const f32 second_inverse_speed = second_speed == 0.0f ? 0.0f : 1.0f / second_speed;
            const f32 second_projection = (second->previous_velocity.x * second_inverse_speed * separation_x +
                                           second->previous_velocity.z * second_inverse_speed * separation_z) *
                                          second_speed;
            const f32 second_component_x = separation_x * second_projection;
            const f32 second_component_z = separation_z * second_projection;

            if ((first->field_0x1f8 & 2) != 0 && (second->field_0x1f8 & 2) == 0) {
                second->velocity.x += (first_component_x - second_component_x) * first_weight * 2.0f;
                second->velocity.z += (first_component_z - second_component_z) * first_weight * 2.0f;
            } else if ((first->field_0x1f8 & 2) == 0 && (second->field_0x1f8 & 2) != 0) {
                first->velocity.x += (second_component_x - first_component_x) * second_weight * 2.0f;
                first->velocity.z += (second_component_z - first_component_z) * second_weight * 2.0f;
            } else {
                first->velocity.x += (second_component_x - first_component_x) * second_weight;
                first->velocity.z += (second_component_z - first_component_z) * second_weight;
                second->velocity.x += (first_component_x - second_component_x) * first_weight;
                second->velocity.z += (first_component_z - second_component_z) * first_weight;
            }
            first->movement_direction.x = first->velocity.x;
            first->movement_direction.z = first->velocity.z;
            second->movement_direction.x = second->velocity.x;
            second->movement_direction.z = second->velocity.z;
        }
        return 2;
    }

    void APIObjectCollisions(i32 count, APIOBJECT **objects, NUVEC *minimums, NUVEC *maximums,
                             i32 (*collision_callback)(APIOBJECT *, APIOBJECT *)) {
        if (collision_callback == NULL) {
            return;
        }
        for (i32 current_index = 1; current_index < count; ++current_index) {
            APIOBJECT *current = objects[current_index];
            for (i32 previous_index = 0; previous_index < current_index; ++previous_index) {
                APIOBJECT *previous = objects[previous_index];
                bool separated = previous->collision_link == current || current->collision_link == previous;
                if (!separated) {
                    const u64 previous_mask = static_cast<u64>(previous->collision_mask_low) |
                                              static_cast<u64>(previous->collision_mask_high) << 32;
                    const u64 current_mask = static_cast<u64>(current->collision_mask_low) |
                                             static_cast<u64>(current->collision_mask_high) << 32;
                    separated = (previous_mask & (static_cast<u64>(1) << current->field_0x289)) != 0 ||
                                (current_mask & (static_cast<u64>(1) << previous->field_0x289)) != 0 ||
                                maximums[current_index].x < minimums[previous_index].x ||
                                maximums[previous_index].x < minimums[current_index].x ||
                                maximums[current_index].z < minimums[previous_index].z ||
                                maximums[previous_index].z < minimums[current_index].z ||
                                maximums[current_index].y < minimums[previous_index].y ||
                                maximums[previous_index].y < minimums[current_index].y;
                }
                if (!separated) {
                    f32 delta_x = current->collision_position.x - previous->collision_position.x;
                    f32 delta_y = current->collision_position.y - previous->collision_position.y;
                    f32 delta_z = current->collision_position.z - previous->collision_position.z;
                    if (previous->field_0x1dc != previous->field_0x1e0 ||
                        current->field_0x1dc != current->field_0x1e0) {
                        delta_y *= (previous->field_0x1dc + current->field_0x1dc) /
                                   (previous->field_0x1e0 + current->field_0x1e0);
                    }
                    const f32 radius = previous->field_0x1dc + current->field_0x1dc;
                    separated = radius * radius <= delta_x * delta_x + delta_y * delta_y + delta_z * delta_z;
                }
                if (separated) {
                    previous->field_0x1ec &= ~current->field_0x1e4;
                    previous->field_0x1f0 &= ~current->field_0x1e8;
                    current->field_0x1ec &= ~previous->field_0x1e4;
                    current->field_0x1f0 &= ~previous->field_0x1e8;
                } else {
                    collision_callback(previous, current);
                }
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
        if (system == NULL || system->object_size == 0 || system->objects == NULL) {
            return NULL;
        }

        u8 *slot = reinterpret_cast<u8 *>(system->objects);
        for (u8 index = 0; index < 64; index++, slot += system->object_size) {
            APIOBJECT *object = reinterpret_cast<APIOBJECT *>(slot);
            if ((object->field_0x1f8 & APIOBJECT_FLAG_IN_USE) == 0) {
                memset(object, 0, system->object_size);
                APIObjectSetUsed(object, index, 1);
                return object;
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
            api.velocity_magnitude = NuFsqrt(horizontal_squared + api.velocity.y * api.velocity.y);
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
