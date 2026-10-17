// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/world/world.h"

#include <stdio.h>
#include <string.h>

#include "gameapi/edtools/edstubs.h"
#include "gameapi/gui/apimenu.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/world/area.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/world/level.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/characters/motion.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/core/input/timer.h"
#include "legogame/game.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numath/nutrig.h"

struct PLAYERPACKET_s;
struct SOCKPOSITION_s;

// --- Helpers moved from world.cpp ---

static NUVEC HubVehiclesDoorPos[2] = {{-24.21f, 0.0f, -25.36f}, {-23.63f, 0.0f, -25.68f}};
static NUVEC HubMinikitDoorPos[2] = {{-27.14f, 0.0f, -24.92f}, {-26.77f, 0.0f, -24.94f}};

void RememberPlayerIDs(i32 a, i32 b, i32 c);

// ---- Player start spawn entries ----
PLAYERSTARTENTRY PlayerStart[8];

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

void *CutScenePlayer_Available(void);

void (*Player_ClearContextFn)(GameObject_s *, i32);
void Whip_Release(GameObject_s *);
i32 SpecialMove_ReleaseVictim(GameObject_s *);

i32 FULLDEBUGTOGGLE;
i32 LIFTPLAYER;
f32 TOGGLEREPEATTIME = 0.25f;
i32 (*Player_ToggleSubCharacterFn)(GameObject_s *, i32, i32);
void (*Player_ToggledCharacterFn)(GameObject_s *, i32);
extern FadeSystem FadeSys;
extern i32 LEGOHINT_FREEPLAYTOGGLE;
void Hint_SetComplete(i32 hint_id);
void Move_DEFAULT(GameObject_s *object);
void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);
void GameAudio_PlaySfx(i32 sfx, NUVEC *position, i32 flags, i32 volume);

i32 Player_HasDoubleBoltDamage(GameObject_s *object);

u32 (*CanPushObstaclesFn)(GameObject_s *) = NULL;

namespace {

} // namespace

i32 CanPullLevers(i32 id);

#include "legoapi/characters/core/players_types.h"

DECOMP_ASSERT(sizeof(SPECIAL_LAYER_s) == 0xc, "SPECIAL_LAYER_s size");

i32 (*LastSafePosExtraFn)(GameObject_s *) = NULL;
void (*GameObjectDimensionsExtraFn)(GameObject_s *) = NULL;

// ==== src/legoapi/characters/core/character.cpp: declarations ====

i32 CanPullLevers(i32 character_id);

#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufilepak.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numusic/sfx.h"

// Forward declarations for local (static) character/gameplay helper stubs.
struct nuvec_s;

CHARACTERDATA *CDataList = NULL;
extern GAMECHARACTERDATA * GCDataList;
// Original 0x120-byte data record @0x00666960. FixUpCharacters copies this
// canonical prefix into every character before character-specific overrides.
// These values cover the fully typed common movement and perception fields.

static __used__ void ExtraDieSfx_LSW(GameObject_s *object);

static __used__ void ExtraHurtSfx_LSW(GameObject_s *object);

// ==== src/legoapi/characters/core/charconfig.cpp: declarations ====

#include "legoapi/gizmo/base/gizactions.h"
#include <math.h>
#include <stdlib.h>

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

void Move_DEFAULT(GameObject_s *);

extern CHARCONFIG_s charconfig;

// Retail only keeps the clone with strip_comments fixed to 1 (regparm text, filename).
#if defined(__i386__)
#define REDIRECT_TEXT_FILE_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define REDIRECT_TEXT_FILE_ATTR __attribute__((noinline, noclone))
#endif

// ==== src/legoapi/characters/core/characters.cpp: declarations ====

#include "nu2api/nu3d/nuportal.h"
#include "legoapi/actions/character/suit.h"
#include "legoapi/actions/character/streaks.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/gizmos/object/lever.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizmessage.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/props/objects/tightrope.h"
#include "legoapi/gizmos/door/spinner.h"
#include "legoapi/gizmos/trigger/minicut.h"
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/gizmos/trigger/giztimer.h"
#include "legoapi/gizmos/trigger/gizrandom.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/gizmos/door/plugs.h"
#include "legoapi/gizmos/door/push.h"
#include "legoapi/gizmos/fx/edgizshadowmachine.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/world/areas.h"
#include "legoapi/world/mission.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuspecial.h"

#include "legoapi/menus/core/text.h"
#include "legoapi/actions/character/specialmoves.h"
#include "legoapi/audio/audio.h"
#include "legoapi/characters/core/customiser.h"
#include "nu2api/numusic/numusic.h"

// LoadPerm1 is one of the few game-level entry points which wires together
// otherwise C-linkage engine subsystems.  Keep these declarations local: the
// individual subsystem TUs intentionally expose their original plain names.
extern "C" {
    i32 NuFileExists(char *);
    i32 NuStrCpy(char *, const char *);

}

// The original table at 0x006281a0.  These are the material animation
// scripts made available before the permanent things scene is loaded.

extern i32 InCollectList_Index(i32 id, COLLECTID *list, i32 count);
extern i32 Collection_Got(i32 id);

#include "legoapi/characters/core/characters_types.h"

// Retail keeps only the IPA-SRA clone, which receives &*layers.
#if defined(__i386__)
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone, regparm(1)))
#else
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone))
#endif

i32 NewPlayerCharacter(GameObject_s *object, i32 id, i32 old_id, i32);

extern i32 last_area;

void GameAudio_PlaySfx(i32, nuvec_s *, i32, i32);

void LoadPerm1();

// ==== src/legoapi/menus/screens/gamemenuall.cpp: declarations ====

#include "gameapi/edtools/edui.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/gui/apimenu_internal.h"
#include "gameframework/saveload.h"
#include "legoapi/core/config/fileselect.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/panel.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nufloat.h"

extern "C" void PlaySfxById(i32 sfx_id, nuvec_s *position);
extern i32 GAMEDEMO;

// Retail clears these "fast repeat" latches but never sets them.

// ==== src/legoapi/menus/screens/menus.cpp: declarations ====
#include "batman.h"
#include "gamelib/crc/crc.h"

#include "legoapi/render/light/fade.h"

extern u32 GAMEPAD_TOGGLELEFT;
extern u32 GAMEPAD_TOGGLERIGHT;

// ==== src/legoapi/render/core/render.cpp: declarations ====
#include "gameapi/edtools/edbri_internal.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/character/transform.h"
#include "legoapi/actions/character/snake.h"
#include "legoapi/menus/core/gamehint.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/screens/arcade.h"
#include "legoapi/render/light/fade_material.h"
#include "legoapi/render/fx/edsplines.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/android/nuportal_android.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nucore/nuvuvec.hpp"

#include "legoapi/characters/motion/animlist.h"
#include "nu2api/nu3d/nutexanm.h"

#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/numath/numath.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"

extern i32 VehicleArea;

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

void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);

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

// ==== src/gameapi/gui/apimenu.cpp: all remaining content ====

#include "legoapi/core/startup/main.h"
#include "legoapi/menus/screens/movies.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nuvideo.h"

void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 volume, i32 flags);

// ==== src/legoapi/core/input/gamepads.cpp: declarations ====
#include "legoapi/misc/androidbatman.h"
#include "legoapi/items/collect/spacelevel.h"
#include "nu2api/nucore/NuInputDevice.h"

extern GAMECAMERA_s *GameCam;
extern WORLDINFO_s *WORLD;
extern "C" {
    extern "C++" FadeSystem FadeSys;
}

// Original bss @0x127a500: 64 pads x 0x60 bytes.
// Original bss @0x127a4e0.

extern u32 GAMEPAD_LIFT;

// Two frames of direction history for each local player.  MovePlayer shifts
// these before accepting the new input; GamePad_Rotate then reports a stable
// angular velocity only when both consecutive turns agree.

f32 GamePad_Rotate(GameObject_s *object);

// ==== src/gameframework/savesystem.cpp: all remaining content ====

#include "nu2api/nucore/numemory.h"

// ==== src/legoapi/core/config/saveload.cpp: declarations ====

#include "gameapi/edtools/gameapi_edtools_types.h"

extern u8 FS_Active;

// Retail keeps the whole body of RenderFileSel3 as the split-out .part clone;
// the exported function only tests FS_Active.

void RenderFileSel3(i32 draw_title);

// ==== src/legoapi/menus/core/text.cpp: declarations ====
#include "nu2api/nu3d/nushader_plain.h"

// Original .data @0x667ce0; the names are filled in by the static initializer.

// ==== src/legoapi/items/objects/objectsall.cpp: declarations ====

#include "legoapi/actions/movement/jumping.h"
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/gizmo/object/gizmoblowups.h"

// ==== src/gameapi/edtools/edpartall.cpp: all remaining content ====
#include "gameapi/edtools/edpart_internal.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edfile.h"

extern "C" {
    void eduiMenuRender(eduimenu_s *menu);
}

// Retail only keeps the IPA-SRA clone, which takes just the parent menu.

// ==== src/legoapi/render/core/terrain_stubs.cpp: declarations ====
#include "legoapi/render/core/terrain_internal.h"
#include "legoapi/render/core/gameliball.h"
#include "legoapi/render/fx/game_deb.h"
#include "gameapi/edtools/edpp_internal.h"
#include "nu2api/numath/numtx_inline.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/android/nurain_android.h"
#include "nu2api/nu3d/android/nuptl_android.h"

#include <stddef.h>

// Runtime-selected groups appended after the fixed terrain allocation.
#include "legoapi/render/core/terrain_stubs_types.h"

// NuFileExists @nufile (nucore_plain.cpp).
extern "C" i32 NuFileExists(char *name);

// ==== src/legoapi/render/fx/parts.cpp: declarations ====
#include "legoapi/actions/combat/hits.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmo/object/giztorpedo.h"

#include <float.h>

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

WORLDINFO_s *WorldInfo_CurrentlyActive();
extern f32 FRAMETIME;

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

// ==== src/gameapi/edtools/edtoolsall_plain.cpp: all remaining content ====
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "gameapi/edtools/edui_colour.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/nukeyboard.h"
#include "gameapi/edtools/edrender.h"
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
#pragma GCC push_options
#pragma GCC optimize("O2", "omit-frame-pointer")

extern "C" {

static __used__ void cbGradChange(void);

static __used__ void cbSel(eduimenu_s *, eduiitem_s *item);

static __used__ void cbSubMenu(eduimenu_s *menu, eduiitem_s *item);

static __used__ void cbgpcfgAdd(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCPPress(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCopy(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgDel(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgPaste(eduimenu_s *menu, eduiitem_s *item, u32 value);

} // extern "C"

#pragma GCC pop_options

// ---- end src/gameapi/edtools/edmenucallbacks.cpp ----

// ---- begin src/gameapi/edtools/edptlall.cpp ----

// The original particle editor's private callbacks and menu state share this TU.
// ---- begin src/gameapi/edtools/edptlcallbacks.cpp ----
#if defined(__SSE__)
#include <xmmintrin.h>
#endif

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

#include "legoapi/misc/utilities.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nu3d/nuspline.h"
#include <new>

extern EdInputContext * Input;
extern MemoryManager theMemoryManager;
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

// ---- begin src/gameapi/edtools/edmenucallbacks.cpp ----
#include <cstdio>

#pragma GCC push_options
#pragma GCC optimize("O2", "omit-frame-pointer")

extern "C" {

static __used__ void cbGradChange(void);

static __used__ void cbSel(eduimenu_s *, eduiitem_s *item);

static __used__ void cbSubMenu(eduimenu_s *menu, eduiitem_s *item);

static __used__ void cbgpcfgAdd(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCPPress(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCopy(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgDel(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgPaste(eduimenu_s *menu, eduiitem_s *item, u32 value);

} // extern "C"

#pragma GCC pop_options

// ---- end src/gameapi/edtools/edmenucallbacks.cpp ----

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
#include "nu2api/numath/nuvec4.h"
#include "nu2api/nucore/nuonline.h"
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.

#include <stdarg.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/numem.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"

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

struct nupad_s;

// ==== src/gameapi/edtools/edui.cpp: all remaining content ====

// ==== src/legoapi/actions/character/speederchase.cpp: declarations ====
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

#include "legoapi/actions/character/speederchase_types.h"

extern SPEEDERCHASEVALUES speedervals[4];
DECOMP_ASSERT(sizeof(SPEEDERCHASEVALUES) == 0x1c, "Speeder chase tuning stride");
DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

// ==== src/legoapi/characters/motion/camera.cpp: declarations ====
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/numath/nuang.h"
#include "legoapi/cutscenes/minicamcut.h"

#include "legoapi/characters/motion/camera_types.h"
// Original data symbol testmovef is float0.005 at 0x617cb0.

DECOMP_ASSERT(sizeof(VIEWCAM_s) == 0x28, "ViewCam size");
DECOMP_ASSERT(offsetof(VIEWCAM_s, pitch) == 0x10, "ViewCam pitch offset");
DECOMP_ASSERT(offsetof(VIEWCAM_s, gamepad) == 0x24, "ViewCam gamepad offset");

DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, path) == 0x40, "Camera context path offset");
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, flags) == 0x4c, "Camera context flags offset");

GAMEPAD_s *ViewCamGetGamePad();

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

// ==== functions in original address order ====

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
#include "legoapi/ai/core/legoai.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/core/input/timing.h"

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

static __used__ void LEGO_100PercentFn();
static __used__ void LEGO_AllGoldBricksFn();

// ==== src/editor/edtoolsall.cpp: all remaining content ====
#include "editor/edpath.h"
#include "editor/antinode_editor_private.h"
#include "nu2api/nucore/nulist.h"

#include "editor/edtoolsall_types.h"

// ==== functions in original address order ====

// ==== src/legoapi/actions/movement/carrying.cpp: declarations ====

// Original defaults: 0x668d30 and 0x668d40, each a four-byte integer.

void SuperCarry_Release(GameObject_s *object);

// ==== src/legoapi/actions/movement/pushing.cpp: declarations ====
#include "legoapi/items/base/animpacket.h"
#include "legoapi/render/light/surfaces.h"

// ==== functions in original address order ====

void Players_InitPositions(WORLDINFO *world) {
    i32 bonus = 0;
    if (world->area != NULL) {
        bonus = (i32)((world->area->flags >> 2) & 1);
    }

    for (i32 i = 0; i < 8; i++) {
        PlayerStart[i].pos = &v000;
        PlayerStart[i].angle = 0;
    }

    PORTALPOS *A = NULL;
    i32 ninit = 2;
    if (!(world->start_door != NULL && bonus == 0)) {
        if (LEGOSPL_START != -1) {
            A = world->portal_places[LEGOSPL_START];
            ninit = 0;
        }
    } else {
        A = reinterpret_cast<PORTALPOS *>(world->start_door->spline);
    }

    if (A != NULL) {
        NUVEC tmp;
        i32 n = ninit;
        for (i32 i = 0; i < 8; i++) {
            const i32 m = n * 2;
            PlayerStart[i].pos = (NUVEC *)&A->positions[m * 3];
            NuVecSub(&tmp, (NUVEC *)&A->positions[m * 3 + 3], PlayerStart[i].pos);
            PlayerStart[i].angle = NuAtan2D(tmp.x, tmp.z);
            if (n * 2 + 4 > A->count)
                n = ninit;
            else
                n = n + 1;
        }
        if (bonus != 0 && A->count > 3) {
            i16 nc = A->count >> 2;
            i32 r = qrand() / (i32)(0xffff / nc + 1);
            PlayerStart[0].pos = (NUVEC *)&A->positions[12 * r];
            NUVEC *target = (NUVEC *)&A->positions[12 * r + 3];
            PlayerStart[0].angle = NuAtan2D(target->x - PlayerStart[0].pos->x, target->z - PlayerStart[0].pos->z);
            PlayerStart[1].pos = (NUVEC *)&A->positions[12 * r + 6];
            target = (NUVEC *)&A->positions[12 * r + 9];
            PlayerStart[1].angle = NuAtan2D(target->x - PlayerStart[0].pos->x, target->z - PlayerStart[1].pos->z);
        }
    }

    // --- hub exit-door lookup ---
    if (HUB_ADATA != NULL && HUB_ADATA == world->area) {
        if (world->level_sub_id != last_area && hub_from_cutsceneplayer == 0) {
            if (hub_from_superstory != -1) {
                i32 area =
                    Episode_FindAreaFromFlags((EPISODEDATA *)((char *)EDataList + hub_from_superstory * 0x1c), 5, 5);
                void *door = Door_FindByIndex(world, area, -1, NULL);
                if (door != NULL) {
                    NuStrCpy(Door_ExitName, (char *)door);
                }
            } else if (hub_from_mission != -1) {
                void *door = Door_FindByName(world, const_cast<char *>("mission_door"));
                if (door != NULL) {
                    NuStrCpy(Door_ExitName, (char *)door);
                }
            } else if (hub_from_arcade != -1) {
                void *door = Door_FindByName(world, const_cast<char *>("door_to_network"));
                if (door != NULL) {
                    NuStrCpy(Door_ExitName, (char *)door);
                }
            } else if (hub_startoutsidebonusdoor_area != -1) {
                void *door = Door_FindByIndex(world, hub_startoutsidebonusdoor_area, -1, NULL);
                if (door != NULL) {
                    NuStrCpy(Door_ExitName, (char *)door);
                }
            } else if (last_area != -1) {
                i32 area = last_area;
                if (VEHICLES_ADATA != NULL && VEHICLES_ADATA->index == area) {
                    PlayerStart[0].pos = (NUVEC *)&HubVehiclesDoorPos[0];
                    PlayerStart[0].angle = 0x9555;
                    PlayerStart[1].pos = (NUVEC *)&HubVehiclesDoorPos[1];
                    PlayerStart[1].angle = 0x9555;
                } else {
                    AREADATA *area_data = &ADataList[area];
                    if ((area_data->flags & 5) == 5) {
                        PlayerStart[0].pos = (NUVEC *)&HubMinikitDoorPos[0];
                        PlayerStart[0].angle = 0x6e38;
                        PlayerStart[1].pos = (NUVEC *)&HubMinikitDoorPos[1];
                        PlayerStart[1].angle = 0x6e38;
                    } else {
                        if ((area_data->flags & 4) != 0 && area_data->episode_index != AREA_EPISODE_NONE) {
                            i32 episode_area =
                                (i32)Episode_FindAreaFromFlags(&EDataList[area_data->episode_index], 5, 5);
                            if (episode_area != -1) {
                                area = episode_area;
                            }
                        }
                        void *door = Door_FindByIndex(world, area, -1, NULL);
                        if (door != NULL) {
                            NuStrCpy(Door_ExitName, (char *)door);
                        }
                    }
                }
            }
        }
    }
    StartDoorPositions();

    Door_CutSpl = NULL;
    if (Door_ExitCameraSplineName[0] != 0) {
        NUGSPLINE *spl = NuSplineFind(world->current_gscn, Door_ExitCameraSplineName);
        Door_CutSpl = spl;
        if (spl == NULL) {
            Door_UseCutCam = 0;
        } else {
            Door_CutCamPos1 = spl->pts[1];
            Door_CutCamPos0 = spl->pts[0];
        }
    }
    Door_ExitCameraSplineName[0] = 0;

    for (i32 i = 0; i < 8; i++) {
        SOCKPOSITION sp;
        ComplexSockPosition(world->sock_sys, PlayerStart[i].pos, -1, -1, &sp);
        PlayerStart[i].sock_location = sp.location;
        PlayerStart[i].sock_ratio = sp.ratio;
    }
    HubStartDoor = NULL;

    if (HUB_ADATA != NULL && HUB_ADATA == world->area) {
        hub_from_superstory = -1;
        hub_from_mission = -1;
        hub_from_arcade = -1;
        if (hub_from_cutsceneplayer != 0) {
            void *av = CutScenePlayer_Available();
            if (av != NULL && static_cast<CUTSCENEPLAYER_s *>(CutScenePlayer_Available())->return_door != -1) {
                void *door = Door_FindByIndex(
                    world, -1, static_cast<CUTSCENEPLAYER_s *>(CutScenePlayer_Available())->return_door, NULL);
                HubStartDoor = door;
                if (door != NULL) {
                    f32 *vps = *(f32 **)((char *)(*(void **)((char *)door + 0xa0)) + 0x8);
                    NUVEC v2;
                    NuVecSub(&v2, (NUVEC *)&vps[15], (NUVEC *)&vps[12]);
                    PlayerStart[0].pos = (NUVEC *)&vps[12];
                    PlayerStart[0].angle = NuAtan2D(v2.x, v2.z);
                    NuVecSub(&v2, (NUVEC *)&vps[21], (NUVEC *)&vps[18]);
                    PlayerStart[1].pos = (NUVEC *)&vps[18];
                    PlayerStart[1].angle = NuAtan2D(v2.x, v2.z);
                } else {
                    shop_from_cutsceneplayer = hub_from_cutsceneplayer;
                }
            } else {
                shop_from_cutsceneplayer = hub_from_cutsceneplayer;
            }
        }
        hub_from_cutsceneplayer = 0;
    }
}

void RememberPlayerIDs(i32 a, i32 b, i32 c) {
    if (VehicleArea != 0 || GAMEDEMO != 0) {
        return;
    }
    if (a == 0 && (WORLD->current_level->flags & 0x4e2) != 2) {
        return;
    }
    const i32 ids[2] = {b, c};
    for (i32 i = 0; i < 2; ++i) {
        const i32 id = ids[i];
        if (id != -1 && (CDataList[id].model_flags & 0x2000) == 0 && PlayerID[i] != id && Collection_Got(id) == 1 &&
            GCDataList[id].field275_0x116 != 0 && PlayerID[1 - i] != id) {
            PlayerID[i] = id;
        }
    }
    if (b != c && PlayerID[1] == b && PlayerID[0] == c) {
        PlayerID[0] = b;
        PlayerID[1] = c;
    }
}

GameObject_s *Player_FindByID(i32 id) {
    for (i32 i = 0; i < 8; i++) {
        GameObject_s *object = Player[i];
        if (object != NULL && (object->apiobj.field_0x1f8 & 0x1001) == 0x1001 && object->id == id)
            return object;
    }
    return NULL;
}

NUVEC *Player_StartPos(GameObject_s *obj) {
    i32 index = obj->apiobj.field_0x27c;
    if (static_cast<u8>(index) > 7) {
        index = obj->apiobj.field_0x289;
    }
    index &= 7;
    return PlayerStart[index].pos != NULL ? PlayerStart[index].pos : PlayerStart[0].pos;
}

i32 Players_AveragePos(nuvec_s *position, SOCKPOSITION_s *socket_position) {
    NUVEC total = {0.0f, 0.0f, 0.0f};
    f32 player_count = 0.0f;

    if (Player[0] != NULL && Player[0]->apiobj.player_controlled) {
        NuVecAdd(&total, &total, &Player[0]->apiobj.position);
        player_count += 1.0f;
    }
    if (Player[1] != NULL && Player[1]->apiobj.player_controlled) {
        NuVecAdd(&total, &total, &Player[1]->apiobj.position);
        player_count += 1.0f;
    }

    if (player_count > 0.0f) {
        NuVecScale(position, &total, 1.0f / player_count);
        if (socket_position != NULL) {
            ComplexSockPosition(WORLD->sock_sys, position, -1, -1, socket_position);
        }
        return 1;
    }

    if (socket_position != NULL) {
        socket_position->location.sock = -1;
    }
    return 0;
}

void SetPlayer() {
    if (Player[0] != NULL && Player[0]->apiobj.player_controlled) {
        player = Player[0];
        if (Player[1] != NULL && Player[1]->apiobj.player_controlled) {
            player2 = Player[1];
        } else {
            player2 = NULL;
        }
    } else if (Player[1] != NULL && Player[1]->apiobj.player_controlled) {
        player = Player[1];
        player2 = NULL;
    } else {
        player = NULL;
    }
}

i32 CanPullLevers(i32 character_id) {
    u32 flags = CDataList[character_id].model_flags;
    if ((flags & 0x01000010) == 0x01000010) {
        return 0;
    }
    return (flags & 0x00040088) != 0;
}

i32 CanWearHatsInFreePlay(i32 character_id) {
    if ((GCDataList[character_id].flags_090 & 0x10) != 0) {
        return 0;
    }
    return CanPullLevers(character_id) != 0;
}

void Player_ClearContext(GameObject_s *object, i32 mode) {
    if (Player_ClearContextFn != NULL)
        Player_ClearContextFn(object, mode);
    Whip_Release(object);
    SuperCarry_Release(object);
    SpecialMove_ReleaseVictim(object);
    object->movement_runtime_flags &= ~0x0c;
}

void Player_ResetContexts(PLAYERPACKET_s *packet) {
    f32 context_blend = 0.0f;

    packet->context_animation_time = 0.0f;
    packet->context_target = -1;
    if ((packet->animation_flags & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) != 0) {
        context_blend = 1.0f;
    }

    packet->context_distance = 5.0f;
    packet->field_0x670 = 0;
    packet->field_0x678 = 0;
    packet->field_0x668 = 0;
    packet->field_0x77e = 0;
    packet->context_blend = context_blend;
    packet->build_context = -1;
    packet->action_movement_variant = 0;
    packet->action_movement_state = 0;
    packet->gamepad->allocated_5a &= static_cast<u8>(~0x04);
    packet->gamepad->allocated_5a &= static_cast<u8>(~0x10);
    packet->input_state = 0;
    packet->secondary_flags &= static_cast<u8>(~0x04);
    packet->field_0x648 = 0;

    const i32 random_context = qrand();
    packet->field_0x6a0 = 0;
    packet->field_0x6a4 = 0;
    packet->field_0x674 = 0;
    packet->field_0x6e0 = 0;
    packet->field_0x6f8 = 0;
    packet->field_0x710 = 0;
    packet->field_0x724 = 0;
    packet->field_0x730 = 0;
    packet->field_0x734 = 0;
    packet->context_animation = 0;
    packet->movement_angle_0 = -1;
    packet->context_mode = 0;
    packet->movement_angle_2 = -1;
    packet->random_context = static_cast<u8>(random_context / 0x8000);
    packet->movement_angle_3 = -1;
    packet->field_0x77d = 0;
    packet->linked_object = NULL;
}

i32 MovingBackwards(GameObject_s *object) {
    GAMEPAD_s *pad = object->pad_gamepad;
    if (pad->operator_data == NULL || pad->input_magnitude == 0.0f) {
        return 0;
    }

    const i32 difference = RotDiff(static_cast<u16>(pad->input_angle + GameCam->input_yaw), object->apiobj.field_0x276);
    return static_cast<u32>(difference + 0x4000) > 0x8000;
}

void PushAway(NUVEC *position, f32 radius, NUVEC *minimum, NUVEC *maximum, GameObject_s *object, GameObject_s *excluded,
              f32 speed_multiplier, u32 flags) {
    i32 count = 1;
    if (object == NULL) {
        object = Obj;
        count = HIGHGAMEOBJECT;
    }
    NUVEC bounds_min, bounds_max;
    if (maximum == NULL || minimum == NULL) {
        bounds_min.x = position->x - radius;
        bounds_min.y = position->y - radius;
        bounds_min.z = position->z - radius;
        bounds_max.x = position->x + radius;
        bounds_max.y = position->y + radius;
        bounds_max.z = position->z + radius;
        minimum = &bounds_min;
        maximum = &bounds_max;
    }
    for (i32 i = 0; i < count; ++i, ++object) {
        if (object == excluded || (object->apiobj.field_0x1f8 & 0x1001) != 0x1001 || object->apiobj.field_0x287 != 0 ||
            (object->field_0xe20 & 0x20) != 0)
            continue;
        if ((flags & 2) != 0 && object->apiobj.field_0x27d == 0)
            continue;
        if ((CInfo[object->character_context].flags & 0x40000000) != 0)
            continue;
        GAMECHARACTERDATA *character = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
        if ((character->flags_090 & 0x8000) != 0)
            continue;
        if (minimum->x > object->apiobj.collision_max.x || object->apiobj.collision_min.x > maximum->x ||
            minimum->z > object->apiobj.collision_max.z || object->apiobj.collision_min.z > maximum->z)
            continue;
        if ((flags & 1) != 0 &&
            (minimum->y > object->apiobj.collision_max.y || object->apiobj.collision_min.y > maximum->y))
            continue;
        f32 dx = object->apiobj.position.x - position->x;
        f32 dz = object->apiobj.position.z - position->z;
        f32 combined_radius = radius + object->apiobj.field_0x1dc;
        if (dx * dx + dz * dz < combined_radius * combined_radius) {
            u16 angle;
            if (dz == 0.0f && dx == 0.0f)
                angle = qrand();
            else
                angle = NuAtan2D(dx, dz);
            f32 speed = speed_multiplier *
                        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->run_speed;
            if (speed < 1.0f)
                speed = 1.0f;
            else if (speed > 3.0f)
                speed = 3.0f;
            f32 target_x = speed * NU_SIN_LUT(angle);
            f32 target_z = speed * NU_COS_LUT(angle);
            object->apiobj.velocity.x = SeekValF(object->apiobj.velocity.x, target_x, 10.0f);
            object->apiobj.velocity.z = SeekValF(object->apiobj.velocity.z, target_z, 10.0f);
        }
    }
}

bool FindNearestPlayerToVec(nuvec_s *position, GameObject_s **nearest_player, float &distance_squared,
                            bool require_character_flags, u32 character_flags) {
    *nearest_player = NULL;
    distance_squared = 0.0f;

#define CONSIDER_PLAYER_DISTANCE(index, filtered)                                                                      \
    do {                                                                                                               \
        GameObject_s *candidate = Player[index];                                                                       \
        if (candidate != NULL && static_cast<i8>(candidate->apiobj.flags_low) < 0) {                                   \
            const f32 candidate_distance = NuVecDistSqr(&candidate->apiobj.position, position, NULL);                  \
            if (candidate_distance < distance_squared || *nearest_player == NULL) {                                    \
                candidate = Player[index];                                                                             \
                if (!(filtered) ||                                                                                     \
                    (candidate->apiobj.character_data->game_character->flags_090 & character_flags) != 0) {            \
                    distance_squared = candidate_distance;                                                             \
                    *nearest_player = candidate;                                                                       \
                }                                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)

    for (i32 i = 0; i < 8; i++) {
        CONSIDER_PLAYER_DISTANCE(i, require_character_flags);
    }

#undef CONSIDER_PLAYER_DISTANCE

    return *nearest_player != NULL;
}

bool FindFurthestPlayerFromVec(nuvec_s *position, GameObject_s **furthest_player, float &distance_squared,
                               bool require_character_flags, u32 character_flags) {
    *furthest_player = NULL;
    distance_squared = 0.0f;

#define CONSIDER_PLAYER_DISTANCE(index, filtered)                                                                      \
    do {                                                                                                               \
        GameObject_s *candidate = Player[index];                                                                       \
        if (candidate != NULL && static_cast<i8>(candidate->apiobj.flags_low) < 0) {                                   \
            const f32 candidate_distance = NuVecDistSqr(&candidate->apiobj.position, position, NULL);                  \
            if (candidate_distance > distance_squared || *furthest_player == NULL) {                                   \
                candidate = Player[index];                                                                             \
                if (!(filtered) ||                                                                                     \
                    (candidate->apiobj.character_data->game_character->flags_090 & character_flags) != 0) {            \
                    distance_squared = candidate_distance;                                                             \
                    *furthest_player = candidate;                                                                      \
                }                                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)

    for (i32 i = 0; i < 8; i++) {
        CONSIDER_PLAYER_DISTANCE(i, require_character_flags);
    }

#undef CONSIDER_PLAYER_DISTANCE

    return *furthest_player != NULL;
}

void Player_ToggleCharacter(GameObject_s *object, i32 direction, i32 sound) {
    f32 hold_time = object->input_toggle_hold_time;
    WORLDINFO *world = WorldInfo_CurrentlyActive();
    FULLDEBUGTOGGLE = 0;
    if ((HUB_ADATA != NULL && HUB_ADATA == world->area) || FadeSys.fade != 0.0f ||
        (CInfo[object->character_context].flags & 0x100) != 0 || object->field_0xcc0 != NULL || FreePlay == 0) {
        return;
    }
    if ((MiniCutCam != 0 && (object->apiobj.field_0x1f8 & 0x180) == 0x80) || object->apiobj.field_0x287 != 0 ||
        object->pad_gamepad == ViewCamGetGamePad()) {
        object->input_toggle_hold_time = TOGGLEHOLDTIME;
        return;
    }
    i16 old_id = object->id;
    i32 left;
    i32 right;
    if (direction != 0) {
        right = direction > 0;
        left = direction <= 0;
    } else {
        u32 right_mask = GAMEPAD_TOGGLERIGHT;
        u32 left_mask = GAMEPAD_TOGGLELEFT;
        if (LIFTPLAYER != 0) {
            right_mask &= ~GAMEPAD_LIFT;
            left_mask &= ~GAMEPAD_LIFT;
        }
        GAMEPAD_s *pad = object->pad_gamepad;
        right = pad->buttons_pressed & right_mask;
        left = pad->buttons_pressed & left_mask;
        if ((right != 0 && left != 0) || (left | right) == 0) {
            if ((object->apiobj.flags_low & 0x80) == 0) {
                return;
            }
            right = pad->buttons_held & right_mask;
            left = pad->buttons_held & left_mask;
            if ((right != 0 && left != 0) || (left | right) == 0) {
                object->input_toggle_hold_time = TOGGLEHOLDTIME;
                return;
            }
            object->input_toggle_hold_time -= FRAMETIME;
            if (!(object->input_toggle_hold_time <= 0.0f)) {
                return;
            }
            object->input_toggle_hold_time = TOGGLEREPEATTIME;
        }
        hold_time = object->input_toggle_hold_time;
    }
    if (Player_ToggleSubCharacterFn != NULL && Player_ToggleSubCharacterFn(object, left, sound) != 0) {
        return;
    }
    i32 index = 0;
    while (index < apicharsys->loaded_model_count && object->id != apicharsys->models[index].model_id) {
        ++index;
    }
    if (index == apicharsys->loaded_model_count) {
        return;
    }
    i32 attempts = 0;
    i32 id;
    do {
        if (left != 0) {
            if (--index == -1) {
                index = apicharsys->loaded_model_count - 1;
            }
        } else if (++index == apicharsys->loaded_model_count) {
            index = 0;
        }
        id = apicharsys->models[index].model_id;
        ++attempts;
        i32 collected = InCollectList_Index(id, NULL, 0);
        if ((apicharsys->models[index].flags & 1) == 0) {
            goto next_character;
        }
        if (collected == -1 && (i32)GCDataList[id].flags_090 >= 0 && (GCDataList[id].flags_094[3] & 1) == 0 &&
            !(VehicleArea != 0 && BonusArea != 0 && Cheats_CheckFlags(0x100) != 0 &&
              (GCDataList[id].flags_094[3] & 2) != 0)) {
            goto next_character;
        }
        if (VehicleArea != 0) {
            if ((CDataList[id].model_flags & 0x2000) == 0 &&
                !(BonusArea != 0 && (CDataList[id].model_flags & 0x4000000) != 0) &&
                (i32)GCDataList[id].flags_090 >= 0 &&
                !(BonusArea != 0 && Cheats_CheckFlags(0x100) != 0 && (GCDataList[id].flags_094[3] & 2) != 0)) {
                goto next_character;
            }
            if (BonusArea != 0) {
                i32 area = AreaFromMiniKitID(id);
                if (area != -1) {
                    if (Game_AreaSave == NULL || Game_AreaSave[area].minikit_complete == 0) {
                        goto next_character;
                    }
                } else if ((i32)GCDataList[id].flags_090 >= 0 && !(BonusArea != 0 && Cheats_CheckFlags(0x100) != 0 &&
                                                                   (GCDataList[id].flags_094[3] & 2) != 0)) {
                    goto next_character;
                }
                if (object->apiobj.field_0x27f <= 16 && (TerLayer[(i8)object->apiobj.field_0x27f].flags & 1) != 0 &&
                    GCDataList[id].field_0x28 <= 0.0f) {
                    goto next_character;
                }
            }
        } else if ((CDataList[id].model_flags & 0x2000) != 0 || (GCDataList[id].flags_090 & 0x40) != 0) {
            goto next_character;
        }
        if (BonusArea == 0 || VehicleArea == 0) {
            if ((i32)GCDataList[id].flags_090 < 0) {
                if ((object->apiobj.flags_low & 0x80) == 0 || Cheats_CheckFlags(0x100) == 0) {
                    goto next_character;
                }
            } else if ((GCDataList[id].flags_094[3] & 1) == 0 && (collected == -1 || Collection_Got(id) == 0)) {
                goto next_character;
            }
        }
        if (object->apiobj.field_0x218 != 2000000.0f && object->apiobj.field_0x220 != 2000000.0f &&
            object->apiobj.character_data->move_fn != Move_DEFAULT &&
            CDataList[id].bounds_max_y - CDataList[id].bounds_min_y >=
                object->apiobj.field_0x220 - object->apiobj.field_0x218) {
            GameAudio_PlaySfx(0x32, &object->apiobj.collision_position, 0, 0);
            goto next_character;
        }
        if (attempts >= apicharsys->loaded_model_count) {
            return;
        }
        if ((object->apiobj.flags_low & 0x80) != 0) {
            Hint_SetComplete(LEGOHINT_FREEPLAYTOGGLE);
            if (VehicleArea == 0) {
                GameCam_Blend(GameCam, 1.0f, 0.0f, 1);
            }
        }
        NewPlayerCharacter(object, id, old_id, 1);
        RememberPlayerIDs(0, Player[0] != NULL ? Player[0]->id : -1, Player[1] != NULL ? Player[1]->id : -1);
        if (Player_ToggledCharacterFn != NULL) {
            Player_ToggledCharacterFn(object, left);
        }
        object->input_toggle_hold_time = hold_time;
        if (sound != 0) {
            if (left != 0) {
                GameAudio_PlaySfx(0x23, &object->apiobj.collision_position, 0, 0);
            } else if (right != 0) {
                GameAudio_PlaySfx(0x24, &object->apiobj.collision_position, 0, 0);
            }
        }
        return;
    next_character:;
    } while (attempts <= apicharsys->loaded_model_count && id != object->id);
}

i32 Players_BothActive() {
    return Player[0] != NULL && Player[0]->apiobj.player_controlled && Player[1] != NULL &&
           Player[1]->apiobj.player_controlled;
}

i32 Player_HasInvincibility(GameObject_s *object) {
    return Cheats_CheckFlags(0x80) != 0 || (object != NULL && object->field_0xdec > 0.0f);
}

i32 Player_HasFastBuild(GameObject_s *player) {
    return Cheats_CheckFlags(0x4000) != 0 || (player != NULL && player->field_0xdec > 0.0f);
}

i32 Player_HasDeflectBolts(GameObject_s *object) {
    return Cheats_CheckFlags(0x80000) != 0 || (object != NULL && object->field_0xdec > 0.0f);
}

i32 Player_HasDoubleBoltDamage(GameObject_s *object) {
    return Cheats_CheckFlags(2) != 0 || (object != NULL && object->field_0xdec > 0.0f);
}

i32 Player_HasDoubleBoltDamage_FromBolt(BOLT_s *bolt) {
    i32 player;
    if (bolt->flags & 1)
        player = 0;
    else if (bolt->flags & 2)
        player = 1;
    else
        player = -1;
    if (player != -1)
        return Player_HasDoubleBoltDamage(Player[player]);
    return 0;
}

i32 Player_HasDoubleWeaponDamage(GameObject_s *object) {
    return Cheats_CheckFlags(0x400) != 0 || (object != NULL && object->field_0xdec > 0.0f);
}

