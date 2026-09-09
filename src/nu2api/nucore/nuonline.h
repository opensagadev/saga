#pragma once

#include "nu2api/nucore/common.h"

extern "C" {
    void NuOnlineInit(void);
    void NuOnlineInitPS(void);
    i32 NuOnlineHasPlayerSignedIn(void);
    i32 NuOnlineHasPlayerSignedInPS(void);
}
