#pragma once

#include "decomp.h"
#include "nu2api/nucore/fixed_width.h"
#include <stddef.h>

struct NuMechPtr_ManagedObject {
    virtual ~NuMechPtr_ManagedObject() {
    }
};

template <class T, i32 Tag> class NuMechPtr {
  public:
    struct ManagedBase : NuMechPtr_ManagedObject {
        NuMechPtr *managed_links;

        ManagedBase() : managed_links(NULL) {
        }
        virtual ~ManagedBase() {
            NuMechPtr *&links = managed_links;
            NuMechPtr *head = links;
            if (head != NULL) {
                while (head->next != head) {
                    NuMechPtr *link = head->next;
                    link->object = NULL;
                    head->next = link->next;
                    link->previous = NULL;
                    link->next = NULL;
                }
                head->object = NULL;
                head->previous = NULL;
                head->next = NULL;
                managed_links = NULL;
            }
        }
    };

    NuMechPtr() : object(NULL), next(NULL), previous(NULL) {
    }
    NuMechPtr(T *value) : object(NULL), next(NULL), previous(NULL) {
        Set(value);
    }
    NuMechPtr(const NuMechPtr &other) : object(NULL), next(NULL), previous(NULL) {
        Set(other.object);
    }
    ~NuMechPtr() {
        Unlink();
    }

    NuMechPtr &operator=(const NuMechPtr &other) {
        T *value = other.object;
        Unlink();
        Attach(value);
        return *this;
    }
    void Reset() {
        Unlink();
    }
    void Set(T *value) {
        Unlink();
        Attach(value);
    }
    T *Get() const {
        return object;
    }
    T *operator->() const {
        return object;
    }

    void Unlink() {
        if (object != NULL) {
            if (next == this) {
                object->managed_links = NULL;
            } else {
                NuMechPtr *previous_link = previous;
                next->previous = previous_link;
                previous_link->next = next;
                if (object->managed_links == this)
                    object->managed_links = next;
            }
            object = NULL;
            next = NULL;
            previous = NULL;
        }
    }

  private:
    void Attach(T *value) {
        if (value != NULL) {
            NuMechPtr *head = value->managed_links;
            if (head == NULL) {
                value->managed_links = this;
                next = this;
                previous = this;
            } else {
                NuMechPtr *first = head->next;
                previous = head;
                next = first;
                head->next = this;
                first->previous = this;
            }
            object = value;
        }
    }

    T *object;
    NuMechPtr *next;
    NuMechPtr *previous;
};

// The original movement globals register this wrapper's destructor.
template <class T, i32 Tag> class MechWeakPtr : public NuMechPtr<T, Tag> {
  public:
    MechWeakPtr() : NuMechPtr<T, Tag>() {
    }
    explicit MechWeakPtr(T *value) : NuMechPtr<T, Tag>(value) {
    }
    using NuMechPtr<T, Tag>::operator=;
};
