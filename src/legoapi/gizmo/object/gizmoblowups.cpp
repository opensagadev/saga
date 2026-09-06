#include "decomp.h"
#include "batman.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/numtx.h"

#include <string.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

static char gizmoblowupnametable[32][32];
static i32 gizmoblowupnametable_numids;
i32 GizmoBlowup_HitMultiplier = 1;
void (*CheckLostDataFn)(GIZMOBLOWUP_s *) = NULL;

void GizBlowup_DeleteTerrain();
void GizmoBlowupUpdateMatrix(GIZMOBLOWUP_s *blowup);
void GizmoBlowupCreateStuff(GIZMOBLOWUP_s *blowup);
void GizmoBlowUp_AddEffects(NUVEC *position, GIZMOBLOWUP_s *blowup, i32 count, i32 flags, GameObject_s *object);
i32 ObjHitObj(GameObject_s *attacker, GameObject_s *target, i32 type, u16 damage, i32 flags, i32 context);
void GameAntinode_UnregisterAntiNode(GAMEANTINODESYS_s *system, GAMEANTINODE_s *node);
extern "C" void PlatOnOff(i32 platform, i32 enabled);
extern "C" i32 NewPlatInst(void *object, i32 instance);
extern "C" i32 FindPlatInst(i32 instance);
extern "C" i32 DeletePlatinst(i32 platform);
extern "C" void PlatInstRotate(i32 platform, i32 rotate);
i32 GizBlowup_InitSingleTerrain(GIZMOBLOWUP_s *blowup);
void GizBlowup_DeleteSingleTerrain(GIZMOBLOWUP_s *blowup);

void GizmoBlowupDraw(void *, void *, float) {
}

void GizmoSortBlowups(WORLDINFO_s *) {
}

void GizmoSwapBlowups(GIZMOBLOWUP_s *, GIZMOBLOWUP_s *) {
}

i32 InitGizmoBlowups(WORLDINFO_s *world) {
    world->gizmo_blowups = NULL;
    world->gizmo_blowup_count = 0;
    if (world->current_level->max_gizmo_blowups == 0) {
        return 0;
    }

    world->giz_buffer.addr = (world->giz_buffer.addr + 0xf) & ~static_cast<usize>(0xf);
    world->gizmo_blowups = static_cast<GIZMOBLOWUP_s *>(GameBufferAlloc(
        &world->giz_buffer, &world->unknown_0108, world->current_level->max_gizmo_blowups * sizeof(GIZMOBLOWUP_s)));
    memset(world->gizmo_blowups, 0, world->current_level->max_gizmo_blowups * sizeof(GIZMOBLOWUP_s));
    return world->gizmo_blowups != NULL;
}

extern "C" void AddFiniteShotDebrisEffect(i32 *, i32, NUVEC *, i32);

void GizBlowup_Respawn(GIZMOBLOWUP_s *blowup) {
    if (blowup == NULL) return;
    blowup->state_flags |= 0x80;
    blowup->output_flags &= ~1;
    blowup->field_0x9f &= ~1;
    blowup->visibility_flags = (blowup->visibility_flags & 0x7f) | 0x40;
    blowup->saved_state_1 = blowup->initial_state_1;
    blowup->saved_state_0 = blowup->initial_state_0;
    nuinstanim_s *animation = NuSpecialGetInstAnim(&blowup->type->animated_special);
    if (animation != NULL && animation->playing != 0) {
        blowup->state_flags |= 0x10;
        if (animation->repeating != 0) blowup->state_flags |= 0x48;
    }
    blowup->state_flags |= 1;
    if (BonusArea != 0 && VehicleArea != 0 && (blowup->draw_flags & 0x1000000) != 0) {
        if (NuSpecialExistsFn(&blowup->type->animated_special)) {
            NuSpecialSetVisibility(&blowup->type->animated_special, 0);
        }
    } else if ((blowup->visibility_flags & 0x40) != 0) {
        NuSpecialSetVisibility(&blowup->type->animated_special, 1);
    }
    if (NuSpecialExistsFn(&blowup->type->decal_special)) {
        NuSpecialSetVisibility(&blowup->type->decal_special, 0);
    }
    blowup->animation_time = 0.0f;
    blowup->output_flags &= 1;
    blowup->visibility_flags &= 0xe4;
    blowup->state_flags &= 0xf1;
    blowup->saved_state_0 = blowup->initial_state_0;
    blowup->field_0x9f &= ~2;
    blowup->saved_state_1 = blowup->initial_state_1;
    animation = NuSpecialGetInstAnim(&blowup->type->animated_special);
    if (animation != NULL) {
        if (BonusArea != 0 && VehicleArea != 0 && (blowup->draw_flags & 0x1000000) != 0 &&
            (blowup->state_flags & 0x10) != 0) {
            animation->playing = 1;
        }
        if (animation->playing != 0 && animation->repeating != 0) blowup->state_flags |= 0x48;
    }
    if (blowup->platform_id != -1) PlatOnOff(blowup->platform_id, 1);
    if (blowup->type->particle_types[2] != -1) {
        i32 handle = -1;
        AddFiniteShotDebrisEffect(&handle, blowup->type->particle_types[2], &blowup->mid_position, 1);
    }
    if (blowup->type->particle_types[3] != -1) {
        i32 handle = -1;
        AddFiniteShotDebrisEffect(&handle, blowup->type->particle_types[3], &blowup->mid_position, 1);
    }
}

i32 GizmoBlowupBlowup(GIZMOBLOWUP_s *blowup, i32 effects, i32 hit_type, i32 damage, GameObject_s *object,
                     i32 hit_context) {
    if (blowup == NULL || (blowup->state_flags & 0x80) == 0 ||
        ((blowup->draw_flags & 0x20) && ShadowMode == 0) ||
        !TouchHacks::CanBlowupBeBlownUp(*blowup, hit_type)) {
        return 0;
    }
    nuinstanim_s *animation = NuSpecialGetInstAnim(&blowup->type->animated_special);
    GIZMOBLOWUPTYPE_s *type = blowup->type;
    if (animation != NULL && type->animation_start_frame != type->animation_end_frame) {
        if ((type->animation_flags & 0x20) == 0 && (blowup->state_flags & 0x40) == 0) {
            if ((blowup->draw_flags & 0x208) == 0) {
                if (blowup->animation_time < type->animation_end_frame) {
                    blowup->state_flags |= 8;
                    return 0;
                }
                blowup->state_flags &= ~8;
            }
        } else {
            const f32 frame = (blowup->draw_flags & 0x800000) ? animation->ltime : blowup->animation_time;
            if (frame < type->animation_start_frame || type->animation_end_frame < frame) {
                return 0;
            }
        }
    }
    switch (hit_type) {
    case 1: blowup->output_flags |= 2; break;
    case 2: blowup->output_flags |= 4; break;
    case 3: blowup->output_flags |= 8; break;
    case 4: blowup->output_flags |= 0x20; break;
    case 5: blowup->output_flags |= 0x10; break;
    case 6: blowup->output_flags |= 0x40; break;
    case 7: blowup->output_flags |= 0x80; break;
    case 8: blowup->visibility_flags |= 1; break;
    case 9: blowup->visibility_flags |= 2; break;
    case 10: blowup->visibility_flags |= 4; break;
    case 11: blowup->visibility_flags |= 8; break;
    case 12: blowup->visibility_flags |= 0x10; break;
    case 13: blowup->visibility_flags |= 0x20; break;
    }
    const i32 has_burst = NuSpecialExistsFn(&type->burst_special);
    if (has_burst) {
        nuinstanim_s *burst = NuSpecialGetInstAnim(&type->burst_special);
        if (burst != NULL) {
            burst->playing = 1;
        }
    }
    bool destroyed = false;
    u8 effect_flags = 1;
    if (damage != -1) {
        blowup->saved_state_1 = static_cast<u8>(blowup->saved_state_1 - damage);
    }
    if (damage == -1 || static_cast<i8>(blowup->saved_state_1) < 1) {
        blowup->saved_state_1 = 0;
        if ((type->animation_flags & 0x20) && (blowup->draw_flags & 0x800000)) {
            NuSpecialSetVisibility(&type->animated_special, 0);
        }
        blowup->output_flags |= 1;
        blowup->visibility_flags &= 0x3f;
        blowup->field_0x9f &= ~1;
        blowup->state_flags &= 0x73;
        blowup->animation_time = 1.0f;
        if (object == NULL) {
            if ((blowup->draw_flags & 0x208) && blowup->saved_state_0 != 0 &&
                (blowup->field_0xb4 > 0.0f || blowup->field_0xb8 <= 0.0f)) {
                const f32 radius_squared = blowup->field_0xb4 * blowup->field_0xb4;
                for (i32 player = 0; player < 8; ++player) {
                    GameObject_s *target = Player[player];
                    if (target != NULL && (target->apiobj.field_0x1f8 & 0x1001) == 0x1001 &&
                        target->apiobj.field_0x287 == 0 && target->field_0x101c <= 0.0f) {
                        const f32 x = blowup->mid_position.x - target->apiobj.position.x;
                        const f32 y = blowup->mid_position.y - target->apiobj.position.y;
                        const f32 z = blowup->mid_position.z - target->apiobj.position.z;
                        if (x * x + y * y + z * z < radius_squared) {
                            ObjHitObj(NULL, target, blowup->saved_state_0, 1, 0, hit_context);
                        }
                    }
                }
            }
        } else if (blowup->saved_state_0 != 0 &&
                   (object->id != id_SPEEDERBIKE || (object->apiobj.flags_low & 0x80) == 0 ||
                    WORLD->current_level != SPEEDERCHASEA_LDATA || disable_narrow_socks != 0)) {
            ObjHitObj(NULL, object, blowup->saved_state_0, 1, 0, hit_context);
        }
        if (blowup->platform_id != -1) {
            PlatOnOff(blowup->platform_id, 0);
            if (has_burst && blowup->field_0x10c != -1) {
                PlatOnOff(blowup->field_0x10c, 1);
            }
            if (type->animation_flags & 0x20) {
                NuSpecialSetVisibility(&type->animated_special, 0);
            }
        }
        if ((blowup->draw_flags & 0x400) == 0 && NuSpecialExistsFn(&blowup->type->decal_special)) {
            blowup->animation_time = 0.1f;
            blowup->state_flags |= 2;
        }
        if (blowup->draw_flags & 0x800) {
            GizmoBlowupCreateStuff(blowup);
        }
        if (GameBlowUpBlownUpFn != NULL) {
            GameBlowUpBlownUpFn(blowup);
        }
        if ((blowup->output_flags & 1) && CheckLostDataFn != NULL) {
            CheckLostDataFn(blowup);
        }
        if (blowup->field_0xd8 > 0.0f) {
            blowup->respawn_timer = blowup->field_0xd8;
        }
        effect_flags = (blowup->draw_flags & 0x40000) ? 0x17 : 7;
        if (blowup->anti_node != NULL) {
            GameAntinode_UnregisterAntiNode(WORLD->game_antinode_sys, blowup->anti_node);
            blowup->anti_node = NULL;
        }
        destroyed = true;
    } else if (blowup->state_flags & 0x20) {
        effect_flags = 4;
    }
    if (blowup->field_0xb8 > 0.0f && destroyed) {
        effect_flags |= 8;
    }
    if (effects != 0) {
        GizmoBlowUp_AddEffects(&blowup->mid_position, blowup, 1, effect_flags, object);
    }
    return 1;
}

void BlowupObjEmit_Stop(PART_s *) {
}

void GizmoBlowupTypeAdd(WORLDINFO_s *, nuhspecial_s *, i32, i32 *) {
}

GIZMOBLOWUPTYPE_s *GizmoBlowup_FindType(char *name, WORLDINFO_s *world) {
    if (world == NULL || world->gizmo_blowup_types == NULL || world->gizmo_blowup_type_count == 0 || name == NULL) {
        return NULL;
    }
    for (i32 index = 0; index < world->gizmo_blowup_type_count; ++index) {
        if (NuStrCmp(world->gizmo_blowup_types[index].name, name) == 0) {
            return &world->gizmo_blowup_types[index];
        }
    }
    return NULL;
}

i32 InitGizmoBlowupTypes(WORLDINFO_s *world) {
    world->gizmo_blowup_types = NULL;
    world->gizmo_blowup_type_count = 0;
    if (world->current_level->max_gizmo_blowup_types == 0) {
        return 0;
    }
    world->gizmo_blowup_types = static_cast<GIZMOBLOWUPTYPE_s *>(
        GameBufferAlloc(&world->giz_buffer, &world->unknown_0108,
                        world->current_level->max_gizmo_blowup_types * sizeof(GIZMOBLOWUPTYPE_s)));
    return world->gizmo_blowup_types != NULL;
}

void SetGizmoBlowUpTarget(GameObject_s *, GIZMOBLOWUP_s *) {
}

void GizBlowup_InitTerrain() {
    if (WORLD->gizmo_blowups != NULL) {
        for (i32 i = 0; i < WORLD->gizmo_blowup_count; ++i) {
            GIZMOBLOWUP_s *blowup = &WORLD->gizmo_blowups[i];
            blowup->platform_id = -1;
            blowup->field_0x10c = -1;
            if ((blowup->draw_flags & 4) != 0) GizBlowup_InitSingleTerrain(blowup);
        }
    }
}

void GizmoBlowupTypeRemove(GIZMOBLOWUPTYPE_s *, WORLDINFO_s *) {
}

void GizmoBlowup_HitBlowup(GameObject_s *, GIZMOBLOWUP_s *, i32, BOLT_s *, nuvec_s *, unsigned char *, u32, i32) {
}

void FindNearestGizmoBlowUp(WORLDINFO_s *, nuvec_s *, float) {
}

void GizmoBlowupCreateStuff(GIZMOBLOWUP_s *) {
}

void GizmoBlowupsFinalSetup(WORLDINFO_s *world) {
    for (i32 type_index = 0; type_index < world->gizmo_blowup_type_count; ++type_index) {
        GIZMOBLOWUPTYPE_s *type = &world->gizmo_blowup_types[type_index];
        type->animation_runtime_flags &= ~1;
        nuinstanim_s *animation = NuSpecialGetInstAnim(&type->animated_special);
        type->animation_base_frame = animation != NULL ? animation->ltime : 0.0f;
        if (NuSpecialExistsFn(&type->decal_special) != 0) {
            NuSpecialSetVisibility(&type->decal_special, 0);
        }
    }

    for (i32 instance_index = 0; instance_index < world->gizmo_blowup_count; ++instance_index) {
        GIZMOBLOWUP_s *blowup = &world->gizmo_blowups[instance_index];
        blowup->animation_time = 1.0f;
        blowup->field_0x9f &= ~0x08;
        blowup->visibility_flags &= ~0x03;
        blowup->output_flags = 0;
        blowup->state_flags = (blowup->state_flags & 0xb7) | 0x81;

        nuhspecial_s *special = blowup->override_special;
        if (special == NULL || NuSpecialExistsFn(special) == 0) {
            special = &blowup->type->animated_special;
        }

        if (NuSpecialExistsFn(special) == 0) {
            blowup->bounds_min = v000;
            blowup->bounds_max = v000;
        } else {
            if (NuSpecialExistsFn(&blowup->type->burst_special) != 0) {
                NuSpecialSetVisibility(&blowup->type->burst_special, 0);
            }
            if ((blowup->type->animation_flags & 0x20) != 0) {
                blowup->position = *NuSpecialGetDrawPos(special);
            }

            NUVEC centre;
            NuSpecialGetRadius(special, &centre, &blowup->target_scale);
            centre.x += blowup->position.x;
            centre.y += blowup->position.y;
            centre.z += blowup->position.z;
            blowup->bounds_min.x = centre.x - blowup->target_scale;
            blowup->bounds_min.y = centre.y - blowup->target_scale;
            blowup->bounds_min.z = centre.z - blowup->target_scale;
            blowup->bounds_max.x = centre.x + blowup->target_scale;
            blowup->bounds_max.y = centre.y + blowup->target_scale;
            blowup->bounds_max.z = centre.z + blowup->target_scale;

            nuinstanim_s *animation = NuSpecialGetInstAnim(special);
            if (animation != NULL && animation->playing != 0) {
                const u8 original_state = blowup->state_flags;
                blowup->state_flags = original_state | 0x10;
                if (animation->repeating != 0) {
                    blowup->state_flags = original_state | 0x58;
                }
            }
        }

        GizmoBlowupUpdateMatrix(blowup);
        blowup->initial_state_1 = static_cast<u8>(blowup->initial_state_1 * GizmoBlowup_HitMultiplier);
    }

    GizBlowup_DeleteTerrain();
    GizBlowup_InitTerrain();
}

void GizBlowup_DeleteTerrain() {
    if (WORLD->gizmo_blowups != NULL) {
        for (i32 i = 0; i < WORLD->gizmo_blowup_count; ++i) {
            GizBlowup_DeleteSingleTerrain(&WORLD->gizmo_blowups[i]);
        }
    }
}

void GizmoBlowupTypeGetCount(WORLDINFO_s *) {
}

void GizmoBlowupUpdateMatrix(GIZMOBLOWUP_s *blowup) {
    if (blowup == NULL) {
        return;
    }

    NUVEC position = blowup->position;
    blowup->transform = *NuSpecialGetDrawMtx(&blowup->type->animated_special);
    *NUMTX_GET_ROW_VEC(&blowup->transform, 3) = v000;
    NuMtxRotateY(&blowup->transform, blowup->field_0xf2);
    NuMtxPreRotateX(&blowup->transform, blowup->field_0xf0);
    NuMtxPreRotateY(&blowup->transform, blowup->field_0xf4);
    NuMtxTranslate(&blowup->transform, &position);
}

void GizmoBlowups_TotalScore(void *) {
}

void GizmoBlowupTypeNameBlank(char *) {
}

void GizmoBlowupCheckProximity(WORLDINFO_s *, GameObject_s *) {
}

void GizmoBlowupGenDecalMatrix(GIZMOBLOWUP_s *, numtx_s *, i32) {
}

i32 GizmoBlowupGetNameTableId(char *name) {
    for (i32 id = 0; id < gizmoblowupnametable_numids; ++id) {
        if (NuStrICmp(gizmoblowupnametable[id], name) == 0) {
            return id;
        }
    }

    if (gizmoblowupnametable_numids >= 32) {
        return -1;
    }

    NuStrNCpy(gizmoblowupnametable[gizmoblowupnametable_numids], name, sizeof(gizmoblowupnametable[0]));
    return gizmoblowupnametable_numids++;
}

i32 InitGizmoBlowupsMtxBuffer(WORLDINFO_s *world) {
    world->gizmo_blowup_mtx_buffer = NULL;
    world->giz_buffer.addr = (world->giz_buffer.addr + 0x7f) & ~static_cast<usize>(0x7f);
    world->gizmo_blowup_mtx_buffer =
        static_cast<NUMTX *>(GameBufferAlloc(&world->giz_buffer, &world->unknown_0108, 0x8000));
    return world->gizmo_blowup_mtx_buffer != NULL;
}

void RemapTypeFlagToBlowupFlag(u32) {
}

void GizmoBlowupGenShadowMatrix(GIZMOBLOWUP_s *, numtx_s *) {
}

i32 GizBlowup_InitSingleTerrain(GIZMOBLOWUP_s *blowup) {
    if (blowup == NULL || (blowup->output_flags & 1) != 0 || (blowup->draw_flags & 4) == 0) return 0;
    if (blowup->platform_id == -1) {
        i32 instance = NuSpecialGetInstanceix(&blowup->type->animated_special);
        if (blowup->override_special != NULL && NuSpecialExistsFn(blowup->override_special)) {
            instance = NuSpecialGetInstanceix(blowup->override_special);
            blowup->platform_id = FindPlatInst(instance);
        } else if ((blowup->draw_flags & 0x802000) == 0x2000 || (blowup->draw_flags & 0x20000000) == 0) {
            blowup->platform_id = NewPlatInst(blowup, instance);
        } else {
            blowup->platform_id = FindPlatInst(instance);
        }
        if (blowup->platform_id == -1) return 0;
        if ((blowup->visibility_flags & 0x40) == 0) PlatOnOff(blowup->platform_id, 0);
    }
    PlatInstRotate(blowup->platform_id, 1);
    if (blowup->field_0x10c == -1 && NuSpecialExistsFn(&blowup->type->burst_special)) {
        blowup->field_0x10c = NewPlatInst(blowup, NuSpecialGetInstanceix(&blowup->type->burst_special));
        if (blowup->field_0x10c == -1) return -1;
    }
    PlatInstRotate(blowup->field_0x10c, 1);
    return 1;
}

void GizBlowup_DeleteSingleTerrain(GIZMOBLOWUP_s *blowup) {
    if (blowup == NULL) return;
    if ((blowup->override_special == NULL || !NuSpecialExistsFn(blowup->override_special)) && blowup->platform_id != -1) {
        DeletePlatinst(blowup->platform_id);
        blowup->platform_id = -1;
    }
    if (blowup->field_0x10c != -1) {
        DeletePlatinst(blowup->field_0x10c);
        blowup->field_0x10c = -1;
    }
}

void GizmoBlowupVisibilityOverrides(WORLDINFO_s *) {
}

void GizmoBlowup_SetAutoSetReflectY(GIZMOBLOWUP_s *, nuvec_s *) {
}

extern void Transform_DrawTarget(NUVEC *position, f32 scale, f32 opacity);
extern i32 Transform_TargettedByObj(void *object);

void GizmoBlowup_TransformDraw_Game(GIZMOBLOWUP_s *blowup) {
    if (Transform_TargettedByObj(blowup) != 0) {
        return;
    }

    Transform_DrawTarget(&blowup->mid_position, 1.4f * blowup->target_scale, 0.4f);
}

void RemapAllTypeFlagsToBlowupFlags(u32) {
}

void GizmoBlowupTypeGetNameFromIndex(WORLDINFO_s *, i32) {
}

i32 GizmoBlowupGetTypeFromNameTableId(WORLDINFO_s *world, i32 name_id) {
    if (name_id < 0 || name_id >= gizmoblowupnametable_numids) {
        return -1;
    }

    const char *name = gizmoblowupnametable[name_id];
    for (i32 type_index = 0; type_index < world->gizmo_blowup_type_count; ++type_index) {
        if (NuStrICmp(world->gizmo_blowup_types[type_index].name, name) == 0) {
            return type_index;
        }
    }

    return -1;
}
