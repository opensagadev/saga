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
            NuMechPtr *head = managed_links;
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

    NuMechPtr(T *value = NULL) : object(NULL), next(NULL), previous(NULL) {
        Attach(value);
    }
    NuMechPtr(const NuMechPtr &other) : object(NULL), next(NULL), previous(NULL) {
        Attach(other.object);
    }
    ~NuMechPtr() {
        Reset();
    }

    NuMechPtr &operator=(const NuMechPtr &other) {
        T *value = other.object;
        Reset();
        Attach(value);
        return *this;
    }
    T *Get() const {
        return object;
    }
    T *operator->() const {
        return object;
    }

    void Reset() {
        if (object != NULL) {
            if (next == this) {
                object->managed_links = NULL;
            } else {
                next->previous = previous;
                previous->next = next;
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
            previous = value->managed_links;
            if (previous == NULL) {
                value->managed_links = this;
                next = this;
                previous = this;
            } else {
                next = previous->next;
                previous->next = this;
                next->previous = this;
            }
            object = value;
        }
    }

    T *object;
    NuMechPtr *next;
    NuMechPtr *previous;
};
