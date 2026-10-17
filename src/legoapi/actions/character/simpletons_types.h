#pragma once

// Types shared by legoapi/actions/character/simpletons.cpp and legoapi/ai/core/gameai.cpp.

struct AtOnceAttacker {
    GameObject_s *object;
    f32 distance;
};
