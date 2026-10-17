#pragma once

#include "nu2api/nucore/fixed_width.h"

struct nuhspecial_s;

void AddShoveObject(nuhspecial_s *special, i16 character_id);

// Declared here for the other units that use them.
struct GameObject_s;
struct WORLDINFO_s;
struct specialsfx_s;
void SetupBlowupSfx(WORLDINFO_s *world, specialsfx_s *special_sfx);
void ShoveSystemCheckGameObject(GameObject_s *object);
