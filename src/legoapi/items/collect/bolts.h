#pragma once

#include "nu2api/nucore/fixed_width.h"

extern i32 addbolt_nosfx;

struct BOLT_s;
struct GameObject_s;
struct nuvec_s;

i32 Bolt_HitGameObjects(BOLT_s *bolt, nuvec_s *points, nuvec_s *minimum, nuvec_s *maximum, f32 radius, u8 *hit_flags);
BOLT_s *Bolt_Find(i32 type_id, nuvec_s *position, GameObject_s *owner);
void Bolt_End(BOLT_s *bolt, i32 run_callback);
