// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "globals.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/items/base/apiobject.h"
#include "nu2api/nucore/nustring.h"

#include <string.h>
#include "legoapi/core/input/qrand.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numusic/sfx.h"
#include <float.h>
#include <stdio.h>
#include "legoapi/audio/audio.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/numtx.h"
#include "legoapi/render/fx/game_deb.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/core/input/timer.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nusound/nusound.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include <stdlib.h>
#include "legoapi/core/input/gamepads.h"
#include "legoapi/world/mission.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nucamera.h"
#include <math.h>
#include "batman.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/render/fx/particles.h"
#include "legoapi/world/areas.h"
#include "legoapi/actions/character/streaks.h"
#include "gameapi/ai/aisys/aisys.h"
#include "nu2api/nu3d/numtl.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "gameframework/saveload.h"
#include "nu2api/nucore/nuvideo.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "legoapi/render/fx/edsplines.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include <new>
#include <stdint.h>
#include "nu2api/nucore/nulist.h"
#include "gameapi/ai/aisys/aiscript_types.h"
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
#include <stdarg.h>
#include <GLES2/gl2.h>
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/menus/screens/movies.h"
#include "legoapi/misc/androidbatman.h"
#include "legoapi/menus/core/gamemessage.h"
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

struct nupad_s;

struct CHEATSYSTEM {
    CHEAT *cheats;
    i32 cheats_count;
    i32 flags;
};
DECOMP_ASSERT(sizeof(CHEATSYSTEM) == 0x0c, "CHEATSYSTEM size");

// GCC 4.7 emits this source-defined BSS group in reverse declaration order.
// The declarations therefore reproduce POWERUP_TEXTID, CheatSystem and
// Cheat_PowerUpTime at the start of the original cheats.cpp BSS contribution.
static f32 Cheat_PowerUpTime;
static CHEATSYSTEM CheatSystem;
i32 POWERUP_TEXTID;

// The original initialized-data order is ONEPLAYERPOWERUPS followed by the
// duration used by both the per-player and shared power-up paths.
f32 CHEAT_POWERUPTIME = 20.0f;
i32 ONEPLAYERPOWERUPS = 1;

extern WORLDINFO WorldInfo[2];

extern i32 LevFlag[4];

extern i32 LevFlag[4]; // shared per-level 16-byte state scratch

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

struct GameObject_s;
struct nuvec_s;

struct EDCREATURE_s;

typedef isize __attribute__((may_alias)) NuGCutWord;

extern "C" {
    extern i16 id_LANDSPEEDER;
}

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
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

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

extern "C" i16 id_LANDSPEEDER;

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
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
    extern i16 id_LANDSPEEDER;
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
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" i16 id_LANDSPEEDER;

extern "C" {
    extern i16 id_MINIATST, id_MINIATTE;
}

extern i16 id_MINIATST, id_MINIATTE;

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
    struct EDCREATURE_s;
}

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

// Original writable flagremaptab at 0x668520, 32 pairs of 32-bit masks.
extern "C" {
extern u32 flagremaptab[32][2];
}

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

void Cheats_Init(CHEAT *cheats) {
    CheatSystem.cheats_count = 0;
    CheatSystem.cheats = cheats;
    if (cheats != NULL && cheats[0].name != NULL) {
        i32 count = 0;
        do {
            ++cheats;
            count++;
        } while (cheats->name != NULL);
        CheatSystem.cheats_count = count;
    }
}

i32 Cheat_FindByName(char *name) {
    for (i32 i = 0; i < CheatSystem.cheats_count; i++) {
        if (NuStrICmp(CheatSystem.cheats[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void Cheats_SetFlags() {
    i32 count = CheatSystem.cheats_count;
    CheatSystem.flags = 0;
    if (count > 0) {
        CHEAT *cheats = CheatSystem.cheats;
        i32 flags = 0;
        f32 powerup_time = Cheat_PowerUpTime;
        i32 vehicle_area = VehicleArea;
        for (i32 i = 0; i < count; i++) {
            if (cheats[i].enabled || (ONEPLAYERPOWERUPS == 0 && powerup_time > 0.0009765625f &&
                                      (cheats[i].flag & (vehicle_area ? 0x20000 : 0x10000)))) {
                flags |= cheats[i].flag;
            }
        }
        CheatSystem.flags = flags;
    }
}

u32 Cheats_CheckFlags(u32 flag) {
    return CheatSystem.flags & flag;
}

u32 Cheat_CheckFlags(i32 cheat_index, u32 flag_mask) {
    if (cheat_index > -1 && cheat_index < CheatSystem.cheats_count) {
        CHEAT target_cheat = CheatSystem.cheats[cheat_index];
        return flag_mask & target_cheat.flag;
    }
    return 0;
}

void Cheat_SetOn(i32 cheat, i32 on, i32) {
    if (cheat < 0 || cheat >= CheatSystem.cheats_count) {
        return;
    }
    CheatSystem.cheats[cheat].enabled = on != 0;
    Cheats_SetFlags();
}

i32 Cheat_IsOn(i32 cheat) {
    if (cheat >= 0 && cheat < CheatSystem.cheats_count) {
        if (CheatSystem.cheats[cheat].enabled != 0) {
            return 1;
        }
        if (ONEPLAYERPOWERUPS == 0 && Cheat_PowerUpTime > 0.0f) {
            if (VehicleArea != 0) {
                if (CheatSystem.cheats[cheat].flag & 0x20000) {
                    return 1;
                }
            } else if (CheatSystem.cheats[cheat].flag & 0x10000) {
                return 1;
            }
        }
    }
    return 0;
}

void Cheat_GetOnOffBitfield(i32 *onoffs, i32 count) {
    memset(onoffs, 0, ((count + 31) / 32) << 2);
    for (i32 i = 0; i < count; i++) {
        if (CheatSystem.cheats[i].enabled) {
            onoffs[i >> 5] |= 1u << (i & 31);
        }
    }
}

void Cheat_SetOnOffBitfield(i32 *onoffs, i32 count) {
    for (i32 i = 0; i < count; i++) {
        if ((onoffs[i >> 5] >> (i & 31)) & 1) {
            CheatSystem.cheats[i].enabled = 1;
        } else {
            CheatSystem.cheats[i].enabled = 0;
        }
    }
}

u32 Cheat_MultiplyScore(u32 score) {
    score = Cheats_CheckFlags(0x4) ? score * 2 : score;
    score = Cheats_CheckFlags(0x8) ? score * 4 : score;
    score = Cheats_CheckFlags(0x10) ? score * 6 : score;
    score = Cheats_CheckFlags(0x20) ? score * 8 : score;
    score = Cheats_CheckFlags(0x40) ? score * 10 : score;
    return score;
}

void Cheats_TurnOff(i32 cheat) {
    i32 count = CheatSystem.cheats_count;
    if (count > 0) {
        CHEAT *cheats = CheatSystem.cheats;
        if (cheat == 0) {
            for (i32 i = 0; i < count; i++) {
                cheats[i].enabled = 0;
            }
        } else {
            for (i32 i = 0; i < count; i++) {
                if (cheats[i].flag & 0x200000) {
                    cheats[i].enabled = 0;
                }
            }
        }
    }
    Cheats_SetFlags();
}

void Cheat_StartPowerUp(nuvec_s *position, GameObject_s *object) {
    if (ONEPLAYERPOWERUPS != 0) {
        if (object == NULL) {
            return;
        }
        object->field_0xdec = CHEAT_POWERUPTIME;
        void *controller = *reinterpret_cast<void **>(reinterpret_cast<char *>(object) + 0xc94);
        NewRumble(*reinterpret_cast<nupad_s **>(controller), 0.7f, 0);
        GameCam_HitRoll();
    } else {
        NewRumbleAllPlayers(0.7f, 0.0f, 0, 0);
        Cheat_PowerUpTime = CHEAT_POWERUPTIME;
    }

    void *display =
        AddGameMessage(TTab[POWERUP_TEXTID], position, 0.5f, position, 0.75f, 0xff, 0xff, 0xff, 0x4023, 1.0f);
    if (display != NULL) {
        *reinterpret_cast<f32 *>(static_cast<char *>(display) + 0xd4) = 0.75f;
    }
    display = AddGameMessage(TTab[POWERUP_TEXTID], position, 0.5f, position, 0.25f, 0xff, 0xff, 0xff, 0x4023, 1.0f);
    if (display != NULL) {
        *reinterpret_cast<f32 *>(static_cast<char *>(display) + 0xd4) = 0.75f;
    }
    GameAudio_PlaySfx(0x50, NULL, 0, 0);
}

i32 Cheat_PowerUpActive(i32 index) {
    if (ONEPLAYERPOWERUPS != 0) {
        if (static_cast<u32>(index) <= 1) {
            for (i32 i = 0; i < 2; ++i) {
                if (Player[i] != NULL && Player[i]->field_0xdec > 0.0009765625f &&
                    Player[i]->apiobj.field_0x27c == index) {
                    return 1;
                }
            }
        }
        return 0;
    }
    return Cheat_PowerUpTime > 0.0009765625f;
}

void Cheats_Reset() {
    Cheat_PowerUpTime = 0.0f;
    Cheats_SetFlags();
}

void Cheats_Update() {
    if (ONEPLAYERPOWERUPS == 0 && Cheat_PowerUpTime > 0.0f) {
        Cheat_PowerUpTime -= FRAMETIME;
        if (Cheat_PowerUpTime <= 0.0f) {
            GameAudio_PlaySfx(0x52, NULL, 0, 0);
        } else {
            GameAudio_PlaySfx(0x51, NULL, 0, 0);
            ConstantRumble(NULL, qrand() * 1.5259021893143654e-05f * 0.5f, 0.0f);
        }
    }
    Cheats_SetFlags();
}

void Cheat_SetArea(i32 cheat, i32 area_id) {
    if (cheat >= 0 && cheat < CheatSystem.cheats_count && area_id >= 0 && area_id < AREACOUNT) {
        CheatSystem.cheats[cheat].area = area_id;
    }
}

i32 GetAnimDirection(nuinstanim_s *animation) {
    if (animation == NULL || animation->tfactor == 0.0f) {
        return -1;
    }
    return !(animation->tfactor >= 0.0f) ? 1 : 0;
}

void DrawCross(nuvec_s *centre, float radius, numtl_s *material, i32 colour) {
    if (material != NULL) {
        colour = (((i32)(material->diffuse_color.b * 255.0f) & 255) << 16) |
                 (((i32)(material->diffuse_color.g * 255.0f) & 255) << 8) |
                 ((i32)(material->diffuse_color.r * 255.0f) & 255);
    }
    NuRndrLine3dDbg(centre->x - radius, centre->y, centre->z, centre->x + radius, centre->y, centre->z, colour);
    NuRndrLine3dDbg(centre->x, centre->y - radius, centre->z, centre->x, centre->y + radius, centre->z, colour);
    NuRndrLine3dDbg(centre->x, centre->y, centre->z - radius, centre->x, centre->y, centre->z + radius, colour);
}

