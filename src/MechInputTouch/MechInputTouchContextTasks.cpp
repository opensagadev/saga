#include "decomp.h"
#include "MechInputTouch_types.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "globals.h"
#include "legoapi/items/base/apiobject.h"

extern i16 id_YODA;
extern i16 id_YODAGHOST;

f32 s_mechTouchMoveToStuckVel = 0.2f;
f32 s_mechTouchMoveToStuckTime = 0.55f;

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
    : controller(&owner), elapsed(0.0f), flags(0) {
    next = NULL;
}

MechTouchTask::~MechTouchTask() {
}

MechTouchTaskTag::MechTouchTaskTag(MechInputTouchGestureBasedController &, GameObject_s &) {
    STUBBED();
}

void MechTouchTaskTag::Update() {
    STUBBED();
}

MechTouchTaskGoTo::MechTouchTaskGoTo(MechInputTouchGestureBasedController &owner, MechObjectInterface *object)
    : MechTouchTask(owner), target(object), room(-1), field_30(0), field_34(0), field_38(0), field_3c(0), field_40(0),
      field_44(0), field_4c(0), field_4d(1), field_4e(0), field_4f(0), field_50(0), field_51(0), field_54(0),
      field_58(0), field_5c(0) {
}

void MechTouchTaskGoTo::OnStart() {
    STUBBED();
}

void MechTouchTaskGoTo::OnStop() {
    STUBBED();
}

void MechTouchTaskGoTo::Render() {
    STUBBED();
}

bool MechTouchTaskGoTo::Update() {
    STUBBED();
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

void MechTouchTaskGoTo::UpdateTarget(MechObjectInterface &) {
    STUBBED();
}

MechTouchTaskGoTo::~MechTouchTaskGoTo() {
}

MechTouchTaskJump::MechTouchTaskJump(MechInputTouchGestureBasedController &, JumpTriggerPacket const &, bool, bool) {
    STUBBED();
}

void MechTouchTaskJump::OnStop() {
    STUBBED();
}

void MechTouchTaskJump::Update() {
    STUBBED();
}

MechTouchTaskBlock::MechTouchTaskBlock(MechInputTouchGestureBasedController &owner) : MechTouchTask(owner) {
    flags |= 5;
}

bool MechTouchTaskBlock::Update() {
    controller->button_was_pressed[0] = 1;
    return true;
}

MechTouchTaskPanel::MechTouchTaskPanel(MechInputTouchGestureBasedController &, MechObjectInterface *, VuVec const &) {
    STUBBED();
}

void MechTouchTaskPanel::Update() {
    STUBBED();
}

MechTouchTaskAttack::MechTouchTaskAttack(MechInputTouchGestureBasedController &, MechObjectInterface *, VuVec const &) {
    STUBBED();
}

void MechTouchTaskAttack::OnStart() {
    STUBBED();
}

void MechTouchTaskAttack::OnStop() {
    STUBBED();
}

void MechTouchTaskAttack::Render() {
    STUBBED();
}

void MechTouchTaskAttack::Update() {
    STUBBED();
}

MechTouchTaskBigJump::MechTouchTaskBigJump(MechInputTouchGestureBasedController &, MechObjectInterface &, signed char) {
    STUBBED();
}

MechTouchTaskBigJump::MechTouchTaskBigJump(MechInputTouchGestureBasedController &, nuvec_s &, signed char) {
    STUBBED();
}

void MechTouchTaskBigJump::Update() {
    STUBBED();
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
    STUBBED();
    return false;
}

MechTouchTaskUseForce::MechTouchTaskUseForce(MechInputTouchGestureBasedController &, MechObjectInterface *,
                                             VuVec const &) {
    STUBBED();
}

void MechTouchTaskUseForce::OnStart() {
    STUBBED();
}

void MechTouchTaskUseForce::OnStop() {
    STUBBED();
}

void MechTouchTaskUseForce::Update() {
    STUBBED();
}

MechTouchTaskUseZipUp::MechTouchTaskUseZipUp(MechInputTouchGestureBasedController &owner) : MechTouchTask(owner) {
}

void MechTouchTaskUseZipUp::OnStart() {
    STUBBED();
}

bool MechTouchTaskUseZipUp::Update() {
    controller->button_pressed[3] = 1;
    return false;
}

MechTouchTaskPullLever::MechTouchTaskPullLever(MechInputTouchGestureBasedController &, MechObjectInterface *,
                                               VuVec const &) {
    STUBBED();
}

void MechTouchTaskPullLever::Update() {
    STUBBED();
}

MechTouchTaskHatMachine::MechTouchTaskHatMachine(MechInputTouchGestureBasedController &, MechObjectInterface *,
                                                 VuVec const &) {
    STUBBED();
}

void MechTouchTaskHatMachine::Update() {
    STUBBED();
}

void MechTouchTaskPlannedGoTo::AnalysePath() {
    STUBBED();
}

void MechTouchTaskPlannedGoTo::BackgroundProcess() {
    STUBBED();
}

void MechTouchTaskPlannedGoTo::GenerateWaypoints() {
    STUBBED();
}

MechTouchTaskPlannedGoTo::MechTouchTaskPlannedGoTo(MechInputTouchGestureBasedController &, MechObjectInterface *,
                                                   bool *) {
    STUBBED();
}

void MechTouchTaskPlannedGoTo::OnResume() {
    STUBBED();
}

void MechTouchTaskPlannedGoTo::OnStart() {
    STUBBED();
}

void MechTouchTaskPlannedGoTo::OnStop() {
    STUBBED();
}

void MechTouchTaskPlannedGoTo::SetupForAnalysis() {
    STUBBED();
}

void MechTouchTaskPlannedGoTo::Update() {
    STUBBED();
}

MechTouchTaskPlannedGoTo::~MechTouchTaskPlannedGoTo() {
}

MechTouchTaskUseTeleport::MechTouchTaskUseTeleport(MechInputTouchGestureBasedController &, MechObjectInterface *,
                                                   VuVec const &) {
    STUBBED();
}

void MechTouchTaskUseTeleport::Update() {
    STUBBED();
}

MechTouchTaskAstroJetPack::MechTouchTaskAstroJetPack(MechInputTouchGestureBasedController &) {
    STUBBED();
}

void MechTouchTaskAstroJetPack::Update() {
    STUBBED();
}

void MechTouchTaskPlannedDoubleClickGoTo::BackgroundProcess() {
    STUBBED();
}

MechTouchTaskPlannedDoubleClickGoTo::MechTouchTaskPlannedDoubleClickGoTo(MechInputTouchGestureBasedController &owner,
                                                                         MechObjectInterface *object)
    : MechTouchTask(owner), target(object), move_to_marker(), target_position(), field_4c(0), finished(0), field_4e(1) {
}

void MechTouchTaskPlannedDoubleClickGoTo::OnResume() {
    STUBBED();
}

void MechTouchTaskPlannedDoubleClickGoTo::OnStart() {
    STUBBED();
}

void MechTouchTaskPlannedDoubleClickGoTo::OnStop() {
    STUBBED();
}

bool MechTouchTaskPlannedDoubleClickGoTo::Update() {
    return !finished;
}

MechTouchTaskPlannedDoubleClickGoTo::~MechTouchTaskPlannedDoubleClickGoTo() {
}
