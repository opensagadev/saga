#include "legoapi/gizmos/object/lever.h"
#include "legoapi/gizmo/base/GizLeverObjectInterface.h"

#include "batman.h"
#include "decomp.h"
#include "gameapi/edtools/edfile.h"
#include "gamelib/util/gamelib_util_types.h"
#include "globals.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/misc/utilities.h"

struct LEVERPROGRESS {
    u32 pulled_down[1];
    u32 enabled[1];
    u32 visible[1];
};

DECOMP_ASSERT(sizeof(LEVERPROGRESS) == 0xc, "LEVER progress ABI");

LEVER_CONFIG LeverSys = {0x55, 0};

extern "C" u8 show_lever_hint;

void ReleaseLever(GameObject_s *object) {
    if (object->character_context == 0x4a) {
        LEVER_s *lever = static_cast<LEVER_s *>(object->field_0x788);
        if (lever != NULL && !lever->being_pulled)
            lever->interacting = 0;
    }
}

i32 lever_gizmotype_id = -1;

void LEVER_s::ClearMechObjectInterface() {
    if (mech_object != NULL)
        delete mech_object;
}

MechObjectInterface *LEVER_s::GetMechObjectInterface() {
    if (mech_object == NULL)
        new GizLeverObjectInterface(*this);
    return mech_object;
}
extern "C" {
    u8 show_lever_hint = 0;
}
void Lever_MoveCode(WORLDINFO_s *world, GameObject_s *object) {
    f32 distance = 1.0e9f;
    if (object->character_context == 0x4a) {
        object->field_0x768 += FRAMETIME;
        if (object->field_0x768 > 1.0f)
            object->field_0x768 = 1.0f;
        object->context_animation_timer += FRAMETIME;
        if (object->context_animation_timer >= object->airborne_action_duration) {
            object->character_context = -1;
            if (object->context_flags & 0x40)
                return;
        } else {
            if (object->context_flags & 0x40)
                return;
            if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL) {
                f32 *playing = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
                if (playing == NULL)
                    return;
                f32 frame = *playing;
                if (!(frame >= AnimListFrame(object->apiobj.character_model, object->context_animation, 0)))
                    return;
            } else if (__builtin_expect(object->context_animation_timer < 0.25f, 0)) {
                return;
            }
        }
        if (object->apiobj.character_data->model_flags & 4) {
            static_cast<LEVER_s *>(object->field_0x788)->baddie = 1;
        } else {
            if (!(object->apiobj.character_data->model_flags & 0x200))
                static_cast<LEVER_s *>(object->field_0x788)->goodie = 1;
        }
        static_cast<LEVER_s *>(object->field_0x788)->flags_high |= 8;
        object->context_flags |= 0x40;
        if ((i8)object->apiobj.object_flags < 0)
            Hint_SetComplete(0x60c);
        return;
    }
    if (object->apiobj.character_model->model_data_b[0x5d] == NULL || object->apiobj.field_0x27d == 0)
        return;
    if (__builtin_expect(!(!ObjLandReady(object) && !objInNetWaitContext(object, 0x4a)), 1)) {
        LEVER_s *lever = Lever_FindNearest(world, &object->apiobj.lower_position, object, &distance);
        if (objInNetWaitContext(object, 0x4a)) {
            object->context_animation_timer -= FRAMETIME;
            if (object->context_animation_timer <= 0.0f) {
                object->character_context = -1;
                object->big_jump_data = NULL;
            }
        }
        if (lever == NULL)
            return;
        if (object == player) {
            show_lever_hint = 0;
            if (distance < 1.0f)
                show_lever_hint = 1;
        }
        f32 range = (object->apiobj.field_0x1dc + 0.25f) * lever->target_indicator_scale;
        if (__builtin_expect(!(distance < range * range), 0))
            return;
        if (!(object->pad_gamepad->buttons_pressed & GAMEPAD_SPECIAL) &&
            __builtin_expect(!objInNetWaitContext(object, 0x4a), 0))
            return;
        Lever_StartPull(object, lever);
    }
}

void Levers_InitTerrain(WORLDINFO_s *world) {
    if (world->levers != NULL) {
        for (i32 index = 0; index < world->nlevers; ++index) {
            world->levers[index].platform_id = NewPlatPickupInst(&world->levers[index], 3);
            PlatInstRotate(world->levers[index].platform_id, 1);
        }
    }
}

void Lever_GetAbsTargetPos(LEVER_s *lever, nuvec_s *target_position) {
    if (target_position != NULL && lever != NULL) {
        NUVEC offset = lever->target_offset;
        NuVecRotateY(&offset, &offset, lever->y_rotation);
        offset.x += lever->position.x;
        offset.z += lever->position.z;
        *target_position = offset;
    }
}

i32 Lever_FullyPulledDown(LEVER_s *lever) {
    return lever->visible && lever->being_pulled && lever->pull_progress >= 1.0f;
}

LEVER_s *Lever_FindNearest(WORLDINFO_s *world, nuvec_s *position, GameObject_s *object, f32 *distance_squared) {
    NUVEC target_position;
    LEVER_s *nearest = NULL;
    f32 nearest_distance = 1.0e9f;
    LEVER_s *lever = world->levers;
    for (i32 index = 0; index < world->nlevers; ++index, ++lever) {
        f32 candidate_distance;
        if (object != NULL) {
            if ((lever->flags & (LEVER_FLAG_INTERACTING | LEVER_FLAG_BEING_PULLED | LEVER_FLAG_VISIBLE |
                                 LEVER_FLAG_ENABLED)) != (LEVER_FLAG_VISIBLE | LEVER_FLAG_ENABLED) ||
                lever->pull_progress != 0.0f || lever->floor_position.y == 2000000.0f)
                continue;

            Lever_GetAbsTargetPos(lever, &target_position);
            candidate_distance = NuVecDistSqr(position, &target_position, NULL);
        } else {
            candidate_distance = NuVecDistSqr(position, &lever->position, NULL);
        }
        if (candidate_distance < nearest_distance) {
            nearest = lever;
            nearest_distance = candidate_distance;
        }
    }
    if (distance_squared != NULL)
        *distance_squared = nearest_distance;
    return nearest;
}

i32 Lever_BeingPulled(LEVER_s *lever) {
    return lever->being_pulled;
}

static i32 Levers_GetMaxGizmos(void *lever) {
    WORLDINFO *world = static_cast<WORLDINFO *>(lever);
    if (world == NULL) {
        return 0;
    }
    return world->current_level->max_levers;
}

static void Levers_AddGizmos(GIZMOSYS *gizmo_sys, i32 type_id, void *world_ptr, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    for (i32 index = 0; index < world->nlevers; ++index) {
        if (NuStrLen(world->levers[index].name) != 0) {
            AddGizmo(gizmo_sys, type_id, NULL, &world->levers[index]);
        }
    }
}

static void Levers_Update(void *world_ptr, void *, float) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (world->levers == NULL) {
        return;
    }

    LEVER_s *cursor = world->levers;
    for (i32 index = 0; index < world->nlevers; ++index, ++cursor) {
        LEVER_s &lever = *cursor;
        if ((lever.flags & (LEVER_FLAG_BEING_PULLED | LEVER_FLAG_INTERACTION_FINISHED | LEVER_FLAG_PULLED_DOWN)) == 0) {
            lever.animation_frame = 0;
            continue;
        }

        if (!lever.being_pulled) {
            lever.being_pulled = 1;
        }
        if (lever.auto_reset_timer > 0.0f) {
            lever.auto_reset_timer -= FRAMETIME;
            if (lever.auto_reset_timer <= 0.0f) {
                lever.returning = 1;
                lever.interaction_finished = 0;
                lever.pulled_down = 1;
                lever.animation_frame = 0;
                lever.auto_reset_timer = 0.0f;
                lever.pull_progress = 0.6f;
            } else {
                lever.animation_frame = static_cast<u16>(static_cast<i32>(lever.pull_progress * -32768.0f));
            }
            continue;
        }

        if ((lever.flags & (LEVER_FLAG_RETURNING | LEVER_FLAG_PULLED_DOWN)) != 0) {
            if (!lever.returning) {
                lever.returning = 1;
            }
            lever.pull_progress -= FRAMETIME;
            if (lever.pull_progress <= 0.0f) {
                lever.pull_progress = 0.0f;
                lever.auto_reset_timer = 0.0f;
                lever.interacting = 0;
                lever.being_pulled = 0;
                lever.goodie = 0;
                lever.baddie = 0;
                lever.returning = 0;
                lever.pulled_down = 0;
                lever.idle_animation_timer = 0.0f;
                lever.animation_frame = 0;
                continue;
            }
        } else {
            if (lever.pull_progress < 1.0f) {
                lever.pull_progress += FRAMETIME;
                if (lever.pull_progress > 1.0f) {
                    lever.pull_progress = 1.0f;
                    PlaySfx(const_cast<char *>("SwLever"), &lever.position);
                    if ((lever.flags & LEVER_FLAG_AUTO_RESET) != 0) {
                        lever.auto_reset_timer = lever.auto_reset_delay;
                    }
                }
            }
        }

        lever.animation_frame = static_cast<u16>(static_cast<i32>(lever.pull_progress * -32768.0f));
    }
}

static void Levers_Draw(void *world_ptr, void *, float) {
    enum LEVER_SPECIAL_INDEX {
        LEVER_SPECIAL_ANIMATED = 30,
        LEVER_SPECIAL_BASE = 31,
        LEVER_SPECIAL_MODEL_FIRST = 32,
        LEVER_SPECIAL_READY = 41,
        LEVER_SPECIAL_COMPLETE = 42,
        LEVER_SPECIAL_INCOMPLETE = 43,
        LEVER_SPECIAL_TARGET = 85,
    };

    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (world == NULL || world->lev_objs == NULL || world->levers == NULL ||
        world->lev_objs[LEVER_SPECIAL_ANIMATED].active == 0) {
        return;
    }

    const u16 target_spin_angle = static_cast<u16>(NuFmod(GameTimer.time_elapsed, 5.0f) / 5.0f * 65536.0f);
    const f32 pulse_phase = NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f) * 2.0f * 65536.0f;
    const f32 target_pulse = NuTrigTable[(static_cast<i32>(pulse_phase) >> 1) & 0x7fff] * 0.2f + 0.8f;
    const f32 ready_phase = NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f) * 2.0f * 65536.0f;
    const f32 ready_alpha = NuTrigTable[(static_cast<i32>(ready_phase) >> 1) & 0x7fff] * 0.15f + 0.85f;

    EnableShadowMapRendering(0);

    nuhspecial_s *animated_special = NULL;
    if (world->lev_objs[LEVER_SPECIAL_ANIMATED].active != 0) {
        animated_special = &world->lev_objs[LEVER_SPECIAL_ANIMATED].special;
        nuinstanim_s *instance_animation = NuSpecialGetInstAnim(animated_special);
        if (instance_animation != NULL) {
            NuAnimEndFrameOld(animated_special->scene->instance_animation_data[instance_animation->anim_ix]);
        }
    }

    auto world_nlevers = world->nlevers;
    for (i32 index = 0; index < world_nlevers; ++index) {
        LEVER_s &lever = world->levers[index];
        if ((lever.flags & LEVER_FLAG_VISIBLE) == 0) {
            continue;
        }

        TouchHacks::TintStack tint;
        lever.flash_timer -= FRAMETIME;
        if (TouchHacks::ShouldFlash(lever.flash_timer)) {
            NUCOLOUR3 *flash_colour = TouchHacks::GetFlashColour();
            NuRndrLightingStateCurrent.ambient = *flash_colour;
            NuRndrSetAmbientLightPS(flash_colour);
        }

        NUMTX animated_matrix;
        if (animated_special == NULL) {
            NuMtxSetRotationX(&animated_matrix, 0);
            NuMtxRotateY(&animated_matrix, lever.y_rotation);
            NuMtxTranslate(&animated_matrix, &lever.position);
            NuSpecialDrawAt(&world->lev_objs[LEVER_SPECIAL_ANIMATED].special, &animated_matrix);
        } else {
            f32 animation_frame = 0.0f;
            if (!(lever.pull_progress <= 0.0f)) {
                if (lever.auto_reset_timer > 0.0f) {
                    animation_frame = 32.0f;
                } else if ((lever.flags & LEVER_FLAG_RETURNING) == 0) {
                    animation_frame = lever.pull_progress * 31.0f + 1.0f;
                } else {
                    animation_frame = 32.0f + ((0.6f - lever.pull_progress) * 32.0f) / 0.6f;
                }
            } else if ((lever.flags & LEVER_FLAG_IDLE_STATE_MASK) == LEVER_FLAG_ENABLED) {
                if (lever.idle_animation_timer > 0.0f) {
                    lever.idle_animation_timer -= FRAMETIME;
                    if (!(lever.idle_animation_timer < 0.0f)) {
                        const f32 idle_phase = lever.idle_animation_timer / 0.15f * 32768.0f + 16384.0f;
                        animation_frame =
                            (1.0f - NuFabs(NuTrigTable[(static_cast<i32>(idle_phase) >> 1) & 0x7fff])) * 1.5f + 16.0f;
                    } else {
                        lever.idle_animation_timer = 0.0f;
                    }
                } else if (qrand() < 0x800) {
                    lever.idle_animation_timer = 0.15f;
                }
            }

            EvalAnim(animated_special, animation_frame, &animated_matrix, 0);
            NuMtxMulVU0(&animated_matrix, &animated_matrix, &lever.transform);
            NuSpecialDrawAt(animated_special, &animated_matrix);
        }

        NUMTX target_matrix = animated_matrix;
        NUMTX base_matrix;
        NuMtxSetRotationY(&base_matrix, lever.y_rotation);
        NuMtxTranslate(&base_matrix, &lever.position);

        LEVEL_OBJECT_RUNTIME &model_entry = world->lev_objs[lever.model_special_index];
        if (model_entry.active != 0) {
            NuSpecialDrawAt(&model_entry.special, &animated_matrix);
        }

        if (world->lev_objs[LEVER_SPECIAL_BASE].active != 0 && (lever.flags & LEVER_FLAG_HIDE_BASE) == 0) {
            NuSpecialDrawAt(&world->lev_objs[LEVER_SPECIAL_BASE].special, &lever.transform);
        }

        i32 status_special = LEVER_SPECIAL_INCOMPLETE;
        if ((lever.flags & LEVER_FLAG_ENABLED) != 0) {
            status_special = !(lever.pull_progress >= 1.0f) ? LEVER_SPECIAL_INCOMPLETE : LEVER_SPECIAL_COMPLETE;
            bool draw_target = false;
            if ((lever.flags & LEVER_FLAG_HIDE_TARGET_INDICATOR) == 0 && lever.target_offset.y != 2000000.0f) {
                draw_target = world->lev_objs[LEVER_SPECIAL_TARGET].active != 0;
            }

            if (__builtin_expect((lever.flags & LEVER_FLAG_IDLE_STATE_MASK) == LEVER_FLAG_ENABLED &&
                                     world->lev_objs[LEVER_SPECIAL_READY].active != 0,
                                 0)) {
                NuSpecialDrawAtAlpha(&world->lev_objs[LEVER_SPECIAL_READY].special, &target_matrix, ready_alpha);
            } else if (!draw_target) {
                goto draw_status;
            }

            GameObject_s *nearest_player;
            f32 distance_squared;
            if (FindNearestPlayerToVec(&lever.position, &nearest_player, distance_squared, false, 0)) {
                const f32 distance_factor = NuFmin(distance_squared / 6.0f, 1.0f);
                const f32 target_alpha =
                    (1.0f -
                     NuTrigTable[(static_cast<i32>(distance_factor * 16384.0f + 49152.0f + 16384.0f) >> 1) & 0x7fff]) *
                    target_pulse;
                if (target_alpha > 0.0f && draw_target) {
                    NUVEC target_position;
                    Lever_GetAbsTargetPos(&lever, &target_position);
                    NuMtxSetRotationY(&animated_matrix, target_spin_angle);
                    if (lever.target_z_rotation != 0) {
                        NuMtxRotateZ(&animated_matrix, lever.target_z_rotation);
                    }
                    if (lever.target_x_rotation != 0) {
                        NuMtxRotateX(&animated_matrix, lever.target_x_rotation);
                    }
                    NuMtxScaleU(&animated_matrix, 0.8f);
                    NuMtxTranslate(&animated_matrix, &target_position);
                    NuMtxPreScaleU(&animated_matrix, lever.target_indicator_scale);
                    NuSpecialDrawAtAlpha(&world->lev_objs[LEVER_SPECIAL_TARGET].special, &animated_matrix,
                                         (1.0f - lever.pull_progress) * target_alpha);
                }
            }
        } else if (lever.pull_progress == 0.0f && ParticlesPerSecond(0.5f, FRAMETIME) > 0) {
            AddGameDebris(WORLD->debris_sys, 0x57, &lever.position);
        }

    draw_status:
        if (world->lev_objs[status_special].active != 0 && (lever.flags & LEVER_FLAG_HIDE_BASE) == 0) {
            NuSpecialDrawAt(&world->lev_objs[status_special].special, &base_matrix);
        }
    }

    ResetShadowMapRendering();
}

static char *Lever_GetGizmoName(GIZMO *gizmo) {
    if (gizmo == NULL) {
        return NULL;
    }
    return static_cast<LEVER_s *>(gizmo->object)->name;
}

static i32 Lever_GetOutput(GIZMO *gizmo, i32 output_index, i32) {
    LEVER_s *lever = static_cast<LEVER_s *>(gizmo->object);
    if (!lever->enabled) {
        return 0;
    }
    switch (output_index) {
        case 0:
            return Lever_FullyPulledDown(lever) != 0;
        case 1:
            if (lever->goodie && Lever_FullyPulledDown(lever) != 0) {
                return 1;
            }
            break;
        case 2:
            if (lever->baddie && Lever_FullyPulledDown(lever) != 0) {
                return 1;
            }
            break;
    }
    return 0;
}

static char *Lever_GetOutputName(GIZMO *gizmo, i32 output_index) {
    if (output_index == 1) {
        return "Down(Goodie)";
    }

    if (output_index != 2) {
        if (output_index == 0) {
            return "Down";
        }

        return NULL;
    }

    return "Down(Baddie)";
}

static i32 Lever_GetNumOutputs(GIZMO *gizmo) {
    return 3;
}

static void Lever_Reset(LEVER_s *lever);

static void Lever_Activate(GIZMO *gizmo, i32 enabled) {
    if (gizmo == NULL || gizmo->object == NULL) {
        return;
    }

    LEVER_s *lever = static_cast<LEVER_s *>(gizmo->object);
    if (enabled != 0) {
        lever->flags |= LEVER_FLAG_ENABLED;
        Lever_Reset(lever);
    } else {
        lever->flags &= ~LEVER_FLAG_ENABLED;
    }
}

static i32 Lever_ActivateRev(GIZMO *gizmo, i32 value, i32 query) {
    if (gizmo == NULL || gizmo->object == NULL) {
        return 1;
    }

    LEVER_s *lever = static_cast<LEVER_s *>(gizmo->object);
    if ((query & 1) != 0) {
        if (!lever->enabled) {
            return value == 0;
        }
        return value;
    }
    lever->enabled = value == 0;
    return 1;
}

static void Lever_SetVisibility(GIZMO *gizmo, i32 visible) {
    if (gizmo == NULL || gizmo->object == NULL) {
        return;
    }

    LEVER_s *lever = static_cast<LEVER_s *>(gizmo->object);
    const u8 was_visible = lever->visible;
    lever->visible = visible != 0;
    if (lever->visible) {
        if (!was_visible) {
            lever->platform_id = NewPlatPickupInst(lever, 3);
            PlatInstRotate(lever->platform_id, 1);
        }
    } else if (was_visible) {
        DeletePlatinst(lever->platform_id);
    }
}

static void *Levers_AllocateProgressData(VARIPTR *buffer, VARIPTR *buffer_end) {
    return GizmoBufferAlloc(buffer, buffer_end, sizeof(LEVERPROGRESS));
}

static void Levers_ClearProgress(void *, void *progress_data) {
    LEVERPROGRESS *progress = (LEVERPROGRESS *)progress_data;
    if (progress == NULL) {
        return;
    }

    progress->pulled_down[0] = 0;
    progress->enabled[0] = 0xffffffff;
    progress->visible[0] = 0xffffffff;
}

static void Levers_StoreProgress(void *world_ptr, void *, void *progress_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    LEVERPROGRESS *progress = static_cast<LEVERPROGRESS *>(progress_ptr);
    if (progress == NULL) {
        return;
    }

    Levers_ClearProgress(NULL, progress);
    if (world == NULL || world->levers == NULL) {
        return;
    }

    const i32 count = world->nlevers;
    const LEVER_s *lever = world->levers;
    for (i32 index = 0; index < count; ++index, ++lever) {
        if (index == 32) {
            break;
        }
        const i32 word = index >> 5;
        const u32 bit = 1u << index;
        if (lever->being_pulled && lever->pull_progress >= 1.0f) {
            progress->pulled_down[word] |= bit;
        }
        if (!lever->visible) {
            progress->visible[word] &= ~bit;
        }
        if (!lever->enabled) {
            progress->enabled[word] &= ~bit;
        }
    }
}

static void Levers_Reset(void *world_ptr, void *, void *progress_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    LEVERPROGRESS *progress = static_cast<LEVERPROGRESS *>(progress_ptr);
    if (world == NULL || world->levers == NULL) {
        return;
    }

    LEVER_s *lever = world->levers;
    for (i32 index = 0; index < world->nlevers; ++index, ++lever) {
        Lever_Reset(lever);
        if (index < 32 && progress != NULL) {
            const u32 bit = 1u << index;
            if ((progress->pulled_down[0] & bit) != 0) {
                lever->being_pulled = 1;
                lever->pull_progress = 1.0f;
                lever->animation_frame = 0x8000;
            }
            lever->visible = (progress->visible[0] & bit) != 0;
            lever->enabled = (progress->enabled[0] & bit) != 0;
        }
    }
}

static void *Levers_ReserveBufferSpace(void *world_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    world->levers = NULL;
    world->nlevers = 0;
    if (world->current_level->max_levers != 0) {
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 0x10);
        world->levers = reinterpret_cast<LEVER_s *>(world->giz_buffer.addr);
        world->giz_buffer.addr += world->current_level->max_levers * sizeof(LEVER_s);
    }
    return world->levers;
}

static i32 Levers_Load(void *world_ptr, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (world->nlevers != 0) {
        return 0;
    }

    const i32 version = EdFileReadInt();
    world->nlevers = EdFileReadInt();
    for (i32 index = 0; index < world->nlevers; ++index) {
        EdFileRead(world->levers[index].name, sizeof(world->levers[index].name));
        EdFileReadNuVec(&world->levers[index].position);
        world->levers[index].y_rotation = static_cast<u16>(EdFileReadShort());
        world->levers[index].model_variant = static_cast<char>(EdFileReadChar());

        if (version > 1) {
            world->levers[index].auto_reset = EdFileReadChar();
        }
        if (version > 2) {
            world->levers[index].auto_reset_delay = EdFileReadFloat();
        } else {
            world->levers[index].auto_reset_delay = 2.0f;
        }
        if (version > 3) {
            world->levers[index].hide_base = EdFileReadChar();
        } else {
            world->levers[index].hide_base = 0;
        }
        if (version > 4) {
            EdFileReadNuVec(&world->levers[index].target_offset);
            world->levers[index].target_indicator_scale = EdFileReadFloat();
        } else {
            world->levers[index].target_offset.x = 0.0f;
            world->levers[index].target_offset.y = 0.0f;
            world->levers[index].target_offset.z = -0.1441f;
            world->levers[index].target_indicator_scale = 1.0f;
        }
        if (version > 5) {
            world->levers[index].hide_target_indicator = EdFileReadChar();
        }
        world->levers[index].platform_id = -1;
    }
    return 1;
}

static void Lever_Reset(LEVER_s *lever) {
    if (lever->model_variant == 'r') {
        lever->model_special_index = 0x22;
    } else if (lever->model_variant == 'o') {
        lever->model_special_index = 0x21;
    } else if (lever->model_variant == 'y') {
        lever->model_special_index = 0x20;
    } else if (lever->model_variant == 'b') {
        lever->model_special_index = 0x23;
    } else if (lever->model_variant == 'g') {
        lever->model_special_index = 0x24;
    } else if (lever->model_variant == 'p') {
        lever->model_special_index = 0x25;
    } else if (lever->model_variant == 'w') {
        lever->model_special_index = 0x26;
    } else if (lever->model_variant == 'u') {
        lever->model_special_index = 0x27;
    } else if (lever->model_variant == 'l') {
        lever->model_special_index = 0x28;
    }

    lever->floor_position.y = 0.0f;
    lever->floor_position.x = 0.0f;
    lever->floor_position.z = 0.1441f;
    NuVecRotateY(&lever->floor_position, &lever->floor_position, lever->y_rotation + 0x8000);
    NuVecAdd(&lever->floor_position, &lever->floor_position, &lever->position);

    NUVEC target_position;
    Lever_GetAbsTargetPos(lever, &target_position);
    target_position.y = lever->position.y;
    lever->floor_position.y = GameShadow(NULL, &lever->floor_position, 0.2f, -1);
    const f32 target_floor = GameShadow(NULL, &target_position, 0.2f, -1);
    if (target_floor == 2000000.0f) {
        lever->target_offset.y = 2000000.0f;
    } else {
        lever->target_offset.y = target_floor + 0.005f;
        FindAnglesZX(&ShadNorm, &lever->target_x_rotation, &lever->target_z_rotation);
    }

    lever->animation_frame = 0;
    lever->visible = 1;
    lever->enabled = 1;
    if (!lever->being_pulled || !lever->auto_reset) {
        lever->pull_progress = 0.0f;
        lever->auto_reset_timer = 0.0f;
        lever->interacting = 0;
        lever->being_pulled = 0;
        lever->goodie = 0;
        lever->baddie = 0;
        lever->returning = 0;
        lever->pulled_down = 0;
    }

    NuMtxSetRotationY(&lever->transform, lever->y_rotation);
    NuMtxTranslate(&lever->transform, &lever->position);
}

ADDGIZMOTYPE *Levers_RegisterGizmo(i32 type_id) {
    static ADDGIZMOTYPE addtype;

    addtype = Default_ADDGIZMOTYPE;
    addtype.name = "Lever";
    addtype.fns.unknown1 = 12;
    addtype.fns.early_update_fn = NULL;
    addtype.prefix = "";
    addtype.fns.panel_draw_fn = NULL;
    addtype.fns.get_visibility_fn = NULL;
    addtype.fns.get_max_gizmos_fn = Levers_GetMaxGizmos;
    addtype.fns.get_pos_fn = NULL;
    addtype.fns.using_special_fn = NULL;
    addtype.fns.add_gizmos_fn = Levers_AddGizmos;
    addtype.fns.bolt_hit_plat_fn = NULL;
    addtype.fns.get_best_bolt_target_fn = NULL;
    addtype.fns.late_update_fn = Levers_Update;
    addtype.fns.bolt_hit_fn = NULL;
    addtype.fns.draw_fn = Levers_Draw;
    addtype.fns.get_gizmo_name_fn = Lever_GetGizmoName;
    addtype.fns.get_output_fn = Lever_GetOutput;
    addtype.fns.get_output_name_fn = Lever_GetOutputName;
    addtype.fns.get_num_outputs_fn = Lever_GetNumOutputs;
    addtype.fns.activate_fn = Lever_Activate;
    addtype.fns.activate_rev_fn = Lever_ActivateRev;
    addtype.fns.set_visibility_fn = Lever_SetVisibility;
    addtype.fns.allocate_progress_data_fn = Levers_AllocateProgressData;
    addtype.fns.clear_progress_fn = Levers_ClearProgress;
    addtype.fns.store_progress_fn = Levers_StoreProgress;
    addtype.fns.reset_fn = Levers_Reset;
    addtype.fns.reserve_buffer_space_fn = Levers_ReserveBufferSpace;
    addtype.fns.load_fn = Levers_Load;
    addtype.fns.post_load_fn = NULL;
    addtype.fns.add_level_sfx_fn = NULL;
    lever_gizmotype_id = type_id;

    return &addtype;
}
