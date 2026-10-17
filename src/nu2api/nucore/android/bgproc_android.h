#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
i32 bgprocIsFreezing();
void bgProcAbortAll();
void bgSuspendMain(i32);
void bgprocFreeze();
void bgprocUnFreeze();
