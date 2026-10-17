#pragma once

// Types shared by legoapi/actions/character/specialmoves.cpp and legoapi/actions/movement/jumping.cpp.

struct SPECIALMOVE_s {
    i16 attacker_animation;
    i16 attacker_id;
    i8 attacker_action_type;
    i8 victim_action_type;
    i16 victim_id;
    i16 victim_animation;
    u16 flags;
    f32 distance;
};
