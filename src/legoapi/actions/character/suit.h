#pragma once

#include "decomp.h"

struct SUIT_s;

void Suits_Init();
i32 Suit_GetIndex(SUIT_s *suit);
SUIT_s *Suit_FindFromLetter(char letter);
SUIT_s *Suit_GetNext(SUIT_s *suit);
SUIT_s *Suit_GetLast(i32 character_id, i32 require_owned);
SUIT_s *Suit_GetDefault(i32 character_id);
void Suits_CollectAll();
