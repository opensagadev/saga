// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/actions/movement/jumping.h"
#include "globals.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/world/world.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/items/objects/gameobjects.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/ai/aisys/aisys.h"

i32 DoubleJump_JediSlam = 0;
f32 SLAMJUMPSPEED = 3.0f;
bool (*IsWearingBackPackFn)(GameObject_s *) = NULL;
i32 (*Jump_PreventJumpFn)(GameObject_s *) = NULL;
i32 (*CanMagnetClimbFn)(GameObject_s *) = NULL;
i32 (*CanGlideFn)(GameObject_s *) = NULL;
i32 DoubleJump_AlwaysReachJump2Height = 0;
i32 (*Slam_GetDebrisFn)(GameObject_s *, i32) = NULL;

#include "legoapi/actions/movement/jumping_types.h"

static const f32 PLAYER_JUMP_MINIMUM_AIR_TIME = 0.1f;
static const f32 PLAYER_JUMP_REENTRY_DELAY = 0.2f;

void PlayJumpSfx(GameObject_s *object, i32 variant);
void PlayLandSfx(GameObject_s *object, i32 variant, i32 force);

void (*BigJump_EndOfLandFn)(GameObject_s *) = NULL;
void FindSlamOrigin(GameObject_s *, NUVEC *, NUVEC *);
void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);
void NewRumbleAllPlayers(f32, f32, i32, i32);
EXPLOSION *AddExplosion(NUVEC *, f32, f32, GameObject_s *, i32, i32);

static i32 BigJump_JumpAction_Default(GameObject_s *object);
i32 (*BigJump_JumpActionFn)(GameObject_s *) = BigJump_JumpAction_Default;

static i32 BigJump_LandAction_Default(GameObject_s *object);
i32 (*BigJump_LandActionFn)(GameObject_s *) = BigJump_LandAction_Default;

extern i16 id_YODA;
void Player_ClearContext(GameObject_s *, i32);
void Player_ResetContexts(PLAYERPACKET_s *);

i32 StartFallLand(GameObject_s *object, i32 action);

void StartEndOfJump(GameObject_s *object);

void MakeJumpReachHeight(GameObject_s *object, float height, i32 force);

// ==== src/legoapi/characters/motion/move.cpp: declarations ====

#include <stdlib.h>
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/hatmachine.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/actions/character/snake.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuqfnt.h"

#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/core/input/qrand.h"
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
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/render/light/surfaces.h"
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

void ComboHitFrame(GameObject_s *object, i32 damage);
void SetWeaponOut(GameObject_s *object);
void FastWeaponIn(GameObject_s *object, i32 sound);
void FastWeaponOut(GameObject_s *object, i32 sound);
void StartHold(GameObject_s *object);
void MakeJumpReachHeight(GameObject_s *, f32, i32);
void PlayJumpSfx(GameObject_s *, i32);
void Hint_SetComplete(i32);
void PlaySabreSfx(char *, GameObject_s *, NUVEC *, i32);
void Punch_Hit(GameObject_s *, GameObject_s *, f32, f32);
void StartQuickShoot(GameObject_s *, i32);
extern "C" i32 AddGameDebrisRot(APIDEBRISSYS_s *, i32, NUVEC *, i32, u16, u16);
i32 (*FindSlamOrigin_UseCPosFn)(GameObject_s *) = NULL;
void (*Jump_EndOfLandContextFn)(GameObject_s *) = NULL;
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
extern i16 id_YODA, id_YODAGHOST;

// Multiplies the ordinary camera's per-frame position blend.  The original
// keeps this as writable camera state (default 1.0), rather than folding it
// into the socket seek rate.

#include "legoapi/characters/motion/move_types.h"

i32 PodLevel(AREADATA_s *area);

// Retail splits the guard off as an inline head; the rest is this partial clone.

extern i32 LevFlag[4];
// Original exported level control, initialized to 2 at 0x623d00.

// These pod tuning values are zero-initialized in the original image.

extern "C" void PlaySfxById(i32 sfx_id, nuvec_s *position);

// Retail re-tests the three block animations at each use instead of caching a flag.
#define HAS_BLOCK_ANIMS(animations)                                                                                    \
    ((animations)[0x1a] != NULL || (animations)[0x1b] != NULL || (animations)[0x1c] != NULL)

// Retail inlines the fade and step checks into each caller; this is the remaining partial clone.

// Retail splits the context test off as an inline head; the rest is this partial clone.

i32 Slam_Start(GameObject_s *object, f32 speed);

void StartLunge(GameObject_s *object, f32 speed, f32 height);

i32 Glide_Start(GameObject_s *object);

void PlayLandSfx(GameObject_s *object, i32 type, i32 surface);

extern i32 DoubleJump_JediSlam;
extern f32 SLAMJUMPSPEED;
void PlayLandSfx(GameObject_s *, i32, i32);
void AddWaterSplash(GameObject_s *, NUVEC *);

i32 StartBackFlip(GameObject_s *object);

void UpdateLastSafePosition(GameObject_s *object);

void FindSlamOrigin(GameObject_s *object, NUVEC *start, NUVEC *end);

DECOMP_ASSERT(sizeof(AWKWARDSHAPESPHERE_s) == 12, "Awkward shape sphere size");
DECOMP_ASSERT(offsetof(AWKWARDSHAPESPHERE_s, radius) == 8, "Awkward shape sphere radius offset");

// ==== src/legoapi/characters/motion/gameanim.cpp: declarations ====
#include "gameapi/edtools/edfile.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numusic/sfx.h"

#include <float.h>
#include <string.h>

// Retail uses translation-unit-local helpers with compiler-inferred private
// calling conventions; keep their shared animation-mode call sites intact.

#include "legoapi/characters/motion/gameanim_types.h"

// ==== src/legoapi/world/levels/episodeV.cpp: declarations ====
#include "gameapi/ai/aisys/aipath.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/ai/core/legoai.h"
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

#include <math.h>

extern ADDPART_s Default_ADDPART;

extern "C" {
    // Shared suspend flag consulted by all timed debris emitters.

    void SetSfxBit_On(i32);
    void PlaySfxById(i32, NUVEC *);

}

// Forward declarations for local (static) part/gizmo helper stubs.

// Local (static) part (PART_s), pickup (GIZMOPICKUP_s), gizmo flow
// (GIZFLOW_s/FLOWBOX_s), power-up and space/starfighter helpers. Stubbed as
// local `t` symbols matching res/libTTapp.so.

extern WORLDINFO_s *WORLD;
extern GAMECAMERA_s *GameCam;

extern "C" {

PART_s *AddPart(ADDPART_s *params);

    // The editor loader clears these page IDs with aligned vector stores.

} // extern "C"

WORLDINFO_s *WorldInfo_CurrentlyActive();
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

extern i16 id_YODA, id_YODAGHOST, id_GAMORREANGUARD, id_JANGOFETT;

// Written by ThingManager's ctor (original global @0x124f2e0, .bss).

void Player_ClearContext(GameObject_s *object, i32 mode);
void Player_ResetContexts(PLAYERPACKET_s *packet);

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

#if defined(__i386__)
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone, regparm(2), sseregparm))
#else
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone))
#endif

// Retail keeps only the IPA-SRA clone, which takes the other object's x and z by address.

i32 RotDiff(u16, u16);

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
void StartQuickShoot(GameObject_s *object, i32 action);

void Punch_Hit(GameObject_s *attacker, GameObject_s *target, float, float gap);

// ==== src/legoapi/characters/core/players.cpp: declarations ====

#include "legoapi/items/base/collection.h"
#include "legoapi/world/levels/episode.h"
#include "legogame/game.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nufile/nufpar.h"

struct PLAYERPACKET_s;

// --- Helpers moved from world.cpp ---

// ---- Player start spawn entries ----

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

void Hint_SetComplete(i32 hint_id);

namespace {

} // namespace

#include "legoapi/characters/core/players_types.h"

DECOMP_ASSERT(sizeof(SPECIAL_LAYER_s) == 0xc, "SPECIAL_LAYER_s size");

// ==== src/legoapi/actions/movement/pushing.cpp: declarations ====
#include "legoapi/gizmos/door/push.h"
#include "legoapi/items/base/animpacket.h"

void FastWeaponIn(GameObject_s *, i32);

extern "C" i32 ParticlesPerSecond(f32 rate, f32 elapsed);

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

void GameCam_Judder(GAMECAMERA_s *camera, float amount, i32 axis, nuvec_s *source);

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
#include "legoapi/gizmos/transport/ledges.h"
#include "legoapi/gizmos/traps/shards.h"

struct FLOWBOX_s;

DECOMP_ASSERT(sizeof(GIZMOTYPES) == 0xc, "Gizmo type-table header ABI");
DECOMP_ASSERT(offsetof(GIZMOTYPES, types) == 8, "Gizmo type-table pointer offset");

// ==== src/legoapi/misc/legoapi_misc.cpp: declarations ====
#include "gameframework/saveload.h"
#include "legoapi/menus/screens/gamestructure.h"

// ==== src/legoapi/world/levels/episodeII.cpp: declarations ====

#include "nu2api/nu3d/nuhspecial.h"

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

extern void Player_ClearContext(GameObject_s *, i32);
extern void Player_ResetContexts(PLAYERPACKET_s *);

// ==== src/legoapi/gizmos/trigger/ai.cpp: all remaining content ====

// ==== src/legoapi/ai/game/creature.cpp: declarations ====

extern "C" {
    void AISysGetCharacterPathPos(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 route, i32 surface_flags);
    void ResetAnimPacket(ANIMPACKET_s *packet, i16 animation);
}

void Player_ClearContext(GameObject_s *object, i32 context);
#include "legoapi/ai/game/creature_types.h"

// ==== src/legoapi/ai/game/aipathcnxhelper.cpp: declarations ====

#include <stdint.h>
#include "nu2api/nucore/nulist.h"

// ==== src/legoapi/ai/core/ai_sys.cpp: declarations ====
#include "gameapi/ai/aisys/aiscript_types.h"

struct nuhspecial_s;
struct nuvec_s;

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

extern void Player_ClearContext(GameObject_s *object, i32 mode);

extern void Player_ResetContexts(PLAYERPACKET_s *packet);

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

void AISysGetCharacterPathPos(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 checks, i32 ground);

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
#pragma GCC optimize("O2", "omit-frame-pointer")
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

// Runtime-selected groups appended after the fixed terrain allocation.
#include "legoapi/render/core/terrain_stubs_types.h"

// NuFileExists @nufile (nucore_plain.cpp).
extern "C" i32 NuFileExists(char *name);

// ==== functions in original address order ====

extern "C" {
void AISysGetCharacterPathPos(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 checks, i32 ground);

}

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

void StartLunge(GameObject_s *, f32, f32);
i32 Slam_Start(GameObject_s *, f32);
void StartHold(GameObject_s *);
void ComboHitFrame(GameObject_s *, i32);
extern bool (*IsWearingBackPackFn)(GameObject_s *);
extern i32 (*Jump_PreventJumpFn)(GameObject_s *);
extern i32 (*CanMagnetClimbFn)(GameObject_s *);
extern i32 (*CanGlideFn)(GameObject_s *);
extern i32 DoubleJump_AlwaysReachJump2Height;
extern i32 (*Slam_GetDebrisFn)(GameObject_s *, i32);

extern void (*BigJump_EndOfLandFn)(GameObject_s *);

extern i32 (*BigJump_JumpActionFn)(GameObject_s *);

extern i32 (*BigJump_LandActionFn)(GameObject_s *);

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

void NewRumble(nupad_s *, f32, i32);

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

// ==== src/legoapi/props/objects/tubes.cpp: all remaining content ====

void NewRumble(nupad_s *pad, f32 strength, i32 mode);

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
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// ==== src/legoapi/misc/memory.cpp: all remaining content ====

// ==== functions in original address order ====

// ==== functions in original address order ====

// ==== src/legoapi/actions/character/specialmoves.cpp: declarations ====

#include "legoapi/actions/character/specialmoves_types.h"
DECOMP_ASSERT(sizeof(SPECIALMOVE_s) == 0x10, "SPECIALMOVE size");
DECOMP_ASSERT(offsetof(SPECIALMOVE_s, victim_animation) == 8, "SPECIALMOVE victim animation offset");
DECOMP_ASSERT(offsetof(SPECIALMOVE_s, flags) == 10, "SPECIALMOVE flags offset");

// ==== functions in original address order ====

static i32 BigJump_JumpAction_Default(GameObject_s *object) {
    if (object->field_0x7aa == 0) {
        if (LEGOACT_COMBOJUMP != -1 && object->apiobj.character_model->model_data_b[LEGOACT_COMBOJUMP] != NULL)
            return LEGOACT_COMBOJUMP;
        if (LEGOACT_JUMP2 != -1 && object->apiobj.character_model->model_data_b[LEGOACT_JUMP2] != NULL)
            return LEGOACT_JUMP2;
    } else if (object->field_0x7aa == 4) {
        if (LEGOACT_FLIP != -1 && object->apiobj.character_model->model_data_b[LEGOACT_FLIP] != NULL)
            return LEGOACT_FLIP;
    }
    return LEGOACT_JUMP;
}

static i32 BigJump_LandAction_Default(GameObject_s *object) {
    switch (object->field_0x7aa) {
        case 0:
            if (LEGOACT_COMBOLAND != -1 && object->apiobj.character_model->model_data_b[LEGOACT_COMBOLAND] != NULL)
                return LEGOACT_COMBOLAND;
            break;
        case 2:
        case 3:
            if (LEGOACT_LAND3 != -1 && object->apiobj.character_model->model_data_b[LEGOACT_LAND3] != NULL)
                return LEGOACT_LAND3;
            break;
        case 4:
            if (LEGOACT_FLIPLAND != -1 && object->apiobj.character_model->model_data_b[LEGOACT_FLIPLAND] != NULL)
                return LEGOACT_FLIPLAND;
            return LEGOACT_LAND;
        case 1:
            return LEGOACT_LAND;
        default:
            return LEGOACT_LAND;
    }
    if (LEGOACT_LAND2 != -1 && object->apiobj.character_model->model_data_b[LEGOACT_LAND2] != NULL)
        return LEGOACT_LAND2;
    return LEGOACT_LAND;
}

void StartJump(GameObject_s *object, i32 movement_state) {
    if (LEGOCONTEXT_JUMP != -1) {
        object->character_context = LEGOCONTEXT_JUMP;
        object->action_movement_state = static_cast<u8>(movement_state);
        object->jump_sequence = 1;
        object->context_animation_timer = 0.0f;
        object->jump_start_height = object->apiobj.collision_min.y;
        object->context_animation = LEGOACT_JUMP;
        object->jump_flags =
            static_cast<u8>((object->jump_flags & ~PLAYER_JUMP_FLAG_SPECIAL_TAKEOFF) |
                            ((movement_state == 3 || movement_state == 4) ? PLAYER_JUMP_FLAG_SPECIAL_TAKEOFF : 0));
        object->context_variant_flags =
            static_cast<i8>(static_cast<u8>(object->context_variant_flags) & ~PLAYER_JUMP_VARIANT_START_CLEAR);
        object->airborne_action_timer = 0.0f;
        if (static_cast<u32>(movement_state - 6) > 1) {
            PlayJumpSfx(object, 0);
        }
    }
    object->apiobj.field_0x27d = 0;
    object->field_0x105c = 0;
    GAMECHARACTERDATA *game_character = object->apiobj.character_data->game_character;
    f32 jump_speed = game_character->jump_speed;
    object->field_0xe22 |= PLAYER_JUMP_RUNTIME_BUTTON_HELD;
    object->delayed_turn_timer = 0.0f;
    object->airborne_input_timer = 0.0f;
    object->apiobj.velocity.y = jump_speed;
    object->airborne_collision_target = NULL;
    ResetAnimPacket(&object->apiobj.anim_packet, -1);
    object->apiobj.pitch_angle = 0;
    object->apiobj.roll_angle = 0;
    object->field_0x1086 = 2;
}

void MakeJumpReachHeight(GameObject_s *object, float height, i32 force) {
    const f32 current_height = object->apiobj.collision_min.y - object->jump_start_height;
    const f32 remaining_height = height - current_height;
    if (remaining_height > 0.0f) {
        const GAMECHARACTERDATA *game_character =
            static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
        const f32 vertical_speed = NuFsqrt(-2.0f * game_character->gravity * remaining_height);
        if (force != 0 || vertical_speed > object->apiobj.velocity.y) {
            object->apiobj.velocity.y = vertical_speed;
        }
    } else if (force != 0) {
        object->apiobj.velocity.y = 0.0f;
    }
}

void StartEndOfJump(GameObject_s *object) {
    if (LEGOCONTEXT_JUMP != -1) {
        object->character_context = LEGOCONTEXT_JUMP;
        object->action_movement_state = 0;
        object->jump_sequence = 2;
        object->context_animation = LEGOACT_FALL;
        object->jump_flags &= ~PLAYER_JUMP_FLAG_SPECIAL_TAKEOFF;
        object->jump_falling = 1;
        object->jump_button_released = 0;
        object->jump_second_jump = 0;
    }
    object->airborne_collision_target = NULL;
}

i32 StartBigJump(GameObject_s *object, NUVEC *destination, i32 mode, f32 height, f32 duration_scale, i32 animation,
                 i8 variant) {
    if (LEGOCONTEXT_BIGJUMP == -1 || (object->apiobj.character_data->model_flags & 0x200000) != 0)
        return 0;
    Player_ClearContext(object, 0);
    GameObject_s *carried = object->field_0xcc0;
    if (carried == NULL || carried->id != id_YODA || WORLD->current_level != DAGOBAHE_LDATA)
        carried = NULL;
    Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    object->field_0xcc0 = carried;
    object->build_button_taps = animation;
    object->field_0x7a3 = 1;
    object->jump_flags &= ~4;
    object->character_context = LEGOCONTEXT_BIGJUMP;
    object->field_0x7aa = variant;
    object->context_animation = BigJump_JumpActionFn(object);
    object->external_force = object->apiobj.position;
    object->launch_origin = *destination;
    object->airborne_action_duration = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 0);
    if (static_cast<u8>(variant - 2) <= 1)
        object->airborne_action_duration = object->apiobj.character_data->game_character->second_jump_duration;
    if (object->field_0x7aa != 0) {
        f32 distance = NuVecXZDist(&object->external_force, &object->launch_origin, NULL);
        f32 speed = object->apiobj.character_data->game_character->run_speed;
        f32 duration;
        if (distance == 0.0f || speed == 0.0f)
            duration = 0.0f;
        else
            duration = distance / speed;
        object->airborne_action_duration =
            object->airborne_action_duration > duration ? object->airborne_action_duration : duration;
        if (object->field_0x7aa == 2) {
            if (LEGOACT_JUMP2 != -1 && object->apiobj.character_model->model_data_b[LEGOACT_JUMP2] != NULL)
                height *= 1.5f;
        } else if (object->field_0x7aa == 3) {
            if (LEGOACT_JUMP3 != -1 && object->apiobj.character_model->model_data_b[LEGOACT_JUMP3] != NULL)
                height *= 1.8f;
        }
    }
    if (object->airborne_action_duration == 0.0f) {
        object->airborne_action_duration = 1.0f;
        object->context_animation = LEGOACT_FALL;
    } else {
        ResetAnimPacket(&object->apiobj.anim_packet, -1);
    }
    object->airborne_action_duration *= duration_scale;
    object->context_animation_timer = 0.0f;
    object->launch_origin.y -= object->character_bottom * object->apiobj.field_0xa8;
    u16 angle = NuAtan2D(destination->x - object->external_force.x, destination->z - object->external_force.z);
    if (object->field_0x7aa == 4)
        angle -= 0x8000;
    object->apiobj.facing_angle = angle;
    object->apiobj.field_0x276 = angle;
    object->apiobj.movement_facing_angle = angle;
    object->field_0xefd = (object->field_0xefd & ~0x20) | ((mode & 1) << 5);
    PlayJumpSfx(object, 2);
    object->ai.movement_event_flags |= 2;
    object->ai.field_0x180 = NULL;
    object->context_variant_flags &= ~1;
    if (!(height >= 0.0f))
        height = 0.0f;
    object->big_jump_height = height;
    return 1;
}

void FindSlamOrigin(GameObject_s *object, NUVEC *start, NUVEC *end) {
    if (FindSlamOrigin_UseCPosFn != NULL && FindSlamOrigin_UseCPosFn(object)) {
        start->x = object->apiobj.collision_position.x;
        start->z = object->apiobj.collision_position.z;
    } else {
        const i32 joint =
            static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->streak_joints[0][0];
        if (joint == -1 || object->apiobj.field_0x288 == 0 || object->apiobj.character_model == NULL ||
            object->apiobj.character_model->points_of_interest[joint] == NULL) {
            start->x = object->apiobj.position.x;
            start->z = object->apiobj.position.z;
        } else {
            start->x = object->joint_matrices[joint].m30;
            start->z = object->joint_matrices[joint].m32;
        }
    }
    start->y = object->apiobj.field_0x218;
    if (end != NULL) {
        const i32 joint =
            static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->streak_joints[0][1];
        if (joint == -1 || object->apiobj.field_0x288 == 0 || object->apiobj.character_model == NULL ||
            object->apiobj.character_model->points_of_interest[joint] == NULL) {
            *end = *start;
        } else {
            *end = *NUMTX_GET_ROW_VEC(&object->joint_matrices[joint], 3);
        }
    }
}

void BigJumpCode(GameObject_s *object) {
    if (LEGOCONTEXT_BIGJUMP == -1 || object->character_context != LEGOCONTEXT_BIGJUMP)
        return;
    object->apiobj.respawn_timer = 0.0f;
    if (object->build_button_taps != 0 && object->field_0x1084 != 0) {
        object->character_context = -1;
        return;
    }
    if (object->field_0x7a3 == 2) {
        object->context_animation_timer += FRAMETIME;
        if (object->context_animation_timer >= object->airborne_action_duration) {
            object->character_context = -1;
            if (BigJump_EndOfLandFn == NULL)
                return;
            BigJump_EndOfLandFn(object);
        }
    } else if (object->field_0x7a3 == 1) {
        object->context_animation_timer += FRAMETIME;
        if (object->context_animation_timer >= object->airborne_action_duration) {
            if (LEGOACT_FALL != -1 && object->context_animation == LEGOACT_FALL) {
                object->character_context = -1;
                return;
            }
            if (object->field_0x7aa != 0 &&
                (object->pad_gamepad->input_magnitude > 0.0f || object->IsRunningTaskType(MechTouchTaskGoTo::HashId))) {
                object->character_context = -1;
                return;
            }
            WORLDINFO *world = WorldInfo_CurrentlyActive();
            if (LEGOACT_COMBOJUMP != -1 && object->context_animation == LEGOACT_COMBOJUMP) {
                object->apiobj.facing_angle += 0x8000;
                object->apiobj.movement_facing_angle += 0x8000;
                object->apiobj.field_0x276 += 0x8000;
                ResetAnimPacket(&object->apiobj.anim_packet, -1);
            }
            if ((object->context_variant_flags & 1) != 0 && object->apiobj.field_0x27d == 0) {
                f32 ground = GameShadow(NULL, &object->launch_origin, 5.0f, -1);
                if (ground != 2000000.0f && object->launch_origin.y - ground >= 0.1f) {
                    StartEndOfJump(object);
                    return;
                }
            }
            ResetAnimPacket(&object->apiobj.anim_packet, -1);
            object->field_0x7a3 = 2;
            object->context_animation = BigJump_LandActionFn(object);
            f32 duration = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1);
            object->airborne_action_duration = duration == 0.0f ? 1.0f : duration;
            object->context_animation_timer = 0.0f;
            if ((object->field_0xefb & 8) != 0 || (object->field_0xefd & 0x20) != 0) {
                GameCam_Judder(GameCam, -0.3f, 0, &object->apiobj.collision_position);
                PlayLandSfx(object, 3, 0);
                NewRumbleAllPlayers(0.6f, 0.0f, 0, 0);
            } else if (object->apiobj.player_controlled && (TestForController() || SuperOptions.touch_controls == 0)) {
                GameCam_Judder(GameCam, -0.2f, 0, &object->apiobj.collision_position);
                PlayLandSfx(object, 3, 0);
            } else {
                PlayLandSfx(object, 0, 0);
            }
            object->external_force = object->apiobj.position;
            object->apiobj.velocity.x = object->apiobj.velocity.y = object->apiobj.velocity.z = 0.0f;
            if ((object->field_0xefd & 0x20) != 0) {
                NUVEC origin;
                FindSlamOrigin(object, &origin, NULL);
                AddExplosion(&origin, 2.0f, 0.4f, object, object->slam_debris_effect, 0x19);
            }
            AISysGetCharacterPathPos(world->ai_sys, &object->apiobj, &object->ai, 0xff, 1);
        } else {
            f32 fraction = object->context_animation_timer / object->airborne_action_duration;
            f32 horizontal = fraction;
            f32 arc = 0.0f;
            if (object->field_0x7aa != 0) {
                f32 rise = object->launch_origin.y - object->external_force.y;
                if (rise > 0.21f) {
                    horizontal = NU_SIN_LUT(static_cast<i32>(fraction * 16384.0f + 32768.0f + 16384.0f)) + 1.0f;
                    if (rise > 0.42f)
                        arc = rise / 3.0f + 0.0f;
                } else if (rise < -0.21f) {
                    horizontal = NU_SIN_LUT(static_cast<i32>(fraction * 16384.0f + 49152.0f + 16384.0f));
                    if (rise < -0.42f)
                        arc = 0.0f - rise / 3.0f;
                }
            }
            f32 sine = NU_SIN_LUT(static_cast<i32>(fraction * 32768.0f));
            object->apiobj.position.x =
                object->external_force.x + (object->launch_origin.x - object->external_force.x) * horizontal;
            object->apiobj.position.y =
                object->external_force.y + (object->launch_origin.y - object->external_force.y) * fraction + arc * sine;
            object->apiobj.position.z =
                object->external_force.z + (object->launch_origin.z - object->external_force.z) * horizontal;
            f32 height = sine * object->apiobj.character_data->game_character->second_jump_height;
            object->apiobj.position.y += (height + height) * object->big_jump_height;
            NuVecSub(&object->apiobj.velocity, &object->apiobj.position, &object->apiobj.start_position);
            NuVecScale(&object->apiobj.velocity, &object->apiobj.velocity, 1.0f / FRAMETIME);
            if ((object->jump_flags & 4) == 0) {
                f32 half_duration = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 0) * 0.5f;
                if (object->context_animation_timer >= half_duration) {
                    if (static_cast<u8>(object->field_0x7aa - 2) <= 1) {
                        if (LEGOACT_JUMP3 != -1 && object->apiobj.character_model->model_data_b[LEGOACT_JUMP3] != NULL)
                            object->context_animation = LEGOACT_JUMP3;
                        else if (LEGOACT_JUMP2 != -1 &&
                                 object->apiobj.character_model->model_data_b[LEGOACT_JUMP2] != NULL)
                            object->context_animation = LEGOACT_JUMP2;
                    }
                    object->jump_flags |= 4;
                }
            }
        }
    }
    if (object->character_context != -1)
        object->ai.movement_event_flags |= 2;
}

i32 ObjLandReady(GameObject_s *object) {
    if (object == NULL) {
        return 0;
    }
    if ((CInfo[object->character_context].flags & 0x1004) != 0 &&
        (LEGOCONTEXT_LAND_COMBATROLL == -1 || LEGOCONTEXT_LAND_COMBATROLL != object->character_context ||
         object->field_0x7a3 != 0)) {
        if (LEGOCONTEXT_WALLSHUFFLE != -1 && LEGOCONTEXT_WALLSHUFFLE == object->character_context) {
            return 0;
        }
        return 1;
    }
    return 0;
}

i32 UseFallAnim(GameObject_s *object) {
    const CHARACTER_CONTEXT_INFO_s &context = CInfo[object->character_context];
    return (context.flags & CHARACTER_CONTEXT_INFO_FLAG_USE_FALL_ANIMATION) != 0 && LEGOACT_FALL != -1 &&
           object->apiobj.character_model->model_data_b[LEGOACT_FALL] != NULL;
}

i32 StartFallLand(GameObject_s *object, i32 action) {
    PlayLandSfx(object, 0, 0);
    if (LEGOCONTEXT_LAND_JUMP == -1) {
        object->movement_runtime_flags &= ~4;
        return 0;
    }
    void **animations = object->apiobj.character_model->model_data_b;
    if (!(action == -1 || animations[action] == NULL)) {
        object->context_animation = action;
        if (animations[object->context_animation] == NULL) {
            object->movement_runtime_flags &= ~4;
            return 0;
        }
    } else {
        if (IsWearingBackPackFn != NULL && IsWearingBackPackFn(object) && LEGOACT_BACKPACKFALLLAND != -1 &&
            animations[LEGOACT_BACKPACKFALLLAND] != NULL) {
            object->context_animation = LEGOACT_BACKPACKFALLLAND;
        } else if (UsingExtraActionsFn != NULL && UsingExtraActionsFn(object) && LEGOACT_EXTRA_LAND2 != -1 &&
                   animations[LEGOACT_EXTRA_LAND2] != NULL) {
            object->context_animation = LEGOACT_EXTRA_LAND2;
        } else if (LEGOACT_FALLLAND != -1 && animations[LEGOACT_FALLLAND] != NULL) {
            object->context_animation = LEGOACT_FALLLAND;
        } else if (LEGOACT_LAND2 != -1 && animations[LEGOACT_LAND2] != NULL) {
            object->context_animation = LEGOACT_LAND2;
        } else {
            object->context_animation = LEGOACT_LAND;
            if (animations[object->context_animation] == NULL) {
                object->movement_runtime_flags &= ~4;
                return 0;
            }
        }
    }
    object->character_context = LEGOCONTEXT_LAND_JUMP;
    ResetAnimPacket(&object->apiobj.anim_packet, -1);
    ResetMiniAnimPacket(&object->mini_animation, -1);
    object->fall_animation_timer = 0.0f;
    object->context_animation_timer = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1);
    object->movement_runtime_flags =
        static_cast<u8>((object->movement_runtime_flags & ~0x08) | ((object->movement_runtime_flags & 0x04) << 1));
    return 1;
}

i32 StartBackFlip(GameObject_s *object) {
    if (LEGOCONTEXT_BACKFLIP == -1 || LEGOACT_BACKFLIP == -1)
        return 0;
    if (object->apiobj.character_model->model_data_b[LEGOACT_BACKFLIP] == NULL)
        return 0;
    object->character_context = LEGOCONTEXT_BACKFLIP;
    ResetAnimPacket(&object->apiobj.anim_packet, -1);
    object->context_animation = LEGOACT_BACKFLIP;
    const f32 duration = AnimDuration(object->id, LEGOACT_BACKFLIP, 0.0f, 0.0f, 1);
    object->field_0xe22 |= 0x10;
    object->context_variant_flags &= ~0x80;
    object->delayed_turn_timer = 0.0f;
    object->airborne_input_timer = 0.0f;
    object->context_animation_timer = duration;
    if (object->context_animation_timer <= 0.0f)
        object->context_animation_timer = 1.0f;
    PlayJumpSfx(object, 2);
    return 1;
}

void BackFlipCode(GameObject_s *object) {
    if (LEGOCONTEXT_BACKFLIP == -1 || object->character_context != LEGOCONTEXT_BACKFLIP) {
        return;
    }

    if ((object->field_0xe22 & 0x10) != 0 && (object->pad_gamepad->buttons_held & GAMEPAD_JUMP) == 0) {
        object->field_0xe22 &= static_cast<u8>(~0x10u);
    }

    if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL &&
        CurrentAnim(&object->apiobj.anim_packet) != object->context_animation) {
        return;
    }

    object->context_animation_timer -= FRAMETIME;
    if (object->context_animation_timer > 0.0f) {
        return;
    }

    object->character_context = -1;
    if ((object->pad_gamepad->buttons_held & GAMEPAD_JUMP) != 0 && (object->field_0xe22 & 0x10) == 0) {
        StartBackFlip(object);
    } else {
        object->apiobj.velocity.x = 0.0f;
        object->apiobj.velocity.z = 0.0f;
    }
}

i32 Slam_Start(GameObject_s *object, f32 speed) {
    if (LEGOCONTEXT_JUMP == -1)
        return 0;
    if ((object->apiobj.character_data->model_flags & 8) != 0)
        SetWeaponOut(object);
    object->character_context = LEGOCONTEXT_JUMP;
    object->action_movement_state = 4;
    object->context_animation_timer = 0.0f;
    object->context_animation = LEGOACT_SLAM;
    if (object->apiobj.player_controlled)
        object->field_0xef9 |= 8;
    object->jump_sequence = 0;
    object->apiobj.velocity.y = speed;
    PlayJumpSfx(object, 4);
    NewRumble(object->pad_gamepad->pad, 0.5f, 0);
    object->jump_flags |= 1;
    if (object->apiobj.player_controlled)
        Hint_SetComplete(0x60a);
    return 1;
}

void JumpCode(GameObject_s *object, i32 jump_pressed, i32 jump_held, u32 animation_set, i32 action_pressed,
              i32 action_held, i32 hit_effect) {
    if ((object->movement_runtime_flags & 0x10) != 0) {
        return;
    }
    WORLDINFO *world = WorldInfo_CurrentlyActive();

    const u8 player_flag = object->apiobj.flags_low & 0x80;
    i32 flip_kind = 0;
    i32 force_flip = -1; // set to 1 when a restarted jump should flip
    if (player_flag != 0 || (object->field_0xef9 & 1) != 0) {
        if (LEGOACT_BACKFLIP != -1 && object->apiobj.character_model->model_data_b[LEGOACT_BACKFLIP] != NULL)
            flip_kind = 1;
        else if ((animation_set & 2) != 0 && LEGOACT_FLIP != -1 &&
                 object->apiobj.character_model->model_data_b[LEGOACT_FLIP] != NULL)
            flip_kind = 2;
    }
    if (object->jump_reentry_timer > 0.0f)
        object->jump_reentry_timer -= FRAMETIME;
    f32 chain_timer = object->jump_chain_timer;
    if (chain_timer > 0.0f) {
        chain_timer -= FRAMETIME;
        object->jump_chain_timer = chain_timer;
    }

    if (object->character_context != -1 &&
        (object->character_context == LEGOCONTEXT_LAND_JUMP || object->character_context == LEGOCONTEXT_LAND_JUMP2 ||
         object->character_context == LEGOCONTEXT_LAND_FLIP ||
         object->character_context == LEGOCONTEXT_LAND_COMBOJUMP ||
         object->character_context == LEGOCONTEXT_LAND_LUNGE || object->character_context == LEGOCONTEXT_LAND_SLAM ||
         object->character_context == LEGOCONTEXT_LAND_COMBATROLL)) {
        if (object->character_context == LEGOCONTEXT_LAND_LUNGE && (object->context_flags & 0x40) == 0) {
            const f32 frame = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
            if (frame > 0.0f) {
                f32 *time = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
                if (time != NULL && *time >= frame) {
                    if (__builtin_expect((object->apiobj.character_data->model_flags & 8) == 0, 0))
                        Punch_Hit(object, object->force_target, PUNCHCHARGAP, PUNCHCHARGAP);
                    else
                        ComboHitFrame(object, 1);
                }
            }
        } else if (object->character_context == LEGOCONTEXT_LAND_COMBATROLL) {
            const f32 frame = AnimStopFrame(object->apiobj.character_model, object->context_animation);
            if (frame > 0.0f) {
                f32 *time = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
                if (time != NULL && frame <= *time)
                    object->field_0x7a3 = 1;
            }
        }
        if (object->character_context == LEGOCONTEXT_LAND_COMBATROLL &&
            (object->action_input_state & 0xff00ff00) == 0x01000100)
            goto jump_takeoff;
        if (jump_pressed != 0 &&
            (LEGOCONTEXT_LAND_LUNGE == -1 || object->character_context != LEGOCONTEXT_LAND_LUNGE) &&
            (LEGOCONTEXT_LAND_SLAM == -1 || object->character_context != LEGOCONTEXT_LAND_SLAM) &&
            (LEGOCONTEXT_LAND_COMBATROLL == -1 || object->character_context != LEGOCONTEXT_LAND_COMBATROLL))
            goto jump_takeoff;
        if ((LEGOCONTEXT_LAND_LUNGE == -1 || object->character_context != LEGOCONTEXT_LAND_LUNGE) &&
            (LEGOCONTEXT_LAND_SLAM == -1 || object->character_context != LEGOCONTEXT_LAND_SLAM) &&
            ((object->apiobj.flags_low & 0x80) != 0 || object->character_context != LEGOCONTEXT_LAND_JUMP ||
             object->context_animation == -1 ||
             (object->context_animation != LEGOACT_FALLLAND &&
              object->context_animation != LEGOACT_BACKPACKFALLLAND)) &&
            ((LEGOCONTEXT_LAND_COMBATROLL == -1 || object->character_context != LEGOCONTEXT_LAND_COMBATROLL) ||
             ((static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_094[2] & 8) != 0 &&
              (object->action_input_state & 0xff00ff00) != 0x01000000)) &&
            object->pad_gamepad->input_magnitude > 0.0f && (object->movement_runtime_flags & 8) == 0) {
            object->character_context = -1;
            return;
        }
        if (object->character_context == LEGOCONTEXT_LAND_COMBATROLL) {
            if (jump_pressed != 0)
                object->landing_followup = 1;
            else if (action_pressed != 0 && LEGOACT_COMBATROLL_FIRE != -1 &&
                     object->apiobj.character_model->model_data_b[LEGOACT_COMBATROLL_FIRE] != NULL)
                object->landing_followup = 2;
        }
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer <= 0.0f) {
            if (Jump_EndOfLandContextFn != NULL)
                Jump_EndOfLandContextFn(object);
            if (object->character_context == LEGOCONTEXT_LAND_COMBATROLL) {
                if (object->landing_followup == 2) {
                    object->action_suppressed = 0;
                    const u16 angle = GamePad_InputAngle(object, object->pad_gamepad);
                    object->apiobj.field_0x276 = angle;
                    object->apiobj.movement_facing_angle = angle;
                    object->apiobj.facing_angle = angle;
                    StartQuickShoot(object, LEGOACT_COMBATROLL_FIRE);
                    object->quick_shoot_timer = 0.1f;
                    return;
                }
                if (object->landing_followup == 1)
                    goto jump_takeoff;
                object->character_context = -1;
                return;
            }
            if (object->character_context == LEGOCONTEXT_LAND_LUNGE && (object->context_flags & 0x40) == 0 &&
                (object->apiobj.character_data->model_flags & 8) != 0)
                ComboHitFrame(object, 1);
            if ((object->character_context == LEGOCONTEXT_LAND_LUNGE ||
                 object->character_context == LEGOCONTEXT_LAND_SLAM) &&
                action_held != 0) {
                StartHold(object);
                return;
            }
            object->character_context = CHARACTER_CONTEXT_NONE;
        } else if (object->character_context == LEGOCONTEXT_LAND_SLAM) {
            i32 count = ParticlesPerSecond(25.0f, FRAMETIME);
            NUVEC start, end;
            FindSlamOrigin(object, &start, &end);
            AddGameDebrisRot(world->debris_sys, hit_effect, &start, ParticlesPerSecond(20.0f, FRAMETIME), 0, 0);
            auto world_debris_sys = world->debris_sys;
            while (count > 0) {
                const f32 t = qrand() * 1.5259022e-05f;
                NUVEC position;
                position.x = (end.x - start.x) * t + start.x;
                position.y = (end.y - start.y) * t + start.y;
                position.z = (end.z - start.z) * t + start.z;
                AddGameDebrisRot(world_debris_sys, hit_effect, &position, 1, 0, 0);
                --count;
            }
        }
        return;
    }

    if (LEGOCONTEXT_JUMP == -1)
        return;
    if (object->character_context != LEGOCONTEXT_JUMP) {
        if ((animation_set & 0x80) == 0 && object->fall_acceleration_timer <= 0.0f &&
            (jump_pressed != 0 || ((object->field_0xef9 & 1) != 0 && flip_kind != 0)) &&
            (object->apiobj.field_0x27d != 0 || object->ground_contact_grace_timer > 0.0f) &&
            (CInfo[object->character_context].flags & 0x1000) != 0 &&
            ((animation_set & 4) == 0 ||
             (!(chain_timer > 0.0f) && (object->apiobj.is_underwater == 0 || object->communicate_blend == 0.0f)))) {
        jump_takeoff:
            if (Jump_PreventJumpFn != NULL && Jump_PreventJumpFn(object))
                return;
            if (LEGOCONTEXT_WEAPONOUT != -1 && object->character_context == LEGOCONTEXT_WEAPONOUT)
                FastWeaponOut(object, 0);
            else if (LEGOCONTEXT_WEAPONIN != -1 && object->character_context == LEGOCONTEXT_WEAPONIN)
                FastWeaponIn(object, 0);
            if (object->apiobj.field_0x27d != 0)
                object->field_0xf03 |= 0x20;
            if (object->jump_reentry_timer <= 0.0f)
                object->jump_sequence = 0;
            if (flip_kind == 0 ||
                (!(object->delayed_turn_timer > 0.0f) && force_flip != 1 && (object->field_0xef9 & 1) == 0)) {
                object->action_movement_state = (animation_set & 4) != 0 ? 5 : 0;
                if (UsingExtraActionsFn != NULL && UsingExtraActionsFn(object) && LEGOACT_EXTRA_JUMP != -1 &&
                    object->apiobj.character_model->model_data_b[LEGOACT_EXTRA_JUMP] != NULL)
                    object->context_animation = LEGOACT_EXTRA_JUMP;
                else
                    object->context_animation = LEGOACT_JUMP;
                if (CanMagnetClimbFn != NULL && CanMagnetClimbFn(object) && LEGOACT_MAGNET_JUMP != -1 &&
                    object->apiobj.character_model->model_data_b[LEGOACT_MAGNET_JUMP] != NULL)
                    object->context_animation = LEGOACT_MAGNET_JUMP;
                object->jump_start_height = object->apiobj.collision_min.y;
                if ((animation_set & 1) == 0)
                    object->jump_sequence = 1;
                else
                    ++object->jump_sequence;
                if ((animation_set & 1) != 0 && object->jump_sequence > 1) {
                    if (UsingExtraActionsFn != NULL && UsingExtraActionsFn(object) && LEGOACT_EXTRA_JUMP2 != -1 &&
                        object->apiobj.character_model->model_data_b[LEGOACT_EXTRA_JUMP2] != NULL)
                        object->context_animation = LEGOACT_EXTRA_JUMP2;
                    else if (LEGOACT_JUMP2 != -1 && object->apiobj.character_model->model_data_b[LEGOACT_JUMP2] != NULL)
                        object->context_animation = LEGOACT_JUMP2;
                    else
                        object->context_animation = LEGOACT_JUMP;
                    object->apiobj.velocity.y =
                        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)
                            ->second_jump_speed;
                    ResetAnimPacket(&object->apiobj.anim_packet, object->context_animation);
                    object->context_variant_flags &= ~0x40;
                    if ((animation_set & 0x100) != 0)
                        MakeJumpReachHeight(object, HIGHJUMPHEIGHT, 1);
                } else {
                    if ((animation_set & 0x40) != 0 &&
                        (object->context_animation == -1 ||
                         object->apiobj.character_model->model_data_b[object->context_animation] == NULL))
                        object->context_animation = LEGOACT_FALL;
                    object->apiobj.velocity.y =
                        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->jump_speed;
                    if (object->action_movement_state != 5)
                        ResetAnimPacket(&object->apiobj.anim_packet, object->context_animation);
                    if ((animation_set & 0x101) == 0x100)
                        MakeJumpReachHeight(object, HIGHJUMPHEIGHT, 1);
                    if ((object->apiobj.flags_low & 0x80) != 0)
                        Hint_SetComplete(0x604);
                }
            } else {
                if (flip_kind == 1 && StartBackFlip(object))
                    return;
                object->action_movement_state = 1;
                object->context_animation = LEGOACT_FLIP;
                ResetAnimPacket(&object->apiobj.anim_packet, LEGOACT_FLIP);
                object->apiobj.velocity.y =
                    static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->second_jump_speed;
            }
            const u8 variant_flags = object->context_variant_flags;
            object->context_animation_timer = 0.0f;
            object->jump_flags &= ~1;
            object->character_context = LEGOCONTEXT_JUMP;
            object->context_variant_flags = variant_flags & ~0x10;
            if (object->action_movement_state == 1 && LEGOACT_FLIP != -1 &&
                object->apiobj.character_model->model_data_b[LEGOACT_FLIP] != NULL)
                object->context_animation = LEGOACT_FLIP;
            object->field_0xe22 |= 0x10;
            object->apiobj.field_0x27d = 0;
            object->field_0x105c = 0;
            object->delayed_turn_timer = 0.0f;
            if ((animation_set & 4) != 0) {
                object->jump_variant_timer =
                    static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->field_0x48;
                if ((object->apiobj.flags_low & 0x80) == 0)
                    object->jump_variant_timer *= 1.5f;
                object->field_0xe31 = 1;
            }
            object->airborne_input_timer = 0.0f;
            object->airborne_collision_target = NULL;
            object->context_variant_flags = variant_flags & 0x6f;
            PlayJumpSfx(object, object->jump_sequence > 1);
            if (object->apiobj.field_0x27f == 1 && object->apiobj.collision_min.y < object->apiobj.water_height)
                AddWaterSplash(object, &object->apiobj.collision_position);
            return;
        }
    } else {
        i8 variant_flags = object->context_variant_flags;
        if ((variant_flags & 0x10) != 0)
            object->movement_runtime_flags |= 0x80;
        if ((object->field_0xe22 & 0x10) != 0 && jump_held == 0) {
            object->field_0xe22 &= ~0x10;
        }

        const f32 vertical_speed = object->apiobj.velocity.y;
        if (vertical_speed < 0.0f) {
            object->airborne_collision_target = NULL;
        }
        const u8 movement_state = object->action_movement_state;
        if (movement_state == 5 && object->field_0xe31 == 2 && object->jump_variant_timer > 0.0f) {
            object->jump_variant_timer -= FRAMETIME;
            if (object->jump_variant_timer <= 0.0f)
                object->field_0xe31 = 3;
        }
        object->context_animation_timer += FRAMETIME;
        if (movement_state == 8 && variant_flags >= 0 &&
            object->apiobj.character_model->model_data_b[object->context_animation] != NULL &&
            object->context_animation_timer >= object->airborne_action_duration && LEGOACT_COMBATROLL_FALL != -1 &&
            object->apiobj.character_model->model_data_b[LEGOACT_COMBATROLL_FALL] != NULL) {
            variant_flags |= 0x80;
            object->context_variant_flags = variant_flags;
        }
        if ((movement_state == 0 || movement_state == 2 || movement_state == 8) &&
            object->context_animation_timer >= 2.0f && object->apiobj.start_position.y <= object->apiobj.position.y &&
            object->apiobj.field_0x27d == 0) {
            object->jump_reentry_timer = 0.0f;
            object->character_context = -1;
            object->fall_animation_timer = 0.2f;
            return;
        }

        bool touch_action = false;
        if (MechInputTouchSystem::s_actualTouchMode == 2 && player_flag != 0)
            touch_action = (object->jump_input_flags & 0x20) != 0;
        else
            object->jump_input_flags &= ~0x20;
        bool landed;
        if (movement_state == 5) {
            landed = (object->apiobj.field_0x27d & 1) != 0 ||
                     (player_flag == 0 && object->field_0x1084 != 0 && object->field_0xe31 == 3);
        } else {
            landed = (object->apiobj.field_0x27d != 0 && object->context_animation_timer >= 0.1f) ||
                     ((movement_state == 3 || movement_state == 4) && object->context_animation_timer >= 2.5f);
        }
        if (!landed) {
            GAMECHARACTERDATA *game_character =
                static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
            if ((touch_action || action_pressed != 0) && (vertical_speed > -1.25f || touch_action) &&
                variant_flags >= 0) {
                if (movement_state == 0 && object->jump_sequence < 2 &&
                    (game_character->field275_0x116 != 0 || (object->apiobj.character_data->model_flags & 8) != 0) &&
                    LEGOACT_LUNGE != -1 && object->apiobj.character_model->model_data_b[LEGOACT_LUNGE] != NULL) {
                    object->field_0x780 = NULL;
                    object->blowup_target = NULL;
                    StartLunge(object, 0.0f, object->apiobj.field_0x1e0);
                    return;
                }
            }
            if (DoubleJump_JediSlam != 0 && (animation_set & 0x10) != 0 && (touch_action || action_pressed != 0) &&
                (vertical_speed > -1.25f || touch_action) && variant_flags >= 0 &&
                ((movement_state == 0 && (object->jump_sequence == 2 || (game_character->field_0x98 & 0x20) != 0)) ||
                 movement_state == 1 || movement_state == 2) &&
                LEGOACT_SLAM != -1 && object->apiobj.character_model->model_data_b[LEGOACT_SLAM] != NULL) {
                if (Slam_Start(object, SLAMJUMPSPEED) != 0) {
                    PlaySabreSfx(NULL, object, NULL, 0);
                    if (object->action_movement_state == 1 || object->action_movement_state == 2) {
                        ResetAnimPacket(&object->apiobj.anim_packet, -1);
                        if (object->action_movement_state == 2 && (object->context_variant_flags & 0x20) == 0) {
                            object->apiobj.field_0x276 += 0x8000;
                            object->apiobj.facing_angle += 0x8000;
                            object->apiobj.movement_facing_angle += 0x8000;
                        }
                    }
                    return;
                }
            }
            const bool buffered_second_jump = (object->jump_input_flags & 0x10) != 0;
            const bool can_start_second_jump =
                (jump_pressed != 0 || buffered_second_jump) && (animation_set & 8) != 0 &&
                object->action_movement_state == PLAYER_JUMP_MOVEMENT_BASIC && object->jump_sequence <= 1 &&
                (object->apiobj.velocity.y > -1.25f || buffered_second_jump);
            if (can_start_second_jump) {
                if (LEGOCONTEXT_DOOMED != -1 && object->character_context == LEGOCONTEXT_DOOMED)
                    return;
                game_character = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                if ((game_character->flags_094[1] & 0x20) != 0) {
                    object->jump_variant_timer = 0.0f;
                    object->character_context = -1;
                    object->field_0xe31 = 1;
                    FastWeaponOut(object, 1);
                    return;
                }
                if ((animation_set & 0x110) == 0 && (game_character->flags_090 & 0x40400000) == 0)
                    return;
                if ((animation_set & 0x100) != 0) {
                    MakeJumpReachHeight(object, HIGHJUMPHEIGHT, 1);
                } else if ((animation_set & 0x200) != 0) {
                    MakeJumpReachHeight(object, game_character->jump_height, 0);
                } else if (DoubleJump_AlwaysReachJump2Height != 0 || object->apiobj.velocity.y > 0.0f) {
                    MakeJumpReachHeight(object, game_character->second_jump_height, 0);
                } else {
                    object->apiobj.velocity.y = 0.0f;
                }
                if (CanGlideFn != NULL && CanGlideFn(object) && Glide_Start(object))
                    return;
                object->jump_sequence++;
                object->context_variant_flags |= 0x40;
                game_character = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                if ((animation_set & 0x110) == 0 && (game_character->flags_090 & 0x400000) == 0) {
                    if ((game_character->flags_090 & 0x40000000) != 0) {
                        object->action_movement_state = 8;
                        object->context_animation = LEGOACT_COMBATROLL_JUMP;
                        object->context_variant_flags &= ~0x80;
                        object->airborne_action_duration =
                            AnimDuration(object->id, LEGOACT_COMBATROLL_JUMP, 0.0f, 0.0f, 1);
                    }
                } else {
                    object->action_movement_state = 0;
                    i16 action;
                    if (LEGOACT_JUMP3 != -1 && object->apiobj.character_model->model_data_b[LEGOACT_JUMP3] != NULL)
                        action = LEGOACT_JUMP3;
                    else if (UsingExtraActionsFn != NULL && UsingExtraActionsFn(object) && LEGOACT_EXTRA_JUMP2 != -1 &&
                             object->apiobj.character_model->model_data_b[LEGOACT_EXTRA_JUMP2] != NULL)
                        action = LEGOACT_EXTRA_JUMP2;
                    else if (LEGOACT_JUMP2 != -1 && object->apiobj.character_model->model_data_b[LEGOACT_JUMP2] != NULL)
                        action = LEGOACT_JUMP2;
                    else
                        action = LEGOACT_JUMP;
                    object->context_animation = action;
                    if ((object->apiobj.flags_low & 0x80) != 0)
                        Hint_SetComplete(0x606);
                }
                object->context_animation_timer = 0.0f;
                object->field_0xe22 |= 0x10;
                PlayJumpSfx(object, 1);
                if ((animation_set & 0x10) == 0) {
                    object->context_variant_flags |= 0x10;
                }
                return;
            }
            return;
        }
        if (player_flag != 0 && (object->action_movement_state != 5 || (object->apiobj.field_0x27d & 1) != 0))
            object->field_0xef9 |= 8;
        const bool restart_jump = jump_held != 0 && (object->field_0xe22 & 0x10) == 0 &&
                                  object->action_movement_state != 3 && object->action_movement_state != 4 &&
                                  object->action_movement_state != 5 && object->action_movement_state != 8;
        if (restart_jump) {
            if (object->action_movement_state == 1) {
                if (object->pad_gamepad->input_magnitude > 0.0f && (object->field_0xe22 & 0x20) != 0 &&
                    abs(RotDiff(object->apiobj.movement_facing_angle, object->current_input_angle)) > 0x6aaa)
                    force_flip = 1;
            } else if (object->action_movement_state == 2) {
                if (object->pad_gamepad->input_magnitude > 0.0f && (object->field_0xe22 & 0x20) != 0 &&
                    abs(RotDiff(object->apiobj.movement_facing_angle, object->current_input_angle)) < 0x1555)
                    force_flip = 1;
            }
        }
        {
            if (object->action_movement_state != 3) {
                if (object->action_movement_state != 4) {
                    if (object->action_movement_state == 8) {
                        if (LEGOCONTEXT_LAND_COMBATROLL != -1 &&
                            object->apiobj.character_model->model_data_b[LEGOACT_COMBATROLL_LAND] != NULL) {
                            NewRumble(object->pad_gamepad->pad, 0.4f, 0);
                            object->character_context = LEGOCONTEXT_LAND_COMBATROLL;
                            object->field_0x7a3 = 0;
                            object->context_animation = LEGOACT_COMBATROLL_LAND;
                            object->context_animation_timer =
                                AnimDuration(object->id, LEGOACT_COMBATROLL_LAND, 0.0f, 0.0f, 1);
                            object->context_flags &= ~0x40;
                            object->jump_reentry_timer = 0.0f;
                            if (!restart_jump)
                                ResetAnimPacket(&object->apiobj.anim_packet, -1);
                            object->landing_followup = 0;
                            if (jump_held != 0 && (object->field_0xe22 & 0x10) == 0)
                                object->landing_followup = 1;
                            else if (action_held != 0 && LEGOACT_COMBATROLL_FIRE != -1 &&
                                     object->apiobj.character_model->model_data_b[LEGOACT_COMBATROLL_FIRE] != NULL)
                                object->landing_followup = 2;
                            PlayLandSfx(object, 4, 0);
                        } else {
                            object->jump_reentry_timer = 0.2f;
                            object->character_context = -1;
                            PlayLandSfx(object, 0, 0);
                        }
                    } else if (object->action_movement_state == 2 && LEGOCONTEXT_LAND_COMBOJUMP != -1 &&
                               LEGOACT_COMBOLAND != -1 &&
                               object->apiobj.character_model->model_data_b[LEGOACT_COMBOLAND] != NULL) {
                        if ((object->context_variant_flags & 0x20) == 0) {
                            object->apiobj.field_0x276 += 0x8000;
                            object->apiobj.facing_angle += 0x8000;
                            object->apiobj.movement_facing_angle += 0x8000;
                            if (!restart_jump)
                                ResetAnimPacket(&object->apiobj.anim_packet, -1);
                        }
                        object->jump_reentry_timer = 0.0f;
                        if (object->pad_gamepad->input_magnitude == 0.0f) {
                            object->character_context = LEGOCONTEXT_LAND_COMBOJUMP;
                            object->context_animation =
                                (object->context_variant_flags & 0x20) != 0 && LEGOACT_FLIPLAND != -1 &&
                                        object->apiobj.character_model->model_data_b[LEGOACT_FLIPLAND] != NULL
                                    ? LEGOACT_FLIPLAND
                                    : LEGOACT_COMBOLAND;
                            object->context_animation_timer =
                                AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1);
                        } else {
                            object->character_context = -1;
                        }
                        PlayLandSfx(object, 0, 0);
                    } else if (object->action_movement_state == 1 && LEGOCONTEXT_LAND_FLIP != -1 &&
                               LEGOACT_FLIPLAND != -1 &&
                               object->apiobj.character_model->model_data_b[LEGOACT_FLIPLAND] != NULL) {
                        object->jump_reentry_timer = 0.0f;
                        if (object->pad_gamepad->input_magnitude == 0.0f) {
                            object->character_context = LEGOCONTEXT_LAND_FLIP;
                            object->context_animation = LEGOACT_FLIPLAND;
                            object->context_animation_timer = AnimDuration(object->id, LEGOACT_FLIPLAND, 0.0f, 0.0f, 1);
                            if (!restart_jump)
                                ResetAnimPacket(&object->apiobj.anim_packet, -1);
                        } else {
                            object->character_context = -1;
                        }
                        PlayLandSfx(object, 0, 0);
                    } else {
                        bool second_landing = false;
                        if (object->action_movement_state == 0 && object->jump_sequence >= 2 &&
                            LEGOCONTEXT_LAND_JUMP2 != -1) {
                            i16 action = LEGOACT_LAND2;
                            if ((object->context_variant_flags & 0x40) != 0 &&
                                (action == -1 || object->apiobj.character_model->model_data_b[action] == NULL))
                                action = LEGOACT_LAND3;
                            second_landing =
                                action != -1 && object->apiobj.character_model->model_data_b[action] != NULL;
                        }
                        if (second_landing) {
                            object->jump_reentry_timer = 0.0f;
                            if (object->pad_gamepad->input_magnitude != 0.0f) {
                                object->character_context = -1;
                            } else if (object->fall_animation_timer >= 0.2f) {
                                StartFallLand(object, -1);
                            } else {
                                object->character_context = LEGOCONTEXT_LAND_JUMP2;
                                if ((object->context_variant_flags & 0x40) != 0 && LEGOACT_LAND3 != -1 &&
                                    object->apiobj.character_model->model_data_b[LEGOACT_LAND3] != NULL)
                                    object->context_animation = LEGOACT_LAND3;
                                else if (UsingExtraActionsFn != NULL && UsingExtraActionsFn(object) &&
                                         LEGOACT_EXTRA_LAND2 != -1 &&
                                         object->apiobj.character_model->model_data_b[LEGOACT_EXTRA_LAND2] != NULL)
                                    object->context_animation = LEGOACT_EXTRA_LAND2;
                                else
                                    object->context_animation = LEGOACT_LAND2;
                                object->context_animation_timer =
                                    AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1);
                                if (!restart_jump)
                                    ResetAnimPacket(&object->apiobj.anim_packet, -1);
                            }
                        } else if ((object->action_movement_state == 0 || object->action_movement_state == 6 ||
                                    object->action_movement_state == 7 || object->action_movement_state == 9) &&
                                   LEGOCONTEXT_LAND_JUMP != -1 && LEGOACT_LAND != -1 &&
                                   object->apiobj.character_model->model_data_b[LEGOACT_LAND] != NULL &&
                                   object->pad_gamepad->input_magnitude == 0.0f) {
                            if (object->fall_animation_timer >= 0.2f) {
                                StartFallLand(object, -1);
                            } else {
                                object->character_context = LEGOCONTEXT_LAND_JUMP;
                                if (UsingExtraActionsFn != NULL && UsingExtraActionsFn(object) &&
                                    LEGOACT_EXTRA_LAND != -1 &&
                                    object->apiobj.character_model->model_data_b[LEGOACT_EXTRA_LAND] != NULL)
                                    object->context_animation = LEGOACT_EXTRA_LAND;
                                else
                                    object->context_animation = LEGOACT_LAND;
                                if (CanMagnetClimbFn != NULL && CanMagnetClimbFn(object) && LEGOACT_MAGNET_JUMP != -1 &&
                                    object->apiobj.character_model->model_data_b[LEGOACT_MAGNET_JUMP] != NULL)
                                    object->context_animation = LEGOACT_MAGNET_JUMP;
                                object->context_animation_timer =
                                    AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1);
                                object->jump_reentry_timer = object->jump_sequence > 1 ? 0.0f : 0.2f;
                                if (!restart_jump)
                                    ResetAnimPacket(&object->apiobj.anim_packet, -1);
                            }
                        } else {
                            object->jump_reentry_timer = 0.2f;
                            object->character_context = -1;
                            if (object->action_movement_state == 5)
                                object->jump_chain_timer = 0.0f;
                        }
                        PlayLandSfx(object, 0, 0);
                    }
                } else {
                    if (LEGOCONTEXT_LAND_SLAM != -1 && LEGOACT_SLAMLAND != -1 &&
                        object->apiobj.character_model->model_data_b[LEGOACT_SLAMLAND] != NULL) {
                        GameCam_Judder(GameCam, 0.5f, 0, &object->apiobj.collision_position);
                        NewRumble(object->pad_gamepad->pad, 0.75f, 0);
                        object->slam_debris_effect = hit_effect;
                        if (Slam_GetDebrisFn != NULL)
                            object->slam_debris_effect = Slam_GetDebrisFn(object, hit_effect);
                        object->character_context = LEGOCONTEXT_LAND_SLAM;
                        object->context_animation = LEGOACT_SLAMLAND;
                        const f32 duration = AnimDuration(object->id, LEGOACT_SLAMLAND, 0.0f, 0.0f, 1);
                        object->context_animation_timer = duration <= 0.0f ? 0.75f : duration;
                        object->jump_reentry_timer = 0.0f;
                        if (!restart_jump) {
                            ResetAnimPacket(&object->apiobj.anim_packet, -1);
                            ResetMiniAnimPacket(&object->mini_animation, -1);
                        }
                        PlayLandSfx(object, 2, 0);
                    } else {
                        object->character_context = -1;
                        object->jump_reentry_timer = 0.2f;
                        PlayLandSfx(object, 0, 0);
                    }
                }
            } else {
                if (!(LEGOCONTEXT_LAND_LUNGE != -1 && LEGOACT_LUNGELAND != -1 &&
                      object->apiobj.character_model->model_data_b[LEGOACT_LUNGELAND] != NULL)) {
                    if ((object->apiobj.character_data->model_flags & 8) == 0) {
                        Punch_Hit(object, object->force_target, PUNCHCHARGAP, PUNCHCHARGAP);
                        object->jump_reentry_timer = 0.2f;
                        object->character_context = -1;
                        if (object->action_movement_state == 5)
                            object->jump_chain_timer = 0.0f;
                    } else {
                        object->character_context = -1;
                        object->jump_reentry_timer = 0.2f;
                    }
                    PlayLandSfx(object, 0, 0);
                } else {
                    NewRumble(object->pad_gamepad->pad, 0.6f, 0);
                    object->context_animation = LEGOACT_LUNGELAND;
                    object->character_context = LEGOCONTEXT_LAND_LUNGE;
                    object->context_animation_timer = AnimDuration(object->id, LEGOACT_LUNGELAND, 0.0f, 0.0f, 1);
                    object->context_flags &= ~0x40;
                    object->jump_reentry_timer = 0.0f;
                    if (!restart_jump)
                        ResetAnimPacket(&object->apiobj.anim_packet, -1);
                    if ((object->apiobj.character_data->model_flags & 8) == 0 &&
                        AnimListFrame(object->apiobj.character_model, object->context_animation, 0) == 0.0f)
                        Punch_Hit(object, object->force_target, PUNCHCHARGAP, PUNCHCHARGAP);
                    PlayLandSfx(object, 1, 0);
                }
            }
        }
        if (restart_jump) {
            UpdateLastSafePosition(object);
            goto jump_takeoff;
        }
        return;
    }
}

