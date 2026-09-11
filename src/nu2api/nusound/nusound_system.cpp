#include "nu2api/nusound/nusound_system.hpp"

#include "nu2api/nucore/nustring.h"
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

NuSoundBus *NuSoundSystem::sMasterBus = NULL;
i32 NuSoundSystem::sAllocdMemory[3] = {0};
i32 NuSoundSystem::sTotalMemory[3] = {0};
i32 NuSoundSystem::sPeakAllocdMemory[3] = {0};
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

NuSoundSystem::NuSoundSystem() {
    field_0xf8 = 0;
    samples = NULL;
    sample_count = 0x100;
    initialised = true;
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
        s_mmDecoder = NU_ALLOC_T(NuSoundMemoryManager, 1, "", 0);
        if (s_mmDecoder != NULL) {
            new (s_mmDecoder) NuSoundMemoryManager{};
        }

        s_mmDecoder->Init("decoder", sDecoderMemory, sTotalMemory[(i32)MemoryDiscipline::DECODER], 4, 0x800);
    }

    s_mmSample = NU_ALLOC_T(NuSoundMemoryManager, 1, "", 0);
    if (s_mmSample != NULL) {
        new (s_mmSample) NuSoundMemoryManager{};
    }

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
    NuSoundBus *bus = GetBus(name);

    if (bus == NULL) {
        bus = (NuSoundBus *)_AllocMemory(NuSoundSystem::MemoryDiscipline::SCRATCH, sizeof(NuSoundBus), 4,
                                         "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1180");

        if (bus != NULL) {
            new (bus) NuSoundBus(name, is_master);
            bus_list.PushBack(bus);
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

    // NuSoundSample and NuSoundDecoder share the intrusive links at +0x20.
    // The system's first offset list is the target's global sample/source
    // list, despite the provisional decoder_list name in this reconstruction.
    this->decoder_list.PushBack(reinterpret_cast<NuSoundDecoder *>(sample));
    i32 hash = GenerateHash(buf);
    sample->next = this->samples[hash];
    this->samples[hash] = sample;

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

    byte hash = 0x5;

    for (char *c = buf; *c != '\0'; c++) {
        hash = (hash * 0x21) + *c;
    }

    return hash;
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

bool NuSoundStreamDesc::DecodeStreamOnOpen() const {
    return false;
}

i32 NuSoundStreamDesc::GetLoopStart() const {
    return 0;
}

i32 NuSoundStreamDesc::GetLoopEnd() const {
    return 0;
}

bool NuSoundSystem::AddListener(NuSoundListener *listener) {
    return this->listener_list.PushBack(listener);
}

void NuSoundSystem::AddRoutingTable(NuSoundRoutingTable *table) {
    routing_table_list.PushBack(table);
}

f32 NuSoundSystem::AmplitudeTodB(f32 amplitude) {
    if (amplitude <= 0.0f) {
        return -100.0f;
    }
    if (amplitude >= 1.0f) {
        return 0.0f;
    }
    return NuLog10(amplitude) * 20.0f;
}

f32 NuSoundSystem::CalculateCrossfadeHeight(NuSoundSystem::CurveData const &, float) const {
    return 0.0f;
}

NuSoundSystem::CurveData *NuSoundSystem::CreateCrossfadeCurve(u32 id) {
    return &crossfade_curves.InsertNode(id)->value;
}

// libTTapp.so 0x31a810: builds the "<name>_decoder" name from the source's
// name, then constructs the format-specific decoder. Only OGG streams
// (encoded format 3) get a decoder; anything else returns NULL and plays
// through the plain sample path.
NuSoundDecoder *NuSoundSystem::CreateDecoder(NuSoundSource *source) {
    const char *name = source->GetName();

    char decoded_name[256];
    u32 name_len = (u32)strlen(name);
    if (name_len >= sizeof(decoded_name) - 9) {
        name_len = sizeof(decoded_name) - 9;
    }
    memcpy(decoded_name, name, name_len);
    memcpy(decoded_name + name_len, "_decoder", 9);

    NuSoundStreamDesc *desc = source->GetStreamDesc();
    if (desc != NULL && desc->GetEncodedDataFormat() == NuSoundStreamDesc::DataFormat::THREE) {
        NuSoundDecoderOGG *decoder = (NuSoundDecoderOGG *)NuSoundSystem::_AllocMemory(
            NuSoundSystem::MemoryDiscipline::SCRATCH, sizeof(NuSoundDecoderOGG), 4,
            "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound_system.cpp:436");

        if (decoder != NULL) {
            new (decoder) NuSoundDecoderOGG(decoded_name, source);
        }

        return decoder;
    }

    return NULL;
}

NuSoundEffect *NuSoundSystem::CreateEffect(NuSoundEffect::EffectType type) {
    NuSoundEffect *effect = NULL;

    switch (type) {
        case NuSoundEffect::EffectType::ATTENUATION: {
            void *memory = _AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectAttenuation), 4,
                                        "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1094");
            if (memory != NULL)
                effect = new (memory) NuSoundEffectAttenuation();
            break;
        }
        case NuSoundEffect::EffectType::PITCH: {
            void *memory = _AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectPitch), 4,
                                        "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1100");
            if (memory != NULL)
                effect = new (memory) NuSoundEffectPitch();
            break;
        }
        case NuSoundEffect::EffectType::FADER: {
            void *memory = _AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectFader), 4,
                                        "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1106");
            if (memory != NULL)
                effect = new (memory) NuSoundEffectFader();
            break;
        }
        case NuSoundEffect::EffectType::PITCH_RAMP: {
            void *memory = _AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectPitchRamp), 4,
                                        "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1112");
            if (memory != NULL)
                effect = new (memory) NuSoundEffectPitchRamp();
            break;
        }
        case NuSoundEffect::EffectType::RANDOM_VOLUME: {
            void *memory = _AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectRandomVolume), 4,
                                        "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1118");
            if (memory != NULL)
                effect = new (memory) NuSoundEffectRandomVolume();
            break;
        }
        case NuSoundEffect::EffectType::RANDOM_PITCH: {
            void *memory = _AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectRandomPitch), 4,
                                        "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1124");
            if (memory != NULL)
                effect = new (memory) NuSoundEffectRandomPitch();
            break;
        }
        case NuSoundEffect::EffectType::DOPPLER: {
            void *memory = _AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectDoppler), 4,
                                        "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1130");
            if (memory != NULL)
                effect = new (memory) NuSoundEffectDoppler();
            break;
        }
        case NuSoundEffect::EffectType::REPEAT: {
            void *memory = _AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectRepeat), 4,
                                        "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1136");
            if (memory != NULL)
                effect = new (memory) NuSoundEffectRepeat();
            break;
        }
        default:
            break;
    }

    if (effect != NULL) {
        effect->Initialise();
        NuSoundMemory::PushNuListNode(effect_update_list, effect);
    }
    return effect;
}

void NuSoundSystem::DefragmentSampleMemory() {
    s_mmSample->Defragment(0);
}

NuSoundSystem::FileType NuSoundSystem::DetermineFileType(NUFILETYPE type) {
    char extension[6] = {};
    NuFileExtGetExt(extension, sizeof(extension), type);
    return DetermineFileType(extension);
}

void NuSoundSystem::Disable() {
    sNumAvailableOutputDevices = 0;
}

bool NuSoundSystem::FileTypeSupported(NuSoundSystem::FileType type) {
    switch (type) {
        case FileType::WAV:
        case FileType::OGG:
            return true;
        default:
            return false;
    }
}

NuSoundSystem *NuSoundSystem::Get() {
    return s_staticInstance;
}

u32 NuSoundSystem::GetAllocdMemory(NuSoundSystem::MemoryDiscipline discipline) {
    return sAllocdMemory[static_cast<u32>(discipline)];
}

u32 NuSoundSystem::GetBufferAlignment() {
    return 0x20;
}

i32 NuSoundSystem::GetClosestSupportedConfig(i32 config) {
    // libTTapp.so 0x31bcb0: config > 7 -> 8, config >= 6 -> 6, else 2.
    if (config > 7) {
        return 8;
    }
    return (config >= 6) ? 6 : 2;
}

const NuSoundSystem::CurveData *NuSoundSystem::GetCrossfadeCurve(u32 id) const {
    const NuMapNode<u32, CurveData> *node = crossfade_curves.FindNode(id);
    if (node != NULL) {
        return &node->value;
    }
    return NULL;
}

NuSoundSystem::FileType NuSoundSystem::GetDefaultFileType(NuSoundSource::FeedType feed_type) {
    if (feed_type == NuSoundSource::FeedType::ZERO) {
        return DetermineFileType(NUFILETYPE_SFX);
    }
    if (feed_type == NuSoundSource::FeedType::STREAMING) {
        return DetermineFileType(NUFILETYPE_SOUNDSTREAM);
    }
    return FileType::INVALID;
}

NuSoundRoutingTable *NuSoundSystem::GetDefaultRoutingTable() {
    return sDefaultRoutingTable;
}

u32 NuSoundSystem::GetGfxMemorySize() {
    if (sGfxMemorySize == 0 || sGfxMemorySize <= GetScratchMemorySize()) {
        return 0x600000;
    }
    return sGfxMemorySize - GetScratchMemorySize();
}

const char *NuSoundSystem::GetLanguageString(bool) {
    return "Eng";
}

u32 NuSoundSystem::GetLargestMemoryFragment(NuSoundSystem::MemoryDiscipline discipline) {
    if (discipline == MemoryDiscipline::SCRATCH) {
        return sScratchMemMgr->CalculateLargestFragmentSize();
    }
    return 0;
}

NuEList<NuSoundListener, DefaultElist> *NuSoundSystem::GetListeners() {
    return &this->listener_list;
}

NuSoundListener *NuSoundSystem::GetNearestRealListener(NuEList<NuSoundListener, DefaultElist> const &listeners,
                                                       VuVec const &position) {
    f32 nearest_distance = FLT_MAX;
    NuSoundListener *nearest = NULL;
    NuSoundListener *listener = static_cast<NuSoundListener *>(listeners.begin->field_0x4);

    while (listener != listeners.end) {
        if (listener->IsEnabled() && listener->GetSensitivity() > 0.0f) {
            f32 distance = listener->GetHeadDistance(position) / listener->GetSensitivity();
            if (nearest_distance > distance || nearest == NULL) {
                nearest_distance = distance;
                nearest = listener;
            }
        }
        listener = static_cast<NuSoundListener *>(listener->field_0x4);
    }

    return nearest;
}

NuSoundListener *NuSoundSystem::GetNearestFocusListener(NuEList<NuSoundListener, DefaultElist> const &listeners,
                                                        VuVec const &position, f32 &nearest_distance) {
    nearest_distance = FLT_MAX;
    NuSoundListener *nearest = NULL;
    NuSoundListener *listener = static_cast<NuSoundListener *>(listeners.begin->field_0x4);

    while (listener != listeners.end) {
        if (listener->IsEnabled() && listener->GetSensitivity() > 0.0f) {
            f32 distance = listener->GetAttenuationDistance(position) / listener->GetSensitivity();
            if (nearest_distance > distance || nearest == NULL) {
                nearest_distance = distance;
                nearest = listener;
            }
        }
        listener = static_cast<NuSoundListener *>(listener->field_0x4);
    }

    return nearest;
}

i32 NuSoundSystem::GetNumAvailableOutputDevices() {
    return sNumAvailableOutputDevices;
}

NuSoundVoice *NuSoundSystem::GetOldestVoice(NuSoundSample *sample, float &playback_position) {
    playback_position = -1.0f;
    NuSoundVoice *oldest = NULL;
    for (NuSoundVoice *voice = voice_list.Front(); voice != voice_list.End(); voice = voice->field_0x28) {
        if (voice->GetState() != NuSoundVoice::PLAYSTATE_PLAYING || voice->sound_source != sample) {
            continue;
        }
        f32 position = voice->GetPlaybackPositionSeconds();
        if (playback_position < 0.0f || playback_position < position) {
            playback_position = position;
            oldest = voice;
        }
    }
    return oldest;
}

i32 NuSoundSystem::GetOutputChannelConfig() {
    return sOutputConfig;
}

u32 NuSoundSystem::GetPeakAllocdMemory(NuSoundSystem::MemoryDiscipline discipline) {
    return sPeakAllocdMemory[static_cast<u32>(discipline)];
}

const char *NuSoundSystem::GetPlatformString() {
    return "ANDROID";
}

NuSoundVoice *NuSoundSystem::GetQuietestVoice(NuSoundSample *sample, float &quietest_volume) {
    quietest_volume = -1.0f;
    NuSoundVoice *quietest = NULL;

    for (NuSoundVoice *voice = voice_list.Front(); voice != voice_list.End(); voice = voice->field_0x28) {
        if (voice->GetState() != NuSoundVoice::PLAYSTATE_PLAYING ||
            voice->sound_source->GetName() != sample->GetName()) {
            continue;
        }

        f32 volume = voice->field67_0xa8 * voice->GetVolume();
        if (quietest_volume < 0.0f || quietest_volume > volume) {
            quietest_volume = volume;
            quietest = voice;
        }
    }

    return quietest;
}

NuSoundRoutingTable *NuSoundSystem::GetRoutingTable(char const *name) {
    for (NuSoundRoutingTable *table = routing_table_list.Front(); table != routing_table_list.End();
         table = reinterpret_cast<NuSoundRoutingTable **>(table)[1]) {
        if (NuStrICmp(table->GetName(), name) == 0) {
            return table;
        }
    }
    return NULL;
}

u32 NuSoundSystem::GetTotalMemory(NuSoundSystem::MemoryDiscipline discipline) {
    return sTotalMemory[static_cast<u32>(discipline)];
}

bool NuSoundSystem::LoadSample(NuSoundSample *sample, void *data, i32 size, NuSoundOutOfMemCallback *callback) {
    NuSoundSample::LoadState state = sample->GetLoadState();
    if (state != NuSoundSample::LoadState::NOT_LOADED) {
        return state == NuSoundSample::LoadState::LOADED;
    }

    if (sample->Load(data, size, callback) != NuSoundSample::ErrorState::NONE) {
        return false;
    }

    this->field_0xf8++;
    return sample->GetLoadState() == NuSoundSample::LoadState::LOADED;
}

void NuSoundSystem::PauseAllVoices() {
    for (NuSoundVoice *voice = voice_list.Front(); voice != voice_list.End(); voice = voice->field_0x28) {
        voice->Pause();
    }
}

void NuSoundSystem::PauseVoices(i32 mask) {
    for (NuSoundVoice *voice = voice_list.Front(); voice != voice_list.End(); voice = voice->field_0x28) {
        if ((voice->field131_0x148 & mask) != 0) {
            voice->Pause();
        }
    }
}

void *NuSoundSystem::ReAllocMemory(NuSoundSystem::MemoryDiscipline encoded_address, u32 size, u32 unused) {
    (void)unused;
    void *address = reinterpret_cast<void *>(static_cast<usize>(encoded_address));
    NuMemoryManager *manager = NuMemoryGet()->GetThreadMem();
    u32 old_size = manager->GetBlockSize(address);
    u32 alignment = manager->GetBlockAlignment(address);
    if (old_size < size) {
        address = manager->_BlockReAlloc(address, size, alignment, 0, NULL, 0);
    }
    return address;
}

void NuSoundSystem::ReleaseBus(NuSoundBus *bus) {
    this->bus_list.Remove(bus);
    bus->~NuSoundBus();
    FreeMemory(MemoryDiscipline::SCRATCH, reinterpret_cast<usize>(bus), 0);
}

void NuSoundSystem::ReleaseCrossfadeCurve(u32 id) {
    crossfade_curves.Erase(id);
}

void NuSoundSystem::ReleaseDecoder(NuSoundDecoder *decoder) {
    // libTTapp.so 0x31aafb: invoke vtable slot 0 (the complete-object
    // destructor) without using the deleting-destructor slot; the scratch
    // allocator owns the storage release below.
    decoder->~NuSoundDecoder();
    FreeMemory(MemoryDiscipline::SCRATCH, reinterpret_cast<usize>(decoder), 0);
}

void NuSoundSystem::ReleaseEffect(NuSoundEffect *effect) {
    if (effect == NULL) {
        return;
    }

    if (effect_update_list.Length() != 0) {
        effect_update_list.RemoveValue(effect);
    }
    sAllocdMemory[static_cast<i32>(MemoryDiscipline::SCRATCH)] -= sizeof(NuEListNode<NuSoundEffect>);

    effect->Shutdown();
    effect->~NuSoundEffect();
    FreeMemory(MemoryDiscipline::SCRATCH, reinterpret_cast<usize>(effect), 0);
}

bool NuSoundSystem::IsUserPlayingMusic() {
    return false;
}

void NuSoundSystem::PauseUserMusic() {
}

void NuSoundSystem::ResumeUserMusic() {
}

bool NuSoundSystem::TitleHasUserMusicControl() {
    return false;
}

void NuSoundSystem::OnEnterSystemMenu() {
}

void NuSoundSystem::OnExitSystemMenu() {
}

void NuSoundSystem::ReleaseSample(NuSoundSample *sample) {
    this->decoder_list.Remove(reinterpret_cast<NuSoundDecoder *>(sample));

    sample->~NuSoundSample();
    FreeMemory(MemoryDiscipline::SCRATCH, reinterpret_cast<usize>(sample), 0);
}

void NuSoundSystem::RemoveListener(NuSoundListener *listener) {
    NuSoundListener *next = static_cast<NuSoundListener *>(listener->field_0x4);
    NuSoundListener *previous = static_cast<NuSoundListener *>(listener->field_0x0);

    if (next == NULL) {
        if (previous == NULL) {
            return;
        }
        this->listener_list.length--;
        previous->field_0x4 = next;
    } else {
        this->listener_list.length--;
        if (previous != NULL) {
            previous->field_0x4 = next;
        }
        next->field_0x0 = previous;
    }

    listener->field_0x4 = NULL;
    listener->field_0x0 = NULL;
}

void NuSoundSystem::ResumeAllVoices() {
    for (NuSoundVoice *voice = voice_list.Front(); voice != voice_list.End(); voice = voice->field_0x28) {
        voice->Resume();
    }
}

void NuSoundSystem::ResumeVoices(i32 mask) {
    for (NuSoundVoice *voice = voice_list.Front(); voice != voice_list.End(); voice = voice->field_0x28) {
        if ((voice->field131_0x148 & mask) != 0) {
            voice->Resume();
        }
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
    NuSoundVoice *voice = this->voice_list.Front();
    while (voice != this->voice_list.End()) {
        NuSoundVoice *next = voice->field_0x28;
        voice->Stop(false);
        this->ReleaseVoice(voice);
        voice = next;
    }

    this->UnloadAllSamples();

    NuSoundDecoder *entry = this->decoder_list.Front();
    while (entry != this->decoder_list.End()) {
        NuSoundDecoder *next = *reinterpret_cast<NuSoundDecoder **>(reinterpret_cast<u8 *>(entry) + 0x24);
        entry->~NuSoundDecoder();
        FreeMemory(MemoryDiscipline::SCRATCH, reinterpret_cast<usize>(entry), 0);
        entry = next;
    }

    NuSoundBus *bus = this->bus_list.Front();
    while (bus != this->bus_list.End()) {
        NuSoundBus *next = reinterpret_cast<NuSoundBus **>(bus)[1];
        this->ReleaseBus(bus);
        bus = next;
    }

    FreeMemory(MemoryDiscipline::SCRATCH, reinterpret_cast<usize>(this->samples), 0);
    this->samples = NULL;

    if (s_mmDecoder != NULL) {
        s_mmDecoder->~NuSoundMemoryManager();
        NU_FREE(s_mmDecoder);
    }
    if (s_mmSample != NULL) {
        s_mmSample->~NuSoundMemoryManager();
        NU_FREE(s_mmSample);
    }

    NuMemoryGet()->DestroyMemoryManager(sScratchMemMgr);
    NU_FREE(sDecoderMemory);
    NU_FREE(sSampleMemory);
    NU_FREE(sScratchMemory);

    this->ShutdownAudioDevice();
}

bool NuSoundSystem::SourceRequiresDecoder(NuSoundSource *source) {
    if (source->GetStreamDesc()->GetEncodedDataFormat() != source->GetStreamDesc()->GetDecodedDataFormat() &&
        !source->GetStreamDesc()->DecodeStreamOnOpen()) {
        if (NuStrIStr(const_cast<char *>(source->GetName()), "coin") != NULL ||
            NuStrIStr(const_cast<char *>(source->GetName()), "counter") != NULL ||
            NuStrIStr(const_cast<char *>(source->GetName()), "fs_") != NULL) {
            return false;
        }
        return NuStrIStr(const_cast<char *>(source->GetName()), "saber") == NULL;
    }
    return false;
}

template <typename T> void NuSoundMemory::PushNuListNode(NuList<T> &list, T const &value) {
    NuMemoryManager *previous = NuMemoryGet()->SetThreadMem(NuSoundSystem::sScratchMemMgr);
    NuListNode<T> *node = static_cast<NuListNode<T> *>(NuMemoryGet()->GetThreadMem()->_BlockAlloc(
        sizeof(NuListNode<T>), 4, NuMemoryManager::MEM_ALLOC_SET_TO_ZERO, "_new", NUMEMORY_CATEGORY_NONE));
    if (node != NULL) {
        node->SetPrev(NULL);
        node->SetNext(NULL);
        node->value = value;
    }
    list.Append(node);
    NuMemoryGet()->SetThreadMem(previous);
    NuSoundSystem::sAllocdMemory[(i32)NuSoundSystem::MemoryDiscipline::SCRATCH] += sizeof(NuListNode<T>);
}

template void NuSoundMemory::PushNuListNode<NuSoundEffect *>(NuList<NuSoundEffect *> &, NuSoundEffect *const &);

void NuSoundSystem::StopAllVoices() {
    for (NuSoundVoice *voice = voice_list.Front(); voice != voice_list.End(); voice = voice->field_0x28) {
        voice->Stop(true);
    }
}

void NuSoundSystem::StopVoices(NuSoundSource const &source) {
    for (NuSoundVoice *voice = voice_list.Front(); voice != voice_list.End(); voice = voice->field_0x28) {
        if (voice->sound_source == &source) {
            voice->Stop(false);
        }
    }
}

void NuSoundSystem::StopVoices(i32 mask) {
    for (NuSoundVoice *voice = voice_list.Front(); voice != voice_list.End(); voice = voice->field_0x28) {
        if ((voice->field131_0x148 & mask) != 0) {
            voice->Stop(false);
        }
    }
}

void NuSoundSystem::UnloadAllSamples() {
    NuSoundDecoder *entry = decoder_list.Front();
    while (entry != decoder_list.End()) {
        NuSoundSample *sample = reinterpret_cast<NuSoundSample *>(entry);
        entry = *reinterpret_cast<NuSoundDecoder **>(reinterpret_cast<u8 *>(entry) + 0x24);
        if (sample->GetLoadState() == NuSoundSample::LoadState::LOADED) {
            UnloadSample(sample);
        }
    }
}

bool NuSoundSystem::UnloadSample(NuSoundSample *sample) {
    if (sample == NULL || sample->field_0x18 != 0 || sample->GetLoadState() == NuSoundSample::LoadState::NOT_LOADED) {
        return false;
    }
    sample->Unload();
    return true;
}

void NuSoundSystem::Update(f32 frametime) {
    if (this->initialised == false) {
        return;
    }

    // Platform hook (on Android this only polls the application state).
    this->UpdateAudioDevice();

    this->mutex.Lock();

    // Pass 1: drive every platform voice's device state.
    for (NuListNodeBase *node = effect_update_list.Head(); node != effect_update_list.Tail(); node = node->GetNext()) {
        static_cast<NuListNode<NuSoundEffect *> *>(node)->value->Process(frametime);
    }

    // Pass 2: update the engine-side mix of every playing voice; stopped
    // auto-delete voices are released.
    NuSoundVoice *voice = voice_list.Front();
    while (voice != voice_list.End()) {
        NuSoundVoice *next = voice->field_0x28;

        NuSoundVoice::PlayState state = voice->GetState();
        if (state == NuSoundVoice::PLAYSTATE_PLAYING) {
            pthread_mutex_lock(&NuSoundWeakPtrListNode::sPtrAccessLock.mutex);
            voice->Update(frametime);
            pthread_mutex_unlock(&NuSoundWeakPtrListNode::sPtrAccessLock.mutex);
        } else if (state == NuSoundVoice::PLAYSTATE_STOPPED && voice->GetAutoDelete()) {
            this->ReleaseVoice(voice);
        }

        voice = next;
    }

    this->mutex.Unlock();
}

f32 NuSoundSystem::dBToAmplitude(f32 db) {
    if (db <= -100.0f) {
        return 0.0f;
    }
    if (db >= 0.0f) {
        return 1.0f;
    }
    return NuExp10(db / 20.0f);
}

NuSoundVoice *NuSoundSystem::CreateVoice(NuSoundSource *source, bool loop) {
    NuSoundDecoder *decoder = NULL;
    NuSoundSource *voice_source = source;
    if (this->SourceRequiresDecoder(source)) {
        decoder = this->CreateDecoder(source);
        decoder->OpenStream(loop);
        if (decoder->IsStreamOpen() == false) {
            this->ReleaseDecoder(decoder);
            return NULL;
        }
        voice_source = decoder;
    } else {
        if (source->IsStreamOpen() == false) {
            return NULL;
        }
    }

    NuSoundStreamDesc *desc = voice_source->GetStreamDesc();
    NuSoundVoiceFactory *factory = this->factory_list.GetFactory(desc->GetDecodedDataFormat());
    NuSoundVoice *voice = factory->CreateVoice(voice_source, loop);
    if (voice == NULL) {
        if (decoder != NULL) {
            decoder->CloseStream();
            this->ReleaseDecoder(decoder);
        }
        return NULL;
    }

    this->mutex.Lock();
    NuSoundWeakPtrListNode::sPtrAccessLock.Lock();
    voice_list.PushBack(voice);
    NuSoundWeakPtrListNode::sPtrAccessLock.Unlock();
    this->mutex.Unlock();

    return voice;
}

void NuSoundSystem::ReleaseVoice(NuSoundVoice *voice) {
    this->mutex.Lock();

    // Detach effects (releasing the ones the system owns).
    for (NuListNodeBase *node = voice->effects.Head(), *end = voice->effects.Tail(); node != end;) {
        NuSoundEffect *effect = static_cast<NuListNode<NuSoundEffect *> *>(node)->value;
        NuListNodeBase *next = node->GetNext();
        voice->RemoveEffect(effect);
        if (effect->keep_attached) {
            this->ReleaseEffect(effect);
        }
        node = next;
    }

    // Streaming sources opened through a decoder close their stream here.
    NuSoundDecoder *decoder = NULL;
    if (this->SourceRequiresDecoder(voice->sound_source)) {
        decoder = (NuSoundDecoder *)voice->sound_source;
    }

    // libTTapp.so 0x31b348: keep the voice's callback vtable and weak-pointer
    // head alive while it is detached and destroyed. The streaming worker
    // holds this same lock across SubmitBuffer.
    NuSoundWeakPtrListNode::sPtrAccessLock.Lock();

    voice_list.Remove(voice);

    // libTTapp.so 0x31b394: run the voice's complete destructor (vtable slot
    // 0, no free), then hand the block back through FreeMemory(SCRATCH).
    voice->~NuSoundVoice();
    NuSoundSystem::FreeMemory(NuSoundSystem::MemoryDiscipline::SCRATCH, (usize)voice, 0);

    NuSoundWeakPtrListNode::sPtrAccessLock.Unlock();

    if (decoder != NULL) {
        decoder->CloseStream();
        this->ReleaseDecoder(decoder);
    }

    this->mutex.Unlock();
}

void NuSound3ExitThreads() {
}

u32 NuSound_GetAllocdSampleMemory() {
    return NuSoundSystem::GetAllocdMemory(NuSoundSystem::MemoryDiscipline::SAMPLE);
}
