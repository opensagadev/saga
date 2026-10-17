#include "decomp.h"
#include "nu2api_nucore_types.h"
#include "nu2api/nu3d/android/nudlist_callbacks.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/ShaderManagerOpenGL.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/android/nutex_android.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nushader.h"
#include "nu2api/nu3d/nushader_plain.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/nuplatform/nuplatform.h"
#include <GLES2/gl2.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

struct TERRSET;
extern TERRSET *CurTerr;


extern "C" {
}



extern "C" void BeginCriticalSectionGL(const char *, i32);
extern "C" void EndCriticalSectionGL(const char *, i32);
extern i32 bgProcIsBgThread(void);
struct MemoryManager;
extern MemoryManager theMemoryManager;
extern "C" void *MemoryManagerAllocPool(MemoryManager *, u32, i32) asm("_ZN13MemoryManager9AllocPoolEji");
extern "C" void MemoryManagerFreePool(MemoryManager *, void *, u32) asm("_ZN13MemoryManager8FreePoolEPvj");

extern "C" void NuAnimData2CalcTime(nuanimdata2_s *, f32, nuanimtime_s *);
extern "C" f32 NuAnimCurve2CalcValEx(nuanimcurve2_s *, nuanimtime_s *, u32);
void NuGCutRigidCalcMtx_3(NUGCUTRIGID_s *, f32, numtx_s *);


extern "C" void NuAnimBuffCreateScratch(nuanimbuff_s *buffer);
extern "C" void NuAnimBuffDestroyScratch(nuanimbuff_s *buffer);
extern nurenderscene_s currentScene;
extern HashRedirect g_shaderProgramRedirects[417];





extern "C" u8 CutSceneBoundingBoxTrackRoot;


// NuIOS_SetCullMode is transcribed in android/nuiosdl_gl.cpp (original 0x29c110).




// NuIOSDLMtlCallback is transcribed in android/nuiosdl_gl.cpp (original 0x29c480).


// NuIOSDLGeom2DCallback is transcribed in android/nuiosdl_gl.cpp (original 0x29d1a0).

i32 NuIOS_GetInAppProduct(i32, NuIOS_InAppProduct *) {
    return 0;
}


i32 NuIOS_GetPurchaseResult() {
    return 0;
}


void NuIOS_DisplaySystemAlert(char const *) {
}

i32 NuIOS_IsProductPurchased(char *) {
    return 0;
}


i32 NuIOS_GetInAppProductByID(char *, NuIOS_InAppProduct *) {
    return 0;
}



void NuIOS_GetNumInAppPurchases() {
}

i32 NuIOS_PurchaseInAppProduct(char *) {
    return 0;
}


i32 NuIOS_CanMakeInAppPurchases() {
    return 0;
}

void NuIOS_RestoreInAppPurchases() {
}


// NuDebrisRendererFlushBuffers is transcribed in android/nuptl_android.c (original 0x296f35).


i32 NuIOS_IsProductPurchasedByNum(i32) {
    return 0;
}

void NuIOS_CateInAppPurchaseManager() {
}


i32 NuIOS_GetInAppProductIdentifier(i32, NuIOS_InAppProduct *) {
    return 0;
}

i32 NuIOS_PurchaseInAppProductByNum(i32) {
    return 0;
}

i32 NuIOS_AreInAppPurchasesAvailable() {
    return 0;
}




extern "C" {
    u32 NuLgtSeed = 12345;
}


void NuMemory::MemErrorHandler::CloseDump(NuMemoryManager *, u32) {
}

void NuMemory::MemErrorHandler::Dump(NuMemoryManager *, u32, char const *) {
}

void NuMemory::MemErrorHandler::HandleError(NuMemoryManager *, NuMemoryManager::ErrorCode, char const *) {
}

void NuMemory::MemErrorHandler::OpenDump(NuMemoryManager *, char const *, u32 &) {
}
