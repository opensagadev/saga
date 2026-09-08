#include "nu2api_nusound_types.h"

#include "nu2api/nusound/nusound_voice.hpp"

NuSoundEffect::~NuSoundEffect() {
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

bool NuSoundEffectAttenuation::AttachBus(NuSoundBus *) {
    return true;
}

void NuSoundEffectAttenuation::ProcessVoice(NuSoundVoice *voice, f32) {
    if (attachments.Length() == 0) {
        output_mix = attenuation;
        return;
    }

    if (voice->GetSurroundMode() != NuSoundSystem::SurroundMode::TWO) {
        for (NuListNodeBase *node = attachments.Head(); node != attachments.Tail(); node = node->GetNext()) {
            void *listener = static_cast<NuListNode<void *> *>(node)->value;
            if (listener != NULL && listener == voice->field60_0x8c) {
                output_mix = attenuation;
                return;
            }
        }
    }
    output_mix = 1.0f;
}

void NuSoundEffectRepeat::ProcessVoice(NuSoundVoice *voice, f32 frametime) {
    NuSoundVoice::PlayState voice_state = voice->GetState();
    if (voice_state == NuSoundVoice::PLAYSTATE_STOPPED) {
        if (armed && repeat_count != 0) {
            armed = false;
            repeat_count--;
            remaining_delay = delay;
            voice->DestroyHardwareVoice();
            voice->CreateHardwareVoice();
            voice->Play();
            voice->Pause();
        }
    } else if (voice_state == NuSoundVoice::PLAYSTATE_PAUSED) {
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
