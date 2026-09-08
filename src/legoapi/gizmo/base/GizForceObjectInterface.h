#pragma once
#include "MechInputTouch/MechInputTouch_types.h"

struct VuVec;
struct GIZFORCE_s;
struct GIZFORCESYS_s;

struct GAMEANIMOBJ_s;
struct GizForceObjectInterface : MechObjectInterface {
    GizForceObjectInterface(GIZFORCE_s &);
    void GetPos(VuVec &, i32) const override;
    f32 GetRadius() const override;
    const char *GetTargetName() const override;
    i32 GetObjectType() const override {
        return 5;
    }
    void *GetTgtVoidPtr() override;
    GIZFORCE_s *GetGizForce() override {
        return force;
    }
    void TargetedFlash() override;
    virtual ~GizForceObjectInterface();
    GIZFORCE_s *force;
    GAMEANIMOBJ_s *animation_object;
};
DECOMP_ASSERT(sizeof(GizForceObjectInterface) == 16, "GizForceObjectInterface ABI");

GIZFORCE_s *GizForce_FindByName(GIZFORCESYS_s *, char *);
GIZFORCE_s *GizForces_FindForce(WORLDINFO_s *, char *);
i32 GizForce_Complete(GIZFORCE_s *);
