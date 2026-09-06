#include "legoapi/items/base/animpacket.h"

extern "C" void ResetMiniAnimPacket(MINIANIMPACKET_s *packet, i32 animation) {
    if (packet != NULL) {
        packet->requested_animation_id = animation;
        packet->previous_animation_id = packet->requested_animation_id;
        packet->current_animation_id = packet->previous_animation_id;
        packet->target_time = 1.0f;
        packet->previous_time = packet->target_time;
        packet->current_time = packet->previous_time;
        packet->field_0x19 = 0;
        packet->reset_state = 4;
    }
}
