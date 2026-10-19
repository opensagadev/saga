#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct LEVELDATA_s;
struct LEVELOBJECT;
struct LEVELSPLINE;
struct WORLDINFO_s;
struct nuhspecial_s;
i32 EquivalentObject_Find(WORLDINFO_s *world, nuhspecial_s *special);
void LevelObjects_InitForGame(LEVELOBJECT *tab, VARIPTR *buf, VARIPTR *buf_end, i32 a4, i32 a5);
void LevelSplines_InitForGame(LEVELSPLINE *tab);
void NewLevelFromMenu(LEVELDATA_s *, i32, i32, i32);
void PortalDoors_Update(WORLDINFO_s *world);
