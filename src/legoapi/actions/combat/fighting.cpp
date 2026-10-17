#include "decomp.h"
#include "legoapi/actions/combat/fighting.h"
#include "legoapi/actions/combat/hits.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/players.h"
#include "nu2api/nu3d/nutex.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/characters/core/playeritems.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/actions/character/speederchase.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/misc/utilities.h"

#include <stdlib.h>

struct AIROW_s;
struct nunativegscene_s;
void BlockSfx(GameObject_s *object);
void KillParts(GameObject_s *, i32, i32, i32, f32, i32, u16 *);
i32 CannotKill(GameObject_s *);
i32 NewBlockAction(GameObject_s *);

void (*Punch_HitHoldFn)(GameObject_s *, GameObject_s *);
i32 (*Punch_GetDamageFn)(GameObject_s *, GameObject_s *);
void (*Punch_HitExtraCodeFn)(GameObject_s *, nuvec_s *);
BLADE_s BladeTab[4] = {
    {101, 223, 1, 59, 64, {255, 31, 0}, {0, 0, 0}},
    {103, 221, 2, 60, 65, {30, 191, 15}, {0, 0, 0}},
    {105, 225, 3, 61, 66, {0, 191, 255}, {0, 0, 0}},
    {107, 227, 4, 62, 67, {200, 0, 255}, {0, 0, 0}},
};

bool IsDownSwipe(NuVec2 const &start, NuVec2 const &end) {
    return abs(RotDiff(NuAtan2D(end.x - start.x, end.y - start.y), 0x8000)) < 0x1c72;
}

void ComboHitFrame(GameObject_s *object, i32 damage) {
    object->context_flags |= 0x40;
    object->sabre_flags |= 5;
    object->sabre_damage = damage;
    if ((object->apiobj.flags_low & 0x80) && Player_HasDoubleWeaponDamage(object)) {
        object->sabre_damage <<= 1;
    }
    BlockSfx(object);
}

BOLTTYPE_s *BoltType_FindByID(i32, WORLDINFO_s *);
DECOMP_ASSERT(offsetof(GameObject_s, field_0x7e4) == 0x7e4, "Quick shoot record offset");
DECOMP_ASSERT(offsetof(GameObject_s, field_0x7e8) == 0x7e8, "Quick shoot record count offset");
DECOMP_ASSERT(offsetof(GameObject_s, quick_shoot_flags) == 0xe2f, "Quick shoot flags offset");
DECOMP_ASSERT(offsetof(GameObject_s, quick_shoot_bolt_id) == 0xe3e, "Quick shoot bolt ID offset");

void ForceNextShootTarget(MechObjectInterface &target);

i32 ObjZappedBlue(GameObject_s *object);

void Punch_Hit(GameObject_s *attacker, GameObject_s *target, float, float gap) {
    NUVEC *hit_position;
    attacker->context_flags |= 0x40;

    if (LEGOCONTEXT_PUNCH != -1 && attacker->character_context == LEGOCONTEXT_PUNCH) {
        CHARACTERANIM_s *animation =
            static_cast<CHARACTERANIM_s *>(attacker->apiobj.character_model->model_data_a[attacker->context_animation]);
        if (animation != NULL) {
            attacker->attack_locator = animation->locator;
        }
    }

    if (target != NULL) {
        if ((target->apiobj.field_0x1f8 & 0x1001) != 0x1001 || target->apiobj.field_0x287 != 0 ||
            target->apiobj.collision_min.y > attacker->apiobj.collision_max.y ||
            attacker->apiobj.collision_min.y > target->apiobj.collision_max.y) {
            goto finish;
        }

        if (LEGOCONTEXT_HOLD != -1 && target->character_context == LEGOCONTEXT_HOLD) {
            NewBlockAction(target);
            hit_position = &target->apiobj.collision_position;
            if (Punch_HitHoldFn != NULL) {
                Punch_HitHoldFn(attacker, target);
            }
            goto hit;
        }

        if (target->apiobj.player_controlled) {
            if (LEGOCONTEXT_PUNCH != -1 && target->character_context == LEGOCONTEXT_PUNCH) {
                goto finish;
            }
            if (LEGOCONTEXT_JUMP != -1 && target->character_context == LEGOCONTEXT_JUMP) {
                if (attacker->action_movement_state == 3) {
                    goto finish;
                }
            }
        }

        if (ObjOpponentStillThere(attacker, target, gap) == 0) {
            goto finish;
        }

        i32 damage = Punch_GetDamageFn != NULL ? Punch_GetDamageFn(attacker, target) : 1;
        if (damage != -1 && Cheats_CheckFlags(0x800) != 0) {
            damage *= 2;
        }

        hit_position = NULL;
        u32 context_flags = CInfo[target->character_context].flags;
        if ((context_flags & 0x80) == 0) {
            if ((context_flags & 0x04000000) != 0 ||
                ((context_flags & 0x08000000) != 0 && (target->jump_flags & 2) != 0) || CannotKill(target) != 0) {
                NewRumble(attacker->pad_gamepad->pad, 0.5f, 0);
                NewRumble(target->pad_gamepad->pad, 0.5f, 0);
                if (static_cast<i32>(CInfo[target->character_context].flags) >= 0) {
                    target->apiobj.movement_facing_angle =
                        NuAtan2D(attacker->apiobj.collision_position.x - target->apiobj.collision_position.x,
                                 attacker->apiobj.collision_position.z - target->apiobj.collision_position.z);
                }
            } else {
                ObjHitObj(attacker, target, damage, static_cast<u16>(ObjHitObj_Flags(attacker) | 0x40), 0, 1);
            }
            hit_position = &target->apiobj.collision_position;
        }

        if ((target->apiobj.field_0x1f8 & 2) == 0) {
            f32 impulse = target->apiobj.character_data->game_character->movement_speed;
            if (attacker->apiobj.scaled_radius > 1.0f) {
                impulse /= attacker->apiobj.scaled_radius;
            }
            u16 angle = attacker->apiobj.movement_facing_angle;
            if (static_cast<i8>(attacker->context_flags) < 0) {
                angle -= 0x8000;
            }
            target->apiobj.velocity.z += NuTrigTable[((static_cast<u32>(angle) + 0x4000) >> 1) & 0x7fff] * impulse;
            target->apiobj.velocity.x += NuTrigTable[angle >> 1] * impulse;
        }
    } else {
        GIZMOBLOWUP_s *blowup = attacker->blowup_target;
        if (blowup == NULL || (blowup->status_flags & 0x804001) != 0x804000 ||
            blowup->bounds_min.y > attacker->apiobj.collision_max.y ||
            attacker->apiobj.collision_min.y > blowup->bounds_max.y) {
            goto finish;
        }
        hit_position = &blowup->mid_position;
        if (GizmoBlowupBlowup(blowup, 1, 1, 1, NULL, 1) != 0) {
            NewBuzz(attacker->pad_gamepad->pad, 0.1f, 0);
        }
        NewRumble(attacker->pad_gamepad->pad, 0.5f, 0);
    }

    if (hit_position == NULL) {
        goto finish;
    }

hit:
    attacker->movement_runtime_flags |= 1;
    if (attacker->apiobj.player_controlled) {
        GameCam_HitJudder();
    }
    goto extra_code;

finish:
    hit_position = NULL;
    attacker->movement_runtime_flags &= ~1;

extra_code:
    if (Punch_HitExtraCodeFn != NULL) {
        Punch_HitExtraCodeFn(attacker, hit_position);
    }
}
