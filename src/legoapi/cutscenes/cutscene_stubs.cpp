#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/cutscenes/cutscene_defrag.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/nusound/nusound.h"

#include <cfloat>
#include <string.h>

extern "C" i32 NuGCutLocatorIsVisble(NUGCUTLOCATOR_s *, f32, nuanimtime_s *, f32 *, f32 *);
extern "C" f32 *NuGCutLocatorCalcMtx(NUGCUTLOCATOR_s *, f32, NUMTX *, nuanimtime_s *);

extern "C" {
extern CutSceneCleanUpEntry * DefragCutSceneList;
extern CutSceneCleanUpEntry * DefragCutSceneListBase;
extern void * DefragCutSceneBaseMem;
extern void * DefragCutSceneEndMem;
extern void * DefragInstBaseMem;
extern void * DefragInstEndMem;
extern instNUGCUTSCENE_s *(*DefragGetInstFn)(void *);
extern void *(*DefragCreateInstFn)(void *, NUGCUTSCENE_s *, VARIPTR *);
extern void (*DefragInstDestroyedFn)(void *);
extern i32 DefragCutSceneListSize;
    extern debinftype **debtab;

    void NuMtxPreTranslate(NUMTX *, NUVEC *);
    void NuMtxMul(NUMTX *, NUMTX *, NUMTX *);
    i32 NuStrICmp(const char *, const char *);

}

extern "C" {

    i32 CheckStreamFileID(void) {
        return 0;
    }

    u8 ForceScenePlayBack;

    void SetForceScenePlayBack(i32 enabled) {
        ForceScenePlayBack = static_cast<u8>(enabled);
    }

i32 instNuGCutSceneAddCamTgt(instNUGCUTSCENE_s *instance, NUVEC *target, f32 start_frame, f32 duration,
                                 i8 target_index);

void instNuGCutSceneCreateCamTgtArray(instNUGCUTSCENE_s *instance, i32 count, VARIPTR *buf);

void instNuGCutSceneSetMtx(instNUGCUTSCENE_s *instance, NUMTX *matrix);

} // extern "C"

// Steers a cutscene character's eyes towards a look-at locator and blinks its eyelids.
// Only version-3 locator animations carry the look and blink channels.
// Retail only keeps the IPA-SRA clone: the system is dropped and the instance
// locator is replaced by its effect handle slot.
#if defined(__i386__)
#define INST_LOOKAT_UPDATE_ATTR __attribute__((noinline, noclone, regparm(2), sseregparm))
#else
#define INST_LOOKAT_UPDATE_ATTR __attribute__((noinline, noclone))
#endif

extern "C" {
    // Aligned four-float row used for retail's movaps matrix copies.

} // extern "C"
