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
};

struct NuPostDataPort {
    i32 index;
    NuDataPortManager *manager;
    void *get() const {
        return manager->entries[index].data;
    }
    void set(void *data) {
        manager->entries[index].data = data;
    }
};

template <typename T> struct NuDataPort : NuPostDataPort {
    NuDataPort() {
        index = -1;
        manager = NULL;
    }
    ~NuDataPort() {
        if (index >= 0)
            --manager->entries[index].references;
    }
};
