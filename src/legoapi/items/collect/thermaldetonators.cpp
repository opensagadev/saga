// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "globals.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/world/world.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/characters/core/players.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/ai/core/gameai.h"
#include "legoapi/items/collect/thermaldetonators.h"
#include "legoapi/props/objects/zipup.h"

struct AIROW_s;
struct nunativegscene_s;

MechWeakPtr<MechObjectInterface, 4> NextThermalTarget;

i32 MatrixReflection(NUMTX *matrix, i32 axis, f32 plane, f32 height, NUMTX *result);
i32 SuperWeirdo(GameObject_s *object);
extern i32 TERRAINMASK_NONWEAPON;
extern "C" f32 brickimpactwait;

// Each control mode retains its own requested and interpolated hint row.

void PartKill_ThermalDetonator(PART_s *part, i32) {
    if (part == NULL) {
        return;
    }

    if (part->owner != NULL) {
        AlertSurroundingCreatures(part->owner, &part->position);
    }

    u16 flags = 0x200;
    if (part->owner != NULL && part->owner->apiobj.player_controlled && Cheat_IsOn(0x17) != 0) {
        flags = 0x1200;
    }

    EXPLOSION *explosion = Detonate(&part->position, flags);
    if (explosion != NULL && part->owner != NULL && static_cast<u8>(part->owner->apiobj.field_0x27c) <= 1) {
        explosion->field_0x33 = static_cast<u8>(part->owner->apiobj.field_0x27c);
    }
}

i32 PartDraw_ThermalDetonator(PART_s *part) {
    const i32 draw = PartDraw_Flickerer(part);
    NUMTX matrix = part->transform;

    NUMTX reflection;
    i32 reflected = 0;
    if ((part->reflection_flags & 2) != 0 && part->reflection_height != 2000000.0f) {
        reflected =
            MatrixReflection(&matrix, 2, part->reflection_height, WORLD->current_level->unknown_0cc, &reflection);
    }

    i32 level_index = -1;
    if (WORLD->lev_objs[0xea].active != 0 && WORLD->lev_objs[0xeb].active != 0) {
        level_index = draw == 1 ? 0xea : 0xeb;
        NuSpecialDrawAt(&WORLD->lev_objs[level_index].special, &matrix);
    }

    if (reflected != 0) {
        NuRndrStartReflectionRender(1);
        if (part->source_special != NULL) {
            NuSpecialDrawAt(&part->special, &reflection);
        }
        if (level_index != -1) {
            NuSpecialDrawAt(&WORLD->lev_objs[level_index].special, &reflection);
        }
        NuRndrEndReflectionRender();
    }
    return 1;
}

void PartUpdate_ThermalDetonator(PART_s *part) {
    if (part == NULL) {
        return;
    }
    const u16 flags = static_cast<u16>(part->active) | (static_cast<u16>(part->render_flags) << 8);
    if ((flags & 0x4002) == 2) {
        if (part->field_100 > 0.0f && part->field_100 < 1.0f) {
            PlaySfx(const_cast<char *>("ThermalDet_Beep"), &part->position);
            part->render_flags |= 0x40;
            return;
        }
    }
    if ((part->active & 2) != 0) {
        return;
    }

    part->reflection_flags &= ~2;
    NewTerrPlatformsOff();
    f32 height = GameShadow(NULL, &part->position, 5.0f, -1);
    if (height == 2000000.0f) {
        return;
    }
    if (part->position.y > height) {
        i32 surface = ShadowInfo();
        if (static_cast<u32>(surface) <= 16 && (TerSurface[surface].flags & 2) != 0) {
            part->reflection_height = height;
            part->reflection_flags |= 2;
        }
    }
    f32 water = EShadY;
    if (water == 2000000.0f) {
        return;
    }
    i32 layer = EShadowInfo();
    if (static_cast<u32>(layer) > 16) {
        return;
    }
    if ((part->render_flags & 0x80) == 0 && water > part->position.y + part->radius &&
        ((TerLayer[layer].flags & 1) != 0 || (layer & ~8) == 1)) {
        part->render_flags |= 0x80;
    }
    if (WORLD != NULL && GameCam != NULL && WORLD->current_level == DEATHSTARRESCUEA_LDATA &&
        GameCam->sock_position.location.sock == 2) {
        part->velocity.x = SeekValF(part->velocity.x, 0.0f, 3.0f);
        part->velocity.z = SeekValF(part->velocity.z, 0.0f, 3.0f);
    }
}

void PartImpact_ThermalDetonator(PART_s *part) {
    if (part == NULL) {
        return;
    }
    if ((part->render_flags & 0x80) != 0 || part->field_209 == 0x1c) {
        KillPart(part, 0);
        return;
    }

    WORLDINFO *world = WorldInfo_CurrentlyActive();
    bool on_blowup = false;
    if (world != NULL && world->current_level == JEDI_B_LDATA && LevBlowUp[0] != NULL) {
        i32 platform = LevBlowUp[0]->platform_id;
        if (platform == static_cast<i32>(part->field_200)) {
            on_blowup = platform != -1;
        }
    }

    i32 stuck = 0;
    i8 surface = static_cast<i8>(part->field_209);
    if (surface >= 0 && surface < 32 && ((TerSurface[surface].flags & 0x1000) != 0 || on_blowup)) {
        PlaySfx(const_cast<char *>("imp_thermalDet_attach"), &part->position);
        stuck = 2;
    } else {
        GameShadow(NULL, &part->position, 5.0f, -1);
        u32 shadow = ShadowInfo();
        if (shadow < 32) {
            if ((TerSurface[shadow].flags & 0x1000) != 0) {
                PlaySfx(const_cast<char *>("imp_thermalDet_attach"), &part->position);
                stuck = 2;
            } else {
                f32 water = EShadY;
                if (water != 2000000.0f && (EShadowInfo() & ~8) == 1 && water > part->position.y) {
                    PlaySfx(const_cast<char *>("FS_WaterJump"), &part->position);
                    stuck = 1;
                }
            }
        }
    }
    if (stuck != 0) {
        void (*stop_callback)(PART_s *) = part->stop_callback;
        part->active |= 2;
        if (stop_callback != NULL) {
            stop_callback(part);
        }
    }
    if (stuck != 2 && brickimpactwait <= 0.0f) {
        PartImpact_Brick(part);
        PlaySfx(const_cast<char *>("ThermalDet_Bnce"), &part->position);
    }
    if ((part->active & 3) == 1 && WORLD != NULL) {
        NUVEC trail = {
            part->impact_position.x - part->impact_normal.x * part->radius,
            part->impact_position.y - part->impact_normal.y * part->radius,
            part->impact_position.z - part->impact_normal.z * part->radius,
        };
        AddGameDebris(WORLD->debris_sys, 1, &trail);
    }
}

void ThermalDetonator_Throw(GameObject_s *object) {
    NUVEC movement;
    ADDPART_s params = Default_ADDPART;
    if (!(object == NULL || WORLD == NULL || WORLD->lev_objs == NULL || WORLD->lev_objs[0xe9].active == 0)) {
        if (object->apiobj.player_controlled) {
            Hint_SetComplete(0x2b8);
            Hint_SetComplete(0x283);
            Hint_SetComplete(0x61d);
        }
        NUMTX matrix;
        bool blocked = false;
        const i32 locator =
            object->apiobj.character_data != NULL && object->apiobj.character_data->game_character != NULL
                ? object->apiobj.character_data->game_character->throw_locator
                : -1;
        if (locator < 0 || locator >= 16 || object->apiobj.character_model == NULL ||
            object->apiobj.character_model->points_of_interest[locator] == NULL) {
            NuMtxSetTranslation(&matrix, &object->apiobj.collision_position);
        } else {
            if (object->id == id_JANGOFETT && object->context_animation == 0x6e) {
                NuMtxSetRotationY(&matrix, qrand());
                NuMtxRotateZ(&matrix, qrand());
                NuMtxRotateX(&matrix, qrand());
                NuMtxTranslate(&matrix, NUMTX_GET_ROW_VEC(&object->joint_matrices[locator], 3));
            } else {
                matrix = object->joint_matrices[locator];
                NuVecNorm(NUMTX_GET_ROW_VEC(&matrix, 0), NUMTX_GET_ROW_VEC(&matrix, 0));
                NuVecNorm(NUMTX_GET_ROW_VEC(&matrix, 1), NUMTX_GET_ROW_VEC(&matrix, 1));
                NuVecNorm(NUMTX_GET_ROW_VEC(&matrix, 2), NUMTX_GET_ROW_VEC(&matrix, 2));
            }
            NUVEC origin = {
                object->apiobj.collision_position.x,
                (object->apiobj.collision_max.y - object->apiobj.collision_min.y) * 0.75f +
                    object->apiobj.collision_min.y,
                object->apiobj.collision_position.z,
            };

            NuVecSub(&movement, NUMTX_GET_ROW_VEC(&matrix, 3), &origin);
            blocked = GameRayCast(&origin, &movement, 0.0f, TERRAINMASK_NONWEAPON | 0x1f) != 0;
        }
        NUVEC velocity;
        if (NextThermalTarget.Get() == NULL) {
            const u16 angle = object->apiobj.movement_facing_angle;
            velocity.x = NU_SIN_LUT(angle) + NU_SIN_LUT(angle) + object->apiobj.velocity.x;
            velocity.z = NU_COS_LUT(angle) + NU_COS_LUT(angle) + object->apiobj.velocity.z;
        } else {
            VuVec target;
            NextThermalTarget->GetPos(target, -1);
            const VuVec origin(object->apiobj.position.x, object->apiobj.position.y, object->apiobj.position.z, 1.0f);
            const VuVec arc = TouchHacks::CalculateXZVelForArcToHitPoint(origin, target, 2.0f, -5.0f);
            NextThermalTarget.Reset();
            // The retail path clamps only X; Z retains the calculated arc velocity.
            velocity.z = arc.z;
            velocity.x = arc.x < 3.0f ? (arc.x < -3.0f ? -3.0f : arc.x) : 3.0f;
        }
        velocity.y = 2.0f;
        if (blocked) {
            const u16 angle = NuAtan2D(object->apiobj.collision_position.x - matrix.m30,
                                       object->apiobj.collision_position.z - matrix.m32);
            velocity.x = NU_SIN_LUT(angle) + NU_SIN_LUT(angle);
            velocity.z = NU_COS_LUT(angle) + NU_COS_LUT(angle);
            matrix.m30 = object->apiobj.collision_position.x;
            matrix.m32 = object->apiobj.collision_position.z;
        }

        params.matrix = &matrix;
        params.velocity = &velocity;
        params.owner = object;
        NUVEC centre;
        NuSpecialGetRadius(&WORLD->lev_objs[0xe9].special, &centre, &params.field_14);
        params.field_18 = params.field_14;
        params.field_14 *= 0.75f;
        params.field_c4 = 1;
        params.gravity = -5.0f;
        params.special = &WORLD->lev_objs[0xe9].special;
        params.flags = 0x08000292;
        params.field_40 = PartCollide_3D;
        params.field_44 = PartKill_ThermalDetonator;
        params.field_3c = PartImpact_ThermalDetonator;
        params.stop_fn = PartStop_Flickerer;
        params.draw_fn = PartDraw_ThermalDetonator;
        params.time_step = FRAMETIME;
        params.field_a4 = 10.0f;

        PART_s *part = AddPart(&params);
        if (part != NULL) {
            part->force_flags = ObjHitObj_Flags(object) & 0xffff;
            part->force_player_mask = 0;
            part->field_1c4 = PartUpdate_ThermalDetonator;
            part->reflection_height = 2000000.0f;
            part->render_flags &= ~0x80;
            part->reflection_flags &= ~3;
        }
        PlaySfx(const_cast<char *>("ThrowDet"), &object->apiobj.collision_position);
        if (object->pad_gamepad != NULL) {
            NewBuzzFrames(object->pad_gamepad->pad, 2, 0);
        }
    }
}

void ThermalDetonator_ThrowMom(GameObject_s *object, nuvec_s *velocity) {
    NUVEC movement;
    ADDPART_s params = Default_ADDPART;
    if (!(object == NULL || WORLD == NULL || WORLD->lev_objs == NULL || WORLD->lev_objs[0xe9].active == 0)) {
        // This entry point creates the thrown part with supplied momentum; it
        // does not calculate or modify the caller's vector.
        NUMTX matrix;
        const i32 locator =
            object->apiobj.character_data != NULL && object->apiobj.character_data->game_character != NULL
                ? object->apiobj.character_data->game_character->throw_locator
                : -1;
        if (locator < 0 || locator >= 16 || object->apiobj.character_model == NULL ||
            object->apiobj.character_model->points_of_interest[locator] == NULL) {
            NuMtxSetTranslation(&matrix, &object->apiobj.collision_position);
        } else {
            if (object->id == id_JANGOFETT && object->context_animation == 0x6e) {
                NuMtxSetRotationY(&matrix, qrand());
                NuMtxRotateZ(&matrix, qrand());
                NuMtxRotateX(&matrix, qrand());
                NuMtxTranslate(&matrix, NUMTX_GET_ROW_VEC(&object->joint_matrices[locator], 3));
            } else {
                matrix = object->joint_matrices[locator];
                NuVecNorm(NUMTX_GET_ROW_VEC(&matrix, 0), NUMTX_GET_ROW_VEC(&matrix, 0));
                NuVecNorm(NUMTX_GET_ROW_VEC(&matrix, 1), NUMTX_GET_ROW_VEC(&matrix, 1));
                NuVecNorm(NUMTX_GET_ROW_VEC(&matrix, 2), NUMTX_GET_ROW_VEC(&matrix, 2));
            }
            NUVEC origin = {
                object->apiobj.collision_position.x,
                (object->apiobj.collision_max.y - object->apiobj.collision_min.y) * 0.75f +
                    object->apiobj.collision_min.y,
                object->apiobj.collision_position.z,
            };

            NuVecSub(&movement, NUMTX_GET_ROW_VEC(&matrix, 3), &origin);
            GameRayCast(&origin, &movement, 0.0f, TERRAINMASK_NONWEAPON | 0x1f);
        }
        params.matrix = &matrix;
        params.velocity = velocity;
        params.owner = object;
        NUVEC centre;
        NuSpecialGetRadius(&WORLD->lev_objs[0xe9].special, &centre, &params.field_14);
        params.field_18 = params.field_14;
        params.field_14 *= 0.75f;
        params.field_c4 = 1;
        params.gravity = -5.0f;
        params.special = &WORLD->lev_objs[0xe9].special;
        params.flags = 0x08000292;
        params.field_40 = PartCollide_3D;
        params.field_3c = PartImpact_ThermalDetonator;
        params.field_44 = PartKill_ThermalDetonator;
        params.stop_fn = PartStop_Flickerer;
        params.draw_fn = PartDraw_ThermalDetonator;
        params.field_a4 = 10.0f;
        params.time_step = FRAMETIME;

        PART_s *part = AddPart(&params);
        if (part != NULL) {
            part->force_flags = ObjHitObj_Flags(object) & 0xffff;
            part->force_player_mask = 0;
            part->field_1c4 = PartUpdate_ThermalDetonator;
            part->reflection_height = 2000000.0f;
            part->reflection_flags &= ~3;
            part->render_flags &= ~0x80;
        }
        PlaySfx(const_cast<char *>("ThrowDet"), &object->apiobj.collision_position);
        if (object->pad_gamepad != NULL) {
            NewBuzzFrames(object->pad_gamepad->pad, 2, 0);
        }
    }
}

i32 ThermalDetonator_MoveCode(GameObject_s *object) {
    if (object->character_context != 0x2e) {
        if (WORLD->lev_objs[0xe9].active != 0 && static_cast<i8>(object->apiobj.flags_low) < 0 &&
            (object->pad_gamepad->buttons_pressed & GAMEPAD_SPECIAL) != 0 &&
            ((object->apiobj.character_data->model_flags & 0x01000000) != 0 || object->field_0x108e == 6 ||
             SuperWeirdo(object) != 0) &&
            (object->apiobj.field_0x27d != 0 || object->field_0xe31 == 1)) {
            switch (object->character_context) {
                case -1:
                case 6:
                case 7:
                    break;
                default:
                    if ((CInfo[object->character_context].flags & 4) == 0) {
                        return 0;
                    }
                    break;
            }
            PART_s *part = FindPart(NULL, 0, object);
            if (part == NULL) {
                goto start_throw;
            }
            if ((part->active & 2) != 0 && !(part->field_100 > 1.0f)) {
                return 0;
            }
            KillPart(part, 0);
            return 1;
        }
        return 0;

    start_throw:
        object->character_context = 0x2e;
        if (object->field_0xe31 == 1) {
            object->context_animation = 0x6f;
        } else if ((object->field_0xe22 & 1) != 0) {
            object->context_animation = 0x6e;
        } else {
            object->context_animation = 0x65;
        }
        if (object->apiobj.character_model->model_data_b[object->context_animation] == NULL) {
            object->context_animation = 0x65;
        }
        if (AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 1) != NULL) {
            ResetAnimPacket(&object->apiobj.anim_packet, -1);
        }
        object->context_flags &= ~0x40;
        if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL) {
            object->context_animation_timer = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1);
        } else {
            object->context_animation_timer = 1.0f;
        }
        return 0;
    }

    f32 *frame = NULL;
    if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL) {
        frame = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
        if (frame == NULL) {
            return 0;
        }
    }
    object->context_animation_timer -= FRAMETIME;
    if (object->context_animation_timer <= 0.0f) {
        object->character_context = -1;
        if ((object->context_flags & 0x40) == 0) {
            object->movement_runtime_flags |= 0x40;
        }
    } else if ((object->context_flags & 0x40) == 0) {
        if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL) {
            const f32 release_frame = AnimListFrame(object->apiobj.character_model, object->context_animation, 2);
            if (frame != NULL && *frame > 0.0f && *frame >= release_frame) {
                object->movement_runtime_flags |= 0x40;
            }
        } else if (object->context_animation_timer < 0.5f) {
            object->movement_runtime_flags |= 0x40;
        }
    }
    if ((object->movement_runtime_flags & 0x40) != 0) {
        object->context_flags |= 0x40;
    }
    return 0;
}

i32 ShinyMetal_UpdateHint(HINT_s *hint) {
    if (player == NULL)
        return 0;
    if (hint->control_mode_ids[0] == 0x283 && JABBASPALACE_ADATA != NULL &&
        WORLD->current_level == JABBASPALACEB_LDATA && Game.area_save[JABBASPALACE_ADATA->index].area_complete == 0 &&
        Mission_Active(NULL) == NULL && (LevGizObst[0] == NULL || LevGizObst[0]->anim_set->state != 0) &&
        LevBlowUp[0] != NULL && (LevBlowUp[0]->status_flags & 1) == 0) {
        if (player != NULL &&
            NuVecDistSqr(&player->apiobj.collision_position, &LevBlowUp[0]->mid_position, NULL) < 25.0f)
            return 1;
        if (player2 != NULL &&
            NuVecDistSqr(&player2->apiobj.collision_position, &LevBlowUp[0]->mid_position, NULL) < 25.0f)
            return 1;
    }
    if (FreePlay == 0) {
        if (hint->control_mode_ids[0] != 0x2b8) {
            if (hint->control_mode_ids[0] != 0x283 || AvailableToPlayer(0x1000000, -1, 6, 1) == 0) {
                return 0;
            }
        } else {
            if ((player->apiobj.character_data->model_flags & 0x1000000) == 0 && player->field_0x108e != 6)
                return 0;
        }
    }
    GIZMOBLOWUP_s *blowup = WORLD->gizmo_blowups;
    if (blowup == NULL)
        return 0;
    i32 count = WORLD->gizmo_blowup_count;
    for (i32 i = 0; i < count; ++i, ++blowup) {
        if ((blowup->draw_flags & 2) == 0 || (blowup->status_flags & 0x804001) != 0x804000)
            continue;
        if (NuVecDistSqr(&player->apiobj.position, &blowup->mid_position, NULL) < 4.0f) {
            if ((player->apiobj.character_data->model_flags & 0x1000000) != 0 || player->field_0x108e == 6)
                return hint->control_mode_ids[0] == 0x2b8;
            if (AvailableToPlayer(0x1000000, -1, FreePlay == 0 ? 6 : 0, 1) != 0)
                return hint->control_mode_ids[0] == 0x283;
            return hint->control_mode_ids[0] == 0x61d;
        }
        count = WORLD->gizmo_blowup_count;
    }
    return 0;
}

