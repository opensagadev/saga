#pragma once

#include "nu2api/nucore/fixed_width.h"

struct TERRSET;
struct TERRPICKUPSET;
struct nugscn_s;

i32 ReadTerrain(unsigned char *base_path, i32 first_group, i16 **buffer, TERRSET *terrain);
i32 ReadTerrainPickup(unsigned char *base_path, i16 **buffer, TERRPICKUPSET *terrain);
void ReadInstanceIDs(i32, nugscn_s *);
extern "C" void CrashDataPtr(void);
