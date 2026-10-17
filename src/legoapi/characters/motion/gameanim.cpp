#include "decomp.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "gameapi/edtools/edfile.h"
#include "globals.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/world.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numusic/sfx.h"

#include <float.h>
#include <string.h>

void EvalAnim(nuhspecial_s *special, f32 frame, numtx_s *matrix, i32 include_instance_translation);
i32 GetDefaultIdle(GameObject_s *object);
// Retail uses translation-unit-local helpers with compiler-inferred private
// calling conventions; keep their shared animation-mode call sites intact.
f32 PodSprint_InStartCountdown(WORLDINFO_s *world);
i32 PodRace_InStartCountdown(WORLDINFO_s *world);
void SetPodMergeAnims(ANIMPACKET_s *packet, i32 index);
extern "C" i16 id_ANAKINSPODGREEN;
i32 MatrixReflection(NUMTX *matrix, i32 axis, f32 plane, f32 height, NUMTX *result);

extern i16 id_BODYGUARD;
extern i16 id_IMPERIALGUARD;
extern i16 id_YODA;
extern i16 id_YODAGHOST;
extern i16 id_GONKDROID;

#include "legoapi/characters/motion/gameanim_types.h"

void Animate_ATAT(GameObject_s *object);

void Animate_POD(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    packet.requested_animation = 1;

    {
        if (__builtin_expect(!object->apiobj.player_controlled, 0)) {
            goto normal_animation;
        }
        const i8 index = object->apiobj.field_0x27c;
        if (index == -1 || (object->id != id_ANAKINSPOD && object->id != id_ANAKINSPODGREEN)) {
            goto normal_animation;
        }
        f32 delta = FRAMETIME * 30.0f;
        CHARACTERMODEL_s *model = object->apiobj.character_model;
        if (model->model_data_b[1] != NULL) {
            CHARACTERANIM_s *info = static_cast<CHARACTERANIM_s *>(model->model_data_a[1]);
            delta *= info->playback_rate / 30.0f;
        }
        pod_animtime[index] += delta;
        if (pod_animtime[index] >= podanimendframe) {
            pod_animtime[index] -= podanimendframe - 1.0f;
        }
        SetPodMergeAnims(&packet, index);
        return;
    }

normal_animation:
    if (PodSprint_InStartCountdown(WORLD) > 0.0f || PodRace_InStartCountdown(WORLD) != 0) {
        packet.requested_animation = 1;
    } else if (object->camera_shake_strength > 0.0f) {
        packet.requested_animation = 0x17;
    } else if (__builtin_expect(!object->apiobj.player_controlled, 0)) {
        packet.requested_animation = 3;
    } else if (PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA) {
        if (object->previous_block_animation != -1) {
            packet.requested_animation = object->previous_block_animation;
        }
    } else {
        packet.requested_animation = 0;
    }
    if (object->apiobj.character_model->model_data_b[packet.requested_animation] == NULL) {
        packet.requested_animation = 1;
    }
}

void Animate_JEDI(GameObject_s *object);

void AnimatePlayer(GameObject_s *object);

void Animate_BEAST(GameObject_s *object);

void Animate_BARMAN(GameObject_s *object);

void Animate_CANNON(GameObject_s *object);

void Animate_WALKER(GameObject_s *object);

void Animate_CRITTER(GameObject_s *object);

void Animate_WEIRDO(GameObject_s *object);

void Animate_DEFAULT(GameObject_s *object);

void Animate_VEHICLE(GameObject_s *object);

void Animate_PROTOCOL(GameObject_s *object);

void Animate_DROIDEKA(GameObject_s *object);

// Original @0x4a9900.
void GameAnimSet_Draw(GAMEANIMSET_s &set) {
    for (GAMEANIMOBJ_s *object = set.objects; object != NULL; object = object->next) {
        if ((object->flags & 2) == 0 && NuSpecialGetVisibilityFn(&object->special) != 0) {
            NuSpecialDrawAt(&object->special, NuSpecialGetDrawMtx(&object->special));
        }
    }
}

i32 GameAnimSet_Play(GAMEANIMSET_s *set, float speed, i32 evaluate_state) {
    if (set == NULL) {
        return 1;
    }

    if (evaluate_state != 0) {
        GameAnimSet_EvaluateState(set);
    }
    set->flags = static_cast<GAMEANIMSET_FLAGS>(set->flags & ~GAMEANIMSET_FLAG_STOP_REQUESTED);

    if (speed >= 0.0f) {
        if (set->state == GAMEANIMSET_STATE_AT_END) {
            return 1;
        }
    } else if (speed < 0.0f) {
        if (set->state == GAMEANIMSET_STATE_AT_START) {
            return 1;
        }
    }

    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        nuinstanim_s *animation = object->instance_animation;
        if (animation == NULL) {
            continue;
        }

        animation->playing = 1;
        animation->waiting = 0;

        f32 direction;
        if (object->end_frame < object->start_frame) {
            direction = -1.0f;
        } else {
            direction = 1.0f;
        }
        f32 current_frame = animation->ltime * direction;
        if (object->start_frame * direction > current_frame) {
            animation->ltime = object->start_frame;
        } else if (current_frame > object->end_frame * direction) {
            animation->ltime = object->end_frame;
        }

        if (animation->fparam1 != 0.0f) {
            animation->tfactor = animation->fparam1 * speed * direction;
        } else {
            animation->tfactor = direction * speed;
        }
    }

    if ((set->flags & GAMEANIMSET_FLAG_IN_SYSTEM_LIST) == 0) {
        GameAnimSet_AddToSystemList(set);
    }
    return 1;
}

i32 GameAnimSet_Stop(GAMEANIMSET_s *set) {
    if (set == NULL) {
        return 1;
    }

    GAMEANIMSET_STATE state = static_cast<GAMEANIMSET_STATE>(set->state & ~GAMEANIMSET_STATE_AT_END);
    if (state != GAMEANIMSET_STATE_ACTIVE_FORWARD) {
        return 1;
    }

    set->flags = static_cast<GAMEANIMSET_FLAGS>(set->flags | GAMEANIMSET_FLAG_STOP_REQUESTED);
    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        if (object->instance_animation != NULL) {
            object->instance_animation->playing = 0;
        }
    }
    return 1;
}

void Animate_ASTROMECH(GameObject_s *object);

void Animate_GEONOSIAN(GameObject_s *object);

i32 GameAnimSet_Reset(GAMEANIMSET_s *set) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            nuinstanim_s *animation = object->instance_animation;
            if (animation != NULL) {
                animation->playing = 0;
                animation->waiting = 0;
                animation->ltime = object->start_frame;
                animation->tfactor = 1.0f;
            }
            GameAnimSet_RemoveFromSystemList(set);
        }
    }
    return 1;
}

void Animate_HOVERDROID(GameObject_s *object);

GAMEANIMSET_s *GameAnimSet_Create(variptr_u *buf, variptr_u *buf_end, GAMEANIMOBJPOOL_s *object_pool,
                                  GAMEANIMSYS_s *system) {
    GAMEANIMSET_s *set = NULL;
    if (object_pool != NULL) {
        set = static_cast<GAMEANIMSET_s *>(GameBufferAlloc(buf, buf_end, sizeof(GAMEANIMSET_s)));
        if (set != NULL) {
            set->object_pool = object_pool;
            set->system = system;
            i32 index = system->set_count;
            if (index < gameanimsysprogress.entry_size) {
                system->sets[index] = set;
                system->set_count = index + 1;
            }
        }
    }
    return set;
}

void Animate_BATTLEDROID(GameObject_s *object);

void Animate_SPEEDERBIKE(GameObject_s *object);

i32 GameAnimSet_Playing(GAMEANIMSET_s *set) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            if (object->instance_animation != NULL && object->instance_animation->playing == 0) {
                return 0;
            }
        }
    }
    return 1;
}

void GameAnimSet_EvalAnim(GAMEANIMSET_s *set) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            EvalAnim2(&object->special, object->instance_animation->ltime);
        }
    }
}

GAMEANIMOBJ_s *GameAnimSet_AddObject(GAMEANIMSET_s *set, nuhspecial_s *special, float start_frame, float end_frame,
                                     i32 append) {
    if (set == NULL || set->object_pool == NULL || set->object_pool->free_objects == NULL || special == NULL ||
        NuSpecialExistsFn(special) == 0) {
        return NULL;
    }

    GAMEANIMOBJPOOL_s *pool = set->object_pool;
    ++set->object_count;
    GAMEANIMOBJ_s *object = pool->free_objects;
    ++pool->active_count;
    pool->free_objects = object->next;

    if (append != 0) {
        object->next = NULL;
        if (set->objects == NULL) {
            set->objects = object;
        } else {
            GAMEANIMOBJ_s *tail = set->objects;
            while (tail->next != NULL) {
                tail = tail->next;
            }
            tail->next = object;
        }
    } else {
        object->next = set->objects;
        set->objects = object;
    }

    const i32 object_index = object - pool->objects;
    object->object_data = static_cast<u8 *>(pool->object_data) + pool->object_data_size * object_index;
    object->special = *special;
    object->instance_animation = NuSpecialGetInstAnim(&object->special);
    if (object->instance_animation == NULL) {
        return object;
    }

    object->animation = object->special.scene->instance_animation_data[object->instance_animation->anim_ix];
    const f32 last_frame = NuSpecialGetAnimEndFrame(&object->special);
    if (last_frame > 0.0f) {
        if (last_frame < end_frame) {
            object->end_frame = last_frame;
        } else {
            object->end_frame = end_frame;
            if (object->end_frame < 1.0f) {
                object->end_frame = 1.0f;
            }
        }
        if (start_frame > last_frame) {
            object->start_frame = last_frame;
        } else {
            object->start_frame = start_frame;
            if (object->start_frame < 1.0f) {
                object->start_frame = 1.0f;
            }
        }
        ++set->animated_object_count;
    } else {
        NuSpecialGetName(&object->special);
    }
    return object;
}

i32 GameAnimSet_JumpToEnd(GAMEANIMSET_s *set) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            if (object->instance_animation != NULL) {
                object->instance_animation->playing = 0;
                object->instance_animation->ltime = object->end_frame;
            }
        }
    }
    return 1;
}

void GameAnimSet_SetOffset(GAMEANIMSET_s *set, NUVEC *offset) {
    if (set == NULL) {
        return;
    }

    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        if ((object->flags & 4) != 0) {
            continue;
        }
        NUMTX *source = NuSpecialGetMtx(&object->special);
        if (source == NULL) {
            continue;
        }

        NUMTX matrix = *source;
        matrix.m30 += offset->x;
        matrix.m31 += offset->y;
        matrix.m32 += offset->z;
        NuSpecialSetDrawMtx(&object->special, &matrix);
    }
}

f32 GameAnimSet_GetAnimPos(GAMEANIMOBJ_s *object) {
    if (object == NULL || object->instance_animation == NULL || object->animation == NULL) {
        return 0.0f;
    }

    if (object->start_frame == object->end_frame) {
        return 1.0f;
    }

    f32 position =
        (object->instance_animation->ltime - object->start_frame) / (object->end_frame - object->start_frame);
    if (position < 0.0f) {
        return 0.0f;
    }
    if (position > 1.0f) {
        position = 1.0f;
    }
    return position;
}

void GameAnimSet_SetAnimPos(GAMEANIMOBJ_s *object, float position) {
    if (object == NULL || object->instance_animation == NULL || object->animation == NULL) {
        return;
    }

    if (position < 0.0f) {
        position = 0.0f;
    }
    if (position > 1.0f) {
        position = 1.0f;
    }
    object->instance_animation->ltime = (object->end_frame - object->start_frame) * position + object->start_frame;
}

void GameAnimSet_SetTFactor(GAMEANIMSET_s *set, float factor) {
    if (set == NULL) {
        return;
    }

    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        nuinstanim_s *animation = object->instance_animation;
        if (animation != NULL) {
            f32 direction = object->end_frame < object->start_frame ? -1.0f : 1.0f;
            if (animation->fparam1 != 0.0f) {
                animation->tfactor = animation->fparam1 * factor * direction;
            } else {
                animation->tfactor = direction * factor;
            }
        }
    }
}

void Animate_REPUBLICGUNSHIP(GameObject_s *object);

i32 GameAnimSet_JumpToStart(GAMEANIMSET_s *set) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            if (object->instance_animation != NULL) {
                object->instance_animation->playing = 0;
                object->instance_animation->ltime = object->start_frame;
            }
        }
    }
    return 1;
}

void Animate_SUPERBATTLEDROID(GameObject_s *object);

void GameAnimSet_RemoveObject(GAMEANIMSET_s *set, GAMEANIMOBJ_s *object) {
    if (object == NULL || set == NULL)
        return;
    if (set->objects == object) {
        set->objects = object->next;
    } else {
        GAMEANIMOBJ_s *previous = set->objects;
        while (previous != NULL && previous->next != object) {
            previous = previous->next;
        }
        if (previous != NULL)
            previous->next = object->next;
    }
    object->next = NULL;
    --set->object_count;
    GAMEANIMOBJPOOL_s *pool = set->object_pool;
    --pool->active_count;
    object->next = pool->free_objects;
    pool->free_objects = object;
}

void GameAnimSet_ScaleFParam1(GAMEANIMSET_s *set, float scale) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            if (object->instance_animation != NULL) {
                object->instance_animation->fparam1 *= scale;
            }
        }
    }
}

i32 GameAnimSet_SetRepeating(GAMEANIMSET_s *set, i32 repeating) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            if (object->instance_animation != NULL) {
                object->instance_animation->repeating = repeating & 1;
            }
        }
    }
    return 1;
}

void GameAnimSet_EvaluateState(GAMEANIMSET_s *set) {
    if ((set->flags & GAMEANIMSET_FLAG_IN_SYSTEM_LIST) != 0) {
        return;
    }

    GAMEANIMOBJ_s *object = set->objects;
    i32 all_at_end = 1;
    i32 all_at_start = 1;

    while (object != NULL) {
        nuinstanim_s *animation = object->instance_animation;
        if (animation != NULL) {
            f32 direction = object->end_frame < object->start_frame ? -1.0f : 1.0f;
            f32 current_frame = animation->ltime * direction;
            if (object->end_frame * direction > current_frame) {
                all_at_end = 0;
            }
            if (current_frame > object->start_frame * direction) {
                all_at_start = 0;
            }
        }
        object = object->next;
    }

    set->state = GAMEANIMSET_STATE_AT_START;
    if (all_at_end != 0) {
        set->state = GAMEANIMSET_STATE_AT_END;
    } else if (all_at_start == 0) {
        set->state = GAMEANIMSET_STATE_BETWEEN_ENDPOINTS;
    }
}

i32 GameAnimSet_GetAveragePos(GAMEANIMSET_s *set, NUVEC *position, i32 frame_selection, i32 include_animated,
                              i32 include_static) {
    NUVEC sum = {0.0f, 0.0f, 0.0f};
    if (position == NULL || set == NULL || set->object_count == 0 || set->objects == NULL)
        return 0;
    i32 count = 0;
    NUMTX matrix;
    GAMEANIMOBJ_s *object = set->objects;
    if (include_static == 0) {
        if (include_animated == 0) {
            do {
                object = object->next;
            } while (object != NULL);
            return 0;
        }
        do {
            if ((object->flags & 1) == 0 && object->instance_animation != NULL) {
                f32 frame;
                if (frame_selection == 0)
                    frame = object->start_frame;
                else if (frame_selection == 1)
                    frame = object->end_frame;
                else
                    frame = object->instance_animation->ltime;
                EvalAnim(&object->special, frame, &matrix, 1);
                ++count;
                NuVecAdd(&sum, &sum, NUMTX_GET_ROW_VEC(&matrix, 3));
            }
            object = object->next;
        } while (object != NULL);
    } else if (include_animated == 0) {
        do {
            if ((object->flags & 1) == 0 && object->instance_animation == NULL) {
                NuVecAdd(&sum, &sum, NuSpecialGetDrawPos(&object->special));
                ++count;
            }
            object = object->next;
        } while (object != NULL);
    } else {
        do {
            if ((object->flags & 1) == 0) {
                if (object->instance_animation != NULL) {
                    f32 frame;
                    if (frame_selection == 0)
                        frame = object->start_frame;
                    else if (frame_selection == 1)
                        frame = object->end_frame;
                    else
                        frame = object->instance_animation->ltime;
                    EvalAnim(&object->special, frame, &matrix, 1);
                    ++count;
                    NuVecAdd(&sum, &sum, NUMTX_GET_ROW_VEC(&matrix, 3));
                } else {
                    NuVecAdd(&sum, &sum, NuSpecialGetDrawPos(&object->special));
                    ++count;
                }
            }
            object = object->next;
        } while (object != NULL);
    }
    if (count == 0)
        return 0;
    NuVecScale(position, &sum, 1.0f / static_cast<f32>(count));
    return 1;
}

GAMEANIMSET_VISIBILITY GameAnimSet_GetVisibility(GAMEANIMSET_s *set) {
    if (set == NULL) {
        return GAMEANIMSET_VISIBILITY_NONE;
    }

    i32 visible_count = 0;
    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        if (NuSpecialGetVisibilityFn(&object->special) == 1) {
            ++visible_count;
        }
    }

    if (visible_count == set->object_count) {
        return GAMEANIMSET_VISIBILITY_ALL;
    }
    if (visible_count > 0) {
        return GAMEANIMSET_VISIBILITY_PARTIAL;
    }
    return GAMEANIMSET_VISIBILITY_NONE;
}

void GameAnimSet_JumpToAnimPos(GAMEANIMSET_s *set, float position) {
    if (set == NULL) {
        return;
    }

    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        nuinstanim_s *animation = object->instance_animation;
        if (animation == NULL || object->animation == NULL) {
            continue;
        }

        animation->playing = 0;
        f32 frame = (object->end_frame - object->start_frame) * position + object->start_frame;
        f32 direction = object->end_frame < object->start_frame ? -1.0f : 1.0f;
        f32 scaled_frame = frame * direction;
        f32 scaled_end = object->end_frame * direction;
        if (scaled_frame <= scaled_end) {
            animation->ltime = frame;
        } else {
            animation->ltime = object->end_frame;
            scaled_frame = scaled_end;
        }
        if (object->start_frame * direction > scaled_frame) {
            animation->ltime = object->start_frame;
        }
    }
}

void GameAnimSet_RemoveSpecial(GAMEANIMSET_s *set, nuhspecial_s *special) {
    if (special == NULL || set == NULL)
        return;
    GAMEANIMOBJ_s *object = set->objects;
    while (object != NULL && NuSpecialCompare(&object->special, special) == 0) {
        object = object->next;
    }
    if (object != NULL)
        GameAnimSet_RemoveObject(set, object);
}

void GameAnimSet_SetVisibility(GAMEANIMSET_s *set, i32 visibility) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            NuSpecialSetVisibility(&object->special, visibility);
        }
    }
}

void GameAnimSet_DrawReflection(GAMEANIMSET_s *set, i32 axis, float offset, numtx_s *matrix) {
    if (set == NULL || set->objects == NULL) {
        return;
    }
    if (matrix == NULL) {
        matrix = NuSpecialGetMtx(&set->objects->special);
    }
    f32 plane = offset + reinterpret_cast<f32 *>(matrix)[11 + axis];
    NuRndrStartReflectionRender(0);
    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        if ((object->flags & 2) == 0 && NuSpecialGetVisibilityFn(&object->special) != 0) {
            NUMTX_ALIGNED16 reflection;
            extern i32 MatrixReflection(NUMTX *, i32, f32, f32, NUMTX *);
            NUMTX *draw_matrix = NuSpecialGetDrawMtx(&object->special);
            if (MatrixReflection(draw_matrix, axis, plane, WORLD->current_level->unknown_0cc, &reflection) != 0) {
                NuSpecialDrawAt(&object->special, &reflection);
            }
        }
    }
    NuRndrEndReflectionRender();
}

GAMEANIMOBJ_s *GameAnimSet_AddObjectByName(GAMEANIMSET_s *set, nugscn_s *scene, char *name, float start_frame,
                                           float end_frame, i32 append, GIZMOSYS_s *gizmo_sys, char *prefix,
                                           char *suffix) {
    if (set == NULL) {
        return NULL;
    }

    nuhspecial_s special;
    if (Gizmo_FindNuSpecial(scene, &special, name, 1, gizmo_sys, prefix, suffix) == 0) {
        return NULL;
    }
    return GameAnimSet_AddObject(set, &special, start_frame, end_frame, append);
}

void GameAnimSet_AddToSystemList(GAMEANIMSET_s *set) {
    if (set != NULL && set->system != NULL && (set->flags & GAMEANIMSET_FLAG_IN_SYSTEM_LIST) == 0) {
        NuLinkedListAppend(&set->system->active_sets, &set->links);
        set->flags = static_cast<GAMEANIMSET_FLAGS>(set->flags | GAMEANIMSET_FLAG_IN_SYSTEM_LIST);
    }
}

f32 GameAnimSet_AutoSetReflectY(GAMEANIMSET_s *set, nuvec_s *position, numtx_s *matrix) {
    if (set != NULL && set->objects != NULL) {
        if (matrix == NULL) {
            matrix = NuSpecialGetMtx(&set->objects->special);
        }
        f32 height = matrix->m31;
        f32 ground = GameShadow(NULL, position, 5.0f, -1);
        if (ground != 2000000.0f) {
            return ground - height;
        }
    }
    return 0.0f;
}

f32 GameAnimSet_GetCurrentFrame(GAMEANIMSET_s *set) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            if (object->instance_animation != NULL) {
                return object->instance_animation->ltime;
            }
        }
    }
    return 0.0f;
}

GAMEANIMOBJPOOL_s *GameAnimSet_CreateObjectPool(variptr_u *buf, variptr_u *buf_end, i32 object_data_size,
                                                i32 capacity) {
    GAMEANIMOBJPOOL_s *pool = NULL;
    if (capacity != 0) {
        pool = static_cast<GAMEANIMOBJPOOL_s *>(GameBufferAlloc(buf, buf_end, sizeof(GAMEANIMOBJPOOL_s)));
        if (pool != NULL) {
            pool->object_data_size = static_cast<u16>(object_data_size);
            pool->capacity = static_cast<u16>(capacity);
            pool->objects = static_cast<GAMEANIMOBJ_s *>(
                GameBufferAlloc(buf, buf_end, static_cast<u16>(capacity) * sizeof(GAMEANIMOBJ_s)));
            if (object_data_size != 0) {
                pool->object_data = GameBufferAlloc(
                    buf, buf_end, static_cast<u16>(pool->object_data_size) * static_cast<u16>(pool->capacity));
            }

            for (i32 i = 0; i < pool->capacity; ++i) {
                GAMEANIMOBJ_s *object = &pool->objects[i];
                object->next = pool->free_objects;
                pool->free_objects = object;
            }
        }
    }
    return pool;
}

i32 GameAnimSet_IsAnimationReset(GAMEANIMSET_s *set) {
    if (set == NULL || set->objects == NULL) {
        return 1;
    }

    i32 animated_count = 0;
    i32 repeating_count = 0;
    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        nuinstanim_s *animation = object->instance_animation;
        if (animation == NULL) {
            continue;
        }

        ++animated_count;
        if (animation->repeating != 0) {
            ++repeating_count;
            continue;
        }
        if (animation->playing != 0) {
            return 0;
        }

        const f32 direction = object->end_frame < object->start_frame ? -1.0f : 1.0f;
        if (animation->ltime * direction > object->start_frame * direction) {
            return 0;
        }
    }

    if (animated_count == 0) {
        return 1;
    }
    return animated_count != repeating_count;
}

void GameAnimSet_RemoveAllObjects(GAMEANIMSET_s *set) {
    if (set != NULL) {
        while (set->objects != NULL)
            GameAnimSet_RemoveObject(set, set->objects);
    }
}

i32 GameAnimSet_GetCentreAndRadius(GAMEANIMSET_s *set, NUVEC *centre, f32 *radius, i32 frame_selection,
                                   i32 include_animated, i32 include_static) {
    f32 object_radius;
    if (centre == NULL || set == NULL || set->object_count == 0 || set->objects == NULL) {
        return 0;
    }

    NUVEC maximum = {-1.0e9f, -1.0e9f, -1.0e9f};
    NUVEC minimum = {1.0e9f, 1.0e9f, 1.0e9f};
    bool found_object = false;

    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        if ((object->flags & 1) != 0) {
            continue;
        }

        NUMTX matrix;
        if (object->instance_animation == NULL) {
            if (include_static == 0) {
                continue;
            }
            NUMTX *draw_matrix = NuSpecialGetDrawMtx(&object->special);
            if (draw_matrix == NULL) {
                continue;
            }
            matrix = *draw_matrix;
        } else {
            if (include_animated == 0) {
                continue;
            }
            f32 frame;
            if (frame_selection == 0) {
                frame = object->start_frame;
            } else if (frame_selection == 1) {
                frame = object->end_frame;
            } else {
                frame = object->instance_animation->ltime;
            }
            EvalAnim(&object->special, frame, &matrix, 1);
        }

        NUVEC4 object_centre;

        NuSpecialGetRadius(&object->special, reinterpret_cast<NUVEC *>(&object_centre), &object_radius);
        object_radius *= 0.75f;
        object_centre.w = 1.0f;
        NuVec4MtxTransformVU0(&object_centre, &object_centre, &matrix);

        const NUVEC object_minimum = {
            object_centre.x - object_radius,
            object_centre.y - object_radius,
            object_centre.z - object_radius,
        };
        const NUVEC object_maximum = {
            object_centre.x + object_radius,
            object_centre.y + object_radius,
            object_centre.z + object_radius,
        };
        if (object_minimum.x < minimum.x) {
            minimum.x = object_minimum.x;
        }
        if (object_minimum.y < minimum.y) {
            minimum.y = object_minimum.y;
        }
        if (object_minimum.z < minimum.z) {
            minimum.z = object_minimum.z;
        }
        if (object_maximum.x > maximum.x) {
            maximum.x = object_maximum.x;
        }
        if (object_maximum.y > maximum.y) {
            maximum.y = object_maximum.y;
        }
        if (object_maximum.z > maximum.z) {
            maximum.z = object_maximum.z;
        }
        found_object = true;
    }

    if (!found_object) {
        return 0;
    }

    centre->x = (maximum.x + minimum.x) * 0.5f;
    centre->y = (maximum.y + minimum.y) * 0.5f;
    centre->z = (maximum.z + minimum.z) * 0.5f;
    if (radius != NULL) {
        const f32 half_x = (maximum.x - minimum.x) * 0.5f;
        const f32 half_y = (maximum.y - minimum.y) * 0.5f;
        const f32 half_z = (maximum.z - minimum.z) * 0.5f;
        *radius = half_x * half_x + half_y * half_y + half_z * half_z;
        *radius = NuFsqrt(*radius);
    }
    return 1;
}

f32 GameAnimSet_GetCompletionRatio(GAMEANIMSET_s *set) {
    if (set == NULL) {
        return 0.0f;
    }
    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        if (object->instance_animation != NULL) {
            if (object->start_frame == object->end_frame) {
                return 1.0f;
            }
            f32 ratio =
                (object->instance_animation->ltime - object->start_frame) / (object->end_frame - object->start_frame);
            if (ratio > 1.0f) {
                ratio = 1.0f;
            }
            if (ratio < 0.0f) {
                ratio = 0.0f;
            }
            return ratio;
        }
    }
    return 0.0f;
}

void GameAnimSet_RemoveFromSystemList(GAMEANIMSET_s *set) {
    if (set != NULL && set->system != NULL) {
        if ((set->flags & GAMEANIMSET_FLAG_IN_SYSTEM_LIST) != 0) {
            NuLinkedListRemove(&set->system->active_sets, &set->links);
            set->flags = static_cast<GAMEANIMSET_FLAGS>(set->flags & ~GAMEANIMSET_FLAG_IN_SYSTEM_LIST);
        }
        GameAnimSet_EvaluateState(set);
    }
}

extern "C" {

    void ANI_FixUpAddrs(ani3_animheader_s *anim, isize delta, i32) {
        if (anim->magic != 0x414e4934) {
            return;
        }
        while (true) {
            if (anim->constants != NULL) {
                anim->constants = reinterpret_cast<i16 *>(reinterpret_cast<usize>(anim->constants) + (usize)delta);
            }
            if (anim->scale_min != NULL) {
                anim->scale_min =
                    reinterpret_cast<ani3_scalemin_s *>(reinterpret_cast<usize>(anim->scale_min) + (usize)delta);
            }
            if (anim->keys != NULL) {
                anim->keys = reinterpret_cast<u8 *>(reinterpret_cast<usize>(anim->keys) + (usize)delta);
            }
            if (anim->curve_types != NULL) {
                anim->curve_types = reinterpret_cast<u16 *>(reinterpret_cast<usize>(anim->curve_types) + (usize)delta);
            }
            if (anim->node_flags != NULL) {
                anim->node_flags = reinterpret_cast<u8 *>(reinterpret_cast<usize>(anim->node_flags) + (usize)delta);
            }
            if (anim->field_38 != NULL) {
                anim->field_38 = reinterpret_cast<void *>(reinterpret_cast<usize>(anim->field_38) + (usize)delta);
            }
            u16 next = anim->next_block;
            if (next == 0) {
                break;
            }
            delta += next;
            anim = reinterpret_cast<ani3_animheader_s *>(reinterpret_cast<usize>(anim) + next);
        }
    }

    // Original @0x2c17d0. The non-quaternion ANI4 player uses a compact
    // four-samples-per-word curve stream. Only groups enabled by the node's
    // translation/rotation/scale flags occupy space in that stream.

    f32 GetInstAnimEndFrame(nugscn_s *scene, nuinstanim_s *instance_animation) {
        if (instance_animation == NULL) {
            return 0.0f;
        }

        void *animation = scene->instance_animation_data[instance_animation->anim_ix];
        if (animation != NULL) {
            return NuAnimEndFrameOld(animation);
        }
        if ((instance_animation->end_frame_lookup_bits & NUINSTANIM_END_FRAME_LOOKUP_MASK) == 0 ||
            scene->animation_end_frames == NULL) {
            return 0.0f;
        }

        return static_cast<f32>(scene->animation_end_frames[instance_animation->end_frame_lookup_index - 1].end_frame);
    }

    NUJOINTPROCANIMFN JointProcAnimFn;

    void SetProceduralAnimationFn(void *function) {
        JointProcAnimFn = reinterpret_cast<NUJOINTPROCANIMFN>(function);
    }

    i32 StateAnimEvaluate(StateAnim *state, u8 *index, u8 *value, f32 frame) {
        u8 next = *index;
        if (next < state->count) {
            i32 changed = 0;
            do {
                if (!(frame >= state->times[next])) {
                    if (changed) {
                        return 1;
                    }
                    break;
                }
                changed = 1;
                *value = state->values[next];
                next = static_cast<u8>(*index + 1);
                *index = next;
            } while (next < state->count);
            if (next >= state->count) {
                return 1;
            }
        }

        i32 changed = 0;
        if (next == 0) {
            return 0;
        }
        do {
            if (!(state->times[next - 1] > frame)) {
                return changed;
            }
            next--;
            *index = next;
            *value = next == 0 ? state->values[0] : state->values[next - 1];
            changed = 1;
            next = *index;
        } while (next != 0);
        return 1;
    }

    i32 StateAnimEvaluate2(StateAnim *state, u8 *index, char *value, f32 frame) {
        i32 current = *index;
        i32 count = state->count;
        if (current >= count) {
            current = count - 1;
        }
        if (current < 0) {
            current = 0;
        }
        char old_value = state->values[current];
        if (!(frame >= state->times[current])) {
            if (current != 0) {
                i32 previous = current - 1;
                current = previous;
                while (!(frame >= state->times[previous])) {
                    if (previous == 0) {
                        break;
                    }
                    --previous;
                    current = previous;
                }
            }
        } else {
            while (true) {
                if (current >= count - 1) {
                    break;
                }
                const i32 next = current + 1;
                if (state->times[next] > frame) {
                    break;
                }
                current = next;
            }
        }
        char new_value = state->values[current];
        *value = new_value;
        *index = static_cast<u8>(current);
        return old_value != new_value;
    }

    void NuSpecialSetInstAnimTime(nuhspecial_s *special, f32 frame) {
        NUGSCN *scene = special->scene;
        if (scene == NULL) {
            return;
        }
        nuinstanim_s *animation = NuSpecialGetInstAnim(special);
        if (animation != NULL) {
            animation->ltime = frame;
            if (!((animation->end_frame_lookup_bits & NUINSTANIM_END_FRAME_LOOKUP_MASK) != 0 &&
                  scene->animation_end_frames != NULL)) {
                return;
            }
            StateAnim *state =
                reinterpret_cast<StateAnim *>(&scene->animation_end_frames[animation->end_frame_lookup_index - 1]);
            char value;
            u8 index = static_cast<u8>(static_cast<u32>(animation->flags) >> NUINSTANIM_STATE_INDEX_SHIFT);
            StateAnimEvaluate2(state, &index, &value, frame);
            animation->flags =
                static_cast<NUINSTANIM_FLAGS>((static_cast<u32>(animation->flags) & ~NUINSTANIM_STATE_INDEX_MASK) |
                                              (static_cast<u32>(index) << NUINSTANIM_STATE_INDEX_SHIFT));
        }
    }

    StateAnim *StateAnimFixPtrs(StateAnim *state, isize delta) {
        if (state == NULL) {
            return NULL;
        }
        state = reinterpret_cast<StateAnim *>(reinterpret_cast<usize>(state) + delta);
        if (state == NULL) {
            return NULL;
        }
        state->times =
            state->times != NULL ? reinterpret_cast<f32 *>(reinterpret_cast<usize>(state->times) + delta) : NULL;
        state->values =
            state->values != NULL ? reinterpret_cast<u8 *>(reinterpret_cast<usize>(state->values) + delta) : NULL;
        return state;
    }

} // extern "C"

void SetAnimFrame(nuhspecial_s *special, float frame) {
    if (NuSpecialExistsFn(special) == 0)
        return;
    NUMTX_ALIGNED16 matrix;
    NuMtxSetIdentity(&matrix);
    nuinstanim_s *instance_animation = NuSpecialGetInstAnim(special);
    if (instance_animation == NULL)
        return;
    nuanimdata_s *animation = special->scene->instance_animation_data[instance_animation->anim_ix];
    if (animation == NULL)
        return;
    // The animation header begins with its final frame; the remaining header is opaque here.
    f32 end_frame;
    memcpy(&end_frame, animation, sizeof(end_frame));
    if (frame == 1.0e9f)
        frame = end_frame;
    if (!(frame >= 1.0f && frame <= end_frame))
        return;
    NuAnimData2CalcMatrix(animation, 0, frame, &matrix);
    instance_animation->mtx = matrix;
    NUMTX *instance_matrix = NuSpecialGetInstanceMtx(special);
    instance_animation->mtx.m30 = instance_matrix->m30;
    instance_animation->mtx.m31 = instance_matrix->m31;
    instance_animation->mtx.m32 = instance_matrix->m32;
    instance_animation->ltime = frame;
}

i32 GetAnimDirection(nuinstanim_s *animation) {
    if (animation == NULL || animation->tfactor == 0.0f) {
        return -1;
    }
    return animation->tfactor < 0.0f ? 1 : 0;
}

static char **TexAnimList;

void InitTexAnimScripts(char **names) {
    TexAnimList = names;
    if (names == NULL)
        return;
    while (*names != NULL) {
        permbuffer_ptr.addr = ALIGN(permbuffer_ptr.addr, 4);
        char path[72];
        NuStrCpy(path, "stuff\\ats\\");
        NuStrCat(path, *names++);
        NuStrCat(path, ".ats");
        NuTexAnimProgReadScript(path, &permbuffer_ptr, permbuffer_end, static_cast<i32>(DEFAULTFPS));
    }
    permbuffer_ptr.addr = ALIGN(permbuffer_ptr.addr, 16);
}

i32 GizmoFileReadGameAnimSet(GAMEANIMSET_s *set, void *world_ptr,
                             void (*read_object_data)(GAMEANIMOBJ_s *, unsigned char), unsigned char version,
                             char *prefix, char *suffix) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    const unsigned char file_version = static_cast<unsigned char>(EdFileReadChar());
    const unsigned char object_count = static_cast<unsigned char>(EdFileReadChar());
    i32 success = 1;

    for (i32 object_index = 0; object_index < object_count; ++object_index) {
        char object_name[64];
        const i32 name_length = static_cast<signed char>(EdFileReadChar());
        if (name_length != 0) {
            EdFileRead(object_name, name_length);
        }

        const f32 start_frame = EdFileReadFloat();
        const f32 end_frame = EdFileReadFloat();
        u32 flags = 0;
        if (file_version > 1) {
            flags = EdFileReadInt();
        }

        if (name_length == 0) {
            continue;
        }

        if (set->object_pool == NULL || set->object_pool->free_objects == NULL) {
            success = 0;
        }

        GAMEANIMOBJ_s *object = GameAnimSet_AddObjectByName(set, world->current_gscn, object_name, start_frame,
                                                            end_frame, 0, world->gizmo_sys, prefix, suffix);
        GAMEANIMOBJ_s missing_object;
        if (object == NULL) {
            memset(&missing_object, 0, sizeof(missing_object));
            object = &missing_object;
        }

        object->flags = flags;
        if (read_object_data != NULL) {
            read_object_data(object, version);
        }
        if (file_version <= 2) {
            object->flags &= ~2u;
        }
    }

    return success;
}

void EvalAnim(nuhspecial_s *special, float frame, numtx_s *matrix, i32 include_instance_translation) {
    if (matrix == NULL || special == NULL) {
        return;
    }

    nuinstanim_s *instance_animation = NuSpecialGetInstAnim(special);
    if (instance_animation == NULL) {
        if (include_instance_translation != 0) {
            NUMTX *instance_matrix = NuSpecialGetInstanceMtx(special);
            if (instance_matrix != NULL) {
                memcpy(matrix, instance_matrix, sizeof(NUMTX));
            }
        }
        return;
    }

    NUGSCN *scene = special->scene;
    nuanimdata_s *animation = scene->instance_animation_data[instance_animation->anim_ix];
    if (animation == NULL) {
        return;
    }

    NuAnimData2CalcMatrix(animation, 0, frame, matrix);
    if (include_instance_translation != 0) {
        NUMTX *instance_matrix = NuSpecialGetInstanceMtx(special);
        if (instance_matrix != NULL) {
            NUVEC *translation = NUMTX_GET_ROW_VEC(matrix, 3);
            const NUVEC *instance_translation = NUMTX_GET_ROW_VEC(instance_matrix, 3);
            translation->x += instance_translation->x;
            translation->y += instance_translation->y;
            translation->z += instance_translation->z;
        }
    }
}

void EvalAnim2(nuhspecial_s *special, float frame) {
    nuinstanim_s *instance_animation = NuSpecialGetInstAnim(special);
    if (instance_animation == NULL) {
        return;
    }

    NUGSCN *scene = special->scene;
    nuanimdata_s *animation = scene->instance_animation_data[instance_animation->anim_ix];
    if (animation == NULL || frame == instance_animation->prev_eval_time) {
        return;
    }

    NUMTX *instance_matrix = NuSpecialGetInstanceMtx(special);
    NuAnimData2CalcMatrix(animation, 0, frame, &instance_animation->mtx);

    instance_animation->prev_eval_time = frame;
    const i32 instance_index = instance_animation - scene->instance_animations;
    NUVEC &animation_translation = *NUMTX_GET_ROW_VEC(&instance_animation->mtx, 3);
    NUVEC &instance_translation = *NUMTX_GET_ROW_VEC(instance_matrix, 3);
    NUMTX *evaluated_matrix = &scene->instance_animation_matrices[instance_index];
    memcpy(evaluated_matrix, &instance_animation->mtx, sizeof(NUMTX));

    animation_translation.x += instance_translation.x;
    animation_translation.y += instance_translation.y;
    animation_translation.z += instance_translation.z;
    NuSpecialUpdate(special);
}

void GameAnimSys_AllocateLevelProgressData(variptr_u *buf, variptr_u *buf_end, i32 capacity, i32 level_count) {
    if (buf_end == NULL || buf == NULL)
        return;
    gameanimsysprogress.count = level_count;
    gameanimsysprogress.entry_size = capacity;
    gameanimsysprogress.entries = static_cast<u8 **>(GameBufferAlloc(buf, buf_end, level_count * sizeof(u8 *)));
    if (gameanimsysprogress.entries != NULL) {
        for (i32 i = 0; i < level_count; ++i)
            gameanimsysprogress.entries[i] = static_cast<u8 *>(GameBufferAlloc(buf, buf_end, capacity));
    }
}

u8 *GameAnimSys_GetProgressData(i32 index) {
    if (index < 0 || index >= gameanimsysprogress.count)
        return NULL;
    return gameanimsysprogress.entries[index];
}

void GameAnimSys_StoreProgress(GAMEANIMSYS_s *system, i32 index) {
    if (system == NULL || system->sets == NULL || index < 0 || index >= gameanimsysprogress.count)
        return;
    u8 *progress = gameanimsysprogress.entries[index];
    for (i32 i = 0; i < gameanimsysprogress.entry_size; ++i) {
        GAMEANIMSET_s *set = system->sets[i];
        if (set == NULL)
            return;
        progress[i] = static_cast<u8>(set->state);
    }
}

void GameAnimSys_ReStoreProgress(GAMEANIMSYS_s *system, i32 index) {
    if (system == NULL || system->sets == NULL || index < 0 || index >= gameanimsysprogress.count)
        return;
    u8 *progress = gameanimsysprogress.entries[index];
    for (i32 i = 0; i < gameanimsysprogress.entry_size; ++i) {
        GAMEANIMSET_s *set = system->sets[i];
        if (set == NULL)
            return;
        const i8 state = static_cast<i8>(progress[i]);
        set->state = static_cast<GAMEANIMSET_STATE>(state);
        if (state == GAMEANIMSET_STATE_ACTIVE_FORWARD || state == GAMEANIMSET_STATE_ACTIVE_BACKWARD) {
            if ((set->flags & GAMEANIMSET_FLAG_IN_SYSTEM_LIST) == 0)
                GameAnimSet_AddToSystemList(set);
        } else if ((set->flags & GAMEANIMSET_FLAG_IN_SYSTEM_LIST) != 0)
            GameAnimSet_RemoveFromSystemList(set);
    }
}

GAMEANIMSYS_s *GameAnimSys_Create(variptr_u *buf, variptr_u *buf_end) {
    GAMEANIMSYS_s *system = static_cast<GAMEANIMSYS_s *>(GameBufferAlloc(buf, buf_end, sizeof(GAMEANIMSYS_s)));
    if (system != NULL && gameanimsysprogress.entry_size != 0) {
        system->sets = static_cast<GAMEANIMSET_s **>(
            GameBufferAlloc(buf, buf_end, gameanimsysprogress.entry_size * sizeof(GAMEANIMSET_s *)));
    }
    return system;
}
