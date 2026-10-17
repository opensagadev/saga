#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct WORLDINFO_s;
int SpecialSfxLoad(char *path, WORLDINFO_s *world);
void SetSpecialSfxBits(i32 *sfx_ids, i32 *sfx_count, WORLDINFO_s *world);
