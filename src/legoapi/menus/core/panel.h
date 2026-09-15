#pragma once

#include "nu2api/nucore/common.h"

struct WORLDINFO_s;

void PanelRender(WORLDINFO_s *world);
void Panel_Clear();
void UpdateStats();
void DrawTimer(i32 time, i32 expanded, i32 reset);
f32 Panel_GetRedBrickSlideTime();
