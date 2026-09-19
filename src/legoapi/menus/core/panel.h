#pragma once

#include "nu2api/nucore/common.h"

struct WORLDINFO_s;
struct GameObject_s;
struct nuhspecial_s;

extern i32 DRAWBGLOAD;
extern u16 PowerUp_PanelYRot;
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
i32 CoinsGoToMainTotal();
