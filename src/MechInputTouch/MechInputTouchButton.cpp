#include "decomp.h"
#include <stddef.h>

#include "MechInputTouch_types.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nu3d/nurndr.h"

extern u32 colourPurple;
u32 colourWhite = 0xffffffff;
u32 colourBlack = 0xff000000;
u32 colourRed = 0xff0000ff;
struct numtl_s *g_nuMtlHandleNull;

extern "C" void NuRndrRect(f32, f32, f32, f32, f32, f32, f32, f32, f32, i32, struct numtl_s *);

void MechInputTouchButton::ClearTouchLocked(bool force) {
    if (!force) {
        MechSystems::Get()->input_touch_system.SetTouchLockedBy(touch_locked_by, NULL, false);
    }
    touch_locked_by = 0xff;
}

bool MechInputTouchButton::CouldTouchBeLockedBy(u32 touch_id) {
    return MechSystems::Get()->input_touch_system.CouldTouchBeLockedBy(touch_id, this);
}

i32 MechInputTouchButton::FindPossibleTriggeringIndexFromID(u32 touch_id) {
    if (possible_triggering_touch_ids[0] == touch_id)
        return 0;
    if (possible_triggering_touch_ids[1] == touch_id)
        return 1;
    if (possible_triggering_touch_ids[2] == touch_id)
        return 2;
    if (possible_triggering_touch_ids[3] == touch_id)
        return 3;
    if (possible_triggering_touch_ids[4] == touch_id)
        return 4;
    if (possible_triggering_touch_ids[5] == touch_id)
        return 5;
    if (possible_triggering_touch_ids[6] == touch_id)
        return 6;
    if (possible_triggering_touch_ids[7] == touch_id)
        return 7;
    if (possible_triggering_touch_ids[8] == touch_id)
        return 8;
    return possible_triggering_touch_ids[9] == touch_id ? 9 : -1;
}

MechInputTouchButton::MechInputTouchButton(NuTouchInputElement::TYPE type, i32 index, i32 id)
    : NuTouchInputElement(type, index, 0) {
    touch_locked_by = 0xff;
    has_pending_touches = false;
    button_id = id;
    possible_triggering_touch_ids[0] = 0xff;
    possible_triggering_touches[0] = false;
    possible_triggering_touch_ids[1] = 0xff;
    possible_triggering_touches[1] = false;
    possible_triggering_touch_ids[2] = 0xff;
    possible_triggering_touches[2] = false;
    possible_triggering_touch_ids[3] = 0xff;
    possible_triggering_touches[3] = false;
    possible_triggering_touch_ids[4] = 0xff;
    possible_triggering_touches[4] = false;
    possible_triggering_touch_ids[5] = 0xff;
    possible_triggering_touches[5] = false;
    possible_triggering_touch_ids[6] = 0xff;
    possible_triggering_touches[6] = false;
    possible_triggering_touch_ids[7] = 0xff;
    possible_triggering_touches[7] = false;
    possible_triggering_touch_ids[8] = 0xff;
    possible_triggering_touches[8] = false;
    possible_triggering_touch_ids[9] = 0xff;
    possible_triggering_touches[9] = false;
}

MechInputTouchButton::MechInputTouchButton(i32 index, u32 id, float x, float y, float width, float height, i32 button)
    : NuTouchInputElement(NuTouchInputElement::TYPE_BUTTON, index, id, x, y, width, height) {
    touch_locked_by = 0xff;
    has_pending_touches = false;
    button_id = button;
    possible_triggering_touch_ids[0] = 0xff;
    possible_triggering_touches[0] = false;
    possible_triggering_touch_ids[1] = 0xff;
    possible_triggering_touches[1] = false;
    possible_triggering_touch_ids[2] = 0xff;
    possible_triggering_touches[2] = false;
    possible_triggering_touch_ids[3] = 0xff;
    possible_triggering_touches[3] = false;
    possible_triggering_touch_ids[4] = 0xff;
    possible_triggering_touches[4] = false;
    possible_triggering_touch_ids[5] = 0xff;
    possible_triggering_touches[5] = false;
    possible_triggering_touch_ids[6] = 0xff;
    possible_triggering_touches[6] = false;
    possible_triggering_touch_ids[7] = 0xff;
    possible_triggering_touches[7] = false;
    possible_triggering_touch_ids[8] = 0xff;
    possible_triggering_touches[8] = false;
    possible_triggering_touch_ids[9] = 0xff;
    possible_triggering_touches[9] = false;
}

__attribute__((weak)) void MechInputTouchButton::Render() {
}

__attribute__((weak)) void MechInputTouchButton::Update(NuInputTouchData const *) {
}

__attribute__((weak)) char const *MechInputTouchButton::GetName() {
    return "UNKNOWN";
}

__attribute__((weak)) char const *MechInputTouchButton::GetDebugText() {
    return "";
}

void MechInputTouchButton::Reset() {
    if (has_pending_touches) {
        possible_triggering_touch_ids[0] = 0xff;
        possible_triggering_touches[0] = false;
        possible_triggering_touch_ids[1] = 0xff;
        possible_triggering_touches[1] = false;
        possible_triggering_touch_ids[2] = 0xff;
        possible_triggering_touches[2] = false;
        possible_triggering_touch_ids[3] = 0xff;
        possible_triggering_touches[3] = false;
        possible_triggering_touch_ids[4] = 0xff;
        possible_triggering_touches[4] = false;
        possible_triggering_touch_ids[5] = 0xff;
        possible_triggering_touches[5] = false;
        possible_triggering_touch_ids[6] = 0xff;
        possible_triggering_touches[6] = false;
        possible_triggering_touch_ids[7] = 0xff;
        possible_triggering_touches[7] = false;
        possible_triggering_touch_ids[8] = 0xff;
        possible_triggering_touches[8] = false;
        possible_triggering_touch_ids[9] = 0xff;
        possible_triggering_touches[9] = false;
        has_pending_touches = false;
    }

    if (touch_locked_by != 0xff) {
        ClearTouchLocked(false);
    }
}

void MechInputTouchButton::SetTouchLocked(u32 touch_id, bool allow_new) {
    touch_locked_by = touch_id;
    MechSystems::Get()->input_touch_system.SetTouchLockedBy(touch_id, this, allow_new);
}

MechInputTouchButtonFaker::MechInputTouchButtonFaker(i32 index, u32 id, float x, float y, float width, float height)
    : MechInputTouchButton(index, id, x, y, width, height, 0) {
    is_pressed = false;
}

void MechInputTouchButtonFaker::Render() {
    if (is_pressed) {
        if (index == 0x800) {
            goto special_button;
        }
        NuRndrCircle(x + width * 0.5f, y + height * 0.5f, width, width / height, 0x40,
                     0.0f, 0.0f, 0.0f, 0.0f, colourWhite, g_nuMtlHandleNull);
        NuRndrCircle(x + width * 0.5f, y + height * 0.5f, width * 0.8f, width / height, 0x40,
                     0.0f, 0.0f, 0.0f, 0.0f, colourWhite, g_nuMtlHandleNull);
        return;
    }
    if (index == 0x800) {
        goto special_button;
    }
    NuRndrCircle(x + width * 0.5f, y + height * 0.5f, width * 0.8f, width / height, 0x40,
                 0.0f, 0.0f, 0.0f, 0.0f, id, g_nuMtlHandleNull);
    return;

special_button:
    NuRndrCircle(x + width * 0.5f, y + height * 0.5f, width * 0.9f, width / height, 0x40,
                 0.0f, 0.0f, 0.0f, 0.0f, colourWhite, g_nuMtlHandleNull);
    NuRndrCircle(x + width * 0.5f, y + height * 0.5f, width * 0.7f, width / height, 0x40,
                 0.0f, 0.0f, 0.0f, 0.0f, is_pressed ? colourRed : colourBlack, g_nuMtlHandleNull);
    NuRndrRect(x + width * 0.3f, y + height * 0.25f, 0.0f, width * 0.15f, height * 0.5f,
               0.0f, 0.0f, 0.0f, 0.0f, colourWhite, g_nuMtlHandleNull);
    NuRndrRect(x + width * 0.6f, y + height * 0.25f, 0.0f, width * 0.15f, height * 0.5f,
               0.0f, 0.0f, 0.0f, 0.0f, colourWhite, g_nuMtlHandleNull);
}

void MechInputTouchButtonFaker::Update(NuInputTouchData const *data) {
    is_pressed = false;
    bool found_locked_touch = false;
    u32 count = data->touch_count;
    if (count != 0) {
        u8 const *cursor = reinterpret_cast<u8 const *>(data);
        for (u32 i = 0; i != count; ++i, cursor += sizeof(NuInputTouch)) {
            NuInputTouch const &touch = *reinterpret_cast<NuInputTouch const *>(cursor + 4);
            u32 id = touch.unknown_14;
            if (!CouldTouchBeLockedBy(id)) {
                continue;
            }
            f32 touch_x = touch.unknown_04;
            f32 touch_y = touch.unknown_08;
            if (touch.unknown_02 != 0 && touch_locked_by == 0xff) {
                if (touch_x >= x && touch_y >= y && touch_x <= x + width && touch_y <= y + height) {
                    SetTouchLocked(id, false);
                }
            }
            if (id == touch_locked_by) {
                is_pressed = true;
                found_locked_touch = true;
            }
        }
    }
    if (touch_locked_by != 0xff && !found_locked_touch) {
        ClearTouchLocked(false);
    }
}

MechInputTouchMainDummyStick::MechInputTouchMainDummyStick(MechInputTouchMainController &main_controller,
                                                           NuTouchInputElement::TYPE type)
    : NuTouchInputElement(type, colourPurple, 0), controller(&main_controller) {
}

MechInputTouchMainDummyButton::MechInputTouchMainDummyButton(MechInputTouchMainController &main_controller, u32 id,
                                                             MechInputTouchMainController::eButtonTypes type)
    : NuTouchInputElement(TYPE_BUTTON, colourPurple, id, 0.0f, 0.0f, 0.0f, 0.0f), controller(&main_controller),
      button_type(static_cast<u32>(type)) {
}

MechInputTouchButtonControlled::MechInputTouchButtonControlled(MechInputTouchMainController &, i32 index)
    : MechInputTouchButton(NuTouchInputElement::TYPE(), 2, 0), controller_index(index) {
}

__attribute__((weak)) bool MechInputTouchButtonControlled::ControlledUpdate(NuInputTouchData const *) {
    return has_pending_touches;
}

__attribute__((weak)) void MechInputTouchButtonControlled::ControlledRender() {
}

__attribute__((weak)) void MechInputTouchButtonControlled::Reset() {
    ControlledReset();
}

__attribute__((weak)) void MechInputTouchButtonControlled::ControlledReset() {
    MechInputTouchButton::Reset();
}
