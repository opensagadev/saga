#include "gamelib_util_types.h"

#include "globals.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/render/light/shadow.h"

f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);

NUCOLOUR3 flashCol = {2.0f, 2.0f, 2.0f};
bool TouchHacks::TouchControlsActive;
extern i32 BonusArea;
extern "C" i16 id_GRABCONTROL, id_WICKET, id_EWOK;

bool TouchHacks::AiPlayerTakeDamageOnKillRescue(GameObject_s &) {
    return TouchControlsActive;
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

void Move_DEFAULT(GameObject_s *);

bool TouchHacks::CanToggleTo(GameObject_s &object, i32 id) {
    if (object.id == id)
        return false;
    if ((CInfo[object.character_context].flags & 0x100) != 0)
        return false;
    if (object.apiobj.field_0x27f <= 16 && (TerLayer[static_cast<i8>(object.apiobj.field_0x27f)].flags & 1) != 0 &&
        GCDataList[id].field_0x28 <= 0.0f)
        return false;
    if (object.apiobj.field_0x218 != 2000000.0f && object.apiobj.field_0x220 != 2000000.0f &&
        object.apiobj.character_data->move_fn != Move_DEFAULT &&
        CDataList[id].bounds_max_y - CDataList[id].bounds_min_y >=
            object.apiobj.field_0x220 - object.apiobj.field_0x218)
        return false;
    return true;
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

bool TouchHacks::CanUseZipup(GameObject_s &object) {
    extern i32 ObjLandReady(GameObject_s *);
    extern i32 SuperWeirdo(GameObject_s *);
    extern i32 Cheat_IsOn(i32);
    if (object.apiobj.character_data == NULL || !ObjLandReady(&object))
        return false;
    if ((object.apiobj.character_data->model_flags & 0x100000) != 0 || SuperWeirdo(&object))
        return true;
    return (object.apiobj.character_data->model_flags & 8) != 0 &&
           (object.apiobj.character_data->game_character->flags_094[1] & 0x80) == 0 && Cheat_IsOn(13) != 0;
}

bool TouchHacks::CheckForAboutToRunIntoKillTerrain(GameObject_s &object, float time) {
    if (WORLD->current_level != SPEEDERCHASEA_LDATA) {
        const f32 dx = object.apiobj.velocity.x * time;
        const f32 dz = time * object.apiobj.velocity.z;
        VuVec position(object.apiobj.position.x, object.apiobj.position.y, object.apiobj.position.z, 1.0f);
        position.x = dx + position.x;
        position.z = dz + position.z;
        position.y += 0.3f;
        if (GameShadow(&object, reinterpret_cast<NUVEC *>(&position), 5.0f, -1) == 2000000.0f)
            return false;
        u32 layer = EShadowInfo();
        if (layer > 16 || (TerLayer[layer].flags & 1) == 0)
            return false;
        VuVec direction(object.apiobj.velocity.x, 0.0f, object.apiobj.velocity.z, 1.0f);
        NuVecNorm(reinterpret_cast<NUVEC *>(&direction), reinterpret_cast<NUVEC *>(&direction));
        const f32 radius = object.apiobj.collision_radius * 0.8f;
        position.x += direction.x * radius;
        position.z += radius * direction.z;
        if (GameShadow(&object, reinterpret_cast<NUVEC *>(&position), 5.0f, -1) == 0.0f)
            return true;
        layer = EShadowInfo();
        return layer > 16 || (TerLayer[layer].flags & 1) != 0;
    }
    return false;
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

CABLE_s *GameObjIsCableTied(GameObject_s *);
extern "C" i16 id_ATST, id_ATST_LOWRES;
i32 TouchHacks::ShouldDeflectBolt(GameObject_s &object, BOLT_s &bolt) {
    if (!TouchControlsActive || VehicleArea == 0)
        return 0;
    if (object.id != id_ATST && object.id != id_ATST_LOWRES)
        return 0;
    if (bolt.owner == NULL || (bolt.owner->apiobj.flags_low & 0x80) == 0)
        return 0;
    CABLE_s *cable = GameObjIsCableTied(&object);
    if (cable == NULL)
        return 0;
    return cable->source == bolt.owner;
}

bool TouchHacks::ShouldFlash(float timer) {
    return timer > 0.0f && NuFmod(timer, 0.3f) < 0.15f;
}

bool TouchHacks::ShouldKeepWeaponOut(GameObject_s &object) {
    return TouchControlsActive && object.id != id_GRABCONTROL && (object.apiobj.flags_low & 0x80) != 0 &&
           object.ai.opponent != NULL && object.character_context == -1;
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
