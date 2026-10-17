#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
extern "C" {
    i32 NuSysDirClose(i32 index);
    i32 NuSysDirOpen(void);
    i32 NuSysDirRead(void);
    void NuIOS_AwardAchievement(void);
    void NuIOS_ShowAchievements(void);
}
