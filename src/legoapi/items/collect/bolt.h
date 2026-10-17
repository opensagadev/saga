#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct BOLT_s;
struct GameObject_s;
struct PART_s;
struct nuvec_s;
i32 Bolt_AlternateFire_LSW(GameObject_s *, i32);
i32 Bolt_HitPart_LSW(BOLT_s *, PART_s *);
void Bolt_Debris_LSW(BOLT_s *, nuvec_s *, i32, nuvec_s *, i32);
