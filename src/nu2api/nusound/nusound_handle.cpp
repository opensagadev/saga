#include "nu2api_nusound_types.h"

pthread_mutex_t NuSoundHandle::sCriticalSection;

NuSoundHandle::NuSoundHandle() : intrusive_prev(NULL), intrusive_next(NULL), voice(NULL) {
}

NuSoundHandle::~NuSoundHandle() {
    pthread_mutex_lock(&sCriticalSection);
    if (voice != NULL) {
        if ((voice->flags2 & 8) != 0) {
            voice->Stop(true);
        }
        voice->UnregisterHandle(this);
    }
    NuListNodeBase *node = effects.Head();
    NuListNodeBase *end = effects.Tail();
    for (; node != end; node = node->next) {
        static_cast<NuListNode<NuSoundEffect *> *>(node)->value->Shutdown();
    }
    pthread_mutex_unlock(&sCriticalSection);
}

bool NuSoundHandle::operator==(NuSoundHandle const &other) {
    return this == &other;
}

NuSoundVoice *NuSoundHandle::GetVoice() const {
    return voice;
}

void NuSoundHandle::Play() {
    if (voice != NULL) {
        voice->Play();
    }
}

void NuSoundHandle::Pause() {
    if (voice != NULL) {
        voice->Pause();
    }
}

void NuSoundHandle::Resume() {
    if (voice != NULL) {
        voice->Resume();
    }
}

void NuSoundHandle::SetVolume(f32 volume) {
    if (voice != NULL) {
        voice->SetVolume(volume);
    }
}

void NuSoundHandle::SetPitch(f32 pitch) {
    if (voice != NULL) {
        voice->SetPitch(pitch);
    }
}

void NuSoundHandle::SetPosition(VuVec *position) {
    if (voice != NULL) {
        voice->SetPosition(position);
    }
}

void NuSoundHandle::SetVelocity(const VuVec &velocity) {
    if (voice != NULL) {
        voice->SetVelocity(velocity);
    }
}

void NuSoundHandle::SetFalloff(f32 near_distance, f32 far_distance, NuSoundSystem::FalloffType type) {
    if (voice != NULL) {
        voice->SetFalloff(near_distance, far_distance, type);
    }
}

f32 NuSoundHandle::GetVolume() const {
    return voice != NULL ? voice->GetVolume() : 0.0f;
}

f32 NuSoundHandle::GetPitch() const {
    return voice != NULL ? voice->GetPitch() : 0.0f;
}

const VuVec *NuSoundHandle::GetPosition() const {
    return voice != NULL ? voice->GetPosition() : NULL;
}

const VuVec *NuSoundHandle::GetVelocity() const {
    return voice->GetVelocity();
}

f32 NuSoundHandle::GetNear() const {
    return voice != NULL ? voice->GetNear() : 0.0f;
}

f32 NuSoundHandle::GetFar() const {
    return voice != NULL ? voice->GetFar() : 0.0f;
}

NuSoundSystem::FalloffType NuSoundHandle::GetFalloffType() const {
    return voice != NULL ? voice->GetFalloffType() : NuSoundSystem::FalloffType::LINEAR;
}

f32 NuSoundHandle::GetLastAttenuation() const {
    return voice != NULL ? voice->field67_0xa8 : 0.0f;
}

f32 NuSoundHandle::GetLastDistanceAttenuation() const {
    return voice != NULL ? voice->field65_0xa0 : 0.0f;
}

f32 NuSoundHandle::GetLastListenerDistance() const {
    return voice != NULL ? voice->field64_0x9c : 0.0f;
}

NuSoundListener *NuSoundHandle::GetLastPositionalListener() const {
    return voice != NULL ? static_cast<NuSoundListener *>(voice->field57_0x80) : NULL;
}

NuSoundListener *NuSoundHandle::GetLastAttenuationListener() const {
    return voice != NULL ? static_cast<NuSoundListener *>(voice->field60_0x8c) : NULL;
}

NuSoundSystem::SurroundMode NuSoundHandle::GetSurroundMode() const {
    return voice != NULL ? voice->GetSurroundMode() : NuSoundSystem::SurroundMode::TWO;
}

f32 NuSoundHandle::GetTotalLengthSeconds() const {
    return voice != NULL ? voice->sound_source->GetStreamDesc()->GetLengthSeconds() : 0.0f;
}

f32 NuSoundHandle::GetPlaybackPositionSeconds() const {
    return voice != NULL ? voice->GetPlaybackPositionSeconds() : 0.0f;
}

u64 NuSoundHandle::GetTotalLengthSamples() const {
    return voice != NULL ? voice->sound_source->GetStreamDesc()->GetLengthSamples() : 0;
}

u64 NuSoundHandle::GetPlaybackPositionSamples() {
    return voice != NULL ? voice->GetPlaybackPositionSamples() : 0;
}

bool NuSoundHandle::IsLooping() const {
    return voice != NULL && (voice->flags2 & 8) != 0;
}

i32 NuSoundHandle::GetState() const {
    return voice != NULL ? voice->GetState() : NuSoundVoice::PLAYSTATE_STOPPED;
}

bool NuSoundHandle::AddEffect(NuSoundEffect *effect) {
    if (voice != NULL) {
        return voice->AddEffect(effect);
    }
    return false;
}

void NuSoundHandle::RemoveEffect(NuSoundEffect *effect) {
    if (voice != NULL) {
        voice->RemoveEffect(effect);
    }
}

NuSoundEffect *NuSoundHandle::GetEffect(NuSoundEffect::EffectType type) {
    return voice != NULL ? voice->GetEffect(type) : NULL;
}

void NuSoundHandle::ResetFrameCount() {
    NuListNodeBase *node = effects.Head();
    NuListNodeBase *end = effects.Tail();
    for (; node != end; node = node->next) {
        static_cast<NuListNode<NuSoundEffect *> *>(node)->value->Enable();
    }
}

void NuSoundHandle::InvalidateVoice() {
    if (voice != NULL) {
        voice->UnregisterHandle(this);
    }
    voice = NULL;
    ResetFrameCount();
}

void NuSoundHandle::Stop() {
    if (voice != NULL) {
        voice->Stop(true);
        InvalidateVoice();
    }
}

NuSoundHandle &NuSoundHandle::operator=(NuSoundHandle &other) {
    if (voice != NULL) {
        voice->UnregisterHandle(this);
    }
    voice = other.GetVoice();
    other.InvalidateVoice();
    if (voice != NULL) {
        voice->RegisterHandle(this);
    }

    NuListNodeBase *node = other.effects.Head();
    NuListNodeBase *end = other.effects.Tail();
    for (; node != end; node = node->next) {
        NuSoundEffect *effect = static_cast<NuListNode<NuSoundEffect *> *>(node)->value;
        effect->Disable();
        NuSoundMemory::PushNuListNode(effects, effect);
    }
    while (other.effects.Head() != other.effects.Tail()) {
        other.effects.Remove(other.effects.Head());
    }
    return *this;
}

NuSoundHandle::NuSoundHandle(NuSoundHandle &other) : intrusive_prev(NULL), intrusive_next(NULL), effects() {
    *this = other;
}

void NuSoundHandle::SetVoice(NuSoundVoice *new_voice) {
    voice = new_voice;
    ResetFrameCount();
}
