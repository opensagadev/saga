#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct PULSESYS_s;
void Pulses_AddSfx(PULSESYS_s *system, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx_count);
void Pulses_Reset(PULSESYS_s *);
