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
void SuperCounters_FindPickup(WORLDINFO_s *, GIZMO_s *, nuvec_s *, SUPERCOUNTERPICKUP **);
void SuperCounter_AnyCollected(SUPERCOUNTER *, WORLDINFO_s *);
void SuperCounters_FixUpGizmos(WORLDINFO_s *);
void SuperCounters_ResetProcessed(WORLDINFO_s *world);
void SuperCounter_ActivateGizmoPickup(GIZMO_s *, GIZMOPICKUP_s *);
void SuperCounter_FindFromNameAndLevel(char *, WORLDINFO_s *, SUPERCOUNTERPICKUP **);

extern i32 openlevels;
