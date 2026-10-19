#pragma once

#include "nu2api/nucore/common.h"

struct NuDataPortManager {
    struct Entry {
        void *data;
        char name[32];
        i32 references;
    } entries[256];
    NuDataPortManager() {
        for (i32 i = 0; i < 256; ++i)
            entries[i].references = 0;
    }
    ~NuDataPortManager() {
    }
    i32 registerPort(char const *name, void *data);
    Entry &getEntry(i32 index) {
        return entries[index];
    }
};

struct NuPostDataPort {
    i32 index;
    NuDataPortManager *manager;
    void *get() const {
        return manager->getEntry(index).data;
    }
    void set(void *data) {
        manager->getEntry(index).data = data;
    }
};

template <typename T> struct NuDataPort : NuPostDataPort {
    NuDataPort() {
        index = -1;
        manager = NULL;
    }
    ~NuDataPort() {
        if (index >= 0)
            --manager->getEntry(index).references;
    }
};
