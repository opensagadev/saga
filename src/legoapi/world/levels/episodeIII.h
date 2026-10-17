#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct DOOR_s;
struct PART_s;
struct WORLDINFO_s;
extern PART_s *boulder_part[2];
extern i32 boulder_blowup_type;
void Boulder_Kill(PART_s *, i32);
void Boulder_Move(PART_s *, f32);
void VaderA_GoneThroughDoor(WORLDINFO_s *, DOOR_s *);
