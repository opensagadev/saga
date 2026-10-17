// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/core/input/qrand.h"

#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/world/world.h"
#include "legoapi/world/level.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"

#include "legoapi/actions/character/tagging_types.h"
#include "nu2api/nu3d/nuspecial.h"
#include "legoapi/actions/movement/jumping.h"
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/render/fx.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nufile/nufile.h"
#include <string.h>
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edcam.h"
#include "nu2api/numusic/sfx.h"
#include <stdio.h>
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include <math.h>
#include <stddef.h>
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/core/config/cheat.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "legoapi/render/fx/edsplines.h"
#include <float.h>
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include <stdlib.h>
#include "gameframework/saveload.h"
#include <cstdio>
#include "nu2api/nu3d/nuspline.h"
#include <new>
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/world/mission.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/numath/nuvec4.h"
#include <stdarg.h>
#include <GLES2/gl2.h>
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/numath/nuang.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nu3d/nupostparams.h"
static TAGTRANSFER_s Tag_Transfer[2];
static const f32 Tag_TransferResetTimer = 0.5f;

// The original keeps this search out of line and passes the object in EAX.

f32 BUCK_RIDERJUMPCLEARANCE = 1.0f;
void (*BuckStartExtraFn)(GameObject_s *) = NULL;

static __attribute__((used)) void edanimcbCubeMap(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbFileLoad(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbFileSave(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbMCTBMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSoundMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbBouncyMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSwitchMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSwitchId(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbMCTBCardType(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbParticleMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSoundType(eduimenu_s *menu, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSetSwitchVar(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbCancelOptMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSoundTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbCancelMCTBMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbLocalSoundMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbMCTBCardFormat(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSoundTiming(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSetSwitchDelay(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbCancelSoundMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbMCTBCardPresent(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetParticleRate(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSetParticleType(eduimenu_s *menu, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbToggleSoundType(eduimenu_s *menu, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbCancelBouncyMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbCancelSwitchMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbMCTBCardLoadSlot(eduimenu_s *menu, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbMCTBCardSaveSlot(eduimenu_s *menu, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbMCTBCardUnFormat(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbParticleTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetBouncyDamping(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSetBouncyTension(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbLocalParticleMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbMCTBCardFreeSpace(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbMCTBCardSlotsUsed(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetLocalSoundType(eduimenu_s *menu, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbCancelParticleMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbLocalSoundTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbMCTBCardDeleteSlot(eduimenu_s *menu, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbCancelSoundTypeMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbMCTBCardCheckFormat(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetBouncyPlayerGrav(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbCancelLocalSoundMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbMCTBCardCheckKeyCard(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbMCTBCardWriteKeyCard(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetLocalParticleType(eduimenu_s *menu, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbToggleParticleSwitch(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbLocalParticleTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbCancelParticleTypeMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbCancelLocalParticleMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbCancelLocalSoundTypeMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbCancelLocalParticleTypeMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbBouncyMenu(eduimenu_s *parent, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSoundMenu(eduimenu_s *parent, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbParticleMenu(eduimenu_s *parent, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSwitchMenu(eduimenu_s *parent, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbLocalParticleMenu(eduimenu_s *parent, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSoundTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbLocalSoundTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbLocalParticleTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbParticleTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSwitchTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbMCTBMenu(eduimenu_s *parent, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbLocalSoundMenu(eduimenu_s *parent, eduiitem_s *, u32);

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

static __used__ __attribute__((noclone)) void eduiFntPrintExGeneric(void *font, int x, int y,
                                                                                int alignment, char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz");
static __used__ void eduiFntPrintExGeneric(void *font, int x, int y, int alignment, char *format, ...);

static __attribute__((noclone)) void eduiFntPrintExLeft(void *font, int x, int y, int alignment,
                                                                  char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.67");

static __attribute__((noclone)) void eduiFntPrintExCentre(void *font, int x, int y, int alignment,
                                                                    char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.68");

#if defined(__clang__)
static void eduiFntPrintEx(void *font, int x, int y, int alignment, char *format, ...)
    __asm__("eduiFntPrintExFormatted");
#else
static inline __attribute__((always_inline)) void eduiFntPrintEx(void *font, int x, int y, int alignment,
                                                                 char *format, ...) {
    if (__builtin_constant_p(alignment) && alignment == 16)
        eduiFntPrintExLeft(font, x, y, alignment, format, __builtin_va_arg_pack());
    else if (__builtin_constant_p(alignment) && alignment == 64)
        eduiFntPrintExCentre(font, x, y, alignment, format, __builtin_va_arg_pack());
    else
        eduiFntPrintExGeneric(font, x, y, alignment, format, __builtin_va_arg_pack());
}
#endif

extern "C" {
static __used__ void cbMMCancel(eduimenu_s *, eduimenu_s *);
static __used__ i32 cbInteractMenuCancelChild(edui_interact_s *interact);
static __used__ void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);
static __used__ void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void cbmcfgCancel(eduimenu_s *menu, eduimenu_s *);
static __used__ void cbMMRegSel(eduimenu_s *, eduiitem_s *selected, u32);
static __used__ void cbMMEditorConfig(eduimenu_s *menu, eduiitem_s *, u32);
static __used__ void eduicbDestroyDirectoryList(eduimenu_s *menu, eduimenu_s *);
static __used__ i32 eduicbInteractColourPick(edui_interact_s *interact);
static __used__ i32 eduicbInteractExpander(edui_interact_s *interact);
static __used__ i32 eduicbInteractProp(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractFilter(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractSel(edui_interact_s *interact);
static __used__ void eduicbItemDestroyExpander(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemDestroyFilter(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemFilePickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *item);
static __used__ void eduicbItemSliderDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemTextPickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbPickCFGCancel(eduimenu_s *menu, eduimenu_s *);
static __used__ i32 eduicbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessColourSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessExpander(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilePick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilter(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGradPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGraph(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessGreyPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessProp(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessSel(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
static __used__ i32 eduicbProcessSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTextPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTexturePick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderColourPick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderExpander(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGraph(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32 selected);
static __used__ i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderProp(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilter(struct eduimenu_s *menu, struct eduiitem_s *item, i32 x, i32 y, i32 scale,
                                           i32 selected);
static __used__ i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSelWithClipColour(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width,
                                                      i32);
static __used__ i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTexturePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ void eduicbSelectDirectoryEntry(eduimenu_s *menu, eduiitem_s *item, u32 value);
static __used__ void eduicbSelectDirectoryExit(eduimenu_s *menu, eduiitem_s *, u32);
}

static void eduiFntPrintExLeft(void *font, int x, int y, int, char *format, ...);
static void eduiFntPrintExCentre(void *font, int x, int y, int, char *format, ...);

extern EdInputContext * Input;

inline EdMatrixControl::~EdMatrixControl() {
    Destroy();
}

inline void EdMatrixControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdMatrixControl));
}

inline void EdStringControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdStringControl));
}

template <> inline void EdValueControl<f32>::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdValueControl<f32>));
}

inline void EdFloatControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdFloatControl));
}

inline EdVectorControl::~EdVectorControl() {
    Destroy();
}

inline void EdVectorControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdVectorControl));
}

inline EdClassObjectNameControl::~EdClassObjectNameControl() {
}

inline void EdClassObjectNameControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdClassObjectNameControl));
}

inline EdSubSystem::~EdSubSystem() {
}

inline void EdControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdControl));
}

// Match each control's allocation type, rather than inheriting the base size.
inline void EdEnumControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdEnumControl));
}

inline void EdBitControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdBitControl));
}

extern "C" {
static __used__ void cbMMCancel(eduimenu_s *, eduimenu_s *);
static __used__ i32 cbInteractMenuCancelChild(edui_interact_s *interact);
static __used__ void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);
static __used__ void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void cbmcfgCancel(eduimenu_s *menu, eduimenu_s *);
static __used__ void cbMMRegSel(eduimenu_s *, eduiitem_s *selected, u32);
static __used__ void cbMMEditorConfig(eduimenu_s *menu, eduiitem_s *, u32);
static __used__ void eduicbDestroyDirectoryList(eduimenu_s *menu, eduimenu_s *);
static __used__ i32 eduicbInteractColourPick(edui_interact_s *interact);
static __used__ i32 eduicbInteractExpander(edui_interact_s *interact);
static __used__ i32 eduicbInteractProp(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractFilter(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractSel(edui_interact_s *interact);
static __used__ void eduicbItemDestroyExpander(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemDestroyFilter(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemFilePickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *item);
static __used__ void eduicbItemSliderDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemTextPickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbPickCFGCancel(eduimenu_s *menu, eduimenu_s *);
static __used__ i32 eduicbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessColourSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessExpander(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilePick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilter(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGradPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGraph(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessGreyPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessProp(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessSel(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
static __used__ i32 eduicbProcessSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTextPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTexturePick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderColourPick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderExpander(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGraph(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32 selected);
static __used__ i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderProp(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilter(struct eduimenu_s *menu, struct eduiitem_s *item, i32 x, i32 y, i32 scale,
                                           i32 selected);
static __used__ i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSelWithClipColour(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width,
                                                      i32);
static __used__ i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTexturePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ void eduicbSelectDirectoryEntry(eduimenu_s *menu, eduiitem_s *item, u32 value);
static __used__ void eduicbSelectDirectoryExit(eduimenu_s *menu, eduiitem_s *, u32);
}

extern f32 HUB_EPISODETITLEY;

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

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
} // extern "C"

struct nupad_s;

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

struct GAMEMESSAGE_s;

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

void Tag_ResetTransfers() {
    Tag_Transfer[0].time = Tag_TransferResetTimer;
    Tag_Transfer[1].time = Tag_TransferResetTimer;
}

void Tag_NewTransfer(GameObject_s *source, GameObject_s *target) {
    const i8 player_index = target->apiobj.field_0x27c;
    if (static_cast<u8>(player_index) < 2) {
        Tag_Transfer[player_index].time = 0.0f;
        Tag_Transfer[player_index].source = source;
        Tag_Transfer[player_index].height[0] = static_cast<f32>(qrand()) * 1.5259022e-05f * 0.4f + 0.4f;
        TAGTRANSFER_s &first = Tag_Transfer[target->apiobj.field_0x27c];
        first.position[0].x = source->apiobj.collision_position.x;
        first.position[0].z = source->apiobj.collision_position.z;
        first.position[0].y = first.height[0] * (source->apiobj.collision_max.y - source->apiobj.collision_min.y) +
                              source->apiobj.collision_min.y;

        const i8 second_index = target->apiobj.field_0x27c;
        Tag_Transfer[second_index].height[1] = static_cast<f32>(qrand()) * 1.5259022e-05f * 0.4f + 0.4f;
        TAGTRANSFER_s &second = Tag_Transfer[target->apiobj.field_0x27c];
        second.position[1].x = source->apiobj.collision_position.x;
        second.position[1].z = source->apiobj.collision_position.z;
        second.position[1].y = second.height[1] * (source->apiobj.collision_max.y - source->apiobj.collision_min.y) +
                               source->apiobj.collision_min.y;

        const i8 third_index = target->apiobj.field_0x27c;
        Tag_Transfer[third_index].height[2] = static_cast<f32>(qrand()) * 1.5259022e-05f * 0.4f + 0.4f;
        TAGTRANSFER_s &third = Tag_Transfer[target->apiobj.field_0x27c];
        third.position[2].x = source->apiobj.collision_position.x;
        third.position[2].z = source->apiobj.collision_position.z;
        third.position[2].y = third.height[2] * (source->apiobj.collision_max.y - source->apiobj.collision_min.y) +
                              source->apiobj.collision_min.y;
    }
    if (!target->apiobj.player_controlled) {
        return;
    }
    if (Tag_DoneFirst == 0) {
        Tag_DoneFirst = 1;
    } else if (__builtin_expect(Tag_DoneFirst == 1, 0)) {
        Tag_DoneFirst = 2;
    }
    Tag_DoneAny = 1;
}

void Tag_UpdateTransfers(i32 effect0, i32 effect1, i32 count) {
    NUVEC_ALIGNED16 position;
    if (Player[0] == NULL) {
        Tag_Transfer[0].time = Tag_TransferResetTimer;
        goto second_player;
    }
    if (!(Tag_Transfer[0].time < Tag_TransferResetTimer))
        goto second_player;
    Tag_Transfer[0].time += FRAMETIME;
    if (__builtin_expect(Tag_Transfer[0].time > Tag_TransferResetTimer, 0)) {
        Tag_Transfer[0].time = Tag_TransferResetTimer;
        goto second_player;
    }
    {
        f32 phase = Tag_Transfer[0].time + Tag_Transfer[0].time;
        {
            GameObject_s *target = Player[0];
            GameObject_s *source = Tag_Transfer[0].source;
            const f32 height = Tag_Transfer[0].height[0];
            NUVEC from = source->apiobj.collision_position;
            NUVEC to = target->apiobj.collision_position;
            from.y = (source->apiobj.collision_max.y - source->apiobj.collision_min.y) * height +
                     source->apiobj.collision_min.y;
            to.y = (target->apiobj.collision_max.y - target->apiobj.collision_min.y) * height +
                   target->apiobj.collision_min.y;
            position.x = (to.x - from.x) * phase + from.x;
            position.y = (to.y - from.y) * phase + from.y;
            position.z = (to.z - from.z) * phase + from.z;
            AddVariableShotDebrisEffectTimed1(effect0, &position, count, FRAMETIME, 0, 0, NULL);
            Tag_Transfer[0].position[0] = position;
        }
        {
            phase = NU_SIN_LUT(static_cast<i32>(phase * 16384.0f));
            GameObject_s *source = Tag_Transfer[0].source;
            GameObject_s *target = Player[0];
            const f32 height = Tag_Transfer[0].height[1];
            NUVEC from = source->apiobj.collision_position;
            NUVEC to = target->apiobj.collision_position;
            from.y = (source->apiobj.collision_max.y - source->apiobj.collision_min.y) * height +
                     source->apiobj.collision_min.y;
            to.y = (target->apiobj.collision_max.y - target->apiobj.collision_min.y) * height +
                   target->apiobj.collision_min.y;
            position.y = (to.y - from.y) * phase + from.y;
            position.x = (to.x - from.x) * phase + from.x;
            position.z = (to.z - from.z) * phase + from.z;
            position.y += 0.005f * NU_SIN_LUT(static_cast<i32>(phase * 65536.0f));
            AddVariableShotDebrisEffectTimed1(effect0, &position, count, FRAMETIME, 0, 0, NULL);
            Tag_Transfer[0].position[1] = position;
        }
        {
            phase = 1.0f - NU_SIN_LUT(static_cast<i32>(phase * 16384.0f + 16384.0f));
            GameObject_s *source = Tag_Transfer[0].source;
            GameObject_s *target = Player[0];
            const f32 height = Tag_Transfer[0].height[2];
            NUVEC from = source->apiobj.collision_position;
            NUVEC to = target->apiobj.collision_position;
            from.y = (source->apiobj.collision_max.y - source->apiobj.collision_min.y) * height +
                     source->apiobj.collision_min.y;
            to.y = (target->apiobj.collision_max.y - target->apiobj.collision_min.y) * height +
                   target->apiobj.collision_min.y;
            position.x = (to.x - from.x) * phase + from.x;
            position.y = (to.y - from.y) * phase + from.y;
            position.z = (to.z - from.z) * phase + from.z;
            position.y += 0.01f * NU_SIN_LUT(static_cast<i32>(phase * 32768.0f + 16384.0f));
            AddVariableShotDebrisEffectTimed1(effect0, &position, count, FRAMETIME, 0, 0, NULL);
            Tag_Transfer[0].position[2] = position;
        }
    }
second_player:
    if (__builtin_expect(Player[1] == NULL, 0)) {
        Tag_Transfer[1].time = Tag_TransferResetTimer;
        return;
    }
    if (!(Tag_Transfer[1].time < Tag_TransferResetTimer))
        return;
    Tag_Transfer[1].time += FRAMETIME;
    if (Tag_Transfer[1].time > Tag_TransferResetTimer) {
        Tag_Transfer[1].time = Tag_TransferResetTimer;
        return;
    }
    {
        f32 phase = Tag_Transfer[1].time + Tag_Transfer[1].time;
        {
            GameObject_s *source = Tag_Transfer[1].source;
            GameObject_s *target = Player[1];
            const f32 height = Tag_Transfer[1].height[0];
            NUVEC from = source->apiobj.collision_position;
            NUVEC to = target->apiobj.collision_position;
            from.y = (source->apiobj.collision_max.y - source->apiobj.collision_min.y) * height +
                     source->apiobj.collision_min.y;
            to.y = (target->apiobj.collision_max.y - target->apiobj.collision_min.y) * height +
                   target->apiobj.collision_min.y;
            position.x = (to.x - from.x) * phase + from.x;
            position.y = (to.y - from.y) * phase + from.y;
            position.z = (to.z - from.z) * phase + from.z;
            AddVariableShotDebrisEffectTimed1(effect1, &position, count, FRAMETIME, 0, 0, NULL);
            Tag_Transfer[1].position[0] = position;
        }
        {
            phase = NU_SIN_LUT(static_cast<i32>(phase * 16384.0f));
            GameObject_s *source = Tag_Transfer[1].source;
            GameObject_s *target = Player[1];
            const f32 height = Tag_Transfer[1].height[1];
            NUVEC from = source->apiobj.collision_position;
            NUVEC to = target->apiobj.collision_position;
            from.y = (source->apiobj.collision_max.y - source->apiobj.collision_min.y) * height +
                     source->apiobj.collision_min.y;
            to.y = (target->apiobj.collision_max.y - target->apiobj.collision_min.y) * height +
                   target->apiobj.collision_min.y;
            position.x = (to.x - from.x) * phase + from.x;
            position.y = (to.y - from.y) * phase + from.y;
            position.z = (to.z - from.z) * phase + from.z;
            position.y += 0.005f * NU_SIN_LUT(static_cast<i32>(phase * 65536.0f));
            AddVariableShotDebrisEffectTimed1(effect1, &position, count, FRAMETIME, 0, 0, NULL);
            Tag_Transfer[1].position[1] = position;
        }
        {
            phase = 1.0f - NU_SIN_LUT(static_cast<i32>(phase * 16384.0f + 16384.0f));
            GameObject_s *target = Player[1];
            GameObject_s *source = Tag_Transfer[1].source;
            const f32 height = Tag_Transfer[1].height[2];
            NUVEC from = source->apiobj.collision_position;
            NUVEC to = target->apiobj.collision_position;
            from.y = (source->apiobj.collision_max.y - source->apiobj.collision_min.y) * height +
                     source->apiobj.collision_min.y;
            to.y = (target->apiobj.collision_max.y - target->apiobj.collision_min.y) * height +
                   target->apiobj.collision_min.y;
            position.x = (to.x - from.x) * phase + from.x;
            position.y = (to.y - from.y) * phase + from.y;
            position.z = (to.z - from.z) * phase + from.z;
            position.y += 0.01f * NU_SIN_LUT(static_cast<i32>(phase * 32768.0f + 16384.0f));
            AddVariableShotDebrisEffectTimed1(effect1, &position, count, FRAMETIME, 0, 0, NULL);
            Tag_Transfer[1].position[2] = position;
        }
    }
}

void Buck_StartRiderJump(GameObject_s *rider, GameObject_s *mount) {
    u16 angle = qrand();
    f32 speed = static_cast<f32>(qrand()) * 1.5259021893143654e-05f * 0.09f + 0.01f;
    rider->apiobj.velocity.x = NuTrigTable[static_cast<u16>(angle / 2)] * speed;
    rider->apiobj.velocity.z = speed * NuTrigTable[((i32)angle + 0x4000) >> 1 & 0x7fff];
    StartJump(rider, 0);
    rider->airborne_collision_target = mount;
    rider->tag_flags |= 1;
    rider->apiobj.velocity.z = 0.0f;
    rider->apiobj.velocity.x = 0.0f;
    rider->pad_gamepad->buttons_held = 0;
    rider->pad_gamepad->buttons_pressed = 0;
    f32 clearance = mount->apiobj.upper_position.y;
    if ((mount->apiobj.character_data->game_character->flags_094[2] & 2) != 0) {
        clearance += BUCK_RIDERJUMPCLEARANCE;
    }
    clearance = clearance - rider->apiobj.lower_position.y + 0.1f;
    GAMECHARACTERDATA *character = rider->apiobj.character_data->game_character;
    if (clearance > character->jump_height) {
        rider->apiobj.velocity.y = NuFsqrt(-2.0f * character->gravity * clearance);
    }
}

void Buck_Start(GameObject_s *object, GameObject_s *rider) {
    if (LEGOCONTEXT_BUCK == -1 || LEGOACT_BUCK == -1 ||
        object->apiobj.character_model->model_data_b[LEGOACT_BUCK] == NULL)
        return;
    if (rider != NULL)
        NewRumble(rider->pad_gamepad->pad, 0.6f, 0);
    object->character_context = LEGOCONTEXT_BUCK;
    object->context_animation = LEGOACT_BUCK;
    object->context_animation_timer = AnimDuration(object->id, LEGOACT_BUCK, 0.0f, 0.0f, 1);
    if (BuckStartExtraFn != NULL)
        BuckStartExtraFn(object);
}

void Buck_MoveCode(GameObject_s *object, i32 start) {
    if (LEGOCONTEXT_BUCK != -1 && object->character_context == LEGOCONTEXT_BUCK) {
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer <= 0.0f)
            object->character_context = -1;
    } else if (object->character_context == -1 && start != 0) {
        Buck_Start(object, object->field_0xcc0);
    }
}

