#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct CABLE_s;
struct GameObject_s;
CABLE_s *GameObjIsCableTied(GameObject_s *);
extern CABLE_s cables[8];
extern f32 max_cable_length;
extern f32 tow_length;
void DestroyCable(CABLE_s *cable);
void ReleaseCable(CABLE_s *, i32);
