#include "globals.h"

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
}

void Suit_GetLast(i32, i32) {
}

void Suit_GetNext(SUIT_s *) {
}

i32 Suit_GetIndex(SUIT_s *suit) {
    if (suit == &Suit[0])
        return 0;
    if (suit == &Suit[1])
        return 1;
    if (suit == &Suit[2])
        return 2;
    if (suit == &Suit[3])
        return 3;
    if (suit == &Suit[4])
        return 4;
    if (suit == &Suit[5])
        return 5;
    if (suit == &Suit[6])
        return 6;
    if (suit == &Suit[7])
        return 7;
    if (suit == &Suit[8])
        return 8;
    if (suit == &Suit[9])
        return 9;
    return -1;
}

void *Suit_GetDefault(i32) {
    return NULL;
}

void Suits_CollectAll() {
    Game.suit_flags = SAVE_SUIT_ALL;
    areaSuitBits = SAVE_SUIT_ALL;
}

void Suit_FindFromLetter(char) {
}

static __used__ void DisguiseAdjust_LSW(int, int, nuvec_s *, nuvec_s *) {
}
