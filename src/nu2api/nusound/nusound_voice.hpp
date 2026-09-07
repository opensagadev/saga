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
  public:
    enum PlayState {
        PLAYSTATE_STOPPED = 1,
        PLAYSTATE_PAUSED = 2,
        PLAYSTATE_PLAYING = 3,
    };

    // Weak-reference bookkeeping (the original embedded NuSoundWeakPtrObj at
    // the start of the voice). queued_buffers counts buffers handed to the
    // hardware but not yet consumed; Play() refuses to double-start while it
    // is non-zero.
    u32 queued_buffers;

    // System intrusive list links.
    NuSoundVoice *field_0x24;
    NuSoundVoice *field_0x28;

    // +0x2c sound source; +0x30/0x31 the flags/flags2 bytes.
    NuSoundSource *sound_source;
    u8 flags;  // low nybble: pause counter; bit3: request loop; bit4: mix update
    u8 flags2; // bit0: auto delete; bit1: last buffer queued; bit2: stop effects
               // running; bit3: looping (from the CreateVoice loop argument)

    u32 surround_mode;
    NuSoundSystem::DownmixType field15_0x38;
    NuSoundRoutingTable *field16_0x3c;

    // Effects list (elist nodes at +0x40..+0x48).
    NuList<NuSoundEffect *> effects;

    // +0x5c..0x7c: the eight output channel gains (the positional mix).
    f32 mix_gains[8];

    void *field56_0x7c;

    NuSoundEffect::ManagedReference positional_references[2];

    f32 field63_0x98;
    f32 field64_0x9c;
    f32 field65_0xa0; // falloff attenuation
    f32 field66_0xa4;
    f32 field67_0xa8; // final mix scalar fed to the hardware volume
    f32 field68_0xac; // pitch scale

    f32 field69_0xb0; // 20.0
    f32 field70_0xb4; // 180.0
    f32 field71_0xb8; // 70.0
    f32 field72_0xbc;
    f32 field73_0xc0;
    f32 field74_0xc4; // 1.0

    VuVec position;
    VuVec direction;
    VuVec velocity;
    f32 pitch;        // +0xf8
    f32 volume;       // +0xfc
    f32 falloff_a;    // +0x100
    f32 falloff_b;    // +0x104
    u32 falloff_type; // +0x108

    f32 field113_0x10c; // LFE gain
    f32 field114_0x110;
    u32 field115_0x114; // 1
    u8 control_118;
    u8 mix_flags;
    u8 control_11a;
    u8 control_11b;

    NuSoundBus *output_bus; // +0x11c, defaults to NuSoundSystem::sMasterBus

    struct HandleLinks {
        NuSoundHandle *previous;
        NuSoundHandle *next;
    } handles_start, handles_end;
    NuSoundHandle *handles_head;
    NuSoundHandle *handles_tail;
    u32 handle_count;
    NuEList<NuSoundListener, DefaultElist> const *listeners;

    PlayState state; // +0x140, guarded by sStateCriticalSection
    f32 field130_0x144;
    i32 field131_0x148; // -1

    static pthread_mutex_t sStateCriticalSection;

  public:
    NuSoundVoice(NuSoundSource *sound_source, bool loop);
    virtual ~NuSoundVoice();

    // NuSoundBufferCallback: implemented by NuVoiceAndroid (the device write).
    void SubmitBuffer(NuSoundBuffer *buffer) override = 0;
    virtual void CheckStarvedBuffers();
    virtual u64 GetPlaybackPositionSamples() = 0;
    virtual void Update(f32 frametime);
    virtual bool CreateHardwareVoice() = 0;
    virtual void DestroyHardwareVoice() = 0;

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

    // Platform half, dispatched through the object vtable in the original.
    virtual void StartHardwareVoice() = 0;            // vtable +0x20
    virtual void StopHardwareVoice() = 0;             // vtable +0x24
    virtual void PauseHardwareVoice() = 0;            // vtable +0x28
    virtual void ResumeHardwareVoice() = 0;           // vtable +0x2c
    virtual void UpdateHardwareVoice(f32 frametime) { // vtable +0x30
    }
    virtual void ApplyHardwareVoiceMix() = 0; // vtable +0x34

    // Remaining original surface (off the title music path; kept as stubs).
    bool AddEffect(NuSoundEffect *effect);
    f32 CalculateFalloffAttenuation(f32 distance);
    f32 CalculateFieldAngle(f32 distance);
    void CalculatePositionalCoefficients(f32 *gains, VuVec const &position, VuMtx const &mtx, f32 falloff_a,
                                         f32 falloff_b);
    i32 GetControllerBits() const;
    const VuVec *GetDirection() const;
    NuSoundSystem::DownmixType GetDownmixerType() const;
    NuSoundEffect *GetEffect(NuSoundEffect::EffectType type);
    NuSoundSystem::FalloffType GetFalloffType() const;
    f32 GetFar() const;
    f32 GetLowFrequencyMix() const;
    f32 GetNear() const;
    u32 GetNumEffects() const;
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

  protected:
    // The 3D positional state; only the surround_mode == 2 (2D omni) path is
    // exercised by the title music.
};

class NuVoiceAndroid : public NuSoundVoice {
  public:
    // OpenSL ES handles (opaque pointers exactly like the original; a host
    // override only has to provide what these point at).
    void *player_object;   // +0x14c SLObjectItf of the audio player
    void *play_interface;  // +0x150 SLPlayItf
    void *queue_interface; // +0x154 SLAndroidSimpleBufferQueueItf
    void *field4_0x158;
    void *volume_interface; // +0x15c SLVolumeItf

    pthread_mutex_t mutex; // +0x160

    u32 field7_0x164;  // playback block position (low)
    u32 field8_0x168;  // playback block position (high)
    u32 field9_0x16c;  // wrap counter (low)
    u32 field10_0x170; // wrap counter (high)
    i32 field11_0x174; // playback position in samples (low)
    i32 field12_0x178; // playback position in samples (high)

    i16 last_volume_level; // +0x17c centibels cache (-32768 = mute)
    u8 hardware_flags;     // +0x17e bit0: start; bit1: stop; bit2: request buffer

  public:
    NuVoiceAndroid(NuSoundSource *sound_source, bool loop);
    virtual ~NuVoiceAndroid();

    // NuSoundBufferCallback: the device write (Enqueue).
    void SubmitBuffer(NuSoundBuffer *buffer) override;

    bool CreateHardwareVoice();
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
    void DestroyHardwareVoice();
    void OnPlayerEvent(u32 event);

    // The SL play-interface callback (static; registered by GetInterfaces).
    static void PlayerCallback(const SLPlayItf_ *const *player, void *context, u32 event);
};
