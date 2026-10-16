#include "decomp.h"
#include "nu2api/nu3d/android/nugscn_android.h"

#include <GLES2/gl2.h>
#include <string.h>

#include "legoapi/characters/core/character.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/numath.h"

i32 g_vaoLifetimeMutex = -1;
u32 g_lastBoundVAO;

static void NuIOSBindVAO(u32 vao_handle) {
    if (vao_handle != g_lastBoundVAO) {
        g_lastBoundVAO = vao_handle;
    }
}

void *NuGScnBufferAllocAligned(i32, i32) {
    STUBBED();
    return NULL;
}

extern "C" void NuGScnRndr3(NUGSCN *scene) {
    NuDisplaySceneRndr(scene->display_list);
}

void NuGScnLoadShadersPS(char *, variptr_u *, variptr_u) {
    STUBBED();
}

static u32 UploadDataToGLBuffer(NUFILE file, u32 size, GLenum target, usize *buffer_handle, VARIPTR *buf,
                                VARIPTR buf_end) {
    u32 n;
    u32 largest_chunk;
    u32 chunk_limit;
    u32 buf_size;
    u32 max_chunk_size;
    u32 chunk_size;

    BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nugscn_android.c", 0x56);
    glGenBuffers(1, reinterpret_cast<GLuint *>(buffer_handle));
    NuIOSBindVAO(0);
    glBindBuffer(target, *buffer_handle);
    glBufferData(target, size, 0, GL_STATIC_DRAW);
    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nugscn_android.c", 0x5d);

    if (bgProcIsBgThread()) {
        NuIOS_YieldThread();
    }

    chunk_limit = g_loadingCharacterInHub != 0 ? 0x4000 : 0x10000;
    buf_size = buf_end.char_ptr - buf->char_ptr;
    max_chunk_size = NuMin(chunk_limit, buf_size);
    largest_chunk = 0;

    for (n = 0; n < size; n += chunk_size) {
        chunk_size = NuMin(max_chunk_size, size - n);
        NuFileRead(file, buf->void_ptr, chunk_size);

        BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nugscn_android.c", 0x73);
        NuIOSBindVAO(0);
        glBindBuffer(target, *buffer_handle);
        if (chunk_size == size) {
            glBufferData(target, chunk_size, buf->void_ptr, GL_STATIC_DRAW);
        } else {
            glBufferSubData(target, n, chunk_size, buf->void_ptr);
        }
        EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nugscn_android.c", 0x80);

        if (bgProcIsBgThread()) {
            NuIOS_YieldThread();
        }
        largest_chunk = NuMax(largest_chunk, chunk_size);
    }
    return largest_chunk;
}

SAGA_HOST_WEAK i32 NuGScnReadTexturesPS(i32 file, variptr_u *buf, variptr_u buf_end) {
    u32 i;
    i32 bytes_read;
    void *data;
    i32 skip;
    i32 not_pvrtc;
    i32 texture_header[6];
    NUNATIVETEX texture;

    (void)buf_end;
    bytes_read = 0;
    bytes_read += NuFileRead(file, &g_VideoResHeader.ntextures, sizeof(g_VideoResHeader.ntextures));

    g_VideoResHeader.texture_hashes = g_VideoResHeader.ntextures & 0x8000;
    g_VideoResHeader.ntextures &= 0x7fff;
    g_VideoResHeader.textures = buf->u32_ptr;
    buf->u32_ptr += g_VideoResHeader.ntextures;
    memset(g_VideoResHeader.textures, 0, (usize)g_VideoResHeader.ntextures * sizeof(u32));

    if (g_VideoResHeader.texture_hashes != 0) {
        bytes_read += NuFileRead(file, g_VideoResHeader.textures, g_VideoResHeader.ntextures * sizeof(u32));
        return bytes_read;
    }

    for (i = 0; i != g_VideoResHeader.ntextures; i++) {
        data = NULL;
        bytes_read += NuFileRead(file, texture_header, sizeof(texture_header));
        if (texture_header[5] == 0) {
            g_VideoResHeader.textures[i] = 0;
            if (texture_header[0] < 0) {
                g_VideoResHeader.textures[i + 1] = 0;
                g_VideoResHeader.textures[i + 2] = 0;
                g_VideoResHeader.textures[i + 3] = 0;
                g_VideoResHeader.textures[i + 4] = 0;
                g_VideoResHeader.textures[i + 5] = 0;
                i += 5;
            }
            continue;
        } else if (texture_header[0] == 0) {
            g_VideoResHeader.textures[i] = 0;
        } else {
            skip = 0;
            data = buf->void_ptr;
            buf->addr += texture_header[5];
            bytes_read += NuFileRead(file, data, texture_header[5]);
            if (skip == 0) {
                NudxFw_D3DBeginCriticalSection();
                if (texture_header[0] < 0) {
                    texture_header[0] = -texture_header[0];
                    texture.image_data = data;
                    texture.size = texture_header[5];
                    NuTexCreatePS(&texture, true);
                    g_VideoResHeader.textures[i] = texture.platform.gl_tex;
                    g_VideoResHeader.textures[i + 1] = 0;
                    g_VideoResHeader.textures[i + 2] = 0;
                    g_VideoResHeader.textures[i + 3] = 0;
                    g_VideoResHeader.textures[i + 4] = 0;
                    g_VideoResHeader.textures[i + 5] = 0;
                    i += 5;
                } else {
                    not_pvrtc = 1;
                    texture.image_data = data;
                    texture.size = texture_header[5];
                    NuTexCreatePS(&texture, true);
                    g_VideoResHeader.textures[i] = texture.platform.gl_tex;
                }
                NudxFw_D3DEndCriticalSection();
            }
            buf->addr -= texture_header[5];
            continue;
        }
    }
    return bytes_read;
}

i32 NuGScnUploadGfxDataFromFilePS(VARIPTR *buf, VARIPTR buf_end, i32 file) {
    VARIPTR max_buf = *buf;
    i32 section_size = 0;
    i32 bytes_read = 0;
    u32 i;
    u32 j;
    i32 padding_read_count;
    i32 vertex_flags;
    i32 vertex_entry_flags;
    u32 keep_in_memory;
    u32 vertex_largest;
    i32 index_flags;
    i32 index_entry_flags;
    u32 index_largest;
    i32 total_size;
    i32 vertex_size;
    i32 index_size;
    u8 padding;

    memset(&g_VideoResHeader, 0, sizeof(g_VideoResHeader));
    bytes_read += NuFileRead(file, &section_size, sizeof(section_size));
    bytes_read += NuGScnReadTexturesPS(file, buf, buf_end);

    vertex_flags = 0;
    bytes_read += NuFileRead(file, &g_VideoResHeader.nvertex_buffers, sizeof(g_VideoResHeader.nvertex_buffers));
    g_VideoResHeader.vertex_buffers = static_cast<usize *>(buf->void_ptr);
    buf->addr += g_VideoResHeader.nvertex_buffers * sizeof(usize);
    for (i = 0; i != g_VideoResHeader.nvertex_buffers; i++) {
        vertex_entry_flags = 0;
        vertex_size = 0;
        bytes_read += NuFileRead(file, &vertex_size, sizeof(vertex_size));
        if (vertex_size == 0) {
            g_VideoResHeader.vertex_buffers[i] = 0;
            continue;
        }

        keep_in_memory = vertex_size & 0x80000000;
        vertex_size &= 0x7fffffff;
        if (keep_in_memory != 0) {
            g_VideoResHeader.vertex_buffers[i] = buf->addr;
            buf->addr += vertex_size;
            bytes_read += NuFileRead(file, reinterpret_cast<void *>(g_VideoResHeader.vertex_buffers[i]), vertex_size);
        } else {
            vertex_largest = UploadDataToGLBuffer(file, vertex_size, GL_ARRAY_BUFFER,
                                                  &g_VideoResHeader.vertex_buffers[i], buf, buf_end);
            bytes_read += vertex_size;
            max_buf.addr = NuMax(max_buf.addr, buf->addr + vertex_largest);
        }
    }

    index_flags = 0;
    bytes_read += NuFileRead(file, &g_VideoResHeader.nindex_buffers, sizeof(g_VideoResHeader.nindex_buffers));
    g_VideoResHeader.index_buffers = static_cast<usize *>(buf->void_ptr);
    buf->addr += g_VideoResHeader.nindex_buffers * sizeof(usize);
    for (j = 0; j != g_VideoResHeader.nindex_buffers; j++) {
        index_entry_flags = 0;
        index_size = 0;
        bytes_read += NuFileRead(file, &index_size, sizeof(index_size));
        if (index_size == 0) {
            g_VideoResHeader.index_buffers[j] = 0;
            continue;
        }
        index_largest = UploadDataToGLBuffer(file, index_size, GL_ELEMENT_ARRAY_BUFFER,
                                             &g_VideoResHeader.index_buffers[j], buf, buf_end);
        bytes_read += index_size;
        max_buf.addr = NuMax(max_buf.addr, buf->addr + index_largest);
    }

    total_size = section_size + 4;
    if (bytes_read < total_size) {
        for (padding_read_count = 0; padding_read_count != total_size - bytes_read; padding_read_count++) {
            NuFileRead(file, &padding, 1);
        }
    }
    if (max_buf.addr > buf->addr) {
        memset(buf->void_ptr, 0, max_buf.addr - buf->addr);
    }
    (void)vertex_flags;
    (void)vertex_entry_flags;
    (void)index_flags;
    (void)index_entry_flags;
    return total_size;
}

typedef struct NuGScnTIDPacket {
    i32 mode;
    i32 tid;
    i32 tids[3];
} NuGScnTIDPacket;

extern "C" void NuGScnFixupTIDsPS(NUGSCN *scene) {
    i32 i;
    i32 j;
    NuGScnTIDPacket *packet;
    i32 *tids;

    if (scene->display_list != NULL) {
        for (i = 0; i < scene->display_list->nitems; i++) {
            if (scene->display_list->items[i].type == 0xb0) {
                packet = static_cast<NuGScnTIDPacket *>(scene->display_list->items[i].next);
                if (packet->mode == 2) {
                    packet->mode = 1;
                    for (j = 0; j < 3; j++) {
                        packet->tids[j] = NuGScnFixupTID(scene, packet->tids[j]);
                    }
                }
                packet->tid = NuGScnFixupTID(scene, packet->tid);
            } else if (scene->display_list->items[i].type == 0xae || scene->display_list->items[i].type == 0xaf) {
                tids = static_cast<i32 *>(scene->display_list->items[i].next);
                if (tids != NULL) {
                    for (j = 0; j < 3; j++) {
                        tids[j] = NuGScnFixupTID(scene, tids[j]);
                    }
                }
            }
        }
    }
}

extern "C" void NuGScnRestoreTIDsPS(NUGSCN *scene) {
    i32 i;
    i32 j;
    NuGScnTIDPacket *packet;
    i32 *tids;

    if (scene->display_list != NULL) {
        for (i = 0; i < scene->display_list->nitems; i++) {
            if (scene->display_list->items[i].type == 0xb0) {
                packet = static_cast<NuGScnTIDPacket *>(scene->display_list->items[i].next);
                if (packet->mode == 2) {
                    for (j = 0; j < 3; j++) {
                        packet->tids[j] = NuGScnRestoreTID(scene, packet->tids[j]);
                    }
                }
                packet->tid = NuGScnRestoreTID(scene, packet->tid);
            } else if (scene->display_list->items[i].type == 0xae || scene->display_list->items[i].type == 0xaf) {
                tids = static_cast<i32 *>(scene->display_list->items[i].next);
                if (tids != NULL) {
                    for (j = 0; j < 3; j++) {
                        tids[j] = NuGScnRestoreTID(scene, tids[j]);
                    }
                }
            }
        }
    }
}

static void PreWarmGeomsAndBakeVAOs(nudisplayscene_s *scene, nunativegscene_s *) {
    for (u32 clip_index = 0; clip_index < static_cast<u32>(scene->nclip_objects); ++clip_index) {
        NUCLIPOBJECT *clip = &scene->clip_objects[clip_index];
        for (u32 item_index = 0; item_index < static_cast<u32>(clip->nmaterials); ++item_index) {
            i32 material_index = clip->material_ids[item_index];
            i32 list_index = clip->indices[item_index];
            NUDISPLAYLISTITEM *item = &scene->items[list_index];
            if (item->type != 0x8f) {
                void *geom = item->next;
                BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nugscn_android.c", 0x216);
                numtl_s *mtl = scene->mtls[material_index];
                g_boundMaterial = mtl;
                g_LastMtl = mtl;
                if (mtl->shader_desc.vtx_desc.unknown_2_16) {
                    g_boundVertexFormat = g_nuDebrisVertexFormat;
                } else if (mtl->shader_desc.vtx_desc.unknown_2_32) {
                    g_boundVertexFormat = g_nuFaceOnVertexFormat;
                } else {
                    NuIOS_SetVertexFormat(reinterpret_cast<usize>(mtl->vertex_decl));
                }
                NuIOSDLPreWarmGeomCallback(geom);
                EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nugscn_android.c", 0x22f);
                if (bgProcIsBgThread() != 0) {
                    NuIOS_YieldThread();
                }
            }
        }
    }
}

extern "C" void NuGScnFixupPS(NUGSCN *scene) {
    i32 i;
    i32 ndynamic;
    nunativegscene_s *native_scene;
    NUDISPLAYLISTGEOM *geometry;
    i32 index_index;
    i32 vertex_index;
    i32 dynamic_indices[64];

    native_scene = scene->field437_0x1d0;
    ndynamic = 0;
    for (i = 0; i < native_scene->ngeometries; i++) {
        geometry = native_scene->geometries[i];
        index_index = static_cast<i32>(geometry->index_buffer);
        vertex_index = static_cast<i32>(geometry->vertex_buffer);
        geometry->index_buffer = static_cast<u32>(g_VideoResHeader.index_buffers[index_index]);
        geometry->vertex_buffer = g_VideoResHeader.vertex_buffers[vertex_index];
        if (geometry->vertex_buffer == 0) {
            geometry->index_count = 0;
            geometry->vertex_count = 0;
        }
        if (geometry->immediate != 0) {
            BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nugscn_android.c", 0x25b);
            NuIOSBindVAO(0);
            glGenBuffers(1, reinterpret_cast<GLuint *>(&geometry->vertex_format));
            glBindBuffer(GL_ARRAY_BUFFER, geometry->vertex_format);
            glBufferData(GL_ARRAY_BUFFER, geometry->vertex_count * geometry->vertex_stride, NULL, GL_DYNAMIC_DRAW);
            EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nugscn_android.c", 0x262);
            if (bgProcIsBgThread()) {
                NuIOS_YieldThread();
            }
            dynamic_indices[ndynamic] = vertex_index;
            ndynamic++;
        } else {
            geometry->vertex_format = 0;
        }
    }
    if (scene != NULL && scene->display_list != NULL && scene->display_list->name != NULL &&
        NuStrIStr(scene->display_list->name, "cloudcityescape_c") != NULL) {
        if (native_scene != NULL && native_scene->ngeometries > 0 &&
            native_scene->geometries[native_scene->ngeometries - 1] != NULL) {
            native_scene->geometries[native_scene->ngeometries - 1]->index_count = 0;
            native_scene->geometries[native_scene->ngeometries - 1]->vertex_count = 0;
        }
    }
    for (i = 0; i < ndynamic; i++) {
        g_VideoResHeader.vertex_buffers[dynamic_indices[i]] = 0;
    }
    for (i = 0; i < native_scene->nvertex_streams; i++) {
        native_scene->vertex_streams[i]->vertex_buffer =
            g_VideoResHeader.vertex_buffers[static_cast<i32>(native_scene->vertex_streams[i]->vertex_buffer)];
    }
    native_scene->nvertex_buffers = g_VideoResHeader.nvertex_buffers;
    native_scene->nindex_buffers = g_VideoResHeader.nindex_buffers;
    memcpy(native_scene->vertex_buffers, g_VideoResHeader.vertex_buffers,
           g_VideoResHeader.nvertex_buffers * sizeof(*native_scene->vertex_buffers));
    memcpy(native_scene->index_buffers, g_VideoResHeader.index_buffers,
           g_VideoResHeader.nindex_buffers * sizeof(*native_scene->index_buffers));

    for (i = 0; i < scene->nummtl; i++) {
        NuMtlUpdate(scene->mtls[i]);
    }
    NuPortalMaxDepth(scene, scene->max_portals);
    NuThreadCriticalSectionBegin(g_vaoLifetimeMutex);
    NuIOS_ResetVAODuplicateFinder();
    PreWarmGeomsAndBakeVAOs(scene->display_list, native_scene);
    NuThreadCriticalSectionEnd(g_vaoLifetimeMutex);
}

SAGA_HOST_WEAK void NuGScnCreatePS(nugscn_s *scene, variptr_u *, variptr_u *) {
    i32 i;
    u32 hash;

    if (g_VideoResHeader.texture_hashes != 0) {
        for (i = 0; i < g_VideoResHeader.ntextures; i++) {
            hash = g_VideoResHeader.textures[i];
            if (hash != 0) {
                g_VideoResHeader.textures[i] = NuIOS_CreateGLTexFromHash(g_VideoResHeader.textures[i]);
            }
            scene->textures[i]->image_data = NULL;
            scene->textures[i]->size = hash;
            scene->textures[i]->platform.gl_tex = g_VideoResHeader.textures[i];
        }
    } else {
        for (i = 0; i != scene->ntextures; i++) {
            scene->textures[i]->platform.gl_tex = g_VideoResHeader.textures[i];
            scene->textures[i]->image_data = NULL;
            scene->textures[i]->size = 0;
        }
    }
}

void NuGScnDestroyPS(nugscn_s *scene) {
    i32 i;
    nunativegscene_s *native_scene;
    NUDISPLAYLISTGEOM *geometry;

    native_scene = scene->field437_0x1d0;
    if (native_scene == NULL) {
        return;
    }

    BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nugscn_android.c", 0x2fd);
    NuIOSBindVAO(0);
    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nugscn_android.c", 0x2ff);

    NuThreadCriticalSectionBegin(g_vaoLifetimeMutex);
    for (i = 0; i < native_scene->ngeometries; i++) {
        geometry = native_scene->geometries[i];
        if (geometry->vertex_format != 0) {
            if (geometry->immediate != 0) {
                BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nugscn_android.c", 0x30b);
                glDeleteBuffers(1, reinterpret_cast<GLuint *>(&geometry->vertex_format));
                EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nugscn_android.c", 0x30d);
                if (bgProcIsBgThread()) {
                    NuIOS_YieldThread();
                }
            } else {
                if (bgProcIsBgThread()) {
                    NuIOS_YieldThread();
                }
            }
        }
        geometry->vertex_format = 0;
    }
    NuThreadCriticalSectionEnd(g_vaoLifetimeMutex);

    for (i = 0; i != native_scene->nvertex_buffers; i++) {
        if (native_scene->vertex_buffers[i] != 0) {
            BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nugscn_android.c", 0x32d);
            glDeleteBuffers(1, reinterpret_cast<GLuint *>(&native_scene->vertex_buffers[i]));
            EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nugscn_android.c", 0x32f);
            if (bgProcIsBgThread()) {
                NuIOS_YieldThread();
            }
            native_scene->vertex_buffers[i] = 0;
        }
    }
    for (i = 0; i != native_scene->nindex_buffers; i++) {
        if (native_scene->index_buffers[i] != 0) {
            BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nugscn_android.c", 0x33f);
            glDeleteBuffers(1, reinterpret_cast<GLuint *>(&native_scene->index_buffers[i]));
            EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nugscn_android.c", 0x341);
            if (bgProcIsBgThread()) {
                NuIOS_YieldThread();
            }
            native_scene->index_buffers[i] = 0;
        }
    }

    BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nugscn_android.c", 0x34c);
    memset(g_lastBound2DTexIds, 0xff, sizeof(g_lastBound2DTexIds));
    memset(g_lastBoundCubeTexIds, 0xff, sizeof(g_lastBoundCubeTexIds));
    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nugscn_android.c", 0x353);

    for (i = 0; i != scene->ntextures; i++) {
        if (scene->textures[i]->platform.gl_tex != 0) {
            BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nugscn_android.c", 0x35a);
            glDeleteTextures(1, &scene->textures[i]->platform.gl_tex);
            scene->textures[i]->platform.gl_tex = 0;
            EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nugscn_android.c", 0x35e);
            if (bgProcIsBgThread()) {
                NuIOS_YieldThread();
            }
        }
    }
}

extern "C" void NuGSceneSetCrossFade(void) {
    STUBBED();
}

extern "C" void NuGSceneSetCrossFadeAlpha(void) {
    STUBBED();
}

extern "C" void NuGSceneProcessCrossFade(void) {
    STUBBED();
}
