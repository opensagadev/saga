#pragma once

#include "nu2api/nucore/common.h"

struct WORLDINFO_s;
struct GameObject_s;
struct nuhspecial_s;
struct HINT_s;

extern i32 DRAWBGLOAD;
extern u16 PowerUp_PanelYRot[2];
extern f32 PowerUp_PanelPosMul[2];
extern f32 POWERUPOBJSIZE;
extern f32 REDBRICKPOSX;
extern f32 REDBRICKPOSY;
extern f32 REDBRICKPOS2X;
extern f32 REDBRICKPOS2Y;
extern f32 PANEL_REDBRICKSCALE;
extern i32 Arcade_Points[2];
extern f32 BOSSICONY;
extern i32 FPSDISPLAY;
extern i32 ShowPlayerCoordinate;

void PanelRender(WORLDINFO_s *world);
void Panel_Clear();
void UpdateStats();
void DrawTimer(i32 time, i32 expanded, i32 reset);
void InitPanel(i32 mode);
void DrawPanel();
void DrawBossHitPoints(GameObject_s *object);
void SpeederChase_DrawMeleeTargets(i16 *character_ids, char *dimmed, i32 count);
void DrawMeleeTargetsRows(i16 *targets, char *icons, f32 *values, i32 count);
void DrawMeleeTargetsNumber(i16 *targets, unsigned char *icons, i32 count, unsigned char state, nuhspecial_s *special);
void DrawMeleeTargets(i16 *targets, char *icons, f32 *values, i32 count);
i32 Tag_UpdateHint(HINT_s *hint);
char *GameObj_GetName(i32 model, GameObject_s *object, char *buffer);
f32 PowerUp_GetPanelY(i32 player_index);
void DrawAutoSaveIcon();
void DrawPlayerIconPrompts(i32, i32, f32, i32, i32, i32, i32, i32, i32, f32, i32, i32, i32, i32);
void CoinTotal_Draw(i32 total, f32 y, f32 scale, i32 remember_positions, f32 icon_phase, i32 red, i32 green, i32 blue);
void DrawSuperStoryTime(f32 x, f32 timer, f32 target, i32 flags, i32 draw_icon);
void DrawBuildUpBar(f32 x, f32 y, i32 amount, i32 maximum, f32 scale, f32 width, f32 alpha, u16 angle);
void DrawBonusScore(f32 y, i32 player0_active, i32 player1_active, f32 alpha, i32 *scores);
i32 InDoubleScoreZone(GameObject_s *object);
f32 DoubleScoreAlpha();
void DrawInDoubleScoreZone(f32 time);
i32 CoinsGoToMainTotal();
