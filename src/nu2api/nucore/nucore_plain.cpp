// Temporary C-linkage compatibility surface for unresolved engine owners.
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "decomp.h"
#include "java/java.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/numath/numtx.h"
#include "globals.h"
#include "nu2api/nu3d/nu3d_includes.h"
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.

struct ani3_animheader_s;
struct nuanimdatachunk_s;

extern "C" {
extern GLuint g_colorRenderbuffer;
extern GLuint g_depthRenderbuffer;
}

using NUHGOBJVIDEOMEMFN = void (*)(nuhgobj_s *);

extern NUHGOBJVIDEOMEMFN hgobj_to_video_mem;
extern NUHGOBJVIDEOMEMFN video_mem_to_hgobj;

void NuPadRecordEndFrame(void);                      // nupad_interface.cpp
i32 NuCameraClipHGobj(nugscn_s *, NUMTX *, NUMTX *); // nu2api_nucore_misc.cpp
i32 findrange(nugscn_s *, i32);                      // utilities.cpp

extern "C" {
    extern nurenderscene_s currentScene;
} // extern "C"

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");
static void DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                           NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix,
                                           void **transform_packet, NUMTX *skin_matrices,
                                           DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster);

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
static void DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                                  NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix,
                                                  void **transform_packet, f32 alpha);
    }

    extern void (*nuapi_endframe_callbackfn)(void);

    // ---------------------------------------------------------------------------
    // iOS / platform
    // ---------------------------------------------------------------------------

    void NuIOS_AwardAchievement(void) {
        // Retail no-op.
    }
void NuIOS_DeallocateSystemFramebuffers(void);

void NuIOS_RecordFlurryEvent(char *event_name);
    void NuIOS_ShowAchievements(void) {
        // Retail no-op.
    }

    extern "C++" void NuAnimDataChunkDestroy(nuanimdatachunk_s *chunk);

    static void *dirnames[16];
    static i32 numdirs;

    i32 NuSysDirClose(i32 index) {
        NuMemoryGet()->GetThreadMem()->BlockFree(dirnames[index], 0);
        dirnames[index] = NULL;
        --numdirs;
        return 0;
    }
    i32 NuSysDirOpen(void) {
        // Retail returns zero.
        return 0;
    }
    i32 NuSysDirRead(void) {
        // Retail returns zero.
        return 0;
    }

void NuQFntPrint2dW(NUQFNT *font, u16 *text);
void NuQFntPrint3DU(NUQFNT *font, char *text);
void NuQFntPrint3DW(NUQFNT *font, u16 *text);
void NuQFntPrintV(NUQFNT *font, const char *format, va_list arguments);

    extern nurenderscene_s currentScene;
    i32 NuLgtArcLaserOldCnt;

#include "nu2api/nucore/nucore_plain_types.h"
void NuHGobjFromVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuHGobjToVideoMem(NUHGOBJVIDEOMEMFN callback);
#undef NUGCUT_CURVE_VALUE

    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    extern "C++" {
        static __attribute__((noinline, noclone)) void
        ClipInstTree(NuInstanceTree_s *tree,
                     i32 tree_index) __asm__("_ZL12ClipInstTreeP16NuInstanceTree_siP16nudisplayscene_sPc.isra.92");
static void ClipInstTree(NuInstanceTree_s *tree, i32 tree_index);
    }

    extern "C++" void OctreeRndr(u8 *visibility, nuoctreenode_s *root, i32 enabled);

#ifndef ANDROID
    void NuGetCurrentThreadId(void) {
        STUBBED();
    }
#endif
#ifndef ANDROID
    void NuThreadCreate(void) {
        STUBBED();
    }
#endif

    extern void (*nuapi_endframe_callbackfn)(void);
void NuRegisterEndFrameCallBackFn(void (*callback)(void));
} // extern "C"

struct nupad_s;
struct nuframebuffer_s;
struct nushaderobject_s;
union variptr_u;

