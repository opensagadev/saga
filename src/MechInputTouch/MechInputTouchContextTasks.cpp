#include "MechInputTouch_types.h"

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
}

void MechTouchTaskTag::Update() {
}

MechTouchTaskGoTo::MechTouchTaskGoTo(MechInputTouchGestureBasedController &owner, MechObjectInterface *object)
    : MechTouchTask(owner), target(object), room(-1), field_30(0), field_34(0), field_38(0), field_3c(0), field_40(0),
      field_44(0), field_4c(0), field_4d(1), field_4e(0), field_4f(0), field_50(0), field_51(0), field_54(0),
      field_58(0), field_5c(0) {
}

void MechTouchTaskGoTo::OnStart() {
}

void MechTouchTaskGoTo::OnStop() {
}

void MechTouchTaskGoTo::Render() {
}

void MechTouchTaskGoTo::Update() {
}

void MechTouchTaskGoTo::UpdateStuck() {
}

void MechTouchTaskGoTo::UpdateTarget(MechObjectInterface &) {
}

MechTouchTaskGoTo::~MechTouchTaskGoTo() {
}

MechTouchTaskJump::MechTouchTaskJump(MechInputTouchGestureBasedController &, JumpTriggerPacket const &, bool, bool) {
}

void MechTouchTaskJump::OnStop() {
}

void MechTouchTaskJump::Update() {
}

MechTouchTaskBlock::MechTouchTaskBlock(MechInputTouchGestureBasedController &) {
}

void MechTouchTaskBlock::Update() {
}

MechTouchTaskPanel::MechTouchTaskPanel(MechInputTouchGestureBasedController &, MechObjectInterface *, VuVec const &) {
}

void MechTouchTaskPanel::Update() {
}

MechTouchTaskAttack::MechTouchTaskAttack(MechInputTouchGestureBasedController &, MechObjectInterface *, VuVec const &) {
}

void MechTouchTaskAttack::OnStart() {
}

void MechTouchTaskAttack::OnStop() {
}

void MechTouchTaskAttack::Render() {
}

void MechTouchTaskAttack::Update() {
}

MechTouchTaskBigJump::MechTouchTaskBigJump(MechInputTouchGestureBasedController &, MechObjectInterface &, signed char) {
}

MechTouchTaskBigJump::MechTouchTaskBigJump(MechInputTouchGestureBasedController &, nuvec_s &, signed char) {
}

void MechTouchTaskBigJump::Update() {
}

void ForceBuildItToUseNext(GIZBUILDIT_s &);

MechTouchTaskBuildIt::MechTouchTaskBuildIt(MechInputTouchGestureBasedController &owner, MechObjectInterface *object,
                                           VuVec const &)
    : MechTouchTaskGoTo(owner, object) {
    if (object->GetGizBuildit() != NULL) {
        ForceBuildItToUseNext(*object->GetGizBuildit());
    }
    flags |= 1;
}

void MechTouchTaskBuildIt::Update() {
}

MechTouchTaskUseForce::MechTouchTaskUseForce(MechInputTouchGestureBasedController &, MechObjectInterface *,
                                             VuVec const &) {
}

void MechTouchTaskUseForce::OnStart() {
}

void MechTouchTaskUseForce::OnStop() {
}

void MechTouchTaskUseForce::Update() {
}

MechTouchTaskUseZipUp::MechTouchTaskUseZipUp(MechInputTouchGestureBasedController &) {
}

void MechTouchTaskUseZipUp::OnStart() {
}

void MechTouchTaskUseZipUp::Update() {
}

MechTouchTaskPullLever::MechTouchTaskPullLever(MechInputTouchGestureBasedController &, MechObjectInterface *,
                                               VuVec const &) {
}

void MechTouchTaskPullLever::Update() {
}

MechTouchTaskHatMachine::MechTouchTaskHatMachine(MechInputTouchGestureBasedController &, MechObjectInterface *,
                                                 VuVec const &) {
}

void MechTouchTaskHatMachine::Update() {
}

void MechTouchTaskPlannedGoTo::AnalysePath() {
}

void MechTouchTaskPlannedGoTo::BackgroundProcess() {
}

void MechTouchTaskPlannedGoTo::GenerateWaypoints() {
}

MechTouchTaskPlannedGoTo::MechTouchTaskPlannedGoTo(MechInputTouchGestureBasedController &, MechObjectInterface *,
                                                   bool *) {
}

void MechTouchTaskPlannedGoTo::OnResume() {
}

void MechTouchTaskPlannedGoTo::OnStart() {
}

void MechTouchTaskPlannedGoTo::OnStop() {
}

void MechTouchTaskPlannedGoTo::SetupForAnalysis() {
}

void MechTouchTaskPlannedGoTo::Update() {
}

MechTouchTaskPlannedGoTo::~MechTouchTaskPlannedGoTo() {
}

MechTouchTaskUseTeleport::MechTouchTaskUseTeleport(MechInputTouchGestureBasedController &, MechObjectInterface *,
                                                   VuVec const &) {
}

void MechTouchTaskUseTeleport::Update() {
}

MechTouchTaskAstroJetPack::MechTouchTaskAstroJetPack(MechInputTouchGestureBasedController &) {
}

void MechTouchTaskAstroJetPack::Update() {
}

void MechTouchTaskPlannedDoubleClickGoTo::BackgroundProcess() {
}

MechTouchTaskPlannedDoubleClickGoTo::MechTouchTaskPlannedDoubleClickGoTo(MechInputTouchGestureBasedController &,
                                                                         MechObjectInterface *) {
}

void MechTouchTaskPlannedDoubleClickGoTo::OnResume() {
}

void MechTouchTaskPlannedDoubleClickGoTo::OnStart() {
}

void MechTouchTaskPlannedDoubleClickGoTo::OnStop() {
}

void MechTouchTaskPlannedDoubleClickGoTo::Update() {
}

MechTouchTaskPlannedDoubleClickGoTo::~MechTouchTaskPlannedDoubleClickGoTo() {
}
