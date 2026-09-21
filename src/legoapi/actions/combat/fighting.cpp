#include "decomp.h"
#include "legoapi/actions/combat/fighting.h"
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
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/characters/core/playeritems.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/items/objects/gameobjects.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;
void BlockSfx(GameObject_s *object);
void StartHold(GameObject_s *object);
void KillParts(GameObject_s *, i32, i32, i32, f32, i32, u16 *);
void Arcade_Kill(i32, i32);
BLADE_s BladeTab[4] = {
    {101, 223, 1, 59, 64, {255, 31, 0}, {0, 0, 0}},
    {103, 221, 2, 60, 65, {30, 191, 15}, {0, 0, 0}},
    {105, 225, 3, 61, 66, {0, 191, 255}, {0, 0, 0}},
    {107, 227, 4, 62, 67, {200, 0, 255}, {0, 0, 0}},
};

void DeflectPart(PART_s *, GameObject_s *, float, float, i32, i32) {
    STUBBED();
}

void IsDownSwipe(NuVec2 const &, NuVec2 const &) {
    STUBBED();
}

void TakeHitCode(GameObject_s *object) {
    f32 timer;
    f32 frame_time;
    f32 impact_speed;
    f32 impact_threshold;
    i16 animation;
    i8 context;
    i32 animation_active;
    i32 kill_parts_mode;
    i32 hearts;
    i32 first_flag;
    i32 second_flag;
    u32 coins;
    bool has_coins;

    animation = object->context_animation;
    animation_active = 1;
    if (animation == -1) {
        animation_active = 0;
    } else if (object->apiobj.character_model->model_data_b[animation] != NULL &&
               AnimPlaying(&object->apiobj.anim_packet, animation, 1, 0) == 0) {
        animation_active = 0;
    }

    context = object->character_context;
    if (context == 0x5a)
        goto context_5a;
    if (context != 0x5f) {
        if (context != 0x15 || animation_active == 0)
            return;
        timer = object->context_animation_timer;
        frame_time = FRAMETIME;
        object->context_animation_timer = timer - frame_time;
        if (0.0f < timer - frame_time)
            return;
        goto finish_context;
    }

    if (animation_active == 0)
        return;
    timer = object->context_animation_timer + FRAMETIME;
    object->context_animation_timer = timer;
    if (timer >= object->airborne_action_duration)
        goto finish_airborne;
    if (timer > 0.5f)
        goto check_ground_impact;
    return;

check_ground_impact:
    if (0.0f > object->reset_velocity.y && object->apiobj.field_0x27d != 0)
        goto finish_airborne;
    if (object->field_0x1084 == 0)
        return;
    impact_speed = object->terrain_impact_speed;
    impact_threshold = timer / object->airborne_action_duration * -0.8f + 0.8f;
    if (impact_speed > impact_threshold)
        goto finish_airborne;
    return;

finish_airborne:
    kill_parts_mode = 1;
    if (Arcade != 0 && static_cast<u8>(object->hit_variant) < 2)
        Arcade_Kill(object->hit_variant, object->apiobj.field_0x27c);
    goto kill_object;

context_5a:
    if (animation_active == 0)
        return;
    timer = object->context_animation_timer;
    frame_time = FRAMETIME;
    object->context_animation_timer = timer - frame_time;
    if (0.0f < timer - frame_time)
        return;
    if (object->field_0x7a3 != 0) {
        if (object->field_0x7a3 != 1)
            return;
        goto finish_context;
    }

    animation = object->context_animation;
    object->movement_runtime_flags |= 0x80;
    if (animation == 0xb9) {
        kill_parts_mode = 0;
        goto kill_object;
    }
    if (animation == 0x3d)
        goto finish_context;
    if (animation == 0xa7)
        object->context_animation = 0xaa;
    else if (animation == 0xa8)
        object->context_animation = 0xab;
    else if (animation == 0xa9)
        object->context_animation = 0xac;
    else
        object->context_animation = 1;

    object->field_0x7a3 = 1;
    animation = object->context_animation;
    if (object->apiobj.character_model->model_data_b[animation] != NULL &&
        (static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[animation])->flags & 2) == 0) {
        object->context_animation_timer = AnimDuration(object->id, animation, 0.0f, 0.0f, 1);
        return;
    }
    object->context_animation_timer = object->field_0x768;
    return;

finish_context:
    object->character_context = -1;
    return;

kill_object:
    KillParts(object, -1, -1, 1, 0.0f, kill_parts_mode, NULL);
    KillGameObject(object, 2, 0);
    if (object->apiobj.field_0x27c != -1)
        return;

    if (BonusArea != 0) {
        coins = static_cast<u16>(object->apiobj.character_data->game_character->field_0xee);
        has_coins = static_cast<i32>(coins) > 0;
    release_hearts:
        hearts = ReleaseHearts();
        if (hearts == 0 && !has_coins)
            return;
        if (BonusArea == 0) {
            if (coins > 2499) {
                first_flag = 0;
                second_flag = 0;
                goto add_pickups;
            }
        no_bonus_under_limit:
            first_flag = 1;
            second_flag = 0;
            goto add_pickups;
        }
    } else {
        if (Cheat_IsOn(0x10) == 0) {
            has_coins = false;
            coins = 0;
            goto release_hearts;
        } else {
            hearts = ReleaseHearts();
            coins = 350;
            if (BonusArea == 0)
                goto no_bonus_under_limit;
        }
    }

    first_flag = 0;
    second_flag = 1;
add_pickups:
    AddPickups(coins, hearts, 0, 0, &object->apiobj.collision_position, NULL, 2.0f, -1, 1.0f, 2000000.0f, NULL,
               first_flag, second_flag, false);
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

void IsFacingTarget(nuvec_s *, nuvec_s *, i32, i32) {
    STUBBED();
}

void StunGameObject(GameObject_s *, GameObject_s *, float, i32) {
    STUBBED();
}

void ComboRotateCode(GameObject_s *, i32) {
    STUBBED();
}

BOLTTYPE_s *BoltType_FindByID(i32, WORLDINFO_s *);
void SetWeaponIn(GameObject_s *);
void SetWeaponOut(GameObject_s *);
DECOMP_ASSERT(offsetof(GameObject_s, field_0x7e4) == 0x7e4, "Quick shoot record offset");
DECOMP_ASSERT(offsetof(GameObject_s, field_0x7e8) == 0x7e8, "Quick shoot record count offset");
DECOMP_ASSERT(offsetof(GameObject_s, quick_shoot_flags) == 0xe2f, "Quick shoot flags offset");
DECOMP_ASSERT(offsetof(GameObject_s, quick_shoot_bolt_id) == 0xe3e, "Quick shoot bolt ID offset");
void StartQuickShoot(GameObject_s *object, i32 action) {
    object->field_0xe21 &= ~8;
    i32 bolt_id = BoltType_FindIDByCreature(object, 0);
    NUVEC direction;
    BoltSys->shoot_direction(object, &direction);
    f32 speed = BoltType_FindByID(bolt_id, WORLD)->field_10;
    f32 range = speed * BoltType_FindByID(bolt_id, WORLD)->field_14;
    f32 range_squared = range * range;
    GameObject_s *target = TargetGameObject(object, &object->apiobj.collision_position, &direction, range,
                                            range_squared, 0, 1, 0, bolt_id);
    if (target != NULL)
        SetObjTarget(object, target);
    else if ((object->apiobj.flags_low & 0x80) != 0 &&
             GizmoSys_SetBestBoltTarget(WORLD->gizmo_sys, WORLD, object, &object->apiobj.collision_position, &direction,
                                        range, range_squared, 1, 0, bolt_id) == 0) {
        GIZMOBLOWUP_s *blowup = GizmoBlowUp_Target(object, &object->apiobj.collision_position, &direction, range,
                                                   range_squared, 1, 0, bolt_id);
        if (blowup != NULL)
            SetGizmoBlowUpTarget(object, blowup);
        else {
            PART_s *part =
                TargetPart(object, &object->apiobj.collision_position, &direction, range, range_squared, 1, bolt_id);
            if (part != NULL)
                SetPartTarget(object, part);
            else {
                target = TargetGameObject(object, &object->apiobj.collision_position, &direction, range, range_squared,
                                          0x200, 1, 0, bolt_id);
                if (target != NULL)
                    SetObjTarget(object, target);
            }
        }
    }
    object->character_context = 10;
    object->context_animation = action;
    object->context_animation_timer = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, action != 0x57);
    ResetAnimPacket(&object->apiobj.anim_packet, -1);
    object->apiobj.anim_packet.flags |= 0x10;
    i32 fire_now = !(AnimListFrame(object->apiobj.character_model, object->context_animation, 1) > 1.0f);
    bolt_id = BoltType_FindIDByCreature(object, 0);
    i32 flags = 1 + 2 * fire_now;
    if (object == Player[0] && nextShootTarget.Get() != NULL)
        nextShootTarget = NuMechPtr<MechObjectInterface, 4>();
    if ((object->apiobj.field_0x1f4 & 0x40000) == 0 || object->apiobj.field_0x27c == -1) {
        NewBuzzFrames(object->pad_gamepad->pad, (object->apiobj.character_data->model_flags & 0x2000) != 0 ? 1 : 2, 0);
        object->quick_shoot_bolt_id = bolt_id;
        object->quick_shoot_flags = flags;
        object->field_0xef9 |= 8;
        if ((object->apiobj.character_data->game_character->flags_098[0] & 2) != 0)
            SetWeaponIn(object);
        if (object->field_0x7e4 != NULL && object->field_0x7e4[8] == 2 && object->field_0x7e8 != 0)
            --object->field_0x7e8;
    }
    object->quick_shoot_timer = 0.0f;
    ++object->action_suppressed;
    SetWeaponOut(object);
}

void ForceNextLungeTarget(MechObjectInterface *target) {
    lungeTarget = NuMechPtr<MechObjectInterface, 4>(target);
}

void ForceNextShootTarget(MechObjectInterface &target) {
    nextShootTarget = NuMechPtr<MechObjectInterface, 4>(&target);
}

i32 ObjZappedBlue(GameObject_s *object) {
    if (object->field_0x7a5 == 0x42)
        return 1;
    if (object->field_0x7a5 == 0x1c) {
        GameObject_s *holder = static_cast<GameObject_s *>(object->field_0x780);
        if (holder != NULL && (holder->field_0xe21 & 1) != 0)
            return 1;
    }
    return 0;
}

void SetForcedAttackOpponent(MechObjectInterface *target) {
    forceNextAttackOpponent = NuMechPtr<MechObjectInterface, 4>(target);
}

void Punch_Hit(GameObject_s *, GameObject_s *, float, float) {
    STUBBED();
}
