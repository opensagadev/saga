#pragma once

#include "decomp.h"

class DefaultElist;

template <typename T, typename D = DefaultElist> class NuEListNode {
  public:
    NuEListNode<T, D> *prev;
    NuEListNode<T, D> *next;
    T *data;
};

template <typename T, typename D = DefaultElist> class NuEList {
  private:
    struct Links {
        T *prev;
        T *next;
    };

    Links begin_sentinel;
    Links end_sentinel;

    static Links *GetLinks(T *entry) {
        return reinterpret_cast<Links *>(entry);
    }

  public:
    T *begin;
    T *end;
    i32 length;

    NuEList() {
        begin_sentinel.prev = nullptr;
        begin_sentinel.next = reinterpret_cast<T *>(&end_sentinel);
        end_sentinel.prev = reinterpret_cast<T *>(&begin_sentinel);
        end_sentinel.next = nullptr;
        begin = reinterpret_cast<T *>(&begin_sentinel);
        end = reinterpret_cast<T *>(&end_sentinel);
        length = 0;
    }

    ~NuEList() {
    }

    T *Front() const {
        return GetLinks(begin)->next;
    }

    T *End() const {
        return end;
    }

    bool PushBack(T *entry) {
        Links *entry_links = GetLinks(entry);
        Links *end_links = GetLinks(end);
        T *previous = end_links->prev;
        end_links->prev = entry;
        entry_links->prev = previous;
        GetLinks(previous)->next = entry;
        entry_links->next = end;
        length++;
        return true;
    }

    void Remove(T *entry) {
        Links *entry_links = GetLinks(entry);
        T *next = entry_links->next;
        T *previous = entry_links->prev;
        if (next == nullptr && previous == nullptr) {
            return;
        }
        length--;
        if (previous != nullptr) {
            GetLinks(previous)->next = next;
        }
        if (next != nullptr) {
            GetLinks(next)->prev = previous;
        }
        entry_links->next = nullptr;
        entry_links->prev = nullptr;
    }
};

DECOMP_ASSERT(sizeof(NuEList<void>) == 0x1c, "NuEList size");
