#pragma once

#include "nu2api/nucore/common.h"
#include "nu2api/nucore/numemory.h"

#include <cstring>

template <typename T> class NuVector {
  public:
    T *data;
    usize capacity;
    usize length;

  public:
    NuVector() : data(nullptr), capacity(0), length(0) {
    }

    ~NuVector() {
        if (length != 0) {
            length = 0;
        }
        if (data != nullptr) {
            NU_FREE(data);
            capacity = 0;
            data = nullptr;
        }
    }

    void PushBack(const T &value) {
        if (length >= capacity) {
            resize((length + 4) & ~usize(3));
        }
        data[length++] = value;
    }

  private:
    void resize(usize new_length) {
        T *new_data = (T *)NuMemoryGet()->GetThreadMem()->_BlockReAlloc(data, new_length * sizeof(T), 4, 0x41, "",
                                                                        NUMEMORY_CATEGORY_NONE);

        if (new_data != data) {
            if (length != 0) {
                for (usize i = 0; i < length; ++i) {
                    new_data[i] = data[i];
                }
            }
            NU_FREE(data);
        }

        data = new_data;
        capacity = new_length;
    }
};
