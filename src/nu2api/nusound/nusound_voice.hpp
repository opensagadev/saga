#pragma once

// NuSoundVoice / NuVoiceAndroid — decompiled from libTTapp.so
// (nu2api.2013/nusound/nusound.cpp, nusound/android/nusound_android.cpp).
//
// NuSoundVoice is the engine-side voice: the play state machine (1 stopped,
// 2 paused, 3 playing), volume/pitch, the eight output-channel gains fed from
// the positional mix, and the per-frame Update. NuVoiceAndroid is the platform
// voice on top of it: one OpenSL ES AudioPlayer per voice (play interface,
// ANDROIDSIMPLEBUFFERQUEUE, volume interface) that receives the mixed PCM.

#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuelist.hpp"
#include "nu2api/nusound/nusound_buffer.hpp"
#include "nu2api/nusound/nusound_system.hpp"
#include "nu2api/nusound/nusound_sync.hpp"
#include "nu2api/nusound/nusound_weakptr.hpp"

#include <pthread.h>

struct SLPlayItf_;

class NuSoundSource;
class NuSoundBus;
class NuSoundEffect;
class NuSoundHandle;
class NuSoundListener;
class NuSoundRoutingTable;
struct nuvec_s;
struct VuMtx;
struct VuVec;

// The voice doubles as the NuSoundBufferCallback the sample hands filled
// buffers to (the original dispatched through the callback vtable slot +8,
// which is NuVoiceAndroid::SubmitBuffer).
class NuSoundBufferCallback : public NuSoundWeakPtrObj<NuSoundBufferCallback> {
  public:
    virtual ~NuSoundBufferCallback() {
    }
    virtual void SubmitBuffer(NuSoundBuffer *buffer) = 0;
};

class NuSoundVoice : public NuSoundBufferCallback {
    friend class NuSoundHandle;

  public:
    enum PlayState {
        PLAYSTATE_STOPPED = 1,
        PLAYSTATE_PAUSED = 2,
        PLAYSTATE_PLAYING = 3,
    };

    // The NuSoundBufferCallback base supplies the weak-reference bookkeeping
    // at +0x00..+0x1f. queued_buffers counts buffers handed to the hardware
    // but not yet consumed.
    u32 queued_buffers;

    // System intrusive list links.
    NuSoundVoice *field_0x24;
    NuSoundVoice *field_0x28;

    // +0x2c sound source; +0x30 owns the lifetime/loop flags.  The playback
    // pause/mix-update flags are a separate byte at +0x119.
    NuSoundSource *sound_source;
    union {
        u8 flags2;
        struct {
            bool auto_delete : 1;
            bool last_buffer_queued : 1;
            bool stop_effects_running : 1;
            bool looping : 1;
            u8 reserved : 4;
        } source_flags;
    };
    u8 padding_0x31[3];

    u32 surround_mode;
    u32 downmixer_type;
    NuSoundRoutingTable *routing_table;

    NuList<NuSoundEffect *> effects; // +0x40

    // +0x5c..0x7c: the eight output channel gains (the positional mix).
    f32 mix_gains[8];

    f32 *custom_surround_mix;

    void *field57_0x80;
    void *field58_0x84;
    void *field59_0x88;

    void *field60_0x8c;
    void *field61_0x90;
    void *field62_0x94;

    f32 field63_0x98;
    f32 field64_0x9c;
    f32 field65_0xa0; // falloff attenuation
    f32 field66_0xa4;
    f32 field67_0xa8; // final mix scalar fed to the hardware volume
    f32 field68_0xac; // pitch scale

    f32 field69_0xb0; // speaker field minimum, 20.0
    f32 field70_0xb4; // speaker field maximum, 180.0
    f32 field71_0xb8; // speaker bleed angle, 70.0
    f32 field72_0xbc; // speaker bleed near
    f32 field73_0xc0; // speaker bleed far
    f32 field74_0xc4; // penetration, 1.0

    VuVec position;  // +0xc8
    VuVec direction; // +0xd8
    VuVec velocity;  // +0xe8

    f32 pitch;        // +0xf8 (SetPitch rejects negative values)
    f32 volume;       // +0xfc (SetVolume rejects values outside 0..1)
    f32 falloff_a;    // +0x100, near distance; defaults to 1.0
    f32 falloff_b;    // +0x104, far distance; defaults to 6.0
    u32 falloff_type; // +0x108

    f32 field113_0x10c; // LFE gain
    f32 start_offset;   // +0x110
    u32 output_devices; // +0x114, defaults to device bit 0
    u8 controller_bits; // +0x118
    u8 flags;           // +0x119: low nybble pause counter; bit4 requests a mix update
    u8 padding_0x11a[2];

    NuSoundBus *output_bus; // +0x11c, defaults to NuSoundSystem::sMasterBus

    NuEList<NuSoundHandle> handles; // +0x120

    NuEList<NuSoundListener, DefaultElist> const *listeners; // +0x13c
    PlayState state;                                         // +0x140, guarded by sStateCriticalSection
    f32 field130_0x144;
    i32 field131_0x148; // -1

    static pthread_mutex_t sStateCriticalSection;

  public:
    NuSoundVoice(NuSoundSource *sound_source, bool loop);
    virtual ~NuSoundVoice();

    // NuSoundBufferCallback: implemented by NuVoiceAndroid (the device write).
    void SubmitBuffer(NuSoundBuffer *buffer) override = 0;

    // Original virtual surface in vtable order.
    virtual void CheckStarvedBuffers();
    virtual u64 GetPlaybackPositionSamples() = 0;
    virtual void Update(f32 frametime);
    virtual bool CreateHardwareVoice() = 0;
    virtual void DestroyHardwareVoice() = 0;
    virtual void StartHardwareVoice() = 0;
    virtual void StopHardwareVoice() = 0;
    virtual void PauseHardwareVoice() = 0;
    virtual void ResumeHardwareVoice() = 0;
    virtual void UpdateHardwareVoice(f32 frametime);
    virtual void ApplyHardwareVoiceMix() = 0;

    // Play state.
    PlayState GetState() const;
    void SetState(PlayState state);
    bool GetAutoDelete() const;
    void SetAutoDelete(bool auto_delete);
    void SetMixUpdate(bool mix_update);
    void SetVolume(f32 volume);
    void SetPitch(f32 pitch);

    // Control.
    void Play();
    void Pause();
    void Resume();
    void Stop(bool with_effects);
    void RegisterHandle(NuSoundHandle *handle);
    void UnregisterHandle(NuSoundHandle *handle);

    // Per-frame processing (NuSoundSystem::Update calls this on every voice).
    void UpdateMix(f32 frametime);
    void CalculatePositionalMix();
    bool BeginStopEffects();
    bool CheckStopEffects();
    bool AreStopEffectsRunning() const;
    f32 CalculateEffectAttenuation();
    f32 CalculateEffectPitchScale();
    void UpdateEffects(f32 frametime, NuSoundEffect::EffectProcessStage stage);
    // Remaining original surface (off the title music path; kept as stubs).
    bool AddEffect(NuSoundEffect *effect);
    f32 CalculateFalloffAttenuation(f32 distance);
    f32 CalculateFieldAngle(f32 distance);
    void CalculatePositionalCoefficients(f32 *gains, VuVec const &position, VuMtx const &mtx,
                                         f32 speaker_field_angle_min, f32 speaker_field_angle_max);
    u8 GetControllerBits() const;
    const VuVec *GetDirection() const;
    NuSoundSystem::DownmixType GetDownmixerType() const;
    NuSoundEffect *GetEffect(NuSoundEffect::EffectType type);
    NuSoundSystem::FalloffType GetFalloffType() const;
    f32 GetFar() const;
    f32 GetLowFrequencyMix() const;
    f32 GetNear() const;
    i32 GetNumEffects() const;
    NuSoundBus *GetOutputBus() const;
    f32 GetPenetration() const;
    f32 GetPitch() const;
    f32 GetPlaybackPositionSeconds();
    const VuVec *GetPosition() const;
    f32 GetReverbWetMix() const;
    NuSoundRoutingTable *GetRoutingTable() const;
    f32 GetSpeakerBleedAngle() const;
    f32 GetSpeakerBleedFar() const;
    f32 GetSpeakerBleedNear() const;
    f32 GetSpeakerFieldAngleMax() const;
    f32 GetSpeakerFieldAngleMin() const;
    f32 GetStartOffset() const;
    NuSoundSystem::SurroundMode GetSurroundMode() const;
    const VuVec *GetVelocity() const;
    f32 GetVolume() const;
    bool IsLooping() const;
    void RemoveEffect(NuSoundEffect *effect);
    void SetControllerBits(i32 bits);
    void SetCustomSurroundMix(f32 *mix);
    void SetDirection(VuVec *direction);
    void SetDownmixerType(NuSoundSystem::DownmixType type);
    void SetFalloff(f32 near, f32 far, NuSoundSystem::FalloffType type);
    void SetLowFrequencyMix(f32 mix);
    void SetOutputBus(NuSoundBus *bus);
    void SetOutputDevices(i32 devices);
    void SetPenetration(f32 penetration);
    void SetPosition(VuVec *position);
    void SetReverbWetMix(f32 mix);
    void SetRoutingTable(NuSoundRoutingTable *table);
    void SetSpeakerBleedAngle(f32 angle);
    void SetSpeakerBleedFar(f32 far);
    void SetSpeakerBleedNear(f32 near);
    void SetSpeakerFieldAngle(f32 min, f32 max);
    void SetStartOffset(f32 offset);
    void SetListeners(NuEList<NuSoundListener, DefaultElist> const *listeners);
    void SetSurroundMode(NuSoundSystem::SurroundMode mode);
    void SetVelocity(VuVec const &velocity);
};

DECOMP_ASSERT(sizeof(NuSoundVoice) == 0x14c, "NuSoundVoice size");

class NuVoiceAndroid : public NuSoundVoice {
  public:
    // OpenSL ES handles (opaque pointers exactly like the original; a host
    // override only has to provide what these point at).
    void *player_object;   // +0x14c SLObjectItf of the audio player
    void *play_interface;  // +0x150 SLPlayItf
    void *queue_interface; // +0x154 SLAndroidSimpleBufferQueueItf
    void *field4_0x158;
    void *volume_interface; // +0x15c SLVolumeItf

    NuSoundMutex mutex; // +0x160

    u32 field7_0x164;  // playback block position (low)
    u32 field8_0x168;  // playback block position (high)
    u32 field9_0x16c;  // wrap counter (low)
    u32 field10_0x170; // wrap counter (high)
    i32 field11_0x174; // playback position in samples (low)
    i32 field12_0x178; // playback position in samples (high)

    i16 last_volume_level; // +0x17c centibels cache (-32768 = mute)
    union {                // +0x17e
        u8 hardware_flags;
        struct {
            u8 start : 1;
            u8 stop : 1;
            u8 request_buffer : 1;
            u8 queue_ran_ahead : 1;
            u8 reserved : 4;
        } hardware_state;
    };

  public:
    NuVoiceAndroid(NuSoundSource *sound_source, bool loop);
    virtual ~NuVoiceAndroid();

    // NuSoundBufferCallback: the device write (Enqueue).
    void SubmitBuffer(NuSoundBuffer *buffer) override;

    bool CreateHardwareVoice() override;
    bool RealiseObject();
    bool GetInterfaces();
    void StartHardwareVoice() override;
    void StopHardwareVoice() override;
    void PauseHardwareVoice() override;
    void ResumeHardwareVoice() override;
    void UpdateHardwareVoice(f32 frametime) override;
    void ApplyHardwareVoiceMix() override;
    bool UpdateQueue();
    bool UpdateState();
    void UpdateSamplePlaybackCount();
    u64 GetPlaybackPositionSamples() override;
    void DestroyHardwareVoice() override;
    void OnPlayerEvent(u32 event);

    // The SL play-interface callback (static; registered by GetInterfaces).
    static void PlayerCallback(const SLPlayItf_ *const *player, void *context, u32 event);
};
