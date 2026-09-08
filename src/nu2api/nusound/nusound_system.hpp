#pragma once

#include "nu2api/nucore/NuMemoryManager.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuelist.hpp"
#include "nu2api/nucore/numap.hpp"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nusound/nulist.hpp"
#include "nu2api/nusound/nusound_memorymanager.hpp"
#include "nu2api/nusound/nusound_source.hpp"
#include "nu2api/nusound/nusound_streamdesc.hpp"
#include "nu2api/nusound/nusound_sync.hpp"

#include "nu2api/nufile/nufile.h"

#include "decomp.h"

struct VuMtx;

class NuSoundLoader;
class NuSoundBus;
class NuSoundSample;
class NuSoundListener;
class NuSoundRoutingTable;
class NuSoundDecoder;
class NuSoundVoice;
class NuThread;
class NuSoundOutOfMemCallback;

class NuSoundEffect {
    friend class NuSoundBus;
    friend class NuSoundSystem;
    friend class NuSoundVoice;

  public:
    enum class EffectType : u32 {
        ATTENUATION = 0,
        PITCH = 1,
        FADER = 2,
        PITCH_RAMP = 3,
        RANDOM_VOLUME = 4,
        RANDOM_PITCH = 5,
        REPEAT = 6,
        UNKNOWN = 7,
        DOPPLER = 8,
    };
    enum class EffectProcessStage : u32 {
        ZERO = 0,
        ONE = 1,
        TWO = 2,
    };

  protected:
    void *field_0x04;
    EffectProcessStage process_stage;
    u32 stop_effect;
    u32 state;
    EffectType type;
    bool enabled;
    u8 padding_0x19[3];
    f32 output_mix;
    f32 pitch_mix;
    bool keep_attached;
    u8 padding_0x25[3];
    NuList<void *> attachments;

  protected:
    NuSoundEffect(EffectType type, EffectProcessStage stage)
        : field_0x04(NULL), process_stage(stage), stop_effect(0), state(1), type(type), enabled(true), output_mix(1.0f),
          pitch_mix(1.0f), keep_attached(false) {
    }

  public:
    virtual bool Initialise();
    virtual void Shutdown();
    virtual void Enable();
    virtual void Disable();
    virtual ~NuSoundEffect();
    virtual bool AttachVoice(NuSoundVoice *voice);
    virtual void DetachVoice(NuSoundVoice *voice);
    virtual void ProcessVoice(NuSoundVoice *voice, f32 frametime);
    virtual bool AttachBus(NuSoundBus *bus);
    virtual void DetachBus(NuSoundBus *bus);
    virtual void ProcessBus(NuSoundBus *bus, f32 frametime);
    virtual void Process(f32 frametime);
};

DECOMP_ASSERT(sizeof(NuSoundEffect) == 0x44, "NuSoundEffect size");

// --- subsystem classes without a dedicated header (definitions live in their
// --- .cpp files; the declarations used to sit in the legacy types catalog) ---

class NuSoundClock {
  public:
    struct Callback {
        virtual void OnCallback(u64 elapsed_ticks, u64 clock_frequency);
        Callback *intrusive_prev;
        Callback *intrusive_next;
    };

  private:
    NuEListOffset<Callback, 4> callbacks;
    u64 clock_frequency;
    u64 previous_ticks;

  public:
    NuSoundClock();
    ~NuSoundClock();

    void AddCallback(Callback *callback);
    void RemoveCallback(Callback *callback);
    void HandleCallbacks();
    u64 GetClockFrequency() const;
    u64 GetTicks() const;
};

DECOMP_ASSERT(sizeof(NuSoundClock) == 0x2c, "NuSoundClock size");

class NuSoundListener {
  public:
    void *field_0x0;
    void *field_0x4;
    void *field_0x8;

  private:
    const VuMtx *head_matrix;
    const VuVec *focus_position;
    const VuVec *screen_position;
    VuVec velocity;
    bool enabled;
    bool focus_position_enabled;
    u8 padding_0x2a[2];
    f32 sensitivity;
    i32 output_devices;

  public:
    NuSoundListener();
    ~NuSoundListener();

    void Enable();
    void Disable();
    bool IsEnabled() const;
    void SetHeadMatrix(const VuMtx *mtx);
    const VuMtx *GetHeadMatrix() const;
    void SetFocusPosition(const VuVec *position);
    const VuVec *GetFocusPosition() const;
    void EnableFocusPosition();
    void DisableFocusPosition();
    bool IsFocusPositionEnabled() const;
    void Set2DScreenPosition(const VuVec *position);
    const VuVec *Get2DScreenPosition() const;
    void SetVelocity(const VuVec &velocity);
    const VuVec *GetVelocity() const;
    void SetSensitivity(f32 sensitivity);
    f32 GetSensitivity() const;
    void SetOutputDevices(i32 devices);
    i32 GetOutputDevices() const;
    const VuVec *GetAttenuationPosition(const VuVec &position) const;
    f32 GetAttenuationDistance(const VuVec &position) const;
    f32 GetHeadDistance(const VuVec &position) const;
};

DECOMP_ASSERT(sizeof(NuSoundListener) == 0x34, "NuSoundListener size");

class NuSoundEffectAttenuation : public NuSoundEffect {
    f32 attenuation;

  public:
    NuSoundEffectAttenuation() : NuSoundEffect(EffectType::ATTENUATION, EffectProcessStage::ZERO), attenuation(1.0f) {
    }
    ~NuSoundEffectAttenuation() override {
    }
    bool AttachBus(NuSoundBus *bus) override;
    void ProcessVoice(NuSoundVoice *voice, f32 frametime) override;
};

class NuSoundEffectPitch : public NuSoundEffect {
  public:
    NuSoundEffectPitch() : NuSoundEffect(EffectType::PITCH, EffectProcessStage::TWO) {
    }
    ~NuSoundEffectPitch() override {
    }
};

class NuSoundEffectRandomVolume : public NuSoundEffect {
  public:
    NuSoundEffectRandomVolume() : NuSoundEffect(EffectType::RANDOM_VOLUME, EffectProcessStage::TWO) {
    }
    ~NuSoundEffectRandomVolume() override {
    }
};

class NuSoundEffectRandomPitch : public NuSoundEffect {
  public:
    NuSoundEffectRandomPitch() : NuSoundEffect(EffectType::RANDOM_PITCH, EffectProcessStage::TWO) {
    }
    ~NuSoundEffectRandomPitch() override {
    }
};

class NuSoundEffectRepeat : public NuSoundEffect {
    f32 delay;
    i32 repeat_count;
    bool armed;
    u8 padding_0x4d[3];
    f32 remaining_delay;

  public:
    NuSoundEffectRepeat()
        : NuSoundEffect(EffectType::REPEAT, EffectProcessStage::ONE), delay(0.0f), repeat_count(1), armed(true),
          remaining_delay(0.0f) {
    }
    ~NuSoundEffectRepeat() override {
    }
    void ProcessVoice(NuSoundVoice *voice, f32 frametime) override;
};

class NuSoundEffectDoppler : public NuSoundEffect {
    f32 speed_of_sound;
    f32 velocity_scale;
    const NuEList<NuSoundListener, DefaultElist> *listeners;

  public:
    NuSoundEffectDoppler();
    ~NuSoundEffectDoppler() override;

    void ProcessVoice(NuSoundVoice *voice, f32 frametime) override;
    void SetParameters(f32 a, f32 b, const NuEList<NuSoundListener, DefaultElist> *listeners);
};

class NuSoundEffectFader : public NuSoundEffect {
  public:
    struct Curve {
        u32 type;
        const void *data;
    };
    enum class FinishState : u32 {
        NONE = 0,
        STOP = 1,
        PAUSE = 2,
        CALLBACK = 3,
    };

  private:
    Curve curve;
    f32 start_mix;
    f32 target_mix;
    f32 duration;
    f32 progress;
    u32 decreasing;
    FinishState finish_state;
    void *callback;
    bool finished;
    u8 padding_0x69[3];

  public:
    NuSoundEffectFader();
    ~NuSoundEffectFader() override;

    bool AttachBus(NuSoundBus *bus) override;
    bool AttachVoice(NuSoundVoice *voice) override;
    void Enable() override;
    void Disable() override;
    void Process(f32 frametime) override;
    void ProcessBus(NuSoundBus *bus, f32 frametime) override;
    void ProcessVoice(NuSoundVoice *voice, f32 frametime) override;
    void SetCurveParams(const Curve &curve);
    void SetParameters(f32 a, f32 b, FinishState state);
};

class NuSoundEffectPitchRamp : public NuSoundEffect {
  public:
    enum class FinishState : u32 {
        NONE = 0,
        STOP = 1,
    };

  private:
    f32 target_pitch;
    f32 duration;
    bool finished;
    u8 padding_0x4d[3];
    FinishState finish_state;

  public:
    NuSoundEffectPitchRamp();
    ~NuSoundEffectPitchRamp() override;

    bool AttachVoice(NuSoundVoice *voice) override;
    void Process(f32 frametime) override;
    void ProcessVoice(NuSoundVoice *voice, f32 frametime) override;
    void SetParameters(f32 a, f32 b, FinishState state);
};

DECOMP_ASSERT(sizeof(NuSoundEffectAttenuation) == 0x48, "NuSoundEffectAttenuation size");
DECOMP_ASSERT(sizeof(NuSoundEffectPitch) == 0x44, "NuSoundEffectPitch size");
DECOMP_ASSERT(sizeof(NuSoundEffectRandomVolume) == 0x44, "NuSoundEffectRandomVolume size");
DECOMP_ASSERT(sizeof(NuSoundEffectRandomPitch) == 0x44, "NuSoundEffectRandomPitch size");
DECOMP_ASSERT(sizeof(NuSoundEffectRepeat) == 0x54, "NuSoundEffectRepeat size");
DECOMP_ASSERT(sizeof(NuSoundEffectDoppler) == 0x50, "NuSoundEffectDoppler size");
DECOMP_ASSERT(sizeof(NuSoundEffectFader) == 0x6c, "NuSoundEffectFader size");
DECOMP_ASSERT(sizeof(NuSoundEffectPitchRamp) == 0x54, "NuSoundEffectPitchRamp size");

// Voice factories (nu2api.2013/nusound/nusound.cpp): one factory per decoded
// data format, indexed by NuSoundStreamDesc::DataFormat.
class NuSoundVoiceFactory {
  public:
    virtual NuSoundVoice *CreateVoice(NuSoundSource *source, bool loop) = 0;
};

class NuSoundVoiceFactoryAndroid_PCM : public NuSoundVoiceFactory {
  public:
    NuSoundVoice *CreateVoice(NuSoundSource *source, bool loop) override;
};

class NuSoundVoiceFactoryList {
  public:
    NuSoundVoiceFactory **factories;
    u32 length;
    u32 capacity;

  public:
    NuSoundVoiceFactoryList();
    ~NuSoundVoiceFactoryList() {
        length = 0;
        if (factories != NULL) {
            NuMemoryGet()->GetThreadMem()->BlockFree(factories, 0);
            capacity = 0;
            factories = NULL;
        }
    }
    void RegisterFactory(NuSoundVoiceFactory *factory, NuSoundStreamDesc::DataFormat format);
    NuSoundVoiceFactory *GetFactory(NuSoundStreamDesc::DataFormat format);
};

class NuSoundSystem {
  public:
    enum class MemoryDiscipline : u32 {
        SCRATCH = 0,
        SAMPLE = 1,
        DECODER = 2,
    };

    enum class ChannelConfig : u32 {};

    enum class AudioChannel : u32 {};
    struct CurveData {
        u8 data[0x800];
    };
    enum class DownmixType : u32 { ZERO = 0, ONE = 1, TWO = 2, THREE = 3 };
    enum class FalloffType : u32 { LINEAR = 0, CURVED = 1 };
    enum class SurroundMode : u32 { ZERO = 0, ONE = 1, TWO = 2, THREE = 3, CUSTOM = 4 };

    // "wav", "adp", "ima", "caf", "xma", "ogg",  "dsp", "msf", "vag", "gcm", "wua", "cbx"
    enum class FileType : u32 {
        WAV = 0,
        ADP = 1,
        IMA = 2,
        CAF = 3,
        XMA = 4,
        OGG = 5,
        DSP = 6,
        MSF = 7,
        VAG = 8,
        GCM = 9,
        WUA = 10,
        CBX = 11,
        _COUNT = 12,
        INVALID = 13,
    };

  public:
    NuSoundCriticalSection mutex;
    NuSoundClock clock;
    NuEListOffset<NuSoundDecoder, 0x20> decoder_list;
    NuSoundSample **samples;
    u32 sample_count;
    NuSoundVoiceFactoryList factory_list;
    NuEListOffset<NuSoundVoice, 0x24> voice_list;
    NuList<NuSoundEffect *> effect_update_list;
    NuEList<NuSoundBus, DefaultElist> bus_list;
    NuEList<NuSoundRoutingTable, DefaultElist> routing_table_list;
    NuEList<NuSoundListener, DefaultElist> listener_list;
    NuMap<u32, CurveData> crossfade_curves;
    u32 field_0xf8;
    u8 padding_0xfc[0xc];
    bool initialised;
    u8 padding_0x109[3];

    // OpenSL ES handles at +0x10c/+0x110/+0x114 in the target object.
    void *engine_object; // SL engine object (realize / GetInterface / destroy)
    void *audio_engine;  // SLEngineItf (CreateAudioPlayer / CreateOutputMix)
    void *output_mix;    // output mix object

  public:
    static NuSoundBus *sMasterBus;
    static NuSoundRoutingTable *sDefaultRoutingTable;
    static i32 sNumAvailableOutputDevices;
    static i32 sOutputConfig;

    static i32 sAllocdMemory[3];
    static i32 sTotalMemory[3];
    static i32 sPeakAllocdMemory[3];
    static u32 sGfxMemorySize;

    static void *sScratchMemory;
    static void *sSampleMemory;
    static void *sDecoderMemory;

    static NuSoundMemoryManager *s_mmSample;
    static NuSoundMemoryManager *s_mmDecoder;

    static const char *sFileExtensions[12];

    static NuSoundSystem *s_staticInstance;

    static NuSoundSystem *GetInstance() {
        return s_staticInstance;
    }

    static struct : NuMemoryManager::IEventHandler {
        u32 unknown;
        void *scratch;
        u32 scratch_size;

        virtual bool AllocatePage(NuMemoryManager *manager, u32 size, u32 _unknown) {
            (void)size;
            (void)_unknown;
            if (scratch == NULL) {
                return false;
            }
            manager->AddPage(scratch, scratch_size, false);
            scratch = NULL;
            return true;
        }
        virtual bool ReleasePage(NuMemoryManager *manager, void *ptr, u32 _unknown) {
            (void)manager;
            (void)ptr;
            (void)_unknown;
            return false;
        }
    } g_handler;

    static NuMemoryManager *sScratchMemMgr;

    NuSoundSystem();

    NuSoundVoice *CreateVoice(NuSoundSource *source, bool loop);
    void ReleaseVoice(NuSoundVoice *voice);
    static bool SourceRequiresDecoder(NuSoundSource *source);
    i32 GetNumAvailableOutputDevices();
    NuSoundRoutingTable *GetDefaultRoutingTable();
    void Update(f32 frametime);

  public:
    static NuSoundLoader *CreateFileLoader(FileType type);
    static void ReleaseFileLoader(NuSoundLoader *loader);

    bool Initialise(i32 size);

    static void *_AllocMemory(MemoryDiscipline disc, u32 size, u32 align, const char *name);

    static u32 FreeMemory(MemoryDiscipline disc, usize address, u32 size);

    static u32 GetStreamBufferSize();
    static u32 GetScratchMemorySize();
    static u32 GetDecoderMemorySize();
    static u32 GetFreeMemory(MemoryDiscipline disc);

    NuSoundSample *AddSample(const char *name, FileType file_type, NuSoundSource::FeedType feed_type);

    const char *GetFileExtension(FileType type);
    static FileType DetermineFileType(const char *path);

    NuSoundSample *GetSample(const char *path);

    i32 GenerateHash(const char *str);

    virtual ~NuSoundSystem();
    virtual NuSoundEffect *CreateEffect(NuSoundEffect::EffectType);
    virtual void ReleaseEffect(NuSoundEffect *);
    virtual NuSoundBus *CreateBus(const char *name, bool is_master);
    virtual NuSoundBus *GetBus(const char *name);
    virtual void ReleaseBus(NuSoundBus *);
    virtual bool IsUserPlayingMusic();
    virtual void PauseUserMusic();
    virtual void ResumeUserMusic();
    virtual bool TitleHasUserMusicControl();
    virtual void OnEnterSystemMenu();
    virtual void OnExitSystemMenu();
    virtual bool InitAudioDevice() = 0;
    virtual void ShutdownAudioDevice() = 0;
    virtual void UpdateAudioDevice() = 0;

    bool AddListener(NuSoundListener *);
    void AddRoutingTable(NuSoundRoutingTable *);
    static f32 AmplitudeTodB(f32 amplitude);
    f32 CalculateCrossfadeHeight(NuSoundSystem::CurveData const &, float) const;
    CurveData *CreateCrossfadeCurve(unsigned int);
    static NuSoundDecoder *CreateDecoder(NuSoundSource *source);
    void DefragmentSampleMemory();
    static FileType DetermineFileType(NUFILETYPE);
    static void Disable();
    static bool FileTypeSupported(NuSoundSystem::FileType type);
    static NuSoundSystem *Get();
    static u32 GetAllocdMemory(NuSoundSystem::MemoryDiscipline discipline);
    static u32 GetBufferAlignment();
    static i32 GetClosestSupportedConfig(i32 config);
    const CurveData *GetCrossfadeCurve(unsigned int) const;
    static FileType GetDefaultFileType(NuSoundSource::FeedType);
    static u32 GetGfxMemorySize();
    static const char *GetLanguageString(bool full);
    static u32 GetLargestMemoryFragment(NuSoundSystem::MemoryDiscipline);
    NuEList<NuSoundListener, DefaultElist> *GetListeners();
    static NuSoundListener *GetNearestRealListener(NuEList<NuSoundListener, DefaultElist> const &, VuVec const &);
    static NuSoundListener *GetNearestFocusListener(NuEList<NuSoundListener, DefaultElist> const &, VuVec const &,
                                                    float &);
    NuSoundVoice *GetOldestVoice(NuSoundSample *, float &);
    i32 GetVoiceCount() const {
        return voice_list.length;
    }
    static i32 GetOutputChannelConfig();
    static u32 GetPeakAllocdMemory(NuSoundSystem::MemoryDiscipline discipline);
    static const char *GetPlatformString();
    NuSoundVoice *GetQuietestVoice(NuSoundSample *, float &);
    NuSoundRoutingTable *GetRoutingTable(char const *);
    static u32 GetTotalMemory(NuSoundSystem::MemoryDiscipline discipline);
    bool LoadSample(NuSoundSample *, void *, int, NuSoundOutOfMemCallback *);
    void PauseAllVoices();
    void PauseVoices(int);
    void *ReAllocMemory(NuSoundSystem::MemoryDiscipline address, unsigned int size, unsigned int unused);
    void ReleaseCrossfadeCurve(unsigned int);
    static void ReleaseDecoder(NuSoundDecoder *decoder);
    void ReleaseSample(NuSoundSample *);
    void RemoveListener(NuSoundListener *);
    void ResumeAllVoices();
    void ResumeVoices(int);
    void SetDefaultRoutingTable(NuSoundRoutingTable *);
    static void SetGfxMemorySize(u32 size);
    void SetMainThreadID(NuThread *);
    void Shutdown();
    void StopAllVoices();
    void StopVoices(NuSoundSource const &);
    void StopVoices(int);
    void UnloadAllSamples();
    bool UnloadSample(NuSoundSample *);
    static f32 dBToAmplitude(f32 db);
};

namespace NuSoundMemory {
    template <typename T> void PushNuListNode(NuList<T> &list, T const &value);
} // namespace NuSoundMemory
// One row of a routing table: an N-in by M-out gain matrix pointing at one of
// the static NuSoundRoutingTable* matrices.
struct NuSoundMixMatrix {
    u32 in;
    u32 out;
    const f32 *matrix;
    bool flag;
    u8 padding[3];

    NuSoundMixMatrix(NuSoundSystem::ChannelConfig from, NuSoundSystem::ChannelConfig to, const f32 *values)
        : in(static_cast<u32>(from)), out(static_cast<u32>(to)), matrix(values), flag(true) {
    }
};

class NuSoundMixer {
  public:
    enum class OutputLayout : u32 { ZERO = 0, ONE = 1 };

  private:
    NuSoundSystem::ChannelConfig input_config;
    NuSoundSystem::ChannelConfig output_config;
    OutputLayout output_layout;
    NuSoundSystem::DownmixType downmix_type;
    NuSoundRoutingTable *routing_table;

  public:
    static u8 sDownmixerChannelMaps[4][8];

    NuSoundMixer(NuSoundSystem::ChannelConfig config, NuSoundSystem::ChannelConfig output,
                 NuSoundMixer::OutputLayout layout, NuSoundSystem::DownmixType downmix, NuSoundRoutingTable *table);
    ~NuSoundMixer();

    void Mix(f32 *in, f32 *out);
    i32 GetOutputIndex(i32 input, i32 output);
};

DECOMP_ASSERT(sizeof(NuSoundMixer) == 0x14, "NuSoundMixer size");
class NuSoundRoutingTable {
    NuSoundRoutingTable *intrusive_prev;
    NuSoundRoutingTable *intrusive_next;
    u16 name_capacity;
    u16 name_length;
    char *name;
    char name_storage[32];
    NuSoundMixMatrix *matrices[36];

  public:
    NuSoundRoutingTable(const char *name);
    NuSoundRoutingTable(const char *name, const NuSoundRoutingTable *parent);

    void SetMatrix(NuSoundSystem::ChannelConfig from, NuSoundSystem::ChannelConfig to, NuSoundMixMatrix *matrix);
    NuSoundMixMatrix *GetMatrix(NuSoundSystem::ChannelConfig from, NuSoundSystem::ChannelConfig to) const;
    static NuSoundSystem::ChannelConfig GetConfig(i32 index);
    static i32 GetIndex(NuSoundSystem::ChannelConfig config);
    const char *GetName() const;
};

DECOMP_ASSERT(sizeof(NuSoundMixMatrix) == 0x10, "NuSoundMixMatrix size");
DECOMP_ASSERT(sizeof(NuSoundRoutingTable) == 0xc0, "NuSoundRoutingTable size");
class NuSoundHandle {
    friend class NuSoundVoice;

    NuSoundHandle *intrusive_prev;
    NuSoundHandle *intrusive_next;
    NuSoundVoice *voice;
    NuList<NuSoundEffect *> effects;

  public:
    static pthread_mutex_t sCriticalSection;

    NuSoundHandle();
    NuSoundHandle(NuSoundHandle &other);
    ~NuSoundHandle();

    void SetVoice(NuSoundVoice *voice);
    NuSoundVoice *GetVoice() const;
    void InvalidateVoice();
    void Play();
    void Pause();
    void Resume();
    void Stop();
    void SetVolume(f32 volume);
    f32 GetVolume() const;
    void SetPitch(f32 pitch);
    f32 GetPitch() const;
    void SetPosition(VuVec *position);
    const VuVec *GetPosition() const;
    void SetVelocity(const VuVec &velocity);
    const VuVec *GetVelocity() const;
    void SetFalloff(f32 near, f32 far, NuSoundSystem::FalloffType type);
    NuSoundSystem::FalloffType GetFalloffType() const;
    f32 GetNear() const;
    f32 GetFar() const;
    i32 GetState() const;
    bool IsLooping() const;
    u64 GetPlaybackPositionSamples();
    f32 GetPlaybackPositionSeconds() const;
    u64 GetTotalLengthSamples() const;
    f32 GetTotalLengthSeconds() const;
    f32 GetLastAttenuation() const;
    NuSoundListener *GetLastAttenuationListener() const;
    f32 GetLastDistanceAttenuation() const;
    f32 GetLastListenerDistance() const;
    NuSoundListener *GetLastPositionalListener() const;
    NuSoundSystem::SurroundMode GetSurroundMode() const;
    bool AddEffect(NuSoundEffect *effect);
    void RemoveEffect(NuSoundEffect *effect);
    NuSoundEffect *GetEffect(NuSoundEffect::EffectType type);
    void ResetFrameCount();
    NuSoundHandle &operator=(NuSoundHandle &other);
    bool operator==(NuSoundHandle const &other);
}; // namespace NuSoundSystem

DECOMP_ASSERT(sizeof(NuSoundHandle) == 0x28, "NuSoundHandle size");

class NuSoundOutOfMemCallback {
  public:
    virtual void operator()() = 0;
};
