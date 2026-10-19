// Original nurndr_android.c renderer backend and display-list callbacks.
//
// This is the iOS/Android counterpart to the PS2/PICA display-list
// consumer.  The game thread builds display lists (nudlist.cpp) that
// are later drained on the render thread; each list item is a callback
// into this TU:
//
//   NuIOSDLGeom2DCallback                        original 0x293ad6
//   NuIOS_BindVertexAttributesImmediate           original 0x2939fe
//   NuIOS_BindVertexAttributesImmediateOverride   original 0x293a65
//   NuIOS_BindVertexAttributesInternal            original 0x293841
//
// Material callbacks and their file-local shader/refraction state live in
// numtl_android.cpp, matching the original material translation unit.

#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/nurendercontext_inline.h"

#include <GLES2/gl2.h>
#include <string.h>

#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/android/nutex_android.h"
#include "nu2api/nu3d/android/nudlist_callbacks.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nushader.h"
#include "nu2api/nu3d/nushader_plain.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nuvec4.h"

// ---------------------------------------------------------------------------
// Renderer binding cache. Other render-context state lives in
// nurendercontext.cpp, and material state in numtl_android.cpp.
// ---------------------------------------------------------------------------

numtl_s *g_boundMaterial = nullptr;
void *g_boundLightPacket = nullptr;
NuVertexFormatPS *g_boundVertexFormat = NULL;
u32 g_activeAttributes = 0;
u32 g_boundShader = 0;

// The original helper at 0x293168 updates the shared renderer cache.
static void NuIOSBindVAO(u32 vao) {
    if (vao != g_lastBoundVAO) {
        g_lastBoundVAO = vao;
    }
}

static inline i32 NuApiFrameCount() {
    // Original 0x29c90e reads the counter at nuapi + 0x3c.
    return nuapi.frame_count;
}

static inline usize ptrToUsize(const void *p) {
    return reinterpret_cast<usize>(p);
}

__attribute__((weak)) GLenum NuIOS_PlatformVertexAttributeType(GLenum type) {
    return type;
}

__attribute__((weak)) isize NuIOS_PlatformPrepareImmediateVertexData(isize data_address, usize) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return data_address;
}

// ---------------------------------------------------------------------------
extern "C" {
    // original 0x2931b9, 303 bytes.
    static void NuRenderContextSetWorld(NUMTX *world) {
        NUMTX transforms[3];
        transforms[0] = *world;
        NuMtxMulH(&transforms[1], world, reinterpret_cast<NUMTX *>(g_renderContext_viewProj));
        NuMtxMul(&transforms[2], world, reinterpret_cast<NUMTX *>(g_renderContext_view));
        NuShaderManagerSetElementsfv(0x52, 0, 3, reinterpret_cast<const f32 *>(transforms));
        NuShaderManagerSetfv(0x3c, reinterpret_cast<const f32 *>(world));
    }

    // original 0x2932e8, 79 bytes. The render-stream matrix is transposed in place.
    static void NuRenderContextSetWorld_transpose(NUMTX *world) {
        NuMtxTranspose(world, world);
        NuShaderManagerSetElementsfv_transpose(0x3c, 0, 1, reinterpret_cast<const f32 *>(world));
    }
}

extern "C" {
    // Original 0x293337, 76 bytes.
    static void NuRenderContextSetKTint(f32 *values) {
        *reinterpret_cast<NUVEC4 *>(g_renderContext_kTint) = *reinterpret_cast<NUVEC4 *>(values);
        NuShaderManagerSetfv(0x44, values);
    }

    // Original 0x293383, 22 bytes.
    static NUVEC4 *NuRenderContextGetKTint(void) {
        return reinterpret_cast<NUVEC4 *>(g_renderContext_kTint);
    }

    // Original 0x293399, 24 bytes.
    static numtl_s *NuRenderContextGetMaterialInUse(void) {
        return g_renderContext_materialInUse;
    }

    // Original 0x293488, 620 bytes. Negation precedes unsigned-to-float conversion.
    static void NuIOS_GetAlphaTestParameters(f32 parameters[2]) {
        if (g_alphaTestEnabled != 0) {
            switch (g_alphaFunc) {
                case 5:
                    parameters[0] = 1.0f;
                    parameters[1] = static_cast<f32>(g_alphaRef) * (1.0f / 255.0f);
                    break;
                case 3:
                    parameters[0] = -1.0f;
                    parameters[1] = static_cast<f32>(-g_alphaRef) * (1.0f / 255.0f);
                    break;
                case 6:
                    parameters[0] = 1.0f;
                    parameters[1] = static_cast<f32>(g_alphaRef) * (1.0f / 255.0f) + (1.0f / 255.0f);
                    break;
                case 2:
                    parameters[0] = -1.0f;
                    parameters[1] = static_cast<f32>(-g_alphaRef) * (1.0f / 255.0f) - (1.0f / 255.0f) > 0.0f
                                        ? static_cast<f32>(-g_alphaRef) * (1.0f / 255.0f) - (1.0f / 255.0f)
                                        : 0.0f;
                    break;
                default:
                    parameters[0] = 0.0f;
                    parameters[1] = -1.0f;
                    break;
            }
        } else {
            parameters[0] = 0.0f;
            parameters[1] = -1.0f;
        }
    }
}

// Original 0x2936f4, 163 bytes.
extern "C" void NuShaderProgramSetVertexParamfv(NUSHADERPROGRAM *program, u32 register_index, const f32 *values,
                                                i32 component_count) {
    for (i32 i = 0; i < program->parameter_count; ++i) {
        NUSHADERPROGRAMPARAMETER *parameter = &program->parameters[i];
        if (parameter->register_index == register_index) {
            g_glConstantSetterTable[parameter->setter](parameter->location, (component_count + 3) / 4, values);
            break;
        }
    }
}

// Original 0x293797, 170 bytes.
extern "C" void NuShaderProgramSetFragmentParamfv(NUSHADERPROGRAM *program, u32 register_index, const f32 *values,
                                                  i32 component_count) {
    register_index |= 0x8000;
    for (i32 i = 0; i < program->parameter_count; ++i) {
        NUSHADERPROGRAMPARAMETER *parameter = &program->parameters[i];
        if (parameter->register_index == register_index) {
            g_glConstantSetterTable[parameter->setter](parameter->location, (component_count + 3) / 4, values);
            break;
        }
    }
}

// Vertex attribute binding — original 0x293841 / 0x2939fe / 0x293a65.
// ---------------------------------------------------------------------------

// Original attribute word array layout (leading dword = active mask,
// then 6 dwords per location):
//   [+0] mask
//   [+1] GL type         (e.g. GL_FLOAT)
//   [+2] component count (1..4)
//   [+3] normalized flag (GL_TRUE/GL_FALSE)
//   [+4] unused / padding
//   [+5] byte offset from vertex base
//   [+6] stride (bytes between vertices)
struct VertexAttribRecord {
    u32 gl_type;
    i32 comp_count;
    u8 normalized;
    u8 reserved[3];
    u32 pad;
    u32 byte_offset;
    u32 stride;
};

struct VertexAttribFormat {
    u32 mask;
    VertexAttribRecord attributes[1];
};

extern "C" {
    // Original 0x293841, 307 bytes.
    static void NuIOS_BindVertexAttributesInternal(isize dataAddr, i32 baseVertex, const u32 *fmtWords, u32 mask) {
        i32 loc = 0;
        mask &= fmtWords[0];
        u32 toDisable = g_activeAttributes & ~mask;
        u32 toEnable = ~g_activeAttributes & mask;
        g_activeAttributes = mask;
        do {
            if (mask & 1) {
                const VertexAttribRecord *rec =
                    &reinterpret_cast<const VertexAttribFormat *>(fmtWords)->attributes[loc];
                if (toEnable & 1) {
                    glEnableVertexAttribArray(loc);
                }
#if defined(__ANDROID__)
                glVertexAttribPointer(
                    loc, rec->comp_count, rec->gl_type, rec->normalized, rec->stride,
                    reinterpret_cast<const void *>(dataAddr + (rec->byte_offset + rec->stride * baseVertex)));
#else
                glVertexAttribPointer(
                    loc, rec->comp_count, NuIOS_PlatformVertexAttributeType(static_cast<GLenum>(rec->gl_type)),
                    rec->normalized, rec->stride,
                    reinterpret_cast<const void *>(dataAddr + (rec->byte_offset + rec->stride * baseVertex)));
#endif
            } else if (toDisable & 1) {
                glDisableVertexAttribArray(loc);
            }

            ++loc;
            mask >>= 1;
            toEnable >>= 1;
        } while ((mask | toEnable | (toDisable >>= 1)) != 0);
    }
}

// Original 0x2938f8: bind the current vertex format from a vertex buffer.
extern "C" {
    static void NuIOS_BindVertexAttributes(isize, i32 baseVertex) {
        NuIOS_BindVertexAttributesInternal(0, baseVertex, reinterpret_cast<const u32 *>(g_boundVertexFormat),
                                           *reinterpret_cast<const u32 *>(g_boundVertexFormat));
    }

    // Original 0x2939bb: override layout, retaining the bound attribute mask.
    static void NuIOS_BindVertexAttributesOverrideDataLayout(usize, const u32 *format) {
        NuIOS_BindVertexAttributesInternal(0, 0, format, *reinterpret_cast<const u32 *>(g_boundVertexFormat));
    }

    // Original 0x2939fe: bind immediate data using the current vertex format.
    static void NuIOS_BindVertexAttributesImmediate(isize, isize dataAddr) {
        NuIOSBindVAO(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        NuIOS_BindVertexAttributesInternal(dataAddr, 0, reinterpret_cast<const u32 *>(g_boundVertexFormat),
                                           *reinterpret_cast<const u32 *>(g_boundVertexFormat));
    }

    // original 0x293a65 — bind immediate data with an explicit record layout.
    // The first argument is unused; the active mask still comes from the current
    // bound vertex format, while `fmt` supplies the attribute records.
    static void NuIOS_BindVertexAttributesImmediateOverrideDataLayout(isize, isize dataAddr, NuVertexFormatPS *fmt) {
        NuIOSBindVAO(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        NuIOS_BindVertexAttributesInternal(dataAddr, 0, reinterpret_cast<const u32 *>(fmt),
                                           *reinterpret_cast<const u32 *>(g_boundVertexFormat));
    }
}

// Original 0x293ac7: the platform error hook is a no-op in this build.
i32 NuCheckGLErrorsFL(const char *, i32) {
    return 0;
}

// Original 0x293ad1, 5 bytes: deliberately empty on this platform.
static void Nu360SetObjectShadowFactor(f32) {
}

// original 0x293ad6 — 2D geometry callback.  Binds the 2D vertex format
// and issues the draw for the geom item built by nurndr_plain's Prim2D.
void NuIOSDLGeom2DCallback(void *arg) {
    struct Geom2DItem {
        u32 prim_type;
        u32 pad0;
        u16 pad1;
        u16 vertex_count; // at +0xa
        u32 pad2;
        u8 vertices[0]; // tightly packed PrimVertexRaw array
    };

    Geom2DItem *geom = (Geom2DItem *)arg;
    u32 flags = geom->pad2;
    isize vertices = (isize)geom->vertices;
    NUSHADEROBJECT *shader;

    if (geom->vertex_count == 0) {
        return;
    }
    shader = NuShaderManagerGetShaderById(g_LastMtl->shader_desc.shader_id);
    if (shader == NULL || shader->glsl.program == 0) {
        return;
    }
    NuIOSBindVAO(0);
    NuShaderObjectGLSLSetupMaterial(shader, g_LastMtl);
    switch (geom->prim_type) {
        case 4:
            NuIOS_BindVertexAttributesImmediateOverrideDataLayout(0, vertices, g_nuPrimVertexFormat);
            glDrawArrays(GL_TRIANGLES, 0, geom->vertex_count);
            break;
        case 3:
            NuIOS_BindVertexAttributesImmediateOverrideDataLayout(0, vertices, g_nuPrimVertexFormat);
            glDrawArrays(GL_LINE_STRIP, 0, geom->vertex_count);
            break;
        case 1:
            NuIOS_BindVertexAttributesImmediateOverrideDataLayout(0, vertices, g_nuPrimVertexFormat);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, geom->vertex_count);
            break;
        case 2:
            NuIOS_BindVertexAttributesImmediateOverrideDataLayout(0, vertices, g_nuPrimVertexFormat);
            glDrawArrays(GL_LINES, 0, geom->vertex_count);
            break;
        case 0:
            NuIOS_BindVertexAttributesImmediateOverrideDataLayout(0, vertices, g_nuPrimVertexFormat);
            glDrawArrays(GL_TRIANGLES, 0, geom->vertex_count);
            break;
        default:
            break;
    }
    (void)flags;
}

void DumpAttributeBindings() {
    for (GLint index = 0; index < 16; ++index) {
        GLint enabled;
        glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
        if (enabled != 0) {
            GLint buffer, size, stride, type, normalized;
            void *pointer;
            glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &buffer);
            glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
            glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
            glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
            glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &normalized);
            glGetVertexAttribPointerv(index, GL_VERTEX_ATTRIB_ARRAY_POINTER, &pointer);
        }
    }
}

void MultilineDump(char const *text) {
    // Logging was stripped from this build, but its temporary line splitting
    // remains. Despite the original const signature, callers supply writable,
    // nonempty strings. Searching starts after the first character.
    char *newline = const_cast<char *>(text);
    const char *line = newline;
    while ((newline = strchr(newline + 1, '\n')) != NULL) {
        *newline = '\0';
        *newline = '\n';
        line = newline + 1;
    }
    (void)line;
}

void DumpShaderSource(u32 shader) {
    GLint length;
    glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &length);
    char *source = static_cast<char *>(NU_ALLOC(length, 4, 1, "", 0));
    glGetShaderSource(shader, length, NULL, source);
    u32 key = 0;
    for (u32 index = 0; index < 2; ++index) {
        NUSHADEROBJECT *object = NuShaderManagerGetShaderById(g_LastMtl->shader_desc.shader_ids[index]);
        if (object != NULL && (object->glsl.vertex_shader == shader || object->glsl.fragment_shader == shader)) {
            key = object->glsl.base.key;
            break;
        }
    }
    // The key was part of the stripped diagnostic heading.
    (void)key;
    MultilineDump(source);
    NU_FREE(source);
}

void DumpProgramSource(u32 program) {
    GLuint shaders[2];
    GLsizei count;
    glGetAttachedShaders(program, 2, &count, shaders);
    for (GLsizei index = 0; index < count; ++index) {
        DumpShaderSource(shaders[index]);
    }
}

void DumpShaderAttributes(u32 program) {
    static char attributeName[256];
    GLint count;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
    for (GLint index = 0; index < count; ++index) {
        GLint size;
        GLenum type;
        glGetActiveAttrib(program, index, sizeof(attributeName), NULL, &size, &type, attributeName);
        GLint location = glGetAttribLocation(program, attributeName);
        char *array_suffix = strchr(attributeName, '[');
        if (array_suffix != NULL) {
            *array_suffix = '\0';
        }
        (void)location;
    }
}

// The original 0x2940a6 helper and its counter belong to this VAO family.
static u32 g_vaoRecordCount;

// Retail stores 2,048 twenty-byte records; the format pointer stays native
// width on host builds. The three unsigned values are opaque lookup keys.
struct NuVAORecord {
    u32 first_key;
    u32 second_key;
    NuVertexFormatPS *format;
    u32 third_key;
    i32 vao;
};
DECOMP_ASSERT(sizeof(NuVAORecord) == 20, "NuVAORecord target layout");
static NuVAORecord g_vaoRecords[2048];

void NuIOS_ResetVAODuplicateFinder() {
    g_vaoRecordCount = 0;
}

// Original 0x2940c0. Retail only records keys and returns the existing
// handle; it does not generate a GL object or clear reused handles here.
static i32 NuIOS_GetOrCreateVAO(u32 first, u32 second, u32 third, NuVertexFormatPS *format) {
    u32 i = 0;
    for (i = 0; i < g_vaoRecordCount; ++i) {
        if (g_vaoRecords[i].first_key == first && g_vaoRecords[i].second_key == second &&
            g_vaoRecords[i].format == format && g_vaoRecords[i].third_key == third) {
            return g_vaoRecords[i].vao;
        }
    }
    g_vaoRecords[i].first_key = first;
    g_vaoRecords[i].second_key = second;
    g_vaoRecords[i].format = format;
    g_vaoRecords[i].third_key = third;
    ++g_vaoRecordCount;
    return g_vaoRecords[i].vao;
}

// original 0x294233 — records the material's vertex format on static geometry
// while a scene is being fixed up.
void NuIOSDLPreWarmGeomCallback(void *arg) {
    NUDISPLAYLISTGEOM *geom = static_cast<NUDISPLAYLISTGEOM *>(arg);
    NUSHADEROBJECT *shader;
    isize immediate_vertices;

    immediate_vertices = reinterpret_cast<isize>(arg);
    immediate_vertices += sizeof(NUDISPLAYLISTGEOM);

    if (g_LastMtl != NULL && g_LastMtl->shader_desc.blend_op2 != 0xff) {
        shader = NuShaderManagerGetShaderById(g_LastMtl->shader_desc.shader_id);
        if (shader != NULL && shader->glsl.program != 0) {
            NuShaderObjectGLSLSetupTextureStates(shader, g_LastMtl);
            NuIOSBindVAO(0);
            switch (geom->primitive_type) {
                case 6:
                    if (geom->immediate == 0) {
                        geom->vertex_decl = g_boundVertexFormat;
                    }
                    break;
                case 0:
                case 1:
                case 2:
                case 3:
                case 5:
                case 0x32:
                    break;
                default:
                    return;
            }
        }
    }
    (void)immediate_vertices;
}

// original 0x29430d — 3D geometry callback
void NuIOSDLGeomCallback(void *arg) {
    auto *geom = static_cast<NUDISPLAYLISTGEOM *>(arg);
    i32 primitive_count = geom->index_count;
    i32 vertex_count = 0;
    NUSHADEROBJECT *shader;
    usize vertex_format;
    isize immediate_vertices = reinterpret_cast<isize>(arg);
    immediate_vertices += sizeof(NUDISPLAYLISTGEOM);
    shader = NuShaderManagerGetCurrentShader();
    if (shader != NULL && shader->glsl.program != 0) {

        NuShaderObjectGLSLSetupMaterial(shader, g_LastMtl);
        switch (geom->primitive_type) {
            case 6:
                vertex_count = primitive_count + 2;
                if (geom->immediate != 0) {
                    if (geom->dynamic_vertex_data != nullptr) {
                        NuIOSBindVAO(0);
                        glBindBuffer(GL_ARRAY_BUFFER, geom->vertex_format);
                        glBufferData(GL_ARRAY_BUFFER, geom->vertex_count * geom->vertex_stride, nullptr,
                                     GL_DYNAMIC_DRAW);
                        glBufferData(GL_ARRAY_BUFFER, geom->vertex_count * geom->vertex_stride,
                                     geom->dynamic_vertex_data, GL_DYNAMIC_DRAW);
                        NuIOS_BindVertexAttributes(0, 0);
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom->index_buffer);
                        glDrawElements(GL_TRIANGLE_STRIP, vertex_count, GL_UNSIGNED_SHORT,
                                       (const void *)(usize)(geom->first_index * 2));
                    } else {
                        NuIOSBindVAO(0);
#ifdef __EMSCRIPTEN__
                        // WebGL requires buffer-backed attributes instead of client arrays.
                        NuIOS_BindVertexAttributes(NuIOS_PlatformPrepareImmediateVertexData(
                                                       geom->vertex_buffer + geom->base_vertex * geom->vertex_stride,
                                                       geom->vertex_count * geom->vertex_stride),
                                                   0);
#else
                        NuIOS_BindVertexAttributesImmediate(0, geom->base_vertex * geom->vertex_stride +
                                                                   geom->vertex_buffer);
#endif
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom->index_buffer);
                        glDrawElements(GL_TRIANGLE_STRIP, vertex_count, GL_UNSIGNED_SHORT,
                                       (const void *)(usize)(geom->first_index * 2));
                    }
                } else {
                    vertex_format = geom->vertex_format;
                    NuIOS_SetVertexFormat(vertex_format);
                    NuIOSBindVAO(0);
                    glBindBuffer(GL_ARRAY_BUFFER, geom->vertex_buffer);
                    NuIOS_BindVertexAttributes(0, geom->base_vertex);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom->index_buffer);
                    glDrawElements(GL_TRIANGLE_STRIP, vertex_count, GL_UNSIGNED_SHORT,
                                   (const void *)(usize)(geom->first_index * 2));
                }
                break;
            case 0:
                vertex_count = geom->vertex_count;
                NuIOS_BindVertexAttributesImmediateOverrideDataLayout(0, immediate_vertices, g_nuPrimVertexFormat);
                glDrawArrays(GL_TRIANGLES, 0, vertex_count);
                break;
            case 1:
                vertex_count = geom->vertex_count;
                NuIOS_BindVertexAttributesImmediateOverrideDataLayout(0, immediate_vertices, g_nuPrimVertexFormat);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, vertex_count);
                break;
            case 2:
                vertex_count = geom->vertex_count;
                NuIOS_BindVertexAttributesImmediateOverrideDataLayout(0, immediate_vertices, g_nuPrimVertexFormat);
                glDrawArrays(GL_LINES, 0, vertex_count);
                break;
            case 3:
                vertex_count = geom->vertex_count;
                NuIOS_BindVertexAttributesImmediateOverrideDataLayout(0, immediate_vertices, g_nuPrimVertexFormat);
                glDrawArrays(GL_LINE_STRIP, 0, vertex_count);
                break;
            case 5:
                vertex_count = geom->vertex_count;
                NuIOS_BindVertexAttributesImmediateOverrideDataLayout(0, immediate_vertices, g_nuPrimVertexFormat);
                glDrawArrays(GL_TRIANGLE_FAN, 0, vertex_count);
                break;
            case 0x32:
                vertex_count = geom->vertex_count;
                NuIOS_BindVertexAttributesImmediateOverrideDataLayout(0, immediate_vertices, g_nuPrimVertexFormat);
                glDrawArrays(GL_POINTS, 0, vertex_count);
                break;
            default:
                break;
        }
        NuIOSBindVAO(0);
    }
}

// Original 0x294764. The skin packet begins with the number of palette
// matrices followed by their contiguous 4x4 values.
void NuIOSDLSkinMtxCallback(void *arg) {
    i32 matrix_count;
    NUSHADEROBJECT *shader;
    VARIPTR data;

    data.void_ptr = arg;
    matrix_count = *data.u32_ptr++;
    shader = NuShaderManagerGetCurrentShader();
    if (shader != NULL) {
        NuShaderObjectSetElementsfv(shader, 0x5a, 0, matrix_count * 4, data.f32_ptr);
    }
}

// original 0x2947cc, 258 bytes — installs a display-list world transform and applies
// the per-instance opacity to the current tint.
void NuIOSDLTransformCallback(void *arg) {
    auto *world = static_cast<NUMTX *>(arg);
    NUVEC4 tint = *NuRenderContextGetKTint();
    const f32 opacity = world->m33;
    const f32 shadow_factor = world->m23;

    if (opacity < 1.0f) {
        tint.w *= opacity;
        NuRenderContextSetZFunc_inline(1);
        NuShaderManagerSetfv(0x44, &tint.x);
    } else {
        numtl_s *material = NuRenderContextGetMaterialInUse();
        NuRenderContextSetZFunc_inline(material->attribs.z_mode);
        NuShaderManagerSetfv(0x44, &tint.x);
    }
    Nu360SetObjectShadowFactor(shadow_factor);

    world->m33 = 1.0f;
    world->m23 = 0.0f;
    NuRenderContextSetWorld(world);
    world->m33 = opacity;
    world->m23 = shadow_factor;
}

// Original 0x2948ce: publish the deferred world matrix without the
// per-instance opacity and shadow-factor words embedded in the packet.
void NuIOSDLDeferredTransformCallback(void *arg) {
    auto *world = static_cast<NUMTX *>(arg);
    const f32 opacity = world->m33;
    const f32 shadow_factor = world->m23;
    world->m33 = 1.0f;
    world->m23 = 0.0f;
    NuRenderContextSetWorld(world);
    world->m33 = opacity;
    world->m23 = shadow_factor;
}

// original 0x294935, 258 bytes — dynamic special transforms are stored transposed in
// the render stream. Restore the ordinary world matrix before publishing it
// to the shader state.
void NuIOSDLTransformParamsCallback(void *arg) {
    auto *stream_matrix = static_cast<NUMTX *>(arg);
    NUVEC4 tint = *NuRenderContextGetKTint();
    const f32 opacity = stream_matrix->m33;
    const f32 shadow_factor = stream_matrix->m32;
    numtl_s *material = NuRenderContextGetMaterialInUse();

    if (opacity < 1.0f) {
        tint.w *= opacity;
        NuRenderContextSetZFunc_inline(1);
        NuShaderManagerSetfv(0x44, &tint.x);
    } else {
        NuRenderContextSetZFunc_inline(material->attribs.z_mode);
        NuShaderManagerSetfv(0x44, &tint.x);
    }
    Nu360SetObjectShadowFactor(shadow_factor);

    stream_matrix->m33 = 1.0f;
    stream_matrix->m32 = 0.0f;
    NuRenderContextSetWorld_transpose(stream_matrix);
    stream_matrix->m33 = opacity;
    stream_matrix->m32 = shadow_factor;
}

// Original 0x294a37: the deferred-params packet stores its shadow factor
// in m32 and requires the transposing world-state setter.
void NuIOSDLDeferredTransformParamsCallback(void *arg) {
    auto *stream_matrix = static_cast<NUMTX *>(arg);
    const f32 opacity = stream_matrix->m33;
    const f32 shadow_factor = stream_matrix->m32;
    stream_matrix->m33 = 1.0f;
    stream_matrix->m32 = 0.0f;
    NuRenderContextSetWorld_transpose(stream_matrix);
    stream_matrix->m33 = opacity;
    stream_matrix->m32 = shadow_factor;
}

// Original 0x294a9e, 628 bytes — matrices and constants consumed by FaceOn_vx.
void NuIOSDLFaceOnTransformCallback(void *arg) {
    f32 opacity = 1.0f;
    VARIPTR packet;
    packet.void_ptr = arg;
    f32 values[4];
    values[0] = static_cast<NUMTX *>(packet.void_ptr)->m30;
    values[1] = static_cast<NUMTX *>(packet.void_ptr)->m31;
    values[2] = static_cast<NUMTX *>(packet.void_ptr)->m32;
    values[3] = 1.0f;
    static_cast<NUMTX *>(packet.void_ptr)->m30 = 0.0f;
    static_cast<NUMTX *>(packet.void_ptr)->m31 = 0.0f;
    static_cast<NUMTX *>(packet.void_ptr)->m32 = 0.0f;
    NUSHADERPROGRAM *program = g_currentShaderProgram;
    if (program != nullptr) {
        opacity = static_cast<NUMTX *>(packet.void_ptr)->m33;
        static_cast<NUMTX *>(packet.void_ptr)->m33 = 1.0f;

        NuShaderProgramSetVertexParamfv(program, 0x50, static_cast<const f32 *>(packet.void_ptr), 16);
        packet.char_ptr += sizeof(NUMTX);
        NuShaderProgramSetVertexParamfv(program, 0x59, values, 4);
        values[0] = *packet.f32_ptr++;
        values[1] = 1.0f;
        values[2] = values[3] = 0.0f;
        NuShaderProgramSetVertexParamfv(program, 0x54, values, 4);
        NuShaderProgramSetVertexParamfv(program, 0x55, static_cast<const f32 *>(packet.void_ptr), 16);
        NuShaderProgramSetVertexParamfv(program, 0, g_renderContext_viewProj, 16);
        NuShaderProgramSetVertexParamfv(program, 0xc, static_cast<const f32 *>(packet.void_ptr), 16);
        f32 alpha_test[2];
        NuIOS_GetAlphaTestParameters(alpha_test);
        NuShaderProgramSetFragmentParamfv(program, 0x70, alpha_test, 2);
        NUVEC4 tint = *NuRenderContextGetKTint();
        if (opacity < 1.0f) {
            tint.w *= opacity;
            NuRenderContextSetZFunc_inline(1);
            NuShaderProgramSetVertexParamfv(program, 0x28, &tint.x, 4);
        } else {
            numtl_s *material = NuRenderContextGetMaterialInUse();
            NuRenderContextSetZFunc_inline(material->attribs.z_mode);
            NuShaderProgramSetVertexParamfv(program, 0x28, &tint.x, 4);
        }
    }
}

// Original 0x294d12, 129 bytes. Two header words precede the group data.
void NuIOSDLVertexGroupsCallback(void *arg) {
    const i32 max_groups = 32;
    VARIPTR packet;
    packet.void_ptr = arg;
    i32 group_count = *packet.u32_ptr++;
    i32 flags = *packet.u32_ptr++;
    const f32 *values = static_cast<const f32 *>(packet.void_ptr);
    i32 vector_count = (group_count + 3) / 4;
    NuShaderManagerSetElementsfv(0x51, 0, vector_count, values);
}

// Original 0x294d93. The packet stores a count followed by up to eight vec4
// vertex-offset entries for semantic 0x50.
void NuIOSDLVertexOffsetsCallback(void *arg) {
    const i32 max_offsets = 8;
    i32 count;
    f32 *offsets;
    VARIPTR data;

    data.void_ptr = arg;
    count = (i32)*data.u32_ptr > max_offsets ? max_offsets : (i32)*data.u32_ptr;
    data.u32_ptr++;
    offsets = data.f32_ptr;
    NuShaderManagerSetElementsfv(0x50, 0, count, offsets);
}

// original 0x294dfe, 692 bytes — installs the light packet produced by
// RndrStateBuildLightState into the shader semantic state.
void NuIOSDLLightsCallback(void *arg) {
    g_boundLightPacket = arg;
    auto *lights = static_cast<NULIGHTSTATE *>(arg);
    NuShaderManagerSetfv(0x35, reinterpret_cast<const f32 *>(&lights->ambient_intensity));
    NuShaderManagerSetfv(0x39, reinterpret_cast<const f32 *>(&lights->light_direction[0]));
    NuShaderManagerSetfv(0x3a, reinterpret_cast<const f32 *>(&lights->light_direction[1]));
    NuShaderManagerSetfv(0x3b, reinterpret_cast<const f32 *>(&lights->light_direction[2]));
    NuShaderManagerSetfv(0x36, reinterpret_cast<const f32 *>(&lights->light_intensity[0]));
    NuShaderManagerSetfv(0x37, reinterpret_cast<const f32 *>(&lights->light_intensity[1]));
    NuShaderManagerSetfv(0x38, reinterpret_cast<const f32 *>(&lights->light_intensity[2]));
    NuShaderManagerSetfv(0x4b, reinterpret_cast<const f32 *>(&lights->specular_mtx));

    NUVEC4 average_direction;
    NuVec4Add(&average_direction, &lights->light_direction[0], &lights->light_direction[1]);
    NuVec4Add(&average_direction, &average_direction, &lights->light_direction[2]);
    NuVecNorm(reinterpret_cast<NUVEC *>(&average_direction), reinterpret_cast<NUVEC *>(&average_direction));
    average_direction.w = 1.0f;
    NuShaderManagerSetfv(0x4e, &average_direction.x);

    NUVEC4 average_colour;
    average_colour.x =
        lights->light_intensity[0].r > (lights->light_intensity[1].r > lights->light_intensity[2].r
                                            ? lights->light_intensity[1].r
                                            : lights->light_intensity[2].r)
            ? lights->light_intensity[0].r
            : (lights->light_intensity[1].r > lights->light_intensity[2].r ? lights->light_intensity[1].r
                                                                           : lights->light_intensity[2].r);
    average_colour.y =
        lights->light_intensity[0].g > (lights->light_intensity[1].g > lights->light_intensity[2].g
                                            ? lights->light_intensity[1].g
                                            : lights->light_intensity[2].g)
            ? lights->light_intensity[0].g
            : (lights->light_intensity[1].g > lights->light_intensity[2].g ? lights->light_intensity[1].g
                                                                           : lights->light_intensity[2].g);
    average_colour.z =
        lights->light_intensity[0].b > (lights->light_intensity[1].b > lights->light_intensity[2].b
                                            ? lights->light_intensity[1].b
                                            : lights->light_intensity[2].b)
            ? lights->light_intensity[0].b
            : (lights->light_intensity[1].b > lights->light_intensity[2].b ? lights->light_intensity[1].b
                                                                           : lights->light_intensity[2].b);
    average_colour.w = 1.0f;
    NuShaderManagerSetfv(0x4d, &average_colour.x);
    NuShaderManagerSetfv(0x57, &lights->specular_intensity.x);
}

void NuIOSDLDeferredMtlCallback(void *) {
    STUBBED();
}

extern "C" void NuRndrPspDraw(void) {
}

i32 rndr_blend_shape_deformer_wt_cnt = 0x3f00;
i32 rndr_blend_shape_deformer_wt_ptrs_cnt = 0x800;
static f32 rndr_blend_shape_deformer_wts[0x4000][0x21];
static f32 *rndr_blend_shape_deformer_wt_ptrs[0x800];

extern "C" f32 *NuRndrCreateBlendShapeDeformerWeightsArray(i32 count) {
    i32 size = (count + 0x20) * sizeof(f32);
    rndr_blend_shape_deformer_wt_cnt -= size;
    if (rndr_blend_shape_deformer_wt_cnt < 0) {
        return NULL;
    }
    f32 *weights = rndr_blend_shape_deformer_wts[rndr_blend_shape_deformer_wt_cnt];
    memset(weights, 0, size);
    return weights;
}

f32 **NuRndrCreateBlendShapeDWAPointers(i32 count) {
    rndr_blend_shape_deformer_wt_ptrs_cnt -= count;
    if (rndr_blend_shape_deformer_wt_cnt < 0) {
        return NULL;
    }
    f32 **pointers = rndr_blend_shape_deformer_wt_ptrs + rndr_blend_shape_deformer_wt_ptrs_cnt;
    return pointers;
}

void NuIOSDLKonstCallback(void *arg) {
    NuRenderContextSetKTint(static_cast<f32 *>(arg));
}

// Original 0x2951b0. Publish the packed fog colour and range to the shader.
void NuIOSDLFogCallback(void *arg) {
    const NUFOGSTATE *fog = static_cast<const NUFOGSTATE *>(arg);
    if (fog->enabled != 0) {
        const f32 fog_colour[4] = {
            static_cast<f32>(static_cast<i32>(fog->colour) & 0xff) / 255.0f,
            static_cast<f32>((static_cast<i32>(fog->colour) >> 8) & 0xff) / 255.0f,
            static_cast<f32>((static_cast<i32>(fog->colour) >> 16) & 0xff) / 255.0f,
            static_cast<f32>(static_cast<i32>(fog->colour >> 24)) / 255.0f,
        };
        const f32 fog_params[4] = {
            fog->near_distance,
            fog->far_distance,
            fog->far_distance - fog->near_distance,
            fog->density,
        };
        NuShaderManagerSetfv(0x47, fog_colour);
        NuShaderManagerSetfv(0x48, fog_params);
    } else {
        const f32 fog_params[4] = {100000.0f, 0.0f, 100000.0f, 0.0f};
        NuShaderManagerSetfv(0x48, fog_params);
    }
}

extern "C" i32 NuRndrSetFxMtx(NUMTX *matrix) {
    return 1;
}

extern "C" void NuWaterOverride(void) {
}

void NuRndrFlush(i32) {
    STUBBED();
}

extern "C" void NuRndrShadowDirCol(const NUVEC *direction, u32 colour, f32 near_distance, f32 far_distance) {
}

extern "C" i32 NuRndrSetBlendData(void) {
    STUBBED();
    return 0;
}

extern "C" void NuLightFogX(f32 near_distance, f32 far_distance, u32 colour, f32, f32, i32, f32 density) {
    NuRndrStateSetFogEnabled(1);
    NuRndrStateSetFogState(near_distance, far_distance, colour, density);
}

// original 0x295393, 141 bytes — every face-on entry is expanded to two triangles in
// the scene vertex buffer.
void NuIOSDLFaceOnCallback(void *arg) {
    if (arg != nullptr) {
        auto *packet = static_cast<NuFaceOnDrawPacket *>(arg);
        NuIOSBindVAO(0);
        glBindBuffer(GL_ARRAY_BUFFER, packet->vertex_buffer);
        NuIOS_BindVertexAttributesOverrideDataLayout(0, reinterpret_cast<const u32 *>(g_nuFaceOnVertexFormat));
        glDrawArrays(GL_TRIANGLES, packet->first_vertex, packet->face_count * 6);
    } else {
        return;
    }
}

// Original 0x295420 -- legacy packet containing three texture ids.
void NuIOSDLLightmapOld(void *arg) {
    f32 shader_offset[3] = {0.0f, 0.0f, 0.0f};
    const i32 *texture_ids = static_cast<const i32 *>(arg);
    i32 index;
    NUNATIVETEX *texture;

    for (index = 0; index < 3; index++) {
        texture = NuTexGetNative(texture_ids[index] > 0 ? texture_ids[index] : 1);
        glActiveTexture(GL_TEXTURE0 + index);
        g_currentTexUnit = index;
        glBindTexture(GL_TEXTURE_2D, texture->platform.gl_tex != 0 ? texture->platform.gl_tex : g_whiteTexture);
    }
    NuShaderManagerSetfv(0x58, shader_offset);
}

// Original 0x2954f0 -- legacy three-lightmap packet followed by a UV offset.
void NuIOSDLLightmapOffsetOld(void *arg) {
    i32 index;
    const i32 *texture_ids = static_cast<const i32 *>(arg);
    const f32 *offset = reinterpret_cast<const f32 *>(static_cast<const i32 *>(arg) + 3);
    NUNATIVETEX *texture;
    NUVEC shader_offset;

    for (index = 0; index < 3; index++) {
        texture = NuTexGetNative(texture_ids[index] > 0 ? texture_ids[index] : 1);
        glActiveTexture(GL_TEXTURE0 + index);
        g_currentTexUnit = index;
        glBindTexture(GL_TEXTURE_2D, texture->platform.gl_tex != 0 ? texture->platform.gl_tex : g_whiteTexture);
    }
    shader_offset.x = offset[0];
    shader_offset.y = -offset[1];
    shader_offset.z = 0.0f;
    NuShaderManagerSetfv(0x58, &shader_offset.x);
}

// Original 0x2955ee -- lightmap display-list packet. Mode 1 installs one
// lightmap; mode 2 walks the packet's three lightmap ids. The latter selects
// texture unit zero for each entry in the original binary.
void NuIOSDLLightmap(void *arg) {
    struct LightmapPacket {
        i32 mode;
        i32 texture_id;
        i32 texture_ids[3];
        f32 offset_u;
        f32 offset_v;
    };

    i32 index;
    LightmapPacket *packet = static_cast<LightmapPacket *>(arg);
    NUNATIVETEX *texture;
    NUNATIVETEX *layer_texture;
    NUVEC shader_offset;

    if (packet->mode == 1) {
        texture = NuTexGetNative(packet->texture_id > 0 ? packet->texture_id : 1);
        glActiveTexture(GL_TEXTURE0);
        g_currentTexUnit = 0;
        glBindTexture(GL_TEXTURE_2D, texture->platform.gl_tex != 0 ? texture->platform.gl_tex : g_whiteTexture);
        shader_offset.x = packet->offset_u;
        shader_offset.y = -packet->offset_v;
        shader_offset.z = 0.0f;
        NuShaderManagerSetfv(0x58, &shader_offset.x);
    } else if (packet->mode == 2) {
        for (index = 0; index < 3; index++) {
            layer_texture = NuTexGetNative(packet->texture_ids[index] > 0 ? packet->texture_ids[index] : 1);
            glActiveTexture(GL_TEXTURE0);
            g_currentTexUnit = 0;
            glBindTexture(GL_TEXTURE_2D,
                          layer_texture->platform.gl_tex != 0 ? layer_texture->platform.gl_tex : g_whiteTexture);
        }
        shader_offset.x = packet->offset_u;
        shader_offset.y = -packet->offset_v;
        shader_offset.z = 0.0f;
        NuShaderManagerSetfv(0x58, &shader_offset.x);
    }
}

extern "C" void NuPolyShadowInit(void) {
}

extern "C" {
    i32 NuRndrShadowCnt = 0;
    NURND_SHADOW_s NuRndrShadPolDat[128] = {};

    void NuRndrAddShadow(NUVEC *position, f32 radius, i32 opacity, i32 x_rotation, i32 y_rotation, i32 z_rotation) {
        i32 clipped = NuCameraClipTestPoints(position, 1, NULL);
        if (clipped == 0 && NuRndrShadowCnt <= 127) {
            NuRndrShadPolDat[NuRndrShadowCnt].position = *position;
            NuRndrShadPolDat[NuRndrShadowCnt].radius = radius;
            NuRndrShadPolDat[NuRndrShadowCnt].opacity = opacity;
            NuRndrShadPolDat[NuRndrShadowCnt].x_rotation = x_rotation;
            NuRndrShadPolDat[NuRndrShadowCnt].y_rotation = y_rotation;
            NuRndrShadPolDat[NuRndrShadowCnt].z_rotation = z_rotation;
            ++NuRndrShadowCnt;
        }
    }

    void NuRndrShadPolys(numtl_s *material) {
        static NURND_VERTEX3D vtx[4];
        if (NuRndrShadowCnt != 0) {
            for (i32 index = 0; index < NuRndrShadowCnt; ++index) {
                i32 i = index;
                NUVEC position = NuRndrShadPolDat[i].position;
                f32 radius = NuRndrShadPolDat[i].radius;
                i32 colour = 0xff0000ff;
                vtx[0].colour = colour;
                vtx[1].colour = colour;
                vtx[2].colour = colour;
                vtx[3].colour = colour;
                NUVEC a;
                a.x = -radius;
                a.y = 0.0f;
                a.z = radius;
                NUVEC b;
                b.x = radius;
                b.y = 0.0f;
                b.z = radius;
                if (NuRndrShadPolDat[i].y_rotation || NuRndrShadPolDat[i].z_rotation ||
                    NuRndrShadPolDat[i].x_rotation) {
                    NUMTX matrix __attribute__((aligned(16)));
                    NuMtxSetIdentity(&matrix);
                    if (NuRndrShadPolDat[i].y_rotation)
                        NuMtxRotateY(&matrix, (i16)NuRndrShadPolDat[i].y_rotation);
                    if (NuRndrShadPolDat[i].z_rotation)
                        NuMtxRotateZ(&matrix, (i16)NuRndrShadPolDat[i].z_rotation);
                    if (NuRndrShadPolDat[i].x_rotation)
                        NuMtxRotateX(&matrix, (i16)NuRndrShadPolDat[i].x_rotation);
                    NuVecMtxRotate(&a, &a, &matrix);
                    NuVecMtxRotate(&b, &b, &matrix);
                }
                vtx[0].position.x = position.x - b.x;
                vtx[0].position.y = position.y - b.y;
                vtx[0].position.z = position.z - b.z;
                vtx[0].u = 0.0f;
                vtx[0].v = 0.0f;
                vtx[1].position.x = position.x - a.x;
                vtx[1].position.y = position.y - a.y;
                vtx[1].position.z = position.z - a.z;
                vtx[1].u = 1.0f;
                vtx[1].v = 0.0f;
                vtx[2].position.x = position.x + a.x;
                vtx[2].position.y = position.y + a.y;
                vtx[2].position.z = position.z + a.z;
                vtx[2].u = 0.0f;
                vtx[2].v = 1.0f;
                vtx[3].position.x = position.x + b.x;
                vtx[3].position.y = position.y + b.y;
                vtx[3].position.z = position.z + b.z;
                vtx[3].u = 1.0f;
                vtx[3].v = 1.0f;
                NuRndrStrip3d(vtx, material, NULL, 4);
            }
            NuRndrShadowCnt = 0;
        }
    }
}

extern "C" void NuRndrScreenGrabTileInit(void *, i32, f32, f32, f32) {
    STUBBED();
}

extern "C" void NuRndrScreenGrabTileDeInit(void *) {
    STUBBED();
}

extern "C" void NuRndrScreenGrabTileBegin(void **) {
    STUBBED();
}

extern "C" void NuRndrScreenGrabTileEnd(void **) {
    STUBBED();
}
