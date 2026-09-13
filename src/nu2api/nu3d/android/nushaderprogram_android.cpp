#include "decomp.h"
#include "nu2api/nu3d/nushader_internal.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nucore/nustring.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Original 0x2a56a0, 81 bytes.
bool LinkShaderProgram(u32 program) {
    glLinkProgram(program);
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    return linked != 0;
}

// Original 0x2a5700, 171 bytes.
bool ValidateShaderProgram(u32 program) {
    glValidateProgram(program);
    GLint log_length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 0) {
        char *log = static_cast<char *>(malloc(log_length));
        glGetProgramInfoLog(program, log_length, &log_length, log);
        free(log);
    }
    GLint valid;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &valid);
    return valid != 0;
}

// Original specialized bodies: 360 bytes at 0x2a5390 and 412 bytes at 0x2a5500.
static i32 GetHLSLRegisterIndex(const char *source, const char *uniform_name, bool texture) {
    char search_name[256];
    sprintf(search_name, "_%s ", uniform_name + 1);
    const char *constants = strstr(source, "//NU2API CONSTANTS :");
    const char *match = strstr(source, search_name);
    const char *attributes = strstr(source, "//NU2API ATTRIBS :");
    if (constants == NULL || match == NULL || (attributes != NULL && match >= attributes)) {
        return -1;
    }
    const char *number = match + NuStrLen(search_name);
    if (texture) {
        if (NuStrNICmp(number, "TEXUNIT", NuStrLen("TEXUNIT")) == 0) {
            number += NuStrLen("TEXUNIT");
        }
    } else if (NuToUpper(static_cast<u8>(*number)) == 'C') {
        ++number;
    }
    if (!isdigit(static_cast<unsigned char>(*number))) {
        return -1;
    }
    i32 index = NuAToI(const_cast<char *>(number));
    const char *declaration = strstr(number, uniform_name + 1);
    if (declaration == NULL || declaration <= number) {
        return -1;
    }
    for (const char *cursor = declaration - 1; *cursor != '\n'; --cursor) {
        if (NuStrNCmp(cursor, "//var ", NuStrLen("//var ")) != 0) {
            return index;
        }
        if (cursor == number) {
            break;
        }
    }
    return -1;
}

// The 16-byte type alignment supplies the trailing padding in the original 0x810-byte pool.
struct __attribute__((aligned(16))) ShaderProgramPool {
    NUSHADERPROGRAM programs[64];
    u8 occupied[8];
    i32 next;
};
DECOMP_ASSERT(sizeof(ShaderProgramPool) == 0x810, "Shader program pool ABI");
DECOMP_ASSERT(offsetof(ShaderProgramPool, occupied) == 0x800, "Shader program occupancy offset");
DECOMP_ASSERT(offsetof(ShaderProgramPool, next) == 0x808, "Shader program cursor offset");
static ShaderProgramPool programPool;
NUSHADERPROGRAM *g_currentShaderProgram = nullptr;
extern "C" {
    NUSHADERPROGRAMPARAMETER g_uniformParameterRecordStorage[1024];
}
static i32 g_uniformParameterRecordAllocator;

static void BuildRegisterIndexToUniformLocationMapping(NUSHADERPROGRAM *result, const char *vertex_source,
                                                       const char *fragment_source) {
    static char uniformName[256];
    glGetProgramiv(result->program, GL_ACTIVE_UNIFORMS, &result->parameter_count);
    if (result->parameter_count != 0) {
        result->parameters = &g_uniformParameterRecordStorage[g_uniformParameterRecordAllocator];
        g_uniformParameterRecordAllocator += result->parameter_count;
    } else {
        result->parameters = NULL;
    }
    i32 removed = 0;
    for (i32 i = 0; i < result->parameter_count; ++i) {
        GLint count;
        GLenum type;
        glGetActiveUniform(result->program, i, sizeof(uniformName), NULL, &count, &type, uniformName);
        char *array_suffix = strchr(uniformName, '[');
        if (array_suffix != NULL) {
            *array_suffix = '\0';
        }

        NUSHADERPROGRAMPARAMETER *parameter = &result->parameters[i - removed];
        i32 register_index = GetHLSLRegisterIndex(vertex_source, uniformName, false);
        u16 stage_flag = 0;
        if (register_index == -1) {
            register_index = GetHLSLRegisterIndex(fragment_source, uniformName, false);
            stage_flag = 0x8000;
        }
        parameter->register_index = stage_flag | static_cast<u16>(register_index);
        parameter->location = glGetUniformLocation(result->program, uniformName);
        const GLSLTypeInfo *type_info = GetGLSLTypeInfo(type);
        if (type_info == NULL) {
            ++removed;
        } else if (type_info->parameter_type == 4) {
            const i32 texture_unit = GetHLSLRegisterIndex(fragment_source, uniformName, true);
            if (texture_unit != -1) {
                const GLint location = glGetUniformLocation(result->program, uniformName);
                glUseProgram(result->program);
                glUniform1i(location, texture_unit);
                glUseProgram(0);
                g_boundShader = 0;
            }
            ++removed;
        } else {
            parameter->setter = type_info->setter_class;
        }
    }
    result->parameter_count -= removed;
    g_uniformParameterRecordAllocator -= removed;
}

extern "C" NUSHADERPROGRAM *NuShaderProgramCreateIOS(const char *vertex_source, const char *fragment_source) {
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    NuStrLen(vertex_source);
    char *precision = const_cast<char *>(strstr(vertex_source, "precision mediump float;"));
    if (precision != NULL) {
        memcpy(precision, "precision highp float;  ", NuStrLen("precision highp float;  "));
    }
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);
    GLint compiled = 0;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == 0) {
        glDeleteShader(vertex_shader);
    }

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    NuStrLen(fragment_source);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);
    compiled = 0;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == 0) {
        glDeleteShader(fragment_shader);
        return NULL;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    LinkShaderProgram(program);
    NuShaderObjectBindAttributeLocationsGLSL(program);
    NUSHADERPROGRAM *result = NULL;
    const i32 start = programPool.next;
    for (i32 pass = 0; pass < 2 && result == NULL; ++pass) {
        const i32 end = pass == 0 ? 64 : start;
        for (i32 slot = pass == 0 ? start : 0; slot < end; ++slot) {
            if ((programPool.occupied[slot / 8] & (1 << (slot & 7))) == 0) {
                programPool.occupied[slot / 8] |= 1 << (slot & 7);
                programPool.next = (slot + 1) % 64;
                result = &programPool.programs[slot];
                break;
            }
        }
    }
    result->vertex_shader = vertex_shader;
    result->fragment_shader = fragment_shader;
    result->program = program;
    BuildRegisterIndexToUniformLocationMapping(result, vertex_source, fragment_source);
    return result;
}
