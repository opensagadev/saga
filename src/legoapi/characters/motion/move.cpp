// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include <stdlib.h>
#include "legoapi/actions/movement/jumping.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/hatmachine.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/actions/character/snake.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"
#include "nu2api/nu3d/nuspecial.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/gamepads.h"
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
#include "legoapi/legoapi_types.h"
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
#include "legoapi/world/world.h"
#include "legoapi/world/mission.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/numtx_inline.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/characters/motion/move_types.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numusic/sfx.h"
#include <float.h>
#include <string.h>
#include "legoapi/characters/motion/gameanim_types.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/ai/aisys/aipath.h"
#include "legoapi/ai/game/creature.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numechptr.hpp"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nu3d/nutex.h"
#include <stdio.h>
#include "gameapi/gui/apimenu.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nufile/nufile.h"
#include <math.h>
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/actions/character/streaks.h"
#include "legoapi/audio/audio.h"
#include "legoapi/menus/screens/arcade.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/core/input/timer.h"
#include "nu2api/nucore/numemory.h"
#include "legoapi/actions/combat/fighting.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/characters/core/playeritems.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/misc/utilities.h"
#include "legogame/game.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/items/base/animpacket.h"
#include "legoapi/gizmo/base/GizBuildItObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizLeverObjectInterface.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizPanelObjectInterface.h"
#include "legoapi/gizmo/base/GizTurretObjectInterface.h"
#include "legoapi/gizmo/base/HatMachineObjectInterface.h"
#include "legoapi/gizmo/base/TeleportObjectInterface.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/gizmos/object/lever.h"
#include "legoapi/items/collect/bolts.h"
#include "gameframework/saveload.h"
#include "nu2api/nu3d/nuhspecial.h"
#include "legoapi/world/levels/podrace.h"
#include <stdint.h>
#include "nu2api/nucore/nulist.h"
#include "gameapi/ai/aisys/aiscript_types.h"
#include "editor/edpath.h"
#include "batman.h"
#include "nu2api/nucore/common.h"
#include "editor/aieditor_state.h"
#include "nu2api/numath/nuplane.h"
#include "editor/aieditor_settings.h"
#include "legoapi/render/core/terrain_internal.h"
#include "nu2api/nu3d/numtl.h"
#include "editor/edpath_types.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include "legoapi/render/core/gameliball.h"
#include "legoapi/render/fx/game_deb.h"
#include <stddef.h>
#include "legoapi/actions/movement/jumping_types.h"
#include <ctype.h>
#include "legoapi/items/collect/batarang.h"
#include "legoapi/world/areas.h"
#include "legoapi/actions/character/specialmoves.h"
#include "nu2api/numusic/numusic.h"
#include "legoapi/misc/supportall.h"
#include <stdarg.h>
#include <limits.h>
#include "legoapi/actions/character/legoapi_tag.h"
#include "legoapi/actions/character/tagging.h"
#include "legoapi/actions/movement/climbing.h"
#include "legoapi/actions/movement/dropinout.h"
#include "legoapi/actions/movement/pushing.h"
#include "legoapi/ai/core/gameai.h"
#include "legoapi/characters/core/characters.h"
#include "legoapi/characters/motion/camera.h"
#include "legoapi/characters/motion/move.h"
#include "legoapi/gizmos/transport/ledge.h"
#include "legoapi/items/collect/detonator.h"
#include "legoapi/items/collect/thermaldetonators.h"
#include "legoapi/items/objects/cable.h"
#include "legoapi/items/objects/game_obj.h"
#include "legoapi/lib/camera.h"
#include "legoapi/props/objects/zipup.h"
#include "legoapi/world/levels/episodeI.h"
#include "legoapi/world/levels/episodeII.h"
#include "legoapi/world/levels/episodeIII.h"
#include "legoapi/world/levels/episodeIV.h"
#include "legoapi/world/levels/episodeVI.h"
#include "legoapi/world/levels/hub.h"

static GameObject_s *ForceBackObj = NULL;
static NUVEC *ForceBackPos = NULL;
static i32 ForceBackType = 0;
static f32 ForceBackRadius = 0.0f;
static f32 ForceBackRadius2 = 0.0f;

// The retail move.cpp initializer owns these four managed references.
MechWeakPtr<MechObjectInterface, 4> nextShootTarget(NULL);
MechWeakPtr<MechObjectInterface, 4> forceNextAttackOpponent;
MechWeakPtr<MechObjectInterface, 4> lungeTarget(NULL);
MechWeakPtr<MechObjectInterface, 4> BobaRocketTarget(NULL);

// Original action/context lookup data, with its arrays and pointers in one owner.
static CHARACTER_CONTEXT_INFO_s _CInfoTab[] = {
    {"NoContext", -1, 0x00001000, 0},
    {"Jump", -1, 0x01000000, 0},
    {"LandJump", -1, 0x00000015, 0},
    {"LandJump2", -1, 0x00000015, 0},
    {"LandFlip", -1, 0x00000015, 0},
    {"LandComboJump", -1, 0x00000015, 0},
    {"Combo", -1, 0x00800010, 1},
    {"WeaponIn", -1, 0x00001013, 0},
    {"WeaponOut", -1, 0x00001013, 0},
    {"Force", -1, 0x00000222, 0},
    {"ComboRotate", -1, 0x00000011, 0},
    {"Shoot", -1, 0x00000011, 0},
    {"Interface", -1, 0x00000033, 0},
    {"Block", -1, 0x04000010, 0},
    {"LandLunge", -1, 0x00800011, 0},
    {"LandSlam", -1, 0x00800011, 0},
    {"Teleport", -1, 0x000001b3, 12},
    {"Swipe", -1, 0x00800010, 0},
    {"Tube", -1, 0x00000408, 0},
    {"ForceThrow", -1, 0x00000012, 0},
    {"HoverUp", -1, 0x00000030, 0},
    {"Rocket", -1, 0x00000011, 0},
    {"TakeHit", -1, 0x10000031, 0},
    {"Zap", -1, 0x00000010, 0},
    {"Deactivated", -1, 0x10200031, 0},
    {"Hold", -1, 0x04000010, 0},
    {"LandSpecial", -1, 0x00000015, 0},
    {"Communicate", -1, 0x00000033, 0},
    {"ForcePush", -1, 0x00000002, 0},
    {"ForcePushed", -1, 0x00000231, 0},
    {"ForceDeflect", -1, 0x00000002, 0},
    {"ForceFrozen", -1, 0x0000000b, 0},
    {"BigJump", -1, 0x02400111, 0},
    {"BackFlip", -1, 0x00000091, 0},
    {"Recoil", -1, 0x00000009, 0},
    {"ForcedBack", -1, 0x00000008, 0},
    {"DropIn", -1, 0x00040113, 0},
    {"DropOut", -1, 0x00040113, 0},
    {"Dodge", -1, 0x00000092, 1},
    {"Punch", -1, 0x00010032, 1},
    {"Push", -1, 0x00002030, 0},
    {"PushSpinner", -1, 0x00003010, 0},
    {"LandCombatRoll", -1, 0x000000b4, 0},
    {"Turn", -1, 0x00000190, 0},
    {"Doomed", -1, 0x00400110, 0},
    {"Launch", -1, 0x00000000, 0},
    {"BuildIt", -1, 0x00000010, 0},
    {"ThrowDetonator", -1, 0x00000033, 0},
    {"Grabbed", -1, 0x00000013, 0},
    {"SpecialMoveVictim", -1, 0x00000033, 0},
    {"Roll", -1, 0x00000010, 16},
    {"UnRoll", -1, 0x00000013, 16},
    {"Slide", -1, 0x40000030, 0},
    {"BeenDragged", -1, 0x00000000, 0},
    {"ZipDown", -1, 0x00000030, 0},
    {"Loop", -1, 0x00000190, 0},
    {"Poo", -1, 0x00000033, 0},
    {"Grab", -1, 0x00000033, 0},
    {"Eaten", -1, 0x60020933, 0},
    {"BarrelRoll", -1, 0x00000190, 0},
    {"BeenTakenOver", -1, 0x600209b0, 0},
    {"GetIn", -1, 0x60400130, 0},
    {"Flatten", -1, 0x00000033, 0},
    {"Buck", -1, 0x00000033, 0},
    {"Eat", -1, 0x00000033, 0},
    {"Disorientate", -1, 0x00000100, 0},
    {"Activate", -1, 0x00200033, 0},
    {"ZappedByFloor", -1, 0x00000031, 0},
    {"Climb", -1, 0x00000431, 0},
    {"Tightrope", -1, 0x00080431, 0},
    {"WallShuffle", -1, 0x00003031, 0},
    {"Grapple", -1, 0x40000430, 0},
    {"ZipUp", -1, 0x40000430, 0},
    {"PlaceDetonator", -1, 0x00000033, 0},
    {"PickUpDetonator", -1, 0x00000031, 0},
    {"PullLever", -1, 0x00000031, 0},
    {"Float", -1, 0x00000030, 2},
    {"Signal", -1, 0x04000433, 0},
    {"Batarang", -1, 0x08000033, 0},
    {"Hang", -1, 0x00000430, 0},
    {"Glide", -1, 0x00000430, 0},
    {"Catch", -1, 0x00000013, 0},
    {"Techno", -1, 0x84000031, 0},
    {"AttractoTarget", -1, 0x04000033, 0},
    {"AttractoDeposit", -1, 0x00000033, 0},
    {"Sonar", -1, 0x00000033, 0},
    {"LedgeTerrain", -1, 0x00000431, 0},
    {"Transform", -1, 0x00000033, 0},
    {"WallJumpWait", -1, 0x00002431, 0},
    {"SuperCarry", -1, 0x00000033, 0},
    {"PushObstacle", -1, 0x00003010, 0},
    {"Stunned", -1, 0x10000031, 0},
    {"Ledge", -1, 0x00000431, 0},
    {"Security", -1, 0x00000033, 0},
    {"Ballooning", -1, 0x00100010, 0},
    {"ThrowQuick", -1, 0x00000033, 0},
    {"DieAir", -1, 0x000080b1, 0},
    {"DieGround", -1, 0x000080b1, 0},
    {"HatMachine", -1, 0x00000033, 0},
    {"Whip", -1, 0x00000033, 1},
    {"NetWait", -1, 0x00000023, 0},
};

static ACTIONINFO_s ActionInfoList[] = {
    {"?", 0x0},
    {"walk", 0x2},
    {"idle", 0x0},
    {"fire", 0x0},
    {"run", 0x4},
    {"tiptoe", 0x1},
    {"fall", 0x0},
    {"jump", 0x0},
    {"land", 0x0},
    {"idle4", 0x0},
    {"jump2", 0x0},
    {"land2", 0x0},
    {"force", 0x0},
    {"flip", 0x0},
    {"flipland", 0x0},
    {"jump3", 0x0},
    {"idle2", 0x0},
    {"weaponin", 0x0},
    {"weaponout", 0x0},
    {"combojump", 0x0},
    {"comboland", 0x0},
    {"idle3", 0x0},
    {"land3", 0x0},
    {"shoot", 0x0},
    {"run2", 0x4},
    {"interface", 0x0},
    {"weaponidle", 0x0},
    {"block1", 0x8},
    {"block2", 0x8},
    {"block3", 0x8},
    {"run3", 0x4},
    {"crawl", 0x0},
    {"lunge", 0x0},
    {"lungeland", 0x0},
    {"slam", 0x0},
    {"slamland", 0x0},
    {"open", 0x0},
    {"hover", 0x0},
    {"fly", 0x0},
    {"left", 0x0},
    {"force2", 0x0},
    {"fall2", 0x0},
    {"weaponback", 0x0},
    {"up", 0x0},
    {"pushed", 0x0},
    {"in", 0x0},
    {"out", 0x0},
    {"combo1_1", 0x0},
    {"combo1_2a", 0x0},
    {"combo1_2b", 0x0},
    {"combo1_3a", 0x0},
    {"combo1_3b", 0x0},
    {"combo1_3c", 0x0},
    {"combo1_3d", 0x0},
    {"combo2_1", 0x0},
    {"combo2_2a", 0x0},
    {"combo2_2b", 0x0},
    {"combo2_3a", 0x0},
    {"combo2_3b", 0x0},
    {"combo2_3c", 0x0},
    {"combo2_3d", 0x0},
    {"shoot2", 0x0},
    {"takehit", 0x0},
    {"takehit2", 0x0},
    {"tiptoe2", 0x1},
    {"walk2", 0x2},
    {"deactivated", 0x0},
    {"deactivated2", 0x0},
    {"deactivated3", 0x0},
    {"deactivated4", 0x0},
    {"interface2", 0x0},
    {"interface3", 0x0},
    {"interface4", 0x0},
    {"hoverup", 0x0},
    {"hoverdown", 0x0},
    {"land4", 0x0},
    {"fall3", 0x0},
    {"fall4", 0x0},
    {"fire2", 0x0},
    {"eat", 0x0},
    {"right", 0x0},
    {"walkbackwards", 0x0},
    {"punch", 0x0},
    {"push", 0x0},
    {"choked", 0x0},
    {"zapped", 0x0},
    {"punch2", 0x0},
    {"punch3", 0x0},
    {"combatroll_fire", 0x0},
    {"forwards", 0x0},
    {"fallland", 0x0},
    {"shootleft", 0x0},
    {"shootright", 0x0},
    {"shootback", 0x0},
    {"pulllever", 0x0},
    {"helmeton", 0x0},
    {"build", 0x0},
    {"idle5", 0x0},
    {"idle6", 0x0},
    {"idle7", 0x0},
    {"idle8", 0x0},
    {"attack", 0x0},
    {"throw", 0x0},
    {"pickup", 0x0},
    {"drop", 0x0},
    {"grabbed", 0x0},
    {"attacked", 0x0},
    {"slide", 0x0},
    {"communicate", 0x0},
    {"ride", 0x0},
    {"helmeton2", 0x0},
    {"throw2", 0x0},
    {"throw3", 0x0},
    {"ride2", 0x0},
    {"extra_tiptoe", 0x1},
    {"extra_walk", 0x2},
    {"extra_run", 0x4},
    {"extra_fall", 0x0},
    {"extra_idle", 0x0},
    {"extra_weaponidle", 0x0},
    {"backflip", 0x0},
    {"extra_jump", 0x0},
    {"extra_jump2", 0x0},
    {"extra_land", 0x0},
    {"extra_land2", 0x0},
    {"extra_lunge", 0x0},
    {"extra_lungeland", 0x0},
    {"extra_weaponin", 0x0},
    {"extra_weaponout", 0x0},
    {"activate", 0x0},
    {"deactivate", 0x0},
    {"walk3", 0x2},
    {"ride3", 0x0},
    {"ride4", 0x0},
    {"splat", 0x0},
    {"ride5", 0x0},
    {"climb_idle", 0x0},
    {"tightrope_idle", 0x0},
    {"tightrope_move", 0x0},
    {"wallshuffle_idle", 0x0},
    {"wallshuffle_left", 0x0},
    {"wallshuffle_right", 0x0},
    {"putdown", 0x0},
    {"float", 0x0},
    {"tightrope_geton", 0x0},
    {"target", 0x0},
    {"hang_idle", 0x0},
    {"hang_move", 0x0},
    {"glide", 0x0},
    {"punch_behind", 0x0},
    {"tightrope_getoff", 0x0},
    {"change", 0x0},
    {"throw_wait", 0x0},
    {"catch", 0x0},
    {"hack", 0x0},
    {"attract", 0x0},
    {"transfer", 0x0},
    {"sonar", 0x0},
    {"ledge_idle", 0x0},
    {"ledge_left", 0x0},
    {"ledge_right", 0x0},
    {"transform", 0x0},
    {"walljump_wait", 0x0},
    {"walljump", 0x0},
    {"supercarry_pickup", 0x0},
    {"supercarry_idle", 0x0},
    {"supercarry_walk", 0x0},
    {"supercarry_throw", 0x0},
    {"stun", 0x0},
    {"stun2", 0x0},
    {"stun3", 0x0},
    {"stunned", 0x0},
    {"stunned2", 0x0},
    {"stunned3", 0x0},
    {"security", 0x0},
    {"superpush_idle", 0x0},
    {"superpush_push", 0x0},
    {"superpush_pull", 0x0},
    {"ballooning", 0x0},
    {"throw_quick", 0x0},
    {"backpackfallland", 0x0},
    {"combatroll_jump", 0x0},
    {"combatroll_fall", 0x0},
    {"combatroll_land", 0x0},
    {"recoil", 0x0},
    {"die_air", 0x0},
    {"stun_die", 0x0},
    {"grapple_idle", 0x0},
    {"grapple_up", 0x0},
    {"grapple_down", 0x0},
    {"grapple_hang", 0x0},
    {"idle9", 0x0},
    {"idle10", 0x0},
    {"ride6", 0x0},
    {"ride7", 0x0},
    {"ride8", 0x0},
    {"ride9", 0x0},
    {"ride10", 0x0},
    {"magnet_walk_metal", 0x2},
    {"magnet_tiptoe", 0x1},
    {"magnet_walk", 0x2},
    {"magnet_run", 0x4},
    {"magnet_jump", 0x0},
    {"magnet_land", 0x0},
    {"dropin", 0x0},
    {"dropout", 0x0},
    {"climb_up", 0x0},
    {"climb_down", 0x0},
    {"climb_left", 0x0},
    {"climb_right", 0x0},
    {"ai_override1", 0x0},
    {"ai_override2", 0x0},
    {"ai_override3", 0x0},
    {"ai_override4", 0x0},
    {"supercarry_putdown", 0x0},
    {"supercarry_bash", 0x0},
    {"supercarry_jump", 0x0},
    {"supercarry_land", 0x0},
    {"supercarry_fallland", 0x0},
    {"ledge_grab", 0x0},
    {"teeter", 0x0},
    {"whip_start", 0x0},
    {"whip_crack", 0x0},
    {"whip_grab", 0x0},
    {"whip_break", 0x0},
    {"whip_swing_start", 0x0},
    {"whip_swing_swing", 0x0},
    {"whip_swing_jump", 0x0},
    {"crawl_idle", 0x0},
    {"crawl_move", 0x0},
    {"swim", 0x0},
    {"wade", 0x0},
    {"dig", 0x0},
    {"winch", 0x0},
};

CHARACTER_CONTEXT_INFO_s *CInfo = &_CInfoTab[1];
ACTIONINFO_s *ActionInfo = &ActionInfoList[1];
EXTRAACTIONDATA_s ExtraActionData[] = {
    {"run1", 3},
    {"idle1", 1},
    {"interface1", 24},
    {"deactivated1", 65},
    {"fall1", 5},
    {"fire1", 2},
    {"force1", 11},
    {"walk1", 0},
    {"tiptoe1", 4},
    {"jump1", 6},
    {"land1", 7},
    {"shoot1", 22},
    {"block", 26},
    {"takehit1", 61},
    {"punch1", 81},
    {"trooperaccess", 69},
    {"throw1", 101},
    {"hunteraccess", 70},
    {"stun1", 167},
    {"stunned1", 170},
    {"ride", 108},
    {"ride_buggy", 112},
    {"ride_gyrocopter", 131},
    {"ride_bantha", 112},
    {"ride_dewback", 131},
    {"ride_landspeeder", 132},
    {"ride_tauntaun", 134},
    {"ride_speederbike", 192},
    {"ride_heavyrepeatingcannon", 193},
    {"ride_troopercannon", 194},
    {"rideluke", 195},
    {"rideluke_running", 196},
    {NULL, 0},
};

float SLAMGRAVITY = -15.0f;
float TURNTIME = 1.0f;
static float applygravity_extrahoveroffset;

void JumpCode(GameObject_s *object, i32 jump_pressed, i32 jump_held, u32 animation_set, i32 action_pressed,
              i32 action_held, i32 special_animation);
i32 GrappleSwingMode = 1;
extern i16 id_GAMORREANGUARD;
extern i16 id_CAPTAINTARPALS;
extern "C" i16 id_BAT;
extern i16 id_WATTO;
PART_s *FindIncomingPart(void *, NUVEC *, f32, u32, f32);
u32 SpecialMove_GetFlags(i32, u32);
f32 FORCEGLOWTIME = 0.1f;
f32 ForceThrowGravity = -2.5f;
f32 ForceThrowSpeed = 5.0f;
f32 MaulF_ForceThrowSpeed = 1.0f;
f32 MaulF_ForceThrowGravity = -0.5f;
f32 DookuC_ForceThrowSpeed = 2.5f;
f32 DookuC_ForceThrowGravity = -2.5f;
f32 DEACTIVATEDTIME = 8.0f;
i32 ForcePush_SuperMindTrick;
i32 ForcePush_SuperPush;
i32 ForcePush_Waft;
i16 *objhitobj_killparts_yrot;
i32 objhitobj_noimpactsfx;
#if defined(__i386__)
#define MOVE_CLONE_ATTR1 __attribute__((noinline, noclone, regparm(1)))
#define MOVE_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define MOVE_CLONE_ATTR1 __attribute__((noinline, noclone))
#define MOVE_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif

static MOVE_CLONE_ATTR2 void CommunicateCode(GameObject_s *, i32)
    __asm__("_ZL15CommunicateCodeP12GameObject_sii.constprop.74");
static MOVE_CLONE_ATTR1 void AwkwardShapeCode(GameObject_s *)
    __asm__("_ZL16AwkwardShapeCodeP12GameObject_si.constprop.78");
// Retail keeps only the IPA-SRA clone of this function.
static MOVE_CLONE_ATTR2 void ForceCode(GameObject_s *, i32, i32) __asm__("_ZL9ForceCodeP12GameObject_siii.isra.29");
extern i16 tCHEAT_SELFDESTRUCT;
extern i16 id_BUZZDROID;
extern i16 id_NAFFDROID1, id_NAFFDROID2, id_NAFFDROID4, id_MOUSEDROID;
static i32 getvehiclehoverheight_hothbattlehack;
extern "C" i16 id_SNAKE;
extern "C" i16 id_MINIATST, id_MINIATTE;
f32 DIEAIRSPEED = 1.5f;
f32 DIEAIRJUMPSPEED = 2.0f;
extern i16 id_PKDROID, id_PITDROID;

// Multiplies the ordinary camera's per-frame position blend.  The original
// keeps this as writable camera state (default 1.0), rather than folding it
// into the socket seek rate.
f32 CamStopBlend = 1.0f;
i32 netcamera;

static GAMECHARACTERDATA *GetGameCharacterData(GameObject_s *object) {
    return object->apiobj.character_data->game_character;
}

pushblock_s *BlockInBlock(WORLDINFO_s *, pushblock_s *, i32, pushblock_s **);
void MoveBlocksOverBlock(WORLDINFO_s *, pushblock_s *, i32, nuvec_s *);
void MoveBlocks(WORLDINFO_s *world, pushblock_s *block, i32 index, nuvec_s *velocity);

static inline void CheckFallLand(GameObject_s *object) {
    if (object->character_context == -1 && object->field_0xe31 == 0 && object->apiobj.field_0x27d != 0 &&
        object->apiobj.field_0x27e == 0 &&
        (object->fall_animation_timer >= 0.2f || (object->movement_runtime_flags & 4) != 0) &&
        ((object->movement_runtime_flags & 4) != 0 || object->pad_gamepad->input_magnitude == 0.0f ||
         (!object->apiobj.player_controlled && object->apiobj.character_model->model_data_b[0x59] != NULL)))
        StartFallLand(object, -1);
    if (object->apiobj.field_0x27d != 0)
        object->movement_runtime_flags &= ~4;
}

extern f32 CurrentSpeed;

f32 speeder_smoke_rate = 5.0f;
f32 speeder_offset_speed = 2.0f;

// Retail splits the guard off as an inline head; the rest is this partial clone.
static MOVE_CLONE_ATTR1 void SelfDestructCodeBody(GameObject_s *object)
    __asm__("_ZL16SelfDestructCodeP12GameObject_si.part.10");

static inline void SelfDestructCode(GameObject_s *object, i32 pressed) {
    if (!object->apiobj.player_controlled || object->apiobj.field_0x287 != 0 || object->character_context == 0x0b ||
        object->character_context == 0x16 || object->character_context == 0x2b || pressed == 0)
        return;
    SelfDestructCodeBody(object);
}

void MoveBlocksOverBlock(WORLDINFO_s *world, pushblock_s *block, i32 excluded, nuvec_s *velocity);

extern i32 LevFlag[4];
// Original exported level control, initialized to 2 at 0x623d00.
i32 GUNSHIPAHACK = 2;

extern "C" i16 id_LANDSPEEDER;

i32 PODSPRINTDEB = 117;

// These pod tuning values are zero-initialized in the original image.
f32 PosSeekPitch[2];
f32 PODBOOSTHEIGHT;

f32 zam_smoke_rate = 10.0f;
f32 test_hoverheight;
void (*LEGO_SET_SLOWDOWNFN)(GameObject_s *);

static __attribute__((noinline)) void ShieldCode(GameObject_s *object);

static bool JediHasAction(const GameObject_s *object, JEDI_ACTION action) {
    return object->apiobj.character_model->model_data_b[action] != NULL;
}

static i32 ComboOpponent_Behind;
static f32 ComboOpponent_Range2;
static f32 PlayerOpponent_Range2;
static u8 SetComboOpponent(GameObject_s *object, f32 range, i32 update_heading, i32 mode);

static inline i32 ComboJump(GameObject_s *object) {
    if (object->apiobj.character_model->model_data_b[0x12] == NULL)
        return 0;
    SetComboOpponent(object, 1.0f, 0, 1);
    object->jump_flags &= ~1;
    object->context_animation_timer = 0.0f;
    object->delayed_turn_timer = 0.0f;
    object->airborne_input_timer = 0.0f;
    object->field_0xe22 |= 0x10;
    object->character_context = 0;
    object->apiobj.velocity.y = object->apiobj.character_data->game_character->second_jump_speed;
    object->context_variant_flags &= 0x4f;
    object->action_movement_state = 2;
    object->context_animation = 0x12;
    object->apiobj.field_0x27d = 0;
    object->field_0x105c = 0;
    object->airborne_collision_target = NULL;
    PlayJumpSfx(object, 2);
    ResetAnimPacket(&object->apiobj.anim_packet, object->context_animation);
    if (object->apiobj.player_controlled)
        object->field_0xef9 |= 8;
    return 1;
}

// Retail re-tests the three block animations at each use instead of caching a flag.
#define HAS_BLOCK_ANIMS(animations)                                                                                    \
    ((animations)[0x1a] != NULL || (animations)[0x1b] != NULL || (animations)[0x1c] != NULL)

static void FindIncomingThreats(GameObject_s *object) {
    void **animations = object->apiobj.character_model->model_data_b;
    object->incoming_bolt = HAS_BLOCK_ANIMS(animations) ? FindIncomingBolt(object, 0, 0) : NULL;
    animations = object->apiobj.character_model->model_data_b;
    GameObject_s *melee = NULL;
    if (HAS_BLOCK_ANIMS(animations)) {
        GameObject_s *target = Obj;
        for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++target) {
            if (target != object && (target->apiobj.field_0x1f8 & 0x1000) != 0 && target->apiobj.field_0x287 == 0 &&
                target->character_context == 5 && (target->context_flags & 0xc) == 0 &&
                target->force_target == object) {
                melee = target;
                break;
            }
        }
    }
    object->incoming_melee = melee;
    GameObject_s *special = NULL;
    if (((object->apiobj.character_data->model_flags & 8) != 0 || object->id == id_BODYGUARD ||
         object->id == id_IMPERIALGUARD) &&
        HAS_BLOCK_ANIMS(animations)) {
        GameObject_s *target = Obj;
        for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++target) {
            if (target != object && (target->apiobj.field_0x1f8 & 0x1000) != 0 && target->apiobj.field_0x287 == 0 &&
                target->character_context == 0x26 && target->force_target == object &&
                (target->id != id_GAMORREANGUARD || target->context_animation != 0x56 ||
                 (target->context_flags & 0x40) == 0)) {
                special = target;
                break;
            }
        }
    }
    object->incoming_special = special;
    if (object->incoming_bolt == NULL && melee == NULL) {
        if (object->apiobj.field_0x27c == -1 || !HAS_BLOCK_ANIMS(animations))
            object->incoming_part = NULL;
        else {
            f32 radius = object->apiobj.field_0x1e0 > object->apiobj.field_0x1dc ? object->apiobj.field_0x1e0
                                                                                 : object->apiobj.field_0x1dc;
            object->incoming_part = FindIncomingPart(object, &object->apiobj.collision_position, radius, 10, 0.0f);
        }
    }
    if ((object->field_0xe22 & 8) == 0 &&
        (object->incoming_bolt != NULL || object->incoming_melee != NULL || object->incoming_part != NULL) &&
        object->character_context == -1)
        object->field_0xe22 |= 8;
}

// Retail inlines the fade and step checks into each caller; this is the remaining partial clone.
static MOVE_CLONE_ATTR2 void ForceGlowCode(GameObject_s *object, i32 model)
    __asm__("_ZL13ForceGlowCodeP12GameObject_si.part.34");

// Retail splits the context test off as an inline head; the rest is this partial clone.
static MOVE_CLONE_ATTR2 void ForcePushCodeBody(GameObject_s *object, i32 held)
    __asm__("_ZL13ForcePushCodeP12GameObject_sii.isra.32.part.33");

static inline void ForcePushCode(GameObject_s *object, i32 held) {
    if (object->character_context == 0x1b)
        ForcePushCodeBody(object, held);
}

static inline __attribute__((always_inline)) void ForceThrowConsiderTarget(GameObject_s *object, i32 i, f32 &distance) {
    if (Player[i] == NULL || (Player[i]->apiobj.field_0x1f8 & 0x1001) != 0x1001 || Player[i]->apiobj.field_0x287 != 0 ||
        (Player[i]->apiobj.character_data->model_flags & 0x80000) != 0)
        return;
    f32 next_distance = NuVecDistSqr(&object->apiobj.position, &Player[i]->apiobj.position, NULL);
    if (object->force_throw_target == NULL ||
        (((object->apiobj.flags_low & 0x80) != 0 || (object->force_throw_target->apiobj.flags_low & 0x80) == 0) &&
         next_distance < distance)) {
        object->force_throw_target = Player[i];
        distance = next_distance;
    }
}

// Retail keeps only the IPA-SRA clone of this function.
static MOVE_CLONE_ATTR2 void ForceThrowCode(GameObject_s *object, i32 pressed)
    __asm__("_ZL14ForceThrowCodeP12GameObject_sii.isra.13");

// Retail keeps only the IPA-SRA clone of this function.
static MOVE_CLONE_ATTR2 void ForceDeflectCode(GameObject_s *object, i32 pressed, i32 held)
    __asm__("_ZL16ForceDeflectCodeP12GameObject_siii.isra.31");

i32 show_autojump_hint;

extern "C" {
    extern i16 id_MINIATST, id_MINIATTE;
}

// Fires the jet pack: plays the jump sound, applies the character's jump
// speed and readies a jet-pack weapon.
static inline void JetPackLaunch(GameObject_s *object) {
    object->field_0xe31 = 1;
    PlayJumpSfx(object, 0);
    GAMECHARACTERDATA *character = object->apiobj.character_data->game_character;
    object->apiobj.velocity.y = character->jump_speed;
    if (character->uses_weapon_action == 2 && object->character_context != 7)
        FastWeaponOut(object, 1);
    object->jump_variant_timer = 0.0f;
}

static i32 IsAFallAnim(i32 animation) {
    if (animation == 5)
        return 1;
    if (animation == 0x28)
        return 1;
    if (animation == 0x4b)
        return 1;
    if (animation == 0x4c)
        return 1;
    return animation == 0x74;
}

i32 SnapPosTaken(WORLDINFO_s *world, pushblock_s *, nuvec_s *position, i32 excluded_index);

void SetHeadTarget(GameObject_s *object, NUVEC *position, i8 priority, f32 time, f32 minimum_delay, f32 maximum_delay) {
    if (position != NULL) {
        if (object->head_target == NULL ||
            (position != object->head_target && object->head_target_priority <= priority)) {
            i32 random = qrand();
            f32 fraction = static_cast<f32>(random) * 1.5259022e-05f;
            object->head_target = position;
            object->head_target_priority = priority;
            f32 delay = maximum_delay * fraction + (1.0f - fraction) * minimum_delay;
            object->head_target_timer = time + delay;
            object->head_target_delay = delay;
        }
        object->head_target_position = *object->head_target;
    } else {
        object->head_target = NULL;
        object->head_target_priority = 0;
        object->head_target_timer = 0.0f;
        object->head_target_delay = 0.0f;
    }
}

void JumpCode(GameObject_s *object, i32 jump_pressed, i32 jump_held, u32 animation_set, i32 action_pressed,
              i32 action_held, i32 hit_effect);

void SetObjAsHeadTarget(GameObject_s *object, GameObject_s *target, signed char, float, float, float) {
    if (target != NULL && object != NULL && target->apiobj.character_data != NULL) {
        i32 joint = GetGameCharacterData(target)->head_locator;
        if (joint != -1)
            SetHeadTarget(object, reinterpret_cast<NUVEC *>(&target->joint_matrices[joint].m30), 2, 1.0f, 0.0f, 0.0f);
        else
            SetHeadTarget(object, &target->apiobj.collision_position, 2, 1.0f, 0.0f, 0.0f);
    }
}

// Retail splits the context test off as an inline head; the rest is this partial clone.
static MOVE_CLONE_ATTR1 void GrabCodeBody(GameObject_s *object) __asm__("_ZL8GrabCodeP12GameObject_s.part.15");

static inline void GrabCode(GameObject_s *object) {
    if (__builtin_expect(object->character_context == 0x38, 1))
        GrabCodeBody(object);
}

static AWKWARDSHAPESPHERE_s AwkwardShapeSphere[] = {
    {&id_RANCOR, 4, 0, 0.3f},    {&id_DEWBACK, 1, 1, 0.2f},  {&id_DEWBACK, 2, 0, 0.125f}, {&id_DEWBACK, 3, 0, 0.1f},
    {&id_DEWBACK, 4, 0, 0.075f}, {&id_BIGGUN, 3, 0, 0.175f}, {&id_BIGGUN, 4, 0, 0.175f},  {&id_BIGGUN, 5, 0, 0.175f},
    {&id_BANTHA, 1, 1, 0.25f},   {&id_ATAT, 7, 1, 1.2f},     {&id_CATAPULT, 1, 0, 0.25f}, {&id_FIRETRUCK, 1, 1, 0.3f},
    {&id_FIRETRUCK, 2, 1, 0.3f}, {NULL, 0, 0, 0.0f},
};

extern i16 id_MINIATST, id_MINIATTE;
extern i16 id_SKELETON;
static NUVEC poopos[2];

extern i16 id_JUMBOHOMINGDROID;
extern i16 id_GAMORREANGUARD, id_PRINCESSLEIASLAVE, id_LAMASU, id_TAUNWE;
extern i16 id_PENGUIN, id_UMBRELLA, id_GLIDEPACK;

static inline i16 JediWeaponIdle(GameObject_s *object) {
    if (((object->field_0xe22 & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) != 0 || object->field_0xe32 == 1) &&
        object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_ALT_WEAPON_IDLE] != NULL)
        return CHARACTER_ANIMATION_ALT_WEAPON_IDLE;
    return CHARACTER_ANIMATION_WEAPON_IDLE;
}

i32 GizmoFileReadGameAnimSet(GAMEANIMSET_s *set, void *world_ptr,
                             void (*read_object_data)(GAMEANIMOBJ_s *, unsigned char), unsigned char version,
                             char *prefix, char *suffix);

extern MechWeakPtr<MechObjectInterface, 4> BobaRocketTarget;
extern "C" void *AIPAthFindPathCnx(AISYS_s *, AIPATH_s *, char *, char *, i32 *);

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

f32 rocket_speed = 1.2f;

struct rtlset;

struct spacelevel_s;
struct quickboltinfo;

extern f32 ForceThrowSpeed, ForceThrowGravity;

PART_s *FindIncomingPart(void *owner, NUVEC *position, f32 radius, u32 flags, f32 range);

i32 testlaser_type;
f32 testlaser_endw = 0.1f;
f32 testlaser_sizewab = 0.01f;
f32 testlaser_sizel = 0.1f;
f32 testlaser_sizew = 0.02f;
extern i16 id_GAMORREANGUARD;
extern i16 id_MOUSEDROID;

extern "C" {
    extern i16 id_BAT;
    extern i16 id_SNAKE;
    extern i16 id_LANDSPEEDER;
    extern i32 FalconDebKey[2];
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

struct CUTSCENEPLAYERCLIP;

extern i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER;

GIZMOBLOWUP_s *GizmoBlowUp_Hit(GameObject_s *, NUVEC *, i32, f32, NUVEC *, NUVEC *, BOLT_s *, u32, u8 *);

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

extern i16 id_PKDROID;

GameObject_s *FindNearestGameObject(NUVEC *position, GameObject_s *exclude, u32 required_flags, f32 radius,
                                    f32 extra_radius, i32 animation, i32 character_id, i32 player_index,
                                    f32 *distance_squared, i32 horizontal_only, i32 (*filter)(GameObject_s *),
                                    bool first_match);

struct HINT_s;

struct MechTouchUIElement;
struct PLAYERITEM_s;
struct PLAYERITEMTYPE_s;
struct PLAYERPACKET_s;
struct SOCKPOSITION_s;
struct TouchHolder;

// --- Helpers moved from world.cpp ---

static f32 sPreResetMulA = 1.0f / 65535.0f;
static f32 sPreResetMulB = 0.03f;
static f32 sPreResetSubC = 0.015f;
static f32 sPreResetD18Scale = 0.25f;
static f32 sPreReset1048Scale = 0.5f;
static f32 sPreResetDivF = 0.21f;
static f32 sPreResetMulG = 0.035f;

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

namespace {

} // namespace

extern "C" i16 id_LANDSPEEDER;
extern i32 CUTCAMONLY, GUNSHIPAHACK, netcamera, complexsockposition_forcesock, movegamecamera_forcesock;
GameObject_s *FindNearestGameObject(NUVEC *, GameObject_s *, u32, f32, f32, i32, i32, i32, f32 *, i32,
                                    i32 (*)(GameObject_s *), bool);

static u8 disorientAxis = 7;

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

i32 IDLESPEEDINNARROWSOCKSONLY = 0;

struct FLOWBOX_s;

extern "C" {
    extern "C++" i32 (*GamePads_IgnoreInputFn)(void);
    extern "C++" FadeSystem FadeSys;
}

struct instNUGCUTSCENE_s;

// --- Cross-file entry points / id globals ---
//
extern "C" {
    void *AIPAthFindPathCnx(AISYS_s *, AIPATH_s *, char *, char *, i32 *); // legoapi/ai pathfinding
}

struct JEDIBNETPACKET_s;

// Jedi_B tuning and spawn tables (unmangled globals in the original).

extern "C" {
    void *AIPAthFindPathCnx(AISYS_s *, AIPATH_s *, char *, char *, i32 *); // original name keeps the typo
}

extern f32 testlaser_endw;
extern f32 testlaser_sizewab;
extern f32 testlaser_sizel;
extern f32 testlaser_sizew;

extern "C" {
    extern i16 id_BAT;
    extern i16 id_SNAKE;
    extern i16 id_LANDSPEEDER;
    extern i32 FalconDebKey[2];
}

struct nuhspecial_s;
struct minitrooperteam_s;
struct nuvec_s;

extern "C" void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from, char *to, i32 *direction);

extern AIACTIONDEF api_aiactiondefs[55];
extern AICONDITIONDEF api_aiconditiondefs[50];

DECOMP_ASSERT(sizeof(api_aiactiondefs) == 0x294, "API action registry size");

DECOMP_ASSERT(sizeof(api_aiconditiondefs) == 0x258, "API condition registry size");

extern "C" {
extern void (*checkantinodefns[3])(APIOBJECT *, AIANTINODE *, NUVEC *, f32);
    extern void (*checkantinodefns[3])(APIOBJECT_s *, AIANTINODE_s *, NUVEC *, f32);
}

extern "C" {
void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from_name, char *to_name, i32 *direction);
} // extern "C"

struct nupad_s;

struct EDAISHAREDPATHNODE_s;
struct AIPATH_s;
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

struct EDCREATURE_s;
struct AIPATHSYS_s;

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
    extern i16 id_BAT;
    extern i16 id_SNAKE;
    extern i16 id_LANDSPEEDER;
    extern i32 FalconDebKey[2];
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

struct GIZSPINNER_s;

namespace {

} // namespace

struct starfighter_s;
struct rtl_s;
struct rtlidata_s;

struct GAMEMESSAGE_s;
struct MENU_s;
struct NuBloomParameters;
struct STATUSPACKET_s;
struct _vum_s;
struct _vuv_s;
struct ripple_node_s;
struct ripple_set_s;
struct VuVec;

extern i16 tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);

extern i16 id_ANAKINJEDISCARRED, id_WOOKIEE, id_TWOFACE;

struct AIPACKET_s;
struct APIOBJECT_s;
struct AISYS_s;
struct AIGROUP_s;
struct nufpar_s;

extern "C" {
    extern "C++" void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
}

extern "C" {
extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId();
} // extern "C"

extern "C" {
void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from_name, char *to_name, i32 *direction);

}

struct GIZMOPICKUP_s;
struct PART_s;

struct APICHARACTERMODELLIST_s;

extern i32 (*Slam_GetDebrisFn)(GameObject_s *, i32);

extern f32 ForceThrowGravity;

static inline void RotateForceGlowMatrix(NUMTX *matrix, i32 angle) {
    const f32 cosine = NU_COS_LUT(angle);
    const f32 sine = NU_SIN_LUT(angle);
    const f32 x0 = matrix->m00;
    const f32 x1 = matrix->m10;
    const f32 x2 = matrix->m20;
    const f32 x3 = matrix->m30;
    matrix->m00 = x0 * cosine - matrix->m01 * sine;
    matrix->m01 = x0 * sine + matrix->m01 * cosine;
    matrix->m10 = x1 * cosine - matrix->m11 * sine;
    matrix->m11 = x1 * sine + matrix->m11 * cosine;
    matrix->m20 = x2 * cosine - matrix->m21 * sine;
    matrix->m21 = x2 * sine + matrix->m21 * cosine;
    matrix->m30 = x3 * cosine - matrix->m31 * sine;
    matrix->m31 = x3 * sine + matrix->m31 * cosine;
}

extern BATARANG_s Batarang[8];
extern i32 (*BlowupExFunc)(GIZMOBLOWUP_s *, i32);
extern void (*GizmoBlowUp_SfxFn)(GIZMOBLOWUP_s *, NUVEC *);
extern i32 (*GizmoBlowUp_NoTargetFn)(WORLDINFO_s *, GameObject_s *);

// Original writable flagremaptab at 0x668520, 32 pairs of 32-bit masks.
extern "C" {
extern u32 flagremaptab[32][2];
}

GIZMOBLOWUP_s *GizmoBlowUp_Hit(GameObject_s *object, NUVEC *points, i32 point_count, f32 radius, NUVEC *minimum,
                               NUVEC *maximum, BOLT_s *bolt, u32 hit_type, u8 *flags);

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern i32 FalconDebKey[2];
    extern "C++" u32 arcade_placed_stud_total;
}

extern i32 FalconDebKey[2];

extern "C" i16 id_LANDSPEEDER;

extern "C" {
    extern i16 id_BAT;
    extern i16 id_SNAKE;
    extern i16 id_LANDSPEEDER;
    extern i32 FalconDebKey[2];
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

static void MoveAnim_Manage(GameObject_s *object, f32 movement_speed, i32 allow_tiptoe, i32 weapon_variant) {
    GAMECHARACTERDATA *game_character = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);

    CHARACTER_ANIMATION animation;
    if (allow_tiptoe != 0 && movement_speed <= (game_character->tiptoe_speed + game_character->walk_speed) * 0.5f) {
        if (weapon_variant != 0 &&
            (object->weapon_scale == 1.0f || object->weapon_scale_state == WEAPON_SCALE_EXTENDING) &&
            object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_SABER_TIPTOE] != NULL) {
            object->apiobj.anim_packet.requested_animation = animation = CHARACTER_ANIMATION_SABER_TIPTOE;
        } else {
            object->apiobj.anim_packet.requested_animation = animation = CHARACTER_ANIMATION_TIPTOE;
        }
    } else if (movement_speed <= (game_character->walk_speed + game_character->run_speed) * 0.5f) {
        if (weapon_variant != 0 &&
            (object->weapon_scale == 1.0f || object->weapon_scale_state == WEAPON_SCALE_EXTENDING) &&
            object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_SABER_WALK] != NULL) {
            object->apiobj.anim_packet.requested_animation = animation = CHARACTER_ANIMATION_SABER_WALK;
        } else if (object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_BACKWARDS] != NULL &&
                   (object->field_0xefd & GAMEOBJECT_MOVEMENT_FLAG_BACKWARDS) != 0) {
            object->apiobj.anim_packet.requested_animation = animation = CHARACTER_ANIMATION_BACKWARDS;
        } else {
            object->apiobj.anim_packet.requested_animation = animation = CHARACTER_ANIMATION_WALK;
        }
    } else {
        object->apiobj.anim_packet.requested_animation = animation =
            weapon_variant != 0 &&
                    (object->weapon_scale == 1.0f || object->weapon_scale_state == WEAPON_SCALE_EXTENDING) &&
                    object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_SABER_RUN] != NULL
                ? CHARACTER_ANIMATION_SABER_RUN
                : CHARACTER_ANIMATION_RUN;
    }

    const SUIT_s *suit = static_cast<const SUIT_s *>(object->suit);
    if (suit != NULL && (suit->store_flag & SUIT_STORE_FLAG_EXTRA_MOVEMENT_ANIMATIONS) != 0 &&
        (object->movement_context_state & 0x00ffff00) != 0x00054300) {
        if (animation == CHARACTER_ANIMATION_TIPTOE &&
            object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_SUIT_TIPTOE] != NULL) {
            object->apiobj.anim_packet.requested_animation = animation = CHARACTER_ANIMATION_SUIT_TIPTOE;
        } else if (animation == CHARACTER_ANIMATION_WALK &&
                   object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_SUIT_WALK] != NULL) {
            object->apiobj.anim_packet.requested_animation = animation = CHARACTER_ANIMATION_SUIT_WALK;
        } else if (animation == CHARACTER_ANIMATION_RUN &&
                   object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_SUIT_RUN] != NULL) {
            object->apiobj.anim_packet.requested_animation = animation = CHARACTER_ANIMATION_SUIT_RUN;
        }
    }

    // The target applies this bounded fallback exactly three times. Keeping
    // the passes explicit preserves its finite walk/run alternation when a
    // model supplies none of the ordinary locomotion clips.
    if (object->apiobj.character_model->model_data_b[animation] == NULL) {
        if (animation == CHARACTER_ANIMATION_TIPTOE) {
            object->apiobj.anim_packet.requested_animation = animation = CHARACTER_ANIMATION_WALK;
        } else if (animation == CHARACTER_ANIMATION_WALK) {
            object->apiobj.anim_packet.requested_animation = animation = CHARACTER_ANIMATION_RUN;
        } else if (animation == CHARACTER_ANIMATION_RUN) {
            object->apiobj.anim_packet.requested_animation = animation = CHARACTER_ANIMATION_WALK;
        } else {
            return;
        }
    }
    if (object->apiobj.character_model->model_data_b[animation] == NULL) {
        if (animation == CHARACTER_ANIMATION_TIPTOE) {
            object->apiobj.anim_packet.requested_animation = animation = CHARACTER_ANIMATION_WALK;
        } else if (animation == CHARACTER_ANIMATION_WALK) {
            object->apiobj.anim_packet.requested_animation = animation = CHARACTER_ANIMATION_RUN;
        } else if (animation == CHARACTER_ANIMATION_RUN) {
            object->apiobj.anim_packet.requested_animation = animation = CHARACTER_ANIMATION_WALK;
        } else {
            return;
        }
    }
    if (object->apiobj.character_model->model_data_b[animation] == NULL) {
        if (animation == CHARACTER_ANIMATION_TIPTOE) {
            object->apiobj.anim_packet.requested_animation = animation = CHARACTER_ANIMATION_WALK;
        } else if (animation == CHARACTER_ANIMATION_WALK) {
            object->apiobj.anim_packet.requested_animation = animation = CHARACTER_ANIMATION_RUN;
        } else if (animation == CHARACTER_ANIMATION_RUN) {
            object->apiobj.anim_packet.requested_animation = animation = CHARACTER_ANIMATION_WALK;
        } else {
            return;
        }
    }
}

static void JumpAnimCode(GameObject_s *object) {
    if (object->context_variant_flags < 0) {
        void **animations = object->apiobj.character_model->model_data_b;
        if (object->action_movement_state == PLAYER_JUMP_MOVEMENT_COMBAT_ROLL &&
            animations[PLAYER_JUMP_ACTION_COMBAT_ROLL_FALL] != NULL) {
            object->apiobj.anim_packet.requested_animation = PLAYER_JUMP_ACTION_COMBAT_ROLL_FALL;
            return;
        }
        if (animations[PLAYER_JUMP_ACTION_FALL] != NULL) {
            object->apiobj.anim_packet.requested_animation = PLAYER_JUMP_ACTION_FALL;
            return;
        }
        object->apiobj.anim_packet.requested_animation = object->context_animation;
        return;
    }
    ANIMPACKET_s *packet = &object->apiobj.anim_packet;
    packet->requested_animation = object->context_animation;
    const u8 state = object->action_movement_state;
    if (object->context_animation != 0x49 && (state == 6 || state < 2 || state == 7 || state == 9)) {
        if (!(packet->blending == 0 && object->context_animation == packet->animation_index &&
              (packet->flags & 1) != 0)) {
            object->airborne_input_timer = 0.0f;
        } else {
            object->airborne_input_timer += FRAMETIME;
            if (object->airborne_input_timer >= 0.1f &&
                (object->nearby_floor_distance == 2000000.0f || object->nearby_floor_distance > 0.35f)) {
                object->context_variant_flags |= 0x80;
            }
        }
    }
}

__attribute__((optimize("O2", "tree-vectorize"))) static void MoveAnim_Check(GameObject_s *object) {
    if (GetAnimBlendMode() == 1)
        return;
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    i16 requested = packet.requested_animation;
    const i16 previous = packet.previous_animation;
    if (object->released_movement_animation == -1)
        goto check_hold;
reset_hold:
    object->movement_animation_hold_timer = 0.1f;
    object->held_movement_animation = -1;
check_release:
    requested = packet.requested_animation;
    {
        const u32 requested_flags = ActionInfo[requested].flags;
        if (requested == previous || (requested_flags & 7) == 0 ||
            (previous != CHARACTER_ANIMATION_ALT_IDLE && previous != CHARACTER_ANIMATION_IDLE) ||
            (requested_flags & 4) != 0)
            goto reset_release;
        if (object->released_movement_animation != -1) {
            object->movement_animation_release_timer -= FRAMETIME;
            if (object->movement_animation_release_timer <= 0.0f)
                object->movement_animation_release_timer = -1.0f;
            else
                packet.requested_animation = object->released_movement_animation;
            return;
        }
        object->movement_animation_hold_timer = 0.1f;
        if (packet.blending == 0 && (requested_flags & 3) != 0) {
            packet.requested_animation = previous;
            object->released_movement_animation = previous;
        }
        return;
    }
reset_release:
    object->movement_animation_release_timer = 0.1f;
    object->released_movement_animation = -1;
    return;
check_hold:
    if (requested == previous)
        goto reset_hold;
    {
        const u32 requested_flags = ActionInfo[requested].flags;
        if (((requested_flags & 7) == 0 && requested != CHARACTER_ANIMATION_ALT_IDLE &&
             requested != CHARACTER_ANIMATION_IDLE) ||
            (ActionInfo[previous].flags & 7) == 0 || (requested_flags & 4) != 0)
            goto reset_hold;
    }
    if (object->held_movement_animation != -1) {
        object->movement_animation_hold_timer -= FRAMETIME;
        if (object->movement_animation_hold_timer <= 0.0f) {
            object->held_movement_animation = -1;
        } else {
            packet.requested_animation = object->held_movement_animation;
            goto reset_release;
        }
    } else {
        object->movement_animation_hold_timer = 0.1f;
        if (packet.blending == 0) {
            bool retain_previous = false;
            if (previous == CHARACTER_ANIMATION_RUN) {
                retain_previous = requested == CHARACTER_ANIMATION_WALK || requested == CHARACTER_ANIMATION_TIPTOE ||
                                  requested == CHARACTER_ANIMATION_IDLE;
            } else if (previous == CHARACTER_ANIMATION_SABER_RUN) {
                const GAMECHARACTERDATA *game_character =
                    static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                const i16 alternate_idle =
                    game_character->field275_0x116 != 0
                        ? CHARACTER_ANIMATION_ALT_IDLE
                        : static_cast<i16>((object->apiobj.character_data->model_flags & 0x80) != 0 ? 118 : 25);
                retain_previous = requested == CHARACTER_ANIMATION_SABER_TIPTOE ||
                                  requested == CHARACTER_ANIMATION_SABER_WALK || requested == alternate_idle;
            }
            if (retain_previous) {
                packet.requested_animation = previous;
                object->held_movement_animation = previous;
                goto reset_release;
            }
        }
    }
    goto check_release;
}

static __attribute__((noinline)) void ShieldCode(GameObject_s *object) {
    f32 target;
    if (object->field_0xe37 == 0) {
        target = 0.0f;
    } else if (object->id == id_ZAMSSPEEDER && WORLD->area != NULL && WORLD->area == BOUNTYHUNTERPURSUIT_ADATA) {
        target = 5.0f;
    } else {
        const i32 animation = CurrentAnim(&object->apiobj.anim_packet);
        if ((object->apiobj.field_0x27d == 0 && !(object->ground_contact_grace_timer > 0.0f)) || animation == 3 ||
            animation == -1 || animation == 5 || animation == 0x23 || animation == 0x6a ||
            object->character_context == 0x17 || object->character_context == 0x3d) {
            target = 0.0f;
        } else {
            target = 1.0f;
        }
    }

    const f32 previous = object->field_0xd24;
    object->field_0xd24 = SeekLinearF(previous, target, 5.0f * FRAMETIME);
    if ((previous == 0.0f && object->field_0xd24 > 0.0f) || (previous == 1.0f && object->field_0xd24 < 1.0f)) {
        PlaySfx(const_cast<char *>("DDekaShOn"), &object->apiobj.collision_position);
    }
}

void BobaRocket_Kill(PART_s *part, i32) {
    EXPLOSION *explosion = Detonate(&part->position, 0);
    if (explosion != NULL && Arcade != 0 && part->owner != NULL &&
        (Player[0] == part->owner || Player[1] == part->owner) &&
        (part->owner->apiobj.field_0x1f8 & 0x1001) == 0x1001 && static_cast<u8>(part->owner->apiobj.field_0x27c) <= 1) {
        explosion->field_0x24 |= 0x10000;
        explosion->object = part->owner;
    }
}

void BobaRocket_Deflect(PART_s *part) {
    part->flags &= ~0x4000u;
    part->flags |= 0x80;
    NewPartRotation(part);
}

void PartImpact_Basketball(PART_s *part) {
    f32 speed = NuVecMag(&part->velocity);
    f32 volume = 0.0f;
    if (speed >= 2.5f)
        volume = 1.0f;
    else if (speed >= 0.1f)
        volume = (speed - 0.1f) / 2.4f;
    if (volume > 0.0f)
        PlaySfxAndSetVolume("BBounce", &part->position, volume);
}

void PartUpdate_Basketball(PART_s *part) {
    if (LevGizmo[0] != NULL) {
        GIZMOBLOWUP_s *blowup = static_cast<GIZMOBLOWUP_s *>(LevGizmo[0]->object);
        if ((blowup->status_flags & 0x800001) == 0 &&
            NuVecDistSqr(&blowup->position, &part->position, NULL) < 0.1f * 0.1f) {
            GizmoActivate(WORLD->gizmo_sys, LevGizmo[0], 1, 0);
            GizmoBlowupBlowup(blowup, 1, -1, -1, NULL, 1);
        }
    }
}

static void SelfDestructCodeBody(GameObject_s *object) {
    InstantKillParts(object, 1, 0.0f);
    EXPLOSION *explosion = Detonate(&object->apiobj.collision_position, 0);
    if (explosion != NULL && Arcade != 0 && static_cast<u8>(object->apiobj.field_0x27c) <= 1) {
        explosion->field_0x24 |= 0x10000;
        explosion->object = object;
    }
    KillPlayer(object, 2, 1, NULL);
    if (object->camera_screen_position.z > 0.0f)
        AddFancyMessage(TTab[tCHEAT_SELFDESTRUCT], object->camera_screen_position.x, object->camera_screen_position.y,
                        0.25f, 1.0f, 1, 0);
}

static void GrabCodeBody(GameObject_s *object) {
    f32 previous_time = object->context_animation_timer;
    f32 time = FRAMETIME + previous_time;
    object->context_animation_timer = time;
    f32 duration = object->airborne_action_duration;
    if (time >= duration) {
        object->character_context = -1;
        return;
    }
    GameObject_s *target = object->force_target;
    i16 animation = object->context_animation;
    if (animation == 0x1f) {
        if (target == NULL)
            return;
    } else if (target == NULL) {
        object->character_context = -1;
        return;
    }
    if ((object->context_flags & 0x40) != 0)
        return;
    bool hit = false;
    if (object->apiobj.character_model->model_data_b[animation] == NULL) {
        f32 halfway = duration * 0.5f;
        if (halfway > previous_time && time >= halfway)
            hit = true;
    } else {
        f32 *frame = AnimPlaying(&object->apiobj.anim_packet, animation, 1, 0);
        if (frame != NULL) {
            f32 start = AnimListFrame(object->apiobj.character_model, object->context_animation, 1);
            if (start >= 1.0f && *frame >= start) {
                f32 end = AnimListFrame(object->apiobj.character_model, object->context_animation, 2);
                if (end >= 1.0f && end >= *frame)
                    hit = true;
            }
        }
    }
    if (!hit)
        return;
    i32 joint = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->weapon_joints[0];
    if (object->apiobj.field_0x288 != 0 && joint != -1 &&
        object->apiobj.character_model->points_of_interest[joint] != NULL &&
        NuVecDistSqr(NUMTX_GET_ROW_VEC(&object->joint_matrices[joint], 3), &target->apiobj.collision_position, NULL) >
            0.7f * 0.7f)
        return;
    if (target->id == id_GAMORREANGUARD && object->context_animation == 0x1f) {
        object->context_flags |= 0x40;
        if (target->character_context == 0x1c && target->force_target != NULL) {
            target->force_target->force_target = NULL;
        }
        Player_ClearContext(target, 1);
        Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(target->player_packet));
        target->character_context = 0x39;
        target->context_animation = 5;
        target->force_target = target;
        target->force_throw_target = NULL;
        PlayDieSfx(target);
        object->field_0xe24 |= 1;
    } else if ((static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24)->flags_090 & 0x8000) ==
               0) {
        object->context_flags |= 0x40;
        GameAudio_PlaySfx(0x4a, &target->apiobj.collision_position, 0, 0);
        objhitobj_noimpactsfx = 1;
        ObjHitObj(object, target, 2, 0, 0, 1);
        u16 angle = NuAtan2D(target->apiobj.collision_position.x - object->apiobj.collision_position.x,
                             target->apiobj.collision_position.z - object->apiobj.collision_position.z);
        f32 momentum = static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24)->field_0x18;
        f32 sine = NU_SIN_LUT(angle);
        f32 cosine = NU_COS_LUT(angle);
        target->apiobj.velocity.x += sine * momentum;
        target->apiobj.velocity.z += cosine * momentum;
    }
}

static void AwkwardShapeCode(GameObject_s *object) {
    i32 joints[8] = {};
    u32 flags[8] = {};
    f32 radii[8] = {};
    if (__builtin_expect(object->apiobj.field_0x288 == 0 || object->apiobj.model_draw_result == 0, 0))
        return;
    if (WORLD->area != NULL && __builtin_expect((WORLD->area->flags & 1) != 0, 0) && object->id == id_ATAT)
        return;
    i32 count = 0;
    if (__builtin_expect(object->id == id_SPEEDERBIKE, 0)) {
        if (__builtin_expect(WORLD->current_level != SPEEDERCHASEA_LDATA || disable_narrow_socks == 0, 0))
            return;
        joints[0] = 2;
        radii[0] = 0.325f;
        count = 1;
    } else {
        for (AWKWARDSHAPESPHERE_s *sphere = AwkwardShapeSphere; sphere->character_id != NULL && count < 8; ++sphere) {
            if (*sphere->character_id == object->id) {
                joints[count] = sphere->joint;
                flags[count] = sphere->flags;
                radii[count] = sphere->radius;
                ++count;
            }
        }
    }
    if (__builtin_expect(count != 0, 0)) {
        for (i32 i = 0; i < count; ++i) {
            if (object->apiobj.character_model->points_of_interest[joints[i]] != NULL) {
                f32 radius = radii[i];
                NUVEC *position = NUMTX_GET_ROW_VEC(&object->joint_matrices[joints[i]], 3);
                PushAway(position, radius, NULL, NULL, NULL, object, 2.0f, 5);
                if (flags[i] != 0 && (object->apiobj.player_controlled || static_cast<i8>(object->field_0xf00) < 0)) {
                    NUVEC displacement;
                    NUVEC extension;
                    NUVEC hit_position;
                    NuVecSub(&displacement, position, &object->apiobj.collision_position);
                    NuVecNorm(&extension, &displacement);
                    NuVecScale(&extension, &extension, radius);
                    NuVecAdd(&displacement, &displacement, &extension);
                    if (GameRayCast(&object->apiobj.collision_position, &displacement, radius * 0.5f,
                                    TERRAINMASK_NONWEAPON | 0x1f)) {
                        NuVecAdd(&hit_position, &object->apiobj.collision_position, &displacement);
                        PushAway(&hit_position, 10.0f, NULL, NULL, object, NULL, 3.0f, 1);
                    }
                }
            }
        }
    }
}

static void LightSabreStreakCode(GameObject_s *object, i32 blade, i32 effect) {
    if (object->weapon_scale < 1.0f)
        return;
    const i8 context = object->character_context;
    if ((context == 0 && (object->action_movement_state == 4 || object->action_movement_state == 2)) || context == 4 ||
        context == 16 || (context == 13 && object->id != id_THEEMPEROR) || context == 14 ||
        (context == 31 && object->field_0x7a3 == 1) ||
        (object->apiobj.player_controlled && Cheat_PowerUpActive(object->apiobj.field_0x27c))) {
        object->sabre_flags |= 2;
    }
    if (object->id == id_IMPERIALGUARD)
        object->sabre_flags &= ~3;
    if (object->sabre_flags == 0 || object->apiobj.field_0x288 == 0 || (object->field_0xe23 & 8) == 0)
        return;
    GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    const i32 joint_a = data->streak_joints[blade][0];
    if (joint_a == -1 || object->apiobj.character_model->points_of_interest[joint_a] == NULL)
        return;
    const i32 joint_b = data->streak_joints[blade][1];
    if (joint_b == -1 || object->apiobj.character_model->points_of_interest[joint_b] == NULL)
        return;
    object->blade_states[blade] = -1;
    NUVEC points[3];
    memcpy(&points[0],
           reinterpret_cast<const u8 *>(object) + offsetof(GameObject_s, joint_matrices) + joint_a * sizeof(NUMTX) +
               offsetof(NUMTX, m30),
           sizeof(points[0]));
    points[1] = *NUMTX_GET_ROW_VEC(&object->joint_matrices[joint_b], 3);
    if ((object->sabre_flags & 2) != 0) {
        i32 colour;
        if (object->apiobj.field_0x27c != -1 && Cheat_IsOn(25))
            colour = 0;
        else if (object->apiobj.field_0x27c != -1 && Player_HasPurpleForce(object))
            colour = 3;
        else if (object->id == id_GRIEVOUS && (blade == 3 || blade == 0))
            colour = 2;
        else if (AnakinGreenSabre(object) || (object->id == id_BOB && (object->field_0xefd & 2) != 0))
            colour = 1;
        else
            colour = static_cast<i8>(GCDataList[object->id].field_0x117);
        object->blade_states[blade] = colour;
        const u8 *rgb = BladeTab[colour].colour;
        const u32 red = rgb[0], green = rgb[1], blue = rgb[2];
        if (object->apiobj.model_draw_result != 0) {
            const u32 packed = 0xff000000u | red | (green << 8) | (blue << 16);
            AddStreakPoints(points, 0.25f, packed, &object->sabre_streaks[blade][0], 0, object);
            if (object->field_0x1087 != 0 && object->field_0x1020 != 2000000.0f) {
                f32 plane = object->field_0x1020;
                if (WORLD->current_level->unknown_0cc != 2000000.0f)
                    plane = WORLD->current_level->unknown_0cc;
                NUVEC reflected[2] = {{points[0].x, plane - (points[0].y - plane), points[0].z},
                                      {points[1].x, plane - (points[1].y - plane), points[1].z}};
                AddStreakPoints(reflected, 0.25f, packed, &object->sabre_streaks[blade][1], 1, object);
            }
        }
    }
    if ((object->sabre_flags & 7) == 0)
        return;
    if (object->apiobj.field_0x27c != -1 && Cheat_IsOn(25))
        effect = 1;
    else if (object->apiobj.field_0x27c != -1 && Player_HasPurpleForce(object))
        effect = 4;
    else if (object->id == id_GRIEVOUS)
        effect = (blade == 3 || blade == 0) ? 3 : 2;
    data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    if (data->field275_0x116 == 12 && object->character_context == 5 && object->combo_stage == 2 &&
        object->context_animation == 52 && object->apiobj.character_model->points_of_interest[4] != NULL) {
        points[0] = *NUMTX_GET_ROW_VEC(&object->joint_matrices[4], 3);
        NuVecAdd(&points[1], &points[0], &object->apiobj.collision_position);
        NuVecScale(&points[1], &points[1], 0.5f);
    }
    NUVEC difference;
    f32 length = NuVecDist(&points[0], &points[1], &difference);
    points[2].x = difference.x * 0.5f + points[1].x;
    points[2].y = difference.y * 0.5f + points[1].y;
    points[2].z = difference.z * 0.5f + points[1].z;
    object->sabre_collision_radius = length * 0.25f;
    f32 extent = length * 1.5f;
    NUVEC minimum = {points[2].x - extent, points[2].y - extent, points[2].z - extent};
    NUVEC maximum = {points[2].x + extent, points[2].y + extent, points[2].z + extent};
    if ((object->sabre_flags & 4) != 0) {
        NUVEC direction;
        u16 angle;
        if (object->character_context == 16)
            angle = object->apiobj.field_0x276 + 0x8000;
        else
            angle = object->apiobj.field_0x276;
        NuVecRotateY(&direction, &v001, angle);
        NuVecScale(&difference, &direction, 0.2f);
        NuVecAdd(&points[0], &object->apiobj.collision_position, &difference);
        NuVecScale(&difference, &direction, 0.5f);
        NuVecAdd(&points[1], &object->apiobj.collision_position, &difference);
        NuVecScale(&difference, &direction, 0.35f);
        NuVecAdd(&points[2], &object->apiobj.collision_position, &difference);
        if (object->character_context == 13) {
            points[0].y = (object->apiobj.upper_position.y - object->apiobj.lower_position.y) * 0.333f +
                          object->apiobj.lower_position.y;
            points[1].y = points[2].y = points[0].y;
        }
        length = NuVecDist(&points[0], &points[1], &difference);
        points[2].x = difference.x * 0.5f + points[1].x;
        points[2].y = difference.y * 0.5f + points[1].y;
        points[2].z = difference.z * 0.5f + points[1].z;
        object->sabre_collision_radius = length * 0.25f;
        extent = length * 1.5f;
        minimum = NUVEC{points[2].x - extent, points[2].y - extent, points[2].z - extent};
        maximum = NUVEC{points[2].x + extent, points[2].y + extent, points[2].z + extent};
    }
    // The original rechecks power-up effects after the geometry callbacks.
    if ((object->sabre_flags & 5) != 0) {
        if (object->apiobj.field_0x27c != -1 && Cheat_IsOn(25))
            effect = 1;
        else if (object->apiobj.field_0x27c != -1 && Player_HasPurpleForce(object))
            effect = 4;
        else if (object->id == id_GRIEVOUS)
            effect = (blade == 3 || blade == 0) ? 3 : 2;
        GameObject_s *nearest = NULL;
        i32 nearest_point = 0;
        f32 nearest_distance = 1000000.0f;
        GameObject_s *target = Obj;
        for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++target) {
            if (target == object || (target->apiobj.field_0x1f8 & 0x1001) != 0x1001 || target->use_model_origin <= 1 ||
                target->apiobj.field_0x287 != 0 || (target->character_context & 0xfd) == 57 ||
                target->character_context == 60 || (CInfo[target->character_context].flags & 0x8000) != 0)
                continue;
            GAMECHARACTERDATA *target_data =
                static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24);
            if ((target_data->flags_090 & 0x8000) != 0 || target->apiobj.collision_min.x > maximum.x ||
                minimum.x > target->apiobj.collision_max.x || target->apiobj.collision_min.z > maximum.z ||
                minimum.z > target->apiobj.collision_max.z || target->apiobj.collision_min.y > maximum.y ||
                minimum.y > target->apiobj.collision_max.y)
                continue;
            for (i32 point = 2; point >= 0; --point) {
                if (!SphereSphereOverlapScaleY(&target->apiobj.collision_position, target->apiobj.field_0x1dc,
                                               target->apiobj.field_0x1e0, &points[point],
                                               object->sabre_collision_radius, object->sabre_collision_radius))
                    continue;
                if ((object->sabre_flags & 1) != 0) {
                    target_data = static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24);
                    if ((target->apiobj.field_0x1f8 & 2) == 0 && (target_data->flags_090 & 0x1040) == 0) {
                        NuVecSub(&difference, &target->apiobj.collision_position, &object->apiobj.collision_position);
                        if (difference.x == 0.0f && difference.z == 0.0f) {
                            difference.x = static_cast<f32>(qrand()) * (1.0f / 65535.0f) - 0.5f;
                            difference.z = static_cast<f32>(qrand()) * (1.0f / 65535.0f) - 0.5f;
                        }
                        NuVecNorm(&difference, &difference);
                        target_data = static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24);
                        const f32 walk_speed = target_data->walk_speed;
                        target->apiobj.movement_direction.z = walk_speed * difference.z + target->apiobj.velocity.z;
                        target->apiobj.movement_direction.x = difference.x * walk_speed + target->apiobj.velocity.x;
                        const f32 speed = target_data->run_speed;
                        const f32 speed_squared =
                            target->apiobj.movement_direction.x * target->apiobj.movement_direction.x +
                            target->apiobj.movement_direction.z * target->apiobj.movement_direction.z;
                        if (speed * speed < speed_squared) {
                            const f32 scale = speed / NuFsqrt(speed_squared);
                            target->apiobj.movement_direction.x *= scale;
                            target->apiobj.movement_direction.z = scale * target->apiobj.movement_direction.z;
                        }
                    }
                    AddGameDebrisRot(WORLD->debris_sys, effect, &points[point], ParticlesPerSecond(10.0f, FRAMETIME), 0,
                                     0);
                    if (sabrerubwait <= 0.0f && object->apiobj.anim_packet.blending == 0 &&
                        object->character_context != 5 && object->character_context != 16 &&
                        object->character_context != 13 && object->character_context != 14 &&
                        (CInfo[object->character_context].flags & 0x4000000) == 0 &&
                        ((CInfo[object->character_context].flags & 0x8000000) == 0 || (object->jump_flags & 2) == 0) &&
                        target->character_context != 5 && target->character_context != 16 &&
                        target->character_context != 13 && target->character_context != 14 &&
                        (CInfo[target->character_context].flags & 0x4000000) == 0 &&
                        ((CInfo[target->character_context].flags & 0x8000000) == 0 || (target->jump_flags & 2) == 0)) {
                        PlaySfx("SaberSparks", &points[point]);
                        sabrerubwait = static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 0.3f + 0.3f;
                        NewRumble(object->pad_gamepad->pad, static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 0.2f + 0.2f,
                                  0);
                        NewRumble(target->pad_gamepad->pad, static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 0.2f + 0.2f,
                                  0);
                    }
                }
                const f32 distance =
                    NuVecDistSqr(&object->apiobj.collision_position, &target->apiobj.collision_position, NULL);
                if (distance < nearest_distance) {
                    nearest_distance = distance;
                    nearest_point = point;
                    nearest = target;
                }
            }
        }
        if (nearest != NULL && (object->sabre_flags & 4) != 0) {
            AddGameDebris(WORLD->debris_sys, effect, &points[nearest_point]);
            if (!CannotKill(nearest)) {
                i32 damage = object->sabre_damage;
                if (damage != 0 && nearest->apiobj.player_controlled) {
                    damage = object->apiobj.player_controlled && Player_HasDoubleWeaponDamage(object) ? 2 : 1;
                }
                ObjHitObj(object, nearest, damage, ObjHitObj_Flags(object) | 0x100, 0, 1);
            } else {
                NewRumble(object->pad_gamepad->pad, 0.5f, 0);
                NewRumble(nearest->pad_gamepad->pad, 0.5f, 0);
            }
            return;
        }
        if ((object->apiobj.player_controlled || (object->field_0xef8 & 0x40) != 0 || object->use_action == 5) &&
            (object->sabre_flags & 4) != 0) {
            if (!object->apiobj.player_controlled && object->apiobj.model_draw_result == 0) {
                if (object->blowup_target == NULL)
                    return;
                GizmoBlowupBlowup(object->blowup_target, 1, -1, 1, NULL, 1);
                AlertSurroundingCreatures(object, &points[0]);
                return;
            }
            if (GizmoBlowUp_Hit(object, points, 3, object->sabre_collision_radius, &minimum, &maximum, NULL, 0, NULL)) {
                AddGameDebris(WORLD->debris_sys, effect, &points[0]);
                AddGameDebris(WORLD->debris_sys, effect, &points[1]);
                AddGameDebris(WORLD->debris_sys, effect, &points[2]);
                NewRumble(object->pad_gamepad->pad, 0.75f, 0);
                return;
            }
        }
    }
    if ((object->sabre_flags & 6) == 0)
        return;
    PART_s *part = HitParts(object, points, 3, object->sabre_collision_radius, &minimum, &maximum, 0x8000008);
    if (part == NULL)
        return;
    if ((part->flags & 8) == 0)
        KillPart(part, 0);
    else
        DeflectPart(part, object, ForceThrowSpeed, ForceThrowGravity, 0, effect);
    AlertSurroundingCreatures(object, &points[0]);
}

static void JediLightCode(GameObject_s *object) {
    if (__builtin_expect(object->dynamic_light_id != -1, 1)) {
        NUVEC colour;
        NUVEC position;
        if (object->character_context == 0x1b && (object->field_0xe21 & 5) == 1 && object->force_target != NULL) {
            rtlDynamicEnable(object->dynamic_light_id, 1);
            NUVEC delta;
            f32 distance =
                NuVecDist(&object->force_target->apiobj.collision_position, &object->apiobj.collision_position, &delta);
            rtlDynamicSetRadii(object->dynamic_light_id, distance * 0.5f, distance * 0.5f + 0.5f);
            colour.x = 0.0f;
            colour.y = qrand() < 0x8000 ? 2.0f : 0.25f;
            colour.z = colour.y;
            rtlDynamicSetColours(object->dynamic_light_id, &colour, NULL);
            position.x = 0.5f * delta.x + object->apiobj.pos_x;
            position.y = 0.5f * delta.y + object->apiobj.pos_y;
            position.z = 0.5f * delta.z + object->apiobj.pos_z;
            rtlDynamicSetPos(object->dynamic_light_id, &position);
            return;
        }
        if (__builtin_expect(!(object->weapon_scale > 0.0f) || object->sabre_flags == 0, 0))
            return;
        position = v000;
        if (object->apiobj.field_0x288 == 0)
            return;
        i32 point_count = 0;
        for (i32 blade = 0; blade < 4; ++blade) {
            if ((object->field_0xe23 & 8) != 0) {
                GAMECHARACTERDATA *data = GetGameCharacterData(object);
                i32 first = data->streak_joints[blade][0];
                if (first != -1 && object->apiobj.character_model->points_of_interest[first] != NULL) {
                    i32 second = data->streak_joints[blade][1];
                    if (second != -1 && object->apiobj.character_model->points_of_interest[second] != NULL) {
                        point_count += 2;
                        NuVecAdd(&position, &position, reinterpret_cast<NUVEC *>(&object->joint_matrices[first].m30));
                        NuVecAdd(&position, &position, reinterpret_cast<NUVEC *>(&object->joint_matrices[second].m30));
                    }
                }
            }
            if (object->apiobj.field_0x288 == 0)
                break;
        }
        if (point_count == 0)
            return;
        NuVecScale(&position, &position, 1.0f / point_count);
        rtlDynamicEnable(object->dynamic_light_id, 1);
        f32 radius = 3.0f * object->sabre_collision_radius * object->weapon_scale * object->apiobj.field_0xa8;
        rtlDynamicSetRadii(object->dynamic_light_id, radius + 0.25f, radius + 0.5f);
        f32 brightness = qrand() < 0x8000 ? 0.003921569f : 0.007843138f;
        i32 blade = -1;
        if (object->apiobj.field_0x27c != -1 && Cheat_IsOn(0x19))
            blade = 0;
        else if (object->apiobj.field_0x27c != -1 && Player_HasPurpleForce(object))
            blade = 3;
        else if (object->id == id_GRIEVOUS) {
            colour.x = static_cast<f32>(static_cast<u32>(BladeTab[1].colour[0]));
            colour.y = static_cast<f32>(static_cast<u32>(BladeTab[1].colour[1]));
            colour.z = static_cast<f32>(static_cast<u32>(BladeTab[1].colour[2]));
            NUVEC other;
            other.x = static_cast<f32>(static_cast<u32>(BladeTab[2].colour[0]));
            other.y = static_cast<f32>(static_cast<u32>(BladeTab[2].colour[1]));
            other.z = static_cast<f32>(static_cast<u32>(BladeTab[2].colour[2]));
            f32 phase =
                (1.0f + NU_SIN_LUT(static_cast<i32>(NuFmod(GameTimer.time_elapsed, 2.0f) * 0.5f * 65536.0f))) * 0.5f;
            colour.x = ((other.x - colour.x) * phase + colour.x) * brightness;
            colour.y = ((other.y - colour.y) * phase + colour.y) * brightness;
            colour.z = ((other.z - colour.z) * phase + colour.z) * brightness;
        } else
            blade = static_cast<i8>(GetGameCharacterData(object)->field_0x117);
        if (blade != -1) {
            colour.x = static_cast<f32>(static_cast<u32>(BladeTab[blade].colour[0])) * brightness;
            colour.y = static_cast<f32>(static_cast<u32>(BladeTab[blade].colour[1])) * brightness;
            colour.z = static_cast<f32>(static_cast<u32>(BladeTab[blade].colour[2])) * brightness;
        }
        rtlDynamicSetColours(object->dynamic_light_id, &colour, NULL);
        rtlDynamicSetColours(object->dynamic_light_id, &colour, NULL);
        rtlDynamicSetPos(object->dynamic_light_id, &position);
    }
}

static void ShootThisFrame(GameObject_s *object, i32 bolt_id, i32 flags) {
    if (object == Player[0] && nextShootTarget.Get() != NULL) {
        NuMechPtr<MechObjectInterface, 4> empty_target;
        nextShootTarget = empty_target;
    }
    if ((object->apiobj.field_0x1f4 & 0x40000) != 0 && object->apiobj.field_0x27c != -1)
        return;
    if ((object->apiobj.character_data->model_flags & 0x2000) != 0) {
        NewBuzzFrames(object->pad_gamepad->pad, 1, 0);
    } else {
        NewBuzzFrames(object->pad_gamepad->pad, 2, 0);
    }
    object->field_0xef9 |= 8;
    object->quick_shoot_bolt_id = bolt_id;
    object->quick_shoot_flags = flags;
    if ((object->apiobj.character_data->game_character->flags_098[0] & 2) != 0) {
        extern void SetWeaponIn(GameObject_s *);
        SetWeaponIn(object);
    }
    if (object->field_0x7e4 != NULL && object->field_0x7e4[8] == 2 && object->field_0x7e8 != 0)
        --object->field_0x7e8;
}

void BobaRocket_Move(PART_s *part, f32 elapsed) {
    NUVEC delta;
    NUVEC next;
    NUVEC scale;
    GameObject_s *recipient = part->recipient;
    i32 spin;
    if (static_cast<i8>(part->active) < 0) {
        NuVecSub(&delta, reinterpret_cast<NUVEC *>(part->pad_0bc), &part->position);
        i32 yaw = NuAtan2D(delta.x, delta.z);
        NuVecRotateY(&delta, &delta, -yaw);
        i32 pitch = -NuAtan2D(delta.y, delta.z);
        part->rotation_x = SeekRot(part->rotation_x, pitch, 8.0f);
        part->rotation_y = SeekRot(part->rotation_y, yaw, 8.0f);
        spin = static_cast<i32>(SeekValF(static_cast<f32>(part->field_124[3]), 0.0f, 1.0f));
        part->field_124[3] = spin;
    } else {
        VuVec target;
        bool has_target = false;
        if (recipient != NULL) {
            target.w = 1.0f;
            f32 target_z = recipient->apiobj.collision_position.z;
            f32 target_y = recipient->apiobj.collision_position.y;
            f32 target_x = recipient->apiobj.collision_position.x;
            target.x = target_x;
            target.y = target_y;
            target.z = target_z;
            has_target = true;
        } else if (BobaRocketTarget.Get() != NULL) {
            BobaRocketTarget.Get()->GetPos(target, -1);
            has_target = true;
        }
        if (has_target) {
            NuVecSub(&delta, &target.xyz, &part->position);
            f32 horizontal_squared = delta.x * delta.x + delta.z * delta.z;
            i32 yaw = NuAtan2D(delta.x, delta.z);
            NuVecRotateY(&delta, &delta, -yaw);
            delta.y = target.y + 0.5f - part->position.y;
            i32 pitch = -NuAtan2D(delta.y, delta.z);
            part->rotation_x = SeekRot(part->rotation_x, pitch, 1.0f);
            part->rotation_y = SeekRot(part->rotation_y, yaw, 4.0f);
            if (horizontal_squared < 1.0f) {
                part->active |= 0x80;
                NuVecSub(&delta, &target.xyz, &part->position);
                NuVecScale(&delta, &delta, 5.0f);
                NuVecAdd(reinterpret_cast<NUVEC *>(part->pad_0bc), &part->position, &delta);
                part->recipient = NULL;
                BobaRocketTarget = NULL;
                part->field_100 = NuFsqrt(horizontal_squared + delta.y * delta.y) / rocket_speed;
            }
        }
        spin = 60000;
        part->field_124[3] = spin;
    }

    part->velocity.x = 0.0f;
    part->velocity.y = 0.0f;
    part->velocity.z = rocket_speed;
    part->field_13c += static_cast<i32>(static_cast<f32>(spin) * FRAMETIME);
    NuVecRotateX(&part->velocity, &part->velocity, part->rotation_x);
    NuVecRotateY(&part->velocity, &part->velocity, part->rotation_y);
    next.x = part->position.x + part->velocity.x * elapsed;
    next.y = part->position.y + part->velocity.y * elapsed + part->gravity * elapsed;
    next.z = part->position.z + part->velocity.z * elapsed;
    NuMtxSetRotationX(&part->transform, NuAngAdd(part->rotation_x, 0x4000));
    NuMtxRotateY(&part->transform, part->rotation_y);
    NuMtxPreRotateY(&part->transform, part->field_13c);
    NuMtxTranslate(&part->transform, &next);
    if (part->scale_time < 0.2f) {
        f32 factor = part->scale_time / 0.2f;
        scale.x = factor;
        scale.y = factor;
        scale.z = factor;
        NuMtxPreScale(&part->transform, &scale);
    }
    if (static_cast<i8>(part->active) < 0) {
        i32 count = ParticlesPerFrame(1.0f, FRAMETIME);
        delta.x = -part->velocity.x;
        delta.y = -part->velocity.y;
        delta.z = -part->velocity.z;
        AddGameDebrisMom(WORLD->debris_sys, 11, &next, count, &delta);
    }
}

static __attribute__((noinline)) void ForcePushed_MoveCode(GameObject_s *object) {
    NuFmax(NuFmax(0.4f, 1.2f), 0.8f);
    if (object->character_context != 0x1c)
        return;
    GameObject_s *source = object->force_target;
    if (__builtin_expect(source == NULL || source->character_context != 0x1b || (source->field_0xe21 & 1) != 0 ||
                             (object->action_flags & 0x280) != 0,
                         1))
        return;
    if (__builtin_expect(object->action_movement_state == 4 && Cheat_IsOn(0x13), 0)) {
        if ((source->apiobj.collision_identity_mask & object->apiobj.collision_contact_mask) != 0) {
            i16 angle =
                NuAtan2D(source->apiobj.pos_x - object->apiobj.pos_x, source->apiobj.pos_z - object->apiobj.pos_z);
            objhitobj_killparts_yrot = &angle;
            if (ObjHitObj(NULL, object, -1, 0, 0, 1) == 2) {
                NewRumble(source->pad_gamepad->pad, 0.5f, 0);
                NewBuzz(source->pad_gamepad->pad, 0.1f, 0);
                GameCam_HitJudder();
            }
        }
    } else if (__builtin_expect(!!(ForcePush_SuperPush), 0)) {
        GameObject_s *target = Obj;
        for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++target) {
            if ((target->apiobj.field_0x1f8 & 0x1001) != 0x1001 || target->apiobj.field_0x287 != 0 ||
                target == source || target == object || target->apiobj.field_0x27c != -1 ||
                (target->field_0xefb & 8) != 0 || CannotKill(target) ||
                (target->apiobj.character_data->model_flags & 0x4002010) != 0x10 ||
                (static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24)->flags_090 & 0x40) !=
                    0 ||
                (static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24)->flags_094[1] & 2) != 0)
                continue;
            if (target->id == id_GONKDROID) {
                if (target->character_context == 0x17)
                    continue;
                if ((target->apiobj.collision_identity_mask & object->apiobj.collision_contact_mask) == 0)
                    continue;
                DeactivatePlayer(target, DEACTIVATEDTIME, NULL);
            } else {
                if ((target->apiobj.collision_identity_mask & object->apiobj.collision_contact_mask) == 0)
                    continue;
                i16 angle =
                    NuAtan2D(target->apiobj.pos_x - object->apiobj.pos_x, target->apiobj.pos_z - object->apiobj.pos_z);
                objhitobj_killparts_yrot = &angle;
                if (ObjHitObj(NULL, target, -1, 0, 0, 1) != 2)
                    continue;
                if (static_cast<u8>(source->apiobj.field_0x27c) < 2 && target->apiobj.field_0x27c == -1 && Arcade)
                    Arcade_AIKilled(source->apiobj.field_0x27c);
            }
            NewRumble(source->pad_gamepad->pad, 0.5f, 0);
            NewBuzz(source->pad_gamepad->pad, 0.1f, 0);
            GameCam_HitJudder();
        }
    }
}

static void ForceThrowCode(GameObject_s *object, i32 pressed) {
    if (object->character_context == 0x12) {
        AlertSurroundingCreatures(object, &object->apiobj.collision_position);
        if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL &&
            CurrentAnim(&object->apiobj.anim_packet) != object->context_animation)
            return;
        f32 previous_time = object->context_animation_timer;
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer <= 0.0f) {
            object->character_context = -1;
            object->gizforce_target = NULL;
            return;
        }
        NewRumble(object->pad_gamepad->pad, qrand() * 1.5259022e-05f * 0.5f, 0);
        PART_s *part = static_cast<PART_s *>(object->field_0x788);
        if (part == NULL) {
            GIZFORCE_s *force = object->gizforce_target;
            if (force != NULL) {
                object->field_0xe22 |= 2;
                if ((object->context_flags & 0x40) == 0) {
                    if (object->gizforce_target_object == NULL) {
                        object->force_glow_candidate = force;
                        object->force_glow_candidate_kind = 0;
                        SetHeadTarget(object, &force->file_position, 1, 2.0f, 5.0f, 10.0f);
                    } else {
                        object->force_glow_candidate = object->gizforce_target_object;
                        object->force_glow_candidate_kind = 1;
                        object->field_0xd8c = force->horizontal_range;
                        SetHeadTarget(object, NuSpecialGetDrawPos(&object->gizforce_target_object->special), 1, 2.0f,
                                      5.0f, 10.0f);
                    }
                    SetHeadTarget(object, &force->file_position, 1, 2.0f, 5.0f, 10.0f);
                    object->force_heading = GizForces_AngleToForce(&object->apiobj.position, object->gizforce_target);
                } else if (force->anim_set != NULL && force->anim_set->object_count == 1) {
                    part = Part_FindFromHSpecial(&force->anim_set->objects->special);
                    if (part != NULL) {
                        object->force_glow_candidate = part;
                        object->force_glow_candidate_kind = 3;
                        SetHeadTarget(object, &part->position, 2, 1.0f, 0.0f, 0.0f);
                        object->force_heading = NuAtan2D(part->position.x - object->apiobj.position.x,
                                                         part->position.z - object->apiobj.position.z);
                        object->force_glow_object = object->force_glow_candidate;
                        object->force_glow_kind = object->force_glow_candidate_kind;
                    }
                }
            }
        } else if ((part->active & 1) == 0)
            object->character_context = -1;
        else {
            object->force_heading =
                NuAtan2D(part->position.x - object->apiobj.position.x, part->position.z - object->apiobj.position.z);
            SetHeadTarget(object, &part->position, 2, 1.0f, 0.0f, 0.0f);
        }
        if (!(previous_time > 1.0f) || !(object->context_animation_timer <= 1.0f))
            return;
        f32 speed, gravity;
        if (WORLD->current_level == MAULF_LDATA) {
            speed = MaulF_ForceThrowSpeed;
            gravity = MaulF_ForceThrowGravity;
        } else if (WORLD->current_level == DOOKUC_LDATA) {
            speed = DookuC_ForceThrowSpeed;
            gravity = DookuC_ForceThrowGravity;
        } else {
            speed = ForceThrowSpeed;
            gravity = ForceThrowGravity;
        }
        object->field_0x788 = GizForce_Throw(object, object->gizforce_target, speed, gravity, 1);
        object->context_flags |= 0x40;
        return;
    }
    if (!pressed && (object->pad_gamepad->allocated_5a & 4) == 0)
        return;
    if ((object->apiobj.field_0x1f8 & 0x180) == 0x80)
        GizForce_FindBestForceTarget(WORLD->giz_force_sys, object);
    if (object->gizforce_target == NULL || (object->gizforce_target->config_flags & 0x80) == 0)
        return;
    object->character_context = 0x12;
    object->context_animation = 0x27;
    if (object->apiobj.character_model->model_data_b[0x27] == NULL)
        object->context_animation = 0xb;
    object->context_flags &= ~0x40;
    object->field_0x788 = NULL;
    object->force_throw_target = NULL;
    object->context_animation_timer = 2.0f;
    // Retail unrolls the scan over all eight player slots.
    f32 distance = 1.0e9f;
    ForceThrowConsiderTarget(object, 0, distance);
    ForceThrowConsiderTarget(object, 1, distance);
    ForceThrowConsiderTarget(object, 2, distance);
    ForceThrowConsiderTarget(object, 3, distance);
    ForceThrowConsiderTarget(object, 4, distance);
    ForceThrowConsiderTarget(object, 5, distance);
    ForceThrowConsiderTarget(object, 6, distance);
    ForceThrowConsiderTarget(object, 7, distance);
    NewRumble(object->pad_gamepad->pad, 0.6f, 0);
    object->gizforce_target->using_object = object;
    GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
    object->force_heading = GizForces_AngleToForce(&object->apiobj.position, object->gizforce_target);
}

static void PooCode(GameObject_s *object) {
    if (WORLD->lev_objs[0x18].active != 0) {

        NUVEC position;
        auto *game_character = object->apiobj.character_data->game_character;
        i32 locator = game_character->poo_locator;
        if (locator != -1 && object->apiobj.character_model->points_of_interest[locator] != NULL) {
            position = *reinterpret_cast<NUVEC *>(&object->joint_matrices[locator].m30);
        } else {
            u16 angle = object->apiobj.field_0x276;
            position.x = object->apiobj.collision_position.x - NU_SIN_LUT(angle) * object->apiobj.field_0x1dc;
            position.y = object->apiobj.collision_position.y;
            position.z = object->apiobj.collision_position.z - NU_COS_LUT(angle) * object->apiobj.field_0x1dc;
        }

        if (object->character_context == 0x37) {
            f32 previous = object->context_animation_timer;
            object->context_animation_timer = previous + FRAMETIME;
            if (object->context_animation_timer >= object->airborne_action_duration) {
                object->character_context = -1;
            } else if (previous < object->airborne_action_duration * 0.5f &&
                       object->context_animation_timer >= object->airborne_action_duration * 0.5f) {
                PlaySfx("Lego_Poo", &position);
                NUVEC velocity;
                velocity.x = (f32)qrand() * (1.0f / 65535.0f) - 0.5f - NU_SIN_LUT(object->apiobj.field_0x276);
                velocity.y = (f32)qrand() * (1.0f / 65535.0f) - 0.5f;
                velocity.z = (f32)qrand() * (1.0f / 65535.0f) - 0.5f - NU_COS_LUT(object->apiobj.field_0x276);
                if (Cheat_IsOn(9)) {
                    NUVEC direction;
                    NuVecNorm(&direction, &velocity);
                    f32 random = (f32)qrand() * (1.0f / 65535.0f);
                    i32 count = random < 0.2f ? 1000 : random < 0.8f ? 100 : 10;
                    AddPickups(count, 0, 0, 0, &position, &direction, 1.0f, -1, 2000000.0f, 0.0f, object, 0, 0, true);
                }
                if (Cheat_IsOn(1)) {
                    ADDPART_ALIGNED16 params = Default_ADDPART;
                    NUMTX_ALIGNED16 matrix;
                    i32 angle_x = qrand();
                    f32 sinx = NU_SIN_LUT(angle_x);
                    f32 cosx = NU_COS_LUT(angle_x);
                    matrix.m00 = 1.0f;
                    matrix.m01 = 0.0f;
                    matrix.m02 = 0.0f;
                    matrix.m03 = 0.0f;
                    matrix.m10 = 0.0f;
                    matrix.m11 = cosx;
                    matrix.m12 = sinx;
                    matrix.m13 = 0.0f;
                    matrix.m20 = 0.0f;
                    matrix.m21 = -sinx;
                    matrix.m22 = cosx;
                    matrix.m23 = 0.0f;
                    matrix.m30 = 0.0f;
                    matrix.m31 = 0.0f;
                    matrix.m32 = 0.0f;
                    matrix.m33 = 1.0f;

                    NuMtxRotateYInline(&matrix, qrand());
                    NuMtxRotateZInline(&matrix, qrand());

                    NuMtxTranslate(&matrix, &position);
                    NUVEC radius_position;
                    NuSpecialGetRadius(&WORLD->lev_objs[0x18].special, &radius_position, &params.field_14);
                    params.matrix = &matrix;
                    params.velocity = &velocity;
                    params.owner = object;
                    params.field_18 = params.field_14;
                    params.gravity = -6.0f;
                    params.special = &WORLD->lev_objs[0x18].special;
                    params.field_28 = 0x18;
                    params.flags = 0x390;
                    params.draw_fn = PartDraw_Flickerer;
                    params.stop_fn = PartStop_Flickerer;
                    params.time_step = FRAMETIME;
                    params.field_a4 = 5.0f;
                    PART_s *part = AddPart(&params);
                    if (part != NULL) {
                        part->owner = object;
                        part->force_flags = static_cast<u16>(ObjHitObj_Flags(object));
                    }
                }
            }
        } else if ((object->pad_gamepad->buttons_pressed & GAMEPAD_SPECIAL) != 0 &&
                   static_cast<i8>(game_character->flags_094[3]) < 0 && object->character_context == -1 &&
                   object->apiobj.field_0x27d != 0 && object->apiobj.player_controlled &&
                   (Cheat[1].enabled != 0 || Cheat[9].enabled != 0)) {
            object->character_context = 0x37;
            object->context_animation = 1;
            object->context_animation_timer = 0.0f;
            object->airborne_action_duration = 0.6f;
        }

        if ((object->pad_gamepad->buttons_pressed & GAMEPAD_SPECIAL) == 0 ||
            ((object->field_0xf00 & 0x10) == 0 || static_cast<u8>(object->apiobj.field_0x27c) > 1)) {
            object->force_use_volume = 0.0f;
            return;
        }
        PART_s *part = FindPart(NULL, -1, object);
        object->force_use_volume = SeekLinearF(object->force_use_volume, part != NULL ? 1.0f : 0.0f, FRAMETIME);
        if (object->force_use_volume > 0.0f) {
            poopos[object->apiobj.field_0x27c] = position;
            PlaySfxAndSetVolume("FliesLp", &poopos[object->apiobj.field_0x27c], object->force_use_volume);
        }
    }
}

void ForceNextShootTarget(MechObjectInterface &target) {
    nextShootTarget = NuMechPtr<MechObjectInterface, 4>(&target);
}

i32 ObjZappedBlue(GameObject_s *object) {
    if (object->field_0x7a5 == 0x42)
        return 1;
    if (object->field_0x7a5 == 0x1c) {
        GameObject_s *holder = static_cast<GameObject_s *>(object->field_0x780);
        if (holder != NULL && (holder->field_0xe21 & 1) != 0)
            return 1;
    }
    return 0;
}

float VehicleTurnOrLoopOffset(GameObject_s *object) {
    if (object->character_context == 0x2a) {
        return (1.0f -
                NU_SIN_LUT((1.0f - object->context_animation_timer / object->airborne_action_duration) * 65536.0f +
                           16384.0f)) *
               0.5f * static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->field_0x88 * 0.5f;
    } else if (object->character_context == 0x36) {
        return (1.0f -
                NU_SIN_LUT((1.0f - object->context_animation_timer / object->airborne_action_duration) * 65536.0f +
                           16384.0f)) *
               0.5f * static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->field_0x88;
    } else if (object->character_context == 0x3a) {
        if (object->apiobj.player_controlled && PODSPRINT_ADATA != NULL && PODSPRINT_ADATA == WORLD->area) {
            return NU_SIN_LUT((1.0f - object->context_animation_timer / object->airborne_action_duration) * 32768.0f) *
                   0.25f;
        }
    }
    return 0.0f;
}

float GetHoverPosY(GameObject_s *obj) {
    if ((obj->apiobj.character_data->model_flags & 0x2000) == 0 ||
        (WorldInfo_CurrentlyActive()->current_level->flags & LEVEL_IN_SPACE) != 0) {
        return 2000000.0f;
    }

    const f32 vehicle_offset = VehicleTurnOrLoopOffset(obj);
    if (obj->apiobj.field_0x27f == 7 || obj->apiobj.field_0x27f == 0x10)
        return vehicle_offset + obj->apiobj.water_height;

    f32 height = obj->apiobj.field_0x218;
    if (height != 2000000.0f) {
        if (__builtin_expect(obj->apiobj.water_height != 2000000.0f, 1))
            height = MAX(obj->apiobj.water_height, height);

        f32 hover_height = WORLD->current_level->hover_height;
        if (hover_height != 0.0f)
            return hover_height + height + vehicle_offset;
        return height + static_cast<GAMECHARACTERDATA *>(obj->apiobj.character_data->field11_0x24)->field_0x28 +
               vehicle_offset;
    }
    return 2000000.0f;
}

float GetVehicleHoverHeight(GameObject_s *object, float *separation_offset) {
    LEVELDATA *level = WORLD->current_level;
    float height = object->hover_height_override;
    if (height == 1.0e9f) {
        height = level->hover_height;
        if (height == 0.0f) {
            height = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->field_0x28;
            if (object->id == id_SPEEDERBIKE && level == SPEEDERCHASEA_LDATA && disable_narrow_socks != 0) {
                height *= 0.5f;
            }
        }
    }

    bool separate;
    if (level != SPEEDERCHASEA_LDATA) {
        separate = object->apiobj.player_controlled && Player[0] != NULL &&
                   (Player[0]->apiobj.character_data->model_flags & 0x2000) != 0 &&
                   (Player[0]->apiobj.player_controlled || Player[0]->character_context == 0x24) && Player[1] != NULL &&
                   (Player[1]->apiobj.character_data->model_flags & 0x2000) != 0 &&
                   (Player[1]->apiobj.player_controlled || Player[1]->character_context == 0x24);
    } else {
        separate = disable_narrow_socks == 0 && object->apiobj.field_0x27c != -1 && object->movement_spline == NULL &&
                   Player[0] != NULL && Player[0]->takeover_source != NULL && Player[0]->id == id_SPEEDERBIKE &&
                   Player[1] != NULL && Player[1]->takeover_source != NULL && Player[0]->id == Player[1]->id;
    }
    if (separate) {
        float combined_height = (Player[0]->apiobj.field_0x1e0 + Player[1]->apiobj.field_0x1e0) * 0.6f;
        float combined_radius = Player[0]->apiobj.field_0x1dc + Player[1]->apiobj.field_0x1dc;
        float outer_radius = combined_radius * 3.0f;
        float inner_distance = combined_radius * combined_radius;
        float outer_distance = outer_radius * outer_radius;
        float distance = NuVecXZDistSqr(&Player[0]->apiobj.position, &Player[1]->apiobj.position, NULL);
        float blend;
        if (distance < inner_distance) {
            blend = 1.0f;
        } else if (distance < outer_distance) {
            blend = 1.0f - (distance - inner_distance) / (outer_distance - inner_distance);
        } else {
            blend = 0.0f;
        }
        float offset = blend * combined_height;
        float correction = 0.0f;
        float adjusted_height = height - offset;
        if (adjusted_height < offset) {
            correction = offset - adjusted_height;
        }
        if (Player[0] == object) {
            adjusted_height = height + offset;
            if (separation_offset != NULL) {
                *separation_offset = offset;
            }
        } else if (separation_offset != NULL) {
            *separation_offset = -offset;
        }
        height = adjusted_height + correction;
        level = WORLD->current_level;
    } else if (separation_offset != NULL) {
        *separation_offset = 0.0f;
    }

    i32 hoth_battle = 0;
    if (level == HOTHBATTLEA_LDATA && (TerLayer[static_cast<i8>(object->apiobj.field_0x27f)].flags & 1) != 0) {
        height = 0.0f;
        hoth_battle = 1;
    }
    getvehiclehoverheight_hothbattlehack = hoth_battle;
    return height;
}

void ApplyGravity(GameObject_s *object, float *gravity, float hover_height, float seek_rate, float *ground_height) {
    float extra_hover_offset = applygravity_extrahoveroffset;
    applygravity_extrahoveroffset = 0.0f;
    if (object->movement_spline != NULL) {
        return;
    }
    i32 flags = object->apiobj.flags_low;
    if ((flags & 0x20) != 0 || object->move_override != NULL) {
        return;
    }
    i32 context = object->character_context;
    float acceleration;
    if ((CInfo[context].flags & 0x400) != 0) {
        if (context != 0x44 || static_cast<u16>(object->context_animation - 5) > 1) {
            return;
        }
    } else if (context == 0x5f) {
        if ((object->context_variant_flags & 1) != 0) {
            return;
        }
    } else if (context == 0x4b) {
        if (object->suit == NULL || (static_cast<SUIT_s *>(object->suit)->store_flag & 0x10) == 0) {
            return;
        }
        acceleration = -1.0f;
        gravity = &acceleration;
    }
    if ((flags & 4) != 0 || static_cast<i8>(object->field_0xefb) < 0 || context == 0x2c) {
        return;
    }
    if ((object->field_0xe20 & 0x20) != 0) {
        object->apiobj.velocity.y = 0.0f;
        return;
    }
    CHARACTERDATA *character = object->apiobj.character_data;
    if ((character->model_flags & 0x2000) == 0 && static_cast<u8>(context - 0x23) < 2) {
        object->apiobj.velocity.y = 0.0f;
        return;
    }
    WORLDINFO_s *world = WORLD;
    if ((WORLD->current_level->flags & 0x40000) != 0 && gravity == NULL &&
        static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->gravity == 0.0f) {
        return;
    }
    if (object->field_0x1086 == 4) {
        return;
    }

    if (context == 0x1e || context == 0x22 || context == 0x42) {
        hover_height = 0.2f;
        seek_rate = 8.0f;
    } else if (context == 0x1c) {
        GameObject_s *target = object->force_target;
        if (target != NULL && (target->action_flags & 0x180) != 0) {
            if (target->field_0x7a3 == 1) {
                return;
            }
            if ((target->field_0xe21 & 4) == 0 &&
                ((target->field_0xe21 & 1) == 0 ||
                 (static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->flags_090 & 0x4000) == 0)) {
                float target_ground_height;
                target_ground_height = target->apiobj.lower_position.y;
                ground_height = &target_ground_height;
                hover_height = (target->apiobj.upper_position.y - target_ground_height) * 0.75f;
                seek_rate = 8.0f;
            }
        }
    } else if (context == 0x5d && object->field_0x768 != 1.0e9f) {
        float target_velocity = (object->apiobj.position.y - object->field_0x768) *
                                static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->gravity * 0.333f;
        object->apiobj.velocity.y = SeekValF(object->apiobj.velocity.y, target_velocity, 8.0f);
        return;
    }

    float height = object->hover_height_override == 1.0e9f ? hover_height : object->hover_height_override;
    i32 falling_hover = 0;
    if (height != 0.0f && object->fall_recovery_timer > 0.0f && WORLD->current_level != E2VEHICLEBONUSA_LDATA) {
        if ((GUNSHIP_ADATA != NULL && GUNSHIP_ADATA == WORLD->area) ||
            (BONUS_GUNSHIP_ADATA != NULL && BONUS_GUNSHIP_ADATA == WORLD->area)) {
            height = 0.0f;
        } else if (static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->field275_0x116 == 2 &&
                   static_cast<i8>(flags) < 0) {
            if (object->fall_hover_height != 2.0e6f) {
                falling_hover = 1;
                if (object->fall_recovery_timer >= 0.6f) {
                    object->fall_hover_height -= FRAMETIME * 3.0f;
                }
            }
        } else {
            height -= object->fall_recovery_timer * 0.5f;
        }
    }
    if (context != 0x33) {
        if ((object->apiobj.field_0x27d & 1) != 0 && object->field_0x1084 == 0 && object->apiobj.velocity.y == 0.0f &&
            object->surface_normal.y > NuTrigTable[0x21c7]) {
            object->apiobj.field_0x27d &= ~1;
        }
        if (height == 0.0f && (object->apiobj.field_0x27d & 1) != 0) {
            object->apiobj.velocity.y = 0.0f;
            return;
        }
    }

    bool hold_jump = false;
    if (gravity != NULL) {
        acceleration = *gravity;
    } else {
        if (object->apiobj.field_0x218 != 2.0e6f && height != 0.0f && context != 0x2b) {
            if (falling_hover) {
                object->terrain_origin_floor_offset = 0.0f;
                float target_velocity = (object->apiobj.position.y - object->fall_hover_height) *
                                        static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->gravity * 0.333f;
                object->apiobj.velocity.y = SeekValF(object->apiobj.velocity.y, target_velocity, seek_rate);
                return;
            }
            if ((object->apiobj.field_0x27f == 0x10 || object->apiobj.field_0x27f == 7) &&
                (VehicleArea != 0 ||
                 (static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->flags_090 & 0x40) == 0 ||
                 object->takeover_source != NULL)) {
                float separation;
                GetVehicleHoverHeight(object, &separation);
                separation += object->apiobj.water_height;
                if (object->hover_height_override != 1.0e9f) {
                    separation += object->hover_height_override;
                }
                float offset = VehicleTurnOrLoopOffset(object);
                float target_velocity;
                if (object->apiobj.model_draw_result != 0 || FRAMETIME < 0.016666668f) {
                    target_velocity =
                        (object->apiobj.collision_position.y - (offset + separation)) *
                        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->gravity * 0.333f;
                } else {
                    target_velocity =
                        ((object->apiobj.collision_position.y - (offset + separation)) *
                         static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->gravity) /
                        (FRAMETIME * 180.0f);
                }
                object->apiobj.velocity.y = SeekValF(object->apiobj.velocity.y, target_velocity, seek_rate);
                return;
            }
            if (PODSPRINT_ADATA != NULL && PODSPRINT_ADATA == world->area && static_cast<i8>(flags) < 0) {
                TUBE *tube = Tube_InAnyCylinder(world, object, 1);
                if (tube != NULL) {
                    ground_height = &tube->top;
                }
            }
            float floor =
                ground_height != NULL && *ground_height != 2.0e6f ? *ground_height : object->apiobj.field_0x218;
            if (object->apiobj.water_height != 2.0e6f && object->apiobj.water_height > floor &&
                object->apiobj.field_0x27f != 5 && object->apiobj.field_0x27f != 7 &&
                object->apiobj.field_0x27f != 0x10) {
                floor = object->apiobj.water_height;
            }
            floor += extra_hover_offset;
            float offset = VehicleTurnOrLoopOffset(object);
            character = object->apiobj.character_data;
            floor += offset;
            height += offset;
            GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(character->field11_0x24);
            if ((data->flags_094[0] & 0x20) != 0 || (PODSPRINT_ADATA != NULL && PODSPRINT_ADATA == WORLD->area) ||
                !(GamePlayTimer.time_elapsed >= 1.0f) || !(object->apiobj.collision_origin.y > height * 5.0f + floor)) {
                object->terrain_origin_floor_offset = height;
                float pull = PODSPRINT_ADATA != NULL && PODSPRINT_ADATA == WORLD->area ? -35.0f : data->gravity;
                float target_velocity;
                if (object->apiobj.model_draw_result != 0 || FRAMETIME < 0.016666668f) {
                    target_velocity = (object->apiobj.collision_origin.y - (floor + height)) * pull * 0.333f;
                } else {
                    target_velocity =
                        ((object->apiobj.collision_origin.y - (floor + height)) * pull) / (FRAMETIME * 180.0f);
                }
                object->apiobj.velocity.y = SeekValF(object->apiobj.velocity.y, target_velocity, seek_rate);
                return;
            }
        }

        bool default_gravity = true;
        if ((object->apiobj.field_0x218 == 2.0e6f || height == 0.0f) && context == 0) {
            if (object->action_movement_state == 3) {
                acceleration = -10.0f;
                default_gravity = false;
            } else if (object->action_movement_state == 4) {
                acceleration = SLAMGRAVITY;
                default_gravity = false;
            } else if (object->action_movement_state == 5) {
                if (object->apiobj.field_0x281 == 0x1a) {
                    object->field_0xe31 = 3;
                    object->jump_variant_timer = 0.0f;
                } else if ((object->pad_gamepad->buttons_held & GAMEPAD_JUMP) == 0) {
                    if (object->field_0xe31 == 2) {
                        object->field_0xe31 = 3;
                    }
                    object->jump_variant_timer = 0.0f;
                } else if (object->field_0xe31 == 3) {
                    acceleration = -1.0f;
                    default_gravity = false;
                } else if (object->field_0xe31 == 2) {
                    object->apiobj.velocity.y = 0.0f;
                    return;
                } else {
                    hold_jump = true;
                }
            }
        }
        if (default_gravity) {
            acceleration = static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->gravity;
            if (object->character_context == 0x2b) {
                if (object->field_0xe36 == 3) {
                    acceleration = -acceleration;
                } else if (object->field_0xe36 == 2) {
                    acceleration = -0.25f;
                } else if (object->field_0xe36 == 4) {
                    acceleration = -2.0f;
                }
            }
        }
        float middle = (object->apiobj.position.y - object->character_bottom * object->apiobj.field_0xa8) +
                       (object->character_top - object->character_bottom) * 0.5f * object->apiobj.field_0xa8;
        if ((object->apiobj.field_0x27d == 0 && object->field_0x1084 != 0 && object->contact_position.y < middle &&
             object->apiobj.anim_packet.blending == 0 &&
             object->apiobj.character_model->model_data_b[object->apiobj.anim_packet.animation_index] != NULL &&
             IsAFallAnim(object->apiobj.anim_packet.animation_index) != 0) ||
            object->fall_acceleration_timer > 0.0f) {
            acceleration += acceleration;
            if (acceleration > -5.0f) {
                acceleration = -5.0f;
            }
        }
    }
    float previous_velocity = object->apiobj.velocity.y;
    if ((object->apiobj.field_0x27d & 0xfd) == 0 || object->id == id_DRAGBOMB) {
        object->apiobj.velocity.y = acceleration * FRAMETIME + previous_velocity;
    }
    if (object->character_context == 0x2b) {
        if (object->field_0xe36 == 3) {
            if (object->apiobj.velocity.y < 0.1f) {
                object->apiobj.velocity.y = 0.1f;
            }
        } else if (object->apiobj.velocity.y > -0.1f) {
            object->apiobj.velocity.y = -0.1f;
        }
    }
    if (hold_jump && previous_velocity > 0.0f && object->apiobj.velocity.y <= 0.0f) {
        float duration = static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->field_0x48;
        if (duration > 0.0f) {
            object->apiobj.velocity.y = 0.0f;
            object->jump_variant_timer = (TouchHacks::TouchControlsActive ? 1.3f : 1.0f) * duration;
            if (!object->apiobj.player_controlled) {
                object->jump_variant_timer *= 1.5f;
            }
            object->field_0xe31 = 2;
        } else {
            object->field_0xe31 = 3;
        }
    }
}

void DrawOffsetCode(GameObject_s *obj, i32 param) {
    NUVEC delta;
    NUVEC_ALIGNED16 target = v000;
    u8 *bytes = reinterpret_cast<u8 *>(obj);
    void *route = *reinterpret_cast<void **>(bytes + 0xcac);
    if (route != NULL) {
        const u16 flags = *reinterpret_cast<u16 *>(static_cast<u8 *>(route) + 0xc);
        if ((flags & 0x10) != 0) {
            target.y = 0.0076f;
        } else if ((flags & 0x40) != 0) {
            target.y = MAGNETOFFSET;
        }
    }
    if ((obj->field_0x7a5 == 0x44 && static_cast<u16>(obj->context_animation - 5) >= 2) ||
        (obj->movement_context_state & 0xffff00) == 0x54300) {
        target = obj->context_position_offset;
    }
    if (param == 0) {
        if (obj->render_offset.x == target.x && obj->render_offset.y == target.y && obj->render_offset.z == target.z) {
            return;
        }

        const f32 distance = NuVecDist(&target, &obj->render_offset, &delta);
        const f32 step = 0.4f * FRAMETIME;
        if (!(distance <= step)) {
            NuVecNorm(&delta, &delta);
            NuVecScale(&delta, &delta, step);
            NuVecAdd(&obj->render_offset, &obj->render_offset, &delta);
            return;
        }
    }
    obj->render_offset = target;
}

void PreResetCode(GameObject_s *obj) {
    u8 t23 = (u8)(obj->field_0xe23 & 0xf8);
    obj->field_0xe20 &= 0xef;
    u8 t22 = (u8)(obj->field_0xe22 & 0x3f);
    u8 t25 = (u8)(obj->movement_runtime_flags & 0xaf);
    obj->field_0xe24 &= 0xcf;
    obj->field_0xf03 &= 0xdf;
    obj->quick_shoot_bolt_id = 0xff;
    obj->slam_debris_effect = 0xff;
    obj->reserved_e3c = 0x00;
    obj->attack_locator = 0xff;
    obj->movement_runtime_flags = t25;
    obj->field_0xe22 = t22;
    obj->field_0xe23 = t23;

    DrawOffsetCode(obj, 0);

    if ((obj->field_0xf00 & 8) != 0) {
        obj->field_0xe22 &= 0xf7;
        obj->weapon_trail_offset.x = (f32)qrand() * sPreResetMulA * sPreResetMulB - sPreResetSubC;
        obj->weapon_trail_offset.y = (f32)qrand() * sPreResetMulA * sPreResetMulB - sPreResetSubC;
        obj->weapon_trail_offset.z = (f32)qrand() * sPreResetMulA * sPreResetMulB - sPreResetSubC;
        obj->field_0xe20 &= 0xfb;
        obj->incoming_bolt = NULL;
        obj->incoming_melee = NULL;
        obj->incoming_special = NULL;
        obj->incoming_part = NULL;
        if (obj->field_0x7a5 != 0xa) {
            obj->field_0xe21 &= 0xf7;
        }
        obj->field_0xe21 &= 0xdf;
        obj->sabre_flags = 0x01;
        obj->field_0xe22 &= 0xfd;
        obj->blade_states[3] = 0xff;
        obj->blade_states[2] = 0xff;
        obj->blade_states[1] = 0xff;
        obj->blade_states[0] = 0xff;
        obj->force_glow_candidate = NULL;
        obj->apiobj.anim_packet.overlay_animation = -1;
        obj->terrain_origin_floor_offset = 0.0f;
        if ((obj->apiobj.character_data->model_flags & 0x8040) == 0) {
            obj->field_0xe31 = 0;
        }

        {
            i32 e04 = obj->dynamic_light_id;
            obj->field_0xe23 &= 0x7f;
            if (e04 != -1) {
                rtlDynamicEnable(e04, 0);
            }
        }

        {
            u8 al = obj->apiobj.field_0x27d;
            obj->movement_runtime_flags &= 0xfd;
            obj->field_0xe24 &= 0xbf;
            obj->field_0x1089 = 0;

            if (al != 0) {
                obj->ground_contact_grace_timer = 0.2f;
            } else if (obj->ground_contact_grace_timer > 0.0f) {
                obj->ground_contact_grace_timer -= FRAMETIME;
            }

            if ((i8) static_cast<u8>(obj->apiobj.field_0x1f8) >= 0 || VehicleArea != 0) {
                goto finish_dfd;
            }

            if (obj->field_0x7a5 == 0 && sPreReset1048Scale * obj->apiobj.character_data->player_config->reset_scale >
                                             obj->context_animation_timer) {
                goto finish_dfd;
            }

            {
                f32 f28v = obj->pad_gamepad->input_magnitude;
                if (f28v > 0.0f) {
                    if ((al & 2) != 0) {
                        goto finish_dfd;
                    }
                    if (obj->field_0x1084 == 0) {
                        goto finish_dfd;
                    }
                } else {
                    if (obj->apiobj.velocity.x == 0.0f && obj->apiobj.velocity.z == 0.0f) {
                        goto finish_dfd;
                    }
                    if ((al & 2) != 0) {
                        goto finish_dfd;
                    }
                    if (obj->field_0x1084 == 0) {
                        goto finish_dfd;
                    }
                }

                {
                    f32 t1 = obj->contact_normal.y;
                    if (!(t1 > NuTrigTable[0x4000])) {
                        goto finish_dfd;
                    }
                    f32 t2 = NuTrigTable[0x3000];
                    if (!(t2 > t1)) {
                        goto finish_dfd;
                    }
                    t1 = obj->apiobj.field_0x1e0 / sPreResetDivF;
                    f32 diff = obj->apiobj.collision_min.y - obj->apiobj.field_0x218;
                    t1 *= sPreResetMulG;
                    if (!(diff > t1)) {
                        goto finish_dfd;
                    }
                    NUVEC_ALIGNED16 dir;
                    if (!(f28v > 0.0f)) {
                        dir.x = obj->apiobj.velocity.x;
                        dir.y = 0.0f;
                        dir.z = obj->apiobj.velocity.z;
                        NuVecNorm(&dir, &dir);
                    } else {
                        NuVecRotateY(&dir, &v001, obj->apiobj.movement_facing_angle);
                    }
                    if (!((obj->contact_position.x - obj->apiobj.position.x) * dir.x +
                              (obj->contact_position.z - obj->apiobj.position.z) * dir.z >
                          0.0f)) {
                        goto finish_dfd;
                    }
                    obj->fall_acceleration_timer = 0.1f;
                }
            }
            goto finish_e1e;

        finish_dfd:
            if (obj->fall_acceleration_timer > 0.0f) {
                obj->fall_acceleration_timer -= FRAMETIME;
            }
            goto finish_e1e;

        finish_e1e:
            GameObjectNearFloor(obj, 1.0f, &obj->nearby_floor_distance);
            obj->pre_reset_hover_height = GetHoverPosY(obj);
            {
                u8 v = obj->apiobj.character_data->player_config->variant;
                if (v != 0xff) {
                    if (static_cast<u8>(obj->apiobj.field_0x27c) != 0xff) {
                        if (Cheat_IsOn(0x19) != 0) {
                            obj->blade_index = 0x00;
                        } else if (static_cast<u8>(obj->apiobj.field_0x27c) != 0xff &&
                                   Player_HasPurpleForce(obj) != 0) {
                            obj->blade_index = 0x03;
                        } else {
                            obj->blade_index = obj->apiobj.character_data->player_config->variant;
                        }
                    } else {
                        obj->blade_index = v;
                    }
                } else {
                    obj->blade_index = 0xff;
                }

                {
                    if ((obj->pad_gamepad->buttons_held & GAMEPAD_ACTION) != 0) {
                        obj->hold_timer += FRAMETIME;
                    } else {
                        if (obj->hold_timer > 0.0f && sPreResetD18Scale > obj->hold_timer) {
                            obj->field_0xe24 |= 0x40;
                        }
                        obj->hold_timer = 0.0f;
                    }
                }
            }
            obj->movement_runtime_flags &= 0x5f;
        }
    }

    return;
}

void PostResetCode(GameObject_s *obj) {
    u8 *p = (u8 *)obj;
    void *pp = *(void **)(p + 0x54);
    *(u8 *)(p + 0x1091) = 0;
    void *q = *(void **)((u8 *)pp + 0x24);
    i16 v = *(i16 *)((u8 *)q + 0xe8);
    if (v != -1) {
        u8 b = *(u8 *)((u8 *)q + 0x11e);
        f32 f = (f32)(u32)b;
        ChatterSfx(obj, v, f);
    }
    if (*(void **)(*(u8 **)(p + 0x54) + 0x18) != (void *)&Move_VEHICLE && *(u16 *)(p + 0xe1c) != 0 &&
        (*(u8 *)(p + 0xe25) & 0x20) == 0) {
        *(u16 *)(p + 0xe1c) = 0;
    }
}

// Original: 1,554 bytes.
f32 ForceTowardsMid(GameObject_s *object) {
    SOCKSYS *system = WorldInfo_CurrentlyActive()->sock_sys;
    if (system == NULL || object->sock_position.location.sock == -1)
        return 0.0f;
    SOCK *socks = system->sock;
    f32 amount = 0.0f;
    if (static_cast<u8>(object->sock_position.candidate_count) > 1)
        return amount;
    SOCK *sock = &socks[object->sock_position.location.sock];
    f32 inner = sock->mid_force_inner_radius;
    f32 outer = sock->mid_force_outer_radius;
    if (sock->flags & 2) {
        if (object->field_0x1086 != 4 && inner > 0.0f && outer > 0.0f) {
            f32 dy = object->sock_position.midpoint.y - object->apiobj.position.y;
            if (dy * dy >= inner * inner) {
                f32 ratio = (NuFabs(dy) - inner) / (outer - inner);
                object->target_velocity.y += (dy * object->apiobj.character_data->game_character->run_speed) * ratio;
                amount = ratio;
            }
        }
        return amount;
    }
    if (inner > 0.0f && outer > 0.0f) {
        i32 planar = sock->flags & 4;
        f32 inner_squared = inner * inner;
        NUVEC delta;
        f32 distance_squared;
        if (planar != 0) {
            delta.x = object->sock_position.midpoint.x - object->apiobj.position.x;
            delta.y = 0.0f;
            delta.z = object->sock_position.midpoint.z - object->apiobj.position.z;
            distance_squared = delta.x * delta.x + delta.z * delta.z;
        } else {
            NuVecSub(&delta, &object->sock_position.midpoint, &object->apiobj.position);
            distance_squared = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
        }
        if (distance_squared >= inner_squared) {
            f32 ratio = (NuFsqrt(distance_squared) - inner) / (outer - inner);
            if (ratio > 3.0f)
                ratio = 3.0f;
            if (ratio >= 0.0f) {
                amount = ratio;
                if (planar == 0 && sock->unknown_7c != 1.0f) {
                    if (object->field_0x1086 == 4) {
                        NuVecInvMtxRotate(&delta, &delta, &object->vehicle_orientation);
                        delta.y *= 1.0f / sock->unknown_7c;
                        NuVecMtxRotate(&delta, &delta, &object->vehicle_orientation);
                    } else {
                        i32 pitch = RotDiff(0, object->sock_position.midpoint_rotation.x);
                        i32 yaw = RotDiff(0, object->sock_position.midpoint_rotation.y);
                        NuVecRotateY(&delta, &delta, -yaw);
                        NuVecRotateX(&delta, &delta, -pitch);
                        delta.y *= 1.0f / sock->unknown_7c;
                        NuVecRotateX(&delta, &delta, pitch);
                        NuVecRotateY(&delta, &delta, yaw);
                    }
                }
                f32 inverse = 1.0f / NuFsqrt(distance_squared);
                f32 speed = object->apiobj.character_data->game_character->run_speed * amount;
                delta.x *= inverse;
                if (planar == 0)
                    delta.y *= inverse;
                delta.z *= inverse;
                object->target_velocity.x += delta.x * speed;
                if (planar == 0)
                    object->target_velocity.y += delta.y * speed;
                object->target_velocity.z += delta.z * speed;
            }
        }
        if (planar == 0)
            return amount;
    }
    if ((sock->flags & 8) != 0 && object->apiobj.position.y >= object->sock_position.midpoint.y) {
        CHARACTERDATA *data = object->apiobj.character_data;
        f32 ceiling = object->sock_position.midpoint.y + data->field14_0x30 * data->field17_0x3c * 3.0f;
        f32 vertical = ceiling > object->apiobj.position.y
                           ? (object->apiobj.position.y - object->sock_position.midpoint.y) /
                                 (ceiling - object->sock_position.midpoint.y)
                           : 1.0f;
        object->target_velocity.y -= vertical * data->game_character->run_speed;
    }
    return amount;
}

void MoveInactiveVehicle(GameObject_s *object, i32, GameObject_s **followed_object) {
    object->target_velocity.x = object->target_velocity.y = object->target_velocity.z = 0.0f;
    GameObject_s *other = Player[0];
    if (other == object) {
        other = Player[1];
    } else if (Player[1] != object) {
        NUVEC position = v000;
        float count = 0.0f;
        if (Player[0] != NULL) {
            NuVecAdd(&position, &position, &Player[0]->apiobj.position);
            count += 1.0f;
        }
        if (Player[1] != NULL) {
            NuVecAdd(&position, &position, &Player[1]->apiobj.position);
            count += 1.0f;
        }
        if (count > 0.0f) {
            NuVecScale(&position, &position, 1.0f / count);
        }
        SeekVec(&object->apiobj.position, &object->apiobj.position, &position, 10.0f);
        object->apiobj.velocity = object->target_velocity;
        other = NULL;
        goto finish_vehicle;
    }
    {
        NUVEC position = other->apiobj.position;
        SeekVec(&object->apiobj.position, &object->apiobj.position, &position, 10.0f);
        object->apiobj.velocity = object->target_velocity;
        object->apiobj.facing_angle = object->apiobj.movement_facing_angle = object->apiobj.field_0x276 =
            other->apiobj.field_0x276;
    }
finish_vehicle:
    if (followed_object != NULL) {
        *followed_object = other;
    }
    object->delayed_turn_timer = 0.0f;
    object->field_0xddc = 0.0f;
}

// Original: 784 bytes.
void KeepVehicleOnScreen(GameObject_s *object, i32 sides, i32 top, i32 bottom) {
    object->field_0xf03 |= 0x10;
    if (GamePlayTimer.time_elapsed < 1.0f)
        return;
    f32 margin = WORLD->area == BATTLEOVERCORUSCANT_ADATA ? 0.5f : 1.0f;
    f32 distance;
    if (sides != 0) {
        OnOrInsidePlane(&object->apiobj.collision_position, &PlayPlane[1].point, &PlayPlane[1].normal, NULL, 0.0f,
                        &distance);
        if (distance < margin) {
            f32 correction = -((distance - margin) / margin);
            f32 speed = object->apiobj.character_data->game_character->run_speed;
            object->target_velocity.x += (speed + speed) * (correction + correction);
        } else {
            OnOrInsidePlane(&object->apiobj.collision_position, &PlayPlane[2].point, &PlayPlane[2].normal, NULL, 0.0f,
                            &distance);
            if (distance < margin) {
                f32 correction = -((distance - margin) / margin);
                f32 speed = -object->apiobj.character_data->game_character->run_speed;
                object->target_velocity.x += (speed + speed) * (correction + correction);
            }
        }
    }
    if (top != 0) {
        OnOrInsidePlane(&object->apiobj.collision_position, &PlayPlane[3].point, &PlayPlane[3].normal, NULL, 0.0f,
                        &distance);
    } else {
        distance = 1000000000.0f;
    }
    if (distance != 1000000000.0f && distance < margin) {
        f32 correction = -((distance - margin) / margin);
        f32 speed = -object->apiobj.character_data->game_character->run_speed;
        object->target_velocity.y += (speed + speed) * (correction + correction);
        return;
    }
    if (bottom != 0) {
        OnOrInsidePlane(&object->apiobj.collision_position, &PlayPlane[4].point, &PlayPlane[4].normal, NULL, 0.0f,
                        &distance);
        if (distance < margin) {
            f32 correction = -((distance - margin) / margin);
            f32 speed = object->apiobj.character_data->game_character->run_speed;
            object->target_velocity.y += (speed + speed) * (correction + correction);
        }
    }
}

i32 FaceOpponent(GameObject_s *object, NUVEC *position) {
    if (position == NULL) {
        GameObject_s *target = object->force_target;
        if (target != NULL) {
            if ((target->apiobj.field_0x1f8 & 0x1001) != 0x1001) {
                return 0;
            }
            position = &target->apiobj.position;
            if (target->apiobj.field_0x287 != 0) {
                return 0;
            }
        } else {
            if (object->blowup_target == NULL) {
                return 0;
            }
            position = &object->blowup_target->mid_position;
        }
    }
    object->apiobj.movement_facing_angle =
        NuAtan2D(position->x - object->apiobj.position.x, position->z - object->apiobj.position.z);
    return 1;
}

void NewSeekHalfLife(i32 &current, i32 target, float fraction) {
    current = static_cast<i16>(static_cast<i16>(current) +
                               static_cast<i16>(static_cast<f32>(static_cast<i16>(target - current)) * fraction));
}

void VehicleCollisionCode(GameObject_s *object) {
    static f32 magdif;
    if (object->character_context != -1 && object->character_context != 0x3a)
        return;
    if (GamePlayTimer.time_elapsed < 1.0f)
        return;
    LEVELDATA_s *level = WORLD->current_level;
    LEVELDATA_s *speeder_level = SPEEDERCHASEA_LDATA;
    if (object->apiobj.field_0x27c == -1 || !object->apiobj.player_controlled || !object->field_0x1084)
        return;
    NUVEC normal, reverse_velocity, debris_position;
    debris_position = object->contact_position;
    i32 random = qrand();
    debris_position.y = object->contact_position.y - 0.5f * object->apiobj.field_0x1e0 +
                        random * (object->apiobj.field_0x1e0 / 65535.0f);
    i32 debris_type = -1;
    if (object->id == id_SPEEDERBIKE) {
        qrand();
        qrand();
        debris_type = 108;
    } else if (object->id == id_TIEFIGHTER || object->id == id_XWING)
        debris_type = 100;
    if (debris_type != -1) {
        AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[debris_type].effect, &debris_position, 52,
                                          FRAMETIME, 0, 0, NULL);
        debris_type = 101;
    }
    if (object->contact_normal.y <= 0.574f && object->contact_normal.y >= -0.574f) {
        if (object->apiobj.horizontal_velocity_magnitude >
            ((GAMECHARACTERDATA_s *)object->apiobj.character_data->field11_0x24)->field_0x10) {
            NuVecNorm(&normal, &object->contact_normal);
            reverse_velocity = object->reset_velocity;
            NuVecNorm(&reverse_velocity, &reverse_velocity);
            reverse_velocity.x = -reverse_velocity.x;
            reverse_velocity.z = -reverse_velocity.z;
            i16 angle = 0x4000 - NuASin(reverse_velocity.x * normal.x + reverse_velocity.z * normal.z);
            if (level == speeder_level && angle <= 0x71b) {
                if (object->apiobj.horizontal_velocity_magnitude >
                    (((GAMECHARACTERDATA_s *)object->apiobj.character_data->field11_0x24)->tiptoe_speed +
                     ((GAMECHARACTERDATA_s *)object->apiobj.character_data->field11_0x24)->walk_speed) *
                        0.5f)
                    goto emit_debris;
            } else if (angle > 0x1fff)
                return;
            magdif = (1.0f / ((GAMECHARACTERDATA_s *)object->apiobj.character_data->field11_0x24)->run_speed) *
                     (object->pre_terrain_speed - object->post_terrain_speed);
            if (!(magdif > 0.6f))
                return;
        emit_debris:
            if (debris_type != -1)
                AddGameDebris(WORLD->debris_sys, debris_type, &object->contact_position);
        }
    }
}

f32 GetVehicleAreaRememberSpeed() {
    if (VehicleArea == 0) {
        return 0.0f;
    }

    f32 speed = 0.0f;
    f32 player_count = 0.0f;
    if (Player[0] != NULL && Player[0]->apiobj.player_controlled) {
        speed += Player[0]->field_0xdc8;
        player_count = 1.0f;
    }
    if (Player[1] != NULL && Player[1]->apiobj.player_controlled) {
        speed += Player[1]->field_0xdc8;
        player_count += 1.0f;
    }
    if (player_count > 1.0f) {
        speed /= player_count;
    }
    if (speed < 0.25f) {
        speed = 0.25f;
    }
    return speed;
}

f32 GetVehicleSpeedMul(GameObject_s *object, f32 speed) {
    f32 effective;
    if (object->character_context == 0x36 || object->character_context == 0x2a || object->character_context == 0x3a)
        effective = ((GAMECHARACTERDATA_s *)object->apiobj.character_data->field11_0x24)->run_speed;
    else if ((object->apiobj.flags_low & 0x80) != 0 && WORLD->current_level == DEATHSTARBATTLED_LDATA &&
             ObjInNarrowSock(object, WORLD->sock_sys, WORLD->level_idx)) {
        GAMECHARACTERDATA_s *data = (GAMECHARACTERDATA_s *)object->apiobj.character_data->field11_0x24;
        f32 fraction = (speed - data->field_0x10) / (data->run_speed - data->field_0x10);
        if (fraction < 0.0f)
            fraction = 0.0f;
        effective = (0.5f + fraction * 0.5f) * data->run_speed;
    } else if ((object->apiobj.flags_low & 0x80) != 0 && (!IDLESPEEDINNARROWSOCKSONLY || object->in_narrow_socket) &&
               (object->field_0xf03 & 2) == 0) {
        f32 idle = ((GAMECHARACTERDATA_s *)object->apiobj.character_data->field11_0x24)->field_0x10;
        effective = idle > speed ? idle : speed;
    } else {
        GAMECHARACTERDATA_s *data = (GAMECHARACTERDATA_s *)object->apiobj.character_data->field11_0x24;
        effective = (speed - data->field_0x10) / (data->run_speed - data->field_0x10) * data->run_speed;
    }
    if (effective < 0.0f)
        effective = 0.0f;
    return effective / ((GAMECHARACTERDATA_s *)object->apiobj.character_data->field11_0x24)->run_speed;
}

i32 NeedsPretendAnim(GameObject_s *object) {
    return object->apiobj.character_model->model_data_b[object->apiobj.anim_packet.requested_animation] == NULL ||
           object->id == id_JEDISTARFIGHTERREDEP3 || object->id == id_JEDISTARFIGHTERYELLOWEP3 ||
           object->id == id_TIEINTERCEPTOR;
}

void MovePlayer_VEHICLEDIRECTIONAL(GameObject_s *object) {
    APIOBJECT_s &api = object->apiobj;
    GAMEPAD_s *pad = object->pad_gamepad;
    GameObject_s *operator_object = object;
    GameObject_s *other;
    NUVEC carwash_delta, a, b, direction, separation;
    if (CarWashHack == NULL && object->id == id_LANDSPEEDER && WORLD->current_level == MOSEISLEYB_LDATA &&
        NuSpecialExistsFn(&LevHSpecial[0]) &&
        NuVecXZDistSqr(NuSpecialGetDrawPos(&LevHSpecial[0]), &api.collision_position, &carwash_delta) < 0.36f) {
        CarWashHack = object;
        if (pad->input_magnitude >
                (api.character_data->game_character->walk_speed + api.character_data->game_character->run_speed) *
                    0.5f &&
            (LevGizObst[0] == NULL || LevGizObst[0]->anim_set == NULL || (LevGizObst[0]->anim_set->flags & 1) == 0))
            CarWashHack = NULL;
    }
    if (object->id == id_WOOKIEFLYER && object->character_context == 0x17)
        ApplyGravity(object, NULL, 0.0f, 10.0f, NULL);
    object->field_0xe23 &= ~0x10;
    object->in_narrow_socket = ObjInNarrowSock(object, WORLD->sock_sys, WORLD->level_idx);
    u16 narrow_yaw = object->in_narrow_socket ? object->yrot : 0;
    if (object->character_context != 0x2a) {
        object->field_0xe24 &= ~2;
        object->previous_block_animation = -1;
        if (object->delayed_turn_timer > 0.0f && object->character_context != 0x36 && object->character_context != 0x3a)
            object->delayed_turn_timer -= FRAMETIME;
    } else
        object->previous_block_animation = -1;
    i32 in_tube = 0;
    if (PODSPRINT_ADATA != NULL && (PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA) &&
        api.player_controlled && Tube_InAnyCylinder(WORLD, object, 1)) {
        NewRumble(object->pad_gamepad->pad, qrand() * (1.0f / 65535.0f), 0);
        in_tube = 1;
    }
    Techno_FindOperator(object, &pad, &operator_object);
    f32 requested_speed;
    if (PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA)
        requested_speed =
            pad->input_magnitude / operator_object->apiobj.character_data->game_character->run_speed * 35.0f;
    else
        requested_speed = api.character_data->game_character->run_speed *
                          (pad->input_magnitude / operator_object->apiobj.character_data->game_character->run_speed);
    if ((object->field_0xe20 & 0x20) != 0 && object->character_context != 0x23 && object->character_context != 0x24) {
        MoveInactiveVehicle(object, 0, &other);
        if (other != NULL) {
            api.field_0x276 = other->apiobj.field_0x276;
            api.movement_facing_angle = api.field_0x276;
            api.facing_angle = api.movement_facing_angle;
            api.velocity = other->apiobj.velocity;
            object->field_0xdc8 = other->field_0xdc8;
            object->tertiary_lean_angle = 0;
            object->movement_lean_angle = 0;
            object->secondary_lean_angle = 0;
        }
        if (__builtin_expect(WORLD->current_level == PLATFORM_LDATA, 0))
            return;
        goto vehicle_collision;
    }
    {
        u16 input_yaw = GamePad_InputAngle(object, pad);
        object->field_0xe22 |= 0x20;
        object->current_input_angle = input_yaw;
        object->target_velocity.y = 0.0f;
        if (object->field_0xddc > 0.0f)
            object->field_0xddc -= FRAMETIME;
        f32 turn_multiplier = 0.0f;
        if ((object->field_0xefd & 4) != 0) {
            api.movement_facing_angle = input_yaw;
            api.facing_angle = api.movement_facing_angle;
            api.field_0x276 = api.facing_angle;
        } else {
            f32 heading_seek_rate = 10.0f;
            if (CarWashHack == object) {
                api.movement_facing_angle = 0x4000;
                heading_seek_rate = 3.0f;
            } else if (requested_speed > 0.0f && object->character_context != 0x2a &&
                       object->character_context != 0x36 && object->character_context != 0x3a &&
                       object->character_context != 0x23 && object->character_context != 0x24 &&
                       (object->character_context != 0x17 || (object->jump_input_flags & 1) != 0) &&
                       !AnimPlaying(&api.anim_packet, 12, 1, 1) && !AnimPlaying(&api.anim_packet, 6, 1, 1)) {
                if ((__builtin_expect(object->in_narrow_socket, 1) && api.player_controlled &&
                     WORLD->current_level == DEATHSTARBATTLED_LDATA &&
                     ObjInNarrowSock(object, WORLD->sock_sys, WORLD->level_idx)) ||
                    (object->in_narrow_socket && (WORLD->current_level == DEATHSTAR2BATTLEE_LDATA ||
                                                  WORLD->current_level == DEATHSTAR2BATTLEF_LDATA ||
                                                  WORLD->current_level == DEATHSTAR2BATTLEG_LDATA)))
                    turn_multiplier = 0.5f;
                else if (WORLD->current_level == SPEEDERCHASEA_LDATA && !disable_narrow_socks)
                    turn_multiplier = 0.75f;
                else if (api.player_controlled && (PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA))
                    turn_multiplier = 0.25f;
                f32 ratio =
                    (NuFsqrt(api.velocity.x * api.velocity.x + api.velocity.z * api.velocity.z) -
                     api.character_data->game_character->tiptoe_speed) /
                    (api.character_data->game_character->run_speed - api.character_data->game_character->tiptoe_speed);
                if (__builtin_expect(ratio < 0.0f, 0))
                    ratio = 0.0f;
                if (ratio > 1.0f)
                    ratio = 1.0f;
                f32 rate = (PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA)
                               ? 0.0f * ratio + 1.0f
                               : api.character_data->game_character->turn_rate +
                                     (api.character_data->game_character->field_0x78 -
                                      api.character_data->game_character->turn_rate) *
                                         ratio;
                if (__builtin_expect(!!(object->in_narrow_socket), 1)) {
                    rate *= -0.25f * object->field_0xdc8 + 1.0f;
                    if (turn_multiplier != 0.0f &&
                        !(__builtin_expect((api.flags_low & 0x80) != 0, 1) &&
                          (PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA)))
                        rate *= turn_multiplier;
                }
                api.movement_facing_angle = TurnRot(api.movement_facing_angle, input_yaw, (i32)(rate * 65536.0f), NULL);
                if (object->in_narrow_socket) {
                    i32 delta = RotDiff(narrow_yaw, api.movement_facing_angle);
                    i32 degrees = static_cast<i32>(-30.0f * object->field_0xdc8 + 60.0f);
                    if (degrees < 0)
                        degrees = 0;
                    if (api.player_controlled && (PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA))
                        degrees = 0;
                    else if (turn_multiplier != 0.0f)
                        degrees = (i32)(degrees * turn_multiplier);
                    i32 limit = static_cast<i32>(static_cast<u32>(degrees) << 16) / 360;
                    if (abs(delta) <= 0x4000) {
                        if (__builtin_expect(delta < -limit, 0))
                            api.movement_facing_angle = narrow_yaw - limit;
                        else if (delta > limit)
                            api.movement_facing_angle = narrow_yaw + limit;
                    } else {
                        limit = static_cast<i32>(static_cast<u32>(180 - degrees) << 16) / 360;
                        if (delta > 0 && delta < limit)
                            api.movement_facing_angle = narrow_yaw + limit;
                        else if (delta < 0 && delta > -limit)
                            api.movement_facing_angle = narrow_yaw - limit;
                    }
                }
            } else if (object->in_narrow_socket &&
                       __builtin_expect(requested_speed == 0.0f || object->character_context == 0x2a ||
                                            object->character_context == 0x36 || object->character_context == 0x3a,
                                        1)) {
                u16 desired = narrow_yaw;
                if (abs(RotDiff(narrow_yaw, api.movement_facing_angle)) > 0x4000)
                    desired += 0x8000;
                f32 rate = (PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA)
                               ? 1.0f
                               : api.character_data->game_character->turn_rate;
                if (object->character_context != 0x2a && object->character_context != 0x36 &&
                    object->character_context != 0x3a)
                    rate *= 0.25f;
                api.movement_facing_angle = TurnRot(api.movement_facing_angle, desired, (i32)(rate * 65536.0f), NULL);
            }
            u8 boundary_player = api.flags_low & 0x80;
            if (!(object->character_context == -1 && object->field_0x1084 &&
                  fabsf(object->contact_normal.y) < NuTrigTable[0x3aaa])) {
                if (__builtin_expect(boundary_player != 0 &&
                                         (api.character_data->game_character->flags_090 & 0x10000) == 0 &&
                                         (i8)object->field_0xf03 >= 0,
                                     0)) {
                    i32 outside = OutSideSplineArea(&api.collision_position, WORLD->camera_splines[16], &a, &b, 0);
                    i32 inside = 0;
                    if (!outside)
                        inside = OutSideSplineArea(&api.collision_position, WORLD->camera_splines[17], &a, &b, 1);
                    if ((api.field_0x1f4 & 0x400) == 0 && (object->field_0xe24 & 2) == 0 && (outside || inside)) {
                        api.movement_facing_angle = NuAtan2D(-(b.z - a.z), b.x - a.x);
                        object->field_0xe24 |= 2;
                        object->field_0xddc = 0.1f;
                    }
                }
            } else {
                if (__builtin_expect(boundary_player != 0 &&
                                         (PODSPRINT_ADATA == NULL || WORLD->area != PODSPRINT_ADATA) &&
                                         (api.character_data->game_character->flags_090 & 0x10000) == 0 &&
                                         WORLD->current_level != SPEEDERCHASEA_LDATA,
                                     1)) {
                    i32 normal_yaw = NuAtan2D(object->contact_normal.x, object->contact_normal.z);
                    if (requested_speed == 0.0f || abs(RotDiff(normal_yaw, object->current_input_angle)) > 0x4000) {
                        i32 delta = RotDiff(normal_yaw, api.field_0x276);
                        if (abs(delta) > 0x4000) {
                            i32 tangent = normal_yaw + (delta < 0 ? -0x4000 : 0x4000);
                            if (object->field_0xddc > 0.0f &&
                                RotDiff(object->previous_boundary_angle, normal_yaw) > 0x2aaa &&
                                abs(RotDiff(api.field_0x276, normal_yaw)) > 0x3fff) {
                                api.movement_facing_angle = static_cast<i32>(
                                    object->previous_boundary_angle +
                                    0.5f * RotDiff(object->previous_boundary_angle, normal_yaw) + 32768.0f);
                                object->field_0xe24 |= 2;
                                object->field_0xddc = 0.1f;
                            } else {
                                api.movement_facing_angle = tangent;
                                object->previous_boundary_angle = normal_yaw;
                                object->field_0xddc = 0.1f;
                            }
                        }
                    }
                }
            }
            if ((CInfo[object->character_context].flags & 1) != 0 && (object->id == id_STAP || object->id == id_STAP2))
                api.field_0x276 = api.movement_facing_angle = api.facing_angle;
            else
                api.facing_angle = api.field_0x276 =
                    SeekRot(api.facing_angle, api.movement_facing_angle, heading_seek_rate);
        }
        f32 seek_rate = (PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA)
                            ? 8.0f
                            : api.character_data->game_character->velocity_seek_rate;
        if ((api.character_data->game_character->flags_090 & 0x40) != 0 && object->field_0xcc0 == NULL)
            seek_rate = 5.0f;
        i32 special_lean = 0;
        if (api.movement_direction.x != 0.0f || api.movement_direction.z != 0.0f) {
            object->field_0xe23 |= 0x10;
            object->target_velocity.x = api.movement_direction.x;
            object->target_velocity.z = api.movement_direction.z;
            api.movement_direction.x = api.movement_direction.z = 0.0f;
        } else if (CarWashHack == object) {
            object->target_velocity.x = carwash_delta.x * 3.0f;
            object->target_velocity.z = carwash_delta.z * 3.0f;
            seek_rate = 5.0f;
        } else {
            const u8 speed_context = object->character_context;
            f32 water_mul = 1.0f;
            if (speed_context == 0x3a)
                object->field_0xdc8 = 1.0f;
            else if (speed_context == 0x36)
                object->field_0xdc8 =
                    NuTrigTable[((i32)((1.0f - object->context_animation_timer / object->airborne_action_duration) *
                                           65536.0f +
                                       16384.0f) >>
                                 1) &
                                0x7fff];
            else if (speed_context == 0x2a) {
                f32 half = 0.5f * object->airborne_action_duration;
                object->field_0xdc8 =
                    !(object->context_animation_timer >= half)
                        ? -(1.0f - object->context_animation_timer / half)
                        : 1.0f - (object->airborne_action_duration - object->context_animation_timer) / half;
            } else if (api.field_0x27c != -1 && FadeSys.fade > 0.0f && (MiniCutCam == 0 || (api.flags_high & 1) == 0))
                object->field_0xdc8 = VehicleAreaRememberSpeed;
            else {
                u8 player_vehicle = api.flags_low & 0x80;
                f32 decel = (PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA)
                                ? 0.25f
                                : api.character_data->game_character->field_0x80;
                f32 desired;
                if (__builtin_expect(WORLD->area == PODRACE_ADATA && object->field_0xee0 != 1.0e9f, 0))
                    desired = GetVehicleSpeedMul(object, object->field_0xee0);
                else if (player_vehicle != 0 && (PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA) &&
                         object->in_narrow_socket) {
                    u32 buttons = pad->buttons_held & (GAMEPAD_JUMP | GAMEPAD_SPECIAL);
                    i32 mode = 0;
                    if (__builtin_expect(buttons == GAMEPAD_JUMP, 0))
                        mode = 1;
                    else if (buttons == GAMEPAD_SPECIAL)
                        mode = -1;
                    if (PodSprint_InStartCountdown(WORLD) > 0.0f) {
                        object->previous_block_animation = 1;
                        desired = 0.0f;
                    } else if (pad->input_magnitude == 0.0f && mode == 0) {
                        object->previous_block_animation = 0;
                        desired = 25.0f;
                    } else if (mode != 1 && pad->input_angle > 0x3554 && pad->input_angle <= 0xcaaa) {
                        if (mode == -1 || (u16)(pad->input_angle - 0x4aab) <= 0x6aa9) {
                            object->previous_block_animation = 4;
                            desired = 15.0f;
                        } else {
                            object->previous_block_animation = 0;
                            desired = 25.0f;
                        }
                    } else {
                        object->previous_block_animation = 3;
                        desired = 35.0f;
                    }
                    desired /= 35.0f;
                } else if (requested_speed > 0.0f && speed_context != 0x23 && speed_context != 0x24) {
                    if (api.intersects_water) {
                        f32 fraction = (api.water_height - api.collision_min.y) / api.field_0x1e0;
                        if (fraction < 0.0f)
                            water_mul = 0.0f;
                        else if (fraction > 0.5f)
                            water_mul = 0.7f;
                        else
                            water_mul = 1.0f - (fraction + fraction) * 0.3f;
                    }
                    if (object->field_0xee0 != 1.0e9f)
                        desired = GetVehicleSpeedMul(object, object->field_0xee0);
                    else {
                        if (WORLD->current_level == SPEEDERCHASEA_LDATA && player_vehicle != 0 && !disable_narrow_socks)
                            desired = GetVehicleSpeedMul(
                                object, api.character_data->game_character->run_speed * 0.75f +
                                            api.character_data->game_character->run_speed * 0.25f *
                                                (pad->input_magnitude / api.character_data->game_character->run_speed));
                        else
                            desired = GetVehicleSpeedMul(object, requested_speed);
                        if (desired > object->field_0xdc8)
                            decel = (PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA)
                                        ? 0.5f
                                        : api.character_data->game_character->field_0x7c;
                    }
                } else if (WORLD->current_level == SPEEDERCHASEA_LDATA && player_vehicle != 0 && !disable_narrow_socks)
                    desired = GetVehicleSpeedMul(object, api.character_data->game_character->run_speed * 0.75f);
                else if (player_vehicle != 0 && (PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA))
                    desired = 25.0f;
                else if ((!IDLESPEEDINNARROWSOCKSONLY || object->in_narrow_socket) && player_vehicle != 0 &&
                         (object->field_0xf03 & 2) == 0)
                    desired =
                        api.character_data->game_character->field_0x10 / api.character_data->game_character->run_speed;
                else
                    desired = 0.0f;
                f32 step = (1.0f / decel) * FRAMETIME;
                if ((PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA))
                    desired *= object->current_speed_mul;
                object->field_0xdc8 = SeekLinearF(object->field_0xdc8, desired, step);
            }
            f32 speed = object->field_0xdc8 * ((PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA)
                                                   ? 35.0f
                                                   : api.character_data->game_character->run_speed);
            if (__builtin_expect((api.flags_low & 0x80) != 0, 1) && WORLD->current_level == DEATHSTARBATTLED_LDATA &&
                ObjInNarrowSock(object, WORLD->sock_sys, WORLD->level_idx))
                speed *= 2.0f;
            else if ((api.flags_low & 0x80) != 0 && (WORLD->current_level == DEATHSTAR2BATTLEE_LDATA ||
                                                     WORLD->current_level == DEATHSTAR2BATTLEF_LDATA ||
                                                     WORLD->current_level == DEATHSTAR2BATTLEG_LDATA))
                speed *= DeathStar2BattleFire_GetSlowDownMul(object) * 1.5f;
            else if (object->id == id_SPEEDERBIKE) {
                if (WORLD->current_level == SPEEDERCHASEA_LDATA && disable_narrow_socks)
                    speed *= 0.333f;
            } else if (in_tube)
                speed *= 0.25f;
            if (object->in_narrow_socket) {
                other = GetOtherActivePlayer(object);
                i32 forwards;
                if (other != NULL) {
                    if (other->in_narrow_socket) {
                        if ((forwards = GoingForwardsAlongNarrowSock(object)) == GoingForwardsAlongNarrowSock(other)) {
                            NuVecRotateY(&direction, &v001, object->yrot);
                            NuVecSub(&separation, &other->apiobj.position, &api.position);
                            f32 longitudinal = separation.x * direction.x + separation.z * direction.z;
                            f32 radius = api.field_0x1dc + other->apiobj.field_0x1dc;
                            if (fabsf(longitudinal) > radius * 2.0f) {
                                f32 amount = fabsf(longitudinal) > radius * 5.0f + 0.1f
                                                 ? 1.0f
                                                 : (fabsf(longitudinal) - radius * 2.0f) /
                                                       (radius * 5.0f + 0.1f - radius * 2.0f);
                                i32 ahead = forwards ? longitudinal > 0.0f : longitudinal < 0.0f;
                                f32 factor = other->apiobj.field_0x287 ? 1.0f : 0.75f;
                                if (!ahead)
                                    speed *= 1.0f - factor * amount;
                            }
                        }
                    }
                }
            }
            speed *= water_mul;
            object->target_velocity.z = speed;
            if (object->character_context == 0x3a) {
                object->target_velocity.x =
                    ((PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA)
                         ? (object->field_0x7a3 == 0 ? 10.5f : -10.5f)
                         : (object->field_0x7a3 == 0 ? 0.4f : -0.4f) * api.character_data->game_character->run_speed);
                NuVecRotateY(&object->target_velocity, &object->target_velocity, api.facing_angle);
            } else if (__builtin_expect((api.flags_low & 0x80) != 0, 1) &&
                       (PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA) && object->in_narrow_socket) {
                special_lean = (i32)(8192.0f * pad->input_direction_z);
                object->target_velocity.x =
                    PodSprint_InStartCountdown(WORLD) > 0.0f ? 0.0f : 35.0f * pad->input_direction_z * 0.25f;
                NuVecRotateY(&object->target_velocity, &object->target_velocity, object->yrot);
            } else {
                object->target_velocity.x = 0.0f;
                NuVecRotateY(&object->target_velocity, &object->target_velocity, api.facing_angle);
            }
        }
        if ((api.flags_low & 0x80) != 0)
            CharPivot_Check(object, &object->target_velocity);
        if (WORLD->current_level == SPEEDERCHASEA_LDATA && object->id == id_SPEEDERBIKE &&
            (MiniCutCam != 0 || (!disable_narrow_socks && (api.flags_low & 0x80) != 0 &&
                                 (other = GetOtherActivePlayer(object)) != NULL && other->apiobj.field_0x287 != 0)))
            object->target_velocity.x = object->target_velocity.z = 0.0f;
        api.velocity.x = SeekValF(api.velocity.x, object->target_velocity.x, seek_rate);
        api.velocity.z = SeekValF(api.velocity.z, object->target_velocity.z, seek_rate);
        if (in_tube) {
            object->target_velocity.y = 5.0f;
            api.velocity.y = SeekValF(api.velocity.y, object->target_velocity.y, 10.0f);
        } else if (__builtin_expect((api.character_data->game_character->field_0x94 & 0x20) != 0, 0) &&
                   ((pad->buttons_held & GAMEPAD_JUMP) != 0 || object->character_context != 0x4b)) {
            object->target_velocity.y = (pad->buttons_held & GAMEPAD_JUMP) != 0
                                            ? api.character_data->game_character->run_speed * 0.7f
                                            : -api.character_data->game_character->run_speed * 0.7f;
            api.velocity.y = SeekValF(api.velocity.y, object->target_velocity.y, 3.0f);
        } else if (object->character_context == 0x4b) {
            object->target_velocity.y =
                (api.water_height - (api.collision_min.y + (api.collision_max.y - api.collision_min.y) * 0.2f)) * 3.0f;
            api.velocity.y = SeekValF(api.velocity.y, object->target_velocity.y, 10.0f);
        }
        if (WORLD->current_level == PLATFORM_LDATA) {
            api.position.x = 11.805f;
            api.position.y = 3.0f;
            api.position.z = -26.105f;
            object->target_velocity = api.velocity = v000;
        }
        if (special_lean != 0)
            object->movement_lean_angle = SeekRot(object->movement_lean_angle, special_lean, 3.0f);
        else if ((api.character_data->game_character->flags_090 & 1) != 0) {
            i32 angular_speed = 0;
            if (object->character_context != 0x36 && object->character_context != 0x2a &&
                object->character_context != 0x3a) {
                angular_speed = (i32)(RotDiff(object->previous_movement_angle, api.field_0x276) / FRAMETIME);
                if (WORLD->current_level == PLATFORM_LDATA) {
                    if (NeedsPretendAnim(object)) {
                        u16 phase = (i32)(NuFmod(GameTimer.time_elapsed, 2.1f) / 2.1f * 65536.0f);
                        angular_speed =
                            (i32)(angular_speed + 4551.0f * NuTrigTable[((phase - 0x2000) >> 1) & 0x7fff]);
                    }
                }
            }
            f32 rate = 8.0f;
            if ((api.character_data->game_character->flags_090 & 0x10000) != 0) {
                angular_speed /= 4;
                rate = 4.0f;
            } else if (turn_multiplier != 0.0f)
                angular_speed = (i32)(angular_speed * (1.0f / turn_multiplier));
            if (angular_speed < -65536)
                angular_speed = -8192;
            else if (angular_speed > 65536)
                angular_speed = 8192;
            else {
                angular_speed /= 4;
                if (angular_speed < -8192)
                    angular_speed = -8192;
                else if (angular_speed > 8192)
                    angular_speed = 8192;
            }
            object->movement_lean_angle = SeekRot(object->movement_lean_angle, angular_speed, rate);
        } else
            object->movement_lean_angle = SeekRot(object->movement_lean_angle, 0, 10.0f);
        if (object->id == id_WOOKIEFLYER) {
            u16 target = object->character_context == 0x17 ? 0 : (i32)(-2730.0f * object->field_0xdc8);
            object->secondary_lean_angle = SeekRot(object->secondary_lean_angle, target, 5.0f);
        }
        if (WORLD->current_level == PLATFORM_LDATA) {
            if (object->character_context != 0x2a && object->character_context != 0x36 &&
                object->character_context != 0x3a) {
                u16 target = 0;
                if (object->pad_gamepad->input_magnitude > 0.0f)
                    target = -(i32)((1.0f - abs(RotDiff(api.field_0x276, input_yaw)) * (1.0f / 32768.0f)) * 4551.0f);
                object->secondary_lean_angle = SeekRot(object->secondary_lean_angle, target, 3.0f);
            }
            return;
        }
    }
vehicle_collision:
    GizmoBlowupCheckProximity(WORLD, object);
    if ((VehicleArea || (WORLD->area == SPEEDERCHASE_ADATA && object->id == id_SPEEDERBIKE)) &&
        !(WORLD->area != NULL && WORLD->area == PODSPRINT_ADATA))
        VehicleCollisionCode(object);
}

// Original: 43 bytes.
i32 TwistLevel(LEVELDATA_s *level) {
    return DOGFIGHTA_LDATA != NULL && level == DOGFIGHTA_LDATA;
}

void AveragePlayerCurrentSpeedMul() {
    i32 count = 0;
    avg_currentspeed_mul = 0.0f;
    f32 total = 0.0f;
    if (Player[0] != NULL && Player[0]->apiobj.player_controlled) {
        total += Player[0]->current_speed_mul;
        count = 1;
    }
    if (Player[1] == NULL || !Player[1]->apiobj.player_controlled) {
        avg_currentspeed_mul = total;
    } else {
        total += Player[1]->current_speed_mul;
        avg_currentspeed_mul = total;
        if (__builtin_expect(count == 1, 1)) {
            avg_currentspeed_mul = total * 0.5f;
        }
    }
}

f32 ForceAlongSock(GameObject_s *object) {
    if (object->sock_position.location.sock == -1 || CurrentSpeed == 0.0f)
        return 0.0f;
    f32 speed = (1.0f + object->field_0xc38) * (CurrentSpeed * avg_currentspeed_mul);
    NUVEC force = {0.0f, 0.0f, speed};
    NuVecRotateX(&force, &force, object->sock_position.midpoint_rotation.x);
    NuVecRotateY(&force, &force, object->sock_position.midpoint_rotation.y);
    NuVecAdd(&object->target_velocity, &object->target_velocity, &force);
    return speed;
}

// Original: 1,336 bytes.
i32 MovePlayer_TWIST(GameObject_s *object) {
    WORLDINFO *world = WorldInfo_CurrentlyActive();
    GAMEPAD_s *pad = object->pad_gamepad;
    if (object->apiobj.field_0x27c == -1 || !TwistLevel(world->current_level) || world->sock_sys == NULL)
        return 0;
    object->field_0x1086 = 4;
    if (object->sock_position.location.sock == -1 ||
        (world->sock_sys->sock[object->sock_position.location.sock].flags & 1))
        return 0;
    i32 yaw_lean = static_cast<i32>(2730.0f * pad->input_direction_z);
    i32 pitch_lean = static_cast<i32>(-pad->input_direction_x * 2730.0f);
    NUMTX orientation;
    SockRotationMatrix(world->sock_sys, &object->sock_position, &orientation, 3, 5);
    object->vehicle_orientation = orientation;
    u16 previous_lean;
    if (object->field_0xe20 & 0x20) {
        GameObject_s *vehicle;
        MoveInactiveVehicle(object, 1, &vehicle);
        if (vehicle != NULL) {
            object->apiobj.field_0x276 = vehicle->apiobj.field_0x276;
            object->apiobj.movement_facing_angle = vehicle->apiobj.field_0x276;
            object->apiobj.velocity = vehicle->apiobj.velocity;
            object->apiobj.facing_angle = vehicle->apiobj.field_0x276;
            object->field_0xdc8 = vehicle->field_0xdc8;
            object->vehicle_orientation = vehicle->vehicle_orientation;
            object->movement_lean_angle = 0;
            object->tertiary_lean_angle = 0;
            object->secondary_lean_angle = 0;
            object->apiobj.roll_angle = 0;
            object->apiobj.pitch_angle = 0;
            object->sock_position = vehicle->sock_position;
            previous_lean = 0;
        } else {
            previous_lean = object->tertiary_lean_angle;
        }
    } else {
        object->target_velocity = {0.0f, 0.0f, 0.0f};
        f32 speed = (pad->input_magnitude / object->apiobj.character_data->game_character->run_speed) * 8.0f;
        if (speed > 0.0f) {
            object->target_velocity.x = pad->input_direction_z * speed;
            object->target_velocity.y = -pad->input_direction_x * speed;
        }
        KeepVehicleOnScreen(object, 1, 1, 1);
        object->target_velocity.z = 0.0f;
        NuVecMtxRotate(&object->target_velocity, &object->target_velocity, &object->vehicle_orientation);
        ForceAlongSock(object);
        SeekVec(&object->apiobj.velocity, &object->apiobj.velocity, &object->target_velocity,
                object->apiobj.character_data->game_character->velocity_seek_rate);
        previous_lean = object->tertiary_lean_angle;
    }
    object->tertiary_lean_angle = SeekRot(previous_lean, static_cast<u16>(yaw_lean), 5.0f);
    object->movement_lean_angle = SeekRot(object->movement_lean_angle, static_cast<u16>(yaw_lean), 5.0f);
    object->secondary_lean_angle = SeekRot(object->secondary_lean_angle, static_cast<u16>(pitch_lean), 5.0f);
    return 1;
}

void CurrentStart(GameObject_s *object, i32 require_twist_level, i32 use_socket_rotation) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    *reinterpret_cast<f32 *>(&object->field_0xc3c) = 0.0f;
    if (object->field_0x661 == 0xff || world->sock_sys == NULL)
        return;

    if (world->sock_sys->sock[static_cast<i8>(object->field_0x661)].current_speed == 0.0f ||
        (require_twist_level == 0 && TwistLevel(world->current_level) == 0))
        return;

    SOCK &socket = world->sock_sys->sock[static_cast<i8>(object->field_0x661)];
    NUVEC current = {0.0f, 0.0f, socket.current_speed};
    *reinterpret_cast<f32 *>(&object->field_0xc3c) = socket.current_speed;
    const f32 multiplier =
        (object->field_0xf02 & 0x20) == 0 ? object->current_speed_mul : object->current_speed_multiplier;
    current.z *= multiplier;
    if (use_socket_rotation != 0) {
        NuVecRotateX(&object->apiobj.velocity, &current, object->sock_position.midpoint_rotation.x);
        NuVecRotateY(&object->apiobj.velocity, &object->apiobj.velocity, object->sock_position.midpoint_rotation.y);
    } else {
        NuVecRotateY(&object->apiobj.velocity, &object->apiobj.velocity, object->apiobj.field_0x276);
    }
    reinterpret_cast<u8 *>(object)[0x4a6] &= static_cast<u8>(~4u);
    object->target_velocity = object->apiobj.velocity;
}

i32 GunshipInLevel(LEVELDATA_s *level) {
    if (BONUS_GUNSHIPA_LDATA == NULL)
        return 0;
    return BONUS_GUNSHIPA_LDATA == level;
}

i32 CircleLevel(LEVELDATA_s *level) {
    return BONUS_GUNSHIPB_LDATA != NULL && level == BONUS_GUNSHIPB_LDATA;
}

// Original TU near 0x153a40.
i32 PodLevel(AREADATA_s *area) {
    return PODRACE_ADATA != NULL && PODRACE_ADATA == area;
}

// Original: 1,232 bytes.
i32 MovePlayer_GUNSHIPIN(GameObject_s *object) {
    WORLDINFO *world = WorldInfo_CurrentlyActive();
    GAMEPAD_s *pad = object->pad_gamepad;
    f32 input_z = pad->input_direction_x;
    f32 magnitude = pad->input_magnitude;
    f32 input_x = pad->input_direction_z;
    if (object->apiobj.field_0x27c == -1)
        return 0;
    if (!GunshipInLevel(world->current_level))
        return 0;
    if (world->sock_sys == NULL)
        return 0;
    if (object->sock_position.location.sock == -1)
        return 0;
    object->field_0x1086 = 3;
    u16 heading = object->sock_angles.y;
    if (object->character_context == 0x23 || object->character_context == 0x24) {
        object->target_velocity = {0.0f, 0.0f, 0.0f};
        ForceAlongSock(object);
        object->apiobj.velocity.x = SeekValF(object->apiobj.velocity.x, object->target_velocity.x, 0.0f);
        object->apiobj.velocity.z = SeekValF(object->apiobj.velocity.z, object->target_velocity.z, 0.0f);
    } else if (object->field_0xe20 & 0x20) {
        GameObject_s *vehicle;
        MoveInactiveVehicle(object, 1, &vehicle);
        if (vehicle != NULL) {
            object->apiobj.field_0x276 = vehicle->apiobj.field_0x276;
            object->apiobj.velocity = vehicle->apiobj.velocity;
            object->apiobj.facing_angle = vehicle->apiobj.field_0x276;
            object->field_0xdc8 = vehicle->field_0xdc8;
            object->movement_lean_angle = 0;
            object->tertiary_lean_angle = 0;
            object->secondary_lean_angle = 0;
            object->apiobj.roll_angle = 0;
            object->apiobj.pitch_angle = 0;
        }
    } else {
        GAMECHARACTERDATA *data = object->apiobj.character_data->game_character;
        f32 seek_rate = data->velocity_seek_rate;
        object->target_velocity.y = 0.0f;
        if (object->apiobj.movement_direction.x != 0.0f || object->apiobj.movement_direction.z != 0.0f) {
            object->target_velocity.x = object->apiobj.movement_direction.x;
            object->target_velocity.z = object->apiobj.movement_direction.z;
            object->apiobj.movement_direction.x = 0.0f;
            object->apiobj.movement_direction.z = 0.0f;
        } else {
            if (((u8 *)LevFlag)[1] != 0) {
                magnitude = data->run_speed;
                input_x = object->apiobj.position.x < -1.0f ? 1.0f : object->apiobj.position.x > 1.0f ? -1.0f : 0.0f;
                input_z = 0.0f;
            }
            object->target_velocity.z = 0.0f;
            if (magnitude > 0.0f) {
                object->target_velocity.x = input_x * magnitude;
                if (GUNSHIPAHACK != 0)
                    object->target_velocity.z = input_z * magnitude;
            } else {
                object->target_velocity.x = 0.0f;
            }
            if (object->field_0x1024 > 0.0f && (object->flicker_flags & 7) != 0) {
                f32 maximum = 1.5f * data->run_speed;
                f32 impulse = (object->field_0x1024 / 0.4f) * maximum;
                if ((object->flicker_flags & 7) == 3) {
                    f32 speed = object->target_velocity.z * 0.5f + impulse;
                    object->target_velocity.z = maximum < speed ? maximum : speed;
                } else {
                    object->target_velocity.z = object->target_velocity.z * 0.5f - impulse;
                    if (-maximum > object->target_velocity.z)
                        object->target_velocity.z = -maximum;
                }
            }
            NuVecRotateY(&object->target_velocity, &object->target_velocity, heading);
            ForceAlongSock(object);
        }
        object->apiobj.velocity.x = SeekValF(object->apiobj.velocity.x, object->target_velocity.x, seek_rate);
        object->apiobj.velocity.z = SeekValF(object->apiobj.velocity.z, object->target_velocity.z, seek_rate);
    }
    object->apiobj.movement_facing_angle = heading;
    object->apiobj.facing_angle = SeekRot(object->apiobj.facing_angle, heading, 8.0f);
    object->apiobj.field_0x276 = object->apiobj.facing_angle;
    object->apiobj.pitch_angle = SeekRot(object->apiobj.pitch_angle, 0, 8.0f);
    u16 lean = (object->character_context == 0x23 || object->character_context == 0x24)
                   ? 0
                   : static_cast<u16>(static_cast<i32>(input_x * 5461.0f));
    object->movement_lean_angle = SeekRot(object->movement_lean_angle, lean, 8.0f);
    return 1;
}

// Original: 1,000 bytes.
i32 MovePlayer_CIRCLE(GameObject_s *object) {
    WORLDINFO *world = WorldInfo_CurrentlyActive();
    GAMEPAD_s *pad = object->pad_gamepad;
    if (object->apiobj.field_0x27c == -1)
        return 0;
    if (!CircleLevel(world->current_level) || world->sock_sys == NULL || object->sock_position.location.sock == -1)
        return 0;
    object->field_0x1086 = 3;
    u16 heading = world->sock_sys->sock[object->sock_position.location.sock].input_yaw +
                  object->sock_position.midpoint_rotation.y;
    object->current_input_angle = heading + pad->input_angle;
    object->field_0xe22 |= GAMEOBJECT_E22_FLAG_INPUT_ANGLE_VALID;
    object->apiobj.movement_facing_angle = heading;
    object->apiobj.facing_angle = SeekRot(object->apiobj.facing_angle, heading, 8.0f);
    object->apiobj.field_0x276 = object->apiobj.facing_angle;
    object->movement_lean_angle = SeekRot(object->movement_lean_angle,
                                          static_cast<u16>(static_cast<i32>(5461.0f * pad->input_direction_z)), 8.0f);
    GAMECHARACTERDATA *data = object->apiobj.character_data->game_character;
    f32 seek_rate = data->velocity_seek_rate;
    if (object->character_context == 0x23 || object->character_context == 0x24) {
        object->target_velocity = {0.0f, 0.0f, 0.0f};
    } else if (object->field_0xe20 & 0x20) {
        GameObject_s *vehicle;
        MoveInactiveVehicle(object, 0, &vehicle);
        if (vehicle != NULL) {
            object->apiobj.field_0x276 = vehicle->apiobj.field_0x276;
            object->apiobj.movement_facing_angle = vehicle->apiobj.field_0x276;
            object->apiobj.facing_angle = vehicle->apiobj.field_0x276;
            object->apiobj.velocity = vehicle->apiobj.velocity;
            object->field_0xdc8 = vehicle->field_0xdc8;
            object->movement_lean_angle = 0;
            object->tertiary_lean_angle = 0;
            object->secondary_lean_angle = 0;
            object->apiobj.pitch_angle = 0;
            object->apiobj.roll_angle = 0;
        }
        return 1;
    } else {
        object->target_velocity.y = 0.0f;
        if (object->apiobj.movement_direction.x != 0.0f || object->apiobj.movement_direction.z != 0.0f) {
            object->target_velocity.x = object->apiobj.movement_direction.x;
            object->target_velocity.z = object->apiobj.movement_direction.z;
            object->apiobj.movement_direction.x = 0.0f;
            object->apiobj.movement_direction.z = 0.0f;
        } else {
            object->target_velocity.z = 0.0f;
            object->target_velocity.x =
                pad->input_magnitude > 0.0f ? pad->input_magnitude * pad->input_direction_z : 0.0f;
            if (object->field_0x1024 > 0.0f && (object->flicker_flags & 7) != 0) {
                f32 maximum = 1.5f * data->run_speed;
                f32 impulse = (object->field_0x1024 / 0.4f) * maximum;
                if ((object->flicker_flags & 7) == 3) {
                    object->target_velocity.x = object->target_velocity.x * 0.5f - impulse;
                    if (-maximum > object->target_velocity.x)
                        object->target_velocity.x = -maximum;
                } else {
                    f32 speed = object->target_velocity.x * 0.5f + impulse;
                    object->target_velocity.x = maximum < speed ? maximum : speed;
                }
            }
            KeepVehicleOnScreen(object, 1, 0, 0);
            NuVecRotateY(&object->target_velocity, &object->target_velocity, heading);
        }
        ForceTowardsMid(object);
    }
    object->apiobj.velocity.x = SeekValF(object->apiobj.velocity.x, object->target_velocity.x, seek_rate);
    object->apiobj.velocity.z = SeekValF(object->apiobj.velocity.z, object->target_velocity.z, seek_rate);
    return 1;
}

// Original: 2,011 bytes.
void MovePlayer_ROLLING(GameObject_s *object) {
    struct {
        NUANGVEC angles;
        NUVEC position;
        NUVEC velocity;
        NUMTX matrix;
    } local __attribute__((aligned(16)));
    object->field_0xe23 &= ~0x10;
    object->target_velocity.y = ObjInTube(object) ? 1.25f : 0.0f;
    f32 seek_rate = object->id == id_DRAGBOMB && (object->field_0xf01 & 2)
                        ? 1.0f
                        : object->apiobj.character_data->game_character->velocity_seek_rate;
    object->target_velocity.x = 0.0f;
    object->target_velocity.z = 0.0f;
    if (object->character_context == 0x2b) {
        if (1.0f > object->turn_braking) {
            object->target_velocity.x = (1.0f - object->turn_braking) * 0.0f;
            object->target_velocity.z = object->target_velocity.x;
            if (object->field_0xe36 == 2) {
                object->target_velocity.x *= 0.5f;
                object->target_velocity.z *= 0.5f;
            }
        }
        if (object->field_0xe36 == 2) {
            NewRumble(object->pad_gamepad->pad, (qrand() * 1.5259022e-05f) * 0.5f, 0);
            i32 effect = WORLD->debris_sys->entries[58].effect;
            if (effect != -1) {
                i32 count = ParticlesPerSecond(60.0f, FRAMETIME);
                if (count > 0) {
                    NUVEC &position = local.position;
                    position.x = object->apiobj.collision_position.x;
                    position.y = object->apiobj.water_height;
                    position.z = object->apiobj.collision_position.z;
                    AddVariableShotDebrisEffect(effect, &position, count, 0, 0);
                }
            }
        }
    }
    object->apiobj.velocity.x = SeekValF(object->apiobj.velocity.x, object->target_velocity.x, seek_rate);
    object->apiobj.velocity.z = SeekValF(object->apiobj.velocity.z, object->target_velocity.z, seek_rate);
    if (WORLD->area != NULL && WORLD->area == GUNSHIP_ADATA)
        GunShip_DragBombSeekBlowUp(object);
    object->field_0x1086 = 0;
    local.matrix = object->apiobj.field_0xb8;
    NUMTX &matrix = local.matrix;
    matrix.m30 = matrix.m31 = matrix.m32 = 0.0f;
    NUANGVEC &angles = local.angles;
    angles.y = NuAtan2D(object->apiobj.velocity.x, object->apiobj.velocity.z);
    NUVEC &local_velocity = local.velocity;
    NuVecRotateY(&local_velocity, &object->apiobj.velocity, -angles.y);
    NuMtxRotateYInline(&matrix, -angles.y);
    NuMtxRotateXInline(&matrix, static_cast<i32>(((local_velocity.z * FRAMETIME) / object->apiobj.collision_radius) *
                                                 10430.3779296875f));
    NuMtxRotateYInline(&matrix, angles.y);
    NuMtxGetEulerXYZ(&matrix, &angles.x, &angles.y, &angles.z);
    object->apiobj.pitch_angle = angles.x;
    object->apiobj.field_0x276 = angles.y;
    object->apiobj.roll_angle = angles.z;
    GizmoBlowupCheckProximity(WORLD, object);
}

// Original: 1,376 bytes.
i32 MovePlayer_POD(GameObject_s *object) {
    WORLDINFO *world = WorldInfo_CurrentlyActive();
    GAMEPAD_s *pad = object->pad_gamepad;
    f32 input = pad->input_direction_z;
    if (object->apiobj.field_0x27c == -1)
        return 0;
    if (!PodLevel(world->area))
        return 0;
    if (object->current_speed_mul == 0.0f)
        return 1;
    i32 index = object->apiobj.field_0x27c;
    if (index != -1 && WORLD->current_level == PODRACEB_LDATA && podrace_section != 1 &&
        (FadeSys.fade != 0.0f || MiniCutCam != 0) && object->sock_position.location.sock != -1 &&
        object->sock_position.distance < 20.0f)
        input = -0.625f;
    if (index <= 1) {
        pod_roll_target[index] = SeekLinearF(pod_roll_target[index], input, (6.0f * FRAMETIME) * fabsf(input));
        pod_roll_target[index] = SeekLinearF(pod_roll_target[index], 0.0f, 3.0f * FRAMETIME);
        pod_roll[index] = SeekValF(pod_roll[index], pod_roll_target[index], 8.0f);
    }
    object->field_0x1086 = 2;
    u16 heading = object->sock_position.location.sock == -1 ? 0 : object->sock_angles.y;
    u16 pitch = 0, roll = 0;
    if (object->apiobj.field_0x218 != 2000000.0f) {
        f32 height = object->apiobj.collision_min.y - object->apiobj.field_0x218;
        f32 blend;
        if (height < 0.5f)
            blend = 1.0f;
        else if (height < 1.5f)
            blend = 1.0f - (height - 0.5f);
        else
            blend = 0.0f;
        FindAnglesZX(&object->surface_normal, NULL, NULL);
        i32 x = RotDiff(0, temp_xrot);
        i32 z = RotDiff(0, temp_zrot);
        pitch = static_cast<u16>(static_cast<i32>((f32)x * blend));
        roll = static_cast<u16>(static_cast<i32>((f32)z * blend));
    }
    object->apiobj.pitch_angle = SeekRot(object->apiobj.pitch_angle, pitch, 8.0f);
    object->apiobj.roll_angle = SeekRot(object->apiobj.roll_angle, roll, 8.0f);
    if (object->field_0xe20 & 0x20) {
        MoveInactiveVehicle(object, 1, NULL);
    } else {
        object->target_velocity = {0.0f, 0.0f, 0.0f};
        if (object->sock_position.location.sock != -1) {
            object->target_velocity.x = index <= 1
                                            ? pod_roll[index] * object->apiobj.character_data->game_character->run_speed
                                            : input * pad->input_magnitude;
        }
        KeepVehicleOnScreen(object, 1, 1, 1);
        if (object->target_velocity.x != 0.0f || object->target_velocity.y != 0.0f || object->target_velocity.z != 0.0f)
            NuVecRotateY(&object->target_velocity, &object->target_velocity, heading);
        if (object->sock_position.location.sock != -1)
            ForceAlongSock(object);
        f32 seek_rate = object->apiobj.character_data->game_character->velocity_seek_rate;
        object->apiobj.velocity.x = SeekValF(object->apiobj.velocity.x, object->target_velocity.x, seek_rate);
        object->apiobj.velocity.z = SeekValF(object->apiobj.velocity.z, object->target_velocity.z, seek_rate);
    }
    object->apiobj.movement_facing_angle = NuAtan2D(object->target_velocity.x, object->target_velocity.z);
    object->apiobj.facing_angle = SeekRot(object->apiobj.facing_angle, object->apiobj.movement_facing_angle, 2.0f);
    object->apiobj.field_0x276 = object->apiobj.facing_angle;
    return 1;
}

void StartTurn(GameObject_s *object) {
    object->airborne_action_duration = TURNTIME;
    object->context_animation_timer = TURNTIME;
    object->field_0x770 = TURNTIME;
    object->character_context = 0x2a;
    object->context_animation = 1;
    PlaySfx("XWing_LoopDeLoop", &object->apiobj.collision_position);
}

void LoopCode(GameObject_s *object, i32 jump_pressed, i32, GAMEPAD_s *pad, i32 allow_loop) {
    if (WORLD->area != NULL) {
        if (WORLD->area == DOGFIGHT_ADATA) {
            allow_loop = 0;
        } else if (WORLD->area == PODSPRINT_ADATA) {
            return;
        }
    }

    if (object->character_context == 0x3a) {
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer <= 0.0f) {
            object->character_context = -1;
        }
        return;
    }

    if (object->character_context == 0x36) {
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer <= 0.0f) {
            object->character_context = -1;
            object->secondary_lean_angle = 0;
            object->delayed_turn_timer = 0.3f;
        } else {
            const f32 phase = object->context_animation_timer / object->airborne_action_duration;
            const i32 index = static_cast<i32>((1.0f - phase) * 32768.0f + 16384.0f);
            const f32 curve = (NuTrigTable[(index >> 1) & 0x7fff] + 1.0f) * 0.5f;
            object->secondary_lean_angle = static_cast<i16>(static_cast<i32>((1.0f - curve) * 65536.0f));
        }
        return;
    }

    if (!(object->apiobj.character_data->game_character->field_0x88 > 0.0f) || object->character_context != -1) {
        return;
    }

    bool turn = false;
    GameObject_s *other = GetOtherActivePlayer(object);
    if (allow_loop != 0 && object->in_narrow_socket != 0 && other != NULL && other->in_narrow_socket != 0 &&
        other->character_context == 0x36 && other->context_animation_timer < other->airborne_action_duration - 0.2f &&
        other->context_animation_timer > 0.3f &&
        GoingForwardsAlongNarrowSock(other) == GoingForwardsAlongNarrowSock(object)) {
        goto start_loop;
    }

    if (!(object->delayed_turn_timer <= 0.0f)) {
        return;
    }

    i32 angle_difference;
    if (TwistLevel(WORLD->current_level) != 0) {
        angle_difference = RotDiff(0, pad->input_angle);
    } else {
        angle_difference = RotDiff(object->apiobj.field_0x276, GamePad_InputAngle(object, pad));
    }

    if (object->movement_spline == NULL && pad->input_magnitude > 0.0f) {
        i32 roll_direction = 0;
        if (static_cast<u32>(angle_difference + 0x5fff) <= 0x58e2) {
            roll_direction = 2;
        } else if (static_cast<u32>(angle_difference - 0x71d) <= 0x58e2) {
            roll_direction = 1;
        }

        if (roll_direction != 0) {
            if (jump_pressed == 0) {
                return;
            }
            if (object->in_narrow_socket != 0 && other != NULL &&
                (other->character_context == 0x36 || other->character_context == 0x2a)) {
                return;
            }

            object->airborne_action_duration = 0.75f;
            object->context_animation_timer = 0.75f;
            object->character_context = 0x3a;
            object->field_0x7a3 = static_cast<u8>(roll_direction - 1);
            object->context_animation = 1;
            PlaySfx("XWing_LoopDeLoop", &object->apiobj.collision_position);
            if (object->apiobj.player_controlled) {
                Hint_SetComplete(0x617);
            }
            return;
        }

        const i32 absolute_difference = angle_difference < 0 ? -angle_difference : angle_difference;
        turn = absolute_difference > 0x3fff;
    }

    if (allow_loop == 0 || jump_pressed == 0) {
        return;
    }
    if (object->in_narrow_socket != 0 && other != NULL &&
        (other->character_context == 0x36 || other->character_context == 0x2a || other->character_context == 0x3a)) {
        return;
    }

    if (turn) {
        StartTurn(object);
        if (object->apiobj.player_controlled) {
            Hint_SetComplete(0x617);
        }
        return;
    }

start_loop:
    object->character_context = 0x36;
    object->context_animation = 1;
    object->airborne_action_duration = LOOPTIME;
    object->context_animation_timer = LOOPTIME;
    PlaySfx("XWing_LoopDeLoop", &object->apiobj.collision_position);
    Hint_SetComplete(0x287);
    if (object->apiobj.player_controlled) {
        Hint_SetComplete(0x617);
    }
}

void TurnCodeCamSafe(GameObject_s *object, numtx_s *matrix) {
    static i32 TURNPITCH __attribute__((used)) = 200;
    static i16 myang __attribute__((used));
    TURNPITCH = 200;
    myang = 20;
    const f32 time = object->context_animation_timer;
    const f32 duration = object->airborne_action_duration;
    const f32 turn_duration = duration * 0.95f;
    if (time < turn_duration) {
        const f32 pitch_duration = duration * 0.5f * 0.5f;
        if (time < pitch_duration) {
            const f32 phase = 1.0f - (1.0f / pitch_duration) * time;
            const i32 index = static_cast<i32>(phase * 16384.0f + 32768.0f + 16384.0f);
            const f32 wave = NuTrigTable[(index >> 1) & 0x7fff];
            NuMtxPreRotateX(matrix, static_cast<i16>(static_cast<i32>((wave + 1.0f) * -3640.0f)));
        }
        const f32 phase = 1.0f - (1.0f / turn_duration) * time;
        const i32 index = static_cast<i32>(phase * 32768.0f + 16384.0f);
        const f32 wave = NuTrigTable[(index >> 1) & 0x7fff];
        NuMtxPreRotateZ(matrix, static_cast<i16>(static_cast<i32>((1.0f - (wave + 1.0f) * 0.5f) * 32768.0f)));
    }
}

void TurnCode(GameObject_s *object, i32 mode, GAMEPAD_s *pad) {
    if (WORLD->area != NULL && (WORLD->area == DOGFIGHT_ADATA || WORLD->area == PODSPRINT_ADATA)) {
        return;
    }

    if (object->character_context != 0x2a) {
        if (object->character_context != -1 && object->character_context != 0x36) {
            return;
        }

        u8 player_active = 0;
        GameObject_s *other = NULL;
        if (object->apiobj.player_controlled) {
            other = GetOtherActivePlayer(object);
            player_active = object->apiobj.flags_low & 0x80;
        }

        i32 finish_turn = 0;
        if (object->character_context == 0x36) {
            finish_turn = static_cast<u16>(object->secondary_lean_angle) <= 0x4000;
        }

        if (player_active != 0 && object->delayed_turn_timer <= 0.0f && object->in_narrow_socket) {
            if (!((object->character_context != 0x36 || finish_turn) && other != NULL)) {
                if (other != NULL && (other->character_context == 0x36 || other->character_context == 0x2a ||
                                      other->character_context == 0x3a)) {
                    return;
                }
            } else {
                if (other->in_narrow_socket) {
                    const i32 forwards = GoingForwardsAlongNarrowSock(object);
                    const i32 other_forwards = GoingForwardsAlongNarrowSock(other);
                    if (forwards != other_forwards) {
                        if (other->character_context == 0x2a &&
                            0.75f * other->airborne_action_duration <= other->context_animation_timer) {
                            return;
                        }
                        StartTurn(object);
                        return;
                    }
                    if (!object->in_narrow_socket) {
                        other = NULL;
                    }
                }
                if (other != NULL && (other->character_context == 0x36 || other->character_context == 0x2a ||
                                      other->character_context == 0x3a)) {
                    return;
                }
            }
        } else if (object->in_narrow_socket && other != NULL &&
                   (other->character_context == 0x36 || other->character_context == 0x2a ||
                    other->character_context == 0x3a)) {
            return;
        }

        if ((object->field_0xe24 & 2) == 0 && (mode == 0 || object->character_context != -1)) {
            if ((object->field_0xeff & 0x20) == 0 && UnderPlayerControl(object) == 0) {
                return;
            }
            if (!(pad->input_magnitude > 0.0f)) {
                return;
            }

            const u16 input_angle = GamePad_InputAngle(object, pad);
            if (object->in_narrow_socket) {
                i32 turn = 0;
                if (object->delayed_turn_timer <= 0.0f) {
                    if (abs(RotDiff(object->yrot, object->apiobj.movement_facing_angle)) <= 0x4000) {
                        turn = abs(RotDiff(object->yrot, input_angle)) > 0x6000;
                    } else {
                        turn = abs(RotDiff(object->yrot, input_angle)) < 0x2000;
                    }
                }

                if (((Player[0] != object && Player[1] != object) ||
                     !(fabsf(object->pad_gamepad->waggle_magnitude) > 0.25f)) &&
                    abs(RotDiff(object->apiobj.field_0x276, input_angle)) > 0x71c6) {
                    if (object->character_context != 0x36) {
                        StartTurn(object);
                        return;
                    }
                    object->field_0xe24 |= 4;
                    turn = finish_turn;
                }
                if (!turn) {
                    return;
                }
            } else {
                if ((Player[0] == object || Player[1] == object) &&
                    fabsf(object->pad_gamepad->waggle_magnitude) > 0.25f) {
                    return;
                }
                if (abs(RotDiff(object->apiobj.field_0x276, input_angle)) <= 0x71c6) {
                    return;
                }
                if (object->character_context == 0x36) {
                    object->field_0xe24 |= 4;
                    if (!finish_turn) {
                        return;
                    }
                }
                if (object->apiobj.player_controlled) {
                    Hint_SetComplete(0x617);
                }
            }
        }

        StartTurn(object);
        return;
    }

    const f32 half_duration = object->airborne_action_duration * 0.5f;
    const f32 quarter_duration = half_duration * 0.5f;
    f32 timer;
    if ((object->field_0xe24 & 4) == 0) {
        timer = object->context_animation_timer;
    } else {
        const f32 lean = static_cast<f32>(object->secondary_lean_angle) * (1.0f / 16384.0f);
        object->field_0xe24 &= ~4;
        timer = object->airborne_action_duration - lean * quarter_duration;
    }

    timer -= FRAMETIME;
    object->context_animation_timer = timer;
    if (timer <= 0.0f) {
        const i32 outside =
            OutSideSplineArea(&object->apiobj.collision_position, WORLD->camera_splines[16], NULL, NULL, 0);
        const i32 inside =
            OutSideSplineArea(&object->apiobj.collision_position, WORLD->camera_splines[17], NULL, NULL, 1);
        if (outside != 0 || inside != 0) {
            object->context_animation_timer = FRAMETIME;
            return;
        }

        const u16 angle = object->apiobj.facing_angle + 0x8000;
        object->field_0xe24 &= ~2;
        object->character_context = -1;
        object->apiobj.movement_facing_angle = angle;
        object->apiobj.field_0x276 = angle;
        object->apiobj.facing_angle = angle;

        TORPEDOPACKET *packet = object->torpedo;
        if (packet != NULL) {
            for (u32 i = 0; i < packet->count; ++i) {
                packet->pickup_data[i] += 0x8000;
                packet->pickup_flags[i] = 0x8000 - packet->pickup_flags[i];
            }
        }

        object->secondary_lean_angle = 0;
        object->movement_lean_angle = 0;
        object->tertiary_lean_angle = 0;
        object->field_0xdc8 = 1.0f;
        object->field_0xe24 &= ~4;
        return;
    }

    if (timer >= half_duration + quarter_duration) {
        object->secondary_lean_angle = static_cast<i16>(static_cast<i32>(
            (1.0f - (1.0f / quarter_duration) * (timer - (half_duration + quarter_duration))) * 16384.0f));
        return;
    }
    if (!(timer >= half_duration)) {
        return;
    }
    const i32 index = (static_cast<i32>((1.0f - (1.0f / quarter_duration) * (timer - half_duration)) * 16384.0f +
                                        49152.0f + 16384.0f) >>
                       1) &
                      0x7fff;
    object->secondary_lean_angle = static_cast<i16>(static_cast<i32>(20024.0f * NuTrigTable[index] + 16384.0f));
}

void ApplyExtraRotation(GameObject_s *object, numtx_s *matrix) {
    switch (object->character_context) {
        case 42:
            TurnCodeCamSafe(object, matrix);
            break;
        case 58: {
            const f32 phase = 1.0f - object->context_animation_timer / object->airborne_action_duration;
            const f32 wave = NuTrigTable[(static_cast<i32>(32768.0f * phase + 16384.0f) >> 1) & 0x7fff];
            const i32 rotation = static_cast<i32>((1.0f - (wave + 1.0f) * 0.5f) * 65536.0f);
            if (object->field_0x7a3 == 0) {
                NuMtxPreRotateZ(matrix, rotation);
            } else {
                NuMtxPreRotateZ(matrix, -rotation);
            }
            break;
        }
    }

    if (object->id == id_MINISLAVE1) {
        NuMtxPreRotateX(matrix, -16384);
        NuMtxPreRotateY(matrix, 32768);
    }
}

void StartLaunch(GameObject_s *object) {
    if (object->field_0x7a5 == 0x2c)
        return;
    Player_ClearContext(object, 1);
    Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    object->field_0x7a5 = 0x2c;
    object->field_0x7a3 = 0;
    object->saved_position = object->launch_origin = object->apiobj.position;
}

void StartFlatten(GameObject_s *source, GameObject_s *target) {
    if (target->character_context == 0x33) {
        return;
    }
    if (target->character_context != 0x3d) {
        GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24);
        if ((data->flags_090 & 0x40) != 0) {
            return;
        }
        if (data->field_0x28 != 0.0f && target->field_0xe31 == 1) {
            return;
        }
        if ((source->apiobj.field_0x1f8 & 4) != 0) {
            return;
        }
        if (target->apiobj.field_0x27d == 0) {
            return;
        }

        Player_ClearContext(target, 0);
        Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(target->player_packet));
        target->character_context = 0x3d;
        target->context_animation_timer = 0.0f;
        target->apiobj.velocity.x *= 0.5f;
        target->apiobj.velocity.z *= 0.5f;
        target->context_animation = target->apiobj.character_model->model_data_b[0x85] != NULL ? 0x85 : 5;
        target->airborne_action_duration = static_cast<f32>(qrand()) * (1.0f / 65535.0f) + 1.0f;

        NUVEC direction;
        NuVecSub(&direction, &target->apiobj.collision_position, &source->apiobj.collision_position);
        u16 angle = static_cast<u16>(NuAtan2D(direction.x, direction.z));
        i32 reverse = 0;
        if (direction.x * target->facing_direction.x + direction.z * target->facing_direction.z < 0.0f) {
            angle += 0x8000;
            reverse = 1;
        }
        target->field_0x7a3 = reverse;
        if (target->context_animation != 0x85) {
            target->apiobj.field_0x276 = angle;
            target->apiobj.facing_angle = angle;
            target->apiobj.movement_facing_angle = angle;
        }
        target->delayed_turn_timer = 0.0f;
        NewBuzzFrames(source->pad_gamepad->pad, 1, 0);
        data = static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24);
        GameAudio_PlaySfxById(data->sfx_hurt, &target->apiobj.collision_position, 0, 0);
        NewBuzz(target->pad_gamepad->pad, 0.1f, 0);
    }
}

void FlattenCode(GameObject_s *object) {
    if (object->character_context != 0x3d)
        return;
    GameObject_s *nearest = NULL;
    f32 nearest_distance = 1000000000.0f;
    GameObject_s *candidate = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++candidate) {
        if ((candidate->apiobj.field_0x1f8 & 0x1001) != 0x1001 || candidate->apiobj.field_0x287 != 0 ||
            (candidate->apiobj.character_data->game_character->flags_090 & 0x1000) == 0 ||
            (CInfo[candidate->character_context].flags & 0x8000) != 0)
            continue;
        f32 radius = object->apiobj.field_0x1dc + candidate->apiobj.field_0x1dc + 0.1f;
        f32 distance = NuVecDistSqr(&object->apiobj.collision_position, &candidate->apiobj.collision_position, NULL);
        if (distance < radius * radius && !object->apiobj.player_controlled) {
            object->context_animation_timer = 0.0f;
            return;
        }
        if (distance < nearest_distance && candidate != object) {
            nearest_distance = distance;
            nearest = candidate;
        }
    }
    object->context_animation_timer += FRAMETIME;
    if (object->context_animation_timer >= object->airborne_action_duration) {
        object->character_context = -1;
        if (object->apiobj.character_model->model_data_b[6] != NULL) {
            StartJump(object, 0);
            if (nearest != NULL && object->apiobj.player_controlled) {
                NUVEC direction;
                NuVecSub(&direction, &object->apiobj.position, &nearest->apiobj.position);
                direction.y = 0.0f;
                NuVecNorm(&direction, &direction);
                NuVecScale(&direction, &direction, 12.0f);
                NuVecAdd(&object->apiobj.velocity, &object->apiobj.velocity, &direction);
            }
        }
    } else {
        NewRumble(object->pad_gamepad->pad, (qrand() * (1.0f / 65535.0f)) * 0.4f, 0);
    }
}

void TakeHitCode(GameObject_s *object) {
    f32 timer;
    f32 frame_time;
    f32 impact_speed;
    f32 impact_threshold;
    i16 animation;
    i8 context;
    i32 animation_active;
    i32 kill_parts_mode;
    i32 hearts;
    i32 second_flag;
    i32 first_flag;
    u32 coins;
    bool has_coins;

    animation = object->context_animation;
    animation_active = 1;
    if (animation == -1) {
        animation_active = 0;
    } else if (object->apiobj.character_model->model_data_b[animation] != NULL &&
               AnimPlaying(&object->apiobj.anim_packet, animation, 1, 0) == 0) {
        animation_active = 0;
    }

    context = object->character_context;
    if (context == 0x5a)
        goto context_5a;
    if (context != 0x5f) {
        if (context != 0x15 || animation_active == 0)
            return;
        timer = object->context_animation_timer;
        frame_time = FRAMETIME;
        object->context_animation_timer = timer - frame_time;
        if (0.0f < timer - frame_time)
            return;
        goto finish_context;
    }

    if (animation_active == 0)
        return;
    timer = object->context_animation_timer + FRAMETIME;
    object->context_animation_timer = timer;
    if (timer >= object->airborne_action_duration)
        goto finish_airborne;
    if (timer > 0.5f)
        goto check_ground_impact;
    return;

check_ground_impact:
    if (0.0f > object->reset_velocity.y && object->apiobj.field_0x27d != 0)
        goto finish_airborne;
    if (object->field_0x1084 == 0)
        return;
    impact_speed = object->terrain_impact_speed;
    impact_threshold = timer / object->airborne_action_duration * -0.8f + 0.8f;
    if (impact_speed > impact_threshold)
        goto finish_airborne;
    return;

finish_airborne:
    kill_parts_mode = 1;
    if (Arcade != 0 && static_cast<u8>(object->hit_variant) < 2)
        Arcade_Kill(object->hit_variant, object->apiobj.field_0x27c);
    goto kill_object;

context_5a:
    if (animation_active == 0)
        return;
    timer = object->context_animation_timer;
    frame_time = FRAMETIME;
    object->context_animation_timer = timer - frame_time;
    if (0.0f < timer - frame_time)
        return;
    if (object->field_0x7a3 != 0) {
        if (object->field_0x7a3 != 1)
            return;
        goto finish_context;
    }

    animation = object->context_animation;
    object->movement_runtime_flags |= 0x80;
    if (animation == 0xb9) {
        kill_parts_mode = 0;
        goto kill_object;
    }
    if (animation == 0x3d)
        goto finish_context;
    if (animation == 0xa7)
        object->context_animation = 0xaa;
    else if (animation == 0xa8)
        object->context_animation = 0xab;
    else if (animation == 0xa9)
        object->context_animation = 0xac;
    else
        object->context_animation = 1;

    object->field_0x7a3 = 1;
    animation = object->context_animation;
    if (object->apiobj.character_model->model_data_b[animation] != NULL &&
        (static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[animation])->flags & 2) == 0) {
        object->context_animation_timer = AnimDuration(object->id, animation, 0.0f, 0.0f, 1);
        return;
    }
    object->context_animation_timer = object->field_0x768;
    return;

finish_context:
    object->character_context = -1;
    return;

kill_object:
    KillParts(object, -1, -1, 1, 0.0f, kill_parts_mode, NULL);
    KillGameObject(object, 2, 0);
    if (object->apiobj.field_0x27c != -1)
        return;

    if (BonusArea != 0) {
        coins = static_cast<u16>(object->apiobj.character_data->game_character->field_0xee);
        has_coins = static_cast<i32>(coins) > 0;
    release_hearts:
        hearts = ReleaseHearts();
        if (hearts == 0 && !has_coins)
            return;
        if (BonusArea == 0) {
            if (coins > 2499) {
                first_flag = 0;
                second_flag = 0;
                goto add_pickups;
            }
        no_bonus_under_limit:
            first_flag = 1;
            second_flag = 0;
            goto add_pickups;
        }
    } else {
        if (Cheat_IsOn(0x10) == 0) {
            has_coins = false;
            coins = 0;
            goto release_hearts;
        } else {
            hearts = ReleaseHearts();
            coins = 350;
            if (BonusArea == 0)
                goto no_bonus_under_limit;
        }
    }

    first_flag = 0;
    second_flag = 1;
add_pickups:
    AddPickups(coins, hearts, 0, 0, &object->apiobj.collision_position, NULL, 2.0f, -1, 1.0f, 2000000.0f, NULL,
               first_flag, second_flag, false);
}

void ReleaseEat(GameObject_s *object) {
    u8 flags = object->field_0xe24;
    GameObject_s *eaten = static_cast<GameObject_s *>(object->field_0x780);
    if (eaten != NULL && (flags & 1) != 0) {
        KillGameObject(eaten, 2, 0);
        flags = object->field_0xe24;
        object->field_0x780 = NULL;
    }
    if (static_cast<u8>(object->field_0x7a5 - 0x38) <= 1) {
        object->field_0x780 = NULL;
        object->field_0x7a5 = 0xff;
    }
    object->field_0xe24 = flags & ~1;
}

void GrabVictim(GameObject_s *object, GameObject_s *victim) {
    if (object->character_context != -1) {
        return;
    }

    object->character_context = 0x38;
    i16 animation = 0x64;
    i32 animation_offset = 0x190;
    if (victim->id == id_GAMORREANGUARD) {
        animation_offset = victim->apiobj.field_0x27c == -1 ? 0x7c : 0x190;
        animation = victim->apiobj.field_0x27c == -1 ? 0x1f : 0x64;
    }
    object->context_animation = animation;
    if (*(void **)((char *)object->apiobj.character_model->model_data_b + animation_offset) != NULL) {
        object->airborne_action_duration = AnimDuration(object->id, animation, 0.0f, 0.0f, 1);
    } else {
        object->airborne_action_duration = 2.0f;
    }
    object->blowup_target = NULL;
    object->context_animation_timer = 0.0f;
    object->field_0x780 = victim;
    object->force_throw_target = NULL;
    object->field_0xe24 &= ~1;
    object->context_flags &= ~0x40;
}

void EatVictim(GameObject_s *object) {
    object->character_context = -1;
    if (object->field_0x780 == NULL || (object->field_0xe24 & 1) == 0) {
        return;
    }
    object->context_animation = 0x4e;
    if (object->apiobj.character_model->model_data_b[0x4e] == NULL) {
        return;
    }
    object->character_context = 0x3f;
    object->context_animation_timer = AnimDuration(object->id, 0x4e, 0.0f, 0.0f, 1);
    object->context_flags &= ~0x40;
}

GameObject_s *ObjOpponent(GameObject_s *object, f32 range, f32 extra_radius, i32 allow_untargeted, i32 mode,
                          i32 player_filter) {
    NUVEC delta;
    if (forceNextAttackOpponent.Get() != NULL) {
        if ((object->apiobj.flags_low & 0x80) != 0) {
            return forceNextAttackOpponent->GetCharacterObject();
        }
    }
    i32 ignore_ai = objopponent_ignoreaiopponent;
    objopponent_ignoreaiopponent = 0;
    GAMEPAD_s *pad = object->pad_gamepad;
    u16 heading = pad->input_magnitude > 0.0f ? GamePad_InputAngle(object, pad) : object->apiobj.movement_facing_angle;
    NUVEC forward;
    NuVecRotateY(&forward, &v001, heading);
    ComboOpponent_Behind = 0;
    GameObject_s *nearest = NULL;
    f32 nearest_distance = 1.0e8f;
    GameObject_s *target = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++target) {
        if (test_ai_combo != 0 && (object->apiobj.flags_low & 0x80) == 0 && object->ai.action_target_ref != NULL &&
            *object->ai.action_target_ref != NULL) {
            target = *object->ai.action_target_ref;
            i = HIGHGAMEOBJECT;
        }
        if ((object->apiobj.flags_low & 0x80) != 0 && nextShootTarget.Get() != NULL &&
            nextShootTarget->GetCharacterObject() != target)
            continue;
        if (target == object || (target->apiobj.field_0x1f8 & 0x1001) != 0x1001 || target->apiobj.field_0x287 != 0 ||
            target->apiobj.collision_min.y > object->apiobj.collision_max.y)
            continue;
        if ((player_filter == 1 && target->apiobj.field_0x27c != -1) ||
            (player_filter == 2 && target->apiobj.field_0x27c == -1))
            continue;
        i8 context = target->character_context;
        if ((context == 0x17 &&
             (target->current_hp == 0 ||
              (static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24)->flags_090 & 0x40) !=
                  0)) ||
            (CInfo[context].flags & 0x8000) != 0 || static_cast<u8>(context & 0xfd) == 0x39 || context == 0x3c)
            continue;
        if (context == 0) {
            if (static_cast<u8>(target->action_movement_state - 3) < 2)
                continue;
        } else if (static_cast<u8>(context - 0xd) < 2 || context == 0x35 || context == 0x47 || context == 0x46)
            continue;
        GAMECHARACTERDATA *runtime = static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24);
        if ((runtime->flags_090 & 0x8000) != 0 || (object->field_0xcc0 != NULL && target == object->field_0xcc0))
            continue;
        u32 object_flags = object->apiobj.field_0x1f4;
        u32 target_flags = target->apiobj.field_0x1f4;
        if ((object_flags & 0x10000) == 0 && (target_flags & 0x10000) == 0 && (object->apiobj.flags_low & 0x80) == 0 &&
            ((object_flags ^ target_flags) & 5) == 0)
            continue;
        if (mode != 0) {
            if (static_cast<u8>(context + 0xa1) < 2 ||
                (context == 0x5a && (target->field_0x7a3 == 0 || (object->apiobj.flags_low & 0x80) == 0)) ||
                (context == 0x17 && target->hitpoints == 0))
                continue;
            if (object->apiobj.field_0x27c != -1) {
                if ((object->apiobj.flags_low & 0x80) != 0) {
                    if (target->apiobj.field_0x27c == -1) {
                        if (WORLD->current_level != HUB_LDATA && (target_flags & 0x10005) == 0)
                            continue;
                    } else {
                        if (((object_flags ^ target_flags) & 1) == 0 &&
                            (object->ai.action_target_ref != NULL || target->ai.action_target_ref != NULL))
                            continue;
                        if (CanPunchGirls == 0 && (object->field_0xf01 & 8) == 0 && (target->field_0xf01 & 8) != 0 &&
                            object->id != id_GAMORREANGUARD) {
                            i32 move = SpecialMove_Check(object, target);
                            if (move == -1 || SpecialMove_GetFlags(move, 1) == 0)
                                continue;
                        }
                    }
                } else if (target->apiobj.field_0x27c != -1 ||
                           (WORLD->current_level != HUB_LDATA && (target_flags & 0x10005) == 0))
                    continue;
            }
            if ((target->apiobj.character_data->model_flags & 0x2000) != 0 ||
                ((((GAMECHARACTERDATA_s *)target->apiobj.character_data->field11_0x24)->flags_090 & 0x40) != 0 &&
                 (target->apiobj.flags_low & 0x80) == 0))
                continue;
        }
        if (!(allow_untargeted != 0 && (ignore_ai != 0 || object->ai.action_target_ref == NULL || mode != 0)) &&
            (target_flags & 1) == 0)
            continue;

        f32 distance = NuVecDistSqr(&target->apiobj.position, &object->apiobj.position, &delta);
        if (!(distance < range * range))
            continue;
        i32 behind = 0;
        if (mode == 1) {
            if (object->combo_stage == 0 && forward.x * delta.x + forward.z * delta.z < 0.0f) {
                if (object->apiobj.character_model->model_data_b[0x94] == NULL)
                    continue;
                behind = 1;
            }
        } else if (forward.x * delta.x + forward.z * delta.z < 0.0f) {
            continue;
        }
        if (extra_radius > 0.0f) {
            f32 radius = object->apiobj.field_0x1dc + target->apiobj.field_0x1dc + extra_radius;
            if (distance >= radius * radius)
                continue;
        }
        if (distance < nearest_distance) {
            nearest = target;
            nearest_distance = distance;
            ComboOpponent_Behind = behind;
        }
    }
    ComboOpponent_Range2 = nearest_distance;
    return nearest;
}

static void SelectOpponent(GameObject_s *object, f32 range, f32 extra_radius, i32 mode, i32 gizmo_first) {
    if (__builtin_expect(forceNextAttackOpponent.Get() != NULL && object->apiobj.player_controlled, 1)) {
        object->force_target = forceNextAttackOpponent->GetCharacterObject();
        object->blowup_target = forceNextAttackOpponent->GetGizBlowup();
        return;
    }

    object->force_target = NULL;
    object->blowup_target = NULL;
    if (gizmo_first == 0) {
        object->force_target = ObjOpponent(object, range, extra_radius, 1, mode, 1);
        if (object->force_target != NULL)
            return;
    }
    if (gizmo_first == 0 && !object->apiobj.player_controlled) {
        if (__builtin_expect(object->blowup_target == NULL, 1))
            object->force_target = ObjOpponent(object, range, extra_radius, 1, mode, 2);
        return;
    }

    object->blowup_target = GizmoBlowUpOpponent(object, range, extra_radius, 0.0f, 4, 0, 0, 0);
    if (object->blowup_target == NULL && gizmo_first == 0) {
        object->force_target = ObjOpponent(object, range, extra_radius, 1, mode, 2);
    }
}

static void PunchCode(GameObject_s *object, i32 pressed, i32 held, i32 require_target, i32 suppress_combo,
                      f32 cooldown) {
    const f32 punch_gap = PUNCHGAP;
    const f32 punch_charge_gap = PUNCHCHARGAP;
    if (require_target != 0) {
        require_target = (object->use_action != 5) + 1;
    }
    if (object->attack_override == 7) {
        return;
    }
    if (object->use_action == 5) {
        pressed = 1;
    }

    if (object->character_context == 0x26) {
        if (object->field_0x7a7 != -1 && object->force_target == NULL) {
            if ((object->field_0xe23 & 0x40) != 0) {
                FastWeaponOut(object, 1);
            }
            object->character_context = -1;
            if (cooldown != 0.0f) {
                object->quick_shoot_timer = cooldown;
            }
            return;
        }

        f32 *frame = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
        if (frame == NULL) {
            return;
        }
        if (object->combo_input_latched == 0 && pressed != 0) {
            if (object->apiobj.player_controlled) {
                if ((object->context_flags & 0x40) != 0 ||
                    object->context_animation_timer <= object->airborne_action_duration) {
                    object->combo_input_latched = 2;
                } else {
                    object->combo_input_latched = 1;
                }
            } else {
                i32 random = qrand();
                if (random < 0) {
                    random += 0x7fff;
                }
                object->combo_input_latched = (random >> 15) + 1;
            }
        }

        if (object->combo_input_latched != 0 && object->context_animation_timer <= object->airborne_action_duration) {
            object->context_animation_timer = 0.0f;
        } else {
            object->context_animation_timer -= FRAMETIME;
        }
        if (object->context_animation_timer > 0.0f) {
            if ((object->context_flags & 0x40) != 0) {
                return;
            }
            f32 hit_frame = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
            if (hit_frame <= 0.0f || *frame < hit_frame) {
                return;
            }
            Punch_Hit(object, object->force_target, punch_gap, punch_charge_gap);
            return;
        }

        object->character_context = -1;
        if (object->field_0x7a7 != -1) {
            if (SpecialMove_GetFlags(object->field_0x7a7, 8) != 0) {
                if (object->force_target != NULL) {
                    SetFlicker(object->force_target, 0.4f);
                    if (static_cast<i8>(object->force_target->apiobj.flags_low) < 0) {
                        TakeHitRumble(object, 0.7f);
                    }
                }
                HitRumble(object);
            }
        } else {
            if ((object->context_flags & 0x40) == 0) {
                Punch_Hit(object, object->force_target, punch_gap, punch_charge_gap);
            }
            if (suppress_combo == 0 && static_cast<i8>(object->context_flags) >= 0 && object->combo_stage < 2 &&
                object->combo_input_latched != 0 && object->attack_override != 6) {
                SelectOpponent(object, 1.25f, punch_charge_gap, 1, 0);
                if (require_target == 0 || object->force_target != NULL || object->blowup_target != NULL) {
                    i32 alternate_animations[4];
                    u8 combo_stage = object->combo_stage++;
                    if ((object->context_flags & 1) != 0) {
                        alternate_animations[0] = -1;
                        alternate_animations[1] = -1;
                        alternate_animations[2] = -1;
                        alternate_animations[3] = -1;
                        if (combo_stage != 0) {
                            if (object->combo_branch == 2) {
                                object->combo_branch = (object->combo_input_latched == 2) + 5;
                            } else {
                                object->combo_branch = (object->combo_input_latched == 2) + 3;
                            }
                            alternate_animations[0] = 6;
                            alternate_animations[1] = 5;
                            alternate_animations[2] = 4;
                            alternate_animations[3] = 3;
                        } else {
                            if (object->combo_input_latched == 2) {
                                object->combo_branch = 2;
                                alternate_animations[0] = 1;
                            } else {
                                object->combo_branch = 1;
                                alternate_animations[0] = 2;
                            }
                        }
                        object->context_animation = object->queued_context_animation + object->combo_branch;
                        if (object->apiobj.character_model->model_data_b[object->context_animation] == NULL &&
                            alternate_animations[0] != -1) {
                            for (i32 i = 0; i < 4; ++i) {
                                i32 animation = object->queued_context_animation + alternate_animations[i];
                                if (object->apiobj.character_model->model_data_b[animation] != NULL) {
                                    object->context_animation = animation;
                                    break;
                                }
                            }
                        }
                    } else {
                        if (combo_stage == 0) {
                            object->context_animation = 0x55;
                        } else {
                            object->context_animation = 0x56;
                        }
                    }

                    if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL) {
                        f32 duration = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 0);
                        if (duration > 0.0f) {
                            object->character_context = 0x26;
                            object->airborne_action_duration =
                                (object->context_flags & 1) != 0 && object->combo_stage < 2 ? 0.2f : 0.0f;
                            object->combo_input_latched = 0;
                            object->context_flags &= ~0x40;
                            object->context_animation_timer = duration + object->airborne_action_duration;
                            ResetAnimPacket(&object->apiobj.anim_packet, -1);
                            if ((object->apiobj.character_data->game_character->flags_098[0] & 0x10) == 0) {
                                if ((AnimMiscFlags(object->apiobj.character_model, object->context_animation) & 4) !=
                                    0) {
                                    PlaySfx(const_cast<char *>("WhipSwish"), &object->apiobj.collision_position);
                                } else {
                                    PlaySfx(const_cast<char *>("Swipe"), &object->apiobj.collision_position);
                                }
                            }
                            if (object->force_target == NULL && object->blowup_target == NULL) {
                                object->apiobj.movement_facing_angle = GamePad_InputAngle(object, object->pad_gamepad);
                            }
                        }
                    }
                }
            } else if ((held != 0 || object->attack_override == 6) &&
                       (object->id == id_GAMORREANGUARD || object->id == id_CAPTAINTARPALS)) {
                StartHold(object);
            }
        }

        if (object->character_context != -1) {
            return;
        }
        if ((object->field_0xe23 & 0x40) != 0) {
            FastWeaponOut(object, 1);
        }
        if (cooldown != 0.0f) {
            object->quick_shoot_timer = cooldown;
        }
        return;
    }

    if (cooldown != 0.0f && object->quick_shoot_timer > 0.0f) {
        object->quick_shoot_timer -= FRAMETIME;
        return;
    }
    if ((object->apiobj.field_0x27d == 0 || object->character_context != -1) &&
        objInNetWaitContext(object, 0x26) == 0) {
        return;
    }
    if (pressed == 0) {
        return;
    }

    SelectOpponent(object, 1.25f, punch_charge_gap, 1, object->use_action == 5);
    if (object->force_target != NULL) {
        if (object->apiobj.player_controlled && object->force_target->character_context != 0x30) {
            i32 special_move = SpecialMove_Check(object, object->force_target);
            if (special_move != -1) {
                object->field_0x7a7 = special_move;
                object->context_variant_flags |= 2;
                object->context_animation = SpecialMove_GetAttackerAction(object->field_0x7a7);
                object->combo_stage = 2;
                object->context_flags &= ~0x80;
                goto start_attack;
            }
            if (object->force_target == NULL) {
                goto check_blowup_target;
            }
        }
        if (ComboOpponent_Behind != 0) {
            goto behind_attack;
        }
    } else {
        if (!object->apiobj.player_controlled && object->attack_override == 0 && object->use_action != 5) {
            return;
        }
    }

check_blowup_target:
    if (object->blowup_target != NULL && GizmoBlowUpOpponent_Behind != 0) {
        object->field_0x7a7 = -1;
    behind_attack:
        object->context_animation = 0x94;
        object->combo_stage = 0;
        object->context_flags |= 0x80;
    } else {
        object->field_0x7a7 = -1;
        switch (object->attack_override) {
            case 1:
                object->context_animation = 0x51;
                object->combo_stage = 2;
                object->context_flags &= ~0x80;
                break;
            case 2:
                object->context_animation = 0x55;
                object->combo_stage = 2;
                object->context_flags &= ~0x80;
                break;
            case 3:
                object->context_animation = 0x56;
                object->combo_stage = 2;
                object->context_flags &= ~0x80;
                break;
            case 4:
                goto behind_attack;
            case 5:
                object->context_animation = 0x64;
                break;
            case 6:
                StartHold(object);
                return;
            case 7:
                return;
            default:
                object->context_animation = 0x51;
                object->combo_stage = 0;
                object->context_flags &= ~0x80;
                break;
        }
    }

start_attack:
    object->context_flags &= ~1;
    if (require_target == 2 && object->force_target == NULL && object->blowup_target != NULL) {
        return;
    }
    if (object->apiobj.character_model->model_data_b[object->context_animation] == NULL) {
        return;
    }
    if (require_target != 0 && object->force_target == NULL && object->blowup_target == NULL) {
        return;
    }

    f32 duration = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 0);
    if (duration > 0.0f) {
        u8 context_flags = object->context_flags;
        object->character_context = 0x26;
        object->combo_input_latched = 0;
        object->context_animation_timer = duration;
        object->context_flags = context_flags & ~0x40;
        object->airborne_action_duration = (context_flags & 1) != 0 ? 0.2f : 0.0f;
        if (static_cast<i8>(object->context_flags) >= 0) {
            object->context_animation_timer += object->airborne_action_duration;
        }
        ResetAnimPacket(&object->apiobj.anim_packet, -1);
        if ((object->apiobj.character_data->game_character->flags_098[0] & 0x10) == 0) {
            if ((AnimMiscFlags(object->apiobj.character_model, object->context_animation) & 4) != 0) {
                PlaySfx(const_cast<char *>("WhipSwish"), &object->apiobj.collision_position);
            } else {
                PlaySfx(const_cast<char *>("Swipe"), &object->apiobj.collision_position);
            }
        }
        if ((object->apiobj.character_data->game_character->flags_090 & 0x20) == 0) {
            bool weapon_out = true;
            if ((object->field_0xe22 & 1) == 0) {
                weapon_out = object->field_0xe32 == 1;
            }
            object->field_0xe23 = (object->field_0xe23 & ~0x40) | (weapon_out << 6);
            SetWeaponIn(object);
        } else {
            object->field_0xe23 |= 0x40;
            SetWeaponOut(object);
        }

        if (object->force_target != NULL && object->field_0x7a7 != -1) {
            GameObject_s *target = object->force_target;
            target->character_context = 0x30;
            target->field_0x7a7 = object->field_0x7a7;
            target->force_target = object;
            target->context_animation = SpecialMove_GetVictimAction(target->field_0x7a7);
            target->context_animation_timer = AnimDuration(target->id, target->context_animation, 0.0f, 0.0f, 0);
            ResetAnimPacket(&target->apiobj.anim_packet, -1);
            i16 angle = NuAtan2D(target->apiobj.position.x - object->apiobj.position.x,
                                 target->apiobj.position.z - object->apiobj.position.z);
            object->apiobj.movement_facing_angle = angle;
            target->apiobj.movement_facing_angle = angle - 0x8000;
        }
    }
}

static u8 SetComboOpponent(GameObject_s *object, f32 range, i32 update_heading, i32 mode) {
    NUVEC delta;
    object->force_target = NULL;
    object->blowup_target = NULL;
    bool searched;
    i32 result;
    if (__builtin_expect(object->apiobj.player_controlled && mode == 0 && object->pad_gamepad->input_magnitude > 0.0f,
                         1)) {
        object->force_target = ObjOpponent(object, range, 0.0f, 0, 0, 1);
        searched = true;
        if (object->force_target != NULL) {
            result = 1;
            if (__builtin_expect((object->apiobj.flags_low & 0x80) != 0, 1))
                goto finished;
            goto nonplayer_target;
        }
    } else {
        searched = false;
    }
    if (object->ai.primary_target_ref != NULL && object->ai.nearest_opponent_metric < range) {
        ComboOpponent_Range2 = object->ai.nearest_opponent_metric;
        object->force_target = *object->ai.primary_target_ref;
        result = 1;
        goto finished;
    }
    if ((object->apiobj.flags_low & 0x80) == 0) {
        result = 0;
    nonplayer_target:
        if ((object->field_0xef8 & 0x40) != 0 || object->use_action == 5) {
            object->blowup_target = GizmoBlowUpOpponent(object, range, 0.0f, 0.0f, mode, 0, 0, 0);
            if (object->blowup_target != NULL) {
                ComboOpponent_Range2 = GizmoBlowUpOpponent_Range2;
                result = 4;
            }
        }
        goto finished;
    }
    if (__builtin_expect(!searched, 1)) {
        object->force_target = ObjOpponent(object, range, 0.0f, 0, 0, 1);
        if (object->force_target != NULL) {
            result = 1;
            goto finished;
        }
    }
    object->blowup_target = GizmoBlowUpOpponent(object, range, 0.0f, 0.0f, mode, 0, 0, 0);
    if (object->blowup_target != NULL) {
        ComboOpponent_Range2 = GizmoBlowUpOpponent_Range2;
        result = 4;
        goto finished;
    }
    {
        NUVEC forward;
        f32 range_squared = range * range;
        if (object->pad_gamepad->input_magnitude > 0.0f) {
            NuVecRotateY(&forward, &v001, GamePad_InputAngle(object, object->pad_gamepad));
        }
        f32 nearest_distance = 1.0e8f;
        GameObject_s *nearest = NULL;
        GameObject_s *target = Obj;
        for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++target) {
            if (target == object || (target->apiobj.field_0x1f8 & 0x1001) != 0x1001 ||
                target->apiobj.field_0x287 != 0 || (target->apiobj.field_0x1f4 & 1) != 0)
                continue;
            i8 context = target->character_context;
            if ((context & ~2) == 0x39 || context == 0x3c || (GetGameCharacterData(target)->flags_090 & 0x8000) != 0 ||
                (CInfo[context].flags & 0x8000) != 0 || (object->field_0xcc0 != NULL && target == object->field_0xcc0))
                continue;

            f32 distance = NuVecDistSqr(&target->apiobj.position, &object->apiobj.position, &delta);
            if (!(distance < range_squared) ||
                (object->pad_gamepad->input_magnitude > 0.0f && forward.x * delta.x + forward.z * delta.z < 0.0f))
                continue;
            u16 angle = NuAtan2D(target->apiobj.position.x - object->apiobj.position.x,
                                 target->apiobj.position.z - object->apiobj.position.z);
            i32 difference = RotDiff(object->apiobj.movement_facing_angle, angle);
            if (difference >= -0x2000) {
                if (difference <= 0x2000 && distance < nearest_distance) {
                    nearest = target;
                    nearest_distance = distance;
                }
            }
        }
        PlayerOpponent_Range2 = nearest_distance;
        object->force_target = nearest;
        if (__builtin_expect(nearest != NULL, 0)) {
            ComboOpponent_Range2 = nearest_distance;
            result = 2;
            goto finished;
        }
    }
    result = 0;
    if (__builtin_expect(!searched, 0)) {
        object->force_target = ObjOpponent(object, range, 0.0f, 0, 0, 1);
        result = object->force_target != NULL;
    }
finished:
    if (update_heading != 0 && object->force_target == NULL && object->pad_gamepad->input_magnitude > 0.0f &&
        (object->field_0xe22 & 0x20) != 0) {
        object->apiobj.movement_facing_angle = object->current_input_angle;
    }
    return result;
}

static void BlockCode(GameObject_s *object, i32 pressed, i32 held, i32 jump_pressed, i32 allow_start) {
    if ((object->field_0xe22 & 1) != 0 && object->weapon_scale_state == 0)
        FindIncomingThreats(object);
    if (object->character_context == 0xc) {
        GameObject_s *attacker = object->block_attacker;
        if (__builtin_expect(attacker != NULL, 1)) {
            if (jump_pressed != 0 && ComboJump(object))
                return;
            if (object->block_latch == 0 && object->blocked_attack_stage != 0) {
                if ((attacker->apiobj.field_0x1f8 & 0x1000) != 0 && attacker->apiobj.field_0x287 == 0) {
                    if ((object->field_0xe23 & 0x20) != 0) {
                        if (attacker->character_context == 0x26 &&
                            attacker->context_animation == object->blocked_attack_stage &&
                            (attacker->id != id_GAMORREANGUARD || attacker->context_animation != 0x56 ||
                             (attacker->context_flags & 0x40) == 0))
                            return;
                    } else if (attacker->character_context == 5 && attacker->combo_stage < object->blocked_attack_stage)
                        return;
                }
                object->blocked_attack_stage = 0;
                return;
            }
        }
        if (object->block_latch == 0) {
            object->context_animation_timer -= FRAMETIME;
            if (object->context_animation_timer <= 0.0f) {
                object->block_latch = 1;
                object->context_animation_timer = 0.2f;
            }
            return;
        }
        if (object->id == id_GAMORREANGUARD && (object->apiobj.flags_low & 0x80) == 0) {
            object->context_animation_timer -= FRAMETIME;
            if (__builtin_expect(object->context_animation_timer <= 0.0f, 1)) {
                object->character_context = -1;
                object->block_cooldown = 1.5f;
            }
            return;
        }
        if (__builtin_expect(pressed == 0 || (object->incoming_bolt == NULL && object->incoming_melee == NULL &&
                                              object->incoming_special == NULL && object->incoming_part == NULL),
                             1)) {
            if (held != 0 && object->incoming_bolt != NULL) {
                object->context_animation_timer = 0.2f;
                return;
            }
            object->context_animation_timer -= FRAMETIME;
            if (object->context_animation_timer > 0.0f)
                return;
            if (__builtin_expect(held != 0 &&
                                     (attacker == NULL || attacker->id != id_GAMORREANGUARD ||
                                      attacker->character_context != 0x26 || attacker->context_animation != 0x56 ||
                                      (attacker->context_flags & 0x40) == 0),
                                 0)) {
                StartHold(object);
                return;
            }
            object->character_context = -1;
            return;
        }
        if (!TouchHacks::ShouldBlock(*object))
            return;
    } else {
        if (object->block_cooldown > 0.0f) {
            object->block_cooldown -= FRAMETIME;
            return;
        }
        if (!allow_start || !pressed || object->apiobj.field_0x27d == 0 ||
            (CInfo[object->character_context].flags & 0x20) != 0 || (object->field_0xef8 & 2) == 0 ||
            (object->incoming_bolt == NULL && object->incoming_melee == NULL && object->incoming_special == NULL &&
             object->incoming_part == NULL))
            return;
    }
    if (!NewBlockAction(object)) {
        object->character_context = -1;
        return;
    }
    object->character_context = 0xc;
    object->block_latch = 0;
    if (object->incoming_bolt == NULL) {
        if (__builtin_expect(object->incoming_part != NULL, 1)) {
            object->block_attacker = NULL;
            object->blocked_bolt = NULL;
            object->blocked_part = object->incoming_part;
            object->context_animation_timer = 0.3f;
            object->blocked_attack_stage = 0;
        } else {
            object->blocked_bolt = NULL;
            object->blocked_part = NULL;
            object->context_animation_timer = 0.1f;
            if (object->incoming_special != NULL) {
                object->block_attacker = object->incoming_special;
                object->blocked_attack_stage = object->incoming_special->context_animation;
                object->field_0xe23 |= 0x20;
            } else {
                object->block_attacker = object->incoming_melee;
                object->blocked_attack_stage = object->incoming_melee->combo_stage + 1;
            }
        }
    } else {
        object->blocked_bolt = object->incoming_bolt;
        object->block_attacker = NULL;
        object->blocked_part = NULL;
        object->context_animation_timer = 0.3f;
        object->blocked_attack_stage = 0;
        if ((object->apiobj.flags_low & 0x80) != 0)
            Hint_SetComplete(0x5dd);
    }
}

static void SwipeCode(GameObject_s *object, i32 pressed, i32 held) {
    if (object->character_context == 0x10) {
        f32 *frame = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
        if (frame != NULL) {
            if ((object->context_flags & 0x40) == 0) {
                f32 hit_frame = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
                if (hit_frame >= 1.0f && hit_frame <= *frame)
                    ComboHitFrame(object, 1);
            }
            object->context_animation_timer -= FRAMETIME;
            if (object->context_animation_timer <= 0.0f) {
                object->field_0xe20 |= 4;
                object->character_context = -1;
                if (object->context_animation == 0x29) {
                    if ((object->context_flags & 0x40) == 0) {
                        ComboHitFrame(object, 1);
                        object->character_context = 0x10;
                        object->context_animation_timer = 0.001f;
                    } else if (held)
                        StartHold(object);
                }
            }
        }
    } else if ((object->field_0xe22 & 1) != 0 && object->weapon_scale_state == 0 && pressed &&
               object->apiobj.field_0x27d != 0 && object->apiobj.character_model != NULL &&
               (object->character_context == 1 || object->character_context == -1 || object->character_context == 2 ||
                object->character_context == 4 || object->character_context == 3) &&
               SetComboOpponent(object, 0.6f, 0, 2) == 1) {
        NUVEC *position;
        if (object->force_target != NULL)
            position = &object->force_target->apiobj.position;
        else if (object->blowup_target != NULL)
            position = &object->blowup_target->mid_position;
        else
            return;
        NUVEC delta;
        NuVecSub(&delta, position, &object->apiobj.position);
        NuVecRotateY(&delta, &delta, -object->apiobj.movement_facing_angle);
        if ((object->pad_gamepad->input_magnitude <= 0.0f || fabsf(delta.x) <= fabsf(delta.z)) && delta.z < 0.0f &&
            ComboOpponent_Range2 < 0.48999998f) {
            object->context_animation = 0x29;
            if (object->apiobj.character_model->model_data_b[0x29] != NULL) {
                object->context_animation_timer = AnimDuration(object->id, 0x29, 0.0f, 0.0f, 1);
                if (object->context_animation_timer > 0.0f) {
                    object->character_context = 0x10;
                    object->context_flags &= ~0x40;
                    PlaySabreSfx(NULL, object, NULL, 1);
                    NewRumble(object->pad_gamepad->pad, 0.4f, 0);
                }
            }
        }
    }
}

static void LightSabreComboCode(GameObject_s *object, i32 action_pressed, i32 action_held, i32 jump_pressed,
                                i32 buttons_pressed) {
    ANIMPACKET_s &animation = object->apiobj.anim_packet;
    const u8 player_active = object->apiobj.flags_low & 0x80;
    f32 cooldown;
    if (player_active != 0) {
        cooldown = 0.0f;
        object->ai_combo_cooldown = cooldown;
    } else {
        cooldown = object->ai_combo_cooldown;
        if (cooldown > 0.0f) {
            cooldown -= FRAMETIME;
            object->ai_combo_cooldown = cooldown;
        }
    }
    const u8 previous_context_flags = object->context_flags;
    object->context_flags &= ~2;
    if (object->character_context == CHARACTER_CONTEXT_COMBO) {
        object->field_0xef9 |= 8;
        if ((object->context_flags & 4) != 0) {
            if ((action_pressed == 0 && object->combo_input_latched == 0) || object->combo_stage > 1) {
                object->combo_input_timer -= FRAMETIME;
                if (object->combo_input_timer <= 0.0f) {
                    object->character_context = CHARACTER_CONTEXT_NONE;
                    object->context_flags |= 2;
                    if (action_held != 0)
                        StartHold(object);
                } else if (object->combo_stage < 2 && jump_pressed != 0) {
                    ComboJump(object);
                }
            } else {
                if (object->combo_stage == 1) {
                    SetComboOpponent(object, 1.0f, 1, 0);
                    object->context_flags &= ~4;
                    object->combo_stage = 2;
                    if (object->id != id_IMPERIALGUARD)
                        PlaySabreSfx(NULL, object, NULL, 1);
                    NewRumble(object->pad_gamepad->pad, 0.35f, 0);
                    if (object->combo_branch == 1) {
                        object->combo_branch =
                            (object->field_0xe20 & 0x40) != 0 &&
                                    JediHasAction(object,
                                                  static_cast<JEDI_ACTION>(object->queued_context_animation + 4))
                                ? 4
                                : 3;
                        if (object->combo_branch == 3)
                            object->field_0xe20 &= ~0x40;
                    } else {
                        object->combo_branch =
                            (object->field_0xe20 & 0x40) != 0 &&
                                    JediHasAction(object,
                                                  static_cast<JEDI_ACTION>(object->queued_context_animation + 6))
                                ? 6
                                : 5;
                        if (__builtin_expect(object->combo_branch == 5, 1))
                            object->field_0xe20 &= ~0x40;
                        else {
                            NewRumble(object->pad_gamepad->pad, 0.6f, 0);
                            if (object->id != id_IMPERIALGUARD) {
                                PlaySabreSfx("ComboSaber1", object, &object->apiobj.collision_position, 0);
                            }
                        }
                    }
                } else {
                    SetComboOpponent(object, 1.0f, 1, 0);
                    object->context_flags &= ~4;
                    object->combo_stage = 1;
                    if (object->id != id_IMPERIALGUARD)
                        PlaySabreSfx(NULL, object, NULL, 1);
                    if ((object->field_0xe20 & 0x40) != 0 &&
                        JediHasAction(object, static_cast<JEDI_ACTION>(object->queued_context_animation + 2))) {
                        object->combo_branch = 2;
                        NewRumble(object->pad_gamepad->pad, 0.5f, 0);
                        if (__builtin_expect(object->id != id_IMPERIALGUARD, 0))
                            PlaySabreSfx("ComboSaber2", object, &object->apiobj.collision_position, 0);
                    } else {
                        object->field_0xe20 &= ~0x40;
                        object->combo_branch = 1;
                        NewRumble(object->pad_gamepad->pad, 0.35f, 0);
                    }
                }
                object->context_animation = object->queued_context_animation + object->combo_branch;
                object->combo_input_latched = 0;
                object->field_0xe20 |= 0x40;
                object->context_flags &= ~0x50;
            }
            goto combo_finished;
        }
        if (animation.animation_index == object->context_animation && animation.blending == 0 &&
            animation.requested_animation == object->context_animation) {
            if (object->combo_stage == 2) {
                f32 *frames = AnimListFrameArray(object->apiobj.character_model, object->context_animation);
                if ((object->context_flags & 0x40) == 0 && frames[0] > 0.0f && animation.current_time >= frames[0]) {
                    ComboHitFrame(object, object->combo_branch == 6 ? 3 : 1);
                }
                if (frames[1] > 0.0f && animation.current_time >= frames[1])
                    object->context_flags |= 8;
                CHARACTERANIM_s *info = static_cast<CHARACTERANIM_s *>(
                    object->apiobj.character_model->model_data_a[object->context_animation]);
                if ((animation.flags & ANIMPACKET_FLAG_FINISHED) != 0 ||
                    (info->blend_out_time > 0.0f &&
                     AnimDuration(object->id, object->context_animation, animation.current_time, 0.0f, 0) <
                         info->blend_out_time)) {
                    object->character_context = CHARACTER_CONTEXT_NONE;
                    object->context_flags |= 2;
                }
                object->sabre_flags |= 2;
                if ((object->context_flags & 0x40) == 0 &&
                    (((object->context_flags & 8) != 0 &&
                      AnimListFrame(object->apiobj.character_model, object->context_animation, 0) == 0.0f) ||
                     (object->context_flags & 2) != 0)) {
                    ComboHitFrame(object, object->combo_branch == 6 ? 3 : 1);
                }
                if (object->character_context == CHARACTER_CONTEXT_NONE && action_held != 0 &&
                    (object->id != id_IMPERIALGUARD || object->combo_stage != 2))
                    StartHold(object);
                if ((object->context_flags & 0x40) != 0 && (previous_context_flags & 0x40) == 0) {
                    NewRumble(object->pad_gamepad->pad, object->combo_branch == 6 ? 0.7f : 0.35f, 0);
                }
                if ((object->apiobj.flags_low & 0x80) == 0 &&
                    ((object->field_0xefb & 8) != 0 || WORLD->current_level == VADERC_LDATA)) {
                    object->ai_combo_cooldown = 3.0f;
                }
                goto combo_finished;
            }
            if ((animation.flags & ANIMPACKET_FLAG_FINISHED) != 0) {
                object->combo_input_timer = 0.2f;
                object->context_flags |= 4;
                BlockSfx(object);
                object->sabre_flags |= 2;
                if ((object->context_flags & 0x40) == 0)
                    ComboHitFrame(object, 1);
                goto combo_finished;
            }
        }
        f32 *time = AnimPlaying(&animation, object->context_animation, 1, 0);
        if (time != NULL && (object->context_flags & 0x40) == 0) {
            const f32 frame = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
            if (frame > 0.0f && *time >= frame) {
                ComboHitFrame(object, object->combo_branch == 2 ? 2 : 1);
            }
        }
        i32 can_queue = 0;
        if (time != NULL) {
            if ((object->context_flags & 0x10) == 0) {
                const f32 frame = AnimListFrame(object->apiobj.character_model, object->context_animation, 1);
                if (frame > 0.0f && *time >= frame)
                    object->context_flags |= 0x10;
            }
            can_queue = AnimDuration(object->id, object->context_animation, *time, 0.0f, 0) < 0.1f;
            if (can_queue)
                object->field_0xe22 |= 8;
        }
        if (__builtin_expect(action_pressed != 0 && object->combo_input_latched == 0, 0)) {
            if (can_queue)
                object->combo_input_latched = 1;
            else
                object->field_0xe20 &= ~0x40;
        }
        object->sabre_flags |= 3;
        goto combo_finished;
    }

    {
        const i8 context = object->character_context;
        if (__builtin_expect(action_pressed == 0, 0) || (!player_active && cooldown > 0.0f) || context == 0x15 ||
            context == 0x5a || context == 0x13 || context == 0x2e ||
            (!player_active && static_cast<u8>(context - 6) < 2) || context == 8 || context == 0x1b ||
            context == 0x2d || context == 0xf || context == 0x27 || context == 0x28 || context == 0x59 ||
            context == 0x30 || context == 0x3d || context == 0x43 || context == 0x4a || context == 0x61 ||
            context == 0x4b) {
            if (((object->pad_gamepad->allocated_5a & 4) != 0 || buttons_pressed != 0) &&
                (object->field_0xe22 & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) != 0 &&
                object->weapon_scale_state == WEAPON_SCALE_IDLE && context != 6 && context != 7 && context != 18 &&
                context != 27 && context != 29 && context != 13 && context != 14 && context != 46 &&
                !(context == 0 && static_cast<u8>(object->action_movement_state - 3) < 2) &&
                (object->field_0xe23 & 1) == 0 && (CInfo[context].flags & 0x4000000) == 0 &&
                ((CInfo[context].flags & 0x8000000) == 0 || (object->jump_flags & 2) == 0) && context != 8) {
                GAMECHARACTERDATA *data = GetGameCharacterData(object);
                const i32 action =
                    data->field275_0x116 == 0 && (object->apiobj.character_data->model_flags & 0x80) != 0 ? 126 : 16;
                if (object->apiobj.field_0x27d != 0 && object->pad_gamepad->input_magnitude == 0.0f &&
                    object->apiobj.character_model->model_data_b[action] != NULL &&
                    (context == -1 || (CInfo[context].flags & 4) != 0)) {
                    SlowWeaponIn(object);
                } else {
                    FastWeaponIn(object, 1);
                }
            }
            goto combo_finished;
        }
        if ((object->field_0xe22 & 1) == 0) {
            GAMECHARACTERDATA *data = GetGameCharacterData(object);
            const i32 draw_action =
                data->field275_0x116 == 0 && (object->apiobj.character_data->model_flags & 0x80) != 0 ? 127 : 17;
            if (object->weapon_scale_state == 0 && object->apiobj.field_0x27d != 0 &&
                object->pad_gamepad->input_magnitude == 0.0f &&
                object->apiobj.character_model->model_data_b[draw_action] != NULL && context >= -1 && context <= 4) {
                SlowWeaponOut(object);
            } else {
                FastWeaponOut(object, 1);
            }
            return;
        }
        if ((object->field_0xe20 & 4) != 0 || (!player_active && object->weapon_scale_state != 0) ||
            object->apiobj.field_0x27d == 0 ||
            (context != 1 && context != -1 && context != 2 && context != 4 && context != 3 && context != 6 &&
             context != 7))
            goto combo_finished;
        SetWeaponOut(object);
        if (object->incoming_bolt != NULL || object->incoming_melee != NULL || object->incoming_special != NULL ||
            object->incoming_part != NULL) {
            if (!TouchHacks::ShouldBlock(*object))
                goto combo_finished;
            if (!NewBlockAction(object)) {
                object->character_context = -1;
                goto combo_finished;
            }
            object->character_context = 0xc;
            object->block_latch = 0;
            if (object->incoming_bolt != NULL) {
                object->blocked_bolt = object->incoming_bolt;
                object->block_attacker = NULL;
                object->blocked_part = NULL;
                object->context_animation_timer = 0.3f;
                object->blocked_attack_stage = 0;
                if ((object->apiobj.flags_low & 0x80) != 0)
                    Hint_SetComplete(0x5dd);
            } else if (object->incoming_part != NULL) {
                object->blocked_bolt = NULL;
                object->blocked_part = object->incoming_part;
                object->block_attacker = NULL;
                object->blocked_attack_stage = 0;
                object->context_animation_timer = 0.3f;
            } else {
                object->blocked_bolt = NULL;
                object->blocked_part = NULL;
                object->context_animation_timer = 0.1f;
                if (object->incoming_special != NULL) {
                    object->blocked_attack_stage = object->incoming_special->context_animation;
                    object->field_0xe23 |= 0x20;
                    object->block_attacker = object->incoming_special;
                } else {
                    object->blocked_attack_stage = object->incoming_melee->combo_stage + 1;
                    object->block_attacker = object->incoming_melee;
                }
            }
            goto combo_finished;
        }
        if (!player_active &&
            ((object->field_0xf04 & 2) != 0 || (object->apiobj.character_data->model_flags & 4) == 0) &&
            (object->field_0xef9 & 4) == 0)
            goto combo_finished;
        SetComboOpponent(object, 1.0f, 1, 0);
        if (object->apiobj.player_controlled && object->force_target != NULL &&
            object->force_target->apiobj.field_0x27c == -1)
            Hint_SetComplete(0x277);
        object->combo_stage = 0;
        object->character_context = CHARACTER_CONTEXT_COMBO;
        object->combo_input_timer = 0.0f;

        JEDI_ACTION action = JEDI_ACTION_COMBO_1_1;
        u8 alternate = 0;
        if (JediHasAction(object, JEDI_ACTION_COMBO_2_1) && object->combo_alternate == 0) {
            action = JEDI_ACTION_COMBO_2_1;
            alternate = 1;
        }
        object->queued_context_animation = action;
        object->context_animation = action;
        object->field_0xe22 |= GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION;
        object->field_0xe20 |= GAMEOBJECT_E20_FLAG_COMBO_MOVEMENT;
        object->context_flags &= GAMEOBJECT_CONTEXT_FLAGS_COMBO_START_RETAIN_MASK;
        object->combo_alternate = alternate;
        object->combo_branch = 0;
        object->combo_input_latched = 0;
        object->weapon_scale = 1.0f;
        object->weapon_scale_state = WEAPON_SCALE_IDLE;
        object->apiobj.velocity.y = 0.0f;
        if (object->id != id_IMPERIALGUARD)
            PlaySabreSfx(NULL, object, NULL, 1);

        if (!JediHasAction(object, action)) {
            object->character_context = CHARACTER_CONTEXT_NONE;
        }
    }
combo_finished:
    if (object->weapon_scale == 1.0f && (object->field_0xe22 & 8) == 0 && object->character_context == 5 &&
        (object->context_flags & 4) != 0)
        object->field_0xe22 |= 8;
}

void SetForcedAttackOpponent(MechObjectInterface *target) {
    forceNextAttackOpponent = NuMechPtr<MechObjectInterface, 4>(target);
}

void StartBallooning(GameObject_s *object, i32 movement_state) {
    object->field_0x7a3 = movement_state;
    object->character_context = 0x5d;
    object->context_animation = object->apiobj.character_model->model_data_b[0xb1] != NULL ? 0xb1 : 1;
    object->field_0x768 = 1000000000.0f;
}

void SetBallooningHeight(GameObject_s *object, float height) {
    if (object->character_context == 0x5d)
        object->field_0x768 = height;
}

void LetGoOfBalloon(GameObject_s *object) {
    if (object->character_context != 0x5d) {
        return;
    }
    object->character_context = -1;
    if (WORLD->lev_objs[0xf7].active == 0) {
        return;
    }
    const i32 hand = object->apiobj.character_data->game_character->hand_locators[0];
    if (hand == -1 || object->apiobj.character_model->points_of_interest[hand] == NULL) {
        return;
    }

    NUMTX_ALIGNED16 matrix;
    NuMtxSetTranslation(&matrix, NUMTX_GET_ROW_VEC(&object->joint_matrices[hand], 3));
    NUVEC velocity = {0.0f, 0.0f, 0.2f};
    NuVecMtxRotate(&velocity, &velocity, &matrix);
    ADDPART_s params = Default_ADDPART;
    params.matrix = &matrix;
    params.velocity = &velocity;
    NUVEC centre;
    NuSpecialGetRadius(&WORLD->lev_objs[0xf7].special, &centre, &params.field_14);
    params.owner = object;
    params.field_18 = params.field_14;
    params.field_28 = 0xf7;
    params.flags = 0x08000200;
    params.gravity = 0.5f;
    params.special = &WORLD->lev_objs[0xf7].special;
    params.field_90 = object->apiobj.field_0x289;
    params.field_40 = PartCollide_3D;
    params.time_step = FRAMETIME;
    PART_s *part = AddPart(&params);
    if (part != NULL) {
        part->force_flags = ObjHitObj_Flags(object) & 0xffff;
    }
}

void PopBalloon(GameObject_s *object) {
    LetGoOfBalloon(object);
}

i32 Glide_Start(GameObject_s *object) {
    if (LEGOCONTEXT_GLIDE == -1)
        return 0;
    object->character_context = 0x4f;
    object->context_animation = 0x93;
    object->context_animation_timer = 0.0f;
    object->airborne_action_duration = 0.4f;
    object->field_0x7a3 = 0;
    object->field_0x788 = NULL;
    return 1;
}

void Glide_MoveCode(GameObject_s *object) {
    f32 height;
    i8 context = object->character_context;
    if (context != 0x4f) {
        GAMECHARACTERDATA *character = object->apiobj.character_data->game_character;
        u8 character_can_glide = (character->flags_094[0] >> 6) & 1;
        if ((object->apiobj.character_data->model_flags & 0x40) != 0) {
            return;
        }

        if (!character_can_glide) {
            SUIT_s *suit = static_cast<SUIT_s *>(object->suit);
            if (suit == NULL || !suit->can_glide) {
                return;
            }
        }
        if ((object->pad_gamepad->buttons_pressed & GAMEPAD_JUMP) == 0 || object->apiobj.field_0x27d != 0) {
            return;
        }
        if (!character_can_glide && AnimPlaying(&object->apiobj.anim_packet, 5, 1, 0) == NULL) {
            return;
        }
        if (character_can_glide == 0) {
            context = object->character_context;
        }
        if (context != 0x11 && context != -1) {
            if (context != 0 || (!character_can_glide && object->context_variant_flags >= 0)) {
                return;
            }
        }
        Glide_Start(object);
        return;
    }

    if (object->apiobj.field_0x27d == 0 && object->apiobj.field_0x218 != 2000000.0f &&
        (height = object->apiobj.collision_min.y - object->apiobj.field_0x218,
         object->apiobj.field_0x1e0 <= height && height != object->apiobj.field_0x1e0) &&
        (object->pad_gamepad->buttons_held & GAMEPAD_ACTION) != 0 &&
        object->apiobj.character_model->model_data_b[0x21] != NULL) {
        if (Slam_Start(object, SLAMJUMPSPEED) != 0) {
            ResetAnimPacket(&object->apiobj.anim_packet, -1);
            ResetMiniAnimPacket(&object->mini_animation, -1);
            return;
        }
    }

    object->movement_runtime_flags |= 0x10;
    if (object->apiobj.character_model->model_data_b[object->context_animation] == NULL ||
        AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0) != NULL) {
        object->context_animation_timer += FRAMETIME;
        if (object->apiobj.field_0x27d == 0) {
            if ((object->pad_gamepad->buttons_held & GAMEPAD_JUMP) != 0) {
                if (object->airborne_action_duration <= 0.1f && object->airborne_action_duration != 0.1f) {
                    object->airborne_action_duration = 0.1f;
                }
            } else {
                object->airborne_action_duration -= FRAMETIME;
                if (object->airborne_action_duration <= 0.0f) {
                    StartEndOfJump(object);
                }
            }
        } else {
            object->character_context = -1;
        }
    }
}

// Original 0x15aae0, 330 bytes.
i32 Glide_SetTargetMom(GameObject_s *object) {
    f32 speed;
    if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL)
        speed = AnimSpeed(object->apiobj.character_model, object->context_animation);
    else
        speed = object->apiobj.character_data->game_character->run_speed;
    if (object->field_0x788 != NULL) {
        speed *= 0.25f;
        object->target_velocity.x = NU_SIN_LUT(object->apiobj.movement_facing_angle) * speed;
        object->target_velocity.z = NU_COS_LUT(object->apiobj.movement_facing_angle) * speed;
    } else {
        object->target_velocity.x = NU_SIN_LUT(object->apiobj.movement_facing_angle) * speed;
        object->target_velocity.z = NU_COS_LUT(object->apiobj.movement_facing_angle) * speed;
        f32 descent = object->apiobj.player_controlled ? -0.5f : -0.375f;
        object->target_velocity.y = descent;
        if (object->context_animation_timer < 0.5f)
            object->target_velocity.y = (object->context_animation_timer + object->context_animation_timer) * descent;
    }
    return 1;
}

void ForceNextLungeTarget(MechObjectInterface *target) {
    lungeTarget = NuMechPtr<MechObjectInterface, 4>(target);
}

void StartLunge(GameObject_s *object, f32 speed, f32 height) {
    object->context_animation_timer = 0.0f;
    object->context_flags &= ~0x80;
    object->character_context = 0;
    object->action_movement_state = 3;
    object->context_animation = 0x1f;
    if (object->apiobj.player_controlled)
        object->field_0xef9 |= 8;
    object->jump_flags |= 1;
    object->apiobj.velocity.y = speed;
    object->jump_sequence = 0;
    if (height > 0.0f)
        MakeJumpReachHeight(object, height, 0);
    PlayJumpSfx(object, 3);
    NewRumble(object->pad_gamepad->pad, 0.5f, 0);
    if (object->apiobj.character_model->model_data_b[object->context_animation] == NULL) {
        object->field_0x768 = 1.5f;
    } else {
        object->field_0x768 = AnimSpeed(object->apiobj.character_model, object->context_animation);
    }

    object->force_target = NULL;
    object->blowup_target = NULL;
    if (lungeTarget.Get() != NULL) {
        object->force_target = lungeTarget->GetCharacterObject();
        object->blowup_target = lungeTarget->GetGizBlowup();
    } else {
        object->force_target = ObjOpponent(object, 2.0f, 1.0f, 1, 2, 1);
        if (object->force_target == NULL) {
            if (object->apiobj.player_controlled) {
                object->blowup_target = GizmoBlowUpOpponent(object, 2.0f, 1.0f, 0.0f, 4, 0, 0, 0);
            }
            if (object->blowup_target == NULL) {
                object->force_target = ObjOpponent(object, 2.0f, 1.0f, 1, 2, 2);
            }
        }
        SelectOpponent(object, 2.0f, 1.0f, 2, 0);
    }
    lungeTarget = NuMechPtr<MechObjectInterface, 4>();

    if ((object->apiobj.character_data->model_flags & 8) != 0)
        SetWeaponOut(object);
    if (object->apiobj.player_controlled) {
        Hint_SetComplete(0x608);
    }
}

void StartJetPackFall(GameObject_s *object, i32 fall) {
    object->context_animation_timer = 0.0f;
    const u8 context_variant_flags = object->context_variant_flags;
    object->jump_flags &= ~PLAYER_JUMP_FLAG_SPECIAL_TAKEOFF;
    object->field_0xe31 = 0;
    object->character_context = 0;
    object->context_variant_flags = context_variant_flags & ~PLAYER_JUMP_VARIANT_END_CLEAR;
    object->action_movement_state = 0;
    object->context_animation = 6;
    object->jump_sequence = 2;
    object->airborne_collision_target = NULL;

    if (fall == 0 && object->apiobj.character_model->model_data_b[0x49] != NULL) {
        object->context_variant_flags = context_variant_flags & 0x2f;
        object->apiobj.velocity.y =
            static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->jump_speed;
        object->context_animation = 0x49;
        PlayJumpSfx(object, 0);
    } else {
        object->character_context = -1;
    }

    if (static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->uses_weapon_action == 2) {
        PlayJumpSfx(object, 1);
    }
}

void JetPackCode(GameObject_s *object, i32 jump_pressed, i32 fall, i32) {
    i8 context = object->character_context;
    if (context == 2 || context == 0x19 ||
        (object->apiobj.character_data->game_character->uses_weapon_action == 2 && context == 1)) {
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer <= 0.0f)
            object->character_context = -1;
        return;
    }
    if (context == 0) {
        if (object->apiobj.field_0x27d == 0)
            return;
        object->context_animation_timer = 0.0f;
        if (object->context_animation == 0x49 && object->apiobj.character_model->model_data_b[0x4a] != NULL) {
            object->context_animation = 0x4a;
            object->context_animation_timer = AnimDuration(object->id, 0x4a, 0.0f, 0.0f, 1);
            if (object->context_animation_timer > 0.0f)
                object->character_context = 0x19;
        } else {
            object->context_animation = 0x0a;
            object->context_animation_timer = AnimDuration(object->id, 0x0a, 0.0f, 0.0f, 1);
            if (object->context_animation_timer > 0.0f)
                object->character_context = 2;
        }
        if (object->context_animation_timer > 0.0f)
            ResetAnimPacket(&object->apiobj.anim_packet, -1);
        else
            object->character_context = -1;
        PlayLandSfx(object, 0, 0);
        return;
    }
    if (object->field_0xe31 == 1) {
        if ((jump_pressed | fall) != 0 && context == -1)
            StartJetPackFall(object, fall);
        return;
    }
    if (context == 0x13) {
        if (AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0) == NULL)
            return;
        object->context_animation_timer -= FRAMETIME;
        if (!(object->context_animation_timer <= 0.0f))
            return;
        JetPackLaunch(object);
        object->character_context = -1;
        return;
    }
    if (jump_pressed == 0 || (object->apiobj.field_0x27d == 0 && !(object->ground_contact_grace_timer > 0.0f)) ||
        ObjLandReady(object) == 0)
        return;
    if (object->character_context == 6)
        FastWeaponIn(object, 0);
    else if (object->character_context == 7)
        FastWeaponOut(object, 0);
    object->context_animation = 0x48;
    object->context_animation_timer = AnimDuration(object->id, 0x48, 0.0f, 0.0f, 1);
    if (!(object->context_animation_timer <= 0.0f))
        object->character_context = 0x13;
    else
        JetPackLaunch(object);
    object->fall_hover_height = 2000000.0f;
}

void ComboRotateCode(GameObject_s *object, i32 action_held) {
    if (object->character_context != 9) {
        return;
    }
    object->context_animation_timer += FRAMETIME;
    i32 rotation;
    if (object->context_animation_timer != object->context_animation_timer) {
        rotation = static_cast<i32>(object->context_animation_timer / object->airborne_action_duration * 32768.0f);
    } else {
        if (action_held == 0) {
            object->character_context = -1;
        } else {
            StartHold(object);
        }
        rotation = -0x8000;
    }
    u16 angle =
        object->pad_79f[0] == 1 ? object->carried_object_angle + rotation : object->carried_object_angle - rotation;
    object->apiobj.field_0x276 = angle;
    object->apiobj.movement_facing_angle = angle;
    object->apiobj.facing_angle = angle;
}

i32 Dodge_UpdateHint(HINT_s *hint) {
    if (WORLD->area != NULL && WORLD->area == HUB_ADATA)
        return 0;
#define CHECK_DODGE_HINT_PLAYER(index)                                                                                 \
    do {                                                                                                               \
        GameObject *object = Player[index];                                                                            \
        if (object == NULL) {                                                                                          \
            break;                                                                                                     \
        }                                                                                                              \
        const u32 flags = object->apiobj.character_data->model_flags;                                                  \
        if ((flags & 0x2000) != 0) {                                                                                   \
            return 0;                                                                                                  \
        }                                                                                                              \
        if (object->apiobj.player_controlled && object->incoming_bolt != NULL) {                                       \
            if (hint->control_mode_ids[0] == 0x265) {                                                                  \
                if ((flags & 8) == 0 && (object->apiobj.character_model->model_data_b[0x4f] != NULL ||                 \
                                         object->apiobj.character_model->model_data_b[0x26] != NULL)) {                \
                    return 1;                                                                                          \
                }                                                                                                      \
            } else if (hint->control_mode_ids[0] == 0x5dd && (flags & 8) != 0) {                                       \
                return 1;                                                                                              \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)
    CHECK_DODGE_HINT_PLAYER(0);
    CHECK_DODGE_HINT_PLAYER(1);
    CHECK_DODGE_HINT_PLAYER(2);
    CHECK_DODGE_HINT_PLAYER(3);
    CHECK_DODGE_HINT_PLAYER(4);
    CHECK_DODGE_HINT_PLAYER(5);
    CHECK_DODGE_HINT_PLAYER(6);
    CHECK_DODGE_HINT_PLAYER(7);
#undef CHECK_DODGE_HINT_PLAYER
    return 0;
}

void MakeThrowVector(NUVEC *result, NUVEC *origin, NUVEC *target, NUVEC *target_velocity, f32 speed, f32 gravity) {
    NUVEC predicted = *target;
    NUVEC delta;
    NUVEC horizontal_origin;
    horizontal_origin.x = origin->x;
    horizontal_origin.y = 0.0f;
    horizontal_origin.z = origin->z;
    NuVecSub(&delta, &predicted, &horizontal_origin);
    NuVecAddScale(&predicted, target, target_velocity, NuVecMag(&delta) / speed);
    horizontal_origin.x = origin->x;
    horizontal_origin.y = 0.0f;
    horizontal_origin.z = origin->z;
    NuVecSub(&delta, &predicted, &horizontal_origin);
    NuVecAddScale(&predicted, target, target_velocity, NuVecMag(&delta) / speed);
    horizontal_origin.x = origin->x;
    horizontal_origin.y = 0.0f;
    horizontal_origin.z = origin->z;
    NuVecSub(&delta, &predicted, &horizontal_origin);
    NuVecAddScale(&predicted, target, target_velocity, NuVecMag(&delta) / speed);
    const f32 time = NuVecXZDist(&predicted, origin, NULL) / speed;
    const f32 vertical_speed = (predicted.y - origin->y) / (FRAMETIME + time) - (gravity * 0.5f) * time;
    const u16 angle = NuAtan2D(predicted.x - origin->x, predicted.z - origin->z);
    result->x = speed * NU_SIN_LUT(angle);
    result->y = vertical_speed;
    result->z = speed * NU_COS_LUT(angle);
}

i32 LightSabre_ColourFromObj(i32 model, i32 *glow_model) {
    i32 colour = -1;
    i32 glow = -1;
    if (model == 0x65) {
        colour = 0;
        glow = 0x66;
    } else if (model == 0x67) {
        colour = 1;
        glow = 0x68;
    } else if (model == 0x69) {
        colour = 2;
        glow = 0x6a;
    } else if (model == 0x6b) {
        colour = 3;
        glow = 0x6c;
    }
    if (glow_model != NULL)
        *glow_model = glow;
    return colour;
}

void LightSabreDebris(GameObject_s *object) {
    i32 hit_effect = -1;
    if (object->blade_index != -1)
        hit_effect = BladeTab[object->blade_index].hit_effect;
    i32 blade_count = 4;
    if (object->id != id_GRIEVOUS)
        blade_count = object->id == id_DARTHMAUL ? 2 : 1;
    auto object_id = object->id;
    auto object_field_0xe23 = object->field_0xe23;
    for (i32 blade = 0; blade < blade_count; ++blade) {
        i32 effect = hit_effect;
        if (object_id == id_GRIEVOUS && !(object->apiobj.field_0x27c != -1 && Cheat_IsOn(0x19)) &&
            !(object->apiobj.field_0x27c != -1 && Player_HasPurpleForce(object))) {
            if (blade == 3 || blade == 0) {
                effect = 3;
            } else {
                effect = 2;
            }
        }
        if (effect == -1 || object->apiobj.field_0x288 == 0 || (object_field_0xe23 & 8) == 0)
            continue;
        GAMECHARACTERDATA *data = object->apiobj.character_data->game_character;
        i32 first = data->streak_joints[blade][0];
        if (first == -1 || object->apiobj.character_model->points_of_interest[first] == NULL)
            continue;
        i32 second = data->streak_joints[blade][1];
        if (second == -1 || object->apiobj.character_model->points_of_interest[second] == NULL)
            continue;
        NUVEC start;
        NUVEC middle;
        NUVEC end;
        start = *NUMTX_GET_ROW_VEC(&object->joint_matrices[first], 3);
        end = *NUMTX_GET_ROW_VEC(&object->joint_matrices[second], 3);
        NuVecAdd(&middle, &start, &end);
        NuVecScale(&middle, &middle, 0.5f);
        AddGameDebris(WORLD->debris_sys, effect, &start);
        AddGameDebris(WORLD->debris_sys, effect, &middle);
        AddGameDebris(WORLD->debris_sys, effect, &end);
    }
}

void EndForce(GameObject_s *object, i32) {
    if (object != NULL && (object->apiobj.field_0x1f4 & 0x40000) == 0) {
        object->force_glow_object = NULL;
        object->field_0xe22 &= ~2;
        object->force_glow_candidate = NULL;
        object->character_context = -1;
    }
    GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
}

static void ForceCode(GameObject_s *object, i32 pressed, i32 held) {
    u8 previous_force = object->force_repeat_requested;
    object->force_repeat_requested = 0;
    if (object->character_context == 8) {
        if ((held || (object->pad_gamepad->allocated_5a & 4) != 0) && object->gizforce_target != NULL) {
            if ((dagobah_training == 0 || object->field_0xcc0 != NULL || FreePlay != 0) &&
                !GizForce_Complete(object->gizforce_target) &&
                !GizForce_StoodOnForce(object->gizforce_target, object)) {
                AlertSurroundingCreatures(object, &object->apiobj.collision_position);
                if ((WORLD->current_level == VADERC_LDATA || !object->apiobj.player_controlled ||
                     object->apiobj.model_draw_result == 0 || !(object->field_0xc54 > 0.0f) ||
                     !(object->head_target_facing < -0.85f || object->head_target_facing > 0.85f)) &&
                    !(WORLD->current_level == DAGOBAHC_LDATA && object->id == id_LUKESKYWALKERDAGOBAH &&
                      LevGizmo[0] != NULL && object->gizforce_target == LevGizmo[0]->object &&
                      GameAnimSet_GetCompletionRatio(object->gizforce_target->anim_set) > 0.5f)) {
                    GIZFORCE_s *force = object->gizforce_target;
                    force->using_object = object;
                    object->force_heading = GizForces_AngleToForce(&object->apiobj.position, force);
                    AlertSurroundingCreatures(object, &object->apiobj.collision_position);
                    object->field_0xe22 |= 2;
                    force = object->gizforce_target;
                    if (object->gizforce_target_object == NULL) {
                        object->force_glow_candidate = force;
                        object->force_glow_candidate_kind = 0;
                        if (force != NULL)
                            SetHeadTarget(object, &force->file_position, 1, 2.0f, 5.0f, 10.0f);
                    } else {
                        object->force_glow_candidate = object->gizforce_target_object;
                        object->force_glow_candidate_kind = 1;
                        object->field_0xd8c = force == NULL ? 1.0f : force->horizontal_range;
                        SetHeadTarget(object, NuSpecialGetDrawPos(&object->gizforce_target_object->special), 1, 2.0f,
                                      5.0f, 10.0f);
                    }
                    if (force != NULL)
                        SetHeadTarget(object, &force->file_position, 1, 2.0f, 5.0f, 10.0f);
                } else {
                    object->character_context = -1;
                    object->gizforce_target = NULL;
                }
            } else {
                AlertSurroundingCreatures(object, &object->apiobj.collision_position);
                object->character_context = -1;
                object->gizforce_target = NULL;
            }
        } else {
            object->character_context = -1;
        }
        if (object->character_context == -1)
            GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
        return;
    }
    if ((static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_094[1] & 0x80) != 0 ||
        (object->field_0xe23 & 1) != 0 || object->character_context == 0x12)
        return;
    bool start = false;
    if (object->apiobj.field_0x27d != 0 &&
        (object->character_context == -1 || (CInfo[object->character_context].flags & 4) != 0)) {
        if (((object->apiobj.flags_low & 0x80) != 0 && pressed) || (object->pad_gamepad->allocated_5a & 4) != 0 ||
            (held && previous_force))
            start = true;
    }
    if (start) {
        if ((object->pad_gamepad->allocated_5a & 4) == 0 || object->gizforce_target == NULL) {
            GizForce_FindBestForceTarget(WORLD->giz_force_sys, object);
        }
        if (object->gizforce_target != NULL) {
            object->context_animation_timer = 0.5f;
            object->character_context = 8;
            NewRumble(object->pad_gamepad->pad, 0.6f, 0);
            object->gizforce_target->using_object = object;
            GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
            object->force_heading = GizForces_AngleToForce(&object->apiobj.position, object->gizforce_target);
            if ((object->apiobj.flags_low & 0x80) != 0) {
                Hint_SetComplete(0x259);
                if ((object->gizforce_target->config_flags & 0x10) != 0)
                    Hint_SetComplete(0x623);
                LSW_HintConditions.force_used = 1;
            }
        }
    } else if ((object->apiobj.flags_low & 0x80) != 0)
        GizForce_FindBestForceTarget(WORLD->giz_force_sys, object);
    GIZFORCE_s *force = object->gizforce_target;
    if (force != NULL && __builtin_expect(static_cast<signed char>(object->apiobj.flags_low) < 0, 1) &&
        __builtin_expect((object->field_0xe22 & 2) == 0, 1) && __builtin_expect(object->apiobj.field_0x27d != 0, 1)) {
        object->field_0xe22 |= 2;
        if (__builtin_expect(object->gizforce_target_object == NULL, 0)) {
            object->force_glow_candidate = force;
            object->force_glow_candidate_kind = 0;
            SetHeadTarget(object, &force->file_position, 1, 2.0f, 5.0f, 10.0f);
        } else {
            object->force_glow_candidate_kind = 1;
            object->force_glow_candidate = object->gizforce_target_object;
            object->field_0xd8c = force->horizontal_range;
            SetHeadTarget(object, NuSpecialGetDrawPos(&object->gizforce_target_object->special), 1, 2.0f, 5.0f, 10.0f);
        }
        SetHeadTarget(object, &force->file_position, 1, 2.0f, 5.0f, 10.0f);
    }
    if (object->character_context == 8 && object->id == id_LUKESKYWALKERDAGOBAH && dagobah_training != 0 &&
        object->field_0xcc0 == NULL && FreePlay == 0) {
        EndForce(object, 0);
        if (AnimPlaying(&object->apiobj.anim_packet, 0xb, 1, 1) == NULL &&
            AnimPlaying(&object->apiobj.anim_packet, 0x27, 1, 1) == NULL) {
            PlaySfx("JForcePush", &object->apiobj.collision_position);
            PlayGruntSfx(object);
        }
        NewBuzzFrames(object->pad_gamepad->pad, 2, 0);
        if (object->gizforce_target != NULL) {
            object->gizforce_target->using_object = NULL;
            object->apiobj.movement_facing_angle =
                GizForces_AngleToForce(&object->apiobj.collision_position, object->gizforce_target);
        }
        object->field_0xe23 |= 1;
        GameCam_HitRoll();
    }
}

void ForceLightning_Origin(GameObject_s *object, NUVEC *primary, NUVEC *secondary) {
    *primary = object->apiobj.collision_position;
    if (secondary != NULL)
        secondary->y = 1000000000.0f;
    if (object->apiobj.field_0x288 == 0)
        return;
    PLAYERCHARACTERCONFIG_s *config = object->apiobj.character_data->player_config;
    i32 joint = config->hand_joints[0];
    if (joint == -1 || object->apiobj.character_model->points_of_interest[joint] == NULL)
        return;
    *primary = *reinterpret_cast<NUVEC *>(&object->joint_matrices[joint].m30);
    if (secondary == NULL || (object->weapon_scale != 0.0f && object->weapon_scale_state != 2))
        return;
    joint = config->hand_joints[1];
    if (joint != -1 && object->apiobj.character_model->points_of_interest[joint] != NULL) {
        *secondary = *reinterpret_cast<NUVEC *>(&object->joint_matrices[joint].m30);
    }
}

static void DrawLightningBolts(GameObject_s *object, GameObject_s *target, i32 lightning_index) {
    NUVEC reflected_origin;
    NUVEC primary;
    NUVEC secondary;
    NUVEC target_position;
    NUVEC delta;
    ForceLightning_Origin(object, &primary, &secondary);

    if (primary.y != 1000000000.0f) {
        target_position.x = target->apiobj.collision_position.x;
        target_position.z = target->apiobj.collision_position.z;
        f32 height = target->apiobj.field_0x194 - target->apiobj.lower_position.y;
        target_position.y =
            target->apiobj.lower_position.y + height * 0.25f + height * (1.0f / 17.0f) * qrand() * 1.5259022e-05f;
        f32 distance = NuVecDist(&target_position, &primary, &delta);
        NuLgtLaser(lightning_type, lightning_sizew[lightning_index], lightning_sizel[lightning_index],
                   lightning_sizewab[lightning_index], &primary, &delta, lightning_col[lightning_index],
                   lightning_endw[lightning_index], distance);
        PlaySfx("ForceLightningLp", &target->apiobj.collision_position);

        if (object->field_0x1088 != 0 && object->field_0x1087 == 2 && NuIOS_IsLowEndDevice() == 0 &&
            lightning_type != 0) {
            reflected_origin = primary;
            NUVEC reflected_target = target_position;
            f32 plane = WORLD->current_level->unknown_0cc;
            if (plane == 2000000.0f)
                plane = object->field_0x1020;
            reflected_origin.y = plane - (reflected_origin.y - plane);
            reflected_target.y = plane - (reflected_target.y - plane);
            distance = NuVecDist(&reflected_target, &reflected_origin, &delta);
            NuLgtLaser(lightning_type, lightning_sizew[lightning_index], lightning_sizel[lightning_index],
                       lightning_sizewab[lightning_index], &reflected_origin, &delta, lightning_col[lightning_index],
                       lightning_endw[lightning_index], distance);
        }
    }

    if (secondary.y == 1000000000.0f) {
        return;
    }
    target_position.x = target->apiobj.collision_position.x;
    target_position.z = target->apiobj.collision_position.z;
    f32 height = target->apiobj.field_0x194 - target->apiobj.lower_position.y;
    target_position.y =
        target->apiobj.lower_position.y + height * 0.25f + height * (1.0f / 17.0f) * qrand() * 1.5259022e-05f;
    f32 distance = NuVecDist(&target_position, &secondary, &delta);
    NuLgtLaser(lightning_type, lightning_sizew[lightning_index], lightning_sizel[lightning_index],
               lightning_sizewab[lightning_index], &secondary, &delta, lightning_col[lightning_index],
               lightning_endw[lightning_index], distance);
    PlaySfx("ForceLightningLp", &target->apiobj.collision_position);

    if (object->field_0x1088 != 0 && object->field_0x1087 == 2 && NuIOS_IsLowEndDevice() == 0 && lightning_type != 0) {
        NUVEC reflected_origin = secondary;
        NUVEC reflected_target = target_position;
        f32 plane = WORLD->current_level->unknown_0cc;
        if (plane == 2000000.0f)
            plane = object->field_0x1020;
        reflected_origin.y = plane - (reflected_origin.y - plane);
        reflected_target.y = plane - (reflected_target.y - plane);
        distance = NuVecDist(&reflected_target, &reflected_origin, &delta);
        NuLgtLaser(lightning_type, lightning_sizew[lightning_index], lightning_sizel[lightning_index],
                   lightning_sizewab[lightning_index], &reflected_origin, &delta, lightning_col[lightning_index],
                   lightning_endw[lightning_index], distance);
    }
}

void ReleaseForce(GameObject_s *object, i32 mode) {
    switch (object->character_context) {
        case 8:
            if (object->apiobj.player_controlled)
                GameCam_Blend(GameCam, 1.0f, 0.0f, 1);
            break;
        case 0x1d:
            if (object->force_part != NULL) {
                object->force_part->force_player_mask &= ~(1 << static_cast<i8>(object->apiobj.field_0x27c));
                if (object->force_part->force_player_mask == 0)
                    object->force_part->force_player_mask = -1;
                object->force_part->gravity = ForceThrowGravity;
                object->force_part = NULL;
            }
            object->character_context = -1;
            break;
        case 0x1b:
            if (object->force_target != NULL) {
                if (object->force_target->character_context == 0x1c) {
                    object->force_target->character_context = -1;
                    object->force_target->force_target = NULL;
                }
                object->force_target = NULL;
            }
            break;
        default:
            return;
    }
    EndForce(object, mode);
}

static void ForceDeflectCode(GameObject_s *object, i32 pressed, i32 held) {
    i32 waiting = objInNetWaitContext(object, 0x1d);
    signed char context = object->character_context;
    if (__builtin_expect(context != 0x1d, 1)) {
        PART_s *assist = NULL;
        if ((object->apiobj.flags_low & 0x80) == 0) {
            if (__builtin_expect((object->field_0xefd & 1) == 0 || player == NULL || player->force_part == NULL, 1))
                return;
            assist = player->force_part;
        }
        if (object->apiobj.field_0x27d == 0)
            return;
        if (__builtin_expect(context != -1 && __builtin_expect((CInfo[context].flags & 4) == 0, 1) && context != 6 &&
                                 object->character_context != 7 && !objInNetWaitContext(object, 0x1d),
                             0))
            return;
        PART_s *part;
        if (__builtin_expect(waiting, 0))
            part = object->force_part;
        else if (__builtin_expect(assist != NULL, 1))
            part = assist;
        else {
            f32 range = TouchHacks::GetIncomingPartRange();
            f32 radius = object->apiobj.field_0x1e0;
            if (radius <= object->apiobj.field_0x1dc)
                radius = object->apiobj.field_0x1dc;
            part = FindIncomingPart(object, &object->apiobj.collision_position, radius, 0x800a, range);
        }
        if (part == NULL)
            return;
        object->field_0xe22 |= 2;
        object->force_glow_candidate = part;
        object->force_glow_candidate_kind = 3;
        SetHeadTarget(object, &part->position, 2, 1.0f, 0.0f, 0.0f);
        if (assist == NULL && pressed == 0 && !waiting)
            return;
        object->force_part = part;
        part->flags |= 0x10000;
        part->gravity = 0.0f;
        object->context_animation_timer = 0.0f;
        object->airborne_action_duration = 0.3f;
        object->force_target = NULL;
        object->blowup_target = NULL;
        u8 mask = 1 << object->apiobj.field_0x27c;
        object->character_context = 0x1d;
        if (part->force_player_mask != -1)
            mask |= part->force_player_mask;
        part->force_player_mask = mask;
        GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
        PlaySfx("JForcePush", &object->apiobj.collision_position);
        return;
    }
    object->field_0xe22 |= 2;
    object->context_animation_timer += FRAMETIME;
    if (__builtin_expect((object->apiobj.flags_low & 0x80) != 0, 1))
        NewRumble(object->pad_gamepad->pad, qrand() * 1.5259022e-05f * 0.6f, 0);
    if (object->force_part != NULL) {
        PART_s *part = object->force_part;
        part->force_flags |= 1;
        part->flags &= ~0x4000;
        if (__builtin_expect(object->context_animation_timer < 0.2f, 0))
            part->velocity.y = SeekValF(part->velocity.y, 2.0f, 6.0f);
        part = object->force_part;
        NUVEC direction;
        if (part->owner == NULL || part->owner->apiobj.field_0x27c != -1)
            NuVecRotateY(&direction, &v001, object->apiobj.facing_angle);
        else {
            NuVecSub(&direction, &part->owner->apiobj.collision_position, &part->position);
            NuVecNorm(&direction, &direction);
        }
        NUVEC velocity;
        NuVecScale(&velocity, &direction, 4.0f);
        SeekVec(&object->force_part->velocity, &object->force_part->velocity, &velocity, 3.0f);
        object->force_glow_candidate = object->force_part;
        object->force_glow_candidate_kind = 3;
        SetHeadTarget(object, &object->force_part->position, 2, 1.0f, 0.0f, 0.0f);
        part = object->force_part;
        if (__builtin_expect(part->move_callback == BobaRocket_Move, 0)) {
            part->move_callback = NULL;
            part->update_callback = NULL;
            part->flags = (part->flags & ~0x4000) | 0x80;
            NewPartRotation(part);
        } else if (part->move_callback == Boulder_Move && object->context_animation_timer >= 0.5f)
            Boulder_Kill(part, 0);
    }
    if ((held != 0 || (object->field_0xefd & 1) != 0) && object->force_part != NULL &&
        object->airborne_action_duration >= 0.2f && object->context_animation_timer < 1.5f) {
        object->airborne_action_duration = object->airborne_action_duration - FRAMETIME > 0.2f
                                               ? object->airborne_action_duration - FRAMETIME
                                               : 0.2f;
        return;
    }
    object->airborne_action_duration -= FRAMETIME;
    if (object->airborne_action_duration <= 0.0f)
        ReleaseForce(object, 0);
}

static void ForcePushCodeBody(GameObject_s *object, i32 held) {
    AlertSurroundingCreatures(object, &object->apiobj.collision_position);
    GameObject_s *initial_target = object->force_target;
    if ((object->pad_gamepad->allocated_5a & 0x10) != 0)
        held = 1;
    object->field_0xe22 |= 2;
    GameObject_s *target = initial_target;
    if (target != NULL) {
        if (target->character_context == 0x0f) {
            object->force_target = NULL;
            EndForce(object, 0);
            target = object->force_target;
        } else if ((target->apiobj.field_0x1f8 & 0x1001) != 0x1001 || target->apiobj.field_0x287 != 0) {
            object->force_target = NULL;
            target = NULL;
        } else if ((object->field_0xe21 & 6) == 0 && target->character_context != 0x1c) {
            object->force_target = NULL;
            EndForce(object, 0);
            target = object->force_target;
        }
    }
    if (target != NULL) {
        SetObjAsHeadTarget(object, target, 2, 1.0f, 0.0f, 0.0f);
        SetObjAsHeadTarget(target, object, 2, 1.0f, 0.0f, 0.0f);
        if (__builtin_expect((object->field_0xe21 & 4) != 0, 0)) {
            object->force_glow_candidate = target;
            object->force_glow_candidate_kind = 2;
            if ((object->field_0xe21 & 1) != 0)
                DrawLightningBolts(object, target, 0);
        } else {
            NewRumble(target->pad_gamepad->pad, qrand() * 1.5259022e-05f * 0.4f, 0);
            if ((object->field_0xe21 & 1) != 0)
                DrawLightningBolts(object, target, 1);
            object->force_glow_candidate = target;
            object->force_glow_candidate_kind = 2;
            if (target->character_context != 0x1c || (target->apiobj.character_data->model_flags & 0x20) != 0 ||
                target->apiobj.character_model->model_data_b[target->context_animation] == NULL ||
                CurrentAnim(&target->apiobj.anim_packet) == target->context_animation)
                object->context_animation_timer += FRAMETIME;
            NewRumble(object->pad_gamepad->pad, qrand() * 1.5259022e-05f * 0.4f, 0);
            f32 duration = (object->field_0xe21 & 2) != 0                                          ? 0.4f
                           : ((object->field_0xe20 & 0x80) != 0 || (object->field_0xe21 & 1) != 0) ? 1.2f
                                                                                                   : 0.8f;
            object->field_0x7a3 = 0;
            bool throw_target = false;
            if ((object->apiobj.flags_low & 0x80) != 0 && (object->apiobj.field_0x1f4 & 0x40000) == 0 &&
                ForcePush_Waft && (target->apiobj.flags_low & 0x80) == 0 && object->context_animation_timer >= 0.2f &&
                object->pad_gamepad->input_magnitude > 0.0f &&
                ((object->field_0xe21 & 1) == 0 || (GetGameCharacterData(target)->flags_090 & 0x4000) == 0) &&
                ((object->field_0xe20 & 0x80) != 0 || (object->field_0xe21 & 1) != 0)) {
                object->field_0x7a3 = 1;
                object->external_force.x = NU_SIN_LUT(object->apiobj.facing_angle) * 0.5f + object->apiobj.position.x;
                object->external_force.y = (object->apiobj.collision_max.y - object->apiobj.collision_min.y) * 0.75f +
                                           object->apiobj.collision_min.y;
                object->external_force.z = NU_COS_LUT(object->apiobj.facing_angle) * 0.5f + object->apiobj.position.z;
                u16 angle = NuAtan2D(target->apiobj.position.x - object->apiobj.position.x,
                                     target->apiobj.position.z - object->apiobj.position.z);
                i32 difference = RotDiff(GamePad_InputAngle(object, object->pad_gamepad), angle);
                if ((difference < 0 ? -difference : difference) < 0xaaa)
                    throw_target = true;
                else if (duration - 0.3f <= object->context_animation_timer)
                    object->context_animation_timer = duration - 0.3f;
            }
            if (object->context_animation_timer >= duration ||
                (((object->field_0xe20 & 0x80) == 0 && (object->field_0xe21 & 3) == 0) &&
                 object->context_animation_timer >= 0.2f && initial_target->field_0x1084 != 0 &&
                 NuFabs(initial_target->contact_normal.y) < NU_SIN_LUT(0x6aaa))) {
                if (throw_target) {
                    object->force_target = NULL;
                    object->character_context = -1;
                    object->pad_gamepad->allocated_5a &= ~0x10;
                    target->force_target = NULL;
                    object->field_0xd14 = 0;
                    EndForce(target, 0);
                    PlaySfx("JForcePush", &object->apiobj.collision_position);
                    KillRumble(object);
                    PlayHurtSfx(target);
                    GameCam_NewShake(NULL, 0.75f, 0.75f, 1.0f);
                    target->character_context = 0x5f;
                    if (target->apiobj.character_model->model_data_b[0xb8] != NULL) {
                        target->context_animation = 0xb8;
                        ResetAnimPacket(&target->apiobj.anim_packet, 0xffff);
                    } else if (target->apiobj.character_model->model_data_b[5] != NULL)
                        target->context_animation = 5;
                    target->current_hp = 1;
                    target->context_animation_timer = 0.0f;
                    target->airborne_action_duration = 2.0f;
                    u16 angle = NuAtan2D(target->apiobj.pos_x - object->apiobj.pos_x,
                                         target->apiobj.pos_z - object->apiobj.pos_z);
                    f32 speed = (qrand() * 1.5259022e-05f * 0.4f + 0.8f) * DIEAIRSPEED;
                    target->external_force.x = NU_SIN_LUT(angle) * speed;
                    target->external_force.z = NU_COS_LUT(angle) * speed;
                    target->context_variant_flags &= ~1;
                    target->apiobj.velocity.y = (qrand() * 1.5259022e-05f * 0.4f + 0.8f) * DIEAIRJUMPSPEED;
                    target->hit_variant =
                        static_cast<u8>(object->apiobj.field_0x27c) <= 1 ? object->apiobj.field_0x27c : -1;
                    SetFlicker(target, 0.4f);
                } else {
                    bool deactivate = true;
                    if ((object->field_0xe21 & 2) != 0) {
                        if (DeactivatePlayer(target, 5.0f, NULL)) {
                            target->action_movement_state = 1;
                            target->field_0x768 = qrand() * 1.5259022e-05f * 0.3f + 0.2f;
                            if (target->id == id_JAWA)
                                PlaySfx("Jawa_Dizzy", &target->apiobj.collision_position);
                        }
                    } else if (target->id == id_GONKDROID ||
                               ((target->apiobj.character_data->model_flags & 0x10) != 0 && target->hitpoints >= 2))
                        DeactivatePlayer(target, DEACTIVATEDTIME, NULL);
                    else {
                        bool immune = FreePlay != 0 && (object->field_0xe21 & 1) != 0 &&
                                      (GetGameCharacterData(target)->flags_090 & 0x4000) != 0;
                        i32 damage = immune                             ? 0
                                     : target->apiobj.field_0x27c == -1 ? static_cast<i8>(target->current_hp)
                                                                        : 1;
                        u16 flags =
                            WORLD->current_level == BLOCKADERUNNERB_LDATA ? (object->apiobj.field_0x1f4 >> 10) & 1 : 0;
                        if (ObjHitObj(object, target, damage, flags, 0, 1) != 2 && (object->field_0xe20 & 0x80) != 0 &&
                            !immune)
                            target->fall_animation_timer = 0.2f;
                        if (object->force_target != NULL) {
                            object->force_target->force_target = NULL;
                            EndForce(object->force_target, 0);
                        }
                        object->force_target = NULL;
                        object->pad_gamepad->allocated_5a &= ~0x10;
                        object->field_0xd14 = 0;
                        deactivate = false;
                    }
                    if (deactivate) {
                        NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
                        GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
                    }
                }
                object->airborne_action_duration = 0.0f;
                GameCam_HitJudder();
            }
        }
    }
    if (object->airborne_action_duration >= 0.2f && held != 0 && object->force_target != NULL) {
        object->airborne_action_duration =
            object->airborne_action_duration - FRAMETIME > 0.2f ? object->airborne_action_duration - FRAMETIME : 0.2f;
    } else if ((object->apiobj.field_0x1f4 & 0x40000) == 0) {
        object->airborne_action_duration -= FRAMETIME;
        if (object->airborne_action_duration <= 0.0f) {
            target = object->force_target;
            if (object->context_animation_timer >= 0.4f) {
                if (target == NULL) {
                    ReleaseForce(object, 0);
                    return;
                }
                if ((target->apiobj.field_0x1f8 & 1) != 0 && target->apiobj.field_0x287 == 0 &&
                    (GetGameCharacterData(target)->uses_weapon_action == 4 || target->id == id_BATTLEDROIDSECURITY ||
                     target->id == id_GONKDROID || target->id == id_PKDROID || target->id == id_PITDROID)) {
                    DeactivatePlayer(target, DEACTIVATEDTIME * 0.5f, NULL);
                    NewRumble(object->pad_gamepad->pad, 0.5f, 0);
                }
                ReleaseForce(object, 0);
                if ((object->field_0xe20 & 0x80) == 0)
                    return;
            } else {
                ReleaseForce(object, 0);
                if ((object->field_0xe20 & 0x80) == 0 || target == NULL)
                    return;
            }
            target->fall_animation_timer = 0.2f;
        }
    }
}

void FindForcePushTarget(GameObject_s *object, i32 activate, i32 target_filter) {
    NUVEC delta;
    if (object->character_context == 0x1b || (object->field_0xe22 & 2) != 0 ||
        (object->apiobj.field_0x27c != -1 && MiniCutCam != 0) || (object->field_0xe23 & 1) != 0) {
        return;
    }

    if ((object->pad_gamepad->allocated_5a & 0x10) != 0) {
        activate = 1;
    } else if (!object->apiobj.player_controlled) {
        return;
    }
    if (object->apiobj.field_0x27d == 0) {
        return;
    }
    if (object->character_context != -1 && !objInNetWaitContext(object, 0x1b) &&
        (CInfo[object->character_context].flags & 4) == 0 && object->character_context != 6 &&
        object->character_context != 7) {
        return;
    }

    i32 choke_style = object->apiobj.character_data->game_character->flags_090 & 2;
    i32 second_style = object->apiobj.character_data->game_character->flags_090 & 4;
    GameObject_s *best = NULL;
    i32 selected_choke;
    i32 selected_second;
    i32 selected_direct;
    if ((object->pad_gamepad->allocated_5a & 0x10) != 0 && object->force_push_target != NULL) {
        best = object->force_push_target;
        if (choke_style == 0 && object->apiobj.character_data->game_character->uses_weapon_action == 0x0c &&
            (object->apiobj.character_data->model_flags & 8) != 0) {
            choke_style = best->id == id_GAMORREANGUARD;
        }
        i32 target_special = best->apiobj.character_data->model_flags & 0x10;
        i32 continuing_direct = 0;
        if (target_special == 0 && (best->apiobj.character_data->game_character->flags_090 & 0x40) != 0 &&
            best->apiobj.character_model->model_data_b[0x41] != NULL) {
            continuing_direct = 1;
        } else if ((choke_style | second_style) != 0) {
            if (second_style != 0) {
                if (best->apiobj.character_model->model_data_b[0x54] == NULL) {
                    if (target_special == 0)
                        return;
                    choke_style = 0;
                    second_style = 0;
                }
            } else if (best->apiobj.character_model->model_data_b[0x53] == NULL) {
                if (target_special == 0)
                    return;
                choke_style = 0;
            }
        } else if (target_special == 0) {
            if (best->apiobj.character_model->model_data_b[0x41] != NULL) {
                continuing_direct = 1;
            } else if (best->apiobj.character_model->model_data_b[0x2b] == NULL) {
                return;
            }
        } else if (best->apiobj.character_model->model_data_b[0x2b] == NULL &&
                   best->apiobj.character_model->model_data_b[5] == NULL) {
            return;
        }
        selected_choke = choke_style;
        selected_second = second_style;
        selected_direct = continuing_direct;
    } else {
        i32 super_weirdo = SuperWeirdo(object);
        if (super_weirdo != 0) {
            i32 random = qrand();
            second_style = random > 0x7fff;
            choke_style = random <= 0x7fff;
        }

        i32 object_count = HIGHGAMEOBJECT;
        f32 best_distance = 1.5625f;
        GameObject_s *candidate = Obj;
        selected_choke = 0;
        selected_second = 0;
        selected_direct = 0;
        for (i32 index = 0; index < object_count; ++index, ++candidate) {
            if (candidate->apiobj.in_use == 0 || candidate == object || candidate->apiobj.character == 0 ||
                candidate->apiobj.field_0x287 != 0 || candidate->apiobj.model_draw_result == 0 ||
                candidate->character_context == 0x3c || candidate->character_context == 0x39 ||
                candidate->character_context == 0x3b || candidate->character_context == 0x41 ||
                candidate->character_context == 0x17 || candidate->character_context == 0x0f ||
                candidate->character_context == 0x47 || candidate->character_context == 0x46 ||
                (candidate->apiobj.character_data->game_character->flags_090 & 0x8000) != 0 ||
                (CInfo[candidate->character_context].flags & 0x8000) != 0 ||
                (candidate->apiobj.character_data->game_character->flags_094[1] & 2) != 0 ||
                (candidate->field_0xefc_word & 0x400010) != 0) {
                continue;
            }
            if (TouchHacks::CanForceTargetObj(*object, *candidate) &&
                !(WORLD->area != NULL && WORLD->area == EMPERORFIGHT_ADATA &&
                  ((candidate->field_0xefb & 8) != 0 || candidate->apiobj.field_0x27c != -1)) &&
                candidate->id != id_BODYGUARD) {
                if (target_filter == 1) {
                    if (candidate->apiobj.field_0x27c != -1) {
                        goto reload_object_count;
                    }
                    if ((candidate->apiobj.field_0x1f4 & 5) != 0) {
                        const u32 *mask = WORLD->api_object_sys->hostility_masks[object->apiobj.field_0x289];
                        const u64 hostility = static_cast<u64>(mask[0]) | (static_cast<u64>(mask[1]) << 32);
                        if (((hostility >> (candidate->apiobj.field_0x289 & 63)) & 1) == 0) {
                            goto reload_object_count;
                        }
                    }
                } else if (target_filter == 2 && candidate->apiobj.field_0x27c == -1) {
                    goto reload_object_count;
                }
                if ((candidate->field_0xefb & 8) != 0) {
                    goto reload_object_count;
                }

                i32 candidate_choke = choke_style;
                i32 candidate_second = second_style;
                i32 candidate_direct = 0;
                if (candidate_choke == 0 && object->apiobj.character_data->game_character->uses_weapon_action == 0x0c &&
                    (object->apiobj.character_data->model_flags & 8) != 0) {
                    candidate_choke = candidate->id == id_GAMORREANGUARD;
                }
                if ((candidate->apiobj.character_data->model_flags & 0x10) == 0 &&
                    (candidate->apiobj.character_data->game_character->flags_090 & 0x40) != 0 &&
                    candidate->apiobj.character_model->model_data_b[0x41] != NULL) {
                    candidate_direct = 1;
                } else if ((candidate_choke | second_style) != 0) {
                    if (super_weirdo != 0 && qrand() <= 0x7fff &&
                        (candidate->apiobj.character_data->model_flags & 0x10) == 0 &&
                        candidate->apiobj.character_model->model_data_b[0x41] != NULL) {
                        candidate_direct = 1;
                    } else if (second_style != 0) {
                        if (candidate->apiobj.character_model->model_data_b[0x54] == NULL) {
                            if ((candidate->apiobj.character_data->model_flags & 0x10) == 0)
                                goto reload_object_count;
                            candidate_choke = 0;
                            candidate_second = 0;
                        }
                    } else if (candidate->apiobj.character_model->model_data_b[0x53] == NULL) {
                        if ((candidate->apiobj.character_data->model_flags & 0x10) == 0)
                            goto reload_object_count;
                        candidate_choke = 0;
                    }
                } else if ((candidate->apiobj.character_data->model_flags & 0x10) == 0) {
                    if (candidate->apiobj.character_model->model_data_b[0x41] != NULL) {
                        candidate_direct = 1;
                    } else if (candidate->apiobj.character_model->model_data_b[0x2b] == NULL) {
                        goto reload_object_count;
                    }
                } else if (candidate->apiobj.character_model->model_data_b[0x2b] == NULL &&
                           candidate->apiobj.character_model->model_data_b[5] == NULL) {
                    goto reload_object_count;
                }

                f32 distance =
                    NuVecDistSqr(&object->apiobj.collision_position, &candidate->apiobj.collision_position, &delta);
                if (candidate->id == id_ATST) {
                    distance *= 1.0f / 3.0f;
                }
                if (!(distance < best_distance)) {
                    goto reload_object_count;
                }
                object_count = HIGHGAMEOBJECT;
                if (delta.x * object->facing_direction.x + delta.z * object->facing_direction.z < 0.0f) {
                    best = candidate;
                    best_distance = distance;
                    selected_choke = candidate_choke;
                    selected_second = candidate_second;
                    selected_direct = candidate_direct;
                }
                continue;
            }
        reload_object_count:
            object_count = HIGHGAMEOBJECT;
        }
    }
    if (best != NULL && best->character_context != 0x0f) {

        object->field_0xe22 |= 2;
        object->force_glow_candidate = best;
        object->force_glow_candidate_kind = 2;
        SetObjAsHeadTarget(object, best, 2, 1.0f, 0.0f, 0.0f);
        if (activate == 0 && !objInNetWaitContext(object, 0x1b)) {
            return;
        }

        if (object->id == id_LUKESKYWALKERDAGOBAH && dagobah_training != 0 && object->field_0xcc0 == NULL &&
            FreePlay == 0) {
            if (AnimPlaying(&object->apiobj.anim_packet, 0x0b, 1, 1) == NULL &&
                AnimPlaying(&object->apiobj.anim_packet, 0x27, 1, 1) == NULL) {
                PlaySfx(const_cast<char *>("JForcePush"), &object->apiobj.collision_position);
                PlayGruntSfx(object);
            }
            NewBuzzFrames(object->pad_gamepad->pad, 2, 0);
            object->apiobj.movement_facing_angle =
                NuAtan2D(best->apiobj.collision_position.x - object->apiobj.collision_position.x,
                         best->apiobj.collision_position.z - object->apiobj.collision_position.z);
            object->field_0xe23 |= 1;
            GameCam_HitRoll();
            return;
        }

        object->character_context = 0x1b;
        object->force_target = best;
        object->field_0x7a3 = 0;
        object->context_animation_timer = 0.0f;
        object->airborne_action_duration = 0.3f;
        GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
        object->field_0xe21 &= ~3;
        object->field_0xe20 &= ~0x80;
        if (selected_direct != 0) {
            object->field_0xe21 |= 2;
        } else if (selected_choke != 0 || selected_second != 0) {
            object->field_0xe20 |= 0x80;
            if (selected_second != 0) {
                object->field_0xe21 |= 1;
            }
        }

        if ((object->field_0xe21 & 4) == 0) {
            FaceOpponent(object, NULL);
            if ((object->field_0xe21 & 2) == 0) {
                Player_ClearContext(best, 1);
                Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(best->player_packet));
                if ((object->field_0xe21 & 1) != 0) {
                    best->context_animation = 0x54;
                    best->action_movement_state = 2;
                } else if ((object->field_0xe20 & 0x80) != 0) {
                    PlaySfx(const_cast<char *>("ForceChokeCrunch"), &best->apiobj.collision_position);
                    best->context_animation = 0x53;
                    best->action_movement_state = 3;
                } else {
                    best->context_animation = best->apiobj.character_model->model_data_b[0x2b] != NULL
                                                  ? 0x2b
                                                  : (best->apiobj.character_model->model_data_b[5] != NULL ? 5 : 0x2b);
                    best->action_movement_state = 0;
                    if (object->apiobj.player_controlled && Cheat_IsOn(0x13)) {
                        best->action_movement_state = 4;
                    }
                }
                best->force_target = object;
                best->character_context = 0x1c;
                FastWeaponIn(best, 0);
                FaceOpponent(best, NULL);
            } else {
                PlaySfx(const_cast<char *>("ForceMindTrick"), &object->apiobj.collision_position);
            }
        }
        PlaySfx(const_cast<char *>("JForcePush"), &object->apiobj.collision_position);
        PlayGruntSfx(best);
    }
}

i32 ForcePushed_SetTargetMom(GameObject_s *object, float *seek_rate) {
    GameObject_s *source = object->force_target;
    if (source == NULL) {
        object->target_velocity.x = object->target_velocity.z = 0.0f;
        return 0;
    }
    if ((source->action_flags & 0x180) != 0) {
        object->target_velocity.x = object->target_velocity.z = 0.0f;
        if (source->character_context == 0x1b && source->field_0x7a3 == 1) {
            NUVEC target = {source->external_force.x,
                            source->external_force.y - object->character_bottom * object->apiobj.field_0xa8,
                            source->external_force.z};
            NUVEC direction;
            float distance_squared = NuVecDistSqr(&target, &object->apiobj.position, &direction);
            if (!(0.1f * 0.1f > distance_squared) &&
                (!(0.2f * 0.2f > distance_squared) || (NuFsqrt(distance_squared) - 0.1f) / 0.1f > 0.0f)) {
                NuVecNorm(&direction, &direction);
                NuVecScale(&object->target_velocity, &direction, 1.0f);
            }
            *seek_rate = 20.0f;
            return 1;
        }
        return 0;
    }
    if (object->action_movement_state == 4 && Cheat_IsOn(0x13) != 0) {
        u16 angle = NuAtan2D(object->apiobj.pos_x - source->apiobj.pos_x, object->apiobj.pos_z - source->apiobj.pos_z);
        object->target_velocity.x = -NU_SIN_LUT(angle) * 2.5f;
        object->target_velocity.z = -NU_COS_LUT(angle) * 2.5f;
        return 0;
    }
    GameObject_s *nearest = NULL;
    if (ForcePush_SuperPush != 0 && static_cast<i8>(object->apiobj.flags_low) >= 0 &&
        (source->action_flags & 0x380) == 0) {
        float push_x = object->apiobj.position.x - source->apiobj.position.x;
        float push_z = object->apiobj.position.z - source->apiobj.position.z;
        float nearest_distance_squared = 2.25f;
        GameObject_s *candidate = Obj;
        i32 loop_limit = HIGHGAMEOBJECT;
        for (i32 index = 0; index < loop_limit; ++index, ++candidate) {
            if ((candidate->apiobj.field_0x1f8 & 0x1001) != 0x1001 || candidate->apiobj.field_0x287 != 0 ||
                candidate == source || candidate == object || candidate->apiobj.field_0x27c != -1 ||
                (candidate->field_0xefb & 8) != 0) {
                continue;
            }
            if (CannotKill(candidate) != 0) {
                loop_limit = HIGHGAMEOBJECT;
                continue;
            }
            if ((candidate->apiobj.character_data->model_flags & 0x4002010) != 0x10) {
                loop_limit = HIGHGAMEOBJECT;
                continue;
            }
            GAMECHARACTERDATA *character =
                static_cast<GAMECHARACTERDATA *>(candidate->apiobj.character_data->field11_0x24);
            if ((character->flags_090 & 0x40) != 0 || (character->flags_094[1] & 2) != 0 ||
                candidate->id == id_GONKDROID || candidate->apiobj.collision_min.y > object->apiobj.collision_max.y ||
                object->apiobj.collision_min.y > candidate->apiobj.collision_max.y) {
                loop_limit = HIGHGAMEOBJECT;
                continue;
            }
            float dx = candidate->apiobj.position.x - object->apiobj.position.x;
            float dz = candidate->apiobj.position.z - object->apiobj.position.z;
            if (!(0.0f > push_x * dx + push_z * dz)) {
                loop_limit = HIGHGAMEOBJECT;
                float distance_squared = dx * dx + dz * dz;
                if (distance_squared < nearest_distance_squared) {
                    nearest_distance_squared = distance_squared;
                    nearest = candidate;
                }
            } else {
                loop_limit = HIGHGAMEOBJECT;
            }
        }
    }
    float dx;
    float dz;
    if (nearest != NULL) {
        dx = nearest->apiobj.pos_x - object->apiobj.pos_x;
        dz = nearest->apiobj.pos_z - object->apiobj.pos_z;
    } else {
        dx = object->apiobj.pos_x - source->apiobj.pos_x;
        dz = object->apiobj.pos_z - source->apiobj.pos_z;
    }
    u16 angle = NuAtan2D(dx, dz);
    object->target_velocity.x = NU_SIN_LUT(angle) * 2.5f;
    object->target_velocity.z = NU_COS_LUT(angle) * 2.5f;
    return 0;
}

i32 ForcePushed_YRotation(GameObject_s *object) {
    GameObject_s *source = object->force_target;
    if (source != NULL && source->character_context == 0x1b && (source->action_flags & 0x380) == 0 &&
        (object->apiobj.velocity.x != 0.0f || object->apiobj.velocity.z != 0.0f)) {
        object->apiobj.movement_facing_angle = NuAtan2D(object->apiobj.velocity.x, object->apiobj.velocity.z) + 0x8000;
        if (object->action_movement_state == 4 && Cheat_IsOn(0x13) != 0) {
            object->apiobj.movement_facing_angle += 0x8000;
        }
    } else {
        FaceOpponent(object, NULL);
    }
    return 0;
}

i32 ForcePushed_SuperPush_Occurring(GameObject_s *first, GameObject_s *second) {
    if (first->character_context == 0x1c) {
        if (first->action_movement_state != 0) {
            return 0;
        }
        GameObject_s *source = first->force_target;
        if (source == NULL || source->character_context != 0x1b || (source->action_flags & 0x380) != 0 ||
            second->id == id_GONKDROID || (second->apiobj.field_0x1f8 & 0x1001) != 0x1001 ||
            second->apiobj.field_0x287 != 0 || source == second || first == second ||
            second->apiobj.field_0x27c != -1 || (second->field_0xefb & 8) != 0 || CannotKill(second) != 0) {
            return 0;
        }
        CHARACTERDATA *character = second->apiobj.character_data;
        if ((character->model_flags & 0x4002010) != 0x10) {
            return 0;
        }
        GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(character->field11_0x24);
        if ((data->flags_090 & 0x40) != 0 || (data->flags_094[1] & 2) != 0) {
            return 0;
        }
        return 1;
    }
    if (second->character_context != 0x1c || second->action_movement_state != 0) {
        return 0;
    }
    GameObject_s *source = second->force_target;
    if (source == NULL || source->character_context != 0x1b || (source->action_flags & 0x380) != 0 ||
        first->id == id_GONKDROID || (first->apiobj.field_0x1f8 & 0x1001) != 0x1001 || first->apiobj.field_0x287 != 0 ||
        first == second || first == source || first->apiobj.field_0x27c != -1 || (first->field_0xefb & 8) != 0 ||
        CannotKill(first) != 0) {
        return 0;
    }
    CHARACTERDATA *character = first->apiobj.character_data;
    if ((character->model_flags & 0x4002010) != 0x10) {
        return 0;
    }
    GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(character->field11_0x24);
    if ((data->flags_090 & 0x40) != 0 || (data->flags_094[1] & 2) != 0) {
        return 0;
    }
    return 1;
}

void ResetForceBack() {
    ForceBackObj = NULL;
    ForceBackPos = NULL;
}

void SetForceBack(GameObject_s *object, nuvec_s *position, float radius, i32 type) {
    ForceBackRadius = radius;
    ForceBackObj = object;
    ForceBackPos = object != NULL ? &object->apiobj.collision_position : position;
    ForceBackRadius2 = radius * radius;
    ForceBackType = type;
}

void DrawForceBackEffect(nuhspecial_s *special) {
    if (special == NULL || !NuSpecialExistsFn(special)) {
        return;
    }
    if (ForceBackObj != NULL && ForceBackType != 3) {
        NuSpecialSetVisibility(special, 1);
        NUMTX matrix = *NuSpecialGetDrawMtx(special);
        NUVEC position;
        position.x = ForceBackObj->apiobj.lower_position.x;
        position.y = 0.005f + ForceBackObj->apiobj.field_0x218;
        position.z = ForceBackObj->apiobj.lower_position.z;
        NUVEC scale;
        scale.x = scale.y = scale.z = ForceBackRadius;
        NuMtxSetTranslation(&matrix, &position);
        NuMtxPreScale(&matrix, &scale);
        NuSpecialSetDrawMtx(special, &matrix);
        NuSpecialUpdate(special);
    } else {
        NuSpecialSetVisibility(special, 0);
    }
}

void ForcedBackCode(GameObject_s *object) {
    f32 distance = 1000000.0f;
    if (ForceBackPos != NULL && (object->field_0xeff & 0x10) == 0)
        distance = NuVecDistSqr(&object->apiobj.collision_position, ForceBackPos, NULL);
    if (object->character_context == 0x22) {
        if (MosEisleyC_PastBarrier(object))
            object->apiobj.position.z = 5.85f;
        object->field_0x768 += FRAMETIME;
        if (object->context_animation_timer > 0.0f) {
            object->context_animation_timer -= FRAMETIME;
            if (object->context_animation_timer < 0.0f)
                object->context_animation_timer = 0.0f;
        }
        if (ForceBackPos == NULL || distance > (ForceBackRadius + 0.2f) * (ForceBackRadius + 0.2f)) {
            if (object->context_animation_timer <= 0.0f) {
                object->character_context = -1;
                object->field_0xeff &= ~2;
                object->ground_contact_grace_timer = 0.0f;
                object->movement_runtime_flags |= 4;
            }
        } else if (ForceBackType == 4 && object->context_animation_timer < 0.2f) {
            object->context_animation_timer = 0.2f;
        }
        if (ForceBackType == 2)
            AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[111].effect,
                                              &object->apiobj.collision_position, 50, FRAMETIME, 0, 0, NULL);
        NewRumble(object->pad_gamepad->pad, (qrand() * (1.0f / 65535.0f)) * 0.6f, 0);
    } else if (ForceBackPos != NULL && object != ForceBackObj &&
               (ForceBackType != 2 || (object->apiobj.character_data->model_flags & 0x10) != 0)) {
        if (MosEisleyC_PastBarrier(object))
            return;
        if ((ForceBackType != 3 || ForceBackObj == NULL || ForceBackObj->force_target == object) &&
            distance < (ForceBackRadius - 0.2f) * (ForceBackRadius - 0.2f)) {
            Player_ClearContext(object, 1);
            Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
            object->context_animation_timer = 0.5f;
            object->field_0x768 = 0.0f;
            object->character_context = 0x22;
            object->context_animation = 5;
            PlaySfx("JForcePush", &object->apiobj.collision_position);
        }
    }
}

void ResetForceGlow(PLAYERPACKET_s *packet) {
    packet->force_glow_x = 0.0f;
    packet->force_glow_y = 0.0f;
    packet->force_glow_z = 0.0f;
    packet->force_glow_object = NULL;
    packet->force_glow_candidate = NULL;
    packet->force_glow_intensity = 0.2f;
}

static void ForceGlowCode(GameObject_s *object, i32 model) {
    if (object->field_0xd80 == 0.0f &&
        (object->force_glow_previous == NULL || object->force_glow_previous == object->force_glow_candidate)) {
        object->force_glow_object = object->force_glow_candidate;
        object->force_glow_kind = object->force_glow_candidate_kind;
    }
    f32 target;
    if (object->force_glow_object == NULL || object->force_glow_candidate != object->force_glow_object) {
        target = FORCEGLOWTIME;
        if (object->field_0xd80 == target && object->force_hold_time > 0.0f) {
            object->force_hold_time -= FRAMETIME;
        } else
            target = 0.0f;
    } else if ((object->apiobj.flags_low & 0x80) == 0 && object->force_glow_kind == 0 &&
               object->character_context != 8 && AnimPlaying(&object->apiobj.anim_packet, 0xb, 1, 1) == NULL &&
               AnimPlaying(&object->apiobj.anim_packet, 0x27, 1, 1) == NULL)
        target = 0.0f;
    else
        target = FORCEGLOWTIME;
    f32 previous = object->field_0xd80;
    if (object->force_glow_target == previous)
        object->force_glow_target = target;
    else {
        if (previous > object->force_glow_target) {
            object->field_0xd80 = previous - FRAMETIME * 0.5f;
            if (object->field_0xd80 < 0.0f)
                object->field_0xd80 = 0.0f;
        } else {
            object->field_0xd80 = previous + FRAMETIME;
            if (object->field_0xd80 > object->force_glow_target)
                object->field_0xd80 = object->force_glow_target;
        }
        if (object->field_0xd80 == FORCEGLOWTIME && previous != FORCEGLOWTIME)
            object->force_hold_time = 0.333f;
    }
    if (object->field_0xd80 > 0.0f && object->force_glow_object == NULL)
        ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    if (!(object->field_0xd80 > 0.0f) || object->force_glow_object == NULL)
        return;
    switch (object->force_glow_kind) {
        case 0: {
            GIZFORCE_s *force = static_cast<GIZFORCE_s *>(object->force_glow_object);
            if ((force->progress_flags & 1) == 0) {
                ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
                return;
            }
            f32 radius = force->radius;
            object->field_0xd8c = TouchHacks::TouchControlsActive && force->strength_0x6c > 0.0f
                                      ? force->strength_0x6c
                                      : force->horizontal_range * radius;
            object->field_0xe1e = model;
            object->force_glow_position = force->position;
            force->field_0xaa |= 2;
            break;
        }
        case 1: {
            GAMEANIMOBJ_s *animation = static_cast<GAMEANIMOBJ_s *>(object->force_glow_object);
            f32 scale = object->field_0xd8c;
            if (!NuSpecialExistsFn(&animation->special)) {
                ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
                return;
            }
            NuSpecialGetRadius(&animation->special, &object->force_glow_position, &object->field_0xd8c);
            NuVecMtxTransformVU0(&object->force_glow_position, &object->force_glow_position,
                                 NuSpecialGetDrawMtx(&animation->special));
            object->field_0xe1e = model;
            object->field_0xd8c *= scale;
            GIZFORCEANIMDATA_s *data = static_cast<GIZFORCEANIMDATA_s *>(animation->object_data);
            if (data != NULL)
                data->force_glow_active = 1;
            break;
        }
        case 2: {
            GameObject_s *target_object = static_cast<GameObject_s *>(object->force_glow_object);
            if ((target_object->apiobj.field_0x1f8 & 0x1001) == 0 ||
                (TouchHacks::TouchControlsActive && target_object->apiobj.field_0x27c != -1)) {
                ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
                return;
            }
            f32 radius = NuFmax(target_object->apiobj.field_0x1dc, target_object->apiobj.field_0x1e0) * 1.75f;
            object->field_0xe1e = model;
            object->force_glow_position.x = target_object->apiobj.collision_position.x;
            f32 bottom = target_object->character_bottom;
            f32 scale = target_object->apiobj.field_0xa8;
            object->force_glow_position.y = bottom * scale + target_object->apiobj.position.y +
                                            (target_object->character_top - bottom) * 0.5f * scale;
            object->force_glow_position.z = target_object->apiobj.collision_position.z;
            object->field_0xd8c = radius;
            break;
        }
        case 3: {
            PART_s *part = static_cast<PART_s *>(object->force_glow_object);
            if ((part->active & 1) == 0) {
                ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
                return;
            }
            f32 radius;
            if (NuSpecialExistsFn(&part->special)) {
                NUVEC center;
                NuSpecialGetRadius(&part->special, &center, &object->field_0xd8c);
                radius = object->field_0xd8c + object->field_0xd8c;
            } else
                radius = part->radius + part->radius;
            object->field_0xe1e = model;
            object->force_glow_position = part->position;
            object->field_0xd8c = radius;
            break;
        }
    }
    object->field_0xd8c *= 1.125f;
}

void DrawForceGlowSprite(nuvec_s *position, float radius, i32 model, float alpha, GameObject_s *) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if (model == -1 || world->lev_objs[model].active == 0) {
        return;
    }
    ResetShadowMapRendering();
    NUVEC_ALIGNED16 scale;
    NUVEC direction;
    NUVEC draw_position;
    NuVecSub(&direction, position, reinterpret_cast<NUVEC *>(&pNuCam->mtx.m30));
    const f32 distance = NuFsqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    NuVecNorm(&direction, &direction);
    f32 offset = radius;
    if (radius < distance - 0.2f || (offset = distance - 0.2f, !(distance < 0.2f))) {
        NuVecScale(&direction, &direction, offset);
        NuVecSub(&draw_position, position, &direction);
        if (offset != 0.0f) {
            scale.x = scale.y = scale.z = radius * (distance - offset) / distance;
            goto draw_glow;
        }
    } else {
        NuVecScale(&direction, &direction, 0.0f);
        NuVecSub(&draw_position, position, &direction);
    }
    scale.x = scale.y = scale.z = radius;
draw_glow:
    NUMTX_ALIGNED16 matrix;
    NuMtxSetScale(&matrix, &scale);
    u16 angle = static_cast<i32>(NuFmod(GameTimer.time_elapsed, 20.0f) / 20.0f * 65536.0f);
    RotateForceGlowMatrix(&matrix, -angle);
    NuMtxMulR(&matrix, &matrix, &GameCam->render_mtx);
    NuMtxTranslate(&matrix, &draw_position);
    NuSpecialDrawAtAlpha(&WORLD->lev_objs[model].special, &matrix, alpha);
    if (WORLD->lev_objs[model + 1].active != 0) {
        NuMtxSetScale(&matrix, &scale);
        angle = static_cast<i32>(NuFmod(GameTimer.time_elapsed, 16.777f) / 16.777f * 65536.0f);
        RotateForceGlowMatrix(&matrix, angle);
        NuMtxMulR(&matrix, &matrix, &GameCam->render_mtx);
        NuMtxTranslate(&matrix, &draw_position);
        NuSpecialDrawAtAlpha(&WORLD->lev_objs[model + 1].special, &matrix, alpha);
    }
    EnableShadowMapRendering(0);
}

void SetBobaRocketTarget(MechObjectInterface *target) {
    BobaRocketTarget = target;
}

i32 GetShootDirection_LSW(GameObject_s *object, nuvec_s *direction) {
    NUVEC temporary;
    if (direction == NULL)
        direction = &temporary;
    if (object->field_0x1086 == 4) {
        NUMTX matrix = object->apiobj.field_0xb8;
        NuMtxPreRotateY(&matrix, 0x8000);
        NuVecMtxRotate(direction, &v001, &matrix);
        return object->apiobj.facing_angle;
    }
    characterdata_s *model = object->apiobj.character_data;
    GAMECHARACTERDATA *data = model->game_character;
    if (data->weapon_shoot_joints[0] != -1 &&
        ((object->id == id_ATAT && (object->apiobj.flags_low & 0x80) == 0) || object->id == id_CLONEWALKER ||
         object->id == id_ATST || object->id == id_ATST_LOWRES)) {
        direction->x = direction->y = 0.0f;
        direction->z = object->id == id_ATAT ? 1.0f : -1.0f;
        NuVecMtxRotate(direction, direction, &object->joint_matrices[data->weapon_shoot_joints[0]]);
        return NuAtan2D(direction->x, direction->z);
    }
    u16 angle;
    if ((model->model_flags & 0x2000) == 0 && (data->flags_090 & 0x80) == 0) {
        angle = object->apiobj.movement_facing_angle;
    } else {
        angle = object->apiobj.facing_angle;
        if (object->character_context == 0x2a &&
            0.25f <= 1.0f - object->context_animation_timer / object->airborne_action_duration) {
            angle -= 0x8000;
        }
    }
    direction->x = NU_SIN_LUT(angle);
    direction->y = 0.0f;
    direction->z = NU_COS_LUT(angle);
    return angle;
}

u16 GetShootDirection_Batman(GameObject_s *object, nuvec_s *direction) {
    NUVEC temporary;
    if (direction == NULL) {
        direction = &temporary;
    }
    u16 angle;
    if (object->field_0x1086 == 4) {
        NUMTX matrix = object->apiobj.field_0xb8;
        NuMtxPreRotateY(&matrix, 0x8000);
        NuVecMtxRotate(direction, &v001, &matrix);
        angle = object->apiobj.facing_angle;
    } else {
        if ((object->apiobj.character_data->model_flags & 0x2000) == 0 &&
            (object->apiobj.character_data->game_character->flags_090 & 0x80) == 0) {
            angle = object->apiobj.movement_facing_angle;
        } else {
            angle = object->apiobj.facing_angle;
            if (object->character_context == 0x2a &&
                0.25f <= 1.0f - object->context_animation_timer / object->airborne_action_duration) {
                angle -= 0x8000;
            }
        }
        direction->x = NU_SIN_LUT(angle);
        direction->y = 0.0f;
        direction->z = NU_COS_LUT(angle);
    }
    return angle;
}

i32 SetObjTarget(GameObject_s *object, GameObject_s *target) {
    object->attack_target_position = target->apiobj.collision_position;
    if ((object->apiobj.character_data->model_flags & 0x2000) == 0 && (object->apiobj.field_0x1f4 & 1) != 0)
        object->attack_target_position.y += Bolt_ObjTargetPosYAdjust(target);
    object->attack_target_velocity = v000;
    object->field_0xe21 |= 8;
    object->collision_target = VehicleArea != 0 ? NULL : target;
    return 1;
}

i32 SetGizmoBlowUpTarget(GameObject_s *object, GIZMOBLOWUP_s *blowup) {
    object->attack_target_position = blowup->mid_position;
    object->attack_target_velocity = v000;
    object->attack_blowup_target = VehicleArea != 0 ? NULL : blowup;
    object->field_0xe21 |= 8;
    return 1;
}

i32 SetPartTarget(GameObject_s *object, PART_s *target) {
    object->attack_target_position = target->position;
    object->attack_target_velocity = target->velocity;
    object->field_0xe21 |= 8;
    object->attack_part_target = VehicleArea != 0 ? NULL : target;
    return 1;
}

static void FireCode(GameObject_s *object, i32 pressed, i32 held, f32 fire_delay, i32 delay_pressed) {
    f32 timer = object->quick_shoot_timer;
    if (timer > 0.0f) {
        timer -= FRAMETIME;
        object->quick_shoot_timer = timer;
    }

    if (BonusWinner != -1) {
        return;
    }

    const i8 context = object->character_context;
    if (context == -1 || context == 0x2a || context == 0x36) {
        if (object->apiobj.field_0x287 != 0) {
            return;
        }
        if (context == 0x36) {
            const f32 turn_progress = 1.0f - object->context_animation_timer / object->airborne_action_duration;
            if (turn_progress > 0.2f && turn_progress < 0.666f) {
                return;
            }
        }
    } else if (context == 0x3a) {
        if (object->apiobj.field_0x287 != 0) {
            return;
        }
    } else {
        return;
    }

    if (delay_pressed != 0 && timer > 0.0f) {
        return;
    }
    if (pressed == 0) {
        if (held == 0 || !(timer <= 0.0f)) {
            return;
        }
    }

    i32 bolt_id;
    if ((object->apiobj.character_data->model_flags & 0x04000000) != 0)
        bolt_id = BoltType_FindIDByCreature(object, 0x15);
    else
        bolt_id = BoltType_FindIDByCreature(object, 5);
    BOLTTYPE_s *bolt_type = BoltType_FindByID(bolt_id, WORLD);
    const u32 target_flags = bolt_type->field_60;

    if (object->apiobj.player_controlled) {
        NUVEC direction;
        BoltSys->shoot_direction(object, &direction);

        f32 range;
        if (BOLT_OVERRIDE_PLAYERBOLTSPEED == 0.0f || BOLT_OVERRIDE_PLAYERBOLTDURATION == 0.0f) {
            range = bolt_type->field_10 * bolt_type->field_14;
        } else {
            range = BOLT_OVERRIDE_PLAYERBOLTSPEED * BOLT_OVERRIDE_PLAYERBOLTDURATION;
        }

        const i16 animation = object->context_animation;
        object->context_animation = 2;
        NUVEC origin;
        BoltSys->shoot_origin(object, &origin);

        const AREADATA_s *area = WORLD->area;

        if (area != NULL && area == BONUS_GUNSHIP_ADATA) {
            if (object->id == id_REPUBLICGUNSHIP || object->id == id_REPUBLICGUNSHIP_GREEN) {
                i32 angle;
                if (player2 == NULL) {
                    angle = WORLD->current_level == BONUS_GUNSHIPB_LDATA ? 3 : 8;
                } else if (object == player) {
                    angle = WORLD->current_level == BONUS_GUNSHIPB_LDATA ? 4 : 10;
                } else if (object == player2) {
                    angle = WORLD->current_level == BONUS_GUNSHIPB_LDATA ? 0 : 5;
                } else {
                    angle = WORLD->current_level == BONUS_GUNSHIPB_LDATA ? 3 : 8;
                }

                NUVEC offset = {0.0f, 0.0f, 100.0f};
                NuVecRotateX(&offset, &offset, (angle << 16) / 360);
                NuVecRotateY(&offset, &offset, object->apiobj.movement_facing_angle);
                NuVecAdd(&object->attack_target_position, &origin, &offset);
                object->attack_target_velocity = v000;
                object->field_0xe21 |= 8;
            }
        } else {
            if ((bolt_type->field_60 & 0x800) == 0 && (area == NULL || area != DOGFIGHT_ADATA)) {
                const i32 target_mode = ((target_flags >> 14) ^ 1) & 1;
                const f32 range_squared = range * range;
                GameObject_s *target =
                    TargetGameObject(object, &origin, &direction, range, range_squared, 0, target_mode, 0, bolt_id);
                if (target != NULL) {
                    SetObjTarget(object, target);
                    if (VehicleArea != 0 && (target->apiobj.character_data->model_flags & 0x2000) != 0) {
                        NUVEC lateral;
                        NuVecRotateY(&lateral, &v001,
                                     NuAtan2D(target->apiobj.velocity.x, target->apiobj.velocity.z) + 0x4000);
                        const f32 lead =
                            fabsf(lateral.x * direction.x + lateral.z * direction.z) * 0.5f * bolt_type->field_14;
                        NUVEC position = {
                            target->apiobj.collision_position.x + target->apiobj.velocity.x * lead,
                            target->apiobj.collision_position.y + target->apiobj.velocity.y * lead,
                            target->apiobj.collision_position.z + target->apiobj.velocity.z * lead,
                        };
                        NuVecSub(&position, &position, &origin);
                        NuVecNorm(&position, &position);
                        NuVecScale(&position, &position, range);
                        NuVecAdd(&object->attack_target_position, &origin, &position);
                    }
                } else if (GizmoSys_SetBestBoltTarget(WORLD->gizmo_sys, WORLD, object, &origin, &direction, range,
                                                      range_squared, target_mode, 0, bolt_id) == 0) {
                    GIZMOBLOWUP_s *blowup =
                        GizmoBlowUp_Target(object, &origin, &direction, range, range_squared, target_mode, 0, bolt_id);
                    if (blowup != NULL) {
                        SetGizmoBlowUpTarget(object, blowup);
                    } else {
                        PART_s *part =
                            TargetPart(object, &origin, &direction, range, range_squared, target_mode, bolt_id);
                        if (part != NULL) {
                            SetPartTarget(object, part);
                        } else if ((bolt_type->field_60 & 0x400) != 0) {
                            object->attack_target_position.x =
                                direction.x * range + origin.x + object->apiobj.velocity.x * bolt_type->field_14;
                            if (object->field_0x1086 == 4) {
                                object->attack_target_position.y =
                                    direction.y * range + origin.y + object->apiobj.velocity.y * bolt_type->field_14;
                            } else {
                                object->attack_target_position.y = origin.y;
                            }
                            object->attack_target_position.z =
                                direction.z * range + origin.z + object->apiobj.velocity.z * bolt_type->field_14;
                            object->field_0xe21 |= 8;
                            object->attack_target_velocity = v000;
                        }
                    }
                }
            }
        }

        object->context_animation = animation;
    }

    if (!(WORLD->area == VEHICLES_ADATA && object->id == id_MILLENNIUMFALCON)) {
        ShootThisFrame(object, bolt_id, 3);
        object->quick_shoot_timer = fire_delay;
        object->bolt_fire_phase = object->bolt_fire_phase == 0;
    } else {
        object->bolt_fire_phase = 0;
        ShootThisFrame(object, bolt_id, 3);
        object->quick_shoot_timer = fire_delay;
    }
}

void StartQuickShoot(GameObject_s *object, i32 action) {
    object->field_0xe21 &= ~8;
    i32 bolt_id = BoltType_FindIDByCreature(object, 0);
    NUVEC direction;
    BoltSys->shoot_direction(object, &direction);
    f32 speed = BoltType_FindByID(bolt_id, WORLD)->field_10;
    f32 range = speed * BoltType_FindByID(bolt_id, WORLD)->field_14;
    f32 range_squared = range * range;
    GameObject_s *target = TargetGameObject(object, &object->apiobj.collision_position, &direction, range,
                                            range_squared, 0, 1, 0, bolt_id);
    if (target != NULL)
        SetObjTarget(object, target);
    else if ((object->apiobj.flags_low & 0x80) != 0 &&
             GizmoSys_SetBestBoltTarget(WORLD->gizmo_sys, WORLD, object, &object->apiobj.collision_position, &direction,
                                        range, range_squared, 1, 0, bolt_id) == 0) {
        GIZMOBLOWUP_s *blowup = GizmoBlowUp_Target(object, &object->apiobj.collision_position, &direction, range,
                                                   range_squared, 1, 0, bolt_id);
        if (blowup != NULL)
            SetGizmoBlowUpTarget(object, blowup);
        else {
            PART_s *part =
                TargetPart(object, &object->apiobj.collision_position, &direction, range, range_squared, 1, bolt_id);
            if (part != NULL)
                SetPartTarget(object, part);
            else {
                target = TargetGameObject(object, &object->apiobj.collision_position, &direction, range, range_squared,
                                          0x200, 1, 0, bolt_id);
                if (target != NULL)
                    SetObjTarget(object, target);
            }
        }
    }
    object->character_context = 10;
    object->context_animation = action;
    object->context_animation_timer = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, action != 0x57);
    ResetAnimPacket(&object->apiobj.anim_packet, -1);
    object->apiobj.anim_packet.flags |= 0x10;
    i32 fire_now = !(AnimListFrame(object->apiobj.character_model, object->context_animation, 1) > 1.0f);
    bolt_id = BoltType_FindIDByCreature(object, 0);
    i32 flags = 1 + 2 * fire_now;
    if (object == Player[0] && nextShootTarget.Get() != NULL)
        nextShootTarget = NuMechPtr<MechObjectInterface, 4>();
    if ((object->apiobj.field_0x1f4 & 0x40000) == 0 || object->apiobj.field_0x27c == -1) {
        NewBuzzFrames(object->pad_gamepad->pad, (object->apiobj.character_data->model_flags & 0x2000) != 0 ? 1 : 2, 0);
        object->quick_shoot_flags = flags;
        object->quick_shoot_bolt_id = bolt_id;
        object->field_0xef9 |= 8;
        if ((object->apiobj.character_data->game_character->flags_098[0] & 2) != 0)
            SetWeaponIn(object);
        if (object->field_0x7e4 != NULL && object->field_0x7e4[8] == 2 && object->field_0x7e8 != 0)
            --object->field_0x7e8;
    }
    object->quick_shoot_timer = 0.0f;
    ++object->action_suppressed;
    SetWeaponOut(object);
}

static void DodgeCode(GameObject_s *object, i32 action_pressed, i32 jump_pressed) {
    if (object->character_context == 0x25) {
        if (action_pressed != 0) {
            object->landing_followup = 2;
        } else if (jump_pressed != 0 && object->context_animation == 0x58) {
            object->landing_followup = 1;
        }

        if (AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0) == NULL) {
            return;
        }
        object->context_animation_timer -= FRAMETIME;
        if (!(object->context_animation_timer <= 0.0f)) {
            return;
        }

        object->character_context = -1;
        ResetAnimPacket(&object->apiobj.anim_packet, -1);
        object->apiobj.velocity.z = 0.0f;
        object->apiobj.velocity.x = 0.0f;
        if (FaceOpponent(object, NULL) != 0) {
            object->apiobj.field_0x276 = object->apiobj.movement_facing_angle;
            object->apiobj.facing_angle = object->apiobj.movement_facing_angle;
        }

        if (object->landing_followup == 2 &&
            PlayerItem_GotAmmo(reinterpret_cast<PLAYERITEM_s *>(&object->field_0x7e4)) != 0) {
            if (object->context_animation == 0x58 && object->pad_gamepad->input_magnitude > 0.0f) {
                SetWeaponOut(object);
                i32 bolt_id = BoltType_FindIDByCreature(object, 0);
                ShootThisFrame(object, bolt_id, 3);
            } else {
                StartQuickShoot(object, 0x16);
            }
            object->quick_shoot_timer = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1);
        } else if (object->landing_followup == 1) {
            StartJump(object, 0);
        }
        return;
    }

    if (object->apiobj.player_controlled && object->apiobj.field_0x27d != 0 &&
        (object->character_context == -1 || (object->character_context == 0x0a && object->context_animation != 0x57))) {
        GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
        if (object->pad_gamepad->input_magnitude <= (data->walk_speed + data->movement_speed) * 0.5f) {
            BOLT_s *bolt = FindIncomingBolt(object, 1, 1);
            object->incoming_bolt = bolt;
            if (bolt == NULL)
                return;
            if (action_pressed == 0)
                return;

            const i16 previous_animation = object->context_animation;
            NUVEC dodge_direction = bolt->field_0xac;
            NuVecRotateY(&dodge_direction, &dodge_direction, 0x4000);

            if ((object->field_0xe21 & 0x20) != 0) {
                if (object->field_0xe12 == 0x26)
                    object->context_animation = 0x4f;
                else
                    object->context_animation = 0x26;
            } else {
                const f32 side = (object->apiobj.collision_position.x - bolt->position.x) * dodge_direction.x +
                                 (object->apiobj.collision_position.z - bolt->position.z) * dodge_direction.z;
                if (side < 0.0f)
                    object->context_animation = 0x4f;
                else
                    object->context_animation = 0x26;
            }
            object->field_0xe12 = object->context_animation;

            if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL) {
                object->context_animation_timer = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 0);
                if (object->context_animation_timer > 0.0f) {
                    object->character_context = 0x25;
                    object->blowup_target = NULL;
                    object->landing_followup = 2;
                    ResetAnimPacket(&object->apiobj.anim_packet, -1);
                    if (object->context_animation == 0x58 && object->pad_gamepad->input_magnitude > 0.0f) {
                        object->force_target = NULL;
                    } else {
                        object->force_target = bolt->owner;
                        if (FaceOpponent(object, NULL) != 0) {
                            object->apiobj.field_0x276 = object->apiobj.movement_facing_angle;
                            object->apiobj.facing_angle = object->apiobj.movement_facing_angle;
                        }
                    }
                    if (object->apiobj.player_controlled) {
                        Hint_SetComplete(0x265);
                    }
                }
            }
            if (object->character_context != 0x25) {
                object->context_animation = previous_animation;
            }
        }
    }
}

static i32 ShootCode(GameObject_s *object, i32 pressed, i32 special_pressed, i32 weapon_mode, i32 allow_airborne,
                     i32 fire_mode) {
    GameObject_s *target = NULL;
    GIZMOBLOWUP_s *blowup = NULL;
    if (object == Player[0] && nextShootTarget.Get() != NULL && pressed != 0) {
        target = nextShootTarget->GetCharacterObject();
        if (object == Player[0] && nextShootTarget.Get() != NULL)
            blowup = nextShootTarget->GetGizBlowup();
    }
    const i32 bolt_id =
        object->id == id_GEONOSIAN && (object->field_0xefd & 2) != 0 ? 0x13 : BoltType_FindIDByCreature(object, 0);
    if (object->quick_shoot_timer > 0.0f)
        object->quick_shoot_timer -= FRAMETIME;
    if (object->character_context == 0x0a) {
        if (AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0) == NULL)
            return 0;
        const bool ammunition = PlayerItem_GotAmmo(reinterpret_cast<PLAYERITEM_s *>(&object->field_0x7e4)) != 0;
        if (ammunition && pressed != 0 && object->quick_shoot_timer <= 0.0f && object->context_animation == 0x57 &&
            object->action_suppressed <= 2) {
            if (object->pad_gamepad->input_magnitude > 0.0f) {
                const u16 angle = GamePad_InputAngle(object, object->pad_gamepad);
                object->apiobj.field_0x276 = angle;
                object->apiobj.movement_facing_angle = angle;
                object->apiobj.facing_angle = angle;
            }
            StartQuickShoot(object, 0x57);
            if (object->action_suppressed == 3)
                object->quick_shoot_timer = 0.5f;
            return 0;
        }
        if (static_cast<u16>(object->context_animation - 0x5a) <= 2) {
            if ((object->field_0xe22 & 4) == 0) {
                const f32 event_frame = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
                if (event_frame > 1.0f) {
                    const f32 frame = object->apiobj.anim_packet.blending != 0
                                          ? object->apiobj.anim_packet.blend_target_time
                                          : object->apiobj.anim_packet.current_time;
                    if (frame >= event_frame) {
                        if (ammunition) {
                            ShootThisFrame(object, bolt_id, 1);
                            object->field_0xe21 |= 8;
                        }
                        object->field_0xe22 |= 4;
                    }
                }
            }
        } else if (object->reserved_e30 == 0) {
            const f32 event_frame = AnimListFrame(object->apiobj.character_model, object->context_animation, 1);
            if (event_frame > 1.0f) {
                const f32 frame = object->apiobj.anim_packet.blending != 0
                                      ? object->apiobj.anim_packet.blend_target_time
                                      : object->apiobj.anim_packet.current_time;
                if (frame >= event_frame) {
                    if (ammunition)
                        ShootThisFrame(object, bolt_id, 2);
                    object->reserved_e30 = 1;
                }
            }
        }
        object->context_animation_timer -= FRAMETIME;
        if (!(object->context_animation_timer <= 0.0f))
            return 0;
        object->field_0xe21 &= ~8;
        object->character_context = -1;
        if (static_cast<u16>(object->context_animation - 0x5a) <= 2 && (object->field_0xe22 & 4) == 0 && ammunition)
            ShootThisFrame(object, bolt_id, 1);
        return 0;
    }

    if ((object->field_0xef8 & 8) != 0 && pressed != 0 && (CInfo[object->character_context].flags & 0x20) == 0) {
        if (static_cast<i8>(object->apiobj.flags_low) < 0) {
            if ((object->character_context == 6 || object->character_context == 7) &&
                (object->apiobj.character_data->game_character->flags_094[1] & 0x10) != 0)
                return 0;
        } else if (object->character_context == 6 || object->character_context == 7) {
            goto shoot_weapon_in;
        } else if (object->character_context == 1) {
            if (object->context_animation == 0xb3 || object->context_animation == 0x59)
                goto shoot_weapon_in;
        }
        if (BonusWinner != -1)
            return 0;
        CHARACTERDATA *character = object->apiobj.character_data;
        GAMECHARACTERDATA *data = character->game_character;
        if (weapon_mode != 0 && (object->field_0xe22 & 1) == 0) {
            if (weapon_mode == 2) {
                SetWeaponOut(object);
            } else {
                if (object->weapon_scale_state == 0 && object->apiobj.field_0x27d != 0 &&
                    object->pad_gamepad->input_magnitude == 0.0f &&
                    object->apiobj.character_model->model_data_b
                            [data->uses_weapon_action == 0 && (character->model_flags & 0x80) != 0 ? 0x7f : 0x11] !=
                        NULL &&
                    (object->character_context == -1 || (CInfo[object->character_context].flags & 4) != 0))
                    SlowWeaponOut(object);
                else
                    FastWeaponOut(object, 1);
                return 0;
            }
        }
        if (object->character_context == -1 && object->field_0xe31 == 1)
            fire_mode = 2;
        if (!PlayerItem_GotAmmo(reinterpret_cast<PLAYERITEM_s *>(&object->field_0x7e4)) ||
            !(object->quick_shoot_timer <= 0.0f) || object->apiobj.field_0x287 != 0)
            return 0;
        if (fire_mode == 0 && object->apiobj.field_0x27d == 0) {
            if (object->character_context != 0 || allow_airborne == 0 ||
                (object->apiobj.character_data->game_character->flags_094[2] & 0x40) != 0)
                return 0;
        } else if (object->character_context == 0x25) {
            return 0;
        }
        if (object->character_context == 0 && object->jump_sequence > 1)
            return 0;
        if (Cheat_IsOn(0x0f)) {
            const u8 kind = object->apiobj.character_data->game_character->uses_weapon_action;
            if (kind == 8 || kind == 1)
                return 0;
        }
        const i16 previous_animation = object->context_animation;
        object->context_animation = fire_mode == 2 ? 0x3c : 0x16;
        if (UnderPlayerControl(object)) {
            NUVEC position = object->apiobj.collision_position;
            NUVEC direction;
            BoltSys->shoot_direction(object, &direction);
            NUVEC forward = direction;
            const f32 speed = BoltType_FindByID(bolt_id, WORLD)->field_10;
            const f32 range = speed * BoltType_FindByID(bolt_id, WORLD)->field_14;
            const f32 range_squared = range * range;
            if (target == NULL && blowup == NULL)
                target = TargetGameObject(object, &position, &direction, range, range_squared, 0, 1, 0, bolt_id);
            if (target == NULL) {
                if (object->apiobj.player_controlled && object->pad_gamepad->input_magnitude == 0.0f &&
                    object->apiobj.field_0x27d != 0 && object->character_context != 0) {
                    if (object->apiobj.character_model->model_data_b[0x5b] != NULL &&
                        AnimPlaying(&object->apiobj.anim_packet, 0x5b, 1, 1) == NULL) {
                        direction.x = forward.z;
                        direction.z = -forward.x;
                        target =
                            TargetGameObject(object, &position, &direction, range, range_squared, 0, 1, 1, bolt_id);
                        if (target != NULL)
                            object->context_animation = 0x5b;
                    }
                    if (target == NULL && object->apiobj.character_model->model_data_b[0x5a] != NULL &&
                        AnimPlaying(&object->apiobj.anim_packet, 0x5a, 1, 1) == NULL) {
                        direction.x = -forward.z;
                        direction.z = forward.x;
                        target =
                            TargetGameObject(object, &position, &direction, range, range_squared, 0, 1, 1, bolt_id);
                        if (target != NULL)
                            object->context_animation = 0x5a;
                    }
                    if (target == NULL && object->apiobj.character_model->model_data_b[0x5c] != NULL &&
                        AnimPlaying(&object->apiobj.anim_packet, 0x5c, 1, 1) == NULL) {
                        direction.x = -forward.x;
                        direction.z = -forward.z;
                        target =
                            TargetGameObject(object, &position, &direction, range, range_squared, 0, 1, 1, bolt_id);
                        if (target != NULL)
                            object->context_animation = 0x5c;
                    }
                }
            }
            if (target != NULL) {
                SetObjTarget(object, target);
            } else if (blowup == NULL) {
                if (!GizmoSys_SetBestBoltTarget(WORLD->gizmo_sys, WORLD, object, &position, &forward, range,
                                                range_squared, 1, 0, bolt_id)) {
                    blowup = GizmoBlowUp_Target(object, &position, &forward, range, range_squared, 1, 0, bolt_id);
                    if (blowup != NULL) {
                        SetGizmoBlowUpTarget(object, blowup);
                    } else {
                        PART_s *part = TargetPart(object, &position, &forward, range, range_squared, 1, bolt_id);
                        if (part != NULL) {
                            SetPartTarget(object, part);
                        } else {
                            target = TargetGameObject(object, &position, &forward, range, range_squared, 0x200, 1, 0,
                                                      bolt_id);
                            if (target != NULL)
                                SetObjTarget(object, target);
                        }
                    }
                }
            } else {
                SetGizmoBlowUpTarget(object, blowup);
            }
        }
        object->quick_shoot_timer = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 0);
        object->field_0xe22 &= ~4;
        object->reserved_e30 = 0;
        if ((object->id == id_ATST || object->id == id_MINIATST || object->id == id_ATST_LOWRES ||
             object->id == id_ATAT || object->id == id_MINIATAT || object->id == id_MINIATTE) &&
            AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 1) == NULL &&
            AnimPlaying(&object->apiobj.anim_packet, 1, 0, 0) == NULL)
            goto immediate_shot;
        if (static_cast<u16>(object->context_animation - 0x5a) <= 2) {
            if (AnimListFrame(object->apiobj.character_model, object->context_animation, 0) > 1.0f) {
                object->context_animation_timer = object->quick_shoot_timer - animduration_blendouttime;
                if (object->context_animation_timer > 0.0f) {
                    object->character_context = 0x0a;
                    goto cooldown;
                }
                object->quick_shoot_timer = 0.0f;
                goto finish;
            }
        } else {
            GAMECHARACTERDATA *data = object->apiobj.character_data->game_character;
            if ((data->flags_098[0] & 4) == 0) {
                const i16 animation = object->apiobj.anim_packet.animation_index;
                if (data->uses_weapon_action == 4 && animation == 3)
                    goto immediate_shot;
                if (animation == 0x17) {
                    if (object->apiobj.character_model->model_data_b[0x17] != NULL)
                        goto immediate_shot;
                } else if (data->uses_weapon_action == 0 && animation == 0x73) {
                    if (object->apiobj.character_model->model_data_b[0x73] != NULL)
                        goto immediate_shot;
                } else if (animation == 6 && object->apiobj.character_model->model_data_b[6] != NULL) {
                    goto immediate_shot;
                }
            }
        }
        {
            const f32 duration = object->quick_shoot_timer - animduration_blendouttime;
            if (duration > 0.0f) {
                object->character_context = 0x0a;
                object->context_animation_timer = duration;
                ResetAnimPacket(&object->apiobj.anim_packet, -1);
                object->apiobj.anim_packet.flags |= 0x10;
                SetWeaponOut(object);
                if (object->id == id_CATAPULT) {
                    NewRumble(object->pad_gamepad->pad, 0.6f, 0);
                    GameCam_NewShake(GameCam, 0.5f, 0.5f, 1.0f);
                }
            }
            if ((object->apiobj.character_data->game_character->flags_098[0] & 8) == 0) {
                i32 flags = 3;
                if (duration > 0.0f)
                    flags =
                        1 + 2 * !(AnimListFrame(object->apiobj.character_model, object->context_animation, 1) > 1.0f);
                ShootThisFrame(object, bolt_id, flags);
            }
        }
        goto cooldown;
    immediate_shot:
        if ((object->apiobj.character_data->game_character->flags_098[0] & 8) == 0) {
            ShootThisFrame(object, bolt_id, 3);
            object->apiobj.velocity.x *= 0.5f;
            object->apiobj.velocity.z *= 0.5f;
        }
    cooldown:
        if (object->quick_shoot_timer <= 0.0f) {
            object->quick_shoot_timer = (qrand() * (1.0f / 65535.0f)) * 0.2f + 0.2f;
            if (object->id == id_SENTRYDROID)
                object->quick_shoot_timer *= 5.0f;
        }
        if (object->apiobj.player_controlled && target != NULL && target->apiobj.field_0x27c == -1)
            Hint_SetComplete(0x277);
    finish:
        if (object->character_context == 0x0a) {
            object->context_flags &= ~0x40;
            if (object->apiobj.character_data->move_fn == Move_CANNON) {
                object->apiobj.facing_angle = object->apiobj.field_0x276;
                object->apiobj.movement_facing_angle = object->apiobj.field_0x276;
            }
        } else {
            object->context_animation = previous_animation;
        }
        return 1;
    }
shoot_weapon_in:
    if (weapon_mode == 0)
        return 0;
    if ((object->pad_gamepad->allocated_5a & 4) == 0 && special_pressed == 0)
        return 0;
    if ((object->field_0xe22 & 1) == 0 || object->weapon_scale_state != 0)
        return 0;
    if (object->character_context == 6 || object->character_context == 7 || object->character_context == 0x47 ||
        object->character_context == 0x46 || object->character_context == 0x0b || object->character_context == 0x2e)
        return 0;
    if (object->apiobj.character_data->game_character->uses_weapon_action == 2 && object->field_0xe31 == 1)
        return 0;
    if (object->character_context == 8)
        return 0;
    if (object->character_context == 0x1b || object->character_context == 0x1d)
        return 0;
    if (TouchHacks::ShouldKeepWeaponOut(*object))
        return 0;
    if (weapon_mode == 2) {
        SetWeaponIn(object);
    } else {
        if (object->apiobj.field_0x27d != 0 && object->pad_gamepad->input_magnitude == 0.0f &&
            object->apiobj.character_model
                    ->model_data_b[object->apiobj.character_data->game_character->uses_weapon_action == 0 &&
                                           (object->apiobj.character_data->model_flags & 0x80) != 0
                                       ? 0x7e
                                       : 0x10] != NULL &&
            (object->character_context == -1 || (CInfo[object->character_context].flags & 4) != 0))
            SlowWeaponIn(object);
        else
            FastWeaponIn(object, 1);
    }
    return 0;
}

NUVEC *GetZapOrigin(GameObject_s *object) {
    if (__builtin_expect(object->apiobj.field_0x288 != 0, 0)) {
        PLAYERCHARACTERCONFIG_s *config = object->apiobj.character_data->player_config;
        i32 joint = config->weapon_shoot_joints[0];
        if (joint != -1 && object->apiobj.character_model->points_of_interest[joint] != NULL) {
            return reinterpret_cast<NUVEC *>(&object->joint_matrices[joint].m30);
        }
        joint = config->weapon_joints[0];
        if (joint != -1 && object->apiobj.character_model->points_of_interest[joint] != NULL) {
            return reinterpret_cast<NUVEC *>(&object->joint_matrices[joint].m30);
        }
    }
    return &object->apiobj.collision_position;
}

void GameObjectStuffAfterAnimation() {
    GameObject_s *object = Obj;
    i32 count = HIGHGAMEOBJECT;
    for (i32 i = 0; i < count; ++i, ++object) {
        if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001 || object->apiobj.field_0x287 != 0 ||
            object->apiobj.field_0x288 == 0)
            continue;
        ADDPART_s part;
        NUMTX matrix;
        NUVEC points[2];
        bool drawn = true;
        if (object->apiobj.model_draw_result == 0) {
            drawn = false;
            if ((object->field_0x1050 & 4) == 0)
                goto after_shooting_and_streaks;
        }
        {
            if (object->script_fire_target != NULL && (object->script_fire_target->apiobj.field_0x1f8 & 1) == 0)
                object->script_fire_target = NULL;
            if (static_cast<i8>(object->quick_shoot_bolt_id) != -1) {
                if (object->id == id_BASKETCANNON) {
                    GameAudio_PlaySfxById(
                        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->sfx_shoot,
                        &object->apiobj.collision_position, 0, 0);
                    // The original reacquires the locator after the audio callback.
                    i32 joint = drawn ? static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)
                                            ->weapon_shoot_joints[0]
                                      : -1;
                    if (drawn && joint != -1 && object->apiobj.character_model->points_of_interest[joint] != NULL)
                        matrix = object->joint_matrices[joint];
                    else {
                        matrix = object->apiobj.field_0xb8;
                        *NUMTX_GET_ROW_VEC(&matrix, 3) = object->apiobj.collision_position;
                    }
                    NUVEC velocity = {0.0f, 2.0f * (static_cast<f32>(qrand()) * (1.0f / 65535.0f)),
                                      -(3.0f + 2.0f * (static_cast<f32>(qrand()) * (1.0f / 65535.0f)))};
                    NuVecMtxRotate(&velocity, &velocity, &matrix);
                    part = Default_ADDPART;
                    part.matrix = &matrix;
                    part.velocity = &velocity;
                    part.field_28 = 90;
                    part.gravity = -10.0f;
                    part.flags = 0x10;
                    part.field_14 = 0.05f;
                    part.field_18 = 0.05f;
                    part.special = &WORLD->lev_objs[90].special;
                    part.field_48 = PartUpdate_Basketball;
                    part.field_3c = PartImpact_Basketball;
                    part.stop_fn = PartStop_Flickerer;
                    part.draw_fn = PartDraw_Flickerer;
                    part.time_step = FRAMETIME;
                    AddPart(&part);
                } else {
                    Bolt_Shoot(object, static_cast<i8>(object->quick_shoot_bolt_id), object->quick_shoot_flags);
                    if (object->quick_shoot_bolt_id == 29)
                        object->context_flags |= 0x40;
                }
            }
            if ((object->field_0xe23 & 4) != 0)
                Torpedo_Shoot(object);
        }
        if (drawn && ((object->apiobj.character_data->model_flags & 8) != 0 || object->id == id_BODYGUARD ||
                      object->id == id_IMPERIALGUARD)) {
            const i32 effect = object->blade_index == -1 ? -1 : BladeTab[object->blade_index].hit_effect;
            GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
            if ((data->field275_0x116 == 3 || object->id == id_GRIEVOUS || object->id == id_COUNTDOOKU) &&
                object->character_context == 0 && object->action_movement_state == 3)
                object->sabre_flags |= 2;
            if (object->id == id_BODYGUARD && (object->character_context == 12 || object->character_context == 24) &&
                static_cast<u16>(object->context_animation - 26) <= 1)
                object->sabre_flags |= 2;
            u8 streak = object->sabre_flags & 2;
            const u8 damage = object->sabre_flags & 4;
            if (object->id == id_GRIEVOUS) {
                bool reset_streak = (object->character_context == 12 || object->character_context == 24) &&
                                    object->context_animation == 26;
                if (reset_streak) {
                    object->sabre_flags |= 2;
                    streak = 2;
                }
                i32 first = -1, second = -1;
                if (damage != 0) {
                    if (object->character_context == 13) {
                        first = 2;
                        second = 0;
                    } else if (object->character_context == 16) {
                        first = 3;
                        second = 2;
                    } else if (object->character_context == 5 &&
                               static_cast<u16>(object->context_animation - 46) <= 13) {
                        static const i32 first_blade[14] = {-1, -1, -1, 2, 2, 2, 2, 1, 3, 3, -1, -1, -1, -1};
                        static const i32 second_blade[14] = {0, 3, 3, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2};
                        first = first_blade[object->context_animation - 46];
                        second = second_blade[object->context_animation - 46];
                    }
                }
                u8 flags = streak | 1;
                for (i32 blade = 0; blade < 4; ++blade) {
                    if (blade == 2 && reset_streak)
                        flags = 1;
                    object->sabre_flags = flags;
                    if (damage != 0 && (first == blade || second == blade))
                        object->sabre_flags |= damage;
                    if (object->character_context == 16) {
                        if (blade < 2)
                            object->sabre_flags &= ~2;
                        else
                            object->sabre_flags |= 2;
                    }
                    LightSabreStreakCode(object, blade, effect);
                }
            } else if (object->id == id_DARTHMAUL) {
                LightSabreStreakCode(object, 0, effect);
                object->sabre_flags = streak | 1;
                LightSabreStreakCode(object, 1, effect);
            } else if (object->id == id_BODYGUARD) {
                u8 first = object->sabre_flags;
                u8 second = first;
                if (object->character_context == 5 && damage != 0) {
                    if (object->combo_stage == 2) {
                        first &= ~4;
                        second |= 4;
                    } else {
                        first |= 4;
                        second &= ~4;
                    }
                } else if (object->character_context == 0 && object->action_movement_state == 4) {
                    first |= 2;
                    second = first;
                }
                object->sabre_flags = first;
                LightSabreStreakCode(object, 0, effect);
                object->sabre_flags = second;
                LightSabreStreakCode(object, 1, effect);
            } else
                LightSabreStreakCode(object, 0, effect);
        }
    after_shooting_and_streaks:
        const i32 debris_joint = static_cast<i8>(object->pad_e3c[1]);
        if (debris_joint != -1)
            AddGameDebris(WORLD->debris_sys, (object->movement_runtime_flags & 1) != 0 ? 139 : 138,
                          NUMTX_GET_ROW_VEC(&object->joint_matrices[debris_joint], 3));
        if (object->character_context == 38) {
            CHARACTERANIM_s *animation =
                static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[object->context_animation]);
            if (animation != NULL && static_cast<i8>(animation->locator) != -1 &&
                ((object->context_flags & 0x40) == 0 || static_cast<i8>(object->pad_e3c[1]) != -1)) {
                points[0] = points[1] =
                    *NUMTX_GET_ROW_VEC(&object->joint_matrices[static_cast<i8>(animation->locator)], 3);
                points[0].y += 0.075f;
                points[1].y -= 0.075f;
                AddStreakPoints(points, 0.25f, 0xffffffff, &object->sabre_streaks[0][0], 0, object);
            }
        }
        if (object->slam_debris_effect != -1)
            AddSlamDebris(object);
        if (object->pad_e3c[0] != 0)
            Batarang_Release(object, object->pad_e3c[0]);
        if (static_cast<i8>(object->field_0xe22) < 0)
            SuperCarry_Throw(object, 0);
        if ((object->field_0xe20 & 0x10) != 0) {
            i32 joint =
                drawn ? static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->rocket_locator
                      : -1;
            if (drawn && joint != -1 && object->apiobj.character_model->points_of_interest[joint] != NULL)
                matrix = object->joint_matrices[joint];
            else {
                NuMtxSetRotationX(&matrix, 0x1555);
                NuMtxRotateY(&matrix, object->apiobj.field_0x276 + 0x8000);
                *NUMTX_GET_ROW_VEC(&matrix, 3) = object->apiobj.collision_position;
            }
            NUVEC velocity = {0.0f, 0.0f, -1.0f};
            NuVecMtxRotate(&velocity, &velocity, &matrix);
            part = Default_ADDPART;
            part.matrix = &matrix;
            part.velocity = &velocity;
            part.field_14 = 0.1f;
            part.field_18 = 0.1f;
            part.special = &WORLD->lev_objs[232].special;
            part.field_28 = 232;
            const bool player = object->apiobj.player_controlled;
            if (player) {
                part.flags = 0xc11b;
            } else {
                part.flags = 0xc31b;
            }
            part.owner = object;
            part.field_90 = static_cast<u8>(object->apiobj.field_0x289);
            part.field_44 = BobaRocket_Kill;
            part.move_fn = BobaRocket_Move;
            part.update_fn = BobaRocket_Deflect;
            part.field_40 = PartCollide_3D;
            part.time_step = FRAMETIME;
            part.gravity = 0.0f;
            if (player) {
                part.field_a4 = 4.0f + 2.0f * (static_cast<f32>(qrand()) * (1.0f / 65535.0f));
            } else {
                part.field_a4 = 10.0f;
            }
            NUVEC direction;
            BoltSys->shoot_direction(object, &direction);
            f32 range = part.field_a4 * rocket_speed;
            part.recipient = TargetGameObject(object, &object->apiobj.collision_position, &direction, range,
                                              range * range, 0, 1, 0, -1);
            PART_s *created = AddPart(&part);
            if (created != NULL)
                created->force_flags = ObjHitObj_Flags(object);
        }
        if ((object->movement_runtime_flags & 0x40) != 0)
            ThermalDetonator_Throw(object);
        if ((object->field_0xe23 & 2) != 0 && object->field_0x780 != NULL) {
            // The zap origin callback may change the object's current target.
            GameObject_s *target = static_cast<GameObject_s *>(object->field_0x780);
            NUVEC *origin = GetZapOrigin(object);
            NUVEC delta;
            f32 distance = NuVecDist(&target->apiobj.collision_position, origin, &delta);
            NuLgtLaser(testlaser_type, testlaser_sizew, testlaser_sizel, testlaser_sizewab, origin, &delta,
                       object->id == id_R2Q5 ? 0xff202080 : 0xff808040, testlaser_endw, distance);
        }
        if (drawn && (object->apiobj.character_data->model_flags & 8) != 0 && object->weapon_scale == 1.0f) {
            for (i32 blade = 0; blade < 4; ++blade) {
                if (object->apiobj.field_0x288 == 0 || (object->field_0xe23 & 8) == 0)
                    continue;
                GAMECHARACTERDATA *blade_data =
                    static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                i32 joint_a = blade_data->streak_joints[blade][0];
                if (joint_a == -1 || object->apiobj.character_model->points_of_interest[joint_a] == NULL)
                    continue;
                i32 joint_b = blade_data->streak_joints[blade][1];
                if (joint_b == -1 || object->apiobj.character_model->points_of_interest[joint_b] == NULL)
                    continue;
                points[0] = *NUMTX_GET_ROW_VEC(&object->joint_matrices[joint_a], 3);
                points[1] = *NUMTX_GET_ROW_VEC(&object->joint_matrices[joint_b], 3);
                i32 colour = object->blade_states[blade];
                if (colour != -1 && object->character_context == 5 && (object->context_flags & 0x1c) == 0 &&
                    (static_cast<u8>(object->combo_branch) & 0xfb) == 2) {
                    i32 index = object->combo_branch == 6 ? BladeTab[colour].hit_effect : BladeTab[colour].effect_6;
                    i32 effect = WORLD->debris_sys->entries[index].effect;
                    if (effect != -1) {
                        i32 count = ParticlesPerSecond(100.0f, FRAMETIME);
                        if (count > 0) {
                            f32 fraction = static_cast<f32>(qrand()) * (1.0f / 65535.0f);
                            NUVEC position = {(points[1].x - points[0].x) * fraction + points[0].x,
                                              (points[1].y - points[0].y) * fraction + points[0].y,
                                              (points[1].z - points[0].z) * fraction + points[0].z};
                            AddVariableShotDebrisEffect(effect, &position, count, 0, 0);
                        }
                    }
                }
                if (!object->apiobj.player_controlled)
                    continue;
                bool hit = false;
                for (i32 side = 0; side < 2; ++side) {
                    NUVEC delta;
                    NuVecSub(&delta, &points[1 - side], &points[side]);
                    if (GameRayCast(&points[side], &delta, 0.0f, TERRAINMASK_NONWEAPON | 0x1f) != 0) {
                        hit = true;
                        f32 fraction = NewRayCastGetTOFI();
                        NUVEC position = {(points[1 - side].x - points[side].x) * fraction + points[side].x,
                                          (points[1 - side].y - points[side].y) * fraction + points[side].y,
                                          (points[1 - side].z - points[side].z) * fraction + points[side].z};
                        i32 index;
                        if (object->id == id_GRIEVOUS && static_cast<u32>(blade - 1) <= 1 &&
                            (object->apiobj.field_0x27c == -1 ||
                             (Cheat_IsOn(25) == 0 &&
                              (object->apiobj.field_0x27c == -1 || !Player_HasPurpleForce(object)))))
                            index = 65;
                        else
                            index = object->blade_index == -1 ? -1 : BladeTab[object->blade_index].effect_8;
                        if (index != -1) {
                            i32 effect = WORLD->debris_sys->entries[index].effect;
                            if (effect != -1) {
                                i32 count = ParticlesPerSecond(50.0f, FRAMETIME);
                                if (count > 0)
                                    AddVariableShotDebrisEffect(effect, &position, count, 0, 0);
                            }
                        }
                    }
                }
                if (hit && sabrerubwait <= 0.0f && object->apiobj.anim_packet.blending == 0 &&
                    object->character_context != 5 && object->character_context != 16 &&
                    object->character_context != 13 && object->character_context != 14 &&
                    (CInfo[object->character_context].flags & 0x4000000) == 0 &&
                    ((CInfo[object->character_context].flags & 0x8000000) == 0 || (object->jump_flags & 2) == 0)) {
                    PlaySabreSfx(const_cast<char *>("SaberSparks"), object, &points[blade], 1);
                    sabrerubwait = 0.3f + 0.3f * (static_cast<f32>(qrand()) * (1.0f / 65535.0f));
                    if (GetMenuID() == -1)
                        NewRumble(object->pad_gamepad->pad,
                                  0.2f + 0.2f * (static_cast<f32>(qrand()) * (1.0f / 65535.0f)), 0);
                }
            }
        }
        if (drawn && object->id == id_LANDSPEEDER) {
            f32 ratio = object->pad_gamepad->input_magnitude /
                        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->movement_speed;
            i32 effect = WORLD->debris_sys->entries[105].effect;
            if (effect != -1)
                AddVariableShotDebrisEffectTimed1(effect, &object->apiobj.lower_position, 60, FRAMETIME, 0, 0, NULL);
            effect = WORLD->debris_sys->entries[104].effect;
            if (effect != -1) {
                if (object->apiobj.character_model->points_of_interest[2] != NULL &&
                    static_cast<i32>(60.0f * ratio) > 0)
                    AddVariableShotDebrisEffectTimed1(effect, NUMTX_GET_ROW_VEC(&object->joint_matrices[2], 3),
                                                      static_cast<i32>(60.0f * ratio), FRAMETIME, 0, 0, NULL);
                if (object->apiobj.character_model->points_of_interest[3] != NULL &&
                    static_cast<i32>(60.0f * ratio) > 0)
                    AddVariableShotDebrisEffectTimed1(effect, NUMTX_GET_ROW_VEC(&object->joint_matrices[3], 3),
                                                      static_cast<i32>(60.0f * ratio), FRAMETIME, 0, 0, NULL);
                if (object->apiobj.character_model->points_of_interest[4] != NULL &&
                    static_cast<i32>(60.0f * ratio) > 0)
                    AddVariableShotDebrisEffectTimed1(effect, NUMTX_GET_ROW_VEC(&object->joint_matrices[4], 3),
                                                      static_cast<i32>(60.0f * ratio), FRAMETIME, 0, 0, NULL);
            }
            effect = WORLD->debris_sys->entries[103].effect;
            if (effect != -1 && object->apiobj.field_0x218 != 2000000.0f) {
                if (object->apiobj.character_model->points_of_interest[2] != NULL) {
                    f32 height = object->joint_matrices[2].m31 - object->apiobj.field_0x218;
                    if (height > 0.0f && height <= 0.2f) {
                        f32 amount = 25.0f;
                        if (height >= 0.1f)
                            amount = (1.0f - (height - 0.1f) / 0.1f) * 25.0f;
                        if (static_cast<i32>(amount * ratio) > 0) {
                            NUVEC position = {object->joint_matrices[2].m30, object->apiobj.field_0x218,
                                              object->joint_matrices[2].m32};
                            AddVariableShotDebrisEffectTimed1(effect, &position, 60, FRAMETIME, 0, 0, NULL);
                        }
                    }
                }
                if (object->apiobj.character_model->points_of_interest[3] != NULL) {
                    f32 height = object->joint_matrices[3].m31 - object->apiobj.field_0x218;
                    if (height > 0.0f && height <= 0.2f) {
                        f32 amount = 25.0f;
                        if (height >= 0.1f)
                            amount = (1.0f - (height - 0.1f) / 0.1f) * 25.0f;
                        if (static_cast<i32>(amount * ratio) > 0) {
                            NUVEC position = {object->joint_matrices[3].m30, object->apiobj.field_0x218,
                                              object->joint_matrices[3].m32};
                            AddVariableShotDebrisEffectTimed1(effect, &position, 60, FRAMETIME, 0, 0, NULL);
                        }
                    }
                }
                if (object->apiobj.character_model->points_of_interest[4] != NULL) {
                    f32 height = object->joint_matrices[4].m31 - object->apiobj.field_0x218;
                    if (height > 0.0f && height <= 0.2f) {
                        f32 amount = 25.0f;
                        if (height >= 0.1f)
                            amount = (1.0f - (height - 0.1f) / 0.1f) * 25.0f;
                        if (static_cast<i32>(ratio * amount) > 0) {
                            NUVEC position = {object->joint_matrices[4].m30, object->apiobj.field_0x218,
                                              object->joint_matrices[4].m32};
                            AddVariableShotDebrisEffectTimed1(effect, &position, 60, FRAMETIME, 0, 0, NULL);
                        }
                    }
                }
            }
        }
        if (VehicleArea != 0 && static_cast<u8>(object->apiobj.field_0x27c) <= 1) {
            i32 *key = &FalconDebKey[static_cast<u8>(object->apiobj.field_0x27c)];
            if (object->id == id_MILLENNIUMFALCON && object->apiobj.player_controlled &&
                (object->field_0xe24 & 8) != 0 && object->apiobj.character_model->points_of_interest[2] != NULL &&
                object->apiobj.field_0xa8 == 1.0f) {
                if (*key == -1)
                    AddDebrisEffect(key, WORLD->debris_sys->entries[136].effect, 0.0f, 0.0f, 0.0f);
                else {
                    matrix = object->joint_matrices[2];
                    NuMtxPreRotateX(&matrix, 0x4000);
                    DebrisPosOrientationMtx(*key, &matrix);
                }
            } else
                DebFreeInstantly(key);
        }
        if (object->field_0xdec > 0.0f && object->apiobj.field_0x287 == 0) {
            f32 radius = object->apiobj.field_0x1dc * (0.5f + 0.5f * (static_cast<f32>(qrand()) * (1.0f / 65535.0f)));
            u16 angle = qrand();
            NUVEC position = {object->apiobj.position.x + radius * NuTrigTable[angle >> 1],
                              object->apiobj.position.y + object->field_0xffc * object->apiobj.field_0xa8 + 0.05f,
                              object->apiobj.position.z + radius * NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff]};
            PowerUp_Particles(WORLD, &position);
        }
        count = HIGHGAMEOBJECT;
    }
}

i32 ZapTarget(GameObject_s *object) {
    if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001 || object->apiobj.field_0x287 != 0)
        return 0;
    u32 flags = object->apiobj.character_data->model_flags;
    if ((flags & 0x10) == 0)
        return 0;
    void **animations = object->apiobj.character_model->model_data_b;
    if (animations[0x41] == NULL)
        return 0;
    i8 context = object->character_context;
    if ((CInfo[context].flags & 0x8000) != 0)
        return 0;
    if ((flags & 0x20) != 0 && (animations[0x42] == NULL || animations[0x43] == NULL || animations[0x44] == NULL))
        return 0;
    if (context == 0x15)
        return 0;
    if (context == 0x17)
        return 0;
    if (context == 0x41)
        return 0;
    if (context == 0x33)
        return 0;
    if (context == 0x3b)
        return 0;
    return context != 0x39;
}

static void ZapCode(GameObject_s *object, i32 pressed, i32 retract_weapon) {
    if (object->character_context != 0x16) {
        if (pressed != 0 && (object->apiobj.character_data->game_character->flags_098[0] & 1) != 0) {
            i16 animation;
            if (object->field_0xe31 == 1) {
                if (object->character_context != -1) {
                    return;
                }
                animation = 0x3c;
                if (object->apiobj.character_model->model_data_b[animation] == NULL) {
                    return;
                }
            } else {
                animation = 0x16;
                if (object->apiobj.character_model->model_data_b[animation] == NULL) {
                    return;
                }
                if (object->apiobj.field_0x27d == 0 || ObjLandReady(object) == 0) {
                    return;
                }
            }

            NUVEC forward;
            object->force_target = NULL;
            NuVecRotateY(&forward, &v001, object->apiobj.movement_facing_angle);
            f32 nearest_distance = 0.36f;
            for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
                GameObject_s *candidate = &Obj[index];
                if (candidate == object) {
                    continue;
                }
                if (ZapTarget(candidate) == 0 &&
                    !(static_cast<i8>(object->apiobj.flags_low) < 0 &&
                      (candidate->apiobj.field_0x1f8 & 0x1001) == 0x1001 && candidate->apiobj.field_0x287 == 0 &&
                      candidate->character_context != 0x17 &&
                      (((object->apiobj.character_data->model_flags & 0x40) != 0 &&
                        candidate->apiobj.character_data->game_character->uses_weapon_action == 1) ||
                       (Cheat_IsOn(0x26) != 0 && candidate->apiobj.character_model->model_data_b[0x41] != NULL)))) {
                    continue;
                }
                NUVEC delta;
                const f32 distance =
                    NuVecDistSqr(&candidate->apiobj.collision_position, &object->apiobj.collision_position, &delta);
                if (distance < nearest_distance && delta.x * forward.x + delta.z * forward.z > 0.0f) {
                    nearest_distance = distance;
                    object->force_target = candidate;
                }
            }

            object->character_context = 0x16;
            object->context_animation = animation;
            object->context_animation_timer = AnimDuration(object->id, animation, 0.0f, 0.0f, 1);
            object->context_flags &= ~0x40;
            SetWeaponOut(object);
            return;
        }

        if (retract_weapon != 0 && (object->id == id_JAWA || object->id == id_UGNAUGHT) &&
            (object->field_0xe22 & 1) != 0 && object->field_0xe32 == 0 && object->character_context != 6 &&
            object->character_context != 7) {
            if (object->apiobj.field_0x27d != 0 && object->pad_gamepad->input_magnitude == 0.0f) {
                i32 animation = 0x40;
                if (object->apiobj.character_data->game_character->uses_weapon_action == 0 &&
                    (object->apiobj.character_data->model_flags & 0x80) != 0) {
                    animation = 0x1f8;
                }
                if (object->apiobj.character_model->model_data_b[animation] != NULL &&
                    (object->character_context == -1 || (CInfo[object->character_context].flags & 4) != 0)) {
                    SlowWeaponIn(object);
                    return;
                }
            }
            FastWeaponIn(object, 1);
        }
        return;
    }

    f32 *frame = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
    if (frame == NULL) {
        return;
    }

    GameObject_s *target = object->force_target;
    NUVEC *origin = GetZapOrigin(object);
    if ((object->context_flags & 0x40) == 0) {
        const f32 hit_frame = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
        if (hit_frame < 1.0f || hit_frame <= *frame) {
            object->context_flags |= 0x40;
        }
        if ((object->context_flags & 0x40) != 0) {
            AddGameDebris(WORLD->debris_sys, object->id == id_R2Q5 ? 1 : 3, origin);
            NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
            if (object->apiobj.character_data->game_character->sfx_shoot == -1)
                GameAudio_PlaySfxById(GetSfxId("R2Zap"), &object->apiobj.collision_position,
                                      GameAudio_GetPlrSfxBits(object), 1);
            else
                GameAudio_PlaySfxById(object->apiobj.character_data->game_character->sfx_shoot,
                                      &object->apiobj.collision_position, GameAudio_GetPlrSfxBits(object), 1);

            if (target == NULL) {
                AddGameDebris(WORLD->debris_sys, 0x58, origin);
            } else {
                const u32 angle = NuAtan2D(object->apiobj.collision_position.x - target->apiobj.collision_position.x,
                                           object->apiobj.collision_position.z - target->apiobj.collision_position.z);
                NUVEC hit_position = {
                    NuTrigTable[(angle >> 1) & 0x7fff] * target->apiobj.collision_radius +
                        target->apiobj.collision_position.x,
                    target->apiobj.collision_position.y,
                    NuTrigTable[(((angle & 0xffff) + 0x4000) >> 1) & 0x7fff] * target->apiobj.collision_radius +
                        target->apiobj.collision_position.z,
                };
                AddGameDebris(WORLD->debris_sys, object->id == id_R2Q5 ? 1 : 3, &hit_position);

                i32 deactivated;
                if (static_cast<i8>(object->apiobj.flags_low) < 0 && Cheat_IsOn(0x26) != 0 &&
                    (target->apiobj.character_data->model_flags & 0x10) != 0) {
                    objhitobj_noimpactsfx = 1;
                    if (ObjHitObj(object, target, -1, 0x401, 0, 1) == 2) {
                        deactivated = 0;
                    } else {
                        deactivated = DeactivatePlayer(target, DEACTIVATEDTIME, NULL);
                    }
                } else {
                    deactivated = DeactivatePlayer(object->force_target, DEACTIVATEDTIME, object);
                }
                if (deactivated != 0 && object->apiobj.player_controlled) {
                    GameCam_HitJudder();
                    if (Cheat_IsOn(0x15) != 0 && (target->apiobj.character_data->model_flags & 0x10) != 0 &&
                        (target->apiobj.character_data->game_character->flags_090 & 0x40) == 0) {
                        target->action_movement_state = 2;
                        const f32 duration = static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 0.3f + 0.2f;
                        target->field_0x768 = duration;
                        target->context_variant_flags =
                            (target->context_variant_flags & ~1) | (object->id == id_R2Q5 ? 1 : 0);
                    }
                }
            }
        }
    }

    if (target != NULL) {
        const f32 animation_end =
            NuAnimEndFrame(object->apiobj.character_model->model_data_b[object->context_animation]);
        const f32 lightning_start = AnimListFrame(object->apiobj.character_model, object->context_animation, 1);
        if (lightning_start >= 1.0f && lightning_start < animation_end) {
            f32 lightning_end = AnimListFrame(object->apiobj.character_model, object->context_animation, 2);
            if (lightning_start < lightning_end) {
                if (lightning_end > animation_end) {
                    lightning_end = animation_end;
                }
                if (lightning_start <= *frame && *frame <= lightning_end) {
                    object->field_0xe23 |= 2;
                    PlaySfx("ForceLightningLp", &target->apiobj.collision_position);
                    if (object->dynamic_light_id != -1) {
                        rtlDynamicEnable(object->dynamic_light_id, 1);
                        NUVEC direction;
                        const f32 distance = NuVecDist(&target->apiobj.collision_position,
                                                       &object->apiobj.collision_position, &direction);
                        rtlDynamicSetRadii(object->dynamic_light_id, distance * 0.5f, distance * 0.5f + 0.5f);
                        qrand();
                        NUVEC colour;
                        if (object->id == id_R2Q5) {
                            colour.x = qrand() < 0x8000 ? 2.0f : 0.25f;
                            colour.y = 0.0f;
                            colour.z = 0.0f;
                        } else {
                            colour.x = 0.0f;
                            colour.y = qrand() < 0x8000 ? 2.0f : 0.25f;
                            colour.z = colour.y;
                        }
                        rtlDynamicSetColours(object->dynamic_light_id, &colour, NULL);
                        NUVEC midpoint = {
                            object->apiobj.collision_position.x + direction.x * 0.5f,
                            object->apiobj.collision_position.y + direction.y * 0.5f,
                            object->apiobj.collision_position.z + direction.z * 0.5f,
                        };
                        rtlDynamicSetPos(object->dynamic_light_id, &midpoint);
                    }
                }
            }
        }
    }

    SetObjAsHeadTarget(object, target, 2, 1.0f, 0.0f, 0.0f);
    object->context_animation_timer -= FRAMETIME;
    if (object->context_animation_timer <= 0.0f) {
        object->character_context = -1;
    }
}

static void CommunicateCode(GameObject_s *object, i32 pressed) {
    i8 context = object->character_context;
    if (context != 0x1a) {
        if (pressed == 0)
            return;
        if (context != -1)
            return;
        if (static_cast<i8>(
                static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_094[0]) >= 0)
            return;
        if (object->apiobj.character_model->model_data_b[0x6b] == NULL)
            return;
        object->context_animation_timer = 0.0f;
        object->character_context = 0x1a;
        object->context_animation = 0x6b;
        object->airborne_action_duration = AnimDuration(object->id, 0x6b, 0.0f, 0.0f, 1);
        return;
    }
    f32 *frame = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
    if (frame == NULL)
        return;
    f32 *events = AnimListFrameArray(object->apiobj.character_model, object->context_animation);
    if (events != NULL && events[0] >= 1.0f && events[1] >= 1.0f && events[2] >= 1.0f && events[3] >= 1.0f) {
        f32 time = *frame;
        if (!(events[0] >= time)) {
            if (time < events[1])
                object->communicate_blend = (time - events[0]) / (events[1] - events[0]);
            else if (time < events[2])
                object->communicate_blend = 1.0f;
            else if (time < events[3])
                object->communicate_blend = 1.0f - (time - events[2]) / (events[3] - events[2]);
            else
                object->communicate_blend = 0.0f;
        } else
            object->communicate_blend = 0.0f;
    }
    object->context_animation_timer += FRAMETIME;
    if (object->context_animation_timer >= object->airborne_action_duration) {
        object->character_context = -1;
        object->communicate_blend = 0.0f;
        if (object->apiobj.player_controlled && Cheat_IsOn(10)) {
            GameObject_s *target = Obj;
            for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++target) {
                if (ZapTarget(target) && object != target && target->apiobj.field_0x27c == -1 &&
                    (target->field_0xefb & 8) == 0 && !CannotKill(target)) {
                    u32 flags =
                        static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24)->flags_090;
                    if ((flags & 0x40) != 0)
                        continue;
                    i8 context = target->character_context;
                    if (context == 0xf)
                        continue;
                    if (context == 0x3c)
                        continue;
                    if (context == 0x47)
                        continue;
                    if (context == 0x46)
                        continue;
                    if ((flags & 0x8000) == 0 &&
                        NuVecDistSqr(&object->apiobj.collision_position, &target->apiobj.collision_position, NULL) <
                            9.0f &&
                        DeactivatePlayer(target, DEACTIVATEDTIME, object)) {
                        PlaySfx("R2Zap", &target->apiobj.collision_position);
                        AddGameDebris(WORLD->debris_sys, 1, &target->apiobj.collision_position);
                        GameCam_HitJudder();
                        target->action_movement_state = 2;
                        f32 random = static_cast<f32>(qrand()) * (1.0f / 65535.0f);
                        target->context_variant_flags |= 1;
                        target->field_0x768 = random * 0.3f + 0.2f;
                        return;
                    }
                }
            }
        }
    }
}

bool FireBountyHunterRocket(GameObject_s *object) {
    if (object->character_context != -1)
        return false;
    characterdata_s *data = object->apiobj.character_data;
    if ((data->model_flags & 0x01000000) == 0 && object->field_0x108e != 6)
        return false;
    i32 locator = data->game_character->rocket_locator;
    if (locator == -1 || object->apiobj.character_model->points_of_interest[locator] == NULL)
        return false;
    if (object->apiobj.field_0x27c != -1) {
        if (!object->apiobj.player_controlled)
            return false;
        if (!Cheat_IsOn(39))
            return false;
    }
    if (object->timer_d5c == 2.0f) {
        object->timer_d5c = 0.0f;
        FindGameMsgsWithID(2, 1, -1, NULL);
    }
    const i32 animation = object->apiobj.field_0x27d == 0 ? 77 : 2;
    object->context_animation = object->apiobj.field_0x27d == 0 ? 77 : 2;
    object->context_animation_timer = AnimDuration(object->id, animation, 0.0f, 0.0f, 1);
    object->field_0xe22 &= ~4;
    if (object->context_animation_timer > 0.0f) {
        object->character_context = 20;
    }
    return object->character_context == 20;
}

void GetShootOrigin_LSW(GameObject_s *object, nuvec_s *position) {
    *position = object->apiobj.collision_position;
    if (object->id == id_ATAT) {
        u16 angle = object->apiobj.field_0x276;
        f32 scale = object->apiobj.field_0x1dc;
        position->x += (NU_SIN_LUT(angle) * scale) * 1.5f;
        position->z += (scale * NU_COS_LUT(angle)) * 1.5f;
    }
}

void CableCode(GameObject_s *object, i32 special_pressed, f32) {
    if ((object->apiobj.flags_low & 0x80) != 0 && object->character_context == -1 && object->cable == NULL) {
        GameObject_s *target =
            CableTargetGameObject(object, &object->apiobj.collision_position, max_cable_length * 0.9f);
        if (target != NULL) {
            object->field_0xe24 |= 0x10;
            if (special_pressed == 0 && !TouchHacks::ShouldAutoGrabDragBomb(*target)) {
                object->force_glow_candidate = target;
                object->force_glow_candidate_kind = 2;
            } else {
                MechAddonCollection *addons = target->GetAddons(false);
                if (addons != NULL) {
                    if (addons->Find(CantPickupBombTimerAddon::s_hashId) == NULL) {
                        object->cable = CreateCable(object, target, 1);
                        if (target->id == id_DRAGBOMB) {
                            Hint_SetComplete(0x5ee);
                        } else if (special_pressed != 0) {
                            Hint_SetComplete(0x28e);
                        }
                    }
                }
            }
        }
    }

    if (object->apiobj.field_0x287 == 0 && FadeSys.fade == 0.0f) {
        if (object->force_glow_step <= 0.0f) {
            ForceGlowCode(object, 0xe3);
        } else {
            object->force_glow_step -= FRAMETIME;
        }
    } else {
        ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    }

    if (object->apiobj.player_controlled && object->force_glow_candidate_kind != 0 &&
        object->force_glow_candidate != NULL && object->field_0xd80 > 0.0f) {
        GameObject_s *target = static_cast<GameObject_s *>(object->force_glow_candidate);
        GAMEMESSAGE_s *message = static_cast<GAMEMESSAGE_s *>(
            AddGameMessage(const_cast<char *>("X"), &target->apiobj.collision_position,
                           AreaPickupScale + AreaPickupScale, NULL, 0.0f, 0xff, 0, 0, 0x10c7, 0.0f));
        if (message != NULL) {
            message->target_type = 5;
            message->draw_callback = DrawGameMessage_Targets;
            message->alpha = static_cast<u8>(object->field_0xd80 / FORCEGLOWTIME * 128.0f);
        }
    }
}

i32 DragBomb_UpdateHint(HINT_s *hint) {
    if (VehicleArea == 0 || player == NULL)
        return 0;
    if ((player->apiobj.character_data->player_config->flags_090 & 0x400) == 0 ||
        player->force_glow_candidate_kind == 0 || player->force_glow_candidate == NULL || !(player->field_0xd80 > 0.0f))
        return 0;
    const i32 id = static_cast<GameObject_s *>(player->force_glow_candidate)->id;
    if (hint->control_mode_ids[0] != 0x5ee) {
        if (hint->control_mode_ids[0] == 0x28e) {
            if (id != id_DRAGBOMB)
                return 1;
        }
    } else {
        if (id == id_DRAGBOMB)
            return 1;
    }
    return 0;
}

i32 CanObjSlide(GameObject_s *object, i32) {
    const i8 surface = object->apiobj.field_0x281;
    if (static_cast<u8>(surface) >= 32)
        return 0;
    if ((TerSurface[surface].flags & 0x400) != 0) {
        if (surface == 5 && CanMagnetClimbFn != NULL) {
            if (CanMagnetClimbFn(object) == 0)
                return 1;
        } else {
            return 1;
        }
    }
    return 0;
}

i32 StartSlide(GameObject_s *object, i32 check_contact) {
    if (VehicleArea || object->ignore_slide_terrain || object->field_0x7a5 == 0x2b || object->field_0x7a5 == 0x1f ||
        object->id == id_ATST || object->id == id_MINIATST || object->id == id_ATST_LOWRES || object->id == id_ATAT ||
        object->id == id_MINIATAT || object->id == id_MINIATTE)
        return 0;
    i32 can_slide = CanObjSlide(object, static_cast<i8>(object->apiobj.field_0x281));
    if (check_contact && object->apiobj.field_0x27d == 0) {
        if (object->field_0x1084 == 0 || can_slide == 0 ||
            !(object->apiobj.collision_position.y > object->contact_position.y))
            return 0;
        can_slide = CanObjSlide(object, static_cast<i8>(object->field_0x6b0));
    }
    if (can_slide == 0)
        return 0;
    object->field_0x7a5 = 0x33;
    object->context_animation = object->apiobj.character_model->model_data_b[106] != NULL ? 106 : 5;
    object->field_0xe31 = 0;
    object->airborne_action_duration = 0.25f;
    return 1;
}

void SlideCode(GameObject_s *object) {
    if (object->field_0x7a5 == 0x33) {
        if (object->apiobj.field_0x27d != 0 && CanObjSlide(object, static_cast<i8>(object->apiobj.field_0x281)) != 0) {
            object->airborne_action_duration = 0.25f;
        } else if (object->airborne_action_duration > 0.0f) {
            object->airborne_action_duration -= FRAMETIME;
            if (object->airborne_action_duration <= 0.0f) {
                object->field_0x7a5 = -1;
            }
        }

        if (object->apiobj.field_0x27d != 0) {
            PlaySfx("Char_Slide_Lp", &object->apiobj.lower_position);
        }
    } else {
        StartSlide(object, 1);
    }
}

// Original 0x166f30, 403 bytes.
i32 Slide_SetTargetMom(GameObject_s *object, u16 input_angle, float input_speed) {
    u16 direction;
    if (object->apiobj.field_0x27d != 0 &&
        (fabsf(object->surface_normal.x) > 0.001f || fabsf(object->surface_normal.z) > 0.001f))
        direction = NuAtan2D(object->surface_normal.x, object->surface_normal.z);
    else
        direction = object->apiobj.movement_facing_angle;
    object->target_velocity.x = NU_SIN_LUT(direction) * 1.2f;
    object->target_velocity.z = NU_COS_LUT(direction) * 1.2f;
    if (object->apiobj.field_0x27d != 0 && input_speed > 0.0f) {
        NUVEC lateral;
        lateral.x = NU_SIN_LUT(input_angle) * input_speed;
        lateral.y = 0.0f;
        lateral.z = NU_COS_LUT(input_angle) * input_speed;
        NuVecRotateY(&lateral, &lateral, -static_cast<i32>(direction));
        lateral.z = 0.0f;
        NuVecRotateY(&lateral, &lateral, direction);
        object->target_velocity.x += lateral.x;
        object->target_velocity.z += lateral.z;
    }
    return 0;
}

void MovePlayer_DIRECTIONAL(GameObject_s *object) {
    GAMEPAD_s *pad = object->pad_gamepad;
    GameObject_s *operator_object = object;
    const i32 current_animation = CurrentAnim(&object->apiobj.anim_packet);
    Techno_FindOperator(object, &pad, &operator_object);
    GAMECHARACTERDATA *game_character = object->apiobj.character_data->game_character;
    const f32 run_speed = game_character->run_speed;
    const f32 input_speed = pad->input_magnitude;
    const f32 operator_run_speed = operator_object->apiobj.character_data->game_character->run_speed;
    APIOBJECT &api = object->apiobj;

    // Original entry path, 0x16716f..0x16722a: attached inactive vehicles own movement.
    if ((object->field_0xe20 & 0x20) != 0 && object->character_context != 0x23 && object->character_context != 0x24) {
        GameObject_s *vehicle;
        MoveInactiveVehicle(object, 0, &vehicle);
        if (vehicle != NULL) {
            object->field_0xdc8 = vehicle->field_0xdc8;
            api.facing_angle = api.movement_facing_angle = api.field_0x276 = vehicle->apiobj.field_0x276;
            api.velocity = vehicle->apiobj.velocity;
            object->movement_lean_angle = 0;
            object->tertiary_lean_angle = 0;
            object->secondary_lean_angle = 0;
            api.roll_angle = 0;
            api.pitch_angle = 0;
        }
        return;
    }

    object->field_0xe23 &= ~0x10;
    if (MovingBackwards(object) != 0) {
        object->field_0xefd |= GAMEOBJECT_MOVEMENT_FLAG_BACKWARDS;
    } else if (__builtin_expect(static_cast<i8>(object->field_0xefd) < 0, 1)) {
        object->field_0xefd |= GAMEOBJECT_MOVEMENT_FLAG_BACKWARDS;
    }
    if (object->character_context == 0x35) {
        if (api.field_0x27d != 0)
            object->character_context = -1;
        else
            PlaySfx("GrapWindLp", &api.collision_position);
    }
    const u16 input_angle = GamePad_InputAngle(object, pad);
    object->field_0xe22 |= GAMEOBJECT_E22_FLAG_INPUT_ANGLE_VALID;
    object->current_input_angle = input_angle;

    if (object->delayed_turn_timer > 0.0f) {
        if (pad->input_magnitude > 0.0f) {
            object->delayed_turn_target_angle = input_angle;
        }
        object->delayed_turn_timer -= FRAMETIME;
        if (object->delayed_turn_timer <= 0.0f) {
            api.movement_facing_angle = object->delayed_turn_target_angle;
        }
    } else {
        const bool has_turn_animation = api.character_model != NULL && (api.character_model->model_data_b[12] != NULL ||
                                                                        api.character_model->model_data_b[119] != NULL);
        if (has_turn_animation && (object->character_context == 1 || object->character_context == -1) &&
            pad->input_magnitude > 0.0f) {
            i32 difference = RotDiff(api.movement_facing_angle, input_angle);
            if (difference < 0)
                difference = -difference;
            if (difference > 0x6aaa) {
                object->delayed_turn_timer = 0.2f;
                object->delayed_turn_target_angle = input_angle;
            }
        }
    }

    f32 desired_speed = run_speed;
    desired_speed *= input_speed / operator_run_speed;
    if (object->character_context == 0x33 && (api.character_data->game_character->flags_090 & 0x200) != 0) {
        object->field_0x1086 = 2;
    } else if ((object->movement_context_state & 0xffff00) == 0x54300) {
        Climb_UpdateMagnetRotation(object);
    } else if ((api.character_data->model_flags & 0x100100) == 0x100000) {
        if (object->action_movement_state == 1) {
            api.pitch_angle = 0;
            api.roll_angle = 0;
        } else {
            object->field_0x1086 = 0;
            u16 pitch = 0;
            if (object->character_context == 0x47) {
                pitch = object->magnet_surface_angle;
                ZIPUP *zipup = static_cast<ZIPUP *>(object->field_0x788);
                if (zipup != NULL && (zipup->flags & 1) != 0) {
                    pitch -= static_cast<u16>(static_cast<i32>(static_cast<f32>(zipup->pitch_adjustment) *
                                                               (object->context_animation_timer / 1.5f)));
                }
            }
            api.pitch_angle = SeekRot(api.pitch_angle, pitch, 8.0f);
            api.roll_angle = SeekRot(api.roll_angle, 0, 8.0f);
        }
    } else if (object->id == id_GRABCONTROL || object->id == id_GRABR2CONTROL) {
        object->field_0x1086 = 2;
        u16 pitch = 0;
        u16 roll = 0;
        if (pad->input_magnitude > 0.0f) {
            NUVEC direction;
            NuVecRotateX(&direction, &v010,
                         static_cast<i32>((desired_speed / api.character_data->game_character->run_speed) * 2730.0f));
            NuVecRotateY(&direction, &direction, input_angle);
            FindAnglesZX(&direction, &pitch, &roll);
        }
        api.pitch_angle = SeekRot(api.pitch_angle, pitch, 8.0f);
        api.roll_angle = SeekRot(api.roll_angle, roll, 8.0f);
    }
    f32 turn_override = 0.0f;
    i32 heading_handled = 0;
    const u32 context_flags = CInfo[object->character_context].flags;
    if ((context_flags & 0x40) != 0) {
        if (api.velocity.x != 0.0f || api.velocity.z != 0.0f)
            api.movement_facing_angle = NuAtan2D(api.velocity.x, api.velocity.z);
    } else if ((context_flags & 0x4000) != 0) {
        api.movement_facing_angle = static_cast<u16>(GameCam->yaw + 0x8000);
    } else if (object->id == id_GRABCONTROL || object->id == id_GRABR2CONTROL) {
        if (WORLD->grabber != NULL) {
            NUVEC *position = Grabber_GetGrabPos(WORLD->grabber, NULL);
            api.movement_facing_angle =
                NuAtan2D(position->x - api.collision_position.x, position->z - api.collision_position.z);
        }
    } else if ((api.character_data->game_character->flags_094[0] & 2) == 0 && object->character_context != 0x3d &&
               object->character_context != 0x15 && object->character_context != 0x5a &&
               object->character_context != 0x0f && object->character_context != 0x1f &&
               object->character_context != 0x3c &&
               (object->character_context != 0x17 || (object->field_0xf04 & 1) != 0) &&
               object->character_context != 0x41 && object->character_context != 0x0b &&
               (object->character_context != 0 ||
                ((object->action_movement_state != 6 && object->action_movement_state != 7 &&
                  object->action_movement_state != 9) ||
                 object->context_variant_flags < 0)) &&
               object->character_context != 0x47 &&
               (object->character_context != 0x29 || api.character_model->model_data_b[0x57] != NULL ||
                (api.character_data->game_character->flags_094[2] & 0x10) != 0) &&
               ((object->field_0xeff & 2) != 0 || CurrentAnim(&api.anim_packet) != 5) &&
               object->character_context != 0x35) {
        // Retail uses ordered context tests here. The labels also retain the
        // ordinary opponent-facing fallthrough from context 0x1b to 0x16.
        do {
            if (object->character_context == 0x46)
                goto directional_heading_0x46;
            if (__builtin_expect(
                    api.player_controlled && object->character_context == 0x1b && (object->field_0xe21 & 2) == 0, 0))
                goto directional_heading_0x1b;
            if (object->character_context == 0x1c)
                goto directional_heading_0x1c;
            if (object->character_context == 0x1b || object->character_context == 0x16)
                goto directional_heading_0x16;
            if (object->character_context == 0x22)
                goto directional_heading_0x22;
            if (object->character_context == 0x1d)
                goto directional_heading_0x1d;
            if (object->character_context == 0x12)
                goto directional_heading_0x12;
            if (object->character_context == 8)
                goto directional_heading_8;
            if (object->character_context == 0x2d)
                goto directional_heading_0x2d;
            if (object->character_context == 0x10)
                goto directional_heading_0x10;
            if (object->character_context == 0x33)
                goto directional_heading_0x33;
            if (object->character_context == 5)
                goto directional_heading_5;
            if (object->character_context == 0x26)
                goto directional_heading_0x26;
            if (object->character_context == 0x58)
                goto directional_heading_0x58;
            if (object->character_context == 0x0a)
                goto directional_heading_0x0a;
            if (object->character_context == 0x0c)
                goto directional_heading_0x0c;
            if (object->character_context == 0x18)
                goto directional_heading_0x18;
            if (object->character_context == 0x25)
                goto directional_heading_0x25;
            if (object->character_context == 0)
                goto directional_heading_0;
            goto directional_common_heading;
        directional_heading_0:
            if ((CInfo[object->character_context].flags & 0x2000) == 0 && object->action_movement_state == 3) {
                FaceOpponent(object, NULL);
                break;
            }
            goto directional_common_heading;
        directional_heading_0x46:
            if (object->field_0x7a3 == 1 || GrappleSwingMode == 1)
                break;
            goto directional_common_heading;
        directional_heading_0x0a:
            if ((CInfo[object->character_context].flags & 0x2000) != 0 ||
                static_cast<u16>(object->context_animation - 0x5a) > 2)
                goto directional_common_heading;
            api.movement_facing_angle = NuAtan2D(object->attack_target_position.x - api.position.x,
                                                 object->attack_target_position.z - api.position.z);
            if (object->context_animation == 0x5a)
                api.movement_facing_angle += 0x4000;
            else if (object->context_animation == 0x5b)
                api.movement_facing_angle -= 0x4000;
            else
                api.movement_facing_angle += 0x8000;
            break;
        directional_heading_0x18: {
            if ((CInfo[object->character_context].flags & 0x2000) != 0)
                goto directional_common_heading;
            NUVEC *position = NULL;
            if (object->incoming_bolt != NULL) {
                if (object->incoming_bolt->active != 0)
                    position = &object->incoming_bolt->position;
            } else if (object->incoming_melee != NULL) {
                if ((object->incoming_melee->apiobj.field_0x1f8 & 0x1000) != 0 &&
                    object->incoming_melee->apiobj.field_0x287 == 0)
                    position = &object->incoming_melee->apiobj.position;
            } else if (object->incoming_part != NULL && (object->incoming_part->active & 1) != 0) {
                position = &object->incoming_part->position;
            }
            if (position != NULL)
                api.movement_facing_angle = NuAtan2D(position->x - api.position.x, position->z - api.position.z);
            break;
        }
        directional_heading_0x0c: {
            if ((CInfo[object->character_context].flags & 0x2000) != 0)
                goto directional_common_heading;
            if (object->block_latch != 0)
                break;
            NUVEC *position = NULL;
            if (object->blocked_bolt != NULL) {
                if (object->blocked_bolt->active != 0)
                    position = &object->blocked_bolt->position;
            } else if (__builtin_expect(object->block_attacker != NULL, 0)) {
                if ((object->block_attacker->apiobj.field_0x1f8 & 0x1000) != 0 &&
                    object->block_attacker->apiobj.field_0x287 == 0)
                    position = &object->block_attacker->apiobj.position;
            } else if (object->blocked_part != NULL && (object->blocked_part->active & 1) != 0) {
                position = &object->blocked_part->position;
            }
            if (position != NULL)
                api.movement_facing_angle = NuAtan2D(position->x - api.position.x, position->z - api.position.z);
            break;
        }
        directional_heading_0x10:
            if ((object->context_flags & 0x40) != 0)
                goto directional_common_heading;
            if (FaceOpponent(object, NULL) != 0)
                api.movement_facing_angle += 0x8000;
            break;
        directional_heading_0x25:
            if ((CInfo[object->character_context].flags & 0x2000) == 0 && object->context_animation != 0x58) {
                FaceOpponent(object, NULL);
                break;
            }
            // The original falls through to the shared push/input heading path.
            goto directional_common_heading;
        directional_heading_0x22:
            if (ForceBackPos != NULL)
                FaceOpponent(object, ForceBackPos);
            break;
        directional_heading_0x1d:
            if (object->force_part != NULL && (object->force_part->active & 1) != 0)
                api.movement_facing_angle = NuAtan2D(object->force_part->position.x - api.collision_position.x,
                                                     object->force_part->position.z - api.collision_position.z);
            break;
        directional_common_heading:
            if ((CInfo[object->character_context].flags & 0x2000) != 0) {
                SetPushAngle(object);
            } else if (object->context_target_position != NULL) {
                FaceOpponent(object, object->context_target_position);
                api.movement_facing_angle += 0x8000;
            } else if (pad->operator_data != NULL && (CInfo[object->character_context].flags & 1) == 0) {
                NUVEC *position = static_cast<NUVEC *>(pad->operator_data);
                api.movement_facing_angle = NuAtan2D(position->x - api.position.x, position->z - api.position.z);
            } else if (api.field_0x27c != -1 && api.field_0x27c == BonusWinner) {
                api.movement_facing_angle = NuAtan2D(GameCam->pos.x - api.position.x, GameCam->pos.z - api.position.z);
            } else if (pad->input_magnitude > 0.0f &&
                       ((CInfo[object->character_context].flags & 1) == 0 || SuperCarry_Carrying(object) ||
                        (object->character_context == 0x20 && (object->field_0xef9 & 1) != 0)) &&
                       object->delayed_turn_timer <= 0.0f && object->character_context != 0x13 &&
                       (object->field_0xf01 & 2) == 0 &&
                       (object->character_context != 0 || object->context_variant_flags < 0 ||
                        object->action_movement_state == 0 || object->action_movement_state == 5 ||
                        object->action_movement_state == 8)) {
                if ((api.flags_low & 0x80) != 0 || object->character_context != -1 || api.field_0x27d != 0 ||
                    IsAFallAnim(CurrentAnim(&api.anim_packet)) == 0)
                    api.movement_facing_angle = input_angle;
                if (object->character_context == 0x20 && (object->field_0xef9 & 1) != 0)
                    api.movement_facing_angle += 0x8000;
            }
            break;
        directional_heading_5:
            if ((object->context_flags & 0x0c) == 0)
                FaceOpponent(object, NULL);
            break;
        directional_heading_0x26:
            if (object->field_0x7a7 == -1 && FaceOpponent(object, NULL) != 0 && (object->context_flags & 0x80) != 0)
                api.movement_facing_angle += 0x8000;
            break;
        directional_heading_0x33:
            if (api.velocity.x != 0.0f || api.velocity.z != 0.0f)
                api.movement_facing_angle = NuAtan2D(api.velocity.x, api.velocity.z);
            break;
        directional_heading_0x1c:
            heading_handled = ForcePushed_YRotation(object);
            break;
        directional_heading_0x58:
            heading_handled = SuperCarry_YRotation(object, input_angle);
            break;
        directional_heading_0x1b:
            if (__builtin_expect(object->field_0x7a3 == 1, 0))
                api.movement_facing_angle = input_angle;
            else
                FaceOpponent(object, NULL);
            turn_override = 0.333f;
            break;
        directional_heading_0x16:
            FaceOpponent(object, NULL);
            break;
        directional_heading_8:
        directional_heading_0x12:
            if ((api.field_0x1f4 & 0x40000) == 0)
                api.movement_facing_angle = object->force_heading;
            break;
        directional_heading_0x2d:
            if ((api.field_0x1f4 & 0x40000) == 0) {
                NUVEC centre;
                GizGetBuildItPlayerPos(object, NULL, &centre);
                GIZBUILDIT_s *buildit = static_cast<GIZBUILDIT_s *>(object->field_0x788);
                if ((buildit->state_flags & 0x20) != 0)
                    GizBuildItPushAwayFromStart(object, buildit);
                api.movement_facing_angle = NuAtan2D(centre.x - api.position.x, centre.z - api.position.z);
            }
            break;
        } while (false);
    }
    const i32 direct_turn = heading_handled != 0 ? 0 : (object->character_context == 0x33 ? 1 : object->snap_facing);
    if (heading_handled == 0) {
        f32 turn_rate;
        if (SuperCarry_Carrying(object))
            turn_rate = 8.0f * object->field_0x768;
        else if (object->character_context == 0x33 || object->character_context == 0x1c)
            turn_rate = 8.0f;
        else if (turn_override != 0.0f)
            turn_rate = 8.0f * turn_override;
        else {
            turn_rate = 8.0f * api.character_data->game_character->turn_rate;
            if (object->id == id_ATAT && WORLD->current_level == SPEEDERCHASEA_LDATA)
                turn_rate += turn_rate;
        }
        if ((object->field_0xefd & 0x80) != 0 && (CInfo[object->character_context].flags & 1) == 0)
            api.movement_facing_angle += 0x8000;
        if ((object->field_0xefd & 4) != 0) {
            api.facing_angle = api.movement_facing_angle;
            api.field_0x276 = api.movement_facing_angle;
        } else if (direct_turn == 0 &&
                   (turn_override != 0.0f || (CInfo[object->character_context].parameter & 2) != 0 ||
                    (api.character_data->game_character->flags_090 & 0x100) != 0 || SuperCarry_Carrying(object))) {
            if (object->character_context == 0x4b && (api.character_data->game_character->flags_090 & 0x100) == 0)
                turn_rate *= 0.333f;
            api.facing_angle =
                TurnRot(api.facing_angle, api.movement_facing_angle, static_cast<i32>(turn_rate * 16384.0f), NULL);
            api.field_0x276 = SeekRot(api.field_0x276, api.facing_angle, 10.0f);
            api.movement_facing_angle = api.facing_angle;
        } else {
            api.facing_angle = SeekRot(api.facing_angle, api.movement_facing_angle, turn_rate);
            api.field_0x276 = api.facing_angle;
        }
    }
    if (ObjInTube(object)) {
        object->target_velocity.y = static_cast<TUBE *>(object->field_0x788)->field_0x24;
        if (WORLD->current_level == CRUISERD_LDATA)
            object->target_velocity.y += object->target_velocity.y;
    } else {
        object->target_velocity.y = 0.0f;
    }
    i32 move_vertical = 0;
    i32 separate_seek_rates = false;
    NUVEC seek_rates;
    seek_rates.x = api.character_data->game_character->velocity_seek_rate;
    if (api.movement_direction.x != 0.0f || api.movement_direction.z != 0.0f) {
        object->target_velocity.x = api.movement_direction.x;
        object->target_velocity.z = api.movement_direction.z;
        api.movement_direction.z = 0.0f;
        api.movement_direction.x = 0.0f;
        object->field_0xe23 |= 0x10;
    } else {
        do {
            if (object->character_context == 0x57) {
                api.position = api.start_position;
                api.collision_position = api.initial_position;
                api.velocity = v000;
                break;
            } else if (object->character_context == 0x45) {
                move_vertical = WallShuffle_SetTargetMom(object, input_angle);
                break;
            } else if (object->character_context == 0x55) {
                move_vertical = LedgeTerrain_SetTargetMom(object);
                break;
            } else if (object->character_context == 0x5b) {
                object->target_velocity.x = (object->launch_origin.x - api.upper_position.x) * 20.0f;
                object->target_velocity.y = (object->launch_origin.y - api.upper_position.y) * 20.0f;
                object->target_velocity.z = (object->launch_origin.z - api.upper_position.z) * 20.0f;
                move_vertical = 1;
                break;
            } else if (object->character_context == 0x27 || object->character_context == 0x28 ||
                       object->character_context == 0x59) {
                {
                    const u16 angle = api.movement_facing_angle;
                    object->target_velocity.x = NU_SIN_LUT(angle);
                    api.velocity.x = object->target_velocity.x;
                    object->target_velocity.z = NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff];
                    api.velocity.z = object->target_velocity.z;
                    break;
                }
            } else if (object->character_context == 0x4e) {
                move_vertical = Hang_SetTargetMom(object);
                break;
            } else if (object->character_context == 0x43) {
                move_vertical = Climb_SetTargetMom(object, input_angle);
                break;
            } else if (object->character_context == 0x46) {
                Grapple_SetRotOrder(object);
                move_vertical = Grapple_SetTargetMom(object);
                break;
            } else if (object->character_context == 0x4a) {
                if ((api.field_0x1f4 & 0x40000) == 0) {
                    const f32 rate = 10.0f * object->field_0x768 + 5.0f;
                    const NUVEC position = static_cast<LEVER_s *>(object->field_0x788)->floor_position;
                    object->target_velocity.z = (position.z - api.position.z) * rate;
                    object->target_velocity.x = (position.x - api.position.x) * rate;
                }
                break;
            } else if (object->character_context == 0x0b) {
                if ((api.field_0x1f4 & 0x40000) == 0) {
                    NUVEC position;
                    GizPanel_GetAbsTargetPos(static_cast<GIZPANEL_s *>(object->field_0x788), &position, 1);
                    const f32 rate = 10.0f * object->field_0x768 + 5.0f;
                    object->target_velocity.z = (position.z - api.position.z) * rate;
                    object->target_velocity.x = (position.x - api.position.x) * rate;
                }
                break;
            } else if (object->character_context == 0x51) {
                {
                    TECHNO_s *techno = static_cast<TECHNO_s *>(object->field_0x788);
                    object->target_velocity.z = (techno->ground_position.z - api.position.z) * 5.0f;
                    object->target_velocity.x = (techno->ground_position.x - api.position.x) * 5.0f;
                    break;
                }
            } else if (object->character_context == 0x53) {
                {
                    ATTRACTO_s *attracto = static_cast<ATTRACTO_s *>(object->field_0x788);
                    object->target_velocity.x = (attracto->active_position.x - api.position.x) * 5.0f;
                    object->target_velocity.z = (attracto->active_position.z - api.position.z) * 5.0f;
                    break;
                }
            } else if (object->character_context == 0x5c) {
                {
                    SECURITYDOOR *door = static_cast<SECURITYDOOR *>(object->field_0x788);
                    object->target_velocity.z = (door->player_position.z - api.position.z) * 5.0f;
                    object->target_velocity.x = (door->player_position.x - api.position.x) * 5.0f;
                    break;
                }
            } else if (object->character_context == 0x4c) {
                {
                    SIGNAL_s *signal = static_cast<SIGNAL_s *>(object->field_0x788);
                    object->target_velocity.x = (signal->target_position.x - api.collision_position.x) * 5.0f;
                    object->target_velocity.y = (signal->target_position.y - api.collision_position.y) * 5.0f;
                    object->target_velocity.z = (signal->target_position.z - api.collision_position.z) * 5.0f;
                    move_vertical = 1;
                    break;
                }
            } else if (object->character_context == 0x44) {
                move_vertical = TightRope_SetTargetMom(object);
                break;
            } else if (object->character_context == 0x4f) {
                move_vertical = Glide_SetTargetMom(object);
                seek_rates.y = seek_rates.x;
                seek_rates.x = seek_rates.z = 2.0f;
                separate_seek_rates = true;
                break;
            } else {
                {
                    if (object->character_context == 0x61 && (api.field_0x1f4 & 0x40000) == 0) {
                        const f32 rate = 10.0f * object->field_0x768 + 5.0f;
                        const NUVEC position = static_cast<HATMACHINE_s *>(object->field_0x788)->player_position;
                        object->target_velocity.z = (position.z - api.position.z) * rate;
                        object->target_velocity.x = (position.x - api.position.x) * rate;
                        break;
                    }
                    const bool animation_movement = object->character_context != 5;
                    if (animation_movement) {
                        const f32 animation_speed = AnimSpeed(api.character_model, current_animation);
                        if (animation_speed != 0.0f) {
                            if (CanStepBack(object) && StepBackFromTarget(object))
                                break;
                            const f32 speed_multiplier = object->animation_speed_multiplier;
                            const f32 stop_frame = AnimStopFrame(api.character_model, current_animation);
                            f32 *frame = AnimPlaying(&api.anim_packet, current_animation, 1, 0);
                            bool stopped = stop_frame > 1.0f && frame != NULL && *frame >= stop_frame;
                            if (!stopped && frame != NULL && api.field_0x27d != 0 &&
                                api.character_model->model_data_b[current_animation] != NULL &&
                                (static_cast<CHARACTERANIM_s *>(api.character_model->model_data_a[current_animation])
                                     ->flags &
                                 2) == 0 &&
                                (api.anim_packet.flags & ANIMPACKET_FLAG_FINISHED) != 0)
                                stopped = true;
                            if (!stopped) {
                                const u16 angle =
                                    (api.character_data->game_character->flags_090 & 0x100) != 0 && direct_turn == 0
                                        ? api.facing_angle
                                        : api.movement_facing_angle;
                                const f32 speed = animation_speed * speed_multiplier;
                                object->target_velocity.x = NU_SIN_LUT(angle) * speed;
                                object->target_velocity.z = NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff] * speed;
                                if (object->character_context == 0x25 && object->context_animation != 0x58)
                                    NuVecRotateY(&object->target_velocity, &object->target_velocity, 0x4000);
                            } else {
                                object->target_velocity.z = 0.0f;
                                object->target_velocity.x = 0.0f;
                                const i32 surface = static_cast<i8>(api.field_0x281);
                                if (!(api.field_0x27d != 0 && surface >= 0 && surface < 32 &&
                                      TerSurface[surface].movement_scale < 1.0f)) {
                                    api.velocity.x = 0.0f;
                                    api.velocity.z = 0.0f;
                                }
                            }
                            break;
                        }
                        // Original zero-animation-speed dispatch, 0x1690d8.
                        if (object->character_context == 0x26 && object->field_0x7a7 != -1 &&
                            object->force_target != NULL) {
                            SpecialMove_Attacker_SetTargetMom(object);
                            break;
                        }
                        if (object->character_context == 0x33) {
                            move_vertical = Slide_SetTargetMom(object, input_angle, desired_speed);
                            break;
                        }
                        if (object->character_context == 0x58) {
                            move_vertical = SuperCarry_SetTargetMom(object, desired_speed);
                            break;
                        }
                        if (object->character_context == 0x5a) {
                            if (object->field_0x7a3 == 0 && (api.field_0x1f8 & 2) == 0) {
                                const u16 angle = api.movement_facing_angle;
                                object->target_velocity.x = -NU_SIN_LUT(angle) * object->external_force.z;
                                object->target_velocity.z =
                                    -NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff] * object->external_force.z;
                            } else {
                                object->target_velocity.x = 0.0f;
                                object->target_velocity.z = 0.0f;
                            }
                            break;
                        }
                    }
                    if ((CInfo[object->character_context].flags & 2) != 0) {
                        object->target_velocity.x = 0.0f;
                        object->target_velocity.z = 0.0f;
                        break;
                    }
                    if (object->character_context == 0x5f) {
                        object->target_velocity.x = object->external_force.x;
                        object->target_velocity.z = object->external_force.z;
                        break;
                    }
                    switch (object->character_context) {
                        case 0x21:
                        case 0x22: {
                            const u16 angle = api.movement_facing_angle;
                            object->target_velocity.x = -NU_SIN_LUT(angle) * 1.2f;
                            object->target_velocity.z = -NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff] * 1.2f;
                            break;
                        }
                        case 0x18:
                        case 0x0c:
                            if (object->incoming_bolt != NULL && desired_speed > 0.0f &&
                                (pad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 &&
                                (object->character_context == 0x18 || object->block_latch == 0)) {
                                object->target_velocity.x =
                                    NuTrigTable[input_angle >> 1] * api.character_data->game_character->run_speed;
                                object->target_velocity.z = NuTrigTable[((input_angle + 0x4000) >> 1) & 0x7fff] *
                                                            api.character_data->game_character->run_speed;
                            } else {
                                object->target_velocity.x = 0.0f;
                                object->target_velocity.z = 0.0f;
                            }
                            break;
                        case 0x41:
                        case 0x17:
                        case 0x1f:
                        case 0x3c:
                        case 0x35:
                            object->target_velocity.x = 0.0f;
                            object->target_velocity.z = 0.0f;
                            break;
                        case 0x29:
                            // Original 0x169c21: walking is allowed only when this
                            // model supplies animation 0x57.
                            if (api.character_model->model_data_b[0x57] != NULL)
                                goto directional_walking;
                            object->target_velocity.x = 0.0f;
                            object->target_velocity.z = 0.0f;
                            break;
                        case 0x2d: {
                            NUVEC position = api.position;
                            GizGetBuildItPlayerPos(object, &position, NULL);
                            object->target_velocity.x = (position.x - api.position.x) * 3.0f;
                            object->target_velocity.z = (position.z - api.position.z) * 3.0f;
                            break;
                        }
                        case 0x1c:
                            move_vertical = ForcePushed_SetTargetMom(object, &seek_rates.x);
                            break;
                        case 0x0f:
                            break;
                        case 0x49:
                            object->target_velocity.x = (object->launch_origin.x - api.position.x) * 8.0f;
                            object->target_velocity.z = (object->launch_origin.z - api.position.z) * 8.0f;
                            break;
                        case 0x47: {
                            if (object->action_movement_state != 1) {
                                ZIPUP *zipup = static_cast<ZIPUP *>(object->field_0x788);
                                if ((zipup->flags & 1) != 0) {
                                    SeekVec(&api.position, &api.position, &zipup->rider_target_position, 8.0f);
                                } else {
                                    NuVecSub(&object->target_velocity, &zipup->hook_origin, &api.position);
                                    NuVecNorm(&object->target_velocity, &object->target_velocity);
                                    NuVecScale(&object->target_velocity, &object->target_velocity, 2.0f);
                                }
                            } else {
                                object->target_velocity = v000;
                                api.velocity = object->target_velocity;
                            }
                            move_vertical = 1;
                            break;
                        }
                        case 5:
                            if ((object->context_flags & 0x1c) != 0) {
                                object->target_velocity.x = 0.0f;
                                api.velocity.x = 0.0f;
                                object->target_velocity.z = 0.0f;
                                api.velocity.z = 0.0f;
                            } else if (StepBackFromTarget(object) == 0) {
                                const f32 speed = AnimSpeed(api.character_model, object->context_animation);
                                const u16 angle = api.movement_facing_angle;
                                object->target_velocity.x = NU_SIN_LUT(angle) * speed;
                                object->target_velocity.z = NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff] * speed;
                            }
                            break;
                        case 0x15: {
                            const f32 speed = AnimSpeed(api.character_model, object->context_animation);
                            const u16 angle = api.movement_facing_angle;
                            object->target_velocity.x = NU_SIN_LUT(angle) * speed;
                            object->target_velocity.z = NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff] * speed;
                            break;
                        }
                        case 0x10: {
                            const f32 speed = object->context_animation == 0x29
                                                  ? AnimSpeed(api.character_model, 0x29)
                                                  : api.character_data->game_character->run_speed;
                            const u16 angle = api.movement_facing_angle;
                            object->target_velocity.x = NU_SIN_LUT(angle) * speed;
                            object->target_velocity.z = NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff] * speed;
                            break;
                        }
                        case 0x31: {
                            if (!api.player_controlled)
                                goto directional_walking;
                            const u16 angle = api.movement_facing_angle;
                            object->target_velocity.x =
                                NU_SIN_LUT(angle) * api.character_data->game_character->run_speed;
                            object->target_velocity.z = NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff] *
                                                        api.character_data->game_character->run_speed;
                            break;
                        }
                        case 1:
                        case 0x19:
                        case 2:
                        case 3:
                        case 4:
                        case 0x0d:
                        case 0x0e:
                        case 9:
                        case 0x0a:
                        case 0x16:
                        case 0x1d:
                        case 0x1b:
                        case 0x12:
                        case 8:
                        case 0x13:
                        case 0x14: {
                        directional_action_momentum:
                            // Original 0x169a49: these contexts only move through
                            // their animation when CInfo permits animation momentum.
                            if ((CInfo[object->character_context].flags & 0x10) != 0) {
                                const f32 speed = AnimSpeed(api.character_model, object->context_animation);
                                const u16 angle = api.movement_facing_angle;
                                object->target_velocity.x = NU_SIN_LUT(angle) * speed;
                                object->target_velocity.z = NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff] * speed;
                            } else {
                                object->target_velocity.x = 0.0f;
                                object->target_velocity.z = 0.0f;
                            }
                            break;
                        }
                        case 0:
                            if (object->action_movement_state == 3) {
                                if (StepBackFromTarget(object) == 0) {
                                    const u16 angle = api.movement_facing_angle;
                                    object->target_velocity.x = NU_SIN_LUT(angle) * object->field_0x768;
                                    object->target_velocity.z =
                                        NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff] * object->field_0x768;
                                }
                                break;
                            }
                            if (object->action_movement_state == 4)
                                goto directional_action_momentum;
                            if (object->action_movement_state == 2) {
                                f32 speed = api.character_data->game_character->run_speed;
                                if ((object->context_variant_flags & 0x20) != 0)
                                    speed = -speed;
                                const u16 angle = api.movement_facing_angle;
                                object->target_velocity.x = NU_SIN_LUT(angle) * speed;
                                object->target_velocity.z = NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff] * speed;
                                break;
                            }
                            if (object->context_variant_flags >= 0 &&
                                (object->action_movement_state == 6 || object->action_movement_state == 7 ||
                                 object->action_movement_state == 9)) {
                                f32 speed = object->airborne_action_timer;
                                if (object->action_movement_state == 6 &&
                                    object->context_animation_timer >=
                                        api.character_data->game_character->jump_duration) {
                                    api.velocity.x = 0.0f;
                                    api.velocity.z = 0.0f;
                                    speed = 0.0f;
                                }
                                const u16 angle = api.movement_facing_angle;
                                object->target_velocity.x = NU_SIN_LUT(angle) * speed;
                                object->target_velocity.z = NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff] * speed;
                                break;
                            }
                            goto directional_walking;
                        default:
                            goto directional_walking;
                    }
                    break;
                directional_walking:
                    if ((pad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) != 0) {
                        object->target_velocity.x = 0.0f;
                        object->target_velocity.z = 0.0f;
                        break;
                    }
                    const bool walking_input = pad->input_magnitude > 0.0f;
                    bool walking_without_input = false;
                    if (!walking_input) {
                        if (object->id == id_ATST || object->id == id_MINIATST || object->id == id_ATST_LOWRES ||
                            object->id == id_ATAT || object->id == id_MINIATAT || object->id == id_MINIATTE)
                            walking_without_input = CurrentAnim(&api.anim_packet) == 0;
                        if (!walking_without_input && object->id == id_RANCOR)
                            walking_without_input =
                                CurrentAnim(&api.anim_packet) == 0 || CurrentAnim(&api.anim_packet) == 0x40;
                    }
                    const bool calculate_walking_speed = walking_input || walking_without_input;
                    if (!calculate_walking_speed && api.anim_packet.blending != 0) {
                        i32 animation = api.anim_packet.blend_animation_a;
                        const f32 speed = AnimSpeed(api.character_model, animation);
                        if (speed != 0.0f) {
                            animation = api.anim_packet.blend_animation_a;
                            CHARACTERANIM_s *info =
                                static_cast<CHARACTERANIM_s *>(api.character_model->model_data_a[animation]);
                            bool animation_active = (info->flags & 2) != 0;
                            if (!animation_active) {
                                const f32 source_time = api.anim_packet.blend_source_time;
                                animation_active =
                                    NuAnimEndFrame(api.character_model->model_data_b[animation]) > source_time;
                            }
                            if (animation_active) {
                                animation = api.anim_packet.blend_animation_a;
                                bool stopped = static_cast<u16>(animation - 0x2e) <= 0x0d;
                                if (!stopped) {
                                    const f32 stop_frame = AnimStopFrame(api.character_model, animation);
                                    stopped = stop_frame > 0.0f && api.anim_packet.blend_source_time >= stop_frame;
                                }
                                if (!stopped) {
                                    const u16 angle =
                                        direct_turn == 0 &&
                                                ((CInfo[object->character_context].parameter & 2) != 0 ||
                                                 (api.character_data->game_character->flags_090 & 0x100) != 0)
                                            ? api.facing_angle
                                            : api.movement_facing_angle;
                                    object->target_velocity.x = speed * NU_SIN_LUT(angle);
                                    object->target_velocity.z = speed * NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff];
                                    if (api.anim_packet.blend_animation_a == 0x4f ||
                                        api.anim_packet.blend_animation_a == 0x26)
                                        NuVecRotateY(&object->target_velocity, &object->target_velocity, 0x4000);
                                } else {
                                    object->target_velocity.x = 0.0f;
                                    object->target_velocity.z = 0.0f;
                                    const i32 surface = static_cast<i8>(api.field_0x281);
                                    if (!(api.field_0x27d != 0 && surface >= 0 && surface < 32 &&
                                          TerSurface[surface].movement_scale < 1.0f)) {
                                        api.velocity.x = 0.0f;
                                        api.velocity.z = 0.0f;
                                    }
                                }
                                break;
                            }
                        }
                    }
                    if (!calculate_walking_speed) {
                        if (object->touch_task == NULL ||
                            object->touch_task->GetHashId().value != MechTouchTaskJump::HashId.value) {
                            object->target_velocity.x = 0.0f;
                            object->target_velocity.z = 0.0f;
                        }
                        break;
                    }
                    f32 water_speed_multiplier = 1.0f;
                    if (calculate_walking_speed) {
                        if (object->character_context == 0x4b) {
                            water_speed_multiplier = 0.7f;
                        } else if (api.intersects_water != 0 && object->id != id_YODA && object->id != id_YODAGHOST) {
                            const f32 depth = (api.water_height - api.collision_min.y) / api.field_0x1e0;
                            if (depth < 0.0f)
                                water_speed_multiplier = 0.0f;
                            else if (depth > 0.5f)
                                water_speed_multiplier = 0.7f;
                            else
                                water_speed_multiplier = 1.0f - (depth + depth) * 0.3f;
                        }
                    }
                    f32 walking_speed = desired_speed;
                    if ((object->field_0xeff & 2) == 0 && IsAFallAnim(CurrentAnim(&api.anim_packet)) != 0) {
                        walking_speed = 0.0f;
                    } else if (object->id == id_YODA || object->id == id_YODAGHOST) {
                        if (object->character_context == 0 && object->action_movement_state == 0) {
                            walking_speed = 1.2f;
                        } else if (pad->input_magnitude > 0.0f) {
                            walking_speed = api.character_data->game_character->run_speed;
                        } else if (AnimPlaying(&api.anim_packet, 5, 1, 0) != NULL) {
                            GAMECHARACTERDATA *character = api.character_data->game_character;
                            walking_speed = (0.7f * character->run_speed) / 3.0f;
                            const f32 magnitude = object->pad_gamepad->input_magnitude;
                            if (!((character->tiptoe_speed + character->walk_speed) * 0.5f >= magnitude)) {
                                if (!((character->walk_speed + character->run_speed) * 0.5f >= magnitude))
                                    walking_speed *= 3.0f;
                                else
                                    walking_speed += walking_speed;
                            }
                        }
                    } else {
                        GAMECHARACTERDATA *walking_character = api.character_data->game_character;
                        if ((walking_character->flags_094[3] & 0x10) != 0) {
                            if (object->field_0xe31 == 1) {
                                walking_speed = walking_character->run_speed / 3.0f;
                                const f32 magnitude = object->pad_gamepad->input_magnitude;
                                if (!((walking_character->tiptoe_speed + walking_character->walk_speed) * 0.5f >=
                                      magnitude)) {
                                    if (!((walking_character->walk_speed + walking_character->run_speed) * 0.5f >=
                                          magnitude))
                                        walking_speed *= 3.0f;
                                    else
                                        walking_speed += walking_speed;
                                }
                            } else {
                                walking_speed = walking_character->walk_speed;
                            }
                        } else {
                            if (object->id == id_GONKDROID) {
                                if (Cheat_IsOn(8) == 0)
                                    walking_speed = CurrentAnim(&api.anim_packet) == 5
                                                        ? 0.5f * api.character_data->game_character->run_speed
                                                        : api.character_data->game_character->walk_speed;
                            } else if (object->id == id_C3PO || object->id == id_TC14) {
                                if (object->character_context == -1 && object->field_0xdb0 > 0.0f) {
                                    walking_speed = (0.6f * walking_character->run_speed) / 3.0f;
                                    const f32 magnitude = object->pad_gamepad->input_magnitude;
                                    if (!((walking_character->tiptoe_speed + walking_character->walk_speed) * 0.5f >=
                                          magnitude)) {
                                        if (!((walking_character->walk_speed + walking_character->run_speed) * 0.5f >=
                                              magnitude))
                                            walking_speed *= 3.0f;
                                        else
                                            walking_speed += walking_speed;
                                    }
                                }
                            } else if (object->id == id_DROIDEKA) {
                                if (api.anim_packet.field_0x3a == 5)
                                    walking_speed *= 0.5f;
                            } else if (object->id == id_ATST || object->id == id_MINIATST ||
                                       object->id == id_ATST_LOWRES || object->id == id_ATAT ||
                                       object->id == id_MINIATAT || object->id == id_MINIATTE ||
                                       object->id == id_RANCOR) {
                                walking_speed = walking_character->run_speed;
                            }
                        }
                    }
                    if (object->character_context == 0 && object->action_movement_state == 0)
                        walking_speed *= api.character_data->game_character->field_0x84;
                    if ((object->field_0xefd & GAMEOBJECT_MOVEMENT_FLAG_BACKWARDS) != 0)
                        walking_speed *= api.character_data->game_character->backwards_speed_multiplier;
                    u16 walking_angle = input_angle;
                    if (direct_turn == 0 && ((CInfo[object->character_context].parameter & 2) != 0 ||
                                             (api.character_data->game_character->flags_090 & 0x100) != 0))
                        walking_angle = api.facing_angle;
                    if (WORLD->current_level == VADERA_LDATA && !api.player_controlled &&
                        GameCam->sock_position.location.sock == 0)
                        walking_speed *= 1.0416666269302368f;
                    object->target_velocity.x = NU_SIN_LUT(walking_angle) * walking_speed * water_speed_multiplier;
                    object->target_velocity.z =
                        NuTrigTable[((walking_angle + 0x4000) >> 1) & 0x7fff] * walking_speed * water_speed_multiplier;
                    break;
                }
            }
        } while (false);
    }
    if ((object->field_0xefd & GAMEOBJECT_MOVEMENT_FLAG_REVERSE_VELOCITY) != 0) {
        NuVecRotateY(&object->target_velocity, &object->target_velocity, 0x8000);
    } else if (api.player_controlled && (api.character_data->game_character->flags_090 & 0x100) != 0)
        CharPivot_Check(object, &object->target_velocity);

    if (object->character_context == 0x4b) {
        const f32 float_height = api.collision_min.y + (api.collision_max.y - api.collision_min.y) * 0.625f;
        object->target_velocity.y = (api.water_height - float_height) * 3.0f;
        if (object->suit != NULL && (static_cast<SUIT_s *>(object->suit)->store_flag & 0x10) != 0) {
            if ((object->pad_gamepad->buttons_held & GAMEPAD_SPECIAL) != 0) {
                if (api.water_height > api.upper_position.y)
                    object->target_velocity.y = 0.6f;
                move_vertical = 1;
            } else {
                object->target_velocity.y = 0.0f;
            }
        } else {
            move_vertical = 1;
        }
        seek_rates.x = seek_rates.z = 1.0f;
        seek_rates.y = api.character_data->game_character->velocity_seek_rate;
    } else if (!separate_seek_rates) {
        seek_rates.y =
            move_vertical != 0 ? seek_rates.x : object->apiobj.character_data->game_character->velocity_seek_rate;
        seek_rates.z = seek_rates.x;
    }

    if (api.field_0x27d != 0 && static_cast<i8>(api.field_0x281) != -1) {
        f32 scale = TerSurface[static_cast<i8>(api.field_0x281)].movement_scale;
        if (scale != 1.0f) {
            seek_rates.x *= scale;
            seek_rates.z *= scale;
        }
    }
    if (object->character_context == 0x2b) {
        if (object->turn_braking < 1.0f) {
            const f32 scale = 1.0f - object->turn_braking;
            object->target_velocity.x *= scale;
            object->target_velocity.z *= scale;
            if (object->field_0xe36 == 2 || object->field_0xe36 == 4) {
                object->target_velocity.x *= 0.5f;
                object->target_velocity.z *= 0.5f;
            }
        } else {
            object->target_velocity.x = 0.0f;
            object->target_velocity.z = 0.0f;
        }
        if (object->field_0xe36 == 2 || object->field_0xe36 == 4) {
            NewRumble(pad->pad, qrand() * 1.5259022e-05f * 0.25f, 0);
            const i32 effect = WORLD->debris_sys->entries[58].effect;
            if (effect != -1) {
                const i32 count = ParticlesPerSecond(60.0f, FRAMETIME);
                if (count > 0) {
                    NUVEC position = {api.collision_position.x, api.water_height, api.collision_position.z};
                    AddVariableShotDebrisEffect(effect, &position, count, 0, 0);
                }
            }
        }
    }
    if ((object->field_0xe23 & 0x10) == 0 && api.field_0x27d != 0 && static_cast<i8>(api.field_0x281) != -1 &&
        (TerSurface[static_cast<i8>(api.field_0x281)].flags & 0x100) != 0)
        Conveyor_AdjustSpeed(&object->target_velocity);
    if (WORLD->current_level == CRUISERB_LDATA && Mission_Active(NULL) != NULL) {
        NUVEC position = {11.25f, 1.466f, -25.445f};
        if (NuVecDistSqr(&api.collision_position, &position, NULL) < 0.4f * 0.4f &&
            api.position.z > position.z - 0.1f) {
            object->target_velocity.z = -2.0f;
        } else {
            position.x = 13.765f;
            if (NuVecDistSqr(&api.collision_position, &position, NULL) < 0.4f * 0.4f &&
                api.position.z > position.z - 0.1f)
                object->target_velocity.z = -2.0f;
        }
    }
    if (move_vertical != 0 || ObjInTube(object)) {
        // The vertical path shares the gizmo payload byte at +0x68.
        if (object->field_0x788 != NULL && (static_cast<const u8 *>(object->field_0x788)[0x68] & 1) != 0) {
            seek_rates.x *= 0.5f;
            seek_rates.y *= 0.5f;
            seek_rates.z *= 0.5f;
        }
        api.velocity.x = SeekValF(api.velocity.x, object->target_velocity.x, seek_rates.x);
        api.velocity.y = SeekValF(api.velocity.y, object->target_velocity.y, seek_rates.y);
        api.velocity.z = SeekValF(api.velocity.z, object->target_velocity.z, seek_rates.z);
    } else {
        if ((object->field_0xe20 & 8) != 0 && api.field_0x27d != 0) {
            const f32 current_speed = NuVecMag(&api.velocity);
            const f32 target_speed = NuVecMag(&object->target_velocity);
            f32 multiplier = 5.0f;
            if (target_speed > 0.0f) {
                f32 ratio = current_speed / target_speed;
                if (ratio > 1.0f)
                    ratio = 1.0f - (ratio - 1.0f);
                if (ratio < 0.7f)
                    multiplier = 5.0f;
                else if (ratio < 0.8f)
                    multiplier = 4.0f;
                else if (ratio < 0.9f)
                    multiplier = 3.0f;
                else if (ratio < 1.0f)
                    multiplier = 2.0f - (ratio - 0.9f) / 0.1f;
                else
                    multiplier = 1.0f;
            }
            object->field_0xd78 = SeekLinearF(object->field_0xd78, multiplier, 5.0f * FRAMETIME);
            if (object->field_0xd78 > 1.0f) {
                seek_rates.x *= object->field_0xd78;
                seek_rates.z *= object->field_0xd78;
            }
        } else {
            object->field_0xd78 = 1.0f;
        }
        if (object->character_context == 0x26) {
            api.velocity.x = object->target_velocity.x;
            api.velocity.z = object->target_velocity.z;
        } else {
            api.velocity.x = SeekValF(api.velocity.x, object->target_velocity.x, seek_rates.x);
            api.velocity.z = SeekValF(api.velocity.z, object->target_velocity.z, seek_rates.z);
        }
    }
    game_character = api.character_data->game_character;
    if ((game_character->flags_090 & 1) != 0) {
        i32 rate =
            static_cast<i32>(static_cast<f32>(RotDiff(object->previous_movement_angle, api.field_0x276)) / FRAMETIME);
        i32 lean;
        if (rate < -0x10000)
            lean = -0x2000;
        else if (rate > 0x10000)
            lean = 0x2000;
        else {
            lean = rate / 4;
            if (lean < -0x2000)
                lean = -0x2000;
            if (lean > 0x2000)
                lean = 0x2000;
        }
        object->movement_lean_angle = SeekRot(object->movement_lean_angle, static_cast<u16>(lean), 8.0f);
    } else if (object->character_context == 0x4b) {
        i32 lean = 0;
        if (object->pad_gamepad->input_magnitude > 0.0f) {
            lean = static_cast<i32>(static_cast<f32>(RotDiff(object->previous_movement_angle, api.field_0x276)) /
                                    FRAMETIME) /
                   15;
            if (lean > 0x2000)
                lean = 0x2000;
            if (lean < -0x2000)
                lean = -0x2000;
        }
        object->movement_lean_angle = SeekRot(object->movement_lean_angle, static_cast<u16>(lean), 8.0f);
    } else {
        object->movement_lean_angle = 0;
    }
    u16 secondary_lean = 0;
    if (object->character_context == 0x3d && object->context_animation != 0x85)
        secondary_lean = object->field_0x7a3 == 0 ? 0xce39 : 0x3c71;
    else if (object->character_context == 0x2f && object->context_animation == 5)
        secondary_lean = 0xc000;
    object->secondary_lean_angle = SeekRot(object->secondary_lean_angle, secondary_lean, 5.0f);
    GizmoBlowupCheckProximity(WORLD, object);
}

void FloatCode(GameObject_s *) {
}

i32 SmartBomb_UpdateHint(HINT_s *) {
    if (VehicleArea == 0 || BonusArea != 0)
        return 0;
    if (!Cheat_IsOn(0x14))
        return 0;
    if (player == NULL)
        return 0;
    return player->field_0x7a5 == 0xff;
}

void Disorientate(GameObject_s *object, nuvec_s *) {
    const i32 elapsed_seconds = static_cast<i32>(object->context_animation_timer);
    if ((elapsed_seconds & 1) == 0 && static_cast<f32>(elapsed_seconds) != object->field_0x770) {
        object->field_0x770 = static_cast<f32>(elapsed_seconds);
        disorientAxis = 0;
        while ((disorientAxis & 5) == 0) {
            if (NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) < 0.5f) {
                disorientAxis |= 1;
            }
            if (NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) < 0.5f) {
                disorientAxis |= 2;
            }
            if (NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) < 0.5f) {
                disorientAxis |= 4;
            }
        }
    }

    if ((disorientAxis & 1) != 0) {
        object->apiobj.pitch_angle =
            SeekRot(object->apiobj.pitch_angle, static_cast<u16>(object->apiobj.pitch_angle + 0x4000), 3.0f);
    }
    if ((disorientAxis & 2) != 0) {
        object->apiobj.field_0x276 =
            SeekRot(object->apiobj.field_0x276, static_cast<u16>(object->apiobj.field_0x276 + 0x4000), 3.0f);
    }
    if ((disorientAxis & 4) != 0) {
        object->apiobj.roll_angle =
            SeekRot(object->apiobj.roll_angle, static_cast<u16>(object->apiobj.roll_angle + 0x4000), 3.0f);
    }

    NUVEC delta;
    const f32 distance_squared =
        NuVecDistSqr(&object->apiobj.collision_position, &object->disorientation_destination, &delta);
    const f32 speed = 75.0f / (distance_squared / 6.0f + 1.0f);
    const f32 fraction = 1.0f / ((delta.x + 0.01f) / (0.01f + delta.z));
    f32 x_velocity = (1.0f - fraction) * speed;
    f32 z_velocity = fraction * speed;
    if (x_velocity > 100.0f) {
        x_velocity = 100.0f;
    } else {
        z_velocity = MIN(100.0f, z_velocity);
    }
    if ((delta.x < 0.0f && x_velocity > 0.0f) || (delta.x > 0.0f && x_velocity < 0.0f)) {
        x_velocity = -x_velocity;
    }
    if ((delta.z < 0.0f && z_velocity > 0.0f) || (delta.z > 0.0f && z_velocity < 0.0f)) {
        z_velocity = -z_velocity;
    }
    object->apiobj.field_0x1fc = x_velocity;
    object->apiobj.field_0x200 = 0.0f;
    object->apiobj.field_0x204 = z_velocity;
}

void DisorientateCode(GameObject_s *object, nuvec_s *target, f32 distance) {
    if (target == NULL) {
        goto continue_disorientation;
    }
    if (object->character_context != 0x40) {
        object->context_animation_timer = 0.0f;
    }
    object->character_context = 0x40;
    object->disorientation_destination = *target;

disorientate:
    Disorientate(object, &object->disorientation_destination);
    object->context_animation_timer += FRAMETIME;
    if (NuVecDistSqr(&object->apiobj.collision_position, &object->disorientation_destination, NULL) > distance * 1.5f &&
        __builtin_expect(object->context_animation_timer > 3.0f, false)) {
        goto start_turn;
    }
    return;

continue_disorientation:
    if (object->character_context != 0x40) {
        return;
    }
    goto disorientate;

start_turn:
    object->character_context = -1;
    StartTurn(object);
}

__attribute__((optimize("O2", "tree-vectorize"))) void Animate_CHARACTER(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if (object->id == id_JABBA && WORLD != NULL && WORLD->current_level == HUB_LDATA) {
        packet.requested_animation = CHARACTER_ANIMATION_IDLE;
        if (object->pad_gamepad->input_magnitude > 0.0f && object->apiobj.character_model->model_data_b[0] != NULL) {
            packet.requested_animation = CHARACTER_ANIMATION_WALK;
        } else if (object->apiobj.character_model->model_data_b[15] != NULL && hub_jabbaawake > 0.0f) {
            packet.requested_animation = 15;
        }
        return;
    }
    bool check_movement_animation = false;

    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) == 0) {
        if (object->context_target_position != NULL) {
            if (object->apiobj.character_model->model_data_b[43] != NULL) {
                packet.requested_animation = 43;
            } else {
                packet.requested_animation =
                    object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL
                        ? CHARACTER_ANIMATION_IDLE
                        : CHARACTER_ANIMATION_FALL;
            }
        } else if (object->character_context == CHARACTER_CONTEXT_FORCE) {
            packet.requested_animation = CHARACTER_ANIMATION_WEAPON_IDLE;
        } else {
            packet.requested_animation = CHARACTER_ANIMATION_FALL;

            if (object->character_context != CHARACTER_CONTEXT_DOOMED &&
                (object->apiobj.field_0x27d != 0 ||
                 ((object->ground_contact_grace_timer > 0.0f ||
                   object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL ||
                   (object->fall_animation_timer < 0.2f && object->nearby_floor_distance != 2000000.0f &&
                    object->nearby_floor_distance < 0.25f && object->apiobj.velocity.y < 0.0f)) &&
                  (!(object->apiobj.character_data->game_character->field_0x28 > 0.0f) ||
                   object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL)))) {
                packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
            }

            if (object->character_context == CHARACTER_CONTEXT_JUMP) {
                JumpAnimCode(object);
            } else if (UseFallAnim(object)) {
                packet.requested_animation = CHARACTER_ANIMATION_FALL;
            } else if (object->character_context == -1 && object->field_0xe31 == 1) {
                packet.requested_animation = object->pad_gamepad->input_magnitude > 0.0f ? 37 : 15;
            } else if (packet.requested_animation != CHARACTER_ANIMATION_FALL) {
                GAMEPAD_s *pad = object->pad_gamepad;
                if ((pad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 && pad->input_magnitude > 0.0f) {
                    const GAMECHARACTERDATA *game_character =
                        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                    if (!object->apiobj.player_controlled && game_character->field275_0x116 == 1) {
                        MoveAnim_Manage(object, pad->input_magnitude, 0, 1);
                    } else {
                        const i32 allow_tiptoe =
                            (game_character->flags_090 & GAMECHARACTER_FLAG_DISABLE_TIPTOE) == 0 ? 1 : 0;
                        MoveAnim_Manage(object, pad->input_magnitude, allow_tiptoe, 1);
                    }
                } else if (((object->field_0xe22 & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) != 0 ||
                            object->field_0xe32 == 1) &&
                           object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_ALT_IDLE] != NULL) {
                    packet.requested_animation = CHARACTER_ANIMATION_ALT_IDLE;
                }
            }
            check_movement_animation = true;
        }
    } else {
        packet.requested_animation = object->context_animation;
    }

    if (check_movement_animation) {
        MoveAnim_Check(object);
    }
    if (object->apiobj.player_controlled && (object->pad_gamepad->buttons_held & GAMEPAD_SPECIAL) != 0) {
        if (object->id == id_GAMORREANGUARD) {
            if (packet.requested_animation == CHARACTER_ANIMATION_ALT_IDLE)
                packet.requested_animation = 15;
        } else if (object->id == id_PRINCESSLEIASLAVE) {
            if (packet.requested_animation == CHARACTER_ANIMATION_IDLE)
                packet.requested_animation = 96;
        } else if ((object->id == id_LAMASU || object->id == id_TAUNWE) &&
                   packet.requested_animation == CHARACTER_ANIMATION_IDLE) {
            packet.requested_animation = 15;
        }
    }
    UpdateCharacterIdle(object);

    const i16 animation = packet.requested_animation;
    if (animation == CHARACTER_ANIMATION_FALL ||
        ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_HIGH_JUMP) != 0 &&
         (animation == CHARACTER_ANIMATION_FALL_VARIANT_75 || animation == CHARACTER_ANIMATION_FALL_VARIANT_40 ||
          animation == CHARACTER_ANIMATION_FALL_VARIANT_76))) {
        object->fall_animation_timer += FRAMETIME;
    } else {
        object->fall_animation_timer = 0.0f;
    }
    if (object->id != id_PENGUIN) {
        if (object->suit != NULL && (static_cast<SUIT_s *>(object->suit)->flags & 2) != 0) {
            if (id_GLIDEPACK < 0 || apicharsys == NULL)
                return;
            i16 model_id = apicharsys->playermodelids[id_GLIDEPACK];
            if (model_id == -1)
                return;
            object->mini_animation.requested_animation_id =
                object->character_context == 0x4f                                                           ? 45
                : object->character_context == CHARACTER_CONTEXT_JUMP && object->action_movement_state == 4 ? 33
                : object->character_context == 14                                                           ? 34
                                                                                                            : 44;
            UpdateMiniAnimPacket(&apicharsys->models[model_id], &object->mini_animation, FRAMETIME * 30.0f, 0.0f,
                                 FRAMETIME);
        }
    } else {
        if (id_UMBRELLA < 0 || apicharsys == NULL)
            return;
        i16 model_id = apicharsys->playermodelids[id_UMBRELLA];
        if (model_id == -1)
            return;
        CHARACTERMODEL_s *model = &apicharsys->models[model_id];
        object->mini_animation.requested_animation_id =
            animation == -1 || model->model_data_b[animation] == NULL ? CHARACTER_ANIMATION_IDLE : animation;
        UpdateMiniAnimPacket(model, &object->mini_animation, FRAMETIME * 30.0f, 0.0f, FRAMETIME);
    }
}

void PostAnimate_FETT(GameObject_s *object) {
    if (object->apiobj.field_0x287 != 0) {
        return;
    }

    const i8 context = object->character_context;
    if (context != -1 && !((context == 0x15 && object->context_animation == 62) ||
                           (context == 0x14 && object->context_animation == 77) ||
                           (context == 0x2e && object->context_animation == 111) ||
                           (context == 0x0a && object->context_animation == 60))) {
        return;
    }
    if (object->field_0xe31 != 1 || object->apiobj.field_0x288 == 0) {
        return;
    }

    const i32 count = ParticlesPerSecond(100.0f, FRAMETIME);
    const i32 first_joint = object->id == id_BOBAFETT ? 10 : 7;
    if (object->apiobj.character_model->points_of_interest[first_joint] != NULL) {
        AddGameDebrisRot(WORLD->debris_sys, 27, NUMTX_GET_ROW_VEC(&object->joint_matrices[first_joint], 3), count, 0,
                         object->apiobj.facing_angle);
    }
    if (object->apiobj.character_model->points_of_interest[8] != NULL) {
        AddGameDebrisRot(WORLD->debris_sys, 27, NUMTX_GET_ROW_VEC(&object->joint_matrices[8], 3), count, 0,
                         object->apiobj.facing_angle);
    }
    PlaySfx(const_cast<char *>("JgThrust"), &object->apiobj.collision_position);
}

void TractorBeamCode(GameObject_s *object) {
    if (object == NULL || WORLD->area == NULL || (WORLD->area->flags & 1) == 0)
        return;
    if (static_cast<i8>(object->apiobj.flags_low) >= 0)
        return;
    if (Cheat[29].enabled == 0 && !(object->field_0xdec > 0.0f))
        return;
    if ((object->apiobj.character_data->model_flags & 0x2000) == 0)
        return;

    const f32 range = object->apiobj.field_0x1dc * 10.0f;
    const f32 range_squared = range * range;
    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
        GameObject_s *candidate = &Obj[index];
        if (candidate == NULL || (candidate->apiobj.field_0x1f8 & 0x1001) != 0x1001 ||
            (candidate->apiobj.character_data->model_flags & 4) == 0 || (candidate->field_0xf04 & 4) != 0 ||
            (candidate->field_0xefb & 8) != 0 || candidate->apiobj.field_0x27c != -1 ||
            candidate->apiobj.field_0x27d != 0) {
            continue;
        }

        NUVEC direction;
        const f32 distance =
            NuVecDistSqr(&candidate->apiobj.collision_position, &object->apiobj.collision_position, &direction);
        const f32 collision_range = object->apiobj.field_0x1dc + candidate->apiobj.field_0x1dc;
        if (distance <= collision_range * collision_range) {
            ObjHitObj(NULL, candidate, -1, 0, 0, 1);
            NewRumble(object->pad_gamepad->pad, 0.5f, 0);
            NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
            continue;
        }
        if (range_squared > distance) {
            u16 z_rotation;
            u16 y_rotation;
            GetRotationAngles(&direction, &z_rotation, &y_rotation);
            AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[128].effect, &object->apiobj.position, 90,
                                              FRAMETIME, z_rotation, y_rotation, NULL);

            const f32 seek_rate = (1.0f + NU_SIN_LUT((1.0f - distance / range_squared) * 16384.0f + 49152.0f)) * 25.0f;
            NuVecNeg(&direction, &direction);
            NuVecAdd(&direction, &direction, &object->apiobj.velocity);
            SeekVec(&candidate->apiobj.velocity, &candidate->apiobj.velocity, &direction, seek_rate);

            NUVEC limit = {1.0e9f, 1.0e9f, 1.0e9f};
            DisorientateCode(candidate, &limit, range);
        }
    }

    PlaySfx(const_cast<char *>("env_tractorbeam_lp"), &object->apiobj.collision_position);
}

void Animate_VEHICLE(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        packet.requested_animation = object->context_animation;
    } else {
        GAMECHARACTERDATA_s *character = object->apiobj.character_data->game_character;
        if ((reinterpret_cast<u8 *>(character)[0x92] & 0x10) != 0) {
            packet.requested_animation = 3;
        } else if ((object->id == id_STAP || object->id == id_STAP2 || object->id == id_JUMBOHOMINGDROID) &&
                   object->pad_gamepad->input_magnitude > 0.0f &&
                   object->apiobj.character_model->model_data_b[0] != NULL) {
            packet.requested_animation = 0;
        } else {
            packet.requested_animation = 1;
        }
    }

    if (WORLD->current_level == PLATFORM_LDATA && NeedsPretendAnim(object) != 0) {
        const u16 phase = static_cast<u16>(NuFmod(GameTimer.time_elapsed, 2.1f) / 2.1f * 65536.0f);
        object->render_offset.x = 0.1f * NU_SIN_LUT(phase);
        object->render_offset.y = 0.05f * NU_SIN_LUT(static_cast<u16>(phase + 0x2000));
        object->render_offset.z = 0.025f * NU_SIN_LUT(static_cast<u16>(phase ^ 0x8000));
        NuVecMtxRotate(&object->render_offset, &object->render_offset, &object->apiobj.field_0xb8);
    }
}

void Animate_SPEEDERBIKE(GameObject_s *object) {
    object->apiobj.anim_packet.requested_animation =
        (CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0
            ? object->context_animation
            : CHARACTER_ANIMATION_IDLE;
}

__attribute__((optimize("O2", "tree-vectorize"))) void Animate_BEAST(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    GAMEPAD_s *pad = object->pad_gamepad;

    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        packet.requested_animation = object->context_animation;
    } else {
        packet.requested_animation = CHARACTER_ANIMATION_FALL;
        if (object->character_context != CHARACTER_CONTEXT_DOOMED &&
            (object->apiobj.field_0x27d != 0 ||
             ((object->ground_contact_grace_timer > 0.0f ||
               object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL ||
               (object->fall_animation_timer < 0.2f && object->nearby_floor_distance != 2000000.0f &&
                object->nearby_floor_distance < 0.25f && object->apiobj.velocity.y < 0.0f)) &&
              !(static_cast<const GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->field_0x28 >
                    0.0f &&
                object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] != NULL)))) {
            packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
        }

        if (UseFallAnim(object)) {
            packet.requested_animation = CHARACTER_ANIMATION_FALL;
        } else if (packet.requested_animation != CHARACTER_ANIMATION_FALL &&
                   (object->pad_gamepad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 &&
                   pad->input_magnitude > 0.0f) {
            const bool has_walk = object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_WALK] != NULL;
            const bool has_run = object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_RUN] != NULL;
            if (!has_run) {
                if (has_walk) {
                    packet.requested_animation = CHARACTER_ANIMATION_WALK;
                }
            } else if (!has_walk) {
                packet.requested_animation = CHARACTER_ANIMATION_RUN;
            } else {
                const GAMECHARACTERDATA *character =
                    static_cast<const GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                const f32 threshold = (character->walk_speed + character->run_speed) * 0.5f;
                if (!(threshold >= object->pad_gamepad->input_magnitude)) {
                    packet.requested_animation = CHARACTER_ANIMATION_RUN;
                } else {
                    packet.requested_animation = CHARACTER_ANIMATION_WALK;
                }
            }
            if (packet.requested_animation == CHARACTER_ANIMATION_WALK && (object->field_0xe24 & 1) != 0) {
                packet.requested_animation = CHARACTER_ANIMATION_SABER_WALK;
            }
        }

        const GAMECHARACTERDATA *character =
            static_cast<const GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
        if (character->run_speed != character->walk_speed && character->run_speed != character->tiptoe_speed) {
            MoveAnim_Check(object);
        }
        if (packet.requested_animation == CHARACTER_ANIMATION_FALL &&
            object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL) {
            packet.requested_animation = CHARACTER_ANIMATION_IDLE;
        }
    }

    UpdateCharacterIdle(object);
    const i16 animation = packet.requested_animation;
    if (animation == CHARACTER_ANIMATION_FALL ||
        ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_HIGH_JUMP) != 0 &&
         (animation == CHARACTER_ANIMATION_FALL_VARIANT_75 || animation == CHARACTER_ANIMATION_FALL_VARIANT_40 ||
          animation == CHARACTER_ANIMATION_FALL_VARIANT_76))) {
        object->fall_animation_timer += FRAMETIME;
    } else {
        object->fall_animation_timer = 0.0f;
    }
}

void Animate_CANNON(GameObject_s *object) {
    object->apiobj.anim_packet.requested_animation =
        (CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0
            ? object->context_animation
            : CHARACTER_ANIMATION_IDLE;
}

__attribute__((optimize("O2", "tree-vectorize"))) void Animate_WEIRDO(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        packet.requested_animation = object->context_animation;
    } else {
        packet.requested_animation = CHARACTER_ANIMATION_FALL;
        if (object->character_context != CHARACTER_CONTEXT_DOOMED) {
            if (object->apiobj.field_0x27d != 0 ||
                ((object->ground_contact_grace_timer > 0.0f ||
                  object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL ||
                  (object->fall_animation_timer < 0.2f && object->nearby_floor_distance != 2000000.0f &&
                   object->nearby_floor_distance < 0.25f && object->apiobj.velocity.y < 0.0f)) &&
                 (!(object->apiobj.character_data->game_character->field_0x28 > 0.0f) ||
                  object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL))) {
                packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
            }
        }

        if (object->character_context == CHARACTER_CONTEXT_JUMP) {
            JumpAnimCode(object);
        } else if (UseFallAnim(object)) {
            packet.requested_animation = CHARACTER_ANIMATION_FALL;
        } else if (object->character_context == CHARACTER_CONTEXT_FORCE_PUSH &&
                   (object->action_flags & GAMEOBJECT_ACTION_FLAG_FORCE_PUSH_WEAPON_IDLE_MASK) == 0) {
            packet.requested_animation =
                object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_ALT_WEAPON_IDLE] != NULL
                    ? CHARACTER_ANIMATION_ALT_WEAPON_IDLE
                    : CHARACTER_ANIMATION_WEAPON_IDLE;
        } else if (object->character_context == CHARACTER_CONTEXT_FORCE_PUSH ||
                   object->character_context == CHARACTER_CONTEXT_FORCE_DEFLECT ||
                   object->character_context == CHARACTER_CONTEXT_FORCE_THROW ||
                   object->character_context == CHARACTER_CONTEXT_FORCE) {
            packet.requested_animation =
                ((object->field_0xe22 & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) != 0 || object->field_0xe32 == 1) &&
                        object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_ALT_WEAPON_IDLE] != NULL
                    ? CHARACTER_ANIMATION_ALT_WEAPON_IDLE
                    : CHARACTER_ANIMATION_WEAPON_IDLE;
        } else if (packet.requested_animation != CHARACTER_ANIMATION_FALL) {
            if ((object->pad_gamepad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 &&
                object->pad_gamepad->input_magnitude > 0.0f) {
                MoveAnim_Manage(object, object->pad_gamepad->input_magnitude, 1, 1);
            } else if ((object->field_0xe22 & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) != 0 || object->field_0xe32 == 1) {
                if (object->apiobj.character_data->game_character->field275_0x116 == 0 &&
                    (object->apiobj.character_data->model_flags & 0x80) != 0) {
                    if (object->apiobj.character_model->model_data_b[118] != NULL)
                        packet.requested_animation = 118;
                } else if (object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_ALT_IDLE] != NULL) {
                    packet.requested_animation = CHARACTER_ANIMATION_ALT_IDLE;
                }
            }
        }

        if (object->apiobj.character_data->game_character->field275_0x116 == 0 &&
            (object->apiobj.character_data->model_flags & 0x80) != 0) {
            if (packet.requested_animation == CHARACTER_ANIMATION_SABER_RUN)
                packet.requested_animation = CHARACTER_ANIMATION_EXTRA_RUN;
            else if (packet.requested_animation == CHARACTER_ANIMATION_SABER_WALK)
                packet.requested_animation = CHARACTER_ANIMATION_EXTRA_WALK;
        }
        MoveAnim_Check(object);
    }
    UpdateCharacterIdle(object);

    if (packet.requested_animation == CHARACTER_ANIMATION_FALL ||
        ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_HIGH_JUMP) != 0 &&
         (packet.requested_animation == CHARACTER_ANIMATION_FALL_VARIANT_75 ||
          packet.requested_animation == CHARACTER_ANIMATION_FALL_VARIANT_40 ||
          packet.requested_animation == CHARACTER_ANIMATION_FALL_VARIANT_76))) {
        object->fall_animation_timer += FRAMETIME;
    } else {
        object->fall_animation_timer = 0.0f;
    }

    const u16 weapon = static_cast<u16>(object->apiobj.character_data->game_character->weapon_model) & ~2u;
    if ((weapon == 101 || weapon == 105) && object->id != id_IMPERIALGUARD && object->weapon_scale > 0.0f) {
        const char *loop_sfx;
        if (object->id == id_BODYGUARD)
            loop_sfx = "Grv_GuardWeaponLp";
        else if ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_JEDI_BADDIE) != 0)
            loop_sfx = "SaberLoopB";
        else
            loop_sfx = "SaberLoopJ";
        const i32 sound = GetSfxId(loop_sfx);
        PlaySfxByIdAndSetVolume(sound, &object->apiobj.collision_position, object->weapon_scale);
    }
}

void Move_DRAGBOMB(GameObject_s *object) {
    DropInOutCode(object);
    if ((object->field_0xe20 & 0x20) != 0)
        return;
    ApplyGravity(object, NULL, 0.0f, 0.0f, NULL);
    if (WORLD != NULL && WORLD->area == GUNSHIP_ADATA && object->apiobj.velocity.y > 1.0f)
        object->apiobj.velocity.y = 1.0f;
    if (object->apiobj.supporting_platform_id != -1 ||
        (object->apiobj.collision_contact_mask != 0 &&
         (object->apiobj.collision_contact_mask &
          ~(player2 != NULL ? player->apiobj.collision_identity_mask | player2->apiobj.collision_identity_mask
                            : player->apiobj.collision_identity_mask)) != 0))
        KillGameObject(object, 2, 0);
    if (object->character_context == 0x34) {
        object->context_animation_timer += FRAMETIME;
        if (object->context_animation_timer > 10.0f)
            KillGameObject(object, 2, 1);
    }
}

void Move_DEFAULT(GameObject_s *object) {
    DropInOutCode(object);
    if ((object->field_0xe20 & 0x20) == 0)
        ApplyGravity(object, NULL, 0.0f, 0.0f, NULL);
}

void Animate_DEFAULT(GameObject_s *object) {
    object->apiobj.anim_packet.requested_animation = CHARACTER_ANIMATION_IDLE;
}

void Animate_JEDI(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;

    if ((object->field_0xe23 & GAMEOBJECT_E23_FLAG_FORCE_WEAPON_IDLE) != 0) {
        packet.requested_animation = JediWeaponIdle(object);
    } else if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        packet.requested_animation = object->context_animation;
    } else {
        packet.requested_animation = CHARACTER_ANIMATION_FALL;

        if (object->field_0x7a5 != CHARACTER_CONTEXT_DOOMED) {
            bool use_default_idle = object->apiobj.field_0x27d != 0;
            if (!use_default_idle) {
                const bool has_fall_animation =
                    object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] != NULL;
                // Original 0x16c978/0x16c9ce joins the character-data check
                // at 0x16c7bc even when the ground-contact timer has expired.
                if (object->ground_contact_grace_timer > 0.0f || !has_fall_animation ||
                    (object->fall_animation_timer < 0.2f && object->nearby_floor_distance != 2000000.0f &&
                     object->nearby_floor_distance < 0.25f && object->apiobj.velocity.y < 0.0f)) {
                    const GAMECHARACTERDATA *game_character =
                        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                    use_default_idle = !(game_character->field_0x28 > 0.0f) || !has_fall_animation;
                }
            }
            if (use_default_idle) {
                packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
            }
        }

        if (object->field_0x7a5 == CHARACTER_CONTEXT_JUMP) {
            JumpAnimCode(object);
        } else if (UseFallAnim(object)) {
            packet.requested_animation = CHARACTER_ANIMATION_FALL;
        } else if (object->field_0x7a5 == CHARACTER_CONTEXT_FORCE_PUSH) {
            if ((object->action_flags & GAMEOBJECT_ACTION_FLAG_FORCE_PUSH_WEAPON_IDLE_MASK) != 0) {
                packet.requested_animation = JediWeaponIdle(object);
            } else {
                packet.requested_animation =
                    object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_ALT_WEAPON_IDLE] != NULL
                        ? CHARACTER_ANIMATION_ALT_WEAPON_IDLE
                        : CHARACTER_ANIMATION_WEAPON_IDLE;
            }
        } else if (object->field_0x7a5 == CHARACTER_CONTEXT_FORCE_DEFLECT ||
                   object->field_0x7a5 == CHARACTER_CONTEXT_FORCE_THROW ||
                   object->field_0x7a5 == CHARACTER_CONTEXT_FORCE) {
            packet.requested_animation = JediWeaponIdle(object);
        } else if (packet.requested_animation != CHARACTER_ANIMATION_FALL) {
            GAMEPAD_s *pad = object->pad_gamepad;
            if ((pad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 && pad->input_magnitude > 0.0f) {
                if (object->id == id_YODA || object->id == id_YODAGHOST) {
                    packet.requested_animation = CHARACTER_ANIMATION_SABER_TIPTOE;
                } else {
                    const GAMECHARACTERDATA *game_character =
                        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                    const i32 allow_tiptoe =
                        (game_character->flags_090 & GAMECHARACTER_FLAG_DISABLE_TIPTOE) == 0 ? 1 : 0;
                    MoveAnim_Manage(object, pad->input_magnitude, allow_tiptoe, 1);
                }
            } else if (((object->field_0xe22 & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) != 0 ||
                        object->field_0xe32 == 1) &&
                       object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_ALT_IDLE] != NULL) {
                packet.requested_animation = CHARACTER_ANIMATION_ALT_IDLE;
            }
        }

        if (UsingExtraActionsFn != NULL && UsingExtraActionsFn(object) != 0) {
            switch (packet.requested_animation) {
                case CHARACTER_ANIMATION_WALK:
                    packet.requested_animation = CHARACTER_ANIMATION_EXTRA_WALK;
                    break;
                case CHARACTER_ANIMATION_IDLE:
                    packet.requested_animation = CHARACTER_ANIMATION_EXTRA_IDLE;
                    break;
                case 2:
                    break;
                case CHARACTER_ANIMATION_RUN:
                    packet.requested_animation = CHARACTER_ANIMATION_EXTRA_RUN;
                    break;
                case CHARACTER_ANIMATION_TIPTOE:
                    packet.requested_animation = CHARACTER_ANIMATION_EXTRA_TIPTOE;
                    break;
                case CHARACTER_ANIMATION_FALL:
                    packet.requested_animation = CHARACTER_ANIMATION_EXTRA_FALL;
                    break;
            }
        }
        MoveAnim_Check(object);
    }
    UpdateCharacterIdle(object);

    if (packet.requested_animation == CHARACTER_ANIMATION_FALL ||
        ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_HIGH_JUMP) != 0 &&
         (packet.requested_animation == CHARACTER_ANIMATION_FALL_VARIANT_75 ||
          packet.requested_animation == CHARACTER_ANIMATION_FALL_VARIANT_40 ||
          packet.requested_animation == CHARACTER_ANIMATION_FALL_VARIANT_76))) {
        object->fall_animation_timer += FRAMETIME;
    } else {
        object->fall_animation_timer = 0.0f;
    }

    if (object->id == id_IMPERIALGUARD || !(object->weapon_scale > 0.0f)) {
        return;
    }

    const char *loop_sfx;
    if (object->id == id_BODYGUARD) {
        loop_sfx = "Grv_GuardWeaponLp";
    } else if ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_JEDI_BADDIE) != 0) {
        loop_sfx = "SaberLoopB";
    } else {
        loop_sfx = "SaberLoopJ";
    }
    PlaySfxByIdAndSetVolume(GetSfxId(loop_sfx), &object->apiobj.collision_position, object->weapon_scale);
}

__attribute__((optimize("O2", "tree-vectorize"))) void Animate_ASTROMECH(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) == 0) {
        if (object->context_target_position != NULL) {
            if (object->apiobj.character_model->model_data_b[43] != NULL) {
                packet.requested_animation = 43;
            } else {
                packet.requested_animation =
                    object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL
                        ? CHARACTER_ANIMATION_IDLE
                        : CHARACTER_ANIMATION_FALL;
            }
        } else {
            packet.requested_animation = CHARACTER_ANIMATION_FALL;
            if (object->character_context != CHARACTER_CONTEXT_DOOMED) {
                if (object->apiobj.field_0x27d != 0) {
                    packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
                } else if (object->ground_contact_grace_timer > 0.0f ||
                           object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL ||
                           (object->fall_animation_timer < 0.2f && object->nearby_floor_distance != 2000000.0f &&
                            object->nearby_floor_distance < 0.25f && object->apiobj.velocity.y < 0.0f)) {
                    const GAMECHARACTERDATA *game_character =
                        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                    if (!(game_character->field_0x28 > 0.0f) ||
                        object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL) {
                        packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
                    }
                }
            }

            if (object->character_context == 0) {
                packet.requested_animation = object->pad_gamepad->input_magnitude > 0.0f ? 37 : 36;
            } else if (UseFallAnim(object)) {
                packet.requested_animation = CHARACTER_ANIMATION_FALL;
            } else if (packet.requested_animation != CHARACTER_ANIMATION_FALL &&
                       (object->pad_gamepad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 &&
                       object->pad_gamepad->input_magnitude > 0.0f) {
                packet.requested_animation = CHARACTER_ANIMATION_WALK;
            }
        }
    } else {
        packet.requested_animation = object->context_animation;
    }

    UpdateCharacterIdle(object);
    const i16 animation = packet.requested_animation;
    if (animation != CHARACTER_ANIMATION_FALL &&
        !((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_HIGH_JUMP) != 0 &&
          (animation == CHARACTER_ANIMATION_FALL_VARIANT_75 || animation == CHARACTER_ANIMATION_FALL_VARIANT_40 ||
           animation == CHARACTER_ANIMATION_FALL_VARIANT_76))) {
        object->fall_animation_timer = 0.0f;
    } else {
        object->fall_animation_timer += FRAMETIME;
    }
}

void PostAnimate_ASTROMECH(GameObject_s *object) {
    const i8 context = object->character_context;
    if (object->apiobj.field_0x287 == 0 && context == 0 && object->apiobj.model_draw_result != 0 &&
        object->apiobj.field_0x288 != 0) {
        if (object->field_0xe31 != 3 || (object->pad_gamepad->buttons_held & GAMEPAD_JUMP) != 0) {
            const i32 count = ParticlesPerFrame(object->field_0xe31 == 3 ? 0.25f : 1.0f, FRAMETIME);
            if (count > 0) {
                const i32 effect = object->id == id_R2Q5 ? 12 : 11;
                if (object->apiobj.character_model->points_of_interest[2] != NULL) {
                    AddGameDebrisRot(WORLD->debris_sys, effect, NUMTX_GET_ROW_VEC(&object->joint_matrices[2], 3), count,
                                     0x71c7, object->apiobj.facing_angle);
                }
                if (object->apiobj.character_model->points_of_interest[3] != NULL) {
                    AddGameDebrisRot(WORLD->debris_sys, effect, NUMTX_GET_ROW_VEC(&object->joint_matrices[3], 3), count,
                                     0x8e38, object->apiobj.facing_angle);
                }
            }
            PlaySfxAndSetVolume(const_cast<char *>("R2Thrust"), &object->apiobj.collision_position,
                                object->field_0xe31 == 3 ? 0.5f : 1.0f);
        }
    } else if (context == -1 && object->pad_gamepad->input_magnitude > 0.0f) {
        if (object->communicate_blend > 0.0f) {
            PlaySfx(const_cast<char *>("drd_r2_mvt_water_lp"), &object->apiobj.upper_position);
        } else if (object->apiobj.field_0x27d != 0) {
            PlaySfx(const_cast<char *>("R2Move"), &object->apiobj.collision_position);
        }
    }

    if (object->communicate_blend > 0.0f && object->communicate_blend < 1.0f) {
        const i32 effect = WORLD->debris_sys->entries[object->id == id_R2Q5 ? 1 : 3].effect;
        if (effect != -1) {
            const i32 count = ParticlesPerSecond(100.0f, FRAMETIME);
            if (count > 0) {
                const i32 locator = object->apiobj.character_data->game_character->thingy_locator;
                if (locator != -1) {
                    NUVEC_ALIGNED16 position = *NUMTX_GET_ROW_VEC(&object->joint_matrices[locator], 3);
                    if (object->apiobj.field_0x27f == 9 && object->apiobj.water_height > position.y) {
                        position.y = object->apiobj.water_height;
                    }
                    AddVariableShotDebrisEffect(effect, &position, count, 0, 0);
                }
            }
        }
    }
}

void SetProtocolDroidDeactivatedAction(GameObject_s *object) {
    if (object->field_0xe38 == 3)
        object->context_animation = 0x42;
    else if (object->field_0xe38 == 2)
        object->context_animation = 0x43;
    else if (object->field_0xe38 == 1)
        object->context_animation = 0x44;
}

static void DeactivatedCode(GameObject_s *object) {
    if (object->character_context == 0x41) {
        if (AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0) == NULL)
            return;
        object->airborne_action_duration += FRAMETIME;
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer <= 0.0f)
            object->character_context = -1;
        return;
    }
    if ((object->apiobj.character_data->model_flags & 0x20000000) != 0) {
        // Retail tests all eight player slots without a loop.
        i32 controlled = 0;
#define CHECK_CONTROLLING_PLAYER(i)                                                                                    \
    if (Player[i] != NULL && Player[i]->character_context == 0x51 && Player[i]->field_0x788 != NULL &&                 \
        static_cast<TECHNO *>(Player[i]->field_0x788)->controlled_object == object)                                    \
        controlled = 1;
        CHECK_CONTROLLING_PLAYER(0)
        CHECK_CONTROLLING_PLAYER(1)
        CHECK_CONTROLLING_PLAYER(2)
        CHECK_CONTROLLING_PLAYER(3)
        CHECK_CONTROLLING_PLAYER(4)
        CHECK_CONTROLLING_PLAYER(5)
        CHECK_CONTROLLING_PLAYER(6)
        CHECK_CONTROLLING_PLAYER(7)
#undef CHECK_CONTROLLING_PLAYER
        if (!controlled && object->field_0xcc0 == NULL && (object->apiobj.flags_low & 0x80) == 0) {
            if (object->character_context != 0x2b && object->character_context != 0x3e) {
                if (object->character_context != 0x17)
                    DeactivatePlayer(object, 1.0e9f, NULL);
                else
                    object->context_animation_timer = 1.0e9f;
            }
        } else {
            if (object->character_context == 0x17)
                ActivatePlayer(object);
        }
    }
    if (object->character_context != 0x17)
        return;
    if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL) {
        if (AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0) == NULL)
            return;
        float elapsed = object->airborne_action_duration + FRAMETIME;
        object->airborne_action_duration = elapsed;
        if (object->context_animation == 0x81) {
            if (AnimDuration(object->id, 0x81, 0.0f, 0.0f, 1) <= elapsed) {
                if (object->apiobj.character_model->model_data_b[0x41] == NULL) {
                    ActivatePlayer(object);
                    return;
                }
                object->context_animation = 0x41;
                if (object->context_animation_timer != 1.0e9f)
                    object->context_animation_timer -= object->airborne_action_duration;
                object->airborne_action_duration = 0.0f;
            }
        } else if ((static_cast<CHARACTERANIM_s *>(
                        object->apiobj.character_model->model_data_a[object->context_animation])
                        ->flags &
                    2) == 0 &&
                   AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1) <= elapsed) {
            ActivatePlayer(object);
            return;
        }
    } else {
        object->airborne_action_duration += FRAMETIME;
    }
    if (object->context_animation_timer != 1.0e9f && object->context_animation_timer < object->airborne_action_duration)
        ActivatePlayer(object);
    else if ((object->apiobj.character_data->model_flags & 0x20) != 0)
        SetProtocolDroidDeactivatedAction(object);
    if (!(object->field_0x768 > 0.0f))
        return;
    object->field_0x768 -= FRAMETIME;
    if (!(object->field_0x768 <= 0.0f))
        return;
    i32 kind;
    f32 duration;
    f32 range;
    if (object->action_movement_state == 2) {
        kind = 2;
        duration = DEACTIVATEDTIME;
        range = 9.0f;
    } else {
        if (!ForcePush_SuperMindTrick || object->action_movement_state != 1)
            return;
        kind = 1;
        duration = 5.0f;
        range = 1.0f;
    }
    GameObject_s *targets[10];
    i32 count = 0;
    GameObject_s *candidate = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT && count < 10; ++i, ++candidate) {
        GameObject_s *target = candidate;
        if (kind != 1) {
            if (!ZapTarget(target) || target == object || target->apiobj.field_0x27c != -1 ||
                (target->field_0xefb & 8) != 0 || CannotKill(target) ||
                (static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24)->flags_090 & 0x8040) !=
                    0 ||
                target->character_context == 0x0f || target->character_context == 0x3c ||
                target->character_context == 0x47 || target->character_context == 0x46)
                continue;
        } else {
            if ((target->apiobj.field_0x1f8 & 0x1001) != 0x1001 || target->apiobj.field_0x287 != 0 ||
                target->apiobj.field_0x27d == 0 || target == object || target->apiobj.field_0x27c != -1 ||
                (target->field_0xefb & 8) != 0 || CannotKill(target) ||
                target->apiobj.character_model->model_data_b[0x41] == NULL)
                continue;
            i8 context = target->character_context;
            if (context == 0x3c || context == 0x39 || context == 0x3b || context == 0x17 || context == 0x41 ||
                context == 0x47 || context == 0x0f || context == 0x46 ||
                (target->apiobj.character_data->model_flags & 0x10) != 0 ||
                (static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24)->flags_090 & 0x8000) !=
                    0 ||
                (CInfo[context].flags & 0x8000) != 0 ||
                (static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24)->flags_094[1] & 2) != 0)
                continue;
        }
        if (NuVecDistSqr(&target->apiobj.collision_position, &object->apiobj.collision_position, NULL) < range)
            targets[count++] = target;
    }
    i32 selected = count == 1 ? 0 : count > 1 ? qrand() / (0xffff / count + 1) : -1;
    if (selected == -1)
        return;
    GameObject_s *target = targets[selected];
    if (DeactivatePlayer(target, duration, NULL)) {
        target->action_movement_state = kind;
        target->context_variant_flags = (target->context_variant_flags & ~1) | (object->context_variant_flags & 1);
        if (kind == 2) {
            PlaySfx("R2Zap", &object->apiobj.collision_position);
            AddGameDebris(WORLD->debris_sys, (target->context_variant_flags & 1) != 0 ? 1 : 3,
                          &target->apiobj.collision_position);
            GameCam_HitJudder();
        }
        target->field_0x768 = qrand() * 1.5259022e-05f * 0.3f + 0.2f;
        if (target->id == id_JAWA)
            PlaySfx("Jawa_Dizzy", &target->apiobj.collision_position);
    }
}

void Move_BEAST(GameObject_s *object) {
    DropInOutCode(object);
    if ((object->field_0xe20 & 0x20) != 0)
        return;
    ApplyGravity(object, NULL, 0.0f, 8.0f, NULL);
    KeepWeaponOut(object);
    TakeHitCode(object);
    FloatCode(object);
    SlideCode(object);
    FlattenCode(object);
    ForcePushed_MoveCode(object);
    ForcedBackCode(object);
    Buck_MoveCode(object, 0);
    DeactivatedCode(object);
    if (object->character_context == -1 && object->field_0xe31 == 0 && object->apiobj.field_0x27d != 0 &&
        object->apiobj.field_0x27e == 0) {
        if ((object->movement_runtime_flags & 4) != 0 ||
            (object->fall_animation_timer >= 0.2f &&
             (object->pad_gamepad->input_magnitude == 0.0f ||
              (!object->apiobj.player_controlled && object->apiobj.character_model->model_data_b[0x59] != NULL))))
            StartFallLand(object, -1);
    }
    if (object->apiobj.field_0x27d != 0)
        object->movement_runtime_flags &= ~4;
    if (object->character_context == 0x3f) {
        object->context_animation_timer -= FRAMETIME;
        bool hit = false;
        if (object->context_animation_timer <= 0.0f) {
            object->character_context = -1;
            hit = (object->context_flags & 0x40) == 0;
        } else {
            f32 *frame = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
            if (frame != NULL) {
                f32 event = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
                hit = event >= 1.0f && event <= *frame;
            }
        }
        if (hit) {
            if (object->force_target != NULL)
                KillGameObject(object->force_target, 2, 0);
            object->field_0xe24 &= ~1;
            object->context_flags |= 0x40;
        }
    }
    GrabCode(object);
    PunchCode(object, GAMEPAD_ACTION & object->pad_gamepad->buttons_pressed, 0, 0, 1,
              object->apiobj.player_controlled ? 1.0f : 2.0f);
    PooCode(object);
    AwkwardShapeCode(object);
    GizmoBlowupCheckProximity(WORLD, object);
}

void Move_JEDI(GameObject_s *object) {
    GAMEPAD_s *pad = object->pad_gamepad;
    const struct {
        u32 action_mask, pressed, held, jump_mask, special_mask;
    } input = {GAMEPAD_ACTION, pad->buttons_pressed, pad->buttons_held, GAMEPAD_JUMP, GAMEPAD_SPECIAL};

    if (object->id == id_BODYGUARD) {
        KeepWeaponOut(object);
    }
    DropInOutCode(object);
    if ((object->field_0xe20 & GAMEOBJECT_E20_FLAG_MOVEMENT_DISABLED) != 0) {
        return;
    }

    ApplyGravity(object, NULL, 0.0f, 8.0f, NULL);
    i32 hit_effect;
    i32 glow_model;
    if (object->id == id_BOB) {
        if ((object->field_0xefd & 2) != 0) {
            glow_model = 0xdd;
            hit_effect = 2;
        } else {
            glow_model = 0xe1;
            hit_effect = 3;
        }
    } else if (AnakinGreenSabre(object)) {
        glow_model = 0xdd;
        hit_effect = 2;
    } else {
        hit_effect = object->blade_index == -1 ? -1 : BladeTab[object->blade_index].hit_effect;
        if (SuperWeirdo(object) && (object->apiobj.character_data->model_flags & 8) == 0) {
            glow_model = 0xdf;
        } else {
            glow_model = object->blade_index == -1 ? -1 : BladeTab[object->blade_index].glow_model;
        }
    }
    if (object->suit != NULL) {
        Signal_MoveCode(WORLD, object);
    }
    TakeHitCode(object);
    FloatCode(object);
    SlideCode(object);
    FlattenCode(object);
    Hang_MoveCode(object);
    Ledge_MoveCode(WORLD, object);
    if (LedgeTerrain_On != 0) {
        LedgeTerrain_MoveCode(object);
    }
    const i32 jump_pressed = input.pressed & input.jump_mask;
    Climb_MoveCode(object);
    TightRope_MoveCode(object, jump_pressed);
    ForcePushed_MoveCode(object);
    ForcedBackCode(object);
    Tube_MoveCode(object, WORLD);
    DeactivatedCode(object);
    PushCode(object, 1);
    BackFlipCode(object);
    if (object->id != id_GRIEVOUS && object->id != id_BODYGUARD) {
        TakeOverCode(object, object->pad_gamepad->buttons_pressed & GAMEPAD_TAG);
    }
    Glide_MoveCode(object);

    u32 jump_animations;
    if (object->id == id_IMPERIALGUARD)
        jump_animations = 0x10;
    else if (object->id == id_BODYGUARD)
        jump_animations = 0x10;
    else
        jump_animations = 0x1b;
    GAMECHARACTERDATA *game_character = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    if ((game_character->flags_090 & 0x00400000) != 0) {
        jump_animations |= 0x100;
    }
    const i32 action_pressed = input.pressed & input.action_mask;
    const i32 action_held = input.held & input.action_mask;
    const i32 special_pressed = input.pressed & input.special_mask;
    JumpCode(object, jump_pressed, input.held & input.jump_mask, jump_animations, action_pressed, action_held,
             hit_effect);

    GizPanel_MoveCode(WORLD, object, special_pressed);
    HatMachine_MoveCode(WORLD, object, special_pressed);
    ZipUp_MoveCode(object, special_pressed);
    BuildIt_MoveCode(object);
    Lever_MoveCode(WORLD, object);
    if ((static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_094[1] & 0x80) == 0) {
        ForceDeflectCode(object, special_pressed, input.held & input.special_mask);
        ForcePushCode(object, input.held & input.special_mask);
        FindForcePushTarget(object, special_pressed, 1);
        ForceThrowCode(object, special_pressed);
        ForceCode(object, special_pressed, input.held & input.special_mask);
        FindForcePushTarget(object, special_pressed, 2);
        if (object->apiobj.field_0x287 == 0 && FadeSys.fade == 0.0f) {
            if (object->force_glow_step > 0.0f)
                object->force_glow_step -= FRAMETIME;
            else
                ForceGlowCode(object, glow_model);
        } else
            ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    }
    if ((object->apiobj.character_data->model_flags & 0x00040000) != 0) {
        Teleport_MoveCode(object, special_pressed);
    }
    ThermalDetonator_MoveCode(object);
    if (object->suit != NULL && (static_cast<SUIT_s *>(object->suit)->store_flag & 4) != 0)
        Detonator_MoveCode(object);
    ComboRotateCode(object, action_held);
    WeaponOutCode(object);
    WeaponInCode(object);
    WeaponScalingCode(object);
    HoldCode(object);
    BlockCode(object, action_pressed, action_held, jump_pressed, 0);
    SwipeCode(object, action_pressed, action_held);
    LightSabreComboCode(object, action_pressed, action_held, jump_pressed, special_pressed);
    JediLightCode(object);
    if ((static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_090 & 0x8000) != 0 &&
        WORLD->debris_sys->entries[124].effect != -1)
        AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[124].effect, &object->apiobj.collision_position, 6,
                                          FRAMETIME, 0, 0, NULL);
    if (object->character_context == -1 && object->field_0xe31 == 0 && object->apiobj.field_0x27d != 0 &&
        object->apiobj.field_0x27e == 0) {
        if (!(object->fall_animation_timer >= 0.2f)) {
            if ((object->movement_runtime_flags & 4) != 0)
                StartFallLand(object, -1);
        } else if ((object->movement_runtime_flags & 4) != 0 || object->pad_gamepad->input_magnitude == 0.0f ||
                   ((object->apiobj.flags_low & 0x80) == 0 &&
                    object->apiobj.character_model->model_data_b[0x59] != NULL))
            StartFallLand(object, -1);
    }
    if (object->apiobj.field_0x27d != 0)
        object->movement_runtime_flags &= ~4;
    HeadMovement(object);
    CloakMovement(object);
    HairMovement(object);
    f32 force_volume = 0.0f;
    if ((object->character_context == 0x1b && object->field_0x7a3 == 1) || object->character_context == 0x1d ||
        object->character_context == 8) {
        force_volume = 1.0f;
    }
    object->force_use_volume = SeekLinearF(object->force_use_volume, force_volume, FRAMETIME);
    if (object->force_use_volume > 0.0f) {
        PlaySfxAndSetVolume("JForceUse", &object->apiobj.collision_position, object->force_use_volume);
    }
    if ((object->apiobj.flags_low & 0x80) != 0 && force_volume == 1.0f &&
        !Cheat_PowerUpActive(object->apiobj.field_0x27c)) {
        ConstantRumble(object, qrand() * 1.5259022e-05f * 0.5f, object->apiobj.field_0x289 * 0.3f);
    }
}

void Move_SPEEDERBIKE(GameObject_s *object) {
    GAMEPAD_s *pad = object->pad_gamepad;
    PreResetCode(object);
    KeepWeaponOut(object);
    DropInOutCode(object);
    if ((object->field_0xe20 & GAMEOBJECT_E20_FLAG_MOVEMENT_DISABLED) != 0)
        return;

    ApplyGravity(object, NULL, GetVehicleHoverHeight(object, NULL), 10.0f, NULL);
    GAMECHARACTERDATA *vehicle = object->apiobj.character_data->game_character;
    if (vehicle->field_0x28 > 0.0f && (vehicle->flags_090 & 0x10000) == 0 &&
        (WORLD->current_level != SPEEDERCHASEA_LDATA || disable_narrow_socks == 0)) {
        LoopCode(object, pad->buttons_pressed & GAMEPAD_JUMP,
                 pad->buttons_held & GAMEPAD_JUMP, pad, 1);
        TurnCode(object, 0, pad);
    }
    DeactivatedCode(object);
    if ((object->apiobj.character_data->model_flags & 0x10000000) != 0) {
        FireCode(object, pad->buttons_pressed & GAMEPAD_ACTION,
                 pad->buttons_held & GAMEPAD_ACTION, 0.2f, 0);
    }

    f32 speed = GetVehicleSpeedMul(object, pad->input_magnitude);
    f32 target = speed <= 1.0f ? speed * 0.75f + 0.75f : 1.5f;
    object->thrust_effect_scale =
        SeekValF(object->thrust_effect_scale, target, object->thrust_effect_scale < target ? 8.0f : 3.0f);
    object->reserved_e27[0] = static_cast<u8>(qrand() >> 8);
    object->reserved_e27[1] = static_cast<u8>(qrand() >> 8);
    object->reserved_e27[2] = static_cast<u8>(qrand() >> 8);
    object->reserved_e27[3] = static_cast<u8>(qrand() >> 8);

    if (WORLD->area != NULL && (WORLD->area->flags & 1) != 0 && object->id == id_MINISTARDESTROYER &&
        object->apiobj.player_controlled && (Cheat[29].enabled != 0 || object->field_0xdec > 0.0f)) {
        TractorBeamCode(object);
    }

    if (object->id == id_SPEEDERBIKE) {
        AwkwardShapeCode(object);
        if (object->current_hp <= 2) {
            const i32 effect = WORLD->debris_sys->entries[134].effect;
            if (effect != -1) {
                i32 count = ParticlesPerSecond(speeder_smoke_rate, FRAMETIME) * (3 - object->current_hp);
                while (count-- > 0) {
                    f32 random = static_cast<f32>(qrand()) * (1.0f / 65535.0f);
                    NUVEC position;
                    position.x = object->apiobj.collision_position.x +
                                 (object->apiobj.initial_position.x - object->apiobj.collision_position.x) * random;
                    position.y = object->apiobj.collision_position.y +
                                 (object->apiobj.initial_position.y - object->apiobj.collision_position.y) * random;
                    position.z = object->apiobj.collision_position.z +
                                 (object->apiobj.initial_position.z - object->apiobj.collision_position.z) * random;
                    AddVariableShotDebrisEffect(effect, &position, 1, 0, 0);
                }
            }
        }
        if (object->movement_spline != NULL) {
            {
                GameObject_s *player = Player[0];
                if (player != NULL && player->apiobj.player_controlled && player->field_0xcc0 != NULL &&
                    (object->apiobj.collision_identity_mask & player->apiobj.collision_contact_mask) != 0 &&
                    !(object->field_0x1024 > 0.0f)) {
                    NUVEC displacement;
                    displacement.x = player->apiobj.position.x - object->apiobj.position.x;
                    displacement.z = player->apiobj.position.z - object->apiobj.position.z;
                    NuVecRotateY(&displacement, &displacement, -object->apiobj.field_0x276);
                    if (displacement.x < 0.0f)
                        object->movement_spline_lateral_speed += speeder_offset_speed;
                    else
                        object->movement_spline_lateral_speed -= speeder_offset_speed;
                }
            }
            {
                GameObject_s *player = Player[1];
                if (player != NULL && player->apiobj.player_controlled && player->field_0xcc0 != NULL &&
                    (object->apiobj.collision_identity_mask & player->apiobj.collision_contact_mask) != 0 &&
                    !(object->field_0x1024 > 0.0f)) {
                    NUVEC displacement;
                    displacement.x = player->apiobj.position.x - object->apiobj.position.x;
                    displacement.z = player->apiobj.position.z - object->apiobj.position.z;
                    NuVecRotateY(&displacement, &displacement, -object->apiobj.field_0x276);
                    if (displacement.x < 0.0f)
                        object->movement_spline_lateral_speed += speeder_offset_speed;
                    else
                        object->movement_spline_lateral_speed -= speeder_offset_speed;
                }
            }
        }
    }
    EngineNoiseCode(object, 0);
}

void Move_CANNON(GameObject_s *object) {
    KeepWeaponOut(object);
    ApplyGravity(object, NULL,
                 object->character_context == 0x17 ? 0.0f : object->apiobj.character_data->game_character->field_0x28,
                 8.0f, NULL);
    DeactivatedCode(object);
    if ((object->field_0xef8 & 8) != 0) {
        if (object->id == id_CATAPULT || object->id == id_BASKETCANNON)
            ShootCode(object, (GAMEPAD_ACTION | GAMEPAD_SPECIAL) & object->pad_gamepad->buttons_pressed, 0, 0, 0, 0);
        FireCode(object, GAMEPAD_ACTION & object->pad_gamepad->buttons_pressed, 0, 0.4f, 1);
    }
    AwkwardShapeCode(object);
    if (object->pad_gamepad->input_magnitude > 0.0f &&
        abs(RotDiff(object->previous_movement_angle, object->apiobj.field_0x276)) > 910.0f * FRAMETIME) {
        if (object->id == id_CATAPULT || object->id == id_BASKETCANNON)
            PlaySfx("fly_paddle_rotate_lp", &object->apiobj.collision_position);
        else
            PlaySfx("R2Move", &object->apiobj.collision_position);
    }
}

void Move_WEIRDO(GameObject_s *object) {
    GAMEPAD_s *pad = object->pad_gamepad;
    u32 action_pressed = pad->buttons_pressed & GAMEPAD_ACTION;
    u32 action_held = pad->buttons_held & GAMEPAD_ACTION;
    u32 jump_pressed = pad->buttons_pressed & GAMEPAD_JUMP;
    u32 special_held = pad->buttons_held & GAMEPAD_SPECIAL;
    u32 jump_held = pad->buttons_held & GAMEPAD_JUMP;
    u32 special_pressed = pad->buttons_pressed & GAMEPAD_SPECIAL;
    u32 tag_pressed = pad->buttons_pressed & GAMEPAD_TAG;
    i32 hit_effect = object->blade_index == -1 ? -1 : BladeTab[object->blade_index].hit_effect;
    i32 glow_model;
    if (SuperWeirdo(object) && (object->apiobj.character_data->model_flags & 8) == 0)
        glow_model = 0xdf;
    else
        glow_model = object->blade_index == -1 ? -1 : BladeTab[object->blade_index].glow_model;
    DropInOutCode(object);
    ApplyGravity(object, NULL, 0.0f, 8.0f, NULL);
    u16 weapon = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->weapon_model & 0xfffd;
    i32 has_sabre = weapon == 0x65;
    if (!has_sabre)
        has_sabre = weapon == 0x69;
    if (object->suit != NULL)
        Signal_MoveCode(WORLD, object);
    TakeHitCode(object);
    FloatCode(object);
    SlideCode(object);
    FlattenCode(object);
    Hang_MoveCode(object);
    Ledge_MoveCode(WORLD, object);
    if (LedgeTerrain_On)
        LedgeTerrain_MoveCode(object);
    Climb_MoveCode(object);
    TightRope_MoveCode(object, jump_pressed);
    ForcePushed_MoveCode(object);
    ForcedBackCode(object);
    Tube_MoveCode(object, WORLD);
    DeactivatedCode(object);
    PushCode(object, 1);
    BackFlipCode(object);
    TakeOverCode(object, tag_pressed);
    Glide_MoveCode(object);
    u32 jump_animations;
    if (!has_sabre) {
        hit_effect = -1;
        jump_animations = SuperWeirdo(object) ? 0x109 : 0x208;
    } else
        jump_animations = SuperWeirdo(object) ? 0x11b : 0x1b;
    JumpCode(object, jump_pressed, jump_held, jump_animations, action_pressed, action_held, hit_effect);
    GizPanel_MoveCode(WORLD, object, special_pressed);
    HatMachine_MoveCode(WORLD, object, special_pressed);
    ZipUp_MoveCode(object, special_pressed);
    if ((object->apiobj.character_data->model_flags & 0x100000) != 0)
        Grapple_MoveCode(object);
    BuildIt_MoveCode(object);
    if (has_sabre || SuperWeirdo(object)) {
        ForceDeflectCode(object, special_pressed, special_held);
        ForcePushCode(object, special_held);
        FindForcePushTarget(object, special_pressed, 1);
        ForceThrowCode(object, special_pressed);
        ForceCode(object, special_pressed, special_held);
        FindForcePushTarget(object, special_pressed, 2);
        if (object->apiobj.field_0x287 == 0 && FadeSys.fade == 0.0f) {
            if (object->force_glow_step > 0.0f)
                object->force_glow_step -= FRAMETIME;
            else
                ForceGlowCode(object, glow_model);
        } else
            ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    }
    ForcePushed_MoveCode(object);
    Lever_MoveCode(WORLD, object);
    if ((object->apiobj.character_data->model_flags & 0x40000) != 0 || SuperWeirdo(object))
        Teleport_MoveCode(object, special_pressed);
    i32 detonator_used = ThermalDetonator_MoveCode(object);
    SuperCarry_MoveCode(WORLD, object);
    GizmoBlowupCheckProximity(WORLD, object);
    ComboRotateCode(object, action_held);
    WeaponOutCode(object);
    WeaponInCode(object);
    WeaponScalingCode(object);
    SpecialMove_VictimCode(object);
    HoldCode(object);
    Attracto_MoveCode(WORLD, object);
    SecurityDoor_MoveCode(WORLD, object);
    if (has_sabre) {
        BlockCode(object, action_pressed, action_held, jump_pressed, 0);
        SwipeCode(object, action_pressed, action_held);
        if (detonator_used == 1)
            special_pressed = 0;
        LightSabreComboCode(object, action_pressed, action_held, jump_pressed, special_pressed);
        JediLightCode(object);
    } else {
        BlockCode(object, action_pressed, action_held, 0, 0);
        PunchCode(object, action_pressed, action_held, 1, 0, 0.0f);
        DodgeCode(object, action_pressed, jump_pressed);
        if ((object->apiobj.character_data->model_flags & 0x80) != 0) {
            if (detonator_used == 1)
                special_pressed = 0;
            ShootCode(object, action_pressed, special_pressed, 1, 1, 0);
        } else if (object->batarang != NULL)
            Batarang_MoveCode(object);
    }
    if (static_cast<i8>(static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_094[0]) <
        0) {
        CommunicateCode(object, GAMEPAD_SPECIAL & object->pad_gamepad->buttons_pressed);
    }
    if (object->character_context == 0x14) {
        f32 *frame = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
        if (frame != NULL) {
            if ((object->field_0xe22 & 4) == 0) {
                f32 event = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
                if (event >= 1.0f && event <= *frame) {
                    object->field_0xe22 |= 4;
                    object->field_0xe20 |= 0x10;
                    PlaySfx("JgRocket", &object->apiobj.collision_position);
                }
            }
            object->context_animation_timer -= FRAMETIME;
            if (object->context_animation_timer <= 0.0f)
                object->character_context = -1;
        }
    } else if (object->character_context == -1 && (object->pad_gamepad->buttons_pressed & GAMEPAD_TAG) != 0 &&
               ((object->apiobj.character_data->model_flags & 0x1000000) != 0 || object->field_0x108e == 6)) {
        FireBountyHunterRocket(object);
    }
    if (object->character_context == -1 && object->field_0xe31 == 0 && object->apiobj.field_0x27d != 0 &&
        object->apiobj.field_0x27e == 0 &&
        ((object->movement_runtime_flags & 4) != 0 ||
         (object->fall_animation_timer >= 0.2f &&
          (object->pad_gamepad->input_magnitude == 0.0f ||
           (!object->apiobj.player_controlled && object->apiobj.character_model->model_data_b[0x59] != NULL)))))
        StartFallLand(object, -1);
    if (object->apiobj.field_0x27d != 0)
        object->movement_runtime_flags &= ~4;
    HeadMovement(object);
    CloakMovement(object);
    HairMovement(object);
}

void Move_CHARACTER(GameObject_s *object) {
    i32 jump = GAMEPAD_JUMP;
    i32 action = GAMEPAD_ACTION;
    i32 special = GAMEPAD_SPECIAL;
    i32 tag = GAMEPAD_TAG;
    GAMEPAD_s *pad = object->pad_gamepad;
    i32 pressed = pad->buttons_pressed;
    i32 held = pad->buttons_held;
    if (object->id == id_GAMORREANGUARD || object->id == id_EWOK || object->id == id_WICKET ||
        object->id == id_CAPTAINTARPALS)
        KeepWeaponOut(object);
    DropInOutCode(object);
    if (BonusArea && VehicleArea && (object->apiobj.character_data->model_flags & 0x4000000) != 0 &&
        object->torpedo != NULL)
        object->torpedo->count = 0;
    if ((object->field_0xe20 & 0x20) != 0)
        return;

    CHARACTERDATA *character = object->apiobj.character_data;
    GAMECHARACTERDATA *runtime = static_cast<GAMECHARACTERDATA *>(character->field11_0x24);
    bool jetpack_airborne = false;
    if (object->apiobj.player_controlled && runtime->uses_weapon_action == 2 && object->character_context != 0x2b &&
        object->field_0xe31 == 1) {
        f32 time = object->apiobj.horizontal_velocity_magnitude / runtime->movement_speed * FRAMETIME +
                   object->jump_variant_timer;
        object->jump_variant_timer = time;
        if (object->apiobj.field_0x27d != 0) {
            object->field_0xe31 = 0;
            object->character_context = -1;
        } else if (time >= runtime->field_0x48 || object->apiobj.field_0x281 == 0x1a) {
            StartJetPackFall(object, 0);
            character = object->apiobj.character_data;
        } else {
            object->apiobj.velocity.y = 0.0f;
            jetpack_airborne = true;
        }
    }
    if (!jetpack_airborne) {
        runtime = static_cast<GAMECHARACTERDATA *>(character->field11_0x24);
        if (runtime->uses_weapon_action == 2 && object->field_0xe31 == 1) {
            ApplyGravity(object, NULL, runtime->field_0x28, 8.0f, NULL);
        } else
            ApplyGravity(object, NULL, 0.0f, 8.0f, NULL);
    }
    if (object->suit != NULL)
        Signal_MoveCode(WORLD, object);
    TakeHitCode(object);
    FloatCode(object);
    SlideCode(object);
    FlattenCode(object);
    Hang_MoveCode(object);
    Ledge_MoveCode(WORLD, object);
    if (LedgeTerrain_On)
        LedgeTerrain_MoveCode(object);
    i32 jump_pressed = jump & pressed;
    Climb_MoveCode(object);
    TightRope_MoveCode(object, jump_pressed);
    ForcePushed_MoveCode(object);
    ForcedBackCode(object);
    Tube_MoveCode(object, WORLD);
    DeactivatedCode(object);
    PushCode(object, 1);
    BackFlipCode(object);
    if (object->id != id_SKELETON)
        TakeOverCode(object, tag & pressed);
    i32 action_pressed = action & pressed;
    i32 action_held = action & held;
    Glide_MoveCode(object);
    runtime = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    if (runtime->uses_weapon_action == 2 && object->field_0xe31 == 1) {
        JetPackCode(object, jump_pressed, 0, 0);
    } else if ((runtime->flags_094[0] & 4) == 0) {
        u32 animations = object->id == id_GAMORREANGUARD ? 0x20 : 8;
        if ((runtime->flags_090 & 0x400000) != 0)
            animations |= 0x101;
        JumpCode(object, jump_pressed, jump & held, animations, action_pressed, action_held, -1);
    }
    i32 special_pressed = special & pressed;
    GizPanel_MoveCode(WORLD, object, special_pressed);
    HatMachine_MoveCode(WORLD, object, special_pressed);
    ZipUp_MoveCode(object, special_pressed);
    if ((object->apiobj.character_data->model_flags & 0x100000) != 0)
        Grapple_MoveCode(object);
    BuildIt_MoveCode(object);
    Lever_MoveCode(WORLD, object);
    ThermalDetonator_MoveCode(object);
    if ((object->apiobj.character_data->model_flags & 0x40000) != 0)
        Teleport_MoveCode(object, special_pressed);
    SuperCarry_MoveCode(WORLD, object);
    GizmoBlowupCheckProximity(WORLD, object);
    WeaponOutCode(object);
    WeaponInCode(object);
    WeaponScalingCode(object);
    SpecialMove_VictimCode(object);
    HoldCode(object);
    BlockCode(object, action_pressed, action_held, 0, (object->apiobj.character_data->model_flags & 0x80) == 0);
    PunchCode(object, action_pressed, action_held, object->apiobj.character_data->model_flags & 0x80, 0, 0.0f);
    DodgeCode(object, action_pressed, jump_pressed);
    Attracto_MoveCode(WORLD, object);
    SecurityDoor_MoveCode(WORLD, object);
    if ((object->apiobj.character_data->model_flags & 0x80) != 0) {
        ShootCode(object, action_pressed, special_pressed, 1, 1, 0);
    } else if (object->batarang != NULL)
        Batarang_MoveCode(object);
    if (static_cast<i8>(static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_094[0]) <
        0) {
        CommunicateCode(object, GAMEPAD_SPECIAL & object->pad_gamepad->buttons_pressed);
    }
    if (object->character_context == 0x14) {
        f32 *frame = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
        if (frame != NULL) {
            if ((object->field_0xe22 & 4) == 0) {
                f32 event = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
                if (event >= 1.0f && event <= *frame) {
                    object->field_0xe22 |= 4;
                    object->field_0xe20 |= 0x10;
                    PlaySfx("JgRocket", &object->apiobj.collision_position);
                }
            }
            object->context_animation_timer -= FRAMETIME;
            if (object->context_animation_timer <= 0.0f)
                object->character_context = -1;
        }
    } else if (object->character_context == -1 && (object->pad_gamepad->buttons_pressed & GAMEPAD_TAG) != 0 &&
               ((object->apiobj.character_data->model_flags & 0x1000000) != 0 || object->field_0x108e == 6)) {
        FireBountyHunterRocket(object);
    }
    if (object->character_context == -1 && object->field_0xe31 == 0 && object->apiobj.field_0x27d != 0 &&
        object->apiobj.field_0x27e == 0) {
        if ((object->movement_runtime_flags & 4) != 0 ||
            (object->fall_animation_timer >= 0.2f &&
             (object->pad_gamepad->input_magnitude == 0.0f ||
              (!object->apiobj.player_controlled && object->apiobj.character_model->model_data_b[0x59] != NULL))))
            StartFallLand(object, -1);
    }
    if (object->apiobj.field_0x27d != 0)
        object->movement_runtime_flags &= ~4;
    if (object->character_context == 0x3f) {
        object->context_animation_timer -= FRAMETIME;
        i32 kill = 0;
        if (object->context_animation_timer > 0.0f) {
            f32 *frame = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
            if (frame != NULL) {
                f32 event = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
                if (event >= 1.0f && event <= *frame)
                    kill = 1;
            }
        } else {
            object->character_context = -1;
            if ((object->context_flags & 0x40) == 0)
                kill = 1;
        }
        if (kill) {
            if (object->force_target != NULL)
                KillGameObject(object->force_target, 2, 0);
            object->field_0xe24 &= ~1;
            object->context_flags |= 0x40;
        }
    }
    GrabCode(object);
    if (object->id == id_RANCOR)
        AwkwardShapeCode(object);
    HeadMovement(object);
    CloakMovement(object);
    HairMovement(object);
}

void SetProtocolDroidInterfaceAction(GameObject_s *object) {
    if (object->field_0xe38 == 3)
        object->context_animation = 0x45;
    else if (object->field_0xe38 == 2)
        object->context_animation = 0x46;
    else if (object->field_0xe38 == 1)
        object->context_animation = 0x47;
}

i32 SetProtocolDroidFallAnim(GameObject_s *object) {
    switch (object->field_0xe38) {
        case 1:
            return 76;
        case 2:
            return 75;
        case 3:
            return 40;
        default:
            return 5;
    }
}

void Animate_PROTOCOL(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        packet.requested_animation = object->context_animation;
        goto final_animation;
    }
    if (object->context_target_position != NULL) {
        packet.requested_animation = static_cast<i16>(SetProtocolDroidFallAnim(object));
        goto final_animation;
    }
    packet.requested_animation = CHARACTER_ANIMATION_FALL;
    if (object->character_context == CHARACTER_CONTEXT_DOOMED)
        goto falling;
    if (object->apiobj.field_0x27d != 0)
        goto idle;
    if (object->ground_contact_grace_timer > 0.0f)
        goto check_fall;
    if (object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL)
        goto check_fall;
    if (!(object->fall_animation_timer < 0.2f) || object->nearby_floor_distance == 2000000.0f ||
        !(object->nearby_floor_distance < 0.25f) || !(object->apiobj.velocity.y < 0.0f))
        goto falling;
check_fall:
    if (!(GetGameCharacterData(object)->field_0x28 <= 0.0f) &&
        object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] != NULL)
        goto falling;
idle:
    packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
    if (packet.requested_animation == CHARACTER_ANIMATION_FALL)
        goto falling;
    switch (object->field_0xe38) {
        case 1:
            packet.requested_animation = 8;
            break;
        case 2:
            packet.requested_animation = 20;
            break;
        case 3:
            packet.requested_animation = 15;
            break;
        default:
            packet.requested_animation = 1;
            break;
    }
    if (UseFallAnim(object)) {
        packet.requested_animation = CHARACTER_ANIMATION_FALL;
    } else if ((object->pad_gamepad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 &&
               object->pad_gamepad->input_magnitude > 0.0f) {
        switch (object->field_0xe38) {
            case 1:
                packet.requested_animation = 29;
                break;
            case 2:
                packet.requested_animation = 23;
                break;
            case 3:
                packet.requested_animation = 3;
                break;
            default:
                packet.requested_animation = 0;
                break;
        }
    }
    goto final_animation;
falling:
    packet.requested_animation = static_cast<i16>(SetProtocolDroidFallAnim(object));
    if (UseFallAnim(object))
        packet.requested_animation = CHARACTER_ANIMATION_FALL;
final_animation:
    if (packet.requested_animation == CHARACTER_ANIMATION_FALL)
        packet.requested_animation = static_cast<i16>(SetProtocolDroidFallAnim(object));
    UpdateCharacterIdle(object);
    const i16 animation = packet.requested_animation;
    if (animation != CHARACTER_ANIMATION_FALL &&
        !((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_HIGH_JUMP) != 0 &&
          (animation == CHARACTER_ANIMATION_FALL_VARIANT_75 || animation == CHARACTER_ANIMATION_FALL_VARIANT_40 ||
           animation == CHARACTER_ANIMATION_FALL_VARIANT_76))) {
        object->fall_animation_timer = 0.0f;
    } else {
        object->fall_animation_timer += FRAMETIME;
    }
}

void Animate_BATTLEDROID(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) == 0) {
        if (object->context_target_position != NULL) {
            if (object->apiobj.character_model->model_data_b[43] != NULL) {
                packet.requested_animation = 43;
            } else {
                if (object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL) {
                    packet.requested_animation = CHARACTER_ANIMATION_IDLE;
                } else {
                    packet.requested_animation = CHARACTER_ANIMATION_FALL;
                }
            }
        } else {
            packet.requested_animation = CHARACTER_ANIMATION_FALL;
            if (object->character_context == CHARACTER_CONTEXT_DOOMED)
                goto choose_movement;
            if (object->apiobj.field_0x27d != 0)
                goto default_idle;
            if (object->ground_contact_grace_timer > 0.0f ||
                object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL ||
                (object->fall_animation_timer < 0.2f && object->nearby_floor_distance != 2000000.0f &&
                 object->nearby_floor_distance < 0.25f && object->apiobj.velocity.y < 0.0f)) {
                if (!(GetGameCharacterData(object)->field_0x28 > 0.0f) ||
                    object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL)
                    goto default_idle;
            }
            goto choose_movement;
        default_idle:
            packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
        choose_movement:

            if (!UseFallAnim(object)) {
                if ((object->pad_gamepad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 &&
                    object->pad_gamepad->input_magnitude > 0.0f) {
                    const GAMECHARACTERDATA *game_character = GetGameCharacterData(object);
                    MoveAnim_Manage(object, object->pad_gamepad->input_magnitude,
                                    (game_character->flags_090 & GAMECHARACTER_FLAG_DISABLE_TIPTOE) == 0 ? 1 : 0, 0);
                }
            } else {
                packet.requested_animation = CHARACTER_ANIMATION_FALL;
            }
            MoveAnim_Check(object);
        }
    } else {
        packet.requested_animation = object->context_animation;
    }

    const i16 animation = packet.requested_animation;
    if (animation != CHARACTER_ANIMATION_FALL &&
        !((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_HIGH_JUMP) != 0 &&
          (animation == CHARACTER_ANIMATION_FALL_VARIANT_75 || animation == CHARACTER_ANIMATION_FALL_VARIANT_40 ||
           animation == CHARACTER_ANIMATION_FALL_VARIANT_76))) {
        object->fall_animation_timer = 0.0f;
    } else {
        object->fall_animation_timer += FRAMETIME;
    }
}

float FindGunshipHoverHeight(GameObject_s *object) {
    float height = 2.0f;
    if (object->apiobj.field_0x27c < 2) {
        float upper_height;
        float lower_height;
        if (WORLD->current_level == BONUS_GUNSHIPA_LDATA) {
            upper_height = 2.8f;
            lower_height = 1.2f;
        } else {
            upper_height = 2.05f;
            lower_height = 0.45f;
            height = 1.25f;
        }
        if (Player[0] != NULL && Player[0]->apiobj.player_controlled && Player[1] != NULL &&
            Player[1]->apiobj.player_controlled) {
            height = object != Player[0] ? lower_height : upper_height;
        }
    }
    return height;
}

void ApplyGravity_Network(GameObject_s *object) {
    CHARACTERDATA *character = object->apiobj.character_data;
    if (character->model_flags == 0x2000) {
        GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(character->field11_0x24);
        if ((data->flags_094[0] & 0x20) == 0) {
            if (data->field275_0x116 == 0x15) {
                ApplyGravity(object, NULL, FindGunshipHoverHeight(object), 8.0f, NULL);
            } else {
                float height;
                if (data->field_0x28 > 0.0f && ((height = GetVehicleHoverHeight(object, NULL)) != 0.0f ||
                                                getvehiclehoverheight_hothbattlehack != 0)) {
                    ApplyGravity(object, NULL, height, 10.0f, NULL);
                } else {
                    height =
                        object->character_context == 0x17
                            ? 0.0f
                            : static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->field_0x28;
                    ApplyGravity(object, NULL, height, 8.0f, NULL);
                }
            }
        }
    } else {
        if (object->apiobj.field_0x27d != 0 ||
            static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->field_0x48 <= object->jump_variant_timer ||
            object->apiobj.field_0x281 == 0x1a) {
            if (static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->field275_0x116 == 2 &&
                object->field_0xe31 == 1) {
                ApplyGravity(object, NULL, static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->field_0x28, 8.0f,
                             NULL);
            } else {
                ApplyGravity(object, NULL, 0.0f, 8.0f, NULL);
            }
        }
    }
}

void Animate_REPUBLICGUNSHIP(GameObject_s *object) {
    object->apiobj.anim_packet.requested_animation = CHARACTER_ANIMATION_IDLE;
    if (object->character_context == 0x23) {
        object->apiobj.anim_packet.requested_animation = 0x2c;
    } else if (object->character_context == 0x24) {
        object->apiobj.anim_packet.requested_animation = 0x2d;
    }
    UpdateCharacterIdle(object);
}

void Move_SUPERBATTLEDROID(GameObject_s *object) {
    DropInOutCode(object);
    if ((object->field_0xe20 & 0x20) != 0)
        return;
    ApplyGravity(object, NULL, 0.0f, 8.0f, NULL);
    TakeHitCode(object);
    FlattenCode(object);
    SlideCode(object);
    ForcePushed_MoveCode(object);
    ForcedBackCode(object);
    Tube_MoveCode(object, WORLD);
    DeactivatedCode(object);
    WeaponOutCode(object);
    WeaponInCode(object);
    WeaponScalingCode(object);
    ShootCode(object, GAMEPAD_ACTION & object->pad_gamepad->buttons_pressed,
              GAMEPAD_SPECIAL & object->pad_gamepad->buttons_pressed, 1, 0, 0);
    CheckFallLand(object);
    i32 special_pressed = GAMEPAD_SPECIAL & object->pad_gamepad->buttons_pressed;
    if ((object->apiobj.character_data->model_flags & 0x10) != 0 && (object->movement_runtime_flags & 2) == 0 &&
        Cheat_IsOn(0x20))
        SelfDestructCode(object, special_pressed);
    GizmoBlowupCheckProximity(WORLD, object);
}

__attribute__((optimize("O2", "tree-vectorize"))) void Animate_SUPERBATTLEDROID(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) == 0) {
        if (object->context_target_position != NULL) {
            if (object->apiobj.character_model->model_data_b[43] != NULL) {
                packet.requested_animation = 43;
            } else {
                const i32 target_state =
                    *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(object->context_target_position) + 0x14);
                packet.requested_animation = target_state == 0 ? CHARACTER_ANIMATION_IDLE : CHARACTER_ANIMATION_FALL;
            }
        } else {
            packet.requested_animation = CHARACTER_ANIMATION_FALL;
            if (object->character_context != CHARACTER_CONTEXT_DOOMED &&
                object->character_context != CHARACTER_CONTEXT_JUMP) {
                bool use_default_idle = object->apiobj.field_0x27d != 0;
                if (!use_default_idle) {
                    const bool has_fall =
                        object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] != NULL;
                    if (object->ground_contact_grace_timer > 0.0f || !has_fall ||
                        (object->fall_animation_timer < 0.2f && object->nearby_floor_distance != 2000000.0f &&
                         object->nearby_floor_distance < 0.25f && object->apiobj.velocity.y < 0.0f)) {
                        use_default_idle = !(GetGameCharacterData(object)->field_0x28 > 0.0f) || !has_fall;
                    }
                }
                if (use_default_idle) {
                    packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
                }
            }

            if (!UseFallAnim(object)) {
                if (packet.requested_animation != CHARACTER_ANIMATION_FALL) {
                    const GAMECHARACTERDATA *game_character = GetGameCharacterData(object);
                    const bool weapon_out = (object->field_0xe22 & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) != 0 ||
                                            object->weapon_scale > 0.0f;
                    if ((object->pad_gamepad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 &&
                        object->pad_gamepad->input_magnitude > 0.0f) {
                        const f32 run_threshold = (game_character->walk_speed + game_character->run_speed) * 0.5f;
                        if (object->pad_gamepad->input_magnitude <= run_threshold) {
                            packet.requested_animation =
                                weapon_out ? CHARACTER_ANIMATION_SABER_WALK : CHARACTER_ANIMATION_WALK;
                        } else {
                            packet.requested_animation =
                                weapon_out ? CHARACTER_ANIMATION_SABER_RUN : CHARACTER_ANIMATION_RUN;
                        }
                    } else if (object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_ALT_IDLE] != NULL &&
                               object->weapon_scale >= 0.5f) {
                        packet.requested_animation = CHARACTER_ANIMATION_ALT_IDLE;
                    }
                }
            } else {
                packet.requested_animation = CHARACTER_ANIMATION_FALL;
            }
            MoveAnim_Check(object);
        }
    } else {
        packet.requested_animation = object->context_animation;
    }

    UpdateCharacterIdle(object);
    const i16 animation = packet.requested_animation;
    if (animation != CHARACTER_ANIMATION_FALL &&
        !((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_HIGH_JUMP) != 0 &&
          (animation == CHARACTER_ANIMATION_FALL_VARIANT_75 || animation == CHARACTER_ANIMATION_FALL_VARIANT_40 ||
           animation == CHARACTER_ANIMATION_FALL_VARIANT_76))) {
        object->fall_animation_timer = 0.0f;
    } else {
        object->fall_animation_timer += FRAMETIME;
    }
}

void PeriscodeCode(GameObject_s *object) {
    if ((object->apiobj.character_data->model_flags & 0x40) == 0)
        return;

    f32 previous = object->communicate_blend;
    f32 target = 0.0f;
    if (object->apiobj.field_0x27f == 9 && object->apiobj.field_0x287 == 0 && WORLD->lev_objs[0x19].active != 0) {
        i32 locator = object->apiobj.character_data->game_character->thingy_locator;
        if (locator != -1 && object->apiobj.character_model->points_of_interest[locator] != NULL &&
            object->apiobj.water_height > object->joint_matrices[locator].m31) {
            if (object->apiobj.field_0x220 == 2000000.0f) {
                target = 1.0f;
            } else {
                f32 clearance = object->apiobj.field_0x220 - object->apiobj.water_height;
                if (!(clearance > 0.0f) || !(clearance < 0.26f))
                    target = 1.0f;
            }
        }
    }
    object->communicate_blend = SeekLinearF(previous, target, 5.0f * FRAMETIME);
    if (previous == 0.0f && object->communicate_blend > 0.0f)
        PlaySfx("drd_r2_scope_up", &object->apiobj.upper_position);
    else if (previous == 1.0f && object->communicate_blend < 1.0f)
        PlaySfx("drd_r2_scope_down", &object->apiobj.upper_position);

    if (object->field_0xdb0 > 0.0f) {
        object->field_0xdb0 -= FRAMETIME;
        if (!(object->field_0xdb0 <= 0.0f))
            return;
        if (object->apiobj.field_0x287 == 0)
            PlaySfx(const_cast<char *>(object->id == id_R2Q5 ? "R2Q501" : "R2D201"),
                    &object->apiobj.collision_position);
    }
    object->field_0xdb0 = qrand() * 1.5259022e-05f * 10.0f + 20.0f;
}

void Move_DROIDGENERIC(GameObject_s *object) {
    const i32 jump_held = GAMEPAD_JUMP & object->pad_gamepad->buttons_held;
    const i32 jump_pressed = GAMEPAD_JUMP & object->pad_gamepad->buttons_pressed;
    i32 zap_pressed = (GAMEPAD_ACTION | GAMEPAD_SPECIAL) & object->pad_gamepad->buttons_pressed;
    if ((object->apiobj.character_data->model_flags & 0x40) != 0) {
        if (Cheat[32].enabled != 0)
            zap_pressed = GAMEPAD_ACTION & object->pad_gamepad->buttons_pressed;
        KeepWeaponIn(object);
    } else if (object->apiobj.character_data->game_character->uses_weapon_action == 4 || object->id == id_PROBEDROID) {
        KeepWeaponOut(object);
    }
    DropInOutCode(object);
    ApplyGravity(object, NULL, 0.0f, object->id == id_BATTLEDROID ? 0.0f : 8.0f, NULL);
    if ((object->apiobj.character_data->model_flags & 0x10) != 0 || VehicleArea == 0) {
        TakeHitCode(object);
        if ((object->apiobj.character_data->model_flags & 0x40) != 0)
            FloatCode(object);
        SlideCode(object);
        FlattenCode(object);
        ForcePushed_MoveCode(object);
        ForcedBackCode(object);
        Tube_MoveCode(object, WORLD);
    }
    if ((object->apiobj.character_data->model_flags & 0x40) != 0) {
        JumpCode(object, jump_pressed, jump_held, 4, 0, 0, -1);
        GizPanel_MoveCode(WORLD, object, (GAMEPAD_SPECIAL | GAMEPAD_ACTION) & object->pad_gamepad->buttons_pressed);
    } else {
        Glide_MoveCode(object);
        if (object->id == id_SUPERBATTLEDROID) {
            WeaponOutCode(object);
            WeaponInCode(object);
            WeaponScalingCode(object);
            ShootCode(object, GAMEPAD_ACTION & object->pad_gamepad->buttons_pressed,
                      GAMEPAD_SPECIAL & object->pad_gamepad->buttons_pressed, 1, 0, 0);
        } else if (object->id == id_GONKDROID) {
            if (Cheat_IsOn(8)) {
                JumpCode(object, jump_pressed, jump_held, 0x40, 0, 0, -1);
                goto zap;
            }
        } else if ((object->apiobj.character_data->model_flags & 0x20) == 0 &&
                   (object->apiobj.character_data->game_character->flags_094[0] & 8) == 0) {
            ShootCode(object, GAMEPAD_ACTION & object->pad_gamepad->buttons_pressed, 0, 0, 0, 0);
        }
        JumpCode(object, 0, 0, 0x80, 0, 0, -1);
    }
zap:
    if (object->id == id_BUZZDROID)
        PunchCode(object, GAMEPAD_ACTION & object->pad_gamepad->buttons_pressed, 0, 0, 1, 0.0f);
    ZapCode(object, zap_pressed, 0);
    DeactivatedCode(object);
    if (object->character_context == -1 && object->field_0xe31 == 0 && object->apiobj.field_0x27d != 0 &&
        object->apiobj.field_0x27e == 0 &&
        ((object->movement_runtime_flags & 4) != 0 ||
         (object->fall_animation_timer >= 0.2f && (object->pad_gamepad->input_magnitude == 0.0f ||
                                                   (static_cast<i8>(object->apiobj.flags_low) >= 0 &&
                                                    object->apiobj.character_model->model_data_b[0x59] != NULL)))))
        StartFallLand(object, -1);
    if (object->apiobj.field_0x27d != 0)
        object->movement_runtime_flags &= ~4;
    if ((object->apiobj.character_data->model_flags & 0x10) != 0) {
        if ((object->apiobj.character_data->model_flags & 0x40) == 0) {
            if (VehicleArea == 0) {
                if (object->character_context == -1 && object->field_0xe31 == 0 && object->apiobj.field_0x27d != 0 &&
                    object->apiobj.field_0x27e == 0 &&
                    ((object->movement_runtime_flags & 4) != 0 ||
                     (object->fall_animation_timer >= 0.2f &&
                      (object->pad_gamepad->input_magnitude == 0.0f ||
                       (!object->apiobj.player_controlled &&
                        object->apiobj.character_model->model_data_b[0x59] != NULL)))))
                    StartFallLand(object, -1);
                if (object->apiobj.field_0x27d != 0)
                    object->movement_runtime_flags &= ~4;
            }
            if (object->id == id_NAFFDROID1 || object->id == id_NAFFDROID2 || object->id == id_NAFFDROID4 ||
                object->id == id_MOUSEDROID) {
                if (object->apiobj.field_0x27d != 0 && object->character_context == -1 &&
                    object->pad_gamepad->input_magnitude > 0.0f)
                    PlaySfx(const_cast<char *>(object->id == id_MOUSEDROID ? "drd_mousebot_lp" : "R2Move"),
                            &object->apiobj.collision_position);
            } else if (object->id == id_PROBEDROID) {
                PlaySfx("Probot_EngLp", &object->apiobj.collision_position);
                FireCode(object, GAMEPAD_ACTION & object->pad_gamepad->buttons_pressed,
                         GAMEPAD_ACTION & object->pad_gamepad->buttons_pressed, 0.5f, 1);
            }
        } else {
            PeriscodeCode(object);
        }
    }
    if (static_cast<i8>(object->apiobj.character_data->game_character->flags_094[0]) < 0)
        CommunicateCode(object, GAMEPAD_SPECIAL & object->pad_gamepad->buttons_pressed);
    if ((object->apiobj.character_data->model_flags & 0x20) != 0) {
        GizPanel_MoveCode(WORLD, object, (GAMEPAD_SPECIAL | GAMEPAD_ACTION) & object->pad_gamepad->buttons_pressed);
        if (WORLD->current_level == HOTHESCAPEB_LDATA)
            BigJumpCode(object);
    }
    const i32 special_pressed = GAMEPAD_SPECIAL & object->pad_gamepad->buttons_pressed;
    if ((object->apiobj.character_data->model_flags & 0x10) != 0 && (object->movement_runtime_flags & 2) == 0 &&
        Cheat_IsOn(0x20))
        SelfDestructCode(object, special_pressed);
    GizmoBlowupCheckProximity(WORLD, object);
}

void Move_WALKER(GameObject_s *object) {
    GAMEPAD_s *pad = object->pad_gamepad;
    KeepWeaponOut(object);
    DropInOutCode(object);
    if (object->torpedo != NULL)
        object->torpedo->count = 0;
    if ((object->field_0xe20 & 0x20) != 0)
        return;
    ApplyGravity(object, NULL, 0.0f, 0.0f, NULL);
    HeadMovement(object);
    if (object->character_context == 0x3d &&
        object->context_animation_timer > object->airborne_action_duration + 0.5f) {
        KillParts(object, -1, -1, 0, 0.0f, 0, NULL);
        KillGameObject(object, 2, 0);
    }
    TakeHitCode(object);
    SlideCode(object);
    DeactivatedCode(object);
    if (ShootCode(object, GAMEPAD_ACTION & pad->buttons_pressed, 0, 0, 0, 0))
        object->bolt_fire_phase = !object->bolt_fire_phase;
    if (object->character_context != 0x41 && object->character_context != 0x17)
        GameAudio_PlaySfxById(object->apiobj.character_data->game_character->sfx_engine,
                              &object->apiobj.collision_position, 0, 0);
    if (WORLD->current_level == ENDORBATTLEC_LDATA && object->apiobj.player_controlled) {
        NewTerrPlatformsOff();
        if (GameShadow(NULL, &object->apiobj.position, 5.0f, -1) != 2000000.0f && ShadowInfo() == 0x0e)
            object->apiobj.velocity.y = -3.0f;
    }
    GizmoBlowupCheckProximity(WORLD, object);
}

void Animate_WALKER(GameObject_s *object) {
    GAMEPAD_s *pad = object->pad_gamepad;
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        object->apiobj.anim_packet.requested_animation = object->context_animation;
        return;
    }

    object->apiobj.anim_packet.requested_animation = CHARACTER_ANIMATION_IDLE;
    if ((pad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 && pad->input_magnitude > 0.0f) {
        object->apiobj.anim_packet.requested_animation = CHARACTER_ANIMATION_WALK;
    }
}

void Move_HOVERDROID(GameObject_s *object) {
    DropInOutCode(object);
    FlattenCode(object);
    if ((object->field_0xe20 & 0x20) != 0)
        return;
    ApplyGravity(object, NULL,
                 object->character_context == 0x17 ? 0.0f : object->apiobj.character_data->game_character->field_0x28,
                 8.0f, NULL);
    Tube_MoveCode(object, WORLD);
    DeactivatedCode(object);
    FireCode(object, GAMEPAD_ACTION & object->pad_gamepad->buttons_pressed,
             GAMEPAD_ACTION & object->pad_gamepad->buttons_held, 0.75f, 1);
    if (object->pad_gamepad->input_magnitude > 0.0f && object->character_context != 0x17)
        GameAudio_PlaySfxById(object->apiobj.character_data->game_character->sfx_engine,
                              &object->apiobj.collision_position, 0, 0);
    i32 special_pressed = GAMEPAD_SPECIAL & object->pad_gamepad->buttons_pressed;
    if ((object->apiobj.character_data->model_flags & 0x10) != 0 && (object->movement_runtime_flags & 2) == 0 &&
        Cheat_IsOn(0x20))
        SelfDestructCode(object, special_pressed);
    GizmoBlowupCheckProximity(WORLD, object);
}

void Animate_HOVERDROID(GameObject_s *object) {
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        object->apiobj.anim_packet.requested_animation = object->context_animation;
    } else if (object->character_context == 0x1e &&
               object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] != NULL) {
        object->apiobj.anim_packet.requested_animation = CHARACTER_ANIMATION_FALL;
    } else {
        object->apiobj.anim_packet.requested_animation = CHARACTER_ANIMATION_IDLE;
    }
}

void Move_CRITTER(GameObject_s *object) {
    if ((object->field_0xe20 & 0x20) != 0)
        return;
    if (object->character_context == 0x17) {
        ApplyGravity(object, NULL, 0.0f, 8.0f, NULL);
    } else if (object->id == id_SNAKE && object->apiobj.field_0x27f == 9) {
        applygravity_extrahoveroffset = -0.15f;
        ApplyGravity(object, NULL, 0.01f, 8.0f, NULL);
    } else {
        if (object->id == id_BAT && object->apiobj.field_0x27f == 3)
            applygravity_extrahoveroffset = 0.2f;
        ApplyGravity(object, NULL, object->apiobj.character_data->game_character->field_0x28, 8.0f, NULL);
    }
    FlattenCode(object);
    if (object->id != id_BAT)
        SlideCode(object);
    ForcePushed_MoveCode(object);
    ForcedBackCode(object);
    Tube_MoveCode(object, WORLD);
    DeactivatedCode(object);
    CheckFallLand(object);
    if (object->id == id_SNAKE) {
        if (object->snake_body == NULL)
            CreateSnakeBody(object, 11);
        if (object->snake_body != NULL)
            UpdateSnakeBody(object);
    }
    GizmoBlowupCheckProximity(WORLD, object);
}

void Animate_CRITTER(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        packet.requested_animation = object->context_animation;
        return;
    }

    if (object->character_context == 30) {
        if (object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] != NULL) {
            packet.requested_animation = CHARACTER_ANIMATION_FALL;
            return;
        }
    }
    packet.requested_animation = CHARACTER_ANIMATION_FALL;

    if (object->character_context != CHARACTER_CONTEXT_DOOMED) {
        if (object->apiobj.field_0x27d != 0 ||
            ((object->ground_contact_grace_timer > 0.0f ||
              object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL ||
              (object->fall_animation_timer < 0.2f && object->nearby_floor_distance != 2000000.0f &&
               object->nearby_floor_distance < 0.25f && object->apiobj.velocity.y < 0.0f)) &&
             (!(static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->field_0x28 > 0.0f) ||
              object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL))) {
            packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
        }
    }

    if (!UseFallAnim(object)) {
        if (packet.requested_animation != CHARACTER_ANIMATION_FALL &&
            (object->pad_gamepad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 &&
            object->pad_gamepad->input_magnitude > 0.0f) {
            const bool has_walk = object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_WALK] != NULL;
            const bool has_run = object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_RUN] != NULL;
            if (has_run) {
                if (has_walk) {
                    const GAMECHARACTERDATA *game_character =
                        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                    const f32 run_threshold = (game_character->walk_speed + game_character->run_speed) * 0.5f;
                    packet.requested_animation = run_threshold >= object->pad_gamepad->input_magnitude
                                                     ? CHARACTER_ANIMATION_WALK
                                                     : CHARACTER_ANIMATION_RUN;
                } else {
                    packet.requested_animation = CHARACTER_ANIMATION_RUN;
                }
            } else if (has_walk) {
                packet.requested_animation = CHARACTER_ANIMATION_WALK;
            }
        }
    } else {
        packet.requested_animation = CHARACTER_ANIMATION_FALL;
    }
    MoveAnim_Check(object);
}

void Move_BARMAN(GameObject_s *object) {
    DropInOutCode(object);
    ApplyGravity(object, NULL, 0.0f, 0.0f, NULL);
}

void Animate_BARMAN(GameObject_s *object) {
    GAMEPAD_s *pad = object->pad_gamepad;
    object->apiobj.anim_packet.requested_animation = CHARACTER_ANIMATION_IDLE;
    if ((pad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 && pad->input_magnitude > 0.0f) {
        object->apiobj.anim_packet.requested_animation = CHARACTER_ANIMATION_WALK;
    }
    UpdateCharacterIdle(object);
}

void Move_JAWA(GameObject_s *object) {
    u32 pressed = object->pad_gamepad->buttons_pressed;
    u32 tag_mask = GAMEPAD_TAG;
    u32 action_mask = GAMEPAD_ACTION;
    u32 held = object->pad_gamepad->buttons_held;
    u32 jump_mask = GAMEPAD_JUMP;
    u32 special_mask = GAMEPAD_SPECIAL;
    DropInOutCode(object);
    if ((object->field_0xe20 & 0x20) != 0)
        return;
    ApplyGravity(object, NULL, 0.0f, 8.0f, NULL);
    TakeHitCode(object);
    FlattenCode(object);
    SlideCode(object);
    ForcePushed_MoveCode(object);
    ForcedBackCode(object);
    Tube_MoveCode(object, WORLD);
    DeactivatedCode(object);
    PushCode(object, 1);
    TakeOverCode(object, pressed & tag_mask);
    if (object->apiobj.player_controlled)
        ForceCode(object, 0, 0);
    JumpCode(object, pressed & jump_mask, held & jump_mask, 0, 0, 0, -1);
    BuildIt_MoveCode(object);
    u32 special_pressed = pressed & special_mask;
    if ((object->apiobj.character_data->model_flags & 0x40000) != 0)
        Teleport_MoveCode(object, special_pressed);
    ZipUp_MoveCode(object, special_pressed);
    if ((object->apiobj.character_data->model_flags & 0x100000) != 0)
        Grapple_MoveCode(object);
    Lever_MoveCode(WORLD, object);
    ThermalDetonator_MoveCode(object);
    HatMachine_MoveCode(WORLD, object, special_pressed);
    GizPanel_MoveCode(WORLD, object, special_pressed);
    WeaponOutCode(object);
    WeaponInCode(object);
    WeaponScalingCode(object);
    ZapCode(object, pressed & action_mask, special_pressed);
    CheckFallLand(object);
    HeadMovement(object);
    CloakMovement(object);
    ChatterSfx(object, 0, 0.0f);
}

void Move_ATAT(GameObject_s *object) {
    GAMEPAD_s *pad = object->pad_gamepad;
    KeepWeaponOut(object);
    DropInOutCode(object);
    if (object->torpedo != NULL)
        object->torpedo->count = 0;
    if ((object->field_0xe20 & 0x20) != 0)
        return;
    ApplyGravity(object, NULL, 0.0f, 0.0f, NULL);
    TakeHitCode(object);
    DeactivatedCode(object);
    if (ShootCode(object, GAMEPAD_ACTION & pad->buttons_pressed, 0, 0, 0, 0))
        object->bolt_fire_phase = !object->bolt_fire_phase;
    AwkwardShapeCode(object);
    HeadMovement(object);
    if (CurrentAnim(&object->apiobj.anim_packet) == 0 && object->character_context != 0x17 &&
        object->character_context != 0x41)
        GameAudio_PlaySfxById(object->apiobj.character_data->game_character->sfx_engine,
                              &object->apiobj.collision_position, 0, 0);
    GizmoBlowupCheckProximity(WORLD, object);
}

void Animate_ATAT(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        packet.requested_animation = object->context_animation;
    } else {
        packet.requested_animation = CHARACTER_ANIMATION_IDLE;
        if (object->apiobj.player_controlled && object->apiobj.character_model->model_data_b[15] != NULL) {
            packet.requested_animation = 15;
        }
        if ((object->pad_gamepad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 &&
            object->pad_gamepad->input_magnitude > 0.0f) {
            packet.requested_animation = CHARACTER_ANIMATION_WALK;
        }
    }

    if (packet.requested_animation != CHARACTER_ANIMATION_IDLE) {
        return;
    }

    const i32 turn = RotDiff(object->previous_movement_angle, object->apiobj.field_0x276);
    if (turn < 0 && object->apiobj.character_model->model_data_b[38] != NULL) {
        packet.requested_animation = 38;
    } else if (turn > 0 && object->apiobj.character_model->model_data_b[79] != NULL) {
        packet.requested_animation = 79;
    }
}

void Move_DROIDEKA(GameObject_s *object) {
    const u32 buttons_pressed = object->pad_gamepad->buttons_pressed;
    const i32 action_pressed = GAMEPAD_ACTION & buttons_pressed;
    const i32 special_pressed = GAMEPAD_SPECIAL & buttons_pressed;

    KeepWeaponOut(object);
    DropInOutCode(object);
    if (object->torpedo != NULL) {
        *reinterpret_cast<u8 *>(object->torpedo) = 0;
    }
    if ((object->field_0xe20 & 0x20) != 0) {
        return;
    }

    ApplyGravity(object, NULL, 0.0f, 0.0f, NULL);
    TakeHitCode(object);
    FlattenCode(object);
    SlideCode(object);
    ForcePushed_MoveCode(object);
    ForcedBackCode(object);
    Tube_MoveCode(object, WORLD);
    DeactivatedCode(object);
    ShootCode(object, action_pressed, special_pressed, 0, 0, 0);

    if (object->character_context == 0x32) {
        if (object->pad_gamepad->input_magnitude >= object->apiobj.character_data->game_character->run_speed - 0.1f) {
            object->character_context = 0x31;
            object->context_animation = 3;
        } else {
            object->context_animation_timer -= FRAMETIME;
            if (object->context_animation_timer <= 0.0f) {
                object->character_context = -1;
            }
        }
    } else if (object->character_context == 0x31) {
        PlaySfx("DDekaRoll", &object->apiobj.collision_position);
        if (AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 0, 0) != NULL &&
            object->pad_gamepad->input_magnitude == 0.0f && object->apiobj.field_0x27d != 0) {
            ResetAnimPacket(&object->apiobj.anim_packet, -1);
            if (object->apiobj.character_model->model_data_b[0x23] == NULL) {
                object->character_context = -1;
            } else {
                object->character_context = 0x32;
                object->context_animation = 0x23;
                object->context_animation_timer = AnimDuration(object->id, 0x23, 0.0f, 0.0f, 1);
                PlaySfx("DDekaFold", &object->apiobj.collision_position);
            }
        }
    } else if (object->character_context == -1 && object->apiobj.field_0x27d != 0) {
        if (object->pad_gamepad->input_magnitude >= object->apiobj.character_data->game_character->run_speed - 0.1f) {
            object->character_context = 0x31;
            object->context_animation = 3;
        } else if (CurrentAnim(&object->apiobj.anim_packet) == 5 &&
                   object->apiobj.character_model->model_data_b[0x23] != NULL) {
            object->character_context = 0x32;
            object->context_animation = 0x23;
            object->context_animation_timer = AnimDuration(object->id, 0x23, 0.0f, 0.0f, 1);
            PlaySfx("DDekaFold", &object->apiobj.collision_position);
        }
    }

    ShieldCode(object);
    if (object->character_context == -1 && object->field_0xe31 == 0 && object->apiobj.field_0x27d != 0 &&
        object->apiobj.field_0x27e == 0 &&
        (object->fall_animation_timer >= 0.2f || (object->movement_runtime_flags & 4) != 0) &&
        ((object->movement_runtime_flags & 4) != 0 || object->pad_gamepad->input_magnitude == 0.0f ||
         (static_cast<i8>(object->apiobj.flags_low) >= 0 &&
          object->apiobj.character_model->model_data_b[0x59] != NULL))) {
        StartFallLand(object, -1);
    }
    if (object->apiobj.field_0x27d != 0) {
        object->movement_runtime_flags &= ~4;
    }

    if ((object->apiobj.character_data->model_flags & 0x10) != 0 && (object->movement_runtime_flags & 2) == 0 &&
        Cheat_IsOn(0x20)) {
        SelfDestructCode(object, special_pressed);
    }
    GizmoBlowupCheckProximity(WORLD, object);
}

void Animate_DROIDEKA(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        packet.requested_animation = object->context_animation;
    } else {
        packet.requested_animation = CHARACTER_ANIMATION_FALL;
        if (object->character_context != CHARACTER_CONTEXT_DOOMED) {
            if (object->apiobj.field_0x27d != 0) {
                packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
            } else if (object->ground_contact_grace_timer > 0.0f) {
                const GAMECHARACTERDATA *game_character =
                    static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                if (!(game_character->field_0x28 > 0.0f) ||
                    object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL) {
                    packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
                }
            } else if (object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL ||
                       (object->fall_animation_timer < 0.2f && object->nearby_floor_distance != 2000000.0f &&
                        object->nearby_floor_distance < 0.25f && object->apiobj.velocity.y < 0.0f)) {
                const GAMECHARACTERDATA *game_character =
                    static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                if (!(game_character->field_0x28 > 0.0f) ||
                    object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL) {
                    packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
                }
            }
        }

        if (!UseFallAnim(object) && !(object->character_context == -1 && object->apiobj.field_0x27d == 0)) {
            if ((object->pad_gamepad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 &&
                object->pad_gamepad->input_magnitude > 0.0f) {
                const GAMECHARACTERDATA *game_character = GetGameCharacterData(object);
                const f32 walk_threshold = (game_character->tiptoe_speed + game_character->walk_speed) * 0.5f;
                packet.requested_animation = !(object->pad_gamepad->input_magnitude <= walk_threshold)
                                                 ? CHARACTER_ANIMATION_WALK
                                                 : CHARACTER_ANIMATION_TIPTOE;
            }
        } else {
            packet.requested_animation = CHARACTER_ANIMATION_FALL;
        }
        MoveAnim_Check(object);
    }

    if (packet.requested_animation != CHARACTER_ANIMATION_FALL &&
        !((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_HIGH_JUMP) != 0 &&
          (packet.requested_animation == CHARACTER_ANIMATION_FALL_VARIANT_75 ||
           packet.requested_animation == CHARACTER_ANIMATION_FALL_VARIANT_40 ||
           packet.requested_animation == CHARACTER_ANIMATION_FALL_VARIANT_76))) {
        object->fall_animation_timer = 0.0f;
    } else {
        object->fall_animation_timer += FRAMETIME;
    }
}

void Move_GEONOSIAN(GameObject_s *object) {
    u32 pressed = object->pad_gamepad->buttons_pressed;
    u32 jump_mask = GAMEPAD_JUMP;
    u32 action_mask = GAMEPAD_ACTION;
    u32 special_mask = GAMEPAD_SPECIAL;
    KeepWeaponOut(object);
    DropInOutCode(object);
    if ((object->field_0xe20 & 0x20) != 0)
        return;
    ApplyGravity(object, NULL,
                 object->field_0xe31 == 1 ? object->apiobj.character_data->game_character->field_0x28 : 0.0f, 8.0f,
                 NULL);
    TakeHitCode(object);
    FloatCode(object);
    SlideCode(object);
    FlattenCode(object);
    ForcePushed_MoveCode(object);
    ForcedBackCode(object);
    Tube_MoveCode(object, WORLD);
    DeactivatedCode(object);
    PushCode(object, 1);
    TakeOverCode(object, GAMEPAD_TAG & object->pad_gamepad->buttons_pressed);
    JetPackCode(object, pressed & jump_mask, 0, 0);
    JumpCode(object, 0, 0, 0x80, 0, 0, -1);
    u32 special_pressed = pressed & special_mask;
    GizPanel_MoveCode(WORLD, object, special_pressed);
    HatMachine_MoveCode(WORLD, object, special_pressed);
    Lever_MoveCode(WORLD, object);
    if ((object->apiobj.character_data->model_flags & 0x40000) != 0)
        Teleport_MoveCode(object, special_pressed);
    GizmoBlowupCheckProximity(WORLD, object);
    SpecialMove_VictimCode(object);
    if ((object->apiobj.character_data->model_flags & 0x80) != 0 &&
        (object->apiobj.character_data->game_character->flags_094[0] & 8) == 0)
        ShootCode(object, pressed & action_mask, special_pressed, 0, 1, 0);
    else if (object->id == id_WATTO)
        ZapCode(object, pressed & action_mask, 0);
    if (static_cast<i8>(object->apiobj.character_data->game_character->flags_094[0]) < 0)
        CommunicateCode(object, special_pressed);
    CheckFallLand(object);
    HeadMovement(object);
    CloakMovement(object);
    HairMovement(object);
    if (object->field_0xe31 == 1)
        PlaySfx("GeonWingsLp", &object->apiobj.collision_position);
}

void Animate_GEONOSIAN(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((CInfo[object->character_context].flags & CHARACTER_CONTEXT_INFO_FLAG_OWNS_ANIMATION) != 0) {
        packet.requested_animation = object->context_animation;
    } else {
        packet.requested_animation = CHARACTER_ANIMATION_FALL;
        if (object->character_context != CHARACTER_CONTEXT_DOOMED &&
            (object->apiobj.field_0x27d != 0 ||
             ((object->ground_contact_grace_timer > 0.0f ||
               object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] == NULL ||
               (object->fall_animation_timer < 0.2f && object->nearby_floor_distance != 2000000.0f &&
                object->nearby_floor_distance < 0.25f && object->apiobj.velocity.y < 0.0f)) &&
              !(static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->field_0x28 > 0.0f &&
                object->apiobj.character_model->model_data_b[CHARACTER_ANIMATION_FALL] != NULL)))) {
            packet.requested_animation = static_cast<i16>(GetDefaultIdle(object));
        }

        if (UseFallAnim(object)) {
            packet.requested_animation = CHARACTER_ANIMATION_FALL;
        } else if (object->character_context == -1 && object->field_0xe31 == 1) {
            packet.requested_animation = object->pad_gamepad->input_magnitude > 0.0f ? 37 : 15;
        } else if (packet.requested_animation != CHARACTER_ANIMATION_FALL &&
                   (object->pad_gamepad->allocated_5a & GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT) == 0 &&
                   object->pad_gamepad->input_magnitude > 0.0f) {
            packet.requested_animation = CHARACTER_ANIMATION_WALK;
        }
        MoveAnim_Check(object);
    }

    UpdateCharacterIdle(object);
    const i16 animation = packet.requested_animation;
    f32 fall_timer = 0.0f;
    if (animation == CHARACTER_ANIMATION_FALL ||
        ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_HIGH_JUMP) != 0 &&
         (animation == CHARACTER_ANIMATION_FALL_VARIANT_40 || animation == CHARACTER_ANIMATION_FALL_VARIANT_75 ||
          animation == CHARACTER_ANIMATION_FALL_VARIANT_76))) {
        fall_timer = object->fall_animation_timer + FRAMETIME;
    }
    object->fall_animation_timer = fall_timer;
}

void CatchUpCode(GameObject_s *object, f32 divisor, f32 maximum, i32 mode) {
    if (static_cast<u8>(object->apiobj.field_0x27c) == 0xff) {
        return;
    }

    const i32 socket_index = static_cast<i8>(object->field_0x661);
    object->field_0xc38 = 0.0f;
    if (socket_index == -1 || WORLD->sock_sys == NULL) {
        return;
    }
    if (mode != 0 && !object->apiobj.player_controlled) {
        return;
    }

    const bool looping = WORLD->sock_sys->sock[socket_index].looping != 0;
    GameObject_s *other;
    if (Player[0] == object) {
        other = Player[1];
    } else if (Player[1] == object) {
        other = Player[0];
    } else {
        return;
    }

    if (other != NULL && (!object->apiobj.player_controlled || other->apiobj.player_controlled) &&
        static_cast<i8>(other->field_0x661) == socket_index && (mode == 0 || other->apiobj.player_controlled)) {
        f32 distance = object->sock_position.normalized_distance - other->sock_position.normalized_distance;
        if (looping) {
            if (!(distance >= 0.5f)) {
                if (-0.5f >= distance) {
                    distance += 1.0f;
                }
            } else {
                distance -= 1.0f;
            }
        }
        if (distance < 0.0f) {
            const f32 catchup = -distance / divisor;
            maximum = MIN(maximum, catchup);
            object->field_0xc38 = maximum;
        }
    }
}

void PodLoseSpeed(GameObject_s *object, i32 hit, i32 rumble) {
    if (hit != 0) {
        ObjHitObj(NULL, object, 1, 0, 0, 1);
    }
    if (hit != 0 || rumble != 0) {
        TakeHitRumble(object, 0.7f);
    }
    object->current_speed_mul *= 0.5f;
    if (object->current_speed_mul < 0.333f) {
        object->current_speed_mul = 0.333f;
    }
}

void PodCollisionCode(GameObject_s *object) {
    static f32 magdif;

    if (GamePlayTimer.time_elapsed < 1.0f) {
        return;
    }
    if (object->apiobj.field_0x27c != -1 && object->apiobj.player_controlled && object->field_0x1084 != 0 &&
        object->contact_normal.y <= 0.574f && object->contact_normal.y >= -0.574f) {
        if (PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA) {
            AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[PODSPRINTDEB].effect,
                                              &object->contact_position, 50, FRAMETIME, 0, 0, NULL);
            if (qrand() <= 0x7fff) {
                NewBuzzFrames(object->pad_gamepad->pad, 1, 0);
            }
            return;
        }

        magdif = (1.0f / object->pre_terrain_speed) * object->post_terrain_speed;
        if (1.0f > magdif) {
            if (0.999f > magdif) {
                ObjHitObj(NULL, object, 1, 0, 0, 1);
            }
            PodLoseSpeed(object, 0, 0);
        }
    }
}

f32 FindPodHoverHeight(GameObject_s *object) {
    static NUVEC podsprintlifthackpos[3] = {
        {-395.26f, -5.48f, -165.08f},
        {-375.38f, -5.04f, -286.5f},
        {-354.91f, -4.85f, -299.32f},
    };

    f32 height = 0.15f;
    if (object == Player[1] && Player[0]->apiobj.player_controlled) {
        f32 distance_squared = NuVecDistSqr(&Player[0]->apiobj.position, &object->apiobj.position, NULL);
        if (distance_squared < 4.0f) {
            f32 distance = NuFsqrt(distance_squared);
            i32 angle = static_cast<i32>((1.0f - distance * 0.5f) * 16384.0f);
            height += NuTrigTable[(angle >> 1) & 0x7fff] * 0.5f;
        }
    }

    if (PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA && object->apiobj.player_controlled) {
        for (i32 i = 0; i < 3; ++i) {
            if (NuVecXZDistSqr(&object->apiobj.position, &podsprintlifthackpos[i], NULL) < 2.0f) {
                height += 1.5f;
                break;
            }
        }
    }
    return height;
}

void Move_POD(GameObject_s *object) {
    AREADATA_s *area = WORLD->area;
    if (area == NULL || (area != PODRACE_ADATA && area != PODSPRINT_ADATA)) {
        Move_VEHICLE(object);
        return;
    }

    ApplyGravity(object, NULL, FindPodHoverHeight(object), 8.0f, NULL);

    GAMECHARACTERDATA *vehicle = object->apiobj.character_data->game_character;
    if (PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA && vehicle->field_0x28 > 0.0f &&
        PodSprint_InStartCountdown(WORLD) <= 0.0f) {
        LoopCode(object, object->pad_gamepad->buttons_pressed & GAMEPAD_JUMP,
                 object->pad_gamepad->buttons_held & GAMEPAD_JUMP, object->pad_gamepad, 0);
    }
    TakeHitCode(object);

    if ((object->apiobj.field_0x1f4 & 0x40000) != 0) {
        object->camera_shake_strength = 0.0f;
    } else if (object->apiobj.field_0x27c != -1) {
        f32 target_speed;
        f32 rate = 3.0f * FRAMETIME;
        if (WORLD->area == PODRACE_ADATA) {
            if ((object->pad_gamepad->buttons_held & GAMEPAD_JUMP) != 0) {
                target_speed = 1.0f + object->camera_shake_strength;
                rate = 5.0f * FRAMETIME;
            } else if (FadeSys.fade == 0.0f && MiniCutCam == 0) {
                target_speed = 0.333f + 1.667f * object->camera_shake_strength;
            } else if (podrace_section == -1) {
                target_speed = 0.333f + 1.667f * object->camera_shake_strength;
            } else {
                target_speed = 1.0f + object->camera_shake_strength;
            }
        } else {
            target_speed = 1.0f + (1.4f - 1.0f) * object->camera_shake_strength;
        }
        object->current_speed_mul = SeekLinearF(object->current_speed_mul, target_speed, rate);

        if ((object->field_0xe20 & 0x20) == 0 && object->apiobj.field_0x281 == 0x14 &&
            object->apiobj.collision_min.y - object->apiobj.field_0x218 < PODBOOSTHEIGHT) {
            NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
            NewRumble(object->pad_gamepad->pad, 0.5f, 0);
            object->current_speed_mul = WORLD->area == PODRACE_ADATA ? 2.0f : 1.4f;
            object->camera_shake_strength = 1.0f;
            if ((object->field_0xe20 & 1) == 0) {
                PlaySfx("PodX_Booster", &object->apiobj.collision_position);
                object->field_0xe20 |= 1;
            }
        } else {
            object->field_0xe20 &= ~1;
            if (object->camera_shake_strength > 0.0f) {
                object->camera_shake_strength -= FRAMETIME;
                if (object->camera_shake_strength < 0.0f)
                    object->camera_shake_strength = 0.0f;
            }
        }
        if (object->current_speed_mul > 1.0f)
            NewRumble(object->pad_gamepad->pad, object->current_speed_mul - 1.0f, 0);
    }

    if (FreePlay != 0 && (object->apiobj.character_data->model_flags & 0x10000000) != 0 &&
        (vehicle->flags_094[0] & 8) == 0) {
        FireCode(object, object->pad_gamepad->buttons_pressed & GAMEPAD_ACTION,
                 object->pad_gamepad->buttons_held & GAMEPAD_ACTION, 0.15f, 0);
    }
    if (WORLD->area != NULL && (WORLD->area == PODRACE_ADATA || WORLD->area == PODSPRINT_ADATA))
        CatchUpCode(object, 0.025f, 3.0f, 0);
    if (static_cast<i8>(object->apiobj.flags_low) < 0)
        ForceCode(object, 0, 0);
    PodCollisionCode(object);

    i32 sound = vehicle->sfx_engine;
    if (vehicle->uses_weapon_action != 0x14) {
        f32 camera_facing = (object->apiobj.collision_position.x - global_camera.mtx.m30) * global_camera.mtx.m20 +
                            (object->apiobj.collision_position.y - global_camera.mtx.m31) * global_camera.mtx.m21 +
                            (object->apiobj.collision_position.z - global_camera.mtx.m32) * global_camera.mtx.m22;
        if (camera_facing < 0.0f)
            PlaySfxByIdAndSetPitch(sound, &object->apiobj.collision_position, 0.7f);
        else
            PlaySfxById(sound, &object->apiobj.collision_position);
    } else {
        if (static_cast<i8>(object->apiobj.flags_low) < 0) {
            i32 player_index = object == Player[0] ? 0 : 1;
            if (WORLD->area == PODRACE_ADATA) {
                f32 steering = -fabsf(NuTrigTable[object->pad_gamepad->input_angle >> 1]) *
                               (object->pad_gamepad->input_magnitude - 6.0f) / 96.0f;
                f32 speed = avg_currentspeed_mul < 1.1f ? avg_currentspeed_mul + 1.0f + steering : 2.0f + steering;
                f32 pitch = speed * 0.5f;
                f32 previous = PosSeekPitch[player_index];
                PosSeekPitch[player_index] = previous + (pitch - previous) * FRAMETIME * 1.5f;
                PlaySfxByIdAndSetPitch(sound, &object->apiobj.collision_position, PosSeekPitch[player_index]);
            } else {
                PosSeekPitch[player_index] =
                    SeekLinearF(PosSeekPitch[player_index],
                                object->apiobj.velocity_magnitude / vehicle->run_speed * 0.35f + 0.65f, FRAMETIME);
                PlaySfxByIdAndSetPitch(sound, &object->apiobj.collision_position, PosSeekPitch[player_index]);
            }
        }
    }
    GizmoBlowupCheckProximity(WORLD, object);
}

void Move_VEHICLE(GameObject_s *object) {
    GAMEPAD_s *pad = object->pad_gamepad;
    if (FreePlay != 0 && WORLD->area != NULL && WORLD->area == PODSPRINT_ADATA) {
        Move_POD(object);
        return;
    }

    KeepWeaponOut(object);
    DropInOutCode(object);
    if ((object->field_0xe20 & GAMEOBJECT_E20_FLAG_MOVEMENT_DISABLED) != 0) {
        return;
    }

    if ((object->apiobj.character_data->game_character->flags_094[0] & 0x20) == 0) {
        f32 hover_height;
        if (object->apiobj.character_data->game_character->uses_weapon_action == 0x15) {
            ApplyGravity(object, NULL, FindGunshipHoverHeight(object), 8.0f, NULL);
        } else if (object->apiobj.character_data->game_character->field_0x28 > 0.0f &&
                   ((hover_height = GetVehicleHoverHeight(object, NULL)) != 0.0f ||
                    getvehiclehoverheight_hothbattlehack != 0)) {
            ApplyGravity(object, NULL, hover_height, 10.0f, NULL);
        } else {
            hover_height =
                object->character_context != 0x17 ? object->apiobj.character_data->game_character->field_0x28 : 0.0f;
            ApplyGravity(object, NULL, hover_height, 8.0f, NULL);
        }
    }

    DeactivatedCode(object);

    if (object->id == id_SNOWMOB) {
        Buck_MoveCode(object, pad->buttons_pressed & (GAMEPAD_SPECIAL | GAMEPAD_ACTION));
    } else if (object->id == id_ZAMSSPEEDER && WORLD->current_level == BOUNTYHUNTERPURSUITE_LDATA &&
               object->current_hp < static_cast<i32>(object->hitpoints)) {
        const i32 effect = WORLD->debris_sys->entries[134].effect;
        if (effect != -1) {
            i32 count = ParticlesPerSecond(zam_smoke_rate, FRAMETIME) *
                        ((static_cast<i32>(object->hitpoints) - static_cast<i32>(object->current_hp)) / 2);
            if (count > 0) {
                do {
                    const f32 random = static_cast<f32>(qrand()) * (1.0f / 65535.0f);
                    NUVEC position = {
                        object->apiobj.collision_position.x +
                            (object->apiobj.initial_position.x - object->apiobj.collision_position.x) * random,
                        object->apiobj.collision_position.y +
                            (object->apiobj.initial_position.y - object->apiobj.collision_position.y) * random,
                        object->apiobj.collision_position.z +
                            (object->apiobj.initial_position.z - object->apiobj.collision_position.z) * random,
                    };
                    AddVariableShotDebrisEffect(effect, &position, 1, 0, 0);
                    count--;
                } while (count != 0);
            }
        }
    }

    if (object->apiobj.character_data->game_character->field_0x28 > 0.0f &&
        (object->apiobj.character_data->game_character->flags_090 & 0x10000) == 0) {
        LoopCode(object, pad->buttons_pressed & GAMEPAD_JUMP, pad->buttons_held & GAMEPAD_JUMP, pad, 1);
        TurnCode(object, 0, pad);
        DisorientateCode(object, NULL, 225.0f);
    }

    if ((object->apiobj.character_data->model_flags & 0x10000000) != 0 &&
        (object->apiobj.character_data->game_character->flags_094[0] & 8) == 0) {
        if (object->id == id_STAP || object->id == id_STAP2) {
            if (__builtin_expect(object->apiobj.character_model->model_data_b[0x16] == NULL, 1)) {
                FireCode(object, pad->buttons_pressed & GAMEPAD_ACTION, pad->buttons_held & GAMEPAD_ACTION, 0.25f, 1);
            } else {
                ShootCode(object, object->pad_gamepad->buttons_pressed & GAMEPAD_ACTION, 0, 0, 0, 1);
            }
        } else {
            FireCode(object, pad->buttons_pressed & GAMEPAD_ACTION, pad->buttons_held & GAMEPAD_ACTION, 0.15f, 0);
        }
    }
    if (__builtin_expect(WORLD->current_level == DOGFIGHTA_LDATA, 0)) {
        CatchUpCode(object, 0.01f, 2.0f, 0);
    }
    if (__builtin_expect((object->apiobj.character_data->game_character->flags_090 & 0x400) != 0, 1)) {
        CableCode(object, pad->buttons_pressed & GAMEPAD_SPECIAL, 0.5f);
    }
    if (WORLD->area != NULL && (WORLD->area->flags & 1) != 0 && object->apiobj.player_controlled &&
        (Cheat[29].enabled != 0 || object->field_0xdec > 0.0f)) {
        TractorBeamCode(object);
    }

    const f32 speed_multiplier = GetVehicleSpeedMul(object, object->pad_gamepad->input_magnitude);
    const f32 thrust_target = !(speed_multiplier > 1.0f) ? speed_multiplier * 0.75f + 0.75f : 1.5f;
    object->thrust_effect_scale =
        SeekValF(object->thrust_effect_scale, thrust_target, object->thrust_effect_scale < thrust_target ? 8.0f : 3.0f);
    object->reserved_e27[0] = static_cast<u8>(qrand() >> 8);
    object->reserved_e27[1] = static_cast<u8>(qrand() >> 8);
    object->reserved_e27[2] = static_cast<u8>(qrand() >> 8);
    object->reserved_e27[3] = static_cast<u8>(qrand() >> 8);

    if (object->character_context == 0x2c) {
        f32 hover_height;
        if (DEATHSTARBATTLE_ADATA != NULL && WORLD->area == DEATHSTARBATTLE_ADATA) {
            hover_height = -1.5f;
        } else if (ANAKINSFLIGHT_ADATA != NULL && WORLD->area == ANAKINSFLIGHT_ADATA && object->ai.locator != NULL) {
            hover_height = object->ai.locator->position.y + test_hoverheight;
        } else {
            hover_height = GetHoverPosY(object);
            if (__builtin_expect(hover_height == 2000000.0f, 1)) {
                hover_height = 0.0f;
            }
        }

        u16 x_rotation;
        object->field_0x1086 = 0;
        if (object->field_0x7a3 == 0) {
            if (object->apiobj.position.y >= hover_height) {
                object->field_0x7a3 = 1;
            } else {
                object->apiobj.roll_angle = 0;
                object->apiobj.pitch_angle = 0xc000;
                object->apiobj.velocity.x = 0.0f;
                object->apiobj.velocity.y = 0.0f;
                object->apiobj.velocity.z = 0.5f * object->apiobj.character_data->game_character->run_speed;
                x_rotation = 0xc000;
                goto rotate_flight_velocity;
            }
        }
        if (object->field_0x7a3 == 1) {
            object->apiobj.pitch_angle = TurnRot(object->apiobj.pitch_angle, 0, 0x10000, NULL);
            object->apiobj.roll_angle = 0;
            object->apiobj.velocity.x = 0.0f;
            object->apiobj.velocity.y = 0.0f;
            object->apiobj.velocity.z = 0.5f * object->apiobj.character_data->game_character->run_speed;
            x_rotation = object->apiobj.pitch_angle;
            if (object->apiobj.pitch_angle == 0) {
                object->field_0x7a3 = 2;
                object->context_destination.y = object->apiobj.position.y;
                object->context_animation_timer = 0.0f;
            }
        } else if (object->field_0x7a3 == 2) {
            object->context_animation_timer += FRAMETIME;
            f32 remaining;
            f32 flight_time = object->context_animation_timer;
            i32 phase;
            if (!(object->context_animation_timer >= 1.0f)) {
                remaining = 1.0f - object->context_animation_timer;
                phase = (static_cast<i32>(32768.0f * object->context_animation_timer + 16384.0f) >> 1) & 0x7fff;
            } else {
                object->character_context = -1;
                phase = 0x6000;
                remaining = 0.0f;
                flight_time = 1.0f;
            }
            object->apiobj.roll_angle = 0;
            object->apiobj.pitch_angle = 0;
            object->apiobj.velocity.x = 0.0f;
            object->apiobj.velocity.y = 0.0f;
            object->apiobj.velocity.z = flight_time * object->apiobj.character_data->game_character->run_speed +
                                        object->apiobj.character_data->game_character->run_speed * 0.5f * remaining;
            x_rotation = 0;
            const f32 blend = 1.0f - (1.0f + NuTrigTable[phase]) * 0.5f;
            object->apiobj.position.y = hover_height * blend + (1.0f - blend) * object->context_destination.y;
        } else {
            x_rotation = object->apiobj.pitch_angle;
            object->character_context = -1;
        }

    rotate_flight_velocity:
        NuVecRotateX(&object->apiobj.velocity, &object->apiobj.velocity, x_rotation);
        NuVecRotateY(&object->apiobj.velocity, &object->apiobj.velocity, object->apiobj.field_0x276);
    }

    FloatCode(object);
    TorpedoCode(object, pad->buttons_held & GAMEPAD_SPECIAL, 0.5f);

    if (object->id == id_ZAMSSPEEDER && WORLD->area != NULL && WORLD->area == BOUNTYHUNTERPURSUIT_ADATA) {
        ShieldCode(object);
    }

    if (!TouchHacks::CanUseVehicleSmartBomb(*object)) {
        object->combo_input_timer = 0.0f;
    } else if (object->combo_input_timer == 0.0f) {
        if ((object->pad_gamepad->buttons_pressed & GAMEPAD_SPECIAL) != 0) {
            object->combo_input_timer += FRAMETIME;
        }
    } else if ((object->pad_gamepad->buttons_held & GAMEPAD_SPECIAL) != 0) {
        object->combo_input_timer += FRAMETIME;
        if (object->combo_input_timer >= 2.0f) {
            object->combo_input_timer = 0.0f;
            TouchHacks().TriggerVehicleSmartBomb(*object);
        } else {
            TouchHacks().PlaySmartBombBuildupEffects(*object, object->combo_input_timer, 2.0f);
        }
    }

    AwkwardShapeCode(object);
    EngineNoiseCode(object, object->character_context == 0x17);

    if (((object->apiobj.player_controlled && object->current_hp == 1) ||
         ((object->apiobj.flags_low & 0x80) == 0 && object->current_hp <= 1)) &&
        object->apiobj.character_data->game_character->uses_weapon_action == 10 &&
        (object->apiobj.character_data->model_flags & 0x04000000) == 0) {
        const i32 effect = WORLD->debris_sys->entries[96].effect;
        if (effect != -1) {
            i32 count = ParticlesPerSecond(50.0f, FRAMETIME);
            if (count > 0) {
                do {
                    const f32 random = static_cast<f32>(qrand()) * (1.0f / 65535.0f);
                    NUVEC position = {
                        object->apiobj.collision_position.x +
                            (object->apiobj.initial_position.x - object->apiobj.collision_position.x) * random,
                        object->apiobj.collision_position.y +
                            (object->apiobj.initial_position.y - object->apiobj.collision_position.y) * random,
                        object->apiobj.collision_position.z +
                            (object->apiobj.initial_position.z - object->apiobj.collision_position.z) * random,
                    };
                    AddVariableShotDebrisEffect(effect, &position, 1, 0, 0);
                    count--;
                } while (count != 0);
            }
        }
    }

    if ((object->apiobj.character_data->game_character->flags_090 & 0x100000) != 0) {
        if (__builtin_expect(Cheat_IsOn(7) != 0, 1)) {
            if ((pad->buttons_pressed & (GAMEPAD_ACTION | GAMEPAD_SPECIAL)) != 0) {
                GameAudio_PlaySfx(0x4b, &object->apiobj.collision_position, 0, 0);
            }
        }
        NUJOINTANIM_s &joint = object->joint_modifiers[0];
        joint.rotation.x = 0.0f;
        joint.joint_index = 0;
        joint.flags = NUJOINTANIM_ROTATION;
        joint.rotation.y = -(static_cast<f32>(RotDiff(object->previous_movement_angle, object->apiobj.field_0x276)) *
                             0.000095873799f) /
                           (6.0f * FRAMETIME);
        joint.rotation.z = 0.0f;
        object->joint_modifiers[1] = joint;
        object->joint_modifiers[1].joint_index = 1;
        object->field_0x1089 = 2;
    }

    if (object->apiobj.field_0x27c == -1 && (object->apiobj.character_data->game_character->flags_090 & 0x40) != 0 &&
        object->apiobj.character_model->model_data_b[0x2a] != NULL &&
        (object->apiobj.character_data->game_character->flags_094[3] & 8) != 0) {
        Grapple_AddDynamic(object, 0);
    }

    if (object->id == id_DODGEM) {
        if (object->current_hp < 2) {
            const i32 effect = WORLD->debris_sys->entries[24].effect;
            if (effect != -1) {
                i32 count = ParticlesPerSecond(5.0f, FRAMETIME);
                if (count > 0) {
                    do {
                        const f32 random = static_cast<f32>(qrand()) * (1.0f / 65535.0f);
                        NUVEC position = {
                            object->apiobj.collision_position.x +
                                (object->apiobj.initial_position.x - object->apiobj.collision_position.x) * random,
                            object->apiobj.collision_position.y +
                                (object->apiobj.initial_position.y - object->apiobj.collision_position.y) * random,
                            object->apiobj.collision_position.z +
                                (object->apiobj.initial_position.z - object->apiobj.collision_position.z) * random,
                        };
                        AddVariableShotDebrisEffect(effect, &position, 1, 0, 0);
                        count--;
                    } while (count != 0);
                }
            }
        } else if (__builtin_expect(object->field_0xcc0 != NULL && object->apiobj.field_0x27c == -1, 0)) {
            GameObject_s *player = Player[0];
            if (player != NULL && static_cast<i8>(player->apiobj.flags_low) < 0 && player->field_0xcc0 != NULL &&
                ((object->apiobj.field_0x1e4 & player->apiobj.field_0x1ec) |
                 (object->apiobj.field_0x1e8 & player->apiobj.field_0x1f0)) != 0 &&
                object->field_0x1024 <= 0.0f) {
                ObjHitObj(player, object, 1, 1, 0, 1);
            }
            player = Player[1];
            if (player != NULL && player->apiobj.player_controlled && player->field_0xcc0 != NULL &&
                ((object->apiobj.field_0x1e4 & player->apiobj.field_0x1ec) |
                 (object->apiobj.field_0x1e8 & player->apiobj.field_0x1f0)) != 0 &&
                object->field_0x1024 <= 0.0f) {
                ObjHitObj(player, object, 1, 1, 0, 1);
            }
        }
    }

    if (GizmoBlowupCheckProximity(WORLD, object) != 0) {
        if (LEGO_SET_SLOWDOWNFN != NULL) {
            LEGO_SET_SLOWDOWNFN(object);
        }
    }
}

void Move_REPUBLICGUNSHIP(GameObject_s *object) {
    if (WORLD->area != NULL && (WORLD->area == GUNSHIP_ADATA || WORLD->area == BONUS_GUNSHIP_ADATA)) {
        GAMEPAD_s *pad = object->pad_gamepad;
        KeepWeaponOut(object);
        DropInOutCode(object);
        if ((object->field_0xe20 & 0x20) != 0)
            return;
        ApplyGravity(object, NULL, FindGunshipHoverHeight(object), 8.0f, NULL);
        TakeHitCode(object);
        FireCode(object, GAMEPAD_ACTION & object->pad_gamepad->buttons_pressed,
                 GAMEPAD_ACTION & object->pad_gamepad->buttons_held, 0.3f, 0);
        if ((object->apiobj.character_data->game_character->flags_090 & 0x400) != 0)
            CableCode(object, GAMEPAD_SPECIAL & pad->buttons_pressed, 0.5f);
    } else {
        Move_VEHICLE(object);
    }
}

void MovePlayer_NETWORK(GameObject_s *object) {
    APIOBJECT_s &api = object->apiobj;
    if ((api.character_data->model_flags & 0x2000) != 0)
        object->in_narrow_socket = ObjInNarrowSock(object, WORLD->sock_sys, WORLD->level_idx);

    f32 turn_override = 0.0f;
    if (api.player_controlled && object->character_context == 0x1b && (object->field_0xe21 & 2) == 0) {
        if (object->field_0x7a3 != 1)
            FaceOpponent(object, NULL);
        turn_override = 0.333f;
    }
    i32 direct_turn = object->character_context == 0x33 ? 1 : object->snap_facing;
    f32 turn_rate;
    if (SuperCarry_Carrying(object))
        turn_rate = object->field_0x768 * 8.0f;
    else if (object->character_context == 0x1c || object->character_context == 0x33)
        turn_rate = 8.0f;
    else if (turn_override != 0.0f)
        turn_rate = turn_override * 8.0f;
    else {
        turn_rate = static_cast<GAMECHARACTERDATA *>(api.character_data->field11_0x24)->turn_rate * 8.0f;
        if (object->id == id_ATAT && WORLD->current_level == SPEEDERCHASEA_LDATA)
            turn_rate += turn_rate;
    }

    if ((object->field_0xefd & 0x80) != 0 && (CInfo[object->character_context].flags & 1) == 0)
        api.movement_facing_angle += 0x8000;
    if ((object->field_0xefd & 4) != 0) {
        api.facing_angle = api.movement_facing_angle;
        api.field_0x276 = api.movement_facing_angle;
    } else if (direct_turn == 0 &&
               (turn_override != 0.0f || (CInfo[object->character_context].parameter & 2) != 0 ||
                (static_cast<GAMECHARACTERDATA *>(api.character_data->field11_0x24)->flags_090 & 0x100) != 0 ||
                SuperCarry_Carrying(object))) {
        if (object->character_context == 0x4b &&
            (static_cast<GAMECHARACTERDATA *>(api.character_data->field11_0x24)->flags_090 & 0x100) == 0)
            turn_rate *= 0.333f;
        api.facing_angle =
            TurnRot(api.facing_angle, api.movement_facing_angle, static_cast<i32>(turn_rate * 3.0f * 16384.0f), NULL);
        api.field_0x276 = SeekRot(api.field_0x276, api.facing_angle, 10.0f);
        api.movement_facing_angle = api.facing_angle;
    } else {
        api.facing_angle = SeekRot(api.facing_angle, api.movement_facing_angle, turn_rate);
        api.field_0x276 = api.facing_angle;
    }
    if (object->torpedo != NULL && (api.field_0x1f8 & 0x1000) != 0 && api.field_0x287 == 0 &&
        (object->field_0xe20 & 0x20) == 0)
        Torpedo_UpdateJobbies(object);
    SpecialMove_VictimCode(object);
    i32 glow_model;
    if (SuperWeirdo(object) && (api.character_data->model_flags & 8) == 0)
        glow_model = 0xdf;
    else {
        glow_model = object->blade_index == -1 ? 0xdf : BladeTab[object->blade_index].glow_model;
        if (glow_model == -1)
            glow_model = 0xdf;
    }
    Tag_Check(object);
    ForcePushCode(object, 0);
    object->field_0xd8c = 1.0f;
    if (api.field_0x287 == 0 && FadeSys.fade == 0.0f) {
        if (object->force_glow_step > 0.0f)
            object->force_glow_step -= FRAMETIME;
        else
            ForceGlowCode(object, glow_model);
    } else
        ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    if ((object->field_0xe20 & 0x20) == 0)
        TorpedoCode(object, 0, 0.0f);
    PeriscodeCode(object);
    MovePlayer_TWIST(object);
    if (PodLevel(WORLD->area)) {
        Move_POD(object);
        KeepOnScreen(object);
    }
    if (object->id == id_SNAKE) {
        if (object->field_0x10b8 == NULL)
            CreateSnakeBody(object, 0xb);
        if (object->field_0x10b8 != NULL)
            UpdateSnakeBody(object);
    }
    object->field_0xefd &= ~0x40;
    api.field_0x214 = api.field_0x218;
    api.start_position = api.position;
    api.initial_position = api.collision_position;
    api.field_0x27e = api.field_0x27d;
    object->previous_movement_angle = api.field_0x276;
    Teleport_NetMoveCode(object);
    if (api.field_0x27c != -1)
        TractorBeamCode(object);
    api.previous_velocity.x = api.velocity.x;
    api.previous_velocity.y = api.velocity.y;
    api.previous_velocity.z = api.velocity.z;
    object->target_velocity = api.velocity;
    object->field_0xefd &= ~4;
    object->field_0x107a = -1;
    api.field_0x1fa |= 1;
    api.flags_low &= ~4;
    APIObjectVelocities(object);
    GameObjectOrigin(object);
    AddSurfaceRipples(object);
}

