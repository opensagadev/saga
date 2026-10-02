#include "legoapi/props/objects/signal.h"

#include "decomp.h"
#include "gameapi/edtools/edfile.h"
#include "globals.h"
#include "legoapi/actions/character/suit.h"
#include "legoapi/actions/movement/jumping.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"

#include <string.h>

extern "C" void NuTexAnimSetSignals(u32 signals);
extern "C" void NewTerrPlatformsOff(void);
void FindAnglesZX(NUVEC *normal, u16 *x_rotation, u16 *z_rotation);

static void Signal_SetVisibility(SIGNAL *signal, i32 visible);

static void *Signals_ReserveBufferSpace(void *world_info) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_info);
    world->signals = NULL;
    world->signal_count = 0;
    if (world->current_level->max_signals != 0) {
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
        world->signals = static_cast<SIGNAL *>(world->giz_buffer.void_ptr);
        world->giz_buffer.addr += world->current_level->max_signals * sizeof(SIGNAL);
    }
    return world->signals;
}

static void Signal_Activate(GIZMO *gizmo, i32 active) {
    if (gizmo != NULL) {
        static_cast<SIGNAL *>(gizmo->object)->active = active != 0;
    }
}

static i32 Signal_ActivateRev(GIZMO *gizmo, i32 active, i32 reverse) {
    if (gizmo == NULL) {
        return 0;
    }
    SIGNAL *signal = static_cast<SIGNAL *>(gizmo->object);
    if (signal == NULL) {
        return 0;
    }
    if ((reverse & 1) == 0) {
        signal->active = active == 0;
        return 1;
    }
    return signal->active == active;
}

static NUVEC *Signal_GetPos(GIZMO *gizmo) {
    SIGNAL *signal = gizmo != NULL ? static_cast<SIGNAL *>(gizmo->object) : NULL;
    return signal != NULL ? &signal->position : NULL;
}

static i32 Signals_GetMaxGizmos(void *world_info) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_info);
    return world != NULL ? world->current_level->max_signals : 0;
}

static char *Signal_GetGizmoName(GIZMO *gizmo) {
    SIGNAL *signal = gizmo != NULL ? static_cast<SIGNAL *>(gizmo->object) : NULL;
    return signal != NULL ? signal->name : NULL;
}

static i32 Signal_GetOutput(GIZMO *gizmo, i32, i32) {
    SIGNAL *signal = static_cast<SIGNAL *>(gizmo->object);
    return (signal->flags & (SIGNAL::FLAG_VISIBLE | SIGNAL::FLAG_ACTIVE)) ==
           (SIGNAL::FLAG_VISIBLE | SIGNAL::FLAG_ACTIVE);
}

static char *Signal_GetOutputName(GIZMO *, i32) {
    return "Active";
}

static i32 Signal_GetNumOutputs(GIZMO *) {
    return 1;
}

static void Signals_ClearProgress(void *, void *progress_data) {
    SIGNALPROGRESS *progress = static_cast<SIGNALPROGRESS *>(progress_data);
    if (progress != NULL) {
        memset(progress->suit_letters, 0, sizeof(progress->suit_letters));
        progress->active_mask = 0xffffffff;
        progress->visible_mask = 0xffffffff;
    }
}

static void *Signals_AllocateProgressData(VARIPTR *buffer, VARIPTR *buffer_end) {
    return GizmoBufferAlloc(buffer, buffer_end, sizeof(SIGNALPROGRESS));
}

static void Signals_Draw(void *world_info, void *, float) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_info);
    SIGNAL *signal = world->signals;
    for (i32 i = 0; i < world->signal_count; i++, signal++) {
        if (!signal->visible) {
            continue;
        }
        NUMTX matrix = signal->matrix;
        if (world->lev_objs[0x36].active) {
            NuSpecialDrawAt(&world->lev_objs[0x36].special, &matrix);
        }
        if (signal->active) {
            if (world->lev_objs[signal->animation_1].active) {
                NuSpecialDrawAt(&world->lev_objs[signal->animation_1].special, &matrix);
            }
            if (world->lev_objs[signal->animation_2].active) {
                f32 alpha = signal->radius;
                NUVEC camera_offset;
                NuVecSub(&camera_offset, &GameCam->pos, &signal->position);
                f32 along_normal = NuVecDot(&camera_offset, &signal->normal);
                f32 distance = NuVecMag(&camera_offset);
                f32 distance_squared = distance * distance - along_normal * along_normal;
                if (distance_squared < 2.25f) {
                    f32 fade;
                    if (distance_squared <= 0.5625f) {
                        fade = 0.0f;
                    } else {
                        fade = (NuFsqrt(distance_squared) - 0.75f) / 0.75f;
                    }
                    if (along_normal < 0.0f || along_normal > 2.0f) {
                        if (along_normal >= 0.0f) {
                            along_normal -= 2.0f;
                        } else {
                            along_normal = -along_normal;
                        }
                        if (along_normal < 0.75f) {
                            fade += (1.0f - fade) * (along_normal / 0.75f);
                        } else {
                            fade = 1.0f;
                        }
                    }
                    alpha *= fade;
                }
                alpha *= signal->scale;
                if (alpha > 0.0f) {
                    NuSpecialDrawAtAlpha(&world->lev_objs[signal->animation_2].special, &matrix, alpha);
                }
            }
        } else if (world->lev_objs[signal->animation_0].active) {
            NuSpecialDrawAt(&world->lev_objs[signal->animation_0].special, &matrix);
        }
    }
}

static void Signals_Update(void *world_info, void *, float) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_info);
    SIGNAL *signal = world->signals;
    for (i32 i = 0; i < world->signal_count; ++i, ++signal) {
        signal->radius = QRAND_FLOAT() * 0.1f + 0.8f;
        if ((signal->flags & (SIGNAL::FLAG_VISIBLE | SIGNAL::FLAG_ACTIVE)) !=
            (SIGNAL::FLAG_VISIBLE | SIGNAL::FLAG_ACTIVE)) {
            signal->scale = 0.0f;
            signal->flags &= ~SIGNAL::FLAG_IN_USE;
            continue;
        }

        f32 target_scale = 0.0f;
        if ((signal->flags & SIGNAL::FLAG_IN_USE) != 0) {
            i32 player_index;
            for (player_index = 0; player_index < 8; ++player_index) {
                GameObject_s *player = Player[player_index];
                if (player != NULL && (player->apiobj.field_0x1f8 & 0x1001) == 0x1001 &&
                    player->id == signal->suit->character_id &&
                    NuVecDistSqr(&player->apiobj.collision_position, &signal->target_position, NULL) <
                        (0.6f + 0.1f) * (0.6f + 0.1f)) {
                    NUVEC offset;
                    NuVecSub(&offset, &player->apiobj.collision_position, &signal->position);
                    f32 along_normal = NuVecDot(&offset, &signal->normal);
                    f32 distance = NuVecMag(&offset);
                    f32 radius = player->apiobj.field_0x1dc + 0.3f;
                    if (distance * distance - along_normal * along_normal < radius * radius) {
                        break;
                    }
                }
            }
            if (player_index == 8) {
                signal->flags &= ~SIGNAL::FLAG_IN_USE;
            }
        } else {
            ADDGAMEMSG message = AddGameMsg_Default;
            message.text = TTab[*signal->suit->text_id];
            message.position = &signal->target_position;
            message.scale = 1.0f;
            message.red = 0xff;
            message.green = 0xff;
            message.blue = 0;
            message.alpha = 0x30;
            message.flags = 0x87;
            AddGameMsg(&message);
            target_scale = 1.0f;
        }
        signal->scale = SeekLinearF(signal->scale, target_scale, 3.0f * FRAMETIME);
    }
}

static void Signals_AddGizmos(GIZMOSYS *gizmo_sys, i32 type_id, void *world_info, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_info);
    for (i32 index = 0; index < world->signal_count; ++index) {
        if (NuStrLen(world->signals[index].name) != 0) {
            AddGizmo(gizmo_sys, type_id, NULL, &world->signals[index]);
        }
    }
}

static i32 Signals_Load(void *world_info, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_info);
    if (world->signal_count != 0) {
        return 0;
    }
    i32 version = EdFileReadInt();
    world->signal_count = EdFileReadInt();
    for (i32 index = 0; index < world->signal_count; ++index) {
        EdFileRead(world->signals[index].name, sizeof(world->signals[index].name));
        EdFileReadNuVec(&world->signals[index].position);
        world->signals[index].character_letter = EdFileReadChar();
        if (version > 1) {
            world->signals[index].suit_letter = EdFileReadChar();
            world->signals[index].yaw = EdFileReadShort();
        } else {
            world->signals[index].suit_letter = 'b';
            world->signals[index].yaw = 0;
        }
    }
    return 1;
}

static void Signals_Reset(void *world_info, void *, void *progress_data) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_info);
    SIGNALPROGRESS *progress = static_cast<SIGNALPROGRESS *>(progress_data);
    if (world == NULL || world->signals == NULL) {
        return;
    }
    SIGNAL *signal = world->signals;
    for (i32 index = 0; index < world->signal_count; ++index, ++signal) {
        NewTerrPlatformsOff();
        f32 height = GameShadow(NULL, &signal->position, 5.0f, -1);
        if (height != 2000000.0f) {
            signal->position.y = height;
            FindAnglesZX(&ShadNorm, &signal->terrain_pitch, &signal->terrain_roll);
        }
        signal->radius = 0.85f;
        signal->scale = 1.0f;
        NuVecRotateZ(&signal->normal, &v010, signal->terrain_roll);
        NuVecRotateX(&signal->normal, &signal->normal, signal->terrain_pitch);
        NuVecScale(&signal->target_position, &signal->normal, 0.6f);
        NuVecAdd(&signal->target_position, &signal->target_position, &signal->position);
        signal->suit = Suit_FindFromLetter(signal->suit_letter);
        if (signal->suit == NULL) {
            signal->suit = Suit;
        }
        signal->character_letter = signal->suit->group;
        signal->suit_letter = signal->suit->letter;
        if (signal->character_letter == 'r') {
            signal->animation_0 = 0x3a;
            signal->animation_1 = 0x3b;
            signal->animation_2 = 0x3c;
        } else {
            signal->animation_0 = 0x37;
            signal->animation_1 = 0x38;
            signal->animation_2 = 0x39;
        }
        signal->visible = 1;
        signal->active = 1;
        signal->in_use = 0;
        NuMtxSetRotationY(&signal->matrix, signal->yaw);
        NuMtxRotateZ(&signal->matrix, signal->terrain_roll);
        NuMtxRotateX(&signal->matrix, signal->terrain_pitch);
        NuMtxTranslate(&signal->matrix, &signal->position);
        if (index < 32 && progress != NULL) {
            u32 mask = 1u << index;
            if (progress->suit_letters[index] != 0) {
                signal->suit = Suit_FindFromLetter(progress->suit_letters[index]);
            }
            Signal_SetVisibility(signal, (progress->visible_mask & mask) != 0);
            signal->active = (progress->active_mask & mask) != 0;
        }
    }
}

static void Signals_StoreProgress(void *world_info, void *, void *progress_data) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_info);
    SIGNALPROGRESS *progress = static_cast<SIGNALPROGRESS *>(progress_data);
    Signals_ClearProgress(world_info, progress_data);
    if (progress == NULL || world == NULL || world->signals == NULL) {
        return;
    }
    SIGNAL *signal = world->signals;
    for (i32 index = 0; index < world->signal_count && index < 32; ++signal, ++index) {
        u32 mask = 1u << index;
        progress->suit_letters[index] = world->signals[index].suit->letter;
        if (!signal->visible) {
            progress->visible_mask &= ~mask;
        }
        if (!signal->active) {
            progress->active_mask &= ~mask;
        }
    }
}

static void Signal_SetVisibility(SIGNAL *signal, i32 visible) {
    u8 was_visible = signal->visible;
    signal->visible = visible != 0;

    if (signal->visible) {
        if (!was_visible) {
            signal->platform_id = NewPlatPickupInst(&signal->matrix, 2);
        }
    } else {
        if (was_visible) {
            DeletePlatinst(signal->platform_id);
        }
    }
}

static void Signal_SetGizmoVisibility(GIZMO *gizmo, i32 visible) {
    if (gizmo != NULL) {
        Signal_SetVisibility(static_cast<SIGNAL *>(gizmo->object), visible);
    }
}

void Signals_InitTerrain(WORLDINFO_s *world) {
    for (i32 index = 0; index < world->signal_count; ++index) {
        SIGNAL *signal = &world->signals[index];
        signal->platform_id = NewPlatPickupInst(&signal->matrix, 2);
    }
}

SIGNAL *Signal_FindNearest(WORLDINFO_s *world, nuvec_s *position, GameObject_s *object, float *distance) {
    SIGNAL *nearest = NULL;
    f32 nearest_distance = 1000000000.0f;
    SIGNAL *signal = world->signals;
    for (i32 index = 0; index < world->signal_count; ++index, ++signal) {
        f32 signal_distance;
        if (object != NULL) {
            if (!signal->visible || !signal->active || signal->in_use || signal->suit->character_id != object->id) {
                continue;
            }
            signal_distance = NuVecDistSqr(position, &signal->target_position, NULL);
        } else {
            signal_distance = NuVecDistSqr(position, &signal->position, NULL);
        }
        if (signal_distance < nearest_distance) {
            nearest_distance = signal_distance;
            nearest = signal;
        }
    }
    if (distance != NULL) {
        *distance = nearest_distance;
    }
    return nearest;
}

ADDGIZMOTYPE *Signals_RegisterGizmo(i32 type_id) {
    static ADDGIZMOTYPE addtype;

    addtype = Default_ADDGIZMOTYPE;
    addtype.name = "Signal";
    addtype.prefix = "";
    addtype.fns.unknown1 = 0x28;
    addtype.fns.get_max_gizmos_fn = Signals_GetMaxGizmos;
    addtype.fns.add_gizmos_fn = Signals_AddGizmos;
    addtype.fns.early_update_fn = NULL;
    addtype.fns.late_update_fn = Signals_Update;
    addtype.fns.draw_fn = Signals_Draw;
    addtype.fns.panel_draw_fn = NULL;
    addtype.fns.get_gizmo_name_fn = Signal_GetGizmoName;
    addtype.fns.get_output_fn = Signal_GetOutput;
    addtype.fns.get_output_name_fn = Signal_GetOutputName;
    addtype.fns.get_num_outputs_fn = Signal_GetNumOutputs;
    addtype.fns.activate_fn = Signal_Activate;
    addtype.fns.activate_rev_fn = Signal_ActivateRev;
    addtype.fns.set_visibility_fn = Signal_SetGizmoVisibility;
    addtype.fns.get_visibility_fn = NULL;
    addtype.fns.get_pos_fn = Signal_GetPos;
    addtype.fns.using_special_fn = NULL;
    addtype.fns.bolt_hit_plat_fn = NULL;
    addtype.fns.get_best_bolt_target_fn = NULL;
    addtype.fns.bolt_hit_fn = NULL;
    addtype.fns.allocate_progress_data_fn = Signals_AllocateProgressData;
    addtype.fns.clear_progress_fn = Signals_ClearProgress;
    addtype.fns.store_progress_fn = Signals_StoreProgress;
    addtype.fns.reset_fn = Signals_Reset;
    addtype.fns.reserve_buffer_space_fn = Signals_ReserveBufferSpace;
    addtype.fns.load_fn = Signals_Load;
    addtype.fns.post_load_fn = NULL;
    addtype.fns.add_level_sfx_fn = NULL;

    return &addtype;
}

void Signal_MoveCode(WORLDINFO_s *world, GameObject_s *object) {
    if (object->character_context != CHARACTER_CONTEXT_SIGNAL) {
        if (object->apiobj.field_0x27d != 0 || object->character_context == 0x1f ||
            object->character_context == CHARACTER_CONTEXT_DOOMED || object->character_context == 0x4b ||
            !object->apiobj.player_controlled || (CInfo[object->character_context].flags & 0x800) != 0) {
            return;
        }
        f32 distance;
        SIGNAL *signal = Signal_FindNearest(world, &object->apiobj.collision_position, object, &distance);
        if (signal != NULL && distance < 0.0625f) {
            object->signal_target = signal;
            object->context_animation_timer = 0.0f;
            object->character_context = CHARACTER_CONTEXT_SIGNAL;
            object->context_animation = 0x96;
            object->airborne_action_duration = AnimDuration(object->id, 0x96, 0.0f, 0.0f, 1);
            if (object->airborne_action_duration <= 0.0f) {
                object->airborne_action_duration = 1.0f;
            }
            object->context_flags &= ~0x40;
        }
    } else {
        f32 *animation_time = NULL;
        if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL) {
            animation_time = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
            if (animation_time == NULL) {
                return;
            }
        }

        object->context_animation_timer += FRAMETIME;
        bool exchange = false;
        if (object->context_animation_timer >= object->airborne_action_duration) {
            u8 exchanged = object->context_flags & 0x40;
            object->character_context = CHARACTER_CONTEXT_NONE;
            StartEndOfJump(object);
            exchange = exchanged == 0;
        } else {
            if ((object->context_flags & 0x40) != 0) {
                return;
            }
            if (animation_time != NULL) {
                f32 frame = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
                exchange = frame >= 1.0f && *animation_time >= frame;
            }
        }

        if (exchange) {
            object->signal_target->flags |= SIGNAL::FLAG_IN_USE;
            SIGNAL *signal = object->signal_target;
            SUIT_s *old_suit = static_cast<SUIT_s *>(object->suit);
            object->context_flags |= 0x40;
            SUIT_s *new_suit = signal->suit;
            object->suit = new_suit;
            signal->suit = old_suit;
            object->ai.capabilities =
                (object->ai.capabilities & ~old_suit->character_flags) | new_suit->character_flags;
            for (i32 i = 0; i < 10; ++i) {
                if (new_suit == &Suit[i]) {
                    areaSuitBits |= 1 << i;
                    break;
                }
            }
            NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
            NewRumble(object->pad_gamepad->pad, 0.6f, 0);
        }
        if ((object->context_flags & 0x40) == 0) {
            NewRumble(object->pad_gamepad->pad, QRAND_FLOAT() * 0.4f, 0);
        }
    }
}

void SetTexAnimSignals() {
    NuTexAnimSetSignals(texanimbits);
}
