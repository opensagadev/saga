#pragma once

#include "nu2api/nucore/common.h"

struct WORLDINFO_s;
struct GIZMO_s;
struct GIZMOPICKUP_s;
struct SUPERCOUNTER;
struct SUPERCOUNTERPICKUP;
struct nuvec_s;

// World areas open state (module legoapi/world, areas.cpp).

void Areas_OpenAll(i32 mode);
void Areas_ConfigureResidents(VARIPTR *buffer, VARIPTR *buffer_end);
void SuperCounters_Reset(i32 area_index);
SUPERCOUNTER *SuperCounters_FindPickup(WORLDINFO_s *, GIZMO_s *, nuvec_s *, SUPERCOUNTERPICKUP **);
i32 SuperCounter_AnyCollected(SUPERCOUNTER *, WORLDINFO_s *);
void SuperCounters_FixUpGizmos(WORLDINFO_s *);
void SuperCounters_ResetProcessed(WORLDINFO_s *world);
void SuperCounter_ActivateGizmoPickup(GIZMO_s *, GIZMOPICKUP_s *);
SUPERCOUNTER *SuperCounter_FindFromNameAndLevel(char *, WORLDINFO_s *, SUPERCOUNTERPICKUP **);

extern i32 openlevels;

// Declared here for the other units that use them.
struct EXTRAMODEL;
extern void Area_Configure(i32, i32, EXTRAMODEL *, i16 *);
