#include "decomp.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/players.h"
#include "nu2api/nu3d/nutex.h"

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

void FaceOpponent(GameObject_s *, nuvec_s *) {
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

void StartQuickShoot(GameObject_s *, i32) {
}

void CanFightLikeAJedi(GameObject_s *) {
}

void ForceNextLungeTarget(MechObjectInterface *) {
}

void ForceNextShootTarget(MechObjectInterface &) {
}

void SetForcedAttackOpponent(MechObjectInterface *) {
}

void Punch_Hit(GameObject_s *, GameObject_s *, float, float) {
}
