// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "MechInputTouch/MechInputTouch_types.h"

#include "legoapi/gizmo/base/gizmo.h"
#include "decomp.h"
#include "globals.h"

#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/gizmo/base/GizBuildItObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizLeverObjectInterface.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizPanelObjectInterface.h"
#include "legoapi/gizmo/base/GizTurretObjectInterface.h"
#include "legoapi/gizmo/base/HatMachineObjectInterface.h"
#include "legoapi/gizmo/base/TeleportObjectInterface.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"

#include <stdio.h>
#include <string.h>
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/config/cheat.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numusic/sfx.h"

extern "C" {
    extern f32 hackFlashTimer;
    extern GAMEANIMSET_s *hackFlashingGameAnimSet;
    nuhspecial_s *hackFlashingSpecial;
}

f32 hackFlashTimer;
GAMEANIMSET_s *hackFlashingGameAnimSet;

struct nuvec_s;

// Original 0x120-byte data record @0x00666960. FixUpCharacters copies this
// canonical prefix into every character before character-specific overrides.
// These values cover the fully typed common movement and perception fields.

void MechObjectInterface::GetFloorTargetPos(VuVec &position, i32 mode) const {
    GetPos(position, mode);
}

void MechTempPosInterface::GetFloorTargetPos(VuVec &result, i32 mode) const {
    GetPos(result, mode);
}

const char *CharacterObjectInterface::GetTargetName() const {
    return "Character";
}

void CharacterObjectInterface::GetPos(VuVec &position, i32) const {
    position = VuVec(object->apiobj.collision_position.x, object->apiobj.collision_position.y,
                     object->apiobj.collision_position.z, 1.0f);
}

f32 CharacterObjectInterface::GetRadius() const {
    return object->field_0x1008;
}

f32 CharacterObjectInterface::GetHeight() const {
    return object->apiobj.scaled_height;
}

void CharacterObjectInterface::TargetedFlash() {
    object->targeted_flash = 1.0f;
}

bool CharacterObjectInterface::IsDead() {
    if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001)
        return 1;
    return object->apiobj.field_0x287 != 0;
}

const char *GizObstacleObjectInterface::GetTargetName() const {
    return obstacle.name;
}

void GizObstacleObjectInterface::GetPos(VuVec &result, i32) const {
    result = VuVec(obstacle.evaluated_position.x, obstacle.evaluated_position.y, obstacle.evaluated_position.z, 1.0f);
}

f32 GizObstacleObjectInterface::GetRadius() const {
    return obstacle.field_0x58;
}

bool GizObstacleObjectInterface::IsDead() {
    return !(obstacle.progress_flags & 1);
}

void GizObstacleObjectInterface::TargetedFlash() {
    hackFlashTimer = 1.0f;
    hackFlashingGameAnimSet = obstacle.anim_set;
}

const char *GizBlowupObjectInterface::GetTargetName() const {
    return blowup->name;
}

void GizBlowupObjectInterface::GetPos(VuVec &position, i32) const {
    if (blowup->field_0x120 != NULL) {
        NUVEC *origin = static_cast<NUVEC *>(blowup->field_0x120);
        position = VuVec(origin->x, origin->y, origin->z, 1.0f);
    } else {
        position.x = blowup->position.x;
        position.y = blowup->position.y;
        position.z = blowup->position.z;
    }
}

f32 GizBlowupObjectInterface::GetRadius() const {
    return blowup->target_scale;
}

bool GizBlowupObjectInterface::IsDead() {
    return (blowup->state_flags & 0x80) == 0;
}

void GizBlowupObjectInterface::TargetedFlash() {
    blowup->flicker_timer = 1.0f;
}

const char *GizForceObjectInterface::GetTargetName() const {
    return force.name;
}

void *GizForceObjectInterface::GetTgtVoidPtr() {
    if (selected_object != NULL)
        return selected_object;
    return &force;
}

void GizForceObjectInterface::TargetedFlash() {
    if (!(force.field_0xaa & 0x40)) {
        hackFlashTimer = 1.0f;
        if (selected_object != NULL) {
            hackFlashingSpecial = &selected_object->special;
            hackFlashingGameAnimSet = NULL;
        } else {
            hackFlashingSpecial = NULL;
            hackFlashingGameAnimSet = force.anim_set;
        }
    }
}

const char *GizBuildItObjectInterface::GetTargetName() const {
    return buildit->name;
}

void GizBuildItObjectInterface::TargetedFlash() {
    hackFlashTimer = 1.0f;
    hackFlashingGameAnimSet = buildit->anim_set;
}

const char *GizLeverObjectInterface::GetTargetName() const {
    return lever.name;
}

void GizLeverObjectInterface::GetPos(VuVec &position, i32) const {
    position = VuVec(lever.position.x, lever.position.y, lever.position.z, 1.0f);
}

f32 GizLeverObjectInterface::GetRadius() const {
    return 0.1f;
}

void GizLeverObjectInterface::TargetedFlash() {
    lever.flash_timer = 1.0f;
}

const char *TeleportObjectInterface::GetTargetName() const {
    return teleport.name;
}

f32 TeleportObjectInterface::GetRadius() const {
    return 0.1f;
}

void TeleportObjectInterface::TargetedFlash() {
    hackFlashTimer = 1.0f;
    hackFlashingSpecial = index > 0 ? &teleport.flap2_special : &teleport.flap1_special;
}

const char *HatMachineObjectInterface::GetTargetName() const {
    return machine.name;
}

void HatMachineObjectInterface::GetPos(VuVec &position, i32) const {
    position = VuVec(machine.position.x, machine.position.y, machine.position.z, 1.0f);
}

f32 HatMachineObjectInterface::GetRadius() const {
    return 0.1f;
}

void HatMachineObjectInterface::TargetedFlash() {
    machine.flash_timer = 1.0f;
}

const char *GizPanelObjectInterface::GetTargetName() const {
    return panel.name;
}

void GizPanelObjectInterface::GetFloorTargetPos(VuVec &position, i32) const {
    position = VuVec(panel.position.x, panel.position.y, panel.position.z, 1.0f);
}

f32 GizPanelObjectInterface::GetRadius() const {
    return 0.1f;
}

void GizPanelObjectInterface::TargetedFlash() {
    panel.flash_timer = 1.0f;
}

const char *GizTurretObjectInterface::GetTargetName() const {
    return turret.name;
}

void GizTurretObjectInterface::GetPos(VuVec &position, i32) const {
    position = VuVec(turret.field_0x3c.x, turret.field_0x3c.y, turret.field_0x3c.z, 1.0f);
}

f32 GizTurretObjectInterface::GetRadius() const {
    return 0.1f;
}

void GizTurretObjectInterface::TargetedFlash() {
    hackFlashTimer = 1.0f;
    hackFlashingGameAnimSet = turret.anim_set;
}

const char *PartObjectInterface::GetTargetName() const {
    return "Part";
}

// Original 0x447180..0x447fb0: this interface belongs to the shared gizmo
// interface family; PART_s owns only the lazy allocation/deletion entry points.
void PartObjectInterface::GetPos(VuVec &position, i32) const {
    position = VuVec(part.position.x, part.position.y, part.position.z, 1.0f);
}

f32 PartObjectInterface::GetRadius() const {
    return part.radius;
}

void GizPanelObjectInterface::GetPos(VuVec &position, i32) const {
    GizPanel_GetAbsTargetPos(&panel, &position.xyz, 1);
}

void GizBuildItObjectInterface::GetPos(VuVec &position, i32) const {
    f32 radius;
    CalcAveragePosAndRad(*buildit, position, radius, true);
}

f32 GizBuildItObjectInterface::GetRadius() const {
    f32 radius;
    VuVec position;
    CalcAveragePosAndRad(*buildit, position, radius, true);
    return radius;
}

f32 GizForceObjectInterface::GetRadius() const {
    if (selected_object != NULL)
        return NuSpecialGetOriginRadius(&selected_object->special);
    return force.radius;
}

void TeleportObjectInterface::GetPos(VuVec &position, i32 direction) const {
    NUVEC *point;
    if (direction == -1) {
        if (index > 0 && (teleport.flags & 1) == 0) {
            point = &teleport.path->pts[teleport.path->length - 1];
        } else {
            point = teleport.path->pts;
        }
    } else {
        point = teleport.path->pts;
    }
    position = VuVec(point->x, point->y, point->z, 1.0f);
}

void GizForceObjectInterface::GetPos(VuVec &result, i32) const {
    if (selected_object != NULL) {
        NUVEC *position = NuSpecialGetDrawPos(&selected_object->special);
        result = VuVec(position->x, position->y, position->z, 1.0f);
    } else {
        result = VuVec(force.position.x, force.position.y, force.position.z, 1.0f);
    }
}

GizPanelObjectInterface::~GizPanelObjectInterface() {
    panel.mech_object_interface = NULL;
}

GizLeverObjectInterface::~GizLeverObjectInterface() {
    lever.mech_object = NULL;
}

PartObjectInterface::~PartObjectInterface() {
    part.mech_object_interface = NULL;
}

CharacterObjectInterface::~CharacterObjectInterface() {
    object->mech_object_interface = NULL;
}

GizBuildItObjectInterface::~GizBuildItObjectInterface() {
    buildit->mech_object_interface = NULL;
}

TeleportObjectInterface::~TeleportObjectInterface() {
    teleport.mech_object_interface = NULL;
}

HatMachineObjectInterface::~HatMachineObjectInterface() {
    machine.mech_object_interface = NULL;
}

GizBlowupObjectInterface::~GizBlowupObjectInterface() {
    blowup->mech_object_interface = NULL;
}

GizForceObjectInterface::~GizForceObjectInterface() {
    force.mech_object_interface = NULL;
}

GizObstacleObjectInterface::~GizObstacleObjectInterface() {
    obstacle.mech_object_interface = NULL;
}

GizTurretObjectInterface::~GizTurretObjectInterface() {
    turret.mech_object_interface = NULL;
}

MechTempPosInterface::MechTempPosInterface(nuvec_s const &value) {
    position.xyz = value;
    radius = 0.2f;
}

MechTempPosInterface::MechTempPosInterface(VuVec const &value) {
    position.x = value.x;
    position.y = value.y;
    position.z = value.z;
    position.w = value.w;
    radius = 0.2f;
}

CharacterObjectInterface::CharacterObjectInterface(GameObject_s &value) : object(&value) {
    object->mech_object_interface = this;
}

GizObstacleObjectInterface::GizObstacleObjectInterface(GIZOBSTACLE_s &value) : obstacle(value) {
    obstacle.mech_object_interface = this;
}

GizBlowupObjectInterface::GizBlowupObjectInterface(GIZMOBLOWUP_s &object) : blowup(&object) {
    object.mech_object_interface = this;
}

GizForceObjectInterface::GizForceObjectInterface(GIZFORCE_s &value) : force(value) {
    force.mech_object_interface = this;
}

GizBuildItObjectInterface::GizBuildItObjectInterface(GIZBUILDIT_s &object) : buildit(&object) {
    object.mech_object_interface = this;
}

GizLeverObjectInterface::GizLeverObjectInterface(LEVER_s &value) : lever(value) {
    lever.mech_object = this;
}

TeleportObjectInterface::TeleportObjectInterface(TELEPORT_s &value, i32 teleport_index)
    : teleport(value), index(teleport_index) {
    value.mech_object_interface = this;
}

HatMachineObjectInterface::HatMachineObjectInterface(HATMACHINE_s &value) : machine(value) {
    machine.mech_object_interface = this;
}

GizPanelObjectInterface::GizPanelObjectInterface(GIZPANEL_s &value) : panel(value) {
    panel.mech_object_interface = this;
}

GizTurretObjectInterface::GizTurretObjectInterface(GIZTURRET_s &value) : turret(value) {
    turret.mech_object_interface = this;
}

PartObjectInterface::PartObjectInterface(PART_s &value) : part(value) {
    part.mech_object_interface = this;
}

