#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct APIOBJECT_s;
struct GIZTURRETSYS_s;
struct GIZTURRET_s;
struct GameObject_s;
GIZTURRET_s *GizTurret_FindByController(GIZTURRETSYS_s *, GameObject_s &);
GIZTURRET_s *GizTurret_FindByName(GIZTURRETSYS_s *, char *);
void GizTurrets_OpponentSelection(GIZTURRETSYS_s *, i32, APIOBJECT_s **, i32, APIOBJECT_s **);
