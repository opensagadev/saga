// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "globals.h"
#include "legoapi/core/config/cheat.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numusic/sfx.h"

#include <float.h>
#include <string.h>
#include <stdio.h>
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/numtx.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/render/fx/game_deb.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/core/input/timer.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nusound/nusound.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include <stdlib.h>
#include "legoapi/world/mission.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nucamera.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/misc/utilities.h"
#include <math.h>
#include "batman.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/render/fx/particles.h"
#include "legoapi/world/areas.h"
#include "legoapi/actions/character/streaks.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "gameframework/saveload.h"
#include "nu2api/nucore/nuvideo.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "legoapi/render/fx/edsplines.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include <new>
#include <stdint.h>
#include "nu2api/nucore/nulist.h"
#include "gameapi/ai/aisys/aiscript_types.h"
#include "editor/edpath.h"
#include "legoapi/actions/character/speederchase.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/numath/nuang.h"
#include "editor/aieditor_state.h"
#include "nu2api/numath/nuplane.h"
#include "editor/aieditor_settings.h"
#include "editor/edpath_types.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/characters/motion/contexts.h"
#include <stddef.h>
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include <cstdio>
#include <stdarg.h>
#include <GLES2/gl2.h>
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/menus/screens/movies.h"
#include "legoapi/misc/androidbatman.h"
#include <cfloat>
#include "legoapi/items/base/animpacket.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "gameapi/ai/aisys/aipath.h"
#include "nu2api/nucore/numechptr.hpp"
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
#include <ctype.h>
#include <limits.h>
#include "legoapi/world/levels/episodeI.h"

// Retail uses translation-unit-local helpers with compiler-inferred private
// calling conventions; keep their shared animation-mode call sites intact.

void Animate_POD(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    packet.requested_animation = 1;

    {
        if (__builtin_expect(!object->apiobj.player_controlled, 0)) {
            goto normal_animation;
        }
        const i8 index = object->apiobj.field_0x27c;
        if (index == -1 || (object->id != id_ANAKINSPOD && object->id != id_ANAKINSPODGREEN)) {
            goto normal_animation;
        }
        f32 delta = FRAMETIME * 30.0f;
        CHARACTERMODEL_s *model = object->apiobj.character_model;
        if (model->model_data_b[1] != NULL) {
            CHARACTERANIM_s *info = static_cast<CHARACTERANIM_s *>(model->model_data_a[1]);
            delta *= info->playback_rate / 30.0f;
        }
        pod_animtime[index] += delta;
        if (pod_animtime[index] >= podanimendframe) {
            pod_animtime[index] -= podanimendframe - 1.0f;
        }
        SetPodMergeAnims(&packet, index);
        return;
    }

normal_animation:
    if (PodSprint_InStartCountdown(WORLD) > 0.0f || PodRace_InStartCountdown(WORLD) != 0) {
        packet.requested_animation = 1;
    } else if (object->camera_shake_strength > 0.0f) {
        packet.requested_animation = 0x17;
    } else if (__builtin_expect(!object->apiobj.player_controlled, 0)) {
        packet.requested_animation = 3;
    } else if (PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA) {
        if (object->previous_block_animation != -1) {
            packet.requested_animation = object->previous_block_animation;
        }
    } else {
        packet.requested_animation = 0;
    }
    if (object->apiobj.character_model->model_data_b[packet.requested_animation] == NULL) {
        packet.requested_animation = 1;
    }
}

extern "C" {
    // Original @0x2c17d0. The non-quaternion ANI4 player uses a compact
    // four-samples-per-word curve stream. Only groups enabled by the node's
    // translation/rotation/scale flags occupy space in that stream.
} // extern "C"

// --- World-module helpers (kept with the WorldInfo API) ---

extern WORLDINFO WorldInfo[2];

struct bgprocinfo_s;

extern i32 LevFlag[4];

extern i32 LevFlag[4]; // shared per-level 16-byte state scratch

// LoadPerm1 is one of the few game-level entry points which wires together
// otherwise C-linkage engine subsystems.  Keep these declarations local: the
// individual subsystem TUs intentionally expose their original plain names.

struct AISYS_s;

// Retail keeps only the IPA-SRA clone, which receives &*layers.
#if defined(__i386__)
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone, regparm(1)))
#else
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone))
#endif

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

#if defined(__i386__)
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone, regparm(2), sseregparm))
#else
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone))
#endif

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

struct STATUSPACKET_s;
struct TouchHolder;
struct nuhspecial_s;

// Retail only keeps the IPA-SRA clone, which drops the unused fourth argument.
#if defined(__i386__)
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone))
#endif

#define ALPHA_GRID_VERTEX(vx, vy)                                                                                      \
    do {                                                                                                               \
        direction.x = (vx);                                                                                            \
        direction.y = -(vy) * camera.aspect;                                                                           \
        direction.z = adjacent;                                                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        NuVecMtxTransform(&direction, &direction, &camera.mtx);                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        brightness = direction.y * 0.5f + 0.5f;                                                                        \
        if (brightness <= near_angle)                                                                                  \
            brightness = near_scale * parameters->intensity;                                                           \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale * parameters->intensity;                                                            \
        else                                                                                                           \
            brightness = ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity; \
        if (parameters->directional) {                                                                                 \
            f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;       \
            if (angle < parameters->direction_near_angle) {                                                            \
                brightness += 128.0f * parameters->direction_far_scale;                                                \
            } else if (!(angle > parameters->direction_far_angle)) {                                                   \
                f32 blend = NuPowFast((angle - parameters->direction_near_angle) /                                     \
                                          (parameters->direction_far_angle - parameters->direction_near_angle),        \
                                      parameters->direction_bias);                                                     \
                brightness +=                                                                                          \
                    128.0f * ((1.0f - blend) * (parameters->direction_far_scale - parameters->direction_near_scale) +  \
                              parameters->direction_near_scale);                                                       \
            }                                                                                                          \
        }                                                                                                              \
        u32 colour = (static_cast<u32>(static_cast<u8>(128)) << 24) |                                                  \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 16) |                   \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 8) |                    \
                     static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness))));                            \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] =                                                                     \
                ((static_cast<i32>(colour) >> 1) & 0x7f7f7f) | (colour & 0xff000000);                                  \
        else                                                                                                           \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = colour;                                                             \
        NuPrim2DAddXYZ((vx), (vy), 0.0f);                                                                              \
    } while (0)

#define SWIPE_PRIM_UV(U, V)                                                                                            \
    do {                                                                                                               \
        PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);                      \
        if (!g_NuPrim_NeedsHalfUVs) {                                                                                  \
            vertex->float_uv[0] = (U);                                                                                 \
            vertex->float_uv[1] = (V);                                                                                 \
        } else {                                                                                                       \
            vertex->half_uv[0] = NuRndrFloatToHalf(U);                                                                 \
            vertex->half_uv[1] = NuRndrFloatToHalf(V);                                                                 \
        }                                                                                                              \
    } while (false)


// Forward declarations for local (static) game-object helper stubs.
struct GameObject_s;
struct nuvec_s;
struct WORLDINFO_s;

struct numtx_s;

// Forward declarations for local (static) character/gameplay helper stubs.
struct EDCREATURE_s;

// Original 0x120-byte data record @0x00666960. FixUpCharacters copies this
// canonical prefix into every character before character-specific overrides.
// These values cover the fully typed common movement and perception fields.

struct GIZMOPICKUP_s;
struct PART_s;

// --- Extern "C" block: functions with confirmed C linkage in original libTTapp.so ---


// The relocation tables are patched as plain words, so every fixup aliases
// cutscene->string_delta and re-reads it.
typedef isize __attribute__((may_alias)) NuGCutWord;
#define NUGCUT_WORD(field) (*reinterpret_cast<NuGCutWord *>(&(field)))
#define NUGCUT_RELOCATE(field, delta) \
    (NUGCUT_WORD(field) = NUGCUT_WORD(field) != 0 ? NUGCUT_WORD(field) + (delta) : 0)

// Retail keeps NuGCutSceneFixPtrs only as two constant-propagated clones:
// .44 relocates animations by anim_delta, .43 is the anim_delta == 0 case.
#if defined(__i386__)
#define NUGCUT_FIXPTRS_ATTR(n) __attribute__((noinline, noclone, regparm(n)))
#else
#define NUGCUT_FIXPTRS_ATTR(n) __attribute__((noinline, noclone))
#endif

struct instNUGCUTSCENE_s;

// Retail only keeps the IPA-SRA clone, which drops the unused frame argument.
#if defined(__i386__)
#define INST_LOCATOR_SYS_END_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define INST_LOCATOR_SYS_END_ATTR __attribute__((noinline, noclone))
#endif

extern "C" {
    extern i16 id_LANDSPEEDER;
}

// The route records use a dense route index, while the editor keeps sixteen
// independently switchable slots.  The original helper first marks both ends
// of every route connection, then builds the compact node lookup and matrix.
#if defined(__i386__)
#define AIPATHBUILD_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define AIPATHBUILD_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif

// Retail keeps several static helpers here only as GCC clones; these attributes
// reproduce the clone names and register conventions.
#if defined(__i386__)
#define AISYSALL_CLONE_ATTR1 __attribute__((noinline, noclone, regparm(1)))
#define AISYSALL_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#define AISYSALL_CLONE_ATTR2_SSE __attribute__((noinline, noclone, regparm(2), sseregparm))
#else
#define AISYSALL_CLONE_ATTR1 __attribute__((noinline, noclone))
#define AISYSALL_CLONE_ATTR2 __attribute__((noinline, noclone))
#define AISYSALL_CLONE_ATTR2_SSE __attribute__((noinline, noclone))
#endif

// Game-specific AI actions and conditions (registered via
// RegisterAIScriptActions / RegisterAIScriptConditions). These are stubbed to
// satisfy the symbol baseline; the action/condition logic itself is not
// decompiled. Each stub matches the mangled symbol of the original binary.

extern AIACTIONDEF api_aiactiondefs[55];

extern AICONDITIONDEF api_aiconditiondefs[50];

DECOMP_ASSERT(sizeof(api_aiactiondefs) == 0x294, "API action registry size");

DECOMP_ASSERT(sizeof(api_aiconditiondefs) == 0x258, "API condition registry size");

#if defined(__i386__)
#define AISCRIPT_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define AISCRIPT_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif

extern "C" {
} // extern "C"

#if defined(__i386__)
#define AI_SYS_STUBS_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define AI_SYS_STUBS_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif

struct nupad_s;


// Item colours shared by the AI editors.


// Retail splits the null checks into each caller and keeps the body as a
// partial-inlining clone; its connection searches are fully peeled.

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

// Retail keeps an unreferenced partial-inlining clone of pathEditorSetCamPos:
// it moves the editor camera to a position at the cursor's height.
#if defined(__i386__)
#define PATHEDITOR_SETCAMPOS_BODY_ATTR __attribute__((used, noinline, noclone, regparm(1)))
#else
#define PATHEDITOR_SETCAMPOS_BODY_ATTR __attribute__((used, noinline, noclone))
#endif

#if defined(__i386__)
#define EDANTINODE_REGPARM1 __attribute__((regparm(1)))
#else
#define EDANTINODE_REGPARM1
#endif


#if defined(__i386__)
#define CREATURE_EDITOR_REGPARM2 __attribute__((regparm(2)))
#else
#define CREATURE_EDITOR_REGPARM2
#endif

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

#if defined(__i386__)
#define LOCATOR_SET_BODY_ATTR __attribute__((noinline, optimize("no-tree-vectorize"), regparm(2)))
#else
#define LOCATOR_SET_BODY_ATTR __attribute__((noinline, optimize("no-tree-vectorize")))
#endif

static __used__ unsigned int AddLocatorToSet(EDLOCATORSET_s *set, EDLOCATOR_s *locator, EDLOCATOR_s *before);

#if defined(__i386__)
static __attribute__((noinline, optimize("no-tree-vectorize"), regparm(2))) unsigned int
#else
static __attribute__((noinline, optimize("no-tree-vectorize"))) unsigned int
#endif
AddLocatorToSetAtEnd(EDLOCATORSET_s *set, EDLOCATOR_s *locator) __asm__(
    "_ZL15AddLocatorToSetP14EDLOCATORSET_sP11EDLOCATOR_sS2_.constprop.131");

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];


}

namespace {

} // namespace


#define ALPHA_GRID_VERTEX(vx, vy)                                                                                      \
    do {                                                                                                               \
        direction.x = (vx);                                                                                            \
        direction.y = -(vy) * camera.aspect;                                                                           \
        direction.z = adjacent;                                                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        NuVecMtxTransform(&direction, &direction, &camera.mtx);                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        brightness = direction.y * 0.5f + 0.5f;                                                                        \
        if (brightness <= near_angle)                                                                                  \
            brightness = near_scale * parameters->intensity;                                                           \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale * parameters->intensity;                                                            \
        else                                                                                                           \
            brightness = ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity; \
        if (parameters->directional) {                                                                                 \
            f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;       \
            if (angle < parameters->direction_near_angle) {                                                            \
                brightness += 128.0f * parameters->direction_far_scale;                                                \
            } else if (!(angle > parameters->direction_far_angle)) {                                                   \
                f32 blend = NuPowFast((angle - parameters->direction_near_angle) /                                     \
                                          (parameters->direction_far_angle - parameters->direction_near_angle),        \
                                      parameters->direction_bias);                                                     \
                brightness +=                                                                                          \
                    128.0f * ((1.0f - blend) * (parameters->direction_far_scale - parameters->direction_near_scale) +  \
                              parameters->direction_near_scale);                                                       \
            }                                                                                                          \
        }                                                                                                              \
        u32 colour = (static_cast<u32>(static_cast<u8>(128)) << 24) |                                                  \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 16) |                   \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 8) |                    \
                     static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness))));                            \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] =                                                                     \
                ((static_cast<i32>(colour) >> 1) & 0x7f7f7f) | (colour & 0xff000000);                                  \
        else                                                                                                           \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = colour;                                                             \
        NuPrim2DAddXYZ((vx), (vy), 0.0f);                                                                              \
    } while (0)

#define SWIPE_PRIM_UV(U, V)                                                                                            \
    do {                                                                                                               \
        PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);                      \
        if (!g_NuPrim_NeedsHalfUVs) {                                                                                  \
            vertex->float_uv[0] = (U);                                                                                 \
            vertex->float_uv[1] = (V);                                                                                 \
        } else {                                                                                                       \
            vertex->half_uv[0] = NuRndrFloatToHalf(U);                                                                 \
            vertex->half_uv[1] = NuRndrFloatToHalf(V);                                                                 \
        }                                                                                                              \
    } while (false)

struct AIPACKET_s;

extern "C" {
    // Shared suspend flag consulted by all timed debris emitters.

}

extern "C" {
    // The editor loader clears these page IDs with aligned vector stores.
} // extern "C"

// The original repeats the per-sound emitter checks for each of the four sound slots.
#define PART_EMIT_SOUND_ON(slot)                                                                                       \
    if (type->sounds[slot] != -1) {                                                                                    \
        SetSfxBit_On(type->sounds[slot]);                                                                              \
        if (type->sound_modes[slot] == 4)                                                                              \
            PlaySfxById(type->sounds[slot], &emitter->position);                                                       \
    }
#define PART_EMIT_SOUND(slot, mode)                                                                                    \
    if (type->sounds[slot] != -1 && type->sound_modes[slot] == (mode))                                                 \
        PlaySfxById(type->sounds[slot], &emitter->position);

// Retail keeps an unreferenced partial-inlining clone of the old HSV2RGB
// helper (the non-grey path) in this unit.
#if defined(__i386__)
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone, regparm(2), sseregparm))
#else
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone))
#endif

// Animation editor subsystem stubs (static, internal linkage).

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

// Remaining animation-editor UI/menu callbacks.

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

// The original particle editor's private callbacks and menu state share this TU.
#if defined(__SSE__)
#include <xmmintrin.h>
#endif

// The original local font helpers read the same private draw state as the
// editor callbacks, so they belong to this translation unit.
static __used__ __attribute__((noclone)) void eduiFntPrintExGeneric(void *font, int x, int y,
                                                                                int alignment, char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz");
static __used__ void eduiFntPrintExGeneric(void *font, int x, int y, int alignment, char *format, ...);

// Left-aligned (16) specialisation of the integer print helper.
static __attribute__((noclone)) void eduiFntPrintExLeft(void *font, int x, int y, int alignment,
                                                                  char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.67");

// Centred (64) specialisation of the integer print helper.
static __attribute__((noclone)) void eduiFntPrintExCentre(void *font, int x, int y, int alignment,
                                                                    char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.68");

#if defined(__clang__)
// Clang has no __builtin_va_arg_pack; format here and print the result. The
// generic helper already owns this overload's mangled name, so rename it.
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
    // Shared editor font; edmainInit receives and stores the font handle.

    // The editor UI callbacks share this translation unit with their cursor,
    // menu, and interaction state in the original binary.

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
    // Retail keeps this as the constant-propagated clone of the generic four-step
    // gradient helper; only the hue/value colour picker band reaches it.
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

// Text-picker commit callbacks. Retail keeps them unmangled and local.

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

// Temporary C-linkage compatibility surface for unresolved engine owners.
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.

struct ani3_animheader_s;

extern "C" {
    // ---------------------------------------------------------------------------
    // Display-list bootstrap (original nucore TU file-statics)
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Redirected symbols — real bodies live elsewhere (kept as comments)
    // ---------------------------------------------------------------------------

    // Nu360_dxClear(u32,u32) is implemented in ios_graphics.cpp (original 0x317070).
    // NuDisplayListSwapBuffersBeginFrame / EndFrame are transcribed in full
    // in nudlist.cpp (originals 0x2eb5d0 / 0x2eaef0).
    // NuHtmlBegin(void*) is transcribed in legoapi/misc/supportall.cpp (original 0x2d5ca0).
    // NuIOS_SetVertexFormat is transcribed in nuiosdl_gl.cpp (original 0x29c070).
    // NuIOS_WaitForRenderThreadCompletion is transcribed in ios_graphics.cpp (original 0xe34b0).
    // NuIOS_WakeRenderThread is transcribed in ios_graphics.cpp (original 0xe3590).
    // NuRenderContextSetZFunc is transcribed below (original 0x2a3860).

    // ---------------------------------------------------------------------------
    // Camera (thin wrappers; full matrix work is in NuCameraSetEx)
    // ---------------------------------------------------------------------------


    // ---------------------------------------------------------------------------
    // Display list — faithful helpers + pending stubs
    // ---------------------------------------------------------------------------
} // extern "C"

extern "C" {
    extern "C++" {
        // Original local helper cloned as DisplayListProcessSkin.isra.36 by GCC.
        // It appends the world transform, skin palette, and geometry calls as one
        // contiguous run in the material display list.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        // Original local helper cloned as DisplayListProcessLightmapped.isra.34.
        // Lightmapped geometry carries an AE/AF/B0 setup command two entries before
        // its geometry command. Keep that setup command adjacent to the dynamic
        // transform and geometry entries when the special is submitted.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

    // ---------------------------------------------------------------------------
    // Scene / render-scene
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Frame lifecycle (NuFrameEnd is faithful; neighbours are stubs)
    // ---------------------------------------------------------------------------

    // Per-frame animation/pad update hooks (originals take the frame time).

    // Frame-end callbacks (original bss @0x6bdaec/0x6bdaf0/0x6bdad0).

    // Faithful transcription of the original frame-end pump. Waits for the
    // target frame interval, ticks material/tex/wind anims, swaps screens
    // via NuRndrSwapScreenEx, then advances nuapi clocks and pad state.

    // ---------------------------------------------------------------------------
    // iOS / platform
    // ---------------------------------------------------------------------------

    // Locale string is owned by the Android/JNI TU; cache only its index here.

    // ---------------------------------------------------------------------------
    // Animation / curves / data
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Bridge / file / heap / memory
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Math / geometry
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Quick-font platform rendering (the generic font run lives in nuqfnt.cpp)
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Rendering / materials / effects (host has GL paths elsewhere)
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Light / wind / particles / debris
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Gobj / hierarchy / scene graph
    // ---------------------------------------------------------------------------


    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------


    // ---------------------------------------------------------------------------
    // Viewport
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Strings / conversion / Unicode
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Containers / lists / params
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Debug / error / profiling
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Thread / misc OS
    // ---------------------------------------------------------------------------

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
    // ---------------------------------------------------------------------------
    // Spline / online / net / other gameplay support
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Rendering extras
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // PS2 / Xbox legacy shims
    // ---------------------------------------------------------------------------
} // extern "C"

union variptr_u;

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

// Original data symbol testmovef is float0.005 at 0x617cb0.


extern "C" i16 id_LANDSPEEDER;


#define ALPHA_GRID_VERTEX(vx, vy)                                                                                      \
    do {                                                                                                               \
        direction.x = (vx);                                                                                            \
        direction.y = -(vy) * camera.aspect;                                                                           \
        direction.z = adjacent;                                                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        NuVecMtxTransform(&direction, &direction, &camera.mtx);                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        brightness = direction.y * 0.5f + 0.5f;                                                                        \
        if (brightness <= near_angle)                                                                                  \
            brightness = near_scale * parameters->intensity;                                                           \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale * parameters->intensity;                                                            \
        else                                                                                                           \
            brightness = ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity; \
        if (parameters->directional) {                                                                                 \
            f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;       \
            if (angle < parameters->direction_near_angle) {                                                            \
                brightness += 128.0f * parameters->direction_far_scale;                                                \
            } else if (!(angle > parameters->direction_far_angle)) {                                                   \
                f32 blend = NuPowFast((angle - parameters->direction_near_angle) /                                     \
                                          (parameters->direction_far_angle - parameters->direction_near_angle),        \
                                      parameters->direction_bias);                                                     \
                brightness +=                                                                                          \
                    128.0f * ((1.0f - blend) * (parameters->direction_far_scale - parameters->direction_near_scale) +  \
                              parameters->direction_near_scale);                                                       \
            }                                                                                                          \
        }                                                                                                              \
        u32 colour = (static_cast<u32>(static_cast<u8>(128)) << 24) |                                                  \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 16) |                   \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 8) |                    \
                     static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness))));                            \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] =                                                                     \
                ((static_cast<i32>(colour) >> 1) & 0x7f7f7f) | (colour & 0xff000000);                                  \
        else                                                                                                           \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = colour;                                                             \
        NuPrim2DAddXYZ((vx), (vy), 0.0f);                                                                              \
    } while (0)

#define SWIPE_PRIM_UV(U, V)                                                                                            \
    do {                                                                                                               \
        PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);                      \
        if (!g_NuPrim_NeedsHalfUVs) {                                                                                  \
            vertex->float_uv[0] = (U);                                                                                 \
            vertex->float_uv[1] = (V);                                                                                 \
        } else {                                                                                                       \
            vertex->half_uv[0] = NuRndrFloatToHalf(U);                                                                 \
            vertex->half_uv[1] = NuRndrFloatToHalf(V);                                                                 \
        }                                                                                                              \
    } while (false)

extern "C" {
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
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

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Viewport
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
} // extern "C"

namespace {

} // namespace

#define ALPHA_GRID_VERTEX(vx, vy)                                                                                      \
    do {                                                                                                               \
        direction.x = (vx);                                                                                            \
        direction.y = -(vy) * camera.aspect;                                                                           \
        direction.z = adjacent;                                                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        NuVecMtxTransform(&direction, &direction, &camera.mtx);                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        brightness = direction.y * 0.5f + 0.5f;                                                                        \
        if (brightness <= near_angle)                                                                                  \
            brightness = near_scale * parameters->intensity;                                                           \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale * parameters->intensity;                                                            \
        else                                                                                                           \
            brightness = ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity; \
        if (parameters->directional) {                                                                                 \
            f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;       \
            if (angle < parameters->direction_near_angle) {                                                            \
                brightness += 128.0f * parameters->direction_far_scale;                                                \
            } else if (!(angle > parameters->direction_far_angle)) {                                                   \
                f32 blend = NuPowFast((angle - parameters->direction_near_angle) /                                     \
                                          (parameters->direction_far_angle - parameters->direction_near_angle),        \
                                      parameters->direction_bias);                                                     \
                brightness +=                                                                                          \
                    128.0f * ((1.0f - blend) * (parameters->direction_far_scale - parameters->direction_near_scale) +  \
                              parameters->direction_near_scale);                                                       \
            }                                                                                                          \
        }                                                                                                              \
        u32 colour = (static_cast<u32>(static_cast<u8>(128)) << 24) |                                                  \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 16) |                   \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 8) |                    \
                     static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness))));                            \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] =                                                                     \
                ((static_cast<i32>(colour) >> 1) & 0x7f7f7f) | (colour & 0xff000000);                                  \
        else                                                                                                           \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = colour;                                                             \
        NuPrim2DAddXYZ((vx), (vy), 0.0f);                                                                              \
    } while (0)

#define SWIPE_PRIM_UV(U, V)                                                                                            \
    do {                                                                                                               \
        PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);                      \
        if (!g_NuPrim_NeedsHalfUVs) {                                                                                  \
            vertex->float_uv[0] = (U);                                                                                 \
            vertex->float_uv[1] = (V);                                                                                 \
        } else {                                                                                                       \
            vertex->half_uv[0] = NuRndrFloatToHalf(U);                                                                 \
            vertex->half_uv[1] = NuRndrFloatToHalf(V);                                                                 \
        }                                                                                                              \
    } while (false)

#define PART_EMIT_SOUND_ON(slot)                                                                                       \
    if (type->sounds[slot] != -1) {                                                                                    \
        SetSfxBit_On(type->sounds[slot]);                                                                              \
        if (type->sound_modes[slot] == 4)                                                                              \
            PlaySfxById(type->sounds[slot], &emitter->position);                                                       \
    }
#define PART_EMIT_SOUND(slot, mode)                                                                                    \

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
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
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

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Viewport
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
} // extern "C"

#define ALPHA_GRID_VERTEX(vx, vy)                                                                                      \
    do {                                                                                                               \
        direction.x = (vx);                                                                                            \
        direction.y = -(vy) * camera.aspect;                                                                           \
        direction.z = adjacent;                                                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        NuVecMtxTransform(&direction, &direction, &camera.mtx);                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        brightness = direction.y * 0.5f + 0.5f;                                                                        \
        if (brightness <= near_angle)                                                                                  \
            brightness = near_scale * parameters->intensity;                                                           \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale * parameters->intensity;                                                            \
        else                                                                                                           \
            brightness = ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity; \
        if (parameters->directional) {                                                                                 \
            f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;       \
            if (angle < parameters->direction_near_angle) {                                                            \
                brightness += 128.0f * parameters->direction_far_scale;                                                \
            } else if (!(angle > parameters->direction_far_angle)) {                                                   \
                f32 blend = NuPowFast((angle - parameters->direction_near_angle) /                                     \
                                          (parameters->direction_far_angle - parameters->direction_near_angle),        \
                                      parameters->direction_bias);                                                     \
                brightness +=                                                                                          \
                    128.0f * ((1.0f - blend) * (parameters->direction_far_scale - parameters->direction_near_scale) +  \
                              parameters->direction_near_scale);                                                       \
            }                                                                                                          \
        }                                                                                                              \
        u32 colour = (static_cast<u32>(static_cast<u8>(128)) << 24) |                                                  \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 16) |                   \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 8) |                    \
                     static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness))));                            \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] =                                                                     \
                ((static_cast<i32>(colour) >> 1) & 0x7f7f7f) | (colour & 0xff000000);                                  \
        else                                                                                                           \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = colour;                                                             \
        NuPrim2DAddXYZ((vx), (vy), 0.0f);                                                                              \
    } while (0)

#define SWIPE_PRIM_UV(U, V)                                                                                            \
    do {                                                                                                               \
        PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);                      \
        if (!g_NuPrim_NeedsHalfUVs) {                                                                                  \
            vertex->float_uv[0] = (U);                                                                                 \
            vertex->float_uv[1] = (V);                                                                                 \
        } else {                                                                                                       \
            vertex->half_uv[0] = NuRndrFloatToHalf(U);                                                                 \
            vertex->half_uv[1] = NuRndrFloatToHalf(V);                                                                 \
        }                                                                                                              \
    } while (false)

extern "C" {
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
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

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Viewport
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
} // extern "C"

// Retail only keeps the split-out body of the IPA-SRA clone (the unused
// cutscene index dropped), called with world and cut in registers.
#if defined(__i386__)
#define CUTSCENE_START_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define CUTSCENE_START_ATTR __attribute__((noinline, noclone))
#endif

// The loop re-reads the active cut through CutStopInfo after each SmartTextEx call.
#define CUT_STOP static_cast<CUTINFO *>(CutStopInfo)

#define NUGCUT_RELOCATE(field, delta) \
    (NUGCUT_WORD(field) = NUGCUT_WORD(field) != 0 ? NUGCUT_WORD(field) + (delta) : 0)

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
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
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

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Viewport
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
} // extern "C"

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];


}

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];


}

#define ALPHA_GRID_VERTEX(vx, vy)                                                                                      \
    do {                                                                                                               \
        direction.x = (vx);                                                                                            \
        direction.y = -(vy) * camera.aspect;                                                                           \
        direction.z = adjacent;                                                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        NuVecMtxTransform(&direction, &direction, &camera.mtx);                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        brightness = direction.y * 0.5f + 0.5f;                                                                        \
        if (brightness <= near_angle)                                                                                  \
            brightness = near_scale * parameters->intensity;                                                           \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale * parameters->intensity;                                                            \
        else                                                                                                           \
            brightness = ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity; \
        if (parameters->directional) {                                                                                 \
            f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;       \
            if (angle < parameters->direction_near_angle) {                                                            \
                brightness += 128.0f * parameters->direction_far_scale;                                                \
            } else if (!(angle > parameters->direction_far_angle)) {                                                   \
                f32 blend = NuPowFast((angle - parameters->direction_near_angle) /                                     \
                                          (parameters->direction_far_angle - parameters->direction_near_angle),        \
                                      parameters->direction_bias);                                                     \
                brightness +=                                                                                          \
                    128.0f * ((1.0f - blend) * (parameters->direction_far_scale - parameters->direction_near_scale) +  \
                              parameters->direction_near_scale);                                                       \
            }                                                                                                          \
        }                                                                                                              \
        u32 colour = (static_cast<u32>(static_cast<u8>(128)) << 24) |                                                  \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 16) |                   \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 8) |                    \
                     static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness))));                            \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] =                                                                     \
                ((static_cast<i32>(colour) >> 1) & 0x7f7f7f) | (colour & 0xff000000);                                  \
        else                                                                                                           \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = colour;                                                             \
        NuPrim2DAddXYZ((vx), (vy), 0.0f);                                                                              \
    } while (0)

#define SWIPE_PRIM_UV(U, V)                                                                                            \
    do {                                                                                                               \
        PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);                      \
        if (!g_NuPrim_NeedsHalfUVs) {                                                                                  \
            vertex->float_uv[0] = (U);                                                                                 \
            vertex->float_uv[1] = (V);                                                                                 \
        } else {                                                                                                       \
            vertex->half_uv[0] = NuRndrFloatToHalf(U);                                                                 \
            vertex->half_uv[1] = NuRndrFloatToHalf(V);                                                                 \
        }                                                                                                              \
    } while (false)

struct MechTouchUIElement;

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

namespace {

} // namespace


// Retail keeps only the IPA-SRA clones (taking the flags field directly),
// mangled with the original ANIMLIST_s parameter type.
#if defined(__i386__)
#define CC_SET_ANIM_FLAGS_ATTR __attribute__((noinline, noclone, regparm(2)))
// The u32 clone keeps retail's push-based prologue only when size-optimized.
#define CC_SET_ANIM_FLAGS_OS_ATTR __attribute__((noinline, noclone, optimize("Os"), regparm(2)))
#else
#define CC_SET_ANIM_FLAGS_ATTR __attribute__((noinline, noclone))
#define CC_SET_ANIM_FLAGS_OS_ATTR __attribute__((noinline, noclone))
#endif

// Retail only keeps the clone with strip_comments fixed to 1 (regparm text, filename).
#if defined(__i386__)
#define REDIRECT_TEXT_FILE_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define REDIRECT_TEXT_FILE_ATTR __attribute__((noinline, noclone))
#endif

#define ALPHA_GRID_VERTEX(vx, vy)                                                                                      \
    do {                                                                                                               \
        direction.x = (vx);                                                                                            \
        direction.y = -(vy) * camera.aspect;                                                                           \
        direction.z = adjacent;                                                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        NuVecMtxTransform(&direction, &direction, &camera.mtx);                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        brightness = direction.y * 0.5f + 0.5f;                                                                        \
        if (brightness <= near_angle)                                                                                  \
            brightness = near_scale * parameters->intensity;                                                           \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale * parameters->intensity;                                                            \
        else                                                                                                           \
            brightness = ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity; \
        if (parameters->directional) {                                                                                 \
            f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;       \
            if (angle < parameters->direction_near_angle) {                                                            \
                brightness += 128.0f * parameters->direction_far_scale;                                                \
            } else if (!(angle > parameters->direction_far_angle)) {                                                   \
                f32 blend = NuPowFast((angle - parameters->direction_near_angle) /                                     \
                                          (parameters->direction_far_angle - parameters->direction_near_angle),        \
                                      parameters->direction_bias);                                                     \
                brightness +=                                                                                          \
                    128.0f * ((1.0f - blend) * (parameters->direction_far_scale - parameters->direction_near_scale) +  \
                              parameters->direction_near_scale);                                                       \
            }                                                                                                          \
        }                                                                                                              \
        u32 colour = (static_cast<u32>(static_cast<u8>(128)) << 24) |                                                  \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 16) |                   \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 8) |                    \
                     static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness))));                            \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] =                                                                     \
                ((static_cast<i32>(colour) >> 1) & 0x7f7f7f) | (colour & 0xff000000);                                  \
        else                                                                                                           \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = colour;                                                             \
        NuPrim2DAddXYZ((vx), (vy), 0.0f);                                                                              \
    } while (0)

#define SWIPE_PRIM_UV(U, V)                                                                                            \
    do {                                                                                                               \
        PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);                      \
        if (!g_NuPrim_NeedsHalfUVs) {                                                                                  \
            vertex->float_uv[0] = (U);                                                                                 \
            vertex->float_uv[1] = (V);                                                                                 \
        } else {                                                                                                       \
            vertex->half_uv[0] = NuRndrFloatToHalf(U);                                                                 \
            vertex->half_uv[1] = NuRndrFloatToHalf(V);                                                                 \
        }                                                                                                              \
    } while (false)

#define PART_EMIT_SOUND_ON(slot)                                                                                       \
    if (type->sounds[slot] != -1) {                                                                                    \
        SetSfxBit_On(type->sounds[slot]);                                                                              \
        if (type->sound_modes[slot] == 4)                                                                              \
            PlaySfxById(type->sounds[slot], &emitter->position);                                                       \
    }
#define PART_EMIT_SOUND(slot, mode)                                                                                    \

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
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
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

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Viewport
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
} // extern "C"

#define ALPHA_GRID_VERTEX(vx, vy)                                                                                      \
    do {                                                                                                               \
        direction.x = (vx);                                                                                            \
        direction.y = -(vy) * camera.aspect;                                                                           \
        direction.z = adjacent;                                                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        NuVecMtxTransform(&direction, &direction, &camera.mtx);                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        brightness = direction.y * 0.5f + 0.5f;                                                                        \
        if (brightness <= near_angle)                                                                                  \
            brightness = near_scale * parameters->intensity;                                                           \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale * parameters->intensity;                                                            \
        else                                                                                                           \
            brightness = ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity; \
        if (parameters->directional) {                                                                                 \
            f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;       \
            if (angle < parameters->direction_near_angle) {                                                            \
                brightness += 128.0f * parameters->direction_far_scale;                                                \
            } else if (!(angle > parameters->direction_far_angle)) {                                                   \
                f32 blend = NuPowFast((angle - parameters->direction_near_angle) /                                     \
                                          (parameters->direction_far_angle - parameters->direction_near_angle),        \
                                      parameters->direction_bias);                                                     \
                brightness +=                                                                                          \
                    128.0f * ((1.0f - blend) * (parameters->direction_far_scale - parameters->direction_near_scale) +  \
                              parameters->direction_near_scale);                                                       \
            }                                                                                                          \
        }                                                                                                              \
        u32 colour = (static_cast<u32>(static_cast<u8>(128)) << 24) |                                                  \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 16) |                   \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 8) |                    \
                     static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness))));                            \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] =                                                                     \
                ((static_cast<i32>(colour) >> 1) & 0x7f7f7f) | (colour & 0xff000000);                                  \
        else                                                                                                           \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = colour;                                                             \
        NuPrim2DAddXYZ((vx), (vy), 0.0f);                                                                              \
    } while (0)

#define SWIPE_PRIM_UV(U, V)                                                                                            \
    do {                                                                                                               \
        PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);                      \
        if (!g_NuPrim_NeedsHalfUVs) {                                                                                  \
            vertex->float_uv[0] = (U);                                                                                 \
            vertex->float_uv[1] = (V);                                                                                 \
        } else {                                                                                                       \
            vertex->half_uv[0] = NuRndrFloatToHalf(U);                                                                 \
            vertex->half_uv[1] = NuRndrFloatToHalf(V);                                                                 \
        }                                                                                                              \
    } while (false)

extern "C" {
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
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

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Viewport
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
} // extern "C"

static NU_READ_GRAPHICS_DATA_ATTR NUGSCN *NuReadGraphicsData(VARIPTR *buf, VARIPTR *buf_end, char *path,
                                                             char *scene_data)
    __asm__("_ZL18NuReadGraphicsDataP9variptr_uS0_PcS1_S1_.isra.104");

#define ALPHA_GRID_VERTEX(vx, vy)                                                                                      \
    do {                                                                                                               \
        direction.x = (vx);                                                                                            \
        direction.y = -(vy) * camera.aspect;                                                                           \
        direction.z = adjacent;                                                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        NuVecMtxTransform(&direction, &direction, &camera.mtx);                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        brightness = direction.y * 0.5f + 0.5f;                                                                        \
        if (brightness <= near_angle)                                                                                  \
            brightness = near_scale * parameters->intensity;                                                           \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale * parameters->intensity;                                                            \
        else                                                                                                           \
            brightness = ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity; \
        if (parameters->directional) {                                                                                 \
            f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;       \
            if (angle < parameters->direction_near_angle) {                                                            \
                brightness += 128.0f * parameters->direction_far_scale;                                                \
            } else if (!(angle > parameters->direction_far_angle)) {                                                   \
                f32 blend = NuPowFast((angle - parameters->direction_near_angle) /                                     \
                                          (parameters->direction_far_angle - parameters->direction_near_angle),        \
                                      parameters->direction_bias);                                                     \
                brightness +=                                                                                          \
                    128.0f * ((1.0f - blend) * (parameters->direction_far_scale - parameters->direction_near_scale) +  \
                              parameters->direction_near_scale);                                                       \
            }                                                                                                          \
        }                                                                                                              \
        u32 colour = (static_cast<u32>(static_cast<u8>(128)) << 24) |                                                  \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 16) |                   \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 8) |                    \
                     static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness))));                            \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] =                                                                     \
                ((static_cast<i32>(colour) >> 1) & 0x7f7f7f) | (colour & 0xff000000);                                  \
        else                                                                                                           \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = colour;                                                             \
        NuPrim2DAddXYZ((vx), (vy), 0.0f);                                                                              \
    } while (0)

#define SWIPE_PRIM_UV(U, V)                                                                                            \
    do {                                                                                                               \
        PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);                      \
        if (!g_NuPrim_NeedsHalfUVs) {                                                                                  \
            vertex->float_uv[0] = (U);                                                                                 \
            vertex->float_uv[1] = (V);                                                                                 \
        } else {                                                                                                       \
            vertex->half_uv[0] = NuRndrFloatToHalf(U);                                                                 \
            vertex->half_uv[1] = NuRndrFloatToHalf(V);                                                                 \
        }                                                                                                              \
    } while (false)

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];


}

// Retail keeps several of this file's static helpers only as GCC clones; these
// declarations pin the clone names and register conventions.
#if defined(__i386__)
#define MOVE_CLONE_ATTR1 __attribute__((noinline, noclone, regparm(1)))
#define MOVE_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define MOVE_CLONE_ATTR1 __attribute__((noinline, noclone))
#define MOVE_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif

// Original exported level control, initialized to 2 at 0x623d00.

extern "C" i16 id_LANDSPEEDER;

// Retail re-tests the three block animations at each use instead of caching a flag.
#define HAS_BLOCK_ANIMS(animations)                                                                                    \
    ((animations)[0x1a] != NULL || (animations)[0x1b] != NULL || (animations)[0x1c] != NULL)

extern "C" {
    extern i16 id_MINIATST, id_MINIATTE;
}


extern i16 id_MINIATST, id_MINIATTE;


// Asteroid chase (AsteroidChase_A / B / C / D)




#define PART_EMIT_SOUND_ON(slot)                                                                                       \
    if (type->sounds[slot] != -1) {                                                                                    \
        SetSfxBit_On(type->sounds[slot]);                                                                              \
        if (type->sound_modes[slot] == 4)                                                                              \
            PlaySfxById(type->sounds[slot], &emitter->position);                                                       \
    }
#define PART_EMIT_SOUND(slot, mode)                                                                                    \

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];


}

namespace {

} // namespace

struct FLOWBOX_s;



// Defined in gameapi/edtools/edtoolsall.cpp, which has no header yet.

struct JEDIBNETPACKET_s;

// --- File-local statics (original _ZL... symbols; not renamed) ---------------




// Jedi (Jedi_B)




// Dooku (Dooku_C)


// PodSprint_InitAISpline is partially inlined into PodSprintA_Init: the
// clear, lookup and spline checks are inlined and the crossing scan is the
// split-out body.
#if defined(__i386__)
#define PODSPRINT_AISPLINE_BODY_ATTR __attribute__((noinline, regparm(1)))
#else
#define PODSPRINT_AISPLINE_BODY_ATTR __attribute__((noinline))
#endif

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
    struct EDCREATURE_s;
}

#if defined(__i386__)
#else
#endif

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];


}

namespace {

} // namespace

#define ALPHA_GRID_VERTEX(vx, vy)                                                                                      \
    do {                                                                                                               \
        direction.x = (vx);                                                                                            \
        direction.y = -(vy) * camera.aspect;                                                                           \
        direction.z = adjacent;                                                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        NuVecMtxTransform(&direction, &direction, &camera.mtx);                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        brightness = direction.y * 0.5f + 0.5f;                                                                        \
        if (brightness <= near_angle)                                                                                  \
            brightness = near_scale * parameters->intensity;                                                           \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale * parameters->intensity;                                                            \
        else                                                                                                           \
            brightness = ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity; \
        if (parameters->directional) {                                                                                 \
            f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;       \
            if (angle < parameters->direction_near_angle) {                                                            \
                brightness += 128.0f * parameters->direction_far_scale;                                                \
            } else if (!(angle > parameters->direction_far_angle)) {                                                   \
                f32 blend = NuPowFast((angle - parameters->direction_near_angle) /                                     \
                                          (parameters->direction_far_angle - parameters->direction_near_angle),        \
                                      parameters->direction_bias);                                                     \
                brightness +=                                                                                          \
                    128.0f * ((1.0f - blend) * (parameters->direction_far_scale - parameters->direction_near_scale) +  \
                              parameters->direction_near_scale);                                                       \
            }                                                                                                          \
        }                                                                                                              \
        u32 colour = (static_cast<u32>(static_cast<u8>(128)) << 24) |                                                  \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 16) |                   \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 8) |                    \
                     static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness))));                            \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] =                                                                     \
                ((static_cast<i32>(colour) >> 1) & 0x7f7f7f) | (colour & 0xff000000);                                  \
        else                                                                                                           \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = colour;                                                             \
        NuPrim2DAddXYZ((vx), (vy), 0.0f);                                                                              \
    } while (0)

#define SWIPE_PRIM_UV(U, V)                                                                                            \
    do {                                                                                                               \
        PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);                      \
        if (!g_NuPrim_NeedsHalfUVs) {                                                                                  \
            vertex->float_uv[0] = (U);                                                                                 \
            vertex->float_uv[1] = (V);                                                                                 \
        } else {                                                                                                       \
            vertex->half_uv[0] = NuRndrFloatToHalf(U);                                                                 \
            vertex->half_uv[1] = NuRndrFloatToHalf(V);                                                                 \
        }                                                                                                              \
    } while (false)

#define ADD_SNAKE_SEGMENT_DEBRIS(index)                                                                                \
    do {                                                                                                               \
        const i32 segment_index = (index);                                                                             \
        NUMTX matrix;                                                                                                  \
        NUANGVEC angles;                                                                                               \
        angles.y = NuAngAdd(object->snake_body->segments[segment_index].yaw, 0x8000);                                  \
        angles.x = object->snake_body->segments[segment_index].pitch;                                                  \
        NuMtxSetRotationXYVU0(&matrix, &angles);                                                                       \
        if (object->snake_body->scale != 1.0f) {                                                                       \
            NUVEC scale = {object->snake_body->scale, object->snake_body->scale, object->snake_body->scale};           \
            NuMtxPreScale(&matrix, &scale);                                                                            \
        }                                                                                                              \
        matrix.m30 += object->snake_body->segments[segment_index].position.x;                                          \
        matrix.m31 += object->snake_body->segments[segment_index].position.y + object->snake_body->scale * 0.02f;      \
        matrix.m32 += object->snake_body->segments[segment_index].position.z;                                          \
        i32 special_index = segment_index == object->snake_body->segment_count - 1 ? 2 : segment_index % 2;            \
        nuhspecial_s *special = &snake_hspecials[special_index];                                                       \
        if (NuSpecialExistsFn(special)) {                                                                              \
            NUVEC momentum;                                                                                            \
            SetKillPartMom(&momentum);                                                                                 \
            momentum.y += 1.0f;                                                                                        \
            ADDPART_s part = Default_ADDPART;                                                                          \
            part.matrix = &matrix;                                                                                     \
            part.velocity = &momentum;                                                                                 \
            part.field_14 = 0.1f;                                                                                      \
            part.field_18 = 0.1f;                                                                                      \
            part.gravity = -5.0f;                                                                                      \
            part.special = special;                                                                                    \
            part.flags = 0x90;                                                                                         \
            part.stop_fn = PartStop_Flickerer;                                                                         \
            part.draw_fn = PartDraw_Flickerer;                                                                         \
            part.field_3c = PartImpact_Brick;                                                                          \
            part.time_step = FRAMETIME;                                                                                \
            part.lighting = reinterpret_cast<PARTLIGHTSOURCE_s *>(&object->light_data);                                \
            AddPart(&part);                                                                                            \
        }                                                                                                              \
    } while (0)

#define ALPHA_GRID_VERTEX(vx, vy)                                                                                      \
    do {                                                                                                               \
        direction.x = (vx);                                                                                            \
        direction.y = -(vy) * camera.aspect;                                                                           \
        direction.z = adjacent;                                                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        NuVecMtxTransform(&direction, &direction, &camera.mtx);                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        brightness = direction.y * 0.5f + 0.5f;                                                                        \
        if (brightness <= near_angle)                                                                                  \
            brightness = near_scale * parameters->intensity;                                                           \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale * parameters->intensity;                                                            \
        else                                                                                                           \
            brightness = ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity; \
        if (parameters->directional) {                                                                                 \
            f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;       \
            if (angle < parameters->direction_near_angle) {                                                            \
                brightness += 128.0f * parameters->direction_far_scale;                                                \
            } else if (!(angle > parameters->direction_far_angle)) {                                                   \
                f32 blend = NuPowFast((angle - parameters->direction_near_angle) /                                     \
                                          (parameters->direction_far_angle - parameters->direction_near_angle),        \
                                      parameters->direction_bias);                                                     \
                brightness +=                                                                                          \
                    128.0f * ((1.0f - blend) * (parameters->direction_far_scale - parameters->direction_near_scale) +  \
                              parameters->direction_near_scale);                                                       \
            }                                                                                                          \
        }                                                                                                              \
        u32 colour = (static_cast<u32>(static_cast<u8>(128)) << 24) |                                                  \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 16) |                   \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 8) |                    \
                     static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness))));                            \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] =                                                                     \
                ((static_cast<i32>(colour) >> 1) & 0x7f7f7f) | (colour & 0xff000000);                                  \
        else                                                                                                           \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = colour;                                                             \
        NuPrim2DAddXYZ((vx), (vy), 0.0f);                                                                              \
    } while (0)

#define SWIPE_PRIM_UV(U, V)                                                                                            \
    do {                                                                                                               \
        PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);                      \
        if (!g_NuPrim_NeedsHalfUVs) {                                                                                  \
            vertex->float_uv[0] = (U);                                                                                 \
            vertex->float_uv[1] = (V);                                                                                 \
        } else {                                                                                                       \
            vertex->half_uv[0] = NuRndrFloatToHalf(U);                                                                 \
            vertex->half_uv[1] = NuRndrFloatToHalf(V);                                                                 \
        }                                                                                                              \
    } while (false)


// Original writable flagremaptab at 0x668520, 32 pairs of 32-bit masks.
extern "C" {
extern u32 flagremaptab[32][2];
}

// Definitions from the original gizmoblowups translation unit.



// The reference defines these with dynamic initialization in this unit.


extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];


}

void GameAnimSys_AllocateLevelProgressData(variptr_u *buf, variptr_u *buf_end, i32 capacity, i32 level_count) {
    if (buf_end == NULL || buf == NULL)
        return;
    gameanimsysprogress.count = level_count;
    gameanimsysprogress.entry_size = capacity;
    gameanimsysprogress.entries = static_cast<u8 **>(GameBufferAlloc(buf, buf_end, level_count * sizeof(u8 *)));
    if (gameanimsysprogress.entries != NULL) {
        for (i32 i = 0; i < level_count; ++i)
            gameanimsysprogress.entries[i] = static_cast<u8 *>(GameBufferAlloc(buf, buf_end, capacity));
    }
}

void GameAnimSys_StoreProgress(GAMEANIMSYS_s *system, i32 index) {
    if (system == NULL || system->sets == NULL || index < 0 || index >= gameanimsysprogress.count)
        return;
    u8 *progress = gameanimsysprogress.entries[index];
    for (i32 i = 0; i < gameanimsysprogress.entry_size; ++i) {
        GAMEANIMSET_s *set = system->sets[i];
        if (set == NULL)
            return;
        progress[i] = static_cast<u8>(set->state);
    }
}

u8 *GameAnimSys_GetProgressData(i32 index) {
    if (index < 0 || index >= gameanimsysprogress.count)
        return NULL;
    return gameanimsysprogress.entries[index];
}

void GameAnimSys_ClearProgress(i32 idx) {
    if (idx < 0) {
        return;
    }
    if (idx >= gameanimsysprogress.count) {
        return;
    }
    u8 *data = gameanimsysprogress.entries[idx];
    if (gameanimsysprogress.entry_size <= 0) {
        return;
    }
    for (i32 i = 0; i < gameanimsysprogress.entry_size; i++) {
        data[i] = 0;
    }
}

GAMEANIMSYS_s *GameAnimSys_Create(variptr_u *buf, variptr_u *buf_end) {
    GAMEANIMSYS_s *system = static_cast<GAMEANIMSYS_s *>(GameBufferAlloc(buf, buf_end, sizeof(GAMEANIMSYS_s)));
    if (system != NULL && gameanimsysprogress.entry_size != 0) {
        system->sets = static_cast<GAMEANIMSET_s **>(
            GameBufferAlloc(buf, buf_end, gameanimsysprogress.entry_size * sizeof(GAMEANIMSET_s *)));
    }
    return system;
}

GAMEANIMSET_s *GameAnimSet_Create(variptr_u *buf, variptr_u *buf_end, GAMEANIMOBJPOOL_s *object_pool,
                                  GAMEANIMSYS_s *system) {
    GAMEANIMSET_s *set = NULL;
    if (object_pool != NULL) {
        set = static_cast<GAMEANIMSET_s *>(GameBufferAlloc(buf, buf_end, sizeof(GAMEANIMSET_s)));
        if (set != NULL) {
            set->object_pool = object_pool;
            set->system = system;
            i32 index = system->set_count;
            if (index < gameanimsysprogress.entry_size) {
                system->sets[index] = set;
                system->set_count = index + 1;
            }
        }
    }
    return set;
}

void GameAnimSet_AddToSystemList(GAMEANIMSET_s *set) {
    if (set != NULL && set->system != NULL && (set->flags & GAMEANIMSET_FLAG_IN_SYSTEM_LIST) == 0) {
        NuLinkedListAppend(&set->system->active_sets, &set->links);
        set->flags = static_cast<GAMEANIMSET_FLAGS>(set->flags | GAMEANIMSET_FLAG_IN_SYSTEM_LIST);
    }
}

void GameAnimSet_EvaluateState(GAMEANIMSET_s *set) {
    if ((set->flags & GAMEANIMSET_FLAG_IN_SYSTEM_LIST) != 0) {
        return;
    }

    GAMEANIMOBJ_s *object = set->objects;
    i32 all_at_end = 1;
    i32 all_at_start = 1;

    while (object != NULL) {
        nuinstanim_s *animation = object->instance_animation;
        if (animation != NULL) {
            f32 direction = object->end_frame < object->start_frame ? -1.0f : 1.0f;
            f32 current_frame = animation->ltime * direction;
            if (object->end_frame * direction > current_frame) {
                all_at_end = 0;
            }
            if (current_frame > object->start_frame * direction) {
                all_at_start = 0;
            }
        }
        object = object->next;
    }

    set->state = GAMEANIMSET_STATE_AT_START;
    if (all_at_end != 0) {
        set->state = GAMEANIMSET_STATE_AT_END;
    } else if (all_at_start == 0) {
        set->state = GAMEANIMSET_STATE_BETWEEN_ENDPOINTS;
    }
}

void GameAnimSet_RemoveFromSystemList(GAMEANIMSET_s *set) {
    if (set != NULL && set->system != NULL) {
        if ((set->flags & GAMEANIMSET_FLAG_IN_SYSTEM_LIST) != 0) {
            NuLinkedListRemove(&set->system->active_sets, &set->links);
            set->flags = static_cast<GAMEANIMSET_FLAGS>(set->flags & ~GAMEANIMSET_FLAG_IN_SYSTEM_LIST);
        }
        GameAnimSet_EvaluateState(set);
    }
}

void GameAnimSys_ReStoreProgress(GAMEANIMSYS_s *system, i32 index) {
    if (system == NULL || system->sets == NULL || index < 0 || index >= gameanimsysprogress.count)
        return;
    u8 *progress = gameanimsysprogress.entries[index];
    for (i32 i = 0; i < gameanimsysprogress.entry_size; ++i) {
        GAMEANIMSET_s *set = system->sets[i];
        if (set == NULL)
            return;
        const i8 state = static_cast<i8>(progress[i]);
        set->state = static_cast<GAMEANIMSET_STATE>(state);
        if (state == GAMEANIMSET_STATE_ACTIVE_FORWARD || state == GAMEANIMSET_STATE_ACTIVE_BACKWARD) {
            if ((set->flags & GAMEANIMSET_FLAG_IN_SYSTEM_LIST) == 0)
                GameAnimSet_AddToSystemList(set);
        } else if ((set->flags & GAMEANIMSET_FLAG_IN_SYSTEM_LIST) != 0)
            GameAnimSet_RemoveFromSystemList(set);
    }
}

GAMEANIMOBJPOOL_s *GameAnimSet_CreateObjectPool(variptr_u *buf, variptr_u *buf_end, i32 object_data_size,
                                                i32 capacity) {
    GAMEANIMOBJPOOL_s *pool = NULL;
    if (capacity != 0) {
        pool = static_cast<GAMEANIMOBJPOOL_s *>(GameBufferAlloc(buf, buf_end, sizeof(GAMEANIMOBJPOOL_s)));
        if (pool != NULL) {
            pool->object_data_size = static_cast<u16>(object_data_size);
            pool->capacity = static_cast<u16>(capacity);
            pool->objects = static_cast<GAMEANIMOBJ_s *>(
                GameBufferAlloc(buf, buf_end, static_cast<u16>(capacity) * sizeof(GAMEANIMOBJ_s)));
            if (object_data_size != 0) {
                pool->object_data = GameBufferAlloc(
                    buf, buf_end, static_cast<u16>(pool->object_data_size) * static_cast<u16>(pool->capacity));
            }

            for (i32 i = 0; i < pool->capacity; ++i) {
                GAMEANIMOBJ_s *object = &pool->objects[i];
                object->next = pool->free_objects;
                pool->free_objects = object;
            }
        }
    }
    return pool;
}

GAMEANIMOBJ_s *GameAnimSet_AddObject(GAMEANIMSET_s *set, nuhspecial_s *special, float start_frame, float end_frame,
                                     i32 append) {
    if (set == NULL || set->object_pool == NULL || set->object_pool->free_objects == NULL || special == NULL ||
        NuSpecialExistsFn(special) == 0) {
        return NULL;
    }

    GAMEANIMOBJPOOL_s *pool = set->object_pool;
    ++set->object_count;
    GAMEANIMOBJ_s *object = pool->free_objects;
    ++pool->active_count;
    pool->free_objects = object->next;

    if (append != 0) {
        object->next = NULL;
        if (set->objects == NULL) {
            set->objects = object;
        } else {
            GAMEANIMOBJ_s *tail = set->objects;
            while (tail->next != NULL) {
                tail = tail->next;
            }
            tail->next = object;
        }
    } else {
        object->next = set->objects;
        set->objects = object;
    }

    const i32 object_index = object - pool->objects;
    object->object_data = static_cast<u8 *>(pool->object_data) + pool->object_data_size * object_index;
    object->special = *special;
    object->instance_animation = NuSpecialGetInstAnim(&object->special);
    if (object->instance_animation == NULL) {
        return object;
    }

    object->animation = object->special.scene->instance_animation_data[object->instance_animation->anim_ix];
    const f32 last_frame = NuSpecialGetAnimEndFrame(&object->special);
    if (last_frame > 0.0f) {
        if (last_frame < end_frame) {
            object->end_frame = last_frame;
        } else {
            object->end_frame = end_frame;
            if (object->end_frame < 1.0f) {
                object->end_frame = 1.0f;
            }
        }
        if (start_frame > last_frame) {
            object->start_frame = last_frame;
        } else {
            object->start_frame = start_frame;
            if (object->start_frame < 1.0f) {
                object->start_frame = 1.0f;
            }
        }
        ++set->animated_object_count;
    } else {
        NuSpecialGetName(&object->special);
    }
    return object;
}

GAMEANIMOBJ_s *GameAnimSet_AddObjectByName(GAMEANIMSET_s *set, nugscn_s *scene, char *name, float start_frame,
                                           float end_frame, i32 append, GIZMOSYS_s *gizmo_sys, char *prefix,
                                           char *suffix) {
    if (set == NULL) {
        return NULL;
    }

    nuhspecial_s special;
    if (Gizmo_FindNuSpecial(scene, &special, name, 1, gizmo_sys, prefix, suffix) == 0) {
        return NULL;
    }
    return GameAnimSet_AddObject(set, &special, start_frame, end_frame, append);
}

void GameAnimSet_RemoveObject(GAMEANIMSET_s *set, GAMEANIMOBJ_s *object) {
    if (object == NULL || set == NULL)
        return;
    if (set->objects == object) {
        set->objects = object->next;
    } else {
        GAMEANIMOBJ_s *previous = set->objects;
        while (previous != NULL && previous->next != object) {
            previous = previous->next;
        }
        if (previous != NULL)
            previous->next = object->next;
    }
    object->next = NULL;
    --set->object_count;
    GAMEANIMOBJPOOL_s *pool = set->object_pool;
    --pool->active_count;
    object->next = pool->free_objects;
    pool->free_objects = object;
}

void GameAnimSet_RemoveSpecial(GAMEANIMSET_s *set, nuhspecial_s *special) {
    if (special == NULL || set == NULL)
        return;
    GAMEANIMOBJ_s *object = set->objects;
    while (object != NULL && NuSpecialCompare(&object->special, special) == 0) {
        object = object->next;
    }
    if (object != NULL)
        GameAnimSet_RemoveObject(set, object);
}

void GameAnimSet_RemoveAllObjects(GAMEANIMSET_s *set) {
    if (set != NULL) {
        while (set->objects != NULL)
            GameAnimSet_RemoveObject(set, set->objects);
    }
}

void GameAnimSet_SetTFactor(GAMEANIMSET_s *set, float factor) {
    if (set == NULL) {
        return;
    }

    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        nuinstanim_s *animation = object->instance_animation;
        if (animation != NULL) {
            f32 direction = object->end_frame < object->start_frame ? -1.0f : 1.0f;
            if (animation->fparam1 != 0.0f) {
                animation->tfactor = animation->fparam1 * factor * direction;
            } else {
                animation->tfactor = direction * factor;
            }
        }
    }
}

void GameAnimSet_ScaleFParam1(GAMEANIMSET_s *set, float scale) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            if (object->instance_animation != NULL) {
                object->instance_animation->fparam1 *= scale;
            }
        }
    }
}

i32 GameAnimSet_GetAveragePos(GAMEANIMSET_s *set, NUVEC *position, i32 frame_selection, i32 include_animated,
                              i32 include_static) {
    NUVEC sum = {0.0f, 0.0f, 0.0f};
    if (position == NULL || set == NULL || set->object_count == 0 || set->objects == NULL)
        return 0;
    i32 count = 0;
    NUMTX matrix;
    GAMEANIMOBJ_s *object = set->objects;
    if (include_static == 0) {
        if (include_animated == 0) {
            do {
                object = object->next;
            } while (object != NULL);
            return 0;
        }
        do {
            if ((object->flags & 1) == 0 && object->instance_animation != NULL) {
                f32 frame;
                if (frame_selection == 0)
                    frame = object->start_frame;
                else if (frame_selection == 1)
                    frame = object->end_frame;
                else
                    frame = object->instance_animation->ltime;
                EvalAnim(&object->special, frame, &matrix, 1);
                ++count;
                NuVecAdd(&sum, &sum, NUMTX_GET_ROW_VEC(&matrix, 3));
            }
            object = object->next;
        } while (object != NULL);
    } else if (include_animated == 0) {
        do {
            if ((object->flags & 1) == 0 && object->instance_animation == NULL) {
                NuVecAdd(&sum, &sum, NuSpecialGetDrawPos(&object->special));
                ++count;
            }
            object = object->next;
        } while (object != NULL);
    } else {
        do {
            if ((object->flags & 1) == 0) {
                if (object->instance_animation != NULL) {
                    f32 frame;
                    if (frame_selection == 0)
                        frame = object->start_frame;
                    else if (frame_selection == 1)
                        frame = object->end_frame;
                    else
                        frame = object->instance_animation->ltime;
                    EvalAnim(&object->special, frame, &matrix, 1);
                    ++count;
                    NuVecAdd(&sum, &sum, NUMTX_GET_ROW_VEC(&matrix, 3));
                } else {
                    NuVecAdd(&sum, &sum, NuSpecialGetDrawPos(&object->special));
                    ++count;
                }
            }
            object = object->next;
        } while (object != NULL);
    }
    if (count == 0)
        return 0;
    NuVecScale(position, &sum, 1.0f / static_cast<f32>(count));
    return 1;
}

i32 GameAnimSet_GetCentreAndRadius(GAMEANIMSET_s *set, NUVEC *centre, f32 *radius, i32 frame_selection,
                                   i32 include_animated, i32 include_static) {
    f32 object_radius;
    if (centre == NULL || set == NULL || set->object_count == 0 || set->objects == NULL) {
        return 0;
    }

    NUVEC maximum = {-1.0e9f, -1.0e9f, -1.0e9f};
    NUVEC minimum = {1.0e9f, 1.0e9f, 1.0e9f};
    bool found_object = false;

    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        if ((object->flags & 1) != 0) {
            continue;
        }

        NUMTX matrix;
        if (object->instance_animation == NULL) {
            if (include_static == 0) {
                continue;
            }
            NUMTX *draw_matrix = NuSpecialGetDrawMtx(&object->special);
            if (draw_matrix == NULL) {
                continue;
            }
            matrix = *draw_matrix;
        } else {
            if (include_animated == 0) {
                continue;
            }
            f32 frame;
            if (frame_selection == 0) {
                frame = object->start_frame;
            } else if (frame_selection == 1) {
                frame = object->end_frame;
            } else {
                frame = object->instance_animation->ltime;
            }
            EvalAnim(&object->special, frame, &matrix, 1);
        }

        NUVEC4 object_centre;

        NuSpecialGetRadius(&object->special, reinterpret_cast<NUVEC *>(&object_centre), &object_radius);
        object_radius *= 0.75f;
        object_centre.w = 1.0f;
        NuVec4MtxTransformVU0(&object_centre, &object_centre, &matrix);

        const NUVEC object_minimum = {
            object_centre.x - object_radius,
            object_centre.y - object_radius,
            object_centre.z - object_radius,
        };
        const NUVEC object_maximum = {
            object_centre.x + object_radius,
            object_centre.y + object_radius,
            object_centre.z + object_radius,
        };
        if (object_minimum.x < minimum.x) {
            minimum.x = object_minimum.x;
        }
        if (object_minimum.y < minimum.y) {
            minimum.y = object_minimum.y;
        }
        if (object_minimum.z < minimum.z) {
            minimum.z = object_minimum.z;
        }
        if (object_maximum.x > maximum.x) {
            maximum.x = object_maximum.x;
        }
        if (object_maximum.y > maximum.y) {
            maximum.y = object_maximum.y;
        }
        if (object_maximum.z > maximum.z) {
            maximum.z = object_maximum.z;
        }
        found_object = true;
    }

    if (!found_object) {
        return 0;
    }

    centre->x = (maximum.x + minimum.x) * 0.5f;
    centre->y = (maximum.y + minimum.y) * 0.5f;
    centre->z = (maximum.z + minimum.z) * 0.5f;
    if (radius != NULL) {
        const f32 half_x = (maximum.x - minimum.x) * 0.5f;
        const f32 half_y = (maximum.y - minimum.y) * 0.5f;
        const f32 half_z = (maximum.z - minimum.z) * 0.5f;
        *radius = half_x * half_x + half_y * half_y + half_z * half_z;
        *radius = NuFsqrt(*radius);
    }
    return 1;
}

i32 GameAnimSet_IsAnimationReset(GAMEANIMSET_s *set) {
    if (set == NULL || set->objects == NULL) {
        return 1;
    }

    i32 animated_count = 0;
    i32 repeating_count = 0;
    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        nuinstanim_s *animation = object->instance_animation;
        if (animation == NULL) {
            continue;
        }

        ++animated_count;
        if (animation->repeating != 0) {
            ++repeating_count;
            continue;
        }
        if (animation->playing != 0) {
            return 0;
        }

        const f32 direction = object->end_frame < object->start_frame ? -1.0f : 1.0f;
        if (animation->ltime * direction > object->start_frame * direction) {
            return 0;
        }
    }

    if (animated_count == 0) {
        return 1;
    }
    return animated_count != repeating_count;
}

i32 GameAnimSet_Reset(GAMEANIMSET_s *set) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            nuinstanim_s *animation = object->instance_animation;
            if (animation != NULL) {
                animation->playing = 0;
                animation->waiting = 0;
                animation->ltime = object->start_frame;
                animation->tfactor = 1.0f;
            }
            GameAnimSet_RemoveFromSystemList(set);
        }
    }
    return 1;
}

i32 GameAnimSet_Play(GAMEANIMSET_s *set, float speed, i32 evaluate_state) {
    if (set == NULL) {
        return 1;
    }

    if (evaluate_state != 0) {
        GameAnimSet_EvaluateState(set);
    }
    set->flags = static_cast<GAMEANIMSET_FLAGS>(set->flags & ~GAMEANIMSET_FLAG_STOP_REQUESTED);

    if (speed >= 0.0f) {
        if (set->state == GAMEANIMSET_STATE_AT_END) {
            return 1;
        }
    } else if (speed < 0.0f) {
        if (set->state == GAMEANIMSET_STATE_AT_START) {
            return 1;
        }
    }

    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        nuinstanim_s *animation = object->instance_animation;
        if (animation == NULL) {
            continue;
        }

        animation->playing = 1;
        animation->waiting = 0;

        f32 direction;
        if (object->end_frame < object->start_frame) {
            direction = -1.0f;
        } else {
            direction = 1.0f;
        }
        f32 current_frame = animation->ltime * direction;
        if (object->start_frame * direction > current_frame) {
            animation->ltime = object->start_frame;
        } else if (current_frame > object->end_frame * direction) {
            animation->ltime = object->end_frame;
        }

        if (animation->fparam1 != 0.0f) {
            animation->tfactor = animation->fparam1 * speed * direction;
        } else {
            animation->tfactor = direction * speed;
        }
    }

    if ((set->flags & GAMEANIMSET_FLAG_IN_SYSTEM_LIST) == 0) {
        GameAnimSet_AddToSystemList(set);
    }
    return 1;
}

i32 GameAnimSet_Stop(GAMEANIMSET_s *set) {
    if (set == NULL) {
        return 1;
    }

    GAMEANIMSET_STATE state = static_cast<GAMEANIMSET_STATE>(set->state & ~GAMEANIMSET_STATE_AT_END);
    if (state != GAMEANIMSET_STATE_ACTIVE_FORWARD) {
        return 1;
    }

    set->flags = static_cast<GAMEANIMSET_FLAGS>(set->flags | GAMEANIMSET_FLAG_STOP_REQUESTED);
    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        if (object->instance_animation != NULL) {
            object->instance_animation->playing = 0;
        }
    }
    return 1;
}

i32 GameAnimSet_SetRepeating(GAMEANIMSET_s *set, i32 repeating) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            if (object->instance_animation != NULL) {
                object->instance_animation->repeating = repeating & 1;
            }
        }
    }
    return 1;
}

i32 GameAnimSet_JumpToEnd(GAMEANIMSET_s *set) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            if (object->instance_animation != NULL) {
                object->instance_animation->playing = 0;
                object->instance_animation->ltime = object->end_frame;
            }
        }
    }
    return 1;
}

i32 GameAnimSet_JumpToStart(GAMEANIMSET_s *set) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            if (object->instance_animation != NULL) {
                object->instance_animation->playing = 0;
                object->instance_animation->ltime = object->start_frame;
            }
        }
    }
    return 1;
}

void GameAnimSet_JumpToAnimPos(GAMEANIMSET_s *set, float position) {
    if (set == NULL) {
        return;
    }

    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        nuinstanim_s *animation = object->instance_animation;
        if (animation == NULL || object->animation == NULL) {
            continue;
        }

        animation->playing = 0;
        f32 frame = (object->end_frame - object->start_frame) * position + object->start_frame;
        f32 direction = object->end_frame < object->start_frame ? -1.0f : 1.0f;
        f32 scaled_frame = frame * direction;
        f32 scaled_end = object->end_frame * direction;
        if (scaled_frame <= scaled_end) {
            animation->ltime = frame;
        } else {
            animation->ltime = object->end_frame;
            scaled_frame = scaled_end;
        }
        if (object->start_frame * direction > scaled_frame) {
            animation->ltime = object->start_frame;
        }
    }
}

i32 GameAnimSet_Playing(GAMEANIMSET_s *set) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            if (object->instance_animation != NULL && object->instance_animation->playing == 0) {
                return 0;
            }
        }
    }
    return 1;
}

f32 GameAnimSet_GetCurrentFrame(GAMEANIMSET_s *set) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            if (object->instance_animation != NULL) {
                return object->instance_animation->ltime;
            }
        }
    }
    return 0.0f;
}

f32 GameAnimSet_GetAnimPos(GAMEANIMOBJ_s *object) {
    if (object == NULL || object->instance_animation == NULL || object->animation == NULL) {
        return 0.0f;
    }

    if (object->start_frame == object->end_frame) {
        return 1.0f;
    }

    f32 position =
        (object->instance_animation->ltime - object->start_frame) / (object->end_frame - object->start_frame);
    if (position < 0.0f) {
        return 0.0f;
    }
    if (position > 1.0f) {
        position = 1.0f;
    }
    return position;
}

void GameAnimSet_SetAnimPos(GAMEANIMOBJ_s *object, float position) {
    if (object == NULL || object->instance_animation == NULL || object->animation == NULL) {
        return;
    }

    if (position < 0.0f) {
        position = 0.0f;
    }
    if (position > 1.0f) {
        position = 1.0f;
    }
    object->instance_animation->ltime = (object->end_frame - object->start_frame) * position + object->start_frame;
}

void GameAnimSet_SetOffset(GAMEANIMSET_s *set, NUVEC *offset) {
    if (set == NULL) {
        return;
    }

    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        if ((object->flags & 4) != 0) {
            continue;
        }
        NUMTX *source = NuSpecialGetMtx(&object->special);
        if (source == NULL) {
            continue;
        }

        NUMTX matrix = *source;
        matrix.m30 += offset->x;
        matrix.m31 += offset->y;
        matrix.m32 += offset->z;
        NuSpecialSetDrawMtx(&object->special, &matrix);
    }
}

f32 GameAnimSet_GetCompletionRatio(GAMEANIMSET_s *set) {
    if (set == NULL) {
        return 0.0f;
    }
    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        if (object->instance_animation != NULL) {
            if (object->start_frame == object->end_frame) {
                return 1.0f;
            }
            f32 ratio =
                (object->instance_animation->ltime - object->start_frame) / (object->end_frame - object->start_frame);
            if (ratio > 1.0f) {
                ratio = 1.0f;
            }
            if (ratio < 0.0f) {
                ratio = 0.0f;
            }
            return ratio;
        }
    }
    return 0.0f;
}

GAMEANIMSET_VISIBILITY GameAnimSet_GetVisibility(GAMEANIMSET_s *set) {
    if (set == NULL) {
        return GAMEANIMSET_VISIBILITY_NONE;
    }

    i32 visible_count = 0;
    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        if (NuSpecialGetVisibilityFn(&object->special) == 1) {
            ++visible_count;
        }
    }

    if (visible_count == set->object_count) {
        return GAMEANIMSET_VISIBILITY_ALL;
    }
    if (visible_count > 0) {
        return GAMEANIMSET_VISIBILITY_PARTIAL;
    }
    return GAMEANIMSET_VISIBILITY_NONE;
}

void GameAnimSet_SetVisibility(GAMEANIMSET_s *set, i32 visibility) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            NuSpecialSetVisibility(&object->special, visibility);
        }
    }
}

void GameAnimSet_EvalAnim(GAMEANIMSET_s *set) {
    if (set != NULL) {
        for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
            EvalAnim2(&object->special, object->instance_animation->ltime);
        }
    }
}

f32 GameAnimSet_AutoSetReflectY(GAMEANIMSET_s *set, nuvec_s *position, numtx_s *matrix) {
    if (set != NULL && set->objects != NULL) {
        if (matrix == NULL) {
            matrix = NuSpecialGetMtx(&set->objects->special);
        }
        f32 height = matrix->m31;
        f32 ground = GameShadow(NULL, position, 5.0f, -1);
        if (ground != 2000000.0f) {
            return ground - height;
        }
    }
    return 0.0f;
}

// Original @0x4a9900.
void GameAnimSet_Draw(GAMEANIMSET_s &set) {
    for (GAMEANIMOBJ_s *object = set.objects; object != NULL; object = object->next) {
        if ((object->flags & 2) == 0 && NuSpecialGetVisibilityFn(&object->special) != 0) {
            NuSpecialDrawAt(&object->special, NuSpecialGetDrawMtx(&object->special));
        }
    }
}

void GameAnimSet_DrawReflection(GAMEANIMSET_s *set, i32 axis, float offset, numtx_s *matrix) {
    if (set == NULL || set->objects == NULL) {
        return;
    }
    if (matrix == NULL) {
        matrix = NuSpecialGetMtx(&set->objects->special);
    }
    f32 plane = offset + reinterpret_cast<f32 *>(matrix)[11 + axis];
    NuRndrStartReflectionRender(0);
    for (GAMEANIMOBJ_s *object = set->objects; object != NULL; object = object->next) {
        if ((object->flags & 2) == 0 && NuSpecialGetVisibilityFn(&object->special) != 0) {
            NUMTX_ALIGNED16 reflection;
            extern i32 MatrixReflection(NUMTX *, i32, f32, f32, NUMTX *);
            NUMTX *draw_matrix = NuSpecialGetDrawMtx(&object->special);
            if (MatrixReflection(draw_matrix, axis, plane, WORLD->current_level->unknown_0cc, &reflection) != 0) {
                NuSpecialDrawAt(&object->special, &reflection);
            }
        }
    }
    NuRndrEndReflectionRender();
}

void GameAnimSys_Update(GAMEANIMSYS_s *system) {
    if (system == NULL) {
        return;
    }

    GAMEANIMSET_s *set = reinterpret_cast<GAMEANIMSET_s *>(NuLinkedListGetHead(&system->active_sets));
    while (set != NULL) {
        GAMEANIMSET_s *next_set =
            reinterpret_cast<GAMEANIMSET_s *>(NuLinkedListGetNext(&system->active_sets, &set->links));

        const u8 was_no_visibility_test = set->flags & GAMEANIMSET_FLAG_NO_VISIBILITY_TEST;
        set->flags &= ~GAMEANIMSET_FLAG_NO_VISIBILITY_TEST;

        if ((set->flags & GAMEANIMSET_FLAG_STOP_REQUESTED) != 0) {
            set->flags &= ~(GAMEANIMSET_FLAG_NO_VISIBILITY_TEST | GAMEANIMSET_FLAG_STOP_REQUESTED);
            GameAnimSet_RemoveFromSystemList(set);
            set = next_set;
            continue;
        }

        set->state = GAMEANIMSET_STATE_AT_START;
        i32 all_playing_forward = 1;
        i32 all_at_start = 1;
        i32 all_at_end = 1;
        i32 any_special_no_visibility_test = 0;

        GAMEANIMOBJ_s *object = set->objects;
        while (object != NULL) {
            if (NuSpecialGetNoVisiTestFn(&object->special) != 0) {
                any_special_no_visibility_test = 1;
            }

            if (object->instance_animation != NULL) {
                const f32 direction = object->start_frame > object->end_frame ? -1.0f : 1.0f;
                const f32 previous_frame = object->instance_animation->ltime;

                if (previous_frame * direction >= object->end_frame * direction) {
                    if (object->instance_animation->tfactor * direction < 0.0f) {
                        object->instance_animation->ltime = object->end_frame;
                    } else if (object->instance_animation->repeating != 0) {
                        object->instance_animation->ltime = previous_frame - object->end_frame + object->start_frame;
                    } else {
                        object->instance_animation->ltime = object->end_frame;
                        object->instance_animation->playing = 0;
                    }
                } else if (previous_frame * direction <= object->start_frame * direction) {
                    if (object->instance_animation->tfactor * direction > 0.0f) {
                        object->instance_animation->ltime = object->start_frame;
                    } else if (object->instance_animation->repeating != 0) {
                        object->instance_animation->ltime = object->end_frame - (object->start_frame - previous_frame);
                    } else {
                        object->instance_animation->ltime = object->start_frame;
                        object->instance_animation->playing = 0;
                    }
                }

                if (previous_frame != object->instance_animation->ltime) {
                    EvalAnim2(&object->special, object->instance_animation->ltime);
                }

                if (object->instance_animation->playing != 0) {
                    set->flags |= GAMEANIMSET_FLAG_NO_VISIBILITY_TEST;
                    if (object->instance_animation->tfactor * direction < 0.0f) {
                        all_playing_forward = 0;
                    }
                }

                const f32 directed_frame = object->instance_animation->ltime * direction;
                if (directed_frame > object->start_frame * direction) {
                    all_at_start = 0;
                }
                if (directed_frame < object->end_frame * direction) {
                    all_at_end = 0;
                }
            }
            object = object->next;
        }

        if ((set->flags & GAMEANIMSET_FLAG_NO_VISIBILITY_TEST) != any_special_no_visibility_test) {
            object = set->objects;
            auto set_flags = set->flags;
            while (object != NULL) {
                NuSpecialSetNoVisiTest(&object->special, set_flags & GAMEANIMSET_FLAG_NO_VISIBILITY_TEST);
                object = object->next;
            }
        }

        if ((set->flags & GAMEANIMSET_FLAG_NO_VISIBILITY_TEST) != 0) {
            set->state =
                all_playing_forward != 0 ? GAMEANIMSET_STATE_ACTIVE_FORWARD : GAMEANIMSET_STATE_ACTIVE_BACKWARD;
        } else {
            if (all_at_end != 0) {
                set->state = GAMEANIMSET_STATE_AT_END;
            } else if (all_at_start == 0) {
                set->state = GAMEANIMSET_STATE_BETWEEN_ENDPOINTS;
            }
            if (was_no_visibility_test != 0) {
                set->flags |= GAMEANIMSET_FLAG_STOP_REQUESTED;
            }
        }

        if ((set->flags & (GAMEANIMSET_FLAG_NO_VISIBILITY_TEST | GAMEANIMSET_FLAG_STOP_REQUESTED)) == 0) {
            GameAnimSet_RemoveFromSystemList(set);
        }
        set = next_set;
    }
}

