#include "nu2api/nusound/nusound_system.hpp"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numemory.h"
#include "globals.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nusound/nusound_bus.hpp"
#include "nu2api/nusound/nusound_android.hpp"
#include "nu2api/nusound/nusound_decoder.hpp"
#include "nu2api/nusound/nusound_decoder_ogg.hpp"
#include "nu2api/nusound/nusound_streamer.hpp"
#include "nu2api/nusound/nusound_voice.hpp"
#include "decomp.h"
#include <float.h>
#include <cstdio>
#include <cstring>
#include <new>
extern const f32 RoutingTableMonoToMono[1] = {1.0f};
extern const f32 RoutingTableMonoToStereo[2] = {0.70794576f, 0.70794576f};
extern const f32 RoutingTableMonoToQuad[4] = {0.7f, 0.7f, 0.5f, 0.5f};
extern const f32 RoutingTableMonoTo51[6] = {0.5f, 0.5f, 0.7f, 0.3f, 0.5f, 0.5f};
extern const f32 RoutingTableMonoTo71[8] = {0.5f, 0.5f, 0.7f, 0.3f, 0.5f, 0.5f, 0.3f, 0.3f};
extern const f32 RoutingTableStereoToMono[2] = {0.70794576f, 0.70794576f};
extern const f32 RoutingTableStereoToStereo[4] = {1.0f, 0.0f, 0.0f, 1.0f};
extern const f32 RoutingTableStereoToQuad[8] = {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f};
extern const f32 RoutingTableStereoTo51[12] = {0.5f, 0.0f, 0.0f, 0.5f, 0.7f, 0.7f, 0.3f, 0.3f, 0.5f, 0.0f, 0.0f, 0.5f};
extern const f32 RoutingTableStereoTo71[16] = {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                               0.7f, 0.0f, 0.0f, 0.7f, 0.3f, 0.0f, 0.0f, 0.3f};
extern const f32 RoutingTableLRCToMono[3] = {0.50119f, 0.50119f, 0.50119f};
extern const f32 RoutingTableLRCToStereo[6] = {1.0f, 0.0f, 0.68f, 0.0f, 1.0f, 0.68f};
extern const f32 RoutingTableLRCToQuad[12] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
extern const f32 RoutingTableLRCTo51[18] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                                            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
extern const f32 RoutingTableLRCTo71[24] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                                            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
extern const f32 RoutingTable51ToMono[6] = {0.50119f, 0.50119f, 0.50119f, 0.50119f, 0.50119f, 0.50119f};
extern const f32 RoutingTable51ToStereo[12] = {1.0f, 0.0f, 0.68f, 0.2f, 0.2f, 0.0f,
                                               0.0f, 1.0f, 0.68f, 0.2f, 0.0f, 0.2f};
extern const f32 RoutingTable51ToQuad[24] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                             0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
extern const f32 RoutingTable51To51[36] = {
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};

extern const f32 RoutingTable51To71[48] = {
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.3f,
};

extern const f32 RoutingTable71ToMono[8] = {0.50119f, 0.50119f, 0.50119f, 0.50119f, 0.50119f, 0.50119f, 0.0f, 0.0f};
extern const f32 RoutingTable71ToStereo[16] = {1.0f, 0.0f, 0.68f, 0.3f, 0.3f, 0.0f, 0.3f, 0.0f,
                                               0.0f, 1.0f, 0.68f, 0.3f, 0.0f, 0.3f, 0.0f, 0.3f};
extern const f32 RoutingTable71ToQuad[32] = {
    1.0f, 0.0f, 0.5f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.6f, 0.0f, 0.4f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.6f, 0.0f, 0.4f,
};

extern const f32 RoutingTable71To51[48] = {
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.6f, 0.0f, 0.4f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.6f, 0.0f, 0.4f,
};

extern const f32 RoutingTable71To71[64] = {
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};

static NuSoundMixMatrix *CreateRoutingMatrix(u32 from, u32 to, const f32 *values) {
    NuSoundMixMatrix *matrix =
        static_cast<NuSoundMixMatrix *>(NU_ALLOC(sizeof(NuSoundMixMatrix), 4, 1, "", NUMEMORY_CATEGORY_NUSOUND));
    if (matrix != NULL) {
        new (matrix) NuSoundMixMatrix(static_cast<NuSoundSystem::ChannelConfig>(from),
                                      static_cast<NuSoundSystem::ChannelConfig>(to), values);
    }
    return matrix;
}

void NuSoundInitDefaultRoutingTables(void) {
    NuSoundRoutingTable *table =
        static_cast<NuSoundRoutingTable *>(NU_ALLOC(sizeof(NuSoundRoutingTable), 4, 1, "", NUMEMORY_CATEGORY_NUSOUND));
    if (table != NULL) {
        new (table) NuSoundRoutingTable("default");
    }

    const NuSoundSystem::ChannelConfig mono = static_cast<NuSoundSystem::ChannelConfig>(1);
    const NuSoundSystem::ChannelConfig stereo = static_cast<NuSoundSystem::ChannelConfig>(2);
    const NuSoundSystem::ChannelConfig lrc = static_cast<NuSoundSystem::ChannelConfig>(3);
    const NuSoundSystem::ChannelConfig quad = static_cast<NuSoundSystem::ChannelConfig>(4);
    const NuSoundSystem::ChannelConfig surround51 = static_cast<NuSoundSystem::ChannelConfig>(6);
    const NuSoundSystem::ChannelConfig surround71 = static_cast<NuSoundSystem::ChannelConfig>(8);

    table->SetMatrix(mono, mono, CreateRoutingMatrix(1, 1, RoutingTableMonoToMono));
    table->SetMatrix(mono, stereo, CreateRoutingMatrix(1, 2, RoutingTableMonoToStereo));
    table->SetMatrix(mono, quad, CreateRoutingMatrix(1, 4, RoutingTableMonoToQuad));
    table->SetMatrix(mono, surround51, CreateRoutingMatrix(1, 6, RoutingTableMonoTo51));
    table->SetMatrix(mono, surround71, CreateRoutingMatrix(1, 8, RoutingTableMonoTo71));

    table->SetMatrix(stereo, mono, CreateRoutingMatrix(2, 1, RoutingTableStereoToMono));
    table->SetMatrix(stereo, stereo, CreateRoutingMatrix(2, 2, RoutingTableStereoToStereo));
    table->SetMatrix(stereo, quad, CreateRoutingMatrix(2, 4, RoutingTableStereoToQuad));
    table->SetMatrix(stereo, surround51, CreateRoutingMatrix(2, 6, RoutingTableStereoTo51));
    table->SetMatrix(stereo, surround71, CreateRoutingMatrix(2, 8, RoutingTableStereoTo71));

    table->SetMatrix(lrc, mono, CreateRoutingMatrix(3, 1, RoutingTableLRCToMono));
    table->SetMatrix(lrc, stereo, CreateRoutingMatrix(3, 2, RoutingTableLRCToStereo));
    table->SetMatrix(lrc, quad, CreateRoutingMatrix(3, 4, RoutingTableLRCToQuad));
    table->SetMatrix(lrc, surround51, CreateRoutingMatrix(3, 6, RoutingTableLRCTo51));
    table->SetMatrix(lrc, surround71, CreateRoutingMatrix(3, 8, RoutingTableLRCTo71));

    table->SetMatrix(surround51, mono, CreateRoutingMatrix(6, 1, RoutingTable51ToMono));
    table->SetMatrix(surround51, stereo, CreateRoutingMatrix(6, 2, RoutingTable51ToStereo));
    table->SetMatrix(surround51, quad, CreateRoutingMatrix(6, 4, RoutingTable51ToQuad));
    table->SetMatrix(surround51, surround51, CreateRoutingMatrix(6, 6, RoutingTable51To51));
    table->SetMatrix(surround51, surround71, CreateRoutingMatrix(6, 8, RoutingTable51To71));

    table->SetMatrix(surround71, mono, CreateRoutingMatrix(8, 1, RoutingTable71ToMono));
    table->SetMatrix(surround71, stereo, CreateRoutingMatrix(8, 2, RoutingTable71ToStereo));
    table->SetMatrix(surround71, quad, CreateRoutingMatrix(8, 4, RoutingTable71ToQuad));
    table->SetMatrix(surround71, surround51, CreateRoutingMatrix(8, 6, RoutingTable71To51));
    table->SetMatrix(surround71, surround71, CreateRoutingMatrix(8, 8, RoutingTable71To71));

    NuSound.AddRoutingTable(table);
    NuSound.SetDefaultRoutingTable(table);
}

bool NuSoundStreamDesc::DecodeStreamOnOpen() const {
    return false;
}

i32 NuSoundStreamDesc::GetLoopStart() const {
    return 0;
}

i32 NuSoundStreamDesc::GetLoopEnd() const {
    return 0;
}

void NuSound3ExitThreads() {
}
