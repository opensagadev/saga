#pragma once

#include "nu2api/nucore/common.h"

f32 GetAspectRatio();
void InitStillRender(VARIPTR *buffer, VARIPTR buffer_end);
void NeedScreenGrab(i32 needed);
void GrabStillScreen(void);
void DrawStillScreen(i32 clear);
void DrawPauseScreenWipe(void);
void HandleStillRender(void);

// Declared here for the other units that use them.
struct Fade;
struct FadeStill;
struct FadeStillWipe;
struct FadeWipe;
struct numtl_s;
extern Fade fade;
extern FadeStill fadeStill;
extern FadeStillWipe fadeStillWipe;
extern FadeWipe fadeWipe;
extern f32 backdrop_bot_b;
extern f32 backdrop_bot_g;
extern f32 backdrop_bot_r;
extern f32 backdrop_top_b;
extern f32 backdrop_top_g;
extern f32 backdrop_top_r;
extern i32 cut_waiting_for_new_level;
extern numtl_s *ScreenFadeMtl;
extern void (*BackDrop_AlphaFn)(f32 *alpha);
extern void BackDrop_UpdateColours(i32 instant);
extern void DrawFadeScreenWipe(void);
void BackDrop_Init(char *path, VARIPTR *buf, VARIPTR *buf_end);
void CreateFadeMaterials();
void GameFog_Reset(void);
