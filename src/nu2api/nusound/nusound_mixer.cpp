#include "nu2api_nusound_types.h"

#include <string.h>

u8 NuSoundMixer::sDownmixerChannelMaps[4][8] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0}
};

i32 NuSoundMixer::GetOutputIndex(i32 output, i32 index) {
    if (output_layout != 0) {
        return output_config * output + index;
    }
    return input_config * index + output;
}

void NuSoundMixer::Mix(float *in, float *out) {
    f32 *input_matrix = routing_table->GetMatrix(input_config, static_cast<NuSoundSystem::ChannelConfig>(8))->matrix;
    f32 *output_matrix = routing_table->GetMatrix(static_cast<NuSoundSystem::ChannelConfig>(8), output_config)->matrix;
    const u8 *channel_map = sDownmixerChannelMaps[static_cast<u32>(downmix_type)];
    memset(out, 0, input_config * output_config * sizeof(f32));
    for (i32 input = 0; input < (i32)input_config; ++input) {
        if ((i32)output_config < (i32)input_config && channel_map[input] == 0) continue;
        for (i32 output = 0; output < (i32)output_config; ++output) {
            f32 gain = 0.0f;
            for (i32 channel = 0; channel < 8; ++channel) {
                gain += input_matrix[channel * input_config + input] * in[channel]
                      * output_matrix[output * 8 + channel];
            }
            i32 index = GetOutputIndex(input, output);
            out[index] = !(gain < 1.0f) ? 1.0f : (gain < 0.0f ? 0.0f : gain);
        }
    }
}

NuSoundMixer::NuSoundMixer(NuSoundSystem::ChannelConfig input, NuSoundSystem::ChannelConfig output,
                           NuSoundMixer::OutputLayout layout, NuSoundSystem::DownmixType downmix,
                           NuSoundRoutingTable *table)
    : input_config(input), output_config(output), output_layout(layout), downmix_type(downmix), routing_table(table) {
}

NuSoundMixer::~NuSoundMixer() {
}
