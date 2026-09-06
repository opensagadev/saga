#pragma once
#include "MechInputTouch/MechInputTouch_types.h"

struct VuVec;
struct GIZBUILDIT_s;

struct GizBuildItObjectInterface : MechObjectInterface {
    GizBuildItObjectInterface(GIZBUILDIT_s &);
    void GetPos(VuVec &, i32) const override;
    f32 GetRadius() const override;
    const char *GetTargetName() const override;
    i32 GetObjectType() const override { return 6; }
    void TargetedFlash() override;
    void *GetTgtVoidPtr() override { return buildit; }
    GIZBUILDIT_s *GetGizBuildit() override { return buildit; }
    virtual ~GizBuildItObjectInterface();
    GIZBUILDIT_s *buildit;
};
DECOMP_ASSERT(sizeof(GizBuildItObjectInterface) == 12, "GizBuildItObjectInterface ABI");
