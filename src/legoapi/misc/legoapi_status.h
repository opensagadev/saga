#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct REGISTERSTATUSPACKET_s;
struct STATUSPACKET_s;
struct STATUS_STAGE_s;
struct rtldata_s;
STATUS_STAGE_s *FindStatusStage(i32 type);
extern f32 jibberlen;
extern f32 pop_timer;
extern f32 slideseek;
extern f32 slidetime;
extern rtldata_s KitPartRTL;
f32 StatusIconsOnOff(f32 progress);
i32 AddGoldBrickMessage(STATUSPACKET_s *packet, i16 brick);
i32 UpdateNewMiniKits(STATUSPACKET_s *, STATUS_STAGE_s *stage);
i32 newCharactersCollected(STATUSPACKET_s *);
void AddStatusMiniKitParts();
void AddStatusStage(STATUSPACKET_s *packet, i32 type, i32 gold_brick_enabled);
void DrawBonusTime(STATUSPACKET_s *, f32, i32);
void DrawStatusMiniKit(float x, float y, float z, float built_scale, float new_scale, i32 count, STATUSPACKET_s *packet, float slide);
void EndOfDemo(i32);
void FinishStatusPacket(i32);
void NewStatusRumbleBuzz(i32 player, float amount, float buzz, i32 priority);
void NextStatusStage(STATUSPACKET_s *packet);
void RegisterStatusScreen(STATUS_STAGE_s *, i32 *, REGISTERSTATUSPACKET_s *);
void SetDrawGoldBrick(STATUSPACKET_s *packet, i32);
void StatusPacketReset(STATUSPACKET_s *);
void StatusStage_Reset(STATUS_STAGE_s *stage);
void Status_DrawPromptMenu(STATUSPACKET_s *, i32, f32);
