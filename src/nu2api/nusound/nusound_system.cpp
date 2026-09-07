#include "decomp_assert.h"
#include "nu2api/nusound/nusound_system.hpp"
#include "nu2api/nusound/nusound_android.hpp"

#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nusound/nusound_bus.hpp"
#include "nu2api/nusound/nusound_decoder.hpp"
#include "nu2api/nusound/nusound_decoder_ogg.hpp"
#include "nu2api/nusound/nusound_streamer.hpp"
#include "nu2api/nusound/nusound_voice.hpp"

#include "decomp.h"
#include "globals.h"
#include "nu2api/nucore/numemory.h"

#include <cstdio>
#include <cfloat>
#include <cstring>
#include <new>

DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(NuSoundEffectAttenuation) == 0x48, "attenuation effect ABI");
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(NuSoundEffectPitch) == 0x44, "pitch effect ABI");
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(NuSoundEffectRandomVolume) == 0x44, "random volume effect ABI");
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(NuSoundEffectRandomPitch) == 0x44, "random pitch effect ABI");
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(NuSoundEffectRepeat) == 0x54, "repeat effect ABI");
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(NuSoundEffectDoppler) == 0x50, "Doppler effect ABI");
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(NuSoundEffectFader) == 0x6c, "fader effect ABI");
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(NuSoundEffectPitchRamp) == 0x54, "pitch ramp effect ABI");

// Sentinels contain only the intrusive links, with the same adjusted object
// pointers used by the original NuEList<NuSoundVoice>.
static NuSoundSystem::VoiceLinks *VoiceLinksFor(NuSoundVoice *voice) {
    return voice != NULL ? reinterpret_cast<NuSoundSystem::VoiceLinks *>(reinterpret_cast<char *>(voice) +
                                                                         offsetof(NuSoundVoice, field_0x24))
                         : NULL;
}

DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(NuSoundVoice, field_0x24) == 0x24,
              "voice intrusive links must retain their Android offset");
#if defined(__arm__)
DECOMP_ASSERT(offsetof(NuSoundSystem, voice_head) == 0x78, "ARM voice list head offset");
DECOMP_ASSERT(offsetof(NuSoundSystem, effect_head) == 0x94, "ARM effect list head offset");
#else
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(NuSoundSystem, voice_head) == 0x74,
              "voice list head must retain its Android offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(NuSoundSystem, effect_head) == 0x90,
              "effect list head must retain its Android offset");
#endif
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(NuSoundEffect, system_owned) == 0x24,
              "effect ownership flag must retain its Android offset");

NuSoundBus *NuSoundSystem::sMasterBus = NULL;

template <typename T> void NuSoundMemory::PushNuListNode(NuList<T> &list, const T &value) {
    NuMemoryManager *previous = NuMemoryGet()->SetThreadMem(NuSoundSystem::sScratchMemMgr);
    NuListNode<T> *node =
        static_cast<NuListNode<T> *>(NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuListNode<T>), 4, 1, "", 0));
    if (node != NULL)
        new (node) NuListNode<T>(NULL, NULL, value);
    list.Append(node);
    NuMemoryGet()->SetThreadMem(previous);
    NuSoundSystem::sAllocdMemory[0] += sizeof(NuListNode<T>);
}

template void NuSoundMemory::PushNuListNode(NuList<NuSoundEffect *> &, NuSoundEffect *const &);
i32 NuSoundSystem::sAllocdMemory[3] = {0};
i32 NuSoundSystem::sTotalMemory[3] = {0};
u32 NuSoundSystem::sGfxMemorySize = 0;
void *NuSoundSystem::sScratchMemory = NULL;
void *NuSoundSystem::sSampleMemory = NULL;
void *NuSoundSystem::sDecoderMemory = NULL;
NuSoundMemoryManager *NuSoundSystem::s_mmSample = NULL;
NuSoundMemoryManager *NuSoundSystem::s_mmDecoder = NULL;
decltype(NuSoundSystem::g_handler) NuSoundSystem::g_handler = {};
const char *NuSoundSystem::sFileExtensions[12] = {"wav", "adp", "ima", "caf", "xma", "ogg",
                                                  "dsp", "msf", "vag", "gcm", "wua", "cbx"};
NuSoundSystem *NuSoundSystem::s_staticInstance = NULL;
NuSoundRoutingTable *NuSoundSystem::sDefaultRoutingTable = NULL;
i32 NuSoundSystem::sNumAvailableOutputDevices = 0;
i32 NuSoundSystem::sOutputConfig = 0;

NuMemoryManager *NuSoundSystem::sScratchMemMgr = NULL;

extern "C" const f32 RoutingTableMonoToMono[1];
extern "C" const f32 RoutingTableMonoToStereo[2];
extern "C" const f32 RoutingTableMonoToQuad[4];
extern "C" const f32 RoutingTableMonoTo51[6];
extern "C" const f32 RoutingTableMonoTo71[8];
extern "C" const f32 RoutingTableStereoToMono[2];
extern "C" const f32 RoutingTableStereoToStereo[4];
extern "C" const f32 RoutingTableStereoToQuad[8];
extern "C" const f32 RoutingTableStereoTo51[12];
extern "C" const f32 RoutingTableStereoTo71[16];
extern "C" const f32 RoutingTableLRCToMono[3];
extern "C" const f32 RoutingTableLRCToStereo[6];
extern "C" const f32 RoutingTableLRCToQuad[12];
extern "C" const f32 RoutingTableLRCTo51[18];
extern "C" const f32 RoutingTableLRCTo71[24];
extern "C" const f32 RoutingTable51ToMono[6];
extern "C" const f32 RoutingTable51ToStereo[12];
extern "C" const f32 RoutingTable51ToQuad[24];
extern "C" const f32 RoutingTable51To51[36];
extern "C" const f32 RoutingTable51To71[48];
extern "C" const f32 RoutingTable71ToMono[8];
extern "C" const f32 RoutingTable71ToStereo[16];
extern "C" const f32 RoutingTable71ToQuad[32];
extern "C" const f32 RoutingTable71To51[48];
extern "C" const f32 RoutingTable71To71[64];

void NuSoundInitDefaultRoutingTables(void) {
    NuSoundRoutingTable *table = static_cast<NuSoundRoutingTable *>(
        NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundRoutingTable), 4, 1, "", 7));
    if (table != NULL)
        new (table) NuSoundRoutingTable("default");
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 1;
            matrix->out = 1;
            matrix->matrix = const_cast<f32 *>(RoutingTableMonoToMono);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(1), static_cast<NuSoundSystem::ChannelConfig>(1),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 1;
            matrix->out = 2;
            matrix->matrix = const_cast<f32 *>(RoutingTableMonoToStereo);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(1), static_cast<NuSoundSystem::ChannelConfig>(2),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 1;
            matrix->out = 4;
            matrix->matrix = const_cast<f32 *>(RoutingTableMonoToQuad);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(1), static_cast<NuSoundSystem::ChannelConfig>(4),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 1;
            matrix->out = 6;
            matrix->matrix = const_cast<f32 *>(RoutingTableMonoTo51);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(1), static_cast<NuSoundSystem::ChannelConfig>(6),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 1;
            matrix->out = 8;
            matrix->matrix = const_cast<f32 *>(RoutingTableMonoTo71);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(1), static_cast<NuSoundSystem::ChannelConfig>(8),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 2;
            matrix->out = 1;
            matrix->matrix = const_cast<f32 *>(RoutingTableStereoToMono);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(2), static_cast<NuSoundSystem::ChannelConfig>(1),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 2;
            matrix->out = 2;
            matrix->matrix = const_cast<f32 *>(RoutingTableStereoToStereo);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(2), static_cast<NuSoundSystem::ChannelConfig>(2),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 2;
            matrix->out = 4;
            matrix->matrix = const_cast<f32 *>(RoutingTableStereoToQuad);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(2), static_cast<NuSoundSystem::ChannelConfig>(4),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 2;
            matrix->out = 6;
            matrix->matrix = const_cast<f32 *>(RoutingTableStereoTo51);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(2), static_cast<NuSoundSystem::ChannelConfig>(6),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 2;
            matrix->out = 8;
            matrix->matrix = const_cast<f32 *>(RoutingTableStereoTo71);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(2), static_cast<NuSoundSystem::ChannelConfig>(8),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 3;
            matrix->out = 1;
            matrix->matrix = const_cast<f32 *>(RoutingTableLRCToMono);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(3), static_cast<NuSoundSystem::ChannelConfig>(1),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 3;
            matrix->out = 2;
            matrix->matrix = const_cast<f32 *>(RoutingTableLRCToStereo);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(3), static_cast<NuSoundSystem::ChannelConfig>(2),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 3;
            matrix->out = 4;
            matrix->matrix = const_cast<f32 *>(RoutingTableLRCToQuad);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(3), static_cast<NuSoundSystem::ChannelConfig>(4),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 3;
            matrix->out = 6;
            matrix->matrix = const_cast<f32 *>(RoutingTableLRCTo51);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(3), static_cast<NuSoundSystem::ChannelConfig>(6),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 3;
            matrix->out = 8;
            matrix->matrix = const_cast<f32 *>(RoutingTableLRCTo71);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(3), static_cast<NuSoundSystem::ChannelConfig>(8),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 6;
            matrix->out = 1;
            matrix->matrix = const_cast<f32 *>(RoutingTable51ToMono);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(6), static_cast<NuSoundSystem::ChannelConfig>(1),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 6;
            matrix->out = 2;
            matrix->matrix = const_cast<f32 *>(RoutingTable51ToStereo);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(6), static_cast<NuSoundSystem::ChannelConfig>(2),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 6;
            matrix->out = 4;
            matrix->matrix = const_cast<f32 *>(RoutingTable51ToQuad);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(6), static_cast<NuSoundSystem::ChannelConfig>(4),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 6;
            matrix->out = 6;
            matrix->matrix = const_cast<f32 *>(RoutingTable51To51);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(6), static_cast<NuSoundSystem::ChannelConfig>(6),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 6;
            matrix->out = 8;
            matrix->matrix = const_cast<f32 *>(RoutingTable51To71);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(6), static_cast<NuSoundSystem::ChannelConfig>(8),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 8;
            matrix->out = 1;
            matrix->matrix = const_cast<f32 *>(RoutingTable71ToMono);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(8), static_cast<NuSoundSystem::ChannelConfig>(1),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 8;
            matrix->out = 2;
            matrix->matrix = const_cast<f32 *>(RoutingTable71ToStereo);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(8), static_cast<NuSoundSystem::ChannelConfig>(2),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 8;
            matrix->out = 4;
            matrix->matrix = const_cast<f32 *>(RoutingTable71ToQuad);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(8), static_cast<NuSoundSystem::ChannelConfig>(4),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 8;
            matrix->out = 6;
            matrix->matrix = const_cast<f32 *>(RoutingTable71To51);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(8), static_cast<NuSoundSystem::ChannelConfig>(6),
                         matrix);
    }
    {
        NuSoundMixMatrix *matrix = static_cast<NuSoundMixMatrix *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuSoundMixMatrix), 4, 1, "", 7));
        if (matrix != NULL) {
            matrix->in = 8;
            matrix->out = 8;
            matrix->matrix = const_cast<f32 *>(RoutingTable71To71);
            matrix->flag = 1;
        }
        table->SetMatrix(static_cast<NuSoundSystem::ChannelConfig>(8), static_cast<NuSoundSystem::ChannelConfig>(8),
                         matrix);
    }
    NuSound.AddRoutingTable(table);
    NuSoundSystem::SetDefaultRoutingTable(table);
}

NuSoundSystem::NuSoundSystem() {

    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, 1);
    pthread_mutex_init(&this->mutex, &attr);
    pthread_mutexattr_destroy(&attr);

    // NuSoundClock::NuSoundClock(&this->clock);
    // clock_callbacks = &(this->clock).callbacks2;
    // puVar1 = &(this->clock).field3_0xc;
    // this->clock_callbacks = clock_callbacks;
    // this->field7_0x44 = puVar1;
    // this->clock_callbacks2 = clock_callbacks;
    // this->field5_0x3c = puVar1;
    // this->field3_0x34 = 0;
    // this->field6_0x40 = 0;
    // this->field9_0x4c = 0;
    // NuSoundVoiceFactoryList::NuSoundVoiceFactoryList(&this->factory_list);
    // this->field17_0x74 = (undefined1 *)&this->field6_0x40;
    // this->field18_0x78 = &this->clock_callbacks;
    // this->field14_0x68 = (undefined1 *)&this->clock_callbacks;
    // this->field15_0x6c = (undefined1 *)&this->field6_0x40;
    // this->list_start = (NuEListNode<> *)&this->field22_0x88;
    // this->list_end = (NuEListNode<> *)&this->field20_0x80;
    // this->field21_0x84 = &this->field22_0x88;
    // this->field22_0x88 = &this->field20_0x80;
    // this->tail_bus = (NuSoundBus *)&this->field29_0xa4;
    // this->field31_0xac = (NuSoundBus *)&this->field27_0x9c;
    // this->field28_0xa0 = (i32)&this->field29_0xa4;
    // this->field29_0xa4 = (NuSoundBus *)&this->field27_0x9c;
    // this->field39_0xcc = (i32)&this->field36_0xc0;
    // this->field38_0xc8 = (i32)&this->field34_0xb8;
    // this->field35_0xbc = (i32)&this->field36_0xc0;
    // this->field36_0xc0 = (i32)&this->field34_0xb8;
    // this->field13_0x64 = 0;
    // this->field45_0xe4 = (undefined1 *)&this->field41_0xd4;
    // this->field16_0x70 = 0;
    // this->voice_count = 0;
    // this->field20_0x80 = 0;
    // this->field23_0x8c = 0;
    // this->field26_0x98 = 0;
    // this->field27_0x9c = 0;
    // this->field30_0xa8 = 0;
    // this->field33_0xb4 = 0;
    // this->field34_0xb8 = 0;
    // this->field37_0xc4 = 0;
    // this->field40_0xd0 = 0;
    // this->field41_0xd4 = 0;
    // this->field44_0xe0 = 0;
    // this->field46_0xe8 = (undefined1 *)&this->field43_0xdc;
    // this->field42_0xd8 = (undefined1 *)&this->field43_0xdc;
    // this->field43_0xdc = (undefined1 *)&this->field41_0xd4;
    // this->field47_0xec = 0;
    unknown_f0[0] = 0;
    unknown_f0[1] = 0;
    this->samples = NULL;
    sample_head = reinterpret_cast<NuSoundSample *>(reinterpret_cast<char *>(&sample_start) -
                                                    offsetof(NuSoundSample, list_previous));
    sample_tail = reinterpret_cast<NuSoundSample *>(reinterpret_cast<char *>(&sample_end) -
                                                    offsetof(NuSoundSample, list_previous));
    sample_start.previous = NULL;
    sample_start.next = sample_tail;
    sample_end.previous = sample_head;
    sample_end.next = NULL;
    sample_list_count = 0;
    sample_load_count = 0;
    this->sample_count = 0x100;
    voice_head =
        reinterpret_cast<NuSoundVoice *>(reinterpret_cast<char *>(&voice_start) - offsetof(NuSoundVoice, field_0x24));
    voice_tail =
        reinterpret_cast<NuSoundVoice *>(reinterpret_cast<char *>(&voice_end) - offsetof(NuSoundVoice, field_0x24));
    voice_start.prev = NULL;
    voice_start.next = voice_tail;
    voice_end.prev = voice_head;
    voice_end.next = NULL;
    this->voice_count = 0;
    effect_head = reinterpret_cast<NuEListNode<NuSoundEffect> *>(&effect_start);
    effect_tail = reinterpret_cast<NuEListNode<NuSoundEffect> *>(&effect_end);
    effect_start.prev = NULL;
    effect_start.next = effect_tail;
    effect_end.prev = effect_head;
    effect_end.next = NULL;
    effect_count = 0;
    bus_head = reinterpret_cast<NuSoundBus *>(&bus_start);
    bus_tail = reinterpret_cast<NuSoundBus *>(&bus_end);
    bus_start.previous = NULL;
    bus_start.next = bus_tail;
    bus_end.previous = bus_head;
    bus_end.next = NULL;
    bus_count = 0;
    routing_head = reinterpret_cast<NuSoundRoutingTable *>(&routing_start);
    routing_tail = reinterpret_cast<NuSoundRoutingTable *>(&routing_end);
    routing_start.prev = NULL;
    routing_start.next = routing_tail;
    routing_end.prev = routing_head;
    routing_end.next = NULL;
    routing_count = 0;
    // libTTapp.so ctor (0x319552): the update gate field63_0x108 starts at 1.
    this->initialised = true;
    // this->field63_0x108 = 1;
    s_staticInstance = this;
}

bool NuSoundSystem::Initialise(i32 size) {
    sTotalMemory[(i32)MemoryDiscipline::SCRATCH] = GetScratchMemorySize();
    sTotalMemory[(i32)MemoryDiscipline::DECODER] = GetDecoderMemorySize();

    sTotalMemory[(i32)MemoryDiscipline::SAMPLE] =
        size - sTotalMemory[(i32)MemoryDiscipline::SCRATCH] - sTotalMemory[(i32)MemoryDiscipline::DECODER];

    sScratchMemory = NU_ALLOC(sTotalMemory[(i32)MemoryDiscipline::SCRATCH], 4, 1, "", NUMEMORY_CATEGORY_NONE);
    sSampleMemory = NU_ALLOC(sTotalMemory[(i32)MemoryDiscipline::SAMPLE], 0x800, 1, "", NUMEMORY_CATEGORY_NONE);
    sDecoderMemory = NU_ALLOC(sTotalMemory[(i32)MemoryDiscipline::DECODER], 0x800, 1, "", NUMEMORY_CATEGORY_NONE);

    NuMemoryGet()->GetThreadMem()->SetBlockDebugCategory(sScratchMemory, 7);

    g_handler.scratch = sScratchMemory;
    g_handler.scratch_size = sTotalMemory[(i32)MemoryDiscipline::SCRATCH];

    sScratchMemMgr = NuMemoryGet()->CreateMemoryManager(&g_handler, "NuSoundSystem Memory");

    if (sTotalMemory[(i32)MemoryDiscipline::DECODER] != 0) {
        NuSoundMemoryManager *manager = NU_ALLOC_T(NuSoundMemoryManager, 1, "", 0);
        if (manager != NULL) {
            new (manager) NuSoundMemoryManager{};
        }
        s_mmDecoder = manager;

        s_mmDecoder->Init("decoder", sDecoderMemory, sTotalMemory[(i32)MemoryDiscipline::DECODER], 4, 0x800);
    }

    NuSoundMemoryManager *manager = NU_ALLOC_T(NuSoundMemoryManager, 1, "", 0);
    if (manager != NULL) {
        new (manager) NuSoundMemoryManager{};
    }
    s_mmSample = manager;

    s_mmSample->EnableDefragOnAlloc(true);
    s_mmSample->Init("sample", sSampleMemory, sTotalMemory[(i32)MemoryDiscipline::SAMPLE], 4, 0x800);

    LOG_DEBUG("this->sample_count=%d", this->sample_count);

    this->samples =
        (NuSoundSample **)_AllocMemory(MemoryDiscipline::SCRATCH, this->sample_count * sizeof(NuSoundSample *), 4,
                                       "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:348");
    memset(this->samples, 0, this->sample_count * sizeof(void *));

    if (InitAudioDevice()) {
        sMasterBus = CreateBus("Master", true);
        if (sMasterBus != 0) {
            NuSoundInitDefaultRoutingTables();
            return true;
        }
    }

    return false;
}

// libTTapp.so 0x319640: SAMPLE/DECODER hand the block header back to the
// pool manager; SCRATCH goes through NuMemoryManager::BlockFree and accounts
// the queried block size.
u32 NuSoundSystem::FreeMemory(MemoryDiscipline disc, usize address, u32 size) {
    u32 freed = size;

    switch (disc) {
        case MemoryDiscipline::SAMPLE:
            if (s_mmSample != NULL) {
                s_mmSample->Free((NuSoundMemoryBuffer *)address);
            }
            break;
        case MemoryDiscipline::DECODER:
            if (s_mmDecoder != NULL) {
                s_mmDecoder->Free((NuSoundMemoryBuffer *)address);
            }
            break;
        case MemoryDiscipline::SCRATCH:
            freed = sScratchMemMgr->GetBlockSize((void *)address);
            sScratchMemMgr->BlockFree((void *)address, 0);
            break;
        default:
            return 0;
    }

    sAllocdMemory[(i32)disc] = sAllocdMemory[(i32)disc] - freed;
    return freed;
}

u32 NuSoundSystem::GetFreeMemory(MemoryDiscipline disc) {
    return sTotalMemory[(i32)disc] - sAllocdMemory[(i32)disc];
}

void *NuSoundSystem::_AllocMemory(MemoryDiscipline disc, u32 size, u32 align, const char *name) {
    u32 uVar1 = GetFreeMemory(disc);

    void *pvVar2 = NULL;

    if (size <= uVar1) {
        switch (disc) {
            case MemoryDiscipline::SAMPLE:
                pvVar2 = s_mmSample->Alloc(size);
                break;
            case MemoryDiscipline::DECODER:
                pvVar2 = s_mmDecoder->Alloc(size);
                break;
            case MemoryDiscipline::SCRATCH:
                pvVar2 = sScratchMemMgr->_TryBlockAlloc(size, align, 1, name, 0);
                break;
            default:
                return NULL;
        }

        if (pvVar2 != NULL) {
            sAllocdMemory[(i32)disc] = sAllocdMemory[(i32)disc] + size;
        }
    }

    return pvVar2;
}

u32 NuSoundSystem::GetStreamBufferSize() {
    return 0x20000;
}
u32 NuSoundSystem::GetScratchMemorySize() {
    return 0x4b000;
}
u32 NuSoundSystem::GetDecoderMemorySize() {
    return 0x100000;
}

NuSoundBus *NuSoundSystem::CreateBus(const char *name, bool is_master) {
    i32 *piVar1;
    i32 iVar2;

    NuSoundBus *bus = GetBus(name);

    if (bus == NULL) {
        bus = (NuSoundBus *)_AllocMemory(NuSoundSystem::MemoryDiscipline::SCRATCH, sizeof(NuSoundBus), 4,
                                         "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1180");

        if (bus != NULL) {
            new (bus) NuSoundBus(name, is_master);
            NuSoundBus *previous = bus_tail->previous;
            bus_tail->previous = bus;
            bus->previous = previous;
            previous->next = bus;
            bus->next = bus_tail;
            ++bus_count;

            // TODO
            // piVar1 = *(i32 **)&this->field_0xb0;
            // iVar2 = *piVar1;
            //*piVar1 = (i32)bus;
            // bus->field0_0x0 = iVar2;
            //*(NuSoundBus **)(iVar2 + 4) = bus;
            // bus->field1_0x4 = piVar1;
            //*(i32 *)&this->field_0xb4 = *(i32 *)&this->field_0xb4 + 1;
        }
    }

    return bus;
}

NuSoundSample *NuSoundSystem::AddSample(const char *name, FileType file_type, NuSoundSource::FeedType feed_type) {
    char buf[0x100];
    sprintf(buf, "%s.%s", name, GetFileExtension(file_type));
    NuFileNormalise(buf, 0x100, buf);

    NuSoundSample *sample = GetSample(buf);
    if (sample != NULL) {
        return sample;
    }

    if (feed_type == NuSoundSource::FeedType::ZERO) {
        sample = (NuSoundSample *)_AllocMemory(MemoryDiscipline::SCRATCH, 0x80, 4,
                                               "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:646");
        if (sample == NULL) {
            return NULL;
        }

        new (sample) NuSoundSample(buf, NuSoundSource::FeedType::ZERO);
    } else if (feed_type == NuSoundSource::FeedType::STREAMING) {

        sample = (NuSoundSample *)_AllocMemory(MemoryDiscipline::SCRATCH, 0x9c, 4,
                                               "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:654");
        if (sample == NULL) {
            return NULL;
        }

        new (sample) NuSoundStreamingSample(buf);
    } else {
        return NULL;
    }

    NuSoundSample *previous = sample_tail->list_previous;
    sample_tail->list_previous = sample;
    sample->list_previous = previous;
    previous->list_next = sample;
    sample->list_next = sample_tail;
    ++sample_list_count;
    i32 hash = GenerateHash(buf);
    sample->next = samples[hash];
    samples[hash] = sample;
    return sample;
}

const char *NuSoundSystem::GetFileExtension(FileType type) {
    return sFileExtensions[(i32)type];
}

NuSoundSample *NuSoundSystem::GetSample(const char *path) {
    i32 hash = GenerateHash(path);
    LOG_DEBUG("GetSample: path=%s, hash=%d", path, hash);

    if (this->samples != NULL) {
        for (NuSoundSample *sample = this->samples[hash]; sample != NULL; sample = sample->next) {
            if (NuStrICmp(sample->GetName(), path) == 0) {
                return sample;
            }
        }
    }

    return NULL;
}

i32 NuSoundSystem::GenerateHash(const char *str) {
    char buf[0x100];
    NuStrUpr(buf, str);

    u32 hash = 0x1505;

    for (char *c = buf; *c != '\0'; c++) {
        hash = (hash * 0x21) + static_cast<signed char>(*c);
    }

    return hash & 0xff;
}

NuSoundSystem::FileType NuSoundSystem::DetermineFileType(const char *path) {
    i32 len = NuStrLen(path);
    if (len >= 5) {
        char ext[4];
        ext[0] = path[len - 3];
        ext[1] = path[len - 2];
        ext[2] = path[len - 1];
        ext[3] = '\0';

        for (i32 i = 0; i < static_cast<i32>(FileType::_COUNT); i++) {
            if (NuStrICmp(ext, sFileExtensions[i]) == 0) {
                return static_cast<FileType>(i);
            }
        }
    }

    return FileType::INVALID;
}

void NuSoundSystem::ReleaseFileLoader(NuSoundLoader *loader) {
    loader->~NuSoundLoader();
    FreeMemory(MemoryDiscipline::SCRATCH, reinterpret_cast<usize>(loader), 0);
}

NuSoundSystem::~NuSoundSystem() {
    s_staticInstance = NULL;
}

i32 NuSoundStreamDesc::DecodeStreamOnOpen() const {
    return 0;
}

i32 NuSoundStreamDesc::GetLoopStart() const {
    return 0;
}

i32 NuSoundStreamDesc::GetLoopEnd() const {
    return 0;
}

bool NuSoundSystem::AddListener(NuSoundListener *listener) {
    NuSoundListener *previous = listeners.tail->prev;
    listeners.tail->prev = listener;
    listener->prev = previous;
    previous->next = listener;
    listener->next = listeners.tail;
    ++listeners.length;
    return true;
}

void NuSoundSystem::AddRoutingTable(NuSoundRoutingTable *table) {
    NuSoundRoutingTable *previous = routing_tail->unknown_00;
    routing_tail->unknown_00 = table;
    table->unknown_00 = previous;
    previous->unknown_04 = table;
    table->unknown_04 = routing_tail;
    ++routing_count;
}

f32 NuSoundSystem::AmplitudeTodB(float amplitude) {
    if (amplitude <= 0.0f)
        return -100.0f;
    if (amplitude >= 1.0f)
        return 0.0f;
    return NuLog10(amplitude) * 20.0f;
}

f32 NuSoundSystem::CalculateCrossfadeHeight(NuSoundSystem::CurveData const &, float) const {
    return 0.0f;
}

void NuSoundSystem::CreateCrossfadeCurve(u32) {
}

// libTTapp.so 0x31a810: builds the "<name>_decoder" name from the source's
// name, then constructs the format-specific decoder. Only OGG streams
// (encoded format 3) get a decoder; anything else returns NULL and plays
// through the plain sample path.
NuSoundDecoder *NuSoundSystem::CreateDecoder(NuSoundSource *source) {
    // The original uses a growable, 16-bit-length string for the decoder name.
    char *decoded_name = const_cast<char *>(theEmptyString);
    u16 length = 1;
    u16 capacity = 1;
    const char *name = source->GetName();
    if (name != NULL) {
        length = static_cast<u16>(strlen(name) + 1);
        if (length > capacity || decoded_name == theEmptyString) {
            capacity = (length + 3) & 0xfffc;
            if (decoded_name == theEmptyString) {
                decoded_name = static_cast<char *>(
                    NU_ALLOC(capacity, 4, 5,
                             "i:/SagaTouch-Android_9176564/nu2api.saga/../nu2api.2013/numemory/NuMemory.h:328", 0));
            } else {
                decoded_name = static_cast<char *>(NuMemoryGet()->GetThreadMem()->_BlockReAlloc(
                    decoded_name, capacity, 4, 5,
                    "i:/SagaTouch-Android_9176564/nu2api.saga/../nu2api.2013/numemory/NuMemory.h:333", 0));
            }
        }
        memcpy(decoded_name, name, length);
    }
    u16 appended_length = length + 8;
    if (appended_length > capacity || decoded_name == theEmptyString) {
        capacity = (appended_length + 3) & 0xfffc;
        if (decoded_name == theEmptyString) {
            decoded_name = static_cast<char *>(NU_ALLOC(
                capacity, 4, 5, "i:/SagaTouch-Android_9176564/nu2api.saga/../nu2api.2013/numemory/NuMemory.h:328", 0));
        } else {
            decoded_name = static_cast<char *>(NuMemoryGet()->GetThreadMem()->_BlockReAlloc(
                decoded_name, capacity, 4, 5,
                "i:/SagaTouch-Android_9176564/nu2api.saga/../nu2api.2013/numemory/NuMemory.h:333", 0));
        }
    }
    memcpy(decoded_name + length - 1, "_decoder", 9);

    NuSoundDecoderOGG *decoder = NULL;
    NuSoundStreamDesc *desc = source->GetStreamDesc();
    if (desc->GetEncodedDataFormat() == NuSoundStreamDesc::DataFormat::THREE) {
        decoder = static_cast<NuSoundDecoderOGG *>(
            _AllocMemory(NuSoundSystem::MemoryDiscipline::SCRATCH, sizeof(NuSoundDecoderOGG), 4,
                         "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound_system.cpp:436"));
        if (decoder != NULL)
            new (decoder) NuSoundDecoderOGG(decoded_name, source);
    }
    if (decoded_name != theEmptyString)
        NuMemoryGet()->GetThreadMem()->BlockFree(decoded_name, 4);
    return decoder;
}

inline void NuSoundEffectAttenuation::ProcessVoice(NuSoundVoice *voice, f32) {
    if (reference_count == 0) {
        attenuation = value;
        return;
    }
    if (static_cast<u32>(voice->GetSurroundMode()) != 2) {
        for (ReferenceNode *node = reference_head->next; node != reference_tail; node = node->next) {
            if (node->reference.object != NULL && node->reference.object == voice->positional_references[1].object) {
                attenuation = value;
                return;
            }
        }
    }
    attenuation = 1.0f;
}

inline void NuSoundEffectRepeat::ProcessVoice(NuSoundVoice *voice, f32 frametime) {
    if (static_cast<u32>(voice->GetState()) == 1) {
        if (armed && repeats != 0) {
            --repeats;
            armed = false;
            remaining = delay;
            voice->DestroyHardwareVoice();
            voice->CreateHardwareVoice();
            voice->Play();
            voice->Pause();
        }
    } else if (static_cast<u32>(voice->GetState()) == 2) {
        if (!armed && repeats != 0) {
            remaining -= frametime;
            if (remaining <= 0.0f)
                voice->Resume();
        }
    } else {
        armed = true;
    }
}

NuSoundEffect *NuSoundSystem::CreateEffect(NuSoundEffect::EffectType type) {
    NuSoundEffect *effect = NULL;
    switch (static_cast<u32>(type)) {
        case 0:
            effect = static_cast<NuSoundEffect *>(
                _AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectAttenuation), 4,
                             "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1094"));
            if (effect != NULL)
                new (effect) NuSoundEffectAttenuation();
            break;
        case 1:
            effect = static_cast<NuSoundEffect *>(
                _AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectPitch), 4,
                             "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1100"));
            if (effect != NULL)
                new (effect) NuSoundEffectPitch();
            break;
        case 2:
            effect = static_cast<NuSoundEffect *>(
                _AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectFader), 4,
                             "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1106"));
            if (effect != NULL)
                new (effect) NuSoundEffectFader();
            break;
        case 3:
            effect = static_cast<NuSoundEffect *>(
                _AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectPitchRamp), 4,
                             "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1112"));
            if (effect != NULL)
                new (effect) NuSoundEffectPitchRamp();
            break;
        case 4:
            effect = static_cast<NuSoundEffect *>(
                _AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectRandomVolume), 4,
                             "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1118"));
            if (effect != NULL)
                new (effect) NuSoundEffectRandomVolume();
            break;
        case 5:
            effect = static_cast<NuSoundEffect *>(
                _AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectRandomPitch), 4,
                             "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1124"));
            if (effect != NULL)
                new (effect) NuSoundEffectRandomPitch();
            break;
        case 8:
            effect = static_cast<NuSoundEffect *>(
                _AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectDoppler), 4,
                             "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1130"));
            if (effect != NULL)
                new (effect) NuSoundEffectDoppler();
            break;
        case 6:
            effect = static_cast<NuSoundEffect *>(
                _AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectRepeat), 4,
                             "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1136"));
            if (effect != NULL)
                new (effect) NuSoundEffectRepeat();
            break;
        default:
            return NULL;
    }
    if (effect == NULL)
        return NULL;
    effect->Initialise();
    NuMemoryManager *old_manager = NuMemoryGet()->SetThreadMem(sScratchMemMgr);
    NuEListNode<NuSoundEffect> *node = static_cast<NuEListNode<NuSoundEffect> *>(
        NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(NuEListNode<NuSoundEffect>), 4, 1, "", 0));
    if (node != NULL) {
        node->prev = NULL;
        node->next = NULL;
        node->data = effect;
    }
    NuEListNode<NuSoundEffect> *previous = effect_tail->prev;
    effect_tail->prev = node;
    node->prev = previous;
    previous->next = node;
    node->next = effect_tail;
    ++effect_count;
    NuMemoryGet()->SetThreadMem(old_manager);
    sAllocdMemory[0] += sizeof(NuEListNode<NuSoundEffect>);
    return effect;
}

void NuSoundSystem::DefragmentSampleMemory() {
    s_mmSample->Defragment(0);
}

NuSoundSystem::FileType NuSoundSystem::DetermineFileType(NUFILETYPE type) {
    char extension[6] = {};
    NuFileExtGetExt(extension, 6, type);
    return DetermineFileType(extension);
}

void NuSoundSystem::Disable() {
    sNumAvailableOutputDevices = 0;
}

bool NuSoundSystem::FileTypeSupported(NuSoundSystem::FileType type) {
    static const bool supported[6] = {true, false, false, false, false, true};
    u32 index = static_cast<u32>(type);
    return index <= 5 ? supported[index] : false;
}

NuSoundSystem *NuSoundSystem::Get() {
    return s_staticInstance;
}

u32 NuSoundSystem::GetAllocdMemory(NuSoundSystem::MemoryDiscipline discipline) {
    return sAllocdMemory[static_cast<i32>(discipline)];
}

u32 NuSoundSystem::GetBufferAlignment() {
    return 32;
}

i32 NuSoundSystem::GetClosestSupportedConfig(i32 config) {
    // libTTapp.so 0x31bcb0: config > 7 -> 8, config >= 6 -> 6, else 2.
    if (config > 7) {
        return 8;
    }
    return (config >= 6) ? 6 : 2;
}

void NuSoundSystem::GetCrossfadeCurve(u32) const {
}

NuSoundSystem::FileType NuSoundSystem::GetDefaultFileType(NuSoundSource::FeedType feed) {
    if (feed == NuSoundSource::FeedType::ZERO)
        return DetermineFileType(static_cast<NUFILETYPE>(6));
    if (feed == NuSoundSource::FeedType::STREAMING)
        return DetermineFileType(static_cast<NUFILETYPE>(7));
    return FileType::INVALID;
}

NuSoundRoutingTable *NuSoundSystem::GetDefaultRoutingTable() {
    return sDefaultRoutingTable;
}

u32 NuSoundSystem::GetGfxMemorySize() {
    if (sGfxMemorySize != 0 && GetScratchMemorySize() < sGfxMemorySize) {
        return sGfxMemorySize - GetScratchMemorySize();
    }
    return 0x600000;
}

const char *NuSoundSystem::GetLanguageString(bool) {
    return "Eng";
}

u32 NuSoundSystem::GetLargestMemoryFragment(NuSoundSystem::MemoryDiscipline discipline) {
    if (discipline == MemoryDiscipline::SCRATCH)
        return sScratchMemMgr->CalculateLargestFragmentSize();
    return 0;
}

NuEList<NuSoundListener, DefaultElist> const *NuSoundSystem::GetListeners() {
    return &listeners;
}

i32 NuSoundSystem::GetNumAvailableOutputDevices() {
    return sNumAvailableOutputDevices;
}

NuSoundVoice *NuSoundSystem::GetOldestVoice(NuSoundSample *sample, float &value) {
    value = -1.0f;
    NuSoundVoice *result = NULL;
    NuSoundVoice *first = VoiceLinksFor(voice_head)->next;
    VoiceLinks *node = first == NULL ? NULL : VoiceLinksFor(first);
    VoiceLinks *end = voice_tail == NULL ? NULL : VoiceLinksFor(voice_tail);
    while (node != end) {
        NuSoundVoice *voice =
            reinterpret_cast<NuSoundVoice *>(reinterpret_cast<char *>(node) - offsetof(NuSoundVoice, field_0x24));
        if (voice->GetState() == NuSoundVoice::PLAYSTATE_PLAYING &&
            voice->sound_source->GetEncodedSource() == sample->GetEncodedSource()) {
            float candidate = voice->GetPlaybackPositionSeconds();
            if (value < 0.0f || candidate > value) {
                result = voice;
                value = candidate;
            }
        }
        NuSoundVoice *next = node->next;
        node = next == NULL ? NULL : VoiceLinksFor(next);
    }
    return result;
}

i32 NuSoundSystem::GetOutputChannelConfig() {
    return sOutputConfig;
}

void NuSoundSystem::GetPeakAllocdMemory(NuSoundSystem::MemoryDiscipline) {
}

const char *NuSoundSystem::GetPlatformString() {
    return "ANDROID";
}

NuSoundVoice *NuSoundSystem::GetQuietestVoice(NuSoundSample *sample, float &value) {
    value = -1.0f;
    NuSoundVoice *result = NULL;
    NuSoundVoice *first = VoiceLinksFor(voice_head)->next;
    VoiceLinks *node = first == NULL ? NULL : VoiceLinksFor(first);
    VoiceLinks *end = voice_tail == NULL ? NULL : VoiceLinksFor(voice_tail);
    while (node != end) {
        NuSoundVoice *voice =
            reinterpret_cast<NuSoundVoice *>(reinterpret_cast<char *>(node) - offsetof(NuSoundVoice, field_0x24));
        if (voice->GetState() == NuSoundVoice::PLAYSTATE_PLAYING &&
            voice->sound_source->GetEncodedSource() == sample->GetEncodedSource()) {
            float candidate = voice->field67_0xa8 * voice->GetVolume();
            if (value < 0.0f || candidate < value) {
                result = voice;
                value = candidate;
            }
        }
        NuSoundVoice *next = node->next;
        node = next == NULL ? NULL : VoiceLinksFor(next);
    }
    return result;
}

NuSoundRoutingTable *NuSoundSystem::GetRoutingTable(char const *name) {
    for (NuSoundRoutingTable *table = routing_head->unknown_04; table != routing_tail; table = table->unknown_04) {
        if (NuStrICmp(table->GetName(), name) == 0)
            return table;
    }
    return NULL;
}

u32 NuSoundSystem::GetTotalMemory(NuSoundSystem::MemoryDiscipline discipline) {
    return sTotalMemory[static_cast<i32>(discipline)];
}

bool NuSoundSystem::LoadSample(NuSoundSample *sample, void *data, i32 size, NuSoundOutOfMemCallback *callback) {
    NuSoundSample::LoadState state = sample->GetLoadState();
    if (state == NuSoundSample::LoadState::NOT_LOADED) {
        if (sample->Load(data, size, callback) != NuSoundSample::ErrorState::NONE)
            return false;
        ++sample_load_count;
        state = sample->GetLoadState();
    }
    return state == NuSoundSample::LoadState::LOADED;
}

void NuSoundSystem::PauseAllVoices() {
    NuSoundVoice *first = VoiceLinksFor(voice_head)->next;
    VoiceLinks *node = first == NULL ? NULL : VoiceLinksFor(first);
    VoiceLinks *end = voice_tail == NULL ? NULL : VoiceLinksFor(voice_tail);
    while (node != end) {
        NuSoundVoice *voice =
            reinterpret_cast<NuSoundVoice *>(reinterpret_cast<char *>(node) - offsetof(NuSoundVoice, field_0x24));
        voice->Pause();
        NuSoundVoice *next = node->next;
        node = next == NULL ? NULL : VoiceLinksFor(next);
    }
}

void NuSoundSystem::PauseVoices(i32 mask) {
    NuSoundVoice *first = VoiceLinksFor(voice_head)->next;
    VoiceLinks *node = first == NULL ? NULL : VoiceLinksFor(first);
    VoiceLinks *end = voice_tail == NULL ? NULL : VoiceLinksFor(voice_tail);
    while (node != end) {
        NuSoundVoice *voice =
            reinterpret_cast<NuSoundVoice *>(reinterpret_cast<char *>(node) - offsetof(NuSoundVoice, field_0x24));
        if ((voice->field131_0x148 & mask) != 0)
            voice->Pause();
        NuSoundVoice *next = node->next;
        node = next == NULL ? NULL : VoiceLinksFor(next);
    }
}

void NuSoundSystem::ReAllocMemory(NuSoundSystem::MemoryDiscipline, u32, u32) {
}

void NuSoundSystem::ReleaseBus(NuSoundBus *bus) {
    NuSoundBus *next = bus->next;
    NuSoundBus *previous = bus->previous;
    if (next != NULL || previous != NULL) {
        --bus_count;
        if (previous != NULL)
            previous->next = next;
        if (next != NULL)
            next->previous = previous;
        bus->next = NULL;
        bus->previous = NULL;
    }
    bus->~NuSoundBus();
    FreeMemory(MemoryDiscipline::SCRATCH, reinterpret_cast<usize>(bus), 0);
}

void NuSoundSystem::ReleaseCrossfadeCurve(u32) {
}

void NuSoundSystem::ReleaseDecoder(NuSoundDecoder *decoder) {
    decoder->~NuSoundDecoder();
    FreeMemory(MemoryDiscipline::SCRATCH, reinterpret_cast<usize>(decoder), 0);
}

void NuSoundSystem::ReleaseEffect(NuSoundEffect *effect) {
    if (effect == NULL) {
        return;
    }

    if (effect_count != 0) {
        NuEListNode<NuSoundEffect> *last = effect_tail->prev;
        NuEListNode<NuSoundEffect> *node = effect_head->next;
        u32 removed = 0;
        for (;;) {
            if (node->data == effect) {
                bool was_last = node == last;
                NuEListNode<NuSoundEffect> *next = node->next;
                if (node->prev != NULL)
                    node->prev->next = next;
                if (next != NULL)
                    next->prev = node->prev;
                NuMemoryGet()->GetThreadMem()->BlockFree(node, 0);
                ++removed;
                if (was_last)
                    break;
                node = next;
            }
            if (node == last)
                break;
            // The original advances once more after removing a non-final
            // node (0x319873..0x319824); retain that iterator behavior.
            node = node->next;
        }
        effect_count -= removed;
    }

    sAllocdMemory[static_cast<i32>(MemoryDiscipline::SCRATCH)] -= sizeof(NuEListNode<NuSoundEffect>);
    effect->Shutdown();
    effect->~NuSoundEffect();
    FreeMemory(MemoryDiscipline::SCRATCH, reinterpret_cast<usize>(effect), 0);
}

void NuSoundSystem::ReleaseSample(NuSoundSample *sample) {
    NuSoundSample *next = sample->list_next;
    NuSoundSample *previous = sample->list_previous;
    if (next != NULL || previous != NULL) {
        --sample_list_count;
        if (previous != NULL)
            previous->list_next = next;
        if (next != NULL)
            next->list_previous = previous;
        sample->list_next = NULL;
        sample->list_previous = NULL;
    }
    sample->~NuSoundSample();
    FreeMemory(MemoryDiscipline::SCRATCH, reinterpret_cast<usize>(sample), 0);
}

void NuSoundSystem::RemoveListener(NuSoundListener *listener) {
    if (listener->next != NULL || listener->prev != NULL) {
        --listeners.length;
        NuSoundListener *next = listener->next;
        NuSoundListener *previous = listener->prev;
        if (previous != NULL)
            previous->next = next;
        if (next != NULL)
            next->prev = previous;
        listener->next = NULL;
        listener->prev = NULL;
    }
}

void NuSoundSystem::ResumeAllVoices() {
    NuSoundVoice *first = VoiceLinksFor(voice_head)->next;
    VoiceLinks *node = first == NULL ? NULL : VoiceLinksFor(first);
    VoiceLinks *end = voice_tail == NULL ? NULL : VoiceLinksFor(voice_tail);
    while (node != end) {
        NuSoundVoice *voice =
            reinterpret_cast<NuSoundVoice *>(reinterpret_cast<char *>(node) - offsetof(NuSoundVoice, field_0x24));
        voice->Resume();
        NuSoundVoice *next = node->next;
        node = next == NULL ? NULL : VoiceLinksFor(next);
    }
}

void NuSoundSystem::ResumeVoices(i32 mask) {
    NuSoundVoice *first = VoiceLinksFor(voice_head)->next;
    VoiceLinks *node = first == NULL ? NULL : VoiceLinksFor(first);
    VoiceLinks *end = voice_tail == NULL ? NULL : VoiceLinksFor(voice_tail);
    while (node != end) {
        NuSoundVoice *voice =
            reinterpret_cast<NuSoundVoice *>(reinterpret_cast<char *>(node) - offsetof(NuSoundVoice, field_0x24));
        if ((voice->field131_0x148 & mask) != 0)
            voice->Resume();
        NuSoundVoice *next = node->next;
        node = next == NULL ? NULL : VoiceLinksFor(next);
    }
}

void NuSoundSystem::SetDefaultRoutingTable(NuSoundRoutingTable *table) {
    sDefaultRoutingTable = table;
}

void NuSoundSystem::SetGfxMemorySize(u32 size) {
    sGfxMemorySize = size;
}

void NuSoundSystem::SetMainThreadID(NuThread *) {
}

void NuSoundSystem::Shutdown() {
    NuSoundVoice *voice = voice_head->field_0x28;
    while (voice != voice_tail) {
        NuSoundVoice *next = voice->field_0x28;
        voice->Stop(false);
        ReleaseVoice(voice);
        voice = next;
    }
    UnloadAllSamples();
    NuSoundSample *sample = sample_head->list_next;
    while (sample != sample_tail) {
        NuSoundSample *next = sample->list_next;
        sample->~NuSoundSample();
        FreeMemory(MemoryDiscipline::SCRATCH, reinterpret_cast<usize>(sample), 0);
        // The reference advances through the saved next node once more.
        sample = next->list_next;
    }
    NuSoundBus *bus = bus_head->next;
    while (bus != bus_tail) {
        NuSoundBus *next = bus->next;
        ReleaseBus(bus);
        bus = next->next;
    }
    FreeMemory(MemoryDiscipline::SCRATCH, reinterpret_cast<usize>(samples), 0);
    samples = NULL;
    if (s_mmSample != NULL) {
        s_mmSample->~NuSoundMemoryManager();
        NuMemoryGet()->GetThreadMem()->BlockFree(s_mmSample, 0);
    }
    if (s_mmDecoder != NULL) {
        s_mmDecoder->~NuSoundMemoryManager();
        NuMemoryGet()->GetThreadMem()->BlockFree(s_mmDecoder, 0);
    }
    NuMemoryGet()->DestroyMemoryManager(sScratchMemMgr);
    NuMemoryGet()->GetThreadMem()->BlockFree(sDecoderMemory, 0);
    NuMemoryGet()->GetThreadMem()->BlockFree(sSampleMemory, 0);
    NuMemoryGet()->GetThreadMem()->BlockFree(sScratchMemory, 0);
    ShutdownAudioDevice();
}

bool NuSoundSystem::SourceRequiresDecoder(NuSoundSource *source) {
    NuSoundStreamDesc *desc = source->GetStreamDesc();
    if (desc == NULL) {
        return false;
    }

    if (desc->GetEncodedDataFormat() != desc->GetDecodedDataFormat() && desc->DecodeStreamOnOpen() == 0) {
        // A handful of short effect sounds get their own special case (they
        // are pre-decoded elsewhere).
        const char *name = source->GetName();
        if (strstr(name, "coin") != NULL || strstr(name, "counter") != NULL || strstr(name, "fs_") != NULL ||
            strstr(name, "saber") != NULL) {
            return false;
        }
        return true;
    }
    return false;
}

void NuSoundSystem::StopAllVoices() {
}

void NuSoundSystem::StopVoices(NuSoundSource const &) {
}

void NuSoundSystem::StopVoices(i32) {
}

void NuSoundSystem::UnloadAllSamples() {
    for (NuSoundSample *sample = sample_head->list_next; sample != sample_tail; sample = sample->list_next) {
        if (sample->GetLoadState() == NuSoundSample::LoadState::LOADED)
            UnloadSample(sample);
    }
}

bool NuSoundSystem::UnloadSample(NuSoundSample *sample) {
    if (sample == NULL || sample->field_0x18 != 0)
        return false;
    if (sample->GetLoadState() == NuSoundSample::LoadState::NOT_LOADED)
        return false;
    sample->Unload();
    return true;
}

void NuSoundSystem::Update(f32 frametime) {
    if (this->initialised == false) {
        return;
    }

    // Platform hook (on Android this only polls the application state).
    this->UpdateAudioDevice();

    pthread_mutex_lock(&this->mutex);

    // Original first pass: process the system's registered effects.
    NuEListNode<NuSoundEffect> *effects_end = effect_tail;
    for (NuEListNode<NuSoundEffect> *node = effect_head->next; node != effects_end; node = node->next) {
        node->data->Process(frametime);
    }

    // Pass 2: update the engine-side mix of every playing voice; stopped
    // auto-delete voices are released.
    VoiceLinks *node = VoiceLinksFor(voice_head->field_0x28);
    VoiceLinks *end = VoiceLinksFor(voice_tail);
    while (node != end) {
        NuSoundVoice *voice =
            reinterpret_cast<NuSoundVoice *>(reinterpret_cast<char *>(node) - offsetof(NuSoundVoice, field_0x24));
        VoiceLinks *next = VoiceLinksFor(node->next);

        NuSoundVoice::PlayState state = voice->GetState();
        if (state == NuSoundVoice::PLAYSTATE_PLAYING) {
            pthread_mutex_lock(&NuSoundWeakPtrListNode::sPtrAccessLock.mutex);
            voice->Update(frametime);
            pthread_mutex_unlock(&NuSoundWeakPtrListNode::sPtrAccessLock.mutex);
        } else if (state == NuSoundVoice::PLAYSTATE_STOPPED && voice->GetAutoDelete()) {
            this->ReleaseVoice(voice);
        }

        node = next;
    }

    pthread_mutex_unlock(&this->mutex);
}

void NuSoundSystem::dBToAmplitude(float) {
}

NuSoundVoice *NuSoundSystem::CreateVoice(NuSoundSource *source, bool loop) {
    NuSoundVoice *voice = NULL;

    if (!SourceRequiresDecoder(source)) {
        if (source->IsStreamOpen()) {
            NuSoundStreamDesc *desc = source->GetStreamDesc();
            NuSoundVoiceFactory *factory = factory_list.GetFactory(desc->GetDecodedDataFormat());
            voice = factory->CreateVoice(source, loop);
        }
    } else {
        source = CreateDecoder(source);
        source->OpenStream(loop);
        if (source->IsStreamOpen()) {
            NuSoundStreamDesc *desc = source->GetStreamDesc();
            NuSoundVoiceFactory *factory = factory_list.GetFactory(desc->GetDecodedDataFormat());
            voice = factory->CreateVoice(source, loop);
            if (voice == NULL) {
                source->CloseStream();
                ReleaseDecoder(static_cast<NuSoundDecoder *>(source));
            }
        } else {
            ReleaseDecoder(static_cast<NuSoundDecoder *>(source));
        }
    }

    if (voice == NULL)
        return NULL;

    // Append the voice to the system's voice list.
    pthread_mutex_lock(&this->mutex);
    pthread_mutex_lock(&NuSoundWeakPtrListNode::sPtrAccessLock.mutex);
    VoiceLinks *tail_links = VoiceLinksFor(voice_tail);
    NuSoundVoice *previous = tail_links->prev;
    VoiceLinks *previous_links = VoiceLinksFor(previous);
    tail_links->prev = voice;
    voice->field_0x24 = previous;
    previous_links->next = voice;
    voice->field_0x28 =
        reinterpret_cast<NuSoundVoice *>(reinterpret_cast<char *>(tail_links) - offsetof(NuSoundVoice, field_0x24));
    this->voice_count++;
    pthread_mutex_unlock(&NuSoundWeakPtrListNode::sPtrAccessLock.mutex);
    pthread_mutex_unlock(&this->mutex);

    return voice;
}

void NuSoundSystem::ReleaseVoice(NuSoundVoice *voice) {
    pthread_mutex_lock(&this->mutex);

    // Detach effects (releasing the ones the system owns).
    for (NuListNodeBase *node = voice->effects.Head(); node != voice->effects.Tail();) {
        NuSoundEffect *effect = static_cast<NuListNode<NuSoundEffect *> *>(node)->value;
        NuListNodeBase *next = node->GetNext();
        voice->RemoveEffect(effect);
        if (effect->system_owned) {
            this->ReleaseEffect(effect);
        }
        node = next;
    }

    // Streaming sources opened through a decoder close their stream here.
    NuSoundDecoder *decoder = NULL;
    if (this->SourceRequiresDecoder(voice->sound_source)) {
        decoder = (NuSoundDecoder *)voice->sound_source;
    }

    // Unlink from the voice list.
    pthread_mutex_lock(&NuSoundWeakPtrListNode::sPtrAccessLock.mutex);
    if (voice->field_0x28 != NULL || voice->field_0x24 != NULL) {
        this->voice_count--;
        if (voice->field_0x24 != NULL) {
            VoiceLinksFor(voice->field_0x24)->next = voice->field_0x28;
        }
        if (voice->field_0x28 != NULL) {
            VoiceLinksFor(voice->field_0x28)->prev = voice->field_0x24;
        }
        voice->field_0x28 = NULL;
        voice->field_0x24 = NULL;
    }

    // libTTapp.so 0x31b394: run the voice's complete destructor (vtable slot
    // 0, no free), then hand the block back through FreeMemory(SCRATCH).
    voice->~NuSoundVoice();
    NuSoundSystem::FreeMemory(NuSoundSystem::MemoryDiscipline::SCRATCH, (usize)voice, 0);
    pthread_mutex_unlock(&NuSoundWeakPtrListNode::sPtrAccessLock.mutex);

    if (decoder != NULL) {
        decoder->CloseStream();
        this->ReleaseDecoder(decoder);
    }

    pthread_mutex_unlock(&this->mutex);
}

void NuSound3ExitThreads() {
}

void NuSound_GetAllocdSampleMemory() {
}

NuSoundListener *NuSoundSystem::GetNearestRealListener(NuEList<NuSoundListener, DefaultElist> const &listeners,
                                                       VuVec const &position) {
    NuSoundListener *nearest = NULL;
    f32 nearest_distance = FLT_MAX;
    for (NuSoundListener *listener = listeners.head->next; listener != listeners.tail; listener = listener->next) {
        if (!listener->IsEnabled() || !(listener->GetSensitivity() > 0.0f))
            continue;
        f32 distance = listener->GetHeadDistance(position);
        distance /= listener->GetSensitivity();
        if (distance < nearest_distance || nearest == NULL) {
            nearest_distance = distance;
            nearest = listener;
        }
    }
    return nearest;
}

NuSoundListener *NuSoundSystem::GetNearestFocusListener(NuEList<NuSoundListener, DefaultElist> const &listeners,
                                                        VuVec const &position, float &nearest_distance) {
    nearest_distance = FLT_MAX;
    NuSoundListener *nearest = NULL;
    for (NuSoundListener *listener = listeners.head->next; listener != listeners.tail; listener = listener->next) {
        if (!listener->IsEnabled() || !(listener->GetSensitivity() > 0.0f))
            continue;
        f32 distance = listener->GetAttenuationDistance(position);
        distance /= listener->GetSensitivity();
        if (distance < nearest_distance || nearest == NULL) {
            nearest_distance = distance;
            nearest = listener;
        }
    }
    return nearest;
}
