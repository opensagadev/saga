#include "decomp.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/motion/gameanim.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edpp_internal.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "nu2api/nucore/nuhgobj.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuspecial.h"
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

#include <float.h>
#include <stdarg.h>
#include <string.h>

static f32 MINFORCEAPART = 0.333f;
static f32 MAXFORCEAPART = 0.666f;
static i32 AnimBlendMode = 1;
static ACTIONINFO_s *APIActionInfo;
static EXTRAACTIONDATA_s *APIExtraActionData;
APICHARACTERSYS *apicharsys;
extern "C" {
    i32 apiloadcharactermodels_append = 0;
    f32 animduration_blendouttime;
    u8 TempNumJoints;
    nuhgobj_s *Temphgobj;
    NUMTL *APITrans_Mtl[2];
    i32 notransparentchardraw;
    i32 drawcharactermodel_nobsa;
    i32 drawcharactermodel_noani;
    i32 drawcharactermodel_restpose;
    i32 drawcharactermodel_keepmergeaction;
    i32 drawcharactermodel_locatorsupdated;
    void (*APIObjResetShadowMapRenderingFn)(void);
    void (*APIObjEnableShadowMapRenderingFn)(void);
    void (*APIObjPlaySfxByIdFn)(i32, NUVEC *);
}
i32 apiloadcharactermodels_nopakfile = 0;
static ANIMREDIRECTFN RedirectAnimFn;
static void *RedirectAnimList;
static char RedirectAnimDir[0x40] = "chars\\commonanims\\";
f32 character_farclip = 0.0f;

static void GameObjectForceApart2D(APIOBJECT *first, APIOBJECT *second) {
    const u16 angle = static_cast<u16>(NuRandFloat() * 65536.0f);

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

    float AnimEndFrame(CHARACTERMODEL_s *model, i32 animation) {
        if (animation != -1 && model->model_data_b[animation] != NULL) {
            return NuAnimEndFrame(model->model_data_b[animation]);
        }
        return 0.0f;
    }

    i32 AnimMiscFlags(CHARACTERMODEL_s *model, i32 animation) {
        if (animation != -1 && model->model_data_b[animation] != NULL) {
            return static_cast<CHARACTERANIM_s *>(model->model_data_a[animation])->misc_flags;
        }
        return 0;
    }

    void AnimList_NoLoad(i32 character_id, ...) {
        i32 current_character = character_id;
        va_list characters;
        va_start(characters, character_id);
        do {
            CHARACTERDATA *character = apicharsys != NULL ? &apicharsys->char_data[current_character] : NULL;
            CHARACTERANIM_s *animation = character != NULL ? character->animations : NULL;
            if (animation != NULL) {
                while (animation->name != NULL) {
                    animation->flags |= 0x8000;
                    ++animation;
                }
            }
            current_character = va_arg(characters, i32);
        } while (current_character != -1);
        va_end(characters);
    }

    void AnimList_RequestAnimGroups(i32 character_id, ...) {
        CHARACTERDATA *character = apicharsys != NULL ? &apicharsys->char_data[character_id] : NULL;
        CHARACTERANIM_s *animations = character != NULL ? character->animations : NULL;
        va_list groups;
        va_start(groups, character_id);
        CHARACTERANIM_s *animation;
        while (true) {
            animation = animations;
            i32 group_id = va_arg(groups, i32);
            if (group_id == -1) {
                break;
            }
            while (animation->name != NULL) {
                if (animation->field_0x0a == group_id) {
                    animation->flags &= ~0x8000;
                }
                ++animation;
            }
        }
        va_end(groups);
    }

    void AnimList_RequestAnimGroupForCreatures(i32 group_id, ...) {
        va_list creatures;
        va_start(creatures, group_id);
        while (true) {
            i32 creature_id = va_arg(creatures, i32);
            if (creature_id == -1) {
                break;
            }
            AnimList_RequestAnimGroups(creature_id, group_id, -1);
        }
        va_end(creatures);
    }

    void SetAnimBlendMode(i32 mode) {
        AnimBlendMode = mode;
    }

    i32 GetAnimBlendMode(void) {
        return AnimBlendMode;
    }

    APIOBJECTSYS_s *APIObjectSysInit(i32 size, VARIPTR *buf, VARIPTR *buf_end) {
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

    void APIObjectRemoveFromLOSTable(APIOBJECTSYS_s *system, APIOBJECT *source, APIOBJECT *target) {
        if (source != NULL) {
            system->line_of_sight[source->field_0x289] &= ~(1ULL << target->field_0x289);
        } else {
            u8 index = target->field_0x289;
            system->hostility_masks[index][0] = 0;
            system->hostility_masks[index][1] = 0;
        }
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
        if (object->apiobj.velocity.x != 0.0f || object->apiobj.velocity.z != 0.0f) {
            f32 horizontal_squared = object->apiobj.velocity.x * object->apiobj.velocity.x +
                                     object->apiobj.velocity.z * object->apiobj.velocity.z;
            object->apiobj.horizontal_velocity_magnitude = NuFsqrt(horizontal_squared);
            if (object->apiobj.velocity.y != 0.0f) {
                object->apiobj.velocity_magnitude =
                    NuFsqrt(object->apiobj.velocity.y * object->apiobj.velocity.y + horizontal_squared);
            } else {
                object->apiobj.velocity_magnitude = object->apiobj.horizontal_velocity_magnitude;
            }
        } else {
            object->apiobj.horizontal_velocity_magnitude = 0.0f;
            object->apiobj.velocity_magnitude = NuFabs(object->apiobj.velocity.y);
        }
    }

    void APICharacterSysInit(VARIPTR *buf, VARIPTR buf_end, i32 char_count, i32 model_capacity, i32 model_id_capacity,
                             i32 extra_capacity, CHARACTERDATA *cdata_list, APICHARACTERLIGHTFN set_creature_lights) {
        (void)buf_end;

        apicharsys = reinterpret_cast<APICHARACTERSYS *>(ALIGN(buf->addr, 0x10));
        buf->addr = ALIGN(buf->addr, 0x10);
        buf->addr += sizeof(*apicharsys);
        memset(apicharsys, 0, sizeof(*apicharsys));

        apicharsys->character_count = char_count;
        apicharsys->model_capacity = model_capacity;
        apicharsys->model_id_capacity = model_id_capacity;
        apicharsys->animation_capacity = extra_capacity;

        if (apicharsys->model_capacity != 0) {
            apicharsys->models = reinterpret_cast<APICHARACTERMODEL *>(ALIGN(buf->addr, 4));
            buf->addr = ALIGN(buf->addr, 4);
            buf->addr += (usize)apicharsys->model_capacity * sizeof(*apicharsys->models);
            memset(apicharsys->models, 0, (usize)apicharsys->model_capacity * sizeof(*apicharsys->models));

            for (i32 i = 0; i < apicharsys->model_capacity; i++) {
                APICHARACTERMODEL *model = &apicharsys->models[i];
                if (apicharsys->model_id_capacity != 0) {
                    model->model_data_a = reinterpret_cast<void **>(ALIGN(buf->addr, 4));
                    buf->addr = ALIGN(buf->addr, 4);
                    buf->addr += static_cast<usize>(apicharsys->model_id_capacity) * sizeof(void *);

                    model->model_data_b = reinterpret_cast<void **>(ALIGN(buf->addr, 4));
                    buf->addr = ALIGN(buf->addr, 4);
                    buf->addr += static_cast<usize>(apicharsys->model_id_capacity) * sizeof(void *);

                    model->model_data_c = reinterpret_cast<void **>(ALIGN(buf->addr, 4));
                    buf->addr = ALIGN(buf->addr, 4);
                    buf->addr += static_cast<usize>(apicharsys->model_id_capacity) * sizeof(void *);
                }
                APICharacterModelReset(model);
            }
        }

        if (apicharsys->character_count != 0) {
            apicharsys->playermodelids = reinterpret_cast<i16 *>(ALIGN(buf->addr, 4));
            buf->addr = ALIGN(buf->addr, 4);
            buf->addr += (usize)apicharsys->character_count * sizeof(*apicharsys->playermodelids);
            memset(apicharsys->playermodelids, 0,
                   (usize)apicharsys->character_count * sizeof(*apicharsys->playermodelids));
        }

        if (apicharsys->animation_capacity != 0) {
            apicharsys->animations = reinterpret_cast<ANIMLIST_s *>(ALIGN(buf->addr, 4));
            buf->addr = ALIGN(buf->addr, 4);
            buf->addr += (usize)apicharsys->animation_capacity * sizeof(*apicharsys->animations);
            memset(apicharsys->animations, 0, (usize)apicharsys->animation_capacity * sizeof(*apicharsys->animations));
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

static nuanimdata2_s *LoadAnimFromPAK(char *path, i32 area_animation, char *data, i32 size) {
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
        char animation_extension[] = ".an3";
        char deformation_extension[] = ".bsa";
        char animation_path[0x200];
        APICHARACTERMODEL *model;
        if (apiloadcharactermodels_append == 0) {
            APIResetCharacterRemap();
            model = apicharsys->models;
            for (i32 model_index = 0; model_index < apicharsys->permanent_model_count; ++model_index, ++model) {
                for (i32 animation_id = 0; animation_id < apicharsys->model_id_capacity; ++animation_id) {
                    if ((model->model_data_b[animation_id] != NULL || model->model_data_c[animation_id] != NULL) &&
                        (static_cast<CHARACTERANIM_s *>(model->model_data_a[animation_id])->flags & 1) == 0) {
                        model->model_data_a[animation_id] = NULL;
                        model->model_data_b[animation_id] = NULL;
                        model->model_data_c[animation_id] = NULL;
                    }
                }
            }
            for (i32 model_index = apicharsys->permanent_model_count; model_index < apicharsys->model_capacity;
                 ++model_index, ++model) {
                model->hierarchy = NULL;
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

            i32 model_loaded = 0;
            if (apicharsys->playermodelids[model_id] != -1) {
                model = &apicharsys->models[apicharsys->playermodelids[model_id]];
            } else {
                model = &apicharsys->models[apicharsys->loaded_model_count];
                APICharacterModelReset(model);

                NuStrCpy(animation_path, directory);
                NuStrCat(animation_path, character.file);
                NuStrCat(animation_path, ".ghg");

                model->hierarchy = NuGHGRead(animation_path, buf, buf_end);
                if (model->hierarchy == NULL) {
                    ++list;
                    continue;
                }
                for (i32 poi = 0; poi < 16; ++poi) {
                    model->points_of_interest[poi] = NuHGobjGetPOI(model->hierarchy, poi);
                }
                model_loaded = 1;
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
                for (; animations != NULL && animations->name != NULL; ++animations) {
                    if ((animations->flags & 0x8000) != 0 || !((area_animation != 0 && (animations->flags & 1) != 0) ||
                                                               (area_animation == 0 && (animations->flags & 1) == 0))) {
                        continue;
                    }
                    if ((animations->flags & 4) != 0 && model->model_data_b[animations->animation_id] == NULL) {
                        if (RedirectAnimFn == NULL ||
                            RedirectAnimFn(RedirectAnimDir, RedirectAnimList, animations, animation_path) == 0) {
                            NuStrCpy(animation_path, directory);
                            NuStrCat(animation_path, animations->name);
                        }
                        NuStrCat(animation_path, animation_extension);
                        i32 item = NuFilePakGetItem(pak, animation_path);
                        if (item != 0) {
                            NuFilePakSetItemRequired(pak, item, 1);
                        }
                    }
                    if ((animations->flags & 8) != 0 && model->model_data_c[animations->animation_id] == NULL) {
                        if (RedirectAnimFn == NULL ||
                            RedirectAnimFn(RedirectAnimDir, RedirectAnimList, animations, animation_path) == 0) {
                            NuStrCpy(animation_path, directory);
                            NuStrCat(animation_path, animations->name);
                        }
                        NuStrCat(animation_path, deformation_extension);
                        i32 item = NuFilePakGetItem(pak, animation_path);
                        if (item != 0) {
                            NuFilePakSetItemRequired(pak, item, 1);
                        }
                    }
                }

                buf->addr -= NuFilePakCondense(pak);
                animations = area_models != 0 && list->count != 0 ? apicharsys->char_data[model_id].animations : NULL;
                for (; animations != NULL && animations->name != NULL; ++animations) {
                    if ((animations->flags & 0x8000) == 0 && ((area_animation != 0 && (animations->flags & 1) != 0) ||
                                                              (area_animation == 0 && (animations->flags & 1) == 0))) {
                        if ((animations->flags & 4) != 0 && model->model_data_b[animations->animation_id] == NULL) {
                            if (RedirectAnimFn == NULL ||
                                RedirectAnimFn(RedirectAnimDir, RedirectAnimList, animations, animation_path) == 0) {
                                NuStrCpy(animation_path, directory);
                                NuStrCat(animation_path, animations->name);
                            }
                            NuStrCat(animation_path, animation_extension);
                            i32 item = NuFilePakGetItem(pak, animation_path);
                            void *data;
                            i32 size;
                            if (item != 0 && NuFilePakGetItemInfo(pak, item, &data, &size) != 0) {
                                const i32 pointer_block = static_cast<i32 *>(data)[1] > static_cast<i32>(0x414e4934);
                                ani3_animheader_s *joint_animation = reinterpret_cast<ani3_animheader_s *>(
                                    pointer_block ? static_cast<u8 *>(data) + 4 : data);
                                if (joint_animation->field_12_low == 0) {
                                    if (pointer_block) {
                                        NuPtrBlockFix(data);
                                    } else {
                                        NuAnimData2Fixup(size, &data);
                                    }
                                    joint_animation->field_12_low = 1;
                                }
                                model->model_data_b[animations->animation_id] = joint_animation;
                                model->model_data_a[animations->animation_id] = animations;
                            }
                        }
                        if ((animations->flags & 8) != 0 && model->model_data_c[animations->animation_id] == NULL) {
                            if (RedirectAnimFn == NULL ||
                                RedirectAnimFn(RedirectAnimDir, RedirectAnimList, animations, animation_path) == 0) {
                                NuStrCpy(animation_path, directory);
                                NuStrCat(animation_path, animations->name);
                            }
                            NuStrCat(animation_path, deformation_extension);
                            i32 item = NuFilePakGetItem(pak, animation_path);
                            void *data;
                            i32 size;
                            if (item != 0 && NuFilePakGetItemInfo(pak, item, &data, &size) != 0) {
                                model->model_data_c[animations->animation_id] =
                                    LoadAnimFromPAK(animation_path, area_animation, static_cast<char *>(data), size);
                                if (model->model_data_c[animations->animation_id] != NULL) {
                                    model->model_data_a[animations->animation_id] = animations;
                                }
                            }
                        }
                    }
                    // Packed BSA failure falls back to the character's own file,
                    // even for entries excluded from the packed-animation scan.
                    if ((animations->flags & 8) != 0 && model->model_data_c[animations->animation_id] == NULL) {
                        NuStrCpy(animation_path, directory);
                        NuStrCat(animation_path, animations->name);
                        NuStrCat(animation_path, ".bsa");
                        model->model_data_c[animations->animation_id] =
                            LoadAnim(animation_path, area_animation, buf, buf_end);
                        if (model->model_data_c[animations->animation_id] != NULL) {
                            model->model_data_a[animations->animation_id] = animations;
                        }
                    }
                }
            } else {
                for (; animations != NULL && animations->name != NULL; ++animations) {
                    if ((animations->flags & 0x8000) != 0 || !((area_animation != 0 && (animations->flags & 1) != 0) ||
                                                               (area_animation == 0 && (animations->flags & 1) == 0))) {
                        continue;
                    }
                    if ((animations->flags & 4) != 0 && model->model_data_b[animations->animation_id] == NULL) {
                        if (RedirectAnimFn == NULL ||
                            RedirectAnimFn(RedirectAnimDir, RedirectAnimList, animations, animation_path) == 0) {
                            NuStrCpy(animation_path, directory);
                            NuStrCat(animation_path, animations->name);
                        }
                        NuStrCat(animation_path, animation_extension);
                        if (NuFileExists(animation_path) != 0) {
                            model->model_data_b[animations->animation_id] =
                                LoadAnim(animation_path, area_animation, buf, buf_end);
                        }
                        if (model->model_data_b[animations->animation_id] != NULL) {
                            model->model_data_a[animations->animation_id] = animations;
                        }
                    }
                    if ((animations->flags & 8) != 0 && model->model_data_c[animations->animation_id] == NULL) {
                        NuStrCpy(animation_path, directory);
                        NuStrCat(animation_path, animations->name);
                        NuStrCat(animation_path, ".bsa");
                        model->model_data_c[animations->animation_id] =
                            LoadAnim(animation_path, area_animation, buf, buf_end);
                        if (model->model_data_c[animations->animation_id] != NULL) {
                            model->model_data_a[animations->animation_id] = animations;
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

    i32 FindAnimIX(CHARACTERDATA *character, char *name) {
        if (character != NULL) {
            CHARACTERANIM_s *animation = character->animations;
            while (animation != NULL && animation->name != NULL) {
                if (NuStrICmp(name, animation->name) == 0) {
                    return animation->animation_id;
                }
                ++animation;
            }
        }
        return -1;
    }

    f32 AnimDuration(i32 character_id, i32 animation, f32 start_frame, f32 end_frame, i32 subtract_frame_time) {
        if (apicharsys == NULL || character_id < 0 || character_id >= apicharsys->character_count || animation < 0 ||
            animation >= apicharsys->model_id_capacity) {
            return 0.0f;
        }

        const i16 model_index = apicharsys->playermodelids[character_id];
        if (model_index == -1) {
            return 0.0f;
        }

        CHARACTERMODEL_s *model = &apicharsys->models[model_index];
        if (model->model_data_b == NULL || model->model_data_b[animation] == NULL || model->model_data_a == NULL ||
            model->model_data_a[animation] == NULL) {
            return 0.0f;
        }

        f32 duration = NuAnimEndFrame(model->model_data_b[animation]);
        if (start_frame >= 1.0f && duration > start_frame) {
            if (end_frame >= 1.0f && duration > end_frame && end_frame > start_frame) {
                duration = end_frame - start_frame;
            } else {
                duration -= start_frame;
            }
        } else if (end_frame >= 1.0f && duration > end_frame && end_frame > start_frame) {
            duration = end_frame - 1.0f;
        } else {
            duration -= 1.0f;
        }

        CHARACTERANIM_s *animation_info = static_cast<CHARACTERANIM_s *>(model->model_data_a[animation]);
        duration *= (1.0f / (animation_info->playback_rate / 30.0f)) * (1.0f / 30.0f);
        animduration_blendouttime = animation_info->blend_out_time;
        if (subtract_frame_time != 0) {
            duration -= animduration_blendouttime;
        }
        return duration;
    }

    // Original @0x3cd56e. Build the hierarchy render-part list selected by
    // the caller's bit mask.
    i32 MakeLayerList_Index(CHARACTERMODEL_s *model, i16 *layers, u32 mask) {
        if (model == NULL) {
            return 0;
        }

        i32 count = 0;
        u32 layer_bit = 1;
        for (i32 layer = 0; layer <= 31 && layer < model->hierarchy->render_count; ++layer) {
            if ((mask & layer_bit) != 0) {
                *layers++ = static_cast<i16>(layer);
                ++count;
            }
            layer_bit <<= 1;
        }
        return count;
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

    void EvalModelAnim(CHARACTERMODEL_s *model, ANIMPACKET_s *packet, NUMTX *world_matrix, NUMTX *joint_matrices,
                       void ***dwa_output, NUVEC *locator_positions, NUMTX *locator_matrices, u32 layer_mask) {
        i16 render_indices[32];
        i32 render_count = MakeLayerList(model, render_indices, layer_mask);
        if (render_count <= 0) {
            return;
        }

        if (dwa_output != NULL) {
            if (packet->blending != 0 && packet->blend_animation_a >= 0 &&
                packet->blend_animation_a < apicharsys->model_id_capacity &&
                model->model_data_c[packet->blend_animation_a] != NULL && packet->blend_animation_b >= 0 &&
                packet->blend_animation_b < apicharsys->model_id_capacity &&
                model->model_data_c[packet->blend_animation_b] != NULL) {
                *dwa_output =
                    NuHGobjEvalDwaBlend2(render_count, render_indices,
                                         static_cast<nuanimdata2_s *>(model->model_data_c[packet->blend_animation_a]),
                                         packet->blend_source_time,
                                         static_cast<nuanimdata2_s *>(model->model_data_c[packet->blend_animation_b]),
                                         packet->blend_target_time, packet->blend_elapsed / packet->blend_duration);
            } else if (packet->blending != 0 && packet->blend_animation_b >= 0 &&
                       packet->blend_animation_b < apicharsys->model_id_capacity &&
                       model->model_data_c[packet->blend_animation_b] != NULL) {
                *dwa_output =
                    NuHGobjEvalDwa2(render_count, render_indices,
                                    static_cast<nuanimdata2_s *>(model->model_data_c[packet->blend_animation_b]),
                                    packet->blend_target_time);
            } else if (packet->blending != 0 && packet->blend_animation_a >= 0 &&
                       packet->blend_animation_a < apicharsys->model_id_capacity &&
                       model->model_data_c[packet->blend_animation_a] != NULL) {
                *dwa_output =
                    NuHGobjEvalDwa2(render_count, render_indices,
                                    static_cast<nuanimdata2_s *>(model->model_data_c[packet->blend_animation_a]),
                                    packet->blend_source_time);
            } else if (packet->blending == 0 && packet->animation_index >= 0 &&
                       packet->animation_index < apicharsys->model_id_capacity &&
                       model->model_data_c[packet->animation_index] != NULL) {
                *dwa_output = NuHGobjEvalDwa2(
                    render_count, render_indices,
                    static_cast<nuanimdata2_s *>(model->model_data_c[packet->animation_index]), packet->current_time);
            } else {
                *dwa_output = NULL;
            }
        }

        if (packet->blending != 0 && packet->blend_animation_a >= 0 &&
            packet->blend_animation_a < apicharsys->model_id_capacity &&
            model->model_data_b[packet->blend_animation_a] != NULL && packet->blend_animation_b >= 0 &&
            packet->blend_animation_b < apicharsys->model_id_capacity &&
            model->model_data_b[packet->blend_animation_b] != NULL) {
            NuHGobjEvalAnimBlend2(
                model->hierarchy, static_cast<ani3_animheader_s *>(model->model_data_b[packet->blend_animation_a]),
                packet->blend_source_time,
                static_cast<ani3_animheader_s *>(model->model_data_b[packet->blend_animation_b]),
                packet->blend_target_time, packet->blend_elapsed / packet->blend_duration, 0, NULL, joint_matrices);
        } else if (packet->blending != 0 && packet->blend_animation_b >= 0 &&
                   packet->blend_animation_b < apicharsys->model_id_capacity &&
                   model->model_data_b[packet->blend_animation_b] != NULL) {
            NuHGobjEvalAnim2(model->hierarchy,
                             static_cast<ani3_animheader_s *>(model->model_data_b[packet->blend_animation_b]),
                             packet->blend_target_time, 0, NULL, joint_matrices);
        } else if (packet->blending != 0 && packet->blend_animation_a >= 0 &&
                   packet->blend_animation_a < apicharsys->model_id_capacity &&
                   model->model_data_b[packet->blend_animation_a] != NULL) {
            NuHGobjEvalAnim2(model->hierarchy,
                             static_cast<ani3_animheader_s *>(model->model_data_b[packet->blend_animation_a]),
                             packet->blend_source_time, 0, NULL, joint_matrices);
        } else if (packet->blending == 0 && packet->animation_index >= 0 &&
                   packet->animation_index < apicharsys->model_id_capacity &&
                   model->model_data_b[packet->animation_index] != NULL) {
            NuHGobjEvalAnim2(model->hierarchy,
                             static_cast<ani3_animheader_s *>(model->model_data_b[packet->animation_index]),
                             packet->current_time, 0, NULL, joint_matrices);
        } else {
            NuHGobjEval(model->hierarchy, 0, NULL, joint_matrices);
        }
        StoreLocatorCoordinates(model, world_matrix, joint_matrices, locator_positions, locator_matrices);
    }

    f32 BlendTimeBetweenAnims(CHARACTERMODEL_s *model, i32 source_animation, i32 target_animation) {
        if (model->model_data_a[source_animation] == NULL || model->model_data_a[target_animation] == NULL) {
            return 0.0f;
        }

        f32 time = static_cast<CHARACTERANIM_s *>(model->model_data_a[target_animation])->blend_in_time;
        const f32 source_time = static_cast<CHARACTERANIM_s *>(model->model_data_a[source_animation])->blend_out_time;
        if (time > source_time) {
            time = source_time;
        }
        return time;
    }

    void ResetAnimPacket(ANIMPACKET_s *packet, i16 animation) {
        if (packet == NULL) {
            return;
        }
        packet->requested_animation = animation;
        packet->previous_animation = packet->requested_animation;
        packet->animation_index = packet->previous_animation;
        packet->previous_time = 1.0f;
        packet->blend_target_time = packet->previous_time;
        packet->current_time = packet->blend_target_time;
        packet->blending = 0;
        packet->flags = ANIMPACKET_FLAG_ANIMATION_CHANGED;
        packet->overlay_animation = -1;
        packet->current_reversed = 0;
        packet->blend_source_reversed = 0;
        packet->blend_target_reversed = 0;
    }

    void ResetMiniAnimPacket(MINIANIMPACKET_s *packet, i32 animation) {
        if (packet != NULL) {
            packet->requested_animation_id = animation;
            packet->previous_animation_id = packet->requested_animation_id;
            packet->current_animation_id = packet->previous_animation_id;
            packet->previous_time = 1.0f;
            packet->blend_target_time = packet->previous_time;
            packet->current_time = packet->blend_target_time;
            packet->blending = 0;
            packet->flags = ANIMPACKET_FLAG_ANIMATION_CHANGED;
        }
    }

    void SetAnimTimeRandom(CHARACTERMODEL_s *model, ANIMPACKET_s *packet) {
        if (model == NULL || packet == NULL) {
            return;
        }
        void *animation = model->model_data_b[packet->field_0x3a];
        if (animation != NULL) {
            const f32 random = NuRandFloat();
            const f32 end_frame = NuAnimEndFrame(model->model_data_b[packet->field_0x3a]);
            packet->field_0x00 = 1.0f + random * (end_frame - 1.0f);
        }
    }

    f32 GetAnimTimeRandom(CHARACTERMODEL_s *model, i32 animation) {
        if (model == NULL || model->model_data_b[animation] == NULL) {
            return 0.0f;
        }
        return 1.0f + NuRandFloat() * (NuAnimEndFrame(model->model_data_b[animation]) - 1.0f);
    }

} // extern "C"

static f32 UpdateAnimTimer(CHARACTERMODEL_s *model, ANIMPACKET_s *packet, i16 animation, f32 time, f32 frame_step,
                           f32 movement_speed, i32 report_events, char *reversed, i32 backwards,
                           f32 backwards_multiplier) {
    i32 looped = 0;
    f32 rate = static_cast<CHARACTERANIM_s *>(model->model_data_a[animation])->playback_rate;
    f32 reference_speed = static_cast<CHARACTERANIM_s *>(model->model_data_a[animation])->movement_speed;
    if (reference_speed > 0.0f) {
        rate *= movement_speed / reference_speed;
        const f32 rate_cap = static_cast<CHARACTERANIM_s *>(model->model_data_a[animation])->movement_rate_cap;
        if (rate < 0.0f) {
            if (rate_cap < 0.0f && rate < rate_cap) {
                rate = rate_cap;
            }
        } else if (rate_cap > 0.0f && rate > rate_cap) {
            rate = rate_cap;
        }
    }
    if (*reversed != 0) {
        rate *= -(frame_step * backwards_multiplier) / 30.0f;
    } else {
        rate *= frame_step / 30.0f;
    }
    time += rate;
    const f32 end_frame = NuAnimEndFrame(model->model_data_b[animation]);

    // Original 0x3ce29f only enters reverse playback for an ordered negative delta.
    if (rate < 0.0f) {
        if (report_events) {
            packet->flags |= ANIMPACKET_FLAG_PLAYING_REVERSED;
        }
        if (time < 1.0f) {
            if ((static_cast<CHARACTERANIM_s *>(model->model_data_a[animation])->flags &
                 CHARACTER_ANIMATION_FLAG_SYNCHRONISED) != 0) {
                if (end_frame > 1.0f) {
                    while (time < 1.0f) {
                        time += end_frame - 1.0f;
                    }
                } else {
                    time = 1.0f;
                }
                if (report_events) {
                    packet->flags |= ANIMPACKET_FLAG_LOOPED;
                }
                looped = 1;
            } else {
                time = 1.0f;
                if (report_events) {
                    packet->flags |= ANIMPACKET_FLAG_FINISHED;
                }
            }
        }
    } else {
        if (time > end_frame) {
            if ((static_cast<CHARACTERANIM_s *>(model->model_data_a[animation])->flags &
                 CHARACTER_ANIMATION_FLAG_SYNCHRONISED) != 0) {
                if (end_frame > 1.0f) {
                    while (time > end_frame) {
                        time -= end_frame - 1.0f;
                    }
                } else {
                    time = 1.0f;
                }
                if (report_events) {
                    packet->flags |= ANIMPACKET_FLAG_LOOPED;
                }
                looped = 1;
            } else {
                time = end_frame;
                if (report_events) {
                    packet->flags |= ANIMPACKET_FLAG_FINISHED;
                }
            }
        }
    }

    if (looped) {
        if (*reversed != 0 && backwards == 0) {
            *reversed = 0;
        } else if (*reversed == 0 && backwards != 0 &&
                   (static_cast<CHARACTERANIM_s *>(model->model_data_a[animation])->flags &
                    CHARACTER_ANIMATION_FLAG_REVERSE_WITH_MOVEMENT) != 0) {
            *reversed = 1;
        }
    }
    return time;
}

extern "C" {

    void UpdateAnimPacket(CHARACTERMODEL_s *model, ANIMPACKET_s *packet, f32 frame_step, f32 movement_speed,
                          f32 blend_step, f32 backwards_multiplier) {
        i32 backwards = 0;
        i32 interrupted_reversed = 0;
        i32 interrupted = 0;
        if (movement_speed < 0.0f) {
            backwards = 1;
            movement_speed = -movement_speed;
        }

        if (model == NULL || packet == NULL) {
            return;
        }

        const i32 paused = packet->flags & ANIMPACKET_FLAG_PAUSED;
        const i32 force_restart = packet->flags & ANIMPACKET_FLAG_FORCE_RESTART;
        packet->flags = 0;
        if (packet->blending != 0) {
            packet->previous_time = packet->blend_target_time;
        } else {
            packet->previous_time = packet->current_time;
        }
        if (frame_step == 0.0f) {
            packet->flags |= ANIMPACKET_FLAG_ZERO_TIMESTEP;
            return;
        }

        if (packet->overlay_animation == -1) {
            if (packet->blending != 0) {
                if (packet->requested_animation != -1 && packet->requested_animation != packet->blend_animation_b &&
                    model->model_data_b[packet->requested_animation] != NULL &&
                    model->model_data_a[packet->requested_animation] != NULL &&
                    static_cast<CHARACTERANIM_s *>(model->model_data_a[packet->requested_animation])->blend_in_time ==
                        0.0f) {
                    packet->animation_index = packet->requested_animation;
                    if (backwards != 0 && model->model_data_b[packet->animation_index] != NULL &&
                        model->model_data_a[packet->animation_index] != NULL &&
                        (static_cast<CHARACTERANIM_s *>(model->model_data_a[packet->animation_index])->flags &
                         CHARACTER_ANIMATION_FLAG_REVERSE_WITH_MOVEMENT) != 0) {
                        packet->current_reversed = 1;
                        packet->current_time = NuAnimEndFrame(model->model_data_b[packet->animation_index]);
                    } else {
                        packet->current_reversed = 0;
                        packet->current_time = 1.0f;
                    }
                    packet->blending = 0;
                    packet->previous_time = packet->current_time;
                    packet->flags |= ANIMPACKET_FLAG_ANIMATION_CHANGED;
                    goto update_timers;
                }

                packet->blend_elapsed += blend_step;
                if (packet->blend_elapsed >= packet->blend_duration) {
                    packet->blending = 0;
                    packet->animation_index = packet->blend_animation_b;
                    packet->current_time = packet->blend_target_time;
                    packet->current_reversed = packet->blend_target_reversed;
                    packet->flags |= ANIMPACKET_FLAG_BLEND_FINISHED;
                    goto update_timers;
                }

                if (AnimBlendMode != 1 || packet->requested_animation == packet->blend_animation_b ||
                    !(packet->requested_animation != -1 && model->model_data_b[packet->requested_animation] != NULL)) {
                    goto update_timers;
                }

                if (packet->blend_elapsed < packet->blend_duration * 0.5f) {
                    packet->previous_animation = packet->blend_animation_a;
                    interrupted_reversed = static_cast<i8>(packet->blend_source_reversed);
                    interrupted = 1;
                } else {
                    packet->previous_animation = packet->blend_animation_b;
                    interrupted_reversed = static_cast<i8>(packet->blend_target_reversed);
                    packet->blend_source_time = packet->blend_target_time;
                    interrupted = 1;
                }
            } else if (packet->requested_animation == packet->previous_animation) {
                // Interrupted blends enter the transition directly, even when
                // returning to their source animation (retail 0x3ce858/0x3ce88a).
                if (force_restart == 0 || packet->requested_animation != packet->animation_index ||
                    !(packet->animation_index != -1 && model->model_data_b[packet->animation_index] != NULL)) {
                    packet->animation_index = packet->requested_animation;
                    packet->blending = 0;
                    goto update_timers;
                }
            }

            if (packet->previous_animation != -1 && packet->requested_animation != -1 &&
                model->model_data_b[packet->previous_animation] != NULL &&
                model->model_data_b[packet->requested_animation] != NULL) {
                if (static_cast<CHARACTERANIM_s *>(model->model_data_a[packet->previous_animation]) != NULL &&
                    static_cast<CHARACTERANIM_s *>(model->model_data_a[packet->requested_animation]) != NULL &&
                    static_cast<CHARACTERANIM_s *>(model->model_data_a[packet->previous_animation])->blend_out_time >
                        blend_step &&
                    static_cast<CHARACTERANIM_s *>(model->model_data_a[packet->requested_animation])->blend_in_time >
                        blend_step) {
                    packet->blending = 1;
                    packet->blend_animation_a = packet->previous_animation;
                    packet->blend_source_reversed = static_cast<u8>(interrupted_reversed);
                    packet->blend_animation_b = packet->requested_animation;
                    if (interrupted == 0) {
                        packet->blend_source_time = packet->current_time;
                    }
                    packet->blend_source_reversed = packet->current_reversed;

                    // End-frame callbacks can change the animation entries and packet indices.
                    if ((static_cast<CHARACTERANIM_s *>(model->model_data_a[packet->blend_animation_a])->flags &
                         CHARACTER_ANIMATION_FLAG_SYNCHRONISED) != 0 &&
                        (static_cast<CHARACTERANIM_s *>(model->model_data_a[packet->blend_animation_b])->flags &
                         CHARACTER_ANIMATION_FLAG_SYNCHRONISED) != 0 &&
                        static_cast<CHARACTERANIM_s *>(model->model_data_a[packet->blend_animation_a])->playback_rate ==
                            static_cast<CHARACTERANIM_s *>(model->model_data_a[packet->blend_animation_b])
                                ->playback_rate &&
                        NuAnimEndFrame(model->model_data_b[packet->blend_animation_a]) ==
                            NuAnimEndFrame(model->model_data_b[packet->blend_animation_b])) {
                        packet->blend_target_time = packet->blend_source_time;
                        packet->blend_target_reversed =
                            backwards != 0 && model->model_data_b[packet->blend_animation_b] != NULL &&
                                    (static_cast<CHARACTERANIM_s *>(model->model_data_a[packet->blend_animation_b])
                                         ->flags &
                                     CHARACTER_ANIMATION_FLAG_REVERSE_WITH_MOVEMENT) != 0
                                ? 1
                                : 0;
                    } else if (backwards != 0 && model->model_data_b[packet->blend_animation_b] != NULL &&
                               (static_cast<CHARACTERANIM_s *>(model->model_data_a[packet->blend_animation_b])->flags &
                                CHARACTER_ANIMATION_FLAG_REVERSE_WITH_MOVEMENT) != 0) {
                        packet->blend_target_reversed = 1;
                        packet->blend_target_time = NuAnimEndFrame(model->model_data_b[packet->blend_animation_b]);
                    } else {
                        packet->blend_target_reversed = 0;
                        packet->blend_target_time = 1.0f;
                    }
                    packet->blend_elapsed = 0.0f;
                    packet->blend_duration =
                        static_cast<CHARACTERANIM_s *>(model->model_data_a[packet->blend_animation_b])->blend_in_time;
                    if (static_cast<CHARACTERANIM_s *>(model->model_data_a[packet->blend_animation_a])->blend_out_time <
                        packet->blend_duration) {
                        packet->blend_duration =
                            static_cast<CHARACTERANIM_s *>(model->model_data_a[packet->blend_animation_a])
                                ->blend_out_time;
                    }
                    packet->flags |= ANIMPACKET_FLAG_ANIMATION_CHANGED;
                    goto update_timers;
                }
            }

            packet->animation_index = packet->requested_animation;
            if (backwards != 0 && packet->animation_index != -1 &&
                model->model_data_b[packet->animation_index] != NULL &&
                model->model_data_a[packet->animation_index] != NULL &&
                (static_cast<CHARACTERANIM_s *>(model->model_data_a[packet->animation_index])->flags &
                 CHARACTER_ANIMATION_FLAG_REVERSE_WITH_MOVEMENT) != 0) {
                packet->current_reversed = 1;
                packet->current_time = NuAnimEndFrame(model->model_data_b[packet->animation_index]);
            } else {
                packet->current_reversed = 0;
                packet->current_time = 1.0f;
            }
            packet->blending = 0;
            packet->previous_time = packet->current_time;
            packet->flags |= ANIMPACKET_FLAG_ANIMATION_CHANGED;
        }

    update_timers:
        // End-frame services can change overlay state; retail reloads it here.
        if (packet->overlay_animation != -1) {
            if ((packet->requested_animation != -1 && model->model_data_b[packet->requested_animation] != NULL) &&
                model->model_data_b[packet->overlay_animation] != NULL) {
                packet->blend_source_time = UpdateAnimTimer(
                    model, packet, packet->requested_animation, packet->blend_source_time, frame_step, movement_speed,
                    0, reinterpret_cast<char *>(&packet->blend_source_reversed), backwards, backwards_multiplier);
                packet->blend_target_time = UpdateAnimTimer(
                    model, packet, packet->overlay_animation, packet->blend_target_time, frame_step, movement_speed, 1,
                    reinterpret_cast<char *>(&packet->blend_target_reversed), backwards, backwards_multiplier);
            }
        } else if (packet->blending != 0) {
            if ((packet->blend_animation_a != -1 && model->model_data_b[packet->blend_animation_a] != NULL) &&
                (packet->blend_animation_b != -1 && model->model_data_b[packet->blend_animation_b] != NULL)) {
                packet->blend_source_time = UpdateAnimTimer(
                    model, packet, packet->blend_animation_a, packet->blend_source_time, frame_step, movement_speed, 0,
                    reinterpret_cast<char *>(&packet->blend_source_reversed), backwards, backwards_multiplier);
                packet->blend_target_time = UpdateAnimTimer(
                    model, packet, packet->blend_animation_b, packet->blend_target_time, frame_step, movement_speed, 1,
                    reinterpret_cast<char *>(&packet->blend_target_reversed), backwards, backwards_multiplier);
            }
        } else if (packet->animation_index != -1 && model->model_data_b[packet->animation_index] != NULL) {
            if (paused != 0) {
                frame_step = 0.0f;
            }
            packet->current_time = UpdateAnimTimer(
                model, packet, packet->animation_index, packet->current_time, frame_step, movement_speed, 1,
                reinterpret_cast<char *>(&packet->current_reversed), backwards, backwards_multiplier);
        } else {
            const i32 requested_animation = packet->requested_animation;
            ResetAnimPacket(packet, -1);
            packet->requested_animation = requested_animation;
        }
    }

    void AnimPacket_MiniToFull(MINIANIMPACKET_s *mini_packet, ANIMPACKET_s *packet) {
        packet->current_time = mini_packet->current_time;
        packet->previous_time = mini_packet->previous_time;
        packet->blend_elapsed = mini_packet->blend_elapsed;
        packet->blend_duration = mini_packet->blend_duration;
        packet->blend_source_time = mini_packet->blend_source_time;
        packet->blend_target_time = mini_packet->blend_target_time;
        packet->flags = mini_packet->flags;
        packet->blending = mini_packet->blending;
        packet->blend_animation_a = mini_packet->blend_animation_a;
        packet->blend_animation_b = mini_packet->blend_animation_b;
        packet->animation_index = mini_packet->animation_index;
        packet->previous_animation = mini_packet->previous_animation;
        packet->requested_animation = mini_packet->requested_animation_id;
        packet->blend_source_reversed = 0;
        packet->blend_target_reversed = 0;
        packet->current_reversed = 0;
        packet->overlay_animation = -1;
    }

    void AnimPacket_FullToMini(ANIMPACKET_s *packet, MINIANIMPACKET_s *mini_packet) {
        mini_packet->current_time = packet->current_time;
        mini_packet->previous_time = packet->previous_time;
        mini_packet->blend_elapsed = packet->blend_elapsed;
        mini_packet->blend_duration = packet->blend_duration;
        mini_packet->blend_source_time = packet->blend_source_time;
        mini_packet->blend_target_time = packet->blend_target_time;
        mini_packet->flags = packet->flags;
        mini_packet->blending = packet->blending;
        mini_packet->blend_animation_a = packet->blend_animation_a;
        mini_packet->blend_animation_b = packet->blend_animation_b;
        mini_packet->animation_index = packet->animation_index;
        mini_packet->previous_animation = packet->previous_animation;
        mini_packet->requested_animation_id = packet->requested_animation;
    }

    void UpdateMiniAnimPacket(CHARACTERMODEL_s *model, MINIANIMPACKET_s *mini_packet, f32 frame_step,
                              f32 movement_speed, f32 blend_step) {
        ANIMPACKET_s packet;
        AnimPacket_MiniToFull(mini_packet, &packet);
        UpdateAnimPacket(model, &packet, frame_step, movement_speed, blend_step, 0.0f);
        AnimPacket_FullToMini(&packet, mini_packet);
    }

    i32 AnimBlendingFromTo(CHARACTERMODEL_s *model, ANIMPACKET_s *packet, i32 source_animation, i32 target_animation) {
        if (packet->blending != 0 && source_animation != -1 && packet->blend_animation_a == source_animation &&
            target_animation != -1 && packet->blend_animation_b == target_animation) {
            if (model != NULL) {
                if (source_animation == -1 || model->model_data_b[source_animation] == NULL) {
                    return 0;
                }
                if (target_animation == -1 || model->model_data_b[target_animation] == NULL) {
                    return 0;
                }
            }
            return 1;
        }
        return 0;
    }

    f32 *AnimPlaying(ANIMPACKET_s *packet, i32 animation, i32 target, i32 source) {
        if (packet == NULL || animation == -1)
            return NULL;
        if (packet->blending != 0) {
            if (target != 0 && packet->blend_animation_b == animation)
                return &packet->blend_target_time;
            if (source != 0 && packet->blend_animation_a == animation)
                return &packet->blend_source_time;
        } else {
            if (packet->animation_index == animation)
                return &packet->current_time;
        }
        return NULL;
    }

    i32 CurrentAnim(ANIMPACKET_s *packet) {
        return packet->blending == 0 ? packet->animation_index : packet->blend_animation_b;
    }

    f32 AnimSpeed(CHARACTERMODEL_s *model, i32 animation) {
        return animation != -1 && model->model_data_b[animation] != NULL
                   ? static_cast<CHARACTERANIM_s *>(model->model_data_a[animation])->action_speed
                   : 0.0f;
    }

    f32 AnimListFrame(CHARACTERMODEL_s *model, i32 animation, i32 frame) {
        if (animation == -1 || model->model_data_b[animation] == NULL || frame < 0 || frame > 3) {
            return 0.0f;
        }
        CHARACTERANIM_s *info = static_cast<CHARACTERANIM_s *>(model->model_data_a[animation]);
        return info->event_frames[frame];
    }

    f32 AnimStopFrame(CHARACTERMODEL_s *model, i32 animation) {
        return animation != -1 && model->model_data_b[animation] != NULL
                   ? static_cast<CHARACTERANIM_s *>(model->model_data_a[animation])->stop_frame
                   : 0.0f;
    }

    f32 *AnimListFrameArray(CHARACTERMODEL_s *model, i32 animation) {
        if (animation == -1 || model->model_data_b[animation] == NULL) {
            return NULL;
        }
        CHARACTERANIM_s *info = static_cast<CHARACTERANIM_s *>(model->model_data_a[animation]);
        return info->event_frames;
    }

    i32 AnimsAvailableToBothCharacters(ANIMPACKET_s *packet, i32 first_character, i32 second_character) {
        if (first_character == -1) {
            return 0;
        }
        i32 first_model_index = apicharsys->playermodelids[first_character];
        if (first_model_index == -1) {
            return 0;
        }
        CHARACTERMODEL_s *first_model = &apicharsys->models[first_model_index];

        if (second_character == -1) {
            return 0;
        }
        i32 second_model_index = apicharsys->playermodelids[second_character];
        if (second_model_index == -1) {
            return 0;
        }
        CHARACTERMODEL_s *second_model = &apicharsys->models[second_model_index];

        if (packet->blending != 0) {
            return first_model->model_data_b[packet->blend_animation_a] != NULL &&
                   first_model->model_data_b[packet->blend_animation_b] != NULL &&
                   second_model->model_data_b[packet->blend_animation_a] != NULL &&
                   second_model->model_data_b[packet->blend_animation_b] != NULL;
        }
        return first_model->model_data_b[packet->animation_index] != NULL &&
               second_model->model_data_b[packet->animation_index] != NULL;
    }

} // extern "C"

void RootFnEx(NUMTX *matrix, void *data, NUVEC *sampled_root, NUVEC *, NUVEC *translation, f32, i32 include_y) {
    APIOBJECT *object = static_cast<APIOBJECT *>(data);

    if (object->previous_animation_root_time > object->anim_packet.current_time) {
        object->previous_animation_root = *sampled_root;
    }

    object->animation_root_delta.x = sampled_root->x - object->previous_animation_root.x;
    object->animation_root_delta.y = include_y ? sampled_root->y - object->previous_animation_root.y : 0.0f;
    object->animation_root_delta.z = sampled_root->z - object->previous_animation_root.z;
    NuVecMtxRotate(&object->animation_root_delta, &object->animation_root_delta, &object->field_0xb8);
    if (!include_y) {
        object->animation_root_delta.y = 0.0f;
    }

    object->previous_animation_root = *sampled_root;
    object->previous_animation_root_time = object->anim_packet.current_time;

    if (object->anim_packet.requested_animation != -1) {
        CHARACTERANIM_s *animation = static_cast<CHARACTERANIM_s *>(
            object->character_model->model_data_a[object->anim_packet.requested_animation]);
        matrix->m30 = translation->x + animation->root_translation.x;
        matrix->m32 = translation->z + animation->root_translation.z;
        if (include_y) {
            matrix->m31 = translation->y + animation->root_translation.y;
        }
    } else {
        matrix->m30 = translation->x;
        if (include_y) {
            matrix->m31 = translation->y;
        }
        matrix->m32 = translation->z;
    }

    if (include_y && object->animation_root_delta.y == 0.0f) {
        object->animation_root_delta.y = 1.0e-11f;
    } else if (object->animation_root_delta.x == 0.0f && object->animation_root_delta.z == 0.0f) {
        object->animation_root_delta.x = 1.0e-11f;
    }
}

extern "C" {

    void RootFn(NUMTX *matrix, void *data, NUVEC *source_root, NUVEC *target_root, NUVEC *root_delta, f32 blend) {
        RootFnEx(matrix, data, source_root, target_root, root_delta, blend, 0);
    }

    void RootFnY(NUMTX *matrix, void *data, NUVEC *source_root, NUVEC *target_root, NUVEC *root_delta, f32 blend) {
        RootFnEx(matrix, data, source_root, target_root, root_delta, blend, 1);
    }

    static void NuVecClear(NUVEC *vector) {
        vector->x = vector->y = vector->z = 0.0f;
    }

    void BlendRootFn(NUMTX *matrix, void *data, NUVEC *source_root, NUVEC *target_root, NUVEC *root_delta, f32 blend) {
        APIOBJECT *object = static_cast<APIOBJECT *>(data);

        // Animation callbacks can change the model or packet; read their current animation entries.
        NUVEC root_offsets[2];
        NUVEC root_motion[2];
        NUVEC root_position[2];

        if ((static_cast<CHARACTERANIM_s *>(
                 object->character_model->model_data_a[object->anim_packet.blend_animation_a])
                 ->flags &
             CHARACTER_ANIMATION_FLAG_ROOT_MOTION) != 0) {
            if (object->previous_animation_root_time > object->anim_packet.blend_source_time ||
                static_cast<CHARACTERANIM_s *>(
                    object->character_model->model_data_a[object->anim_packet.blend_animation_a]) !=
                    object->previous_animation_root_info) {
                object->previous_animation_root = *source_root;
                object->previous_animation_root_info = static_cast<CHARACTERANIM_s *>(
                    object->character_model->model_data_a[object->anim_packet.blend_animation_a]);
            }

            root_motion[0].x = source_root->x - object->previous_animation_root.x;
            if ((static_cast<CHARACTERANIM_s *>(
                     object->character_model->model_data_a[object->anim_packet.blend_animation_a])
                     ->flags &
                 CHARACTER_ANIMATION_FLAG_VERTICAL_ROOT_MOTION) != 0) {
                root_motion[0].y = source_root->y - object->previous_animation_root.y;
            } else {
                root_motion[0].y = 0.0f;
            }
            root_motion[0].z = source_root->z - object->previous_animation_root.z;
            object->previous_animation_root = *source_root;

            root_position[0].x = 0.0f;
            if ((static_cast<CHARACTERANIM_s *>(
                     object->character_model->model_data_a[object->anim_packet.blend_animation_a])
                     ->flags &
                 CHARACTER_ANIMATION_FLAG_VERTICAL_ROOT_MOTION) != 0) {
                root_position[0].y = 0.0f;
            } else {
                root_position[0].y = source_root->y;
            }
            root_position[0].z = 0.0f;
            object->previous_animation_root_time = object->anim_packet.blend_source_time;
        } else {
            NuVecClear(&root_motion[0]);
            root_position[0] = *source_root;
            object->previous_animation_root_time = FLT_MAX;
        }

        root_offsets[0].x =
            static_cast<CHARACTERANIM_s *>(object->character_model->model_data_a[object->anim_packet.blend_animation_a])
                ->root_translation.x;
        root_offsets[0].y =
            static_cast<CHARACTERANIM_s *>(object->character_model->model_data_a[object->anim_packet.blend_animation_a])
                ->root_translation.y;
        root_offsets[0].z =
            static_cast<CHARACTERANIM_s *>(object->character_model->model_data_a[object->anim_packet.blend_animation_a])
                ->root_translation.z;

        if ((static_cast<CHARACTERANIM_s *>(
                 object->character_model->model_data_a[object->anim_packet.blend_animation_b])
                 ->flags &
             CHARACTER_ANIMATION_FLAG_ROOT_MOTION) != 0) {
            if (object->previous_blend_target_root_time > object->anim_packet.blend_target_time ||
                static_cast<CHARACTERANIM_s *>(
                    object->character_model->model_data_a[object->anim_packet.blend_animation_b]) !=
                    object->previous_blend_target_root_info) {
                object->previous_blend_target_root = *target_root;
                object->previous_blend_target_root_info = static_cast<CHARACTERANIM_s *>(
                    object->character_model->model_data_a[object->anim_packet.blend_animation_b]);
            }

            root_motion[1].x = target_root->x - object->previous_blend_target_root.x;
            if ((static_cast<CHARACTERANIM_s *>(
                     object->character_model->model_data_a[object->anim_packet.blend_animation_b])
                     ->flags &
                 CHARACTER_ANIMATION_FLAG_VERTICAL_ROOT_MOTION) != 0) {
                root_motion[1].y = target_root->y - object->previous_blend_target_root.y;
            } else {
                root_motion[1].y = 0.0f;
            }
            root_motion[1].z = target_root->z - object->previous_blend_target_root.z;
            object->previous_blend_target_root = *target_root;

            root_position[1].x = 0.0f;
            if ((static_cast<CHARACTERANIM_s *>(
                     object->character_model->model_data_a[object->anim_packet.blend_animation_b])
                     ->flags &
                 CHARACTER_ANIMATION_FLAG_VERTICAL_ROOT_MOTION) != 0) {
                root_position[1].y = 0.0f;
            } else {
                root_position[1].y = target_root->y;
            }
            root_position[1].z = 0.0f;
            object->previous_blend_target_root_time = object->anim_packet.blend_target_time;
        } else {
            NuVecClear(&root_motion[1]);
            root_position[1] = *target_root;
            object->previous_blend_target_root_time = FLT_MAX;
        }

        root_offsets[1].x =
            static_cast<CHARACTERANIM_s *>(object->character_model->model_data_a[object->anim_packet.blend_animation_b])
                ->root_translation.x;
        root_offsets[1].y =
            static_cast<CHARACTERANIM_s *>(object->character_model->model_data_a[object->anim_packet.blend_animation_b])
                ->root_translation.y;
        root_offsets[1].z =
            static_cast<CHARACTERANIM_s *>(object->character_model->model_data_a[object->anim_packet.blend_animation_b])
                ->root_translation.z;

        NuVecLerp(NUMTX_GET_ROW_VEC(matrix, 3), &root_position[1], &root_position[0], blend);

        NUVEC blended_offset;
        NuVecLerp(&blended_offset, &root_offsets[1], &root_offsets[0], blend);
        root_delta->x += blended_offset.x;
        root_delta->y += blended_offset.y;
        root_delta->z += blended_offset.z;
        NuMtxTranslate(matrix, root_delta);

        NuVecMtxRotate(&root_motion[0], &root_motion[0], &object->field_0xb8);
        if ((static_cast<CHARACTERANIM_s *>(
                 object->character_model->model_data_a[object->anim_packet.blend_animation_a])
                 ->flags &
             CHARACTER_ANIMATION_FLAG_VERTICAL_ROOT_MOTION) == 0) {
            root_motion[0].y = 0.0f;
        }
        NuVecMtxRotate(&root_motion[1], &root_motion[1], &object->field_0xb8);
        if ((static_cast<CHARACTERANIM_s *>(
                 object->character_model->model_data_a[object->anim_packet.blend_animation_b])
                 ->flags &
             CHARACTER_ANIMATION_FLAG_VERTICAL_ROOT_MOTION) == 0) {
            root_motion[1].y = 0.0f;
        }
        NuVecLerp(&object->animation_root_delta, &root_motion[1], &root_motion[0], blend);

        if (((static_cast<CHARACTERANIM_s *>(
                  object->character_model->model_data_a[object->anim_packet.blend_animation_a])
                  ->flags &
              CHARACTER_ANIMATION_FLAG_VERTICAL_ROOT_MOTION) != 0 ||
             (static_cast<CHARACTERANIM_s *>(
                  object->character_model->model_data_a[object->anim_packet.blend_animation_b])
                  ->flags &
              CHARACTER_ANIMATION_FLAG_VERTICAL_ROOT_MOTION) != 0) &&
            object->animation_root_delta.y == 0.0f) {
            object->animation_root_delta.y = 1.0e-11f;
        } else if (object->animation_root_delta.x == 0.0f && object->animation_root_delta.y == 0.0f &&
                   object->animation_root_delta.z == 0.0f) {
            object->animation_root_delta.x = 1.0e-11f;
        }
    }

    i32 ParticlesPerFrame(f32 particles_per_frame, f32 frame_time) {
        i32 scaled_count = static_cast<i32>(particles_per_frame * 65536.0f * (frame_time * 60.0f));
        i32 count = 0;

        while (scaled_count > 0xffff) {
            scaled_count -= 0x10000;
            ++count;
        }
        if ((NuRandInt() >> 16) < static_cast<u32>(scaled_count)) {
            ++count;
        }
        return count;
    }

    i32 ParticlesPerSecond(f32 particles_per_second, f32 frame_time) {
        return ParticlesPerFrame(particles_per_second / 60.0f, frame_time);
    }

    i32 FindGameDebris(APIDEBRISSYS_s *debris_sys, char *name) {
        for (i32 index = debris_sys->named_count; index < debris_sys->capacity; ++index) {
            if (NuStrICmp(name, debris_sys->entries[index].name) == 0) {
                return index;
            }
        }
        return -1;
    }

    extern i32 EDPP_MAX_TYPES;

    APIDEBRISSYS_s *InitGameDebris(VARIPTR *cursor, VARIPTR end, i32 count, i32 flags, char **names, char page) {
        (void)end;
        if (cursor->addr == 0) {
            return NULL;
        }

        APIDEBRISSYS_s *sys = reinterpret_cast<APIDEBRISSYS_s *>(ALIGN(cursor->addr, 0x10));
        cursor->addr = ALIGN(cursor->addr, 0x10);
        cursor->addr += sizeof(*sys);
        if (sys != NULL) {
            memset(sys, 0, sizeof(*sys));
            sys->capacity = count;
            sys->named_count = flags;
            sys->entries = reinterpret_cast<GAMEDEBRISENTRY_s *>(ALIGN(cursor->addr, 0x10));
            cursor->addr = ALIGN(cursor->addr, 0x10);
            cursor->addr += static_cast<usize>(count) * sizeof(*sys->entries);
            if (sys->entries != NULL) {
                memset(sys->entries, 0xff, static_cast<usize>(count) * sizeof(*sys->entries));

                i32 i;
                for (i = 0; i < sys->named_count; i++) {
                    NuStrCpy(sys->entries[i].name, names[i]);
                    sys->entries[i].effect = -1;
                    sys->entries[i].effect = LookupDebrisEffectPage(sys->entries[i].name, page);
                }
                for (i32 j = 1; i < sys->capacity && j < EDPP_MAX_TYPES; j++) {
                    if (debtab == NULL) {
                        break;
                    }
                    sys->entries[i].effect = -1;
                    if (debtab[j] != NULL) {
                        NuStrCpy(sys->entries[i].name, debtab[j]->name);
                        sys->entries[i].effect = LookupDebrisEffectPageOnly(sys->entries[i].name, page);
                        i++;
                    }
                }
                for (; i < sys->capacity; i++) {
                    sys->entries[i].effect = -1;
                }
                return sys;
            }
        }
        return NULL;
    }

    i32 AddGameDebris(APIDEBRISSYS_s *system, i32 type, NUVEC *position) {
        if (type >= 0 && type < system->capacity && system->entries[type].effect != -1) {
            i32 handle = -1;
            AddFiniteShotDebrisEffect(&handle, system->entries[type].effect, position, 1);
            return 1;
        }
        return 0;
    }

    i32 AddGameDebrisMomentum(APIDEBRISSYS_s *system, i32 type, NUVEC *position, NUVEC *emitter_momentum,
                              NUVEC *particle_momentum) {
        if (type >= 0 && type < system->capacity && system->entries[type].effect != -1) {
            i32 handle = -1;
            AddFiniteShotDebrisEffect2(&handle, system->entries[type].effect, position, emitter_momentum,
                                       particle_momentum, 1);
            return 1;
        }
        return 0;
    }

    i32 AddGameDebrisXYZ(APIDEBRISSYS_s *system, i32 type, f32 x, f32 y, f32 z) {
        if (type >= 0 && type < system->capacity && system->entries[type].effect != -1) {
            i32 handle = -1;
            NUVEC position = {x, y, z};
            AddFiniteShotDebrisEffect(&handle, system->entries[type].effect, &position, 1);
            return 1;
        }
        return 0;
    }

    i32 AddGameDebrisRot(APIDEBRISSYS_s *system, i32 type, NUVEC *position, i32 count, u16 z_rotation, u16 y_rotation) {
        if (type >= 0 && type < system->capacity && system->entries[type].effect != -1 && count > 0) {
            AddVariableShotDebrisEffect(system->entries[type].effect, position, count, z_rotation, y_rotation);
            return 1;
        }
        return 0;
    }

    i32 AddGameDebrisMtx(APIDEBRISSYS_s *system, i32 type, NUVEC *position, i32 count, NUMTX *matrix) {
        if (type >= 0 && type < system->capacity && system->entries[type].effect != -1 && count > 0) {
            NUMTX_ALIGNED16 orientation;
            NuMtxSetRotationX(&orientation, 0x4000);
            NuMtxMulR(&orientation, &orientation, matrix);
            AddVariableShotDebrisEffectMtx3(system->entries[type].effect, position, &nuvec_zero, count, &orientation,
                                            &numtx_identity);
            return 1;
        }
        return 0;
    }

    i32 AddGameDebrisMom(APIDEBRISSYS_s *system, i32 type, NUVEC *position, i32 count, NUVEC *momentum) {
        if (type >= 0 && type < system->capacity && system->entries[type].effect != -1 && count > 0) {
            NUVEC zero = {0.0f, 0.0f, 0.0f};
            if (momentum == NULL)
                momentum = &zero;
            AddVariableShotDebrisEffectMtx3(system->entries[type].effect, position, momentum, count, NULL, NULL);
            return 1;
        }
        return 0;
    }

    void SetAPIObjPlaySfxByIdFn(void (*play_sfx)(i32, NUVEC *)) {
        APIObjPlaySfxByIdFn = play_sfx;
    }

    void AddAnimEffects(CHARACTERMODEL_s *model, CHARACTERDATA *, ANIMPACKET_s *packet, NUMTX *locator_matrices,
                        GameObject_s *object, CHARACTER_EFFECT_s *effects, i32 paused, f32 frame_time,
                        void (*footprint_callback)(void *, GameObject_s *, i32, i32), APIDEBRISSYS_s *debris_sys) {
        if (paused != 0 || effects == NULL) {
            return;
        }

        i32 animation_id;
        f32 animation_time;
        if (packet->blending != 0 && packet->blend_animation_b >= 0 &&
            packet->blend_animation_b < apicharsys->model_id_capacity &&
            model->model_data_b[packet->blend_animation_b] != NULL) {
            animation_id = packet->blend_animation_b;
            animation_time = packet->blend_target_time;
        } else if (packet->blending != 0 && packet->blend_animation_a >= 0 &&
                   packet->blend_animation_a < apicharsys->model_id_capacity &&
                   model->model_data_b[packet->blend_animation_a] != NULL) {
            animation_id = packet->blend_animation_a;
            animation_time = packet->blend_source_time;
        } else if (packet->blending == 0 && packet->animation_index >= 0 &&
                   packet->animation_index < apicharsys->model_id_capacity &&
                   model->model_data_b[packet->animation_index] != NULL) {
            animation_id = packet->animation_index;
            animation_time = packet->current_time;
        } else {
            return;
        }

        for (; effects->character_id != -1; ++effects) {
            if (effects->character_id != model->model_id || effects->action_id != animation_id) {
                continue;
            }

            if (object != NULL) {
                if ((effects->flags & 0x400) != 0 && object->apiobj.field_0x27c == -1) {
                    continue;
                }
                if ((effects->flags & 0x10) != 0 && (object->apiobj.field_0x27d & 2) == 0) {
                    continue;
                }
                if ((effects->flags & 0x40) != 0 && object->apiobj.is_underwater != 0) {
                    continue;
                }
                if ((effects->flags & 0x20) != 0 && object->apiobj.is_underwater == 0) {
                    continue;
                }
                if ((effects->flags & 0x80) != 0 && object->apiobj.intersects_water == 0) {
                    continue;
                }
                if ((effects->flags & 0x03000000) != 0 &&
                    (object->apiobj.model_draw_result == 0 || (object->apiobj.field_0x27d & 2) == 0)) {
                    continue;
                }
            } else if ((effects->flags & 0x03000000) != 0) {
                continue;
            }

            i32 active = 0;
            if ((effects->flags & 0x4) != 0) {
                active = 1;
            } else if ((effects->flags & 0x8) != 0) {
                if (effects->frame_2 > 1.0f && effects->frame_1 > effects->frame_2) {
                    active = animation_time >= effects->frame_1 || animation_time <= effects->frame_2;
                } else {
                    active = animation_time >= effects->frame_1 &&
                             (effects->frame_2 <= 1.0f ||
                              (effects->frame_2 > effects->frame_1 && animation_time <= effects->frame_2));
                }
            } else {
                for (i32 event = 0; event <= 1; ++event) {
                    const f32 event_time = event == 0 ? effects->frame_1 : effects->frame_2;
                    if (event_time <= 1.0f) {
                        continue;
                    }
                    if ((packet->flags & ANIMPACKET_FLAG_PLAYING_REVERSED) == 0) {
                        if (animation_time > event_time) {
                            if (packet->previous_time <= event_time || ((packet->flags & ANIMPACKET_FLAG_LOOPED) != 0 &&
                                                                        packet->previous_time > animation_time)) {
                                active = 1;
                            }
                        } else if ((packet->flags & ANIMPACKET_FLAG_LOOPED) != 0 &&
                                   packet->previous_time > animation_time && packet->previous_time <= event_time) {
                            active = 1;
                        }
                    } else if (animation_time < event_time) {
                        if (packet->previous_time >= event_time) {
                            active = 1;
                        }
                    } else if ((packet->flags & ANIMPACKET_FLAG_LOOPED) != 0 &&
                               packet->previous_time > animation_time && packet->previous_time >= event_time) {
                        active = 1;
                    }
                }
            }
            if (active == 0) {
                continue;
            }

            NUVEC position = {0.0f, 0.0f, 0.0f};
            NUVEC locator_positions[16];
            i32 locator_indices[16];
            i32 locator_count = 0;
            i32 position_count = 0;
            for (i32 locator = 0; locator < 16; ++locator) {
                if ((effects->locators & (1 << locator)) == 0 || model->points_of_interest[locator] == NULL) {
                    continue;
                }
                locator_indices[locator_count] = locator;
                locator_positions[locator_count] = *NUMTX_GET_ROW_VEC(&locator_matrices[locator], 3);
                if (locator_count == 0 || (effects->flags & 0x800) != 0) {
                    NuVecAdd(&position, &position, NUMTX_GET_ROW_VEC(&locator_matrices[locator], 3));
                }
                ++locator_count;
            }
            if (locator_count > 0 && (effects->flags & 0x800) != 0) {
                NuVecScale(&position, &position, 1.0f / static_cast<f32>(locator_count));
            }
            position_count = locator_count;
            if ((effects->flags & 0x1000) != 0 && object != NULL) {
                position.x = object->apiobj.position.x;
                position.y = object->apiobj.collision_min.y;
                position.z = object->apiobj.position.z;
                position_count = 1;
                locator_count = 0;
            }
            if (object != NULL) {
                if (position_count == 0) {
                    locator_positions[0] = object->apiobj.position;
                    position = locator_positions[0];
                    position_count = 1;
                    if ((effects->flags & 0x2000) != 0 && object->apiobj.field_0x218 != 2000000.0f) {
                        position.y = object->apiobj.field_0x218;
                    } else if ((effects->flags & 0x4000) != 0 && object->apiobj.water_height != 2000000.0f) {
                        position.y = object->apiobj.water_height;
                    }
                } else if ((effects->flags & 0x2000) != 0 && object->apiobj.field_0x218 != 2000000.0f) {
                    for (i32 i = 0; i < locator_count; ++i) {
                        locator_positions[i].y = object->apiobj.field_0x218;
                    }
                } else if ((effects->flags & 0x4000) != 0 && object->apiobj.water_height != 2000000.0f) {
                    for (i32 i = 0; i < locator_count; ++i) {
                        locator_positions[i].y = object->apiobj.water_height;
                    }
                }
            }

            if (footprint_callback != NULL && locator_count == 0 && (effects->flags & 0x03000000) != 0) {
                footprint_callback(&object->apiobj.position, object, effects->flags & 0x01000000, 1);
            } else if (footprint_callback != NULL && position_count > 0 && (effects->flags & 0x03000000) != 0) {
                for (i32 i = 0; i < locator_count; ++i) {
                    const i32 locator = locator_indices[i];
                    if (object->apiobj.character_model->points_of_interest[locator] != NULL) {
                        footprint_callback(&locator_matrices[locator], object, effects->flags & 0x01000000, 0);
                    }
                }
            }

            if (object != NULL) {
                if (effects->bits_on != 0) {
                    object->apiobj.field_0x285 |= effects->bits_on;
                }
                if (effects->bits_off != 0) {
                    object->apiobj.field_0x285 &= ~effects->bits_off;
                }
            }
            if (position_count > 0 && object != NULL &&
                (((effects->flags & 0x40) != 0 && object->apiobj.is_underwater != 0) ||
                 ((effects->flags & 0x20) != 0 && object->apiobj.is_underwater == 0))) {
                continue;
            }

            do {
                if (object != NULL) {
                    if ((effects->flags & 0x100000) != 0) {
                        if (effects->minimum > object->apiobj.velocity_magnitude) {
                            break;
                        }
                    } else if ((effects->flags & 0x40000) != 0) {
                        if (effects->minimum > object->apiobj.horizontal_velocity_magnitude) {
                            break;
                        }
                    }
                    if ((effects->flags & 0x200000) != 0) {
                        if (object->apiobj.velocity_magnitude > effects->maximum) {
                            break;
                        }
                    } else if ((effects->flags & 0x80000) != 0) {
                        if (object->apiobj.horizontal_velocity_magnitude > effects->maximum) {
                            break;
                        }
                    }
                }

                if (position_count > 0 && effects->debris_id != -1 && effects->debris_id < debris_sys->capacity) {
                    if ((effects->flags & 1) != 0) {
                        i32 count = effects->particle_rate > 0.0f
                                        ? ((effects->flags & 0x800000) != 0
                                               ? ParticlesPerSecond(effects->particle_rate, frame_time)
                                               : ParticlesPerFrame(effects->particle_rate, frame_time))
                                        : effects->particle_count;
                        if (count < 0) {
                            count = 1;
                        }
                        if (count > 0 && (effects->random == 0 || (NuRandInt() >> 16) <= (effects->random << 8))) {
                            if ((effects->flags & 0x1000) != 0) {
                                if ((effects->flags & 0x400000) != 0 && object != NULL) {
                                    AddGameDebrisMom(debris_sys, effects->debris_id, &position, count,
                                                     &object->apiobj.velocity);
                                } else {
                                    AddGameDebrisRot(debris_sys, effects->debris_id, &position, count, 0, 0);
                                }
                            } else {
                                u16 y_rotation = 0;
                                u16 z_rotation = y_rotation;
                                if ((effects->flags & 0x800) != 0) {
                                    if ((effects->flags & 0x400000) != 0 && object != NULL) {
                                        AddGameDebrisMom(debris_sys, effects->debris_id, &position, count,
                                                         &object->apiobj.velocity);
                                    } else {
                                        AddGameDebrisRot(debris_sys, effects->debris_id, &position, count, z_rotation,
                                                         y_rotation);
                                    }
                                } else if (locator_count != 0) {
                                    for (i32 i = 0; i < locator_count; ++i) {
                                        if ((effects->flags & 0x400000) != 0 && object != NULL) {
                                            NUVEC velocity;
                                            NuVecSub(&velocity, &object->apiobj.position,
                                                     &object->apiobj.start_position);
                                            NuVecScale(&velocity, &velocity, 1.0f / frame_time);
                                            AddGameDebrisMom(debris_sys, effects->debris_id, &locator_positions[i],
                                                             count, &velocity);
                                        } else if ((effects->flags & 0x4000000) != 0) {
                                            AddGameDebrisMtx(debris_sys, effects->debris_id, &locator_positions[i],
                                                             count, &locator_matrices[i]);
                                        } else {
                                            AddGameDebrisRot(debris_sys, effects->debris_id, &locator_positions[i],
                                                             count, z_rotation, y_rotation);
                                        }
                                    }
                                } else {
                                    if ((effects->flags & 0x400000) != 0 && object != NULL) {
                                        AddGameDebrisMom(debris_sys, effects->debris_id, &position, count,
                                                         &object->apiobj.velocity);
                                    } else {
                                        AddGameDebrisRot(debris_sys, effects->debris_id, &position, count, z_rotation,
                                                         y_rotation);
                                    }
                                }
                            }
                        }
                    } else if ((effects->flags & 0x1000) != 0 || (effects->flags & 0x800) != 0) {
                        AddGameDebris(debris_sys, effects->debris_id, &position);
                    } else if (locator_count != 0) {
                        for (i32 i = 0; i < position_count; ++i) {
                            AddGameDebris(debris_sys, effects->debris_id, &locator_positions[i]);
                        }
                    } else {
                        AddGameDebris(debris_sys, effects->debris_id, &position);
                    }
                }
            } while (0);
            if (APIObjPlaySfxByIdFn != NULL && object != NULL && effects->sound_id != -1) {
                APIObjPlaySfxByIdFn(effects->sound_id,
                                    (effects->flags & 0x8000) != 0 ? &object->apiobj.collision_position : NULL);
            }
        }
    }

    void NuHGobjRestrictEvaluation(nuhgobj_s *object) {
        Temphgobj = object;
        if (object != NULL) {
            TempNumJoints = object->joint_count;
            object->joint_count = 1;
        }
    }

    void NuHGobjRestoreEvaluation(void) {
        if (Temphgobj != NULL) {
            Temphgobj->joint_count = TempNumJoints;
            Temphgobj = NULL;
        }
    }

    void APITransparentInit(void) {
        APITrans_Mtl[0] = NuMtlCreate3D(1);
        APITrans_Mtl[0]->attribs.unknown_1_1_2 = 1;
        APITrans_Mtl[0]->attribs.unknown_1_4_8 = 1;
        APITrans_Mtl[0]->diffuse_color.r = 0.0f;
        APITrans_Mtl[0]->diffuse_color.g = 0.0f;
        APITrans_Mtl[0]->diffuse_color.b = 0.0f;
        APITrans_Mtl[0]->opacity = 1.0f;
        APITrans_Mtl[0]->attribs.alpha_mode = 2;
        APITrans_Mtl[0]->attribs.z_mode = 0;
        APITrans_Mtl[0]->attribs.alpha_ref = 0;
        APITrans_Mtl[0]->sort_pri = 0x24;
        NuMtlUpdate(APITrans_Mtl[0]);

        APITrans_Mtl[1] = NuMtlCreate3D(1);
        APITrans_Mtl[1]->attribs.unknown_1_1_2 = 1;
        APITrans_Mtl[1]->attribs.unknown_1_4_8 = 1;
        APITrans_Mtl[1]->diffuse_color.r = 0.0f;
        APITrans_Mtl[1]->diffuse_color.g = 0.0f;
        APITrans_Mtl[1]->diffuse_color.b = 0.0f;
        APITrans_Mtl[1]->opacity = 1.0f;
        APITrans_Mtl[1]->attribs.alpha_mode = 2;
        APITrans_Mtl[1]->attribs.z_mode = 1;
        APITrans_Mtl[1]->attribs.alpha_ref = 0;
        APITrans_Mtl[1]->sort_pri = 0x24;
        NuMtlUpdate(APITrans_Mtl[1]);
    }

    void APITransparentCharDraw(nuhgobj_s *object, NUMTX *world_matrix, i32 render_count, i16 *render_indices,
                                NUMTX *joint_matrices, void **dwa, i32 render_flags) {
        i32 layer_six = 0;
        i32 layer_zero = 0;
        if (notransparentchardraw == 1 || APITrans_Mtl[0] == NULL) {
            return;
        }

        if (render_count > 1) {
            for (i32 i = 0; i < render_count; ++i) {
                if (render_indices[i] == 6) {
                    render_indices[i] = render_indices[render_count - 1];
                    layer_six = render_count - 1;
                    --render_count;
                }
                if (render_indices[i] == 0) {
                    render_indices[i] = render_indices[render_count - 1];
                    layer_zero = render_count - 1;
                    --render_count;
                }
            }
        }

        u8 previous_alpha_mode = object->data_0x198[8];
        object->data_0x198[8] = 1;
        NuSpecialConstAlpha(1, 0.0f);
        NuHGobjRndrMtxDwa(object, world_matrix, render_count, render_indices, joint_matrices, dwa, render_flags);
        NuSpecialConstAlpha(0, 0.0f);
        object->data_0x198[8] = previous_alpha_mode;
        if (layer_six != 0) {
            render_indices[layer_six] = 6;
        }
        if (layer_zero != 0) {
            render_indices[layer_zero] = 0;
        }
    }

    // Original @0x3d0563. Hierarchy evaluation, DWA, locator storage, character
    // surface effects, transparency and reflection.
    i32 APIDrawCharacterModel(CHARACTERMODEL_s *model, CHARACTERDATA *character_data, ANIMPACKET_s *animation,
                              NUMTX *matrix, NUMTX *, NUMTX *reflection_matrix, NUVEC *locator_positions,
                              NUMTX *locator_matrices, GameObject_s *object, u32 flags, NUJOINTANIM_s *joint_overrides,
                              i32 joint_override_count, i32 paused, f32 frame_time, NUMTX *output_matrices,
                              void (*footprint_callback)(void *, GameObject_s *, i32, i32),
                              APIDEBRISSYS_s *debris_sys) {
        drawcharactermodel_locatorsupdated = 0;
        if (model == NULL)
            return 0;
        i32 result = 0;
        i32 evaluate_only = 0;
        if (animation != NULL) {
            if (animation->frame != 0xffff) {
                if (animation->field_0x3a >= 0 && animation->field_0x3a < apicharsys->model_id_capacity &&
                    model->model_data_b[animation->field_0x3a] != NULL && static_cast<i16>(animation->frame) >= 0 &&
                    static_cast<i16>(animation->frame) < apicharsys->model_id_capacity &&
                    model->model_data_b[static_cast<i16>(animation->frame)] != NULL) {
                    if ((static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->field_0x3a])->flags & 0x220) !=
                            0 ||
                        (static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->frame])->flags & 0x220) != 0)
                        evaluate_only = 1;
                }
            } else if (animation->blending != 0 && animation->blend_animation_a >= 0 &&
                       animation->blend_animation_a < apicharsys->model_id_capacity &&
                       model->model_data_b[animation->blend_animation_a] != NULL && animation->blend_animation_b >= 0 &&
                       animation->blend_animation_b < apicharsys->model_id_capacity &&
                       model->model_data_b[animation->blend_animation_b] != NULL) {
                if ((static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->blend_animation_a])->flags &
                     0x220) != 0 ||
                    (static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->blend_animation_b])->flags &
                     0x220) != 0)
                    evaluate_only = 1;
            } else if (animation->blending != 0 && animation->blend_animation_b >= 0 &&
                       animation->blend_animation_b < apicharsys->model_id_capacity &&
                       model->model_data_b[animation->blend_animation_b] != NULL) {
                if ((static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->blend_animation_b])->flags &
                     0x220) != 0)
                    evaluate_only = 1;
            } else if (animation->blending != 0 && animation->blend_animation_a >= 0 &&
                       animation->blend_animation_a < apicharsys->model_id_capacity &&
                       model->model_data_b[animation->blend_animation_a] != NULL) {
                if ((static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->blend_animation_a])->flags &
                     0x220) != 0)
                    evaluate_only = 1;
            } else if (animation->blending == 0 && animation->animation_index >= 0 &&
                       animation->animation_index < apicharsys->model_id_capacity &&
                       model->model_data_b[animation->animation_index] != NULL) {
                if ((static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->animation_index])->flags & 0x220) !=
                    0)
                    evaluate_only = 1;
            }
        }
        if (model->hierarchy == NULL || matrix == NULL)
            goto cleanup;

        {
            NUVEC bounds_min = model->hierarchy->bounds_min;
            NUVEC bounds_max = model->hierarchy->bounds_max;
            if ((object == NULL || (object->apiobj.field_0x1f4 & 0x200) == 0) &&
                NuCameraClipTestExtents(&bounds_min, &bounds_max, matrix, character_farclip, 0) == 0) {
                if (evaluate_only == 0)
                    goto cleanup;
                evaluate_only = 1;
            } else {
                evaluate_only = 0;
            }
            if (evaluate_only != 0)
                NuHGobjRestrictEvaluation(model->hierarchy);

            i16 render_indices[32];
            const i32 render_count = MakeLayerList != NULL ? MakeLayerList(model, render_indices, flags) : 0;
            if (render_count <= 0)
                goto cleanup;

            void **dwa = NULL;
            if (evaluate_only == 0 && animation != NULL && drawcharactermodel_nobsa == 0 &&
                drawcharactermodel_restpose == 0) {
                if (animation->blending != 0 && animation->blend_animation_a >= 0 &&
                    animation->blend_animation_a < apicharsys->model_id_capacity &&
                    model->model_data_c[animation->blend_animation_a] != NULL && animation->blend_animation_b >= 0 &&
                    animation->blend_animation_b < apicharsys->model_id_capacity &&
                    model->model_data_c[animation->blend_animation_b] != NULL) {
                    dwa = NuHGobjEvalDwaBlend2(
                        render_count, render_indices,
                        static_cast<nuanimdata2_s *>(model->model_data_c[animation->blend_animation_a]),
                        animation->time,
                        static_cast<nuanimdata2_s *>(model->model_data_c[animation->blend_animation_b]),
                        animation->blend_target_time, animation->blend_elapsed / animation->blend_duration);
                } else if (animation->blending != 0 && animation->blend_animation_b >= 0 &&
                           animation->blend_animation_b < apicharsys->model_id_capacity &&
                           model->model_data_c[animation->blend_animation_b] != NULL) {
                    dwa =
                        NuHGobjEvalDwa2(render_count, render_indices,
                                        static_cast<nuanimdata2_s *>(model->model_data_c[animation->blend_animation_b]),
                                        animation->blend_target_time);
                } else if (animation->blending != 0 && animation->blend_animation_a >= 0 &&
                           animation->blend_animation_a < apicharsys->model_id_capacity &&
                           model->model_data_c[animation->blend_animation_a] != NULL) {
                    dwa =
                        NuHGobjEvalDwa2(render_count, render_indices,
                                        static_cast<nuanimdata2_s *>(model->model_data_c[animation->blend_animation_a]),
                                        animation->time);
                } else if (animation->blending == 0 && animation->animation_index >= 0 &&
                           animation->animation_index < apicharsys->model_id_capacity &&
                           model->model_data_c[animation->animation_index] != NULL) {
                    dwa = NuHGobjEvalDwa2(render_count, render_indices,
                                          static_cast<nuanimdata2_s *>(model->model_data_c[animation->animation_index]),
                                          animation->current_time);
                }
            }

            bool evaluated = false;
            if (animation != NULL && drawcharactermodel_noani == 0 && drawcharactermodel_restpose == 0) {
                if (animation->frame != 0xffff) {
                    if (animation->field_0x3a >= 0 && animation->field_0x3a < apicharsys->model_id_capacity &&
                        model->model_data_b[animation->field_0x3a] != NULL && static_cast<i16>(animation->frame) >= 0 &&
                        static_cast<i16>(animation->frame) < apicharsys->model_id_capacity &&
                        model->model_data_b[static_cast<i16>(animation->frame)] != NULL) {
                        NuHGobjEvalAnimBlend2(
                            model->hierarchy,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->field_0x3a]),
                            animation->time, static_cast<ani3_animheader_s *>(model->model_data_b[animation->frame]),
                            animation->time, animation->field_0x44, joint_override_count, joint_overrides,
                            output_matrices);
                        evaluated = true;
                    }
                } else if (animation->blending != 0 && animation->blend_animation_a >= 0 &&
                           animation->blend_animation_a < apicharsys->model_id_capacity &&
                           model->model_data_b[animation->blend_animation_a] != NULL &&
                           animation->blend_animation_b >= 0 &&
                           animation->blend_animation_b < apicharsys->model_id_capacity &&
                           model->model_data_b[animation->blend_animation_b] != NULL) {
                    if ((static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->blend_animation_a])->flags &
                         0x20) != 0 ||
                        (static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->blend_animation_b])->flags &
                         0x20) != 0) {
                        NuHGobjEvalAnimBlend2Root(
                            model->hierarchy,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->blend_animation_a]),
                            animation->time,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->blend_animation_b]),
                            animation->blend_target_time, animation->blend_elapsed / animation->blend_duration,
                            joint_override_count, joint_overrides, output_matrices, BlendRootFn, object);
                    } else {
                        NuHGobjEvalAnimBlend2(
                            model->hierarchy,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->blend_animation_a]),
                            animation->time,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->blend_animation_b]),
                            animation->blend_target_time, animation->blend_elapsed / animation->blend_duration,
                            joint_override_count, joint_overrides, output_matrices);
                    }
                    evaluated = true;
                } else if (animation->blending != 0 && animation->blend_animation_b >= 0 &&
                           animation->blend_animation_b < apicharsys->model_id_capacity &&
                           model->model_data_b[animation->blend_animation_b] != NULL) {
                    CHARACTERANIM_s *selected =
                        static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->blend_animation_b]);
                    if ((selected->flags & 0x20) != 0) {
                        NuHGobjEvalAnim2Root(
                            model->hierarchy,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->blend_animation_b]),
                            animation->blend_target_time, joint_override_count, joint_overrides, output_matrices,
                            (selected->flags & 0x200) != 0 ? RootFnY : RootFn, object);
                    } else {
                        NuHGobjEvalAnim2(
                            model->hierarchy,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->blend_animation_b]),
                            animation->blend_target_time, joint_override_count, joint_overrides, output_matrices);
                    }
                    evaluated = true;
                } else if (animation->blending != 0 && animation->blend_animation_a >= 0 &&
                           animation->blend_animation_a < apicharsys->model_id_capacity &&
                           model->model_data_b[animation->blend_animation_a] != NULL) {
                    CHARACTERANIM_s *selected =
                        static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->blend_animation_a]);
                    if ((selected->flags & 0x20) != 0) {
                        NuHGobjEvalAnim2Root(
                            model->hierarchy,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->blend_animation_a]),
                            animation->time, joint_override_count, joint_overrides, output_matrices,
                            (selected->flags & 0x200) != 0 ? RootFnY : RootFn, object);
                    } else {
                        NuHGobjEvalAnim2(
                            model->hierarchy,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->blend_animation_a]),
                            animation->time, joint_override_count, joint_overrides, output_matrices);
                    }
                    evaluated = true;
                } else if (animation->blending == 0 && animation->animation_index >= 0 &&
                           animation->animation_index < apicharsys->model_id_capacity &&
                           model->model_data_b[animation->animation_index] != NULL) {
                    CHARACTERANIM_s *selected =
                        static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->animation_index]);
                    if ((selected->flags & 0x20) != 0) {
                        NuHGobjEvalAnim2Root(
                            model->hierarchy,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->animation_index]),
                            animation->current_time, joint_override_count, joint_overrides, output_matrices,
                            (selected->flags & 0x200) != 0 ? RootFnY : RootFn, object);
                    } else {
                        NuHGobjEvalAnim2(
                            model->hierarchy,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->animation_index]),
                            animation->current_time, joint_override_count, joint_overrides, output_matrices);
                    }
                    evaluated = true;
                }
            }
            if (!evaluated) {
                NuHGobjEval(model->hierarchy, joint_override_count,
                            reinterpret_cast<nuhgobjjointoverride_s *>(joint_overrides), output_matrices);
            }
            StoreLocatorCoordinates(model, matrix, output_matrices, locator_positions, locator_matrices);
            drawcharactermodel_locatorsupdated = 1;

            if (locator_matrices != NULL && paused == 0 && character_data != NULL && character_data->effects != NULL) {
                AddAnimEffects(model, character_data, animation, locator_matrices, object,
                               apicharsys->char_data[model->model_id].effects, paused, frame_time, footprint_callback,
                               debris_sys);
            }

            const i32 render_flags = object == NULL || (object->apiobj.field_0x1f4 & 0x200) == 0;
            if (object != NULL && apicharsys != NULL && apicharsys->set_creature_lights != NULL) {
                apicharsys->set_creature_lights(&object->apiobj);
            }

            if (!evaluate_only) {
                if (object != NULL && (object->apiobj.field_0x1f4 & 0x20000) != 0) {
                    if (APIObjResetShadowMapRenderingFn != NULL)
                        APIObjResetShadowMapRenderingFn();
                    APITransparentCharDraw(model->hierarchy, matrix, render_count, render_indices, output_matrices, dwa,
                                           render_flags);
                    if (APIObjEnableShadowMapRenderingFn != NULL)
                        APIObjEnableShadowMapRenderingFn();
                }
                result = NuHGobjRndrMtxDwa(model->hierarchy, matrix, render_count, render_indices, output_matrices, dwa,
                                           render_flags);
                if (object != NULL && object->apiobj.surface_effect_count != 0) {
                    if (object->apiobj.surface_effect_count > 16)
                        object->apiobj.surface_effect_count = 16;
                    NUVEC points[16];
                    NuHGobjRndrRandShadowSurfacePoints(model->hierarchy, matrix, output_matrices,
                                                       object->apiobj.surface_effect_count, points, 0);
                    for (i32 i = 0; i < object->apiobj.surface_effect_count; ++i) {
                        AddVariableShotDebrisEffect(object->apiobj.surface_effect_id, &points[i], 1, 0, 0);
                    }
                    object->apiobj.surface_effect_count = 0;
                }
                if (reflection_matrix != NULL) {
                    model->hierarchy->data_0x198[8] = 1;
                    if (object == NULL || (object->apiobj.field_0x1f4 & 0x1000) == 0)
                        nurndr_force_lod = 1;
                    NuRndrStartReflectionRender(result);
                    NuHGobjRndrMtxDwa(model->hierarchy, reflection_matrix, render_count, render_indices,
                                      output_matrices, dwa, render_flags);
                    NuRndrEndReflectionRender();
                    if (object == NULL || (object->apiobj.field_0x1f4 & 0x1000) == 0)
                        nurndr_force_lod = 0;
                }
            }

            if (evaluate_only != 0)
                NuHGobjRestoreEvaluation();
        }
    cleanup:
        drawcharactermodel_nobsa = 0;
        drawcharactermodel_noani = 0;
        drawcharactermodel_restpose = 0;
        if (animation != NULL && drawcharactermodel_keepmergeaction == 0) {
            animation->frame = 0xffff;
        }
        drawcharactermodel_keepmergeaction = 0;
        return result;
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

    CHARACTERDATA *ConfigureCharacterList(char *file, VARIPTR *bufferStart, VARIPTR *bufferEnd, i32 count,
                                          i32 *countDest, i32 count2, GAMECHARACTERDATA **dataList) {
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

    void WindShear(NUMTX *output, NUMTX *input, i32 wind_scale, i32 wind_speed) {
        u32 seed = static_cast<u32>(wind_speed);
        const f32 speed = (static_cast<f32>(wind_speed) / 65535.0f) * global_windspeed;
        const f32 amplitude = (static_cast<f32>(wind_scale) / 65535.0f) * global_windscale;
        const f32 random = NuRandFloatSeeded(&seed);
        const u32 frame = static_cast<u32>(NuRndrGlobalFrameCount());
        const f32 phase = random * 3.142f + random * 3.142f + static_cast<f32>(frame) * speed;
        const f32 shear_x =
            (NU_SIN_LUT(4.2f * phase * 10430.3779296875f) * 0.25f +
             (NU_SIN_LUT(2.1f * phase * 10430.3779296875f) * 0.5f + NU_SIN_LUT(phase * 10430.3779296875f))) *
            amplitude;
        const f32 shear_z =
            (0.25f * NU_COS_LUT(4.4f * phase * 10430.3779296875f) +
             (0.5f * NU_COS_LUT(2.3f * phase * 10430.3779296875f) + NU_COS_LUT(phase * 1.1f * 10430.3779296875f))) *
            amplitude;
        *output = *input;
        output->m10 = input->m00 * shear_x + input->m10 + input->m20 * shear_z;
        output->m11 = input->m01 * shear_x + input->m11 + input->m21 * shear_z;
        output->m12 = input->m02 * shear_x + input->m12 + input->m22 * shear_z;
        output->m13 = shear_x * input->m03 + input->m13 + shear_z * input->m23;
    }

} // extern "C"
