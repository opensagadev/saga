#include "decomp.h"
#include "nu2api/nu3d/nushader_internal.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuvector.hpp"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The 16-byte type alignment supplies the trailing padding in the original 0x810-byte pool.
struct __attribute__((aligned(16))) ShaderProgramPool {
    NUSHADERPROGRAM programs[64];
    i8 occupied[8];
    i32 next;

    ShaderProgramPool() : next(0) {
        memset(occupied, 0, sizeof(occupied));
    }
};
DECOMP_ASSERT(sizeof(ShaderProgramPool) == 0x810, "Shader program pool ABI");
DECOMP_ASSERT(offsetof(ShaderProgramPool, occupied) == 0x800, "Shader program occupancy offset");
DECOMP_ASSERT(offsetof(ShaderProgramPool, next) == 0x808, "Shader program cursor offset");
static ShaderProgramPool programPool;

// The reference constructs the program pool before the VuVec constants.
#include "nu2api/nucore/nuvuvec.hpp"

// Original Pair type identity only; its element layout is not used here.
// These two named empty vectors require only the canonical scalar lifetime.
template <typename First, typename Second> struct Pair;

static NuVector<Pair<const u8 *, u32>> vertexShaders;
static NuVector<Pair<const u8 *, u32>> pixelShaders;

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
static inline __attribute__((always_inline)) i32 GetHLSLRegisterIndexImpl(const char *source, const char *uniform_name,
                                                                          bool texture) {
    char search_name[256];
    sprintf(search_name, "_%s ", uniform_name + 1);
    const char *constants = strstr(source, "//NU2API CONSTANTS :");
    const char *match = strstr(source, search_name);
    const char *attributes = strstr(source, "//NU2API ATTRIBS :");
    if (match == NULL || constants == NULL || (attributes != NULL && match >= attributes)) {
        return -1;
    }
    const char *number = match + NuStrLen(search_name);
    if (texture) {
        const i32 token_length = NuStrLen("TEXUNIT");
        if (NuStrNICmp(number, "TEXUNIT", token_length) == 0) {
            number += token_length;
        }
    } else if (NuToUpper(static_cast<u8>(*number)) == 'C') {
        ++number;
    }
    // Bionic's NDEBUG inline isdigit, including its EOF test.
    const i32 digit = *number;
#ifdef __ANDROID__
    if (digit == -1 || ((_ctype_ + 1)[static_cast<u8>(digit)] & 0x04) == 0) {
        return -1;
    }
#else
    if (digit < '0' || digit > '9') {
        return -1;
    }
#endif
    i32 index = NuAToI(const_cast<char *>(number));
    const char *declaration = strstr(number, uniform_name + 1);
    if (declaration <= number || declaration == NULL) {
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

// BuildRegisterIndexToUniformLocationMapping's static uniform name buffer;
// both retail constprop clones read it directly.
static char build_uniform_name[256]
    __asm__("_ZZL42BuildRegisterIndexToUniformLocationMappingP17nushaderprogram_sPKcS2_E11uniformName");

static __attribute__((noinline, noclone)) i32 GetHLSLConstantRegisterIndex(const char *source)
    __asm__("_ZL20GetHLSLRegisterIndexPKcS0_b.constprop.25");
static i32 GetHLSLConstantRegisterIndex(const char *source) {
    return GetHLSLRegisterIndexImpl(source, build_uniform_name, false);
}

static __attribute__((noinline, noclone)) i32 GetHLSLUniformRegisterIndex(const char *source, bool texture)
    __asm__("_ZL20GetHLSLRegisterIndexPKcS0_b.constprop.26");
static i32 GetHLSLUniformRegisterIndex(const char *source, bool texture) {
    return GetHLSLRegisterIndexImpl(source, build_uniform_name, texture);
}

NUSHADERPROGRAM *g_currentShaderProgram = nullptr;
extern "C" {
    NUSHADERPROGRAMPARAMETER g_uniformParameterRecordStorage[1024];
}
static i32 g_uniformParameterRecordAllocator;

template <typename First, typename Second> struct Pair {
    First first;
    Second second;
};
static void BuildRegisterIndexToUniformLocationMapping(NUSHADERPROGRAM *result, const char *vertex_source,
                                                       const char *fragment_source) {
    char *uniformName = build_uniform_name;
    glGetProgramiv(result->program, GL_ACTIVE_UNIFORMS, &result->parameter_count);
    if (result->parameter_count != 0) {
        result->parameters = &g_uniformParameterRecordStorage[g_uniformParameterRecordAllocator];
        g_uniformParameterRecordAllocator += result->parameter_count;
    } else {
        result->parameters = NULL;
    }
    i32 removed = 0;
    auto result_parameter_count = result->parameter_count;
    for (i32 i = 0; i < result_parameter_count; ++i) {
        GLint count;
        GLenum type;
        glGetActiveUniform(result->program, i, sizeof(build_uniform_name), NULL, &count, &type, uniformName);
        char *array_suffix = strchr(uniformName, '[');
        if (array_suffix != NULL) {
            *array_suffix = '\0';
        }

        NUSHADERPROGRAMPARAMETER *parameter = &result->parameters[i - removed];
        i32 register_index = GetHLSLConstantRegisterIndex(vertex_source);
        u16 stage_flag = 0;
        if (register_index == -1) {
            register_index = GetHLSLConstantRegisterIndex(fragment_source);
            stage_flag = 0x8000;
        }
        parameter->register_index = stage_flag | static_cast<u16>(register_index);
        parameter->location = glGetUniformLocation(result->program, uniformName);
        const GLSLTypeInfo *type_info = GetGLSLTypeInfo(type);
        if (type_info == NULL) {
            ++removed;
        } else if (type_info->parameter_type == 4) {
            const i32 texture_unit = GetHLSLUniformRegisterIndex(fragment_source, true);
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
    const char *vertex_text = vertex_source;
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    NuStrLen(vertex_text);
    char *precision = const_cast<char *>(strstr(vertex_text, "precision mediump float;"));
    if (precision != NULL) {
        memcpy(precision, "precision highp float;  ", NuStrLen("precision highp float;  "));
    }
    glShaderSource(vertex_shader, 1, &vertex_text, NULL);
    glCompileShader(vertex_shader);
    GLint compiled = 0;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == 0) {
        glDeleteShader(vertex_shader);
    }

    const char *fragment_text = fragment_source;
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    NuStrLen(fragment_text);
    glShaderSource(fragment_shader, 1, &fragment_text, NULL);
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
    if (__builtin_expect(start <= 63, 1)) {
        i32 slot = start;
        do {
            if ((programPool.occupied[slot >> 3] & (1 << (slot & 7))) == 0) {
                programPool.occupied[slot >> 3] |= 1 << (slot & 7);
                programPool.next = (slot + 1) % 64;
                result = &programPool.programs[slot];
                break;
            }
        } while (++slot != 64);
    }
    if (result == NULL && start > 0) {
        i32 slot = 0;
        do {
            if ((programPool.occupied[slot >> 3] & (1 << (slot & 7))) == 0) {
                programPool.occupied[slot >> 3] |= 1 << (slot & 7);
                programPool.next = (slot + 1) % 64;
                result = &programPool.programs[slot];
                break;
            }
        } while (++slot != start);
    }
    result->vertex_shader = vertex_shader;
    result->fragment_shader = fragment_shader;
    result->program = program;
    BuildRegisterIndexToUniformLocationMapping(result, vertex_source, fragment_source);
    return result;
}
