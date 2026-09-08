#include "MechInputTouch_types.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/motion.h"
#include "legoapi/items/base/apiobject.h"

HashedKey MechEdgeStopAddon::s_hashId("MechEdgeStopAddon");

// Original constructor: 179 bytes, 0x500980.
MechEdgeStopAddon::MechEdgeStopAddon(MechObjectInterface &object)
    : MechAddon(object, s_hashId.value), character(object.GetCharacterObject()), stop_timer(0.0f), was_jumping(0) {
}

// Original: 262 bytes, 0x5007a0.
bool MechEdgeStopAddon::OnProcess(MechAddon::ProcessStage stage, float delta_time) {
    if (character == NULL)
        return false;
    if (character->apiobj.field_0x27c == 0 && TouchHacks::TouchControlsActive && stage == PROCESS_STAGE_0) {
        stop_timer -= delta_time;
        const bool jumping = character->character_context == LEGOCONTEXT_JUMP;
        bool stopped = false;
        if (was_jumping) {
            if (!jumping || character->apiobj.field_0x218 > jump_start_height + 0.15f) {
                stop_timer = 0.5f;
                stopped = true;
            }
        } else if (jumping) {
            jump_start_height = character->apiobj.field_0x218;
            if (character->apiobj.field_0x218 > jump_start_height + 0.15f) {
                stop_timer = 0.5f;
                stopped = true;
            }
        }
        was_jumping = jumping;
        if (stopped || stop_timer >= 0.0f)
            ++character->edge_stop_requests;
    }
    return true;
}

MechEdgeStopAddon::~MechEdgeStopAddon() {
}
