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
#include "legoapi/items/base/apiobject.h"
#include "legoapi/characters/core/character.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "globals.h"
#include <math.h>

DECOMP_ASSERT(offsetof(WORLDINFO, tightropes) == 0x505c, "World tightrope array offset");
DECOMP_ASSERT(offsetof(WORLDINFO, tightrope_count) == 0x5060, "World tightrope count offset");

extern "C" f32 AnimDuration(i32, i32, f32, f32, i32);
extern void FindAnglesZX(NUVEC *, u16 *, u16 *);
extern void GameObjectOrigin(GameObject_s *);

extern f32 PushingTowardsAngle(u16, u16);
extern void StartJump(GameObject_s *, i32);
extern void StartEndOfJump(GameObject_s *);
extern i32 StartFallLand(GameObject_s *, i32);

extern "C" f32 AnimDuration(i32, i32, f32, f32, i32);
void FindAnglesZX(NUVEC *, u16 *, u16 *);
void GameObjectOrigin(GameObject_s *);

struct TIGHTROPEPROGRESS {
    i32 state[2];
};

TIGHTROPE *TightRope_InRange(GameObject_s *object, WORLDINFO_s *world, NUVEC *position) {
    TIGHTROPE *rope = world->tightropes;
    f32 width = 3.0f * object->apiobj.field_0x1dc;
    NUVEC origin = object->apiobj.collision_position;
    for (i32 index = 0; index < world->tightrope_count; ++index, ++rope) {
        if (rope->enabled == 0 || rope->available == 0) {
            continue;
        }
        NUVEC offset;
        offset.x = origin.x - rope->start.x;
        offset.y = 0.0f;
        offset.z = origin.z - rope->start.z;
        NuVecRotateY(&offset, &offset, -rope->rotation);
        if (!(offset.z >= 0.0f && rope->horizontal_length >= offset.z && offset.x >= -width && width >= offset.x)) {
            continue;
        }
        offset.x = 0.0f;
        offset.y = (rope->end.y - rope->start.y) * (offset.z / rope->horizontal_length) + rope->start.y;
        if (!(object->apiobj.field_0x1e0 > fabsf(offset.y - origin.y))) {
            continue;
        }
        if (position != NULL) {
            f32 margin = (object->apiobj.character_data->game_character->flags_090 & 0x10000000) != 0
                             ? object->apiobj.field_0x1e0
                             : object->apiobj.field_0x1dc;
            if (offset.z > rope->horizontal_length - margin) {
                offset.z = rope->horizontal_length - margin;
            } else if (margin > offset.z) {
                offset.z = margin;
            }
            NuVecRotateY(position, &offset, rope->rotation);
            position->x += rope->start.x;
            position->z += rope->start.z;
        }
        return rope;
    }
    return NULL;
}

static i32 TightRope_Attach(GameObject_s *object, WORLDINFO_s *world) {
    NUVEC position;
    TIGHTROPE *rope = TightRope_InRange(object, world, &position);
    if (rope == NULL) {
        return 0;
    }
    object->character_context = 0x44;
    object->field_0x788 = rope;
    object->context_variant_flags = (object->context_variant_flags & ~8) |
                                    ((((object->apiobj.character_data->game_character->flags_090 >> 19) ^ 1) & 1) << 3);
    if (object->apiobj.character_model->model_data_b[0x8f] != NULL &&
        ((object->apiobj.character_data->game_character->flags_090 & 0x80000) == 0 ||
         position.y > (object->apiobj.upper_position.y - object->apiobj.lower_position.y) * 0.25f +
                          object->apiobj.lower_position.y)) {
        object->context_animation = 0x8f;
        object->context_animation_timer = AnimDuration(object->id, 0x8f, 0.0f, 0.0f, 1);
    } else {
        object->context_animation = 0x88;
    }
    object->context_destination = position;
    rope = static_cast<TIGHTROPE *>(object->field_0x788);
    i32 difference = RotDiff(rope->rotation, object->apiobj.movement_facing_angle);
    if (difference < 0) {
        difference = -difference;
    }
    rope = static_cast<TIGHTROPE *>(object->field_0x788);
    u16 rotation = rope->rotation;
    if (difference > 0x4000) {
        rotation += 0x8000;
    }
    object->apiobj.movement_facing_angle = rotation;
    object->field_0x768 = NuVecXZDist(&object->apiobj.collision_position, &rope->start, NULL);
    rope = static_cast<TIGHTROPE *>(object->field_0x788);
    f32 horizontal_length = NuFsqrt(rope->direction.x * rope->direction.x + rope->direction.z * rope->direction.z);
    rope = static_cast<TIGHTROPE *>(object->field_0x788);
    i32 slope = NuAtan2D(rope->direction.y, horizontal_length);
    NuVecRotateX(&position, &v010, -slope);
    rope = static_cast<TIGHTROPE *>(object->field_0x788);
    NuVecRotateY(&position, &position, rope->rotation);
    FindAnglesZX(&position, &object->context_x_rotation, &object->context_z_rotation);
    if ((object->apiobj.character_data->game_character->flags_090 & 0x10000000) != 0) {
        position.x = 0.0f;
        position.y = object->apiobj.upper_position.y - object->apiobj.lower_position.y;
        position.z = 0.0f;
        NuVecRotateZ(&object->context_position_offset, &position, object->context_z_rotation);
        NuVecRotateX(&object->context_position_offset, &object->context_position_offset, object->context_x_rotation);
        NuVecSub(&object->context_position_offset, &position, &object->context_position_offset);
    } else {
        object->context_position_offset = v000;
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

static i32 TightRope_MoveUpdate(GameObject_s *object, i32 jumping) {
    if (!(object->pad_gamepad->input_magnitude > 0.0f) || object->context_animation == 0x8f) {
        if (jumping == 0) {
            object->context_animation = 0x88;
        }
        return 1;
    }
    NUVEC previous = object->context_destination;
    TIGHTROPE *rope = static_cast<TIGHTROPE *>(object->field_0x788);
    u16 rotation = rope->rotation;
    f32 input = PushingTowardsAngle(GamePad_InputAngle(object, object->pad_gamepad), rotation);
    f32 direction;
    f32 speed;
    if (input > NuTrigTable[0x3000]) {
        if (jumping != 0) {
            speed = 0.6f;
        } else {
            rope = static_cast<TIGHTROPE *>(object->field_0x788);
            object->context_animation = 0x89;
            object->apiobj.movement_facing_angle = rope->rotation;
            speed = AnimSpeed(object->apiobj.character_model, 0x89);
        }
        direction = 1.0f;
    } else if (-NuTrigTable[0x3000] > input) {
        if (jumping != 0) {
            speed = 0.6f;
        } else {
            rope = static_cast<TIGHTROPE *>(object->field_0x788);
            object->context_animation = 0x89;
            object->apiobj.movement_facing_angle = rope->rotation + 0x8000;
            speed = AnimSpeed(object->apiobj.character_model, 0x89);
        }
        direction = -1.0f;
    } else {
        if (jumping == 0) {
            object->context_animation = 0x88;
        }
        return 1;
    }
    NUVEC movement;
    rope = static_cast<TIGHTROPE *>(object->field_0x788);
    NuVecScale(&movement, &rope->direction, speed * direction * FRAMETIME);
    NuVecAdd(&object->context_destination, &object->context_destination, &movement);
    rope = static_cast<TIGHTROPE *>(object->field_0x788);
    NuVecSub(&object->context_destination, &object->context_destination, &rope->start);
    object->context_destination.z = NuVecDot(&object->context_destination, &rope->direction);
    f32 margin = (object->apiobj.character_data->game_character->flags_090 & 0x10000000) != 0
                     ? object->apiobj.field_0x1e0
                     : object->apiobj.field_0x1dc;
    i32 result = 1;
    if (object->context_destination.z > rope->horizontal_length - margin) {
        object->context_destination.z = rope->horizontal_length - margin;
        result = 0;
    } else if (margin > object->context_destination.z) {
        object->context_destination.z = margin;
        result = 0;
    }
    NuVecScale(&object->context_destination, &rope->direction, object->context_destination.z);
    NuVecAdd(&object->context_destination, &object->context_destination, &rope->start);
    if (jumping == 0 && previous.x == object->context_destination.x && previous.y == object->context_destination.y &&
        previous.z == object->context_destination.z) {
        object->context_animation = 0x88;
    }
    return result;
}

void TightRope_MoveCode(GameObject_s *object, i32 jump_pressed) {
    if (object->character_context != 0x44) {
        if (object->apiobj.field_0x27d != 0 || !(0.0f >= object->apiobj.velocity.y) ||
            object->apiobj.character_model->model_data_b[0x88] == NULL) {
            return;
        }
        if (object->character_context != -1) {
            if (object->character_context != 0 || !(object->context_animation_timer >= 0.1f) ||
                object->action_movement_state == 3 || object->action_movement_state == 4 ||
                object->action_movement_state == 8) {
                return;
            }
        }
        if ((object->apiobj.object_flags & 0x80) == 0 && (object->field_0xf01 & 0x40) == 0) {
            return;
        }
        TightRope_Attach(object, WORLD);
        object->build_button_taps = 0;
        object->external_force.z = 0.0f;
        object->external_force.y = 0.0f;
        return;
    }
    if (object->context_animation != 0x8f) {
        object->external_force.y += FRAMETIME;
        if (static_cast<u16>(object->context_animation - 5) <= 1) {
            object->context_animation_timer += FRAMETIME;
            void *rope = object->field_0x788;
            if (object->context_animation == 5) {
                if (object->context_animation_timer >=
                    object->airborne_action_duration + object->airborne_action_duration) {
                    StartEndOfJump(object);
                }
            } else if (object->context_animation_timer >= object->airborne_action_duration) {
                object->context_animation = 5;
            }
            if (0.0f >= object->apiobj.velocity.y && TightRope_Attach(object, WORLD) == 0 &&
                object->apiobj.field_0x27d != 0) {
                object->movement_runtime_flags |= 4;
                if (object->context_animation != 5 || StartFallLand(object, -1) == 0) {
                    if (object->apiobj.character_model->model_data_b[7] != NULL &&
                        object->pad_gamepad->input_magnitude == 0.0f) {
                        object->character_context = 1;
                        object->context_animation = 7;
                        object->context_animation_timer = AnimDuration(object->id, 7, 0.0f, 0.0f, 1);
                        object->jump_reentry_timer = 0.2f;
                        ResetAnimPacket(&object->apiobj.anim_packet, -1);
                    } else {
                        object->character_context = -1;
                    }
                }
                object->movement_runtime_flags &= ~4;
            }
            if (object->character_context != 0x44 || object->field_0x788 != rope) {
                return;
            }
            TightRope_MoveUpdate(object, 1);
        } else if (jump_pressed == 0) {
            if (TightRope_MoveUpdate(object, 0) == 0 && object->context_animation != 0x8f &&
                object->external_force.y >= 0.5f) {
                StartJump(object, 0);
                object->apiobj.velocity.y =
                    (object->apiobj.character_data->game_character->flags_090 & 0x80000) != 0 ? 1.2f : 1.8f;
            }
        } else {
            goto jump;
        }
    } else {
        if (jump_pressed != 0) {
            goto jump;
        }
        f32 *playing = AnimPlaying(&object->apiobj.anim_packet, 0x8f, 1, 0);
        if (playing != NULL) {
            f32 frame = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
            if (frame >= 1.0f && *playing >= frame && object->pad_gamepad->input_magnitude > 0.0f &&
                fabsf(PushingTowardsAngle(GamePad_InputAngle(object, object->pad_gamepad),
                                          static_cast<TIGHTROPE *>(object->field_0x788)->rotation)) >
                    NuTrigTable[0x3000]) {
                TightRope_MoveUpdate(object, 0);
            } else {
                object->context_animation_timer -= FRAMETIME;
                if (0.0f >= object->context_animation_timer) {
                    object->context_animation = 0x88;
                }
            }
        }
    }
    object->field_0x768 =
        NuVecXZDist(&object->apiobj.collision_position, &static_cast<TIGHTROPE *>(object->field_0x788)->start, NULL);
    return;

jump:
    if ((object->apiobj.object_flags & 0x80) != 0) {
        object->apiobj.velocity.y = 2.0f;
        object->context_animation = 6;
        object->context_animation_timer = 0.0f;
        f32 duration = AnimDuration(object->id, 6, 0.0f, 0.0f, 0);
        object->airborne_action_duration = duration <= 0.0f ? 1.0f : duration;
        ResetAnimPacket(&object->apiobj.anim_packet, -1);
    } else {
        StartJump(object, 0);
        object->movement_runtime_flags |= 0x10;
    }
}

TIGHTROPE *TightRope_FindNearest(NUVEC *position, WORLDINFO_s *world, i32 *endpoint, f32 *distance_squared) {
    TIGHTROPE *nearest = NULL;
    i32 nearest_endpoint = -1;
    f32 nearest_distance = 1000000000.0f;
    TIGHTROPE *rope = world->tightropes;
    for (i32 index = 0; index < world->tightrope_count; ++index, ++rope) {
        f32 distance = NuVecDistSqr(position, &rope->start, NULL);
        if (nearest_distance > distance) {
            nearest_distance = distance;
            nearest = rope;
            nearest_endpoint = 0;
        }
        distance = NuVecDistSqr(position, &rope->end, NULL);
        if (nearest_distance > distance) {
            nearest_distance = distance;
            nearest = rope;
            nearest_endpoint = 1;
        }
    }
    if (endpoint != NULL) {
        *endpoint = nearest_endpoint;
    }
    if (distance_squared != NULL) {
        *distance_squared = nearest_distance;
    }
    return nearest;
}

i32 TightRope_SetTargetMom(GameObject_s *object) {
    NUVEC *anchor = (object->apiobj.character_data->game_character->flags_090 & 0x80000) != 0
                        ? &object->apiobj.lower_position
                        : &object->apiobj.upper_position;
    object->target_velocity.x = (object->context_destination.x - anchor->x) * 3.0f;
    object->target_velocity.y = (object->context_destination.y - anchor->y) * 3.0f;
    object->target_velocity.z = (object->context_destination.z - anchor->z) * 3.0f;
    TIGHTROPE *rope = static_cast<TIGHTROPE *>(object->field_0x788);
    u16 rotation = rope->rotation + 0x4000;
    f32 fraction = object->field_0x768 / rope->horizontal_length;
    if (fraction > 1.0f) {
        fraction = 1.0f;
    }
    f32 amplitude = 0.5f * NU_SIN_LUT(static_cast<i32>(fraction * 32768.0f));
    f32 direction = NU_SIN_LUT(rotation);
    object->target_velocity.x += (qrand() * (1.0f / 65535.0f) - 0.5f) * amplitude * direction;
    object->target_velocity.y -= qrand() * (1.0f / 65535.0f) * amplitude;
    direction = NU_COS_LUT(rotation);
    object->target_velocity.z += (qrand() * (1.0f / 65535.0f) - 0.5f) * amplitude * direction;
    return static_cast<u16>(object->context_animation - 5) > 1;
}
