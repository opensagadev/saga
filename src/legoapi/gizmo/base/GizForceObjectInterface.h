#pragma once
#include "MechInputTouch/MechInputTouch_types.h"

struct VuVec;
struct GIZFORCE_s;
struct GIZFORCESYS_s;

struct GAMEANIMOBJ_s;
struct GizForceObjectInterface : MechObjectInterface {
    GIZFORCE_s &force;
    GAMEANIMOBJ_s *selected_object;
    GizForceObjectInterface(GIZFORCE_s &);
    void GetPos(VuVec &, i32) const override;
    f32 GetRadius() const override;
    const char *GetTargetName() const override;
    void *GetTgtVoidPtr() override;
    i32 GetObjectType() const override {
        return 5;
    }
    GIZFORCE_s *GetGizForce() override {
        return &force;
    }
    void TargetedFlash() override;
    ~GizForceObjectInterface() override;
};
DECOMP_ASSERT(sizeof(GizForceObjectInterface) == 0x10, "Force object interface size");
DECOMP_ASSERT(offsetof(GizForceObjectInterface, selected_object) == 0xc, "Force selected object offset");

GIZFORCE_s *GizForce_FindByName(GIZFORCESYS_s *, char *);
GIZFORCE_s *GizForces_FindForce(WORLDINFO_s *, char *);
i32 GizForce_Complete(GIZFORCE_s *);
