#include "nu2api/nuandroid/nuphoneos.h"

#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuthreadqueue.h"

static PHONEEVENTCALLBACK *s_phoneOSEventCallbacks[7];
typedef NuThreadQueue<NuPhoneOSMessage, 128> NuPhoneOSQueue;
static NuPhoneOSQueue s_phoneOSMessageQueue;

DECOMP_ASSERT(sizeof(NuPhoneOSQueue) == 0xe5c, "PhoneOS message queue target layout");
DECOMP_ASSERT(offsetof(NuPhoneOSQueue, write_count) == 0x50, "PhoneOS queue write counter");
DECOMP_ASSERT(offsetof(NuPhoneOSQueue, read_count) == 0x54, "PhoneOS queue read counter");
DECOMP_ASSERT(offsetof(NuPhoneOSQueue, records) == 0x58, "PhoneOS queue records");
DECOMP_ASSERT(offsetof(NuPhoneOSQueue, waiting_token) == 0xe58, "PhoneOS queue token");

i32 g_systemPauseReceived;
i32 g_systemResumeReceived;
i32 g_systemDidBecomeActiveReceived;

void NuPhoneOSRegisterEventCallback(i32 type, PHONEEVENTCALLBACK *callback_fn) {
    s_phoneOSEventCallbacks[type] = callback_fn;
}

extern "C" void NuPhoneOSMessagePost(const NuPhoneOSMessage *message, i32 nonblocking, i32 wait_until_processed) {
    if (nonblocking == 0) {
        s_phoneOSMessageQueue.free_slots.Wait();
        NuPhoneOSQueue::Record record = {*message, NuPhoneOSQueue::NO_TOKEN};
        s_phoneOSMessageQueue.records[__atomic_load_n(&s_phoneOSMessageQueue.write_count, __ATOMIC_RELAXED) & 127] =
            record;
        if (__atomic_load_n(&s_phoneOSMessageQueue.read_count, __ATOMIC_RELAXED) ==
            __atomic_load_n(&s_phoneOSMessageQueue.write_count, __ATOMIC_RELAXED)) {
            s_phoneOSMessageQueue.became_empty.TryWait();
            s_phoneOSMessageQueue.became_nonempty.TryWait();
            s_phoneOSMessageQueue.became_nonempty.Signal();
        }
        __atomic_store_n(&s_phoneOSMessageQueue.write_count,
                         __atomic_load_n(&s_phoneOSMessageQueue.write_count, __ATOMIC_RELAXED) + 1U, __ATOMIC_RELAXED);
        s_phoneOSMessageQueue.queued_items.Signal();
    } else {
        if (!s_phoneOSMessageQueue.free_slots.TryWait())
            return;
        NuPhoneOSQueue::Record record = {*message, NuPhoneOSQueue::NO_TOKEN};
        s_phoneOSMessageQueue.records[__atomic_load_n(&s_phoneOSMessageQueue.write_count, __ATOMIC_RELAXED) & 127] =
            record;
        if (__atomic_load_n(&s_phoneOSMessageQueue.read_count, __ATOMIC_RELAXED) ==
            __atomic_load_n(&s_phoneOSMessageQueue.write_count, __ATOMIC_RELAXED)) {
            s_phoneOSMessageQueue.became_empty.TryWait();
            s_phoneOSMessageQueue.became_nonempty.TryWait();
            s_phoneOSMessageQueue.became_nonempty.Signal();
        }
        __atomic_store_n(&s_phoneOSMessageQueue.write_count,
                         __atomic_load_n(&s_phoneOSMessageQueue.write_count, __ATOMIC_RELAXED) + 1U, __ATOMIC_RELAXED);
        s_phoneOSMessageQueue.queued_items.Signal();
    }
    if (wait_until_processed != 0)
        s_phoneOSMessageQueue.WaitUntilEmpty();
}

extern "C" void NuPhoneOSMessagePump(void) {
    if (g_systemPauseReceived != 0) {
        if (s_phoneOSEventCallbacks[PHONE_EVENT_PAUSE] != NULL)
            s_phoneOSEventCallbacks[PHONE_EVENT_PAUSE](NULL);
        g_systemPauseReceived = 0;
    }
    if (g_systemResumeReceived != 0) {
        if (s_phoneOSEventCallbacks[PHONE_EVENT_RESUME] != NULL)
            s_phoneOSEventCallbacks[PHONE_EVENT_RESUME](NULL);
        g_systemResumeReceived = 0;
    }
    if (g_systemDidBecomeActiveReceived != 0) {
        if (s_phoneOSEventCallbacks[PHONE_EVENT_BECOME_ACTIVE] != NULL)
            s_phoneOSEventCallbacks[PHONE_EVENT_BECOME_ACTIVE](NULL);
        g_systemDidBecomeActiveReceived = 0;
    }
    NuPhoneOSMessage message;
    while (s_phoneOSMessageQueue.queued_items.TryWait()) {
        message =
            s_phoneOSMessageQueue.records[__atomic_load_n(&s_phoneOSMessageQueue.read_count, __ATOMIC_RELAXED) & 127]
                .message;
        u32 token =
            s_phoneOSMessageQueue.records[__atomic_load_n(&s_phoneOSMessageQueue.read_count, __ATOMIC_RELAXED) & 127]
                .token;
        if (token != NuPhoneOSQueue::NO_TOKEN &&
            token == __atomic_load_n(&s_phoneOSMessageQueue.waiting_token, __ATOMIC_RELAXED))
            s_phoneOSMessageQueue.token_received.Signal();
        __atomic_store_n(&s_phoneOSMessageQueue.read_count,
                         __atomic_load_n(&s_phoneOSMessageQueue.read_count, __ATOMIC_RELAXED) + 1U, __ATOMIC_RELAXED);
        if (__atomic_load_n(&s_phoneOSMessageQueue.read_count, __ATOMIC_RELAXED) ==
            __atomic_load_n(&s_phoneOSMessageQueue.write_count, __ATOMIC_RELAXED)) {
            s_phoneOSMessageQueue.became_nonempty.TryWait();
            s_phoneOSMessageQueue.became_empty.TryWait();
            s_phoneOSMessageQueue.became_empty.Signal();
        }
        s_phoneOSMessageQueue.free_slots.Signal();
        if (s_phoneOSEventCallbacks[message.type] == NULL)
            continue;
        s_phoneOSEventCallbacks[message.type](&message.data);
    }
}
