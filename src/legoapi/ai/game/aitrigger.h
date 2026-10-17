#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct AISYS_s;
struct AITRIGGERSETSYS_s;
struct AITRIGGERSET_s;
struct GIZMO_s;
struct WORLDINFO_s;
i32 AITriggerSetAddTrigger(AISYS_s *, AITRIGGERSET_s *, GIZMO_s *);
void AITriggerSetSysProcess(AITRIGGERSETSYS_s *system);
void AITriggerSetSysReset(AITRIGGERSETSYS_s *);
void AITriggerSysAutoSetUp(WORLDINFO_s *, AITRIGGERSETSYS_s *);
