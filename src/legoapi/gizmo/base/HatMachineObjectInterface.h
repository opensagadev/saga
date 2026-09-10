#pragma once
#include "MechInputTouch/MechInputTouch_types.h"

struct VuVec;
struct HATMACHINE_s;

struct HatMachineObjectInterface : MechObjectInterface {
    HATMACHINE_s &machine;
    HatMachineObjectInterface(HATMACHINE_s &);
    void GetPos(VuVec &, i32) const override;
    f32 GetRadius() const override;
    const char *GetTargetName() const override;
    i32 GetObjectType() const override {
        return 9;
    }
    void *GetTgtVoidPtr() override {
        return &machine;
    }
    HATMACHINE_s *GetHatMachine() override {
        return &machine;
    }
    void TargetedFlash() override;
    ~HatMachineObjectInterface() override;
};
DECOMP_ASSERT(sizeof(HatMachineObjectInterface) == 0xc, "Hat-machine object interface size");
