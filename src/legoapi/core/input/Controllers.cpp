#include "decomp.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nucore/NuInputDevice.h"

// Physical controller hooks (retail Controllers.cpp).

extern "C" {

    void Controller_Init(void) {
    }

    void Controller_Exit(void) {
    }

    void Controller_Update(void) {
    }

    i32 Controller_IsConnected(void) {
        return NuInputDevicePS::IsConnectedPS(1);
    }

    i32 Controller_Read(i32, u8 *left_x, u8 *left_y, u8 *right_x, u8 *right_y, u8 *left_trigger, u8 *right_trigger,
                        u8 *button_a, u8 *button_b, u32 *buttons, u8 *motion, u32 *status) {
        if (Controller_IsConnected() == 0) {
            *left_x = 0x80;
            *left_y = 0x80;
            *right_x = 0x80;
            *right_y = 0x80;
            *left_trigger = 0;
            *right_trigger = 0;
            *button_a = 0;
            *button_b = 0;
            *buttons = 0;
            *status = 0;
            motion[0] = 0;
            motion[1] = 0;
            motion[2] = 0;
            motion[3] = 0;
            motion[4] = 0;
            motion[5] = 0;
        }
        return 1;
    }

} // extern "C"
