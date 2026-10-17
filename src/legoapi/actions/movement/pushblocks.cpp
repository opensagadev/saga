// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "legoapi/actions/movement/pushblocks.h"

#include <math.h>
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/numath.h"
#include "decomp.h"
#include "nu2api/nucore/nuanim3.h"
#include "globals.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/world/level.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/actions/movement/pushblocks_types.h"
#include "legoapi/gizmos/door/push.h"
#include "gameapi/edtools/edfile.h"
#include "legoapi/world/world.h"
#include "legoapi/render/light/surfaces.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nufloat.h"
#include <string.h>
#include "legoapi/world/area.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/items/base/animpacket.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/actions/movement/jumping.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "nu2api/nu3d/nutex.h"
#include <stdlib.h>
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/hatmachine.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/actions/character/snake.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/core/config/cheat.h"
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
#include "legoapi/render/core/rtl.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/world/mission.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/numtx_inline.h"
#include "legoapi/characters/motion/move_types.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numusic/sfx.h"
#include <float.h>
#include "legoapi/characters/motion/gameanim_types.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/ai/aisys/aipath.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/ai/core/legoai.h"
#include "legoapi/ai/game/creature.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/gizmos/traps/gizbombgen.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/menus/core/panel.h"
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
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/characters/core/customiser.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nufile/nufile.h"
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
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/gizmos/door/spinner.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/core/input/timing.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "legoapi/items/objects/gameobjects_types.h"
#include "legoapi/actions/combat/fighting.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/characters/core/playeritems.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/world/levels/episode.h"
#include "legogame/game.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/characters/core/players_types.h"
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
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "gameapi/edtools/edui_colour.h"
#include "gameapi/edtools/edgra_internal.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include "nu2api/nucore/nuvideo.h"
#include <cstdio>
#include "gameapi/edtools/edtools_types.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include <new>
#include "gameapi/gui/apimenu_internal.h"
#include "legoapi/core/config/fileselect.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nucore/nuonline.h"
#include <GLES2/gl2.h>
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "java/android.h"
#include "java/java.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/NuInputManager.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nuscreen.hpp"
#include "nu2api/nuplatform/nudevicespecs.hpp"

void AlertSurroundingCreatures(GameObject_s *, NUVEC *);

pushblock_s *BlockInBlock(WORLDINFO_s *, pushblock_s *, i32, pushblock_s **);

u32 (*CanPushBlocksFn)(GameObject_s *) = NULL;
i32 pushposincrease;
i32 runoutofpostabspace;

void SetAnimFrame(nuhspecial_s *, f32);
void ResetPushProgress(WORLDINFO_s *, void *);
void ResetSinglePushBlockHeight(WORLDINFO_s *, pushblock_s *, i32);
i32 TerrainBlockOnBlock(WORLDINFO_s *, pushblock_s *, NUVEC *, f32 *);

pushblock_s *NearestPushBlock(WORLDINFO_s *world, nuvec_s *position, float range);

void PushSeekComplete(pushblock_s *block, i32 index);

i32 OtherBlockInRange(WORLDINFO_s *world, pushblock_s *block, nuvec_s *position, i32 excluded);

void ResetSinglePushBlock(WORLDINFO_s *, pushblock_s *block, i32);

pushblock_s *NearestFacingPushBlock(WORLDINFO_s *world, GameObject_s *object, float range);

void ResetSinglePushBlockHeight(WORLDINFO_s *world, pushblock_s *block, i32 index);

i32 GizPushBlock_EndFrameCompleted(pushblock_s *push_block, i32 output_index);

i32 PushBlock(GameObject_s *object);

DECOMP_ASSERT(sizeof(PUSHPROGRESS) == 0x24c, "push-block progress size");

enum PUSHBLOCK_CONFIG_FLAGS {
    PUSHBLOCK_FLAG_VISIBLE = 1 << 2,
    PUSHBLOCK_FLAG_STATE = 1 << 1,
};

i32 pushblock_gizmotype_id;

void ResetPushProgress(WORLDINFO_s *world, void *progress);
void ResetSinglePushBlock(WORLDINFO_s *world, pushblock_s *push_block, i32 index);
void MoveBlocks(WORLDINFO_s *world, pushblock_s *push_block, i32 index, NUVEC *velocity);
void PushSeekComplete(pushblock_s *push_block, i32 snap_index);

extern i32 LEGOHINT_PUSHBLOCKS;
static NUVEC hothbtestpos = {29.0f, 0.0f, 13.0f};
i32 SnapPosTaken(WORLDINFO_s *, pushblock_s *, NUVEC *, i32);
i32 OtherBlockInRange(WORLDINFO_s *, pushblock_s *, NUVEC *, i32);
void AddShoveObject(nuhspecial_s *, i16);
void UpdatePushBlocks(void *world_ptr, void *, float);

i32 GizPush_GetNumOutputs(GIZMO *gizmo);

static char outputName[13] = "Any Complete";

char *GizPush_GetOutputName(GIZMO *gizmo, i32 output_index);

i32 GizPush_GetOutput(GIZMO *gizmo, i32 output_index, i32);

void ResetPushProgress(WORLDINFO_s *world, void *progress_data);

void AlertSurroundingCreatures(GameObject_s *object, NUVEC *position);

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
i32 PodLevel(AREADATA_s *);
extern "C" i16 id_DODGEM;

i32 PodLevel(AREADATA_s *area);

// Retail splits the guard off as an inline head; the rest is this partial clone.

extern i32 LevFlag[4];
// Original exported level control, initialized to 2 at 0x623d00.

extern "C" i16 id_LANDSPEEDER;

// These pod tuning values are zero-initialized in the original image.

extern "C" void PlaySfxById(i32 sfx_id, nuvec_s *position);

// Retail re-tests the three block animations at each use instead of caching a flag.
#define HAS_BLOCK_ANIMS(animations)                                                                                    \
    ((animations)[0x1a] != NULL || (animations)[0x1b] != NULL || (animations)[0x1c] != NULL)

extern "C" {
    extern i16 id_MINIATST, id_MINIATTE;
}

DECOMP_ASSERT(sizeof(AWKWARDSHAPESPHERE_s) == 12, "Awkward shape sphere size");
DECOMP_ASSERT(offsetof(AWKWARDSHAPESPHERE_s, radius) == 8, "Awkward shape sphere radius offset");

extern i16 id_MINIATST, id_MINIATTE;

// Retail uses translation-unit-local helpers with compiler-inferred private
// calling conventions; keep their shared animation-mode call sites intact.

extern AILOCATOR_s * locator;

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

    void PlaySfxById(i32, NUVEC *);

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

DECOMP_ASSERT(offsetof(GameObject_s, field_0x7e4) == 0x7e4, "Quick shoot record offset");
DECOMP_ASSERT(offsetof(GameObject_s, field_0x7e8) == 0x7e8, "Quick shoot record count offset");
DECOMP_ASSERT(offsetof(GameObject_s, quick_shoot_flags) == 0xe2f, "Quick shoot flags offset");
DECOMP_ASSERT(offsetof(GameObject_s, quick_shoot_bolt_id) == 0xe3e, "Quick shoot bolt ID offset");

struct HINT_s;

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

namespace {

} // namespace

DECOMP_ASSERT(sizeof(SPECIAL_LAYER_s) == 0xc, "SPECIAL_LAYER_s size");

// Original data symbol testmovef is float0.005 at 0x617cb0.

DECOMP_ASSERT(sizeof(VIEWCAM_s) == 0x28, "ViewCam size");
DECOMP_ASSERT(offsetof(VIEWCAM_s, pitch) == 0x10, "ViewCam pitch offset");
DECOMP_ASSERT(offsetof(VIEWCAM_s, gamepad) == 0x24, "ViewCam gamepad offset");

extern "C" i16 id_LANDSPEEDER;

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

extern "C" {
    extern i16 id_LANDSPEEDER;
}

void AlertSurroundingCreatures(GameObject_s *object, nuvec_s *position);

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
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

i32 Action_ReleaseLocator(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count,
                          i32 first_time, float);

i32 Action_GetLocatorFromSet(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                             i32 param_count, i32 first_time, float);

i32 Action_AssignLocatorInSet(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count,
                              i32 first_time, float);

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
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

pushblock_s *BlockInBlock(WORLDINFO_s *world, pushblock_s *block, i32 excluded, pushblock_s **support);

extern "C" {
    void PlaySfxById(i32, NUVEC *);

}

#define PART_EMIT_SOUND_ON(slot)                                                                                       \
    if (type->sounds[slot] != -1) {                                                                                    \
        SetSfxBit_On(type->sounds[slot]);                                                                              \
        if (type->sound_modes[slot] == 4)                                                                              \
            PlaySfxById(type->sounds[slot], &emitter->position);                                                       \
    }
#define PART_EMIT_SOUND(slot, mode)                                                                                    \

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
extern "C" void PlaySfxById(i32, nuvec_s *);

DECOMP_ASSERT(sizeof(EdManipulator) == 0x6c, "EdManipulator ABI");

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

    DECOMP_ASSERT(sizeof(NuCameraReflect) == 0x84, "camera reflection state size");
    DECOMP_ASSERT(__builtin_offsetof(NuCameraReflect, mtx) == 4, "camera reflection matrix offset");

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

#include "nu2api/nucore/nucore_plain_types.h"

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    DECOMP_ASSERT(sizeof(NuInstanceTreeNode) == 0x0c, "instance tree node stride");
    DECOMP_ASSERT(sizeof(NuInstanceTreeBounds) == 0x20, "instance tree bounds stride");
    DECOMP_ASSERT(sizeof(NuInstanceTree_s) == 0x28, "instance tree size");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, nodes) == 0x14, "instance tree nodes offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, roots) == 0x20, "instance tree roots offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, far_clips) == 0x24, "instance tree far clips offset");

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

void MoveBlocksOverBlock(WORLDINFO_s *, pushblock_s *, i32, nuvec_s *);
void MoveBlocks(WORLDINFO_s *world, pushblock_s *block, i32 index, nuvec_s *velocity);

void MoveBlocksOverBlock(WORLDINFO_s *world, pushblock_s *block, i32 excluded, nuvec_s *velocity);

#define HAS_BLOCK_ANIMS(animations)                                                                                    \
    ((animations)[0x1a] != NULL || (animations)[0x1b] != NULL || (animations)[0x1c] != NULL)

extern "C" {
    extern i16 id_MINIATST, id_MINIATTE;
}

i32 SnapPosTaken(WORLDINFO_s *world, pushblock_s *, nuvec_s *position, i32 excluded_index);

extern "C" {
    void PlaySfxById(i32, NUVEC *);

}

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

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

namespace {

} // namespace

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

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

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

extern "C" {
extern u32 flagremaptab[32][2];
}

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

struct pushblock_s;

namespace {
#include "legoapi/render/core/terrain_types.h"
    DECOMP_ASSERT(sizeof(TERRAIN_SCENE_OBJECT) == 0x50, "TERRAIN_SCENE_OBJECT ABI");

    DECOMP_ASSERT(sizeof(TERRAIN_DISPLAY_OBJECT) == 0xd0, "TERRAIN_DISPLAY_OBJECT ABI");
    DECOMP_ASSERT(offsetof(TERRAIN_DISPLAY_OBJECT, transform) == 0xb8, "TERRAIN_DISPLAY_OBJECT transform offset");
    DECOMP_ASSERT(offsetof(TERRAIN_DISPLAY_OBJECT, scene_object_index) == 0xc0,
                  "TERRAIN_DISPLAY_OBJECT scene_object_index offset");

    DECOMP_ASSERT(offsetof(TERRAIN_DISPLAY_SCENE, object_count) == 0x6c, "TERRAIN_DISPLAY_SCENE object_count offset");

    DECOMP_ASSERT(offsetof(TERRAIN_GSCENE, object_count) == 0x1c, "TERRAIN_GSCENE object_count offset");
    DECOMP_ASSERT(offsetof(TERRAIN_GSCENE, object_map_present) == 0x58, "TERRAIN_GSCENE object_map_present offset");
    DECOMP_ASSERT(offsetof(TERRAIN_GSCENE, display_scene) == 0x110, "TERRAIN_GSCENE display_scene offset");

} // namespace

namespace {

    // Retail scan callers contain the scale-and-normal reconstruction directly.
#define TERRAIN_SCALE_SCAN_SHAPE(candidate_arg, group_arg, writer_arg)                                                 \
    do {                                                                                                               \
        TERRAIN_SHAPE *&scale_candidate = (candidate_arg);                                                             \
        const TERRAIN_GROUP &scale_group = (group_arg);                                                                \
        TerrainScanWriter *scale_writer = (writer_arg);                                                                \
        if (TerI->object_scale != 1.0f) {                                                                              \
            TERRAIN_SHAPE *source = scale_candidate;                                                                   \
            scale_candidate = &ScaleTerrain[scale_writer->scaled_shape_count];                                         \
            scale_candidate->material[0] = source->material[0];                                                        \
            scale_candidate->material[1] = source->material[1];                                                        \
            scale_candidate->flags = source->flags;                                                                    \
            scale_candidate->normal_flags = source->normal_flags;                                                      \
                                                                                                                       \
            for (i32 vector_index = 0; vector_index < 3; ++vector_index) {                                             \
                scale_candidate->vectors[vector_index].x = source->vectors[vector_index].x;                            \
                scale_candidate->vectors[vector_index].z = source->vectors[vector_index].z;                            \
                scale_candidate->vectors[vector_index].y =                                                             \
                    (source->vectors[vector_index].y + scale_group.origin.y) * TerI->inverse_object_scale -            \
                    scale_group.origin.y;                                                                              \
            }                                                                                                          \
                                                                                                                       \
            if (source->normals[1].y < 65535.0f) {                                                                     \
                scale_candidate->vectors[3].x = source->vectors[3].x;                                                  \
                scale_candidate->vectors[3].z = source->vectors[3].z;                                                  \
                scale_candidate->vectors[3].y =                                                                        \
                    (source->vectors[3].y + scale_group.origin.y) * TerI->inverse_object_scale - scale_group.origin.y; \
                                                                                                                       \
                const f32 length = NuFsqrt(source->normals[1].x * source->normals[1].x +                               \
                                           source->normals[1].y * source->normals[1].y * TerI->object_scale_sq +       \
                                           source->normals[1].z * source->normals[1].z);                               \
                const f32 inverse_length = length == 0.0f ? 0.0f : 1.0f / length;                                      \
                scale_candidate->normals[1].x = source->normals[1].x * inverse_length;                                 \
                scale_candidate->normals[1].y = source->normals[1].y * TerI->object_scale * inverse_length;            \
                scale_candidate->normals[1].z = source->normals[1].z * inverse_length;                                 \
            } else {                                                                                                   \
                scale_candidate->normals[1].y = 65536.0f;                                                              \
            }                                                                                                          \
                                                                                                                       \
            const f32 length = NuFsqrt(source->normals[0].x * source->normals[0].x +                                   \
                                       source->normals[0].y * source->normals[0].y * TerI->object_scale_sq +           \
                                       source->normals[0].z * source->normals[0].z);                                   \
            const f32 inverse_length = length == 0.0f ? 0.0f : 1.0f / length;                                          \
            scale_candidate->normals[0].x = source->normals[0].x * inverse_length;                                     \
            scale_candidate->normals[0].y = source->normals[0].y * TerI->object_scale * inverse_length;                \
            scale_candidate->normals[0].z = source->normals[0].z * inverse_length;                                     \
            ++scale_writer->scaled_shape_count;                                                                        \
        }                                                                                                              \
    } while (0)

    // Preserve the retail arithmetic ordering and the sweep-diagonal service
    // call, even in the wall-only caller where its result is unused.
#define TERRAIN_GET_SCAN_BOUNDS(bounds_arg, radius_arg, wall_arg)                                                      \
    do {                                                                                                               \
        TerrainScanBounds &scan_bounds = (bounds_arg);                                                                 \
        f32 &bounds_radius = (radius_arg);                                                                             \
        const TerrainQuery_s &scan_query = *TerI;                                                                      \
        if (scan_query.scan_result != 1) {                                                                             \
            const f32 move_length = NuFsqrt(scan_query.movement.x * scan_query.movement.x +                            \
                                            scan_query.movement.y * scan_query.movement.y +                            \
                                            scan_query.movement.z * scan_query.movement.z);                            \
            const TerrainQuery_s &sphere_query = *TerI;                                                                \
            const f32 reach = (wall_arg) ? 0.1f + sphere_query.collision_radius + move_length                          \
                                         : move_length + 0.1f + sphere_query.collision_radius;                         \
            const f32 vertical = reach * sphere_query.object_scale;                                                    \
            scan_bounds.max_x = sphere_query.position.x + 0.05f + reach;                                               \
            scan_bounds.max_z = sphere_query.position.z + 0.05f + reach;                                               \
            scan_bounds.min_x = sphere_query.position.x - 0.05f - reach;                                               \
            scan_bounds.min_z = sphere_query.position.z - 0.05f - reach;                                               \
            scan_bounds.min_y = sphere_query.position.y - 0.05f - vertical;                                            \
            scan_bounds.max_y = sphere_query.position.y + 0.05f + vertical;                                            \
            bounds_radius = reach;                                                                                     \
        } else {                                                                                                       \
            const f32 radius = scan_query.collision_radius;                                                            \
            if (!(scan_query.movement.x > 0.0f)) {                                                                     \
                scan_bounds.min_x = scan_query.position.x + scan_query.movement.x - 0.02f - radius;                    \
                scan_bounds.max_x = scan_query.position.x + 0.02f + radius;                                            \
            } else {                                                                                                   \
                scan_bounds.min_x = scan_query.position.x - 0.02f - radius;                                            \
                scan_bounds.max_x = scan_query.position.x + scan_query.movement.x + 0.02f + radius;                    \
            }                                                                                                          \
            if (!(scan_query.movement.y > 0.0f)) {                                                                     \
                scan_bounds.min_y = scan_query.position.y + scan_query.movement.y - 0.02f - radius;                    \
                scan_bounds.max_y = scan_query.position.y + 0.02f + radius;                                            \
            } else {                                                                                                   \
                scan_bounds.min_y = scan_query.position.y - 0.02f - radius;                                            \
                scan_bounds.max_y = scan_query.position.y + scan_query.movement.y + 0.02f + radius;                    \
            }                                                                                                          \
            if (!(scan_query.movement.z > 0.0f)) {                                                                     \
                scan_bounds.min_z = scan_query.position.z + scan_query.movement.z - 0.02f - radius;                    \
                scan_bounds.max_z = scan_query.position.z + 0.02f + radius;                                            \
            } else {                                                                                                   \
                scan_bounds.min_z = scan_query.position.z - 0.02f - radius;                                            \
                scan_bounds.max_z = scan_query.position.z + scan_query.movement.z + 0.02f + radius;                    \
            }                                                                                                          \
            const f32 dx = scan_bounds.min_x - scan_bounds.max_x;                                                      \
            const f32 dy = scan_bounds.min_y - scan_bounds.max_y;                                                      \
            const f32 dz = scan_bounds.min_z - scan_bounds.max_z;                                                      \
            bounds_radius = NuFsqrt(dx * dx + dy * dy + dz * dz);                                                      \
        }                                                                                                              \
        if (!(wall_arg) && scan_query.object_scale > 1.0f)                                                             \
            bounds_radius += (scan_query.object_scale - 1.0f) * scan_query.collision_radius;                           \
    } while (0)

    // These wall-spline traversals are part of all three retail scan bodies.
#define TERRAIN_COLLECT_WALL_SPLINES(bounds_arg, mask_arg, clear_arg, reset_empty_arg)                                 \
    do {                                                                                                               \
        TerrainScanBounds wall_bounds = (bounds_arg);                                                                  \
        const i32 wall_mask = (mask_arg);                                                                              \
        const bool wall_clear_override = (clear_arg);                                                                  \
        wall_bounds.min_x -= 0.02f;                                                                                    \
        wall_bounds.min_z -= 0.02f;                                                                                    \
        wall_bounds.max_x += 0.02f;                                                                                    \
        wall_bounds.max_z += 0.02f;                                                                                    \
        if ((reset_empty_arg) || (CurTerr->spatial_nodes != NULL && WallSplCount < 64))                                \
            WallSplCount = 0;                                                                                          \
        for (TERRAIN_SPATIAL_NODE *node = CurTerr->spatial_nodes; node != NULL;                                        \
             node = *reinterpret_cast<TERRAIN_SPATIAL_NODE **>(reinterpret_cast<u8 *>(node) - sizeof(void *))) {       \
            const u8 mask = node->field_0x02 >> 8;                                                                     \
            if (mask != 0 && (mask & wall_mask) == 0)                                                                  \
                continue;                                                                                              \
            for (i32 first = 0; first < node->point_count; first += 16) {                                              \
                NUVEC *points = node->points + first;                                                                  \
                i32 end = node->point_count;                                                                           \
                if (points[0].y != 2147483648.0f) {                                                                    \
                    if (!(points[1].y >= wall_bounds.min_x && wall_bounds.max_x >= points[0].y &&                      \
                          points[3].y >= wall_bounds.min_z && wall_bounds.max_z >= points[2].y))                       \
                        continue;                                                                                      \
                    end = MIN(end, first + 16);                                                                        \
                }                                                                                                      \
                for (i32 i = first; i < end; ++i) {                                                                    \
                    NUVEC &a = node->points[i];                                                                        \
                    NUVEC &b = node->points[i + 1];                                                                    \
                    if (!((a.x >= wall_bounds.min_x && wall_bounds.max_x >= b.x) ||                                    \
                          (b.x >= wall_bounds.min_x && wall_bounds.max_x >= a.x)))                                     \
                        continue;                                                                                      \
                    if (!((a.z >= wall_bounds.min_z && wall_bounds.max_z >= b.z) ||                                    \
                          (b.z >= wall_bounds.min_z && wall_bounds.max_z >= a.z)))                                     \
                        continue;                                                                                      \
                    if (WallSplCount < 64) {                                                                           \
                        WallSplList[WallSplCount].position = a;                                                        \
                        WallSplList[WallSplCount + 1].position = b;                                                    \
                        for (i32 p = 0; p < 2; ++p) {                                                                  \
                            u8 *material = WallSplList[WallSplCount + p].material;                                     \
                            material[0] = static_cast<u8>(node->field_0x02);                                           \
                            material[1] = mask;                                                                        \
                            material[2] = 0;                                                                           \
                            material[3] = 0;                                                                           \
                        }                                                                                              \
                        WallSplCount += 2;                                                                             \
                    }                                                                                                  \
                }                                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
        if (wall_clear_override)                                                                                       \
            TerrOverRideScan = NULL;                                                                                   \
    } while (0)

} // namespace

// Constructs the fixed terrain header and its variable-sized group, index and
// platform arrays in the caller's arena. The four bounds arrays and the cell
// assignment array are temporary storage carved backwards from buf_end.
#define IS_SNAN(x) (*(u32 *)&(x) == 0xffc00000)

namespace {

    DECOMP_ASSERT(sizeof(TERRAIN_IMPACT_RECORD) == 0x1c, "TERRAIN_IMPACT_RECORD ABI");

} // namespace

// Retail only keeps the IPA-SRA clone, which takes &normal->y.
#if defined(__i386__)
#define TERRAIN_KILL_PLAYER_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define TERRAIN_KILL_PLAYER_ATTR __attribute__((noinline, noclone))
#endif

DECOMP_ASSERT(sizeof(TERRAIN_DEBUG_RECORD) == 100, "Terrain debug record ABI");

i32 TerrainBlockOnBlock(WORLDINFO_s *world, pushblock_s *block, nuvec_s *points, float *heights);

namespace {

    // Target NewScanRot 0x378fb0 uses 0.1f on both horizontal axes when
    // selecting the terrain shapes that can contain the cast point.

} // namespace

void NewScanRot(nuvec_s *position, i32 terrain_mask);

namespace {

} // namespace

// Original 0x378fb0: the optimized rotating scan shares the terrain engine state.

namespace {

} // namespace

namespace {

} // namespace

static void *PushBlocks_ReserveBufferSpace(void *world_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    world->push_blocks = NULL;
    world->push_block_count = 0;
    if (world->current_level->max_push_blocks != 0) {
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
        world->push_blocks = static_cast<pushblock_s *>(world->giz_buffer.void_ptr);
        world->giz_buffer.addr += world->current_level->max_push_blocks * sizeof(pushblock_s);
    }
    return world->push_blocks;
}

static i32 Push_GetMaxGizmos(void *world_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    return world != NULL ? world->current_level->max_push_blocks : 0;
}

static char *GizPush_GetGizmoName(GIZMO *gizmo) {
    if (gizmo == NULL) {
        return NULL;
    }
    return static_cast<pushblock_s *>(gizmo->object)->name;
}

i32 GizPush_GetNumOutputs(GIZMO *gizmo) {
    return static_cast<pushblock_s *>(gizmo->object)->output_count;
}

static void Push_ClearProgress(void *, void *progress_ptr) {
    PUSHPROGRESS *progress = static_cast<PUSHPROGRESS *>(progress_ptr);
    if (progress != NULL) {
        progress->state_mask[0] = 0;
        progress->visible_mask[0] = ~0u;
        progress->position_mask[0] = 0;
        NuMemSet128(progress->positions, 0, 0);
    }
}

static void *Push_AllocateProgressData(VARIPTR *buffer, VARIPTR *buffer_end) {
    return GizmoBufferAlloc(buffer, buffer_end, 0x24c);
}

static void Push_SetVisibility(GIZMO *gizmo, i32 visible) {
    pushblock_s *push_block = static_cast<pushblock_s *>(gizmo->object);
    push_block->push_visible = visible != 0;
    NuSpecialSetVisibility(&push_block->special, visible);
    for (i32 index = 0; index < push_block->end_position_count; ++index) {
        NuSpecialSetVisibility(&push_block->end_position_specials[index], push_block->push_visible);
    }
}

static void Push_AddGizmos(GIZMOSYS *gizmo_sys, i32 type_id, void *world_ptr, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    for (i32 index = 0; index < world->push_block_count; ++index) {
        if (NuStrLen(world->push_blocks[index].name) != 0) {
            AddGizmo(gizmo_sys, type_id, NULL, &world->push_blocks[index]);
        }
    }
}

static i32 edpush_Load(void *world_ptr, void *) {
    static i32 version = -1;

    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (world->push_block_count != 0) {
        return 0;
    }

    version = EdFileReadInt();
    world->push_block_count = EdFileReadInt();
    i32 valid_count = 0;
    char special_name[256];
    for (i32 file_index = 0; file_index < world->push_block_count; ++file_index) {
        pushblock_s *push_block = &world->push_blocks[valid_count];
        memset(special_name, 0, sizeof(special_name));
        EdFileRead(special_name, EdFileReadChar());

        i32 special_missing = 1;
        if (Gizmo_FindNuSpecial(world->current_gscn, &push_block->special, special_name, 1, world->gizmo_sys,
                                const_cast<char *>("PushBlock"), push_block->name) != 0) {
            NuStrCpy(push_block->name, NuSpecialGetName(&push_block->special));
            special_missing = 0;
        }

        push_block->snap_distance = EdFileReadFloat();
        push_block->flags_0cb &= ~0x04;
        if (EdFileReadChar() != 0) {
            push_block->flags_0cb |= 0x04;
        }
        push_block->flags_0cb &= ~0x08;
        if (EdFileReadChar() != 0) {
            push_block->flags_0cb |= 0x08;
        }
        push_block->flags_0ca &= ~0x10;
        if (EdFileReadChar() != 0) {
            push_block->flags_0ca |= 0x10;
        }
        push_block->flags_0ca &= ~0x20;
        if (EdFileReadChar() != 0) {
            push_block->flags_0ca |= 0x20;
        }

        if (version > 3) {
            push_block->flags_0cb &= ~0x20;
            if (EdFileReadChar() != 0) {
                push_block->flags_0cb |= 0x20;
            }
            push_block->flags_0cb &= ~0x40;
            if (EdFileReadChar() != 0) {
                push_block->flags_0cb |= 0x40;
            }
        }
        if (version > 4) {
            push_block->flags_0ca &= ~0x40;
            if (EdFileReadChar() != 0) {
                push_block->flags_0ca |= 0x40;
            }
            push_block->flags_0ca &= ~0x80;
            if (EdFileReadChar() != 0) {
                push_block->flags_0ca |= 0x80;
            }
        }

        push_block->end_position_count = 0;
        if (version > 2) {
            const u8 end_position_count = static_cast<u8>(EdFileReadChar());
            push_block->end_position_count = end_position_count;
            for (i32 end_index = 0; end_index < end_position_count; ++end_index) {
                memset(special_name, 0, sizeof(special_name));
                EdFileRead(special_name, EdFileReadChar());
                special_name[16] = '\0';
                if (Gizmo_FindNuSpecial(world->current_gscn, &push_block->end_position_specials[end_index],
                                        special_name, 1, world->gizmo_sys, const_cast<char *>("PushBlockLinkObject"),
                                        push_block->name) == 0) {
                    --push_block->end_position_count;
                }
            }
        }
        if (special_missing == 0) {
            ++valid_count;
        }
    }
    world->push_block_count = valid_count;
    return 1;
}

char *GizPush_GetOutputName(GIZMO *gizmo, i32 output_index) {

    pushblock_s *push_block = static_cast<pushblock_s *>(gizmo->object);
    if (output_index < 0 || output_index > push_block->output_count) {
        return NULL;
    }
    if (output_index == 0) {
        return const_cast<char *>("Any Complete");
    }
    NuIToA(output_index, outputName, 10);
    NuStrCat(outputName, " Complete");
    return outputName;
}

static void Push_StoreProgress(void *world_ptr, void *, void *progress_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    PUSHPROGRESS *progress = static_cast<PUSHPROGRESS *>(progress_ptr);
    if (progress == NULL) {
        return;
    }

    Push_ClearProgress(NULL, progress);
    if (world == NULL || world->push_blocks == NULL || world->push_block_count <= 0) {
        return;
    }

    pushblock_s *push_block = world->push_blocks;
    for (i32 index = 0; index < world->push_block_count; ++index, ++push_block) {
        if (index == 16) {
            break;
        }
        const i32 mask_index = index >> 5;
        const u32 mask = 1u << (index & 31);
        if ((push_block->flags_0ca & PUSHBLOCK_FLAG_VISIBLE) == 0) {
            progress->visible_mask[mask_index] &= ~mask;
        }
        if ((push_block->flags_0cb & PUSHBLOCK_FLAG_STATE) != 0) {
            progress->state_mask[mask_index] |= mask;
        }

        if (NuSpecialExistsFn(&push_block->special) != 0) {
            progress->positions[index] = *NUMTX_GET_ROW_VEC(NuSpecialGetInstanceMtx(&push_block->special), 3);
        } else {
            progress->positions[index] = v000;
        }

        for (i32 end_index = 0; end_index < push_block->end_position_count; ++end_index) {
            nuhspecial_s *special = &push_block->end_position_specials[end_index];
            if (NuSpecialExistsFn(special) != 0) {
                progress->end_positions[end_index][index] = *NUMTX_GET_ROW_VEC(NuSpecialGetInstanceMtx(special), 3);
            } else {
                progress->end_positions[end_index][index] = v000;
            }
        }
        progress->position_mask[mask_index] |= mask;
    }
}

void KnockPushBlock(pushblock_s *block, nuvec_s *direction) {
    if (block != NULL) {
        block->velocity.x = -direction->x;
        block->velocity.y = -direction->y;
        block->velocity.z = -direction->z;
        NuVecScale(&block->velocity, &block->velocity, 0.005f);
        block->runtime_flags_0c8 |= 0x80;
    }
}

static i32 Pushblocks_BoltHitPlat(void *world_ptr, void *push_blocks_ptr, BOLT *bolt, unsigned char *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    pushblock_s *push_block = static_cast<pushblock_s *>(push_blocks_ptr);
    if (push_block != NULL) {
        for (i32 index = 0; index < world->push_block_count; ++index, ++push_block) {
            if ((push_block->flags_0ca & PUSHBLOCK_FLAG_VISIBLE) == 0 || (push_block->flags_0cb & 1) == 0 ||
                (push_block->flags_0cb & PUSHBLOCK_FLAG_STATE) != 0) {
                continue;
            }
            const i32 platform = FindPlatInst(NuSpecialGetInstanceix(&push_block->special));
            if (!(platform == -1 || platform != bolt->hit_platform)) {
                if (bolt->owner != NULL) {
                    NUVEC direction;
                    NuVecSub(&direction, &bolt->owner->apiobj.collision_position, push_block->position);
                    if (direction.x == direction.z) {
                        if (direction.x < 0.0f) {
                            direction.x = -QRAND_FLOAT();
                            direction.z = -QRAND_FLOAT();
                        } else {
                            direction.x = QRAND_FLOAT();
                            direction.z = QRAND_FLOAT();
                        }
                    }
                    if (NuFabs(direction.x) > NuFabs(direction.z)) {
                        direction.z = 0.0f;
                    } else {
                        direction.x = 0.0f;
                    }
                    NuVecNorm(&direction, &direction);
                    KnockPushBlock(push_block, &direction);
                }
                return 1;
            }
        }
    }
    return 0;
}

void ResetSinglePushBlock(WORLDINFO_s *, pushblock_s *block, i32) {
    block->platform_id = FindPlatInst(NuSpecialGetInstanceix(&block->special));
    SetAnimFrame(&block->special, 1.0f);
    block->runtime_flags_0c8 &= 0xf2;
    block->runtime_flags_0c9 &= 0xf0;
    block->flags_0cb &= ~2u;
    block->completion_flags &= 0xf807;
    block->packed_state_flags &= 0xfffc7fff;
    block->pushing_object = NULL;

    if (NuSpecialExistsFn(&block->special) == 0) {
        block->bounds_min = v000;
        block->bounds_max = v000;
        block->position = &v000;
        return;
    }

    NuSpecialGetBounds(&block->special, &block->bounds_min, &block->bounds_max);
    NUMTX *matrix = NuSpecialGetInstanceMtx(&block->special);
    block->position = reinterpret_cast<NUVEC *>(&matrix->m30);
    if ((block->runtime_flags_0c9 & 0x70) != 0 && runoutofpostabspace == 0) {
        *block->position = block->snap_positions[0];
    }
}

static void Push_Activate(GIZMO *gizmo, i32 active) {
    if (gizmo != NULL) {
        pushblock_s *push_block = static_cast<pushblock_s *>(gizmo->object);
        if (active != 0) {
            push_block->flags_0cb |= 1;
            WORLDINFO *world = WORLD;
            pushblock_s *candidate = world->push_blocks;
            for (i32 index = 0; index < world->push_block_count; ++index, ++candidate) {
                if (candidate == push_block) {
                    ResetSinglePushBlock(world, push_block, index);
                    break;
                }
            }
            push_block->flags_0ca |= PUSHBLOCK_FLAG_VISIBLE;
        } else {
            push_block->flags_0cb &= ~1;
        }
    }
}

void ResetPushProgress(WORLDINFO_s *world, void *progress_data) {
    pushblock_s *block = world->push_blocks;
    if (block == NULL || world->push_block_count <= 0) {
        return;
    }

    PUSHPROGRESS *progress = static_cast<PUSHPROGRESS *>(progress_data);
    for (i32 index = 0; index < world->push_block_count; ++index, ++block) {
        if (index < 16 && progress != NULL) {
            const u32 bit = 1u << index;
            block->flags_0cb = (block->flags_0cb & ~2u) | (((progress->state_mask[0] & bit) != 0) << 1);
            block->push_visible = (progress->visible_mask[0] & bit) != 0;
            if (!block->push_visible) {
                NuSpecialSetVisibility(&block->special, 0);
                block->push_visible = 0;
                for (i32 output = 0; output < block->end_position_count; ++output) {
                    NuSpecialSetVisibility(&block->end_position_specials[output], 0);
                }
            }

            if ((progress->position_mask[0] & bit) == 0) {
                continue;
            }
            nuinstanim_s *animation = NuSpecialGetInstAnim(&block->special);
            if (block->push_visible || animation == NULL) {
                NUMTX *matrix = NuSpecialGetInstanceMtx(&block->special);
                *NUMTX_GET_ROW_VEC(matrix, 3) = progress->positions[index];
                NuSpecialUpdate(&block->special);
                if (block->end_position_count != 0) {
                    i32 output = 0;
                    do {
                        matrix = NuSpecialGetInstanceMtx(&block->end_position_specials[output]);
                        *NUMTX_GET_ROW_VEC(matrix, 3) = progress->end_positions[output][index];
                        NuSpecialUpdate(&block->end_position_specials[output]);
                    } while (block->end_position_count > ++output);
                }
            } else {
                NUMTX evaluated;
                NUMTX *matrix = NuSpecialGetInstanceMtx(&block->special);
                EvalAnim(&block->special, 1.0f, &evaluated, 0);
                *NUMTX_GET_ROW_VEC(matrix, 3) = *NUMTX_GET_ROW_VEC(&evaluated, 3);
                NuSpecialUpdate(&block->special);
                if (block->end_position_count != 0) {
                    i32 output = 0;
                    do {
                        nuhspecial_s *special = &block->end_position_specials[output];
                        matrix = NuSpecialGetInstanceMtx(special);
                        EvalAnim(special, 1.0f, &evaluated, 0);
                        *NUMTX_GET_ROW_VEC(matrix, 3) = *NUMTX_GET_ROW_VEC(&evaluated, 3);
                        NuSpecialUpdate(special);
                    } while (block->end_position_count > ++output);
                }
            }
        }
    }
}

static void Push_Reset(void *world, void *, void *progress) {
    ResetPushProgress(static_cast<WORLDINFO_s *>(world), progress);
}

pushblock_s *NearestFacingPushBlock(WORLDINFO_s *world, GameObject_s *object, float range) {
    pushblock_s *nearest = NULL;
    if (world == NULL)
        return nearest;
    pushblock_s *blocks = world->push_blocks;
    if (blocks == NULL)
        return nearest;
    if (world->push_block_count == 0)
        return nearest;
    if (LEGOCONTEXT_PUSH == -1)
        return nearest;
    if (object->character_context != LEGOCONTEXT_PUSH)
        return nearest;
    f32 upper = object->apiobj.upper_position.y;
    u16 facing = object->apiobj.movement_facing_angle;
    f32 lower = object->apiobj.lower_position.y;
    f32 adjust = (upper - lower) * 0.25f;
    f32 max_y = upper - adjust;
    f32 min_y = lower + adjust;
    i32 mode;
    if (facing == 0) {
        mode = 2;
    } else {
        u16 angle16 = facing;
        i32 angle = facing;
        if (angle > 0xfc72) {
            mode = 2;
        } else if (angle16 <= 0x38d) {
            mode = 2;
        } else if (angle16 == 0x4000) {
            mode = 0;
            goto have_mode;
        } else if (angle <= 0x3c72) {
            goto check_8000;
        } else if (angle > 0x438d) {
            goto check_8000;
        } else {
            mode = 0;
            goto have_mode;
        }
    check_8000:
        if (angle16 == 0x8000) {
            mode = 3;
            goto have_mode;
        } else if (angle <= 0x7c72) {
            goto check_c000;
        } else if (angle > 0x838d) {
            goto check_c000;
        } else {
            mode = 3;
            goto have_mode;
        }
    check_c000:
        if (angle16 == 0xc000) {
            mode = 1;
            goto have_mode;
        } else if (angle <= 0xbc72) {
            return nearest;
        } else if (angle > 0xc38d) {
            return nearest;
        } else {
            mode = 1;
        }
    have_mode:;
    }
    nearest = NULL;
    if (world->push_block_count <= 0)
        return nearest;
    f32 nearest_dist = 1000000000.0f;
    for (i32 i = 0; i < world->push_block_count; ++i, blocks = world->push_blocks) {
        pushblock_s *block = &blocks[i];
        if (NuSpecialExistsFn(&block->special) != 0) {
            if (NuSpecialGetOnScreenFn(&block->special) == 0)
                continue;
            if (NuSpecialGetVisibilityFn(&block->special) == 0)
                continue;
        }
        if ((block->packed_state_flags & 0x4000100) != 0)
            continue;
        if ((block->flags_0cb & 2) != 0)
            continue;
        NUVEC *pos = block->position;
        f32 top = pos->y + fabsf(block->bounds_max.y);
        f32 bottom = pos->y - fabsf(block->bounds_min.y);
        f32 min_z = block->bounds_min.z;
        f32 max_z = block->bounds_max.z;
        f32 min_x = block->bounds_min.x;
        if (top > max_y && bottom > max_y)
            continue;
        if (__builtin_expect(min_y > top, 0)) {
            if (min_y > bottom)
                continue;
        }
        f32 dx = pos->x - object->apiobj.collision_position.x;
        f32 dy = pos->y - object->apiobj.collision_position.y;
        f32 dz = pos->z - object->apiobj.collision_position.z;
        NUVEC delta;
        delta.x = dx;
        delta.y = dy;
        delta.z = dz;
        f32 gap;
        if (mode == 2) {
            goto mode2_geometry;
        }
        if (mode == 3) {
            goto mode3_geometry;
        }
        if (mode == 1) {
            goto mode1_geometry;
        }
    mode0_geometry: {
        f32 block_min_z = min_z + pos->z - 0.01f;
        f32 block_max_z = pos->z + max_z - 0.01f;
        if (block_min_z >= object->apiobj.collision_position.z)
            continue;
        if (object->apiobj.collision_position.z >= block_max_z)
            continue;
        if ((block->flags_0ca & 0x20) != 0)
            continue;
        if ((block->flags_0ca & 0x90) == 0x90)
            continue;
        gap = dx - fabsf(min_x);
        goto have_gap;
    }
    mode1_geometry: {
        f32 block_max_z = max_z + pos->z - 0.01f;
        f32 block_min_z = pos->z + min_z - 0.01f;
        if (object->apiobj.collision_position.z >= block_max_z)
            continue;
        if (block_min_z >= object->apiobj.collision_position.z)
            continue;
        if ((block->flags_0ca & 0x20) != 0)
            continue;
        if ((block->flags_0ca & 0x50) == 0x50)
            continue;
        gap = fabsf(dx) - block->bounds_max.x;
        goto have_gap;
    }
    mode3_geometry: {
        f32 block_min_x = min_x + pos->x - 0.01f;
        f32 block_max_x = pos->x + block->bounds_max.x - 0.01f;
        if (block_min_x >= object->apiobj.collision_position.x)
            continue;
        if (object->apiobj.collision_position.x >= block_max_x)
            continue;
        if ((block->flags_0ca & 0x10) != 0)
            continue;
        if ((block->flags_0ca & 0x60) == 0x60)
            continue;
        gap = fabsf(dz) - max_z;
        goto have_gap;
    }
    mode2_geometry: {
        f32 block_max_x = block->bounds_max.x + pos->x - 0.01f;
        f32 block_min_x = pos->x + min_x - 0.01f;
        if (object->apiobj.collision_position.x >= block_max_x)
            continue;
        if (block_min_x >= object->apiobj.collision_position.x)
            continue;
        if ((block->flags_0ca & 0x10) != 0)
            continue;
        if ((block->flags_0ca & 0xa0) == 0xa0)
            continue;
        gap = dz - fabsf(min_z);
        goto have_gap;
    }
    have_gap: {
        if (gap > (object->apiobj.field_0x1dc + 0.01f) * 2.0f)
            continue;
        i32 yaw = NuAtan2D(delta.x, delta.z);
        i32 rot = 0x4000 - (yaw & 0xffff);
        NuVecRotateY(&delta, &delta, rot);
        i32 pitch = NuAtan2D(delta.x, delta.y);
        i16 yaw_diff = (i16)(yaw - object->apiobj.movement_facing_angle);
        i32 yaw_abs = yaw_diff;
        yaw_abs = yaw_abs < 0 ? -yaw_abs : yaw_abs;
        if ((u16)yaw_abs > 0x2000)
            continue;
        i32 pitch_diff = 0x4000 - (u16)pitch;
        pitch_diff = pitch_diff < 0 ? -pitch_diff : pitch_diff;
        if ((u16)pitch_diff > 0x4000)
            continue;
        f32 dist2 = dx * dx + dz * dz;
        if (range <= dist2)
            continue;
        if (nearest_dist <= dist2)
            continue;
        nearest_dist = dist2;
        nearest = block;
    }
    }
    return nearest;
}

i32 PushBlock(GameObject_s *object) {
    pushblock_s *block = NearestFacingPushBlock(WORLD, object, 2.0f);
    if (block != NULL) {
        block->pushing_object = object;
        block->runtime_flags_0c8 |= 1;
        AlertSurroundingCreatures(object, &object->apiobj.collision_position);
        return 1;
    }
    object->character_context = -1;
    return 0;
}

pushblock_s *BlockInBlock(WORLDINFO_s *world, pushblock_s *block, i32 excluded, pushblock_s **support) {
    NUVEC *position = block->position;
    if (support)
        *support = NULL;
    for (i32 i = 0; i < world->push_block_count; ++i) {
        if (i == excluded)
            continue;
        pushblock_s *other = &world->push_blocks[i];
        if (other->flags_0cb & 2)
            continue;
        if (!NuSpecialGetVisibilityFn(&other->special))
            continue;
        f32 x = position->x - other->position->x, y = position->y - other->position->y,
            z = position->z - other->position->z;
        bool overlapx = x > (other->bounds_min.x - block->bounds_max.x) + 0.01f &&
                        x < (other->bounds_max.x - block->bounds_min.x) - 0.01f;
        f32 ymax = other->bounds_max.y - block->bounds_min.y;
        bool overlapz = z > (other->bounds_min.z - block->bounds_max.z) + 0.01f &&
                        z < (other->bounds_max.z - block->bounds_min.z) - 0.01f;
        bool overlapy = y > other->bounds_min.y - block->bounds_max.y && y < ymax;
        if (support && overlapz && overlapx && y - ymax > 0.0f) {
            if (!*support)
                *support = other;
            else if ((position->y - (*support)->position->y) - ((*support)->bounds_max.y - block->bounds_min.y) >
                     y - ymax) {
                other->supporting_block = *support;
                *support = other;
            }
        }
        if (overlapx && overlapy && overlapz)
            return other;
    }
    return NULL;
}

void ResetSinglePushBlockHeight(WORLDINFO_s *world, pushblock_s *block, i32 index) {
    BlockInBlock(world, block, index, &block->block_below);

    const f32 epsilon = 0.01f;
    f32 support_height;
    if (block->block_below != NULL) {
        pushblock_s *support = block->block_below;
        support_height = support->bounds_max.y + support->position->y + epsilon;
        block->support_height = support_height;
    } else {
        support_height = block->support_height;
    }

    NUVEC *position = block->position;
    f32 position_y = position->y;
    f32 penetration = block->bounds_min.y + position_y - support_height;
    block->vertical_penetration = penetration;
    if (penetration > epsilon) {
        position->y = position_y - penetration;
        block->vertical_penetration = 0.0f;
    }
}

__attribute__((optimize("O2", "tree-vectorize"))) void MoveBlocksOverBlock(WORLDINFO_s *world, pushblock_s *block, i32 excluded, nuvec_s *velocity) {
    if (block->runtime_flags_0c9 & 4)
        return;
    f32 xmin = block->bounds_min.x + block->position->x, xmax = block->position->x + block->bounds_max.x;
    f32 zmin = block->bounds_min.z + block->position->z, zmax = block->position->z + block->bounds_max.z;
    for (i32 i = 0; i < world->push_block_count; ++i) {
        if (i == excluded)
            continue;
        pushblock_s *other = &world->push_blocks[i];
        if ((other->flags_0cb & 4) || !(other->flags_0ca & 4))
            continue;
        NUVEC *position = other->position;
        if (block->position->y >= position->y)
            continue;
        if (!(position->x >= (xmin - other->bounds_max.x) + 0.01f &&
              position->x <= (xmax - other->bounds_min.x) - 0.01f &&
              position->z >= (zmin - other->bounds_max.z) + 0.01f &&
              position->z <= (zmax - other->bounds_min.z) - 0.01f))
            continue;
        other->snap_origin = *position;
        position->x += velocity->x;
        position->y -= velocity->y;
        other->ground_offset -= velocity->y;
        position->z += velocity->z;
        other->runtime_flags_0c8 |= 4;
        if (BlockInBlock(world, other, i, &other->supporting_block))
            other->runtime_flags_0c9 |= 4;
        NuSpecialUpdate(&other->special);
    }
}

void MoveBlocks(WORLDINFO_s *world, pushblock_s *block, i32 index, nuvec_s *velocity) {
    NUVEC delta = {velocity->x, 0.0f, velocity->z};
    block->position->x += delta.x;
    block->position->z += delta.z;
    NuSpecialUpdate(&block->special);
    for (i32 i = 0; i < block->end_position_count; ++i) {
        NUMTX *matrix = NuSpecialGetInstanceMtx(&block->end_position_specials[i]);
        matrix->m30 = block->position->x;
        matrix->m32 = block->position->z;
        NuSpecialUpdate(&block->end_position_specials[i]);
    }
    if (BlockInBlock(world, block, index, &block->supporting_block))
        block->runtime_flags_0c9 |= 4;
    else {
        MoveBlocksOverBlock(world, block, index, &delta);
        if (block->pushing_object && (fabsf(delta.x) > 0.001f || fabsf(delta.z) > 0.001f) &&
            NuFmod(GameTimer.time_elapsed, 0.25f) < 0.1f)
            NewRumble(block->pushing_object->pad_gamepad->pad, ((f32)qrand() * 1.5259022e-5f) * 0.05f, 0);
    }
    if (block->runtime_flags_0c8 & 8) {
        delta.y = velocity->y;
        delta.x = delta.z = 0;
        block->position->y -= delta.y;
        block->ground_offset -= delta.y;
        pushblock_s *hit = BlockInBlock(world, block, index, &block->supporting_block);
        if (hit) {
            block->settled_height = hit->position->y + hit->bounds_max.y;
            block->runtime_flags_0c9 |= 8;
        } else
            MoveBlocksOverBlock(world, block, index, &delta);
        NuSpecialUpdate(&block->special);
    }
}

i32 TerrainBlockOnBlock(WORLDINFO_s *world, pushblock_s *block, nuvec_s *points, float *heights) {
    if (!heights || !points)
        return 0;
    heights[0] = heights[1] = heights[2] = heights[3] = -10000.0f;
    for (i32 i = 0; i < world->push_block_count; ++i) {
        pushblock_s *other = &world->push_blocks[i];
        if (other == block)
            continue;
        f32 xmin = other->position->x - fabsf(other->bounds_min.x),
            xmax = other->position->x + fabsf(other->bounds_max.x);
        f32 zmin = other->position->z - fabsf(other->bounds_min.z),
            zmax = other->position->z + fabsf(other->bounds_max.z);
        f32 height = fabsf(other->bounds_max.y) + other->position->y;
        for (i32 j = 0; j < 4; ++j)
            if (points[j].x > xmin && points[j].x < xmax && points[j].z > zmin && points[j].z < zmax)
                heights[j] = height;
    }
    return 1;
}

void GizmoPushBlockInitAndReset(WORLDINFO_s *world, void *progress) {
    world->push_block_position_count = 0;
    runoutofpostabspace = 0;
    pushposincrease = 0;
    world->push_block_positions = reinterpret_cast<NUVEC *>(world->giz_buffer.void_ptr);
    world->giz_buffer.addr =
        ALIGN(world->giz_buffer.addr + world->current_level->max_push_block_end_pos * sizeof(NUVEC), 4);
    pushblock_s *block = world->push_blocks;
    ResetPushProgress(world, progress);

    for (i32 index = 0; index < world->push_block_count; ++index, ++block) {
        block->runtime_flags_0c9 &= 0x8f;
        nuinstanim_s *animation = NuSpecialGetInstAnim(&block->special);
        if (animation != NULL) {
            i32 positions =
                static_cast<i32>(NuAnimEndFrameOld(block->special.scene->instance_animation_data[animation->anim_ix])) &
                7;
            block->output_count = positions;
            block->runtime_flags_0c9 = (block->runtime_flags_0c9 & 0x8f) | ((positions & 7) << 4);
            const i32 required = world->push_block_position_count + positions;
            if (!(required > world->current_level->max_push_block_end_pos)) {
                if (positions != 0 && runoutofpostabspace == 0) {
                    block->snap_positions = &world->push_block_positions[world->push_block_position_count];
                    f32 animation_frame = 1.0f;
                    for (i32 position = 0; position < ((block->runtime_flags_0c9 >> 4) & 7);
                         ++position, animation_frame += 1.0f) {
                        NUMTX evaluated;
                        EvalAnim(&block->special, animation_frame, &evaluated, 0);
                        block->snap_positions[position] = *reinterpret_cast<NUVEC *>(&evaluated.m30);
                        evaluated = *NuSpecialGetMtx(&block->special);
                        block->snap_positions[position].x += evaluated.m30;
                        block->snap_positions[position].y += evaluated.m31;
                        block->snap_positions[position].z += evaluated.m32;
                        ++world->push_block_position_count;
                    }
                }
            } else {
                pushposincrease +=
                    runoutofpostabspace == 0 ? required - world->current_level->max_push_block_end_pos : positions;
                runoutofpostabspace = 1;
            }
        }
        ResetSinglePushBlock(world, block, index);
    }

    block = world->push_blocks;
    auto world_push_block_count = world->push_block_count;
    for (i32 index = 0; index < world_push_block_count; ++index, ++block) {
        NUVEC centre = *block->position;
        block->velocity = v000;
        block->target_velocity = v000;
        block->snap_origin = centre;

        const f32 left = fabsf(block->bounds_min.x) - 0.006f;
        const f32 right = fabsf(block->bounds_max.x) - 0.006f;
        const f32 back = fabsf(block->bounds_min.z) - 0.006f;
        const f32 front = fabsf(block->bounds_max.z) - 0.006f;
        const f32 y = centre.y - fabsf(block->bounds_min.y) + 0.001f + 0.025f;
        NUVEC corners[4] = {centre, centre, centre, centre};
        corners[0].y = corners[1].y = corners[2].y = corners[3].y = y;
        corners[0].x -= left;
        corners[0].z -= back;
        corners[1].x += right;
        corners[1].z -= back;
        corners[2].x += right;
        corners[2].z += front;
        corners[3].x -= left;
        corners[3].z += front;
        PlatOnOff(block->platform_id, 0);
        NewTerrPlatformsOff();
        const f32 height0 = GameShadow(NULL, &corners[0], 0.1f, -1);
        block->terrain_info[0] = static_cast<i8>(ShadowInfo());
        block->extra_terrain_info[0] = static_cast<i8>(EShadowInfo());
        NewTerrPlatformsOff();
        const f32 height1 = GameShadow(NULL, &corners[1], 0.1f, -1);
        block->terrain_info[1] = static_cast<i8>(ShadowInfo());
        block->extra_terrain_info[1] = static_cast<i8>(EShadowInfo());
        NewTerrPlatformsOff();
        const f32 height2 = GameShadow(NULL, &corners[2], 0.1f, -1);
        block->terrain_info[2] = static_cast<i8>(ShadowInfo());
        block->extra_terrain_info[2] = static_cast<i8>(EShadowInfo());
        NewTerrPlatformsOff();
        const f32 height3 = GameShadow(NULL, &corners[3], 0.1f, -1);
        block->terrain_info[3] = static_cast<i8>(ShadowInfo());
        block->extra_terrain_info[3] = static_cast<i8>(EShadowInfo());
        PlatOnOff(block->platform_id, 1);

        if (height1 == height2 && height0 == height1 && height2 == height3) {
            NewTerrPlatformsOff();
            centre.y = GameShadow(NULL, &centre, 5.0f, -1);
            if (centre.y == 2000000.0f) {
                centre.y = 0.0f;
            }
            block->ground_height = centre.y;
        } else {
            block->ground_height = (height1 + height0 + height2 + height3) * 0.25f;
        }
        f32 support_heights[4];
        TerrainBlockOnBlock(world, block, corners, support_heights);
        block->previous_extra_terrain_info = *reinterpret_cast<i32 *>(block->extra_terrain_info);
    }

    for (i32 index = 0; index < world->push_block_count; ++index) {
        ResetSinglePushBlockHeight(world, &world->push_blocks[index], index);
    }
}

i32 OtherBlockInRange(WORLDINFO_s *world, pushblock_s *block, nuvec_s *position, i32 excluded) {
    NUVEC centre;
    f32 radius;
    NuSpecialGetRadius(&block->special, &centre, &radius);
    radius *= radius;
    for (i32 i = 0; i < world->push_block_count; ++i) {
        if (i == excluded)
            continue;
        pushblock_s *other = &world->push_blocks[i];
        if ((other->packed_state_flags & 0x01040000) != 0x01040000 || (other->flags_0cb & 4))
            continue;
        if (!NuSpecialGetVisibilityFn(&other->special))
            continue;
        NUVEC minimum, maximum;
        NuSpecialGetBounds(&other->special, &minimum, &maximum);
        NuFmax(fabsf(minimum.x), maximum.x);
        NuFmax(fabsf(minimum.z), maximum.z);
        f32 other_radius;
        NuSpecialGetRadius(&other->special, &centre, &other_radius);
        centre = *other->position;
        f32 x = centre.x - position->x, z = centre.z - position->z;
        f32 distance = (x * x + z * z) - other_radius * other_radius;
        if (radius >= distance || 0.0f >= distance)
            return 1;
    }
    return 0;
}

i32 SnapPosTaken(WORLDINFO_s *world, pushblock_s *, nuvec_s *position, i32 excluded_index) {
    for (i32 i = 0; i < world->push_block_count; ++i) {
        if (i == excluded_index) {
            continue;
        }
        pushblock_s *block = &world->push_blocks[i];
        if ((block->packed_state_flags & 0x02000300) == 0) {
            continue;
        }
        const f32 dx = block->position->x - position->x;
        const f32 dy = block->position->y - position->y;
        const f32 dz = block->position->z - position->z;
        if (dx * dx + dy * dy + dz * dz <= 0.05f * 0.05f) {
            return 1;
        }
    }
    return 0;
}

void PushSeekComplete(pushblock_s *block, i32 index) {
    block->completed_outputs |= 1 << index;
    block->runtime_flags_0c9 &= ~2;
    if (!(block->flags_0cb & 0x20)) {
        block->flags_0cb |= 2;
        block->runtime_flags_0c9 |= 1;
    }
    if (block->pushing_object)
        NewBuzz(block->pushing_object->pad_gamepad->pad, 0.1f, 0);
}

void UpdatePushBlocks(void *world_ptr, void *, float) {
    static f32 snap_time = 0.25f;
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (!world || !world->push_block_count || !world->push_blocks)
        return;
    snap_time = 0.25f;
    i32 target_seen = 0;
    for (i32 index = 0; index < world->push_block_count; ++index) {
        pushblock_s *p = &world->push_blocks[index];
        i32 visible = NuSpecialGetVisibilityFn(&p->special) & 1;
        p->flags_0ca = (p->flags_0ca & ~4) | (visible << 2);
        if (!visible)
            continue;
        GizObstacles_AddTrigger(p->position);
        if ((p->runtime_flags_0c8 & 0x81) == 0x81)
            p->runtime_flags_0c8 &= 0x3f;
        if ((i8)p->runtime_flags_0c8 < 0)
            p->runtime_flags_0c8 |= 0x40;
        if ((p->runtime_flags_0c9 & 1) || (p->flags_0cb & 2))
            continue;
        if (__builtin_expect(!!(p->snap_positions), 1)) {
            if (p->runtime_flags_0c9 & 2) {
                if (p->snap_timer <= snap_time) {
                    f32 fraction = NU_SIN_LUT((i32)((p->snap_timer / snap_time) * 16384.0f + 49152.0f + 16384.0f));
                    i32 ix = (p->packed_state_flags >> 15) & 7;
                    p->position->x = p->snap_origin.x + (p->snap_positions[ix].x - p->snap_origin.x) * fraction;
                    p->position->y = p->snap_origin.y + (p->snap_positions[ix].y - p->snap_origin.y) * fraction;
                    p->position->z = p->snap_origin.z + (p->snap_positions[ix].z - p->snap_origin.z) * fraction;
                    NuSpecialUpdate(&p->special);
                    for (i32 i = 0; i < p->end_position_count; ++i) {
                        NUMTX *m = NuSpecialGetInstanceMtx(&p->end_position_specials[i]);
                        NUVEC *target = &p->snap_positions[(p->packed_state_flags >> 15) & 7];
                        m->m32 = p->end_position_origins[i].z + (target->z - p->end_position_origins[i].z) * fraction;
                        m->m30 = p->end_position_origins[i].x + (target->x - p->end_position_origins[i].x) * fraction;
                        NuSpecialUpdate(&p->end_position_specials[i]);
                    }
                    p->snap_timer += FRAMETIME;
                    continue;
                }
                PushSeekComplete(p, (p->packed_state_flags >> 15) & 7);
            } else {
                for (i32 i = 1; i < ((p->runtime_flags_0c9 >> 4) & 7); ++i) {
                    NUVEC *target = &p->snap_positions[i];
                    NUVEC delta = {target->x - p->position->x, target->y - p->position->y, target->z - p->position->z};
                    f32 radius = p->snap_distance;
                    if (world->current_level == HOTHESCAPEB_LDATA &&
                        NuVecXZDistSqr(target, &hothbtestpos, NULL) < 0.0625f)
                        radius *= 2.5f;
                    else
                        radius += radius;
                    if (!(radius * radius > (delta.y * delta.y + delta.x * delta.x) + delta.z * delta.z))
                        continue;
                    if (SnapPosTaken(world, p, target, index))
                        continue;
                    if ((p->flags_0cb & 0x10) || OtherBlockInRange(world, p, &p->snap_positions[i], index)) {
                        target_seen = 1;
                        continue;
                    }
                    p->runtime_flags_0c9 |= 2;
                    if (p->flags_0cb & 0x20)
                        p->flags_0cb |= 0x10;
                    p->snap_origin.x = p->position->x;
                    p->snap_origin.z = p->position->z;
                    for (i32 j = 0; j < p->end_position_count; ++j)
                        p->end_position_origins[j] = *NuSpecialGetDrawPos(&p->end_position_specials[j]);
                    p->packed_state_flags = (p->packed_state_flags & 0xfffc7fff) | ((i & 7) << 15);
                    p->snap_timer = FRAMETIME;
                    target_seen = 1;
                    break;
                }
                if (!target_seen) {
                    if (p->flags_0cb & 0x20) {
                        p->flags_0cb &= ~0x10;
                        p->packed_state_flags &= 0xfffc7fff;
                    }
                    if (!(p->flags_0cb & 0x40))
                        p->completion_flags &= 0xf807;
                }
            }
        }
        if (!(p->runtime_flags_0c8 & 0x40) && ((p->runtime_flags_0c8 & 3) == 2 || p->snap_origin.x != p->position->x ||
                                               p->snap_origin.z != p->position->z)) {
            p->target_velocity.z = p->target_velocity.x = p->velocity.z = p->velocity.x = 0;
        }
        if (!(p->runtime_flags_0c8 & 0x49))
            continue;
        if (p->runtime_flags_0c8 & 0x41) {
            i32 surface = p->terrain_info[0];
            bool same = p->terrain_info[1] == surface && p->terrain_info[2] == surface && p->terrain_info[3] == surface;
            if (__builtin_expect(!!(same), 1)) {
                if ((u32)surface <= 31 && (TerSurface[surface].flags & 0x20000))
                    p->runtime_flags_0c8 |= 0x40;
                else
                    p->runtime_flags_0c8 &= ~0x40;
            }
            if (__builtin_expect(!!(p->pushing_object), 1)) {
                if (!(p->runtime_flags_0c8 & 0x40))
                    p->target_velocity.z = p->target_velocity.x = 0;
                f32 x = p->pushing_object->target_velocity.x, z = p->pushing_object->target_velocity.z;
                if (!(fabsf(x) > fabsf(z))) {
                    p->velocity.x = p->target_velocity.x = 0;
                    p->target_velocity.z = z * FRAMETIME;
                } else {
                    p->velocity.z = p->target_velocity.z = 0;
                    p->target_velocity.x = x * FRAMETIME;
                }
            } else if (same) {
                p->target_velocity.x *= TerSurface[surface].movement_scale;
                p->target_velocity.z *= TerSurface[surface].movement_scale;
            } else
                p->target_velocity.x = p->velocity.x = p->target_velocity.z = p->velocity.z = 0;
            p->velocity.x = SeekValF(p->velocity.x, p->target_velocity.x, 1.0f);
            p->velocity.z = SeekValF(p->velocity.z, p->target_velocity.z, 1.0f);
            if (p->velocity.x == 0) {
                if (fabsf(p->velocity.z) <= 0.0001f) {
                    p->runtime_flags_0c8 &= 0x3f;
                    p->velocity.z = p->target_velocity.z = 0;
                }
            } else {
                if (__builtin_expect(fabsf(p->velocity.x) <= 0.0001f, 0)) {
                    p->runtime_flags_0c8 &= 0x3f;
                    p->velocity.x = p->target_velocity.x = 0;
                }
            }
        }
        if (p->runtime_flags_0c8 & 8) {
            p->target_velocity.y = 10.0f * FRAMETIME;
            p->target_velocity.x = p->target_velocity.z = p->velocity.x = 0;
            p->velocity.y = SeekValF(p->velocity.y, p->target_velocity.y, 1.0f);
            p->velocity.z = 0;
        } else
            p->target_velocity.y = 0;
    }
    for (i32 index = 0; index < world->push_block_count; ++index) {
        pushblock_s *p = &world->push_blocks[index];
        if (__builtin_expect((p->flags_0ca & 4) && !(p->packed_state_flags & 0x04000100) && !(p->flags_0cb & 2) &&
            (p->runtime_flags_0c8 & 0x69), 0))
            MoveBlocks(world, p, index, &p->velocity);
    }
    for (i32 index = 0; index < world->push_block_count; ++index) {
        pushblock_s *p = &world->push_blocks[index];
        if (!(p->flags_0ca & 4) || (p->flags_0cb & 2))
            continue;
        if (!NuSpecialGetVisibilityFn(&p->special) || !(p->runtime_flags_0c8 & 0x4d))
            continue;
        f32 left = fabsf(p->bounds_min.x) - 0.006f, right = fabsf(p->bounds_max.x) - 0.006f;
        f32 back = fabsf(p->bounds_min.z) - 0.006f, front = fabsf(p->bounds_max.z) - 0.006f;
        NUVEC *position = p->position;
        f32 y = (position->y - fabsf(p->bounds_min.y) + 0.001f) + 0.025f;
        if (__builtin_expect(p->ground_height > y, 0))
            y = p->ground_height + 0.25f;
        NUVEC corners[4] = {
            {position->x - left, y, position->z - back},
            {position->x + right, y, position->z - back},
            {position->x + right, y, position->z + front},
            {position->x - left, y, position->z + front},
        };
        PlatOnOff(p->platform_id, 0);
        bool platforms_off = p->supporting_block || p->previous_supporting_block;
        f32 heights[4];
        for (i32 i = 0; i < 4; ++i) {
            if (platforms_off)
                NewTerrPlatformsOff();
            heights[i] = GameShadow(NULL, &corners[i], 0.1f, -1);
            p->terrain_info[i] = ShadowInfo();
            p->extra_terrain_info[i] = EShadowInfo();
        }
        PlatOnOff(p->platform_id, 1);
        BlockInBlock(world, p, index, &p->supporting_block);
        if (p->supporting_block || p->previous_supporting_block) {
            p->previous_supporting_block = p->supporting_block;
            f32 support[4];
            TerrainBlockOnBlock(world, p, corners, support);
            for (i32 i = 0; i < 4; ++i)
                if (support[i] != -10000.0f)
                    heights[i] = support[i];
        }
        f32 low = p->ground_height - 0.01f, high = p->ground_height + 0.01f;
        if ((low > heights[0] && low > heights[1] && low > heights[2] && low > heights[3]) ||
            (heights[0] > high && heights[1] > high && heights[2] > high && heights[3] > high)) {
            p->runtime_flags_0c8 |= 8;
            p->ground_height = ((heights[0] + heights[1]) + heights[2] + heights[3]) * 0.25f + 0.01f;
            p->ground_offset = (p->position->y + p->bounds_min.y) - p->ground_height;
        }
        if (p->supporting_block) {
            p->ground_height = p->position->y + p->supporting_block->bounds_max.y;
            p->ground_offset = (p->position->y + p->bounds_min.y) - p->ground_height;
        }
        if (p->ground_offset > 0.01f)
            p->runtime_flags_0c8 |= 8;
        else if (p->runtime_flags_0c8 & 8) {
            p->runtime_flags_0c9 |= 8;
            p->settled_height = p->ground_height;
            p->runtime_flags_0c8 &= ~8;
        }
        i32 safe = 0;
        for (i32 i = 0; i < 4; ++i)
            if (p->extra_terrain_info[i] == 8 || p->terrain_info[i] == 14)
                ++safe;
        bool reset = safe != 4;
        if (world->current_level == HOTHESCAPEB_LDATA && NuVecXZDistSqr(p->position, &hothbtestpos, NULL) < 1.0f) {
            if (!(corners[0].z < 13.2f && !(corners[1].z >= 13.2f) && !(corners[2].z >= 13.2f) &&
                  !(corners[3].z >= 13.2f)))
                reset = true;
        }
        if (reset)
            p->runtime_flags_0c9 |= 4;
    }
    for (i32 index = 0; index < world->push_block_count; ++index) {
        pushblock_s *p = &world->push_blocks[index];
        if (!(p->flags_0ca & 4) || (p->runtime_flags_0c9 & 1) || (p->flags_0cb & 2))
            continue;
        if (!(p->runtime_flags_0c9 & 4) && (p->runtime_flags_0c8 & 1))
            GameAudio_PlaySfx(72, p->position, GameAudio_GetPlrSfxBits(p->pushing_object), 0);
        if (p->runtime_flags_0c9 & 0xc) {
            if (p->runtime_flags_0c9 & 4) {
                p->position->x = p->snap_origin.x;
                p->position->z = p->snap_origin.z;
                p->target_velocity.x = p->target_velocity.z = p->velocity.x = p->velocity.z = 0;
                memcpy(p->extra_terrain_info, &p->previous_extra_terrain_info, 4);
                p->runtime_flags_0c8 &= 0x3f;
                NuSpecialUpdate(&p->special);
                for (i32 i = 0; i < p->end_position_count; ++i)
                    if (NuSpecialExistsFn(&p->end_position_specials[i])) {
                        NUMTX *m = NuSpecialGetInstanceMtx(&p->end_position_specials[i]);
                        if ((usize)m != 0xffffffd0u) {
                            m->m30 = p->snap_origin.x;
                            m->m32 = p->snap_origin.z;
                            NuSpecialUpdate(&p->end_position_specials[i]);
                        }
                    }
                p->runtime_flags_0c8 &= ~1;
                p->runtime_flags_0c9 &= ~4;
            }
            if (p->runtime_flags_0c9 & 8) {
                p->ground_height = p->settled_height;
                p->position->y = p->settled_height - p->bounds_min.y;
                p->target_velocity.y = p->velocity.y = 0;
                p->runtime_flags_0c8 &= ~8;
                p->runtime_flags_0c9 &= ~8;
                p->ground_offset = (p->position->y + p->bounds_min.y) - p->settled_height;
                NuSpecialUpdate(&p->special);
            }
        } else if (p->runtime_flags_0c8 & 1) {
            if ((p->runtime_flags_0c8 & 3) == 1 && p->pushing_object)
                GameAudio_PlaySfx(71, &p->pushing_object->apiobj.collision_position,
                                  GameAudio_GetPlrSfxBits(p->pushing_object), 0);
            if (LEGOHINT_PUSHBLOCKS != -1 && p->pushing_object &&
                (p->pushing_object->apiobj.object_flags & 0x81) == 0x81)
                Hint_SetComplete(LEGOHINT_PUSHBLOCKS);
        }
    }
    for (i32 index = 0; index < world->push_block_count; ++index) {
        pushblock_s *p = &world->push_blocks[index];
        if (!(p->flags_0ca & 4))
            continue;
        if (p->pushing_object) {
            if (p->platform_id != -1 && (p->runtime_flags_0c8 & 9)) {
                p->pushing_object->field_0xefc |= 1;
                AddShoveObject(&p->special, (i16)p->platform_id);
            } else
                p->pushing_object->field_0xefc &= ~1;
        }
        if (!(p->packed_state_flags & 0x220)) {
            i32 safe = 0;
            for (i32 i = 0; i < 4; ++i)
                if (__builtin_expect(p->extra_terrain_info[i] == 8 || p->terrain_info[i] == 14, 0))
                    ++safe;
            if (safe == 4) {
                memcpy(&p->previous_extra_terrain_info, p->extra_terrain_info, 4);
                p->snap_origin = *p->position;
            }
        }
        p->pushing_object = NULL;
        p->runtime_flags_0c8 = (p->runtime_flags_0c8 & ~2) | ((p->runtime_flags_0c8 & 1) << 1);
        p->runtime_flags_0c8 = (p->runtime_flags_0c8 & 0xea) | ((p->runtime_flags_0c8 << 1) & 0x10);
    }
}

pushblock_s *NearestPushBlock(WORLDINFO_s *world, nuvec_s *position, float range) {
    if (position == NULL || world == NULL)
        return NULL;
    const NUVEC minimum = {position->x - range, position->y - range, position->z - range};
    const NUVEC maximum = {position->x + range, position->y + range, position->z + range};
    pushblock_s *nearest = NULL;
    f32 nearest_distance = 1000000000.0f;
    pushblock_s *block = world->push_blocks;
    for (i32 i = 0; i < world->push_block_count; ++i, ++block) {
        if ((block->packed_state_flags & 0x1040100) != 0x1040000)
            continue;
        NUVEC *candidate = block->position;
        if (candidate->x < minimum.x || candidate->x > maximum.x || candidate->z < minimum.z ||
            candidate->z > maximum.z || candidate->y < minimum.y || candidate->y > maximum.y)
            continue;
        const f32 distance = NuVecDistSqr(position, candidate, NULL);
        if (distance < nearest_distance) {
            nearest = block;
            nearest_distance = distance;
        }
    }
    return nearest;
}

i32 Push_UpdateHints(HINT_s *) {
    if (LEGOACT_PUSH == -1)
        return 0;
    for (i32 i = 0; i < 8; ++i) {
        GameObject_s *object = Player[i];
        if (object != NULL && object->apiobj.player_controlled &&
            object->apiobj.character_model->model_data_b[LEGOACT_PUSH] != NULL &&
            NearestPushBlock(WORLD, &object->apiobj.collision_position, 2.0f) != NULL)
            return 1;
    }
    return 0;
}

i32 GizPushBlock_EndFrameCompleted(pushblock_s *push_block, i32 output_index) {
    if (push_block == NULL) {
        return -1;
    }

    if (output_index != 0) {
        const u8 completed_outputs = push_block->completion_flags / PUSH_BLOCK_FIRST_OUTPUT_FLAG;
        return (completed_outputs >> output_index) & 1;
    }
    return (push_block->completion_flags & PUSH_BLOCK_ANY_OUTPUT_MASK) != 0;
}

i32 GizPush_GetOutput(GIZMO *gizmo, i32 output_index, i32) {
    pushblock_s *push_block = static_cast<pushblock_s *>(gizmo->object);
    if (push_block == NULL) {
        return 0;
    }
    return GizPushBlock_EndFrameCompleted(push_block, output_index) != 0;
}

ADDGIZMOTYPE *Push_RegisterGizmo(i32 type_id) {
    static ADDGIZMOTYPE addtype;

    addtype = Default_ADDGIZMOTYPE;
    addtype.name = "PushBlocks";
    addtype.prefix = "";
    addtype.fns.unknown1 = 0x24c;
    addtype.fns.get_max_gizmos_fn = Push_GetMaxGizmos;
    addtype.fns.add_gizmos_fn = Push_AddGizmos;
    addtype.fns.early_update_fn = NULL;
    addtype.fns.late_update_fn = UpdatePushBlocks;
    addtype.fns.draw_fn = NULL;
    addtype.fns.panel_draw_fn = NULL;
    addtype.fns.get_gizmo_name_fn = GizPush_GetGizmoName;
    addtype.fns.get_output_fn = GizPush_GetOutput;
    addtype.fns.get_output_name_fn = GizPush_GetOutputName;
    addtype.fns.get_num_outputs_fn = GizPush_GetNumOutputs;
    addtype.fns.activate_fn = Push_Activate;
    addtype.fns.set_visibility_fn = Push_SetVisibility;
    addtype.fns.get_visibility_fn = NULL;
    addtype.fns.get_pos_fn = NULL;
    addtype.fns.using_special_fn = NULL;
    addtype.fns.bolt_hit_plat_fn = Pushblocks_BoltHitPlat;
    addtype.fns.get_best_bolt_target_fn = NULL;
    addtype.fns.bolt_hit_fn = NULL;
    addtype.fns.allocate_progress_data_fn = Push_AllocateProgressData;
    addtype.fns.clear_progress_fn = Push_ClearProgress;
    addtype.fns.store_progress_fn = Push_StoreProgress;
    addtype.fns.reset_fn = Push_Reset;
    addtype.fns.reserve_buffer_space_fn = PushBlocks_ReserveBufferSpace;
    addtype.fns.load_fn = edpush_Load;
    addtype.fns.post_load_fn = NULL;
    addtype.fns.add_level_sfx_fn = NULL;
    pushblock_gizmotype_id = type_id;

    return &addtype;
}

