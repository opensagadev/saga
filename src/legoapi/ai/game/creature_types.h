#pragma once

// Types shared by legoapi/ai/game/creature.cpp and legoapi/ai/core/gameai.cpp.

enum AI_CREATURE_FLAGS : i32 {
    AI_CREATURE_FLAG_FORMATION_REVERSED = 0x01,
    AI_CREATURE_FLAG_SKIP_LOW_END = 0x20,
};
enum AI_CREATURE_RESET_MODE : u8 {
    AI_CREATURE_RESET_READY = 0,
    AI_CREATURE_RESET_STAGGERED = 1,
    AI_CREATURE_RESET_ACTIVE = 2,
    AI_CREATURE_RESET_DISABLED = 4,
};
