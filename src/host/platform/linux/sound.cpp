#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nusound/nusound_voice.hpp"

extern "C" void __real__ZN12NuSoundVoice11SetPositionEP5VuVec(NuSoundVoice *voice, VuVec *position);

extern "C" void __wrap__ZN12NuSoundVoice11SetPositionEP5VuVec(NuSoundVoice *voice, VuVec *position) {
    if (!position) {
        __real__ZN12NuSoundVoice11SetPositionEP5VuVec(voice, nullptr);
        return;
    }

    // Some game callers pass a three-float nuvec_s; reading VuVec::w would overrun it.
    VuVec safe_position{position->x, position->y, position->z, 1.0f};
    __real__ZN12NuSoundVoice11SetPositionEP5VuVec(voice, &safe_position);
}
