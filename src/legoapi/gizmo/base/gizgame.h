#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct BOLT_s;
struct GIZMOSYS_s;
struct nuvec_s;
i32 GizmoSys_BoltHit(GIZMOSYS_s *, void *, BOLT_s *, nuvec_s *, nuvec_s *, nuvec_s *, f32, u8 *);
