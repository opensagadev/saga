#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nurndrstat.h"
#include <string.h>

// Shared immediate-mode primitive state (BSS and initialized data).
VARIPTR *g_NuPrim_StreamBufferPtr;
i32 g_NuPrim_VertexCount;
char g_NuPrim_NeedsOverbrightening;
char g_NuPrim_NeedsHalfUVs;

i32 NuPrimCSPos;
NUPRIMSCALEMODE NuPrimCoordSystemStack[16];

f32 NuPrim_XScale = 1.0f;
f32 NuPrim_YScale = 1.0f;
f32 NuPrim_XBias;
f32 NuPrim_YBias;

static u16 *g_NuPrim_VertexCountPtr;
static u16 g_NuPrim_CurrentPrimType = 10000;
static NUMTX g_identity_mtx = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                               0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

extern "C" {
    static void NuPrimPushCoordSystem(NUPRIMSCALEMODE scale_mode) {
        NuPrimCSPos++;
        NuPrimSetCoordinateSystem(scale_mode);
    }
}

extern "C" {
    static NUDISPLAYLIST *NuDisplayListGet2dList(void) {
        return &global_dlist_manager.dlist_2d;
    }
}

static void NuDisplayListSetNext(NUDISPLAYLISTITEM *item, void *next) {
    item->next = next;
}

static void NuDisplayListSetID_CALL(NUDISPLAYLISTITEM *item) {
    item->id = 3;
}

static NUDISPLAYLISTITEM *NuDisplayListAddItem(NUDISPLAYLIST *list, u8 type, void *next) {
    list->items->type = type;
    NuDisplayListSetID_CALL(list->items);
    NuDisplayListSetNext(list->items, next);
    list->items = reinterpret_cast<NUDISPLAYLISTITEM *>(reinterpret_cast<u8 *>(list->items) + 0x10);
    return reinterpret_cast<NUDISPLAYLISTITEM *>(reinterpret_cast<u8 *>(list->items) - 0x10);
}

struct PrimStreamHeader {
    u32 prim_type;
    u32 pad0;
    u16 pad1;
    u16 vertex_count;
    u32 pad2;
};
static_assert(sizeof(PrimStreamHeader) == 0x10, "PrimStreamHeader must be 0x10");

extern "C" void NuPrim2DBegin(u32 prim_type, u32, NUMTL *mtl) {
    if (mtl == nullptr) {
        mtl = numtl_defaultmtl2d;
    }

    g_NuPrim_NeedsOverbrightening = mtl->tex_id != 0;
    g_NuPrim_NeedsHalfUVs = mtl->shader_desc.vtx_desc.has_half_uvs;
    VARIPTR *buf = NuDisplayListGetBuffer();

    NUDISPLAYLIST *list;
    if (mtl->display_list != nullptr) {
        list = mtl->display_list;
        list->dlist->clip_materials = 1;
        list->dlist->mtl_used[list->dlist->current_buffer][list->mtl_id / 8] |= 1 << (list->mtl_id % 8);
    } else {
        list = NuDisplayListGet2dList();
        NuDisplayListLinkMtl(list, mtl);
    }

    RndrStateSetConstAlphaTint(0, 0, 0.0f, nullptr, nullptr);
    DisplayListUpdateRenderState(list, &render_state);
    NuDisplayListLinkItems(list, 1);
    g_NuPrim_StreamBufferPtr = buf;

    PrimStreamHeader *header = static_cast<PrimStreamHeader *>(g_NuPrim_StreamBufferPtr->void_ptr);
    header->prim_type = prim_type;
    g_NuPrim_StreamBufferPtr->addr += sizeof(PrimStreamHeader);
    g_NuPrim_VertexCountPtr = &header->vertex_count;
    g_NuPrim_CurrentPrimType = static_cast<u16>(prim_type);
    g_NuPrim_VertexCount = 0;
    NuDisplayListAddItem(list, 0x93, header);
}

extern "C" void NuPrim3DBegin(u32 prim_type, u32, NUMTL *mtl, NUMTX *world_mtx) {
    if (mtl == nullptr) {
        mtl = numtl_defaultmtl3d;
    }

    g_NuPrim_NeedsOverbrightening = mtl->tex_id != 0;
    g_NuPrim_NeedsHalfUVs = mtl->shader_desc.vtx_desc.has_half_uvs;

    VARIPTR *buf = NuDisplayListGetBuffer();
    NUDISPLAYLIST *list;
    if (mtl->display_list != nullptr) {
        list = mtl->display_list;
        list->dlist->clip_materials = 1;
        list->dlist->mtl_used[list->dlist->current_buffer][list->mtl_id / 8] |= 1 << (list->mtl_id % 8);
    } else {
        list = numtl_defaultmtl3d->display_list;
    }

    DisplayListUpdateRenderState(list, &render_state);
    NuDisplayListLinkItems(list, 2);
    g_NuPrim_StreamBufferPtr = buf;

    NUMTX *transform = static_cast<NUMTX *>(DisplayListCreateGeomTransformPS(
        g_NuPrim_StreamBufferPtr, world_mtx != nullptr ? world_mtx : &g_identity_mtx, nullptr, nullptr, nullptr));

    NUDISPLAYLISTGEOM *geometry = static_cast<NUDISPLAYLISTGEOM *>(g_NuPrim_StreamBufferPtr->void_ptr);
    geometry->primitive_type = prim_type;
    geometry->vertex_count = 0;
    g_NuPrim_StreamBufferPtr->addr += sizeof(NUDISPLAYLISTGEOM);

    g_NuPrim_VertexCountPtr = reinterpret_cast<u16 *>(&geometry->vertex_count);
    g_NuPrim_CurrentPrimType = static_cast<u16>(prim_type);

    NuDisplayListAddItem(list, 0x8c, transform);
    NuDisplayListAddItem(list, 0x82, geometry);
}

extern "C" void NuPrim2DEnd(void) {
    *g_NuPrim_VertexCountPtr = static_cast<u16>(g_NuPrim_VertexCount);
    g_NuPrim_VertexCount = 0;
}

extern "C" void NuPrim3DEnd(void) {
    *g_NuPrim_VertexCountPtr = static_cast<u16>(g_NuPrim_VertexCount);
    g_NuPrim_VertexCount = 0;
}

extern "C" void NuPrim2DAddXYZ(float x, float y, float z) {
    ((PrimVertexRaw *)g_NuPrim_StreamBufferPtr->void_ptr)->x = NuPrim_XBias + NuPrim_XScale * x;
    ((PrimVertexRaw *)g_NuPrim_StreamBufferPtr->void_ptr)->y = NuPrim_YBias + NuPrim_YScale * y;
    ((PrimVertexRaw *)g_NuPrim_StreamBufferPtr->void_ptr)->z = z;
    g_NuPrim_StreamBufferPtr->addr += sizeof(PrimVertexRaw);
    g_NuPrim_VertexCount++;

    if (g_NuPrim_CurrentPrimType == 4 && (g_NuPrim_VertexCount & 1) == 0) {
        // Expand the last two quad vertices into two triangles.
        PrimVertexRaw *v = (PrimVertexRaw *)(g_NuPrim_StreamBufferPtr->addr - 2 * sizeof(PrimVertexRaw));
        g_NuPrim_StreamBufferPtr->addr += 4 * sizeof(PrimVertexRaw);
        g_NuPrim_VertexCount += 4;

        v[2] = v[1];
        v[3] = v[2];
        v[4].x = v[0].x;
        v[4].y = v[1].y;
        v[4].z = v[1].z;
        v[4].uv[0] = v[0].uv[0];
        v[4].uv[1] = v[1].uv[1];
        v[4].color = v[1].color;
        v[1].x = v[2].x;
        v[1].y = v[0].y;
        v[1].z = v[0].z;
        v[1].uv[0] = v[2].uv[0];
        v[1].uv[1] = v[0].uv[1];
        v[1].color = v[0].color;
        v[5] = v[0];
    }
}

extern "C" void NuPrimInit(VARIPTR *, VARIPTR) {
    NuPrimCSPos = -1;
    NuPrimPushCoordSystem(NUPRIM_SCALEMODE_PS2);
}

extern "C" void NuPrimSetCoordinateSystem(NUPRIMSCALEMODE scale_mode) {
    NuPrimCoordSystemStack[NuPrimCSPos] = scale_mode;

    switch (scale_mode) {
        case NUPRIM_SCALEMODE_PS2:
            NuPrim_XScale = 0.003125f;
            NuPrim_YScale = -0.008928572f;
            NuPrim_XBias = -1.0f;
            NuPrim_YBias = 1.0f;
            break;
        case NUPRIM_SCALEMODE_NORMALISED:
            NuPrim_XScale = 1.0f;
            NuPrim_YScale = -1.0f;
            NuPrim_XBias = 0.0f;
            NuPrim_YBias = 0.0f;
            break;
        case NUPRIM_SCALEMODE_ABSOLUTE:
            NuPrim_XScale = 2.0f;
            NuPrim_YScale = -2.0f;
            NuPrim_XBias = -1.0f;
            NuPrim_YBias = 1.0f;
            break;
    }
}
