#include "host/platform/free_camera.hpp"

#include <atomic>
#include <cstring>

#include "batman.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edui.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nushader_plain.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"

extern f32 g_renderContext_viewProj[16];
extern f32 g_renderContext_viewProjInverse[16];
extern f32 g_renderContext_view[16];
extern f32 g_renderContext_projection[16];
extern f32 g_renderContext_position[4];
extern i32 global_frame_count;
extern i32 edlevel_mouseandkeyboard;

namespace {
    std::atomic<bool> host_free_camera_enabled{false};
    std::atomic<bool> host_free_camera_ready{false};
    std::atomic<u32> host_free_camera_controls{0};
    thread_local bool host_suppress_editor_camera_input = false;

    bool host_free_camera_initialized = false;
    u32 host_free_camera_previous_controls = 0;
    i32 host_free_camera_last_move_frame = -1;
    i32 host_free_camera_previous_pad_fly_mode = 0;
    i32 host_free_camera_previous_editor_input_mode = 0;
    bool host_free_camera_overrode_editor_input = false;

    constexpr u8 host_pad_center = 0x80;
    constexpr u8 host_pad_minimum = 0x00;
    constexpr u8 host_pad_maximum = 0xff;

    void host_free_camera_capture_editor_input() {
        if (editor_active && !host_free_camera_overrode_editor_input) {
            host_free_camera_previous_editor_input_mode = edlevel_mouseandkeyboard;
            // Mouse motion controls the editor cursor. Only the host's numpad
            // controls should move the free camera, not edcamMove(NULL).
            edlevel_mouseandkeyboard = 0;
            host_free_camera_overrode_editor_input = true;
        }
    }

    void host_free_camera_restore() {
        if (host_free_camera_initialized) {
            PadFlyMode = host_free_camera_previous_pad_fly_mode;
        }
        if (host_free_camera_overrode_editor_input) {
            edlevel_mouseandkeyboard = host_free_camera_previous_editor_input_mode;
            host_free_camera_overrode_editor_input = false;
        }
        host_free_camera_initialized = false;
        host_free_camera_previous_controls = 0;
        host_free_camera_last_move_frame = -1;
    }

    void host_free_camera_initialize(NUMTX *incoming_view) {
        edcam_s *editor_camera = edcamGetEdCam();
        NUMTX incoming_world;
        NuMtxInv(&incoming_world, incoming_view);

        editor_camera->position = GameCam->target;
        editor_camera->pitch = static_cast<i32>(GameCam->pitch) + static_cast<i32>(GameCam->field_0x214);
        editor_camera->yaw = static_cast<i32>(GameCam->yaw) + static_cast<i32>(GameCam->field_0x218);
        NUVEC incoming_position = {incoming_world.m30, incoming_world.m31, incoming_world.m32};
        editor_camera->distance = -NuVecDist(&incoming_position, &GameCam->target, NULL);
        if (editor_camera->distance > -editor_camera->minimum_distance) {
            editor_camera->distance = -editor_camera->minimum_distance;
        }
        editor_camera->offset = {0.0f, 0.0f, 0.0f};

        host_free_camera_previous_pad_fly_mode = PadFlyMode;
        PadFlyMode = 1;
        host_free_camera_capture_editor_input();
        host_free_camera_initialized = true;
        LOG_INFO("host free camera: using built-in edcam at (%.3f, %.3f, %.3f), distance=%.3f", incoming_position.x,
                 incoming_position.y, incoming_position.z, editor_camera->distance);
    }

    nupad_s host_free_camera_pad(u32 controls) {
        nupad_s pad{};
        pad.analog_left_x = host_pad_center;
        pad.analog_left_y = host_pad_center;
        pad.analog_right_x = host_pad_center;
        pad.analog_right_y = host_pad_center;

        const bool move = (controls & HOST_FREE_CAMERA_SHIFT) != 0;
        u8 &horizontal = move ? pad.analog_left_x : pad.analog_right_x;
        u8 &vertical = move ? pad.analog_left_y : pad.analog_right_y;
        if ((controls & HOST_FREE_CAMERA_NUMPAD_4) != 0) {
            horizontal = host_pad_minimum;
        } else if ((controls & HOST_FREE_CAMERA_NUMPAD_6) != 0) {
            horizontal = host_pad_maximum;
        }
        if ((controls & HOST_FREE_CAMERA_NUMPAD_8) != 0) {
            vertical = host_pad_minimum;
        } else if ((controls & HOST_FREE_CAMERA_NUMPAD_5) != 0) {
            vertical = host_pad_maximum;
        }
        return pad;
    }
} // namespace

void HostFreeCameraConfigure(bool enabled) {
    host_free_camera_enabled.store(enabled, std::memory_order_relaxed);
    host_free_camera_ready.store(false, std::memory_order_relaxed);
    host_free_camera_controls.store(0, std::memory_order_relaxed);
}

void HostFreeCameraSetReady(bool ready) {
    host_free_camera_ready.store(ready, std::memory_order_relaxed);
}

void HostFreeCameraSetControls(u32 controls) {
    host_free_camera_controls.store(controls, std::memory_order_relaxed);
}

bool HostFreeCameraActive(void) {
    return host_free_camera_enabled.load(std::memory_order_relaxed) &&
           host_free_camera_ready.load(std::memory_order_relaxed);
}

void HostFreeCameraSuppressEditorCameraInput(bool suppress) {
    host_suppress_editor_camera_input = suppress;
}

#ifdef __linux__
extern "C" void __real_edcamMove(nupad_s *pad);

extern "C" void __wrap_edcamMove(nupad_s *pad) {
    if (!host_suppress_editor_camera_input)
        __real_edcamMove(pad);
}
#endif

static void host_free_camera_apply(NUMTX *view) {
    const bool newly_initialized = !host_free_camera_initialized;
    if (newly_initialized) {
        host_free_camera_initialize(view);
    }

    const u32 controls = host_free_camera_controls.load(std::memory_order_relaxed);
    if (controls != host_free_camera_previous_controls) {
        LOG_INFO("host free camera: controls=0x%x mode=%s", controls,
                 (controls & HOST_FREE_CAMERA_SHIFT) != 0 ? "move" : "rotate");
        host_free_camera_previous_controls = controls;
    }

    if (host_free_camera_last_move_frame != global_frame_count && !(newly_initialized && editor_active) &&
        (!editor_active || eduiGetCameraEnabled())) {
        nupad_s pad = host_free_camera_pad(controls);
        const i32 cursor_enabled = edmainGetCursorEnabled();
        // The editor has already processed the mouse this frame.
        edmainSetCursorEnabled(0);
        edcamMoveEx(&pad, NuTimeGetFrameTime());
        edmainSetCursorEnabled(cursor_enabled);
        host_free_camera_last_move_frame = global_frame_count;
    }
    if (newly_initialized && editor_active)
        host_free_camera_last_move_frame = global_frame_count;
    NUMTX camera_world;
    edcamMtx(&camera_world);
    NuMtxInv(view, &camera_world);
    if (editor_active) {
        // Editor rays read these camera matrices on the next Process frame.
        if (NUCAMERA *camera = edmainGetCamera()) {
            camera->mtx = camera_world;
        }
        global_camera.mtx = camera_world;
    }

    if (WORLD != NULL) {
        WORLD->room_visibility_flag = 1;
        WORLD->rooms_visible_ptr = WORLD->rooms_visible;
        memset(WORLD->rooms_visible, 1, sizeof(WORLD->rooms_visible));
    }
}

extern "C" void NuRenderContextSetViewProj(NUMTX *view, NUMTX *projection) {
    NUMTX active_view = *view;
    if (HostFreeCameraActive() && GameCam != NULL) {
        host_free_camera_apply(&active_view);
    } else if (host_free_camera_initialized || host_free_camera_overrode_editor_input) {
        host_free_camera_restore();
    }

    NUVEC scale = {
        g_NuVpRegion.projection_x_scale,
        g_NuVpRegion.projection_y_scale,
        1.0f,
    };
    NUVEC translation = {
        g_NuVpRegion.projection_x_offset,
        g_NuVpRegion.projection_y_offset,
        0.0f,
    };
    NUMTX scale_mtx;
    NUMTX translation_mtx;
    NUMTX adjusted_projection;
    NuMtxSetScale(&scale_mtx, &scale);
    NuMtxSetTranslation(&translation_mtx, &translation);
    NuMtxMulH(&adjusted_projection, projection, &scale_mtx);
    NuMtxMulH(&adjusted_projection, &adjusted_projection, &translation_mtx);

    memcpy(g_renderContext_view, &active_view, sizeof(NUMTX));
    memcpy(g_renderContext_projection, &adjusted_projection, sizeof(NUMTX));

    NUMTX inverse_view;
    NuMtxInv(&inverse_view, &active_view);
    g_renderContext_position[0] = inverse_view.m30 / inverse_view.m33;
    g_renderContext_position[1] = inverse_view.m31 / inverse_view.m33;
    g_renderContext_position[2] = inverse_view.m32 / inverse_view.m33;
    g_renderContext_position[3] = 1.0f;

    NuMtxMulH(reinterpret_cast<NUMTX *>(g_renderContext_viewProj), &active_view, &adjusted_projection);
    NuMtxInvH(reinterpret_cast<NUMTX *>(g_renderContext_viewProjInverse),
              reinterpret_cast<NUMTX *>(g_renderContext_viewProj));

    NUMTX depth_remap = numtx_identity;
    depth_remap.m22 = 2.0f;
    depth_remap.m32 = -1.0f;
    NuMtxMulH(reinterpret_cast<NUMTX *>(g_renderContext_viewProj), reinterpret_cast<NUMTX *>(g_renderContext_viewProj),
              &depth_remap);

    NuShaderManagerSetfv(0x3d, g_renderContext_view);
    NuShaderManagerSetfv(0x3e, g_renderContext_viewProj);
    NuShaderManagerSetfv(0x56, g_renderContext_position);

    f32 fov;
    f32 aspect;
    f32 near_clip;
    f32 far_clip;
    f32 perspective[4];
    NuMtxGetPerspectiveD3D(projection, &fov, &aspect, &near_clip, &far_clip);
    perspective[0] = near_clip;
    perspective[1] = far_clip;
    perspective[2] = far_clip - near_clip;
    perspective[3] = perspective[2] / far_clip;
    NuShaderManagerSetfv(0x49, perspective);

    f32 frustum[4];
    NuMtxGetFrustumD3D(projection, &frustum[0], &frustum[1], &frustum[2], &frustum[3], &near_clip, &far_clip);
    frustum[1] -= frustum[0];
    frustum[3] -= frustum[2];
    NuShaderManagerSetfv(0x4a, frustum);
}
