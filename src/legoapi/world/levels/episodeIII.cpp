// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include <string.h>
#include <stdio.h>
#include "decomp.h"
#include "legoapi/world/level.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/collect/spacelevel.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "gameapi/ai/aisys/aisys.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numusic/numusic.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/chris.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/render.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/world/levels/episodeIII_types.h"
#include "legoapi/actions/movement/jumping.h"
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/characters/motion/gameanim.h"
#include "nu2api/nufile/nufile.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpart_internal.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edfile.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/numusic/sfx.h"
#include "legoapi/render/core/terrain_internal.h"
#include "legoapi/render/core/gameliball.h"
#include "legoapi/render/fx/game_deb.h"
#include "gameapi/edtools/edpp_internal.h"
#include "nu2api/numath/numtx_inline.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/android/nurain_android.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/nu3d/nurndr.h"
#include <math.h>
#include <stddef.h>
#include "legoapi/render/core/terrain_stubs_types.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmo/object/giztorpedo.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/characters/core/customiser.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/core/input/gamepads.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "legoapi/render/fx/edsplines.h"
#include <float.h>
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "gameapi/edtools/edui_colour.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/nupad.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edgra_internal.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include "nu2api/nucore/nuvideo.h"
#include <stdlib.h>
#include "gameframework/saveload.h"
#include <cstdio>
#include "gameapi/edtools/edtools_types.h"
#include "legoapi/misc/utilities.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nu3d/nuspline.h"
#include <new>
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/gui/apimenu_internal.h"
#include "legoapi/core/config/fileselect.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/audio/audio.h"
#include "legoapi/menus/core/text.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/nucore/nuonline.h"
#include <stdarg.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/numem.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "java/android.h"
#include "java/java.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/NuInputManager.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nuscreen.hpp"
#include "nu2api/numath/nuquat.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/menus/core/gamehint.h"
#include "legogame/game.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/actions/character/speederchase_types.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/cutscenes/minicamcut.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/characters/motion/camera_types.h"
#include "nu2api/nufile/nufilepak.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/actions/character/transform.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/actions/character/snake.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/menus/screens/arcade.h"
#include "legoapi/render/light/fade_material.h"
#include "nu2api/nu3d/android/nuportal_android.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/numath/numath.h"
#include "nu2api/nu3d/nupostparams.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/object/lever.h"
#include "editor/edlevelall.h"
#include "legoapi/world/levels/episodeIII.h"
#include "nu2api/nu3d/nu3d_includes.h"

extern i32 LevFlag[4];

extern "C" {
    void *AIPAthFindPathCnx(AISYS_s *, AIPATH_s *, char *, char *, i32 *);
}

extern i16 id_PALPATINE;
extern i16 id_WOOKIEE;

static GIZAIMESSAGE_s *KashyyykA_msg_TotalWookies;
static NUVEC TempleC_StreamStatusPos = {52.57f, 0.76f, -16.54f};
static GIZAIMESSAGE_s *KashyyykA_msg_WookiesToRescue;
static GameObject_s *Grievous_obj; // current Grievous boss object
static f32 BoulderWait;
static i32 i_boulder;
PART_s *boulder_part[2];
i32 boulder_blowup_type = -1;

DECOMP_ASSERT(sizeof(VADERA_s) == 0x30, "VADERA_s size");
DECOMP_ASSERT(offsetof(VADERA_s, big_jump_locator) == 0x24, "VADERA_s locator offset");

static VADERA_s vader_a;
static GIZAIMESSAGE_s *vader_b_complete_msg; // Vader B "complete" message handle
static u8 vader_b_playersDead;               // Vader B player-death flag


DECOMP_ASSERT(offsetof(CRUISERC_s, count_dooku) == 4, "CRUISERC_s Dooku offset");

extern "C" {
    CRUISERCNETPACKET_s *crusiserc_netpacket = NULL;
}
static CRUISERC_s cruiser_c;

DECOMP_ASSERT(sizeof(CRUISERDNETPACKET_s) == 12, "Cruiser D packet size");

extern "C" {
    CRUISERDNETPACKET_s *cruiserd_netpacket;
}
static i32 CruiserE_ix;
static nuhspecial_s CruiserD_Lift;
static nuinstanim_s *CruiserD_LiftAnim;
static i32 CruiserD_Lift_plat_id;
static GIZAIMESSAGE_s *CruiserD_LiftChase_msg;
static f32 CruiserD_frame = 1.0f;
static i32 CruiserD_direction = 1;

// Episode 3 level handlers, in the game's Episode_III progression:
// dogfight / cruiser / grievous / kashyyyk / temple / vader / a-new-hope.

// ===========================================================================
// Dogfight (Dogfight_A)
// ===========================================================================


speedup_s DogFightSpeedList[] = {
    {58.0f, 0.5f},  {72.0f, 1.0f},  {174.0f, 0.5f}, {183.0f, 1.0f}, {207.0f, 0.5f},
    {220.0f, 1.0f}, {313.0f, 0.5f}, {335.0f, 1.0f}, {0.0f, 0.0f},
};

// ===========================================================================
// Cruiser (Cruiser_A / Cruiser_C / Cruiser_D)
// ===========================================================================









// ===========================================================================
// Grievous (Grievous_A)
// ===========================================================================




// ===========================================================================
// Kashyyyk (Kashyyyk_A / Kashyyyk_B / Kashyyyk_C / Kashyyyk_D)
// ===========================================================================













// ===========================================================================
// Temple (Temple_A / Temple_C)
// ===========================================================================




// ===========================================================================
// Vader (Vader_A / Vader_B / Vader_C)
// ===========================================================================

void *vadera_netpacket;

static NUVEC vadar_cam_pos = {0.0f, 0.88f, -12.4f};
static NUVEC vadar_cam_tgt = {0.0f, 2.0f, -17.4f};




i32 Vader_ObiWanKilledAnakin;
void *vaderc_netpacket;












// ===========================================================================
// A New Hope (ANewHope_A)
// ===========================================================================


// ==== src/legoapi/items/objects/objectsall.cpp: declarations ====
// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.



struct AIROW_s;
struct nunativegscene_s;
struct SHOPINPUT;




NUVEC boulder_oldpos[2];







// ==== src/gameapi/edtools/edpartall.cpp: all remaining content ====


extern "C" {
}

extern "C" {
}

// Retail only keeps the IPA-SRA clone, which takes just the parent menu.

// Retail only keeps the IPA-SRA clone, which takes just the parent menu.

extern "C" {
}

// ==== src/legoapi/render/core/terrain_stubs.cpp: declarations ====


// Runtime-selected groups appended after the fixed terrain allocation.
extern "C" {
}



extern "C" {
}


extern "C" {
    extern "C++" void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
}


// NuFileExists @nufile (nucore_plain.cpp).




extern "C" {

void Debris(i32 update_panel_time);

} // extern "C"

extern "C" {
















extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId();



} // extern "C"

// ==== src/legoapi/render/fx/parts.cpp: declarations ====



struct rtlset;

extern "C" {
    // Shared suspend flag consulted by all timed debris emitters.


}


// Forward declarations for local (static) part/gizmo helper stubs.
struct spacelevel_s;
struct quickboltinfo;

// Local (static) part (PART_s), pickup (GIZMOPICKUP_s), gizmo flow
// (GIZFLOW_s/FLOWBOX_s), power-up and space/starfighter helpers. Stubbed as
// local `t` symbols matching res/libTTapp.so.







extern "C" {

















    // The editor loader clears these page IDs with aligned vector stores.









} // extern "C"



extern "C" {
}

extern "C" {

}










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


// Editor UI callbacks retained in the original editor's menu code.

// ---- end src/gameapi/edtools/edmenucallbacks.cpp ----


// ---- begin src/gameapi/edtools/edptlall.cpp ----

extern "C" {
}

// The original particle editor's private callbacks and menu state share this TU.
// ---- begin src/gameapi/edtools/edptlcallbacks.cpp ----
#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
}

extern "C" {
}

// Particle-list editor UI/menu callbacks.

// Particle editor UI/menu callbacks.

// ---- end src/gameapi/edtools/edptlcallbacks.cpp ----

// Particle list editor subsystem stubs (static, internal linkage).


// ---- end src/gameapi/edtools/edptlall.cpp ----

extern "C" {
}

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

// Only eduiMenuRender calls the integer clip helper, always left-aligned.


extern "C" {
    // Retail only has IPA-SRA clones of these two helpers.

    // GCC 4.7 emits these file-scope objects in reverse declaration order, so
    // each group below mirrors the original linked order in reverse.



}


extern "C" {
}


extern "C" {
}

extern "C" {
}



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

// ==== src/gameapi/edtools/edtoolsall.cpp: declarations ====
// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern EdInputContext * Input;


extern f32 Scale;
DECOMP_ASSERT(sizeof(EdManipulator) == 0x6c, "EdManipulator ABI");

extern "C" {
}




extern "C" {
}


















extern EdEnumControl::Item OpenClosedItems[3];
extern EdEnumControl::Item OnOffItems[3];
extern EdEnumControl::Item YesNoItems[3];

extern i32 AxisColour[8];

inline EdMatrixControl::~EdMatrixControl() {
    Destroy();
}

inline void EdMatrixControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdMatrixControl));
}

inline void EdStringControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdStringControl));
}

extern f32 MouseScale;

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

// ==== src/gameapi/edtools/edtoolsall_plain.cpp: declarations ====

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

// Remaining animation-editor UI/menu callbacks.

// ---- end src/gameapi/edtools/edanimall.cpp ----
// ---- begin src/gameapi/edtools/edmenucallbacks.cpp ----

// Editor UI callbacks retained in the original editor's menu code.

// ---- end src/gameapi/edtools/edmenucallbacks.cpp ----

// ---- begin src/gameapi/edtools/edptlall.cpp ----

extern "C" {
}

// The original particle editor's private callbacks and menu state share this TU.
// ---- begin src/gameapi/edtools/edptlcallbacks.cpp ----
#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
}

extern "C" {
}

// Particle-list editor UI/menu callbacks.

// Particle editor UI/menu callbacks.

// ---- end src/gameapi/edtools/edptlcallbacks.cpp ----

// Particle list editor subsystem stubs (static, internal linkage).

// ---- end src/gameapi/edtools/edptlall.cpp ----

extern "C" {
}

// The original local font helpers read the same private draw state as the
// editor callbacks, so they belong to this translation unit.

// Left-aligned (16) specialisation of the integer print helper.

// Centred (64) specialisation of the integer print helper.

// Only caller prints the property edit buffer left-aligned.

// Only eduiMenuRender calls the integer clip helper, always left-aligned.


extern "C" {
    // Retail only has IPA-SRA clones of these two helpers.

    // GCC 4.7 emits these file-scope objects in reverse declaration order, so
    // each group below mirrors the original linked order in reverse.



}

extern "C" {
}

extern "C" {
}

extern "C" {
}

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

// Text-picker commit callbacks. Retail keeps them unmangled and local.

// ==== src/legoapi/menus/screens/gamemenuall.cpp: declarations ====

extern f32 HUB_EPISODETITLEY;
extern i16 tSELECT, tSELECTED, tSELECTING;


// Retail clears these "fast repeat" latches but never sets them.
extern void (*drawslotinfofn)(f32, f32, i32, i32);
extern void (*drawslotsfn)(MENU *, f32);







void MenuInitialiseEx(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                      void (*draw_save_slots_info_fn)(f32, f32, i32, i32), i32 is_fade_enabled, i32 is_shadow_enabled);

void MenuInitialise(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                    void (*draw_save_slots_fn)(MENU *, f32), i32 is_fade_enabled, i32 is_shadow_enabled);

extern "C" {


























} // extern "C"

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
// Temporary C-linkage compatibility surface for unresolved engine owners.
extern "C" {
}
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.




struct ani3_animheader_s;

extern "C" {

}

extern "C" {
}

// C++-linkage helpers defined in sibling TUs.
void NuPadRecordEndFrame(void);                      // nupad_interface.cpp

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
    extern "C++" {
        // Retail only has the IPA-SRA clone (the unused scene and bits arguments dropped).
    }

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
struct nuframebuffer_s;
struct nushaderobject_s;
union variptr_u;



// ==== src/gameapi/edtools/edui.cpp: all remaining content ====

extern "C" {


}


// ==== src/legoapi/actions/character/speederchase.cpp: declarations ====

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.



DECOMP_ASSERT(sizeof(SPEEDERCHASEVALUES) == 0x1c, "Speeder chase tuning stride");
DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");














// ==== src/legoapi/characters/motion/camera.cpp: declarations ====


// Original data symbol testmovef is float0.005 at 0x617cb0.

DECOMP_ASSERT(sizeof(VIEWCAM_s) == 0x28, "ViewCam size");
DECOMP_ASSERT(offsetof(VIEWCAM_s, pitch) == 0x10, "ViewCam pitch offset");
DECOMP_ASSERT(offsetof(VIEWCAM_s, gamepad) == 0x24, "ViewCam gamepad offset");







extern "C" {


} // extern "C"

extern "C" i16 id_LANDSPEEDER;
extern i32 CUTCAMONLY, GUNSHIPAHACK, netcamera, complexsockposition_forcesock, movegamecamera_forcesock;
GameObject_s *FindNearestGameObject(NUVEC *, GameObject_s *, u32, f32, f32, i32, i32, i32, f32 *, i32,
                                    i32 (*)(GameObject_s *), bool);

DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, path) == 0x40, "Camera context path offset");
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, flags) == 0x4c, "Camera context flags offset");







// ==== src/gameapi/edtools/edfile.cpp: all remaining content ====































// ==== src/legoapi/core/config/saveload.cpp: declarations ====


extern void (*FS_Callback)(char *, char *);


// Retail keeps the whole body of RenderFileSel3 as the split-out .part clone;
// the exported function only tests FS_Active.







extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));



extern "C" {


} // extern "C"

// ==== src/legoapi/render/core/render.cpp: declarations ====









struct starfighter_s;
struct rtl_s;
struct rtlidata_s;




struct GAMEMESSAGE_s;
struct HINT_s;
struct MENU_s;
struct NuBloomParameters;
struct STATUSPACKET_s;
struct TouchHolder;
struct _vum_s;
struct _vuv_s;
struct nuhspecial_s;
struct ripple_node_s;
struct ripple_set_s;
struct VuVec;


// Camera zoom state

// Graphics loading flags


extern "C" {
}



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
extern "C" {

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








// DrawPanel HUD globals referenced by the cantina-bar-patrons WIP. Initial
// red-brick positions come from the Game init values in legogame/game.cpp.





extern i16 tPOWERBRICK, tLOCKED, tGOLDBRICK;



extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);

extern "C" {
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


extern i16 id_ANAKINJEDISCARRED, id_WOOKIEE, id_TWOFACE;




// ==== functions in original address order ====

extern "C" {




}

extern "C" {






























}

extern "C" {






















}

extern "C" {










































}

extern "C" {



































}

extern "C" {








}

extern "C" {








}

extern "C" {

}

extern "C" {


}

extern "C" {

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

}

extern "C" {





}

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
// Temporary C-linkage compatibility surface for unresolved engine owners.
extern "C" {
}
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.



extern "C" {

}

extern "C" {
}

// C++-linkage helpers defined in sibling TUs.
void NuPadRecordEndFrame(void);                      // nupad_interface.cpp

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
    extern "C++" {
        // Retail only has the IPA-SRA clone (the unused scene and bits arguments dropped).
    }

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

// ==== functions in original address order ====

extern "C" {

}

extern "C" {


}

extern "C" {







}

extern "C" {

}

extern "C" {


}

extern "C" {



}

// ==== src/legoapi/gizmo/gizmos/gizmos_gizactions.cpp: all remaining content ====





struct GIZSPINNER_s;

// ==== functions in original address order ====

static void VaderA_StartCollapseStage(WORLDINFO_s *world) {
    i32 path_index;
    nuhspecial_s specials[100];
    u32 *path_connection = static_cast<u32 *>(
        AIPAthFindPathCnx(world->ai_sys, world->ai_sys->path_sys->active_path, "Block1_a", "Block1_b", &path_index));

    if (path_connection != NULL) {
        path_connection[path_index] |= 0x80000000;
    }

    vader_a.forces[0] = GizForces_FindForce(world, "force1");
    vader_a.forces[1] = GizForces_FindForce(world, "force2");
    vader_a.forces[2] = GizForces_FindForce(world, "force3");
    vader_a.forces[3] = GizForces_FindForce(world, "force4");
    vader_a.count = 1;
    vader_a.timer = 30.0f;
    vader_a.in_control_room_message =
        SetGizAIMessage(gizaimessagesys, "InControlRoom", 1.0f, vader_a.in_control_room_message);
    vader_a.ceiling_collapse_message =
        SetGizAIMessage(gizaimessagesys, "ceiling_collapse", 1.0f, vader_a.ceiling_collapse_message);

    i32 special_count = NuSpecialFindMulti(world->current_gscn, specials, "wobble", 100, 0);
    for (i32 i = 0; i < special_count; ++i) {
        NuSpecialSetVisibility(&specials[i], 0);
    }
}

void Boulder_Kill(PART_s *part, i32) {
    i32 i;
    if (boulder_part[0] == part)
        i = 0;
    else if (__builtin_expect(boulder_part[1] == part, 0))
        i = 1;
    else
        return;
    if (part == NULL)
        return;
    if (boulder_blowup_type != -1)
        GizmoBlowUpTypeBlowUp(WORLD, boulder_blowup_type, &part->position);
    NuSpecialSetVisibility(&LevHSpecial[i], 0);
    LevInstAnim[i]->ltime = 0.0f;
    LevInstAnim[i]->playing = 0;
    boulder_part[i] = NULL;
}

void Boulder_Move(PART_s *part, float) {
    if (netclient != 0)
        return;
    i32 i;
    if (part == boulder_part[0])
        i = 0;
    else if (part == boulder_part[1])
        i = 1;
    else {
        KillPart(part, 0);
        return;
    }
    boulder_oldpos[i] = part->position;
    part->transform = *NuSpecialGetDrawMtx(&LevHSpecial[i]);
    if (!(1.0f >= part->scale_time)) {
        NuVecSub(&part->velocity, &part->position, &boulder_oldpos[i]);
        NuVecScale(&part->velocity, &part->velocity, 1.0f / FRAMETIME);
    } else {
        part->velocity = v000;
    }
    if (LevInstAnim[i]->ltime > 1.0f && !LevInstAnim[i]->playing) {
        KillPart(part, 0);
        return;
    }
    if (part->active & 1)
        PlaySfx(const_cast<char *>("Kas_BoulderLoop"), &part->position);
}

void CruiserAInit(WORLDINFO_s *world) {
    *((u8 *)LevFlag) = 0;
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "starfighter1", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[1], "starfighter2", 1);
    if (FreePlay)
        *((u8 *)LevFlag) = 2;
}

void CruiserAUpdate(WORLDINFO_s *) {
    if (*((u8 *)LevFlag) == 1) {
        void *sp;

        *((u8 *)LevFlag) = 2;
        sp = (void *)LevHSpecial;
        if (NuSpecialExistsFn(sp) != 0)
            NuSpecialSetVisibility(sp, 1);
        sp = (char *)sp + 0xc;
        if (NuSpecialExistsFn(sp) != 0)
            NuSpecialSetVisibility(sp, 1);
    }
}

void CruiserCReset(WORLDINFO_s *) {
    crusiserc_netpacket = static_cast<CRUISERCNETPACKET_s *>(SetLevelHack(4));
    cruiser_c.count_dooku = NULL;
    cruiser_c.dooku_fight = NULL;
    cruiser_c.free_palpatine = NULL;
    cruiser_c.palpatine = NULL;
    cruiser_c.palpatine_locator = NULL;
}

void CruiserCUpdate(WORLDINFO_s *) {
    if (cruiser_c.free_palpatine == NULL)
        cruiser_c.free_palpatine = CheckGizAIMessage(gizaimessagesys, "FreePalpatine", NULL);
    if (cruiser_c.dooku_fight == NULL)
        cruiser_c.dooku_fight = CheckGizAIMessage(gizaimessagesys, "DookuFight", NULL);
    if (netclient != 0) {
        cruiser_c.free_palpatine->value = crusiserc_netpacket->free_palpatine;
        cruiser_c.dooku_fight->value = crusiserc_netpacket->dooku_fight;
    }
    if (cruiser_c.palpatine == NULL)
        cruiser_c.palpatine = (GameObject_s *)FindGameObject((i32)(i16)id_PALPATINE, 0x400, 1, 1, 0);
    if (cruiser_c.palpatine_locator == NULL)
        cruiser_c.palpatine_locator = AIPathFindLocator(WORLD->ai_sys, "Palpatine");
    if (cruiser_c.count_dooku == NULL)
        cruiser_c.count_dooku = (GameObject_s *)FindGameObject((i32)(i16)id_COUNTDOOKU, 1, 1, 1, 0);
    if (FreePlay == 0 && cruiser_c.palpatine != NULL && cruiser_c.palpatine_locator != NULL &&
        cruiser_c.free_palpatine != NULL && cruiser_c.free_palpatine->value < 2.0f) {
        cruiser_c.palpatine->apiobj.position = cruiser_c.palpatine_locator->position;
        cruiser_c.palpatine->apiobj.field_0x276 = cruiser_c.palpatine_locator->direction;
    }
}

void CruiserCPanel(WORLDINFO_s *) {
    if (netclient == 0) {
        if (cruiser_c.count_dooku != NULL && cruiser_c.dooku_fight != NULL && cruiser_c.dooku_fight->value == 1.0f)
            DrawBossHitPoints(cruiser_c.count_dooku);
        else
            DrawBossHitPoints(NULL);
    }
}

void CruiserDInit(WORLDINFO_s *world) {
    if (world->area->level_count != 0) {
        auto world_area = world->area;
        for (i32 i = 0; i < world_area->level_count; i++) {
            if (static_cast<u16>(world_area->levels[i]) == static_cast<u16>(CRUISERE_LDATA->idx))
                CruiserE_ix = i;
        }
    }

    if (NuSpecialFind(WORLD->current_gscn, &CruiserD_Lift, "lift", 1)) {
        CruiserD_LiftAnim = NuSpecialGetInstAnim(&CruiserD_Lift);
        CruiserD_Lift_plat_id = FindPlatInst(NuSpecialGetInstanceix(&CruiserD_Lift));
    }

    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, gizpanel_gizmotype_id, "panel1");
    cruiserd_netpacket = static_cast<CRUISERDNETPACKET_s *>(SetLevelHack(12));

    char name[16] __attribute__((aligned(16)));
#define FIND_CRUISER_D_TUBE(NUMBER)                                                                                    \
    sprintf(name, "Tube%d", NUMBER);                                                                                   \
    if (TUBE *tube = Tube_FindByName(world, name))                                                                     \
    tube->flags |= TUBE_FLAG_TOUCH_RADIUS
    FIND_CRUISER_D_TUBE(1);
    FIND_CRUISER_D_TUBE(2);
    FIND_CRUISER_D_TUBE(3);
    FIND_CRUISER_D_TUBE(4);
    FIND_CRUISER_D_TUBE(5);
    FIND_CRUISER_D_TUBE(6);
    FIND_CRUISER_D_TUBE(7);
    FIND_CRUISER_D_TUBE(8);
    FIND_CRUISER_D_TUBE(9);
    FIND_CRUISER_D_TUBE(10);
#undef FIND_CRUISER_D_TUBE
}

void CruiserDReset(WORLDINFO_s *) {
    CruiserD_LiftChase_msg = CheckGizAIMessage(gizaimessagesys, "LiftChase", NULL);
    MiscTime = 0.0f;

    if (NuSpecialExistsFn(&CruiserD_Lift) != 0 && CruiserD_LiftAnim != NULL) {
        if ((*(u8 *)((u8 *)LevelProgressData + CruiserE_ix * 0x2e24 + 0x2800) & 1) != 0) {
            if (CruiserD_direction < 0) {
                CruiserD_LiftAnim->ltime = CruiserD_frame;
                CruiserD_LiftAnim->playing = 1;
                CruiserD_LiftAnim->tfactor = -0.1f;
            } else {
                f32 end_frame = *(f32 *)CruiserD_Lift.scene->instance_animation_data[CruiserD_LiftAnim->anim_ix];
                CruiserD_LiftAnim->playing = 0;
                CruiserD_LiftAnim->tfactor = 0.1f;
                CruiserD_LiftAnim->ltime = end_frame;
            }
        } else {
            CruiserD_frame = 1.0f;
            CruiserD_direction = 1;
            CruiserD_LiftAnim->playing = 1;
            CruiserD_LiftAnim->ltime = 1.0f;
            CruiserD_LiftAnim->tfactor = 0.1f;
        }
    }

    CruiserD_LiftChase = 0;
}

void CruiserDUpdate(WORLDINFO_s *) {
    CruiserD_LiftChase = 0;
    if (!NuSpecialExistsFn(&CruiserD_Lift) || CruiserD_LiftAnim == NULL || CruiserD_LiftChase_msg == NULL)
        return;

    if (netclient == 0) {
        CruiserD_LiftChase_msg->value = 0.0f;
        if (CruiserD_direction >= 0 && LevGizmo[0] != NULL && LevGizmo[0]->object != NULL &&
            (((u8 *)LevGizmo[0]->object)[0x68] & 2) != 0) {
            CruiserD_direction = -1;
            CruiserD_LiftAnim->tfactor = -0.1f;
            CruiserD_LiftAnim->playing = 1;
        }

        if (!CruiserD_LiftAnim->playing) {
            CruiserD_frame = CruiserD_LiftAnim->ltime;
            return;
        }

        if (CruiserD_Lift_plat_id != -1) {
            if (CruiserD_direction >= 0) {
                CruiserD_LiftChase_msg->value = 1.0f;
                CruiserD_LiftChase = 1;
            }
            NUVEC *lift_pos = NuSpecialGetDrawPos(&CruiserD_Lift);
#define CRUISERD_CHECK_PLAYER(index)                                                                                   \
    {                                                                                                                  \
        GameObject_s *victim = Player[index];                                                                          \
        if (victim != NULL && victim->apiobj.field_0x287 == 0 &&                                                       \
            (victim->apiobj.supporting_platform_id == CruiserD_Lift_plat_id || victim->apiobj.pos_z > lift_pos->z)) {  \
            ObjHitObj(NULL, victim, -1, 0, 0, 1);                                                                      \
            KillGameObject(victim, 2, 0);                                                                              \
            if (CruiserD_direction >= 0 && MiscTime == 0.0f)                                                           \
                MiscTime = 1.0f;                                                                                       \
        }                                                                                                              \
    }
            CRUISERD_CHECK_PLAYER(0);
            CRUISERD_CHECK_PLAYER(1);
            CRUISERD_CHECK_PLAYER(2);
            CRUISERD_CHECK_PLAYER(3);
            CRUISERD_CHECK_PLAYER(4);
            CRUISERD_CHECK_PLAYER(5);
            CRUISERD_CHECK_PLAYER(6);
            CRUISERD_CHECK_PLAYER(7);
#undef CRUISERD_CHECK_PLAYER
            if (MiscTime > 0.0f) {
                MiscTime -= FRAMETIME;
                if (MiscTime <= 0.0f) {
                    MiscTime = 0.0f;
                    if (ChallengeMode != 3)
                        ResetLevel(NULL, NULL, 1);
                }
            }
        }
    }

    if (CruiserD_LiftAnim->playing)
        PlaySfx("Cru_HugeWallMoveLp", NuSpecialGetDrawPos(&CruiserD_Lift));

    CruiserD_frame = CruiserD_LiftAnim->ltime;
    if (netclient != 0) {
        CruiserD_LiftAnim->ltime = cruiserd_netpacket->frame;
        CruiserD_LiftAnim->tfactor = cruiserd_netpacket->speed;
        CruiserD_LiftAnim->playing = (cruiserd_netpacket->flags & 1) != 0;
    }
}

void GrievousA_Init(WORLDINFO_s *world) {
    GIZMOBLOWUP_s *b;
    if ((b = GizmoBlowUp_FindByName(world, "blast_2_blowup1")) != NULL) {
        nuvec_s pos = {5.42f, 2.76f, 1.79f};
        NuSpecialSetDrawPos(&b->type->animated_special, &pos);
        UpdateMidPos(b);
    }
    if ((b = GizmoBlowUp_FindByName(world, "blast_1_blowup1")) != NULL)
        b->field_0x124 = 1;
    if ((b = GizmoBlowUp_FindByName(world, "blast_1_blowup2")) != NULL)
        b->field_0x124 = 1;
}

void GrievousA_Reset(WORLDINFO_s *) {
    if (netclient != 0)
        return;
    Grievous_obj = (GameObject_s *)FindGameObject((i32)(i16)id_GRIEVOUS, 1, 1, 0, 0);
    if (Grievous_obj != NULL)
        DrawBossHitPoints(Grievous_obj);
}

void GrievousA_Update(WORLDINFO_s *world) {
    if (netclient != 0)
        return;

    if (Grievous_obj == NULL)
        return;

    if (Grievous_obj->current_hp > 0)
        return;

    if (FreePlay == 0)
        KillBossPlayCutScene((i32)(i16)id_GRIEVOUS, 0, 0.0f, "ep3_GeneralGrievous_Outro");
    else
        KillBossCompleteLevel((i32)(i16)id_GRIEVOUS, 0, 0.0f);
}

void KashyyykA_Init(WORLDINFO_s *world) {
    GIZMOBLOWUP_s *b;
    if ((b = GizmoBlowUp_FindByName(world, "bridge_1_switc1")) != NULL) {
        b->field_0x128 = 0.3f;
        b->field_0x124 = 1;
        b->field_0xa0 &= ~2;
    }
    if ((b = GizmoBlowUp_FindByName(world, "bridge_1_switc2")) != NULL) {
        b->field_0x128 = 0.3f;
        b->field_0x124 = 1;
        b->field_0xa0 &= ~2;
    }
    if ((b = GizmoBlowUp_FindByName(world, "bridge_2_switc1")) != NULL) {
        b->field_0x128 = 0.3f;
        b->field_0x124 = 1;
        b->field_0xa0 &= ~2;
    }
    if ((b = GizmoBlowUp_FindByName(world, "bridge_2_switc2")) != NULL) {
        b->field_0x128 = 0.3f;
        b->field_0x124 = 1;
        b->field_0xa0 &= ~2;
    }
}

void KashyyykA_Reset(WORLDINFO_s *) {
    KashyyykA_msg_TotalWookies = CheckGizAIMessage(gizaimessagesys, "TotalWookies", NULL);
    KashyyykA_msg_WookiesToRescue = CheckGizAIMessage(gizaimessagesys, "WookiesToRescue", NULL);
}

void KashyyykA_Update(WORLDINFO_s *) {
}

void KashyyykA_Panel(WORLDINFO_s *) {
    if (Mission_Active(MissionSys) != NULL)
        return;
    i16 characters[3] = {id_WOOKIEE, id_WOOKIEE, id_WOOKIEE};
    char rescued[3] = {1, 1, 1};
    if (KashyyykA_msg_TotalWookies != NULL && KashyyykA_msg_TotalWookies->value > 0.0f &&
        KashyyykA_msg_WookiesToRescue != NULL && KashyyykA_msg_WookiesToRescue->value > 0.0f) {
        i32 remaining = (i32)KashyyykA_msg_WookiesToRescue->value;
        if (remaining > 3)
            remaining = 3;
        for (i32 i = 0; i < remaining; ++i)
            rescued[i] = 0;
        i32 total = (i32)KashyyykA_msg_TotalWookies->value;
        if (total > 3)
            total = 3;
        DrawMeleeTargets(characters, rescued, NULL, total);
    }
}

void KashyyykB_Init(WORLDINFO_s *) {
}

void KashyyykB_Reset(WORLDINFO_s *) {
}

void KashyyykB_Update(WORLDINFO_s *) {
}

void KashyyykC_Init(WORLDINFO_s *world) {
    GIZFORCE_s *f = GizForces_FindForce(world, "force16");
    if (f != NULL) {
        if (f->force_strength == 3.0f)
            f->force_strength = 20.0f;
        f->strength_0x6c = 1.0f;
    }
}

void KashyyykC_Update(WORLDINFO_s *) {
}

void KashyyykD_Init(WORLDINFO_s *) {
    i_boulder = 0;
    BoulderWait = 0.0f;
    boulder_blowup_type = GizmoBlowupGetTypeFromNameTableId(WORLD, GizmoBlowupGetNameTableId("ball_blowup_null"));
}

void KashyyykD_Reset(WORLDINFO_s *world) {
    char name[0x100];
    for (i32 i = 0; i < 2; ++i) {
        sprintf(name, "ball%ib", i + 1);
        if (NuSpecialFind(world->current_gscn, &LevHSpecial[i], name, 1))
            NuSpecialSetVisibility(&LevHSpecial[i], 0);
        boulder_part[i] = NULL;
        sprintf(name, "ball%i", i + 1);
        if (NuSpecialFind(world->current_gscn, &LevHSpecial[i], name, 1)) {
            NuSpecialSetVisibility(&LevHSpecial[i], 0);
            LevInstAnim[i] = NuSpecialGetInstAnim(&LevHSpecial[i]);
            LevInstAnim[i]->playing = 0;
            LevInstAnim[i]->ltime = 0.0f;
        }
    }
}

void KashyyykD_Update(WORLDINFO_s *) {
    if (netclient != 0)
        return;
    if (BoulderWait <= 0.0f) {
        i32 i = i_boulder;
        if (boulder_part[i] != NULL)
            return;
        nuhspecial_s *special;
        if (LevInstAnim[i] != NULL && (special = &LevHSpecial[i])->display_special != NULL) {
            ADDPART_s params = Default_ADDPART;
            params.matrix = &numtx_identity;
            params.velocity = &v000;
            params.owner = NULL;
            params.field_14 = 0.3f;
            params.field_18 = 0.3f;
            params.gravity = 0.0f;
            params.special = special;
            params.flags = 0x80861a;
            params.move_fn = Boulder_Move;
            params.field_40 = PartCollide_3D;
            params.field_44 = Boulder_Kill;
            params.time_step = FRAMETIME;
            PART_s *part = AddPart(&params);
            if (part != NULL)
                boulder_part[i] = part;
            LevInstAnim[i]->ltime = 1.0f;
            LevInstAnim[i]->playing = 1;
            LevInstAnim[i]->mtx = numtx_identity;
            NuSpecialSetVisibility(special, 1);
        }
        ++i_boulder;
        if (i_boulder == 2)
            i_boulder = 0;
        BoulderWait = 8.0f;
    } else {
        BoulderWait -= FRAMETIME;
    }
}

i32 Action_BoulderSection(AISYS_s *system, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                          i32 param_count, i32 first_time, f32) {
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL)
        return 1;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (first_time != 0) {
        processor->action_data_4 = 1.0f;
        processor->action_data_5 = 1.0f;
        for (i32 index = 0; index < param_count; ++index) {
            char *value = NuStrIStr(params[index], "boulder_range=");
            if (value != NULL) {
                processor->action_data_4 = AIParamToFloat(&packet->script_process, value + 14);
            } else if ((value = NuStrIStr(params[index], "attack_time=")) != NULL) {
                processor->action_data_5 = AIParamToFloat(&packet->script_process, value + 12);
            } else {
                packet->movement_instruction_parameter = AIParamToFloat(processor, params[index]);
            }
        }
    }
    const bool can_attack = FreePlay == 0 || CharCategory_IsCategory(object, 0) != 0 ||
                            CharCategory_IsCategory(object, 1) != 0 || CharCategory_IsCategory(object, 8) != 0;
    if (!can_attack) {
        processor->action_timer -= FRAMETIME;
        if (processor->action_timer < 0.0f) {
            processor->action_timer = 0.5f;
            object->pad_gamepad->buttons_pressed |= GAMEPAD_TOGGLELEFT;
        }
    }
    object->field_0xef8 |= GAMEOBJECT_EF8_FLAG_KEEP_WEAPON_OUT;
    PART_s *nearest = NULL;
    f32 nearest_distance = processor->action_data_4 * processor->action_data_4;
    NUVEC difference;
    if (boulder_part[0] != NULL) {
        const f32 distance = NuVecDistSqr(&object->apiobj.collision_position, &boulder_part[0]->position, &difference);
        if (distance < nearest_distance) {
            nearest = boulder_part[0];
            nearest_distance = distance;
        }
    }
    if (boulder_part[1] != NULL) {
        const f32 distance = NuVecDistSqr(&object->apiobj.collision_position, &boulder_part[1]->position, &difference);
        if (distance < nearest_distance)
            nearest = boulder_part[1];
    }
    // The retail third probe reads padding past this two-entry array. Do not
    // reproduce that out-of-bounds access or enlarge the recovered global.
    if (nearest != NULL) {
        packet->movement_look_target = &nearest->position;
        if (can_attack)
            object->pad_gamepad->buttons_pressed |= GAMEPAD_ACTION;
    } else {
        if (system != NULL && system->player_1 != NULL && system->player_1->ai != NULL) {
            AIPACKET_s *player_packet = system->player_1->ai;
            AIMoveInstruction(packet, &player_packet->last_path_position, player_packet->mover_height,
                              &player_packet->path_info, 1, packet->movement_instruction_parameter);
        }
    }
    return 0;
}

void TempleA_Init(WORLDINFO_s *world) {
    GIZMOBLOWUP_s *b;
    if ((b = GizmoBlowUp_FindByName(world, "builditt_blow1")) != NULL)
        b->field_0xa0 |= 2;
    if ((b = GizmoBlowUp_FindByName(world, "deton_build1")) != NULL)
        b->field_0xa0 |= 2;
}

void TempleC_Init(WORLDINFO_s *world) {
    char *force_names[10] = {"force8", "force10", "force5",  "force4",  "force3",
                             "force2", "force1",  "force23", "force21", "force19"};
    for (i32 i = 0; i < 10; ++i) {
        GIZFORCE_s *force = GizForces_FindForce(world, force_names[i]);
        if (force != NULL && (force->config_flags & 0x400) != 0) {
            for (GAMEANIMOBJ_s *object = force->anim_set->objects; object != NULL; object = object->next) {
                if ((object->flags & 1) == 0)
                    object->current_frame = 0.5f;
            }
        }
    }
    GIZMOBLOWUP_s *blowup;
    if ((blowup = GizmoBlowUp_FindByName(world, "thermo_box11")) != NULL)
        blowup->field_0xa0 |= 2;
    if ((blowup = GizmoBlowUp_FindByName(world, "thermo_box21")) != NULL)
        blowup->field_0xa0 |= 2;
    if ((blowup = GizmoBlowUp_FindByName(world, "thermo_box31")) != NULL)
        blowup->field_0xa0 |= 2;
    if ((blowup = GizmoBlowUp_FindByName(world, "Thermo1")) != NULL)
        blowup->field_0xa0 |= 2;
}

void TempleC_AlwaysUpdate(WORLDINFO_s *) {
    NUVEC position;
    if (TEMPLESTATUS_LDATA != NULL && Players_AveragePos(&position, NULL) &&
        NuVecXZDistSqr(&position, &TempleC_StreamStatusPos, NULL) < 100.0f)
        other_level_override = TEMPLESTATUS_LDATA->idx;
}

void VaderA_Init(WORLDINFO_s *world) {
    memset(&vader_a, 0, sizeof(vader_a));
    vadera_netpacket = SetLevelHack(8);
    vader_a.big_jump_locator = AIPathFindLocator(world->ai_sys, "Bigjump_0");
    vader_a.in_control_room_message = SetGizAIMessage(gizaimessagesys, "InControlRoom", 0.0f, NULL);
    vader_a.ceiling_collapse_message = SetGizAIMessage(gizaimessagesys, "ceiling_collapse", 0.0f, NULL);
    vader_a.timer_message = SetGizAIMessage(gizaimessagesys, "timer", 0.0f, NULL);
    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "LozTheTosser1");
    if (blowup != NULL)
        blowup->field_0x124 = 1;
}

void VaderA_Reset(WORLDINFO_s *world) {
    DrawTimer(0, 0, 1);

    if (netclient == 0 && vader_a.count != 0) {
        VaderA_StartCollapseStage(world);
    }

    if (vader_a.timer_message != NULL) {
        vader_a.timer_message->value = 0.0f;
    }

    vader_a.collapse_started = 1;
    vader_a.reset_flag = 0;
}

void VaderA_Update(WORLDINFO_s *) {
    if (vader_a.timer_message != NULL)
        vader_a.timer_message->value += FRAMETIME;

    if (netclient == 0) {
        if (vader_a.collapse_started != 0) {
            vader_a.collapse_started = 0;
            if (Player[0] != NULL && (Player[0]->apiobj.field_0x1f8 & 1) != 0)
                SetObjOnSurface(Player[0], 1);
            if (Player[1] != NULL && (Player[1]->apiobj.field_0x1f8 & 1) != 0)
                SetObjOnSurface(Player[1], 1);
            if (Player[2] != NULL && (Player[2]->apiobj.field_0x1f8 & 1) != 0)
                SetObjOnSurface(Player[2], 1);
            if (Player[3] != NULL && (Player[3]->apiobj.field_0x1f8 & 1) != 0)
                SetObjOnSurface(Player[3], 1);
            if (Player[4] != NULL && (Player[4]->apiobj.field_0x1f8 & 1) != 0)
                SetObjOnSurface(Player[4], 1);
            if (Player[5] != NULL && (Player[5]->apiobj.field_0x1f8 & 1) != 0)
                SetObjOnSurface(Player[5], 1);
            if (Player[6] != NULL && (Player[6]->apiobj.field_0x1f8 & 1) != 0)
                SetObjOnSurface(Player[6], 1);
            if (Player[7] != NULL && (Player[7]->apiobj.field_0x1f8 & 1) != 0)
                SetObjOnSurface(Player[7], 1);
        }

        if (vader_a.big_jump_locator != NULL && player != NULL && player->apiobj.supporting_platform_id != -1)
            vader_a.big_jump_locator->position = player->apiobj.lower_position;
    }

    if (vader_a.count != 0) {
        f32 previous_time = vader_a.timer;
        vader_a.timer -= FRAMETIME;

        if (static_cast<i16>(vader_a.count) <= 2) {
            if (vader_a.forces[0] != NULL && GizForce_Complete(vader_a.forces[0])) {
                vader_a.timer += 20.0f;
                if (TouchHacks::TouchControlsActive)
                    vader_a.timer += 10.0f;
                vader_a.subtitle = 1;
                vader_a.forces[0] = NULL;
            }
            if (vader_a.forces[1] != NULL && GizForce_Complete(vader_a.forces[1])) {
                vader_a.timer += 20.0f;
                if (TouchHacks::TouchControlsActive)
                    vader_a.timer += 10.0f;
                vader_a.subtitle = 1;
                vader_a.forces[1] = NULL;
            }
            if (vader_a.forces[2] != NULL && GizForce_Complete(vader_a.forces[2])) {
                vader_a.timer += 20.0f;
                if (TouchHacks::TouchControlsActive)
                    vader_a.timer += 10.0f;
                vader_a.subtitle = 1;
                vader_a.forces[2] = NULL;
            }
            if (vader_a.forces[3] != NULL && GizForce_Complete(vader_a.forces[3])) {
                vader_a.timer += 20.0f;
                if (TouchHacks::TouchControlsActive)
                    vader_a.timer += 10.0f;
                vader_a.subtitle = 1;
                vader_a.forces[3] = NULL;
            }

            static const f32 time_limits[3] = {30.0f, 15.0f, 0.0f};
            if (time_limits[static_cast<i16>(vader_a.count)] > vader_a.timer) {
                if (vader_a.count == 1) {
                    SetGizAIMessage(gizaimessagesys, "ceiling_collapse", 2.0f, vader_a.ceiling_collapse_message);
                } else if (vader_a.count == 2) {
                    SetGizAIMessage(gizaimessagesys, "ceiling_collapse", 3.0f, vader_a.ceiling_collapse_message);
                    vader_a.timer = 2.0f;
                    NuCameraGetMtx();
                    GameCameraMakeMiniCut2(&vadar_cam_pos, &vadar_cam_tgt, 0, 0.0f, 2.0f, 0.0f, 0.5f, 0, 0, 1);
                }
                ++vader_a.count;
            }

            if (vader_a.subtitle != 0 ||
                (vader_a.timer > 0.0f && static_cast<i32>(previous_time) != static_cast<i32>(vader_a.timer)))
                TickTockSfx();
        } else if (netclient == 0 && vader_a.timer <= 0.0f) {
            if (Player[0] != NULL && Player[0]->apiobj.player_controlled)
                LoseCoins(Player[0], 1);
            if (Player[1] != NULL && Player[1]->apiobj.player_controlled)
                LoseCoins(Player[1], 1);
            KillGameObject(player, 2, 0);
        }
    }

    if (vader_a.reset_flag == 0 &&
        ((Player[0] != NULL && Player[0]->apiobj.field_0x287 != 0 && (Player[0]->apiobj.field_0x1f4 & 0x40000) == 0) ||
         (Player[1] != NULL && Player[1]->apiobj.field_0x287 != 0 && (Player[1]->apiobj.field_0x1f4 & 0x40000) == 0))) {
        if (GameCam->sock_position.location.sock != 0 || player->apiobj.field_0x287 != 0) {
            vader_a.reset_flag = 1;
            ResetLevel(NULL, NULL, 1);
        }
    }

    VADERANETPACKET_s *packet = static_cast<VADERANETPACKET_s *>(vadera_netpacket);
    if (netclient == 0) {
        packet->count = vader_a.count;
        packet->timer = vader_a.timer;
        packet->subtitle = vader_a.subtitle;
    } else {
        vader_a.count = packet->count;
        vader_a.timer = packet->timer;
        vader_a.subtitle = packet->subtitle;
    }
}

void VaderA_DrawPanel(WORLDINFO_s *) {
    if (__builtin_expect(vader_a.count <= 2, 0)) {
        if (vader_a.count != 0) {
            DrawTimer((i32)vader_a.timer + 1, vader_a.subtitle, 0);
            vader_a.subtitle = 0;
        }
    }
}

void VaderA_GoneThroughDoor(WORLDINFO_s *world, DOOR_s *door) {
    if (netclient == 0 && vader_a.count == 0)
        VaderA_StartCollapseStage(world);
    if (door != NULL && NuStrICmp(door->name, "door_control") == 0)
        door->active = 1;
}

void VaderB_Init(WORLDINFO_s *) {
}

void VaderB_Reset(WORLDINFO_s *) {
    vader_b_complete_msg = SetGizAIMessage(gizaimessagesys, "VaderBComplete", 0.0f, NULL);
    vader_b_playersDead = 0;
}

void VaderB_Update(WORLDINFO_s *) {
    if (netclient == 0 && vader_b_complete_msg != NULL && vader_b_complete_msg->value == 1.0f && VADERB_LDATA != NULL)
        GoToNewLevel(VADERC_LDATA->idx);
    if (vader_b_playersDead == 0 &&
        ((Player[0] != NULL && Player[0]->apiobj.field_0x287 != 0 && (Player[0]->apiobj.field_0x1f4 & 0x40000) == 0) ||
         (Player[1] != NULL && Player[1]->apiobj.field_0x287 != 0 && (Player[1]->apiobj.field_0x1f4 & 0x40000) == 0))) {
        vader_b_playersDead = 1;
        ResetLevel(NULL, NULL, 1);
    }
}

void VaderB_DrawPanel(WORLDINFO_s *) {
}

void VaderC_Init(WORLDINFO_s *world) {
    char *names[10] = {"rock1", "rock2", "rock3", "rock4", "rock5", "rock6", "rock7", "rock8", "rock10", "rock11"};
    memset(&vader_c, 0, sizeof(vader_c));
    vader_c.final_fight_message = CheckGizAIMessage(gizaimessagesys, "FinalFight", NULL);
    vader_c.big_jump_locator = AIPathFindLocator(world->ai_sys, "Bigjump_0");
    for (i32 i = 0; i < 10; ++i) {
        if (NuSpecialFind(world->current_gscn, &vader_c.rocks[i], names[i], 1))
            vader_c.platform_ids[i] = FindPlatInst(NuSpecialGetInstanceix(&vader_c.rocks[i]));
        else
            vader_c.platform_ids[i] = -1;
    }
    Vader_ObiWanKilledAnakin = 0;
    vaderc_netpacket = SetLevelHack(1);
    LevGizObst[0] = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle19");
}

void VaderC_Reset(WORLDINFO_s *) {
    vader_c.field_0x94 = 0;
    vader_c.field_0x95 = 0;
}

void VaderC_Update(WORLDINFO_s *world) {
    extern TERRSET *CurTerr;
    extern i32 obstacle_gizmotype_id;

    GIZAIMESSAGE_s *final_fight = vader_c.final_fight_message;
    if (netclient == 0 && final_fight != NULL && ChallengeMode == 0 && final_fight->value == 0.0f &&
        vader_c.big_jump_locator != NULL && player != NULL && player->apiobj.supporting_platform_id != -1) {
        vader_c.big_jump_locator->position = player->apiobj.lower_position;

        u8 progress = vader_c.field_0x94;
        for (i32 i = 0; i < 10; ++i) {
            i16 platform_id = vader_c.platform_ids[i];
            if (platform_id == -1)
                continue;

            if (Player[0] != NULL && Player[0]->apiobj.field_0x27d != 0 &&
                Player[0]->apiobj.supporting_platform_id == platform_id &&
                Player[0]->apiobj.position.y >=
                    static_cast<NUMTX *>(CurTerr->platforms[platform_id].scene_object)->m31) {
                progress |= 1;
                vader_c.field_0x94 = progress;
            }
            if (Player[1] != NULL && Player[1]->apiobj.field_0x27d != 0 &&
                Player[1]->apiobj.supporting_platform_id == platform_id &&
                Player[1]->apiobj.position.y >=
                    static_cast<NUMTX *>(CurTerr->platforms[platform_id].scene_object)->m31) {
                progress |= 2;
                vader_c.field_0x94 = progress;
            }
        }

        if (progress == 3) {
            final_fight->value = 1.0f;
            DOOR_s *door = Door_FindByName(world, "door_fight");
            if (door != NULL)
                Door_GoThrough(world, door, 1);
        }
    }

    if (__builtin_expect(netclient != 0 && GameTimer.time_elapsed < 5.0f, 0)) {
        GIZMO *gizmo = GizmoFindByName(WORLD->gizmo_sys, obstacle_gizmotype_id, "obstacle20");
        if (gizmo != NULL && gizmo->object != NULL)
            static_cast<GIZOBSTACLE_s *>(gizmo->object)->progress_flags &= ~1;
        gizmo = GizmoFindByName(WORLD->gizmo_sys, obstacle_gizmotype_id, "obstacle21");
        if (gizmo != NULL && gizmo->object != NULL)
            static_cast<GIZOBSTACLE_s *>(gizmo->object)->progress_flags &= ~1;
        gizmo = GizmoFindByName(WORLD->gizmo_sys, obstacle_gizmotype_id, "obstacle22");
        if (gizmo != NULL && gizmo->object != NULL)
            static_cast<GIZOBSTACLE_s *>(gizmo->object)->progress_flags &= ~1;
        gizmo = GizmoFindByName(WORLD->gizmo_sys, obstacle_gizmotype_id, "obstacle23");
        if (gizmo != NULL && gizmo->object != NULL)
            static_cast<GIZOBSTACLE_s *>(gizmo->object)->progress_flags &= ~1;
    }

    if (vader_c.field_0x95 == 0) {
        GameObject_s *second_player = Player[1];
        GameObject_s *first_player = Player[0];
        if (((first_player != NULL && (first_player->apiobj.field_0x1f8 & 0x80) != 0 &&
              first_player->apiobj.field_0x287 != 0 && (first_player->apiobj.field_0x1f4 & 0x40000) == 0) ||
             (second_player != NULL && (second_player->apiobj.field_0x1f8 & 0x80) != 0 &&
              second_player->apiobj.field_0x287 != 0 && (second_player->apiobj.field_0x1f4 & 0x40000) == 0)) &&
            (static_cast<u8 *>(vaderc_netpacket)[0] == 0 || first_player == NULL ||
             (first_player->apiobj.field_0x1f8 & 0x80) == 0 || second_player == NULL ||
             (second_player->apiobj.field_0x1f8 & 0x80) == 0) &&
            (ChallengeMode == 0 || AreaGlobals.values.field_0x1c <= 9)) {
            vader_c.field_0x95 = 1;
            if (__builtin_expect(vader_c.final_fight_message->value == 0.0f, 1))
                ResetLevel(NULL, NULL, 1);
        }
    }

    if (LevGizObst[0] != NULL && LevGizObst[0]->anim_set != NULL && LevGizObst[0]->anim_set->objects != NULL &&
        LevGizObst[0]->anim_set->objects->instance_animation != NULL) {
        GAMEANIMOBJ_s *object = LevGizObst[0]->anim_set->objects;
        nuinstanim_s *anim = object->instance_animation;
        if (object->end_frame > 200.0f) {
            f32 factor = 1.0f;
            if (anim->ltime >= 200.0f) {
                factor = (object->end_frame - anim->ltime) / (object->end_frame - 200.0f);
                factor *= factor;
            }
            if (anim->fparam1 == 0.0f)
                anim->tfactor = factor;
            else
                anim->tfactor = factor * anim->fparam1;
        }
    }
}

void VaderC_DrawPanel(WORLDINFO_s *) {
    if (netclient == 0 && vader_c.final_fight_message != NULL && vader_c.final_fight_message->value == 1.0f) {
        if (player2 != NULL) {
            DrawBossHitPoints(NULL);
        } else {
            GameObject_s *opponent = Player[0];
            if (opponent == player)
                opponent = Player[1];
            if (opponent != NULL)
                DrawBossHitPoints(opponent);
        }
    }
}

