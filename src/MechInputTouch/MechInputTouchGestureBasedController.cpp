// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/animation_ids.h"
#include "legoapi/ai/core/legoai.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gamelib/util/gamelib_util_types.h"
#include "nu2api/nucore/numemory.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/gizmos/transport/teleport.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmos/object/hatmachine.h"
#include "legoapi/gizmos/object/lever.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/audio/audio.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"

#include <new>
#include <string.h>
#include "legoapi/actions/combat/fighting.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/players.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/characters/core/playeritems.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/misc/utilities.h"
#include <stdlib.h>
#include "legoapi/actions/movement/jumping.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/actions/character/snake.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/traps/attracto.h"
#include "legoapi/props/objects/signal.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/props/objects/tightrope.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/menus/core/gamemessage.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/numtx_inline.h"
#include "legoapi/characters/motion/move_types.h"
#include "gameapi/edtools/edfile.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numusic/sfx.h"
#include <float.h>
#include "legoapi/characters/motion/gameanim_types.h"
#include "gameapi/ai/aisys/aipath.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/ai/game/creature.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/gizmos/traps/gizbombgen.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/menus/core/panel.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numechptr.hpp"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nu3d/nuspline.h"
#include <stdio.h>
#include "legoapi/world/levels/episodeV_types.h"
#include "gameapi/edtools/edpart_internal.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/gizmo/object/giztorpedo.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/characters/core/customiser.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nufile/nufile.h"
#include <math.h>
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/actions/character/streaks.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/menus/screens/arcade.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/gizmos/door/spinner.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/core/input/timing.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "legoapi/items/objects/gameobjects_types.h"
#include "legoapi/world/levels/episode.h"
#include "legogame/game.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/characters/core/players_types.h"
#include "legoapi/gizmos/door/push.h"
#include "legoapi/items/base/animpacket.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/cutscenes/minicamcut.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/numouse.h"
#include "legoapi/characters/motion/camera_types.h"
#include "legoapi/items/collect/spacelevel.h"
#include "gameapi/edtools/edcam.h"
#include "legoapi/actions/character/speederchase_types.h"
#include "legoapi/gizmo/base/GizBuildItObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizLeverObjectInterface.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizPanelObjectInterface.h"
#include "legoapi/gizmo/base/GizTurretObjectInterface.h"
#include "legoapi/gizmo/base/HatMachineObjectInterface.h"
#include "legoapi/gizmo/base/TeleportObjectInterface.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/gizmos/fx/edgizshadowmachine.h"
#include "legoapi/gizmo/base/gizmessage.h"
#include "legoapi/gizmos/trigger/gizrandom.h"
#include "legoapi/gizmos/trigger/giztimer.h"
#include "legoapi/gizmos/trigger/minicut.h"
#include "legoapi/gizmos/door/plugs.h"
#include "legoapi/items/collect/bolts.h"
#include "legoapi/gizmos/fx/guidelines.h"
#include "legoapi/gizmos/transport/ledges.h"
#include "legoapi/gizmos/traps/shards.h"
#include "gameframework/saveload.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "nu2api/nu3d/nuhspecial.h"
#include "legoapi/world/levels/episodeII_types.h"
#include "legoapi/misc.h"
#include "legoapi/world/levels/podrace.h"
#include "legoapi/world/levels/episodeI_types.h"
#include "legoapi/ai/core/gameai_types.h"
#include "legoapi/ai/game/creature_types.h"
#include <stdint.h>
#include "nu2api/nucore/nulist.h"
#include "gameapi/ai/aisys/aiscript_types.h"
#include "legoapi/actions/character/simpletons_types.h"
#include "editor/edpath.h"
#include "legoapi/ai/core/ai_pathgeometry.h"
#include "batman.h"
#include "legoapi/gizmo/object/takeoverobjects.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nurdp.h"
#include "nu2api/nufile/nufilepak.h"
#include "legoapi/ai/core/aisysall_types.h"
#include "editor/aieditor_state.h"
#include "editor/antinode_editor_private.h"
#include "nu2api/numath/nuplane.h"
#include "editor/aieditor_settings.h"
#include "editor/path_connections.h"
#include "gameapi/edtools/edrender.h"
#include "legoapi/render/core/terrain_internal.h"
#include "nu2api/nu3d/numtl.h"
#include "editor/edpath_types.h"
#include "legoapi/ai/game/gameantinode.h"
#include "gameapi/edtools/edbri_internal.h"
#include "legoapi/actions/character/transform.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/light/fade_material.h"
#include "nu2api/nu3d/android/nuportal_android.h"
#include "nu2api/nu3d/nuvport.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/numath/numath.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include "editor/edtoolsall_types.h"
#include "legoapi/render/core/gameliball.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nu3d/android/nurain_android.h"
#include <stddef.h>
#include "legoapi/render/core/terrain_stubs_types.h"
#include "legoapi/actions/movement/jumping_types.h"
#include <ctype.h>
#include "legoapi/world/levels/episode_types.h"
#include "legoapi/items/collect/batarang.h"
#include "legoapi/gizmo/object/gizmoblowups_types.h"
#include "legoapi/actions/character/suit.h"
#include "legoapi/world/areas.h"
#include "legoapi/actions/character/specialmoves.h"
#include "nu2api/numusic/numusic.h"
#include "legoapi/characters/core/characters_types.h"
#include "legoapi/misc/supportall.h"
#include "legoapi/render/core/screen.h"
#include <stdarg.h>
#include <limits.h>
#include "legoapi/gizmo/base/gizflow.h"
#include "legoapi/misc/supportall_types.h"
#include "MechInputTouch/MechAutoJumpManager.h"
#include "MechInputTouch/MechInputTouchGestureBasedController.h"
#include "gameapi/ai/aisys/gameaiscript.h"
#include "legoapi/actions/movement/climbing.h"
#include "legoapi/characters/motion/move.h"
#include "legoapi/items/objects/cable.h"
#include "legoapi/world/levels/episodeV.h"

extern "C" i16 id_WATTO;
bool isBucking;

// Retail clears the four stick values with one unaligned vector store.
typedef f32 StickValuesVec4 __attribute__((vector_size(16), aligned(4)));

DECOMP_ASSERT(offsetof(GameObject_s, field_0x7e4) == 0x7e4, "Quick shoot record offset");
DECOMP_ASSERT(offsetof(GameObject_s, field_0x7e8) == 0x7e8, "Quick shoot record count offset");
DECOMP_ASSERT(offsetof(GameObject_s, quick_shoot_flags) == 0xe2f, "Quick shoot flags offset");
DECOMP_ASSERT(offsetof(GameObject_s, quick_shoot_bolt_id) == 0xe3e, "Quick shoot bolt ID offset");

// Retail keeps several of this file's static helpers only as GCC clones; these
// declarations pin the clone names and register conventions.
#if defined(__i386__)
#define MOVE_CLONE_ATTR1 __attribute__((noinline, noclone, regparm(1)))
#define MOVE_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define MOVE_CLONE_ATTR1 __attribute__((noinline, noclone))
#define MOVE_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif

// Retail splits the guard off as an inline head; the rest is this partial clone.

extern i32 LevFlag[4];

// Retail re-tests the three block animations at each use instead of caching a flag.
#define HAS_BLOCK_ANIMS(animations)                                                                                    \
    ((animations)[0x1a] != NULL || (animations)[0x1b] != NULL || (animations)[0x1c] != NULL)

// Retail splits the context test off as an inline head; the rest is this partial clone.

DECOMP_ASSERT(sizeof(AWKWARDSHAPESPHERE_s) == 12, "Awkward shape sphere size");
DECOMP_ASSERT(offsetof(AWKWARDSHAPESPHERE_s, radius) == 8, "Awkward shape sphere radius offset");

// Retail uses translation-unit-local helpers with compiler-inferred private
// calling conventions; keep their shared animation-mode call sites intact.

DECOMP_ASSERT(sizeof(HOTHBATTLEE_NETPACKET_s) == 0x58, "Hoth panel packet size");
DECOMP_ASSERT(offsetof(HOTHBATTLEE_NETPACKET_s, alpha) == 0x24, "Hoth panel packet alpha offset");
DECOMP_ASSERT(offsetof(HOTHBATTLEE_NETPACKET_s, count) == 0x54, "Hoth panel packet count offset");

// Asteroid chase (AsteroidChase_A / B / C / D)

DECOMP_ASSERT(sizeof(ASTEROID_s) == 0x18, "ASTEROID_s size");

DECOMP_ASSERT(sizeof(FINALASTEROID_s) == 0x3c, "final asteroid size");
DECOMP_ASSERT(offsetof(FINALASTEROID_s, blowup_count) == 0x2c, "final asteroid blowup count offset");
DECOMP_ASSERT(offsetof(FINALASTEROID_s, rotation_x) == 0x34, "final asteroid rotation offset");

DECOMP_ASSERT(sizeof(ASTEROIDCNETPACKET_s) == 8, "asteroid C network packet size");

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

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

namespace {

} // namespace

DECOMP_ASSERT(sizeof(SPECIAL_LAYER_s) == 0xc, "SPECIAL_LAYER_s size");

// Original data symbol testmovef is float0.005 at 0x617cb0.

DECOMP_ASSERT(sizeof(VIEWCAM_s) == 0x28, "ViewCam size");
DECOMP_ASSERT(offsetof(VIEWCAM_s, pitch) == 0x10, "ViewCam pitch offset");
DECOMP_ASSERT(offsetof(VIEWCAM_s, gamepad) == 0x24, "ViewCam gamepad offset");

DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, path) == 0x40, "Camera context path offset");
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, flags) == 0x4c, "Camera context flags offset");

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(SPEEDERCHASEVALUES) == 0x1c, "Speeder chase tuning stride");
DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

struct FLOWBOX_s;

DECOMP_ASSERT(sizeof(GIZMOTYPES) == 0xc, "Gizmo type-table header ABI");
DECOMP_ASSERT(offsetof(GIZMOTYPES, types) == 8, "Gizmo type-table pointer offset");

static_assert(sizeof(struct GUNSHIP_LEVFLAG_s) == 16, "LevFlag must be 16 bytes");

// Defined in gameapi/edtools/edtoolsall.cpp, which has no header yet.

struct JEDIBNETPACKET_s;

// --- File-local statics (original _ZL... symbols; not renamed) ---------------

DECOMP_ASSERT(sizeof(KAMINODISCO_s) == 1000, "Kamino disco original layout");

DECOMP_ASSERT(sizeof(DOOKU_C_s) == 20, "Dooku C state ABI");

DECOMP_ASSERT(sizeof(KAMINO_E_s) == 120, "Kamino E state ABI");

// Jedi (Jedi_B)

DECOMP_ASSERT(sizeof(JEDIB_CREATURE_s) == 0x60, "JEDIB_CREATURE_s ABI");

DECOMP_ASSERT(sizeof(JEDIB_s) == 0x63ec, "JEDIB_s ABI");

DECOMP_ASSERT(sizeof(JEDIBNETPACKET_s) == 0x20, "JEDIBNETPACKET_s ABI");

// Dooku (Dooku_C)

static_assert(sizeof(struct LEVFLAGBYTES_s) == 16, "LevFlag must be 16 bytes");

// PodSprint_InitAISpline is partially inlined into PodSprintA_Init: the
// clear, lookup and spline checks are inlined and the crossing scan is the
// split-out body.
#if defined(__i386__)
#define PODSPRINT_AISPLINE_BODY_ATTR __attribute__((noinline, regparm(1)))
#else
#define PODSPRINT_AISPLINE_BODY_ATTR __attribute__((noinline))
#endif

// Helpers defined in other original translation units but kept here until
// they get their own home (their originals live outside the ep1 TU).

struct nuhspecial_s;
struct nuvec_s;

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
#include "legoapi/ai/core/ai_sys_stubs_types.h"
    DECOMP_ASSERT(sizeof(RebuildEditorCreature) == 0xac, "editor creature rebuild stride");
    DECOMP_ASSERT(offsetof(RebuildEditorCreature, path_check) == 0x38, "editor creature path offset");
    DECOMP_ASSERT(offsetof(RebuildEditorCreature, activation_area) == 0x6c, "editor creature area offset");
    DECOMP_ASSERT(offsetof(RebuildEditorCreature, activation) == 0x8f, "editor creature activation offset");
} // extern "C"

#if defined(__i386__)
#define AI_SYS_STUBS_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define AI_SYS_STUBS_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif

struct nupad_s;

DECOMP_ASSERT(sizeof(EDAIAREA_s) == 0x44, "area editor record size");

// Item colours shared by the AI editors.

DECOMP_ASSERT(sizeof(AIPATHCNXTYPE_s) == 0x4c, "editor path connection type size");
DECOMP_ASSERT(offsetof(AIPATHCNXTYPE_s, flags) == 0x48, "editor path connection options offset");

// Retail splits the null checks into each caller and keeps the body as a
// partial-inlining clone; its connection searches are fully peeled.

struct EDAISHAREDPATHNODE_s;
struct eduimenu_s;
struct eduiitem_s;

DECOMP_ASSERT(offsetof(EdUiNameInputItem, name) == 0x4c, "editor name input offset");
DECOMP_ASSERT(offsetof(EdUiNameInputItem, max_name_length) == 0x15a, "editor name input limit offset");

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

struct EDCREATURE_s;

DECOMP_ASSERT(offsetof(CreatureEditorRecord, valid_positions) == 0x54, "creature valid position mask offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, position) == 0x28, "creature position offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, script_name) == 0x18, "creature script name offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, script_params) == 0x70, "creature script params offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, angle) == 0x34, "creature angle offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, character_type) == 0x58, "creature character type offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, path) == 0x3c, "creature path offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, set) == 0x5a, "creature set offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, flags) == 0x68, "creature flags offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, activation_area) == 0x6c, "creature activation area offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, trigger_area) == 0x80, "creature trigger area offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, activation) == 0x8f, "creature activation offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, view_distance) == 0x9c, "creature view distance offset");
DECOMP_ASSERT(offsetof(CreatureEditorRecord, negative_min_view_height) == 0xa8, "creature min view height offset");
DECOMP_ASSERT(sizeof(CreatureEditorRecord) == 0xac, "editor creature record size");

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
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

namespace {
#include "legoapi/gizmo/gizmos/gizmos_gizactions_types.h"

} // namespace

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

struct AIPACKET_s;
struct AISYS_s;

// NuFileExists @nufile (nucore_plain.cpp).

struct GIZMOPICKUP_s;
struct PART_s;

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

DECOMP_ASSERT(sizeof(TROOPERCANNON_s) == 0x30, "Trooper cannon state size");
DECOMP_ASSERT(offsetof(TROOPERCANNON_s, character_name) == 0x0c, "Trooper cannon name offset");
DECOMP_ASSERT(offsetof(TROOPERCANNON_s, rebuilding) == 0x2c, "Trooper cannon rebuilding offset");

// Original writable flagremaptab at 0x668520, 32 pairs of 32-bit masks.
extern "C" {
extern u32 flagremaptab[32][2];
}

// Definitions from the original gizmoblowups translation unit.

DECOMP_ASSERT(sizeof(BLOWUPPROGRESS_s) == 0x100, "BLOWUPPROGRESS_s ABI");

DECOMP_ASSERT(offsetof(WORLDINFO_s, blowup_target_candidates) == 0x50d4, "Blowup target candidate-list ABI");

// LoadPerm1 is one of the few game-level entry points which wires together
// otherwise C-linkage engine subsystems.  Keep these declarations local: the
// individual subsystem TUs intentionally expose their original plain names.

// Retail keeps only the IPA-SRA clone, which receives &*layers.
#if defined(__i386__)
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone, regparm(1)))
#else
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone))
#endif

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

// The reference defines these with dynamic initialization in this unit.

DECOMP_ASSERT(sizeof(TIMINGBAR_LABEL) == 12, "Timing-bar label size");

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

void MechInputTouchGestureBasedController::Render() {
}

void MechInputTouchGestureBasedController::Deactivate() {
    if (!active) {
        return;
    }
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
        Obj[i].KillTasks();
    }
    active = 0;
    MechSystems::Get()->gesture_tracking_system.UnregisterGestureTracker(*this);
    if (player != NULL) {
        player->jump_input_flags &= ~0x20;
    }
    buttons_were_pressed = 0;
    buttons_pressed = 0;
    buttons_repeat = 0;
}

void MechInputTouchGestureBasedController::Activate() {
    if (active) {
        return;
    }
    if (field_90 != NULL && !field_90->is_down) {
        field_90 = NULL;
    }
    if (field_94 != NULL && !field_94->is_down) {
        field_94 = NULL;
    }
    active = 1;
    MechSystems::Get()->gesture_tracking_system.RegisterGestureTracker(*this, 150);
    field_90 = NULL;
    MechSystems::Get()->gesture_controller = this;
}

MechInputTouchGestureBasedController::~MechInputTouchGestureBasedController() {
}

MechInputTouchGestureBasedController::MechInputTouchGestureBasedController(
    i32 index, MechInputTouchGestureBasedController::StickMode mode)
    : MechInputTouchMainController(index), temporary_position(), stick_mode(mode), field_a5(0), tag_button(NULL) {
    active = 0;
    field_a6 = 0;
    field_98 = 0.0f;
    smart_bomb_touch = NULL;
    field_90 = NULL;
    field_94 = NULL;
    field_9c = 0.0f;
    field_a0 = 0.0f;
}

bool MechInputTouchGestureBasedController::MenuDisable() {
    const i32 menu_id = GetMenuID();
    if (menu_id == 12 || menu_id == 16 || menu_id == 13 || menu_id == 17 || menu_id == 8 || menu_id == 18) {
        return true;
    }
    return menu_id == 14;
}

bool IsDownSwipe(NuVec2 const &start, NuVec2 const &end) {
    return abs(RotDiff(NuAtan2D(end.x - start.x, end.y - start.y), 0x8000)) < 0x1c72;
}

void MechInputTouchGestureBasedController::KillTasks(bool kill_active_task) {
    GameObject_s *object = Player[player_id];
    if (object == NULL) {
        return;
    }
    if (object->touch_task == NULL) {
        return;
    }
    if ((object->touch_task->flags & 2) != 0 && !kill_active_task) {
        return;
    }
    object->KillTasks();
}

void MechInputTouchGestureBasedController::StartNewTask(MechTouchTask *task, TouchHolder &holder, bool clear_touches,
                                                        bool kill_tasks) {
    GameObject_s *object = Player[player_id];
    if (object == NULL) {
        return;
    }
    if (object->touch_task != NULL && (object->touch_task->flags & 2) != 0) {
        delete task;
        return;
    }
    if (kill_tasks) {
        KillTasks(false);
    }
    if (object->touch_task != NULL && task != NULL) {
        object->touch_task->OnSuspend();
        task->next = object->touch_task;
    }
    object->touch_task = task;
    if (clear_touches) {
        field_90 = NULL;
        field_94 = NULL;
    }
    if (task != NULL) {
        task->character = Player[player_id];
        task->touch_holder = &holder;
        object->touch_task->OnStart();
    }
}

bool MechInputTouchGestureBasedController::OnRelease(GameObject_s &object, TouchHolder &holder) {
    if (smart_bomb_touch == &holder) {
        smart_bomb_touch = NULL;
    }
    if (field_94 == &holder) {
        field_94 = NULL;
    }
    if (field_90 == &holder) {
        if (((object.apiobj.character_data != NULL && (object.apiobj.character_data->model_flags & 0x40) != 0) ||
             object.id == id_WATTO) &&
            object.character_context == LEGOCONTEXT_JUMP) {
            KillTasks(true);
        }
        CABLE_s *cable = GameObjOwnsAnyCables(&object);
        if (cable != NULL) {
            if (cable->target != NULL && TouchHacks::FindBombTarget(object) != NULL) {
                MechObjectInterface *target_object = cable->target->GetMechObjectInterface();
                CantPickupBombTimerAddon *addon = NU_ALLOC_T(CantPickupBombTimerAddon, 1, "", 0);
                if (addon != NULL) {
                    new (addon) CantPickupBombTimerAddon(*target_object, 6.0f);
                }
                cable->target->GetAddons(true)->Add(*addon);
            }
            ReleaseCable(cable, 0);
        }
        field_90 = NULL;
        field_a5 = 0;
    }
    if (object.touch_task != NULL && (object.touch_task->flags & 1) != 0 &&
        object.touch_task->touch_holder == &holder) {
        KillTasks(true);
        StartNewTask(NULL, holder, false, true);
    }
    return false;
}

bool MechInputTouchGestureBasedController::OnDown(GameObject_s &object, TouchHolder &holder) {
    if (object.apiobj.character_data == NULL || object.apiobj.character_data->player_config == NULL) {
        return false;
    }
    VuVec position(holder.down_position.x, holder.down_position.y, 0.0f, 1.0f);
    MechObjectInterface *force_target = MechInputTouchSystem::FindTargetObject(object, position, 0x2010, NULL, NULL);
    if (force_target != NULL) {
        StartNewTask(new MechTouchTaskUseForce(*this, force_target, position), holder, true, true);
        if (object.touch_task != NULL && object.touch_task->GetHashId().value == MechTouchTaskUseForce::HashId.value) {
            object.touch_task->Update();
        }
        return true;
    }
    if ((object.incoming_bolt != NULL || (object.field_0xe21 & 0x20) != 0) && holder.target_object.Get() != NULL &&
        holder.target_object->GetCharacterObject() == &object) {
        button_was_pressed[0] = 1;
        return true;
    }
    if (field_90 != NULL) {
        if (field_94 == NULL) {
            const f32 dy = holder.touch_position.y - field_90->touch_position.y;
            const f32 dx = holder.touch_position.x - field_90->touch_position.x;
            if (dx * dx + dy * dy < 1.0f) {
                field_94 = &holder;
            }
        }
    } else {
        field_a0 = 0.0f;
        field_a5 = 0;
        field_90 = &holder;
    }
    if (smart_bomb_touch == NULL && player != NULL && holder.target_object.Get() == player->GetMechObjectInterface() &&
        TouchHacks::CanUseVehicleSmartBomb(object)) {
        smart_bomb_touch = &holder;
    }
    return true;
}

bool MechInputTouchGestureBasedController::OnHold(GameObject_s &object, TouchHolder &holder) {
    if (object.apiobj.character_data == NULL || object.apiobj.character_data->player_config == NULL ||
        (object.field_0xcc0 != NULL && object.field_0xcc0->id != id_YODA)) {
        return false;
    }
    MechObjectInterface *target = holder.target_object.Get();
    if (target == NULL || object.touch_task != NULL) {
        return false;
    }
    VuVec position __attribute__((aligned(16)))(holder.down_position.x, holder.down_position.y, 0.0f, 1.0f);
    if (holder.target_object->GetObjectType() == 12) {
        void *force_target = object.force_glow_object;
        if (force_target != NULL && holder.target_object->GetPart() == force_target) {
            target = holder.target_object.Get();
            StartNewTask(new MechTouchTaskUseForce(*this, target, position), holder, true, true);
            holder.consumed = 1;
            return true;
        }
    }
    const i32 type = holder.target_object->GetObjectType();
    if (type == 5) {
        holder.target_object->TargetedFlash();
        target = holder.target_object.Get();
        StartNewTask(new MechTouchTaskUseForce(*this, target, position), holder, true, true);
        holder.consumed = 1;
        return true;
    }
    if (type == 6) {
        holder.target_object->TargetedFlash();
        target = holder.target_object.Get();
        StartNewTask(new MechTouchTaskBuildIt(*this, target, position), holder, true, true);
        holder.consumed = 1;
        return true;
    }
    if (type != 2) {
        return false;
    }
    GameObject_s *character = holder.target_object->GetCharacterObject();
    if (character == NULL) {
        return false;
    }
    if (character->apiobj.character_data == NULL) {
        holder.consumed = 1;
        return true;
    }
    if ((character->apiobj.character_data->player_config->flags_090 & 0x40) != 0 && character->field_0xcc0 == NULL &&
        TouchHacks::CanTagVehicle(object, *character)) {
        StartNewTask(new MechTouchTaskTag(*this, *character), holder, true, true);
    } else if (VehicleArea == 0 && TouchHacks::CanTagTo(object, *character)) {
        if (!(InCollectList_Index(character->id, NULL, 0) != -1 &&
              ((WORLD != NULL && WORLD->area == HUB_ADATA) || TouchHacks::InParty(*character)))) {
            GameAudio_PlaySfx(0x32, &object.apiobj.collision_position, 0, 0);
            if (object.pad_gamepad != NULL) {
                NewRumble(object.pad_gamepad->pad, 0.5f, 0);
            }
            if (character->ai.script_process.base_script != NULL &&
                AIScriptSetBaseScriptStateByName(&character->ai.script_process, const_cast<char *>("MapRunAway")) !=
                    0) {
                const f32 elapsed = FRAMETIME;
                WORLDINFO_s *world = WORLD;
                if (world != NULL && world->ai_sys != NULL) {
                    AIScriptProcess(world->ai_sys, &character->apiobj, &character->ai, &character->ai.script_process,
                                    elapsed);
                }
            }
        } else {
            tag_button = MechSystems::Get()->NewTagButton(*character, holder);
        }
    } else if (character == object.force_glow_object) {
        target = holder.target_object.Get();
        StartNewTask(new MechTouchTaskUseForce(*this, target, position), holder, true, true);
    } else if (character->apiobj.field_0x27c == -1 && static_cast<i32>(character->apiobj.field_0x1f4) >= 0) {
        target = holder.target_object.Get();
        StartNewTask(new MechTouchTaskAttack(*this, target, position), holder, true, true);
    } else if (character == &object) {
        StartNewTask(new MechTouchTaskBlock(*this), holder, false, true);
    } else if (dagobah_training != 0 && character->id == id_YODA && WORLD->area == DAGOBAH_ADATA) {
        StartNewTask(new MechTouchTaskTag(*this, *character), holder, true, true);
    }
    holder.consumed = 1;
    return true;
}

void MechInputTouchGestureBasedController::ProcessDragMovement(GameObject_s &object) {
    if (field_90 == NULL) {
        return;
    }
    f32 origin_x = field_90->down_position.x;
    f32 origin_y = field_90->down_position.y;
    if (object.id != id_GRABCONTROL && stick_mode.value == 0) {
        origin_x = MAX(-0.6f, (MIN(object.camera_screen_position.x, 0.6f)));
        origin_y = MAX(-0.6f, (MIN(object.camera_screen_position.y, 0.6f)));
    }
    NuVec2 drag = {origin_x - field_90->touch_position.x, origin_y - field_90->touch_position.y};
    if (field_94 != NULL) {
        const NuVec2 other_drag = {origin_x - field_94->touch_position.x, origin_y - field_94->touch_position.y};
        if (drag.x * drag.x + drag.y * drag.y < other_drag.x * other_drag.x + other_drag.y * other_drag.y) {
            drag = other_drag;
        }
    }
    const f32 distance = NuFsqrt(drag.y * drag.y + drag.x * drag.x);
    bool moving = distance > 0.05f;
    const NuVec2 down_drag = {field_90->down_position.x - field_90->touch_position.x,
                              field_90->down_position.y - field_90->touch_position.y};
    if (field_90->target_object.Get() == NULL && isBucking) {
        moving = true;
    }
    const bool dragged = down_drag.x * down_drag.x + down_drag.y * down_drag.y > 0.0025f;
    if (!dragged) {
        if (field_90->held_time > 0.2f || field_90->target_object.Get() == NULL) {
            if (object.character_context != -1 && (object.character_context != LEGOCONTEXT_JUMP || !isBucking)) {
                moving = false;
            }
        } else {
            moving = false;
        }
    }
    field_a0 -= FRAMETIME;
    if (field_a0 > 0.0f) {
        moving = false;
    }
    const i32 context = object.character_context;
    if (context == 0x2b || context == 0x0f || context == 0x36 || context == 0x2a || context == 0x4a ||
        context == 0x47 || context == 0x22 || context == 0x61 || context == 0x3c || tag_button.Get() != NULL ||
        !moving) {
        field_9c = 0.0f;
        return;
    }
    if (!(object.touch_task != NULL && (object.touch_task->flags & 4) != 0) && context != 0x27 && context != 0x33 &&
        context != 0x18 && context != 0x0c && context != LEGOCONTEXT_TUBE && context != LEGOCONTEXT_PUSHSPINNER &&
        context != LEGOCONTEXT_JUMP && context != LEGOCONTEXT_BIGJUMP) {
        object.character_context = -1;
    }
    field_a5 = 1;
    KillTasks(false);
    field_90->consumed = 1;
    if (field_94 != NULL) {
        field_94->consumed = 1;
    }
    if (object.touch_task != NULL && (object.touch_task->flags & 8) == 0) {
        return;
    }
    const i32 angle = NuAtan2D(drag.x, drag.y);
    const f32 strength = (MAX(0.0f, (MIN((distance - 0.05f) * 4.0f, 1.0f)))) * 1.4f;
    const f32 stick_x = -(strength * NU_SIN_LUT(angle));
    const f32 stick_y = strength * NU_COS_LUT(angle);
    stick_values[0] = MAX(-1.0f, (MIN(stick_x, 1.0f)));
    stick_values[1] = MAX(-1.0f, (MIN(stick_y, 1.0f)));
    field_9c += FRAMETIME;
    if (field_9c > 1.0f) {
        Hint_SetComplete(0x5f3);
    }
    if (field_94 != NULL) {
        stick_values[2] = stick_values[0];
        stick_values[0] = 0.0f;
        stick_values[3] = stick_values[1];
        stick_values[1] = 0.0f;
    }
}

bool MechInputTouchGestureBasedController::StartJumpUsingAIPath(JumpTriggerPacket const &packet, i32 heading) {
    MechAutoJumpConnection *jump = MechAutoJumpGetBest(packet, heading);
    if (jump == NULL) {
        return false;
    }

    const i32 direction = jump->direction;
    AIPATHCNX *connection = jump->connection;
    AIPATHNODE &node = jump->path->nodes[connection->node_indices[direction == 0]];
    GameObject_s *object = Player[player_id];
    const u32 flags = connection->traversal_flags[direction];
    bool complete_success_hint;
    i32 animation;
    const u32 high_jump = flags & (LEGO_AIPATHCNX_HIGH_JUMP | 0x800000);
    if (high_jump != 0 && (object->ai.capabilities & high_jump) != 0) {
        animation = 3;
        complete_success_hint = true;
    } else {
        const u32 double_jump = flags & (LEGO_AIPATHCNX_DOUBLE_JUMP | 0x400000);
        if (double_jump == 0) {
            animation = 1;
            complete_success_hint = false;
        } else {
            const bool capable = (object->ai.capabilities & double_jump) != 0;
            animation = capable ? 2 : 1;
            complete_success_hint = capable;
        }
    }

    if (LEGOACT_FLIP != -1 && object->apiobj.character_model->model_data_b[LEGOACT_FLIP] != NULL) {
        const u16 desired = connection->route_mask - (direction != 0 ? 0x8000 : 0);
        const i32 difference = RotDiff(object->apiobj.facing_angle, desired);
        const i32 absolute_difference = difference < 0 ? -difference : difference;
        if (absolute_difference >= 0x6aab) {
            animation = 4;
        }
    }

    TouchHolder *holder = packet.touch_holder;
    MechTouchTaskBigJump *task = new MechTouchTaskBigJump(*this, node.position, static_cast<i8>(animation));
    StartNewTask(task, *holder, false, false);
    MechAutoJumpSetIsUsing(*object, *jump);
    if (complete_success_hint) {
        Hint_SetComplete(id_HINT_LSW_AUTOJUMP);
    }
    Hint_SetComplete(id_HINT_LSW_AUTOJUMP_FAIL);
    return true;
}

bool MechInputTouchGestureBasedController::TriggerJumpTask(JumpTriggerPacket const &packet, bool disable_autopilot,
                                                           bool use_velocity, bool try_ai_path) {
    GameObject_s *object = packet.player;
    GameObject_s *original_object = object;
    i32 context = object->character_context;
    if (context == LEGOCONTEXT_JUMP) {
        return false;
    }
    if (object->apiobj.field_0x27d == 0) {
        if (VehicleArea == 0 && object->field_0xcc0 == NULL) {
            return false;
        }
    } else if (VehicleArea == 0 && object->field_0xcc0 == NULL) {
        if (!TouchHacks::CanJump(*object))
            return false;
        object = packet.player;
        context = object->character_context;
    }
    if (context != -1 && context != LEGOCONTEXT_COMBO && context != LEGOCONTEXT_PUNCH && context != LEGOCONTEXT_BLOCK &&
        context != LEGOCONTEXT_HOLD) {
        return true;
    }
    if (object->apiobj.field_0x27d == 0) {
        if (!ObjLandReady(object) && VehicleArea == 0)
            return true;
        object = packet.player;
    }

    const i32 model_flags = object->apiobj.character_data->model_flags;
    TouchHolder *holder;
    MechTouchTask *task;
    if ((model_flags & 0x40) != 0 || original_object->id == id_WATTO) {
        holder = packet.touch_holder;
        task = new MechTouchTaskAstroJetPack(*this);
    } else {
        if (try_ai_path && StartJumpUsingAIPath(packet, object->apiobj.facing_angle))
            return true;
        holder = packet.touch_holder;
        task = new MechTouchTaskJump(*this, packet, disable_autopilot, use_velocity);
    }
    StartNewTask(task, *holder, false, false);
    return true;
}

bool MechInputTouchGestureBasedController::OnSwipe(GameObject_s &object, TouchHolder &holder, i32 index) {
    if (object.apiobj.character_data == NULL || object.apiobj.character_data->player_config == NULL) {
        return false;
    }
    if (ZipUp_FindNearest(WORLD, &object.apiobj.position, object.field_0x1008, NULL, NULL, &object, true) != NULL) {
        StartNewTask(new MechTouchTaskUseZipUp(*this), holder, false, true);
        MechSystems::Get()->NewSwipeMarker(holder, index, static_cast<SwipeDecalRenderer::Style>(0));
        return true;
    }
    if (object.id == id_GRABCONTROL && IsDownSwipe(holder.swipe_samples[index].position, holder.touch_position)) {
        button_was_pressed[0] = 1;
        MechSystems::Get()->NewSwipeMarker(holder, index, static_cast<SwipeDecalRenderer::Style>(0));
        return true;
    }
    if (WORLD->current_level == HOTHESCAPEB_LDATA && object.id == id_SNOWMOB && DoBuckStart(&object)) {
        return true;
    }
    if (object.field_0xe31 == 1 && IsDownSwipe(holder.swipe_samples[index].position, holder.touch_position)) {
        StartJetPackFall(&object, 0);
        return false;
    }
    if (object.character_context == LEGOCONTEXT_JUMP) {
        if (object.touch_task == NULL || object.touch_task->GetHashId().value != MechTouchTaskJump::HashId.value) {
            return false;
        }
        if ((TouchHacks::CanLunge(object) || TouchHacks::CanSlam(object)) &&
            IsDownSwipe(holder.swipe_samples[index].position, holder.touch_position) &&
            !TouchHacks::CheckForAboutToRunIntoKillTerrain(object, 0.1f)) {
            const i32 style = object.jump_sequence > 1 ? 2 : 1;
            MechSystems::Get()->NewSwipeMarker(holder, index, static_cast<SwipeDecalRenderer::Style>(style));
            object.field_0xf04 |= 0x20;
            return false;
        }
        MechSystems::Get()->NewSwipeMarker(holder, index, static_cast<SwipeDecalRenderer::Style>(0));
        object.field_0xf04 |= 0x10;
        return false;
    }

    MechSystems::Get()->NewSwipeMarker(holder, index, static_cast<SwipeDecalRenderer::Style>(0));
    JumpTriggerPacket packet;
    packet.player = &object;
    packet.type = 3;
    packet.touch_holder = &holder;
    packet.start = holder.swipe_samples[index].position;
    packet.velocity = VuVec_Zero;
    packet.end = holder.touch_position;
    isBucking = (reinterpret_cast<u8 *>(object.apiobj.character_data->player_config)[0x96] & 2) != 0;
    TriggerJumpTask(packet, true, false, true);
    return true;
}

bool MechInputTouchGestureBasedController::OnDoubleClick(GameObject_s &object, TouchHolder &holder) {
    if (VehicleArea != 0)
        return OnClick(object, holder);
    if (object.character_context == LEGOCONTEXT_JUMP || object.character_context == LEGOCONTEXT_BIGJUMP ||
        object.apiobj.field_0x27d == 0)
        return true;
    if (object.apiobj.character_data == NULL || object.apiobj.character_data->player_config == NULL ||
        object.id == id_GRABCONTROL || object.character_context == 0x2b)
        return false;

    VuVec touch_position(holder.down_position.x, holder.down_position.y, 0.0f, 1.0f);
    MechObjectInterface *target = holder.target_object.Get();
    if (target == NULL)
        target = holder.previous_target_object.Get();
    if (target == NULL)
        target = MechInputTouchSystem::FindTargetObject(object, touch_position, 0x80, NULL, &temporary_position);
    if (target == NULL)
        return false;

    VuVec target_position;
    target->GetPos(target_position, -1);
    target_position.x -= object.apiobj.position.x;
    target_position.y -= object.apiobj.position.y;
    target_position.z -= object.apiobj.position.z;
    const f32 distance_sq = target_position.x * target_position.x + target_position.y * target_position.y +
                            target_position.z * target_position.z;
    bool attack_jump = false;
    if (target->GetObjectType() == 2) {
        GameObject_s *character = target->GetCharacterObject();
        if (character == &object) {
            GameObject_s *nearby = FindNearestGameObject(&object.apiobj.position, &object, 0, 1.5f, 1.5f, -1, -1, 100,
                                                         NULL, 0, NULL, false);
            if (nearby == NULL) {
                JumpTriggerPacket packet = {};
                packet.type = 2;
                packet.player = &object;
                packet.touch_holder = &holder;
                packet.velocity =
                    VuVec(object.apiobj.velocity.x, object.apiobj.velocity.y, object.apiobj.velocity.z, 1.0f);
                VuVec position(object.apiobj.position.x, object.apiobj.position.y, object.apiobj.position.z, 1.0f);
                memcpy(packet.field_1c, &position, sizeof(position));
                Hint_SetComplete(0x629);
                object.field_0xf04 |= 0x10;
                TriggerJumpTask(packet, false, false, false);
            } else {
                StartNewTask(new MechTouchTaskAttack(*this, nearby->GetMechObjectInterface(), touch_position), holder,
                             false, true);
            }
            return true;
        }
        if (character == NULL || !(distance_sq < 1.4f * 1.4f) || static_cast<i32>(character->apiobj.field_0x1f4) < 0)
            return OnClick(object, holder);
        if (character->apiobj.character_data != NULL &&
            ((character->apiobj.character_data->model_flags & 0x10) != 0 ||
             ((character->apiobj.character_data->model_flags & 8) != 0 && qrand() <= 0x3a97)))
            attack_jump = true;
    } else if (target->GetObjectType() == 4) {
        if (!(distance_sq < 1.4f * 1.4f))
            return OnClick(object, holder);
    } else {
        GameObject_s *nearby =
            FindNearestGameObject(&object.apiobj.position, &object, 0, 0.5f, 0.5f, -1, -1, 100, NULL, 0, NULL, false);
        if (nearby != NULL) {
            StartNewTask(new MechTouchTaskAttack(*this, nearby->GetMechObjectInterface(), touch_position), holder,
                         false, true);
            return OnClick(object, holder);
        }
        StartNewTask(new MechTouchTaskPlannedDoubleClickGoTo(*this, target), holder, false, true);
        return true;
    }
    if ((object.apiobj.character_data->model_flags & 8) == 0)
        return OnClick(object, holder);

    if (attack_jump)
        object.field_0xf04 |= 0x10;
    object.field_0xf04 |= 0x20;
    target->GetPos(target_position, -1);
    GameObject_s *backup_recipient = player != NULL ? player : &object;
    NUVEC previous_velocity = backup_recipient->apiobj.velocity;
    NUVEC previous_target_velocity = backup_recipient->target_velocity;
    VuVec velocity = TouchHacks::CalculateJumpVelToHitPoint(object, target_position);
    object.apiobj.velocity.x = object.target_velocity.x = velocity.x;
    object.apiobj.velocity.y = object.target_velocity.y = velocity.y;
    object.apiobj.velocity.z = object.target_velocity.z = velocity.z;
    JumpTriggerPacket packet = {};
    packet.type = 2;
    packet.player = &object;
    packet.touch_holder = &holder;
    packet.velocity = VuVec(object.apiobj.velocity.x, object.apiobj.velocity.y, object.apiobj.velocity.z, 1.0f);
    memcpy(packet.field_1c, &target_position, sizeof(target_position));
    ForceNextLungeTarget(target);
    SetForcedAttackOpponent(target);
    if (!TriggerJumpTask(packet, true, true, true)) {
        GameObject_s *restore_recipient = player != NULL ? player : &object;
        restore_recipient->apiobj.velocity = previous_velocity;
        restore_recipient->target_velocity = previous_target_velocity;
    }
    return true;
}

void MechInputTouchGestureBasedController::ProcessAutoJumpOverGap(GameObject_s *object) {
    LEVELDATA *level = WORLD->current_level;
    if (level != RESCUEE_LDATA && level != GUNGAN_A_LDATA) {
        if (level == MOSEISLEYA_LDATA && (object->apiobj.character_data->model_flags & 0x40) == 0) {
            return;
        }
        TouchHolder *holder = field_90;
        if (holder == NULL && object->touch_task != NULL) {
            holder = object->touch_task->touch_holder;
        }
        if (holder == NULL) {
            return;
        }
        JumpTriggerPacket packet;
        packet.player = object;

        packet.touch_holder = holder;
        packet.type = 1;
        packet.velocity = VuVec(object->apiobj.velocity.x, object->apiobj.velocity.y, object->apiobj.velocity.z, 1.0f);
        if (object->apiobj.field_0x27d == 0 && !ObjLandReady(object)) {
            return;
        }
        if (object->character_context == LEGOCONTEXT_JUMP || object->character_context == LEGOCONTEXT_LAND_JUMP ||
            (object->touch_task != NULL && !object->touch_task->IsGoToTask()) || (object->field_0xf04 & 0x40) != 0) {
            return;
        }
        bool danger = TouchHacks::CheckForAboutToRunIntoKillTerrain(*object, 0.3f);
        if (!danger && !TouchHacks::CheckForAboutToRunOffAnEdge(*object, 0.3f)) {
            return;
        }
        if ((object->apiobj.character_data->model_flags & 0x40) == 0 && object->id != id_WATTO &&
            MechAutoJumpGetBest(packet, object->apiobj.facing_angle) == NULL &&
            !TouchHacks::CheckJumpForLandingSpot(*object, danger ? 2.0f : 0.05f)) {
            return;
        }
        f32 speed =
            *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(object->apiobj.character_data->player_config) + 0x1c);
        VuVec boosted_velocity __attribute__((aligned(16)))(object->apiobj.velocity.x, object->apiobj.velocity.y,
                                                            object->apiobj.velocity.z, 1.0f);
        f32 magnitude_sq = boosted_velocity.x * boosted_velocity.x + boosted_velocity.y * boosted_velocity.y +
                           boosted_velocity.z * boosted_velocity.z;
        if (magnitude_sq < speed * speed) {
            NuVecNorm(&boosted_velocity.xyz, &boosted_velocity.xyz);
            speed =
                *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(object->apiobj.character_data->player_config) + 0x1c);
            boosted_velocity.y *= speed;
            boosted_velocity.x *= speed;
            boosted_velocity.z *= speed;
        }
        NUVEC previous_velocity = player->apiobj.velocity;
        NUVEC previous_target_velocity = player->target_velocity;
        object->apiobj.velocity = boosted_velocity.xyz;
        object->target_velocity = boosted_velocity.xyz;
        if (!TriggerJumpTask(packet, false, true, true)) {
            player->apiobj.velocity = previous_velocity;
            player->target_velocity = previous_target_velocity;

        }
    }
}

void MechInputTouchGestureBasedController::ProcessAutoJumpWhenStuck(GameObject_s &object) {
    TouchHolder *holder = field_90;
    if (holder == NULL && object.touch_task != NULL) {
        holder = object.touch_task->touch_holder;
    }
    if (holder == NULL) {
        return;
    }
    if (object.character_context != -1 || object.pad_gamepad == NULL ||
        (object.apiobj.character_data->model_flags & 0x2000) != 0) {
        field_a6 = 0;
        field_98 = 0.0f;
        return;
    }
    NUVEC movement = {object.apiobj.velocity.x, 0.0f, object.apiobj.velocity.z};
    const f32 movement_sq = NuVecMagSqr(&movement);
    const f32 intended_speed = object.pad_gamepad->input_magnitude * 0.5f;
    if (!(movement_sq < intended_speed * intended_speed)) {
        field_98 = 0.0f;
        field_a6 = 0;
        return;
    }
    field_98 += FRAMETIME;
    if (!(field_98 > 0.2f) || field_a6 != 0) {
        return;
    }
    field_a6 = 1;
    NUVEC probe;
    probe.x = object.apiobj.position.x;
    probe.y = object.apiobj.position.y;
    probe.z = object.apiobj.position.z;
    VuVec intended(object.target_velocity.x, object.target_velocity.y, object.target_velocity.z, 1.0f);
    NuVecNorm(&intended.xyz, &intended.xyz);
    probe.x += intended.x * 0.5f;
    probe.y += 1.0f;
    probe.z += intended.z * 0.5f;
    if (!(GameShadow(player, &probe, 2.0f, -1) > player->apiobj.position.y + 0.1f)) {
        return;
    }
    NUVEC previous_velocity = player->apiobj.velocity;
    f32 speed = *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(player->apiobj.character_data->player_config) + 0x18);
    NUVEC velocity = {intended.x * speed, intended.y * speed, intended.z * speed};
    NUVEC previous_target_velocity = player->target_velocity;
    player->apiobj.velocity = velocity;
    player->target_velocity = velocity;
    JumpTriggerPacket packet;
    packet.type = 1;
    packet.player = &object;
    packet.touch_holder = holder;
    packet.velocity = VuVec(velocity.x, velocity.y, velocity.z, 1.0f);
    if (!TriggerJumpTask(packet, false, true, true)) {
        player->apiobj.velocity = previous_velocity;
        player->target_velocity = previous_target_velocity;
    }
}

void MechInputTouchGestureBasedController::Update(NuInputTouchData const *data) {
    if (player != NULL && NewMode == 0 && NewLData == NULL && FadeSys.fade == 0.0f && Paused == 0 && CUTSTOPGAME == 0 &&
        !MenuDisable() && TouchHacks::TouchControlsActive && GetMenuID() == -1 && MiniCutCam != 2 &&
        !Controller_IsConnected()) {
        Activate();
    } else {
        Deactivate();
    }
    *reinterpret_cast<StickValuesVec4 *>(stick_values) = StickValuesVec4{0.0f, 0.0f, 0.0f, 0.0f};
    if (!active || data == NULL) {
        return;
    }
    GameObject_s *object = Player[player_id];
    if (object != NULL) {
        if (object->apiobj.field_0x27d != 0) {
            isBucking = false;
        }
        if (object->touch_task != NULL) {
            for (MechTouchTask *task = object->touch_task->next; task != NULL; task = task->next) {
                task->BackgroundProcess();
            }
            if (object->touch_task->character != player || !object->touch_task->Update()) {
                if (object->touch_task != NULL) {
                    TouchHolder *holder = object->touch_task->touch_holder;
                    if ((object->touch_task->flags & 1) != 0) {
                        field_a0 = 0.75f;
                    }
                    MechTouchTask *next = object->touch_task->next;
                    object->touch_task->OnStop();
                    delete object->touch_task;
                    object->touch_task = next;
                    if (next != NULL) {
                        next->OnResume();
                    }
                    if (object->touch_task == NULL && holder->is_down && field_90 == NULL) {
                        field_90 = holder;
                        field_a5 = 0;
                    }
                }
            }
        }
        ProcessDragMovement(*object);
        if (VehicleArea == 0 && (object->apiobj.character_data->model_flags & 0x2000) == 0) {
            ProcessAutoJumpOverGap(object);
            ProcessAutoJumpWhenStuck(*object);
        }
        if (smart_bomb_touch != NULL) {
            MechObjectInterface *touched_object = smart_bomb_touch->target_object.Get();
            VuVec position(smart_bomb_touch->down_position.x, smart_bomb_touch->down_position.y, 0.0f, 1.0f);
            if (touched_object != MechInputTouchSystem::FindTargetObject(*object, position, 1, touched_object, NULL)) {
                smart_bomb_touch = NULL;
            } else {
                if (smart_bomb_touch->held_time >= 1.0f) {
                    TouchHacks::TriggerVehicleSmartBomb(*player);
                    smart_bomb_touch = NULL;
                } else {
                    TouchHacks::PlaySmartBombBuildupEffects(*player, smart_bomb_touch->held_time, 1.0f);
                }
            }
        }
        if (tag_button.Get() != NULL && tag_button.Get()->on_click != NULL) {
            tag_button = NuMechPtr<MechTouchUIElement, 4>();
        }
    }
    UpdateButtons();
}

bool MechInputTouchGestureBasedController::PerformCloseMechanic(GameObject_s &object, TouchHolder &holder) {
    if ((object.field_0xcc0 != NULL && object.field_0xcc0->id != id_YODA) || VehicleArea != 0) {
        return false;
    }

    VuVec position(holder.down_position.x, holder.down_position.y, 0.0f, 1.0f);
    if (TouchHacks::CanUseTeleport(object)) {
        VuVec teleport_position;
        if (Teleport_Find(&object, 0.05f * 0.05f, &teleport_position) != NULL) {
            f32 dx = teleport_position.x - object.apiobj.position.x;
            f32 dy = teleport_position.y - object.apiobj.position.y;
            f32 dz = teleport_position.z - object.apiobj.position.z;
            if (__builtin_fabsf(dy) < object.apiobj.scaled_height && dx * dx + dz * dz < 0.35f * 0.35f) {
                StartNewTask(new MechTouchTaskUseTeleport(*this, NULL, position), holder, true, true);
                return true;
            }
        }
    }

    if (TouchHacks::CanUseZipup(object) &&
        ZipUp_FindNearest(WORLD, &object.apiobj.position, object.field_0x1008, NULL, NULL, &object, true) != NULL) {
        StartNewTask(new MechTouchTaskUseZipUp(*this), holder, true, true);
        return true;
    }

    f32 distance = 1000000000.0f;
    if (TouchHacks::CanUseHatMachine(object)) {
        HATMACHINE_s *machine = HatMachine_FindNearest(WORLD, &object.apiobj.position, &object, &distance);
        if (machine != NULL) {
            f32 range = object.field_0x1008 + 0.2f;
            if (distance <= range * range) {
                StartNewTask(new MechTouchTaskHatMachine(*this, machine->GetMechObjectInterface(), position), holder,
                             true, true);
                return true;
            }
        }
    }

    distance = 1000000000.0f;
    if (TouchHacks::CanUseLever(object)) {
        LEVER_s *lever = Lever_FindNearest(WORLD, &object.apiobj.position, &object, &distance);
        if (lever != NULL) {
            f32 range = object.field_0x1008 + 0.2f;
            if (distance <= range * range) {
                StartNewTask(new MechTouchTaskPullLever(*this, lever->GetMechObjectInterface(), position), holder, true,
                             true);
                return true;
            }
        }
    }

    distance = 1000000000.0f;
    GIZPANEL_s *panel = GizPanel_FindNearest(WORLD, &object.apiobj.position, &object, &distance, 0);
    if (panel != NULL && GizPanel_CanUsePanel(&object, panel)) {
        f32 range = object.field_0x1008 + 0.2f;
        if (distance <= range * range) {
            StartNewTask(new MechTouchTaskPanel(*this, panel->GetMechObjectInterface(), position), holder, true, true);
            return true;
        }
    }

    f32 range = object.field_0x1008 * 4.0f;
    GameObject_s *candidate =
        FindNearestGameObject(&object.apiobj.position, &object, 0, range, range, -1, -1, 100, NULL, 0, NULL, false);
    if (candidate != NULL && static_cast<i32>(candidate->apiobj.field_0x1f4) >= 0) {
        StartNewTask(new MechTouchTaskAttack(*this, candidate->GetMechObjectInterface(), position), holder, false,
                     true);
        return true;
    }
    return false;
}

bool MechInputTouchGestureBasedController::OnClick(GameObject_s &object, TouchHolder &holder) {
    if (object.apiobj.character_data == NULL || object.apiobj.character_data->player_config == NULL ||
        object.id == id_GRABCONTROL || object.character_context == 0x2b) {
        return false;
    }
    holder.clicked = 1;
    VuVec position __attribute__((aligned(16)))(holder.down_position.x, holder.down_position.y, 0.0f, 1.0f);
    MechObjectInterface *target = holder.target_object.Get();
    if (target != NULL && (target->GetObjectType() == 5 || target->GetObjectType() == 6)) {
        i32 flags = 0x1f4f;
        if (object.field_0xcc0 != NULL && object.field_0xcc0->id != id_YODA) {
            flags = TouchHacks::CanShoot(object) ? 0x5c05 : 0;
            if (TouchHacks::CanPoo(object) || object.apiobj.character_data->move_fn == Move_BEAST) {
                flags |= 1;
            }
        }
        target = MechInputTouchSystem::FindTargetObject(object, position, flags, NULL, NULL);
    }
    holder.previous_target_object = target;
    if (target != NULL && target->GetCharacterObject() != NULL && target->GetCharacterObject()->id == id_BIGGUN) {
        return false;
    }

    bool dispatch = object.id == id_TRAININGREMOTE;
    if (!dispatch && object.character_context != LEGOCONTEXT_JUMP && object.character_context != LEGOCONTEXT_BIGJUMP) {
        dispatch = object.apiobj.field_0x27d != 0 || VehicleArea != 0 ||
                   (object.apiobj.character_data->model_flags & 0x2000) != 0 || object.field_0xe31 == 1;
    }
    if (!dispatch) {
        if ((object.apiobj.character_data->model_flags & 0x8000) != 0 && object.id != id_WATTO &&
            object.field_0xe31 != 1) {
            object.field_0xf04 |= 0x10;
            return true;
        }
        if (target != NULL && (target->GetObjectType() == 4 || target->GetObjectType() == 2)) {
            if (TouchHacks::CanShoot(object)) {
                ForceNextShootTarget(*target);
                button_was_pressed[0] = 1;
                return true;
            }
            ForceNextLungeTarget(target);
            object.field_0xf04 |= 0x20;
            return true;
        }
        ForceNextLungeTarget(NULL);
        object.field_0xf04 |= 0x20;
        return true;
    }

    if (target == NULL) {
        target = MechInputTouchSystem::FindTargetObject(object, position, 0x80, NULL, &temporary_position);
        if (VehicleArea != 0) {
            if (target != NULL && object.character_context == -1) {
                VuVec target_position;
                target->GetPos(target_position, -1);
                target_position.x -= object.apiobj.position.x;
                target_position.y -= object.apiobj.position.y;
                target_position.z -= object.apiobj.position.z;
                object.apiobj.movement_facing_angle = NuAtan2D(target_position.x, target_position.z);
            }
            button_was_pressed[0] = 1;
            target = NULL;
        }
    }
    holder.previous_target_object = target;
    holder.target_object = target;
    if (target == NULL) {
        return false;
    }
    if (VehicleArea != 0)
        button_was_pressed[0] = 1;
    target->TargetedFlash();
    switch (target->GetObjectType()) {
        case 1:
        case 5:
            StartNewTask(new MechTouchTaskPlannedGoTo(*this, target, NULL), holder, false, true);
            return true;
        case 2: {
            GameObject_s *character = target->GetCharacterObject();
            if (character == player) {
                if (object.touch_task != NULL &&
                    object.touch_task->GetHashId().value == MechTouchTaskAttack::HashId.value) {
                    return false;
                }
                if (FireBountyHunterRocket(player)) {
                    return false;
                }
                if (TouchHacks::CanPoo(object)) {
                    button_was_pressed[3] = 1;
                    return false;
                }
                if (PerformCloseMechanic(object, holder)) {
                    return false;
                }
                if (object.character_context != LEGOCONTEXT_WEAPONOUT && !(object.weapon_scale > 0.0f)) {
                    SlowWeaponOut(&object);
                }
                return true;
            }
            if (target->GetCharacterObject() == NULL) {
                return false;
            }
            if (static_cast<i32>(target->GetCharacterObject()->apiobj.field_0x1f4) < 0) {
                VuVec target_position;
                target->GetPos(target_position, -1);
                NUVEC direction = {player->apiobj.position.x - target_position.x, 0.0f,
                                   player->apiobj.position.z - target_position.z};
                NuVecNorm(&direction, &direction);
                f32 scale = target->GetRadius() * 2.5f;
                temporary_position.position =
                    VuVec(target_position.x + direction.x * scale, target_position.y + direction.y * scale,
                          target_position.z + direction.z * scale, 0.0f);
                MechTouchTaskPlannedGoTo *task = new MechTouchTaskPlannedGoTo(*this, &temporary_position, NULL);
                task->field_6fd = 1;
                task->field_6fe = 1;
                StartNewTask(task, holder, false, true);
                return true;
            }
            if (static_cast<i32>(target->GetCharacterObject()->apiobj.field_0x1f4) < 0)
                return false;
            StartNewTask(new MechTouchTaskAttack(*this, target, position), holder, false, true);
            return true;
        }
        case 3:
        case 4:
        case 11: {
            if (target->GetGizObstacle() != NULL && target->GetGizObstacle()->mode == 2) {
                StartNewTask(new MechTouchTaskPlannedGoTo(*this, target, NULL), holder, false, true);
            } else {
                StartNewTask(new MechTouchTaskAttack(*this, target, position), holder, false, true);
            }
            return true;
        }
        case 7:
            StartNewTask(new MechTouchTaskPullLever(*this, target, position), holder, false, true);
            return true;
        case 8:
            StartNewTask(new MechTouchTaskUseTeleport(*this, target, position), holder, false, true);
            return true;
        case 9:
            StartNewTask(new MechTouchTaskHatMachine(*this, target, position), holder, false, true);
            return true;
        case 10:
            StartNewTask(new MechTouchTaskPanel(*this, target, position), holder, false, true);
            return true;
        default:
            return false;
    }
}

