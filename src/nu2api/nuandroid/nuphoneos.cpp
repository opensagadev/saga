#include "nu2api/nuandroid/nuphoneos.h"

#include "nu2api/nucore/common.h"

static PHONEEVENTCALLBACK *s_phoneOSEventCallbacks[7];

void NuPhoneOSRegisterEventCallback(i32 type, PHONEEVENTCALLBACK *callback_fn) {
    s_phoneOSEventCallbacks[type] = callback_fn;
}

extern "C" void NuPhoneOSMessagePost(const NuPhoneOSMessage *message, i32 nonblocking, i32 wait_until_processed) {
    STUBBED();
    (void)message;
    (void)nonblocking;
    (void)wait_until_processed;
}

extern "C" void NuPhoneOSMessagePump(void) {
    STUBBED();
}
