// Android debris renderer buffer management and its original TU-local state.

#include <GLES2/gl2.h>

#include "decomp.h"
#include "globals.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/common.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nutrig.h"

static NUMTX NuRndr_DebrisMtx;
static NUVEC4 NuRndr_DebrisPlane;
static NUMTX *NuRndr_DebrisRotMtxPtr;
static i32 g_UseSysMemVB;
static u32 g_CurrentDebriVBIndex;
static u32 g_DebriVB[8];
static void *g_DebriSysMemVB[2][64];
static u32 g_CurrentVBVertexCount;
static u32 g_VBSize;
static u32 g_VBMaxVertexCount;
static void *g_pVBData;
static void *g_debrisUploadBuffer;
static nunativedebrisdata_s *g_ParticleGroup;
static void *g_lastPartEffect;
static u32 g_FrameVertexCount;
static u32 g_writeBufferIndex;
static u32 g_readBufferIndex = 1;

static u32 g_IdealDynamicVBSize = 0xc800;

static void NuIOSBindVAO(u32 vao) {
    if (vao != g_lastBoundVAO) {
        g_lastBoundVAO = vao;
    }
}

static void NuDisplayListSetNext(NUDISPLAYLISTITEM *item, void *next) {
    item->next = next;
}

static void NuDisplayListSetID_CALL(NUDISPLAYLISTITEM *item) {
    item->id = 3;
}

static NUDISPLAYLISTITEM *NuDisplayListAddItem(NUDISPLAYLIST *list, u8 type, void *next) {
    NUDISPLAYLISTITEM *item = list->items;
    item->type = type;
    NuDisplayListSetID_CALL(item);
    NuDisplayListSetNext(item, next);
    list->items = reinterpret_cast<NUDISPLAYLISTITEM *>(reinterpret_cast<u8 *>(list->items) + 0x10);
    return reinterpret_cast<NUDISPLAYLISTITEM *>(reinterpret_cast<u8 *>(list->items) - 0x10);
}

// The original particle renderer contains its own file-local copies of these
// bind helpers; the geometry renderer's 0x293xxx copies remain in its TU.
struct DebrisVertexAttribRecord {
    u32 gl_type;
    u32 comp_count;
    u8 normalized;
    u8 reserved[3];
    u32 pad;
    u32 byte_offset;
    u32 stride;
};

extern "C" {
    static void NuIOS_BindVertexAttributesInternal(isize data_addr, usize base_vertex, const u32 *format, u32 mask) {
        i32 location = 0;
        mask &= format[0];
        u32 disable = g_activeAttributes & ~mask;
        u32 enable = ~g_activeAttributes & mask;
        g_activeAttributes = mask;
        do {
            if (mask & 1) {
                const DebrisVertexAttribRecord *attribute =
                    reinterpret_cast<const DebrisVertexAttribRecord *>(format + location * 6 + 1);
                if (enable & 1) {
                    glEnableVertexAttribArray(location);
                }
                const void *address = reinterpret_cast<const void *>(data_addr + attribute->byte_offset +
                                                                     base_vertex * attribute->stride);
                glVertexAttribPointer(location, attribute->comp_count, attribute->gl_type, attribute->normalized,
                                      attribute->stride, address);
            } else if (disable & 1) {
                glDisableVertexAttribArray(location);
            }
            ++location;
            mask >>= 1;
            enable >>= 1;
            disable >>= 1;
        } while ((mask | enable | disable) != 0);
    }

    static void NuIOS_BindVertexAttributes(isize, usize base_vertex) {
        const u32 *format = reinterpret_cast<const u32 *>(g_boundVertexFormat);
        NuIOS_BindVertexAttributesInternal(0, base_vertex, format, format[0]);
    }

    static void NuIOS_BindVertexAttributesImmediate(isize, isize data_addr) {
        NuIOSBindVAO(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        const u32 *format = reinterpret_cast<const u32 *>(g_boundVertexFormat);
        NuIOS_BindVertexAttributesInternal(data_addr, 0, format, format[0]);
    }
}

extern "C" void NuInitDebrisRenderer(VARIPTR *buffer, VARIPTR buffer_end) {
    u32 ideal_dynamic_vb_size = g_IdealDynamicVBSize;
    g_VBMaxVertexCount = ideal_dynamic_vb_size / 0x18;
    if (NuIOS_IsLowEndDevice() != 0) {
        g_VBMaxVertexCount >>= 1;
    }
    g_VBSize = g_VBMaxVertexCount * 0x18;

    for (i32 frame = 0; frame < 2; ++frame) {
        for (i32 index = 0; index < 64; ++index) {
            g_DebriSysMemVB[frame][index] = buffer->void_ptr;
            buffer->addr += g_VBSize;
        }
        BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nuptl_android.c", 0x8c);
        NuIOSBindVAO(0);
        glGenBuffers(4, &g_DebriVB[frame * 4]);
        for (i32 index = 0; index < 4; ++index) {
            glBindBuffer(GL_ARRAY_BUFFER, g_DebriVB[frame * 4 + index]);
            glBufferData(GL_ARRAY_BUFFER, g_VBSize, NULL, GL_STREAM_DRAW);
        }
        EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nuptl_android.c", 0x96);
    }
    g_debrisUploadBuffer = buffer->void_ptr;
    buffer->addr += g_VBSize;
    g_pVBData = g_debrisUploadBuffer;
}

i32 NuDebrisRendererNextBuffer() {
    if (g_UseSysMemVB == 0 && g_pVBData != NULL && g_CurrentVBVertexCount != 0) {
        BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nuptl_android.c", 0xc1);
        glBindBuffer(GL_ARRAY_BUFFER, g_DebriVB[g_writeBufferIndex * 4 + g_CurrentDebriVBIndex]);
        glBufferData(GL_ARRAY_BUFFER, g_VBSize, NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, g_CurrentVBVertexCount * sizeof(debris_vertex_s), g_pVBData);
        g_pVBData = NULL;
        EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nuptl_android.c", 0xc9);
    }

    if (g_UseSysMemVB != 0) {
        if (g_CurrentDebriVBIndex + 1 >= 64) {
            return 0;
        }
        g_CurrentDebriVBIndex = (g_CurrentDebriVBIndex + 1) & 63;
    } else {
        if (g_CurrentDebriVBIndex + 1 >= 4) {
            g_CurrentDebriVBIndex = 0;
            g_UseSysMemVB = 1;
        } else {
            g_CurrentDebriVBIndex = (g_CurrentDebriVBIndex + 1) & 3;
        }
    }

    g_CurrentVBVertexCount = 0;
    g_pVBData = g_UseSysMemVB == 0 ? g_debrisUploadBuffer : g_DebriSysMemVB[g_writeBufferIndex][g_CurrentDebriVBIndex];
    return 1;
}

// original 0x296f35

void NuDebrisRendererFlushBuffers(void) {
    if ((g_UseSysMemVB == 0) && (g_pVBData != NULL) && (g_CurrentVBVertexCount != 0)) {
        BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nuptl_android.c", 0xa4);
        glBindBuffer(GL_ARRAY_BUFFER, g_DebriVB[g_writeBufferIndex * 4 + g_CurrentDebriVBIndex]);
        glBufferData(GL_ARRAY_BUFFER, g_VBSize, NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, g_CurrentVBVertexCount * 0x18, g_pVBData);
        EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nuptl_android.c", 0xab);
    }
    g_pVBData = NULL;
    g_CurrentDebriVBIndex = 0;
    g_UseSysMemVB = 0;
    if (g_forceSysMemVbs != 0) {
        g_UseSysMemVB = 1;
    }
    g_CurrentVBVertexCount = 0;
    g_lastPartEffect = NULL;
    g_FrameVertexCount = 0;
    g_readBufferIndex = g_writeBufferIndex;
    g_writeBufferIndex = (g_writeBufferIndex + 1) & 1;
}

extern "C" void NuRndrSetParticleRotation(NUMTX *rotation) {
    NuRndr_DebrisRotMtxPtr = rotation;
}
extern "C" void NuRndrParticleGroup(uv1debdata *chunks, PartHeader *header, NUMTL *material, f32 time, NUMTX *matrix,
                                    i32 particle_type, f32 a, f32 b, f32 c, f32 near_clip) {
    if (header == NULL) {
        g_lastPartEffect = NULL;
        return;
    }
    if (!(material == NULL || material->particle_type_tag == -105)) {
        if (header != g_lastPartEffect) {
            if (material->attribs.unknown_2_1_2 != 2 || material->attribs.unknown_2_4 == 0) {
                material->attribs.unknown_2_1_2 = 2;
                material->attribs.unknown_2_4 = 1;
                NuMtlUpdate(material);
            }
            if (NuRndr_DebrisRotMtxPtr == NULL) {
                NuMtxCalcDebrisFaceOn(&NuRndr_DebrisMtx);
            } else {
                NuRndr_DebrisMtx = *NuRndr_DebrisRotMtxPtr;
            }

            NUCAMERA camera;
            NuCameraGet(&camera);
            NuRndr_DebrisPlane.x = camera.mtx.m20;
            NuRndr_DebrisPlane.y = camera.mtx.m21;
            NuRndr_DebrisPlane.z = camera.mtx.m22;
            NuRndr_DebrisPlane.w =
                -(camera.mtx.m30 * camera.mtx.m20 + camera.mtx.m31 * camera.mtx.m21 + camera.mtx.m32 * camera.mtx.m22);
            header->last_render_time = time;

            VARIPTR *buffer = NuDisplayListGetBuffer();
            g_ParticleGroup = static_cast<nunativedebrisdata_s *>(buffer->void_ptr);
            buffer->addr += sizeof(nunativedebrisdata_s);
            g_ParticleGroup->vertex_buffer_index = static_cast<u8>(g_CurrentDebriVBIndex);
            g_ParticleGroup->use_system_memory_vb = g_UseSysMemVB;
            g_ParticleGroup->first_vertex = static_cast<i32>(g_CurrentVBVertexCount);
            g_ParticleGroup->vertex_count = 0;
            g_ParticleGroup->material = material;
            if (g_pVBData == NULL) {
                g_pVBData = g_debrisUploadBuffer;
            }
            AddParticleGroupToDisplayList(g_ParticleGroup);
            g_lastPartEffect = header;
        }

        dma_particle_chunk_s *chunk = reinterpret_cast<dma_particle_chunk_s *>(chunks);
        i32 done = 0;
        i32 count = 0;
        while (done == 0) {
            i32 command = static_cast<i8>(chunk->command);
            dma_particle_chunk_s *next = chunk->next;
            switch (command) {
                case 0x4e:
                    if (next != NULL) {
                        BuildDebrisVerts(header, reinterpret_cast<uv1debdata *>(chunk), material, time, matrix,
                                         particle_type, a, b, c, near_clip);
                        chunk = next;
                    }
                    break;
                case 0x52:
                    BuildDebrisVerts(header, reinterpret_cast<uv1debdata *>(chunk), material, time, matrix,
                                     particle_type, a, b, c, near_clip);
                    done = 1;
                    break;
            }
            if (++count > 0x100)
                break;
        }
    }
}
void BuildDebrisVerts(PartHeader *header, uv1debdata *chunk_data, NUMTL *material, f32 time, NUMTX *matrix,
                      i32 particle_type, f32, f32, f32, f32 near_clip) {
    f32 u0;
    f32 v0;
    f32 u1;
    f32 v1;
    if (material->particle_type_tag == -105) {
        u0 = 0.0f;
        v0 = 0.0f;
        u1 = 1.0f;
        v1 = 1.0f;
    } else {
        u0 = header->texture_u0;
        v0 = header->texture_v0;
        u1 = header->texture_u1;
        v1 = header->texture_v1;
    }
    dma_particle_chunk_s *chunk = reinterpret_cast<dma_particle_chunk_s *>(chunk_data);
    u32 emitted = 0;

    for (i32 particle_index = 0; particle_index < 32; ++particle_index) {
        const dma_particle_s &particle = chunk->particles[particle_index];
        const f32 age = time - particle.start_time;
        const f32 frame_position = particle.inverse_lifetime * age;
        const u32 frame_index = static_cast<u32>(frame_position);
        if (!(frame_index >= 63)) {
            const f32 fraction = particle.inverse_lifetime * age - static_cast<f32>(frame_index);
            const debris_particle_frame_s &first = header->frames[frame_index];
            const debris_particle_frame_s &second = header->frames[frame_index + 1];
            NUVEC position = {
                particle.position.x + particle.momentum.x * age,
                particle.position.y + particle.momentum.y * age + header->gravity * age * age * 0.945f,
                particle.position.z + particle.momentum.z * age,
            };
            NUVEC rotated = {
                position.x * matrix->m00 + position.y * matrix->m10 + position.z * matrix->m20,
                position.x * matrix->m01 + position.y * matrix->m11 + position.z * matrix->m21,
                position.x * matrix->m02 + position.y * matrix->m12 + position.z * matrix->m22,
            };
            NuRndr_DebrisMtx.m30 = rotated.x + matrix->m30;
            NuRndr_DebrisMtx.m31 = rotated.y + matrix->m31;
            NuRndr_DebrisMtx.m32 = rotated.z + matrix->m32;
            if (particle_type == 6 || particle_type == 7) {
                NuRndrParticleSetRepeat(reinterpret_cast<NUVEC *>(&NuRndr_DebrisMtx.m30));
            }

            const f32 plane_distance =
                NuRndr_DebrisPlane.w +
                (NuRndr_DebrisMtx.m32 * NuRndr_DebrisPlane.z +
                 (NuRndr_DebrisMtx.m30 * NuRndr_DebrisPlane.x + NuRndr_DebrisMtx.m31 * NuRndr_DebrisPlane.y));
            if (plane_distance < near_clip) {
                continue;
            }

            if (g_CurrentVBVertexCount + emitted + 6 > g_VBMaxVertexCount) {
                g_ParticleGroup->vertex_count += static_cast<i32>(emitted);
                g_FrameVertexCount += emitted;
                g_CurrentVBVertexCount += emitted;
                emitted = 0;
                if (NuDebrisRendererNextBuffer() == 0) {
                    return;
                }
                VARIPTR *buffer = NuDisplayListGetBuffer();
                nunativedebrisdata_s *packet = static_cast<nunativedebrisdata_s *>(buffer->void_ptr);
                packet->material = g_ParticleGroup->material;
                packet->vertex_buffer_index = static_cast<u8>(g_CurrentDebriVBIndex);
                packet->use_system_memory_vb = g_UseSysMemVB;
                packet->first_vertex = static_cast<i32>(g_CurrentVBVertexCount);
                packet->vertex_count = 0;
                buffer->addr += sizeof(*packet);
                g_ParticleGroup = packet;
                AddParticleGroupToDisplayList(g_ParticleGroup);
            }

            const f32 inverse_fraction = 1.0f - fraction;
            // Retail uses 16-byte records. The active XYZ view is passed to
            // the three-component helper; the fourth component is untouched.
            union {
                NUVEC4 xyzw;
                NUVEC xyz;
            } corners[4];
            corners[0].xyz = {first.position.x * inverse_fraction + second.position.x * fraction,
                              first.position.y * inverse_fraction + second.position.y * fraction,
                              first.position.z * inverse_fraction + second.position.z * fraction};
            corners[1].xyz = {first.extent.x * inverse_fraction + second.extent.x * fraction,
                              first.extent.y * inverse_fraction + second.extent.y * fraction,
                              first.extent.z * inverse_fraction + second.extent.z * fraction};
            corners[2].xyz = {first.texture_offset.x * inverse_fraction + second.texture_offset.x * fraction,
                              first.texture_offset.y * inverse_fraction + second.texture_offset.y * fraction,
                              first.texture_offset.z * inverse_fraction + second.texture_offset.z * fraction};
            corners[3].xyz.x = corners[0].xyz.x + (corners[1].xyz.x - corners[2].xyz.x);
            corners[3].xyz.y = corners[0].xyz.y + (corners[1].xyz.y - corners[2].xyz.y);
            corners[3].xyz.z = corners[0].xyz.z + (corners[1].xyz.z - corners[2].xyz.z);
            for (i32 corner = 0; corner < 4; ++corner) {
                NuVecMtxTransform(&corners[corner].xyz, &corners[corner].xyz, &NuRndr_DebrisMtx);
            }

            debris_vertex_s *vertices = static_cast<debris_vertex_s *>(g_pVBData) + g_CurrentVBVertexCount + emitted;
            const u32 colour = first.colour;
            vertices[0] = {corners[0].xyz, colour, u0, v1};
            vertices[1] = {corners[2].xyz, colour, u1, v1};
            vertices[2] = {corners[1].xyz, colour, u1, v0};
            vertices[3] = vertices[0];
            vertices[4] = vertices[2];
            vertices[5] = {corners[3].xyz, colour, u0, v0};
            emitted += 6;
        }
    }

    g_ParticleGroup->vertex_count += static_cast<i32>(emitted);
    g_CurrentVBVertexCount += emitted;
    g_FrameVertexCount += emitted;
}
void AddParticleGroupToDisplayList(nunativedebrisdata_s *group) {
    NUDISPLAYLIST *list = group->material->display_list;
    list->dlist->clip_materials = 1;
    list->dlist->mtl_used[list->dlist->current_buffer][list->mtl_id / 8] |= 1 << (list->mtl_id % 8);

    DisplayListUpdateRenderState(list, &render_state);
    NuDisplayListLinkItems(list, 1);
    NuDisplayListAddItem(list, 0xa7, group);
}
void NuIOSDLDebrisCallback(void *data) {
    nunativedebrisdata_s *packet = static_cast<nunativedebrisdata_s *>(data);
    if (packet->vertex_count == 0) {
        return;
    }
    if (packet->use_system_memory_vb != 0) {
        g_boundVertexFormat = reinterpret_cast<usize>(g_nuDebrisVertexFormat);
        NuIOS_BindVertexAttributesImmediate(
            0, reinterpret_cast<isize>(g_DebriSysMemVB[g_readBufferIndex][packet->vertex_buffer_index]));
        glDrawArrays(GL_TRIANGLES, packet->first_vertex, packet->vertex_count);
    } else {
        NuIOSBindVAO(0);
        g_boundVertexFormat = reinterpret_cast<usize>(g_nuDebrisVertexFormat);
        glBindBuffer(GL_ARRAY_BUFFER, g_DebriVB[g_readBufferIndex * 4 + packet->vertex_buffer_index]);
        NuIOS_BindVertexAttributes(0, 0);
        glDrawArrays(GL_TRIANGLES, packet->first_vertex, packet->vertex_count);
    }
}

extern "C" {
    dma_particle_chunk_s *CreateDmaParticleSet(void *memory, i32 *size) {
        dma_particle_chunk_s *chunk = static_cast<dma_particle_chunk_s *>(memory);
        u8 *cursor = static_cast<u8 *>(memory);
        chunk->command = 0x52;
        chunk->next = NULL;
        cursor += 0x10;
        reinterpret_cast<u32 *>(cursor)[1] = 0;
        reinterpret_cast<u32 *>(cursor)[2] = 0;
        reinterpret_cast<u32 *>(cursor)[3] = 0;
        reinterpret_cast<u32 *>(cursor)[4] = 0;
        cursor += 0x10;
        for (i32 i = 0; i < 32; ++i) {
            dma_particle_s *particle = reinterpret_cast<dma_particle_s *>(cursor);
            particle->position.x = 1.0f;
            particle->position.y = 2.0f;
            particle->position.z = 3.0f;
            particle->momentum.x = 4.0f;
            particle->momentum.y = 5.0f;
            particle->momentum.z = 6.0f;
            particle->start_time = -1.0f;
            particle->inverse_lifetime = 128.0f;
            cursor += sizeof(*particle);
        }
        *reinterpret_cast<u32 *>(cursor) = 0;
        cursor += sizeof(u32);
        *size = cursor - reinterpret_cast<u8 *>(chunk);
        return chunk;
    }

    dma_particle_chunk_s *CreateDmaParticleSetGlass(void *memory, i32 *size) {
        return CreateDmaParticleSet(memory, size);
    }

    PartHeader *CreateDmaPartEffectList(void *memory, i32 *size) {
        u8 *cursor = reinterpret_cast<u8 *>(ALIGN(reinterpret_cast<usize>(memory), 0x10));
        u8 *start = cursor;
        PartHeader *header = reinterpret_cast<PartHeader *>(cursor);
        debris_particle_frame_s *frame = header->frames;
        frame += 64;
        cursor = reinterpret_cast<u8 *>(frame);
        *size = cursor - start;
        return reinterpret_cast<PartHeader *>(start);
    }

    void LinkDmaParticalSets(dma_particle_chunk_s **chunks, i32 count) {
        dma_particle_chunk_s *chunk = chunks[count - 1];
        chunk->command = 0x52;
        chunk->next = NULL;
        for (i32 i = count - 2; i >= 0; --i) {
            chunk = chunks[i];
            chunk->command = 0x4e;
            chunk->next = chunks[i + 1];
        }
    }
}

void NuRndrParticleSetRepeat(NUVEC *position) {
    NUVEC repeat = {
        (position->x - NuRndrDebBase.x) / NuRndrDebRange.x,
        (position->y - NuRndrDebBase.y) / NuRndrDebRange.y,
        (position->z - NuRndrDebBase.z) / NuRndrDebRange.z,
    };
    repeat.x -= static_cast<f32>(static_cast<i32>(repeat.x + 65536.0f) - 65536);
    repeat.y -= static_cast<f32>(static_cast<i32>(repeat.y + 65536.0f) - 65536);
    repeat.z -= static_cast<f32>(static_cast<i32>(repeat.z + 65536.0f) - 65536);
    position->x = NuRndrDebBase.x + repeat.x * NuRndrDebRange.x;
    position->y = NuRndrDebBase.y + repeat.y * NuRndrDebRange.y;
    position->z = NuRndrDebBase.z + repeat.z * NuRndrDebRange.z;
}
void NuRndrParticleDraw(variptr_u *, PartHeader *header, uv1debdata *data, float time, numtx_s *matrix, i32 *,
                        float clip_distance, i32 mode, numtl_s *, float, float) {
    dma_particle_s *particle = reinterpret_cast<dma_particle_chunk_s *>(data)->particles;
    f32 half_gravity = header->gravity / 2.0f;
    for (i32 index = 0; index != 32; ++index, ++particle) {
        f32 age = time - particle->start_time;
        u32 frame = static_cast<u32>(particle->inverse_lifetime * age);
        if (frame > 62) {
            continue;
        }
        u32 next_frame = frame + 1;
        f32 fraction = particle->inverse_lifetime * age - static_cast<f32>(frame);
        debris_particle_frame_s *first = &header->frames[frame];
        debris_particle_frame_s *second = &header->frames[next_frame];
        NUVEC position;
        position.x = particle->position.x + particle->momentum.x * age;
        position.y = particle->position.y + particle->momentum.y * age + header->gravity * age * age * 0.945f;
        position.z = particle->position.z + particle->momentum.z * age;
        NUVEC rotated;
        rotated.x = position.x * matrix->m00 + position.y * matrix->m10 + position.z * matrix->m20;
        rotated.y = position.x * matrix->m01 + position.y * matrix->m11 + position.z * matrix->m21;
        rotated.z = position.x * matrix->m02 + position.y * matrix->m12 + position.z * matrix->m22;
        NuRndr_DebrisMtx.m30 = rotated.x + matrix->m30;
        NuRndr_DebrisMtx.m31 = rotated.y + matrix->m31;
        NuRndr_DebrisMtx.m32 = rotated.z + matrix->m32;
        if (mode == 6 || mode == 7) {
            NuRndrParticleSetRepeat(reinterpret_cast<NUVEC *>(&NuRndr_DebrisMtx.m30));
        }
        f32 distance = NuRndr_DebrisPlane.w +
                       (NuRndr_DebrisMtx.m32 * NuRndr_DebrisPlane.z +
                        (NuRndr_DebrisMtx.m30 * NuRndr_DebrisPlane.x + NuRndr_DebrisMtx.m31 * NuRndr_DebrisPlane.y));
        if (clip_distance > distance) {
            continue;
        }
        NUVEC offset;
        offset.x = first->position.x * (1.0f - fraction) + second->position.x * fraction;
        offset.y = first->position.y * (1.0f - fraction) + second->position.y * fraction;
        offset.z = first->position.z * (1.0f - fraction) + second->position.z * fraction;
        NUVEC extent;
        extent.x = first->extent.x * (1.0f - fraction) + second->extent.x * fraction;
        extent.y = first->extent.y * (1.0f - fraction) + second->extent.y * fraction;
        extent.z = first->extent.z * (1.0f - fraction) + second->extent.z * fraction;
        NUVEC texture_offset;
        texture_offset.x = first->texture_offset.x * (1.0f - fraction) + second->texture_offset.x * fraction;
        texture_offset.y = first->texture_offset.y * (1.0f - fraction) + second->texture_offset.y * fraction;
        texture_offset.z = first->texture_offset.z * (1.0f - fraction) + second->texture_offset.z * fraction;
        NuVecMtxTransform(&offset, &offset, &NuRndr_DebrisMtx);
        NuVecMtxTransform(&extent, &extent, &NuRndr_DebrisMtx);
        NuVecMtxTransform(&texture_offset, &texture_offset, &NuRndr_DebrisMtx);
        f32 red = static_cast<f32>(reinterpret_cast<const u8 *>(&first->colour)[0]) / 255.0f;
        f32 green = static_cast<f32>(reinterpret_cast<const u8 *>(&first->colour)[1]) / 255.0f;
        f32 blue = static_cast<f32>(reinterpret_cast<const u8 *>(&first->colour)[2]) / 255.0f;
        f32 alpha = static_cast<f32>(reinterpret_cast<const u8 *>(&first->colour)[3]) / 255.0f;
    }
}

extern "C" void GenericDebinfoDmaTypeUpdate(debinftype *effect) {
    if (effect->native_data == NULL) {
        if (freeDmaDebType >= EDPP_MAX_DMADEBTYPES) {
            return;
        }
        effect->native_data = DmaDebTypes[freeDmaDebType++];
    }

    const bool fly = effect != NULL && effect->name != NULL && NuStrCmp(effect->name, "FLY") == 0 &&
                     static_cast<u32>(effect->texture_u0) == 0x8003c &&
                     static_cast<u32>(effect->texture_v0) == 0x80100 &&
                     static_cast<u32>(effect->texture_u1) == 0x8005e && static_cast<u32>(effect->texture_v1) == 0x80082;
    if (fly) {
        for (u32 i = 0; i < 8; ++i) {
            effect->width_keys[i].value = effect->height_keys[i].value;
            effect->height_keys[i].value += effect->height_keys[i].value;
            effect->alpha_keys[i].value *= 1.5f;
            effect->rotation_keys[i].value *= 1.5f;
        }
        effect->texture_u0 = static_cast<f32>(static_cast<u32>(effect->texture_u0) & ~0x1ffU);
        effect->texture_u0 += 63.75f;
        effect->texture_v0 = static_cast<f32>(static_cast<u32>(effect->texture_v0) & ~0x1ffU);
        effect->texture_v0 += 127.5f;
        effect->texture_u1 = static_cast<f32>(static_cast<u32>(effect->texture_u1) & ~0x1ffU);
        effect->texture_u1 += 95.625f;
        effect->texture_v1 = static_cast<f32>(static_cast<u32>(effect->texture_v1) & ~0x1ffU);
        effect->texture_v1 += 191.25f;
    }
    PartHeader *header = effect->native_data;
    header->gravity = effect->field_0a0;
    header->texture_u0 = static_cast<f32>(static_cast<u32>(effect->texture_u0) & 0x1ffU) * (1.0f / 255.0f);
    header->texture_v0 = static_cast<f32>(static_cast<u32>(effect->texture_v0) & 0x1ffU) * (1.0f / 255.0f);
    header->texture_u1 = static_cast<f32>(static_cast<u32>(effect->texture_u1) & 0x1ffU) * (1.0f / 255.0f);
    header->texture_v1 = static_cast<f32>(static_cast<u32>(effect->texture_v1) & 0x1ffU) * (1.0f / 255.0f);

    f32 width, height, rotation, alpha, red, green, blue;
    f32 time, duration, elapsed, wave_x, wave_y;
    f32 position_x, position_y, texture_x, texture_y, edge_x, edge_y;
    i32 i;
    for (i32 frame_index = 0; frame_index < 64; ++frame_index) {
        width = height = rotation = 0.0f;
        red = green = blue = alpha = 0.0f;
        time = static_cast<f32>(frame_index) / 64.0f;
        for (i = 0; i < 7; ++i) {
            if (effect->width_keys[i].time <= time && time <= effect->width_keys[i + 1].time) {
                duration = effect->width_keys[i + 1].time - effect->width_keys[i].time;
                elapsed = time - effect->width_keys[i].time;
                if (elapsed == 0.0f)
                    width = effect->width_keys[i].value;
                else
                    width = effect->width_keys[i].value +
                            (elapsed / duration) * (effect->width_keys[i + 1].value - effect->width_keys[i].value);
                break;
            }
        }
        for (i = 0; i < 7; ++i) {
            if (effect->height_keys[i].time <= time && time <= effect->height_keys[i + 1].time) {
                duration = effect->height_keys[i + 1].time - effect->height_keys[i].time;
                elapsed = time - effect->height_keys[i].time;
                if (elapsed == 0.0f)
                    height = effect->height_keys[i].value;
                else
                    height = effect->height_keys[i].value +
                             (elapsed / duration) * (effect->height_keys[i + 1].value - effect->height_keys[i].value);
                break;
            }
        }
        for (i = 0; i < 7; ++i) {
            if (effect->rotation_keys[i].time <= time && time <= effect->rotation_keys[i + 1].time) {
                duration = effect->rotation_keys[i + 1].time - effect->rotation_keys[i].time;
                elapsed = time - effect->rotation_keys[i].time;
                if (elapsed == 0.0f)
                    rotation = effect->rotation_keys[i].value;
                else
                    rotation =
                        effect->rotation_keys[i].value +
                        (elapsed / duration) * (effect->rotation_keys[i + 1].value - effect->rotation_keys[i].value);
                break;
            }
        }
        wave_x = effect->jib_x_amplitude * NU_SIN_LUT(65536.0f * (effect->jib_x_frequency * time));
        wave_y = effect->jib_y_amplitude * NU_SIN_LUT(65536.0f * (effect->jib_y_frequency * time));
        debris_particle_frame_s &frame = effect->native_data->frames[frame_index];
        position_x =
            -(width / 4.0f) * NU_SIN_LUT(rotation + 16384.0f) - (height / 4.0f) * NU_SIN_LUT(rotation) + wave_x;
        position_y = (width / 4.0f) * NU_SIN_LUT(rotation) - (height / 4.0f) * NU_SIN_LUT(rotation + 16384.0f) + wave_y;
        texture_x = (width / 4.0f) * NU_SIN_LUT(rotation + 16384.0f) - (height / 4.0f) * NU_SIN_LUT(rotation) + wave_x;
        texture_y = -(width / 4.0f) * NU_SIN_LUT(rotation) - (height / 4.0f) * NU_SIN_LUT(rotation + 16384.0f) + wave_y;
        edge_x = (height / 2.0f) * NU_SIN_LUT(rotation);
        edge_y = (height / 2.0f) * NU_SIN_LUT(rotation + 16384.0f);
        frame.position.x = position_x / 2048.0f;
        frame.position.y = position_y / 2048.0f;
        frame.position.z = 0.0f;
        frame.texture_offset.x = texture_x / 2048.0f;
        frame.texture_offset.y = texture_y / 2048.0f;
        frame.texture_offset.z = 0.0f;
        frame.extent.x = (texture_x + edge_x) / 2048.0f;
        frame.extent.y = (texture_y + edge_y) / 2048.0f;
        frame.extent.z = 0.0f;
        for (i = 0; i < 7; ++i) {
            if (effect->colour_keys[i].time <= time && time <= effect->colour_keys[i + 1].time) {
                duration = effect->colour_keys[i + 1].time - effect->colour_keys[i].time;
                elapsed = time - effect->colour_keys[i].time;
                if (elapsed == 0.0f) {
                    red = effect->colour_keys[i].red;
                    green = effect->colour_keys[i].green;
                    blue = effect->colour_keys[i].blue;
                } else {
                    red = effect->colour_keys[i].red +
                          (elapsed / duration) *
                              static_cast<i32>(effect->colour_keys[i + 1].red - effect->colour_keys[i].red);
                    green = effect->colour_keys[i].green +
                            (elapsed / duration) *
                                static_cast<i32>(effect->colour_keys[i + 1].green - effect->colour_keys[i].green);
                    blue = effect->colour_keys[i].blue +
                           (elapsed / duration) *
                               static_cast<i32>(effect->colour_keys[i + 1].blue - effect->colour_keys[i].blue);
                }
                break;
            }
        }
        red += red;
        if (red > 255.0f)
            red = 255.0f;
        green += green;
        if (green > 255.0f)
            green = 255.0f;
        blue += blue;
        if (blue > 255.0f)
            blue = 255.0f;
        for (i = 0; i < 7; ++i) {
            if (effect->alpha_keys[i].time <= time && time <= effect->alpha_keys[i + 1].time) {
                duration = effect->alpha_keys[i + 1].time - effect->alpha_keys[i].time;
                elapsed = time - effect->alpha_keys[i].time;
                if (elapsed == 0.0f)
                    alpha = effect->alpha_keys[i].value;
                else
                    alpha = effect->alpha_keys[i].value +
                            (elapsed / duration) * (effect->alpha_keys[i + 1].value - effect->alpha_keys[i].value);
                break;
            }
        }
        frame.colour = (static_cast<u32>(static_cast<i32>(blue)) << 16) |
                       (static_cast<u32>(static_cast<i32>(green)) << 8) | static_cast<u32>(red) |
                       (static_cast<u32>(alpha) << 24);
    }
}

// Original provides only the mangled spelling (_Z28NuDebrisRendererFlushBuffersv).
