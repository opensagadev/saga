#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct MISSIONSAVE_s;
struct MISSIONSYS_s;
extern i32 Missions_PartyAvailable(MISSIONSYS_s *);
i32 Missions_NumCompleted(MISSIONSYS_s *, MISSIONSAVE_s *, i32);
