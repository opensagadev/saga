// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "legoapi/gizmos/object/gizpanel.h"

#include "decomp.h"
#include "batman.h"
#include "gameapi/edtools/edfile.h"
#include "gamelib/util/gamelib_util_types.h"
#include "globals.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

#include <string.h>
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/world/mission.h"
#include "nu2api/numusic/sfx.h"
#include "legoapi/gizmo/base/GizPanelObjectInterface.h"
#include "legoapi/actions/movement/climbing.h"
#include "legoapi/ai/core/gameai.h"
#include "legoapi/characters/motion/move.h"
#include "legoapi/render/core/gameliball.h"

struct GIZPANELPROGRESS {
    i32 state;
    i32 goodie_state;
    i32 baddie_state;
};

i32 gizpanel_gizmotype_id = -1;
extern "C" {
    f32 GIZPANEL_PLAYERPOSLIFT = 0.001f;
    f32 GIZPANEL_PICY = 0.0775f;
    f32 GIZPANEL_PICZ = -0.123f;
}

void FindAnglesXY(NUVEC *direction, u16 *x_rotation, u16 *y_rotation);

static __used__ u8 storm_panel_active;
static __used__ u8 droid_panel_active;
static __used__ u8 bountyhunter_panel_active;

void FindAnglesZX(NUVEC *normal, u16 *x_rotation, u16 *z_rotation);
i32 SuperWeirdo(GameObject_s *object);
extern i16 id_GRANDMOFFTARKIN, id_IMPERIALOFFICER, id_IMPERIALSHUTTLEPILOT;

static i32 GizPanel_GetMaxGizmos(void *world_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    return world != NULL ? world->current_level->max_giz_panels : 0;
}

static char *GizPanel_GetGizmoName(GIZMO *gizmo) {
    if (gizmo != NULL && gizmo->object != NULL) {
        return static_cast<GIZPANEL *>(gizmo->object)->name;
    }
    return NULL;
}

static i32 GizPanel_GetOutput(GIZMO *gizmo, i32, i32) {
    GIZPANEL *panel = static_cast<GIZPANEL *>(gizmo->object);
    u8 flags = panel->flags;
    if ((flags & (GIZPANEL_FLAG_VISIBLE | GIZPANEL_FLAG_TRACK_PLAYER)) !=
        (GIZPANEL_FLAG_VISIBLE | GIZPANEL_FLAG_TRACK_PLAYER)) {
        return 0;
    }
    return panel->state;
}

static char *GizPanel_GetOutputName(GIZMO *, i32 output_index) {
    return output_index == 0 ? const_cast<char *>("Finished") : NULL;
}

static i32 GizPanel_GetNumOutputs(GIZMO *) {
    return 1;
}

static i32 GizPanel_Load(void *world_ptr, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (world == NULL) {
        return 0;
    }
    GIZPANELSYS_s *panel_sys = world->giz_panel_sys;
    if (panel_sys == NULL || panel_sys->count != 0) {
        return 0;
    }
    const i32 version = EdFileReadInt();
    i32 index = 0;
    panel_sys->count = EdFileReadInt();
    if (panel_sys->count <= 0) {
        return 1;
    }
    do {
        const i32 name_length = EdFileReadInt();
        EdFileRead(panel_sys->panels[index].name, name_length);
        EdFileReadNuVec(&panel_sys->panels[index].position);
        GIZPANEL &yaw_panel = panel_sys->panels[index];
        yaw_panel.y_rotation = EdFileReadUnsignedShort();
        GIZPANEL &variant_panel = panel_sys->panels[index];
        variant_panel.model_variant = EdFileReadChar();

        if (version <= 2) {
            GIZPANEL *panel = &panel_sys->panels[index];
            panel->target_offset.x = 0.0f;
            panel->target_offset.y = 0.0f;
            panel->target_offset.z = -0.4f;
            panel->target_scale = 1.0f;
        } else {
            GIZPANEL &panel = panel_sys->panels[index];
            const u8 hide_base = static_cast<u8>(EdFileReadChar()) & 1;
            panel.flags = static_cast<GIZPANEL_FLAGS>((panel.flags & ~GIZPANEL_FLAG_HIDE_BASE) | (hide_base << 5));
            if (version != 3) {
                EdFileReadNuVec(&panel_sys->panels[index].target_offset);
                GIZPANEL &scale_panel = panel_sys->panels[index];
                scale_panel.target_scale = EdFileReadFloat();
                if (version != 4) {
                    GIZPANEL &current_panel = panel_sys->panels[index];
                    const u8 hide_target = static_cast<u8>(EdFileReadChar()) & 1;
                    current_panel.flags = static_cast<GIZPANEL_FLAGS>(
                        (current_panel.flags & ~GIZPANEL_FLAG_HIDE_TARGET) | (hide_target << 6));
                    if (version != 5) {
                        GIZPANEL &baddie_panel = panel_sys->panels[index];
                        const i8 baddie = EdFileReadChar();
                        baddie_panel.flags = static_cast<GIZPANEL_FLAGS>((baddie_panel.flags & ~GIZPANEL_FLAG_BADDIE) |
                                                                         (static_cast<u32>(baddie) << 7));
                        GIZPANEL &model_panel = panel_sys->panels[index];
                        const u8 alt_model = static_cast<u8>(EdFileReadChar()) & 1;
                        model_panel.draw_flags = static_cast<GIZPANEL_DRAW_FLAGS>(
                            (model_panel.draw_flags & ~GIZPANEL_DRAW_FLAG_ALT_MODEL) | alt_model);
                        if (version != 6) {
                            GIZPANEL &shadow_panel = panel_sys->panels[index];
                            const u8 no_shadow_reset = static_cast<u8>(EdFileReadChar()) & 1;
                            shadow_panel.draw_flags = static_cast<GIZPANEL_DRAW_FLAGS>(
                                (shadow_panel.draw_flags & ~GIZPANEL_DRAW_FLAG_NO_SHADOW_RESET) |
                                (no_shadow_reset << 1));
                            if (version != 7) {
                                GIZPANEL &legacy_panel = panel_sys->panels[index];
                                const bool legacy = EdFileReadChar() != 0;
                                legacy_panel.draw_flags = static_cast<GIZPANEL_DRAW_FLAGS>(
                                    (legacy_panel.draw_flags & ~GIZPANEL_DRAW_FLAG_LEGACY) |
                                    (legacy ? GIZPANEL_DRAW_FLAG_LEGACY : 0));
                            }
                        }
                    }
                }
            } else {
                GIZPANEL *default_panel = &panel_sys->panels[index];
                default_panel->target_offset.x = 0.0f;
                default_panel->target_offset.y = 0.0f;
                default_panel->target_offset.z = -0.4f;
                default_panel->target_scale = 1.0f;
            }
        }
        GIZPANEL &panel = panel_sys->panels[index];
        if (version <= 7) {
            panel.draw_flags = static_cast<GIZPANEL_DRAW_FLAGS>(panel.draw_flags & ~GIZPANEL_DRAW_FLAG_LEGACY);
        }
        ++index;
    } while (index < panel_sys->count);
    return 1;
}

static void *GizPanels_ReserveBufferSpace(void *world_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    world->giz_panel_sys = NULL;
    if (world->current_level->max_levers != 0) {
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 16);
        world->giz_panel_sys = static_cast<GIZPANELSYS_s *>(world->giz_buffer.void_ptr);
        world->giz_buffer.addr += sizeof(GIZPANELSYS_s);
        memset(world->giz_panel_sys, 0, sizeof(*world->giz_panel_sys));

        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 16);
        world->giz_panel_sys->panels = static_cast<GIZPANEL *>(world->giz_buffer.void_ptr);
        world->giz_buffer.addr += world->current_level->max_giz_panels * sizeof(GIZPANEL);
        memset(world->giz_panel_sys->panels, 0, world->current_level->max_giz_panels * sizeof(GIZPANEL));
    }
    return world->giz_panel_sys;
}

static void GizPanels_ClearProgress(void *, void *progress_data) {
    GIZPANELPROGRESS *progress = (GIZPANELPROGRESS *)progress_data;
    if (progress == NULL) {
        return;
    }

    progress->state = 0;
    progress->goodie_state = -1;
    progress->baddie_state = -1;
}

static void *GizPanels_AllocateProgressData(VARIPTR *buffer, VARIPTR *buffer_end) {
    return GizmoBufferAlloc(buffer, buffer_end, sizeof(GIZPANELPROGRESS));
}

static __attribute__((noinline)) void GizPanel_CreateTerrain(GIZPANEL *panel) {
    if (panel == NULL) {
        return;
    }
    if (panel->platform_id != -1) {
        DeletePlatinst(panel->platform_id);
    }

    switch (panel->model_variant) {
        case 0:
            panel->platform_id = NewPlatPickupInst(panel, 4);
            break;
        case 1:
            panel->platform_id = NewPlatPickupInst(panel, 7);
            break;
        case 2:
            panel->platform_id = NewPlatPickupInst(panel, 5);
            break;
        case 3:
            panel->platform_id = NewPlatPickupInst(panel, 6);
            break;
        default:
            break;
    }
    PlatInstRotate(panel->platform_id, 1);
}

static void GizPanel_SetVisibility(GIZMO *gizmo, i32 visible) {
    if (gizmo == NULL || gizmo->object == NULL) {
        return;
    }

    GIZPANEL *panel = static_cast<GIZPANEL *>(gizmo->object);
    const i32 was_visible = panel->visible;
    panel->visible = visible != 0;
    if (panel->visible != 0) {
        if (was_visible == 0) {
            GizPanel_CreateTerrain(panel);
        }
    } else if (was_visible != 0) {
        DeletePlatinst(panel->platform_id);
    }
}

static void GizPanel_Update(void *world_ptr, void *, float delta_time) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    GIZPANELSYS_s *panel_sys = world->giz_panel_sys;
    if (panel_sys == NULL) {
        return;
    }

    GameObject_s **players = Player;
    GameObject_s *nearest_player = NULL;
    NUVEC player_direction;
    NUVEC panel_position;
    NUVEC direction;
    for (i32 index = 0; index < panel_sys->count; ++index) {
        GIZPANEL &panel = panel_sys->panels[index];

        if (__builtin_expect((panel.flags & GIZPANEL_FLAG_TRACK_PLAYER) != 0, 0)) {
            const f32 maximum_distance = (panel.flags & GIZPANEL_FLAG_PLAYER_NEAR) != 0 ? 1.1f * 1.1f : 0.9f * 0.9f;
            f32 nearest_distance = 1000000000.0f;
#define CONSIDER_PANEL_PLAYER(player_index)                                                                            \
    do {                                                                                                               \
        GameObject_s *candidate = players[player_index];                                                               \
        if (__builtin_expect(candidate != NULL && (candidate->apiobj.flags_high & 0x10) != 0 && candidate->apiobj.field_0x287 == 0, 0)) {   \
            panel_position = panel.position;                                                                           \
            if (panel.model_variant == 2) {                                                                            \
                panel_position.y += 0.245f;                                                                            \
                panel_position.z -= 0.19f;                                                                             \
            } else if (panel.model_variant == 3) {                                                                     \
                panel_position.y += 0.34f;                                                                             \
                panel_position.z -= 0.17f;                                                                             \
            }                                                                                                          \
            const f32 distance = NuVecDistSqr(&panel_position, &candidate->apiobj.collision_position, &direction);     \
            if (__builtin_expect(distance < nearest_distance && distance < maximum_distance, 0)) {                                          \
                nearest_distance = distance;                                                                           \
                nearest_player = players[player_index];                                                                \
                player_direction = direction;                                                                          \
            } else if (player_index == 0) {                                                                            \
                nearest_player = NULL;                                                                                 \
                nearest_distance = 1000000000.0f;                                                                      \
            }                                                                                                          \
        } else if (player_index == 0) {                                                                                \
            nearest_player = NULL;                                                                                     \
            nearest_distance = 1000000000.0f;                                                                          \
        }                                                                                                              \
    } while (0)

            CONSIDER_PANEL_PLAYER(0);
            CONSIDER_PANEL_PLAYER(1);
            CONSIDER_PANEL_PLAYER(2);
            CONSIDER_PANEL_PLAYER(3);
            CONSIDER_PANEL_PLAYER(4);
            CONSIDER_PANEL_PLAYER(5);
            CONSIDER_PANEL_PLAYER(6);
            CONSIDER_PANEL_PLAYER(7);

#undef CONSIDER_PANEL_PLAYER
        }

        if (nearest_player == NULL) {
            panel.target_x_rotation = SeekRot(panel.target_x_rotation, 0xe000, 10.0f);
            panel.target_y_rotation = SeekRot(panel.target_y_rotation, 0, 10.0f);
            if (__builtin_expect((panel.flags & GIZPANEL_FLAG_PLAYER_NEAR) != 0, 0)) {
                PlaySfx(const_cast<char *>("DPanelDis"), &panel.position);
            }
            panel.flags = static_cast<GIZPANEL_FLAGS>(panel.flags & ~GIZPANEL_FLAG_PLAYER_NEAR);
            if (panel.activation_time > 0.0f) {
                panel.activation_time -= delta_time;
                if (panel.activation_time < 0.0f) {
                    panel.activation_time = 0.0f;
                }
            }
        } else {
            const u16 previous_x_rotation = panel.target_x_rotation;
            const u16 previous_y_rotation = panel.target_y_rotation;
            u16 target_x_rotation;
            u16 target_y_rotation;
            FindAnglesXY(&player_direction, &target_x_rotation, &target_y_rotation);
            target_y_rotation = static_cast<u16>(target_y_rotation - panel.y_rotation);

            if (__builtin_expect(!(previous_y_rotation <= 0x4000), 0)) {
                if (previous_y_rotation >= 0xc000 && target_y_rotation <= 0xbfff && target_y_rotation >= 0x4001) {
                    target_y_rotation = 0xc000;
                }
            } else {
                if (target_y_rotation > 0x4000 && target_y_rotation < 0xc000) {
                    target_y_rotation = 0x4000;
                }
            }
            if (previous_x_rotation <= 0x4000) {
                if (__builtin_expect(target_x_rotation > 0x4000 && target_x_rotation < 0xc000, 0)) {
                    target_x_rotation = static_cast<u16>(panel.y_rotation + 0x4000);
                }
            } else if (previous_x_rotation > 0xbfff && target_x_rotation < 0xc000 && target_x_rotation > 0x4000) {
                target_x_rotation = 0xc000;
            }

            panel.target_x_rotation = SeekRot(panel.target_x_rotation, target_x_rotation, 5.0f);
            panel.target_y_rotation = SeekRot(panel.target_y_rotation, target_y_rotation, 5.0f);
            if ((panel.flags & GIZPANEL_FLAG_PLAYER_NEAR) == 0) {
                panel.activation_time = 0.0f;
                panel.flags = static_cast<GIZPANEL_FLAGS>(panel.flags | GIZPANEL_FLAG_PLAYER_NEAR);
                PlaySfx(const_cast<char *>("DPanelAct"), &panel.position);
            } else {
                panel.activation_time += FRAMETIME;
                if (panel.activation_time > 0.8f) {
                    panel.activation_time = 0.8f;
                }
            }
        }
    }
}

static void GizPanel_AddGizmos(GIZMOSYS *gizmo_sys, i32 type_id, void *world_ptr, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    GIZPANELSYS_s *panel_sys;
    if (world != NULL && (panel_sys = world->giz_panel_sys) != NULL && panel_sys->count > 0) {
        i32 index = 0;
        do {
            if (NuStrLen(panel_sys->panels[index].name) != 0) {
                AddGizmo(gizmo_sys, type_id, NULL, &world->giz_panel_sys->panels[index]);
            }
            panel_sys = world->giz_panel_sys;
            ++index;
        } while (panel_sys->count > index);
    }
}

static void GizPanels_StoreProgress(void *world_ptr, void *, void *progress_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    GIZPANELPROGRESS *progress = static_cast<GIZPANELPROGRESS *>(progress_ptr);
    if (progress == NULL) {
        return;
    }

    progress->state = 0;
    progress->goodie_state = -1;
    progress->baddie_state = -1;
    GIZPANELSYS_s *panel_sys;
    GIZPANEL *panel;
    if (world != NULL && (panel_sys = world->giz_panel_sys) != NULL && (panel = panel_sys->panels) != NULL &&
        panel_sys->count > 0) {
        i32 index = 0;
        auto panel_sys_count = panel_sys->count;
        do {
            const i32 word_index = index >> 5;
            const u32 bit = 1u << (index & 31);
            u8 flags = panel->flags;
            if ((flags & GIZPANEL_FLAG_STATE) != 0) {
                reinterpret_cast<u32 *>(&progress->state)[word_index] |= bit;
                flags = panel->flags;
            }
            if ((flags & GIZPANEL_FLAG_VISIBLE) == 0) {
                reinterpret_cast<u32 *>(&progress->baddie_state)[word_index] &= ~bit;
                flags = panel->flags;
            }
            if ((flags & GIZPANEL_FLAG_TRACK_PLAYER) == 0) {
                reinterpret_cast<u32 *>(&progress->goodie_state)[word_index] &= ~bit;
            }
            ++index;
            ++panel;
        } while (index != panel_sys_count && index != 32);
    }
}

void GizPanel_InitTerrain(WORLDINFO_s *world) {
    GIZPANELSYS_s *panel_sys = world->giz_panel_sys;
    if (panel_sys == NULL || panel_sys->count <= 0) {
        return;
    }

    for (i32 index = 0; index < panel_sys->count; ++index) {
        GIZPANEL_s *panel = &panel_sys->panels[index];
        panel->platform_id = -1;
        GizPanel_CreateTerrain(panel);
        panel_sys = world->giz_panel_sys;
    }
}

void GizPanel_GetAbsTargetPos(GIZPANEL_s *panel, nuvec_s *target_position, i32 player_position) {
    if (target_position == NULL || panel == NULL) {
        return;
    }

    NUVEC offset;
    if (player_position != 0) {
        if (panel->model_variant == 0) {
            offset = {-0.04f, panel->target_offset.y, -0.3f};
        } else if (panel->model_variant == 1) {
            offset = {0.035f, panel->target_offset.y, -0.25f};
        } else {
            offset = panel->target_offset;
        }
    } else {
        offset = panel->target_offset;
    }

    NuVecRotateY(&offset, &offset, panel->y_rotation);
    offset.x += panel->position.x;
    offset.z += panel->position.z;
    *target_position = offset;
}

static void GizPanel_Draw(void *world_ptr, void *, float) {
    struct PANEL_MODELS {
        i32 base;
        i32 animated_transition;
        i32 animated;
        i32 state_off;
        i32 target;
        i32 arm;
        i32 state_on;
        NUVEC arm_offset;
    };

    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    GIZPANELSYS_s *panel_sys;
    if (__builtin_expect(world == NULL || (panel_sys = world->giz_panel_sys) == NULL || panel_sys->count == 0, 0)) {
        return;
    }

    const u16 target_spin = static_cast<u16>(NuFmod(GameTimer.time_elapsed, 5.0f) / 5.0f * 65536.0f);
    const f32 pulse_phase = NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f) * 2.0f * 65536.0f;
    const f32 target_alpha = 0.8f + NuTrigTable[(static_cast<i32>(pulse_phase) >> 1) & 0x7fff] * 0.2f;

    for (i32 index = 0; index < panel_sys->count; ++index) {
        GIZPANEL &panel = panel_sys->panels[index];
        if ((panel.flags & GIZPANEL_FLAG_VISIBLE) == 0 && editor_active == 0) {
            continue;
        }

        TouchHacks::TintStack tint;
        panel.flash_timer -= FRAMETIME;
        const bool flashing = TouchHacks::ShouldFlash(panel.flash_timer);
        if (flashing) {
            NUCOLOUR3 *flash_colour = TouchHacks::GetFlashColour();
            NuRndrLightingStateCurrent.ambient = *flash_colour;
            NuRndrSetAmbientLightPS(flash_colour);
            NUVEC special_tint = {0.5f, 0.5f, 2.0f};
            NuSpecialConstTint(1, &special_tint);
        }

        if ((panel.draw_flags & GIZPANEL_DRAW_FLAG_NO_SHADOW_RESET) == 0) {
            EnableShadowMapRendering(0);
        } else {
            ResetShadowMapRendering();
        }

        PANEL_MODELS models = {-1, -1, -1, -1, -1, -1, -1, {0.0f, 0.0f, 0.0f}};
        const bool alternate = (panel.draw_flags & GIZPANEL_DRAW_FLAG_ALT_MODEL) != 0;
        switch (panel.model_variant) {
            case 0:
                models.base = alternate ? 0x107 : 0x106;
                models.animated = (panel.flags & GIZPANEL_FLAG_BADDIE) != 0 ? 0x109 : 0x108;
                models.animated_transition = 0x10a;
                models.state_off = 0x10c;
                models.state_on = 0x10b;
                models.target = 0x55;
                break;
            case 1:
                models.base = alternate ? 0x107 : 0x100;
                models.animated = (panel.flags & GIZPANEL_FLAG_BADDIE) != 0 ? 0x102 : 0x101;
                models.animated_transition = 0x103;
                models.state_off = 0x105;
                models.state_on = 0x104;
                models.target = 0x55;
                break;
            case 2:
                models.base = 0x10d;
                models.animated = 0x10e;
                models.animated_transition = 0x10f;
                models.state_off = 0x111;
                models.state_on = 0x110;
                models.target = 0x113;
                models.arm = 0x112;
                models.arm_offset = {0.0f, 0.245f, -0.19f};
                break;
            case 3:
                models.base = 0x114;
                models.animated = 0x115;
                models.animated_transition = 0x116;
                models.state_off = 0x118;
                models.state_on = 0x117;
                models.target = 0x11a;
                models.arm = 0x119;
                models.arm_offset = {0.0f, 0.34f, -0.17f};
                break;
            default:
                break;
        }

        NUMTX base_matrix;
        NuMtxSetRotationY(&base_matrix, panel.y_rotation);
        NuMtxTranslate(&base_matrix, &panel.position);
        if ((panel.flags & GIZPANEL_FLAG_HIDE_BASE) == 0 && models.base != -1 &&
            world->lev_objs[models.base].active != 0) {
            NuSpecialDrawAt(&world->lev_objs[models.base].special, &base_matrix);
        }

        const i32 state_model = (panel.flags & GIZPANEL_FLAG_STATE) != 0 ? models.state_on : models.state_off;
        if ((panel.flags & GIZPANEL_FLAG_HIDE_BASE) == 0 && state_model != -1 &&
            world->lev_objs[state_model].active != 0) {
            NuSpecialDrawAt(&world->lev_objs[state_model].special, &base_matrix);
        }

        if ((panel.flags & GIZPANEL_FLAG_HIDE_BASE) == 0 && (panel.activation_time > 0.0f || editor_active != 0) &&
            models.animated != -1) {
            NUVEC picture_offset = {0.0f, GIZPANEL_PICY, GIZPANEL_PICZ};
            NUVEC transition_offset = {0.0f, GIZPANEL_PICY, GIZPANEL_PICZ - 0.0005f};
            NuVecRotateY(&picture_offset, &picture_offset, panel.y_rotation);
            NuVecRotateY(&transition_offset, &transition_offset, panel.y_rotation);

            NUMTX transition_base = base_matrix;
            NuMtxTranslate(&base_matrix, &picture_offset);
            NuMtxTranslate(&transition_base, &transition_offset);

            NUMTX animated_matrix;
            if (panel.activation_time < 1.2f && editor_active == 0) {
                const f32 transition_progress = panel.activation_time / 1.2f;
                EvalAnim(&world->lev_objs[models.animated_transition].special, transition_progress * 23.0f + 1.0f,
                         &animated_matrix, 0);
                animated_matrix.m30 = 0.0f;
                animated_matrix.m31 = 0.0f;
                animated_matrix.m32 = 0.0f;
                NuMtxMulVU0(&animated_matrix, &animated_matrix, &transition_base);
                NuSpecialDrawAtAlpha(&world->lev_objs[models.animated_transition].special, &animated_matrix,
                                     1.0f - transition_progress);
                EvalAnim(&world->lev_objs[models.animated].special, panel.activation_time / 0.8f * 24.0f + 1.0f,
                         &animated_matrix, 0);
                animated_matrix.m30 = 0.0f;
                animated_matrix.m31 = 0.0f;
                animated_matrix.m32 = 0.0f;
                NuMtxMulVU0(&animated_matrix, &animated_matrix, &base_matrix);
                NuSpecialDrawAtAlpha(&world->lev_objs[models.animated].special, &animated_matrix, 1.0f);
            } else {
                EvalAnim(&world->lev_objs[models.animated].special, panel.activation_time / 0.8f * 24.0f + 1.0f,
                         &animated_matrix, 0);
                animated_matrix.m30 = 0.0f;
                animated_matrix.m31 = 0.0f;
                animated_matrix.m32 = 0.0f;
                NuMtxMulVU0(&animated_matrix, &animated_matrix, &base_matrix);
                NuSpecialDrawAtAlpha(&world->lev_objs[models.animated].special, &animated_matrix, 1.0f);
            }
        }

        if ((panel.flags & (GIZPANEL_FLAG_TRACK_PLAYER | GIZPANEL_FLAG_HIDE_TARGET)) == GIZPANEL_FLAG_TRACK_PLAYER &&
            panel.target_offset.y != 2000000.0f && models.target != -1 && world->lev_objs[models.target].active != 0) {
            NUVEC target_position;
            GizPanel_GetAbsTargetPos(&panel, &target_position, 0);
            NuMtxSetRotationY(&base_matrix, target_spin);
            if (panel.target_roll != 0) {
                NuMtxRotateZ(&base_matrix, panel.target_roll);
            }
            if (panel.target_pitch != 0) {
                NuMtxRotateX(&base_matrix, panel.target_pitch);
            }
            NuMtxTranslate(&base_matrix, &target_position);
            NuMtxPreScaleU(&base_matrix, panel.target_scale);
            NuSpecialDrawAtAlpha(&world->lev_objs[models.target].special, &base_matrix,
                                 (panel.flags & 1) != 0 ? 0.0f : target_alpha);
        }

        if ((panel.flags & GIZPANEL_FLAG_HIDE_BASE) == 0 && models.arm != -1 &&
            world->lev_objs[models.arm].active != 0) {
            NUVEC arm_offset = models.arm_offset;
            NuVecRotateX(&arm_offset, &arm_offset, panel.arm_x_rotation);
            NuVecRotateY(&arm_offset, &arm_offset, panel.y_rotation);
            NuMtxSetRotationX(&base_matrix, panel.arm_x_rotation);
            NuMtxRotateY(&base_matrix, panel.y_rotation);
            NuMtxPreRotateX(&base_matrix, panel.target_x_rotation);
            NuMtxRotateY(&base_matrix, panel.target_y_rotation);
            NuMtxTranslate(&base_matrix, &panel.position);
            NuMtxTranslate(&base_matrix, &arm_offset);
            NuSpecialDrawAt(&world->lev_objs[models.arm].special, &base_matrix);
        }
        if (flashing) {
            NuSpecialConstTint(0, NULL);
        }
    }
    ResetShadowMapRendering();
}

void GizPanel_GetAbsPlayerPos(GIZPANEL_s *panel, nuvec_s *position) {
    if (position != NULL) {
        if (panel != NULL && static_cast<u8>(panel->model_variant - 2) <= 1)
            GizPanel_GetAbsTargetPos(panel, position, 0);
        else
            *position = panel->floor_position;
    }
}

void GizPanel_Reset(GIZPANEL_s *panel) {
    NUVEC *floor_position = &panel->floor_position;
    floor_position->x = 0.0f;
    floor_position->z = 0.4f;
    floor_position->y = 0.0f;
    NuVecRotateY(floor_position, floor_position, panel->y_rotation + 0x8000);
    NuVecAdd(floor_position, floor_position, &panel->position);

    NUVEC target_position;
    GizPanel_GetAbsTargetPos(panel, &target_position, 0);
    target_position.y = panel->position.y;
    floor_position->y = GameShadow(NULL, floor_position, 0.2f, -1);
    const f32 target_floor = GameShadow(NULL, &target_position, 0.2f, -1);
    panel->target_offset.y = target_floor;
    if (target_floor != 2000000.0f) {
        panel->target_offset.y = target_floor + GIZPANEL_PLAYERPOSLIFT;
        FindAnglesZX(&ShadNorm, &panel->target_pitch, &panel->target_roll);
    }

    panel->activation_time = 0.0f;
    panel->arm_x_rotation = 0;
    panel->flags =
        static_cast<GIZPANEL_FLAGS>((panel->flags & 0xfc) | GIZPANEL_FLAG_VISIBLE | GIZPANEL_FLAG_TRACK_PLAYER);
    NuMtxSetRotationY(&panel->matrix, panel->y_rotation);
    NuMtxTranslate(&panel->matrix, &panel->position);
}

static void GizPanels_Reset(void *world_ptr, void *, void *progress_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    GIZPANELPROGRESS *progress = static_cast<GIZPANELPROGRESS *>(progress_ptr);
    GIZPANELSYS_s *panel_sys;
    GIZPANEL *panels;
    if (world != NULL && (panel_sys = world->giz_panel_sys) != NULL && (panels = panel_sys->panels) != NULL &&
        panel_sys->count > 0) {
        i32 index = 0;
        while (true) {
            GIZPANEL *panel = &panels[index];
            GizPanel_Reset(panel);
            if (index < 32 && progress != NULL) {
                const u32 bit = 1u << index;
                if ((progress->state & bit) != 0) {
                    panel->state = 1;
                }
                panel->visible = (progress->baddie_state & bit) != 0;
                panel->track_player = (progress->goodie_state & bit) != 0;
            }
            ++index;
            if (panel_sys->count <= index) {
                break;
            }
            panels = panel_sys->panels;
        }
    }
}

static void GizPanel_Activate(GIZMO *gizmo, i32 active) {
    if (gizmo != NULL && gizmo->object != NULL) {
        GIZPANEL *panel = static_cast<GIZPANEL *>(gizmo->object);
        if (active == 0) {
            panel->flags = static_cast<GIZPANEL_FLAGS>(panel->flags & ~GIZPANEL_FLAG_TRACK_PLAYER);
            return;
        }
        panel->flags = static_cast<GIZPANEL_FLAGS>(panel->flags | GIZPANEL_FLAG_TRACK_PLAYER);
        GizPanel_Reset(panel);
    }
}

i32 GizPanel_CanUsePanel(GameObject_s *object, GIZPANEL_s *panel) {
    if (panel == NULL || object == NULL)
        return 0;
    if (SuperWeirdo(object) != 0)
        return 1;
    switch (panel->model_variant) {
        case 0:
            return (object->apiobj.character_data->model_flags & 0x40) != 0 ||
                   (object->apiobj.character_data->model_flags & 0x1000010) == 0x1000010;
        case 1:
            return (object->apiobj.character_data->model_flags & 0x20) != 0 ||
                   (object->apiobj.character_data->model_flags & 0x1000010) == 0x1000010;
        case 2:
            return (object->apiobj.character_data->model_flags & 0x1000000) != 0 || object->field_0x108e == 6;
        case 3:
            if (object->field_0x108e == 5 ||
                (static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_090 & 0x40000) !=
                    0)
                return 1;
            if (GCDataList[object->id].uses_weapon_action == 8 || GCDataList[object->id].uses_weapon_action == 1)
                return 1;
            return object->id == id_DARTHVADER || object->id == id_THEEMPEROR || object->id == id_GRANDMOFFTARKIN ||
                   object->id == id_IMPERIALOFFICER || object->id == id_IMPERIALSHUTTLEPILOT;
        default:
            return 0;
    }
}

MechObjectInterface *GIZPANEL_s::GetMechObjectInterface() {
    if (mech_object_interface == NULL) {
        new GizPanelObjectInterface(*this);
    }
    return mech_object_interface;
}

void GIZPANEL_s::ClearMechObjectInterface() {
    delete mech_object_interface;
}

GIZPANEL_s *GizPanel_FindByName(WORLDINFO_s *world, char *name) {
    if (world != NULL && world->giz_panel_sys != NULL) {
        GIZPANEL_s *panel = world->giz_panel_sys->panels;
        for (i32 index = 0; index < world->giz_panel_sys->count; ++index, ++panel) {
            if (NuStrICmp(panel->name, name) == 0)
                return panel;
        }
    }
    return NULL;
}

GIZPANEL_s *GizPanel_FindNearest(WORLDINFO_s *world, nuvec_s *position, GameObject_s *object, float *distance_squared,
                                 i32 check_eligibility) {
    if (world == NULL || world->giz_panel_sys == NULL)
        return NULL;
    GIZPANEL_s *nearest = NULL;
    f32 nearest_distance = 1.0e9f;
    for (i32 index = 0; index < world->giz_panel_sys->count; ++index) {
        GIZPANEL_s *panel = &world->giz_panel_sys->panels[index];
        if (object != NULL) {
            if (panel->reserved_flag == 0 && panel->state == 0 && panel->visible && panel->track_player &&
                panel->floor_position.y != 2000000.0f &&
                (check_eligibility == 0 || GizPanel_CanUsePanel(object, panel) != 0)) {
                NUVEC target;
                GizPanel_GetAbsTargetPos(panel, &target, 0);
                const f32 distance = NuVecDistSqr(position, &target, NULL);
                if (distance < nearest_distance) {
                    nearest_distance = distance;
                    nearest = panel;
                }
            }
        } else {
            const f32 distance = NuVecDistSqr(position, &panel->position, NULL);
            if (distance < nearest_distance) {
                nearest_distance = distance;
                nearest = panel;
            }
        }
    }
    if (distance_squared != NULL)
        *distance_squared = nearest_distance;
    return nearest;
}

i32 GizPanel_BeingUsed(GIZPANEL_s *panel) {
    return panel->flags & 1;
}

ADDGIZMOTYPE *GizPanel_RegisterGizmo(i32 type_id) {
    static ADDGIZMOTYPE addtype;

    addtype = Default_ADDGIZMOTYPE;

    addtype.name = "Panel";
    addtype.prefix = "";
    addtype.fns.unknown1 = 0xc;
    addtype.fns.get_max_gizmos_fn = GizPanel_GetMaxGizmos;
    addtype.fns.add_gizmos_fn = GizPanel_AddGizmos;
    addtype.fns.early_update_fn = NULL;
    addtype.fns.late_update_fn = GizPanel_Update;
    addtype.fns.draw_fn = GizPanel_Draw;
    addtype.fns.panel_draw_fn = NULL;
    addtype.fns.get_gizmo_name_fn = GizPanel_GetGizmoName;
    addtype.fns.get_output_fn = GizPanel_GetOutput;
    addtype.fns.get_output_name_fn = GizPanel_GetOutputName;
    addtype.fns.get_num_outputs_fn = GizPanel_GetNumOutputs;
    addtype.fns.activate_fn = GizPanel_Activate;
    addtype.fns.activate_rev_fn = NULL;
    addtype.fns.set_visibility_fn = GizPanel_SetVisibility;
    addtype.fns.get_visibility_fn = NULL;
    addtype.fns.get_pos_fn = NULL;
    addtype.fns.using_special_fn = NULL;
    addtype.fns.bolt_hit_plat_fn = NULL;
    addtype.fns.get_best_bolt_target_fn = NULL;
    addtype.fns.bolt_hit_fn = NULL;
    addtype.fns.allocate_progress_data_fn = GizPanels_AllocateProgressData;
    addtype.fns.clear_progress_fn = GizPanels_ClearProgress;
    addtype.fns.store_progress_fn = GizPanels_StoreProgress;
    addtype.fns.reset_fn = GizPanels_Reset;
    addtype.fns.reserve_buffer_space_fn = GizPanels_ReserveBufferSpace;
    addtype.fns.load_fn = GizPanel_Load;
    addtype.fns.post_load_fn = NULL;
    addtype.fns.add_level_sfx_fn = NULL;
    gizpanel_gizmotype_id = type_id;

    return &addtype;
}

void GizPanel_PlaySfx(char *name, nuvec_s *position, i32 player_bits) {
    if (position == NULL || name == NULL)
        return;
    const i16 sfx_id = static_cast<i16>(GetSfxId(name));
    if (sfx_id == -1)
        return;
    if (player_bits == 0) {
        PlaySfxById(sfx_id, position);
    } else {
        if ((player_bits & 1) != 0) {
            nusound_special_positions[1] = *position;
            PlaySfxById(sfx_id, &nusound_special_positions[1]);
            nusound_special_positions[1] = nusound_special_positions[0];
        }
        if ((player_bits & 2) != 0) {
            nusound_special_positions[2] = *position;
            PlaySfxById(sfx_id, &nusound_special_positions[2]);
            nusound_special_positions[2] = nusound_special_positions[0];
        }
    }
}

void GizPanel_Use(GameObject_s &object, GIZPANEL_s &panel) {
    u16 angle = panel.y_rotation;
    object.field_0x788 = &panel;
    object.field_0x768 = 0.0f;
    object.delayed_turn_timer = 0.0f;
    object.apiobj.movement_facing_angle = angle;
    object.field_0xe21 &= ~0x10;
    object.character_context = 0x0b;
    object.field_0x7a3 = 0;
    FastWeaponIn(&object, 0);
    object.movement_runtime_flags |= 2;
    GIZPANEL_s *active_panel = static_cast<GIZPANEL_s *>(object.field_0x788);
    if (active_panel->model_variant == 2) {
        object.context_animation = object.apiobj.character_model->model_data_b[0x46] != NULL ? 0x46 : 0x45;
        if (object.apiobj.player_controlled) {
            Hint_SetComplete(0x261);
            Hint_SetComplete(0x26a);
        }
    } else if (active_panel->model_variant == 3) {
        object.context_animation = object.apiobj.character_model->model_data_b[0x45] != NULL
                                       ? 0x45
                                       : (object.apiobj.character_model->model_data_b[0x46] != NULL ? 0x46 : 0x45);
        MakeBaddiesForgetAboutParty(1);
        if (object.apiobj.player_controlled) {
            Hint_SetComplete(0x260);
            Hint_SetComplete(0x269);
        }
    } else {
        object.context_animation = 0x18;
        if (active_panel->model_variant == 1) {
            if (!InStory() || (static_cast<GIZPANEL_s *>(object.field_0x788)->draw_flags & 4) == 0)
                GizPanel_PlaySfx("TC14_VLA", &object.apiobj.collision_position, 1 << object.apiobj.field_0x27c);
            if ((object.apiobj.character_data->model_flags & 0x20) != 0)
                SetProtocolDroidInterfaceAction(&object);
        } else {
            if (!InStory() || (static_cast<GIZPANEL_s *>(object.field_0x788)->draw_flags & 4) == 0)
                GizPanel_PlaySfx("R2D2_VLA", &object.apiobj.collision_position, 1 << object.apiobj.field_0x27c);
        }
        if (object.apiobj.player_controlled) {
            Hint_SetComplete(0x25f);
            if (static_cast<GIZPANEL_s *>(object.field_0x788)->model_variant == 0)
                Hint_SetComplete(0x625);
            else
                Hint_SetComplete(0x624);
        }
        LSW_HintConditions.panel_used = 1;
    }
    f32 duration = AnimDuration(object.id, object.context_animation, 0.0f, 0.0f, 1);
    object.field_0xdb0 = 0.0f;
    object.context_animation_timer = duration;
    if (duration <= 0.0f)
        object.context_animation_timer = 2.0f;
}

void GizPanel_MoveCode(WORLDINFO_s *world, GameObject_s *object, i32 use) {
    if (use != 0 && !object->apiobj.player_controlled && world->current_level == MOSEISLEYA_LDATA)
        use = 0;
    if (object->field_0xdb0 > 0.0f)
        object->field_0xdb0 -= FRAMETIME;
    storm_panel_active = 0;
    droid_panel_active = 0;
    bountyhunter_panel_active = 0;
    if (object->character_context == 0x0b && object->field_0x788 != NULL) {
        if (object->apiobj.character_model->model_data_b[object->context_animation] == NULL ||
            AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0) != NULL) {
            object->field_0x768 += FRAMETIME;
            if (object->field_0x768 > 1.0f)
                object->field_0x768 = 1.0f;
            object->context_animation_timer -= FRAMETIME;
            if (object->context_animation_timer <= 0.0f) {
                object->character_context = -1;
                static_cast<GIZPANEL_s *>(object->field_0x788)->flags =
                    static_cast<GIZPANEL_FLAGS>(static_cast<GIZPANEL_s *>(object->field_0x788)->flags & ~1);
                static_cast<GIZPANEL_s *>(object->field_0x788)->flags =
                    static_cast<GIZPANEL_FLAGS>(static_cast<GIZPANEL_s *>(object->field_0x788)->flags | 2);
                object->field_0x788 = NULL;
            }
            if ((object->field_0xe21 & 0x10) == 0)
                object->field_0xe21 |= 0x10;
        } else {
            object->context_animation_timer -= FRAMETIME;
            if (object->context_animation_timer <= 0.0f) {
                if (object->field_0x788 != NULL) {
                    NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
                    if (static_cast<GIZPANEL_s *>(object->field_0x788)->model_variant == 2)
                        PlaySfx("Hunter_Granted", &object->apiobj.collision_position);
                    if (static_cast<GIZPANEL_s *>(object->field_0x788)->model_variant == 3)
                        PlaySfx("Trooper_Granted", &object->apiobj.collision_position);
                } else {
                    NewRumble(object->pad_gamepad->pad, 0.5f, 0);
                }
                if (object->apiobj.player_controlled) {
                    const u8 variant = static_cast<GIZPANEL_s *>(object->field_0x788)->model_variant;
                    if (variant <= 1) {
                        if (Mission_Active(NULL) != NULL) {
                            Hint_SetComplete(0x2b9);
                        } else {
                            Hint_SetComplete(0x25f);
                            LSW_HintConditions.panel_used = 1;
                            if (static_cast<GIZPANEL_s *>(object->field_0x788)->model_variant == 0)
                                Hint_SetComplete(0x625);
                            else
                                Hint_SetComplete(0x624);
                        }
                    } else if (variant == 3) {
                        Hint_SetComplete(0x260);
                    } else if (variant == 2) {
                        Hint_SetComplete(0x261);
                    }
                }
                object->character_context = -1;
                static_cast<GIZPANEL_s *>(object->field_0x788)->flags =
                    static_cast<GIZPANEL_FLAGS>(static_cast<GIZPANEL_s *>(object->field_0x788)->flags & ~1);
                static_cast<GIZPANEL_s *>(object->field_0x788)->flags =
                    static_cast<GIZPANEL_FLAGS>(static_cast<GIZPANEL_s *>(object->field_0x788)->flags | 2);
                object->field_0x788 = NULL;
            } else if ((object->apiobj.character_data->model_flags & 0x20) != 0) {
                SetProtocolDroidInterfaceAction(object);
            }
        }
        return;
    }
    if (!(object->character_context != 1 && object->character_context != -1 && object->character_context != 2 &&
          !objInNetWaitContext(object, 0x0b))) {
        f32 distance;
        GIZPANEL_s *panel = GizPanel_FindNearest(WORLD, &object->apiobj.position, object, &distance, 1);
        if (panel != NULL) {
            const f32 radius = (0.25f + object->apiobj.field_0x1dc) * panel->target_scale;
            if (distance < radius * radius &&
                (use != 0 || (object->panel_use_request == 1 && object->big_jump_data != NULL)))
                GizPanel_Use(*object, *panel);
        } else if (use != 0 && object->apiobj.player_controlled &&
                   (object->apiobj.character_data->model_flags & 0x20) != 0 && object->field_0xdb0 <= 0.0f) {
            GizPanel_PlaySfx("TC14_VLN", &object->apiobj.collision_position, 1 << object->apiobj.field_0x27c);
            object->field_0xdb0 = 0.5f;
        }
    }
}

i32 GizPanel_UpdateHint(HINT_s *hint) {
    bool storm = false;
    bool droid = false;
    i32 droid_type = -1;
    bool bountyhunter = false;
    for (i32 index = 0; index < 8; ++index) {
        if (Player[index] == NULL)
            continue;
        f32 distance;
        GIZPANEL_s *panel = GizPanel_FindNearest(WORLD, &Player[index]->apiobj.position, Player[index], &distance, 0);
        if (panel == NULL || static_cast<u8>(panel->flags & 0x0b) != 8)
            continue;
        f32 radius = (1.0f + Player[index]->apiobj.field_0x1dc) * panel->target_scale;
        if (distance < radius * radius) {
            switch (panel->model_variant) {
                case 0:
                case 1:
                    droid = true;
                    droid_type = panel->model_variant;
                    break;
                case 2:
                    bountyhunter = true;
                    break;
                case 3:
                    storm = true;
                    break;
            }
        }
    }
    if (storm && (hint->control_mode_ids[0] == 0x269 || hint->control_mode_ids[0] == 0x260)) {
        if (AvailableToPlayer(0, 1, 5, 0) != 0)
            return hint->control_mode_ids[0] == 0x260;
        return hint->control_mode_ids[0] == 0x269;
    }
    if (bountyhunter && (hint->control_mode_ids[0] == 0x26a || hint->control_mode_ids[0] == 0x261)) {
        if (AvailableToPlayer(0x1000000, -1, 6, 0) != 0)
            return hint->control_mode_ids[0] == 0x261;
        return hint->control_mode_ids[0] == 0x26a;
    }
    if (droid) {
        if (hint->control_mode_ids[0] == 0x624)
            return droid_type == 1 && FreePlay != 0 && AvailableToPlayer(0x30, -1, 0, 1) == 0;
        if (hint->control_mode_ids[0] == 0x625)
            return droid_type == 0 && FreePlay != 0 && AvailableToPlayer(0x50, -1, 0, 1) == 0;
        if (hint->control_mode_ids[0] == 0x25f)
            return LSW_HintConditions.tc14_present;
    }
    return 0;
}

