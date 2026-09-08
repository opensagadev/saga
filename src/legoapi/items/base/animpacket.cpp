#include "legoapi/items/base/animpacket.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/gameanim.h"

extern "C" f32 AnimStopFrame(CHARACTERMODEL_s *model, i32 animation) {
    return animation != -1 && model->model_data_b[animation] != NULL
               ? static_cast<CHARACTERANIM_s *>(model->model_data_a[animation])->stop_frame
               : 0.0f;
}

extern "C" f32 AnimSpeed(CHARACTERMODEL_s *model, i32 animation) {
    return animation != -1 && model->model_data_b[animation] != NULL
               ? static_cast<CHARACTERANIM_s *>(model->model_data_a[animation])->action_speed
               : 0.0f;
}

extern "C" void ResetMiniAnimPacket(MINIANIMPACKET_s *packet, i32 animation) {
    if (packet != NULL) {
        packet->requested_animation_id = animation;
        packet->previous_animation_id = packet->requested_animation_id;
        packet->current_animation_id = packet->previous_animation_id;
        packet->previous_time = 1.0f;
        packet->blend_target_time = packet->previous_time;
        packet->current_time = packet->blend_target_time;
        packet->blending = 0;
        packet->flags = ANIMPACKET_FLAG_ANIMATION_CHANGED;
    }
}
