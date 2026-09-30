#pragma once

#include "nu2api/nucore/fixed_width.h"

struct ADDGAMEMSG;
struct GAMEMESSAGE_s;
struct nuvec_s;

extern ADDGAMEMSG AddGameMsg_Default;
void AddFancyMessage(char *text, f32 x, f32 y, f32 scale, f32 duration, i32 message_id, i32 colour_type);
GAMEMESSAGE_s *AddGameMsg(ADDGAMEMSG *message);
void AddGameMsgCount(nuvec_s *position, i32 count, i32 total, u8 red, u8 green, u8 blue, f32 duration);
void EndScoreMessage(GAMEMESSAGE_s *message);
i32 FindGameMsgsWithID(i32 id, i32 remove, i32 player, GAMEMESSAGE_s *exclude);
void GameMsg_DrawAdjustNewPos_CoinToTotal(GAMEMESSAGE_s *message);
void TransformGameMessages(nuvec_s *position, nuvec_s *right, nuvec_s *direction);
void DrawGameMessages();
