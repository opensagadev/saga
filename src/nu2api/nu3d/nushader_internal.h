#pragma once

#include "nu2api/nu3d/nu2api_nu3d_types.h"
#include "nu2api/nu3d/nushader.h"

struct GLSLTypeInfo {
    GLenum gl_type;
    u32 parameter_type;
    u32 setter_class;
    u32 element_count;
};

extern "C" {
    extern i32 g_semanticMaskCount;
    extern NUSHADERUSAGEMASK g_semanticMasks[128];
}

extern "C" const GLSLTypeInfo *GetGLSLTypeInfo(GLenum type);
NUSHADERUSAGEMASK *GetUsageMask(NUSHADERUSAGEMASK *mask);
void NuShaderObjectInit(nushaderobject_s *object, const nushaderobjectkey_s *key, i32 param, u32 vertex_shader,
                        u32 fragment_shader, eSHADERVERSION version);

bool LinkShaderProgram(u32 program);
bool ValidateShaderProgram(u32 program);
