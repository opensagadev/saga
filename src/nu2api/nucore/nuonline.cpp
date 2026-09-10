#include "nu2api/nucore/nuonline.h"

extern "C" {
    // These entry points are empty in the original Android implementation.
    void NuOnlineSetPresenceMode(void) {
    }
    void NuOnlineSetDefaultPresenceMode(void) {
    }
    void NuOnlineSetProperty(void) {
    }
    void NuOnlineSetPresenceModeEx(void) {
    }
    void NuOnlineSetDefaultPresenceModeEx(void) {
    }
    void NuOnlineSetContextEx(void) {
    }
    void NuOnlineSetDefaultContextEx(void) {
    }
    void NuOnlineSetPropertyEx(void) {
    }
}

i32 NuOnlineAchievementAchieved(i32 achievement, NUONLINEACHIEVEMENTCALLBACK callback) {
    return NuOnlineAchievementAchievedPS(achievement, callback);
}

i32 NuOnlineAchievementAchievedEx(i32 player, i32 achievement, NUONLINEACHIEVEMENTCALLBACK callback) {
    return NuOnlineAchievementAchievedExPS(player, achievement, callback);
}

void NuOnlineSetContext(void) {
}

void NuOnlineSetDefaultContext(void) {
}

void NuOnlineInit(void) {
    NuOnlineInitPS();
}

i32 NuOnlineHasPlayerSignedIn(void) {
    return NuOnlineHasPlayerSignedInPS();
}

i32 NuOnlineHasPlayerSignedInEx(void) {
    return 0;
}

i32 NuOnlineHasPlayerDownloaded(u32 argument) {
    return NuOnlineHasPlayerDownloadedPS(argument);
}
