#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct AIPATHCNXHELPERSYS_s;
struct AIPATHCNXHELPER_s;
struct AIPATHCNX_s;
struct WORLDINFO_s;
extern AIPATHCNXHELPER_s *AIPathCnxHelperSys_AddHelper(AIPATHCNXHELPERSYS_s *system, AIPATHCNX_s *connection, u8 direction, void *target, u8 type);
void AIPathCnxHelperSysReset(WORLDINFO_s *world, AIPATHCNXHELPERSYS_s *system);
