#include "nu2api/nucore/nuonline.h"

void NuOnlineInit(void) {
    NuOnlineInitPS();
}

i32 NuOnlineHasPlayerSignedIn(void) {
    return NuOnlineHasPlayerSignedInPS();
}
