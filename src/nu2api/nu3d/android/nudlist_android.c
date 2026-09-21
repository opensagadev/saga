#include "decomp.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/android/nudlist_callbacks.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuvport.h"

#include <string.h>

// Original 0x625ae0: writable handlers for item types 0x80..0xb0.
static nudl_handler_fn __ItemFnTable[49] = {
    NuIOSDLMtlCallback,             // 0x80
    nullptr,                        // 0x81
    NuIOSDLGeomCallback,            // 0x82
    NuIOSDLTransformCallback,       // 0x83
    nullptr,                        // 0x84
    nullptr,                        // 0x85
    nullptr,                        // 0x86
    nullptr,                        // 0x87
    nullptr,                        // 0x88
    nullptr,                        // 0x89
    nullptr,                        // 0x8a
    NuIOSDLGeomCallback,            // 0x8b
    NuIOSDLTransformParamsCallback, // 0x8c
    nullptr,                        // 0x8d
    nullptr,                        // 0x8e
    NuIOSDLFaceOnCallback,          // 0x8f
    NuIOSDLFaceOnTransformCallback, // 0x90
    nullptr,                        // 0x91
    nullptr,                        // 0x92
    NuIOSDLGeom2DCallback,          // 0x93
    NuIOSDLLightsCallback,          // 0x94
    nullptr,                        // 0x95
    nullptr,                        // 0x96
    nullptr,                        // 0x97
    NuIOSDLGeomCallback,            // 0x98
    NuIOSDLSkinMtxCallback,         // 0x99
    NuIOSDLCameraCallback,          // 0x9a
    nullptr,                        // 0x9b
    nullptr,                        // 0x9c
    nullptr,                        // 0x9d
    nullptr,                        // 0x9e
    nullptr,                        // 0x9f
    nullptr,                        // 0xa0
    nullptr,                        // 0xa1
    nullptr,                        // 0xa2
    nullptr,                        // 0xa3
    nullptr,                        // 0xa4
    NuIOSDLKonstCallback,           // 0xa5
    NuIOSDLFogCallback,             // 0xa6
    NuIOSDLDebrisCallback,          // 0xa7
    nullptr,                        // 0xa8
    NuIOSDLVertexGroupsCallback,    // 0xa9
    NuIOSDLVertexOffsetsCallback,   // 0xaa
    NuIOSDLReflectionCallback,      // 0xab
    nullptr,                        // 0xac
    nullptr,                        // 0xad
    NuIOSDLLightmapOld,             // 0xae
    NuIOSDLLightmapOffsetOld,       // 0xaf
    NuIOSDLLightmap,                // 0xb0
};

// Original 0x625bc0: writable shadow handlers.
extern "C" {
    nudl_handler_fn __ShadowItemTable[49] = {
        NuIOSDLDeferredMtlCallback,             // 0x80
        nullptr,                                // 0x81
        NuIOSDLGeomCallback,                    // 0x82
        NuIOSDLDeferredTransformCallback,       // 0x83
        nullptr,                                // 0x84
        nullptr,                                // 0x85
        nullptr,                                // 0x86
        nullptr,                                // 0x87
        nullptr,                                // 0x88
        nullptr,                                // 0x89
        nullptr,                                // 0x8a
        NuIOSDLGeomCallback,                    // 0x8b
        NuIOSDLDeferredTransformParamsCallback, // 0x8c
        nullptr,                                // 0x8d
        nullptr,                                // 0x8e
        nullptr,                                // 0x8f
        nullptr,                                // 0x90
        nullptr,                                // 0x91
        nullptr,                                // 0x92
        nullptr,                                // 0x93
        nullptr,                                // 0x94
        nullptr,                                // 0x95
        nullptr,                                // 0x96
        nullptr,                                // 0x97
        NuIOSDLGeomCallback,                    // 0x98
        NuIOSDLSkinMtxCallback,                 // 0x99
        nullptr,                                // 0x9a
        nullptr,                                // 0x9b
        nullptr,                                // 0x9c
        nullptr,                                // 0x9d
        nullptr,                                // 0x9e
        nullptr,                                // 0x9f
        nullptr,                                // 0xa0
        nullptr,                                // 0xa1
        nullptr,                                // 0xa2
        nullptr,                                // 0xa3
        nullptr,                                // 0xa4
        nullptr,                                // 0xa5
        nullptr,                                // 0xa6
        nullptr,                                // 0xa7
        nullptr,                                // 0xa8
        NuIOSDLVertexGroupsCallback,            // 0xa9
        NuIOSDLVertexOffsetsCallback,           // 0xaa
        nullptr,                                // 0xab
        nullptr,                                // 0xac
        nullptr,                                // 0xad
        nullptr,                                // 0xae
        nullptr,                                // 0xaf
        nullptr,                                // 0xb0
    };
}

// Original 0x625c84: initialized pointer, not assigned by a constructor.
static nudl_handler_fn *CurrentItemTable = __ItemFnTable;

extern "C" {
    // Original 0x29aa49: private to this display-list translation unit.
    static void NuDisplayListResetBuffer(void) {
        display_list_buffer = reinterpret_cast<VARIPTR *>(&rndrstream_free);
        display_list_buffer_end = reinterpret_cast<VARIPTR *>(rndrstream_end.addr);
    }
}

static void NuDisplayListSetNext(nudisplaylistitem_s *item, void *next) {
    item->next = next;
}

static void NuDisplayListSetID_CALL(nudisplaylistitem_s *item) {
    item->id = 3;
}

static void NuDisplayListSetID_CNT(nudisplaylistitem_s *item) {
    item->id = 0;
}

static void NuDisplayListSetID_NEXT(nudisplaylistitem_s *item) {
    item->id = 1;
}

static void NuDisplayListSetID_RET(nudisplaylistitem_s *item) {
    item->id = 4;
}

static void NuDisplayListSetID(nudisplaylistitem_s *item, u8 id) {
    switch (id) {
        case 0:
            NuDisplayListSetID_CNT(item);
            break;
        case 1:
            NuDisplayListSetID_NEXT(item);
            break;
        case 3:
            NuDisplayListSetID_CALL(item);
            break;
        case 4:
            NuDisplayListSetID_RET(item);
            break;
    }
}

static void NuDisplayListSetItem(nudisplaylistitem_s *item, u8 type, u8 id, void *next) {
    item->type = type;
    NuDisplayListSetNext(item, next);
    NuDisplayListSetID(item, id);
}

// Original 0x29ab93.
extern "C" void NuDisplayListInit(VARIPTR *buf, VARIPTR buf_end) {
    // The embedded 2D list at +0x4b8 starts at its stream-head sentinel.
    global_dlist_manager.dlist_2d.first = &global_dlist_manager.dlist_2d_first;
    DisplayListCreateDynMtlList(buf, buf_end);
    NuDisplayListResetBuffer();
    global_dlist_manager.loading_critical_section = NuThreadCreateCriticalSection();
}

extern "C" void NuDisplayListDrawItems(nudisplaylistitem_s *items) {
    NuDisplayListExecute(items, CurrentItemTable);
}

extern "C" void NuDisplayListDraw(void) {
    STUBBED();
}

extern "C" void DisplayListDebugPS(void) {
    STUBBED();
}

extern "C" void NuDisplayListAddClut(nudisplaylistitem_s *item, i32) {
    NuDisplayListSetItem(item, 0x87, 0, nullptr);
}

extern "C" void NuDisplayListAddTexture(nudisplaylistitem_s *item, i32) {
    NuDisplayListSetItem(item, 0x87, 0, nullptr);
}

extern "C" void NuDisplayListAddMaterialState(nudisplaylistitem_s *item, void *mtl) {
    NuDisplayListSetItem(item, 0x80, 3, mtl);
}

extern "C" void NuDisplayListAddMicrocode(nudisplaylistitem_s *item, void *) {
    NuDisplayListSetItem(item, 0x87, 0, nullptr);
}

extern "C" void NuDisplayListAddLightState(nudisplaylistitem_s *item, void *) {
    NuDisplayListSetItem(item, 0x87, 0, nullptr);
}

extern "C" void DisplayListCreatePS(void) {
    STUBBED();
}

extern "C" void NuDisplayListLinkItem(nudisplaylist_s *list, u8 type, void *call_addr) {
    VARIPTR *buf = NuDisplayListGetBuffer();
    NuDisplayListLinkItemVP(list, type, call_addr, buf);
}

extern "C" VARIPTR *NuDisplayListLinkItemVP(nudisplaylist_s *list, u8 type, void *call_addr, VARIPTR *buf) {
    list->mtl_last->next = buf->void_ptr;
    reinterpret_cast<nudisplaylistitem_s *>(buf->void_ptr)->type = type;
    NuDisplayListSetID_CALL(reinterpret_cast<nudisplaylistitem_s *>(buf->void_ptr));
    if (call_addr != nullptr) {
        NuDisplayListSetNext(reinterpret_cast<nudisplaylistitem_s *>(buf->void_ptr), call_addr);
    } else {
        NuDisplayListSetNext(reinterpret_cast<nudisplaylistitem_s *>(buf->void_ptr),
                             reinterpret_cast<nudisplaylistitem_s *>(buf->void_ptr) + 2);
    }

    (reinterpret_cast<nudisplaylistitem_s *>(buf->void_ptr) + 1)->type = 0x8d;
    NuDisplayListSetID_NEXT(reinterpret_cast<nudisplaylistitem_s *>(buf->void_ptr) + 1);
    NuDisplayListSetNext(reinterpret_cast<nudisplaylistitem_s *>(buf->void_ptr) + 1, list->dyn_geom + 1);
    list->mtl_last = reinterpret_cast<nudisplaylistitem_s *>(buf->void_ptr) + 1;
    buf->addr += sizeof(nudisplaylistitem_s) * 2;
    if (call_addr != nullptr) {
        return nullptr;
    }
    return buf;
}

extern "C" VARIPTR *NuDisplayListLinkItems(nudisplaylist_s *list, i32 count) {
    VARIPTR *buf = NuDisplayListGetBuffer();
    NuDisplayListSetNext(list->mtl_last, buf->void_ptr);
    list->items = reinterpret_cast<nudisplaylistitem_s *>(buf->void_ptr);
    buf->addr += count * sizeof(nudisplaylistitem_s);

    reinterpret_cast<nudisplaylistitem_s *>(buf->void_ptr)->type = 0x8d;
    NuDisplayListSetID_NEXT(reinterpret_cast<nudisplaylistitem_s *>(buf->void_ptr));
    NuDisplayListSetNext(reinterpret_cast<nudisplaylistitem_s *>(buf->void_ptr), list->dyn_geom + 1);
    list->mtl_last = reinterpret_cast<nudisplaylistitem_s *>(buf->void_ptr);
    buf->addr += 0x10;
    return buf;
}

extern "C" void *DisplayListCreateGeomTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *next, void *tx) {
    static NUMTX scale;
    static NUMTX translate;
    static NUMTX viewtranslate;
    static NUMTX viewscale;
    static NUMTX viewTransform;

    (void)mtl;
    (void)next;
    (void)tx;

    NUMTX *projection = NuCameraGetProjectionMtx();
    NUMTX *view = NuCameraGetViewMtx();
    (void)projection;
    (void)view;

    NUVEC scale_vector = {1.0f, 1.0f, 1.0f};
    NUVEC translation_vector = {0.0f, 0.0f, 0.0f};
    scale_vector.x = g_NuVpRegion.projection_x_scale;
    scale_vector.y = g_NuVpRegion.projection_y_scale;
    NuMtxSetScale(&scale, &scale_vector);
    translation_vector.x = g_NuVpRegion.projection_x_offset;
    translation_vector.y = g_NuVpRegion.projection_y_offset;
    NuMtxSetTranslation(&translate, &translation_vector);

    buffer->addr = ALIGN(buffer->addr, 4);
    NUMTX *result = static_cast<NUMTX *>(buffer->void_ptr);
    NuMtxTranspose(result, transform);
    buffer->addr += sizeof(NUMTX);
    return result;
}

extern "C" void *DisplayListCreateFaceonTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *, void *faceon_data) {
    void *faceon = faceon_data;
    buffer->addr = ALIGN(buffer->addr, 4);
    void *packet = buffer->void_ptr;
    *static_cast<NUMTX *>(buffer->void_ptr) = *transform;
    const f32 magnitude =
        NuFsqrt((NuVecMagSqr(reinterpret_cast<NUVEC *>(buffer->void_ptr)) +
                 NuVecMagSqr(reinterpret_cast<NUVEC *>(buffer->char_ptr + 0x10)) +
                 NuVecMagSqr(reinterpret_cast<NUVEC *>(buffer->char_ptr + 0x20))) /
                3.0f);
    buffer->addr += sizeof(NUMTX);
    *buffer->f32_ptr++ = magnitude;

    NUVEC direction;
    if (*static_cast<i32 *>(faceon) == 0) {
        direction.x = 0.0f;
        direction.y = direction.x;
        direction.z = direction.y;
        NuMtxCalcCheapFaceOn(buffer->mtx_ptr++, &direction);
    } else {
        direction.x = transform->m10;
        direction.y = transform->m11;
        direction.z = transform->m12;
        NuMtxCalcCheapFaceY_v2(buffer->mtx_ptr++, &direction);
    }
    return packet;
}

void DisplayListCreateGeomItemPS(variptr_u *, void *, numtl_s *) {
    STUBBED();
}

// Original 0x29b338.  Build the compact skin-palette packet consumed by
// NuIOSDLSkinMtxCallback.  Blend-shape deltas are applied to a stream copy
// of immediate geometry before the ordinary geometry callback runs.
extern "C" void *DisplayListCreateSkinTransformPS(VARIPTR *buffer, NUMTX *skin_matrices,
                                                  DEFORMERWEIGHTSARRAY *deformer_weights, NUDISPLAYLISTGEOM *geometry,
                                                  NUDISPLAYLISTGEOM **render_geometry) {
    if (deformer_weights != NULL && geometry->immediate != 0) {
        buffer->addr = ALIGN(buffer->addr, 0x20);
        NUDISPLAYLISTGEOM *copy = static_cast<NUDISPLAYLISTGEOM *>(buffer->void_ptr);
        *render_geometry = copy;
        buffer->addr += sizeof(*copy);
        *copy = *geometry;

        buffer->addr = ALIGN(buffer->addr, 0x20);
        copy->dynamic_vertex_data = buffer->void_ptr;
        const usize vertex_data_size = static_cast<usize>(geometry->vertex_stride) * geometry->vertex_count;
        buffer->addr += vertex_data_size;

        u8 *source = reinterpret_cast<u8 *>(static_cast<usize>(geometry->vertex_buffer)) +
                     static_cast<usize>(geometry->vertex_stride) * geometry->base_vertex;
        u8 *destination = static_cast<u8 *>(copy->dynamic_vertex_data);
        memmove(destination, source, vertex_data_size);

        for (i32 vertex = 0; vertex < geometry->vertex_count; ++vertex) {
            NUVEC *position = reinterpret_cast<NUVEC *>(destination);
            for (i32 deformer = 0; deformer < deformer_weights->count; ++deformer) {
                NUVEC *offsets = geometry->deformer_vertex_offsets[deformer];
                const f32 weight = deformer_weights->weights[deformer];
                if (offsets != NULL && weight != 0.0f) {
                    position->x += offsets[vertex].x * weight;
                    position->y += offsets[vertex].y * weight;
                    position->z += offsets[vertex].z * weight;
                }
            }
            destination += geometry->vertex_stride;
            source += geometry->vertex_stride;
        }
    } else {
        geometry->dynamic_vertex_data = NULL;
        *render_geometry = geometry;
    }

    i32 *matrix_count = static_cast<i32 *>(buffer->void_ptr);
    void *packet = matrix_count;
    *matrix_count = 0;
    buffer->addr += sizeof(*matrix_count);
    for (i32 i = 0; i < 8 && geometry->joint_indices[i] != 0xff; ++i) {
        NUMTX *packet_matrix = static_cast<NUMTX *>(buffer->void_ptr);
        *packet_matrix = skin_matrices[geometry->joint_indices[i]];
        buffer->addr += sizeof(NUMTX);
        ++*matrix_count;
    }
    return packet;
}

extern "C" void DisplayListCreateRigidSkinTransformPS(void) {
    STUBBED();
}

extern "C" void DisplayListCreateRigidSkin2TransformPS(void) {
    STUBBED();
}

void DisplayListCreateInstSurfGeomPS(variptr_u *, numtx_s *) {
    STUBBED();
}

extern "C" void DisplayListPrintItemPS(void) {
    STUBBED();
}

extern "C" void DisplayListSwapBuffersPS(void) {
}

extern "C" void DisplayListCreateFxItemPS(void *, i32) {
    STUBBED();
}

extern "C" void DisplayListDestroyFxItemPS(void *) {
    STUBBED();
}

extern "C" void DisplayListSetFxItemParamPS(void *, i32, f32, i32) {
    STUBBED();
}

void DisplayListGenerateTransforms(nudisplayscene_s *) {
}

extern "C" void *NuDisplayListPrepareFaceonPS(VARIPTR *, void *faceon, NUMTX *) {
    void *prepared = faceon;
    return prepared;
}

// NuDisplaySceneAddPS @ 0x2ab7aa.  The apparently redundant assignment is
// present in the Android original.
extern "C" void NuDisplaySceneAddPS(NUDLDLISTSCENE *scene) {
    for (i32 i = 0; i < scene->nitems; ++i) {
        if (scene->items[i].type == 0x82) {
            scene->items[i].type = 0x82;
        }
    }
}

// NuDisplaySceneDestroyPS @ 0x2ab7f9
extern "C" void NuDisplaySceneDestroyPS(NUDLDLISTSCENE *) {
}

extern "C" void NuDisplaySceneClonePS(NUDLDLISTSCENE *source, NUDLDLISTSCENE *destination, VARIPTR *buffer) {
    destination->ps = nullptr;
    if (source->ps) {
        void *copy = (void *)ALIGN(buffer->addr, 4);
        buffer->addr = ALIGN(buffer->addr, 4);
        buffer->addr += 4;
        memcpy(copy, source->ps, 4);
        destination->ps = copy;
    }
}

extern "C" void DisplayListUpdateSpecialTransformPS(nuhspecial_s *, NUMTX *) {
    STUBBED();
}

extern "C" void NuDisplayListSetItemTable(i32 which) {
    switch (which) {
        case 0:
            CurrentItemTable = __ItemFnTable;
            break;
        case 1:
            CurrentItemTable = __ShadowItemTable;
            break;
        default:
            break;
    }
}

extern "C" void DisplayListSetAlphaPS(nudisplaylistitem_s *prev_item, nudisplaylistitem_s *item, f32 alpha) {
    NUMTX *matrix = reinterpret_cast<NUMTX *>(prev_item->next);
    matrix->m33 = alpha < 0.0f ? 0.0f : (alpha > 1.0f ? 1.0f : alpha);
    (void)item;
}

extern "C" void DisplayListSetShadowCasterFlagPS(NUDISPLAYLISTITEM *previous, NUDISPLAYLISTITEM *, i32 flag) {
    NUMTX *transform = (NUMTX *)previous->next;
    transform->m32 = (f32)flag;
}
