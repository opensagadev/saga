#pragma once
#include "MechInputTouch/MechInputTouch_types.h"

struct VuVec;
struct GIZTURRET_s;

struct GizTurretObjectInterface : MechObjectInterface {
    GIZTURRET_s &turret;
    GizTurretObjectInterface(GIZTURRET_s &);
    void GetPos(VuVec &, i32) const override;
    f32 GetRadius() const override;
    const char *GetTargetName() const override;
    i32 GetObjectType() const override {
        return 11;
    }
    void *GetTgtVoidPtr() override {
        return &turret;
    }
    GIZTURRET_s *GetGizTurret() override {
        return &turret;
    }
    void TargetedFlash() override;
    ~GizTurretObjectInterface() override;
};
DECOMP_ASSERT(sizeof(GizTurretObjectInterface) == 0xc, "Turret object interface size");
