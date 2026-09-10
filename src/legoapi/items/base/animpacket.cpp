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

extern "C" f32 *AnimPlaying(ANIMPACKET_s *packet, i32 animation, i32 target, i32 source) {
    if (packet == NULL || animation == -1)
        return NULL;
    if (packet->blending != 0) {
        if (target != 0 && packet->blend_animation_b == animation)
            return &packet->blend_target_time;
        if (source != 0 && packet->blend_animation_a == animation)
            return &packet->blend_source_time;
    } else {
        if (packet->animation_index == animation)
            return &packet->current_time;
    }
    return NULL;
}

extern "C" i32 AnimBlendingFromTo(CHARACTERMODEL_s *model, ANIMPACKET_s *packet, i32 source_animation,
                                  i32 target_animation) {
    if (packet->blending != 0 && source_animation != -1 && packet->blend_animation_a == source_animation &&
        target_animation != -1 && packet->blend_animation_b == target_animation) {
        if (model != NULL) {
            if (source_animation == -1 || model->model_data_b[source_animation] == NULL) {
                return 0;
            }
            if (target_animation == -1 || model->model_data_b[target_animation] == NULL) {
                return 0;
            }
        }
        return 1;
    }
    return 0;
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
