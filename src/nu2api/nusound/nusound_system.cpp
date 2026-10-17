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
extern const f32 RoutingTableMonoToMono[1];
extern const f32 RoutingTableMonoToStereo[2];
extern const f32 RoutingTableMonoToQuad[4];
extern const f32 RoutingTableMonoTo51[6];
extern const f32 RoutingTableMonoTo71[8];
extern const f32 RoutingTableStereoToMono[2];
extern const f32 RoutingTableStereoToStereo[4];
extern const f32 RoutingTableStereoToQuad[8];
extern const f32 RoutingTableStereoTo51[12];
extern const f32 RoutingTableStereoTo71[16];
extern const f32 RoutingTableLRCToMono[3];
extern const f32 RoutingTableLRCToStereo[6];
extern const f32 RoutingTableLRCToQuad[12];
extern const f32 RoutingTableLRCTo51[18];
extern const f32 RoutingTableLRCTo71[24];
extern const f32 RoutingTable51ToMono[6];
extern const f32 RoutingTable51ToStereo[12];
extern const f32 RoutingTable51ToQuad[24];
extern const f32 RoutingTable51To51[36];

extern const f32 RoutingTable51To71[48];

extern const f32 RoutingTable71ToMono[8];
extern const f32 RoutingTable71ToStereo[16];
extern const f32 RoutingTable71ToQuad[32];

extern const f32 RoutingTable71To51[48];

extern const f32 RoutingTable71To71[64];


void NuSoundInitDefaultRoutingTables(void);

bool NuSoundStreamDesc::DecodeStreamOnOpen() const {
    return false;
}

i32 NuSoundStreamDesc::GetLoopStart() const {
    return 0;
}

i32 NuSoundStreamDesc::GetLoopEnd() const {
    return 0;
}

void NuSound3ExitThreads();
