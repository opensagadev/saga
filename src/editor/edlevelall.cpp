// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "globals.h"
#include "gameapi/edtools/edcam.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/items/base/apiobject.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nufile/nufile.h"
#include <string.h>
#include <new>
#include <float.h>
#include <stdio.h>
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edgra.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/misc/utilities.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include <math.h>
#include <stdlib.h>
#include "gameframework/saveload.h"
#include <cstdio>
#include "legoapi/characters/core/players.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec4.h"
#include <stdarg.h>
#include <GLES2/gl2.h>
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/numath/nuang.h"
#include "legoapi/items/collect/torpedo.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/render/fx/edsplines.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nu3d/nupostparams.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/doors/door.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/menus/screens/movies.h"
#include "legoapi/misc/androidbatman.h"
#include "legoapi/render/core/gameliball.h"
#include <stddef.h>
#include "editor/edpath.h"
#include "nu2api/nucore/nulist.h"
#include "legoapi/items/base/animpacket.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "gameapi/ai/aisys/aipath.h"
#include "nu2api/nucore/numechptr.hpp"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/gizmo/base/GizBuildItObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizLeverObjectInterface.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizPanelObjectInterface.h"
#include "legoapi/gizmo/base/GizTurretObjectInterface.h"
#include "legoapi/gizmo/base/HatMachineObjectInterface.h"
#include "legoapi/gizmo/base/TeleportObjectInterface.h"
#include "nu2api/nu3d/nuhspecial.h"
#include "legoapi/world/levels/podrace.h"
#include <stdint.h>
#include "gameapi/ai/aisys/aiscript_types.h"
#include "editor/aieditor_state.h"
#include "nu2api/numath/nuplane.h"
#include "editor/aieditor_settings.h"
#include "editor/edpath_types.h"
#include <ctype.h>
#include <limits.h>
#include "nu2api/nusound/nusound.h"
#include "editor/edlevelall.h"
#include "gameapi/edtools/edtoolsall.h"
#include "nu2api/nu3d/nu3d_includes.h"
#if defined(__i386__) && defined(__SSE__)
#include <xmmintrin.h>
#endif
#if defined(__i386__) && defined(__SSE2__)
#include <emmintrin.h>
#endif

static void cbEdLevelSettingsMenu(eduimenu_s *, eduiitem_s *, u32);

#define EDLEVEL_ADD_ACTIVE_SCENE(menu, scene_index)                                                                    \
    do {                                                                                                               \
        LevelEditorScene *scene = theLevelEditor.GetEdScene(scene_index);                                              \
        if (scene != NULL && scene->active)                                                                            \
            eduiMenuAddItem(menu, eduiItemToggleCreate((scene_index) << 6, &EdLevelAttr, scene->editable, 1,           \
                                                       ClassEditor::cbEdFilterLED, scene->name));                      \
    } while (0)

static inline i32 get_class_object_attribute(EdClass *ed_class, void *object, EdRef *reference, i32 attribute, i32 type,
                                             void *data, i32 size) {
    if (reference != NULL) {
        if (reference->GetAttributeData(object, attribute, type, data, size)) {
            return 1;
        }
    }
    EdMember member;
    if (ed_class->FindMember(&member, object, attribute, 1)) {
        return member.reference->GetAttributeData(member.object, attribute, type, data, size) != 0;
    }
    return 0;
}

ClassEditor theClassEditor;
CursorTool theCursorTool;
static i32 class_editor_create_pending = -1;

i32 BaseEditor::blockDepth;
i32 BaseEditor::blockStart[8];
eduimenu_s *edLevelActiveMenu;
i32 edLevelDestroyActiveMenu;
eduimenu_s *edLevelDestroyThisMenu;
eduimenu_s *edLevelDestroyThisMenu2;
i32 edlevel_mouseandkeyboard;
LevelEditor theLevelEditor;

EdManipulator theDefaultManipulator;
EdManMove theMoveManipulator;
EdManRotate theRotateManipulator;
EdManScale theScaleManipulator;
PlaceableHelper thePlaceableHelper;
PropertyTool thePropertyTool;
EdRegistry theRegistry;
SceneObjectHelper theSceneObjectHelper;
SplineHelper theSplineHelper;
KnotHelper theKnotHelper;
SplineTool theSplineTool;
EdSystem theEdSystem;
PropertyMenuMetrics menu_startmetrics __attribute__((aligned(16))) = {20, 5, 200, 400};
eduiiattr_s EdLevelAttr = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
i32 EdLevelFnt;
i32 EdLevelFntScale = 24;
NUMTL *edLevel2dMtl;
NUMTL *edLevel3dMtl;
NUMTL *edLevelTexMtl;

template <typename T> T *CreateObject(MemoryBuffer *buffer) {
    void *storage = buffer->Allocate(sizeof(T));
    if (storage == NULL)
        return NULL;
    return new (storage) T();
}

eduimenu_s *edLevelNextMenu;

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" VuVec SpherePos1;
extern "C" VuVec SpherePos2;
VuVec SpherePos1;
VuVec SpherePos2;
extern EdInputContext * Input;
extern "C" eduiitem_s *eduiItemColourPickCreate(usize, const void *, EdUiItemCallback, char *);
i32 EdType_Char;
i32 EdType_Short;
i32 EdType_Int;
i32 EdType_Float;
i32 EdType_VuVec;
i32 EdType_VuMtx;
i32 EdType_Enumeration;
i32 EdType_String;
i32 EdType_Colour3;
i32 EdType_NuHSpecial;
i32 EdType_NuVec;
i32 EdType_NuMtx;

f32 EdManipulator::Scale = 2.0f;
i32 pad_disabled;
eduimenu_s *edLevelPinnedMenu;

NUMTL *EdDrawMtl[2];
static i32 NewPrim;
static i32 NewMtl;
static const VuMtx *NewMtx;

template <i32 Axis> static inline void EdDrawCircle(VuVec const &centre, float radius, i32 colour, i32 segments) {
    VuVec base(0.0f, Axis == 2 ? radius : 0.0f, Axis == 2 ? 0.0f : radius, 0.0f);
    VuVec point(base.x + centre.x, base.y + centre.y, base.z + centre.z, 0.0f);
    for (i32 i = 1; i <= segments; i++) {
        VuVec previous(point.x, point.y, point.z, point.w);
        i32 angle = i * (0x10000 / segments);
        if (Axis == 0) {
            NuVecRotateX(&point.xyz, &base.xyz, angle);
        } else if (Axis == 1) {
            NuVecRotateY(&point.xyz, &base.xyz, angle);
        } else {
            NuVecRotateZ(&point.xyz, &base.xyz, angle);
        }
        point = VuVec(point.x + centre.x, point.y + centre.y, point.z + centre.z, 0.0f);
        EdDrawLineSegment(previous, point, colour);
    }
}

__attribute__((force_align_arg_pointer)) void EdDrawPolyCylinder(VuVec const &start, VuVec const &end, i32 sides,
                                                                 i32 colour, i32 cap_colour, float radius, float limit,
                                                                 float offset);

static i32 rotation_axis = 1;
static EdBitControl *edBitControl;
static edui_prop_s *edBitItem;
static i32 edBitIndex;
extern EdEnumControl::Item YesNoItems[3];
static EdEnumControl *active_enum_control;

i32 EdManipulator::AxisColour[8] = {
    static_cast<i32>(0xffffffff), static_cast<i32>(0xff0000ff), static_cast<i32>(0xff00ff00),
    static_cast<i32>(0xffff0000), static_cast<i32>(0xff00ffff), static_cast<i32>(0xffff00ff),
    static_cast<i32>(0xffffff00), static_cast<i32>(0xffffffff),
};

static EdColourControl *edColourControl;

static inline void set_colour_preview(eduiitem_s *item, const NUVEC &colour) {
    u32 red_alpha = 0xff000000 | (static_cast<i32>(colour.x * 255.0f) & 0xff);
    u32 green_blue = ((static_cast<u32>(static_cast<i32>(colour.y * 255.0f)) << 8) & 0xffff) |
                     ((static_cast<i32>(colour.z * 255.0f) & 0xff) << 16);
    u32 value = red_alpha | green_blue;
    *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(item) + 0x50) = value;
}

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

EdSfxNameControl *sfxNameControl;

static EdSpecialObjectControl *active_special_object_control;

static i32 SpecialObjectFilter(void *object) {
    return theSceneObjectHelper.scene_id == static_cast<Placeable *>(object)->scene_id;
}

static EdClassObjectNameControl *edClassObjectNameControl;

inline EdClassObjectNameControl::~EdClassObjectNameControl() {
}

inline void EdClassObjectNameControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdClassObjectNameControl));
}

inline EdSubSystem::~EdSubSystem() {
}

SAGA_TARGET_INLINE_COPY(inline EdControl::~EdControl() {})

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

static i32 get_manipulator_attribute(ClassObjectListEntry *entry, i32 attribute, i32 type, void *data) {
    if (entry->reference != NULL && entry->reference->GetAttributeData(entry->object, attribute, type, data, 0))
        return 1;
    EdMember member;
    return entry->ed_class->FindMember(&member, entry->object, attribute, 1) &&
           member.reference->GetAttributeData(member.object, attribute, type, data, 0);
}

static void set_manipulator_attribute(ClassObjectListEntry *entry, i32 attribute, i32 type, void *data) {
    if (entry->reference != NULL && entry->reference->SetAttributeData(entry->object, attribute, type, data, 0))
        return;
    EdMember member;
    if (entry->ed_class->FindMember(&member, entry->object, attribute, 1))
        member.reference->SetAttributeData(member.object, attribute, type, data, 0);
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

// Only caller prints the property edit buffer left-aligned.
static __attribute__((noinline, noclone)) void eduiFntPrintExEditBuffer(void *font, float x, float y, int alignment,
                                                                        char *format, ...)
    __asm__("_ZL14eduiFntPrintExPvffiPcz.constprop.66");

static __used__ void eduiFntPrintClipEx(void *font, float x, float y, int alignment, float clip_x, float clip_width,
                                        char *format, ...);
static __attribute__((noinline, noclone)) void eduiFntPrintClipEx(void *font, i32 x, i32 y, i32 alignment, i32 clip_x,
                                                                  i32 clip_width, char *format, ...)
    __asm__("_ZL18eduiFntPrintClipExPviiiiiPcz.constprop.70");

extern "C" {
    static __attribute__((noinline, noclone)) void
    eduicbAttachDirectoryList(eduimenu_s *, edui_file_pick_s *) __asm__("eduicbAttachDirectoryList.isra.32");
    static __attribute__((noinline, noclone)) i32
    eduicbProcessPropKeyboard(eduimenu_s *, edui_prop_s *) __asm__("eduicbProcessPropKeyboard.isra.27");

}

extern "C" {
eduiitem_s *eduiItemColourPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

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
    static void eduiDrawHueValueBand(i32 x, i32 y, i32 width, i32 height, f32 start, f32 end, u32 first, u32 second)
        __asm__("_ZL19eduiDrawGrad4StepsViiiiffiiiii.constprop.59");
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

#include "nu2api/nucore/nucore_plain_types.h"

    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    extern "C++" {
        static __attribute__((noinline, noclone)) void
        ClipInstTree(NuInstanceTree_s *tree,
                     i32 tree_index) __asm__("_ZL12ClipInstTreeP16NuInstanceTree_siP16nudisplayscene_sPc.isra.92");
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
union variptr_u;

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

static __attribute__((noinline, noclone)) void RenderFileSel3Body(i32 draw_title)
    __asm__("_Z14RenderFileSel3i.part.38");

struct nuhspecial_s;
struct VuVec;

#if defined(__i386__)
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone))
#endif

extern "C" {
eduiitem_s *eduiItemColourPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

}

i16 Placeable::CurrentLedFile;
static EdStringControl *edObjectNameControl;

template <typename Ref>
static __attribute__((always_inline)) void add_placeable_reference(EdClass *object_class, char *type, char *name,
                                                                   i32 offset, i32 size, i32 attributes,
                                                                   EdControl *control = NULL) {
    void *memory = theMemoryManager.AllocPool(sizeof(Ref), 1);
    Ref *reference = new (memory) Ref(type, name, offset, size, attributes, control, 0);
    object_class->AddType(reference);
}

inline PlaceableNameControl::~PlaceableNameControl() {
}

inline void PlaceableNameControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(PlaceableNameControl));
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

struct GameObject_s;
struct nuvec_s;
struct WORLDINFO_s;

extern i32 LevFlag[4];

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

namespace {

} // namespace

struct EDCREATURE_s;

struct AISYS_s;

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

static NU_READ_GRAPHICS_DATA_ATTR NUGSCN *NuReadGraphicsData(VARIPTR *buf, VARIPTR *buf_end, char *path,
                                                             char *scene_data)
    __asm__("_ZL18NuReadGraphicsDataP9variptr_uS0_PcS1_S1_.isra.104");

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

namespace {

} // namespace

struct FLOWBOX_s;

struct JEDIBNETPACKET_s;

extern AIACTIONDEF api_aiactiondefs[55];
extern AICONDITIONDEF api_aiconditiondefs[50];

DECOMP_ASSERT(sizeof(api_aiactiondefs) == 0x294, "API action registry size");

DECOMP_ASSERT(sizeof(api_aiconditiondefs) == 0x258, "API condition registry size");

struct EDAISHAREDPATHNODE_s;
struct eduimenu_s;
struct eduiitem_s;

static __used__ void pathEditor_cbCancelRenamePathMenu(eduimenu_s *, eduimenu_s *);
static __used__ void pathEditor_cbCancelRenameNodeMenu(eduimenu_s *, eduimenu_s *);
static __used__ void pathEditor_cbCancelSelectMenu(eduimenu_s *, eduimenu_s *);
static __used__ void pathEditor_cbSetCurrentPath(eduimenu_s *, eduiitem_s *, u32);
static __used__ void pathEditor_cbSetCurrentPath(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void pathEditor_cbCancelSelectMenu(eduimenu_s *menu, eduimenu_s *);
static __used__ void pathEditor_cbCancelRenameNodeMenu(eduimenu_s *menu, eduimenu_s *);
static __used__ void pathEditor_cbCancelRenamePathMenu(eduimenu_s *menu, eduimenu_s *);
static __used__ void routeEditor_cbCancelRouteUsers(eduimenu_s *, eduimenu_s *);
static __used__ void routeEditor_cbSetRouteUsers(eduimenu_s *, eduiitem_s *, u32);
static __used__ __attribute__((optimize("peel-loops"))) void routeEditor_cbRenameRoute(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void routeEditor_cbCancelRenameRouteMenu(eduimenu_s *, eduimenu_s *);
static __used__ void routeEditor_cbSetRouteUsers(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void routeEditor_cbCancelRouteUsers(eduimenu_s *menu, eduimenu_s *);
static __used__ void routeEditor_cbCancelRenameRouteMenu(eduimenu_s *menu, eduimenu_s *);

static __used__ i32 routeEditor_AddToRoute(EDAIPATHNODE_s *node, EDAIPATHNODE_s *other) __asm__(
    "_ZL22routeEditor_AddToRouteP14EDAIPATHNODE_sS0_.part.7");

static __used__ i32 routeEditor_AddToRoute(EDAIPATHNODE_s *node, EDAIPATHNODE_s *other);

#pragma GCC optimize("O2", "omit-frame-pointer")

#if defined(__i386__)
#define CREATURE_EDITOR_REGPARM1 __attribute__((regparm(1)))
#else
#define CREATURE_EDITOR_REGPARM1
#endif
static __used__
    __attribute__((always_inline)) CREATURE_EDITOR_REGPARM1 void creatureEditor_Updated(EDCREATURE_s *creature);

static __used__ __attribute__((force_align_arg_pointer)) void creatureEditor_cbResetParams(eduimenu_s *menu,
                                                                                           eduiitem_s *, unsigned int);

static __used__ __attribute__((force_align_arg_pointer)) void creatureEditor_cbVisionMenu(eduimenu_s *parent,
                                                                                          eduiitem_s *, unsigned int);

static __used__ __attribute__((force_align_arg_pointer)) void creatureEditor_cbSetScript(eduimenu_s *, eduiitem_s *item,
                                                                                         unsigned int);

extern "C" {
    struct EDCREATURE_s;
}

#if defined(__i386__)
static void __attribute__((regparm(1))) DestroyLocator(EDLOCATOR_s *);
static unsigned int __attribute__((regparm(2))) AddLocatorToSet(EDLOCATORSET_s *, EDLOCATOR_s *, EDLOCATOR_s *);
#else
static void DestroyLocator(EDLOCATOR_s *);
static unsigned int AddLocatorToSet(EDLOCATORSET_s *, EDLOCATOR_s *, EDLOCATOR_s *);
#endif
static __used__ void locatorEditor_cbAddLocatorsByNameYesNo(eduimenu_s *, eduiitem_s *, u32);

static __used__ __attribute__((optimize("no-tree-vectorize"))) void
locatorEditor_cbAddLocatorsByNameYesNo(eduimenu_s *, eduiitem_s *item, u32);
static __used__ __attribute__((optimize("no-tree-vectorize"))) void DestroyLocator(EDLOCATOR_s *locator);

static __used__ unsigned int AddLocatorToSet(EDLOCATORSET_s *set, EDLOCATOR_s *locator, EDLOCATOR_s *before);

#if defined(__i386__)
static __attribute__((noinline, optimize("no-tree-vectorize"), regparm(2))) unsigned int
#else
static __attribute__((noinline, optimize("no-tree-vectorize"))) unsigned int
#endif
AddLocatorToSetAtEnd(EDLOCATORSET_s *set, EDLOCATOR_s *locator) __asm__(
    "_ZL15AddLocatorToSetP14EDLOCATORSET_sP11EDLOCATOR_sS2_.constprop.131");

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

namespace {

} // namespace

struct AIPACKET_s;

struct GIZMOPICKUP_s;
struct PART_s;

// Original writable flagremaptab at 0x668520, 32 pairs of 32-bit masks.
extern "C" {
extern u32 flagremaptab[32][2];
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

#if defined(__SSE__) && !defined(__EMSCRIPTEN__)
#include <xmmintrin.h>
#endif

inline void SceneInstance::operator delete(void *pointer) {
    theMemoryManager.FreePool(pointer, sizeof(SceneInstance));
}

char const *SceneInstance::GetName() const {
    return name.data != NULL ? name.data + 1 : NULL;
}

void SceneInstance::SetName(char const *value) {
    name.Set(value);
}

// The original helper has an EdClassInterface primary vtable and an EdSubSystem
// secondary vtable at +0xc. The field-based types retain those exact layouts.
#define SCENE_VIRTUAL(name, symbol) extern "C" void name() asm(symbol)
#define SCENE_SLOT(field, name) reinterpret_cast<decltype(EdClassInterfaceVTable::field)>(name)
SCENE_VIRTUAL(base_destroy, "_ZN16EdClassInterfaceD1Ev");
SCENE_VIRTUAL(base_delete, "_ZN16EdClassInterfaceD0Ev");
SCENE_VIRTUAL(base_clear, "_ZN16EdClassInterface10ClearLevelEi");
SCENE_VIRTUAL(base_next_filtered, "_ZN16EdClassInterface13GetNextObjectEPvPFiS0_E");
SCENE_VIRTUAL(base_defunct, "_ZN16EdClassInterface13DefunctObjectEPv");
SCENE_VIRTUAL(base_revive, "_ZN16EdClassInterface12ReviveObjectEPv");
SCENE_VIRTUAL(base_set_guid, "_ZN16EdClassInterface13SetObjectGuidEPvi");
SCENE_VIRTUAL(base_get_guid, "_ZN16EdClassInterface13GetObjectGuidEPv");
SCENE_VIRTUAL(base_constructor_data, "_ZN16EdClassInterface18GetConstructorDataEPvS0_i");
SCENE_VIRTUAL(base_construct, "_ZN16EdClassInterface9ConstructEPvS0_");
SCENE_VIRTUAL(base_process, "_ZN16EdClassInterface7ProcessEPvR14EdInputContext");
SCENE_VIRTUAL(base_render, "_ZN16EdClassInterface6RenderEPvi");
SCENE_VIRTUAL(base_enter_editor, "_ZN16EdClassInterface11EnterEditorEv");
SCENE_VIRTUAL(base_exit_editor, "_ZN16EdClassInterface10ExitEditorEv");
SCENE_VIRTUAL(base_enter_level, "_ZN16EdClassInterface10EnterLevelEv");
SCENE_VIRTUAL(base_exit_level, "_ZN16EdClassInterface9ExitLevelEv");
SCENE_VIRTUAL(base_update_lists, "_ZN16EdClassInterface11UpdateListsEP12MemoryBufferS1_");
SCENE_VIRTUAL(base_pre_load, "_ZN16EdClassInterface21PreLoadInitialisationEP12MemoryBufferS1_");
SCENE_VIRTUAL(base_post_load, "_ZN16EdClassInterface22PostLoadInitialisationEP12MemoryBufferS1_");
SCENE_VIRTUAL(base_pre_save, "_ZN16EdClassInterface21PreSaveInitialisationEv");
SCENE_VIRTUAL(base_post_save, "_ZN16EdClassInterface22PostSaveInitialisationEv");
SCENE_VIRTUAL(base_serialise, "_ZN16EdClassInterface15SerialiseObjectER8EdStreamPv");
SCENE_VIRTUAL(base_distance_ray, "_ZN16EdClassInterface16DistanceToObjectER5VuVecS1_PvPP5EdRef");
SCENE_VIRTUAL(base_distance_point, "_ZN16EdClassInterface16DistanceToObjectER5VuVecPvPP5EdRef");
SCENE_VIRTUAL(base_add_menu, "_ZN16EdClassInterface12AddMenuItemsEP10eduimenu_s");
SCENE_VIRTUAL(base_import, "_ZN16EdClassInterface6ImportEv");
SCENE_VIRTUAL(scene_destroy, "_ZN17SceneObjectHelperD1Ev");
SCENE_VIRTUAL(scene_delete, "_ZN17SceneObjectHelperD0Ev");
SCENE_VIRTUAL(scene_clear, "_ZN17SceneObjectHelper10ClearLevelEi");
SCENE_VIRTUAL(scene_flush, "_ZN17SceneObjectHelper5FlushEv");
SCENE_VIRTUAL(scene_count, "_ZN17SceneObjectHelper13GetNumObjectsEv");
SCENE_VIRTUAL(scene_next, "_ZN17SceneObjectHelper13GetNextObjectEPv");
SCENE_VIRTUAL(scene_create, "_ZN17SceneObjectHelper12CreateObjectEPvii");
SCENE_VIRTUAL(scene_destroy_object, "_ZN17SceneObjectHelper13DestroyObjectEPvi");
SCENE_VIRTUAL(scene_update_lists, "_ZN17SceneObjectHelper11UpdateListsEP12MemoryBufferS1_");
SCENE_VIRTUAL(scene_pre_load, "_ZN17SceneObjectHelper21PreLoadInitialisationEP12MemoryBufferS1_");
SCENE_VIRTUAL(scene_post_load, "_ZN17SceneObjectHelper22PostLoadInitialisationEP12MemoryBufferS1_");
SCENE_VIRTUAL(scene_add_menu, "_ZN17SceneObjectHelper12AddMenuItemsEP10eduimenu_s");
SCENE_VIRTUAL(scene_sub_render, "_ZN17SceneObjectHelper9SubRenderEv");
SCENE_VIRTUAL(scene_sub_destroy, "_ZThn12_N17SceneObjectHelperD1Ev");
SCENE_VIRTUAL(scene_sub_delete, "_ZThn12_N17SceneObjectHelperD0Ev");
SCENE_VIRTUAL(sub_initialise, "_ZN11EdSubSystem13SubInitialiseER9variptr_uS1_i");
SCENE_VIRTUAL(sub_reset, "_ZN11EdSubSystem8SubResetEv");
SCENE_VIRTUAL(sub_process, "_ZN11EdSubSystem10SubProcessEf");
SCENE_VIRTUAL(scene_sub_render_thunk, "_ZThn12_N17SceneObjectHelper9SubRenderEv");

extern const EdClassInterfaceVTableObject sceneBaseInterfaceVTable asm("_ZTV16EdClassInterface")
    __attribute__((weak)) = {NULL,
                             NULL,
                             {SCENE_SLOT(destroy, base_destroy),
                              SCENE_SLOT(delete_object, base_delete),
                              SCENE_SLOT(clear_level, base_clear),
                              NULL,
                              NULL,
                              NULL,
                              SCENE_SLOT(get_next_filtered_object, base_next_filtered),
                              NULL,
                              NULL,
                              SCENE_SLOT(defunct_object, base_defunct),
                              SCENE_SLOT(revive_object, base_revive),
                              SCENE_SLOT(set_object_guid, base_set_guid),
                              SCENE_SLOT(get_object_guid, base_get_guid),
                              SCENE_SLOT(get_constructor_data, base_constructor_data),
                              SCENE_SLOT(construct, base_construct),
                              SCENE_SLOT(process, base_process),
                              SCENE_SLOT(render, base_render),
                              SCENE_SLOT(enter_editor, base_enter_editor),
                              SCENE_SLOT(exit_editor, base_exit_editor),
                              SCENE_SLOT(enter_level, base_enter_level),
                              SCENE_SLOT(exit_level, base_exit_level),
                              SCENE_SLOT(update_lists, base_update_lists),
                              SCENE_SLOT(pre_load_initialisation, base_pre_load),
                              SCENE_SLOT(post_load_initialisation, base_post_load),
                              SCENE_SLOT(pre_save_initialisation, base_pre_save),
                              SCENE_SLOT(post_save_initialisation, base_post_save),
                              SCENE_SLOT(serialise_object, base_serialise),
                              SCENE_SLOT(distance_to_ray, base_distance_ray),
                              SCENE_SLOT(distance_to_point, base_distance_point),
                              SCENE_SLOT(add_menu_items, base_add_menu),
                              SCENE_SLOT(import, base_import)}};

extern const SceneObjectHelperVTableObject sceneObjectHelperVTable asm("_ZTV17SceneObjectHelper")
    __attribute__((weak)) = {
        NULL,
        NULL,
        {SCENE_SLOT(destroy, scene_destroy),
         SCENE_SLOT(delete_object, scene_delete),
         SCENE_SLOT(clear_level, scene_clear),
         SCENE_SLOT(flush, scene_flush),
         SCENE_SLOT(get_num_objects, scene_count),
         SCENE_SLOT(get_next_object, scene_next),
         SCENE_SLOT(get_next_filtered_object, base_next_filtered),
         SCENE_SLOT(create_object, scene_create),
         SCENE_SLOT(destroy_object, scene_destroy_object),
         SCENE_SLOT(defunct_object, base_defunct),
         SCENE_SLOT(revive_object, base_revive),
         SCENE_SLOT(set_object_guid, base_set_guid),
         SCENE_SLOT(get_object_guid, base_get_guid),
         SCENE_SLOT(get_constructor_data, base_constructor_data),
         SCENE_SLOT(construct, base_construct),
         SCENE_SLOT(process, base_process),
         SCENE_SLOT(render, base_render),
         SCENE_SLOT(enter_editor, base_enter_editor),
         SCENE_SLOT(exit_editor, base_exit_editor),
         SCENE_SLOT(enter_level, base_enter_level),
         SCENE_SLOT(exit_level, base_exit_level),
         SCENE_SLOT(update_lists, scene_update_lists),
         SCENE_SLOT(pre_load_initialisation, scene_pre_load),
         SCENE_SLOT(post_load_initialisation, scene_post_load),
         SCENE_SLOT(pre_save_initialisation, base_pre_save),
         SCENE_SLOT(post_save_initialisation, base_post_save),
         SCENE_SLOT(serialise_object, base_serialise),
         SCENE_SLOT(distance_to_ray, base_distance_ray),
         SCENE_SLOT(distance_to_point, base_distance_point),
         SCENE_SLOT(add_menu_items, scene_add_menu),
         SCENE_SLOT(import, base_import)},
        reinterpret_cast<decltype(SceneObjectHelperVTableObject::sub_render)>(scene_sub_render),
        -12,
        NULL,
        {scene_sub_destroy, scene_sub_delete, sub_initialise, sub_reset, sub_process, scene_sub_render_thunk}};

struct EdSubSystemVTableObject {
    void *offset_to_top;
    void *type_info;
    void (*methods[6])();
};
extern const EdSubSystemVTableObject edSubSystemVTable asm("_ZTV11EdSubSystem");

extern "C" void baseInterfaceDestructor(EdClassInterface *object) asm("_ZN16EdClassInterfaceD1Ev")
    __attribute__((weak));
void baseInterfaceDestructor(EdClassInterface *object) {
    object->vtable = const_cast<EdClassInterfaceVTable *>(&sceneBaseInterfaceVTable.methods);
}
extern "C" void baseInterfaceCompleteDestructor(EdClassInterface *) asm("_ZN16EdClassInterfaceD2Ev")
    __attribute__((weak, alias("_ZN16EdClassInterfaceD1Ev")));
extern "C" void baseInterfaceDeletingDestructor(EdClassInterface *object) asm("_ZN16EdClassInterfaceD0Ev")
    __attribute__((weak));
void baseInterfaceDeletingDestructor(EdClassInterface *object) {
    object->vtable = const_cast<EdClassInterfaceVTable *>(&sceneBaseInterfaceVTable.methods);
    ::operator delete(object);
}

extern "C" void sceneHelperDestructor(SceneObjectHelper *object) asm("_ZN17SceneObjectHelperD1Ev")
    __attribute__((weak));
void sceneHelperDestructor(SceneObjectHelper *object) {
    *reinterpret_cast<void **>(&object->subsystem) =
        const_cast<void *>(static_cast<void const *>(&edSubSystemVTable.methods));
    object->class_interface.vtable = const_cast<EdClassInterfaceVTable *>(&sceneBaseInterfaceVTable.methods);
}
extern "C" void sceneHelperCompleteDestructor(SceneObjectHelper *) asm("_ZN17SceneObjectHelperD2Ev")
    __attribute__((weak, alias("_ZN17SceneObjectHelperD1Ev")));
extern "C" void sceneHelperDeletingDestructor(SceneObjectHelper *object) asm("_ZN17SceneObjectHelperD0Ev")
    __attribute__((weak));
void sceneHelperDeletingDestructor(SceneObjectHelper *object) {
    *reinterpret_cast<void **>(&object->subsystem) =
        const_cast<void *>(static_cast<void const *>(&edSubSystemVTable.methods));
    object->class_interface.vtable = const_cast<EdClassInterfaceVTable *>(&sceneBaseInterfaceVTable.methods);
}
extern "C" void sceneHelperSubDestructor(EdSubSystem *object) asm("_ZThn12_N17SceneObjectHelperD1Ev")
    __attribute__((weak));
void sceneHelperSubDestructor(EdSubSystem *object) {
    sceneHelperDestructor(reinterpret_cast<SceneObjectHelper *>(reinterpret_cast<u8 *>(object) - 12));
}
extern "C" void sceneHelperSubDeletingDestructor(EdSubSystem *object) asm("_ZThn12_N17SceneObjectHelperD0Ev")
    __attribute__((weak));
void sceneHelperSubDeletingDestructor(EdSubSystem *object) {
    sceneHelperDeletingDestructor(reinterpret_cast<SceneObjectHelper *>(reinterpret_cast<u8 *>(object) - 12));
}

template <typename Ref>
static void add_scene_reference(EdClass *object_class, char *type, char *name, i32 offset, i32 size, i32 attributes,
                                EdControl *control = NULL) {
    void *memory = theMemoryManager.AllocPool(sizeof(Ref), 1);
    Ref *reference = new (memory) Ref(type, name, offset, size, attributes, control, 0);
    object_class->AddType(reference);
}

extern "C" void sceneHelperSubRender(EdSubSystem *object) asm("_ZThn12_N17SceneObjectHelper9SubRenderEv");

inline void SceneObject::operator delete(void *pointer) {
    theMemoryManager.FreePool(pointer, sizeof(SceneObject));
}

// EdClassInterface's field-based representation preserves the original ABI for
// the rest of the editor.  These two objects are its original Itanium vtables:
// two header words followed by the 31 EdClassInterface slots.
#define EDVT_SYMBOL(name, symbol) extern "C" void name() asm(symbol)
#define EDVT_SLOT(field, symbol) reinterpret_cast<decltype(EdClassInterfaceVTable::field)>(symbol)
EDVT_SYMBOL(edvt_base_clear, "_ZN16EdClassInterface10ClearLevelEi");
EDVT_SYMBOL(edvt_base_next_filtered, "_ZN16EdClassInterface13GetNextObjectEPvPFiS0_E");
EDVT_SYMBOL(edvt_base_defunct, "_ZN16EdClassInterface13DefunctObjectEPv");
EDVT_SYMBOL(edvt_base_revive, "_ZN16EdClassInterface12ReviveObjectEPv");
EDVT_SYMBOL(edvt_base_set_guid, "_ZN16EdClassInterface13SetObjectGuidEPvi");
EDVT_SYMBOL(edvt_base_get_guid, "_ZN16EdClassInterface13GetObjectGuidEPv");
EDVT_SYMBOL(edvt_base_constructor_data, "_ZN16EdClassInterface18GetConstructorDataEPvS0_i");
EDVT_SYMBOL(edvt_base_construct, "_ZN16EdClassInterface9ConstructEPvS0_");
EDVT_SYMBOL(edvt_base_enter_editor, "_ZN16EdClassInterface11EnterEditorEv");
EDVT_SYMBOL(edvt_base_exit_editor, "_ZN16EdClassInterface10ExitEditorEv");
EDVT_SYMBOL(edvt_base_enter_level, "_ZN16EdClassInterface10EnterLevelEv");
EDVT_SYMBOL(edvt_base_exit_level, "_ZN16EdClassInterface9ExitLevelEv");
EDVT_SYMBOL(edvt_base_update_lists, "_ZN16EdClassInterface11UpdateListsEP12MemoryBufferS1_");
EDVT_SYMBOL(edvt_base_pre_load, "_ZN16EdClassInterface21PreLoadInitialisationEP12MemoryBufferS1_");
EDVT_SYMBOL(edvt_base_post_load, "_ZN16EdClassInterface22PostLoadInitialisationEP12MemoryBufferS1_");
EDVT_SYMBOL(edvt_base_pre_save, "_ZN16EdClassInterface21PreSaveInitialisationEv");
EDVT_SYMBOL(edvt_base_post_save, "_ZN16EdClassInterface22PostSaveInitialisationEv");
EDVT_SYMBOL(edvt_base_serialise, "_ZN16EdClassInterface15SerialiseObjectER8EdStreamPv");
EDVT_SYMBOL(edvt_base_distance_ray, "_ZN16EdClassInterface16DistanceToObjectER5VuVecS1_PvPP5EdRef");
EDVT_SYMBOL(edvt_base_distance_point, "_ZN16EdClassInterface16DistanceToObjectER5VuVecPvPP5EdRef");
EDVT_SYMBOL(edvt_base_add_menu, "_ZN16EdClassInterface12AddMenuItemsEP10eduimenu_s");
EDVT_SYMBOL(edvt_base_import, "_ZN16EdClassInterface6ImportEv");
EDVT_SYMBOL(edvt_spline_clear, "_ZN12SplineHelper10ClearLevelEi");
EDVT_SYMBOL(edvt_spline_flush, "_ZN12SplineHelper5FlushEv");
EDVT_SYMBOL(edvt_spline_count, "_ZN12SplineHelper13GetNumObjectsEv");
EDVT_SYMBOL(edvt_spline_next, "_ZN12SplineHelper13GetNextObjectEPv");
EDVT_SYMBOL(edvt_spline_create, "_ZN12SplineHelper12CreateObjectEPvii");
EDVT_SYMBOL(edvt_spline_destroy, "_ZN12SplineHelper13DestroyObjectEPvi");
EDVT_SYMBOL(edvt_spline_process, "_ZN12SplineHelper7ProcessEPvR14EdInputContext");
EDVT_SYMBOL(edvt_spline_render, "_ZN12SplineHelper6RenderEPvi");
EDVT_SYMBOL(edvt_spline_pre_load, "_ZN12SplineHelper21PreLoadInitialisationEP12MemoryBufferS1_");
EDVT_SYMBOL(edvt_spline_post_load, "_ZN12SplineHelper22PostLoadInitialisationEP12MemoryBufferS1_");
EDVT_SYMBOL(edvt_spline_serialise, "_ZN12SplineHelper15SerialiseObjectER8EdStreamPv");
EDVT_SYMBOL(edvt_spline_add_menu, "_ZN12SplineHelper12AddMenuItemsEP10eduimenu_s");
EDVT_SYMBOL(edvt_knot_flush, "_ZN10KnotHelper5FlushEv");
EDVT_SYMBOL(edvt_knot_count, "_ZN10KnotHelper13GetNumObjectsEv");
EDVT_SYMBOL(edvt_knot_next, "_ZN10KnotHelper13GetNextObjectEPv");
EDVT_SYMBOL(edvt_knot_create, "_ZN10KnotHelper12CreateObjectEPvii");
EDVT_SYMBOL(edvt_knot_destroy, "_ZN10KnotHelper13DestroyObjectEPvi");
EDVT_SYMBOL(edvt_knot_process, "_ZN10KnotHelper7ProcessEPvR14EdInputContext");
EDVT_SYMBOL(edvt_knot_render, "_ZN10KnotHelper6RenderEPvi");
EDVT_SYMBOL(edvt_knot_distance_ray, "_ZN10KnotHelper16DistanceToObjectER5VuVecS1_PvPP5EdRef");
EDVT_SYMBOL(edvt_spline_destructor, "_ZN12SplineHelperD1Ev");
EDVT_SYMBOL(edvt_spline_delete, "_ZN12SplineHelperD0Ev");
EDVT_SYMBOL(edvt_knot_destructor, "_ZN10KnotHelperD1Ev");
EDVT_SYMBOL(edvt_knot_delete, "_ZN10KnotHelperD0Ev");

extern const EdClassInterfaceVTableObject splineHelperVTable asm("_ZTV12SplineHelper")
    __attribute__((weak)) = {NULL,
                             NULL,
                             {reinterpret_cast<decltype(EdClassInterfaceVTable::destroy)>(edvt_spline_destructor),
                              reinterpret_cast<decltype(EdClassInterfaceVTable::delete_object)>(edvt_spline_delete),
                              EDVT_SLOT(clear_level, edvt_spline_clear),
                              EDVT_SLOT(flush, edvt_spline_flush),
                              EDVT_SLOT(get_num_objects, edvt_spline_count),
                              EDVT_SLOT(get_next_object, edvt_spline_next),
                              EDVT_SLOT(get_next_filtered_object, edvt_base_next_filtered),
                              EDVT_SLOT(create_object, edvt_spline_create),
                              EDVT_SLOT(destroy_object, edvt_spline_destroy),
                              EDVT_SLOT(defunct_object, edvt_base_defunct),
                              EDVT_SLOT(revive_object, edvt_base_revive),
                              EDVT_SLOT(set_object_guid, edvt_base_set_guid),
                              EDVT_SLOT(get_object_guid, edvt_base_get_guid),
                              EDVT_SLOT(get_constructor_data, edvt_base_constructor_data),
                              EDVT_SLOT(construct, edvt_base_construct),
                              EDVT_SLOT(process, edvt_spline_process),
                              EDVT_SLOT(render, edvt_spline_render),
                              EDVT_SLOT(enter_editor, edvt_base_enter_editor),
                              EDVT_SLOT(exit_editor, edvt_base_exit_editor),
                              EDVT_SLOT(enter_level, edvt_base_enter_level),
                              EDVT_SLOT(exit_level, edvt_base_exit_level),
                              EDVT_SLOT(update_lists, edvt_base_update_lists),
                              EDVT_SLOT(pre_load_initialisation, edvt_spline_pre_load),
                              EDVT_SLOT(post_load_initialisation, edvt_spline_post_load),
                              EDVT_SLOT(pre_save_initialisation, edvt_base_pre_save),
                              EDVT_SLOT(post_save_initialisation, edvt_base_post_save),
                              EDVT_SLOT(serialise_object, edvt_spline_serialise),
                              EDVT_SLOT(distance_to_ray, edvt_base_distance_ray),
                              EDVT_SLOT(distance_to_point, edvt_base_distance_point),
                              EDVT_SLOT(add_menu_items, edvt_spline_add_menu),
                              EDVT_SLOT(import, edvt_base_import)}};

extern const EdClassInterfaceVTableObject knotHelperVTable asm("_ZTV10KnotHelper")
    __attribute__((weak)) = {NULL,
                             NULL,
                             {reinterpret_cast<decltype(EdClassInterfaceVTable::destroy)>(edvt_knot_destructor),
                              reinterpret_cast<decltype(EdClassInterfaceVTable::delete_object)>(edvt_knot_delete),
                              EDVT_SLOT(clear_level, edvt_base_clear),
                              EDVT_SLOT(flush, edvt_knot_flush),
                              EDVT_SLOT(get_num_objects, edvt_knot_count),
                              EDVT_SLOT(get_next_object, edvt_knot_next),
                              EDVT_SLOT(get_next_filtered_object, edvt_base_next_filtered),
                              EDVT_SLOT(create_object, edvt_knot_create),
                              EDVT_SLOT(destroy_object, edvt_knot_destroy),
                              EDVT_SLOT(defunct_object, edvt_base_defunct),
                              EDVT_SLOT(revive_object, edvt_base_revive),
                              EDVT_SLOT(set_object_guid, edvt_base_set_guid),
                              EDVT_SLOT(get_object_guid, edvt_base_get_guid),
                              EDVT_SLOT(get_constructor_data, edvt_base_constructor_data),
                              EDVT_SLOT(construct, edvt_base_construct),
                              EDVT_SLOT(process, edvt_knot_process),
                              EDVT_SLOT(render, edvt_knot_render),
                              EDVT_SLOT(enter_editor, edvt_base_enter_editor),
                              EDVT_SLOT(exit_editor, edvt_base_exit_editor),
                              EDVT_SLOT(enter_level, edvt_base_enter_level),
                              EDVT_SLOT(exit_level, edvt_base_exit_level),
                              EDVT_SLOT(update_lists, edvt_base_update_lists),
                              EDVT_SLOT(pre_load_initialisation, edvt_base_pre_load),
                              EDVT_SLOT(post_load_initialisation, edvt_base_post_load),
                              EDVT_SLOT(pre_save_initialisation, edvt_base_pre_save),
                              EDVT_SLOT(post_save_initialisation, edvt_base_post_save),
                              EDVT_SLOT(serialise_object, edvt_base_serialise),
                              EDVT_SLOT(distance_to_ray, edvt_knot_distance_ray),
                              EDVT_SLOT(distance_to_point, edvt_base_distance_point),
                              EDVT_SLOT(add_menu_items, edvt_base_add_menu),
                              EDVT_SLOT(import, edvt_base_import)}};

extern const EdClassInterfaceVTableObject edClassInterfaceVTable asm("_ZTV16EdClassInterface");

SplineHelper::~SplineHelper() {
    vtable = const_cast<EdClassInterfaceVTable *>(&edClassInterfaceVTable.methods);
}
KnotHelper::~KnotHelper() {
    vtable = const_cast<EdClassInterfaceVTable *>(&edClassInterfaceVTable.methods);
}

extern "C" void splineHelperDeletingDestructor(SplineHelper *object) asm("_ZN12SplineHelperD0Ev");
void splineHelperDeletingDestructor(SplineHelper *object) {
    object->vtable = const_cast<EdClassInterfaceVTable *>(&edClassInterfaceVTable.methods);
    ::operator delete(object);
}

extern "C" void knotHelperDeletingDestructor(KnotHelper *object) asm("_ZN10KnotHelperD0Ev");
void knotHelperDeletingDestructor(KnotHelper *object) {
    object->vtable = const_cast<EdClassInterfaceVTable *>(&edClassInterfaceVTable.methods);
    ::operator delete(object);
}

template <typename Ref>
static Ref *make_spline_reference(EdClass *object_class, char *type, char *name, i32 offset, i32 size, i32 attributes,
                                  EdControl *control, bool add_to_class = true) {
    Ref *reference =
        new (theMemoryManager.AllocPool(sizeof(Ref), 1)) Ref(type, name, offset, size, attributes, control, 0);
    if (add_to_class)
        object_class->AddType(reference);
    return reference;
}

static EdStringControl *make_spline_string_control() {
    return new (theMemoryManager.AllocPool(sizeof(EdStringControl), 1)) EdStringControl();
}

static EdVectorControl *make_spline_vector_control() {
    return new (theMemoryManager.AllocPool(sizeof(EdVectorControl), 1)) EdVectorControl();
}

static EdEnumControl *make_spline_yes_no_control() {
    EdEnumControl *control = new (theMemoryManager.AllocPool(sizeof(EdEnumControl), 1)) EdEnumControl;
    control->items = EdEnumControl::YesNoItems;
    return control;
}

static EdFloatControl *make_spline_float_control(f32 minimum, f32 maximum) {
    EdFloatControl *control = new (theMemoryManager.AllocPool(sizeof(EdFloatControl), 1)) EdFloatControl;
    control->value_type = EdType_Float;
    control->format = const_cast<char *>("%.2f");
    control->maximum = maximum;
    control->minimum = minimum;
    return control;
}

inline SplineObject::SplineObject()
    : next(NULL), previous(NULL), knots{}, points{}, led_file(Placeable::CurrentLedFile) {
}

inline SplineObject::~SplineObject() {
    SplinePointList *point_list = &points;
    while (point_list->first != NULL) {
        SplinePointBlock *block = point_list->first;
        if (block->next != NULL)
            block->next->previous = block->previous;
        else
            point_list->last = block->previous;
        if (block->previous != NULL)
            block->previous->next = block->next;
        else
            point_list->first = block->next;
        block->next = NULL;
        block->previous = NULL;
        --point_list->block_count;
        delete block;
    }
    SplineKnotList *knot_list = &knots;
    while (knot_list->first != NULL) {
        SplineKnot *knot = knot_list->first;
        if (knot->next != NULL)
            knot->next->previous = knot->previous;
        else
            knot_list->last = knot->previous;
        if (knot->previous != NULL)
            knot->previous->next = knot->next;
        else
            knot_list->first = knot->next;
        knot->next = NULL;
        knot->previous = NULL;
        --knot_list->count;
        theMemoryManager.FreePool(knot, sizeof(SplineKnot));
    }
}

inline void SplineObject::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(SplineObject));
}

inline void SplinePointBlock::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(SplinePointBlock));
}

// The original translation unit exports this internal counter under its unmangled C name.
static i32 bezierline_depth asm("bezierline_depth");

typedef f32 BezierVec4 __attribute__((vector_size(16)));
typedef f32 BezierVec4Unaligned __attribute__((vector_size(16), aligned(4)));

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

namespace {

} // namespace

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

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

namespace {

} // namespace

#pragma GCC optimize("O2", "omit-frame-pointer")

extern "C" {
    struct EDCREATURE_s;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

namespace {

} // namespace

extern "C" {
extern u32 flagremaptab[32][2];
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

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

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

namespace {

} // namespace

#pragma GCC optimize("O2", "omit-frame-pointer")

extern "C" {
    struct EDCREATURE_s;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

namespace {

} // namespace

extern "C" {
extern u32 flagremaptab[32][2];
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

namespace {

} // namespace

struct numtx_s;

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

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

namespace {

} // namespace

#pragma GCC optimize("O2", "omit-frame-pointer")

extern "C" {
    struct EDCREATURE_s;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

namespace {

} // namespace

extern "C" {
extern u32 flagremaptab[32][2];
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern WORLDINFO WorldInfo[2];

namespace {

} // namespace

void BaseEditor::Initialise(variptr_u &, variptr_u &, i32 value) {
    field_0x0c = value;
}

i32 ClassEditor::ReadBlock(DATAPTR *) {
    return 0;
}

void ClassEditor::WriteBlock(i32) {
}

void ClassEditor::ClearLevel(i32 level) {
    for (i32 i = 0; i < theRegistry.class_count; ++i) {
        EdClassInterface *interface = theRegistry.classes[i].interface;
        if (interface != NULL) {
            interface->vtable->clear_level(interface, level);
        }
    }
}

void ClassEditor::Flush() {
    for (i32 i = 0; i < theRegistry.class_count; ++i) {
        EdClassInterface *interface = theRegistry.classes[i].interface;
        if (interface != NULL) {
            interface->vtable->flush(interface);
        }
    }
}

void ClassEditor::Enter() {
    for (i32 i = 0; i < theRegistry.class_count; ++i) {
        EdClassInterface *interface = theRegistry.classes[i].interface;
        if (interface != NULL) {
            interface->vtable->enter_editor(interface);
        }
    }
}

void ClassEditor::Exit() {
    for (i32 i = 0; i < theRegistry.class_count; ++i) {
        EdClassInterface *interface = theRegistry.classes[i].interface;
        if (interface != NULL) {
            interface->vtable->exit_editor(interface);
        }
    }
}

void EdClassObjectNameControl::Render() {
}

void EdControl::Refresh() {
}

void EdControl::Process(EdInputContext &) {
}

void EdControl::Render() {
}

void EdControl::SetMenuItemAttr(i32 mask, eduiitem_s *menu_item, eduiiattr_s *selected, eduiiattr_s *unselected) {
    if (reference->attributes & mask) {
        ClassObjectListEntry *entry = theClassEditor.selected_objects.first;
        if (entry == NULL)
            goto use_unselected;
        volatile i32 subobject_count = 0;
        for (; entry; entry = entry->next) {
            if (entry->object != object || entry->reference == NULL)
                continue;
            ++subobject_count;
            if (entry->reference == reference)
                goto use_unselected;
        }
        if (subobject_count == 0)
            goto use_unselected;
    }
    memcpy(menu_item->colours, selected, sizeof(*selected));
    return;
use_unselected:
    memcpy(menu_item->colours, unselected, sizeof(*unselected));
}

void EdMatrixControl::SetMenuItemAttr(i32 mask, eduiitem_s *menu_item, eduiiattr_s *selected, eduiiattr_s *unselected) {
    if (menu_item == components[0] || menu_item == components[1] || menu_item == components[2]) {
        if (mask & 8)
            memcpy(menu_item->colours, unselected, sizeof(*selected));
        else
            memcpy(menu_item->colours, selected, sizeof(*selected));
    }
    if (menu_item == components[3] || menu_item == components[4] || menu_item == components[5]) {
        if (mask && (mask & 0x10))
            memcpy(menu_item->colours, unselected, sizeof(*selected));
        else
            memcpy(menu_item->colours, selected, sizeof(*selected));
    }
    if (menu_item == components[6] || menu_item == components[7] || menu_item == components[8]) {
        if (mask && (mask & 0x20))
            memcpy(menu_item->colours, unselected, sizeof(*selected));
        else
            memcpy(menu_item->colours, selected, sizeof(*selected));
    }
}

void EdBitControl::Refresh() {
}

void EdSfxNameControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
}

void CursorTool::Initialise(variptr_u &, variptr_u &, i32) {
}

void cbEdLevelSetSliderFloat(eduimenu_s *, eduiitem_s *item, u32) {
    *static_cast<f32 *>(item->data_ptr) = static_cast<edui_slider_s *>(item)->value;
}

void cbEdLevelToggleInt(eduimenu_s *, eduiitem_s *item, u32) {
    i32 *value = static_cast<i32 *>(item->data_ptr);
    *value ^= 1;
    item->highlighted = *value;
}

static void cbEdLevelEditorSelect(eduimenu_s *, eduiitem_s *item, u32) {
    theLevelEditor.active_editor = static_cast<BaseEditor *>(item->data_ptr);
    edLevelDestroyActiveMenu = 1;
}

VuVec const *Placeable::GetInitialPosition() const {
    VuMtx const *matrix = GetInitialTransform();
    return matrix ? reinterpret_cast<VuVec const *>(&matrix->matrix.m30) : NULL;
}

VuVec const *Placeable::GetCurrentPosition() const {
    VuMtx const *matrix = GetCurrentTransform();
    return matrix ? reinterpret_cast<VuVec const *>(&matrix->matrix.m30) : NULL;
}

void PlaceableNameControl::Render() {
}

void PropertyTool::Initialise(variptr_u &, variptr_u &, i32) {
}

void SerialiseChar(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 1, 1);
}

void SerialiseShort(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 1);
}

void SerialiseInt(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 1);
}

void SerialiseFloat(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 1);
}

void SerialiseVuVec(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 3);
}

void SerialiseVuMtx(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 16);
}

void SerialiseNuVec(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 3);
}

void SerialiseNuMtx(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 16);
}

void SerialiseString(EdStream &stream, void *data, i32 count) {
    stream.SerialiseString(static_cast<char *>(data), count);
}

void SerialiseColour3(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 3);
}

void *EdClassInterface::GetNextObject(void *current, i32 (*filter)(void *)) {
    void *next = vtable->get_next_object(this, current);
    while (next != NULL && filter(next) == 0)
        next = vtable->get_next_object(this, next);
    return next;
}

void *EdRef::GetMemberObject(void *object) {
    void *member = static_cast<u8 *>(object) + member_offset;
    if (member != NULL && (attributes & 0x40000000)) {
        member = *static_cast<void **>(member);
    }
    return member;
}

void SpecialObject::SetInitialPosition(VuVec const *) {
}

int SceneInstance::GetVisibility() const {
    return visibility;
}

void SceneInstance::SetVisibility(i32 visible) {
    visibility = visible;
}

VuMtx const *SceneInstance::GetInitialTransform() const {
    return reinterpret_cast<VuMtx const *>(&initial_transform);
}

VuMtx const *SceneInstance::GetCurrentTransform() const {
    return reinterpret_cast<VuMtx const *>(&current_transform);
}

VuVec const *SceneInstance::GetInitialPosition() const {
    return reinterpret_cast<VuVec const *>(&initial_transform.m30);
}

void SceneInstance::SetInitialPosition(VuVec const *position) {
#if defined(__SSE__) && !defined(__EMSCRIPTEN__)
    __m128 value = _mm_setzero_ps();
    value = _mm_loadl_pi(value, reinterpret_cast<__m64 const *>(position));
    value = _mm_loadh_pi(value, reinterpret_cast<__m64 const *>(reinterpret_cast<u8 const *>(position) + 8));
    _mm_storel_pi(reinterpret_cast<__m64 *>(&current_transform.m30), value);
    _mm_storeh_pi(reinterpret_cast<__m64 *>(&current_transform.m32), value);
#else
    *reinterpret_cast<VuVec *>(&current_transform.m30) = *position;
#endif
}

VuVec const *SceneInstance::GetCurrentPosition() const {
    return reinterpret_cast<VuVec const *>(&current_transform.m30);
}

void SceneInstance::SetCurrentPosition(VuVec const *position) {
#if defined(__SSE__) && !defined(__EMSCRIPTEN__)
    __m128 value = _mm_setzero_ps();
    value = _mm_loadl_pi(value, reinterpret_cast<__m64 const *>(position));
    value = _mm_loadh_pi(value, reinterpret_cast<__m64 const *>(reinterpret_cast<u8 const *>(position) + 8));
    _mm_storel_pi(reinterpret_cast<__m64 *>(&current_transform.m30), value);
    _mm_storeh_pi(reinterpret_cast<__m64 *>(&current_transform.m32), value);
#else
    *reinterpret_cast<VuVec *>(&current_transform.m30) = *position;
#endif
}

void SceneObjectHelper::ClearLevel(i32 level) {
    for (SceneObject *object =
             static_cast<SceneObject *>(class_interface.vtable->get_next_object(&class_interface, NULL));
         object != NULL;) {
        SceneObject *next_object =
            static_cast<SceneObject *>(class_interface.vtable->get_next_object(&class_interface, object));
        if (object->led_file == level)
            class_interface.vtable->destroy_object(&class_interface, object, 0);
        object = next_object;
    }
    i32 scene_index;
    if (level == 0)
        scene_index = 0;
    else if (level == 1)
        scene_index = 1;
    else if (level == 2)
        scene_index = 2;
    else if (level == 3)
        scene_index = 3;
    else if (level == 4)
        scene_index = 4;
    else if (level == 5)
        scene_index = 5;
    else if (level == 6)
        scene_index = 6;
    else if (level == 7)
        scene_index = 7;
    else if (level == 8)
        scene_index = 8;
    else if (level == 9)
        scene_index = 9;
    else
        return;
    scenes[scene_index] = NULL;
    scene_counts[scene_index] = 0;
}

void SceneObjectHelper::Flush() {
    memset(scenes, 0, sizeof(scenes));
    memset(scene_counts, 0, sizeof(scene_counts));
    owned_last = NULL;
    owned_first = NULL;
    owned_object_count = 0;
}

void SceneObjectHelper::PostLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
}

void SceneObjectHelper::UpdateLists(MemoryBuffer *first, MemoryBuffer *second) {
    class_interface.vtable->pre_load_initialisation(&class_interface, first, second);
}

i32 SceneObjectHelper::GetNumObjects() {
    return scene_object_count + owned_object_count;
}

void SceneObjectHelper::cbEdSceneObjectShowHiddenSolid(eduimenu_s *, eduiitem_s *item, u32) {
    theSceneObjectHelper.show_hidden_solid ^= 1;
    item->highlighted = theSceneObjectHelper.show_hidden_solid & 1;
}

void SceneObjectHelper::cbEdSceneObjectShowHiddenWire(eduimenu_s *, eduiitem_s *item, u32) {
    theSceneObjectHelper.show_hidden_wire ^= 1;
    item->highlighted = theSceneObjectHelper.show_hidden_wire & 1;
}

void SceneObjectHelper::cbEdSceneObjectShowOwnedObjects(eduimenu_s *, eduiitem_s *item, u32) {
    theSceneObjectHelper.show_owned_objects ^= 1;
    item->highlighted = theSceneObjectHelper.show_owned_objects & 1;
}

void EdSpecialObjectControl::Process(EdInputContext &) {
}

void SplineHelper::PreLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
}

int SplineHelper::GetNumObjects() {
    return object_count;
}

void *SplineHelper::GetNextObject(void *current) {
    return current != NULL ? static_cast<SplineObject *>(current)->next : first_object;
}

void SplineHelper::cbEdSplineAutoGenPoints(eduimenu_s *, eduiitem_s *item, u32) {
    theSplineHelper.auto_generate_points ^= 1;
    item->highlighted = theSplineHelper.auto_generate_points & 1;
}

i32 KnotHelper::GetNumObjects() {
    i32 count = 0;
    for (SplineObject *spline = theSplineHelper.first_object; spline != NULL; spline = spline->next)
        count += spline->knots.count;
    return count;
}

void SplineTool::Initialise(variptr_u &, variptr_u &, i32) {
}

i32 SplineTool::Process(EdInputContext &) {
    return 0;
}

void SplineTool::Render() {
}

i32 EdOutputStream::SerialiseString(char **text) {
    return SerialiseString(*text, 0);
}

i32 EdOutputStream::SerialiseString(char **text, i32) {
    return SerialiseString(*text, 0);
}

i32 EdFileOutputStream::Eat(i32, i32) {
    return 0;
}

void EdControl::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    EdControl *control = new (theMemoryManager.AllocPool(sizeof(EdControl), 1)) EdControl;
    control->reference = member;
    control->object = target;
    control->item = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, cbSelected, nullptr, nullptr, 0,
                                       member->name, nullptr);
    eduiMenuAddItem(menu, control->item);
}

void EdVectorControl::Refresh() {
    VuVec vector;
    reference->GetMemberData(object, EdType_VuVec, &vector, 0);
    char value[128];
    sprintf(value, "%.2f", vector.x);
    eduiItemPropSetText(static_cast<edui_prop_s *>(components[0]), value);
    sprintf(value, "%.2f", vector.y);
    eduiItemPropSetText(static_cast<edui_prop_s *>(components[1]), value);
    sprintf(value, "%.2f", vector.z);
    eduiItemPropSetText(static_cast<edui_prop_s *>(components[2]), value);
}

void EdVectorControl::cbChanged(eduimenu_s *, eduiitem_s *item, u32) {
    EdVectorControl *control = static_cast<EdVectorControl *>(item->data_ptr);
    VuVec vector;
    control->reference->GetMemberData(control->object, EdType_VuVec, &vector, 0);
    f32 value = 0.0f;
    if (item == control->components[0]) {
        value = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
        vector.x = value;
    }
    if (item == control->components[1]) {
        value = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
        vector.y = value;
    }
    if (item == control->components[2]) {
        value = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
        vector.z = value;
    }
    vector.w = 1.0f;
    control->reference->SetMemberData(control->object, EdType_VuVec, &vector, 0, nullptr);
    char text[128];
    sprintf(text, "%.2f", value);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), text);
}

void EdColourControl::Refresh() {
    NUVEC colour;
    eduiitem_s *preview_item = item;
    reference->GetMemberData(object, EdType_Colour3, &colour, 0);
    char value[128];
    sprintf(value, "%.2f %.2f %.2f", colour.x, colour.y, colour.z);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), value);
    set_colour_preview(preview_item, colour);
}

SplinePointBlock::~SplinePointBlock() {
    if (points != NULL)
        theMemoryManager.FreePool(points, capacity * sizeof(VuVec));
}

void KnotHelper::DestroyObject(void *object, i32) {
    theLevelEditor.destroying_objects = 1;
    SplineKnot *knot = static_cast<SplineKnot *>(object);
    SplineObject *spline = knot->spline;
    if (knot->previous != NULL)
        knot->previous->next = knot->next;
    else
        spline->knots.first = knot->next;
    if (knot->next != NULL)
        knot->next->previous = knot->previous;
    else
        spline->knots.last = knot->previous;
    knot->next = NULL;
    knot->previous = NULL;
    --spline->knots.count;
    theMemoryManager.FreePool(knot, sizeof(SplineKnot));
    if (spline->knots.count == 0) {
        if (spline->previous != NULL)
            spline->previous->next = spline->next;
        else
            theSplineHelper.first_object = spline->next;
        if (spline->next != NULL)
            spline->next->previous = spline->previous;
        else
            theSplineHelper.last_object = spline->previous;
        spline->next = NULL;
        spline->previous = NULL;
        --theSplineHelper.object_count;
        delete spline;
    }
}

char const *EdFileOutputStream::BeginBlock(char const *name) {
    i32 position = NuFilePos(file);
    if (block_count < 8) {
        block_positions[block_count++] = position;
    }
    SerialiseBuffer(&position, sizeof(position), 1);
    SerialiseString(const_cast<char *>(name), 0);
    return name;
}

i32 EdFileInputStream::Eat(i32 size, i32 count) {
    return NuFileSeek(file, size * count, NUFILE_SEEK_CURRENT);
}

void EdFileInputStream::EndBlock() {
    Block block = blocks[--block_count];
    i32 position = NuFilePos(file);
    if (position != block.position + block.size) {
        NuFileSeek(file, block.position + block.size, NUFILE_SEEK_START);
    }
    name_length = block.name_offset;
}

void EdFileOutputStream::EndBlock() {
    i32 position = block_positions[--block_count];
    i32 end = NuFilePos(file);
    i32 size = end - position;
    NuFileSeek(file, position, NUFILE_SEEK_START);
    SerialiseBuffer(&size, sizeof(size), 1);
    NuFileSeek(file, end, NUFILE_SEEK_START);
}

i32 EdOutputStream::SerialiseString(char *text, i32) {
    if (!text) {
        text = const_cast<char *>("(NULL)");
    }
    i32 length = NuStrLen(text) + 1;
    i32 written = SerialiseBuffer(&length, sizeof(length), 1);
    return written + SerialiseBuffer(text, 1, length);
}

char const *EdFileInputStream::BeginBlock(char const *name) {
    i32 position = NuFilePos(file);
    if (block_count > 0) {
        Block &block = blocks[block_count - 1];
        if (position >= block.position + block.size) {
            return NULL;
        }
    }
    char *block_name;
    if (pending) {
        block_name = block_names + pending_block.name_offset;
    } else {
        pending_block.position = position;
        pending_block.name_offset = name_length;
        pending = 1;
        SerialiseBuffer(&pending_block.size, sizeof(pending_block.size), 1);
        block_name = block_names + name_length;
        SerialiseString(block_name, sizeof(block_names) - name_length);
        name_length += NuStrLen(block_name) + 1;
    }
    if (name && NuStrICmp(name, block_name)) {
        return NULL;
    }
    if (block_count < 8) {
        blocks[block_count++] = pending_block;
    }
    pending = 0;
    return block_name;
}

i32 EdInputStream::SerialiseString(char *text, i32 capacity) {
    i32 length;
    char temporary[256];
    SerialiseBuffer(&length, sizeof(length), 1);
    if (length > capacity) {
        SerialiseBuffer(temporary, 1, sizeof(temporary));
        NuStrNCpy(text, temporary, capacity);
        text[capacity - 1] = 0;
        return capacity;
    }
    return SerialiseBuffer(text, 1, length);
}

void ClassEditor::AddMenuItems(eduimenu_s *menu) {
    i32 index;
    eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdClassFileMenu, const_cast<char *>("File")));
    eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdClassToolsMenu, const_cast<char *>("Tools")));
    eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdClassSnapMenu, const_cast<char *>("Snap")));
    eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdClassModeMenu, const_cast<char *>("Mode")));
    eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdClassViewMenu, const_cast<char *>("Show/Hide")));
    eduiMenuAddItem(menu, eduiItemSeparatorCreate(0, &EdLevelAttr));
    eduiMenuAddItem(menu,
                    eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdClassSelectClassMenu, const_cast<char *>("Select")));
    eduiMenuAddItem(menu, eduiItemSeparatorCreate(0, &EdLevelAttr));

    if (selected_objects.count == 0) {
        eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdClassNewMenu, const_cast<char *>("New")));
        return;
    }

    eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdClassNewMenu, const_cast<char *>("New/Copy")));
    eduiMenuAddItem(menu,
                    eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdClassDeleteObject, const_cast<char *>("Delete")));

    // The original keeps this class list 16-byte aligned.
    EdClass *classes[32] __attribute__((aligned(16)));
    i32 class_count = 0;
    for (ClassObjectListEntry *entry = selected_objects.first; entry != NULL; entry = entry->next) {

        for (index = 0; index < class_count; ++index) {
            if (classes[index] == entry->ed_class)
                break;
        }
        if (index == class_count)
            classes[class_count++] = entry->ed_class;
    }
    for (i32 index = 0; index < class_count; ++index) {
        EdClassInterface *interface = classes[index]->interface;
        interface->vtable->add_menu_items(interface, menu);
    }
}

void SplineHelper::AddMenuItems(eduimenu_s *menu) {
    eduiMenuAddItem(menu, eduiItemSeparatorCreate(0, &EdLevelAttr));
    eduiMenuAddItem(menu,
                    eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdSplineSmoothKnot, const_cast<char *>("Smooth Knot")));
    eduiMenuAddItem(
        menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdSplineSmoothSpline, const_cast<char *>("Smooth Spline")));
    eduiMenuAddItem(
        menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdSplineReverseSpline, const_cast<char *>("Reverse Spline")));
    eduiMenuAddItem(
        menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdSplineReGenPoints, const_cast<char *>("Re-Gen Points")));
    eduiMenuAddItem(menu, eduiItemCheckCreate(0, &EdLevelAttr, auto_generate_points, 0, cbEdSplineAutoGenPoints,
                                              const_cast<char *>("Auto-Gen Points")));
}

void SceneObjectHelper::AddMenuItems(eduimenu_s *menu) {
    eduiMenuAddItem(menu, eduiItemSeparatorCreate(0, &EdLevelAttr));
    eduiMenuAddItem(menu, eduiItemCheckCreate(0, &EdLevelAttr, show_hidden_solid, 0, cbEdSceneObjectShowHiddenSolid,
                                              const_cast<char *>("Show Hidden Solid")));
    eduiMenuAddItem(menu, eduiItemCheckCreate(0, &EdLevelAttr, show_hidden_wire, 0, cbEdSceneObjectShowHiddenWire,
                                              const_cast<char *>("Show Hidden Wire")));
    eduiMenuAddItem(menu, eduiItemCheckCreate(0, &EdLevelAttr, show_owned_objects, 0, cbEdSceneObjectShowOwnedObjects,
                                              const_cast<char *>("Show Owned Obj")));
}

void *SceneObjectHelper::GetNextObject(void *item) {
    if (item == NULL) {
        iteration_scene_index = 0;
        iteration_object_index = 0;
    } else {
        ++iteration_object_index;
        if (iteration_scene_index > 9)
            goto owned_objects;
    }
    while (iteration_scene_index < 10) {
        while (scene_counts[iteration_scene_index] > iteration_object_index) {
            SceneObject *object = reinterpret_cast<SceneObject *>(
                reinterpret_cast<u8 *>(scenes[iteration_scene_index]) + iteration_object_index * sizeof(SceneObject));
            if (object->reserved_0x28 == 0) {
                if (scene_filter[0] == 0 || NuStrIStr(const_cast<char *>(object->GetName()), scene_filter) == NULL)
                    return reinterpret_cast<SceneObject *>(reinterpret_cast<u8 *>(scenes[iteration_scene_index]) +
                                                           iteration_object_index * sizeof(SceneObject));
            }
            ++iteration_object_index;
        }
        ++iteration_scene_index;
        iteration_object_index = 0;
    }
    if (item == NULL)
        return NULL;

owned_objects:
    return static_cast<SceneObject *>(item)->editor_owned != 0 ? static_cast<SceneInstance *>(item)->next : owned_first;
}

char const *SpecialObject::GetName() const {
    return NuSpecialGetName(const_cast<nuhspecial_s *>(&special));
}

void EdBitControl::cbButton(eduimenu_s *menu, eduiitem_s *item, u32) {
    edBitItem = reinterpret_cast<edui_prop_s *>(item);
    edBitControl = static_cast<EdBitControl *>(item->data_ptr);
    edBitIndex = edBitItem->extra_data;
    eduimenu_s *choices =
        eduiMenuCreate(item->x + menu->width, item->y, 180, 250,
                       reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (choices != NULL) {
        for (Item *entry = edBitControl->items; entry->name != NULL; ++entry) {
            eduiMenuAddItem(choices, eduiItemSelCreate(reinterpret_cast<usize>(entry), item->colours, 0, 0,
                                                       cbSelectItem, entry->name));
        }
        choices->flags |= 1;
        eduiMenuAttach(menu, choices);
        eduiMenuFitWidth(choices, 5);
        reinterpret_cast<u8 *>(item)[0x4c] &= ~8;
    }
}

void EdEnumControl::cbButton(eduimenu_s *menu, eduiitem_s *item, u32) {
    active_enum_control = static_cast<EdEnumControl *>(item->data_ptr);
    eduimenu_s *choices =
        eduiMenuCreate(item->x + menu->width, item->y, 180, 250,
                       reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (choices == NULL)
        return;
    for (Item *entry = active_enum_control->items; entry->name != NULL; ++entry)
        eduiMenuAddItem(
            choices, eduiItemSelCreate(reinterpret_cast<usize>(entry), item->colours, 0, 0, cbSelectItem, entry->name));
    choices->flags |= 1;
    eduiMenuAttach(menu, choices);
    eduiMenuFitWidth(choices, 5);
    eduiMenuFitOnScreen(choices, 30);
    static_cast<edui_prop_s *>(item)->unknown_property_flags &= ~8u;
}

void ClassEditor::cbEdClassNewMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (menu == NULL)
        return;
    for (i32 index = 0; index < theRegistry.class_count; ++index) {
        EdClass *ed_class = &theRegistry.classes[index];
        if ((ed_class->flags & 0x20000000) == 0)
            eduiMenuAddItem(menu, eduiItemSelCreate(index, &EdLevelAttr, 0, 0, cbEdClassNewObject, ed_class->name));
    }
    if (menu->first == NULL)
        eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect,
                                                const_cast<char *>("No Registered Classes")));
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
}

void ClassEditor::cbEdClassToolsMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (menu == NULL)
        return;
    eduiMenuAddItem(menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdClassRemoveDuplicates,
                                            const_cast<char *>("Remove Duplicates")));
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
}

i32 SpecialObject::Exists() const {
    return NuSpecialExistsFn(const_cast<nuhspecial_s *>(&special));
}

void SceneInstance::Render(VuMtx const *) const {
    NuSpecialDrawAt(const_cast<nuhspecial_s *>(&special), const_cast<NUMTX *>(&current_transform));
}

void SpecialObject::SetVisibility(i32 enabled) {
    NuSpecialSetVisibility(&special, enabled);
}

void SpecialObject::SetCollision(i32 enabled) {
    NuSpecialSetCollision(&special, enabled);
}

i32 SpecialObject::GetVisibility() const {
    return NuSpecialGetVisibilityFn(const_cast<nuhspecial_s *>(&special));
}

i32 SpecialObject::GetCollision() const {
    return NuSpecialGetCollision(const_cast<nuhspecial_s *>(&special)) != 0;
}

VuMtx const *SpecialObject::GetCurrentTransform() const {
    return reinterpret_cast<VuMtx const *>(NuSpecialGetDrawMtx(const_cast<nuhspecial_s *>(&special)));
}

void SpecialObject::SetCurrentPosition(VuVec const *position) {
    NuSpecialSetDrawPos(&special, reinterpret_cast<NUVEC *>(const_cast<VuVec *>(position)));
}

VuVec const *SpecialObject::GetCurrentPosition() const {
    return reinterpret_cast<VuVec const *>(NuSpecialGetDrawPos(const_cast<nuhspecial_s *>(&special)));
}

VuVec const *SpecialObject::GetInitialPosition() const {
    return reinterpret_cast<VuVec const *>(NuSpecialGetPos(const_cast<nuhspecial_s *>(&special)));
}

void SpecialObject::SetCurrentTransform(VuMtx const *matrix) {
    NuSpecialSetDrawMtx(&special, reinterpret_cast<NUMTX *>(const_cast<VuMtx *>(matrix)));
}

void SpecialObject::SetInitialTransform(VuMtx const *matrix) {
    NuSpecialSetMtx(&special, reinterpret_cast<NUMTX *>(const_cast<VuMtx *>(matrix)));
}

VuMtx const *SpecialObject::GetInitialTransform() const {
    return reinterpret_cast<VuMtx const *>(NuSpecialGetMtx(const_cast<nuhspecial_s *>(&special)));
}

f32 SpecialObject::GetRadius() const {
    f32 radius = 1.0f;
    NUVEC position;
    if (NuSpecialExistsFn(const_cast<nuhspecial_s *>(&special))) {
        NuSpecialGetRadius(const_cast<nuhspecial_s *>(&special), &position, &radius);
    }
    return radius;
}

numtl_s *SpecialObject::GetMtl(i32 index) const {
    return NuSpecialGetMtl(const_cast<nuhspecial_s *>(&special), index);
}

i32 SpecialObject::GetNumMtls() const {
    return NuSpecialNumMtls(const_cast<nuhspecial_s *>(&special));
}

void Placeable::SetCurrentPosition(VuVec const *position) {
    NUMTX matrix;
    VuMtx const *current = GetCurrentTransform();
    // The original updates only a temporary matrix.
    if (current) {
        matrix = current->matrix;
        NuMtxTranslate(&matrix, const_cast<NUVEC *>(&position->xyz));
    } else {
        NuMtxSetTranslation(&matrix, const_cast<NUVEC *>(&position->xyz));
    }
}

void Placeable::SetInitialPosition(VuVec const *position) {
    NUMTX matrix;
    VuMtx const *initial = GetInitialTransform();
    if (initial) {
        matrix = initial->matrix;
        NuMtxTranslate(&matrix, const_cast<NUVEC *>(&position->xyz));
    } else {
        NuMtxSetTranslation(&matrix, const_cast<NUVEC *>(&position->xyz));
    }
}

void EdVectorControl::cbButton(eduimenu_s *menu, eduiitem_s *item, u32 value) {
    static_cast<edui_prop_s *>(item)->unknown_property_flags |= 0x20;
    nupad_s *pad = EdControl::Input->pad;
    f32 changed = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
    f32 dx;
    f32 dy;
    eduiGetCursorDelta(&dx, &dy);
    changed -= dy * 100.0f;
    dy = eduiGetAnalougePadValue(pad);
    changed -= dy;
    char text[128];
    sprintf(text, "%.2f", changed);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), text);
    cbChanged(menu, item, value);
}

void cbEdLevelDestroyOnSelect(eduimenu_s *menu, eduiitem_s *, u32) {
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
    if (menu == edLevelActiveMenu) {
        edLevelActiveMenu = NULL;
    }
    if (menu == edLevelPinnedMenu) {
        edLevelPinnedMenu = NULL;
    }
}

void EdColourControl::cbColourSelected(eduimenu_s *menu, eduiitem_s *item, u32 flags) {
    edui_colour_pick_s *picker = static_cast<edui_colour_pick_s *>(item);
    NUVEC colour = {picker->red, picker->green, picker->blue};
    eduiitem_s *preview_item = edColourControl->item;
    edColourControl->reference->SetMemberData(edColourControl->object, EdType_Colour3, &colour, 0, NULL);
    char value[128];
    sprintf(value, "%.2f %.2f %.2f", colour.x, colour.y, colour.z);
    eduiItemPropSetText(static_cast<edui_prop_s *>(edColourControl->item), value);
    set_colour_preview(preview_item, colour);
    cbEdLevelDestroyOnSelect(menu, item, flags);
}

void ClassEditor::cbDestroyMenu(eduimenu_s *menu, eduimenu_s *) {
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
    if (theClassEditor.menu == menu) {
        theClassEditor.menu = NULL;
    }
}

void EdColourControl::cbButton(eduimenu_s *menu, eduiitem_s *item, u32) {
    edColourControl = static_cast<EdColourControl *>(item->data_ptr);
    eduimenu_s *picker_menu =
        eduiMenuCreate(item->x + menu->width, item->y, 180, 250,
                       reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (picker_menu == NULL)
        return;
    NUVEC colour;
    edColourControl->reference->GetMemberData(edColourControl->object, EdType_Colour3, &colour, 0);
    eduiitem_s *picker = eduiItemColourPickCreate(0, item->colours, cbColourSelected, const_cast<char *>("Colour"));
    eduiItemColourPickSetRGB(static_cast<edui_colour_pick_s *>(picker), colour.x, colour.y, colour.z);
    eduiMenuAddItem(picker_menu, picker);
    eduiMenuAttach(menu, picker_menu);
    static_cast<edui_prop_s *>(item)->unknown_property_flags &= ~8;
}

void EdColourControl::cbChanged(eduimenu_s *, eduiitem_s *item, u32) {
    EdColourControl *control = static_cast<EdColourControl *>(item->data_ptr);
    NUVEC colour;
    control->reference->GetMemberData(control->object, EdType_Colour3, &colour, 0);
    sscanf(static_cast<edui_prop_s *>(item)->property_text, "%f %f %f", &colour.x, &colour.y, &colour.z);
    control->reference->SetMemberData(control->object, EdType_Colour3, &colour, 0, NULL);
    char value[128];
    sprintf(value, "%.2f %.2f %.2f", colour.x, colour.y, colour.z);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), value);
    set_colour_preview(item, colour);
}

void EdStringControl::cbPress(eduimenu_s *menu, eduiitem_s *item, u32) {
    eduiAddPropTextPickEnt(menu, item);
}

__attribute__((force_align_arg_pointer)) void EdBitControl::cbChanged(eduimenu_s *, eduiitem_s *item, u32) {
    EdBitControl *control = static_cast<EdBitControl *>(item->data_ptr);
    for (Item *entry = control->items; entry->name != NULL; ++entry) {
        if (NuStrICmp(entry->name, reinterpret_cast<edui_prop_s *>(item)->property_text) == 0) {
            eduiItemPropSetText(reinterpret_cast<edui_prop_s *>(item), entry->name);
            return;
        }
    }
    i32 values[4];
    values[0] = NuAToI(reinterpret_cast<edui_prop_s *>(item)->property_text);
    Item *entry = control->items;
    char *text = entry->name;
    while (text != NULL) {
        if (entry->value == values[0])
            break;
        ++entry;
        text = entry->name;
    }
    char number[0x80];
    if (text == NULL) {
        control->reference->GetMemberData(control->object, EdType_Int, values, 0);
        sprintf(number, "%d", values[0]);
        text = number;
    }
    eduiItemPropSetText(reinterpret_cast<edui_prop_s *>(item), text);
}

void EdEnumControl::cbChanged(eduimenu_s *, eduiitem_s *item, u32) {
    EdEnumControl *control = static_cast<EdEnumControl *>(item->data_ptr);
    edui_prop_s *property = reinterpret_cast<edui_prop_s *>(item);
    for (Item *entry = control->items; entry->name != NULL; ++entry) {
        if (NuStrICmp(entry->name, property->property_text) == 0) {
            eduiItemPropSetText(property, entry->name);
            return;
        }
    }
    i32 value = NuAToI(property->property_text);
    for (Item *entry = control->items; entry->name != NULL; ++entry) {
        if (entry->value == value) {
            eduiItemPropSetText(property, entry->name);
            return;
        }
    }
    control->reference->GetMemberData(control->object, EdType_Int, &value, 0);
    char text[128];
    sprintf(text, "%d", value);
    eduiItemPropSetText(property, text);
}

void ClassEditor::cbEdClassModeMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (menu == NULL)
        return;
    eduiMenuAddItem(menu, eduiItemToggleCreate(0, &EdLevelAttr, theClassEditor.mode == 0, 1, cbEdClassSetMode,
                                               const_cast<char *>("Select")));
    eduiMenuAddItem(menu, eduiItemToggleCreate(1, &EdLevelAttr, theClassEditor.mode == 1, 1, cbEdClassSetMode,
                                               const_cast<char *>("Create")));
    eduiMenuAddItem(menu, eduiItemToggleCreate(2, &EdLevelAttr, theClassEditor.mode == 2, 1, cbEdClassSetMode,
                                               const_cast<char *>("Delete")));
    eduiMenuAddItem(menu, eduiItemToggleCreate(3, &EdLevelAttr, theClassEditor.mode == 3, 1, cbEdClassSetMode,
                                               const_cast<char *>("Move")));
    eduiMenuAddItem(menu, eduiItemToggleCreate(4, &EdLevelAttr, theClassEditor.mode == 4, 1, cbEdClassSetMode,
                                               const_cast<char *>("Rotate")));
    eduiMenuAddItem(menu, eduiItemToggleCreate(5, &EdLevelAttr, theClassEditor.mode == 5, 1, cbEdClassSetMode,
                                               const_cast<char *>("Scale")));
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
}

void ClassEditor::cbEdClassSnapMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (menu == NULL)
        return;
    eduiMenuAddItem(menu, eduiItemToggleCreate(1, &EdLevelAttr, theClassEditor.snap_mode == 1, 1, cbEdClassSetSnap,
                                               const_cast<char *>("Height")));
    eduiMenuAddItem(menu, eduiItemToggleCreate(2, &EdLevelAttr, theClassEditor.snap_mode == 2, 1, cbEdClassSetSnap,
                                               const_cast<char *>("Ray")));
    eduiMenuAddItem(menu, eduiItemToggleCreate(0, &EdLevelAttr, theClassEditor.snap_mode == 0, 1, cbEdClassSetSnap,
                                               const_cast<char *>("Off")));
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
}

void ClassEditor::cbEdClassSetSnap(eduimenu_s *menu, eduiitem_s *item, u32) {
    theClassEditor.snap_mode = item->data;
    eduiMenuHighlight(menu, item);
}

void SplineHelper::ClearLevel(i32 level) {
    if (level == -1)
        return;
    EdClassInterface *interface = reinterpret_cast<EdClassInterface *>(this);
    for (SplineObject *spline = static_cast<SplineObject *>(interface->vtable->get_next_object(interface, NULL));
         spline != NULL;) {
        SplineObject *next = static_cast<SplineObject *>(interface->vtable->get_next_object(interface, spline));
        if (spline->led_file == level)
            interface->vtable->destroy_object(interface, spline, 0);
        spline = next;
    }
}

i32 EdInputStream::SerialiseString(char **text, i32 capacity) {
    if (*text) {
        return SerialiseString(*text, capacity);
    }
    return SerialiseString(text);
}

i32 EdFileOutputStream::SerialiseBuffer(void *data, i32 size, i32 count) {
    if (swap_endianness && size > 1) {
        u8 *cursor = static_cast<u8 *>(data);
        for (i32 i = 0; i < count; i++) {
            switch (size) {
                case 2:
                    EdFileSwapEndianess16(cursor);
                    break;
                case 4:
                    EdFileSwapEndianess32(cursor);
                    break;
            }
            cursor += size;
        }
    }
    i32 result = NuFileWrite(file, data, size * count);
    if (swap_endianness && size > 1) {
        u8 *cursor = static_cast<u8 *>(data);
        for (i32 i = 0; i < count; i++) {
            switch (size) {
                case 2:
                    EdFileSwapEndianess16(cursor);
                    break;
                case 4:
                    EdFileSwapEndianess32(cursor);
                    break;
            }
            cursor += size;
        }
    }
    return result;
}

i32 EdFileInputStream::SerialiseBuffer(void *data, i32 size, i32 count) {
    i32 result = NuFileRead(file, data, size * count);
    if (swap_endianness && size > 1) {
        u8 *cursor = static_cast<u8 *>(data);
        for (i32 i = 0; i < count; i++) {
            switch (size) {
                case 2:
                    EdFileSwapEndianess16(cursor);
                    break;
                case 4:
                    EdFileSwapEndianess32(cursor);
                    break;
            }
            cursor += size;
        }
    }
    return result;
}

void SpecialObject::Render(VuMtx const *matrix) const {
    if (matrix != NULL) {
        NuSpecialDrawAt(const_cast<nuhspecial_s *>(&special), reinterpret_cast<NUMTX *>(const_cast<VuMtx *>(matrix)));
    } else {
        NuSpecialDrawAt(const_cast<nuhspecial_s *>(&special),
                        NuSpecialGetDrawMtx(const_cast<nuhspecial_s *>(&special)));
    }
}

void cbEdLevelDestroy(eduimenu_s *menu, eduimenu_s *) {
    if ((menu->flags & 4) != 0) {
        return;
    }
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
    eduiSetCameraEnabled(1);
    if (menu == edLevelActiveMenu) {
        edLevelActiveMenu = NULL;
    }
    if (menu != edLevelPinnedMenu) {
        if (edLevelPinnedMenu != NULL && edLevelActiveMenu == NULL) {
            edLevelActiveMenu = edLevelPinnedMenu;
        }
    } else {
        edLevelPinnedMenu = NULL;
    }
}

void EdSpecialObjectControl::cbSelectObject(eduimenu_s *, eduiitem_s *item, u32) {
    if (active_special_object_control == NULL)
        return;
    nuhspecial_s selected;
    nuhspecial_s *choice;
    void *data = item->data_ptr;
    if (reinterpret_cast<uintptr_t>(data) == static_cast<uintptr_t>(-1)) {
        NuSpecialClear(&selected);
        choice = &selected;
    } else {
        choice = &static_cast<SpecialObject *>(data)->special;
        char *name = NuSpecialGetName(choice);
        if (name != NULL)
            eduiItemPropSetText(reinterpret_cast<edui_prop_s *>(active_special_object_control->item), name);
    }
    EdSpecialObjectControl *control = active_special_object_control;
    control->reference->SetMemberData(control->object, EdType_NuHSpecial, choice, 0, NULL);
}

void SceneObjectHelper::DestroyObject(void *item, i32) {
    theLevelEditor.destroying_objects = 1;
    SceneInstance *object = static_cast<SceneInstance *>(item);
    if (object->editor_owned == 0)
        return;
    if (object->next != NULL)
        object->next->previous = object->previous;
    else
        owned_last = object->previous;
    if (object->previous != NULL)
        object->previous->next = object->next;
    else
        owned_first = object->next;
    object->next = NULL;
    object->previous = NULL;
    --owned_object_count;
    delete object;
}

void *KnotHelper::GetNextObject(void *current) {
    if (current == NULL) {
        SplineObject *spline = theSplineHelper.first_object;
        if (spline == NULL)
            return NULL;
        do {
            if (spline->knots.count != 0)
                return spline->knots.first;
            spline = spline->next;
        } while (spline != NULL);
        return NULL;
    }

    SplineKnot *knot = static_cast<SplineKnot *>(current);
    SplineKnot *next = knot->next;
    if (next == NULL) {
        SplineObject *spline = knot->spline->next;
        if (spline != NULL) {
            do {
                if (spline->knots.count != 0)
                    return spline->knots.first;
                spline = spline->next;
            } while (spline != NULL);
        }
    }
    return next;
}

void SplineHelper::DestroyObject(void *object, i32) {
    theLevelEditor.destroying_objects = 1;
    SplineObject *spline = static_cast<SplineObject *>(object);
    for (SplineObject *current = first_object; current != NULL; current = current->next) {
        if (current == spline && current->knots.count == 0) {
            if (current->previous != NULL)
                current->previous->next = current->next;
            else
                first_object = current->next;
            if (current->next != NULL)
                current->next->previous = current->previous;
            else
                last_object = current->previous;
            current->next = NULL;
            current->previous = NULL;
            --object_count;
            delete current;
            return;
        }
    }
}

void EdMatrixControl::Refresh() {
    VuMtx matrix;
    reference->GetMemberData(object, EdType_VuMtx, &matrix, 0);
    char value[128];
#define REFRESH_MATRIX_COMPONENT(index, number)                                                                        \
    if (components[index]) {                                                                                           \
        sprintf(value, "%.2f", number);                                                                                \
        eduiItemPropSetText(static_cast<edui_prop_s *>(components[index]), value);                                     \
    }
    REFRESH_MATRIX_COMPONENT(0, matrix.matrix.m30);
    REFRESH_MATRIX_COMPONENT(1, matrix.matrix.m31);
    REFRESH_MATRIX_COMPONENT(2, matrix.matrix.m32);
    if (components[3] || components[4] || components[5]) {
        NUANG x, y, z;
        NuMtxGetEulerXYZ(&matrix.matrix, &x, &y, &z);
        REFRESH_MATRIX_COMPONENT(3, static_cast<f32>(x) * (360.0f / 65536.0f));
        REFRESH_MATRIX_COMPONENT(4, static_cast<f32>(y) * (360.0f / 65536.0f));
        REFRESH_MATRIX_COMPONENT(5, static_cast<f32>(z) * (360.0f / 65536.0f));
    }
    REFRESH_MATRIX_COMPONENT(6, NuVecMag(reinterpret_cast<NUVEC *>(&matrix.matrix.m00)));
    REFRESH_MATRIX_COMPONENT(7, NuVecMag(reinterpret_cast<NUVEC *>(&matrix.matrix.m10)));
    REFRESH_MATRIX_COMPONENT(8, NuVecMag(reinterpret_cast<NUVEC *>(&matrix.matrix.m20)));
}

void *SplineHelper::CreateObject(void *, i32, i32) {
    SplineObject *spline = new (theMemoryManager.AllocPool(sizeof(SplineObject), 1)) SplineObject();
    spline->previous = last_object;
    if (last_object != NULL)
        last_object->next = spline;
    const bool had_first = first_object != NULL;
    last_object = spline;
    if (!had_first)
        first_object = spline;
    ++object_count;
    return spline;
}

i32 EdInputStream::SerialiseString(char **text) {
    i32 length;
    SerialiseBuffer(&length, sizeof(length), 1);
    char *allocated = (char *)memory_buffer->Allocate(length);
    *text = allocated;
    return SerialiseBuffer(allocated, 1, length);
}

void EdMatrixControl::cbChanged(eduimenu_s *, eduiitem_s *item, u32) {
    NUMTX rebuilt;
    EdMatrixControl *control = static_cast<EdMatrixControl *>(item->data_ptr);
    // The original stores the source matrix at a 16-byte aligned stack address.
    VuMtx source __attribute__((aligned(16)));
    control->reference->GetMemberData(control->object, EdType_VuMtx, &source, 0);
    f32 changed_value = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
    NUMTX &matrix = source.matrix;
    if (item == control->components[0])
        matrix.m30 = changed_value;
    if (item == control->components[1])
        matrix.m31 = changed_value;
    if (item == control->components[2])
        matrix.m32 = changed_value;
    if (control->components[3] || control->components[4] || control->components[5] || control->components[6] ||
        control->components[7] || control->components[8]) {

        NuMtxSetIdentity(&rebuilt);
        if (control->components[6] && control->components[7] && control->components[8]) {
            // The original builds the scale as an aligned four-component vector with w set to one.
            VuVec scale __attribute__((aligned(16)));
            scale.x = NuAToF(static_cast<edui_prop_s *>(control->components[6])->property_text);
            scale.y = NuAToF(static_cast<edui_prop_s *>(control->components[7])->property_text);
            scale.z = NuAToF(static_cast<edui_prop_s *>(control->components[8])->property_text);
            scale.w = 1.0f;
            NuMtxScale(&rebuilt, reinterpret_cast<NUVEC *>(&scale));
        }
        {
            NUANG x = static_cast<NUANG>(NuAToF(static_cast<edui_prop_s *>(control->components[3])->property_text) *
                                         (65536.0f / 360.0f));
            NUANG z = static_cast<NUANG>(NuAToF(static_cast<edui_prop_s *>(control->components[5])->property_text) *
                                         (65536.0f / 360.0f));
            NUANG y = static_cast<NUANG>(NuAToF(static_cast<edui_prop_s *>(control->components[4])->property_text) *
                                         (65536.0f / 360.0f));
            f32 sine = NU_SIN_LUT(x), cosine = NU_COS_LUT(x);
            f32 saved = rebuilt.m01;
            rebuilt.m01 = saved * cosine - rebuilt.m02 * sine;
            rebuilt.m02 = saved * sine + rebuilt.m02 * cosine;
            saved = rebuilt.m11;
            rebuilt.m11 = saved * cosine - rebuilt.m12 * sine;
            rebuilt.m12 = saved * sine + rebuilt.m12 * cosine;
            saved = rebuilt.m21;
            rebuilt.m21 = saved * cosine - rebuilt.m22 * sine;
            rebuilt.m22 = saved * sine + rebuilt.m22 * cosine;

            sine = NU_SIN_LUT(y);
            cosine = NU_COS_LUT(y);
            saved = rebuilt.m00;
            rebuilt.m00 = saved * cosine + rebuilt.m02 * sine;
            rebuilt.m02 = rebuilt.m02 * cosine - saved * sine;
            saved = rebuilt.m10;
            rebuilt.m10 = saved * cosine + rebuilt.m12 * sine;
            rebuilt.m12 = rebuilt.m12 * cosine - saved * sine;
            saved = rebuilt.m20;
            rebuilt.m20 = saved * cosine + rebuilt.m22 * sine;
            rebuilt.m22 = rebuilt.m22 * cosine - saved * sine;

            sine = NU_SIN_LUT(z);
            cosine = NU_COS_LUT(z);
            saved = rebuilt.m00;
            rebuilt.m00 = saved * cosine - rebuilt.m01 * sine;
            rebuilt.m01 = saved * sine + rebuilt.m01 * cosine;
            saved = rebuilt.m10;
            rebuilt.m10 = saved * cosine - rebuilt.m11 * sine;
            rebuilt.m11 = saved * sine + rebuilt.m11 * cosine;
            saved = rebuilt.m20;
            rebuilt.m20 = saved * cosine - rebuilt.m21 * sine;
            rebuilt.m21 = saved * sine + rebuilt.m21 * cosine;
        }
        matrix.m00 = rebuilt.m00;
        matrix.m01 = rebuilt.m01;
        matrix.m02 = rebuilt.m02;
        matrix.m03 = rebuilt.m03;
        matrix.m10 = rebuilt.m10;
        matrix.m11 = rebuilt.m11;
        matrix.m12 = rebuilt.m12;
        matrix.m13 = rebuilt.m13;
        matrix.m20 = rebuilt.m20;
        matrix.m21 = rebuilt.m21;
        matrix.m22 = rebuilt.m22;
        matrix.m23 = rebuilt.m23;
    }
    matrix.m33 = 1.0f;
    control->reference->SetMemberData(control->object, EdType_VuMtx, &source, 0, nullptr);
    // The original keeps the text buffer at a 16-byte aligned stack address.
    char text[128] __attribute__((aligned(16)));
    sprintf(text, "%.2f", changed_value);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), text);
}

void EdMatrixControl::cbButton(eduimenu_s *menu, eduiitem_s *item, u32 value) {
    static_cast<edui_prop_s *>(item)->unknown_property_flags |= 0x20;
    nupad_s *pad = EdControl::Input->pad;
    f32 changed = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
    f32 dx;
    f32 dy;
    eduiGetCursorDelta(&dx, &dy);
    changed -= dy * 100.0f;
    dy = eduiGetAnalougePadValue(pad);
    changed -= dy;
    char text[128];
    sprintf(text, "%.2f", changed);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), text);
    cbChanged(menu, item, value);
}

void SceneInstance::SetInitialTransform(VuMtx const *transform) {
    initial_transform = *reinterpret_cast<NUMTX const *>(transform);
}

void SceneInstance::SetCurrentTransform(VuMtx const *transform) {
    current_transform = *reinterpret_cast<NUMTX const *>(transform);
}

void BaseEditor::WriteBeginBlock(i32 file, i32 type) {
    blockStart[blockDepth++] = NuFilePos(file);
    NuFileWriteInt(file, type);
    NuFileWriteInt(file, 0);
    NuFileWriteInt(file, 0);
    NuFileWriteInt(file, 0);
}

void BaseEditor::WriteEndBlock(i32 file) {
    --blockDepth;
    NuFileAlign(file, 15);
    i32 end = NuFilePos(file);
    NuFileSeek(file, blockStart[blockDepth] + 4, NUFILE_SEEK_START);
    NuFileWriteInt(file, end - blockStart[blockDepth]);
    NuFileSeek(file, end, NUFILE_SEEK_START);
}

void BaseEditor::WriteMetaData(i32 file, i32 type, i32 version, i32 count) {
    NuFileWriteInt(file, 1);
    NuFileWriteInt(file, type);
    NuFileWriteInt(file, version);
    NuFileWriteInt(file, count);
}

void BaseEditor::ReadBuffer(void **destination, void *source, i32 size) {
    if (field_0x0c != 0) {
        memcpy(*destination, source, size);
    } else {
        *destination = source;
    }
}

ClassEditor::ClassEditor() {
    next = NULL;
    previous = NULL;
    first_tool = NULL;
    tool_count = 0;
    last_tool = NULL;
    selected_objects.last = NULL;
    selected_objects.count = 0;
    selected_objects.first = NULL;
    current_object.object = NULL;
    current_object.reference = NULL;
    current_object.ed_class = NULL;
    pending_object.object = NULL;
    pending_object.reference = NULL;
    pending_object.ed_class = NULL;
    snap_distance = 10.0f;
    manipulator = &theDefaultManipulator;
    class_filter = -1;
}

void ClassEditor::RegisterTool(EdTool &tool) {
    tool.next = NULL;
    tool.previous = last_tool;
    if (last_tool != NULL) {
        last_tool->next = &tool;
    }
    EdTool *old_first = first_tool;
    last_tool = &tool;
    if (old_first == NULL) {
        first_tool = &tool;
    }
    ++tool_count;
}

void ClassEditor::UpdateLists(MemoryBuffer *first, MemoryBuffer *second) {
    for (i32 i = 0; i < theRegistry.class_count; ++i) {
        EdClassInterface *interface = theRegistry.classes[i].interface;
        if (interface != NULL) {
            interface->vtable->update_lists(interface, first, second);
        }
    }
}

void ClassEditor::PreLoadInitialisation(MemoryBuffer *first, MemoryBuffer *second) {
    for (i32 i = 0; i < theRegistry.class_count; ++i) {
        EdClassInterface *interface = theRegistry.classes[i].interface;
        if (interface != NULL) {
            interface->vtable->pre_load_initialisation(interface, first, second);
        }
    }
}

void ClassEditor::PostLoadInitialisation(MemoryBuffer *first, MemoryBuffer *second) {
    for (i32 i = 0; i < theRegistry.class_count; ++i) {
        EdClassInterface *interface = theRegistry.classes[i].interface;
        if (interface != NULL) {
            interface->vtable->post_load_initialisation(interface, first, second);
        }
    }
}

void ClassEditor::PreSaveInitialisation() {
    for (i32 i = 0; i < theRegistry.class_count; ++i) {
        EdClassInterface *interface = theRegistry.classes[i].interface;
        if (interface != NULL) {
            interface->vtable->pre_save_initialisation(interface);
        }
    }
}

void ClassEditor::PostSaveInitialisation() {
    for (i32 i = 0; i < theRegistry.class_count; ++i) {
        EdClassInterface *interface = theRegistry.classes[i].interface;
        if (interface != NULL) {
            interface->vtable->post_save_initialisation(interface);
        }
    }
}

__attribute__((force_align_arg_pointer)) void ClassEditor::UpdateSnapRay(VuVec &position) {
    if (snap_mode == 1) {
        snap_ray.x = 0.0f;
        snap_ray.y = -1000.0f;
        snap_ray.z = 0.0f;
    } else if (snap_mode == 2) {
#if defined(__i386__) && defined(__SSE__)
        __m128 lanes = _mm_loadl_pi(_mm_setzero_ps(), reinterpret_cast<const __m64 *>(&position));
        lanes = _mm_loadh_pi(lanes, reinterpret_cast<const __m64 *>(&position.z));
        _mm_storel_pi(reinterpret_cast<__m64 *>(&snap_ray), lanes);
        _mm_storeh_pi(reinterpret_cast<__m64 *>(&snap_ray.z), lanes);
#else
        snap_ray = position;
#endif
    }
}

void *ClassEditor::CreateObject() {
    return NULL;
}

void ClassEditor::cbEdClassImportMenu(eduimenu_s *, eduiitem_s *, u32) {
}

void ClassEditor::cbEdClassExportMenu(eduimenu_s *, eduiitem_s *, u32) {
}

void ClassEditor::SetViewMenuHilight(eduimenu_s *menu) {
    for (eduiitem_s *item = menu->first; item != NULL; item = item->next) {
        i32 index = item->data - 3;
        if (static_cast<u32>(index) <= 60) {
            item->highlighted = (theClassEditor.class_filter >> index) & 1;
        }
    }
}

void ClassEditor::cbEdClassSetView(eduimenu_s *menu, eduiitem_s *item, u32) {
    i32 index = item->data;
    if (__builtin_expect(index == 1, 0)) {
        theClassEditor.class_filter = 0;
    } else if (static_cast<u32>(index) < 1) {
        theClassEditor.class_filter = -1;
    } else if (index == 2) {
        theClassEditor.class_filter = ~theClassEditor.class_filter;
    } else if (index - 3 >= 0) {
        theClassEditor.class_filter ^= 1 << (index - 3);
    }
    SetViewMenuHilight(menu);
}

i32 ClassObjectList::IsInList(ClassObject object) {
    for (ClassObjectListEntry *entry = first; entry != NULL; entry = entry->next) {
        if (entry->object == object.object && entry->reference == object.reference) {
            return true;
        }
    }
    return false;
}

i32 ClassEditor::IsSelectedObject(ClassObject &object) {
    return selected_objects.IsInList(object);
}

i32 ClassObjectList::IsInList(void *object, EdRef *reference) {
    ClassObjectListEntry *entry = first;
    if (entry == NULL) {
        return false;
    }

    if (reference != NULL) {
        do {
            if (entry->object == object && entry->reference == reference) {
                return true;
            }
            entry = entry->next;
        } while (entry != NULL);
    } else {
        do {
            if (entry->object == object) {
                return true;
            }
            entry = entry->next;
        } while (entry != NULL);
    }
    return false;
}

i32 ClassEditor::IsSelectedObject(void *object, EdRef *reference) {
    return selected_objects.IsInList(object, reference);
}

i32 ClassObjectList::IsInList(EdClass *ed_class) {
    for (ClassObjectListEntry *entry = first; entry != NULL; entry = entry->next) {
        if (entry->ed_class == ed_class) {
            return true;
        }
    }
    return false;
}

i32 ClassEditor::IsSelectedClass(EdClass *object_class) {
    return selected_objects.IsInList(object_class);
}

EdInputContext::EdInputContext() {
}

void EdInputContext::Set(i32 input, float value, float repeat_delay) {
    if (value != 0.0f) {
        float now = current_time;
        float repeat_threshold = repeat_window + now;
        values[input] = value;
        float next_repeat = repeat_times[input];
        pressed[input] = held[input] == 0;
        held[input] = 1;
        if (next_repeat >= repeat_threshold) {
            repeated[input] = 1;
            next_repeat = now;
        } else if (next_repeat == 0.0f) {
            repeated[input] = 1;
        }
        repeat_times[input] = next_repeat + repeat_delay;
        return;
    }

    values[input] = value;
    released[input] = held[input] != 0;
    repeat_times[input] = 0.0f;
    held[input] = 0;
}

void EdInputContext::Clear(i32 input) {
    if (static_cast<u32>(input) < 40) {
        values[input] = 0.0f;
        cleared[input] = 1;
    }
}

f32 EdInputContext::Get(i32 input) {
    if (static_cast<u32>(input) < 40) {
        return values[input];
    }
    return 0.0f;
}

f32 EdInputContext::GetPress(i32 input) {
    if (static_cast<u32>(input) < 40 && pressed[input] != 0) {
        return values[input];
    }
    return 0.0f;
}

f32 EdInputContext::GetRelease(i32 input) {
    if (static_cast<u32>(input) < 40 && released[input] != 0) {
        return values[input];
    }
    return 0.0f;
}

f32 EdInputContext::GetHold(i32 input) {
    if (static_cast<u32>(input) < 40 && held[input] != 0) {
        return values[input];
    }
    return 0.0f;
}

void ClassEditor::UpdateClassFilter(EdInputContext &input) {
    i32 count = theRegistry.class_count;
    if (count > 10) {
        count = 10;
    }
    for (i32 i = 0; i < count; ++i) {
        if (input.GetPress(i + 26) != 0.0f) {
            if (input.GetHold(22) != 0.0f) {
                class_filter = 1 << i;
            } else {
                class_filter ^= 1 << i;
            }
        }
    }
    if (input.GetPress(36) != 0.0f) {
        if (input.GetHold(22) != 0.0f) {
            class_filter = -1;
        } else {
            class_filter = ~class_filter;
        }
    }
}

f32 EdInputContext::GetRepeat(i32 input) {
    if (static_cast<u32>(input) < 40 && repeated[input] != 0) {
        return values[input];
    }
    return 0.0f;
}

EdVectorControl::EdVectorControl() {
}

void EdVectorControl::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    EdVectorControl *control = new (theMemoryManager.AllocPool(sizeof(EdVectorControl), 1)) EdVectorControl();
    if (!control)
        return;
    control->reference = member;
    control->object = target;
    VuVec vector;
    member->GetMemberData(target, EdType_VuVec, &vector, 0);
    control->item = eduiItemExpanderCreate(reinterpret_cast<usize>(control), &EdLevelAttr, cbSelected, member->name);
    eduiMenuAddItem(menu, control->item);
    char value[128];
    sprintf(value, "%.2f", vector.x);
    control->components[0] = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, cbSelected, cbChanged,
                                                cbButton, 2, const_cast<char *>("tx"), value);
    control->components[0]->unknown_10 = 1;
    eduiItemExpanderAddChild(static_cast<edui_expander_s *>(control->item), control->components[0]);
    sprintf(value, "%.2f", vector.y);
    control->components[1] = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, cbSelected, cbChanged,
                                                cbButton, 2, const_cast<char *>("ty"), value);
    control->components[1]->unknown_10 = 2;
    eduiItemExpanderAddChild(static_cast<edui_expander_s *>(control->item), control->components[1]);
    sprintf(value, "%.2f", vector.z);
    control->components[2] = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, cbSelected, cbChanged,
                                                cbButton, 2, const_cast<char *>("tz"), value);
    control->components[2]->unknown_10 = 3;
    eduiItemExpanderAddChild(static_cast<edui_expander_s *>(control->item), control->components[2]);
}

void EdVectorControl::Destroy() {
    if (components[0])
        components[0]->data_ptr = nullptr;
    if (components[1])
        components[1]->data_ptr = nullptr;
    if (components[2])
        components[2]->data_ptr = nullptr;
}

EdMatrixControl::EdMatrixControl() {
}

void EdMatrixControl::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    EdMatrixControl *control = new (theMemoryManager.AllocPool(sizeof(EdMatrixControl), 1)) EdMatrixControl();
    if (!control)
        return;
    control->reference = member;
    control->object = target;
    // Keep the matrix at the 16-byte aligned stack address used by the original.
    VuMtx matrix __attribute__((aligned(16)));
    member->GetMemberData(target, EdType_VuMtx, &matrix, 0);
    control->item = eduiItemExpanderCreate(reinterpret_cast<usize>(control), &EdLevelAttr, cbSelected, member->name);
    eduiMenuAddItem(menu, control->item);
    char value[128];
#define ADD_MATRIX_COMPONENT(index, label, number)                                                                     \
    sprintf(value, "%.2f", number);                                                                                    \
    control->components[index] = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, cbSelected,        \
                                                    cbChanged, cbButton, 2, const_cast<char *>(label), value);         \
    control->components[index]->unknown_10 = index % 3 + 1;                                                            \
    eduiItemExpanderAddChild(static_cast<edui_expander_s *>(control->item), control->components[index])
    // These enabled groups are the common path in the original block layout.
    if (__builtin_expect(member->attributes & 8, 1)) {
        ADD_MATRIX_COMPONENT(0, "pos x", matrix.matrix.m30);
        ADD_MATRIX_COMPONENT(1, "pos y", matrix.matrix.m31);
        ADD_MATRIX_COMPONENT(2, "pos z", matrix.matrix.m32);
    }
    if (__builtin_expect(member->attributes & 0x10, 1)) {
        NUANG x, y, z;
        NuMtxGetEulerXYZ(&matrix.matrix, &x, &y, &z);
        ADD_MATRIX_COMPONENT(3, "rot x", static_cast<f32>(x) * (360.0f / 65536.0f));
        ADD_MATRIX_COMPONENT(4, "rot y", static_cast<f32>(y) * (360.0f / 65536.0f));
        ADD_MATRIX_COMPONENT(5, "rot z", static_cast<f32>(z) * (360.0f / 65536.0f));
    }
    if (member->attributes & 0x20) {
        f32 x = NuVecMag(reinterpret_cast<NUVEC *>(&matrix.matrix.m00));
        f32 y = NuVecMag(reinterpret_cast<NUVEC *>(&matrix.matrix.m10));
        f32 z = NuVecMag(reinterpret_cast<NUVEC *>(&matrix.matrix.m20));
        ADD_MATRIX_COMPONENT(6, "scale x", x);
        ADD_MATRIX_COMPONENT(7, "scale y", y);
        ADD_MATRIX_COMPONENT(8, "scale z", z);
    }
}

void EdMatrixControl::Destroy() {
    if (components[0])
        components[0]->data_ptr = nullptr;
    if (components[1])
        components[1]->data_ptr = nullptr;
    if (components[2])
        components[2]->data_ptr = nullptr;
    if (components[3])
        components[3]->data_ptr = nullptr;
    if (components[4])
        components[4]->data_ptr = nullptr;
    if (components[5])
        components[5]->data_ptr = nullptr;
    if (components[6])
        components[6]->data_ptr = nullptr;
    if (components[7])
        components[7]->data_ptr = nullptr;
    if (components[8])
        components[8]->data_ptr = nullptr;
}

char *EdEnumControl::GetEnumString(i32 value) {
    for (Item *entry = items; entry->name != NULL; ++entry) {
        if (entry->value == value)
            return entry->name;
    }
    return "Unknown";
}

__attribute__((force_align_arg_pointer)) void EdBitControl::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    void *memory = theMemoryManager.AllocPool(sizeof(EdBitControl), 1);
    EdBitControl *control = new (memory) EdBitControl;
    control->reference = member;
    control->items = items;
    control->object = target;
    control->bit_mask = bit_mask;
    i32 value;
    member->GetMemberData(target, EdType_Int, &value, 0);
    control->item =
        eduiItemExpanderCreate(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected, member->name);
    eduiMenuAddItem(menu, control->item);
    for (i32 bit = 0; bit < 32; ++bit) {
        if ((control->bit_mask & (1u << bit)) == 0)
            continue;
        char bit_name[0x20];
        sprintf(bit_name, "%d", bit);
        char *text = GetEnumString((value >> bit) & 1);
        eduiitem_s *child = eduiItemPropCreateEx(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected,
                                                 cbChanged, cbButton, 1, bit_name, text, bit);
        eduiItemExpanderAddChild(reinterpret_cast<edui_expander_s *>(control->item), child);
    }
}

__attribute__((force_align_arg_pointer)) void EdBitControl::cbSelectItem(eduimenu_s *menu, eduiitem_s *item,
                                                                         u32 flags) {
    if (edBitControl != NULL) {
        Item *choice = static_cast<Item *>(item->data_ptr);
        eduiItemPropSetText(edBitItem, edBitControl->GetEnumString(choice->value));
        u32 value[3] __attribute__((aligned(16)));
        edBitControl->reference->GetMemberData(edBitControl->object, EdType_Int, value, 0);
        u32 bits;
        if (choice->value != 0) {
            bits = (1u << edBitIndex) | value[0];
        } else {
            bits = ~(1u << edBitIndex) & value[0];
        }
        value[0] = bits;
        edBitControl->reference->SetMemberData(edBitControl->object, EdType_Int, value, 0, NULL);
        cbEdLevelDestroyOnSelect(menu, item, flags);
    }
}

void EdEnumControl::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    void *memory = theMemoryManager.AllocPool(sizeof(EdEnumControl), 1);
    EdEnumControl *control = new (memory) EdEnumControl;
    control->reference = member;
    control->object = target;
    control->items = items;
    i32 value;
    member->GetMemberData(target, EdType_Int, &value, 0);
    control->item = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected, cbChanged,
                                       cbButton, 1, member->name, GetEnumString(value));
    eduiMenuAddItem(menu, control->item);
}

void EdEnumControl::Refresh() {
    i32 value;
    reference->GetMemberData(object, EdType_Int, &value, 0);
    eduiItemPropSetText(reinterpret_cast<edui_prop_s *>(item), GetEnumString(value));
}

void EdEnumControl::cbSelectItem(eduimenu_s *menu, eduiitem_s *item, u32 flags) {
    if (active_enum_control != NULL) {
        Item *choice = static_cast<Item *>(item->data_ptr);
        i32 value = choice->value;
        eduiItemPropSetText(static_cast<edui_prop_s *>(active_enum_control->item),
                            active_enum_control->GetEnumString(value));
        active_enum_control->reference->SetMemberData(active_enum_control->object, EdType_Int, &value, 0, NULL);
        cbEdLevelDestroyOnSelect(menu, item, flags);
    }
}

i32 EdEnumControl::GetEnumValue(char *name) {
    for (Item *entry = items; entry->name != NULL; ++entry) {
        if (NuStrICmp(entry->name, name) != 0)
            return entry->value;
    }
    return 0;
}

EdStringControl::EdStringControl() {
}

EdClassObjectNameControl::EdClassObjectNameControl() : selected{NULL, NULL, NULL} {
}

void EdStringControl::GetVal(char *value, i32 capacity) {
    if (reference->type_id == EdType_String)
        reference->GetMemberData(object, EdType_String, value, capacity);
}

void EdSfxNameControl::cbButton(eduimenu_s *menu, eduiitem_s *item, u32) {
    sfxNameControl = static_cast<EdSfxNameControl *>(item->data_ptr);
    eduimenu_s *choices =
        eduiMenuCreate(item->x + menu->width, item->y, 180, 250,
                       reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (choices) {
        char value[128];
        sfxNameControl->GetVal(value, sizeof(value));
        eduiMenuAddItem(choices, eduiItemSelCreate(static_cast<usize>(-1), item->colours, 0, 0, cbSelectSfx,
                                                   const_cast<char *>("None")));
        for (i32 id = 0;; ++id) {
            char *name = GetSfxName(id);
            if (!name)
                break;
            eduiMenuAddItem(choices, eduiItemSelCreate(static_cast<usize>(id), item->colours, 0, 0, cbSelectSfx, name));
        }
        choices->flags |= 1;
        eduiMenuAttach(menu, choices);
        eduiMenuFitWidth(choices, 5);
        reinterpret_cast<u8 *>(item)[0x4c] &= ~8;
    }
}

void EdStringControl::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    EdStringControl *control = new (theMemoryManager.AllocPool(sizeof(EdStringControl), 1)) EdStringControl();
    if (!control)
        return;
    control->reference = member;
    control->object = target;
    char value[128];
    control->GetVal(value, sizeof(value));
    control->item = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected, cbChanged,
                                       cbPress, 1, member->name, value);
    eduiMenuAddItem(menu, control->item);
}

void EdStringControl::Refresh() {
    char value[128];
    GetVal(value, sizeof(value));
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), value);
}

void EdClassObjectNameControl::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    void *memory = theMemoryManager.AllocPool(sizeof(EdClassObjectNameControl), 1);
    EdClassObjectNameControl *control = new (memory) EdClassObjectNameControl();
    if (control == NULL)
        return;
    control->reference = member;
    control->object = target;
    char value[128];
    control->GetVal(value, sizeof(value));
    control->item = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected, cbChanged,
                                       cbButton, 1, member->name, value);
    control->item->flags |= 4;
    eduiMenuAddItem(menu, control->item);
}

void EdStringControl::SetVal(char const *value) {
    if (reference->type_id != EdType_String)
        return;
    char copy[128];
    NuStrNCpy(copy, value, reference->size);
    reference->SetMemberData(object, EdType_String, copy, 0, nullptr);
}

void PlaceableNameControl::cbSelectObject(eduimenu_s *, eduiitem_s *item, u32) {
    if (edObjectNameControl != NULL) {
        Placeable *object = static_cast<Placeable *>(item->data_ptr);
        char const *name = object != NULL ? object->GetName() : "None";
        eduiItemPropSetText(static_cast<edui_prop_s *>(edObjectNameControl->item), const_cast<char *>(name));
        edObjectNameControl->SetVal(name);
    }
}

void EdSfxNameControl::cbSelectSfx(eduimenu_s *menu, eduiitem_s *item, u32) {
    if (sfxNameControl) {
        PlaySfxById(static_cast<i32>(item->data), NULL);
        char *name = GetSfxName(static_cast<i32>(item->data));
        if (!name)
            name = const_cast<char *>("None");
        eduiItemPropSetText(static_cast<edui_prop_s *>(sfxNameControl->item), name);
        sfxNameControl->SetVal(name);
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }
}

void EdStringControl::cbChanged(eduimenu_s *, eduiitem_s *item, u32) {
    static_cast<EdStringControl *>(item->data_ptr)->SetVal(static_cast<edui_prop_s *>(item)->property_text);
}

EdColourControl::EdColourControl() {
}

void EdColourControl::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    EdColourControl *control = new (theMemoryManager.AllocPool(sizeof(EdColourControl), 1)) EdColourControl();
    if (control == NULL)
        return;
    control->reference = member;
    control->object = target;
    NUVEC colour;
    member->GetMemberData(target, EdType_Colour3, &colour, 0);
    char name[128];
    strcpy(name, member->name);
    char value[128];
    sprintf(value, "%.2f %.2f %.2f", colour.x, colour.y, colour.z);
    control->item = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected, cbChanged,
                                       cbButton, 3, name, value);
    set_colour_preview(control->item, colour);
    eduiMenuAddItem(menu, control->item);
}

EdSfxNameControl::EdSfxNameControl() {
}

void EdSfxNameControl::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    EdSfxNameControl *control = new (theMemoryManager.AllocPool(sizeof(EdSfxNameControl), 1)) EdSfxNameControl();
    if (control) {
        control->reference = member;
        control->object = target;
        char value[128];
        control->GetVal(value, sizeof(value));
        control->item = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected,
                                           cbChanged, cbButton, 1, member->name, value);
        eduiMenuAddItem(menu, control->item);
    }
}

void EdDrawBegin(i32 material) {
    if (EdDrawMtl[0] == NULL) {
        EdDrawMtl[0] = NuMtlCreate3D(1);
        EdDrawMtl[0]->attribs.cull_mode = 2;
        EdDrawMtl[0]->attribs.alpha_mode = 1;
        NuMtlUpdate(EdDrawMtl[0]);
        EdDrawMtl[1] = NuMtlCreate3D(1);
        EdDrawMtl[1]->attribs.cull_mode = 2;
        EdDrawMtl[1]->attribs.z_mode = 3;
        EdDrawMtl[1]->attribs.alpha_mode = 1;
        NuMtlUpdate(EdDrawMtl[1]);
    }
    NewPrim = 1;
    NewMtl = material;
}

void EdDrawEnd() {
    if (NewPrim) {
        if (NewPrim == 2 || NewPrim == 3) {
            NuPrim3DEnd();
        }
        NewPrim = 0;
    }
}

void EdDrawMtx(VuMtx const *matrix) {
    EdDrawEnd();
    EdDrawBegin(0);
    NewMtx = matrix;
}

void EdDrawLineSegment(VuVec const &a, VuVec const &b, i32 colour) {
    if (__builtin_expect(NewPrim == 1, 0)) {
        NuPrim3DBegin(2, 5, EdDrawMtl[NewMtl], NewMtx ? const_cast<NUMTX *>(&NewMtx->matrix) : NULL);
        NewPrim = 2;
    }
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(a.x, a.y, a.z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(b.x, b.y, b.z);
}

void EdDrawLineCross(VuVec const &position, float size, i32 colour) {
    VuVec start;
    VuVec end;
    start.y = end.y = position.y;
    start.z = end.z = position.z;
    start.x = position.x - size;
    end.x = position.x + size;
    EdDrawLineSegment(start, end, colour);

    start.x = end.x = position.x;
    start.y = position.y - size;
    end.y = position.y + size;
    EdDrawLineSegment(start, end, colour);

    start.y = end.y = position.y;
    start.z = position.z - size;
    end.z = position.z + size;
    EdDrawLineSegment(start, end, colour);
}

void EdDrawLineCircleX(VuVec const &centre, float radius, i32 colour, i32 segments) {
    EdDrawCircle<0>(centre, radius, colour, segments);
}

void EdDrawLineCircleY(VuVec const &centre, float radius, i32 colour, i32 segments) {
    EdDrawCircle<1>(centre, radius, colour, segments);
}

void KnotHelper::Render(void *object, i32) {
    SplineKnot *knot = static_cast<SplineKnot *>(object);
    EdDrawBegin(0);
    EdDrawLineCircleY(knot->position, 0.25f * EdManipulator::Scale, static_cast<i32>(0x80808080), 8);
    EdDrawLineCircleY(knot->in_tangent, 0.25f * EdManipulator::Scale, static_cast<i32>(0x80800000), 8);
    EdDrawLineCircleY(knot->out_tangent, 0.25f * EdManipulator::Scale, static_cast<i32>(0x80000080), 8);
    EdDrawLineSegment(knot->position, knot->in_tangent, -1);
    EdDrawLineSegment(knot->position, knot->out_tangent, -1);
    EdDrawEnd();
}

void EdDrawLineCircleZ(VuVec const &centre, float radius, i32 colour, i32 segments) {
    EdDrawCircle<2>(centre, radius, colour, segments);
}

void EdDrawLineArrow(VuMtx const &transform, float size, i32 colour) {
    VuVec points[4] = {
        VuVec(0.0f, 0.0f, -size, 0.0f),
        VuVec(0.0f, 0.0f, size, 0.0f),
        VuVec(size * 0.5f, 0.0f, 0.0f, 0.0f),
        VuVec(-size * 0.5f, 0.0f, 0.0f, 0.0f),
    };
    NUMTX *matrix = const_cast<NUMTX *>(&transform.matrix);
    NuVecMtxTransform(&points[0].xyz, &points[0].xyz, matrix);
    NuVecMtxTransform(&points[1].xyz, &points[1].xyz, matrix);
    NuVecMtxTransform(&points[2].xyz, &points[2].xyz, matrix);
    NuVecMtxTransform(&points[3].xyz, &points[3].xyz, matrix);
    EdDrawLineSegment(points[0], points[1], colour);
    EdDrawLineSegment(points[1], points[2], colour);
    EdDrawLineSegment(points[1], points[3], colour);
    EdDrawLineSegment(points[2], points[3], colour);
}

void EdDrawLineCube(VuMtx const &transform, float size, i32 colour) {
    VuVec points[9] = {
        VuVec(size, size, -size, 0.0f),  VuVec(size, size, size, 0.0f),    VuVec(-size, size, size, 0.0f),
        VuVec(-size, size, -size, 0.0f), VuVec(size, -size, -size, 0.0f),  VuVec(size, -size, size, 0.0f),
        VuVec(-size, -size, size, 0.0f), VuVec(-size, -size, -size, 0.0f), VuVec(0.0f, 0.0f, size, 0.0f),
    };
    NUMTX *matrix = const_cast<NUMTX *>(&transform.matrix);
    NuVecMtxTransform(&points[0].xyz, &points[0].xyz, matrix);
    NuVecMtxTransform(&points[1].xyz, &points[1].xyz, matrix);
    NuVecMtxTransform(&points[2].xyz, &points[2].xyz, matrix);
    NuVecMtxTransform(&points[3].xyz, &points[3].xyz, matrix);
    NuVecMtxTransform(&points[4].xyz, &points[4].xyz, matrix);
    NuVecMtxTransform(&points[5].xyz, &points[5].xyz, matrix);
    NuVecMtxTransform(&points[6].xyz, &points[6].xyz, matrix);
    NuVecMtxTransform(&points[7].xyz, &points[7].xyz, matrix);
    NuVecMtxTransform(&points[8].xyz, &points[8].xyz, matrix);
    EdDrawLineSegment(points[0], points[1], colour);
    EdDrawLineSegment(points[1], points[2], colour);
    EdDrawLineSegment(points[2], points[3], colour);
    EdDrawLineSegment(points[3], points[0], colour);
    EdDrawLineSegment(points[0], points[2], colour);
    EdDrawLineSegment(points[1], points[3], colour);
    EdDrawLineSegment(points[4], points[5], colour);
    EdDrawLineSegment(points[5], points[6], colour);
    EdDrawLineSegment(points[6], points[7], colour);
    EdDrawLineSegment(points[7], points[4], colour);
    EdDrawLineSegment(points[0], points[4], colour);
    EdDrawLineSegment(points[1], points[5], colour);
    EdDrawLineSegment(points[2], points[6], colour);
    EdDrawLineSegment(points[3], points[7], colour);
    EdDrawLineSegment(points[5], points[8], colour);
    EdDrawLineSegment(points[6], points[8], colour);
}

void EdDrawLineSphere(VuVec const &center, float radius, float scale, i32 colour) {
    const float radius_squared = radius * radius;
    VuVec first;
    VuVec second;
    for (i32 latitude = 0; latitude < 8; ++latitude) {
        float first_radius;
        float second_radius;
        if (latitude < 4)
            first_radius = radius * NU_SIN_LUT(latitude * 0x1000);
        else
            first_radius = radius * NU_SIN_LUT((8 - latitude) * 0x1000);
        if (latitude < 3)
            second_radius = radius * NU_SIN_LUT((latitude + 1) * 0x1000);
        else
            second_radius = radius * NU_SIN_LUT((7 - latitude) * 0x1000);
        float first_height = NuFsqrt(radius_squared - first_radius * first_radius) * scale;
        float second_height = NuFsqrt(radius_squared - second_radius * second_radius) * scale;
        if (latitude >= 5)
            first_height = -first_height;
        if (latitude >= 4)
            second_height = -second_height;
        first_height += center.y;
        second_height += center.y;
#define ED_DRAW_SPHERE_LONGITUDE(angle, next_angle)                                                                    \
    {                                                                                                                  \
        first = VuVec(center.x + first_radius * NU_COS_LUT(angle), first_height,                                       \
                      center.z + first_radius * NU_SIN_LUT(angle), 1.0f);                                              \
        second = VuVec(center.x + second_radius * NU_COS_LUT(angle), second_height,                                    \
                       center.z + second_radius * NU_SIN_LUT(angle), 1.0f);                                            \
        EdDrawLineSegment(first, second, colour);                                                                      \
        if (latitude != 0) {                                                                                           \
            second = VuVec(center.x + first_radius * NU_COS_LUT(next_angle), first_height,                             \
                           center.z + first_radius * NU_SIN_LUT(next_angle), 1.0f);                                    \
            EdDrawLineSegment(first, second, colour);                                                                  \
        }                                                                                                              \
    }
        ED_DRAW_SPHERE_LONGITUDE(0x0000, 0x2000);
        ED_DRAW_SPHERE_LONGITUDE(0x2000, 0x4000);
        ED_DRAW_SPHERE_LONGITUDE(0x4000, 0x6000);
        ED_DRAW_SPHERE_LONGITUDE(0x6000, 0x8000);
        ED_DRAW_SPHERE_LONGITUDE(0x8000, 0xa000);
        ED_DRAW_SPHERE_LONGITUDE(0xa000, 0xc000);
        ED_DRAW_SPHERE_LONGITUDE(0xc000, 0xe000);
        ED_DRAW_SPHERE_LONGITUDE(0xe000, 0x10000);
    }
}

void EdSpecialObjectControl::Render() {
    if (menu != NULL) {
        Placeable *object = reinterpret_cast<Placeable *>(menu);
        VuVec center = *object->GetCurrentPosition();
        f32 radius = object->GetRadius();
        EdDrawBegin(0);
        EdDrawLineSphere(center, radius, 1.0f, static_cast<i32>(0x80808080));
        EdDrawEnd();
    }
}

void EdDrawPolyTri(VuVec const &a, VuVec const &b, VuVec const &c, i32 colour) {
    if (__builtin_expect(NewPrim == 1, 0)) {
        NuPrim3DBegin(0, 5, EdDrawMtl[NewMtl], NewMtx ? const_cast<NUMTX *>(&NewMtx->matrix) : NULL);
        NewPrim = 3;
    }
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(a.x, a.y, a.z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(b.x, b.y, b.z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(c.x, c.y, c.z);
}

void EdDrawPolySector(VuVec const &center, float radius, i32 axis, i32 first_angle, i32 last_angle, i32 colour,
                      i32 segments) {
    i32 step = 0x1000;
    if (segments != 0)
        step = 0x10000 / segments;
    i32 angle = first_angle;
    i32 remaining;
    if (first_angle < last_angle) {
        remaining = (last_angle - first_angle) & 0xffff;
        if (remaining >= 0x8000)
            return;
    } else {
        remaining = (first_angle - last_angle) & 0xffff;
        if (remaining >= 0x8000)
            return;
        angle = last_angle;
    }
    if (remaining <= 0)
        return;
    const float negative_radius = -radius;
    do {
        const i32 previous_angle = angle;
        if (remaining > step) {
            angle += step;
            remaining -= step;
        } else {
            angle += remaining;
            remaining = 0;
        }
        VuVec first;
        VuVec second;
        switch (axis) {
            case 1:
                first.w = 1.0f;
                first.x = negative_radius * 0.0f;
                first.y = negative_radius * 0.0f;
                first.z = negative_radius;
                NuVecRotateY(&first.xyz, &first.xyz, -previous_angle);
                second.w = 1.0f;
                second.y = negative_radius * 0.0f;
                second.x = negative_radius * 0.0f;
                second.z = negative_radius;
                NuVecRotateY(&second.xyz, &second.xyz, -angle);
                break;
            case 2:
                first.w = 1.0f;
                first.x = 0.0f * radius;
                first.y = radius;
                first.z = 0.0f * radius;
                NuVecRotateZ(&first.xyz, &first.xyz, -previous_angle);
                second.w = 1.0f;
                second.x = 0.0f * radius;
                second.y = radius;
                second.z = 0.0f * radius;
                NuVecRotateZ(&second.xyz, &second.xyz, -angle);
                break;
            case 0:
                first.w = 1.0f;
                first.x = 0.0f * radius;
                first.y = 0.0f * radius;
                first.z = radius;
                NuVecRotateX(&first.xyz, &first.xyz, -previous_angle);
                second.w = 1.0f;
                second.y = 0.0f * radius;
                second.x = 0.0f * radius;
                second.z = radius;
                NuVecRotateX(&second.xyz, &second.xyz, -angle);
                break;
        }
        first.x += center.x;
        first.y += center.y;
        first.z += center.z;
        second.x += center.x;
        second.y += center.y;
        second.z += center.z;
        EdDrawPolyTri(center, first, second, colour);
    } while (remaining > 0);
}

void EdDrawPolyCylinder(VuMtx const &transform, float half_length, float radius, float end_radius, i32 sides,
                        i32 colour, i32 cap_start, i32 cap_end) {
    const float taper = end_radius / radius;
    i32 shaded_colour = colour & 0xff000000;
    shaded_colour |= ((colour & 0xff) * 0xdc) >> 8;
    shaded_colour |= ((((colour >> 8) & 0xff) * 0xdc) & 0xff00) | (((((colour >> 16) & 0xff) * 0xdc) >> 8) << 16);
    VuVec first;
    VuVec second;
    VuVec third;
    EdDrawMtx(&transform);
    i32 segment_colour = 0;
    float previous_sine = NU_SIN_LUT(0) * radius;
    float previous_cosine = NU_COS_LUT(0) * radius;
    for (i32 segment = 0; segment < sides; ++segment) {
        segment_colour = (segment & 1) == 0 ? colour : shaded_colour;
        const i32 angle = (segment + 1) * 0x10000 / sides;
        const float sine = NU_SIN_LUT(angle) * radius;
        const float cosine = NU_COS_LUT(angle) * radius;
        first = VuVec(previous_sine * taper, previous_cosine * taper, half_length, 1.0f);
        second = VuVec(sine * taper, cosine * taper, half_length, 1.0f);
        third = VuVec(sine, cosine, -half_length, 1.0f);
        EdDrawPolyTri(first, second, third, segment_colour);
        first = VuVec(previous_sine * taper, previous_cosine * taper, half_length, 1.0f);
        second = VuVec(sine, cosine, -half_length, 1.0f);
        third = VuVec(previous_sine, previous_cosine, -half_length, 1.0f);
        EdDrawPolyTri(first, second, third, segment_colour);
        previous_sine = sine;
        previous_cosine = cosine;
    }
    if (cap_start != 0 || cap_end != 0) {
        float first_sine = NU_SIN_LUT(0) * radius;
        float first_cosine = NU_COS_LUT(0) * radius;
        const i32 first_angle = 0x10000 / sides;
        float previous_sine = NU_SIN_LUT(first_angle) * radius;
        float previous_cosine = NU_COS_LUT(first_angle) * radius;
        for (i32 segment = 0; segment < sides - 2; ++segment) {
            const i32 angle = (segment + 2) * 0x10000 / sides;
            const float sine = NU_SIN_LUT(angle) * radius;
            const float cosine = NU_COS_LUT(angle) * radius;
            if (cap_end != 0) {
                first = VuVec(first_sine * taper, first_cosine * taper, half_length, 1.0f);
                second = VuVec(sine * taper, cosine * taper, half_length, 1.0f);
                third = VuVec(previous_sine * taper, previous_cosine * taper, half_length, 1.0f);
                EdDrawPolyTri(first, second, third, segment_colour);
            }
            if (cap_start != 0) {
                first = VuVec(first_sine, first_cosine, -half_length, 1.0f);
                second = VuVec(previous_sine, previous_cosine, -half_length, 1.0f);
                third = VuVec(sine, cosine, -half_length, 1.0f);
                EdDrawPolyTri(first, second, third, segment_colour);
            }
            previous_sine = sine;
            previous_cosine = cosine;
        }
    }
    EdDrawMtx(NULL);
}

__attribute__((force_align_arg_pointer)) void EdDrawPolyCylinder(VuVec const &start, VuVec const &end, i32 sides,
                                                                 i32 colour, i32 cap_colour, float radius, float limit,
                                                                 float offset) {
    VuVec direction(end.x - start.x, end.y - start.y, end.z - start.z, 0.0f);
    const float length = NuVecMag(&direction.xyz);
    if (!(length > 0.0f))
        return;
    const float inverse_length = 1.0f / length;
    direction.x *= inverse_length;
    direction.y *= inverse_length;
    direction.z *= inverse_length;
    NUANGVEC angles;
    if (direction.x == 0.0f && direction.z == 0.0f) {
        angles.x = 0x2000;
        angles.y = 0;
    } else {
        angles.y = NuAtan2D(direction.x, direction.z);
        NuVecRotateY(&direction.xyz, &direction.xyz, -angles.y);
        angles.x = -NuAtan2D(direction.y, direction.z);
    }
    angles.z = 0;
    VuVec center((end.x + start.x) * 0.5f, (end.y + start.y) * 0.5f, (end.z + start.z) * 0.5f, 0.0f);
    VuMtx transform;
    NuMtxSetRotateXYZVU0(&transform.matrix, &angles);
    NuMtxTranslate(&transform.matrix, &center.xyz);
    const float half_length = length * 0.5f;
    const float minimum_radius = radius * half_length;
    float first_radius = limit > minimum_radius ? limit : minimum_radius;
    if (first_radius > offset)
        first_radius = offset;
    float second_radius = limit > minimum_radius * 0.1f ? limit : minimum_radius * 0.1f;
    if (second_radius > offset)
        second_radius = offset;
    EdDrawPolyCylinder(transform, half_length, first_radius, second_radius, sides, colour, 1, 1);
}

void EdDrawPolyArrow(VuVec const &start, VuVec const &end, i32 sides, i32 colour, float radius, float limit,
                     float radius_factor, float radius_offset) {
    VuVec displacement(end.x - start.x, end.y - start.y, end.z - start.z, 0.0f);
    const float length = NuVecMag(&displacement.xyz);
    if (!(length > 0.0f))
        return;
    const float inverse_length = 1.0f / length;
    VuVec direction(displacement.x * inverse_length, displacement.y * inverse_length, displacement.z * inverse_length,
                    0.0f);
    const float half_length = length * 0.4f;
    const float minimum_radius = radius_factor * half_length + radius_offset;

    NUANGVEC angles;
    if (direction.x == 0.0f && direction.z == 0.0f) {
        angles.x = -0x4000;
        angles.y = 0;
    } else {
        angles.y = NuAtan2D(direction.x, direction.z);
        NuVecRotateY(&direction.xyz, &direction.xyz, -angles.y);
        angles.x = -NuAtan2D(direction.y, direction.z);
    }
    angles.z = 0;
    const float draw_radius = radius > minimum_radius ? radius : minimum_radius;
    const float draw_limit = limit < draw_radius ? limit : draw_radius;

    VuMtx transform;
    VuVec center(start.x + displacement.x * 0.4f, start.y + displacement.y * 0.4f, start.z + displacement.z * 0.4f,
                 0.0f);
    NuMtxSetRotateXYZVU0(&transform.matrix, &angles);
    NuMtxTranslate(&transform.matrix, &center.xyz);
    EdDrawPolyCylinder(transform, half_length, draw_limit, draw_limit, sides, colour, 1, 0);
    NuMtxTranslateNeg(&transform.matrix, &center.xyz);
    center = VuVec(end.x - displacement.x * 0.1f, end.y - displacement.y * 0.1f, end.z - displacement.z * 0.1f, 0.0f);
    NuMtxTranslate(&transform.matrix, &center.xyz);
    EdDrawPolyCylinder(transform, half_length * 0.25f, draw_limit * 1.6f, 0.0f, sides, colour, 1, 0);
}

void EdDrawPolyAxis(VuMtx const &transform, float size, i32 opacity) {
    const NUMTX &matrix = transform.matrix;
    const VuVec &origin = *reinterpret_cast<const VuVec *>(&matrix.m30);
    VuVec tip(matrix.m30 + matrix.m00 * size, matrix.m31 + matrix.m01 * size, matrix.m32 + matrix.m02 * size, 0.0f);
    const float width = size * 0.02f;
    EdDrawPolyArrow(origin, tip, 8, static_cast<i32>(0xff000000 | (opacity & 0xff)), width, width, 0.02f, 0.0f);
    tip = VuVec(matrix.m30 + matrix.m10 * size, matrix.m31 + matrix.m11 * size, matrix.m32 + matrix.m12 * size, 0.0f);
    EdDrawPolyArrow(origin, tip, 8, static_cast<i32>(0xff000000 | ((static_cast<u32>(opacity) << 8) & 0xffff)), width,
                    width, 0.02f, 0.0f);
    tip = VuVec(matrix.m30 + matrix.m20 * size, matrix.m31 + matrix.m21 * size, matrix.m32 + matrix.m22 * size, 0.0f);
    EdDrawPolyArrow(origin, tip, 8, static_cast<i32>(0xff000000 | ((opacity & 0xff) << 16)), width, width, 0.02f, 0.0f);
}

void eduiSetPinnedMenu(eduimenu_s *menu) {
    edLevelPinnedMenu = menu;
}

void ClassEditor::cbEdClassSetPinned(eduimenu_s *menu, eduiitem_s *item, u32) {
    menu->flags ^= 4;
    item->highlighted = !item->highlighted;
    if (__builtin_expect(!!(menu->flags & 4), 0)) {
        eduiMenuDetach(menu);
        eduiSetPinnedMenu(menu);
    } else {
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
        if (__builtin_expect(edLevelActiveMenu == menu, 1)) {
            edLevelActiveMenu = NULL;
        }
        eduiSetPinnedMenu(NULL);
    }
}

void edSetPadDisabled(i32 disabled) {
    pad_disabled = disabled;
}

i32 edGetPadDisabled() {
    return pad_disabled;
}

void EdInputContext::Update(nucamera_s *camera, nupad_s *new_pad, float elapsed, bool) {
    static __used__ volatile u8 UseMouse;
    pad = new_pad;

    f32 *view = reinterpret_cast<f32 *>(reserved_00);
    view[0] = camera->mtx.m30;
    view[1] = camera->mtx.m31;
    view[2] = camera->mtx.m32;
    view[4] = camera->mtx.m10;
    view[5] = camera->mtx.m11;
    view[6] = camera->mtx.m12;
    view[4] *= 1000.0f;
    view[5] *= 1000.0f;
    view[6] *= 1000.0f;

    f32 cursor_x = 0.5f;
    f32 cursor_y = 0.5f;
    if (eduiUsedAlgPad(new_pad) > 0) {
        eduiSetCursorCoords(cursor_x, cursor_y);
        UseMouse = 0;
    } else {
        eduiGetCursorCoords(&cursor_x, &cursor_y);
    }
    NUVEC ray_end;
    NuCameraCalcRay(cursor_x, cursor_y, reinterpret_cast<NUVEC *>(reserved_00 + 0x20), &ray_end, camera);
    view[12] = ray_end.x - view[8];
    view[13] = ray_end.y - view[9];
    view[14] = ray_end.z - view[10];
    view[15] = 0.0f;
    delta_time = elapsed;

    // The original editor suppresses its input context while a property text
    // field is being edited, releasing held actions before returning.
    if (eduiPropTextPos >= 0) {
        for (i32 input_index = 0; input_index < 40; ++input_index)
            Set(input_index, 0.0f, elapsed);
        memset(cleared, 0, sizeof(cleared));
        return;
    }

    const i32 shift_or_s = NuKeyboard(0x2a) | NuKeyboard(0x36) | NuKeyboard(0x1f);
    const i32 alt_or_space = NuKeyboard(0x38) | NuKeyboard(0xb8) | NuKeyboard(0x39);
    const i32 control_or_c = NuKeyboard(0x1d) | NuKeyboard(0x9d) | NuKeyboard(0x2e);
    const i32 left_click = (NuMouseReadButtons() == 1 || (pad->digital_buttons & 0x800) != 0) && !alt_or_space;
    const f32 mouse_x = NuMouseReadXVel();
    const f32 mouse_y = NuMouseReadYVel();
    const f32 mouse_z = NuMouseReadZVel();
    u32 square = 0;
    i32 right_click;
    if (edGetPadDisabled() == 0) {
        if (eduiGetActiveMenu() == NULL) {
            const u32 buttons = pad->digital_buttons;
            square = buttons & 0x80;
            Set(38, static_cast<f32>(static_cast<i32>(buttons & 0x40)), elapsed);
        }
        right_click = NuMouseReadButtons() == 2 || (pad->digital_buttons & 0x20) != 0;
    } else {
        right_click = NuMouseReadButtons() == 2;
    }
    Set(0, mouse_x, elapsed);
    Set(1, mouse_y, elapsed);
    Set(2, mouse_z, elapsed);
    Set(3, static_cast<i32>(left_click && !control_or_c), elapsed);
    Set(4, static_cast<i32>(right_click && !alt_or_space && !control_or_c), elapsed);
    Set(5, static_cast<f32>(NuKeyboard(0x10)), elapsed);
    Set(6, static_cast<f32>(NuKeyboard(0x11)), elapsed);
    Set(7, static_cast<f32>(NuKeyboard(0x12)), elapsed);
    Set(8, static_cast<f32>(NuKeyboard(0x13)), elapsed);
    Set(9, static_cast<f32>(NuKeyboard(0x21)), elapsed);
    Set(10, static_cast<f32>(NuKeyboard(0x22)), elapsed);
    Set(11, static_cast<f32>(NuKeyboard(0xd2)), elapsed);
    Set(12, static_cast<f32>(NuKeyboard(0xd3)), elapsed);
    Set(13, static_cast<f32>(alt_or_space), elapsed);
    Set(14, static_cast<f32>(NuKeyboard(0x0d)), elapsed);
    Set(15, static_cast<f32>(NuKeyboard(0x0c)), elapsed);
    Set(16, static_cast<f32>(shift_or_s), elapsed);
    Set(17, static_cast<f32>(NuKeyboard(0x1b)), elapsed);
    Set(18, static_cast<f32>(NuKeyboard(0x1a)), elapsed);
    Set(19, static_cast<f32>(NuKeyboard(0xcd)), elapsed);
    Set(20, static_cast<f32>(NuKeyboard(0xcb)), elapsed);
    Set(21, static_cast<f32>(shift_or_s), elapsed);
    Set(22, static_cast<f32>(control_or_c), elapsed);
    Set(23, static_cast<i32>(NuKeyboard(0x1f) && control_or_c), elapsed);
    Set(24, static_cast<f32>(NuKeyboard(0x01)), elapsed);
    Set(25, static_cast<i32>(left_click && control_or_c), elapsed);
    Set(26, static_cast<f32>(NuKeyboard(2)), elapsed);
    Set(27, static_cast<f32>(NuKeyboard(3)), elapsed);
    Set(28, static_cast<f32>(NuKeyboard(4)), elapsed);
    Set(29, static_cast<f32>(NuKeyboard(5)), elapsed);
    Set(30, static_cast<f32>(NuKeyboard(6)), elapsed);
    Set(31, static_cast<f32>(NuKeyboard(7)), elapsed);
    Set(32, static_cast<f32>(NuKeyboard(8)), elapsed);
    Set(33, static_cast<f32>(NuKeyboard(9)), elapsed);
    Set(34, static_cast<f32>(NuKeyboard(10)), elapsed);
    Set(35, static_cast<f32>(NuKeyboard(11)), elapsed);
    Set(37, static_cast<f32>(static_cast<i32>(square)), elapsed);
    Set(39, static_cast<f32>(static_cast<i32>(pad->digital_buttons & 0x800)), elapsed);
    memset(cleared, 0, sizeof(cleared));
}

void LevelEditor::SetPadText(i32 buttons, char *text) {
    for (i32 i = 0; i < 32; ++i) {
        if ((static_cast<u32>(buttons) & (1u << i)) != 0) {
            pad_text[i] = text;
        }
    }
}

void LevelEditor::RegisterEditor(BaseEditor &editor) {
    editor.next = NULL;
    editor.previous = last_editor;
    if (last_editor != NULL) {
        last_editor->next = &editor;
    }
    BaseEditor *old_first = first_editor;
    last_editor = &editor;
    if (old_first == NULL) {
        first_editor = &editor;
    }
    ++editor_count;
}

void LevelEditor::BeginMultiLoad(variptr_u *buffer, variptr_u *buffer_end) {
    // The original keeps these stream descriptors 16-byte aligned in a realigned frame.
    MemoryBuffer source
        __attribute__((aligned(16))) = {buffer, buffer_end, 0, static_cast<u32>(buffer_end->addr - buffer->addr)};
    if (editor_buffer_from_front == 0) {
        editor_buffer_end = *buffer_end;
        editor_buffer_begin.addr = buffer_end->addr - 0x20000;
    }
    multi_load_active = 1;
    editor_buffer_cursor = editor_buffer_begin;
    MemoryBuffer scratch
        __attribute__((aligned(16))) = {&editor_buffer_cursor, &editor_buffer_end, 0,
                                        static_cast<u32>(editor_buffer_end.addr - editor_buffer_begin.addr)};
    theClassEditor.PreLoadInitialisation(&source, &scratch);
}

void LevelEditor::EndMultiLoad(variptr_u *buffer, variptr_u *buffer_end) {
    MemoryBuffer source
        __attribute__((aligned(16))) = {buffer, buffer_end, 0, static_cast<u32>(buffer_end->addr - buffer->addr)};
    if (editor_buffer_from_front == 0) {
        editor_buffer_end = *buffer_end;
        editor_buffer_begin.addr = buffer_end->addr - 0x20000;
    }
    editor_buffer_cursor = editor_buffer_begin;
    MemoryBuffer scratch
        __attribute__((aligned(16))) = {&editor_buffer_cursor, &editor_buffer_end, 0,
                                        static_cast<u32>(editor_buffer_end.addr - editor_buffer_begin.addr)};
    theClassEditor.PostLoadInitialisation(&source, &scratch);
    current_led_file = -1;
    multi_load_active = 0;
}

i32 LoadState(i32, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *) {
    return 0;
}

void LevelEditor::SetSaveFilename(char *name) {
    if (name != NULL) {
        NuStrCpy(save_filename, name);
        NuStrCpy(editor_filename, save_filename);
        char *extension = NuStrRChr(editor_filename, '.');
        if (extension != NULL) {
            *extension = '\0';
        }
        NuStrCat(editor_filename, ".led");
    } else {
        save_filename[0] = '\0';
        editor_filename[0] = '\0';
    }
}

void LevelEditor::Reset() {
    reset_pending = 0;
}

i32 LevelEditor::AddScene(char *name, nugscn_s *scene, i32 active) {
    for (i32 i = 0; i < reset_pending; ++i) {
        if (NuStrICmp(scenes[i].name, name) == 0) {
            scenes[i].scene = scene;
            scenes[i].active = active;
            return i;
        }
    }
    if (reset_pending < 10) {
        i32 index = reset_pending++;
        NuStrNCpy(scenes[index].name, name, 32);
        scenes[index].scene = scene;
        scenes[index].active = active;
        return index;
    }
    return 0;
}

i32 LevelEditor::FindSceneId(char *name) {
    if (__builtin_expect(NuStrICmp(scenes[0].name, name) == 0, 0))
        return 0;
    if (__builtin_expect(NuStrICmp(scenes[1].name, name) == 0, 0))
        return 1;
    if (__builtin_expect(NuStrICmp(scenes[2].name, name) == 0, 0))
        return 2;
    if (__builtin_expect(NuStrICmp(scenes[3].name, name) == 0, 0))
        return 3;
    if (NuStrICmp(scenes[4].name, name) == 0)
        return 4;
    if (__builtin_expect(NuStrICmp(scenes[5].name, name) == 0, 0))
        return 5;
    if (__builtin_expect(NuStrICmp(scenes[6].name, name) == 0, 0))
        return 6;
    if (__builtin_expect(NuStrICmp(scenes[7].name, name) == 0, 0))
        return 7;
    if (__builtin_expect(NuStrICmp(scenes[8].name, name) == 0, 0))
        return 8;
    // The original's unsigned carry test accepts only an exact match here.
    return (NuStrICmp(scenes[9].name, name) == 0) * 10 - 1;
}

nugscn_s *LevelEditor::GetScene(char *name) {
    for (i32 i = 0; i < reset_pending; ++i) {
        if (NuStrICmp(name, scenes[i].name) == 0) {
            return scenes[i].scene;
        }
    }
    return NULL;
}

nugscn_s *LevelEditor::GetScene(i32 index) {
    if (static_cast<u32>(index) < 10) {
        return scenes[index].scene;
    }
    return NULL;
}

void SerialiseNuHSpecial(EdStream &stream, void *data, i32) {
    nuhspecial_s *special = static_cast<nuhspecial_s *>(data);
    if (stream.mode == 2) {
        char *name = NuSpecialGetName(special);
        stream.SerialiseString(name, 0);
    }

    if (stream.mode == 1) {
        char name[128];
        stream.SerialiseString(name, 128);
        NuSpecialFind(NULL, special, name, 0);
        theLevelEditor.GetScene(0);
        for (i32 index = 0; index < theLevelEditor.reset_pending; ++index) {
            if (NuSpecialFind(theLevelEditor.GetScene(index), special, name, 0))
                break;
        }
    }
}

LevelEditorScene *LevelEditor::GetEdScene(i32 index) {
    if (static_cast<u32>(index) >= 10) {
        return NULL;
    }
    return &scenes[index];
}

void ClassEditor::cbEdClassSelectClassMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (menu == NULL)
        return;
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 0);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 1);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 2);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 3);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 4);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 5);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 6);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 7);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 8);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 9);
    eduiMenuAddItem(menu, eduiItemSeparatorCreate(0, &EdLevelAttr));
    for (i32 index = 0; index < theRegistry.class_count; ++index) {
        EdClass *ed_class = &theRegistry.classes[index];
        if ((ed_class->flags & 0x20000000) == 0 && ed_class->interface != NULL)
            eduiMenuAddItem(menu,
                            eduiItemSelCreate(index, &EdLevelAttr, 0, 0, cbEdClassSelectObjectMenu, ed_class->name));
    }
    if (menu->first == NULL)
        eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect,
                                                const_cast<char *>("No Registered Classes")));
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
    eduiMenuSortItemsByTxt(menu);
}

void ClassEditor::cbEdClassViewMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    const i32 shortcuts[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    if (menu == NULL)
        return;
    eduiMenuAddItem(menu, eduiItemCheckCreate(0, &EdLevelAttr, 0, 0, cbEdClassSetPinned, const_cast<char *>("Pinned")));
    menu->flags &= ~4;
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 0);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 1);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 2);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 3);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 4);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 5);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 6);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 7);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 8);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 9);
    eduiMenuAddItem(menu, eduiItemSeparatorCreate(0, &EdLevelAttr));
    eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdClassSetView, const_cast<char *>("All")));
    eduiMenuAddItem(menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdClassSetView, const_cast<char *>("None")));
    eduiMenuAddItem(menu, eduiItemSelCreate(2, &EdLevelAttr, 0, 0, cbEdClassSetView, const_cast<char *>("Invert")));
    if (theRegistry.class_count < 1) {
        eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect,
                                                const_cast<char *>("No Registered Classes")));
    } else {
        char label[128];
        for (i32 index = 0; index < theRegistry.class_count; ++index) {
            EdClass *ed_class = &theRegistry.classes[index];
            if ((ed_class->flags & 0x20000000) != 0)
                continue;
            if (index < 10)
                sprintf(label, "%d %s", shortcuts[index], ed_class->name);
            else
                sprintf(label, "   %s", ed_class->name);
            eduiMenuAddItem(menu, eduiItemCheckCreate(index + 3, &EdLevelAttr,
                                                      (theClassEditor.class_filter >> (index & 31)) & 1, 0,
                                                      cbEdClassSetView, label));
        }
    }
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
}

void ClassEditor::cbEdFilterLED(eduimenu_s *, eduiitem_s *item, u32) {
    if (item == NULL)
        return;
    LevelEditorScene *scene = theLevelEditor.GetEdScene(static_cast<u32>(item->data) >> 6);
    if (scene == NULL)
        return;
    theLevelEditor.current_led_file = -1;
    i32 editable = scene->editable + 1;
    scene->editable = editable;
    item->highlighted = editable;
}

void ClassEditor::cbEdClassFileMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (menu == NULL)
        return;
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 0);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 1);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 2);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 3);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 4);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 5);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 6);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 7);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 8);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 9);
    eduiMenuAddItem(menu, eduiItemSeparatorCreate(0, &EdLevelAttr));
    eduiMenuAddItem(menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdLevelSave, const_cast<char *>("Save")));
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
}

void ClassEditor::cbEdLevelDeselectAll(eduimenu_s *menu, eduiitem_s *, u32) {
    // The original callback walks the first explicit argument as an item list.
    eduiitem_s *item = reinterpret_cast<eduiitem_s *>(menu);
#define EDLEVEL_DESELECT_SCENE(index)                                                                                  \
    do {                                                                                                               \
        LevelEditorScene *scene = theLevelEditor.GetEdScene(index);                                                    \
        if (scene != NULL && scene->active) {                                                                          \
            item = item->previous;                                                                                     \
            scene->editable = 0;                                                                                       \
            item->highlighted = 0;                                                                                     \
        }                                                                                                              \
    } while (0)
    EDLEVEL_DESELECT_SCENE(0);
    EDLEVEL_DESELECT_SCENE(1);
    EDLEVEL_DESELECT_SCENE(2);
    EDLEVEL_DESELECT_SCENE(3);
    EDLEVEL_DESELECT_SCENE(4);
    EDLEVEL_DESELECT_SCENE(5);
    EDLEVEL_DESELECT_SCENE(6);
    EDLEVEL_DESELECT_SCENE(7);
    EDLEVEL_DESELECT_SCENE(8);
    EDLEVEL_DESELECT_SCENE(9);
#undef EDLEVEL_DESELECT_SCENE
}

void ClassEditor::cbEdLevelSelectAll(eduimenu_s *menu, eduiitem_s *, u32) {
    eduiitem_s *item = reinterpret_cast<eduiitem_s *>(menu);
#define EDLEVEL_SELECT_SCENE(index)                                                                                    \
    do {                                                                                                               \
        LevelEditorScene *scene = theLevelEditor.GetEdScene(index);                                                    \
        if (scene != NULL && scene->active) {                                                                          \
            item = item->previous;                                                                                     \
            scene->editable = 1;                                                                                       \
            item->highlighted = 1;                                                                                     \
        }                                                                                                              \
    } while (0)
    EDLEVEL_SELECT_SCENE(0);
    EDLEVEL_SELECT_SCENE(1);
    EDLEVEL_SELECT_SCENE(2);
    EDLEVEL_SELECT_SCENE(3);
    EDLEVEL_SELECT_SCENE(4);
    EDLEVEL_SELECT_SCENE(5);
    EDLEVEL_SELECT_SCENE(6);
    EDLEVEL_SELECT_SCENE(7);
    EDLEVEL_SELECT_SCENE(8);
    EDLEVEL_SELECT_SCENE(9);
#undef EDLEVEL_SELECT_SCENE
}

i32 LevelEditor::IsActiveScene(nugscn_s *scene) {
    for (i32 i = 0; i < reset_pending; ++i) {
        if (scenes[i].scene == scene) {
            return scenes[i].active;
        }
    }
    return 0;
}

i32 LevelEditor::IsEditable(i32 index) {
    if (index == -1) {
        return 1;
    }
    if (static_cast<u32>(index) >= 10) {
        return 0;
    }
    return scenes[index].editable != 0 || scenes[index].active == 0;
}

void LevelEditor::ClearLevel(i32 index) {
    scenes[index].active = 0;
    scenes[index].scene = NULL;
    for (BaseEditor *editor = first_editor; editor != NULL; editor = editor->next) {
        editor->ClearLevel(index);
    }
}

void LevelEditor::Flush() {
    for (BaseEditor *editor = first_editor; editor != NULL; editor = editor->next) {
        editor->Flush();
    }
    reset_pending = 0;
    memset(scenes, 0, sizeof(scenes));
}

void cbEdLevelSetSliderInt(eduimenu_s *, eduiitem_s *item, u32) {
    *static_cast<i32 *>(item->data_ptr) = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}

void cbEdLevelSetText(eduimenu_s *, eduiitem_s *item, u32) {
    strcpy(static_cast<char *>(item->data_ptr), static_cast<edui_textpicker_s *>(item)->value);
}

void LevelEditor::CreateEditorList(eduimenu_s *parent, eduiitem_s *item) {
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy,
                                      const_cast<char *>("Editor List"));
    if (menu == NULL)
        return;
    for (BaseEditor *editor = first_editor; editor != NULL; editor = editor->next) {
        eduiMenuAddItem(menu, eduiItemSelCreate(reinterpret_cast<usize>(editor), &EdLevelAttr, 0, 0,
                                                cbEdLevelEditorSelect, editor->GetName()));
    }
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
}

static void cbEdLevelEditorList(eduimenu_s *menu, eduiitem_s *item, u32) {
    theLevelEditor.CreateEditorList(menu, item);
}

void LevelEditor::CreateMenu() {
    f32 x[4] __attribute__((aligned(16)));
    f32 y[4] __attribute__((aligned(16)));
    eduiGetCursorCoords(x, y);
    x[0] *= 640.0f;
    y[0] *= 448.0f;
    eduimenu_s *menu;
    if (NuKeyboard(0x0f) != 0) {
        menu = eduiMenuCreate(static_cast<i32>(x[0]), static_cast<i32>(y[0]), 100, 200,
                              reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy,
                              const_cast<char *>("Level Editor"));
        if (menu == NULL) {
            edLevelActiveMenu = NULL;
            return;
        }
        eduiMenuAddItem(
            menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdLevelEditorList, const_cast<char *>("Editors...")));
        eduiMenuAddItem(
            menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdLevelSettingsMenu, const_cast<char *>("Options...")));
    } else {
        menu = eduiMenuCreate(static_cast<i32>(x[0]), static_cast<i32>(y[0]), 100, 200,
                              reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
        if (menu == NULL) {
            edLevelActiveMenu = NULL;
            return;
        }
        if (active_editor != NULL) {
            active_editor->AddMenuItems(menu);
        }
    }
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    edLevelActiveMenu = menu;
}

void LevelEditor::CloseMenu() {
    eduiSetCameraEnabled(1);
    if (edLevelActiveMenu != NULL) {
        if (edLevelActiveMenu->parent != NULL) {
            edLevelActiveMenu->parent->child = NULL;
        }
        if (edLevelActiveMenu->child != NULL) {
            edLevelActiveMenu->child->parent = NULL;
        }
    }
    edLevelDestroyActiveMenu = 1;
}

void LevelEditor::SetNextMenu(eduimenu_s *menu) {
    edLevelNextMenu = menu;
}

void ClassEditor::SelectLED(i32 class_id) {
    // The original keeps the cursor coordinates in 16-byte aligned slots.
    f32 x __attribute__((aligned(16)));
    f32 y __attribute__((aligned(16)));
    eduiGetCursorCoords(&x, &y);
    class_editor_create_pending = class_id;
    eduimenu_s *led_menu = eduiMenuCreate(static_cast<i32>(x * 640.0f), static_cast<i32>(y * 448.0f), 300, 250,
                                          reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy,
                                          const_cast<char *>("Select LED File"));
    if (led_menu == NULL)
        return;
    for (i32 index = 0; index < theLevelEditor.reset_pending; ++index) {
        LevelEditorScene *scene = theLevelEditor.GetEdScene(index);
        if (scene != NULL && scene->editable)
            eduiMenuAddItem(led_menu, eduiItemSelCreate(index, &EdLevelAttr, 0, 0, cbFileSelected, scene->name));
    }
    eduiMenuFitWidth(led_menu, 5);
    eduiMenuFitOnScreen(led_menu, 1);
    theLevelEditor.SetNextMenu(led_menu);
}

void ClassEditor::DestroySelectedObjects() {
    char title[136];
    NuStrCpy(title, "Destroy Selected Objects");
    f32 x, y;
    eduiGetCursorCoords(&x, &y);
    eduimenu_s *confirm_menu =
        eduiMenuCreate(static_cast<i32>(x * 640.0f), static_cast<i32>(y * 448.0f), 300, 50,
                       reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbDestroyMenu, title);
    if (__builtin_expect(confirm_menu == NULL, 0))
        return;
    eduiMenuAddItem(confirm_menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbDestroyObject, const_cast<char *>("Yes")));
    eduiMenuAddItem(confirm_menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbDestroyObject, const_cast<char *>("No")));
    eduiMenuFitWidth(confirm_menu, 5);
    eduiMenuFitOnScreen(confirm_menu, 1);
    theLevelEditor.SetNextMenu(confirm_menu);
}

void ClassEditor::cbEdClassDeleteObject(eduimenu_s *, eduiitem_s *, u32) {
    theClassEditor.DestroySelectedObjects();
}

char *LevelEditor::AddText(char *text) {
    i32 size = NuStrLen(text) + 1;
    char *result = text_buffer + text_length;
    NuStrCpy(result, text);
    text_length += size;
    return result;
}

void LevelEditor::AddInfoText(char *text) {
    for (i32 i = 0; i < 32; ++i) {
        if (info_text[i] == NULL) {
            info_text[i] = AddText(text);
            return;
        }
    }
}

void LevelEditor::DrawInfoText(char **lines, i32 count, i32 x, i32 y, i32 available_width, i32, i32 text_colour,
                               i32 background) {
    NuQFntPushPrintMode(2);
    NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
    NuQFntSet(system_qfont);
    const i32 line_height = static_cast<i32>(NuQFntHeight(system_qfont) * 0.15625f);
    const f32 font_height = NuQFntHeight(system_qfont);
    const f32 baseline = NuQFntBaseline(system_qfont);
    i32 width = 0;
    i32 height = 0;
    for (i32 index = 0; index < count; ++index) {
        if (lines[index] != NULL) {
            const i32 line_width = static_cast<i32>(NuQFntPrintLenU(system_qfont, lines[index])) >> 4;
            if (line_width > width) {
                width = line_width;
            }
            height += line_height;
        }
    }
    NuQFntPopCoordinateSystem();
    NuQFntPopPrintMode();
    if (height < 1) {
        return;
    }
    if (x < 0) {
        x = available_width - width - 5;
    }
    if (y < 0) {
        y = 5;
    }
    NuRndrRect2di(x << 4, y << 3, width << 4, height << 3, background, edLevel2dMtl);
    NuQFntPushPrintMode(2);
    NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
    NuQFntSet(system_qfont);
    NuQFntSetColour(system_qfont, text_colour);
    for (i32 index = 0; index < count; ++index) {
        if (lines[index] != NULL) {
            NuQFntPrintEx(system_qfont, x << 4, static_cast<i32>(0.125f * font_height + baseline) + y * 8, 0x10,
                          lines[index]);
            y += line_height;
        }
    }
    NuQFntPopCoordinateSystem();
    NuQFntPopPrintMode();
}

void LevelEditor::Display(ThingRenderData *) {
    f32 cursor_x __attribute__((aligned(16))) = 0.1f;
    f32 cursor_y __attribute__((aligned(16))) = 0.9f;
    if (editors_entered) {

        if (theLevelEditor.current_led_file != 0xff) {
            LevelEditorScene *scene = theLevelEditor.GetEdScene(theLevelEditor.current_led_file);
            if (scene != NULL) {
                char *name = scene->name;
                DrawInfoText(&name, 1, static_cast<i32>(640.0f * cursor_x), static_cast<i32>(448.0f * cursor_y),
                             info_width, info_height, info_colour, info_background);
            }
        }
        i32 draw_camera;
        // The original keeps the no-cursor path in the main render sequence.
        if (__builtin_expect(edmainGetCursorEnabled() == 0, 1)) {
            draw_camera = field_0x28;
        } else {
            eduiFlushInteracts();
            draw_camera = field_0x28;
        }
        if (draw_camera != 0) {
            NUCAMERA *camera = edmainGetCamera();
            const f32 distance = edcamGetDist();
            const f32 far_clip = distance + distance;
            if (global_camera.far_clip <= far_clip && global_camera.far_clip != far_clip) {
                camera->far_clip = far_clip;
            }
            edcamSet();
            EdDrawBegin(0);
            EdDrawLineCross(*reinterpret_cast<VuVec *>(background_colour), 1.0f, 0xff000000);
            EdDrawEnd();
        }
        if (active_editor != NULL) {
            active_editor->Render();
        }
        NuRndrLine3dDbgFlush();
        eduiGetCursorCoords(&cursor_x, &cursor_y);
        DrawInfoText(info_text, 32, static_cast<i32>((1.0f + cursor_x) * 640.0f),
                     static_cast<i32>((cursor_y + 1.0f) * 448.0f), info_width, info_height, info_colour,
                     info_background);
        if (edLevelActiveMenu != NULL) {
            NuFntSet(EdLevelFnt);
            NuFntScale(EdLevelFntScale, EdLevelFntScale);
            eduiMenuRender(edLevelActiveMenu);
        }
        if (edLevelPinnedMenu != NULL) {
            NuFntSet(EdLevelFnt);
            NuFntScale(EdLevelFntScale, EdLevelFntScale);
            eduiMenuRender(edLevelPinnedMenu);
        }
        eduiSetCursorColour(static_cast<u32>(field_0x2c));
        if (edmainGetCursorEnabled()) {
            eduiRenderCursor();
        }
    }
}

void LevelEditor::Enter() {
    edLevelActiveMenu = NULL;
    eduiSetUsingMenuFocus(1);
    editors_entered = 1;
    active = 1;
    for (BaseEditor *editor = first_editor; editor != NULL; editor = editor->next) {
        editor->Enter();
    }
}

void LevelEditor::Exit() {
    active = 0;
    eduiSetUsingMenuFocus(0);
    if (editors_entered != 0) {
        for (BaseEditor *editor = first_editor; editor != NULL; editor = editor->next) {
            editor->Exit();
        }
        editors_entered = 0;
    }
}

void EdManipulator::GetAxisLocators(VuVec &origin, VuVec *points, VuMtx *matrix) {
    const f32 scale = Scale + 0.0f;
    const f32 half = (scale + 0.0f) * 0.5f;
    points[3].x = 0.0f;
    points[3].y = 0.0f;
    points[3].w = 1.0f;
    points[2].x = 0.0f;
    points[2].z = 0.0f;
    points[2].w = 1.0f;
    points[1].y = 0.0f;
    points[1].z = 0.0f;
    points[1].w = 1.0f;
    points[7].x = 0.0f;
    points[7].y = 0.0f;
    points[7].z = 0.0f;
    points[7].w = 1.0f;
    points[0].x = 0.0f;
    points[0].y = 0.0f;
    points[0].z = 0.0f;
    points[0].w = 1.0f;
    points[1].x = scale;
    points[2].y = scale;
    points[3].z = scale;
    points[4].x = half;
    points[4].y = half;
    points[4].z = 0.0f;
    points[4].w = 0.0f;
    points[5].x = half;
    points[5].y = 0.0f;
    points[5].z = half;
    points[5].w = 0.0f;
    points[6].x = 0.0f;
    points[6].y = half;
    points[6].z = half;
    points[6].w = 0.0f;
    if (matrix != NULL) {
        for (i32 index = 0; index < 8; ++index) {
            VuVec &point = points[index];
            // The original reads the matrix rows as vectors, so they are reloaded after each locator store.
            const VuVec *rows = reinterpret_cast<const VuVec *>(&matrix->matrix);
            const f32 x = point.x;
            const f32 y = point.y;
            const f32 z = point.z;
            const f32 transformed_x = x * rows[0].x + y * rows[1].x + z * rows[2].x;
            const f32 transformed_y = x * rows[0].y + y * rows[1].y + z * rows[2].y;
            const f32 transformed_z = x * rows[0].z + y * rows[1].z + z * rows[2].z;
            point.x = transformed_x;
            point.y = transformed_y;
            point.z = transformed_z;
        }
    }
    for (i32 index = 0; index < 8; ++index) {
        points[index].x += origin.x;
        points[index].y += origin.y;
        points[index].z += origin.z;
    }
}

void EdManipulator::DrawAxis(VuVec &origin, VuMtx *matrix) {
    // The original keeps the locator array in a 16-byte aligned slot of a realigned frame.
    VuVec points[8] __attribute__((aligned(16)));
    GetAxisLocators(origin, points, matrix);
    EdDrawBegin(1);
    EdDrawLineSphere(points[1], Scale * 0.25f, 1.0f,
                     *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0 ? AxisColour[1]
                                                                                     : static_cast<i32>(0xff808080));
    EdDrawLineSphere(points[2], Scale * 0.25f, 1.0f,
                     *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0 ? AxisColour[2]
                                                                                     : static_cast<i32>(0xff808080));
    EdDrawLineSphere(points[3], Scale * 0.25f, 1.0f,
                     *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0 ? AxisColour[3]
                                                                                     : static_cast<i32>(0xff808080));
    VuMtx box;
    NuMtxSetIdentity(&box.matrix);
    box.matrix.m30 = points[4].x;
    box.matrix.m31 = points[4].y;
    box.matrix.m32 = points[4].z;
    box.matrix.m33 = 1.0f;
    EdDrawLineCube(box, Scale * 0.1f,
                   *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0 ? AxisColour[4]
                                                                                   : static_cast<i32>(0xff808080));
    box.matrix.m30 = points[5].x;
    box.matrix.m31 = points[5].y;
    box.matrix.m32 = points[5].z;
    box.matrix.m33 = 1.0f;
    EdDrawLineCube(box, Scale * 0.1f,
                   *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0 ? AxisColour[5]
                                                                                   : static_cast<i32>(0xff808080));
    box.matrix.m30 = points[6].x;
    box.matrix.m31 = points[6].y;
    box.matrix.m32 = points[6].z;
    box.matrix.m33 = 1.0f;
    EdDrawLineCube(box, Scale * 0.1f,
                   *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0 ? AxisColour[6]
                                                                                   : static_cast<i32>(0xff808080));
    box.matrix.m30 = points[7].x;
    box.matrix.m31 = points[7].y;
    box.matrix.m32 = points[7].z;
    box.matrix.m33 = 1.0f;
    EdDrawLineCube(box, Scale * 0.1f,
                   *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0 ? AxisColour[7]
                                                                                   : static_cast<i32>(0xff808080));
    EdDrawEnd();
    EdDrawBegin(0);
    const float arrow_size = Scale * 0.25f;
    float arrow_radius;
    VuVec end;
    VuVec start;
    {
        const VuVec &point = points[1];
        const VuVec &center = points[7];
        const VuVec offset((point.x - center.x) * arrow_size * 0.5f, (point.y - center.y) * arrow_size * 0.5f,
                           (point.z - center.z) * arrow_size * 0.5f, 0.0f);
        start = VuVec(point.x - offset.x, point.y - offset.y, point.z - offset.z, 0.0f);
        end = VuVec(offset.x + point.x, offset.y + point.y, offset.z + point.z, 0.0f);
        arrow_radius = arrow_size * 0.2f;
        const i32 colour = *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0
                               ? AxisColour[1]
                               : static_cast<i32>(0xff808080);
        EdDrawPolyArrow(start, end, 8, colour, arrow_radius, arrow_radius, 0.5f, 0.0f);
    }
    {
        const VuVec &point = points[2];
        const VuVec &center = points[7];
        const VuVec offset((point.x - center.x) * arrow_size * 0.5f, (point.y - center.y) * arrow_size * 0.5f,
                           (point.z - center.z) * arrow_size * 0.5f, 0.0f);
        start = VuVec(point.x - offset.x, point.y - offset.y, point.z - offset.z, 0.0f);
        end = VuVec(offset.x + point.x, offset.y + point.y, offset.z + point.z, 0.0f);
        const i32 colour = *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0
                               ? AxisColour[2]
                               : static_cast<i32>(0xff808080);
        EdDrawPolyArrow(start, end, 8, colour, arrow_radius, arrow_radius, 0.2f, 0.0f);
    }
    {
        const VuVec &point = points[3];
        const VuVec &center = points[7];
        const VuVec offset((point.x - center.x) * arrow_size * 0.5f, (point.y - center.y) * arrow_size * 0.5f,
                           (point.z - center.z) * arrow_size * 0.5f, 0.0f);
        start = VuVec(point.x - offset.x, point.y - offset.y, point.z - offset.z, 0.0f);
        end = VuVec(offset.x + point.x, offset.y + point.y, offset.z + point.z, 0.0f);
        const i32 colour = *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0
                               ? AxisColour[3]
                               : static_cast<i32>(0xff808080);
        EdDrawPolyArrow(start, end, 8, colour, arrow_radius, arrow_radius, 0.2f, 0.0f);
    }
    EdDrawEnd();
}

void EdManipulator::DrawRotator(VuVec &origin) {
    f32 scale = Scale;
    EdDrawBegin(1);
    EdDrawLineCircleX(origin, scale,
                      *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0 ? AxisColour[1]
                                                                                      : static_cast<i32>(0xff808080),
                      32);
    EdDrawLineCircleY(origin, scale,
                      *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0 ? AxisColour[2]
                                                                                      : static_cast<i32>(0xff808080),
                      32);
    EdDrawLineCircleZ(origin, scale,
                      *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0 ? AxisColour[3]
                                                                                      : static_cast<i32>(0xff808080),
                      32);
    EdDrawEnd();
    const u8 *data = reinterpret_cast<const u8 *>(this);
    const i32 *selected_axis = reinterpret_cast<const i32 *>(data + 0x0c);
    const i32 *start_angle = reinterpret_cast<const i32 *>(data + 0x60);
    const i32 *end_angle = reinterpret_cast<const i32 *>(data + 0x64);
    if (*start_angle != *end_angle && *selected_axis != 0) {
        EdDrawBegin(1);
        EdDrawPolySector(origin, scale, *selected_axis - 1, *start_angle, *end_angle, static_cast<i32>(0x80808080), 32);
        EdDrawEnd();
    }
}

EdManMove::EdManMove() {
    selected_attribute = 8;
}

EdManRotate::EdManRotate() {
    selected_attribute = 16;
}

EdManScale::EdManScale() {
    selected_attribute = 32;
}

EdString::~EdString() {
    if (data != NULL) {
        theMemoryManager.FreePool(data, data[0]);
    }
}

void EdString::Set(char const *value) {
    if (value == NULL) {
        if (data != NULL) {
            theMemoryManager.FreePool(data, data[0]);
            data = NULL;
        }
        return;
    }

    i32 length = NuStrLen(value);
    if (data != NULL && length + 2 > data[0]) {
        theMemoryManager.FreePool(data, data[0]);
        data = NULL;
    }
    if (data == NULL) {
        data = static_cast<char *>(theMemoryManager.AllocPool(length + 2, 1));
        data[0] = ((length + 1) / 32) * 32 + 32;
    }
    NuStrNCpy(data + 1, value, 250);
}

Placeable *PlaceableInterface::Find(char *name) {
    EdClassInterface *interface = reinterpret_cast<EdClassInterface *>(this);
    for (Placeable *object = static_cast<Placeable *>(interface->vtable->get_next_object(interface, NULL));
         object != NULL; object = static_cast<Placeable *>(interface->vtable->get_next_object(interface, object))) {
        if (NuStrICmp(name, object->GetName()) == 0)
            return object;
    }
    return NULL;
}

i32 PlaceableInterface::Find(char *name, Placeable **objects, i32 capacity) {
    EdClassInterface *interface = reinterpret_cast<EdClassInterface *>(this);
    i32 count = 0;
    Placeable *object = static_cast<Placeable *>(interface->vtable->get_next_object(interface, NULL));
    while (object != NULL && count < capacity) {
        if (NuStrIStr(const_cast<char *>(object->GetName()), name) != NULL)
            objects[count++] = object;
        object = static_cast<Placeable *>(interface->vtable->get_next_object(interface, object));
    }
    return count;
}

i32 PlaceableInterface::DebugOutputObjects() {
    EdClassInterface *interface = reinterpret_cast<EdClassInterface *>(this);
    i32 count = 0;
    for (void *object = interface->vtable->get_next_object(interface, NULL); object != NULL;
         object = interface->vtable->get_next_object(interface, object))
        ++count;
    return count;
}

PlaceableHelper::PlaceableHelper() {
    object_type_count = 0;
}

void PlaceableHelper::RegisterObjectType(char *name, PlaceableInterface *interface) {
    i32 index = object_type_count++;
    object_types[index].name = name;
    object_types[index].interface = interface;
    *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(interface) + 8) = object_type_count;
}

Placeable *PlaceableHelper::FindObject(char *name) {
    for (i32 index = 0; index < object_type_count; ++index) {
        EdClassInterface *interface = reinterpret_cast<EdClassInterface *>(object_types[index].interface);
        for (Placeable *object = static_cast<Placeable *>(interface->vtable->get_next_object(interface, NULL));
             object != NULL; object = static_cast<Placeable *>(interface->vtable->get_next_object(interface, object))) {
            if (NuStrICmp(object->GetName(), name) == 0)
                return object;
        }
    }
    return NULL;
}

void *PlaceableHelper::GetNextObject(void *object) {
    if (object == NULL)
        iteration_object_type = 0;
    while (iteration_object_type < object_type_count) {
        EdClassInterface *interface =
            reinterpret_cast<EdClassInterface *>(object_types[iteration_object_type].interface);
        void *next = interface->vtable->get_next_object(interface, object);
        if (next != NULL)
            return next;
        ++iteration_object_type;
        object = NULL;
    }
    return NULL;
}

void PlaceableNameControl::cbButton(eduimenu_s *parent, eduiitem_s *item, u32) {
    edObjectNameControl = static_cast<EdStringControl *>(item->data_ptr);
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (menu == NULL)
        return;

    char value[128];
    edObjectNameControl->GetVal(value, sizeof(value));
    eduiMenuAddItem(menu, eduiItemSelCreate(0, item->colours, 0, 0, cbSelectObject, const_cast<char *>("None")));
    for (Placeable *object = static_cast<Placeable *>(thePlaceableHelper.GetNextObject(NULL)); object != NULL;
         object = static_cast<Placeable *>(thePlaceableHelper.GetNextObject(object))) {
        eduiMenuAddItem(menu, eduiItemSelCreate(reinterpret_cast<usize>(object), item->colours, 0, 0, cbSelectObject,
                                                const_cast<char *>(object->GetName())));
        if (NuStrICmp(value, object->GetName()) == 0)
            menu->selected = edui_last_item;
    }
    menu->flags |= 1;
    eduiMenuAttach(parent, menu);
    eduiMenuFitWidth(menu, 5);
    static_cast<edui_prop_s *>(item)->unknown_property_flags &= ~8;
}

void PlaceableNameControl::cbChanged(eduimenu_s *, eduiitem_s *item, u32) {
    EdStringControl *control = static_cast<EdStringControl *>(item->data_ptr);
    for (Placeable *object = static_cast<Placeable *>(thePlaceableHelper.GetNextObject(NULL)); object != NULL;
         object = static_cast<Placeable *>(thePlaceableHelper.GetNextObject(object))) {
        if (NuStrICmp(static_cast<edui_prop_s *>(item)->property_text, object->GetName()) == 0) {
            eduiItemPropSetText(static_cast<edui_prop_s *>(item), const_cast<char *>(object->GetName()));
            control->SetVal(object->GetName());
            return;
        }
    }
}

void ClassEditor::cbEdClassRemoveDuplicates(eduimenu_s *, eduiitem_s *, u32) {
    Placeable *objects[256];
    i32 count = 0;
    for (Placeable *object = static_cast<Placeable *>(thePlaceableHelper.GetNextObject(NULL));
         object != NULL && count < 256; object = static_cast<Placeable *>(thePlaceableHelper.GetNextObject(object))) {
        objects[count++] = object;
    }
    for (i32 first = 0; first < count; ++first) {
        Placeable *object = objects[first];
        if (object == NULL)
            continue;
        char const *name = object->GetName();
        VuVec const *position = object->GetInitialPosition();
        NUVEC first_position = {position->x, position->y, position->z};
        i32 object_type = 0;
        for (; object_type < thePlaceableHelper.object_type_count; ++object_type) {
            void *interface = thePlaceableHelper.object_types[object_type].interface;
            if (reinterpret_cast<i32 *>(interface)[2] == object->scene_id)
                break;
        }
        if (!(NuVecMag(&first_position) > 0.1f))
            continue;
        for (i32 second = first + 1; second < count; ++second) {
            Placeable *other = objects[second];
            if (other != NULL) {
                char const *other_name = other->GetName();
                VuVec const *other_position = other->GetInitialPosition();
                NUVEC second_position = {other_position->x, other_position->y, other_position->z};
                i32 other_type = 0;
                for (; other_type < thePlaceableHelper.object_type_count; ++other_type) {
                    void *interface = thePlaceableHelper.object_types[other_type].interface;
                    if (reinterpret_cast<i32 *>(interface)[2] == other->scene_id)
                        break;
                }
                if (!(NuVecMag(&first_position) > 0.1f))
                    continue;
                NUVEC separation = {first_position.x - second_position.x, first_position.y - second_position.y,
                                    first_position.z - second_position.z};
                if (NuVecMag(&separation) < 0.1f && object_type == other_type &&
                    (strstr(name, other_name) != NULL || strstr(other_name, name) != NULL)) {
                    EdClassInterface *interface =
                        reinterpret_cast<EdClassInterface *>(thePlaceableHelper.object_types[object_type].interface);
                    interface->vtable->destroy_object(interface, other, 0);
                    objects[second] = NULL;
                }
            }
        }
    }
}

void *PlaceableHelper::GetNextObject(void *object, i32 (*filter)(void *)) {
    for (object = GetNextObject(object); object != NULL; object = GetNextObject(object)) {
        if (filter(object) != 0)
            return object;
    }
    return NULL;
}

void EdSpecialObjectControl::cbButton(eduimenu_s *parent, eduiitem_s *item, u32) {
    EdSpecialObjectControl *control = static_cast<EdSpecialObjectControl *>(item->data_ptr);
    active_special_object_control = control;
    eduimenu_s *choices =
        eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                       reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (choices == NULL)
        return;
    nuhspecial_s selected;
    control->reference->GetMemberData(control->object, EdType_NuHSpecial, &selected, 0);
    eduiMenuAddItem(choices, eduiItemSelCreate(static_cast<usize>(-1), item->colours, 0, 0, cbSelectObject,
                                               const_cast<char *>("None")));
    for (Placeable *object = static_cast<Placeable *>(thePlaceableHelper.GetNextObject(NULL, SpecialObjectFilter));
         object != NULL;) {
        eduiMenuAddItem(choices, eduiItemSelCreate(reinterpret_cast<usize>(object), item->colours, 0, 0, cbSelectObject,
                                                   const_cast<char *>(object->GetName())));
        Placeable *next_object =
            static_cast<Placeable *>(thePlaceableHelper.GetNextObject(object, SpecialObjectFilter));
        if (NuSpecialCompare(&static_cast<SpecialObject *>(object)->special, &selected) != 0)
            choices->selected = edui_last_item;
        object = next_object;
    }
    eduiMenuSortItemsByTxt(choices);
    choices->flags |= 1;
    eduiMenuAttach(parent, choices);
    eduiMenuFitWidth(choices, 5);
    eduiMenuFitOnScreen(choices, 30);
    static_cast<edui_prop_s *>(item)->unknown_property_flags &= ~8;
}

void EdSpecialObjectControl::cbChanged(eduimenu_s *, eduiitem_s *item, u32) {
    EdSpecialObjectControl *control = static_cast<EdSpecialObjectControl *>(item->data_ptr);
    nuhspecial_s selected;
    NuSpecialClear(&selected);
    char *name = reinterpret_cast<edui_prop_s *>(item)->property_text;
    for (Placeable *object = static_cast<Placeable *>(thePlaceableHelper.GetNextObject(NULL, SpecialObjectFilter));
         object != NULL;
         object = static_cast<Placeable *>(thePlaceableHelper.GetNextObject(object, SpecialObjectFilter))) {
        if (NuStrICmp(name, const_cast<char *>(object->GetName())) == 0) {
            selected = *static_cast<SpecialObject *>(object)->GetNuHSpecial();
            break;
        }
    }
    if (NuSpecialExistsFn(&selected) != 0) {
        eduiItemPropSetText(reinterpret_cast<edui_prop_s *>(item), NuSpecialGetName(&selected));
        control->reference->SetMemberData(control->object, EdType_NuHSpecial, &selected, 0, NULL);
    }
}

Placeable *PlaceableHelper::Find(char *name) {
    for (i32 index = 0; index < object_type_count; ++index) {
        Placeable *object = object_types[index].interface->Find(name);
        if (object != NULL)
            return object;
    }
    return NULL;
}

i32 PlaceableHelper::Find(char *name, Placeable **objects, i32 capacity) {
    i32 count = 0;
    for (i32 index = 0; index < object_type_count; ++index)
        count += object_types[index].interface->Find(name, objects + count, capacity - count);
    return count;
}

i32 PlaceableHelper::IsEditorObject(ClassObject &object) {
    if (object.object != NULL) {
        for (i32 index = 0; index < object_type_count; ++index) {
            EdClassInterface *interface = reinterpret_cast<EdClassInterface *>(object_types[index].interface);
            if (interface->object_class == object.ed_class)
                return 1;
        }
    }
    return 0;
}

void PlaceableNameControl::Process(EdInputContext &input) {
    if (input.GetHold(0x16) != 0.0f) {
        ClassObject object = theClassEditor.current_object;
        if (thePlaceableHelper.IsEditorObject(object) == 0) {
            theClassEditor.field_3c = static_cast<i32>(0xff000080);
        } else {
            theClassEditor.field_3c = static_cast<i32>(0xff008000);
            if (input.GetPress(0x19) != 0.0f)
                SetVal(static_cast<Placeable *>(object.object)->GetName());
        }
    }
}

PlaceableNameControl::PlaceableNameControl() : reserved_10(0) {
}

void PlaceableNameControl::AddMenuItem(eduimenu_s *menu, EdRef *reference, void *object) {
    PlaceableNameControl *control =
        new (theMemoryManager.AllocPool(sizeof(PlaceableNameControl), 1)) PlaceableNameControl();
    if (control == NULL)
        return;
    control->reference = reference;
    control->object = object;
    char value[128];
    control->GetVal(value, sizeof(value));
    control->item = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected, cbChanged,
                                       cbButton, 1, reference->name, value);
    eduiMenuAddItem(menu, control->item);
}

void PropertyMenu::Destroy() {
    eduicbMenuCloseAllexpanders(menu);
    if (menu != NULL) {
        for (eduiitem_s *item = menu->first; item != NULL; item = item->next) {
            delete static_cast<EdControl *>(item->data_ptr);
        }
    }
    eduiMenuDestroy(menu);
}

bool PropertyMenu::ContainsObject(ClassObject &object) {
    for (i32 i = 0; i < object_count; ++i) {
        if (objects[i].object == object.object && objects[i].reference == object.reference) {
            return true;
        }
    }
    return false;
}

i32 PropertyMenu::ContainsObject(void *object) {
    for (i32 i = 0; i < object_count; ++i) {
        if (objects[i].object == object) {
            return 1;
        }
    }
    return 0;
}

void PropertyMenu::AddObject(ClassObject &object) {
    if (object_count < 8) {
        objects[object_count++] = object;
    }
}

void PropertyMenu::ClearObjecs() {
    object_count = 0;
}

void PropertyMenu::SelectAttr(i32 selected) {
    for (eduiitem_s *item = menu->first; item != NULL; item = item->next) {
        EdControl *control = static_cast<EdControl *>(item->data_ptr);
        if (control != NULL) {
            control->SetMenuItemAttr(selected, item, &thePropertyTool.selected_attr, &thePropertyTool.unselected_attr);
        }
    }
}

PropertyTool::PropertyTool() {
    active_menu = NULL;
    last_menu = NULL;
    menu_count = 0;
    show_type_names = 1;
    menu_attr.background = 0x80000000;
    menu_attr.text = 0x80ff0000;
    menu_attr.highlight = 0x80804040;
    menu_attr.disabled = 0x80404040;
    selected_attr.background = 0x80000000;
    selected_attr.text = 0x80ff0000;
    selected_attr.highlight = 0x80406080;
    selected_attr.disabled = 0x80404040;
    unselected_attr.background = 0x80000000;
    unselected_attr.text = 0x80ff0000;
    unselected_attr.highlight = 0x80707080;
    unselected_attr.disabled = 0x80808080;
}

void PropertyTool::SelectAttr(i32 selected) {
    for (PropertyMenu *menu = active_menu; menu != NULL; menu = menu->next) {
        menu->SelectAttr(selected);
    }
}

void ClassEditor::SetMode(i32 new_mode) {
    mode = new_mode;
    switch (new_mode) {
        case 3:
            manipulator = &theMoveManipulator;
            break;
        case 4:
            manipulator = &theRotateManipulator;
            break;
        case 5:
            manipulator = &theScaleManipulator;
            break;
        default:
            manipulator = &theDefaultManipulator;
            break;
    }
    thePropertyTool.SelectAttr(manipulator->selected_attribute);
}

void ClassEditor::cbEdPadSetManipulatorMode(eduimenu_s *, eduiitem_s *, u32) {
    if (theClassEditor.mode == 4) {
        theClassEditor.mode = 5;
    } else if (theClassEditor.mode == 5) {
        theClassEditor.mode = 3;
    } else if (theClassEditor.mode == 3) {
        theClassEditor.mode = 4;
    }
    theClassEditor.SetMode(theClassEditor.mode);
}

void ClassEditor::cbEdClassSetMode(eduimenu_s *menu, eduiitem_s *item, u32) {
    theClassEditor.SetMode(item->data);
    eduiMenuHighlight(menu, item);
}

i32 ClassEditor::SelectObject(ClassObject &object, i32 mode) {
    i32 selected = 1;
    if (mode == 1) {
        if (object.object == NULL) {
            selected = 0;
            goto update_property_selection;
        }
        for (ClassObjectListEntry *entry = selected_objects.first; entry != NULL; entry = entry->next) {
            if (entry->object == object.object && entry->reference == object.reference) {
                if (entry->next != NULL) {
                    entry->next->previous = entry->previous;
                } else {
                    selected_objects.last = entry->previous;
                }
                if (entry->previous != NULL) {
                    entry->previous->next = entry->next;
                } else {
                    selected_objects.first = entry->next;
                }
                entry->next = NULL;
                entry->previous = NULL;
                --selected_objects.count;
                theMemoryManager.FreePool(entry, sizeof(*entry));
                selected = 0;
                goto update_property_selection;
            }
        }
    } else if (mode == 2) {
        for (ClassObjectListEntry *entry = selected_objects.first; entry != NULL;) {
            ClassObjectListEntry *next = entry->next;
            if (entry->object == object.object) {
                if (entry->next != NULL) {
                    entry->next->previous = entry->previous;
                } else {
                    selected_objects.last = entry->previous;
                }
                if (entry->previous != NULL) {
                    entry->previous->next = entry->next;
                } else {
                    selected_objects.first = entry->next;
                }
                entry->next = NULL;
                entry->previous = NULL;
                --selected_objects.count;
                theMemoryManager.FreePool(entry, sizeof(*entry));
            }
            entry = next;
        }
    } else {
        while (selected_objects.first != NULL) {
            ClassObjectListEntry *entry = selected_objects.first;
            if (entry->next != NULL) {
                entry->next->previous = entry->previous;
            } else {
                selected_objects.last = entry->previous;
            }
            if (entry->previous != NULL) {
                entry->previous->next = entry->next;
            } else {
                selected_objects.first = entry->next;
            }
            entry->next = NULL;
            entry->previous = NULL;
            --selected_objects.count;
            theMemoryManager.FreePool(entry, sizeof(*entry));
        }
    }

    if (object.object != NULL) {
        ClassObjectListEntry *entry =
            new (theMemoryManager.AllocPool(sizeof(ClassObjectListEntry), 1)) ClassObjectListEntry;
        memset(&entry->ed_class, 0, sizeof(object));
        entry->next = NULL;
        memcpy(&entry->ed_class, &object, sizeof(object));
        entry->previous = selected_objects.last;
        if (selected_objects.last != NULL) {
            selected_objects.last->next = entry;
        }
        ClassObjectListEntry *first = selected_objects.first;
        selected_objects.last = entry;
        if (first == NULL) {
            selected_objects.first = entry;
        }
        ++selected_objects.count;
    }
update_property_selection:
    thePropertyTool.SelectAttr(manipulator->selected_attribute);
    return selected;
}

void KnotHelper::Process(void *object, EdInputContext &) {
    SplineKnot *knot = static_cast<SplineKnot *>(object);
    if (!theClassEditor.IsSelectedObject(knot->spline, NULL)) {
        ClassObject spline_object = {theSplineHelper.object_class, knot->spline, NULL};
        theClassEditor.SelectObject(spline_object, 1);
    }
}

// The original realigns the stack for its local ClassObject.
__attribute__((force_align_arg_pointer)) i32 EdControl::SelectSubObject() {
    ClassObject selection = {};
    ClassObjectListEntry *entry;
    for (entry = theClassEditor.selected_objects.first; entry; entry = entry->next) {
        if (entry->object == object)
            break;
    }
    if (entry == NULL)
        return 1;
    selection.ed_class = entry->ed_class;
    selection.object = entry->object;
    selection.reference = entry->reference;
    if (selection.object != NULL) {
        i32 mode = static_cast<i32>(Input->GetHold(16)) != 0 ? 1 : 2;
        selection.reference = reference;
        theClassEditor.SelectObject(selection, mode);
        if (!theClassEditor.selected_objects.IsInList(selection.object, NULL))
            theClassEditor.SelectObject(selection, 1);
    }
    return 1;
}

void PropertyTool::SetMenuControl(eduimenu_s *menu, EdControl *control) {
    for (PropertyMenu *property_menu = active_menu; property_menu != NULL; property_menu = property_menu->next) {
        if (property_menu->menu == menu) {
            property_menu->control = control;
            return;
        }
    }
}

void EdMatrixControl::cbSelected(eduimenu_s *menu, eduiitem_s *item, u32 value) {
    EdMatrixControl *control = static_cast<EdMatrixControl *>(item->data_ptr);
    control->SelectSubObject();
    if (item == control->item || item == control->components[0] || item == control->components[1] ||
        item == control->components[2]) {
        theClassEditor.SetMode(3);
    }
    if (item == control->components[3] || item == control->components[4] || item == control->components[5]) {
        theClassEditor.SetMode(4);
    }
    if (item == control->components[6] || item == control->components[7] || item == control->components[8]) {
        theClassEditor.SetMode(5);
    }
    thePropertyTool.SetMenuControl(menu, control);
}

void EdVectorControl::cbSelected(eduimenu_s *menu, eduiitem_s *item, u32 value) {
    EdVectorControl *control = static_cast<EdVectorControl *>(item->data_ptr);
    control->item = item;
    if (control->SelectSubObject() != 0)
        theClassEditor.SetMode(3);
    else
        theClassEditor.SetMode(0);
    thePropertyTool.SetMenuControl(menu, control);
}

void EdControl::cbSelected(eduimenu_s *menu, eduiitem_s *menu_item, u32) {
    EdControl *control = static_cast<EdControl *>(menu_item->data_ptr);
    i32 selected = control->SelectSubObject();
    theClassEditor.SetMode(0);
    if (selected != 0)
        memcpy(menu_item->colours, &thePropertyTool.unselected_attr, sizeof(eduiiattr_s));
    thePropertyTool.SetMenuControl(menu, control);
}

PropertyMenu *PropertyTool::FindItemMenu(PropertyMenu *menu, ClassItem *item) {
    for (; menu != NULL; menu = menu->next) {
        if (menu->ContainsObject(item->object)) {
            return menu;
        }
    }
    return NULL;
}

void PropertyTool::RefreshMenuControls(PropertyMenu *property_menu) {
    eduiitem_s *item = property_menu->menu->field_0c;
    NuFntSet(EdLevelFnt);
    NuFntScale(EdLevelFntScale, EdLevelFntScale);
    for (; item != NULL; item = item->next) {
        EdControl *control = static_cast<EdControl *>(item->data_ptr);
        if (control != NULL) {
            control->Refresh();
        }
        if (item == property_menu->menu->field_10) {
            break;
        }
    }
}

void PropertyTool::BringToFront(PropertyMenu *menu) {
    if (menu->next != NULL) {
        menu->next->previous = menu->previous;
    } else {
        last_menu = menu->previous;
    }
    if (menu->previous != NULL) {
        menu->previous->next = menu->next;
    } else {
        active_menu = menu->next;
    }
    menu->previous = NULL;
    menu->next = NULL;
    --menu_count;
    menu->order = -2;
    PropertyMenu *position;
    for (position = active_menu; position != NULL; position = position->next) {
        if (position->order >= -1)
            break;
    }
    if (position == NULL) {
        menu->previous = last_menu;
        if (last_menu != NULL) {
            last_menu->next = menu;
        }
        last_menu = menu;
        if (active_menu == NULL) {
            active_menu = menu;
        }
    } else {
        menu->previous = position->previous;
        menu->next = position;
        if (position->previous != NULL) {
            position->previous->next = menu;
        } else {
            active_menu = menu;
        }
        position->previous = menu;
    }
    ++menu_count;
}

PropertyMenu *PropertyTool::GetActiveMenu(PropertyMenu *menu) {
    for (; menu != NULL; menu = menu->next) {
        if (menu->menu == eduiGetActiveMenuParent()) {
            return menu;
        }
    }
    return NULL;
}

PropertyMenu *PropertyTool::GetNextActiveMenu() {
    PropertyMenu *menu = GetActiveMenu(active_menu);
    return menu != NULL ? menu->next : NULL;
}

eduimenu_s *PropertyTool::GetNextDefaultActiveMenu(eduimenu_s *menu) {
    if (menu == NULL) {
        return edLevelActiveMenu;
    }
    return menu == edLevelActiveMenu ? edLevelPinnedMenu : NULL;
}

eduimenu_s *GetMenuActiveChild(eduimenu_s *menu) {
    if (menu == NULL) {
        return NULL;
    }
    if (menu->child != NULL)
        return GetMenuActiveChild(menu->child);
    return menu;
}

void PropertyTool::SetDefaultActiveMenu(PropertyMenu *menu) {
    if (menu != NULL && menu->menu != NULL) {
        eduiSetActiveMenu(GetMenuActiveChild(menu->menu));
    } else {
        eduiSetActiveMenu(edLevelActiveMenu != NULL ? edLevelActiveMenu : edLevelPinnedMenu);
    }
}

void PropertyTool::ToggleActiveMenu() {
    PropertyMenu *menu = GetActiveMenu(active_menu);
    if (menu == NULL) {
        if (GetNextDefaultActiveMenu(eduiGetActiveMenu()) == NULL) {
            SetDefaultActiveMenu(active_menu);
        }
    } else {
        if (menu == last_menu) {
            eduiSetActiveMenu(NULL);
        } else {
            SetDefaultActiveMenu(GetNextActiveMenu());
        }
    }
}

i32 PropertyTool::ProcessControls(EdInputContext &input) {
    for (PropertyMenu *menu = active_menu; menu != NULL; menu = menu->next) {
        if (menu->control != NULL) {
            menu->control->Process(input);
        }
    }
    return 0;
}

PropertyMenuMetrics PropertyTool::ediGetMenuStartMetrics() {
    return menu_startmetrics;
}

void PropertyTool::ediMenuStoreMetrics(eduimenu_s *menu) {
    const i32 x = menu->x;
    const i32 y = menu->y;
    menu_startmetrics.x = x > 0 ? x : 20;
    menu_startmetrics.y = y > 0 ? y : 5;
    menu_startmetrics.width = menu->width;
    menu_startmetrics.height = menu->height;
}

void PropertyTool::ediMenuRetrieveMetrics(eduimenu_s *menu) {
#if defined(__i386__) && defined(__SSE2__)
    __m128i metrics = _mm_load_si128(reinterpret_cast<const __m128i *>(&menu_startmetrics));
    _mm_storeu_si128(reinterpret_cast<__m128i *>(&menu->x), metrics);
#else
    memcpy(&menu->x, &menu_startmetrics, sizeof(menu_startmetrics));
#endif
}

void PropertyTool::AutoLocateMenu(PropertyMenu *property_menu) {
    eduimenu_s *menu = property_menu->menu;
    menu->x = 70;
    menu->y = 30;
    bool use_stored_metrics = true;
    PropertyMenu *other = active_menu;
    while (other != NULL) {
        if (__builtin_expect(other->menu != menu, 1)) {
            if (menu->x + menu->width >= other->menu->x && other->menu->x + other->menu->width >= menu->x) {
                menu->x += 10;
                if (static_cast<float>(menu->x + menu->width) > 590.0f) {
                    menu->x = 20;
                    menu->y = 5;
                    return;
                }
                use_stored_metrics = false;
                other = active_menu;
                continue;
            }
            use_stored_metrics = false;
        }
        other = other->next;
    }
    if (__builtin_expect(!!(use_stored_metrics), 1)) {
        ediMenuRetrieveMetrics(menu);
    }
    if (static_cast<float>(menu->x + menu->width) > 590.0f) {
        menu->y = 5;
        menu->x = 20;
    }
}

bool PropertyTool::HasActiveMenu() {
    return active_menu != NULL;
}

void SerialiseStringAddr(EdStream &stream, void *data, i32 count) {
    stream.SerialiseString(static_cast<char **>(data), count);
}

void EdRef::Serialise(EdStream &stream, i32 *class_mapping) {
    if (stream.version != 0) {
        if (attributes < 0 && class_mapping != NULL) {
            stream.SerialiseBuffer(&class_mapping[type_id], sizeof(i32), 1);
        } else {
            stream.SerialiseBuffer(&type_id, sizeof(i32), 1);
        }
        stream.SerialiseString(&name);
        stream.SerialiseBuffer(&member_offset, sizeof(i32), 1);
        stream.SerialiseBuffer(&size, sizeof(i32), 1);
    } else {
        stream.SerialiseBuffer(&type_id, sizeof(i32), 1);
        stream.SerialiseString(&name);
        stream.SerialiseBuffer(&member_offset, sizeof(i32), 1);
    }
    stream.SerialiseBuffer(&attributes, sizeof(i32), 1);
}

i32 EdRef::GetAttributeData(void *object, i32 attribute, i32 requested_type, void *data, i32 data_size) {
    if (__builtin_expect((attributes & attribute) != 0, 0)) {
        if ((attribute & 8) && type_id == EdType_VuMtx) {
            NUMTX_ALIGNED16 matrix;
            GetMemberData(object, type_id, &matrix, 0);
            u32 *out = static_cast<u32 *>(data);
            const u32 *row = reinterpret_cast<const u32 *>(&matrix.m30);
            out[0] = row[0];
            out[1] = row[1];
            out[2] = row[2];
            out[3] = row[3];
            return 1;
        }
        if (attribute & 0x100) {
            i16 value;
            GetMemberData(object, EdType_Short, &value, 0);
            *static_cast<i16 *>(data) = value;
            return 1;
        }
        GetMemberData(object, requested_type, data, data_size);
        return 1;
    }
    return 0;
}

i32 EdRef::SetAttributeData(void *object, i32 attribute, i32 requested_type, void *data, i32 data_size) {
    if (!(attributes & attribute)) {
        return 0;
    }
    if ((attribute & 8) && type_id == EdType_VuMtx) {
        NUMTX_ALIGNED16 matrix;
        GetMemberData(object, type_id, &matrix, 0);
        memmove(&matrix.m30, data, sizeof(VuVec));
        SetMemberData(object, EdType_VuMtx, &matrix, 0, NULL);
        return 1;
    }
    if (attribute & 0x100) {
        SetMemberData(object, EdType_Short, &data, sizeof(i16), NULL);
        return 0;
    }
    SetMemberData(object, requested_type, data, data_size, NULL);
    return 1;
}

void EdRegistry::Initialise(variptr_u &buffer, variptr_u &, i32 max_classes, i32 max_types, i32 max_mappings,
                            i32 max_notifiers) {
    class_capacity = max_classes;
    type_capacity = max_types;
    mapping_capacity = max_mappings;
    notifier_capacity = max_notifiers;
    type_count = 0;
    class_count = 0;
    object_count = 0;
    notifier_count = 0;
    types = reinterpret_cast<EdType *>(ALIGN(buffer.addr, 16));
    buffer.addr = reinterpret_cast<usize>(types) + sizeof(EdType) * max_types;
    memset(types, 0, sizeof(EdType) * max_types);
    classes = reinterpret_cast<EdClass *>(ALIGN(buffer.addr, 16));
    buffer.addr = reinterpret_cast<usize>(classes) + sizeof(EdClass) * class_capacity;
    memset(classes, 0, sizeof(EdClass) * class_capacity);
    mappings = reinterpret_cast<NameMapping *>(ALIGN(buffer.addr, 16));
    buffer.addr = reinterpret_cast<usize>(mappings) + sizeof(NameMapping) * mapping_capacity;
    memset(mappings, 0, sizeof(NameMapping) * mapping_capacity);
    notifiers = reinterpret_cast<EdObjectNotifier **>(ALIGN(buffer.addr, 16));
    buffer.addr = reinterpret_cast<usize>(notifiers) + sizeof(EdObjectNotifier *) * notifier_capacity;
    memset(notifiers, 0, sizeof(EdObjectNotifier *) * notifier_capacity);
    initialised = 1;
}

void EdRegistry::Flush() {
    type_count = 0;
    class_count = 0;
    object_count = 0;
}

i32 EdRegistry::RegisterType(char *name, i32 size, void (*serialise)(EdStream &, void *, i32)) {
    i32 index = type_count++;
    EdType *type = &types[index];
    type->name = name;
    type->size = size;
    type->serialise = serialise;
    return index;
}

void EdRegistry::RegisterBaseTypes() {
    EdType_Char = RegisterType("Char", 1, SerialiseChar);
    EdType_Short = RegisterType("Short", 2, SerialiseShort);
    EdType_Int = RegisterType("Int", 4, SerialiseInt);
    EdType_Float = RegisterType("Float", 4, SerialiseFloat);
    EdType_VuVec = RegisterType("VuVec", 16, SerialiseVuVec);
    EdType_VuMtx = RegisterType("VuMtx", 64, SerialiseVuMtx);
    EdType_Enumeration = RegisterType("Enum", 4, SerialiseInt);
    EdType_String = RegisterType("String", -1, SerialiseString);
    EdType_Colour3 = RegisterType("Colour3", 12, SerialiseColour3);
    EdType_NuHSpecial = RegisterType("NuHSpecial", 12, SerialiseNuHSpecial);
    EdType_NuVec = RegisterType("NuVec", 12, SerialiseNuVec);
    EdType_NuMtx = RegisterType("NuMtx", 64, SerialiseNuMtx);
}

void ClassEditor::Initialise(variptr_u &first, variptr_u &second, i32) {
    theRegistry.Initialise(first, second, 32, 32, 10, 1);
    theRegistry.RegisterBaseTypes();
    RegisterTool(theCursorTool);
    RegisterTool(theSplineTool);
    active_tool = &theCursorTool;
}

EdClass *EdRegistry::RegisterClass(char *name, EdClassInterface *interface, i32 flags) {
    i32 index = class_count++;
    EdClass *object_class = &classes[index];
    object_class->name = name;
    object_class->interface = interface;
    object_class->flags = flags;
    if (interface != nullptr) {
        interface->object_class = object_class;
    }
    return object_class;
}

i32 EdRegistry::AddMapping(char *source, char *destination) {
    NameMapping *mapping = &mappings[object_count++];
    mapping->source = source;
    mapping->destination = destination;
    return 1;
}

char *EdRegistry::MapName(char *name) {
    for (i32 index = 0; index < object_count; ++index) {
        if (NuStrICmp(mappings[index].source, name) == 0) {
            name = mappings[index].destination;
            break;
        }
    }
    return name;
}

i32 EdRegistry::GetTypeId(char *name) {
    for (i32 index = 0; index < type_count; ++index) {
        if (NuStrICmp(types[index].name, name) == 0) {
            return index;
        }
    }
    return -1;
}

EdType *EdRegistry::GetType(char *name) {
    for (i32 index = 0; index < type_count; ++index) {
        if (NuStrICmp(types[index].name, name) == 0) {
            return &types[index];
        }
    }
    return nullptr;
}

EdType *EdRegistry::GetType(i32 index) {
    if (index < 0 || index >= type_count) {
        return nullptr;
    }
    return &types[index];
}

void EdRef::CheckType(i32 requested_type) {
    if (type_id == requested_type || (type_id == EdType_NuVec && requested_type == EdType_VuVec) ||
        (type_id == EdType_NuMtx && requested_type == EdType_VuMtx)) {
        return;
    }
    theRegistry.GetType(requested_type);
    theRegistry.GetType(type_id);
}

void EdRefSpecialObject::SetMemberData(void *object, i32 type, void *data, i32, i16 *) {
    SpecialObject *special_object = static_cast<SpecialObject *>(object);
    CheckType(type);
    switch (member_offset) {
        case static_cast<i32>(0x80000008):
            special_object->special = *static_cast<nuhspecial_s *>(data);
            break;
        case static_cast<i32>(0x80000009):
            NuSpecialSetVisibility(&special_object->special, *static_cast<i32 *>(data));
            break;
        case static_cast<i32>(0x8000000a):
            NuSpecialSetCollision(&special_object->special, *static_cast<i32 *>(data));
            break;
    }
}

void EdRefSpecialObject::GetMemberData(void *object, i32 type, void *data, i32) {
    SpecialObject *special_object = static_cast<SpecialObject *>(object);
    CheckType(type);
    switch (member_offset) {
        case static_cast<i32>(0x80000008):
            *static_cast<nuhspecial_s *>(data) = special_object->special;
            break;
        case static_cast<i32>(0x80000009):
            *static_cast<i32 *>(data) = NuSpecialGetVisibilityFn(&special_object->special);
            break;
        case static_cast<i32>(0x8000000a):
            *static_cast<i32 *>(data) = NuSpecialGetCollision(&special_object->special);
            break;
    }
}

void EdRefPlaceable::SetMemberData(void *object, i32 type, void *data, i32, i16 *) {
    Placeable *placeable = static_cast<Placeable *>(object);
    CheckType(type);
    switch (member_offset) {
        case static_cast<i32>(0x80000001):
            placeable->led_file = *static_cast<i16 *>(data);
            break;
        case static_cast<i32>(0x80000002):
            placeable->attributes = *static_cast<i32 *>(data);
            break;
        case static_cast<i32>(0x80000003):
            if (NuStrLen(static_cast<char *>(data)) > 0) {
                placeable->SetName(static_cast<char *>(data));
            } else {
                placeable->SetName(NULL);
            }
            break;
        case static_cast<i32>(0x80000005):
            if (NuStrLen(static_cast<char *>(data)) > 0) {
                placeable->params.Set(static_cast<char *>(data));
            } else {
                placeable->params.Set(NULL);
            }
            break;
        case static_cast<i32>(0x80000006):
            placeable->SetInitialTransform(static_cast<VuMtx *>(data));
            placeable->SetCurrentTransform(static_cast<VuMtx *>(data));
            break;
    }
}

void EdRefPlaceable::GetMemberData(void *object, i32 type, void *data, i32 data_size) {
    Placeable *placeable = static_cast<Placeable *>(object);
    CheckType(type);
    switch (member_offset) {
        case static_cast<i32>(0x80000001):
            *static_cast<i16 *>(data) = placeable->led_file;
            break;
        case static_cast<i32>(0x80000002):
            *static_cast<i32 *>(data) = placeable->attributes;
            break;
        case static_cast<i32>(0x80000003): {
            const char *object_name = placeable->GetName();
            NuStrNCpy(static_cast<char *>(data), object_name ? object_name : "", data_size);
            break;
        }
        case static_cast<i32>(0x80000005): {
            const char *params = placeable->params.data ? placeable->params.data + 1 : NULL;
            // The original tests the reference name before copying parameter text.
            NuStrNCpy(static_cast<char *>(data), name ? params : "", data_size);
            break;
        }
        case static_cast<i32>(0x80000006): {
            const VuMtx *matrix = placeable->GetCurrentTransform();
            if (matrix) {
                *static_cast<VuMtx *>(data) = *matrix;
            }
            break;
        }
        case static_cast<i32>(0x80000007):
            *static_cast<f32 *>(data) = placeable->GetRadius();
            break;
    }
}

i32 EdRef::GetTypeSize(i32 requested_type, i32 data_size) {
    CheckType(requested_type);
    i32 type_size = size;
    if (type_size <= 0) {
        type_size = theRegistry.GetType(type_id)->size;
    }
    if (type_size > data_size && data_size > 0) {
        theRegistry.GetType(type_id);
    }
    return type_size;
}

void EdRef::SetMemberData(void *object, i32 requested_type, void *data, i32 data_size, i16 *) {
    void *member = static_cast<u8 *>(object) + member_offset;
    i32 type_size = GetTypeSize(requested_type, data_size);
    if (member != NULL) {
        if (attributes & 0x40000000) {
            member = *static_cast<void **>(member);
        }
        memmove(member, data, type_size);
    }
}

void EdRef::GetMemberData(void *object, i32 requested_type, void *data, i32 data_size) {
    void *member = static_cast<u8 *>(object) + member_offset;
    i32 type_size = GetTypeSize(requested_type, data_size);
    if (member != NULL) {
        if (attributes & 0x40000000) {
            member = *static_cast<void **>(member);
        }
        memmove(data, member, type_size);
    }
}

__attribute__((force_align_arg_pointer)) void EdRefKnot::GetMemberData(void *object, i32 type, void *data,
                                                                       i32 data_size) {
    SplineKnot *knot = static_cast<SplineKnot *>(object);
    CheckType(type);
    switch (member_offset) {
        case static_cast<i32>(0x80000001):
            NuStrNCpy(static_cast<char *>(data), knot->spline->name, data_size);
            break;
        case static_cast<i32>(0x80000002):
            *static_cast<f32 *>(data) = 0.25f * EdManipulator::Scale;
            break;
        // The original copies each 16-byte vector with paired SSE half-loads and stores.
        case static_cast<i32>(0x80000003): {
#if defined(__SSE__)
            const __m64 *source = reinterpret_cast<const __m64 *>(&knot->position);
            __m128 lanes = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), source), source + 1);
            _mm_storel_pi(reinterpret_cast<__m64 *>(data), lanes);
            _mm_storeh_pi(reinterpret_cast<__m64 *>(static_cast<f32 *>(data) + 2), lanes);
#else
            *static_cast<VuVec *>(data) = knot->position;
#endif
            break;
        }
        case static_cast<i32>(0x80000004): {
#if defined(__SSE__)
            const __m64 *source = reinterpret_cast<const __m64 *>(&knot->in_tangent);
            __m128 lanes = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), source), source + 1);
            _mm_storel_pi(reinterpret_cast<__m64 *>(data), lanes);
            _mm_storeh_pi(reinterpret_cast<__m64 *>(static_cast<f32 *>(data) + 2), lanes);
#else
            *static_cast<VuVec *>(data) = knot->in_tangent;
#endif
            break;
        }
        case static_cast<i32>(0x80000005): {
#if defined(__SSE__)
            const __m64 *source = reinterpret_cast<const __m64 *>(&knot->out_tangent);
            __m128 lanes = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), source), source + 1);
            _mm_storel_pi(reinterpret_cast<__m64 *>(data), lanes);
            _mm_storeh_pi(reinterpret_cast<__m64 *>(static_cast<f32 *>(data) + 2), lanes);
#else
            *static_cast<VuVec *>(data) = knot->out_tangent;
#endif
            break;
        }
        default:
            EdRef::GetMemberData(object, type, data, data_size);
            break;
    }
}

void EdRefSpline::GetMemberData(void *object, i32 type, void *data, i32 data_size) {
    SplineObject *spline = static_cast<SplineObject *>(object);
    CheckType(type);
    switch (member_offset) {
        case static_cast<i32>(0x80000001):
            *static_cast<f32 *>(data) = spline->step;
            break;
        case static_cast<i32>(0x80000002):
            *static_cast<f32 *>(data) = spline->height;
            break;
        case static_cast<i32>(0x80000003):
            *static_cast<i32 *>(data) = spline->drop != 0;
            break;
        case static_cast<i32>(0x80000004):
            *static_cast<i32 *>(data) = spline->closed;
            break;
        default:
            EdRef::GetMemberData(object, type, data, data_size);
            break;
    }
}

void PropertyTool::GetTypeName(EdRef *reference, char *name) {
    if (show_type_names != 0) {
        EdType *type = theRegistry.GetType(reference->type_id);
        if (type != NULL) {
            sprintf(name, "%s %s", type->name, reference->name);
            return;
        }
    }
    NuStrCpy(name, reference->name);
}

i32 EdRegistry::GetClassId(char *name) {
    for (i32 index = 0; index < class_count; ++index) {
        if (NuStrICmp(classes[index].name, name) == 0) {
            return index;
        }
    }
    return -1;
}

i32 EdRegistry::GetClassId(EdClass *object_class) {
    return object_class - classes;
}

EdClass *EdRegistry::GetClass(char *name) {
    for (i32 index = 0; index < class_count; ++index) {
        if (NuStrICmp(classes[index].name, name) == 0) {
            return &classes[index];
        }
    }
    return nullptr;
}

EdClass *EdRegistry::GetClass(i32 index) {
    if (index < 0 || index >= class_count) {
        return nullptr;
    }
    return &classes[index];
}

void PropertyTool::GetClassName(EdRef *reference, char *name) {
    if (show_type_names != 0) {
        EdClass *ed_class = theRegistry.GetClass(reference->type_id);
        if (ed_class != NULL) {
            sprintf(name, "%s %s", ed_class->name, reference->name);
            return;
        }
    }
    NuStrCpy(name, reference->name);
}

void EdClassObjectNameControl::cbButton(eduimenu_s *parent, eduiitem_s *item, u32) {
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    edClassObjectNameControl = static_cast<EdClassObjectNameControl *>(item->data_ptr);
    if (menu == NULL)
        return;
    for (i32 index = 0; index < theRegistry.class_count; ++index) {
        EdClass *ed_class = theRegistry.GetClass(index);
        if ((ed_class->flags & 0x20000000) == 0 && ed_class->interface != NULL) {
            eduiMenuAddItem(menu, eduiItemSelCreate(index, &EdLevelAttr, 0, 0, cbSelectClass, ed_class->name));
        }
    }
    if (menu->first == NULL) {
        eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect,
                                                const_cast<char *>("No Registered Classes")));
    }
    menu->flags |= 1;
    eduiMenuAttach(parent, menu);
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 5);
    static_cast<edui_prop_s *>(item)->unknown_property_flags &= ~8;
}

EdRef::EdRef(char *type_name, char *member_name, i32 offset, i32 member_size, i32 member_attributes,
             EdControl *member_control, i32 group)
    : next(NULL), previous(NULL), attributes(0) {
    char direct_type[32];
    i32 length = NuStrLen(type_name);
    if (length > 0 && type_name[length - 1] == '*') {
        NuStrCpy(direct_type, type_name);
        direct_type[length - 1] = '\0';
        type_name = direct_type;
        attributes |= 0x40000000;
    }
    type_id = theRegistry.GetTypeId(type_name);
    if (type_id < 0) {
        type_id = theRegistry.GetClassId(type_name);
        theRegistry.GetClass(type_id);
        attributes |= 0x80000000;
    } else {
        theRegistry.GetType(type_id);
    }
    name = member_name;
    member_offset = offset;
    size = member_size;
    attributes |= member_attributes;
    control = member_control;
    replication_group = group;
}

void PropertyTool::AddPropertyMenuItems(eduimenu_s *menu, EdClass *ed_class, void *object, eduiitem_s *parent) {
    auto menu_last = menu->last;
    for (EdRef *member = ed_class->members; member != NULL; member = member->next) {
        if (member->attributes < 0) {
            eduiitem_s *expander = eduiItemExpanderCreate(0, &EdLevelAttr, 0, member->name);
            if (parent != NULL) {
                eduiItemExpanderAddChild(static_cast<edui_expander_s *>(parent), expander);
            } else {
                eduiMenuAddItem(menu, expander);
            }
            EdClass *child_class = theRegistry.GetClass(member->type_id);
            AddPropertyMenuItems(menu, child_class, member->GetMemberObject(object), expander);
        } else if (member->control != NULL) {
            eduiitem_s *last = menu_last;
            member->control->AddMenuItem(menu, member, object);
            if (parent != NULL) {
                eduiitem_s *item = last != NULL ? last->next : menu->first;
                while (item != NULL) {
                    eduiitem_s *next = item->next;
                    eduiMenuRemoveItem(menu, item);
                    eduiItemExpanderAddChild(static_cast<edui_expander_s *>(parent), item);
                    item = next;
                }
            }
        }
    }
}

void EdRegistry::ClassIFaceProcess(i32 class_id, void *object, EdInputContext &context) {
    EdClass *object_class = GetClass(class_id);
    if (object_class && object_class->interface) {
        EdClassInterface *interface = object_class->interface;
        interface->vtable->process(interface, object, context);
    }
}

void EdRegistry::ClassIFaceProcess(EdClass *object_class, void *object, EdInputContext &context) {
    if (object_class && object_class->interface) {
        EdClassInterface *interface = object_class->interface;
        interface->vtable->process(interface, object, context);
    }
}

void EdRegistry::ClassIFaceRender(i32 class_id, void *object, i32 flags) {
    EdClass *object_class = GetClass(class_id);
    if (object_class && object_class->interface) {
        EdClassInterface *interface = object_class->interface;
        interface->vtable->render(interface, object, flags);
    }
}

void EdRegistry::ClassIFaceRender(EdClass *object_class, void *object, i32 flags) {
    if (object_class && object_class->interface) {
        EdClassInterface *interface = object_class->interface;
        interface->vtable->render(interface, object, flags);
    }
}

void EdRegistry::AddObjectNotifier(EdObjectNotifier *notifier) {
    if (notifier_count < notifier_capacity) {
        notifiers[notifier_count++] = notifier;
    }
}

void EdRegistry::NotifyCreateObject(void *object, EdClass *object_class, void *source, i32 index, i32 context,
                                    i32 flags) {
    for (i32 i = 0; i < notifier_count; ++i) {
        EdObjectNotifier *notifier = notifiers[i];
        notifier->NotifyCreateObject(object, object_class, source, index, context, flags);
    }
}

void *EdRegistry::CreateObject(EdClassInterface *interface, void *source, i32 index, i32 guid, i32 flags) {
    if (!guid && create_object_guid) {
        guid = create_object_guid();
    }
    void *object = interface->vtable->create_object(interface, source, index, flags);
    if (object) {
        interface->vtable->set_object_guid(interface, object, guid);
        if (!(flags & 2)) {
            NotifyCreateObject(object, interface->object_class, source, index, guid, flags);
        }
    }
    return object;
}

void EdRegistry::NotifyDestroyObject(void *object, EdClass *object_class, i32 index, i32 flags) {
    for (i32 i = 0; i < notifier_count; ++i) {
        EdObjectNotifier *notifier = notifiers[i];
        notifier->NotifyDestroyObject(object, object_class, index, flags);
    }
}

void EdRegistry::DestroyObject(EdClassInterface *interface, void *object, i32 index, i32 flags) {
    theLevelEditor.destroying_objects = 1;
    if (object) {
        if (!(flags & 2)) {
            NotifyDestroyObject(object, interface->object_class, index, flags);
        }
        interface->vtable->destroy_object(interface, object, flags);
    }
    theLevelEditor.destroying_objects = 0;
}

void ClassEditor::DestroySelectedObjectsNow() {
    while (selected_objects.first != NULL) {
        ClassObjectListEntry *entry = selected_objects.first;
        if (current_object.object == entry->object && current_object.reference == entry->reference) {
            current_object.ed_class = NULL;
            current_object.object = NULL;
        }
        theRegistry.DestroyObject(entry->ed_class->interface, entry->object, 0, 0);
        if (entry->next != NULL) {
            entry->next->previous = entry->previous;
        } else {
            selected_objects.last = entry->previous;
        }
        if (entry->previous != NULL) {
            entry->previous->next = entry->next;
        } else {
            selected_objects.first = entry->next;
        }
        entry->next = NULL;
        entry->previous = NULL;
        --selected_objects.count;
        theMemoryManager.FreePool(entry, sizeof(*entry));
    }
}

void cbCEDeleteConfirmed(eduimenu_s *menu, eduiitem_s *item, u32 buttons) {
    theClassEditor.DestroySelectedObjectsNow();
    cbEdLevelDestroyOnSelect(menu, item, buttons);
}

void ClassEditor::cbDestroyObject(eduimenu_s *, eduiitem_s *item, u32) {
    if (item->data != 0) {
        theClassEditor.DestroySelectedObjectsNow();
    }
    theLevelEditor.CloseMenu();
}

void EdRegistry::NotifyDefunctObject(void *object, EdClass *object_class, i32 flags) {
    for (i32 i = 0; i < notifier_count; ++i) {
        EdObjectNotifier *notifier = notifiers[i];
        notifier->NotifyDefunctObject(object, object_class, flags);
    }
}

void EdRegistry::DefunctObject(EdClassInterface *interface, void *object, i32, i32 flags) {
    if (object) {
        if (!(flags & 2)) {
            NotifyDefunctObject(object, interface->object_class, flags);
        }
        interface->vtable->defunct_object(interface, object);
        EdDefunctListEntry *entry = new EdDefunctListEntry;
        entry->object_class = interface->object_class;
        entry->object = object;
        entry->next = nullptr;
        entry->previous = defunct_objects.last;
        if (defunct_objects.last) {
            defunct_objects.last->next = entry;
        }
        defunct_objects.last = entry;
        if (!defunct_objects.first) {
            defunct_objects.first = entry;
        }
        ++defunct_objects.count;
    }
}

void EdRegistry::NotifyReviveObject(void *object, EdClass *object_class, i32 flags) {
    for (i32 i = 0; i < notifier_count; ++i) {
        EdObjectNotifier *notifier = notifiers[i];
        notifier->NotifyReviveObject(object, object_class, flags);
    }
}

void EdDefunctList::ReviveAll(i32 flags) {
    while (first) {
        EdDefunctListEntry *entry = first;
        EdClassInterface *interface = entry->object_class->interface;
        interface->vtable->revive_object(interface, entry->object);
        if (!(flags & 2)) {
            theRegistry.NotifyReviveObject(entry->object, entry->object_class, flags);
        }
        if (entry->next) {
            entry->next->previous = entry->previous;
        } else {
            last = entry->previous;
        }
        if (entry->previous) {
            entry->previous->next = entry->next;
        } else {
            first = entry->next;
        }
        entry->next = nullptr;
        entry->previous = nullptr;
        --count;
        delete entry;
    }
}

void EdType::Serialise(EdStream &stream) {
    if (stream.BeginBlock("Type")) {
        stream.SerialiseString(&name);
        stream.SerialiseBuffer(&size, sizeof(size), 1);
        stream.EndBlock();
    }
}

void EdClass::SerialiseObject(EdStream &stream, void *object) {
    if (stream.BeginBlock("Object") == NULL) {
        return;
    }
    if (object != NULL) {
        // The original keeps this scratch buffer 16-byte aligned.
        u8 data[256] __attribute__((aligned(16)));
        for (EdRef *member = members; member != NULL; member = member->next) {
            if (!((stream.flags & 0x400000) != 0 ? (member->attributes & 0x400000) != 0
                                                 : (member->attributes & 0x10000000) != 0)) {
                if (member->attributes < 0) {
                    EdClass *member_class = theRegistry.GetClass(member->type_id);
                    member_class->SerialiseObject(stream, member->GetMemberObject(object));
                } else {
                    EdType *type = theRegistry.GetType(member->type_id);
                    i32 size = member->size > 0 ? member->size : type->size;
                    if (stream.mode == 2) {
                        member->GetMemberData(object, member->type_id, data, sizeof(data));
                    }
                    type->serialise(stream, data, size);
                    if (stream.mode == 1) {
                        member->SetMemberData(object, member->type_id, data, sizeof(data), NULL);
                    }
                }
            }
        }
        if (interface != NULL) {
            interface->vtable->serialise_object(interface, stream, object);
        }
    }
    stream.EndBlock();
}

void SplineHelper::SerialiseObject(EdStream &stream, void *object) {
    SplineObject *spline = static_cast<SplineObject *>(object);
    i32 knot_count;
    if (stream.mode == 2) {
        knot_count = spline->knots.count;
        stream.SerialiseBuffer(&knot_count, sizeof(knot_count), 1);
        for (SplineKnot *knot = spline->knots.first; knot != NULL; knot = knot->next)
            theKnotHelper.object_class->SerialiseObject(stream, knot);
    }
    if (stream.mode == 1) {
        stream.SerialiseBuffer(&knot_count, sizeof(knot_count), 1);
        for (i32 index = 0; index < knot_count; ++index) {
            SplineKnot *knot = static_cast<SplineKnot *>(theMemoryManager.AllocPool(sizeof(SplineKnot), 1));
            knot->previous = NULL;
            knot->next = NULL;
            knot->led_file = Placeable::CurrentLedFile;
            theKnotHelper.object_class->SerialiseObject(stream, knot);
            knot->spline = spline;
            knot->previous = spline->knots.last;
            knot->next = NULL;
            if (spline->knots.last != NULL)
                spline->knots.last->next = knot;
            const bool had_first = spline->knots.first != NULL;
            spline->knots.last = knot;
            if (__builtin_expect(!had_first, 1))
                spline->knots.first = knot;
            ++spline->knots.count;
        }
    }
}

i32 EdClass::SerialiseObjectHeader(EdStream &stream, void *object) {
    // The original stores this flag in a 16-byte aligned slot of a realigned frame.
    u8 present __attribute__((aligned(16))) = 0;
    if (stream.mode == 2 && object != NULL) {
        present = 1;
    }
    stream.SerialiseBuffer(&present, 1, 1);
    return present;
}

void EdClass::CopyObject(void *destination, void *source) {
    for (EdRef *member = members; member != NULL; member = member->next) {
        if ((member->attributes & 0x1800000) != 0) {
            continue;
        }
        if (member->attributes < 0) {
            EdClass *member_class = theRegistry.GetClass(member->type_id);
            void *source_member = member->GetMemberObject(source);
            void *destination_member = member->GetMemberObject(destination);
            member_class->CopyObject(destination_member, source_member);
        } else {
            theRegistry.GetType(member->type_id);
            u8 data[256];
            member->GetMemberData(source, member->type_id, data, sizeof(data));
            member->SetMemberData(destination, member->type_id, data, sizeof(data), NULL);
        }
    }
}

void EdClass::AddType(EdRef *member) {
    member->next = NULL;
    member->previous = last_member;
    if (last_member != NULL) {
        last_member->next = member;
    }
    EdRef *first = members;
    last_member = member;
    if (first == NULL) {
        members = member;
    }
    ++member_count;
    flags |= member->attributes & 0x17a;
    if (member->attributes < 0) {
        flags |= theRegistry.GetClass(member->type_id)->flags & 0x17a;
    }
}

void EdClass::Serialise(EdStream &stream, i32 *class_mapping) {
    if (stream.BeginBlock("Class") == NULL) {
        return;
    }
    stream.SerialiseString(&name);
    i32 count __attribute__((aligned(16)));
    if (stream.version == 0) {
        stream.SerialiseBuffer(&count, sizeof(count), 1);
    }
    if (stream.mode != 2) {
        if (stream.mode == 1) {
            if (stream.version <= 2) {
                stream.SerialiseBuffer(&count, sizeof(count), 1);
                for (i32 i = 0; i < count; ++i) {
                    EdRef *member = new (stream.secondary_buffer->Allocate(sizeof(EdRef))) EdRef;
                    member->Serialise(stream, NULL);
                    AddType(member);
                }
            }
            stream.SerialiseBuffer(&count, sizeof(count), 1);
            for (i32 i = 0; i < count; ++i) {
                EdRef *member = new (stream.secondary_buffer->Allocate(sizeof(EdRef))) EdRef;
                member->Serialise(stream, NULL);
                AddType(member);
            }
        }
    } else {
        count = 0;
        for (EdRef *member = members; member != NULL; member = member->next) {
            if ((stream.flags & 0x400000) != 0) {
                if ((member->attributes & 0x400000) != 0) {
                    continue;
                }
            } else {
                if ((class_mapping != NULL && member->attributes < 0 && class_mapping[member->type_id] == -1) ||
                    (member->attributes & 0x10000000) != 0) {
                    continue;
                }
            }
            ++count;
        }
        stream.SerialiseBuffer(&count, sizeof(count), 1);
        for (EdRef *member = members; member != NULL; member = member->next) {
            if ((stream.flags & 0x400000) != 0) {
                if ((member->attributes & 0x400000) != 0) {
                    continue;
                }
            } else {
                if ((class_mapping != NULL && member->attributes < 0 && class_mapping[member->type_id] == -1) ||
                    (member->attributes & 0x10000000) != 0) {
                    continue;
                }
            }
            member->Serialise(stream, class_mapping);
        }
    }
    stream.EndBlock();
}

void PlaceableHelper::Initialise() {
    EdClass *object_class = theRegistry.RegisterClass("Placeable", NULL, 0);
    if (object_class == NULL)
        return;
    add_placeable_reference<EdRefPlaceable>(object_class, "Short", "LEDFile", static_cast<i32>(0x80000001), 0,
                                            0x10000100);
    add_placeable_reference<EdRefPlaceable>(object_class, "Int", "Attributes", static_cast<i32>(0x80000002), 0, 1);
    void *name_storage = theMemoryManager.AllocPool(sizeof(EdStringControl), 1);
    EdStringControl *name_control = new (name_storage) EdStringControl();
    add_placeable_reference<EdRefPlaceable>(object_class, "String", "Name", static_cast<i32>(0x80000003), 250, 2,
                                            name_control);
    void *params_storage = theMemoryManager.AllocPool(sizeof(EdStringControl), 1);
    EdStringControl *params_control = new (params_storage) EdStringControl();
    add_placeable_reference<EdRefPlaceable>(object_class, "String", "Params", static_cast<i32>(0x80000005), 250, 0,
                                            params_control);
    void *matrix_storage = theMemoryManager.AllocPool(sizeof(EdMatrixControl), 1);
    EdMatrixControl *matrix_control = new (matrix_storage) EdMatrixControl();
    add_placeable_reference<EdRefPlaceable>(object_class, "VuMtx", "Mtx", static_cast<i32>(0x80000006), 0, 0x38,
                                            matrix_control);
}

i32 EdClass::GetStreamClasses(EdStream &stream, i32 *classes, i32 &count, i32 capacity) {
    i32 result = 0;
    i32 class_index = count;
    if (class_index < capacity) {
        classes[class_index] = theRegistry.GetClassId(this);
        count = class_index + 1;
    }
    for (EdRef *member = members; member != NULL; member = member->next) {
        if (member->attributes >= 0) {
            continue;
        }
        if ((stream.flags & 0x400000) != 0) {
            if ((member->attributes & 0x400000) != 0) {
                continue;
            }
        } else if ((member->attributes & 0x10000000) != 0) {
            continue;
        }
        theRegistry.GetClass(member->type_id)->GetStreamClasses(stream, classes, count, capacity);
        result = 1;
    }
    return result;
}

void EdRegistry::GetStreamClassMapping(EdStream &stream, i32 *mapping, i32 &count, i32) {
    i32 used_classes[64] = {};
    i32 loop_limit = class_count;
    for (i32 index = 0; index < loop_limit; ++index) {
        EdClass *object_class = &classes[index];
        i32 object_flags = object_class->flags;
        if (stream.flags & 0x400000) {
            if (object_flags & 0x400000) {
                continue;
            }
        } else if (object_flags & 0x10000000) {
            continue;
        }
        i32 stream_classes[64];
        i32 stream_class_count = 0;
        object_class->GetStreamClasses(stream, stream_classes, stream_class_count, 64);
        for (i32 stream_class = 0; stream_class < stream_class_count; ++stream_class) {
            used_classes[stream_classes[stream_class]] = 1;
        }
        loop_limit = class_count;
    }
    count = 0;
    for (i32 index = 0; index < class_count; ++index) {
        if (used_classes[index]) {
            mapping[index] = count++;
        } else {
            mapping[index] = -1;
        }
    }
}

void EdRegistry::Serialise(EdStream &stream) {
    if (stream.BeginBlock("TypeList")) {
        stream.SerialiseBuffer(&type_count, sizeof(type_count), 1);
        for (i32 index = 0; index < type_count; ++index) {
            types[index].Serialise(stream);
        }
        stream.EndBlock();
    }
    if (stream.BeginBlock("ClassList")) {
        if (stream.mode == 2) {
            i32 count = 0;
            i32 mapping[64];
            GetStreamClassMapping(stream, mapping, count, 64);
            stream.SerialiseBuffer(&count, sizeof(count), 1);
            for (i32 index = 0; index < class_count; ++index) {
                if (mapping[index] != -1) {
                    classes[index].Serialise(stream, mapping);
                }
            }
        }
        if (stream.mode == 1) {
            stream.SerialiseBuffer(&class_count, sizeof(class_count), 1);
            for (i32 index = 0; index < class_count; ++index) {
                classes[index].Serialise(stream, nullptr);
            }
        }
        stream.EndBlock();
    }
}

EdRef *EdClass::FindTypeRef(char *member_name, i32 recursive) {
    for (EdRef *member = members; member != NULL; member = member->next) {
        if (recursive == 0 || member->attributes >= 0) {
            if (__builtin_expect(NuStrICmp(member->name, member_name) == 0, 0)) {
                return member;
            }
        } else {
            EdRef *reference = theRegistry.GetClass(member->type_id)->FindTypeRef(member_name, 1);
            if (reference != NULL) {
                return reference;
            }
        }
    }
    return NULL;
}

void EdClass::SerialiseObject(EdStream &stream, void *object, EdClass *schema, EdRegistry *registry) {
    if (stream.BeginBlock("Object") == NULL) {
        return;
    }
    if (object != NULL) {
        u8 data[256] __attribute__((aligned(16)));
        for (EdRef *source = schema->members; source != NULL; source = source->next) {
            EdRef *member = FindTypeRef(source->name, 0);
            if (member != NULL) {
                if (stream.mode != 1) {
                    if (stream.mode != 2) {
                        continue;
                    }
                    if ((stream.flags & 0x400000) != 0 ? (member->attributes & 0x400000) != 0
                                                       : (member->attributes & 0x10000000) != 0) {
                        continue;
                    }
                }
                if (!(member->attributes < 0)) {
                    registry->GetType(source->type_id);
                    EdType *type = theRegistry.GetType(member->type_id);
                    i32 size = member->size > 0 ? member->size : type->size;
                    if (stream.mode == 2) {
                        member->GetMemberData(object, member->type_id, data, sizeof(data));
                    }
                    type->serialise(stream, data, size);
                    if (stream.mode == 1) {
                        member->SetMemberData(object, member->type_id, data, sizeof(data), NULL);
                    }
                } else {
                    EdClass *source_class = registry->GetClass(source->type_id);
                    EdClass *member_class = theRegistry.GetClass(member->type_id);
                    member_class->SerialiseObject(stream, member->GetMemberObject(object), source_class, registry);
                }
            } else {
                EdType *type = registry->GetType(source->type_id);
                if (source->size <= 0) {
                    stream.Eat(type->size, 1);
                } else if (NuStrICmp(type->name, "String") != 0) {
                    stream.Eat(source->size, 1);
                } else {
                    i32 length;
                    stream.SerialiseBuffer(&length, sizeof(length), 1);
                    stream.Eat(length, 1);
                }
            }
        }
        if (interface != NULL) {
            interface->vtable->serialise_object(interface, stream, object);
        }
    }
    stream.EndBlock();
}

EdRef *EdClass::FindTypeRef(i32 attributes, i32 recursive) {
    EdRef *member = members;
    if (member == NULL)
        return NULL;
    if (recursive == 0) {
        do {
            if (member->attributes >= 0 && (member->attributes & attributes) != 0)
                return member;
            member = member->next;
        } while (member != NULL);
        return NULL;
    }
    do {
        if (member->attributes >= 0) {
            if ((member->attributes & attributes) != 0)
                return member;
        } else {
            EdRef *reference = theRegistry.GetClass(member->type_id)->FindTypeRef(attributes, 1);
            if (reference != NULL)
                return reference;
        }
        member = member->next;
    } while (member != NULL);
    return NULL;
}

void EdClassObjectNameControl::cbSelectClass(eduimenu_s *parent, eduiitem_s *item, u32) {
    EdClass *ed_class = theRegistry.GetClass(item->data);
    edClassObjectNameControl->selected.ed_class = ed_class;
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (menu == NULL)
        return;
    EdRef *name_reference = ed_class->FindTypeRef(2, 1);
    if (name_reference != NULL) {
        for (void *object = ed_class->interface->vtable->get_next_object(ed_class->interface, NULL); object != NULL;
             object = ed_class->interface->vtable->get_next_object(ed_class->interface, object)) {
            char name[128];
            if (name_reference->GetAttributeData(object, 2, EdType_String, name, sizeof(name))) {
                eduiMenuAddItem(
                    menu, eduiItemSelCreate(reinterpret_cast<usize>(object), &EdLevelAttr, 0, 0, cbSelectObject, name));
            }
        }
    }
    if (menu->first == NULL) {
        eduiMenuAddItem(
            menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect, const_cast<char *>("No Object")));
    }
    menu->flags |= 1;
    eduiMenuAttach(parent, menu);
    eduiSetActiveMenu(menu);
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 5);
}

i32 EdClass::FindMember(EdMember *result, void *object, i32 attributes, i32 recursive) {
    EdRef *member = members;
    if (member == NULL)
        return 0;
    if (recursive == 0) {
        do {
            if (member->attributes >= 0 && (member->attributes & attributes) != 0)
                goto found;
            member = member->next;
        } while (member != NULL);
        return 0;
    }
    do {
        if (member->attributes < 0) {
            EdClass *member_class = theRegistry.GetClass(member->type_id);
            void *member_object = member->GetMemberObject(object);
            if (member_class->FindMember(result, member_object, attributes, 1) != 0)
                return 1;
        } else if ((member->attributes & attributes) != 0)
            goto found;
        member = member->next;
    } while (member != NULL);
    return 0;
found:
    result->object = object;
    result->reference = member;
    return 1;
}

f32 EdClassInterface::DistanceToObject(VuVec &point, void *object, EdRef **reference) {
    VuVec position;
    f32 radius = 0.0f;
    EdMember member;
    f32 distance = 3.402823466e38f;
    if (object_class->FindMember(&member, object, 8, 1)) {

        member.reference->GetAttributeData(member.object, 8, EdType_VuVec, &position, 0);
        if (object_class->FindMember(&member, object, 0x40, 1))
            member.reference->GetAttributeData(member.object, 0x40, EdType_Float, &radius, 0);
        VuVec delta;
        delta.x = point.x - position.x;
        delta.y = point.y - position.y;
        delta.z = point.z - position.z;
        distance = NuVecMag(&delta.xyz) - radius;
        if (distance < 0.0f)
            distance = 0.0f;
    }
    if (reference != NULL)
        *reference = NULL;
    return distance;
}

void ClassObject::Set(char *name) {
    ed_class = NULL;
    object = NULL;
    char *separator = NuStrChr(name, '.');
    if (separator == NULL) {
        return;
    }

    char class_name[128];
    char object_name[128];
    char candidate_name[128];
    // The original keeps this member lookup in a 16-byte aligned slot.
    EdMember member __attribute__((aligned(16)));
    *separator = '\0';
    NuStrCpy(class_name, name);
    *separator = '.';
    NuStrCpy(object_name, separator + 1);

    ed_class = theRegistry.GetClass(class_name);
    if (ed_class == NULL || !(ed_class->flags & 2) || ed_class->interface == NULL) {
        return;
    }

    EdClassInterface *interface = ed_class->interface;
    object = interface->vtable->get_next_object(interface, NULL);
    while (object != NULL) {
        if (ed_class->FindMember(&member, object, 2, 1)) {
            member.reference->GetAttributeData(member.object, 2, EdType_String, candidate_name, 128);
            if (NuStrICmp(object_name, candidate_name) == 0) {
                break;
            }
        }
        interface = ed_class->interface;
        object = interface->vtable->get_next_object(interface, object);
    }
}

i32 ClassEditor::IsUniqueName(char *name) {
    EdMember member;
    char candidate_name[128];
    for (i32 i = 0; i < theRegistry.class_count; ++i) {
        EdClass *ed_class = &theRegistry.classes[i];
        EdClassInterface *interface = ed_class->interface;
        if (interface == NULL || !(ed_class->flags & 2)) {
            continue;
        }

        void *object = interface->vtable->get_next_object(interface, NULL);
        if (object == NULL)
            continue;
        do {
            if (ed_class->FindMember(&member, object, 2, 1)) {
                member.reference->GetAttributeData(member.object, 2, EdType_String, candidate_name, 128);
                if (NuStrICmp(name, candidate_name) == 0) {
                    return 0;
                }
            }
            interface = ed_class->interface;
            object = interface->vtable->get_next_object(interface, object);
        } while (object != NULL);
    }
    return 1;
}

void ClassEditor::MakeUniqueName(char const *name, char *destination, i32 size) {
    i32 prefix_length = NuStrLen(name);
    const char *digit = name + prefix_length - 1;
    i32 digit_count = 0;
    while (*digit >= '0' && *digit <= '9') {
        --digit;
        ++digit_count;
    }

    i32 suffix;
    if (digit_count != 0) {
        prefix_length -= digit_count;
        suffix = NuAToI(const_cast<char *>(name + prefix_length));
    } else {
        suffix = 0;
        digit_count = 2;
    }

    if (prefix_length + digit_count >= size) {
        prefix_length = size + ~digit_count;
    }
    NuStrNCpy(destination, name, prefix_length + 1);
    char format[16];
    sprintf(format, "%%.%dd", digit_count);
    do {
        ++suffix;
        sprintf(destination + prefix_length, format, suffix);
    } while (!IsUniqueName(destination));
}

void ClassEditor::InitialiseObject(ClassObject &created) {
    EdMember member __attribute__((aligned(16)));
    if (created.ed_class->FindMember(&member, created.object, 2, 1)) {
        char name[128];
        char unique_name[128];
        member.reference->GetAttributeData(member.object, 2, EdType_String, name, sizeof(name));
        MakeUniqueName(name, unique_name, sizeof(unique_name));
        member.reference->SetAttributeData(member.object, 2, EdType_String, unique_name, 0);
    }
    if (created.ed_class->FindMember(&member, created.object, 8, 1)) {
        member.reference->SetAttributeData(member.object, 8, EdType_VuVec, reinterpret_cast<u8 *>(manipulator) + 0x50,
                                           0);
    }
}

i32 ClassEditor::CreateObject(EdClass *ed_class) {
    if (ed_class == NULL || ed_class->interface == NULL)
        return 0;
    void *object = theRegistry.CreateObject(ed_class->interface, NULL, 4, 0, 2);
    if (object == NULL)
        return 0;
    ClassObject created = {ed_class, object, NULL};
    InitialiseObject(created);
    theRegistry.NotifyCreateObject(created.object, created.ed_class, NULL, 0, 0, 0);
    SelectObject(created, 0);
    return 1;
}

i32 ClassEditor::CreateObject(i32 class_id) {
    if (theLevelEditor.current_led_file == -1) {
        SelectLED(class_id);
        return 1;
    }
    Placeable::CurrentLedFile = theLevelEditor.current_led_file;
    EdClass *ed_class = theRegistry.GetClass(class_id);
    if (ed_class == NULL || (ed_class->flags & 0x04000000))
        return 0;
    return CreateObject(ed_class);
}

i32 ClassEditor::CreateObject(ClassObject &source) {
    // The original keeps the created object descriptor and the flags word in 16-byte aligned slots.
    ClassObject created __attribute__((aligned(16))) = {source.ed_class, NULL, NULL};
    if (theLevelEditor.current_led_file == -1) {
        SelectLED(-1);
        return 1;
    }
    Placeable::CurrentLedFile = theLevelEditor.current_led_file;
    EdClassInterface *interface;
    if (created.ed_class->flags & 0x04000000) {
        created.object = theRegistry.CreateObject(created.ed_class->interface, source.object, 4, 0, 2);
    } else {
        created.object = theRegistry.CreateObject(created.ed_class->interface, NULL, 4, 0, 2);
    }
    if (created.object == NULL)
        return 0;

    if (source.ed_class->flags & 0x04000000) {
        i32 flags __attribute__((aligned(16))) = 0x04000000;
        i32 int_type = EdType_Int;
        if (created.reference == NULL || !created.reference->SetAttributeData(created.object, 1, int_type, &flags, 0)) {
            EdMember member __attribute__((aligned(16)));
            if (created.ed_class->FindMember(&member, created.object, 1, 1))
                member.reference->SetAttributeData(member.object, 1, int_type, &flags, 0);
        }
    } else {
        created.ed_class->CopyObject(created.object, source.object);
        interface = created.ed_class->interface;
        interface->vtable->construct(interface, created.object, source.object);
    }
    InitialiseObject(created);
    theRegistry.NotifyCreateObject(created.object, created.ed_class, NULL, 0, 0, 0);
    SelectObject(created, 0);
    return 1;
}

void ClassEditor::cbFileSelected(eduimenu_s *, eduiitem_s *item, u32) {
    if (item == NULL)
        return;
    theLevelEditor.current_led_file = static_cast<i16>(item->data);
    if (class_editor_create_pending != -1) {
        theClassEditor.CreateObject(class_editor_create_pending);
    } else if (theClassEditor.selected_objects.first != NULL) {
        ClassObjectListEntry *entry = theClassEditor.selected_objects.first;
        ClassObject object = {entry->ed_class, entry->object, entry->reference};
        theClassEditor.CreateObject(object);
    }
    theLevelEditor.CloseMenu();
}

i32 ClassEditor::cbEdCreateClassNewObject(i32 class_id) {
    EdClass *ed_class = theRegistry.GetClass(class_id);
    for (ClassObjectListEntry *entry = theClassEditor.selected_objects.first; entry != NULL; entry = entry->next) {
        if (entry->ed_class == ed_class) {
            return theClassEditor.CreateObject(*reinterpret_cast<ClassObject *>(&entry->ed_class));
        }
    }
    if (ed_class->flags & 0x04000000)
        return 0;
    return theClassEditor.CreateObject(class_id);
}

void ClassEditor::cbEdClassNewObject(eduimenu_s *parent, eduiitem_s *item, u32) {
    i32 class_id = static_cast<i32>(item->data);
    EdClass *ed_class = theRegistry.GetClass(class_id);
    if (theClassEditor.cbEdCreateClassNewObject(class_id)) {
        theLevelEditor.CloseMenu();
        return;
    }
    eduimenu_s *error_menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                            reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy,
                                            const_cast<char *>("Error!"));
    if (error_menu == NULL)
        return;
    eduiMenuAddItem(error_menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect,
                                                  const_cast<char *>("Failed to create new object")));
    const char *error_text = "Unknown class id";
    if (ed_class != NULL)
        error_text = "No selected objects to clone";
    if (ed_class == NULL || (ed_class->flags & 0x04000000))
        eduiMenuAddItem(error_menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect,
                                                      const_cast<char *>(error_text)));
    eduiMenuFitWidth(error_menu, 5);
    eduiMenuFitOnScreen(error_menu, 1);
    eduiMenuAttach(parent, error_menu);
}

i32 ClassEditor::cbEdCopySelectedObject(EdInputContext &) {
    if (theClassEditor.selected_objects.first != NULL) {
        return theClassEditor.CreateObject(
            *reinterpret_cast<ClassObject *>(&theClassEditor.selected_objects.first->ed_class));
    }
    return 0;
}

i32 ClassObjectList::GetAveragePosition(VuVec &average) {
    average = VuVec(0.0f, 0.0f, 0.0f, 1.0f);
    i32 position_count = 0;
    for (ClassObjectListEntry *entry = first; entry != NULL; entry = entry->next) {
        EdMember member;
        // The original's local vector is 16-byte aligned.
        VuVec position __attribute__((aligned(16)));
        i32 position_type = EdType_VuVec;
        bool found = entry->reference != NULL &&
                     entry->reference->GetAttributeData(entry->object, 8, position_type, &position, 0) != 0;
        if (!found) {
            found = entry->ed_class->FindMember(&member, entry->object, 8, 1) != 0 &&
                    member.reference->GetAttributeData(member.object, 8, position_type, &position, 0) != 0;
        }
        if (found) {
            average.x += position.x;
            average.y += position.y;
            average.z += position.z;
            ++position_count;
        }
    }
    if (position_count != 0) {
        float scale = 1.0f / position_count;
        average.x *= scale;
        average.y *= scale;
        average.z *= scale;
    }
    return position_count;
}

void ClassEditor::ViewSelected() {
    VuVec position;
    if (selected_objects.GetAveragePosition(position)) {
        theLevelEditor.background_colour[0] = position.x;
        theLevelEditor.background_colour[1] = position.y;
        theLevelEditor.background_colour[2] = position.z;
        theLevelEditor.background_colour[3] = position.w;
        edcamSetPos(reinterpret_cast<NUVEC *>(theLevelEditor.background_colour));
    }
}

i32 ClassObjectList::GetAveragePosition(VuVec &average, float &radius) {
    average = VuVec(0.0f, 0.0f, 0.0f, 1.0f);
    i32 position_count = 0;
    VuVec positions[64];
    // The original keeps these locals in 16-byte aligned slots of a realigned frame.
    float radii[64] __attribute__((aligned(16)));
    for (ClassObjectListEntry *entry = first; entry != NULL && position_count < 64; entry = entry->next) {
        VuVec &position = positions[position_count];
        EdMember member __attribute__((aligned(16)));
        i32 position_type = EdType_VuVec;
        if (entry->reference != NULL &&
            entry->reference->GetAttributeData(entry->object, 8, position_type, &position, 0))
            goto position_found;
        if (!entry->ed_class->FindMember(&member, entry->object, 8, 1))
            continue;
        if (!member.reference->GetAttributeData(member.object, 8, position_type, &position, 0))
            continue;
    position_found:
        average.x += position.x;
        average.y += position.y;
        average.z += position.z;
        radii[position_count] = 1.0f;
        i32 radius_type = EdType_Float;
        if (entry->reference != NULL &&
            entry->reference->GetAttributeData(entry->object, 64, radius_type, &radii[position_count], 0))
            goto radius_found;
        if (entry->ed_class->FindMember(&member, entry->object, 64, 1))
            member.reference->GetAttributeData(member.object, 64, radius_type, &radii[position_count], 0);
    radius_found:
        ++position_count;
    }
    if (position_count != 0) {
        float scale = 1.0f / position_count;
        average.x *= scale;
        average.y *= scale;
        average.z *= scale;
        radius = 0.0f;
        for (i32 index = 0; index < position_count; ++index) {
            NUVEC difference;
            difference.x = average.x - positions[index].x;
            difference.y = average.y - positions[index].y;
            difference.z = average.z - positions[index].z;
            float extent = NuVecMag(&difference) + radii[index];
            radius = extent > radius ? extent : radius;
        }
    }
    return position_count;
}

void ClassEditor::FocusSelected() {
    VuVec position __attribute__((aligned(16)));
    f32 radius;
    if (selected_objects.GetAveragePosition(position, radius)) {
        theLevelEditor.background_colour[0] = position.x;
        theLevelEditor.background_colour[1] = position.y;
        theLevelEditor.background_colour[2] = position.z;
        theLevelEditor.background_colour[3] = position.w;
        edcamSetPos(reinterpret_cast<NUVEC *>(theLevelEditor.background_colour));
        NUCAMERA *camera = edmainGetCamera();
        i32 angle = static_cast<i32>(camera->fov * 0.5f * 10430.3779296875f);
        f32 cosine = NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff];
        f32 sine = NuTrigTable[(angle >> 1) & 0x7fff];
        f32 distance = 1.2f * radius * (cosine / sine);
        edcamSetDist(-distance);
    }
}

void ClassEditor::cbEdClassSelectObject(eduimenu_s *, eduiitem_s *item, u32) {
    theClassEditor.pending_object.object = item->data_ptr;
    if (theClassEditor.pending_object.object != NULL) {
        theClassEditor.SelectObject(theClassEditor.pending_object, 0);
        theClassEditor.FocusSelected();
        theLevelEditor.CloseMenu();
    }
}

i32 ClassEditor::Editable(void *object, EdClass *object_class, i32 index) {
    i16 scene = 0;
    if (object == NULL) {
        return (class_filter >> index) & 1;
    }
    i32 short_type = EdType_Short;
    EdMember member __attribute__((aligned(16)));
    if (object_class->FindMember(&member, object, 256, 1)) {
        member.reference->GetAttributeData(member.object, 256, short_type, &scene, 0);
    }
    return theLevelEditor.IsEditable(scene) != 0;
}

i32 ClassEditor::FindNearestObject(VuVec &origin, VuVec &direction, ClassObject &result, ClassObject &after,
                                   i32 filter) {
    ClassObject candidates[16] __attribute__((aligned(16))) = {};
    i32 candidate_count = 0;
    for (i32 class_index = 0; class_index < theRegistry.class_count; ++class_index) {
        EdClass *ed_class = &theRegistry.classes[class_index];
        if (!Editable(NULL, ed_class, class_index) || ed_class->interface == NULL || (ed_class->flags & 8) == 0)
            continue;
        for (void *object = ed_class->interface->vtable->get_next_object(ed_class->interface, NULL); object != NULL;
             object = ed_class->interface->vtable->get_next_object(ed_class->interface, object)) {
            if (!Editable(object, ed_class, -1))
                continue;
            EdRef *reference = NULL;
            f32 distance = ed_class->interface->vtable->distance_to_ray(ed_class->interface, origin, direction, object,
                                                                        &reference);
            if (distance < 0.01f && candidate_count < 16)
                candidates[candidate_count++] = {ed_class, object, reference};
        }
    }
    if (candidate_count == 0)
        return 0;
    i32 choice = 0;
    if (after.object != NULL) {
        for (i32 index = 0; index < candidate_count; ++index) {
            if (candidates[index].object != after.object) {
                continue;
            }
            choice = (index + 1) % candidate_count;
            break;
        }
    }
    result = candidates[choice];
    return 1;
}

i32 ClassEditor::FindNearestObject(VuVec &origin, VuVec &direction, ClassObject &result, i32 filter) {
    EdClass *nearest_class = NULL;
    void *nearest_object = NULL;
    f32 nearest_distance = FLT_MAX;
    EdRef *nearest_reference = NULL;
    for (i32 class_index = 0; class_index < theRegistry.class_count; ++class_index) {
        EdClass *ed_class = &theRegistry.classes[class_index];
        if (!Editable(NULL, ed_class, class_index) || ed_class->interface == NULL || (ed_class->flags & 8) == 0)
            continue;
        for (void *object = ed_class->interface->vtable->get_next_object(ed_class->interface, NULL); object != NULL;
             object = ed_class->interface->vtable->get_next_object(ed_class->interface, object)) {
            if (!Editable(object, ed_class, -1))
                continue;
            // The original keeps the hit reference in a 16-byte aligned slot.
            EdRef *reference __attribute__((aligned(16))) = NULL;
            f32 distance = ed_class->interface->vtable->distance_to_ray(ed_class->interface, origin, direction, object,
                                                                        &reference);
            if (distance < nearest_distance) {
                nearest_class = ed_class;
                nearest_object = object;
                nearest_reference = reference;
                nearest_distance = distance;
            }
        }
    }
    if (filter != 0 && nearest_distance > 0.1f) {
        nearest_class = NULL;
        nearest_object = NULL;
    }
    result = {nearest_class, nearest_object, nearest_reference};
    return nearest_object != NULL;
}

void ClassEditor::UpdateSelectedObjects(EdInputContext &input) {
    for (ClassObjectListEntry *entry = selected_objects.first; entry != NULL;) {
        ClassObjectListEntry *next_entry = entry->next;
        void *selected_object = entry->object;
        EdClassInterface *interface = entry->ed_class->interface;
        void *object = interface->vtable->get_next_object(interface, NULL);
        for (;;) {
            if (object == NULL)
                break;
            if (object == selected_object)
                break;
            object = interface->vtable->get_next_object(interface, object);
        }
        if (object == NULL) {
            if (entry->next != NULL)
                entry->next->previous = entry->previous;
            else
                selected_objects.last = entry->previous;
            if (entry->previous != NULL)
                entry->previous->next = entry->next;
            else
                selected_objects.first = entry->next;
            entry->next = NULL;
            entry->previous = NULL;
            --selected_objects.count;
            theMemoryManager.FreePool(entry, sizeof(*entry));
        }
        entry = next_entry;
    }
    if (__builtin_expect(selected_objects.first == NULL, 0))
        return;
    ClassObjectListEntry *entry = selected_objects.first;
    EdClassInterface *interface = entry->ed_class->interface;
    void *object = NULL;
    if (input.GetPress(19) != 0.0f) {
        object = interface->vtable->get_next_object(interface, entry->object);
        if (object == NULL)
            object = interface->vtable->get_next_object(interface, NULL);
        for (i32 remaining = 4096; remaining != 0; --remaining) {
            if (Editable(object, entry->ed_class, -1))
                break;
            object = interface->vtable->get_next_object(interface, object);
            if (object == NULL)
                object = interface->vtable->get_next_object(interface, NULL);
        }
    }
    if (input.GetPress(20) != 0.0f) {
        void *current = entry->object;
        void *previous = NULL;
        auto entry_ed_class = entry->ed_class;
        for (i32 remaining = 4096; remaining != 0; --remaining) {
            if (Editable(current, entry_ed_class, -1))
                previous = current;
            current = interface->vtable->get_next_object(interface, current);
            if (__builtin_expect(current == NULL, 1))
                current = interface->vtable->get_next_object(interface, NULL);
            if (current == entry->object)
                break;
        }
        object = previous;
    }
    if (object != NULL) {
        ClassObject selected = {entry->ed_class, object, NULL};
        theClassEditor.SelectObject(selected, 0);
        theClassEditor.ViewSelected();
    }
}

i32 ClassEditor::FindNearestObject(VuVec &point, ClassObject &result, ClassObject &after, i32 filter) {
    EdMember member;
    EdClass *candidate_classes[16] = {};
    void *candidate_objects[16] = {};
    i32 candidate_count = 0;
    for (i32 class_index = 0; class_index < theRegistry.class_count; ++class_index) {
        EdClass *ed_class = &theRegistry.classes[class_index];
        if (!Editable(NULL, ed_class, class_index) || ed_class->interface == NULL || (ed_class->flags & 8) == 0 ||
            ed_class->interface->vtable->get_next_object == NULL)
            continue;
        for (void *object = ed_class->interface->vtable->get_next_object(ed_class->interface, NULL); object != NULL;
             object = ed_class->interface->vtable->get_next_object(ed_class->interface, object)) {
            if (!Editable(object, ed_class, -1))
                continue;

            if (!ed_class->FindMember(&member, object, 8, 1))
                continue;
            VuVec position;
            member.reference->GetAttributeData(member.object, 8, EdType_VuVec, &position, 0);
            f32 dx = position.x - point.x;
            f32 dy = position.y - point.y;
            f32 dz = position.z - point.z;
            f32 distance_squared = dx * dx + dy * dy + dz * dz;
            // The original keeps the radius in a 16-byte aligned slot.
            f32 radius __attribute__((aligned(16))) = 1.0f;
            if (ed_class->FindMember(&member, object, 0x40, 1))
                member.reference->GetAttributeData(member.object, 0x40, EdType_Float, &radius, 0);
            if (distance_squared < radius * radius && candidate_count < 16) {
                candidate_classes[candidate_count] = ed_class;
                candidate_objects[candidate_count] = object;
                ++candidate_count;
            }
        }
    }
    if (candidate_count == 0)
        return 0;
    i32 choice = 0;
    if (after.object != NULL) {
        for (i32 index = 0; index < candidate_count; ++index) {
            if (candidate_objects[index] == after.object) {
                choice = (index + 1) % candidate_count;
                break;
            }
        }
    }
    result.ed_class = candidate_classes[choice];
    result.object = candidate_objects[choice];
    return 1;
}

i32 ClassEditor::FindNearestObject(VuVec &point, ClassObject &result, i32 filter) {
    EdClass *nearest_class = NULL;
    void *nearest_object = NULL;
    f32 nearest_distance = FLT_MAX;
    for (i32 class_index = 0; class_index < theRegistry.class_count; ++class_index) {
        EdClass *ed_class = &theRegistry.classes[class_index];
        if (!Editable(NULL, ed_class, class_index) || (ed_class->flags & 8) == 0)
            continue;
        EdClassInterface *interface = ed_class->interface;
        for (void *object = interface->vtable->get_next_object(interface, NULL); object != NULL;
             object = ed_class->interface->vtable->get_next_object(ed_class->interface, object)) {
            // The original keeps the lookup and position in 16-byte aligned slots.
            EdMember member __attribute__((aligned(16)));
            if (!ed_class->FindMember(&member, object, 8, 1))
                continue;
            VuVec position __attribute__((aligned(16)));
            member.reference->GetAttributeData(member.object, 8, EdType_VuVec, &position, 0);
            f32 dx = position.x - point.x;
            f32 dy = position.y - point.y;
            f32 dz = position.z - point.z;
            f32 distance = dx * dx + dy * dy + dz * dz;
            if (distance < nearest_distance) {
                nearest_class = ed_class;
                nearest_object = object;
                nearest_distance = distance;
            }
        }
    }
    if (nearest_object != NULL && filter != 0) {
        f32 nearest_radius = NuFsqrt(nearest_distance);
        // The original keeps the radius and lookup in 16-byte aligned slots.
        f32 radius __attribute__((aligned(16))) = 1.0f;
        if ((nearest_class->flags & 0x40) != 0) {
            EdMember member __attribute__((aligned(16)));
            if (nearest_class->FindMember(&member, nearest_object, 0x40, 1))
                member.reference->GetAttributeData(member.object, 0x40, EdType_Float, &radius, 0);
        }
        if (nearest_radius > radius) {
            nearest_class = NULL;
            nearest_object = NULL;
        }
    }
    result.object = nearest_object;
    result.ed_class = nearest_class;
    return nearest_object != NULL;
}

void ClassEditor::cbEdClassSelectObjectMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    EdClass *ed_class = theRegistry.GetClass(static_cast<i32>(item->data));
    theClassEditor.pending_object.ed_class = ed_class;
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (menu == NULL)
        return;
    EdRef *name_ref = ed_class->FindTypeRef(2, 1);
    for (void *object = ed_class->interface->vtable->get_next_object(ed_class->interface, NULL); object != NULL;
         object = ed_class->interface->vtable->get_next_object(ed_class->interface, object)) {
        char name[128];
        if (name_ref == NULL || !name_ref->GetAttributeData(object, 2, EdType_String, name, sizeof(name)))
            NuStrCpy(name, ed_class->name);
        if (theClassEditor.Editable(object, ed_class, -1))
            eduiMenuAddItem(menu, eduiItemSelCreate(reinterpret_cast<usize>(object), &EdLevelAttr, 0, 0,
                                                    cbEdClassSelectObject, name));
    }
    if (menu->first == NULL) {
        eduiMenuAddItem(
            menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect, const_cast<char *>("No Object")));
    } else {
        eduiMenuAddItemFirst(
            menu, eduiItemFilterCreate(0, &EdLevelAttr, const_cast<char *>("FILTER"), const_cast<char *>("")));
    }
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
    eduiMenuSortItemsByTxt(menu);
}

i32 CursorTool::Process(EdInputContext &input) {
    VuVec &origin = *reinterpret_cast<VuVec *>(input.reserved_00 + 0x20);
    VuVec &direction = *reinterpret_cast<VuVec *>(input.reserved_00 + 0x30);
    ClassObject hover __attribute__((aligned(16))) = {NULL, NULL, NULL};
    const f32 extend_selection = input.GetHold(16);
    VuVec end __attribute__((aligned(16)));
    end.x = origin.x + direction.x;
    end.y = origin.y + direction.y;
    end.z = origin.z + direction.z;
    const i32 colour = NuRandInt();
    EdDrawBegin(0);
    EdDrawLineSegment(origin, end, colour);
    EdDrawEnd();
    if (theLevelEditor.field_0x30 == 0)
        return 0;

    ClassObject after __attribute__((aligned(16))) = {NULL, NULL, NULL};
    if (theClassEditor.selected_objects.count > 0) {
        ClassObjectListEntry *first = theClassEditor.selected_objects.first;
        after = *reinterpret_cast<ClassObject *>(&first->ed_class);
    }
    if (theClassEditor.FindNearestObject(origin, direction, hover, 1)) {
        theClassEditor.current_object = hover;
        if (input.GetPress(3) != 0.0f) {
            theClassEditor.FindNearestObject(origin, direction, hover, after, 1);
            theClassEditor.SelectObject(hover, static_cast<i32>(extend_selection) != 0);
        }
        char text[128] __attribute__((aligned(16)));
        char name[128] __attribute__((aligned(16)));
        if (!get_class_object_attribute(hover.ed_class, hover.object, hover.reference, 2, EdType_String, name,
                                        sizeof(name))) {
            theLevelEditor.AddInfoText(const_cast<char *>("object : no name"));
        } else {
            sprintf(text, "object : %s", name);
            theLevelEditor.AddInfoText(text);
        }
    } else {
        hover.ed_class = NULL;
        hover.object = NULL;
        theClassEditor.current_object = hover;
        if (input.GetPress(11) != 0.0f) {
            if (after.object != NULL)
                theClassEditor.CreateObject(after);
            else
                theClassEditor.CreateObject();
        }
    }
    if (input.GetPress(12) != 0.0f)
        theClassEditor.DestroySelectedObjects();
    if (hover.object == NULL && input.GetPress(3) != 0.0f) {
        hover.ed_class = NULL;
        hover.object = NULL;
        theClassEditor.SelectObject(hover, 0);
    }
    return 0;
}

__attribute__((force_align_arg_pointer)) void ClassObject::GetName(char *destination, i32 size) {
    if (object == NULL) {
        NuStrNCpy(destination, "None", size);
        return;
    }

    char name[128];
    EdMember member;
    i32 string_type = EdType_String;
    if ((reference == NULL || !reference->GetAttributeData(object, 2, string_type, name, sizeof(name))) &&
        (!ed_class->FindMember(&member, object, 2, 1) ||
         !member.reference->GetAttributeData(member.object, 2, string_type, name, sizeof(name)))) {
        NuStrCpy(name, "NoName");
    }
    sprintf(destination, "%s.%s", ed_class->name, name);
}

void EdClassObjectNameControl::Process(EdInputContext &input) {
    if (input.GetHold(0x16) != 0.0f) {
        ClassObject selected = theClassEditor.current_object;
        theClassEditor.field_3c = static_cast<i32>(0xff008000);
        if (input.GetPress(0x19) != 0.0f) {
            char name[128];
            selected.GetName(name, sizeof(name));
            SetVal(name);
        }
    }
}

void EdClassObjectNameControl::cbSelectObject(eduimenu_s *menu, eduiitem_s *item, u32) {
    EdClassObjectNameControl *control = edClassObjectNameControl;
    if (control != NULL) {
        control->selected.object = item->data_ptr;
        char name[128];
        const char *value;
        if (__builtin_expect(control->selected.object != NULL, 1)) {
            control->selected.GetName(name, sizeof(name));
            value = name;
        } else {
            value = "None";
        }
        eduiItemPropSetText(static_cast<edui_prop_s *>(edClassObjectNameControl->item), const_cast<char *>(value));
        edClassObjectNameControl->SetVal(value);
    }
    edLevelDestroyThisMenu = menu;
    edLevelDestroyThisMenu2 = menu->parent;
}

void EdClassObjectNameControl::cbChanged(eduimenu_s *, eduiitem_s *item, u32) {
    EdClassObjectNameControl *control = static_cast<EdClassObjectNameControl *>(item->data_ptr);
    ClassObject selected{};
    selected.Set(static_cast<edui_prop_s *>(item)->property_text);
    char name[128];
    selected.GetName(name, sizeof(name));
    eduiItemPropSetText(static_cast<edui_prop_s *>(control->item), name);
    control->SetVal(name);
}

void PropertyTool::RenderMenu(PropertyMenu *property_menu) {
    eduimenu_s *menu = property_menu->menu;
    if (property_menu->order == -1)
        AutoLocateMenu(property_menu);
    VuVec start __attribute__((aligned(16)));
    VuVec end __attribute__((aligned(16)));
    NuCameraCalcRay((static_cast<f32>(menu->x) + static_cast<f32>(menu->width) * 0.5f) / 640.0f,
                    (static_cast<f32>(menu->y) + static_cast<f32>(menu->height) * 0.125f) / 448.0f, &start.xyz,
                    &end.xyz, NULL);
    ClassObject &object = property_menu->objects[0];
    if (object.reference == NULL || !object.reference->GetAttributeData(object.object, 8, EdType_VuVec, &end, 0)) {
        EdMember member;
        if (object.ed_class->FindMember(&member, object.object, 8, 1))
            member.reference->GetAttributeData(member.object, 8, EdType_VuVec, &end, 0);
    }
    EdDrawBegin(0);
    EdDrawLineSegment(start, end, static_cast<i32>(reinterpret_cast<usize>(&selected_attr)));
    EdDrawEnd();
    if (property_menu->menu != NULL)
        eduiMenuRender(menu);
    if (menu->selected != NULL) {
        EdControl *control = static_cast<EdControl *>(menu->selected->data_ptr);
        if (control != NULL)
            control->Render();
    }
}

void PropertyTool::Render() {
    PropertyMenu *selected = NULL;
    for (PropertyMenu *menu = last_menu; menu != NULL; menu = menu->previous) {
        if (menu == GetActiveMenu(active_menu)) {
            selected = menu;
        } else {
            RenderMenu(menu);
        }
    }
    if (selected != NULL) {
        RenderMenu(selected);
    }
}

void CursorTool::Render() {
    thePropertyTool.Render();
}

PropertyMenu *PropertyTool::CreatePropertyMenu(ClassObject &object) {
    PropertyMenu *property_menu = new (theMemoryManager.AllocPool(sizeof(PropertyMenu), 1)) PropertyMenu();
    PropertyMenuMetrics metrics __attribute__((aligned(16))) = ediGetMenuStartMetrics();
    char name[64];
    char title[128];

    i32 name_type = EdType_String;
    if (__builtin_expect(object.reference == NULL ||
        !object.reference->GetAttributeData(object.object, 2, name_type, name, sizeof(name)), 0)) {
        EdMember member;
        if (!object.ed_class->FindMember(&member, object.object, 2, 1) ||
            !member.reference->GetAttributeData(member.object, 2, name_type, name, sizeof(name)))
            NuStrCpy(name, const_cast<char *>("no name"));
    }
    sprintf(title, "%s - %s", object.ed_class->name, name);
    eduimenu_s *menu = eduiMenuCreate(metrics.x, metrics.y, metrics.width, metrics.height,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), NULL, title);
    if (menu != NULL) {
        AddPropertyMenuItems(menu, object.ed_class, object.object, NULL);
        menu->selected = menu->first;
        eduiMenuFitWidth(menu, 5);
    }
    eduiMenuSetAttr(menu, &selected_attr.background);
    property_menu->menu = menu;
    property_menu->objects[0] = object;
    property_menu->object_count = 1;
    property_menu->control = NULL;
    property_menu->SelectAttr(theClassEditor.manipulator != NULL ? theClassEditor.manipulator->selected_attribute : 0);
    return property_menu;
}

PropertyMenu *PropertyTool::RetrievePropertyMenu(ClassObject *object, PropertyMenuList *) {
    return CreatePropertyMenu(*object);
}

i32 PropertyTool::ProcessMenu(EdInputContext &input) {
    PropertyMenuList rebuilt = {};
    i32 order = 0;
    for (PropertyMenu *menu = active_menu; menu != NULL; menu = menu->next) {
        menu->order = order++;
    }
    for (ClassObjectListEntry *entry = theClassEditor.selected_objects.first; entry != NULL; entry = entry->next) {
        PropertyMenu *menu = FindItemMenu(active_menu, reinterpret_cast<ClassItem *>(entry));
        if (menu != NULL) {
            menu->ClearObjecs();
            menu->AddObject(*reinterpret_cast<ClassObject *>(&entry->ed_class));
            if (menu->next != NULL) {
                menu->next->previous = menu->previous;
            } else {
                last_menu = menu->previous;
            }
            if (menu->previous != NULL) {
                menu->previous->next = menu->next;
            } else {
                active_menu = menu->next;
            }
            menu->next = NULL;
            menu->previous = NULL;
            --menu_count;
        } else {
            menu = FindItemMenu(rebuilt.first, reinterpret_cast<ClassItem *>(entry));
            if (menu != NULL) {
                menu->AddObject(*reinterpret_cast<ClassObject *>(&entry->ed_class));
                continue;
            }
            for (PropertyMenu *other = active_menu; other != NULL; other = other->next) {
                ediMenuStoreMetrics(other->menu);
            }
            menu = RetrievePropertyMenu(reinterpret_cast<ClassObject *>(&entry->ed_class),
                                        reinterpret_cast<PropertyMenuList *>(&active_menu));
            ediMenuRetrieveMetrics(menu->menu);
            menu->order = -1;
        }

        PropertyMenu *position = rebuilt.first;
        while (position != NULL && position->order <= menu->order) {
            position = position->next;
        }
        if (position != NULL) {
            menu->next = position;
            menu->previous = position->previous;
            if (position->previous != NULL) {
                position->previous->next = menu;
            } else {
                rebuilt.first = menu;
            }
            position->previous = menu;
            ++rebuilt.count;
            continue;
        }
        menu->next = NULL;
        menu->previous = rebuilt.last;
        if (rebuilt.last != NULL) {
            rebuilt.last->next = menu;
        } else {
            rebuilt.first = menu;
        }
        rebuilt.last = menu;
        ++rebuilt.count;
    }
    while (active_menu != NULL) {
        PropertyMenu *menu = active_menu;
        PropertyMenu *next = menu->next;
        if (next != NULL) {
            next->previous = menu->previous;
        } else {
            last_menu = menu->previous;
        }
        if (menu->previous != NULL) {
            menu->previous->next = next;
        } else {
            active_menu = next;
        }
        menu->next = NULL;
        menu->previous = NULL;
        --menu_count;
        menu->Destroy();
        theMemoryManager.FreePool(menu, sizeof(PropertyMenu));
    }
    active_menu = rebuilt.first;
    last_menu = rebuilt.last;
    menu_count = rebuilt.count;
    if (edLevelActiveMenu != NULL) {
        return 0;
    }
    EdControl::Input = &input;
    PropertyMenu *active = GetActiveMenu(active_menu);
    if (active != NULL) {
        RefreshMenuControls(active);
        if (eduiMenuProcess(active->menu, input.delta_time, input.pad) != 0) {
            return 1;
        }
    }
    for (PropertyMenu *menu = active_menu; menu != NULL;) {
        PropertyMenu *next = menu->next;
        if (menu != active) {
            RefreshMenuControls(menu);
            if (eduiMenuProcess(menu->menu, input.delta_time, input.pad) != 0) {
                return 1;
            }
        }
        menu = next;
    }
    if ((input.pad->digital_buttons_pressed & 0x100) != 0) {
        ToggleActiveMenu();
    }
    EdControl::Input = NULL;
    return 0;
}

i32 PropertyTool::Process(EdInputContext &input) {
    i32 result = ProcessMenu(input);
    ProcessControls(input);
    return result;
}

void ClassEditor::Process(EdInputContext &input) {
    EdMember member;
    UpdateSnapRay(*reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(&input) + 0x30));
    UpdateSelectedObjects(input);
    UpdateClassFilter(input);
    if (menu != NULL) {
        theLevelEditor.field_0x30 = 0;
        theLevelEditor.field_0x38 = 0;
        NuFntSet(EdLevelFnt);
        NuFntScale(EdLevelFntScale, EdLevelFntScale);
        eduiMenuProcess(menu, *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(&input) + 0x44),
                        *reinterpret_cast<nupad_s **>(reinterpret_cast<u8 *>(&input) + 0x40));
        if (menu != NULL)
            return;
    }
    if (input.GetHold(13) != 0.0f)
        return;
    field_3c = 0xff808080;
    theLevelEditor.field_0x30 = 1;
    theLevelEditor.field_0x38 = 1;
    if (thePropertyTool.Process(input) == 0) {
        if (input.GetPress(5) != 0.0f)
            SetMode(0);
        if (input.GetPress(6) != 0.0f)
            SetMode(3);
        if (input.GetPress(7) != 0.0f)
            SetMode(4);
        if (input.GetPress(8) != 0.0f)
            SetMode(5);
        if (input.GetPress(37) != 0.0f) {
            ClassObject nearest = {};
            VuVec &origin = *reinterpret_cast<VuVec *>(input.reserved_00 + 0x20);
            VuVec &direction = *reinterpret_cast<VuVec *>(input.reserved_00 + 0x30);
            if (FindNearestObject(origin, direction, nearest, 1) == 0)
                cbEdCopySelectedObject(input);
            else
                DestroySelectedObjects();
        }
        if (manipulator == NULL)
            manipulator = &theDefaultManipulator;
        if (manipulator->Process(input, selected_objects) == 0 && active_tool != NULL)
            active_tool->Process(input);
    }
    if (!(selected_objects.count > 0)) {
        return;
    }
    if (input.GetPress(9) != 0.0f)
        FocusSelected();
    if (input.GetPress(10) != 0.0f)
        ViewSelected();
    for (ClassObjectListEntry *entry = selected_objects.first; entry != NULL; entry = entry->next)
        theRegistry.ClassIFaceProcess(entry->ed_class, entry->object, input);
    if (input.GetPress(39) != 0.0f) {
        ClassObjectListEntry *entry = selected_objects.first;

        VuVec position __attribute__((aligned(16)));
        i32 position_type = EdType_VuVec;
        if (entry->reference == NULL ||
            !entry->reference->GetAttributeData(entry->object, 8, position_type, &position, 0)) {
            if (entry->ed_class->FindMember(&member, entry->object, 8, 1))
                member.reference->GetAttributeData(member.object, 8, position_type, &position, 0);
        }
        theLevelEditor.background_colour[0] = position.x;
        theLevelEditor.background_colour[1] = position.y;
        theLevelEditor.background_colour[2] = position.z;
        theLevelEditor.background_colour[3] = position.w;
        edcamSetPos(reinterpret_cast<NUVEC *>(theLevelEditor.background_colour));
        thePropertyTool.Process(input);
        if (mode == 0)
            mode = 3;
        SetMode(mode);
        thePropertyTool.SetDefaultActiveMenu(NULL);
    }
}

// The original realigns the incoming stack for its matrix and vector locals.
__attribute__((force_align_arg_pointer)) i32 EdManRotate::RotateItem(EdInputContext &, ClassObjectList &objects,
                                                                     i32 angle, i32 axis) {
    ClassObjectListEntry *first = objects.first;
    VuVec average;
    objects.GetAveragePosition(average);
    if (angle == 0 || first == NULL)
        return axis;
    i32 cosine_index = ((angle + 0x4000) >> 1) & 0x7fff;
    i32 sine_index = (angle >> 1) & 0x7fff;
    for (ClassObjectListEntry *entry = first; entry != NULL; entry = entry->next) {
        NUMTX matrix;
        NuMtxSetIdentity(&matrix);
        i32 matrix_type = EdType_VuMtx;
        if (entry->reference == NULL ||
            entry->reference->GetAttributeData(entry->object, 0x10, matrix_type, &matrix, 0) == 0) {
            EdMember member;
            if (__builtin_expect(entry->ed_class->FindMember(&member, entry->object, 0x10, 1) == 0 ||
                member.reference->GetAttributeData(member.object, 0x10, matrix_type, &matrix, 0) == 0, 1))
                continue;
        }
        switch (axis) {
            case 1: {
                average = VuVec(matrix.m30, matrix.m31, matrix.m32, matrix.m33);
                f32 cosine = NuTrigTable[cosine_index];
                f32 sine = NuTrigTable[sine_index];
                f32 m01 = matrix.m01, m11 = matrix.m11, m21 = matrix.m21;
                matrix.m01 = cosine * m01 - sine * matrix.m02;
                matrix.m02 = m01 * sine + matrix.m02 * cosine;
                matrix.m11 = cosine * m11 - sine * matrix.m12;
                matrix.m12 = m11 * sine + matrix.m12 * cosine;
                matrix.m21 = cosine * m21 - sine * matrix.m22;
                matrix.m22 = m21 * sine + matrix.m22 * cosine;
                break;
            }
            case 2: {
                average = VuVec(matrix.m30, matrix.m31, matrix.m32, matrix.m33);
                f32 sine = NuTrigTable[sine_index];
                f32 cosine = NuTrigTable[cosine_index];
                f32 m00 = matrix.m00, m10 = matrix.m10, m20 = matrix.m20;
                matrix.m00 = cosine * m00 + sine * matrix.m02;
                matrix.m02 = matrix.m02 * cosine - m00 * sine;
                matrix.m10 = cosine * m10 + sine * matrix.m12;
                matrix.m12 = matrix.m12 * cosine - m10 * sine;
                matrix.m20 = cosine * m20 + sine * matrix.m22;
                matrix.m22 = matrix.m22 * cosine - m20 * sine;
                break;
            }
            case 3: {
                average = VuVec(matrix.m30, matrix.m31, matrix.m32, matrix.m33);
                f32 sine = NuTrigTable[sine_index];
                f32 cosine = NuTrigTable[cosine_index];
                f32 m00 = matrix.m00, m10 = matrix.m10, m20 = matrix.m20;
                matrix.m00 = cosine * m00 - sine * matrix.m01;
                matrix.m01 = m00 * sine + matrix.m01 * cosine;
                matrix.m10 = cosine * m10 - sine * matrix.m11;
                matrix.m11 = m10 * sine + matrix.m11 * cosine;
                matrix.m20 = cosine * m20 - sine * matrix.m21;
                matrix.m21 = m20 * sine + matrix.m21 * cosine;
                break;
            }
            default:
                continue;
        }
        matrix_type = EdType_VuMtx;
        if (entry->reference == NULL ||
            entry->reference->SetAttributeData(entry->object, 0x10, matrix_type, &matrix, 0) == 0) {
            EdMember member;
            if (entry->ed_class->FindMember(&member, entry->object, 0x10, 1) != 0)
                member.reference->SetAttributeData(member.object, 0x10, matrix_type, &matrix, 0);
        }
    }
    return axis;
}

void ClassEditor::DrawObjectSphere(ClassObject &selected, i32 colour) {
    if (__builtin_expect(selected.object == NULL, 1))
        return;
    f32 radius;
    EdMember member;
    i32 radius_type = EdType_Float;
    if (selected.reference == NULL ||
        selected.reference->GetAttributeData(selected.object, 0x40, radius_type, &radius, 0) == 0) {
        if (selected.ed_class->FindMember(&member, selected.object, 0x40, 1) == 0 ||
            member.reference->GetAttributeData(member.object, 0x40, radius_type, &radius, 0) == 0) {
            radius = 1.0f;
        }
    }
    // The original places this matrix on a 16-byte-aligned stack slot.
    VuMtx transform __attribute__((aligned(16)));
    i32 matrix_type = EdType_VuMtx;
    if ((selected.reference == NULL ||
         selected.reference->GetAttributeData(selected.object, 0x10, matrix_type, &transform, 0) == 0) &&
        (selected.ed_class->FindMember(&member, selected.object, 0x10, 1) == 0 ||
         member.reference->GetAttributeData(member.object, 0x10, matrix_type, &transform, 0) == 0)) {
        VuVec position;
        i32 position_type = EdType_VuVec;
        if ((selected.reference != NULL &&
             selected.reference->GetAttributeData(selected.object, 8, position_type, &position, 0) != 0) ||
            (selected.ed_class->FindMember(&member, selected.object, 8, 1) != 0 &&
             member.reference->GetAttributeData(member.object, 8, position_type, &position, 0) != 0)) {
            EdDrawBegin(0);
            EdDrawLineSphere(position, radius, 1.0f, colour);
            EdDrawEnd();
        }
        return;
    }

    VuVec const &position = *reinterpret_cast<VuVec const *>(&transform.matrix.m30);
    EdDrawBegin(0);
    EdDrawLineSphere(position, radius, 1.0f, colour);
    VuVec tip(transform.matrix.m30 - transform.matrix.m20 * radius,
              transform.matrix.m31 - transform.matrix.m21 * radius,
              transform.matrix.m32 - transform.matrix.m22 * radius, 0.0f);
    EdDrawPolyArrow(position, tip, 4, colour, 0.1f, 0.1f, 0.1f, 0.1f);
    EdDrawEnd();
}

void EdManipulator::Render(ClassObjectList &selected) {
    for (ClassObjectListEntry *entry = selected.first; entry != NULL; entry = entry->next) {
        ClassObject &object = *reinterpret_cast<ClassObject *>(&entry->ed_class);
        theClassEditor.DrawObjectSphere(object, static_cast<i32>(0xff800000));
    }
}

__attribute__((force_align_arg_pointer)) void EdManScale::Render(ClassObjectList &selected) {
    if (selected.count > 2)
        EdManipulator::Render(selected);
    VuVec average;
    if (selected.GetAveragePosition(average) != 0) {
        VuMtx transform;
        ClassObjectListEntry *entry = selected.first;
        const i32 type = EdType_VuMtx;
        if (entry->reference == NULL ||
            entry->reference->GetAttributeData(entry->object, 0x20, type, &transform, 0) == 0) {
            EdMember member;
            if (entry->ed_class->FindMember(&member, entry->object, 0x20, 1) != 0)
                member.reference->GetAttributeData(member.object, 0x20, type, &transform, 0);
        }
        DrawAxis(average, &transform);
    }
}

void EdManRotate::Render(ClassObjectList &selected) {
    if (selected.count > 2)
        EdManipulator::Render(selected);
    VuVec average;
    if (selected.GetAveragePosition(average) != 0)
        DrawRotator(average);
}

void EdManMove::Render(ClassObjectList &selected) {
    if (selected.count > 2)
        EdManipulator::Render(selected);
    VuVec average;
    if (selected.GetAveragePosition(average) != 0)
        DrawAxis(average, NULL);
}

void ClassEditor::Render() {
    thePropertyTool.Render();
    if (active_tool != NULL)
        active_tool->Render();
    if (current_object.object != NULL && !IsSelectedObject(current_object))
        DrawObjectSphere(current_object, field_3c);
    if (selected_objects.count > 0) {
        if (manipulator != NULL) {
            manipulator->Render(selected_objects);
        } else {
            for (ClassObjectListEntry *entry = selected_objects.first; entry != NULL; entry = entry->next) {
                DrawObjectSphere(*reinterpret_cast<ClassObject *>(&entry->ed_class), 0xff800000);
            }
        }
        for (ClassObjectListEntry *entry = selected_objects.first; entry != NULL; entry = entry->next)
            theRegistry.ClassIFaceRender(entry->ed_class, entry->object, 1);
    }

    for (i32 index = 0; index < theRegistry.class_count; ++index) {
        EdClass *ed_class = theRegistry.GetClass(index);
        if (!Editable(NULL, ed_class, index) || ed_class->interface == NULL || (ed_class->flags & 0x08000080) == 0)
            continue;

        for (void *object = ed_class->interface->vtable->get_next_object(ed_class->interface, NULL); object != NULL;
             object = ed_class->interface->vtable->get_next_object(ed_class->interface, object)) {
            if (!Editable(object, ed_class, -1))
                continue;

            ClassObject selected = {ed_class, object, NULL};
            if ((ed_class->flags & 0x08000000) != 0 && selected_objects.IsInList(selected) == 0)
                theRegistry.ClassIFaceRender(ed_class, object, 0);
            if ((ed_class->flags & 0x80) != 0 && selected_objects.IsInList(selected) != 0 &&
                object != current_object.object)
                DrawObjectSphere(selected, 0xff000080);
        }
    }

    if (menu != NULL)
        eduiMenuRender(menu);
}

void SceneObjectHelper::SubRender() {
    for (SceneInstance *object = owned_first; object != NULL; object = object->next) {
        if (object->reserved_0x28 != 0)
            continue;
        if (object->visibility == 0 &&
            (__builtin_expect(show_hidden_solid == 0, 0) || theLevelEditor.editors_entered == 0)) {
            if (__builtin_expect(show_hidden_wire != 0, 0) && theLevelEditor.editors_entered != 0) {
                ClassObject selection{class_interface.object_class, object, NULL};
                theClassEditor.DrawObjectSphere(selection, 0x80808080);
            }
            continue;
        }
        EdDrawBegin(0);
        object->Render(NULL);
        EdDrawEnd();
    }
}

void sceneHelperSubRender(EdSubSystem *object) {
    SceneObjectHelper *helper = reinterpret_cast<SceneObjectHelper *>(reinterpret_cast<u8 *>(object) - 12);
    helper->SubRender();
}

void *EdClass::FindObject(char *object_name) {
    void *object = interface->vtable->get_next_object(interface, NULL);
    while (object != NULL) {
        EdMember member;
        i32 string_type = EdType_String;
        if (FindMember(&member, object, 2, 1) != 0) {
            // The original aligns the stack for this name buffer.
            char name_buffer[256] __attribute__((aligned(16)));
            if (member.reference->GetAttributeData(member.object, 2, string_type, name_buffer, sizeof(name_buffer)) !=
                    0 &&
                NuStrICmp(object_name, name_buffer) == 0) {
                return object;
            }
        }
        object = interface->vtable->get_next_object(interface, object);
    }
    return NULL;
}

void EdRegistry::SerialiseObjects(EdStream &stream, EdRegistry *source_registry) {
    char object_name[256];

    i32 filter_flags;
    i16 group;
    auto get_attribute = [&](EdClass *object_class, void *object, i32 attribute, i32 type, void *data, i32 size) {
        EdMember member;
        if (object_class->FindMember(&member, object, attribute, 1))
            member.reference->GetAttributeData(member.object, attribute, type, data, size);
    };
    auto read_objects = [&](EdClass *object_class, EdClass *source_class) {
        i32 count;
        stream.SerialiseBuffer(&count, sizeof(count), 1);
        for (i32 index = 0; index < count; ++index) {
            i32 object_flags;
            stream.SerialiseBuffer(&object_flags, sizeof(object_flags), 1);
            void *object;
            if (object_flags & 0x02000000) {
                stream.SerialiseString(object_name, sizeof(object_name));
                object = object_class->FindObject(object_name);
            } else if (object_class->flags & 0x04000000) {
                stream.SerialiseString(object_name, sizeof(object_name));
                void *original = object_class->FindObject(object_name);
                if (original == NULL) {
                    object_class->SerialiseObject(stream, NULL, source_class, source_registry);
                    continue;
                }
                object = theRegistry.CreateObject(object_class->interface, original, 4, 0, 2);
            } else {
                object = theRegistry.CreateObject(object_class->interface, NULL, 4, 0, 2);
            }
            object_class->SerialiseObject(stream, object, source_class, source_registry);
            if (object != NULL)
                theRegistry.NotifyCreateObject(object, object_class, NULL, 0, 0, 0);
        }
    };

    if (stream.mode == 2) {
        i32 count = 0;
        for (i32 index = 0; index < class_count; ++index) {
            EdClass *object_class = &classes[index];
            if (object_class->interface == NULL)
                continue;
            if (stream.flags & 0x400000) {
                if (object_class->flags & 0x400000)
                    continue;
            } else if (object_class->flags & 0x10000000) {
                continue;
            }
            ++count;
        }
        stream.SerialiseBuffer(&count, sizeof(count), 1);
        for (i32 index = 0; index < class_count; ++index) {
            EdClass *object_class = &classes[index];
            if (object_class->interface == NULL)
                continue;
            if (stream.flags & 0x400000) {
                if (object_class->flags & 0x400000)
                    continue;
            } else if (object_class->flags & 0x10000000) {
                continue;
            }
            stream.BeginBlock("ObjectList");
            stream.SerialiseString(&object_class->name);
            i32 object_count = 0;
            for (void *object = object_class->interface->vtable->get_next_object(object_class->interface, NULL);
                 object != NULL;
                 object = object_class->interface->vtable->get_next_object(object_class->interface, object)) {
                group = 0;
                filter_flags = 0;
                get_attribute(object_class, object, 1, EdType_Int, &filter_flags, 0);
                get_attribute(object_class, object, 0x100, EdType_Short, &group, 0);
                if (stream.flags & 0x400000) {
                    if (filter_flags & 0x400000)
                        continue;
                } else if (filter_flags & 0x10000000) {
                    continue;
                }
                if (group != stream.unknown_10)
                    continue;
                ++object_count;
            }
            stream.SerialiseBuffer(&object_count, sizeof(object_count), 1);
            for (void *object = object_class->interface->vtable->get_next_object(object_class->interface, NULL);
                 object != NULL;
                 object = object_class->interface->vtable->get_next_object(object_class->interface, object)) {
                group = 0;
                filter_flags = 0;
                get_attribute(object_class, object, 1, EdType_Int, &filter_flags, 0);
                get_attribute(object_class, object, 0x100, EdType_Short, &group, 0);
                if (stream.flags & 0x400000) {
                    if (filter_flags & 0x400000)
                        continue;
                } else if (filter_flags & 0x10000000) {
                    continue;
                }
                if (group != stream.unknown_10)
                    continue;
                i32 object_flags = 0;
                get_attribute(object_class, object, 1, EdType_Int, &object_flags, 0);
                stream.SerialiseBuffer(&object_flags, sizeof(object_flags), 1);
                if ((object_flags & 0x02000000) || (object_class->flags & 0x04000000)) {
                    get_attribute(object_class, object, 4, EdType_String, object_name, sizeof(object_name));
                    stream.SerialiseString(object_name, sizeof(object_name));
                }
                object_class->SerialiseObject(stream, object);
            }
            stream.EndBlock();
        }
    }

    if (stream.mode == 1) {
        if (!(stream.version <= 3)) {
            i32 count;
            stream.SerialiseBuffer(&count, sizeof(count), 1);
            for (i32 index = 0; index < count; ++index) {
                if (stream.BeginBlock("ObjectList") != NULL) {
                    char class_name[128];
                    stream.SerialiseString(class_name, sizeof(class_name));
                    EdClass *source_class = source_registry->GetClass(class_name);
                    EdClass *object_class = GetClass(MapName(source_class->name));
                    if (object_class != NULL && object_class->interface != NULL)
                        read_objects(object_class, source_class);
                }
                stream.EndBlock();
            }
        } else {
            for (i32 index = 0; index < source_registry->class_count; ++index) {
                EdClass *source_class = source_registry->GetClass(index);
                EdClass *object_class = GetClass(MapName(source_class->name));
                if (object_class != NULL && object_class->interface != NULL) {
                    if (stream.BeginBlock("ObjectList") != NULL)
                        read_objects(object_class, source_class);
                    stream.EndBlock();
                }
            }
        }
    }
}

EditorSettings::EditorSettings() {
    cursor_radius = 1.0f;
    snap_terrain = 1;
}

LevelEditor::LevelEditor() {
    first_editor = NULL;
    last_editor = NULL;
    editor_count = 0;
    background_colour[3] = 1.0f;
    overlay_alpha = 0.6f;
    active = 0;
    background_colour[0] = 0.0f;
    reset_pending = 0;
    background_colour[1] = 0.0f;
    field_0x20 = 0;
    background_colour[2] = 0.0f;
    field_0x28 = 1;
    field_0x24 = 0;
    field_0x30 = 1;
    field_0x38 = 1;
    info_x = 0;
    info_y = 0;
    info_width = 640;
    info_height = 448;
    info_colour = 0x80808080;
    info_background = 0x40000000;
    pad_x = 0;
    pad_y = 0;
    pad_width = 640;
    pad_height = 448;
    pad_background = 0x80000000;
    pad_colour = 0x80808080;
    SetPadText(0x1000, const_cast<char *>("D-Up"));
    SetPadText(0x4000, const_cast<char *>("D-Down"));
    SetPadText(0x8000, const_cast<char *>("D-Left"));
    SetPadText(0x2000, const_cast<char *>("D-Right"));
    SetPadText(0x10, const_cast<char *>("Triangle"));
    SetPadText(0x40, const_cast<char *>("Cross"));
    SetPadText(0x80, const_cast<char *>("Square"));
    SetPadText(0x20, const_cast<char *>("Circle"));
    SetPadText(4, const_cast<char *>("L1"));
    SetPadText(1, const_cast<char *>("L2"));
    SetPadText(8, const_cast<char *>("R1"));
    SetPadText(2, const_cast<char *>("R2"));
    SetPadText(0x200, const_cast<char *>("L3"));
    SetPadText(0x400, const_cast<char *>("R1"));
    SetPadText(0x800, const_cast<char *>("START"));
    SetPadText(0x100, const_cast<char *>("SELECT"));
}

void EditorSettings::AddMenuItems(eduimenu_s *menu) {
    eduiMenuAddItem(menu, eduiItemSliderCreate(reinterpret_cast<usize>(&cursor_radius), &EdLevelAttr, 0,
                                               cbEdLevelSetSliderFloat, 1.0f, 50.0f, cursor_radius,
                                               const_cast<char *>("Cursor Radius")));
    eduiMenuAddItem(menu, eduiItemCheckCreate(reinterpret_cast<usize>(&snap_terrain), &EdLevelAttr, snap_terrain, 0,
                                              cbEdLevelToggleInt, const_cast<char *>("Snap Terrain")));
    edui_last_item->highlighted = snap_terrain & 1;
}

static void cbEdLevelSettingsMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy,
                                      const_cast<char *>("Options"));
    if (menu == NULL)
        return;
    eduiMenuAddItem(menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdLevelSave, const_cast<char *>("Save Data")));
    theLevelEditor.settings.AddMenuItems(menu);
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
}

void EditorSettings::Serialise(EdStream &stream) {
    stream.SerialiseBuffer(&cursor_radius, sizeof(cursor_radius), 1);
    stream.SerialiseBuffer(&snap_terrain, sizeof(snap_terrain), 1);
}

i32 LevelEditor::WriteStream(EdFileOutputStream &stream) {
    ++file_version;
    stream.BeginBlock("FileInfo");
    stream.SerialiseBuffer(&file_version, sizeof(file_version), 1);
    stream.EndBlock();
    stream.BeginBlock("Settings");
    settings.Serialise(stream);
    stream.EndBlock();
    stream.BeginBlock("Editors");
    i32 count = editor_count;
    stream.SerialiseBuffer(&count, sizeof(count), 1);
    for (BaseEditor *editor = first_editor; editor != NULL; editor = editor->next) {
        stream.BeginBlock(editor->GetName());
        editor->Serialise(stream);
        stream.EndBlock();
    }
    stream.EndBlock();
    return 1;
}

i32 LevelEditor::ReadStream(EdFileInputStream &stream) {
    if (stream.BeginBlock("FileInfo") != NULL) {
        stream.SerialiseBuffer(&file_version, sizeof(file_version), 1);
        stream.EndBlock();
    }
    if (stream.BeginBlock("Settings") != NULL) {
        settings.Serialise(stream);
        stream.EndBlock();
    }
    if (stream.BeginBlock("Editors") != NULL) {
        // The original keeps the editor count in a 16-byte aligned slot.
        i32 count __attribute__((aligned(16)));
        stream.SerialiseBuffer(&count, sizeof(count), 1);
        for (i32 index = 0; index < count; ++index) {
            char const *name = stream.BeginBlock(NULL);
            for (BaseEditor *editor = first_editor; editor != NULL; editor = editor->next) {
                if (NuStrICmp(editor->GetName(), name) == 0) {
                    editor->Serialise(stream);
                    break;
                }
            }
            stream.EndBlock();
        }
        stream.EndBlock();
    }
    return 1;
}

SpecialObject::SpecialObject() {
    NuSpecialClear(&special);
}

SceneObject::SceneObject() {
    editor_owned = 0;
    scene_id = static_cast<i16>(theSceneObjectHelper.scene_id);
}

void SceneObjectHelper::PreLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
    for (i32 level = 0; level < 10; ++level) {
        i32 *count_slot = &scene_counts[level];
        SceneObject **scene_slot = &scenes[level];
        nugscn_s *scene = theLevelEditor.GetScene(level);
        if (scene != NULL && *count_slot == 0) {
            *count_slot = NuGScnNumSpecials(scene);
            const usize bytes = *count_slot * sizeof(SceneObject);
            SceneObject *objects = NULL;
            if (bytes < *theMemoryManager.end_cell - *theMemoryManager.cursor_cell) {
                const usize aligned = ALIGN(*theMemoryManager.cursor_cell, 0x10);
                *theMemoryManager.cursor_cell = aligned + bytes;
                objects = reinterpret_cast<SceneObject *>(aligned);
                memset(objects, 0, bytes);
                theMemoryManager.allocated += bytes;
                theMemoryManager.remaining -= bytes;
                theMemoryManager.high_water = *theMemoryManager.cursor_cell;
            }
            *scene_slot = objects;
            for (i32 index = 0; index < *count_slot; ++index)
                new (&(*scene_slot)[index]) SceneObject();
        }
    }

    scene_object_count = 0;
    i32 *count_slot = scene_counts;
    for (i32 level = 0; level < 10; ++level, ++count_slot) {
        SceneObject *object = *reinterpret_cast<SceneObject **>(count_slot - 10);
        nugscn_s *scene = theLevelEditor.GetScene(level);
        if (scene != NULL) {
            const i32 count = *count_slot;
            for (i32 index = 0; index < count; ++index) {
                NuGScnGetSpecial(&object->special, scene, index);
                object->led_file = level;
                object->attributes = 0x12400000;
                char *name = NuSpecialGetName(&object->special);
                if (name != NULL)
                    NuStrICmp(name, "reflectport");
                ++object;
                ++scene_object_count;
            }
        }
    }
}

SceneInstance::SceneInstance() : next(NULL), previous(NULL) {
    editor_owned = 1;
    scene_id = static_cast<i16>(theSceneObjectHelper.scene_id);
    NuMtxSetIdentity(&initial_transform);
    NuMtxSetIdentity(&current_transform);
    visibility = 1;
}

void *SceneObjectHelper::CreateObject(void *, i32, i32) {
    void *storage = theMemoryManager.AllocPool(sizeof(SceneInstance), 1);
    SceneInstance *object = new (storage) SceneInstance();
    if (object != NULL) {
        object->next = NULL;
        object->previous = owned_last;
        if (owned_last != NULL)
            owned_last->next = object;
        owned_last = object;
        if (owned_first == NULL)
            owned_first = object;
        ++owned_object_count;
        object->SetName("object");
    }
    return object;
}

Placeable *SceneObject::Clone(i32 attributes) const {
    SceneInstance *copy = static_cast<SceneInstance *>(theSceneObjectHelper.CreateObject(NULL, 0, 0));
    if (copy == NULL)
        return NULL;
    EdClass *object_class = theSceneObjectHelper.class_interface.object_class;
    object_class->CopyObject(copy, const_cast<SceneObject *>(this));
    char unique_name[128];
    theClassEditor.MakeUniqueName(copy->GetName(), unique_name, sizeof(unique_name));
    copy->SetName(unique_name);
    copy->attributes = attributes;
    return copy;
}

void SceneObjectHelper::SetSceneFilter(char *filter) {
    if (filter != NULL)
        NuStrCpy(scene_filter, filter);
}

EdSpecialObjectControl::EdSpecialObjectControl() {
    menu = NULL;
}

void EdSpecialObjectControl::AddMenuItem(eduimenu_s *parent, EdRef *member, void *target) {
    EdSpecialObjectControl *control =
        new (theMemoryManager.AllocPool(sizeof(EdSpecialObjectControl), 1)) EdSpecialObjectControl();
    if (!control)
        return;
    control->reference = member;
    control->object = target;
    nuhspecial_s special;
    member->GetMemberData(target, EdType_NuHSpecial, &special, 0);
    char *name = NuSpecialGetName(&special);
    if (name == NULL)
        name = const_cast<char *>("no name");
    control->item = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected, cbChanged,
                                       cbButton, 1, member->name, name);
    eduiMenuAddItem(parent, control->item);
}

f32 BezierLineLength(VuVec &start, VuVec &first_control, VuVec &end, VuVec &second_control) {
    VuVec first{(start.x + first_control.x) * 0.5f, (start.y + first_control.y) * 0.5f,
                (start.z + first_control.z) * 0.5f, 0.0f};
    VuVec middle{(first_control.x + second_control.x) * 0.5f, (first_control.y + second_control.y) * 0.5f,
                 (first_control.z + second_control.z) * 0.5f, 0.0f};
    VuVec last{(second_control.x + end.x) * 0.5f, (second_control.y + end.y) * 0.5f, (second_control.z + end.z) * 0.5f,
               0.0f};
    VuVec first_middle{(first.x + middle.x) * 0.5f, (first.y + middle.y) * 0.5f, (first.z + middle.z) * 0.5f, 0.0f};
    VuVec middle_last{(middle.x + last.x) * 0.5f, (middle.y + last.y) * 0.5f, (middle.z + last.z) * 0.5f, 0.0f};
    VuVec midpoint{(first_middle.x + middle_last.x) * 0.5f, (first_middle.y + middle_last.y) * 0.5f,
                   (first_middle.z + middle_last.z) * 0.5f, 0.0f};
    NUVEC error{(start.x + end.x) * 0.5f - midpoint.x, (start.y + end.y) * 0.5f - midpoint.y,
                (start.z + end.z) * 0.5f - midpoint.z};
    if (!(NuVecMag(&error) < 0.01f) && bezierline_depth < 2) {
        ++bezierline_depth;
        f32 first_length = BezierLineLength(start, first, midpoint, first_middle);
        f32 second_length = BezierLineLength(midpoint, middle_last, end, last);
        --bezierline_depth;
        return first_length + second_length;
    }
    NUVEC chord{start.x - end.x, start.y - end.y, start.z - end.z};
    return NuVecMag(&chord);
}

f32 BezierLineLength(VuVec &start, VuVec &first_control, VuVec &end, VuVec &second_control, f32 t) {
    f32 complement = 1.0f - t;
    VuVec middle{second_control.x * complement + first_control.x * t,
                 second_control.y * complement + first_control.y * t,
                 second_control.z * complement + first_control.z * t, 0.0f};
    VuVec first{first_control.x * complement + start.x * t, first_control.y * complement + start.y * t,
                first_control.z * complement + start.z * t, 0.0f};
    VuVec last{end.x * complement + second_control.x * t, end.y * complement + second_control.y * t,
               end.z * complement + second_control.z * t, 0.0f};
    VuVec first_middle{middle.x * complement + first.x * t, middle.y * complement + first.y * t,
                       middle.z * complement + first.z * t, 0.0f};
    VuVec middle_last{last.x * complement + middle.x * t, last.y * complement + middle.y * t,
                      last.z * complement + middle.z * t, 0.0f};
    VuVec point{middle_last.x * complement + first_middle.x * t, middle_last.y * complement + first_middle.y * t,
                middle_last.z * complement + first_middle.z * t, 0.0f};
    return BezierLineLength(start, first, point, first_middle);
}

i32 BezierLinePos(VuVec &result, VuVec &start, VuVec &first_control, VuVec &end, VuVec &second_control, f32 distance) {
    f32 t = 0.5f;
    f32 step = 0.25f;
    for (i32 count = 256; count != 0; --count) {
        f32 complement = 1.0f - t;
        VuVec middle{second_control.x * complement + first_control.x * t,
                     second_control.y * complement + first_control.y * t,
                     second_control.z * complement + first_control.z * t, 0.0f};
        VuVec first{first_control.x * complement + start.x * t, first_control.y * complement + start.y * t,
                    first_control.z * complement + start.z * t, 0.0f};
        VuVec last{end.x * complement + second_control.x * t, end.y * complement + second_control.y * t,
                   end.z * complement + second_control.z * t, 0.0f};
        VuVec first_middle{middle.x * complement + first.x * t, middle.y * complement + first.y * t,
                           middle.z * complement + first.z * t, 0.0f};
        VuVec middle_last{last.x * complement + middle.x * t, last.y * complement + middle.y * t,
                          last.z * complement + middle.z * t, 0.0f};
        VuVec point{middle_last.x * complement + first_middle.x * t, middle_last.y * complement + first_middle.y * t,
                    middle_last.z * complement + first_middle.z * t, 1.0f};
        f32 length = BezierLineLength(start, first, point, first_middle);
        f32 difference = distance - length;
        if (difference < 0.0f)
            difference = -difference;
        if (!(difference > 0.01f)) {
            *reinterpret_cast<BezierVec4Unaligned *>(&result) = *reinterpret_cast<BezierVec4 *>(&point);
            return 1;
        }
        if (distance < length)
            t += step;
        else
            t -= step;
        step *= 0.5f;
        if (count == 1) {
            *reinterpret_cast<BezierVec4Unaligned *>(&result) = *reinterpret_cast<BezierVec4 *>(&point);
            return 1;
        }
    }
    return 1;
}

void BezierLineEval(VuVec &result, VuVec &start, VuVec &first_control, VuVec &end, VuVec &second_control, float along) {
    const f32 complement = 1.0f - along;
    const f32 complement_squared = complement * complement;
    const f32 squared = along * along;
    const f32 start_weight = complement_squared * complement;
    const f32 first_weight = (along * 3.0f) * complement_squared;
    const f32 second_weight = (3.0f * squared) * complement;
    const f32 end_weight = along * squared;
    const f32 z = end.z * end_weight +
                  ((first_control.z * first_weight + start.z * start_weight) + second_control.z * second_weight);
    const f32 y = end.y * end_weight +
                  (second_control.y * second_weight + (first_control.y * first_weight + start.y * start_weight));
    const f32 x = end_weight * end.x +
                  (second_weight * second_control.x + (first_weight * first_control.x + start_weight * start.x));
    result.w = 0.0f;
    result.y = y;
    result.z = z;
    result.x = x;
}

void DrawBezierLine(VuVec &start, VuVec &start_control, VuVec &end, VuVec &end_control, numtl_s *material, i32 colour) {
    static i32 bezier_draw_depth;

    const f32 camera_y = global_camera.mtx.m31;
    const f32 camera_x = global_camera.mtx.m30;
    const f32 camera_z = global_camera.mtx.m32;
    const f32 start_y = start.y - camera_y;
    const f32 start_z = start.z - camera_z;
    const f32 start_x = start.x - camera_x;
    const f32 start_distance = start_x * start_x + start_y * start_y + start_z * start_z;
    const f32 end_x = end.x - camera_x;
    const f32 end_y = end.y - camera_y;
    const f32 end_z = end.z - camera_z;
    const f32 end_distance = end_x * end_x + end_y * end_y + end_z * end_z;
    const f32 distance = end_distance < start_distance ? end_distance : start_distance;
    const f32 threshold = distance < 5.5f ? 5.5f * 0.001f : distance * 0.001f;

    const f32 chord_y = end.y - start.y;
    const f32 chord_x = end.x - start.x;
    const f32 chord_z = end.z - start.z;
    if (!(chord_x * chord_x + chord_y * chord_y + chord_z * chord_z > threshold) || bezier_draw_depth > 15) {
        EdDrawLineSegment(start, end, colour);
        return;
    }

    ++bezier_draw_depth;
    const f32 end_control_x = end_control.x;
    const f32 end_control_y = end_control.y;
    const f32 control_x = start_control.x;
    const f32 end_control_z = end_control.z;
    VuVec second = {(control_x + end_control_x) * 0.5f, (end_control_y + start_control.y) * 0.5f,
                    (end_control_z + start_control.z) * 0.5f, 0.0f};
    VuVec first = {(start.x + control_x) * 0.5f, (start.y + start_control.y) * 0.5f, (start_control.z + start.z) * 0.5f,
                   0.0f};
    VuVec third = {(end.x + end_control_x) * 0.5f, (end.y + end_control_y) * 0.5f, (end.z + end_control_z) * 0.5f,
                   0.0f};
    VuVec fourth = {(first.x + second.x) * 0.5f, (first.y + second.y) * 0.5f, (first.z + second.z) * 0.5f, 0.0f};
    VuVec fifth = {(third.x + second.x) * 0.5f, (third.y + second.y) * 0.5f, (third.z + second.z) * 0.5f, 0.0f};
    VuVec midpoint = {(fifth.x + fourth.x) * 0.5f, (fifth.y + fourth.y) * 0.5f, (fifth.z + fourth.z) * 0.5f, 0.0f};

    DrawBezierLine(start, first, midpoint, fourth, material, colour);
    DrawBezierLine(midpoint, fifth, end, third, material, colour);
    --bezier_draw_depth;
}

SplinePointBlock::SplinePointBlock() {
    next = NULL;
    previous = NULL;
    capacity = 16;
    points = static_cast<VuVec *>(theMemoryManager.AllocPool(16 * sizeof(VuVec), 1));
}

SplinePointBlock::SplinePointBlock(i32 requested_capacity) {
    next = NULL;
    previous = NULL;
    capacity = requested_capacity;
    points = static_cast<VuVec *>(theMemoryManager.AllocPool(requested_capacity * sizeof(VuVec), 1));
}

void SplinePointBlock::Draw() {
    for (i32 index = 0; index < point_count; ++index)
        EdDrawLineCross(points[index], 0.5f, -1);
}

i32 SplinePointList::GetNumPoints() {
    i32 point_count = 0;
    for (SplinePointBlock *block = first; block != NULL; block = block->next) {
        point_count = block->point_count;
    }
    return point_count;
}

__attribute__((force_align_arg_pointer)) i32 SplinePointList::GetPoint(i32 index, VuVec &point) {
    SplinePointBlock *block = first;
    if (block == NULL)
        return false;
    if (index >= block->point_count) {
        do {
            block = block->next;
            if (block == NULL)
                return false;
        } while (index >= block->point_count);
    }

#if defined(__i386__) && defined(__SSE__)
    const VuVec &source = block->points[index];
    __m128 lanes = _mm_loadl_pi(_mm_setzero_ps(), reinterpret_cast<const __m64 *>(&source));
    lanes = _mm_loadh_pi(lanes, reinterpret_cast<const __m64 *>(&source.z));
    _mm_storel_pi(reinterpret_cast<__m64 *>(&point), lanes);
    _mm_storeh_pi(reinterpret_cast<__m64 *>(&point.z), lanes);
#else
    point = block->points[index];
#endif
    return true;
}

void SplinePointList::AddPoint(VuVec &point) {
    SplinePointBlock *block = first;
    while (block != NULL) {
        if (block->point_count < block->capacity) {
            break;
        }
        block = block->next;
    }
    if (block == NULL) {
        block = new (theMemoryManager.AllocPool(sizeof(SplinePointBlock), 1)) SplinePointBlock();
        if (block == NULL)
            return;
        i32 index = block->point_count;
        VuVec &destination = block->points[index];
        block->point_count = index + 1;
        destination.x = point.x;
        destination.y = point.y;
        destination.z = point.z;
        destination.w = point.w;
        block->next = NULL;
        block->previous = last;
        if (last != NULL)
            last->next = block;
        last = block;
        if (first == NULL)
            first = block;
        ++block_count;
        return;
    }
    i32 index = block->point_count;
    VuVec &destination = block->points[index];
    block->point_count = index + 1;
    destination.x = point.x;
    destination.y = point.y;
    destination.z = point.z;
    destination.w = point.w;
}

void SplinePointList::Clear() {
    SplinePointBlock *block = first;
    while (block != NULL) {
        SplinePointBlock *next = block->next;
        if (block->capacity == 16) {
            if (next == NULL)
                last = block->previous;
            else
                next->previous = block->previous;
            if (block->previous == NULL)
                first = next;
            else
                block->previous->next = next;
            block->next = NULL;
            block->previous = NULL;
            --block_count;
            delete block;
        } else {
            block->point_count = 0;
        }
        block = next;
    }
}

void EdRefSpline::SetMemberData(void *object, i32 type, void *data, i32 data_size, i16 *) {
    CheckType(type);
    switch (member_offset) {
        case static_cast<i32>(0x80000001):
            static_cast<SplineObject *>(object)->step = *static_cast<f32 *>(data);
            if (theSplineHelper.auto_generate_points)
                static_cast<SplineObject *>(object)->points.Clear();
            break;
        case static_cast<i32>(0x80000002):
            static_cast<SplineObject *>(object)->height = *static_cast<f32 *>(data);
            if (theSplineHelper.auto_generate_points)
                static_cast<SplineObject *>(object)->points.Clear();
            break;
        case static_cast<i32>(0x80000003):
            static_cast<SplineObject *>(object)->drop = *static_cast<i32 *>(data) != 0;
            if (theSplineHelper.auto_generate_points)
                static_cast<SplineObject *>(object)->points.Clear();
            break;
        case static_cast<i32>(0x80000004):
            static_cast<SplineObject *>(object)->closed = *static_cast<i32 *>(data);
            if (theSplineHelper.auto_generate_points)
                static_cast<SplineObject *>(object)->points.Clear();
            break;
        default:
            // The original setter delegates unrecognized members to the getter.
            EdRef::GetMemberData(object, type, data, data_size);
            break;
    }
}

void SplinePointList::Draw() {
    for (SplinePointBlock *block = first; block != NULL; block = block->next)
        block->Draw();
}

__attribute__((force_align_arg_pointer)) i32 SplineKnotList::GetPoint(i32 index, VuVec &point) {
    SplineKnot *knot = first;
    while (index != 0 && knot != NULL) {
        knot = knot->next;
        index--;
    }

    if (knot == NULL) {
        return false;
    }

#if defined(__i386__) && defined(__SSE__)
    __m128 lanes = _mm_loadl_pi(_mm_setzero_ps(), reinterpret_cast<const __m64 *>(&knot->position));
    lanes = _mm_loadh_pi(lanes, reinterpret_cast<const __m64 *>(&knot->position.z));
    _mm_storel_pi(reinterpret_cast<__m64 *>(&point), lanes);
    _mm_storeh_pi(reinterpret_cast<__m64 *>(&point.z), lanes);
#else
    point = knot->position;
#endif
    return true;
}

void SplineObject::ReverseKnots() {
    SplineKnot *old_first = knots.first;
    SplineKnot *knot = knots.last;
    knots.last = NULL;
    knots.first = NULL;
    knots.count = 0;
    i32 next_count = 1;
    if (knot != NULL) {
        while (true) {
            SplineKnot *prior = knot->previous;
            SplineKnot *next = knot->next;
            if (__builtin_expect(next != NULL, 1))
                next->previous = prior;
            if (prior != NULL)
                prior->next = next;
            else
                old_first = next;
            knot->next = NULL;
            knot->previous = knots.last;
            if (knots.last != NULL)
                knots.last->next = knot;
            bool had_first = knots.first != NULL;
            knots.last = knot;
            if (!had_first)
                knots.first = knot;
            i32 count = next_count++;
            if (prior == NULL) {
                knots.count = count;
                break;
            }
            knot = prior;
        }
    }
    while (old_first != NULL) {
        SplineKnot *next = old_first->next;
        if (next != NULL)
            next->previous = old_first->previous;
        if (__builtin_expect(old_first->previous != NULL, 0))
            old_first->previous->next = next;
        old_first->next = NULL;
        old_first->previous = NULL;
        theMemoryManager.FreePool(old_first, sizeof(SplineKnot));
        old_first = next;
    }
}

void SplineObject::Draw(i32, i32 colour_mode, i32 straight, float show_points) {
    i32 colour = colour_mode == 0 ? static_cast<i32>(0x80808080) : static_cast<i32>(0x80008080);
    EdDrawBegin(0);
    for (SplineKnot *knot = knots.first; knot != NULL && knot->next != NULL; knot = knot->next) {
        SplineKnot *next = knot->next;
        if (straight != 0 || step < 0.1f)
            EdDrawLineSegment(knot->position, next->position, colour);
        else
            DrawBezierLine(knot->position, knot->out_tangent, next->position, next->in_tangent, edLevel3dMtl, colour);
    }
    u32 show_points_bits;
    __builtin_memcpy(&show_points_bits, &show_points, sizeof(show_points_bits));
    if (show_points_bits != 0 && step > 0.1f)
        points.Draw();
    EdDrawEnd();
}

void SplineHelper::Render(void *object, i32 flags) {
    static_cast<SplineObject *>(object)->Draw(flags, flags == 0, flags, 0.0f);
}

void SplineHelper::Initialise() {
    EdClass *spline_class =
        theRegistry.RegisterClass(const_cast<char *>("Spline"), reinterpret_cast<EdClassInterface *>(this), 0x08400100);
    make_spline_reference<EdRef>(spline_class, const_cast<char *>("String"), const_cast<char *>("Spline"), 0x0c, 0x20,
                                 2, make_spline_string_control());
    make_spline_reference<EdRef>(spline_class, const_cast<char *>("Short"), const_cast<char *>("LEDFile"), 0x44, 0,
                                 0x10000100, NULL);
    make_spline_reference<EdRefSpline>(spline_class, const_cast<char *>("Float"), const_cast<char *>("Height"),
                                       static_cast<i32>(0x80000002), 0, 0, make_spline_float_control(-1.0e9f, 1.0e9f));
    make_spline_reference<EdRefSpline>(spline_class, const_cast<char *>("Float"), const_cast<char *>("Step"),
                                       static_cast<i32>(0x80000001), 0, 0, make_spline_float_control(0.0f, 1000.0f));
    make_spline_reference<EdRefSpline>(spline_class, const_cast<char *>("Int"), const_cast<char *>("Drop"),
                                       static_cast<i32>(0x80000003), 0, 0, make_spline_yes_no_control());
    make_spline_reference<EdRefSpline>(spline_class, const_cast<char *>("Int"), const_cast<char *>("Closed"),
                                       static_cast<i32>(0x80000004), 0, 0, make_spline_yes_no_control());

    EdClass *knot_class = theRegistry.RegisterClass(const_cast<char *>("Knot"),
                                                    reinterpret_cast<EdClassInterface *>(&theKnotHelper), 0x18400000);
    EdRef *position =
        make_spline_reference<EdRefKnot>(knot_class, const_cast<char *>("VuVec"), const_cast<char *>("Pos"),
                                         static_cast<i32>(0x80000003), 0, 8, make_spline_vector_control(), false);
    EdRef *in_tangent =
        make_spline_reference<EdRefKnot>(knot_class, const_cast<char *>("VuVec"), const_cast<char *>("In"),
                                         static_cast<i32>(0x80000004), 0, 8, make_spline_vector_control(), false);
    EdRef *out_tangent =
        make_spline_reference<EdRefKnot>(knot_class, const_cast<char *>("VuVec"), const_cast<char *>("Out"),
                                         static_cast<i32>(0x80000005), 0, 8, make_spline_vector_control(), false);
    theKnotHelper.position_ref = position;
    theKnotHelper.in_tangent_ref = in_tangent;
    theKnotHelper.out_tangent_ref = out_tangent;
    make_spline_reference<EdRefKnot>(knot_class, const_cast<char *>("String"), const_cast<char *>("Spline"),
                                     static_cast<i32>(0x80000001), 0x20, 0x10000002, make_spline_string_control());
    make_spline_reference<EdRef>(knot_class, const_cast<char *>("Short"), const_cast<char *>("LEDFile"), 0x44, 0,
                                 0x10000100, NULL);
    knot_class->AddType(position);
    knot_class->AddType(in_tangent);
    knot_class->AddType(out_tangent);
    make_spline_reference<EdRefKnot>(knot_class, const_cast<char *>("Float"), const_cast<char *>("Radius"),
                                     static_cast<i32>(0x80000002), 0, 0x10000040, NULL);
}

SplineObject *SplineHelper::Find(char *name) {
    for (SplineObject *spline = first_object; spline != NULL; spline = spline->next) {
        if (NuStrICmp(name, spline->name) == 0)
            return spline;
    }
    return NULL;
}

i32 SplineHelper::Find(char *substring, SplineObject **results, i32 capacity) {
    i32 count = 0;
    for (SplineObject *spline = first_object; spline != NULL && count < capacity; spline = spline->next) {
        if (NuStrIStr(spline->name, substring) != NULL)
            results[count++] = spline;
    }
    return count;
}

EdStream::EdStream() {
    memory_buffer = NULL;
    secondary_buffer = NULL;
    mode = 0;
    swap_endianness = 0;
    flags = 0;
}

EdStream::EdStream(MemoryBuffer *buffer) {
    memory_buffer = buffer;
    secondary_buffer = NULL;
    mode = 0;
    swap_endianness = 0;
    flags = 0;
}

EdStream::EdStream(MemoryBuffer *buffer, MemoryBuffer *secondary) {
    memory_buffer = buffer;
    secondary_buffer = secondary;
    mode = 0;
    swap_endianness = 0;
    flags = 0;
}

void EdFileInputStream::Open(i32 handle, i32) {
    file = handle;
    version = 0;
    if (BeginBlock("StreamInfo")) {
        SerialiseBuffer(&version, sizeof(version), 1);
        EndBlock();
    }
}

i32 LevelEditor::LoadState(variptr_u *buffer, variptr_u *buffer_end, variptr_u *scratch_begin, variptr_u *scratch_end,
                           variptr_u *file_begin, variptr_u *file_end) {
    reserved_0x298 = 1;
    MemoryBuffer source = {buffer, buffer_end, 0, 0};
    MemoryBuffer scratch = {scratch_begin, scratch_end, 0, static_cast<u32>(scratch_end->addr - scratch_begin->addr)};
    if (!multi_load_active) {
        theClassEditor.PreLoadInitialisation(&source, &scratch);
    }
    i32 result = 0;
    NUFILE file = NuMemFileOpen(file_begin->void_ptr, file_end->addr - file_begin->addr, NUFILE_READ);
    if (file) {
        EdFileInputStream stream(&source, &scratch);
        stream.Open(file, 4);
        stream.flags = 0x400000;
        result = ReadStream(stream);
        stream.file = 0;
        NuFileClose(file);
    }
    if (!multi_load_active) {
        theClassEditor.PostLoadInitialisation(&source, &scratch);
    }
    reserved_0x298 = 0;
    return result;
}

i32 LevelEditor::Load(char *filename, variptr_u *buffer, variptr_u *buffer_end, i32 flags) {
    MemoryBuffer source
        __attribute__((aligned(16))) = {buffer, buffer_end, 0, static_cast<u32>(buffer_end->addr - buffer->addr)};
    MemoryBuffer scratch __attribute__((aligned(16)));
    scratch.position = NULL;
    scratch.end = NULL;
    if (editor_buffer_from_front == 0) {
        editor_buffer_end = *buffer_end;
        editor_buffer_begin.addr = buffer_end->addr - 0x20000;
    }

    {
        char *basename = NuStrRChr(filename, '/');
        i32 basename_length = NuStrLen(basename + 1);
        // The original keeps these name buffers 16-byte aligned and reuses their slots for the stream.
        char scene_name[32] __attribute__((aligned(16)));
        char directory[128] __attribute__((aligned(16)));
        NuStrNCpy(scene_name, basename + 1, basename_length - 3);
        NuStrNCpy(directory, filename, NuStrLen(filename) - basename_length);
        Placeable::CurrentLedFile = static_cast<i16>(FindSceneId(scene_name));
        i16 led_file = -1;
        if (Placeable::CurrentLedFile != -1) {
            NuStrCpy(scenes[Placeable::CurrentLedFile].directory, directory);
            led_file = Placeable::CurrentLedFile;
        }
        current_led_file = led_file;
    }

    editor_buffer_cursor = editor_buffer_begin;
    scratch = {&editor_buffer_cursor, &editor_buffer_end, 0,
               static_cast<u32>(editor_buffer_end.addr - editor_buffer_begin.addr)};
    SetSaveFilename(filename);
    if (!multi_load_active) {
        theClassEditor.PreLoadInitialisation(&source, &scratch);
    }

    i32 result = -1;
    void *file_data = scratch.position->void_ptr;
    i32 file_size = NuFileLoadBuffer(save_filename, file_data, scratch.remaining);
    if (file_size > 0) {
        if (static_cast<u32>(file_size) < scratch.end->addr - scratch.position->addr) {
            scratch.position->addr += file_size;
            scratch.used += file_size;
            scratch.remaining -= file_size;
        }
        NUFILE file = NuMemFileOpen(file_data, file_size, NUFILE_READ);
        if (file) {
            EdFileInputStream stream(&source, &scratch);
            if (flags) {
                stream.flags = 0x200000;
            }
            stream.Open(file, 4);
            i32 loaded = ReadStream(stream);
            stream.file = 0;
            NuFileClose(file);
            if (loaded) {
                result = Placeable::CurrentLedFile;
                scenes[result].editable = 1;
                Placeable::CurrentLedFile = -1;
            }
        }
    }
    if (!multi_load_active) {
        theClassEditor.PostLoadInitialisation(&source, &scratch);
    }
    return result;
}

void EdFileOutputStream::Open(i32 handle, i32 stream_version) {
    file = handle;
    version = stream_version;
    if (BeginBlock("StreamInfo")) {
        SerialiseBuffer(&version, sizeof(version), 1);
        EndBlock();
    }
}

i32 LevelEditor::SaveState(variptr_u *buffer, variptr_u *buffer_end) {
    i32 result = 0;
    NUFILE file = NuMemFileOpen(buffer->void_ptr, buffer_end->addr - buffer->addr, NUFILE_WRITE);
    if (file) {
        reserved_0x298 = 1;
        EdFileOutputStream stream;
        stream.Open(file, 4);
        stream.flags = 0x400000;
        result = WriteStream(stream);
        NuFilePos(file);
        stream.file = 0;
        NuFileClose(file);
    }
    reserved_0x298 = 0;
    return result;
}

i32 LevelEditor::SaveState(i32 scene_index, variptr_u *buffer, variptr_u *buffer_end) {
    i32 size = buffer_end->addr - buffer->addr;
    NUFILE file = NuMemFileOpen(buffer->void_ptr, size, NUFILE_WRITE);
    if (file) {
        reserved_0x298 = 1;
        EdFileOutputStream stream;
        stream.Open(file, 4);
        stream.flags = 0x400000;
        stream.unknown_10 = scene_index;
        WriteStream(stream);
        size = NuFilePos(file);
        stream.file = 0;
        NuFileClose(file);
    }
    reserved_0x298 = 0;
    return size;
}

i32 __attribute__((optimize("no-partial-inlining"))) LevelEditor::Save() {
    i32 saved_any = 0;
    if (__builtin_expect(!!(active), 1)) {
#define EDLEVEL_SAVE_SCENE(index)                                                                                      \
    do {                                                                                                               \
        LevelEditorScene &scene = scenes[index];                                                                       \
        theClassEditor.PreSaveInitialisation();                                                                        \
        sprintf(save_filename, "%s/%s.led", scene.directory, scene.name);                                              \
        if (scene.editable) {                                                                                          \
            variptr_u buffer = editor_buffer_begin;                                                                    \
            editor_buffer_cursor = buffer;                                                                             \
            NUFILE memory_file = NuMemFileOpen(buffer.void_ptr, editor_buffer_end.addr - buffer.addr, NUFILE_WRITE);   \
            if (memory_file) {                                                                                         \
                EdFileOutputStream stream;                                                                             \
                stream.Open(memory_file, 4);                                                                           \
                stream.unknown_10 = index;                                                                             \
                WriteStream(stream);                                                                                   \
                i32 size = NuFilePos(memory_file);                                                                     \
                stream.file = 0;                                                                                       \
                NuFileClose(memory_file);                                                                              \
                NUFILE output_file = NuFileOpen(save_filename, NUFILE_WRITE);                                          \
                if (!output_file) {                                                                                    \
                    scene.saved = 0;                                                                                   \
                } else {                                                                                               \
                    NuFileWrite(output_file, buffer.void_ptr, size);                                                   \
                    NuFileClose(output_file);                                                                          \
                    scene.saved = 1;                                                                                   \
                    saved_any = 1;                                                                                     \
                }                                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)
        EDLEVEL_SAVE_SCENE(0);
        EDLEVEL_SAVE_SCENE(1);
        EDLEVEL_SAVE_SCENE(2);
        EDLEVEL_SAVE_SCENE(3);
        EDLEVEL_SAVE_SCENE(4);
        EDLEVEL_SAVE_SCENE(5);
        EDLEVEL_SAVE_SCENE(6);
        EDLEVEL_SAVE_SCENE(7);
        EDLEVEL_SAVE_SCENE(8);
        EDLEVEL_SAVE_SCENE(9);
#undef EDLEVEL_SAVE_SCENE
        theClassEditor.PostSaveInitialisation();
    }
    return saved_any;
}

void LevelEditor::ProcessEvenWhenPaused(ThingProcessData *data) {
    if (!editors_entered) {
        return;
    }
    f32 delta_time = data->t;
    nupad_s *pad = data->pads[0];
    text_length = 0;
    memset(info_text, 0, sizeof(info_text));
    memset(reserved_0xef8, 0, 0x80);
    memset(reserved_0xef8 + 0x80, 0, 0x80);
    field_0x2c = -1;
    editable_scene_count = 0;
    if (scenes[0].editable)
        ++editable_scene_count;
    if (scenes[1].editable)
        ++editable_scene_count;
    if (scenes[2].editable)
        ++editable_scene_count;
    if (scenes[3].editable)
        ++editable_scene_count;
    if (scenes[4].editable)
        ++editable_scene_count;
    if (scenes[5].editable)
        ++editable_scene_count;
    if (scenes[6].editable)
        ++editable_scene_count;
    if (scenes[7].editable)
        ++editable_scene_count;
    if (scenes[8].editable)
        ++editable_scene_count;
    if (scenes[9].editable)
        ++editable_scene_count;
    if (edLevelActiveMenu == NULL && field_0x38 != 0 && input.GetPress(4) != 0.0f) {
        eduiSetCameraEnabled(0);
        eduiSetDefaultActiveMenu(eduiGetActiveMenu());
        theLevelEditor.CreateMenu();
    }
    if (edLevelDestroyActiveMenu != 0) {
        eduiMenuDestroy(edLevelActiveMenu);
        edLevelActiveMenu = NULL;
        edLevelDestroyActiveMenu = 0;
    }
    if (edLevelDestroyThisMenu != NULL) {
        eduiMenuDestroy(edLevelDestroyThisMenu);
        edLevelDestroyThisMenu = NULL;
    }
    if (edLevelDestroyThisMenu2 != NULL) {
        eduiMenuDestroy(edLevelDestroyThisMenu2);
        edLevelDestroyThisMenu2 = NULL;
    }
    if (edLevelNextMenu != NULL) {
        if (edLevelActiveMenu != NULL) {
            eduiMenuDestroy(edLevelActiveMenu);
        }
        eduimenu_s *next_menu = edLevelNextMenu;
        edLevelNextMenu = NULL;
        edLevelActiveMenu = next_menu;
    }
    nucamera_s *camera = &global_camera;
    if (field_0x28 != 0) {
        camera = edmainGetCamera();
    }
    input.Update(camera, pad, delta_time, thePropertyTool.HasActiveMenu());
    if (edmainGetCursorEnabled()) {
        eduiProcessCursor(0.0f, pad);
    }
    if (field_0x28 != 0 && eduiGetCameraEnabled()) {
        if (edlevel_mouseandkeyboard == 0) {
            edcamMove(pad);
        } else {
            edcamMove(NULL);
        }
        edcamGetPosAng(reinterpret_cast<NUVEC *>(background_colour), &field_0x20, &field_0x24);
    }
    if (input.GetPress(17) != 0.0f) {
        overlay_alpha += 0.1f;
        eduiSetFontScale(overlay_alpha, overlay_alpha);
    }
    if (input.GetPress(18) != 0.0f && static_cast<f32>(EdLevelFntScale) > 0.1f) {
        overlay_alpha -= 0.1f;
        eduiSetFontScale(overlay_alpha, overlay_alpha);
    }
    if (edLevelActiveMenu != NULL) {
        NuFntSet(EdLevelFnt);
        NuFntScale(EdLevelFntScale, EdLevelFntScale);
        if (eduiMenuProcess(edLevelActiveMenu, delta_time, pad) != 0) {
            input.Clear(3);
        } else if (edLevelActiveMenu != edLevelPinnedMenu) {
            if (input.GetPress(3) != 0.0f) {
                eduiSetCameraEnabled(1);
                eduiMenuDestroy(edLevelActiveMenu);
                edLevelActiveMenu = NULL;
                eduiSetDefaultActiveMenu(NULL);
            }
            if (input.GetPress(4) != 0.0f) {
                eduiMenuDestroy(edLevelActiveMenu);
                edLevelActiveMenu = NULL;
                theLevelEditor.CreateMenu();
            }
        }
    } else if (edLevelPinnedMenu != NULL && !thePropertyTool.HasActiveMenu()) {
        NuFntSet(EdLevelFnt);
        NuFntScale(EdLevelFntScale, EdLevelFntScale);
        if (eduiMenuProcess(edLevelPinnedMenu, delta_time, pad) != 0) {
            return;
        }
    }
    if (active_editor != NULL) {
        active_editor->Process(input);
    }
    if (input.GetPress(23) != 0.0f) {
        theLevelEditor.Save();
    }
}

void cbEdLevelSave(eduimenu_s *parent, eduiitem_s *item, u32) {
    theLevelEditor.Save();
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy,
                                      const_cast<char *>("Save File"));
    char message[128];
    if (!theLevelEditor.active) {
        if (menu) {
            strcpy(message, "Saving not allowed in Debug Mode - change to Edit Mode");
            eduiMenuAddItem(menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect, message));
            eduiMenuFitWidth(menu, 5);
            eduiMenuFitOnScreen(menu, 1);
        }
    } else {
        for (i32 scene_index = 0; scene_index < theLevelEditor.reset_pending; ++scene_index) {
            LevelEditorScene *scene = theLevelEditor.GetEdScene(scene_index);
            if (!scene) {
                continue;
            }
            if (scene->active && scene->editable && scene->saved) {
                scene->saved = 0;
                if (menu) {
                    sprintf(message, "%s successfully saved", scene->name);
                    eduiMenuAddItem(menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect, message));
                    eduiMenuFitWidth(menu, 5);
                    eduiMenuFitOnScreen(menu, 1);
                }
            } else if (menu && scene->active && scene->editable) {
                sprintf(message, "%s : Save Failed, have you got the lock?", scene->name);
                eduiMenuAddItem(menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect, message));
                eduiMenuFitWidth(menu, 5);
                eduiMenuFitOnScreen(menu, 1);
            }
        }
    }
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
}

void EdSystem::RegisterSubSystem(EdSubSystem *subsystem) {
    subsystem->next = NULL;
    subsystem->previous = last_subsystem;
    if (last_subsystem != NULL)
        last_subsystem->next = subsystem;
    last_subsystem = subsystem;
    if (first_subsystem == NULL)
        first_subsystem = subsystem;
    ++subsystem_count;
}

void SceneObjectHelper::Initialise() {
    EdClass *object_class = theRegistry.RegisterClass("SceneObject", &class_interface, 0x100);
    if (object_class != NULL) {
        add_scene_reference<EdRef>(object_class, "Placeable", "Placeable", 0, 0, 0);

        void *control_memory = theMemoryManager.AllocPool(sizeof(EdSpecialObjectControl), 1);
        EdSpecialObjectControl *object_control = new (control_memory) EdSpecialObjectControl();
        add_scene_reference<EdRefSpecialObject>(object_class, "NuHSpecial", "Object", static_cast<i32>(0x80000008), 0,
                                                0, object_control);

        void *visible_memory = theMemoryManager.AllocPool(sizeof(EdEnumControl), 1);
        EdEnumControl *visible_control = new (visible_memory) EdEnumControl;
        visible_control->items = EdEnumControl::YesNoItems;
        add_scene_reference<EdRefSpecialObject>(object_class, "Int", "Visible", static_cast<i32>(0x80000009), 0, 0,
                                                visible_control);

        void *collision_memory = theMemoryManager.AllocPool(sizeof(EdEnumControl), 1);
        EdEnumControl *collision_control = new (collision_memory) EdEnumControl;
        collision_control->items = EdEnumControl::YesNoItems;
        add_scene_reference<EdRefSpecialObject>(object_class, "Int", "Collision", static_cast<i32>(0x8000000a), 0, 0,
                                                collision_control);

        add_scene_reference<EdRefPlaceable>(object_class, "Int", "Attributes", static_cast<i32>(0x80000002), 0,
                                            0x10400000);
        add_scene_reference<EdRefPlaceable>(object_class, "Short", "LEDFile", static_cast<i32>(0x80000001), 0,
                                            0x10400000);
        add_scene_reference<EdRefPlaceable>(object_class, "String", "Name", static_cast<i32>(0x80000003), 250,
                                            0x10400000);
        add_scene_reference<EdRefPlaceable>(object_class, "VuMtx", "Pos", static_cast<i32>(0x80000006), 0, 0x10400000);
    }
    thePlaceableHelper.RegisterObjectType("Scene Object", reinterpret_cast<PlaceableInterface *>(this));
    theEdSystem.RegisterSubSystem(&subsystem);
    memset(scene_counts, 0, sizeof(scene_counts));
}

void EdSystem::Initalise(variptr_u &buffer, variptr_u &buffer_end, i32 flags) {
    for (EdSubSystem *subsystem = first_subsystem; subsystem != NULL; subsystem = subsystem->next)
        subsystem->SubInitialise(buffer, buffer_end, flags);
}

void LevelEditor::Initalise(variptr_u &buffer, variptr_u &buffer_end, i32 from_front) {
    editor_buffer_from_front = from_front;
    if (from_front != 0) {
        editor_buffer_begin.addr = ALIGN(buffer.addr, 16);
        buffer.addr = ALIGN(buffer.addr, 16) + 0x20000;
        editor_buffer_end = buffer;
    }

    RegisterEditor(theClassEditor);
    for (BaseEditor *editor = first_editor; editor != NULL; editor = editor->next) {
        editor->Initialise(buffer, buffer_end, from_front);
    }
    active_editor = &theClassEditor;
    theEdSystem.Initalise(buffer, buffer_end, from_front);

    edLevel2dMtl = NuMtlCreate(1);
    edLevel2dMtl->diffuse_color.r = 0.5f;
    edLevel2dMtl->diffuse_color.g = 0.5f;
    edLevel2dMtl->diffuse_color.b = 0.5f;
    edLevel2dMtl->opacity = 1.0f;
    edLevel2dMtl->attribs.unknown_2_4 = 1;
    edLevel2dMtl->attribs.cull_mode = 2;
    edLevel2dMtl->attribs.z_mode = 3;
    edLevel2dMtl->attribs.alpha_mode = 1;
    NuMtlUpdate(edLevel2dMtl);

    edLevel3dMtl = NuMtlCreate(1);
    edLevel3dMtl->diffuse_color.r = 0.5f;
    edLevel3dMtl->diffuse_color.g = 0.5f;
    edLevel3dMtl->diffuse_color.b = 0.5f;
    edLevel3dMtl->opacity = 1.0f;
    edLevel3dMtl->attribs.alpha_mode = 0;
    edLevel3dMtl->attribs.unknown_2_1_2 = 2;
    edLevel3dMtl->attribs.unknown_2_4 = 1;
    NuMtlUpdate(edLevel3dMtl);

    edLevelTexMtl = NuMtlCreate(1);
    edLevelTexMtl->diffuse_color.r = 0.5f;
    edLevelTexMtl->diffuse_color.g = 0.5f;
    edLevelTexMtl->diffuse_color.b = 0.5f;
    edLevelTexMtl->opacity = 1.0f;
    edLevelTexMtl->attribs.alpha_mode = 0;
    edLevelTexMtl->attribs.cull_mode = 2;
    edLevelTexMtl->attribs.z_mode = 3;
    edLevelTexMtl->attribs.unknown_2_4 = 1;
    NuMtlUpdate(edLevelTexMtl);

    NuFntSet(EdLevelFnt);
    NuFntScale(EdLevelFntScale, EdLevelFntScale);
    EdDrawBegin(0);
    EdDrawEnd();
}

void EdSystem::Reset() {
    for (EdSubSystem *subsystem = first_subsystem; subsystem != NULL; subsystem = subsystem->next) {
        subsystem->SubReset();
    }
}

void EdSystem::Process(float delta_time) {
    for (EdSubSystem *subsystem = first_subsystem; subsystem != NULL; subsystem = subsystem->next)
        subsystem->SubProcess(delta_time);
}

void EdSystem::Render() {
    for (EdSubSystem *subsystem = first_subsystem; subsystem != NULL; subsystem = subsystem->next)
        subsystem->SubRender();
}

void EdTerrInit(void *, void *) {
}

void EdTerrShadow(nuvec_s *position, float height_above, float height_below, i32 terrain_mask) {
    reinterpret_cast<void (*)(nuvec_s *, f32, f32, i32)>(NewShadow)(position, height_above, height_below, terrain_mask);
}

i32 EdTerrRay(VuVec &, VuVec &) {
    return 0;
}

__attribute__((force_align_arg_pointer)) void SplineObject::DropPoint(VuVec &point) {
    if (drop != 0) {
        VuVec start = point;
        point.y -= 10.0f;
        VuVec direction{0.0f, -1000.0f, 0.0f, 1.0f};
        if (EdTerrRay(start, direction) != 0) {
#if defined(__i386__) && defined(__SSE__)
            __m128 lanes = _mm_load_ps(&start.x);
            _mm_storel_pi(reinterpret_cast<__m64 *>(&point), lanes);
            _mm_storeh_pi(reinterpret_cast<__m64 *>(&point.z), lanes);
#else
            point = start;
#endif
        }
    }
}

void SplineObject::GenBezierPoints() {
    points.Clear();
    SplineKnot *start = knots.first;
    if (start == NULL)
        return;
    SplineKnot *end = start->next;
    i32 closing = 0;
    f32 distance = 0.0f;
    while (end != NULL) {
        f32 length = BezierLineLength(start->position, start->out_tangent, end->position, end->in_tangent);
        for (; distance < length; distance += step) {
            VuVec point;
            BezierLinePos(point, start->position, start->out_tangent, end->position, end->in_tangent, distance);
            DropPoint(point);
            point.y += height;
            points.AddPoint(point);
        }
        distance -= length;
        start = end;
        end = end->next;
        if (closing)
            return;
        if (end == NULL && closed != 0) {
            closing = 1;
            end = knots.first;
        }
    }
}

void SplineObject::GenLinearPoints() {
    points.Clear();
    VuVec point;
    for (SplineKnot *knot = knots.first; knot != NULL; knot = knot->next) {
        point.x = knot->position.x;
        point.y = knot->position.y;
        point.z = knot->position.z;
        point.w = knot->position.w;
        DropPoint(point);
        point.y += height;
        points.AddPoint(point);
    }
    if (closed != 0 && knots.first != NULL) {
        point.x = knots.first->position.x;
        point.y = knots.first->position.y;
        point.z = knots.first->position.z;
        point.w = knots.first->position.w;
        DropPoint(point);
        point.y += height;
        points.AddPoint(point);
    }
}

void SplineObject::GenPoints() {
    if (0.1f > step)
        GenLinearPoints();
    else
        GenBezierPoints();
}

void SplineHelper::cbEdSplineReGenPoints(eduimenu_s *, eduiitem_s *, u32) {
    for (ClassObjectListEntry *selected = theClassEditor.selected_objects.first; selected != NULL;
         selected = selected->next) {
        if (selected->ed_class == theSplineHelper.object_class)
            static_cast<SplineObject *>(selected->object)->GenPoints();
    }
}

void SplineHelper::cbEdSplineReverseSpline(eduimenu_s *, eduiitem_s *, u32) {
    for (ClassObjectListEntry *selected = theClassEditor.selected_objects.first; selected != NULL;
         selected = selected->next) {
        if (selected->ed_class == theSplineHelper.object_class) {
            SplineObject *spline = static_cast<SplineObject *>(selected->object);
            spline->ReverseKnots();
            if (theSplineHelper.auto_generate_points)
                spline->GenPoints();
        }
    }
}

SplineObject *SplineObject::Clone() {
    SplineObject *clone = new (theMemoryManager.AllocPool(sizeof(SplineObject), 1)) SplineObject();
    NuStrCpy(clone->name, name);
    SplineKnot *source = knots.first;
    for (i32 index = 0; index < knots.count; ++index) {
        SplineKnot *knot = new (theMemoryManager.AllocPool(sizeof(SplineKnot), 1)) SplineKnot;
        if (knot != NULL) {
            knot->next = NULL;
            knot->previous = NULL;
        }
        knot->spline = clone;
        knot->led_file = Placeable::CurrentLedFile;
        knot->previous = clone->knots.last;
        if (clone->knots.last != NULL)
            clone->knots.last->next = knot;
        const bool had_first = clone->knots.first != NULL;
        clone->knots.last = knot;
        if (!had_first)
            clone->knots.first = knot;
        ++clone->knots.count;
        knot->position.x = source->position.x;
        knot->position.y = source->position.y;
        knot->position.z = source->position.z;
        knot->position.w = source->position.w;
        knot->in_tangent.x = source->in_tangent.x;
        knot->in_tangent.y = source->in_tangent.y;
        knot->in_tangent.z = source->in_tangent.z;
        knot->in_tangent.w = source->in_tangent.w;
        knot->out_tangent.x = source->out_tangent.x;
        knot->out_tangent.y = source->out_tangent.y;
        knot->out_tangent.w = source->out_tangent.w;
        knot->out_tangent.z = source->out_tangent.z;
        source = source->next;
    }
    clone->step = step;
    clone->height = height;
    clone->drop = drop;
    clone->closed = closed;
    GenPoints();
    return clone;
}

void SplineHelper::PostLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
    for (SplineObject *spline = first_object; spline != NULL; spline = spline->next)
        spline->GenPoints();
}

void SplineHelper::Process(void *object, EdInputContext &input) {
    SplineObject *spline = static_cast<SplineObject *>(object);
    if (input.GetHold(3) != 0.0f)
        return;
    if (spline->points.block_count != 0)
        return;
    spline->GenPoints();
}

void SplineKnot::Smooth() {
    VuVec before = position;
    VuVec after = position;
    if (previous != NULL)
        before = previous->position;
    if (next != NULL)
        after = next->position;
    if (previous == NULL && next == NULL) {
        before = VuVec{0.0f, 0.0f, 0.0f, 0.0f};
        after = before;
    }
    f32 dx = (after.x - before.x) * 0.1f;
    f32 dy = (after.y - before.y) * 0.1f;
    f32 dz = (after.z - before.z) * 0.1f;
    VuVec out{position.x + dx, position.y + dy, position.z + dz, 0.0f};
    VuVec in{position.x - dx, position.y - dy, position.z - dz, 0.0f};
    if (spline != NULL) {
        spline->DropPoint(out);
        spline->DropPoint(in);
    }
    in_tangent = in;
    out_tangent = out;
}

void SplineHelper::cbEdSplineSmoothKnot(eduimenu_s *, eduiitem_s *, u32) {
    SplineObject *changed_splines[63];
    i32 changed_count = 0;
    for (ClassObjectListEntry *selected = theClassEditor.selected_objects.first; selected != NULL;
         selected = selected->next) {
        if (selected->ed_class != theKnotHelper.object_class)
            continue;
        SplineKnot *knot = static_cast<SplineKnot *>(selected->object);
        knot->Smooth();
        if (!theSplineHelper.auto_generate_points || knot->spline == NULL)
            continue;
        i32 index = 0;
        while (index < changed_count && changed_splines[index] != knot->spline)
            ++index;
        if (index == changed_count)
            changed_splines[changed_count++] = knot->spline;
    }
    for (i32 index = 0; index < changed_count; ++index)
        changed_splines[index]->GenPoints();
}

void SplineObject::SmoothKnots() {
    for (SplineKnot *knot = knots.first; knot != NULL; knot = knot->next)
        knot->Smooth();
}

void SplineHelper::cbEdSplineSmoothSpline(eduimenu_s *, eduiitem_s *, u32) {
    for (ClassObjectListEntry *selected = theClassEditor.selected_objects.first; selected != NULL;
         selected = selected->next) {
        if (selected->ed_class == theSplineHelper.object_class) {
            SplineObject *spline = static_cast<SplineObject *>(selected->object);
            spline->SmoothKnots();
            if (theSplineHelper.auto_generate_points)
                spline->GenPoints();
        }
    }
}

void *KnotHelper::CreateObject(void *, i32, i32) {
    SplineKnot *selected_knot = NULL;
    SplineObject *spline = NULL;
    for (ClassObjectListEntry *selected = theClassEditor.selected_objects.first; selected != NULL;
         selected = selected->next) {
        if (spline == NULL && selected->ed_class == theSplineHelper.object_class)
            spline = static_cast<SplineObject *>(selected->object);
        if (selected_knot == NULL && selected->ed_class == theKnotHelper.object_class)
            selected_knot = static_cast<SplineKnot *>(selected->object);
        if (spline != NULL && selected_knot != NULL)
            break;
    }
    if (spline == NULL) {
        spline = new (theMemoryManager.AllocPool(sizeof(SplineObject), 1)) SplineObject();
        theClassEditor.MakeUniqueName("NewSpline", spline->name, sizeof(spline->name));
        spline->previous = theSplineHelper.last_object;
        if (theSplineHelper.last_object != NULL)
            theSplineHelper.last_object->next = spline;
        else
            theSplineHelper.first_object = spline;
        theSplineHelper.last_object = spline;
        ++theSplineHelper.object_count;
    }
    SplineKnot *knot = static_cast<SplineKnot *>(theMemoryManager.AllocPool(sizeof(SplineKnot), 1));
    if (knot != NULL) {
        knot->next = NULL;
        knot->previous = NULL;
    }
    knot->led_file = Placeable::CurrentLedFile;
    knot->spline = spline;
    if (selected_knot == NULL) {
        knot->next = NULL;
        knot->previous = spline->knots.last;
        if (spline->knots.last != NULL)
            spline->knots.last->next = knot;
        else
            spline->knots.first = knot;
        spline->knots.last = knot;
    } else {
        knot->next = selected_knot->next;
        knot->previous = selected_knot;
        if (selected_knot->next == NULL)
            spline->knots.last = knot;
        else
            selected_knot->next->previous = knot;
        selected_knot->next = knot;
    }
    ++spline->knots.count;
    knot->Smooth();
    return knot;
}

void ClassEditor::SnapPoint(VuVec &point) {
    if (static_cast<u32>(snap_mode - 1) > 1)
        return;
    VuVec candidate = point;
    VuVec ray = snap_ray;
    NuVecNorm(reinterpret_cast<NUVEC *>(&ray), reinterpret_cast<NUVEC *>(&ray));
    ray.y *= -snap_distance;
    ray.x *= -snap_distance;
    ray.z = -snap_distance * ray.z;
    candidate.w = 0.0f;
    candidate.x = ray.x + point.x;
    candidate.y = ray.y + point.y;
    candidate.z = ray.z + point.z;
    if (EdTerrRay(candidate, snap_ray))
        point = candidate;
}

void EdRefKnot::SetMemberData(void *object, i32 type, void *data, i32 data_size, i16 *) {
    SplineKnot *knot = static_cast<SplineKnot *>(object);
    CheckType(type);
    switch (member_offset) {
        case static_cast<i32>(0x80000001):
        case static_cast<i32>(0x80000002):
            return;
        case static_cast<i32>(0x80000003): {
            VuVec *position = static_cast<VuVec *>(data);
            f32 dy = position->y - knot->position.y;
            f32 dx = position->x - knot->position.x;
            f32 dz = position->z - knot->position.z;
            knot->position.y += dy;
            knot->position.x += dx;
            knot->position.z += dz;
            knot->position.w = 1.0f;
            if (!theClassEditor.IsSelectedObject(knot, theKnotHelper.in_tangent_ref)) {
                knot->in_tangent.x += dx;
                knot->in_tangent.y += dy;
                knot->in_tangent.z += dz;
                theClassEditor.SnapPoint(knot->in_tangent);
            }
            if (!theClassEditor.IsSelectedObject(knot, theKnotHelper.out_tangent_ref)) {
                knot->out_tangent.x += dx;
                knot->out_tangent.y += dy;
                knot->out_tangent.z += dz;
                theClassEditor.SnapPoint(knot->out_tangent);
            }
            break;
        }
        case static_cast<i32>(0x80000004): {
#if defined(__SSE__)
            const __m64 *source = reinterpret_cast<const __m64 *>(data);
            __m128 lanes = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), source), source + 1);
            _mm_storel_pi(reinterpret_cast<__m64 *>(&knot->in_tangent), lanes);
            _mm_storeh_pi(reinterpret_cast<__m64 *>(&knot->in_tangent) + 1, lanes);
#else
            knot->in_tangent = *static_cast<VuVec *>(data);
#endif
            break;
        }
        case static_cast<i32>(0x80000005): {
#if defined(__SSE__)
            const __m64 *source = reinterpret_cast<const __m64 *>(data);
            __m128 lanes = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), source), source + 1);
            _mm_storel_pi(reinterpret_cast<__m64 *>(&knot->out_tangent), lanes);
            _mm_storeh_pi(reinterpret_cast<__m64 *>(&knot->out_tangent) + 1, lanes);
#else
            knot->out_tangent = *static_cast<VuVec *>(data);
#endif
            break;
        }
        default:
            EdRef::GetMemberData(object, type, data, data_size);
            return;
    }
    if (theSplineHelper.auto_generate_points && knot->spline) {
        knot->spline->points.Clear();
    }
}

i32 EdManipulator::Process(EdInputContext &input, ClassObjectList &selected) {
    const f32 step = input.GetHold(21) != 0.0f ? 3.0f : 1.0f;
    if (input.GetHold(14) != 0.0f)
        Scale += step * input.delta_time;
    if (input.GetHold(15) != 0.0f) {
        if (Scale <= 0.1f)
            Scale = 0.1f;
        else
            Scale -= step * input.delta_time;
    }

    // The original keeps this vector in a 16-byte aligned slot of a realigned frame.
    VuVec average __attribute__((aligned(16)));
    f32 distance = 0.0f;
    VuVec *ray_origin = reinterpret_cast<VuVec *>(input.reserved_00 + 0x20);
    if (selected.GetAveragePosition(average) != 0) {
        VuVec displacement = {average.x - ray_origin->x, average.y - ray_origin->y, average.z - ray_origin->z, 0.0f};
        distance = NuVecMag(reinterpret_cast<NUVEC *>(&displacement));
    }
    VuVec direction = *reinterpret_cast<VuVec *>(input.reserved_00 + 0x30);
    NuVecNorm(reinterpret_cast<NUVEC *>(&direction), reinterpret_cast<NUVEC *>(&direction));
    VuVec *last_point = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x30);
    VuVec point = {ray_origin->x + direction.x * distance, ray_origin->y + direction.y * distance,
                   ray_origin->z + direction.z * distance, 0.0f};
    VuVec *delta = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x40);
    VuVec *cursor = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x50);
    if (input.GetPress(3) != 0.0f) {
        delta->x = 0.0f;
        delta->y = 0.0f;
        delta->z = 0.0f;
        delta->w = 1.0f;
        last_point->x = point.x;
        last_point->y = point.y;
        last_point->z = point.z;
        last_point->w = 0.0f;
    } else {
        delta->w = 0.0f;
        last_point->w = 0.0f;
        delta->x = point.x - last_point->x;
        delta->y = point.y - last_point->y;
        delta->z = point.z - last_point->z;
        last_point->x = point.x;
        last_point->y = point.y;
        last_point->z = point.z;
    }
    VuVec *ray_cursor = reinterpret_cast<VuVec *>(input.reserved_00);
    cursor->x = ray_cursor->x;
    cursor->y = ray_cursor->y;
    cursor->z = ray_cursor->z;
    cursor->w = ray_cursor->w;
    if (EdTerrRay(*cursor, *reinterpret_cast<VuVec *>(input.reserved_00 + 0x30)) == 0) {
        cursor->x = last_point->x;
        cursor->y = last_point->y;
        cursor->z = last_point->z;
        cursor->w = last_point->w;
    }
    last_point->w = 1.0f;
    delta->w = 1.0f;
    cursor->w = 1.0f;
    *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x08) = 0;
    for (ClassObjectListEntry *entry = selected.first; entry != NULL; entry = entry->next) {
        if (entry->ed_class->FindTypeRef(selected_attribute, 1) != NULL) {
            *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x08) = 1;
            break;
        }
    }
    return 0;
}

f32 LineToPointDistance(VuVec &origin, VuVec &direction, VuVec &point, VuVec *closest) {
    f32 length = NuVecMag(reinterpret_cast<NUVEC *>(&direction));
    f32 inverse_length = 1.0f / length;
    f32 nx = direction.x * inverse_length;
    f32 ny = direction.y * inverse_length;
    f32 nz = direction.z * inverse_length;
    f32 projection = (point.x - origin.x) * nx + (point.y - origin.y) * ny + (point.z - origin.z) * nz;

    VuVec on_segment = origin;
    if (projection > 0.0f) {
        if (!(projection >= length)) {
            on_segment.x += nx * projection;
            on_segment.y += ny * projection;
            on_segment.z += nz * projection;
        } else {
            on_segment.x += direction.x;
            on_segment.y += direction.y;
            on_segment.z += direction.z;
        }
        on_segment.w = 0.0f;
    }

    VuVec difference = {on_segment.x - point.x, on_segment.y - point.y, on_segment.z - point.z, 0.0f};
    f32 distance = NuVecMag(reinterpret_cast<NUVEC *>(&difference));
    if (closest != NULL)
        *closest = on_segment;
    return distance;
}

f32 KnotHelper::DistanceToObject(VuVec &origin, VuVec &direction, void *object, EdRef **reference) {
    SplineKnot *knot = static_cast<SplineKnot *>(object);
    f32 distance = LineToPointDistance(origin, direction, knot->position, NULL);
    EdRef *nearest = position_ref;
    f32 in_distance = LineToPointDistance(origin, direction, knot->in_tangent, NULL);
    if (in_distance < distance) {
        distance = in_distance;
        nearest = in_tangent_ref;
    }
    f32 out_distance = LineToPointDistance(origin, direction, knot->out_tangent, NULL);
    if (out_distance < distance) {
        distance = out_distance;
        nearest = out_tangent_ref;
    }
    distance -= 0.25f * EdManipulator::Scale;
    if (reference != NULL)
        *reference = nearest;
    return distance < 0.0f ? 0.0f : distance;
}

// The original realigns the stack for its local locator vectors.
__attribute__((force_align_arg_pointer)) i32 EdManipulator::SelectAxis(EdInputContext &input, VuVec &origin,
                                                                       VuVec &first_axis, VuVec &second_axis,
                                                                       VuMtx *matrix) {
    VuVec locators[8];
    first_axis.y = 0.0f;
    first_axis.z = 0.0f;
    first_axis.w = 1.0f;
    first_axis.x = 0.0f;
    second_axis.x = 0.0f;
    second_axis.y = 0.0f;
    second_axis.w = 1.0f;
    second_axis.z = 0.0f;
    GetAxisLocators(origin, locators, matrix);

    VuVec *ray_direction = reinterpret_cast<VuVec *>(input.reserved_00 + 0x30);
    VuVec *ray_origin = reinterpret_cast<VuVec *>(input.reserved_00 + 0x20);
    i32 nearest = 0;
    f32 nearest_distance = __FLT_MAX__;
#define CHECK_AXIS(index, radius)                                                                                      \
    {                                                                                                                  \
        f32 distance = LineToPointDistance(*ray_origin, *ray_direction, locators[index], NULL);                        \
        if (distance < (radius) * Scale && distance < nearest_distance) {                                              \
            nearest_distance = distance;                                                                               \
            nearest = index;                                                                                           \
        }                                                                                                              \
    }
    CHECK_AXIS(1, 0.25f);
    CHECK_AXIS(2, 0.25f);
    CHECK_AXIS(3, 0.25f);
    CHECK_AXIS(4, 0.1f);
    CHECK_AXIS(5, 0.1f);
    CHECK_AXIS(6, 0.1f);
    CHECK_AXIS(7, 0.1f);

    i32 *selected_axis = reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x0c);

    switch (nearest) {
        case 1:
            first_axis = VuVec(1.0f, 0.0f, 0.0f, 1.0f);
            break;
        case 2:
            first_axis = VuVec(0.0f, 1.0f, 0.0f, 1.0f);
            break;
        case 3:
            first_axis = VuVec(0.0f, 0.0f, 1.0f, 1.0f);
            break;
        case 4:
            first_axis = VuVec(1.0f, 0.0f, 0.0f, 1.0f);
            second_axis = VuVec(0.0f, 1.0f, 0.0f, 1.0f);
            break;
        case 5:
            first_axis = VuVec(1.0f, 0.0f, 0.0f, 1.0f);
            second_axis = VuVec(0.0f, 0.0f, 1.0f, 1.0f);
            break;
        case 6:
            first_axis = VuVec(0.0f, 1.0f, 0.0f, 1.0f);
            second_axis = VuVec(0.0f, 0.0f, 1.0f, 1.0f);
            break;
        case 7:
            first_axis = VuVec(0.0f, 1.0f, 0.0f, 1.0f);
            second_axis = VuVec(0.0f, 0.0f, 1.0f, 1.0f);
            break;
        default:
            break;
    }
    if (matrix != NULL) {
        const NUMTX &transform = matrix->matrix;
        f32 first_x = first_axis.x;
        first_axis.z = first_x * transform.m02 + first_axis.y * transform.m12 + first_axis.z * transform.m22;
        first_axis.y = first_x * transform.m01 + first_axis.y * transform.m11 + first_axis.z * transform.m21;
        first_axis.x = first_x * transform.m00 + first_axis.y * transform.m10 + first_axis.z * transform.m20;

        f32 second_x = second_axis.x;
        f32 second_z = second_axis.z;
        second_axis.y = second_x * transform.m01 + second_axis.y * transform.m11 + second_z * transform.m21;
        second_axis.z = second_x * transform.m02 + second_axis.y * transform.m12 + second_z * transform.m22;
        second_axis.x = second_x * transform.m00 + second_axis.y * transform.m10 + second_z * transform.m20;

        NuVecNorm(reinterpret_cast<NUVEC *>(&first_axis), reinterpret_cast<NUVEC *>(&first_axis));
        NuVecNorm(reinterpret_cast<NUVEC *>(&second_axis), reinterpret_cast<NUVEC *>(&second_axis));
    }
    if (input.GetPress(3) != 0.0f) {
        *selected_axis = nearest;
        VuVec *selected_first = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x10);
        VuVec *selected_second = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x20);
        selected_first->x = first_axis.x;
        selected_first->y = first_axis.y;
        selected_first->z = first_axis.z;
        selected_first->w = first_axis.w;
        selected_second->x = second_axis.x;
        selected_second->y = second_axis.y;
        selected_second->z = second_axis.z;
        selected_second->w = second_axis.w;
    } else if (input.GetHold(3) != 0.0f) {
        nearest = *selected_axis;
        const VuVec *selected_first = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x10);
        const VuVec *selected_second = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x20);
        first_axis.z = selected_first->z;
        first_axis.y = selected_first->y;
        first_axis.x = selected_first->x;
        first_axis.w = selected_first->w;
        second_axis.x = selected_second->x;
        second_axis.y = selected_second->y;
        second_axis.z = selected_second->z;
        second_axis.w = selected_second->w;
    } else {
        *selected_axis = 0;
        *reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x10) = VuVec(0.0f, 0.0f, 0.0f, 1.0f);
        *reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x20) = VuVec(0.0f, 0.0f, 0.0f, 1.0f);
    }
    return nearest;
}

// The original realigns the stack for its matrix and vector locals.
__attribute__((force_align_arg_pointer)) i32 EdManScale::Process(EdInputContext &input, ClassObjectList &selected) {
    VuVec projected;
    EdManipulator::Process(input, selected);
    VuVec average;
    if (selected.GetAveragePosition(average) == 0)
        return 0;

    VuMtx transform;
    EdMember member;
    i32 matrix_type = EdType_VuMtx;
    ClassObjectListEntry *first = selected.first;
    if (first->reference == NULL ||
        first->reference->GetAttributeData(first->object, 0x10, matrix_type, &transform, 0) == 0) {
        if (first->ed_class->FindMember(&member, first->object, 0x10, 1) != 0)
            member.reference->GetAttributeData(member.object, 0x10, matrix_type, &transform, 0);
    }
    VuVec second_axis;
    VuVec first_axis;
    i32 axis = SelectAxis(input, average, first_axis, second_axis, &transform);
    theLevelEditor.field_0x2c = AxisColour[axis];
    if (input.GetHold(3) != 0.0f) {
        VuVec const &delta = *reinterpret_cast<VuVec const *>(reinterpret_cast<u8 *>(this) + 0x40);
        for (ClassObjectListEntry *entry = selected.first; entry != NULL; entry = entry->next) {
            average = VuVec(0.0f, 0.0f, 0.0f, 1.0f);
            matrix_type = EdType_VuMtx;
            if (__builtin_expect(entry->reference == NULL ||
                entry->reference->GetAttributeData(entry->object, 0x20, matrix_type, &transform, 0) == 0, 0)) {
                if (__builtin_expect(entry->ed_class->FindMember(&member, entry->object, 0x20, 1) != 0, 1))
                    member.reference->GetAttributeData(member.object, 0x20, matrix_type, &transform, 0);
            }

            f32 scale_x = 1.0f;
            f32 scale_y = 1.0f;
            f32 scale_z = 1.0f;
            switch (axis) {
                case 1:
                case 2:
                case 3: {
                    NUMTX &matrix = transform.matrix;

                    projected.x = matrix.m00 * first_axis.x + matrix.m10 * first_axis.y + matrix.m20 * first_axis.z;
                    projected.y = matrix.m01 * first_axis.x + matrix.m11 * first_axis.y + matrix.m21 * first_axis.z;
                    projected.z = matrix.m02 * first_axis.x + matrix.m12 * first_axis.y + matrix.m22 * first_axis.z;
                    f32 magnitude = NuVecMag(reinterpret_cast<NUVEC *>(&projected));
                    f32 scaled_magnitude = Scale * magnitude;
                    f32 movement = delta.x * first_axis.x + delta.y * first_axis.y + delta.z * first_axis.z;
                    if (movement == 0.0f)
                        continue;
                    NuVecInvMtxRotate(reinterpret_cast<NUVEC *>(&first_axis), reinterpret_cast<NUVEC *>(&first_axis),
                                      &matrix);
                    NuVecNorm(reinterpret_cast<NUVEC *>(&first_axis), reinterpret_cast<NUVEC *>(&first_axis));
                    f32 change = (scaled_magnitude + movement) / scaled_magnitude - 1.0f;
                    scale_y = first_axis.y * change + 1.0f;
                    scale_x = first_axis.x * change + 1.0f;
                    scale_z = first_axis.z * change + 1.0f;
                    break;
                }
                case 4:
                case 5:
                case 6: {
                    NUMTX &matrix = transform.matrix;
                    VuVec projected;
                    projected.x = matrix.m00 * first_axis.x + matrix.m10 * first_axis.y + matrix.m20 * first_axis.z;
                    projected.y = matrix.m01 * first_axis.x + matrix.m11 * first_axis.y + matrix.m21 * first_axis.z;
                    projected.z = matrix.m02 * first_axis.x + matrix.m12 * first_axis.y + matrix.m22 * first_axis.z;
                    f32 magnitude = NuVecMag(reinterpret_cast<NUVEC *>(&projected));
                    f32 scaled_magnitude = Scale * magnitude;
                    f32 movement = delta.x * first_axis.x + delta.y * first_axis.y + delta.z * first_axis.z;
                    movement += delta.x * second_axis.x + delta.y * second_axis.y + delta.z * second_axis.z;
                    if (movement == 0.0f)
                        continue;
                    NuVecInvMtxRotate(reinterpret_cast<NUVEC *>(&first_axis), reinterpret_cast<NUVEC *>(&first_axis),
                                      &matrix);
                    NuVecNorm(reinterpret_cast<NUVEC *>(&first_axis), reinterpret_cast<NUVEC *>(&first_axis));
                    f32 change = (scaled_magnitude + movement) / scaled_magnitude - 1.0f;
                    scale_y = first_axis.y * change + second_axis.y * change + 1.0f;
                    scale_z = first_axis.z * change + second_axis.z * change + 1.0f;
                    scale_x = first_axis.x * change + second_axis.x * change + 1.0f;
                    break;
                }
                case 7: {
                    f32 movement = input.Get(1) - input.Get(0) + input.Get(2);
                    if (movement == 0.0f)
                        continue;
                    scale_x = scale_y = scale_z = movement * 0.005f + 1.0f;
                    break;
                }
                default:
                    continue;
            }
            NUMTX &matrix = transform.matrix;
            average = VuVec(matrix.m30, matrix.m31, matrix.m32, matrix.m33);
            const NUMTX original = matrix;
            const f32 zero = 0.0f;
            matrix.m02 = scale_x * original.m02 + zero * original.m12 + zero * original.m22;
            matrix.m00 = scale_x * original.m00 + zero * original.m10 + zero * original.m20;
            matrix.m10 = zero * original.m00 + scale_y * original.m10 + zero * original.m20;
            matrix.m01 = scale_x * original.m01 + zero * original.m11 + zero * original.m21;
            matrix.m11 = zero * original.m01 + scale_y * original.m11 + zero * original.m21;
            matrix.m12 = zero * original.m02 + scale_y * original.m12 + zero * original.m22;
            matrix.m20 = zero * original.m00 + zero * original.m10 + scale_z * original.m20;
            matrix.m21 = zero * original.m01 + zero * original.m11 + scale_z * original.m21;
            matrix.m22 = zero * original.m02 + zero * original.m12 + scale_z * original.m22;
            matrix.m03 = matrix.m13 = matrix.m23 = 0.0f;
            matrix.m30 = average.x;
            matrix.m31 = average.y;
            matrix.m32 = average.z;
            matrix.m33 = average.w;
            matrix_type = EdType_VuMtx;
            if (entry->reference == NULL ||
                entry->reference->SetAttributeData(entry->object, 0x20, matrix_type, &transform, 0) == 0) {
                if (entry->ed_class->FindMember(&member, entry->object, 0x20, 1) != 0)
                    member.reference->SetAttributeData(member.object, 0x20, matrix_type, &transform, 0);
            }
        }
    }
    return axis != 0;
}

__attribute__((force_align_arg_pointer)) i32 EdManMove::Process(EdInputContext &input, ClassObjectList &selected) {
    VuVec position;
    EdManipulator::Process(input, selected);
    VuVec average;
    if (selected.GetAveragePosition(average) == 0)
        return 0;
    VuVec first_axis;
    VuVec second_axis;
    i32 axis = SelectAxis(input, average, first_axis, second_axis, NULL);
    theLevelEditor.field_0x2c = AxisColour[axis];
    if (input.GetHold(3) == 0.0f && input.GetHold(38) == 0.0f)
        return axis != 0;
    const VuVec *delta = reinterpret_cast<VuVec const *>(reinterpret_cast<u8 *>(this) + 0x40);
    for (ClassObjectListEntry *entry = selected.first; entry != NULL; entry = entry->next) {
        position = VuVec_Zero;
        get_manipulator_attribute(entry, 8, EdType_VuVec, &position);
        if (axis == 7) {
            if (input.GetHold(38) == 0.0f)
                continue;
            memcpy(&position, theLevelEditor.background_colour, sizeof(position));
        } else {
            f32 amount = delta->x * first_axis.x + delta->y * first_axis.y + delta->z * first_axis.z;
            if (amount != 0.0f) {
                position.y += first_axis.y * amount;
                position.z += first_axis.z * amount;
                position.x += first_axis.x * amount;
            }
            if (axis >= 4) {
                f32 second_amount = delta->x * second_axis.x + delta->y * second_axis.y + delta->z * second_axis.z;
                if (second_amount != 0.0f) {
                    position.y += second_axis.y * second_amount;
                    position.x += second_axis.x * second_amount;
                    position.z += second_axis.z * second_amount;
                } else if (amount == 0.0f) {
                    continue;
                }
            } else if (amount == 0.0f) {
                continue;
            }
        }
        theClassEditor.SnapPoint(position);
        set_manipulator_attribute(entry, 8, EdType_VuVec, &position);
    }
    return axis != 0;
}

f32 EdClassInterface::DistanceToObject(VuVec &origin, VuVec &direction, void *object, EdRef **reference) {
    VuVec position;
    f32 radius = 1.0f;
    EdMember member;
    f32 distance = 3.402823466e38f;
    if (object_class->FindMember(&member, object, 8, 1)) {

        member.reference->GetAttributeData(member.object, 8, EdType_VuVec, &position, 0);
        if (object_class->FindMember(&member, object, 0x40, 1))
            member.reference->GetAttributeData(member.object, 0x40, EdType_Float, &radius, 0);
        distance = LineToPointDistance(origin, direction, position, NULL) - radius;
        if (distance < 0.0f)
            distance = 0.0f;
    }
    if (reference != NULL)
        *reference = NULL;
    return distance;
}

f32 LineToPlaneDistance(VuVec &origin, VuVec &direction, VuVec &plane) {
    f32 first = origin.x * plane.x + origin.y * plane.y + origin.z * plane.z + plane.w;
    f32 second = (origin.x + direction.x) * plane.x + (origin.y + direction.y) * plane.y +
                 (origin.z + direction.z) * plane.z + plane.w;
    if (first < 0.0f && second < 0.0f) {
        return first > second ? first : second;
    }
    if (first > 0.0f && second > 0.0f) {
        return first < second ? first : second;
    }
    return 0.0f;
}

i32 LineToPlaneIntersecion(VuVec &origin, VuVec &direction, VuVec &plane, VuVec *intersection) {
    f32 start_distance = origin.x * plane.x + origin.y * plane.y + origin.z * plane.z + plane.w;
    f32 end_distance = (origin.x + direction.x) * plane.x + (origin.y + direction.y) * plane.y +
                       (origin.z + direction.z) * plane.z + plane.w;
    if (!(start_distance * end_distance < 0.0f))
        return 0;
    if (intersection != NULL) {
        f32 t = -start_distance / (end_distance - start_distance);
        intersection->x = direction.x * t;
        intersection->y = direction.y * t;
        intersection->z = direction.z * t;
        intersection->w = 0.0f;
        intersection->x += origin.x;
        intersection->y += origin.y;
        intersection->z += origin.z;
    }
    return 1;
}

i32 LineToSphereIntersection(VuVec &origin, VuVec &direction, VuVec &center, f32 radius, VuVec *far_intersection,
                             VuVec *near_intersection) {
    f32 qx = (origin.x + direction.x) - origin.x;
    f32 qy = (origin.y + direction.y) - origin.y;
    f32 qz = (origin.z + direction.z) - origin.z;
    f32 a = (qx * qx + qy * qy) + qz * qz;
    if (NuFabs(a) < 1.1920928955078125e-7f)
        return 0;
    f32 dx = origin.x - center.x;
    f32 dy = origin.y - center.y;
    f32 dz = origin.z - center.z;
    f32 projection = (dx * qx + dy * qy) + dz * qz;
    f32 b = projection + projection;
    f32 center_squared = (center.x * center.x + center.y * center.y) + center.z * center.z;
    f32 origin_squared = (origin.x * origin.x + origin.y * origin.y) + origin.z * origin.z;
    f32 origin_center = (origin.x * center.x + origin.y * center.y) + origin.z * center.z;
    f32 c = ((center_squared + origin_squared) - (origin_center + origin_center)) - radius * radius;
    f32 discriminant = b * b - (4.0f * a) * c;
    if (discriminant < 0.0f)
        return 0;
    f32 far_root = NuFsqrt(discriminant);
    f32 denominator = a + a;
    f32 far_t = (far_root - b) / denominator;
    f32 near_root = NuFsqrt(discriminant);
    f32 near_t = (-b - near_root) / denominator;
    if (far_intersection != NULL) {
        *far_intersection =
            VuVec(origin.x + direction.x * far_t, origin.y + direction.y * far_t, origin.z + direction.z * far_t, 0.0f);
    }
    if (near_intersection != NULL) {
        *near_intersection = VuVec(origin.x + direction.x * near_t, origin.y + direction.y * near_t,
                                   origin.z + direction.z * near_t, 0.0f);
    }
    return 1;
}

i32 EdManipulator::SelectRotator(EdInputContext &input, VuVec &center, VuVec &plane) {
    // The original keeps the sphere hits and axis planes in one block whose address escapes.
    struct {
        VuVec far_point;
        VuVec near_point;
        VuVec normals[3];
    } hits;
    VuVec &near_point = hits.near_point;
    f32 scale = Scale;
    i32 *selected_axis = reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x0c);
    i32 *start_angle = reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x60);
    i32 *last_angle = reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x64);
    i32 *angle_delta = reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x68);
    VuVec *selected_plane = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x10);
    VuVec *secondary_plane = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x20);
    f32 pressed = input.GetPress(3);
    if (pressed != 0.0f || input.GetHold(3) == 0.0f) {
        VuVec &far_point = hits.far_point;
        VuVec *normals = hits.normals;
        normals[0] = VuVec(1.0f, 0.0f, 0.0f, -center.x);
        normals[1] = VuVec(0.0f, 1.0f, 0.0f, -center.y);
        normals[2] = VuVec(0.0f, 0.0f, 1.0f, -center.z);
        i32 axis = 0;
        VuVec chosen;
        f32 nearest_distance = __FLT_MAX__;
        i32 angle = 0;
        if (LineToSphereIntersection(*reinterpret_cast<VuVec *>(input.reserved_00 + 0x20),
                                     *reinterpret_cast<VuVec *>(input.reserved_00 + 0x30), center, scale + 0.01f,
                                     &far_point, &near_point) != 0) {
            SpherePos1.x = far_point.x;
            SpherePos1.y = far_point.y;
            SpherePos1.z = far_point.z;
            SpherePos1.w = far_point.w;
            SpherePos2.x = near_point.x;
            SpherePos2.y = near_point.y;
            SpherePos2.z = near_point.z;
            SpherePos2.w = near_point.w;
            VuVec screen_point;
            VuVec projection;
            VuVec difference;
#define CHECK_ROTATOR(point, normal, candidate)                                                                        \
    {                                                                                                                  \
        f32 distance_to_plane = point.x * normal.x + point.y * normal.y + point.z * normal.z + normal.w;               \
        projection = VuVec(point.x - normal.x * distance_to_plane, point.y - normal.y * distance_to_plane,             \
                           point.z - normal.z * distance_to_plane, 0.0f);                                              \
        NuCameraTransformScreenClip(&projection.xyz, &projection.xyz, 1, NULL);                                        \
        NuCameraTransformScreenClip(&screen_point.xyz, &point.xyz, 1, NULL);                                           \
        difference.x = screen_point.x - projection.x;                                                                  \
        difference.y = screen_point.y - projection.y;                                                                  \
        difference.z = screen_point.z - projection.z;                                                                  \
        f32 distance = NuVecMag(&difference.xyz);                                                                      \
        if (distance < 0.05f && distance < nearest_distance) {                                                         \
            nearest_distance = distance;                                                                               \
            chosen = point;                                                                                            \
            axis = candidate;                                                                                          \
        }                                                                                                              \
    }
            for (i32 candidate = 0; candidate < 3; ++candidate) {
                CHECK_ROTATOR(far_point, normals[candidate], candidate + 1);
                CHECK_ROTATOR(near_point, normals[candidate], candidate + 1);
            }
            if (axis != 0) {
                f32 x = chosen.x - center.x;
                f32 y = chosen.y - center.y;
                f32 z = chosen.z - center.z;
                switch (axis) {
                    case 1:
                        plane = VuVec(1.0f, 0.0f, 0.0f, 1.0f);
                        plane.w = -center.x;
                        angle = NuAtan2DA(y, z);
                        break;
                    case 2:
                        plane = VuVec(0.0f, 1.0f, 0.0f, 1.0f);
                        plane.w = -center.y;
                        angle = NuAtan2DA(x, -z);
                        break;
                    case 3:
                        plane = VuVec(0.0f, 0.0f, 1.0f, 1.0f);
                        plane.w = -center.z;
                        angle = NuAtan2DA(x, y);
                        break;
                }
            }
        }
        if (input.GetPress(3) != 0.0f) {
            *selected_axis = axis;
            selected_plane->x = plane.x;
            selected_plane->y = plane.y;
            selected_plane->z = plane.z;
            selected_plane->w = plane.w;
            *start_angle = angle;
            *last_angle = angle;
            *angle_delta = 0;
            return axis;
        }
        *selected_axis = 0;
        *start_angle = 0;
        *last_angle = 0;
        *angle_delta = 0;
        *selected_plane = VuVec_Zero;
        *secondary_plane = VuVec_Zero;
        return axis;
    }
    i32 axis = *selected_axis;
    plane.x = selected_plane->x;
    plane.y = selected_plane->y;
    plane.z = selected_plane->z;
    plane.w = selected_plane->w;
    VuVec intersection;
    if (LineToPlaneIntersecion(*reinterpret_cast<VuVec *>(input.reserved_00 + 0x20),
                               *reinterpret_cast<VuVec *>(input.reserved_00 + 0x30), plane, &intersection) == 0) {
        *angle_delta = 0;
        return axis;
    }
    f32 x = intersection.x - center.x;
    f32 y = intersection.y - center.y;
    f32 z = intersection.z - center.z;
    i32 angle = 0;
    switch (axis) {
        case 1:
            angle = NuAtan2DA(y, z);
            break;
        case 2:
            angle = NuAtan2DA(x, -z);
            break;
        case 3:
            angle = NuAtan2DA(x, y);
            break;
    }
    i32 delta = *last_angle - angle;
    *last_angle = angle;
    delta &= 0xffff;
    if (delta >= 0x8000)
        delta -= 0x10000;
    *angle_delta = delta;
    return axis;
}

i32 EdManRotate::Process(EdInputContext &input, ClassObjectList &selected) {
    EdManipulator::Process(input, selected);
    VuVec average;
    if (selected.GetAveragePosition(average) == 0)
        return 0;
    eduiSetCameraEnabled(1);
    if (input.pad != NULL && (input.pad->digital_buttons_pressed & 0x10) != 0) {
        if (++rotation_axis == 4)
            rotation_axis = 1;
    }
    if (input.GetHold(38) != 0.0f) {
        eduiSetCameraEnabled(0);
        i32 angle = static_cast<i32>(eduiGetAnalougePadValue(input.pad) * 400.0f);
        return RotateItem(input, selected, angle, rotation_axis);
    }
    if (input.GetHold(3) == 0.0f)
        return 0;
    VuVec axis;
    i32 selected_axis = SelectRotator(input, average, axis);
    theLevelEditor.field_0x2c = AxisColour[selected_axis];
    i32 angle = *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x68);
    return RotateItem(input, selected, angle, selected_axis);
}

void ClassEditor::Serialise(EdStream &stream) {
    if (stream.mode == 2) {
        theRegistry.Serialise(stream);
        theRegistry.SerialiseObjects(stream, NULL);
    }
    if (stream.mode == 1) {
        MemoryBuffer *saved = stream.memory_buffer;
        stream.memory_buffer = stream.secondary_buffer;
        EdRegistry *source = ::CreateObject<EdRegistry>(stream.secondary_buffer);
        source->Initialise(*stream.secondary_buffer->position, *stream.secondary_buffer->end, 50, 50, 10, 1);
        source->Serialise(stream);
        stream.memory_buffer = saved;
        theRegistry.SerialiseObjects(stream, source);
    }
}

