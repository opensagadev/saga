#pragma once

#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nusound/nusound_sync.hpp"

#include <pthread.h>

#include "decomp.h"

struct NuSoundWeakPtrListNode {
  public:
    static NuSoundCriticalSection sPtrListLock;
    static NuSoundCriticalSection sPtrAccessLock;

  public:
    // struct Payload {
    NuSoundWeakPtrListNode *prev;
    NuSoundWeakPtrListNode *next;
    //} payload;

  public:
    virtual ~NuSoundWeakPtrListNode() {
    }

    virtual void Clear() = 0;
};

template <typename T> class NuSoundWeakPtrObj {
  public:
    // The original object is also the list's head sentinel.  The second
    // sentinel begins two pointer widths into the object (eight bytes on
    // Android x86); the cached pointers address those biased sentinels.
    NuSoundWeakPtrListNode *head_sentinel_prev;
    NuSoundWeakPtrListNode *head_sentinel_next;
    NuSoundWeakPtrListNode *tail_sentinel_prev;
    NuSoundWeakPtrListNode *tail_sentinel_next;
    NuSoundWeakPtrListNode *head;
    NuSoundWeakPtrListNode *tail;
    i32 weak_count;

  public:
    NuSoundWeakPtrObj() {
        this->head_sentinel_prev = NULL;
        this->head_sentinel_next = (NuSoundWeakPtrListNode *)((u8 *)this + 2 * sizeof(NuSoundWeakPtrListNode *));
        this->tail_sentinel_prev = (NuSoundWeakPtrListNode *)this;
        this->tail_sentinel_next = NULL;
        this->head = (NuSoundWeakPtrListNode *)this;
        this->tail = (NuSoundWeakPtrListNode *)((u8 *)this + 2 * sizeof(NuSoundWeakPtrListNode *));
        this->weak_count = 0;
    }

    void Link(NuSoundWeakPtrListNode *node) {
        NuSoundWeakPtrListNode::sPtrListLock.Lock();

        NuSoundWeakPtrListNode *tail = this->tail;
        NuSoundWeakPtrListNode **tail_prev = tail != NULL ? &tail->prev : NULL;
        NuSoundWeakPtrListNode *previous = tail_prev != NULL ? *tail_prev : NULL;
        if (previous != NULL) {
            *tail_prev = node;
            node->prev = previous;
            previous->next = node;
        } else {
            if (tail_prev != NULL) {
                *tail_prev = node;
            }
            node->prev = NULL;
        }
        node->next = tail;
        this->weak_count++;

        NuSoundWeakPtrListNode::sPtrListLock.Unlock();
    }

    void Unlink(NuSoundWeakPtrListNode *node) {
        NuSoundWeakPtrListNode::sPtrListLock.Lock();

        NuSoundWeakPtrListNode *next = node->next;
        NuSoundWeakPtrListNode *previous = node->prev;
        this->weak_count--;

        if (next != NULL) {
            if (previous != NULL) {
                previous->next = next;
                next->prev = previous;
            } else {
                next->prev = NULL;
            }
        } else if (previous != NULL) {
            previous->next = NULL;
        }

        node->prev = NULL;
        node->next = NULL;

        NuSoundWeakPtrListNode::sPtrListLock.Unlock();
    }

    // Removes the first linked weak pointer; the retail list helper guards the
    // successor's back link through a pointer-to-member that may be null.
    NuSoundWeakPtrListNode *PopFront() {
        NuSoundWeakPtrListNode *node = this->head->next;
        NuSoundWeakPtrListNode *next = node->next;
        NuSoundWeakPtrListNode *previous = node->prev;
        NuSoundWeakPtrListNode **next_prev = next != NULL ? &next->prev : NULL;
        if (previous != NULL) {
            if (next_prev != NULL) {
                previous->next = next;
                *next_prev = previous;
            } else {
                previous->next = NULL;
            }
        } else if (next_prev != NULL) {
            *next_prev = previous;
        }
        this->weak_count--;
        node->next = NULL;
        node->prev = NULL;
        return node;
    }

    virtual ~NuSoundWeakPtrObj();
};

template <typename T> class NuSoundWeakPtr : public NuSoundWeakPtrListNode {
  public:
    NuSoundWeakPtrObj<T> *obj;

  public:
    NuSoundWeakPtr() : obj(NULL) {
        this->prev = NULL;
        this->next = NULL;
    }

    NuSoundWeakPtr(T *ptr) : obj(NULL) {
        this->prev = NULL;
        this->next = NULL;
        Set(ptr);
    }

    NuSoundWeakPtr(const NuSoundWeakPtr &other) : obj(NULL) {
        this->prev = NULL;
        this->next = NULL;
        // The source pointer must be read while the list lock is held.  The
        // target wraps the generated weak-pointer copy in this outer lock;
        // Set() and Link() take the same recursive lock again.
        NuSoundWeakPtrListNode::sPtrListLock.Lock();
        Set((T *)other.obj);
        NuSoundWeakPtrListNode::sPtrListLock.Unlock();
    }

    NuSoundWeakPtr &operator=(const NuSoundWeakPtr &other) {
        NuSoundWeakPtrListNode::sPtrListLock.Lock();
        Set((T *)other.obj);
        NuSoundWeakPtrListNode::sPtrListLock.Unlock();
        return *this;
    }

    virtual ~NuSoundWeakPtr() {
        NuSoundWeakPtrListNode::sPtrListLock.Lock();

        if (this->obj != NULL) {
            this->obj->Unlink(this);
            this->obj = NULL;
        }

        NuSoundWeakPtrListNode::sPtrListLock.Unlock();
    }

    void Clear() {
        this->obj = NULL;
    }

    void Set(T *ptr) {
        NuSoundWeakPtrListNode::sPtrListLock.Lock();

        if (this->obj != (void *)ptr) {
            if (this->obj != NULL) {
                this->obj->Unlink(this);
            }

            if (ptr != NULL) {
                // Register this weak pointer in the target's list.
                ((NuSoundWeakPtrObj<T> *)ptr)->Link(this);
            }

            this->obj = (NuSoundWeakPtrObj<T> *)ptr;
        }

        NuSoundWeakPtrListNode::sPtrListLock.Unlock();
    }
};
