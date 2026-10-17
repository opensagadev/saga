#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct EXPLOSION;
struct GameObject_s;
struct nuvec_s;
EXPLOSION *AddExplosion(nuvec_s *, f32, f32, GameObject_s *, i32, i32);
void ResetExplosions(void);
void UpdateExplosion_Generic(EXPLOSION *);
