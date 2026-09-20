#pragma once

#include "gameapi/edtools/gameapi_edtools_types.h"

// Animation-editor state shared by the reconstructed editor and sound owners.
extern "C" {
    extern edanim_param_s AnimParams[64];
    extern nugscn_s *edanim_page_scene[8];
    extern i32 edanim_nearest;
    extern i32 edanim_nearest_param_id;
    extern i32 edanim_nearest_sound;
    extern i32 edanim_sound_type;
}
