#include "nu2api_nusound_types.h"

#include <string.h>

NuSoundSystem::ChannelConfig NuSoundRoutingTable::GetConfig(i32 index) {
    static const u32 configs[5] = {2, 3, 4, 6, 8};
    NuSoundSystem::ChannelConfig config = static_cast<NuSoundSystem::ChannelConfig>(1);
    if (static_cast<u32>(index - 1) < 5) {
        config = static_cast<NuSoundSystem::ChannelConfig>(configs[index - 1]);
    }
    return config;
}

i32 NuSoundRoutingTable::GetIndex(NuSoundSystem::ChannelConfig config) {
    static const i32 indices[7] = {1, 2, 3, 0, 4, 0, 5};
    u32 channels = static_cast<u32>(config);
    if (static_cast<u32>(channels - 2) < 7) {
        return indices[channels - 2];
    }
    return 0;
}

NuSoundMixMatrix *NuSoundRoutingTable::GetMatrix(NuSoundSystem::ChannelConfig from,
                                                 NuSoundSystem::ChannelConfig to) const {
    return matrices[GetIndex(from) * 6 + GetIndex(to)];
}

const char *NuSoundRoutingTable::GetName() const {
    return name;
}

NuSoundRoutingTable::NuSoundRoutingTable(char const *table_name) {
    intrusive_prev = NULL;
    intrusive_next = NULL;
    name_capacity = sizeof(name_storage);
    name_length = 1;
    name = name_storage;
    name_storage[0] = '\0';

    name_length = static_cast<u16>(strlen(table_name) + 1);
    memcpy(name, table_name, name_length);
    memset(matrices, 0, sizeof(matrices));
}

NuSoundRoutingTable::NuSoundRoutingTable(char const *table_name, NuSoundRoutingTable const *parent) {
    intrusive_prev = NULL;
    intrusive_next = NULL;
    name_capacity = sizeof(name_storage);
    name_length = 1;
    name = name_storage;
    name_storage[0] = '\0';

    name_length = static_cast<u16>(strlen(table_name) + 1);
    memcpy(name, table_name, name_length);
    memset(matrices, 0, sizeof(matrices));

    for (i32 from = 0; from < 6; ++from) {
        for (i32 to = 0; to < 6; ++to) {
            NuSoundSystem::ChannelConfig from_config = GetConfig(from);
            NuSoundSystem::ChannelConfig to_config = GetConfig(to);
            SetMatrix(from_config, to_config, parent->GetMatrix(from_config, to_config));
        }
    }
}

void NuSoundRoutingTable::SetMatrix(NuSoundSystem::ChannelConfig from, NuSoundSystem::ChannelConfig to,
                                    NuSoundMixMatrix *matrix) {
    matrices[GetIndex(from) * 6 + GetIndex(to)] = matrix;
}
