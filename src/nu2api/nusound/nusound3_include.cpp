#include "nu2api/nusound/nusound3_include.hpp"

#include "decomp.h"

#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nucore/nucore.hpp"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/nuvector.hpp"
#include "nu2api/numath/numath.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/nusound/nusound.h"
#include "nu2api/nusound/nusound_android.hpp"
#include "nu2api/nusound/nusound_buffer.hpp"
#include "nu2api/nusound/nusound_loader.hpp"
#include "nu2api/nusound/nusound_loader_ogg.hpp"
#include "nu2api/nusound/nusound_streamer.hpp"
#include "nu2api/nusound/nusound_voice.hpp"
#include "nu2api/nusound/nusound_decoder.hpp"

#include <new>
#include <stdarg.h>
#include <string.h>

// Stereo stream slot (0x10 bytes in the original):
//   +0x0 stream            the streaming sample playing in this slot
//   +0x4 loop              PENDING-START flag: PlayStereoV sets it to 1, and
//                          NuSound3Update clears it once the voice is created
//   +0x8 volume            the PS2 volume (0..16383) as raw bits
//   +0xc field_0xc         the voice/loader loop flag (PlayStereoV key 0xb)
//   +0xd field_0xd         started flag: set once the voice began playing
struct NuSoundStream {
    NuSoundStreamingSample *stream;
    u8 loop;
    i32 ps2volume;
    u8 field_0xc;
    u8 field_0xd;
};

// The single stereo-stream voice shared by all stream slots. This is a class
// member in the original ABI, even though the slot bookkeeping lives here.
class NuSound3Stream {
  public:
    static NuSoundWeakPtr<NuSoundVoice> mVoice;
};

DECOMP_ASSERT(sizeof(NuSound3Stream::mVoice) == 0x10, "NuSound3Stream voice pointer size");

// The original focused the listener on the player object; the title screen
// runs before gameplay, where it is NULL and the focus stays disabled.
static void *g_NuSoundFocusPlayer = NULL;
static nuvec_s g_NuSoundFocusPosition;

extern "C" {
    const char *audio_ps2_music_ext = ".vag";
}

template <> class NuVector<nusound_filename_info_s> {
  public:
    nusound_filename_info_s *data;
    u32 capacity;
    u32 length;
    NuVector() : data(NULL), capacity(0), length(0) {
    }
    ~NuVector() {
        if (length != 0)
            length = 0;
        if (data != NULL) {
            NuMemoryGet()->GetThreadMem()->BlockFree(data, 0);
            capacity = 0;
            data = NULL;
        }
    }
    void PushBack(const nusound_filename_info_s &value) {
        if (length + 1 > capacity) {
            u32 new_capacity = (length + 4) & ~3u;
            nusound_filename_info_s *replacement =
                static_cast<nusound_filename_info_s *>(NuMemoryGet()->GetThreadMem()->_BlockReAlloc(
                    data, new_capacity * sizeof(*data), 4, 0x41, "", NUMEMORY_CATEGORY_NONE));
            if (replacement != data) {
                nusound_filename_info_s *previous = data;
                for (u32 i = 0; i < length; ++i)
                    replacement[i] = previous[i];
                NuMemoryGet()->GetThreadMem()->BlockFree(previous, 0);
            }
            capacity = new_capacity;
            data = replacement;
        }
        data[length++] = value;
    }
};

static NuVector<nusound_filename_info_s> g_NuSoundSamples;

extern "C" bool NuSound3IsSampleLoaded(i32 index) {
    return g_NuSoundSamples.data[index].sample->GetLoadState() == NuSoundSample::LoadState::LOADED;
}

extern "C" NuSoundVoice *NuSound3FindOldestVoice(i32 index, f32 &age) {
    NuSoundSample *sample = g_NuSoundSamples.data[index].sample;
    return sample != NULL ? NuSound.GetOldestVoice(sample, age) : NULL;
}

extern "C" NuSoundVoice *NuSound3FindQuietestVoice(i32 index, f32 &volume) {
    NuSoundSample *sample = g_NuSoundSamples.data[index].sample;
    return sample != NULL ? NuSound.GetQuietestVoice(sample, volume) : NULL;
}

static NuSoundStream *g_NuSoundStreams[4] = {0};

extern "C" f32 NuSound3GetStreamPlaybackTime(i32 index) {
    if (g_NuSoundStreams[index] != NULL && NuSound3Stream::mVoice.obj != NULL) {
        return reinterpret_cast<NuSoundVoice *>(NuSound3Stream::mVoice.obj)->GetPlaybackPositionSeconds();
    }
    return 0.0f;
}

static NuSoundLoadTrigger g_NuSoundLoadTrigger;
static NuCriticalSection g_NuSoundLoadCriticalSection("NuSoundCriticalSection::mCriticalSection");

// 0x44-byte voice wrapper the NuSound3 API keeps per playing source. The
// elists link these while they are pending playback / active / pending
// destruction.
struct NuSound3Voice {
    NuSoundWeakPtr<NuSoundVoice> weak_ptr;
    NuSoundSource *source;
    f32 pitch;     // +0x1c
    i32 volume;    // +0x20 raw volume bits
    f32 falloff_a; // +0x24
    f32 falloff_b; // +0x28
    bool has_3d;
    nuvec_s position;
    nuvec_s *position_source;
    i32 pause_counter; // frames in the update sweep
};

DECOMP_ASSERT(sizeof(NuSound3Voice) == 0x3c, "NuSound3Voice payload size");

template <> class NuEListNode<NuSound3Voice, DefaultElist> {
  public:
    NuEListNode *prev;
    NuEListNode *next;
    NuSound3Voice data;
};

DECOMP_ASSERT(sizeof(NuSoundStream) == 0x10, "stream slot size");
DECOMP_ASSERT(offsetof(NuSoundStream, loop) == 4, "stream pending flag offset");
DECOMP_ASSERT(offsetof(NuSoundStream, ps2volume) == 8, "stream volume offset");

template <> class NuEList<NuSound3Voice, DefaultElist> {
  public:
    typedef NuEListNode<NuSound3Voice> Node;
    struct VoiceQueueLinks {
        Node *prev;
        Node *next;
    } start, end;
    Node *head;
    Node *tail;
    u32 length;
    NuEList() {
        head = reinterpret_cast<Node *>(&start);
        tail = reinterpret_cast<Node *>(&end);
        start.prev = NULL;
        end.next = NULL;
        start.next = tail;
        end.prev = head;
        length = 0;
    }
    ~NuEList() {
        while (length != 0) {
            Node *node = head->next;
            if (node->prev != NULL)
                node->prev->next = node->next;
            if (node->next != NULL)
                node->next->prev = node->prev;
            --length;
            node->next = NULL;
            node->prev = NULL;
            delete node;
        }
    }
};

DECOMP_ASSERT(sizeof(NuEList<NuSound3Voice>) == 0x1c, "voice queue size");
DECOMP_ASSERT(sizeof(NuEListNode<NuSound3Voice>) == 0x44, "voice node size");

namespace {

    // NuEList append/remove, done here as local helpers so the list header keeps
    // emitting no inline code into every translation unit that pulls it in.
    void StreamListPushBack(NuEList<NuSound3Voice> *list, NuEListNode<NuSound3Voice> *node) {
        node->prev = list->tail->prev;
        node->next = list->tail;
        list->tail->prev->next = node;
        list->tail->prev = node;
        ++list->length;
    }

    void StreamListRemove(NuEList<NuSound3Voice> *list, NuEListNode<NuSound3Voice> *node) {
        if (node->prev != NULL)
            node->prev->next = node->next;
        if (node->next != NULL)
            node->next->prev = node->prev;
        --list->length;
        node->prev = NULL;
        node->next = NULL;
    }

} // namespace

static NuEList<NuSound3Voice> g_NuSoundVoicesPendingPlayback{};
static NuEList<NuSound3Voice> g_NuSoundVoicesActive{};
static NuEList<NuSound3Voice> g_NuSoundVoicesPendingDestruction{};
static i32 g_NuSoundNumReplaceableVoices;

NuSoundWeakPtr<NuSoundVoice> NuSound3Stream::mVoice;

extern "C" void NuSound3StopVoice(NuSoundVoice *voice) {
    if (voice == NULL)
        return;
    voice->Stop(false);
    for (NuEListNode<NuSound3Voice> *node = g_NuSoundVoicesActive.head->next; node != g_NuSoundVoicesActive.tail;
         node = node->next) {
        if (reinterpret_cast<NuSoundVoice *>(node->data.weak_ptr.obj) != voice)
            continue;
        if (node->next != g_NuSoundVoicesActive.tail || node != NULL) {
            if (node->next != NULL || node->prev != NULL) {
                --g_NuSoundVoicesActive.length;
                if (node->prev != NULL)
                    node->prev->next = node->next;
                if (node->next != NULL)
                    node->next->prev = node->prev;
                node->next = NULL;
                node->prev = NULL;
            }
            StreamListPushBack(&g_NuSoundVoicesPendingDestruction, node);
            ++g_NuSoundNumReplaceableVoices;
        }
        return;
    }
}

extern "C" i32 NuSound3CountVoices(i32 index) {
    NuSoundSample *sample = g_NuSoundSamples.data[index].sample;
    if (sample == NULL)
        return 0;
    i32 count = sample->field_0x18;
    for (NuEListNode<NuSound3Voice> *node = g_NuSoundVoicesPendingPlayback.head->next;
         node != g_NuSoundVoicesPendingPlayback.tail; node = node->next) {
        if (node->data.source == sample)
            ++count;
    }
    return count;
}

static NuSoundBuffer g_NuSoundStreamBuffers[4];

static NuSoundListener g_NuSoundListener;

extern "C" void NuSound3Listener(const VuMtx *matrix) {
    g_NuSoundListener.SetHeadMatrix(matrix);
}

extern "C" const VuMtx *NuSound3GetListener() {
    return g_NuSoundListener.GetHeadMatrix();
}

static NuSoundStreamer *g_NuSoundStreamer = NULL;

__attribute__((visibility("hidden"))) u16 *g_NuSoundLoadBits asm("_ZL17g_NuSoundLoadBits") = NULL;
__attribute__((visibility("hidden"))) u16 *g_NuSoundLoadBitsCache asm("_ZL22g_NuSoundLoadBitsCache") = NULL;
__attribute__((visibility("hidden"))) i32 g_NuSoundNumLoadBitShorts asm("_ZL25g_NuSoundNumLoadBitShorts") = 0;
static NuThread *g_NuSoundLoadThread = NULL;

void NuSound3SampleLoadThread(void *arg) {
    (void)arg;

    while (true) {
        pthread_mutex_lock(&g_NuSoundLoadTrigger.mutex);
        while (!g_NuSoundLoadTrigger.a) {
            pthread_cond_wait(&g_NuSoundLoadTrigger.cond, &g_NuSoundLoadTrigger.mutex);
        }
        g_NuSoundLoadTrigger.a = g_NuSoundLoadTrigger.b;
        pthread_mutex_unlock(&g_NuSoundLoadTrigger.mutex);

        if (g_NuSoundLoadBits == NULL) {
            continue;
        }

        pthread_mutex_lock(&g_NuSoundLoadCriticalSection.mutex);

        for (u32 i = 0; i < g_NuSoundSamples.length; i++) {
            nusound_filename_info_s &info = g_NuSoundSamples.data[i];
            if (info.index <= 0xfff) {
                continue;
            }

            i32 request = static_cast<u16>(g_NuSoundLoadBits[i >> 4] & (1 << (i & 0xf)));
            NuSoundSample::LoadState load_state = info.sample->GetLoadState();
            info.sample->GetLastErrorState();
            NuSoundSample *sample = info.sample;

            if (request == 0 && load_state != NuSoundSample::LoadState::NOT_LOADED && sample != NULL &&
                sample->field_0x18 == 0) {
                sample->Release();
                info.sample->Unload();
            }
        }

        for (u32 i = 0; i < g_NuSoundSamples.length; i++) {
            nusound_filename_info_s &info = g_NuSoundSamples.data[i];
            if (info.index <= 0xfff) {
                continue;
            }

            i32 request = static_cast<u16>(g_NuSoundLoadBits[i >> 4] & (1 << (i & 0xf)));
            NuSoundSample::LoadState load_state = info.sample->GetLoadState();
            NuSoundSample::ErrorState error = info.sample->GetLastErrorState();
            NuSoundSample *sample = info.sample;

            if (request != 0 && error != NuSoundSample::ErrorState::FILE_NOT_FOUND &&
                load_state == NuSoundSample::LoadState::NOT_LOADED && sample != NULL) {
                sample->Reference();
                info.sample->Load(NULL, 0, NULL);
            }
        }

        pthread_mutex_unlock(&g_NuSoundLoadCriticalSection.mutex);
    }
}

nusound_filename_info_s *ConfigureMusic(char *file, VARIPTR *bufferStart, VARIPTR *bufferEnd) {
    nusound_filename_info_s *finfo;

    music_man.Initialise("audio\\music.cfg", NULL, bufferStart, *bufferEnd);
    music_man.GetSoundFiles(&finfo, NULL);

    audio_ps2_music_ext = ".mib";

    // MusicConfig *musicConfig;
    // musicConfig = (MusicConfig *)((i32)bufferStart->voidptr + 3U & 0xfffffffc);

    // musicConfig->field0_0x0 = 0;
    // ActionPairTab = &musicConfig->actionTab;
    // musicConfig->actionTab = -1;
    // musicConfig->ambientTab = -1;
    // AmbientPairTab = &musicConfig->ambientTab;
    // bufferStart->voidptr = musicConfig + 1;

    return finfo;
}

NuSoundLoader *NuSoundSystem::CreateFileLoader(FileType type) {
    NuSoundLoaderWAV *wav_loader;

    switch (type) {
        case FileType::WAV:
            // libTTapp.so allocates the WAV loader from SCRATCH (0x1c bytes,
            // nusound.cpp:1233) and placement-news it in place.
            wav_loader =
                (NuSoundLoaderWAV *)_AllocMemory(NuSoundSystem::MemoryDiscipline::SCRATCH, 0x1c, 4,
                                                 "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/nusound.cpp:1233");
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

void NuSound3Init(i32 zero) {
    i32 extra_memory = NuIOS_IsLowEndDevice() ? 0 : 0x1ccccd;

    NuCore::Initialize();

    NuSound.Initialise(0x633333 + extra_memory);
    // libTTapp.so 0x3109af: NuSound3Init brings up the decoder singleton
    // thread right after the sound system.
    NuSoundDecoder::Initialise();

    // NuMemoryGet()->GetThreadMem()->_BlockAlloc(0xa48, 4, 1, "", 7);
    NuSoundStreamer *streamer = NU_ALLOC_T(NuSoundStreamer, 1, "", NUMEMORY_CATEGORY_NUSOUND);
    if (streamer != NULL) {
        new (streamer) NuSoundStreamer{};
    }
    g_NuSoundStreamer = streamer;

    // g_NuSoundLoadThread = NuThreadManager::CreateThread(NuCore::m_threadManager, NuSound3SampleLoadThread, (void
    // *)0x0,
    // 0, "NuSoundLoadThread", 0, 1, 1);
    g_NuSoundLoadThread =
        NuCore::m_threadManager->CreateThread(NuSound3SampleLoadThread, NULL, 0, "NuSoundLoadThread", 0,
                                              NUTHREADCAFECORE_UNKNOWN_1, NUTHREADXBOX360CORE_UNKNOWN_1);

    // NuSound3Init registers the single 3D listener with the head matrix of
    // the title screen camera.
    NuSound.AddListener(&g_NuSoundListener);
    g_NuSoundListener.SetHeadMatrix(reinterpret_cast<const VuMtx *>(&global_camera.mtx));
    g_NuSoundListener.Enable();

    g_NuSoundStreamBuffers[0].Allocate(NuSoundSystem::GetStreamBufferSize() / 2,
                                       NuSoundSystem::MemoryDiscipline::SAMPLE);
    g_NuSoundStreamBuffers[1].Allocate(NuSoundSystem::GetStreamBufferSize() / 2,
                                       NuSoundSystem::MemoryDiscipline::SAMPLE);
    g_NuSoundStreamBuffers[2].Allocate(NuSoundSystem::GetStreamBufferSize() / 2,
                                       NuSoundSystem::MemoryDiscipline::SAMPLE);
    g_NuSoundStreamBuffers[3].Allocate(NuSoundSystem::GetStreamBufferSize() / 2,
                                       NuSoundSystem::MemoryDiscipline::SAMPLE);
}

i32 NuSound3InitV(VARIPTR *bufferStart, VARIPTR bufferEnd, i32 zero1, i32 zero2) {
    NuSound3Init(0);
    return 1;
}

// PS2 volume (0..16383) to the engine's linear scalar.
f32 PS2VolumeToScalar(i32 volume) {
    return (f32)volume / 16383.0f;
}

// NuSound3PlayStereoV is the original varargs play entry. The music player
// pushes every argument as a raw dword EXCEPT STARTOFFSET, which is a true
// 8-byte double: VOL is float bits in an int (always 0 from NuMusic, which
// drives volume via NuSound3SetStereoStreamVolume instead), PITCH is a raw
// fixed-point dword the original reads and ignores. The sample's two stream
// buffers are carved out of g_NuSoundStreamBuffers[stream_index * 2] and
// [stream_index * 2 + 1].
i32 NuSound3PlayStereoV(NUSOUNDPLAYTOK token, ...) {
    va_list args;
    va_start(args, token);

    i32 stream_index = 0;
    i32 sample_index = -1;
    i32 volume_bits = 0;
    i32 pitch = 0;
    f32 start_offset = 0.0f;
    i32 voice_loop = 0;

    while (token != NUSOUNDPLAYTOK_END) {
        switch (token) {
            case NUSOUNDPLAYTOK_STEREOSTREAM: {
                stream_index = va_arg(args, i32);
                break;
            }
            case NUSOUNDPLAYTOK_SAMPLE: {
                sample_index = va_arg(args, i32);
                break;
            }
            case NUSOUNDPLAYTOK_VOL: {
                volume_bits = va_arg(args, i32);
                break;
            }
            case NUSOUNDPLAYTOK_PITCH: {
                pitch = va_arg(args, i32);
                break;
            }
            case NUSOUNDPLAYTOK_STARTOFFSET: {
                start_offset = (f32)va_arg(args, double);
                break;
            }
            case NUSOUNDPLAYTOK_LOOPTYPE: {
                voice_loop = va_arg(args, i32);
                break;
            }
            default: {
                LOG_WARN("Unknown token %d", token);
                break;
            }
        }

        token = (NUSOUNDPLAYTOK)va_arg(args, u32);
    }

    if (stream_index < 0 || stream_index >= 4) {
        return 0;
    }

    NuSoundStreamingSample *streaming_sample = g_NuSoundSamples.data[sample_index].sample;
    if (streaming_sample->GetThreadQueueCount() < 1) {
        NuSoundStream *stream_ptr = g_NuSoundStreams[stream_index];
        if (stream_ptr != NULL) {
            NuSoundStreamingSample *stream = stream_ptr->stream;
            if (stream != streaming_sample) {
                NuSound3StopStereoStream(stream_index);
                stream = stream_ptr->stream;
            }

            NuSoundSample::LoadState load_state = stream->GetLoadState();
            if ((load_state == NuSoundSample::LoadState::NOT_LOADED) && stream->GetResourceCount() == 0) {
                delete stream_ptr;
                g_NuSoundStreams[stream_index] = NULL;
            } else if (g_NuSoundStreams[stream_index] != NULL) {
                return 0;
            }
        }

        if (streaming_sample != NULL && streaming_sample->GetResourceCount() == 0) {
            NuSoundStream *node = new NuSoundStream();

            node->ps2volume = volume_bits;
            node->loop = 1; // PENDING-START; NuSound3Update creates the voice.
            node->field_0xc = (u8)(voice_loop != 0);
            node->field_0xd = 0;
            g_NuSoundStreams[stream_index] = node;
            node->stream = streaming_sample;

            streaming_sample->sound_buffer1 = &g_NuSoundStreamBuffers[stream_index * 2];
            streaming_sample->sound_buffer2 = &g_NuSoundStreamBuffers[stream_index * 2 + 1];

            streaming_sample->Reference();
            g_NuSoundStreamer->RequestCue(streaming_sample, node->field_0xc != 0, start_offset, false);

            return 1;
        }
    }

    return 0;
}

// NuSound3StopStereoStream tears down the stream node in a stereo-stream slot.
// The slot's node itself is deleted by NuSound3Update once the sample has
// fully unloaded.
void NuSound3StopStereoStream(i32 stream_index) {
    NuSoundStream *stream = g_NuSoundStreams[stream_index];
    if (stream == NULL) {
        return;
    }

    if (NuSound3Stream::mVoice.obj != NULL && stream->field_0xd != 0) {
        ((NuSoundVoice *)NuSound3Stream::mVoice.obj)->Stop(true);
        NuSound.ReleaseVoice((NuSoundVoice *)NuSound3Stream::mVoice.obj);
        NuSoundWeakPtrListNode::sPtrListLock.Lock();
        if (NuSound3Stream::mVoice.obj != NULL) {
            NuSound3Stream::mVoice.obj->Unlink(&NuSound3Stream::mVoice);
            NuSound3Stream::mVoice.obj = NULL;
        }
        NuSoundWeakPtrListNode::sPtrListLock.Unlock();
        stream->field_0xd = 0;
    }

    NuSoundStreamingSample *sample = stream->stream;
    if (sample != NULL && sample->GetResourceCount() > 0) {
        stream->stream->Release();
        g_NuSoundStreamer->RequestClose(stream->stream);
    }
}

void NuSound3PauseStereoStream(i32 stream_index) {
    NuSoundStream *stream = g_NuSoundStreams[stream_index];
    if (stream != NULL && NuSound3Stream::mVoice.obj != NULL) {
        ((NuSoundVoice *)NuSound3Stream::mVoice.obj)->Pause();
    }
}

void NuSound3ResumeStereoStream(i32 stream_index) {
    NuSoundStream *stream = g_NuSoundStreams[stream_index];
    if (stream != NULL && NuSound3Stream::mVoice.obj != NULL) {
        ((NuSoundVoice *)NuSound3Stream::mVoice.obj)->Resume();
    }
}

// Queue a source using the reference per-source replacement policy.
void NuSound3CreateVoice(nuvec_s *pos, i32 index, f32 falloff_a, f32 falloff_b, i32 volume, i32 unused, f32 pitch,
                         bool has_3d) {
    if (NuSound.GetNumAvailableOutputDevices() < 1 || index < 0 || index >= static_cast<i32>(g_NuSoundSamples.length))
        return;
    NuSoundSample *sample = g_NuSoundSamples.data[index].sample;
    if (sample == NULL || sample->GetLoadState() != NuSoundSample::LoadState::LOADED)
        return;
    if (sample->GetResourceCount() <= 0)
        return;
    i32 total = g_NuSoundVoicesPendingPlayback.length + g_NuSoundVoicesActive.length;
    i32 count = sample->field_0x18;
    for (NuEListNode<NuSound3Voice> *node = g_NuSoundVoicesPendingPlayback.head->next;
         node != g_NuSoundVoicesPendingPlayback.tail; node = node->next) {
        if (node->data.source == sample)
            ++count;
    }
    if (count > 2 || total > 15) {
        if (has_3d || g_NuSoundSamples.data[index].field7_0x1c == 0 || count <= 2)
            return;
        f32 age = 0.0f;
        NuSoundVoice *oldest = NuSound.GetOldestVoice(sample, age);
        if (oldest != NULL)
            NuSound3StopVoice(oldest);
    }
    NuEListNode<NuSound3Voice> *node = new NuEListNode<NuSound3Voice>();
    node->data.source = sample;
    node->data.falloff_a = falloff_a;
    node->data.falloff_b = falloff_b;
    node->data.volume = volume;
    node->data.pitch = pitch;
    node->data.has_3d = has_3d;
    node->data.position_source = pos;
    if (pos != NULL)
        node->data.position = *pos;
    StreamListPushBack(&g_NuSoundVoicesPendingPlayback, node);
}

extern "C" void NuSound3Play(i32 index, i32 volume, i32 unused, f32 pitch) {
    NuSound3CreateVoice(NULL, index, 0.0f, 0.0f, volume, unused, pitch, false);
}

extern "C" void NuSound3Play3d(nuvec_s *position, i32 index, f32 near_distance, f32 far_distance, i32 volume,
                               i32 unused, f32 pitch) {
    NuSound3CreateVoice(position, index, near_distance, far_distance, volume, unused, pitch, false);
}

extern "C" void NuSound3Play3dPri(nuvec_s *position, i32 index, f32 unused_near, f32 unused_far, i32 volume, i32 unused,
                                  f32 pitch) {
    NuSound3CreateVoice(position, index, 0.0f, 0.0f, volume, unused, pitch, false);
}

extern "C" void NuSound3PlayPri(i32 index, i32 volume, i32 unused, f32 pitch) {
    NuSound3CreateVoice(NULL, index, 0.0f, 0.0f, volume, unused, pitch, false);
}

extern "C" void NuSound3Play3dLoopSfx(nuvec_s *position, i32 index, f32 near_distance, f32 far_distance, i32 volume,
                                      i32 unused, f32 pitch) {
    for (NuEListNode<NuSound3Voice> *node = g_NuSoundVoicesActive.head->next; node != g_NuSoundVoicesActive.tail;
         node = node->next) {
        if (node->data.position_source != position)
            continue;
        node->data.pause_counter = 0;
        if (node->data.weak_ptr.obj != NULL) {
            reinterpret_cast<NuSoundVoice *>(node->data.weak_ptr.obj)->SetPosition(reinterpret_cast<VuVec *>(position));
            reinterpret_cast<NuSoundVoice *>(node->data.weak_ptr.obj)->SetVolume(PS2VolumeToScalar(volume));
            reinterpret_cast<NuSoundVoice *>(node->data.weak_ptr.obj)->SetPitch(pitch);
            reinterpret_cast<NuSoundVoice *>(node->data.weak_ptr.obj)
                ->SetFalloff(near_distance, far_distance, static_cast<NuSoundSystem::FalloffType>(0));
        }
        return;
    }
    NuSound3CreateVoice(position, index, near_distance, far_distance, volume, unused, pitch, true);
}

void NuSound3Update(void) {
    if (NuSound.GetNumAvailableOutputDevices() < 1) {
        NuSound.Update(NuTimeGetFrameTime());
        return;
    }

    // The listener focus follows the player (NULL on the title screen).
    if (g_NuSoundFocusPlayer == NULL) {
        g_NuSoundListener.DisableFocusPosition();
    } else {
        g_NuSoundListener.SetFocusPosition((const VuVec *)&g_NuSoundFocusPosition);
        g_NuSoundListener.EnableFocusPosition();
    }

    pthread_mutex_lock(&NuSound.mutex);

    // (a) Voices queued for destruction: release and unlink.
    for (NuEListNode<NuSound3Voice> *node = g_NuSoundVoicesPendingDestruction.head->next;
         node != g_NuSoundVoicesPendingDestruction.tail;) {
        NuEListNode<NuSound3Voice> *next = node->next;
        if (node->data.weak_ptr.obj != NULL) {
            NuSound.ReleaseVoice((NuSoundVoice *)node->data.weak_ptr.obj);
            node->data.weak_ptr.Set(NULL);
        }
        StreamListRemove(&g_NuSoundVoicesPendingDestruction, node);
        delete node;
        node = next;
    }

    // (b) Active sweep: release stopped voices and unrefreshed loops.
    for (NuEListNode<NuSound3Voice> *node = g_NuSoundVoicesActive.head->next; node != g_NuSoundVoicesActive.tail;) {
        NuEListNode<NuSound3Voice> *next = node->next;
        NuSoundVoice *voice = (NuSoundVoice *)node->data.weak_ptr.obj;

        if (voice != NULL) {
            NuSoundVoice::PlayState state = voice->GetState();
            if (state != NuSoundVoice::PLAYSTATE_STOPPED && node->data.pause_counter < 16) {
                if ((voice->flags & 8) != 0) {
                    node->data.pause_counter++;
                }
                node = next;
                continue;
            }
            voice->Stop(true);
            NuSound.ReleaseVoice(reinterpret_cast<NuSoundVoice *>(node->data.weak_ptr.obj));
        }

        StreamListRemove(&g_NuSoundVoicesActive, node);
        delete node;
        node = next;
    }

    // (c) Pending playback: create voices while the budget allows.
    while (NuSound.voice_count < 30) {
        NuEListNode<NuSound3Voice> *node = g_NuSoundVoicesPendingPlayback.head->next;
        if (node == g_NuSoundVoicesPendingPlayback.tail) {
            break;
        }

        StreamListRemove(&g_NuSoundVoicesPendingPlayback, node);
        NuSoundVoice *voice = NuSound.CreateVoice(node->data.source, node->data.has_3d);
        if (voice == NULL) {
            delete node;
            continue;
        }

        node->data.weak_ptr.Set(voice);
        voice->SetAutoDelete(true);
        voice->SetVolume(PS2VolumeToScalar(node->data.volume));
        voice->SetPitch(node->data.pitch);
        if (node->data.position_source != NULL) {
            voice->SetFalloff(node->data.falloff_a, node->data.falloff_b, NuSoundSystem::FalloffType::LINEAR);
            voice->SetPosition((VuVec *)&node->data.position);
            voice->SetSurroundMode(NuSoundSystem::SurroundMode::ZERO);
            voice->SetListeners(NuSound.GetListeners());
        }
        voice->Play();

        StreamListPushBack(&g_NuSoundVoicesActive, node);
    }

    pthread_mutex_unlock(&NuSound.mutex);

    // (d) The two stereo-stream slots: create the pending voice, keep its
    // volume in sync, and tear the stream down once the voice stopped.
    for (i32 i = 0; i < 2; i++) {
        NuSoundStream *stream = g_NuSoundStreams[i];
        if (stream == NULL) {
            continue;
        }

        if (stream->loop != 0 && NuSound3Stream::mVoice.obj == NULL) {
            NuSoundStreamingSample *sample = stream->stream;
            if (sample != NULL && sample->GetLoadState() == NuSoundSample::LoadState::STREAM_READY &&
                stream->stream->GetResourceCount() >= 1 && stream->stream->GetThreadQueueCount() == 0) {
                NuSound3Stream::mVoice.Set(NuSound.CreateVoice(stream->stream, stream->field_0xc != 0));
                if (NuSound3Stream::mVoice.obj != NULL) {
                    ((NuSoundVoice *)NuSound3Stream::mVoice.obj)->SetAutoDelete(false);
                    ((NuSoundVoice *)NuSound3Stream::mVoice.obj)->SetVolume(PS2VolumeToScalar(stream->ps2volume));
                    ((NuSoundVoice *)NuSound3Stream::mVoice.obj)->Play();
                    stream->loop = 0;
                    stream->field_0xd = 1;
                }
            }
        } else if (stream->field_0xd != 0) {
            if (NuSound3Stream::mVoice.obj != NULL) {
                ((NuSoundVoice *)NuSound3Stream::mVoice.obj)->SetVolume(PS2VolumeToScalar(stream->ps2volume));
                if (((NuSoundVoice *)NuSound3Stream::mVoice.obj)->GetState() == NuSoundVoice::PLAYSTATE_STOPPED) {
                    NuSound3StopStereoStream(i);
                }
            }
        }

        if (stream->field_0xd != 0 && NuSound3Stream::mVoice.obj == NULL) {
            NuSound3StopStereoStream(i);
        }

        // Slot cleanup: fully unloaded streams free their node.
        if (stream->stream == NULL) {
            delete stream;
            g_NuSoundStreams[i] = NULL;
        } else if (stream->stream->GetLoadState() == NuSoundSample::LoadState::NOT_LOADED &&
                   stream->stream->GetResourceCount() == 0 && stream->stream->GetThreadQueueCount() == 0) {
            delete stream;
            g_NuSoundStreams[i] = NULL;
        }
    }

    // (e) Wake the sample loader when the registered request table changes.
    for (i32 i = 0; i < g_NuSoundNumLoadBitShorts; i++) {
        if (g_NuSoundLoadBitsCache[i] != g_NuSoundLoadBits[i]) {
            memmove(g_NuSoundLoadBitsCache, g_NuSoundLoadBits,
                    static_cast<usize>(g_NuSoundNumLoadBitShorts) * sizeof(u16));

            pthread_mutex_lock(&g_NuSoundLoadTrigger.mutex);
            if (!g_NuSoundLoadTrigger.a) {
                bool broadcast = g_NuSoundLoadTrigger.b;
                g_NuSoundLoadTrigger.a = true;
                if (broadcast) {
                    pthread_cond_broadcast(&g_NuSoundLoadTrigger.cond);
                } else {
                    pthread_cond_signal(&g_NuSoundLoadTrigger.cond);
                }
            }
            pthread_mutex_unlock(&g_NuSoundLoadTrigger.mutex);
        }
    }

    NuSound.Update(NuTimeGetFrameTime());
}

// Reports the state of one stereo stream slot. A ready stream is only
// considered playing after its shared voice has actually started.
i32 NuSound3GetStereoStreamStatus(i32 stream_index) {
    NuSoundStream *stream = g_NuSoundStreams[stream_index];
    if (stream != NULL && stream->stream != NULL) {
        if (stream->stream->GetLoadState() == NuSoundSample::LoadState::STREAM_READY &&
            NuSound3Stream::mVoice.obj != NULL && stream->field_0xd != 0) {
            return NUSOUND_STEREO_STREAM_PLAYING;
        }
        const i32 finished_status = NUSOUND_STEREO_STREAM_FINISHED;
        const i32 resource_count = stream->stream->GetResourceCount();
        if (resource_count <= 0) {
            return NUSOUND_STEREO_STREAM_INACTIVE;
        }
        return finished_status;
    }
    return NUSOUND_STEREO_STREAM_INACTIVE;
}

// Stream key status for the NuMusic voice state machine: 0 = slot empty,
// 1 = stream loaded / playing, 2 = stream finished (the voice stopped or is
// already gone).
i32 NuSound3StreamKeyStatus(i32 stream_index) {
    return NuSound3GetStereoStreamStatus(stream_index);
}

// Volume arrives as the PS2-style 0..16383 fixed point computed by
// NuMusic::Process. The streamer applies it when mixing.
void NuSound3SetStereoStreamVolume(i32 stream_index, i32 volume) {
    NuSoundStream *stream = g_NuSoundStreams[stream_index];
    if (stream != NULL) {
        stream->ps2volume = volume;
        if (stream->field_0xd != 0 && NuSound3Stream::mVoice.obj != NULL) {
            ((NuSoundVoice *)NuSound3Stream::mVoice.obj)->SetVolume(PS2VolumeToScalar(stream->ps2volume));
        }
    }
}

// Decibel attenuation from music.cfg (DUCK/ATTENUATION/GLOBALATTENUATION with
// negative values): -100 dB and below is silence, 0 dB and above is unity.
extern "C" f32 NuSound3AmplitudeTodB(f32 amplitude) {
    return NuSoundSystem::AmplitudeTodB(amplitude);
}

f32 NuSound3dBToAmplitude(f32 db) {
    if (db <= -100.0f) {
        return 0.0f;
    }
    if (db >= 0.0f) {
        return 1.0f;
    }
    return NuExp10(db / 20.0f);
}

void NuSound3SetSampleTable(nusound_filename_info_s *info, variptr_u *buffer_start, variptr_u buffer_end) {
    if (info == NULL) {
        return;
    }

    for (; info != NULL && info->index != -1; info++) {
        // TODO: dont cast classes
        if (info->index > 0xfff) {
            info->sample = (NuSoundStreamingSample *)NuSound.AddSample(info->filename, NuSoundSystem::FileType::WAV,
                                                                       NuSoundSource::FeedType::ZERO);
        } else {
            info->sample = (NuSoundStreamingSample *)NuSound.AddSample(info->filename, NuSoundSystem::FileType::OGG,
                                                                       NuSoundSource::FeedType::STREAMING);
        }

        g_NuSoundSamples.PushBack(*info);
    }
}
