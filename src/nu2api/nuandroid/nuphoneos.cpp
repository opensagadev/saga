#include "nu2api/nuandroid/nuphoneos.h"

#include "nu2api/nucore/common.h"

static PHONEEVENTCALLBACK *s_phoneOSEventCallbacks[7];

void NuPhoneOSRegisterEventCallback(i32 type, PHONEEVENTCALLBACK *callback_fn) {
    s_phoneOSEventCallbacks[type] = callback_fn;
}
