#include "gamelib_util_types.h"

#include "globals.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/nufloat.h"

NUCOLOUR3 flashCol = {2.0f, 2.0f, 2.0f};
bool TouchHacks::TouchControlsActive;
extern i32 BonusArea;
extern "C" i16 id_GRABCONTROL, id_WICKET, id_EWOK;

void TouchHacks::AiPlayerTakeDamageOnKillRescue(GameObject_s &) {
}

void TouchHacks::CalculateJumpVelToHitPoint(GameObject_s &, VuVec const &) {
}

void TouchHacks::CalculateJumpVelToHitPointDblJump(GameObject_s &, VuVec const &) {
}

void TouchHacks::CalculateXZVelForArcToHitPoint(VuVec const &, VuVec const &, float, float) {
}

i32 TouchHacks::CanBlowupBeBlownUp(GIZMOBLOWUP_s &blowup, i32 hit_type) {
    if (hit_type != 1) {
        return 1;
    }
    return (blowup.draw_flags >> 7) & 1;
}

void TouchHacks::CanForceTargetObj(GameObject_s &, GameObject_s &) {
}

void TouchHacks::CanJump(GameObject_s &) {
}

void TouchHacks::CanJumpToPoint(GameObject_s &, AIPATHNODE_s const &) {
}

void TouchHacks::CanJumpToPoint(GameObject_s &, VuVec const &) {
}

void TouchHacks::CanLunge(GameObject_s &) {
}

void TouchHacks::CanPoo(GameObject_s &) {
}

void TouchHacks::CanShoot(GameObject_s &) {
}

void TouchHacks::CanSlam(GameObject_s &) {
}

void TouchHacks::CanTagTo(GameObject_s &, GameObject_s &) {
}

void TouchHacks::CanTagVehicle(GameObject_s &, GameObject_s &) {
}

void TouchHacks::CanThrowBountyBomb(GameObject_s &) {
}

void TouchHacks::CanToggleTo(GameObject_s &, i32) {
}

void TouchHacks::CanUseBuildIt(GameObject_s &) {
}

void TouchHacks::CanUseGizForce(GameObject_s &) {
}

void TouchHacks::CanUseGizForce(GameObject_s &, GIZFORCE_s &) {
}

void TouchHacks::CanUseHatMachine(GameObject_s &) {
}

void TouchHacks::CanUseLever(GameObject_s &) {
}

void TouchHacks::CanUseTeleport(GameObject_s &) {
}

void TouchHacks::CanUseVehicleSmartBomb(GameObject_s &) {
}

void TouchHacks::CanUseZipup(GameObject_s &) {
}

void TouchHacks::CheckForAboutToRunIntoKillTerrain(GameObject_s &, float) {
}

void TouchHacks::CheckForAboutToRunOffAnEdge(GameObject_s &, float) {
}

void TouchHacks::CheckJumpForLandingSpot(GameObject_s &, float) {
}

void TouchHacks::CleanupAllMechObjectInterfaces(WORLDINFO_s *) {
}

void TouchHacks::FindBombTarget(GameObject_s &) {
}

nucolour3_s *TouchHacks::GetFlashColour() {
    return &flashCol;
}

f32 TouchHacks::GetIncomingPartRange() {
    return 16.0f;
}

i32 TouchHacks::GetLoseStudsDieValue() {
    return BonusArea != 0 ? 10000 : 1000;
}

i32 TouchHacks::GetLoseStudsFallValue() {
    return 0;
}

bool TouchHacks::InParty(GameObject_s &object) {
    for (i32 index = 0; index < 8; ++index) {
        if (Player[index] == &object) {
            return true;
        }
    }
    return false;
}

void TouchHacks::PlaySmartBombBuildupEffects(GameObject_s &, float, float) {
}

void TouchHacks::ShouldAutoGrabDragBomb(GameObject_s &) {
}

bool TouchHacks::ShouldBlock(GameObject_s &object) {
    if (TouchControlsActive && object.incoming_melee != NULL && object.apiobj.field_0x27c == -1) {
        return qrand() > 14999;
    }
    return true;
}

void TouchHacks::ShouldDeflectBolt(GameObject_s &, BOLT_s &) {
}

bool TouchHacks::ShouldFlash(float timer) {
    return timer > 0.0f && NuFmod(timer, 0.3f) < 0.15f;
}

bool TouchHacks::ShouldKeepWeaponOut(GameObject_s &object) {
    return TouchControlsActive && object.id != id_GRABCONTROL &&
           (object.apiobj.flags_low & 0x80) != 0 && object.ai.opponent != NULL && object.character_context == -1;
}

bool TouchHacks::ShouldPutWeaponAway(GameObject_s &object) {
    return TouchControlsActive && object.id != id_GRABCONTROL && object.id != id_WICKET && object.id != id_EWOK &&
           (object.apiobj.flags_low & 0x80) != 0 && object.ai.opponent == NULL && object.weapon_out_timer > 5.0f &&
           object.character_context == -1 && object.field_0xe31 != 1;
}

bool TouchHacks::SolveRoot(float a, float b, float c, float &root1, float &root2) {
    if (a == 0.0f) {
        return false;
    }

    const f32 discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f) {
        return false;
    }

    const f32 square_root = NuFsqrt(discriminant);
    const f32 denominator = a + a;
    root1 = (-b - square_root) / denominator;
    root2 = (square_root - b) / denominator;
    return true;
}

void TouchHacks::TriggerVehicleSmartBomb(GameObject_s &) {
}
