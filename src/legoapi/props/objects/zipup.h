#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct DETONATOR_s;
struct EXPLOSION;
struct GameObject_s;
struct HINT_s;
struct nuvec_s;
DETONATOR_s *Detonator_FindNearest(nuvec_s *, float, GameObject_s *);
extern EXPLOSION *Detonate(nuvec_s *position, u16 flags);
i32 ZipUps_UpdateHint(HINT_s *);
void Detonators_Reset(void);
void ZipUp_MoveCode(GameObject_s *object, i32 special_pressed);
void ZipUps_Reset(void *world_ptr, void *, void *progress_ptr);
