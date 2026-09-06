#include "nu2api_nusound_types.h"
#include "nusound_voice.hpp"

bool NuSoundEffectPitchRamp::AttachVoice(NuSoundVoice *) {
    return true;
}

NuSoundEffectPitchRamp::NuSoundEffectPitchRamp() {
    unknown_08[0] = 0;
    unknown_08[1] = 0;
    unknown_08[2] = 0;
    unknown_08[3] = 3;
    attenuation = 1.0f;
    pitch_scale = 1.0f;
    system_owned = false;
    enabled = true;
    field_44 = 1.0f;
    field_48 = 0.0f;
    field_4c = false;
    field_50 = 0;
}

void NuSoundEffectPitchRamp::Process(float frametime) {
    if (!enabled || pitch_scale == field_44) return;
    unknown_08[2] = 1;
    field_4c = false;
    if (field_48 == 0.0f) {
        pitch_scale = field_44;
        return;
    }
    float step = frametime == 0.0f ? 0.0f : frametime / field_48;
    if (field_44 > pitch_scale) {
        pitch_scale += step;
        if (pitch_scale > field_44) {
            unknown_08[2] = 0;
            field_4c = true;
            pitch_scale = field_44;
        }
    } else {
        pitch_scale -= step;
        if (field_44 > pitch_scale) {
            unknown_08[2] = 0;
            field_4c = true;
            pitch_scale = field_44;
        }
    }
}

void NuSoundEffectPitchRamp::ProcessVoice(NuSoundVoice *voice, float) {
    if (field_4c) {
        if (field_50 == 1) voice->Stop(true);
        field_4c = false;
    }
}

void NuSoundEffectPitchRamp::SetParameters(float target, float divisor, NuSoundEffectPitchRamp::FinishState state) {
    field_50 = static_cast<u32>(state);
    field_44 = target;
    field_48 = divisor;
    if (0.01f > target) field_44 = 0.01f;
    if (divisor == 0.0f) pitch_scale = target;
}

NuSoundEffectPitchRamp::~NuSoundEffectPitchRamp() {
}
