#include "nu2api_nusound_types.h"
#include <float.h>

#include <float.h>

void SetSoundFadeDist(WORLDINFO_s *, OPTIONSSAVE_s *) {
}

void edanimSoundPlace(i32, nuvec_s *) {
}

void edanimSoundCreate(nuvec_s *) {
}

f32 GameSetSoundVolume(OPTIONSSAVE_s *) {
    return 0.0f;
}

NuSoundListener *NuSoundSystem::GetNearestRealListener(NuEList<NuSoundListener, DefaultElist> const &listeners,
                                                       VuVec const &position) {
    f32 nearest_distance = FLT_MAX;
    NuSoundListener *nearest = NULL;

    NuSoundListener *end = listeners.end;
    NuSoundListener *listener = static_cast<NuSoundListener *>(listeners.begin->field_0x4);
    while (listener != end) {
        if (listener->IsEnabled()) {
            if (listener->GetSensitivity() > 0.0f) {
                f32 distance = listener->GetHeadDistance(position) / listener->GetSensitivity();
                if (nearest_distance > distance || nearest == NULL) {
                    nearest_distance = distance;
                    nearest = listener;
                }
            }
        }
        listener = static_cast<NuSoundListener *>(listener->field_0x4);
    }
    return nearest;
}

NuSoundListener *NuSoundSystem::GetNearestFocusListener(NuEList<NuSoundListener, DefaultElist> const &listeners,
                                                        VuVec const &position, float &nearest_distance) {
    nearest_distance = FLT_MAX;
    NuSoundListener *nearest = NULL;

    NuSoundListener *end = listeners.end;
    NuSoundListener *listener = static_cast<NuSoundListener *>(listeners.begin->field_0x4);
    while (listener != end) {
        if (listener->IsEnabled()) {
            if (listener->GetSensitivity() > 0.0f) {
                f32 distance = listener->GetAttenuationDistance(position) / listener->GetSensitivity();
                if (nearest_distance > distance || nearest == NULL) {
                    nearest_distance = distance;
                    nearest = listener;
                }
            }
        }
        listener = static_cast<NuSoundListener *>(listener->field_0x4);
    }
    return nearest;
}
