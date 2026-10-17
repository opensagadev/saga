#pragma once

// Types shared by legoapi/characters/core/characters.cpp and gameapi/ai/aisys/gameaiscript.cpp.

enum LegoObjectId : i16 {
    LEGO_OBJECT_FLOOR_TARGET = 0x55,
};
struct DefaultIdleCharacterData {
    u8 pad[0x116];
    u8 use_standard_idle;
};
