#include "nu2api_nusound_types.h"

u8 NuSoundMixer::sDownmixerChannelMaps[4][8] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
};

i32 NuSoundMixer::GetOutputIndex(i32 input, i32 output) {
    if (output_layout == OutputLayout::ZERO) {
        return static_cast<i32>(input_config) * output + input;
    }
    return static_cast<i32>(output_config) * input + output;
}

void NuSoundMixer::Mix(float *input, float *output) {
    const i32 input_channels = static_cast<i32>(input_config);
    const i32 output_channels = static_cast<i32>(output_config);
    const f32 *input_matrix =
        routing_table->GetMatrix(input_config, static_cast<NuSoundSystem::ChannelConfig>(8))->matrix;
    const f32 *output_matrix =
        routing_table->GetMatrix(static_cast<NuSoundSystem::ChannelConfig>(8), output_config)->matrix;
    const u8 *downmix_channels = sDownmixerChannelMaps[static_cast<u32>(downmix_type)];

    memset(output, 0, input_channels * output_channels * sizeof(f32));
    for (i32 i = 0; i < input_channels; ++i) {
        if (output_channels < input_channels && downmix_channels[i] == 0) {
            continue;
        }
        for (i32 j = 0; j < output_channels; ++j) {
            const f32 *output_row = output_matrix + j * 8;
            f32 mixed = input_matrix[i] * input[0] * output_row[0] + 0.0f +
                        input_matrix[input_channels + i] * input[1] * output_row[1] +
                        input_matrix[input_channels * 2 + i] * input[2] * output_row[2] +
                        input_matrix[input_channels * 3 + i] * input[3] * output_row[3] +
                        input_matrix[input_channels * 4 + i] * input[4] * output_row[4] +
                        input_matrix[input_channels * 5 + i] * input[5] * output_row[5] +
                        input_matrix[input_channels * 6 + i] * input[6] * output_row[6] +
                        input_matrix[input_channels * 7 + i] * input[7] * output_row[7];
            if (mixed > 1.0f) {
                mixed = 1.0f;
            } else if (mixed < 0.0f) {
                mixed = 0.0f;
            }
            output[GetOutputIndex(i, j)] = mixed;
        }
    }
}

NuSoundMixer::NuSoundMixer(NuSoundSystem::ChannelConfig config, NuSoundSystem::ChannelConfig output,
                           NuSoundMixer::OutputLayout layout, NuSoundSystem::DownmixType downmix,
                           NuSoundRoutingTable *table)
    : input_config(config), output_config(output), output_layout(layout), downmix_type(downmix), routing_table(table) {
}

NuSoundMixer::~NuSoundMixer() {
}
