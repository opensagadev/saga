#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/actions/combat/rope.h"

#include "decomp.h"
#include "globals.h"
#include "gameapi/edtools/edfile.h"
#include "legoapi/actions/movement/jumping.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

#include <stdio.h>

extern "C" void NewTerrPlatformsOff(void);
f32 GameShadow(GameObject_s *object, NUVEC *position, f32 probe_height, i32 terrain_mask);
void FindAnglesZX(NUVEC *normal, u16 *x_rotation, u16 *z_rotation);
void EnableShadowMapRendering(i32 enabled);
void ResetShadowMapRendering();
void SetWeaponIn(GameObject_s *object);

extern i32 editor_active;
extern NUMTL *SolidMtl3D;

i32 grapple_gizmotype_id = -1;
i32 Grapple_RopeSwingsAvailable;
i16 Grapple_RopeSwingRotate;
extern i32 GrappleSwingMode;

struct GRAPPLEPROGRESS {
    u32 active_mask;
    u32 visible_mask;
};

static NUVEC GrapplePointOffset = {0.0f, -0.023f, 0.169f};
static GRAPPLE DynamicGrapple[4];

static void Grapple_ResetRopePoints(GRAPPLE *grapple) {
    const f32 segment_length = grapple->rope_length / 6.0f;
    for (i32 point_index = 0; point_index < 6; ++point_index) {
        grapple->target_rope_points[point_index].x = grapple->hook_position.x;
        grapple->target_rope_points[point_index].y =
            grapple->hook_position.y - (segment_length * static_cast<f32>(point_index) + segment_length);
        grapple->target_rope_points[point_index].z = grapple->hook_position.z;
    }
    for (i32 point_index = 0; point_index < 6; ++point_index) {
        grapple->rope_points[point_index] = grapple->target_rope_points[point_index];
    }
}

static void Grapple_ResetDynamic(GRAPPLE *grapple, i32 index) {
    sprintf(grapple->name, "dynamic%i", index + 1);
    grapple->active = 1;
    grapple->visible = 0;
    grapple->y_rotation = 0;
    grapple->attached_object = NULL;
    grapple->has_terrain_platform = 1;
}

GameObject_s *Grapple_Occupied(GRAPPLE *grapple, GameObject_s *object, AIPATHCNX_s *connection) {
    if (LEGOCONTEXT_GRAPPLE != -1) {
        GameObject_s *candidate = Obj;
        for (i32 index = 0; index < HIGHGAMEOBJECT; ++index, ++candidate) {
            if ((candidate->apiobj.field_0x1f8 & 0x1001) == 0x1001 && object != candidate) {
                if (candidate->character_context == LEGOCONTEXT_GRAPPLE && candidate->field_0x788 == grapple) {
                    return candidate;
                }
                if (connection != NULL && candidate->ai.path_info.connection == connection && LEGOCONTEXT_JUMP != -1 &&
                    candidate->character_context == LEGOCONTEXT_JUMP) {
                    return candidate;
                }
            }
        }
    }
    return NULL;
}

static i32 Grapples_GetMaxGizmos(void *world_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (world == NULL) {
        return 0;
    }
    return world->current_level->max_grapples;
}

static void Grapples_AddGizmos(GIZMOSYS *gizmo_sys, i32 type_id, void *world_ptr, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    for (i32 index = 0; index < world->grapple_count; ++index) {
        GRAPPLE *grapple = &world->grapples[index];
        if (NuStrLen(grapple->name) != 0) {
            AddGizmo(gizmo_sys, type_id, NULL, grapple);
        }
    }
}

static inline f32 Grapple_ResetShadow(GRAPPLE *grapple) {
    const u8 has_terrain_platform = grapple->has_terrain_platform;
    NuVecRotateX(&grapple->hook_position, &GrapplePointOffset, grapple->x_rotation);
    NuVecRotateY(&grapple->hook_position, &grapple->hook_position, grapple->y_rotation);
    NuVecAdd(&grapple->hook_position, &grapple->hook_position, &grapple->position);

    const f32 shadow_y = grapple->hook_position.y - 0.1f;
    const f32 hook_x = grapple->hook_position.x;
    grapple->shadow_probe_position.x = hook_x;
    grapple->ground_position.x = hook_x;
    grapple->shadow_probe_position.y = shadow_y;
    const f32 hook_z = grapple->hook_position.z;
    grapple->shadow_probe_position.z = hook_z;
    grapple->ground_position.z = hook_z;

    if (has_terrain_platform == 0) {
        NewTerrPlatformsOff();
    }
    const f32 ground_y = GameShadow(NULL, &grapple->hook_position, 5.0f, -1);
    grapple->ground_position.y = ground_y;
    if (grapple->ground_position.y != 2000000.0f) {
        grapple->ground_position.y += 0.005f;
        FindAnglesZX(&ShadNorm, &grapple->ground_x_rotation, &grapple->ground_z_rotation);
    }
    return ground_y;
}

static inline void Grapple_ClampRopeLength(GRAPPLE *grapple) {
    if (grapple->ground_position.y != 2000000.0f) {
        const f32 available_length = grapple->hook_position.y - (grapple->ground_position.y + 0.1f);
        if (grapple->rope_length > available_length) {
            if (0.7f > available_length) {
                grapple->rope_length = 0.7f;
            } else {
                grapple->rope_length = available_length;
            }
        }
    }
}

static inline void Grapple_ResetRope(GRAPPLE *grapple) {
    if (Grapple_ResetShadow(grapple) != 2000000.0f) {
        Grapple_ClampRopeLength(grapple);
    }
    Grapple_ResetRopePoints(grapple);
}

static inline void Grapple_UpdateDynamicRope(GRAPPLE *grapple, GameObject_s *attached) {
    grapple->position = attached->apiobj.lower_position;
    grapple->y_rotation = static_cast<u16>(attached->apiobj.field_0x276 + 0x8000);
    Grapple_ResetShadow(grapple);
    Grapple_ClampRopeLength(grapple);
    Grapple_ResetRopePoints(grapple);
    grapple->flags |= GRAPPLE_FLAG_ACTIVE | GRAPPLE_FLAG_VISIBLE;
    grapple->has_terrain_platform = 1;
    grapple->activation_progress = 0.0f;
}

static void Grapples_Update(void *world_ptr, void *, float) {
    NUVEC direction;
    NUVEC attachment_position;
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);

    GRAPPLE *grapple = world->grapples;
    for (i32 grapple_index = 0; grapple_index < world->grapple_count; ++grapple_index, ++grapple) {
        if ((grapple->flags & (GRAPPLE_FLAG_ACTIVE | GRAPPLE_FLAG_VISIBLE)) ==
            (GRAPPLE_FLAG_ACTIVE | GRAPPLE_FLAG_VISIBLE)) {
            if (grapple->activation_progress > 0.0f) {
                grapple->activation_progress -= FRAMETIME;
            }

            if (NuSpecialExistsFn(&grapple->display_special) != 0) {
                grapple->hook_position = *NuSpecialGetDrawPos(&grapple->display_special);
                grapple->shadow_probe_position.x = grapple->hook_position.x;
                grapple->shadow_probe_position.y = grapple->hook_position.y - 0.1f;
                grapple->shadow_probe_position.z = grapple->hook_position.z;
                grapple->ground_position.x = grapple->hook_position.x;
                grapple->ground_position.z = grapple->hook_position.z;
                grapple->ground_position.y = GameShadow(NULL, &grapple->hook_position, 5.0f, -1);
                if (grapple->ground_position.y != 2000000.0f) {
                    grapple->ground_position.y += 0.005f;
                    FindAnglesZX(&ShadNorm, &grapple->ground_x_rotation, &grapple->ground_z_rotation);
                }
            }

            if ((grapple->flags & GRAPPLE_FLAG_DISABLED) != 0) {
                GameObject_s *occupant = Grapple_Occupied(grapple, NULL, NULL);

                if (occupant != NULL) {
                    PLAYERCHARACTERCONFIG_s *character_config = occupant->apiobj.character_data->player_config;
                    const NUVEC *attachment_source;
                    const i32 joint_a = character_config->grapple_joint_a;
                    if (joint_a != -1 && occupant->apiobj.character_model->points_of_interest[joint_a] != NULL) {
                        attachment_source = NUMTX_GET_ROW_VEC(&occupant->joint_matrices[joint_a], 3);
                        const i32 joint_b = character_config->grapple_joint_b;
                        if (joint_b != -1 && occupant->apiobj.character_model->points_of_interest[joint_b] != NULL) {
                            attachment_position = *NUMTX_GET_ROW_VEC(&occupant->joint_matrices[joint_b], 3);
                            goto attachment_ready;
                        }
                    } else {
                        GRAPPLE *occupied_grapple = static_cast<GRAPPLE *>(occupant->field_0x788);
                        attachment_source = &occupant->apiobj.collision_position;
                        if ((occupied_grapple->flags & GRAPPLE_FLAG_DISABLED) == 0) {
                            attachment_source = &occupant->apiobj.upper_position;
                        }
                    }
                    attachment_position = *attachment_source;
                attachment_ready:;
                } else {
                    const f32 rope_length = grapple->rope_length;
                    const f32 sway = rope_length * 0.0125f;
                    attachment_position.x =
                        grapple->hook_position.x +
                        NU_SIN_LUT(static_cast<u16>(NuFmod(GameTimer.time_elapsed, 1.5f) / 1.5f * 65536.0f) +
                                   grapple_index * 0x2000) *
                            sway;
                    attachment_position.y = grapple->hook_position.y - rope_length * 0.333f;
                    attachment_position.z =
                        grapple->hook_position.z +
                        NU_SIN_LUT(static_cast<u16>(NuFmod(GameTimer.time_elapsed, 1.217f) / 1.217f * 65536.0f) +
                                   grapple_index * 0x2666) *
                            sway;
                }

                const f32 distance = NuVecDist(&attachment_position, &grapple->hook_position, &direction);
                const f32 inverse_distance = 1.0f / distance;
                f32 segment_offset = 0.0f;
                f32 slack = 0.0f;
                if (grapple->rope_length > distance) {
                    slack = grapple->rope_length - distance;
                    segment_offset = slack / 6.0f;
                    direction.x *= inverse_distance;
                    direction.y *= inverse_distance;
                    direction.z *= inverse_distance;
                }

                for (i32 point_index = 0; point_index < 6; ++point_index) {
                    const f32 point_fraction = static_cast<f32>(point_index + 1) / 6.0f;
                    const f32 offset = segment_offset * static_cast<f32>(point_index) + segment_offset;
                    grapple->target_rope_points[point_index].x = attachment_position.x + direction.x * offset;
                    grapple->target_rope_points[point_index].y = attachment_position.y + direction.y * offset;
                    grapple->target_rope_points[point_index].z = attachment_position.z + direction.z * offset;

                    if (occupant != NULL && slack > 0.0f) {
                        const f32 wave_scale = 0.1f * slack * point_fraction;
                        grapple->target_rope_points[point_index].x +=
                            NU_SIN_LUT(static_cast<u16>(NuFmod(GameTimer.time_elapsed, 1.5f) / 1.5f * 65536.0f) +
                                       grapple_index * 0x2000) *
                            wave_scale;
                        grapple->target_rope_points[point_index].z +=
                            NU_SIN_LUT(static_cast<u16>(NuFmod(GameTimer.time_elapsed, 1.217f) / 1.217f * 65536.0f) +
                                       grapple_index * 0x2666) *
                            wave_scale;
                    }
                }

                SeekVec(&grapple->rope_points[0], &grapple->rope_points[0], &grapple->target_rope_points[0], 24.0f);
                SeekVec(&grapple->rope_points[1], &grapple->rope_points[1], &grapple->target_rope_points[1], 12.0f);
                SeekVec(&grapple->rope_points[2], &grapple->rope_points[2], &grapple->target_rope_points[2], 8.0f);
                SeekVec(&grapple->rope_points[3], &grapple->rope_points[3], &grapple->target_rope_points[3], 6.0f);
                SeekVec(&grapple->rope_points[4], &grapple->rope_points[4], &grapple->target_rope_points[4], 4.5f);
                SeekVec(&grapple->rope_points[5], &grapple->rope_points[5], &grapple->target_rope_points[5], 3.0f);
            }
        }
    }

    grapple = DynamicGrapple;
    GRAPPLE *dynamic_end = &DynamicGrapple[4];
    for (; grapple != dynamic_end; ++grapple) {
        if (grapple->activation_progress > 0.0f) {
            grapple->activation_progress -= FRAMETIME;
        }

        GameObject_s *attached = grapple->attached_object;
        if (attached != NULL && grapple->retain_attachment == 0) {
            if ((attached->apiobj.field_0x1f8 & 0x1001) == 0x1001 && attached->apiobj.field_0x287 == 0) {
                GameObject_s *linked_object = attached->field_0xcc0;
                if (linked_object != NULL && LEGOCONTEXT_BEENTAKENOVER != -1 &&
                    linked_object->character_context == LEGOCONTEXT_BEENTAKENOVER) {
                    Grapple_UpdateDynamicRope(grapple, attached);
                    continue;
                }
            } else {
                grapple->attached_object = NULL;
            }
            grapple->flags &= ~GRAPPLE_FLAG_VISIBLE;
        }
    }
}

static void Grapples_DrawList(WORLDINFO *world, GRAPPLE *grapples, i32 grapple_count, u16 target_rotation,
                              f32 target_alpha) {
    f32 distance_squared;
    NUVEC rope_start;
    GameObject_s *nearest_player = NULL;
    NUMTX matrix;
    if (grapples != NULL && world != NULL) {

        for (i32 index = 0; index < grapple_count; ++index, ++grapples) {
            GRAPPLE *grapple = grapples;
            if ((grapple->flags & GRAPPLE_FLAG_VISIBLE) == 0) {
                continue;
            }

            if (NuSpecialExistsFn(&grapple->display_special) == 0 && LEGOOBJ_GRAPPLE_HOOK != -1 &&
                world->lev_objs[LEGOOBJ_GRAPPLE_HOOK].active != 0) {
                NuMtxSetRotationX(&matrix, grapple->x_rotation);
                NuMtxRotateY(&matrix, grapple->y_rotation);
                NuMtxTranslate(&matrix, &grapple->position);
                NuSpecialDrawAt(&world->lev_objs[LEGOOBJ_GRAPPLE_HOOK].special, &matrix);
            }

            if ((grapple->flags & GRAPPLE_FLAG_ACTIVE) == 0) {
                continue;
            }

            const u8 occupancy_flags = grapple->flags & (GRAPPLE_FLAG_DISABLED | GRAPPLE_FLAG_REVERSED);
            if (occupancy_flags == GRAPPLE_FLAG_DISABLED && Grapple_Occupied(grapple, NULL, NULL) == NULL) {
                rope_start = grapple->hook_position;
                DrawRopeCurved(&rope_start, grapple->rope_points, 6, 0, NULL);
            }

            if ((grapple->flags & GRAPPLE_FLAG_DISABLED) == 0 || editor_active != 0) {
                if (grapple->ground_position.y == 2000000.0f || LEGOOBJ_FLOORTARGET == -1 ||
                    world->lev_objs[LEGOOBJ_FLOORTARGET].active == 0) {
                    continue;
                }

                NuMtxSetRotationY(&matrix, target_rotation);
                if (grapple->ground_z_rotation != 0) {
                    NuMtxRotateZ(&matrix, grapple->ground_z_rotation);
                }
                if (grapple->ground_x_rotation != 0) {
                    NuMtxRotateX(&matrix, grapple->ground_x_rotation);
                }
                NuMtxScaleU(&matrix, 0.8f);
                NuMtxTranslate(&matrix, &grapple->ground_position);

                f32 alpha = target_alpha;
                if (editor_active == 0) {
                    if (!FindNearestPlayerToVec(&grapple->ground_position, &nearest_player, distance_squared, false,
                                                0)) {
                        continue;
                    }

                    const i32 distance_band = static_cast<i32>(distance_squared / 6.0f);
                    const i32 fade_index = distance_band > 0 ? 0x2000 : (distance_band << 13) & 0x6000;
                    alpha -= NuTrigTable[fade_index];
                }

                if (alpha > 0.0f) {
                    NuSpecialDrawAtAlpha(&world->lev_objs[LEGOOBJ_FLOORTARGET].special, &matrix, alpha);
                }
            }
        }
    }
}

static void Grapples_Draw(void *world_ptr, void *, float) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    const f32 rotation_time = NuFmod(GameTimer.time_elapsed, 5.0f);
    const f32 pulse_time = NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f) * 2.0f * 65536.0f;
    const f32 target_alpha = NuTrigTable[(static_cast<i32>(pulse_time) >> 1) & 0x7fff] * 0.2f + 0.8f;

    EnableShadowMapRendering(0);
    const u16 rotation = static_cast<u16>(rotation_time / 5.0f * 65536.0f);
    Grapples_DrawList(world, world->grapples, world->grapple_count, rotation, target_alpha);
    Grapples_DrawList(world, DynamicGrapple, 4, rotation, target_alpha);
    ResetShadowMapRendering();
}

static char *Grapple_GetGizmoName(GIZMO *gizmo) {
    return gizmo != NULL ? static_cast<GRAPPLE *>(gizmo->object)->name : NULL;
}

static i32 Grapple_GetOutput(GIZMO *gizmo, i32 output_index, i32) {
    GRAPPLE *grapple = static_cast<GRAPPLE *>(gizmo->object);
    if ((grapple->flags & (GRAPPLE_FLAG_ACTIVE | GRAPPLE_FLAG_VISIBLE)) !=
        (GRAPPLE_FLAG_ACTIVE | GRAPPLE_FLAG_VISIBLE)) {
        return 0;
    }
    if (output_index == 1 || output_index == 2) {
        return Grapple_Occupied(grapple, NULL, NULL) != 0;
    }
    return 1;
}

static char *Grapple_GetOutputName(GIZMO *, i32 output_index) {
    if (output_index == 1) {
        return const_cast<char *>("Occupied");
    }
    if (output_index == 2) {
        return const_cast<char *>("Occupied By 2");
    }
    return const_cast<char *>("Active");
}

static i32 Grapple_GetNumOutputs(GIZMO *) {
    return 3;
}

static void Grapple_Activate(GIZMO *gizmo, i32 active) {
    if (gizmo != NULL) {
        GRAPPLE *grapple = static_cast<GRAPPLE *>(gizmo->object);
        grapple->active = active != 0;
        if ((grapple->flags & GRAPPLE_FLAG_ACTIVE) != 0) {
            grapple->activation_progress = 1.0f;
        }
    }
}

static void Grapple_SetVisibility(GIZMO *gizmo, i32 visible) {
    if (gizmo != NULL) {
        GRAPPLE *grapple = static_cast<GRAPPLE *>(gizmo->object);
        grapple->visible = visible != 0;
    }
}

static void *Grapples_AllocateProgressData(VARIPTR *buffer, VARIPTR *buffer_end) {
    return GizmoBufferAlloc(buffer, buffer_end, sizeof(GRAPPLEPROGRESS));
}

static void Grapples_ClearProgress(void *, void *progress_ptr) {
    GRAPPLEPROGRESS *progress = static_cast<GRAPPLEPROGRESS *>(progress_ptr);
    if (progress != NULL) {
        progress->active_mask = ~0u;
        progress->visible_mask = ~0u;
    }
}

static void Grapples_StoreProgress(void *world_ptr, void *, void *progress_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    GRAPPLEPROGRESS *progress = static_cast<GRAPPLEPROGRESS *>(progress_ptr);

    if (progress == NULL) {
        return;
    }

    progress->active_mask = ~0u;
    progress->visible_mask = ~0u;
    if (world == NULL || world->grapples == NULL) {
        return;
    }

    for (i32 index = 0; index < world->grapple_count && index < 32; ++index) {
        const u32 mask = 1u << index;
        if ((world->grapples[index].flags & GRAPPLE_FLAG_VISIBLE) == 0) {
            progress->visible_mask &= ~mask;
        }
        if ((world->grapples[index].flags & GRAPPLE_FLAG_ACTIVE) == 0) {
            progress->active_mask &= ~mask;
        }
    }
}

static void Grapples_Reset(void *world_ptr, void *, void *progress_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    GRAPPLEPROGRESS *progress = static_cast<GRAPPLEPROGRESS *>(progress_ptr);
    if (world == NULL) {
        return;
    }

    GRAPPLE *grapple = world->grapples;
    if (grapple != NULL) {
        for (i32 index = 0; index < world->grapple_count; ++index, ++grapple) {
            GRAPPLE *rope_grapple = &world->grapples[index];
            Grapple_ResetRope(rope_grapple);
            rope_grapple->flags |= GRAPPLE_FLAG_ACTIVE | GRAPPLE_FLAG_VISIBLE;
            rope_grapple->has_terrain_platform = 0;
            rope_grapple->activation_progress = 0.0f;

            if (index <= 31 && progress != NULL) {
                const u32 bit = 1u << index;
                grapple->visible = (progress->visible_mask & bit) != 0;
                grapple->active = (progress->active_mask & bit) != 0;
            }
        }
    }

    for (i32 index = 0; index < 4; ++index) {
        Grapple_ResetDynamic(&DynamicGrapple[index], index);
    }
}

static void *Grapples_ReserveBufferSpace(void *world_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    void *reserved_space = NULL;
    world->grapples = NULL;
    world->grapple_count = 0;

    if (world->current_level->max_grapples != 0) {
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
        world->grapples = reinterpret_cast<GRAPPLE *>(world->giz_buffer.addr);
        world->giz_buffer.addr += world->current_level->max_grapples * sizeof(GRAPPLE);
        reserved_space = world->grapples;
    }
    return reserved_space;
}

static i32 Grapples_Load(void *world_ptr, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    char special_name[256];
    if (world->grapple_count != 0) {
        return 0;
    }

    const i32 version = EdFileReadInt();
    world->grapple_count = EdFileReadInt();
    for (i32 index = 0; index < world->grapple_count; ++index) {
        EdFileRead(world->grapples[index].name, 16);
        EdFileReadNuVec(&world->grapples[index].position);
        if (version < 2) {
            EdFileReadUnsignedShort();
        }
        world->grapples[index].y_rotation = EdFileReadUnsignedShort();
        if (version > 2) {
            world->grapples[index].field_0x4c = EdFileReadFloat();
        } else {
            world->grapples[index].field_0x4c = 1.0f;
        }
        if (version > 3) {
            world->grapples[index].disabled = EdFileReadChar() != 0;
            world->grapples[index].rope_length = EdFileReadFloat();
        } else {
            world->grapples[index].disabled = 0;
            world->grapples[index].rope_length = 2.0f;
        }
        if (version > 4) {
            world->grapples[index].x_rotation = EdFileReadUnsignedShort();
        } else {
            world->grapples[index].x_rotation = 0;
        }
        if (version > 5) {
            world->grapples[index].reversed = EdFileReadChar() != 0;
        } else {
            world->grapples[index].reversed = 0;
        }
        world->grapples[index].display_special = nuhspecial_s();
        if (version > 6) {
            const i32 name_length = EdFileReadUnsignedChar();
            if (name_length != 0) {
                EdFileRead(special_name, name_length);
                special_name[name_length] = '\0';
                NuSpecialFind(world->current_gscn, &world->grapples[index].display_special, special_name, 1);
            }
        }
    }
    return 1;
}

static void Grapple_FindNearestInList(NUVEC *position, GRAPPLE *grapple, i32 count, GameObject_s *object,
                                     GRAPPLE **nearest, f32 *nearest_distance) {
    NUVEC local;
    for (i32 index = 0; index < count; ++index, ++grapple) {
        f32 distance;
        if (object != NULL) {
            if ((grapple->flags & (GRAPPLE_FLAG_ACTIVE | GRAPPLE_FLAG_VISIBLE)) !=
                    (GRAPPLE_FLAG_ACTIVE | GRAPPLE_FLAG_VISIBLE) ||
                (grapple->has_terrain_platform != 0 && grapple->attached_object == NULL)) {
                continue;
            }
            if ((grapple->flags & GRAPPLE_FLAG_DISABLED) != 0) {
                if (Grapple_Occupied(grapple, NULL, NULL) != NULL) {
                    continue;
                }
                if (object->apiobj.player_controlled != 0 &&
                    (object->apiobj.field_0x27d != 0 || !(object->apiobj.velocity.y <= 0.0f) ||
                     (object->character_context != -1 &&
                      (LEGOCONTEXT_JUMP == -1 || object->character_context != LEGOCONTEXT_JUMP ||
                       !(object->context_animation_timer >= 0.1f))))) {
                    continue;
                }
            }
            if ((grapple->flags & GRAPPLE_FLAG_REVERSED) != 0 &&
                ((object->apiobj.character_data->game_character->flags_094[3] & 0x20) == 0 ||
                 (grapple->flags & GRAPPLE_FLAG_DISABLED) == 0)) {
                continue;
            }
            if (!(object->apiobj.upper_position.y < grapple->shadow_probe_position.y)) {
                continue;
            }
            if ((grapple->flags & GRAPPLE_FLAG_DISABLED) != 0) {
                if (!(object->apiobj.upper_position.y > grapple->hook_position.y - (0.1f + grapple->rope_length))) {
                    continue;
                }
            } else if (!(object->apiobj.pos_y > grapple->ground_position.y)) {
                continue;
            }
            NuVecSub(&local, &object->apiobj.upper_position, &grapple->hook_position);
            NuVecRotateY(&local, &local, -GameCam->yaw);
            if (!(NuFabs(local.z) < 0.5f)) {
                continue;
            }
            distance = local.x * local.x;
        } else {
            distance = NuVecDistSqr(position, &grapple->hook_position, NULL);
        }
        if (distance < *nearest_distance) {
            *nearest = grapple;
            *nearest_distance = distance;
        }
    }
}

GRAPPLE *Grapple_FindNearest(WORLDINFO *world, NUVEC *position, GameObject_s *object, f32 *distance_squared) {
    if (world->grapples == NULL) {
        return NULL;
    }
    GRAPPLE *nearest = NULL;
    f32 nearest_distance;
    if (object != NULL) {
        nearest_distance = object->apiobj.field_0x1dc;
        if (object->apiobj.field_0x27d == 0 && Grapple_RopeSwingsAvailable == 0) {
            nearest_distance += nearest_distance;
        }
        nearest_distance += 0.25f;
        nearest_distance *= nearest_distance;
    } else {
        nearest_distance = 1000000000.0f;
    }
    Grapple_FindNearestInList(position, world->grapples, world->grapple_count, object, &nearest, &nearest_distance);
    if (object != NULL) {
        Grapple_FindNearestInList(position, DynamicGrapple, 4, object, &nearest, &nearest_distance);
    }
    if (distance_squared != NULL) {
        *distance_squared = nearest_distance;
    }
    return nearest;
}

GRAPPLE *Grapple_FindNearestToPos(WORLDINFO *world, NUVEC *position) {
    GRAPPLE *nearest = NULL;
    if (world->grapples != NULL) {
        f32 nearest_distance = 1000000000.0f;
        GRAPPLE *grapple = world->grapples;
        for (i32 index = 0; index < world->grapple_count; ++index, ++grapple) {
            NUVEC midpoint;
            midpoint.x = (grapple->ground_position.x + grapple->hook_position.x) * 0.5f;
            midpoint.y = (grapple->ground_position.y + grapple->hook_position.y) * 0.5f;
            midpoint.z = (grapple->ground_position.z + grapple->hook_position.z) * 0.5f;
            const f32 distance = NuVecDistSqr(&midpoint, position, NULL);
            if (distance < nearest_distance) {
                nearest_distance = distance;
                nearest = grapple;
            }
        }
    }
    return nearest;
}

void Grapple_DrawLine(GameObject_s *object) {
    GRAPPLE *grapple = static_cast<GRAPPLE *>(object->field_0x788);
    if ((grapple->flags & GRAPPLE_FLAG_DISABLED) == 0 &&
        object->apiobj.character_model->model_data_b[object->context_animation] != NULL &&
        AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 0, 0) == NULL) {
        return;
    }
    if (object->field_0x7a3 == 1) {
        return;
    }

    grapple = static_cast<GRAPPLE *>(object->field_0x788);
    NURND_VERTEX3D vertices[2];
    vertices[1].colour = vertices[0].colour = grapple->reversed ? 0xff003f5f : 0xffffffff;
    PLAYERCHARACTERCONFIG_s *config = object->apiobj.character_data->player_config;
    const NUVEC *first_attachment;
    const NUVEC *second_attachment = NULL;
    const i32 joint_a = config->grapple_joint_a;
    if (joint_a != -1 && object->apiobj.character_model->points_of_interest[joint_a] != NULL) {
        first_attachment = NUMTX_GET_ROW_VEC(&object->joint_matrices[joint_a], 3);
        const i32 joint_b = config->grapple_joint_b;
        if (joint_b != -1 && object->apiobj.character_model->points_of_interest[joint_b] != NULL) {
            second_attachment = NUMTX_GET_ROW_VEC(&object->joint_matrices[joint_b], 3);
        }
    } else {
        first_attachment = grapple->disabled ? &object->apiobj.collision_position : &object->apiobj.upper_position;
    }

    vertices[0].position = grapple->hook_position;
    vertices[1].position = *first_attachment;
    if (second_attachment != NULL) {
        NuRndrLine3d(vertices, SolidMtl3D, NULL);
        vertices[0].position = vertices[1].position;
        vertices[1].position = *second_attachment;
    }
    NuRndrLine3d(vertices, SolidMtl3D, NULL);
    grapple = static_cast<GRAPPLE *>(object->field_0x788);
    if ((grapple->flags & (GRAPPLE_FLAG_DISABLED | GRAPPLE_FLAG_REVERSED)) == GRAPPLE_FLAG_DISABLED) {
        vertices[0].position = vertices[1].position;
        DrawRopeCurved(&vertices[0].position, grapple->rope_points, 6, 0, NULL);
    }
}

ADDGIZMOTYPE *Grapples_RegisterGizmo(i32 type_id) {
    static ADDGIZMOTYPE addtype;

    addtype = Default_ADDGIZMOTYPE;
    addtype.name = "Grapple";
    addtype.prefix = "";
    addtype.fns.unknown1 = 8;
    addtype.fns.early_update_fn = NULL;
    addtype.fns.panel_draw_fn = NULL;
    addtype.fns.get_visibility_fn = NULL;
    addtype.fns.get_max_gizmos_fn = Grapples_GetMaxGizmos;
    addtype.fns.get_pos_fn = NULL;
    addtype.fns.using_special_fn = NULL;
    addtype.fns.add_gizmos_fn = Grapples_AddGizmos;
    addtype.fns.bolt_hit_plat_fn = NULL;
    addtype.fns.get_best_bolt_target_fn = NULL;
    addtype.fns.late_update_fn = Grapples_Update;
    addtype.fns.bolt_hit_fn = NULL;
    addtype.fns.draw_fn = Grapples_Draw;
    addtype.fns.get_gizmo_name_fn = Grapple_GetGizmoName;
    addtype.fns.get_output_fn = Grapple_GetOutput;
    addtype.fns.get_output_name_fn = Grapple_GetOutputName;
    addtype.fns.get_num_outputs_fn = Grapple_GetNumOutputs;
    addtype.fns.activate_fn = Grapple_Activate;
    addtype.fns.activate_rev_fn = NULL;
    addtype.fns.set_visibility_fn = Grapple_SetVisibility;
    addtype.fns.allocate_progress_data_fn = Grapples_AllocateProgressData;
    addtype.fns.clear_progress_fn = Grapples_ClearProgress;
    addtype.fns.store_progress_fn = Grapples_StoreProgress;
    addtype.fns.reset_fn = Grapples_Reset;
    addtype.fns.reserve_buffer_space_fn = Grapples_ReserveBufferSpace;
    addtype.fns.load_fn = Grapples_Load;
    addtype.fns.post_load_fn = NULL;
    addtype.fns.add_level_sfx_fn = NULL;
    grapple_gizmotype_id = type_id;

    return &addtype;
}

GRAPPLE *Grapple_AddDynamic(void *attached_object, i32 retain_attachment) {
    for (i32 index = 0; index < 4; ++index) {
        if (DynamicGrapple[index].attached_object == attached_object) {
            return &DynamicGrapple[index];
        }
    }

    for (i32 index = 0; index < 4; ++index) {
        GRAPPLE *grapple = &DynamicGrapple[index];
        if (grapple->attached_object == NULL) {
            grapple->attached_object = static_cast<GameObject_s *>(attached_object);
            grapple->retain_attachment = static_cast<u8>(retain_attachment);
            grapple->flags |= GRAPPLE_FLAG_VISIBLE;
            return grapple;
        }
    }
    return NULL;
}

void Grapple_RemoveDynamic(void *attached_object) {
    for (i32 index = 0; index < 4; ++index) {
        GRAPPLE *grapple = &DynamicGrapple[index];
        if (grapple->attached_object == attached_object) {
            grapple->attached_object = NULL;
            grapple->flags &= ~GRAPPLE_FLAG_VISIBLE;
            return;
        }
    }
}

void Grapple_MoveCode(GameObject_s *object) {
    if (object == NULL || object->pad_gamepad == NULL || LEGOCONTEXT_GRAPPLE == -1) {
        return;
    }
    object->field_0xe24 &= ~0x80;

    if (object->character_context != LEGOCONTEXT_GRAPPLE) {
        if (Grapples_Available == 0 ||
            (static_cast<i8>(object->apiobj.flags_low) >= 0 && object->field_0xf0c != 1)) {
            return;
        }
        const bool ready = ObjLandReady(object) != 0 || object->character_context == LEGOCONTEXT_JUMP ||
                           (object->character_context == LEGOCONTEXT_GLIDE && LEGOACT_GLIDE != -1 &&
                            object->context_animation == LEGOACT_GLIDE && object->field_0x788 == NULL);
        if (!ready) {
            return;
        }

        GRAPPLE *grapple = Grapple_FindNearest(WORLD, &object->apiobj.collision_position, object, NULL);
        if (grapple == NULL) {
            return;
        }
        if (static_cast<i8>(object->apiobj.flags_low) >= 0 && object->can_use_object != grapple) {
            return;
        }

        const bool activate = (object->pad_gamepad->buttons_pressed & GAMEPAD_SPECIAL) != 0 ||
                              (object->panel_use_request == 4 && object->tube_entry_data != NULL);
        if (!activate) {
            SetHeadTarget(object, &grapple->hook_position, 4, 2.0f, 1.0f, 2.0f);
            return;
        }

        object->field_0x788 = grapple;
        object->character_context = static_cast<i8>(LEGOCONTEXT_GRAPPLE);
        object->context_animation_timer = 0.0f;
        object->airborne_action_duration = 0.0f;
        object->field_0x7a3 = 0;
        object->context_animation = LEGOACT_GRAPPLE_IDLE;
        object->field_0x768 = 0.0f;
        object->grapple_swing_degrees = 0;
        object->grapple_swing_phase = 0x2000;
        object->takeover_start_angle = object->apiobj.facing_angle;
        if ((grapple->flags & GRAPPLE_FLAG_DISABLED) == 0) {
            object->context_variant_flags |= 1;
            object->apiobj.facing_angle = static_cast<u16>(grapple->y_rotation + 0x8000);
        } else {
            object->context_variant_flags &= ~1;
            SetWeaponIn(object);
        }
        if (object->pad_gamepad->input_magnitude > 0.0f) {
            object->takeover_start_angle = GamePad_InputAngle(object, object->pad_gamepad);
        }
        if (GrappleSwingMode == 1) {
            const i32 difference = RotDiff(object->takeover_start_angle,
                                           static_cast<u16>(grapple->y_rotation + 0x4000));
            const i32 absolute_difference = difference < 0 ? -difference : difference;
            object->takeover_start_angle =
                static_cast<u16>(grapple->y_rotation + (absolute_difference <= 0x4000 ? 0x4000 : 0xc000));
        }
        GameCam_Blend(NULL, 0.5f, 0.0f, 1);
        GameAudio_PlaySfx(0x44, &object->apiobj.lower_position, 0, 0);
        object->field_0xe31 = 0;
        return;
    }

    GRAPPLE *grapple = static_cast<GRAPPLE *>(object->field_0x788);
    if (grapple == NULL ||
        (grapple->flags & (GRAPPLE_FLAG_ACTIVE | GRAPPLE_FLAG_VISIBLE)) !=
            (GRAPPLE_FLAG_ACTIVE | GRAPPLE_FLAG_VISIBLE) ||
        (static_cast<i8>(object->apiobj.flags_low) >= 0 && object->can_use_object != grapple)) {
        if (grapple != NULL) {
            grapple->activation_progress = 0.0f;
        }
        object->character_context = -1;
        object->field_0x788 = NULL;
        GameCam_Blend(NULL, 0.5f, 0.0f, 1);
        return;
    }

    if ((object->pad_gamepad->buttons_pressed & GAMEPAD_JUMP) != 0) {
        if ((grapple->flags & GRAPPLE_FLAG_DISABLED) != 0) {
            grapple->activation_progress = 1.0f;
        }
        StartJump(object, 0);
        object->movement_runtime_flags |= 0x10;
        object->field_0x788 = NULL;
        GameAudio_PlaySfx(0x45, &object->apiobj.collision_position, 0, 0);
        GameCam_Blend(NULL, 0.5f, 0.0f, 1);
        return;
    }

    if (object->field_0x7a3 == 1) {
        if (object->pad_gamepad->input_magnitude > 0.0f) {
            object->field_0x7a3 = 0;
            object->airborne_action_duration = 0.0f;
            object->context_animation = LEGOACT_GRAPPLE_IDLE;
            object->field_0x768 = 0.1f;
        }
        return;
    }

    object->context_animation_timer += FRAMETIME;
    if ((object->pad_gamepad->buttons_pressed & GAMEPAD_SPECIAL) != 0 &&
        object->pad_gamepad->input_magnitude == 0.0f && (grapple->flags & GRAPPLE_FLAG_DISABLED) == 0) {
        object->context_variant_flags ^= 1;
    }

    const f32 body_height = object->apiobj.upper_position.y - object->apiobj.lower_position.y;
    f32 maximum_length = grapple->shadow_probe_position.y - (grapple->ground_position.y + body_height + 0.1f);
    if ((grapple->flags & GRAPPLE_FLAG_DISABLED) != 0) {
        const f32 rope_limit = grapple->rope_length - (body_height * 2.0f + 0.1f);
        if (rope_limit < maximum_length) {
            maximum_length = rope_limit;
        }
    }
    if (maximum_length < 0.0f) {
        maximum_length = 0.0f;
    }

    bool climb_up = (object->context_variant_flags & 1) != 0;
    bool climb_down = false;
    if (object->pad_gamepad->input_magnitude > 0.0f) {
        const u16 input_angle = GamePad_InputAngle(object, object->pad_gamepad);
        const i32 difference = RotDiff(grapple->y_rotation, input_angle);
        const i32 absolute_difference = difference < 0 ? -difference : difference;
        if (absolute_difference < 0x4000) {
            climb_down = true;
            climb_up = false;
            object->apiobj.facing_angle = grapple->y_rotation;
        } else {
            climb_up = true;
            object->apiobj.facing_angle = static_cast<u16>(grapple->y_rotation + 0x8000);
        }
    }

    f32 movement_speed = 1.0f;
    if (climb_up && LEGOACT_GRAPPLE_UP != -1 &&
        object->apiobj.character_model->model_data_b[LEGOACT_GRAPPLE_UP] != NULL) {
        object->context_animation = LEGOACT_GRAPPLE_UP;
        movement_speed = MAX(AnimSpeed(object->apiobj.character_model, LEGOACT_GRAPPLE_UP), 0.5f);
        object->field_0x768 -= movement_speed * FRAMETIME;
    } else if (climb_down && LEGOACT_GRAPPLE_DOWN != -1 &&
               object->apiobj.character_model->model_data_b[LEGOACT_GRAPPLE_DOWN] != NULL) {
        object->context_animation = LEGOACT_GRAPPLE_DOWN;
        movement_speed = MAX(AnimSpeed(object->apiobj.character_model, LEGOACT_GRAPPLE_DOWN), 0.5f);
        object->field_0x768 += movement_speed * FRAMETIME;
    } else {
        object->context_animation = LEGOACT_GRAPPLE_IDLE;
    }

    if (object->field_0x768 < 0.0f) {
        object->field_0x768 = 0.0f;
    } else if (object->field_0x768 > maximum_length) {
        object->field_0x768 = maximum_length;
        object->field_0xe24 |= 0x80;
    }

    if (Grapple_RopeSwingRotate != 0 && (grapple->flags & GRAPPLE_FLAG_DISABLED) != 0) {
        object->apiobj.facing_angle = static_cast<u16>(object->apiobj.facing_angle +
                                                       Grapple_RopeSwingRotate * FRAMETIME);
    }
    object->grapple_swing_phase = static_cast<u16>(
        object->grapple_swing_phase + static_cast<i32>(0x4000 * FRAMETIME / (object->field_0x768 * 2.0f + 0.5f)));
    if (object->pad_gamepad->input_magnitude > 0.0f) {
        object->grapple_swing_degrees = static_cast<u8>(MIN(60, object->grapple_swing_degrees + 10));
    } else if (object->grapple_swing_degrees >= 10) {
        object->grapple_swing_degrees -= 10;
    } else {
        object->grapple_swing_degrees = 0;
    }

    if (object->field_0x768 <= 0.0f &&
        (grapple->flags & (GRAPPLE_FLAG_DISABLED | GRAPPLE_FLAG_REVERSED)) == 0) {
        object->airborne_action_duration += FRAMETIME;
        if (object->airborne_action_duration >= 0.5f) {
            object->field_0x7a3 = 1;
            object->context_animation =
                LEGOACT_GRAPPLE_HANG != -1 &&
                        object->apiobj.character_model->model_data_b[LEGOACT_GRAPPLE_HANG] != NULL
                    ? LEGOACT_GRAPPLE_HANG
                    : LEGOACT_LEDGE_IDLE;
            NUVEC offset = GrapplePointOffset;
            NuVecRotateX(&offset, &offset, grapple->x_rotation);
            offset.z += object->apiobj.field_0x1dc;
            NuVecRotateY(&offset, &offset, grapple->y_rotation);
            NuVecAdd(&object->external_force, &grapple->position, &offset);
            object->apiobj.facing_angle = static_cast<u16>(grapple->y_rotation + 0x8000);
        }
    } else {
        object->airborne_action_duration = 0.0f;
    }
}

i32 Grapple_ReachedTop(GameObject_s *object) {
    if (LEGOCONTEXT_GRAPPLE != -1 && object->character_context == LEGOCONTEXT_GRAPPLE) {
        if (object->field_0x7a3 == 1 ||
            (object->field_0x7a3 == 0 && object->airborne_action_duration >= 0.5f)) {
            return 1;
        }
    }
    return 0;
}

void Grapple_SetPlayerTargetPoint(GameObject_s *object, nuvec_s *target) {
    target->x = 0.0f;
    target->z = 0.0f;
    target->y = -object->field_0x768;
    i32 amplitude = (static_cast<i32>(object->grapple_swing_degrees) << 16) / 360;
    i32 angle = static_cast<i32>(static_cast<f32>(amplitude) * NuTrigTable[object->grapple_swing_phase >> 1] *
                                 static_cast<GRAPPLE *>(object->field_0x788)->field_0x4c);
    NuVecRotateX(target, target, angle);
    NuVecRotateY(target, target, object->takeover_start_angle);
    NuVecAdd(target, target, &static_cast<GRAPPLE *>(object->field_0x788)->shadow_probe_position);
}

// Original 0x4d6f90, 248 bytes.
i32 Grapple_SetTargetMom(GameObject_s *object) {
    if (object->field_0x7a3 == 1) {
        object->target_velocity.x = (object->external_force.x - object->apiobj.upper_position.x) * 20.0f;
        object->target_velocity.y = (object->external_force.y - object->apiobj.upper_position.y) * 20.0f;
        object->target_velocity.z = (object->external_force.z - object->apiobj.upper_position.z) * 20.0f;
    } else {
        NUVEC point;
        Grapple_SetPlayerTargetPoint(object, &point);
        object->target_velocity.x = (point.x - object->apiobj.upper_position.x) * 5.0f;
        object->target_velocity.y = (point.y - object->apiobj.upper_position.y) * 5.0f;
        object->target_velocity.z = (point.z - object->apiobj.upper_position.z) * 5.0f;
    }
    return 1;
}

// Original 0x4d7090, 23 bytes.
void Grapple_SetRotOrder(GameObject_s *object) {
    if (object->field_0x7a3 != 1)
        object->field_0x1086 = 2;
}

i32 Grapple_LookAtPos(GameObject_s *object, NUVEC *position) {
    position->x = static_cast<GRAPPLE *>(object->field_0x788)->hook_position.x;
    position->y = static_cast<GRAPPLE *>(object->field_0x788)->hook_position.y - object->field_0x768;
    position->z = static_cast<GRAPPLE *>(object->field_0x788)->hook_position.z;
    return 1;
}
