#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct APICHARACTERMODELLIST_s;
struct GameObject_s;
struct WORLDINFO_s;
struct nuhspecial_s;
struct numtx_s;
union variptr_u;
extern void CharScenes_AreaLoad(APICHARACTERMODELLIST_s *, variptr_u *, variptr_u);
nuhspecial_s *CharScene_FindHSpecial(WORLDINFO_s *world, i32 character_id);
void CharScene_Draw(WORLDINFO_s *, i32, numtx_s *, numtx_s *);
void CharScenes_Init(VARIPTR *buf, VARIPTR *buf_end);
void CharScenes_LevelDump(WORLDINFO_s *world);
void HairMovement(GameObject_s *object);
void HeadMovement(GameObject_s *object);
