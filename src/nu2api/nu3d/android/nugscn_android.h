#pragma once

#include "decomp.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nufile/nufile.h"

#include <GLES2/gl2.h>
#include <stddef.h>

struct nudisplaylistgeom_s;
struct nunativevertexstream_s {
    u32 unknown_00;
    u32 unknown_04;
    usize vertex_buffer;
};

struct nunativegscene_s {
    u16 nvertex_buffers;
    u16 pad_02;
    usize *vertex_buffers;
    u16 nindex_buffers;
    u16 pad_0a;
    usize *index_buffers;
    nudisplaylistgeom_s **geometries;
    i32 ngeometries;
    nunativevertexstream_s **vertex_streams;
    i32 nvertex_streams;
};

// The original Android executable uses 32-bit pointers; native host builds
// retain their own natural pointer layout.
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(nunativegscene_s, vertex_buffers) == 0x04, "native scene vertex buffers");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(nunativegscene_s, index_buffers) == 0x0c, "native scene index buffers");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(nunativegscene_s, geometries) == 0x10, "native scene geometries");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(nunativegscene_s, ngeometries) == 0x14, "native scene geometry count");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(nunativegscene_s, vertex_streams) == 0x18, "native scene vertex streams");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(nunativegscene_s, nvertex_streams) == 0x1c,
              "native scene vertex stream count");
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(nunativegscene_s) == 0x20, "native scene size");
DECOMP_ASSERT(offsetof(nunativevertexstream_s, vertex_buffer) == 0x08, "native vertex stream buffer");

i32 NuGScnUploadGfxDataFromFilePS(VARIPTR *buf, VARIPTR buf_end, i32 file);
extern u32 g_lastBoundVAO;

#ifdef __cplusplus
extern "C" {
#endif
    extern i32 g_vaoLifetimeMutex;
    // Placeholder only: the original consumes three stack arguments; their types remain unresolved.
    void NuGSceneSetCrossFade(void);
    void NuGSceneSetCrossFadeAlpha(void);
    void NuGSceneProcessCrossFade(void);
#ifdef __cplusplus
}
#endif
