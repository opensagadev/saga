#include "decomp.h"
#include "MechInputTouch_types.h"
#include "globals.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/animation_ids.h"
#include "legoapi/ai/core/legoai.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gamelib/util/gamelib_util_types.h"
#include "nu2api/nucore/numemory.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/gizmos/transport/teleport.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmos/object/hatmachine.h"
#include "legoapi/gizmos/object/lever.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/core/input/qrand.h"

#include <new>
#include <string.h>

i32 GetMenuID();
CABLE_s *GameObjOwnsAnyCables(GameObject_s *);
void ReleaseCable(CABLE_s *, i32);
extern "C" i16 id_WATTO;
extern i16 id_YODA;
void ForceNextLungeTarget(MechObjectInterface *);
bool FireBountyHunterRocket(GameObject_s *);
void SlowWeaponOut(GameObject_s *);
extern i32 id_HINT_LSW_AUTOJUMP;
extern i32 id_HINT_LSW_AUTOJUMP_FAIL;
MechAutoJumpConnection *MechAutoJumpGetBest(JumpTriggerPacket const &, i32);
void MechAutoJumpSetIsUsing(GameObject_s &, MechAutoJumpConnection &);
bool isBucking;
i32 DoBuckStart(GameObject_s *);
void StartJetPackFall(GameObject_s *, i32);
void SetForcedAttackOpponent(MechObjectInterface *);
bool IsDownSwipe(NuVec2 const &, NuVec2 const &);

void MechInputTouchGestureBasedController::Activate() {
    if (active) {
        return;
    }
    if (field_90 != NULL && !field_90->is_down) {
        field_90 = NULL;
    }
    if (field_94 != NULL && !field_94->is_down) {
        field_94 = NULL;
    }
    active = 1;
    MechSystems::Get()->gesture_tracking_system.RegisterGestureTracker(*this, 150);
    field_90 = NULL;
    MechSystems::Get()->gesture_controller = this;
}

void MechInputTouchGestureBasedController::Deactivate() {
    if (!active) {
        return;
    }
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
        Obj[i].KillTasks();
    }
    active = 0;
    MechSystems::Get()->gesture_tracking_system.UnregisterGestureTracker(*this);
    if (player != NULL) {
        player->jump_input_flags &= ~0x20;
    }
    buttons_were_pressed = 0;
    buttons_pressed = 0;
    buttons_repeat = 0;
}

void MechInputTouchGestureBasedController::KillTasks(bool kill_active_task) {
    GameObject_s *object = Player[player_id];
    if (object == NULL) {
        return;
    }
    if (object->touch_task == NULL) {
        return;
    }
    if ((object->touch_task->flags & 2) != 0 && !kill_active_task) {
        return;
    }
    object->KillTasks();
}

MechInputTouchGestureBasedController::MechInputTouchGestureBasedController(
    i32 index, MechInputTouchGestureBasedController::StickMode mode)
    : MechInputTouchMainController(index), temporary_position(), stick_mode(mode), field_a5(0), tag_button(NULL) {
    active = 0;
    field_a6 = 0;
    field_98 = 0.0f;
    smart_bomb_touch = NULL;
    field_90 = NULL;
    field_94 = NULL;
    field_9c = 0.0f;
    field_a0 = 0.0f;
}

bool MechInputTouchGestureBasedController::MenuDisable() {
    const i32 menu_id = GetMenuID();
    if (menu_id == 12 || menu_id == 16 || menu_id == 13 || menu_id == 17 || menu_id == 8 || menu_id == 18) {
        return true;
    }
    return menu_id == 14;
}

bool MechInputTouchGestureBasedController::OnClick(GameObject_s &object, TouchHolder &holder) {
    if (object.apiobj.character_data == NULL || object.apiobj.character_data->player_config == NULL ||
        object.id == id_GRABCONTROL || object.character_context == 0x2b) {
        return false;
    }
    holder.clicked = 1;
    VuVec position(holder.down_position.x, holder.down_position.y, 0.0f, 1.0f);
    MechObjectInterface *target = holder.target_object.Get();
    if (target != NULL && (target->GetObjectType() == 5 || target->GetObjectType() == 6)) {
        i32 flags = 0x1f4f;
        if (object.field_0xcc0 != NULL && object.field_0xcc0->id != id_YODA) {
            if (TouchHacks::CanShoot(object)) {
                flags = 0x5c05;
            } else if (TouchHacks::CanPoo(object) || object.apiobj.character_data->move_fn == Move_BEAST) {
                flags = 1;
            } else {
                flags = 0;
            }
        }
        target = MechInputTouchSystem::FindTargetObject(object, position, flags, NULL, NULL);
    }
    holder.previous_target_object = target;
    if (target != NULL && target->GetCharacterObject() != NULL && target->GetCharacterObject()->id == id_BIGGUN) {
        return false;
    }

    bool dispatch = object.id == id_TRAININGREMOTE || object.character_context == LEGOCONTEXT_JUMP ||
                    object.character_context == LEGOCONTEXT_BIGJUMP || object.apiobj.field_0x27d != 0 ||
                    VehicleArea != 0;
    if (!dispatch) {
        dispatch = (object.apiobj.character_data->model_flags & 0x2000) != 0 || object.field_0xe31 == 1;
    }
    if (!dispatch) {
        if ((object.apiobj.character_data->model_flags & 0x8000) != 0 && object.id != id_WATTO &&
            object.field_0xe31 != 1) {
            object.field_0xf04 |= 0x10;
            return true;
        }
        if (target != NULL && (target->GetObjectType() == 2 || target->GetObjectType() == 4) &&
            TouchHacks::CanShoot(object)) {
            ForceNextShootTarget(*target);
            button_was_pressed[0] = 1;
            return true;
        }
        ForceNextLungeTarget(target);
        object.field_0xf04 |= 0x20;
        return true;
    }

    if (target == NULL) {
        target = MechInputTouchSystem::FindTargetObject(object, position, 0x80, NULL, &temporary_position);
    }
    holder.target_object = target;
    holder.previous_target_object = target;
    if (target == NULL) {
        return false;
    }
    target->TargetedFlash();
    switch (target->GetObjectType()) {
        case 1:
        case 5:
            StartNewTask(new MechTouchTaskPlannedGoTo(*this, target, NULL), holder, false, true);
            return true;
        case 2: {
            GameObject_s *character = target->GetCharacterObject();
            if (character == Player[0]) {
                if (object.touch_task != NULL &&
                    object.touch_task->GetHashId().value == MechTouchTaskAttack::HashId.value) {
                    return false;
                }
                if (FireBountyHunterRocket(Player[0])) {
                    return false;
                }
                if (TouchHacks::CanPoo(object)) {
                    button_was_pressed[3] = 1;
                    return false;
                }
                if (PerformCloseMechanic(object, holder)) {
                    return false;
                }
                if (object.character_context != LEGOCONTEXT_WEAPONOUT && object.weapon_scale <= 0.0f) {
                    SlowWeaponOut(&object);
                }
                return true;
            }
            if (character == NULL) {
                return false;
            }
            if (static_cast<i32>(character->apiobj.field_0x1f4) >= 0) {
                StartNewTask(new MechTouchTaskAttack(*this, target, position), holder, false, true);
                return true;
            }
            VuVec target_position;
            target->GetPos(target_position, -1);
            NUVEC direction = {Player[0]->apiobj.position.x - target_position.x, 0.0f,
                               Player[0]->apiobj.position.z - target_position.z};
            NuVecNorm(&direction, &direction);
            f32 scale = target->GetRadius() * 2.5f;
            temporary_position.position = VuVec(target_position.x + direction.x * scale, target_position.y,
                                                target_position.z + direction.z * scale, 0.0f);
            MechTouchTaskPlannedGoTo *task = new MechTouchTaskPlannedGoTo(*this, &temporary_position, NULL);
            task->field_6fd = 1;
            task->field_6fe = 1;
            StartNewTask(task, holder, false, true);
            return true;
        }
        case 3:
        case 4:
        case 11: {
            GIZOBSTACLE_s *obstacle = target->GetGizObstacle();
            if (obstacle != NULL && reinterpret_cast<u8 *>(obstacle)[0x91] == 2) {
                StartNewTask(new MechTouchTaskPlannedGoTo(*this, target, NULL), holder, false, true);
            } else {
                StartNewTask(new MechTouchTaskAttack(*this, target, position), holder, false, true);
            }
            return true;
        }
        case 7:
            StartNewTask(new MechTouchTaskPullLever(*this, target, position), holder, false, true);
            return true;
        case 8:
            StartNewTask(new MechTouchTaskUseTeleport(*this, target, position), holder, false, true);
            return true;
        case 9:
            StartNewTask(new MechTouchTaskHatMachine(*this, target, position), holder, false, true);
            return true;
        case 10:
            StartNewTask(new MechTouchTaskPanel(*this, target, position), holder, false, true);
            return true;
        default:
            return false;
    }
}

bool MechInputTouchGestureBasedController::OnDoubleClick(GameObject_s &object, TouchHolder &holder) {
    if (VehicleArea != 0) {
        return OnClick(object, holder);
    }
    if (object.character_context == LEGOCONTEXT_JUMP) {
        return true;
    }
    if (object.character_context == LEGOCONTEXT_BIGJUMP || object.apiobj.field_0x27d == 0) {
        return true;
    }
    if (object.apiobj.character_data == NULL || object.apiobj.character_data->player_config == NULL ||
        object.id == id_GRABCONTROL || object.character_context == 0x2b) {
        return false;
    }
    VuVec touch_position(holder.down_position.x, holder.down_position.y, 0.0f, 1.0f);
    MechObjectInterface *target = holder.target_object.Get();
    if (target == NULL) {
        target = holder.previous_target_object.Get();
    }
    if (target == NULL) {
        target = MechInputTouchSystem::FindTargetObject(object, touch_position, 0x80, NULL, &temporary_position);
    }
    if (target == NULL) {
        return false;
    }
    VuVec target_position;
    target->GetPos(target_position, -1);
    const f32 dx = target_position.x - object.apiobj.position.x;
    const f32 dy = target_position.y - object.apiobj.position.y;
    const f32 dz = target_position.z - object.apiobj.position.z;
    const f32 distance_sq = dx * dx + dy * dy + dz * dz;
    const i32 type = target->GetObjectType();
    if (type == 2) {
        GameObject_s *character = target->GetCharacterObject();
        if (character == &object) {
            GameObject_s *nearby = FindNearestGameObject(&object.apiobj.position, &object, 0, 1.5f, 1.5f, -1, -1, 100,
                                                         NULL, 0, NULL, false);
            if (nearby != NULL) {
                StartNewTask(new MechTouchTaskAttack(*this, nearby->GetMechObjectInterface(), touch_position), holder,
                             false, true);
            } else {
                JumpTriggerPacket packet = {};
                packet.type = 2;
                packet.player = &object;
                packet.touch_holder = &holder;
                packet.velocity =
                    VuVec(object.apiobj.velocity.x, object.apiobj.velocity.y, object.apiobj.velocity.z, 1.0f);
                object.field_0xf04 |= 0x10;
                TriggerJumpTask(packet, false, false, false);
            }
            return true;
        }
        if (character != NULL && distance_sq < 1.96f && static_cast<i32>(character->apiobj.field_0x1f4) >= 0 &&
            character->apiobj.character_data != NULL &&
            ((character->apiobj.character_data->model_flags & 0x10) != 0 ||
             ((character->apiobj.character_data->model_flags & 8) != 0 && qrand() <= 0x3a97)) &&
            (object.apiobj.character_data->model_flags & 8) != 0) {
            object.field_0xf04 |= 0x10;
            VuVec velocity = TouchHacks::CalculateJumpVelToHitPoint(object, target_position);
            object.apiobj.velocity.x = object.target_velocity.x = velocity.x;
            object.apiobj.velocity.y = object.target_velocity.y = velocity.y;
            object.apiobj.velocity.z = object.target_velocity.z = velocity.z;
            JumpTriggerPacket packet = {};
            packet.type = 2;
            packet.player = &object;
            packet.touch_holder = &holder;
            packet.velocity = velocity;
            packet.start = holder.down_position;
            packet.end = holder.touch_position;
            ForceNextLungeTarget(target);
            SetForcedAttackOpponent(target);
            TriggerJumpTask(packet, true, true, true);
            return true;
        }
    }
    if (type == 4 && distance_sq < 1.96f && (object.apiobj.character_data->model_flags & 8) != 0) {
        object.field_0xf04 |= 0x20;
        VuVec velocity = TouchHacks::CalculateJumpVelToHitPoint(object, target_position);
        object.apiobj.velocity.x = object.target_velocity.x = velocity.x;
        object.apiobj.velocity.y = object.target_velocity.y = velocity.y;
        object.apiobj.velocity.z = object.target_velocity.z = velocity.z;
        JumpTriggerPacket packet = {};
        packet.type = 2;
        packet.player = &object;
        packet.touch_holder = &holder;
        packet.velocity = velocity;
        packet.start = holder.down_position;
        packet.end = holder.touch_position;
        ForceNextLungeTarget(target);
        SetForcedAttackOpponent(target);
        TriggerJumpTask(packet, true, true, true);
        return true;
    }
    GameObject_s *nearby =
        FindNearestGameObject(&object.apiobj.position, &object, 0, 0.5f, 0.5f, -1, -1, 100, NULL, 0, NULL, false);
    if (nearby != NULL) {
        StartNewTask(new MechTouchTaskAttack(*this, nearby->GetMechObjectInterface(), touch_position), holder, false,
                     true);
    } else {
        StartNewTask(new MechTouchTaskPlannedDoubleClickGoTo(*this, target), holder, false, true);
    }
    return true;
}

bool MechInputTouchGestureBasedController::OnDown(GameObject_s &object, TouchHolder &holder) {
    if (object.apiobj.character_data == NULL || object.apiobj.character_data->player_config == NULL) {
        return false;
    }
    VuVec position(holder.down_position.x, holder.down_position.y, 0.0f, 1.0f);
    MechObjectInterface *force_target = MechInputTouchSystem::FindTargetObject(object, position, 0x2010, NULL, NULL);
    if (force_target != NULL) {
        StartNewTask(new MechTouchTaskUseForce(*this, force_target, position), holder, true, true);
        if (object.touch_task != NULL && object.touch_task->GetHashId().value == MechTouchTaskUseForce::HashId.value) {
            object.touch_task->Update();
        }
        return true;
    }
    if ((object.incoming_bolt != NULL || (object.field_0xe21 & 0x20) != 0) && holder.target_object.Get() != NULL &&
        holder.target_object->GetCharacterObject() == &object) {
        button_was_pressed[0] = 1;
        return true;
    }
    if (field_90 == NULL) {
        field_a0 = 0.0f;
        field_90 = &holder;
        field_a5 = 0;
    } else if (field_94 == NULL) {
        const f32 dx = holder.touch_position.x - field_90->touch_position.x;
        const f32 dy = holder.touch_position.y - field_90->touch_position.y;
        if (dx * dx + dy * dy < 1.0f) {
            field_94 = &holder;
        }
    }
    if (smart_bomb_touch == NULL && player != NULL && holder.target_object.Get() == player->GetMechObjectInterface() &&
        TouchHacks::CanUseVehicleSmartBomb(object)) {
        smart_bomb_touch = &holder;
    }
    return true;
}

bool MechInputTouchGestureBasedController::OnHold(GameObject_s &object, TouchHolder &holder) {
    if (object.apiobj.character_data == NULL || object.apiobj.character_data->player_config == NULL ||
        (object.field_0xcc0 != NULL && object.field_0xcc0->id != id_YODA)) {
        return false;
    }
    MechObjectInterface *target = holder.target_object.Get();
    if (target == NULL || object.touch_task != NULL) {
        return false;
    }
    VuVec position(holder.down_position.x, holder.down_position.y, 0.0f, 1.0f);
    const i32 type = target->GetObjectType();
    if (type == 12 && object.force_glow_candidate != NULL && target->GetPart() == object.force_glow_candidate) {
        StartNewTask(new MechTouchTaskUseForce(*this, target, position), holder, true, true);
        holder.consumed = 1;
        return true;
    }
    if (type == 5) {
        target->TargetedFlash();
        StartNewTask(new MechTouchTaskUseForce(*this, target, position), holder, true, true);
        holder.consumed = 1;
        return true;
    }
    if (type == 6) {
        target->TargetedFlash();
        StartNewTask(new MechTouchTaskBuildIt(*this, target, position), holder, true, true);
        holder.consumed = 1;
        return true;
    }
    if (type != 2) {
        return false;
    }
    GameObject_s *character = target->GetCharacterObject();
    if (character == NULL) {
        return false;
    }
    if (character->apiobj.character_data == NULL) {
        holder.consumed = 1;
        return true;
    }
    if (character->apiobj.character_data->player_config != NULL &&
        (character->apiobj.character_data->player_config->flags_090 & 0x40) != 0 && character->field_0xcc0 == NULL &&
        TouchHacks::CanTagVehicle(object, *character)) {
        StartNewTask(new MechTouchTaskTag(*this, *character), holder, true, true);
    } else if (VehicleArea == 0 && TouchHacks::CanTagTo(object, *character)) {
        tag_button = MechSystems::Get()->NewTagButton(*character, holder);
    } else if (character == object.force_glow_candidate) {
        StartNewTask(new MechTouchTaskUseForce(*this, target, position), holder, true, true);
    } else if (character->apiobj.field_0x27c == -1 && static_cast<i32>(character->apiobj.field_0x1f4) >= 0) {
        StartNewTask(new MechTouchTaskAttack(*this, target, position), holder, true, true);
    } else if (character == &object) {
        StartNewTask(new MechTouchTaskBlock(*this), holder, false, true);
    }
    holder.consumed = 1;
    return true;
}

bool MechInputTouchGestureBasedController::OnRelease(GameObject_s &object, TouchHolder &holder) {
    if (smart_bomb_touch == &holder) {
        smart_bomb_touch = NULL;
    }
    if (field_94 == &holder) {
        field_94 = NULL;
    }
    if (field_90 == &holder) {
        if (((object.apiobj.character_data != NULL && (object.apiobj.character_data->model_flags & 0x40) != 0) ||
             object.id == id_WATTO) &&
            object.character_context == LEGOCONTEXT_JUMP) {
            KillTasks(true);
        }
        CABLE_s *cable = GameObjOwnsAnyCables(&object);
        if (cable != NULL) {
            if (cable->target != NULL && TouchHacks::FindBombTarget(object) != NULL) {
                MechObjectInterface *target_object = cable->target->GetMechObjectInterface();
                CantPickupBombTimerAddon *addon = NU_ALLOC_T(CantPickupBombTimerAddon, 1, "", 0);
                if (addon != NULL) {
                    new (addon) CantPickupBombTimerAddon(*target_object, 6.0f);
                }
                cable->target->GetAddons(true)->Add(*addon);
            }
            ReleaseCable(cable, 0);
        }
        field_90 = NULL;
        field_a5 = 0;
    }
    if (object.touch_task != NULL && (object.touch_task->flags & 1) != 0 &&
        object.touch_task->touch_holder == &holder) {
        KillTasks(true);
        StartNewTask(NULL, holder, false, true);
    }
    return false;
}

bool MechInputTouchGestureBasedController::OnSwipe(GameObject_s &object, TouchHolder &holder, i32 index) {
    if (object.apiobj.character_data == NULL || object.apiobj.character_data->player_config == NULL) {
        return false;
    }
    if (ZipUp_FindNearest(WORLD, &object.apiobj.position, object.field_0x1008, NULL, NULL, &object, true) != NULL) {
        StartNewTask(new MechTouchTaskUseZipUp(*this), holder, false, true);
        MechSystems::Get()->NewSwipeMarker(holder, index, static_cast<SwipeDecalRenderer::Style>(0));
        return true;
    }
    if (object.id == id_GRABCONTROL && IsDownSwipe(holder.swipe_samples[index].position, holder.touch_position)) {
        button_was_pressed[0] = 1;
        MechSystems::Get()->NewSwipeMarker(holder, index, static_cast<SwipeDecalRenderer::Style>(0));
        return true;
    }
    if (WORLD->current_level == HOTHESCAPEB_LDATA && object.id == id_SNOWMOB && DoBuckStart(&object)) {
        return true;
    }
    if (object.field_0xe31 == 1 && IsDownSwipe(holder.swipe_samples[index].position, holder.touch_position)) {
        StartJetPackFall(&object, 0);
        return false;
    }
    if (object.character_context == LEGOCONTEXT_JUMP) {
        if (object.touch_task == NULL || object.touch_task->GetHashId().value != MechTouchTaskJump::HashId.value) {
            return false;
        }
        if ((TouchHacks::CanLunge(object) || TouchHacks::CanSlam(object)) &&
            IsDownSwipe(holder.swipe_samples[index].position, holder.touch_position) &&
            !TouchHacks::CheckForAboutToRunIntoKillTerrain(object, 0.1f)) {
            const i32 style = object.jump_sequence > 1 ? 2 : 1;
            MechSystems::Get()->NewSwipeMarker(holder, index, static_cast<SwipeDecalRenderer::Style>(style));
            object.field_0xf04 |= 0x20;
            return false;
        }
        MechSystems::Get()->NewSwipeMarker(holder, index, static_cast<SwipeDecalRenderer::Style>(0));
        object.field_0xf04 |= 0x10;
        return false;
    }

    MechSystems::Get()->NewSwipeMarker(holder, index, static_cast<SwipeDecalRenderer::Style>(0));
    JumpTriggerPacket packet = {};
    packet.type = 3;
    packet.player = &object;
    packet.touch_holder = &holder;
    packet.velocity = VuVec_Zero;
    packet.start = holder.swipe_samples[index].position;
    packet.end = holder.touch_position;
    isBucking = (reinterpret_cast<u8 *>(object.apiobj.character_data->player_config)[0x96] & 2) != 0;
    TriggerJumpTask(packet, true, false, true);
    return true;
}

bool MechInputTouchGestureBasedController::PerformCloseMechanic(GameObject_s &object, TouchHolder &holder) {
    if ((object.field_0xcc0 != NULL && object.field_0xcc0->id != id_YODA) || VehicleArea != 0) {
        return false;
    }

    VuVec position(holder.down_position.x, holder.down_position.y, 0.0f, 1.0f);
    if (TouchHacks::CanUseTeleport(object)) {
        VuVec teleport_position;
        if (Teleport_Find(&object, 0.0025f, &teleport_position) != NULL) {
            f32 dx = teleport_position.x - object.apiobj.position.x;
            f32 dy = teleport_position.y - object.apiobj.position.y;
            f32 dz = teleport_position.z - object.apiobj.position.z;
            if (__builtin_fabsf(dy) < object.apiobj.scaled_height && dx * dx + dz * dz < 0.1225f) {
                StartNewTask(new MechTouchTaskUseTeleport(*this, NULL, position), holder, true, true);
                return true;
            }
        }
    }

    if (TouchHacks::CanUseZipup(object) &&
        ZipUp_FindNearest(WORLD, &object.apiobj.position, object.field_0x1008, NULL, NULL, &object, true) != NULL) {
        StartNewTask(new MechTouchTaskUseZipUp(*this), holder, true, true);
        return true;
    }

    f32 distance = 1000000000.0f;
    if (TouchHacks::CanUseHatMachine(object)) {
        HATMACHINE_s *machine = HatMachine_FindNearest(WORLD, &object.apiobj.position, &object, &distance);
        f32 range = object.field_0x1008 + 0.2f;
        if (machine != NULL && distance <= range * range) {
            StartNewTask(new MechTouchTaskHatMachine(*this, machine->GetMechObjectInterface(), position), holder, true,
                         true);
            return true;
        }
    }

    distance = 1000000000.0f;
    if (TouchHacks::CanUseLever(object)) {
        LEVER_s *lever = Lever_FindNearest(WORLD, &object.apiobj.position, &object, &distance);
        f32 range = object.field_0x1008 + 0.2f;
        if (lever != NULL && distance <= range * range) {
            StartNewTask(new MechTouchTaskPullLever(*this, lever->GetMechObjectInterface(), position), holder, true,
                         true);
            return true;
        }
    }

    distance = 1000000000.0f;
    GIZPANEL_s *panel = GizPanel_FindNearest(WORLD, &object.apiobj.position, &object, &distance, 0);
    if (panel != NULL && GizPanel_CanUsePanel(&object, panel)) {
        f32 range = object.field_0x1008 + 0.2f;
        if (distance <= range * range) {
            StartNewTask(new MechTouchTaskPanel(*this, panel->GetMechObjectInterface(), position), holder, true, true);
            return true;
        }
    }

    f32 range = object.field_0x1008 * 4.0f;
    GameObject_s *candidate =
        FindNearestGameObject(&object.apiobj.position, &object, 0, range, range, -1, -1, 100, NULL, 0, NULL, false);
    if (candidate != NULL && static_cast<i32>(candidate->apiobj.field_0x1f4) >= 0) {
        StartNewTask(new MechTouchTaskAttack(*this, candidate->GetMechObjectInterface(), position), holder, false,
                     true);
        return true;
    }
    return false;
}

void MechInputTouchGestureBasedController::ProcessAutoJumpOverGap(GameObject_s *object) {
    LEVELDATA *level = WORLD->current_level;
    if (level == RESCUEE_LDATA || level == GUNGAN_A_LDATA) {
        return;
    }
    if (level == MOSEISLEYA_LDATA && (object->apiobj.character_data->model_flags & 0x40) == 0) {
        return;
    }
    TouchHolder *holder = field_90;
    if (holder == NULL && object->touch_task != NULL) {
        holder = object->touch_task->touch_holder;
    }
    if (holder == NULL) {
        return;
    }
    if (object->apiobj.field_0x27d == 0 && !ObjLandReady(object)) {
        return;
    }
    if (object->character_context == LEGOCONTEXT_JUMP || object->character_context == LEGOCONTEXT_LAND_JUMP ||
        (object->touch_task != NULL && !object->touch_task->IsGoToTask()) || (object->field_0xf04 & 0x40) != 0) {
        return;
    }
    bool danger = TouchHacks::CheckForAboutToRunIntoKillTerrain(*object, 0.3f);
    if (!danger && !TouchHacks::CheckForAboutToRunOffAnEdge(*object, 0.3f)) {
        return;
    }
    JumpTriggerPacket packet = {};
    packet.type = 1;
    packet.player = object;
    packet.touch_holder = holder;
    packet.velocity = VuVec(object->apiobj.velocity.x, object->apiobj.velocity.y, object->apiobj.velocity.z, 1.0f);
    if ((object->apiobj.character_data->model_flags & 0x40) == 0 && object->id != id_WATTO &&
        MechAutoJumpGetBest(packet, object->apiobj.facing_angle) == NULL &&
        !TouchHacks::CheckJumpForLandingSpot(*object, danger ? 2.0f : 0.05f)) {
        return;
    }
    NUVEC previous_velocity = object->apiobj.velocity;
    NUVEC previous_target_velocity = object->target_velocity;
    f32 speed =
        object->apiobj.character_data->player_config != NULL
            ? *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(object->apiobj.character_data->player_config) + 0x1c)
            : 0.0f;
    NUVEC boosted_velocity = object->apiobj.velocity;
    f32 magnitude_sq = boosted_velocity.x * boosted_velocity.x + boosted_velocity.y * boosted_velocity.y +
                       boosted_velocity.z * boosted_velocity.z;
    if (magnitude_sq < speed * speed) {
        NuVecNorm(&boosted_velocity, &boosted_velocity);
        boosted_velocity.x *= speed;
        boosted_velocity.y *= speed;
        boosted_velocity.z *= speed;
    }
    object->apiobj.velocity = boosted_velocity;
    object->target_velocity = boosted_velocity;
    if (!TriggerJumpTask(packet, false, true, true)) {
        object->apiobj.velocity = previous_velocity;
        object->target_velocity = previous_target_velocity;
    }
}

void MechInputTouchGestureBasedController::ProcessAutoJumpWhenStuck(GameObject_s &object) {
    TouchHolder *holder = field_90;
    if (holder == NULL && object.touch_task != NULL) {
        holder = object.touch_task->touch_holder;
    }
    if (holder == NULL) {
        return;
    }
    if (object.character_context != -1 || object.pad_gamepad == NULL ||
        (object.apiobj.character_data->model_flags & 0x2000) != 0) {
        field_98 = 0.0f;
        field_a6 = 0;
        return;
    }
    const f32 intended_speed = object.pad_gamepad->input_magnitude * 0.5f;
    const f32 movement_sq =
        object.apiobj.velocity.x * object.apiobj.velocity.x + object.apiobj.velocity.z * object.apiobj.velocity.z;
    if (movement_sq >= intended_speed * intended_speed) {
        field_98 = 0.0f;
        field_a6 = 0;
        return;
    }
    field_98 += FRAMETIME;
    if (field_98 <= 0.2f || field_a6 != 0) {
        return;
    }
    field_a6 = 1;
    NUVEC intended = object.target_velocity;
    f32 length = NuFsqrt(intended.x * intended.x + intended.y * intended.y + intended.z * intended.z);
    if (length <= 0.0f) {
        return;
    }
    intended.x /= length;
    intended.y /= length;
    intended.z /= length;
    NUVEC probe = object.apiobj.position;
    probe.x += intended.x * object.target_velocity.x;
    probe.y += intended.y * object.target_velocity.y;
    probe.z += intended.z * object.target_velocity.z;
    if (GameShadow(Player[0], &probe, 2.0f, -1) <= Player[0]->apiobj.position.y + 0.1f) {
        return;
    }
    NUVEC previous_velocity = Player[0]->apiobj.velocity;
    NUVEC previous_target_velocity = Player[0]->target_velocity;
    f32 speed =
        *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(Player[0]->apiobj.character_data->player_config) + 0x18);
    NUVEC velocity = {intended.x * speed, intended.y * speed, intended.z * speed};
    Player[0]->apiobj.velocity = velocity;
    Player[0]->target_velocity = velocity;
    JumpTriggerPacket packet = {};
    packet.type = 1;
    packet.player = &object;
    packet.touch_holder = holder;
    packet.velocity = VuVec(velocity.x, velocity.y, velocity.z, 1.0f);
    if (!TriggerJumpTask(packet, false, true, true)) {
        Player[0]->apiobj.velocity = previous_velocity;
        Player[0]->target_velocity = previous_target_velocity;
    }
}

void MechInputTouchGestureBasedController::ProcessDragMovement(GameObject_s &object) {
    if (field_90 == NULL) {
        return;
    }
    f32 origin_x = field_90->down_position.x;
    f32 origin_y = field_90->down_position.y;
    if (object.id != id_GRABCONTROL && stick_mode.value == 0) {
        origin_x = MAX(-0.6f, (MIN(object.camera_screen_position.x, 0.6f)));
        origin_y = MAX(-0.6f, (MIN(object.camera_screen_position.y, 0.6f)));
    }
    NuVec2 drag = {origin_x - field_90->touch_position.x, origin_y - field_90->touch_position.y};
    if (field_94 != NULL) {
        const NuVec2 other_drag = {origin_x - field_94->touch_position.x, origin_y - field_94->touch_position.y};
        if (drag.x * drag.x + drag.y * drag.y < other_drag.x * other_drag.x + other_drag.y * other_drag.y) {
            drag = other_drag;
        }
    }
    const f32 distance = NuFsqrt(drag.y * drag.y + drag.x * drag.x);
    bool moving = distance > 0.05f;
    const NuVec2 down_drag = {field_90->down_position.x - field_90->touch_position.x,
                              field_90->down_position.y - field_90->touch_position.y};
    if (field_90->target_object.Get() == NULL && isBucking) {
        moving = true;
    }
    const bool dragged = down_drag.x * down_drag.x + down_drag.y * down_drag.y > 0.0025f;
    if (!dragged) {
        if (field_90->held_time > 0.2f || field_90->target_object.Get() == NULL) {
            if (object.character_context != -1 && (object.character_context != LEGOCONTEXT_JUMP || !isBucking)) {
                moving = false;
            }
        } else {
            moving = false;
        }
    }
    field_a0 -= FRAMETIME;
    if (field_a0 > 0.0f) {
        moving = false;
    }
    const i32 context = object.character_context;
    if (context == 0x2b || context == 0x0f || context == 0x36 || context == 0x2a || context == 0x4a ||
        context == 0x47 || context == 0x22 || context == 0x61 || context == 0x3c || tag_button.Get() != NULL ||
        !moving) {
        field_9c = 0.0f;
        return;
    }
    if (!(object.touch_task != NULL && (object.touch_task->flags & 4) != 0) && context != 0x27 && context != 0x33 &&
        context != 0x18 && context != 0x0c && context != LEGOCONTEXT_TUBE && context != LEGOCONTEXT_PUSHSPINNER &&
        context != LEGOCONTEXT_JUMP && context != LEGOCONTEXT_BIGJUMP) {
        object.character_context = -1;
    }
    field_a5 = 1;
    KillTasks(false);
    field_90->consumed = 1;
    if (field_94 != NULL) {
        field_94->consumed = 1;
    }
    if (object.touch_task != NULL && (object.touch_task->flags & 8) == 0) {
        return;
    }
    const i32 angle = NuAtan2D(drag.x, drag.y);
    const f32 strength = (MAX(0.0f, (MIN((distance - 0.05f) * 4.0f, 1.0f)))) * 1.4f;
    const f32 stick_x = -(strength * NU_SIN_LUT(angle));
    const f32 stick_y = strength * NU_COS_LUT(angle);
    stick_values[0] = MAX(-1.0f, (MIN(stick_x, 1.0f)));
    stick_values[1] = MAX(-1.0f, (MIN(stick_y, 1.0f)));
    field_9c += FRAMETIME;
    if (field_9c > 1.0f) {
        Hint_SetComplete(0x5f3);
    }
    if (field_94 != NULL) {
        stick_values[2] = stick_values[0];
        stick_values[0] = 0.0f;
        stick_values[3] = stick_values[1];
        stick_values[1] = 0.0f;
    }
}

void MechInputTouchGestureBasedController::Render() {
}

bool MechInputTouchGestureBasedController::StartJumpUsingAIPath(JumpTriggerPacket const &packet, i32 heading) {
    MechAutoJumpConnection *jump = MechAutoJumpGetBest(packet, heading);
    if (jump == NULL) {
        return false;
    }

    AIPATHCNX *connection = jump->connection;
    const i32 direction = jump->direction;
    AIPATHNODE &node = jump->path->nodes[connection->node_indices[direction == 0]];
    GameObject_s *object = Player[player_id];
    const u32 flags = connection->traversal_flags[direction];
    i32 animation;
    bool complete_success_hint;
    const u32 high_jump = flags & (LEGO_AIPATHCNX_HIGH_JUMP | 0x800000);
    if (high_jump != 0 && (object->ai.capabilities & high_jump) != 0) {
        animation = 3;
        complete_success_hint = true;
    } else {
        const u32 double_jump = flags & (LEGO_AIPATHCNX_DOUBLE_JUMP | 0x400000);
        if (double_jump != 0) {
            const bool capable = (object->ai.capabilities & double_jump) != 0;
            animation = capable ? 2 : 1;
            complete_success_hint = capable;
        } else {
            animation = 1;
            complete_success_hint = false;
        }
    }

    if (LEGOACT_FLIP != -1 && object->apiobj.character_model->model_data_b[LEGOACT_FLIP] != NULL) {
        const u16 desired = connection->route_mask - (direction != 0 ? 0x8000 : 0);
        const i32 difference = RotDiff(object->apiobj.facing_angle, desired);
        const i32 absolute_difference = difference < 0 ? -difference : difference;
        if (absolute_difference >= 0x6aab) {
            animation = 4;
        }
    }

    MechTouchTaskBigJump *task = new MechTouchTaskBigJump(*this, node.position, static_cast<i8>(animation));
    TouchHolder *holder = packet.touch_holder;
    StartNewTask(task, *holder, false, false);
    MechAutoJumpSetIsUsing(*object, *jump);
    if (complete_success_hint) {
        Hint_SetComplete(id_HINT_LSW_AUTOJUMP);
    }
    Hint_SetComplete(id_HINT_LSW_AUTOJUMP_FAIL);
    return true;
}

void MechInputTouchGestureBasedController::StartNewTask(MechTouchTask *task, TouchHolder &holder, bool clear_touches,
                                                        bool kill_tasks) {
    GameObject_s *object = Player[player_id];
    if (object == NULL) {
        return;
    }
    if (object->touch_task != NULL && (object->touch_task->flags & 2) != 0) {
        delete task;
        return;
    }
    if (kill_tasks) {
        KillTasks(false);
    }
    if (object->touch_task != NULL && task != NULL) {
        object->touch_task->OnSuspend();
        task->next = object->touch_task;
    }
    object->touch_task = task;
    if (clear_touches) {
        field_90 = NULL;
        field_94 = NULL;
    }
    if (task != NULL) {
        task->character = Player[player_id];
        task->touch_holder = &holder;
        object->touch_task->OnStart();
    }
}

bool MechInputTouchGestureBasedController::TriggerJumpTask(JumpTriggerPacket const &packet, bool disable_autopilot,
                                                           bool use_velocity, bool try_ai_path) {
    GameObject_s *object = packet.player;
    TouchHolder *holder = packet.touch_holder;
    const i32 context = object->character_context;
    if (context == LEGOCONTEXT_JUMP) {
        return false;
    }
    if (object->apiobj.field_0x27d == 0) {
        if (VehicleArea == 0 && object->field_0xcc0 == NULL) {
            return false;
        }
    } else if (VehicleArea == 0 && object->field_0xcc0 == NULL && !TouchHacks::CanJump(*object)) {
        return false;
    }
    if (context != -1 && context != LEGOCONTEXT_COMBO && context != LEGOCONTEXT_PUNCH && context != LEGOCONTEXT_BLOCK &&
        context != LEGOCONTEXT_HOLD) {
        return true;
    }
    if (object->apiobj.field_0x27d == 0 && !ObjLandReady(object) && VehicleArea == 0) {
        return true;
    }

    const i32 model_flags = object->apiobj.character_data->model_flags;
    if ((model_flags & 0x40) != 0 || object->id == id_WATTO) {
        StartNewTask(new MechTouchTaskAstroJetPack(*this), *holder, false, false);
        return true;
    }
    if (try_ai_path && StartJumpUsingAIPath(packet, object->apiobj.facing_angle)) {
        return true;
    }
    StartNewTask(new MechTouchTaskJump(*this, packet, disable_autopilot, use_velocity), *holder, false, false);
    return true;
}

void MechInputTouchGestureBasedController::Update(NuInputTouchData const *data) {
    if (player != NULL && NewMode == 0 && NewLData == NULL && FadeSys.fade == 0.0f && Paused == 0 && CUTSTOPGAME == 0 &&
        !MenuDisable() && TouchHacks::TouchControlsActive && GetMenuID() == -1 && MiniCutCam != 2 &&
        !Controller_IsConnected()) {
        Activate();
    } else {
        Deactivate();
    }
    memset(stick_values, 0, sizeof(stick_values));
    if (!active || data == NULL) {
        return;
    }
    GameObject_s *object = Player[player_id];
    if (object != NULL) {
        if (object->apiobj.field_0x27d != 0) {
            isBucking = false;
        }
        if (object->touch_task != NULL) {
            for (MechTouchTask *task = object->touch_task->next; task != NULL; task = task->next) {
                task->BackgroundProcess();
            }
            if (object->touch_task->character != player || !object->touch_task->Update()) {
                if (object->touch_task != NULL) {
                    TouchHolder *holder = object->touch_task->touch_holder;
                    if ((object->touch_task->flags & 1) != 0) {
                        field_a0 = 0.75f;
                    }
                    MechTouchTask *next = object->touch_task->next;
                    object->touch_task->OnStop();
                    delete object->touch_task;
                    object->touch_task = next;
                    if (next != NULL) {
                        next->OnResume();
                    }
                    if (object->touch_task == NULL && holder->is_down && field_90 == NULL) {
                        field_90 = holder;
                        field_a5 = 0;
                    }
                }
            }
        }
        ProcessDragMovement(*object);
        if (VehicleArea == 0 && (object->apiobj.character_data->model_flags & 0x2000) == 0) {
            ProcessAutoJumpOverGap(object);
            ProcessAutoJumpWhenStuck(*object);
        }
        if (smart_bomb_touch != NULL) {
            MechObjectInterface *touched_object = smart_bomb_touch->target_object.Get();
            VuVec position(smart_bomb_touch->down_position.x, smart_bomb_touch->down_position.y, 0.0f, 1.0f);
            if (touched_object == MechInputTouchSystem::FindTargetObject(*object, position, 1, touched_object, NULL)) {
                if (smart_bomb_touch->held_time >= 1.0f) {
                    TouchHacks::TriggerVehicleSmartBomb(*player);
                    smart_bomb_touch = NULL;
                } else {
                    TouchHacks::PlaySmartBombBuildupEffects(*player, smart_bomb_touch->held_time, 1.0f);
                }
            } else {
                smart_bomb_touch = NULL;
            }
        }
        if (tag_button.Get() != NULL && tag_button.Get()->on_click != NULL) {
            tag_button = NuMechPtr<MechTouchUIElement, 4>();
        }
    }
    UpdateButtons();
}

MechInputTouchGestureBasedController::~MechInputTouchGestureBasedController() {
}
