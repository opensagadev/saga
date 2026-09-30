#pragma once

#include "decomp.h"

struct WORLDINFO_s;

struct NEWMINIPIECE_s {
    char name[8];
    i16 level;
    u8 pad[2];
};
DECOMP_ASSERT(sizeof(NEWMINIPIECE_s) == 12, "new minikit piece ABI");
DECOMP_ASSERT(offsetof(NEWMINIPIECE_s, level) == 8, "new minikit piece level offset");
extern NEWMINIPIECE_s NewMiniPiece[10];
extern u16 hub_iconang[4];
void ResetIconWibble();
void UpdateIconWibble();

void DrawStatusScreen(WORLDINFO_s *world);
void UpdateStatusScreen(WORLDINFO_s *world);
void SetBonusWinner(i32 player);
