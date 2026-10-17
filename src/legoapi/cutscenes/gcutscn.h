#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct NUGCUTLOCATORFNENTRY_s;
struct NUGCUTLOCATORSYS_s;
struct NUGCUTLOCATOR_s;
struct NUGCUTRIGID_s;
struct NUGCUTSCENE_s;
struct instNUGCUTLOCATOR_s;
struct instNUGCUTLOOKAT_s;
struct instNUGCUTSCENE_s;
struct nugscn_s;
struct numtl_s;
struct numtx_s;
NUGCUTSCENE_s *RelocateCutScene(NUGCUTSCENE_s *, VARIPTR *);
extern u8 set_cutscenecammtx;
i32 FindTexAnimFromMtl(nugscn_s *, numtl_s *);
i32 instNuGCutSceneSwapBuffers(instNUGCUTSCENE_s *instance, i32 force);
void NewCopyAnims(instNUGCUTSCENE_s *);
void NuGCutRigidCalcMtx(NUGCUTRIGID_s *, f32, numtx_s *);
void NuGCutRigidCalcMtx_3(NUGCUTRIGID_s *, f32, numtx_s *);
void instGetLookAtLocatorInfo(instNUGCUTSCENE_s *instance, instNUGCUTLOOKAT_s *opaque_state);
void instNuGCutSceneEndButNotSystems(instNUGCUTSCENE_s *instance);
void instNuGCutSceneEndFirstFrame(instNUGCUTSCENE_s *instance);
void instNuGCutSceneResetCamLock(instNUGCUTSCENE_s *instance);
extern "C" {
    extern NUGCUTLOCATORFNENTRY_s *locatorfns;
    extern i32 NuGCutAudioStream;
    extern i32 NuGCutDebFixUp_SearchAllPages;
    extern instNUGCUTSCENE_s *cutscene_load_instance;
    i32 instNuGCutSceneIsFinished(instNUGCUTSCENE_s *cutscene);
    void NuGCutSceneSysPostBackgroundLoad(void);
    void NuGCutSceneSysRender(i32);
    void NuGCutSceneSysUpdate(i32, i32, f32);
    void NuGCutSetCutAudioStream(i32 stream);
    void instNuGCutLocatorUpdate(instNUGCUTSCENE_s *, NUGCUTLOCATORSYS_s *, instNUGCUTLOCATOR_s *, NUGCUTLOCATOR_s *, f32, numtx_s *, i32);
    void instNuGCutSceneDestroy(instNUGCUTSCENE_s *);
    void instNuGCutScenePause(instNUGCUTSCENE_s *, u8);
    void instNuGCutSceneReset(instNUGCUTSCENE_s *);
    void instNuGCutSceneServiceLoad(void);
    void instNuGCutSceneStart(instNUGCUTSCENE_s *);
    void instNuGCutSceneStop(instNUGCUTSCENE_s *);
}
