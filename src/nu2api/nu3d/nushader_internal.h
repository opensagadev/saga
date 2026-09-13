#pragma once

#include "nu2api/nu3d/nushader.h"

struct GLSLTypeInfo {
    GLenum gl_type;
    u32 parameter_type;
    u32 setter_class;
    u32 element_count;
};

extern "C" const GLSLTypeInfo *GetGLSLTypeInfo(GLenum type);

bool LinkShaderProgram(u32 program);
bool ValidateShaderProgram(u32 program);
