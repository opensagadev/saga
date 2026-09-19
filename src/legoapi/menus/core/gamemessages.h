#pragma once

#include "nu2api/nucore/fixed_width.h"

struct ADDGAMEMSG;
struct GAMEMESSAGE_s;
struct nuvec_s;

extern ADDGAMEMSG AddGameMsg_Default;
GAMEMESSAGE_s *AddGameMsg(ADDGAMEMSG *message);
void EndScoreMessage(GAMEMESSAGE_s *message);
i32 FindGameMsgsWithID(i32 id, i32 remove, i32 player, GAMEMESSAGE_s *exclude);
void GameMsg_DrawAdjustNewPos_CoinToTotal(GAMEMESSAGE_s *message);
void TransformGameMessages(nuvec_s *position, nuvec_s *right, nuvec_s *direction);
void DrawGameMessages();
