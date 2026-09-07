#include "nu2api_nusound_types.h"
#include "nusound_android.hpp"

static_assert(sizeof(void *) != 4 || offsetof(NuSoundEffectFader, curve) == 0x44,
              "fader curve must retain its Android offset");

bool NuSoundEffectFader::AttachBus(NuSoundBus *) {
    return true;
}

bool NuSoundEffectFader::AttachVoice(NuSoundVoice *) {
    return true;
}

void NuSoundEffectFader::Disable() {
    enabled = false;
}

void NuSoundEffectFader::Enable() {
    unknown_08[2] = unknown_58 < 1.0f;
    enabled = true;
}

NuSoundEffectFader::NuSoundEffectFader() {
    unknown_08[0] = 0;
    unknown_08[1] = 0;
    unknown_08[2] = 0;
    unknown_08[3] = 2;
    system_owned = false;
    attenuation = 1.0f;
    enabled = true;
    pitch_scale = 1.0f;
    curve.mode = 0;
    curve.data = NULL;
    unknown_4c = 1.0f;
    unknown_50 = 1.0f;
    unknown_54 = 0;
    unknown_58 = 1.0f;
    unknown_5c = 0;
    unknown_60 = 0;
    unknown_64 = 0;
    unknown_68 = false;
}

void NuSoundEffectFader::Process(float frametime) {
    if (!enabled || !(unknown_58 < 1.0f)) return;
    unknown_08[2] = 1;
    if (unknown_54 == 0.0f) {
        unknown_58 = 1.0f;
        attenuation = unknown_50;
        return;
    }
    f32 step = frametime == 0.0f ? 0.0f : frametime / unknown_54;
    f32 progress = unknown_58 + step;
    if (progress < 1.0f && progress < 0.0f) progress = 0.0f;
    else if (!(progress < 1.0f)) progress = 1.0f;
    unknown_58 = progress;
    f32 destination_weight = 0.0f;
    f32 source_weight = 1.0f;
    if (curve.mode == 0) {
        destination_weight = progress;
        source_weight = 1.0f - progress;
    } else if (curve.mode == 1) {
        destination_weight = NuSound.CalculateCrossfadeHeight(
            *static_cast<NuSoundSystem::CurveData *>(curve.data), progress);
        source_weight = 1.0f - destination_weight;
        progress = unknown_58;
    }
    attenuation = destination_weight * unknown_50 + source_weight * unknown_4c;
    if (progress == 1.0f) {
        unknown_08[2] = 0;
        unknown_68 = true;
    }
}

void NuSoundEffectFader::ProcessBus(NuSoundBus *, float) {
}

void NuSoundEffectFader::ProcessVoice(NuSoundVoice *voice, float) {
    if (!unknown_68) return;
    switch (unknown_60) {
    case 1:
        voice->Stop(true);
        break;
    case 2:
        voice->Pause();
        break;
    case 3:
        if (unknown_64 != NULL) unknown_64->OnFinish();
        break;
    }
    unknown_68 = false;
}

void NuSoundEffectFader::SetCurveParams(NuSoundEffectFader::Curve const &value) {
    curve = value;
}

void NuSoundEffectFader::SetParameters(float target, float duration, NuSoundEffectFader::FinishState state) {
    if (target != unknown_50 || duration != unknown_54) {
        f32 current = attenuation;
        unknown_4c = current;
        unknown_5c = !(target > current);
        f32 progress = 0.0f;
        if (!(duration > 0.0f)) {
            attenuation = target;
            unknown_08[2] = 0;
            progress = 1.0f;
            current = target;
        }
        if (target == current) {
            unknown_58 = 1.0f;
            unknown_08[2] = 0;
            unknown_68 = true;
        } else {
            unknown_58 = progress;
        }
        unknown_50 = target;
        unknown_54 = duration;
    }
    unknown_60 = static_cast<u32>(state);
}

NuSoundEffectFader::~NuSoundEffectFader() {
}
