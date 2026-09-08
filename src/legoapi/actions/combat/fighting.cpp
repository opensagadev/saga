#include "decomp.h"
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

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;
void BlockSfx(GameObject_s *object);
BLADE_s BladeTab[4] = {
    {101, 223, 1, 59, 64, {255, 31, 0}, {0, 0, 0}},
    {103, 221, 2, 60, 65, {30, 191, 15}, {0, 0, 0}},
    {105, 225, 3, 61, 66, {0, 191, 255}, {0, 0, 0}},
    {107, 227, 4, 62, 67, {200, 0, 255}, {0, 0, 0}},
};

void DeflectPart(PART_s *, GameObject_s *, float, float, i32, i32) {
}

void IsDownSwipe(NuVec2 const &, NuVec2 const &) {
}

void TakeHitCode(GameObject_s *) {
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
}

void StunGameObject(GameObject_s *, GameObject_s *, float, i32) {
}

void ComboRotateCode(GameObject_s *, i32) {
}

BOLTTYPE_s *BoltType_FindByID(i32, WORLDINFO_s *);
extern "C" f32 AnimDuration(i32, i32, f32, f32, i32);
void NewBuzzFrames(nupad_s *, i32, i32);
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

void ForceNextLungeTarget(MechObjectInterface *) {
}

void ForceNextShootTarget(MechObjectInterface &target) {
    nextShootTarget = NuMechPtr<MechObjectInterface, 4>(&target);
}

void SetForcedAttackOpponent(MechObjectInterface *) {
}

void Punch_Hit(GameObject_s *, GameObject_s *, float, float) {
}
