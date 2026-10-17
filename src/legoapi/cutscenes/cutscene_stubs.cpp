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
void instNuGCutSceneEndButNotSystems(instNUGCUTSCENE_s *instance);
void instNuGCutSceneResetCamLock(instNUGCUTSCENE_s *);

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
    extern NUGCUTLOCATORFNENTRY_s *locatorfns;
    extern f32 timeincrement;
    extern i32 processdeb;
    extern f32 glyntestha;

    void NuAnimData2CalcTime(nuanimdata2_s *, f32, nuanimtime_s *);
    void NuMtxPreTranslate(NUMTX *, NUVEC *);
    void NuMtxMul(NUMTX *, NUMTX *, NUMTX *);
    i32 NuStrICmp(const char *, const char *);
    void instNuGCutSceneStart(instNUGCUTSCENE_s *);

    i32 AddScaledVariableShotDebrisEffect2(i32, NUVEC *, i32, f32, NUMTX *, NUMTX *, f32);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisPopulateInstance(i32, f32);
    void DebrisEmitterPos(i32, f32, f32, f32);
    void DebrisOrientationMtx(i32, NUMTX *);
    void DebrisPosOrientationMtx(i32, NUMTX *);
    void AddScaledFiniteShotDebrisEffect(i32 *, i32, NUVEC *, NUVEC *, NUVEC *, i32, f32);
    void DebrisEmitterOrientationMtx(i32, NUMTX *);
    void DebFreeInstantly(i32 *);
}

extern "C" {

    i32 CheckStreamFileID(void) {
        return 0;
    }

void DisplayCutSceneMemory(void);

void PauseGameCut(void);

void RestoreGameCut(void);

    u8 ForceScenePlayBack;

    void SetForceScenePlayBack(i32 enabled) {
        ForceScenePlayBack = static_cast<u8>(enabled);
    }

f32 instCutSceneTimeElapsed(instNUGCUTSCENE_s *instance);

i32 instNuGCutSceneAddCamTgt(instNUGCUTSCENE_s *instance, NUVEC *target, f32 start_frame, f32 duration,
                                 i8 target_index);

void instNuGCutSceneAddCleanUpItem(void *handle, i32 enabled);

void instNuGCutSceneCalculateAverageCentre(instNUGCUTSCENE_s *instance, NUMTX *matrix, NUVEC *out);

void instNuGCutSceneChain(instNUGCUTSCENE_s *instance, instNUGCUTSCENE_s *next);

i32 instNuGCutSceneCharGetStartMtx(instNUGCUTSCENE_s *instance, const char *name, NUMTX *out);

void instNuGCutSceneCreateCamTgtArray(instNUGCUTSCENE_s *instance, i32 count, VARIPTR *buf);

void instNuGCutSceneDisable(instNUGCUTSCENE_s *instance);

void instNuGCutSceneEnable(instNUGCUTSCENE_s *instance);

i32 instNuGCutSceneIsFinished(instNUGCUTSCENE_s *instance);

void instNuGCutScenePlay(instNUGCUTSCENE_s *instance, i32 forward);

void instNuGCutSceneResetCleanUp(void *list_storage, void *cut_base, void *cut_end, void *inst_base, void *inst_end,
                                     instNUGCUTSCENE_s *(*get_inst)(void *),
                                     void *(*create_inst)(void *, NUGCUTSCENE_s *, VARIPTR *),
                                     void (*inst_destroyed)(void *));

void instNuGCutSceneRotateY(instNUGCUTSCENE_s *instance, NUANG angle);

void instNuGCutSceneSetEndCallback(instNUGCUTSCENE_s *instance, void (*callback)(instNUGCUTSCENE_s *));

void instNuGCutSceneSetMtx(instNUGCUTSCENE_s *instance, NUMTX *matrix);

void instNuGCutSceneSetPos(instNUGCUTSCENE_s *instance, NUVEC *pos);

void instNuGCutSceneSetRepeat(instNUGCUTSCENE_s *instance, i32 repeat);

void instNuGCutSceneStop(instNUGCUTSCENE_s *instance);

f32 instNuGCutSceneTimeLeft(instNUGCUTSCENE_s *instance);

f32 instNuGCutSceneTotalTime(instNUGCUTSCENE_s *instance);

void instNuGCutSceneTranslate(instNUGCUTSCENE_s *instance, NUVEC *translation);

void instNuGCutSceneWaitAtEnd(instNUGCUTSCENE_s *instance, u8 enabled);

void instNuGCutSoundStream(void);

} // extern "C"

void instGetLookAtLocatorInfo(instNUGCUTSCENE_s *instance, instNUGCUTLOOKAT_s *state);

// Steers a cutscene character's eyes towards a look-at locator and blinks its eyelids.
// Only version-3 locator animations carry the look and blink channels.
// Retail only keeps the IPA-SRA clone: the system is dropped and the instance
// locator is replaced by its effect handle slot.
#if defined(__i386__)
#define INST_LOOKAT_UPDATE_ATTR __attribute__((noinline, noclone, regparm(2), sseregparm))
#else
#define INST_LOOKAT_UPDATE_ATTR __attribute__((noinline, noclone))
#endif
static INST_LOOKAT_UPDATE_ATTR void instNuGCutLookAtLocatorUpdate(i32 *effect_handle, NUGCUTLOCATOR_s *locator,
                                                                  f32 frame, NUMTX *parent_mtx,
                                                                  instNUGCUTSCENE_s *instance)
    __asm__("_ZL29instNuGCutLookAtLocatorUpdateP18NUGCUTLOCATORSYS_sP19instNUGCUTLOCATOR_sP15NUGCUTLOCATOR_"
            "sfP7numtx_sP17instNUGCUTSCENE_s.isra.23");
static void instNuGCutLookAtLocatorUpdate(i32 *effect_handle, NUGCUTLOCATOR_s *locator, f32 frame,
                                          NUMTX *parent_mtx, instNUGCUTSCENE_s *instance);

extern "C" {
    // Aligned four-float row used for retail's movaps matrix copies.
    typedef f32 LocatorMtxRow __attribute__((vector_size(16)));

void instNuGCutLocatorUpdate(instNUGCUTSCENE_s *instance, NUGCUTLOCATORSYS_s *system,
                                 instNUGCUTLOCATOR_s *inst_locator, NUGCUTLOCATOR_s *locator, f32 frame,
                                 NUMTX *parent_mtx, i32 paused);

} // extern "C"
