#include "globals.h"
#include "legoapi/actions/character/suit.h"
#include "legoapi/characters/core/character.h"

SUIT_s Suit[10] = {
    {"batman", "batman", &tBATMANSUIT, {0x0000}, 'b', 'b', 0x0000, 0, 0, 0},
    {"batman", "batman_shadow", &tSHADOWSUIT, {0x0001}, 'b', 'i', 0x0000, 0, 0, 0},
    {"batman", "batman_glide", &tGLIDESUIT, {0x0102}, 'b', 'a', 0x0004, 0, 0, 0},
    {"batman", "batman_demolition", &tDEMOLITIONSUIT, {0x0104}, 'b', 'c', 0x0000, 0, 0, 0},
    {"batman", "batman_sonar", &tSONARSUIT, {0x0008}, 'b', 'h', 0x0000, 0, 0, 0},
    {"robin", "robin", &tROBINSUIT, {0x0000}, 'r', 'r', 0x0000, 0, 0, 0},
    {"robin", "robin_water", &tWATERSUIT, {0x0110}, 'r', 'w', 0x0000, 0, 0, 0},
    {"robin", "robin_technology", &tTECHNOLOGYSUIT, {0x0120}, 'r', 't', 0x0000, 0, 0, 0},
    {"robin", "robin_magnet", &tMAGNETSUIT, {0x0040}, 'r', 'm', 0x2000, 0, 0, 0},
    {"robin", "robin_attract", &tATTRACTSUIT, {0x0180}, 'r', 'd', 0x0000, 0, 0, 0},
};

void Suits_Init() {
    for (i32 index = 0; index < 10; ++index) {
        Suit[index].index = static_cast<u8>(index);
        Suit[index].character_id = static_cast<i16>(CharIDFromName(Suit[index].base_character_name));
    }
}

SUIT_s *Suit_GetLast(i32 character_id, i32 require_owned) {
    for (i32 index = 9; index >= 0; --index) {
        if (require_owned != 0 && (areaSuitBits & (1u << index)) == 0) {
            continue;
        }
        if (Suit[index].character_id == character_id) {
            return &Suit[index];
        }
    }
    return NULL;
}

SUIT_s *Suit_GetNext(SUIT_s *suit) {
    return &Suit[(suit->index + 1) % 10];
}

i32 Suit_GetIndex(SUIT_s *suit) {
    for (i32 index = 0; index < 10; ++index) {
        if (&Suit[index] == suit) {
            return index;
        }
    }
    return -1;
}

SUIT_s *Suit_GetDefault(i32 character_id) {
    for (i32 index = 0; index < 10; ++index) {
        if (Suit[index].character_id == character_id) {
            return &Suit[index];
        }
    }
    return NULL;
}

void Suits_CollectAll() {
    Game.suit_flags = SAVE_SUIT_ALL;
    areaSuitBits = SAVE_SUIT_ALL;
}

SUIT_s *Suit_FindFromLetter(char letter) {
    for (i32 index = 0; index < 10; ++index) {
        if (Suit[index].letter == letter) {
            return &Suit[index];
        }
    }
    return NULL;
}
