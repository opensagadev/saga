#include "nu2api_nusound_types.h"
#include "nusound_voice.hpp"
#include "nu2api/numath/nuvec.h"

NuSoundEffectDoppler::NuSoundEffectDoppler() {
    unknown_08[0] = 0;
    unknown_08[1] = 0;
    unknown_08[2] = 1;
    unknown_08[3] = 8;
    attenuation = 1.0f;
    pitch_scale = 1.0f;
    system_owned = false;
    enabled = true;
    speed_of_sound = 343.0f;
    velocity_scale = 1.0f;
    listeners = NULL;
}

void NuSoundEffectDoppler::ProcessVoice(NuSoundVoice *voice, float) {
    if (!enabled || listeners == NULL || voice->GetPosition() == NULL) return;
    NuSoundListener *listener = NuSoundSystem::GetNearestRealListener(*listeners, *voice->GetPosition());
    if (listener == NULL || listener->GetFocusPosition() == NULL || voice->GetPosition() == NULL) return;
    const VuVec *source_velocity = voice->GetVelocity();
    const VuVec *listener_velocity = listener->GetVelocity();
    const VuVec *source_position = voice->GetPosition();
    const VuVec *listener_position = listener->GetFocusPosition();
    NUVEC direction;
    direction.x = listener_position->x - source_position->x;
    direction.y = listener_position->y - source_position->y;
    direction.z = listener_position->z - source_position->z;
    NuVecNorm(&direction, &direction);
    float listener_speed = (listener_velocity->x * direction.x + listener_velocity->y * direction.y) + listener_velocity->z * direction.z;
    float source_speed = (direction.x * source_velocity->x + direction.y * source_velocity->y) + direction.z * source_velocity->z;
    pitch_scale = (speed_of_sound - listener_speed * velocity_scale) / (speed_of_sound - source_speed * velocity_scale);
}

void NuSoundEffectDoppler::SetParameters(float speed, float scale, NuEList<NuSoundListener, DefaultElist> const *list) {
    speed_of_sound = speed;
    velocity_scale = scale;
    listeners = list;
}

NuSoundEffectDoppler::~NuSoundEffectDoppler() {
}
