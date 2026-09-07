#include "nu2api_nusound_types.h"
#include "globals.h"

extern i32 VehicleArea;
extern "C" void SetSoundVolume(f32 volume);
void legoSetCutVolume(f32 volume);
f32 GameSetMusicVolume(OPTIONSSAVE_s *options);
f32 GameSetSoundVolume(OPTIONSSAVE_s *options);

void SetSoundFadeDist(WORLDINFO_s *world, OPTIONSSAVE_s *options) {
    options->field3_0x3 = 8;
    options->field4_0x4 = 6;
    if (SetSoundFadeDistCallBackFn == NULL || SetSoundFadeDistCallBackFn(world) == 0) {
        if (VehicleArea == 0) {
            nusound_fade_start = 2.0f;
            nusound_fade_end = 15.0f;
        } else {
            nusound_fade_start = 10.0f;
            nusound_fade_end = 80.0f;
        }
    }
    GameSetSoundVolume(options);
    GameSetMusicVolume(options);
}

void edanimSoundPlace(i32, nuvec_s *) {
}

void edanimSoundCreate(nuvec_s *) {
}

f32 GameSetSoundVolume(OPTIONSSAVE_s *options) {
    f32 volume = (static_cast<f32>(static_cast<u8>(options->field3_0x3)) / 10.0f) *
                 (static_cast<f32>(static_cast<u8>(options->field5_0x5)) / 10.0f);
    SetSoundVolume(volume);
    legoSetCutVolume((static_cast<f32>(static_cast<u8>(options->field5_0x5)) * 0.85f) / 10.0f);
    return volume;
}
