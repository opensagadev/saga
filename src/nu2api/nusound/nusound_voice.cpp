// NuSoundVoice — decompiled from libTTapp.so (nu2api.2013/nusound/nusound.cpp).
// Engine-side voice: play state machine, volume/pitch, the eight output
// gains fed from the positional mix, and the per-frame Update that pushes the
// mix down into the platform voice (NuVoiceAndroid).

#include "nu2api_nusound_types.h"

#include "decomp.h"

#include "nu2api/nucore/nuthread.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nusound/nusound_bus.hpp"
#include "nu2api/nusound/nusound_source.hpp"

#include <string.h>
#include <math.h>

extern f32 NuATan2f(f32, f32);

pthread_mutex_t NuSoundVoice::sStateCriticalSection = PTHREAD_MUTEX_INITIALIZER;
static_assert(sizeof(void *) != 4 || offsetof(NuSoundVoice, volume) == 0xfc, "voice volume offset");
static_assert(sizeof(void *) != 4 || offsetof(NuSoundVoice, falloff_a) == 0x100, "voice falloff offset");
static_assert(sizeof(void *) != 4 || offsetof(NuSoundVoice, mix_flags) == 0x119, "voice mix flags offset");
static_assert(sizeof(void *) != 4 || offsetof(NuSoundVoice, state) == 0x140, "voice playback state offset");
static_assert(sizeof(void *) != 4 || offsetof(NuVoiceAndroid, player_object) == 0x14c, "Android player handle offset");

// ---------------------------------------------------------------------------
// construction / destruction
// ---------------------------------------------------------------------------

NuSoundVoice::NuSoundVoice(NuSoundSource *sound_source, bool loop) {
    this->field_0x24 = NULL;
    this->field_0x28 = NULL;
    for (u32 i = 0; i < 2; ++i) {
        positional_references[i].object = NULL;
        positional_references[i].next = NULL;
        positional_references[i].previous = NULL;
    }
    handles_head = reinterpret_cast<NuSoundHandle *>(&handles_start);
    handles_tail = reinterpret_cast<NuSoundHandle *>(&handles_end);
    handles_start.previous = NULL;
    handles_start.next = handles_tail;
    handles_end.previous = handles_head;
    handles_end.next = NULL;
    handle_count = 0;
    this->queued_buffers = 0;

    // The source is locked for the lifetime of the voice and keeps the stream
    // open until the voice releases it.
    sound_source->IsStreamOpen();
    sound_source->Lock();
    sound_source->VoiceReference();
    this->sound_source = sound_source;

    this->field63_0x98 = 0.0f;
    this->field64_0x9c = 0.0f;
    this->field65_0xa0 = 1.0f; // falloff attenuation
    this->field66_0xa4 = 0.0f;
    this->field67_0xa8 = 1.0f; // final mix scalar
    this->field68_0xac = 1.0f; // pitch scale
    this->volume = 1.0f;
    this->pitch = 1.0f;
    this->falloff_a = 1.0f;
    this->falloff_b = 6.0f;
    this->field69_0xb0 = 20.0f;
    this->field70_0xb4 = 180.0f;
    this->field71_0xb8 = 70.0f;
    this->field72_0xbc = 0.0f;
    this->field73_0xc0 = 0.0f;
    this->field74_0xc4 = 1.0f;
    this->field113_0x10c = 1.0f; // LFE gain
    this->field114_0x110 = 0;
    this->field115_0x114 = 1;
    this->control_118 = 0;
    this->output_bus = NuSoundSystem::sMasterBus;
    this->field15_0x38 = static_cast<NuSoundSystem::DownmixType>(0);
    this->field16_0x3c = NuSoundSystem::GetDefaultRoutingTable();
    this->listeners = NULL;
    this->field56_0x7c = NULL;
    this->surround_mode = 2; // 2D omni
    this->falloff_type = 0;
    this->field130_0x144 = 1.0f;
    this->field131_0x148 = -1;

    this->flags = (u8)(this->flags & 0xf6 | loop << 3);
    this->flags &= 0xf9;
    this->mix_flags = (u8)(this->mix_flags & 0xf0 | 0x10);

    this->SetState(PLAYSTATE_STOPPED); // libTTapp.so ctor tail (0x3275b9)
    memset(this->mix_gains, 0, sizeof(this->mix_gains));
}

NuSoundVoice::~NuSoundVoice() {
    NuSoundHandle *end = handles_tail;
    for (NuSoundHandle *handle = handles_head->next; handle != end; handle = handle->next) {
        handle->SetVoice(NULL);
    }
    while (handle_count != 0) {
        NuSoundHandle *handle = handles_head->next;
        if (handle->previous != NULL) handle->previous->next = handle->next;
        if (handle->next != NULL) handle->next->previous = handle->previous;
        handle->next = NULL;
        handle->previous = NULL;
        --handle_count;
    }
    this->sound_source->VoiceRelease();
    this->sound_source->Unlock();
    while (handle_count != 0) {
        NuSoundHandle *handle = handles_head->next;
        if (handle->previous != NULL) handle->previous->next = handle->next;
        if (handle->next != NULL) handle->next->previous = handle->previous;
        handle->next = NULL;
        handle->previous = NULL;
        --handle_count;
        delete handle;
    }
    for (i32 i = 1; i >= 0; --i) {
        NuSoundEffect::ManagedReference &ref = positional_references[i];
        if (ref.object != NULL) {
            if (ref.next == &ref) {
                ref.object->references = NULL;
            } else {
                ref.next->previous = ref.previous;
                ref.previous->next = ref.next;
                if (ref.object->references == &ref) ref.object->references = ref.next;
            }
            ref.object = NULL;
            ref.next = NULL;
            ref.previous = NULL;
        }
    }
}

// ---------------------------------------------------------------------------
// play state
// ---------------------------------------------------------------------------

NuSoundVoice::PlayState NuSoundVoice::GetState() const {
    NuSoundVoice::PlayState state;

    pthread_mutex_lock(&sStateCriticalSection);
    state = this->state;
    pthread_mutex_unlock(&sStateCriticalSection);
    return state;
}

void NuSoundVoice::SetState(PlayState state) {
    pthread_mutex_lock(&sStateCriticalSection);
    this->state = state;
    pthread_mutex_unlock(&sStateCriticalSection);
}

bool NuSoundVoice::GetAutoDelete() const {
    return (this->flags & 1) != 0;
}

void NuSoundVoice::SetAutoDelete(bool auto_delete) {
    this->flags = (u8)(this->flags & 0xfe | auto_delete);
}

void NuSoundVoice::SetMixUpdate(bool mix_update) {
    this->mix_flags = (u8)(this->mix_flags & 0xef | mix_update << 4);
}

void NuSoundVoice::SetVolume(f32 volume) {
    // The original rejects out-of-range values instead of clamping.
    if (0.0f <= volume && volume <= 1.0f) {
        this->volume = volume;
    }
}

void NuSoundVoice::SetPitch(f32 pitch) {
    if (0.0f <= pitch) {
        this->pitch = pitch;
    }
}

// ---------------------------------------------------------------------------
// play control
// ---------------------------------------------------------------------------

void NuSoundVoice::Play() {
    NuSoundVoice::PlayState state = this->GetState();
    if (state == PLAYSTATE_PLAYING) {
        return;
    }
    state = this->GetState();
    if (state == PLAYSTATE_PAUSED) {
        this->Resume();
        return;
    }

    if (this->queued_buffers == 0) {
        // Ask the source for the initial buffers; the streamer (or the sample
        // itself) hands them back through SubmitBuffer.
        for (i32 i = 0; i < (i32)this->sound_source->GetNumInitialBuffers(); i++) {
            if ((this->flags & 8) == 0 && (this->flags & 2) != 0) {
                break;
            }
            this->sound_source->RequestBuffer(
                (this->flags >> 3) & 1, NuSoundWeakPtr<NuSoundBufferCallback>(this));
        }
    }

    this->Update(0.0f);         // prime the mix and the hardware state
    this->StartHardwareVoice(); // flags the device start (applied in UpdateHardwareVoice)
    this->SetState(PLAYSTATE_PLAYING);
}

void NuSoundVoice::Pause() {
    if (this->GetState() == PLAYSTATE_PLAYING) {
        this->PauseHardwareVoice();
        this->SetState(PLAYSTATE_PAUSED);
    }
    this->mix_flags = (u8)(this->mix_flags & 0xf0 | (this->mix_flags + 1) & 0xf);
}

void NuSoundVoice::Resume() {
    u8 flags = this->mix_flags;
    if ((flags & 0xf) != 0) {
        flags = (u8)(flags & 0xf0 | (flags & 0xf) + 0xf & 0xf);
        this->mix_flags = flags;
    }
    if ((flags & 0xf) != 0) {
        return;
    }

    if (this->GetState() == PLAYSTATE_PAUSED) {
        this->Update(0.0f);
        this->ResumeHardwareVoice();
        this->SetState(PLAYSTATE_PLAYING);
    }

    this->mix_flags &= 0xf0;
}

void NuSoundVoice::Stop(bool with_effects) {
    if (this->GetState() == PLAYSTATE_STOPPED) {
        return;
    }

    if (with_effects) {
        if (this->BeginStopEffects()) {
            // Stop effects were started; the voice stops once they finished.
            if (!this->CheckStopEffects()) {
                return;
            }
        }
    }

    this->StopHardwareVoice();
    this->SetState(PLAYSTATE_STOPPED);
}

// ---------------------------------------------------------------------------
// per-frame processing
// ---------------------------------------------------------------------------

void NuSoundVoice::Update(f32 frametime) {
    if ((this->flags & 4) != 0 && this->CheckStopEffects()) {
        this->Stop(true);
    }

    this->UpdateEffects(frametime, NuSoundEffect::EffectProcessStage::ZERO);

    if ((this->mix_flags & 0x10) != 0 || this->GetState() == PLAYSTATE_STOPPED) {
        this->UpdateMix(frametime);
    }

    this->UpdateEffects(frametime, NuSoundEffect::EffectProcessStage::ONE);

    this->ApplyHardwareVoiceMix();
    this->UpdateHardwareVoice(frametime);
}

void NuSoundVoice::UpdateMix(f32 frametime) {
    this->CalculatePositionalMix();
    f32 bus_gains[8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};

    if (this->output_bus != NULL) {
        this->output_bus->ApplyFinalMix(bus_gains);
    }

    f32 attenuation = this->CalculateEffectAttenuation();
    this->field67_0xa8 = bus_gains[2] * attenuation * this->field67_0xa8;
    this->field68_0xac = this->CalculateEffectPitchScale();
    f32 volume = this->volume;

    for (u32 i = 0; i < 8; i++) {
        this->mix_gains[i] *= bus_gains[i] * attenuation * volume;
    }

}

void NuSoundVoice::CalculatePositionalMix() {
    memset(mix_gains, 0, sizeof(mix_gains));
    field67_0xa8 = 0.0f;
    field65_0xa0 = 1.0f;
    field63_0x98 = 0.0f;
    field64_0x9c = 0.0f;
    field66_0xa4 = field69_0xb0;
    for (u32 i = 0; i < 2; ++i) {
        NuSoundEffect::ManagedReference &ref = positional_references[i];
        if (ref.object != NULL) {
            if (ref.next == &ref) {
                ref.object->references = NULL;
            } else {
                ref.next->previous = ref.previous;
                ref.previous->next = ref.next;
                if (ref.object->references == &ref) ref.object->references = ref.next;
            }
            ref.object = NULL;
            ref.next = NULL;
            ref.previous = NULL;
        }
    }
    if (this->surround_mode == 2) {
        // 2D omni: every channel at full gain except the LFE channel, which
        // keeps the voice's low-frequency mix.
        this->mix_gains[0] = 1.0f;
        this->mix_gains[1] = 1.0f;
        this->mix_gains[2] = 1.0f;
        this->mix_gains[3] = (f32)this->field113_0x10c;
        this->mix_gains[4] = 1.0f;
        this->mix_gains[5] = 1.0f;
        this->mix_gains[6] = 1.0f;
        this->mix_gains[7] = 1.0f;
        this->field67_0xa8 = 1.0f;
        return;
    }

    if (surround_mode == 4) {
        memmove(mix_gains, field56_0x7c, sizeof(mix_gains));
        return;
    }
    if (surround_mode == 1) {
        VuVec focus_position = position;
        f32 distance = 0.0f;
        NuSoundListener *focus = NuSoundSystem::GetNearestFocusListener(*listeners, focus_position, distance);
        if (focus == NULL || !(falloff_b > distance)) return;
        VuVec real_position = position;
        NuSoundListener *real = NuSoundSystem::GetNearestRealListener(*listeners, real_position);
        if (real == NULL) return;
        const f32 *head = reinterpret_cast<const f32 *>(real->GetHeadMatrix());
        VuVec relative(head[12] - direction.x, head[13] - direction.y, head[14] - direction.z, 0.0f);
        f32 coefficients[8] = {};
        f32 inner = field69_0xb0;
        f32 outer = inner + field71_0xb8;
        outer = outer < 360.0f ? outer : 360.0f;
        CalculatePositionalCoefficients(coefficients, relative, *real->GetHeadMatrix(), inner, outer);
        f32 attenuation = CalculateFalloffAttenuation(distance);
        for (u32 i = 0; i < 8; ++i) {
            f32 gain = attenuation * coefficients[i];
            mix_gains[i] = focus->GetSensitivity() * gain;
        }
        if (NuSoundSystem::GetOutputChannelConfig() > 5) {
            f32 gain = attenuation * field113_0x10c;
            mix_gains[3] = focus->GetSensitivity() * gain;
        }
        field67_0xa8 = focus->GetSensitivity() * attenuation;
        NuSoundListener *selected[2] = {real, focus};
        for (u32 i = 0; i < 2; ++i) {
            NuSoundEffect::ManagedReference &ref = positional_references[i];
            NuSoundEffect::ManagedReference *head_ref = selected[i]->references;
            if (head_ref == NULL) {
                selected[i]->references = &ref;
                ref.next = &ref;
                ref.previous = &ref;
            } else {
                ref.next = head_ref->next;
                ref.previous = head_ref;
                head_ref->next = &ref;
                ref.next->previous = &ref;
            }
            ref.object = reinterpret_cast<NuSoundEffect::ReferenceTarget *>(selected[i]);
        }
        field65_0xa0 = attenuation;
        field64_0x9c = distance;
        return;
    }
    if (surround_mode == 3) {
        VuVec focus_position = position;
        f32 distance = 0.0f;
        NuSoundListener *focus = NuSoundSystem::GetNearestFocusListener(*listeners, focus_position, distance);
        if (focus == NULL || !(falloff_b > distance)) return;
        f32 attenuation = CalculateFalloffAttenuation(distance);
        field67_0xa8 = attenuation;
        for (u32 i = 0; i < 8; ++i) mix_gains[i] = focus->GetSensitivity() * attenuation;
        f32 lfe = attenuation * field113_0x10c;
        mix_gains[3] = focus->GetSensitivity() * lfe;
        field67_0xa8 = focus->GetSensitivity() * attenuation;
        NuSoundEffect::ManagedReference &ref = positional_references[1];
        NuSoundEffect::ManagedReference *head_ref = focus->references;
        if (head_ref == NULL) {
            focus->references = &ref;
            ref.next = &ref;
            ref.previous = &ref;
        } else {
            ref.next = head_ref->next;
            ref.previous = head_ref;
            head_ref->next = &ref;
            ref.next->previous = &ref;
        }
        ref.object = reinterpret_cast<NuSoundEffect::ReferenceTarget *>(focus);
        field65_0xa0 = attenuation;
        field64_0x9c = distance;
        return;
    }
    if (surround_mode == 0) {
        VuVec real_position = position;
        NuSoundListener *real = NuSoundSystem::GetNearestRealListener(*listeners, real_position);
        VuVec focus_position = position;
        f32 distance = 0.0f;
        NuSoundListener *focus = NuSoundSystem::GetNearestFocusListener(*listeners, focus_position, distance);
        if (real == NULL || !(focus->GetSensitivity() > 0.0f) || !(falloff_b > distance)) return;
        f32 attenuation = CalculateFalloffAttenuation(distance);
        if (!(attenuation > 0.0f)) return;
        VuVec head_position = position;
        f32 head_distance = real->GetHeadDistance(head_position);
        f32 second_distance = 0.0f;
        NuSoundListener *second = NULL;
        for (NuSoundListener *listener = listeners->head->next; listener != listeners->tail; listener = listener->next) {
            if (listener == real || !listener->IsEnabled()) continue;
            VuVec candidate_position = position;
            second_distance = listener->GetHeadDistance(candidate_position);
            if (6.0f > second_distance - head_distance) {
                second = listener;
                break;
            }
        }
        f32 coefficients[2][8] = {};
        f32 angle = 0.0f;
        if (real->Get2DScreenPosition() != NULL) {
            NUMTX identity;
            NuMtxSetIdentity(&identity);
            f32 z = 1.0f - real->Get2DScreenPosition()->y;
            f32 x = real->Get2DScreenPosition()->x;
            VuVec screen_position(x, 0.0f, z, 1.0f);
            f32 outer = field69_0xb0 + field71_0xb8;
            outer = outer < 360.0f ? outer : 360.0f;
            CalculatePositionalCoefficients(mix_gains, screen_position,
                reinterpret_cast<const VuMtx &>(identity), field69_0xb0, outer);
        } else {
            angle = CalculateFieldAngle(head_distance);
            f32 outer = angle + field71_0xb8;
            outer = outer < 360.0f ? outer : 360.0f;
            VuVec first_position = position;
            CalculatePositionalCoefficients(coefficients[0], first_position, *real->GetHeadMatrix(), angle, outer);
            if (second != NULL) {
                f32 second_angle = CalculateFieldAngle(second_distance);
                // The reference retains the first listener's outer angle here.
                VuVec second_position = position;
                CalculatePositionalCoefficients(coefficients[1], second_position, *second->GetHeadMatrix(), second_angle, outer);
                for (u32 i = 0; i < 8; ++i)
                    mix_gains[i] = coefficients[0][i] > coefficients[1][i] ? coefficients[0][i] : coefficients[1][i];
            } else {
                memmove(mix_gains, coefficients[0], sizeof(mix_gains));
            }
        }
        for (u32 i = 0; i < 8; ++i) mix_gains[i] = (focus->GetSensitivity() * attenuation) * mix_gains[i];
        if (NuSoundSystem::GetOutputChannelConfig() > 5) {
            f32 lfe = attenuation * field113_0x10c;
            mix_gains[3] = focus->GetSensitivity() * lfe;
        }
        field67_0xa8 = focus->GetSensitivity() * attenuation;
        NuSoundListener *selected[2] = {real, focus};
        for (u32 i = 0; i < 2; ++i) {
            NuSoundEffect::ManagedReference &ref = positional_references[i];
            NuSoundEffect::ManagedReference *head_ref = selected[i]->references;
            if (head_ref == NULL) {
                selected[i]->references = &ref;
                ref.next = &ref;
                ref.previous = &ref;
            } else {
                ref.next = head_ref->next;
                ref.previous = head_ref;
                head_ref->next = &ref;
                ref.next->previous = &ref;
            }
            ref.object = reinterpret_cast<NuSoundEffect::ReferenceTarget *>(selected[i]);
        }
        field65_0xa0 = attenuation;
        field63_0x98 = head_distance;
        field64_0x9c = distance;
        field66_0xa4 = angle;
    }
}

bool NuSoundVoice::AreStopEffectsRunning() const {
    if ((flags & 4) != 0) {
        for (NuListNode<NuSoundEffect *> *node = static_cast<NuListNode<NuSoundEffect *> *>(effects.Head()); node != effects.Tail(); node = static_cast<NuListNode<NuSoundEffect *> *>(node->GetNext())) {
            NuSoundEffect *effect = node->value;
            if (effect->unknown_08[1] == 1 && effect->unknown_08[2] == 1) return true;
        }
    }
    return false;
}

bool NuSoundVoice::CheckStopEffects() {
    if ((flags & 4) != 0) {
        for (NuListNode<NuSoundEffect *> *node = static_cast<NuListNode<NuSoundEffect *> *>(effects.Head()); node != effects.Tail(); node = static_cast<NuListNode<NuSoundEffect *> *>(node->GetNext())) {
            NuSoundEffect *effect = node->value;
            if (effect->unknown_08[1] == 1 && effect->unknown_08[2] == 1) return false;
        }
    }
    return true;
}

void NuSoundVoice::UpdateEffects(f32 frametime, NuSoundEffect::EffectProcessStage stage) {
    NuListNode<NuSoundEffect *> *node = static_cast<NuListNode<NuSoundEffect *> *>(effects.Head());
    while (node != effects.Tail()) {
        NuSoundEffect *effect = node->value;
        node = static_cast<NuListNode<NuSoundEffect *> *>(node->GetNext());
        if (effect->unknown_08[0] == static_cast<u32>(stage) && effect->enabled) {
            effect->ProcessVoice(this, frametime);
        }
        if (effect->unknown_08[2] == 2 && !effect->system_owned) {
            RemoveEffect(effect);
        }
    }
}

void NuSoundVoice::CheckStarvedBuffers() {
}

// ---------------------------------------------------------------------------
// remaining original surface (off the title music path; kept as stubs)
// ---------------------------------------------------------------------------

bool NuSoundVoice::AddEffect(NuSoundEffect *effect) {
    if (effects.length != 0) {
        NuListNodeBase *last = effects.tail->GetPrev();
        NuListNodeBase *node = effects.Head();
        for (;;) {
            if (static_cast<NuListNode<NuSoundEffect *> *>(node)->value == effect) return false;
            if (node == last) break;
            node = node->GetNext();
        }
    }
    bool attached = effect->AttachVoice(this);
    if (attached) NuSoundMemory::PushNuListNode(effects, effect);
    return attached;
}

bool NuSoundVoice::BeginStopEffects() {
    if ((flags & 4) == 0) {
        for (NuListNode<NuSoundEffect *> *node = static_cast<NuListNode<NuSoundEffect *> *>(effects.Head()); node != effects.Tail(); node = static_cast<NuListNode<NuSoundEffect *> *>(node->GetNext())) {
            NuSoundEffect *effect = node->value;
            if (effect->unknown_08[1] == 1) {
                effect->Enable();
                flags |= 4;
            }
        }
    }
    return (flags & 4) != 0;
}

f32 NuSoundVoice::CalculateEffectAttenuation() {
    f32 value = 1.0f;
    for (NuListNode<NuSoundEffect *> *node = static_cast<NuListNode<NuSoundEffect *> *>(effects.Head()); node != effects.Tail(); node = static_cast<NuListNode<NuSoundEffect *> *>(node->GetNext())) {
        value *= node->value->attenuation;
    }
    return value;
}

f32 NuSoundVoice::CalculateEffectPitchScale() {
    f32 value = 1.0f;
    for (NuListNode<NuSoundEffect *> *node = static_cast<NuListNode<NuSoundEffect *> *>(effects.Head()); node != effects.Tail(); node = static_cast<NuListNode<NuSoundEffect *> *>(node->GetNext())) {
        value *= node->value->pitch_scale;
    }
    return value;
}

f32 NuSoundVoice::CalculateFalloffAttenuation(f32 distance) {
    if (distance > falloff_a) {
        if (falloff_type == 0) {
            return (falloff_b - distance) / (falloff_b - falloff_a);
        }
        if (falloff_type == 1) {
            f32 factor = (falloff_b - distance) / (falloff_b - falloff_a);
            f32 scaled = (1.0f - factor) * 10.0f + 1.0f;
            return 1.0f / (scaled * scaled);
        }
    }
    return 1.0f;
}

f32 NuSoundVoice::CalculateFieldAngle(f32 distance) {
    f32 angle = field69_0xb0;
    if (field73_0xc0 > distance) {
        if (field72_0xbc > distance) return field70_0xb4;
        f32 scale = (field73_0xc0 - distance) / (field73_0xc0 - field72_0xbc);
        angle += scale * (field70_0xb4 - angle);
    }
    return angle;
}

void NuSoundVoice::CalculatePositionalCoefficients(f32 *gains, VuVec const &position,
                                                VuMtx const &mtx, f32 inner_angle, f32 outer_angle) {
    NUVEC local;
    NuVecInvMtxTransform(&local, (NUVEC *)&position, (numtx_s *)&mtx);
    f32 angle = NuFmod(NuATan2f(local.x, local.z) * 180.0f / 3.1415927410125732f, 360.0f);
    outer_angle *= 0.5f;
    f32 outer_left = NuFmod(angle - outer_angle, 360.0f);
    f32 outer_right = NuFmod(angle + outer_angle, 360.0f);
    inner_angle *= 0.5f;
    f32 inner_left = NuFmod(angle - inner_angle, 360.0f);
    f32 inner_right = NuFmod(angle + inner_angle, 360.0f);
    // The reference visits both wrapped representations before the positive
    // center endpoint. Existing nonzero gains are preserved; LFE is untouched.
    const f32 speaker_angles[] = {-360, -330, -270, -210, -150, -90, -30,
                                   0,   30,   90,  150,  210, 270, 330, 360};
    const u32 channels[] = {2, 1, 5, 7, 6, 4, 0, 2, 1, 5, 7, 6, 4, 0, 2};
    for (u32 i = 0; i < 15; ++i) {
        f32 speaker = speaker_angles[i];
        u32 channel = channels[i];
        if (gains[channel] == 0.0f && speaker > outer_left && outer_right > speaker) {
            if (speaker > inner_left && inner_right > speaker) {
                gains[channel] = 1.0f;
            } else {
                bool left = speaker - angle < 0.0f;
                f32 outer = left ? outer_left : outer_right;
                f32 inner = left ? inner_left : inner_right;
                f32 gain = fabsf((outer - speaker) / (outer - inner));
                gains[channel] = gain < 1.0f ? gain : 1.0f;
            }
        }
    }
}

i32 NuSoundVoice::GetControllerBits() const {
    return control_118;
}

const VuVec * NuSoundVoice::GetDirection() const {
    return &direction;
}

NuSoundSystem::DownmixType NuSoundVoice::GetDownmixerType() const {
    return field15_0x38;
}

NuSoundEffect *NuSoundVoice::GetEffect(NuSoundEffect::EffectType type) {
    for (NuListNode<NuSoundEffect *> *node = static_cast<NuListNode<NuSoundEffect *> *>(effects.Head()); node != effects.Tail(); node = static_cast<NuListNode<NuSoundEffect *> *>(node->GetNext())) {
        NuSoundEffect *effect = node->value;
        if (effect->unknown_08[3] == static_cast<u32>(type)) return effect;
    }
    return NULL;
}

NuSoundSystem::FalloffType NuSoundVoice::GetFalloffType() const {
    return static_cast<NuSoundSystem::FalloffType>(falloff_type);
}

f32 NuSoundVoice::GetFar() const {
    return falloff_b;
}

f32 NuSoundVoice::GetLowFrequencyMix() const {
    return field113_0x10c;
}

f32 NuSoundVoice::GetNear() const {
    return falloff_a;
}

u32 NuSoundVoice::GetNumEffects() const {
    return effects.length;
}

NuSoundBus * NuSoundVoice::GetOutputBus() const {
    return output_bus;
}

f32 NuSoundVoice::GetPenetration() const {
    return field74_0xc4;
}

f32 NuSoundVoice::GetPitch() const {
    return pitch;
}

f32 NuSoundVoice::GetPlaybackPositionSeconds() {
    u64 samples = GetPlaybackPositionSamples();
    i32 rate = static_cast<i32>(sound_source->GetStreamDesc()->GetSampleRate());
    return static_cast<f32>(samples) / static_cast<f32>(rate);
}

const VuVec *NuSoundVoice::GetPosition() const {
    return &position;
}

f32 NuSoundVoice::GetReverbWetMix() const {
    return field130_0x144;
}

NuSoundRoutingTable * NuSoundVoice::GetRoutingTable() const {
    return field16_0x3c;
}

f32 NuSoundVoice::GetSpeakerBleedAngle() const {
    return field71_0xb8;
}

f32 NuSoundVoice::GetSpeakerBleedFar() const {
    return field73_0xc0;
}

f32 NuSoundVoice::GetSpeakerBleedNear() const {
    return field72_0xbc;
}

f32 NuSoundVoice::GetSpeakerFieldAngleMax() const {
    return field70_0xb4;
}

f32 NuSoundVoice::GetSpeakerFieldAngleMin() const {
    return field69_0xb0;
}

f32 NuSoundVoice::GetStartOffset() const {
    return field114_0x110;
}

NuSoundSystem::SurroundMode NuSoundVoice::GetSurroundMode() const {
    return static_cast<NuSoundSystem::SurroundMode>(surround_mode);
}

const VuVec *NuSoundVoice::GetVelocity() const {
    return &velocity;
}

f32 NuSoundVoice::GetVolume() const {
    return volume;
}

bool NuSoundVoice::IsLooping() const {
    return (flags & 8) != 0;
}

void NuSoundVoice::RegisterHandle(NuSoundHandle *) {
}

void NuSoundVoice::RemoveEffect(NuSoundEffect *effect) {
    NuListNodeBase *found = effects.Head();
    while (found != effects.tail && static_cast<NuListNode<NuSoundEffect *> *>(found)->value != effect) found = found->next;
    if (found == effects.tail) return;
    effect->DetachVoice(this);
    if (effects.length != 0) {
        NuListNodeBase *last = effects.tail->prev;
        NuListNodeBase *node = effects.Head();
        unsigned int removed = 0;
        for (;;) {
            if (static_cast<NuListNode<NuSoundEffect *> *>(node)->value == effect) {
                bool is_last = node == last;
                NuListNodeBase *next = node->next;
                NuListNodeBase *previous = node->prev;
                if (previous != NULL) previous->next = next;
                if (next != NULL) next->prev = previous;
                NuMemoryGet()->GetThreadMem()->BlockFree(node, 0);
                ++removed;
                if (is_last) break;
                node = next;
                if (node == last) break;
                // The original advances again after removing a non-final node.
                node = node->next;
            } else {
                if (node == last) break;
                node = node->next;
            }
        }
        effects.length -= removed;
    }
    NuSoundSystem::sAllocdMemory[0] -= sizeof(NuListNode<NuSoundEffect *>);
}



void NuSoundVoice::SetControllerBits(i32 bits) {
    control_118 = static_cast<u8>(bits);
}

void NuSoundVoice::SetCustomSurroundMix(f32 *) {
}

void NuSoundVoice::SetDirection(VuVec *value) {
    if (value != NULL) {
        direction.x = value->x;
        direction.y = value->y;
        direction.z = value->z;
        direction.w = value->w;
        NuVecNorm(reinterpret_cast<NUVEC *>(&direction), reinterpret_cast<NUVEC *>(&direction));
    }
}

void NuSoundVoice::SetDownmixerType(NuSoundSystem::DownmixType type) {
    field15_0x38 = type;
}

void NuSoundVoice::SetFalloff(f32 near, f32 far, NuSoundSystem::FalloffType type) {
    if (near >= 0.0f && far > near) {
        falloff_a = near;
        falloff_b = far;
        falloff_type = static_cast<u32>(type);
    }
}

void NuSoundVoice::SetLowFrequencyMix(f32 mix) {
    field113_0x10c = mix;
}

void NuSoundVoice::SetOutputBus(NuSoundBus *bus) {
    output_bus = bus == NULL ? NuSoundSystem::sMasterBus : bus;
}

void NuSoundVoice::SetOutputDevices(i32 devices) {
    field115_0x114 = devices;
}

void NuSoundVoice::SetPenetration(f32 penetration) {
    field74_0xc4 = penetration;
}

void NuSoundVoice::SetPosition(VuVec *value) {
    if (value != NULL) position = *value;
}

void NuSoundVoice::SetReverbWetMix(f32 mix) {
    this->field130_0x144 = mix;
}

void NuSoundVoice::SetRoutingTable(NuSoundRoutingTable *table) {
    field16_0x3c = table;
}

void NuSoundVoice::SetSpeakerBleedAngle(f32 angle) {
    field71_0xb8 = angle;
}

void NuSoundVoice::SetSpeakerBleedFar(f32 distance) {
    field73_0xc0 = distance;
}

void NuSoundVoice::SetSpeakerBleedNear(f32 distance) {
    field72_0xbc = distance;
}

void NuSoundVoice::SetSpeakerFieldAngle(f32 minimum, f32 maximum) {
    field69_0xb0 = minimum;
    field70_0xb4 = maximum;
}

void NuSoundVoice::SetStartOffset(f32 offset) {
    field114_0x110 = offset;
}

void NuSoundVoice::SetListeners(NuEList<NuSoundListener, DefaultElist> const *value) {
    listeners = value;
}

void NuSoundVoice::SetSurroundMode(NuSoundSystem::SurroundMode value) {
    surround_mode = static_cast<u32>(value);
}

void NuSoundVoice::SetVelocity(VuVec const &value) {
    velocity.x = value.x;
    velocity.y = value.y;
    velocity.z = value.z;
    velocity.w = value.w;
}

void NuSoundVoice::UnregisterHandle(NuSoundHandle *) {
}
