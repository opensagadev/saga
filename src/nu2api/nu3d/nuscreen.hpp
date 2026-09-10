#pragma once

#include "nu2api/nucore/common.h"

class NuScreen {

    static NuScreen *ms_instance;

    f32 width, height;

  public:
    NuScreen();
    ~NuScreen();

    static bool Exists();
    static NuScreen *Get() {
        return ms_instance;
    }
    f32 GetWidth() const {
        return width;
    }
    f32 GetHeight() const {
        return height;
    }
    static void Create();

    void Destroy();
    void SetSceeenDimensions(f32 width, f32 height);
};
