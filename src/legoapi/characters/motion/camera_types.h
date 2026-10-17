#pragma once

// Types shared by legoapi/characters/motion/camera.cpp and legoapi/render/core/gameliball.cpp.

// Replaces VIEWCAM_s's five unresolved words without changing its target ABI.
struct VIEWCAM_s {
    i32 mode;
    NUVEC target;
    i16 pitch;         // 0x10
    i16 yaw;           // 0x12
    f32 distance;      // 0x14
    f32 target_height; // 0x18
    f32 zoom_scale;    // 0x1c
    f32 field_0x20;
    GAMEPAD_s *gamepad; // 0x24
};
// Camera-only views of the context-specific data read by PlayerCamPos.
struct CAMERA_CONTEXT_PATH_s {
    u8 reserved_00[0x40];
    nugspline_s *path;
    u8 reserved_44[8];
    u8 flags;
};
struct CAMERA_CONTEXT_FOCUS_s {
    u8 reserved_00[0x28];
    NUVEC focus;
};
