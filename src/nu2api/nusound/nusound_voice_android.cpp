// NuVoiceAndroid — decompiled from libTTapp.so
// (nu2api.2013/nusound/android/nusound_android.cpp).
//
// Platform voice on top of NuSoundVoice: one OpenSL ES AudioPlayer per voice
// with an ANDROIDSIMPLEBUFFERQUEUE (the device write is the Enqueue call in
// SubmitBuffer) and a VOLUME interface for gain / stereo position. The
// original drove OpenSL through its interface vtables; the exact slots
// libTTapp.so used are reproduced below so the transcription stays faithful.

#include "nu2api_nusound_types.h"

#include "decomp.h"

#include <new>
#include <stdio.h>
#include <string.h>

#include "nu2api/nucore/nucore.hpp"
#include "nu2api/nusound/nusound_android.hpp"
#include "nu2api/nusound/opensles_abi.hpp"
#include "nu2api/nusound/nusound_streamer.hpp"

#include <math.h>

namespace {

    // OpenSL interface vtable slots as used by libTTapp.so.
    typedef u32 (*ObjectRealizeFn)(void *, u32);
    typedef u32 (*ObjectResumeFn)(void *, u32);
    typedef u32 (*ObjectGetStateFn)(void *, u32 *);
    typedef u32 (*ObjectGetInterfaceFn)(void *, const void *, void **);
    typedef u32 (*ObjectDestroyFn)(void *);
    typedef u32 (*EngineCreateAudioPlayerFn)(void *, void **, void *, void *, u32, const void **, const u32 *);
    typedef u32 (*PlaySetPlayStateFn)(void *, u32);
    typedef u32 (*PlayGetPlayStateFn)(void *, u32 *);
    typedef u32 (*PlayGetPositionFn)(void *, u32 *);
    typedef u32 (*PlayRegisterCallbackFn)(void *, void (*)(const SLPlayItf_ *const *, void *, u32), void *);
    typedef u32 (*PlaySetCallbackEventsMaskFn)(void *, u32);
    typedef u32 (*QueueEnqueueFn)(void *, void *, u32);
    typedef u32 (*QueueClearFn)(void *);
    typedef u32 (*QueueGetStateFn)(void *, SLAndroidSimpleBufferQueueState_ *);
    typedef u32 (*VolumeSetVolumeLevelFn)(void *, i32);
    typedef u32 (*VolumeEnableStereoPositionFn)(void *, u32);
    typedef u32 (*VolumeSetStereoPositionFn)(void *, i32);

    struct OpenSLBufferQueueLocator {
        u32 locator_type;
        u32 num_buffers;
    };

    struct OpenSLPcmFormat {
        u32 format_type;
        u32 channels;
        u32 sample_rate;
        u32 bits;
        u32 container_bits;
        u32 speaker_mask;
        u32 endianness;
    };

    struct OpenSLDataEndpoint {
        void *locator;
        void *format;
    };

    struct OpenSLOutputMixLocator {
        u32 locator_type;
        void *output_mix;
    };

#define SL_SLOT(itf, fn_type, byte_offset) (*(fn_type *)((char *)(*(void **)(itf)) + (byte_offset)))

} // namespace

// ---------------------------------------------------------------------------
// construction / destruction
// ---------------------------------------------------------------------------

NuVoiceAndroid::NuVoiceAndroid(NuSoundSource *sound_source, bool loop) : NuSoundVoice(sound_source, loop) {
    this->player_object = NULL;
    this->play_interface = NULL;
    this->queue_interface = NULL;
    this->field4_0x158 = NULL;
    this->volume_interface = NULL;

    NuSoundMutexInit(&this->mutex);

    this->field7_0x164 = 0;
    this->field8_0x168 = 0;
    this->field9_0x16c = 0;
    this->field10_0x170 = 0;
    this->field11_0x174 = 0;
    this->field12_0x178 = 0;

    this->last_volume_level = -0x8000; // muted until the first mix arrives
    this->hardware_flags = 0;

    // libTTapp.so ctor tail (0x32c1ae): the platform voice builds its OpenSL
    // player immediately, before any Play.
    this->CreateHardwareVoice();
}

NuVoiceAndroid::~NuVoiceAndroid() {
    this->DestroyHardwareVoice();
    NuSoundMutexDestroy(&this->mutex);
}

// ---------------------------------------------------------------------------
// the device write
// ---------------------------------------------------------------------------

void NuVoiceAndroid::SubmitBuffer(NuSoundBuffer *buffer) {
    if (buffer == NULL || this->queue_interface == NULL || *(void **)this->queue_interface == NULL) {
        return;
    }

    u32 max_buffer_size = this->sound_source->GetMaxBufferSize();
    NuSoundBuffer::Context &context = buffer->GetCurrentContext();
    u32 size = (u32)context.size2;
    NuSoundStreamDesc *desc = this->sound_source->GetStreamDesc();
    u32 blocks = size / desc->GetBlockSize();

    NuSoundMutexLock(&this->mutex);

    char debug[256] = {};
    sprintf(debug + strlen(debug), "%s ", this->sound_source->GetName());
    strcat(debug, "Submit   ");
    sprintf(debug + strlen(debug), "%12d ", blocks);

    if (size != 0) {
        u32 error =
            SL_SLOT(this->queue_interface, QueueEnqueueFn, 0)(this->queue_interface, buffer->GetAddress(), size);
        NuSoundAndroid::ReportErrorCode(error, "Enqueue buffer");
        this->queued_buffers++;
    }

    if (size < max_buffer_size) {
        this->flags2 |= 2; // 0x32bd0e
        if (size == 0) {
            this->hardware_flags |= 2; // 0x32bd16
        }
    }

    NuSoundMutexUnlock(&this->mutex);
}

// ---------------------------------------------------------------------------
// device lifecycle
// ---------------------------------------------------------------------------

bool NuVoiceAndroid::CreateHardwareVoice() {
    if (this->sound_source == NULL) {
        return false;
    }
    NuSoundStreamDesc *desc = this->sound_source->GetStreamDesc();
    if (desc == NULL) {
        return false;
    }

    struct {
        OpenSLBufferQueueLocator locator;
        OpenSLDataEndpoint audio_src;
        OpenSLOutputMixLocator mix_locator;
        OpenSLDataEndpoint audio_sink;
        const void *iids[2];
        u32 required[2];
        OpenSLPcmFormat pcm_format;
    } data;

    data.pcm_format.format_type = 2;
    u32 channels = (u32)desc->GetNumChannels();
    data.pcm_format.channels = channels;
    if (channels == 1) {
        data.pcm_format.speaker_mask = 4;
    } else if (channels == 2) {
        data.pcm_format.speaker_mask = 3;
    } else {
        return false;
    }

    u32 rate_millis = (u32)desc->GetSampleRate() * 1000;
    data.pcm_format.sample_rate = rate_millis;
    if (NuSoundAndroid::IsValidSampleRate(rate_millis) == false) {
        return false;
    }

    u32 bits = desc->GetBitsPerChannel();
    data.pcm_format.bits = bits;
    if (NuSoundAndroid::IsValidBitRate(bits) == false) {
        return false;
    }
    data.pcm_format.container_bits = desc->GetBitsPerChannel();
    data.pcm_format.endianness = 2;

    // SLDataLocator_AndroidSimpleBufferQueue { locator type, numBuffers = 2 }.
    data.locator.locator_type = 0x800007bd;
    data.locator.num_buffers = 2;
    // SLDataFormat_PCM { format type, channels, rate (milliHz), bits,
    // container bits, channel mask, little endian }.
    data.audio_src.locator = &data.locator;
    data.audio_src.format = &data.pcm_format;

    // Output mix sink.
    data.mix_locator.locator_type = 4;
    data.mix_locator.output_mix = NuSoundSystem::Get()->output_mix;
    data.audio_sink.locator = &data.mix_locator;
    data.audio_sink.format = NULL;

    data.iids[0] = SL_IID_ANDROIDSIMPLEBUFFERQUEUE;
    data.iids[1] = SL_IID_VOLUME;
    data.required[0] = 1;
    data.required[1] = 1;

    u32 error = SL_SLOT(NuSoundSystem::Get()->audio_engine, EngineCreateAudioPlayerFn,
                        8)(NuSoundSystem::Get()->audio_engine, &this->player_object, &data.audio_src, &data.audio_sink,
                           2, data.iids, data.required);
    if (NuSoundAndroid::ReportErrorCode(error, "Create audio player") != 0) {
        return false;
    }

    if (this->RealiseObject() == false) {
        return false;
    }

    return this->GetInterfaces();
}

bool NuVoiceAndroid::RealiseObject() {
    if (this->player_object == NULL || *(void **)this->player_object == NULL) {
        return false;
    }
    u32 error = SL_SLOT(this->player_object, ObjectRealizeFn, 0)(this->player_object, 0);
    return NuSoundAndroid::ReportErrorCode(error, "Realize player object") == 0;
}

bool NuVoiceAndroid::GetInterfaces() {
    u32 error = SL_SLOT(this->player_object, ObjectGetInterfaceFn, 0xc)(this->player_object, SL_IID_PLAY,
                                                                        &this->play_interface);
    if (NuSoundAndroid::ReportErrorCode(error, "Get the play interface") != 0) {
        return false;
    }

    error = SL_SLOT(this->player_object, ObjectGetInterfaceFn,
                    0xc)(this->player_object, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &this->queue_interface);
    if (NuSoundAndroid::ReportErrorCode(error, "Get the buffer queue interface") != 0) {
        return false;
    }

    error = SL_SLOT(this->player_object, ObjectGetInterfaceFn, 0xc)(this->player_object, SL_IID_VOLUME,
                                                                    &this->volume_interface);
    if (NuSoundAndroid::ReportErrorCode(error, "Get the volume interface") != 0) {
        return false;
    }

    error = SL_SLOT(this->play_interface, PlayRegisterCallbackFn, 0x10)(this->play_interface,
                                                                        NuVoiceAndroid::PlayerCallback, this);
    if (NuSoundAndroid::ReportErrorCode(error, "register callback on the play interface") != 0) {
        return false;
    }

    error = SL_SLOT(this->play_interface, PlaySetCallbackEventsMaskFn, 0x14)(this->play_interface, 0x1f);
    return NuSoundAndroid::ReportErrorCode(error, "set callback events mask on the play interface") == 0;
}

void NuVoiceAndroid::StartHardwareVoice() {
    if (this->play_interface == NULL || *(void **)this->play_interface == NULL) {
        return;
    }

    // libTTapp.so 0x32b6d4: starting a hardware voice resets all of the
    // platform play-position tracking before arming the deferred start.
    this->last_volume_level = -1;
    this->field7_0x164 = 0;
    this->field8_0x168 = 0;
    this->field9_0x16c = 0;
    this->field10_0x170 = 0;
    this->field11_0x174 = 0;
    this->field12_0x178 = 0;
    this->hardware_flags |= 1;
}

void NuVoiceAndroid::StopHardwareVoice() {
    if (this->play_interface == NULL || *(void **)this->play_interface == NULL) {
        return;
    }

    u32 error = SL_SLOT(this->play_interface, PlaySetPlayStateFn, 0)(this->play_interface, 1);
    NuSoundAndroid::ReportErrorCode(error, "Set the player's state to stopped");

    error = SL_SLOT(this->queue_interface, QueueClearFn, 4)(this->queue_interface);
    NuSoundAndroid::ReportErrorCode(error, "Cleared the buffer queue");

    this->flags2 &= 0xfd;
}

void NuVoiceAndroid::PauseHardwareVoice() {
    if (this->play_interface == NULL || *(void **)this->play_interface == NULL) {
        return;
    }

    u32 error = SL_SLOT(this->play_interface, PlaySetPlayStateFn, 0)(this->play_interface, 2);
    NuSoundAndroid::ReportErrorCode(error, "Set the player's state to paused");
}

void NuVoiceAndroid::ResumeHardwareVoice() {
    if (this->play_interface == NULL || *(void **)this->play_interface == NULL) {
        return;
    }

    u32 error = SL_SLOT(this->play_interface, PlaySetPlayStateFn, 0)(this->play_interface, 3);
    NuSoundAndroid::ReportErrorCode(error, "Set the player's state to playing (resume)");
}

void NuVoiceAndroid::DestroyHardwareVoice() {
    if (this->player_object == NULL) {
        return;
    }

    SL_SLOT(this->player_object, ObjectDestroyFn, 0x18)(this->player_object);

    this->player_object = NULL;
    this->play_interface = NULL;
    this->queue_interface = NULL;
    this->field4_0x158 = NULL;
    this->volume_interface = NULL;
}

// ---------------------------------------------------------------------------
// per-frame device state
// ---------------------------------------------------------------------------

bool NuVoiceAndroid::UpdateState() {
    // libTTapp.so 0x32c1ea: the poll reads the PLAYER OBJECT's state (object
    // vtable slot 0x8, SLObjectItf::GetState) — voice+0x14c is the object,
    // the interfaces hang off it (see GetInterfaces).
    if (this->player_object == NULL || *(void **)this->player_object == NULL) {
        return false;
    }

    u32 state = 2; // SL_OBJECT_STATE_SUSPENDED default
    u32 error = SL_SLOT(this->player_object, ObjectGetStateFn, 8)(this->player_object, &state);
    if (NuSoundAndroid::ReportErrorCode(error, "Get the object state") != 0) {
        return false;
    }

    if (state != 1) {
        if (state != 3) {
            return true;
        }
        // 0x32c260: object vtable slot 0x4 (SLObjectItf::Resume).
        error = SL_SLOT(this->player_object, ObjectResumeFn, 4)(this->player_object, 0);
        return NuSoundAndroid::ReportErrorCode(error, "resume the player object") == 0;
    }

    // Realized: a looping voice re-realizes and restarts per the voice state;
    // a non-looping voice is finished.
    if ((this->flags2 & 8) != 0) {
        if (this->RealiseObject() == false) {
            return false;
        }
        // This apparently inverted test is what the original executes
        // (libTTapp.so 0x32c2a4: test al; jne 0x32c250). GetInterfaces returns
        // true on success, so the priming Update performed by Play returns
        // false here. Since the logical voice is still STOPPED, the caller's
        // Stop(true) is a no-op and the initial queued buffers remain intact.
        if (this->GetInterfaces()) {
            return false;
        }

        NuSoundVoice::PlayState voice_state = this->GetState();
        if (voice_state == PLAYSTATE_STOPPED) {
            this->StopHardwareVoice();
            return true;
        }
        if (voice_state == PLAYSTATE_PAUSED) {
            this->StartHardwareVoice();
            this->PauseHardwareVoice();
            return true;
        }
        this->StartHardwareVoice();
        return true;
    }

    return false;
}

bool NuVoiceAndroid::UpdateQueue() {
    if (this->queue_interface == NULL || *(void **)this->queue_interface == NULL) {
        return true;
    }

    SLAndroidSimpleBufferQueueState_ state;
    u32 error = SL_SLOT(this->queue_interface, QueueGetStateFn, 8)(this->queue_interface, &state);
    if (NuSoundAndroid::ReportErrorCode(error, "Get queue state") != 0) {
        return false;
    }

    if (this->sound_source->feed_type == NuSoundSource::FeedType::STREAMING && !this->source_flags.last_buffer_queued) {
        // Starvation watchdog: remember whether the queue ever ran ahead, and
        // request a refill as soon as it runs low.
        // libTTapp.so 0x32c37e..0x32c3ad reads and writes voice+0x17e,
        // NuVoiceAndroid::hardware_flags. Using NuSoundVoice::flags (+0x31)
        // left bit 4 invisible to UpdateHardwareVoice and delayed every refill
        // until HEADATEND.
        if (!this->hardware_state.queue_ran_ahead) {
            if (state.count > 1) {
                this->hardware_state.queue_ran_ahead = 1;
            }
        } else if (state.count < 2) {
            this->hardware_state.queue_ran_ahead = 0;
            this->hardware_state.request_buffer = 1;
        }
    }
    return true;
}

void NuVoiceAndroid::UpdateHardwareVoice(f32 frametime) {
    (void)frametime;

    if (this->UpdateState() == false) {
        this->Stop(true);
        return;
    }

    this->UpdateQueue();

    u8 flags = this->hardware_flags;
    if ((flags & 1) == 0) {
        if ((flags & 2) != 0) {
            this->Stop(true);

            u32 state = 3;
            u32 error = SL_SLOT(this->play_interface, PlayGetPlayStateFn, 4)(this->play_interface, &state);
            NuSoundAndroid::ReportErrorCode(error, "Get the player state");
            if (state == 1) {
                this->hardware_flags &= 0xfd;
            }
        }
    } else {
        u32 error = SL_SLOT(this->play_interface, PlaySetPlayStateFn, 0)(this->play_interface, 3);
        u32 reported = NuSoundAndroid::ReportErrorCode(error, "Set the player's state to playing");
        if (reported == 0) {
            this->hardware_flags &= 0xfe;
        }
    }

    if ((this->hardware_flags & 4) != 0) {
        this->sound_source->RequestBuffer((this->flags2 >> 3) & 1, this);
        this->hardware_flags &= 0xfb;
    }

    this->UpdateSamplePlaybackCount();
}

void NuVoiceAndroid::ApplyHardwareVoiceMix() {
    if (this->volume_interface == NULL || *(void **)this->volume_interface == NULL) {
        return;
    }

    f32 gain = this->field67_0xa8;
    i16 level = -0x8000;
    if (gain >= 0.01f) {
        level = (i16)(i32)(log10((f64)gain) * 2000.0);
    }

    if (level != this->last_volume_level) {
        u32 error = SL_SLOT(this->volume_interface, VolumeSetVolumeLevelFn, 0)(this->volume_interface, level);
        NuSoundAndroid::ReportErrorCode(error, "Volume SetVolumeLevel");
        this->last_volume_level = level;
    }

    NuSoundStreamDesc *desc = this->sound_source->GetStreamDesc();
    u32 channels = (u32)desc->GetNumChannels();
    if (channels == 1) {
        // Mono sources are panned through the stereo position interface from
        // the eight positional gains.
        f32 stereo_gains[64] = {0};
        NuSoundMixer mixer(static_cast<NuSoundSystem::ChannelConfig>(1), static_cast<NuSoundSystem::ChannelConfig>(2),
                           NuSoundMixer::OutputLayout::ONE, (NuSoundSystem::DownmixType)this->downmixer_type,
                           this->routing_table);
        mixer.Mix(this->mix_gains, stereo_gains);

        f32 left = stereo_gains[0];
        f32 right = stereo_gains[1];
        f32 maximum = left > right ? left : right;
        i16 pan = 0;
        if (maximum > 0.0f) {
            pan = (i16)(i32)(((right - left) / maximum) * 1000.0f);
        }

        u32 error = SL_SLOT(this->volume_interface, VolumeEnableStereoPositionFn, 0x14)(this->volume_interface, 1);
        NuSoundAndroid::ReportErrorCode(error, "Volume EnableStereoPosition(true)");

        error = SL_SLOT(this->volume_interface, VolumeSetStereoPositionFn, 0x1c)(this->volume_interface, pan);
        NuSoundAndroid::ReportErrorCode(error, "Volume SetStereoPosition");
    } else {
        u32 error = SL_SLOT(this->volume_interface, VolumeEnableStereoPositionFn, 0x14)(this->volume_interface, 0);
        NuSoundAndroid::ReportErrorCode(error, "Volume EnableStereoPosition(false)");
    }
}

// ---------------------------------------------------------------------------
// async events
// ---------------------------------------------------------------------------

void NuVoiceAndroid::OnPlayerEvent(u32 event) {
    if ((event & 1) == 0) {
        return;
    }

    if (this->sound_source->feed_type == NuSoundSource::FeedType::STREAMING) {
        NuSoundMutexLock(&this->mutex);
        if (!this->source_flags.looping && this->source_flags.last_buffer_queued) {
            goto finished;
        }
    } else {
        NuSoundMutexLock(&this->mutex);
        if (!this->source_flags.looping) {
            goto finished;
        }
    }
    this->hardware_state.request_buffer = 1;
    goto unlock;
finished:
    this->hardware_state.stop = 1;
unlock:
    NuSoundMutexUnlock(&this->mutex);
}

void NuVoiceAndroid::PlayerCallback(const SLPlayItf_ *const *player, void *context, u32 event) {
    (void)player;
    if (context != NULL) {
        ((NuVoiceAndroid *)context)->OnPlayerEvent(event);
    }
}

// ---------------------------------------------------------------------------
// playback position
// ---------------------------------------------------------------------------

void NuVoiceAndroid::UpdateSamplePlaybackCount() {
    if (this->play_interface == NULL || *(void **)this->play_interface == NULL) {
        return;
    }

    u32 millisec = 0;
    u32 error = SL_SLOT(this->play_interface, PlayGetPositionFn, 0xc)(this->play_interface, &millisec);
    if (NuSoundAndroid::ReportErrorCode(error, "Player GetPosition") != 0) {
        return;
    }

    NuSoundSource *source = this->sound_source;
    NuSoundStreamDesc *desc = source->GetStreamDesc();
    i32 rate = (i32)desc->GetSampleRate();
    i32 position = (rate / 1000) * (i32)millisec;

    if (source->feed_type != NuSoundSource::FeedType::STREAMING) {
        this->field11_0x174 = position;
        this->field12_0x178 = 0;
        return;
    }

    u64 max_buffer_size = (u64)(i64)(i32)source->GetMaxBufferSize();
    u64 block_size = (u64)(i64)(i32)desc->GetBlockSize();
    u64 block_samples = max_buffer_size / block_size;
    u64 relative_position = (u64)(u32)position % block_samples;

    u64 previous_position = ((u64)(u32)this->field8_0x168 << 32) | (u32)this->field7_0x164;
    u64 wraps = ((u64)(u32)this->field10_0x170 << 32) | (u32)this->field9_0x16c;
    if (relative_position < previous_position) {
        wraps++;
        this->field9_0x16c = (i32)wraps;
        this->field10_0x170 = (i32)(wraps >> 32);
    }

    this->field7_0x164 = (i32)relative_position;
    this->field8_0x168 = (i32)(relative_position >> 32);

    u64 wrapped_position = block_samples * wraps;
    u64 estimated_position = relative_position + wrapped_position;
    bool player_position = true;
    u64 samples = (u64)(u32)position;
    if (samples < estimated_position) {
        player_position = false;
        samples = estimated_position;
    }

    this->field11_0x174 = (i32)samples;
    this->field12_0x178 = (i32)(samples >> 32);

    char debug[256] = {};
    strcat(debug, "Position ");
    sprintf(debug + strlen(debug), "%12d ", (u64)(u32)position);
    sprintf(debug + strlen(debug), "%12d ", relative_position);
    sprintf(debug + strlen(debug), "%12d ", block_samples);
    sprintf(debug + strlen(debug), "%12d ", wraps);
    sprintf(debug + strlen(debug), "%12d ", wrapped_position);
    sprintf(debug + strlen(debug), "%12d ", estimated_position);
    strcpy(debug + strlen(debug), player_position ? "PLAYER" : "ESTIMATED");
}

u64 NuVoiceAndroid::GetPlaybackPositionSamples() {
    return *reinterpret_cast<u64 *>(&this->field11_0x174);
}

// ---------------------------------------------------------------------------
// voice factory
// ---------------------------------------------------------------------------

NuSoundVoice *NuSoundVoiceFactoryAndroid_PCM::CreateVoice(NuSoundSource *source, bool loop) {
    NuVoiceAndroid *voice = (NuVoiceAndroid *)NuSoundSystem::_AllocMemory(
        NuSoundSystem::MemoryDiscipline::SCRATCH, sizeof(NuVoiceAndroid), 4,
        "i:/SagaTouch-Android_9176564/nu2api.2013/nusound/android/nusound_android.cpp:292");
    if (voice != NULL) {
        new (voice) NuVoiceAndroid(source, loop);
    }
    return voice;
}

NuSoundVoiceFactoryList::NuSoundVoiceFactoryList() {
    factories = NULL;
    length = 0;
    capacity = 0;
    factories = static_cast<NuSoundVoiceFactory **>(NuMemoryGet()->GetThreadMem()->_BlockReAlloc(
        factories, 16 * sizeof(NuSoundVoiceFactory *), 4, 0x41, "", NUMEMORY_CATEGORY_NONE));
    length = 16;
    capacity = 16;
}

void NuSoundVoiceFactoryList::RegisterFactory(NuSoundVoiceFactory *factory, NuSoundStreamDesc::DataFormat format) {
    this->factories[(u32)format] = factory;
}

NuSoundVoiceFactory *NuSoundVoiceFactoryList::GetFactory(NuSoundStreamDesc::DataFormat format) {
    return this->factories[(u32)format];
}
