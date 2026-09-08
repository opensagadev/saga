#include "nu2api_nusound_types.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "globals.h"
#include "nu2api/nu3d/nuspecial.h"

#include <string.h>

extern i32 VehicleArea;
extern "C" edanim_param_s AnimParams[64];
extern "C" i32 edanim_nearest;
extern "C" i32 edanim_nearest_param_id;
extern "C" i32 edanim_sound_type;
extern "C" NUGSCN *edbits_base_scene;
extern "C" char *edbitsGetSoundName(i32 sound_type);
extern "C" void NuGScnGetSpecial(nuhspecial_s *special, NUGSCN *scene, i32 index);
extern "C" void SetSoundVolume(f32 volume);
void legoSetCutVolume(f32 volume);
f32 GameSetMusicVolume(OPTIONSSAVE_s *options);
f32 GameSetSoundVolume(OPTIONSSAVE_s *options);

void edanimSoundPlace(i32 sound_index, nuvec_s *position) {
    nuhspecial_s special;
    NuGScnGetSpecial(&special, edbits_base_scene, edanim_nearest);
    NuVecSub(reinterpret_cast<NUVEC *>(AnimParams[edanim_nearest_param_id].sound_positions[sound_index]), position,
             NuSpecialGetPos(&special));
}

void edanimSoundCreate(nuvec_s *position) {
    edanim_param_s &params = AnimParams[edanim_nearest_param_id];
    i32 sound_index = params.sound_count;
    if (sound_index != 8 && edanim_sound_type != -1) {
        edanimSoundPlace(sound_index, position);
        params.sound_ids[sound_index] = edanim_sound_type;
        params.sound_flags[sound_index] = 1;
        strcpy(params.sound_names[sound_index], edbitsGetSoundName(edanim_sound_type));
        params.sound_count++;
        params.sound_values[sound_index] = 50.0f;
    }
}
