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
    struct Links {
        NuSoundWeakPtrListNode *prev;
        NuSoundWeakPtrListNode *next;
    } start, end;

    NuSoundWeakPtrListNode *head;
    NuSoundWeakPtrListNode *tail;
    i32 weak_count;

  public:
    static Links *GetLinks(NuSoundWeakPtrListNode *node) {
        return node != NULL ? reinterpret_cast<Links *>(reinterpret_cast<char *>(node) + sizeof(void *)) : NULL;
    }

    NuSoundWeakPtrObj() {
        head = reinterpret_cast<NuSoundWeakPtrListNode *>(reinterpret_cast<char *>(&start) - sizeof(void *));
        tail = reinterpret_cast<NuSoundWeakPtrListNode *>(reinterpret_cast<char *>(&end) - sizeof(void *));
        start.prev = NULL;
        start.next = tail;
        end.prev = head;
        end.next = NULL;
        weak_count = 0;
    }

    void Link(NuSoundWeakPtrListNode *node) {
        NuSoundWeakPtrListNode::sPtrListLock.Lock();

        NuSoundWeakPtrListNode *insertion_point = tail;
        Links *tail_links = GetLinks(insertion_point);
        NuSoundWeakPtrListNode *previous = tail_links->prev;
        tail_links->prev = node;
        node->prev = previous;
        GetLinks(previous)->next = node;
        node->next = insertion_point;
        this->weak_count++;

        NuSoundWeakPtrListNode::sPtrListLock.Unlock();
    }

    void Unlink(NuSoundWeakPtrListNode *node) {
        NuSoundWeakPtrListNode::sPtrListLock.Lock();

        if (node->next != NULL || node->prev != NULL) {
            --this->weak_count;
            if (node->prev != NULL) {
                GetLinks(node->prev)->next = node->next;
            }
            if (node->next != NULL) {
                GetLinks(node->next)->prev = node->prev;
            }
            node->next = NULL;
            node->prev = NULL;
        }

        NuSoundWeakPtrListNode::sPtrListLock.Unlock();
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
