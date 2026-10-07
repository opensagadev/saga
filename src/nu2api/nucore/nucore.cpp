#include "decomp.h"
#include <float.h>
#include "nu2api/nucore/nucore.hpp"
#include "nu2api/nucore/nuapi.h"

#include <new>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "nu2api/nucore/nustring.h"

#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nucore/NuCopyFilter.h"
#include "nu2api/nucore/NuDataPortManager.h"
#include "nu2api/nucore/NuDeferredFilter.h"
#include "nu2api/nucore/NuDeferredFilterGen.h"
#include "nu2api/nucore/NuDeviceSpecs.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/numtl.h"
extern "C" void DisplaySceneRndrSpecials(NUDLDLISTSCENE *, i32, void *);
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nucore/NuMainFilter.h"
#include "nu2api/nucore/NuMainFilterGen.h"
#include "nu2api/nucore/NuMotionAccumFilter.h"
#include "nu2api/nucore/NuMotionAccumFilterGen.h"
#include "nu2api/nucore/NuMotionFilter.h"
#include "nu2api/nucore/NuMotionFilterGen.h"
#include "nu2api/nucore/NuNetEmu.h"
#include "nu2api/nucore/NuPlatform.h"
#include "nu2api/nucore/NuPostFilter.h"
#include "nu2api/nucore/NuPostFilterGen.h"
#include "nu2api/nucore/NuSpeedBlurFilter.h"
#include "nu2api/nucore/NuSpeedBlurFilterGen.h"
#include "nu2api/nucore/NuVoiceAndroid.h"
#include "gamelib/util/Utilities.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/nushader.h"
#include "nu2api/nu3d/nupostdraw.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nupostshaders.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nucore/nuvuvectype.hpp"

extern const f32 nuvec4_one[4];
extern "C" void NuShaderManagerSetfv(i32, const f32 *);

u32 NuPostFilter::m_fullscreenVertexBuffer, NuPostFilter::m_fullscreenIndexBuffer;
u32 NuPostFilter::m_fullscreenVertexFormat, NuPostFilter::m_fullscreenGridVertexBuffer;
u32 NuPostFilter::m_fullscreenGridIndexBuffer;
i32 NuPostFilter::m_quadGridPrimCount;
extern u32 g_lastBoundVAO;
extern void *g_nuFullscreenVertexFormat;

NuApplicationState *NuCore::m_applicationState;
NuThreadManager *NuCore::m_threadManager;

void NuCore::Initialize() {
    GetApplicationState();

    m_threadManager = new NuThreadManager();
}

NuApplicationState *NuCore::GetApplicationState(void) {
    if (m_applicationState != NULL) {
        return m_applicationState;
    }

    NuApplicationState *state = NU_ALLOC_T(NuApplicationState, 1, "", NUMEMORY_CATEGORY_NONE);
    if (state != NULL) {
        new (state) NuApplicationState();
    }

    m_applicationState = state;

    return state;
}

void NuCopyFilter::destroyResources() {
    NuFramebufferDestroy(copy_fbo);
    NuPostFilterGen::destroyResources();
}

void NuCopyFilter::initResources() {
    NuPostFilterGen::initResources();
    copy_fbo = NuFramebufferCreate();
    NuFramebufferAttachTex2D(copy_fbo, 0, workTex, 0);
    input_fbo = copy_fbo;
}

void NuCopyFilter::render(nuframebuffer_s *output) {
    nueffecttex_s *color = NuFramebufferGetAttachedTex(input_fbo, 0, NULL, NULL);
    if (copy_texture != NULL)
        copy(color, copy_texture, output);
    else
        copy(color, output);
}

void NuCopyFilter::reset() {
    input_fbo = copy_fbo;
    copy_texture = NULL;
}

void NuMainFilter::initResources() {
    NuMainFilterGen::initResources();
    programs[15] = NuShaderProgramCreateIOS(blur7x7_vx, blur7x7dof_px);
}

void NuPostFilter::initSharedResources(i32, i32) {
    BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nupostfilter.cpp", 0x2a);
    NuPostFilterGen::initSharedResources();
    NuPostFilterGen::copyTexProgram = NuShaderProgramCreateIOS(default_vx, copytex_px);
    NuPostFilterGen::copyTexLodProgram = NULL;
    NuPostFilterGen::blur5x5Program = NULL;
    NuPostFilterGen::blur7x7Program = NULL;
    NuPostFilterGen::blurGuardProgram = NULL;
    const f32 vertices[12] = {-1, 1, 0, -1, -1, 0, 1, 1, 0, 1, -1, 0};
    glGenBuffers(1, &m_fullscreenVertexBuffer);
    if (g_lastBoundVAO != 0)
        g_lastBoundVAO = 0;
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreenVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenBuffers(1, &m_fullscreenGridVertexBuffer);
    glGenBuffers(1, &m_fullscreenGridIndexBuffer);
    i32 rows = static_cast<i32>((static_cast<f32>(nurndr_pixel_height) / nurndr_pixel_width) * 16.0f);
    i32 size = (rows + 1) * 17 * 3 * sizeof(f32);
    f32 *grid = static_cast<f32 *>(NU_ALLOC(size, 4, 1, "", NUMEMORY_CATEGORY_NONE));
    // The original expands each fixed-width row into 17 vertex writes.
    f32 *row = grid;
    for (i32 y = 0; y <= rows; ++y, row += 51) {
        const f32 fraction = static_cast<f32>(y) / rows;
        const f32 screen_y = 1.0f - (fraction + fraction);
#define GRID_VERTEX(column)                                                                                            \
    row[(column) * 3] = -1.0f + (column) * 0.125f;                                                                     \
    row[(column) * 3 + 1] = screen_y;                                                                                  \
    row[(column) * 3 + 2] = 0.0f
        GRID_VERTEX(0);
        GRID_VERTEX(1);
        GRID_VERTEX(2);
        GRID_VERTEX(3);
        GRID_VERTEX(4);
        GRID_VERTEX(5);
        GRID_VERTEX(6);
        GRID_VERTEX(7);
        GRID_VERTEX(8);
        GRID_VERTEX(9);
        GRID_VERTEX(10);
        GRID_VERTEX(11);
        GRID_VERTEX(12);
        GRID_VERTEX(13);
        GRID_VERTEX(14);
        GRID_VERTEX(15);
        GRID_VERTEX(16);
#undef GRID_VERTEX
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreenGridVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, size, grid, GL_STATIC_DRAW);
    NU_FREE(grid);
    u16 *indices = static_cast<u16 *>(NU_ALLOC(rows * 192, 4, 1, "", NUMEMORY_CATEGORY_NONE));
    // Each row contains 16 cells, with the original two-triangle winding.
    u16 *index_row = indices;
    i32 vertex = 0;
    for (i32 y = 0; y < rows; ++y, index_row += 96, vertex += 17) {
#define GRID_CELL(column)                                                                                              \
    index_row[(column) * 6] = vertex + (column);                                                                       \
    index_row[(column) * 6 + 1] = vertex + (column) + 1;                                                               \
    index_row[(column) * 6 + 2] = vertex + (column) + 17;                                                              \
    index_row[(column) * 6 + 3] = vertex + (column) + 17;                                                              \
    index_row[(column) * 6 + 4] = vertex + (column) + 1;                                                               \
    index_row[(column) * 6 + 5] = vertex + (column) + 18
        GRID_CELL(0);
        GRID_CELL(1);
        GRID_CELL(2);
        GRID_CELL(3);
        GRID_CELL(4);
        GRID_CELL(5);
        GRID_CELL(6);
        GRID_CELL(7);
        GRID_CELL(8);
        GRID_CELL(9);
        GRID_CELL(10);
        GRID_CELL(11);
        GRID_CELL(12);
        GRID_CELL(13);
        GRID_CELL(14);
        GRID_CELL(15);
#undef GRID_CELL
    }
    if (g_lastBoundVAO != 0)
        g_lastBoundVAO = 0;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_fullscreenGridIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, rows * 192, indices, GL_STATIC_DRAW);
    NU_FREE(indices);
    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nupostfilter.cpp", 0x7d);
}

void NuPostFilter::renderFrustum(numtx_s *) {
    STUBBED();
}

void NuMotionFilter::initResources() {
}

void NuDeferredFilter::initResources() {
}

void NuSpeedBlurFilter::initResources() {
}

void NuPostFilterGen::GetSampleOffsets_GaussBlur5x5(i32 width, i32 height, VuVec *samples, float scale) {
    f32 dx = 1.0f / width, dy = 1.0f / height;
    // Retail assigns the thirteen diamond taps before its weight-scale loop.
    // Preserve zero products and the positive two-step additions as written.
    samples[0].x = -2.0f * dx;
    samples[0].y = dy * 0.0f;
    samples[0].z = 0.053990968f;
    samples[0].w = 0.0f;
    samples[1].x = -dx;
    samples[1].y = -dy;
    samples[1].z = 0.14676267f;
    samples[1].w = 0.0f;
    samples[2].x = -dx;
    samples[2].y = dy * 0.0f;
    samples[2].z = 0.24197073f;
    samples[2].w = 0.0f;
    samples[3].x = -dx;
    samples[3].y = dy;
    samples[3].z = 0.14676267f;
    samples[3].w = 0.0f;
    samples[4].x = dx * 0.0f;
    samples[4].y = -2.0f * dy;
    samples[4].z = 0.053990968f;
    samples[4].w = 0.0f;
    samples[5].x = dx * 0.0f;
    samples[5].y = -dy;
    samples[5].z = 0.24197073f;
    samples[5].w = 0.0f;
    samples[6].x = dx * 0.0f;
    samples[6].y = dy * 0.0f;
    samples[6].z = 0.3989423f;
    samples[6].w = 0.0f;
    samples[7].x = dx * 0.0f;
    samples[7].y = dy;
    samples[7].z = 0.24197073f;
    samples[7].w = 0.0f;
    samples[8].x = dx * 0.0f;
    samples[8].y = dy + dy;
    samples[8].z = 0.053990968f;
    samples[8].w = 0.0f;
    samples[9].x = dx;
    samples[9].y = -dy;
    samples[9].z = 0.14676267f;
    samples[9].w = 0.0f;
    samples[10].x = dx;
    samples[10].y = dy * 0.0f;
    samples[10].z = 0.24197073f;
    samples[10].w = 0.0f;
    samples[11].x = dx;
    samples[11].y = dy;
    samples[11].z = 0.14676267f;
    samples[11].w = 0.0f;
    samples[12].x = dx + dx;
    samples[12].y = dy * 0.0f;
    samples[12].z = 0.053990968f;
    samples[12].w = 0.0f;
    for (i32 i = 0; i < 13; ++i) {
        samples[i].z = (samples[i].z / 2.1698399f) * scale;
    }
}

void NuDeferredFilterGen::render() {
    nuframebuffer_s *output = static_cast<nuframebuffer_s *>(portOutFramebuffer.get());
    NuProxyBuffer *color = static_cast<NuProxyBuffer *>(portColorBuffer.get());
    NuProxyBuffer *normal = static_cast<NuProxyBuffer *>(portNormalBuffer.get());
    NuProxyBuffer *depth_rt = static_cast<NuProxyBuffer *>(portDepthRTBuffer.get());
    NuProxyBuffer *depth = static_cast<NuProxyBuffer *>(portDepthBuffer.get());
    NuPostResolve(color);
    NuPostResolve(normal);
    NuPostResolve(depth_rt);
    NuPostResolve(depth);
    nueffecttex_s *destination = NuFramebufferGetAttachedTex(output, 0, NULL, NULL);
    f32 last_sample = static_cast<f32>(sample_count) - 1.0f;
    bool first = true;
    for (i32 i = 0; i < dynamic_light_count; ++i) {
        NuDynamicLight *light = dynamic_lights[i];
        if (light->reserved_7bc == 0)
            continue;
        NUMTX view, projection;
        memcpy(&view, g_renderContext_view, sizeof(view));
        memcpy(&projection, g_renderContext_projection, sizeof(projection));
        i32 shadow_count = light->active_render_set_count;
        for (i32 shadow = 0; shadow < shadow_count; ++shadow) {
            // The original selects consecutive texture members beginning at 0x30.
            nueffecttex_s *shadow_texture = textures[shadow + 2];
            NuFramebufferAttachTex2D(shadow_fbos[shadow], 4, shadow_texture, 0);
            NuFramebufferBind(shadow_fbos[shadow]);
            i32 width, height;
            NuEffectTexGetDimension(shadow_texture, 0, &width, &height);
            NuRenderContextSetViewport(0, 0, width, height);
            NuFramebufferClear(0x300, 0xffffffff);
            light->renderShadowMap(shadow, shadow_fbos[shadow]);
            NuFramebufferResolveAll(true);
        }
        NuRenderContextSetViewProj(&view, &projection);
        NuFramebufferBind(light_fbo);
        if (first)
            NuFramebufferClear(0x900, 0x00ff0000);
        i32 type = light->parameter_5;
        nushaderprogram_s *program = programs[type == 0 || type == 1 ? 0 : 1];
        if (!first) {
            g_boundShader = 0;
            glUseProgram(0);
            g_currentShaderProgram = NULL;
        }
        {
            nushaderprogram_s *bound = program;
            g_boundShader = bound != NULL ? bound->program : 0;
            glUseProgram(g_boundShader);
            g_currentShaderProgram = bound;
        }
        light->bindShaderResources(program);
        PostBlurDrawQuad();
        NuFramebufferResolve(0, false);
        first = false;
    }
    copy(textures[0], 1, textures[0], 0, copyTexProgram, NULL);
    blur7x7Loopback(textures[0], 1, textures[0], 2, 2, sample_count - 2, true, 1.0f, blur7x7Program);
    NuFramebufferBind(output);
    i32 width = NuFramebufferGetWidth(output);
    i32 height = NuFramebufferGetHeight(output);
    NuRenderContextSetViewport(0, 0, width, height);
    {
        nushaderprogram_s *bound = programs[2];
        g_boundShader = bound != NULL ? bound->program : 0;
        glUseProgram(g_boundShader);
        g_currentShaderProgram = bound;
    }
    f32 params[4] = {parameters[1], last_sample, parameters[2], parameters[3]};
    PostBlurSetVertexParam(programs[2], 0x80a0, params, 4);
    PostBlurDrawQuad();
    color->texture = destination;
    color->kind = 0;
    color->enabled = true;
    color->resolved = false;
    portColorBuffer.set(color);
}

void NuDeferredFilterGen::resetAll() {
    for (i32 i = 0; i < dynamic_light_count; ++i) {
        dynamic_lights[i]->resetGeometry();
    }
    dynamic_light_count = 0;
    deferred_geometry_count = 0;
}

void NuMotionAccumFilter::initResources() {
}

NuNetEmu::EmuPacket *NuNetEmu::FindPacket(nunetaddr_s *, i32 size) {
    EmuPacket *packet = field_04;
    u32 now = UtilGetFrameStartTime();
    size += 2;
    while (packet != NULL) {
        if (size <= 0xbb8 - packet->payload_size &&
            (packet->flush_time >= packet->send_time || now <= packet->flush_time)) {
            break;
        }
        packet = packet->next;
    }
    return packet;
}

NuNetEmu theNuNetEmu;

typedef i32 (*NuNetSessionSendToFn)(NetSession *, void *, i32, nunetaddr_s *);
typedef i32 (*NuNetSessionRecvFromFn)(NetSession *, void *, i32, nunetaddr_s *);
struct MemoryManager;
extern MemoryManager theMemoryManager;
extern "C" void *MemoryManagerAllocPool(MemoryManager *, u32, i32) asm("_ZN13MemoryManager9AllocPoolEji");
extern "C" void MemoryManagerFreePool(MemoryManager *, void *, u32) asm("_ZN13MemoryManager8FreePoolEPvj");

static inline i32 NuNetSessionSendTo(void *data, i32 size, nunetaddr_s *address) {
    void **vtable = *reinterpret_cast<void ***>(theSession);
    return reinterpret_cast<NuNetSessionSendToFn>(vtable[22])(theSession, data, size, address);
}

static inline i32 NuNetSessionRecvFrom(void *data, i32 size, nunetaddr_s *address) {
    void **vtable = *reinterpret_cast<void ***>(theSession);
    return reinterpret_cast<NuNetSessionRecvFromFn>(vtable[23])(theSession, data, size, address);
}

extern i32 unref(unsigned char *, unsigned char *);
extern i32 refpack(unsigned char *, abi_long, unsigned char *);

struct NuNetEmuSegment {
    u16 offset;
    u16 size;
};

static i32 cbSortSeg(void const *left, void const *right) {
    u16 left_size = static_cast<NuNetEmuSegment const *>(left)->size;
    u16 right_size = static_cast<NuNetEmuSegment const *>(right)->size;
    if (left_size < right_size) {
        return 1;
    }
    if (left_size > right_size) {
        return -1;
    }
    return 0;
}

NuNetEmu::NuNetEmu() : field_04(NULL), field_08(NULL), field_0c(0), raw_stats("EmuRaw"), packet_stats("EmuPack") {
    field_10 = 0;
    field_14 = 0;
    field_18 = 0;
    field_20 = 0;
    field_24 = 0;
    field_28 = 0;
    field_2c = 0;
    field_30 = 0;
    field_34 = 0.0f;
    field_38 = 0x320;
    field_3c = 0x32;
    field_17bc = 0;
    field_17c0 = 0x1800;
    field_17c4 = 0x2800;
    field_17c8 = 0x80;
    field_17b0 = 0;
    field_17b4 = 0;
    field_17b8 = 0;
    SetConditions(CONDITIONS_NORMAL);
    field_00 = 0x200;
}

i32 NuNetEmu::RecvFrom(void *data, i32 size, nunetaddr_s &address) {
    if (field_10 == 0) {
        return NuNetSessionRecvFrom(data, size, &address);
    }

    i32 received = 0;
    if (field_17b0 >= field_17b4) {
        received = NuNetSessionRecvFrom(packed_buffer, 0xbb8, &address);
        if (received > 0) {
            packet_stats.total.values[1] += received;
            packet_stats.total.values[3]++;
            field_17b0 = 0;
            field_17b8 = received;
            u32 start_time = UtilGetTime();
            field_17b4 = unref(packed_buffer, unpacked_buffer);
            packet_stats.packed_values[1] += UtilGetTime() - start_time;
            raw_stats.total.values[3]++;
            raw_stats.total.values[1] += field_17b4;
        }
    }

    if (field_17b0 < field_17b4 && received >= 0) {
        i32 payload_size = unpacked_buffer[field_17b0] | (unpacked_buffer[field_17b0 + 1] << 8);
        i32 copy_size = payload_size <= size ? payload_size : size;
        memmove(data, unpacked_buffer + field_17b0 + 2, copy_size);
        field_17b0 += payload_size + 2;
        return copy_size;
    }
    return received;
}

i32 NuNetEmu::SendTo(void *data, i32 size, nunetaddr_s *address, i32) {
    if (field_10 == 0) {
        return NuNetSessionSendTo(data, size, address);
    }

    if (field_24 == 1) {
        f32 random = NuRandFloat();
        if (field_34 > random) {
            return size;
        }
    } else if (field_24 == 2) {
        if (field_30 == 0) {
            f32 random = NuRandFloat();
            if (field_34 > random) {
                f32 range = static_cast<f32>(field_2c - field_28);
                f32 random = NuRandFloat();
                field_30 = static_cast<i32>(random * range) + field_28;
            }
        }
        if (field_30 > 0) {
            field_30--;
            return size;
        }
    }

    if (field_0c >= field_17c8) {
        return 0;
    }

    EmuPacket *packet = FindPacket(address, size);
    if (packet == NULL) {
        packet = new (MemoryManagerAllocPool(&theMemoryManager, sizeof(EmuPacket), 1)) EmuPacket(address);
        u32 now = UtilGetFrameStartTime();
        if (field_18 > 0) {
            f32 range = static_cast<f32>(field_18 - field_14);
            f32 random = NuRandFloat();
            packet->send_time = now + (static_cast<u32>(static_cast<i32>(random * range)) + static_cast<u32>(field_14));
        } else {
            packet->send_time = 0;
        }
        packet->creation_time = now;
        packet->flush_time = now + field_3c;

        packet->next = NULL;
        packet->previous = field_08;
        if (field_08 != NULL) {
            field_08->next = packet;
        }
        field_08 = packet;
        if (field_04 == NULL) {
            field_04 = packet;
        }
        field_0c++;
    }
    packet->AddPayload(data, size);
    field_1c += size;
    return size;
}

void NuNetEmu::SetConditions(NuNetEmu::eConditions conditions) {
    field_20 = conditions;
    switch (conditions) {
        case CONDITIONS_NORMAL:
            field_24 = 0;
            field_14 = 0;
            field_18 = 0;
            break;
        case CONDITIONS_1:
            field_24 = 1;
            field_14 = 20;
            field_34 = 0.005f;
            field_18 = 40;
            break;
        case CONDITIONS_2:
            field_24 = 1;
            field_14 = 50;
            field_34 = 0.02f;
            field_18 = 70;
            break;
        case CONDITIONS_3:
            field_24 = 2;
            field_28 = 5;
            field_34 = 0.01f;
            field_2c = 15;
            field_14 = 100;
            field_18 = 120;
            break;
    }
}

i32 NuNetEmu::SplitSendPacket(NuNetEmu::EmuPacket *packet) {
    u32 start_time = UtilGetTime();
    field_17b8 = refpack(packet->payload, packet->payload_size, packed_buffer);
    packet_stats.packed_values[0] += UtilGetTime() - start_time;

    i32 packed_size = field_17b8;
    if (packed_size <= 0x4f0) {
        packet_stats.total.values[0] += packed_size;
        packet_stats.total.values[2]++;
        NuNetSessionSendTo(packed_buffer, packed_size, reinterpret_cast<nunetaddr_s *>(&packet->address));
        return packed_size;
    }

    packet_stats.split_packets++;
    NuNetEmuSegment segments[256];
    u32 segment_count = 0;
    u32 offset = 0;
    while (offset < packet->payload_size && segment_count < 256) {
        i32 payload_size = packet->payload[offset] | (packet->payload[offset + 1] << 8);
        segments[segment_count].offset = offset;
        segments[segment_count].size = payload_size + 2;
        segment_count++;
        offset += payload_size + 2;
    }
    qsort(segments, segment_count, sizeof(NuNetEmuSegment), cbSortSeg);

    EmuPacket *first = new (MemoryManagerAllocPool(&theMemoryManager, sizeof(EmuPacket), 1))
        EmuPacket(reinterpret_cast<nunetaddr_s *>(&packet->address));
    EmuPacket *second = new (MemoryManagerAllocPool(&theMemoryManager, sizeof(EmuPacket), 1))
        EmuPacket(reinterpret_cast<nunetaddr_s *>(&packet->address));

    EmuPacket *packets[2] = {first, second};
    for (u32 i = 0; i < segment_count; i++) {
        EmuPacket *destination = packets[second->payload_size < first->payload_size];
        memmove(destination->payload + destination->payload_size, packet->payload + segments[i].offset,
                segments[i].size);
        destination->payload_size += segments[i].size;
    }

    i32 sent = SplitSendPacket(first) + SplitSendPacket(second);
    if (first != NULL) {
        first->~EmuPacket();
        MemoryManagerFreePool(&theMemoryManager, first, sizeof(EmuPacket));
    }
    if (second != NULL) {
        second->~EmuPacket();
        MemoryManagerFreePool(&theMemoryManager, second, sizeof(EmuPacket));
    }
    return sent;
}

// The retained emulator uses this sixteen-bit unsigned conversion closure.
static inline f32 NetEmuUnsignedFloat(u32 value) {
    return static_cast<f32>(static_cast<i32>(value >> 16)) * 65536.0f +
           static_cast<f32>(static_cast<i32>(value & 0xffff));
}

void NuNetEmu::Update() {
    u32 now = UtilGetFrameStartTime();
    if (now > static_cast<u32>(field_17bc)) {
        u32 bandwidth = field_0c > 2 ? field_17c4 : field_17c0;
        u32 budget = bandwidth / 30;
        u32 sent = 0;
        EmuPacket *packet = field_04;
        while (sent < budget && packet != NULL) {
            if (now >= packet->send_time && (now >= packet->flush_time || packet->payload_size >= field_38)) {
                EmuPacket *next = packet->next;
                raw_stats.total.values[0] += packet->payload_size;
                raw_stats.total.values[2]++;
                sent += SplitSendPacket(packet);

                if (packet->next != NULL) {
                    packet->next->previous = packet->previous;
                } else {
                    field_08 = packet->previous;
                }
                if (packet->previous != NULL) {
                    packet->previous->next = packet->next;
                } else {
                    field_04 = packet->next;
                }
                packet->next = NULL;
                packet->previous = NULL;
                field_0c--;
                packet->~EmuPacket();
                MemoryManagerFreePool(&theMemoryManager, packet, sizeof(EmuPacket));
                packet = next;
            } else {
                packet = packet->next;
            }
        }
        field_17bc = now + static_cast<i32>(NetEmuUnsignedFloat(sent) / (NetEmuUnsignedFloat(bandwidth) / 1000.0f));
    }
    field_1c = 0;
    for (EmuPacket *packet = field_04; packet != NULL; packet = packet->next) {
        if (packet->flush_time >= packet->send_time || now <= packet->flush_time) {
            field_1c += packet->payload_size;
        }
    }

    raw_stats.Update();
    packet_stats.Update();
    {
        f32 ratio = 0.0f;
        f32 raw_size = NetEmuUnsignedFloat(raw_stats.total.values[0]);
        if (raw_size > 0.0f) {
            ratio = NetEmuUnsignedFloat(packet_stats.total.values[0]) / raw_size;
        }
        packet_stats.pack_ratio = ratio;

        f32 average = 0.0f;
        if (packet_stats.total.values[2] > 0) {
            average =
                NetEmuUnsignedFloat(packet_stats.total.values[0]) / NetEmuUnsignedFloat(packet_stats.total.values[2]);
        }
        packet_stats.average_packet_size = average;
    }
    packet_stats.held_packets = field_0c;
}

void NuDynamicLight::addShadowCasterScene(nugscn_s *scene) {
    if (render_set_capacity > 0) {
        RenderSet *current = render_sets;
        RenderSet *end = render_sets + render_set_capacity;
        do {
            RenderSet &set = *current;
            set.scene_cursor[0] = NULL;
            set.scene_first[0] = NULL;
            set.scene_end[0] = NULL;
            set.scene_cursor[1] = NULL;
            set.scene_first[1] = NULL;
            set.scene_end[1] = NULL;
        } while (++current != end);
    }
    NUDLDLISTSCENE *dl = scene->display_list;
    i32 instance = 0;
    for (i32 lod = 0; lod < dl->nclip_objects; ++instance, ++lod) {
        if ((dl->visibility_flags[instance] & 0x21) == 0x21) {
            if (dl->lod_ranges[lod] != 0.0f) {
                f32 dx = global_camera.mtx.m30 - dl->clip_bounds[instance].center.x;
                f32 dy = global_camera.mtx.m31 - dl->clip_bounds[instance].center.y;
                f32 dz = global_camera.mtx.m32 - dl->clip_bounds[instance].center.z;
                f32 distance = dx * dx + dy * dy + dz * dz;
                while (dl->lod_ranges[lod] > distance)
                    ++lod;
            }
            NUCLIPOBJECT *object = &dl->clip_objects[lod];
            for (i32 i = 0; i < render_set_capacity; ++i) {
                RenderSet &set = render_sets[i];
                NUPORTALBOX &box = scene->portal_boxes[instance];
                f32 sx = set.capsule_end.x - set.capsule_center.x;
                f32 sy = set.capsule_end.y - set.capsule_center.y;
                f32 sz = set.capsule_end.z - set.capsule_center.z;
                f32 length = sy * sy + sx * sx + sz * sz;
                f32 radius = set.capsule_radius + box.first_w;
                f32 ox = box.first.x - set.capsule_center.x;
                f32 oy = box.first.y - set.capsule_center.y;
                f32 oz = box.first.z - set.capsule_center.z;
                f32 dot = sx * ox + sy * oy + sz * oz;
                f32 projection = (1.0f / length) * dot;
                f32 positive = (0.0f < projection ? 1.0f : 0.0f) * projection;
                f32 t = (positive <= 1.0f ? 1.0f : 0.0f) * positive + (1.0f < positive ? 1.0f : 0.0f);
                f32 distance = (ox * ox + oy * oy + oz * oz) + (length * t) * t - (dot + dot) * t;
                if (distance > radius * radius)
                    continue;
                VuVec minimum, maximum;
                minimum.x = box.first.x - box.second.x;
                minimum.y = box.first.y - box.second.y;
                minimum.z = box.first.z - box.second.z;
                maximum.x = box.first.x + box.second.x;
                maximum.y = box.first.y + box.second.y;
                maximum.z = box.first.z + box.second.z;
                if (!testShadowExtrusion(minimum, maximum, i))
                    continue;
                for (i32 j = 0; j < object->nmaterials; ++j) {
                    NUMTL *material = scene->mtls[object->material_ids[j]];
                    i32 channel = material->shader_desc.unknown_1b4 != 0;
                    if ((usize)set.scene_cursor[channel] >= (usize)set.scene_end[channel]) {
                        NUDISPLAYLISTITEM *cursor =
                            (NUDISPLAYLISTITEM *)((display_list_buffer->addr + 15) & ~(usize)15);
                        set.scene_cursor[channel] = cursor;
                        display_list_buffer->addr = (usize)(cursor + 49);
                        if (set.scene_end[channel]) {
                            set.scene_end[channel]->next = cursor;
                            set.scene_end[channel]->type = 0x8d;
                            set.scene_end[channel]->id = 1;
                        } else
                            set.scene_first[channel] = cursor;
                        set.scene_end[channel] = cursor + 48;
                        material = scene->mtls[object->material_ids[j]];
                    }
                    set.scene_cursor[channel][0].id = 3;
                    set.scene_cursor[channel][0].type = 0x80;
                    set.scene_cursor[channel][0].next = material;
                    set.scene_cursor[channel][1] = dl->items[object->indices[j] - 1];
                    set.scene_cursor[channel][2] = dl->items[object->indices[j]];
                    set.scene_cursor[channel][1].id = 3;
                    set.scene_cursor[channel][2].id = 3;
                    set.scene_cursor[channel] += 3;
                }
            }
        }
        while (dl->lod_ranges[lod] != 0.0f)
            ++lod;
    }
    DisplaySceneRndrSpecials(dl, 0, NULL);
    for (i32 i = 0; i < render_set_capacity; ++i) {
        RenderSet &set = render_sets[i];
        if (set.scene_first[0]) {
            NUDISPLAYLIST *list = &set.display_lists[0];
            NuDisplayListLinkItems(list, 1);
            list->items->type = 0x80;
            list->items->id = 3;
            list->items->next = scene->mtls[0];
            ++list->items;
            NuDisplayListLinkList(list, set.scene_first[0], set.scene_cursor[0]);
        }
        if (set.scene_first[1]) {
            NUDISPLAYLIST *list = &set.display_lists[1];
            NuDisplayListLinkItems(list, 1);
            list->items->type = 0x80;
            list->items->id = 3;
            list->items->next = scene->mtls[0];
            ++list->items;
            NuDisplayListLinkList(list, set.scene_first[1], set.scene_cursor[1]);
        }
    }
}

static inline void CloneLightVector(NUVEC4 &destination, const NUVEC4 &source) {
    destination.x = source.x;
    destination.y = source.y;
    destination.z = source.z;
    destination.w = source.w;
}
NuDynamicLight *NuDynamicLight::clone(variptr_u *arena, variptr_u) {
    NuDynamicLight *copy = reinterpret_cast<NuDynamicLight *>((arena->addr + 31) & ~usize(31));
    arena->addr = reinterpret_cast<usize>(copy) + ((sizeof(NuDynamicLight) + 31) & ~usize(31));
    RenderSet *destination_ptr = copy->render_sets;
    const RenderSet *source_ptr = render_sets;
    do {
        RenderSet &destination = *destination_ptr;
        const RenderSet &source = *source_ptr;
        destination.view = source.view;
        destination.projection = source.projection;
        destination.warp = source.warp;
        destination.shadow_transform = source.shadow_transform;
        memcpy(destination.reserved_100, source.reserved_100, sizeof(source.reserved_100));
        destination.warp_factor = source.warp_factor;
        CloneLightVector(destination.corners[0], source.corners[0]);
        CloneLightVector(destination.corners[1], source.corners[1]);
        CloneLightVector(destination.corners[2], source.corners[2]);
        CloneLightVector(destination.corners[3], source.corners[3]);
        CloneLightVector(destination.corners[4], source.corners[4]);
        CloneLightVector(destination.corners[5], source.corners[5]);
        CloneLightVector(destination.corners[6], source.corners[6]);
        CloneLightVector(destination.corners[7], source.corners[7]);
        CloneLightVector(destination.shadow_planes[0], source.shadow_planes[0]);
        CloneLightVector(destination.shadow_planes[1], source.shadow_planes[1]);
        CloneLightVector(destination.shadow_planes[2], source.shadow_planes[2]);
        CloneLightVector(destination.shadow_planes[3], source.shadow_planes[3]);
        CloneLightVector(destination.shadow_planes[4], source.shadow_planes[4]);
        CloneLightVector(destination.shadow_planes[5], source.shadow_planes[5]);
        CloneLightVector(destination.shadow_planes[6], source.shadow_planes[6]);
        CloneLightVector(destination.shadow_planes[7], source.shadow_planes[7]);
        CloneLightVector(destination.shadow_planes[8], source.shadow_planes[8]);
        CloneLightVector(destination.shadow_planes[9], source.shadow_planes[9]);
        CloneLightVector(destination.shadow_planes[10], source.shadow_planes[10]);
        CloneLightVector(destination.shadow_planes[11], source.shadow_planes[11]);
        destination.shadow_plane_count = source.shadow_plane_count;
        CloneLightVector(destination.far_plane, source.far_plane);
        CloneLightVector(destination.capsule_center, source.capsule_center);
        CloneLightVector(destination.capsule_end, source.capsule_end);
        destination.capsule_radius = source.capsule_radius;
        destination.display_lists[0] = source.display_lists[0];
        destination.display_lists[1] = source.display_lists[1];
        {
            nurndrstate_s *first = source.render_states[0];
            nurndrstate_s *second = source.render_states[1];
            destination.render_states[1] = second;
            destination.render_states[0] = first;
        }
        {
            NUDISPLAYLISTITEM *first = source.list_items[0];
            NUDISPLAYLISTITEM *second = source.list_items[1];
            destination.list_items[1] = second;
            destination.list_items[0] = first;
        }
        {
            NUDISPLAYLISTITEM *first = source.scene_cursor[0];
            NUDISPLAYLISTITEM *second = source.scene_cursor[1];
            destination.scene_cursor[1] = second;
            destination.scene_cursor[0] = first;
        }
        {
            NUDISPLAYLISTITEM *first = source.scene_first[0];
            NUDISPLAYLISTITEM *second = source.scene_first[1];
            destination.scene_first[1] = second;
            destination.scene_first[0] = first;
        }
        {
            NUDISPLAYLISTITEM *first = source.scene_end[0];
            NUDISPLAYLISTITEM *second = source.scene_end[1];
            destination.scene_end[1] = second;
            destination.scene_end[0] = first;
        }
        destination.reserved_33c[0] = source.reserved_33c[0];
        destination.reserved_33c[1] = source.reserved_33c[1];
        destination.reserved_33c[2] = source.reserved_33c[2];
        destination.reserved_33c[3] = source.reserved_33c[3];
        destination.reserved_33c[4] = source.reserved_33c[4];
        destination.reserved_33c[5] = source.reserved_33c[5];
        destination.reserved_33c[6] = source.reserved_33c[6];
        destination.reserved_33c[7] = source.reserved_33c[7];
        destination.geometry_count = source.geometry_count;
        ++source_ptr;
        ++destination_ptr;
    } while (destination_ptr != copy->render_sets + 2);
    copy->reserved_6c0[0] = reserved_6c0[0];
    copy->reserved_6c0[1] = reserved_6c0[1];
    copy->reserved_6c0[2] = reserved_6c0[2];
    copy->render_set_capacity = render_set_capacity;
    copy->active_render_set_count = active_render_set_count;
    copy->reserved_6d4[0] = reserved_6d4[0];
    copy->reserved_6d4[1] = reserved_6d4[1];
    copy->reserved_6d4[2] = reserved_6d4[2];
    copy->reserved_6d4[3] = reserved_6d4[3];
    copy->reserved_6d4[4] = reserved_6d4[4];
    copy->reserved_6d4[5] = reserved_6d4[5];
    copy->reserved_6d4[6] = reserved_6d4[6];
    copy->reserved_6d4[7] = reserved_6d4[7];
    copy->reserved_6d4[8] = reserved_6d4[8];
    copy->reserved_6d4[9] = reserved_6d4[9];
    copy->reserved_6d4[10] = reserved_6d4[10];
    copy->reserved_6d4[11] = reserved_6d4[11];
    copy->reserved_6d4[12] = reserved_6d4[12];
    copy->reserved_6d4[13] = reserved_6d4[13];
    copy->reserved_6d4[14] = reserved_6d4[14];
    copy->reserved_6d4[15] = reserved_6d4[15];
    copy->reserved_6d4[16] = reserved_6d4[16];
    copy->reserved_6d4[17] = reserved_6d4[17];
    copy->reserved_6d4[18] = reserved_6d4[18];
    copy->reserved_6d4[19] = reserved_6d4[19];
    copy->direction.x = direction.x;
    copy->direction.y = direction.y;
    copy->direction.z = direction.z;
    copy->reserved_730 = reserved_730;
    copy->view = view;
    copy->projection = projection;
    copy->parameter_4 = parameter_4;
    copy->parameter_5 = parameter_5;
    copy->reserved_7bc = reserved_7bc;
    copy->reserved_7c0[0] = reserved_7c0[0];
    copy->reserved_7c0[1] = reserved_7c0[1];
    CloneLightVector(copy->reserved_7c8, reserved_7c8);
    copy->used_on_specials = used_on_specials;
    for (i32 i = 0; i < render_set_capacity; ++i) {
        RenderSet &set = render_sets[i];
        set.geometry_count = 0;
        NuDisplayListReset(&set.display_lists[0]);
        NUDISPLAYLISTITEM *item0 = set.display_lists[0].first;
        item0->type = 0x8d;
        item0->next = NULL;
        item0->id = 1;
        NuDisplayListReset(&set.display_lists[1]);
        NUDISPLAYLISTITEM *item1 = set.display_lists[1].first;
        item1->type = 0x8d;
        item1->id = 1;
        item1->next = NULL;
    }
    return copy;
}

void NuDynamicLight::renderShadowMap(i32 index, nuframebuffer_s *) {
    RenderSet &set = render_sets[index];
    if (set.display_lists[0].mtl_last == set.display_lists[0].first &&
        set.display_lists[1].mtl_last == set.display_lists[1].first && set.geometry_count == 0)
        return;
    refreshShadowTransform(set);
    NuDisplayListSetItemTable(1);
    NUMTX bias = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5f, 0, 0, 0, 0.5f, 1};
    NUMTX matrix;
    NuMtxMulH(&matrix, &set.warp, &bias);
    NuRenderContextSetViewProj(&matrix, &numtx_identity);
    if (set.display_lists[0].mtl_last != set.display_lists[0].first) {
        NUDISPLAYLISTITEM *item = set.display_lists[0].mtl_last;
        item->type = 0x84;
        item->id = 4;
        item->next = NULL;
        if (g_renderContext_zFunc != 0) {
            glEnable(0xb71);
            glDepthMask(1);
            glDepthFunc(0x203);
        }
        g_renderContext_zFunc = 0;
        memcpy(g_renderContext_kTint, nuvec4_one, sizeof(g_renderContext_kTint));
        NuShaderManagerSetfv(0x44, nuvec4_one);
        NuDisplayListDrawItems(set.display_lists[0].first);
        set.display_lists[0].mtl_last = set.display_lists[0].first;
    }
    if (set.display_lists[1].mtl_last != set.display_lists[1].first) {
        NUDISPLAYLISTITEM *item = set.display_lists[1].mtl_last;
        item->type = 0x84;
        item->id = 4;
        item->next = NULL;
        if (g_renderContext_zFunc != 0) {
            glEnable(0xb71);
            glDepthMask(1);
            glDepthFunc(0x203);
        }
        g_renderContext_zFunc = 0;
        memcpy(g_renderContext_kTint, nuvec4_one, sizeof(g_renderContext_kTint));
        NuShaderManagerSetfv(0x44, nuvec4_one);
        NuDisplayListDrawItems(set.display_lists[1].first);
        set.display_lists[1].mtl_last = set.display_lists[1].first;
    }
    for (i32 i = 0; i < set.geometry_count; ++i)
        NuDisplayListDrawRenderScene(set.reserved_33c[i]);
    NuDisplayListSetItemTable(0);
}

void NuDynamicLight::resetGeometry() {
    for (i32 i = 0; i < render_set_capacity; ++i) {
        RenderSet &set = render_sets[i];
        set.geometry_count = 0;
        NuDisplayListReset(&set.display_lists[0]);
        NUDISPLAYLISTITEM *item0 = set.display_lists[0].first;
        item0->type = 0x8d;
        item0->next = NULL;
        item0->id = 1;
        NuDisplayListReset(&set.display_lists[1]);
        NUDISPLAYLISTITEM *item1 = set.display_lists[1].first;
        item1->type = 0x8d;
        item1->id = 1;
        item1->next = NULL;
    }
}

void NuDynamicLight::setCameraViewProj(NUMTX *view, NUMTX *projection) {
    cacheCameraView = *view;
    cacheCameraProj = *projection;
}

NUMTX NuDynamicLight::cacheCameraView;
NUMTX NuDynamicLight::cacheCameraProj;
