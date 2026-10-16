#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuprim_internal.h"

#include <string.h>

#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nuvec4.h"

extern "C" u32 NuWindRand(void);

struct nugobj_s;
struct PartHeader;
struct uv1debdata;
struct nuvec4_s;

i32 nurndr_pixel_width;
i32 nurndr_pixel_height;
i32 nurndr_nforced_mtls;
NUMTL **nurndr_forced_mtl_table;
NUMTL *nurndr_forced_mtl;
i32 nurndr_force_lod;

i32 global_GobjIsReflectedGeometry;
i32 global_GobjIsShadowReceive;

i32 NuRndIsReflectionGobj(void) {
    return global_GobjIsReflectedGeometry;
}

i32 NuRndIsShadowReceiveRenderGobj(void) {
    return global_GobjIsShadowReceive;
}

void NuRndrInitEx(i32 stream_buffer_size, VARIPTR *buffer, VARIPTR buffer_end) {
    NuRndrStreamInit(stream_buffer_size, buffer);
}

u32 rndrstream_nbuffers = 2;
i32 nurndr_maxstreamsize;
VARIPTR rndrstream_end;
VARIPTR rndrstream_free;
static VARIPTR rndrstream[NURNDR_STREAM_MAX_BUFFERS];

static i32 rndrstream_buffid;
static i32 rndrstream_used;     // _ZL15rndrstream_used @0x11b8380
static i32 rndrstream_max_used; // _ZL19rndrstream_max_used @0x11b8390

void NuRndrStreamInit(i32 stream_buffer_size, VARIPTR *buffer) {
    i32 i;

    if (buffer == NULL) {
        for (i = 0; i < rndrstream_nbuffers; i++) {
            rndrstream[i].void_ptr = NU_ALLOC(stream_buffer_size, 4, 1, "", NUMEMORY_CATEGORY_NONE);
            memset(rndrstream[i].void_ptr, 0, stream_buffer_size);
        }
    } else {
        for (i = 0; i < rndrstream_nbuffers; i++) {
            buffer->addr = ALIGN(buffer->addr, 0x80);
            rndrstream[i].void_ptr = buffer->void_ptr;

            memset(buffer->void_ptr, 0, stream_buffer_size);

            buffer->void_ptr = (void *)(buffer->addr + stream_buffer_size);
        }
    }

    rndrstream_buffid = 0;
    nurndr_maxstreamsize = stream_buffer_size;
    rndrstream_end.addr = rndrstream[0].addr + stream_buffer_size;
    rndrstream_free.addr = ALIGN(rndrstream[0].addr, 16);
}

// original 0x2e6e90 — rotate to the next stream buffer, recording the high
// watermark. The infinite-loop tail is the original's buffer-overflow trap.
extern "C" void NuRndrSwapStreamBuffers(void) {
    i32 last_used;

    last_used = (i32)((i64)rndrstream_free.addr - (i64)rndrstream[rndrstream_buffid].addr);
    if (rndrstream_max_used < last_used) {
        rndrstream_max_used = last_used;
    }
    if (last_used > nurndr_maxstreamsize) {
        for (;;) {
        }
    }
    rndrstream_buffid++;
    if (rndrstream_buffid == rndrstream_nbuffers) {
        rndrstream_buffid = 0;
    }
    rndrstream_free.addr = ALIGN(rndrstream[rndrstream_buffid].addr, 16);
    rndrstream_end.addr = rndrstream[rndrstream_buffid].addr + nurndr_maxstreamsize;
    rndrstream_used = last_used;
}

void NuRndrRectUV2d(f32 x, f32 y, f32, f32 width, f32 height, f32 u0, f32 v0, f32 u1, f32 v1, i32 colour,
                    numtl_s *material) {
    NuPrim2DBegin(4, 7, material);
    NuRndrPrimTexturedColour(u0, v0, colour);
    NuPrim2DAddXYZ(x, y, 0.0f);
    NuRndrPrimTexturedColour(u1, v1, colour);
    NuPrim2DAddXYZ(x + width, y + height, 0.0f);
    NuPrim2DEnd();
}

void NuRndrRectUV2diZ(i32 x, i32 y, i32 width, i32 height, f32 u0, f32 v0, f32 u1, f32 v1, i32 colour,
                      numtl_s *material, i32 z) {
    const f32 sx = static_cast<f32>(x) * 0.0625f;
    const f32 sy = static_cast<f32>(y) * 0.0625f;
    const f32 sw = static_cast<f32>(width) * 0.0625f;
    const f32 sh = static_cast<f32>(height) * 0.0625f;
    const f32 sz = static_cast<f32>(z) * (1.0f / 16777215.0f);
    NuPrim2DBegin(4, 7, material);
    NuRndrPrimTexturedColour(u0, v0, colour);
    NuPrim2DAddXYZ(sx, sy, sz);
    NuRndrPrimTexturedColour(u1, v1, colour);
    NuPrim2DAddXYZ(sx + sw, sy + sh, sz);
    NuPrim2DEnd();
}

static NUMTL *pZClearMaterial;
static NUMTL *pAlphaMask;
static const NUPRIMSCALEMODE kPRIM_COORD_LOOKUP[3] = {NUPRIM_SCALEMODE_PS2, NUPRIM_SCALEMODE_NORMALISED,
                                                      NUPRIM_SCALEMODE_ABSOLUTE};

void RndrMaskScreen(i32 texture, f32 clear_x, f32 clear_y, f32 clear_width, f32 clear_height, f32 mask_x, f32 mask_y,
                    f32 mask_width, f32 mask_height, i32 coordinate_mode) {
    pAlphaMask->tex_id = static_cast<i16>(texture);
    NuMtlUpdate(pAlphaMask);
    NuRndrBeginScene(-1);
    NUPRIMSCALEMODE coordinates = kPRIM_COORD_LOOKUP[coordinate_mode];
    ++NuPrimCSPos;
    NuPrimSetCoordinateSystem(coordinates);
    NuPrim2DBegin(1, 7, pZClearMaterial);
    NuRndrPrimSetColour(0xff000000);
    NuRndrPrimUV(0.0f, 0.0f);
    NuPrim2DAddXYZ(clear_x, clear_y, 0.0f);
    NuRndrPrimSetColour(0xff000000);
    NuRndrPrimUV(0.0f, 0.0f);
    NuPrim2DAddXYZ(clear_x + clear_width, clear_y, 0.0f);
    NuRndrPrimSetColour(0xff000000);
    NuRndrPrimUV(0.0f, 0.0f);
    NuPrim2DAddXYZ(clear_x, clear_y + clear_height, 0.0f);
    NuRndrPrimSetColour(0xff000000);
    NuRndrPrimUV(0.0f, 0.0f);
    NuPrim2DAddXYZ(clear_x + clear_width, clear_y + clear_height, 0.0f);
    NuPrim2DEnd();
    --NuPrimCSPos;
    NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[NuPrimCSPos]);
    ++NuPrimCSPos;
    NuPrimSetCoordinateSystem(coordinates);
    NuPrim2DBegin(1, 7, pAlphaMask);
    NuRndrPrimSetColour(0xff000000);
    NuRndrPrimUV(0.0f, 0.0f);
    NuPrim2DAddXYZ(mask_x, mask_y, 1.0f);
    NuRndrPrimSetColour(0xff000000);
    NuRndrPrimUV(1.0f, 0.0f);
    NuPrim2DAddXYZ(mask_x + mask_width, mask_y, 1.0f);
    NuRndrPrimSetColour(0xff000000);
    NuRndrPrimUV(0.0f, 1.0f);
    NuPrim2DAddXYZ(mask_x, mask_y + mask_height, 1.0f);
    NuRndrPrimSetColour(0xff000000);
    NuRndrPrimUV(1.0f, 1.0f);
    NuPrim2DAddXYZ(mask_x + mask_width, mask_y + mask_height, 1.0f);
    NuPrim2DEnd();
    --NuPrimCSPos;
    NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[NuPrimCSPos]);
    NuRndrEndScene();
}

void NuRndrInitGeneric() {
    const i32 render_plane = NuMtlSetCurrentRenderPlane(22);
    if (!pZClearMaterial) {
        pZClearMaterial = NuMtlCreate(1);
        pZClearMaterial->attribs.z_mode = 2;
        pZClearMaterial->attribs.alpha_mode = 2;
        pZClearMaterial->attribs.alpha_test = 1;
        NuMtlUpdate(pZClearMaterial);
    }
    if (!pAlphaMask) {
        pAlphaMask = NuMtlCreate(1);
        pAlphaMask->attribs.alpha_fail = 0;
        pAlphaMask->attribs.z_mode = 2;
        pAlphaMask->attribs.alpha_test = 6;
        pAlphaMask->attribs.alpha_ref = 0;
        pAlphaMask->attribs.alpha_mode = 2;
    }
    NuMtlSetCurrentRenderPlane(render_plane);
    NuVpResetRegions();
}

void NuRndrGradRect2diZ(i32 x, i32 y, i32 width, i32 height, i32 *colours, numtl_s *material, i32 z) {
    const f32 sx = static_cast<f32>(x) * 0.0625f;
    const f32 sy = static_cast<f32>(y) * 0.0625f;
    const f32 sw = static_cast<f32>(width) * 0.0625f;
    const f32 sh = static_cast<f32>(height) * 0.0625f;
    const f32 sz = static_cast<f32>(z) * (1.0f / 16777215.0f);
    NuPrim2DBegin(1, 7, material);
    NuRndrPrimSetColour(colours[0]);
    NuPrim2DAddXYZ(sx, sy, sz);
    NuRndrPrimSetColour(colours[1]);
    NuPrim2DAddXYZ(sx + sw, sy, sz);
    NuRndrPrimSetColour(colours[2]);
    NuPrim2DAddXYZ(sx, sy + sh, sz);
    NuRndrPrimSetColour(colours[3]);
    NuPrim2DAddXYZ(sx + sw, sy + sh, sz);
    NuPrim2DEnd();
}

void NuRndrRectUV2dNoScale(float, float, float, float, float, float, float, float, i32, numtl_s *) {
    STUBBED();
}

void NuRndrCalcRandEllipsePos(nuvec4_s *position, numtx_s *matrix, nuvec_s *axes) {
    f32 x = (f32)(i32)NuWindRand() - 32768.0f;
    f32 y = (f32)(i32)NuWindRand() - 32768.0f;
    f32 z = (f32)(i32)NuWindRand() - 32768.0f;
    f32 scale = 1.0f / NuFsqrt(x * x + y * y + z * z);
    x *= scale;
    y *= scale;
    z *= scale;
    position->x = axes[1].x * x + axes[2].x * y + axes[3].x * z + axes[0].x;
    position->y = axes[1].y * x + axes[2].y * y + axes[3].y * z + axes[0].y;
    position->z = axes[1].z * x + axes[2].z * y + axes[3].z * z + axes[0].z;
    position->w = 1.0f;
    NuVec4MtxTransformVU0(position, position, matrix);
}

void NuRndrCalcRandCylinderPos(nuvec4_s *position, numtx_s *matrix, nuvec_s *axes) {
    f32 x = (f32)((i32)NuWindRand() - 32768) * (1.0f / 32768.0f);
    f32 y = (f32)(i32)NuWindRand() - 32768.0f;
    f32 z = (f32)(i32)NuWindRand() - 32768.0f;
    f32 scale = 1.0f / NuFsqrt(y * y + z * z);
    y *= scale;
    z *= scale;
    position->x = axes[1].x * x + axes[2].x * y + axes[3].x * z + axes[0].x;
    position->y = axes[1].y * x + axes[2].y * y + axes[3].y * z + axes[0].y;
    position->z = axes[1].z * x + axes[2].z * y + axes[3].z * z + axes[0].z;
    position->w = 1.0f;
    NuVec4MtxTransformVU0(position, position, matrix);
}
