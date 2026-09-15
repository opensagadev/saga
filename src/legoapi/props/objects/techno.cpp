#include "legoapi/props/objects/techno.h"

#include "batman.h"
#include "decomp.h"
#include "gameapi/edtools/edfile.h"
#include "gamelib/util/gamelib_util_types.h"
#include "globals.h"
#include "legoapi/audio/audio.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/items/base/animpacket.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"

struct TECHNOPROGRESS {
    i32 state[2];
};

extern "C" void NewTerrPlatformsOff(void);
f32 GameShadow(GameObject_s *object, NUVEC *position, f32 probe_height, i32 terrain_mask);
void FindAnglesZX(NUVEC *normal, u16 *x_rotation, u16 *z_rotation);

TECHNO_CONFIG TechnoSys = {
    0.2f,
    0x55,
    0x2c,
    -1,
    -1,
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    0x2d,
    0x2e,
    {0.0991f, 0.0f, 0.0762f},
    {-0.0991f, 0.0f, 0.0762f},
};

i32 techno_gizmotype_id = -1;

static f32 TechnoMoveSpeed[2];

static void *Technos_ReserveBufferSpace(void *world_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    TECHNO *result = NULL;
    world->technos = NULL;
    world->ntechnos = 0;
    if (world->current_level->max_technos != 0) {
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
        result = static_cast<TECHNO *>(world->giz_buffer.void_ptr);
        world->technos = result;
        world->giz_buffer.addr += world->current_level->max_technos * sizeof(TECHNO);
    }
    return result;
}

static void Techno_SetVisibility(GIZMO *gizmo, i32 visible) {
    if (gizmo == NULL) {
        return;
    }
    TECHNO *techno = static_cast<TECHNO *>(gizmo->object);
    techno->visible = visible != 0;
}

static void Techno_Activate(GIZMO *gizmo, i32 active) {
    if (gizmo == NULL) {
        return;
    }
    TECHNO *techno = static_cast<TECHNO *>(gizmo->object);
    techno->active = active != 0;
}

static void Technos_EarlyUpdate(void *world_ptr, void *, float) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (world == NULL || world->technos == NULL) {
        return;
    }
    TECHNO *techno = world->technos;
    for (i32 index = 0; index < world->ntechnos; ++index, ++techno) {
        techno->flags &= ~TECHNO_FLAG_USED_THIS_FRAME;
    }
}

static i32 Technos_GetMaxGizmos(void *world_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (world == NULL) {
        return 0;
    }
    return world->current_level->max_technos;
}

static char *Techno_GetGizmoName(GIZMO *gizmo) {
    return gizmo != NULL ? static_cast<TECHNO *>(gizmo->object)->name : NULL;
}

static char *Techno_GetOutputName(GIZMO *gizmo, i32 output_index) {
    return const_cast<char *>("Active");
}

static i32 Techno_GetNumOutputs(GIZMO *gizmo) {
    return 1;
}

static NUVEC *Techno_GetPos(GIZMO *gizmo) {
    if (gizmo == NULL || gizmo->object == NULL) {
        return NULL;
    }
    return &static_cast<TECHNO *>(gizmo->object)->position;
}

static void Technos_Reset(void *world_ptr, void *, void *progress_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    TECHNOPROGRESS *progress = static_cast<TECHNOPROGRESS *>(progress_ptr);
    if (world == NULL || world->technos == NULL) {
        return;
    }

    for (i32 index = 0; index < world->ntechnos; ++index) {
        TECHNO &techno = world->technos[index];
        techno.ground_position.x = 0.0f;
        techno.ground_position.y = 0.0f;
        techno.ground_position.z = TechnoSys.interaction_time;
        NuVecRotateY(&techno.ground_position, &techno.ground_position, techno.y_rotation);
        NuVecAdd(&techno.ground_position, &techno.ground_position, &techno.position);

        NewTerrPlatformsOff();
        const f32 floor_height = GameShadow(NULL, &techno.ground_position, 5.0f, -1);
        if (floor_height == floor_height) {
            techno.ground_position.y = 2000000.0f;
            techno.ground_offset = 0.0f;
        } else {
            techno.ground_position.y = floor_height + 0.005f;
            FindAnglesZX(&ShadNorm, &techno.ground_x_rotation, &techno.ground_z_rotation);
        }

        techno.flags =
            static_cast<u8>((techno.flags | TECHNO_FLAG_ACTIVE | TECHNO_FLAG_VISIBLE) & ~TECHNO_FLAG_USED_THIS_FRAME);
        NuStrCpy(techno.target_name, techno.target_object_name);

        if (progress != NULL && index <= 31) {
            const u32 bit = 1u << index;
            const u8 visible = (progress->state[1] & bit) != 0;
            techno.flags = static_cast<u8>((techno.flags & ~TECHNO_FLAG_VISIBLE) | (visible << 1));
            const u8 active = (progress->state[0] & bit) != 0;
            techno.flags = static_cast<u8>((techno.flags & ~TECHNO_FLAG_ACTIVE) | active);
        }
    }
}

static void Technos_ClearProgress(void *, void *progress_data) {
    TECHNOPROGRESS *progress = (TECHNOPROGRESS *)progress_data;
    if (progress == NULL) {
        return;
    }

    progress->state[0] = -1;
    progress->state[1] = -1;
}

static void *Technos_AllocateProgressData(VARIPTR *buffer, VARIPTR *buffer_end) {
    return GizmoBufferAlloc(buffer, buffer_end, sizeof(TECHNOPROGRESS));
}

static i32 Techno_GetOutput(GIZMO *gizmo, i32, i32) {
    if (gizmo == NULL || gizmo->object == NULL) {
        return 0;
    }

    TECHNO *techno = static_cast<TECHNO *>(gizmo->object);
    if ((techno->flags & (TECHNO_FLAG_ACTIVE | TECHNO_FLAG_VISIBLE)) != (TECHNO_FLAG_ACTIVE | TECHNO_FLAG_VISIBLE) ||
        techno->controlled_object == NULL) {
        return 0;
    }
    if (techno->target_mode == 1) {
        return (techno->flags & TECHNO_FLAG_COMPLETE) != 0;
    }
    return 0;
}

static void Technos_Draw(void *world_ptr, void *, float) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (TechnoSys.active_effect_id == -1 || world->lev_objs[TechnoSys.active_effect_id].active == 0 ||
        world->technos == NULL) {
        return;
    }

    const u16 spin_angle = static_cast<u16>(NuFmod(GameTimer.time_elapsed, 5.0f) / 5.0f * 65536.0f);
    const f32 pulse_phase = NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f) * 2.0f * 65536.0f;
    const f32 pulse = NuTrigTable[(static_cast<i32>(pulse_phase) >> 1) & 0x7fff] * 0.2f + 0.8f;

    TECHNO *techno = world->technos;
    for (i32 index = 0; index < world->ntechnos; ++index, ++techno) {
        if ((techno->flags & TECHNO_FLAG_VISIBLE) == 0) {
            continue;
        }

        NUMTX matrix;
        if (TechnoSys.active_effect_id != -1) {
            NuMtxSetTranslation(&matrix, &TechnoSys.idle_offset);
            NuMtxRotateY(&matrix, techno->y_rotation);
            NuMtxTranslate(&matrix, &techno->position);
            NuSpecialDrawAt(&world->lev_objs[TechnoSys.active_effect_id].special, &matrix);
        }
        if (TechnoSys.success_effect_id != -1) {
            NuMtxSetTranslation(&matrix, &TechnoSys.active_offset);
            NuMtxRotateY(&matrix, techno->y_rotation);
            NuMtxTranslate(&matrix, &techno->position);
            NuSpecialDrawAt(&world->lev_objs[TechnoSys.success_effect_id].special, &matrix);
        }
        if (TechnoSys.failure_effect_id != -1) {
            NuMtxSetTranslation(&matrix, &TechnoSys.complete_offset);
            NuMtxRotateY(&matrix, techno->y_rotation);
            NuMtxTranslate(&matrix, &techno->position);
            NuSpecialDrawAt(&world->lev_objs[TechnoSys.failure_effect_id].special, &matrix);
        }

        if (world->lev_objs[TechnoSys.activation_effect_id].active != 0) {
            const f32 hand_phase = techno->ground_offset * 32768.0f + 16384.0f;
            const i32 hand_angle = static_cast<i32>(
                (1.0f - (NuTrigTable[(static_cast<i32>(hand_phase) >> 1) & 0x7fff] + 1.0f) * 0.5f) * 29127.0f);
            NuMtxSetRotationY(&matrix, hand_angle);
            NuMtxTranslate(&matrix, &TechnoSys.left_hand_offset);
            NuMtxRotateY(&matrix, techno->y_rotation);
            NUVEC *translation = NUMTX_GET_ROW_VEC(&matrix, 3);
            NuVecAdd(translation, translation, &techno->position);
            NuSpecialDrawAt(&world->lev_objs[TechnoSys.activation_effect_id].special, &matrix);
        }
        if (world->lev_objs[TechnoSys.completion_effect_id].active != 0) {
            const f32 hand_phase = techno->ground_offset * 32768.0f + 16384.0f;
            const i32 hand_angle = static_cast<i32>(
                (1.0f - (NuTrigTable[(static_cast<i32>(hand_phase) >> 1) & 0x7fff] + 1.0f) * 0.5f) * 29127.0f);
            NuMtxSetRotationY(&matrix, -hand_angle);
            NuMtxTranslate(&matrix, &TechnoSys.right_hand_offset);
            NuMtxRotateY(&matrix, techno->y_rotation);
            NUVEC *translation = NUMTX_GET_ROW_VEC(&matrix, 3);
            NuVecAdd(translation, translation, &techno->position);
            NuSpecialDrawAt(&world->lev_objs[TechnoSys.completion_effect_id].special, &matrix);
        }

        if ((techno->flags & TECHNO_FLAG_ACTIVE) != 0 && techno->ground_position.y != 2000000.0f &&
            world->lev_objs[TechnoSys.floor_target_object_id].active != 0 && pulse > 0.0f) {
            NuMtxSetRotationY(&matrix, spin_angle);
            if (techno->ground_z_rotation != 0) {
                NuMtxRotateZ(&matrix, techno->ground_z_rotation);
            }
            if (techno->ground_x_rotation != 0) {
                NuMtxRotateX(&matrix, techno->ground_x_rotation);
            }
            NuMtxTranslate(&matrix, &techno->ground_position);
            NuSpecialDrawAtAlpha(&world->lev_objs[TechnoSys.floor_target_object_id].special, &matrix, pulse);
        }
    }
}

static void Technos_LateUpdate(void *world_ptr, void *, float) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (world != NULL) {
        TECHNO *techno = world->technos;
        if (techno != NULL && world->ntechnos > 0) {
            for (i32 index = 0; index < world->ntechnos; ++index, ++techno) {
                techno->ground_offset =
                    SeekLinearF(techno->ground_offset, (techno->flags & TECHNO_FLAG_USED_THIS_FRAME) != 0 ? 1.0f : 0.0f,
                                FRAMETIME * 2.5f);
            }
        }
    }
}

static void Technos_AddGizmos(GIZMOSYS *gizmo_sys, i32 type_id, void *world_ptr, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    for (i32 index = 0; index < world->ntechnos; ++index) {
        TECHNO &techno = world->technos[index];
        if (NuStrLen(techno.name) != 0) {
            AddGizmo(gizmo_sys, type_id, NULL, &techno);
        }
    }
}

static i32 Technos_Load(void *world_ptr, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (world->ntechnos != 0) {
        return 0;
    }

    const i32 version = EdFileReadInt();
    world->ntechnos = EdFileReadInt();
    for (i32 index = 0; index < world->ntechnos; ++index) {
        TECHNO &techno = world->technos[index];
        EdFileRead(techno.name, sizeof(techno.name));
        EdFileReadNuVec(&techno.position);
        techno.y_rotation = static_cast<i16>(EdFileReadShort());

        if (version <= 1) {
            techno.enabled = 1;
            techno.scale = 1.0f;
            techno.output = 0;
            continue;
        }

        techno.target_mode = static_cast<u8>(EdFileReadChar());
        const i32 target_name_length = EdFileReadInt();
        EdFileRead(techno.target_object_name, target_name_length);
        NuStrCpy(techno.target_name, techno.target_object_name);
        if (version == 2) {
            techno.enabled = 1;
            techno.scale = 1.0f;
            techno.output = 0;
            continue;
        }

        techno.enabled = static_cast<u8>(EdFileReadChar());
        if (version == 3) {
            techno.scale = 1.0f;
            techno.output = 0;
            continue;
        }

        techno.scale = EdFileReadFloat();
        techno.output = version == 4 ? 0 : EdFileReadInt();
    }
    return 1;
}

static void Technos_StoreProgress(void *world_ptr, void *, void *progress_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    TECHNOPROGRESS *progress = static_cast<TECHNOPROGRESS *>(progress_ptr);
    if (progress == NULL) {
        return;
    }

    progress->state[0] = -1;
    progress->state[1] = -1;
    if (world == NULL || world->technos == NULL) {
        return;
    }

    const i32 count = world->ntechnos < 32 ? world->ntechnos : 32;
    for (i32 index = 0; index < count; ++index) {
        const u32 bit = 1u << index;
        if ((world->technos[index].flags & TECHNO_FLAG_VISIBLE) == 0) {
            progress->state[1] &= ~bit;
        }
        if ((world->technos[index].flags & TECHNO_FLAG_ACTIVE) == 0) {
            progress->state[0] &= ~bit;
        }
    }
}

TECHNO *Technos_FindControllingTechno(GameObject_s *object) {
    if (object != NULL) {
        for (i32 index = 0; index < WORLD->ntechnos; ++index) {
            TECHNO *techno = &WORLD->technos[index];
            if (techno->target_mode == 1 && techno->controlled_object == object)
                return techno;
        }
    }
    return NULL;
}

void *Technos_FindTgt(TECHNO_s *techno) {
    if (techno == NULL || techno->controlled_object != NULL) {
        return techno != NULL ? techno->controlled_object : NULL;
    }

    switch (techno->target_mode) {
        case 0: {
            void *target = GetNamedGameObject(WORLD->ai_sys, techno->target_name);
            if (target != NULL) {
                techno->controlled_object = target;
                techno->target_mode = 1;
                break;
            }
            target = GizmoFindByName(WORLD->gizmo_sys, -1, techno->target_name);
            if (target != NULL) {
                techno->controlled_object = target;
                techno->target_mode = 3;
                break;
            }
            if (NuSpecialFind(WORLD->current_gscn, reinterpret_cast<nuhspecial_s *>(techno->target_special_storage),
                              techno->target_name, 0) != 0) {
                techno->target_mode = 2;
                techno->controlled_object = techno->target_special_storage;
                break;
            }
            techno->target_mode = 0;
            techno->controlled_object = NULL;
            break;
        }
        case 1:
            techno->controlled_object = GetNamedGameObject(WORLD->ai_sys, techno->target_name);
            if (techno->controlled_object == NULL) {
                techno->target_mode = 0;
            }
            break;
        case 2:
            techno->controlled_object = techno->target_special_storage;
            if (NuSpecialFind(WORLD->current_gscn, reinterpret_cast<nuhspecial_s *>(techno->target_special_storage),
                              techno->target_name, 0) == 0) {
                techno->target_mode = 0;
                techno->controlled_object = NULL;
            }
            break;
        case 3:
            techno->controlled_object = GizmoFindByName(WORLD->gizmo_sys, -1, techno->target_name);
            if (techno->controlled_object == NULL) {
                techno->target_mode = 0;
            }
            break;
        default:
            techno->target_mode = 0;
            break;
    }
    return techno->controlled_object;
}

NUVEC *Technos_TgtPos(TECHNO_s *techno) {
    if (techno == NULL) {
        return NULL;
    }

    if (techno->controlled_object == NULL) {
        Technos_FindTgt(techno);
    }

    switch (techno->target_mode) {
        case 1:
            return &static_cast<GameObject_s *>(techno->controlled_object)->apiobj.collision_position;
        case 2:
            return NuSpecialGetPos(techno->controlled_object);
        case 3:
            return GizmoGetPos(WORLD->gizmo_sys, static_cast<GIZMO *>(techno->controlled_object));
        default:
            return NULL;
    }
}

void Technos_MoveTarget(TECHNO_s *techno, GameObject_s *object) {
    f32 speed = 0.0f;
    if (object != NULL) {
        if (static_cast<i8>(object->apiobj.flags_low) >= 0 && object->use_action == 2) {
            speed = 1.0f;
        } else if ((techno->enabled & 1) != 0) {
            if ((object->pad_gamepad->allocated_5a & GAMEPAD_RUNTIME_WAGGLED) != 0) {
                speed = 1.0f;
            }
        } else {
            if ((techno->enabled & 2) != 0) {
                speed = object->pad_gamepad->waggle_magnitude;
            } else if ((techno->enabled & 4) != 0) {
                speed = object->pad_gamepad->input_direction_x;
            } else if ((techno->enabled & 8) != 0) {
                speed = object->pad_gamepad->input_direction_z;
            }
            if ((techno->output & 1) != 0 && speed < 0.0f) {
                speed = 0.0f;
            }
        }
    }

    if (techno->controlled_object == NULL) {
        Technos_FindTgt(techno);
    }
    if (techno->target_mode != 3) {
        return;
    }

    GIZMO *gizmo = static_cast<GIZMO *>(techno->controlled_object);
    if (NuStrICmp(gizmotypes->types[gizmo->type_id].name, "GIZOBSTACLE") != 0) {
        return;
    }

    GIZOBSTACLE_s *obstacle = static_cast<GIZOBSTACLE_s *>(gizmo->object);
    if ((techno->enabled & 2) != 0 && object != NULL && static_cast<u8>(object->apiobj.field_0x27c) < 2) {
        f32 rate = object->pad_gamepad->waggle_magnitude != 0.0f ? 3.0f * FRAMETIME : 2.0f * FRAMETIME;
        TechnoMoveSpeed[static_cast<i8>(object->apiobj.field_0x27c)] =
            SeekLinearF(TechnoMoveSpeed[static_cast<i8>(object->apiobj.field_0x27c)], speed, rate);
        GizObstacle_SetTechnoControlled(obstacle, TechnoMoveSpeed[static_cast<i8>(object->apiobj.field_0x27c)]);
    } else {
        GizObstacle_SetTechnoControlled(obstacle, speed);
    }
}

i32 GizTechno_CanUseTechno(GameObject_s *, TECHNO_s *) {
    return 1;
}

TECHNO *Techno_FindNearest(WORLDINFO_s *world, nuvec_s *position, GameObject_s *object, float *distance) {
    TECHNO *nearest = NULL;
    f32 nearest_distance = 1000000000.0f;

    TECHNO *techno = world->technos;
    for (i32 i = 0; i < world->ntechnos; ++i, ++techno) {
        f32 candidate_distance;
        if (object != NULL) {
            if ((techno->flags & (TECHNO_FLAG_ACTIVE | TECHNO_FLAG_VISIBLE)) !=
                    (TECHNO_FLAG_ACTIVE | TECHNO_FLAG_VISIBLE) ||
                techno->ground_position.y == 2000000.0f) {
                continue;
            }
            candidate_distance = NuVecDistSqr(position, &techno->ground_position, NULL);
        } else {
            candidate_distance = NuVecDistSqr(position, &techno->position, NULL);
        }
        if (candidate_distance < nearest_distance) {
            nearest_distance = candidate_distance;
            nearest = techno;
        }
    }

    if (distance != NULL) {
        *distance = nearest_distance;
    }
    return nearest;
}

ADDGIZMOTYPE *Technos_RegisterGizmo(i32 type_id) {
    static ADDGIZMOTYPE addtype;

    addtype = Default_ADDGIZMOTYPE;
    addtype.name = "Techno";
    addtype.prefix = "";
    addtype.fns.unknown1 = 8;
    addtype.fns.early_update_fn = Technos_EarlyUpdate;
    addtype.fns.panel_draw_fn = NULL;
    addtype.fns.get_visibility_fn = NULL;
    addtype.fns.get_max_gizmos_fn = Technos_GetMaxGizmos;
    addtype.fns.get_pos_fn = Techno_GetPos;
    addtype.fns.using_special_fn = NULL;
    addtype.fns.add_gizmos_fn = Technos_AddGizmos;
    addtype.fns.bolt_hit_plat_fn = NULL;
    addtype.fns.get_best_bolt_target_fn = NULL;
    addtype.fns.late_update_fn = Technos_LateUpdate;
    addtype.fns.bolt_hit_fn = NULL;
    addtype.fns.draw_fn = Technos_Draw;
    addtype.fns.get_gizmo_name_fn = Techno_GetGizmoName;
    addtype.fns.get_output_fn = Techno_GetOutput;
    addtype.fns.get_output_name_fn = Techno_GetOutputName;
    addtype.fns.get_num_outputs_fn = Techno_GetNumOutputs;
    addtype.fns.activate_fn = Techno_Activate;
    addtype.fns.activate_rev_fn = NULL;
    addtype.fns.set_visibility_fn = Techno_SetVisibility;
    addtype.fns.allocate_progress_data_fn = Technos_AllocateProgressData;
    addtype.fns.clear_progress_fn = Technos_ClearProgress;
    addtype.fns.store_progress_fn = Technos_StoreProgress;
    addtype.fns.reset_fn = Technos_Reset;
    addtype.fns.reserve_buffer_space_fn = Technos_ReserveBufferSpace;
    addtype.fns.load_fn = Technos_Load;
    addtype.fns.post_load_fn = NULL;
    addtype.fns.add_level_sfx_fn = NULL;
    techno_gizmotype_id = type_id;

    return &addtype;
}

i32 Techno_isReady(TECHNO_s *techno) {
    if (techno == NULL) {
        return 0;
    }

    if (techno->controlled_object == NULL) {
        Technos_FindTgt(techno);
    }

    switch (techno->target_mode) {
        case 1: {
            GameObject_s *object = static_cast<GameObject_s *>(techno->controlled_object);
            return object != NULL && (object->apiobj.field_0x1f8 & 0x1001) == 0x1001;
        }
        case 2:
            return NuSpecialGetVisibilityFn(techno->controlled_object) != 0;
        case 3:
            return techno->controlled_object != NULL;
        default:
            return 0;
    }
}

void Techno_MoveCode(WORLDINFO_s *world, GameObject_s *object) {
    if (object->character_context != 0x51) {
        if ((static_cast<i8>(object->apiobj.flags_low) < 0 || object->use_action == 2) && object->suit != NULL &&
            (static_cast<SUIT_s *>(object->suit)->flags & 0x20) != 0) {
            f32 distance;
            TECHNO *techno = Techno_FindNearest(world, &object->apiobj.lower_position, object, &distance);
            if (techno == NULL) {
                return;
            }

            f32 range = object->apiobj.field_0x1dc + 2000000.0f;
            f32 hint_range = range * 2.5f;
            if (hint_range * hint_range > distance) {
                techno->flags |= TECHNO_FLAG_USED_THIS_FRAME;
            }

            if (object->apiobj.field_0x27d == 0 || ObjLandReady(object) == 0 || !(range * range > distance) ||
                ((object->pad_gamepad->buttons_pressed & GAMEPAD_SPECIAL) == 0 && object->use_action != 2)) {
                return;
            }

            if (Techno_isReady(techno) != 0) {
                object->character_context = 0x51;
                object->field_0x788 = techno;
                object->apiobj.movement_facing_angle = techno->y_rotation + 0x8000;
                object->context_animation = 0x99;
                object->context_animation_timer = 0.0f;
                GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
                if (static_cast<u8>(object->apiobj.field_0x27c) < 2) {
                    TechnoMoveSpeed[static_cast<u8>(object->apiobj.field_0x27c)] = 0.0f;
                }
            } else {
                GameAudio_PlaySfx(0x32, &techno->position, 0, 0);
            }
        }
    } else {
        TECHNO *techno = static_cast<TECHNO *>(object->field_0x788);
        techno->flags |= TECHNO_FLAG_USED_THIS_FRAME;

        if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL &&
            AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0) == NULL) {
            return;
        }

        object->context_animation_timer += FRAMETIME;
        if ((object->pad_gamepad->buttons_pressed & GAMEPAD_TAG) != 0 ||
            (static_cast<i8>(object->apiobj.flags_low) >= 0 && object->use_action != 2)) {
            GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
            object->character_context = -1;
            object->tag_flags |= 1;
            object->apiobj.movement_facing_angle += 0x8000;
            Technos_MoveTarget(techno, NULL);
        } else {
            Technos_MoveTarget(techno, object);
        }
    }
}

i32 Techno_FindOperator(void *target, GAMEPAD_s **pad, GameObject_s **operator_object) {
    for (i32 index = 0; index < 8; ++index) {
        if (Player[index] != NULL && Player[index]->character_context == 0x51 && Player[index]->field_0x788 != NULL) {
            TECHNO *techno = static_cast<TECHNO *>(Player[index]->field_0x788);
            if ((techno->target_mode == 2 &&
                 NuSpecialCompare(static_cast<nuhspecial_s *>(target),
                                  static_cast<nuhspecial_s *>(techno->controlled_object)) != 0) ||
                static_cast<TECHNO *>(Player[index]->field_0x788)->controlled_object == target) {
                if (pad != NULL) {
                    *pad = Player[index]->pad_gamepad;
                }
                if (operator_object != NULL) {
                    *operator_object = Player[index];
                }
                return 1;
            }
        }
    }
    return 0;
}
