#include "nu2api_nusound_types.h"
#include <string.h>

extern "C" const f32 RoutingTableMonoToMono[1] = {1.0f};
extern "C" const f32 RoutingTableMonoToStereo[2] = {0.7079457640647888f, 0.7079457640647888f};
extern "C" const f32 RoutingTableMonoToQuad[4] = {0.699999988079071f, 0.699999988079071f, 0.5f, 0.5f};
extern "C" const f32 RoutingTableMonoTo51[6] = {0.5f, 0.5f, 0.699999988079071f, 0.30000001192092896f, 0.5f, 0.5f};
extern "C" const f32 RoutingTableMonoTo71[8] = {0.5f, 0.5f, 0.699999988079071f, 0.30000001192092896f, 0.5f, 0.5f, 0.30000001192092896f, 0.30000001192092896f};
extern "C" const f32 RoutingTableStereoToMono[2] = {0.7079457640647888f, 0.7079457640647888f};
extern "C" const f32 RoutingTableStereoToStereo[4] = {1.0f, 0.0f, 0.0f, 1.0f};
extern "C" const f32 RoutingTableStereoToQuad[8] = {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f};
extern "C" const f32 RoutingTableStereoTo51[12] = {0.5f, 0.0f, 0.0f, 0.5f, 0.699999988079071f, 0.699999988079071f, 0.30000001192092896f, 0.30000001192092896f, 0.5f, 0.0f, 0.0f, 0.5f};
extern "C" const f32 RoutingTableStereoTo71[16] = {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.699999988079071f, 0.0f, 0.0f, 0.699999988079071f, 0.30000001192092896f, 0.0f, 0.0f, 0.30000001192092896f};
extern "C" const f32 RoutingTableLRCToMono[3] = {0.5011900067329407f, 0.5011900067329407f, 0.5011900067329407f};
extern "C" const f32 RoutingTableLRCToStereo[6] = {1.0f, 0.0f, 0.6800000071525574f, 0.0f, 1.0f, 0.6800000071525574f};
extern "C" const f32 RoutingTableLRCToQuad[12] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
extern "C" const f32 RoutingTableLRCTo51[18] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
extern "C" const f32 RoutingTableLRCTo71[24] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
extern "C" const f32 RoutingTable51ToMono[6] = {0.5011900067329407f, 0.5011900067329407f, 0.5011900067329407f, 0.5011900067329407f, 0.5011900067329407f, 0.5011900067329407f};
extern "C" const f32 RoutingTable51ToStereo[12] = {1.0f, 0.0f, 0.6800000071525574f, 0.20000000298023224f, 0.20000000298023224f, 0.0f, 0.0f, 1.0f, 0.6800000071525574f, 0.20000000298023224f, 0.0f, 0.20000000298023224f};
extern "C" const f32 RoutingTable51ToQuad[24] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
extern "C" const f32 RoutingTable51To51[36] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
extern "C" const f32 RoutingTable51To71[48] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.30000001192092896f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.30000001192092896f};
extern "C" const f32 RoutingTable71ToMono[8] = {0.5011900067329407f, 0.5011900067329407f, 0.5011900067329407f, 0.5011900067329407f, 0.5011900067329407f, 0.5011900067329407f, 0.0f, 0.0f};
extern "C" const f32 RoutingTable71ToStereo[16] = {1.0f, 0.0f, 0.6800000071525574f, 0.30000001192092896f, 0.30000001192092896f, 0.0f, 0.30000001192092896f, 0.0f, 0.0f, 1.0f, 0.6800000071525574f, 0.30000001192092896f, 0.0f, 0.30000001192092896f, 0.0f, 0.30000001192092896f};
extern "C" const f32 RoutingTable71ToQuad[32] = {1.0f, 0.0f, 0.5f, 0.30000001192092896f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.30000001192092896f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.6000000238418579f, 0.0f, 0.4000000059604645f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.6000000238418579f, 0.0f, 0.4000000059604645f};
extern "C" const f32 RoutingTable71To51[48] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.6000000238418579f, 0.0f, 0.4000000059604645f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.6000000238418579f, 0.0f, 0.4000000059604645f};
extern "C" const f32 RoutingTable71To71[64] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

NuSoundSystem::ChannelConfig NuSoundRoutingTable::GetConfig(i32 index) {
    switch (index) {
    case 1: return static_cast<NuSoundSystem::ChannelConfig>(2);
    case 2: return static_cast<NuSoundSystem::ChannelConfig>(3);
    case 3: return static_cast<NuSoundSystem::ChannelConfig>(4);
    case 4: return static_cast<NuSoundSystem::ChannelConfig>(6);
    case 5: return static_cast<NuSoundSystem::ChannelConfig>(8);
    default: return static_cast<NuSoundSystem::ChannelConfig>(1);
    }
}

i32 NuSoundRoutingTable::GetIndex(NuSoundSystem::ChannelConfig config) {
    switch (config) {
    case 2: return 1;
    case 3: return 2;
    case 4: return 3;
    case 6: return 4;
    case 8: return 5;
    default: return 0;
    }
}

NuSoundMixMatrix *NuSoundRoutingTable::GetMatrix(NuSoundSystem::ChannelConfig from, NuSoundSystem::ChannelConfig to) const {
    return matrices[GetIndex(from)][GetIndex(to)];
}

const char *NuSoundRoutingTable::GetName() const {
    return name_data;
}

NuSoundRoutingTable::NuSoundRoutingTable(char const *name) {
    unknown_04 = 0;
    name_data = name_storage;
    unknown_00 = 0;
    name_capacity = 32;
    name_length = 1;
    name_storage[0] = 0;
    u16 length = strlen(name) + 1;
    memcpy(name_data, name, length);
    name_length = length;
    memset(matrices, 0, sizeof(matrices));
}

NuSoundRoutingTable::NuSoundRoutingTable(char const *name, NuSoundRoutingTable const *parent) {
    unknown_04 = 0;
    name_data = name_storage;
    unknown_00 = 0;
    name_capacity = 32;
    name_length = 1;
    name_storage[0] = 0;
    u16 length = strlen(name) + 1;
    memcpy(name_data, name, length);
    name_length = length;
    memset(matrices, 0, sizeof(matrices));
    for (i32 input = 0; input < 6; ++input) {
        for (i32 output = 0; output < 6; ++output) {
            NuSoundSystem::ChannelConfig from = GetConfig(input);
            NuSoundSystem::ChannelConfig to = GetConfig(output);
            SetMatrix(from, to, parent->GetMatrix(from, to));
        }
    }
}

void NuSoundRoutingTable::SetMatrix(NuSoundSystem::ChannelConfig from, NuSoundSystem::ChannelConfig to, NuSoundMixMatrix *matrix) {
    matrices[GetIndex(from)][GetIndex(to)] = matrix;
}
