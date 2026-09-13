#include "decomp.h"
#include "nu2api/nucore/nuonline.h"

extern "C" {
    // These entry points are empty in the original Android implementation.
    void NuOnlineSetPresenceMode(void) {
        STUBBED();
    }
    void NuOnlineSetDefaultPresenceMode(void) {
        STUBBED();
    }
    void NuOnlineSetProperty(void) {
        STUBBED();
    }
    void NuOnlineSetPresenceModeEx(void) {
        STUBBED();
    }
    void NuOnlineSetDefaultPresenceModeEx(void) {
        STUBBED();
    }
    void NuOnlineSetContextEx(void) {
        STUBBED();
    }
    void NuOnlineSetDefaultContextEx(void) {
        STUBBED();
    }
    void NuOnlineSetPropertyEx(void) {
        STUBBED();
    }
    void NuOnlineSignInPlayer(void) {
        STUBBED();
    }
}

i32 NuOnlineAchievementAchieved(i32 achievement, NUONLINEACHIEVEMENTCALLBACK callback) {
    return NuOnlineAchievementAchievedPS(achievement, callback);
}

i32 NuOnlineAchievementAchievedEx(i32 player, i32 achievement, NUONLINEACHIEVEMENTCALLBACK callback) {
    return NuOnlineAchievementAchievedExPS(player, achievement, callback);
}

void NuOnlineSetContext(void) {
    STUBBED();
}

void NuOnlineSetDefaultContext(void) {
    STUBBED();
}

void NuOnlineInit(void) {
    NuOnlineInitPS();
}

i32 NuOnlineHasPlayerSignedIn(void) {
    return NuOnlineHasPlayerSignedInPS();
}

i32 NuOnlineHasPlayerSignedInEx(void) {
    STUBBED();
    return 0;
}

i32 NuOnlineHasPlayerDownloaded(u32 argument) {
    return NuOnlineHasPlayerDownloadedPS(argument);
}
