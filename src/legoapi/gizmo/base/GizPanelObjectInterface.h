#pragma once
#include "MechInputTouch/MechInputTouch_types.h"

struct VuVec;
struct GIZPANEL_s;

struct GizPanelObjectInterface : MechObjectInterface {
    GIZPANEL_s &panel;
    GizPanelObjectInterface(GIZPANEL_s &);
    void GetFloorTargetPos(VuVec &, i32) const override;
    void GetPos(VuVec &, i32) const override;
    f32 GetRadius() const override;
    const char *GetTargetName() const override;
    i32 GetObjectType() const override {
        return 10;
    }
    void *GetTgtVoidPtr() override {
        return &panel;
    }
    GIZPANEL_s *GetPanel() override {
        return &panel;
    }
    void TargetedFlash() override;
    ~GizPanelObjectInterface() override;
};
DECOMP_ASSERT(sizeof(GizPanelObjectInterface) == 0xc, "Panel object interface size");
