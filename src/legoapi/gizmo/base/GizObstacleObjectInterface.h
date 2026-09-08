#pragma once
#include "MechInputTouch/MechInputTouch_types.h"

struct VuVec;
struct GIZOBSTACLE_s;
struct GIZOBSTACLESYS_s;

struct GizObstacleObjectInterface : MechObjectInterface {
    GIZOBSTACLE_s &obstacle;
    GizObstacleObjectInterface(GIZOBSTACLE_s &);
    void GetPos(VuVec &, i32) const override;
    f32 GetRadius() const override;
    const char *GetTargetName() const override;
    bool IsDead() override;
    i32 GetObjectType() const override {
        return 3;
    }
    void *GetTgtVoidPtr() override {
        return &obstacle;
    }
    GIZOBSTACLE_s *GetGizObstacle() override {
        return &obstacle;
    }
    void TargetedFlash() override;
    ~GizObstacleObjectInterface() override;
};
DECOMP_ASSERT(sizeof(GizObstacleObjectInterface) == 0xc, "Obstacle object interface size");

GIZOBSTACLE_s *GizObstacle_FindByName(GIZOBSTACLESYS_s *, char *);
void GizObstacle_EvalAveragePosAndRadius(GIZOBSTACLE_s *obstacle, i32 unknown);
