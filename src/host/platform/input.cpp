#include "decomp.h"
#include "host/platform/input.hpp"

#include <atomic>
#include <cstring>

#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/nuapi.h"

namespace {
    constexpr u32 host_touch_device = 0;
    constexpr u32 host_gamepad_device = 1;
    std::atomic<u32> host_pending_buttons[2];
    std::atomic<u32> host_held_buttons[2];
    std::atomic<u32> host_keyboard_buttons[2];
    std::atomic<u32> host_android_buttons;
    std::atomic<f32> host_left_x[2];
    std::atomic<f32> host_left_y[2];
    u32 host_frame_buttons[2];

    u32 AndroidButtonForKey(i32 key) {
        switch (key) {
            case 3:
            case 108:
                return 0x800;
            case 4:
                return 0x80000000;
            case 96:
                return 0x40;
            case 97:
                return 0x20;
            case 99:
                return 0x80;
            case 100:
                return 0x10;
            default:
                return 0;
        }
    }
} // namespace

__attribute__((weak)) void HostInputResetPlatform() {
}

__attribute__((weak)) u32 HostInputConsumePlatform(i32) {
    return 0;
}

__attribute__((weak)) void HostInputTouch(i32, i32, i32, i32) {
}

void HostInputReset() {
    host_android_buttons.store(0, std::memory_order_relaxed);
    for (i32 port = 0; port < 2; ++port) {
        host_pending_buttons[port].store(0, std::memory_order_relaxed);
        host_held_buttons[port].store(0, std::memory_order_relaxed);
        host_keyboard_buttons[port].store(0, std::memory_order_relaxed);
        host_left_x[port].store(0.0f, std::memory_order_relaxed);
        host_left_y[port].store(0.0f, std::memory_order_relaxed);
        host_frame_buttons[port] = 0;
    }
    HostInputResetPlatform();
}

void HostInputSetHeld(i32 port, u32 buttons) {
    if (port < 0 || port >= 2) {
        return;
    }

    host_held_buttons[port].store(buttons, std::memory_order_release);
}

void HostInputSetKeyboardHeld(i32 port, u32 buttons) {
    if (port < 0 || port >= 2) {
        return;
    }

    host_keyboard_buttons[port].store(buttons, std::memory_order_release);
}

void HostInputSetAnalog(i32 port, f32 left_x, f32 left_y) {
    if (port < 0 || port >= 2) {
        return;
    }

    if (left_x < -1.0f)
        left_x = -1.0f;
    else if (left_x > 1.0f)
        left_x = 1.0f;
    if (left_y < -1.0f)
        left_y = -1.0f;
    else if (left_y > 1.0f)
        left_y = 1.0f;
    host_left_x[port].store(left_x, std::memory_order_release);
    host_left_y[port].store(left_y, std::memory_order_release);
}

void HostInputTap(i32 port, u32 buttons) {
    if (port < 0 || port >= 2) {
        return;
    }

    host_pending_buttons[port].fetch_or(buttons, std::memory_order_release);
}

namespace NuInputDevicePS {

    void HandleGamepPadStatusConnect(bool) {
        // The host always exposes its keyboard-backed gamepad.
    }

    void HandleKeyDown_ANDROID_SPECIFIC(i32 key) {
        host_android_buttons.fetch_or(AndroidButtonForKey(key), std::memory_order_release);
    }

    void HandleKeyUp_ANDROID_SPECIFIC(i32 key) {
        host_android_buttons.fetch_and(~AndroidButtonForKey(key), std::memory_order_release);
    }

    void HandleSensor_ANDROID_SPECIFIC(i32, f32, f32, f32) {
    }

    i32 HandleTouch_ANDROID_SPECIFIC(i32 type, i32, i32, f32 x, f32 y) {
        if (type == 0) {
            HostInputTouch(static_cast<i32>(x), static_cast<i32>(y), nuapi.screen_width, nuapi.screen_height);
        }
        return 0;
    }

    void HandleGamePadAxis_ANDROID_SPECIFIC(f32 x, f32 y, f32, f32, f32, f32) {
        HostInputSetAnalog(0, x, y);
    }

    u32 ClassInitPS() {
        HostInputReset();
        // Android exposes a built-in touch device at index 0 and the external
        // gamepad at index 1. Game code also queries index 1 directly when it
        // decides whether to render controller-oriented menu feedback.
        return 2;
    }

    void ClassShutdownPS() {
        HostInputReset();
    }

    void UpdateAllPS(f32) {
        for (i32 port = 0; port < 2; ++port) {
            const u32 tapped = host_pending_buttons[port].exchange(0, std::memory_order_acq_rel);
            const u32 held = host_held_buttons[port].load(std::memory_order_acquire);
            const u32 keyboard = host_keyboard_buttons[port].load(std::memory_order_acquire);
            const u32 platform = HostInputConsumePlatform(port);
            host_frame_buttons[port] = tapped | held | keyboard | platform |
                                       (port == 0 ? host_android_buttons.load(std::memory_order_acquire) : 0);
        }
    }

    bool IsConnectedPS(u32 device) {
        // The host harness currently supplies keyboard-backed gamepad input,
        // but no touch events.  Advertising the placeholder touch device as
        // connected makes NuPad initially map player 0 to that silent device;
        // the first gamepad press is then spent remapping to device 1 instead
        // of reaching the menu.
        return device == host_gamepad_device;
    }

    bool IsInterceptedPS(u32) {
        return false;
    }

    bool HasHeadphonesConnectedPS(u32) {
        return false;
    }

    void EnableDPDPS(u32) {
    }

    void DisableDPDPS(u32) {
    }

    NUPADTYPE GetTypePS(u32 device) {
        return device == host_touch_device ? NUPADTYPE_TOUCH : NUPADTYPE_GAMEPAD;
    }

    NUPADATTACHMENTTYPE GetAttachmentTypePS(u32) {
        return NUPADATTACHMENTTYPE_NONE;
    }

    u32 GetCapsPS(u32 device) {
        return device == host_touch_device ? 0x440 : 0;
    }

    f32 GetVolumePS(u32) {
        return 0.0f;
    }

    void SetMotorsPS(u32, f32, f32) {
    }

    void ReadButtonsPS(u32 device, u32 *states) {
        // Harness port 0 is the first player-facing gamepad. The platform
        // device index remains 1, matching the Android device topology.
        *states = device == host_gamepad_device ? host_frame_buttons[0] : 0;
    }

    void ReadAnalogValuesPS(u32 device, f32 *values) {
        memset(values, 0, sizeof(f32) * 12);
        if (device == host_gamepad_device) {
            values[NUPADANALOGVALUE_LEFT_X] = host_left_x[0].load(std::memory_order_acquire);
            values[NUPADANALOGVALUE_LEFT_Y] = host_left_y[0].load(std::memory_order_acquire);
        }
    }

    void ReadMotionValuesPS(u32, f32 *values) {
        memset(values, 0, sizeof(f32) * 20);
    }

    void ReadTouchDataPS(u32, NuInputTouchData *data) {
        memset(data, 0, sizeof(*data));
    }

    void ReadMouseDataPS(u32, NuInputMouseData *data) {
        memset(data, 0, sizeof(*data));
    }

} // namespace NuInputDevicePS
