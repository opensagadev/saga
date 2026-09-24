#include "legoapi/gizmos/object/hatmachine.h"

#include "decomp.h"
#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/edtools/edfile.h"
#include "globals.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/gizmo/base/HatMachineObjectInterface.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/menus/core/gamehint.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

#include <string.h>

static const NUVEC HatMachine_HatOffset = {0.0f, 0.3f, 0.0f};

extern u8 show_hatmachine_hint;
extern i32 editor_active;
extern "C" i16 id_PRINCESSLEIABOUSHH;
void LoseHelmet(GameObject_s *, i32, i32);
void FastWeaponIn(GameObject_s *, i32);
void AlertSurroundingCreatures(GameObject_s *, NUVEC *);
void MakeBaddiesForgetAboutParty(i32);

enum HATMACHINE_ANIMATION_STATE {
    HATMACHINE_ANIMATION_IDLE = 0,
    HATMACHINE_ANIMATION_PLAY_HAT_SFX = 2,
    HATMACHINE_ANIMATION_COMPLETE = 7,
};

enum HATMACHINE_PLATFORM_TYPE {
    HATMACHINE_PLATFORM_COLLISION = 2,
};

struct HATMACHINEPROGRESS {
    i32 preserved_state;
    u32 enabled_mask;
    u32 visible_mask;
};

i32 hatmachine_gizmotype_id = -1;

static void HatMachine_Reset(HATMACHINE_s *machine);

static i32 HatMachine_GetMaxGizmos(void *world_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    return world != NULL ? world->current_level->max_hat_machines : 0;
}

static char *HatMachine_GetGizmoName(GIZMO *gizmo) {
    if (gizmo == NULL || gizmo->object == NULL) {
        return NULL;
    }
    return static_cast<HATMACHINE *>(gizmo->object)->name;
}

static i32 HatMachine_GetOutput(GIZMO *gizmo, i32, i32) {
    HATMACHINE *machine = static_cast<HATMACHINE *>(gizmo->object);
    return (static_cast<u8>(machine->flags) >> 1) & 1;
}

static char *HatMachine_GetOutputName(GIZMO *, i32 output_index) {
    return output_index == 0 ? const_cast<char *>("Finished") : NULL;
}

static i32 HatMachine_GetNumOutputs(GIZMO *) {
    return 1;
}

static i32 HatMachine_Load(void *world_ptr, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (world == NULL) {
        return 0;
    }

    HATMACHINESYS_s *system = world->hat_machine_sys;
    if (system == NULL || system->count != 0) {
        return 0;
    }

    const i32 version = EdFileReadInt();
    system->count = EdFileReadInt();
    if (system->count > 0) {
        for (i32 index = 0; index < system->count; ++index) {
            EdFileRead(system->machines[index].name, EdFileReadInt());
            EdFileReadNuVec(&system->machines[index].position);
            system->machines[index].yaw = EdFileReadShort();
            system->machines[index].configured_hat = static_cast<u8>(EdFileReadChar());

            if (version <= 2) {
                system->machines[index].model_letter = 'r';
            } else {
                system->machines[index].model_letter = static_cast<char>(EdFileReadChar());
            }

            if (version <= 3) {
                HATMACHINE *machine = &system->machines[index];
                machine->target_offset.y = 0.0f;
                machine->target_offset.x = 0.0f;
                machine->target_offset.z = -0.1441f;
                machine->scale = 1.0f;
                machine->platform_id = -1;
                continue;
            } else {
                EdFileReadNuVec(&system->machines[index].target_offset);
                system->machines[index].scale = EdFileReadFloat();
                if (version != 4) {
                    const u8 hidden = static_cast<u8>(EdFileReadChar()) & 1;
                    system->machines[index].flags = static_cast<HATMACHINE_FLAGS>(
                        (system->machines[index].flags & ~HATMACHINE_FLAG_HIDE_MACHINE) | (hidden << 5));
                }
            }
            system->machines[index].platform_id = -1;
        }
    }
    return 1;
}

static void *HatMachines_ReserveBufferSpace(void *world_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    world->hat_machine_sys = NULL;
    if (world->current_level->max_levers == 0) {
        return NULL;
    }

    world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
    world->hat_machine_sys = static_cast<HATMACHINESYS_s *>(world->giz_buffer.void_ptr);
    world->giz_buffer.addr += sizeof(HATMACHINESYS_s);
    memset(world->hat_machine_sys, 0, sizeof(*world->hat_machine_sys));

    world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 16);
    world->hat_machine_sys->machines = static_cast<HATMACHINE *>(world->giz_buffer.void_ptr);
    world->giz_buffer.addr += world->current_level->max_hat_machines * sizeof(HATMACHINE);
    memset(world->hat_machine_sys->machines, 0, world->current_level->max_hat_machines * sizeof(HATMACHINE));
    return world->hat_machine_sys;
}

static void HatMachines_ClearProgress(void *, void *progress_data) {
    HATMACHINEPROGRESS *progress = (HATMACHINEPROGRESS *)progress_data;
    if (progress == NULL) {
        return;
    }

    progress->enabled_mask = ~0u;
    progress->visible_mask = ~0u;
}

static void HatMachine_Update(void *world_ptr, void *, float elapsed) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    HATMACHINESYS_s *system = world->hat_machine_sys;
    if (system == NULL || system->count <= 0) {
        return;
    }

    for (i32 index = 0; index < system->count; ++index) {
        HATMACHINE *machine = &system->machines[index];

        if (machine->hat_delay > 0.0f) {
            machine->hat_delay -= FRAMETIME;
            if (machine->hat_delay <= 0.0f && machine->displayed_hat == 0) {
                if (machine->configured_hat_count != 0) {
                    machine->displayed_hat = machine->configured_hat_count;
                } else {
                    machine->displayed_hat =
                        static_cast<u8>(NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) * 4.0f) + 1;
                }
                machine->hat_delay = 1.0f;
            }
        }

        if ((machine->flags & HATMACHINE_FLAG_ANIMATING) != 0 && machine->animation_state > 0) {
            machine->state_elapsed += elapsed;
            if (machine->state_elapsed > machine->state_duration) {
                ++machine->animation_state;
                if (machine->animation_state == HATMACHINE_ANIMATION_PLAY_HAT_SFX) {
                    PlaySfx("HatOn", &machine->position);
                }
                machine->state_elapsed = 0.0f;

                if (machine->animation_state >= HATMACHINE_ANIMATION_COMPLETE) {
                    machine->state_duration = 0.0f;
                    machine->hat_delay = 0.6f;
                    machine->animation_time = 0.0f;
                    machine->animation_state = HATMACHINE_ANIMATION_IDLE;
                    machine->flags = static_cast<HATMACHINE_FLAGS>(machine->flags & ~HATMACHINE_FLAG_ANIMATING);
                }
            }
        }
    }
}

static void *HatMachines_AllocateProgressData(VARIPTR *buffer, VARIPTR *buffer_end) {
    return GizmoBufferAlloc(buffer, buffer_end, sizeof(HATMACHINEPROGRESS));
}

static void HatMachine_SetVisibility(GIZMO *gizmo, i32 visible) {
    if (gizmo == NULL || gizmo->object == NULL) {
        return;
    }

    HATMACHINE *machine = static_cast<HATMACHINE *>(gizmo->object);
    u8 flags = machine->flags;
    u8 previous_visibility = flags;
    previous_visibility >>= 2;
    const i32 was_visible = previous_visibility & 1;
    visible = visible != 0;
    flags = static_cast<u8>((flags & ~HATMACHINE_FLAG_VISIBLE) | (visible << 2));
    machine->flags = static_cast<HATMACHINE_FLAGS>(flags);

    if ((flags & HATMACHINE_FLAG_VISIBLE) != 0) {
        if (was_visible == 0) {
            machine->platform_id = NewPlatPickupInst(machine, HATMACHINE_PLATFORM_COLLISION);
            PlatInstRotate(machine->platform_id, 1);
        }
    } else if (was_visible != 0) {
        DeletePlatinst(machine->platform_id);
    }
}

static void HatMachine_AddGizmos(GIZMOSYS *gizmo_sys, i32 type_id, void *world_ptr, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (world == NULL || world->hat_machine_sys == NULL || world->hat_machine_sys->count <= 0) {
        return;
    }

    for (i32 index = 0; index < world->hat_machine_sys->count; ++index) {
        HATMACHINE *machine = &world->hat_machine_sys->machines[index];
        if (NuStrLen(machine->name) != 0) {
            AddGizmo(gizmo_sys, type_id, NULL, machine);
        }
    }
}

static void HatMachines_StoreProgress(void *world_ptr, void *, void *progress_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    HATMACHINEPROGRESS *progress = static_cast<HATMACHINEPROGRESS *>(progress_ptr);
    if (progress == NULL) {
        return;
    }

    progress->enabled_mask = ~0u;
    progress->visible_mask = ~0u;
    if (world == NULL || world->hat_machine_sys == NULL || world->hat_machine_sys->machines == NULL) {
        return;
    }

    HATMACHINESYS_s *system = world->hat_machine_sys;
    for (i32 index = 0; index < system->count && index < 32; ++index) {
        const u32 mask = 1u << index;
        HATMACHINE *machine = &system->machines[index];
        if ((machine->flags & HATMACHINE_FLAG_VISIBLE) == 0) {
            progress->visible_mask &= ~mask;
        }
        if ((machine->flags & HATMACHINE_FLAG_ENABLED) == 0) {
            progress->enabled_mask &= ~mask;
        }
    }
}

void Hat_GetAbsTargetPos(HATMACHINE_s *machine, NUVEC *position) {
    if (position == NULL || machine == NULL) {
        return;
    }

    NUVEC offset = machine->target_offset;
    NuVecRotateY(&offset, &offset, machine->y_rotation);
    offset.x += machine->position.x;
    offset.z += machine->position.z;
    *position = offset;
}

static void HatMachine_Reset(HATMACHINE_s *machine) {
    machine->player_position.y = 0.0f;
    machine->player_position.x = 0.0f;
    machine->player_position.z = 0.1441f;
    NuVecRotateY(&machine->player_position, &machine->player_position, machine->yaw + 0x8000);
    NuVecAdd(&machine->player_position, &machine->player_position, &machine->position);

    NUVEC target_position;
    Hat_GetAbsTargetPos(machine, &target_position);
    target_position.y = machine->position.y;

    machine->player_position.y = GameShadow(NULL, &machine->player_position, 1.0f, -1);
    const f32 target_y = GameShadow(NULL, &target_position, 1.0f, -1);
    if (target_y == 2000000.0f) {
        machine->target_offset.y = target_y;
    } else {
        machine->target_offset.y = target_y + 0.005f;
        FindAnglesZX(&ShadNorm, &machine->terrain_pitch, &machine->terrain_roll);
    }

    machine->animation_state = 0;
    machine->progress_state1 = 1;
    machine->progress_state0 = 1;
    machine->state_bit0 = 0;
    machine->state_bit1 = 0;
    machine->hat_refresh_timer = 0.0f;
    machine->animation_duration = 0.0f;
    machine->render_animation_time = 0.0f;

    if (machine->model_letter == 'r') {
        machine->model_special_index = 0x22;
    } else {
        machine->model_special_index = (machine->model_letter == 'o') + 0x20;
    }

    if (machine->configured_hat != 0) {
        machine->current_hat = machine->configured_hat;
    } else {
        machine->current_hat = static_cast<u8>(NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) * 4.0f) + 1;
    }

    NuMtxSetRotationY(&machine->matrix, machine->yaw);
    NuMtxTranslate(&machine->matrix, &machine->position);
}

static void HatMachines_Reset(void *world_ptr, void *, void *progress_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (world == NULL) {
        return;
    }

    HATMACHINESYS_s *system = world->hat_machine_sys;
    if (system == NULL || system->machines == NULL || system->count <= 0) {
        return;
    }

    HATMACHINEPROGRESS *progress = static_cast<HATMACHINEPROGRESS *>(progress_ptr);
    for (i32 index = 0; index < system->count; ++index) {
        HATMACHINE_s *machine = &system->machines[index];
        HatMachine_Reset(machine);

        if (index <= 31 && progress != NULL) {
            const u32 mask = 1u << index;
            machine->progress_state1 = (progress->visible_mask & mask) != 0;
            machine->progress_state0 = (progress->enabled_mask & mask) != 0;
        }
    }
}

static void HatMachine_Activate(GIZMO *gizmo, i32 enabled) {
    if (gizmo == NULL || gizmo->object == NULL) {
        return;
    }

    HATMACHINE *machine = static_cast<HATMACHINE *>(gizmo->object);
    if (__builtin_expect(enabled == 0, 0))
        goto disable;

    machine->progress_state0 = 1;
    HatMachine_Reset(machine);
    return;

disable:
    machine->progress_state0 = 0;
}

static void HatMachine_Draw(void *world_ptr, void *, float) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (world == NULL || world->hat_machine_sys == NULL || world->hat_machine_sys->count == 0) {
        return;
    }

    const u16 spin_angle = static_cast<u16>(NuFmod(GameTimer.time_elapsed, 5.0f) / 5.0f * 65536.0f);
    const f32 pulse_phase = NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f) * 2.0f * 65536.0f;
    const f32 target_pulse = NuTrigTable[(static_cast<i32>(pulse_phase) >> 1) & 0x7fff] * 0.2f + 0.8f;
    const f32 ready_phase = NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f) * 2.0f * 65536.0f;
    const f32 ready_alpha = NuTrigTable[(static_cast<i32>(ready_phase) >> 1) & 0x7fff] * 0.15f + 0.85f;

    EnableShadowMapRendering(0);

    // The four machine effects share the same animation playback convention.  End
    // their current instance animation once per draw, before evaluating a frame.
    nuhspecial_s *animated_special = &world->lev_objs[288].special;
    nuinstanim_s *animated_instance_animation = NuSpecialGetInstAnim(animated_special);
    f32 animated_end_frame = 0.0f;
    if (animated_instance_animation != NULL && animated_special->scene != NULL) {
        animated_end_frame =
            NuAnimEndFrameOld(animated_special->scene->instance_animation_data[animated_instance_animation->anim_ix]);
    }

    nuhspecial_s *effect_special_a = &world->lev_objs[285].special;
    nuinstanim_s *effect_instance_animation_a = NuSpecialGetInstAnim(effect_special_a);
    f32 effect_end_frame_a = 0.0f;
    if (effect_instance_animation_a != NULL && effect_special_a->scene != NULL) {
        effect_end_frame_a =
            NuAnimEndFrameOld(effect_special_a->scene->instance_animation_data[effect_instance_animation_a->anim_ix]);
    }

    nuhspecial_s *effect_special_b = &world->lev_objs[289].special;
    nuinstanim_s *effect_instance_animation_b = NuSpecialGetInstAnim(effect_special_b);
    f32 effect_end_frame_b = 0.0f;
    if (effect_instance_animation_b != NULL && effect_special_b->scene != NULL) {
        effect_end_frame_b =
            NuAnimEndFrameOld(effect_special_b->scene->instance_animation_data[effect_instance_animation_b->anim_ix]);
    }

    nuhspecial_s *effect_special_c = &world->lev_objs[284].special;
    nuinstanim_s *effect_instance_animation_c = NuSpecialGetInstAnim(effect_special_c);
    f32 effect_end_frame_c = 0.0f;
    if (effect_instance_animation_c != NULL && effect_special_c->scene != NULL) {
        effect_end_frame_c =
            NuAnimEndFrameOld(effect_special_c->scene->instance_animation_data[effect_instance_animation_c->anim_ix]);
    }

    HATMACHINESYS_s *system = world->hat_machine_sys;
    for (i32 index = 0; index < system->count; ++index) {
        HATMACHINE_s *machine = &system->machines[index];
        if ((machine->flags & HATMACHINE_FLAG_VISIBLE) == 0 && editor_active == 0) {
            continue;
        }

        TouchHacks::TintStack tint;
        machine->flash_timer -= FRAMETIME;
        if (TouchHacks::ShouldFlash(machine->flash_timer)) {
            NUCOLOUR3 *flash_colour = TouchHacks::GetFlashColour();
            NuRndrLightingStateCurrent.ambient = *flash_colour;
            NuRndrSetAmbientLightPS(flash_colour);
        }

        f32 animation_frame = machine->state_elapsed;
        if (animation_frame < 0.0f) {
            animation_frame = 0.0f;
        }

        // The inactive model is still useful to touch controls.  The selected
        // model is drawn below after the animated machine body.
        if (world->lev_objs[283].active != 0) {
            NuSpecialDrawAt(&world->lev_objs[283].special, &machine->transform);
        }

        if ((machine->flags & HATMACHINE_FLAG_ENABLED) == 0) {
            if (world->lev_objs[287].active != 0) {
                NuSpecialDrawAt(&world->lev_objs[287].special, &machine->transform);
            }
        } else if (world->lev_objs[286].active != 0) {
            NuSpecialDrawAt(&world->lev_objs[286].special, &machine->transform);
        }

        if ((machine->flags & HATMACHINE_FLAG_HIDE_MACHINE) == 0 && machine->target_offset.y != 2000000.0f &&
            world->lev_objs[85].active != 0) {
            NUVEC target_position;
            Hat_GetAbsTargetPos(machine, &target_position);
            NUMTX target_matrix;
            NuMtxSetRotationY(&target_matrix, spin_angle);
            if (machine->terrain_roll != 0) {
                NuMtxRotateZ(&target_matrix, machine->terrain_roll);
            }
            if (machine->terrain_pitch != 0) {
                NuMtxRotateX(&target_matrix, machine->terrain_pitch);
            }
            NuMtxScaleU(&target_matrix, 0.8f);
            NuMtxTranslate(&target_matrix, &target_position);
            NuMtxPreScaleU(&target_matrix, machine->scale);
            const f32 target_alpha = machine->state_bit0 != 0 ? 0.0f : target_pulse;
            NuSpecialDrawAtAlpha(&world->lev_objs[85].special, &target_matrix, target_alpha);
        }

        // The animated machine body is evaluated in its own space and then
        // composed with the machine transform.
        if (animated_instance_animation != NULL) {
            if (machine->animation_state > 0) {
                machine->animation_time += FRAMETIME;
                if (machine->animation_state <= 3 && machine->animation_time >= 3.0f) {
                    machine->animation_state = 4;
                    machine->state_elapsed = 0.0f;
                    machine->state_duration = 2.0f;
                }
                animation_frame = machine->animation_time * animated_instance_animation->tfactor * 60.0f;
                if (animation_frame > animated_end_frame) {
                    animation_frame = animated_end_frame;
                }
            } else {
                machine->animation_time = 0.0f;
                if ((machine->flags & (HATMACHINE_FLAG_ANIMATING | HATMACHINE_FLAG_FINISHED |
                                       HATMACHINE_FLAG_ENABLED)) == HATMACHINE_FLAG_ENABLED) {
                    if (machine->idle_bounce_timer > 0.0f) {
                        machine->idle_bounce_timer -= FRAMETIME;
                        if (machine->idle_bounce_timer > 0.0f) {
                            const f32 idle_phase = machine->idle_bounce_timer / 0.15f * 50.0f + 16384.0f;
                            animation_frame =
                                (1.0f - NuFabs(NuTrigTable[(static_cast<i32>(idle_phase) >> 1) & 0x7fff])) * 1.5f +
                                16.0f;
                        }
                    } else if (qrand() <= 0x7ff) {
                        machine->idle_bounce_timer = 0.15f;
                    }
                }
            }

            NUMTX animated_matrix;
            EvalAnim(animated_special, animation_frame, &animated_matrix, 0);
            NuMtxMulVU0(&animated_matrix, &animated_matrix, &machine->transform);
            NuSpecialDrawAt(animated_special, &animated_matrix);
            if (world->lev_objs[machine->target_special_index].active != 0) {
                NuSpecialDrawAt(&world->lev_objs[machine->target_special_index].special, &animated_matrix);
            }
            if ((machine->flags & (HATMACHINE_FLAG_ANIMATING | HATMACHINE_FLAG_FINISHED | HATMACHINE_FLAG_ENABLED)) ==
                    HATMACHINE_FLAG_ENABLED &&
                world->lev_objs[41].active != 0) {
                NuSpecialDrawAtAlpha(&world->lev_objs[41].special, &animated_matrix, ready_alpha);
            }
        }

        // Draw the three optional animation effects in order.  The first
        // effect's translation supplies the hat's final placement.
        NUVEC draw_position = machine->position;
        NUMTX effect_matrix;
        if (effect_instance_animation_a != NULL) {
            f32 frame = machine->animation_time * animated_instance_animation->tfactor * 60.0f;
            if (frame > effect_end_frame_a)
                frame = effect_end_frame_a;
            EvalAnim(effect_special_a, frame, &effect_matrix, 0);
            NuMtxMulVU0(&effect_matrix, &effect_matrix, &machine->transform);
            NuSpecialDrawAt(effect_special_a, &effect_matrix);
            draw_position.x = effect_matrix.m30;
            draw_position.z = effect_matrix.m32;
        }
        if (effect_instance_animation_b != NULL) {
            f32 frame = machine->animation_time * animated_instance_animation->tfactor * 60.0f;
            if (frame > effect_end_frame_b)
                frame = effect_end_frame_b;
            EvalAnim(effect_special_b, frame, &effect_matrix, 0);
            NuMtxMulVU0(&effect_matrix, &effect_matrix, &machine->transform);
            NuSpecialDrawAt(effect_special_b, &effect_matrix);
        }
        if (effect_instance_animation_c != NULL) {
            f32 frame = machine->animation_time * animated_instance_animation->tfactor * 60.0f;
            if (frame > effect_end_frame_c)
                frame = effect_end_frame_c;
            EvalAnim(effect_special_c, frame, &effect_matrix, 0);
            NuMtxMulVU0(&effect_matrix, &effect_matrix, &machine->transform);
            NuSpecialDrawAt(effect_special_c, &effect_matrix);
        }

        if ((machine->flags & HATMACHINE_FLAG_ENABLED) != 0 && machine->displayed_hat != 0) {
            const i32 hat_index = machine->displayed_hat + 249;
            if (machine->animation_time < 2.35f && world->lev_objs[hat_index].active != 0) {
                const NUVEC *hat_offset = &HatMachine_HatOffset;
                NUVEC hat_position = *hat_offset;
                const f32 hat_phase = machine->hat_delay * 32768.0f + 16384.0f;
                const f32 hat_sine = NuTrigTable[(static_cast<i32>(hat_phase) >> 1) & 0x7fff];
                const f32 hat_scale = (hat_sine + 1.0f) * 0.5f;
                hat_position.y += (1.0f - hat_scale) * 0.1f;

                if (machine->animation_state <= 2) {
                    f32 bob_scale = 0.01f;
                    if (animation_frame != 0.0f) {
                        const f32 frame_fade = animation_frame / 50.0f;
                        bob_scale = frame_fade <= 1.0f ? (1.0f - frame_fade) * 0.01f : 0.0f;
                    }
                    hat_position.y +=
                        NuTrigTable[(static_cast<i32>(GameTimer.time_elapsed * 32768.0f) >> 1) & 0x7fff] * bob_scale;
                }

                NuVecRotateY(&hat_position, &hat_position, machine->yaw);
                NuMtxSetRotationY(&effect_matrix, machine->yaw + 0x8000);
                NuMtxTranslate(&effect_matrix, &draw_position);
                NuMtxTranslate(&effect_matrix, &hat_position);
                NuMtxPreScaleU(&effect_matrix, hat_scale);
                NuSpecialDrawAt(&world->lev_objs[hat_index].special, &effect_matrix);
            }
        }
    }

    ResetShadowMapRendering();
}

void HatMachines_InitTerrain(WORLDINFO_s *world) {
    if (world->hat_machine_sys != NULL) {
        for (i32 index = 0; index < world->hat_machine_sys->count; ++index) {
            HATMACHINE *machine = &world->hat_machine_sys->machines[index];
            machine->platform_id = NewPlatPickupInst(&machine->matrix, HATMACHINE_PLATFORM_COLLISION);
            PlatInstRotate(world->hat_machine_sys->machines[index].platform_id, 1);
        }
    }
}

MechObjectInterface *HATMACHINE_s::GetMechObjectInterface() {
    if (mech_object_interface == NULL) {
        new HatMachineObjectInterface(*this);
    }
    return mech_object_interface;
}

void HATMACHINE_s::ClearMechObjectInterface() {
    delete mech_object_interface;
}

HATMACHINE *HatMachine_FindNearest(WORLDINFO_s *world, nuvec_s *position, GameObject_s *object, float *distance) {
    if (world == NULL || world->hat_machine_sys == NULL) {
        return NULL;
    }

    f32 nearest_distance = 1.0e9f;
    HATMACHINE_s *nearest = NULL;
    if (world->hat_machine_sys->count > 0) {
        if (object != NULL) {
            for (i32 index = 0; index < world->hat_machine_sys->count; ++index) {
                HATMACHINE_s *machine = &world->hat_machine_sys->machines[index];
                if ((machine->flags & 0xf) != (HATMACHINE_FLAG_VISIBLE | HATMACHINE_FLAG_ENABLED) ||
                    machine->player_position.y == 2000000.0f) {
                    continue;
                }

                NUVEC target_position;
                Hat_GetAbsTargetPos(machine, &target_position);
                const f32 candidate_distance = NuVecDistSqr(position, &target_position, NULL);
                if (candidate_distance < nearest_distance) {
                    nearest_distance = candidate_distance;
                    nearest = machine;
                }
            }
        } else {
            for (i32 index = 0; index < world->hat_machine_sys->count; ++index) {
                HATMACHINE_s *machine = &world->hat_machine_sys->machines[index];
                const f32 candidate_distance = NuVecDistSqr(position, &machine->position, NULL);
                if (candidate_distance < nearest_distance) {
                    nearest_distance = candidate_distance;
                    nearest = machine;
                }
            }
        }
    }

    if (distance != NULL) {
        *distance = nearest_distance;
    }
    return nearest;
}

i32 HatMachine_BeingUsed(HATMACHINE_s *hat_machine) {
    return hat_machine->state_bit0;
}

ADDGIZMOTYPE *HatMachine_RegisterGizmo(i32 type_id) {
    static ADDGIZMOTYPE addtype;

    addtype = Default_ADDGIZMOTYPE;
    addtype.name = "HatMachine";
    addtype.prefix = "";
    addtype.fns.unknown1 = 0xc;
    addtype.fns.early_update_fn = NULL;
    addtype.fns.panel_draw_fn = NULL;
    addtype.fns.get_visibility_fn = NULL;
    addtype.fns.get_max_gizmos_fn = HatMachine_GetMaxGizmos;
    addtype.fns.get_pos_fn = NULL;
    addtype.fns.using_special_fn = NULL;
    addtype.fns.add_gizmos_fn = HatMachine_AddGizmos;
    addtype.fns.bolt_hit_plat_fn = NULL;
    addtype.fns.get_best_bolt_target_fn = NULL;
    addtype.fns.late_update_fn = HatMachine_Update;
    addtype.fns.bolt_hit_fn = NULL;
    addtype.fns.draw_fn = HatMachine_Draw;
    addtype.fns.get_gizmo_name_fn = HatMachine_GetGizmoName;
    addtype.fns.get_output_fn = HatMachine_GetOutput;
    addtype.fns.get_output_name_fn = HatMachine_GetOutputName;
    addtype.fns.get_num_outputs_fn = HatMachine_GetNumOutputs;
    addtype.fns.activate_fn = HatMachine_Activate;
    addtype.fns.activate_rev_fn = NULL;
    addtype.fns.set_visibility_fn = HatMachine_SetVisibility;
    addtype.fns.allocate_progress_data_fn = HatMachines_AllocateProgressData;
    addtype.fns.clear_progress_fn = HatMachines_ClearProgress;
    addtype.fns.store_progress_fn = HatMachines_StoreProgress;
    addtype.fns.reset_fn = HatMachines_Reset;
    addtype.fns.reserve_buffer_space_fn = HatMachines_ReserveBufferSpace;
    addtype.fns.load_fn = HatMachine_Load;
    addtype.fns.post_load_fn = NULL;
    addtype.fns.add_level_sfx_fn = NULL;
    hatmachine_gizmotype_id = type_id;

    return &addtype;
}

void HatMachine_MoveCode(WORLDINFO_s *world, GameObject_s *object, i32 special_pressed) {
    HATMACHINE_s *machine = static_cast<HATMACHINE_s *>(object->field_0x788);
    if (object->field_0xdb0 > 0.0f) {
        object->field_0xdb0 -= FRAMETIME;
    }

    if (object->character_context != 0x61 || machine == NULL) {
        if (object->apiobj.character_model->model_data_b[0x5d] == NULL || object->apiobj.field_0x27d == 0 ||
            ObjLandReady(object) == 0) {
            return;
        }

        f32 distance;
        machine = HatMachine_FindNearest(world, &object->apiobj.collision_position, object, &distance);
        if (machine == NULL) {
            return;
        }
        if (object == player) {
            show_hatmachine_hint = 0;
            if (distance < 1.0f) {
                show_hatmachine_hint = machine->current_hat;
            }
        }

        if (distance < (0.25f + object->apiobj.field_0x1dc) * machine->scale &&
            (special_pressed != 0 || (object->panel_use_request == 2 && object->big_jump_data != NULL))) {
            object->field_0x788 = machine;
            object->field_0x768 = 0.0f;
            object->delayed_turn_timer = 0.0f;
            object->apiobj.movement_facing_angle = machine->yaw;
            object->field_0xe21 &= ~0x10;
            object->character_context = 0x61;
            object->field_0x7a3 = 0;
            FastWeaponIn(object, 0);
            object->movement_runtime_flags |= 2;
            object->context_animation = 0x5d;
            machine->animation_duration = 3.0f;
            machine->animation_state = 1;
            const f32 end_frame = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
            machine->blink_timer = 0.0f;
            machine->flags = static_cast<HATMACHINE_FLAGS>(machine->flags | HATMACHINE_FLAG_ANIMATING);
            machine->state_elapsed = -end_frame;
            machine->animation_duration += end_frame;
            LoseHelmet(object, 0, 0);
            NewRumble(object->pad_gamepad->pad, 0.5f, 0);
            if (object->apiobj.anim_packet.blending != 0) {
                ResetAnimPacket(&object->apiobj.anim_packet, -1);
            }
            AlertSurroundingCreatures(object, &object->apiobj.collision_position);
            object->context_animation_timer =
                AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1);
            machine->animation_duration = object->context_animation_timer;
            if (object->context_animation_timer <= 0.0f) {
                object->context_animation_timer = 2.0f;
            }
            object->field_0xdb0 = 0.0f;
            return;
        }

        if (static_cast<i8>(object->apiobj.flags_low) < 0 &&
            (object->apiobj.character_data->model_flags & 0x20) != 0 && object->field_0xdb0 <= 0.0f) {
            PlaySfx(const_cast<char *>("TC14_VLN"), &object->apiobj.collision_position);
            object->field_0xdb0 = 0.5f;
        }
        return;
    }

    if (object->apiobj.character_model->model_data_b[object->context_animation] == NULL ||
        AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0) != NULL) {
        object->field_0x768 = MIN(object->field_0x768 + FRAMETIME, 1.0f);
        object->context_animation_timer -= FRAMETIME;

        if (object->field_0x7a3 == 1) {
            if (machine->animation_state != 4) {
                return;
            }
            if ((object->apiobj.character_data->game_character->flags_090 & 0x10) == 0 &&
                (object->id != id_PRINCESSLEIABOUSHH || FreePlay == 0)) {
                PlaySfx(const_cast<char *>("HatOn"), &object->apiobj.upper_position);
                object->field_0x108e = machine->current_hat;
                if (object->field_0x108e == 5) {
                    MakeBaddiesForgetAboutParty(1);
                }
                if (static_cast<i8>(object->apiobj.flags_low) < 0) {
                    if (machine->current_hat == 5) {
                        Hint_SetComplete(0x627);
                    } else if (machine->current_hat == 6) {
                        Hint_SetComplete(0x628);
                    }
                }
            }
            machine->current_hat = 0;
            object->field_0x7a3 = 2;
            machine->animation_state = 6;
        } else if (object->field_0x7a3 == 0) {
            if (object->context_animation_timer > 0.0f) {
                return;
            }
            const bool disguise_blocked =
                (object->apiobj.character_data->game_character->flags_090 & 0x10) == 0 &&
                (object->id != id_PRINCESSLEIABOUSHH || FreePlay == 0);
            if (object->apiobj.character_model->model_data_b[0x6d] == NULL || disguise_blocked) {
                if (object->apiobj.character_model->model_data_b[0x5e] == NULL) {
                    if (machine->animation_state != 2) {
                        return;
                    }
                    object->context_animation = 1;
                    object->field_0x7a3 = 1;
                    machine->animation_duration = 0.2f;
                    return;
                }
                if (machine->animation_state == 2) {
                    object->context_animation = 0x5e;
                    object->field_0x7a3 = 1;
                    object->context_animation_timer = AnimDuration(object->id, 0x5e, 0.0f, 0.0f, 1);
                    return;
                }
            } else if (machine->animation_state == 2) {
                object->context_animation = 0x6d;
                object->field_0x7a3 = 1;
                object->context_animation_timer = AnimDuration(object->id, 0x6d, 0.0f, 0.0f, 1);
                machine->animation_duration = object->context_animation_timer * 0.35f;
                return;
            }
            object->context_animation = 1;
            return;
        } else if (object->field_0x7a3 != 2) {
            return;
        }

        if (object->context_animation_timer > 0.0f) {
            return;
        }
        object->character_context = -1;
        object->context_animation_timer = 0.0f;
        machine = static_cast<HATMACHINE_s *>(object->field_0x788);
        if (machine == NULL) {
            return;
        }
        if (machine->configured_hat == 6) {
            PlaySfx(const_cast<char *>("Hunter_Granted"), &object->apiobj.collision_position);
        }
        if (machine->configured_hat == 5) {
            PlaySfx(const_cast<char *>("Trooper_Granted"), &object->apiobj.collision_position);
        }
    } else {
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer > 0.0f) {
            return;
        }
        machine = static_cast<HATMACHINE_s *>(object->field_0x788);
        if (machine == NULL) {
            NewRumble(object->pad_gamepad->pad, 0.5f, 0);
        } else {
            NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
            machine->hat_delay = 0.6f;
            if (machine->configured_hat == 6) {
                PlaySfx(const_cast<char *>("Hunter_Granted"), &object->apiobj.collision_position);
            }
            if (machine->configured_hat == 5) {
                PlaySfx(const_cast<char *>("Trooper_Granted"), &object->apiobj.collision_position);
            }
        }
        object->character_context = -1;
    }
    object->field_0x788 = NULL;
}
