// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "gameapi/gui/apimenu_internal.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/mission.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numath/nufloat.h"
#include <stdio.h>
#include <string.h>
#include "nu2api/numath/numtx.h"
#include "legoapi/world/level.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/world.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/render/fx/game_deb.h"
#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/props/system/socksys.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nusound/nusound.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include <stdlib.h>
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nu3d/nugscn.h"
#include <math.h>
#include "batman.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/render/fx/particles.h"
#include "legoapi/world/areas.h"
#include "legoapi/actions/character/streaks.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "nu2api/numusic/sfx.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "gameframework/saveload.h"
#include "nu2api/nucore/nuvideo.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nucore/nuhgobj.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/render/fx/edsplines.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nupostparams.h"
#include <new>
#include <stdint.h>
#include "nu2api/nucore/nulist.h"
#include "gameapi/ai/aisys/aiscript_types.h"
#include <float.h>
#include "editor/edpath.h"
#include "legoapi/actions/character/speederchase.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/numath/nuang.h"
#include "editor/aieditor_state.h"
#include "nu2api/numath/nuplane.h"
#include "editor/aieditor_settings.h"
#include "editor/edpath_types.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/characters/motion/contexts.h"
#include <stddef.h>
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include <cstdio>
#include "nu2api/numath/nuvec4.h"
#include <stdarg.h>
#include <GLES2/gl2.h>
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/menus/screens/movies.h"
#include "legoapi/misc/androidbatman.h"
#include <cfloat>
#include "legoapi/items/base/animpacket.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "gameapi/ai/aisys/aipath.h"
#include "nu2api/nucore/numechptr.hpp"
#include "legoapi/gizmo/base/GizBuildItObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizLeverObjectInterface.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizPanelObjectInterface.h"
#include "legoapi/gizmo/base/GizTurretObjectInterface.h"
#include "legoapi/gizmo/base/HatMachineObjectInterface.h"
#include "legoapi/gizmo/base/TeleportObjectInterface.h"
#include "nu2api/nu3d/nuhspecial.h"
#include "legoapi/world/levels/podrace.h"
#include <ctype.h>
#include <limits.h>
#include "legoapi/lib/text.h"
#include "nu2api/nuandroid/ios_device.h"
char *ASCII_UP = "\xc2\xac";
extern char * apitxt_ENGLISH;
extern char * apitxt_FRENCH;
extern char * apitxt_GERMAN;
extern char * apitxt_ITALIAN;
extern char * apitxt_SPANISH;
extern char * apitxt_DANISH;

// Original .data @0x667ce0; the names are filled in by the static initializer.
static LANGUAGEDATA Text_LanguageList_Default[7] = {{1, apitxt_ENGLISH}, {2, apitxt_FRENCH},  {4, apitxt_GERMAN},
                                                    {5, apitxt_ITALIAN}, {3, apitxt_SPANISH}, {8, apitxt_DANISH},
                                                    {-1, NULL}};
LANGUAGEDATA *Text_LanguageList = Text_LanguageList_Default;
void (*Text_GameSetLanguageFn)(i32);
static VUFNT *app_fnt;
static TEXTCRAWL_s *s_crawlPtr = nullptr;
static NUMTX s_textcrawl_mtx;
static NUVEC s_textcrawl_offset = {0.0f, -1.0f, 10.0f};
f32 APITEXTSCALEX = 1.0f;
f32 APITEXTSCALEY = 1.0f;
i32 textcrawlactive = 0;
i32 Arcade_TextCrawlID = 0x1f1;
i32 Arcade_TextCrawlParagraphs = 2;

static char **TTab_Original;
static i32 Text_MaxStrings_Overall;
static u32 *Text_StringBits;

i32 MenuStopDraw;

char *txtpath_JAPANESE = const_cast<char *>("japanese");
char *txtpath_DANISH = const_cast<char *>("danish");
char *txtpath_SPANISH = const_cast<char *>("spanish");
char *txtpath_ITALIAN = const_cast<char *>("italian");
char *txtpath_GERMAN = const_cast<char *>("german");
char *txtpath_FRENCH = const_cast<char *>("french");
char *txtpath_ENGLISH = const_cast<char *>("english");

static inline i32 Text_MenuColourLerp(u8 first, u8 second, f32 amount) {
    return static_cast<i32>(static_cast<f32>(first) * amount + static_cast<f32>(second) * (1.0f - amount));
}

extern WORLDINFO WorldInfo[2];

extern i32 LevFlag[4];

extern i32 LevFlag[4]; // shared per-level 16-byte state scratch

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

struct MENU_s;

struct EDCREATURE_s;

typedef isize __attribute__((may_alias)) NuGCutWord;

extern AIACTIONDEF api_aiactiondefs[55];
extern AICONDITIONDEF api_aiconditiondefs[50];

DECOMP_ASSERT(sizeof(api_aiactiondefs) == 0x294, "API action registry size");

DECOMP_ASSERT(sizeof(api_aiconditiondefs) == 0x258, "API condition registry size");

struct eduimenu_s;
struct eduiitem_s;

extern "C" {
    struct EDCREATURE_s;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

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

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

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
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
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

}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
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

}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
    struct EDCREATURE_s;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

// Original writable flagremaptab at 0x668520, 32 pairs of 32-bit masks.
extern "C" {
extern u32 flagremaptab[32][2];
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

void Text_InitLanguageList(LANGUAGEDATA *language_list) {
    if (language_list != NULL) {
        Text_LanguageList = language_list;
    }

    LANGUAGECOUNT = 0;
    while (Text_LanguageList[LANGUAGECOUNT].language != -1) {
        ++LANGUAGECOUNT;
    }
}

void Text_SetLanguage(i32 language) {
    if (language == -1) {
        language = Text_Language;
    } else {
        if (language == 0) {
            language = 1;
        }
        Text_Language = language;
    }

    NuLanguageSet(language);
    if (Text_GameSetLanguageFn != NULL) {
        Text_GameSetLanguageFn(Text_Language);
    }
}

char *Text_GetLanguagePath(i32 language) {
    switch (language) {
        case 0:
            return txtpath_JAPANESE;
        case 2:
            return txtpath_FRENCH;
        case 3:
            return txtpath_SPANISH;
        case 4:
            return txtpath_GERMAN;
        case 5:
            return txtpath_ITALIAN;
        case 8:
            return txtpath_DANISH;
        default:
            return txtpath_ENGLISH;
    }
}

void Text_LoadFont(char *path, variptr_u *buf, variptr_u *buf_end) {
    create_qfont3dz = 1;
    app_fnt = LoadGameFont(path, path, buf, buf_end, 1);
    LoadButtonFont("stuff\\text\\Buttons", 0, buf, buf_end, 0);
}

void *Text_IsFontLoaded() {
    return app_fnt;
}

void Text_InitStringTable(i32 count, variptr_u *buf, variptr_u *) {
    TTab_Original = reinterpret_cast<char **>(buf->void_ptr);
    usize table_size = (count + 1) * sizeof(char *);
    buf->addr = ALIGN(buf->addr + table_size, 4);
    memset(TTab_Original, 0, table_size);
    Text_MaxStrings_Overall = count;
    TTab_Original[0] = "Err...";
    TTab = TTab_Original + 1;

    i32 flags_size = ((count + 31) >> 5) * sizeof(u32);
    Text_StringBits = buf->u32_ptr;
    memset(Text_StringBits, 0, flags_size);
    buf->addr += flags_size;
}

i32 Text_GetMaxOverallStrings() {
    return Text_MaxStrings_Overall;
}

void Text_LoadAndFixUpStrings(unsigned char *filename, unsigned char **buffer, char **table, i32 count) {
    unsigned char *out = *buffer;
    NUFPAR *parser = NuFParCreate(reinterpret_cast<char *>(filename));
    if (parser != nullptr) {
        if (parser->is_utf16 != 0) {
            while (NuFParGetLineW(parser) != 0) {
                i32 index = NuFParGetInt(parser);
                if (!(index <= 0) && !(index >= count)) {
                    NuFParGetWordW(parser);
                    u16 *wide = reinterpret_cast<u16 *>(parser->word_buf);
                    i32 length = NuStrLenW(wide);
                    if (length <= 0)
                        continue;
                    table[index] = reinterpret_cast<char *>(out);
                    for (i32 i = 0; i < length; i++)
                        out = NuUTF8CharFromUnicode(out, wide[i]);
                    *out++ = 0;
                }
            }
        } else if (parser->is_utf8 != 0) {
            while (NuFParGetLine(parser) != 0) {
                NuFParGetWord(parser);
                if (parser->word_buf[0] == 0)
                    continue;
                i32 index = NuAToI(parser->word_buf);
                if (index <= 0 || index >= count)
                    continue;
                NuFParGetWord(parser);
                if (NuStrICmp(parser->word_buf, "360") == 0 || NuStrICmp(parser->word_buf, "gc") == 0 ||
                    NuStrICmp(parser->word_buf, "ps2") == 0 || NuStrICmp(parser->word_buf, "ps3") == 0 ||
                    NuStrICmp(parser->word_buf, "psp") == 0 || NuStrICmp(parser->word_buf, "pc") == 0 ||
                    NuStrICmp(parser->word_buf, "wii") == 0 || NuStrICmp(parser->word_buf, "xbox") == 0)
                    continue;
                i32 length = NuStrLen(parser->word_buf);
                table[index] = reinterpret_cast<char *>(out);
                NuStrCpy(reinterpret_cast<char *>(out), parser->word_buf);
                out += length + 1;
            }
        } else {
            while (NuFParGetLine(parser) != 0) {
                i32 index = NuFParGetInt(parser);
                if (index <= 0 || index >= count)
                    continue;
                NuFParGetWord(parser);
                if (NuStrICmp(parser->word_buf, "360") == 0 || NuStrICmp(parser->word_buf, "gc") == 0 ||
                    NuStrICmp(parser->word_buf, "ps2") == 0 || NuStrICmp(parser->word_buf, "ps3") == 0 ||
                    NuStrICmp(parser->word_buf, "psp") == 0 || NuStrICmp(parser->word_buf, "pc") == 0 ||
                    NuStrICmp(parser->word_buf, "wii") == 0 || NuStrICmp(parser->word_buf, "xbox") == 0)
                    continue;
                char *word = parser->word_buf;
                i32 length = NuStrLen(word);
                if (length <= 0)
                    continue;
                table[index] = reinterpret_cast<char *>(out);
                for (i32 i = 0; i < length; ++i) {
                    const u8 character = static_cast<u8>(word[i]);
                    if (character >= 0x80)
                        out = NuUTF8CharFromUnicode(out, character);
                    else
                        *out++ = character;
                }
                *out++ = 0;
            }
        }
        NuFParDestroy(parser);
    }
    *buffer = out;
}

void Text_InitTable(TEXTENTRY *entry, i32 first, i32 last) {
    if (entry == nullptr)
        return;

    i32 index = 0;
    while (entry != nullptr && entry->text_id != nullptr) {
        if (entry->value != -1) {
            index = entry->value;
            if (index >= first && index <= last)
                *entry->text_id = entry->value;
        } else {
            if (index >= first && index <= last) {
                entry->value = index;
                *entry->text_id = index;
                Text_StringBits[index / 32] |= 1U << (index & 0x1f);
            } else {
                entry->value = 0;
                *entry->text_id = 0;
            }
        }
        index++;
        entry++;
    }
}

void Text_InsertCommasIntoNumber(char *number, char *text, i32 length) {
    char separator;
    if (Text_Language == 2) {
        separator = ' ';
    } else if (Text_Language == 1 || Text_Language == 18 || Text_Language == 0) {
        separator = ',';
    } else {
        separator = '.';
    }

    if (length < 0) {
        length = NuStrLen(number);
    }

    i32 output = 0;
    for (i32 digit = 0; digit < length; ++digit) {
        text[output++] = number[digit];
        const i32 remaining = length - digit - 1;
        if (remaining != 0 && remaining % 3 == 0) {
            text[output++] = separator;
        }
    }
    text[output] = '\0';
}

void Text_MakeScore(u32 score, char *text) {
    char digits[80];
    char *end = &digits[79];
    *end = '\0';

    char *first = end - 1;
    *first = static_cast<char>('0' + score % 10);
    score /= 10;
    while (score != 0) {
        *--first = static_cast<char>('0' + score % 10);
        score /= 10;
    }

    Text_InsertCommasIntoNumber(first, text, static_cast<i32>(end - first));
}

void Text_MakeTime(float time, i32 show_hours, i32 show_minutes, i32 show_centiseconds, char *text) {
    time = MAX(0.0f, time);

    i32 hours = 0;
    i32 minutes;
    if (show_hours != 0) {
        hours = static_cast<i32>(time / 3600.0f);
        minutes = static_cast<i32>(NuFmod(time / 60.0f, 60.0f));
    } else {
        minutes = static_cast<i32>(time / 60.0f);
    }

    i32 seconds;
    if (show_hours != 0 || show_minutes != 0) {
        seconds = static_cast<i32>(NuFmod(time, 60.0f));
    } else {
        seconds = static_cast<i32>(time);
    }
    const i32 centiseconds = static_cast<i32>(NuFmod(time, 1.0f) * 100.0f);

    if (text == nullptr) {
        return;
    }

    if (show_hours != 0) {
        if (show_centiseconds != 0) {
            sprintf(text, "%i:%.2i:%.2i.%.2i", hours, minutes, seconds, centiseconds);
        } else {
            sprintf(text, "%i:%.2i:%.2i", hours, minutes, seconds);
        }
    } else if (show_minutes != 0) {
        if (show_centiseconds != 0) {
            sprintf(text, "%i:%.2i.%.2i", minutes, seconds, centiseconds);
        } else {
            sprintf(text, "%i:%.2i", minutes, seconds);
        }
    } else if (show_centiseconds != 0) {
        sprintf(text, "%i.%.2i", seconds, centiseconds);
    } else {
        sprintf(text, "%i", seconds);
    }
}

void Text_ExpandAllButtonStrings(char *input, char *output) {
    char token[256];
    char expanded[64];

    *output = '\0';
    while (*input != '\0') {
        if (*input != '[') {
            *output = *input;
            output[1] = '\0';
            ++input;
            ++output;
            continue;
        }

        token[0] = '[';
        i32 token_length = 1;
        while (input[token_length] != '\0' && input[token_length] != ']') {
            token[token_length] = input[token_length];
            ++token_length;
        }

        char literal = '[';
        if (input[token_length] != '\0') {
            token[token_length] = ']';
            token[token_length + 1] = '\0';
            token_length = NuStrLen(token);
            if (token_length > 0) {
                if (Text_ExpandButtonString(token, expanded) != 0) {
                    char *character = expanded;
                    while (*character != '\0') {
                        *output = *character;
                        output[1] = '\0';
                        ++character;
                        ++output;
                    }
                    NuStrCat(output, expanded);
                    input += token_length;
                    continue;
                }
            }
            literal = *input;
        }

        *output = literal;
        output[1] = '\0';
        ++input;
        ++output;
    }
    *output = '\0';
}

i32 Text_StripComments(char *text, char *dest, i32 param) {
    char *end = text + NuStrLen(text);
    char *out = dest;
    while (text <= end) {
        char c = *text;
        if (c == '#' || c == ';' || (c == '/' && (c = *++text) == '/')) {
            c = *++text;
            while (c != '\r' && c != '\0' && c != '\n') {
                c = *++text;
            }
        }
        if (param != 0 && (c == ',' || c == '=')) {
            c = ' ';
        }
        *out++ = c;
        text++;
    }
    *out = '\0';
    return (i32)(out - dest);
}

void IntroText_SetTextID(i32 id) {
    if (id > 0 && TTab[id] != nullptr)
        IntroText_TextID = id;
}

void IntroText_Draw(float alpha) {
    if (IntroText_TextID == -1)
        return;
    SetQFont2D();
    NuQFntSetSpaceWidth(QFont2D, 8.0f);
    u16 encoded[128];
    Text3DStringEncode(TTab[IntroText_TextID], encoded);
    NuQFntSetJustifiedTolerances(1.0f, 1.0f);
    u32 colour = (static_cast<i32>(alpha * 128.0f) << 24) | 0x7f5f00;
    NuQFntPrintJustifiedW(QFont2D, encoded, -0.85f, INTROTEXT_Y, 1.0f, INTROTEXT_SCALE, INTROTEXT_SCALE, 1.7f, 1.0f,
                          colour, 0);
}

void TextCrawl_Init(TEXTCRAWL_s *crawl, i32 id, i32 unk) {
    if (crawl == nullptr || QFont3DZ == nullptr) {
        return;
    }
    s_crawlPtr = crawl;
    reinterpret_cast<i8 *>(crawl)[0x0d] = id >= 0 && id < AREACOUNT ? static_cast<i8>(id) : -1;
    NuMtxSetRotationX(&s_textcrawl_mtx, 0x3333);
    NuMtxRotateY(&s_textcrawl_mtx, 0);
    NuMtxRotateZ(&s_textcrawl_mtx, 0);
    NuMtxTranslate(&s_textcrawl_mtx, &s_textcrawl_offset);
    if (unk != 0) {
        QFont3DTime = 0.0f;
    }
}

void TextCrawl_Draw(float dt, i32 paragraphs, float alpha, char *text) {
    if (s_crawlPtr == nullptr || QFont3DZ == nullptr) {
        return;
    }
    NuQFntSetJustifiedTolerances(1.2f, 1.2f);
    if (paragraphs == 0 && QFont3DTime >= 60.0f) {
        return;
    }

    AREADATA *area = NULL;
    i32 episode_index = -1;
    i32 body_text_id = -1;
    i32 area_index = -1;
    const i8 crawl_area = reinterpret_cast<i8 *>(s_crawlPtr)[0x0d];
    if (crawl_area != -1) {
        area = &ADataList[crawl_area];
        area_index = static_cast<i8>(area->area_index);
        episode_index = static_cast<i8>(area->episode_index);
        body_text_id = area->text_id;
    }

    char *episode_text = NULL;
    char *episode_name = NULL;
    char *chapter_text = NULL;
    char *area_name = NULL;
    i32 paragraph_count = 0;
    char chapter_buffer[0x40];

    if (text != NULL) {
        episode_name = text;
        body_text_id = -1;
    } else {
        MISSIONDATA *mission = Mission_Active(NULL);
        if (mission != NULL) {
            if (mission->name_id != -1) {
                area_name = TTab[mission->name_id];
            }
            body_text_id = mission->text_id;
            paragraph_count = 3;
        } else if (Arcade != 0) {
            if (area != NULL && area->name_id != -1) {
                area_name = TTab[area->name_id];
            }
            body_text_id = Arcade_TextCrawlID;
            paragraph_count = Arcade_TextCrawlParagraphs;
        } else if (episode_index == -1) {
            if (area != NULL && area->name_id != -1) {
                area_name = TTab[area->name_id];
            }
        } else {
            if (EDataList[episode_index].name_id != -1) {
                episode_name = TTab[EDataList[episode_index].name_id];
            }
            if (EDataList[episode_index].text_id != -1) {
                episode_text = TTab[EDataList[episode_index].text_id];
            }
            if (area_index != -1) {
                if ((area->flags & AREAFLAG_BONUS_AREA) != 0) {
                    i16 *bonus_text = (area->flags & AREAFLAG_VEHICLE_AREA) != 0 ? s_crawlPtr->vehicle_bonus_text
                                                                                 : s_crawlPtr->character_bonus_text;
                    if (bonus_text != NULL) {
                        chapter_text = TTab[*bonus_text];
                    }
                } else {
                    if (s_crawlPtr->heading_text != NULL) {
                        sprintf(chapter_buffer, "%s %i", TTab[*s_crawlPtr->heading_text], area_index + 1);
                        chapter_text = chapter_buffer;
                    }
                }
            }
            if (area->name_id != -1) {
                area_name = TTab[area->name_id];
            }
            paragraph_count = 0;
        }
    }

    g_bgloadAsFastAsPossibleHack = alpha > 0.75f;
    QFont3DTime += dt;
    if (QFont3DTime >= 60.0f) {
        if (paragraphs == 0) {
            return;
        }
        QFont3DTime = 0.0f;
    }

    NUMTX matrix;
    NuMtxMul(&matrix, &s_textcrawl_mtx, NuCameraGetMtx());
    NuQFntSet(QFont3DZ);
    NuQFntSetMtx(QFont3DZ, &matrix);
    NuQFntPushPrintMode(4);
    NuQFntSetCoordinateSystem(NUQFNT_CSMODE_ABSOLUTE);

    f32 colour_scale = 1.0f;
    if (paragraphs == 0 && QFont3DTime >= 55.0f) {
        colour_scale = 1.0f - (QFont3DTime - 55.0f) / 5.0f;
    }
    const u32 colour = (static_cast<u32>(static_cast<i32>(128.0f * colour_scale * alpha)) << 24) |
                       ((static_cast<u32>(static_cast<i32>(111.0f * colour_scale)) & 0xff) << 8) | 0xff;
    NuQFntSetColour(QFont3DZ, colour);

    f32 y = QFont3DTime * 0.4f - 4.5f;
    const f32 x_scale = APITEXTSCALEX * 0.01f;
    const f32 y_scale = APITEXTSCALEY * 0.01f;
    u16 encoded[1024];

    if (episode_name != NULL) {
        if (text != NULL && NuStrCmp(text, "?") == 0) {
            NuQFntSetScale(QFont3DZ, x_scale * 2.0f, y_scale * 3.0f);
        } else {
            NuQFntSetScale(QFont3DZ, x_scale, y_scale);
        }
        Text3DStringEncode(episode_name, encoded);
        NuQFntMove(QFont3DZ, -NuQFntPrintLenW(QFont3DZ, encoded) * 0.5f, y, 0.0f);
        NuQFntPrintW(QFont3DZ, encoded);
        y += NuQFntHeight(QFont3DZ) * 1.5f;
    }

    if (episode_text != NULL) {
        NuQFntSetScale(QFont3DZ, x_scale, y_scale * 2.0f);
        Text3DStringEncode(episode_text, encoded);
        NuQFntMove(QFont3DZ, -NuQFntPrintLenW(QFont3DZ, encoded) * 0.5f, y, 0.0f);
        NuQFntPrintW(QFont3DZ, encoded);
        y += NuQFntHeight(QFont3DZ) * 1.5f;
    }

    if (chapter_text != NULL) {
        NuQFntSetScale(QFont3DZ, x_scale, y_scale);
        Text3DStringEncode(chapter_text, encoded);
        NuQFntMove(QFont3DZ, -NuQFntPrintLenW(QFont3DZ, encoded) * 0.5f, y, 0.0f);
        NuQFntPrintW(QFont3DZ, encoded);
        y += NuQFntHeight(QFont3DZ) * 1.5f;
    }

    if (area_name != NULL) {
        NuQFntSetScale(QFont3DZ, x_scale, y_scale * 2.0f);
        Text3DStringEncode(area_name, encoded);
        NuQFntMove(QFont3DZ, -NuQFntPrintLenW(QFont3DZ, encoded) * 0.5f, y, 0.0f);
        NuQFntPrintW(QFont3DZ, encoded);
        y += NuQFntHeight(QFont3DZ) * 1.5f;
    }

    if (body_text_id != -1) {
        NuQFntSetScale(QFont3DZ, x_scale, y_scale);
        if (paragraph_count == 0) {
            paragraph_count = area == NULL ? s_crawlPtr->paragraph_count : area->text_id_value;
        }
        for (i32 i = 0; i < paragraph_count; ++i) {
            char *paragraph = TTab[body_text_id + i];
            if (paragraph == NULL || *paragraph == '\0') {
                break;
            }
            Text3DStringEncode(paragraph, encoded);
            y +=
                NuQFntPrintJustifiedW(QFont3DZ, encoded, -3.5f, y, 0.0f, x_scale, y_scale, 7.0f, 1.3f, colour, &matrix);
            y += NuQFntHeight(QFont3DZ);
        }
    }
    NuQFntPopPrintMode();
}

void MenuUpdateViewTextStrings(MENU_s *menu) {
    if (menu->cancel_pressed != 0) {
        BackupMenu();
        return;
    }

    if ((Player[0] == nullptr || (Player[0]->pad_gamepad->buttons_held & GAMEPAD_MENUSELECT) == 0) &&
        (Player[1] == nullptr || (Player[1]->pad_gamepad->buttons_held & GAMEPAD_MENUSELECT) == 0)) {
        return;
    }

    if (menu->up_held != 0) {
        menu->selected_row -= 5;
        if (menu->selected_row < menu->first_row) {
            menu->selected_row = menu->last_row;
        }
        menu->selected_item = menu->selected_row - menu->first_row;
    } else if (menu->down_held != 0) {
        menu->selected_row += 5;
        if (menu->selected_row > menu->last_row) {
            menu->selected_row = menu->first_row;
        }
        menu->selected_item = menu->selected_row - menu->first_row;
    }
}

void MenuDrawViewTextStrings(MENU_s *menu) {
    i32 blue;
    i32 green;
    menu->item_scale = 2.0f;
    menu->centre_offset = MENUDY * 2.0f;
    menu->draw_x = -0.75f;
    menu->draw_y = static_cast<f32>(-menu->selected_row) * menu->centre_offset;

    char text[2048];
    auto menu_item_scale = menu->item_scale;
    for (i32 i = 0; i < Text_MaxStrings_Overall; ++i) {
        dme_sy = menu_item_scale;
        dme_sx = 0.6f;
        dme_align = 0;
        GameDrawMenuEntry(menu, const_cast<char *>(" "));

        const f32 draw_y = menu->draw_y;
        const f32 centre_offset = menu->centre_offset;
        if (MenuStopDraw != 0) {
            continue;
        }
        const f32 y = draw_y - centre_offset;
        if (!(y >= -1.25f && y < 1.25f)) {
            continue;
        }

        const bool registered = (Text_StringBits[i >> 5] & (1U << (i & 0x1f))) != 0;
        i32 red;

        if (registered) {
            if (menu->selected_item == i) {
                if (!TestForController()) {
                    if (menu_pulse > 0.0f) {
                        red = Text_MenuColourLerp(MENUFLASH0R, MENUNORMALR, menu_pulse);
                        green = Text_MenuColourLerp(MENUFLASH0G, MENUNORMALG, menu_pulse);
                        blue = Text_MenuColourLerp(MENUFLASH0B, MENUNORMALB, menu_pulse);
                    } else {
                        red = MENUENTRYR;
                        green = MENUENTRYG;
                        blue = MENUENTRYB;
                    }
                } else {
                    if (menu_pulsate > 0.0f) {
                        red = Text_MenuColourLerp(MENUFLASH0R, MENUFLASH1R, menu_pulsate);
                        green = Text_MenuColourLerp(MENUFLASH0G, MENUFLASH1G, menu_pulsate);
                        blue = Text_MenuColourLerp(MENUFLASH0B, MENUFLASH1B, menu_pulsate);
                    } else if (menu_flash != 0) {
                        red = MENUFLASH0R;
                        green = MENUFLASH0G;
                        blue = MENUFLASH0B;
                    } else {
                        red = MENUFLASH1R;
                        green = MENUFLASH1G;
                        blue = MENUFLASH1B;
                    }
                }
            } else {
                if (menu_pulse > 0.0f) {
                    red = Text_MenuColourLerp(MENUFLASH0R, MENUNORMALR, menu_pulse);
                    green = Text_MenuColourLerp(MENUFLASH0G, MENUNORMALG, menu_pulse);
                    blue = Text_MenuColourLerp(MENUFLASH0B, MENUNORMALB, menu_pulse);
                } else {
                    red = MENUENTRYR;
                    green = MENUENTRYG;
                    blue = MENUENTRYB;
                }
            }
        } else {
            if (menu->selected_item != i) {
                red = 255;
                green = 0;
                blue = 0;
            } else {
                red = 255;
                if (menu_flash < 1) {
                    green = 191;
                    blue = 0;
                } else {
                    green = 255;
                    blue = 255;
                }
            }
        }

        dme_r = static_cast<u8>(red);
        dme_g = static_cast<u8>(green);
        dme_rgb = 1;
        dme_b = static_cast<u8>(blue);
        sprintf(text, "%i", i);
        SmartTextEx(text, -0.76f, y, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 8, static_cast<u8>(red),
                    static_cast<u8>(green), static_cast<u8>(blue), 0.09000003f, 1, nullptr, 0, MenuA);

        if (TTab[i] != nullptr) {
            dme_rgb = 1;
            Text_ExpandAllButtonStrings(TTab[i], text);
            SmartTextEx(text, -0.74f, y, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 2, static_cast<u8>(red),
                        static_cast<u8>(green), static_cast<u8>(blue), 1.59f, 2, nullptr, 0, MenuA);
        }
    }
}

void Text_LocaliseDecimalPoint(char *text) {
    if ((Text_Language >= 2 && Text_Language <= 5) || Text_Language == 8 || Text_Language == 6 || Text_Language == 16 ||
        Text_Language == 7 || Text_Language == 12) {
        while (*text != '\0') {
            if (*text == '.') {
                *text = ',';
                return;
            }
            ++text;
        }
    }
}

void GameDrawMenuEntry(MENU_s *menu, char *text) {
    if (Paused != 0) {
        dme_align = PauseMenus_Align;
        menu->draw_x = PauseMenus_X;
    }
    DrawMenuEntryEx(menu, text, static_cast<u8>(MenuA));
}

// These queries follow GameDrawMenuEntry in the reference text/menu helpers,
// separate from UpdateGameMenu's input-routing translation unit.
i32 GetMenuID(void) {
    if (GameMenu[GameMenuLevel].menu != -1) {
        return MenuInfo[GameMenu[GameMenuLevel].menu].id;
    }
    return -1;
}

i32 GetParentMenuID() {
    if (GameMenuLevel <= 1) {
        return -1;
    }
    const i16 parent_menu = GameMenu[GameMenuLevel - 1].menu;
    if (parent_menu == -1) {
        return -1;
    }
    return MenuInfo[parent_menu].id;
}

