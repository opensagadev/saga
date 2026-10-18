#pragma once

#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nusound/nusound_sync.hpp"

#include <pthread.h>

#include "decomp.h"

struct NuSoundWeakPtrListNode;

// The intrusive list links are a plain base of the polymorphic node, so they
// sit after the vtable pointer and node/link conversions adjust by its width.
struct NuSoundWeakPtrListLink {
    NuSoundWeakPtrListNode *prev;
    NuSoundWeakPtrListNode *next;
};

struct NuSoundWeakPtrListNode : public NuSoundWeakPtrListLink {
  public:
    static NuSoundCriticalSection sPtrListLock;
    static NuSoundCriticalSection sPtrAccessLock;

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

        // Insert before the end sentinel.
        NuSoundWeakPtrListLink *link = node;
        NuSoundWeakPtrListLink *tail = this->tail;
        NuSoundWeakPtrListLink *previous = tail->prev;
        tail->prev = static_cast<NuSoundWeakPtrListNode *>(link);
        link->prev = static_cast<NuSoundWeakPtrListNode *>(previous);
        previous->next = node;
        link->next = static_cast<NuSoundWeakPtrListNode *>(tail);
        this->weak_count++;

        NuSoundWeakPtrListNode::sPtrListLock.Unlock();
    }

    void Unlink(NuSoundWeakPtrListNode *node) {
        NuSoundWeakPtrListNode::sPtrListLock.Lock();

        if (node->next != NULL || node->prev != NULL) {
            this->weak_count--;

            NuSoundWeakPtrListLink *next = node->next;
            NuSoundWeakPtrListLink *previous = node->prev;
            if (previous != NULL) {
                previous->next = static_cast<NuSoundWeakPtrListNode *>(next);
            }
            if (next != NULL) {
                next->prev = static_cast<NuSoundWeakPtrListNode *>(previous);
            }
            node->prev = NULL;
            node->next = NULL;
        }

        NuSoundWeakPtrListNode::sPtrListLock.Unlock();
    }

    // Removes the first linked weak pointer.
    NuSoundWeakPtrListNode *PopFront() {
        NuSoundWeakPtrListNode *node = this->head->next;
        NuSoundWeakPtrListLink *next = node->next;
        NuSoundWeakPtrListLink *previous = node->prev;
        if (previous != NULL) {
            previous->next = static_cast<NuSoundWeakPtrListNode *>(next);
        }
        if (next != NULL) {
            next->prev = static_cast<NuSoundWeakPtrListNode *>(previous);
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
        Assign((T *)other.obj);
    }

    NuSoundWeakPtr &operator=(const NuSoundWeakPtr &other) {
        Assign((T *)other.obj);
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

    __attribute__((noinline)) void Set(T *ptr);

    // Copies, assignments and some direct updates expand the pointer update
    // in place.
    void Assign(T *ptr) {
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

// Defined outside the class so it is not implicitly inline: retail calls Set
// out of line when a weak pointer is built from a raw pointer.
template <typename T> void NuSoundWeakPtr<T>::Set(T *ptr) {
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

// Builds a weak pointer whose target is registered in place (Assign) rather
// than through the out-of-line Set; used when a temporary callback pointer is
// handed straight to RequestBuffer.
template <typename T> inline NuSoundWeakPtr<T> NuSoundMakeWeakPtr(T *ptr) {
    NuSoundWeakPtr<T> pointer;
    pointer.Assign(ptr);
    return pointer;
}
