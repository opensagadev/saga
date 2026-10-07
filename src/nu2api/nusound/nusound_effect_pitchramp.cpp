#include "nu2api_nusound_types.h"

#include "nu2api/numath/nufloat.h"

bool NuSoundEffectPitchRamp::AttachVoice(NuSoundVoice *) {
    return true;
}

NuSoundEffectPitchRamp::NuSoundEffectPitchRamp()
    : NuSoundEffect(EffectType::PITCH_RAMP, EffectProcessStage::ZERO), target_pitch(1.0f), duration(0.0f),
      finished(false), finish_state(FinishState::NONE) {
    state = 0;
}

void NuSoundEffectPitchRamp::Process(float frametime) {
    if (enabled && pitch_mix != target_pitch) {
        state = 1;
        finished = false;
        if (duration == 0.0f) {
            pitch_mix = target_pitch;
        } else {
            if (target_pitch > pitch_mix) {
                pitch_mix += NuFdiv(frametime, duration);
                if (pitch_mix > target_pitch) {
                    state = 0;
                    finished = true;
                    pitch_mix = target_pitch;
                }
            } else {
                pitch_mix -= NuFdiv(frametime, duration);
                if (pitch_mix < target_pitch) {
                    state = 0;
                    finished = true;
                    pitch_mix = target_pitch;
                }
            }
        }
    }
}

void NuSoundEffectPitchRamp::ProcessVoice(NuSoundVoice *voice, float) {
    if (finished) {
        if (finish_state == FinishState::STOP) {
            voice->Stop(true);
        }
        finished = false;
    }
}

void NuSoundEffectPitchRamp::SetParameters(float target, float time, NuSoundEffectPitchRamp::FinishState finish) {
    finish_state = finish;
    target_pitch = target;
    duration = time;
    if (target < 0.01f) {
        target_pitch = 0.01f;
    }
    if (time == 0.0f) {
        pitch_mix = target;
    }
}

NuSoundEffectPitchRamp::~NuSoundEffectPitchRamp() {
}
