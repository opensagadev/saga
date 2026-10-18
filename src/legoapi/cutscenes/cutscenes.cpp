// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/audio/sfx.h"
#include "globals.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/core/screen.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/nusound/nusound.h"

#include <string.h>
#include <stdio.h>
#include "legoapi/world/mission.h"
#include "legoapi/world/levels/episode.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/audio/audio.h"
#include "legoapi/world/world_shared.h"
#include <math.h>
#include "nu2api/nucore/nuthread.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/numath/nuvec4.h"
#include <cfloat>
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include <stdarg.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/numath/numtx.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/items/objects/gameobjects.h"
#include "editor/aieditor_state.h"
#include "gameapi/ai/aisys/aisys.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nuplane.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/core/input/timer.h"
#include "nu2api/nucore/nutime.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include <stdlib.h>
#include "nu2api/nu3d/nuportal.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "gameapi/edtools/edui.h"
#include "gameframework/saveload.h"
#include "batman.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/menus/core/gamehint.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/render/light/fade_material.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nu3d/nupostparams.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/menus/screens/movies.h"
#include "nu2api/nucore/nuvideo.h"
#include "legoapi/misc/androidbatman.h"
#include "legoapi/render/core/terrain_internal.h"
#include "legoapi/render/core/gameliball.h"
#include "legoapi/render/fx/game_deb.h"
#include <stddef.h>
#include "gameapi/edtools/edgra.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include <cstdio>
#include "legoapi/misc/utilities.h"
#include "nu2api/nu3d/nuspline.h"
#include <new>
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nulist.h"
#include "legoapi/items/base/animpacket.h"
#include "legoapi/characters/core/characters.h"
#include "legoapi/cutscenes/gcutscn.h"
#include "legoapi/world/levels.h"
#include "nu2api/nu3d/nu3d_includes.h"

extern i32 CUTCAMONLY;
CUTSCENESYS *CutSceneSys;

static u8 CutBlobShadowAlpha;
static f32 CutBlobShadowFadeNear;
static f32 CutBlobShadowFadeFar;
static f32 CutReflectRange;
static f32 CutReflectRange2;

i32 CutInstEndCount;
static instNUGCUTSCENE_s *CutInstEnd[4];
static i32 CutInstEndStop;
static i32 cutaudiopaused;
static CUTINFO *g_lastCutInfo;
static f32 g_lastCutsceneTime;
static f32 g_accumCutsceneTime;
static i32 CutFrame;
static i32 stream_cut_issued;
i32 NewCutInfoCount;
static CUTINFO *NewCutInfo[8];
u8 cutskip_dontplaylevelintro;
i32 CUTNOFOG;
f32 LevelNameMul;
f32 LevelNameTime;

#if defined(__i386__)
#define CUTSCENE_START_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define CUTSCENE_START_ATTR __attribute__((noinline, noclone))
#endif
static CUTSCENE_START_ATTR i32 CutScene_StartBody(WORLDINFO_s *world, CUTINFO *cut)
    __asm__("_ZL14CutScene_StartP11WORLDINFO_sP7CUTINFOi.isra.24.part.25");

static inline i32 CutScene_Start(WORLDINFO_s *world, CUTINFO *cut, i32) {
    return CutScene_StartBody(world, cut);
}

static __used__ void LocatorFunction_Blaster(instNUGCUTSCENE_s *, NUGCUTLOCATORSYS_s *, instNUGCUTLOCATOR_s *,
                                             NUGCUTLOCATOR_s *locator, float frame, numtx_s *parent_mtx, int);

static NUGCUTLOCATORFNENTRY_s cutscene_locatorfns[] = {
    {"blaster", 0, 0, 0, LocatorFunction_Blaster},
    {NULL, 0, 0, 0, NULL},
};

// The loop re-reads the active cut through CutStopInfo after each SmartTextEx call.
#define CUT_STOP static_cast<CUTINFO *>(CutStopInfo)

i32 STATUS_R = 255;
i32 STATUS_G = 191;
i32 STATUS_B;

struct CUTSCENEPLAYERCLIP;
struct instNUGCUTCHAR_s;
struct NUGCUTCHAR_s;
struct NUGCUTRIGID_s;
struct instNUGCUTRIGID_s;

static i32 CS_area = 0;
CUTSYS *CS_cutsys = NULL;
static WORLDINFO *CS_worldinfo = NULL;
static CUTINFO *CS_CutInfo;
static VARIPTR *CS_buffptr;
static VARIPTR *CS_buffend;
static i32 CS_texanimcount;
static i32 CS_fadefogcount;
static void CS_play_sfx(NUFPAR *fp);
static void CS_sfx(NUFPAR *fp);

i32 CUTCOUNT = 0;
CutSceneListEntry *CutList = NULL;
void (*CutScene_OverrideConfigFileNameFn)(char *, i32, i32) = NULL;
i32 ACTIVECUTCOUNT = 0;
f32 CutSceneScale = 1.0f;
extern "C" {
    u8 CUTSUBTITLEDEFAULT_R = 0xff;
    u8 CUTSUBTITLEDEFAULT_G = 0xff;
    u8 CUTSUBTITLEDEFAULT_B = 0xff;
    u8 CUTSUBTITLEDEFAULT_A = 0x80;
    u8 CUTSUBTITLEDEFAULT_ALIGN = 0;
    f32 CUTSUBTITLEDEFAULT_X = 0.0f;
    f32 CUTSUBTITLEDEFAULT_Y = -0.5f;
    f32 CUTSUBTITLEDEFAULT_SCALEX = 1.0f;
    f32 CUTSUBTITLEDEFAULT_SCALEY = 1.0f;
    f32 CUTSUBTITLEDEFAULT_FITWIDTH = 1.7f;
}
i32 CUTCAM = 0;
i32 CUTCAMONLY = 0;

static void CS_no_fog(NUFPAR *);
static void CS_lowend_lowbits(NUFPAR *);
static void CS_is_outro(NUFPAR *);
static void CS_unskippable_in_story(NUFPAR *);
static void CS_skip_use_goto(NUFPAR *);
static void CS_in_game(NUFPAR *);
static void CS_snap_out(NUFPAR *);
static void CS_cam_only(NUFPAR *);
static void CS_wipe_out(NUFPAR *);
static void CS_new_mode(NUFPAR *);
static void CS_replace_players(NUFPAR *);
static void CS_looping(NUFPAR *);
static void CS_start_cam(NUFPAR *);
static void CS_super_widescreen(NUFPAR *);
static void CS_level_intro(NUFPAR *);
static void CS_hold_audio(NUFPAR *);
static void CS_playonce(NUFPAR *);
static void CS_nextcutscene_inplayablelevel(NUFPAR *);
static void CS_farclip(NUFPAR *fp);
static void CS_render_group(NUFPAR *fp);
static void CS_reflect_range(NUFPAR *fp);
static void CS_blobshadow_fadefar(NUFPAR *fp);
static void CS_blobshadow_fadenear(NUFPAR *fp);
static void CS_blobshadow_alpha(NUFPAR *fp);
static void CS_nearclip(NUFPAR *fp);
static void CS_burnout_flare(NUFPAR *fp);
static void CS_burnout_intensity(NUFPAR *fp);
static void CS_burnout_threshold(NUFPAR *fp);
static void CS_fpsec(NUFPAR *fp);
static void CS_lowend_disthack(NUFPAR *fp);
static void CS_draw_gizmo_sys(NUFPAR *fp);
static void CS_deb_page(NUFPAR *fp);
static void CS_draw_world(NUFPAR *fp);
static void CS_next_cut_scene(NUFPAR *fp);
static void CS_go_through_door(NUFPAR *fp);
static void CS_tex_anim(NUFPAR *fp);
static void CS_fadefog(NUFPAR *fp);
static void CS_cutsceneplayerobj(NUFPAR *fp);
static void CS_goto_level(NUFPAR *fp);
static inline void CS_subtitle(NUFPAR *fp);

static NUFPCOMJMP CutScene_ConfigKeywords[] = {
    {const_cast<char *>("fpsec"), CS_fpsec},
    {const_cast<char *>("no_fog"), CS_no_fog},
    {const_cast<char *>("draw_world"), CS_draw_world},
    {const_cast<char *>("lowend_lowbits"), CS_lowend_lowbits},
    {const_cast<char *>("lowend_disthack"), CS_lowend_disthack},
    {const_cast<char *>("is_outro"), CS_is_outro},
    {const_cast<char *>("unskippable_in_story"), CS_unskippable_in_story},
    {const_cast<char *>("skip_use_goto"), CS_skip_use_goto},
    {const_cast<char *>("in_game"), CS_in_game},
    {const_cast<char *>("blobshadow_alpha"), CS_blobshadow_alpha},
    {const_cast<char *>("blobshadow_fadenear"), CS_blobshadow_fadenear},
    {const_cast<char *>("blobshadow_fadefar"), CS_blobshadow_fadefar},
    {const_cast<char *>("reflect_range"), CS_reflect_range},
    {const_cast<char *>("render_group"), CS_render_group},
    {const_cast<char *>("deb_page"), CS_deb_page},
    {const_cast<char *>("snap_out"), CS_snap_out},
    {const_cast<char *>("wipe_out"), CS_wipe_out},
    {const_cast<char *>("new_mode"), CS_new_mode},
    {const_cast<char *>("cam_only"), CS_cam_only},
    {const_cast<char *>("replace_players"), CS_replace_players},
    {const_cast<char *>("burnout_threshold"), CS_burnout_threshold},
    {const_cast<char *>("burnout_intensity"), CS_burnout_intensity},
    {const_cast<char *>("burnout_flare"), CS_burnout_flare},
    {const_cast<char *>("looping"), CS_looping},
    {const_cast<char *>("start_cam"), CS_start_cam},
    {const_cast<char *>("super_widescreen"), CS_super_widescreen},
    {const_cast<char *>("go_through_door"), CS_go_through_door},
    {const_cast<char *>("sfx"), CS_sfx},
    {const_cast<char *>("play_sfx"), CS_play_sfx},
    {const_cast<char *>("goto_level"), CS_goto_level},
    {const_cast<char *>("next_cut_scene"), CS_next_cut_scene},
    {const_cast<char *>("holdaudio"), CS_hold_audio},
    {const_cast<char *>("hold_audio"), CS_hold_audio},
    {const_cast<char *>("level_intro"), CS_level_intro},
    {const_cast<char *>("tex_anim"), CS_tex_anim},
    {const_cast<char *>("fadefog"), CS_fadefog},
    {const_cast<char *>("subtitle"), CS_subtitle},
    {const_cast<char *>("play_once"), CS_playonce},
    {const_cast<char *>("nextcutscene_inplayablelevel"), CS_nextcutscene_inplayablelevel},
    {const_cast<char *>("cutsceneplayerobj"), CS_cutsceneplayerobj},
    {const_cast<char *>("cutsceneplayer_obj"), CS_cutsceneplayerobj},
    {const_cast<char *>("nearclip_360"), CS_nearclip},
    {const_cast<char *>("farclip_360"), CS_farclip},
    {const_cast<char *>("nearclip_pc"), CS_nearclip},
    {const_cast<char *>("farclip_pc"), CS_farclip},
    {const_cast<char *>("nearclip"), CS_nearclip},
    {const_cast<char *>("farclip"), CS_farclip},
    {const_cast<char *>("drawgizmosys"), CS_draw_gizmo_sys},
    {NULL, NULL},
};

static void CutScene_Configure(CUTINFO *cut, char *name, VARIPTR *buf, VARIPTR *buf_end);

static CUTSYS *CutScenes_LoadFromList(NUGSCN *gscn1, NUGSCN *gscn2, i32 param1, VARIPTR *buf, VARIPTR *buf_end,
                                      i32 param2, i32 param3, WORLDINFO *world) {
    CUTSYS *sys;
    void *initial;
    CUTINFO *cut;
    CUTINFO *entries[32];
    char full_path[128];
    char config_name[128];

    CS_area = param2;
    initial = buf->void_ptr;
    sys = (CUTSYS *)ALIGN((usize)initial, 4);
    buf->void_ptr = sys + 1;
    CS_cutsys = sys;
    CS_worldinfo = world;
    sys->character_bits = reinterpret_cast<u32 *>(buf->void_ptr);
    buf->void_ptr = reinterpret_cast<char *>(buf->void_ptr) + ((CHARCOUNT + 0x1f) / 32) * 4;
    for (i32 i = 0; i < CUTCOUNT; ++i) {
        if (CutList[i].level != param3) {
            continue;
        }
        void *saved = buf->void_ptr;
        entries[sys->count] = reinterpret_cast<CUTINFO *>(saved);
        buf->addr = ALIGN(buf->addr + sizeof(*cut), 4);
        cut = entries[sys->count];
        NuStrCpy(full_path, "cut\\");
        NuStrCat(full_path, CutList[i].directory);
        NuStrCat(full_path, "\\");
        NuStrCat(full_path, CutList[i].filename);
        NuStrCpy(config_name, full_path);
        NuStrCat(config_name, ".txt");
        CutScene_Configure(cut, config_name, buf, buf_end);
        if ((cut->flags & 0x800) != 0 || InStory() != 0 ||
            (LDataList[param3].flags & (LEVEL_INTRO | LEVEL_MIDTRO | LEVEL_OUTRO)) != 0) {
            NuStrCpy(config_name, full_path);
            NuStrCat(config_name, ".cu2");
            cut->scene = NuGCutSceneLoad(config_name, buf, buf_end, 0);
            if (cut->scene != NULL) {
                NuStrCpy(cut->name, CutList[i].filename);
                NuGCutSceneFixUp(reinterpret_cast<NUGCUTSCENE_s *>(cut->scene), gscn1, 0, param1);
                NuGCutSceneFixUpExtra(reinterpret_cast<NUGCUTSCENE_s *>(cut->scene), gscn2);
                cut->instance = instNuGCutSceneCreate(reinterpret_cast<NUGCUTSCENE_s *>(cut->scene), NULL, NULL,
                                                      CutList[i].filename, buf, 0);
                if (cut->instance != NULL) {
                    reinterpret_cast<instNUGCUTSCENE_s *>(cut->instance)->rate =
                        cut->frames_per_second * DEFAULTFRAMETIME;
                    buf->addr = ALIGN(buf->addr, 0x10);
                }
                ++sys->count;
                continue;
            }
        }
        buf->void_ptr = saved;
    }
    if (sys->count <= 0) {
        buf->void_ptr = initial;
        return NULL;
    }
    sys->cuts = reinterpret_cast<CUTINFO **>(buf->void_ptr);
    const usize table_size = sys->count * sizeof(CUTINFO *);
    memmove(sys->cuts, entries, table_size);
    buf->addr = ALIGN(buf->addr + table_size * sys->count, 4);
    return sys;
}

typedef isize __attribute__((may_alias)) NuGCutWord;

struct instNUGCUTSCENE_s;
struct NUGCUTLOCATORSYS_s;
struct instNUGCUTLOCATOR_s;
struct NUGCUTLOCATOR_s;
struct numtx_s;

i32 (*CutScenePlayer_AcceptFn)(CUTSCENEPLAYERCLIP *) = NULL;
extern i16 tCHAPTER;

CUTSCENEPLAYER_s *CutScenePlayer = NULL;

extern void (*CutScene_OverrideConfigFileNameFn)(char *, i32, i32);

struct ani3_animheader_s;

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
} // extern "C"

union variptr_u;

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

struct nuhspecial_s;

extern "C" {
    extern "C++" FadeSystem FadeSys;
}

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

static __used__ __attribute__((noclone)) void eduiFntPrintExGeneric(void *font, int x, int y,
                                                                                int alignment, char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz");
static __used__ void eduiFntPrintExGeneric(void *font, int x, int y, int alignment, char *format, ...);

static __attribute__((noclone)) void eduiFntPrintExLeft(void *font, int x, int y, int alignment,
                                                                  char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.67");

static __attribute__((noclone)) void eduiFntPrintExCentre(void *font, int x, int y, int alignment,
                                                                    char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.68");

#if defined(__clang__)
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
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

static void eduiFntPrintExLeft(void *font, int x, int y, int, char *format, ...);
static void eduiFntPrintExCentre(void *font, int x, int y, int, char *format, ...);

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
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

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
} // extern "C"

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

extern i32 CUTCAMONLY, GUNSHIPAHACK, netcamera, complexsockposition_forcesock, movegamecamera_forcesock;

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
} // extern "C"

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

// Original defaults: 0x668d30 and 0x668d40, each a four-byte integer.

static void CS_no_fog(NUFPAR *) {
    CS_CutInfo->flags |= 4;
}

static void CS_lowend_lowbits(NUFPAR *) {
    CS_CutInfo->flags |= 0x10000;
}

static void CS_is_outro(NUFPAR *) {
    CS_CutInfo->flags |= 0x20000;
}

static void CS_unskippable_in_story(NUFPAR *) {
    CS_CutInfo->flags |= 0x40000;
}

static void CS_skip_use_goto(NUFPAR *) {
    CS_CutInfo->flags |= 0x80000;
}

static void CS_in_game(NUFPAR *) {
    CS_CutInfo->flags &= ~3U;
    CS_CutInfo->flags |= 0x800;
}

static void CS_snap_out(NUFPAR *) {
    CS_CutInfo->flags |= 0x10;
}

static void CS_cam_only(NUFPAR *) {
    CS_CutInfo->flags |= 0x20;
}

static void CS_wipe_out(NUFPAR *) {
    CS_CutInfo->flags |= 0x100;
}

static void CS_new_mode(NUFPAR *) {
    CS_CutInfo->flags |= 0x400;
}

static void CS_replace_players(NUFPAR *) {
    CS_CutInfo->flags |= 0x40;
}

static void CS_looping(NUFPAR *) {
    CS_CutInfo->flags |= 0x200;
}

static void CS_start_cam(NUFPAR *) {
    CS_CutInfo->flags |= 0x2000;
}

static void CS_super_widescreen(NUFPAR *) {
    CS_CutInfo->flags |= 0x4000;
}

static void CS_level_intro(NUFPAR *) {
    CS_CutInfo->flags |= 0x1000;
}

static void CS_hold_audio(NUFPAR *) {
    CS_CutInfo->linked_audio = 1;
}

static void CS_playonce(NUFPAR *) {
    CS_CutInfo->end_flags |= 1;
}

static void CS_nextcutscene_inplayablelevel(NUFPAR *) {
    CS_CutInfo->end_flags |= 2;
}

static void CutScene_CreateCharacterInstance(NUGCUTCHAR_s *character, instNUGCUTCHAR_s *instance, variptr_u *) {
    if ((character->flags & 2) != 0) {
        instance->character_model = character->character_model;
    }
}

static void CutScene_ResetCharacters(instNUGCUTSCENE_s *instance) {
    NUGCUTSCENE_s *cutscene = instance->cutscene;
    NUGCUTCHARSYS_s *character_system = cutscene->character_system;
    instNUGCUTCHARSYS_s *character_instance = instance->character_instance;

    i32 i = 0;
    while (i < character_system->character_count) {
        instNUGCUTCHAR_s *character = &character_instance->characters[i];
        character->field_04 = 0;
        character->field_14 = 0;
        character->field_15 = 0xff;
        ++i;
    }
}

static void bgAckStreamCutScene(bgprocinfo_s *) {
    stream_cut_issued = 0;
}

static __used__ void LocatorFunction_Blaster(instNUGCUTSCENE_s *, NUGCUTLOCATORSYS_s *, instNUGCUTLOCATOR_s *,
                                             NUGCUTLOCATOR_s *locator, float frame, numtx_s *parent_mtx, int) {
    nuanimtime_s time;
    NuAnimData2CalcTime(locator->animation, frame, &time);
    if (NuGCutLocatorIsVisble(locator, frame, &time, NULL, NULL) == 0) {
        return;
    }
    // Retail realigns the frame (and esp,-16) for this matrix.
    NUMTX_ALIGNED16 matrix;
    NuGCutLocatorCalcMtx(locator, frame, &matrix, &time);
    if ((locator->flags & 4) != 0) {
        NuMtxPreTranslate(&matrix, &locator->pivot);
    }
    if (parent_mtx != NULL) {
        NuMtxMul(&matrix, &matrix, parent_mtx);
    }
    if (CutSceneSys != NULL) {
        Draw3DObjectMtx(NULL, CutSceneSys->blaster_object_0, &matrix);
        Draw3DObjectMtx(NULL, CutSceneSys->blaster_object_1, &matrix);
    }
}

static void CutScene_EvalCharacter(instNUGCUTSCENE_s *cutscene_instance, NUGCUTSCENE_s *, instNUGCUTCHAR_s *instance,
                                   NUGCUTCHAR_s *character, f32 frame) {
    GameObject_s *object = NULL;
    if ((character->flags & 2) == 0) {
        object = static_cast<GameObject_s *>(instance->character_model);
    }
    i32 visible;
    NUMTX_ALIGNED32 matrix;
    NuGCutCharAnimProcess(character, frame, &matrix, &visible, NULL, NULL, NULL, NULL, NULL);
    if (static_cast<i8>(cutscene_instance->flags_88) < 0) {
        NuMtxMul(&matrix, &matrix, &cutscene_instance->matrix);
    }
    if (object != NULL) {
        object->apiobj.field_0xb8 = matrix;
        object->apiobj.position = *NUMTX_GET_ROW_VEC(&matrix, 3);
    }
}

static void CutScene_DrawCharacter(instNUGCUTSCENE_s *cutscene_instance, NUGCUTSCENE_s *cutscene,
                                   instNUGCUTCHAR_s *instance, NUGCUTCHAR_s *character, f32 frame, i32 paused) {
    i32 visible;
    i32 layer_mask = -1;
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    CUTSYS *cutscene_system = world->cutscene_sys;
    if (cutscene_system == NULL) {
        return;
    }

    CUTINFO *cut = NULL;
    for (i32 i = 0; i < cutscene_system->count && cut == NULL; ++i) {
        CUTINFO *candidate = cutscene_system->cuts[i];
        if (candidate->instance == cutscene_instance) {
            cut = candidate;
        }
    }

    GameObject_s *scene_object = NULL;
    CHARACTERMODEL_s *model = NULL;
    if (CutScene_ReplaceCharacterModelFn != NULL && cut != NULL) {
        i32 replacement_id = CutScene_ReplaceCharacterModelFn(cut, character);
        if (replacement_id != -1 && apicharsys->playermodelids[replacement_id] != -1) {
            model = &apicharsys->models[apicharsys->playermodelids[replacement_id]];
        }
    }
    if (model == NULL) {
        if ((character->flags & 2) == 0) {
            scene_object = static_cast<GameObject_s *>(instance->character_model);
            model = scene_object->apiobj.character_model;
        } else {
            model = static_cast<CHARACTERMODEL_s *>(instance->character_model);
        }
    }
    nuhgobj_s *hierarchy = model->hierarchy;
    if (hierarchy == NULL) {
        return;
    }

    const i32 character_id = model->model_id;
    CHARACTERDATA *character_data = &apicharsys->char_data[character_id];
    GAMECHARACTERDATA *game_data = static_cast<GAMECHARACTERDATA *>(character_data->field11_0x24);
    // Retail realigns this frame with `and esp, -32` for the world matrix.
    NUMTX_ALIGNED32 world_matrix;
    u32 animation_index;
    f32 animation_rate;
    f32 blend_time;
    f32 animation_start_frame;
    bool use_low_detail = false;
    if (cut != NULL) {
        use_low_detail = (cut->flags & 0x10000) != 0 && g_isLowEndDevice != 0;
        NuGCutCharAnimProcess(character, frame, &world_matrix, &visible, &animation_index, &animation_rate, &blend_time,
                              &animation_start_frame, &layer_mask);
        if (cut->low_end_distance > 0.0f && g_isLowEndDevice != 0 &&
            NuVecDistSqr(reinterpret_cast<NUVEC *>(&world_matrix.m30), reinterpret_cast<NUVEC *>(&pNuCam->mtx.m30),
                         NULL) > cut->low_end_distance * cut->low_end_distance) {
            return;
        }
    } else {
        NuGCutCharAnimProcess(character, frame, &world_matrix, &visible, &animation_index, &animation_rate, &blend_time,
                              &animation_start_frame, &layer_mask);
    }
    if (paused != 0) {
        animation_rate = 0.0f;
    }
    if (layer_mask == -1) {
        if (!use_low_detail) {
            if (reinterpret_cast<u8 *>(CutSceneSys)[7] == 0) {
                layer_mask = static_cast<i32>(game_data->layer_mask_special);
            } else {
                layer_mask = static_cast<i32>(game_data->layer_mask);
            }
        } else {
            layer_mask = static_cast<i32>(game_data->layer_mask_low);
        }
    }
    if (static_cast<i8>(cutscene_instance->flags_88) < 0) {
        NuMtxMul(&world_matrix, &world_matrix, &cutscene_instance->matrix);
    }
    if (scene_object != NULL) {
        scene_object->apiobj.field_0xb8 = world_matrix;
        scene_object->apiobj.position = *reinterpret_cast<NUVEC *>(&world_matrix.m30);
    }
    if (visible == 0) {
        return;
    }

    if (NuIOS_IsLowEndDevice() != 0 && RETAKEINTRO2_LDATA != NULL && RETAKEINTRO2_LDATA == world->current_level) {
        static f32 lastFrameTime;
        static i32 gungansRenderedThisFrame;
        layer_mask = static_cast<i32>(game_data->layer_mask_medium);
        if (frame != lastFrameTime) {
            gungansRenderedThisFrame = 0;
            lastFrameTime = frame;
        }
        if (model->model_id == id_GUNGAN) {
            if (gungansRenderedThisFrame++ > 7) {
                return;
            }
        }
    }

    i16 render_indices[32];
    const i32 render_count = game_data->make_layer_list(model, render_indices, static_cast<u32>(layer_mask));
    if (__builtin_expect(render_count < 1, 1)) {
        return;
    }

    if (instance->field_15 != animation_index) {
        const u8 requested_animation = static_cast<u8>(animation_index);
        if (!(blend_time > 0.0f) || instance->field_15 == 0xff) {
            instance->animation_frame_a = !(animation_start_frame > 1.0f) ? 1.0f : animation_start_frame;
            instance->field_16 = requested_animation;
        } else {
            if ((instance->field_14 & 1) == 0) {
                if (instance->field_15 != 0) {
                    instance->field_16 = instance->field_15;
                    animation_index = instance->field_15;
                } else {
                    instance->animation_frame_a = frame;
                    instance->field_16 = requested_animation;
                }
            } else {
                instance->field_16 = instance->field_17;
                instance->animation_frame_a = instance->animation_frame_b;
                animation_index = instance->field_17;
            }
            if (!(animation_start_frame > 1.0f)) {
                animation_start_frame = 1.0f;
            }
            instance->field_17 = requested_animation;
            instance->animation_frame_b = animation_start_frame;
            if (requested_animation != static_cast<u8>(animation_index)) {
                instance->blend_progress = 0;
                instance->field_14 |= 1;
            }
        }
        instance->field_15 = requested_animation;
    }

    bool blending = (instance->field_14 & 1) != 0;
    u32 animation_a_index = instance->field_16;
    if (blending) {
        if (blend_time <= 0.0f) {
            instance->blend_progress = 0;
            animation_a_index = instance->field_17;
            instance->field_14 ^= 1;
            instance->field_16 = instance->field_17;
            instance->animation_frame_a = instance->animation_frame_b;
            blending = false;
        } else {
            instance->blend_progress += (1.0f / blend_time) * (FRAMETIME * 60.0f);
            if (__builtin_expect(instance->blend_progress >= 1.0f, 0)) {
                instance->blend_progress = 0;
                animation_a_index = instance->field_17;
                instance->field_16 = instance->field_17;
                instance->field_14 ^= 1;
                instance->animation_frame_a = instance->animation_frame_b;
                blending = false;
            }
        }
    }

    ani3_animheader_s *animation_a;
    nuanimdata2_s *dwa_animation_a;
    if (animation_a_index == 0) {
        instance->animation_frame_a = frame;
        animation_a = reinterpret_cast<ani3_animheader_s *>(character->face_animation);
        dwa_animation_a = character->extra_animation;
    } else {
        animation_a = static_cast<ani3_animheader_s *>(model->model_data_b[animation_a_index - 1]);
        dwa_animation_a = static_cast<nuanimdata2_s *>(model->model_data_c[animation_a_index - 1]);
    }

    NUMTX joint_matrices[256];
    void **dwa = NULL;
    if (!blending) {
        if (animation_a == NULL) {
            NuHGobjEval(hierarchy, 0, NULL, joint_matrices);
        } else {
            NuHGobjEvalAnim2(hierarchy, animation_a, instance->animation_frame_a, 0, NULL, joint_matrices);
        }
        if (dwa_animation_a != NULL) {
            dwa = NuHGobjEvalDwa2(render_count, render_indices, dwa_animation_a, instance->animation_frame_a);
        }
    } else {
        ani3_animheader_s *animation_b;
        nuanimdata2_s *dwa_animation_b;
        if (instance->field_17 == 0) {
            instance->animation_frame_b = frame;
            animation_b = reinterpret_cast<ani3_animheader_s *>(character->face_animation);
            dwa_animation_b = character->extra_animation;
        } else {
            animation_b = static_cast<ani3_animheader_s *>(model->model_data_b[instance->field_17 - 1]);
            dwa_animation_b = static_cast<nuanimdata2_s *>(model->model_data_c[instance->field_17 - 1]);
        }
        if (animation_a != NULL && animation_b != NULL) {
            NuHGobjEvalAnimBlend2(hierarchy, animation_a, instance->animation_frame_a, animation_b,
                                  instance->animation_frame_b, instance->blend_progress, 0, NULL, joint_matrices);
        } else if (animation_b != NULL) {
            NuHGobjEvalAnim2(hierarchy, animation_b, instance->animation_frame_b, 0, NULL, joint_matrices);
        } else if (__builtin_expect(animation_a != NULL, 0)) {
            NuHGobjEvalAnim2(hierarchy, animation_a, instance->animation_frame_a, 0, NULL, joint_matrices);
        } else {
            NuHGobjEval(hierarchy, 0, NULL, joint_matrices);
        }

        if (__builtin_expect(dwa_animation_a != NULL && dwa_animation_b != NULL, 1)) {
            dwa = NuHGobjEvalDwaBlend2(render_count, render_indices, dwa_animation_a, instance->animation_frame_a,
                                       dwa_animation_b, instance->animation_frame_b, instance->blend_progress);
        }

        if (animation_b != NULL && instance->field_17 != 0xff && instance->field_17 != 0 &&
            (cut == NULL || cut != CutStopInfo || CutSceneWaiting == 0)) {
            const i32 current_animation = static_cast<u8>(instance->field_17) - 1;
            const f32 end_frame = AnimEndFrame(model, current_animation);
            instance->animation_frame_b += FRAMETIME * 60.0f * animation_rate;
            if (instance->animation_frame_b > end_frame) {
                CHARACTERANIM_s *animation_info =
                    static_cast<CHARACTERANIM_s *>(model->model_data_a[current_animation]);
                if (__builtin_expect((animation_info->flags & 2) == 0, 1)) {
                    instance->animation_frame_b = end_frame;
                } else {
                    instance->animation_frame_b = instance->animation_frame_b - end_frame + 1.0f;
                }
            }
        }
    }

    NUVEC locator_positions[16];
    NUMTX locator_matrices[16];
    StoreLocatorCoordinates(model, &world_matrix, joint_matrices, locator_positions, locator_matrices);

    if (animation_a != NULL && instance->field_16 != 0xff && instance->field_16 != 0 &&
        (cut == NULL || cut != CutStopInfo || CutSceneWaiting == 0)) {
        const i32 current_animation = static_cast<u8>(instance->field_16) - 1;
        const f32 end_frame = AnimEndFrame(model, current_animation);
        instance->animation_frame_a += FRAMETIME * 60.0f * animation_rate;
        if (instance->animation_frame_a > end_frame) {
            CHARACTERANIM_s *animation_info = static_cast<CHARACTERANIM_s *>(model->model_data_a[current_animation]);
            if ((animation_info->flags & 2) != 0) {
                instance->animation_frame_a = instance->animation_frame_a - end_frame + 1.0f;
            } else {
                instance->animation_frame_a = end_frame;
            }
        }
    }

    EnableShadowMapRendering(0);
    if (Cheats_CheckFlags(1) != 0) {
        SetZeroLights();
    } else {
        FindAndSetLights(reinterpret_cast<NUVEC *>(&world_matrix.m30), 1.0f, world->rtl_set);
    }

    u8 render_character = 1;
    if ((character->flags & 0x20) == 0 && (character_data->model_flags & 0x10000) != 0) {
        render_character = static_cast<u8>(((character->flags >> 4) ^ 1) & 1);
    }
    hierarchy->suppress_shadow_surface_points = render_character;

    if ((game_data->flags_090 & 0x8000) != 0) {
        APITransparentCharDraw(hierarchy, &world_matrix, render_count, render_indices, joint_matrices, dwa,
                               character->flags & 8);
    }
    NuHGobjRndrMtxDwa(hierarchy, &world_matrix, render_count, render_indices, joint_matrices, dwa,
                      character->flags & 8);

    NUMTX *attachment_matrix = NULL;
    if ((character_data->flags & 1) != 0) {
        attachment_matrix = &world_matrix;
        i32 locator = game_data->thingy_locator;
        if (locator != -1 && model->points_of_interest[locator] != NULL) {
            attachment_matrix = &locator_matrices[locator];
        }
        CharScene_Draw(world, character_id, attachment_matrix, NULL);
    }
    ResetShadowMapRendering();

    i32 surface = -1;
    f32 ground_height = 2000000.0f;
    bool shadow_sampled = false;
    i32 platform = -1;
    if ((character->flags & 0x10) != 0 && CutBlobShadowAlpha != 0 && (character_data->model_flags & 0x10000) == 0 &&
        g_isLowEndDevice == 0) {
        u8 shadow_alpha = CutBlobShadowAlpha;
        if (game_data->field_0xf6 != 0xff) {
            shadow_alpha = game_data->field_0xf6;
        }
        if (shadow_alpha != 0) {
            f32 shadow_radius = game_data->field_0x8c;
            if (game_data->shadow_locators != 0) {
                if (!(shadow_radius < 99.0f)) {
                    shadow_radius = character_data->collision_radius * character_data->model_scale;
                }
                if (shadow_radius > 0.0f) {
                    f32 alpha = BlobShadowFade(reinterpret_cast<NUVEC *>(&world_matrix.m30), CutBlobShadowFadeNear,
                                               CutBlobShadowFadeFar, 1.0f);
                    if (alpha > 0.0f) {
                        for (i32 i = 0; i < 16; ++i) {
                            if ((game_data->shadow_locators & (1 << i)) != 0 && model->points_of_interest[i] != NULL) {
                                NUVEC shadow_position;
                                shadow_position.x = locator_positions[i].x;
                                f32 locator_ground = GameShadow(NULL, &locator_positions[i], 5.0f, -1);
                                shadow_position.z = locator_positions[i].z;
                                if (locator_ground != 2000000.0f && ShadNorm.y > 0.0f) {
                                    shadow_position.y = locator_ground + 0.005f;
                                    u16 x_rotation;
                                    u16 z_rotation;
                                    FindAnglesZX(&ShadNorm, &x_rotation, &z_rotation);
                                    NuRndrAddShadow(
                                        &shadow_position, shadow_radius,
                                        static_cast<i32>(alpha * static_cast<f32>(shadow_alpha) * ShadNorm.y),
                                        x_rotation, 0, z_rotation);
                                }
                            }
                        }
                    }
                }
                ground_height = 2000000.0f;
                platform = -1;
                shadow_sampled = false;
            } else {
                if (!(shadow_radius < 99.0f)) {
                    shadow_radius = character_data->collision_radius * character_data->model_scale * 2.0f;
                }
                if (shadow_radius > 0.0f) {
                    ground_height = GameShadow(NULL, reinterpret_cast<NUVEC *>(&world_matrix.m30), 5.0f, -1);
                    if (ground_height != 2000000.0f) {
                        platform = NewShadowOnPlatform();
                        surface = ShadowInfo();
                        if (ShadNorm.y > 0.0f) {
                            NUVEC shadow_position = *reinterpret_cast<NUVEC *>(&world_matrix.m30);
                            shadow_position.y = ground_height + 0.005f;
                            f32 alpha = BlobShadowFade(&shadow_position, CutBlobShadowFadeNear, CutBlobShadowFadeFar,
                                                       ShadNorm.y);
                            if (alpha > 0.0f) {
                                u16 x_rotation;
                                u16 z_rotation;
                                FindAnglesZX(&ShadNorm, &x_rotation, &z_rotation);
                                NuRndrAddShadow(&shadow_position, shadow_radius,
                                                static_cast<i32>(static_cast<f32>(shadow_alpha) * alpha * ShadNorm.y),
                                                x_rotation, 0, z_rotation);
                            }
                        }
                    }
                    shadow_sampled = true;
                }
            }
        }
    }

    f32 reflection_height = 2000000.0f;
    if ((character->flags & 0x20) != 0 && (game_data->flags_090 & 0x8000) == 0 &&
        NuVecDistSqr(reinterpret_cast<NUVEC *>(&world_matrix.m30), reinterpret_cast<NUVEC *>(&GameCam->render_mtx.m30),
                     NULL) < CutReflectRange2 &&
        Reflections_On != 0) {
        if (!shadow_sampled) {
            ground_height = GameShadow(NULL, reinterpret_cast<NUVEC *>(&world_matrix.m30), 5.0f, -1);
            if (ground_height != 2000000.0f) {
                platform = NewShadowOnPlatform();
                surface = ShadowInfo();
            }
        }
        if (surface >= 0 && surface < 32 && (TerSurface[surface].flags & 2) != 0 && ground_height != 2000000.0f) {
            reflection_height = ground_height;
        } else if (platform != -1) {
            reflection_height = FindReflectionNoPlatforms(reinterpret_cast<NUVEC *>(&world_matrix.m30));
        }
        if (reflection_height != 2000000.0f) {
            NUMTX reflection_matrix;
            if (MatrixReflection(&world_matrix, 2, reflection_height, WORLD->current_level->unknown_0cc,
                                 &reflection_matrix) != 0) {
                NuRndrStartReflectionRender(0);
                NuHGobjRndrMtxDwa(hierarchy, &reflection_matrix, render_count, render_indices, joint_matrices, dwa,
                                  character->flags & 8);
                if (attachment_matrix != NULL) {
                    NUMTX attachment_reflection;
                    if (MatrixReflection(attachment_matrix, 2, reflection_height, WORLD->current_level->unknown_0cc,
                                         &attachment_reflection) != 0) {
                        CharScene_Draw(world, character_id, NULL, &attachment_reflection);
                    }
                }
                NuRndrEndReflectionRender();
            }
        }
    }

    if (CutSceneSys->field_04 != -1 && reinterpret_cast<i8 *>(CutSceneSys)[6] != -1 &&
        Cheat_IsOn(reinterpret_cast<i8 *>(CutSceneSys)[6]) != 0) {
        dco_id = character_id;
        dco_gcdata = game_data;
        dco_cmodel = model;
        dco_wearinghat = 0;
        dco_reflectaxis = reflection_height != 2000000.0f ? 2 : 0;
        dco_reflectcoord = reflection_height;
        DrawObjectOnCharacter(NULL, NULL, CutSceneSys->field_04, NULL, game_data->head_locator, -1, locator_matrices,
                              reflection_height != 2000000.0f, static_cast<u32>(layer_mask), NULL, NULL, 1.0f, 1.0f);
    }

    if (character->locator_index != 0xff) {
        NUGCUTLOCATORSYS_s *locator_system = cutscene->locator_system;
        for (i32 i = 0; i < character->locator_count; ++i) {
            i32 locator_index = character->locator_index + i;
            NUGCUTLOCATOR_s *locator = &locator_system->locators[locator_index];
            NUMTX *parent_matrix = &world_matrix;
            if (locator->field_5a != 0xff) {
                NUMTX locator_parent;
                NuMtxMulVU0(&locator_parent, &joint_matrices[locator->field_5a], &world_matrix);
                instNuGCutLocatorUpdate(cutscene_instance, locator_system,
                                        &cutscene_instance->locator_instance->locators[locator_index], locator, frame,
                                        &locator_parent, paused);
            } else {
                instNuGCutLocatorUpdate(cutscene_instance, locator_system,
                                        &cutscene_instance->locator_instance->locators[locator_index], locator, frame,
                                        parent_matrix, paused);
            }
        }
    }
}

static void bgLoadStreamCutScene(bgprocinfo_s *) {
    if (cutscene_load_instance != NULL) {
        instNuGCutSceneServiceLoad();
    }
}

static void CutScene_Configure(CUTINFO *cut, char *name, VARIPTR *buf, VARIPTR *buf_end) {
    CUTSCENEPLAYEROBJ state_entries[32];

    CS_CutInfo = cut;
    CS_buffptr = buf;
    CS_buffend = buf_end;
    cut->state_count = 0;
    cut->flags = 3;
    cut->frames_per_second = 30.0f;
    cut->burnout_threshold = 1.0f;
    cut->burnout_intensity = 0.0f;
    cut->burnout_flare = 0.0f;
    cut->camera_near_clip = 0.0f;
    cut->camera_far_clip = 0;
    cut->legacy_music_index = -1;
    cut->skip_level = -1;
    cut->debris_render_group = 2;
    cut->linked_audio = 0;
    cut->door_name[0] = '\0';
    cut->next_cutscene[0] = '\0';
    cut->music_handle = -1;

    for (i32 i = 0; i < 6; ++i) {
        cut->sfx[i].id = -1;
    }
    cut->blob_shadow_alpha = 0xff;
    cut->blob_shadow_fade_near = 0xff;
    cut->blob_shadow_fade_far = 0xff;
    cut->reflection_range = 0xff;
    for (i32 i = 0; i < 4; ++i) {
        cut->texture_animations[i].index = -1;
    }
    for (i32 i = 0; i < 2; ++i) {
        cut->fade_fog[i].frame = 0.0f;
        cut->fade_fog[i].near_distance = 0.0f;
        cut->fade_fog[i].far_distance = 0.0f;
        cut->fade_fog[i].value = 0.0f;
    }
    cut->subtitle_count = 0;
    cut->subtitle_data = NULL;
    cut->end_flags &= ~3U;
    cut->pad_18b = 0;
    cut->low_end_distance = 0.0f;
    cut->state_entries = state_entries;

    NUFPAR *fp = NuFParCreate(name);
    if (fp == NULL) {
        return;
    }
    CS_texanimcount = 0;
    CS_fadefogcount = 0;
    NuFParPushCom(fp, CutScene_ConfigKeywords);
    while (NuFParGetLine(fp) != 0) {
        if (NuFParGetWord(fp) == 0) {
            continue;
        }
        NuFParInterpretWord(fp);
    }
    NuFParDestroy(fp);

    CUTINFO *info = CS_CutInfo;
    if (info->blob_shadow_fade_near > info->blob_shadow_fade_far) {
        info->blob_shadow_fade_near = info->blob_shadow_fade_far;
    }
    if (info->subtitle_data != NULL && info->subtitle_count == 0) {
        info->subtitle_data = NULL;
    }
    if (info->state_count == 0) {
        info->state_entries = NULL;
    } else {
        buf->void_ptr = reinterpret_cast<void *>(ALIGN(buf->addr, 4));
        info->state_entries = reinterpret_cast<CUTSCENEPLAYEROBJ *>(buf->void_ptr);
        const i32 size = info->state_count * sizeof(CUTSCENEPLAYEROBJ);
        memmove(info->state_entries, state_entries, size);
        buf->void_ptr = reinterpret_cast<char *>(buf->void_ptr) + size;
    }
}

static void CS_farclip(NUFPAR *fp) {
    CS_CutInfo->camera_far_clip = static_cast<u16>(NuFParGetInt(fp));
}

static void CS_render_group(NUFPAR *fp) {
    CS_CutInfo->debris_render_group = static_cast<i8>(NuFParGetInt(fp));
}

static void CS_reflect_range(NUFPAR *fp) {
    CS_CutInfo->reflection_range = static_cast<u8>(NuFParGetInt(fp));
}

static void CS_blobshadow_fadefar(NUFPAR *fp) {
    CS_CutInfo->blob_shadow_fade_far = static_cast<u8>(NuFParGetInt(fp));
}

static void CS_blobshadow_fadenear(NUFPAR *fp) {
    CS_CutInfo->blob_shadow_fade_near = static_cast<u8>(NuFParGetInt(fp));
}

static void CS_blobshadow_alpha(NUFPAR *fp) {
    i32 alpha = NuFParGetInt(fp);
    alpha = alpha < 0 ? 0 : alpha;
    alpha = alpha > 0xfe ? 0xfe : alpha;
    CS_CutInfo->blob_shadow_alpha = static_cast<u8>(alpha);
}

static void CS_nearclip(NUFPAR *fp) {
    CS_CutInfo->camera_near_clip = NuFParGetFloat(fp);
}

static void CS_burnout_flare(NUFPAR *fp) {
    CS_CutInfo->burnout_flare = NuFParGetFloat(fp);
    CS_CutInfo->flags |= 0x80;
}

static void CS_burnout_intensity(NUFPAR *fp) {
    CS_CutInfo->burnout_intensity = NuFParGetFloat(fp);
    CS_CutInfo->flags |= 0x80;
}

static void CS_burnout_threshold(NUFPAR *fp) {
    CS_CutInfo->burnout_threshold = NuFParGetFloat(fp);
    CS_CutInfo->flags |= 0x80;
}

static void CS_fpsec(NUFPAR *fp) {
    CS_CutInfo->frames_per_second = NuFParGetFloat(fp);
}

static inline void CS_subtitle(NUFPAR *fp) {
    if (CS_CutInfo->subtitle_data == NULL) {
        CS_CutInfo->subtitle_data = reinterpret_cast<CUTSCENESUBTITLE *>(CS_buffptr->addr);
    }

    CUTSCENESUBTITLE *subtitle = &CS_CutInfo->subtitle_data[CS_CutInfo->subtitle_count];
    subtitle->text_id = -1;
    subtitle->start_frame = 0.0f;
    subtitle->end_frame = 0.0f;
    subtitle->red = CUTSUBTITLEDEFAULT_R;
    subtitle->green = CUTSUBTITLEDEFAULT_G;
    subtitle->blue = CUTSUBTITLEDEFAULT_B;
    subtitle->alpha = CUTSUBTITLEDEFAULT_A;
    subtitle->alignment = CUTSUBTITLEDEFAULT_ALIGN;
    subtitle->x = CUTSUBTITLEDEFAULT_X;
    subtitle->y = CUTSUBTITLEDEFAULT_Y;
    subtitle->x_scale = CUTSUBTITLEDEFAULT_SCALEX;
    subtitle->y_scale = CUTSUBTITLEDEFAULT_SCALEY;
    subtitle->max_width = CUTSUBTITLEDEFAULT_FITWIDTH;
    subtitle->fade_time = 0.0f;

    while (NuFParGetWord(fp) != 0) {
        if (NuStrICmp(fp->word_buf, "text_id") == 0) {
            subtitle->text_id = static_cast<i16>(NuFParGetInt(fp));
        } else if (NuStrICmp(fp->word_buf, "colour") == 0) {
            subtitle->red = static_cast<u8>(NuFParGetFloat(fp) * 255.0f);
            subtitle->green = static_cast<u8>(NuFParGetFloat(fp) * 255.0f);
            subtitle->blue = static_cast<u8>(NuFParGetFloat(fp) * 255.0f);
            subtitle->alpha = static_cast<u8>(NuFParGetFloat(fp) * 128.0f);
        } else if (NuStrICmp(fp->word_buf, "r") == 0) {
            subtitle->red = static_cast<u8>(NuFParGetFloat(fp) * 255.0f);
        } else if (NuStrICmp(fp->word_buf, "g") == 0) {
            subtitle->green = static_cast<u8>(NuFParGetFloat(fp) * 255.0f);
        } else if (NuStrICmp(fp->word_buf, "b") == 0) {
            subtitle->blue = static_cast<u8>(NuFParGetFloat(fp) * 255.0f);
        } else if (NuStrICmp(fp->word_buf, "a") == 0) {
            subtitle->alpha = static_cast<u8>(NuFParGetFloat(fp) * 128.0f);
        } else if (NuStrICmp(fp->word_buf, "align") == 0) {
            if (NuStrICmp(fp->word_buf, "centre") == 0) {
                subtitle->alignment = 0;
            } else if (NuStrICmp(fp->word_buf, "top") == 0) {
                subtitle->alignment = 1;
            } else if (NuStrICmp(fp->word_buf, "top_right") == 0) {
                subtitle->alignment = 9;
            } else if (NuStrICmp(fp->word_buf, "right") == 0) {
                subtitle->alignment = 8;
            } else if (NuStrICmp(fp->word_buf, "bottom_right") == 0) {
                subtitle->alignment = 12;
            } else if (NuStrICmp(fp->word_buf, "bottom") == 0) {
                subtitle->alignment = 4;
            } else if (NuStrICmp(fp->word_buf, "bottom_left") == 0) {
                subtitle->alignment = 6;
            } else if (NuStrICmp(fp->word_buf, "left") == 0) {
                subtitle->alignment = 2;
            } else if (NuStrICmp(fp->word_buf, "top_left") == 0) {
                subtitle->alignment = 3;
            }
        } else if (NuStrICmp(fp->word_buf, "start_time") == 0) {
            subtitle->start_frame = NuFabs(NuFParGetFloat(fp));
        } else if (NuStrICmp(fp->word_buf, "end_time") == 0) {
            subtitle->end_frame = NuFabs(NuFParGetFloat(fp));
        } else if (NuStrICmp(fp->word_buf, "pos") == 0) {
            subtitle->x = NuFParGetFloat(fp);
            subtitle->y = NuFParGetFloat(fp);
        } else if (NuStrICmp(fp->word_buf, "x") == 0) {
            subtitle->x = NuFParGetFloat(fp);
        } else if (NuStrICmp(fp->word_buf, "y") == 0) {
            subtitle->y = NuFParGetFloat(fp);
        }

        if (NuStrICmp(fp->word_buf, "scale") == 0) {
            subtitle->x_scale = NuFParGetFloat(fp);
            subtitle->y_scale = subtitle->x_scale;
        } else if (NuStrICmp(fp->word_buf, "x_scale") == 0) {
            subtitle->x_scale = NuFParGetFloat(fp);
        } else if (NuStrICmp(fp->word_buf, "y_scale") == 0) {
            subtitle->y_scale = NuFParGetFloat(fp);
        } else if (NuStrICmp(fp->word_buf, "fit_width") == 0) {
            subtitle->max_width = NuFabs(NuFParGetFloat(fp));
        } else if (NuStrICmp(fp->word_buf, "fade_time") == 0) {
            subtitle->fade_time = NuFabs(NuFParGetFloat(fp));
        }
    }

    i16 text_id = subtitle->text_id;
    if (text_id >= 0 && text_id < Text_GetMaxOverallStrings() && TTab[text_id] != NULL &&
        subtitle->start_frame < subtitle->end_frame) {
        if (subtitle->fade_time > 0.0f && subtitle->end_frame - subtitle->start_frame < subtitle->fade_time * 2.0f) {
            subtitle->fade_time = 0.0f;
        }
        CS_buffptr->addr += sizeof(CUTSCENESUBTITLE);
        ++CS_CutInfo->subtitle_count;
    }
}

static void CS_lowend_disthack(NUFPAR *fp) {
    if (NuFParGetWord(fp) != 0) {
        f32 distance = NuAToF(fp->word_buf);
        if (distance > 0.0f) {
            CS_CutInfo->low_end_distance = distance;
        }
    }
}

static void CS_draw_gizmo_sys(NUFPAR *fp) {
    if (NuFParGetWord(fp) == 0) {
        return;
    }
    if (NuStrICmp(fp->word_buf, "on") == 0) {
        CS_CutInfo->flags |= 0x8000;
    } else if (NuStrICmp(fp->word_buf, "off") == 0) {
        CS_CutInfo->flags &= ~0x8000U;
    }
}

static void CS_deb_page(NUFPAR *fp) {
    if (NuFParGetWord(fp) == 0) {
        return;
    }
    if (NuStrICmp(fp->word_buf, "level") == 0) {
        CS_CutInfo->flags |= 8;
    } else {
        CS_CutInfo->flags &= ~8U;
    }
}

static void CS_draw_world(NUFPAR *fp) {
    if (NuFParGetWord(fp) == 0) {
        return;
    }
    if (NuStrICmp(fp->word_buf, "on") == 0) {
        CS_CutInfo->flags |= 2;
    } else if (NuStrICmp(fp->word_buf, "off") == 0) {
        CS_CutInfo->flags &= ~2U;
    }
}

static void CS_next_cut_scene(NUFPAR *fp) {
    if (NuFParGetWord(fp) != 0 && NuStrLen(fp->word_buf) <= 0x3f) {
        NuStrCpy(CS_CutInfo->next_cutscene, fp->word_buf);
        CS_CutInfo->linked_audio = 1;
    }
}

static void CS_go_through_door(NUFPAR *fp) {
    if (NuFParGetWord(fp) != 0 && NuStrLen(fp->word_buf) <= 0xf) {
        NuStrCpy(CS_CutInfo->door_name, fp->word_buf);
    }
}

static void CutScene_RigidPostRender(NUGCUTRIGID_s *rigid, instNUGCUTRIGID_s *instance, NUMTX *matrix) {
    if (Reflections_On == 0 || (rigid->flags & 0x10) == 0) {
        return;
    }

    NUVEC *position = reinterpret_cast<NUVEC *>(&matrix->m30);
    if (!(NuVecDistSqr(position, reinterpret_cast<NUVEC *>(&GameCam->render_mtx.m30), NULL) < CutReflectRange2)) {
        return;
    }

    f32 ground = GameShadow(NULL, position, 5.0f, -1);
    if (ground == 2000000.0f) {
        return;
    }
    i32 surface = ShadowInfo();
    if (!(static_cast<u32>(surface) > 0x20) && (TerSurface[surface].flags & 2) != 0) {

        WORLDINFO_s *world = WorldInfo_CurrentlyActive();
        NUMTX reflection = *matrix;
        reflection.m01 = -reflection.m01;
        reflection.m11 = -reflection.m11;
        reflection.m21 = -reflection.m21;
        reflection.m31 = ground - (reflection.m31 - ground);

        nuhspecial_s *special = reinterpret_cast<nuhspecial_s *>(instance);
        i32 object_index = LevelObject_FindIndexFromName_RefOnly(NuSpecialGetName(special));
        if (object_index != -1) {
            i32 reflection_index = LevelObject_GetReflection(object_index);
            if (reflection_index != -1) {
                nuhspecial_s *reflection_special = &world->lev_objs[reflection_index].special;
                if (reflection_special != NULL) {
                    special = reflection_special;
                }
            }
        }

        NuRndrStartReflectionRender(0);
        NuSpecialDrawAt(special, &reflection);
        NuRndrEndReflectionRender();
    }
}

static void CS_tex_anim(NUFPAR *fp) {
    if (CS_texanimcount > 3) {
        return;
    }
    // Retail re-reads the cut info and slot for each store.
    if ((CS_CutInfo->texture_animations[CS_texanimcount].index = NuFParGetInt(fp)) == -1) {
        return;
    }
    if ((CS_CutInfo->texture_animations[CS_texanimcount].frame = static_cast<f32>(NuFParGetInt(fp))) >= 0.0f) {
        ++CS_texanimcount;
    }
}

static void CS_fadefog(NUFPAR *fp) {
    if (CS_fadefogcount > 1) {
        return;
    }
    f32 value;
    CS_CutInfo->fade_fog[CS_fadefogcount].frame = value = NuFParGetFloat(fp);
    if (value >= 0.0f) {
        CS_CutInfo->fade_fog[CS_fadefogcount].near_distance = value = NuFParGetFloat(fp);
        if (value >= 0.0f) {
            CS_CutInfo->fade_fog[CS_fadefogcount].far_distance = value = NuFParGetFloat(fp);
            if (value >= 0.0f) {
                CS_CutInfo->fade_fog[CS_fadefogcount].value = value = NuFParGetFloat(fp);
                if (value >= 0.0f) {
                    ++CS_fadefogcount;
                }
            }
        }
    }
}

static void CS_cutsceneplayerobj(NUFPAR *fp) {
    if (CS_CutInfo->state_count > 0x1f || NuFParGetWord(fp) == 0) {
        return;
    }

    if (NuSpecialFind(CS_worldinfo->current_gscn, &CS_CutInfo->state_entries[CS_CutInfo->state_count].special,
                      fp->word_buf, 1) == 0) {
        return;
    }
    // Every flag update re-indexes the entry: the u8 store may alias state_count.
#define CS_STATE_FLAGS CS_CutInfo->state_entries[CS_CutInfo->state_count].flags
    CS_STATE_FLAGS &= ~CLIP_OBJECT_SHOW;
    CS_STATE_FLAGS &= ~CLIP_OBJECT_HIDE;
    while (NuFParGetWord(fp) != 0) {
        if (NuStrICmp(fp->word_buf, "on") == 0) {
            CS_STATE_FLAGS |= CLIP_OBJECT_SHOW;
            CS_STATE_FLAGS &= ~CLIP_OBJECT_HIDE;
        } else if (NuStrICmp(fp->word_buf, "off") == 0) {
            CS_STATE_FLAGS &= ~CLIP_OBJECT_SHOW;
            CS_STATE_FLAGS |= CLIP_OBJECT_HIDE;
        } else if (NuStrICmp(fp->word_buf, "end_anim") == 0 || NuStrICmp(fp->word_buf, "endanim") == 0 ||
                   NuStrICmp(fp->word_buf, "anim_end") == 0 || NuStrICmp(fp->word_buf, "animend") == 0) {
            CS_STATE_FLAGS &= ~CLIP_OBJECT_SHOW;
            CS_STATE_FLAGS &= ~CLIP_OBJECT_HIDE;
            CS_STATE_FLAGS |= CLIP_OBJECT_ANIM_END;
        }
    }
#undef CS_STATE_FLAGS
    ++CS_CutInfo->state_count;
}

static void CS_goto_level(NUFPAR *fp) {
    if (NuFParGetWord(fp) == 0) {
        return;
    }
    // Retail realigns the frame (and esp,-16) and keeps both out-parameters in 16-byte aligned slots.
    i32 level_index __attribute__((aligned(16)));
    Level_FindByName(fp->word_buf, &level_index);
    if (level_index != -1 && &LDataList[level_index] == HUB_LDATA && CS_area != -1) {
        i32 status_index __attribute__((aligned(16)));
        Area_FindStatusLevel(&ADataList[CS_area], &status_index);
        if (status_index != -1) {
            level_index = status_index;
        }
    }
    CS_CutInfo->skip_level = static_cast<i16>(level_index);
}

static void CS_play_sfx(NUFPAR *fp) {
    if (NuFParGetWord(fp) == 0) {
        return;
    }

    i32 sfx_id = GetSfxId(fp->word_buf);
    if (sfx_id == -1) {
        return;
    }

    i32 slot;
    if (CS_CutInfo->sfx[0].id == -1) {
        slot = 0;
    } else if (CS_CutInfo->sfx[1].id == -1) {
        slot = 1;
    } else if (CS_CutInfo->sfx[2].id == -1) {
        slot = 2;
    } else if (CS_CutInfo->sfx[3].id == -1) {
        slot = 3;
    } else if (CS_CutInfo->sfx[4].id == -1) {
        slot = 4;
    } else if (CS_CutInfo->sfx[5].id == -1) {
        slot = 5;
    } else {
        return;
    }

    CS_CutInfo->sfx[slot].id = static_cast<i16>(sfx_id);
    CS_CutInfo->sfx[slot].flags &= ~1U;
    while (NuFParGetWord(fp) != 0) {
        if (NuStrICmp(fp->word_buf, "frame") == 0) {
            CS_CutInfo->sfx[slot].frame = NuFParGetFloat(fp);
            if (CS_CutInfo->sfx[slot].frame < 1.0f) {
                CS_CutInfo->sfx[slot].frame = 1.0f;
            }
        } else if (NuStrICmp(fp->word_buf, "pos") == 0) {
            if (NuFParGetFloat(fp) == 0.0f) {
                continue;
            }
            CS_CutInfo->sfx[slot].position.x = NuAToF(fp->word_buf);
            if (NuFParGetFloat(fp) == 0.0f) {
                continue;
            }
            CS_CutInfo->sfx[slot].position.y = NuAToF(fp->word_buf);
            if (NuFParGetFloat(fp) == 0.0f) {
                continue;
            }
            CS_CutInfo->sfx[slot].position.z = NuAToF(fp->word_buf);
            CS_CutInfo->sfx[slot].flags |= 1;
        }
    }
}

static void CS_sfx(NUFPAR *fp) {
    if (NuFParGetWord(fp) != 0) {
        CS_CutInfo->legacy_music_index = GetMusicIndex(fp->word_buf, MusicInfo, -1);
        CS_CutInfo->music_handle = music_man.GetTrackHandle(TRACK_CLASS_CUTSCENE, fp->word_buf);
    }
}

static void CutScene_FindCharacters(NUGCUTSCENE_s *cutscene) {
    NUGCUTCHARSYS_s *system = cutscene->character_system;
    for (i32 i = 0; i < system->character_count; ++i) {
        NUGCUTCHAR_s *character = &system->characters[i];
        CUTSYS *cutscene_system = CS_cutsys;
        character->flags |= 2;
        NUGCUTLOCATORSYS_s *locator_system = cutscene->locator_system;
        char *name = character->name;

        i32 model_index;
        for (model_index = 0; model_index < apicharsys->loaded_model_count; ++model_index) {
            i32 character_id = apicharsys->models[model_index].model_id;
            if (NuStrICmp(name, CDataList[character_id].file) == 0) {
                CHARACTERMODEL_s *model = &apicharsys->models[model_index];
                character_id = model->model_id;
                character->character_model = model;
                if (character_id != -1) {
                    cutscene_system->character_bits[character_id / 32] |= 1U << (character_id & 0x1f);
                }
                goto resolve_locators;
            }
        }
        character->character_model = NULL;

    resolve_locators:
        if (character->has_locator != 0 && reinterpret_cast<isize>(character->locator) <= 0xfe) {
            isize locator_index = reinterpret_cast<isize>(character->locator);
            character->locator_index = static_cast<u8>(reinterpret_cast<usize>(character->locator));
            character->locator = &locator_system->locators[locator_index];
        } else {
            character->locator_index = 0xff;
        }
    }
}

void CutScenes_ConfigureList(char *filename, variptr_u *buffer, variptr_u) {
    CutList = NULL;
    CUTCOUNT = 0;
    NUFPAR *parser = NuFParCreate(filename);
    if (parser == NULL) {
        return;
    }

    char *cursor = reinterpret_cast<char *>(ALIGN(buffer->addr, 4));
    buffer->char_ptr = cursor;
    CutSceneListEntry entries[1000];
    char directory[128];
    char file[128];

    while (CUTCOUNT < 1000 && NuFParGetLine(parser) != 0) {
        if (__builtin_expect(NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "cutscene_start") == 0, 0)) {

            directory[0] = '\0';
            file[0] = '\0';
            entries[CUTCOUNT].level = -1;
            while (NuFParGetLine(parser) != 0 && NuFParGetWord(parser) != 0) {
                if (__builtin_expect(NuStrICmp(parser->word_buf, "cutscene_end") == 0, 1)) {
                    if (__builtin_expect(directory[0] == '\0', 0)) {
                        break;
                    }
                    if (file[0] == '\0') {
                        break;
                    }
                    if (entries[CUTCOUNT].level == -1) {
                        break;
                    }

                    i32 duplicate = 0;
                    for (i32 i = 0; i < CUTCOUNT; ++i) {
                        if (__builtin_expect(duplicate != 0, 0)) {
                            break;
                        }
                        if (NuStrICmp(directory, entries[i].directory) == 0 &&
                            NuStrICmp(file, entries[i].filename) == 0 && entries[CUTCOUNT].level == entries[i].level) {
                            duplicate = 1;
                        }
                    }
                    if (duplicate == 0) {
                        entries[CUTCOUNT].directory = cursor;
                        NuStrCpy(cursor, directory);
                        i32 directory_length = NuStrLen(directory);
                        cursor[directory_length] = '\0';
                        cursor += directory_length + 1;
                        entries[CUTCOUNT].filename = cursor;
                        NuStrCpy(cursor, file);
                        i32 file_length = NuStrLen(file);
                        cursor[file_length] = '\0';
                        cursor += file_length + 1;
                        ++CUTCOUNT;
                    }
                    break;
                }
                if (NuStrICmp(parser->word_buf, "dir") == 0 || NuStrICmp(parser->word_buf, "path") == 0) {
                    if (NuFParGetWord(parser) != 0 && NuStrLen(parser->word_buf) <= 127) {
                        NuStrCpy(directory, parser->word_buf);
                    }
                } else if (NuStrICmp(parser->word_buf, "file") == 0 || NuStrICmp(parser->word_buf, "filename") == 0) {
                    if (NuFParGetWord(parser) != 0 && NuStrLen(parser->word_buf) <= 63) {
                        NuStrCpy(file, parser->word_buf);
                    }
                } else if (NuStrICmp(parser->word_buf, "level") == 0 || NuStrICmp(parser->word_buf, "in_level") == 0) {
                    if (NuFParGetWord(parser) != 0) {
                        i32 level;
                        Level_FindByName(parser->word_buf, &level);
                        entries[CUTCOUNT].level = static_cast<i16>(level);
                    }
                }
            }
        }
    }

    NuFParDestroy(parser);
    if (__builtin_expect(CUTCOUNT > 0, 1)) {
        buffer->addr = ALIGN(reinterpret_cast<usize>(cursor), 4);
        CutList = reinterpret_cast<CutSceneListEntry *>(buffer->void_ptr);
        usize list_bytes = CUTCOUNT * sizeof(CutSceneListEntry);
        memmove(buffer->void_ptr, entries, list_bytes);
        buffer->addr += list_bytes;
    }
}

__attribute__((optimize("O2", "tree-vectorize"))) void *CutScenes_Load(char *config, NUGSCN *gscn1, NUGSCN *gscn2, i32 param1, VARIPTR *buf, VARIPTR *buf_end, i32 param2,
                     i32 param3, WORLDINFO *world) {
    NUFPAR *fp;
    CUTSYS *sys;
    void *initial;
    CUTINFO *cut;
    // Retail realigns the frame (and esp,-16) and keeps the name buffer in a 16-byte aligned slot.
    char name[128] __attribute__((aligned(16)));
    CUTINFO *entries[32];
    char full_path[128];
    char config_name[128];

    if (CutList != NULL && CUTCOUNT > 0) {
        return CutScenes_LoadFromList(gscn1, gscn2, param1, buf, buf_end, param2, param3, world);
    }
    {
        if (config == NULL) {
            return NULL;
        }

        fp = NuFParCreateMem((char *)"cutscenes", config, 0xffff);
        if (fp == NULL) {
            return NULL;
        }

        CS_area = param2;
        initial = buf->void_ptr;
        sys = (CUTSYS *)ALIGN((usize)initial, 4);
        buf->void_ptr = sys + 1;
        CS_cutsys = sys;
        CS_worldinfo = world;
        sys->character_bits = reinterpret_cast<u32 *>(buf->void_ptr);
        buf->void_ptr = reinterpret_cast<char *>(buf->void_ptr) + ((CHARCOUNT + 0x1f) / 32) * 4;

        while (NuFParGetLine(fp) != 0) {
            if (NuFParGetWord(fp) == 0 || NuStrICmp(fp->word_buf, "cutscene") != 0 || sys->count > 0x1f ||
                NuFParGetWord(fp) == 0) {
                continue;
            }

            cut = (CUTINFO *)buf->void_ptr;
            entries[sys->count] = cut;
            buf->addr = ALIGN(buf->addr + sizeof(*cut), 4);
            NuStrLwr(name, fp->word_buf);
            i32 dot = NuStrLen(name) - 1;
            while (dot > 0 && name[dot] != '.') {
                --dot;
            }
            if (name[dot] == '.') {
                name[dot] = '\0';
            }

            if (name[0] == 'c' && name[1] == 'u' && name[2] == 't' && name[3] == '\\') {
                NuStrCpy(name, name + 4);
            }
            NuStrCpy(config_name, name);
            if (CutScene_OverrideConfigFileNameFn != NULL) {
                CutScene_OverrideConfigFileNameFn(config_name, param2, param3);
            }
            NuStrCpy(full_path, "cut\\");
            NuStrCat(full_path, config_name);
            NuStrCat(full_path, ".txt");
            CutScene_Configure(cut, full_path, buf, buf_end);
            NuStrCpy(full_path, "cut\\");
            NuStrCat(full_path, name);
            NuStrCat(full_path, ".txt");

            if ((cut->flags & 0x800) == 0 && InStory() == 0 &&
                (LDataList[param3].flags & (LEVEL_INTRO | LEVEL_MIDTRO | LEVEL_OUTRO)) == 0) {
                buf->void_ptr = cut;
                continue;
            }
            buf->void_ptr = (char *)ALIGN(buf->addr, 0x40);
            NuStrCpy(full_path, "cut\\");
            NuStrCat(full_path, name);
            NuStrCat(full_path, ".cu2");
            cut->scene = NuGCutSceneLoad(full_path, buf, buf_end, 0);
            if (cut->scene == NULL) {
                NuStrCpy(full_path, "cut\\");
                NuStrCat(full_path, name);
                NuStrCat(full_path, ".cut");
                cut->scene = NuGCutSceneLoad(full_path, buf, buf_end, 0);
                if (cut->scene == NULL) {
                    buf->void_ptr = cut;
                    continue;
                }
            }
            i32 separator = NuStrLen(name) - 1;
            while (separator >= 0 && name[separator] != '\\') {
                --separator;
            }
            NuStrCpy(cut->name, name + separator + 1);
            NuGCutSceneFixUp(reinterpret_cast<NUGCUTSCENE_s *>(cut->scene), gscn1, 0, param1);
            NuGCutSceneFixUpExtra(reinterpret_cast<NUGCUTSCENE_s *>(cut->scene), gscn2);
            cut->instance =
                instNuGCutSceneCreate(reinterpret_cast<NUGCUTSCENE_s *>(cut->scene), NULL, NULL, name, buf, 0);
            if (cut->instance != NULL) {
                reinterpret_cast<instNUGCUTSCENE_s *>(cut->instance)->rate = cut->frames_per_second * DEFAULTFRAMETIME;
                buf->void_ptr = (char *)ALIGN(buf->addr, 0x10);
            }
            sys->count++;
        }
        NuFParDestroy(fp);
        if (sys->count <= 0) {
            buf->void_ptr = initial;
            return NULL;
        }
        sys->cuts = reinterpret_cast<CUTINFO **>(buf->void_ptr);
        const usize table_size = sys->count * sizeof(CUTINFO *);
        memmove(sys->cuts, entries, table_size);
        // Retail reserves count pointer-table rows after copying the count entries.
        buf->addr = ALIGN(buf->addr + table_size * sys->count, 4);
        return sys;
    }
}

void *CutScene_FindInst(CUTSYS *system, char *name) {
    if (system != NULL) {
        for (i32 i = 0; i < system->count; ++i) {
            if (NuStrICmp(system->cuts[i]->name, name) == 0) {
                return system->cuts[i]->instance;
            }
        }
    }
    return NULL;
}

CUTINFO *CutScene_Find(CUTSYS *cutscene_system, char *name) {
    if (name == NULL || cutscene_system == NULL) {
        return NULL;
    }

    for (i32 i = 0; i < cutscene_system->count; ++i) {
        if (NuStrICmp(cutscene_system->cuts[i]->name, name) == 0) {
            return cutscene_system->cuts[i];
        }
    }
    return NULL;
}

CUTINFO *NewCutScene(CUTINFO *cut, CUTSYS *system, char *name, i32) {
    if (cut == NULL) {
        if (name != NULL && system != NULL)
            cut = CutScene_Find(system, name);
        if (cut == NULL)
            return NULL;
    }
    if (NewCutInfoCount < 8) {
        NewCutInfo[NewCutInfoCount++] = cut;
        return cut;
    }
    return NULL;
}

i32 CutScene_PlayingOrRequested(CUTINFO *cut) {
    if (cut == NULL) {
        if (CutStopInfo != NULL || NewCutInfoCount != 0)
            return 1;
        return 0;
    }
    if (cut == CutStopInfo)
        return 1;
    for (i32 i = 0; i < NewCutInfoCount; ++i) {
        if (NewCutInfo[i] == cut)
            return 1;
    }
    return 0;
}

i32 CutScene_HasPlayed(CUTINFO *cut) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    CUTSYS *cutscene_system = world->cutscene_sys;
    if (cut == NULL || cutscene_system == NULL || cutscene_system->count <= 0) {
        return 0;
    }

    i32 cutscene_index = -1;
    for (i32 index = 0; index < cutscene_system->count; ++index) {
        if (cutscene_system->cuts[index] == cut) {
            cutscene_index = index;
        }
    }
    i32 has_played = 0;
    if (cutscene_index != -1) {
        world = WorldInfo_CurrentlyActive();
        has_played = (world->level_progress->played_cutscene_mask & (1 << cutscene_index)) != 0;
    }
    return has_played;
}

void CutScene_SnapToEnd(CUTINFO *cut) {
    if (cut != NULL) {
        instNuGCutSceneEnd(static_cast<instNUGCUTSCENE_s *>(cut->instance));
    }
}

void CutScene_StartAudio() {
}

void CutScenes_Stop(CUTSYS *system) {
    if (system == NULL || system->count <= 0) {
        return;
    }
    for (i32 i = 0; i < system->count; ++i) {
        CUTINFO *cut = system->cuts[i];
        if (cut->instance != NULL) {
            instNuGCutSceneStop(static_cast<instNUGCUTSCENE_s *>(cut->instance));
        }
    }
}

void CutScenes_Destroy(CUTSYS *system) {
    if (system == NULL || system->count <= 0) {
        return;
    }
    for (i32 i = 0; i < system->count; ++i) {
        if (system->cuts[i]->instance != NULL) {
            instNuGCutSceneDestroy(static_cast<instNUGCUTSCENE_s *>(system->cuts[i]->instance));
            system->cuts[i]->instance = NULL;
        }
    }
    for (i32 i = 0; i < system->count; ++i) {
        if (system->cuts[i]->scene != NULL) {
            NuGCutSceneDestroy(static_cast<NUGCUTSCENE_s *>(system->cuts[i]->scene));
            system->cuts[i]->scene = NULL;
        }
    }
}

i32 CutScene_IsSkippable(CUTINFO *cut) {
    return FadeSys.fade == 0.0f && cut != NULL && (cut != CutStopInfo || CutSceneWaiting == 0 || cutaudiopaused == 0);
}

void CutScenes_Draw(WORLDINFO_s *world) {
    if (world->cutscene_sys != NULL && ACTIVECUTCOUNT > 0) {
        SetLevelLights(world->rtl_set, 1.0f);

        CUTINFO *cut = static_cast<CUTINFO *>(CutStopInfo);
        if (cut == NULL) {
            CutBlobShadowAlpha = static_cast<u8>(world->current_level->blob_shadow_alpha);
            CutBlobShadowFadeNear = static_cast<f32>(static_cast<u8>(world->current_level->blob_shadow_fade_near));
            CutBlobShadowFadeFar = static_cast<f32>(static_cast<u8>(world->current_level->blob_shadow_fade_far));
            CutReflectRange = static_cast<f32>(static_cast<u8>(world->current_level->reflection_range));
        } else {
            CutBlobShadowAlpha = cut->blob_shadow_alpha;
            CutBlobShadowFadeNear = static_cast<f32>(cut->blob_shadow_fade_near);
            CutBlobShadowFadeFar = static_cast<f32>(cut->blob_shadow_fade_far);
            CutReflectRange = static_cast<f32>(cut->reflection_range);
        }
        CutReflectRange2 = CutReflectRange * CutReflectRange;
        NuGCutSceneSysRender(Paused);

        if (CUTDRAWWORLD != 0 && world->current_gscn != NULL) {
            SetLevelLights(world->rtl_set, 1.0f);
            NuGScnRndr3(world->current_gscn);
            if (world->gizmo_sys != NULL && CutStopInfo != NULL &&
                (static_cast<CUTINFO *>(CutStopInfo)->flags & 0x8000) != 0) {
                GizmoSysDraw(world->gizmo_sys, world, FRAMETIME);
            }
        }
    }
}

void CutScenes_BGLoadManager() {
    if (cutscene_load_instance != NULL && stream_cut_issued == 0) {
        stream_cut_issued = 1;
        bgPostRequest(bgLoadStreamCutScene, bgAckStreamCutScene, NULL, 0);
    }
    NuGCutSceneSysPostBackgroundLoad();
}

void CutScenes_InitSystem(CUTSCENESYS *system) {
    CutSceneSys = system;
    NuGCutSceneSysInit(cutscene_locatorfns);
    NuSetCutSceneCharacterRenderFn(CutScene_DrawCharacter);
    NuSetCutSceneFindCharactersFn(CutScene_FindCharacters);
    NuSetCutSceneCharacterCreateDataFn(CutScene_CreateCharacterInstance);
    NuSetCutSceneResetCharactersFn(CutScene_ResetCharacters);
    NuSetCutSceneCharacterEvalFn(CutScene_EvalCharacter);
    NuSetCutSceneRigidPostRenderFn(CutScene_RigidPostRender);
}

void CutScene_DrawSubtitles() {
    if (CutStopInfo == NULL || CUT_STOP->subtitle_data == NULL) {
        return;
    }

    CUTSCENESUBTITLE *subtitle = CUT_STOP->subtitle_data;
    for (i32 i = 0; i < CUT_STOP->subtitle_count; ++i, ++subtitle) {
        f32 frame = CUT_STOP->field_58;
        if (frame >= subtitle->start_frame && subtitle->end_frame >= frame) {
            f32 alpha = 1.0f;
            if (subtitle->fade_time > 0.0f) {
                f32 fade_end = subtitle->start_frame + subtitle->fade_time;
                if (fade_end > frame) {
                    alpha = (frame - subtitle->start_frame) / (fade_end - subtitle->start_frame);
                } else {
                    f32 fade_start = subtitle->end_frame - subtitle->fade_time;
                    if (frame > fade_start) {
                        alpha = 1.0f - (frame - fade_start) / (subtitle->end_frame - fade_start);
                    }
                }
            }

            SmartTextEx(TTab[subtitle->text_id], subtitle->x, subtitle->y, 1.0f, subtitle->x_scale, subtitle->y_scale,
                        1.0f, subtitle->alignment, subtitle->red, subtitle->green, subtitle->blue, subtitle->max_width,
                        1, NULL, 0, static_cast<i32>(static_cast<f32>(static_cast<i32>(subtitle->alpha)) * alpha));
        }
    }
}

void CutScenePlayer_Configure(char *file, VARIPTR *buffer, VARIPTR *, i16 *clip_text, i16 *intro, i16 *midtro,
                              i16 *outro, i16 *ending) {
    NUFPAR *fp = NuFParCreate(file);
    if (fp == NULL)
        return;
    CUTSCENEPLAYER_s player;
    buffer->addr = (buffer->addr + 3) & ~3U;
    player.clips = reinterpret_cast<CUTSCENEPLAYERCLIP *>(buffer->addr);
    player.active = NULL;
    player.clip_count = 0;
    player.clip_text = clip_text;
    player.intro_text = intro;
    player.midtro_text = midtro;
    player.outro_text = outro;
    player.ending_text = ending;
    while (NuFParGetLine(fp)) {
        if (NuFParGetWord(fp) && !(NuStrICmp(fp->word_buf, "clip"))) {
            CUTSCENEPLAYERCLIP *clip = &player.clips[player.clip_count];
            clip->level_id = -1;
            clip->type = CLIP_INTRO;
            clip->name[0] = 0;
            clip->guest_episode = -1;
            while (NuFParGetWord(fp)) {
                if (NuStrICmp(fp->word_buf, "in_level") == 0) {
                    i32 level;
                    if (NuFParGetWord(fp) && Level_FindByName(fp->word_buf, &level))
                        clip->level_id = level;
                } else if (NuStrICmp(fp->word_buf, "name") == 0) {
                    if (NuFParGetWord(fp) && NuStrLen(fp->word_buf) <= 63)
                        NuStrCpy(clip->name, fp->word_buf);
                } else if (NuStrICmp(fp->word_buf, "intro") == 0)
                    clip->type = CLIP_INTRO;
                else if (NuStrICmp(fp->word_buf, "midtro") == 0)
                    clip->type = CLIP_MIDTRO;
                else if (NuStrICmp(fp->word_buf, "outro") == 0)
                    clip->type = CLIP_OUTRO;
                else if (NuStrICmp(fp->word_buf, "ending") == 0)
                    clip->type = CLIP_ENDING;
                else if (NuStrICmp(fp->word_buf, "guest_in_episode") == 0 && NuFParGetWord(fp))
                    clip->guest_episode = NuAToI(fp->word_buf);
            }
            if (clip->level_id != -1 && (CutScenePlayer_AcceptFn == NULL || CutScenePlayer_AcceptFn(clip)))
                ++player.clip_count;
        }
    }
    NuFParDestroy(fp);
    if (player.clip_count == 0) {
        return;
    }
    buffer->void_ptr = &player.clips[player.clip_count];
    memmove(buffer->void_ptr, &player, sizeof(player));
    CutScenePlayer = reinterpret_cast<CUTSCENEPLAYER_s *>(buffer->void_ptr);
    buffer->addr += sizeof(player);
}

i32 CutScenePlayer_CountEpisodeClips(i32 episode, i32 include_guests, i16 *ids) {
    i32 count = 0;
    CUTSCENEPLAYER_s *player = CutScenePlayer;
    if (player != NULL) {
        i32 clip_count = player->clip_count;
        if (__builtin_expect(clip_count != 0, 1)) {
            LEVELDATA *levels = LDataList;
            CUTSCENEPLAYERCLIP *clip = player->clips;
            for (i32 i = 0; i < clip_count; ++i, ++clip) {
                if (__builtin_expect(levels[clip->level_id].episode_index == episode ||
                    (include_guests != 0 && clip->guest_episode == episode), 1)) {
                    if (ids != NULL) {
                        ids[count++] = i;
                        clip_count = player->clip_count;
                    } else {
                        ++count;
                    }
                }
            }
        }
    }
    return count;
}

void CutScenePlayer_Reset() {
    if (CutScenePlayer != NULL) {
        CutScenePlayer->active = 0;
    }
}

void *CutScenePlayer_Available(void) {
    return CutScenePlayer;
}

CUTSCENEPLAYERCLIP *CutScenePlayer_Active() {
    return CutScenePlayer != NULL ? CutScenePlayer->active : 0;
}

void CutScenes_End() {
    if (CutScenePlayer_Active() != 0 && NewLData != NULL && NewLData == HUB_LDATA) {
        return;
    }
    for (i32 i = 0; i < CutInstEndCount; ++i) {
        instNuGCutSceneEnd(CutInstEnd[i]);
        if (i == CutInstEndStop) {
            CUTSTOPGAME = 0;
            CutStopInfo = NULL;
            GameCam_Reset(GameCam);
        }
    }
}

i32 CutScenePlayer_CanStart(i32 clip_id) {
    if (CutScenePlayer != NULL && clip_id >= 0 && clip_id < CutScenePlayer->clip_count) {
        const i32 area_id = static_cast<i8>(LDataList[CutScenePlayer->clips[clip_id].level_id].area_index);
        if (area_id != -1) {
            AREADATA_s *area = &ADataList[area_id];
            if ((area->flags & 2) == 0) {
                if (Game_AreaSave != NULL && Game_AreaSave[area_id].area_complete != 0)
                    return 1;
            } else {
                const i32 episode = static_cast<i8>(area->episode_index);
                if (episode != -1 && Episode_IsComplete(&EDataList[episode], NULL))
                    return 2;
            }
        }
    }
    return 0;
}

void CutScenePlayer_Start(i32 clip_id, i32 door) {
    if (CutScenePlayer != NULL && clip_id >= 0 && clip_id < CutScenePlayer->clip_count) {
        NewLevelFromMenu(&LDataList[CutScenePlayer->clips[clip_id].level_id], -1, -1, 1);
        CutScenePlayer->active = &CutScenePlayer->clips[clip_id];
        NextArea_FreePlay = 0;
        FreePlay = 0;
        CutScenePlayer->return_door = door;
    }
}

void CutScenePlayer_GetText(i32 id, char *title, char *subtitle, i32 mode) {
    if (CutScenePlayer == NULL) {
        if (title != NULL)
            NuStrCpy(title, "?");
        if (subtitle != NULL)
            NuStrCpy(subtitle, "?");
        return;
    }
    if (title != NULL) {
        if (mode <= 1) {
            if (CutScenePlayer->clip_text != NULL)
                sprintf(title, TTab[*CutScenePlayer->clip_text], id + 1);
            else
                sprintf(title, "Clip %i", id + 1);
        } else
            title[0] = 0;
        if (mode != 0) {
            char *type_text = NULL;
            switch (CutScenePlayer->clips[id].type) {
                case CLIP_INTRO:
                    if (CutScenePlayer->intro_text)
                        type_text = TTab[*CutScenePlayer->intro_text];
                    break;
                case CLIP_MIDTRO:
                    if (CutScenePlayer->midtro_text)
                        type_text = TTab[*CutScenePlayer->midtro_text];
                    break;
                case CLIP_OUTRO:
                    if (CutScenePlayer->outro_text)
                        type_text = TTab[*CutScenePlayer->outro_text];
                    break;
                case CLIP_ENDING:
                    if (CutScenePlayer->ending_text)
                        type_text = TTab[*CutScenePlayer->ending_text];
                    break;
            }
            if (mode <= 1)
                NuStrCat(title, " - ");
            else {
                title[0] = 0;
                if (mode != 2) {
                    const i32 area = LDataList[CutScenePlayer->clips[id].level_id].area_index;
                    if (area != -1 && !(ADataList[area].flags & 2)) {
                        const i32 chapter = static_cast<i8>(ADataList[area].area_index);
                        if (chapter != -1) {
                            sprintf(title, "%s %i", TTab[tCHAPTER], chapter + 1);
                            NuStrCat(title, " - ");
                        }
                    }
                }
            }
            NuStrCat(title, type_text);
        }
    }
    if (subtitle != NULL) {
        LEVELDATA_s *level = &LDataList[CutScenePlayer->clips[id].level_id];
        if (level->area_index != -1) {
            AREADATA_s *area = &ADataList[level->area_index];
            if (!(area->flags & 2) || level->episode_index != -1) {
                const i32 text_id = !(area->flags & 2) ? area->name_id : EDataList[level->episode_index].text_id;
                NuStrCpy(subtitle, TTab[text_id]);
                return;
            }
        }
        NuStrCpy(subtitle, "?");
    }
}

void CutScenePlayer_SetObjects(CUTINFO *cut) {
    if (CutScenePlayer_Active() != NULL && cut->state_entries != NULL) {
        for (i32 i = 0; i < cut->state_count; ++i) {
            CUTSCENEPLAYEROBJ *entry = &cut->state_entries[i];
            const u8 flags = entry->flags;
            if (flags & CLIP_OBJECT_SHOW)
                NuSpecialSetVisibility(&entry->special, 1);
            else if (flags & CLIP_OBJECT_HIDE)
                NuSpecialSetVisibility(&entry->special, 0);
            else if (flags & CLIP_OBJECT_ANIM_END) {
                f32 end = NuSpecialGetAnimEndFrame(&entry->special);
                nuinstanim_s *anim = NuSpecialGetInstAnim(&cut->state_entries[i].special);
                if (anim != NULL) {
                    anim->ltime = end;
                    anim->playing = 1;
                }
            }
        }
    }
}

void CutScenes_Update(WORLDINFO_s *world, i32 paused) {
    FADETYPE fade_type;
    i32 active_before[32];
    if (CutScenePlayer_Active() != 0 && CutStopInfo != NULL) {
        CutScenePlayer_SetObjects(static_cast<CUTINFO *>(CutStopInfo));
    }

    CutInstEnd[0] = NULL;
    CutInstEndStop = -1;
    CUTSTOPGAME = 0;
    CutInstEndCount = 0;
    CUTCAMONLY = 0;
    CutStopInfo = NULL;
    CUTCAM = 0;
    CUTDRAWWORLD = 0;
    CUTNOFOG = 0;
    CUTSKIPLOCK = 0;

    if (world->cutscene_sys == NULL) {
        return;
    }

    i32 stop_index = -1;
    for (i32 i = 0; i < world->cutscene_sys->count; ++i) {
        CUTINFO *cut = world->cutscene_sys->cuts[i];
        if (cut->instance == NULL) {
            continue;
        }
        instNUGCUTSCENE_s *instance = reinterpret_cast<instNUGCUTSCENE_s *>(cut->instance);
        if ((instance->flags_88 & 2) == 0) {
            continue;
        }
        if ((cut->flags & 1) != 0) {
            CUTDRAWWORLD = cut->flags & 2;
            CUTNOFOG = cut->flags & 4;
            CUTSTOPGAME = 1;
            CutStopInfo = cut;
            stop_index = i;
            DebrisSetRenderGroup(cut->debris_render_group);
            ++CutFrame;
            LevelNameTime = 0.0f;
            LevelNameMul = 0.0f;
            break;
        } else if ((cut->flags & 0x20) != 0) {
            CUTCAMONLY = 1;
        }
    }
    if (stop_index == -1) {
        DebrisSetRenderGroup(1);
    }

    ACTIVECUTCOUNT = 0;
    for (i32 i = 0; i < world->cutscene_sys->count; ++i) {
        CUTINFO *cut = world->cutscene_sys->cuts[i];
        if (cut->instance == NULL) {
            active_before[i] = 0;
            continue;
        }
        instNUGCUTSCENE_s *instance = static_cast<instNUGCUTSCENE_s *>(cut->instance);
        if ((instance->flags_88 & 2) == 0) {
            active_before[i] = 0;
            continue;
        }

        active_before[i] = 1;
        cut->previous_frame = instance->current_frame;
        cut->field_58 += FRAMETIME;
        if (cutaudiopaused != 0 && cut->field_58 >= 3.0f) {
            cutaudiopaused = 0;
        }

        if (i == stop_index || stop_index == -1) {
            if (!(NOSOUND == 0 && NOMUSIC == 0 && i == stop_index && cut->music_handle != -1 &&
                  instance->rate == 0.0f && cutaudiopaused != 0 &&
                  music_man.GetStatus(TRACK_CLASS_CUTSCENE, NULL) != 4)) {
                if (static_cast<instNUGCUTSCENE_s *>(cut->instance)->current_frame == 1.0f && cut->music_handle != -1 &&
                    music_man.GetStatus(TRACK_CLASS_CUTSCENE, NULL) != 4) {
                    music_man.PlayTrack(TRACK_CLASS_CUTSCENE, 0);
                } else if (music_man.GetStatus(TRACK_CLASS_CUTSCENE, NULL) == 4) {
                    if (g_lastCutInfo != cut) {
                        g_accumCutsceneTime += g_lastCutsceneTime;
                        g_lastCutsceneTime =
                            static_cast<NUGCUTSCENE_s *>(cut->scene)->duration / cut->frames_per_second;
                        g_lastCutInfo = cut;
                    }
                    f32 audio_frame = (music_man.GetPlaybackTime(TRACK_CLASS_CUTSCENE) - g_accumCutsceneTime) *
                                      cut->frames_per_second;
                    if (audio_frame < 0.0f) {
                        audio_frame = 0.0f;
                    }
                    f32 rate = audio_frame - (static_cast<instNUGCUTSCENE_s *>(cut->instance)->current_frame - 1.0f);
                    static_cast<instNUGCUTSCENE_s *>(cut->instance)->rate = rate < 0.0f ? 0.0f : rate;
                } else {
                    static_cast<instNUGCUTSCENE_s *>(cut->instance)->rate = cut->frames_per_second * FRAMETIME;
                }

                if (i == stop_index && CutSceneWaiting != 0) {
                    CutSceneWaiting = 0;
                    cutaudiopaused = 0;
                }
            } else {
                static_cast<instNUGCUTSCENE_s *>(cut->instance)->rate = 0.0f;
                CutSceneWaiting = 1;
            }

            instNuGCutScenePause(static_cast<instNUGCUTSCENE_s *>(cut->instance), 0);
            if (CutScene_PreUpdateFn != NULL) {
                CutScene_PreUpdateFn(cut);
            }
            for (i32 j = 0; j < 4; ++j) {
                CUTSCENETEXANIM *animation = &cut->texture_animations[j];
                if (animation->index != -1) {
                    const u32 bit = 1U << animation->index;
                    if ((texanimbits & bit) == 0 &&
                        animation->frame <= static_cast<instNUGCUTSCENE_s *>(cut->instance)->current_frame) {
                        texanimbits |= bit;
                    }
                }
            }
        } else {
            instance->rate = 0.0f;
            instNuGCutScenePause(instance, 1);
        }
        ++ACTIVECUTCOUNT;
    }

    if (g_isLowEndDevice != 0 && ACTIVECUTCOUNT > 0) {
        drawcharactermodel_nobsa = 0;
    }
    NuGCutSceneSysUpdate(paused, 0, 1.0f);
    if (CutScene_PostUpdateFn != NULL) {
        CutScene_PostUpdateFn();
    }

    if (paused == 0) {
        for (i32 i = 0; i < world->cutscene_sys->count; ++i) {
            CUTINFO *cut = world->cutscene_sys->cuts[i];
            if (cut->instance == NULL || active_before[i] == 0) {
                continue;
            }
            instNUGCUTSCENE_s *instance = static_cast<instNUGCUTSCENE_s *>(cut->instance);
            if (instance->rate > 0.0f) {
                for (i32 j = 0; j < 6; ++j) {
                    CUTSCENESFX *sfx = &cut->sfx[j];
                    if (sfx->id != -1 && cut->previous_frame < sfx->frame &&
                        sfx->frame <= static_cast<instNUGCUTSCENE_s *>(cut->instance)->current_frame) {
                        PlaySfxById(sfx->id, (sfx->flags & 1) != 0 ? &sfx->position : NULL);
                    }
                }
            }

            if ((i != stop_index || stop_index == -1) &&
                instNuGCutSceneIsFinished(static_cast<instNUGCUTSCENE_s *>(cut->instance)) != 0) {
                if (CutScene_StoppedFn != NULL) {
                    CutScene_StoppedFn(cut);
                }
                if (CutInstEndCount < 4) {
                    instance = static_cast<instNUGCUTSCENE_s *>(cut->instance);
                    CutInstEnd[CutInstEndCount++] = instance;
                    instance->flags_88 |= 2;
                }
            }
        }
    }
    if (stop_index == -1) {
        return;
    }

    CUTINFO *stop_cut = world->cutscene_sys->cuts[stop_index];
    GameFog_Reset();
    if (instNuGCutSceneIsFinished(static_cast<instNUGCUTSCENE_s *>(stop_cut->instance)) == 0) {
        return;
    }
    if (NewLData == NULL || NewLData == WORLD->current_level || NewLData != HUB_LDATA) {

        fade_type.type = FADE_TYPE_STILL_WIPE;
        FadeSys.SetFade(fade_type, 0);
    }
    if (CutScene_StoppedFn != NULL) {
        CutScene_StoppedFn(stop_cut);
    }
    for (i32 i = 0; i < 4; ++i) {
        if (stop_cut->texture_animations[i].index != -1) {
            texanimbits &= ~(1U << stop_cut->texture_animations[i].index);
        }
    }

    if (stop_cut->skip_level != -1) {
        NewLData = &LDataList[stop_cut->skip_level];
        if (HUB_LDATA != NULL && NewLData == HUB_LDATA) {
            if (CutScenePlayer_Active() != 0) {
                NewLevelFromMenu(HUB_LDATA, -1, -1, 1);
                hub_from_cutsceneplayer = 1;
            } else if (WORLD->area != NULL && (WORLD->area->flags & 2) != 0) {
                if (SuperStory == 0) {
                    NewLData = CREDITS_LDATA;
                } else {
                    NewLData = STATUS_LDATA;
                }
            }
        } else if (CutScenePlayer_Active() != 0 &&
                   ((NewLData->flags & (LEVEL_OUTRO | LEVEL_MIDTRO | LEVEL_INTRO)) == 0 &&
                    (stop_cut->end_flags & 2) == 0)) {
            NewLevelFromMenu(HUB_LDATA, -1, -1, 1);
            hub_from_cutsceneplayer = 1;
        }
        if (waiting_for_level != -1) {
            cut_waiting_for_new_level = 1;
            PauseGameCut();
            music_man.PauseTrack(TRACK_CLASS_CUTSCENE);
            newlevel_resumecutaudio = 1;
        }
    } else {
        CUTINFO *next = NewCutScene(NULL, world->cutscene_sys, stop_cut->next_cutscene, 0);
        if (CutScenePlayer_Active() != 0 && next == NULL) {
            NewLevelFromMenu(HUB_LDATA, -1, -1, 1);
            hub_from_cutsceneplayer = 1;
        } else if ((stop_cut->flags & 0x400) != 0) {
            NewMode = 1;
        } else if (stop_cut->door_name[0] != '\0') {
            DOOR_s *door = Door_FindByName(world, stop_cut->door_name);
            if (door != NULL) {
                Door_GoThrough(world, door, 1);
            }
        } else if ((stop_cut->flags & 0x100) != 0) {
            FADETYPE fade_type;
            fade_type.type = FADE_TYPE_STILL_WIPE;
            FadeSys.SetFade(fade_type, 0);
            NeedScreenGrab(1);
            FadeSys.SetStage(1);
            GameAudio_PlaySfx(0x2d, NULL, 0, 0);
        }
    }
    CUTNOFOG = 0;

    if (CutInstEndCount < 4) {
        instNUGCUTSCENE_s *stop_instance = static_cast<instNUGCUTSCENE_s *>(stop_cut->instance);
        CutInstEndStop = CutInstEndCount;
        CutInstEnd[CutInstEndCount++] = stop_instance;
        stop_instance->flags_88 |= 2;
    }
}

static i32 CutScene_StartBody(WORLDINFO_s *world, CUTINFO *cut) {
    if (cut->music_handle != -1) {
        g_lastCutsceneTime = 0.0f;
        g_accumCutsceneTime = 0.0f;
        g_lastCutInfo = NULL;
    }
    instNuGCutSceneReset(static_cast<instNUGCUTSCENE_s *>(cut->instance));
    if (CutScene_StartFn != NULL) {
        CutScene_StartFn(cut);
    }
    if (CutScenePlayer_Active() != 0) {
        CutScenePlayer_SetObjects(cut);
    }
    static_cast<instNUGCUTSCENE_s *>(cut->instance)->rate = cut->frames_per_second * FRAMETIME;
    instNuGCutSceneStart(static_cast<instNUGCUTSCENE_s *>(cut->instance));
    if ((cut->flags & 0x200) != 0) {
        static_cast<instNUGCUTSCENE_s *>(cut->instance)->flags_88 |= 8;
    } else {
        static_cast<instNUGCUTSCENE_s *>(cut->instance)->flags_88 &= ~8U;
    }
    cut->field_58 = 0.0f;

    if (cut->music_handle != -1) {
        music_man.SelectTrackByHandle(TRACK_CLASS_CUTSCENE, cut->music_handle);
        i32 status = music_man.PlayTrack(TRACK_CLASS_CUTSCENE, 0);
        static_cast<instNUGCUTSCENE_s *>(cut->instance)->rate = 0.0f;
        if ((cut->flags & 1) == 0 || status != 1) {
            cutaudiopaused = 0;
        } else {
            CutSceneWaiting = 1;
            PauseGameAudio(0);
            cutaudiopaused = 1;
        }
    } else {
        PauseGameAudio(0);
        SetLinkedCutSceneMusic(static_cast<instNUGCUTSCENE_s *>(cut->instance),
                               cut->linked_audio == 0 ? MUSIC_PLAYBACK_DUAL_STREAM
                                                      : MUSIC_PLAYBACK_DUAL_STREAM_PENDING);
        static_cast<instNUGCUTSCENE_s *>(cut->instance)->rate = cut->frames_per_second * FRAMETIME;
        if ((cut->flags & 1) != 0) {
            CutSceneWaiting = 0;
            cutaudiopaused = 0;
        }
    }

    if ((cut->flags & 1) == 0) {
        return 1;
    }
    Panel_Clear();
    CutFrame = 0;
    for (i32 i = 0; i < world->cutscene_sys->count; ++i) {
        CUTINFO *other = world->cutscene_sys->cuts[i];
        instNUGCUTSCENE_s *other_instance = static_cast<instNUGCUTSCENE_s *>(other->instance);
        if ((other_instance->flags_88 & 2) != 0) {
            if (other == cut || (other->flags & 1) == 0) {
                other_instance->rate = 0.0f;
                instNuGCutScenePause(static_cast<instNUGCUTSCENE_s *>(world->cutscene_sys->cuts[i]->instance), 1);
            } else {
                other_instance->rate = 0.0f;
                instNuGCutSceneStop(other_instance);
            }
        }
    }
    CUTSTOPGAME = 1;
    CUTNOFOG = cut->flags & 4;
    CUTDRAWWORLD = cut->flags & 2;
    CutStopInfo = cut;
    DebrisSetRenderGroup(cut->debris_render_group);
    LevelNameTime = 0.0f;
    LevelNameMul = 0.0f;
    ACTIVECUTCOUNT = 1;
    CutBorderScale = 1.0f;
    cut_waiting_for_new_level = 0;
    GameFog_Reset();
    for (CUTSCENETEXANIM &animation : cut->texture_animations) {
        if (animation.index != -1) {
            texanimbits &= ~(1U << (animation.index & 0x1f));
        }
    }
    return 1;
}

void CutScenes_Reset(WORLDINFO_s *world) {
    CUTSYS *system = world->cutscene_sys;
    if (system != NULL) {
        auto system_count = system->count;
        for (i32 i = 0; i < system_count; ++i) {
            CUTINFO *cut = system->cuts[i];
            instNUGCUTSCENE_s *instance = static_cast<instNUGCUTSCENE_s *>(cut->instance);
            if (instance == NULL)
                continue;
            NUGCUTCHARSYS_s *characters = static_cast<NUGCUTSCENE_s *>(cut->scene)->character_system;
            instNUGCUTCHARSYS_s *runtime = instance->character_instance;
            if (characters == NULL || runtime == NULL)
                continue;
            for (i32 j = 0; j < characters->character_count; ++j) {
                instNUGCUTCHAR_s *character = &runtime->characters[j];
                if (character->character != reinterpret_cast<void *>(1))
                    continue;
                NUGCUTCHAR_s *definition = &characters->characters[j];
                if ((character->field_14 & 2) == 0) {
                    char *model_file = definition->model_file;
                    i32 p;
                    for (p = 0; p < 8; ++p) {
                        GameObject_s *player = Player[p];
                        if (player != NULL && (player->apiobj.flags_low & 1) != 0 &&
                            NuStrICmp(player->apiobj.character_data->file, model_file) == 0) {
                            character->character = player;
                            break;
                        }
                    }
                    if (p == 8) {
                        character->character = NULL;
                    }
                } else {
                    character->character = NULL;
                }
                definition->character = character->character;
            }
        }
    }
    cutaudiopaused = 0;
    CutStopInfo = NULL;
    CutSceneWaiting = 0;
    CutBorderScale = 0.0f;
    if (world->cutscene_sys != NULL && reset_restart != 0 &&
        ((world->current_level->flags & 0xe0) != 0 || world->level_progress == NULL ||
         (world->level_progress->flags & 2) == 0)) {
        CUTINFO *cut = NULL;
        CUTSCENEPLAYERCLIP *clip = CutScenePlayer_Active();
        if (clip != NULL && world->cutscene_sys->count > 0) {
            if (clip->name[0] != '\0')
                cut = CutScene_Find(world->cutscene_sys, clip->name);
            if (cut == NULL)
                cut = world->cutscene_sys->cuts[0];
        }
        if (cut == NULL && cutskip_dontplaylevelintro == 0) {
            for (i32 i = 0; i < world->cutscene_sys->count; ++i) {
                if ((world->cutscene_sys->cuts[i]->flags & 0x1000) == 0) {
                    continue;
                }
                cut = world->cutscene_sys->cuts[i];
                break;
            }
        }
        if (cut != NULL) {
            MechSystems::Get()->PauseButton().skip_prompt_timer = 0.0f;
            if (cut->instance != NULL) {
                bool start = true;
                LEVEL_PROGRESS_s *progress = world->level_progress;
                if (world->cutscene_sys != NULL) {
                    i32 index = -1;
                    for (i32 i = 0; i < world->cutscene_sys->count; ++i) {
                        if (world->cutscene_sys->cuts[i] == cut)
                            index = i;
                    }
                    if (progress != NULL) {
                        u32 mask = 1u << (index & 31);
                        if (static_cast<i16>(index) != -1 && (cut->playback_flags & 1) != 0 &&
                            (progress->played_cutscene_mask & mask) != 0)
                            start = false;
                        else
                            progress->played_cutscene_mask |= mask;
                    }
                }
                if (start && CutScene_Start(world, cut, 0) != 0)
                    CutBorderScale = 1.0f;
            }
        }
    }
    NewCutInfoCount = 0;
    cutskip_dontplaylevelintro = 0;
    cut_waiting_for_new_level = 0;
}

void CutScenes_Start(WORLDINFO_s *world) {
    for (i32 i = 0; i < NewCutInfoCount; ++i) {
        CUTINFO *cut = NewCutInfo[i];
        MechSystems::Get()->PauseButton().skip_prompt_timer = 0.0f;
        if (cut == NULL || cut->instance == NULL) {
            continue;
        }
        LEVEL_PROGRESS_s *level_progress = world->level_progress;
        i32 cutscene_index = -1;
        if (world->cutscene_sys != NULL) {
            for (i32 j = 0; j < world->cutscene_sys->count; ++j) {
                if (world->cutscene_sys->cuts[j] == cut) {
                    cutscene_index = j;
                }
            }
            if (level_progress != NULL) {
                const u32 bit = 1U << (cutscene_index & 0x1f);
                if (static_cast<i16>(cutscene_index) != -1 && (cut->end_flags & 1) != 0 &&
                    (level_progress->played_cutscene_mask & bit) != 0) {
                    continue;
                }
                level_progress->played_cutscene_mask |= bit;
            }
        }
        CutScene_Start(world, cut, cutscene_index);
    }
    NewCutInfoCount = 0;
}

