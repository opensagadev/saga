#include "decomp.h"
#include "MechInputTouch_types.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/lever.h"
#include "globals.h"
#include "legoapi/items/base/apiobject.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/items/objects/gameobjects.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/menus/core/gamehint.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/render/core/terrain.h"

extern i16 id_YODA;
extern i16 id_YODAGHOST;
extern i16 id_WATTO;
void GizPanel_Use(GameObject_s &, GIZPANEL_s &);
void TakeOver2GetIn(GameObject_s *, GameObject_s *);
i32 TagCode(GameObject_s *, GameObject_s *, i32, i32, i32);
void SetForcedAttackOpponent(MechObjectInterface *);
void SetWeaponOut(GameObject_s *);
bool FireBountyHunterRocket(GameObject_s *);
void SetBobaRocketTarget(MechObjectInterface *);
f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);
extern NuMechPtr<MechObjectInterface, 4> NextThermalTarget;

f32 s_mechTouchMoveToStuckVel = 0.2f;
f32 s_mechTouchMoveToStuckTime = 0.55f;
f32 s_mechTouchMoveToWalkDist = 0.4f;
f32 s_mechTouchMoveToWalkAlwaysDist = 0.0f;
f32 s_mechTouchMoveToWalkSpeed = 0.75f;

HashedKey MechTouchTask::HashId("UNKNOWN");
HashedKey MechTouchTaskGoTo::HashId("Goto");
HashedKey MechTouchTaskPlannedGoTo::HashId("PlannedGoTo");
HashedKey MechTouchTaskAttack::HashId("Attack");
HashedKey MechTouchTaskBlock::HashId("Block");
HashedKey MechTouchTaskUseForce::HashId("Force");
HashedKey MechTouchTaskUseTeleport::HashId("Teleport");
HashedKey MechTouchTaskBuildIt::HashId("Build It");
HashedKey MechTouchTaskTag::HashId("Tag");
HashedKey MechTouchTaskJump::HashId("Jump");
HashedKey MechTouchTaskAstroJetPack::HashId("Astro Jet Pack");
HashedKey MechTouchTaskBigJump::HashId("Big Jump");
HashedKey MechTouchTaskPullLever::HashId("Pull Lever");
HashedKey MechTouchTaskHatMachine::HashId("Hat Machine");
HashedKey MechTouchTaskUseZipUp::HashId("Zip Up");
HashedKey MechTouchTaskPanel::HashId("Panel");
HashedKey MechTouchTaskPlannedDoubleClickGoTo::HashId("DblClickGoTo");

MechTouchTask::MechTouchTask(MechInputTouchGestureBasedController &owner)
    : controller(&owner), touch_holder(NULL), flags(0) {
    next = NULL;
}

MechTouchTask::~MechTouchTask() {
}

MechTouchTaskTag::MechTouchTaskTag(MechInputTouchGestureBasedController &owner, GameObject_s &object)
    : MechTouchTask(owner), target(&object) {
}

bool MechTouchTaskTag::Update() {
    GameObject_s *object = Player[controller->player_id];
    if (object != NULL) {
        if ((target->apiobj.character_data->player_config->flags_090 & 0x40) != 0) {
            TakeOverGameObject(object, target, 1, 0);
        } else if ((target->field_0xf00 & 2) != 0) {
            TakeOver2GetIn(target, object);
        } else {
            TagCode(object, target, 0, 1, 1);
        }
    }
    return false;
}

MechTouchTaskGoTo::MechTouchTaskGoTo(MechInputTouchGestureBasedController &owner, MechObjectInterface *object)
    : MechTouchTask(owner), target(object), room(-1), field_30(0), field_34(0), field_38(0), field_3c(0), field_40(0),
      field_44(0), field_4c(0), field_4d(1), field_4e(0), field_4f(0), field_50(0), field_51(0) {
}

void MechTouchTaskGoTo::OnStart() {
    if (target.Get() != NULL && target.Get()->GetObjectType() == 1 && field_50 == 0) {
        move_to_marker = NuMechPtr<MoveToMarker, 4>(MechSystems::Get()->NewMoveToMarker(*target.Get()));
    }
}

void MechTouchTaskGoTo::OnStop() {
    if (move_to_marker.Get() != NULL) {
        move_to_marker.Get()->FadeOut();
        move_to_marker = NuMechPtr<MoveToMarker, 4>();
    }
}

void MechTouchTaskGoTo::Render() {
}

bool MechTouchTaskGoTo::Update() {
    if (target.Get() != NULL) {
        GameObject_s *object = Player[controller->player_id];
        if (object != NULL) {
            const f32 object_x = object->apiobj.collision_position.x;
            const f32 object_y = object->apiobj.collision_position.y;
            const f32 object_z = object->apiobj.collision_position.z;
            VuVec position;
            target.Get()->GetPos(position, room);
            const f32 target_y = position.y;
            const f32 dx = position.x - object_x;
            const f32 dz = position.z - object_z;
            field_40 = NuFsqrt(dx * dx + dz * dz);
            VuVec direction;
            field_44 = MechInputTouchSystem::DetermineMoveDir2D(*object, position, true, direction);
            if (field_4d != 0) {
                field_4c = field_40 >= s_mechTouchMoveToWalkDist;
                field_3c = field_40;
                field_4d = 0;
                field_38 = field_40;
            }
            if (field_51 == 0 && (object->apiobj.character_data->model_flags & 0x40000000) == 0) {
                UpdateStuck();
            }
            if (field_4e != 0) {
                field_48 -= FRAMETIME;
                if (!(field_48 < 0.0f) && !(field_40 < 0.1f) && object->character_context == -1) {
                    controller->button_was_pressed[2] = 1;
                }
            } else if (!(field_44 < 0.000001f) && !(field_40 < 0.1f)) {
                if (field_4c == 0 || !(field_40 > s_mechTouchMoveToWalkAlwaysDist)) {
                    direction.x *= s_mechTouchMoveToWalkSpeed;
                    direction.y *= s_mechTouchMoveToWalkSpeed;
                    direction.z *= s_mechTouchMoveToWalkSpeed;
                }
                movement_x = direction.x;
                movement_y = -direction.y;
                field_38 = field_40;
                field_3c = MIN(field_3c, field_40);
                field_34 += FRAMETIME;
                controller->stick_values[0] = movement_x;
                controller->stick_values[1] = movement_y;
                if (GetHashId().value == HashId.value || GetHashId().value == MechTouchTaskAttack::HashId.value) {
                    if (target_y - object_y >= -0.15f && (object->apiobj.character_data->model_flags & 0x40) == 0 &&
                        object->id != id_WATTO) {
                        ++object->edge_stop_requests;
                    }
                }
                return true;
            }
        }
    }
    if (move_to_marker.Get() != NULL && GetHashId().value == HashId.value) {
        move_to_marker.Get()->FadeOut();
        move_to_marker = NuMechPtr<MoveToMarker, 4>();
    }
    return false;
}

void MechTouchTaskGoTo::UpdateStuck() {
    const bool was_stuck = field_4e != 0;
    field_4e = 0;
    if (was_stuck) {
        field_48 = 0;
        return;
    }

    f32 velocity_threshold = s_mechTouchMoveToStuckVel;
    const f32 velocity_delta = field_38 - field_40;
    if (player != NULL && (player->id == id_YODA || player->id == id_YODAGHOST)) {
        velocity_threshold *= 0.25f;
    }

    if (field_4f == 0) {
        field_4f = velocity_delta > velocity_threshold;
    } else {
        bool stuck = false;
        if (field_34 > 0.25f && velocity_delta > 0.0f) {
            stuck = velocity_delta < velocity_threshold * 0.5f;
        }
        field_4e = stuck;
    }

    f32 stuck_time = 0.0f;
    if (field_40 >= field_3c) {
        stuck_time = field_30 + FRAMETIME;
    }
    field_30 = stuck_time;
    if (stuck_time > s_mechTouchMoveToStuckTime) {
        field_4e = 1;
    }
    if (field_4e != 0) {
        field_48 = 1.0f;
    }
}

bool MechTouchTaskGoTo::UpdateTarget(MechObjectInterface &object) {
    target = NuMechPtr<MechObjectInterface, 4>(&object);
    return true;
}

MechTouchTaskGoTo::~MechTouchTaskGoTo() {
}

MechTouchTaskJump::MechTouchTaskJump(MechInputTouchGestureBasedController &owner, JumpTriggerPacket const &packet,
                                     bool disable_auto, bool direct_velocity)
    : MechTouchTask(owner), velocity(packet.velocity), stick(), start(), end() {
    frames = 0;
    started = false;
    landed = false;
    descending = false;
    disable_autopilot = disable_auto;
    use_velocity = direct_velocity;
    if (packet.type == 3) {
        start = packet.start;
        end = packet.end;
        f32 x = packet.start.x - packet.end.x;
        f32 y = packet.start.y - packet.end.y;
        if (x * x + y * y > 0.01f) {
            i32 angle = NuAtan2D(x, y);
            stick.x = NU_SIN_LUT(angle);
            stick.y = NU_COS_LUT(angle);
            f32 speed = player->apiobj.character_data->game_character->movement_speed;
            stick.x *= speed;
            stick.y *= speed;
            stick.x = MAX(-1.0f, MIN(-stick.x, 1.0f));
            stick.y = MAX(-1.0f, MIN(stick.y, 1.0f));
        }
    }
    flags |= 10;
}

void MechTouchTaskJump::OnStop() {
    GameObject_s *object = Player[controller->player_id];
    if (object != NULL) {
        object->jump_input_flags &= ~0x10;
        MechAddonCollection *addons = object->GetAddons(false);
        if (addons != NULL) {
            MechAddon *autopilot = addons->Find(MechJumpAutoPilotAddon::s_hashId);
            if (autopilot != NULL) {
                addons->Remove(*autopilot);
            }
        }
        object->jump_input_flags &= ~0x30;
    }
}

bool MechTouchTaskJump::Update() {
    if ((character->jump_input_flags & 0x20) != 0) {
        ++character->edge_stop_requests;
    }
    if (started && player->apiobj.field_0x27d != 0 && frames > 5) {
        landed = true;
    }
    bool stationary = velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z < 1.0f &&
                      stick.x * stick.x + stick.y * stick.y < 1.4210854715202004e-14f;
    if (!landed) {
        if (player->character_context != LEGOCONTEXT_JUMP) {
            if (velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z > 1.0f || use_velocity) {
                if (!use_velocity) {
                    NuVecNorm(&velocity.xyz, &velocity.xyz);
                    f32 speed = player->apiobj.character_data->game_character->movement_speed;
                    velocity.x *= speed;
                    velocity.y *= speed;
                    velocity.z *= speed;
                }
                player->apiobj.velocity = player->target_velocity = velocity.xyz;
            } else {
                controller->stick_values[0] = stick.x;
                controller->stick_values[1] = stick.y;
                if (stick.x * stick.x + stick.y * stick.y < 1.1920928955078125e-7f) {
                    player->apiobj.velocity.x = player->target_velocity.x = 0.0f;
                    player->apiobj.velocity.z = player->target_velocity.z = 0.0f;
                }
            }
        }
        if (player->character_context == LEGOCONTEXT_JUMP) {
            if (started) {
                if (player->apiobj.velocity.y <= 0.0f && !descending) {
                    descending = true;
                }
            } else {
                descending = false;
            }
        } else {
            descending = started;
        }
        if (!started) {
            controller->button_was_pressed[2] = 1;
        }
        if (!disable_autopilot) {
            if (!started && player->GetAddons(false) != NULL) {
                MechAddon *autopilot = player->GetAddons(true)->Find(MechJumpAutoPilotAddon::s_hashId);
                if (autopilot != NULL) {
                    static_cast<MechJumpAutoPilotAddon *>(autopilot)->Recalculate();
                }
            }
            if (player->character_context == LEGOCONTEXT_JUMP && !stationary &&
                (player->GetAddons(false) == NULL ||
                 player->GetAddons(true)->Find(MechJumpAutoPilotAddon::s_hashId) == NULL)) {
                f32 x = player->apiobj.velocity.x;
                f32 z = player->apiobj.velocity.z;
                f32 length = NuFsqrt(x * x + z * z);
                f32 scale = 0.0f;
                if (length != 0.0f) {
                    scale = 1.0f / length;
                }
                x *= scale;
                z *= scale;
                f32 speed = player->apiobj.character_data->game_character->movement_speed;
                player->apiobj.velocity.x = x * speed;
                player->apiobj.velocity.z = z * speed;
                MechJumpAutoPilotAddon *autopilot = new MechJumpAutoPilotAddon(*player->GetMechObjectInterface());
                player->GetAddons(true)->Add(*autopilot);
            }
        }
    }
    if (player->character_context == LEGOCONTEXT_JUMP) {
        if (player->action_movement_state != 1) {
            player->apiobj.facing_angle = player->apiobj.movement_facing_angle =
                NuAtan2D(player->apiobj.velocity.x, player->apiobj.velocity.z);
        }
        started = true;
    }
    ++frames;
    return frames <= 4 || player->character_context == LEGOCONTEXT_JUMP;
}

MechTouchTaskBlock::MechTouchTaskBlock(MechInputTouchGestureBasedController &owner) : MechTouchTask(owner) {
    flags |= 5;
}

bool MechTouchTaskBlock::Update() {
    controller->button_was_pressed[0] = 1;
    return true;
}

MechTouchTaskPanel::MechTouchTaskPanel(MechInputTouchGestureBasedController &owner, MechObjectInterface *object,
                                       VuVec const &)
    : MechTouchTaskGoTo(owner, object) {
}

bool MechTouchTaskPanel::Update() {
    if (target.Get() == NULL || player == NULL) {
        return false;
    }
    GIZPANEL_s *panel = target.Get()->GetPanel();
    if (panel == NULL) {
        return false;
    }
    if (player->character_context != 0x0b) {
        VuVec position;
        target.Get()->GetPos(position, -1);
        position.x -= player->apiobj.position.x;
        position.y -= player->apiobj.position.y;
        position.z -= player->apiobj.position.z;
        const f32 distance_squared = position.x * position.x + position.y * position.y + position.z * position.z;
        if (distance_squared < 1.4f * 1.4f) {
            field_51 = 1;
            const f32 radius = (player->apiobj.field_0x1dc + 0.25f) * panel->target_scale;
            if (distance_squared < radius * radius) {
                GizPanel_Use(*player, *panel);
            }
        }
        if (!MechTouchTaskGoTo::Update()) {
            controller->button_pressed[3] = 1;
            return false;
        }
    }
    return !panel->state;
}

MechTouchTaskAttack::MechTouchTaskAttack(MechInputTouchGestureBasedController &owner, MechObjectInterface *object,
                                         VuVec const &)
    : MechTouchTaskGoTo(owner, object) {
}

void MechTouchTaskAttack::OnStart() {
    MechTouchTaskGoTo::OnStart();
    SetForcedAttackOpponent(target.Get());
    NextThermalTarget = NuMechPtr<MechObjectInterface, 4>(target.Get());
    SetWeaponOut(player);
    field_60 = 0;
    if (move_to_marker.Get() != NULL) {
        move_to_marker.Get()->field_5c = 1.0f;
    }
}

void MechTouchTaskAttack::OnStop() {
    MechTouchTaskGoTo::OnStop();
    SetForcedAttackOpponent(NULL);
}

void MechTouchTaskAttack::Render() {
    MechTouchTaskGoTo::Render();
}

bool MechTouchTaskAttack::Update() {
    GameObject_s *object = Player[controller->player_id];
    if (object != NULL && target.Get() != NULL) {
        MechObjectInterface *opponent = target.Get();
        const NUVEC origin = object->apiobj.position;
        VuVec delta;
        opponent->GetPos(delta, -1);
        delta.x -= origin.x;
        delta.y -= origin.y;
        delta.z -= origin.z;
        const f32 distance_squared = delta.x * delta.x + delta.z * delta.z;
        const f32 radius = opponent->GetRadius();
        bool nearby = false;
        if ((object->apiobj.character_data->model_flags & 8) != 0) {
            nearby = FindNearestGameObject(&object->apiobj.position, object, 0, 1.0f, 1.0f, -1, -1, 100, NULL, 0, NULL,
                                           true) != NULL;
            if ((object->field_0xe22 & 8) != 0) {
                nearby = true;
            }
        }

        const u32 model_flags = object->apiobj.character_data->model_flags;
        f32 attack_range_squared;
        if ((model_flags & 0x40) != 0) {
            attack_range_squared = 0.09f;
        } else {
            f32 attack_range = radius + 0.2f;
            if ((model_flags & 0x40000000) != 0) {
                attack_range += object->apiobj.collision_radius * 1.25f;
            }
            attack_range_squared = attack_range * attack_range;
        }
        const bool in_range = distance_squared <= attack_range_squared;
        if (in_range || nearby || (object->field_0xef8 & 4) != 0 || object->id == id_TRAININGREMOTE ||
            (object->field_0xcc0 != NULL && TouchHacks::CanShoot(*object)) || VehicleArea != 0) {
            if (opponent->GetCharacterObject() != object) {
                ForceNextShootTarget(*opponent);
            }
            field_51 = 1;
            GIZMOBLOWUP_s *blowup;
            if (TouchHacks::CanThrowBountyBomb(*object) && (blowup = opponent->GetGizBlowup()) != NULL &&
                blowup->type != NULL && (blowup->draw_flags & 2) != 0) {
                if (FireBountyHunterRocket(object)) {
                    SetBobaRocketTarget(opponent);
                } else {
                    controller->button_pressed[3] = 1;
                }
            } else {
                if (VehicleArea != 0) {
                    controller->button_pressed[3] = 1;
                }
                field_60 |= in_range;
                controller->button_pressed[0] = 1;
            }
        }

        if ((object->field_0xef8 & 4) != 0 || VehicleArea != 0 ||
            (object->field_0xcc0 != NULL && TouchHacks::CanShoot(*object))) {
            if (VehicleArea != 0 && object->torpedo->target != NULL &&
                object->torpedo->target == target.Get()->GetTgtVoidPtr()) {
                return false;
            }
            if (object->character_context == -1 || (object->apiobj.character_data->model_flags & 0x2000) == 0) {
                object->apiobj.movement_facing_angle = NuAtan2D(delta.x, delta.z);
            }
            return false;
        }
    }

    bool keep = target.Get() != NULL && !target.Get()->IsDead();
    if (target.Get() != NULL) {
        GameObject_s *opponent = target.Get()->GetCharacterObject();
        if (opponent != NULL) {
            keep = field_60 == 0 && opponent->apiobj.field_0x287 == 0;
        }
    }
    MechTouchTaskGoTo::Update();
    return keep;
}

MechTouchTaskBigJump::MechTouchTaskBigJump(MechInputTouchGestureBasedController &owner, MechObjectInterface &object,
                                           signed char animation_id)
    : MechTouchTask(owner), animation(animation_id) {
    object.GetPos(destination, -1);
    started = false;
}

MechTouchTaskBigJump::MechTouchTaskBigJump(MechInputTouchGestureBasedController &owner, nuvec_s &position,
                                           signed char animation_id)
    : MechTouchTask(owner), animation(animation_id) {
    destination = VuVec(position.x, position.y, position.z, 1.0f);
    started = false;
}

bool MechTouchTaskBigJump::Update() {
    if (player->character_context != LEGOCONTEXT_BIGJUMP && !started) {
        StartBigJump(player, &destination.xyz, 0, 0.2f, 1.0f, 0, animation);
    }
    started |= player->character_context == LEGOCONTEXT_BIGJUMP;
    return started && player->character_context == LEGOCONTEXT_BIGJUMP;
}

MechTouchTaskBuildIt::MechTouchTaskBuildIt(MechInputTouchGestureBasedController &owner, MechObjectInterface *object,
                                           VuVec const &)
    : MechTouchTaskGoTo(owner, object) {
    if (object->GetGizBuildit() != NULL) {
        ForceBuildItToUseNext(*object->GetGizBuildit());
    }
    flags |= 1;
}

bool MechTouchTaskBuildIt::Update() {
    if (target.Get() == NULL) {
        return false;
    }
    GIZBUILDIT_s *buildit = target.Get()->GetGizBuildit();
    if (buildit == NULL || player == NULL) {
        return false;
    }
    room = buildit->built_object_count;
    VuVec position;
    target.Get()->GetPos(position, buildit->built_object_count);
    position.x -= player->apiobj.position.x;
    position.y -= player->apiobj.position.y;
    position.z -= player->apiobj.position.z;
    const f32 distance_squared = position.x * position.x + position.y * position.y + position.z * position.z;
    if (distance_squared < 4.0f) {
        field_51 = 1;
        const f32 radius = static_cast<f32>(static_cast<u32>(buildit->progress)) / 100.0f * buildit->bounds_radius;
        if (!(player->apiobj.upper_position.y > buildit->start_position.y - radius) ||
            !(player->apiobj.lower_position.y < buildit->start_position.y + radius)) {
            return false;
        }
        controller->button_was_pressed[3] = 1;
    }
    MechTouchTaskGoTo::Update();
    return target.Get()->GetGizBuildit()->build_state == 0;
}

MechTouchTaskUseForce::MechTouchTaskUseForce(MechInputTouchGestureBasedController &owner, MechObjectInterface *object,
                                             VuVec const &)
    : MechTouchTaskGoTo(owner, object) {
    flags |= 1;
    field_60 = 0;
}

void MechTouchTaskUseForce::OnStart() {
    if (player != NULL && target.Get() != NULL) {
        player->force_glow_previous = target.Get()->GetTgtVoidPtr();
    }
    MechTouchTaskGoTo::OnStart();
}

void MechTouchTaskUseForce::OnStop() {
    if (player != NULL && target.Get() != NULL) {
        player->force_glow_previous = NULL;
        if (target.Get()->GetObjectType() == 5) {
            static_cast<GizForceObjectInterface *>(target.Get())->selected_object = NULL;
        }
    }
    MechTouchTaskGoTo::OnStop();
}

bool MechTouchTaskUseForce::Update() {
    MechObjectInterface *object = target.Get();
    if (player == NULL || object == NULL ||
        (object->GetGizForce() == NULL && object->GetPart() == NULL && object->GetCharacterObject() == NULL)) {
        return false;
    }
    if (player->character_context == 8) {
        field_60 = 1;
    }
    VuVec position;
    target.Get()->GetPos(position, room);
    position.x -= player->apiobj.pos_x;
    position.z -= player->apiobj.pos_z;
    GIZFORCE_s *force = object->GetGizForce();
    bool within_range = false;
    if (force != NULL) {
        const f32 dx = position.x;
        const f32 dz = position.z;
        const f32 radius = force->interaction_radius;
        if (TouchHacks::CanUseGizForce(*player, *force)) {
            if (object->GetTgtVoidPtr() == player->force_glow_object) {
                controller->button_was_pressed[3] = 1;
                return true;
            }
            const f32 range = radius * 0.9f;
            within_range = dx * dx + dz * dz < range * range;
        } else {
            return false;
        }
    } else if (object->GetTgtVoidPtr() == player->force_glow_object) {
        controller->button_was_pressed[3] = 1;
        return true;
    }
    if (!within_range && MechTouchTaskGoTo::Update()) {
        return true;
    }
    player->apiobj.movement_facing_angle = NuAtan2D(position.x, position.z);
    return true;
}

MechTouchTaskUseZipUp::MechTouchTaskUseZipUp(MechInputTouchGestureBasedController &owner) : MechTouchTask(owner) {
}

void MechTouchTaskUseZipUp::OnStart() {
    player->target_velocity.x = player->apiobj.velocity.x = 0.0f;
    player->target_velocity.y = player->apiobj.velocity.y = 0.0f;
    player->target_velocity.z = player->apiobj.velocity.z = 0.0f;
}

bool MechTouchTaskUseZipUp::Update() {
    controller->button_pressed[3] = 1;
    return false;
}

MechTouchTaskPullLever::MechTouchTaskPullLever(MechInputTouchGestureBasedController &owner, MechObjectInterface *object,
                                               VuVec const &)
    : MechTouchTaskGoTo(owner, object) {
    lever = object->GetGizLever();
}

bool MechTouchTaskPullLever::Update() {
    if (lever == NULL) {
        return false;
    }
    VuVec position;
    target.Get()->GetPos(position, -1);
    position.x -= player->apiobj.position.x;
    position.y -= player->apiobj.position.y;
    position.z -= player->apiobj.position.z;
    const f32 distance_squared = position.x * position.x + position.y * position.y + position.z * position.z;
    if (distance_squared < 0.25f) {
        f32 nearest_distance;
        if (Lever_FindNearest(WORLD, &player->apiobj.lower_position, player, &nearest_distance) == lever) {
            player->apiobj.movement_facing_angle = NuAtan2D(position.x, position.z);
            Lever_StartPull(player, lever);
        }
    } else if (MechTouchTaskGoTo::Update()) {
        return true;
    }
    return (lever->flags & 0x82) == 0x80;
}

MechTouchTaskHatMachine::MechTouchTaskHatMachine(MechInputTouchGestureBasedController &owner,
                                                 MechObjectInterface *object, VuVec const &)
    : MechTouchTaskGoTo(owner, object) {
    machine = object->GetHatMachine();
}

bool MechTouchTaskHatMachine::Update() {
    if (machine == NULL) {
        return false;
    }
    controller->button_pressed[3] = 1;
    MechTouchTaskGoTo::Update();
    return !machine->state_bit0;
}

void MechTouchTaskPlannedGoTo::AnalysePath() {
    const i32 end = path_index + MIN(2, path_count - path_index);
    VuVec position = start_position;
    for (; path_index <= end;) {
        position.x += step_x;
        position.z += step_z;
        const f32 ground_height = GameShadow(player, &position.xyz, 0.0f, -1);
        if (ground_height > position.y) {
            break;
        }
        path_points[path_index].x = position.x;
        path_points[path_index].z = position.z;
        const i32 layer = EShadowInfo();
        if (static_cast<u32>(layer) <= 16 &&
            (TerLayer[layer].flags & TERRAIN_LAYER_FLAG_REJECT_CHARACTER_SHADOW) != 0) {
            path_points[path_index].y = path_points[path_index - 1].y;
            ++field_20;
            if (field_20 > 1) {
                break;
            }
        } else {
            last_index = path_index;
            path_points[path_index].y = ground_height;
            if (path_index > 1 && path_index < path_count) {
                MechTempPosInterface marker_position;
                marker_position.position = path_points[path_index];
                marker_position.position.x += static_cast<f32>(qrand()) * 1.5259022e-6f - 0.05f;
                marker_position.position.z += static_cast<f32>(qrand()) * 1.5259022e-6f - 0.05f;
                const f32 base_radius = static_cast<f32>(path_index) / static_cast<f32>(path_count) * 0.2f;
                marker_position.radius = MAX(static_cast<f32>(qrand()) * 1.5259022e-7f + base_radius, 0.05f);
                if (field_6fe == 0) {
                    MoveToMarker *marker = MechSystems::Get()->NewMoveToMarker(marker_position);
                    if (marker != NULL) {
                        marker->flags |= 3;
                    }
                }
            }
            field_20 = 0;
        }
        start_position = position;
        position.y = path_points[path_index].y + step_y;
        ++path_index;
    }
    if (path_index <= end) {
        if (last_index > 0) {
            for (i32 i = last_index + 1; i <= path_count; ++i) {
                path_points[i].y = -1000000000.0f;
            }
        }
        field_6ff = 0;
        analysis_state = 1;
        if (completion != NULL) {
            *completion = false;
        }
    } else if (path_index >= path_count) {
        analysis_state = 1;
    }
}

void MechTouchTaskPlannedGoTo::BackgroundProcess() {
    if (player != NULL && move_to_marker.Get() != NULL) {
        const f32 dz = player->apiobj.position.z - waypoints[current_waypoint].position.z;
        const f32 dx = player->apiobj.position.x - waypoints[current_waypoint].position.x;
        if (dx * dx + dz * dz < 0.25f) {
            Hint_SetComplete(0x5f1);
            move_to_marker.Get()->FadeOut();
            NuMechPtr<MoveToMarker, 4> empty_marker;
            move_to_marker = empty_marker;
        }
    }
}

void MechTouchTaskPlannedGoTo::GenerateWaypoints() {
    i32 waypoint_index = 0;
    i32 current = 0;
    if (path_count <= 0 || path_points[1].y == -1000000000.0f) {
        goto write_final_waypoint;
    }
    current = 1;
    while (true) {
        if (path_points[current].y > path_points[current - 1].y + 0.15f) {
            waypoints[waypoint_index].active = 1;
            waypoints[waypoint_index].position = path_points[current - 1];
            waypoints[waypoint_index].field_14 = 0;
            waypoints[waypoint_index].target_position.position = path_points[current - 1];
            waypoints[waypoint_index + 1].active = 1;
            waypoints[waypoint_index + 1].position = path_points[current];
            waypoints[waypoint_index + 1].field_14 = 1;
            waypoints[waypoint_index + 1].target_position.position = path_points[current];
            if (current == path_count || path_points[current + 1].y == -1000000000.0f) {
                break;
            }
            waypoint_index += 2;
        }
        const i32 next = current + 1;
        if (next > path_count || waypoint_index > 30 || path_points[next].y == -1000000000.0f) {
            goto write_final_waypoint;
        }
        current = next;
    }
    goto cleanup;

write_final_waypoint:
    waypoints[waypoint_index].active = 1;
    waypoints[waypoint_index].position = path_points[current];
    waypoints[waypoint_index].field_14 = 0;
    waypoints[waypoint_index].target_position.position = path_points[current];
    target_position.position = path_points[current];

cleanup:
    if (field_6fd == 0) {
        move_to_marker = NuMechPtr<MoveToMarker, 4>(MechSystems::Get()->NewMoveToMarker(target_position));
    }
    delete[] path_points;
    path_points = NULL;
    delete go_to_task;
    go_to_task = NULL;
    analysis_state = 2;
}

MechTouchTaskPlannedGoTo::MechTouchTaskPlannedGoTo(MechInputTouchGestureBasedController &owner,
                                                   MechObjectInterface *object, bool *completed)
    : MechTouchTask(owner), target_position(), target(object), waypoints(), move_to_marker() {
    field_6fd = 0;
    field_6fc = 0;
    field_6fe = 0;
    path_points = NULL;
    completion = completed;
    go_to_task = NULL;
}

void MechTouchTaskPlannedGoTo::OnResume() {
    if (player == NULL) {
        return;
    }
    const f32 dz = player->apiobj.position.z - waypoints[current_waypoint].position.z;
    const f32 dx = player->apiobj.position.x - waypoints[current_waypoint].position.x;
    if (dx * dx + dz * dz < 0.01f) {
        ++current_waypoint;
        field_6fc = 0;
        return;
    }
    if (field_6fc != 0) {
        current_waypoint = 32;
        return;
    }
    MechInputTouchGestureBasedController *owner = controller;
    field_6fc = 1;
    MechTouchTaskGoTo *task = new MechTouchTaskGoTo(*owner, &waypoints[current_waypoint].target_position);
    task->field_50 = 1;
    controller->StartNewTask(task, *touch_holder, true, false);
}

void MechTouchTaskPlannedGoTo::OnStart() {
    if (target.Get() != NULL && player != NULL) {
        MechInputTouchGestureBasedController *owner = controller;
        MechTouchTaskGoTo *task = new MechTouchTaskGoTo(*owner, target.Get());
        go_to_task = task;
        task->field_50 = 1;
        task->OnStart();
        SetupForAnalysis();
    }
    current_waypoint = 0;
}

void MechTouchTaskPlannedGoTo::OnStop() {
    MoveToMarker *marker = move_to_marker.Get();
    if (marker != NULL) {
        marker->FadeOut();
    }
}

void MechTouchTaskPlannedGoTo::SetupForAnalysis() {
    const VuVec origin(player->apiobj.position.x, player->apiobj.position.y, player->apiobj.position.z, 1.0f);
    VuVec destination;
    target.Get()->GetFloorTargetPos(destination, -1);
    const f32 dx = destination.x - origin.x;
    const f32 dz = destination.z - origin.z;
    const i32 estimated_points = static_cast<i32>(NuCeil((dx * dx + dz * dz) / 1.21f));
    if (estimated_points <= 4) {
        path_count = 5;
    } else if (estimated_points <= 9) {
        path_count = 10;
    } else {
        path_count = estimated_points;
    }
    path_points = new VuVec[path_count + 1];
    const volatile VuVec &zero = VuVec_Zero;
    const f32 zero_x = zero.x;
    const f32 zero_y = zero.y;
    const f32 zero_z = zero.z;
    const f32 zero_w = zero.w;
    for (i32 i = 0; i <= path_count; ++i) {
        path_points[i].x = zero_x;
        path_points[i].y = zero_y;
        path_points[i].z = zero_z;
        path_points[i].w = zero_w;
        path_points[i].y = -1000000000.0f;
    }
    path_points[0] = origin;
    start_position = origin;
    path_index = 1;
    step_x = dx / static_cast<f32>(path_count);
    step_z = dz / static_cast<f32>(path_count);
    last_index = -1;
    field_20 = 0;
    analysis_state = 0;
    field_6ff = 1;
    const GAMECHARACTERDATA *character = player->apiobj.character_data->game_character;
    step_y =
        -(character->jump_speed * character->jump_speed) / (character->gravity + character->gravity) * 1.5f + 0.01f;
    start_position.y += step_y;
    if (completion != NULL) {
        *completion = true;
    }
}

bool MechTouchTaskPlannedGoTo::Update() {
    if (go_to_task != NULL && analysis_state <= 1) {
        go_to_task->Update();
    }
    if (analysis_state == 0) {
        AnalysePath();
        return true;
    }
    if (analysis_state == 1) {
        GenerateWaypoints();
        return true;
    }
    if (analysis_state != 2 || player == NULL || current_waypoint > 31) {
        return false;
    }

    if (waypoints[current_waypoint].active == 0) {
        return false;
    }
    if (waypoints[current_waypoint].field_14 == 0) {
        MechTouchPlannedWaypoint &waypoint = waypoints[current_waypoint];
        MechTouchTaskGoTo *task = new MechTouchTaskGoTo(*controller, &waypoint.target_position);
        task->field_50 = 1;
        controller->StartNewTask(task, *touch_holder, true, false);
        return true;
    }
    if (!TouchHacks::CanJump(*player)) {
        return true;
    }

    MechTouchPlannedWaypoint &waypoint = waypoints[current_waypoint];
    const f32 dx = waypoint.position.x - player->apiobj.position.x;
    const f32 dy = waypoint.position.y - player->apiobj.position.y;
    const f32 dz = waypoint.position.z - player->apiobj.position.z;
    f32 velocity_x;
    f32 velocity_y;
    f32 velocity_z;
    if (dx * dx + dy * dy + dz * dz > 1.96f || dy > player->apiobj.scaled_height) {
        const VuVec velocity = TouchHacks::CalculateJumpVelToHitPointDblJump(*player, waypoint.position);
        velocity_x = velocity.x;
        velocity_y = velocity.y;
        velocity_z = velocity.z;
        player->jump_input_flags |= 0x10;
    } else {
        const VuVec velocity = TouchHacks::CalculateJumpVelToHitPoint(*player, waypoint.position);
        velocity_x = velocity.x;
        velocity_y = velocity.y;
        velocity_z = velocity.z;
    }
    const f32 previous_z = player->apiobj.velocity.z;
    const f32 previous_target_z = player->target_velocity.z;
    const f32 previous_y = player->apiobj.velocity.y;
    const f32 previous_target_y = player->target_velocity.y;
    const f32 previous_x = player->apiobj.velocity.x;
    const f32 previous_target_x = player->target_velocity.x;
    const i32 angle = NuAtan2D(velocity_x, velocity_z);
    player->apiobj.movement_facing_angle = angle;
    player->apiobj.facing_angle = angle;
    player->apiobj.velocity.x = velocity_x;
    player->apiobj.velocity.y = velocity_y;
    player->apiobj.velocity.z = velocity_z;
    player->target_velocity = player->apiobj.velocity;

    JumpTriggerPacket packet;
    packet.type = 2;
    packet.player = player;
    packet.touch_holder = touch_holder;
    packet.velocity.x = velocity_x;
    packet.velocity.y = velocity_y;
    packet.velocity.z = velocity_z;
    packet.velocity.w = 1.0f;
    *reinterpret_cast<VuVec *>(packet.field_1c) = waypoint.position;
    if (!controller->TriggerJumpTask(packet, true, true, true)) {
        player->apiobj.velocity.x = previous_x;
        player->apiobj.velocity.y = previous_y;
        player->apiobj.velocity.z = previous_z;
        player->target_velocity.x = previous_target_x;
        player->target_velocity.y = previous_target_y;
        player->target_velocity.z = previous_target_z;
    }
    return true;
}

MechTouchTaskPlannedGoTo::~MechTouchTaskPlannedGoTo() {
    delete[] path_points;
    path_points = NULL;
    delete go_to_task;
    go_to_task = NULL;
}

MechTouchTaskUseTeleport::MechTouchTaskUseTeleport(MechInputTouchGestureBasedController &owner,
                                                   MechObjectInterface *object, VuVec const &)
    : MechTouchTaskGoTo(owner, object), field_60(NULL) {
}

bool MechTouchTaskUseTeleport::Update() {
    if (target.Get() != NULL) {
        VuVec position;
        target.Get()->GetPos(position, -1);
        position.x -= player->apiobj.position.x;
        position.y -= player->apiobj.position.y;
        position.z -= player->apiobj.position.z;
        const f32 distance_squared = position.x * position.x + position.y * position.y + position.z * position.z;
        if (distance_squared < 0.25f) {
            controller->button_pressed[3] = 1;
            field_51 = 1;
        }
    }
    MechTouchTaskGoTo::Update();
    return player != NULL && player->character_context != 0x0f;
}

MechTouchTaskAstroJetPack::MechTouchTaskAstroJetPack(MechInputTouchGestureBasedController &owner)
    : MechTouchTask(owner), started(false), start_timeout(0.75f), grounded_time(0.0f) {
    flags |= 14;
}

bool MechTouchTaskAstroJetPack::Update() {
    controller->button_was_pressed[2] = 1;
    start_timeout -= FRAMETIME;
    started |= player->character_context == LEGOCONTEXT_JUMP;
    if (player->apiobj.field_0x27d != 0) {
        grounded_time += FRAMETIME;
    }
    if ((started || start_timeout < 0.0f) &&
        (player->character_context != LEGOCONTEXT_JUMP || (player->apiobj.field_0x27d != 0 && grounded_time > 0.75f))) {
        return false;
    }
    if (next != NULL) {
        --player->edge_stop_requests;
        return next->Update();
    }
    return true;
}

void MechTouchTaskPlannedDoubleClickGoTo::BackgroundProcess() {
    if (player != NULL && move_to_marker.Get() != NULL) {
        const VuVec &position = move_to_marker.Get()->position;
        const f32 dx = position.x - player->apiobj.position.x;
        const f32 dy = position.y - player->apiobj.position.y;
        const f32 dz = position.z - player->apiobj.position.z;
        if (dx * dx + dy * dy + dz * dz < 0.25f) {
            Hint_SetComplete(0x5f2);
            move_to_marker.Get()->persistent = false;
            move_to_marker.Get()->FadeOut();
            move_to_marker = NuMechPtr<MoveToMarker, 4>();
        }
    }
}

MechTouchTaskPlannedDoubleClickGoTo::MechTouchTaskPlannedDoubleClickGoTo(MechInputTouchGestureBasedController &owner,
                                                                         MechObjectInterface *object)
    : MechTouchTask(owner), target(object), move_to_marker(), target_position(), field_4c(0), finished(0), field_4e(1) {
}

void MechTouchTaskPlannedDoubleClickGoTo::OnResume() {
    if (field_4c != 0 || target.Get() == NULL || player == NULL || field_4e == 0) {
        finished = true;
        return;
    }
    field_4c = 1;

    VuVec position;
    target.Get()->GetPos(position, -1);
    const f32 dx = position.x - player->apiobj.position.x;
    const f32 dy = position.y - player->apiobj.position.y;
    const f32 dz = position.z - player->apiobj.position.z;
    const GAMECHARACTERDATA *character = player->apiobj.character_data->game_character;
    const f32 max_rise =
        -(character->jump_speed * character->jump_speed) / (character->gravity + character->gravity) * 2.5f;
    if (dy > max_rise) {
        finished = true;
        return;
    }

    f32 velocity_x;
    f32 velocity_y;
    f32 velocity_z;
    if (dx * dx + dy * dy + dz * dz > 1.96f || dy > player->apiobj.scaled_height) {
        const VuVec velocity = TouchHacks::CalculateJumpVelToHitPointDblJump(*player, position);
        velocity_x = velocity.x;
        velocity_y = velocity.y;
        velocity_z = velocity.z;
        player->jump_input_flags |= 0x10;
    } else {
        const VuVec velocity = TouchHacks::CalculateJumpVelToHitPoint(*player, position);
        velocity_x = velocity.x;
        velocity_y = velocity.y;
        velocity_z = velocity.z;
    }

    const f32 previous_z = player->apiobj.velocity.z;
    const f32 previous_target_z = player->target_velocity.z;
    const f32 previous_y = player->apiobj.velocity.y;
    const f32 previous_target_y = player->target_velocity.y;
    const f32 previous_x = player->apiobj.velocity.x;
    const f32 previous_target_x = player->target_velocity.x;
    const i32 angle = NuAtan2D(velocity_x, velocity_z);
    player->apiobj.movement_facing_angle = angle;
    player->apiobj.facing_angle = angle;
    player->apiobj.velocity.x = velocity_x;
    player->apiobj.velocity.y = velocity_y;
    player->apiobj.velocity.z = velocity_z;
    player->target_velocity = player->apiobj.velocity;

    JumpTriggerPacket packet;
    packet.type = 2;
    packet.player = player;
    packet.touch_holder = touch_holder;
    packet.velocity.x = velocity_x;
    packet.velocity.y = velocity_y;
    packet.velocity.z = velocity_z;
    packet.velocity.w = 1.0f;
    *reinterpret_cast<VuVec *>(packet.field_1c) = position;
    if (!controller->TriggerJumpTask(packet, true, true, true)) {
        player->apiobj.velocity.x = previous_x;
        player->apiobj.velocity.y = previous_y;
        player->apiobj.velocity.z = previous_z;
        player->target_velocity.x = previous_target_x;
        player->target_velocity.y = previous_target_y;
        player->target_velocity.z = previous_target_z;
    }
}

void MechTouchTaskPlannedDoubleClickGoTo::OnStart() {
    if (target.Get() == NULL || player == NULL) {
        return;
    }

    VuVec position;
    target.Get()->GetPos(position, -1);
    MoveToMarker *marker = MechSystems::Get()->FindMoveToMarkerAtPos(position, true);
    move_to_marker = NuMechPtr<MoveToMarker, 4>(marker);
    if (move_to_marker.Get() == NULL) {
        move_to_marker = NuMechPtr<MoveToMarker, 4>(MechSystems::Get()->NewMoveToMarker(*target.Get()));
    }
    marker = move_to_marker.Get();
    if (marker != NULL) {
        marker->persistent = true;
        marker->BlowUp();
    }

    position.y -= player->apiobj.position.y;
    position.x -= player->apiobj.position.x;
    position.z -= player->apiobj.position.z;
    const f32 distance_squared = position.x * position.x + position.z * position.z;
    if (distance_squared < 4.0f) {
        OnResume();
        return;
    }

    const f32 distance = NuFsqrt(distance_squared);
    const f32 offset_z = position.z / distance * 2.0f;
    const f32 offset_x = position.x / distance * 2.0f;
    target.Get()->GetPos(position, -1);
    position.x -= offset_x;
    position.z -= offset_z;
    position.y = GameShadow(player, &position.xyz, 5.0f, -1);
    target_position.position.x = position.x;
    target_position.position.y = position.y;
    target_position.position.z = position.z;
    target_position.position.w = position.w;

    MechInputTouchGestureBasedController *owner = controller;
    MechTouchTaskPlannedGoTo *task =
        new MechTouchTaskPlannedGoTo(*owner, &target_position, reinterpret_cast<bool *>(&field_4e));
    task->field_6fd = 1;
    controller->StartNewTask(task, *touch_holder, false, false);
}

void MechTouchTaskPlannedDoubleClickGoTo::OnStop() {
    if (move_to_marker.Get() != NULL) {
        move_to_marker.Get()->persistent = false;
        move_to_marker.Get()->FadeOut();
    }
}

bool MechTouchTaskPlannedDoubleClickGoTo::Update() {
    return !finished;
}

MechTouchTaskPlannedDoubleClickGoTo::~MechTouchTaskPlannedDoubleClickGoTo() {
}
