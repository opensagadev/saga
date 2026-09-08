#include "legoapi/gizmos/transport/ledges.h"

#include "decomp.h"
#include "gameapi/edtools/edfile.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nutrig.h"
#include "globals.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/gamepads.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/legoapi_types.h"
void StartJump(GameObject_s *, i32);

#include "legoapi/world/level.h"
#include "legoapi/world/world.h"

struct LEDGEPROGRESS {
    i32 state[8];
};

struct LEDGEPIECE {
    i16 field_0x0;
    i8 type_code;
    u8 field_0x3;
    NUVEC start;
    NUVEC end;
    u16 field_0x1c;
    u16 field_0x1e;
};

static LEDGEPIECE LedgePiece[6] = {
    {79, '8', 1, {-0.3048f, 0.0f, 0.0f}, {0.3048f, 0.0f, 0.0f}, 0, 0},
    {75, '4', 1, {-0.1524f, 0.0f, 0.0f}, {0.1524f, 0.0f, 0.0f}, 0, 0},
    {76, '2', 1, {-0.0762f, 0.0f, 0.0f}, {0.0762f, 0.0f, 0.0f}, 0, 0},
    {77, 'i', 0, {-0.1524f, 0.0f, 0.0f}, {0.0f, 0.0f, -0.1524f}, 0x4000, 0x2000},
    {78, 'o', 0, {-0.1524f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.1524f}, 0xc000, 0x6000},
    {80, 'e', 1, {-0.03048f, 0.0f, 0.0f}, {0.03048f, 0.0f, 0.0f}, 0, 0},
};
DECOMP_ASSERT(sizeof(LEDGEPIECE) == 0x20, "LEDGEPIECE ABI");

static LEDGE *Ledge_AttachPoint(WORLDINFO_s *world, NUVEC *position, NUVEC *bounds_min, NUVEC *bounds_max, u16 *angle) {
    LEDGE *ledge = static_cast<LEDGE *>(world->ledges);
    if (ledge == NULL)
        return NULL;
    LEDGE *nearest_segment = NULL;
    LEDGE *nearest_endpoint = NULL;
    f32 segment_distance = 100000.0f;
    f32 endpoint_distance = 100000.0f;
    NUVEC segment_position, endpoint_position;
    u16 segment_angle = 0, endpoint_angle = 0;
    for (i32 i = 0; i < world->ledge_count; ++i, ++ledge) {
        if ((ledge->state_flags & 3) != 3 || ((ledge->flags & 1) && ShadowMode == 0))
            continue;
        if (bounds_min->x > ledge->bounds_max.x || ledge->bounds_min.x > bounds_max->x ||
            bounds_min->z > ledge->bounds_max.z || ledge->bounds_min.z > bounds_max->z ||
            bounds_min->y > ledge->bounds_max.y || ledge->bounds_min.y > bounds_max->y)
            continue;
        LEDGEPIECE *piece = &LedgePiece[ledge->type_index];
        NUVEC local = {position->x - ledge->position.x, 0.0f, position->z - ledge->position.z};
        NuVecRotateY(&local, &local, -ledge->y_rotation);
        bool check_start = false;
        bool check_end = false;
        if (piece->field_0x3 != 0) {
            if (local.x >= piece->start.x && local.x <= piece->end.x) {
                f32 distance = fabsf(local.z);
                if (distance < segment_distance) {
                    local.z = 0.0f;
                    NuVecRotateY(&local, &local, ledge->y_rotation);
                    NuVecAdd(&segment_position, &local, &ledge->position);
                    nearest_segment = ledge;
                    segment_distance = distance;
                    segment_angle = ledge->y_rotation;
                }
                continue;
            }
            check_start = local.x < piece->start.x;
            check_end = !check_start;
        } else {
            u16 local_angle = NuAtan2D(local.x - piece->start.x, local.z - piece->end.z);
            if (static_cast<u32>(RotDiff(piece->field_0x1e, local_angle) + 0x2000) <= 0x4000) {
                f32 radius = fabsf(piece->start.x);
                NUVEC point = {radius * NU_SIN_LUT(local_angle) + piece->start.x, 0.0f,
                               radius * NU_COS_LUT(local_angle) + piece->end.z};
                f32 distance = NuVecDist(&local, &point, NULL);
                if (distance < segment_distance) {
                    NuVecRotateY(&local, &point, ledge->y_rotation);
                    NuVecAdd(&segment_position, &local, &ledge->position);
                    nearest_segment = ledge;
                    segment_distance = distance;
                    segment_angle = local_angle + ledge->y_rotation;
                    if (piece->field_0x1c > 0x8000)
                        segment_angle += 0x8000;
                }
                continue;
            }
            check_start = check_end = true;
        }
        if (check_start) {
            f32 distance = NuVecDistSqr(&local, &piece->start, NULL);
            if (distance < endpoint_distance) {
                NuVecRotateY(&local, &piece->start, ledge->y_rotation);
                NuVecAdd(&endpoint_position, &local, &ledge->position);
                nearest_endpoint = ledge;
                endpoint_distance = distance;
                endpoint_angle = ledge->y_rotation;
            }
        }
        if (check_end) {
            f32 distance = NuVecDistSqr(&local, &piece->end, NULL);
            if (distance < endpoint_distance) {
                NuVecRotateY(&local, &piece->end, ledge->y_rotation);
                NuVecAdd(&endpoint_position, &local, &ledge->position);
                nearest_endpoint = ledge;
                endpoint_distance = distance;
                endpoint_angle = ledge->y_rotation;
                if (piece->field_0x3 == 0)
                    endpoint_angle += piece->field_0x1c;
            }
        }
    }
    if (nearest_segment != NULL) {
        *position = segment_position;
        *angle = segment_angle;
        return nearest_segment;
    }
    if (nearest_endpoint != NULL) {
        *position = endpoint_position;
        *angle = endpoint_angle;
        return nearest_endpoint;
    }
    return NULL;
}

static i32 Ledges_GetMaxGizmos(void *world_info) {
    WORLDINFO *world = (WORLDINFO *)world_info;
    if (world == NULL) {
        return 0;
    }

    return world->current_level->max_ledges;
}

static void Ledges_AddGizmos(GIZMOSYS *gizmo_sys, i32 type_id, void *world_info, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_info);
    for (i32 i = 0; i < world->ledge_count; ++i) {
        if (NuStrLen(static_cast<LEDGE *>(world->ledges)[i].name) != 0) {
            AddGizmo(gizmo_sys, type_id, NULL, &static_cast<LEDGE *>(world->ledges)[i]);
        }
    }
}

static void Ledges_Draw(void *world_info, void *, float) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_info);
    LEDGE *ledge = static_cast<LEDGE *>(world->ledges);
    if (ledge == NULL)
        return;
    for (i32 i = 0; i < world->ledge_count; ++i, ++ledge) {
        if (!(ledge->state_flags & 2))
            continue;
        if (world->lev_objs[LedgePiece[ledge->type_index].field_0x0].active == 0)
            continue;
        NUMTX matrix;
        NuMtxSetRotationY(&matrix, ledge->y_rotation);
        NuMtxTranslate(&matrix, &ledge->position);
        NuSpecialDrawAt(&world->lev_objs[LedgePiece[ledge->type_index].field_0x0].special, &matrix);
    }
}

static char *Ledge_GetGizmoName(GIZMO *gizmo) {
    return gizmo != NULL ? static_cast<char *>(gizmo->object) : NULL;
}

static i32 Ledge_GetOutput(GIZMO *gizmo, i32, i32) {
    return (static_cast<LEDGE *>(gizmo->object)->state_flags & 3) == 3;
}

static char *Ledge_GetOutputName(GIZMO *gizmo, i32 output_index) {
    return const_cast<char *>("CanUse");
}

static i32 Ledge_GetNumOutputs(GIZMO *gizmo) {
    return 1;
}

static void Ledge_Activate(GIZMO *gizmo, i32 active) {
    if (gizmo != NULL) {
        LEDGE *ledge = static_cast<LEDGE *>(gizmo->object);
        ledge->state_flags = (ledge->state_flags & ~1) | (active != 0);
    }
}

static void Ledge_SetVisibility(GIZMO *gizmo, i32 visible) {
    if (gizmo != NULL) {
        LEDGE *ledge = static_cast<LEDGE *>(gizmo->object);
        ledge->state_flags = (ledge->state_flags & ~2) | ((visible != 0) << 1);
    }
}

static void *Ledges_AllocateProgressData(VARIPTR *buffer, VARIPTR *end) {
    return GizmoBufferAlloc(buffer, end, sizeof(LEDGEPROGRESS));
}

static void Ledges_ClearProgress(void *, void *progress_data) {
    LEDGEPROGRESS *progress = (LEDGEPROGRESS *)progress_data;
    if (progress == NULL) {
        return;
    }

    for (i32 i = 0; i < 8; i++) {
        progress->state[i] = -1;
    }
}

static void Ledges_StoreProgress(void *world_info, void *, void *progress_data) {
    LEDGEPROGRESS *progress = static_cast<LEDGEPROGRESS *>(progress_data);
    if (progress == NULL)
        return;
    for (i32 i = 0; i < 8; ++i)
        progress->state[i] = -1;
    WORLDINFO *world = static_cast<WORLDINFO *>(world_info);
    if (world == NULL || world->ledges == NULL)
        return;
    LEDGE *ledge = static_cast<LEDGE *>(world->ledges);
    for (i32 i = 0; i < world->ledge_count && i < 128; ++i, ++ledge) {
        u32 mask = 1u << (i & 31);
        if (!(ledge->state_flags & 2))
            progress->state[4 + (i >> 5)] &= ~mask;
        if (!(ledge->state_flags & 1))
            progress->state[i >> 5] &= ~mask;
    }
}

static void Ledges_Reset(void *world_info, void *, void *progress_data) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_info);
    if (world == NULL || world->ledges == NULL)
        return;
    LEDGEPROGRESS *progress = static_cast<LEDGEPROGRESS *>(progress_data);
    LEDGE *ledge = static_cast<LEDGE *>(world->ledges);
    for (i32 i = 0; i < world->ledge_count; ++i, ++ledge) {
        i32 type;
        for (type = 0; type < 6; ++type) {
            if (ledge->type_code == LedgePiece[type].type_code)
                break;
        }
        if (type == 6) {
            type = 2;
            ledge->type_code = LedgePiece[2].type_code;
        }
        ledge->type_index = type;
        ledge->bounds_min.x = ledge->bounds_min.y = ledge->bounds_min.z = 999.0f;
        ledge->bounds_max.x = ledge->bounds_max.y = ledge->bounds_max.z = -999.0f;
        NUVEC point;
        NuVecRotateY(&point, &LedgePiece[ledge->type_index].start, ledge->y_rotation);
        if (point.x < ledge->bounds_min.x)
            ledge->bounds_min.x = point.x;
        if (point.x > ledge->bounds_max.x)
            ledge->bounds_max.x = point.x;
        if (point.y < ledge->bounds_min.y)
            ledge->bounds_min.y = point.y;
        if (point.y > ledge->bounds_max.y)
            ledge->bounds_max.y = point.y;
        if (point.z < ledge->bounds_min.z)
            ledge->bounds_min.z = point.z;
        if (point.z > ledge->bounds_max.z)
            ledge->bounds_max.z = point.z;
        NuVecRotateY(&point, &LedgePiece[ledge->type_index].end, ledge->y_rotation);
        if (point.x < ledge->bounds_min.x)
            ledge->bounds_min.x = point.x;
        if (point.x > ledge->bounds_max.x)
            ledge->bounds_max.x = point.x;
        if (point.y < ledge->bounds_min.y)
            ledge->bounds_min.y = point.y;
        if (point.y > ledge->bounds_max.y)
            ledge->bounds_max.y = point.y;
        if (point.z < ledge->bounds_min.z)
            ledge->bounds_min.z = point.z;
        if (point.z > ledge->bounds_max.z)
            ledge->bounds_max.z = point.z;
        ledge->bounds_min.x -= 0.05f;
        ledge->bounds_min.y -= 0.05f;
        ledge->bounds_min.z -= 0.05f;
        ledge->bounds_max.x += 0.05f;
        ledge->bounds_max.y += 0.05f;
        ledge->bounds_max.z += 0.05f;
        NuVecAdd(&ledge->bounds_min, &ledge->bounds_min, &ledge->position);
        NuVecAdd(&ledge->bounds_max, &ledge->bounds_max, &ledge->position);
        ledge->state_flags |= 3;
        if (i < 128 && progress != NULL) {
            u32 mask = 1u << (i & 31);
            ledge->state_flags = (ledge->state_flags & ~2) | ((progress->state[4 + (i >> 5)] & mask) != 0 ? 2 : 0);
            ledge->state_flags = (ledge->state_flags & ~1) | ((progress->state[i >> 5] & mask) != 0 ? 1 : 0);
        }
    }
}

static void *Ledges_ReserveBufferSpace(void *world_info) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_info);
    world->ledges = NULL;
    world->ledge_count = 0;
    if (world->current_level->max_ledges != 0) {
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
        world->ledges = reinterpret_cast<LEDGE *>(world->giz_buffer.addr);
        world->giz_buffer.addr += world->current_level->max_ledges * sizeof(LEDGE);
    }
    return world->ledges;
}

static i32 Ledges_Load(void *world_info, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_info);
    if (world->ledge_count != 0)
        return 0;
    i32 version = EdFileReadInt();
    world->ledge_count = EdFileReadInt();
    for (i32 i = 0; i < world->ledge_count; ++i) {
        LEDGE *ledge = &static_cast<LEDGE *>(world->ledges)[i];
        EdFileRead(ledge->name, 8);
        EdFileReadNuVec(&ledge->position);
        ledge->y_rotation = EdFileReadShort();
        ledge->type_code = EdFileReadChar();
        if (version > 1) {
            ledge->field_0x1c = EdFileReadShort();
            ledge->field_0x1e = EdFileReadShort();
            ledge->flags = version == 2 ? 0 : EdFileReadUnsignedChar();
        } else {
            ledge->field_0x1c = -1;
            ledge->field_0x1e = -1;
            ledge->flags = 0;
        }
    }
    return 1;
}

ADDGIZMOTYPE *Ledges_RegisterGizmo(i32 type_id) {
    static ADDGIZMOTYPE addtype;

    addtype = Default_ADDGIZMOTYPE;
    addtype.name = "Ledge";
    addtype.prefix = "";
    addtype.fns.unknown1 = 0x20;
    addtype.fns.early_update_fn = NULL;
    addtype.fns.panel_draw_fn = NULL;
    addtype.fns.get_visibility_fn = NULL;
    addtype.fns.get_max_gizmos_fn = Ledges_GetMaxGizmos;
    addtype.fns.get_pos_fn = NULL;
    addtype.fns.using_special_fn = NULL;
    addtype.fns.add_gizmos_fn = Ledges_AddGizmos;
    addtype.fns.bolt_hit_plat_fn = NULL;
    addtype.fns.get_best_bolt_target_fn = NULL;
    addtype.fns.late_update_fn = NULL;
    addtype.fns.bolt_hit_fn = NULL;
    addtype.fns.draw_fn = Ledges_Draw;
    addtype.fns.get_gizmo_name_fn = Ledge_GetGizmoName;
    addtype.fns.get_output_fn = Ledge_GetOutput;
    addtype.fns.get_output_name_fn = Ledge_GetOutputName;
    addtype.fns.get_num_outputs_fn = Ledge_GetNumOutputs;
    addtype.fns.activate_fn = Ledge_Activate;
    addtype.fns.activate_rev_fn = NULL;
    addtype.fns.set_visibility_fn = Ledge_SetVisibility;
    addtype.fns.allocate_progress_data_fn = Ledges_AllocateProgressData;
    addtype.fns.clear_progress_fn = Ledges_ClearProgress;
    addtype.fns.store_progress_fn = Ledges_StoreProgress;
    addtype.fns.reset_fn = Ledges_Reset;
    addtype.fns.reserve_buffer_space_fn = Ledges_ReserveBufferSpace;
    addtype.fns.load_fn = Ledges_Load;
    addtype.fns.post_load_fn = NULL;
    addtype.fns.add_level_sfx_fn = NULL;

    return &addtype;
}

void Ledge_MoveCode(WORLDINFO_s *world, GameObject_s *object) {
    if (object->character_context != 0x5b) {
        if (object->apiobj.field_0x27d != 0 || !(object->apiobj.velocity.y <= 0.0f))
            return;
        if (object->character_context != 0x43 && object->character_context != -1) {
            if (object->character_context != 0 || !(object->context_animation_timer >= 0.1f))
                return;
        }
        if (!(object->apiobj.field_0x1f8 & 0x80) && !(object->field_0xf01 & 0x80))
            return;
        f32 radius = 3.0f * object->apiobj.field_0x1dc;
        NUVEC minimum = {object->apiobj.collision_position.x - radius, object->apiobj.collision_position.y,
                         object->apiobj.collision_position.z - radius};
        NUVEC maximum = {object->apiobj.collision_position.x + radius, object->apiobj.upper_position.y,
                         object->apiobj.collision_position.z + radius};
        NUVEC position = {(maximum.x + minimum.x) * 0.5f, (maximum.y + minimum.y) * 0.5f,
                          (maximum.z + minimum.z) * 0.5f};
        u16 angle;
        if (Ledge_AttachPoint(world, &position, &minimum, &maximum, &angle) == NULL)
            return;
        object->character_context = 0x5b;
        object->context_animation = 0x9d;
        object->apiobj.velocity = v000;
        object->external_force = position;
        object->apiobj.movement_facing_angle = angle + 0x8000;
        object->launch_origin.x =
            object->external_force.x - NU_SIN_LUT(object->apiobj.movement_facing_angle) * object->apiobj.field_0x1dc;
        object->launch_origin.y = object->external_force.y;
        object->launch_origin.z =
            object->external_force.z - NU_COS_LUT(object->apiobj.movement_facing_angle) * object->apiobj.field_0x1dc;
        return;
    }
    if (object->pad_gamepad->buttons_pressed & GAMEPAD_JUMP) {
        StartJump(object, 0);
        object->movement_runtime_flags |= 0x10;
        f32 height = 0.2f + object->external_force.y - object->jump_start_height;
        if (height > 0.0f) {
            object->apiobj.velocity.y =
                NuFsqrt(-2.0f * object->apiobj.character_data->game_character->gravity * height);
        }
        return;
    }
    if (object->apiobj.field_0x27d & 2) {
        object->character_context = -1;
        return;
    }
    if (!(object->pad_gamepad->input_magnitude > 0.0f)) {
        object->context_animation = 0x9d;
        return;
    }
    u16 input = GamePad_InputAngle(object, object->pad_gamepad);
    u16 angle = object->apiobj.movement_facing_angle + 0x4000;
    f32 push = PushingTowardsAngle(input, angle);
    f32 speed;
    if (push > NuTrigTable[0x3555]) {
        if (object->apiobj.character_model->model_data_b[0x9f] != NULL) {
            object->context_animation = 0x9f;
            speed = AnimSpeed(object->apiobj.character_model, 0x9f);
            if (speed == 0.0f)
                return;
        } else
            speed = 0.5f;
    } else if (push < -NuTrigTable[0x3555]) {
        if (object->apiobj.character_model->model_data_b[0x9e] != NULL) {
            object->context_animation = 0x9e;
            speed = -AnimSpeed(object->apiobj.character_model, 0x9e);
            if (speed == 0.0f)
                return;
        } else
            speed = -0.5f;
    } else {
        object->context_animation = 0x9d;
        return;
    }
    NUVEC previous = object->external_force;
    f32 radius = object->apiobj.field_0x1dc;
    NUVEC minimum = {object->apiobj.collision_position.x - radius, object->apiobj.collision_position.y,
                     object->apiobj.collision_position.z - radius};
    NUVEC maximum = {object->apiobj.collision_position.x + radius, object->apiobj.upper_position.y,
                     object->apiobj.collision_position.z + radius};
    f32 distance = speed * FRAMETIME;
    NUVEC offset = {NU_SIN_LUT(angle) * distance, 0.0f, NU_COS_LUT(angle) * distance};
    NUVEC position;
    NuVecAdd(&position, &object->external_force, &offset);
    if (Ledge_AttachPoint(world, &position, &minimum, &maximum, &angle) == NULL) {
        object->context_animation = 0x9d;
        return;
    }
    object->external_force = position;
    object->apiobj.movement_facing_angle = angle + 0x8000;
    object->launch_origin.x =
        object->external_force.x - NU_SIN_LUT(object->apiobj.movement_facing_angle) * object->apiobj.field_0x1dc;
    object->launch_origin.y = object->external_force.y;
    object->launch_origin.z =
        object->external_force.z - NU_COS_LUT(object->apiobj.movement_facing_angle) * object->apiobj.field_0x1dc;
    if (previous.x == object->external_force.x && previous.y == object->external_force.y &&
        previous.z == object->external_force.z)
        object->context_animation = 0x9d;
}
