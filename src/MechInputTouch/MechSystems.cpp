// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/numtx.h"

#include <stddef.h>
#include <string.h>
#include <new>

extern void (*preRenderFlashingHack)(void);
extern void (*postRenderFlashingHack)(void);
void PreRenderFlashHack();
void PostRenderFlashHack();

u8 MechSystems::SkipTextScroll = 0;

// vtable for MechSystems @0x66b320 slot 0x08: returns the static name string
// @0x573c42 (from MechSystems::GetName @0x45def0).
char const *MechSystems::GetName() {
    return "MechSystems";
}

i32 MechInputTouchMenuController::AnyTouchesThisFrame = 0;
bool MechInputTouchMenuController::PackButtonPressed = false;
i32 MechInputTouchMenuController::PackButtonID = -1;

// ==== src/legoapi/render/core/screen.cpp: declarations ====
#include "legoapi/render/core/screen.h"
#include "legoapi/render/light/fade.h"
#include "legoapi/characters/motion.h"
#include "legoapi/menus/core/text.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/world/area.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/characters/motion/gameanim.h"
#include "gamelib/util/gamelib_util_types.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "batman.h"
#include "gameapi/edtools/edgra.h"
#include "legoapi/core/startup/main.h"
#include "nu2api/nucore/nuapi.h"

#include <GLES2/gl2.h>
#include <stdio.h>

f32 GetAspectRatio();

// The fade system and the four fade effects handed to FadeSystem::AddFade()
// are screen.cpp globals in the original: its static initializer registers
// their destructors (original bss @0x127bf20 / 0x127bf08 / 0x127bf00 /
// 0x127bef8 / 0x127bef0).
extern Fade fade;

// Material handles used by the original 2-D fade paths.

u32 animSetVisibilityHack[6];
i32 specVisibilityFlashHack;

void GameAnimSet_Draw(GAMEANIMSET_s &set);
extern f32 hackFlashTimer;
extern GAMEANIMSET_s *hackFlashingGameAnimSet;
extern nuhspecial_s *hackFlashingSpecial;

extern i32 Paused;

// ==== src/legoapi/menus/core/hud.cpp: all remaining content ====
void RndrTexQuad(f32, f32, f32, f32, i32, numtl_s *, i32);
#include "gameframework/saveload.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/panel.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nutrig.h"

extern "C" i32 NewMode;
extern "C" i32 editor_active;
extern "C" i32 CutSceneWaiting;
extern "C" i32 Paused;

namespace {

} // namespace

// ==== src/legoapi/render/core/render.cpp: declarations ====
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/character/transform.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/actions/character/snake.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/menus/core/gamehint.h"
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
#include <math.h>

#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
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
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/numath.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"

struct TouchHolder;
struct nuhspecial_s;
struct VuVec;

extern char *ASCII_DOWN;

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
static NU_READ_GRAPHICS_DATA_ATTR NUGSCN *NuReadGraphicsData(VARIPTR *buf, VARIPTR *buf_end, char *path,
                                                             char *scene_data)
    __asm__("_ZL18NuReadGraphicsDataP9variptr_uS0_PcS1_S1_.isra.104");

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

f32 SwipeDecalRenderer_MaxWidth = 0.6f;
f32 SwipeDecalRenderer_MinWidth = 0.2f;

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

extern i32 GetMenuID(void);
static __used__ i32 PauseRenderOff();

static __used__ i32 MatrixReflection_CanOverride();

extern i32 qrand(void);

static __used__ void BackDrop_Alpha(float *alpha);

// ==== src/legoapi/characters/motion/move.cpp: declarations ====

#include <stdlib.h>
#include "legoapi/actions/movement/jumping.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/gizmos/object/hatmachine.h"
#include "legoapi/characters/motion/action_info.h"

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
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/props/objects/tightrope.h"
#include "legoapi/menus/core/gamemessage.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/numtx_inline.h"

// The retail move.cpp initializer owns these four managed references.

// Original action/context lookup data, with its arrays and pointers in one owner.

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
i32 GetMenuID(void);

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

DECOMP_ASSERT(sizeof(AWKWARDSHAPESPHERE_s) == 12, "Awkward shape sphere size");
DECOMP_ASSERT(offsetof(AWKWARDSHAPESPHERE_s, radius) == 8, "Awkward shape sphere radius offset");

// ==== src/legoapi/characters/motion/gameanim.cpp: declarations ====
#include "gameapi/edtools/edfile.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numusic/sfx.h"

#include <float.h>

// Retail uses translation-unit-local helpers with compiler-inferred private
// calling conventions; keep their shared animation-mode call sites intact.

#include "legoapi/characters/motion/gameanim_types.h"

// ==== src/legoapi/world/levels/episodeV.cpp: declarations ====
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/ai/aisys/aipath.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/ai/core/legoai.h"
#include "legoapi/ai/game/creature.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/gizmos/traps/gizbombgen.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "nu2api/nucore/numechptr.hpp"
#include "nu2api/nu3d/nuspline.h"

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
#include "legoapi/gizmo/object/giztorpedo.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/characters/core/customiser.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/android/nuptl_android.h"

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

extern GameObject_s *Obj;
extern i32 HIGHGAMEOBJECT, VehicleArea;
extern WORLDINFO_s *WORLD;

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
#include "gameapi/edtools/edui.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/gizmos/door/spinner.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/core/input/timing.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"

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
#include "legoapi/actions/combat/fighting.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/characters/core/playeritems.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/misc/utilities.h"

i32 qrand();

DECOMP_ASSERT(offsetof(GameObject_s, field_0x7e4) == 0x7e4, "Quick shoot record offset");
DECOMP_ASSERT(offsetof(GameObject_s, field_0x7e8) == 0x7e8, "Quick shoot record count offset");
DECOMP_ASSERT(offsetof(GameObject_s, quick_shoot_flags) == 0xe2f, "Quick shoot flags offset");
DECOMP_ASSERT(offsetof(GameObject_s, quick_shoot_bolt_id) == 0xe3e, "Quick shoot bolt ID offset");

// ==== src/legoapi/characters/core/players.cpp: declarations ====

#include "legogame/game.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nufile/nufpar.h"

struct MechTouchUIElement;

// --- Helpers moved from world.cpp ---

// ---- Player start spawn entries ----

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

namespace {

} // namespace

#include "legoapi/characters/core/players_types.h"

DECOMP_ASSERT(sizeof(SPECIAL_LAYER_s) == 0xc, "SPECIAL_LAYER_s size");

// ==== src/legoapi/actions/movement/pushing.cpp: declarations ====
#include "legoapi/gizmos/door/push.h"
#include "legoapi/items/base/animpacket.h"

// ==== src/legoapi/characters/motion/camera.cpp: declarations ====
#include "legoapi/cutscenes/minicamcut.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/numouse.h"

#include "legoapi/characters/motion/camera_types.h"
// Original data symbol testmovef is float0.005 at 0x617cb0.

DECOMP_ASSERT(sizeof(VIEWCAM_s) == 0x28, "ViewCam size");
DECOMP_ASSERT(offsetof(VIEWCAM_s, pitch) == 0x10, "ViewCam pitch offset");
DECOMP_ASSERT(offsetof(VIEWCAM_s, gamepad) == 0x24, "ViewCam gamepad offset");

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
#include "legoapi/menus/screens/gamestructure.h"

extern void *CutStopInfo;
extern "C" {
    extern i32 Paused;
    extern i32 NewMode;
    extern i32 CutSceneWaiting;
    extern i32 editor_active;
}

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

// ==== src/legoapi/gizmos/trigger/ai.cpp: all remaining content ====

// ==== src/legoapi/ai/game/creature.cpp: declarations ====

#include "legoapi/ai/game/creature_types.h"

// ==== src/legoapi/ai/game/aipathcnxhelper.cpp: declarations ====

#include <stdint.h>
#include "nu2api/nucore/nulist.h"

// ==== src/legoapi/ai/core/ai_sys.cpp: declarations ====
#include "gameapi/ai/aisys/aiscript_types.h"

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

// Runtime-selected groups appended after the fixed terrain allocation.
#include "legoapi/render/core/terrain_stubs_types.h"

// NuFileExists @nufile (nucore_plain.cpp).
extern "C" i32 NuFileExists(char *name);

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

#include "legoapi/actions/movement/jumping_types.h"

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

// ==== src/legoapi/misc/supportall.cpp: declarations ====

#if defined(__SSE__)
#include <xmmintrin.h>
#endif
#include "legoapi/misc/supportall.h"
#include <stdarg.h>
#include <limits.h>
#include "legoapi/gizmo/base/gizflow.h"

// The reference defines these with dynamic initialization in this unit.
extern MemoryManager theMemoryManager;
#include "legoapi/misc/supportall_types.h"

DECOMP_ASSERT(sizeof(TIMINGBAR_LABEL) == 12, "Timing-bar label size");

void RndrTexQuad3D(VuMtx const &matrix, i32 colour, numtl_s *material);

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

// ==== functions in original address order ====

void MechSystems::Reset(ThingResetData *) {
}

void MechSystems::ExitLevel(WORLDINFO_s *) {
    if (level_ui_elements[0] != NULL) {
        TouchUI().RemoveUIElement(*level_ui_elements[0]);
        delete level_ui_elements[0];
        level_ui_elements[0] = NULL;
    }
    if (level_ui_elements[1] != NULL) {
        TouchUI().RemoveUIElement(*level_ui_elements[1]);
        delete level_ui_elements[1];
        level_ui_elements[1] = NULL;
    }
    if (level_ui_elements[2] != NULL) {
        TouchUI().RemoveUIElement(*level_ui_elements[2]);
        delete level_ui_elements[2];
        level_ui_elements[2] = NULL;
    }
}

void PostRenderFlashHack() {
    if (hackFlashingGameAnimSet != NULL && hackFlashingGameAnimSet->objects != NULL) {
        GAMEANIMOBJ_s *object = hackFlashingGameAnimSet->objects;
        NuSpecialSetVisibility(&object->special, animSetVisibilityHack[0] & 1);
        u32 index = 1;
        while ((object = object->next) != NULL) {
            NuSpecialSetVisibility(&object->special,
                                   (static_cast<i32>(animSetVisibilityHack[index >> 5]) >> (index & 31)) & 1);
            if (++index == 0xc1)
                break;
        }
    }
    if (hackFlashingSpecial != NULL)
        NuSpecialSetVisibility(hackFlashingSpecial, specVisibilityFlashHack);
    if (hackFlashTimer <= 0.0f && hackFlashTimer != 0.0f) {
        hackFlashingSpecial = NULL;
        hackFlashingGameAnimSet = NULL;
    }
}

void PreRenderFlashHack() {
    if (hackFlashingGameAnimSet == NULL && hackFlashingSpecial == NULL)
        return;

    TouchHacks::TintStack tint;
    hackFlashTimer -= FRAMETIME;
    if (TouchHacks::ShouldFlash(hackFlashTimer)) {
        NUCOLOUR3 *colour = TouchHacks::GetFlashColour();
        NuRndrLightingStateCurrent.ambient = *colour;
        NuRndrSetAmbientLightPS(colour);
    }

    if (hackFlashingGameAnimSet != NULL) {
        GameAnimSet_Draw(*hackFlashingGameAnimSet);
        animSetVisibilityHack[1] = 0;
        GAMEANIMOBJ_s *object = hackFlashingGameAnimSet->objects;
        animSetVisibilityHack[0] = 0;
        u32 index = 0;
        while (object != NULL) {
            if (NuSpecialGetVisibilityFn(&object->special) != 0)
                animSetVisibilityHack[index >> 5] |= 1u << (index & 31);
            ++index;
            NuSpecialSetVisibility(&object->special, 0);
            if (index == 0xc0)
                break;
            object = object->next;
        }
    }
    if (hackFlashingSpecial != NULL) {
        NuSpecialDrawAt(hackFlashingSpecial, NuSpecialGetDrawMtx(hackFlashingSpecial));
        specVisibilityFlashHack = NuSpecialGetVisibilityFn(hackFlashingSpecial);
        NuSpecialSetVisibility(hackFlashingSpecial, 0);
    }
}

MechSystems::~MechSystems() {
    NuMtlDestroy(location_ping_material);
    location_ping_material = NULL;
    NuMtlDestroy(swipe_material);
    swipe_material = NULL;
    NuMtlDestroy(tag_hold_background_material);
    tag_hold_background_material = NULL;
    NuMtlDestroy(radar_pulse_material);
    radar_pulse_material = NULL;
    PauseButton().MechTouchUIPauseButton::~MechTouchUIPauseButton();
    PlayerButton().MechTouchUIPlayerButton::~MechTouchUIPlayerButton();
    TouchUI().~MechTouchUI();
}

HudRadarPulse::HudRadarPulse(VuVec const &initial_position) : position(initial_position) {
    pulses[1].delay = 0.2f;
    pulses[2].delay = 0.4f;

    pulses[0].delay_finished = 0;
    pulses[0].finished = 0;
    pulses[0].angle = 0;
    pulses[0].delay = 0.0f;
    pulses[1].delay_finished = 0;
    pulses[0].radius = 0.0f;
    pulses[1].finished = 0;
    pulses[0].speed = 0.75f;
    pulses[1].angle = 0;
    pulses[1].radius = 0.0f;
    pulses[2].delay_finished = 0;
    pulses[1].speed = 0.75f;
    pulses[2].finished = 0;
    pulses[2].radius = 0.0f;
    pulses[2].angle = 0;
    pulses[2].speed = 0.75f;

    active = 1;
    paused = 0;
}

void HudRadarPulse::Process(float delta) {
    if (active) {
        active = 0;
        if (NewMode == 0 && NewLData == NULL && editor_active == 0 && GameTimer.time_elapsed > 0.0f &&
            GameTimer.update_count != 0 && WORLD != NULL && CutSceneWaiting == 0 &&
            (Paused == 0 || GetMenuID() == 0x19 || GetMenuID() == 0x15) &&
            (CUTSTOPGAME == 0 || CutScene_IsSkippable(static_cast<CUTINFO *>(CutStopInfo))) && MiniCutCam == 0 &&
            (paused || (WORLD->current_level != TITLES_LDATA && WORLD->current_level != STATUS_LDATA &&
                        !(WORLD->current_level->flags & 0x400) && memcard_autosavestarted == 0 &&
                        !(memcard_autosavepostdelay > 0.0f) && !(memcard_autosavepredelay > 0.0f))))
            active = 1;
    }
    for (i32 i = 0; i < 3; ++i) {
        HudRadarPulseStage &pulse = pulses[i];
        if (!pulse.delay_finished) {
            pulse.delay -= delta;
            if (pulse.delay <= 0.0f)
                pulse.delay_finished = 1;
        }
        if (pulse.delay_finished && !pulse.finished) {
            pulse.radius += pulse.speed * delta;
            float speed = pulse.speed - 0.4f * delta;
            pulse.speed = speed > 0.0f ? speed : 0.0f;
            pulse.angle += static_cast<i32>(32768.0f * delta);
            if (pulse.angle > 0x7fff)
                pulse.finished = 1;
        }
    }
}

u8 HudRadarPulse::IsFinished() {
    if (pulses[0].delay_finished && pulses[0].finished && pulses[1].delay_finished && pulses[1].finished &&
        pulses[2].delay_finished)
        return pulses[2].finished;
    return 0;
}

SwipeDecalRenderer::SwipeDecalRenderer(TouchHolder &holder, i32 index, SwipeDecalRenderer::Style style) {
    alpha.Initialize();
    width.Initialize();
    this->style = style;

    const NuVec2 &swipe_point = holder.swipe_samples[index].position;
    const f32 swipe_x = (swipe_point.x + 1.0f) * 0.5f;
    const f32 swipe_y = (swipe_point.y + 1.0f) * 0.5f;
    const f32 current_x = (holder.touch_position.x + 1.0f) * 0.5f;
    const f32 current_y = (holder.touch_position.y + 1.0f) * 0.5f;
    const f32 dx = current_x - swipe_x;
    const f32 dy = current_y - swipe_y;
    f32 target_width = NuFsqrt(dx * dx + dy * dy);
    // MAX/MIN expand the inner MIN twice, which gives retail's re-tested clamp branches.
    target_width = MAX(SwipeDecalRenderer_MinWidth, MIN(target_width, SwipeDecalRenderer_MaxWidth));

    width.Start(0.0f, target_width, 0.1f);
    *width.target = 0.0f;
    x = swipe_x;
    y = swipe_y;
    angle = NuAtan2D(dx, dy);
    alpha.Start(0.02f, 1.0f, 0.1f);
    *alpha.target = 0.02f;
}

void SwipeDecalRenderer::Process(float frame_time) {
    width.Process(frame_time);
    alpha.Process(frame_time);

    if (alpha.value > 0.5f && !alpha.IsActive() && !width.IsActive()) {
        alpha.Start(*alpha.target, 0.0f, 0.3f);
        alpha.delay = 0.2f;
    }
}

MoveToMarker::MoveToMarker(MechObjectInterface &object) : target(&object) {
    field_5c = 0.0f;
    field_108_0 = false;
    field_108_1 = false;
    fading_out = false;
    field_108_3 = false;
    blowing_up = false;
    persistent = false;
    object.GetFloorTargetPos(position, -1);
    field_fc = 0.0f;
    field_100 = 0.0f;
    rotation.Start(0.0f, 21845.0f, 0.5f);
    *rotation.target = 0.0f;
    colour.elapsed = 0.0f;
    colour.duration = 0.5f;
    colour.delay = 0.0f;
    colour.from = VuVec(255.0f, 255.0f, 255.0f, 1.0f);
    colour.to = VuVec(0.0f, 85.0f, 253.0f, 1.0f);
    *colour.target = VuVec(255.0f, 255.0f, 255.0f, 1.0f);
    scale.Start(0.0f, 1.0f, 0.5f);
    *scale.target = 0.0f;
    secondary_rotation.Start(0.0f, 32768.0f, 0.5f);
    *secondary_rotation.target = 0.0f;
    height = object.GetHeight();
    if (object.GetObjectType() == 1) {
        temporary_target = true;
        radius.Start(0.0f, object.GetRadius(), 0.2f);
        *radius.target = 0.0f;
        alpha.Start(0.0f, 1.0f, 0.1f);
        *alpha.target = 0.0f;
    }
}

void MoveToMarker::FadeOut() {
    if (!persistent) {
        fading_out = true;
    }
}

void MoveToMarker::Process(float frame_time) {
    field_5c -= frame_time;
    if (fading_out) {
        if (!field_108_3 && field_5c < 0.0f) {
            field_108_3 = true;
            fading_out = false;
            scale.Start(*scale.target, 0.0f, 0.3f);
            alpha.Start(*alpha.target, 0.0f, 0.3f);
            secondary_rotation.Start(*secondary_rotation.target, 0.0f, 0.3f);
            if (target.Get() != NULL && target.Get()->GetObjectType() == 1) {
                radius.Start(*radius.target, 0.0f, 0.7f);
            }
        }
    }
    if (target.Get() != NULL) {
        if (target.Get()->GetObjectType() != 1) {
            target.Get()->GetFloorTargetPos(position, -1);
        }
    }
    rotation.Process(frame_time);
    scale.Process(frame_time);
    colour.Process(frame_time);
    radius.Process(frame_time);
    alpha.Process(frame_time);
    secondary_rotation.Process(frame_time);
    if (!radius.IsActive() && radius.value > 0.0f) {
        if (target.Get() != NULL && target.Get()->GetObjectType() == 1) {
            radius.Start(*radius.target, radius.value * 2.0f, 0.2f);
            alpha.Start(*alpha.target, 0.0f, 0.1f);
        }
        if (field_108_1) {
            FadeOut();
        }
    }
    if (!rotation.IsActive()) {
        field_100 -= static_cast<i32>(frame_time * 65536.0f);
    }
    field_fc += static_cast<i32>(frame_time * 65536.0f);
}

void MoveToMarker::BlowUp() {
    if (blowing_up) {
        return;
    }
    blowing_up = true;
    fading_out = false;
    field_108_3 = false;
    secondary_rotation.Start(*secondary_rotation.target, 16384.0f, 0.3f);
    colour.from.x = colour.target->x;
    colour.from.y = colour.target->y;
    colour.from.z = colour.target->z;
    colour.from.w = colour.target->w;
    colour.to = VuVec(21.0f, 146.0f, 27.0f, 1.0f);
    colour.elapsed = 0.0f;
    colour.duration = 0.2f;
    colour.delay = 0.0f;
    rotation.Start(*rotation.target, 16384.0f, 0.3f);
    scale.Start(*scale.target, 1.0f, 0.3f);
}

MechSystems::MechSystems() {
    new (ui_storage) MechTouchUI();
    new (player_button_storage) MechTouchUIPlayerButton();
    new (pause_button_storage) MechTouchUIPauseButton();
    initialized = 0;
    new (click_to_press_start_tracker_storage) ClickToPressStartGestureTracker();
    gesture_controller = NULL;
    location_ping_material = NULL;
    swipe_material = NULL;
    tag_hold_background_material = NULL;
    radar_pulse_material = NULL;
    memset(move_to_markers, 0, sizeof(move_to_markers));
    swipe_markers[0] = NULL;
    swipe_markers[1] = NULL;
    swipe_markers[2] = NULL;
    swipe_markers[3] = NULL;
    level_ui_elements[0] = NULL;
    level_ui_elements[1] = NULL;
    level_ui_elements[2] = NULL;
    radar_pulses[0] = NULL;
    radar_pulses[1] = NULL;
    radar_pulses[2] = NULL;
    radar_pulses[3] = NULL;
}

void MechSystems::LoadPerm() {
    MechInputTouchVirtualConsoleController::LoadPerm();

    if (location_ping_material == NULL) {
        location_ping_material = NuMtlCreate3D(1);
        NUMTL *material = location_ping_material;
        material->diffuse_color.r = 0.0f;
        material->diffuse_color.g = 0.0f;
        material->diffuse_color.b = 0.0f;
        material->sort_pri = 0xff;
        u8 *raw = reinterpret_cast<u8 *>(material);
        raw[0x41] = static_cast<u8>((raw[0x41] & 0x0f) | 0x60);
        raw[0x40] = static_cast<u8>((raw[0x40] & 0xf0) | 1);
        material->opacity = 0.0f;
        raw[0x42] = static_cast<u8>((raw[0x42] & 0x8c) | 0x12);
        material->tex_id = static_cast<i16>(
            NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_LOCATIONPING"), &permbuffer_ptr, permbuffer_end));
        NuMtlUpdate(location_ping_material);
    }
    if (swipe_material == NULL) {
        swipe_material = NuMtlCreate(1);
        NUMTL *material = swipe_material;
        material->diffuse_color.r = 0.0f;
        material->diffuse_color.g = 0.0f;
        material->diffuse_color.b = 0.0f;
        material->sort_pri = 0xff;
        u8 *raw = reinterpret_cast<u8 *>(material);
        raw[0x41] = static_cast<u8>((raw[0x41] & 0x0f) | 0x60);
        raw[0x40] = static_cast<u8>((raw[0x40] & 0xf0) | 1);
        material->opacity = 0.0f;
        raw[0x42] = static_cast<u8>((raw[0x42] & 0x8c) | 0x12);
        material->tex_id = static_cast<i16>(
            NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_SWIPE"), &permbuffer_ptr, permbuffer_end));
        NuMtlUpdate(swipe_material);
    }
    if (tag_hold_background_material == NULL) {
        tag_hold_background_material = NuMtlCreate(1);
        NUMTL *material = tag_hold_background_material;
        material->diffuse_color.r = 0.0f;
        material->diffuse_color.g = 0.0f;
        material->diffuse_color.b = 0.0f;
        material->sort_pri = 0xff;
        u8 *raw = reinterpret_cast<u8 *>(material);
        raw[0x41] = static_cast<u8>((raw[0x41] & 0x0f) | 0x60);
        raw[0x40] = static_cast<u8>((raw[0x40] & 0xf0) | 1);
        material->opacity = 0.0f;
        raw[0x42] = static_cast<u8>((raw[0x42] & 0x8c) | 0x12);
        material->tex_id = static_cast<i16>(
            NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_TAGHOLDBGTEX"), &permbuffer_ptr, permbuffer_end));
        NuMtlUpdate(tag_hold_background_material);
    }
    if (radar_pulse_material == NULL) {
        radar_pulse_material = NuMtlCreate(1);
        NUMTL *material = radar_pulse_material;
        material->diffuse_color.r = 0.0f;
        material->diffuse_color.g = 0.0f;
        material->diffuse_color.b = 0.0f;
        material->sort_pri = 0xff;
        u8 *raw = reinterpret_cast<u8 *>(material);
        raw[0x41] = static_cast<u8>((raw[0x41] & 0x0f) | 0x60);
        raw[0x40] = static_cast<u8>((raw[0x40] & 0xf0) | 1);
        material->opacity = 0.0f;
        raw[0x42] = static_cast<u8>((raw[0x42] & 0x8c) | 0x12);
        material->tex_id = static_cast<i16>(
            NuTexRead(const_cast<char *>("STUFF/UIBUTTONS/UIBUTTONS_RIPPLE"), &permbuffer_ptr, permbuffer_end));
        NuMtlUpdate(radar_pulse_material);
    }
}

void MechSystems::Init() {
    if (initialized == 0) {
        input_touch_system.Init();
        TouchUI().Init();
        TouchUI().AddUIElement(PlayerButton());
        TouchUI().AddUIElement(PauseButton());
        initialized = 1;
        preRenderFlashingHack = PreRenderFlashHack;
        postRenderFlashingHack = PostRenderFlashHack;
    }
}

void MechSystems::ProcessOnlyWhenPaused(ThingProcessData *) {
    if (initialized == 0) {
        Init();
    }
}

void MechSystems::ProcessEvenWhenPaused(ThingProcessData *data) {
    if (initialized == 0) {
        Init();
    }
    if (radar_pulses[0] != NULL) {
        radar_pulses[0]->Process(FRAMETIME);
        if (radar_pulses[0]->IsFinished()) {
            delete radar_pulses[0];
            radar_pulses[0] = NULL;
        }
    }
    if (radar_pulses[1] != NULL) {
        radar_pulses[1]->Process(FRAMETIME);
        if (radar_pulses[1]->IsFinished()) {
            delete radar_pulses[1];
            radar_pulses[1] = NULL;
        }
    }
    if (radar_pulses[2] != NULL) {
        radar_pulses[2]->Process(FRAMETIME);
        if (radar_pulses[2]->IsFinished()) {
            delete radar_pulses[2];
            radar_pulses[2] = NULL;
        }
    }
    if (radar_pulses[3] != NULL) {
        radar_pulses[3]->Process(FRAMETIME);
        if (radar_pulses[3]->IsFinished()) {
            delete radar_pulses[3];
            radar_pulses[3] = NULL;
        }
    }
    input_touch_system.ProcessEvenWhenPaused(data);
    TouchUI().Process(FRAMETIME);
}

void MechSystems::EnterLevel(WORLDINFO_s *) {
    if (initialized == 0) {
        Init();
    }
    if (initialized != 0) {
        PlayerButton().SetupTargetIds();
    }
}

void MechSystems::Process(ThingProcessData *) {
    if (initialized == 0) {
        Init();
    }
    if (initialized != 0) {
        for (i32 i = 0; i < 32; ++i) {
            if (move_to_markers[i] != NULL) {
                move_to_markers[i]->Process(FRAMETIME);
                if (move_to_markers[i]->field_108_3 && move_to_markers[i]->scale.value <= 0.001f) {
                    delete move_to_markers[i];
                    move_to_markers[i] = NULL;
                }
            }
        }
        for (i32 i = 0; i < 4; ++i) {
            if (swipe_markers[i] != NULL) {
                swipe_markers[i]->Process(FRAMETIME);
                if (swipe_markers[i]->alpha.value < 0.01f) {
                    delete swipe_markers[i];
                    swipe_markers[i] = NULL;
                }
            }
        }
        for (i32 i = 0; i < 3; ++i) {
            if (level_ui_elements[i] != NULL && level_ui_elements[i]->fading_out != 0 &&
                level_ui_elements[i]->first_fade.value <= 0.0f) {
                TouchUI().RemoveUIElement(*level_ui_elements[i]);
                delete level_ui_elements[i];
                level_ui_elements[i] = NULL;
            }
        }
    }
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if (world != NULL && world->mech_auto_jump_manager != NULL) {
        world->mech_auto_jump_manager->Process();
    }
}

MechSystems *MechSystems::Get() {
    static MechSystems *instance = NULL;
    if (instance == NULL) {
        instance = new MechSystems();
    }
    return instance;
}

void HudRadarPulse::Render() {
    if (active) {
        if (pulses[0].delay_finished && !pulses[0].finished) {
            f32 x = position.x + 1.0f;
            f32 y = 1.0f - position.y;
            numtl_s *material = MechSystems::Get()->radar_pulse_material;
            i32 alpha = static_cast<i32>(90.0f * NU_SIN_LUT(pulses[0].angle));
            f32 radius = pulses[0].radius;
            f32 width = GetAspectRatio() * radius;
            RndrTexQuad(x * 0.5f, y * 0.5f, width, radius, static_cast<i32>((static_cast<u32>(alpha) << 24) | 0x808080),
                        material, 0);
        }
        if (pulses[1].delay_finished && !pulses[1].finished) {
            f32 x = position.x + 1.0f;
            f32 y = 1.0f - position.y;
            numtl_s *material = MechSystems::Get()->radar_pulse_material;
            i32 alpha = static_cast<i32>(90.0f * NU_SIN_LUT(pulses[1].angle));
            f32 radius = pulses[1].radius;
            f32 width = GetAspectRatio() * radius;
            RndrTexQuad(x * 0.5f, y * 0.5f, width, radius, static_cast<i32>((static_cast<u32>(alpha) << 24) | 0x808080),
                        material, 0);
        }
        if (pulses[2].delay_finished && !pulses[2].finished) {
            f32 x = position.x + 1.0f;
            f32 y = 1.0f - position.y;
            numtl_s *material = MechSystems::Get()->radar_pulse_material;
            i32 alpha = static_cast<i32>(90.0f * NU_SIN_LUT(pulses[2].angle));
            f32 radius = pulses[2].radius;
            f32 width = GetAspectRatio() * radius;
            RndrTexQuad(x * 0.5f, y * 0.5f, width, radius, static_cast<i32>((static_cast<u32>(alpha) << 24) | 0x808080),
                        material, 0);
        }
    }
}

void SwipeDecalRenderer::Render() {
    NUVEC vertices[4] = {
        {-0.037499998f, 0.0f, 0.0f},
        {0.037499998f, 0.0f, 0.0f},
        {-0.037499998f, -width.value, 0.0f},
        {0.037499998f, -width.value, 0.0f},
    };
    const f32 uv_height = width.value / width.to;

    NuVecRotateZ(&vertices[0], &vertices[0], angle);
    NuVecRotateZ(&vertices[1], &vertices[1], angle);
    NuVecRotateZ(&vertices[2], &vertices[2], angle);
    NuVecRotateZ(&vertices[3], &vertices[3], angle);
    for (i32 i = 0; i < 4; ++i) {
        vertices[i].x += x;
        vertices[i].y += 1.0f - y;
    }

    NuPrim2DBegin(1, 7, MechSystems::Get()->swipe_material);
    u32 colour = (static_cast<u32>(static_cast<i32>(alpha.value * 128.0f)) << 24) | 0x000080;
    if (style != static_cast<Style>(1)) {
        if (__builtin_expect(style == static_cast<Style>(2), 0))
            colour = (static_cast<u32>(static_cast<i32>(alpha.value * 255.0f)) << 24) | 0x0000ff;
        else
            colour |= 0x808000;
    }

    NuRndrPrimSetColour(colour);
    SWIPE_PRIM_UV(0.0f, 0.0f);
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * vertices[0].x, static_cast<f32>(PS2_VREZ_H) * vertices[0].y, 0.0f);
    NuRndrPrimSetColour(colour);
    SWIPE_PRIM_UV(1.0f, 0.0f);
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * vertices[1].x, static_cast<f32>(PS2_VREZ_H) * vertices[1].y, 0.0f);
    NuRndrPrimSetColour(colour);
    SWIPE_PRIM_UV(0.0f, uv_height);
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * vertices[2].x, static_cast<f32>(PS2_VREZ_H) * vertices[2].y, 0.0f);
    NuRndrPrimSetColour(colour);
    SWIPE_PRIM_UV(1.0f, uv_height);
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * vertices[3].x, static_cast<f32>(PS2_VREZ_H) * vertices[3].y, 0.0f);
    NuPrim2DEnd();
}

void MechSystems::Display(ThingRenderData *) {
    TouchUI().Render();
    if (initialized != 0) {
        for (i32 i = 0; i < 4; ++i) {
            if (swipe_markers[i] != NULL) {
                swipe_markers[i]->Render();
            }
        }
        for (i32 i = 0; i < 4; ++i) {
            if (radar_pulses[i] != NULL) {
                radar_pulses[i]->Render();
            }
        }
    }
}

void MoveToMarker::Render() {
    NUVEC draw_position = {position.x, position.y, position.z};
    NUMTX_ALIGNED16 ping_matrix;
    NUMTX_ALIGNED16 text_matrix;
    if (temporary_target) {
        MechSystems *systems = MechSystems::Get();
        if (systems->location_ping_material != NULL) {
            NUVEC ping_scale = {radius.value, radius.value, radius.value};
            NuMtxSetRotationY(&ping_matrix, 0);
            NuMtxRotateX(&ping_matrix, 0x4000);
            NuMtxScale(&ping_matrix, &ping_scale);
            draw_position.y += 0.01f;
            NuMtxTranslate(&ping_matrix, &draw_position);
            extern void RndrTexQuad3D(VuMtx const &, i32, numtl_s *);
            const i32 ping_colour = (static_cast<i32>(alpha.value * 255.0f) << 24) | 0xffffff;
            RndrTexQuad3D(reinterpret_cast<VuMtx const &>(ping_matrix), ping_colour, systems->location_ping_material);
        }
    }
    if (!field_108_0) {

        draw_position.y = NuTrigTable[(static_cast<i32>(rotation.value) >> 1) & 0x7fff] * 0.1f + height + position.y +
                          NuTrigTable[(field_100 >> 1) & 0x7fff] * 0.005f;
        const f32 marker_scale =
            (NuTrigTable[(static_cast<i32>(secondary_rotation.value) >> 1) & 0x7fff] * 0.25f + 1.0f) * 0.1f;
        NUVEC text_scale = {marker_scale, marker_scale, marker_scale};
        NuMtxSetRotationY(&text_matrix, field_fc);
        NuMtxScale(&text_matrix, &text_scale);
        NuMtxTranslate(&text_matrix, &draw_position);

        extern char *ASCII_DOWN;
        NuQFntPushPrintMode(4);
        NuQFntSet(QFont3DZ);
        NuQFntSetMtx(QFont3DZ, &text_matrix);
        NuQFntSetCoordinateSystem(NUQFNT_CSMODE_ABSOLUTE);
        const i32 colour = static_cast<u8>(static_cast<i32>(this->colour.value.x)) |
                           (static_cast<u8>(static_cast<i32>(this->colour.value.y)) << 8) |
                           (static_cast<u8>(static_cast<i32>(this->colour.value.z)) << 16) |
                           (static_cast<i32>(scale.value * 255.0f) << 24);
        NuQFntSetColour(QFont3DZ, colour);
        NuQFntSetScale(QFont3DZ, 0.0375f, 0.05f);
        NuQFntMove(QFont3DZ, NuQFntPrintLenU(QFont3DZ, ASCII_DOWN) * -0.5f, 0.0f, 0.0f);
        NuQFntPrintU(QFont3DZ, ASCII_DOWN);
        NuQFntPopPrintMode();
    }
}

void MechSystems::Render(ThingRenderData *) {
    if (initialized != 0) {
        for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
            if (Obj[i].touch_task != NULL) {
                Obj[i].touch_task->Render();
            }
        }
        for (i32 i = 0; i < 32; ++i) {
            if (move_to_markers[i] != NULL) {
                move_to_markers[i]->Render();
            }
        }
    }
    WORLDINFO *world = WorldInfo_CurrentlyActive();
    if (world != NULL && world->mech_auto_jump_manager != NULL) {
        world->mech_auto_jump_manager->Render();
    }
}

MoveToMarker *MechSystems::NewMoveToMarker(MechObjectInterface &object) {
    if (object.GetObjectType() == 3) {
        return NULL;
    }
    for (i32 i = 0; i < 32; ++i) {
        if (move_to_markers[i] == NULL) {
            MoveToMarker *marker = new MoveToMarker(object);
            move_to_markers[i] = marker;
            return marker;
        }
    }
    return NULL;
}

MoveToMarker *MechSystems::FindMoveToMarkerAtPos(VuVec const &position, bool active_only) {
    for (i32 i = 0; i < 32; ++i) {
        MoveToMarker *marker = move_to_markers[i];
        if (marker == NULL || (active_only && marker->field_108_0)) {
            continue;
        }
        const f32 dx = marker->position.x - position.x;
        const f32 dy = marker->position.y - position.y;
        const f32 dz = marker->position.z - position.z;
        if (dx * dx + dy * dy + dz * dz < 0.4f * 0.4f) {
            return marker;
        }
    }
    return NULL;
}

void MechSystems::NewSwipeMarker(TouchHolder &holder, i32 direction, SwipeDecalRenderer::Style style) {
    i32 slot;
    if (swipe_markers[0] != NULL) {
        if (swipe_markers[1] == NULL) {
            slot = 1;
        } else if (swipe_markers[2] == NULL) {
            slot = 2;
        } else if (swipe_markers[3] == NULL) {
            slot = 3;
        } else {
            return;
        }
    } else {
        slot = 0;
    }
    swipe_markers[slot] = new SwipeDecalRenderer(holder, direction, style);
}

void MechSystems::NewRadarPulse(VuVec const &position, bool paused) {
    i32 slot;
    if (radar_pulses[0] == NULL) {
        slot = 0;
    } else if (radar_pulses[1] == NULL) {
        slot = 1;
    } else if (radar_pulses[2] == NULL) {
        slot = 2;
    } else if (radar_pulses[3] == NULL) {
        slot = 3;
    } else {
        return;
    }
    HudRadarPulse *pulse = new HudRadarPulse(position);
    radar_pulses[slot] = pulse;
    if (paused) {
        pulse->paused = 1;
    }
}

MechTouchUITagButton *MechSystems::NewTagButton(GameObject_s &object, TouchHolder &holder) {
    if (level_ui_elements[0] != NULL && level_ui_elements[0]->target_object.Get() == object.GetMechObjectInterface()) {
        return NULL;
    }
    if (level_ui_elements[1] != NULL && level_ui_elements[1]->target_object.Get() == object.GetMechObjectInterface()) {
        return NULL;
    }
    if (level_ui_elements[2] != NULL && level_ui_elements[2]->target_object.Get() == object.GetMechObjectInterface()) {
        return NULL;
    }

    i32 slot;
    for (slot = 0; slot < 3; slot++) {
        if (level_ui_elements[slot] == NULL) {
            break;
        }
    }
    if (slot == 3) {
        return NULL;
    }
    level_ui_elements[slot] = new MechTouchUITagButton(object, holder);
    TouchUI().AddUIElement(*level_ui_elements[slot]);
    return level_ui_elements[slot];
}

void MechSystems::RenderCurrentPlayerHighlight() {
    if (player != NULL && WORLD->lev_objs[166].active != 0) {
        NUVEC scale = {0.6f, 0.6f, 0.6f};
        NUVEC position = player->apiobj.position;
        position.y = GameShadow(player, &position, 2.0f, -1) + 0.001f;

        NUMTX_ALIGNED16 matrix;
        NuMtxSetRotationY(&matrix, 0);
        NuMtxRotateX(&matrix, 0x4000);
        NuMtxScale(&matrix, &scale);
        NuMtxTranslate(&matrix, &position);
        NuSpecialDrawAtAlpha(&WORLD->lev_objs[166].special, &matrix, 0.25f);
    }
}

void MechSystems::HookUpClickToPressStart() {
    gesture_tracking_system.RegisterGestureTracker(ClickToPressStartTracker(), 0);
}

void MechSystems::UnhookClickToPressStart() {
    gesture_tracking_system.UnregisterGestureTracker(ClickToPressStartTracker());
}

