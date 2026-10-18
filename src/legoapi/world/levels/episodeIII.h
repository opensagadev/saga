#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct AIPACKET_s;
struct AISCRIPTPROCESS_s;
struct AISYS_s;
struct DOOR_s;
struct PART_s;
struct WORLDINFO_s;
i32 Action_BoulderSection(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count, i32 first_time, float);
extern PART_s *boulder_part[2];
extern i32 boulder_blowup_type;
void Boulder_Kill(PART_s *, i32);
void Boulder_Move(PART_s *, f32);
void VaderA_GoneThroughDoor(WORLDINFO_s *, DOOR_s *);
