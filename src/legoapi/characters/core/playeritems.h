#pragma once

#include "nu2api/nucore/fixed_width.h"

i32 ReleaseHearts();

// Declared here for the other units that use them.
struct PLAYERITEM_s;
i32 PlayerItem_GotAmmo(PLAYERITEM_s *);
