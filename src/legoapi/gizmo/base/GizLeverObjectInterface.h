#pragma once
#include "MechInputTouch/MechInputTouch_types.h"

struct VuVec;
struct LEVER_s;

struct GizLeverObjectInterface : MechObjectInterface {
    LEVER_s &lever;
    GizLeverObjectInterface(LEVER_s &);
    void GetPos(VuVec &, i32) const override;
    f32 GetRadius() const override;
    const char *GetTargetName() const override;
    i32 GetObjectType() const override {
        return 7;
    }
    void *GetTgtVoidPtr() override {
        return &lever;
    }
    LEVER_s *GetGizLever() override {
        return &lever;
    }
    void TargetedFlash() override;
    ~GizLeverObjectInterface() override;
};
DECOMP_ASSERT(sizeof(GizLeverObjectInterface) == 0xc, "Lever object interface size");
