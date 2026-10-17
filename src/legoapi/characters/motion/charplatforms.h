#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct CHARPLATFORMSYS_s;
struct GameObject_s;
GameObject_s *CharPlatform_FindObjFromPlatID(CHARPLATFORMSYS_s *system, i32 platform_id);
void CharPlatforms_Reset(CHARPLATFORMSYS_s *);
