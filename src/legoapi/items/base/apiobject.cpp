#include "decomp.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "nu2api/nucore/nuhgobj.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufilepak.h"
#include "nu2api/nufile/nufpar.h"

#include <string.h>

static f32 MINFORCEAPART = 0.333f;
static f32 MAXFORCEAPART = 0.666f;
static ACTIONINFO_s *APIActionInfo;
static EXTRAACTIONDATA_s *APIExtraActionData;
APICHARACTERSYS *apicharsys;
extern "C" {
i32 apiloadcharactermodels_append = 0;
}
i32 apiloadcharactermodels_nopakfile = 0;
static ANIMREDIRECTFN RedirectAnimFn;
static void *RedirectAnimList;
static char RedirectAnimDir[0x40] = "chars\\commonanims\\";

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

    i32 NewRayCast(NUVEC *position, NUVEC *movement, f32 radius, i32 scan_flags);

    i32 QuickNewRayCast(NUVEC *position, NUVEC *movement, f32 radius, i32 scan_flags, f32 max_distance, f32 step) {
        if (step > 0.0f) {
            f32 distance = movement->x * movement->x + movement->y * movement->y + movement->z * movement->z;
            i32 hit;
            if (distance > max_distance * max_distance)
                return 0;
            if (distance < step * step)
                return NewRayCast(position, movement, radius, scan_flags);
            distance = NuFsqrt(distance);
            NUVEC direction;
            NuVecScale(&direction, movement, NuFdiv(1.0f, distance));
            NUVEC travelled = {0.0f, 0.0f, 0.0f};
            NUVEC current = *position;
            NUVEC segment;
            while (distance > 0.0f) {
                if (distance >= step) {
                    NuVecScale(&segment, &direction, step);
                    distance -= step;
                } else {
                    NuVecScale(&segment, &direction, distance);
                    distance = 0.0f;
                }
                hit = NewRayCast(&current, &segment, radius, scan_flags);
                if (hit != 0) {
                    NuVecAdd(movement, &travelled, &segment);
                    return hit;
                }
                NuVecAdd(&current, &current, &segment);
                NuVecAdd(&travelled, &travelled, &segment);
            }
            return 0;
        } else {
            return NewRayCast(position, movement, radius, scan_flags);
        }
    }

    void APIObjectLOSChecks(APIOBJECTSYS_s *system, i32 checks, i32 source_count, APIOBJECT **sources, i32 target_count,
                            APIOBJECT **targets, f32 ray_step) {
        i32 source_visible;
        i32 target_visible;
        i32 initial_target;
        i32 initial_source;
        if (system == NULL || checks == 0 || source_count == 0 || target_count == 0)
            goto done;
        if (system->los_source_index >= source_count) {
            system->los_source_index = 0;
            ++system->los_target_index;
        }
        if (system->los_target_index >= target_count)
            system->los_target_index = 0;
        initial_target = system->los_target_index;
        initial_source = system->los_source_index;
        do {
            APIOBJECT *target = targets[system->los_target_index];
            i32 target_index = target->field_0x289;
            do {
                APIOBJECT *source = sources[system->los_source_index];
                i32 source_index = source->field_0x289;
                NUVEC difference;
                NuVecSub(&difference, &target->collision_position, &source->collision_position);
                f32 distance_squared =
                    difference.x * difference.x + difference.y * difference.y + difference.z * difference.z;
                f32 source_range = source->viewdistance + target->visibility_range_extension;
                f32 target_range = target->viewdistance + source->visibility_range_extension;
                if (source->force_los_visible) {
                    source_visible = 1;
                } else if ((source->use_cached_los || target->use_cached_los) &&
                           ((system->line_of_sight[source_index] >> target->field_0x289) & 1) != 0 &&
                           distance_squared < source_range * source_range) {
                    source_visible = 1;
                } else if (distance_squared < source_range * source_range &&
                           difference.y - target->field_0x1e0 < source->maxviewheight &&
                           difference.y + target->field_0x1e0 > source->minviewheight) {
                    source_visible = 1;
                } else {
                    source_visible = 0;
                }
                if (target->force_los_visible) {
                    target_visible = 1;
                } else if ((target->use_cached_los || source->use_cached_los) &&
                           ((system->line_of_sight[target_index] >> source->field_0x289) & 1) != 0 &&
                           distance_squared < target_range * target_range) {
                    target_visible = 1;
                } else if (distance_squared < target_range * target_range &&
                           -difference.y - source->field_0x1e0 < target->maxviewheight &&
                           source->field_0x1e0 - difference.y > target->minviewheight) {
                    target_visible = 1;
                } else {
                    target_visible = 0;
                }
                if ((source_visible | target_visible) != 0) {
                    if (system->skip_los_raycast || target->skip_los_raycast || source->skip_los_raycast) {
                        if (source_visible != 0)
                            system->line_of_sight[source_index] |= (u64)1 << target_index;
                        else
                            system->line_of_sight[source_index] &= ~((u64)1 << target_index);
                        if (target_visible != 0)
                            system->line_of_sight[target_index] |= (u64)1 << source_index;
                        else
                            system->line_of_sight[target_index] &= ~((u64)1 << source_index);
                    } else if (QuickNewRayCast(&source->collision_position, &difference, 0.0f, 0, 100.0f, ray_step) ==
                               0) {
                        if (source_visible != 0)
                            system->line_of_sight[source_index] |= (u64)1 << target_index;
                        else
                            system->line_of_sight[source_index] &= ~((u64)1 << target_index);
                        if (target_visible != 0)
                            system->line_of_sight[target_index] |= (u64)1 << source_index;
                        else
                            system->line_of_sight[target_index] &= ~((u64)1 << source_index);
                    } else {
                        if (source_visible != 0 && source->skip_los_raycast)
                            system->line_of_sight[source_index] |= (u64)1 << target_index;
                        else
                            system->line_of_sight[source_index] &= ~((u64)1 << target_index);
                        if (target_visible != 0 && target->skip_los_raycast)
                            system->line_of_sight[target_index] |= (u64)1 << source_index;
                        else
                            system->line_of_sight[target_index] &= ~((u64)1 << source_index);
                    }
                } else {
                    system->line_of_sight[target_index] &= ~((u64)1 << source_index);
                    system->line_of_sight[source_index] &= ~((u64)1 << target_index);
                }
                ++system->los_source_index;
                --checks;
                if (checks == 0)
                    goto done;
                if (system->los_source_index == initial_source && system->los_target_index == initial_target)
                    goto done;
            } while (system->los_source_index < source_count);
            system->los_source_index = 0;
            ++system->los_target_index;
            if (system->los_target_index >= target_count)
                system->los_target_index = 0;
            if (system->los_source_index == initial_source && system->los_target_index == initial_target)
                goto done;
        } while (checks != 0);
    done:;
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

    void APICharacterSysInit(VARIPTR *buf, VARIPTR buf_end, i32 char_count, i32 model_capacity, i32 model_id_capacity,
                             i32 extra_capacity, CHARACTERDATA *cdata_list, APICHARACTERLIGHTFN set_creature_lights) {
        (void)buf_end;

        buf->addr = ALIGN(buf->addr, 0x10);
        apicharsys = (APICHARACTERSYS *)buf->void_ptr;
        buf->addr += sizeof(*apicharsys);
        memset(apicharsys, 0, sizeof(*apicharsys));

        apicharsys->character_count = char_count;
        apicharsys->model_capacity = model_capacity;
        apicharsys->model_id_capacity = model_id_capacity;
        apicharsys->animation_capacity = extra_capacity;

        if (model_capacity != 0) {
            buf->addr = ALIGN(buf->addr, 4);
            apicharsys->models = (APICHARACTERMODEL *)buf->void_ptr;
            buf->addr += (usize)model_capacity * sizeof(*apicharsys->models);
            memset(apicharsys->models, 0, (usize)model_capacity * sizeof(*apicharsys->models));

            for (i32 i = 0; i < model_capacity; i++) {
                APICHARACTERMODEL *model = &apicharsys->models[i];
                if (model_id_capacity != 0) {
                    usize table_size = (usize)model_id_capacity * sizeof(void *);

                    buf->addr = ALIGN(buf->addr, 4);
                    model->model_data_a = (void **)buf->void_ptr;
                    buf->addr += table_size;

                    buf->addr = ALIGN(buf->addr, 4);
                    model->model_data_b = (void **)buf->void_ptr;
                    buf->addr += table_size;

                    buf->addr = ALIGN(buf->addr, 4);
                    model->model_data_c = (void **)buf->void_ptr;
                    buf->addr += table_size;
                }
                APICharacterModelReset(model);
            }
        }

        if (char_count != 0) {
            buf->addr = ALIGN(buf->addr, 4);
            apicharsys->playermodelids = buf->i16_ptr;
            buf->addr += (usize)char_count * sizeof(*apicharsys->playermodelids);
            memset(apicharsys->playermodelids, 0, (usize)char_count * sizeof(*apicharsys->playermodelids));
        }

        if (extra_capacity != 0) {
            buf->addr = ALIGN(buf->addr, 4);
            apicharsys->animations = (ANIMLIST_s *)buf->void_ptr;
            buf->addr += (usize)extra_capacity * sizeof(*apicharsys->animations);
            memset(apicharsys->animations, 0, (usize)extra_capacity * sizeof(*apicharsys->animations));
        }

        apicharsys->char_data = cdata_list;
        apicharsys->set_creature_lights = set_creature_lights;
    }

    void APICharacterModelReset(APICHARACTERMODEL *model) {
        model->model_id = 0;
        model->flags &= 0xfe;
        model->field_0x3 = 0;
        model->hierarchy = NULL;

        if (apicharsys->model_id_capacity != 0) {
            usize table_size = (usize)apicharsys->model_id_capacity * sizeof(void *);
            memset(model->model_data_a, 0, table_size);
            memset(model->model_data_b, 0, table_size);
            memset(model->model_data_c, 0, table_size);
        }
        memset(model->points_of_interest, 0, sizeof(model->points_of_interest));
    }

} // extern "C"

static void NormalizeAnimPath(char *path) {
    while (*path != '\0') {
        *path = static_cast<char>(NuToUpper(*path));
        ++path;
    }
}

static nuanimdata2_s *LoadAnimFromPAK(char *path, i32 area_animation, void *data, i32 size) {
    NormalizeAnimPath(path);
    for (i32 i = 0; i < apicharsys->loaded_animation_count; ++i) {
        if (NuStrCmp(path, apicharsys->animations[i].path) == 0) {
            return apicharsys->animations[i].animation;
        }
    }

    if (apicharsys->loaded_animation_count >= apicharsys->animation_capacity) {
        ++apicharsys->animation_load_attempts;
        return NULL;
    }

    ANIMLIST_s &entry = apicharsys->animations[apicharsys->loaded_animation_count];
    NuStrCpy(entry.path, path);
    entry.animation = static_cast<nuanimdata2_s *>(NuAnimData2LoadBuffFromPAK(data, size));
    if (entry.animation != NULL) {
        if (area_animation != 0) {
            ++apicharsys->area_animation_count;
        }
        ++apicharsys->loaded_animation_count;
        ++apicharsys->animation_load_attempts;
    }
    return entry.animation;
}

static nuanimdata2_s *LoadAnim(char *path, i32 area_animation, VARIPTR *buf, VARIPTR buf_end) {
    NormalizeAnimPath(path);
    for (i32 i = 0; i < apicharsys->loaded_animation_count; ++i) {
        if (NuStrCmp(path, apicharsys->animations[i].path) == 0) {
            return apicharsys->animations[i].animation;
        }
    }

    if (apicharsys->loaded_animation_count >= apicharsys->animation_capacity) {
        ++apicharsys->animation_load_attempts;
        return NULL;
    }

    ANIMLIST_s &entry = apicharsys->animations[apicharsys->loaded_animation_count];
    NuStrCpy(entry.path, path);
    entry.animation = static_cast<nuanimdata2_s *>(NuAnimData2LoadBuff(entry.path, buf, &buf_end));
    if (entry.animation != NULL) {
        if (area_animation != 0) {
            ++apicharsys->area_animation_count;
        }
        ++apicharsys->loaded_animation_count;
        ++apicharsys->animation_load_attempts;
    }
    return entry.animation;
}

static void GetAnimationPath(char *path, const char *default_dir, CHARACTERANIM_s *animation, const char *extension) {
    if (RedirectAnimFn == NULL || RedirectAnimFn(RedirectAnimDir, RedirectAnimList, animation, path) == 0) {
        NuStrCpy(path, default_dir);
        NuStrCat(path, animation->name);
    }
    NuStrCat(path, extension);
}

static bool ShouldLoadAnimation(const CHARACTERANIM_s &animation, i32 area_animation) {
    return (animation.flags & 0x8000) == 0 && ((animation.flags & 1) != 0) == (area_animation != 0);
}


extern "C" {

    void APIObjectRegisterAnimRedirect(ANIMREDIRECTFN fn, void *list, char *directory) {
        i32 length = NuStrLen(directory);
        RedirectAnimFn = fn;
        RedirectAnimList = list;
        if (length <= 0x3f) {
            NuStrCpy(RedirectAnimDir, directory);
            if (directory[length - 1] != '\\') {
                NuStrCat(RedirectAnimDir, "\\");
            }
        }
    }

    void APIResetCharacterRemap(void) {
        for (i32 i = 0; i < apicharsys->character_count; ++i) {
            if ((apicharsys->char_data[i].model_flags & 2) == 0) {
                apicharsys->playermodelids[i] = -1;
            }
        }
    }

    void APILoadCharacterModels(APICHARACTERMODELLIST_s *list, i32 area_animation, VARIPTR *buf, VARIPTR buf_end,
                                i32 area_models) {
        if (apiloadcharactermodels_append == 0) {
            APIResetCharacterRemap();
            for (i32 model_index = 0; model_index < apicharsys->permanent_model_count; ++model_index) {
                APICHARACTERMODEL &model = apicharsys->models[model_index];
                for (i32 animation_id = 0; animation_id < apicharsys->model_id_capacity; ++animation_id) {
                    if ((model.model_data_b[animation_id] != NULL || model.model_data_c[animation_id] != NULL) &&
                        (static_cast<CHARACTERANIM_s *>(model.model_data_a[animation_id])->flags & 1) == 0) {
                        model.model_data_a[animation_id] = NULL;
                        model.model_data_b[animation_id] = NULL;
                        model.model_data_c[animation_id] = NULL;
                    }
                }
            }
            for (i32 model_index = apicharsys->permanent_model_count; model_index < apicharsys->model_capacity;
                 ++model_index) {
                apicharsys->models[model_index].hierarchy = NULL;
            }
            apicharsys->loaded_model_count = apicharsys->permanent_model_count;
        }
        apiloadcharactermodels_append = 0;

        while (list != NULL && list->model_id != -1 && apicharsys->loaded_model_count < apicharsys->model_capacity) {
            const i32 model_id = list->model_id;
            CHARACTERDATA &character = apicharsys->char_data[model_id];

            char directory[0x40];
            NuStrCpy(directory, "chars\\");
            NuStrCat(directory, character.dir);
            NuStrCat(directory, "\\");

            char directory_pack_path[0x100];
            char model_pack_path[0x100];
            NuStrCpy(directory_pack_path, directory);
            NuStrCat(directory_pack_path, character.dir);
            NuStrCat(directory_pack_path, ".fpk");
            NuStrCpy(model_pack_path, directory);
            NuStrCat(model_pack_path, character.file);
            NuStrCat(model_pack_path, ".fpk");

            APICHARACTERMODEL *model;
            bool model_loaded = false;
            if (apicharsys->playermodelids[model_id] != -1) {
                model = &apicharsys->models[apicharsys->playermodelids[model_id]];
            } else {
                model = &apicharsys->models[apicharsys->loaded_model_count];
                APICharacterModelReset(model);

                char hierarchy_path[0x200];
                NuStrCpy(hierarchy_path, directory);
                NuStrCat(hierarchy_path, character.file);
                NuStrCat(hierarchy_path, ".ghg");

                model->hierarchy = NuGHGRead(hierarchy_path, buf, buf_end);
                if (model->hierarchy == NULL) {
                    ++list;
                    continue;
                }
                for (i32 poi = 0; poi < 16; ++poi) {
                    model->points_of_interest[poi] = NuHGobjGetPOI(model->hierarchy, poi);
                }
                model_loaded = true;
            }

            CHARACTERANIM_s *animations = area_models != 0 && list->count != 0 ? character.animations : NULL;
            void *pak = NULL;
            if (apiloadcharactermodels_nopakfile == 0) {
                pak = NuFilePakLoad(directory_pack_path, buf, buf_end, 0x10);
                if (pak == NULL) {
                    pak = NuFilePakLoad(model_pack_path, buf, buf_end, 0x10);
                }
            }

            if (pak != NULL) {
                for (CHARACTERANIM_s *animation = animations; animation != NULL && animation->name != NULL;
                     ++animation) {
                    if (!ShouldLoadAnimation(*animation, area_animation)) {
                        continue;
                    }
                    char animation_path[0x200];
                    if ((animation->flags & 4) != 0 && model->model_data_b[animation->animation_id] == NULL) {
                        GetAnimationPath(animation_path, directory, animation, ".an3");
                        i32 item = NuFilePakGetItem(pak, animation_path);
                        if (item != 0) {
                            NuFilePakSetItemRequired(pak, item, 1);
                        }
                    }
                    if ((animation->flags & 8) != 0 && model->model_data_c[animation->animation_id] == NULL) {
                        GetAnimationPath(animation_path, directory, animation, ".bsa");
                        i32 item = NuFilePakGetItem(pak, animation_path);
                        if (item != 0) {
                            NuFilePakSetItemRequired(pak, item, 1);
                        }
                    }
                }

                buf->addr -= NuFilePakCondense(pak);
                for (CHARACTERANIM_s *animation = animations; animation != NULL && animation->name != NULL;
                     ++animation) {
                    if (!ShouldLoadAnimation(*animation, area_animation)) {
                        continue;
                    }
                    char animation_path[0x200];
                    if ((animation->flags & 4) != 0 && model->model_data_b[animation->animation_id] == NULL) {
                        GetAnimationPath(animation_path, directory, animation, ".an3");
                        i32 item = NuFilePakGetItem(pak, animation_path);
                        void *data;
                        i32 size;
                        if (item != 0 && NuFilePakGetItemInfo(pak, item, &data, &size) != 0) {
                            const bool pointer_block = static_cast<i32 *>(data)[1] > static_cast<i32>(0x414e4934);
                            ani3_animheader_s *joint_animation = reinterpret_cast<ani3_animheader_s *>(
                                pointer_block ? static_cast<u8 *>(data) + 4 : data);
                            if ((joint_animation->field_12 & 0xff) == 0) {
                                if (pointer_block) {
                                    NuPtrBlockFix(data);
                                } else {
                                    NuAnimData2Fixup(size, &data);
                                }
                                joint_animation->field_12 |= 1;
                            }
                            model->model_data_b[animation->animation_id] = joint_animation;
                            model->model_data_a[animation->animation_id] = animation;
                        }
                    }
                    if ((animation->flags & 8) != 0 && model->model_data_c[animation->animation_id] == NULL) {
                        GetAnimationPath(animation_path, directory, animation, ".bsa");
                        i32 item = NuFilePakGetItem(pak, animation_path);
                        void *data;
                        i32 size;
                        if (item != 0 && NuFilePakGetItemInfo(pak, item, &data, &size) != 0) {
                            model->model_data_c[animation->animation_id] =
                                LoadAnimFromPAK(animation_path, area_animation, data, size);
                            if (model->model_data_c[animation->animation_id] != NULL) {
                                model->model_data_a[animation->animation_id] = animation;
                            }
                        }
                    }
                }
            } else {
                for (CHARACTERANIM_s *animation = animations; animation != NULL && animation->name != NULL;
                     ++animation) {
                    if (!ShouldLoadAnimation(*animation, area_animation)) {
                        continue;
                    }
                    char animation_path[0x200];
                    if ((animation->flags & 4) != 0 && model->model_data_b[animation->animation_id] == NULL) {
                        GetAnimationPath(animation_path, directory, animation, ".an3");
                        if (NuFileExists(animation_path) != 0) {
                            model->model_data_b[animation->animation_id] =
                                LoadAnim(animation_path, area_animation, buf, buf_end);
                        }
                        if (model->model_data_b[animation->animation_id] != NULL) {
                            model->model_data_a[animation->animation_id] = animation;
                        }
                    }
                    if ((animation->flags & 8) != 0 && model->model_data_c[animation->animation_id] == NULL) {
                        GetAnimationPath(animation_path, directory, animation, ".bsa");
                        model->model_data_c[animation->animation_id] =
                            LoadAnim(animation_path, area_animation, buf, buf_end);
                        if (model->model_data_c[animation->animation_id] != NULL) {
                            model->model_data_a[animation->animation_id] = animation;
                        }
                    }
                }
            }

            if (model_loaded) {
                model->model_id = model_id;
                model->flags = (model->flags & ~1) | (area_models != 0 && list->count != 0 ? 1 : 0);
                apicharsys->playermodelids[model_id] = apicharsys->loaded_model_count;
                if (area_animation != 0) {
                    ++apicharsys->permanent_model_count;
                    character.model_flags |= 2;
                }
                ++apicharsys->loaded_model_count;
            }
            ++list;
        }
    }


    APICHARACTERMODEL *APICharacterLoaded(i32 character_id) {
        if (character_id == -1) {
            return NULL;
        }

        const i16 model_index = apicharsys->playermodelids[character_id];
        if (model_index == -1) {
            return NULL;
        }
        return &apicharsys->models[model_index];
    }

    // Original @0x3cd1ea. Destroy the area-loaded hierarchy tail in mode 0
    // or every loaded hierarchy in other modes; model slots are reset later.
    void APIDumpCharacterModels(i32 mode) {
        i32 model_index = mode == 0 ? apicharsys->permanent_model_count : 0;
        while (model_index < apicharsys->loaded_model_count) {
            APICHARACTERMODEL &model = apicharsys->models[model_index];
            if (model.hierarchy != NULL) {
                NuHGobjDestroy(model.hierarchy);
            }
            ++model_index;
        }
    }

    i32 InModelList(APICHARACTERMODELLIST_s *list, i32 id, i32 *out_index) {
        if (list != NULL) {
            i32 i = 0;
            for (; list->model_id != -1; list++, i++) {
                if (list->model_id == id) {
                    if (out_index != NULL)
                        *out_index = i;
                    return 1;
                }
            }
        }
        if (out_index != NULL)
            *out_index = -1;
        return 0;
    }

    void SetActionInfo(ACTIONINFO_s *action_info, EXTRAACTIONDATA_s *extra_action_data) {
        APIActionInfo = action_info;
        APIExtraActionData = extra_action_data;
    }

    u32 ActionInfoFlags(i32 action) {
        return APIActionInfo != NULL ? APIActionInfo[action].flags : 0;
    }

    const char *ActionInfoName(i32 action) {
        return APIActionInfo != NULL ? APIActionInfo[action].name : "?";
    }

    i32 ActionFromName(const char *name) {
        if (apicharsys == NULL || name == NULL) {
            return -1;
        }

        if (APIActionInfo != NULL) {
            for (i32 action = 0; action < apicharsys->model_id_capacity; ++action) {
                if (NuStrICmp(APIActionInfo[action].name, name) == 0) {
                    return action;
                }
            }
        }

        if (APIExtraActionData != NULL) {
            for (EXTRAACTIONDATA_s *extra = APIExtraActionData; extra->name != NULL; ++extra) {
                if (NuStrICmp(extra->name, name) == 0) {
                    return extra->action;
                }
            }
        }
        return -1;
    }

CHARACTERDATA *ConfigureCharacterList(char *file, VARIPTR *bufferStart, VARIPTR *bufferEnd, i32 count, i32 *countDest,
                                      i32 count2, GAMECHARACTERDATA **dataList) {
    bool bVar1;
    bool bVar2;
    nufpar_s *fp;
    CHARACTERDATA *characterdata;
    i16 dirnameOffsets[500];
    i16 filenameOffsets[500];
    char buf[10000];
    CHARACTERDATA *cdatas;
    i32 j;
    usize offset;
    i32 i;
    CHARACTERDATA *cdata;

    fp = NuFParCreate(file);
    if (500 < count) {
        count = 500;
    }
    bufferStart->void_ptr = (void *)ALIGN(bufferStart->addr, 4);
    characterdata = (CHARACTERDATA *)bufferStart->void_ptr;
    i = 0;

    memset(buf, 0, 10000);

    buf[0] = '\0';
    offset = 0;
    bVar2 = false;
    cdata = characterdata;
    while (NuFParGetLine(fp) != 0) {
        NuFParGetWord(fp);
        if (*fp->word_buf != '\0') {
            if (bVar2) {
                if (NuStrICmp(fp->word_buf, "char_end") == 0) {
                    bVar2 = false;
                    if ((dirnameOffsets[i] != -1) && (filenameOffsets[i] != -1)) {
                        i = i + 1;
                        cdata = cdata + 1;
                    }
                } else if (NuStrICmp(fp->word_buf, "dir") == 0 && NuFParGetWord(fp) != 0) {
                    i32 len = NuStrLen(fp->word_buf);
                    if ((len + offset + 1) < 10000) {
                        NuStrCpy(buf + offset, fp->word_buf);
                        dirnameOffsets[i] = (i16)offset;
                        offset = offset + len + 1;
                    }
                } else if (NuStrICmp(fp->word_buf, "file") == 0 && NuFParGetWord(fp) != 0) {
                    i32 len = NuStrLen(fp->word_buf);
                    if ((len + offset + 1) < 10000) {
                        NuStrCpy(buf + offset, fp->word_buf);
                        filenameOffsets[i] = (i16)offset;
                        offset = offset + len + 1;
                    }
                }
            } else {
                if (NuStrICmp(fp->word_buf, "char_start") == 0 && i < count) {
                    bVar1 = true;
                } else {
                    bVar1 = false;
                }

                if (bVar1) {
                    bVar2 = true;
                    dirnameOffsets[i] = -1;
                    filenameOffsets[i] = -1;
                    cdata->field0_0x0 = -1;
                    cdata->model_flags = 0;
                    cdata->dir = (char *)0x0;
                    cdata->file = (char *)0x0;
                    cdata->animations = NULL;
                    cdata->field5_0x14 = 0;
                    cdata->move_fn = NULL;
                    cdata->animate_fn = NULL;
                    cdata->draw_fn = NULL;
                    cdata->field11_0x24 = 0;
                    cdata->field12_0x28 = 0;
                    cdata->field13_0x2c = 1.0f;
                    cdata->field14_0x30 = 0.5f;
                    cdata->field15_0x34 = -0.5f;
                    cdata->field16_0x38 = 0.5f;
                    cdata->field17_0x3c = 1.0f;
                    cdata->flags = cdata->flags & 0xfe;
                    cdata->field20_0x42 = -1;
                    cdata->field21_0x44 = 0;
                    cdata->field22_0x48 = 0;
                }
            }
        }
    }
    NuFParDestroy(fp);
    if (i < 1) {
        characterdata = NULL;
    } else {
        bufferStart->void_ptr = cdata;
        memmove(bufferStart->void_ptr, buf, offset);
        for (j = 0; j < i; j = j + 1) {
            characterdata[j].dir = (char *)((i32)dirnameOffsets[j] + (usize)bufferStart->void_ptr);
            characterdata[j].file = (char *)((i32)filenameOffsets[j] + (usize)bufferStart->void_ptr);
        }
        bufferStart->void_ptr = (void *)((usize)bufferStart->void_ptr + offset);
        bufferStart->void_ptr = (void *)ALIGN((usize)bufferStart->void_ptr, 4);
        if (0 < count2) {
            if (dataList != NULL) {
                *dataList = (GAMECHARACTERDATA_s *)bufferStart->void_ptr;
            }
            for (j = 0; j < i; j = j + 1) {
                characterdata[j].field11_0x24 = bufferStart->void_ptr;
                bufferStart->void_ptr = (void *)((usize)bufferStart->void_ptr + count2);
            }
        }
        bufferStart->void_ptr = (void *)ALIGN((usize)bufferStart->void_ptr, 4);
        if (countDest != (i32 *)0x0) {
            *countDest = i;
        }
    }
    return characterdata;
}

    void WindShear(void) {
    }

} // extern "C"
