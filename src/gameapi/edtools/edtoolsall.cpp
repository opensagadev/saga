// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edgra.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numusic/sfx.h"
#include <stdio.h>
#include <string.h>
#include <new>
#include "nu2api/numath/nurand.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edui_colour.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuprim.h"
#include <math.h>
#include <stdlib.h>
#include "gameframework/saveload.h"
#include <cstdio>
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/gui/apimenu_internal.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/config/fileselect.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec4.h"
#include <stdarg.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "java/java.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/nufile/nufilepak.h"
#include "legoapi/items/collect/torpedo.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/render/fx/edsplines.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nu3d/nupostparams.h"
#include "editor/edlevelall.h"
#include "gameapi/edtools/edtoolsall.h"
#include "gamelib/nuwind/nuwind.h"
#include "nu2api/nu3d/nu3d_includes.h"
#include "nu2api/numath/numath_includes.h"
#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" VuVec SpherePos1;
extern "C" VuVec SpherePos2;
extern VuVec SpherePos1;
extern VuVec SpherePos2;
EdInputContext *EdControl::Input;
extern "C" i32 edbri_nearest;
extern "C" NUMTL *edbri_mtl, *edbri_mtl_zoff;
extern "C" f32 edbri_length, edbri_width;
extern "C" i32 edbri_rotz, edbri_roty, edbri_planks, edbri_post_interval;
extern "C" i32 edbri_plank_instance_type, edbri_post_instance_type, edbri_bridges_used;
extern "C" eduiitem_s *eduiItemColourPickCreate(usize, const void *, EdUiItemCallback, char *);
extern f32 Scale;

extern "C" {
    char edgra_filter_string[16] = "GRASS";
    extern i32 edpart_emitrotx, edpart_emitroty, edpart_emitrotz;
    extern i32 edpp_copy_mode, edpp_copyrotz, edpp_copyroty, edpp_copy_source_count, edpp_copy_enclosed;
    extern NUMTL *edpp_mtl, *edpp_boxmtl;
}

__attribute__((force_align_arg_pointer)) void EdDrawPolyCylinder(VuVec const &start, VuVec const &end, i32 sides,
                                                                 i32 colour, i32 cap_colour, float radius, float limit,
                                                                 float offset);

eduiitem_s *eduiItemFileSelectorCreate(u32 data, eduiiattr_s *colours, EdUiItemCallback callback, char *text);

EdEnumControl::Item EdEnumControl::OpenClosedItems[] = {{"Open", 1}, {"Closed", 0}, {NULL, 0}};
EdEnumControl::Item EdEnumControl::OnOffItems[] = {{"On", 1}, {"Off", 0}, {NULL, 0}};
EdEnumControl::Item EdEnumControl::YesNoItems[] = {{"Yes", 1}, {"No", 0}, {NULL, 0}};

extern i32 AxisColour[8];

inline EdMatrixControl::~EdMatrixControl() {
    Destroy();
}

inline void EdMatrixControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdMatrixControl));
}

inline void EdStringControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdStringControl));
}

template <> f32 EdValueControl<f32>::MouseScale = 100.0f;

template <> SAGA_HOST_LINKABLE_DTOR EdValueControl<f32>::~EdValueControl() {
}

template <> inline void EdValueControl<f32>::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdValueControl<f32>));
}

SAGA_HOST_LINKABLE_DTOR
EdFloatControl::~EdFloatControl() {
}

inline void EdFloatControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdFloatControl));
}

template <> void EdValueControl<f32>::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    EdValueControl<f32> *control =
        new (theMemoryManager.AllocPool(sizeof(EdValueControl<f32>), 1)) EdValueControl<f32>();
    control->format = format;
    control->value_type = value_type;
    control->minimum = minimum;
    control->maximum = maximum;
    control->reference = member;
    control->object = target;
    f32 value;
    member->GetMemberData(target, value_type, &value, 0);
    char text[128];
    sprintf(text, format, value);
    control->item = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected, cbChanged,
                                       cbButton, 2, member->name, text);
    eduiMenuAddItem(menu, control->item);
}

template <> void EdValueControl<f32>::Refresh() {
    f32 value;
    reference->GetMemberData(object, value_type, &value, 0);
    char text[128];
    sprintf(text, format, value);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), text);
}

template <> void EdValueControl<f32>::cbChanged(eduimenu_s *, eduiitem_s *item, u32) {
    EdValueControl<f32> *control = static_cast<EdValueControl<f32> *>(item->data_ptr);
    f32 value = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
    if (value < control->minimum)
        value = control->minimum;
    if (value > control->maximum)
        value = control->maximum;
    control->reference->SetMemberData(control->object, control->value_type, &value, 0, NULL);
    char text[128];
    sprintf(text, control->format, value);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), text);
}

template <>
__attribute__((force_align_arg_pointer)) void EdValueControl<f32>::cbButton(eduimenu_s *, eduiitem_s *item, u32) {
    EdValueControl<f32> *control = static_cast<EdValueControl<f32> *>(item->data_ptr);
    nupad_s *pad = EdControl::Input->pad;
    f32 sensitivity = 0.01f;
    if (control->maximum - control->minimum < 5.0f)
        sensitivity = 0.001f;
    static_cast<edui_prop_s *>(item)->unknown_property_flags |= 0x20;
    f32 initial_value = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
    f32 dx = 0.0f;
    // The original keeps the change, shared with the cursor delta, at a 16-byte aligned stack address.
    f32 change __attribute__((aligned(16))) = 0.0f;
    if (pad && (pad->digital_buttons & EDUI_CURSOR_PRIMARY)) {
        f32 left_y = static_cast<i32>(pad->analog_left_y);
        if (left_y > 128.0f)
            change = 10.0f * sensitivity * (left_y - 128.0f);
        if (pad->analog_left_y < 128)
            change = -10.0f * sensitivity * (128.0f - left_y);
        f32 right_y = static_cast<i32>(pad->analog_right_y);
        if (right_y > 128.0f)
            change = 0.1f * sensitivity * (right_y - 128.0f);
        if (pad->analog_right_y < 128)
            change = -0.1f * sensitivity * (128.0f - right_y);
    } else {
        eduiGetCursorDelta(&dx, &change);
        change *= MouseScale;
    }
    initial_value -= change;
    char text[128];
    sprintf(text, control->format, initial_value);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), text);
    control = static_cast<EdValueControl<f32> *>(item->data_ptr);
    f32 value = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
    if (value < control->minimum)
        value = control->minimum;
    if (value > control->maximum)
        value = control->maximum;
    control->reference->SetMemberData(control->object, control->value_type, &value, 0, NULL);
    char updated_text[128];
    sprintf(updated_text, control->format, value);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), updated_text);
}

inline EdVectorControl::~EdVectorControl() {
    Destroy();
}

inline void EdVectorControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdVectorControl));
}

void EdClassInterface::ClearLevel(i32) {
}

void EdClassInterface::DefunctObject(void *) {
}

void EdClassInterface::ReviveObject(void *) {
}

void EdClassInterface::SetObjectGuid(void *, i32) {
}

i32 EdClassInterface::GetObjectGuid(void *) {
    return 0;
}

i32 EdClassInterface::GetConstructorData(void *, void *, i32) {
    return 0;
}

void EdClassInterface::Construct(void *, void *) {
}

void EdClassInterface::Process(void *, EdInputContext &) {
}

void EdClassInterface::Render(void *, i32) {
}

void EdClassInterface::EnterEditor() {
}

void EdClassInterface::ExitEditor() {
}

void EdClassInterface::EnterLevel() {
}

void EdClassInterface::ExitLevel() {
}

void EdClassInterface::UpdateLists(MemoryBuffer *, MemoryBuffer *) {
}

void EdClassInterface::PreLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
}

void EdClassInterface::PostLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
}

void EdClassInterface::PreSaveInitialisation() {
}

void EdClassInterface::PostSaveInitialisation() {
}

void EdClassInterface::SerialiseObject(EdStream &, void *) {
}

void EdClassInterface::AddMenuItems(eduimenu_s *) {
}

void EdClassInterface::Import() {
}

void SplineHelper::Flush() {
    last_object = NULL;
    first_object = NULL;
    object_count = 0;
}

void KnotHelper::Flush() {
}

inline EdClassObjectNameControl::~EdClassObjectNameControl() {
}

inline void EdClassObjectNameControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdClassObjectNameControl));
}

inline EdSubSystem::~EdSubSystem() {
}

__attribute__((weak)) void EdSubSystem::SubInitialise(variptr_u &, variptr_u &, i32) {
}

__attribute__((weak)) void EdSubSystem::SubReset() {
}

__attribute__((weak)) void EdSubSystem::SubProcess(float) {
}

__attribute__((weak)) void EdSubSystem::SubRender() {
}

SAGA_HOST_LINKABLE_DTOR
EdControl::~EdControl() {
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

#if defined(__i386__)
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone, regparm(2), sseregparm))
#else
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone))
#endif
static HSV2RGB_BODY_ATTR void
HSV2RGBBody(f32 hue, f32 saturation, f32 value, f32 *red, f32 *green, f32 *blue) __asm__("_ZL7HSV2RGBfffPfS_S_.part.18");

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

// Editor UI callbacks retained in the original editor's menu code.
#pragma GCC push_options
#pragma GCC optimize("O2", "omit-frame-pointer")

static eduimenu_s *submenu1;
static eduimenu_s *submenu0;
static eduimenu_s *testmenu0;
static i32 selection_value;
static f32 cliph;
static f32 clips;
static f32 clipv;
static f32 clipg;
static f32 clipboard_r = 0.5f;
static f32 clipboard_g = 0.5f;
static f32 clipboard_b = 0.5f;
static eduimenu_s *colourmenu;
static u32 *cp_rgba;
static f32 cpt_r;
static f32 cpt_g;
static f32 cpt_b;
static f32 cpt_a;
static f32 *cp_r;
static f32 *cp_g;
static f32 *cp_b;
static f32 *cp_a;
static edui_colour_pick_s *cp_item;
static eduiitem_s *cp_info;
static i32 (*cp_process)(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
static eduiitem_s *cp_copy;
static eduiitem_s *cp_paste;

static __attribute__((always_inline)) inline u32 cpPackColour(f32 red, f32 green, f32 blue, f32 alpha) {
    return (static_cast<i32>(alpha * 255.0f) << 24) | ((static_cast<i32>(blue * 255.0f) & 0xff) << 16) |
           ((static_cast<i32>(green * 255.0f) & 0xff) << 8) | (static_cast<i32>(red * 255.0f) & 0xff);
}

static __attribute__((always_inline)) inline void cpUpdateInfo(eduiitem_s *info, edui_colour_pick_s *picker) {
    char text[256];
    if (info->data != 0) {
        sprintf(text, "R:%1.2f G:%1.2f B:%1.2f", picker->red, picker->green, picker->blue);
    } else {
        sprintf(text, "H:%1.2f S:%1.2f V:%1.2f", picker->hue, picker->saturation, picker->value);
    }
    eduiItemSetText(info, text);
}

static __used__ void cbColourPickSel(eduimenu_s *menu, eduiitem_s *, u32);
static __used__ void cbToggleIndicatorMode(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void cbCopy(eduimenu_s *, eduiitem_s *, u32);
static __used__ void cbPaste(eduimenu_s *, eduiitem_s *, u32);
static __used__ i32 cbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, float delta_time, nupad_s *pad);

extern "C" {
static __used__ void cbGradChange(void);
static __used__ void cbSel(eduimenu_s *, eduiitem_s *item);
static __used__ void cbSubMenu(eduimenu_s *menu, eduiitem_s *item);
static __used__ void cbgpcfgAdd(eduimenu_s *menu, eduiitem_s *item, u32 value);
static __used__ void cbgpcfgCPPress(eduimenu_s *menu, eduiitem_s *item, u32 value);
static __used__ void cbgpcfgCopy(eduimenu_s *menu, eduiitem_s *item, u32 value);
static __used__ void cbgpcfgDel(eduimenu_s *menu, eduiitem_s *item, u32 value);
static __used__ void cbgpcfgPaste(eduimenu_s *menu, eduiitem_s *item, u32 value);
} // extern "C"

#pragma GCC pop_options

static i32 editor_return;
static i32 bShowCursor = 1;
static NUQFNT *edui_font;
static i32 edui_donotdraw;
static i32 item_width;
static void *dir_list;
static i32 dir_size;
static eduimenu_s *filepick_menu;
static edui_file_pick_s *filepick_item;
static char ed_levelfile[256];
static f32 edui_font_scale_x = 0.9f;
static f32 edui_font_scale_y = 0.9f;
static f32 edmain_menu_scale = 1.0f;
static u32 edui_cursor_colour = 0xff000000;
static char eduiPropTextEdit[256];
static char eduiPropTextStore[256];
static char *textrow[4];
static char TextPickCopyBuffer[0x40];
static i32 cursor_flash;

extern "C" {
    extern u32 edblack[4];
    extern char edbits_general_save_directory[256];
    extern char edbits_general_save_name[256];
    extern char edbits_general_save_extension[256];
}

void cbFileSaveEffects(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbClipboardMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbOrphanListMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbDeleteOrphans(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbGhostMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbStarMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeDistortX(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeDistortY(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetScaleFactor(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSoundIDMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeGenRate(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeETime(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlRepeatBoxMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeRampTime(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchType(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCutClipboard(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbPasteClipboard(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbEmptyClipboard(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOn(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOnRan(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOffRan(eduimenu_s *, eduiitem_s *, u32);

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern u32 edblack[4];
    extern NUMTL *DebMat[];
}

static void cbPtlChangeGrav(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeEmitVel(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeSScale(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSelReadout(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeDamageFlags(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDamageFlagMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlTorusMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbCancelChangeNameMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangeTextureSelect(eduimenu_s *, eduiitem_s *, u32);

extern "C" {
    extern u32 edblack[4];
    extern u32 edgrey[4];
    extern u32 eddarkred[4];
    extern DEBRISGENERATOR gensorttab[13];
    extern DEBRISMOMENTUMADJUSTER gencodetab[7];
}

static void cbPtlQuickDeleteType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSelTextureType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlTextureSelectMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlAddEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCopyEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDeleteEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeNameMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlQuickDeleteMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlReadoutMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbFileLoadEffects(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSelType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangePriority(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlToggleDynamicPriority(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSetFacing(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbScaleEffectMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlEmitVelMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlGravMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlVarStartMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlStartVelMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlVarEmitMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeGenRateMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeETimeMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlEmitTimeMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCutOffMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCopySize(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyScaleFactor(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeRepeatFlag(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCutOn(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeDrawCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeSoundCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCameraCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeNumCollSpheres(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDefaultCollEnv(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeTorusLife(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeTorusRad1(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeTorusRad2(eduimenu_s *, eduiitem_s *, u32);

// Particle-list editor UI/menu callbacks.

static void edptlcbScaleEffectMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbApplyScaleFactor(eduimenu_s *menu, eduiitem_s *, u32);

// Particle editor UI/menu callbacks.

static void cbPtlSelType(eduimenu_s *menu, eduiitem_s *item, u32);
static void cbPtlCopySize(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlGravMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void cbPtlAddEffect(eduimenu_s *menu, eduiitem_s *, u32);
static void cbPtlSetFacing(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlTorusMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void cbPtlChangeGrav(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlCopyEffect(eduimenu_s *menu, eduiitem_s *, u32);
static void cbPtlCutOffMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void cbPtlSelReadout(eduimenu_s *, eduiitem_s *item, u32);
static void cbChangeNameMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void cbPtlChangeCutOn(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlEmitVelMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void cbPtlReadoutMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void cbPtlVarEmitMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void cbChangeETimeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void cbChangeTorusLife(eduimenu_s *, eduiitem_s *item, u32);
static void cbChangeTorusRad1(eduimenu_s *, eduiitem_s *item, u32);
static void cbChangeTorusRad2(eduimenu_s *, eduiitem_s *item, u32);
static void cbFileLoadEffects(eduimenu_s *parent, eduiitem_s *, u32);
static void cbPtlChangeCutOff(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeSScale(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlDeleteEffect(eduimenu_s *menu, eduiitem_s *, u32);
static void cbPtlEmitTimeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void cbPtlStartVelMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void cbPtlVarStartMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void cbPtlChangeEmitVel(eduimenu_s *, eduiitem_s *item, u32);
static void cbChangeGenRateMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void cbPtlChangePriority(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlDamageFlagMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void cbPtlDefaultCollEnv(eduimenu_s *menu, eduiitem_s *, u32);
static void cbPtlSelTextureType(eduimenu_s *menu, eduiitem_s *item, u32);
static void cbPtlQuickDeleteMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void cbPtlQuickDeleteType(eduimenu_s *menu, eduiitem_s *item, u32);
static void cbPtlChangeDrawCutOff(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeRepeatFlag(eduimenu_s *, eduiitem_s *item, u32);
static void cbChangeNumCollSpheres(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeDamageFlags(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeSoundCutOff(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlTextureSelectMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void cbPtlChangeCameraCutOff(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeTextureSelect(eduimenu_s *menu, eduiitem_s *item, u32);
static void cbPtlToggleDynamicPriority(eduimenu_s *, eduiitem_s *item, u32);
static void cbCancelChangeNameMenu(eduimenu_s *menu, eduimenu_s *);

void cbFileSaveEffects(eduimenu_s *parent, eduiitem_s *, u32);

static void edptlChangeRepeatBox(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbDrawflagMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyStarRatio(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyGhostTime(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyNumGhosts(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetMasterGroup(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetMaxThin(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbTestDetailMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchId(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchVar(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbStartPage(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbStopPage(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbClearPage(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlRepeatBoxMenu(eduimenu_s *parent, eduiitem_s *, u32);

// Particle list editor subsystem stubs (static, internal linkage).

static void edptlcbStarMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbStopPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbClearPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbGhostMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbStartPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetMaxThin(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetSwitchId(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSoundIDMenu(eduimenu_s *parent, eduiitem_s *item, u32);
static void edptlcbCutClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbDrawflagMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetSwitchVar(eduimenu_s *, eduiitem_s *item, u32);
static void edptlChangeRepeatBox(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbClipboardMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbDeleteOrphans(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32);
static void edptlcbApplyGhostTime(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbApplyNumGhosts(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOffRan(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOff(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOnRan(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOn(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeETime(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeGenRate(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbApplyStarRatio(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbChangeDistortX(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbChangeDistortY(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbChangeRampTime(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbEmptyClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbOrphanListMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbPasteClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbSetMasterGroup(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetScaleFactor(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSwitchTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbTestDetailMenu(eduimenu_s *parent, eduiitem_s *, u32);

extern "C" {
    __attribute__((visibility("hidden"))) void tpentOnEnter(eduimenu_s *, eduiitem_s *, u32);
    __attribute__((visibility("hidden"))) void tpentPropOnEnter(eduimenu_s *, eduiitem_s *, u32);
}

// The editor's draw helpers evaluate their arguments even during layout-only
// passes; only the renderer call is suppressed by edui_donotdraw.
static inline void eduiDrawRect(i32 x, i32 y, i32 width, i32 height, i32 colour, NUMTL *material) {
    if (!edui_donotdraw)
        NuRndrRect2di(x, y, width, height, colour, material);
}

static inline void eduiDrawLineRect(i32 x, i32 y, i32 width, i32 height, i32 colour, NUMTL *material) {
    if (!edui_donotdraw)
        NuRndrLineRect2di(x, y, width, height, colour, material);
}

static inline void eduiDrawGradRect(i32 x, i32 y, i32 width, i32 height, i32 *colours, NUMTL *material) {
    if (!edui_donotdraw)
        NuRndrGradRect2di(x, y, width, height, colours, material);
}

static inline void eduiDrawLine(i32 x0, i32 y0, i32 x1, i32 y1, i32 colour, NUMTL *material) {
    if (!edui_donotdraw)
        NuRndrLine2di(x0, y0, x1, y1, colour, material);
}

static inline void eduiDrawLineStrip(i32 *points, f32 *uvs, i32 count, i32 colour, NUMTL *material) {
    if (!edui_donotdraw)
        NuRndrLineStrip2di(points, uvs, count, colour, material);
}

static inline void eduiDrawTriStrip(i32 *points, f32 *uvs, i32 count, i32 colour, NUMTL *material) {
    if (!edui_donotdraw)
        NuRndrTriStrip2di(points, uvs, count, colour, material);
}

static inline void eduiDrawFontSet(NUQFNT *font) {
    if (!edui_donotdraw)
        NuQFntSet(font);
}

static inline void eduiSetFontColour(NUQFNT *font, u32 colour) {
    if (!edui_donotdraw)
        NuQFntSetColour(font, colour);
}

// Some original callers expect the draw to happen and keep the renderer call on the fallthrough path.
static inline void eduiDrawRectLikely(i32 x, i32 y, i32 width, i32 height, i32 colour, NUMTL *material) {
    if (__builtin_expect(!edui_donotdraw, 1))
        NuRndrRect2di(x, y, width, height, colour, material);
}

static inline void eduiDrawLineRectLikely(i32 x, i32 y, i32 width, i32 height, i32 colour, NUMTL *material) {
    if (__builtin_expect(!edui_donotdraw, 1))
        NuRndrLineRect2di(x, y, width, height, colour, material);
}

static inline void eduiDrawGradRectLikely(i32 x, i32 y, i32 width, i32 height, i32 *colours, NUMTL *material) {
    if (__builtin_expect(!edui_donotdraw, 1))
        NuRndrGradRect2di(x, y, width, height, colours, material);
}

static inline void eduiDrawLineLikely(i32 x0, i32 y0, i32 x1, i32 y1, i32 colour, NUMTL *material) {
    if (__builtin_expect(!edui_donotdraw, 1))
        NuRndrLine2di(x0, y0, x1, y1, colour, material);
}

static inline void eduiDrawLineStripLikely(i32 *points, f32 *uvs, i32 count, i32 colour, NUMTL *material) {
    if (__builtin_expect(!edui_donotdraw, 1))
        NuRndrLineStrip2di(points, uvs, count, colour, material);
}

static inline void eduiDrawTriStripLikely(i32 *points, f32 *uvs, i32 count, i32 colour, NUMTL *material) {
    if (__builtin_expect(!edui_donotdraw, 1))
        NuRndrTriStrip2di(points, uvs, count, colour, material);
}

static inline void eduiDrawFontSetLikely(NUQFNT *font) {
    if (__builtin_expect(!edui_donotdraw, 1))
        NuQFntSet(font);
}

static inline void eduiSetFontColourLikely(NUQFNT *font, u32 colour) {
    if (__builtin_expect(!edui_donotdraw, 1))
        NuQFntSetColour(font, colour);
}

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
static void eduiFntPrintEx(void *font, int x, int y, int alignment, char *format, ...) {
    char text[1024];
    va_list arguments;
    va_start(arguments, format);
    NuVSPrintf(text, format, arguments);
    va_end(arguments);
    eduiFntPrintExGeneric(font, x, y, alignment, const_cast<char *>("%s"), text);
}
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

static void eduiFntPrintEx(void *font, float x, float y, int alignment, char *format, ...) {
    va_list arguments;
    va_start(arguments, format);
    eduiFntPrintEx(font, static_cast<i32>(x * 16.0f), static_cast<i32>(y * 8.0f), alignment, format, arguments);
    va_end(arguments);
}

// Only caller prints the property edit buffer left-aligned.
static __attribute__((noinline, noclone)) void eduiFntPrintExEditBuffer(void *font, float x, float y, int alignment,
                                                                        char *format, ...)
    __asm__("_ZL14eduiFntPrintExPvffiPcz.constprop.66");

static __used__ void eduiFntPrintClipEx(void *font, float x, float y, int alignment, float clip_x, float clip_width,
                                        char *format, ...);
static __attribute__((noinline, noclone)) void eduiFntPrintClipEx(void *font, i32 x, i32 y, i32 alignment, i32 clip_x,
                                                                  i32 clip_width, char *format, ...)
    __asm__("_ZL18eduiFntPrintClipExPviiiiiPcz.constprop.70");
static i32 show_x_scale_help;
static i32 show_y_scale_help;

extern "C" {
    static i32 eduicbProcessGradPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessFilePick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static __attribute__((noinline, noclone)) void
    eduicbAttachDirectoryList(eduimenu_s *, edui_file_pick_s *) __asm__("eduicbAttachDirectoryList.isra.32");
    static void eduicbSelectDirectoryEntry(eduimenu_s *, eduiitem_s *, u32);
    static void eduicbSelectDirectoryExit(eduimenu_s *, eduiitem_s *, u32);
    static i32 eduicbProcessTextPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static __attribute__((noinline, noclone)) i32
    eduicbProcessPropKeyboard(eduimenu_s *, edui_prop_s *) __asm__("eduicbProcessPropKeyboard.isra.27");
    static i32 eduicbRenderExpander(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderFilter(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderProp(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    typedef void (*EDBITSPLAYSOUNDCALLBACK)(NUVEC *, i32);
    typedef i32 (*EDBITSREQUESTSOUNDCALLBACK)(char *);

    // GCC 4.7 emits these file-scope objects in reverse declaration order, so
    // each group below mirrors the original linked order in reverse.
    i32 bCameraEnabled = 1;
    i32 edui_filepick_listsize = 8;
    u32 eduiitem_context_outline_colour = 0x0000ff00;
    u32 eduiitem_selected_outline_colour = 0x80ffffff;
    u32 eduimenu_selected_outline_colour = 0x80ffffff;
    u32 eduimenu_outline_colour = 0x80404040;
    f32 edmain_default_camera_speed = 1.0f;
    i32 ed_main_menu_h = 320;
    i32 ed_main_menu_w = 180;
    i32 ed_main_menu_y = 50;
    i32 ed_main_menu_x = 220;
    eduiiattr_s ed_attr = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    i32 eduiPropTextPos = -1;
    i32 edmain_render_interacts;

    eduimenu_s *edui_messagemenu;
extern u32 edblack[4];
extern u32 edgrey[4];
extern u32 eddarkred[4];
    NUMTL *uimtls[5];
    i32 ui_bgmtl;
    i32 ui_outmtl;

extern NUMTL * edpp_mtl;
extern NUMTL * edpp_boxmtl;
extern i32 edpp_copy_mode;
extern i32 edpp_copy_enclosed;
extern i32 edpp_copy_source_count;
extern i32 edpp_copyrotz;
extern i32 edpp_copyroty;

}

extern "C" {
    extern NUMTL *edbri_mtl, *edbri_mtl_zoff;
    extern eduimenu_s *edbri_options_menu, *edbri_active_menu;
    extern i32 edbri_nearest, edbri_plank_instance_type, edbri_post_instance_type;
}

static NUVEC *ed_loc;

extern "C" {
extern char edbits_general_save_directory[256];
extern char edbits_general_save_name[256];
extern char edbits_general_save_extension[256];
extern EDBITSPLAYSOUNDCALLBACK edbitsPlaySound;
extern EDBITSREQUESTSOUNDCALLBACK edbitsRequestSound;
extern eduimenu_s * edbri_active_menu;
extern eduimenu_s * edbri_options_menu;
extern eduimenu_s * edbri_dpadmode_menu;
extern eduimenu_s * edbri_plankinstance_menu;
extern eduimenu_s * edbri_postinstance_menu;
extern eduimenu_s * edbri_plankcount_menu;
extern eduimenu_s * edbri_bridgeproperties_menu;
extern eduimenu_s * edbri_ropecolour_menu;
extern NUMTL * edbri_mtl;
extern NUMTL * edbri_mtl_zoff;
extern i32 edbri_cam_ax;
extern i32 edbri_cam_ay;
extern i32 edbri_nearest;
extern i32 edbri_plank_instance_type;
extern i32 edbri_post_instance_type;
extern i32 edbri_bridges_used;
extern i32 edbri_rotz;
extern i32 edbri_roty;
extern i32 edbri_mode;
extern f32 edbri_length;
extern f32 edbri_width;
extern i32 edbri_planks;
extern i32 edbri_post_interval;
}

static edcam_s gp_cam = {
    {0.0f, 0.0f, 0.0f},
    0,
    0,
    -2.0f,
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 1.0f},
    1,
    1,
    {0.0f, 0.0f, 0.0f},
    0,
    0,
    {0.0015625f, 0.0015625f, 0.0015625f},
    2,
    2,
    0.0078125f,
    0.5f,
    0.2f,
    0.2f,
    4.0f,
    0.15f,
    0.2f,
    0.01f,
    0.1f,
    EDCAM_FREEDOM_POSITION_X | EDCAM_FREEDOM_POSITION_Y | EDCAM_FREEDOM_POSITION_Z | EDCAM_FREEDOM_PITCH |
        EDCAM_FREEDOM_YAW | EDCAM_FREEDOM_DISTANCE,
    {0, 0, 0},
};

static NUCAMERA *edmaincam = NULL;
static NUCAMERA *edinternalcam = NULL;
static eduimenu_s *active_menu;
static eduimenu_s *default_active_menu;
static eduimenu_s *processing_menu;
static f32 edui_cursor_x = 50.0f;
static f32 edui_cursor_y = 50.0f;
static f32 edui_cursor_dx;
static f32 edui_cursor_dy;
static i32 edui_cursor_locked;
static u32 edui_cursor_buttons;
static u32 edui_cursor_buttons_old;
static u32 edui_cursor_buttons_db;
static f32 slider_acceleration;
static i32 numInteracts;
static eduimenu_s *ed_main_menu;
static eduimenu_s *ed_cfg_menu;
static i32 ed_init;
static ed_module_s *ed_list;
static i32 ed_module_active;
static ed_module_s *ed_curr;
static edui_interact_s eduiInteracts[64];
static edui_interact_s *eduiInteractLocked;

extern "C" {
    static void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *, u32);
}

extern "C" {
    i32 PadFlyMode = 0;
    NUMTX *ed_remap_mtx = NULL;
    i32 edmain_cursor_enabled = 0;

void edanimUpdateObjects(float elapsed);

void edbitsRegisterPlaySound(EDBITSPLAYSOUNDCALLBACK callback);
void edbitsRegisterRequestSound(EDBITSREQUESTSOUNDCALLBACK callback);
    // Shared editor font; edmainInit receives and stores the font handle.
    void *ed_fnt;

    extern f32 globaltime;

i32 edppFindAllSounds(i32 page, NUVEC *positions, i32 (*sounds)[4], i32 capacity, i32 skip);
void edppSetSaveName(u32 page, char *name);
edui_textpicker_s *eduiAddTextPickEntEx(eduimenu_s *menu, eduiitem_s *source, EdUiItemCallback callback);
    i32 bUsingMenuFocus;
eduiitem_s *eduiItemColourPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemColourSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           f32 minimum, f32 range, f32 value, u8 red, u8 green, u8 blue, char *text);
eduiitem_s *eduiItemGraphCreate(usize data, const void *colours, EdUiItemCallback callback, nugraph_s *graph,
                                    i32 width, i32 height);
eduiitem_s *eduiItemGreyPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemNumberCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);
eduiitem_s *eduiItemSelWithClipColourCreate(usize data, const void *colours, i32 selected, i32 group,
                                                EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemTextSelectorCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           i32 count, i32 selected, char *text, char **options);
    eduiitem_s *edui_last_item;

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

    static inline i32 eduiPickerDeadZone32(i32 raw_value) {
        i32 value = raw_value - 128;
        if (value > 0) {
            if (value < 32)
                return 0;
            return (value - 32) * 255 / 223;
        }
        if (value > -32)
            return 0;
        return (value + 32) * 255 / 223;
    }

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
    static void eduiDrawHueValueBand(i32 x, i32 y, i32 width, i32 height, f32 start, f32 end, u32 first, u32 second)
        __asm__("_ZL19eduiDrawGrad4StepsViiiiffiiiii.constprop.59");
    static i32 edui_grad_steps = 8;
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

// Text-picker commit callbacks. Retail keeps them unmangled and local.
extern "C" {
__attribute__((visibility("hidden"), optimize("O3"))) void tpentOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                            unsigned int buttons);

__attribute__((visibility("hidden"), optimize("O3"))) void tpentPropOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                                unsigned int buttons);
}

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern f32 HUB_EPISODETITLEY;
extern i16 tSELECT, tSELECTED, tSELECTING;
extern "C" void NuIOS_RecordFlurryEvent(char *event_name);
extern i16 tLOCKED;
extern i16 tPOWERBRICK;
extern f32 newgamecamtime;
extern "C" f32 MENUBOTY;
extern i32 memcard_deleteneeded;
extern i32 memcard_deletestarted;
extern i32 memcard_deletefailed;
extern i32 memcard_formatting;
extern i32 memcard_formatme;
extern i32 memcard_formatfailed;

extern void (*drawslotinfofn)(f32, f32, i32, i32);
extern void (*drawslotsfn)(MENU *, f32);

void MenuInitialiseEx(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                      void (*draw_save_slots_info_fn)(f32, f32, i32, i32), i32 is_fade_enabled, i32 is_shadow_enabled);

void MenuInitialise(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                    void (*draw_save_slots_fn)(MENU *, f32), i32 is_fade_enabled, i32 is_shadow_enabled);

extern "C" {
extern f32 MENUBOTY;
extern f32 MENUENTRYEXWIDTH;

void MenuSetPreDrawFn(void (*draw_fn)(MENU *));

void MenuSetHeaderDrawFn(void (*draw_fn)(void));
} // extern "C"

struct ani3_animheader_s;
struct nuanimdatachunk_s;

extern "C" {
extern GLuint g_colorRenderbuffer;
extern GLuint g_depthRenderbuffer;
}

using NUHGOBJVIDEOMEMFN = void (*)(nuhgobj_s *);

void NuPadRecordEndFrame(void);                      // nupad_interface.cpp

extern "C" {
    void NuShaderManagerSetfv(i32 semantic, const f32 *values);

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

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

    extern void (*preRenderFlashingHack)(void);
    extern void (*postRenderFlashingHack)(void);
    extern void (*nuapi_endframe_callbackfn)(void);

void NuIOS_RecordFlurryEvent(char *event_name);

    extern "C++" void NuAnimDataChunkDestroy(nuanimdatachunk_s *chunk);

void NuQFntPrint2dW(NUQFNT *font, u16 *text);
void NuQFntPrint3DU(NUQFNT *font, char *text);
void NuQFntPrint3DW(NUQFNT *font, u16 *text);
void NuQFntPrintV(NUQFNT *font, const char *format, va_list arguments);

    extern nurenderscene_s currentScene;

    void NuWindUpdateArray(NUVEC **);

#include "nu2api/nucore/nucore_plain_types.h"
void NuHGobjFromVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuHGobjToVideoMem(NUHGOBJVIDEOMEMFN callback);

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

    NUKEYCHAR NuKeyChar[65] = {
        {0x2, 0x31, 0x21, {0, 0}},  {0x3, 0x32, 0x22, {0, 0}},  {0x4, 0x33, 0xa3, {0, 0}},  {0x5, 0x34, 0x24, {0, 0}},
        {0x6, 0x35, 0x25, {0, 0}},  {0x7, 0x36, 0x5e, {0, 0}},  {0x8, 0x37, 0x26, {0, 0}},  {0x9, 0x38, 0x2a, {0, 0}},
        {0xa, 0x39, 0x28, {0, 0}},  {0xb, 0x30, 0x29, {0, 0}},  {0xc, 0x2d, 0x5f, {0, 0}},  {0xd, 0x3d, 0x2b, {0, 0}},
        {0xf, 0x09, 0x09, {0, 0}},  {0x10, 0x71, 0x51, {0, 0}}, {0x11, 0x77, 0x57, {0, 0}}, {0x12, 0x65, 0x45, {0, 0}},
        {0x13, 0x72, 0x52, {0, 0}}, {0x14, 0x74, 0x54, {0, 0}}, {0x15, 0x79, 0x59, {0, 0}}, {0x16, 0x75, 0x55, {0, 0}},
        {0x17, 0x69, 0x49, {0, 0}}, {0x18, 0x6f, 0x4f, {0, 0}}, {0x19, 0x70, 0x50, {0, 0}}, {0x1a, 0x5b, 0x7b, {0, 0}},
        {0x1b, 0x5d, 0x7d, {0, 0}}, {0x1e, 0x61, 0x41, {0, 0}}, {0x1f, 0x73, 0x53, {0, 0}}, {0x20, 0x64, 0x44, {0, 0}},
        {0x21, 0x66, 0x46, {0, 0}}, {0x22, 0x67, 0x47, {0, 0}}, {0x23, 0x68, 0x48, {0, 0}}, {0x24, 0x6a, 0x4a, {0, 0}},
        {0x25, 0x6b, 0x4b, {0, 0}}, {0x26, 0x6c, 0x4c, {0, 0}}, {0x27, 0x3b, 0x3a, {0, 0}}, {0x28, 0x27, 0x40, {0, 0}},
        {0x2b, 0x5c, 0x3f, {0, 0}}, {0x2c, 0x7a, 0x5a, {0, 0}}, {0x2d, 0x78, 0x58, {0, 0}}, {0x2e, 0x63, 0x43, {0, 0}},
        {0x2f, 0x76, 0x56, {0, 0}}, {0x30, 0x62, 0x42, {0, 0}}, {0x31, 0x6e, 0x4e, {0, 0}}, {0x32, 0x6d, 0x4d, {0, 0}},
        {0x33, 0x2c, 0x3c, {0, 0}}, {0x34, 0x2e, 0x3e, {0, 0}}, {0x35, 0x2f, 0x00, {0, 0}}, {0x37, 0x2a, 0x2a, {0, 0}},
        {0x39, 0x20, 0x20, {0, 0}}, {0x47, 0x37, 0x37, {0, 0}}, {0x48, 0x38, 0x38, {0, 0}}, {0x49, 0x39, 0x39, {0, 0}},
        {0x4a, 0x2d, 0x2d, {0, 0}}, {0x4b, 0x34, 0x34, {0, 0}}, {0x4c, 0x35, 0x35, {0, 0}}, {0x4d, 0x36, 0x36, {0, 0}},
        {0x4e, 0x2b, 0x2b, {0, 0}}, {0x4f, 0x31, 0x31, {0, 0}}, {0x50, 0x32, 0x32, {0, 0}}, {0x51, 0x33, 0x33, {0, 0}},
        {0x52, 0x30, 0x30, {0, 0}}, {0x53, 0x2e, 0x2e, {0, 0}}, {0x8d, 0x3d, 0x3d, {0, 0}}, {0xb5, 0x2f, 0x2f, {0, 0}},
        {0x0, 0x00, 0x00, {0, 0}},
    };
    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    extern "C++" {
        static __attribute__((noinline, noclone)) void
        ClipInstTree(NuInstanceTree_s *tree,
                     i32 tree_index) __asm__("_ZL12ClipInstTreeP16NuInstanceTree_siP16nudisplayscene_sPc.isra.92");
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

extern "C" {
    static void eduicbItemExpanderClose(edui_expander_s *expander) {
        for (eduiitem_s *item = expander->first_child; item; item = item->next) {
            if (item->type == EDUI_ITEM_EXPANDER)
                eduicbItemExpanderClose(static_cast<edui_expander_s *>(item));
            if (item == expander->last_child)
                break;
        }
        if (expander->first_child && expander->open) {
            expander->next = expander->last_child->next;
            if (expander->last_child->next)
                expander->last_child->next->previous = expander;
            expander->first_child->previous = NULL;
            expander->open = 0;
        }
    }

}

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

void SetLevelExBlowupFunc(i32 (*callback)(GIZMOBLOWUP_s *, i32));

extern "C" {
    i32 edmain_auto_speed = 1;
} // extern "C"

extern "C" i16 id_LANDSPEEDER;
extern f32 newgamecamtime, cutscenecam_focalLength, CamStopBlend, EMPERORFIGHTA_CAMDYHACK;
extern i32 CUTCAMONLY, GUNSHIPAHACK, netcamera, complexsockposition_forcesock, movegamecamera_forcesock;
extern NUMTX cutscenecammtx, CutCamMtx;
GameObject_s *FindNearestGameObject(NUVEC *, GameObject_s *, u32, f32, f32, i32, i32, i32, f32 *, i32,
                                    i32 (*)(GameObject_s *), bool);

extern f32 EMPERORFIGHTA_CAMDYHACK;
extern i32 movegamecamera_forcesock;
extern NUMTX CutCamMtx;

extern "C" {
    i32 near_clip_at_cursor;
} // extern "C"

static i32 edfile_media;
i32 edfile_handle = -1;
i32 edfile_write_flag;

static void *edfile_pakfile;

char edfile_buffer[0x1000];
i32 edfile_buffer_pointer;
i32 edfile_lock[2];

i32 edfile_readwrongendianess;
i32 edfile_mcresult;

extern i32 memcard_deleteneeded, memcard_deletestarted, memcard_deletefailed;
extern i32 memcard_formatme, memcard_formatting, memcard_formatfailed;
i32 numfilteroutblocks;
char filteroutblocks[8][12];
i32 numfilterblocks;
char filterblocks[8][96];

char FS_FileList[0x10000];
char FS_LastFileName[64];
char FS_Path[256];
i32 FS_NumFiles;
i32 FS_CurrentPosFileNum;
i32 FS_CursorLine;
char FS_ParentEntry[8] = "D######";
i32 FS_SortMode = 3;
char *FS_CurrentCursorPos = FS_FileList;
char *FS_CurrentPos = FS_FileList;
char *FS_FileListEnd = FS_FileList;
char FS_Title[64] = "Title";
char FS_Filter[256] = "*.nup | *.hgp   ";
char FS_FilterOut[256] = "_PC. | _360. | _PS3.";
f32 FS_X = 50.0f;
f32 FS_Y = 40.0f;
f32 FS_Width = 248.0f;
f32 FS_W;
f32 FS_H;
u8 FS_Active;
u8 FS_RefreshDir;
u8 FS_ShowVolumes;
void (*FS_Callback)(char *, char *);

static inline u32 FS_EncodedDateKey(const char *name) {
    u32 key = static_cast<u8>(name[6]) - 'A';
    key = key * 13 + static_cast<u8>(name[5]) - 'A';
    key = key * 33 + static_cast<u8>(name[4]) - 'A';
    key = key + ((key + (key << 1)) << 3) + static_cast<u8>(name[3]) - 'A';
    key *= 61;
    key += static_cast<u8>(name[2]);
    key -= 'A';
    key *= 61;
    return key + static_cast<u8>(name[1]) - 'A';
}

char *SortStrings[4] = {const_cast<char *>("A to Z"), const_cast<char *>("Z to A"), const_cast<char *>("Old to New"),
                        const_cast<char *>("New To Old")};

static __attribute__((noinline, noclone)) void RenderFileSel3Body(i32 draw_title)
    __asm__("_Z14RenderFileSel3i.part.38");

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

extern "C" {
void SaveSystemInitialiseEx(i32 slots, void *makeSaveHash, void *save, i32 saveSize, void *header, i32 headerSize,
                                i32 autosave, void (*drawSaveIcon)(void));

void SetSaveSuccessFn(void (*callback)(void));
} // extern "C"

struct starfighter_s;
struct rtl_s;
struct rtlidata_s;

extern f32 HUB_EPISODETITLEY;

struct GAMEMESSAGE_s;
struct HINT_s;
struct MENU_s;
struct NuBloomParameters;
struct STATUSPACKET_s;
struct TouchHolder;
struct _vum_s;
struct _vuv_s;
struct nuhspecial_s;
struct ripple_node_s;
struct ripple_set_s;
struct VuVec;

extern f32 MENUBOTY;

extern i16 tNONEWBESTTIME, tNEWBESTTIME;

extern i16 tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);

extern i16 id_ANAKINJEDISCARRED, id_WOOKIEE, id_TWOFACE;

extern "C" {
    extern nurenderscene_s currentScene;
}

extern "C" {
    static __used__ void cbMMRegSel(eduimenu_s *, eduiitem_s *selected, u32) {
        ed_curr = static_cast<ed_module_s *>(selected->data_ptr);
        if (__builtin_expect(!ed_curr->reserved, 1)) {
            ed_module_active = 1;
            if (ed_curr->activate)
                ed_curr->activate();
        }
    }

    __attribute__((optimize("O2", "tree-vectorize"))) static __used__ i32 eduicbInteractSel(edui_interact_s *interact) {
        eduimenu_s *menu = interact->menu;
        edui_sel_s *item = static_cast<edui_sel_s *>(interact->item);
        if (interact->callback) {
            if (edui_cursor_buttons_db & EDUI_CURSOR_PRIMARY)
                item->selected(menu, item, edui_cursor_buttons);
            else if ((edui_cursor_buttons & EDUI_CURSOR_PRIMARY) && item->held)
                item->held(menu, item, edui_cursor_buttons);
        }
        return 0;
    }

    static __used__ i32 eduicbProcessTexturePick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad) {
        edui_texture_pick_s *pick = static_cast<edui_texture_pick_s *>(item);
        i32 stick_x = static_cast<i32>(pad->analog_left_x) - 128;
        i32 stick_y = static_cast<i32>(pad->analog_left_y) - 128;
        if (stick_x > 0) {
            if (stick_x < 32)
                stick_x = 0;
            else
                stick_x = (stick_x - 32) * 255 / 223;
        } else if (stick_x > -32)
            stick_x = 0;
        else
            stick_x = (stick_x + 32) * 255 / 223;
        if (stick_y > 0) {
            if (stick_y < 32)
                stick_y = 0;
            else
                stick_y = (stick_y - 32) * 255 / 223;
        } else if (stick_y > -32)
            stick_y = 0;
        else
            stick_y = (stick_y + 32) * 255 / 223;
        f32 x = static_cast<f32>(stick_x);
        f32 y = static_cast<f32>(stick_y);
        i32 corner = pick->selected_corner;
        x = pick->uv_x[corner] + x / (8192.0f * pick->zoom);
        y = pick->uv_y[corner] + y / (8192.0f * pick->zoom);
        x = MIN(1.0f, (MAX(0.0f, x)));
        y = MIN(1.0f, (MAX(0.0f, y)));
        pick->uv_x[corner] = x;
        pick->uv_y[corner] = y;
        i32 left = pad->analog_l1 ? pad->analog_l1 : pad->analog_l2;
        i32 right = pad->analog_r1 ? pad->analog_r1 : pad->analog_r2;
        if (__builtin_expect(!!(left), 0)) {
            f32 zoom = pick->zoom - left / 4192.0f;
            if (!(zoom < 1.0f)) {
                pick->zoom = zoom;
            } else {
                pick->zoom = 1.0f;
            }
            return 1;
        }
        if (__builtin_expect(!!(right), 1)) {
            f32 zoom = pick->zoom + right / 4192.0f;
            pick->zoom = zoom > 16.0f ? 16.0f : zoom;
            return 1;
        }
        if (pad->digital_buttons_pressed & 0x100) {
            pick->uv_x[corner] = static_cast<i32>(pick->uv_x[corner] * 256.0f + 0.5f) * (1.0f / 256.0f);
            pick->uv_y[corner] = static_cast<i32>(pick->uv_y[corner] * 256.0f + 0.5f) * (1.0f / 256.0f);
        }
        if (pad->digital_buttons_pressed & 0x80)
            pick->selected_corner = corner != 1;
        if ((pad->digital_buttons_pressed & 0x40) && pick->changed)
            pick->changed(menu, item, pad->digital_buttons);
        return 0;
    }

    static __used__ i32 eduicbProcessSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad) {
        static i32 rep_timer = 30;
        static f32 accel = 1.0f;
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        if ((pad->digital_buttons & 0x0c) == 0x0c) {
            f32 value;
            if (slider->minimum <= 0.0f && slider->minimum + slider->range >= 0.0f)
                value = 0.0f;
            else if (slider->minimum <= 1.0f && slider->minimum + slider->range >= 1.0f)
                value = 1.0f;
            else {
                rep_timer = 30;
                return 0;
            }
            slider->value = value;
            slider->normalized_value = (value - slider->minimum) / slider->range;
            slider->change_timer = 60;
            if (slider->changed)
                slider->changed(menu, item, pad->digital_buttons);
            rep_timer = 30;
        } else if (pad->digital_buttons & 0x04) {
            if (rep_timer > 0)
                --rep_timer;
            else
                rep_timer = 0;
            if (!rep_timer || (pad->digital_buttons_pressed & 0x04)) {
                f32 step = slider->granularity * accel;
                f32 value = slider->value - step;
                if (value < slider->minimum)
                    value = slider->minimum;
                value = static_cast<i32>((value > 0.0f ? value + step * 0.5f : value - step * 0.5f) / step) * step;
                slider->value = value;
                slider->normalized_value = (value - slider->minimum) / slider->range;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
                accel += slider_acceleration;
            }
        } else if (pad->digital_buttons & 0x08) {
            if (rep_timer > 0)
                --rep_timer;
            else
                rep_timer = 0;
            if (!rep_timer || (pad->digital_buttons_pressed & 0x08)) {
                f32 step = slider->granularity * accel;
                f32 value = slider->value + step;
                if (value > slider->minimum + slider->range)
                    value = slider->minimum + slider->range;
                value = static_cast<i32>((value > 0.0f ? value + step * 0.5f : value - step * 0.5f) / step) * step;
                slider->value = value;
                slider->normalized_value = (value - slider->minimum) / slider->range;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
                accel += slider_acceleration;
            }
        } else {
            rep_timer = 30;
            accel = 1.0f;
        }
        return 0;
    }

    static __used__ i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *) {
        return 0;
    }

    static __used__ void eduicbItemDestroyExpander(eduimenu_s *menu, eduiitem_s *item) {
        edui_expander_s *expander = static_cast<edui_expander_s *>(item);
        if (expander->last_child)
            expander->last_child->next = NULL;
        while (expander->first_child) {
            eduiitem_s *next = expander->first_child->next;
            expander->first_child->destroy(menu, expander->first_child);
            expander->first_child = next;
        }
    }

    static __used__ void cbSel(eduimenu_s *, eduiitem_s *item) {
        selection_value = item->data;
    }

    static __used__ void cbGradChange(void) {
    }

    i32 cbInteractMenuTitle(edui_interact_s *interact) {
        static f32 offsetx;
        static f32 offsety;
        i32 result = (edui_cursor_buttons & EDUI_CURSOR_PRIMARY) != 0;
        eduimenu_s *menu = interact->menu;
        if (!(menu->flags & 1)) {
            if (!eduiInteractLocked) {
                offsetx = 0.0f - menu->x;
                offsety = 0.0f - menu->y;
            }
            menu->x = static_cast<i32>(0.0f - offsetx);
            menu->y = static_cast<i32>(0.0f - offsety);
        }
        return result;
    }

    i32 cbInteractMenuScrollUp(edui_interact_s *interact) {
        eduimenu_s *menu = interact->menu;
        if (menu->field_0c) {
            if (menu->field_0c->previous)
                menu->field_0c = menu->field_0c->previous;
            menu->selected = menu->field_0c;
        }
        return 0;
    }

    i32 cbInteractMenuScrollDown(edui_interact_s *interact) {
        eduimenu_s *menu = interact->menu;
        if (menu->field_10) {
            if (menu->field_10->next)
                menu->field_10 = menu->field_10->next;
            menu->selected = menu->field_10;
        }
        return 0;
    }

    static __used__ i32 cbInteractMenuCancelChild(edui_interact_s *interact) {
        eduimenu_s *menu = interact->menu;
        if (menu && menu->child && menu->child->callback)
            menu->child->callback(menu->child, menu);
        return 0;
    }

}

static __used__ void cbCopy(eduimenu_s *, eduiitem_s *, u32) {
    f32 red = *cp_r;
    f32 green = *cp_g;
    f32 blue = *cp_b;
    clipboard_r = red;
    clipboard_g = green;
    clipboard_b = blue;
    i32 red_byte = static_cast<i32>(red * 255.0f) & 0xff;
    i32 green_byte = static_cast<i32>(green * 255.0f) & 0xff;
    i32 blue_byte = static_cast<i32>(blue * 255.0f) & 0xff;
    u32 colour = 0x80000000u | red_byte;
    colour |= green_byte << 8;
    colour |= blue_byte << 16;
    cp_paste->colours[2] = colour;
}

extern "C" {
    void eduicbItemDestroyProp(eduimenu_s *menu, eduiitem_s *item) {
        edui_prop_s *prop = static_cast<edui_prop_s *>(item);
        if (prop->text) {
            NU_FREE(prop->text);
            prop->text = NULL;
        }
        if (prop->property_text) {
            NU_FREE(prop->property_text);
            prop->property_text = NULL;
        }
        NU_FREE(prop);
        menu->field_0c = menu->first;
    }

    static __used__ void eduicbItemDestroyFilter(eduimenu_s *menu, eduiitem_s *item) {
        edui_filter_s *filter = static_cast<edui_filter_s *>(item);
        while (filter->first_child) {
            eduiitem_s *next = filter->first_child->next;
            filter->first_child->destroy(menu, filter->first_child);
            filter->first_child = next;
        }
        eduicbItemDestroyProp(menu, item);
    }

    void eduicbItemDestroy(eduimenu_s *menu, eduiitem_s *item) {
        edui_interact_s *interact = eduiInteracts;
        for (i32 i = 0; i < numInteracts; ++i, ++interact) {
            if (menu == interact->menu && item == interact->item) {
                interact->menu = NULL;
                interact->item = NULL;
            }
        }
        if (item->text) {
            NU_FREE(item->text);
            item->text = NULL;
        }
        NU_FREE(item);
        menu->field_0c = menu->first;
    }

    static __used__ void eduicbItemSliderDestroy(eduimenu_s *menu, eduiitem_s *item) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        if (slider->format) {
            NU_FREE(slider->format);
            slider->format = NULL;
        }
        eduicbItemDestroy(menu, item);
    }

    static __used__ void eduicbItemFilePickDestroy(eduimenu_s *menu, eduiitem_s *item) {
        edui_file_pick_s *file_pick = static_cast<edui_file_pick_s *>(item);
        if (file_pick->format) {
            NU_FREE(file_pick->format);
            file_pick->format = NULL;
        }
        eduicbItemDestroy(menu, item);
    }

    static __used__ void eduicbItemTextPickDestroy(eduimenu_s *menu, eduiitem_s *item) {
        edui_textpicker_s *text_pick = static_cast<edui_textpicker_s *>(item);
        if (text_pick->format) {
            NU_FREE(text_pick->format);
            text_pick->format = NULL;
        }
        eduicbItemDestroy(menu, item);
    }

}

static __used__ void eduiFntPrintExGeneric(void *font, int x, int y, int alignment, char *format, ...) {
    if (edui_donotdraw) {
        return;
    }
    char text[1024] __attribute__((aligned(16)));
    NuQFntPushPrintMode(2);
    va_list arguments;
    va_start(arguments, format);
    NuVSPrintf(text, format, arguments);
    va_end(arguments);
    i32 width = static_cast<i32>(NuQFntPrintLenU(font, text));
    i32 left = x;
    switch (alignment) {
    case 32:
        left = x - width;
        break;
    case 64:
        left = x - width / 2;
        break;
    }
    NuQFntMove(font, left, y, 0.0f);
    NuQFntPrintU(font, text);
    NuQFntPopPrintMode();
}

static __used__ void eduiFntPrintClipEx(void *font, float x, float y, int alignment, float clip_x, float clip_width,
                                        char *format, ...) {
    va_list arguments;
    if (__builtin_expect(!edui_donotdraw, 1)) {
        char text[1024];
        i32 screen_x = static_cast<i32>(x * 16.0f);
        i32 screen_y = static_cast<i32>(y * 8.0f);
        i32 screen_clip_x = static_cast<i32>(clip_x * 16.0f);
        i32 screen_clip_width = static_cast<i32>(clip_width * 16.0f);
        NuQFntPushPrintMode(2);

        va_start(arguments, format);
        NuVSPrintf(text, format, arguments);
        va_end(arguments);
        i32 width = static_cast<i32>(NuQFntPrintLenU(font, text));
        if (alignment == 32)
            screen_x -= width;
        else if (alignment == 64)
            screen_x -= width / 2;
        if (screen_x < screen_clip_x)
            screen_x = screen_clip_x;
        i32 length = NuStrLen(text);
        if (width > screen_clip_width && length > 1) {
            char *end = text + length - 1;
            do {
                *end-- = '\0';
                width = static_cast<i32>(NuQFntPrintLenU(font, text));
            } while (width > screen_clip_width && end != text);
        }
        NuQFntMove(font, screen_x, screen_y, 0.0f);
        NuQFntPrintU(font, text);
        NuQFntPopPrintMode();
    }
}

extern "C" {
    static __used__ i32 eduicbProcessGraph(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad) {
        edui_graph_s *graph_item = static_cast<edui_graph_s *>(item);
        show_y_scale_help = show_y_scale_help > 1 ? show_y_scale_help - 1 : 0;
        show_x_scale_help = show_x_scale_help > 1 ? show_x_scale_help - 1 : 0;
        f32 x = eduiPickerDeadZone32(pad->analog_left_x) * (1.0f / 8192.0f);
        f32 y = -eduiPickerDeadZone32(pad->analog_left_y) * (1.0f / 8192.0f);
        x += graph_item->cursor_x;
        y += graph_item->cursor_y;
        x = MIN(1.0f, (MAX(0.0f, x)));
        y = MIN(1.0f, (MAX(0.0f, y)));
        graph_item->cursor_x = x;
        graph_item->cursor_y = y;

        u32 left = pad->analog_l2;
        u32 right = pad->analog_r2;
        if (left) {
            if (right) {
                show_x_scale_help = 60;
                graph_item->graph->x_scale = 1.0f;
                return 1;
            }
            if (left > 55) {
                show_x_scale_help = 60;
                f32 scale = graph_item->graph->x_scale - left * 0.02f / 255.0f;
                graph_item->graph->x_scale = scale < 0.01f ? 0.01f : scale;
                return 1;
            }
        } else if (__builtin_expect(right > 55, 1)) {
            show_x_scale_help = 60;
            graph_item->graph->x_scale = right * 0.02f / 255.0f + graph_item->graph->x_scale;
            return 1;
        }
        left = pad->analog_l1;
        right = pad->analog_r1;
        if (left) {
            if (__builtin_expect(!!(right), 0)) {
                show_y_scale_help = 60;
                graph_item->graph->y_scale = 1.0f;
                return 1;
            }
            if (__builtin_expect(!!(left), 0)) {
                show_y_scale_help = 60;
                f32 scale = graph_item->graph->y_scale - left * 0.02f / 255.0f;
                graph_item->graph->y_scale = scale < 0.01f ? 0.01f : scale;
                return 1;
            }
        } else if (right) {
            show_y_scale_help = 60;
            graph_item->graph->y_scale = right * 0.02f / 255.0f + graph_item->graph->y_scale;
            return 1;
        }
        u32 pressed = pad->digital_buttons_pressed;
        if ((pressed & 0x80) && graph_item->selected_point >= 0) {
            nugraphDeletePoint(graph_item->graph, graph_item->selected_point);
            nugraphCalcCurve(graph_item->graph, 100);
            return 1;
        }
        if (pressed & 0x20) {
            nugraphAddPoint(graph_item->graph, graph_item->cursor_x, graph_item->cursor_y);
            nugraphCalcCurve(graph_item->graph, 100);
            return 1;
        }
        if (__builtin_expect((pressed & 0x40) && graph_item->selected_point >= 0, 0)) {
            graph_item->cursor_x = graph_item->graph->x[graph_item->selected_point];
            graph_item->cursor_y = graph_item->graph->y[graph_item->selected_point];
        }
        if ((pad->digital_buttons & 0x40) && graph_item->selected_point >= 0) {
            i32 index = graph_item->selected_point;
            nugraph_s *graph = graph_item->graph;
            graph->x[index] = graph_item->cursor_x;
            graph->y[index] = graph_item->cursor_y;
            if (index == 0) {
                graph->x[index] = 0.0f;
            } else if (index == graph->point_count - 1) {
                graph->x[index] = 1.0f;
            } else if (graph->x[index] <= graph->x[index - 1]) {
                graph->x[index] = graph->x[index - 1] + 0.01f;
            } else if (graph->x[index] >= graph->x[index + 1]) {
                graph->x[index] = graph->x[index + 1] - 0.01f;
            }
            nugraphCalcCurve(graph, 100);
            return 1;
        }
        if ((pressed & 0x10) && graph_item->changed)
            graph_item->changed(menu, item, pressed);
        return 0;
    }

    static __used__ i32 eduicbProcessGreyPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad) {
        edui_colour_pick_s *pick = static_cast<edui_colour_pick_s *>(item);
        i32 left = pad->analog_l1 ? pad->analog_l1 : pad->analog_l2;
        i32 right = pad->analog_r1 ? pad->analog_r1 : pad->analog_r2;
        if (left) {
            f32 value = pick->value - left / 4192.0f;
            pick->value = value < 0.0f ? 0.0f : value;
            return 1;
        }
        if (right) {
            f32 value = pick->value + right / 4192.0f;
            pick->value = value > 1.0f ? 1.0f : value;
            return 1;
        }
        pick->saturation = 0.0f;
        pick->hue = 0.0f;
        if ((pad->digital_buttons_pressed & 0x40) && pick->changed)
            pick->changed(menu, item, pad->digital_buttons);
        return 0;
    }

}

static void HSV2RGBBody(f32 hue, f32 saturation, f32 value, f32 *red, f32 *green, f32 *blue) {
    if (hue == 360.0f)
        hue = 0.0f;
    hue /= 60.0f;
    i32 sector = static_cast<i32>(NuFloor(hue));
    f32 fraction = hue - sector;
    f32 low = (1.0f - saturation) * value;
    f32 falling = (1.0f - fraction * saturation) * value;
    f32 rising = (1.0f - (1.0f - fraction) * saturation) * value;
    switch (sector) {
    case 0:
        *red = value;
        *green = rising;
        *blue = low;
        break;
    case 1:
        *red = falling;
        *green = value;
        *blue = low;
        break;
    case 2:
        *red = low;
        *green = value;
        *blue = rising;
        break;
    case 3:
        *red = low;
        *green = falling;
        *blue = value;
        break;
    case 4:
        *red = rising;
        *green = low;
        *blue = value;
        break;
    case 5:
        *red = value;
        *green = low;
        *blue = falling;
        break;
    }
}

static void eduiFntPrintExLeft(void *font, int x, int y, int, char *format, ...) {
    if (edui_donotdraw) {
        return;
    }
    char text[1024] __attribute__((aligned(16)));
    NuQFntPushPrintMode(2);
    va_list arguments;
    va_start(arguments, format);
    NuVSPrintf(text, format, arguments);
    va_end(arguments);
    NuQFntPrintLenU(font, text);
    NuQFntMove(font, x, y, 0.0f);
    NuQFntPrintU(font, text);
    NuQFntPopPrintMode();
}

static void eduiFntPrintExEditBuffer(void *font, float x, float y, int, char *format, ...) {
    va_list arguments;
    va_start(arguments, format);
    eduiFntPrintExLeft(font, static_cast<i32>(x * 16.0f), static_cast<i32>(y * 8.0f), 16, eduiPropTextEdit,
                       arguments);
    va_end(arguments);
}

static void eduiFntPrintExCentre(void *font, int x, int y, int, char *format, ...) {
    if (edui_donotdraw) {
        return;
    }
    char text[1024] __attribute__((aligned(16)));
    NuQFntPushPrintMode(2);
    va_list arguments;
    va_start(arguments, format);
    NuVSPrintf(text, format, arguments);
    va_end(arguments);
    i32 width = static_cast<i32>(NuQFntPrintLenU(font, text));
    x -= width / 2;
    NuQFntMove(font, x, y, 0.0f);
    NuQFntPrintU(font, text);
    NuQFntPopPrintMode();
}

static void eduiFntPrintClipEx(void *font, i32 x, i32 y, i32, i32 clip_x, i32 clip_width, char *format, ...) {
    va_list arguments;
    if (!edui_donotdraw) {
        char text[1024] __attribute__((aligned(16)));
        NuQFntPushPrintMode(2);

        va_start(arguments, format);
        NuVSPrintf(text, format, arguments);
        va_end(arguments);
        i32 width = static_cast<i32>(NuQFntPrintLenU(font, text));
        if (x < clip_x)
            x = clip_x;
        i32 length = NuStrLen(text);
        if (width > clip_width) {
            char *end = text + length - 1;
            if (end > text) {
                do {
                    *end-- = '\0';
                    width = static_cast<i32>(NuQFntPrintLenU(font, text));
                } while (width > clip_width && end != text);
            }
        }
        NuQFntMove(font, x, y, 0.0f);
        NuQFntPrintU(font, text);
        NuQFntPopPrintMode();
    }
}

extern "C" {
    static __attribute__((noinline)) void eduiRenderGraphLine(nugraph_s *graph, i32 x, i32 y, i32 width, i32 height,
                                                              u32 colour) {
        i32 previous_x = 0;
        i32 previous_y = height - static_cast<i32>(height * graph->y[0]);
        f32 fraction = 0.0f;
        for (i32 sample = 0; sample < 101; ++sample) {
            i32 current_x = static_cast<i32>(width * fraction);
            i32 current_y =
                height -
                static_cast<i32>(height * nugraphGetYatX(graph, fraction * graph->x_extent * graph->x_scale, 2));
            eduiDrawLine((x + previous_x) << 4, (y + previous_y) << 3, (x + current_x) << 4, (y + current_y) << 3,
                         colour, uimtls[0]);
            previous_x = current_x;
            previous_y = current_y;
            fraction += 0.01f;
        }
    }

    static void eduiDrawHueValueBand(i32 x, i32 y, i32 width, i32 height, f32 start, f32 end, u32 first, u32 second) {
        i32 left = (x << 4) + static_cast<i32>(start * static_cast<f32>(width << 4));
        i32 band_width = static_cast<i32>(end * static_cast<f32>(width << 4)) -
                         static_cast<i32>(start * static_cast<f32>(width << 4));
        i32 strip_height = (height << 3) / 8;
        i32 first_red = first & 0xff;
        i32 first_green = (first >> 8) & 0xff;
        i32 first_blue = (first >> 16) & 0xff;
        i32 first_alpha = static_cast<i32>(first >> 24) - 0x80;
        i32 second_red = second & 0xff;
        i32 second_green = (second >> 8) & 0xff;
        i32 second_blue = (second >> 16) & 0xff;
        i32 second_alpha = static_cast<i32>(second >> 24) - 0x80;
        struct {
            i32 top_left;
            i32 top_right;
            u8 bottom_left[4];
            u8 bottom_right[4];
        } colours;
        colours.top_left = static_cast<i32>(0x80000000u);
        colours.top_right = static_cast<i32>(0x80000000u);
#define EDUI_DRAW_VALUE_STRIP(strip)                                                                                   \
    {                                                                                                                  \
        u8 *bottom_left = colours.bottom_left;                                                                         \
        u8 *bottom_right = colours.bottom_right;                                                                       \
        i32 first_red_step = first_red * (strip);                                                                      \
        i32 first_green_step = first_green * (strip);                                                                  \
        i32 first_blue_step = first_blue * (strip);                                                                    \
        i32 first_alpha_step = first_alpha * (strip);                                                                  \
        i32 second_red_step = second_red * (strip);                                                                    \
        i32 second_green_step = second_green * (strip);                                                                \
        i32 second_blue_step = second_blue * (strip);                                                                  \
        i32 second_alpha_step = second_alpha * (strip);                                                                \
        bottom_left[0] = first_red_step / edui_grad_steps;                                                                    \
        bottom_left[1] = first_green_step / edui_grad_steps;                                                                  \
        bottom_left[2] = first_blue_step / edui_grad_steps;                                                                   \
        bottom_left[3] = 0x80 + first_alpha_step / edui_grad_steps;                                                           \
        bottom_right[0] = second_red_step / edui_grad_steps;                                                                  \
        bottom_right[1] = second_green_step / edui_grad_steps;                                                                \
        bottom_right[2] = second_blue_step / edui_grad_steps;                                                                 \
        bottom_right[3] = 0x80 + second_alpha_step / edui_grad_steps;                                                         \
        eduiDrawGradRect(left, (y << 3) + (strip) * strip_height, band_width, strip_height,                            \
                         reinterpret_cast<i32 *>(&colours), uimtls[ui_bgmtl]);                                         \
        colours.top_left = *reinterpret_cast<i32 *>(colours.bottom_left);                                              \
        colours.top_right = *reinterpret_cast<i32 *>(colours.bottom_right);                                            \
    }
        EDUI_DRAW_VALUE_STRIP(0);
        EDUI_DRAW_VALUE_STRIP(1);
        EDUI_DRAW_VALUE_STRIP(2);
        EDUI_DRAW_VALUE_STRIP(3);
        EDUI_DRAW_VALUE_STRIP(4);
        EDUI_DRAW_VALUE_STRIP(5);
        EDUI_DRAW_VALUE_STRIP(6);
        EDUI_DRAW_VALUE_STRIP(7);
#undef EDUI_DRAW_VALUE_STRIP
    }

    static __used__ i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        NuQFntHeight(edui_font);
        NuQFntBaseline(edui_font);
        numtl_s *background_material = uimtls[ui_bgmtl];
        item->y = y;
        item->x = x;
        eduiDrawRect(x << 4, y << 3, width << 4, 64, item->colours[2 + item->highlighted], background_material);
        eduiDrawLine((x + 4) << 4, (y << 3) + 32, ((x + width) << 4) - 64, (y << 3) + 32, 0xff000000, uimtls[ui_bgmtl]);
        return 8;
    }

    static __used__ i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        edui_gradient_pick_s *gradient = static_cast<edui_gradient_pick_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 2;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        eduiDrawRect(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted], uimtls[ui_bgmtl]);
        eduiDrawFontSet(edui_font);
        eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        if (gradient->change_timer && gradient->selected_stage) {
            eduiFntPrintEx(edui_font, (width + x * 2) << 3, (y << 3) + baseline, 64, "%1.03f",
                           gradient->selected_stage->time);
            --gradient->change_timer;
        } else
            eduiFntPrintEx(edui_font, (width + x * 2) << 3, (y << 3) + baseline, 64, item->text);

        i32 half_height = height >> 1;
        edui_gradient_node_s *stage = gradient->first_stage;
        if (!stage)
            return height;
        f32 stage_time = stage->time;
        u32 stage_colour = stage->colour;
        edui_gradient_node_s *next = stage->next;
        while (next) {
            i32 colours[4] = {static_cast<i32>(stage_colour), static_cast<i32>(next->colour),
                              static_cast<i32>(stage_colour), static_cast<i32>(next->colour)};
            i32 left = static_cast<i32>(stage_time * (width << 4));
            i32 right = static_cast<i32>(next->time * (width << 4));
            eduiDrawGradRectLikely((x << 4) + left, (y + half_height) << 3, right - left, half_height << 3, colours,
                             uimtls[ui_bgmtl]);
            stage_time = next->time;
            stage_colour = next->colour;
            next = next->next;
        }
        for (edui_gradient_node_s *stage = gradient->first_stage; stage; stage = stage->next) {
            i32 marker_x = (x - 2 + static_cast<i32>(stage->time * width)) << 4;
            i32 marker_y = ((y + half_height) << 3) - 16;
            i32 marker_height = (half_height << 3) + 16;
            eduiDrawRect(marker_x, marker_y, 80, marker_height, stage->colour, uimtls[0]);
            u32 outline = gradient->selected_stage == stage ? 0x80ffffff : 0x80000000;
            eduiDrawLineRect(marker_x, marker_y, 80, marker_height, outline, uimtls[ui_outmtl]);
        }
        return height;
    }

    static __used__ i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 0.15625f);
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        eduiDrawRect(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted], uimtls[ui_bgmtl]);
        eduiDrawFontSetLikely(edui_font);
        eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        if (item->text_alignment == 16)
            eduiFntPrintExGeneric(edui_font, x << 4, (y << 3) + baseline, 16, item->text);
        else if (item->text_alignment == 32)
            eduiFntPrintExGeneric(edui_font, (x + width) << 4, (y << 3) + baseline, 32, item->text);
        else
            eduiFntPrintExGeneric(edui_font, (x * 2 + width) << 3, (y << 3) + baseline, item->text_alignment, item->text);
        return height;
    }

    static __used__ i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        eduiDrawRect(x << 4, y << 3, width << 4, height << 4, item->colours[2 + item->highlighted], uimtls[0]);
        eduiDrawFontSet(edui_font);
        eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        char format[512];
        NuStrCpy(format, item->text);
        NuStrCat(format, slider->format);
        eduiFntPrintEx(edui_font, (x * 2 + width) << 3, (y << 3) + baseline, 64, format,
                       static_cast<i32>(slider->value));
        i32 bottom = (y + height + height - 1) << 3;
        i32 top = (y + height) << 3;
        f32 span = width;
        i32 marker_x = static_cast<i32>(x + 1 + slider->normalized_value * span) << 4;
        eduiDrawLine(marker_x, top, marker_x, bottom, item->colours[item->highlighted], uimtls[0]);
        marker_x = static_cast<i32>(x + slider->normalized_value * span) << 4;
        eduiDrawLine(marker_x, top, marker_x, bottom, item->colours[item->highlighted], uimtls[0]);
        marker_x = static_cast<i32>(x - 1 + slider->normalized_value * span) << 4;
        eduiDrawLine(marker_x, top, marker_x, bottom, item->colours[item->highlighted], uimtls[0]);
        return height * 2;
    }

    __attribute__((optimize("O2", "tree-vectorize"))) static __used__ i32 eduicbRenderTexturePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        u32 colours[2] = {0x807f7f7f, 0x807f7f7f};
        (void)menu;
        edui_texture_pick_s *picker = static_cast<edui_texture_pick_s *>(item);
        item->x = x;
        item->y = y;
        f32 size = static_cast<f32>(width);
        f32 uv_span = 1.0f / picker->zoom;
        f32 origin_u = picker->uv_x[picker->selected_corner] - 0.5f / picker->zoom;
        f32 origin_v = picker->uv_y[picker->selected_corner] - 0.5f / picker->zoom;
        f32 maximum = 1.0f - uv_span;
        if (origin_u < 0.0f)
            origin_u = maximum < 0.0f ? maximum : 0.0f;
        else if (maximum < origin_u)
            origin_u = maximum;
        if (origin_v < 0.0f)
            origin_v = maximum < 0.0f ? maximum : 0.0f;
        else if (maximum < origin_v)
            origin_v = maximum;
        f32 point_u[2] = {
            (picker->uv_x[0] - origin_u) / uv_span,
            (picker->uv_x[1] - origin_u) / uv_span,
        };
        f32 point_v[2] = {
            (picker->uv_y[0] - origin_v) / uv_span,
            (picker->uv_y[1] - origin_v) / uv_span,
        };
        for (i32 corner = 0; corner < 2; ++corner) {
            if (point_u[corner] < 0.0f)
                point_u[corner] = 0.0f;
            else if (point_u[corner] > 1.0f)
                point_u[corner] = 1.0f;
            if (point_v[corner] < 0.0f)
                point_v[corner] = 0.0f;
            else if (point_v[corner] > 1.0f)
                point_v[corner] = 1.0f;
        }
        colours[picker->selected_corner] = 0x80ffffff;
        if (!edui_donotdraw) {
            NuRndrRectUV2di(x << 4, y << 3, width << 4, width << 3, origin_u, origin_v, origin_u + uv_span,
                            origin_v + uv_span, 0x80808080, picker->material);
            for (i32 corner = 0; corner < 2; ++corner) {
                i32 vertical = static_cast<i32>(x + size * point_u[corner]) << 4;
                i32 horizontal = static_cast<i32>(y + size * point_v[corner]) << 3;
                NuRndrLine2di(x << 4, horizontal, (x + width - 1) << 4, horizontal, colours[corner], uimtls[0]);
                NuRndrLine2di(vertical, y << 3, vertical, (y + width) * 8 - 8, colours[corner], uimtls[0]);
            }
        }
        i32 label_y = y + width;
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        if (!edui_donotdraw) {
            NuRndrRect2di(x << 4, label_y << 3, width << 4, height << 3, item->colours[2 + item->highlighted],
                          uimtls[ui_bgmtl]);
            NuQFntSet(edui_font);
            NuQFntSetColour(edui_font, item->colours[item->highlighted]);
        }
        eduiFntPrintEx(edui_font, (x * 2 + width) << 3, (label_y << 3) + baseline, 64, "%1.03f,%1.03f",
                       picker->uv_x[picker->selected_corner] * 256.0f, picker->uv_y[picker->selected_corner] * 256.0f);
        return width + height;
    }

    static __used__ i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        edui_text_selector_s *selector = static_cast<edui_text_selector_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        eduiDrawRect(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted], uimtls[ui_bgmtl]);
        eduiDrawFontSet(edui_font);
        eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        char format[512];
        NuStrCpy(format, item->text);
        NuStrCat(format, ": %s");
        eduiFntPrintEx(edui_font, x << 4, (y << 3) + baseline, 16, format,
                       selector->options[static_cast<i32>(selector->value)]);
        return height;
    }

    static __used__ i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        edui_slider_s *number = static_cast<edui_slider_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->y = y;
        item->x = x;
        eduiDrawRect(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted], uimtls[ui_bgmtl]);
        eduiDrawFontSet(edui_font);
        eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        char format[512];
        NuStrCpy(format, item->text);
        NuStrCat(format, number->format);
        eduiFntPrintEx(edui_font, x << 4, (y << 3) + baseline, 16, format, number->value);
        return height;
    }

    __attribute__((optimize("O2", "tree-vectorize"))) static __used__ i32 eduicbRenderGraph(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32 selected) {
        (void)menu;
        edui_graph_s *picker = static_cast<edui_graph_s *>(item);
        f32 cursor_y = picker->cursor_y;
        f32 cursor_x = picker->cursor_x;
        item->x = x;
        item->y = y;
        i32 total_width = picker->width;
        i32 graph_width = total_width - 60;
        i32 total_height = picker->height;
        i32 graph_height = total_height - 60;
        i32 plot_x = x + 30;
        i32 plot_y = y + 30;
        i32 plot_bottom = plot_y + graph_height;
        eduiDrawRect(x << 4, y << 3, total_width << 4, total_height << 3, 0x80000000, uimtls[0]);
        eduiDrawRect(plot_x << 4, plot_y << 3, graph_width << 4, graph_height << 3, 0x80100000, uimtls[0]);
        if (!picker->graph) {
            eduiDrawFontSet(edui_font);
            eduiSetFontColour(edui_font, 0x80000080);
            eduiFntPrintEx(edui_font, (x + total_width) << 3, (y + total_height) << 2, 64, "Invalid Graph Pointer");
            return total_height;
        }
        eduiDrawFontSetLikely(edui_font);
        eduiSetFontColour(edui_font, 0x80808080);
        if (picker->x_scale > 0.0f) {
            f32 tick_count = picker->graph->x_extent * picker->graph->x_scale / picker->x_scale;
            i32 label_interval = static_cast<i32>(tick_count / 10.0f) + 1;
            for (i32 tick = 1; tick <= static_cast<i32>(tick_count); ++tick) {
                f32 tick_offset = static_cast<f32>(graph_width * tick) / tick_count;
                u32 colour = 0x80260000;
                if (tick % label_interval == 0) {
                    eduiDrawFontSet(edui_font);
                    eduiFntPrintEx(edui_font, static_cast<i32>(plot_x + tick_offset) << 4,
                                   (plot_bottom << 3) - 48 + static_cast<i32>(NuQFntHeight(edui_font)), 64, "%1.01f",
                                   static_cast<f32>(tick) * picker->x_scale);
                    colour = 0x80340000;
                }
                i32 line_x = static_cast<i32>(plot_x + tick_offset) << 4;
                eduiDrawLine(line_x, plot_y << 3, line_x, (plot_bottom << 3) - 8, colour, uimtls[0]);
            }
        }
        if (picker->y_scale > 0.0f) {
            f32 tick_count = picker->graph->y_extent * picker->graph->y_scale / picker->y_scale;
            i32 label_interval = static_cast<i32>(tick_count / 10.0f) + 1;
            for (i32 tick = 1; tick <= static_cast<i32>(tick_count); ++tick) {
                f32 tick_offset = static_cast<f32>(graph_height * tick) / tick_count;
                u32 colour = 0x80260000;
                if (tick % label_interval == 0) {
                    eduiDrawFontSet(edui_font);
                    eduiFntPrintEx(edui_font, (x + 28) << 4,
                                   (static_cast<i32>(plot_bottom - tick_offset) << 3) +
                                       (static_cast<i32>(NuQFntHeight(edui_font)) >> 1),
                                   32, "%1.01f", static_cast<f32>(tick) * picker->y_scale);
                    colour = 0x80340000;
                }
                i32 line_y = static_cast<i32>(plot_bottom - tick_offset) << 3;
                eduiDrawLine(plot_x << 4, line_y, (plot_x + graph_width - 1) << 4, line_y, colour, uimtls[0]);
            }
        }
        eduiDrawFontSet(edui_font);
        eduiFntPrintEx(edui_font, (plot_x + graph_width) << 4, (y << 3) + static_cast<i32>(NuQFntHeight(edui_font)), 32,
                       picker->x_label);
        eduiFntPrintEx(edui_font, (x + total_width - 3) << 4, (y + total_height) << 3, 32, picker->y_label);
        eduiFntPrintEx(edui_font, plot_x << 4, (y << 3) + static_cast<i32>(NuQFntHeight(edui_font)), 16, picker->title);
        eduiFntPrintEx(edui_font, (x + 28) << 4, (plot_bottom << 3) - 48 + static_cast<i32>(NuQFntHeight(edui_font)),
                       32, "0.0");
        u32 cursor_colour = selected ? 0x80707070 : 0x80303030;
        i32 cursor_line_y = static_cast<i32>(plot_y + (1.0f - cursor_y) * graph_height) << 3;
        eduiDrawLine(plot_x << 4, cursor_line_y, (plot_x + graph_width - 1) << 4, cursor_line_y, cursor_colour,
                     uimtls[0]);
        i32 cursor_line_x = static_cast<i32>(plot_x + cursor_x * graph_width) << 4;
        eduiDrawLine(cursor_line_x, plot_y << 3, cursor_line_x, (plot_bottom << 3) - 8, cursor_colour, uimtls[0]);
        picker->selected_point = -1;
        f32 closest = 1.0f;
        for (i32 point = 0; point < picker->graph->point_count; ++point) {
            i32 px = plot_x + static_cast<i32>(picker->graph->x[point] * graph_width);
            i32 py = plot_bottom - static_cast<i32>(picker->graph->y[point] * graph_height);
            eduiDrawLine((px - 10) << 4, py << 3, (px + 10) << 4, py << 3, 0x80505050, uimtls[0]);
            eduiDrawLine(px << 4, (py << 3) - 80, px << 4, (py << 3) + 80, 0x80505050, uimtls[0]);
            f32 distance =
                fabsf(picker->cursor_x - picker->graph->x[point]) + fabsf(picker->cursor_y - picker->graph->y[point]);
            if (distance < closest && distance < 0.05f) {
                picker->selected_point = point;
                closest = distance;
            }
        }
        if (picker->onion_skins[0])
            eduiRenderGraphLine(picker->onion_skins[0], plot_x, plot_y, graph_width, graph_height, 0x80402000);
        if (picker->onion_skins[1])
            eduiRenderGraphLine(picker->onion_skins[1], plot_x, plot_y, graph_width, graph_height, 0x80402000);
        if (picker->onion_skins[2])
            eduiRenderGraphLine(picker->onion_skins[2], plot_x, plot_y, graph_width, graph_height, 0x80402000);
        if (picker->onion_skins[3])
            eduiRenderGraphLine(picker->onion_skins[3], plot_x, plot_y, graph_width, graph_height, 0x80402000);
        if (picker->onion_skins[4])
            eduiRenderGraphLine(picker->onion_skins[4], plot_x, plot_y, graph_width, graph_height, 0x80402000);
        if (picker->onion_skins[5])
            eduiRenderGraphLine(picker->onion_skins[5], plot_x, plot_y, graph_width, graph_height, 0x80402000);
        if (picker->onion_skins[6])
            eduiRenderGraphLine(picker->onion_skins[6], plot_x, plot_y, graph_width, graph_height, 0x80402000);
        if (picker->onion_skins[7])
            eduiRenderGraphLine(picker->onion_skins[7], plot_x, plot_y, graph_width, graph_height, 0x80402000);
        eduiRenderGraphLine(picker->graph, plot_x, plot_y, graph_width, graph_height, 0x80800000);
        if (picker->selected_point >= 0 && !edui_donotdraw) {
            i32 point = picker->selected_point;
            NuRndrLineRect2di((plot_x + static_cast<i32>(picker->graph->x[point] * graph_width) - 6) << 4,
                              ((plot_bottom - static_cast<i32>(picker->graph->y[point] * graph_height)) << 3) - 48, 192,
                              96, 0x80ffffff, uimtls[ui_outmtl]);
        }
        if (show_x_scale_help || show_y_scale_help) {
            eduiDrawFontSet(edui_font);
            i32 help_x = (plot_x + (graph_width >> 1)) << 4;
            i32 help_y = plot_y << 3;
            help_y += static_cast<i32>(NuQFntHeight(edui_font));
            eduiSetFontColour(edui_font, 0x80808080);
            eduiFntPrintEx(edui_font, help_x, help_y, 64, "Scaling %c Axis", show_x_scale_help ? 'X' : 'Y');
            help_y += static_cast<i32>(NuQFntHeight(edui_font));
            eduiFntPrintEx(edui_font, help_x, help_y, 64, "%1.03f",
                           show_x_scale_help ? picker->graph->x_scale : picker->graph->y_scale);
            help_y += static_cast<i32>(NuQFntHeight(edui_font));
        }
        i32 label_height = static_cast<i32>(NuQFntHeight(edui_font) * 1.125f) >> 3;
        i32 label_y = (y + total_height) << 3;
        f32 label_font_height = NuQFntHeight(edui_font);
        i32 label_baseline = static_cast<i32>(NuQFntBaseline(edui_font)) +
                             (((label_height << 3) - static_cast<i32>(label_font_height)) >> 1);
        eduiDrawRect(x << 4, label_y, total_width << 4, label_height << 3, item->colours[2 + item->highlighted],
                     uimtls[ui_bgmtl]);
        eduiDrawFontSet(edui_font);
        eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        if (picker->selected_point >= 0) {
            i32 point = picker->selected_point;
            eduiFntPrintEx(edui_font, (x + 2) << 4, label_y + label_baseline, 16, "Point %d = %1.02f, %1.02f", point,
                           picker->graph->x[point] * picker->graph->x_scale * picker->graph->x_extent,
                           picker->graph->y[point] * picker->graph->y_scale * picker->graph->y_extent);
        }
        eduiFntPrintEx(edui_font, (x + total_width - 4) << 4, label_y + label_baseline, 32,
                       "Cursor pos = %1.02f, %1.02f",
                       picker->cursor_x * picker->graph->x_scale * picker->graph->x_extent,
                       picker->cursor_y * picker->graph->y_scale * picker->graph->y_extent);
        return total_height + label_height;
    }

    static __used__ i32 eduicbRenderFilePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        (void)menu;
        edui_file_pick_s *picker = static_cast<edui_file_pick_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        if (picker->reserved_54) {
            eduiDrawRect(x << 4, y << 3, width << 4, height << 4, item->colours[2 + item->highlighted],
                         uimtls[ui_bgmtl]);
            eduiDrawFontSet(edui_font);
            eduiSetFontColour(edui_font, item->colours[item->highlighted]);
            i32 centre = (x * 2 + width) << 3;
            eduiFntPrintEx(edui_font, centre, (y << 3) + baseline, 64, item->text);
            eduiFntPrintEx(edui_font, centre, ((y + height) << 3) + baseline, 64, picker->format, picker->name);
            return height * 2;
        }
        eduiDrawRect(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted], uimtls[ui_bgmtl]);
        char formatted[264];
        sprintf(formatted, picker->format, picker->name);
        eduiDrawFontSet(edui_font);
        eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        eduiFntPrintEx(edui_font, (x * 2 + width) << 3, (y << 3) + baseline, 64, "%s %s", item->text, formatted);
        return height;
    }

    __attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32) {
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->y = y;
        item->x = x;
        eduiDrawRect(x << 4, y << 3, width << 4, height << 3, item->colours[2], uimtls[ui_bgmtl]);
        eduiDrawFontSetLikely(edui_font);
        eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        eduiFntPrintEx(edui_font, x << 4, (y << 3) + baseline, 16, item->text);

        i32 top = (y << 3) + 16;
        i32 left = ((x - height + width - 2) << 4) + 16;
        i32 right = left + ((height - 2) << 4) - 16;
        i32 bottom = top + ((height - 2) << 3) - 8;
        if (item->highlighted) {
            i32 middle_y = (top + bottom) >> 1;
            i32 quarter_x = (left + ((left + right) >> 1)) >> 1;
            i32 lower_y = (bottom + middle_y) >> 1;
            i32 points[8] = {left, middle_y, quarter_x, lower_y, quarter_x, bottom, right, top};
            eduiDrawTriStrip(points, NULL, 4, item->colours[0], uimtls[0]);
        } else {
            i32 middle_y = (top + bottom) >> 1;
            i32 quarter_x = (left + ((left + right) >> 1)) >> 1;
            i32 lower_y = (bottom + middle_y) >> 1;
            i32 points[10] = {left, middle_y, quarter_x, lower_y, right, top, quarter_x, bottom, left, middle_y};
            eduiDrawLineStrip(points, NULL, 5, item->colours[0], uimtls[0]);
        }
        return height;
    }

    static __used__ i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        edui_colour_pick_s *pick = static_cast<edui_colour_pick_s *>(item);
        i32 colours[4] = {static_cast<i32>(0x80000000), static_cast<i32>(0x80ffffff), static_cast<i32>(0x80000000),
                          static_cast<i32>(0x80ffffff)};
        i32 height = width / 4;
        item->x = x;
        item->y = y;
        {
            i32 left = static_cast<i32>(0.0f * (width << 4));
            i32 right = static_cast<i32>(1.0f * (width << 4));
            eduiDrawGradRect((x << 4) + left, y << 3, right - left, height << 3, colours, uimtls[ui_bgmtl]);
        }
        i32 cursor_x = static_cast<i32>(x + 1 + (width - 2) * pick->value) << 4;
        eduiDrawLine(cursor_x, y << 3, cursor_x, (y + (height >> 1) - 1) << 3, 0x80ffffff, uimtls[0]);
        cursor_x = static_cast<i32>(x + (width - 2) * pick->value) << 4;
        eduiDrawLine(cursor_x, y << 3, cursor_x, (y + (height >> 1) - 1) << 3, 0x80ffffff, uimtls[0]);
        cursor_x = static_cast<i32>(x - 1 + (width - 2) * pick->value) << 4;
        eduiDrawLine(cursor_x, y << 3, cursor_x, (y + (height >> 1) - 1) << 3, 0x80ffffff, uimtls[0]);
        cursor_x = static_cast<i32>(x + 1 + (width - 2) * pick->value) << 4;
        eduiDrawLine(cursor_x, (y + (height >> 1)) << 3, cursor_x, (y + height - 1) << 3, 0x80000000, uimtls[0]);
        cursor_x = static_cast<i32>(x + (width - 2) * pick->value) << 4;
        eduiDrawLine(cursor_x, (y + (height >> 1)) << 3, cursor_x, (y + height - 1) << 3, 0x80000000, uimtls[0]);
        cursor_x = static_cast<i32>(x - 1 + (width - 2) * pick->value) << 4;
        eduiDrawLine(cursor_x, (y + (height >> 1)) << 3, cursor_x, (y + height - 1) << 3, 0x80000000, uimtls[0]);
        return height;
    }

    static __used__ i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        edui_colour_slider_s *slider = static_cast<edui_colour_slider_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        eduiDrawRect(x << 4, y << 3, width << 4, height << 4, item->colours[2 + item->highlighted], uimtls[0]);
        eduiDrawFontSet(edui_font);
        eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        eduiFntPrintEx(edui_font, (x + 64) << 4, (y << 3) + baseline, 64, item->text);
        eduiFntPrintEx(edui_font, (x + 116) << 4, (y << 3) + baseline, 64, slider->format,
                       static_cast<i32>(slider->value));

        i32 colour = slider->red + (slider->green << 8) + (slider->blue << 16);
        i32 colours[4] = {static_cast<i32>(0xff000000), colour, static_cast<i32>(0xff000000), colour};
        i32 top = (y + height) << 3;
        {
            i32 left = static_cast<i32>(0.0f * (width << 4));
            i32 right = static_cast<i32>(1.0f * (width << 4));
            eduiDrawGradRect((x << 4) + left, top, right - left, height << 3, colours, uimtls[ui_bgmtl]);
        }
        i32 bottom = (y + height + height - 1) << 3;
        {
            i32 marker = static_cast<i32>((x + 1) + slider->normalized_value * width) << 4;
            eduiDrawLine(marker, top, marker, bottom, item->colours[item->highlighted], uimtls[0]);
        }
        {
            i32 marker = static_cast<i32>(x + slider->normalized_value * width) << 4;
            eduiDrawLine(marker, top, marker, bottom, item->colours[item->highlighted], uimtls[0]);
        }
        {
            i32 marker = static_cast<i32>((x - 1) + slider->normalized_value * width) << 4;
            eduiDrawLine(marker, top, marker, bottom, item->colours[item->highlighted], uimtls[0]);
        }
        return height * 2;
    }

    static __used__ i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        auto *picker = static_cast<edui_textpicker_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->y = y;
        item->x = x;

        char letter[2] = "0";
        if (picker->editing) {
            item_width = width > height * 10 ? width : height * 10;
            i32 total_height = (picker->keyboard_flags & 2) ? height * 6 : height * 5;
            eduiDrawRect(x << 4, y << 3, item_width << 4, total_height << 3, item->colours[2], uimtls[ui_bgmtl]);
            for (i32 row = 0; row < 4; ++row) {
                auto draw_key = [&](i32 column) {
                    if (row == picker->keyboard_row && column == picker->keyboard_column) {
                        eduiDrawFontSet(edui_font);
                        eduiSetFontColour(edui_font, item->colours[1]);
                    } else {
                        eduiDrawFontSet(edui_font);
                        eduiSetFontColour(edui_font, item->colours[0]);
                    }
                    letter[0] = textrow[row][column];
                    eduiFntPrintEx(edui_font, (x + height * column) << 4, ((y + row * height) << 3) + baseline, 16,
                                   letter);
                };
                draw_key(0);
                draw_key(1);
                draw_key(2);
                draw_key(3);
                draw_key(4);
                draw_key(5);
                draw_key(6);
                draw_key(7);
                draw_key(8);
                draw_key(9);
            }
            eduiDrawFontSetLikely(edui_font);
            eduiSetFontColour(edui_font, item->colours[1]);
            i32 value_y = ((y + height * 4) << 3) + baseline;
            eduiFntPrintEx(edui_font, x << 4, value_y, 16, picker->value);
            if (--cursor_flash < 1)
                cursor_flash = 32;
            if (cursor_flash >= 16 && !edui_donotdraw) {
                char prefix[256];
                i32 position = picker->cursor;
                i32 offset = 0;
                if (position) {
                    NuStrNCpy(prefix, picker->value, position + 1);
                    offset = static_cast<i32>(NuQFntPrintLenU(edui_font, prefix));
                }
                char at_cursor[2] = {position < NuStrLen(picker->value) ? picker->value[position] : 'A', '\0'};
                NuRndrRect2di((x << 4) + offset, value_y - static_cast<i32>(NuQFntBaseline(edui_font)),
                              static_cast<i32>(NuQFntPrintLenU(edui_font, at_cursor)),
                              static_cast<i32>(NuQFntHeight(edui_font)), item->colours[1] ^ 0xffffff, NULL);
            }
            if (picker->keyboard_flags & 2) {
                eduiDrawFontSet(edui_font);
                eduiSetFontColour(edui_font, item->colours[0]);
                eduiFntPrintEx(edui_font, x << 4, ((y + height * 5) << 3) + baseline, 16,
                               TextPickCopyBuffer[0] ? TextPickCopyBuffer
                                                     : const_cast<char *>("Copy L2+R2 Paste L1+R1"));
            }
            return total_height;
        } else {
            if (!(picker->keyboard_flags & 1)) {
                eduiDrawRect(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted],
                             uimtls[ui_bgmtl]);
                char formatted[256];
                sprintf(formatted, picker->format, picker->value);
                eduiDrawFontSet(edui_font);
                eduiSetFontColour(edui_font, item->colours[item->highlighted]);
                eduiFntPrintEx(edui_font, x << 4, (y << 3) + baseline, 16, const_cast<char *>("%s %s"), item->text,
                               formatted);
                return height;
            }
            {
                eduiDrawRectLikely(x << 4, y << 3, width << 4, height << 4, item->colours[2 + item->highlighted],
                             uimtls[ui_bgmtl]);
            }
            eduiDrawFontSetLikely(edui_font);
            eduiSetFontColourLikely(edui_font, item->colours[item->highlighted]);
            i32 centre = (x * 2 + width) << 3;
            eduiFntPrintEx(edui_font, centre, (y << 3) + baseline, 64, item->text);
            eduiFntPrintEx(edui_font, centre, ((y + height) << 3) + baseline, 64, picker->format, picker->value);
            return height * 2;
        }
    }

    static __used__ i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        eduiDrawRect(x << 4, y << 3, width << 4, height << 4, item->colours[2 + item->highlighted], uimtls[ui_bgmtl]);
        eduiDrawFontSet(edui_font);
        eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        char format[512];
        NuStrCpy(format, item->text);
        NuStrCat(format, slider->format);
        eduiFntPrintEx(edui_font, (x * 2 + width) << 3, (y << 3) + baseline, 64, format, slider->value);
        if (slider->normalized_value >= 0.0f && slider->normalized_value <= 1.0f) {
            i32 top = (y + height) << 3;
            i32 bottom = (y + height + height - 1) << 3;
            f32 span = width - 2;
            i32 marker_x = static_cast<i32>(x + 1 + slider->normalized_value * span) << 4;
            eduiDrawLine(marker_x, top, marker_x, bottom, item->colours[item->highlighted], uimtls[0]);
            marker_x = static_cast<i32>(x + slider->normalized_value * span) << 4;
            eduiDrawLine(marker_x, top, marker_x, bottom, item->colours[item->highlighted], uimtls[0]);
            marker_x = static_cast<i32>(x - 1 + slider->normalized_value * span) << 4;
            eduiDrawLine(marker_x, top, marker_x, bottom, item->colours[item->highlighted], uimtls[0]);
        }
        return height * 2;
    }

    static __used__ i32 eduicbProcessExpander(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad) {
        (void)delta_time;
        edui_expander_s *expander = static_cast<edui_expander_s *>(item);
        if (pad->digital_buttons_pressed & EDUI_CURSOR_PRIMARY) {
            if (expander->changed)
                expander->changed(menu, item, 0);
            if (expander->first_child) {
                if (!expander->open) {
                    expander->last_child->next = item->next;
                    if (item->next)
                        item->next->previous = expander->last_child;
                    expander->first_child->previous = item;
                    item->next = expander->first_child;
                    expander->open = 1;
                } else {
                    eduicbItemExpanderClose(expander);
                }
            }
        }
        return 0;
    }

    static __used__ i32 eduicbInteractExpander(edui_interact_s *interact) {
        eduimenu_s *menu = interact->menu;
        edui_expander_s *expander = static_cast<edui_expander_s *>(interact->item);
        if (__builtin_expect(edui_cursor_buttons_db & EDUI_CURSOR_PRIMARY, 1)) {
            if (expander->changed)
                expander->changed(menu, expander, 0);
            if (edui_cursor_x >= expander->button_x && edui_cursor_y >= expander->button_y * 0.5f &&
                edui_cursor_x < expander->button_x + expander->button_size &&
                edui_cursor_y < (expander->button_y + expander->button_size) * 0.5f && expander->first_child) {
                if (!expander->open) {
                    expander->last_child->next = expander->next;
                    if (expander->next)
                        expander->next->previous = expander->last_child;
                    expander->first_child->previous = expander;
                    expander->next = expander->first_child;
                    expander->open = 1;
                } else {
                    eduicbItemExpanderClose(expander);
                }
            }
        }
        return 0;
    }

    static __used__ i32 eduicbInteractProp(struct edui_interact_s *interact) {
        eduimenu_s *menu = interact->menu;
        edui_prop_s *property = static_cast<edui_prop_s *>(interact->item);
        u8 &property_flags = reinterpret_cast<u8 *>(property)[0x4c];
        bool handled = false;
        if (__builtin_expect(edui_cursor_buttons & EDUI_CURSOR_PRIMARY, 0)) {
            if (!(property_flags & 0x0a)) {
                if (__builtin_expect(property_flags & 1, 0)) {
                    if (edui_cursor_x >= interact->x + property->label_width + 1.0f && edui_cursor_y >= interact->y &&
                        edui_cursor_x < property->button_x && edui_cursor_y < interact->y + interact->height) {
                        property_flags |= 1;
                        eduiPropTextPos = -1;
                        handled = true;
                    }
                } else {
                    if (__builtin_expect(edui_cursor_x >= interact->x, 0) && edui_cursor_y >= interact->y &&
                        edui_cursor_x < interact->x + property->label_width - 1.0f &&
                        edui_cursor_y < interact->y + interact->height && property->selected) {
                        property->selected(menu, property, 0);
                    }
                    if (edui_cursor_x >= interact->x + property->label_width - 1.0f &&
                        edui_cursor_y >= interact->y &&
                        edui_cursor_x < interact->x + property->label_width + 1.0f &&
                        edui_cursor_y < interact->y + interact->height) {
                        property_flags |= 0x06;
                    }
                    if (edui_cursor_x >= static_cast<edui_prop_s *>(interact->item)->label_width + interact->x + 1.0f &&
                        edui_cursor_y >= interact->y &&
                        edui_cursor_x < static_cast<edui_prop_s *>(interact->item)->button_x &&
                        edui_cursor_y < interact->y + interact->height && !(property_flags & 1)) {
                        if (property->selected)
                            property->selected(menu, property, 0);
                        NuKeyFlush();
                        property_flags |= 1;
                        NuStrCpy(eduiPropTextStore, property->property_text);
                        NuStrCpy(eduiPropTextEdit, property->property_text);
                        eduiPropTextPos = NuStrLen(property->property_text);
                    }
                    if (((edui_cursor_x >= property->button_x && edui_cursor_y >= property->button_y * 0.5f &&
                          edui_cursor_x < property->button_x + property->button_size &&
                          edui_cursor_y < (property->button_y + property->button_size) * 0.5f) ||
                         eduiInteractLocked) &&
                        property->button) {
                        property->button(menu, property, 0);
                        if (property_flags & 0x20)
                            handled = true;
                    }
                }
            }
        }
        return handled || (property_flags & 0x0b);
    }

    static __used__ i32 eduicbInteractFilter(struct edui_interact_s *interact) {
        return eduicbInteractProp(interact);
    }

    static __used__ i32 eduicbProcessColourSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad) {
        static i32 rep_timer = 30;
        static f32 dynascale;
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        if ((pad->digital_buttons & 0x0c) == 0x0c) {
            if (slider && slider->minimum <= 0.0f && slider->minimum + slider->range >= 0.0f) {
                slider->value = 0.0f;
                slider->normalized_value = (slider->value - slider->minimum) / slider->range;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
            }
            rep_timer = 30;
        } else if ((pad->digital_buttons & 0x03) == 0x03) {
            if (slider && slider->minimum <= 0.0f && slider->minimum + slider->range >= 255.0f) {
                slider->value = 255.0f;
                slider->normalized_value = (slider->value - slider->minimum) / slider->range;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
            }
            rep_timer = 30;
        } else if (pad->digital_buttons & 0x04) {
            if (rep_timer > 0)
                --rep_timer;
            else
                rep_timer = 0;
            if (!rep_timer || (pad->digital_buttons_pressed & 0x04)) {
                f32 value = slider->value - slider->granularity;
                if (value < slider->minimum)
                    value = slider->minimum;
                f32 rounded = value > 0.0f ? value + slider->granularity * 0.5f : value - slider->granularity * 0.5f;
                slider->value = static_cast<i32>(rounded / slider->granularity) * slider->granularity;
                slider->normalized_value = (slider->value - slider->minimum) / slider->range;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
            }
        } else if (pad->digital_buttons & 0x08) {
            if (rep_timer > 0)
                --rep_timer;
            else
                rep_timer = 0;
            if (!rep_timer || (pad->digital_buttons_pressed & 0x08)) {
                f32 value = slider->value + slider->granularity;
                f32 maximum = slider->minimum + slider->range;
                value = maximum < value ? maximum : value;
                f32 rounded = value > 0.0f ? value + slider->granularity * 0.5f : value - slider->granularity * 0.5f;
                slider->value = static_cast<i32>(rounded / slider->granularity) * slider->granularity;
                slider->normalized_value = (slider->value - slider->minimum) / slider->range;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
            }
        } else {
            rep_timer = 30;
            if (pad->analog_l2) {
                f32 scale;
                if (!pad->unknown_a9) {
                    dynascale = 1.627604291343232e-7f;
                    scale = 1.627604251552839e-5f;
                } else {
                    dynascale *= 1.05f;
                    scale = dynascale;
                }
                f32 value = slider->normalized_value - static_cast<u32>(pad->analog_l2) * scale;
                if (value < 0.0f)
                    value = 0.0f;
                slider->normalized_value = value;
                slider->value = value * slider->range + slider->minimum;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
            } else if (pad->analog_r2) {
                f32 scale;
                if (!pad->unknown_a9) {
                    dynascale = 1.627604291343232e-7f;
                    scale = 1.627604251552839e-5f;
                } else {
                    dynascale *= 1.05f;
                    scale = dynascale;
                }
                f32 value = slider->normalized_value + static_cast<u32>(pad->analog_r2) * scale;
                if (value > 1.0f)
                    value = 1.0f;
                slider->normalized_value = value;
                slider->value = value * slider->range + slider->minimum;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
            } else {
                dynascale = 1.627604291343232e-7f;
            }
        }
        return 0;
    }

    static __used__ i32 eduicbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad) {
        edui_colour_pick_s *pick = static_cast<edui_colour_pick_s *>(item);
        if (!edui_cursor_buttons) {
            pick->dragging_colour = 0;
            pick->dragging_saturation = 0;
        }
        f32 dx = eduiPickerDeadZone32(pad->analog_left_x) * 0.001f;
        f32 dy = eduiPickerDeadZone32(pad->analog_left_y) * 0.001f;
        pick->cursor_x += dx;
        pick->cursor_y += dy;
        pick->cursor_x = MIN(1.0f, (MAX(0.0f, pick->cursor_x)));
        pick->cursor_y = MIN(1.0f, (MAX(0.0f, pick->cursor_y)));
        if (pad->analog_l2) {
            pick->saturation = MAX(0.0f, pick->saturation - static_cast<u32>(pad->analog_l2) * delta_time * 0.005f);
            return 1;
        }
        if (pad->analog_r2) {
            pick->saturation = MIN(1.0f, pick->saturation + static_cast<u32>(pad->analog_r2) * delta_time * 0.005f);
            return 1;
        }
        pick->hue = pick->cursor_x * 360.0f;
        pick->value = pick->cursor_y;
        eduiHSVToRGB(pick->hue, pick->saturation, pick->value, pick->red, pick->green, pick->blue);
        if (((pad->digital_buttons_pressed & EDUI_CURSOR_PRIMARY) || pick->confirm) && pick->changed)
            pick->changed(menu, item, pad->digital_buttons);
        return 0;
    }

    static __used__ i32 eduicbInteractColourPick(edui_interact_s *interact) {
        edui_colour_pick_s *pick = static_cast<edui_colour_pick_s *>(interact->item);
        if (((edui_cursor_x >= interact->x && edui_cursor_y >= interact->y &&
              edui_cursor_x < interact->x + interact->width &&
              edui_cursor_y < interact->y + interact->height * 0.75f) ||
             pick->dragging_colour) &&
            !pick->dragging_saturation) {
            pick->cursor_y = (edui_cursor_y - interact->y) / (interact->height * 0.75f);
            pick->cursor_x = (edui_cursor_x - interact->x) / interact->width;
            pick->dragging_colour = (edui_cursor_buttons & EDUI_CURSOR_PRIMARY) != 0;
        }
        if (((edui_cursor_x >= interact->x && edui_cursor_y >= interact->y + interact->height * 0.75f &&
              edui_cursor_x < interact->x + interact->width &&
              edui_cursor_y < interact->y + interact->height * 0.75f + interact->height * 0.125f) ||
             pick->dragging_saturation) &&
            !pick->dragging_colour) {
            pick->saturation = (edui_cursor_x - interact->x) / interact->width;
            pick->dragging_saturation = (edui_cursor_buttons & EDUI_CURSOR_PRIMARY) != 0;
        }
        pick->cursor_x = MIN(1.0f, (MAX(0.0f, pick->cursor_x)));
        pick->cursor_y = MIN(1.0f, (MAX(0.0f, pick->cursor_y)));
        pick->saturation = MIN(1.0f, (MAX(0.0f, pick->saturation)));
        pick->hue = pick->cursor_x * 360.0f;
        pick->value = pick->cursor_y;
        eduiHSVToRGB(pick->hue, pick->saturation, pick->value, pick->red, pick->green, pick->blue);
        if (edui_cursor_x >= interact->x &&
            edui_cursor_y >= interact->y + interact->height * 0.75f + interact->height * 0.125f &&
            edui_cursor_x < interact->x + interact->width && edui_cursor_y < interact->y + interact->height &&
            (edui_cursor_buttons_db & EDUI_CURSOR_PRIMARY)) {
            pick->dragging_colour = 0;
            pick->dragging_saturation = 0;
            pick->confirm = 1;
        }
        return pick->dragging_colour || pick->dragging_saturation;
    }

    __attribute__((optimize("O2", "tree-vectorize"))) static __used__ i32 eduicbRenderColourPick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        (void)menu;
        edui_colour_pick_s *picker = static_cast<edui_colour_pick_s *>(item);
        item->y = y;
        item->x = x;
        i32 main_height = (width * 3) / 4;
        eduiDrawHueValueBand(x, y, width, main_height, 0.0f, 1.0f / 6.0f, 0x800000ffu, 0x8000ffffu);
        eduiDrawHueValueBand(x, y, width, main_height, 1.0f / 6.0f, 2.0f / 6.0f, 0x8000ffffu, 0x8000ff00u);
        eduiDrawHueValueBand(x, y, width, main_height, 2.0f / 6.0f, 0.5f, 0x8000ff00u, 0x80ffff00u);
        eduiDrawHueValueBand(x, y, width, main_height, 0.5f, 4.0f / 6.0f, 0x80ffff00u, 0x80ff0000u);
        eduiDrawHueValueBand(x, y, width, main_height, 4.0f / 6.0f, 5.0f / 6.0f, 0x80ff0000u, 0x80ff00ffu);
        eduiDrawHueValueBand(x, y, width, main_height, 5.0f / 6.0f, 1.0f, 0x80ff00ffu, 0x800000ffu);
        if (!edui_donotdraw) {
            i32 value_y = static_cast<i32>(y + main_height * picker->cursor_y) << 3;
            NuRndrLine2di(x << 4, value_y, (x + width - 1) << 4, value_y, 0x80ffffff, uimtls[0]);
        }
        if (!edui_donotdraw) {
            i32 hue_x = static_cast<i32>(x + width * picker->cursor_x) << 4;
            NuRndrLine2di(hue_x, y << 3, hue_x, ((y + main_height) << 3) - 8, 0x80ffffff, uimtls[0]);
        }
        i32 bar_height = width / 8;
        f32 full_red, full_green, full_blue;
        eduiHSVToRGB(picker->hue, 1.0f, picker->value, full_red, full_green, full_blue);
        i32 grey_byte = static_cast<i32>(picker->value * 255.0f);
        i32 grey = static_cast<i32>(0x80000000u | (grey_byte * 0x10101));
        i32 full = static_cast<i32>(0x80000000u | (static_cast<i32>(full_blue * 255.0f) << 16) |
                                    (static_cast<i32>(full_green * 255.0f) << 8) | static_cast<i32>(full_red * 255.0f));
        i32 saturation_colours[4] = {grey, full, grey, full};
        i32 saturation_y = y + main_height;
        if (!edui_donotdraw)
            NuRndrGradRect2di(x << 4, saturation_y << 3, width << 4, bar_height << 3, saturation_colours,
                              uimtls[ui_bgmtl]);
        if (!edui_donotdraw) {
            i32 marker_mid = saturation_y + (bar_height >> 1);
            for (i32 offset = 1; offset >= -1 && !edui_donotdraw; --offset) {
                i32 marker_x = static_cast<i32>(static_cast<f32>(x + offset) + picker->saturation * (width - 2)) << 4;
                NuRndrLine2di(marker_x, saturation_y << 3, marker_x, (marker_mid << 3) - 8, 0x80ffffff, uimtls[0]);
            }
            for (i32 offset = 1; offset >= -1 && !edui_donotdraw; --offset) {
                i32 marker_x = static_cast<i32>(static_cast<f32>(x + offset) + picker->saturation * (width - 2)) << 4;
                NuRndrLine2di(marker_x, marker_mid << 3, marker_x, ((saturation_y + bar_height) << 3) - 8, 0x80000000,
                              uimtls[0]);
            }
        }
        f32 red, green, blue;
        eduiHSVToRGB(picker->hue, picker->saturation, picker->value, red, green, blue);
        i32 selected = static_cast<i32>(0x80000000u | (static_cast<i32>(blue * 255.0f) << 16) |
                                        (static_cast<i32>(green * 255.0f) << 8) | static_cast<i32>(red * 255.0f));
        i32 selected_colours[4] = {selected, selected, selected, selected};
        if (__builtin_expect(!edui_donotdraw, 0))
            NuRndrGradRect2di(x << 4, (saturation_y + bar_height) << 3, width << 4, bar_height << 3, selected_colours,
                              uimtls[ui_bgmtl]);
        return width;
    }

    static __used__ i32 eduicbRenderSelWithClipColour(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width,
                                                      i32) {
        (void)menu;
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        {
            eduiDrawRect(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted],
                         uimtls[ui_bgmtl]);
            eduiDrawFontSet(edui_font);
            eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        }
        eduiFntPrintEx(edui_font, (x * 2 + width) << 3, (y << 3) + baseline, 64, item->text);
        f32 green = clipg;
        f32 red = clipg;
        f32 blue = clipg;
        if (item->type == 7)
            eduiHSVToRGB(cliph, clips, clipv, red, green, blue);
        u32 colour = 0x80000000u | (static_cast<i32>(blue * 255.0f) << 16) | (static_cast<i32>(green * 255.0f) << 8) |
                     static_cast<i32>(red * 255.0f);
        i32 colours[4] = {static_cast<i32>(colour), static_cast<i32>(colour), static_cast<i32>(colour),
                          static_cast<i32>(colour)};
        eduiDrawGradRect((x + width - 45) << 4, y << 3, 40 << 4, height << 3, colours, uimtls[ui_bgmtl]);
        return height;
    }

    void edmainSetReturn(i32 result) {
        editor_return = result;
    }

    static __used__ void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32) {
        edmainSetReturn(1);
    }

    // The editor UI callbacks share this translation unit with their cursor,
    // menu, and interaction state in the original binary.

    static __used__ void cbMMCancel(eduimenu_s *, eduimenu_s *) {
        edmainSetReturn(1);
    }

    ed_module_s *edmainCurrent(void) {
        return ed_module_active ? ed_curr : NULL;
    }

    i32 edmainActivate(ed_module_s *module, i32 notify) {
        if (module) {
            for (ed_module_s *entry = ed_list; entry; entry = entry->next) {
                if (entry == module) {
                    ed_curr = module;
                    ed_module_active = 1;
                    if (module->activate && notify)
                        module->activate();
                    return 1;
                }
            }
        } else if (ed_curr) {
            if (ed_curr->deactivate && notify)
                ed_curr->deactivate();
            ed_curr = NULL;
            ed_module_active = 0;
        }
        return 0;
    }

    void edmainSetCamera(NUMTX *matrix) {
        edmaincam->mtx = *matrix;
        NuCameraSet(edmaincam);
    }

    NUCAMERA *edmainGetCamera(void) {
        return edmaincam;
    }

    void edmainExtCamera(NUCAMERA *camera) {
        edmaincam = camera != NULL ? camera : edinternalcam;
    }

    void edmainSetCursorEnabled(i32 enabled) {
        edmain_cursor_enabled = enabled;
    }

    i32 edmainGetCursorEnabled(void) {
        return edmain_cursor_enabled;
    }

    eduimenu_s *edGetMainMenu(void) {
        return ed_main_menu;
    }

    void edmainRegisterLocVec(NUVEC *position) {
        ed_loc = position;
    }

    NUVEC *edmainQueryLocVec(void) {
        return ed_loc;
    }

    void eduiSetGlobalSliderAccel(f32 acceleration) {
        if (acceleration < 0.0f)
            acceleration = 0.0f;
        else if (acceleration > 1.0f)
            acceleration = 1.0f;
        slider_acceleration = acceleration;
    }

    void eduiInitMaterials(void) {
        uimtls[0] = NuMtlCreate(1);
        uimtls[0]->opacity = 1.0f;
        uimtls[0]->attribs.alpha_mode = 0;
        uimtls[0]->attribs.cull_mode = 2;
        uimtls[0]->attribs.z_mode = 3;
        NuMtlUpdate(uimtls[0]);
        uimtls[1] = NuMtlCreate(1);
        uimtls[1]->opacity = 0.999f;
        uimtls[1]->diffuse_color.r = 0.2f;
        uimtls[1]->diffuse_color.g = 0.2f;
        uimtls[1]->diffuse_color.b = 0.2f;
        uimtls[1]->attribs.cull_mode = 2;
        uimtls[1]->attribs.z_mode = 3;
        uimtls[1]->attribs.alpha_mode = 3;
        NuMtlUpdate(uimtls[1]);
        uimtls[2] = NuMtlCreate(1);
        uimtls[2]->opacity = 0.999f;
        uimtls[2]->diffuse_color.r = 0.2f;
        uimtls[2]->diffuse_color.g = 0.2f;
        uimtls[2]->diffuse_color.b = 0.2f;
        uimtls[2]->attribs.cull_mode = 2;
        uimtls[2]->attribs.z_mode = 3;
        uimtls[2]->attribs.alpha_mode = 2;
        NuMtlUpdate(uimtls[2]);
        uimtls[3] = NuMtlCreate(1);
        uimtls[3]->opacity = 0.999f;
        uimtls[3]->diffuse_color.r = 0.2f;
        uimtls[3]->diffuse_color.g = 0.2f;
        uimtls[3]->diffuse_color.b = 0.2f;
        uimtls[3]->attribs.cull_mode = 2;
        uimtls[3]->attribs.z_mode = 3;
        uimtls[3]->attribs.alpha_mode = 1;
        NuMtlUpdate(uimtls[3]);
    }

    void eduiSetRenderPlane(i8 plane) {
        NuMtlSetRenderPlane(uimtls[0], plane);
        NuMtlSetRenderPlane(uimtls[1], plane);
        NuMtlSetRenderPlane(uimtls[2], plane);
        NuMtlSetRenderPlane(uimtls[3], plane);
    }

    eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item) {
        if (menu->last)
            menu->last->next = item;
        else
            menu->first = item;
        item->previous = menu->last;
        item->next = NULL;
        menu->field_0c = menu->first;
        menu->field_10 = 0;
        menu->last = item;
        edui_last_item = item;
        return item;
    }

    void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item) {
        if (menu->first)
            menu->first->previous = item;
        else
            menu->last = item;
        item->next = menu->first;
        item->previous = NULL;
        menu->first = item;
        menu->field_10 = 0;
        menu->field_0c = item;
        edui_last_item = item;
    }

    void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item) {
        if (menu->last)
            menu->last->next = item;
        else
            menu->first = item;
        item->previous = menu->last;
        item->next = NULL;
        menu->field_10 = 0;
        menu->last = item;
        menu->field_0c = item;
        edui_last_item = item;
    }

    void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after) {
        if (after) {
            item->next = after->next;
            item->previous = after;
            after->next = item;
            if (item->next)
                item->next->previous = item;
            else
                menu->last = item;
            menu->field_10 = 0;
            menu->field_0c = menu->first;
            edui_last_item = item;
        } else
            eduiMenuAddItemFirst(menu, item);
    }

    void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before) {
        if (before) {
            item->previous = before->previous;
            item->next = before;
            before->previous = item;
            // The original does not update the preceding item's next link.
            if (!item->previous)
                menu->first = item;
            menu->field_0c = menu->first;
            menu->field_10 = 0;
            edui_last_item = item;
        } else
            eduiMenuAddItemLast(menu, item);
    }

    i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item) {
        if (menu->first == item)
            return 0;
        eduiitem_s *previous = item->previous;
        eduiitem_s *next = item->next;
        item->next = previous;
        item->previous = previous->previous;
        if (item->previous)
            item->previous->next = item;
        else
            menu->first = item;
        if (next)
            next->previous = previous;
        else
            menu->last = item->next;
        previous->previous = item;
        previous->next = next;
        menu->field_0c = menu->first;
        menu->field_10 = 0;
        return 1;
    }

    i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item) {
        if (menu->last == item)
            return 0;
        eduiitem_s *next = item->next;
        eduiitem_s *previous = item->previous;
        item->previous = next;
        item->next = next->next;
        if (item->next)
            item->next->previous = item;
        else
            menu->last = item;
        if (previous)
            previous->next = next;
        else
            menu->first = next;
        next->next = item;
        next->previous = previous;
        menu->field_0c = menu->first;
        menu->field_10 = 0;
        return 1;
    }

    void eduiMenuSortItemsByTxt(eduimenu_s *menu) {
        eduiitem_s *item = menu->first;
        if (item) {
            i32 sorted;
            do {
                eduiitem_s *first = item;
                sorted = 1;
                while (item->next && item->text && item->next->text) {
                    if (item->type != 20 && (item->next->type == 20 || NuStrICmp(item->text, item->next->text) > 0)) {
                        eduiMenuItemMoveDown(menu, item);
                        if (item == first)
                            first = item->previous;
                        sorted = 0;
                    } else {
                        item = item->next;
                    }
                }
                item = first;
            } while (!sorted);
        }
    }

    void eduiMenuSelectFirstEntry(eduimenu_s *menu) {
        menu->selected = NULL;
    }

    void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item) {
        if (menu->first == item)
            menu->first = item->next;
        if (menu->last == item)
            menu->last = item->previous;
        if (menu->selected == item)
            menu->selected = item->next ? item->next : item->previous;
        menu->field_0c = menu->first;
        menu->field_10 = 0;
        if (item->next)
            item->next->previous = item->previous;
        if (item->previous)
            item->previous->next = item->next;
        item->next = NULL;
        item->previous = NULL;
    }

    f32 eduiGetAnalougePadValue(nupad_s *pad) {
        f32 value = 0.0f;
        if (pad && (pad->digital_buttons & EDUI_CURSOR_PRIMARY)) {
            if (pad->analog_left_x > 192)
                value = (pad->analog_left_y - 128.0f) * 0.01f;
            if (pad->analog_left_x < 64)
                value = (128.0f - pad->analog_left_y) * -0.01f;
            if (pad->analog_right_x > 192)
                value = (pad->analog_right_y - 128.0f) * 0.001f;
            if (pad->analog_right_x < 64)
                value = (128.0f - pad->analog_right_y) * -0.001f;
        }
        return value;
    }

    eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback,
                               char *title) {
        u32 bytes = sizeof(eduimenu_s) + 1;
        if (title)
            bytes += NuStrLen(title);
        eduimenu_s *menu = static_cast<eduimenu_s *>(NU_ALLOC(bytes, 4, 1, "", 0));
        if (menu) {
            memset(menu, 0, bytes);
            menu->field_24 = -1;
            menu->x = x;
            menu->y = y;
            menu->width = width;
            menu->height = height;
            menu->field_28 = -1;
            menu->font = font;
            menu->callback = callback;
            menu->flags = 0;
            if (title) {
                menu->title = reinterpret_cast<char *>(menu + 1);
                NuStrCpy(menu->title, title);
            }
            active_menu = menu;
        }
        edui_last_item = NULL;
        return menu;
    }

    void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours) {
        do {
            for (eduiitem_s *item = menu->first; item; item = item->next) {
                item->colours[0] = colours[0];
                item->colours[1] = colours[1];
                item->colours[2] = colours[2];
                item->colours[3] = colours[3];
            }
            menu = menu->child;
        } while (menu);
    }

    void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours) {
        for (eduiitem_s *item = menu->first; item; item = item->next) {
            item->colours[0] &= 0xffffff;
            item->colours[0] |= colours[0] & 0xff000000;
            item->colours[1] &= 0xffffff;
            item->colours[1] |= colours[1] & 0xff000000;
            item->colours[2] &= 0xffffff;
            item->colours[2] |= colours[2] & 0xff000000;
            item->colours[3] &= 0xffffff;
            item->colours[3] |= colours[3] & 0xff000000;
        }
    }

    void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled) {
        do {
            for (eduiitem_s *item = menu->first; item; item = item->next)
                item->disabled = disabled;
            menu = menu->child;
        } while (menu);
    }

    void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item) {
        if (item->selection_group) {
            for (eduiitem_s *entry = menu->first; entry; entry = entry->next) {
                if (entry == item) {
                    if (entry->type == EDUI_ITEM_TOGGLE)
                        entry->highlighted = 1 - entry->highlighted;
                    else
                        entry->highlighted = 1;
                } else if (entry->selection_group == item->selection_group) {
                    entry->highlighted = 0;
                }
            }
        }
    }

    static __used__ i32 eduicbProcessSel(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad) {
        edui_sel_s *selection = static_cast<edui_sel_s *>(item);
        if (pad->digital_buttons_pressed & EDUI_CURSOR_PRIMARY) {
            eduiMenuHighlight(menu, item);
            if (selection->selected)
                selection->selected(menu, item, pad->digital_buttons);
        } else if ((pad->digital_buttons & EDUI_CURSOR_PRIMARY) && selection->held) {
            selection->held(menu, item, pad->digital_buttons);
        }
        return 0;
    }

    i32 eduiItemSetText(eduiitem_s *item, char *text) {
        if (item->text && NuStrLen(item->text) < NuStrLen(text)) {
            NU_FREE(item->text);
            item->text = NULL;
        }
        if (!item->text) {
            u32 size = NuStrLen(text) + 1;
            item->text = static_cast<char *>(NU_ALLOC(size, 4, 1, "", 0));
        }
        if (item->text) {
            NuStrCpy(item->text, text);
            return 1;
        }
        return 0;
    }

}

static __used__ void cbToggleIndicatorMode(eduimenu_s *, eduiitem_s *item, u32) {
    edui_colour_pick_s *picker = cp_item;
    item->data = 1 - item->data;
    cpUpdateInfo(item, picker);
}

static __used__ i32 cbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, float delta_time, nupad_s *pad) {
    edui_colour_pick_s *picker = cp_item;
    i32 result = cp_process(menu, item, delta_time, pad);
    char text[256];
    i32 mode = cp_info->data;
    if (mode != 0)
        sprintf(text, "R:%1.2f G:%1.2f B:%1.2f", picker->red, picker->green, picker->blue);
    else
        sprintf(text, "H:%1.2f S:%1.2f V:%1.2f", picker->hue, picker->saturation, picker->value);
    eduiItemSetText(cp_info, text);
    return result;
}

extern "C" {
    eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text) {
        edui_sel_s *item = static_cast<edui_sel_s *>(NU_ALLOC(sizeof(edui_sel_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 0;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessSel;
        item->render = eduicbRenderSel;
        item->destroy = eduicbItemDestroy;
        item->interact = eduicbInteractSel;
        item->highlighted = selected;
        item->selection_group = group;
        item->text = NULL;
        item->text_alignment = 0x40;
        eduiItemSetText(item, text ? text : (char *)"<no name>");
        item->selected = callback;
        item->held = NULL;
        return item;
    }

    eduiitem_s *eduiItemSelWithClipColourCreate(usize data, const void *colours, i32 selected, i32 group,
                                                EdUiItemCallback callback, char *text) {
        eduiitem_s *item = eduiItemSelCreate(data, colours, selected, group, callback, text);
        if (item)
            item->render = eduicbRenderSelWithClipColour;
        return item;
    }

    eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text) {
        eduiitem_s *item = eduiItemSelCreate(data, colours, selected, group, callback, text);
        if (item) {
            item->type = 2;
            item->process = eduicbProcessSel;
            item->render = eduicbRenderCheck;
        }
        return item;
    }

    eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text) {
        eduiitem_s *item = eduiItemSelCreate(data, colours, selected, group, callback, text);
        if (item) {
            item->type = 3;
            item->process = eduicbProcessSel;
            item->render = eduicbRenderCheck;
        }
        return item;
    }

    void eduiItemTextPickSetFmt(edui_textpicker_s *item, char *format) {
        if (item->format) {
            i32 old_length = NuStrLen(item->format);
            i32 new_length = NuStrLen(format);
            if (old_length >= new_length)
                goto copy;
            NU_FREE(item->format);
        }
        item->format = static_cast<char *>(NU_ALLOC(NuStrLen(format) + 1, 4, 1, "", 0));
    copy:
        NuStrCpy(item->format, format);
    }

    eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        auto *item = static_cast<edui_textpicker_s *>(NU_ALLOC(sizeof(edui_textpicker_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 1;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessTextPick;
        item->render = eduicbRenderTextPick;
        item->destroy = eduicbItemTextPickDestroy;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        item->max_length = 255;
        item->callback = callback;
        item->format = NULL;
        item->cursor = 0;
        eduiItemTextPickSetFmt(item, const_cast<char *>("\"%s\""));
        textrow[0] = const_cast<char *>("1234567890");
        textrow[1] = const_cast<char *>("QWERTYUIOP");
        textrow[2] = const_cast<char *>("ASDFGHJKL_");
        textrow[3] = const_cast<char *>("ZXCVBNM<>?");
        return item;
    }

    i32 cbCompateDirentByDateAsc(const void *first, const void *second) {
        return static_cast<const FilePickDirectoryEntry *>(first)->year -
               static_cast<const FilePickDirectoryEntry *>(second)->year;
    }

    i32 cbCompateDirentByDateDec(const void *first, const void *second) {
        return static_cast<const FilePickDirectoryEntry *>(second)->year -
               static_cast<const FilePickDirectoryEntry *>(first)->year;
    }

    i32 cbCompateDirentBySizeAsc(const void *first, const void *second) {
        i32 first_size = static_cast<const FilePickDirectoryEntry *>(first)->size;
        i32 second_size = static_cast<const FilePickDirectoryEntry *>(second)->size;
        if (first_size < second_size)
            return -1;
        if (first_size > second_size)
            return 1;
        return 0;
    }

    i32 cbCompateDirentBySizeDec(const void *first, const void *second) {
        i32 first_size = static_cast<const FilePickDirectoryEntry *>(first)->size;
        i32 second_size = static_cast<const FilePickDirectoryEntry *>(second)->size;
        if (first_size < second_size)
            return 1;
        if (first_size > second_size)
            return -1;
        return 0;
    }

    i32 cbCompateDirentByNameAsc(const void *first, const void *second) {
        return NuStrCmp(static_cast<const FilePickDirectoryEntry *>(first)->name,
                        static_cast<const FilePickDirectoryEntry *>(second)->name);
    }

    i32 cbCompateDirentByNameDec(const void *first, const void *second) {
        return NuStrCmp(static_cast<const FilePickDirectoryEntry *>(second)->name,
                        static_cast<const FilePickDirectoryEntry *>(first)->name);
    }

    void eduiItemFilePickSetFmt(edui_file_pick_s *item, char *format) {
        if (item->format) {
            i32 old_length = NuStrLen(item->format);
            i32 new_length = NuStrLen(format);
            if (old_length >= new_length)
                goto copy;
            NU_FREE(item->format);
        }
        item->format = static_cast<char *>(NU_ALLOC(NuStrLen(format) + 1, 4, 1, "", 0));
    copy:
        NuStrCpy(item->format, format);
    }

    eduiitem_s *eduiItemFilePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        auto *item = static_cast<edui_file_pick_s *>(NU_ALLOC(sizeof(edui_file_pick_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 1;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessFilePick;
        item->render = eduicbRenderFilePick;
        item->destroy = eduicbItemFilePickDestroy;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        item->changed = callback;
        item->format = NULL;
        item->name[0] = '\0';
        item->directory[0] = '\0';
        item->filename[0] = '\0';
        item->compare_entries = NULL;
        eduiItemFilePickSetFmt(item, const_cast<char *>("\"%s\""));
        return item;
    }

    void eduiGradStageSetRGB(edui_gradient_node_s *stage, f32 red, f32 green, f32 blue) {
        f32 hue = 0.0f, saturation = 0.0f, value;
        eduiRGBToHSV(red, green, blue, hue, saturation, value);
        stage->hue = hue;
        stage->value = value;
        stage->saturation = saturation;
        stage->colour = 0x80000000u | static_cast<u32>(static_cast<i32>(red)) |
                        (static_cast<u32>(static_cast<i32>(green)) << 8) |
                        (static_cast<u32>(static_cast<i32>(blue)) << 16);
    }

    void eduiGradStageSetHSV(edui_gradient_node_s *stage, f32 hue, f32 saturation, f32 value) {
        // Left uninitialised as in the original: the out-of-range sector path
        // reads undefined values, which GCC materialises as zero.
        f32 red, green, blue;
        eduiHSVToRGB(hue, saturation, value, red, green, blue);
        stage->saturation = saturation;
        stage->hue = hue;
        stage->value = value;
        stage->colour = 0x80000000u | static_cast<u32>(static_cast<i32>(red * 255.0f)) |
                        (static_cast<u32>(static_cast<i32>(green * 255.0f)) << 8) |
                        (static_cast<u32>(static_cast<i32>(blue * 255.0f)) << 16);
    }

    edui_gradient_node_s *eduiGradStageAdd(edui_gradient_pick_s *item, f32 time, f32 hue, f32 saturation, f32 value) {
        edui_gradient_node_s *stage = static_cast<edui_gradient_node_s *>(NU_ALLOC(sizeof(*stage), 4, 1, "", 0));
        if (!stage)
            return NULL;
        memset(stage, 0, sizeof(*stage));
        edui_gradient_node_s *previous = NULL;
        edui_gradient_node_s *next = item->first_stage;
        while (next && !(next->time > time)) {
            previous = next;
            next = next->next;
        }
        if (previous)
            previous->next = stage;
        else
            item->first_stage = stage;
        if (next)
            next->previous = stage;
        stage->previous = previous;
        stage->time = time;
        stage->next = next;
        eduiGradStageSetHSV(stage, hue, saturation, value);
        item->selected_stage = stage;
        return stage;
    }

    edui_gradient_node_s *eduiGradStageAddRGB(edui_gradient_pick_s *item, f32 time, f32 red, f32 green, f32 blue) {
        f32 hue = 0.0f, saturation = 0.0f, value;
        eduiRGBToHSV(red, green, blue, hue, saturation, value);
        return eduiGradStageAdd(item, time, hue, saturation, value);
    }

    void eduiGradStageDelete(edui_gradient_pick_s *item, edui_gradient_node_s *stage) {
        edui_gradient_node_s *previous = stage->previous;
        edui_gradient_node_s *next = stage->next;
        if (previous)
            previous->next = next;
        else
            item->first_stage = next;
        if (next)
            next->previous = previous;
        if (next) {
            item->selected_stage = next;
        } else {
            item->selected_stage = previous;
        }
        NU_FREE(stage);
    }

    static __used__ void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *item) {
        edui_gradient_pick_s *gradient = static_cast<edui_gradient_pick_s *>(item);
        while (gradient->first_stage)
            eduiGradStageDelete(gradient, gradient->first_stage);
        NU_FREE(gradient);
    }

    i32 eduiGradPickRead(eduiitem_s *item, edui_gradient_stage_s *stages, i32 capacity) {
        edui_gradient_node_s *node = static_cast<edui_gradient_pick_s *>(item)->first_stage;
        f32 red, green, blue;
        i32 count = 0;
        for (; node; node = node->next, ++count) {
            if (count < capacity) {
                edui_gradient_stage_s *stage = &stages[count];
                stage->time = node->time;
                stage->hue = node->hue;
                stage->value = node->value;
                stage->saturation = node->saturation;
                eduiHSVToRGB(node->hue, node->saturation, node->value, red, green, blue);
                stage->red = red;
                stage->green = green;
                stage->blue = blue;
                stage->colour = 0x80000000u + static_cast<i32>(red * 255.0f) + (static_cast<i32>(green * 255.0f) << 8) +
                                (static_cast<i32>(blue * 255.0f) << 16);
                stage->metadata = node->metadata;
            }
        }
        return count;
    }

    eduiitem_s *eduiItemGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        edui_gradient_pick_s *item =
            static_cast<edui_gradient_pick_s *>(NU_ALLOC(sizeof(edui_gradient_pick_s), 4, 1, "", 0));
        if (__builtin_expect(item != NULL, 1)) {
            memset(item, 0, sizeof(*item));
            item->type = 7;
            item->data = data;
            memcpy(item->colours, colours, sizeof(item->colours));
            item->process = eduicbProcessGradPick;
            item->render = eduicbRenderGradPick;
            item->destroy = eduicbItemGradPickDestroy;
            item->text_alignment = 0x40;
            item->selection_group = 0;
            eduiItemSetText(item, text);
            item->change_timer = 0;
            item->changed = callback;
        }
        return item;
    }

    eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        eduiitem_s *item = eduiItemGradPickCreate(data, colours, callback, text);
        if (item)
            item->type = 8;
        return item;
    }

    eduiitem_s *eduiItemDataGradPickCreate(usize data, const void *colours, EdUiItemCallback callback,
                                           EdUiItemCallback press, EdUiItemCallback add, EdUiItemCallback remove,
                                           EdUiItemCallback copy, EdUiItemCallback paste, char *text) {
        edui_gradient_pick_s *item =
            static_cast<edui_gradient_pick_s *>(eduiItemGradPickCreate(data, colours, callback, text));
        if (item) {
            item->type = 9;
            item->press = press;
            item->add = add;
            item->remove = remove;
            item->copy = copy;
            item->paste = paste;
        }
        return item;
    }

    void eduiItemColourPickSetHSV(edui_colour_pick_s *item, f32 hue, f32 saturation, f32 value) {
        item->hue = hue;
        item->saturation = saturation;
        item->value = value;
        item->cursor_x = hue / 360.0f;
        item->cursor_y = value;
    }

    void eduiItemColourPickSetRGB(edui_colour_pick_s *item, f32 red, f32 green, f32 blue) {
        f32 maximum = MAX(red, MAX(green, blue));
        f32 minimum = MIN(red, MIN(green, blue));
        item->value = maximum;
        item->saturation = maximum != 0.0f ? (maximum - minimum) / maximum : 0.0f;
        if (item->saturation != 0.0f) {
            f32 delta = maximum - minimum;
            f32 hue;
            if (red == maximum)
                hue = (green - blue) / delta;
            else if (green == maximum)
                hue = 2.0f + (blue - red) / delta;
            else if (blue == maximum)
                hue = 4.0f + (red - green) / delta;
            else
                hue = item->hue;
            hue *= 60.0f;
            if (hue < 0.0f)
                hue += 360.0f;
            item->hue = hue;
            item->cursor_y = maximum;
            item->cursor_x = hue / 360.0f;
        } else {
            item->hue = 0.0f;
            item->cursor_x = 0.0f;
            item->cursor_y = maximum;
        }
    }

}

static __used__ void cbPaste(eduimenu_s *, eduiitem_s *, u32) {
    *cp_r = clipboard_r;
    *cp_g = clipboard_g;
    *cp_b = clipboard_b;
    if (cp_rgba != nullptr)
        *cp_rgba = cpPackColour(*cp_r, *cp_g, *cp_b, *cp_a);
    eduiItemColourPickSetRGB(cp_item, clipboard_r, clipboard_g, clipboard_b);
}

extern "C" {
    eduiitem_s *eduiItemColourPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        edui_colour_pick_s *item = static_cast<edui_colour_pick_s *>(NU_ALLOC(sizeof(edui_colour_pick_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 10;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessColourPick;
        item->render = eduicbRenderColourPick;
        item->interact = eduicbInteractColourPick;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        item->destroy = eduicbItemDestroy;
        eduiItemSetText(item, text);
        item->cursor_x = 0.5f;
        item->cursor_y = 0.5f;
        item->hue = 180.0f;
        item->value = 0.5f;
        item->saturation = 1.0f;
        item->changed = callback;
        return item;
    }

    eduiitem_s *eduiItemGreyPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        eduiitem_s *item = eduiItemColourPickCreate(data, colours, callback, text);
        if (item) {
            item->type = 11;
            item->process = eduicbProcessGreyPick;
            item->render = eduicbRenderGreyPick;
        }
        return item;
    }

    eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        edui_texture_pick_s *item =
            static_cast<edui_texture_pick_s *>(NU_ALLOC(sizeof(edui_texture_pick_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 12;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessTexturePick;
        item->render = eduicbRenderTexturePick;
        item->destroy = eduicbItemDestroy;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        item->uv_x[0] = 0.4f;
        item->uv_y[0] = 0.4f;
        item->uv_x[1] = 0.5f;
        item->uv_y[1] = 0.5f;
        item->zoom = 1.0f;
        item->selected_corner = 0;
        item->changed = callback;
        return item;
    }

    eduiitem_s *eduiItemGraphCreate(usize data, const void *colours, EdUiItemCallback callback, nugraph_s *graph,
                                    i32 width, i32 height) {
        edui_graph_s *item = static_cast<edui_graph_s *>(NU_ALLOC(sizeof(edui_graph_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 15;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessGraph;
        item->render = eduicbRenderGraph;
        item->destroy = eduicbItemDestroy;
        item->changed = callback;
        item->graph = graph;
        item->width = width;
        item->height = height;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        item->selected_point = -1;
        item->cursor_x = 0.5f;
        item->cursor_y = 0.5f;
        item->x_scale = 1.0f;
        item->y_scale = 1.0f;
        for (i32 i = 0; i != 8; ++i)
            item->onion_skins[i] = NULL;
        if (graph->point_count <= 1)
            nugraphInit(graph);
        nugraphCalcCurve(graph, 100);
        return item;
    }

    void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph) {
        if (item->type == 15) {
            i32 slot;
            if (!item->onion_skins[0])
                slot = 0;
            else if (!item->onion_skins[1])
                slot = 1;
            else if (!item->onion_skins[2])
                slot = 2;
            else if (!item->onion_skins[3])
                slot = 3;
            else if (!item->onion_skins[4])
                slot = 4;
            else if (!item->onion_skins[5])
                slot = 5;
            else if (!item->onion_skins[6])
                slot = 6;
            else if (!item->onion_skins[7])
                slot = 7;
            else
                return;
            item->onion_skins[slot] = graph;
        }
    }

    void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y) {
        if (item->type == 15) {
            item->cursor_x = MIN(1.0f, (MAX(0.0f, x)));
            item->cursor_y = MIN(1.0f, (MAX(0.0f, y)));
        }
    }

    void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title) {
        if (item->type == 15) {
            if (x)
                NuStrCpy(item->x_label, x);
            if (y)
                NuStrCpy(item->y_label, y);
            if (title)
                NuStrCpy(item->title, title);
        }
    }

    void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify) {
        item->normalized_value = (value - item->minimum) / item->range;
        item->value = value;
        if (reset_timer)
            item->change_timer = 60;
        if (notify && item->changed)
            item->changed(NULL, item, 0);
    }

    void eduiItemSliderSetVal(edui_slider_s *item, f32 value) {
        eduiItemSliderSetValEx(item, value, 1, 1);
    }

    i32 eduicbInteractSlider(edui_interact_s *interact) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(interact->item);
        f32 value = (edui_cursor_x - interact->x) / interact->width;
        value = CLAMP(value, 0.0f, 1.0f);
        eduiItemSliderSetVal(slider, value * slider->range);
        return (edui_cursor_buttons & EDUI_CURSOR_PRIMARY) != 0;
    }

    void eduiItemSliderSetFmt(edui_slider_s *item, char *format) {
        if (!item->format) {
            u32 size = NuStrLen(format) + 1;
            item->format = static_cast<char *>(NU_ALLOC(size, 4, 1, "", 0));
        } else if (NuStrLen(item->format) < NuStrLen(format)) {
            NU_FREE(item->format);
            u32 size = NuStrLen(format) + 1;
            item->format = static_cast<char *>(NU_ALLOC(size, 4, 1, "", 0));
        }
        NuStrCpy(item->format, format);
    }

    void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity) {
        item->granularity = granularity;
    }

    eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text) {
        edui_slider_s *item = static_cast<edui_slider_s *>(NU_ALLOC(sizeof(edui_slider_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 6;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessSlider;
        item->interact = eduicbInteractSlider;
        item->render = eduicbRenderSlider;
        item->destroy = eduicbItemSliderDestroy;
        item->selection_group = group;
        item->text_alignment = 0x40;
        eduiItemSetText(item, text);
        item->granularity = 0.001f;
        item->changed = callback;
        item->minimum = minimum;
        item->range = range;
        item->format = NULL;
        eduiItemSliderSetFmt(item, (char *)"(%1.03f)");
        eduiItemSliderSetVal(item, value);
        return item;
    }

    eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 range, i32 value, char *text) {
        edui_slider_s *item = static_cast<edui_slider_s *>(
            eduiItemSliderCreate(data, colours, group, callback, minimum, range, value, text));
        if (item) {
            eduiItemSliderSetFmt(item, (char *)"(%d)");
            item->granularity = 1.0f;
            item->render = eduicbRenderSliderInt;
        }
        return item;
    }

    eduiitem_s *eduiItemNumberCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text) {
        eduiitem_s *item = eduiItemSliderCreate(data, colours, group, callback, minimum, range, value, text);
        if (item)
            item->render = eduicbRenderNumber;
        return item;
    }

    eduiitem_s *eduiItemTextSelectorCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           i32 count, i32 selected, char *text, char **options) {
        edui_text_selector_s *item =
            static_cast<edui_text_selector_s *>(NU_ALLOC(sizeof(edui_text_selector_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 6;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessSlider;
        item->interact = eduicbInteractSlider;
        item->render = eduicbRenderTextSelector;
        item->destroy = eduicbItemSliderDestroy;
        item->selection_group = group;
        item->text_alignment = 0x40;
        eduiItemSetText(item, text);
        item->format = NULL;
        item->changed = callback;
        item->minimum = 0.0f;
        item->range = static_cast<f32>(count - 1);
        item->granularity = 1.0f;
        eduiItemSliderSetVal(item, static_cast<f32>(selected));
        item->options = options;
        return item;
    }

    i32 eduiItemPropSetText(edui_prop_s *item, char *text) {
        if (item->property_text && NuStrLen(item->property_text) < NuStrLen(text)) {
            NU_FREE(item->property_text);
            item->property_text = NULL;
        }
        if (!item->property_text) {
            u32 size = NuStrLen(text) + 1;
            item->property_text = static_cast<char *>(NU_ALLOC(size, 4, 1, "", 0));
        }
        if (item->property_text) {
            NuStrCpy(item->property_text, text);
            return 1;
        }
        return 0;
    }

    i32 eduiGetCameraEnabled(void) {
        return bCameraEnabled;
    }

    void eduiSetCameraEnabled(i32 enabled) {
        bCameraEnabled = enabled;
    }

    i32 NuKeyToAscii(u32 key, i32 shifted) {
        for (NUKEYCHAR *entry = NuKeyChar; entry->key != 0; ++entry) {
            if (entry->key == key)
                return shifted ? entry->shifted : entry->normal;
        }
        return 0;
    }

    static i32 eduicbProcessPropKeyboard(eduimenu_s *menu, edui_prop_s *property) {
        if (!(property->unknown_property_flags & 1))
            return 0;
        i32 length = NuStrLen(eduiPropTextEdit);
        if (eduiPropTextPos < 0)
            eduiPropTextPos = 0;
        if (eduiPropTextPos > length)
            eduiPropTextPos = length;
        u32 modifiers = 0;
        i32 key = NuKeyGet(&modifiers);
        if (key == -1)
            return 0;
        if (key == 0x1c) {
            eduiItemPropSetText(property, eduiPropTextEdit);
            if (property->changed)
                property->changed(menu, property, 0);
            property->unknown_property_flags &= ~1;
            eduiPropTextPos = -1;
            return 0;
        }
        if (key < 0x1d) {
            if (key == 1) {
                property->unknown_property_flags &= ~1;
                eduiPropTextPos = -1;
                return 0;
            }
            if (key == 0xe) {
                if (eduiPropTextPos < 1)
                    return 0;
                char *cursor = eduiPropTextEdit + eduiPropTextPos - 1;
                --eduiPropTextPos;
                char character = *cursor;
                while (character) {
                    character = cursor[1];
                    *cursor++ = character;
                }
                return 0;
            }
        } else {
            if (key == 0xcd) {
                if (eduiPropTextPos < length)
                    ++eduiPropTextPos;
                return 0;
            }
            if (key == 0xd3) {
                if (eduiPropTextPos >= length)
                    return 0;
                char *cursor = eduiPropTextEdit + eduiPropTextPos;
                char character = *cursor;
                while (character) {
                    character = cursor[1];
                    *cursor++ = character;
                }
                return 0;
            }
            if (key == 0xcb) {
                if (eduiPropTextPos > 0)
                    --eduiPropTextPos;
                return 0;
            }
        }
        char letter = NuKeyToAscii(key, modifiers & 1);
        if (letter) {
            eduiPropTextEdit[length + 1] = '\0';
            char *cursor = eduiPropTextEdit + length;
            char *insertion = eduiPropTextEdit + eduiPropTextPos;
            if (insertion < cursor) {
                do {
                    *cursor = cursor[-1];
                    --cursor;
                } while (cursor != insertion);
            }
            *cursor = letter;
            ++eduiPropTextPos;
        }
        return 0;
    }

    static __used__ i32 eduicbProcessProp(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad) {
        (void)delta_time;
        edui_prop_s *property = static_cast<edui_prop_s *>(item);
        u8 *property_flags = reinterpret_cast<u8 *>(property) + 0x4c;
        if (__builtin_expect(!!(*property_flags & 2), 1)) {
            property->label_width = edui_cursor_x - static_cast<f32>(item->x);
            if (property->label_width < 1.0f)
                property->label_width = 1.0f;
            for (eduiitem_s *other = menu->first; other; other = other->next) {
                if (other->type == 17 && other != item)
                    static_cast<edui_prop_s *>(other)->label_width = property->label_width;
            }
            if (!(edui_cursor_buttons & EDUI_CURSOR_PRIMARY))
                *property_flags &= ~2;
        }
        eduicbProcessPropKeyboard(menu, property);
        eduiSetCameraEnabled(1);
        if ((property->unknown_property_flags & 8) ||
            (item->type == 17 && (pad->digital_buttons & EDUI_CURSOR_PRIMARY))) {
            if (property->button) {
                eduiSetCameraEnabled(0);
                property->button(menu, item, edui_cursor_buttons);
                property->unknown_property_flags &= ~8;
            }
            if (!(edui_cursor_buttons & EDUI_CURSOR_PRIMARY))
                property->unknown_property_flags &= ~8;
        }
        return 0;
    }

    eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data) {
        edui_prop_s *item = static_cast<edui_prop_s *>(NU_ALLOC(sizeof(edui_prop_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 17;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessProp;
        item->render = eduicbRenderProp;
        item->destroy = eduicbItemDestroyProp;
        item->interact = eduicbInteractProp;
        item->highlighted = 0;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        eduiItemPropSetText(item, value);
        item->button_type = button_type;
        item->label_width = 50.0f;
        item->selected = selected;
        item->changed = changed;
        item->button = button;
        item->extra_data = extra_data;
        return item;
    }

    eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value) {
        return eduiItemPropCreateEx(data, colours, selected, changed, button, button_type, text, value, 0);
    }

    eduiitem_s *eduiItemSeparatorCreate(usize data, const void *colours) {
        eduiitem_s *item = static_cast<eduiitem_s *>(NU_ALLOC(sizeof(eduiitem_s), 4, 1, "", 0));
        if (item) {
            memset(item, 0, sizeof(*item));
            item->type = 18;
            item->data = data;
            memcpy(item->colours, colours, sizeof(item->colours));
            item->process = eduicbProcessSeparator;
            item->render = eduicbRenderSeparator;
            item->text_alignment = 0x40;
            item->highlighted = 0;
            item->destroy = eduicbItemDestroy;
            item->selection_group = 0;
        }
        return item;
    }

    void eduicbMenuOpenAllexpanders(eduimenu_s *menu) {
        for (eduiitem_s *item = menu->first; item; item = item->next) {
            if (item->type == EDUI_ITEM_EXPANDER) {
                edui_expander_s *expander = static_cast<edui_expander_s *>(item);
                eduiitem_s *child = expander->first_child;
                if (child && !expander->open) {
                    expander->last_child->next = expander->next;
                    if (expander->next)
                        expander->next->previous = expander->last_child;
                    child->previous = expander;
                    expander->next = child;
                    expander->open = 1;
                }
            }
        }
    }

    void eduicbMenuCloseAllexpanders(eduimenu_s *menu) {
        for (eduiitem_s *item = menu->last; item; item = item->previous) {
            if (item->type == EDUI_ITEM_EXPANDER)
                eduicbItemExpanderClose(static_cast<edui_expander_s *>(item));
        }
    }

    void eduiMenuFitWidth(eduimenu_s *menu, i32 padding) {
        eduiitem_s *first = menu->first;
        eduicbMenuOpenAllexpanders(menu);
        NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
        NuQFntSetScale(edui_font, edui_font_scale_x, edui_font_scale_y);

        i32 width = 0;
        if (menu->title) {
            i32 title_width = static_cast<i32>(NuQFntPrintLenU(edui_font, menu->title)) >> 4;
            if (title_width > width)
                width = title_width;
        }
        i32 property_label_width = 0;
        i32 property_value_width = 0;

        for (eduiitem_s *item = first; item; item = item->next) {
            i32 item_text_width = 0;
            switch (item->type) {
                case 1:
                    item_text_width = (static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3) * 10;
                    break;
                case 2:
                case 3:
                    if (item->text) {
                        item_text_width = (static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3) +
                                          (static_cast<i32>(NuQFntPrintLenU(edui_font, item->text)) >> 4);
                    }
                    break;
                case 17: {
                    edui_prop_s *property = static_cast<edui_prop_s *>(item);
                    i32 label_width = item->text ? static_cast<i32>(NuQFntPrintLenU(edui_font, item->text)) >> 4 : 30;
                    if (label_width > property_label_width)
                        property_label_width = label_width;
                    i32 value_width = property->property_text
                                          ? static_cast<i32>(NuQFntPrintLenU(edui_font, property->property_text)) >> 4
                                          : 30;
                    if (value_width > property_value_width)
                        property_value_width = value_width;
                    item_text_width = value_width;
                    break;
                }
                case 6: {
                    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
                    char format[512];
                    char text[512];
                    NuStrCpy(format, item->text);
                    NuStrCat(format, slider->format);
                    sprintf(text, format, 0, 0x80000000);
                    item_text_width = static_cast<i32>(NuQFntPrintLenU(edui_font, text)) >> 4;
                    break;
                }
                default:
                    if (item->text)
                        item_text_width = static_cast<i32>(NuQFntPrintLenU(edui_font, item->text)) >> 4;
                    break;
            }
            if (item_text_width > width)
                width = item_text_width;
        }

        i32 property_width =
            property_label_width + property_value_width + (static_cast<i32>(NuQFntHeight(edui_font)) >> 3);
        if (property_width > width)
            width = property_width;

        for (eduiitem_s *item = menu->first; item; item = item->next) {
            if (item->type == 17)
                static_cast<edui_prop_s *>(item)->label_width = static_cast<f32>(property_label_width + 1);
        }
        menu->width = width + padding;

        NuQFntPopCoordinateSystem();
        eduicbMenuCloseAllexpanders(menu);
    }

    f32 edmainSetMainMenuScale(f32 scale) {
        f32 previous = edmain_menu_scale;
        edmain_menu_scale = scale;
        if (ed_main_menu)
            eduiMenuFitWidth(ed_main_menu, 8);
        return previous;
    }

extern "C" void eduiMenuDestroyItems(eduimenu_s *menu) {
    if (menu) {
        eduicbMenuCloseAllexpanders(menu);
        while (menu->first) {
            eduiitem_s *next = menu->first->next;
            menu->first->destroy(menu, menu->first);
            menu->first = next;
        }
        menu->last = NULL;
        menu->selected = NULL;
        menu->field_0c = NULL;
        menu->field_10 = NULL;
    }
}

    void eduiMenuDestroy(eduimenu_s *menu) {
        if (menu) {
            if (eduiInteractLocked && eduiInteractLocked->menu == menu)
                eduiInteractLocked = NULL;
            if (processing_menu == menu)
                processing_menu = NULL;
            if (menu->parent) {
                if (active_menu == menu)
                    active_menu = menu->parent;
                menu->parent->child = NULL;
            }
            if (default_active_menu == menu)
                default_active_menu = NULL;
            if (active_menu == menu)
                active_menu = NULL;
            if (menu->child)
                menu->child->parent = NULL;
            eduiMenuDestroyItems(menu);
            NU_FREE(menu);
        }
    }

    void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *) {
        eduiMenuDestroy(edui_messagemenu);
        edui_messagemenu = NULL;
    }

    static __used__ void eduicbPickCFGCancel(eduimenu_s *menu, eduimenu_s *) {
        eduiMenuDestroy(menu);
    }

    void edmainClose(void) {
        if (ed_init) {
            NuCameraDestroy(edinternalcam);
            while (ed_list) {
                if (ed_list->close)
                    ed_list->close();
                ed_list = ed_list->next;
            }
            if (ed_main_menu) {
                eduiMenuDestroy(ed_main_menu);
                ed_main_menu = NULL;
            }
            if (ed_cfg_menu) {
                eduiMenuDestroy(ed_cfg_menu);
                ed_cfg_menu = NULL;
            }
            ed_list = NULL;
            ed_curr = NULL;
            ed_init = 0;
        }
    }

    i32 edmainRegister(ed_module_s *module) {
        if (ed_init)
            edmainClose();
        if (ed_list)
            ed_list->previous = module;
        module->next = ed_list;
        ed_list = module;
        module->previous = NULL;
        return 1;
    }

    void eduiIitemExpanderSetDepth(edui_expander_s *item, i32 depth) {
        item->depth = depth;
        for (eduiitem_s *child = item->first_child; child; child = child->next) {
            if (child->type == 19)
                eduiIitemExpanderSetDepth(static_cast<edui_expander_s *>(child), depth + 1);
            if (child->type == 17)
                static_cast<edui_prop_s *>(child)->depth = depth + 1;
            if (child == item->last_child)
                break;
        }
    }

    void eduiItemExpanderAddChild(edui_expander_s *item, eduiitem_s *child) {
        child->previous = item->last_child;
        if (item->last_child)
            item->last_child->next = child;
        item->last_child = child;
        if (!item->first_child)
            item->first_child = child;
        child->next = NULL;
        if (child->type == 19)
            eduiIitemExpanderSetDepth(static_cast<edui_expander_s *>(child), item->depth + 1);
        if (child->type == 17)
            static_cast<edui_prop_s *>(child)->depth = item->depth + 1;
    }

    eduiitem_s *eduiItemExpanderCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        edui_expander_s *item = static_cast<edui_expander_s *>(NU_ALLOC(sizeof(edui_expander_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 19;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessExpander;
        item->render = eduicbRenderExpander;
        item->interact = eduicbInteractExpander;
        item->destroy = eduicbItemDestroyExpander;
        item->highlighted = 0;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        item->changed = callback;
        return item;
    }

    void eduiItemFilterAddItem(edui_filter_s *item, eduiitem_s *child) {
        eduiitem_s *first = item->first_child;
        child->previous = NULL;
        child->next = first;
        if (first)
            first->previous = child;
        item->first_child = child;
    }

    void eduiItemFilterRemoveItem(edui_filter_s *item, eduiitem_s *child) {
        if (item->first_child == child)
            item->first_child = child->next;
        if (child->next)
            child->next->previous = child->previous;
        if (child->previous)
            child->previous->next = child->next;
        child->next = NULL;
        child->previous = NULL;
    }

    static __used__ i32 eduicbProcessFilter(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad) {
        auto *filter = static_cast<edui_filter_s *>(item);
        (void)delta_time;
        u8 *filter_flags = reinterpret_cast<u8 *>(filter) + 0x4c;
        if (__builtin_expect(!!(*filter_flags & 2), 1)) {
            filter->label_width = edui_cursor_x - static_cast<f32>(item->x);
            if (filter->label_width < 5.0f)
                filter->label_width = 5.0f;
            for (eduiitem_s *other = menu->first; other; other = other->next) {
                if (__builtin_expect(other->type == 17 && other != item, 0))
                    static_cast<edui_prop_s *>(other)->label_width = filter->label_width;
            }
            if (!(edui_cursor_buttons & EDUI_CURSOR_PRIMARY))
                *filter_flags &= ~2;
        }
        eduicbProcessPropKeyboard(menu, filter);
        eduiSetCameraEnabled(1);
        if ((filter->unknown_property_flags & 8) ||
            (pad && item->type == 17 && (pad->digital_buttons & EDUI_CURSOR_PRIMARY))) {
            if (filter->button) {
                eduiSetCameraEnabled(0);
                filter->button(menu, item, edui_cursor_buttons);
                filter->unknown_property_flags &= ~8;
            }
            if (!(edui_cursor_buttons & EDUI_CURSOR_PRIMARY))
                filter->unknown_property_flags &= ~8;
        }
        char *query = (filter->unknown_property_flags & 1) ? eduiPropTextEdit : filter->property_text;
        i32 changed = 0;
        if (!(!*query)) {
            for (eduiitem_s *candidate = menu->first; candidate;) {
                eduiitem_s *next = candidate->next;
                if (candidate->type != 18 && candidate->type != 20 && !NuStrIStr(candidate->text, query)) {
                    eduiMenuRemoveItem(menu, candidate);
                    eduiItemFilterAddItem(filter, candidate);
                    changed = 1;
                }
                candidate = next;
            }
            for (eduiitem_s *candidate = filter->first_child; candidate;) {
                eduiitem_s *next = candidate->next;
                if (NuStrIStr(candidate->text, query)) {
                    eduiItemFilterRemoveItem(filter, candidate);
                    eduiMenuAddItem(menu, candidate);
                    changed = 1;
                }
                candidate = next;
            }
        } else {
            for (eduiitem_s *child = filter->first_child; child;) {
                eduiitem_s *next = child->next;
                eduiItemFilterRemoveItem(filter, child);
                eduiMenuAddItem(menu, child);
                child = next;
                changed = 1;
            }
        }
        if (changed || !(filter->unknown_84[4] & 1)) {
            eduiMenuSortItemsByTxt(menu);
            filter->unknown_84[4] |= 1;
        }
        return 0;
    }

    eduiitem_s *eduiItemFilterCreate(usize data, const void *colours, char *text, char *value) {
        edui_filter_s *item = static_cast<edui_filter_s *>(NU_ALLOC(sizeof(edui_filter_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 20;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessFilter;
        item->render = eduicbRenderFilter;
        item->interact = eduicbInteractFilter;
        item->destroy = eduicbItemDestroyFilter;
        item->highlighted = 0;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        eduiItemPropSetText(item, value);
        item->label_width = 50.0f;
        return item;
    }

    i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected) {
        item_width = width;
        i32 height = item->render(menu, item, x, y, width, selected);
        if (selected) {
            u32 colour = (item->flags & 4) ? eduiitem_context_outline_colour : eduiitem_selected_outline_colour;
            if (__builtin_expect(!edui_donotdraw, 1))
                NuRndrLineRect2di(x << 4, (y << 3) + 8, item_width << 4, (height << 3) - 16, colour, uimtls[ui_outmtl]);
        }
        if (item->disabled) {
            i32 left = x << 4;
            i32 top = y << 3;
            i32 span = width << 4;
            i32 bottom = top + (height << 3);
            eduiDrawLine(left, top, left + span / 4, bottom, item->colours[0], uimtls[ui_bgmtl]);
            eduiDrawLine(left + span / 4, top, left + span / 2, bottom, item->colours[0], uimtls[ui_bgmtl]);
            eduiDrawLine(left + span / 2, top, left + span * 3 / 4, bottom, item->colours[0], uimtls[ui_bgmtl]);
            eduiDrawLine(left + span * 3 / 4, top, left + span, bottom, item->colours[0], uimtls[ui_bgmtl]);
        }
        return height;
    }

    void eduiSetUsingMenuFocus(i32 enabled) {
        bUsingMenuFocus = enabled;
    }

    i32 eduiGetUsingMenuFocus(void) {
        return bUsingMenuFocus;
    }

    void eduiSetDefaultActiveMenu(eduimenu_s *menu) {
        default_active_menu = menu;
    }

    void eduiSetActiveMenu(eduimenu_s *menu) {
        if (eduiGetUsingMenuFocus()) {
            if (menu) {
                while (__builtin_expect(menu->child != NULL, 0))
                    menu = menu->child;
                active_menu = menu;
            } else {
                menu = default_active_menu;
                if (menu)
                    while (menu->child)
                        menu = menu->child;
                active_menu = menu;
            }
        }
    }

    i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child) {
        if (child->parent)
            return 0;
        while (menu->child)
            menu = menu->child;
        child->parent = menu;
        menu->child = child;
        eduiSetActiveMenu(child);
        return 1;
    }

    static __used__ void cbSubMenu(eduimenu_s *menu, eduiitem_s *item) {
        eduiMenuAttach(menu, reinterpret_cast<eduimenu_s *>(item->data_ptr));
    }

    void eduiCreate3LineMessageMenu(eduimenu_s *parent, char *first, char *second, char *third, i32 first_highlight,
                                    i32 second_highlight, i32 third_highlight) {
        eduiiattr_s normal{0x800000c0, 0x80ff0000, 0x80808080, 0x80404040};
        eduiiattr_s highlighted{0x8000c000, 0x80ff0000, 0x80808080, 0x80404040};
        edui_messagemenu =
            eduiMenuCreate(70, 70, 300, 250, parent->font, eduicbCancelMessageMenu, const_cast<char *>("Message"));
        if (edui_messagemenu) {
            if (first)
                eduiMenuAddItem(edui_messagemenu,
                                eduiItemSelCreate(1, first_highlight ? &highlighted : &normal, 0, 0, NULL, first));
            if (second)
                eduiMenuAddItem(edui_messagemenu,
                                eduiItemSelCreate(1, second_highlight ? &highlighted : &normal, 0, 0, NULL, second));
            if (third)
                eduiMenuAddItem(edui_messagemenu,
                                eduiItemSelCreate(1, third_highlight ? &highlighted : &normal, 0, 0, NULL, third));
            eduiMenuAttach(parent, edui_messagemenu);
            edui_messagemenu->x = parent->x + 10;
            edui_messagemenu->y = parent->y + 40;
        }
    }

    void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted) {
        static const u32 initial_colours[4]
            __attribute__((aligned(16))) = {0x800000c0u, 0x80ff0000, 0x80808080, 0x80404040};
        u32 colours[4] __attribute__((aligned(16)));
        memcpy(colours, initial_colours, sizeof(colours));
        if (highlighted == 1)
            colours[0] = 0x8000c000u;
        edui_messagemenu =
            eduiMenuCreate(70, 70, 180, 250, parent->font, eduicbCancelMessageMenu, const_cast<char *>("Message"));
        if (!edui_messagemenu)
            return;
        eduiMenuAddItem(edui_messagemenu, eduiItemSelCreate(1, colours, 0, 0, NULL, message));
        eduiMenuAttach(parent, edui_messagemenu);
        edui_messagemenu->x = parent->x + 10;
        edui_messagemenu->y = parent->y + 40;
    }

    static __used__ i32 eduicbProcessGradPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad) {
        (void)delta_time;
        edui_gradient_pick_s *gradient = static_cast<edui_gradient_pick_s *>(item);
        if (gradient->changed)
            gradient->changed(menu, item, pad->digital_buttons);
        if (pad->digital_buttons_pressed & 0x40) {
            static eduimenu_s *gpcfg;
            gpcfg = eduiMenuCreate(menu->x + 10, menu->y + 40, 180, 250, menu->font, eduicbPickCFGCancel,
                                   "GradPick Adjust");
            if (!gpcfg)
                return 0;
            eduiitem_s *pick;
            if (item->type == 8) {
                pick = eduiItemGreyPickCreate(reinterpret_cast<usize>(item), &ed_attr, cbgpcfgCPPress, "Colourpick");
                if (gradient->selected_stage)
                    eduiItemColourPickSetHSV(static_cast<edui_colour_pick_s *>(pick), gradient->selected_stage->hue,
                                             gradient->selected_stage->saturation, gradient->selected_stage->value);
            } else if (item->type == 9)
                pick = eduiItemSelCreate(reinterpret_cast<usize>(item), &ed_attr, 0, 0, cbgpcfgCPPress, "Edit");
            else {
                pick = eduiItemColourPickCreate(reinterpret_cast<usize>(item), &ed_attr, cbgpcfgCPPress, "Colourpick");
                if (gradient->selected_stage)
                    eduiItemColourPickSetHSV(static_cast<edui_colour_pick_s *>(pick), gradient->selected_stage->hue,
                                             gradient->selected_stage->saturation, gradient->selected_stage->value);
            }
            eduiMenuAddItem(gpcfg, pick);
            eduiMenuAddItem(gpcfg, eduiItemSelCreate(reinterpret_cast<usize>(item), &ed_attr, 0, 0, cbgpcfgAdd, "Add"));
            eduiMenuAddItem(gpcfg, eduiItemSelCreate(reinterpret_cast<usize>(item), &ed_attr, 0, 0, cbgpcfgDel, "Del"));
            eduiMenuAddItem(gpcfg,
                            eduiItemSelCreate(reinterpret_cast<usize>(item), &ed_attr, 0, 0, cbgpcfgCopy, "Copy"));
            edui_last_item->type = item->type;
            eduiMenuAddItem(gpcfg, eduiItemSelWithClipColourCreate(reinterpret_cast<usize>(item), &ed_attr, 0, 0,
                                                                   cbgpcfgPaste, "Paste"));
            edui_last_item->type = item->type;
            eduiMenuAttach(menu, gpcfg);
            return 0;
        }
        if (!gradient->selected_stage)
            gradient->selected_stage = gradient->first_stage;
        edui_gradient_node_s *stage = gradient->selected_stage;
        if (!stage)
            return 0;
        if (pad->digital_buttons_pressed & 0x8000) {
            if (stage->previous)
                gradient->selected_stage = stage->previous;
            return 1;
        }
        if (pad->digital_buttons_pressed & 0x2000) {
            if (stage->next)
                gradient->selected_stage = stage->next;
            return 1;
        }
        if (pad->digital_buttons & 0xa000)
            return 1;
        f32 lower = stage->previous ? stage->previous->time : 0.0f;
        f32 upper = stage->next ? stage->next->time : 1.0f;
        u8 right2 = pad->analog_r2;
        u8 left2 = pad->analog_l2;
        u8 left1 = pad->analog_l1;
        u8 right1 = pad->analog_r1;
        i32 decrease = left1 ? left1 : left2;
        i32 increase = right1 ? right1 : right2;
        if (!(increase && !decrease)) {
            if (decrease) {
                f32 time = stage->time - static_cast<f32>(decrease) / 15360.0f;
                stage->time = lower > time ? lower : time;
                gradient->change_timer = 60;
            }
        } else {
            f32 time = static_cast<f32>(increase) / 15360.0f + stage->time;
            stage->time = upper < time ? upper : time;
            gradient->change_timer = 60;
        }
        return 0;
    }

    static void eduicbAttachDirectoryList(eduimenu_s *menu, edui_file_pick_s *picker) {
        if (!dir_list)
            return;

        picker->reopen_directory = 0;
        filepick_menu = menu;
        filepick_item = picker;
        eduimenu_s *directory_menu = eduiMenuCreate(menu->x + 20, menu->y + 20, 250, 300, menu->font,
                                                    eduicbDestroyDirectoryList, const_cast<char *>("Directory"));
        if (!directory_menu)
            return;

        char text[1032];
        sprintf(text, "%s%s", picker->directory, picker->filename);
        NUFILE directory = NuFileOpenDir(text);
        if (directory) {
            dir_size = 0;
            auto *entries = static_cast<FilePickDirectoryEntry *>(dir_list);
            while (dir_size < edui_filepick_listsize && NuFileReadDir(directory, &entries[dir_size]) > 0)
                ++dir_size;
            if (picker->compare_entries)
                qsort(entries, dir_size, sizeof(FilePickDirectoryEntry), picker->compare_entries);

            if (picker->filename[0]) {
                eduiitem_s *parent = eduiItemSelCreate(9999, picker->colours, 0, 1, eduicbSelectDirectoryEntry,
                                                       const_cast<char *>(".."));
                parent->text_alignment = 0x10;
                eduiMenuAddItem(directory_menu, parent);
            }

            for (i32 index = 0; index < dir_size; ++index) {
                FilePickDirectoryEntry &entry = entries[index];
                if (!(entry.flags & 8))
                    continue;
                sprintf(text, "<%s>", entry.name);
                eduiitem_s *item = eduiItemSelCreate(index, picker->colours, 0, 1, eduicbSelectDirectoryEntry, text);
                item->text_alignment = 0x10;
                eduiMenuAddItem(directory_menu, item);
            }

            for (i32 index = 0; index < dir_size; ++index) {
                FilePickDirectoryEntry &entry = entries[index];
                if (entry.flags & 8 || (picker->filename[0x100] && !NuStrIStr(entry.name, picker->filename + 0x100)))
                    continue;
                sprintf(text, "%s  ( size=%d  date=%d/%d/%d %d:%d )", entry.name, entry.size, entry.month, entry.day,
                        entry.year, entry.hour, entry.minute);
                eduiitem_s *item = eduiItemSelCreate(index, picker->colours, 0, 1, eduicbSelectDirectoryEntry, text);
                item->text_alignment = 0x10;
                eduiMenuAddItem(directory_menu, item);
            }

            if (!directory_menu->first)
                eduiMenuAddItem(directory_menu,
                                eduiItemSelCreate(dir_size, picker->colours, 0, 1, eduicbSelectDirectoryExit,
                                                  const_cast<char *>("No Files")));
            NuFileCloseDir(directory);
        }
        eduiMenuFitWidth(directory_menu, 8);
        eduiMenuAttach(menu, directory_menu);
    }

    static __used__ i32 eduicbProcessFilePick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad) {
        (void)delta_time;
        auto *picker = static_cast<edui_file_pick_s *>(item);
        if ((pad->digital_buttons_pressed & EDUI_CURSOR_PRIMARY) || picker->reopen_directory) {
            eduiMenuHighlight(menu, item);
            eduicbAttachDirectoryList(menu, picker);
        }
        return 0;
    }

    edui_textpicker_s *eduiAddTextPickEntEx(eduimenu_s *menu, eduiitem_s *source, EdUiItemCallback callback) {
        static eduimenu_s *tpent;
        tpent =
            eduiMenuCreate(source->x + 10, source->y + 10, 180, 50, menu->font, NULL, const_cast<char *>("Enter Text"));
        tpent->flags |= 2;
        auto *picker = static_cast<edui_textpicker_s *>(
            eduiItemTextPickCreate(reinterpret_cast<usize>(source), &ed_attr, callback, const_cast<char *>("")));
        eduiMenuAddItem(tpent, picker);
        eduiMenuAttach(menu, tpent);
        picker->editing = 1;
        eduiMenuFitWidth(tpent, 8);
        picker->cursor = NuStrLen(picker->value);
        return picker;
    }

    void eduiAddPropTextPickEnt(eduimenu_s *menu, eduiitem_s *source) {
        auto *picker = eduiAddTextPickEntEx(menu, source, tpentPropOnEnter);
        if (!picker)
            return;
        NuStrCpy(picker->value, static_cast<edui_prop_s *>(source)->property_text);
        picker->cursor = NuStrLen(picker->value);
        memcpy(picker->colours, source->colours, sizeof(picker->colours));
    }

    void eduiAddTextPickEnt(eduimenu_s *menu, eduiitem_s *source) {
        auto *picker = eduiAddTextPickEntEx(menu, source, tpentOnEnter);
        if (!picker)
            return;
        NuStrCpy(picker->value, static_cast<edui_textpicker_s *>(source)->value);
        picker->cursor = NuStrLen(picker->value);
        memcpy(picker->colours, source->colours, sizeof(picker->colours));
        picker->max_length = static_cast<edui_textpicker_s *>(source)->max_length;
        u8 *picker_flags = reinterpret_cast<u8 *>(&picker->keyboard_flags);
        const u8 *source_flags =
            reinterpret_cast<const u8 *>(&static_cast<edui_textpicker_s *>(source)->keyboard_flags);
        *picker_flags = (*picker_flags & ~2u) | (*source_flags & 2u);
    }

    static __used__ i32 eduicbProcessTextPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad) {
        auto *picker = static_cast<edui_textpicker_s *>(item);
        u32 modifiers;
        i32 key = NuKeyGet(&modifiers);
        NuStrLen(picker->value);

        auto delete_previous = [&]() {
            if (__builtin_expect(picker->cursor == 0, 0))
                return;
            if (picker->value[picker->cursor]) {
                --picker->cursor;
                for (i32 index = picker->cursor; index < 0xff; ++index)
                    picker->value[index] = picker->value[index + 1];
            } else {
                picker->value[--picker->cursor] = '\0';
            }
        };
        auto insert_character = [&](char character) {
            if (__builtin_expect(NuStrLen(picker->value) >= picker->max_length, 0))
                return;
            // The original includes the cursor slot in the shift before
            // overwriting it. Read the enclosing object's bytes so cursor zero
            // accesses the preceding object byte rather than value[-1].
            const char *picker_bytes = reinterpret_cast<const char *>(picker);
            for (i32 index = 0xff; index >= picker->cursor; --index)
                picker->value[index] = picker_bytes[offsetof(edui_textpicker_s, value) + index - 1];
            picker->value[picker->cursor++] = character;
        };
        auto next_row = [&]() {
            if (++picker->keyboard_row > ((picker->keyboard_flags & 2) ? 5 : 3))
                picker->keyboard_row = 0;
        };
        auto previous_row = [&]() {
            if (__builtin_expect(--picker->keyboard_row < 0, 1))
                picker->keyboard_row = (picker->keyboard_flags & 2) ? 5 : 3;
        };
        auto next_column = [&]() {
            if (__builtin_expect(picker->keyboard_row >= 4, 0))
                return;
            if (++picker->keyboard_column >= NuStrLen(textrow[picker->keyboard_row]))
                picker->keyboard_column = 0;
        };
        auto previous_column = [&]() {
            if (picker->keyboard_row >= 4)
                return;
            if (--picker->keyboard_column < 0)
                picker->keyboard_column = NuStrLen(textrow[picker->keyboard_row]) - 1;
        };

        if (__builtin_expect(key != -1, 1)) {
            switch (key) {
                case 0xc8:
                    previous_row();
                    break;
                case 0xd0:
                    next_row();
                    break;
                case 0xcb:
                    previous_column();
                    break;
                case 0xcd:
                    next_column();
                    break;
                case 0x0e:
                case 0xd3:
                    delete_previous();
                    break;
                case 0x1c:
                case 1:
                    if (__builtin_expect(!!(picker->callback), 0))
                        picker->callback(menu, item, pad->digital_buttons);
                    break;
                default: {
                    char character = NuKeyToAscii(key, modifiers & 1);
                    if (character)
                        insert_character(character);
                } break;
            }
        }

        if (__builtin_expect(!picker || !picker->editing, 0)) {
            if (pad->digital_buttons_pressed & 0x40)
                eduiAddTextPickEnt(menu, item);
            return 0;
        }

        if (__builtin_expect(!!(pad->digital_buttons_pressed & 0x4000), 0))
            next_row();
        if (__builtin_expect(!!(pad->digital_buttons_pressed & 0x1000), 0))
            previous_row();
        if (__builtin_expect(!!(pad->digital_buttons_pressed & 0x8000), 1))
            previous_column();
        if (pad->digital_buttons_pressed & 0x2000)
            next_column();
        if ((pad->digital_buttons_pressed & 0x10) && picker->callback)
            picker->callback(menu, item, pad->digital_buttons);
        if ((pad->digital_buttons & 0xc) == 0xc) {
            picker->cursor = 0;
            picker->value[0] = '\0';
            if (__builtin_expect((picker->keyboard_flags & 2) && TextPickCopyBuffer[0], 0)) {
                NuStrNCpy(picker->value, TextPickCopyBuffer, picker->max_length - 1);
                picker->value[picker->max_length - 1] = '\0';
                picker->cursor = NuStrLen(picker->value);
            }
        }
        if ((pad->digital_buttons & 3) == 3 && (picker->keyboard_flags & 2)) {
            NuStrNCpy(TextPickCopyBuffer, picker->value, 0x40);
            picker->value[0x3f] = '\0';
        }
        if (pad->digital_buttons_pressed & 4)
            picker->cursor = picker->cursor > 0 ? picker->cursor - 1 : 0;
        if (pad->digital_buttons_pressed & 8) {
            if (__builtin_expect(picker->cursor < NuStrLen(picker->value), 0))
                ++picker->cursor;
            else
                picker->cursor = NuStrLen(picker->value);
        }
        if (pad->digital_buttons_pressed & 0x40)
            insert_character(textrow[picker->keyboard_row][picker->keyboard_column]);
        if (__builtin_expect(!!(pad->digital_buttons_pressed & 0x80), 1))
            delete_previous();
        if (__builtin_expect(!!(pad->digital_buttons_pressed & 0x20), 0)) {
            if (textrow[1] == const_cast<char *>("QWERTYUIOP")) {
                textrow[0] = const_cast<char *>("1234567890");
                textrow[1] = const_cast<char *>("qwertyuiop");
                textrow[2] = const_cast<char *>("asdfghjkl_");
                textrow[3] = const_cast<char *>("zxcvbnm<>?");
            } else {
                textrow[0] = const_cast<char *>("1234567890");
                textrow[1] = const_cast<char *>("QWERTYUIOP");
                textrow[2] = const_cast<char *>("ASDFGHJKL_");
                textrow[3] = const_cast<char *>("ZXCVBNM<>?");
            }
        }
        return 0;
    }

    static __used__ void cbMMEditorConfig(eduimenu_s *menu, eduiitem_s *, u32) {
        ed_cfg_menu->x = menu->x + 10;
        ed_cfg_menu->y = menu->y + 10;
        eduiMenuAttach(menu, ed_cfg_menu);
    }

    eduimenu_s *eduiGetActiveMenu(void) {
        return active_menu;
    }

    void eduiMenuRender(eduimenu_s *menu) {
        const auto font_set = [](void *font) { eduiDrawFontSet(font); };
        const auto font_colour = [](void *font, u32 colour) { eduiSetFontColour(font, colour); };
        const auto rect = [](i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material) {
            eduiDrawRect(x, y, width, height, colour, material);
        };
        const auto line = [](i32 x0, i32 y0, i32 x1, i32 y1, i32 colour, numtl_s *material) {
            eduiDrawLine(x0, y0, x1, y1, colour, material);
        };
        const auto outline_rect = [](i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material) {
            eduiDrawLineRect(x, y, width, height, colour, material);
        };
        const auto interact = [](i32 x, i32 y, i32 width, i32 height, eduimenu_s *menu, eduiitem_s *item,
                                 i32 (*callback)(edui_interact_s *), u32 buttons, i32 field_20) {
            if (__builtin_expect(numInteracts < 64 && !eduiInteractLocked, 0)) {
                edui_interact_s *entry = &eduiInteracts[numInteracts];
                entry->x = static_cast<f32>(x);
                entry->menu = menu;
                entry->item = item;
                entry->callback = callback;
                entry->buttons = buttons;
                entry->y = static_cast<f32>(y) * 0.5f;
                entry->width = static_cast<f32>(width);
                entry->height = static_cast<f32>(height) * 0.5f;
                entry->field_20 = field_20;
                ++numInteracts;
            }
        };
        ++NuPrimCSPos;
        i32 x = menu->x;
        i32 y = menu->y;
        NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_PS2);
        NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
        NuQFntPushPrintMode(2);
        font_set(edui_font);
        NuQFntSetScale(edui_font, edui_font_scale_x, edui_font_scale_y);

        const f32 font_height = NuQFntHeight(edui_font);
        const f32 title_font_height = NuQFntHeight(edui_font);
        const f32 baseline = NuQFntBaseline(edui_font);
        if (menu->title && menu->title[0]) {
            const i32 title_height = static_cast<i32>(font_height * 0.15625f);
            if (!menu->child)
                interact(x, y, menu->width, title_height, menu, NULL, cbInteractMenuTitle, 0x40, 0);
            rect(x << 4, y << 3, menu->width << 4, title_height << 3, 0x80000000, uimtls[ui_bgmtl]);
            font_set(edui_font);
            font_colour(edui_font, 0x807f7f7f);
            eduiFntPrintClipEx(edui_font, (x + 1) << 4,
                               static_cast<i32>(title_font_height * 0.125f + baseline) + (y << 3), 0x10, x << 4,
                               menu->width << 4, menu->title);
            y += title_height;
        }

        eduiitem_s *item = menu->field_0c;
        i32 max_item_height = 0;
        if (item) {
            i32 find_selected = menu->selected != NULL;

            if (item != menu->first) {
                const i32 scroll_height = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f);
                if (!menu->child)
                    interact(x, y, menu->width, scroll_height, menu, NULL, cbInteractMenuScrollUp, 0x40, 0);
                rect(x << 4, y << 3, menu->width << 4, scroll_height << 3, 0x80000000, uimtls[ui_bgmtl]);
                const i32 top = (y << 3) + 8;
                const i32 bottom = ((y + scroll_height) << 3) - 16;
                i32 centre = (x * 2 + menu->width) << 3;
                line(centre, top, centre - ((scroll_height - 2) << 4), bottom, 0x80ffffff, uimtls[0]);
                centre = (x * 2 + menu->width) << 3;
                line(centre, top, centre + ((scroll_height - 2) << 4), bottom, 0x80ffffff, uimtls[0]);
                y += scroll_height;
            }

            while (item) {
                menu->field_10 = item;
                i32 is_selected;
                if (__builtin_expect(item != menu->selected, 0)) {
                    is_selected = 0;
                } else {
                    find_selected = 0;
                    if (menu->child)
                        is_selected = 0;
                    else
                        is_selected = 1;
                }

                const i32 item_height = eduiItemRender(item, menu, x, y, menu->width, is_selected);
                if (item_height > max_item_height)
                    max_item_height = item_height;

                if (!menu->child)
                    interact(x, y, menu->width, item_height, menu, item, reinterpret_cast<edui_sel_s *>(item)->interact,
                             0x40, 0);

                item = item->next;
                if (!(menu->flags & 2))
                    y += item_height;
                else
                    x += menu->width;

                if (!item)
                    break;
                if (!(menu->y + menu->height <= y)) {
                    continue;
                }
                const i32 scroll_height = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f);
                if (__builtin_expect(!menu->child, 1))
                    interact(x, y, menu->width, scroll_height, menu, NULL, cbInteractMenuScrollDown, 0x40, 0);
                rect(x << 4, y << 3, menu->width << 4, scroll_height << 3, 0x80000000, uimtls[ui_bgmtl]);
                const i32 bottom = ((y + scroll_height - 2) << 3);
                i32 centre = (x * 2 + menu->width) << 3;
                const i32 top = (y << 3) + 8;
                line(centre, bottom, centre - ((scroll_height - 2) << 4), top, 0x80ffffff, uimtls[0]);
                centre = (x * 2 + menu->width) << 3;
                line(centre, bottom, centre + ((scroll_height - 2) << 4), top, 0x80ffffff, uimtls[0]);
                break;
            }

            if (find_selected && menu->field_0c->next)
                menu->field_0c = menu->field_0c->next;
        }

        if (!(menu->flags & 2))
            x = menu->x + menu->width;
        else
            y += max_item_height;
        menu->field_24 = x - menu->x;
        menu->field_28 = y - menu->y;

        const u32 outline = eduiGetActiveMenu() == menu ? eduimenu_selected_outline_colour : eduimenu_outline_colour;
        outline_rect((menu->x - 1) << 4, (menu->y << 3) - 8, (menu->field_24 + 2) << 4, (menu->field_28 << 3) + 16,
                     outline, uimtls[ui_outmtl]);

        if (menu->child) {
            interact(menu->x, menu->y, menu->field_24, menu->field_28, menu, NULL, cbInteractMenuCancelChild, 0x40, 1);
            eduiMenuRender(menu->child);
        }

        NuQFntPopPrintMode();
        NuQFntPopCoordinateSystem();
        --NuPrimCSPos;
        NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[NuPrimCSPos]);
    }

    void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding) {
        if (menu->field_24 < 0) {
            i32 saved = edui_donotdraw;
            edui_donotdraw = 1;
            eduiMenuRender(menu);
            edui_donotdraw = saved;
        }
        if (menu->x + menu->field_24 > 640 - padding)
            menu->x -= menu->field_24;
        if (menu->x < padding)
            menu->x = padding;
        if (menu->y + menu->field_28 > 448 - padding)
            menu->y -= menu->field_28;
        if (menu->y < padding)
            menu->y = padding;
    }

    static __used__ i32 eduicbRenderProp(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        char prefix[256];
        edui_prop_s *property = static_cast<edui_prop_s *>(item);
        f32 draw_x = x;
        f32 draw_y = y;
        f32 draw_width = width;
        u32 divider_colours[2] = {0x0ff00000, 0xffffffff};
        u32 button_colours[2] = {0xff000000, 0xffffffff};
        item->x = x;
        item->y = y;
        u8 *flags = reinterpret_cast<u8 *>(property) + 0x4c;
        u32 highlighted;
        if (edui_cursor_x >= property->button_x && edui_cursor_y >= property->button_y * 0.5f &&
            edui_cursor_x < property->button_x + property->button_size &&
            edui_cursor_y < (property->button_y + property->button_size) * 0.5f && !eduiInteractLocked)
            highlighted = 1;
        else
            highlighted = (*flags >> 3) & 1;
        *flags = (*flags & ~16u) | (highlighted << 4);
        if (__builtin_expect(!edui_donotdraw, 1))
            NuQFntSet(edui_font);
        if (__builtin_expect(item->unknown_10 == 2, 0)) {
            if (!edui_donotdraw)
                NuQFntSetColour(edui_font, 0xff006000);
        } else if (__builtin_expect(item->unknown_10 == 3, 0)) {
            if (!edui_donotdraw)
                NuQFntSetColour(edui_font, 0xff600000);
        } else if (item->unknown_10 == 1) {
            if (__builtin_expect(!edui_donotdraw, 0))
                NuQFntSetColour(edui_font, 0xff000060);
        } else if (!edui_donotdraw) {
            NuQFntSetColour(edui_font, item->colours[item->highlighted]);
        }
        f32 height = NuQFntHeight(edui_font) * 1.25f * 0.125f;
        f32 baseline = (NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font)) * 0.125f;
        f32 indent_width = height - 2.0f;
        f32 indentation = property->depth * indent_width;
        if (property->unknown_property_flags & 1) {
            f32 text_width = NuQFntPrintLenU(edui_font, eduiPropTextEdit) * 0.0625f;
            f32 edit_width = draw_width - property->label_width - 4.0f;
            edit_width = MAX(edit_width, text_width);
            if (!edui_donotdraw)
                NuRndrRect2di(static_cast<i32>(draw_x * 16.0f), static_cast<i32>(draw_y * 8.0f),
                              static_cast<i32>(draw_width * 16.0f), static_cast<i32>(height * 8.0f),
                              item->colours[2 + item->highlighted], uimtls[0]);
            if (!edui_donotdraw)
                NuRndrRect2di(static_cast<i32>((draw_x + property->label_width + 2.0f) * 16.0f),
                              static_cast<i32>((draw_y + 2.0f) * 8.0f), static_cast<i32>(edit_width * 16.0f),
                              static_cast<i32>((height - 4.0f) * 8.0f), 0xffffffff, uimtls[0]);
            for (i32 depth = 0; depth < property->depth; ++depth) {
                f32 guide_x = draw_x + (depth + 0.5f) * indent_width;
                if (__builtin_expect(!edui_donotdraw, 1))
                    NuRndrLine2di(static_cast<i32>(guide_x * 16.0f), static_cast<i32>(draw_y * 8.0f),
                                  static_cast<i32>(guide_x * 16.0f), static_cast<i32>((draw_y + height) * 8.0f),
                                  0xff000000, uimtls[0]);
            }
            u32 divider_colour = divider_colours[(property->unknown_property_flags >> 2) & 1];
            if (!edui_donotdraw)
                NuRndrLine2di(static_cast<i32>((draw_x + property->label_width) * 16.0f),
                              static_cast<i32>(draw_y * 8.0f),
                              static_cast<i32>((draw_x + property->label_width) * 16.0f),
                              static_cast<i32>((draw_y + height) * 8.0f), divider_colour, uimtls[0]);
            eduiFntPrintClipEx(edui_font, draw_x + indentation, draw_y + baseline, 16, draw_x - indentation,
                               property->label_width, item->text);
            eduiFntPrintExEditBuffer(edui_font, draw_x + property->label_width + 2.0f, draw_y + baseline, 16, eduiPropTextEdit);

            NuStrNCpy(prefix, eduiPropTextEdit, eduiPropTextPos + 1);
            prefix[eduiPropTextPos + 1] = '\0';
            f32 caret_width = NuQFntPrintLenU(edui_font, prefix) * 0.0625f;
            f32 caret_x = draw_x + property->label_width + 2.0f + caret_width;
            if (!edui_donotdraw)
                NuRndrLine2di(static_cast<i32>(caret_x * 16.0f), static_cast<i32>(draw_y * 8.0f),
                              static_cast<i32>(caret_x * 16.0f), static_cast<i32>((draw_y + height) * 8.0f), 0xff000000,
                              uimtls[0]);
        } else {
            property->button_size = indent_width;
            property->button_y = draw_y + 1.0f;
            property->button_x = draw_x + draw_width - (1.0f + indent_width);
            for (eduimenu_s *descendant = menu; descendant; descendant = descendant->child) {
                if (descendant == eduiGetActiveMenu()) {
                    if (!edui_donotdraw)
                        NuRndrRect2di(static_cast<i32>(draw_x * 16.0f), static_cast<i32>(draw_y * 8.0f),
                                      static_cast<i32>(draw_width * 16.0f), static_cast<i32>(height * 8.0f),
                                      item->colours[2 + item->highlighted], uimtls[ui_bgmtl]);
                    break;
                }
            }
            u32 divider_colour = divider_colours[(property->unknown_property_flags >> 2) & 1];
            if (__builtin_expect(!edui_donotdraw, 1))
                NuRndrLine2di(static_cast<i32>((draw_x + property->label_width) * 16.0f),
                              static_cast<i32>(draw_y * 8.0f),
                              static_cast<i32>((draw_x + property->label_width) * 16.0f),
                              static_cast<i32>((draw_y + height) * 8.0f), divider_colour, uimtls[0]);
            for (i32 depth = 0; depth < property->depth; ++depth) {
                f32 guide_x = draw_x + (depth + 0.5f) * indent_width;
                if (!edui_donotdraw)
                    NuRndrLine2di(static_cast<i32>(guide_x * 16.0f), static_cast<i32>(draw_y * 8.0f),
                                  static_cast<i32>(guide_x * 16.0f), static_cast<i32>((draw_y + height) * 8.0f),
                                  0xff000000, uimtls[0]);
            }
            eduiFntPrintClipEx(edui_font, draw_x + indentation, draw_y + baseline, 16, draw_x - indentation,
                               property->label_width, item->text);
            f32 value_x = property->button_x - 1.0f;
            f32 clip_x = draw_x + property->label_width;
            eduiFntPrintClipEx(edui_font, value_x, draw_y + baseline, 32, clip_x, value_x - clip_x,
                               property->property_text);
            if (property->button_type == 3) {
                u32 fill_colour;
                memcpy(&fill_colour, property->unknown_4e + 2, sizeof(fill_colour));
                if (!edui_donotdraw)
                    NuRndrRect2di(static_cast<i32>(property->button_x * 16.0f),
                                  static_cast<i32>(property->button_y * 8.0f),
                                  static_cast<i32>(property->button_size * 16.0f),
                                  static_cast<i32>(property->button_size * 8.0f), fill_colour, uimtls[0]);
            }
            eduiDrawLineRect(static_cast<i32>(property->button_x * 16.0f), static_cast<i32>(property->button_y * 8.0f),
                             static_cast<i32>(property->button_size * 16.0f),
                             static_cast<i32>(property->button_size * 8.0f), 0xff000000, uimtls[ui_outmtl]);
            i32 points[6];
            if (__builtin_expect(property->button_type == 1, 0)) {
                points[0] = static_cast<i32>((property->button_x + property->button_size * 0.1f) * 16.0f);
                points[1] = static_cast<i32>((property->button_y + property->button_size * 0.1f) * 8.0f);
                points[2] = static_cast<i32>((property->button_x + property->button_size * 0.9f) * 16.0f);
                points[3] = static_cast<i32>((property->button_y + property->button_size * 0.5f) * 8.0f);
                points[4] = static_cast<i32>((property->button_x + property->button_size * 0.1f) * 16.0f);
                points[5] = static_cast<i32>((property->button_y + property->button_size * 0.9f) * 8.0f);
                u32 colour = button_colours[(property->unknown_property_flags >> 4) & 1];
                if (!edui_donotdraw)
                    NuRndrTriStrip2di(points, NULL, 3, colour, uimtls[0]);
            }
            if (property->button_type == 2) {
                points[0] = static_cast<i32>((property->button_x + property->button_size * 0.1f) * 16.0f);
                points[1] = static_cast<i32>((property->button_y + property->button_size * 0.45f) * 8.0f);
                points[2] = static_cast<i32>((property->button_x + property->button_size * 0.5f) * 16.0f);
                points[3] = static_cast<i32>((property->button_y + property->button_size * 0.1f) * 8.0f);
                points[4] = static_cast<i32>((property->button_x + property->button_size * 0.9f) * 16.0f);
                points[5] = static_cast<i32>((property->button_y + property->button_size * 0.45f) * 8.0f);
                u32 colour = button_colours[(property->unknown_property_flags >> 4) & 1];
                if (__builtin_expect(!edui_donotdraw, 0))
                    NuRndrTriStrip2di(points, NULL, 3, colour, uimtls[0]);
                points[0] = static_cast<i32>((property->button_x + property->button_size * 0.1f) * 16.0f);
                points[1] = static_cast<i32>((property->button_y + property->button_size * 0.55f) * 8.0f);
                points[2] = static_cast<i32>((property->button_x + property->button_size * 0.5f) * 16.0f);
                points[3] = static_cast<i32>((property->button_y + property->button_size * 0.9f) * 8.0f);
                points[4] = static_cast<i32>((property->button_x + property->button_size * 0.9f) * 16.0f);
                points[5] = static_cast<i32>((property->button_y + property->button_size * 0.55f) * 8.0f);
                colour = button_colours[(property->unknown_property_flags >> 4) & 1];
                if (!edui_donotdraw)
                    NuRndrTriStrip2di(points, NULL, 3, colour, uimtls[0]);
            }
        }
        return static_cast<i32>(height);
    }

    static __used__ i32 eduicbRenderFilter(struct eduimenu_s *menu, struct eduiitem_s *item, i32 x, i32 y, i32 scale,
                                           i32 selected) {
        return eduicbRenderProp(menu, item, x, y, scale, selected);
    }

    i32 eduiMenuDetach(eduimenu_s *menu) {
        if (menu->parent) {
            if (menu->child)
                eduiMenuDetach(menu->child);
            if (eduiGetActiveMenu() == menu)
                active_menu = menu->parent;
            menu->parent->child = NULL;
            menu->parent = NULL;
            return 1;
        }
        eduiSetActiveMenu(NULL);
        return 0;
    }

}

static __used__ void cbColourPickSel(eduimenu_s *menu, eduiitem_s *, u32) {
    *cp_r = cp_item->red;
    *cp_g = cp_item->green;
    *cp_b = cp_item->blue;
    if (cp_rgba != nullptr)
        *cp_rgba = cpPackColour(*cp_r, *cp_g, *cp_b, *cp_a);
    eduiMenuDetach(menu);
}

extern "C" {
    void cbCancelSubMenu(eduimenu_s *menu, eduimenu_s *) {
        eduiMenuDetach(menu);
    }

    static __used__ void cbgpcfgPaste(eduimenu_s *menu, eduiitem_s *item, u32 value) {
        edui_gradient_pick_s *picker = reinterpret_cast<edui_gradient_pick_s *>(item->data_ptr);
        edui_gradient_node_s *stage = picker->selected_stage;
        if (stage != nullptr) {
            if (item->type == 7)
                eduiGradStageSetHSV(stage, cliph, clips, clipv);
            if (item->type == 8)
                eduiGradStageSetHSV(stage, 0.0f, 0.0f, clipg);
            if (picker->paste != nullptr)
                picker->paste(menu, item, value);
        }
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    static __used__ void cbgpcfgCopy(eduimenu_s *menu, eduiitem_s *item, u32 value) {
        edui_gradient_pick_s *picker = reinterpret_cast<edui_gradient_pick_s *>(item->data_ptr);
        edui_gradient_node_s *stage = picker->selected_stage;
        if (stage != nullptr) {
            if (item->type == 7) {
                cliph = stage->hue;
                clips = stage->saturation;
                clipv = stage->value;
            } else if (item->type == 8) {
                clipg = stage->value;
            }
            if (picker->copy != nullptr)
                picker->copy(menu, item, value);
        }
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    static __used__ void cbgpcfgDel(eduimenu_s *menu, eduiitem_s *item, u32 value) {
        edui_gradient_pick_s *picker = reinterpret_cast<edui_gradient_pick_s *>(item->data_ptr);
        if (picker->remove != nullptr)
            picker->remove(menu, item, value);
        if (picker->selected_stage != nullptr)
            eduiGradStageDelete(picker, picker->selected_stage);
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    static __used__ void cbgpcfgAdd(eduimenu_s *menu, eduiitem_s *item, u32 value) {
        edui_gradient_pick_s *picker = reinterpret_cast<edui_gradient_pick_s *>(item->data_ptr);
        edui_gradient_node_s *stage = picker->selected_stage;
        f32 time = 0.0f;
        f32 hue = 0.0f;
        f32 saturation = 0.0f;
        f32 intensity = 1.0f;
        if (stage != nullptr) {
            time = ((stage->next != nullptr ? stage->next->time : 1.0f) + stage->time) * 0.5f;
            hue = stage->hue;
            saturation = stage->saturation;
            intensity = stage->value;
        }
        picker->selected_stage = eduiGradStageAdd(picker, time, hue, saturation, intensity);
        if (picker->add != nullptr)
            picker->add(menu, item, value);
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    static __used__ void cbgpcfgCPPress(eduimenu_s *menu, eduiitem_s *item, u32 value) {
        edui_gradient_pick_s *picker = reinterpret_cast<edui_gradient_pick_s *>(item->data_ptr);
        if (picker->selected_stage != nullptr && (item->type == 10 || item->type == 11)) {
            edui_colour_pick_s *colour = reinterpret_cast<edui_colour_pick_s *>(item);
            eduiGradStageSetHSV(picker->selected_stage, colour->hue, colour->saturation, colour->value);
        }
        if (picker->press != nullptr)
            picker->press(menu, item, value);
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    static __used__ void eduicbSelectDirectoryExit(eduimenu_s *menu, eduiitem_s *, u32) {
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    static __used__ void eduicbSelectDirectoryEntry(eduimenu_s *menu, eduiitem_s *item, u32 value) {
        if (!dir_list)
            return;
        if (item->data == 9999) {
            char *separator = NuStrRChr(filepick_item->filename, '/');
            if (separator)
                *separator = '\0';
            else
                filepick_item->filename[0] = '\0';
            filepick_item->reopen_directory = 1;
        }
        if (static_cast<FilePickDirectoryEntry *>(dir_list)[item->data].flags & 8) {
            if (filepick_item->filename[0])
                NuStrCat(filepick_item->filename, const_cast<char *>("/"));
            NuStrCat(filepick_item->filename, static_cast<FilePickDirectoryEntry *>(dir_list)[item->data].name);
            filepick_item->reopen_directory = 1;
        }
        if (!(static_cast<FilePickDirectoryEntry *>(dir_list)[item->data].flags & 8)) {
            NuStrCpy(filepick_item->name, static_cast<FilePickDirectoryEntry *>(dir_list)[item->data].name);
            if (filepick_item->changed)
                filepick_item->changed(filepick_menu, filepick_item, value);
        }
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    static __used__ void eduicbDestroyDirectoryList(eduimenu_s *menu, eduimenu_s *) {
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    __attribute__((visibility("hidden"), optimize("O3"))) void tpentPropOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                                unsigned int buttons) {
        void *original = *(void **)((char *)item + 0x0c);
        void (*callback)(void *, void *, unsigned int) =
            *(void (**)(void *, void *, unsigned int))((char *)original + 0x78);
        eduimenu_s *parent = *(eduimenu_s **)((char *)menu + 0x40);

        NuStrCpy(*(char **)((char *)original + 0x64), (char *)item + 0x4c);
        if (callback)
            callback(parent, original, buttons);
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
        if (parent)
            eduiSetActiveMenu(parent);
    }

    __attribute__((visibility("hidden"), optimize("O3"))) void tpentOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                            unsigned int buttons) {
        void *original = *(void **)((char *)item + 0x0c);
        void (*callback)(void *, void *, unsigned int) =
            *(void (**)(void *, void *, unsigned int))((char *)original + 0x15c);
        void *parent = *(void **)((char *)menu + 0x40);

        NuStrCpy((char *)original + 0x4c, (char *)item + 0x4c);
        if (callback)
            callback(parent, original, buttons);
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    static __used__ void cbmcfgCancel(eduimenu_s *menu, eduimenu_s *) {
        eduiMenuDetach(menu);
    }

    static __used__ i32 eduicbRenderExpander(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        edui_expander_s *expander = static_cast<edui_expander_s *>(item);
        u32 button_colours[2] = {0xff000000, 0xffffffff};
        const f32 item_x = static_cast<f32>(x);
        const f32 item_y = static_cast<f32>(y);
        const f32 item_width = static_cast<f32>(width);
        item->x = x;
        item->y = y;
        const bool over_button = edui_cursor_x >= expander->button_x && edui_cursor_y >= expander->button_y * 0.5f &&
                                 edui_cursor_x < expander->button_x + expander->button_size &&
                                 edui_cursor_y < (expander->button_y + expander->button_size) * 0.5f;
        // The original updates only the byte containing open and hover at offset 0x54.
        u8 *hover_flags = reinterpret_cast<u8 *>(expander) + 0x54;
        *hover_flags = (*hover_flags & ~2u) | static_cast<u8>(over_button) << 1;

        {
            eduiDrawFontSet(edui_font);
            eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        }
        const f32 row_height = NuQFntHeight(edui_font) * 1.25f * 0.125f;
        const f32 baseline = (NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font)) * 0.125f;
        for (eduimenu_s *submenu = menu; submenu; submenu = submenu->child) {
            if (submenu == eduiGetActiveMenu()) {
                eduiDrawRect(static_cast<i32>(item_x * 16.0f), static_cast<i32>(item_y * 8.0f),
                             static_cast<i32>(item_width * 16.0f), static_cast<i32>(row_height * 8.0f),
                             item->colours[2 + item->highlighted], uimtls[ui_bgmtl]);
                break;
            }
        }
        const f32 button_size = row_height - 2.0f;
        for (i32 depth = 0; depth < expander->depth; ++depth) {
            if (__builtin_expect(!edui_donotdraw, 1)) {
                const i32 guide_x = static_cast<i32>((item_x + (static_cast<f32>(depth) + 0.5f) * button_size) * 16.0f);
                NuRndrLine2di(guide_x, static_cast<i32>(item_y * 8.0f), guide_x,
                              static_cast<i32>((item_y + row_height) * 8.0f), 0xff000000, uimtls[0]);
            }
        }
        expander->button_size = button_size;
        expander->button_x = item_x + static_cast<f32>(expander->depth) * button_size + 1.0f;
        expander->button_y = item_y + 1.0f;
        if (expander->first_child) {
            eduiDrawLineRect(static_cast<i32>(expander->button_x * 16.0f), static_cast<i32>(expander->button_y * 8.0f),
                             static_cast<i32>(expander->button_size * 16.0f),
                             static_cast<i32>(expander->button_size * 8.0f), 0xff000000, uimtls[ui_outmtl]);
            const u8 button_flags = *hover_flags;
            eduiDrawLine(static_cast<i32>((expander->button_x + 2.0f) * 16.0f),
                         static_cast<i32>((expander->button_y + expander->button_size * 0.5f) * 8.0f),
                         static_cast<i32>((expander->button_x + expander->button_size - 2.0f) * 16.0f),
                         static_cast<i32>((expander->button_y + expander->button_size * 0.5f) * 8.0f),
                         button_colours[(button_flags >> 1) & 1], uimtls[0]);
            if ((button_flags & 1) == 0 && !edui_donotdraw)
                NuRndrLine2di(static_cast<i32>((expander->button_x + expander->button_size * 0.5f) * 16.0f),
                              static_cast<i32>((expander->button_y + 2.0f) * 8.0f),
                              static_cast<i32>((expander->button_x + expander->button_size * 0.5f) * 16.0f),
                              static_cast<i32>((expander->button_y + expander->button_size - 2.0f) * 8.0f),
                              button_colours[(button_flags >> 1) & 1], uimtls[0]);
        }
        const f32 text_x = expander->button_x + expander->button_size + 2.0f;
        eduiFntPrintClipEx(edui_font, text_x, item_y + baseline, 16, text_x,
                           item_width - (expander->button_size - 2.0f), item->text);
        return static_cast<i32>(row_height);
    }

    eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu) {
        if (menu == NULL) {
            return NULL;
        }
        if (menu->parent == NULL) {
            return menu;
        }
        do {
            menu = menu->parent;
        } while (menu->parent != NULL);
        return menu;
    }

    eduimenu_s *eduiGetActiveMenuParent(void) {
        return eduiGetTopLevelParent(active_menu);
    }

    i32 eduiClearActiveMenu(void) {
        eduiSetActiveMenu(NULL);
        return 0;
    }

    i32 eduiMenuIsActive(eduimenu_s *menu) {
        if (__builtin_expect(!eduiGetUsingMenuFocus(), 1))
            return 1;
        if (eduiGetUsingMenuFocus() && active_menu == menu)
            return 1;
        return 0;
    }

    i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad) {
        if (menu && menu->selected && !(menu->selected->flags & EDUI_ITEM_DISABLED) && menu->selected->process)
            return menu->selected->process(menu, menu->selected, delta_time, pad);
        return 0;
    }

    void eduiMenuEnsureSelection(eduimenu_s *menu) {
        if (!menu->selected)
            menu->selected = menu->first;
    }

    i32 eduiCursorOverMenu(eduimenu_s *menu) {
        return edui_cursor_x >= menu->x && edui_cursor_y >= menu->y && edui_cursor_x < menu->x + menu->width &&
               edui_cursor_y < menu->y + menu->height;
    }

    i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad) {
        if (__builtin_expect(!pad || !(pad->digital_buttons_pressed & 0x10), 1))
            return 0;

        eduimenu_s *parent = menu->parent;
        if (!eduiGetUsingMenuFocus() && parent)
            eduiMenuDetach(menu);
        if (menu->callback)
            menu->callback(menu, parent);
        return 1;
    }

    void cbInteractMenuScrollTo(eduimenu_s *menu, char *text) {
        if (!text || !menu || !text[0])
            return;
        i32 comparison = menu->first && menu->first->text ? NuStrICmp(menu->first->text, text) : -1;
        menu->field_0c = menu->first;
        menu->selected = menu->first;
        while (menu->selected->next && comparison < 0) {
            menu->selected = menu->selected->next;
            if (menu->selected->text) {
                comparison = NuStrICmp(menu->selected->text, text);
            } else {
                comparison = -1;
            }
        }
        menu->field_10 = menu->selected;
    }

    void cbInteractMenuKeySelect(eduimenu_s *menu) {
        u32 modifiers;
        char text[16];
        i32 key = NuKeyGet(&modifiers);
        if (key >= 0) {
            text[0] = NuKeyToAscii(key, modifiers & 1);
            text[1] = 0;
            cbInteractMenuScrollTo(menu, text);
        }
    }

    i32 eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result) {
        static f32 up_rept;
        static f32 down_rept;
        cbInteractMenuKeySelect(menu);
        if (item_result || !pad)
            return 0;
        if (menu && menu->selected && menu->selected->input && !menu->selected->disabled && pad->digital_buttons) {
            if (menu->selected->input(menu, menu->selected, pad->digital_buttons, pad->digital_buttons_pressed))
                return 0;
        }
        u32 buttons = pad->digital_buttons;
        u32 pressed = pad->digital_buttons_pressed;
        u32 up_button = (menu->flags & 2) ? 0x8000 : 0x1000;
        u32 down_button = (menu->flags & 2) ? 0x2000 : 0x4000;
        i32 up = 0;
        if (pressed & up_button) {
            up_rept = 1.0f / 3.0f;
            up = 1;
        }
        if (__builtin_expect((buttons & up_button) && up_rept > 0.0f, 1)) {
            up_rept -= delta_time;
            if (up_rept <= 0.0f) {
                up_rept = 1.0f / 6.0f;
                up = 1;
            }
        }
        if (!(menu->flags & 6)) {
            if ((buttons & 0x8000) && !(menu->flags & 1)) {
                i32 distance = static_cast<i32>(delta_time * 1500.0f);
                if (distance < 1)
                    distance = 1;
                menu->x -= distance;
            }
            if ((buttons & 0x2000) && !(menu->flags & 1)) {
                i32 distance = static_cast<i32>(delta_time * 1500.0f);
                if (distance < 1)
                    distance = 1;
                menu->x += distance;
            }
        }
        i32 down = 0;
        if (pressed & down_button) {
            down_rept = 1.0f / 3.0f;
            down = 1;
        }
        if ((buttons & down_button) && down_rept > 0.0f) {
            down_rept -= delta_time;
            if (down_rept <= 0.0f) {
                down_rept = 1.0f / 6.0f;
                down = 1;
            }
        }
        i32 to_first = up && (buttons & 1);
        i32 page_up = up && (buttons & 2);
        i32 page_down = down && (buttons & 2);
        i32 to_last = down && (buttons & 1);
        if (page_down && menu->field_10) {
            i32 count = 1;
            for (eduiitem_s *item = menu->field_0c; item && item != menu->field_10; item = item->next)
                ++count;
            while (menu->selected->next && count--) {
                menu->selected = menu->selected->next;
            }
            return 0;
        }
        if (page_up && menu->field_10) {
            i32 count = 1;
            for (eduiitem_s *item = menu->field_0c; item && item != menu->field_10; item = item->next)
                ++count;
            menu->selected = menu->field_0c;
            while (menu->selected->previous && count--) {
                menu->selected = menu->selected->previous;
                menu->field_0c = menu->selected;
            }
            return 0;
        }
        if (up && menu->selected && menu->selected->previous) {
            do {
                if (menu->selected == menu->field_0c)
                    menu->field_0c = menu->selected->previous;
                eduiitem_s *item = menu->selected->previous;
                while (item->type == 18)
                    item = item->previous;
                menu->selected = item;
            } while (menu->selected->previous && to_first);
            return 0;
        }
        if (down && menu->selected && menu->selected->next) {
            do {
                eduiitem_s *item = menu->selected->next;
                while (item->type == 18)
                    item = item->next;
                menu->selected = item;
            } while (menu->selected->next && to_last);
            if (__builtin_expect(!!(to_last), 0)) {
                i32 count = 1;
                for (eduiitem_s *item = menu->field_0c; item && item != menu->field_10; item = item->next)
                    ++count;
                menu->field_0c = menu->selected;
                while (menu->field_0c->previous && count--)
                    menu->field_0c = menu->field_0c->previous;
            }
            return 0;
        }
        eduiCheckForPadMenuCancel(menu, pad);
        return 0;
    }

    void eduiFlushInteracts(void) {
        numInteracts = 0;
    }

    i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad) {
        i32 result = 0;
        if (__builtin_expect(!!(edmainGetCursorEnabled()), 0)) {
            if (eduiInteractLocked) {
                if (__builtin_expect((!eduiInteractLocked->menu || !eduiInteractLocked->item) && eduiInteractLocked->field_20, 0))
                    eduiInteractLocked = NULL;
            }
            if (eduiInteractLocked) {
                result = 1;
                if (__builtin_expect(menu == eduiInteractLocked->menu || menu->parent == eduiInteractLocked->menu, 0)) {
                    eduiInteractLocked = eduiInteractLocked->callback(eduiInteractLocked) ? eduiInteractLocked : NULL;
                    result = eduiInteractLocked != NULL;
                }
            } else {
                edui_interact_s *interact = eduiInteracts + numInteracts - 1;
                for (i32 i = 0; i < numInteracts; ++i, --interact) {
                    if (interact->menu == menu && (interact->buttons & edui_cursor_buttons_db) &&
                        edui_cursor_x >= interact->x && edui_cursor_y >= interact->y &&
                        edui_cursor_x < interact->x + interact->width &&
                        edui_cursor_y < interact->y + interact->height) {
                        if (menu && interact->item && interact->item->type != 0x12)
                            menu->selected = interact->item;
                        if (interact->field_20 || (interact->buttons & edui_cursor_buttons)) {
                            if (!interact->field_20 && eduiGetActiveMenu() != menu)
                                eduiSetActiveMenu(menu);
                            if (interact->callback && interact->callback(interact))
                                eduiInteractLocked = interact;
                        }
                        result = 1;
                        break;
                    }
                }
            }
        }
        return result;
    }

    i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad) {
        if (eduiMenuIsActive(menu)) {
            eduiMenuEnsureSelection(menu);
            i32 result = eduiMenuProcessSelectedItem(menu, delta_time, pad);
            if (eduiMenuIsActive(menu) && processing_menu == menu) {
                eduiMenuEnsureSelection(menu);
                if (eduiMenuIsActive(menu) && processing_menu == menu)
                    eduiMenuProcessInput(menu, delta_time, pad, result);
            }
        }
        return eduiProcessInteracts(menu, pad);
    }

    i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad) {
        processing_menu = menu;
        if (menu->child) {
            i32 result = eduiMenuProcess(menu->child, delta_time, pad);
            if (processing_menu) {
                if (result)
                    eduiSetActiveMenu(menu->child ? menu->child : menu);
                else
                    result = eduiProcessInteracts(menu, pad);
            }
            return result;
        }
        return eduiMenuProcessAux(menu, delta_time, pad);
    }

    void eduiRenderInteracts(void) {
        if (numInteracts < 1)
            return;

        ++NuPrimCSPos;
        NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_PS2);
        NuPrim2DBegin(2, 5, uimtls[0]);

        edui_interact_s *hovered = NULL;
        edui_interact_s *interact = eduiInteractLocked ? eduiInteractLocked : eduiInteracts;
        const i32 count = eduiInteractLocked ? 1 : numInteracts;
        for (i32 i = 0; i < count; ++i, ++interact) {
            const u32 colour = NuRandInt();
#define EDUI_INTERACT_VERTEX(x, y)                                                                                     \
    NuRndrPrimSetColour(colour);                                                                                       \
    NuPrim2DAddXYZ((x), (y), 0.0f)
            EDUI_INTERACT_VERTEX(interact->x, interact->y);
            EDUI_INTERACT_VERTEX(interact->x + interact->width, interact->y);
            EDUI_INTERACT_VERTEX(interact->x + interact->width, interact->y);
            EDUI_INTERACT_VERTEX(interact->x + interact->width, interact->y + interact->height);
            EDUI_INTERACT_VERTEX(interact->x + interact->width, interact->y + interact->height);
            EDUI_INTERACT_VERTEX(interact->x, interact->y + interact->height);
            EDUI_INTERACT_VERTEX(interact->x, interact->y + interact->height);
            EDUI_INTERACT_VERTEX(interact->x, interact->y);
#undef EDUI_INTERACT_VERTEX

            if (interact->x <= edui_cursor_x && interact->y <= edui_cursor_y &&
                edui_cursor_x < interact->x + interact->width && edui_cursor_y < interact->y + interact->height)
                hovered = interact;
        }
        NuPrim2DEnd();

        if (hovered) {
            NuPrim2DBegin(4, 5, uimtls[3]);
            const u32 colour = (NuRandInt() & 0x00ffffff) | 0x40000000;
            NuRndrPrimSetColour(colour);
            NuPrim2DAddXYZ(hovered->x, hovered->y, 0.0f);
            NuRndrPrimSetColour(colour);
            NuPrim2DAddXYZ(hovered->x + hovered->width, hovered->y + hovered->height, 0.0f);
            NuPrim2DEnd();
        }

        --NuPrimCSPos;
        NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[NuPrimCSPos]);
    }

    void eduiShowCursor(i32 show) {
        bShowCursor = show;
    }

    void eduiGetCursorCoords(f32 *x, f32 *y) {
        *x = edui_cursor_x / 640.0f;
        *y = edui_cursor_y / 224.0f;
    }

    void eduiSetCursorCoords(f32 x, f32 y) {
        edui_cursor_x = x * 640.0f;
        edui_cursor_y = y * 224.0f;
    }

    void eduiGetCursorDelta(f32 *x, f32 *y) {
        *x = edui_cursor_dx / 640.0f;
        *y = edui_cursor_dy / 224.0f;
    }

    i32 eduiUsedAlgPad(nupad_s *pad) {
        if (pad) {
            if (pad->analog_left_x < 64 || pad->analog_left_x > 192 || pad->analog_left_y < 64 ||
                pad->analog_left_y > 192)
                return EDUI_ANALOG_PAD_LEFT;
            if (pad->analog_right_x < 64 || pad->analog_right_x > 192 || pad->analog_right_y < 64 ||
                pad->analog_right_y > 192)
                return EDUI_ANALOG_PAD_RIGHT;
        }
        return EDUI_ANALOG_PAD_NONE;
    }

    void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad) {
        if (pad && eduiUsedAlgPad(pad) && !(pad->digital_buttons & EDUI_CURSOR_PRIMARY)) {
            eduiSetCursorCoords(0.5f, 0.5f);
        } else {
            edui_cursor_dx = -NuMouseReadXVel() * 0.1f;
            edui_cursor_dy = -NuMouseReadYVel() * 0.1f;
        }
        edui_cursor_locked = NuKeyboard(0x38) != 0;
        if (!edui_cursor_locked) {
            edui_cursor_x += edui_cursor_dx;
            edui_cursor_y += edui_cursor_dy;
        }
        if (edui_cursor_x < 0.0f)
            edui_cursor_x = 0.0f;
        if (edui_cursor_x > 640.0f)
            edui_cursor_x = 640.0f;
        if (edui_cursor_y < 0.0f)
            edui_cursor_y = 0.0f;
        if (edui_cursor_y > 224.0f)
            edui_cursor_y = 224.0f;
        edui_cursor_buttons_old = edui_cursor_buttons;
        edui_cursor_buttons = 0;
        edui_cursor_buttons |= NuMouseReadButtons() == 1 ? EDUI_CURSOR_PRIMARY : 0;
        edui_cursor_buttons |= NuMouseReadButtons() == 2 ? EDUI_CURSOR_SECONDARY : 0;
        edui_cursor_buttons_db = edui_cursor_buttons & ~edui_cursor_buttons_old;
    }

    void eduiProcessCursor(f32 delta_time, nupad_s *pad) {
        eduiProcessCursorDefault(delta_time, pad);
    }

    i32 edmainProcess(f32 delta_time, nupad_s *pad) {
        editor_return = 0;
        if (edmain_cursor_enabled)
            eduiProcessCursor(0.0f, NULL);

        if (ed_module_active && ed_curr) {
            if (!ed_curr->process) {
                ed_module_active = 0;
                return editor_return;
            }
            i32 result = ed_curr->process(delta_time, pad);
            if (!result)
                return editor_return;
            if (ed_curr->deactivate)
                ed_curr->deactivate();
            if (result == 2)
                editor_return = 1;
            ed_module_active = 0;
            return editor_return;
        }

        if (!ed_curr) {
            ed_curr = ed_list;
            eduiSetActiveMenu(ed_main_menu);
            eduiSetDefaultActiveMenu(ed_main_menu);
        }

        if (ed_main_menu) {
            for (eduiitem_s *item = ed_main_menu->first; item; item = item->next) {
                if (item->data_ptr) {
                    ed_module_s *module = static_cast<ed_module_s *>(item->data_ptr);
                    item->disabled = (reinterpret_cast<usize>(module->reserved) & 1) != 0;
                }
            }
            eduiMenuProcess(ed_main_menu, delta_time, pad);
        }
        return editor_return;
    }

    void eduiSetCursorColour(u32 colour) {
        edui_cursor_colour = colour;
    }

    void eduiRenderCursor(void) {
        if (edui_cursor_locked)
            return;
        NuPrim2DBegin(0, 7, uimtls[0]);
#define EDUI_CURSOR_VERTEX(xoff, yoff)                                                                                 \
    NuRndrPrimSetColour(edui_cursor_colour);                                                                           \
    NuRndrPrimUV(0.0f, 0.0f);                                                                                          \
    NuPrim2DAddXYZ(edui_cursor_x + (xoff), edui_cursor_y + (yoff), 0.0f)
        EDUI_CURSOR_VERTEX(0.0f, 0.0f);
        EDUI_CURSOR_VERTEX(8.0f, 2.0f);
        EDUI_CURSOR_VERTEX(4.0f, 4.0f);
        EDUI_CURSOR_VERTEX(0.0f, 0.0f);
        EDUI_CURSOR_VERTEX(12.0f, 5.4f);
        EDUI_CURSOR_VERTEX(10.8f, 6.0f);
#undef EDUI_CURSOR_VERTEX
        NuPrim2DEnd();
    }

    void edmainRender(void) {
        if (edmain_cursor_enabled)
            eduiFlushInteracts();

        if (ed_module_active && ed_curr) {
            if (ed_curr->render)
                ed_curr->render();
        } else if (ed_main_menu) {
            NuQFntPushPrintMode(2);
            NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
            NuQFntSet(system_qfont);
            NuQFntSetScale(system_qfont, edmain_menu_scale, edmain_menu_scale);
            eduiMenuRender(ed_main_menu);
            NuQFntPopCoordinateSystem();
            NuQFntPopPrintMode();
        }

        if (edmain_cursor_enabled) {
            eduiRenderCursor();
            if (edmain_render_interacts)
                eduiRenderInteracts();
        }
    }

    void eduiInit(void) {
        eduiInitMaterials();
        if (edui_filepick_listsize && !dir_list)
            dir_list = NU_ALLOC(static_cast<usize>(edui_filepick_listsize) * 0x118, 4, 1, "", 0);
    }

    void edmainInit(void *font, char *configuration_file) {
        if (ed_init)
            edmainClose();

        ed_module_active = 0;
        ed_fnt = font;
        for (ed_module_s *module = ed_list; module; module = module->next) {
            if (module->init)
                module->init();
        }

        edinternalcam = NuCameraCreate();
        edmainExtCamera(edinternalcam);
        eduiInit();

        NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
        NuQFntSetScale(system_qfont, edmain_menu_scale, edmain_menu_scale);

        ed_cfg_menu = eduiMenuCreate(220, 50, 180, 320, ed_fnt, cbmcfgCancel, const_cast<char *>("Editor Config"));
        if (ed_cfg_menu) {
            eduiMenuAddItem(ed_cfg_menu,
                            eduiItemSliderCreate(0, &ed_attr, 0, cbmcfgCameraSpeed, 1.0f, 20.0f,
                                                 edmain_default_camera_speed, const_cast<char *>("Camera Speed")));
            eduiMenuAddItem(ed_cfg_menu, eduiItemToggleCreate(0, &ed_attr, edmain_auto_speed, 1, cbSetAutoSpeed,
                                                              const_cast<char *>("Proportional Cursor Speed")));
            eduiMenuFitWidth(ed_cfg_menu, 8);
        }

        ed_main_menu = eduiMenuCreate(ed_main_menu_x, ed_main_menu_y, ed_main_menu_w, ed_main_menu_h, ed_fnt,
                                      cbMMCancel, const_cast<char *>("Editor Options"));
        if (ed_main_menu) {
            for (ed_module_s *module = ed_list; module; module = module->next) {
                eduiitem_s *item =
                    eduiMenuAddItem(ed_main_menu, eduiItemSelCreate(reinterpret_cast<usize>(module), &ed_attr, 0, 1,
                                                                    cbMMRegSel, const_cast<char *>(module->name)));
                if (module == ed_curr)
                    eduiMenuHighlight(ed_main_menu, item);
            }
            eduiMenuAddItem(ed_main_menu, eduiItemSelCreate(0, &ed_attr, 0, 0, cbMMEditorConfig,
                                                            const_cast<char *>("Editor Config")));
            eduiMenuAddItem(ed_main_menu,
                            eduiItemSelCreate(0, &ed_attr, 0, 0, cbMMReturnToApp, const_cast<char *>("Return To App")));
            eduiMenuFitWidth(ed_main_menu, 8);
        }

        NuQFntPopCoordinateSystem();

        if (configuration_file) {
            NuStrCpy(ed_levelfile, configuration_file);
            if (ed_levelfile[0]) {
                i32 file = NuFileOpen(ed_levelfile, NUFILE_READ);
                if (file) {
                    NuFileBeginBlkRead(file, 0x3032554e);
                    i32 block;
                    while ((block = NuFileBeginBlkRead(file, 0)) != 0) {
                        for (ed_module_s *module = ed_list; module; module = module->next) {
                            if (module->block_id == static_cast<u32>(block)) {
                                if (module->read)
                                    module->read(file);
                                break;
                            }
                        }
                        NuFileEndBlkRead(file);
                    }
                    NuFileEndBlkRead(file);
                    NuFileClose(file);
                }
            }
        } else {
            ed_levelfile[0] = '\0';
        }
        ed_init = 1;
    }

    void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height) {
        ed_attr = *colours;
        ed_main_menu_x = x;
        ed_main_menu_y = y;
        ed_main_menu_w = width;
        ed_main_menu_h = height;
        edmainInit(font, configuration_file);
    }

    void eduiSetFont(NUQFNT *font) {
        edui_font = font;
    }

    void eduiSetFontScale(f32 x, f32 y) {
        edui_font_scale_x = x;
        edui_font_scale_y = y;
    }

    void CreateTestMenu(void *font) {
        u32 colours[4] = {0x80000000u, 0x80ff0000u, 0x80808080u, 0x80404040u};
        eduiInitMaterials();

        submenu1 = eduiMenuCreate(90, 90, 180, 50, font, NULL, const_cast<char *>("SubMenu1"));
        if (submenu1 != NULL) {
            eduiMenuAddItem(submenu1, eduiItemSelCreate(0, colours, 0, 1, reinterpret_cast<EdUiItemCallback>(cbSel),
                                                        const_cast<char *>("Sub Sel 0")));
            eduiMenuAddItem(submenu1, eduiItemSelCreate(1, colours, 0, 1, reinterpret_cast<EdUiItemCallback>(cbSel),
                                                        const_cast<char *>("Sub Sel 1")));
            eduiMenuAddItem(submenu1, eduiItemSelCreate(2, colours, 0, 1, reinterpret_cast<EdUiItemCallback>(cbSel),
                                                        const_cast<char *>("Sub Sel 2")));
        }

        submenu0 = eduiMenuCreate(70, 70, 180, 50, font, NULL, const_cast<char *>("SubMenu0"));
        if (submenu0 != NULL) {
            eduiMenuAddItem(submenu0, eduiItemSelCreate(0, colours, 0, 1, reinterpret_cast<EdUiItemCallback>(cbSel),
                                                        const_cast<char *>("Sub Sel")));
            eduiMenuAddItem(submenu0, eduiItemSelCreate(reinterpret_cast<usize>(submenu1), colours, 0, 0,
                                                        reinterpret_cast<EdUiItemCallback>(cbSubMenu),
                                                        const_cast<char *>("Sub Menu 1..")));
            eduiMenuAddItem(submenu0, eduiItemSelCreate(2, colours, 0, 1, reinterpret_cast<EdUiItemCallback>(cbSel),
                                                        const_cast<char *>("Sub Sel")));
        }

        testmenu0 = eduiMenuCreate(50, 50, 180, 50, font, NULL, const_cast<char *>("Main Menu"));
        if (testmenu0 != NULL) {
            eduiMenuAddItem(testmenu0, eduiItemSelCreate(reinterpret_cast<usize>(submenu0), colours, 0, 0,
                                                         reinterpret_cast<EdUiItemCallback>(cbSubMenu),
                                                         const_cast<char *>("Sub Menu 0..")));

            edui_gradient_pick_s *gradient = static_cast<edui_gradient_pick_s *>(eduiItemGradPickCreate(
                1, colours, reinterpret_cast<EdUiItemCallback>(cbGradChange), const_cast<char *>("GradPick")));
            if (gradient != NULL) {
                eduiGradStageAdd(gradient, 0.0f, 0.0f, 0.0f, 0.0f);
                eduiGradStageAdd(gradient, 1.0f, 0.0f, 0.0f, 1.0f);
            }
            eduiMenuAddItem(testmenu0, gradient);

            edui_gradient_pick_s *grey_gradient = static_cast<edui_gradient_pick_s *>(eduiItemGreyGradPickCreate(
                1, colours, reinterpret_cast<EdUiItemCallback>(cbGradChange), const_cast<char *>("GreyGradPick")));
            if (grey_gradient != NULL) {
                eduiGradStageAdd(grey_gradient, 0.0f, 0.0f, 0.0f, 0.0f);
                eduiGradStageAdd(grey_gradient, 1.0f, 0.0f, 0.0f, 1.0f);
            }
            eduiMenuAddItem(testmenu0, grey_gradient);

            eduiMenuAddItem(testmenu0, eduiItemSelCreate(2, colours, 0, 1, reinterpret_cast<EdUiItemCallback>(cbSel),
                                                         const_cast<char *>("Sel")));
        }
    }

    void TestMenu(nupad_s *pad) {
        eduiMenuProcess(testmenu0, 0.016666668f, pad);
        eduiMenuRender(testmenu0);
    }

    eduiitem_s *eduiItemColourSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           f32 minimum, f32 range, f32 value, u8 red, u8 green, u8 blue, char *text) {
        edui_colour_slider_s *item =
            static_cast<edui_colour_slider_s *>(NU_ALLOC(sizeof(edui_colour_slider_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 13;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessColourSlider;
        item->selection_group = group;
        item->render = eduicbRenderColourSlider;
        item->text_alignment = 0x40;
        item->destroy = eduicbItemSliderDestroy;
        eduiItemSetText(item, text);
        item->granularity = 1.0f;
        item->format = NULL;
        item->changed = callback;
        item->minimum = minimum;
        item->range = range;
        item->red = red;
        item->green = green;
        item->blue = blue;
        eduiItemSliderSetFmt(item, (char *)"(%d)");
        eduiItemSliderSetVal(item, value);
        return item;
    }

    void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z) {
        gp_cam.position_speed = {position_x, position_y, position_z};
    }

    void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance) {
        gp_cam.position_speed = {position_x, position_y, position_z};
        gp_cam.distance_speed = distance;
    }

    static __used__ void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *item, u32) {
        f32 divisor = 512.0f / static_cast<edui_slider_s *>(item)->value;
        f32 speed = 0.2f / divisor;
        edcamSetSpeed(speed, speed, speed, 1.0f / divisor);
    }

    void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale) {
        gp_cam.auto_move_base = move_base;
        gp_cam.auto_move_dist_scale = move_distance_scale;
        gp_cam.auto_zoom_base = zoom_base;
        gp_cam.auto_zoom_dist_scale = zoom_distance_scale;
    }

    void cbSetAutoSpeed(eduimenu_s *, eduiitem_s *item, u32) {
        edmain_auto_speed = item->highlighted;
        if (edmain_auto_speed != 0) {
            edcamSetAutoSpeed(0.15f, 0.2f, 0.01f, 0.1f);
        } else {
            edcamSetAutoSpeed(0.0f, 0.0f, 0.0f, 0.0f);
        }
    }

    void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement) {
        gp_cam.mouse_pitch_speed = pitch;
        gp_cam.mouse_yaw_speed = yaw;
        gp_cam.mouse_move_speed = movement;
    }

    void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance) {
        gp_cam.allow_position_x = position_x;
        gp_cam.allow_position_y = position_y;
        gp_cam.allow_position_z = position_z;
        gp_cam.allow_pitch = pitch;
        gp_cam.allow_yaw = yaw;
        gp_cam.allow_distance = distance;
    }

}

void do_Pad_flymode_camera(edcam_s *camera, float delta_time, nupad_s *pad) {
    constexpr f32 kNominalFrameTime = 1.0f / 60.0f;
    constexpr f32 kPitchSpeedScale = 32.0f;
    constexpr f32 kYawSpeedScale = 64.0f;
    constexpr f32 kPositionSpeedScale = 0.01f;
    constexpr i32 kPadDeadZone = 32;
    constexpr i32 kPitchLimit = 0x4000;

    const f32 frame_scale = delta_time / kNominalFrameTime;
    const f32 move_speed = camera->auto_move_dist_scale == 0.0f
                               ? 1.0f
                               : camera->auto_move_base + NuFabs(camera->distance) * camera->auto_move_dist_scale;
    const f32 zoom_speed = camera->auto_zoom_dist_scale == 0.0f
                               ? 1.0f
                               : camera->auto_zoom_base + NuFabs(camera->distance) * camera->auto_zoom_dist_scale;

    NUVEC opposite_offset;
    opposite_offset.z = 0.0f;
    opposite_offset.y = 0.0f;
    opposite_offset.x = 0.0f;
    NUMTX rotation = numtx_identity;
    NuMtxRotateX(&rotation, camera->pitch);
    NuMtxRotateY(&rotation, camera->yaw);

    opposite_offset.z = -camera->distance;
    NuVecMtxRotate(&opposite_offset, &opposite_offset, &rotation);
    NUVEC old_opposite;
    NuVecAdd(&old_opposite, &camera->position, &opposite_offset);

    const i32 pad_yaw = NuPs2ApplyDeadZone(pad->analog_right_x, kPadDeadZone);
    const i32 yaw_speed = camera->pad_yaw_speed;
    const i32 pad_pitch = NuPs2ApplyDeadZone(pad->analog_right_y, kPadDeadZone);
    i32 pitch_delta = static_cast<i32>(static_cast<f32>(pad_pitch) * static_cast<f32>(camera->pad_pitch_speed) *
                                       delta_time * kPitchSpeedScale);
    if ((camera->freedoms & EDCAM_FREEDOM_INVERT_PAD_PITCH) != 0) {
        pitch_delta = -pitch_delta;
    }
    if ((camera->freedoms & EDCAM_FREEDOM_PITCH) != 0) {
        camera->pitch += pitch_delta;
        camera->pitch = MIN(kPitchLimit, camera->pitch);
        camera->pitch = MAX(-kPitchLimit, camera->pitch);
    }
    if ((camera->freedoms & EDCAM_FREEDOM_YAW) != 0) {
        camera->yaw +=
            static_cast<i32>(static_cast<f32>(pad_yaw) * static_cast<f32>(yaw_speed) * delta_time * kYawSpeedScale);
    }

    rotation = numtx_identity;
    NuMtxRotateX(&rotation, camera->pitch);
    NuMtxRotateY(&rotation, camera->yaw);
    opposite_offset.z = -camera->distance;
    opposite_offset.y = 0.0f;
    opposite_offset.x = 0.0f;
    NuVecMtxRotate(&opposite_offset, &opposite_offset, &rotation);
    NUVEC new_opposite;
    NuVecAdd(&new_opposite, &camera->position, &opposite_offset);
    NuVecAdd(&camera->position, &camera->position, &new_opposite);
    NuVecSub(&camera->position, &camera->position, &old_opposite);

    NUVEC movement = {0.0f, 0.0f, 0.0f};
    const i32 pad_move_x = NuPs2ApplyDeadZone(pad->analog_left_x, kPadDeadZone);
    movement.x -= static_cast<f32>(pad_move_x) * delta_time * camera->distance * kPositionSpeedScale;
    const i32 pad_move_z = NuPs2ApplyDeadZone(pad->analog_left_y, kPadDeadZone);
    movement.z += static_cast<f32>(pad_move_z) * delta_time * camera->distance * kPositionSpeedScale;

    movement.y += static_cast<f32>(pad->analog_l1) * camera->position_speed.y * move_speed * frame_scale * 0.5f;
    movement.y -= static_cast<f32>(pad->analog_l2) * camera->position_speed.y * move_speed * frame_scale * 0.5f;

    if (camera->allow_distance) {
        camera->distance += static_cast<f32>(pad->analog_r1) * camera->distance_speed * zoom_speed * frame_scale;
        camera->distance -= static_cast<f32>(pad->analog_r2) * camera->distance_speed * zoom_speed * frame_scale;
        camera->distance = MIN(-camera->minimum_distance, camera->distance);
    }

    NuVecMtxRotate(&movement, &movement, &rotation);
    if ((camera->freedoms & EDCAM_FREEDOM_POSITION_X) != 0) {
        camera->position.x += movement.x;
    }
    if ((camera->freedoms & EDCAM_FREEDOM_POSITION_Y) != 0) {
        camera->position.y += movement.y;
    }
    if ((camera->freedoms & EDCAM_FREEDOM_POSITION_Z) != 0) {
        camera->position.z += movement.z;
    }
}

extern "C" {
    void do_Pad_Standard_camera(edcam_s *camera, f32 delta_time, nupad_s *pad) {
        NUVEC movement = {0.0f, 0.0f, 0.0f};
        if (!pad->is_valid)
            return;

        const f32 frame_scale = delta_time / (1.0f / 60.0f);
        const f32 move_speed = camera->auto_move_dist_scale == 0.0f
                                   ? 1.0f
                                   : NuFabs(camera->distance) * camera->auto_move_dist_scale + camera->auto_move_base;
        const f32 zoom_speed = camera->auto_zoom_dist_scale == 0.0f
                                   ? 1.0f
                                   : NuFabs(camera->distance) * camera->auto_zoom_dist_scale + camera->auto_zoom_base;

        const u8 pad_yaw_axis = pad->analog_right_x;
        const u8 pad_pitch_axis = pad->analog_right_y;
        if (camera->freedoms & EDCAM_FREEDOM_DISTANCE) {
            f32 zoom_in = static_cast<f32>(pad->analog_r1) * camera->distance_speed * zoom_speed;
            if (zoom_in > 1.0f)
                zoom_in = 1.0f;
            camera->distance += zoom_in * frame_scale;
            f32 zoom_out = static_cast<f32>(pad->analog_r2) * camera->distance_speed * zoom_speed;
            if (zoom_out > 1.0f)
                zoom_out = 1.0f;
            camera->distance -= zoom_out * frame_scale;
            camera->distance = MIN(-camera->minimum_distance, camera->distance);
        }
        if (camera->freedoms & EDCAM_FREEDOM_PITCH) {
            const i32 pitch_input = NuPs2ApplyDeadZone(pad_pitch_axis, 32);
            camera->pitch -=
                static_cast<i32>(static_cast<f32>(pitch_input * camera->pad_pitch_speed) * delta_time * 64.0f);
            camera->pitch = MIN(0x4000, camera->pitch);
            camera->pitch = MAX(-0x4000, camera->pitch);
        }
        if (camera->freedoms & EDCAM_FREEDOM_YAW) {
            const i32 yaw_input = NuPs2ApplyDeadZone(pad_yaw_axis, 32);
            camera->yaw -= static_cast<i32>(static_cast<f32>(yaw_input * camera->pad_yaw_speed) * delta_time * 64.0f);
        }

        if (camera->freedoms & EDCAM_FREEDOM_POSITION_Y) {
            movement.y += static_cast<f32>(pad->analog_l1) * camera->position_speed.y * move_speed * frame_scale * 0.5f;
            movement.y -= static_cast<f32>(pad->analog_l2) * camera->position_speed.y * move_speed * frame_scale * 0.5f;
        }
        if (camera->freedoms & EDCAM_FREEDOM_POSITION_Z) {
            movement.z = -static_cast<f32>(NuPs2ApplyDeadZone(pad->analog_left_y, 32)) * camera->position_speed.y *
                         move_speed * frame_scale;
        }
        if (camera->freedoms & EDCAM_FREEDOM_POSITION_X) {
            movement.x = static_cast<f32>(NuPs2ApplyDeadZone(pad->analog_left_x, 32)) * camera->position_speed.y *
                         move_speed * frame_scale;
        }
        NuVecRotateY(&movement, &movement, camera->yaw);
        NuVecAdd(&camera->position, &camera->position, &movement);

        NUVEC snap_delta;
        NuVecSub(&snap_delta, &camera->position, &camera->snap_origin);
        snap_delta.x /= camera->snap_step.x;
        snap_delta.y /= camera->snap_step.y;
        snap_delta.z /= camera->snap_step.z;
        camera->snapped_position.x = NuFloor(snap_delta.x) * camera->snap_step.x + camera->snap_origin.x;
        camera->snapped_position.y = NuFloor(snap_delta.y) * camera->snap_step.y + camera->snap_origin.y;
        camera->snapped_position.z = NuFloor(snap_delta.z) * camera->snap_step.z + camera->snap_origin.z;
    }

    void do_maya_mouse_camera(edcam_s *camera) {
        static i16 TargetAng;
        static i16 MouseOldAng;

        f32 zoom_scale = camera->auto_zoom_dist_scale == 0.0f
                             ? 1.0f
                             : NuFabs(camera->distance) * camera->auto_zoom_dist_scale + camera->auto_zoom_base;
        NUVEC movement = {0.0f, 0.0f, 0.0f};
        if (NuKeyboard(0x38) != 0) {

            const f32 mouse_x = NuMouseReadXVel() * camera->mouse_pitch_speed;
            const f32 mouse_y = NuMouseReadYVel() * camera->mouse_yaw_speed;
            const f32 mouse_z = NuMouseReadZVel() * camera->mouse_move_speed;
            const u32 buttons = NuMouseReadButtons();

            if (buttons == 2 || buttons == 5) {
                if (camera->freedoms & EDCAM_FREEDOM_DISTANCE) {
                    if (mouse_y != 0.0f || mouse_x != 0.0f)
                        TargetAng = static_cast<i16>(NuAtan2D(-mouse_x, -mouse_y));
                    u16 angle = static_cast<u16>(TargetAng);
                    i32 difference = static_cast<u16>(angle - MouseOldAng);
                    if (difference > 0x7fff)
                        difference -= 0x10000;
                    if (abs(difference) > 20000)
                        angle = static_cast<u16>(TargetAng);
                    else
                        angle = static_cast<u16>(MouseOldAng + difference / 4);
                    MouseOldAng = angle;

                    f32 sine = NU_SIN_LUT(static_cast<u16>(angle) + 0x2000);
                    if (NuFabs(sine) > 0.1f) {
                        if (sine > 0.0f)
                            sine -= 0.1f;
                        else
                            sine += 0.1f;
                        f32 distance = NuFsqrt(mouse_x * mouse_x + mouse_y * mouse_y) * sine *
                                           (zoom_scale * camera->distance_speed) * camera->mouse_move_speed +
                                       camera->distance;
                        const f32 minimum = -camera->minimum_distance;
                        camera->distance = MIN(minimum, distance);
                    }
                }
            } else if (buttons == 3 || buttons == 4) {
                movement.x += mouse_x * 0.0078125f;
                movement.y -= mouse_y * 0.0078125f;
            } else if (buttons == 1) {
                if (camera->freedoms & EDCAM_FREEDOM_PITCH) {
                    camera->pitch -= static_cast<i32>(mouse_y * 16.0f);
                    if (camera->pitch > 0x4000)
                        camera->pitch = 0x4000;
                    if (camera->pitch < -0x4000)
                        camera->pitch = -0x4000;
                }
                if (camera->freedoms & EDCAM_FREEDOM_YAW)
                    camera->yaw -= static_cast<i32>(mouse_x * 16.0f);
            }

            const f32 distance = camera->distance - zoom_scale * camera->distance_speed * mouse_z;
            const f32 minimum = -camera->minimum_distance;
            camera->distance = MIN(minimum, distance);

            NUMTX rotation = numtx_identity;
            NuMtxRotateX(&rotation, camera->pitch);
            NuMtxRotateY(&rotation, camera->yaw);
            NuVecMtxRotate(&movement, &movement, &rotation);
            if (camera->freedoms & EDCAM_FREEDOM_POSITION_X)
                camera->position.x += movement.x;
            if (camera->freedoms & EDCAM_FREEDOM_POSITION_Y)
                camera->position.y += movement.y;
            if (camera->freedoms & EDCAM_FREEDOM_POSITION_Z)
                camera->position.z += movement.z;
        }
    }

    void do_mouse_flymode_camera(edcam_s *camera, f32 delta_time) {
        f32 zoom_scale = camera->auto_zoom_dist_scale == 0.0f
                             ? 1.0f
                             : NuFabs(camera->distance) * camera->auto_zoom_dist_scale + camera->auto_zoom_base;

        f32 mouse_y = NuMouseReadYRel();
        f32 mouse_x = NuMouseReadXRel();
        f32 mouse_z = NuMouseReadZRel();
        u32 buttons = NuMouseReadButtons();
        if (camera->allow_distance) {
            f32 distance = camera->distance - mouse_z * camera->distance_speed * zoom_scale;
            camera->distance = MIN(-camera->minimum_distance, distance);
        }

        NUVEC opposite_offset;
        opposite_offset.z = 0.0f;
        opposite_offset.y = 0.0f;
        opposite_offset.x = 0.0f;
        NUMTX rotation = numtx_identity;
        NuMtxRotateX(&rotation, camera->pitch);
        NuMtxRotateY(&rotation, camera->yaw);
        opposite_offset.z = -camera->distance;
        NuVecMtxRotate(&opposite_offset, &opposite_offset, &rotation);
        NUVEC opposite_positions[2];
        NUVEC &old_opposite = opposite_positions[0];
        NuVecAdd(&old_opposite, &camera->position, &opposite_offset);

        i32 pitch;
        if (!camera->allow_pitch)
            pitch = camera->pitch;
        else
            pitch = static_cast<i32>(mouse_y * 16.0f) + camera->pitch;
        pitch = MIN(0x4000, pitch);
        pitch = MAX(pitch, -0x4000);
        camera->pitch = pitch;
        if (camera->allow_yaw)
            camera->yaw += static_cast<i32>(mouse_x * 16.0f);

        rotation = numtx_identity;
        NuMtxRotateX(&rotation, camera->pitch);
        NuMtxRotateY(&rotation, camera->yaw);
        opposite_offset.z = -camera->distance;
        opposite_offset.y = 0.0f;
        opposite_offset.x = 0.0f;
        NuVecMtxRotate(&opposite_offset, &opposite_offset, &rotation);
        NUVEC &new_opposite = opposite_positions[1];
        NuVecAdd(&new_opposite, &camera->position, &opposite_offset);
        NuVecAdd(&camera->position, &camera->position, &new_opposite);
        NuVecSub(&camera->position, &camera->position, &old_opposite);

        NUVEC movement = {0.0f, 0.0f, 0.0f};
        if (buttons == 2)
            movement.z = delta_time * camera->distance;
        else if (buttons == 1)
            movement.z = -delta_time * camera->distance;
        NuVecMtxRotate(&movement, &movement, &rotation);
        if (camera->allow_position_x)
            camera->position.x += movement.x;
        if (camera->allow_position_y)
            camera->position.y += movement.y;
        if (camera->allow_position_z)
            camera->position.z += movement.z;
    }

    void edcamMoveEx(nupad_s *pad, f32 delta_time) {
        if (edmainGetCursorEnabled() != 0) {
            if (PadFlyMode == 0 || NuKeyboard(0x38) != 0) {
                do_maya_mouse_camera(&gp_cam);
            } else {
                do_mouse_flymode_camera(&gp_cam, delta_time);
            }
        }
        if (pad != NULL) {
            if (PadFlyMode == 0) {
                do_Pad_Standard_camera(&gp_cam, delta_time, pad);
            } else {
                do_Pad_flymode_camera(&gp_cam, delta_time, pad);
            }
        }
    }

    void edcamMove(nupad_s *pad) {
        edcamMoveEx(pad, NuTimeGetFrameTime());
    }

    void edcamSetPos(NUVEC *position) {
        gp_cam.position = *position;
        gp_cam.offset = {0.0f, 0.0f, 0.0f};
    }

    void edcamSetAng(i32 pitch, i32 yaw) {
        gp_cam.pitch = pitch;
        gp_cam.yaw = yaw;
    }

    void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw) {
        gp_cam.position = *position;
        gp_cam.offset = {0.0f, 0.0f, 0.0f};
        gp_cam.pitch = pitch;
        gp_cam.yaw = yaw;
    }

    void edcamGetPosAng(NUVEC *position, i32 *pitch, i32 *yaw) {
        if (position != NULL) {
            *position = gp_cam.position;
        }
        if (pitch != NULL) {
            *pitch = gp_cam.pitch;
        }
        if (yaw != NULL) {
            *yaw = gp_cam.yaw;
        }
    }

    void edcamGetPosAngSnap(NUVEC *position, i32 *pitch, i32 *yaw) {
        if (position != NULL) {
            *position = gp_cam.snapped_position;
        }
        if (pitch != NULL) {
            *pitch = gp_cam.snapped_pitch;
        }
        if (yaw != NULL) {
            *yaw = gp_cam.snapped_yaw;
        }
    }

}

void edcamSetContoller(i32 invert_pitch) {
    gp_cam.invert_pad_pitch = invert_pitch;
}

extern "C" {
    void edcamSetDist(f32 distance) {
        gp_cam.distance = distance;
    }

    f32 edcamGetDist(void) {
        return gp_cam.distance;
    }

    edcam_s *edcamGetEdCam(void) {
        return &gp_cam;
    }

    void edcamSetOffset(NUVEC *offset) {
        gp_cam.offset = *offset;
    }

    void edcamGetOffset(NUVEC *offset) {
        *offset = gp_cam.offset;
    }

    void edcamMtx(NUMTX *matrix) {
        NUVEC distance = {0.0f, 0.0f, gp_cam.distance};
        NuMtxSetTranslation(matrix, &distance);
        NuMtxRotateX(matrix, gp_cam.pitch);
        NuMtxRotateY(matrix, gp_cam.yaw);
        NuMtxTranslate(matrix, &gp_cam.position);
        NuMtxTranslate(matrix, &gp_cam.offset);
        if (ed_remap_mtx != NULL) {
            NuMtxMul(matrix, matrix, ed_remap_mtx);
            ed_remap_mtx = NULL;
        }
    }

    void edcamSet(void) {
        NUMTX matrix;
        edcamMtx(&matrix);
        edmainSetCamera(&matrix);
    }

    NUVEC *edcamGetPosPointer(void) {
        return &gp_cam.position;
    }

}

void EdFileSwapEndianess16(void *data) {
    char *bytes;
    char tmp;

    bytes = (char *)data;

    tmp = bytes[0];
    bytes[0] = bytes[1];
    bytes[1] = tmp;
}

void EdFileSwapEndianess32(void *data) {
    char *bytes;
    char tmp;

    bytes = (char *)data;

    tmp = bytes[0];
    bytes[0] = bytes[3];
    bytes[3] = tmp;

    tmp = bytes[1];
    bytes[1] = bytes[2];
    bytes[2] = tmp;
}

void EdFileSetReadWrongEndianess(i32 value) {
}

void EdFileSetMedia(i32 media) {
    edfile_media = media;
}

void EdFileSetPakFile(void *pak) {
    edfile_pakfile = pak;
}

void EdFileResetBuffers() {
    memset(edfile_buffer, 0, sizeof(edfile_buffer));
    edfile_buffer_pointer = 0;
    edfile_lock[1] = 0;
    edfile_lock[0] = 0;
}

void EdFileFillBuffer() {
    if (edfile_handle != -1) {
        memset(edfile_buffer, 0, sizeof(edfile_buffer));

        if (edfile_media == 1) {
            NuFileRead(edfile_handle, edfile_buffer, 0x1000);
            edfile_buffer_pointer = 0;
        }
    }
}

i32 EdFileOpen(char *filepath, NUFILEMODE mode) {
    i32 item_handle;
    void *item;
    i32 item_size;

    if (edfile_handle != -1) {
        return 0;
    }

    EdFileResetBuffers();

    edfile_handle = -1;
    edfile_write_flag = 0;

    if (edfile_media != 1) {
        return 0;
    }

    if (mode != NUFILE_READ) {
        if (mode != NUFILE_WRITE) {
            return 0;
        }

        edfile_write_flag = 1;
        edfile_handle = NuFileOpen(filepath, NUFILE_WRITE);

        if (edfile_handle > 0) {
            return 1;
        }

        edfile_handle = -1;

        return 0;
    }

    if (edfile_pakfile != NULL) {
        item_handle = NuFilePakGetItem(edfile_pakfile, filepath);

        if (item_handle != 0) {
            NuFilePakGetItemInfo(edfile_pakfile, item_handle, &item, &item_size);

            edfile_handle = NuMemFileOpen(item, item_size, NUFILE_READ);

            if (edfile_handle > 0) {
                EdFileFillBuffer();

                return 1;
            }

            edfile_handle = -1;
        }
    }

    edfile_handle = NuFileOpen(filepath, NUFILE_READ);

    if (edfile_handle > 0) {
        EdFileFillBuffer();

        return 1;
    }

    edfile_handle = -1;

    return 0;
}

void EdFileFlushBuffer() {
    if (edfile_handle == -1) {
        return;
    }

    if (edfile_buffer_pointer == 0 || edfile_media != 1) {
        return;
    }

    NuFileWrite(edfile_handle, edfile_buffer, edfile_buffer_pointer);
    edfile_buffer_pointer = 0;
}

i32 EdFileClose() {
    edfile_pakfile = NULL;

    if (edfile_handle == -1) {
        return 0;
    }

    if (__builtin_expect(edfile_write_flag != 0, 0)) {
        EdFileFlushBuffer();
    }

    if (edfile_media != 1) {
        return 0;
    }

    NuFileClose(edfile_handle);
    edfile_handle = -1;
    edfile_media = 0;

    return 1;
}

void EdFileWrite(void *data, i32 len) {
    i32 to_write;

    while (len > 0) {
        to_write = MIN(0x1000 - edfile_buffer_pointer, len);

        memcpy(edfile_buffer + edfile_buffer_pointer, data, to_write);

        edfile_buffer_pointer += to_write;
        len -= to_write;
        data = (void *)((usize)data + to_write);

        if (edfile_buffer_pointer == 0x1000) {
            EdFileFlushBuffer();
        }
    }
}

void EdFileWriteFloat(f32 value) {
    f32 data = value;
    if (edfile_readwrongendianess)
        EdFileSwapEndianess32(&data);
    EdFileWrite(&data, 4);
}

void EdFileWriteInt(i32 value) {
    i32 data = value;
    if (edfile_readwrongendianess)
        EdFileSwapEndianess32(&data);
    EdFileWrite(&data, 4);
}

void EdFileWriteUnsignedInt(u32 value) {
    u32 data = value;
    if (edfile_readwrongendianess)
        EdFileSwapEndianess32(&data);
    EdFileWrite(&data, 4);
}

void EdFileWriteShort(i16 value) {
    i16 data = value;
    if (edfile_readwrongendianess)
        EdFileSwapEndianess16(&data);
    EdFileWrite(&data, 2);
}

void EdFileWriteUnsignedShort(u16 value) {
    u16 data = value;
    if (edfile_readwrongendianess)
        EdFileSwapEndianess16(&data);
    EdFileWrite(&data, 2);
}

void EdFileWriteChar(char value) {
    EdFileWrite(&value, 1);
}

void EdFileWriteUnsignedChar(u8 value) {
    EdFileWrite(&value, 1);
}

void EdFileWriteNuVec(NUVEC *value) {
    EdFileWriteFloat(value->x);
    EdFileWriteFloat(value->y);
    EdFileWriteFloat(value->z);
}

void EdFileRead(void *buf, i32 len) {
    i32 to_read;

    while (len > 0) {
        to_read = MIN(0x1000 - edfile_buffer_pointer, len);
        memcpy(buf, edfile_buffer + edfile_buffer_pointer, to_read);

        edfile_buffer_pointer += to_read;
        len -= to_read;
        buf = (void *)((usize)buf + to_read);

        if (edfile_buffer_pointer == 0x1000) {
            EdFileFillBuffer();
        }
    }
}

f32 EdFileReadFloat() {
    f32 data;

    EdFileRead(&data, 4);

    if (edfile_readwrongendianess) {
        EdFileSwapEndianess32(&data);
    }

    return data;
}

i32 EdFileReadInt() {
    i32 data;

    EdFileRead(&data, 4);

    if (edfile_readwrongendianess) {
        EdFileSwapEndianess32(&data);
    }

    return data;
}

u32 EdFileReadUnsignedInt() {
    u32 data;

    EdFileRead(&data, 4);

    if (edfile_readwrongendianess) {
        EdFileSwapEndianess32(&data);
    }

    return data;
}

i16 EdFileReadShort() {
    i16 data;

    EdFileRead(&data, 2);

    if (edfile_readwrongendianess) {
        EdFileSwapEndianess16(&data);
    }

    return data;
}

u16 EdFileReadUnsignedShort() {
    u16 data;

    EdFileRead(&data, 2);

    if (edfile_readwrongendianess) {
        EdFileSwapEndianess16(&data);
    }

    return data;
}

char EdFileReadChar() {
    char data;

    EdFileRead(&data, 1);

    return data;
}

unsigned char EdFileReadUnsignedChar() {
    unsigned char data;

    EdFileRead(&data, 1);

    return data;
}

void EdFileReadNuVec(NUVEC *out) {
    out->x = EdFileReadFloat();
    out->y = EdFileReadFloat();
    out->z = EdFileReadFloat();
}

i32 EdFileWriteMemCard(char *filename, i32 size, void *data) {
    EdFileSetMedia(2);
    if (EdFileOpen(filename, static_cast<NUFILEMODE>(1))) {
        EdFileWrite(data, size);
        return EdFileClose();
    }
    return 0;
}

i32 EdFileReadMemCard(char *filename, i32 size, void *data) {
    EdFileSetMedia(2);
    if (EdFileOpen(filename, static_cast<NUFILEMODE>(0))) {
        EdFileRead(data, size);
        return EdFileClose();
    }
    return 0;
}

i32 EdFileBackup(char *source, char *destination) {
    i64 remaining = NuFileSize(source);
    if (remaining > 0) {
        NUFILE output = NuFileOpen(destination, NUFILE_WRITE);
        NUFILE input = NuFileOpen(source, NUFILE_READ);
        if (output > 0 && input > 0) {
            while (remaining > 0) {
                i32 size = MIN(static_cast<i32>(remaining), 0x1000);
                NuFileRead(input, edfile_buffer, size);
                NuFileWrite(output, edfile_buffer, size);
                remaining -= 0x1000;
            }
            NuFileClose(input);
            NuFileClose(output);
            return 1;
        }
        if (output > 0)
            NuFileClose(output);
        if (input > 0)
            NuFileClose(input);
    }
    return 0;
}

void FS_MakeDateTimeString(FS_FILEENTRYHDR *entry, char *output) {
    const u8 *date = reinterpret_cast<const u8 *>(entry);
    sprintf(output, " %.2d/%.2d/%d %.2d:%.2d:%.2d", date[4] - 'A', date[5] - 'A', date[6] + 1915, date[3] - 'A',
            date[2] - 'A', date[1] - 'A');
}

void FS_MakeTimeString(FS_FILEENTRYHDR *entry, char *output) {
    const u8 *date = reinterpret_cast<const u8 *>(entry);
    sprintf(output, " %.2d:%.2d:%.2d", date[3] - 'A', date[2] - 'A', date[1] - 'A');
}

void FS_MakeDateString(FS_FILEENTRYHDR *entry, char *output) {
    const u8 *date = reinterpret_cast<const u8 *>(entry);
    sprintf(output, " %.2d/%.2d/%d", date[4] - 'A', date[5] - 'A', date[6] + 1915);
}

static void RenderFileSel3Body(i32 draw_title) {
    char buffer[256];
    f32 x = FS_X;
    f32 y = FS_Y;
    const f32 width = FS_Width;
    NuQFntPushPrintMode(2);
    NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
    NuQFntSet(system_qfont);
    NuQFntSetPointSize(system_qfont, 1.0f, 1.0f);
    const f32 height = NuQFntHeight(system_qfont);
    const f32 baseline = NuQFntBaseline(system_qfont);
    f32 full_width = width;
    if (FS_NumFiles > 14) {
        full_width = width + 8.0f;
    }

    f32 text_x;
    if (draw_title) {
        const f32 box_width = full_width + 4.0f;
        const f32 box_height = height * 17.0f / 16.0f + 6.0f;
        NuRndrRect2d(x, y, 0.0f, box_width, box_height, 0xff808080, NULL);
        FS_W = box_width;
        FS_H = box_height;
        x += 2.0f;
        text_x = (x + 4.0f) * 16.0f;
        y += 1.0f;
        const f32 title_height = height / 16.0f;
        NuRndrRect2d(x, y, 0.0f, full_width, title_height, 0xff404040, NULL);
        NuRndrLine3dDbg(x, y, 0.0f, width + x, y, 0.0f, 0xff808080);
        const f32 title_y = y * 16.0f + baseline;
        NuQFntSet(system_qfont);
        NuQFntSetColour(system_qfont, 0x80808080);
        NuQFntMove(system_qfont, text_x, title_y, 0.0f);
        NuQFntPrintU(system_qfont, FS_Title);
        y += title_height + 2.0f;
    } else {
        const f32 box_width = full_width + 4.0f;
        const f32 box_height = height * 16.0f / 16.0f + 4.0f;
        NuRndrRect2d(x, y, 0.0f, box_width, box_height, 0xff808080, NULL);
        FS_W = box_width;
        FS_H = box_height;
        x += 2.0f;
        text_x = (x + 4.0f) * 16.0f;
        y += 1.0f;
    }

    const f32 line_height = height / 16.0f;
    const f32 path_y = y * 16.0f + baseline;
    NuRndrRect2d(x, y, 0.0f, full_width, line_height, 0x20202020, NULL);
    char *text;
    if (FS_ShowVolumes) {
        NuStrCpy(buffer, "Volumes:-");
        text = buffer;
    } else {
        text = FS_Path;
        const f32 available = full_width - 8.0f;
        f32 length = NuQFntPrintLenU(system_qfont, text) / 16.0f;
        if (length > available) {
            f32 scale = available / length;
            if (__builtin_expect(scale < 0.5f, 0)) {
                // Too long even at half width: keep the drive and first
                // directory, then "\....", then the last directory.
                char *first = NuStrChr(text, '\\');
                if (first != NULL) {
                    char *second = NuStrChr(first + 1, '\\');
                    if (second == NULL) {
                        second = first;
                    }
                    *second = '\0';
                    NuStrCpy(buffer, text);
                    *second = '\\';
                    NuStrCat(buffer, "\\....");
                    char *last;
                    const i32 tail_length = NuStrLen(second);
                    if (tail_length != 0 && second[tail_length - 1] == '\\') {
                        second[tail_length - 1] = '\0';
                        last = NuStrRChr(second, '\\');
                        second[tail_length - 1] = '\\';
                    } else {
                        last = NuStrRChr(second, '\\');
                    }
                    if (second < last) {
                        NuStrCat(buffer, last);
                        text = buffer;
                    }
                }
                length = NuQFntPrintLenU(system_qfont, text) / 16.0f;
                if (!(length > available)) {
                    goto path_done;
                }
                scale = available / length;
            }
            NuQFntSetPointSize(system_qfont, scale, 1.0f);
        }
    path_done:;
    }
    NuQFntSet(system_qfont);
    NuQFntMove(system_qfont, text_x, path_y, 0.0f);
    NuQFntSetColour(system_qfont, 0x80408080);
    NuQFntPrintU(system_qfont, text);
    NuQFntSetPointSize(system_qfont, 1.0f, 1.0f);

    y += line_height + 2.0f;
    const f32 list_height = height * 14.0f;
    const f32 list_box_height = list_height / 16.0f;
    NuRndrRect2d(x, y, 0.0f, width, list_box_height, 0x20202020, NULL);
    if (FS_NumFiles > 14) {
        NuRndrRect2d(width + x, y, 0.0f, 8.0f, list_box_height, 0xff646464, NULL);
        f32 thumb = list_box_height / FS_NumFiles * 14.0f;
        if (thumb < 5.0f) {
            thumb = 5.0f;
        }
        NuRndrRect2d(width + x, (list_box_height - thumb) * FS_CurrentPosFileNum / (FS_NumFiles - 14) + y, 0.0f, 8.0f,
                     thumb, 0xffc8c8c8, NULL);
    }

    f32 line_y = path_y + 32.0f;
    char *entry = FS_CurrentPos;
    for (i32 line = 0; line < 14; ++line) {
        if (entry >= FS_FileListEnd) {
            break;
        }
        line_y += height;
        if (entry == FS_CurrentCursorPos) {
            NuRndrRect2d(x, (line_y - baseline) / 16.0f, 0.0f, width, line_height, 0xff202060, NULL);
        }
        NuQFntSet(system_qfont);
        NuQFntMove(system_qfont, text_x, line_y, 0.0f);
        if (entry[0] == 'V') {
            NuQFntSetColour(system_qfont, 0x80204080);
        } else if (entry[0] == 'D') {
            NuQFntSetColour(system_qfont, 0x80804020);
        } else {
            NuQFntSetColour(system_qfont, 0x80408080);
        }
        NuQFntSetPointSize(system_qfont, 1.0f, 1.0f);
        char *name = entry + 7;
        NuQFntPrintU(system_qfont, name);
        if (entry[6] != '#') {
            NuQFntSetPointSize(system_qfont, 0.8f, 1.0f);
            FS_MakeTimeString(reinterpret_cast<FS_FILEENTRYHDR *>(entry), buffer);
            f32 length = NuQFntPrintLenU(system_qfont, buffer);
            NuQFntSetColour(system_qfont, 0x80208080);
            const f32 time_x = (width + x) * 16.0f - length;
            NuQFntMove(system_qfont, time_x, line_y, 0.0f);
            NuQFntPrintU(system_qfont, buffer);
            FS_MakeDateString(reinterpret_cast<FS_FILEENTRYHDR *>(entry), buffer);
            length = NuQFntPrintLenU(system_qfont, buffer);
            NuQFntSetColour(system_qfont, 0x80808020);
            NuQFntMove(system_qfont, time_x - length, line_y, 0.0f);
            NuQFntPrintU(system_qfont, buffer);
        }
        entry = name + NuStrLen(name) + 1;
    }

    const f32 footer_y = y * 16.0f + baseline + list_height;
    NuQFntSet(system_qfont);
    NuQFntSetPointSize(system_qfont, 1.0f, 1.0f);
    NuQFntSetColour(system_qfont, 0x80808080);
    NuQFntMove(system_qfont, x * 16.0f, footer_y, 0.0f);
    NuQFntPrintU(system_qfont, FS_Filter);
    NuQFntSetColour(system_qfont, 0x80000040);
    const f32 sort_length = NuQFntPrintLenU(system_qfont, SortStrings[FS_SortMode]);
    NuQFntMove(system_qfont, (full_width + x) * 16.0f - sort_length, footer_y, 0.0f);
    NuQFntPrintU(system_qfont, SortStrings[FS_SortMode]);
    NuQFntPopCoordinateSystem();
    NuQFntPopPrintMode();
}

void FS_MoveCursorDown(i32 steps) {
    char **currentPos = &FS_CurrentPos;
    char **cursorPos = &FS_CurrentCursorPos;
    while (steps > 0) {
        i32 currentPosLength = NuStrLen(*currentPos);
        i32 cursorLength = NuStrLen(*cursorPos);
        char *next = *cursorPos + cursorLength + 1;
        char *listEnd = FS_FileListEnd;

        if (next < listEnd) {
            i32 *cursorLine = &FS_CursorLine;
            i32 line = *cursorLine;
            if (line == 13) {
                char *nextTop = *currentPos + currentPosLength + 1;
                if (listEnd > nextTop) {
                    *currentPos = nextTop;
                    ++FS_CurrentPosFileNum;
                }
            } else {
                *cursorLine = line + 1;
            }
            *cursorPos = next;
        }
        --steps;
    }
}

i32 FS_PrevNameLen(char *name) {
    if (__builtin_expect(name == FS_FileList, 0)) {
        return 0;
    }
    char *previous = name - 2;
    if (__builtin_expect(*previous == '\0', 0)) {
        return 1;
    }
    if (__builtin_expect(previous == FS_FileList, 0)) {
        return 2;
    }
    i32 length = 0;
    for (;;) {
        --previous;
        ++length;
        if (*previous == '\0') {
            return length + 1;
        }
        if (previous == FS_FileList) {
            return length + 2;
        }
    }
}

void FS_MoveCursorUp(i32 steps) {
    while (steps > 0) {
        i32 currentPosLength = FS_PrevNameLen(FS_CurrentPos);
        i32 cursorLength = FS_PrevNameLen(FS_CurrentCursorPos);
        char *cursor = FS_CurrentCursorPos;

        if (cursor == FS_CurrentPos) {
            FS_CurrentPos = cursor - currentPosLength;
            if (FS_CurrentPosFileNum != 0) {
                --FS_CurrentPosFileNum;
            }
        } else {
            --FS_CursorLine;
        }

        FS_CurrentCursorPos = cursor - cursorLength;
        --steps;
    }
}

void RenderFileSel3(i32 draw_title) {
    if (FS_Active == 0) {
        return;
    }
    RenderFileSel3Body(draw_title);
}

extern "C" {
extern "C" void RenderFileSel2(i32 x, i32 y, i32 *width, i32 *height) {
    FS_X = static_cast<f32>(x);
    FS_Y = static_cast<f32>(y) * 0.5f;
    RenderFileSel3(0);
    *height = static_cast<i32>(FS_H + FS_H);
    *width = static_cast<i32>(FS_W);
}

}

void RenderFileSel() {
    RenderFileSel3(1);
}

f32 FS_GetDirTextWidth() {
    char date_time[64];
    f32 max_width = 0.0f;
    for (char *entry = FS_FileList; entry < FS_FileListEnd;) {
        char *name = entry + 7;
        NuQFntSetPointSize(system_qfont, 1.0f, 1.0f);
        f32 width = NuQFntPrintLenU(system_qfont, name);
        if (entry[6] != '#') {
            NuQFntSetPointSize(system_qfont, 0.8f, 1.0f);
            FS_MakeDateTimeString(reinterpret_cast<FS_FILEENTRYHDR *>(entry), date_time);
            width += NuQFntPrintLenU(system_qfont, date_time);
        }
        if (width > max_width) {
            max_width = width;
        }
        entry = name + NuStrLen(name) + 1;
    }
    return max_width;
}

char *FS_GetFilterString(char *input, char *output) {
    while (*input == ' ' || *input == '*') {
        ++input;
    }

    char *end = input;
    if (*end != '|' && *end != '\0') {
        do {
            ++end;
        } while (*end != '\0' && *end != '|' && *end != '*');

        char *trimmed = end;
        while (trimmed != input && trimmed[-1] == ' ') {
            --trimmed;
        }
        for (; input < trimmed; ++input) {
            *output++ = *input;
        }
    }
    *output = '\0';
    return end;
}

i32 FS_FileNameFilter(char *name) {
    char lower[256];
    NuStrCpy(lower, name);
    NuStrToLower(lower);

    for (i32 i = 0; i < numfilteroutblocks; ++i) {
        if (strstr(lower, filteroutblocks[i]) != NULL) {
            return 0;
        }
    }

    for (i32 i = 0; i < numfilterblocks; ++i) {
        const char *block = filterblocks[i];
        char *scan = lower;
        do {
            char *match;
            if (block[0] != '\0') {
                match = strstr(scan, block);
                if (match == NULL) {
                    break;
                }
                scan = match + 1;
            } else {
                match = scan;
                scan = NULL;
                goto eligible_filter;
            }

            if (block[12] == '\0') {
                goto eligible_filter;
            }
            match = strstr(match, block + 12);
            if (match == NULL) {
                continue;
            }
            if (block[24] == '\0') {
                goto eligible_filter;
            }
            match = strstr(match, block + 24);
            if (match == NULL) {
                continue;
            }
            if (block[36] == '\0') {
                goto eligible_filter;
            }
            match = strstr(match, block + 36);
            if (match == NULL) {
                continue;
            }
            if (block[48] == '\0') {
                goto eligible_filter;
            }
            match = strstr(match, block + 48);
            if (match == NULL) {
                continue;
            }
            if (block[60] == '\0') {
                goto eligible_filter;
            }
            match = strstr(match, block + 60);
            if (match == NULL) {
                continue;
            }
            if (block[72] != '\0') {
                continue;
            }

        eligible_filter:
            i32 dots = 0;
            for (char *p = match; *p != '\0'; ++p) {
                if (*p == '.') {
                    if (++dots > 1) {
                        break;
                    }
                }
            }
            if (dots < 2) {
                return 1;
            }
        } while (scan != NULL);
    }
    return 0;
}

void FS_BuildFilterBlocks(char *input) {
    numfilterblocks = 0;
    NuStrToLower(input);
    i32 slot = 0;
    while (*input != '\0') {
        input = FS_GetFilterString(input, &filterblocks[0][0] + numfilterblocks * 96 + slot * 12);
        if (*input == '|') {
            filterblocks[numfilterblocks][(slot + 1) * 12] = '\0';
            ++numfilterblocks;
            slot = 0;
            ++input;
        } else {
            ++slot;
        }
    }
    if (slot != 0) {
        ++numfilterblocks;
    }
}

void FS_BuildFilterOutBlocks(char *input) {
    numfilteroutblocks = 0;
    NuStrToLower(input);
    while (*input != '\0') {
        input = FS_GetFilterString(input, filteroutblocks[numfilteroutblocks]);
        if (filteroutblocks[numfilteroutblocks][0] != '\0') {
            ++numfilteroutblocks;
        }
        if (*input == '|') {
            ++input;
        }
    }
}

void FS_SortStrings(char *start, char *end, i32 mode) {
    char tmp[256];

    if (!(static_cast<u32>(mode) > 1)) {
        const i32 direction = mode == 0 ? 1 : -1;
        for (char *current = start; current != end;) {
            i32 current_length = NuStrLen(current);
            char *candidate = current + NuStrLen(current) + 1;
            while (candidate != end) {
                const i32 candidate_length = NuStrLen(candidate);
                if (NuStrICmp(current + 7, candidate + 7) == direction) {
                    NuStrCpy(tmp, candidate);
                    memmove(current + candidate_length + 1, current, candidate - current);
                    memcpy(current, tmp, candidate_length + 1);
                    current_length = candidate_length;
                }
                candidate += candidate_length + 1;
            }
            current += current_length + 1;
        }
    } else {
        const i32 direction = mode == 2 ? 1 : -1;
        for (char *current = start; current != end;) {
            u32 current_key = FS_EncodedDateKey(current);
            i32 current_length = NuStrLen(current);
            char *candidate = current + NuStrLen(current) + 1;
            while (candidate != end) {
                const u32 candidate_key = FS_EncodedDateKey(candidate);
                const i32 candidate_length = NuStrLen(candidate);
                const i32 compare = candidate_key < current_key ? 1 : (candidate_key > current_key ? -1 : 0);
                if (compare == direction) {
                    NuStrCpy(tmp, candidate);
                    memmove(current + candidate_length + 1, current, candidate - current);
                    memcpy(current, tmp, candidate_length + 1);
                    current_key = candidate_key;
                    current_length = candidate_length;
                }
                candidate += candidate_length + 1;
            }
            current += current_length + 1;
        }
    }
}

void FS_SetCursorToLastFileName() {
    char *last_name = FS_LastFileName;
    if (last_name[0] == '\0' || FS_NumFiles <= 0) {
        return;
    }
    char *entry = FS_FileList;
    for (i32 index = 0; index < FS_NumFiles; ++index) {
        if (NuStrICmp(entry + 7, last_name) == 0) {
            FS_CurrentCursorPos = entry;
            FS_CurrentPos = entry;
            FS_CursorLine = 0;
            FS_CurrentPosFileNum = index;
            if (index == 0 || index + 13 < FS_NumFiles) {
                return;
            }
            char *current = entry;
            i32 last_visible = index + 12;
            volatile i32 old_visible;
            do {
                i32 previous_length = FS_PrevNameLen(current);
                if (previous_length == 0) {
                    return;
                }
                current = FS_CurrentPos - previous_length;
                --FS_CurrentPosFileNum;
                FS_CurrentPos = current;
                ++FS_CursorLine;
                if (last_visible == 13) {
                    return;
                }
                old_visible = last_visible--;
            } while (old_visible >= FS_NumFiles);
            return;
        }
        entry += NuStrLen(entry) + 1;
    }
}

void FS_GetDirList(char *path, char *filter, char *filter_out) {
    FS_BuildFilterBlocks(filter);
    FS_BuildFilterOutBlocks(filter_out);

    FS_CurrentCursorPos = FS_FileList;
    FS_CurrentPos = FS_FileList;
    FS_NumFiles = 0;
    FS_CurrentPosFileNum = 0;
    FS_CursorLine = 0;

    NUFILE directory = NuFileOpenDir(path);
    __builtin_memcpy(FS_FileList, FS_ParentEntry, 7);
    NuStrCpy(FS_FileList + 7, "..");
    ++FS_NumFiles;

    char *volatile next;
    char entry[0x118] __attribute__((aligned(16)));
    volatile char header[7];
    if (directory != 0) {
        next = FS_FileList + 10;
        while (i32 name_length = NuFileReadDir(directory, entry)) {
            char *name = entry + 0x18;
            if (*reinterpret_cast<u16 *>(name) == '.' || (*reinterpret_cast<u32 *>(name) & 0xffffff) == 0x2e2e ||
                !(entry[0] & 8)) {
                continue;
            }
            if (next + name_length + 8 >= FS_FileList + sizeof(FS_FileList)) {
                break;
            }
            header[0] = 'D';
            header[4] = static_cast<u8>(entry[11]) + 'A';
            header[3] = static_cast<u8>(entry[10]) + 'A';
            header[2] = static_cast<u8>(entry[9]) + 'A';
            header[1] = static_cast<u8>(entry[8]) + 'A';
            header[5] = static_cast<u8>(entry[12]) + 'A';
            u8 encoded_year = static_cast<u8>(entry[14]) - 123;
            header[6] = encoded_year;
            __builtin_memcpy(next, const_cast<const char *>(header), 6);
            next[6] = encoded_year;
            char *name_dest = next + 7;
            NuStrCpy(name_dest, name);
            name_dest[name_length] = '\0';
            next = name_dest + name_length + 1;
            ++FS_NumFiles;
        }
        FS_SortStrings(FS_FileList + 10, next, FS_SortMode);
        NuFileCloseDir(directory);
    } else {
        next = FS_FileList + 10;
        FS_SortStrings(next, next, FS_SortMode);
    }

    char *file_begin = next;
    directory = NuFileOpenDir(path);
    if (directory != 0) {
        while (i32 name_length = NuFileReadDir(directory, entry)) {
            char *name = entry + 0x18;
            if ((entry[0] & 8) || !FS_FileNameFilter(name)) {
                continue;
            }
            if (next + name_length + 8 >= FS_FileList + sizeof(FS_FileList)) {
                break;
            }
            header[0] = 'F';
            header[4] = static_cast<u8>(entry[19]) + 'A';
            header[3] = static_cast<u8>(entry[18]) + 'A';
            header[2] = static_cast<u8>(entry[17]) + 'A';
            header[1] = static_cast<u8>(entry[16]) + 'A';
            header[5] = static_cast<u8>(entry[20]) + 'A';
            u8 encoded_year = static_cast<u8>(entry[22]) - 123;
            header[6] = encoded_year;
            __builtin_memcpy(next, const_cast<const char *>(header), 6);
            next[6] = encoded_year;
            char *name_dest = next + 7;
            NuStrCpy(name_dest, name);
            name_dest[name_length] = '\0';
            next = name_dest + name_length + 1;
            ++FS_NumFiles;
        }
        FS_SortStrings(file_begin, next, FS_SortMode);
        NuFileCloseDir(directory);
    } else {
        FS_SortStrings(file_begin, file_begin, FS_SortMode);
    }
    FS_FileListEnd = next;
}

i32 FS_GetPadWithRepeat(nupad_s *pad, float repeat, float elapsed) {
    static i32 LastPad;
    static float PadRepeat;

    if (pad->digital_buttons != LastPad) {
        LastPad = pad->digital_buttons;
        PadRepeat = 0.5f;
        return LastPad;
    }

    PadRepeat -= elapsed;
    if (PadRepeat < 0.0f) {
        PadRepeat += repeat;
        return LastPad;
    }
    return 0;
}

i32 ProcessFileSel3(f32 elapsed, nupad_s *pad) {
    if (FS_Active == 0)
        return 0;
    if (FS_RefreshDir != 0) {
        FS_GetDirList(FS_Path, FS_Filter, FS_FilterOut);
        NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
        NuQFntSet(system_qfont);
        NuQFntSetPointSize(system_qfont, 1.0f, 1.0f);
        f32 width = FS_GetDirTextWidth() * 0.0625f;
        NuQFntPopCoordinateSystem();
        if (width < 240.0f)
            width = 240.0f;
        FS_Width = width + 8.0f;
        FS_GetPadWithRepeat(pad, 0.05f, elapsed);
        if (FS_LastFileName[0] != '\0')
            FS_SetCursorToLastFileName();
        FS_RefreshDir = 0;
    }
    const i32 buttons = FS_GetPadWithRepeat(pad, 0.05f, elapsed);
    if ((buttons & 0x4000) != 0)
        FS_MoveCursorDown(1);
    if ((buttons & 0x1000) != 0)
        FS_MoveCursorUp(1);
    if ((buttons & 1) != 0)
        FS_MoveCursorDown(14);
    if ((buttons & 4) != 0)
        FS_MoveCursorUp(14);
    if ((buttons & 2) != 0)
        FS_MoveCursorDown(FS_NumFiles);
    if ((buttons & 8) != 0)
        FS_MoveCursorUp(FS_NumFiles);
    if ((buttons & 0x80) != 0) {
        FS_RefreshDir = 2;
        FS_SortMode = static_cast<i32>(static_cast<u32>(FS_SortMode) + 1);
        if (FS_SortMode == 4)
            FS_SortMode = 0;
        NuStrCpy(FS_LastFileName, FS_CurrentCursorPos + 7);
    }
    if ((buttons & 0x20) != 0) {
        const i32 length = NuStrLen(FS_Path);
        if (length != 0 && FS_Path[length - 1] == '\\')
            FS_Path[length - 1] = '\0';
        char *separator = NuStrRChr(FS_Path, '\\');
        if (separator != NULL)
            separator[1] = '\0';
        else
            NuStrCat(FS_Path, "\\");
        FS_RefreshDir = 1;
        FS_LastFileName[0] = '\0';
    }
    if ((buttons & 0x40) != 0) {
        if (FS_CurrentCursorPos[0] == 'V') {
            FS_ShowVolumes = 0;
            NuStrCpy(FS_Path, FS_CurrentCursorPos + 7);
            FS_RefreshDir = 1;
            FS_LastFileName[0] = '\0';
        } else if (FS_CurrentCursorPos[0] == 'D') {
            if (FS_CurrentCursorPos[7] == '.' && FS_CurrentCursorPos[8] == '.' && FS_CurrentCursorPos[9] == '\0') {
                const i32 length = NuStrLen(FS_Path);
                if (length != 0 && FS_Path[length - 1] == '\\')
                    FS_Path[length - 1] = '\0';
                char *separator = NuStrRChr(FS_Path, '\\');
                if (separator != NULL)
                    separator[1] = '\0';
                else
                    NuStrCat(FS_Path, "\\");
            } else {
                const i32 length = NuStrLen(FS_Path);
                if (length != 0 && FS_Path[length - 1] != '\\')
                    NuStrCat(FS_Path, "\\");
                NuStrCat(FS_Path, FS_CurrentCursorPos + 7);
                NuStrCat(FS_Path, "\\");
            }
            FS_RefreshDir = 1;
            FS_LastFileName[0] = '\0';
        } else {
            FS_Active = 0;
            NuStrCpy(FS_LastFileName, FS_CurrentCursorPos + 7);
            if (FS_Callback != NULL)
                FS_Callback(FS_Path, FS_CurrentCursorPos + 7);
            return 1;
        }
    }
    return 0;
}

extern "C" {
extern "C" i32 ProcessFileSel2(f32 elapsed, nupad_s *pad) {
    return ProcessFileSel3(elapsed, pad);
}

extern "C" void FileSelKill(void) {
    NuStrCpy(FS_LastFileName, FS_CurrentCursorPos + 7);
    FS_Active = 0;
}

}

void ProcessFileSel(f32 elapsed, nupad_s *pad) {
    ProcessFileSel3(elapsed, pad);
    RenderFileSel();
    if ((pad->digital_buttons & 0x10) != 0)
        FileSelKill();
}

extern "C" {
    void FS_SetFileSelPathFromName(char *name) {
        char file_name[256] = "";
        NuStrCpy(FS_Path, name);
        if (NuStrRChr(FS_Path, '.') != NULL) {
            char *separator = NuStrRChr(FS_Path, '\\');
            if (separator != NULL) {
                NuStrCpy(file_name, separator + 1);
                separator[1] = '\0';
            } else {
                NuStrCpy(file_name, name);
                FS_Path[0] = '\0';
            }
        }
        if (file_name[0] != '\0') {
            reinterpret_cast<i32 (*)(char *, char *, i32)>(&NuFileExtConvert)(FS_LastFileName, file_name, 255);
        } else {
            FS_LastFileName[0] = '\0';
        }
    }

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *)) {
    if (title != NULL)
        NuStrCpy(FS_Title, title);
    if (path != NULL)
        NuStrCpy(FS_Path, path);
    if (filter != NULL)
        NuStrCpy(FS_Filter, filter);
    else
        FS_Filter[0] = '\0';
    if (filter_out != NULL)
        NuStrCpy(FS_FilterOut, filter_out);
    else
        FS_FilterOut[0] = '\0';
    FS_Active = 1;
    FS_Callback = callback;
    FS_RefreshDir = 2;
}

}

eduiitem_s *eduiItemFileSelectorCreate(u32 data, eduiiattr_s *colours, EdUiItemCallback callback, char *text) {
    struct FileSelectorItem : eduiitem_s {
        i32 field_48;
        f32 field_4c;
        f32 field_50;
        f32 field_54;
        f32 field_58;
        f32 field_5c;
        EdUiItemCallback callback;
    };
    DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(FileSelectorItem) == 0x64, "file selector item ABI");

    auto *item = static_cast<FileSelectorItem *>(NU_ALLOC(sizeof(FileSelectorItem), 4, 1, "", 0));
    if (__builtin_expect(item != NULL, 1)) {
        memset(item, 0, sizeof(*item));
        item->type = 0x10;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->destroy = eduicbItemDestroy;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        item->field_4c = 0.5f;
        item->field_50 = 0.5f;
        item->field_54 = 180.0f;
        item->field_5c = 0.5f;
        item->field_58 = 1.0f;
        item->callback = callback;
    }
    return item;
}

extern "C" {
    void cbTriggerSubMenu(eduimenu_s *menu, eduiitem_s *item, u32) {
        eduimenu_s *submenu = static_cast<eduimenu_s *>(item->data_ptr);
        if (submenu != NULL) {
            eduiMenuAttach(menu, submenu);
            submenu->x = item->x + 10;
            submenu->y = MIN(50, PS2_REZ_H * 2 - submenu->height);
        }
        item->highlighted = 0;
    }

    void cbCancelSubMenuFromItem(eduimenu_s *menu, eduiitem_s *, u32) {
        eduiMenuDetach(menu);
    }

    void cbNearClipAtCursor(eduimenu_s *, eduiitem_s *item, u32) {
        near_clip_at_cursor = reinterpret_cast<const u8 *>(item)[0x11] & 1;
    }

extern "C" void edDrawCross(NUVEC *centre, u8 red, u8 green, u8 blue) {
    i32 colour = 0xff000000 | red | (green << 8) | (blue << 16);
    NuRndrLine3dDbg(centre->x - 0.5f, centre->y, centre->z, centre->x + 0.5f, centre->y, centre->z, colour);
    NuRndrLine3dDbg(centre->x, centre->y - 0.5f, centre->z, centre->x, centre->y + 0.5f, centre->z, colour);
    NuRndrLine3dDbg(centre->x, centre->y, centre->z - 0.5f, centre->x, centre->y, centre->z + 0.5f, colour);
}

}

void edDrawLine(nuvec_s *, nuvec_s *, unsigned char, unsigned char, unsigned char) {
}

extern "C" {
extern "C" void AddColourPick(eduimenu_s *parent, eduiitem_s *item, f32 *red, f32 *green, f32 *blue, u32 *rgba) {
    eduiMenuAttach(parent, colourmenu);
    colourmenu->x = (item != nullptr ? item->x : parent->x) + 10;
    i32 y = (item != nullptr ? item->y : parent->y) + 10;
    i32 bottom = PS2_REZ_H - 20 - colourmenu->height;
    colourmenu->y = y <= bottom ? y : bottom;
    cp_rgba = rgba;

    f32 red_value;
    f32 green_value;
    f32 blue_value;
    if (rgba != nullptr) {
        u32 colour = *rgba;
        red_value = static_cast<f32>(colour & 0xff) / 255.0f;
        green_value = static_cast<f32>((colour >> 8) & 0xff) / 255.0f;
        blue_value = static_cast<f32>((colour >> 16) & 0xff) / 255.0f;
        f32 alpha_value = static_cast<f32>(colour >> 24) / 255.0f;
        cpt_r = red_value;
        cpt_g = green_value;
        cpt_b = blue_value;
        cpt_a = alpha_value;
        cp_r = &cpt_r;
        cp_g = &cpt_g;
        cp_b = &cpt_b;
    } else {
        cp_r = red;
        cp_g = green;
        cp_b = blue;
        red_value = *red;
        green_value = *green;
        blue_value = *blue;
    }
    cp_a = &cpt_a;

    eduiItemColourPickSetRGB(cp_item, red_value, green_value, blue_value);
    char text[256];
    sprintf(text, "R:%1.2f G:%1.2f B:%1.2f", *cp_r, *cp_g, *cp_b);
    eduiItemSetText(cp_info, text);
}

extern "C" void CreateColourPicker(void) {
    static i32 initialised;
    const u32 colours[4] __attribute__((aligned(16))) = {0x80000000u, 0x800000ffu, 0x80808080u, 0x80f0f0f0u};
    if (initialised != 0)
        return;

    initialised = 1;
    colourmenu = eduiMenuCreate(200, 70, 180, 250, nullptr, cbCancelSubMenu, const_cast<char *>("Pick Colour"));
    if (colourmenu == nullptr)
        return;

    cp_item = static_cast<edui_colour_pick_s *>(
        eduiMenuAddItem(colourmenu, eduiItemColourPickCreate(0, colours, cbColourPickSel, const_cast<char *>(""))));
    cp_process = cp_item->process;
    cp_item->process = cbProcessColourPick;
    cp_info = eduiMenuAddItem(
        colourmenu, eduiItemSelCreate(1, colours, 0, 0, cbToggleIndicatorMode, const_cast<char *>("R:?? G:?? B:??")));
    cp_copy = eduiMenuAddItem(colourmenu,
                              eduiItemSelCreate(1, colours, 0, 0, cbCopy, const_cast<char *>("Copy To Clipboard")));
    cp_paste = eduiMenuAddItem(
        colourmenu, eduiItemSelCreate(1, colours, 0, 0, cbPaste, const_cast<char *>("Paste From Clipboard")));
}

extern "C" void RndrOSphere(NUVEC *centre, f32 radius, i32 colour, i32 segments, i32) {
    NUMTX_ALIGNED32 matrix = global_camera.mtx;
    matrix.m30 = centre->x;
    matrix.m31 = centre->y;
    matrix.m32 = centre->z;
    i32 step = 65536 / segments;
    NURND_VERTEX3D vertices[2];
    vertices[0].position.x = radius;
    vertices[0].position.y = 0.0f;
    vertices[0].position.z = 0.0f;
    vertices[0].colour = colour;
    vertices[1].colour = colour;
    NuVecRotateZ(&vertices[1].position, &vertices[0].position, step);
    for (i32 i = 0; i < segments; ++i) {
        NUVEC_ALIGNED16 start = vertices[0].position;
        NUVEC_ALIGNED16 end = vertices[1].position;
        NuVecMtxTransform(&start, &start, &matrix);
        NuVecMtxTransform(&end, &end, &matrix);
        NuRndrLine3dDbg(start.x, start.y, start.z, end.x, end.y, end.z, vertices[0].colour);
        vertices[0].position = vertices[1].position;
        NuVecRotateZ(&vertices[1].position, &vertices[1].position, step);
    }
}

extern "C" void RndrCircleXZ(NUVEC *centre, f32 radius, i32 colour, i32 segments) {
    NUMTX_ALIGNED32 matrix = numtx_identity;
    matrix.m30 = centre->x;
    matrix.m31 = centre->y;
    matrix.m32 = centre->z;
    i32 step = 65536 / segments;
    NURND_VERTEX3D vertices[2];
    vertices[0].position.x = radius;
    vertices[0].position.y = 0.0f;
    vertices[0].position.z = 0.0f;
    vertices[0].colour = colour;
    vertices[1].colour = colour;
    NuVecRotateY(&vertices[1].position, &vertices[0].position, step);
    for (i32 i = 0; i < segments; ++i) {
        NUVEC_ALIGNED16 start = vertices[0].position;
        NUVEC_ALIGNED16 end = vertices[1].position;
        NuVecMtxTransform(&start, &start, &matrix);
        NuVecMtxTransform(&end, &end, &matrix);
        NuRndrLine3dDbg(start.x, start.y, start.z, end.x, end.y, end.z, vertices[0].colour);
        vertices[0].position = vertices[1].position;
        NuVecRotateY(&vertices[1].position, &vertices[1].position, step);
    }
}

extern "C" void RndrOSquare(NUVEC *centre, f32 radius, i32 colour) {
    f32 corners[4][2] = {{-1.0f, -1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, 1.0f}};
    NUMTX_ALIGNED32 matrix = global_camera.mtx;
    matrix.m31 = centre->y;
    matrix.m30 = centre->x;
    matrix.m32 = centre->z;
    for (i32 remaining = 4; remaining > 0; --remaining) {
        const i32 i = 4 - remaining;
        NUVEC_ALIGNED16 start;
        NUVEC_ALIGNED16 end;
        if (i == 0) {
            start = NUVEC{-radius, -radius, 0.0f};
            end = NUVEC{radius, -radius, 0.0f};
        } else {
            const f32 start_y = corners[i][1] * radius;
            const f32 end_x = corners[(i + 1) & 3][0] * radius;
            const f32 end_y = corners[(i + 1) & 3][1] * radius;
            const f32 start_x = corners[i][0] * radius;
            start = NUVEC{start_x, start_y, 0.0f};
            end = NUVEC{end_x, end_y, 0.0f};
        }
        NuVecMtxTransform(&start, &start, &matrix);
        NuVecMtxTransform(&end, &end, &matrix);
        NuRndrLine3dDbg(start.x, start.y, start.z, end.x, end.y, end.z, colour);
    }
}

    void FlushMenuHighlights(eduimenu_s *menu) {
        for (eduiitem_s *item = menu->first; item != NULL; item = item->next) {
            if (item->type == 0) {
                item->highlighted = 0;
            }
        }
    }

}
