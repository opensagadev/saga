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
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nutrig.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

NuMechPtr<MechObjectInterface, 4> NextThermalTarget;

EXPLOSION *AddExplosion(nuvec_s *, f32, f32, GameObject_s *, i32, i32);
void NewRumbleAllPlayers(f32, f32, i32, i32);
void GameCam_NewShake(GAMECAMERA_s *, f32, f32, f32);
void GameCam_Judder(GAMECAMERA_s *, f32, i32, nuvec_s *);
void AlertSurroundingCreatures(GameObject_s *, NUVEC *);
i32 qrand();
EXPLOSION *Detonate(nuvec_s *position, u16 flags);
i32 MatrixReflection(NUMTX *matrix, i32 axis, f32 plane, f32 height, NUMTX *result);
void KillPart(PART_s *part, i32 reason);
void NewBuzzFrames(nupad_s *pad, i32 frames, i32 channel);
i32 SuperWeirdo(GameObject_s *object);
void ThermalDetonator_ThrowMom(GameObject_s *object, nuvec_s *velocity);
void PartUpdate_ThermalDetonator(PART_s *part);
void PartImpact_ThermalDetonator(PART_s *part);
void PartKill_ThermalDetonator(PART_s *part, i32 reason);
i32 PartDraw_ThermalDetonator(PART_s *part);
i32 GameRayCast(NUVEC *position, NUVEC *movement, f32 radius, i32 mask);
extern i32 TERRAINMASK_NONWEAPON;
extern f32 brickimpactwait;

void ThermalDetonator_Throw(GameObject_s *object) {
    ADDPART_s params = Default_ADDPART;
    if (!(object == NULL || WORLD == NULL || WORLD->lev_objs == NULL || WORLD->lev_objs[0xea].active == 0)) {
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
            NUVEC movement;
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
            velocity.x = arc.x < 3.0f ? (arc.x < -3.0f ? -3.0f : arc.x) : 3.0f;
            velocity.z = arc.z;
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
        params.position = &object->apiobj.collision_position;
        params.velocity = &velocity;
        params.owner = object;
        NUVEC centre;
        NuSpecialGetRadius(&WORLD->lev_objs[0xea].special, &centre, &params.field_14);
        params.field_18 = params.field_14;
        params.field_14 *= 0.75f;
        params.field_c4 = 1;
        params.gravity = -5.0f;
        params.special = &WORLD->lev_objs[0xea].special;
        params.flags = 0x08000292;
        params.update_fn = PartCollide_3D;
        params.field_40 = PartImpact_ThermalDetonator;
        params.field_44 = PartKill_ThermalDetonator;
        params.stop_fn = PartStop_Flickerer;
        params.draw_fn = PartDraw_ThermalDetonator;
        params.time_step = FRAMETIME;
        params.field_a4 = 10.0f;

        PART_s *part = AddPart(&params);
        if (part != NULL) {
            part->force_flags = ObjHitObj_Flags(object) & 0xffff;
            part->force_player_mask = 0;
            part->update_callback = PartUpdate_ThermalDetonator;
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

i32 PartDraw_ThermalDetonator(PART_s *part) {
    const i32 draw = PartDraw_Flickerer(part);
    NUMTX matrix = part->transform;

    NUMTX reflection;
    i32 reflected = 0;
    if ((part->reflection_flags & 2) != 0 && part->reflection_height != 0.0f) {
        reflected =
            MatrixReflection(&matrix, 2, part->reflection_height, WORLD->current_level->unknown_0cc, &reflection);
    }

    LEVEL_OBJECT_RUNTIME *level_special = NULL;
    if (WORLD->lev_objs[0xea].active != 0 && WORLD->lev_objs[0xeb].active != 0) {
        const i32 index = draw == 1 ? 0xea : 0xeb;
        level_special = &WORLD->lev_objs[index];
        NuSpecialDrawAt(&level_special->special, &matrix);
    }

    if (reflected != 0) {
        NuRndrStartReflectionRender(1);
        if (part->source_special != NULL) {
            NuSpecialDrawAt(&part->special, &reflection);
        }
        if (level_special != NULL) {
            NuSpecialDrawAt(&level_special->special, &reflection);
        }
        NuRndrEndReflectionRender();
    }
    return 1;
}

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

i32 ThermalDetonator_MoveCode(GameObject_s *object) {
    if (object->character_context != 0x2e) {
        if (WORLD->lev_objs[0xe9].active == 0 || static_cast<i8>(object->apiobj.flags_low) >= 0 ||
            (object->pad_gamepad->buttons_pressed & GAMEPAD_SPECIAL) == 0) {
            return 0;
        }
        const bool can_throw = (object->apiobj.character_data->model_flags & 0x01000000) != 0 ||
                               object->field_0x108e == 6 || SuperWeirdo(object) != 0;
        if (!can_throw || (object->apiobj.field_0x27d == 0 && object->field_0xe31 != 1)) {
            return 0;
        }
        const i8 context = object->character_context;
        if (context != -1 && context != 6 && context != 7 && (CInfo[context].flags & 4) == 0) {
            return 0;
        }

        PART_s *part = FindPart(NULL, 0, object);
        if (part != NULL) {
            if ((part->active & 2) != 0 && !(part->field_100 > 1.0f)) {
                return 0;
            }
            KillPart(part, 0);
            return 1;
        }

        object->character_context = 0x2e;
        object->context_animation = object->field_0xe31 == 1 ? 0x6f : (object->field_0xe22 & 1) != 0 ? 0x6e : 0x65;
        if (object->apiobj.character_model->model_data_b[object->context_animation] == NULL) {
            object->context_animation = 0x65;
        }
        if (AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 1) != NULL) {
            ResetAnimPacket(&object->apiobj.anim_packet, -1);
        }
        object->context_flags &= ~0x40;
        object->context_animation_timer =
            object->apiobj.character_model->model_data_b[object->context_animation] != NULL
                ? AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1)
                : 1.0f;
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
    if (!(object->context_animation_timer > 0.0f)) {
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
        } else if (!(object->context_animation_timer >= 0.5f)) {
            object->movement_runtime_flags |= 0x40;
        }
    }
    if ((object->movement_runtime_flags & 0x40) != 0) {
        object->context_flags |= 0x40;
    }
    return 0;
}

void ThermalDetonator_ThrowMom(GameObject_s *object, nuvec_s *velocity) {
    ADDPART_s params = Default_ADDPART;
    if (!(object == NULL || WORLD == NULL || WORLD->lev_objs == NULL || WORLD->lev_objs[0xea].active == 0)) {
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
            NUVEC movement;
            NuVecSub(&movement, NUMTX_GET_ROW_VEC(&matrix, 3), &origin);
            GameRayCast(&origin, &movement, 0.0f, TERRAINMASK_NONWEAPON | 0x1f);
        }
        params.matrix = &matrix;
        params.velocity = velocity;
        params.owner = object;
        NUVEC centre;
        NuSpecialGetRadius(&WORLD->lev_objs[0xea].special, &centre, &params.field_14);
        params.field_18 = params.field_14;
        params.field_14 *= 0.75f;
        params.field_c4 = 1;
        params.gravity = -5.0f;
        params.special = &WORLD->lev_objs[0xea].special;
        params.flags = 0x08000292;
        params.update_fn = PartCollide_3D;
        params.field_40 = PartImpact_ThermalDetonator;
        params.field_44 = PartKill_ThermalDetonator;
        params.stop_fn = PartStop_Flickerer;
        params.draw_fn = PartDraw_ThermalDetonator;
        params.time_step = FRAMETIME;
        params.field_a4 = 10.0f;

        PART_s *part = AddPart(&params);
        if (part != NULL) {
            part->force_flags = ObjHitObj_Flags(object) & 0xffff;
            part->force_player_mask = 0;
            part->update_callback = PartUpdate_ThermalDetonator;
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

void PartUpdate_ThermalDetonator(PART_s *part) {
    if (part == NULL) {
        return;
    }
    if ((part->active & 2) != 0 && (part->render_flags & 0x40) == 0) {
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

EXPLOSION *Detonate(nuvec_s *position, u16 flags) {
    AddGameDebris(WORLD->debris_sys, 0x49, position);
    AddGameDebris(WORLD->debris_sys, 0x4a, position);
    AddGameDebris(WORLD->debris_sys, 0x4b, position);
    AddPartDebris(WORLD->part_debris_sys, 2, position);
    NewRumbleAllPlayers(1.0f, 0.1f, 0, 0);
    PlaySfx((char *)"exp_thermalDet", position);
    f32 amount_a;
    f32 amount_b;
    if ((flags & 0x1000) != 0) {
        f32 amount = qrand() < 0x8000 ? -2.0f : 2.0f;
        GameCam_Judder(GameCam, amount, 2, position);
        GameCam_NewShake(GameCam, 2.0f, 1.0f, 1.0f);
        PlaySfx((char *)"exp_thermalDet", position);
        amount_a = 1.3125f;
        amount_b = 0.875f;
    } else {
        GameCam_NewShake(GameCam, 1.0f, 1.0f, 1.0f);
        amount_a = 0.75f;
        amount_b = 0.5f;
    }
    return AddExplosion(position, amount_a, amount_b, NULL, -1, (flags & 0xffff) | 0x67);
}
