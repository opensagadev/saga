#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct WORLDINFO_s;
struct minisnowtrooper_s;
struct minitrooperteam_s;
extern i32 droid_hack;
i32 TrooperShoot(WORLDINFO_s *world, minitrooperteam_s *team, minisnowtrooper_s *trooper, u16 *shot_angle, i32 team_index);
