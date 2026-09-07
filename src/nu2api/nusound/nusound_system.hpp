#pragma once

#include "decomp_assert.h"
#include "nu2api/nucore/NuMemoryManager.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuelist.hpp"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nusound/nulist.hpp"
#include "nu2api/nusound/nusound_memorymanager.hpp"
#include "nu2api/nusound/nusound_source.hpp"
#include "nu2api/nusound/nusound_streamdesc.hpp"

#include "nu2api/nufile/nufile.h"

#include "decomp.h"
#include <string.h>

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

class NuSoundMemory {
  public:
    template <typename T> static void PushNuListNode(NuList<T> &list, const T &value);
};

class NuSoundEffect {
  public:
    enum class EffectType : u32 {};
    enum class EffectProcessStage : u32 {
        ZERO = 0,
        ONE = 1,
    };
    virtual bool Initialise();
    virtual void Shutdown() {}
    virtual void Enable() { enabled = true; }
    virtual void Disable() { enabled = false; }
    virtual ~NuSoundEffect();
    virtual bool AttachVoice(NuSoundVoice *) { return true; }
    virtual void DetachVoice(NuSoundVoice *) {}
    virtual void ProcessVoice(NuSoundVoice *, f32) {}
    virtual bool AttachBus(NuSoundBus *) { return false; }
    virtual void DetachBus(NuSoundBus *) {}
    virtual void ProcessBus(NuSoundBus *, f32) {}
    virtual void Process(f32) {}

    struct ManagedReference;
    struct ReferenceTarget { void *unknown_00; void *unknown_04; ManagedReference *references; };
    struct ManagedReference {
        ReferenceTarget *object;
        ManagedReference *next;
        ManagedReference *previous;
    };
    ManagedReference *references;
    u32 unknown_08[4];
    bool enabled;
    u8 unknown_19[3];
    f32 attenuation;
    f32 pitch_scale;
    bool system_owned;
    u8 unknown_25[3];
    struct ReferenceNode {
        ReferenceNode *prev;
        ReferenceNode *next;
        ManagedReference reference;
    };
    struct ReferenceLinks { ReferenceNode *prev; ReferenceNode *next; } reference_start, reference_end;
    ReferenceNode *reference_head;
    ReferenceNode *reference_tail;
    u32 reference_count;

    NuSoundEffect() : references(NULL) {
        reference_head = reinterpret_cast<ReferenceNode *>(&reference_start);
        reference_tail = reinterpret_cast<ReferenceNode *>(&reference_end);
        reference_start.prev = NULL;
        reference_start.next = reference_tail;
        reference_end.prev = reference_head;
        reference_end.next = NULL;
        reference_count = 0;
    }
};

// --- subsystem classes without a dedicated header (definitions live in their
// --- .cpp files; the declarations used to sit in the legacy types catalog) ---

class NuSoundClock {
  public:
    struct Callback {
        virtual void OnCallback(u64 elapsed, u64 frequency) {}
        Callback *prev;
        Callback *next;
    };

  private:
    struct Links { Callback *prev; Callback *next; } start, end;
    Callback *head;
    Callback *tail;
    u32 callback_count;
    u64 frequency;
    u64 last_ticks;
    static Links *GetLinks(Callback *callback) {
        return reinterpret_cast<Links *>(reinterpret_cast<char *>(callback) + sizeof(void *));
    }

  public:

    NuSoundClock();
    ~NuSoundClock();

    void AddCallback(Callback *callback);
    void RemoveCallback(Callback *callback);
    void HandleCallbacks();
    u64 GetClockFrequency() const;
    u64 GetTicks() const;
};

class NuSoundListener {
  public:
    NuSoundListener *prev;
    NuSoundListener *next;
    NuSoundEffect::ManagedReference *references;
    const VuMtx *head_matrix;
    const VuVec *focus_position;
    const VuVec *screen_position;
    VuVec velocity;
    bool enabled;
    bool focus_enabled;
    f32 sensitivity;
    i32 output_devices;
    NuSoundListener();
    ~NuSoundListener();

    void Enable();
    void Disable();
    bool IsEnabled() const;
    void SetHeadMatrix(const VuMtx *mtx);
    const VuMtx * GetHeadMatrix() const;
    void SetFocusPosition(const VuVec *position);
    const VuVec *GetFocusPosition() const;
    void EnableFocusPosition();
    void DisableFocusPosition();
    bool IsFocusPositionEnabled() const;
    void Set2DScreenPosition(const VuVec *position);
    const VuVec * Get2DScreenPosition() const;
    void SetVelocity(const VuVec &velocity);
    const VuVec * GetVelocity() const;
    void SetSensitivity(f32 sensitivity);
    f32 GetSensitivity() const;
    void SetOutputDevices(i32 devices);
    i32 GetOutputDevices() const;
    const VuVec *GetAttenuationPosition(const VuVec &position) const;
    f32 GetAttenuationDistance(const VuVec &position) const;
    f32 GetHeadDistance(const VuVec &position) const;
};

template <> class NuEList<NuSoundListener, DefaultElist> {
  public:
    struct Links { NuSoundListener *prev; NuSoundListener *next; };
    Links start;
    Links end;
    NuSoundListener *head;
    NuSoundListener *tail;
    u32 length;

    NuEList() {
        head = reinterpret_cast<NuSoundListener *>(&start);
        tail = reinterpret_cast<NuSoundListener *>(&end);
        start.prev = NULL;
        start.next = tail;
        end.prev = head;
        end.next = NULL;
        length = 0;
    }
};

class NuSoundEffectDoppler : public NuSoundEffect {
  public:
    f32 speed_of_sound;
    f32 velocity_scale;
    const NuEList<NuSoundListener, DefaultElist> *listeners;
    NuSoundEffectDoppler();
    virtual ~NuSoundEffectDoppler();

    void ProcessVoice(NuSoundVoice *voice, f32 frametime);
    void SetParameters(f32 a, f32 b, const NuEList<NuSoundListener, DefaultElist> *listeners);
};

class NuSoundEffectFader : public NuSoundEffect {
  public:
    struct Curve { u32 mode; void *data; };
    enum class FinishState : u32 {};
    struct FinishCallback { virtual void OnFinish() = 0; };

    Curve curve;
    f32 unknown_4c;
    f32 unknown_50;
    f32 unknown_54;
    f32 unknown_58;
    u32 unknown_5c;
    u32 unknown_60;
    FinishCallback *unknown_64;
    bool unknown_68;

    NuSoundEffectFader();
    virtual ~NuSoundEffectFader();

    bool AttachBus(NuSoundBus *bus);
    bool AttachVoice(NuSoundVoice *voice);
    void Enable();
    void Disable();
    void Process(f32 frametime);
    void ProcessBus(NuSoundBus *bus, f32 frametime);
    void ProcessVoice(NuSoundVoice *voice, f32 frametime);
    void SetCurveParams(const Curve &curve);
    void SetParameters(f32 a, f32 b, FinishState state);
};

class NuSoundEffectPitchRamp : public NuSoundEffect {
  public:
    enum class FinishState : u32 {};
    f32 field_44;
    f32 field_48;
    bool field_4c;
    u32 field_50;

    NuSoundEffectPitchRamp();
    virtual ~NuSoundEffectPitchRamp();

    bool AttachVoice(NuSoundVoice *voice) override;
    void Process(f32 frametime);
    void ProcessVoice(NuSoundVoice *voice, f32 frametime);
    void SetParameters(f32 a, f32 b, FinishState state);
};

class NuSoundEffectAttenuation : public NuSoundEffect {
  public:
    f32 value;
    NuSoundEffectAttenuation() {
        unknown_08[0] = 0; unknown_08[1] = 0; unknown_08[2] = 1; unknown_08[3] = 0;
        attenuation = 1.0f; pitch_scale = 1.0f; system_owned = false; enabled = true;
        value = 1.0f;
    }
    ~NuSoundEffectAttenuation() {}
    bool AttachBus(NuSoundBus *) { return true; }
    void ProcessVoice(NuSoundVoice *, f32);
};

class NuSoundEffectPitch : public NuSoundEffect {
  public:
    NuSoundEffectPitch() {
        unknown_08[0] = 2; unknown_08[1] = 0; unknown_08[2] = 1; unknown_08[3] = 1;
        attenuation = 1.0f; pitch_scale = 1.0f; system_owned = false; enabled = true;
    }
    ~NuSoundEffectPitch() {}
};

class NuSoundEffectRandomVolume : public NuSoundEffect {
  public:
    NuSoundEffectRandomVolume() {
        unknown_08[0] = 2; unknown_08[1] = 0; unknown_08[2] = 1; unknown_08[3] = 4;
        attenuation = 1.0f; pitch_scale = 1.0f; system_owned = false; enabled = true;
    }
    ~NuSoundEffectRandomVolume() {}
};

class NuSoundEffectRandomPitch : public NuSoundEffect {
  public:
    NuSoundEffectRandomPitch() {
        unknown_08[0] = 2; unknown_08[1] = 0; unknown_08[2] = 1; unknown_08[3] = 5;
        attenuation = 1.0f; pitch_scale = 1.0f; system_owned = false; enabled = true;
    }
    ~NuSoundEffectRandomPitch() {}
};

class NuSoundEffectRepeat : public NuSoundEffect {
  public:
    f32 delay;
    u32 repeats;
    bool armed;
    f32 remaining;
    NuSoundEffectRepeat() {
        unknown_08[0] = 1; unknown_08[1] = 0; unknown_08[2] = 1; unknown_08[3] = 6;
        attenuation = 1.0f; pitch_scale = 1.0f; system_owned = false; enabled = true;
        repeats = 1; delay = 0.0f; armed = true;
    }
    ~NuSoundEffectRepeat() {}
    void ProcessVoice(NuSoundVoice *, f32);
};

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
    u32 capacity;
    u32 count;

  public:
    NuSoundVoiceFactoryList();
    ~NuSoundVoiceFactoryList() {
        count = 0;
        if (factories != NULL) {
            NuMemoryGet()->GetThreadMem()->BlockFree(factories, 0);
            capacity = 0;
            factories = NULL;
        }
    }
    void RegisterFactory(NuSoundVoiceFactory *factory, NuSoundStreamDesc::DataFormat format);
    NuSoundVoiceFactory *GetFactory(NuSoundStreamDesc::DataFormat format);
};

class NuSoundSystemCallbacks : public NuMemoryManager::IEventHandler {
  public:
    void *scratch;
    u32 scratch_size;

    bool AllocatePage(NuMemoryManager *manager, u32 size, u32 unknown) override {
        if (scratch == NULL) return false;
        manager->AddPage(scratch, scratch_size, false);
        scratch = NULL;
        return true;
    }
    bool ReleasePage(NuMemoryManager *manager, void *ptr, u32 unknown) override {
        return false;
    }
    virtual bool OpenDump(NuMemoryManager *manager, const char *filename, u32 &id) {
        id = NuFileOpen(const_cast<char *>(filename), NUFILE_WRITE);
        return id != 0;
    }
    virtual void CloseDump(NuMemoryManager *manager, u32 id) {
        NuFileClose(id);
    }
    virtual void Dump(NuMemoryManager *manager, u32 id, const char *message) {
        NuFileWrite(id, const_cast<char *>(message), strlen(message));
    }
};

class NuSoundSystem {
  public:
    enum class MemoryDiscipline : u32 {
        SCRATCH = 0,
        SAMPLE = 1,
        DECODER = 2,
    };

    enum ChannelConfig : u32 {};

    enum class AudioChannel : u32 {};
    struct CurveData {};
    enum class DownmixType : u32 {};
    enum class FalloffType { LINEAR = 0 };
    enum class SurroundMode { ZERO = 0 };

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
    pthread_mutex_t mutex;
    NuSoundClock clock;
    struct SampleLinks { NuSoundSample *previous; NuSoundSample *next; } sample_start, sample_end;
    NuSoundSample *sample_head;
    NuSoundSample *sample_tail;
    u32 sample_list_count;

  private:
    NuSoundSample **samples;
    u32 sample_count;

  public:
    NuSoundVoiceFactoryList factory_list;
    // Voice bookkeeping: an intrusive doubly-linked list of all live voices
    // (links live in the voices at +0x24/+0x28), the per-format voice factory
    // and the audio clock.
    struct VoiceLinks { NuSoundVoice *prev; NuSoundVoice *next; } voice_start, voice_end;
    NuSoundVoice *voice_head;
    NuSoundVoice *voice_tail;
    i32 voice_count;
    struct EffectLinks { NuEListNode<NuSoundEffect> *prev; NuEListNode<NuSoundEffect> *next; } effect_start, effect_end;
    NuEListNode<NuSoundEffect> *effect_head;
    NuEListNode<NuSoundEffect> *effect_tail;
    u32 effect_count;
    struct BusLinks { NuSoundBus *previous; NuSoundBus *next; } bus_start, bus_end;
    NuSoundBus *bus_head;
    NuSoundBus *bus_tail;
    u32 bus_count;
    struct RoutingLinks { NuSoundRoutingTable *prev; NuSoundRoutingTable *next; };
    RoutingLinks routing_start;
    RoutingLinks routing_end;
    NuSoundRoutingTable *routing_head;
    NuSoundRoutingTable *routing_tail;
    u32 routing_count;
    NuEList<NuSoundListener, DefaultElist> listeners;
    u32 unknown_f0[2];
    u32 sample_load_count;
    u8 unknown_fc[0xc];
    bool initialised;

    // The original class contains additional intrusive lists and bookkeeping
    // between the update gate at +0x108 and the fields reconstructed above.
    // Keep their storage in the target object even before their types are
    // known, so the OpenSL handles retain their observed ABI offsets.
    u8 unknown_before_device_handles[3];

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

    static NuSoundSystemCallbacks g_handler;

    static NuMemoryManager *sScratchMemMgr;

    NuSoundSystem();

    virtual ~NuSoundSystem();
    virtual NuSoundEffect *CreateEffect(NuSoundEffect::EffectType);
    virtual void ReleaseEffect(NuSoundEffect *);
    virtual NuSoundBus *CreateBus(const char *name, bool is_master);
    virtual NuSoundBus *GetBus(const char *name);
    virtual void ReleaseBus(NuSoundBus *);
    virtual bool IsUserPlayingMusic() { return false; }
    virtual void PauseUserMusic() {}
    virtual void ResumeUserMusic() {}
    virtual bool TitleHasUserMusicControl() { return true; }
    virtual void OnEnterSystemMenu() {}
    virtual void OnExitSystemMenu() {}
    virtual bool InitAudioDevice() = 0;
    virtual void ShutdownAudioDevice() = 0;
    virtual void UpdateAudioDevice() = 0;

    NuSoundVoice *CreateVoice(NuSoundSource *source, bool loop);
    void ReleaseVoice(NuSoundVoice *voice);
    static bool SourceRequiresDecoder(NuSoundSource *source);
    i32 GetNumAvailableOutputDevices();
    static NuSoundRoutingTable *GetDefaultRoutingTable();
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

    static i32 GenerateHash(const char *str);

    // vtable:
    // create_effect
    // release_effect
    // release_bus
    // is_user_playing_music
    // pause_user_music
    // resume_user_music
    // title_has_user_music_control
    // on_enter_system_menu
    // on_exit_system_menu
    // init_audio_device
    // shutdown_audio_device
    // update_audio_device


    bool AddListener(NuSoundListener *);
    void AddRoutingTable(NuSoundRoutingTable *);
    static f32 AmplitudeTodB(float);
    f32 CalculateCrossfadeHeight(NuSoundSystem::CurveData const &, float) const;
    void CreateCrossfadeCurve(unsigned int);
    static NuSoundDecoder *CreateDecoder(NuSoundSource *source);
    void DefragmentSampleMemory();
    static FileType DetermineFileType(NUFILETYPE);
    void Disable();
    static bool FileTypeSupported(NuSoundSystem::FileType);
    static NuSoundSystem *Get();
    static u32 GetAllocdMemory(NuSoundSystem::MemoryDiscipline);
    static u32 GetBufferAlignment();
    i32 GetClosestSupportedConfig(i32 config);
    void GetCrossfadeCurve(unsigned int) const;
    static FileType GetDefaultFileType(NuSoundSource::FeedType);
    static u32 GetGfxMemorySize();
    static const char *GetLanguageString(bool);
    static u32 GetLargestMemoryFragment(NuSoundSystem::MemoryDiscipline);
    NuEList<NuSoundListener, DefaultElist> const *GetListeners();
    static NuSoundListener *GetNearestRealListener(NuEList<NuSoundListener, DefaultElist> const &, VuVec const &);
    static NuSoundListener *GetNearestFocusListener(NuEList<NuSoundListener, DefaultElist> const &, VuVec const &, float &);
    NuSoundVoice *GetOldestVoice(NuSoundSample *, float &);
    static i32 GetOutputChannelConfig();
    void GetPeakAllocdMemory(NuSoundSystem::MemoryDiscipline);
    static const char *GetPlatformString();
    NuSoundVoice *GetQuietestVoice(NuSoundSample *, float &);
    NuSoundRoutingTable *GetRoutingTable(char const *);
    static u32 GetTotalMemory(NuSoundSystem::MemoryDiscipline);
    bool LoadSample(NuSoundSample *, void *, int, NuSoundOutOfMemCallback *);
    void PauseAllVoices();
    void PauseVoices(int);
    void ReAllocMemory(NuSoundSystem::MemoryDiscipline, unsigned int, unsigned int);
    void ReleaseCrossfadeCurve(unsigned int);
    static void ReleaseDecoder(NuSoundDecoder *decoder);
    void ReleaseSample(NuSoundSample *);
    void RemoveListener(NuSoundListener *);
    void ResumeAllVoices();
    void ResumeVoices(int);
    static void SetDefaultRoutingTable(NuSoundRoutingTable *);
    static void SetGfxMemorySize(unsigned int);
    void SetMainThreadID(NuThread *);
    void Shutdown();
    void StopAllVoices();
    void StopVoices(NuSoundSource const &);
    void StopVoices(int);
    void UnloadAllSamples();
    bool UnloadSample(NuSoundSample *);
    void dBToAmplitude(float);
};
// One row of a routing table: an N-in by M-out gain matrix pointing at one of
// the static NuSoundRoutingTable* matrices.
struct NuSoundMixMatrix {
    u32 in;
    u32 out;
    f32 *matrix;
    u8 flag;
};

class NuSoundMixer {
  public:
    enum OutputLayout : u32 {};

    NuSoundMixer(NuSoundSystem::ChannelConfig config, NuSoundSystem::ChannelConfig output,
                 NuSoundMixer::OutputLayout layout, NuSoundSystem::DownmixType downmix, NuSoundRoutingTable *table);
    ~NuSoundMixer();

    void Mix(f32 *in, f32 *out);
    i32 GetOutputIndex(i32 output, i32 index);

    NuSoundSystem::ChannelConfig input_config;
    NuSoundSystem::ChannelConfig output_config;
    OutputLayout output_layout;
    NuSoundSystem::DownmixType downmix_type;
    NuSoundRoutingTable *routing_table;
    static u8 sDownmixerChannelMaps[4][8];
};
class NuSoundRoutingTable {
  public:
    NuSoundRoutingTable(const char *name);
    NuSoundRoutingTable(const char *name, const NuSoundRoutingTable *parent);
    ~NuSoundRoutingTable();

    void SetMatrix(NuSoundSystem::ChannelConfig from, NuSoundSystem::ChannelConfig to, NuSoundMixMatrix *matrix);
    NuSoundMixMatrix *GetMatrix(NuSoundSystem::ChannelConfig from, NuSoundSystem::ChannelConfig to) const;
    static NuSoundSystem::ChannelConfig GetConfig(i32 config);
    static i32 GetIndex(NuSoundSystem::ChannelConfig config);
    const char *GetName() const;
    NuSoundRoutingTable *unknown_00;
    NuSoundRoutingTable *unknown_04;
    u16 name_capacity;
    u16 name_length;
    char *name_data;
    char name_storage[32];
    NuSoundMixMatrix *matrices[6][6];
};
class NuSoundHandle {
  public:
    // Callback slots recovered from handle destruction, voice invalidation,
    // and ResetFrameCount; this list does not contain sound effects.
    class Callback {
      public:
        virtual void OnDestroy(NuSoundHandle *) = 0;
        virtual void OnInvalidateVoice(NuSoundHandle *) = 0;
        virtual void OnResetFrameCount(NuSoundHandle *) = 0;
    };

  private:
    NuSoundHandle *previous;
    NuSoundHandle *next;
    NuSoundVoice *voice;
    NuList<Callback *> callbacks;
    friend class NuSoundVoice;

  public:
    NuSoundHandle();
    NuSoundHandle(NuSoundHandle &other);
    ~NuSoundHandle();

    void SetVoice(NuSoundVoice *voice);
    void GetVoice() const;
    void InvalidateVoice();
    void Play();
    void Pause();
    void Resume();
    void Stop();
    void SetVolume(f32 volume);
    void GetVolume() const;
    void SetPitch(f32 pitch);
    void GetPitch() const;
    void SetPosition(VuVec *position);
    void GetPosition() const;
    void SetVelocity(const VuVec &velocity);
    void GetVelocity() const;
    void SetFalloff(f32 near, f32 far, NuSoundSystem::FalloffType type);
    void GetFalloffType() const;
    void GetNear() const;
    void GetFar() const;
    void GetState() const;
    void IsLooping() const;
    void GetPlaybackPositionSamples();
    void GetPlaybackPositionSeconds() const;
    void GetTotalLengthSamples() const;
    void GetTotalLengthSeconds() const;
    void GetLastAttenuation() const;
    void GetLastAttenuationListener() const;
    void GetLastDistanceAttenuation() const;
    void GetLastListenerDistance() const;
    void GetLastPositionalListener() const;
    void GetSurroundMode() const;
    void AddEffect(NuSoundEffect *effect);
    void RemoveEffect(NuSoundEffect *effect);
    void GetEffect(NuSoundEffect::EffectType type);
    void ResetFrameCount();
    void operator=(NuSoundHandle &other);
    void operator==(NuSoundHandle const &other);
}; // namespace NuSoundSystem

// Binary layouts below describe Android. Host/WASM builds use their own ABI,
// including native mutex sizes and 64-bit member alignment.
#if defined(__arm__)
// Original ARM constructors: clock 0x2f2af8, system 0x2ee5f4;
// InitAudioDevice 0x2fce38 passes this+0x110 to slCreateEngine.
DECOMP_ASSERT(sizeof(NuSoundClock) == 0x30, "ARM sound clock layout");
DECOMP_ASSERT(__builtin_offsetof(NuSoundSystem, factory_list) == 0x5c, "ARM voice factory offset");
DECOMP_ASSERT(__builtin_offsetof(NuSoundSystem, initialised) == 0x10c, "ARM audio update gate offset");
DECOMP_ASSERT(__builtin_offsetof(NuSoundSystem, engine_object) == 0x110, "ARM OpenSL engine handle offset");
#else
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(NuSoundClock) == 0x2c, "Android sound clock layout");
DECOMP_ASSERT(sizeof(void *) != 4 || __builtin_offsetof(NuSoundSystem, factory_list) == 0x58, "Android voice factory offset");
DECOMP_ASSERT(sizeof(void *) != 4 || __builtin_offsetof(NuSoundSystem, initialised) == 0x108, "Android audio update gate offset");
DECOMP_ASSERT(sizeof(void *) != 4 || __builtin_offsetof(NuSoundSystem, engine_object) == 0x10c,
              "Android OpenSL engine handle offset");
#endif
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(NuSoundVoiceFactoryList) == 0xc, "Android voice factory list layout");
DECOMP_ASSERT(sizeof(void *) != 4 || __builtin_offsetof(NuSoundSystem, clock) == 8, "Android sound clock offset");

class NuSoundOutOfMemCallback {
  public:
    // Slot names are descriptive; the reference establishes their order,
    // 32-bit size argument, and boolean result through indirect calls.
    virtual bool OnAllocationFailure(u32 size) = 0;
    virtual bool OnAllocationFailureMinusTwo(u32 size) = 0;
};
