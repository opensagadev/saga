// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/actions/combat/fighting.h"
#include "legoapi/actions/combat/hits.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/players.h"
#include "nu2api/nu3d/nutex.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/characters/core/playeritems.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/actions/character/speederchase.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/misc/utilities.h"

#include <stdlib.h>
#include "legoapi/actions/movement/jumping.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/hatmachine.h"
#include "legoapi/actions/character/snake.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/traps/attracto.h"
#include "legoapi/props/objects/signal.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/gizmos/transport/teleport.h"
#include "legoapi/gizmos/door/zipups.h"
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
#include "legoapi/world/area.h"
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
#include "legoapi/items/base/apiobject.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numusic/sfx.h"
#include <float.h>
#include <string.h>
#include "legoapi/characters/motion/gameanim_types.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/ai/aisys/aipath.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/ai/core/legoai.h"
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
#include "legoapi/audio/audio.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/menus/screens/arcade.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/gizmos/door/spinner.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/core/input/timing.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "legoapi/items/objects/gameobjects_types.h"
#include "legoapi/items/base/collection.h"
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
#include "legoapi/gizmos/object/lever.h"
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
#include "MechInputTouch/MechInputTouchGestureBasedController.h"
#include "gameapi/ai/aisys/gameaiscript.h"
#include "legoapi/actions/movement/climbing.h"
#include "legoapi/characters/motion/move.h"
#include "legoapi/world/levels/episodeV.h"


void (*Punch_HitHoldFn)(GameObject_s *, GameObject_s *);
i32 (*Punch_GetDamageFn)(GameObject_s *, GameObject_s *);
void (*Punch_HitExtraCodeFn)(GameObject_s *, nuvec_s *);
BLADE_s BladeTab[4] = {
    {101, 223, 1, 59, 64, {255, 31, 0}, {0, 0, 0}},
    {103, 221, 2, 60, 65, {30, 191, 15}, {0, 0, 0}},
    {105, 225, 3, 61, 66, {0, 191, 255}, {0, 0, 0}},
    {107, 227, 4, 62, 67, {200, 0, 255}, {0, 0, 0}},
};


DECOMP_ASSERT(offsetof(GameObject_s, field_0x7e4) == 0x7e4, "Quick shoot record offset");
DECOMP_ASSERT(offsetof(GameObject_s, field_0x7e8) == 0x7e8, "Quick shoot record count offset");
DECOMP_ASSERT(offsetof(GameObject_s, quick_shoot_flags) == 0xe2f, "Quick shoot flags offset");
DECOMP_ASSERT(offsetof(GameObject_s, quick_shoot_bolt_id) == 0xe3e, "Quick shoot bolt ID offset");

// ==== src/legoapi/characters/motion/move.cpp: declarations ====



// The retail move.cpp initializer owns these four managed references.

// Original action/context lookup data, with its arrays and pointers in one owner.


i32 (*CanStartHoldFn)(GameObject_s *) = NULL;
// Retail keeps several of this file's static helpers only as GCC clones; these
// declarations pin the clone names and register conventions.
#if defined(__i386__)
#define MOVE_CLONE_ATTR1 __attribute__((noinline, noclone, regparm(1)))
#define MOVE_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define MOVE_CLONE_ATTR1 __attribute__((noinline, noclone))
#define MOVE_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif

// Retail keeps only the IPA-SRA clone of this function.

// Multiplies the ordinary camera's per-frame position blend.  The original
// keeps this as writable camera state (default 1.0), rather than folding it
// into the socket seek rate.



// Retail splits the guard off as an inline head; the rest is this partial clone.

extern i32 LevFlag[4];
// Original exported level control, initialized to 2 at 0x623d00.

// These pod tuning values are zero-initialized in the original image.


// Retail re-tests the three block animations at each use instead of caching a flag.
#define HAS_BLOCK_ANIMS(animations)                                                                                    \
    ((animations)[0x1a] != NULL || (animations)[0x1b] != NULL || (animations)[0x1c] != NULL)

// Retail inlines the fade and step checks into each caller; this is the remaining partial clone.

// Retail splits the context test off as an inline head; the rest is this partial clone.

DECOMP_ASSERT(sizeof(AWKWARDSHAPESPHERE_s) == 12, "Awkward shape sphere size");
DECOMP_ASSERT(offsetof(AWKWARDSHAPESPHERE_s, radius) == 8, "Awkward shape sphere radius offset");

// ==== src/legoapi/characters/motion/gameanim.cpp: declarations ====


// Retail uses translation-unit-local helpers with compiler-inferred private
// calling conventions; keep their shared animation-mode call sites intact.


// ==== src/legoapi/world/levels/episodeV.cpp: declarations ====



DECOMP_ASSERT(sizeof(HOTHBATTLEE_NETPACKET_s) == 0x58, "Hoth panel packet size");
DECOMP_ASSERT(offsetof(HOTHBATTLEE_NETPACKET_s, alpha) == 0x24, "Hoth panel packet alpha offset");
DECOMP_ASSERT(offsetof(HOTHBATTLEE_NETPACKET_s, count) == 0x54, "Hoth panel packet count offset");

// ===========================================================================
// Asteroid chase (AsteroidChase_A / B / C / D)
// ===========================================================================

DECOMP_ASSERT(sizeof(ASTEROID_s) == 0x18, "ASTEROID_s size");

DECOMP_ASSERT(sizeof(FINALASTEROID_s) == 0x3c, "final asteroid size");
DECOMP_ASSERT(offsetof(FINALASTEROID_s, blowup_count) == 0x2c, "final asteroid blowup count offset");
DECOMP_ASSERT(offsetof(FINALASTEROID_s, rotation_x) == 0x34, "final asteroid rotation offset");

DECOMP_ASSERT(sizeof(ASTEROIDCNETPACKET_s) == 8, "asteroid C network packet size");

// ==== src/legoapi/render/fx/parts.cpp: declarations ====



extern "C" {
    // Shared suspend flag consulted by all timed debris emitters.


}

// Forward declarations for local (static) part/gizmo helper stubs.

// Local (static) part (PART_s), pickup (GIZMOPICKUP_s), gizmo flow
// (GIZFLOW_s/FLOWBOX_s), power-up and space/starfighter helpers. Stubbed as
// local `t` symbols matching res/libTTapp.so.

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




// ==== src/legoapi/items/objects/gameobjects.cpp: declarations ====

// Written by ThingManager's ctor (original global @0x124f2e0, .bss).


// Enabled in the target data image.  Ordinary background characters are
// staggered across frames; special movement states opt back into full-rate
// processing through the forced-update path.

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// The special-route list reserves the first ten ids for suit characters.  The
// remaining ids enumerate the current story list while omitting variants that
// are represented by those dedicated routes.

#if defined(__i386__)
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone, regparm(2), sseregparm))
#else
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone))
#endif

// Retail keeps only the IPA-SRA clone, which takes the other object's x and z by address.



// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

// ==== src/legoapi/actions/combat/fighting.cpp: declarations ====


// ==== src/legoapi/characters/core/players.cpp: declarations ====


// --- Helpers moved from world.cpp ---

// ---- Player start spawn entries ----

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----


namespace {

} // namespace


DECOMP_ASSERT(sizeof(SPECIAL_LAYER_s) == 0xc, "SPECIAL_LAYER_s size");

// ==== src/legoapi/actions/movement/pushing.cpp: declarations ====

// ==== src/legoapi/characters/motion/camera.cpp: declarations ====

// Original data symbol testmovef is float0.005 at 0x617cb0.

DECOMP_ASSERT(sizeof(VIEWCAM_s) == 0x28, "ViewCam size");
DECOMP_ASSERT(offsetof(VIEWCAM_s, pitch) == 0x10, "ViewCam pitch offset");
DECOMP_ASSERT(offsetof(VIEWCAM_s, gamepad) == 0x24, "ViewCam gamepad offset");


DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, path) == 0x40, "Camera context path offset");
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, flags) == 0x4c, "Camera context flags offset");

// ==== src/legoapi/audio/specialsfx.cpp: declarations ====


// ==== src/legoapi/actions/character/speederchase.cpp: declarations ====

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.


DECOMP_ASSERT(sizeof(SPEEDERCHASEVALUES) == 0x1c, "Speeder chase tuning stride");
DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");


// ==== src/legoapi/gizmo/base/gizmo.cpp: declarations ====


struct FLOWBOX_s;

DECOMP_ASSERT(sizeof(GIZMOTYPES) == 0xc, "Gizmo type-table header ABI");
DECOMP_ASSERT(offsetof(GIZMOTYPES, types) == 8, "Gizmo type-table pointer offset");

// ==== src/legoapi/misc/legoapi_misc.cpp: declarations ====

// ==== src/legoapi/world/levels/episodeII.cpp: declarations ====


static_assert(sizeof(struct GUNSHIP_LEVFLAG_s) == 16, "LevFlag must be 16 bytes");

// --- Cross-file entry points / id globals ---
//

// Defined in gameapi/edtools/edtoolsall.cpp, which has no header yet.

struct JEDIBNETPACKET_s;

// Jedi_B tuning and spawn tables (unmangled globals in the original).

// --- File-local statics (original _ZL... symbols; not renamed) ---------------

DECOMP_ASSERT(sizeof(KAMINODISCO_s) == 1000, "Kamino disco original layout");

DECOMP_ASSERT(sizeof(DOOKU_C_s) == 20, "Dooku C state ABI");

DECOMP_ASSERT(sizeof(KAMINO_E_s) == 120, "Kamino E state ABI");

// Episode 2 level handlers, in the game's Episode_II progression:
// kamino / factory (geonosis droid factory) / jedi / gunship / bonus gunship /
// dooku, then the NewTown bonus. The coruscant bounty-hunter pursuit lives in
// pursuit.cpp, its own translation unit in the original binary.

// ===========================================================================
// Kamino
// ===========================================================================

// ===========================================================================
// Geonosis — droid factory (Factory_B / Factory_G)
// ===========================================================================

// ===========================================================================
// Jedi (Jedi_B)
// ===========================================================================

DECOMP_ASSERT(sizeof(JEDIB_CREATURE_s) == 0x60, "JEDIB_CREATURE_s ABI");

DECOMP_ASSERT(sizeof(JEDIB_s) == 0x63ec, "JEDIB_s ABI");

DECOMP_ASSERT(sizeof(JEDIBNETPACKET_s) == 0x20, "JEDIBNETPACKET_s ABI");

// Pushes `position` out of any antinode it falls inside, then resolves it to a
// path point and fills in `locator` for it. Returns 0 when the point is off the
// arena or off the path network.
// JediBInitLocator is inlined into JediB_Init apart from everything after
// the network check, which retail keeps as the split-out body of the IPA-SRA
// clone (it only needs &world->ai_sys).


// ===========================================================================
// Gunship (Gunship_A / Gunship_B)
// ===========================================================================

// ===========================================================================
// Bonus gunship (Bonus_Gunship_A / Bonus_Gunship_B)
// ===========================================================================

// ===========================================================================
// Dooku (Dooku_C)
// ===========================================================================

// ==== src/legoapi/world/levels/episodeI.cpp: declarations ====


static_assert(sizeof(struct LEVFLAGBYTES_s) == 16, "LevFlag must be 16 bytes");

// Episode 1 level handlers: negotiations / gungan / rescue / podrace /
// podsprint / retake / maul / anakins-flight. This is the only implemented
// level file; episodes II-VI are still stubs.
//
// Functions appear in the original translation unit's address order (see
// res/libTTapp.so 0x1fa8e0..0x203d50). PodLevel and SetPodMergeAnims live at
// the end because the original binary defines them in other units.

// --- Cross-file entry points (C linkage) -----------------------------------
//
// Declared here rather than in a shared header because episodeI.cpp is their
// only consumer within the levels module.

// --- Cross-file entry points (C++ linkage) ---------------------------------

// PodSprintA_Update boulder-seek state.

// --- File-local layout types -----------------------------------------------

// Every caller passes mode 0, so retail keeps only this constprop clone.

// --- File-local state --------------------------------------------------------
//
// Names are the original statics (_ZL... symbols in res/libTTapp.so) and must
// not be renamed. Initial values match the original .data image.

// Mushroom-collapse cutscene state (initial values from the .data image;
// mushroom_time_* are also mutated by Action_MushroomCollapse).

// Retake-G guard counters (net-shared through retakeg_netpacket).

// Maul boss levels.

// Gungan plains wildlife spawner (0x114 bytes, layout verified against
// _ZL8gungan_a).
// ===========================================================================
// Sniper turrets (shared by PodRace C and PodSprint A)
// ===========================================================================

// PodSprint_InitAISpline is partially inlined into PodSprintA_Init: the
// clear, lookup and spline checks are inlined and the crossing scan is the
// split-out body.
#if defined(__i386__)
#define PODSPRINT_AISPLINE_BODY_ATTR __attribute__((noinline, regparm(1)))
#else
#define PODSPRINT_AISPLINE_BODY_ATTR __attribute__((noinline))
#endif

// UpdatePacemakerDisplay is inlined into its callers apart from the message
// update, which retail keeps as the split-out body of the IPA-SRA clone (it
// only needs &world->lev_objs).

// ===========================================================================
// Negotiations / Gungan plains / Rescue (coruscant)
// ===========================================================================

// ===========================================================================
// Pod race core (A/B/C laps)
// ===========================================================================


// ===========================================================================
// Pod sprint (Sebulba events)
// ===========================================================================



// ===========================================================================
// Retake the palace (D/E/G)
// ===========================================================================

// ===========================================================================
// Maul boss (A/B/D/E/F)
// ===========================================================================

// ===========================================================================
// Anakin's flight (B)
// ===========================================================================

// ===========================================================================
// Helpers defined in other original translation units but kept here until
// they get their own home (their originals live outside the ep1 TU).
// ===========================================================================

// ==== src/legoapi/ai/core/gameai.cpp: declarations ====


// ==== src/legoapi/gizmos/trigger/ai.cpp: all remaining content ====

// ==== src/legoapi/ai/game/creature.cpp: declarations ====



// ==== src/legoapi/ai/game/aipathcnxhelper.cpp: declarations ====


// ==== src/legoapi/ai/core/ai_sys.cpp: declarations ====

struct nuhspecial_s;
struct nuvec_s;

// ==== src/legoapi/actions/character/simpletons.cpp: declarations ====


// ---- begin src/legoapi/ai/core/aipathbuild.cpp ----

// The route records use a dense route index, while the editor keeps sixteen
// independently switchable slots.  The original helper first marks both ends
// of every route connection, then builds the compact node lookup and matrix.
#if defined(__i386__)
#define AIPATHBUILD_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define AIPATHBUILD_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif
// Retail keeps only the IPA-SRA clone, without the unused path index.

// ---- end src/legoapi/ai/core/aipathbuild.cpp ----

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

// IPA-SRA keeps only the fields this helper reads: values for those read before any call,
// addresses for those read after one.


// IPA-SRA hands this clone the address of system->path_sys rather than the system.

// ---- begin src/gameapi/ai/aisys/aisys.cpp ----





// Game-specific AI actions and conditions (registered via
// RegisterAIScriptActions / RegisterAIScriptConditions). These are stubbed to
// satisfy the symbol baseline; the action/condition logic itself is not
// decompiled. Each stub matches the mangled symbol of the original binary.

extern AIACTIONDEF api_aiactiondefs[55];

extern AICONDITIONDEF api_aiconditiondefs[50];

DECOMP_ASSERT(sizeof(api_aiactiondefs) == 0x294, "API action registry size");

DECOMP_ASSERT(sizeof(api_aiconditiondefs) == 0x258, "API condition registry size");

// ---- end src/gameapi/ai/aisys/aisys.cpp ----

// ==== src/gameapi/ai/aisys/aiscript.cpp: all remaining content ====


#if defined(__i386__)
#define AISCRIPT_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define AISCRIPT_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif

// Retail inlines the "goto" test into each caller; this is the remaining partial clone.

// Retail inlines the system test into its callers; this is the remaining partial clone.




// ==== src/legoapi/ai/core/ai_sys_stubs.cpp: declarations ====

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

// Retail keeps only the IPA-SRA clone, without the unused object and elapsed time.

extern "C" {


} // extern "C"

// ==== src/editor/area_editor.cpp: all remaining content ====

struct nupad_s;

DECOMP_ASSERT(sizeof(EDAIAREA_s) == 0x44, "area editor record size");

// Item colours shared by the AI editors.

// ==== src/editor/edpath.cpp: all remaining content ====

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

// ==== src/editor/aieditor.cpp: all remaining content ====


#if defined(__i386__)
#define EDANTINODE_REGPARM1 __attribute__((regparm(1)))
#else
#define EDANTINODE_REGPARM1
#endif

// ==== src/editor/creature_editor.cpp: all remaining content ====

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

// ==== src/editor/locator_editor.cpp: all remaining content ====

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

// ==== src/legoapi/items/objects/gameobjects.cpp: declarations ====

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// ==== src/gameapi/ai/aisys/aistate.cpp: all remaining content ====

// ==== src/legoapi/gizmo/gizmos/gizmos_gizactions.cpp: declarations ====




namespace {
#include "legoapi/gizmo/gizmos/gizmos_gizactions_types.h"

} // namespace

// ==== src/legoapi/ai/game/gameantinode.cpp: declarations ====

// ==== src/legoapi/render/core/render.cpp: declarations ====


// Camera zoom state

// Graphics loading flags

// --- NuGScn graphics-data reader ---
// NuReadGraphicsData is a C++ static function in the original (GCC clones it,
// hence the `.isra.NNN` suffix in the ROM symbol table).

// Retail only keeps the IPA-SRA clone, which drops the unused fourth argument.
#if defined(__i386__)
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone))
#endif

// --- Extern "C": NuGScn functions have C linkage in original ---


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

// DrawPanel HUD globals referenced by the cantina-bar-patrons WIP. Initial
// red-brick positions come from the Game init values in legogame/game.cpp.

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


// ==== src/legoapi/actions/character/starwars.cpp: all remaining content ====

struct AIPACKET_s;
struct AISYS_s;

// ==== src/editor/antinode_editor.cpp: all remaining content ====

// ==== src/editor/edtoolsall.cpp: declarations ====


// ==== src/legoapi/render/core/terrain_stubs.cpp: declarations ====


// Runtime-selected groups appended after the fixed terrain allocation.

// NuFileExists @nufile (nucore_plain.cpp).

// ==== functions in original address order ====

// ==== functions in original address order ====

// ==== src/legoapi/items/base/collection.cpp: declarations ====

struct GIZMOPICKUP_s;
struct PART_s;

// ==== src/legoapi/actions/character/snake.cpp: declarations ====

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

// ==== src/legoapi/actions/movement/jumping.cpp: declarations ====


// ==== src/legoapi/actions/movement/carrying.cpp: declarations ====

// Original defaults: 0x668d30 and 0x668d40, each a four-byte integer.

// ==== src/legoapi/menus/core/hint.cpp: declarations ====

// The hint icon transitions and the touch icon x position are dynamically
// initialized in this unit's static constructor in the reference.

// Retail keeps this body as GCC's split-out clone; the inline head stays in
// the caller.

// Original local callbacks referenced by Hints_LSW.

// Each control mode retains its own requested and interpolated hint row.

// ==== src/legoapi/misc/utilities.cpp: declarations ====



// Retail copies a fully inside end point as one unaligned vector.

// ==== src/legoapi/gizmo/object/gizforce.cpp: all remaining content ====

// ==== src/legoapi/render/core/render.cpp: declarations ====

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

// ==== src/legoapi/world/levels/episode.cpp: declarations ====

DECOMP_ASSERT(sizeof(TROOPERCANNON_s) == 0x30, "Trooper cannon state size");
DECOMP_ASSERT(offsetof(TROOPERCANNON_s, character_name) == 0x0c, "Trooper cannon name offset");
DECOMP_ASSERT(offsetof(TROOPERCANNON_s, rebuilding) == 0x2c, "Trooper cannon rebuilding offset");

// Episode-global logic and helpers shared across episodes: completion/
// episode bookkeeping, super-story, boss/cutscene-adjacent helpers. The
// per-episode level *handlers* live in the matching episodeI..VI.cpp files.

// ===========================================================================
// Episode bookkeeping
// ===========================================================================

// ===========================================================================
// HUD / score helpers
// ===========================================================================

// ===========================================================================
// Shared gameplay helpers
// ===========================================================================

// ==== src/legoapi/items/collect/batarang.cpp: declarations ====


// ==== src/legoapi/gizmo/object/gizmoblowups.cpp: declarations ====

// Original writable flagremaptab at 0x668520, 32 pairs of 32-bit masks.
extern "C" {
extern u32 flagremaptab[32][2];
}

// Definitions from the original gizmoblowups translation unit.


DECOMP_ASSERT(sizeof(BLOWUPPROGRESS_s) == 0x100, "BLOWUPPROGRESS_s ABI");

DECOMP_ASSERT(offsetof(WORLDINFO_s, blowup_target_candidates) == 0x50d4, "Blowup target candidate-list ABI");


// ==== src/legoapi/characters/core/characters.cpp: declarations ====


// LoadPerm1 is one of the few game-level entry points which wires together
// otherwise C-linkage engine subsystems.  Keep these declarations local: the
// individual subsystem TUs intentionally expose their original plain names.
extern "C" {

}

// The original table at 0x006281a0.  These are the material animation
// scripts made available before the permanent things scene is loaded.


// Retail keeps only the IPA-SRA clone, which receives &*layers.
#if defined(__i386__)
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone, regparm(1)))
#else
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone))
#endif

// ==== src/legoapi/props/objects/tubes.cpp: all remaining content ====


// ==== src/legoapi/misc/supportall.cpp: declarations ====

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

// The reference defines these with dynamic initialization in this unit.

DECOMP_ASSERT(sizeof(TIMINGBAR_LABEL) == 12, "Timing-bar label size");

// Debug-capture output helpers consumed by NuDisplayListCaptureSortPriority.
// Transcribed from the original C-linkage symbols:
//   NuHtmlBegin    0x2d5ca0   NuHtmlFlush   0x2d5c30
//   NuHtmlWrite    0x2d5cd0   NuHtmlHeading1 0x2d5d40



// ==== src/legoapi/items/objects/gameobjects.cpp: declarations ====

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// ==== src/legoapi/misc/memory.cpp: all remaining content ====

// ==== functions in original address order ====

// ==== functions in original address order ====

// ==== functions in original address order ====

void ComboHitFrame(GameObject_s *object, i32 damage) {
    object->context_flags |= 0x40;
    object->sabre_flags |= 5;
    object->sabre_damage = damage;
    if ((object->apiobj.flags_low & 0x80) && Player_HasDoubleWeaponDamage(object)) {
        object->sabre_damage <<= 1;
    }
    BlockSfx(object);
}

void StartHold(GameObject_s *object) {
    if (CanStartHoldFn == NULL)
        return;
    if (CanStartHoldFn(object) == 2) {
        object->hold_timer = 0.0f;
        object->character_context = -1;
        return;
    }
    if (LEGOCONTEXT_HOLD != -1 && NewBlockAction(object) != 0) {
        object->context_animation_timer = 0.3f;
        object->character_context = LEGOCONTEXT_HOLD;
        PlaySabreSfx(NULL, object, NULL, 1);
        return;
    }
    object->character_context = -1;
}

void HoldCode(GameObject_s *object) {
    if (LEGOCONTEXT_HOLD != -1 && object->character_context == LEGOCONTEXT_HOLD) {
        if (object->context_animation_timer > 0.0f)
            object->context_animation_timer -= FRAMETIME;
        else if ((object->pad_gamepad->buttons_held & GAMEPAD_ACTION) == 0)
            object->character_context = -1;
    }
}

void HoldCode_Copy(GameObject_s *object) {
    if (LEGOCONTEXT_HOLD != -1 && object->character_context == LEGOCONTEXT_HOLD) {
        if (object->context_animation_timer > 0.0f)
            object->context_animation_timer -= FRAMETIME;
        else if ((object->pad_gamepad->buttons_held & GAMEPAD_ACTION) == 0)
            object->character_context = -1;
    }
}

void Punch_Hit(GameObject_s *attacker, GameObject_s *target, float, float gap) {
    NUVEC *hit_position;
    attacker->context_flags |= 0x40;

    if (LEGOCONTEXT_PUNCH != -1 && attacker->character_context == LEGOCONTEXT_PUNCH) {
        CHARACTERANIM_s *animation =
            static_cast<CHARACTERANIM_s *>(attacker->apiobj.character_model->model_data_a[attacker->context_animation]);
        if (animation != NULL) {
            attacker->attack_locator = animation->locator;
        }
    }

    if (target != NULL) {
        if ((target->apiobj.field_0x1f8 & 0x1001) != 0x1001 || target->apiobj.field_0x287 != 0 ||
            target->apiobj.collision_min.y > attacker->apiobj.collision_max.y ||
            attacker->apiobj.collision_min.y > target->apiobj.collision_max.y) {
            goto finish;
        }

        if (LEGOCONTEXT_HOLD != -1 && target->character_context == LEGOCONTEXT_HOLD) {
            NewBlockAction(target);
            hit_position = &target->apiobj.collision_position;
            if (Punch_HitHoldFn != NULL) {
                Punch_HitHoldFn(attacker, target);
            }
            goto hit;
        }

        if (target->apiobj.player_controlled) {
            if (LEGOCONTEXT_PUNCH != -1 && target->character_context == LEGOCONTEXT_PUNCH) {
                goto finish;
            }
            if (LEGOCONTEXT_JUMP != -1 && target->character_context == LEGOCONTEXT_JUMP) {
                if (attacker->action_movement_state == 3) {
                    goto finish;
                }
            }
        }

        if (ObjOpponentStillThere(attacker, target, gap) == 0) {
            goto finish;
        }

        i32 damage = Punch_GetDamageFn != NULL ? Punch_GetDamageFn(attacker, target) : 1;
        if (damage != -1 && Cheats_CheckFlags(0x800) != 0) {
            damage *= 2;
        }

        hit_position = NULL;
        u32 context_flags = CInfo[target->character_context].flags;
        if ((context_flags & 0x80) == 0) {
            if ((context_flags & 0x04000000) != 0 ||
                ((context_flags & 0x08000000) != 0 && (target->jump_flags & 2) != 0) || CannotKill(target) != 0) {
                NewRumble(attacker->pad_gamepad->pad, 0.5f, 0);
                NewRumble(target->pad_gamepad->pad, 0.5f, 0);
                if (static_cast<i32>(CInfo[target->character_context].flags) >= 0) {
                    target->apiobj.movement_facing_angle =
                        NuAtan2D(attacker->apiobj.collision_position.x - target->apiobj.collision_position.x,
                                 attacker->apiobj.collision_position.z - target->apiobj.collision_position.z);
                }
            } else {
                ObjHitObj(attacker, target, damage, static_cast<u16>(ObjHitObj_Flags(attacker) | 0x40), 0, 1);
            }
            hit_position = &target->apiobj.collision_position;
        }

        if ((target->apiobj.field_0x1f8 & 2) == 0) {
            f32 impulse = target->apiobj.character_data->game_character->movement_speed;
            if (attacker->apiobj.scaled_radius > 1.0f) {
                impulse /= attacker->apiobj.scaled_radius;
            }
            u16 angle = attacker->apiobj.movement_facing_angle;
            if (static_cast<i8>(attacker->context_flags) < 0) {
                angle -= 0x8000;
            }
            target->apiobj.velocity.z += NuTrigTable[((static_cast<u32>(angle) + 0x4000) >> 1) & 0x7fff] * impulse;
            target->apiobj.velocity.x += NuTrigTable[angle >> 1] * impulse;
        }
    } else {
        GIZMOBLOWUP_s *blowup = attacker->blowup_target;
        if (blowup == NULL || (blowup->status_flags & 0x804001) != 0x804000 ||
            blowup->bounds_min.y > attacker->apiobj.collision_max.y ||
            attacker->apiobj.collision_min.y > blowup->bounds_max.y) {
            goto finish;
        }
        hit_position = &blowup->mid_position;
        if (GizmoBlowupBlowup(blowup, 1, 1, 1, NULL, 1) != 0) {
            NewBuzz(attacker->pad_gamepad->pad, 0.1f, 0);
        }
        NewRumble(attacker->pad_gamepad->pad, 0.5f, 0);
    }

    if (hit_position == NULL) {
        goto finish;
    }

hit:
    attacker->movement_runtime_flags |= 1;
    if (attacker->apiobj.player_controlled) {
        GameCam_HitJudder();
    }
    goto extra_code;

finish:
    hit_position = NULL;
    attacker->movement_runtime_flags &= ~1;

extra_code:
    if (Punch_HitExtraCodeFn != NULL) {
        Punch_HitExtraCodeFn(attacker, hit_position);
    }
}

