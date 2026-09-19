#ifndef NU2API_NUANDROID_NUPHONEOS_H
#define NU2API_NUANDROID_NUPHONEOS_H

#include "decomp.h"
#include "nu2api/nucore/common.h"

enum NUPHONE_TOUCH_ACTION {
    NUPHONE_TOUCH_DOWN = 0,
    NUPHONE_TOUCH_UP = 1,
    NUPHONE_TOUCH_MOVE = 2,
    NUPHONE_TOUCH_CANCEL = 3,
};

typedef struct NuPhoneOSMessageData {
    i32 touch_id;
    i32 x;
    i32 y;
    i32 pressure;
    NUPHONE_TOUCH_ACTION touch_action;
} NuPhoneOSMessageData;

DECOMP_ASSERT(sizeof(NuPhoneOSMessageData) == 0x14, "NuPhoneOSMessageData must remain 0x14 bytes");

typedef struct NuPhoneOSMessage {
    i32 type;
    NuPhoneOSMessageData data;
} NuPhoneOSMessage;

DECOMP_ASSERT(sizeof(NuPhoneOSMessage) == 0x18, "NuPhoneOSMessage must remain 0x18 bytes");

typedef void PHONEEVENTCALLBACK(const NuPhoneOSMessageData *);

enum {
    PHONE_EVENT_TOUCH = 1,
    PHONE_EVENT_PAUSE = 3,
    PHONE_EVENT_RESUME = 4,
    PHONE_EVENT_BECOME_ACTIVE = 6,
};

#ifdef __cplusplus
extern "C" {
#endif
    void NuPhoneOSRegisterEventCallback(i32 type, PHONEEVENTCALLBACK *callback_fn);
    void NuPhoneOSMessagePost(const NuPhoneOSMessage *message, i32 nonblocking, i32 wait_until_processed);
    void NuPhoneOSMessagePump(void);
#ifdef __cplusplus
}
#endif

#endif
