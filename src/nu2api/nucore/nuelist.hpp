#pragma once

#include "decomp.h"
#include "nu2api/nucore/numemory.h"

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
        while (length != 0) {
            T *entry = PopFront();
            // The original destroys entries in place and returns them to the
            // thread allocator rather than going through operator delete.
            entry->~T();
            NU_FREE(entry);
        }
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

    // Unlinks the first entry without the membership check Remove performs.
    T *PopFront() {
        T *entry = Front();
        Links *entry_links = GetLinks(entry);
        T *previous = entry_links->prev;
        T *next = entry_links->next;
        if (previous != nullptr) {
            GetLinks(previous)->next = next;
        }
        if (next != nullptr) {
            GetLinks(next)->prev = previous;
        }
        length--;
        entry_links->next = nullptr;
        entry_links->prev = nullptr;
        return entry;
    }

    void Remove(T *entry) {
        Links *entry_links = GetLinks(entry);
        T *previous = entry_links->prev;
        T *next = entry_links->next;
        if (next != nullptr || previous != nullptr) {
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
    }
};

DECOMP_ASSERT(sizeof(NuEList<void>) == 0x1c, "NuEList size");

// Intrusive list variant used when an object's links do not begin at offset
// zero.  The sentinels are represented as deliberately biased T pointers, just
// like the original NuEList implementation, so applying Offset reaches the
// embedded sentinel links.
template <typename T, usize Offset> class NuEListOffset {
  private:
    struct Links {
        T *prev;
        T *next;
    };

    Links begin_sentinel;
    Links end_sentinel;

    static Links *GetLinks(T *entry) {
        return reinterpret_cast<Links *>(reinterpret_cast<u8 *>(entry) + Offset);
    }

    T *BeginSentinel() {
        return reinterpret_cast<T *>(reinterpret_cast<u8 *>(&begin_sentinel) - Offset);
    }

    T *EndSentinel() {
        return reinterpret_cast<T *>(reinterpret_cast<u8 *>(&end_sentinel) - Offset);
    }

  public:
    typedef Links Link;

    T *begin;
    T *end;
    i32 length;

    // Link-record iteration.  The original walks the embedded link records
    // (a base subobject at Offset), so converting an entry pointer to its
    // links is null-checked like a derived-to-base pointer conversion.
    static Link *LinkOf(T *entry) {
        return entry != nullptr ? GetLinks(entry) : nullptr;
    }

    static T *EntryOf(Link *link) {
        return reinterpret_cast<T *>(reinterpret_cast<u8 *>(link) - Offset);
    }

    // Null-checked links-to-entry conversion (base-to-derived static_cast).
    static T *CheckedEntryOf(Link *link) {
        return link != nullptr ? EntryOf(link) : nullptr;
    }

    static Link *NextLink(Link *link) {
        return LinkOf(link->next);
    }

    Link *FrontLink() const {
        return LinkOf(Front());
    }

    Link *EndLink() const {
        return LinkOf(end);
    }

    NuEListOffset() {
        T *begin_entry = BeginSentinel();
        T *end_entry = EndSentinel();
        begin_sentinel.prev = nullptr;
        begin_sentinel.next = end_entry;
        end_sentinel.prev = begin_entry;
        end_sentinel.next = nullptr;
        begin = begin_entry;
        end = end_entry;
        length = 0;
    }

    ~NuEListOffset() {
        while (length != 0) {
            T *entry = Front();
            Remove(entry);
            delete entry;
        }
    }

    T *Front() const {
        return GetLinks(begin)->next;
    }

    T *Back() const {
        return GetLinks(end)->prev;
    }

    T *End() const {
        return end;
    }

    bool PushBack(T *entry) {
        Link *entry_link = LinkOf(entry);
        Link *end_link = LinkOf(end);
        Link *previous_link = LinkOf(end_link->prev);
        end_link->prev = CheckedEntryOf(entry_link);
        entry_link->prev = CheckedEntryOf(previous_link);
        previous_link->next = EntryOf(entry_link);
        entry_link->next = EntryOf(end_link);
        length++;
        return true;
    }

    void Remove(T *entry) {
        Links *entry_links = GetLinks(entry);
        T *next = entry_links->next;
        if (next != nullptr) {
            T *previous = entry_links->prev;
            length--;
            Links *next_links = GetLinks(next);
            if (previous != nullptr) {
                if (next_links != nullptr) {
                    GetLinks(previous)->next = next;
                    next_links->prev = previous;
                } else {
                    GetLinks(previous)->next = nullptr;
                }
            } else if (next_links != nullptr) {
                next_links->prev = nullptr;
            }
            entry_links->next = nullptr;
            entry_links->prev = nullptr;
        } else {
            T *previous = entry_links->prev;
            if (previous != nullptr) {
                length--;
                GetLinks(previous)->next = nullptr;
                entry_links->next = nullptr;
                entry_links->prev = nullptr;
            }
        }
    }
};

DECOMP_ASSERT(sizeof(NuEListOffset<void, 0>) == 0x1c, "offset NuEList size");
