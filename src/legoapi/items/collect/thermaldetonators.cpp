#include "decomp.h"
#include "globals.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/legoapi_types.h"
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

void ThermalDetonator_Throw(GameObject_s *object) {
    if (object == NULL || WORLD == NULL || WORLD->lev_objs == NULL || WORLD->lev_objs[0xea].active == 0) {
        return;
    }

    NUMTX matrix;
    NuMtxSetTranslation(&matrix, &object->apiobj.collision_position);
    NUVEC velocity;
    ThermalDetonator_ThrowMom(object, &velocity);

    ADDPART_s params = Default_ADDPART;
    params.matrix = &matrix;
    params.velocity = &velocity;
    params.owner = object;
    params.field_14 = 0.1f;
    params.field_18 = 0.1f;
    params.gravity = -5.0f;
    params.special = &WORLD->lev_objs[0xea].special;
    params.flags = 0x08000292;
    params.update_fn = PartUpdate_ThermalDetonator;
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
    object->movement_runtime_flags &= ~0x40;
    PlaySfx(const_cast<char *>("ThrowDet"), &object->apiobj.collision_position);
    if (object->pad_gamepad != NULL) {
        NewBuzzFrames(object->pad_gamepad->pad, 2, 0);
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
    if (part->owner != NULL && static_cast<i8>(part->owner->apiobj.flags_low) < 0 && Cheat_IsOn(0x17) != 0) {
        flags = 0x1200;
    }

    EXPLOSION *explosion = Detonate(&part->position, flags);
    if (explosion != NULL && part->owner != NULL && static_cast<u8>(part->owner->apiobj.field_0x27c) <= 1) {
        explosion->field_0x33 = static_cast<u8>(part->owner->apiobj.field_0x27c);
    }
}

i32 ThermalDetonator_MoveCode(GameObject_s *object) {
    if (object == NULL || object->apiobj.character_model == NULL || object->pad_gamepad == NULL || WORLD == NULL ||
        WORLD->lev_objs == NULL || WORLD->lev_objs[0xea].active == 0) {
        return 0;
    }

    if (object->character_context != 0x2e) {
        if (static_cast<i8>(object->apiobj.flags_low) >= 0 ||
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

        for (i32 index = 0; index < MAXPARTS; ++index) {
            PART_s *part = &Part[index];
            if ((part->active & 1) != 0 && part->owner == object &&
                part->draw_callback == PartDraw_ThermalDetonator) {
                if ((part->active & 2) != 0 && part->elapsed <= 1.0f) {
                    return 0;
                }
                KillPart(part, 0);
                return 1;
            }
        }

        object->character_context = 0x2e;
        object->context_animation = object->field_0xe31 == 1 ? 0x6f : (object->field_0xe22 & 1) != 0 ? 0x6e : 0x65;
        if (object->apiobj.character_model->model_data_b[object->context_animation] == NULL) {
            object->context_animation = 0x65;
        }
        object->context_flags &= ~0x40;
        object->movement_runtime_flags &= ~0x40;
        ResetAnimPacket(&object->apiobj.anim_packet, -1);
        object->context_animation_timer =
            object->apiobj.character_model->model_data_b[object->context_animation] != NULL
                ? AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1)
                : 1.0f;
        return 0;
    }

    f32 *frame = object->apiobj.character_model->model_data_b[object->context_animation] != NULL
                     ? AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0)
                     : NULL;
    object->context_animation_timer -= FRAMETIME;
    const f32 release_frame =
        object->apiobj.character_model->model_data_b[object->context_animation] != NULL
            ? AnimListFrame(object->apiobj.character_model, object->context_animation, 2)
            : 0.0f;
    if ((object->context_flags & 0x40) == 0 &&
        ((frame != NULL && *frame > 0.0f && *frame >= release_frame) ||
         (frame == NULL && object->context_animation_timer <= 0.5f))) {
        object->context_flags |= 0x40;
        object->movement_runtime_flags |= 0x40;
    }
    if (object->context_animation_timer <= 0.0f) {
        object->character_context = -1;
    }
    return 0;
}

void ThermalDetonator_ThrowMom(GameObject_s *object, nuvec_s *velocity) {
    if (object == NULL || velocity == NULL) {
        return;
    }
    const u16 angle = object->apiobj.movement_facing_angle;
    velocity->x = object->apiobj.velocity.x + NU_SIN_LUT(angle) * 2.0f;
    velocity->y = object->apiobj.velocity.y + 2.0f;
    velocity->z = object->apiobj.velocity.z + NU_COS_LUT(angle) * 2.0f;
}

void PartImpact_ThermalDetonator(PART_s *part) {
    if (part == NULL) {
        return;
    }
    if ((part->render_flags & 0x80) != 0 || part->field_209 == 0x1c) {
        KillPart(part, 0);
        return;
    }
    PartImpact_Brick(part);
    PlaySfx(const_cast<char *>("ThermalDet_Bnce"), &part->position);
    if ((part->active & 3) == 1) {
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
    if ((part->active & 2) != 0 && part->elapsed > 0.0f && part->elapsed < 1.0f &&
        (part->render_flags & 0x40) == 0) {
        PlaySfx(const_cast<char *>("ThermalDet_Beep"), &part->position);
        part->render_flags |= 0x40;
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
