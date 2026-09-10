#include <stddef.h>

#include "MechInputTouch_types.h"

u8 MechInputTouchMainDummyButton::IsPressed() const {
    return controller->button_repeats[button_type];
}
