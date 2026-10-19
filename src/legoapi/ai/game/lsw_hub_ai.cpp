// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/ai/game/lsw_hub_ai.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nurand.h"

#include <stdio.h>
#include <string.h>
#include "batman.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/core/config/cheat.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/render.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/net/netplay.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nurndr.h"
#include "gameframework/saveload.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "gamelib/util/gamelib_util_types.h"
#include "nu2api/nufile/nufpar.h"
#include <stdlib.h>
#include "nu2api/nusound/nusound.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nu3d/numtl.h"
#include <math.h>
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/numath/nuvec4.h"
#include <cfloat>
#include "gameapi/edtools/gameapi_edtools_types.h"
#include <stdarg.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/gizmo/base/gizactions.h"
#include "editor/aieditor_state.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nuplane.h"
#include "legoapi/render/light/shadow.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "gameapi/edtools/edui.h"
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
#include "legoapi/props/system/socksys.h"
#include "nu2api/nucore/nutime.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nu3d/nuportal.h"
#include <stddef.h>
#include "gameapi/edtools/edgra.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include <cstdio>
#include <new>
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/ai/core/gameai.h"

struct HUBAI_s {
    AIAREA *areas[10];
    AIPATH *paths[10];
    AILOCATOR *area_locators[10][4];
    i16 area;
    i16 unknown_0x0f2;
    i16 table_count;
    i16 counter_count;
    u8 unknown_0x0f8[4];
    void *unknown_0x0fc;
    void *unknown_0x100;
    GIZAIMESSAGE_s *area_message;
    GIZAIMESSAGE_s *serving_customer_message;
    AILOCATOR *counter_a[4];
    AILOCATOR *counter_b[4];
    AILOCATOR *table_locators[8];
    AILOCATOR *serve_player;
};

DECOMP_ASSERT(sizeof(HUBAI_s) == 0x150, "HUBAI_s size");

HUBAI_s hub_ai = {};

char *hub_areas[10] = {"MAINROOM", "EPISODE1", "EPISODE2", "EPISODE3", "EPISODE4",
                       "EPISODE5", "EPISODE6", "JUNKYARD", "BONUS",    "BOUNTY"};

enum HUB_AI_CONSTANTS {
    HUB_AREA_COUNT = 10,
    HUB_PLAYER_COUNT = 8,
    HUB_TABLE_COUNT = 8,
    HUB_CHARACTER_CAPACITY = 340,
    HUB_BARMAN_CREATURE_SET = 2,
};

static inline void Hub_MarkCharacterUnavailable(i16 *characters, i16 character) {
    if (character != -1) {
        characters[character] = -1;
    }
}

// These two arrays are generic level-loader state rather than hub-owned state.

extern f32 PANEL_MINIKITCOUNTSCALE, PANEL_MINIKITCOUNTY;

// All recovered callers use player zero and full opacity. Keeping this private
// allows the compiler to specialize those constant arguments naturally.
// Retail only keeps the player-one, full-alpha specialisation of the cursor
// drawing body; its name records the partial-inlining and constprop clones.

extern i16 tSELECT, tSELECTED, tSELECTING, tCONFIRMED, tYES, tNO;

// Private hub menu and drawing helpers.

extern AREADATA *E1CHARACTER_ADATA, *E2CHARACTER_ADATA, *E3CHARACTER_ADATA, *E4CHARACTER_ADATA, *E5CHARACTER_ADATA, *E6CHARACTER_ADATA;
extern i16 id_PALPATINE, id_LAMASU, id_WOOKIEE, id_WAMPA, id_BAT, id_HANINCARBONITE;

extern i16 tCHALLENGECOMPLETE, tMINIKIT;

extern void (*CutScene_OverrideConfigFileNameFn)(char *, i32, i32);

typedef isize __attribute__((may_alias)) NuGCutWord;

extern "C" {
extern void *(*DefragCreateInstFn)(void *, NUGCUTSCENE_s *, VARIPTR *);

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

    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
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
} // extern "C"

extern "C" {
extern void (*checkantinodefns[3])(APIOBJECT *, AIANTINODE *, NUVEC *, f32);
    extern void (*checkantinodefns[3])(APIOBJECT_s *, AIANTINODE_s *, NUVEC *, f32);
}

extern f32 HUB_EPISODETITLEY;
extern i16 tSELECT, tSELECTED, tSELECTING;

extern void (*drawslotinfofn)(f32, f32, i32, i32);
extern void (*drawslotsfn)(MENU *, f32);

extern i16 tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);

extern i16 id_ANAKINJEDISCARRED, id_WOOKIEE, id_TWOFACE;

extern "C" {
    extern "C++" FadeSystem FadeSys;
}

extern void (*FS_Callback)(char *, char *);

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

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern EdInputContext * Input;
extern f32 Scale;
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

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

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

#include "nu2api/nucore/nucore_plain_types.h"
} // extern "C"

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

extern "C" i16 id_LANDSPEEDER;
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

#include "nu2api/nucore/nucore_plain_types.h"
} // extern "C"

struct AISYS_s;
struct AISCRIPT_s;

extern char *hub_areas[HUB_AREA_COUNT];

static void Hub_MakeListCharactersAvailable(i16 *characters) {
    for (i32 character = 0; character < CHARCOUNT; ++character) {
        characters[character] = apicharsys->playermodelids[character];
    }

    GameObject_s *object = Obj;
    for (i32 object_index = 0; object_index < HIGHGAMEOBJECT; ++object_index, ++object) {
        if ((object->apiobj.field_0x1f8 & APIOBJECT_FLAG_IN_USE) != 0) {
            characters[object->id] = -1;
        }
    }

    Hub_MarkCharacterUnavailable(characters, id_BARMAN);
    Hub_MarkCharacterUnavailable(characters, id_TRAININGREMOTE);
    Hub_MarkCharacterUnavailable(characters, id_JABBA);
    Hub_MarkCharacterUnavailable(characters, id_WOMPRAT);
    Hub_MarkCharacterUnavailable(characters, id_WEIRDO1);
    Hub_MarkCharacterUnavailable(characters, id_WEIRDO2);
    Hub_MarkCharacterUnavailable(characters, id_CANTINABAND);
    Hub_MarkCharacterUnavailable(characters, id_DROIDEKA);
    Hub_MarkCharacterUnavailable(characters, id_BOB);
    Hub_MarkCharacterUnavailable(characters, id_WHIP);

    for (i32 pack = 0; pack < 11; ++pack) {
        i16 *character = StorePack[pack].id;
        if (character != NULL) {
            i16 character_id = *character;
            if (character_id != -1 && Store_IsPackUnlocked(pack) == 0) {
                characters[character_id] = -1;
            }
        }
    }
}

static void Hub_GoneThroughDoor(WORLDINFO_s *world) {
    if (player == NULL) {
        return;
    }

    i32 new_area;
    for (new_area = 0; new_area < HUB_AREA_COUNT; ++new_area) {
        if (CheckPosAIArea(hub_ai.areas[new_area], PlayerStart[static_cast<i8>(player->apiobj.field_0x27c)].pos,
                           0.0f) != 0) {
            break;
        }
    }
    if (new_area == HUB_AREA_COUNT || new_area == hub_ai.area) {
        return;
    }

    hub_ai.area = static_cast<i16>(new_area);
    AIPATH *area_path = hub_ai.paths[new_area];
    if (area_path != NULL && AISysSetLevelPath(world->ai_sys, area_path->name) != 0) {
#define SET_HUB_PLAYER_PATH(index)                                                                                     \
    do {                                                                                                               \
        GameObject_s *path_player = Player[index];                                                                     \
        if (path_player != NULL && (path_player->apiobj.field_0x1f8 & APIOBJECT_FLAG_IN_USE) != 0) {                   \
            AISysCharacterSetPath(&path_player->ai, world->ai_sys->path_sys->active_path);                             \
            AISysGetCharacterPathPos(world->ai_sys, &path_player->apiobj, &path_player->ai, 0xff,                      \
                                     static_cast<i8>(path_player->apiobj.field_0x27d));                                \
        }                                                                                                              \
    } while (0)
        SET_HUB_PLAYER_PATH(0);
        SET_HUB_PLAYER_PATH(1);
        SET_HUB_PLAYER_PATH(2);
        SET_HUB_PLAYER_PATH(3);
        SET_HUB_PLAYER_PATH(4);
        SET_HUB_PLAYER_PATH(5);
        SET_HUB_PLAYER_PATH(6);
        SET_HUB_PLAYER_PATH(7);
#undef SET_HUB_PLAYER_PATH
    }

    i16 available_characters[HUB_CHARACTER_CAPACITY];
    Hub_MakeListCharactersAvailable(available_characters);

    GameObject_s *object = Obj;
    for (i32 object_index = 0; object_index < HIGHGAMEOBJECT; ++object_index, ++object) {
        if ((object->apiobj.field_0x1f8 & APIOBJECT_FLAG_IN_USE) == 0 || object->apiobj.field_0x27c != -1 ||
            object->ai.field_0x134 != 0xff) {
            continue;
        }

        object->field_0xeb4 = 0;
        FreeTorpedoPacket(&object->torpedo);
        RemoveGameObject(object, 1);
    }

    hub_ai.unknown_0x0f2 = 0;
    hub_ai.unknown_0x0fc = NULL;
    hub_ai.unknown_0x100 = NULL;
    SetGizAIMessage(gizaimessagesys, "ServingCustomer", 0.0f, hub_ai.serving_customer_message);
    if (hub_ai.area_message != NULL) {
        hub_ai.area_message->value = static_cast<f32>(hub_ai.area);
    }

    hub_ai.table_count = 0;
    char name[32];
#define FIND_HUB_TABLE(index)                                                                                          \
    do {                                                                                                               \
        if (hub_ai.area == 0) {                                                                                        \
            sprintf(name, "Table_%d", index);                                                                          \
        } else {                                                                                                       \
            sprintf(name, "Table%d_%d", hub_ai.area, index);                                                           \
        }                                                                                                              \
        hub_ai.table_locators[index] = AIPathFindLocator(world->ai_sys, name);                                         \
        if (hub_ai.table_locators[index] == NULL) {                                                                    \
            return;                                                                                                    \
        }                                                                                                              \
        ++hub_ai.table_count;                                                                                          \
    } while (0)
    FIND_HUB_TABLE(0);
    FIND_HUB_TABLE(1);
    FIND_HUB_TABLE(2);
    FIND_HUB_TABLE(3);
    FIND_HUB_TABLE(4);
    FIND_HUB_TABLE(5);
    FIND_HUB_TABLE(6);
    FIND_HUB_TABLE(7);
#undef FIND_HUB_TABLE
}

void LSW_Hub_InitAI(WORLDINFO_s *world) {
    memset(&hub_ai, 0, sizeof(hub_ai));

    char name[32];
    for (i32 area = 0; area < 10; ++area) {
        hub_ai.areas[area] = AISysFindArea(world->ai_sys, hub_areas[area]);
        hub_ai.paths[area] = AISysFindPath(world->ai_sys, hub_areas[area]);

        for (i32 locator = 0; locator < 4; ++locator) {
            sprintf(name, "%s_%d", hub_areas[area], locator + 1);
            hub_ai.area_locators[area][locator] = AIPathFindLocator(world->ai_sys, name);
        }
    }

    hub_ai.area = -1;
    hub_ai.area_message = SetGizAIMessage(gizaimessagesys, "Area", 0.0f, NULL);
    hub_ai.serving_customer_message = SetGizAIMessage(gizaimessagesys, "ServingCustomer", 0.0f, NULL);
    hub_ai.counter_count = 0;

    for (i32 counter = 0; counter < 4; ++counter) {
        sprintf(name, "CounterA_%d", counter);
        hub_ai.counter_a[counter] = AIPathFindLocator(world->ai_sys, name);
        sprintf(name, "CounterB_%d", counter);
        hub_ai.counter_b[counter] = AIPathFindLocator(world->ai_sys, name);
        if (hub_ai.counter_a[counter] == NULL || hub_ai.counter_b[counter] == NULL) {
            break;
        }
        hub_ai.counter_count++;
    }

    hub_ai.serve_player = AIPathFindLocator(world->ai_sys, "ServePlayer");
    hub_ai.table_count = 0;
    for (i32 i = 0; i < 8; ++i) {
        hub_ai.table_locators[i] = NULL;
    }
}

void LSW_Hub_ResetAI(WORLDINFO_s *world) {
    hub_ai.area = -1;
    LevGizObst[0] = GizObstacle_FindByName(world->giz_obstacle_sys, "BandDoor_Open");
}

i32 Hub_CurrentArea() {
    if (WORLD->current_level == HUB_LDATA) {
        return hub_ai.area;
    }
    return -1;
}

i32 Hub_Outside() {
    return Hub_CurrentArea() == 7;
}

// Level-system stubs that have not yet been split into a subsystem file.

i32 GetTableLocator(void) {
    return 0;
}

i32 GetCounterLocator(i32) {
    return 0;
}

void Hub_CallBarman(GameObject_s *) {
}

i32 Hub_GetRandomCharType() {
    i16 available_characters[HUB_CHARACTER_CAPACITY];
    i32 candidates[HUB_CHARACTER_CAPACITY];
    Hub_MakeListCharactersAvailable(available_characters);

    i32 candidate_count = 0;
    for (i32 character = 0; character < CHARCOUNT; ++character) {
        if (available_characters[character] != -1) {
            candidates[candidate_count++] = character;
        }
    }

    if (candidate_count == 0) {
        if (id_MOSEISLEYCITIZEN != -1 && apicharsys->playermodelids[id_MOSEISLEYCITIZEN] != -1) {
            candidates[candidate_count++] = id_MOSEISLEYCITIZEN;
        }
        if (id_CANTINAALIEN != -1 && apicharsys->playermodelids[id_CANTINAALIEN] != -1) {
            candidates[candidate_count++] = id_CANTINAALIEN;
        }
        if (candidate_count == 0) {
            return -1;
        }
    }
    return candidates[NuRandInt() % candidate_count];
}

void LSW_Hub_UpdateAI(WORLDINFO_s *world) {
    if (hub_character_ready != -1) {
        SetLevelSfxBits(world);
        hub_character_ready = -1;
    }

    if (netclient != 0) {
        return;
    }

    if (hub_ai.area == -1) {
        Hub_GoneThroughDoor(world);
        if (netclient != 0) {
            return;
        }
    }

    GIZOBSTACLE_s *barman_obstacle = LevGizObst[0];
    if (barman_obstacle != NULL) {
        if (aicreature_sets_alive[HUB_BARMAN_CREATURE_SET] != 0) {
            barman_obstacle->runtime_flags |= GIZOBSTACLE_RUNTIME_FLAG_AI_ACTIVE;
            barman_obstacle->runtime_flags &= static_cast<u8>(~GIZOBSTACLE_RUNTIME_FLAG_BLOCKED);
        } else {
            barman_obstacle->runtime_flags &= static_cast<u8>(~GIZOBSTACLE_RUNTIME_FLAG_AI_ACTIVE);
            barman_obstacle->runtime_flags |= GIZOBSTACLE_RUNTIME_FLAG_BLOCKED;
        }
    }
}

void *Condition_InHubAreaInit(AISYS_s *, char *argument, AISCRIPT_s *) {
    AI_HUB_AREA_ARGUMENT result = {};
    result.value = -1;
    if (argument != NULL) {
        for (isize area = 0; area < HUB_AREA_COUNT; ++area) {
            if (NuStrICmp(argument, hub_areas[area]) == 0) {
                result.value = area;
                break;
            }
        }
    }
    return result.pointer;
}

f32 Condition_InHubArea(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *area_argument) {
    AI_HUB_AREA_ARGUMENT argument = {};
    argument.pointer = area_argument;
    return argument.value == hub_ai.area ? 1.0f : 0.0f;
}

