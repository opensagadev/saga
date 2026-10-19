#pragma once

#include "decomp.h"

union variptr_u;

void SpecialMoves_Configure(char *filename, variptr_u *buffer, variptr_u *end);

// Declared here for the other units that use them.
struct GameObject_s;
i32 SpecialMove_Check(GameObject_s *, GameObject_s *);
i32 SpecialMove_GetAttackerAction(i32);
i32 SpecialMove_GetVictimAction(i32);
i32 SpecialMove_ReleaseVictim(GameObject_s *);
u32 SpecialMove_GetFlags(i32 index, u32 mask);
void SpecialMove_Cancel(GameObject_s *object);
void SpecialMove_VictimCode(GameObject_s *);
