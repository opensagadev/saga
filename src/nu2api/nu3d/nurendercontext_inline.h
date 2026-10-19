#pragma once

#include <GLES2/gl2.h>

#include "nu2api/nu3d/nurendercontext.h"

extern "C" {
    // File-local copy of NuRenderContextSetZFunc that the original render
    // units compile in from this header so that callers can inline it.
    static void NuRenderContextSetZFunc_inline(i32 mode) {
        if (g_renderContext_zFunc != mode) {
            switch (mode) {
                case 0:
                    glEnable(GL_DEPTH_TEST);
                    glDepthMask(GL_TRUE);
                    glDepthFunc(GL_LEQUAL);
                    break;
                case 1:
                    glEnable(GL_DEPTH_TEST);
                    glDepthMask(GL_FALSE);
                    glDepthFunc(GL_LEQUAL);
                    break;
                case 2:
                    glDisable(GL_DEPTH_TEST);
                    glDepthMask(GL_TRUE);
                    break;
                case 3:
                    glDisable(GL_DEPTH_TEST);
                    glDepthMask(GL_FALSE);
                    break;
            }
        }
        g_renderContext_zFunc = mode;
    }
}
