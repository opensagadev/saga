#include "nu2api/nucore/android/NuInputDevice_android.h"

#include <pthread.h>
#include <math.h>
#include "globals.h"
#include "java/native_window.h"

#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/common.h"

namespace NuInputDevicePS {
    bool m_gamepadConnected;

    u32 m_deviceButtons;
    u32 m_padButtons;

    pthread_mutex_t m_touchEventQueueCriticalSection;
    struct TouchEvent {
        i32 type;
        i32 device;
        i32 touch;
        f32 x;
        f32 y;
    };
    TouchEvent m_touchEventQueue[256];
    i32 m_touchEventQueueSize;
    bool m_classInitCalled;
    NuInputTouchData m_touchDataR;
    NuInputTouchData m_touchDataW;
    bool m_touchActive[10];
    f32 m_gamePadAxis[8];
    f32 m_acceleration[3];
    f32 m_sensorPitch;
    f32 m_sensorRoll;

    u32 ClassInitPS(void) {
        memset(&m_touchDataR, 0, sizeof(m_touchDataR));
        memset(&m_touchDataW, 0, sizeof(m_touchDataW));
        memset(m_touchActive, 0, sizeof(m_touchActive));
        m_classInitCalled = true;
        for (i32 i = 0; i < 8; ++i) m_gamePadAxis[i] = 0.0f;
        return 2;
    }

    void ClassShutdownPS(void) {
    }

    void UpdateAllPS(f32 delta_time) {
        if (g_appWindow != NULL) {
            f32 width = ANativeWindow_getWidth(g_appWindow);
            f32 height = ANativeWindow_getHeight(g_appWindow);
            pthread_mutex_lock(&m_touchEventQueueCriticalSection);
            for (i32 i = 0; i != m_touchEventQueueSize; ++i) {
                const TouchEvent &event = m_touchEventQueue[i];
                switch (event.type) {
                case 0:
                    m_touchDataW.touch_events[event.device].unknown_14 = event.touch;
                    m_touchDataW.touch_events[event.device].unknown_04 = event.x / width;
                    m_touchDataW.touch_events[event.device].unknown_08 = event.y / height;
                    m_touchActive[event.device] = true;
                    break;
                case 1:
                    m_touchActive[event.device] = false;
                    break;
                case 2:
                    m_touchDataW.touch_events[event.device].unknown_04 = event.x / width;
                    m_touchDataW.touch_events[event.device].unknown_08 = event.y / height;
                    break;
                }
            }
            m_touchEventQueueSize = 0;
            pthread_mutex_unlock(&m_touchEventQueueCriticalSection);
            m_touchDataR.touch_count = 0;
            u32 count = 0;
            for (i32 i = 0; i < 10; ++i) {
                if (m_touchActive[i]) {
                    m_touchDataR.touch_events[count].unknown_04 = m_touchDataW.touch_events[i].unknown_04;
                    m_touchDataR.touch_events[count].unknown_08 = m_touchDataW.touch_events[i].unknown_08;
                    m_touchDataR.touch_events[count].unknown_14 = m_touchDataW.touch_events[i].unknown_14;
                    ++count;
                }
            }
            m_touchDataR.touch_count = count;
        }
        m_sensorPitch = atan2f(m_acceleration[0], m_acceleration[2]) * 0.31830987334251404f * -0.5f;
        m_sensorRoll = asin(m_acceleration[1]) * 0.15915493667125702;
    }

    void HandleGamepPadStatusConnect(bool is_connected) {
        m_gamepadConnected = is_connected;
    }

    bool IsConnectedPS(u32 port) {
        if (port == 0) {
            return true;
        }

        if (port == 1) {
            return m_gamepadConnected;
        }

        return false;
    }

    bool IsInterceptedPS(u32 port) {
        return false;
    }

    bool HasHeadphonesConnectedPS(u32 port) {
        return false;
    }

    NUPADTYPE GetTypePS(u32 port) {
        return port == 0 ? NUPADTYPE_TOUCH : NUPADTYPE_GAMEPAD;
    }

    NUPADATTACHMENTTYPE GetAttachmentTypePS(u32 port) {
        return NUPADATTACHMENTTYPE_NONE;
    }

    u32 GetCapsPS(u32 port) {
        return port == 0 ? 0x440 : 0x0;
    }

    f32 GetVolumePS(u32 port) {
        return 0.0f;
    }

    void SetMotorsPS(u32 port, f32 motor_1, f32 motor_2) {
    }

    void ReadButtonsPS(u32 port, u32 *states) {
        *states = 0;

        if (port == 0) {
            *states = m_deviceButtons;
        }

        if (port == 1) {
            *states = m_padButtons;
        }
    }

    void ReadAnalogValuesPS(u32 port, f32 *values) {
        if (port == 1) {
            memset(values, 0, 12 * sizeof(f32));
            values[10] = m_gamePadAxis[2];
            values[11] = m_gamePadAxis[3];
            f32 x = m_gamePadAxis[0] + m_gamePadAxis[6];
            values[8] = MAX(-1.0f, (MIN(x, 1.0f)));
            f32 y = m_gamePadAxis[1] + m_gamePadAxis[7];
            values[9] = MAX(-1.0f, (MIN(y, 1.0f)));
            values[6] = m_gamePadAxis[4];
            values[7] = m_gamePadAxis[5];
        }
    }

    void ReadMotionValuesPS(u32 port, f32 *values) {
        if (port == 1) {
            memset(values, 0, 20 * sizeof(f32));
            values[1] = m_sensorPitch;
            values[0] = m_sensorRoll;
        }
    }

    void ReadTouchDataPS(u32 port, NuInputTouchData *data) {
        if (port == 0) memcpy(data, &m_touchDataR, sizeof(*data));
    }

    void ReadMouseDataPS(u32 port, NuInputMouseData *data) {
        memset(data, 0, sizeof(*data));
    }

    u32 GetGamePadButtonIndex(i32 key, i32 *port) {
        *port = 0;
        u32 button = 0;
        switch (key) {
        case 3: case 108: *port = 1; button = 0x800; break;
        case 4: button = 0x80000000; break;
        case 19: case 20: case 21: case 22:
        case 102: case 103: case 106: case 107: *port = 1; break;
        case 96: *port = 1; button = 0x40; break;
        case 97: *port = 1; button = 0x20; break;
        case 99: *port = 1; button = 0x80; break;
        case 100: *port = 1; button = 0x10; break;
        }
        return button;
    }

    i32 HandleTouch_ANDROID_SPECIFIC(i32 type, i32 device, i32 touch, f32 x, f32 y) {
        pthread_mutex_lock(&m_touchEventQueueCriticalSection);
        TouchEvent &event = m_touchEventQueue[m_touchEventQueueSize];
        event.type = type;
        event.device = device;
        event.touch = touch;
        event.x = x;
        event.y = y;
        ++m_touchEventQueueSize;
        pthread_mutex_unlock(&m_touchEventQueueCriticalSection);
        return 0;
    }

    void HandleGamePadAxis_ANDROID_SPECIFIC(f32 x, f32 y, f32 z, f32 rz, f32 left, f32 right) {
        pthread_mutex_lock(&m_touchEventQueueCriticalSection);
        m_gamePadAxis[0] = z;
        m_gamePadAxis[1] = rz;
        m_gamePadAxis[2] = left;
        m_gamePadAxis[3] = right;
        m_gamePadAxis[6] = x;
        m_gamePadAxis[7] = y;
        pthread_mutex_unlock(&m_touchEventQueueCriticalSection);
    }

    void HandleSensor_ANDROID_SPECIFIC(i32, f32, f32, f32) {
    }

    void HandleKeyDown_ANDROID_SPECIFIC(i32 key) {
        u32 button_idx;
        i32 port;

        pthread_mutex_lock(&m_touchEventQueueCriticalSection);

        button_idx = GetGamePadButtonIndex(key, &port);

        if (port == 0) {
            m_deviceButtons |= button_idx;
        } else {
            m_padButtons |= button_idx;
        }

        pthread_mutex_unlock(&m_touchEventQueueCriticalSection);
    }

    void HandleKeyUp_ANDROID_SPECIFIC(i32 key) {
        u32 button_idx;
        i32 port;

        pthread_mutex_lock(&m_touchEventQueueCriticalSection);

        button_idx = ~GetGamePadButtonIndex(key, &port);

        if (port == 0) {
            m_deviceButtons &= button_idx;
        } else {
            m_padButtons &= button_idx;
        }

        pthread_mutex_unlock(&m_touchEventQueueCriticalSection);
    }
}; // namespace NuInputDevicePS
