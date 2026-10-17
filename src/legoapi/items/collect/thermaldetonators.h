#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct GameObject_s;
struct HINT_s;
struct PART_s;
i32 PartDraw_ThermalDetonator(PART_s *part);
i32 ShinyMetal_UpdateHint(HINT_s *hint);
i32 ThermalDetonator_MoveCode(GameObject_s *object);
void PartImpact_ThermalDetonator(PART_s *part);
void PartKill_ThermalDetonator(PART_s *part, i32 reason);
void PartUpdate_ThermalDetonator(PART_s *part);
void ThermalDetonator_Throw(GameObject_s *);
