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
#include "nu2api/nusound/nusound_loader.hpp"
#include "nu2api/nusound/nusound_loader_ogg.hpp"
#include "nu2api/nusound/nusound_streamer.hpp"
#include "nu2api/nusound/nusound_voice.hpp"
#include "nu2api_nusound_types.h"

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
NuSoundSystemCallbacks NuSoundSystem::g_handler;
const char *NuSoundSystem::sFileExtensions[12] = {"wav", "adp", "ima", "caf", "xma", "ogg",
                                                  "dsp", "msf", "vag", "gcm", "wua", "cbx"};
NuSoundSystem *NuSoundSystem::s_staticInstance = NULL;
NuSoundRoutingTable *NuSoundSystem::sDefaultRoutingTable = NULL;
i32 NuSoundSystem::sNumAvailableOutputDevices = 0;
i32 NuSoundSystem::sOutputConfig = 2;
NuMemoryManager *NuSoundSystem::sScratchMemMgr = NULL;

NuSoundBus *NuSoundSystem::GetBus(const char *name) {
    NuSoundBus *bus = bus_list.Front();
    NuSoundBus *end = bus_list.End();
    for (; bus != end; bus = reinterpret_cast<NuSoundBus **>(bus)[1]) {
        if (NuStrICmp(bus->GetName(), name) == 0) {
            return bus;
        }
    }
    return NULL;
}

NuSoundVoiceFactoryList::NuSoundVoiceFactoryList() {
    factories = NULL;
    length = 0;
    capacity = 0;
    NuSoundVoiceFactory **replacement =
        static_cast<NuSoundVoiceFactory **>(NuMemoryGet()->GetThreadMem()->_BlockReAlloc(
            factories, 16 * sizeof(NuSoundVoiceFactory *), 4, 0x41, "", NUMEMORY_CATEGORY_NONE));
    if (replacement != factories) {
        u32 previous_capacity = capacity;
        for (u32 i = 0; i < previous_capacity; ++i) {
            replacement[i] = factories[i];
        }
        NuMemoryGet()->GetThreadMem()->BlockFree(factories, 0);
    }
    length = 16;
    factories = replacement;
    capacity = 16;
}

void NuSoundVoiceFactoryList::RegisterFactory(NuSoundVoiceFactory *factory, NuSoundStreamDesc::DataFormat format) {
    this->factories[(u32)format] = factory;
}

NuSoundVoiceFactory *NuSoundVoiceFactoryList::GetFactory(NuSoundStreamDesc::DataFormat format) {
    return this->factories[(u32)format];
}

void NuSoundSystem::SetGfxMemorySize(u32 size) {
    sGfxMemorySize = size;
}

u32 NuSoundSystem::GetScratchMemorySize() {
    return 0x4b000;
}

u32 NuSoundSystem::GetGfxMemorySize() {
    if (sGfxMemorySize == 0 || sGfxMemorySize <= GetScratchMemorySize()) {
        return 0x600000;
    }
    return sGfxMemorySize - GetScratchMemorySize();
}

u32 NuSoundSystem::GetStreamBufferSize() {
    return 0x20000;
}

u32 NuSoundSystem::GetBufferAlignment() {
    return 0x20;
}

u32 NuSoundSystem::GetDecoderMemorySize() {
    return 0x100000;
}

NuSoundSystem::NuSoundSystem() {
    samples = NULL;
    field_0xf8 = 0;
    sample_count = 0x100;
    initialised = true;
    s_staticInstance = this;
}

NuSoundSystem *NuSoundSystem::Get() {
    return s_staticInstance;
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

// libTTapp.so 0x319640: SAMPLE/DECODER hand the block header back to the
// pool manager; SCRATCH goes through NuMemoryManager::BlockFree and accounts
// the queried block size.
u32 NuSoundSystem::FreeMemory(MemoryDiscipline disc, usize address, u32 size) {
    switch (disc) {
        case MemoryDiscipline::SCRATCH:
            size = sScratchMemMgr->GetBlockSize((void *)address);
            sScratchMemMgr->BlockFree((void *)address, 0);
            break;
        case MemoryDiscipline::SAMPLE:
            s_mmSample->Free((NuSoundMemoryBuffer *)address);
            break;
        case MemoryDiscipline::DECODER:
            s_mmDecoder->Free((NuSoundMemoryBuffer *)address);
            break;
    }

    sAllocdMemory[(i32)disc] -= size;
    return size;
}

void NuSoundSystem::ReleaseBus(NuSoundBus *bus) {
    this->bus_list.Remove(bus);
    bus->~NuSoundBus();
    FreeMemory(MemoryDiscipline::SCRATCH, reinterpret_cast<usize>(bus), 0);
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

u32 NuSoundSystem::GetAllocdMemory(NuSoundSystem::MemoryDiscipline discipline) {
    return sAllocdMemory[static_cast<u32>(discipline)];
}

u32 NuSoundSystem::GetFreeMemory(MemoryDiscipline disc) {
    return sTotalMemory[(i32)disc] - sAllocdMemory[(i32)disc];
}

void *NuSoundSystem::_AllocMemory(MemoryDiscipline disc, u32 size, u32 align, const char *name) {
    if (__builtin_expect(GetFreeMemory(disc) < size, 0)) {
        return NULL;
    }

    void *ptr = NULL;
    switch (disc) {
        case MemoryDiscipline::SAMPLE:
            ptr = s_mmSample->Alloc(size);
            break;
        case MemoryDiscipline::DECODER:
            ptr = s_mmDecoder->Alloc(size);
            break;
        case MemoryDiscipline::SCRATCH:
            ptr = sScratchMemMgr->_TryBlockAlloc(size, align, 1, name, 0);
            break;
    }

    if (ptr != NULL) {
        sAllocdMemory[(i32)disc] += size;
    }

    return ptr;
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
        NuSoundMemoryManager *decoder = NU_ALLOC_T(NuSoundMemoryManager, 1, "", 0);
        if (decoder != NULL) {
            new (decoder) NuSoundMemoryManager{};
        }
        s_mmDecoder = decoder;

        decoder->Init("decoder", sDecoderMemory, sTotalMemory[(i32)MemoryDiscipline::DECODER], 4, 0x800);
    }

    NuSoundMemoryManager *sample = NU_ALLOC_T(NuSoundMemoryManager, 1, "", 0);
    if (sample != NULL) {
        new (sample) NuSoundMemoryManager{};
    }
    s_mmSample = sample;

    sample->EnableDefragOnAlloc(true);
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

u32 NuSoundSystem::GetTotalMemory(NuSoundSystem::MemoryDiscipline discipline) {
    return sTotalMemory[static_cast<u32>(discipline)];
}

u32 NuSoundSystem::GetPeakAllocdMemory(NuSoundSystem::MemoryDiscipline discipline) {
    return sPeakAllocdMemory[static_cast<u32>(discipline)];
}

u32 NuSoundSystem::GetLargestMemoryFragment(NuSoundSystem::MemoryDiscipline discipline) {
    if (discipline == MemoryDiscipline::SCRATCH) {
        return sScratchMemMgr->CalculateLargestFragmentSize();
    }
    return 0;
}

void NuSoundSystem::DefragmentSampleMemory() {
    s_mmSample->Defragment(0);
}

i32 NuSoundSystem::GenerateHash(const char *str) {
    char buf[0x100];
    NuStrUpr(buf, str);

    i32 hash = 0x5;

    for (i8 *c = (i8 *)buf; *c != '\0'; c++) {
        hash = (hash * 0x21) + *c;
    }

    return (u8)hash;
}

void NuSoundSystem::ReleaseSample(NuSoundSample *sample) {
    this->decoder_list.Remove(reinterpret_cast<NuSoundDecoder *>(sample));

    sample->~NuSoundSample();
    FreeMemory(MemoryDiscipline::SCRATCH, reinterpret_cast<usize>(sample), 0);
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

bool NuSoundSystem::UnloadSample(NuSoundSample *sample) {
    if (sample == NULL || sample->field_0x18 != 0 || sample->GetLoadState() == NuSoundSample::LoadState::NOT_LOADED) {
        return false;
    }
    sample->Unload();
    return true;
}

void NuSoundSystem::UnloadAllSamples() {
    typedef NuEListOffset<NuSoundDecoder, 0x20> SampleList;
    SampleList::Link *link = decoder_list.FrontLink();
    SampleList::Link *end = decoder_list.EndLink();
    for (; link != end; link = SampleList::NextLink(link)) {
        NuSoundSample *sample = reinterpret_cast<NuSoundSample *>(SampleList::EntryOf(link));
        if (sample->GetLoadState() == NuSoundSample::LoadState::LOADED) {
            UnloadSample(sample);
        }
    }
}

void NuSoundSystem::StopAllVoices() {
    typedef NuEListOffset<NuSoundVoice, 0x24> VoiceList;
    VoiceList::Link *link = voice_list.FrontLink();
    VoiceList::Link *end = voice_list.EndLink();
    for (; link != end; link = VoiceList::NextLink(link)) {
        NuSoundVoice *voice = VoiceList::EntryOf(link);
        voice->Stop(true);
    }
}

void NuSoundSystem::StopVoices(i32 mask) {
    typedef NuEListOffset<NuSoundVoice, 0x24> VoiceList;
    VoiceList::Link *link = voice_list.FrontLink();
    VoiceList::Link *end = voice_list.EndLink();
    for (; link != end; link = VoiceList::NextLink(link)) {
        NuSoundVoice *voice = VoiceList::EntryOf(link);
        if ((voice->field131_0x148 & mask) != 0) {
            voice->Stop(false);
        }
    }
}

void NuSoundSystem::StopVoices(NuSoundSource const &source) {
    typedef NuEListOffset<NuSoundVoice, 0x24> VoiceList;
    VoiceList::Link *link = voice_list.FrontLink();
    VoiceList::Link *end = voice_list.EndLink();
    for (; link != end; link = VoiceList::NextLink(link)) {
        NuSoundVoice *voice = VoiceList::EntryOf(link);
        if (voice->sound_source == &source) {
            voice->Stop(false);
        }
    }
}

void NuSoundSystem::PauseVoices(i32 mask) {
    typedef NuEListOffset<NuSoundVoice, 0x24> VoiceList;
    VoiceList::Link *link = voice_list.FrontLink();
    VoiceList::Link *end = voice_list.EndLink();
    for (; link != end; link = VoiceList::NextLink(link)) {
        NuSoundVoice *voice = VoiceList::EntryOf(link);
        if ((voice->field131_0x148 & mask) != 0) {
            voice->Pause();
        }
    }
}

void NuSoundSystem::PauseAllVoices() {
    typedef NuEListOffset<NuSoundVoice, 0x24> VoiceList;
    VoiceList::Link *link = voice_list.FrontLink();
    VoiceList::Link *end = voice_list.EndLink();
    for (; link != end; link = VoiceList::NextLink(link)) {
        NuSoundVoice *voice = VoiceList::EntryOf(link);
        voice->Pause();
    }
}

void NuSoundSystem::ResumeVoices(i32 mask) {
    typedef NuEListOffset<NuSoundVoice, 0x24> VoiceList;
    VoiceList::Link *link = voice_list.FrontLink();
    VoiceList::Link *end = voice_list.EndLink();
    for (; link != end; link = VoiceList::NextLink(link)) {
        NuSoundVoice *voice = VoiceList::EntryOf(link);
        if ((voice->field131_0x148 & mask) != 0) {
            voice->Resume();
        }
    }
}

void NuSoundSystem::ResumeAllVoices() {
    typedef NuEListOffset<NuSoundVoice, 0x24> VoiceList;
    VoiceList::Link *link = voice_list.FrontLink();
    VoiceList::Link *end = voice_list.EndLink();
    for (; link != end; link = VoiceList::NextLink(link)) {
        NuSoundVoice *voice = VoiceList::EntryOf(link);
        voice->Resume();
    }
}

NuSoundVoice *NuSoundSystem::GetOldestVoice(NuSoundSample *sample, float &playback_position) {
    playback_position = -1.0f;
    NuSoundVoice *oldest = NULL;
    typedef NuEListOffset<NuSoundVoice, 0x24> VoiceList;
    VoiceList::Link *link = voice_list.FrontLink();
    VoiceList::Link *end = voice_list.EndLink();
    for (; link != end; link = VoiceList::NextLink(link)) {
        NuSoundVoice *voice = VoiceList::EntryOf(link);
        if (voice->GetState() != NuSoundVoice::PLAYSTATE_PLAYING ||
            voice->sound_source->GetName() != sample->GetName()) {
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

NuSoundVoice *NuSoundSystem::GetQuietestVoice(NuSoundSample *sample, float &quietest_volume) {
    quietest_volume = -1.0f;
    NuSoundVoice *quietest = NULL;

    typedef NuEListOffset<NuSoundVoice, 0x24> VoiceList;
    VoiceList::Link *link = voice_list.FrontLink();
    VoiceList::Link *end = voice_list.EndLink();
    for (; link != end; link = VoiceList::NextLink(link)) {
        NuSoundVoice *voice = VoiceList::EntryOf(link);
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

NuSoundLoader *NuSoundSystem::CreateFileLoader(FileType type) {
    NuSoundLoaderWAV *wav_loader;

    switch (type) {
        case FileType::WAV:
            // libTTapp.so allocates the WAV loader from SCRATCH (0x1c bytes,
            // nusound.cpp:1233) and placement-news it in place.
            wav_loader =
                (NuSoundLoaderWAV *)_AllocMemory(NuSoundSystem::MemoryDiscipline::SCRATCH, sizeof(NuSoundLoaderWAV), 4,
                                                 "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1219");
            if (wav_loader != NULL) {
                new (wav_loader) NuSoundLoaderWAV();
                return wav_loader;
            }
            break;
        case FileType::OGG:

            NuSoundLoaderOGG *ogg_loader =
                (NuSoundLoaderOGG *)_AllocMemory(NuSoundSystem::MemoryDiscipline::SCRATCH, sizeof(NuSoundLoaderOGG), 4,
                                                 "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1247");
            if (ogg_loader != NULL) {
                new (ogg_loader) NuSoundLoaderOGG();
                return ogg_loader;
            }

            break;
    }

    return NULL;
}

void NuSoundSystem::ReleaseFileLoader(NuSoundLoader *loader) {
    loader->~NuSoundLoader();
    FreeMemory(MemoryDiscipline::SCRATCH, reinterpret_cast<usize>(loader), 0);
}

// libTTapp.so 0x31a810: the temporary name uses the engine string
// allocation discipline and is released after the decoder copies it.
NuSoundDecoder *NuSoundSystem::CreateDecoder(NuSoundSource *source) {
    char *decoded_name = const_cast<char *>(theEmptyString);
    u16 name_length = 1;
    u16 name_capacity = 1;
    const char *name = source->GetName();
    if (name != NULL) {
        const u16 length = static_cast<u16>(strlen(name) + 1);
        if (decoded_name == theEmptyString || length > name_capacity) {
            name_capacity = static_cast<u16>((length + 3) & 0xfffc);
            if (decoded_name == theEmptyString) {
                decoded_name = static_cast<char *>(
                    NU_ALLOC(name_capacity, 4, 5,
                             "i:/SagaTouch-Android_9176564/nu2api.saga/../nu2api.2013/numemory/NuMemory.h:328", 0));
            } else {
                decoded_name = static_cast<char *>(NuMemoryGet()->GetThreadMem()->_BlockReAlloc(
                    decoded_name, name_capacity, 4, 5,
                    "i:/SagaTouch-Android_9176564/nu2api.saga/../nu2api.2013/numemory/NuMemory.h:333", 0));
            }
        }
        memcpy(decoded_name, name, length);
        name_length = length;
    }

    const u16 appended_length = static_cast<u16>(name_length + 8);
    if (appended_length > name_capacity || decoded_name == theEmptyString) {
        name_capacity = static_cast<u16>((appended_length + 3) & 0xfffc);
        if (decoded_name == theEmptyString) {
            decoded_name = static_cast<char *>(
                NU_ALLOC(name_capacity, 4, 5,
                         "i:/SagaTouch-Android_9176564/nu2api.saga/../nu2api.2013/numemory/NuMemory.h:328", 0));
        } else {
            decoded_name = static_cast<char *>(NuMemoryGet()->GetThreadMem()->_BlockReAlloc(
                decoded_name, name_capacity, 4, 5,
                "i:/SagaTouch-Android_9176564/nu2api.saga/../nu2api.2013/numemory/NuMemory.h:333", 0));
        }
    }
    memcpy(decoded_name + name_length - 1, "_decoder", 9);

    NuSoundDecoder *decoder = NULL;
    if (source->GetStreamDesc()->GetEncodedDataFormat() == NuSoundStreamDesc::DataFormat::THREE) {
        decoder = static_cast<NuSoundDecoderOGG *>(
            NuSoundSystem::_AllocMemory(NuSoundSystem::MemoryDiscipline::SCRATCH, sizeof(NuSoundDecoderOGG), 4,
                                        "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1335"));
        if (decoder != NULL) {
            new (decoder) NuSoundDecoderOGG(decoded_name, source);
        }
    }
    if (decoded_name != theEmptyString) {
        NuMemoryGet()->GetThreadMem()->BlockFree(decoded_name, 4);
    }
    return decoder;
}

void NuSoundSystem::ReleaseDecoder(NuSoundDecoder *decoder) {
    // libTTapp.so 0x31aafb: invoke vtable slot 0 (the complete-object
    // destructor) without using the deleting-destructor slot; the scratch
    // allocator owns the storage release below.
    decoder->~NuSoundDecoder();
    FreeMemory(MemoryDiscipline::SCRATCH, reinterpret_cast<usize>(decoder), 0);
}

void NuSoundSystem::AddRoutingTable(NuSoundRoutingTable *table) {
    routing_table_list.PushBack(table);
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

void NuSoundSystem::SetDefaultRoutingTable(NuSoundRoutingTable *table) {
    sDefaultRoutingTable = table;
}

NuSoundRoutingTable *NuSoundSystem::GetDefaultRoutingTable() {
    return sDefaultRoutingTable;
}

bool NuSoundSystem::AddListener(NuSoundListener *listener) {
    return this->listener_list.PushBack(listener);
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

NuEList<NuSoundListener, DefaultElist> *NuSoundSystem::GetListeners() {
    return &this->listener_list;
}

const NuSoundSystem::CurveData *NuSoundSystem::GetCrossfadeCurve(u32 id) const {
    const NuMapNode<u32, CurveData> *node = crossfade_curves.FindNode(id);
    if (node != NULL) {
        return &node->value;
    }
    return NULL;
}

f32 NuSoundSystem::CalculateCrossfadeHeight(NuSoundSystem::CurveData const &, float) const {
    return 0.0f;
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

NuSoundSystem::FileType NuSoundSystem::DetermineFileType(NUFILETYPE type) {
    char extension[6] = {};
    NuFileExtGetExt(extension, sizeof(extension), type);
    return DetermineFileType(extension);
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

const char *NuSoundSystem::GetFileExtension(FileType type) {
    return sFileExtensions[(i32)type];
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
        if (__builtin_expect(sample == NULL, 1)) {
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

void NuSoundSystem::Shutdown() {
    typedef NuEListOffset<NuSoundVoice, 0x24> VoiceList;
    VoiceList::Link *link = this->voice_list.FrontLink();
    VoiceList::Link *voice_end = this->voice_list.EndLink();
    while (link != voice_end) {
        NuSoundVoice *voice = VoiceList::EntryOf(link);
        VoiceList::Link *next = VoiceList::NextLink(link);
        voice->Stop(false);
        this->ReleaseVoice(voice);
        link = next;
    }

    this->UnloadAllSamples();

    typedef NuEListOffset<NuSoundDecoder, 0x20> DecoderList;
    DecoderList::Link *decoder_link = this->decoder_list.FrontLink();
    DecoderList::Link *decoder_end = this->decoder_list.EndLink();
    while (decoder_link != decoder_end) {
        NuSoundDecoder *entry = DecoderList::EntryOf(decoder_link);
        DecoderList::Link *next = DecoderList::NextLink(decoder_link);
        entry->~NuSoundDecoder();
        FreeMemory(MemoryDiscipline::SCRATCH, reinterpret_cast<usize>(entry), 0);
        // The reference advances twice per iteration here.
        decoder_link = DecoderList::NextLink(next);
    }

    NuSoundBus *bus = this->bus_list.Front();
    NuSoundBus *bus_end = this->bus_list.End();
    while (bus != bus_end) {
        NuSoundBus *next = reinterpret_cast<NuSoundBus **>(bus)[1];
        this->ReleaseBus(bus);
        // The reference advances twice per iteration here as well.
        bus = reinterpret_cast<NuSoundBus **>(next)[1];
    }

    FreeMemory(MemoryDiscipline::SCRATCH, reinterpret_cast<usize>(this->samples), 0);
    this->samples = NULL;

    NuSoundMemoryManager *manager = s_mmSample;
    if (manager != NULL) {
        manager->~NuSoundMemoryManager();
        NU_FREE(manager);
    }
    manager = s_mmDecoder;
    if (manager != NULL) {
        manager->~NuSoundMemoryManager();
        NU_FREE(manager);
    }

    NuMemoryGet()->DestroyMemoryManager(sScratchMemMgr);
    NU_FREE(sDecoderMemory);
    NU_FREE(sSampleMemory);
    NU_FREE(sScratchMemory);

    this->ShutdownAudioDevice();
}

void NuSoundSystem::Update(f32 frametime) {
    if (this->initialised == false) {
        return;
    }

    // Platform hook (on Android this only polls the application state).
    this->UpdateAudioDevice();

    this->mutex.Lock();

    // Pass 1: drive every platform voice's device state.
    for (NuListNodeBase *node = effect_update_list.Head(), *end = effect_update_list.Tail(); node != end;
         node = node->GetNext()) {
        static_cast<NuListNode<NuSoundEffect *> *>(node)->value->Process(frametime);
    }

    // Pass 2: update the engine-side mix of every playing voice; stopped
    // auto-delete voices are released.
    typedef NuEListOffset<NuSoundVoice, 0x24> VoiceList;
    VoiceList::Link *link = voice_list.FrontLink();
    VoiceList::Link *end = voice_list.EndLink();
    while (link != end) {
        NuSoundVoice *voice = VoiceList::EntryOf(link);
        VoiceList::Link *next = VoiceList::NextLink(link);

        NuSoundVoice::PlayState state = voice->GetState();
        if (state == NuSoundVoice::PLAYSTATE_PLAYING) {
            pthread_mutex_lock(&NuSoundWeakPtrListNode::sPtrAccessLock.critical_section.mutex);
            voice->Update(frametime);
            pthread_mutex_unlock(&NuSoundWeakPtrListNode::sPtrAccessLock.critical_section.mutex);
        } else if (state == NuSoundVoice::PLAYSTATE_STOPPED && voice->GetAutoDelete()) {
            this->ReleaseVoice(voice);
        }

        link = next;
    }

    this->mutex.Unlock();
}

NuSoundVoice *NuSoundSystem::CreateVoice(NuSoundSource *source, bool loop) {
    NuSoundVoice *voice;
    if (!SourceRequiresDecoder(source)) {
        if (!source->IsStreamOpen()) {
            return NULL;
        }
        NuSoundStreamDesc *desc = source->GetStreamDesc();
        NuSoundVoiceFactory *factory = factory_list.GetFactory(desc->GetDecodedDataFormat());
        voice = factory->CreateVoice(source, loop);
        if (voice == NULL) {
            return NULL;
        }
    } else {
        NuSoundDecoder *decoder = CreateDecoder(source);
        decoder->OpenStream(loop);
        if (!decoder->IsStreamOpen()) {
            ReleaseDecoder(decoder);
            return NULL;
        }
        NuSoundStreamDesc *desc = decoder->GetStreamDesc();
        NuSoundVoiceFactory *factory = factory_list.GetFactory(desc->GetDecodedDataFormat());
        voice = factory->CreateVoice(decoder, loop);
        if (voice == NULL) {
            decoder->CloseStream();
            ReleaseDecoder(decoder);
            return NULL;
        }
    }

    mutex.Lock();
    NuSoundWeakPtrListNode::sPtrAccessLock.Lock();
    voice_list.PushBack(voice);
    NuSoundWeakPtrListNode::sPtrAccessLock.Unlock();
    mutex.Unlock();
    return voice;
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

NuSoundListener *NuSoundSystem::GetNearestRealListener(NuEList<NuSoundListener, DefaultElist> const &listeners,
                                                       VuVec const &position) {
    NuSoundListener *nearest = NULL;
    f32 nearest_distance = FLT_MAX;
    NuSoundListener *end = listeners.end;
    NuSoundListener *listener = static_cast<NuSoundListener *>(listeners.begin->field_0x4);

    while (listener != end) {
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
    NuSoundListener *end = listeners.end;
    NuSoundListener *listener = static_cast<NuSoundListener *>(listeners.begin->field_0x4);

    while (listener != end) {
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

f32 NuSoundSystem::AmplitudeTodB(f32 amplitude) {
    if (amplitude <= 0.0f) {
        return -100.0f;
    }
    if (amplitude >= 1.0f) {
        return 0.0f;
    }
    return NuLog10(amplitude) * 20.0f;
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

i32 NuSoundSystem::GetOutputChannelConfig() {
    return sOutputConfig;
}

i32 NuSoundSystem::GetClosestSupportedConfig(i32 config) {
    // libTTapp.so 0x31bcb0: config > 7 -> 8, config >= 6 -> 6, else 2.
    i32 result = 8;
    if (config <= 7) {
        result = 2;
        if (config >= 6) {
            result = 6;
        }
    }
    return result;
}

i32 NuSoundSystem::GetNumAvailableOutputDevices() {
    return sNumAvailableOutputDevices;
}

void NuSoundSystem::SetMainThreadID(NuThread *) {
}

const char *NuSoundSystem::GetPlatformString() {
    return "ANDROID";
}

const char *NuSoundSystem::GetLanguageString(bool) {
    return "Eng";
}

void NuSoundSystem::Disable() {
    sNumAvailableOutputDevices = 0;
}

u32 NuSound_GetAllocdSampleMemory() {
    return NuSoundSystem::GetAllocdMemory(NuSoundSystem::MemoryDiscipline::SAMPLE);
}

NuSoundEffect *NuSoundSystem::CreateEffect(NuSoundEffect::EffectType type) {
    NuSoundEffect *effect = NULL;

    switch (type) {
        case NuSoundEffect::EffectType::ATTENUATION: {
            effect = static_cast<NuSoundEffect *>(_AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectAttenuation), 4,
                                                                       "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1094"));
            if (effect != NULL)
                new (effect) NuSoundEffectAttenuation();
            break;
        }
        case NuSoundEffect::EffectType::PITCH: {
            effect = static_cast<NuSoundEffect *>(_AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectPitch), 4,
                                                                       "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1100"));
            if (effect != NULL)
                new (effect) NuSoundEffectPitch();
            break;
        }
        case NuSoundEffect::EffectType::FADER: {
            effect = static_cast<NuSoundEffect *>(_AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectFader), 4,
                                                                       "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1106"));
            if (effect != NULL)
                new (effect) NuSoundEffectFader();
            break;
        }
        case NuSoundEffect::EffectType::PITCH_RAMP: {
            effect = static_cast<NuSoundEffect *>(_AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectPitchRamp), 4,
                                                                       "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1112"));
            if (effect != NULL)
                new (effect) NuSoundEffectPitchRamp();
            break;
        }
        case NuSoundEffect::EffectType::RANDOM_VOLUME: {
            effect = static_cast<NuSoundEffect *>(_AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectRandomVolume), 4,
                                                                       "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1118"));
            if (effect != NULL)
                new (effect) NuSoundEffectRandomVolume();
            break;
        }
        case NuSoundEffect::EffectType::RANDOM_PITCH: {
            effect = static_cast<NuSoundEffect *>(_AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectRandomPitch), 4,
                                                                       "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1124"));
            if (effect != NULL)
                new (effect) NuSoundEffectRandomPitch();
            break;
        }
        case NuSoundEffect::EffectType::REPEAT: {
            effect = static_cast<NuSoundEffect *>(_AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectRepeat), 4,
                                                                       "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1136"));
            if (effect != NULL)
                new (effect) NuSoundEffectRepeat();
            break;
        }
        case NuSoundEffect::EffectType::DOPPLER: {
            effect = static_cast<NuSoundEffect *>(_AllocMemory(MemoryDiscipline::SCRATCH, sizeof(NuSoundEffectDoppler), 4,
                                                                       "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1130"));
            if (effect != NULL)
                new (effect) NuSoundEffectDoppler();
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

void NuSoundSystem::ReleaseCrossfadeCurve(u32 id) {
    crossfade_curves.Erase(id);
}

NuSoundSystem::CurveData *NuSoundSystem::CreateCrossfadeCurve(u32 id) {
    return &crossfade_curves.InsertNode(id)->value;
}

NuSoundSystem::~NuSoundSystem() {
    s_staticInstance = NULL;
}

bool NuSoundEffect::Initialise() {
    return true;
}

void NuSoundEffect::Shutdown() {
}

void NuSoundEffect::Enable() {
    this->enabled = true;
}

void NuSoundEffect::Disable() {
    this->enabled = false;
}

bool NuSoundEffect::AttachVoice(NuSoundVoice *) {
    return true;
}

void NuSoundEffect::DetachVoice(NuSoundVoice *) {
}

void NuSoundEffect::ProcessVoice(NuSoundVoice *, f32) {
}

bool NuSoundEffect::AttachBus(NuSoundBus *) {
    return false;
}

void NuSoundEffect::DetachBus(NuSoundBus *) {
}

void NuSoundEffect::ProcessBus(NuSoundBus *, f32) {
}

void NuSoundEffect::Process(f32) {
}

bool NuSoundSystem::IsUserPlayingMusic() {
    return false;
}

void NuSoundSystem::PauseUserMusic() {
}

void NuSoundSystem::ResumeUserMusic() {
}

bool NuSoundSystem::TitleHasUserMusicControl() {
    return true;
}

void NuSoundSystem::OnEnterSystemMenu() {
}

void NuSoundSystem::OnExitSystemMenu() {
}

bool NuSoundEffectAttenuation::AttachBus(NuSoundBus *) {
    return true;
}

void NuSoundEffectRepeat::ProcessVoice(NuSoundVoice *voice, f32 frametime) {
    if (voice->GetState() == NuSoundVoice::PLAYSTATE_STOPPED) {
        if (armed && repeat_count != 0) {
            armed = false;
            repeat_count--;
            remaining_delay = delay;
            voice->DestroyHardwareVoice();
            voice->CreateHardwareVoice();
            voice->Play();
            voice->Pause();
        }
    } else if (voice->GetState() == NuSoundVoice::PLAYSTATE_PAUSED) {
        if (!armed && repeat_count != 0) {
            remaining_delay -= frametime;
            if (remaining_delay <= 0.0f) {
                voice->Resume();
            }
        }
    } else {
        armed = true;
    }
}

void NuSoundEffectAttenuation::ProcessVoice(NuSoundVoice *voice, f32) {
    if (attachments.Length() == 0) {
        output_mix = attenuation;
        return;
    }

    if (voice->GetSurroundMode() != NuSoundSystem::SurroundMode::TWO) {
        for (NuListNodeBase *node = attachments.Head(); node != attachments.Tail(); node = node->GetNext()) {
            NuSoundListener *listener = static_cast<NuListNode<NuSoundVoiceListenerLink> *>(node)->value.listener;
            if (listener != NULL && listener == voice->focus_listener_link.listener) {
                output_mix = attenuation;
                return;
            }
        }
    }
    output_mix = 1.0f;
}

template <typename T> void NuSoundMemory::PushNuListNode(NuList<T> &list, T const &value) {
    NuMemoryManager *previous = NuMemoryGet()->SetThreadMem(NuSoundSystem::sScratchMemMgr);
    NuListNode<T> *node = static_cast<NuListNode<T> *>(NuMemoryGet()->GetThreadMem()->_BlockAlloc(
        sizeof(NuListNode<T>), 4, NuMemoryManager::MEM_ALLOC_SET_TO_ZERO, "", NUMEMORY_CATEGORY_NONE));
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

// The reference exports this weak template member at 0x31d650.
template void NuMap<u32, NuSoundSystem::CurveData>::DeleteNodeLinks(NuMapNode<u32, NuSoundSystem::CurveData> *);
