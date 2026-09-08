#include "legoapi/gizmos/transport/tightropes.h"

#include "decomp.h"
#include "gameapi/edtools/edfile.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

#include "legoapi/world/level.h"
#include "legoapi/world/world.h"

extern "C" f32 AnimDuration(i32, i32, f32, f32, i32);
void FindAnglesZX(NUVEC *, u16 *, u16 *);
void GameObjectOrigin(GameObject_s *);

struct TIGHTROPEPROGRESS {
    i32 state[2];
};

TIGHTROPE *TightRope_InRange(GameObject_s *object, WORLDINFO_s *world, NUVEC *result) {
    TIGHTROPE *rope = world->tightropes;
    const f32 width = 3.0f * object->apiobj.field_0x1dc;
    const NUVEC position = object->apiobj.collision_position;
    for (i32 i = 0; i < world->tightrope_count; ++i, ++rope) {
        if (rope->enabled == 0 || rope->active == 0) {
            continue;
        }
        NUVEC local;
        local.x = position.x - rope->start.x;
        local.z = position.z - rope->start.z;
        NuVecRotateY(&local, &local, -rope->angle);
        if (local.z >= 0.0f && local.z <= rope->horizontal_length && local.x >= -width && local.x <= width) {
            local.x = 0.0f;
            local.y = rope->start.y + (rope->end.y - rope->start.y) * (local.z / rope->horizontal_length);
            f32 margin = object->apiobj.field_0x1e0;
            if (NuFabs(local.y - position.y) < margin) {
                if (result != NULL) {
                    if ((static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_090 &
                         0x10000000) == 0) {
                        margin = object->apiobj.field_0x1dc;
                    }
                    if (local.z > rope->horizontal_length - margin) {
                        local.z = rope->horizontal_length - margin;
                    } else if (local.z < margin) {
                        local.z = margin;
                    }
                    NuVecRotateY(result, &local, rope->angle);
                    result->x += rope->start.x;
                    result->z += rope->start.z;
                }
                return rope;
            }
        }
    }
    return NULL;
}

static __used__ i32 TightRope_Attach(GameObject_s *object, WORLDINFO_s *world) {
    NUVEC point;
    TIGHTROPE *rope = TightRope_InRange(object, world, &point);
    if (rope == NULL) {
        return 0;
    }
    object->character_context = 0x44;
    object->field_0x788 = rope;
    object->context_variant_flags =
        (object->context_variant_flags & ~8) |
        (((static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_090 & 0x80000) == 0)
         << 3);
    if (object->apiobj.character_model->model_data_b[0x8f] != NULL &&
        ((static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_090 & 0x80000) == 0 ||
         point.y > (object->apiobj.upper_position.y - object->apiobj.lower_position.y) * 0.25f +
                       object->apiobj.lower_position.y)) {
        object->context_animation = 0x8f;
        object->context_animation_timer = AnimDuration(object->id, 0x8f, 0.0f, 0.0f, 1);
    } else {
        object->context_animation = 0x88;
    }
    object->tightrope_position = point;
    i32 difference =
        RotDiff(static_cast<TIGHTROPE *>(object->field_0x788)->angle, object->apiobj.movement_facing_angle);
    if (difference < 0) {
        difference = -difference;
    }
    u16 angle = static_cast<TIGHTROPE *>(object->field_0x788)->angle;
    if (difference > 0x4000) {
        angle += 0x8000;
    }
    object->apiobj.movement_facing_angle = angle;
    object->field_0x768 =
        NuVecXZDist(&object->apiobj.collision_position, &static_cast<TIGHTROPE *>(object->field_0x788)->start, NULL);
    rope = static_cast<TIGHTROPE *>(object->field_0x788);
    f32 horizontal = NuFsqrt(rope->direction.x * rope->direction.x + rope->direction.z * rope->direction.z);
    NuVecRotateX(&point, &v010, -NuAtan2D(static_cast<TIGHTROPE *>(object->field_0x788)->direction.y, horizontal));
    NuVecRotateY(&point, &point, static_cast<TIGHTROPE *>(object->field_0x788)->angle);
    FindAnglesZX(&point, &object->tightrope_x_rotation, &object->tightrope_z_rotation);
    if ((static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_090 & 0x10000000) != 0) {
        point.x = 0.0f;
        point.y = object->apiobj.upper_position.y - object->apiobj.lower_position.y;
        point.z = 0.0f;
        NuVecRotateZ(&object->tightrope_offset, &point, object->tightrope_z_rotation);
        NuVecRotateX(&object->tightrope_offset, &object->tightrope_offset, object->tightrope_x_rotation);
        NuVecSub(&object->tightrope_offset, &point, &object->tightrope_offset);
    } else {
        object->tightrope_offset = v000;
    }
    return 1;
}

i32 TightRope_SnapTo(GameObject_s *object, NUVEC *position) {
    object->apiobj.lower_position = *position;
    object->apiobj.upper_position = object->apiobj.lower_position;
    object->apiobj.upper_position.y += object->apiobj.field_0x1e0;
    object->apiobj.collision_position = object->apiobj.lower_position;
    object->apiobj.lower_position.y -= object->apiobj.field_0x1e0;
    if (TightRope_Attach(object, WORLD) == 0) {
        return 0;
    }
    object->apiobj.position.x = object->apiobj.collision_position.x;
    object->apiobj.position.z = object->apiobj.collision_position.z;
    if ((static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_090 & 0x80000) != 0) {
        object->apiobj.position.y = object->tightrope_position.y - object->field_0xffc * object->apiobj.field_0xa8;
    } else {
        object->apiobj.position.y = object->tightrope_position.y - object->field_0x1000 * object->apiobj.field_0xa8;
    }
    u8 origin_flag = (object->field_0xe24 >> 3) & 1;
    object->field_0xe24 &= ~8;
    GameObjectOrigin(object);
    object->field_0xe24 = (object->field_0xe24 & ~8) | (origin_flag << 3);
    return 1;
}

static i32 TightRopes_GetMaxGizmos(void *world_info) {
    WORLDINFO *world = (WORLDINFO *)world_info;
    if (world == NULL) {
        return 0;
    }

    return world->current_level->max_tightropes;
}

static void TightRopes_AddGizmos(GIZMOSYS *gizmo_sys, i32 type_id, void *world_info, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_info);
    for (i32 i = 0; i < world->tightrope_count; ++i) {
        if (NuStrLen(world->tightropes[i].name) != 0) {
            AddGizmo(gizmo_sys, type_id, NULL, &world->tightropes[i]);
        }
    }
}

static void TightRopes_Draw(void *, void *, float) {
    UNIMPLEMENTED();
}

static char *TightRope_GetGizmoName(GIZMO *gizmo) {
    return gizmo != NULL ? static_cast<char *>(gizmo->object) : NULL;
}

static i32 TightRope_GetOutput(GIZMO *gizmo, i32, i32) {
    TIGHTROPE *rope = static_cast<TIGHTROPE *>(gizmo->object);
    return rope->visible != 0 && rope->active != 0;
}

static char *TightRope_GetOutputName(GIZMO *gizmo, i32 output_index) {
    UNIMPLEMENTED();
    return {};
}

static i32 TightRope_GetNumOutputs(GIZMO *gizmo) {
    return 1;
}

static void TightRope_Activate(GIZMO *gizmo, i32 active) {
    if (gizmo != NULL) {
        static_cast<TIGHTROPE *>(gizmo->object)->active = active != 0;
    }
}

static void TightRope_SetVisibility(GIZMO *gizmo, i32 visible) {
    if (gizmo != NULL) {
        static_cast<TIGHTROPE *>(gizmo->object)->visible = visible != 0;
    }
}

static void *TightRopes_AllocateProgressData(VARIPTR *buffer, VARIPTR *end) {
    return GizmoBufferAlloc(buffer, end, sizeof(TIGHTROPEPROGRESS));
}

static void TightRopes_ClearProgress(void *, void *progress_data) {
    TIGHTROPEPROGRESS *progress = (TIGHTROPEPROGRESS *)progress_data;
    if (progress == NULL) {
        return;
    }

    progress->state[0] = -1;
    progress->state[1] = -1;
}

static void TightRopes_StoreProgress(void *world_info, void *, void *progress_data) {
    TIGHTROPEPROGRESS *progress = static_cast<TIGHTROPEPROGRESS *>(progress_data);
    if (progress == NULL)
        return;
    progress->state[0] = -1;
    progress->state[1] = -1;
    WORLDINFO *world = static_cast<WORLDINFO *>(world_info);
    if (world == NULL || world->tightropes == NULL)
        return;
    for (i32 i = 0; i < world->tightrope_count && i < 32; ++i) {
        u32 mask = 1u << i;
        if (!world->tightropes[i].visible)
            progress->state[1] &= ~mask;
        if (!world->tightropes[i].active)
            progress->state[0] &= ~mask;
    }
}

static void TightRopes_Reset(void *world_info, void *, void *progress_data) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_info);
    if (world == NULL || world->tightropes == NULL)
        return;
    TIGHTROPEPROGRESS *progress = static_cast<TIGHTROPEPROGRESS *>(progress_data);
    TIGHTROPE *rope = world->tightropes;
    for (i32 i = 0; i < world->tightrope_count; ++i, ++rope) {
        NuVecSub(&rope->direction, &rope->end_position, &rope->start_position);
        rope->y_rotation = NuAtan2D(rope->direction.x, rope->direction.z);
        rope->length = NuVecMag(&rope->direction);
        NuVecScale(&rope->direction, &rope->direction, 1.0f / rope->length);
        rope->visible = 1;
        rope->active = 1;
        if (i < 32 && progress != NULL) {
            u32 mask = 1u << i;
            rope->visible = (progress->state[1] & mask) != 0;
            rope->active = (progress->state[0] & mask) != 0;
        }
    }
}

static void *TightRopes_ReserveBufferSpace(void *world_info) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_info);
    world->tightropes = NULL;
    world->tightrope_count = 0;
    if (world->current_level->max_tightropes != 0) {
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
        world->tightropes = reinterpret_cast<TIGHTROPE *>(world->giz_buffer.addr);
        world->giz_buffer.addr += world->current_level->max_tightropes * sizeof(TIGHTROPE);
    }
    return world->tightropes;
}

static i32 TightRopes_Load(void *world_info, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_info);
    if (world->tightrope_count != 0)
        return 0;
    i32 version = EdFileReadInt();
    world->tightrope_count = EdFileReadInt();
    for (i32 i = 0; i < world->tightrope_count; ++i) {
        EdFileRead(world->tightropes[i].name, 16);
        EdFileReadNuVec(&world->tightropes[i].start_position);
        EdFileReadNuVec(&world->tightropes[i].end_position);
        if (version > 1) {
            world->tightropes[i].field_0x28 = EdFileReadUnsignedShort();
            world->tightropes[i].field_0x2a = EdFileReadUnsignedShort();
            world->tightropes[i].field_0x2c = EdFileReadUnsignedShort();
            world->tightropes[i].field_0x2e = EdFileReadUnsignedShort();
            world->tightropes[i].field_0x30 = EdFileReadUnsignedChar();
            world->tightropes[i].field_0x31 = EdFileReadUnsignedChar();
            if (version != 2)
                world->tightropes[i].field_0x32 = EdFileReadChar();
        }
    }
    return 1;
}

ADDGIZMOTYPE *TightRopes_RegisterGizmo(i32 type_id) {
    static ADDGIZMOTYPE addtype;

    addtype = Default_ADDGIZMOTYPE;
    addtype.name = "TightRope";
    addtype.prefix = "";
    addtype.fns.unknown1 = 8;
    addtype.fns.early_update_fn = NULL;
    addtype.fns.panel_draw_fn = NULL;
    addtype.fns.get_visibility_fn = NULL;
    addtype.fns.get_max_gizmos_fn = TightRopes_GetMaxGizmos;
    addtype.fns.get_pos_fn = NULL;
    addtype.fns.using_special_fn = NULL;
    addtype.fns.add_gizmos_fn = TightRopes_AddGizmos;
    addtype.fns.bolt_hit_plat_fn = NULL;
    addtype.fns.get_best_bolt_target_fn = NULL;
    addtype.fns.late_update_fn = NULL;
    addtype.fns.bolt_hit_fn = NULL;
    addtype.fns.draw_fn = TightRopes_Draw;
    addtype.fns.get_gizmo_name_fn = TightRope_GetGizmoName;
    addtype.fns.get_output_fn = TightRope_GetOutput;
    addtype.fns.get_output_name_fn = TightRope_GetOutputName;
    addtype.fns.get_num_outputs_fn = TightRope_GetNumOutputs;
    addtype.fns.activate_fn = TightRope_Activate;
    addtype.fns.activate_rev_fn = NULL;
    addtype.fns.set_visibility_fn = TightRope_SetVisibility;
    addtype.fns.allocate_progress_data_fn = TightRopes_AllocateProgressData;
    addtype.fns.clear_progress_fn = TightRopes_ClearProgress;
    addtype.fns.store_progress_fn = TightRopes_StoreProgress;
    addtype.fns.reset_fn = TightRopes_Reset;
    addtype.fns.reserve_buffer_space_fn = TightRopes_ReserveBufferSpace;
    addtype.fns.load_fn = TightRopes_Load;
    addtype.fns.post_load_fn = NULL;
    addtype.fns.add_level_sfx_fn = NULL;

    return &addtype;
}
