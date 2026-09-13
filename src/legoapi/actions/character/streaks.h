#pragma once

#include "nu2api/nucore/common.h"

struct nuvec_s;

void InitStreaks(VARIPTR *buffer, VARIPTR end, char *path);
void ResetStreaks();
void UpdateStreaks(float elapsed);
void DrawStreaks();
void AddStreakPoints(nuvec_s *points, float duration, u32 colour, void **handle, i32 mode, void *owner);
