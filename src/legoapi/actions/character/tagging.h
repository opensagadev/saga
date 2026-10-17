#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct GameObject_s;
extern f32 BUCK_RIDERJUMPCLEARANCE;
extern void (*BuckStartExtraFn)(GameObject_s *);
void Buck_MoveCode(GameObject_s *, i32);
void Tag_NewTransfer(GameObject_s *, GameObject_s *);
void Tag_ResetTransfers(void);
