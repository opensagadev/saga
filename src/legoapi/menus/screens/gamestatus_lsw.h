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

// Declared here for the other units that use them.
struct STATUSPACKET_s;
struct STATUS_STAGE_s;
struct WORLDINFO_s;
extern f32 STATUS_TITLE_Y;
extern f32 icon_y;
extern f32 iconalphaoverride;
extern i32 draw_player_icons;
extern i32 status_prompt;
f32 getFinishedStatusAlpha(STATUSPACKET_s *);
i32 FinishStatusPacket_LSW(WORLDINFO_s *, STATUSPACKET_s *, i32);
i32 InitStatusScreen_LSW(WORLDINFO_s *, STATUSPACKET_s *);
void AllMiniKits_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void AllMiniKits_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void AllMiniKits_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void BonusComplete_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void BonusComplete_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void BonusComplete_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void BonusTime_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void BonusTime_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void BonusTime_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void BonusWin_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void BonusWin_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void BonusWin_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void ChallangeCash_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void ChallangeCash_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void ChallangeCash_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void Coins_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void Coins_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void Coins_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void CollectCharactersOff_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void CollectCharactersOff_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void CollectCharactersOff_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void CollectCharcters_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void CollectCharcters_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void CollectCharcters_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void DrawStatusBG_LSW(STATUSPACKET_s *);
void Exit_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void Fade_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void GoldBrick_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void GoldBrick_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void GoldBrick_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void LSW_registerStatusScreen(void);
void LevelComplete_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void LevelComplete_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void LevelComplete_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void MiniKit_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void MiniKit_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void MiniKit_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void Prompt_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void Prompt_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void RedBrick_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void RedBrick_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void RedBrick_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void ResetStatusPacket_LSW(STATUSPACKET_s *);
void Save_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void SuperStoryScore_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void SuperStoryScore_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void SuperStoryScore_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void SuperStoryTime_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void SuperStoryTime_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void SuperStoryTime_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
void TrueHero_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);
void TrueHero_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);
void TrueHero_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, f32);
