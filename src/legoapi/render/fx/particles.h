#pragma once

#include "nu2api/nucore/common.h"

struct WORLDINFO_s;

void AddCameraRain(WORLDINFO_s *world, i32 mode);
void Particles_Start(WORLDINFO_s *world);
void Particles_Stop(WORLDINFO_s *world);
void Particles_DumpAreaPage(void);
void Particles_LoadAreaPage(char *name);
