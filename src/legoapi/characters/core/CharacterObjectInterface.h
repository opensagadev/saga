#pragma once
#include "MechInputTouch/MechInputTouch_types.h"

struct GameObject_s;
struct VuVec;

struct CharacterObjectInterface : MechObjectInterface {
    CharacterObjectInterface(GameObject_s &);
    f32 GetHeight() const override;
    void GetPos(VuVec &, i32) const override;
    f32 GetRadius() const override;
    const char *GetTargetName() const override;
    bool IsDead() override;
    void TargetedFlash() override;
    i32 GetObjectType() const override {
        return 2;
    }
    GameObject_s *GetCharacterObject() override {
        return object;
    }
    void *GetTgtVoidPtr() override {
        return object;
    }
    ~CharacterObjectInterface() override;
    GameObject_s *object;
};
DECOMP_ASSERT(sizeof(CharacterObjectInterface) == 0xc, "CharacterObjectInterface ABI");
