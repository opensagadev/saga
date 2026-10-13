#pragma once

#include "nu2api/nucore/common.h"
#include "nu2api/nucore/numemory.h"

class NuSoundHandle;

class NuListNodeBase {
    friend class NuSoundHandle;

    NuListNodeBase *prev;
    NuListNodeBase *next;

  public:
    NuListNodeBase(NuListNodeBase *prev, NuListNodeBase *next) : prev(prev), next(next) {
    }

    void SetPrev(NuListNodeBase *prev) {
        this->prev = prev;
    }

    void SetNext(NuListNodeBase *next) {
        this->next = next;
    }

    NuListNodeBase *GetNext() const {
        return this->next;
    }

    NuListNodeBase *GetPrev() const {
        return this->prev;
    }

    void Remove() {
        NuListNodeBase *prev = this->prev;
        NuListNodeBase *next = this->next;
        if (prev != NULL) {
            prev->next = next;
        }
        if (next != NULL) {
            next->prev = prev;
        }
    }
};

template <typename T> class NuListNode : public NuListNodeBase {
  public:
    T value;

  public:
    NuListNode(NuListNode<T> *prev, NuListNode<T> *next, T value) : NuListNodeBase(prev, next), value(value) {
    }
};

template <typename T> class NuList {
    friend class NuSoundHandle;

    NuListNodeBase start;
    NuListNodeBase end;

    NuListNodeBase *head;
    NuListNodeBase *tail;

    i32 length;

  public:
    NuList() : start(NULL, NULL), end(NULL, NULL), head(&start), tail(&end) {
        start.SetNext(&end);
        end.SetPrev(&start);
        length = 0;
    }

    ~NuList() {
        while (Head() != tail) {
            Remove(Head());
        }
    }

    void Append(NuListNode<T> *node) {
        NuListNodeBase *tail = this->tail;
        NuListNodeBase *last = tail->GetPrev();
        tail->SetPrev(node);

        node->SetPrev(last);
        last->SetNext(node);
        node->SetNext(tail);

        length++;
    }

    NuListNodeBase *Head() const {
        return this->head->GetNext();
    }

    NuListNodeBase *Tail() const {
        return this->tail;
    }

    i32 Length() const {
        return this->length;
    }

    bool Contains(T const &value) const {
        if (length == 0) {
            return false;
        }
        NuListNodeBase *last = tail->GetPrev();
        NuListNodeBase *node = Head();
        if (value == static_cast<NuListNode<T> *>(node)->value) {
            return true;
        }
        while (node != last) {
            node = node->GetNext();
            if (value == static_cast<NuListNode<T> *>(node)->value) {
                return true;
            }
        }
        return false;
    }

    void Remove(NuListNodeBase *node) {
        length--;

        node->Remove();

        NU_FREE(node);
    }

    // Callers check Length() first.  After removing a match the reference
    // resumes the scan past the following node.
    i32 RemoveValue(T const &value) {
        i32 removed = 0;
        NuListNodeBase *last = tail->GetPrev();
        NuListNodeBase *node = Head();
        for (;;) {
            if (static_cast<NuListNode<T> *>(node)->value == value) {
                NuListNodeBase *next = node->GetNext();
                if (node == last) {
                    node->Remove();
                    NU_FREE(node);
                    ++removed;
                    break;
                }
                node->Remove();
                NU_FREE(node);
                ++removed;
                node = next;
            }
            if (node == last) {
                break;
            }
            node = node->GetNext();
        }
        length -= removed;
        return removed;
    }
};
