#include "decomp.h"
#include "MechInputTouch_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"

extern "C" {
    extern i16 id_BASKETCANNON;
    extern i16 id_STAP;
    extern i16 id_STAP2;
    extern i16 id_TROOPERCANNON;
    extern i16 id_CANNON;
    extern i16 id_MOSCANNON;
    extern i16 id_ATST;
    extern i16 id_BIGGUN;
}

HashedKey MechAutofireAddon::s_hashId("MechAutofireAddon");
HashedKey CantPickupBombTimerAddon::s_hashId("CantPickupBombTimerAddon");

bool CantPickupBombTimerAddon::OnProcess(MechAddon::ProcessStage, float elapsed) {
    remaining_time -= elapsed;
    return remaining_time > 0.0f;
}

bool MechAutofireAddon::OnProcess(MechAddon::ProcessStage stage, float) {
    if (character == NULL)
        return false;
    if (character->apiobj.field_0x27c != -1 && TouchHacks::TouchControlsActive && stage == PROCESS_STAGE_0) {
        MechInputTouchGestureBasedController *controller = MechSystems::Get()->gesture_controller;
        if (controller != NULL) {
            if (character->id == id_BASKETCANNON) {
                controller->button_pressed[0] = 1;
            } else if ((MechInputTouchSystem::s_actualTouchMode == 4 &&
                        controller->temporary_position.managed_links != NULL) ||
                       (MechInputTouchSystem::s_actualTouchMode == 2 && controller->field_a5 != 0) ||
                       character->character_context == 0x3a || character->character_context == 0x36 ||
                       character->character_context == 0x2a) {
                if (character->id == id_STAP || character->id == id_STAP2 || character->id == id_TROOPERCANNON ||
                    character->id == id_CANNON || character->id == id_MOSCANNON || character->id == id_ATST ||
                    character->id == id_BIGGUN) {
                    controller->button_pressed[0] = 1;
                } else {
                    controller->button_was_pressed[0] = 1;
                }
            }
        }
    }
    return true;
}

CantPickupBombTimerAddon::~CantPickupBombTimerAddon() {
}

MechAutofireAddon::~MechAutofireAddon() {
}

MechAutofireAddon::MechAutofireAddon(MechObjectInterface &object)
    : MechAddon(object, s_hashId.value), character(object.GetCharacterObject()) {
}

CantPickupBombTimerAddon::CantPickupBombTimerAddon(MechObjectInterface &object, float duration)
    : MechAddon(object, s_hashId.value), remaining_time(duration) {
    GameObject_s *character = object.GetCharacterObject();
    if (character != NULL) {
        character->apiobj.velocity.x += character->apiobj.velocity.x;
        character->apiobj.velocity.z += character->apiobj.velocity.z;
    }
}
