#pragma once

// Types shared by legoapi/ai/core/gameai.cpp and legogame/game.cpp.

struct ADAPTIVEDIFFICULTY_s {
    i32 difficulty;
    f32 multiplier;
    f32 elapsed;
};
