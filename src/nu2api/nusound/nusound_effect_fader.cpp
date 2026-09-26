#include "decomp.h"
#include "nu2api_nusound_types.h"

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
    enabled = true;
    state = progress < 1.0f;
}

NuSoundEffectFader::NuSoundEffectFader()
    : NuSoundEffect(EffectType::FADER, EffectProcessStage::ZERO), curve{0, NULL}, start_mix(1.0f), target_mix(1.0f),
      duration(0.0f), progress(1.0f), decreasing(0), finish_state(FinishState::NONE), callback(NULL), finished(false) {
    state = 0;
}

void NuSoundEffectFader::Process(float frametime) {
    if (!enabled || !(progress < 1.0f)) {
        return;
    }

    state = 1;
    if (duration == 0.0f) {
        progress = 1.0f;
        output_mix = target_mix;
        return;
    }

    progress = MAX(0.0f, MIN(progress + (frametime != 0.0f ? frametime / duration : 0.0f), 1.0f));

    f32 target_weight = 0.0f;
    f32 start_weight = 1.0f;
    if (curve.type == 0) {
        target_weight = progress;
        start_weight = 1.0f - progress;
    } else if (curve.type == 1) {
        target_weight = NuSoundSystem::GetInstance()->CalculateCrossfadeHeight(
            *static_cast<const NuSoundSystem::CurveData *>(curve.data), progress);
        start_weight = 1.0f - target_weight;
    }

    output_mix = target_weight * target_mix + start_weight * start_mix;
    if (progress == 1.0f) {
        state = 0;
        finished = true;
    }
}

void NuSoundEffectFader::ProcessBus(NuSoundBus *, float) {
}

void NuSoundEffectFader::ProcessVoice(NuSoundVoice *voice, float) {
    if (!finished) {
        return;
    }

    if (finish_state == FinishState::PAUSE) {
        voice->Pause();
    } else if (finish_state == FinishState::CALLBACK) {
        if (callback != NULL) {
            callback->OnFinished();
        }
    } else if (finish_state == FinishState::STOP) {
        voice->Stop(true);
    }
    finished = false;
}

void NuSoundEffectFader::SetCurveParams(NuSoundEffectFader::Curve const &new_curve) {
    curve = new_curve;
}

void NuSoundEffectFader::SetParameters(float target, float time, NuSoundEffectFader::FinishState finish) {
    if (target != target_mix || time != duration) {
        start_mix = output_mix;
        decreasing = !(target > output_mix);
        f32 initial_progress = 0.0f;

        if (!(time > 0.0f)) {
            output_mix = target;
            state = 0;
            initial_progress = 1.0f;
        }

        progress = initial_progress;
        if (target == output_mix) {
            progress = 1.0f;
            state = 0;
            finished = true;
        }
        target_mix = target;
        duration = time;
    }
    finish_state = finish;
}

NuSoundEffectFader::~NuSoundEffectFader() {
}
