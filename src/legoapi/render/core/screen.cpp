// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/screen.h"
#include "legoapi/render/light/fade.h"
#include "legoapi/characters/motion.h"
#include "legoapi/menus/core/text.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/characters/motion/gameanim.h"
#include "gamelib/util/gamelib_util_types.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "batman.h"
#include "gameapi/edtools/edgra.h"
#include "legoapi/core/startup/main.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/numath/numtx.h"

#include <GLES2/gl2.h>
#include <stdio.h>
#include <string.h>
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/render/core/render.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/character/transform.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/actions/character/snake.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/menus/core/gamehint.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/arcade.h"
#include "legoapi/render/light/fade_material.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nu3d/android/nuportal_android.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/cutscenes/cutscenes.h"
#include <math.h>
#include "nu2api/numath/nutrig.h"
#include "gameframework/saveload.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/numath.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include "nu2api/nucore/android/NuThread_android.h"
#include "legoapi/render/fx/particles.h"
#include "legoapi/ai/core/legoai.h"
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/actions/character/streaks.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/audio/audio.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/door/spinner.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/timing.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "legoapi/items/objects/gameobjects_types.h"
#include "legoapi/actions/character/suit.h"
#include "legoapi/items/base/apiobject.h"
#include <new>
#include "nu2api/numath/nuang.h"
#include "legoapi/gizmos/object/newblowup.h"
#include <stdlib.h>
#include "legoapi/cutscenes/minicamcut.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/numouse.h"
#include "legoapi/characters/motion/camera_types.h"
#include "legogame/game.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/characters/core/players_types.h"
#include "nu2api/nufile/nufilepak.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/gizmos/object/lever.h"
#include "legoapi/gizmo/base/gizmessage.h"
#include "legoapi/props/objects/tightrope.h"
#include "legoapi/gizmos/trigger/minicut.h"
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/gizmos/trigger/giztimer.h"
#include "legoapi/gizmos/trigger/gizrandom.h"
#include "legoapi/gizmos/door/plugs.h"
#include "legoapi/gizmos/door/push.h"
#include "legoapi/gizmos/fx/edgizshadowmachine.h"
#include "legoapi/world/areas.h"
#include "legoapi/actions/character/specialmoves.h"
#include "legoapi/characters/core/customiser.h"
#include "nu2api/numusic/numusic.h"
#include "legoapi/characters/core/characters_types.h"
#include "gameapi/gui/apimenu_internal.h"
#include "legoapi/core/config/fileselect.h"
#include "nu2api/nu3d/nugscn.h"
#include "gamelib/crc/crc.h"
#include "legoapi/menus/screens/movies.h"
#include "nu2api/nucore/nuvideo.h"
#include "legoapi/misc/androidbatman.h"
#include "legoapi/items/collect/spacelevel.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "nu2api/nu3d/nushader_plain.h"
#include "legoapi/actions/movement/jumping.h"
#include "gameapi/edtools/edpart_internal.h"
#include "gameapi/edtools/edcam.h"
#include "legoapi/render/core/terrain_internal.h"
#include "legoapi/render/core/gameliball.h"
#include "legoapi/render/fx/game_deb.h"
#include "gameapi/edtools/edpp_internal.h"
#include "nu2api/numath/numtx_inline.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nu3d/android/nurain_android.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include <stddef.h>
#include "legoapi/render/core/terrain_stubs_types.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmo/object/giztorpedo.h"
#include <float.h>
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edui_colour.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edgra_internal.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include <cstdio>
#include "gameapi/edtools/edtools_types.h"
#include "legoapi/misc/utilities.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/nucore/nuonline.h"
#include <stdarg.h>
#include "nu2api/nucore/numem.h"
#include "java/android.h"
#include "java/java.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/NuInputManager.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nuscreen.hpp"
#include "nu2api/numath/nuquat.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/actions/character/speederchase_types.h"
#include "editor/edpath.h"
#include "editor/antinode_editor_private.h"
#include "nu2api/nucore/nulist.h"
#include "editor/edtoolsall_types.h"
#include "legoapi/items/base/animpacket.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/gizmos/object/hatmachine.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/gizmos/traps/attracto.h"
#include "legoapi/props/objects/signal.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/gizmos/transport/teleport.h"
#include "legoapi/menus/core/gamemessage.h"
#include "legoapi/characters/motion/move_types.h"
#include "legoapi/characters/motion/gameanim_types.h"
#include "gameapi/ai/aisys/aipath.h"
#include "legoapi/ai/game/creature.h"
#include "legoapi/gizmos/traps/gizbombgen.h"
#include "nu2api/nucore/numechptr.hpp"
#include "legoapi/world/levels/episodeV_types.h"
#include "legoapi/actions/combat/fighting.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/characters/core/playeritems.h"
#include "legoapi/gizmo/base/GizBuildItObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizLeverObjectInterface.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizPanelObjectInterface.h"
#include "legoapi/gizmo/base/GizTurretObjectInterface.h"
#include "legoapi/gizmo/base/HatMachineObjectInterface.h"
#include "legoapi/gizmo/base/TeleportObjectInterface.h"
#include "legoapi/items/collect/bolts.h"
#include "legoapi/gizmos/fx/guidelines.h"
#include "legoapi/gizmos/transport/ledges.h"
#include "legoapi/gizmos/traps/shards.h"
#include "nu2api/nu3d/nuhspecial.h"
#include "legoapi/world/levels/episodeII_types.h"
#include "legoapi/misc.h"
#include "legoapi/world/levels/podrace.h"
#include "legoapi/world/levels/episodeI_types.h"
#include "legoapi/ai/core/gameai_types.h"
#include "legoapi/ai/game/creature_types.h"
#include <stdint.h>
#include "gameapi/ai/aisys/aiscript_types.h"
#include "legoapi/actions/character/simpletons_types.h"
#include "legoapi/ai/core/ai_pathgeometry.h"
#include "legoapi/gizmo/object/takeoverobjects.h"
#include "nu2api/nucore/nurdp.h"
#include "legoapi/ai/core/aisysall_types.h"
#include "editor/aieditor_state.h"
#include "nu2api/numath/nuplane.h"
#include "editor/aieditor_settings.h"
#include "editor/path_connections.h"
#include "editor/edpath_types.h"
#include "legoapi/ai/game/gameantinode.h"
#include "legoapi/actions/movement/jumping_types.h"
#include <ctype.h>
#include "legoapi/world/levels/episode_types.h"
#include "legoapi/items/collect/batarang.h"
#include "legoapi/gizmo/object/gizmoblowups_types.h"
#include "legoapi/misc/supportall.h"
#include <limits.h>
#include "legoapi/gizmo/base/gizflow.h"
#include "legoapi/misc/supportall_types.h"
#include "gameapi/ai/aisys/gameaiscript.h"
#include "legoapi/characters/motion/camera.h"
#include "legoapi/characters/motion/move.h"
#include "legoapi/world/levels/episodeV.h"
#include "nu2api/nu3d/nu3d_includes.h"

// The fade system and the four fade effects handed to FadeSystem::AddFade()
// are screen.cpp globals in the original: its static initializer registers
// their destructors (original bss @0x127bf20 / 0x127bf08 / 0x127bf00 /
// 0x127bef8 / 0x127bef0).
FadeSystem FadeSys;
Fade fade;
FadeWipe fadeWipe;
FadeStillWipe fadeStillWipe;
FadeStill fadeStill;

// Material handles used by the original 2-D fade paths.
NUMTL *ScreenFadeMtl;
NUMTL *FadeMtl2;
NUMTL *FadeMtl;

static u8 ScreenGrabNeeded;
static i32 pause_rt;
static NUMTL *pause_rndr_mtl;
static i32 old_pause_state;
i32 (*PauseRenderOffFn)(void);
i32 cut_waiting_for_new_level;

nugscn_s *FadeLoop_ObjScene;
nuhspecial_s FadeLoop_ObjHSpecial;

struct nuhspecial_s;

// Retail only keeps the IPA-SRA clone, which drops the unused fourth argument.
#if defined(__i386__)
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone))
#endif
static NU_READ_GRAPHICS_DATA_ATTR NUGSCN *NuReadGraphicsData(VARIPTR *buf, VARIPTR *buf_end, char *path,
                                                             char *scene_data)
    __asm__("_ZL18NuReadGraphicsDataP9variptr_uS0_PcS1_S1_.isra.104");

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

static NUGSCN *backdrop_scene = nullptr;

static nuhspecial_s backdrop_hspecial[4];

static f32 backdrop_back_wait = 0.0f;

f32 backdrop_top_r = 0.0f;
f32 backdrop_top_g = 0.0f;
f32 backdrop_top_b = 0.0f;
f32 backdrop_bot_r = 0.0f;
f32 backdrop_bot_g = 0.0f;
f32 backdrop_bot_b = 24.0f;

static f32 backdrop_top_tr = 0.0f;
static f32 backdrop_top_tg = 0.0f;
static f32 backdrop_top_tb = 0.0f;
static f32 backdrop_bot_tr = 0.0f;
static f32 backdrop_bot_tg = 0.0f;
static f32 backdrop_bot_tb = 24.0f;

i32 backdrop_black = 0;

void (*BackDrop_AlphaFn)(float *) = nullptr;

// Written by ThingManager's ctor (original global @0x124f2e0, .bss).

GAMEFOG_STATE GameFog = {};
f32 dof_fstop;

static i32 GameFogSnap;
static rtlfog_s *GameFogSet;
static f32 GameFogDuration;
static f32 GameFogTime;
static GAMEFOG_STATE GameFogOld;
static GAMEFOG_STATE GameFogNew;

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

// Forward declarations for local (static) game-object helper stubs.
struct GameObject_s;
struct nuvec_s;
struct WORLDINFO_s;

// Original data symbol testmovef is float0.005 at 0x617cb0.


extern i32 LevFlag[4];


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

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

namespace {

} // namespace


struct numtx_s;

// Forward declarations for local (static) character/gameplay helper stubs.
struct EDCREATURE_s;

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
    extern "C++" FadeSystem FadeSys;
}

// Original .data @0x667ce0; the names are filled in by the static initializer.

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

#include "nu2api/nucore/nucore_plain_types.h"

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

struct nupad_s;
union variptr_u;

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

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
extern AICONDITIONDEF lego_aiconditiondefs[178];


}

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
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

// Retail re-tests the three block animations at each use instead of caching a flag.
#define HAS_BLOCK_ANIMS(animations)                                                                                    \
    ((animations)[0x1a] != NULL || (animations)[0x1b] != NULL || (animations)[0x1c] != NULL)

// Retail splits the context test off as an inline head; the rest is this partial clone.


// Retail uses translation-unit-local helpers with compiler-inferred private
// calling conventions; keep their shared animation-mode call sites intact.


// Asteroid chase (AsteroidChase_A / B / C / D)




#define PART_EMIT_SOUND_ON(slot)                                                                                       \
    if (type->sounds[slot] != -1) {                                                                                    \
        SetSfxBit_On(type->sounds[slot]);                                                                              \
        if (type->sound_modes[slot] == 4)                                                                              \
            PlaySfxById(type->sounds[slot], &emitter->position);                                                       \
    }
#define PART_EMIT_SOUND(slot, mode)                                                                                    \

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];


}


namespace {

} // namespace

struct FLOWBOX_s;


extern "C" {
    extern "C++" FadeSystem FadeSys;
}


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
} // extern "C"

#if defined(__i386__)
#define AI_SYS_STUBS_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define AI_SYS_STUBS_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif


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
extern AICONDITIONDEF lego_aiconditiondefs[178];


}

namespace {
#include "legoapi/gizmo/gizmos/gizmos_gizactions_types.h"

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


// Original writable flagremaptab at 0x668520, 32 pairs of 32-bit masks.
extern "C" {
extern u32 flagremaptab[32][2];
}

// Definitions from the original gizmoblowups translation unit.



// The reference defines these with dynamic initialization in this unit.


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

void FadeLoop_SetObj(nugscn_s *scene, char *name) {
    FadeLoop_ObjScene = scene;
    if (scene != NULL && NuSpecialFind(scene, &FadeLoop_ObjHSpecial, name, 1) == 0) {
        FadeLoop_ObjScene = NULL;
    }
}

i32 FadeLoop_UsingObj() {
    return FadeLoop_ObjScene != NULL;
}

void FadeLoop_DrawObj(float alpha) {
    if (FadeLoop_ObjScene != NULL && NuSpecialExistsFn(&FadeLoop_ObjHSpecial)) {
        NUVEC scale = {0.125f, 0.125f, 0.125f};
        NUMTX_ALIGNED16 matrix;
        NuMtxSetScale(&matrix, &scale);
        matrix.m32 = 1.0f;
        NuSpecialDrawAtAlpha(&FadeLoop_ObjHSpecial, &matrix, alpha);
    }
}

void CreateFadeMaterials() {
    FadeMtl = NuMtlCreate(1);
    FadeMtl->attribs.z_mode = 3;
    FadeMtl->attribs.alpha_mode = 3;
    FadeMtl->attribs.unknown_2_1_2 = 2;
    FadeMtl->attribs.unknown_1_1_2 = 1;
    FadeMtl->attribs.unknown_1_4_8 = 1;
    FadeMtl->attribs.unknown_2_4 = 1;
    FadeMtl->attribs.filter_mode = 1;
    NuMtlUpdate(FadeMtl);

    FadeMtl2 = NuMtlCreate(1);
    FadeMtl2->attribs.z_mode = 3;
    FadeMtl2->attribs.alpha_mode = 1;
    FadeMtl2->attribs.unknown_2_1_2 = 2;
    FadeMtl2->attribs.unknown_1_1_2 = 1;
    FadeMtl2->attribs.unknown_1_4_8 = 1;
    FadeMtl2->attribs.unknown_2_4 = 1;
    FadeMtl2->attribs.filter_mode = 1;
    NuMtlUpdate(FadeMtl2);

    ScreenFadeMtl = NuMtlCreate(1);
    ScreenFadeMtl->opacity = 1.0f;
    ScreenFadeMtl->tex_id = 0;
    ScreenFadeMtl->sort_pri = 0x7ffe;
    ScreenFadeMtl->attribs.z_mode = 3;
    ScreenFadeMtl->attribs.unknown_1_1_2 = 1;
    ScreenFadeMtl->attribs.unknown_2_1_2 = 2;
    ScreenFadeMtl->attribs.unknown_2_4 = 1;
    ScreenFadeMtl->attribs.unknown_1_4_8 = 1;
    ScreenFadeMtl->attribs.alpha_mode = 1;
    ScreenFadeMtl->attribs.filter_mode = 1;
    NuMtlUpdate(ScreenFadeMtl);
}

void BackDrop_ResetColours() {
    backdrop_top_r = 0.0f;
    backdrop_top_g = 0.0f;
    backdrop_top_b = 0.0f;
    backdrop_bot_r = 0.0f;
    backdrop_bot_g = 0.0f;
    backdrop_bot_b = 0.0f;
    backdrop_back_wait = 0.0001f;
    backdrop_top_tr = 0.0f;
    backdrop_top_tg = 0.0f;
    backdrop_top_tb = 0.0f;
    backdrop_bot_tr = 0.0f;
    backdrop_bot_tg = 0.0f;
    backdrop_bot_tb = 0.0f;
}

void BackDrop_UpdateColours(i32 instant) {
    if (backdrop_black) {
        backdrop_top_r = 0.0f;
        backdrop_top_g = 0.0f;
        backdrop_top_b = 0.0f;
        backdrop_bot_r = 0.0f;
        backdrop_bot_g = 0.0f;
        backdrop_bot_b = 0.0f;
        return;
    }

    if (backdrop_back_wait > 0.0f) {
        backdrop_back_wait -= FRAMETIME;
        if (!(0.0f >= backdrop_back_wait)) {
            return;
        }
        if (instant != 0) {
            backdrop_top_tb = 0.0f;
            backdrop_top_tg = 0.0f;
            backdrop_top_tr = 0.0f;
            backdrop_bot_tb = 0.0f;
            backdrop_bot_tg = 0.0f;
            backdrop_bot_tr = 0.0f;
        } else {
            backdrop_top_tr = 0.0f;
            backdrop_top_tg = 0.0f;
            backdrop_top_tb = 0.0f;
            backdrop_bot_tr = 0.0f;
            backdrop_bot_tg = 0.0f;
            backdrop_bot_tb = 0.0f;
        }
        return;
    }

    if (backdrop_top_r == backdrop_top_tr && backdrop_top_g == backdrop_top_tg && backdrop_top_b == backdrop_top_tb &&
        backdrop_bot_r == backdrop_bot_tr && backdrop_bot_g == backdrop_bot_tg && backdrop_bot_b == backdrop_bot_tb) {
        backdrop_back_wait = static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 2.5f + 2.5f;
        return;
    }

    const f32 step = FRAMETIME * 10.0f;
    backdrop_top_r = SeekLinearF(backdrop_top_r, backdrop_top_tr, step);
    backdrop_top_g = SeekLinearF(backdrop_top_g, backdrop_top_tg, step);
    backdrop_top_b = SeekLinearF(backdrop_top_b, backdrop_top_tb, step);
    backdrop_bot_r = SeekLinearF(backdrop_bot_r, backdrop_bot_tr, step);
    backdrop_bot_g = SeekLinearF(backdrop_bot_g, backdrop_bot_tg, step);
    backdrop_bot_b = SeekLinearF(backdrop_bot_b, backdrop_bot_tb, step);
}

void BackDrop_Init(char *path, variptr_u *buf, variptr_u *buf_end) {
    if (backdrop_scene == NULL) {
        backdrop_scene = NuGScnRead(buf, *buf_end, path);
    }
    memset(backdrop_hspecial, 0, sizeof(backdrop_hspecial));
    if (backdrop_scene == NULL) {
        return;
    }
    NuSpecialFind(backdrop_scene, &backdrop_hspecial[0], (char *)"ball1", 1);
    NuSpecialFind(backdrop_scene, &backdrop_hspecial[1], (char *)"ball2", 1);
    NuSpecialFind(backdrop_scene, &backdrop_hspecial[2], (char *)"ball3", 1);
    NuSpecialFind(backdrop_scene, &backdrop_hspecial[3], (char *)"ball4", 1);
}

void BackDrop_Update(float dt) {
    if (backdrop_scene != NULL) {
        NuGScnUpdate(backdrop_scene, dt * 60.0f);
    }
}

void BackDrop_Draw(float alpha, i32 flags) {
    if (backdrop_scene == NULL) {
        return;
    }
    if (flags == 0 && BackDrop_AlphaFn != NULL) {
        BackDrop_AlphaFn(&alpha);
    }
    if (!(alpha > 0.0f)) {
        return;
    }

    for (i32 special_index = 0; special_index < 2; ++special_index) {
        nuhspecial_s *special = &backdrop_hspecial[special_index];
        if (NuSpecialExistsFn(special) == 0) {
            continue;
        }
        // Retail realigns the frame (and esp,-16) for this matrix.
        NUMTX_ALIGNED16 mtx = *NuSpecialGetDrawMtx(special);
        NuSpecialDrawAtAlpha(special, &mtx, alpha);
        f32 x = mtx.m30;
        f32 y = mtx.m31;
        f32 z = mtx.m32;
        u16 angle = (u16)qrand();
        for (i32 i = 0; i < 3; ++i) {
            mtx.m30 = NU_SIN_LUT(angle) * 0.01f + x;
            mtx.m31 = NU_COS_LUT(angle) * 0.01f + y;
            mtx.m32 = z;
            NuSpecialDrawAtAlpha(special, &mtx, alpha);
            angle = (u16)(angle + 0x5555);
        }
    }
}

void FinishLoop(i32 frames) {
    if (!FinishLoop_On) {
        return;
    }

    if (finishloop_backdroponly != 0) {
        pNuCam->mtx = numtx_identity;
        NuCameraSet(pNuCam);

        do {
            NuFrameBegin();
            NuRndrBeginScene(-1);
            NuRndrClear(0xb00, 0, 1.0f);
            BackDrop_Draw(1.0f, 1);
            NuRndrEndScene();
            edGraEnableTerrainSwap();
            NuFrameEnd();
            edGraDisableTerrainSwap();
        } while (--frames > 0);
        finishloop_backdroponly = 0;
        return;
    }

    if (FadeSys.pending_type == FADE_TYPE_NONE) {
        do {
            NuFrameBegin();
            NuRndrBeginScene(-1);
            NuRndrClear(0xb00, 0, 1.0f);
            NuRndrEndScene();
            edGraEnableTerrainSwap();
            NuFrameEnd();
            edGraDisableTerrainSwap();
        } while (--frames > 0);
    } else {
        do {
            NuFrameBegin();
            FadeSys.Draw();
            edGraEnableTerrainSwap();
            NuFrameEnd();
            edGraDisableTerrainSwap();
        } while (--frames > 0);
    }
}

void FadeLoop(char *text, i32 direction, float duration, void (*draw)(float)) {
    f32 alpha = 0.0f;
    f32 target = 1.0f;
    if (direction != 0) {
        alpha = 1.0f;
        target = 0.0f;
    }
    f32 rate = 10.0f;
    if (duration != 0.0f) {
        rate = 1.0f / duration;
    }
    FRAMETIME = DEFAULTFRAMETIME;
    while (alpha != target) {
        NuFrameBegin();
        alpha = SeekLinearF(alpha, target, rate * FRAMETIME);
        NuRndrBeginScene(-1);
        NuRndrClear(0xb00, 0, 1.0f);
        if (FadeLoop_ObjScene != NULL) {
            FadeLoop_DrawObj(alpha);
        }
        if (text != NULL) {
            SetQFont2D();
            Text3D(text, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0, static_cast<u8>(alpha * 0.0f),
                   static_cast<u8>(alpha * 191.0f), static_cast<u8>(alpha * 255.0f));
        }
        if (draw != NULL) {
            draw(alpha);
        }
        NuRndrEndScene();
        edGraEnableTerrainSwap();
        FRAMETIME = NuFrameEnd();
        edGraDisableTerrainSwap();
    }
    if (direction == 1) {
        FadeLoop_SetObj(NULL, NULL);
        FinishLoop(2);
    }
}

void NoRender() {
    pNuCam->mtx = numtx_identity;
    NuCameraSet(pNuCam);
    NuRndrBeginScene(-1);

    if (back_rgba[0] == back_rgba[1]) {
        NuRndrClear(0xf00, back_rgba[0], 1.0f);
    } else {
        NuRndrGradClear(0xf00, back_rgba[0], back_rgba[1], 1.0f);
    }

    if (MainRenderTime != 1.0f) {
        BackDrop_Draw(1.0f - MainRenderTime, 0);
    }
    NuRndrEndScene();
}

void BackDrop_Dump() {
    if (backdrop_scene != NULL) {
        NuGScnRemove(backdrop_scene);
    }
}

void DrawParallax(nuhspecial_s *special) {
    if (NuIOS_IsLowEndDevice() != 0 && WORLD != NULL && WORLD->current_level != NULL &&
        WORLD->current_level->data_display.far_clip < 20000.0f &&
        WORLD->current_level->data_display.fog_start < 20000.0f) {
        return;
    }

    const f32 uniform_scale = pNuCam->far_clip * 0.05f;
    NUVEC scale = {uniform_scale, uniform_scale, uniform_scale};
    NUMTX_ALIGNED16 matrix;
    NuMtxSetScale(&matrix, &scale);
    matrix.m30 = pNuCam->mtx.m30;
    matrix.m31 = pNuCam->mtx.m31;
    matrix.m32 = pNuCam->mtx.m32;
    NuRndrStateSetFogEnabled(0);
    NuSpecialDrawAt(special, &matrix);
    NuRndrStateSetFogEnabled(1);
}

void GameFog_Reset() {
    GameFogSnap = 1;
    GameFogDuration = 0.0f;
    GameFogSet = 0;
    GameFogTime = 0.0f;
}

void GameFog_Update(WORLDINFO_s *world) {
    rtlfog_s *previous = GameFogSet;
    if (world->rtl_set != NULL &&
        !(CUTSTOPGAME != 0 && CutStopInfo != NULL && (static_cast<CUTINFO *>(CutStopInfo)->flags & 4) != 0)) {
        GameFogSet = rtlGetFogSet(world->rtl_set, NUMTX_GET_ROW_VEC(&pNuCam->mtx, 3));
    } else {
        GameFogSet = NULL;
    }

    if (GameFogSnap == 0 && GameFogSet != previous) {
        GameFogTime = 0.0f;
        GameFogDuration = 3.0f;
        GameFogOld = GameFog;
    }
    if (GameFogSnap != 0 || GameFogSet != previous) {
        if (GameFogSet != NULL) {
            u32 colour = GameFogSet->colour;
            GameFogNew.high_quality_colour_channels[0] = colour >> 24;
            GameFogNew.high_quality_colour_channels[1] = (colour >> 16) & 0xff;
            GameFogNew.high_quality_colour_channels[2] = (colour >> 8) & 0xff;
            GameFogNew.high_quality_colour_channels[3] = colour & 0xff;
            GameFogNew.high_quality_start = GameFogSet->start;
            GameFogNew.high_quality_end = GameFogSet->end;
            GameFogNew.high_quality_density = GameFogSet->density;
            GameFogNew.low_quality_density = GameFogSet->low_quality_density;
            colour = GameFogSet->low_quality_colour;
            GameFogNew.low_quality_colour_channels[0] = colour >> 24;
            GameFogNew.low_quality_colour_channels[1] = (colour >> 16) & 0xff;
            GameFogNew.low_quality_colour_channels[2] = (colour >> 8) & 0xff;
            GameFogNew.low_quality_colour_channels[3] = colour & 0xff;
        } else {
            GameFogNew.high_quality_colour_channels[0] = GameFogNew.high_quality_colour_channels[1] =
                GameFogNew.high_quality_colour_channels[2] = GameFogNew.high_quality_colour_channels[3] = 0.0f;
            GameFogNew.high_quality_start = 5.0f;
            GameFogNew.high_quality_end = 20.0f;
            GameFogNew.high_quality_density = 0.0f;
            GameFogNew.low_quality_density = 0.0f;
            GameFogNew.low_quality_colour_channels[0] = GameFogNew.low_quality_colour_channels[1] =
                GameFogNew.low_quality_colour_channels[2] = GameFogNew.low_quality_colour_channels[3] = 0.0f;
        }
    }

    f32 fstop = 22.0f;
    if (GameFogSet != NULL) {
        fstop = static_cast<f32>(static_cast<u32>(GameFogSet->depth_of_field_fstop)) / 10.0f;
        if (fstop == 0.0f)
            fstop = 22.0f;
    }
    dof_fstop = fstop;

    if (NuIOS_IsLowEndDevice() && world->current_level != NULL) {
        GameFogNew.low_quality_end = world->current_level->data_display.far_clip;
        GameFogNew.low_quality_start = world->current_level->data_display.fog_start;
    }
    if (GameFogSnap != 0) {
        GameFog = GameFogNew;
        GameFogDuration = 0.0f;
        GameFogTime = 0.0f;
    } else {
        if (GameFogTime < GameFogDuration) {
            GameFogTime += FRAMETIME;
            if (GameFogTime > GameFogDuration)
                GameFogTime = GameFogDuration;
        }
        if (GameFogTime >= GameFogDuration) {
            GameFog = GameFogNew;
        } else {
            f32 blend = 1.0f - (NU_SIN_LUT(GameFogTime / GameFogDuration * 32768.0f + 16384.0f) + 1.0f) * 0.5f;
            GameFog.high_quality_colour_channels[0] =
                GameFogOld.high_quality_colour_channels[0] +
                (GameFogNew.high_quality_colour_channels[0] - GameFogOld.high_quality_colour_channels[0]) * blend;
            GameFog.high_quality_colour_channels[1] =
                GameFogOld.high_quality_colour_channels[1] +
                (GameFogNew.high_quality_colour_channels[1] - GameFogOld.high_quality_colour_channels[1]) * blend;
            GameFog.high_quality_colour_channels[2] =
                GameFogOld.high_quality_colour_channels[2] +
                (GameFogNew.high_quality_colour_channels[2] - GameFogOld.high_quality_colour_channels[2]) * blend;
            GameFog.high_quality_colour_channels[3] =
                GameFogOld.high_quality_colour_channels[3] +
                (GameFogNew.high_quality_colour_channels[3] - GameFogOld.high_quality_colour_channels[3]) * blend;
            GameFog.high_quality_start =
                GameFogOld.high_quality_start + (GameFogNew.high_quality_start - GameFogOld.high_quality_start) * blend;
            GameFog.high_quality_end =
                GameFogOld.high_quality_end + (GameFogNew.high_quality_end - GameFogOld.high_quality_end) * blend;
            GameFog.high_quality_density = GameFogOld.high_quality_density +
                                           (GameFogNew.high_quality_density - GameFogOld.high_quality_density) * blend;
            GameFog.low_quality_density = GameFogOld.low_quality_density +
                                          (GameFogNew.low_quality_density - GameFogOld.low_quality_density) * blend;
            GameFog.low_quality_colour_channels[0] =
                GameFogOld.low_quality_colour_channels[0] +
                (GameFogNew.low_quality_colour_channels[0] - GameFogOld.low_quality_colour_channels[0]) * blend;
            GameFog.low_quality_colour_channels[1] =
                GameFogOld.low_quality_colour_channels[1] +
                (GameFogNew.low_quality_colour_channels[1] - GameFogOld.low_quality_colour_channels[1]) * blend;
            GameFog.low_quality_colour_channels[2] =
                GameFogOld.low_quality_colour_channels[2] +
                (GameFogNew.low_quality_colour_channels[2] - GameFogOld.low_quality_colour_channels[2]) * blend;
            GameFog.low_quality_colour_channels[3] =
                GameFogOld.low_quality_colour_channels[3] +
                (GameFogNew.low_quality_colour_channels[3] - GameFogOld.low_quality_colour_channels[3]) * blend;
        }
    }
    GameFog.colour = (static_cast<i32>(GameFog.high_quality_colour_channels[0]) << 24) +
                     (static_cast<i32>(GameFog.high_quality_colour_channels[1]) << 16) +
                     (static_cast<i32>(GameFog.high_quality_colour_channels[2]) << 8) +
                     static_cast<i32>(GameFog.high_quality_colour_channels[3]);
    GameFog.low_quality_colour = (static_cast<i32>(GameFog.low_quality_colour_channels[0]) << 24) +
                                 (static_cast<i32>(GameFog.low_quality_colour_channels[1]) << 16) +
                                 (static_cast<i32>(GameFog.low_quality_colour_channels[2]) << 8) +
                                 static_cast<i32>(GameFog.low_quality_colour_channels[3]);
    GameFogSnap = 0;
    g_BackgroundColour = GameFog.colour;
}

void GameDisplaySettings(LEVELDATADISPLAY *display, i32 *background_colours) {
    CUTINFO *cut = static_cast<CUTINFO *>(CutStopInfo);
    if (cut != NULL && cut->camera_near_clip != 0.0f) {
        pNuCam->near_clip = cut->camera_near_clip;
    } else {
        pNuCam->near_clip = display->unknown_04;
    }

    u16 far_clip;
    if (cut != NULL) {
        far_clip = cut->camera_far_clip;
        if (far_clip == 0) {
            far_clip = static_cast<u16>(display->unknown_14);
        }
    } else {
        far_clip = static_cast<u16>(display->unknown_14);
    }
    pNuCam->far_clip = static_cast<f32>(static_cast<u32>(far_clip));

    LEVELDATA *level = WORLD->current_level;
    const bool use_backdrop = level == TITLES_LDATA || (level->flags & LEVEL_STATUS) != 0 || level == STATUS_LDATA ||
                              level == CREDITS_LDATA || MainRenderTime < 0.0f;
    if (!use_backdrop) {
        background_colours[0] =
            0x80000000u | static_cast<u8>(display->bg_red_top) |
            (static_cast<u8>(display->bg_green_top) << 8 | static_cast<u8>(display->bg_blue_top) << 16);
        background_colours[1] =
            0x80000000u | static_cast<u8>(display->bg_red_bottom) |
            (static_cast<u8>(display->bg_green_bottom) << 8 | static_cast<u8>(display->bg_blue_bottom) << 16);
        return;
    }

    BackDrop_UpdateColours(0);

    i32 top_b = static_cast<i32>(backdrop_top_b);
    i32 top_g = static_cast<i32>(backdrop_top_g);
    i32 top_r = static_cast<i32>(backdrop_top_r);
    i32 bottom_r = static_cast<i32>(backdrop_bot_r);
    i32 bottom_b = static_cast<i32>(backdrop_bot_b);
    i32 bottom_g = static_cast<i32>(backdrop_bot_g);

    f32 alpha = 1.0f;
    if (BackDrop_AlphaFn != NULL) {
        BackDrop_AlphaFn(&alpha);
        if (alpha != 0.0f) {
            top_r = static_cast<i32>(static_cast<f32>(top_r) * alpha);
            top_b = static_cast<i32>(static_cast<f32>(top_b) * alpha);
            top_g = static_cast<i32>(static_cast<f32>(top_g) * alpha);
            bottom_r = static_cast<i32>(static_cast<f32>(bottom_r) * alpha);
            bottom_g = static_cast<i32>(static_cast<f32>(bottom_g) * alpha);
            bottom_b = static_cast<i32>(static_cast<f32>(bottom_b) * alpha);
        }
    }

    background_colours[0] = 0x80000000u | (top_r & 0xff) | (top_g & 0xff) << 8 | (top_b & 0xff) << 16;
    background_colours[1] = 0x80000000u | (bottom_r & 0xff) | (bottom_g & 0xff) << 8 | (bottom_b & 0xff) << 16;
}

void SetCameraZoom(f32 zoom) {
    CameraZoom = zoom;
}

void WidescreenCode(i32) {
    pNuCam->aspect = 1.0f / NuIOS_GetAspectRatio();
    pNuCam->fov = (1.0f / NuIOS_GetAspectRatio() + 0.75f) * 0.5f * (1.0f / CameraZoom);
    SmartTextSetWidescreen(1.3333334f / NuIOS_GetAspectRatio(), 1.0f);
}

void KeepPointOnScreen(NUVEC *position, NUVEC *velocity) {
    if (position->x < -0.85f) {
        position->x = -0.85f;
        if (velocity != NULL)
            velocity->x = 0.0f;
    } else if (position->x > 0.85f) {
        position->x = 0.85f;
        if (velocity != NULL)
            velocity->x = 0.0f;
    }
    if (position->y < -0.85f) {
        position->y = -0.85f;
        if (velocity != NULL)
            velocity->y = 0.0f;
    } else if (position->y > 0.85f) {
        position->y = 0.85f;
        if (velocity != NULL)
            velocity->y = 0.0f;
    }
}

void UpdateCutBorders() {
    f32 target_scale = 1.0f;

    if (CUTSTOPGAME == 0 && (LEGOCAMMODE_DOORCUT == -1 || GameCam->mode != LEGOCAMMODE_DOORCUT)) {
        if (LEGOCAMMODE_OBSTACLE == -1 || GameCam->mode != LEGOCAMMODE_OBSTACLE || ObstacleCamBorders == 0) {
            target_scale = 0.0f;
        }
    }

    CutBorderScale = SeekLinearF(CutBorderScale, target_scale, FRAMETIME * 2.0f);
}

void DrawCutBorders(i32 widescreen) {
    if (!(CutBorderScale > 0.0f)) {
        return;
    }

    NuRndrBeginScene(-1);
    f32 border_scale = 0.1f;
    const CUTINFO *cut = static_cast<CUTINFO *>(CutStopInfo);
    if (cut != NULL && (cut->flags & 0x4000) != 0) {
        border_scale = 0.21276596f;
    } else {
        const CUTSYS *system = WORLD->cutscene_sys;
        if (system != NULL && system->count == 1 && (system->cuts[0]->flags & 0x4000) != 0) {
            border_scale = 0.21276596f;
        }
    }
    if (widescreen != 0) {
        border_scale = (border_scale * 4.0f - 0.5f) / 3.0f;
    }
    if (border_scale < 0.02f) {
        border_scale = 0.02f;
    }
    const i32 height =
        static_cast<i32>(border_scale * 3584.0f * NU_SIN_LUT(static_cast<i32>(CutBorderScale * 16384.0f)));
    NuRndrRect2di(0, 0, 0x2800, height, 0x80ffffff, FadeMtl);
    NuRndrRect2di(0, 0xe00 - height, 0x2800, height, 0x80ffffff, FadeMtl);
    NuRndrEndScene();
}

void InitStillRender(variptr_u *, variptr_u) {
    static NUNATIVETEX nativePauseTex;

    pause_rt = NuTexGenTexture(&nativePauseTex);
    nativePauseTex.ref_count = 1;
    // Retail writes each checksum byte (48c587..48c60c); image_data and size retain their initialized state.
    nativePauseTex.checksum[0] = 0;
    nativePauseTex.checksum[1] = 0;
    nativePauseTex.checksum[2] = 0;
    nativePauseTex.checksum[3] = 0;
    nativePauseTex.checksum[4] = 0;
    nativePauseTex.checksum[5] = 0;
    nativePauseTex.checksum[6] = 0;
    nativePauseTex.checksum[7] = 0;
    nativePauseTex.checksum[8] = 0;
    nativePauseTex.checksum[9] = 0;
    nativePauseTex.checksum[10] = 0;
    nativePauseTex.checksum[11] = 0;
    nativePauseTex.checksum[12] = 0;
    nativePauseTex.checksum[13] = 0;
    nativePauseTex.checksum[14] = 0;
    nativePauseTex.checksum[15] = 0;
    nativePauseTex.height = g_backingHeight;
    nativePauseTex.width = g_backingWidth;

    BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/legoapi.saga/screen.cpp", 0x561);
    glGenTextures(1, &nativePauseTex.platform.gl_tex);
    glActiveTexture(GL_TEXTURE0);
    g_currentTexUnit = 0;
    glBindTexture(GL_TEXTURE_2D, nativePauseTex.platform.gl_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g_backingWidth, g_backingHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/legoapi.saga/screen.cpp", 0x56d);

    pause_rndr_mtl = NuMtlCreate3D(1);
    pause_rndr_mtl->sort_pri = 0x7fff;
    pause_rndr_mtl->shader_desc.flags = 0x1000;
    pause_rndr_mtl->opacity = 1.0f;
    u8 *attrib = reinterpret_cast<u8 *>(&pause_rndr_mtl->attribs);
    attrib[1] = static_cast<u8>((attrib[1] & 0x30) | 0xc5);
    attrib[2] = static_cast<u8>((attrib[2] & 0xfc) | 6);
    attrib[0] = static_cast<u8>((attrib[0] & 0xc0) | 0x11);
    attrib[1] = 0xe5;
    pause_rndr_mtl->tex_id = static_cast<i16>(pause_rt);
    pause_rndr_mtl->shader_desc.diffuse_color[0] = -1;
    pause_rndr_mtl->shader_desc.unknown_a8 = 1;
    pause_rndr_mtl->shader_desc.vtx_desc.flags = (pause_rndr_mtl->shader_desc.vtx_desc.flags & ~0x3800u) | 0x40800u;
    NuMtlUpdate(pause_rndr_mtl);
    pause_rndr_on = 0;
}

void ResizePauseScreenTexture(i32, i32) {
    STUBBED();
}

void DrawPauseScreenWipe() {
    NuRndrBeginScene(-1);

    i32 x = 0;
    i32 width = 0x2800;
    i32 y = 0;
    i32 height = 0xe00;
    f32 u0 = 0.0f;
    f32 u1 = 1.0f;
    f32 v1 = 0.0f;
    f32 v0 = 1.0f;
    u32 colours[4];

    if ((FadeSys.direction & 3) != 0) {
        if ((FadeSys.direction & 1) != 0) {
            const f32 fade = FadeSys.fade;
            u0 = 1.0f - fade;
            x = static_cast<i32>(u0 * 10240.0f);
            width = 0x2800 - x;
            colours[0] = 0x00808080u;
            colours[1] = 0x80808080u;
            colours[2] = 0x00808080u;
            colours[3] = 0x80808080u;
            NuRndrGradRectUV2di(x - 0x400, 0, 0x400, 0xe00, u0 - 0.1f, 1.0f, u0, 0.0f, colours, pause_rndr_mtl);
        } else {
            const f32 fade = FadeSys.fade;
            width = static_cast<i32>(fade * 10240.0f);
            colours[0] = 0x80808080u;
            colours[1] = 0x00808080u;
            colours[2] = 0x80808080u;
            colours[3] = 0x00808080u;
            NuRndrGradRectUV2di(width, 0, 0x400, 0xe00, fade, 1.0f, fade + 0.1f, 0.0f, colours, pause_rndr_mtl);
            u1 = fade;
        }
    } else if ((FadeSys.direction & 0xc) != 0) {
        if ((FadeSys.direction & 4) != 0) {
            const f32 fade = FadeSys.fade;
            const f32 edge = 1.0f - fade;
            y = static_cast<i32>(edge * 3584.0f);
            height = 0xe00 - y;
            colours[0] = 0x00808080u;
            colours[1] = 0x00808080u;
            colours[2] = 0x80808080u;
            colours[3] = 0x80808080u;
            NuRndrGradRectUV2di(0, y - 0x166, 0x2800, 0x166, 0.0f, 1.0f - (edge - 0.1f), 1.0f, 1.0f - edge, colours,
                                pause_rndr_mtl);
            v0 = 1.0f - edge;
        } else {
            const f32 fade = FadeSys.fade;
            height = static_cast<i32>(fade * 3584.0f);
            colours[0] = 0x80808080u;
            colours[1] = 0x80808080u;
            colours[2] = 0x00808080u;
            colours[3] = 0x00808080u;
            NuRndrGradRectUV2di(0, height, 0x2800, 0x166, 0.0f, 1.0f - fade, 1.0f, 1.0f - (fade + 0.1f), colours,
                                pause_rndr_mtl);
            v1 = 1.0f - fade;
        }
    }

    NuRndrRectUV2di(x, y, width, height, u0, v0, u1, v1, 0x80808080u, pause_rndr_mtl);
    NuRndrEndScene();
}

void StillMemRestore() {
    // No operation in the Android build.
}

void GrabStillScreen() {
    if (ScreenGrabNeeded != 0) {
        ScreenGrabNeeded = 0;
        NuRndrBeginScene(-1);
        NuBackbufferCopy(pause_rt);
        NuRndrEndScene();
        pause_fade = 0;
    }
}

void NeedScreenGrab(i32 needed) {
    ScreenGrabNeeded = needed != 0;
}

i8 IsGrabbingScreen() {
    return ScreenGrabNeeded;
}

void DrawStillScreen(i32 clear) {
    NuRndrBeginScene(-1);
    NuVpGetCurrentViewport();
    if (clear != 0) {
        NuRndrClear(0x500, 0, 1.0f);
    }
    if (MainRenderTime < 1.0f) {
        const u32 colour = (static_cast<i32>(MainRenderTime * 128.0f) << 24) | 0x00808080u;
        u32 colours[4] = {colour, colour, colour, colour};
        NuRndrGradRectUV2di(0, 0, 0x2800, 0xe00, 0.0f, 1.0f, 1.0f, 0.0f, colours, pause_rndr_mtl);
    } else {
        NuRndrRectUV2di(0, 0, 0x2800, 0xe00, 0.0f, 1.0f, 1.0f, 0.0f, 0x80808080u, pause_rndr_mtl);
    }
    NuRndrEndScene();
}

void DrawPauseFade() {
    if (editor_active != 0 || screendump != 0)
        return;

    f32 step = FRAMETIME * 2.0f;
    i32 fade;
    if (Paused == 0 && NetPaused == 0) {
        fade = static_cast<i32>(pause_fade - step);
        if (fade < 0)
            fade = 0;
    } else {
        fade = static_cast<i32>(pause_fade + step);
        if (fade > 0) {
            pause_fade = 1.0f;
            return;
        }
    }
    pause_fade = static_cast<f32>(fade);
}

void ClearStill() {
    old_pause_state = 0;
    Paused = 0;
}

void HandleStillRender() {
    GetMenuID();
    if (__builtin_expect(pause_rndr_on != 0 && FadeSys.pending_type == FADE_TYPE_NONE, 1)) {
        NuRndrBeginScene(-1);
        if (MainRenderTime < 1.0f) {
            if (__builtin_expect(back_rgba[0] == back_rgba[1], 0)) {
                NuRndrClear(0xf00, back_rgba[0], 1.0f);
            } else {
                NuRndrGradClear(0xf00, back_rgba[0], back_rgba[1], 1.0f);
            }
            BackDrop_Draw(1.0f - MainRenderTime, 0);
        } else {
            NuRndrClear(0x300, 0, 1.0f);
        }
        NuRndrEndScene();
    }

    if (__builtin_expect(grab_screen_image != 2 && pause_rndr_on != 0 && pause_rt != 0 && FadeSys.pending_type == FADE_TYPE_NONE, 0)) {
        DrawStillScreen(clear_screen_onstill);
    }

    if (Paused != 0) {
        if (old_pause_state == 0 && FadeSys.pending_type == FADE_TYPE_NONE) {
            NeedScreenGrab(1);
        }
    }
    if (Paused == 0 && old_pause_state != 0) {
        pause_rndr_on = 0;
    }
    old_pause_state = Paused;

    if (!((waiting_for_level == -1 ||
           (gone_through_door_to_new_level == 0 && cut_waiting_for_new_level == 0 && waiting_for_new_level == 0)) &&
          (NewLData != CREDITS_LDATA || LastLData != STATUS_LDATA) &&
          (NewLData != STATUS_LDATA ||
           ((WORLD->level_sub_id == -1 || (WORLD->area[WORLD->level_sub_id].flags & 2) == 0) && GAMEDEMO == 0)) &&
          NewLData != CREDITS_LDATA &&
          (NewLData != HUB_LDATA || WORLD->level_sub_id == -1 || (WORLD->area[WORLD->level_sub_id].flags & 2) == 0) &&
          !(MainRenderTime > 0.0f && MainRenderTime < 1.0f))) {
        pause_rndr_on = 1;
    } else {
        if (Paused == 0 || IsGrabbingScreen() != 0) {
            pause_rndr_on = static_cast<u32>(grab_screen_image) > 1;
        } else if (PauseRenderOffFn != NULL && PauseRenderOffFn() != 0) {
            pause_rndr_on = 0;
        } else {
            pause_rndr_on = 1;
        }
    }

    if (screendump != 0) {
        pause_rndr_on = 0;
    }
    clear_screen_onstill = 1;
    grab_screen_image = 0;
}

void DrawFadeScreenWipe() {
    NuRndrBeginScene(-1);
    extern numtl_s *SolidMtl;

    FadeSystem &fade_info = FadeSys;
    const f32 fade_amount = fade_info.fade;
    const u32 direction = fade_info.direction;
    i32 gradient[4];
    i32 solid_x = 0;
    i32 solid_y = 0;
    i32 solid_width = 10240;
    i32 solid_height = 3584;

    if ((direction & 3) != 0) {
        i32 gradient_x;
        const bool positive = (direction & 1) != 0;
        if ((positive && fade_info.rate > 0.0f) || (!positive && !(fade_info.rate > 0.0f))) {
            gradient[0] = static_cast<i32>(0x80000000u);
            gradient[1] = 0;
            gradient[2] = static_cast<i32>(0x80000000u);
            gradient[3] = 0;
            solid_width = static_cast<i32>(fade_amount * 10240.0f);
            gradient_x = solid_width;
        } else {
            gradient[0] = 0;
            gradient[1] = static_cast<i32>(0x80000000u);
            gradient[2] = 0;
            gradient[3] = static_cast<i32>(0x80000000u);
            const f32 edge = (1.0f - fade_amount) * 10240.0f;
            solid_x = static_cast<i32>(edge);
            solid_width = 10240 - solid_x;
            gradient_x = static_cast<i32>(edge - 1024.0f);
        }
        NuRndrGradRect2di(gradient_x, 0, 1024, 3584, gradient, FadeMtl2);
    } else if ((direction & 0xc) != 0) {
        // The vertical sign test is the same two-way rate/direction test as
        // the horizontal one, with bit 2 selecting the opposite side.
        i32 gradient_y;
        const bool positive = (direction & 4) == 0;
        if ((positive && fade_info.rate > 0.0f) || (!positive && !(fade_info.rate > 0.0f))) {
            gradient[0] = 0;
            gradient[1] = 0;
            gradient[2] = static_cast<i32>(0x80000000u);
            gradient[3] = static_cast<i32>(0x80000000u);
            const f32 edge = (1.0f - fade_amount) * 3584.0f;
            solid_y = static_cast<i32>(edge);
            solid_height = 3584 - solid_y;
            gradient_y = static_cast<i32>(edge - 358.0f);
        } else {
            gradient[0] = static_cast<i32>(0x80000000u);
            gradient[1] = static_cast<i32>(0x80000000u);
            gradient[2] = 0;
            gradient[3] = 0;
            solid_height = static_cast<i32>(fade_amount * 3584.0f);
            gradient_y = solid_height;
        }
        NuRndrGradRect2di(0, gradient_y, 10240, 358, gradient, FadeMtl2);
    }

    if (!(fade_amount < 0.0f)) {
        NuRndrRect2di(solid_x, solid_y, solid_width, solid_height, 0, SolidMtl);
    }
    NuRndrEndScene();
}

