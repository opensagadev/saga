// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "legoapi/legoapi_types.h"
#include "decomp.h"
#include "legoapi/actions/movement/jumping.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/ai/core/legoai.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/core/input/gamepads.h"
#include <math.h>
#include "globals.h"
#include "legoapi/characters/motion.h"
#include "legoapi/world/world.h"
#include "gameapi/ai/aisys/aisys.h"
#include <string.h>
struct CLIMBOBJECT_s;

void FindAnglesXY(NUVEC *, u16 *, u16 *);
void Climb_SetMagnetAction(GameObject_s *);
void Climb_SetMagnetDrawOffsetTarget(GameObject_s *, NUVEC *);

f32 MAGNETOFFSET;

#include "legoapi/characters/motion/gameanim.h"

void Climb_SetMagnetAction(GameObject_s *object);

void Climb_SetMagnetDrawOffsetTarget(GameObject_s *object, nuvec_s *offset);

extern "C" i32 NewRayCastGetImpactTerrainType();
extern i32 TERRAINMASK_NONWEAPON, TERRAINMASK_NONDROID;
i32 GameRayCast(NUVEC *, NUVEC *, f32, i32);

static __used__ void ClimbObject_FindNormal(CLIMBOBJECT_s *object);

i32 CanClimbSurface(GameObject_s *object, i32 surface);

// ==== src/legoapi/props/objects/ledgeterrain.cpp: all remaining content ====
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/gizmos/transport/ledges.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nutex.h"

f32 LEDGETERRAINLOOKAHEAD = 0.02f;
i32 LedgeTerrain_CheckAnims = 1;

static inline i32 LedgeTerrain_Attach(GameObject_s *object, u16 facing, NUVEC *position, u16 *wall_angle);

// ==== src/legoapi/characters/motion/move.cpp: declarations ====

#include <stdlib.h>
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/hatmachine.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/actions/character/snake.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuqfnt.h"

#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/config/cheat.h"
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
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/world/level.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/numtx_inline.h"

// The retail move.cpp initializer owns these four managed references.

// Original action/context lookup data, with its arrays and pointers in one owner.

extern CHARACTER_CONTEXT_INFO_s * CInfo;
extern ACTIONINFO_s * ActionInfo;

u16 SeekRot(u16 current, u16 target, f32 rate);
extern "C" void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
extern "C" void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);
void SetWeaponOut(GameObject_s *object);
void SetWeaponIn(GameObject_s *object);
void SlowWeaponIn(GameObject_s *object);
void FastWeaponIn(GameObject_s *object, i32 sound);
void SlowWeaponOut(GameObject_s *object);
void FastWeaponOut(GameObject_s *object, i32 sound);
extern i16 id_IMPERIALGUARD;
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);
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
extern "C" i16 id_ATAT, id_DODGEM;

// Multiplies the ordinary camera's per-frame position blend.  The original
// keeps this as writable camera state (default 1.0), rather than folding it
// into the socket seek rate.

extern "C" {
    extern i16 id_BODYGUARD;
    extern i16 id_GRIEVOUS;
}

#include "legoapi/characters/motion/move_types.h"

i32 PodLevel(AREADATA_s *area);

// Retail splits the guard off as an inline head; the rest is this partial clone.

extern i32 LevFlag[4];
// Original exported level control, initialized to 2 at 0x623d00.

extern "C" i16 id_LANDSPEEDER, id_WOOKIEFLYER, id_STAP2;

// These pod tuning values are zero-initialized in the original image.

extern "C" void PlaySfxById(i32 sfx_id, nuvec_s *position);

// Retail re-tests the three block animations at each use instead of caching a flag.
#define HAS_BLOCK_ANIMS(animations)                                                                                    \
    ((animations)[0x1a] != NULL || (animations)[0x1b] != NULL || (animations)[0x1c] != NULL)

// Retail inlines the fade and step checks into each caller; this is the remaining partial clone.

// Retail splits the context test off as an inline head; the rest is this partial clone.

extern "C" {
    extern i16 id_ATST, id_MINIATST, id_ATST_LOWRES, id_ATAT, id_MINIATAT, id_MINIATTE;
}

float SeekLinearF(float current, float target, float step);

extern i16 id_ATAT;
extern i32 TERRAINMASK_NONWEAPON;

DECOMP_ASSERT(sizeof(AWKWARDSHAPESPHERE_s) == 12, "Awkward shape sphere size");
DECOMP_ASSERT(offsetof(AWKWARDSHAPESPHERE_s, radius) == 8, "Awkward shape sphere radius offset");

extern i16 id_GEONOSIAN, id_MINIATST, id_ATST_LOWRES, id_ATAT, id_MINIATAT, id_MINIATTE, id_SENTRYDROID;
void SetWeaponIn(GameObject_s *);

// ==== src/legoapi/characters/motion/gameanim.cpp: declarations ====
#include "gameapi/edtools/edfile.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/gizmo/base/gizmo.h"
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

i32 GetDefaultIdle(GameObject_s *object);
// Retail uses translation-unit-local helpers with compiler-inferred private
// calling conventions; keep their shared animation-mode call sites intact.

extern i16 id_BODYGUARD;
extern i16 id_GONKDROID;

#include "legoapi/characters/motion/gameanim_types.h"

// ==== src/legoapi/world/levels/episodeV.cpp: declarations ====
#include "gameapi/ai/aisys/aipath.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/ai/game/creature.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/gizmos/traps/gizbombgen.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/menus/core/panel.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numechptr.hpp"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nu3d/nuspline.h"

#include <stdio.h>

extern "C" i16 id_PROBEDROID, id_ATST_LOWRES, id_ATAT;
extern AILOCATOR_s * locator;

#include "legoapi/world/levels/episodeV_types.h"
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

extern ADDPART_s Default_ADDPART;

extern "C" {
    void DebFreeInstantly(i32 *handle);
    // Shared suspend flag consulted by all timed debris emitters.

    void SetSfxBit_On(i32);
    void PlaySfxById(i32, NUVEC *);

    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
}

// Forward declarations for local (static) part/gizmo helper stubs.

// Local (static) part (PART_s), pickup (GIZMOPICKUP_s), gizmo flow
// (GIZFLOW_s/FLOWBOX_s), power-up and space/starfighter helpers. Stubbed as
// local `t` symbols matching res/libTTapp.so.

extern "C" {

void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);

    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

PART_s *AddPart(ADDPART_s *params);

    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);

void AddVariableShotDebrisEffect(i32 effect, NUVEC *position, i32 count, i16 z_rotation, i16 y_rotation);

void AddVariableShotDebrisEffectTimed1(i32 effect, NUVEC *position, i32 count, f32 time, i16 z_rotation,
                                           i16 y_rotation, NUMTX *particle_orientation);

    // The editor loader clears these page IDs with aligned vector stores.

    void DebFreeInstantly(i32 *);

} // extern "C"

extern f32 FRAMETIME;
void PartStop_Flickerer(PART_s *);
i32 PartDraw_Flickerer(PART_s *);

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

void PartImpact_Brick(PART_s *part);

void PartStop_Flickerer(PART_s *part);

i32 PartDraw_Flickerer(PART_s *part);

// ==== src/legoapi/items/objects/gameobjects.cpp: declarations ====
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

extern "C" {
    extern NUVEC plr_lastpos;
    extern i16 id_GRIEVOUS;
    extern i16 id_BODYGUARD;
    extern i16 id_IMPERIALGUARD;
    extern i16 id_JAWA;
    extern i16 id_UGNAUGHT;
    extern i16 id_ATAT;
    extern i16 id_GONKDROID;
    extern i16 id_LANDSPEEDER, id_MILLENNIUMFALCON;
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisPosOrientationMtx(i32, NUMTX *);
    void DebFreeInstantly(i32 *);
    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);
}

// Written by ThingManager's ctor (original global @0x124f2e0, .bss).

#include "legoapi/items/objects/gameobjects_types.h"

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

f32 GameShadow(GameObject_s *object, nuvec_s *position, f32 probe_height, i32 terrain_mask);

i32 GameRayCast(NUVEC *position, NUVEC *displacement, f32 radius, i32 mask);

#if defined(__i386__)
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone, regparm(2), sseregparm))
#else
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone))
#endif

// Retail keeps only the IPA-SRA clone, which takes the other object's x and z by address.

void *GameBufferAlloc(variptr_u *buf, variptr_u *buf_end, i32 size);

u16 SeekRot(u16, u16, f32);
i32 RotDiff(u16, u16);

extern "C" i16 id_THEEMPEROR, id_IMPERIALGUARD, id_BODYGUARD;

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

// ==== src/legoapi/actions/combat/fighting.cpp: declarations ====
#include "legoapi/actions/combat/fighting.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/characters/core/playeritems.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/misc/utilities.h"

i32 qrand();

void SetWeaponOut(GameObject_s *);
DECOMP_ASSERT(offsetof(GameObject_s, field_0x7e4) == 0x7e4, "Quick shoot record offset");
DECOMP_ASSERT(offsetof(GameObject_s, field_0x7e8) == 0x7e8, "Quick shoot record count offset");
DECOMP_ASSERT(offsetof(GameObject_s, quick_shoot_flags) == 0xe2f, "Quick shoot flags offset");
DECOMP_ASSERT(offsetof(GameObject_s, quick_shoot_bolt_id) == 0xe3e, "Quick shoot bolt ID offset");

// ==== src/legoapi/characters/core/players.cpp: declarations ====

#include "legoapi/items/base/collection.h"
#include "legoapi/world/levels/episode.h"
#include "legogame/game.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nufile/nufpar.h"

struct MechTouchUIElement;
struct TouchHolder;

extern "C" NUVEC plr_lastpos;

// --- Helpers moved from world.cpp ---

// ---- Player start spawn entries ----

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

void GameAudio_PlaySfx(i32 sfx, NUVEC *position, i32 flags, i32 volume);

namespace {

} // namespace

#include "legoapi/characters/core/players_types.h"

DECOMP_ASSERT(sizeof(SPECIAL_LAYER_s) == 0xc, "SPECIAL_LAYER_s size");

// ==== src/legoapi/actions/movement/pushing.cpp: declarations ====
#include "legoapi/gizmos/door/push.h"
#include "legoapi/items/base/animpacket.h"

void FastWeaponIn(GameObject_s *, i32);

f32 PushingTowardsAngle(u16 input_angle, u16 direction);

// ==== src/legoapi/characters/motion/camera.cpp: declarations ====
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/cutscenes/minicamcut.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/numouse.h"

#include "legoapi/characters/motion/camera_types.h"
// Original data symbol testmovef is float0.005 at 0x617cb0.

DECOMP_ASSERT(sizeof(VIEWCAM_s) == 0x28, "ViewCam size");
DECOMP_ASSERT(offsetof(VIEWCAM_s, pitch) == 0x10, "ViewCam pitch offset");
DECOMP_ASSERT(offsetof(VIEWCAM_s, gamepad) == 0x24, "ViewCam gamepad offset");

extern "C" i16 id_GRABCONTROL, id_GRABR2CONTROL, id_LANDSPEEDER, id_ATST, id_DEWBACK;
f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);

DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, path) == 0x40, "Camera context path offset");
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, flags) == 0x4c, "Camera context flags offset");

// ==== src/legoapi/audio/specialsfx.cpp: declarations ====

i32 qrand(void);

// ==== src/legoapi/actions/character/speederchase.cpp: declarations ====
#include "legoapi/items/collect/spacelevel.h"
#include "gameapi/edtools/edcam.h"

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

#include "legoapi/actions/character/speederchase_types.h"

extern SPEEDERCHASEVALUES speedervals[4];
DECOMP_ASSERT(sizeof(SPEEDERCHASEVALUES) == 0x1c, "Speeder chase tuning stride");
DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

extern "C" void AddDebrisEffect(i32 *, i32, f32, f32, f32);
extern "C" void DebrisPosOrientationMtx(i32, NUMTX *);

// ==== src/legoapi/gizmo/base/gizmo.cpp: declarations ====

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
#include "legoapi/gizmos/traps/shards.h"

struct FLOWBOX_s;

DECOMP_ASSERT(sizeof(GIZMOTYPES) == 0xc, "Gizmo type-table header ABI");
DECOMP_ASSERT(offsetof(GIZMOTYPES, types) == 8, "Gizmo type-table pointer offset");

// ==== src/legoapi/misc/legoapi_misc.cpp: declarations ====
#include "gameframework/saveload.h"
#include "legoapi/menus/screens/gamestructure.h"

extern void *CutStopInfo;

// ==== src/legoapi/world/levels/episodeII.cpp: declarations ====

#include "nu2api/nu3d/nuhspecial.h"

struct instNUGCUTSCENE_s;

#include "legoapi/world/levels/episodeII_types.h"
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

void JediB_Init(WORLDINFO_s *world);

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

#include "legoapi/misc.h"
#include "legoapi/world/levels/podrace.h"

#include "legoapi/world/levels/episodeI_types.h"
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

void PodRaceAUpdate(WORLDINFO_s *world);

// ===========================================================================
// Pod sprint (Sebulba events)
// ===========================================================================

void PodSprintA_Init(WORLDINFO_s *world);

void PodSprintA_Update(WORLDINFO_s *world);

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

#include "legoapi/ai/core/gameai_types.h"

// ==== src/legoapi/items/objects/gameobjects.cpp: declarations ====

extern "C" {
    extern NUVEC plr_lastpos;
    extern i16 id_GRIEVOUS;
    extern i16 id_BODYGUARD;
    extern i16 id_IMPERIALGUARD;
    extern i16 id_JAWA;
    extern i16 id_UGNAUGHT;
    extern i16 id_ATAT;
    extern i16 id_GONKDROID;
    extern i16 id_LANDSPEEDER, id_MILLENNIUMFALCON;
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisPosOrientationMtx(i32, NUMTX *);
    void DebFreeInstantly(i32 *);
    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);
}

// ==== src/legoapi/gizmos/trigger/ai.cpp: all remaining content ====

// ==== src/legoapi/ai/game/creature.cpp: declarations ====

extern "C" {
    extern NUVEC plr_lastpos;
    void ResetAnimPacket(ANIMPACKET_s *packet, i16 animation);
}

#include "legoapi/ai/game/creature_types.h"

// ==== src/legoapi/ai/game/aipathcnxhelper.cpp: declarations ====

#include <stdint.h>
#include "nu2api/nucore/nulist.h"

// ==== src/legoapi/ai/core/ai_sys.cpp: declarations ====
#include "gameapi/ai/aisys/aiscript_types.h"

struct nuhspecial_s;
struct nuvec_s;

void *GameBufferAlloc(VARIPTR *, VARIPTR *, i32);

// ==== src/legoapi/actions/character/simpletons.cpp: declarations ====

#include "legoapi/actions/character/simpletons_types.h"

// ---- begin src/legoapi/ai/core/aipathbuild.cpp ----
#include "editor/edpath.h"

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

#include "legoapi/ai/core/ai_pathgeometry.h"

// IPA-SRA hands this clone the address of system->path_sys rather than the system.

// ---- begin src/gameapi/ai/aisys/aisys.cpp ----

#include "batman.h"

#include "legoapi/gizmo/object/takeoverobjects.h"

extern "C" ADDPART_s Default_ADDPART;

extern "C" PART_s *AddPart(ADDPART_s *);

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

#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nurdp.h"
#include "nu2api/nufile/nufilepak.h"

#if defined(__i386__)
#define AISCRIPT_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define AISCRIPT_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif

// Retail inlines the "goto" test into each caller; this is the remaining partial clone.

// Retail inlines the system test into its callers; this is the remaining partial clone.

#include "legoapi/ai/core/aisysall_types.h"

void RegisterAIScriptActions(AIACTIONDEF *action_defs);

void RegisterAIScriptConditions(AICONDITIONDEF *cond_defs);

// ==== src/legoapi/ai/core/ai_sys_stubs.cpp: declarations ====
#include "editor/aieditor_state.h"
#include "editor/antinode_editor_private.h"
#include "nu2api/numath/nuplane.h"

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

extern AIEDITOR_RENDER_STATE * aieditor;

} // extern "C"

// ==== src/editor/area_editor.cpp: all remaining content ====
#include "editor/aieditor_settings.h"

struct nupad_s;

DECOMP_ASSERT(sizeof(EDAIAREA_s) == 0x44, "area editor record size");

// Item colours shared by the AI editors.

// ==== src/editor/edpath.cpp: all remaining content ====
#include "editor/path_connections.h"

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

static __used__ void pathEditor_DestroySharedNode(EDAISHAREDPATHNODE_s *shared);

// Retail keeps an unreferenced partial-inlining clone of pathEditorSetCamPos:
// it moves the editor camera to a position at the cursor's height.
#if defined(__i386__)
#define PATHEDITOR_SETCAMPOS_BODY_ATTR __attribute__((used, noinline, noclone, regparm(1)))
#else
#define PATHEDITOR_SETCAMPOS_BODY_ATTR __attribute__((used, noinline, noclone))
#endif

// ==== src/editor/aieditor.cpp: all remaining content ====

#include "gameapi/edtools/edrender.h"
#include "legoapi/render/core/terrain_internal.h"
#include "nu2api/nu3d/numtl.h"

#if defined(__i386__)
#define EDANTINODE_REGPARM1 __attribute__((regparm(1)))
#else
#define EDANTINODE_REGPARM1
#endif

// ==== src/editor/creature_editor.cpp: all remaining content ====

#include "editor/edpath_types.h"
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

static __used__ void creatureEditor_cbCancelMenu(eduimenu_s *menu, eduimenu_s *);

static __used__ __attribute__((force_align_arg_pointer)) void creatureEditor_cbResetParams(eduimenu_s *menu,
                                                                                           eduiitem_s *, unsigned int);

static __used__ __attribute__((force_align_arg_pointer)) void creatureEditor_cbVisionMenu(eduimenu_s *parent,
                                                                                          eduiitem_s *, unsigned int);

static __used__ __attribute__((force_align_arg_pointer)) void creatureEditor_cbSetScript(eduimenu_s *, eduiitem_s *item,
                                                                                         unsigned int);

static __used__ void creatureEditor_cbSetScriptParam(eduimenu_s *menu, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cbSetActivation(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cbSetType(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cbSetAreaActivation(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cbSetLocator(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cbSelectRespawnLocator(eduimenu_s *parent, eduiitem_s *, unsigned int);

static __used__ void creatureEditor_cbSelectLocator(eduimenu_s *parent, eduiitem_s *, unsigned int);

static __used__ void creatureEditor_cbSelectTriggerArea(eduimenu_s *parent, eduiitem_s *, unsigned int);

static __used__ void creatureEditor_cbSetRespawnLocator(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_assigntoset(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cbSetTriggerArea(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_ngroup(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_nacross(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_xspacing(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_zspacing(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_difficulty(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_heardistance(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_viewdistance(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_minviewheight(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_maxviewheight(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_min_n_respawns(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_max_n_respawns(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_min_t_respawn(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_max_t_respawn(eduimenu_s *, eduiitem_s *item, unsigned int);

static __used__ void creatureEditor_cb_stagger_start(eduimenu_s *, eduiitem_s *item, unsigned int);

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
static __used__ void locatorEditor_cbCancelRenameMenu(eduimenu_s *menu, eduimenu_s *);
static __used__ void locatorEditor_cbCancelSelectLocatorSet(eduimenu_s *menu, eduimenu_s *);
static __used__ void locatorEditor_cbCancelRenameLocatorSetMenu(eduimenu_s *, eduimenu_s *);

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
    extern NUVEC plr_lastpos;
    extern i16 id_GRIEVOUS;
    extern i16 id_BODYGUARD;
    extern i16 id_IMPERIALGUARD;
    extern i16 id_JAWA;
    extern i16 id_UGNAUGHT;
    extern i16 id_ATAT;
    extern i16 id_GONKDROID;
    extern i16 id_LANDSPEEDER, id_MILLENNIUMFALCON;
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisPosOrientationMtx(i32, NUMTX *);
    void DebFreeInstantly(i32 *);
    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// ==== src/gameapi/ai/aisys/aistate.cpp: all remaining content ====

// ==== src/legoapi/gizmo/gizmos/gizmos_gizactions.cpp: declarations ====

i32 Action_ReleaseLocator(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count,
                          i32 first_time, float);

extern u32 GAMEPAD_SPECIAL, GAMEPAD_JUMP, GAMEPAD_TOGGLERIGHT;

i32 Action_GetLocatorFromSet(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                             i32 param_count, i32 first_time, float);

i32 Action_AssignLocatorInSet(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count,
                              i32 first_time, float);

namespace {
#include "legoapi/gizmo/gizmos/gizmos_gizactions_types.h"

} // namespace

// ==== src/legoapi/ai/game/gameantinode.cpp: declarations ====
#include "legoapi/ai/game/gameantinode.h"

// ==== src/legoapi/render/core/render.cpp: declarations ====
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

struct STATUSPACKET_s;

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

#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"

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

extern "C" {
    extern i16 id_ATAT;
}

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

extern i32 qrand(void);
extern f32 SeekLinearF(f32, f32, f32);

// ==== src/legoapi/actions/character/starwars.cpp: all remaining content ====

struct AIPACKET_s;
struct AISYS_s;

// ==== src/editor/antinode_editor.cpp: all remaining content ====

// ==== src/editor/edtoolsall.cpp: declarations ====

#include "editor/edtoolsall_types.h"

// ==== src/legoapi/render/core/terrain_stubs.cpp: declarations ====
#include "legoapi/render/core/gameliball.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nu3d/android/nurain_android.h"

#include <stddef.h>

extern i32 TERRAINMASK_NONDROID;
extern NUVEC ShadNorm;
// Runtime-selected groups appended after the fixed terrain allocation.
#include "legoapi/render/core/terrain_stubs_types.h"

extern "C" {
    void DebFreeInstantly(i32 *);
}

// NuFileExists @nufile (nucore_plain.cpp).
extern "C" i32 NuFileExists(char *name);

extern "C" {

void DebrisPosOrientationMtx(i32 handle, NUMTX *source);

} // extern "C"

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

extern i32 (*CanMagnetClimbFn)(GameObject_s *);

#include "legoapi/actions/movement/jumping_types.h"

void StartJump(GameObject_s *object, i32 movement_state);

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

#include <ctype.h>

void FindAnglesXY(nuvec_s *direction, u16 *x_rotation, u16 *y_rotation);

i32 RotDiff(u16 current, u16 target);

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

extern "C" {
    extern i16 id_ATAT;
}

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

#include "legoapi/world/levels/episode_types.h"
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
#include "legoapi/items/collect/batarang.h"

// ==== src/legoapi/gizmo/object/gizmoblowups.cpp: declarations ====

// Original writable flagremaptab at 0x668520, 32 pairs of 32-bit masks.
extern "C" {
extern u32 flagremaptab[32][2];
}

// Definitions from the original gizmoblowups translation unit.

#include "legoapi/gizmo/object/gizmoblowups_types.h"

DECOMP_ASSERT(sizeof(BLOWUPPROGRESS_s) == 0x100, "BLOWUPPROGRESS_s ABI");

DECOMP_ASSERT(offsetof(WORLDINFO_s, blowup_target_candidates) == 0x50d4, "Blowup target candidate-list ABI");

// ==== src/legoapi/characters/core/characters.cpp: declarations ====
#include "legoapi/actions/character/suit.h"
#include "legoapi/world/areas.h"

#include "legoapi/actions/character/specialmoves.h"
#include "nu2api/numusic/numusic.h"

// LoadPerm1 is one of the few game-level entry points which wires together
// otherwise C-linkage engine subsystems.  Keep these declarations local: the
// individual subsystem TUs intentionally expose their original plain names.
extern "C" {
    i32 NuFileExists(char *);

}

// The original table at 0x006281a0.  These are the material animation
// scripts made available before the permanent things scene is loaded.

#include "legoapi/characters/core/characters_types.h"

// Retail keeps only the IPA-SRA clone, which receives &*layers.
#if defined(__i386__)
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone, regparm(1)))
#else
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone))
#endif

i32 GetDefaultIdle(GameObject_s *obj);

void GameAudio_PlaySfx(i32, nuvec_s *, i32, i32);

// ==== src/legoapi/props/objects/tubes.cpp: all remaining content ====

extern "C" void AddVariableShotDebrisEffectTimed1(i32 effect, NUVEC *position, i32 count, f32 time, i16 z_rotation,
                                                  i16 y_rotation, NUMTX *orientation);

// ==== src/legoapi/misc/supportall.cpp: declarations ====

#if defined(__SSE__)
#include <xmmintrin.h>
#endif
#include "legoapi/misc/supportall.h"
#include "legoapi/render/core/screen.h"
#include <stdarg.h>
#include <limits.h>
#include "legoapi/gizmo/base/gizflow.h"

// The reference defines these with dynamic initialization in this unit.
extern MemoryManager theMemoryManager;
#include "legoapi/misc/supportall_types.h"

DECOMP_ASSERT(sizeof(TIMINGBAR_LABEL) == 12, "Timing-bar label size");

// Debug-capture output helpers consumed by NuDisplayListCaptureSortPriority.
// Transcribed from the original C-linkage symbols:
//   NuHtmlBegin    0x2d5ca0   NuHtmlFlush   0x2d5c30
//   NuHtmlWrite    0x2d5cd0   NuHtmlHeading1 0x2d5d40

extern "C" void NuHtmlWrite(const char *text, ...);

extern "C" void NuHtmlHeading1(const char *fmt, ...);

// ==== src/legoapi/items/objects/gameobjects.cpp: declarations ====

extern "C" {
    extern NUVEC plr_lastpos;
    extern i16 id_GRIEVOUS;
    extern i16 id_BODYGUARD;
    extern i16 id_IMPERIALGUARD;
    extern i16 id_JAWA;
    extern i16 id_UGNAUGHT;
    extern i16 id_ATAT;
    extern i16 id_GONKDROID;
    extern i16 id_LANDSPEEDER, id_MILLENNIUMFALCON;
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisPosOrientationMtx(i32, NUMTX *);
    void DebFreeInstantly(i32 *);
    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// ==== src/legoapi/misc/memory.cpp: all remaining content ====

extern "C" {

void DebFreeInstantly(i32 *handle);

} // extern "C"

// ==== functions in original address order ====

// ==== functions in original address order ====

// ==== functions in original address order ====

// ==== src/legoapi/characters/core/playeritems.cpp: declarations ====

void ResetCharacterIdle(GameObject_s *object, i32 mode, i32 animation);
i32 (*Fighting_WeaponInActionFn)(GameObject_s *) = NULL;
i32 (*Fighting_WeaponOutActionFn)(GameObject_s *) = NULL;

static inline void FastWeaponInSfx(GameObject_s *object) {
    if (object->weapon_scale_state == WEAPON_SCALE_RETRACTING)
        return;
    const i32 current_animation = CurrentAnim(&object->apiobj.anim_packet);
    if (current_animation != -1) {
        CHARACTERANIM_s *animation =
            static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[current_animation]);
        if (animation != NULL && (animation->flags & CHARACTER_ANIMATION_FLAG_GUN_ON) != 0)
            return;
    }
    const u32 model_flags = object->apiobj.character_data->model_flags;
    if ((model_flags & CHARACTER_MODEL_FLAG_JEDI) != 0) {
        if (object->apiobj.field_0x27c != -1 || WeaponInOut_NoAIJediSfx == 0) {
            GameAudio_PlaySfx(0x3d, &object->apiobj.collision_position, GameAudio_GetPlrSfxBits(object), 1);
        }
    } else if ((model_flags & CHARACTER_MODEL_FLAG_ALTERNATE_WEAPON) != 0) {
        GameAudio_PlaySfx(0x42, &object->apiobj.collision_position, 0, 1);
    }
}

void FastWeaponIn(GameObject_s *object, i32 force_sound);

static inline void FastWeaponOutSfx(GameObject_s *object) {
    if (object->weapon_scale_state == WEAPON_SCALE_EXTENDING)
        return;
    const i32 current_animation = CurrentAnim(&object->apiobj.anim_packet);
    if (current_animation != -1) {
        CHARACTERANIM_s *animation =
            static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[current_animation]);
        if (animation != NULL && (animation->flags & CHARACTER_ANIMATION_FLAG_GUN_OFF) != 0)
            return;
    }
    const u32 model_flags = object->apiobj.character_data->model_flags;
    if ((model_flags & CHARACTER_MODEL_FLAG_JEDI) != 0) {
        if (object->apiobj.field_0x27c != -1 || WeaponInOut_NoAIJediSfx == 0) {
            GameAudio_PlaySfx(0x3e, &object->apiobj.collision_position, GameAudio_GetPlrSfxBits(object), 1);
        }
    } else if ((model_flags & CHARACTER_MODEL_FLAG_ALTERNATE_WEAPON) != 0) {
        GameAudio_PlaySfx(0x43, &object->apiobj.collision_position, 0, 1);
    }
}

void FastWeaponOut(GameObject_s *object, i32 force_sound);

// ---- Player start spawn entries ----

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

namespace {

} // namespace

// ==== src/legoapi/characters/core/character.cpp: declarations ====

// Forward declarations for local (static) character/gameplay helper stubs.

extern "C" {
extern i16 id_IMPERIALGUARD;
extern i16 id_BODYGUARD;
extern i16 id_ATAT;
extern i16 id_GRIEVOUS;
extern i16 id_MILLENNIUMFALCON;
extern i16 id_UGNAUGHT;
extern i16 id_GONKDROID;
extern i16 id_JAWA;

}

// Original 0x120-byte data record @0x00666960. FixUpCharacters copies this
// canonical prefix into every character before character-specific overrides.
// These values cover the fully typed common movement and perception fields.

static __used__ void ExtraDieSfx_LSW(GameObject_s *object);

static __used__ void ExtraHurtSfx_LSW(GameObject_s *object);

// ==== src/legoapi/characters/core/charconfig.cpp: declarations ====

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

extern CHARCONFIG_s charconfig;

// Retail only keeps the clone with strip_comments fixed to 1 (regparm text, filename).
#if defined(__i386__)
#define REDIRECT_TEXT_FILE_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define REDIRECT_TEXT_FILE_ATTR __attribute__((noinline, noclone))
#endif

// ==== src/legoapi/characters/core/characters.cpp: declarations ====

extern "C" {
    i32 NuFileExists(char *);

}

void LoadPerm1();

// ==== src/legoapi/menus/screens/gamemenuall.cpp: declarations ====

#include "gameapi/gui/apimenu_internal.h"
#include "legoapi/core/config/fileselect.h"
#include "nu2api/nu3d/nugscn.h"

// Retail clears these "fast repeat" latches but never sets them.

// ==== src/legoapi/menus/screens/menus.cpp: declarations ====
#include "gamelib/crc/crc.h"

#include "legoapi/render/light/fade.h"

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

extern "C" {
    extern i16 id_ATAT;
}

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

// ==== src/gameapi/gui/apimenu.cpp: all remaining content ====

#include "legoapi/core/startup/main.h"
#include "legoapi/menus/screens/movies.h"
#include "nu2api/nucore/nuvideo.h"

void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 volume, i32 flags);

extern "C" void SmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment,
                            u8 red, u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box,
                            i32 suppress_draw, u32 alpha);

// ==== src/legoapi/core/input/gamepads.cpp: declarations ====
#include "legoapi/misc/androidbatman.h"
#include "nu2api/nucore/NuInputDevice.h"

// Original bss @0x127a500: 64 pads x 0x60 bytes.
// Original bss @0x127a4e0.

extern u32 GAMEPAD_JUMP;

// Two frames of direction history for each local player.  MovePlayer shifts
// these before accepting the new input; GamePad_Rotate then reports a stable
// angular velocity only when both consecutive turns agree.

f32 GamePad_Rotate(GameObject_s *object);

u16 GamePad_InputAngle(GameObject_s *object, GAMEPAD_s *pad);

extern "C" f32 NuFsqrt(f32);

// ==== src/gameframework/savesystem.cpp: all remaining content ====

// ==== src/legoapi/core/config/saveload.cpp: declarations ====

#include "gameapi/edtools/gameapi_edtools_types.h"

extern u8 FS_Active;

// Retail keeps the whole body of RenderFileSel3 as the split-out .part clone;
// the exported function only tests FS_Active.

void RenderFileSel3(i32 draw_title);

// ==== src/legoapi/menus/core/text.cpp: declarations ====
#include "nu2api/nu3d/nushader_plain.h"

// Original .data @0x667ce0; the names are filled in by the static initializer.

extern "C" {

void SmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                     u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw, u32 alpha);
}

// ==== src/gameapi/edtools/edpartall.cpp: all remaining content ====

extern "C" {
    void eduiMenuRender(eduimenu_s *menu);
}

// Retail only keeps the IPA-SRA clone, which takes just the parent menu.

// ==== src/legoapi/render/core/terrain_stubs.cpp: declarations ====

extern "C" {
    void DebFreeInstantly(i32 *);
}

extern "C" {

void DebrisPosOrientationMtx(i32 handle, NUMTX *source);

} // extern "C"

// ==== src/legoapi/render/fx/parts.cpp: declarations ====

extern "C" {
    void DebFreeInstantly(i32 *handle);

    void SetSfxBit_On(i32);
    void PlaySfxById(i32, NUVEC *);

    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
}

extern "C" {

void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);

    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

PART_s *AddPart(ADDPART_s *params);

    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);

void AddVariableShotDebrisEffect(i32 effect, NUVEC *position, i32 count, i16 z_rotation, i16 y_rotation);

void AddVariableShotDebrisEffectTimed1(i32 effect, NUVEC *position, i32 count, f32 time, i16 z_rotation,
                                           i16 y_rotation, NUMTX *particle_orientation);

    void DebFreeInstantly(i32 *);

} // extern "C"

#define PART_EMIT_SOUND_ON(slot)                                                                                       \
    if (type->sounds[slot] != -1) {                                                                                    \
        SetSfxBit_On(type->sounds[slot]);                                                                              \
        if (type->sound_modes[slot] == 4)                                                                              \
            PlaySfxById(type->sounds[slot], &emitter->position);                                                       \
    }
#define PART_EMIT_SOUND(slot, mode)                                                                                    \

// ==== src/gameapi/edtools/edtoolsall_plain.cpp: all remaining content ====
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "gameapi/edtools/edui_colour.h"
#include "gameapi/edtools/edgra_internal.h"
#include "nu2api/nu3d/android/nuobject_android.h"

// Retail keeps an unreferenced partial-inlining clone of the old HSV2RGB
// helper (the non-grey path) in this unit.
#if defined(__i386__)
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone, regparm(2), sseregparm))
#else
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone))
#endif

// The original animation editor callbacks and lifecycle share file-local state.
// ---- begin src/gameapi/edtools/edanimall.cpp ----

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

// ---- end src/gameapi/edtools/edanimall.cpp ----
// ---- begin src/gameapi/edtools/edmenucallbacks.cpp ----
#include <cstdio>

// Editor UI callbacks retained in the original editor's menu code.

// ---- end src/gameapi/edtools/edmenucallbacks.cpp ----

// ---- begin src/gameapi/edtools/edptlall.cpp ----

extern "C" {
    void DebFreeInstantly(i32 *handle);
}

// The original particle editor's private callbacks and menu state share this TU.
// ---- begin src/gameapi/edtools/edptlcallbacks.cpp ----

extern "C" {
    void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);
}

// Particle-list editor UI/menu callbacks.

// Particle editor UI/menu callbacks.

// ---- end src/gameapi/edtools/edptlcallbacks.cpp ----

// Particle list editor subsystem stubs (static, internal linkage).

// ---- end src/gameapi/edtools/edptlall.cpp ----

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

// Only caller prints the property edit buffer left-aligned.

static __used__ void eduiFntPrintClipEx(void *font, float x, float y, int alignment, float clip_x, float clip_width,
                                        char *format, ...);
// Only eduiMenuRender calls the integer clip helper, always left-aligned.

#include "gameapi/edtools/edtools_types.h"

extern "C" {
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
}

extern "C" {

    // Shared editor font; edmainInit receives and stores the font handle.

void edmainInit(void *font, char *configuration_file);

void eduiMenuRender(eduimenu_s *menu);

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

// ==== src/gameapi/edtools/edtoolsall.cpp: declarations ====

#include "nu2api/nucore/NuDynamicLight.h"
#include <new>

extern EdInputContext * Input;
extern "C" void PlaySfxById(i32, nuvec_s *);

DECOMP_ASSERT(sizeof(EdManipulator) == 0x6c, "EdManipulator ABI");

extern "C" {
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
}

extern "C" {
    void DebFreeInstantly(i32 *);
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

// ---- begin src/gameapi/edtools/edmenucallbacks.cpp ----
#include <cstdio>


// ---- end src/gameapi/edtools/edmenucallbacks.cpp ----

// ---- begin src/gameapi/edtools/edptlall.cpp ----

extern "C" {
    void DebFreeInstantly(i32 *handle);
}

// ---- begin src/gameapi/edtools/edptlcallbacks.cpp ----

extern "C" {
    void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);
}

// ---- end src/gameapi/edtools/edptlcallbacks.cpp ----

// ---- end src/gameapi/edtools/edptlall.cpp ----

extern "C" {
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
}

extern "C" {

void edmainInit(void *font, char *configuration_file);

void eduiMenuRender(eduimenu_s *menu);

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

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
// Temporary C-linkage compatibility surface for unresolved engine owners.
#include "nu2api/nucore/nuonline.h"
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.

#include <GLES2/gl2.h>
#include "nu2api/nucore/numem.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"

#include "java/android.h"
#include "java/java.h"
#include "nu2api/nucore/NuInputManager.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nuscreen.hpp"

// C++-linkage helpers defined in sibling TUs.

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

void NuCameraSetEx(NUCAMERA *cam, i32 fast);

    // ---------------------------------------------------------------------------
    // Display list — faithful helpers + pending stubs
    // ---------------------------------------------------------------------------

} // extern "C"

// Local helpers matching original static display-list setters (t local symbols)

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

union variptr_u;

// ==== src/gameapi/edtools/edui.cpp: all remaining content ====

// ==== src/gameapi/edtools/edfile.cpp: all remaining content ====

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

extern "C" {
    extern i16 id_ATAT;
}

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

// ==== functions in original address order ====

extern "C" {

void eduiMenuRender(eduimenu_s *menu);

}

extern "C" {

void edmainInit(void *font, char *configuration_file);

}

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
//

extern "C" {

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

void NuCameraSetEx(NUCAMERA *cam, i32 fast);

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

// ==== functions in original address order ====

extern "C" {

PART_s *AddPart(ADDPART_s *params);

}

// ==== functions in original address order ====

extern "C" {
void SmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                     u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw, u32 alpha);

}

// ==== src/legoapi/render/core/render.cpp: declarations ====

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

extern "C" {
    extern i16 id_ATAT;
}

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

static __used__ i32 PauseRenderOff();

static __used__ i32 MatrixReflection_CanOverride();

static __used__ void BackDrop_Alpha(float *alpha);

// ==== functions in original address order ====

// ==== src/legoapi/items/objects/gameobjects.cpp: declarations ====

extern "C" {
    extern NUVEC plr_lastpos;
    extern i16 id_GRIEVOUS;
    extern i16 id_BODYGUARD;
    extern i16 id_IMPERIALGUARD;
    extern i16 id_JAWA;
    extern i16 id_UGNAUGHT;
    extern i16 id_ATAT;
    extern i16 id_GONKDROID;
    extern i16 id_LANDSPEEDER, id_MILLENNIUMFALCON;
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisPosOrientationMtx(i32, NUMTX *);
    void DebFreeInstantly(i32 *);
    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

static __used__ void LEGO_100PercentFn();
static __used__ void LEGO_AllGoldBricksFn();

// ==== src/editor/edtoolsall.cpp: all remaining content ====

// ==== functions in original address order ====

// ==== functions in original address order ====

// ==== functions in original address order ====

// ==== src/legoapi/actions/movement/pushblocks.cpp: declarations ====
#include "legoapi/actions/movement/pushblocks.h"

#include "legoapi/actions/movement/pushblocks_types.h"

// ==== src/legoapi/audio/sfx.cpp: declarations ====

#include "nu2api/nusound/nusound.h"

#include "legoapi/audio/sfx_types.h"

DECOMP_ASSERT(sizeof(RepeatSfx) == 0x10, "RepeatSfx size");

void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 flags, i32 volume);

extern "C" {
    void SetSfxBit_On(i32 sound);
}

extern "C" {

void PlaySfxById(i32 sfx_id, nuvec_s *position);

void SetSfxBit_On(i32 sound);

} // extern "C"

i32 GameAudio_GetPlrSfxBits(void *object_ptr);

// ==== src/legogame/game.cpp: declarations ====

// Retail data @0x617200: common animations shared by the old and new characters.

// ==== src/legoapi/items/objects/gameobjects.cpp: declarations ====

extern "C" {
    extern NUVEC plr_lastpos;
    extern i16 id_GRIEVOUS;
    extern i16 id_BODYGUARD;
    extern i16 id_IMPERIALGUARD;
    extern i16 id_JAWA;
    extern i16 id_UGNAUGHT;
    extern i16 id_ATAT;
    extern i16 id_GONKDROID;
    extern i16 id_LANDSPEEDER, id_MILLENNIUMFALCON;
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisPosOrientationMtx(i32, NUMTX *);
    void DebFreeInstantly(i32 *);
    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

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

extern "C" {
    extern i16 id_ATAT;
}

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

// ==== src/legoapi/menus/core/gamehint.cpp: declarations ====

extern f32 ICONXPOS_TOUCHYFEELY;
extern f32 ICONYPOS_TOUCHYFEELY;
f32 SeekLinearF(f32, f32, f32);

void MechHintUIButton_OnClick_Callback(MechTouchUIElement &, TouchHolder &);

#include "legoapi/menus/core/gamehint_types.h"
DECOMP_ASSERT(sizeof(MechHintUIButton) == 0xa8, "MechHintUIButton size");
DECOMP_ASSERT(offsetof(MechHintUIButton, pending_hint) == 0x7c, "Pending touch hint offset");
DECOMP_ASSERT(offsetof(MechHintUIButton, display_elapsed) == 0x80, "Touch hint display timer offset");

void MechHintUIButton_OnClick_Callback(MechTouchUIElement &element, TouchHolder &);

// Static game message and hint helpers. Stubbed to satisfy the symbol baseline.

// ==== src/legoapi/items/base/game_object.cpp: declarations ====

#include <new>

// Forward declarations for local (static) game-object helper stubs.
struct GameObject_s;
struct WORLDINFO_s;
struct BOLT_s;

extern "C" {
    extern i16 id_IMPERIALGUARD;
}

// ==== src/legoapi/characters/core/character.cpp: declarations ====

extern "C" {
extern i16 id_IMPERIALGUARD;
extern i16 id_BODYGUARD;
extern i16 id_ATAT;
extern i16 id_GRIEVOUS;
extern i16 id_MILLENNIUMFALCON;
extern i16 id_UGNAUGHT;
extern i16 id_GONKDROID;
extern i16 id_JAWA;

}

// ==== src/legoapi/cutscenes/cutscene.cpp: declarations ====
#include "legoapi/cutscenes/cutscene_defrag.h"

// --- Extern "C" block: functions with confirmed C linkage in original libTTapp.so ---

#include "legoapi/cutscenes/cutscene_types.h"
DECOMP_ASSERT(sizeof(CutSceneTriggerNameEntry) == 0x34, "cutscene trigger name entry stride");
DECOMP_ASSERT(offsetof(CutSceneTriggerNameTable, entries) == 0x0c, "cutscene trigger name table entries offset");

// Exported (default visibility) in the original library.

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

extern "C" void DebFreeInstantly(i32 *handle);

// Retail only keeps the IPA-SRA clone, which drops the unused frame argument.
#if defined(__i386__)
#define INST_LOCATOR_SYS_END_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define INST_LOCATOR_SYS_END_ATTR __attribute__((noinline, noclone))
#endif

static __used__ void instNuGCutTriggerSysUpdate(instNUGCUTSCENE_s *instance, float frame);

// ==== src/legoapi/items/objects/gameobjects.cpp: declarations ====

extern "C" {
    extern NUVEC plr_lastpos;
    extern i16 id_GRIEVOUS;
    extern i16 id_BODYGUARD;
    extern i16 id_IMPERIALGUARD;
    extern i16 id_JAWA;
    extern i16 id_UGNAUGHT;
    extern i16 id_ATAT;
    extern i16 id_GONKDROID;
    extern i16 id_LANDSPEEDER, id_MILLENNIUMFALCON;
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisPosOrientationMtx(i32, NUMTX *);
    void DebFreeInstantly(i32 *);
    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);
}

// ==== src/legoapi/gizmos/trigger/ai.cpp: all remaining content ====

// ==== src/legoapi/ai/game/creature.cpp: declarations ====

extern "C" {
    extern NUVEC plr_lastpos;
    void ResetAnimPacket(ANIMPACKET_s *packet, i16 animation);
}

// ==== src/gameapi/ai/aisys/aiscript.cpp: all remaining content ====

// ==== src/legoapi/ai/core/ai_sys_stubs.cpp: declarations ====

extern "C" {

extern AIEDITOR_RENDER_STATE * aieditor;

} // extern "C"

// ==== src/editor/area_editor.cpp: all remaining content ====

// ==== src/editor/edpath.cpp: all remaining content ====

// ==== src/editor/aieditor.cpp: all remaining content ====

// ==== src/editor/creature_editor.cpp: all remaining content ====


// ==== src/editor/locator_editor.cpp: all remaining content ====

extern "C" {
    struct EDCREATURE_s;
}

#if defined(__i386__)
#else
#endif

// ==== src/legoapi/items/objects/gameobjects.cpp: declarations ====

extern "C" {
    extern NUVEC plr_lastpos;
    extern i16 id_GRIEVOUS;
    extern i16 id_BODYGUARD;
    extern i16 id_IMPERIALGUARD;
    extern i16 id_JAWA;
    extern i16 id_UGNAUGHT;
    extern i16 id_ATAT;
    extern i16 id_GONKDROID;
    extern i16 id_LANDSPEEDER, id_MILLENNIUMFALCON;
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisPosOrientationMtx(i32, NUMTX *);
    void DebFreeInstantly(i32 *);
    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// ==== src/gameapi/ai/aisys/aistate.cpp: all remaining content ====

// ==== src/legoapi/gizmo/gizmos/gizmos_gizactions.cpp: declarations ====

namespace {

} // namespace

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

extern "C" {
    extern i16 id_ATAT;
}

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

// ==== src/editor/antinode_editor.cpp: all remaining content ====

// ==== src/legoapi/render/core/terrain_stubs.cpp: declarations ====

extern "C" {
    void DebFreeInstantly(i32 *);
}

extern "C" {

void DebrisPosOrientationMtx(i32 handle, NUMTX *source);

} // extern "C"

// ==== functions in original address order ====

// ==== functions in original address order ====

// ==== src/gameapi/edtools/edpartall.cpp: all remaining content ====

extern "C" {
    void eduiMenuRender(eduimenu_s *menu);
}

// ==== src/legoapi/render/core/terrain_stubs.cpp: declarations ====

extern "C" {
    void DebFreeInstantly(i32 *);
}

extern "C" {

void DebrisPosOrientationMtx(i32 handle, NUMTX *source);

} // extern "C"

// ==== src/legoapi/render/fx/parts.cpp: declarations ====

extern "C" {
    void DebFreeInstantly(i32 *handle);

    void SetSfxBit_On(i32);
    void PlaySfxById(i32, NUVEC *);

    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
}

extern "C" {

void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);

    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

PART_s *AddPart(ADDPART_s *params);

    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);

void AddVariableShotDebrisEffect(i32 effect, NUVEC *position, i32 count, i16 z_rotation, i16 y_rotation);

void AddVariableShotDebrisEffectTimed1(i32 effect, NUVEC *position, i32 count, f32 time, i16 z_rotation,
                                           i16 y_rotation, NUMTX *particle_orientation);

    void DebFreeInstantly(i32 *);

} // extern "C"

#define PART_EMIT_SOUND_ON(slot)                                                                                       \
    if (type->sounds[slot] != -1) {                                                                                    \
        SetSfxBit_On(type->sounds[slot]);                                                                              \
        if (type->sound_modes[slot] == 4)                                                                              \
            PlaySfxById(type->sounds[slot], &emitter->position);                                                       \
    }
#define PART_EMIT_SOUND(slot, mode)                                                                                    \

// ==== src/gameapi/edtools/edtoolsall_plain.cpp: all remaining content ====

// ---- begin src/gameapi/edtools/edmenucallbacks.cpp ----
#include <cstdio>


// ---- end src/gameapi/edtools/edmenucallbacks.cpp ----

// ---- begin src/gameapi/edtools/edptlall.cpp ----

extern "C" {
    void DebFreeInstantly(i32 *handle);
}

// ---- begin src/gameapi/edtools/edptlcallbacks.cpp ----

extern "C" {
    void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);
}

// ---- end src/gameapi/edtools/edptlcallbacks.cpp ----

// ---- end src/gameapi/edtools/edptlall.cpp ----

extern "C" {
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
}

extern "C" {

void edmainInit(void *font, char *configuration_file);

void eduiMenuRender(eduimenu_s *menu);

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

// ==== src/gameapi/edtools/edtoolsall.cpp: declarations ====

#include <new>

extern "C" {
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
}

extern "C" {
    void DebFreeInstantly(i32 *);
}

// ---- begin src/gameapi/edtools/edmenucallbacks.cpp ----
#include <cstdio>


// ---- end src/gameapi/edtools/edmenucallbacks.cpp ----

// ---- begin src/gameapi/edtools/edptlall.cpp ----

extern "C" {
    void DebFreeInstantly(i32 *handle);
}

// ---- begin src/gameapi/edtools/edptlcallbacks.cpp ----

extern "C" {
    void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);
}

// ---- end src/gameapi/edtools/edptlcallbacks.cpp ----

// ---- end src/gameapi/edtools/edptlall.cpp ----

extern "C" {
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
}

extern "C" {

void edmainInit(void *font, char *configuration_file);

void eduiMenuRender(eduimenu_s *menu);

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

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
//

extern "C" {

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

void NuCameraSetEx(NUCAMERA *cam, i32 fast);

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

// ==== src/gameapi/edtools/edui.cpp: all remaining content ====

// ==== src/gameapi/edtools/edfile.cpp: all remaining content ====

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

extern "C" {
    extern i16 id_ATAT;
}

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

// ==== functions in original address order ====

extern "C" {

void eduiMenuRender(eduimenu_s *menu);

}

extern "C" {

void edmainInit(void *font, char *configuration_file);

}

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
//

extern "C" {

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

void NuCameraSetEx(NUCAMERA *cam, i32 fast);

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

// ==== functions in original address order ====

extern "C" {

PART_s *AddPart(ADDPART_s *params);

}

// ==== src/legoapi/characters/core/characters.cpp: declarations ====

extern "C" {
    i32 NuFileExists(char *);

}

// ==== src/legoapi/core/startup/game.cpp: declarations ====

extern void GameAudio_PlaySfx(i32, nuvec_s *, i32, i32);

namespace {

} // namespace

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

extern "C" {
    extern i16 id_ATAT;
}

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

// ==== src/gameapi/gui/apimenu.cpp: all remaining content ====

// ==== src/gameframework/savesystem.cpp: all remaining content ====

// ==== src/legoapi/menus/core/text.cpp: declarations ====

extern "C" {

void SmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                     u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw, u32 alpha);
}

// ==== src/gameapi/edtools/edpartall.cpp: all remaining content ====

extern "C" {
    void eduiMenuRender(eduimenu_s *menu);
}

// ==== src/legoapi/render/core/terrain_stubs.cpp: declarations ====

extern "C" {
    void DebFreeInstantly(i32 *);
}

extern "C" {

void DebrisPosOrientationMtx(i32 handle, NUMTX *source);

} // extern "C"

// ==== src/legoapi/render/fx/parts.cpp: declarations ====

extern "C" {
    void DebFreeInstantly(i32 *handle);

    void SetSfxBit_On(i32);
    void PlaySfxById(i32, NUVEC *);

    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
}

extern "C" {

void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);

    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

PART_s *AddPart(ADDPART_s *params);

    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);

void AddVariableShotDebrisEffect(i32 effect, NUVEC *position, i32 count, i16 z_rotation, i16 y_rotation);

void AddVariableShotDebrisEffectTimed1(i32 effect, NUVEC *position, i32 count, f32 time, i16 z_rotation,
                                           i16 y_rotation, NUMTX *particle_orientation);

    void DebFreeInstantly(i32 *);

} // extern "C"

#define PART_EMIT_SOUND_ON(slot)                                                                                       \
    if (type->sounds[slot] != -1) {                                                                                    \
        SetSfxBit_On(type->sounds[slot]);                                                                              \
        if (type->sound_modes[slot] == 4)                                                                              \
            PlaySfxById(type->sounds[slot], &emitter->position);                                                       \
    }
#define PART_EMIT_SOUND(slot, mode)                                                                                    \

// ==== src/gameapi/edtools/edtoolsall_plain.cpp: all remaining content ====

// ---- begin src/gameapi/edtools/edmenucallbacks.cpp ----
#include <cstdio>


// ---- end src/gameapi/edtools/edmenucallbacks.cpp ----

// ---- begin src/gameapi/edtools/edptlall.cpp ----

extern "C" {
    void DebFreeInstantly(i32 *handle);
}

// ---- begin src/gameapi/edtools/edptlcallbacks.cpp ----

extern "C" {
    void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);
}

// ---- end src/gameapi/edtools/edptlcallbacks.cpp ----

// ---- end src/gameapi/edtools/edptlall.cpp ----

extern "C" {
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
}

extern "C" {

void edmainInit(void *font, char *configuration_file);

void eduiMenuRender(eduimenu_s *menu);

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

// ==== src/gameapi/edtools/edtoolsall.cpp: declarations ====

#include <new>

extern "C" {
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
}

extern "C" {
    void DebFreeInstantly(i32 *);
}

// ---- begin src/gameapi/edtools/edmenucallbacks.cpp ----
#include <cstdio>


// ---- end src/gameapi/edtools/edmenucallbacks.cpp ----

// ---- begin src/gameapi/edtools/edptlall.cpp ----

extern "C" {
    void DebFreeInstantly(i32 *handle);
}

// ---- begin src/gameapi/edtools/edptlcallbacks.cpp ----

extern "C" {
    void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);
}

// ---- end src/gameapi/edtools/edptlcallbacks.cpp ----

// ---- end src/gameapi/edtools/edptlall.cpp ----

extern "C" {
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
}

extern "C" {

void edmainInit(void *font, char *configuration_file);

void eduiMenuRender(eduimenu_s *menu);

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

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
//

extern "C" {

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

void NuCameraSetEx(NUCAMERA *cam, i32 fast);

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

// ==== src/gameapi/edtools/edui.cpp: all remaining content ====

// ==== src/gameapi/edtools/edfile.cpp: all remaining content ====

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

extern "C" {
    extern i16 id_ATAT;
}

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

// ==== functions in original address order ====

extern "C" {

void eduiMenuRender(eduimenu_s *menu);

}

extern "C" {

void edmainInit(void *font, char *configuration_file);

}

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
//

extern "C" {

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

void NuCameraSetEx(NUCAMERA *cam, i32 fast);

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

// ==== functions in original address order ====

extern "C" {

PART_s *AddPart(ADDPART_s *params);

}

// ==== functions in original address order ====

extern "C" {
void SmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                     u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw, u32 alpha);

}

// ==== src/legoapi/menus/screens/gamestatus_lsw.cpp: declarations ====

#include "legoapi/menus/screens/gamestatus_lsw.h"

void LevelComplete_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32);

// Retail keeps the active-stage drawing as the split-out .part clone of
// LevelComplete_LSW_Draw; the inline head only tests `active`.

void LevelComplete_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active);

// ==== src/legoapi/menus/screens/shop.cpp: declarations ====

// Shop state is file-local, matching the original _ZL symbols.

// ==== functions in original address order ====

// ==== src/legoapi/items/objects/gameobjects.cpp: declarations ====

extern "C" {
    extern NUVEC plr_lastpos;
    extern i16 id_GRIEVOUS;
    extern i16 id_BODYGUARD;
    extern i16 id_IMPERIALGUARD;
    extern i16 id_JAWA;
    extern i16 id_UGNAUGHT;
    extern i16 id_ATAT;
    extern i16 id_GONKDROID;
    extern i16 id_LANDSPEEDER, id_MILLENNIUMFALCON;
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisPosOrientationMtx(i32, NUMTX *);
    void DebFreeInstantly(i32 *);
    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// ==== src/legoapi/misc/memory.cpp: all remaining content ====

extern "C" {

void DebFreeInstantly(i32 *handle);

} // extern "C"

// ==== functions in original address order ====

// ==== src/legoapi/render/fx/parts.cpp: declarations ====

extern "C" {
    void DebFreeInstantly(i32 *handle);

    void SetSfxBit_On(i32);
    void PlaySfxById(i32, NUVEC *);

    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
}

extern "C" {

void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);

    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

PART_s *AddPart(ADDPART_s *params);

    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);

void AddVariableShotDebrisEffect(i32 effect, NUVEC *position, i32 count, i16 z_rotation, i16 y_rotation);

void AddVariableShotDebrisEffectTimed1(i32 effect, NUVEC *position, i32 count, f32 time, i16 z_rotation,
                                           i16 y_rotation, NUMTX *particle_orientation);

    void DebFreeInstantly(i32 *);

} // extern "C"

#define PART_EMIT_SOUND_ON(slot)                                                                                       \
    if (type->sounds[slot] != -1) {                                                                                    \
        SetSfxBit_On(type->sounds[slot]);                                                                              \
        if (type->sound_modes[slot] == 4)                                                                              \
            PlaySfxById(type->sounds[slot], &emitter->position);                                                       \
    }
#define PART_EMIT_SOUND(slot, mode)                                                                                    \

// ==== src/legoapi/items/objects/gameobjects.cpp: declarations ====

extern "C" {
    extern NUVEC plr_lastpos;
    extern i16 id_GRIEVOUS;
    extern i16 id_BODYGUARD;
    extern i16 id_IMPERIALGUARD;
    extern i16 id_JAWA;
    extern i16 id_UGNAUGHT;
    extern i16 id_ATAT;
    extern i16 id_GONKDROID;
    extern i16 id_LANDSPEEDER, id_MILLENNIUMFALCON;
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisPosOrientationMtx(i32, NUMTX *);
    void DebFreeInstantly(i32 *);
    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// ==== src/legoapi/world/areas.cpp: declarations ====
#include "legoapi/items/collect/minikits.h"
#include "legoapi/render/fx/particles.h"

// ===========================================================================
// ===========================================================================

// ===========================================================================
// ===========================================================================

// ===========================================================================
// ===========================================================================

// ==== src/legoapi/cutscenes/cutscene.cpp: declarations ====

#define NUGCUT_RELOCATE(field, delta) \
    (NUGCUT_WORD(field) = NUGCUT_WORD(field) != 0 ? NUGCUT_WORD(field) + (delta) : 0)

// ==== src/legoapi/cutscenes/cutscene_stubs.cpp: declarations ====

#include <cfloat>

extern "C" {

    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisPosOrientationMtx(i32, NUMTX *);
    void DebFreeInstantly(i32 *);
}

// Steers a cutscene character's eyes towards a look-at locator and blinks its eyelids.
// Only version-3 locator animations carry the look and blink channels.
// Retail only keeps the IPA-SRA clone: the system is dropped and the instance
// locator is replaced by its effect handle slot.
#if defined(__i386__)
#define INST_LOOKAT_UPDATE_ATTR __attribute__((noinline, noclone, regparm(2), sseregparm))
#else
#define INST_LOOKAT_UPDATE_ATTR __attribute__((noinline, noclone))
#endif

// ==== src/nu2api/nusound/nu2api_nusound_plain.cpp: all remaining content ====

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
//

extern "C" {

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------

void NuCameraSetEx(NUCAMERA *cam, i32 fast);

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

// ==== src/nu2api/nucore/nugcutscene_anim.cpp: all remaining content ====

// ==== src/legoapi/ai/core/ai_sys_stubs.cpp: all remaining content ====

extern "C" {

extern AIEDITOR_RENDER_STATE * aieditor;

} // extern "C"

// ==== src/nu2api/nucore/nugcutscene.cpp: all remaining content ====

// Callback storage is declared in reverse linked-BSS order because the
// original GCC 4.7 -O3 object emits these definitions in reverse source order.

// ==== src/legoapi/cutscenes/cutscenes.cpp: declarations ====

extern "C" {
    void PlaySfxById(i32 sfx_id, nuvec_s *position);
}

// Retail only keeps the split-out body of the IPA-SRA clone (the unused
// cutscene index dropped), called with world and cut in registers.
#if defined(__i386__)
#define CUTSCENE_START_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define CUTSCENE_START_ATTR __attribute__((noinline, noclone))
#endif

// The loop re-reads the active cut through CutStopInfo after each SmartTextEx call.
#define CUT_STOP static_cast<CUTINFO *>(CutStopInfo)

// ==== functions in original address order ====

// ==== src/legoapi/cutscenes/cutscenes.cpp: declarations ====

extern "C" {
    void PlaySfxById(i32 sfx_id, nuvec_s *position);
}

// ==== src/legoapi/items/collect/bolts.cpp: declarations ====
#include "legoapi/characters/motion/chris.h"

// Configuration entry point and parser callbacks share the original BT state.

// Retail only has the IPA-SRA clone of Bolt_HitPlat(BOLT_s *, u8 *, WORLDINFO_s *), which drops the
// unused world argument.
static __attribute__((noinline, noclone)) i32 Bolt_HitPlat(BOLT_s *bolt, u8 *hit_flags)
    __asm__("_ZL12Bolt_HitPlatP6BOLT_sPhP11WORLDINFO_s.isra.38");

// The original caller appears to treat this u16-returning function as i32;
// correcting this declaration lowers its match.

// ==== src/legoapi/gizmos/object/gizobstacle.cpp: declarations ====

#include "legoapi/actions/combat/shovesys.h"

namespace {

#include "legoapi/gizmos/object/gizobstacle_types.h"

    DECOMP_ASSERT(sizeof(GIZOBSTACLEPROGRESS_s) == 0x70, "GIZOBSTACLE progress ABI");

} // namespace

namespace {

// Box-trigger and character-flag tests are expanded in place, as in the reference.
#define OBSTACLE_USES_BOX_TRIGGER(obstacle) ((obstacle)->mode == 5 || (obstacle)->mode == 6)
#define OBSTACLE_CHARACTER_FLAGS(object)                                                                               \
    static_cast<const OBSTACLE_CHARACTER_DATA_FLAGS_s *>(static_cast<const void *>((object)->apiobj.character_data))
#define OBSTACLE_CHARACTER_DATA_HAS_FLAG(object, field, flag)                                                          \
    ((OBSTACLE_CHARACTER_FLAGS(object)->field & (flag)) != 0)

} // namespace

// Retail places this in .text.unlikely and never partially inlines it.

// Obstacle modes dispatch through this exact eight-entry target table.

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

extern "C" {
    extern i16 id_ATAT;
}

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

// ==== functions in original address order ====

// ==== src/legoapi/world/levelconfig.cpp: declarations ====

// Game-specific pre-load limits. These must be applied before WorldInfo
// reserves each level's fixed-size gameplay systems.

// ==== functions in original address order ====

// ==== functions in original address order ====

// ==== functions in original address order ====

static __used__ void ClimbObject_FindNormal(CLIMBOBJECT_s *object) {
    AIPATHNODE *second = &object->path->nodes[object->connection->node_indices[1]];
    AIPATHNODE *first = &object->path->nodes[object->connection->node_indices[0]];
    AIPATHNODE *node = second->position.y > first->position.y ? first : second;
    NUVEC displacement, origin;
#define PROBE_CLIMB_NORMAL(angle)                                                                                      \
    do {                                                                                                               \
        displacement.x = node->radius + 0.5f;                                                                          \
        displacement.y = 0.0f;                                                                                         \
        displacement.z = 0.0f;                                                                                         \
        NuVecRotateY(&displacement, &displacement, angle);                                                             \
        origin.x = node->position.x;                                                                                   \
        origin.y = (node->max_height + node->min_height) * 0.5f;                                                       \
        origin.z = node->position.z;                                                                                   \
        if (GameRayCast(&origin, &displacement, 0.0f, TERRAINMASK_NONWEAPON | TERRAINMASK_NONDROID | 0x5f) != 0 &&     \
            (TerSurface[static_cast<i8>(NewRayCastGetImpactTerrainType())].flags & 0x10000) != 0) {                    \
            goto found_normal;                                                                                         \
        }                                                                                                              \
    } while (0)
    PROBE_CLIMB_NORMAL(0);
    PROBE_CLIMB_NORMAL(0x4000);
    PROBE_CLIMB_NORMAL(0x8000);
    PROBE_CLIMB_NORMAL(0xc000);
    PROBE_CLIMB_NORMAL(0x10000);
    PROBE_CLIMB_NORMAL(0x14000);
    PROBE_CLIMB_NORMAL(0x18000);
    PROBE_CLIMB_NORMAL(0x1c000);
#undef PROBE_CLIMB_NORMAL
    return;
found_normal:
    object->flags |= 1;
    object->impact_x = origin.x + displacement.x;
    object->impact_z = origin.z + displacement.z;
    object->normal = ShadNorm;
}

// Retail keeps the tail of LedgeTerrain_Attach split out by partial inlining;
// the contact checks stay inline in each caller.
#if defined(__i386__)
#define LEDGE_ATTACH_PART_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define LEDGE_ATTACH_PART_ATTR __attribute__((noinline, noclone))
#endif
static LEDGE_ATTACH_PART_ATTR i32 LedgeTerrain_AttachPart(GameObject_s *object, u16 facing, NUVEC *position,
                                                          u16 *wall_angle)
    __asm__("_ZL19LedgeTerrain_AttachP12GameObject_stP7nuvec_sPt.part.0");

static inline i32 LedgeTerrain_Attach(GameObject_s *object, u16 facing, NUVEC *position, u16 *wall_angle) {
    if (!object->field_0x1084)
        return 0;
    u8 surface = object->field_0x6b0;
    if (surface < 32 && (TerSurface[surface].flags & 0x581))
        return 0;
    if (object->apiobj.collision_position.y - 0.05f >= object->contact_position.y ||
        fabsf(object->contact_normal.y) > NuTrigTable[0x3000])
        return 0;
    return LedgeTerrain_AttachPart(object, facing, position, wall_angle);
}

static LEDGE_ATTACH_PART_ATTR i32 LedgeTerrain_AttachPart(GameObject_s *object, u16 facing, NUVEC *position,
                                                          u16 *wall_angle) {
    *wall_angle = NuAtan2D(object->contact_normal.x, object->contact_normal.z);
    i32 difference = RotDiff(facing, *wall_angle);
    if (difference < 0)
        difference = -difference;
    if (difference < 0x4000)
        return 0;

    u16 inward_angle = *wall_angle + 0x8000;
    f32 distance = object->apiobj.field_0x1dc + LEDGETERRAINLOOKAHEAD;
    position->x = NU_SIN_LUT(inward_angle) * distance + object->apiobj.position.x;
    position->y = object->apiobj.upper_position.y;
    position->z = NU_COS_LUT(inward_angle) * distance + object->apiobj.position.z;
    f32 height = GameShadow(NULL, position, 5.0f, -1);
    if (height == 2000000.0f)
        return 0;
    if (NuTrigTable[0x2555] > ShadNorm.y)
        return 0;
    f32 height_difference = position->y - height;
    if (height_difference > 0.025f || height_difference < -0.1f)
        return 0;
    if (object->apiobj.field_0x220 != 2000000.0f && height > object->apiobj.field_0x220)
        return 0;
    position->y = height;
    return 1;
}

void LedgeTerrain_MoveCode(GameObject_s *object) {
    if (LEGOCONTEXT_LEDGETERRAIN == -1)
        return;
    NUVEC position;
    u16 wall_angle;
    if (object->character_context == LEGOCONTEXT_LEDGETERRAIN) {
        if (object->pad_gamepad->buttons_pressed & GAMEPAD_JUMP) {
            StartJump(object, 0);
            object->movement_runtime_flags |= 0x10;
            f32 height = 0.1f + object->external_force.y - object->jump_start_height;
            if (height > 0.0f)
                object->apiobj.velocity.y =
                    NuFsqrt(-2.0f * object->apiobj.character_data->game_character->gravity * height);
            return;
        }
        if (object->apiobj.field_0x27d & 2) {
            object->character_context = -1;
            return;
        }
        if (object->field_0x7a3 || object->context_animation != LEGOACT_LEDGE_GRAB ||
            AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0)) {
            object->context_animation_timer += FRAMETIME;
        }
        if (!object->field_0x7a3 && object->context_animation_timer >= object->field_0x768)
            object->field_0x7a3 = 1;
        f32 travelled = 0.0f;
        if (LedgeTerrain_Attach(object, object->apiobj.movement_facing_angle, &position, &wall_angle)) {
            f32 x = object->external_force.x;
            f32 z = object->external_force.z;
            object->apiobj.movement_facing_angle = wall_angle + 0x8000;
            object->external_force.y = position.y;
            object->launch_origin.x = position.x;
            object->external_force.x = object->apiobj.position.x;
            object->external_force.z = object->apiobj.position.z;
            object->airborne_action_duration = 0.25f;
            object->launch_origin.y = position.y;
            object->launch_origin.z = position.z;
            z = object->apiobj.position.z - z;
            x = object->apiobj.position.x - x;
            travelled = NuFsqrt(x * x + z * z);
        } else {
            object->airborne_action_duration -= FRAMETIME;
            if (object->airborne_action_duration <= 0.0f) {
                object->character_context = -1;
                return;
            }
        }
        if (!object->field_0x7a3)
            return;
        if (!(object->pad_gamepad->input_magnitude > 0.0f)) {
            object->context_animation = LEGOACT_LEDGE_IDLE;
            return;
        }
        u16 input = GamePad_InputAngle(object, object->pad_gamepad);
        u16 sideways = object->apiobj.movement_facing_angle + 0x4000;
        f32 push = PushingTowardsAngle(input, sideways);
        f32 speed;
        if (push > NuTrigTable[0x3555]) {
            if (LEGOACT_LEDGE_RIGHT != -1 &&
                object->apiobj.character_model->model_data_b[LEGOACT_LEDGE_RIGHT] != NULL) {
                object->context_animation = LEGOACT_LEDGE_RIGHT;
                speed = fabsf(AnimSpeed(object->apiobj.character_model, LEGOACT_LEDGE_RIGHT));
            } else {
                if (LedgeTerrain_CheckAnims)
                    return;
                speed = 0.5f;
                goto move;
            }
        } else if (push < -NuTrigTable[0x3555]) {
            if (LEGOACT_LEDGE_LEFT != -1 && object->apiobj.character_model->model_data_b[LEGOACT_LEDGE_LEFT] != NULL) {
                object->context_animation = LEGOACT_LEDGE_LEFT;
                speed = -fabsf(AnimSpeed(object->apiobj.character_model, LEGOACT_LEDGE_LEFT));
            } else {
                if (LedgeTerrain_CheckAnims)
                    return;
                speed = -0.5f;
                goto move;
            }
        } else {
            object->context_animation = LEGOACT_LEDGE_IDLE;
            return;
        }
        if (speed == 0.0f)
            return;
        if (LedgeTerrain_CheckAnims && !AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0))
            return;
    move:
        if (speed < 0.0f)
            travelled = -travelled;
        f32 distance = speed * FRAMETIME + travelled;
        NUVEC offset = {NU_SIN_LUT(sideways) * distance, 0.0f, NU_COS_LUT(sideways) * distance};
        NuVecAdd(&object->external_force, &object->external_force, &offset);
        NuVecAdd(&object->launch_origin, &object->launch_origin, &offset);
        return;
    }
    if (object->apiobj.field_0x27d != 0 || !(object->apiobj.velocity.y <= 0.0f))
        return;
    if (LedgeTerrain_CheckAnims &&
        (LEGOACT_LEDGE_IDLE == -1 || object->apiobj.character_model->model_data_b[LEGOACT_LEDGE_IDLE] == NULL))
        return;
    if (object->character_context != -1 &&
        !(LEGOCONTEXT_CLIMB != -1 && object->character_context == LEGOCONTEXT_CLIMB)) {
        if (LEGOCONTEXT_JUMP == -1 || object->character_context != LEGOCONTEXT_JUMP ||
            !(object->context_animation_timer >= 0.1f))
            return;
    }
    if (!object->apiobj.player_controlled && !(object->field_0xf01 & 0x80))
        return;
    if (!(object->pad_gamepad->input_magnitude > 0.0f))
        return;
    u16 facing = GamePad_InputAngle(object, object->pad_gamepad);
    if (!LedgeTerrain_Attach(object, facing, &position, &wall_angle))
        return;
    object->field_0x7a3 = 0;
    object->context_animation_timer = 0.0f;
    object->character_context = LEGOCONTEXT_LEDGETERRAIN;
    object->apiobj.velocity = v000;
    if (LEGOACT_LEDGE_GRAB != -1 && object->apiobj.character_model->model_data_b[LEGOACT_LEDGE_GRAB] != NULL) {
        object->context_animation = LEGOACT_LEDGE_GRAB;
        object->field_0x768 = AnimDuration(object->id, LEGOACT_LEDGE_GRAB, 0.0f, 0.0f, 1);
    } else {
        object->field_0x768 = 0.1f;
        object->context_animation = LEGOACT_LEDGE_IDLE;
        ResetAnimPacket(&object->apiobj.anim_packet, LEGOACT_LEDGE_IDLE);
    }
    object->external_force.x = object->apiobj.position.x;
    object->external_force.y = position.y;
    object->external_force.z = object->apiobj.position.z;
    object->apiobj.movement_facing_angle = wall_angle + 0x8000;
    object->launch_origin = position;
    object->airborne_action_duration = 0.25f;
    return;
}

i32 LedgeTerrain_SetTargetMom(GameObject_s *object) {
    object->target_velocity.x = (NU_SIN_LUT(object->apiobj.movement_facing_angle) * LEDGETERRAINLOOKAHEAD +
                                 object->external_force.x - object->apiobj.upper_position.x) *
                                10.0f;
    object->target_velocity.z = (NU_COS_LUT(object->apiobj.movement_facing_angle) * LEDGETERRAINLOOKAHEAD +
                                 object->external_force.z - object->apiobj.upper_position.z) *
                                10.0f;
    object->target_velocity.y = (object->external_force.y - object->apiobj.upper_position.y) * 10.0f;
    return 1;
}

i32 CanClimbSurface(GameObject_s *object, i32 surface) {
    i32 result = 0;
    if (static_cast<u32>(surface) < 32 && (TerSurface[surface].flags & 0x10000) != 0) {
        result = 1;
        if (surface == 5 && (CanMagnetClimbFn == NULL || CanMagnetClimbFn(object) == 0))
            result = 0;
    }
    return result;
}

void Climb_SetMagnetAction(GameObject_s *object) {
    object->context_animation = LEGOACT_IDLE;
    if (object->pad_gamepad->input_magnitude > 0.0f) {
        if (LEGOACT_MAGNET_WALK_METAL != -1)
            object->context_animation = LEGOACT_MAGNET_WALK_METAL;
        else if (LEGOACT_WALK != -1)
            object->context_animation = LEGOACT_WALK;
    }
}

// Original 0x4f1f90, 134 bytes.
void Climb_UpdateMagnetRotation(GameObject_s *object) {
    object->field_0x1086 = 0;
    object->apiobj.pitch_angle =
        SeekRot(object->apiobj.pitch_angle, static_cast<u16>(-0x4000 - object->magnet_surface_angle), 6.0f);
    object->apiobj.roll_angle = SeekRot(object->apiobj.roll_angle, 0, 10.0f);
}

void Climb_SetMagnetDrawOffsetTarget(GameObject_s *object, nuvec_s *offset) {
    NuVecScale(offset, &object->external_force, MAGNETOFFSET);
    NuVecAdd(offset, offset, &object->launch_origin);
    NuVecSub(offset, offset, &object->apiobj.position);
}

void Climb_MoveCode(GameObject_s *object) {
    if (LEGOCONTEXT_CLIMB == -1)
        return;
    if (object->character_context != LEGOCONTEXT_CLIMB) {
        if (object->character_context == -1)
            goto enter_climb;
        if (LEGOCONTEXT_WALLSHUFFLE != -1 && object->character_context == LEGOCONTEXT_WALLSHUFFLE)
            goto enter_climb;
        if (LEGOCONTEXT_JUMP != -1 && object->character_context == LEGOCONTEXT_JUMP) {
            if (object->context_animation_timer >= 1.0f ||
                (object->context_animation_timer >= 0.1f && object->apiobj.velocity.y <= 0.0f))
                goto enter_climb;
        }
        if (LEGOCONTEXT_GLIDE != -1 && object->character_context == LEGOCONTEXT_GLIDE && object->field_0x788 == NULL)
            goto enter_climb;
        return;
    enter_climb:
        if (!(object->pad_gamepad->input_magnitude > 0.0f) || object->field_0x1084 == 0 ||
            !(fabsf(object->contact_normal.y) < NuTrigTable[0x3000]) ||
            CanClimbSurface(object, static_cast<i8>(object->field_0x6b0)) == 0)
            return;
        const NUVEC normal = object->contact_normal;
        const u8 surface = object->field_0x6b0;
        NUVEC forward;
        NuVecRotateY(&forward, &v001, object->apiobj.movement_facing_angle);
        const f32 x = object->contact_position.x - object->apiobj.position.x;
        const f32 z = object->contact_position.z - object->apiobj.position.z;
        if (!(forward.x * x + forward.z * z > 0.0f))
            return;
        object->apiobj.movement_facing_angle = NuAtan2D(x, z);
        object->apiobj.velocity.y = 0.0f;
        object->character_context = LEGOCONTEXT_CLIMB;
        object->field_0x7a6 = surface;
        object->airborne_action_duration = 0.25f;
        if (surface == 5) {
            Climb_SetMagnetAction(object);
            Climb_SetMagnetDrawOffsetTarget(object, &object->zipup_swing_position);
        } else {
            object->context_animation = LEGOACT_CLIMB_IDLE;
        }
        object->external_force = normal;
        object->launch_origin = object->contact_position;
        FindAnglesXY(&object->external_force, &object->magnet_surface_angle, NULL);
        object->apiobj.velocity.z = 0.0f;
        object->apiobj.velocity.x = 0.0f;
        return;
    }

    if ((object->pad_gamepad->buttons_pressed & GAMEPAD_JUMP) != 0 && object->field_0x7a6 != 5) {
        StartJump(object, 0);
        object->movement_runtime_flags |= 0x10;
        const u16 angle = object->apiobj.movement_facing_angle;
        const f32 speed = object->apiobj.character_data->game_character->run_speed;
        object->apiobj.velocity.x = -NU_SIN_LUT(angle) * speed;
        object->apiobj.velocity.z = -NU_COS_LUT(angle) * speed;
        return;
    }
    const u16 input_angle = GamePad_InputAngle(object, object->pad_gamepad);
    if ((object->apiobj.field_0x27d & 2) != 0 &&
        (object->apiobj.field_0x281 == object->field_0x7a6 ||
         (object->pad_gamepad->input_magnitude > 0.0f &&
          PushingTowardsAngle(input_angle, object->apiobj.movement_facing_angle) < 0.0f))) {
        object->character_context = -1;
    } else if (object->field_0x1084 != 0 && object->field_0x6b0 == object->field_0x7a6) {
        object->airborne_action_duration = 0.25f;
        object->apiobj.movement_facing_angle = NuAtan2D(object->contact_position.x - object->apiobj.position.x,
                                                        object->contact_position.z - object->apiobj.position.z);
        object->external_force = object->contact_normal;
        object->launch_origin = object->contact_position;
        FindAnglesXY(&object->external_force, &object->magnet_surface_angle, NULL);
    } else {
        object->airborne_action_duration -= FRAMETIME;
        if (object->airborne_action_duration <= 0.0f)
            object->character_context = -1;
    }
    if (object->character_context != LEGOCONTEXT_CLIMB)
        return;
    if (object->field_0x7a6 != 5) {
        if (object->pad_gamepad->input_magnitude > 0.0f) {
            i32 difference =
                RotDiff(object->apiobj.movement_facing_angle, GamePad_InputAngle(object, object->pad_gamepad));
            if (difference < 0)
                difference = -difference;
            object->context_animation = difference > 0x4000 ? LEGOACT_CLIMB_DOWN : LEGOACT_CLIMB_UP;
            if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL)
                return;
        }
        object->context_animation = LEGOACT_CLIMB_IDLE;
    } else {
        Climb_SetMagnetAction(object);
        Climb_SetMagnetDrawOffsetTarget(object, &object->zipup_swing_position);
        object->movement_runtime_flags |= 0x20;
        if (object->pad_gamepad->input_magnitude > 0.0f)
            object->tertiary_lean_angle =
                SeekRot(object->tertiary_lean_angle,
                        static_cast<u16>(RotDiff(object->apiobj.movement_facing_angle, input_angle)), 9.0f);
    }
}

// Original 0x4f26a0, 498 bytes.
i32 Climb_SetTargetMom(GameObject_s *object, u16 input_angle) {
    f32 speed = 0.0f;
    i32 animation = object->context_animation;
    if (animation != -1 &&
        (animation == LEGOACT_CLIMB_UP || animation == LEGOACT_CLIMB_DOWN || animation == LEGOACT_CLIMB_LEFT ||
         animation == LEGOACT_CLIMB_RIGHT || animation == LEGOACT_MAGNET_WALK_METAL)) {
        if (object->apiobj.character_model->model_data_b[animation] != NULL)
            speed = fabsf(AnimSpeed(object->apiobj.character_model, animation));
    }
    if (speed == 0.0f)
        speed = 0.5f;
    u16 angle = object->apiobj.movement_facing_angle;
    object->target_velocity.x = NuTrigTable[static_cast<u16>(angle / 2)] * speed;
    object->target_velocity.y = 0.0f;
    object->target_velocity.z = NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff] * speed;
    if (object->pad_gamepad->input_magnitude > 0.0f) {
        angle += 0x4000;
        f32 lateral = PushingTowardsAngle(input_angle, angle);
        object->target_velocity.x += NuTrigTable[static_cast<u16>(angle / 2)] * lateral * speed;
        object->target_velocity.z += lateral * NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff] * speed;
        f32 vertical = PushingTowardsAngle(input_angle, object->apiobj.movement_facing_angle);
        object->target_velocity.y += vertical * speed;
    }
    return 1;
}

void Hang_MoveCode(GameObject_s *object) {
    i16 animation;
    i8 context;
    i32 *hang_context = &LEGOCONTEXT_HANG;
    if (*hang_context == -1) {
        return;
    }

    if (*hang_context != static_cast<i8>(object->character_context))
        goto try_start_hang;

    if ((object->pad_gamepad->buttons_pressed & GAMEPAD_JUMP) != 0)
        goto check_hang_jump;

check_hang_contact:
    if (object->apiobj.field_0x27d == 0) {
        if (object->apiobj.field_0x280 != object->field_0x7a6)
            goto cancel_hang_velocity;

        if (object->pad_gamepad->input_magnitude > 0.0f) {
            animation = LEGOACT_HANG_MOVE;
            if (animation != -1)
                goto animation_selected;
        }
        animation = LEGOACT_HANG_IDLE;
    animation_selected:
        object->context_animation = animation;
        if (object->field_0x1084 != 0 && object->apiobj.field_0x280 == object->field_0x6b0) {
            object->airborne_action_duration = 0.25f;
            goto copy_hang_force;
        }

        const f32 duration = object->airborne_action_duration;
        const f32 frame_time = FRAMETIME;
        object->airborne_action_duration = duration - frame_time;
        if (duration - frame_time <= 0.0f)
            goto cancel_hang;
        return;
    }

cancel_hang:
    object->character_context = -1;
    return;

cancel_hang_velocity:
    object->apiobj.velocity.y = 0.0f;
    object->character_context = -1;
    return;

check_hang_jump:
    if (object->field_0x7a6 != 5)
        goto cancel_hang_velocity;
    goto check_hang_contact;

try_start_hang:
    if (object->apiobj.field_0x27d != 0)
        return;
    if (object->field_0x1084 == 0)
        return;
    if (CanClimbSurface(object, static_cast<i8>(object->field_0x6b0)) == 0)
        return;
    if (-NuTrigTable[0x3000] > object->contact_normal.y)
        goto hang_normal_valid;
    return;

hang_normal_valid:
    context = object->character_context;
    if (context != -1) {
        const i32 climb_context = LEGOCONTEXT_CLIMB;
        if (climb_context != -1 && climb_context == context)
            goto start_hang;
        const i32 jump_context = LEGOCONTEXT_JUMP;
        if (jump_context == -1 || jump_context != context)
            return;
        if (object->context_animation_timer >= 0.1f)
            goto start_hang;
        return;
    }

start_hang:
    object->airborne_action_duration = 0.25f;
    object->character_context = static_cast<i8>(*hang_context);
    object->context_animation = LEGOACT_HANG_IDLE;
    object->field_0x7a6 = object->field_0x6b0;

copy_hang_force:
    object->external_force = object->contact_normal;
}

i32 Hang_SetTargetMom(GameObject_s *object) {
    if (object->pad_gamepad->input_magnitude > 0.0f) {
        f32 speed;
        if (LEGOACT_HANG_MOVE != -1 && object->apiobj.character_model->model_data_b[LEGOACT_HANG_MOVE] != NULL) {
            speed = AnimSpeed(object->apiobj.character_model, LEGOACT_HANG_MOVE);
            if (speed < 0.25f)
                speed = 0.25f;
        } else {
            speed = 0.5f;
        }
        object->target_velocity.x = NU_SIN_LUT(object->apiobj.movement_facing_angle) * speed;
        object->target_velocity.z = NU_COS_LUT(object->apiobj.movement_facing_angle) * speed;
    } else {
        object->target_velocity.x = object->target_velocity.z = 0.0f;
    }
    object->target_velocity.y = 0.5f;
    return 1;
}

void *CreateClimbObjectSys(VARIPTR *buf, VARIPTR *buf_end, i32 count) {
    if (count == 0)
        return NULL;
    CLIMBOBJECTSYS_s *system = static_cast<CLIMBOBJECTSYS_s *>(GameBufferAlloc(buf, buf_end, sizeof(CLIMBOBJECTSYS_s)));
    if (system == NULL)
        return NULL;
    system->capacity = static_cast<u16>(count);
    system->objects =
        static_cast<CLIMBOBJECT_s *>(GameBufferAlloc(buf, buf_end, system->capacity * sizeof(CLIMBOBJECT_s)));
    return system;
}

void InitClimbObjectSys(WORLDINFO_s *world) {
    if (world == NULL || world->ai_sys == NULL || world->ai_sys->path_sys == NULL || world->climb_object_sys == NULL)
        return;
    world->climb_object_sys->count = 0;
    memset(world->climb_object_sys->objects, 0, world->climb_object_sys->capacity * sizeof(CLIMBOBJECT_s));
    for (i32 index = 0; index < world->ai_sys->path_sys->path_count; ++index) {
        AIPATH *path = world->ai_sys->path_sys->paths[index];
        if (path == NULL)
            continue;
        AIPATHCNX *connection = path->connections;
        for (i32 connection_index = 0; connection_index < path->connection_count; ++connection_index, ++connection) {
            CLIMBOBJECTSYS_s *system = world->climb_object_sys;
            if (system->count < system->capacity &&
                (connection->traversal_flags[0] & (LEGO_AIPATHCNX_MAGNETCLIMB | LEGO_AIPATHCNX_CLIMB)) != 0) {
                CLIMBOBJECT_s *object = &system->objects[system->count++];
                object->connection = connection;
                object->path = path;
                ClimbObject_FindNormal(object);
            }
        }
    }
}

CLIMBOBJECT_s *FindClimbObject(CLIMBOBJECTSYS_s *system, AIPATHCNX_s *connection) {
    if (system == NULL)
        return NULL;
    CLIMBOBJECT_s *object = system->objects;
    for (i32 index = 0; index < system->count; ++index, ++object) {
        if (object->connection != connection)
            continue;
        if ((object->flags & 2) != 0)
            return NULL;
        if ((object->flags & 1) != 0)
            return object;
        ClimbObject_FindNormal(object);
        if ((object->flags & 1) != 0)
            return object;
        object->flags |= 2;
    }
    return NULL;
}

// Original 0x4f2e50, 258 bytes. The original returns an integer.
i32 ObjOpponentStillThere(GameObject_s *object, GameObject_s *opponent, f32 gap) {
    i32 result = 0;
    if (object->force_target != NULL) {
        NUVEC forward, difference;
        NuVecRotateY(&forward, &v001, object->apiobj.field_0x276);
        f32 distance = NuVecDistSqr(&opponent->apiobj.position, &object->apiobj.position, &difference);
        f32 dot = forward.x * difference.x + forward.z * difference.z;
        if (static_cast<i8>(object->context_flags) < 0 ? dot <= 0.0f : dot >= 0.0f) {
            f32 radius = object->apiobj.field_0x1dc + opponent->apiobj.field_0x1dc + gap;
            result = distance < radius * radius;
        }
    }
    return result;
}

void SetWeaponOut(GameObject_s *object) {
    const i8 context = object->character_context;
    if (context != -1 && (context == LEGOCONTEXT_WEAPONIN || context == LEGOCONTEXT_WEAPONOUT)) {
        object->character_context = -1;
    }
    object->weapon_scale = 1.0f;
    object->weapon_out_timer = 0.0f;
    object->field_0xe22 |= GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION;
    object->weapon_scale_state = WEAPON_SCALE_IDLE;
}

void FastWeaponOut(GameObject_s *object, i32 force_sound) {
    const i8 context = object->character_context;
    if (context != -1 && (context == LEGOCONTEXT_WEAPONIN || context == LEGOCONTEXT_WEAPONOUT)) {
        object->character_context = -1;
    }
    if (__builtin_expect(force_sound != 0, 1) && object->weapon_scale == 0.0f)
        FastWeaponOutSfx(object);
    object->weapon_scale_rate = 5.0f;
    object->weapon_out_timer = 0.0f;
    object->weapon_scale_state = WEAPON_SCALE_EXTENDING;
}

void SlowWeaponOut(GameObject_s *object) {
    if (LEGOCONTEXT_WEAPONOUT != -1 && Fighting_WeaponOutActionFn != NULL) {
        const i32 action = Fighting_WeaponOutActionFn(object);
        if (action != -1 && object->apiobj.character_model->model_data_b[action] != NULL) {
            const f32 end = NuAnimEndFrame(object->apiobj.character_model->model_data_b[action]);
            f32 start = AnimListFrame(object->apiobj.character_model, action, 0);
            if (start < 1.0f)
                start = 1.0f;
            if (start >= 1.0f && start < end) {
                const f32 finish = AnimListFrame(object->apiobj.character_model, action, 1);
                if (finish > end || finish > start) {
                    object->context_animation = action;
                    object->character_context = LEGOCONTEXT_WEAPONOUT;
                    object->context_animation_timer = AnimDuration(object->id, action, 0.0f, 0.0f, 1);
                    object->weapon_scale = 0.0f;
                    object->weapon_out_timer = 0.0f;
                    return;
                }
            }
        }
    }
    FastWeaponOut(object, 1);
}

void KeepWeaponOut(GameObject_s *object) {
    object->weapon_scale = 1.0f;
    object->field_0xe22 |= GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION;
    object->weapon_scale_state = WEAPON_SCALE_IDLE;
    object->field_0xef8 |= GAMEOBJECT_EF8_FLAG_KEEP_WEAPON_OUT;
}

void SetWeaponIn(GameObject_s *object) {
    const i8 context = object->character_context;
    if (context != -1 && (context == LEGOCONTEXT_WEAPONIN || context == LEGOCONTEXT_WEAPONOUT)) {
        object->character_context = -1;
    }
    object->weapon_scale = 0.0f;
    object->field_0xe22 &= ~GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION;
    object->weapon_scale_state = WEAPON_SCALE_IDLE;
}

void FastWeaponIn(GameObject_s *object, i32 force_sound) {
    const i8 context = object->character_context;
    if (context != -1 && (context == LEGOCONTEXT_WEAPONIN || context == LEGOCONTEXT_WEAPONOUT)) {
        object->character_context = -1;
    }
    if (__builtin_expect(force_sound != 0, 1) && object->weapon_scale == 1.0f)
        FastWeaponInSfx(object);
    object->weapon_scale_rate = 5.0f;
    object->weapon_scale_state = WEAPON_SCALE_RETRACTING;
}

void SlowWeaponIn(GameObject_s *object) {
    if (LEGOCONTEXT_WEAPONIN != -1 && Fighting_WeaponInActionFn != NULL) {
        const i32 action = Fighting_WeaponInActionFn(object);
        if (action != -1 && object->apiobj.character_model->model_data_b[action] != NULL) {
            const f32 end = NuAnimEndFrame(object->apiobj.character_model->model_data_b[action]);
            f32 start = AnimListFrame(object->apiobj.character_model, action, 0);
            if (!(start >= 1.0f))
                start = 1.0f;
            if (start >= 1.0f && start < end) {
                const f32 finish = AnimListFrame(object->apiobj.character_model, action, 1);
                if (finish > end || finish > start) {
                    object->context_animation = action;
                    object->character_context = LEGOCONTEXT_WEAPONIN;
                    object->context_animation_timer = AnimDuration(object->id, action, 0.0f, 0.0f, 1);
                    object->weapon_scale = 1.0f;
                    return;
                }
            }
        }
    }
    FastWeaponIn(object, 1);
}

void WeaponOutCode(GameObject_s *object) {
    object->weapon_out_timer += FRAMETIME;
    if (TouchHacks::ShouldKeepWeaponOut(*object)) {
        object->weapon_out_timer = 0.0f;
        if ((object->field_0xe22 & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) == 0 && object->character_context != 7) {
            if (object->weapon_scale_state == WEAPON_SCALE_IDLE && object->apiobj.field_0x27d != 0 &&
                object->pad_gamepad->input_magnitude == 0.0f) {
                i32 action = 17;
                if (object->apiobj.character_data->game_character->field275_0x116 == 0 &&
                    (object->apiobj.character_data->model_flags & 0x80) != 0)
                    action = 127;
                if (object->apiobj.character_model->model_data_b[action] != NULL &&
                    (object->character_context == -1 || (CInfo[object->character_context].flags & 4) != 0)) {
                    SlowWeaponOut(object);
                    goto after_weapon_touch;
                }
            }
            FastWeaponOut(object, 1);
        }
    } else if (TouchHacks::ShouldPutWeaponAway(*object) &&
               (object->field_0xe22 & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) != 0 && object->character_context != 6) {
        SlowWeaponIn(object);
    }
after_weapon_touch:
    if (LEGOCONTEXT_WEAPONOUT == -1 || object->character_context != LEGOCONTEXT_WEAPONOUT)
        return;
    if (object->pad_gamepad->input_magnitude > 0.0f &&
        (object->apiobj.character_data->game_character->field_0x94 & 0x1000) == 0) {
        if (object->weapon_scale == 0.0f && object->weapon_scale_state != WEAPON_SCALE_EXTENDING) {
            FastWeaponOut(object, 0);
        }
        object->character_context = -1;
        return;
    }
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    const i16 animation = packet.blending ? packet.blend_animation_b : packet.animation_index;
    if (animation != object->context_animation) {
        object->weapon_scale = 0.0f;
        return;
    }
    const f32 start = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
    object->context_animation_timer -= FRAMETIME;
    if (object->context_animation_timer <= 0.0f) {
        object->character_context = -1;
        if (object->weapon_scale != 0.0f)
            return;
    } else {
        if (object->weapon_scale != 0.0f || object->weapon_scale_state == WEAPON_SCALE_EXTENDING ||
            packet.requested_animation != object->context_animation)
            return;
        const f32 time = packet.blending ? packet.blend_target_time : packet.current_time;
        if (!(time >= start))
            return;
    }
    const f32 end = AnimListFrame(object->apiobj.character_model, object->context_animation, 1);
    object->weapon_scale_state = WEAPON_SCALE_EXTENDING;
    object->weapon_scale_rate = 1.0f / AnimDuration(object->id, object->context_animation, start, end, 0);
    if ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_JEDI) != 0) {
        if (object->apiobj.field_0x27c != -1 || WeaponInOut_NoAIJediSfx == 0) {
            GameAudio_PlaySfx(0x3e, &object->apiobj.collision_position, GameAudio_GetPlrSfxBits(object), 1);
        }
    } else {
        GameAudio_PlaySfx(0x43, &object->apiobj.collision_position, 0, 1);
    }
    ResetCharacterIdle(object, 1, GetDefaultIdle(object));
}

void WeaponInCode(GameObject_s *object) {
    if (LEGOCONTEXT_WEAPONIN == -1 || object->character_context != LEGOCONTEXT_WEAPONIN)
        return;
    GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    if (object->pad_gamepad->input_magnitude > 0.0f && (data->field_0x94 & 0x1000) == 0) {
        if (object->weapon_scale == 1.0f && object->weapon_scale_state != WEAPON_SCALE_RETRACTING) {
            FastWeaponIn(object, 0);
        }
        object->character_context = -1;
        return;
    }
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    const i16 animation = packet.blending ? packet.blend_animation_b : packet.animation_index;
    if (animation != object->context_animation) {
        object->weapon_scale = 1.0f;
        return;
    }
    const f32 start = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
    object->context_animation_timer -= FRAMETIME;
    if (object->context_animation_timer <= 0.0f) {
        object->character_context = -1;
        if (object->weapon_scale != 1.0f)
            return;
    } else {
        if (object->weapon_scale != 1.0f || object->weapon_scale_state == WEAPON_SCALE_RETRACTING ||
            packet.requested_animation != object->context_animation)
            return;
        const f32 time = packet.blending ? packet.blend_target_time : packet.current_time;
        if (!(time >= start))
            return;
    }
    const f32 end = AnimListFrame(object->apiobj.character_model, object->context_animation, 1);
    object->weapon_scale_state = WEAPON_SCALE_RETRACTING;
    object->weapon_scale_rate = 1.0f / AnimDuration(object->id, object->context_animation, start, end, 0);
    if ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_JEDI) != 0) {
        if (object->apiobj.field_0x27c != -1 || WeaponInOut_NoAIJediSfx == 0) {
            GameAudio_PlaySfx(0x3d, &object->apiobj.collision_position, GameAudio_GetPlrSfxBits(object), 1);
        }
    } else {
        GameAudio_PlaySfx(0x42, &object->apiobj.collision_position, 0, 1);
    }
    ResetCharacterIdle(object, 1, GetDefaultIdle(object));
}

void KeepWeaponIn(GameObject_s *object) {
    object->weapon_scale = 0.0f;
    object->field_0xe22 &= ~GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION;
    object->weapon_scale_state = WEAPON_SCALE_IDLE;
    object->field_0xef8 &= ~GAMEOBJECT_EF8_FLAG_KEEP_WEAPON_OUT;
}

void AutoWeaponOnOff(GameObject_s *object) {
    const i8 context = object->character_context;
    if (context != -1 && (context == LEGOCONTEXT_WEAPONOUT || context == LEGOCONTEXT_WEAPONIN))
        return;
    const i32 index = CurrentAnim(&object->apiobj.anim_packet);
    if (index == -1)
        return;
    CHARACTERANIM_s *animation = static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[index]);
    const u32 flags = animation->flags;
    const ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((flags & CHARACTER_ANIMATION_FLAG_GUN_OFF) != 0) {
        if (object->weapon_scale > 0.0f) {
            if ((packet.flags & ANIMPACKET_FLAG_ANIMATION_CHANGED) != 0 && packet.blending != 0) {
                FastWeaponIn(object, 0);
            } else if ((packet.flags & ANIMPACKET_FLAG_ANIMATION_CHANGED) != 0 ||
                       object->weapon_scale_state != WEAPON_SCALE_RETRACTING) {
                SetWeaponIn(object);
            }
        }
    } else if ((flags & CHARACTER_ANIMATION_FLAG_GUN_ON) != 0 && object->weapon_scale < 1.0f) {
        if ((packet.flags & ANIMPACKET_FLAG_ANIMATION_CHANGED) != 0 && packet.blending != 0) {
            FastWeaponOut(object, 0);
        } else if ((packet.flags & ANIMPACKET_FLAG_ANIMATION_CHANGED) != 0 ||
                   object->weapon_scale_state != WEAPON_SCALE_EXTENDING) {
            SetWeaponOut(object);
        }
    }
}

void WeaponScalingCode(GameObject_s *object) {
    switch (object->weapon_scale_state) {
        case WEAPON_SCALE_EXTENDING:
            object->weapon_scale = SeekLinearF(object->weapon_scale, 1.0f, object->weapon_scale_rate * FRAMETIME);
            if (object->weapon_scale >= 1.0f) {
                object->weapon_scale_state = WEAPON_SCALE_IDLE;
                object->field_0xe22 |= GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION;
            }
            break;
        case WEAPON_SCALE_RETRACTING:
            object->weapon_scale = SeekLinearF(object->weapon_scale, 0.0f, object->weapon_scale_rate * FRAMETIME);
            if (object->weapon_scale <= 0.0f) {
                object->weapon_scale_state = WEAPON_SCALE_IDLE;
                object->field_0xe22 &= ~GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION;
            }
            break;
        default:
            break;
    }
}

i32 NewBlockAction(GameObject_s *object) {
    i32 actions[3];
    i32 count = 0;
    for (i32 action = 0; action < apicharsys->model_id_capacity && count < 3; ++action) {
        if ((ActionInfo[action].flags & 8) != 0 && object->apiobj.character_model->model_data_b[action] != NULL) {
            actions[count++] = action;
        }
    }
    if (count == 0)
        return 0;
    i32 action = actions[0];
    if (count != 1) {
        do {
            action = actions[qrand() / (0xffff / count + 1)];
        } while (action == object->previous_block_animation);
    }
    object->previous_block_animation = action;
    object->context_animation = action;
    return 1;
}

void BlockSfx(GameObject_s *object) {
    GameObject_s *target = object->force_target;
    if (target != NULL) {
        const u32 context_flags = CInfo[target->character_context].flags;
        if ((context_flags & 0x4000000) != 0 || ((context_flags & 0x8000000) != 0 && (target->jump_flags & 2) != 0)) {
            const i32 sfx_bits = GameAudio_GetPlrSfxBits(target);
            GameAudio_PlaySfx(0x3f, &object->apiobj.collision_position, sfx_bits, 0);
        }
    }
}

i32 CanStepBack(GameObject_s *object) {
    i8 context = object->character_context;
    if ((CInfo[context].flags & 0x10000) != 0 ||
        (LEGOCONTEXT_JUMP != -1 && LEGOCONTEXT_JUMP == context && object->action_movement_state == 3)) {
        return 1;
    }
    GAMECHARACTERDATA *character = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    if ((character->flags_090 & 0x8000) != 0 && LEGOCONTEXT_COMBO != 0 && LEGOCONTEXT_COMBO == context) {
        return 1;
    }
    return 0;
}

i32 StepBackFromTarget(GameObject_s *object) {
    NUVEC *target_position;
    float inner_distance;
    float outer_distance;
    if (object->blowup_target != NULL) {
        target_position = &object->blowup_target->mid_position;
        if ((LEGOCONTEXT_COMBO != -1 && LEGOCONTEXT_COMBO == object->character_context) ||
            (LEGOCONTEXT_JUMP != -1 && LEGOCONTEXT_JUMP == object->character_context &&
             object->action_movement_state == 3)) {
            float target_radius = object->blowup_target->target_scale;
            inner_distance = object->apiobj.collision_radius * 1.75f + target_radius;
            outer_distance = object->apiobj.collision_radius * 2.25f + target_radius;
        } else {
            inner_distance = object->apiobj.collision_radius * 0.75f + object->blowup_target->target_scale;
            outer_distance = object->apiobj.collision_radius * 1.25f + object->blowup_target->target_scale;
        }
    } else {
        GAMECHARACTERDATA *character = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
        if ((character->flags_090 & 0x8000) == 0 || object->force_target == NULL) {
            return 0;
        }
        if ((LEGOCONTEXT_COMBO == -1 || LEGOCONTEXT_COMBO != object->character_context) &&
            (LEGOCONTEXT_JUMP == -1 || LEGOCONTEXT_JUMP != object->character_context ||
             object->action_movement_state != 3)) {
            return 0;
        }
        float target_radius = object->force_target->apiobj.field_0x1dc;
        target_position = &object->force_target->apiobj.collision_position;
        inner_distance = object->apiobj.field_0x1dc * 1.75f + target_radius;
        outer_distance = object->apiobj.field_0x1dc * 2.25f + target_radius;
    }
    float speed = AnimSpeed(object->apiobj.character_model, object->context_animation);
    if (speed != 0.0f) {
        float distance_squared = NuVecXZDistSqr(&object->apiobj.collision_position, target_position, NULL);
        if (!(distance_squared >= outer_distance * outer_distance)) {
            float blend = 1.0f;
            if (!(inner_distance * inner_distance >= distance_squared)) {
                blend = 1.0f - (NuFsqrt(distance_squared) - inner_distance) / (outer_distance - inner_distance);
            }
            speed = (-speed - speed) * blend + speed;
            object->target_velocity.x = NU_SIN_LUT(object->apiobj.movement_facing_angle) * speed;
            object->target_velocity.z = speed * NU_COS_LUT(object->apiobj.movement_facing_angle);
            return 1;
        }
    }
    return 0;
}

