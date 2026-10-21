#pragma once

#include <pthread.h>

#include "nu2api/nucore/nuthread.h"

typedef pthread_mutex_t NuSoundMutex;
typedef pthread_cond_t NuSoundCondition;

static inline void NuSoundMutexInit(NuSoundMutex *mutex) {
    pthread_mutex_init(mutex, NULL);
}
static inline void NuSoundMutexInitRecursive(NuSoundMutex *mutex) {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}
static inline void NuSoundMutexDestroy(NuSoundMutex *mutex) {
    pthread_mutex_destroy(mutex);
}
static inline void NuSoundMutexLock(NuSoundMutex *mutex) {
    pthread_mutex_lock(mutex);
}
static inline void NuSoundMutexUnlock(NuSoundMutex *mutex) {
    pthread_mutex_unlock(mutex);
}
static inline void NuSoundConditionInit(NuSoundCondition *condition) {
    pthread_cond_init(condition, NULL);
}
static inline void NuSoundConditionDestroy(NuSoundCondition *condition) {
    pthread_cond_destroy(condition);
}
static inline void NuSoundConditionWait(NuSoundCondition *condition, NuSoundMutex *mutex) {
    pthread_cond_wait(condition, mutex);
}
static inline void NuSoundConditionSignal(NuSoundCondition *condition) {
    pthread_cond_signal(condition);
}
static inline void NuSoundConditionBroadcast(NuSoundCondition *condition) {
    pthread_cond_broadcast(condition);
}

class NuSoundCriticalSection {
  public:
    NuCriticalSection critical_section;

    NuSoundCriticalSection() : critical_section("NuSoundCriticalSection::mCriticalSection") {
    }

    void Lock() {
        critical_section.Lock();
    }

    void Unlock() {
        critical_section.Unlock();
    }
};
