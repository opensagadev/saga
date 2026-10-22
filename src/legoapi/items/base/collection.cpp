// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "legoapi/items/base/collection.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/characters/core/players.h"

#include "decomp.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/world.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/menus/core/gamehint.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"

#include <string.h>
#include <stdlib.h>
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/world/mission.h"
#include "legoapi/props/system/socksys.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nusound/nusound.h"
#include "nu2api/nufile/nufile.h"
#include <stdio.h>
#include "legoapi/gizmo/base/gizactions.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameframework/saveload.h"
#include "legoapi/render/core/render.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/world/level.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuvideo.h"
#include "legoapi/items/collect/torpedo.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/core/input/timer.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nuhgobj.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include <math.h>
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include <new>
#include "legoapi/gizmos/trigger/ai.h"
#include <stdint.h>
#include "nu2api/nucore/nulist.h"
#include "gameapi/ai/aisys/aiscript_types.h"
#include <float.h>
#include "editor/edpath.h"
#include "legoapi/actions/character/speederchase.h"
#include "batman.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/nucore/common.h"
#include "editor/aieditor_state.h"
#include "nu2api/numath/nuplane.h"
#include "editor/aieditor_settings.h"
#include "editor/edpath_types.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/render/fx/game_deb.h"
#include <stddef.h>
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edgra.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include <cstdio>
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/numath/nuvec4.h"
#include <stdarg.h>
#include <GLES2/gl2.h>
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "legoapi/world/areas.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/menus/screens/movies.h"
#include "legoapi/misc/androidbatman.h"
#include <limits.h>
#include "legoapi/render/fx/particles.h"
#include <cfloat>
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"

struct APICHARACTERMODELLIST_s;

COLLECTID *TempCollectID = NULL;

static i32 CollectCount;
static COLLECTID *CollectList;
i32 COLLECTION_COMPLETIONCOUNT = 0;

f32 COLLECTION_DX = 0.15f;
f32 COLLECTION_DY = -0.2f;
f32 COLLECTION_ICONSIZE = 0.16f;
i32 LEGOOBJ_ICON_FRAME_NEUTRAL = -1;
i32 LEGOOBJ_ICON_FRAME_BLUE = -1;
i32 LEGOOBJ_ICON_FRAME_GREEN = -1;
nuhspecial_s *collection_draw_hspecial;
i32 (*collection_draw_IsValidFn)(COLLECTION_s *, i32);
void (*Collection_GetSelectingPlayerIDsFn)(i16 *);

static inline void Collection_AddNeighbourFlags(u32 *flags, i32 col, i32 row, i32 sx, i32 sy) {
    if (sx == -1 || sy == -1)
        return;
    if ((col == sx && (row == sy - 1 || row == sy + 1)) || (row == sy && (col == sx - 1 || col == sx + 1)))
        *flags |= 1;
    else if ((col == sx - 1 || col == sx + 1) && (row == sy - 1 || row == sy + 1))
        *flags |= 2;
    else if ((col == sx && (row == sy - 2 || row == sy + 2)) || (row == sy && (col == sx - 2 || col == sx + 2)))
        *flags |= 4;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

struct nuhspecial_s;

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

struct nufpar_s;

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

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
    extern "C++" FadeSystem FadeSys;
}

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

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

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

// The original caller appears to treat this u16-returning function as i32;
// correcting this declaration lowers its match.

// Game-specific pre-load limits. These must be applied before WorldInfo
// reserves each level's fixed-size gameplay systems.

i32 InCollectList_Index(i32 id, COLLECTID *list, i32 count) {
    i32 i;
    COLLECTID *p;

    TempCollectID = NULL;

    if (list == NULL) {
        list = CollectList;
        count = CollectCount;
        if (list == NULL)
            return id;
    }

    p = list;
    for (i = 0; i < count; i++, p++) {
        if (p->id == id) {
            TempCollectID = p;
            return i;
        }
    }

    return -1;
}

void Collection_Configure(char *file, VARIPTR *bufferStart, VARIPTR *bufferEnd) {
    nufpar_s *fp = NuFParCreate(file);
    if (fp == NULL)
        return;
    CollectCount = 0;
    COLLECTID *collect = reinterpret_cast<COLLECTID *>(ALIGN(bufferStart->addr, 4));
    CollectList = collect;
    bufferStart->void_ptr = collect;
    COLLECTION_COMPLETIONCOUNT = 0;
    while (NuFParGetLine(fp) != 0) {
    read_word:
        if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "collect") == 0 && NuFParGetWord(fp) != 0) {
            const i16 id = CharIDFromName(fp->word_buf);
            LOG_DEBUG("Collection_Configure: Found collect id %s -> %d", fp->word_buf, id);
            collect->id = id;
            if (id == -1 || __builtin_expect(InCollectList_Index(id, CollectList, CollectCount) != -1, 0))
                continue;
            collect->type = 0;
            collect->field2_0x3 = 0xff;
            collect->can_buy = 0;
            collect->field3_0x4 = 0;
            collect->field6_0xa = 0;
            collect->field5_0x9 = 0;
            collect->cheat_code[0] = '\0';
            while (NuFParGetWord(fp) != 0) {
                if (NuStrICmp(fp->word_buf, "story") == 0) {
                    collect->type = 1;
                } else if (NuStrICmp(fp->word_buf, "area_complete") == 0) {
                    i32 area;
                    if (NuFParGetWord(fp) != 0 && Area_FindByName(fp->word_buf, &area) != NULL) {
                        collect->type = 2;
                        collect->field2_0x3 = static_cast<u8>(area);
                    }
                } else if (__builtin_expect(NuStrICmp(fp->word_buf, "all_episodes_complete") == 0, 1)) {
                    collect->type = 3;
                } else if (NuStrICmp(fp->word_buf, "in_pack") == 0) {
                    if (NuFParGetWord(fp) != 0) {
                        i32 pack = Store_FindPack(-1, fp->word_buf);
                        if (pack != -1) {
                            collect->type = 8;
                            collect->field2_0x3 = static_cast<u8>(pack);
                        }
                    }
                } else if (NuStrICmp(fp->word_buf, "100_percent") == 0) {
                    collect->type = 7;
                } else if (NuStrICmp(fp->word_buf, "gold_bricks") == 0) {
                    collect->type = 6;
                    const i32 value = NuFParGetInt(fp);
                    const u32 sign = static_cast<u32>(value >> 31);
                    collect->field6_0xa = static_cast<u16>((static_cast<u32>(value) ^ sign) - sign);
                } else if (NuStrICmp(fp->word_buf, "all_minikits_complete") == 0) {
                    collect->type = 4;
                } else if (NuStrICmp(fp->word_buf, "minikit") == 0) {
                    collect->type = 5;
                } else if (NuStrICmp(fp->word_buf, "buy_in_shop") == 0) {
                    collect->can_buy = 1;
                    collect->field3_0x4 = NuFParGetInt(fp);
                } else if (NuStrICmp(fp->word_buf, "cheat_code") == 0 && NuFParGetWord(fp) != 0 &&
                           NuStrLen(fp->word_buf) == 6) {
                    char *code = collect->cheat_code;
                    NuStrCpy(code, fp->word_buf);
                    NuStrUpr(code, code);
                }
            }
            if (__builtin_expect(collect->type == 0 ? collect->can_buy != 0 : collect->type != 8, 1) &&
                collect->type != 7) {
                collect->field5_0x9 = 1;
                ++COLLECTION_COMPLETIONCOUNT;
            }
            ++CollectCount;
            const i32 next_line = NuFParGetLine(fp);
            ++collect;
            if (next_line == 0)
                break;
            goto read_word;
        }
    }
    NuFParDestroy(fp);
    if (__builtin_expect(CollectCount < 1, 0))
        CollectList = NULL;
    else
        bufferStart->void_ptr = collect;
}

void Collection_CreateMaster(char *file, i16 *idlist, COLLECTION_s *collection, i32 param4, float param5) {
    collection->list = CollectList;
    collection->count_x = (u16)param4;
    collection->count_y = (u16)CollectCount;
    collection->field_c = file;
    collection->field_8 = idlist;
    collection->field_10 = param5;
}

void Collection_CreateCustom(char *name, i16 *id_list, COLLECTION_s *collection, u32 required_model_flags,
                             u32 excluded_model_flags, u32 required_game_flags, i32 require_buyable, i32 columns,
                             VARIPTR *buffer, VARIPTR *, i32 use_all_characters, f32 scale) {
    collection->count_x = static_cast<u16>(columns);
    collection->field_c = name;
    collection->field_8 = id_list;
    collection->list = reinterpret_cast<COLLECTID *>(ALIGN(buffer->addr, 4));
    collection->field_10 = scale;
    collection->count_y = 0;

    if (use_all_characters == 0) {
        if (CollectCount > 0) {
            COLLECTID *const end = CollectList + CollectCount;
            for (COLLECTID *current = CollectList; current != end; ++current) {
                COLLECTID &source = *current;
                const i32 id = source.id;
                if (__builtin_expect(id < 0, 0)) {
                    continue;
                }
                if (excluded_model_flags != 0 && (apicharsys->char_data[id].model_flags & excluded_model_flags) != 0) {
                    continue;
                }
                if (require_buyable != 0 && source.can_buy == 0) {
                    continue;
                }
                if (required_game_flags != 0 &&
                    (GCDataList[id].flags_090 & required_game_flags) != required_game_flags) {
                    continue;
                }
                if (!(required_model_flags != 0 &&
                      (CDataList[id].model_flags & required_model_flags) != required_model_flags)) {
                    collection->list[collection->count_y] = source;
                    ++collection->count_y;
                }
            }
        }
    } else {
        for (i32 id = 0; id < CHARCOUNT; ++id) {
            if (excluded_model_flags != 0 && (apicharsys->char_data[id].model_flags & excluded_model_flags) != 0) {
                continue;
            }
            if (required_game_flags != 0 && (GCDataList[id].flags_090 & required_game_flags) != required_game_flags) {
                continue;
            }
            if (!(required_model_flags != 0 &&
                  (CDataList[id].model_flags & required_model_flags) != required_model_flags)) {
                COLLECTID &entry = collection->list[collection->count_y];
                memset(&entry, 0, sizeof(entry));
                collection->list[collection->count_y].id = static_cast<i16>(id);
                ++collection->count_y;
            }
        }
    }

    buffer->void_ptr = collection->list + collection->count_y;
}

void Collection_GetPos(COLLECTION_s *collection, i32 id, float *x, float *y) {
    COLLECTID *list = collection->list;

    for (i32 i = 0; i < collection->count_y; i++) {
        if (list[i].id == id) {
            float *f = (float *)&list[i].cheat_code[8];
            *x = f[0];
            *y = f[1];
            return;
        }
    }
}

COLLECTID *CollectIDUnlocked(i32 id) {
    i32 index = InCollectList_Index(id, CollectList, CollectCount);
    if (index == -1) {
        return NULL;
    }
    if (Game_CharacterSave == NULL || (Game_CharacterSave[id] & SAVE_CHARACTER_UNLOCKED) == 0) {
        COLLECTID *entry = &CollectList[index];
        switch (entry->type) {
            case 0:
                break;
            case 2:
                if (__builtin_expect(static_cast<i8>(entry->field2_0x3) == -1 ||
                                         Game_AreaSave[static_cast<i8>(entry->field2_0x3)].area_complete == 0,
                                     1)) {
                    return NULL;
                }
                break;
            case 3:
                if (Episodes_Completed() != EPISODECOUNT) {
                    return NULL;
                }
                break;
            case 4:
                if (AllMiniKitsDone(Game_AreaSave) == 0) {
                    return NULL;
                }
                break;
            case 6:
                if (Game_CompletionSave == NULL ||
                    reinterpret_cast<STATUSCOLLECT_s *>(Game_CompletionSave)->gold_bricks < entry->field6_0xa) {
                    return NULL;
                }
                break;
            case 7:
                if (Game_100PercentComplete() == 0) {
                    return NULL;
                }
                break;
            case 8:
                if (__builtin_expect(!Store_IsPackUnlocked(static_cast<i8>(entry->field2_0x3)), 1)) {
                    return NULL;
                }
                break;
            default:
                return NULL;
        }
    }
    return &CollectList[index];
}

i32 Collection_Got(i32 id) {
    if (InCollectList_Index(id, NULL, 0) == -1) {
        return 0;
    }

    i32 area = AreaFromMiniKitID(id);
    if (area != -1) {
        if (Game_AreaSave == NULL) {
            return 0;
        }
        return Game_AreaSave[area].minikit_complete >= SAVE_COMPLETE ? 2 : 0;
    }

    if (Game_CharacterSave != NULL && (Game_CharacterSave[id] & SAVE_CHARACTER_AVAILABLE) == 0) {
        return 0;
    }
    return 1;
}

static inline f32 Collection_SelectedOpacity(void) {
    return NuTrigTable[(static_cast<i32>(GlobalTimer.time_elapsed_mod_seconds * 65536.0f) >> 1) & 0x7fff] * 0.125f +
           0.375f;
}

void Collection_Draw(COLLECTION_s *collection, float x, float y, float scale, APICHARACTERMODELLIST_s *models,
                     float alpha, i32 hide_selected) {
    const f32 base_dx = COLLECTION_DX;
    const f32 base_dy = COLLECTION_DY;
    const f32 base_size = COLLECTION_ICONSIZE;
    nuhspecial_s *special = collection_draw_hspecial;
    collection_draw_hspecial = NULL;
    i32 (*valid)(COLLECTION_s *, i32) = collection_draw_IsValidFn;
    collection_draw_IsValidFn = NULL;
    if (collection->list == NULL || FadeSys.fade > 0.0f)
        return;
    const i32 count = collection->count_y;
    const i32 columns = collection->count_x;
    if (columns == 0 || count == 0)
        return;
    f32 dx = base_dx * scale;
    f32 size = base_size * scale;
    const i32 rows = count % columns != 0 ? count / columns + 1 : count / columns;
    if (Game_OptionsSave != NULL && Game_OptionsSave->field11_0xb != 0) {
        dx *= 0.75f;
        size *= 0.875f;
    }
    i32 selected_x[2] = {-1, -1};
    i32 selected_y[2] = {-1, -1};
    if (models != NULL && Collection_GetSelectingPlayerIDsFn != NULL) {
        i16 ids[2] = {-1, -1};
        Collection_GetSelectingPlayerIDsFn(ids);
        if (ids[0] != -1 || ids[1] != -1) {
            i32 found = 0;
            for (i32 row = 0; row < rows; ++row)
                for (i32 col = 0; col < columns; ++col) {
                    const i32 index = row * columns + col;
                    if (__builtin_expect(found != 2, 1) && index < count &&
                        (collection->list[index].id == ids[0] || collection->list[index].id == ids[1])) {
                        selected_x[found] = col;
                        selected_y[found++] = row;
                    }
                }
        }
    }
    const f32 dy = base_dy * scale;
    collection->field_14 = dy;
    if (alpha > 1.0f)
        alpha = 1.0f;
    f32 py = y - static_cast<i32>(rows - 1) * dy * 0.5f;
    const f32 start_x = x - static_cast<i32>(columns - 1) * dx * 0.5f;
    for (i32 row = 0; row < rows; ++row) {
        f32 px = start_x;
        for (i32 col = 0; col < columns; ++col, px += dx) {
            const i32 index = row * columns + col;
            if (index >= count)
                continue;
            COLLECTID *entry = &collection->list[index];
            i32 id = entry->id;
            *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(entry) + 0x14) = px;
            *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(entry) + 0x18) = py;
            if (__builtin_expect(!(alpha > 0.0f), 0))
                continue;
            i32 frame = LEGOOBJ_ICON_FRAME_NEUTRAL;
            f32 opacity = 1.0f;
            i32 model_index;
            if (models != NULL && InModelList(models, id, &model_index) != 0) {
                if (__builtin_expect(hide_selected != 0, 0))
                    continue;
                opacity = 0.25f;
                if (__builtin_expect(model_index == 0, 1)) {
                    frame = LEGOOBJ_ICON_FRAME_BLUE;
                    opacity = Collection_SelectedOpacity();
                } else if (model_index == 1) {
                    frame = LEGOOBJ_ICON_FRAME_GREEN;
                    opacity = Collection_SelectedOpacity();
                }
            }
            const i32 got = valid != NULL ? valid(collection, index) : Collection_Got(id);
            i32 unlocked = 1;
            if (got == 0) {
                opacity *= 0.25f;
                unlocked = 0;
                id = -1;
            }
            opacity *= alpha;
            if (!(opacity > 0.0f))
                continue;
            u32 neighbours = 0;
            Collection_AddNeighbourFlags(&neighbours, col, row, selected_x[0], selected_y[0]);
            Collection_AddNeighbourFlags(&neighbours, col, row, selected_x[1], selected_y[1]);
            if (__builtin_expect((neighbours & 1) != 0, 0))
                opacity *= 0.333f;
            else if ((neighbours & 2) != 0)
                opacity *= 0.5f;
            else if ((neighbours & 4) != 0)
                opacity *= 0.666f;
            if (special != NULL) {
                drawcharicon_hspecial_spin =
                    static_cast<i32>((NuFmod(GameTimer.time_elapsed, 3.0f) / 3.0f) * 65536.0f) + index * 0x1555;
                DrawCharIcon(-1, px, py, 0.002f, size, frame, opacity, opacity, 1, unlocked != 0 ? special : NULL);
            } else
                DrawCharIcon(id, px, py, 0.002f, size, frame, opacity, opacity, 1, NULL);
        }
        py += dy;
    }
}

i32 Collection_GotAnyOfType(i32 type, u32 flags) {
    if (CollectList == NULL || CollectCount <= 0)
        return 0;
    for (i32 i = 0; i < CollectCount; ++i) {
        i32 id = CollectList[i].id;
        if ((type == -1 || (signed char)GCDataList[id].field275_0x116 == type) &&
            (flags == 0 || (CDataList[id].model_flags & flags) == flags) && Collection_Got(id))
            return 1;
    }
    return 0;
}

i32 AddToCollection(i32 id) {
    if (id > 0 && id < CHARCOUNT && InCollectList_Index(id, NULL, 0) != -1) {
        if (Collection_Got(id) == 0) {
            if (Game_CharacterSave != NULL)
                Game_CharacterSave[id] |= SAVE_CHARACTER_AVAILABLE | SAVE_CHARACTER_UNLOCKED;
            return 1;
        }
    }
    return 0;
}

void CollectAllCharacters(i32 only_story) {
    if (CollectCount <= 0)
        return;

    if (only_story == 0) {
        for (i32 i = 0; i < CollectCount; ++i) {
            if (CollectList[i].type != 8)
                AddToCollection(CollectList[i].id);
        }
    } else {
        for (i32 i = 0; i < CollectCount; ++i) {
            if (CollectList[i].type == 1)
                AddToCollection(CollectList[i].id);
        }
    }
}

i32 Collection_GetIDList(COLLECTION_s *collection, u32 model_flag_mask, u32 required_model_flags, i16 *ids,
                         i32 *first_id, i32 *second_id, i32 unused) {
    (void)unused;
    if (first_id != NULL) {
        *first_id = -1;
    }
    if (second_id != NULL) {
        *second_id = -1;
    }

    i32 result_count = 0;
    for (i32 index = 0; index < collection->count_y; ++index) {
        const i16 id = collection->list[index].id;
        if (__builtin_expect(
                (CDataList[id].model_flags & model_flag_mask) != required_model_flags || Collection_Got(id) == 0, 1)) {
            continue;
        }

        if (ids != NULL) {
            ids[result_count] = id;
        }
        ++result_count;

        if (first_id != NULL && *first_id == -1) {
            *first_id = id;
        } else if (second_id != NULL && *second_id == -1) {
            *second_id = id;
        }
    }

    if (ids != NULL) {
        ids[result_count] = -1;
    }
    return result_count;
}

i32 GameAudio_GetPlrSfxBits(void *object_ptr) {
    APIOBJECT *object = static_cast<APIOBJECT *>(object_ptr);
    i32 sfx_bits = 0;
    if (object != NULL && static_cast<i8>(object->flags_low) < 0) {
        sfx_bits = 1 << object->field_0x27c;
    }
    return sfx_bits;
}

