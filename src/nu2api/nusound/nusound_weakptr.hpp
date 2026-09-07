#pragma once

#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuthread.h"

#include <pthread.h>

#include "decomp.h"

struct NuSoundWeakPtrListNode {
  public:
    static NuCriticalSection sPtrListLock;
    static NuCriticalSection sPtrAccessLock;

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
    // sentinel begins eight bytes into the object; the two cached pointers
    // at +0x14/+0x18 address those sentinels.
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
        this->head_sentinel_next = (NuSoundWeakPtrListNode *)((u8 *)this + 8);
        this->tail_sentinel_prev = (NuSoundWeakPtrListNode *)this;
        this->tail_sentinel_next = NULL;
        this->head = (NuSoundWeakPtrListNode *)this;
        this->tail = (NuSoundWeakPtrListNode *)((u8 *)this + 8);
        this->weak_count = 0;
    }

    void Link(NuSoundWeakPtrListNode *node) {
        NuSoundWeakPtrListNode::sPtrListLock.Lock();

        // libTTapp.so 0x315320: insert immediately before the tail sentinel.
        node->prev = this->tail->prev;
        node->next = this->tail;
        this->tail->prev->next = node;
        this->tail->prev = node;
        this->weak_count++;

        NuSoundWeakPtrListNode::sPtrListLock.Unlock();
    }

    void Unlink(NuSoundWeakPtrListNode *node) {
        NuSoundWeakPtrListNode::sPtrListLock.Lock();

        node->prev->next = node->next;
        node->next->prev = node->prev;

        this->weak_count--;

        node->prev = NULL;
        node->next = NULL;

        NuSoundWeakPtrListNode::sPtrListLock.Unlock();
    }

    virtual ~NuSoundWeakPtrObj() {
        NuSoundWeakPtrListNode::sPtrListLock.Lock();

        NuSoundWeakPtrListNode *node = this->head->next;
        while (node != this->tail) {
            NuSoundWeakPtrListNode *next = node->next;
            node->Clear();
            node->prev = NULL;
            node->next = NULL;
            node = next;
        }

        this->head->next = this->tail;
        this->tail->prev = this->head;
        this->weak_count = 0;

        NuSoundWeakPtrListNode::sPtrListLock.Unlock();
    }
};

template <typename T> class NuSoundWeakPtr : public NuSoundWeakPtrListNode {
  public:
    NuSoundWeakPtrObj<T> *obj;

  public:
    NuSoundWeakPtr() : obj(NULL) {
        this->prev = NULL;
        this->next = NULL;
    }

    explicit NuSoundWeakPtr(T *ptr) : obj(NULL) {
        this->prev = NULL;
        this->next = NULL;
        this->Set(ptr);
    }

    NuSoundWeakPtr(const NuSoundWeakPtr &other) : obj(NULL) {
        this->prev = NULL;
        this->next = NULL;
        this->Set((T *)other.obj);
    }

    NuSoundWeakPtr &operator=(const NuSoundWeakPtr &other) {
        this->Set((T *)other.obj);
        return *this;
    }

    NuSoundWeakPtr(const NuSoundWeakPtr &other) : obj(NULL) {
        Set((T *)other.obj);
    }

    NuSoundWeakPtr &operator=(const NuSoundWeakPtr &other) {
        Set((T *)other.obj);
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

    void Set(T *ptr);
};
