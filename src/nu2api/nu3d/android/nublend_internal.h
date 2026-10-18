#pragma once

#include <GLES2/gl2.h>
#include "nu2api/nu3d/android/nurndr_android.h"

template <typename AlphaReference>
static inline void NuSetBlendState(u32 blend, AlphaReference alpha_reference) {
    switch (blend) {
        case 1:
            glEnable(GL_BLEND);
            glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            break;
        case 2:
            glEnable(GL_BLEND);
            glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_SRC_ALPHA, GL_ONE);
            break;
        case 3:
            glEnable(GL_BLEND);
            glBlendEquationSeparate(GL_FUNC_REVERSE_SUBTRACT, GL_FUNC_ADD);
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ONE);
            break;
        case 0:
            glDisable(GL_BLEND);
            break;
        case 10:
            glDisable(GL_BLEND);
            g_alphaTestEnabled = 1;
            g_alphaFunc = 5;
            g_alphaRef = alpha_reference() & 0xff;
            break;
        default:
            break;
    }
    g_lastAlphaBlend = blend;
    g_lastAlphaRef = alpha_reference();
}
