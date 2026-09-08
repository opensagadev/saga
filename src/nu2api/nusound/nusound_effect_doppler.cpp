#include "nu2api_nusound_types.h"

#include "nu2api/numath/nuvec.h"

NuSoundEffectDoppler::NuSoundEffectDoppler()
    : NuSoundEffect(EffectType::DOPPLER, EffectProcessStage::ZERO), speed_of_sound(343.0f), velocity_scale(1.0f),
      listeners(NULL) {
}

void NuSoundEffectDoppler::ProcessVoice(NuSoundVoice *voice, float) {
    if (!enabled || listeners == NULL || voice->GetPosition() == NULL) {
        return;
    }

    NuSoundListener *listener = NuSoundSystem::GetNearestRealListener(*listeners, *voice->GetPosition());
    if (listener == NULL || listener->GetFocusPosition() == NULL || voice->GetPosition() == NULL) {
        return;
    }

    const VuVec *voice_velocity = voice->GetVelocity();
    const VuVec *listener_velocity = listener->GetVelocity();
    const VuVec *voice_position = voice->GetPosition();
    const VuVec *listener_position = listener->GetFocusPosition();

    VuVec direction(listener_position->x - voice_position->x, listener_position->y - voice_position->y,
                    listener_position->z - voice_position->z, 0.0f);
    NuVecNorm(reinterpret_cast<NUVEC *>(&direction), reinterpret_cast<NUVEC *>(&direction));

    f32 listener_speed =
        listener_velocity->x * direction.x + listener_velocity->y * direction.y + listener_velocity->z * direction.z;
    f32 voice_speed =
        voice_velocity->x * direction.x + voice_velocity->y * direction.y + voice_velocity->z * direction.z;
    pitch_mix = (speed_of_sound - listener_speed * velocity_scale) / (speed_of_sound - voice_speed * velocity_scale);
}

void NuSoundEffectDoppler::SetParameters(float speed, float scale,
                                         NuEList<NuSoundListener, DefaultElist> const *listener_list) {
    speed_of_sound = speed;
    velocity_scale = scale;
    listeners = listener_list;
}

NuSoundEffectDoppler::~NuSoundEffectDoppler() {
}
