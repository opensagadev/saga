#pragma once

struct VuVec;
struct TELEPORT_s;

struct TeleportObjectInterface {
    void *field_0x4;
    TELEPORT_s &teleport;
    i32 index;

    TeleportObjectInterface(TELEPORT_s &, i32);
    void GetPos(VuVec &, i32) const;
    f32 GetRadius() const;
    const char *GetTargetName() const;
    void TargetedFlash();
    virtual ~TeleportObjectInterface();
};
