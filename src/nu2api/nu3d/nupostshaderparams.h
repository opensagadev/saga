#pragma once

#include "nu2api/nu3d/nushader.h"

extern "C" void (*g_glConstantSetterTable[4])(u32, i32, const void *);

static inline void PostBlurSetVertexParam(nushaderprogram_s *program, u32 index, const f32 *values, i32 count) {
    for (i32 i = 0; i < program->parameter_count; ++i) {
        nushaderprogramparameter_s *parameter = &program->parameters[i];
        if (parameter->register_index == index) {
            g_glConstantSetterTable[parameter->setter](parameter->location, count / 4, values);
            break;
        }
    }
}
