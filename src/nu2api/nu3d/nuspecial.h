#ifndef NU2API_NU3D_NUSPECIAL_H
#define NU2API_NU3D_NUSPECIAL_H

#include "decomp.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuhspecial.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numath/numtx.h"

struct nuinstanim_s;
struct NuLegacyInstanceLayout {
    u8 pad_00[0x44];
    union {
        u8 flags;
        struct {
            u8 visible : 1;
            u8 on_screen : 1;
            u8 unused_flag_2 : 1;
            u8 no_visibility_test : 1;
            u8 unused_flags_4_7 : 4;
        };
    };
    u8 pad_45[3];
    nuinstanim_s *animation;
};
struct NuSpecialLegacyLayout {
    u8 pad_00[0x40];
    void *instance;
    char *name;
    u32 flags;
};

struct NUSPECIALVERTEXSTATES {
    u8 count;
    u8 block_count; // Number of 16-byte blocks allocated for values.
    u8 flags;
    u8 unknown_03;
    i8 *values;
};
extern "C" NUSPECIALVERTEXSTATES *nuspecial_vertex_states;
extern "C" NUSPECIALVERTEXSTATES *NuVertexStatesCreate(VARIPTR *buffer, i32 count);
extern "C" void NuVertexStatesSetGroupState(NUSPECIALVERTEXSTATES *states, i32 group, i32 value);
extern "C" i32 nuspecial_vertex_noffsets;
extern "C" VARIPTR nuspecial_vertex_offsets;

enum NUSPECIAL_DRAW_FLAGS {
    NUSPECIAL_DRAW_MATERIAL_MAP = 1 << 2,
    NUSPECIAL_DRAW_FORCE_MATERIAL = 1 << 3,
};

f32 NuSpecialGetAnimPos(nuhspecial_s *special);
void NuSpecialReflection(i32 reflection);

enum NULEGACYINSTANCE_FLAGS : u8 {
    NULEGACYINSTANCE_FLAG_VISIBLE = 1 << 0,
    NULEGACYINSTANCE_FLAG_NO_VISIBILITY_TEST = 1 << 3,
};

enum NULEGACYSPECIAL_FLAGS : u32 {
    NULEGACYSPECIAL_FLAG_COLLISION = 1 << 9,
};

enum NUDISPLAYSPECIAL_FLAGS : u32 {
    NUDISPLAYSPECIAL_FLAG_VISIBLE = 1 << 1,
    NUDISPLAYSPECIAL_FLAG_ON_SCREEN = 1 << 2,
    NUDISPLAYSPECIAL_FLAG_NO_VISIBILITY_TEST = 1 << 7,
    NUDISPLAYSPECIAL_FLAG_COLLISION = 1 << 9,
    NUDISPLAYSPECIAL_FLAG_MATRIX_UPDATED = 1 << 10,
};

struct NUDISPLAYSPECIAL {
    NUMTX instance_mtx; // 0x00
    NUMTX draw_mtx;     // 0x40
    NUVEC4 bounds_min;  // 0x80
    NUVEC4 bounds_max;  // 0x90
    u8 pad_a0[0x10];
    NUCLIPOBJECT *clip_objects; // 0xb0
    char *name;                 // 0xb4
    u32 flags;                  // 0xb8, NUDISPLAYSPECIAL_FLAGS
    f32 *clip_range;            // 0xbc
    i32 instance_ix;            // 0xc0
    union {
        nuinstanim_s *instance_animation; // 0xc4, animated specials point at a matrix-first record
        NUMTX *draw_mtx_ptr;              // 0xc4, direct draw override used by scene rendering
    };
    u8 pad_c8[8];
};
typedef NUDISPLAYSPECIAL NUDISPLAYSPECIAL_s;

DECOMP_ASSERT(sizeof(NUDISPLAYSPECIAL) == 0xd0, "display special size");
DECOMP_ASSERT(offsetof(NUDISPLAYSPECIAL, draw_mtx) == 0x40, "display special draw matrix offset");
DECOMP_ASSERT(offsetof(NUDISPLAYSPECIAL, flags) == 0xb8, "display special flags offset");

// Named-scene-object ("special") query / manipulation API. Declarations live
// here (module nu2api/nu3d) so level and gameplay code includes one header
// instead of hand-declaring these C-linkage helpers.

extern "C" {
    void NuGScnGetSpecial(nuhspecial_s *special, NUGSCN *scene, i32 index);
    i32 NuGScnNumSpecials(NUGSCN *scene);
    i32 NuSpecialGetNumSpecials(NUGSCN *scene);
    i32 NuSpecialGetFirst(NUGSCN *scene, nuhspecial_s *special, i32 flags);
    void NuSpecialGetNext(nuhspecial_s *special);
    void NuSpecialList(NUGSCN *scene);
    i32 NuSpecialFind(NUGSCN *scene, nuhspecial_s *dest, char *name, i32 flags);
    i32 NuSpecialFindMulti(NUGSCN *scene, nuhspecial_s *dest, char *name, i32 capacity, i32 flags);
    i32 NuSpecialFindMultiWC(NUGSCN *scene, nuhspecial_s *dest, char (*wildcards)[20], char *pattern, i32 capacity,
                             i32 flags);
    i32 NuSpecialCompare(nuhspecial_s *first, nuhspecial_s *second);
    i32 NuSpecialExistsFn(void *special);
    void NuSpecialClear(void *special);
    void NuSpecialGetBounds(void *special, NUVEC *minimum, NUVEC *maximum);
    void NuSpecialSetBounds(nuhspecial_s *special, NUVEC *minimum, NUVEC *maximum);
    void NuSpecialGetRadius(void *special, NUVEC *position, f32 *radius);
    f32 NuSpecialGetAnimEndFrame(nuhspecial_s *special);
    nuinstanim_s *NuSpecialGetInstAnim(nuhspecial_s *special);
    void NuSpecialSetInstAnimTime(nuhspecial_s *special, f32 frame);
    i32 NuSpecialTestAnim(nuhspecial_s *special);
    void NuSpecialSetMtx(nuhspecial_s *special, NUMTX *matrix);
    void NuSpecialSetCollision(nuhspecial_s *special, i32 enabled);
    i32 NuSpecialGetCollision(nuhspecial_s *special);
    i32 NuSpecialForceToAlpha(nuhspecial_s *special);
    NUMTL *NuSpecialGetMtl(nuhspecial_s *special, i32 index);
    i32 NuMtlSpecialSetUV(nuhspecial_s *special, f32 u, f32 v);
    void NuSpecialMtl(NUMTL *material);
    void NuSpecialForceMtl(NUMTL *material);
    void NuSpecialMtlMap(i32 count, NUMTL **materials);
    i32 NuSpecialNumMtls(nuhspecial_s *special);
    NUMTX *NuSpecialGetInstanceMtx(nuhspecial_s *special);
    i32 NuSpecialGetInstanceix(nuhspecial_s *special);
    char *NuSpecialGetName(nuhspecial_s *special);
    void NuSpecialUpdate(nuhspecial_s *special);
    void NuSpecialSetVisibility(void *special, i32 visible);
    void NuSpecialSetDrawMtx(void *special, NUMTX *mtx);
    NUMTX *NuSpecialGetDrawMtx(void *special);
    NUMTX *NuSpecialGetMtx(void *special);
    NUVEC *NuSpecialGetDrawPos(void *special);
    struct nuvec_s *NuSpecialGetPos(void *special);
    i32 NuSpecialDrawAt(void *special, NUMTX *mtx);
    void NuSpecialBurstDrawAt(nuhspecial_s *special, u32 count, NUMTX *matrices, i32 clip);
    i32 NuSpecialGetVisibilityFn(void *special);
    i32 NuSpecialGetOnScreenFn(nuhspecial_s *special);
    i32 NuSpecialGetNoVisiTestFn(nuhspecial_s *special);
    void NuSpecialSetNoVisiTest(nuhspecial_s *special, i32 enabled);
    void NuSpecialSetOnScreen(nuhspecial_s *special, i32 enabled);
    void NuSpecialSetInstanceMtx(nuhspecial_s *special, NUMTX *matrix);
    void NuSpecialSetDrawPos(nuhspecial_s *special, NUVEC *pos);
    i32 NuSpecialClipTestExtents(void *special, void *mtx);
    i32 NuSpecialSetClipping(i32 enabled, i32 state);
    void NuSpecialConstAlpha(i32 enabled, f32 alpha);
    void NuSpecialConstTint(i32 enabled, NUVEC *tint);
    float NuSpecialGetOriginRadius(void *special);
    i32 NuSpecialDrawAtAlpha(void *special, NUMTX *mtx, f32 alpha);
    void NuSpecialVertexStates(NUSPECIALVERTEXSTATES *states);
    void NuSpecialVertexOffsets(i32 count, VARIPTR offsets);
    void NuSpecialSetRenderPlane(void);
    void NuSpecialSetAlphaTest(void);
    void NuSpecialAddShadowLight(void);
    void NuSpecialClearShadowLights(void);
    i32 NuSpecialGetActiveShadowLights(void);
    i32 NuSpecialHasActiveShadowLights(void);
    void *NuSpecialGetShadowLight(i32 index);
    i32 NuSpecialClipTestShadowLights(NUVEC *minimum, NUVEC *maximum, i32 flags);
    i32 NuSpecialHaveShadowClipTestResults(void);
    i32 NuSpecialGetShadowClipTestResult(i32 index);
    void NuSpecialClearShadowClipTestResults(void);
}

#endif
