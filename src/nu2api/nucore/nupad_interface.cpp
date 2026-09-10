#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/NuInputManager.h"
#include "nu2api/nucore/NuTouchInputButton.h"
#include "nu2api/nucore/NuTouchInputElement.h"
#include "nu2api/nucore/NuTouchInputStick.h"
#include "nu2api/nucore/NuVirtualTouchDevice.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nutrig.h"
extern "C" i32 NuRndrBeginScene(i32 begin_flags);
extern "C" void NuRndrEndScene(void);

NuInputManager *inputManager;
NuVirtualTouchDevice *inputTouchDevice;
bool used_touch_IDs[10];

void NuPad_Interface_Render(void) {
    NuRndrBeginScene(-1);
    if (inputTouchDevice != NULL)
        inputTouchDevice->Render();
    NuRndrEndScene();
}

void NuPad_Interface_InputManagerInitialise(void) {
    NuInputDevice *touch_dev;
    u32 i;

    inputManager = new NuInputManager();
    inputManager->UpdateAll(0.0f);

    touch_dev = inputManager->GetFirstDeviceByType(NUPADTYPE_TOUCH);
    if (touch_dev != NULL) {
        inputTouchDevice = new NuVirtualTouchDevice(4);
        inputTouchDevice->CreateDefaultLayout(0);

        touch_dev->AddTranslator(inputTouchDevice);
    }

    for (i = 0; i < 10; i++) {
        used_touch_IDs[i] = false;
    }
}

void NuPad_Interface_InputManagerUpdate(f32 delta_time) {
    inputManager->UpdateAll(delta_time);
}

extern "C" void NuPad_Interface_ResetAllTouches(void) {
    NuInputDevice *device = inputManager->GetDevice(0);

    memset(device->touch_data.touch_events, 0, sizeof(device->touch_data.touch_events));
    device->touch_data.touch_count = 0;
    memset(used_touch_IDs, 0, sizeof(used_touch_IDs));
}

u32 NuPad_Interface_NuPadRead(i32 port, u8 *analog_left_x, u8 *analog_left_y, u8 *analog_right_x, u8 *analog_right_y,
                              u8 *analog_l1, u8 *analog_l2, u8 *analog_r1, u8 *analog_r2, u32 *digital_buttons,
                              u8 *unknown, i32 *analog_button_flags) {
    NuInputDevice *device;

    device = inputManager->GetDevice(port);

    if (device == NULL) {
        return 0;
    }

    *analog_left_x = (device->GetAnalogValue(NUPADANALOGVALUE_LEFT_X) + 1.0f) * 127.0f;
    *analog_left_y = (device->GetAnalogValue(NUPADANALOGVALUE_LEFT_Y) + 1.0f) * 127.0f;
    *analog_right_x = (device->GetAnalogValue(NUPADANALOGVALUE_RIGHT_X) + 1.0f) * 127.0f;
    *analog_right_y = (device->GetAnalogValue(NUPADANALOGVALUE_RIGHT_Y) + 1.0f) * 127.0f;

    *analog_l1 = device->GetAnalogValue(NUPADANALOGVALUE_L1) * 127.0f;
    *analog_l2 = device->GetAnalogValue(NUPADANALOGVALUE_L2) * 127.0f;
    *analog_r1 = device->GetAnalogValue(NUPADANALOGVALUE_R1) * 127.0f;
    *analog_r2 = device->GetAnalogValue(NUPADANALOGVALUE_R2) * 127.0f;

    *digital_buttons = device->GetButtons();

    return device->IsConnected();
}

i32 NuPad_Interface_GetMaxDevices(void) {
    return inputManager->GetMaxDevices();
}

void NuPad_UpdateTouchScreenData(void) {
    NuInputDevice *device = inputManager->GetDevice(0);
    if (device != NULL) {
        NuInputDevicePS::ReadTouchDataPS(0, &device->touch_data);
    }
}

void NuInputDevice::DisableDPD() {
    NuInputDevicePS::DisableDPDPS(port);
}

void NuInputDevice::EnableDPD() {
    NuInputDevicePS::EnableDPDPS(port);
}

u32 NuInputDevice::GetIndexByType() const {
    return idx_by_type;
}

u32 NuInputDevice::GetLastValidIndexByType() const {
    return prev_valid_idx_by_type;
}

NUPADTYPE NuInputDevice::GetLastValidType() const {
    return prev_valid_type;
}

f32 NuInputDevice::GetMotionValue(NUPADMOTIONVALUE input) const {
    if (IsConnected())
        return motion_values[input];
    return 0.0f;
}

const NuInputMouseData *NuInputDevice::GetMouseData() const {
    return &mouse_data;
}

u32 NuInputDevice::GetPort() const {
    return port;
}

const NuInputTouchData *NuInputDevice::GetTouchData() const {
    return &touch_data;
}

f32 NuInputDevice::GetVolume() const {
    return volume;
}

bool NuInputDevice::HasHeadphonesConnected() const {
    return has_headphones_connected;
}

bool NuInputDevice::IsButtonPressed(u32 buttons) const {
    return IsConnected() && (button_states & buttons) != 0;
}

bool NuInputDevice::IsIntercepted() const {
    return is_intercepted;
}

void NuInputDevice::KillRumble() {
    motor_1 = 0.0f;
    motor_2 = 0.0f;
    is_rumble_killed = true;
}

void NuInputDevice::SetMotors(float motor_1, float motor_2) {
    this->motor_1 = motor_1;
    this->motor_2 = motor_2;
}

bool NuInputDevice::SupportsCaps(u32 mask) const {
    return (GetCaps() & mask) != 0;
}

NuInputDevice::~NuInputDevice() {
}

void NuButtonLayout::ActivateLayout() {
    for (u32 i = 0; i < unknown_c8; ++i)
        elements[i]->Activate();
}

void NuButtonLayout::DeactivateLayout() {
    for (u32 i = 0; i < unknown_c8; ++i)
        elements[i]->Deactivate();
}

void NuButtonLayout::Render() {
    for (u32 i = 0; i < unknown_c8; ++i)
        elements[i]->Render();
}

void NuButtonLayout::Update(NuInputTouchData const *data) {
    for (u32 i = 0; i < unknown_c8; ++i)
        elements[i]->Update(data);
}

void NuButtonLayout::UpdateButtons(i32 index) {
    for (u32 i = 0; i < unknown_c8; ++i)
        elements[i]->UpdateButtons(index);
}

NuButtonLayout::~NuButtonLayout() {
    for (u32 i = 0; i < unknown_c8; ++i)
        delete elements[i];
}

const NuInputDevice *NuInputManager::GetDevice(u32 port) const {
    return devices[port];
}

const NuInputDevice *NuInputManager::GetFirstDeviceByType(NUPADTYPE type) const {
    for (u32 i = 0; i < max_devices; ++i) {
        if (devices[i]->GetType() == type)
            return devices[i];
    }
    return NULL;
}

void NuInputDevicePS::DisableDPDPS(u32) {
}

void NuInputDevicePS::EnableDPDPS(u32) {
}

void NuInputDevicePS::GetIdentifierPS(u32) {
}

void NuTouchInputStick::Render() {
}

void NuTouchInputStick::Update(NuInputTouchData const *data) {
    stick_x = 0.0f;
    stick_y = 0.0f;
    u32 count = data->touch_count;
    for (u32 i = 0; i < count; ++i) {
        const NuInputTouch &touch = data->touch_events[i];
        u8 active = touch.unknown_00;
        u8 released = touch.unknown_01;
        u8 started = touch.unknown_02;
        float tx = touch.unknown_04;
        float ty = touch.unknown_08;
        u32 touch_id = touch.unknown_14;
        if (!released && !started && !active)
            continue;
        float dx = tx - x;
        float dy = ty - y;
        float dx2 = dx * dx;
        float dy2 = dy * dy;
        if (!unknown_3c && started) {
            float rx = width * 0.5f;
            float ry = height * 0.5f;
            if (dx2 / (rx * rx) + dy2 / (ry * ry) <= 1.0f) {
                unknown_38 = touch_id;
                unknown_3c = true;
            }
        }
        if (released && unknown_38 == touch_id) {
            unknown_3c = false;
        } else if (unknown_3c && unknown_38 == touch_id) {
            float distance = NuFsqrt(dy2 + dx2);
            i32 angle = NuAtan2D(dx, dy);
            float magnitude = distance / height;
            float sx = NU_SIN_LUT(angle);
            float sy = NU_COS_LUT(angle);
            magnitude = magnitude < 1.0f ? (magnitude < 0.0f ? 0.0f : magnitude) : 1.0f;
            magnitude = (float)(magnitude * 1.4) * 3.0f;
            sx *= magnitude;
            sy *= magnitude;
            stick_x = sx < 1.0f ? (sx > -1.0f ? sx : -1.0f) : 1.0f;
            stick_y = sy < 1.0f ? (sy > -1.0f ? sy : -1.0f) : 1.0f;
            return;
        }
    }
    unknown_3c = false;
}

void NuTouchInputButton::Render() {
}

NuTouchInputElement::NuTouchInputElement(NuTouchInputElement::TYPE type, i32 id, u32 index) {
    this->id = id;
    this->index = index;
    this->type = type;
}

NuTouchInputElement::NuTouchInputElement(NuTouchInputElement::TYPE type, i32 id, u32 index, float x, float y,
                                         float width, float height) {
    this->id = id;
    this->index = index;
    this->type = type;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}
