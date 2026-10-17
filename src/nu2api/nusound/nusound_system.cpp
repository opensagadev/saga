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
#include "nu2api/nusound/nusound3_include.h"
#include "nu2api/nusound/nusound_routing.h"







bool NuSoundStreamDesc::DecodeStreamOnOpen() const {
    return false;
}

i32 NuSoundStreamDesc::GetLoopStart() const {
    return 0;
}

i32 NuSoundStreamDesc::GetLoopEnd() const {
    return 0;
}

