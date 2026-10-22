// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "globals.h"
#include "legoapi/audio/audio.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numusic/sfx.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/world/mission.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/light/surfaces.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nusound/nusound.h"
#include "nu2api/nufile/nufile.h"
#include <stdio.h>
#include <string.h>
#include "legoapi/audio/sfx_types.h"
#include "legoapi/characters/motion.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameframework/saveload.h"
#include "legoapi/world/world.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/props/doors/door.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/collect/torpedo.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edui.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/core/input/timer.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nuhgobj.h"
#include "legoapi/menus/screens/gamemenuall.h"
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
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include <new>
#include <stdlib.h>
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
#include "legoapi/render/core/gameliball.h"
#include "legoapi/render/fx/game_deb.h"
#include <stddef.h>
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
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
#include "legoapi/items/base/animpacket.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "gameapi/ai/aisys/aipath.h"
#include "nu2api/nucore/numechptr.hpp"
#include "legoapi/gizmo/base/GizBuildItObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizLeverObjectInterface.h"
#include "legoapi/gizmo/base/GizPanelObjectInterface.h"
#include "legoapi/gizmo/base/GizTurretObjectInterface.h"
#include "legoapi/gizmo/base/HatMachineObjectInterface.h"
#include "legoapi/gizmo/base/TeleportObjectInterface.h"
#include "nu2api/nu3d/nuhspecial.h"
#include "legoapi/world/levels/podrace.h"
#include <ctype.h>
#include "nu2api/nusound/nusound3_include.hpp"
#include "nu2api/nusound/nusound_buffer.hpp"
#include "nu2api/nusound/nusound_loader.hpp"
#include "nu2api/nusound/nusound_loader_ogg.hpp"
#include "nu2api/nusound/nusound_voice.hpp"
#include "nu2api/nusound/nusound3_include_types.h"

i32 (*GameAudio_ActionMusicFn)(void) = NULL;
static f32 sticky_attack_timeout[2] = {1.0f, 6.0f};
static i32 CurrentMusicPair_Quiet = -1;
static i32 last_currentmusicpair_quiet = -1;
static i32 sticky_attack;
static f32 sticky_attack_time;
static i32 MusicPlrsUnderAttack;
static i32 MusicActive = -1;
static i32 MusicActiveReset;
static f32 MusicSwapDelay;
static i32 MusicSeekIndex;
static i32 MusicInside;
static i32 MusicFindCut;
static i32 MusicRestoreTrack;
static i32 MusicOnFlag = 1;
static i32 MusicPlrsHoldAttack;
// The reference constructs the music manager in this unit's static constructor.
NuMusic music_man;

static i32 repsfxcount;
static RepeatSfx repsfxtab[32];
static i32 ticktock;

extern "C" {
    void (*ExtraDieSfxFn)(GameObject_s *);
    void (*ExtraHurtSfxFn)(GameObject_s *);
}

static GAMEAUDIO GameAudio_Default;
static GAMEAUDIO *GameAudio = &GameAudio_Default;

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

struct GameObject_s;
struct nuvec_s;
struct WORLDINFO_s;

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

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern i32 LevFlag[4];

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
    struct EDCREATURE_s;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

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

// The original focused the listener on the player object; the title screen
// runs before gameplay, where it is NULL and the focus stays disabled.
extern "C" {
    const char *audio_ps2_music_ext = ".vag";
}

DECOMP_ASSERT(sizeof(NuEList<NuSound3Voice>) == 0x1c, "NuSound3Voice list size");
extern NuSoundWeakPtr<NuSoundVoice> mVoice;
static i16 *ActionPairTab;
static i16 *AmbientPairTab;

__attribute__((optimize("O3"))) void GameAudio_Reset() {
    memset(&GameAudio_Default, 0, sizeof(GameAudio_Default));
    GameAudio = &GameAudio_Default;
    memset(GameAudio_Default.sfx_names, 0, sizeof(GameAudio_Default.sfx_names));
    for (i32 i = 0; i < 0x55; ++i) {
        GameAudio_Default.sfx_ids[i] = -1;
    }
}

void GameAudio_PlaySfxById(i32 sfx_id, nuvec_s *position, i32 flags, i32) {
    if (flags == 0) {
        PlaySfxById(sfx_id, position);
        return;
    }
    if ((flags & ~2) == 1) {
        nusound_special_positions[1] = *position;
        PlaySfxById(sfx_id, &nusound_special_positions[1]);
    }
    flags -= 2;
    if (static_cast<u32>(flags) <= 1) {
        nusound_special_positions[2] = *position;
        PlaySfxById(sfx_id, &nusound_special_positions[2]);
    }
}

void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 flags, i32 volume) {
    if ((u32)sfx < 0x55) {
        GameAudio_PlaySfxById(GameAudio->sfx_ids[sfx], position, flags, volume);
    }
}

void GameAudio_PlaySfxAndSetVolume(i32 sfx, nuvec_s *position, f32 volume) {
    if (static_cast<u32>(sfx) < 0x55) {
        PlaySfxByIdAndSetVolume(GameAudio->sfx_ids[sfx], position, volume);
    }
}

i32 GameAudio_GetSfxId(i32 sfx) {
    if (static_cast<u32>(sfx) <= 0x54) {
        return GameAudio->sfx_ids[sfx];
    }
    return -1;
}

void GameAudio_Init(GAMEAUDIO *audio) {
    GameAudio = audio;
    for (i32 i = 0; i < 0x55; ++i) {
        GAMEAUDIO *current_audio = GameAudio;
        current_audio->sfx_ids[i] = static_cast<i16>(GetSfxId(current_audio->sfx_names[i]));
    }

    MenuRegisterSoundFX(GameAudio_GetSfxId(0x2f), GameAudio_GetSfxId(0x30), GameAudio_GetSfxId(0x31),
                        GameAudio_GetSfxId(0x32));
}

void GameAudio_AddSfx(i32 sfx, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx) {
    if (sfx_count == NULL || sfx_ids == NULL || *sfx_count >= max_sfx) {
        return;
    }

    i32 sfx_id = GameAudio_GetSfxId(sfx);
    if (sfx_id == -1) {
        return;
    }
    for (i32 i = 0; i < *sfx_count; ++i) {
        if (sfx_ids[i] == sfx_id) {
            return;
        }
    }
    sfx_ids[*sfx_count] = sfx_id;
    ++*sfx_count;
}

void GameAudio_SetActionMusicTimes(f32 initial_delay, f32 hold_time) {
    sticky_attack_timeout[0] = initial_delay;
    sticky_attack_timeout[1] = hold_time;
}

void legoSetMusicVolume(float v) {
    // Original: NuMusic::SetClassVolume(0x23, v) — the mask covers QUIET (1),
    // ACTION (2) and the 0x20 theme class.
    music_man.SetClassVolume(0x23, v);
}

void legoSetCutVolume(float v) {
    music_man.SetClassVolume(TRACK_CLASS_CUTSCENE, v);
}

// Original: reads the user's music volume from the options save as the product
// of two 0..10 sliders scaled to 0..1 (option bytes at 0x4 and 0x5).
f32 GameGetMusicVolume(OPTIONSSAVE_s *options) {
    return ((f32)(u8)options->field5_0x5 / 10.0f) * ((f32)(u8)options->field4_0x4 / 10.0f);
}

// Original: applies GameGetMusicVolume, zeroing it while the title logos are
// up (SuperOptions.music_enabled == 0 on the titles level); the title menu restores
// the user's volume via GameGetMusicVolume once the menu phase starts.
f32 GameSetMusicVolume(OPTIONSSAVE_s *options) {
    f32 volume = GameGetMusicVolume(options);
    if (SuperOptions.music_enabled == 0 && WORLD->current_level == TITLES_LDATA) {
        volume = 0.0f;
    }
    legoSetMusicVolume(volume);
    return volume;
}

f32 GameSetSoundVolume(OPTIONSSAVE_s *options) {
    f32 volume = (static_cast<f32>(static_cast<u32>(static_cast<u8>(options->field3_0x3))) / 10.0f) *
                 (static_cast<f32>(static_cast<u32>(static_cast<u8>(options->field5_0x5))) / 10.0f);
    SetSoundVolume(volume);
    legoSetCutVolume(0.85f * static_cast<f32>(static_cast<u32>(static_cast<u8>(options->field5_0x5))) / 10.0f);
    return volume;
}

void ResetLevSfx(WORLDINFO *world) {
    for (i32 i = 0; i < 0x40; i++) {
        world->level_sfx[i].id = -1;
    }
    world->level_sfx_count = 0;
}

void AddLevSfx(WORLDINFO_s *world, nuvec_s *position, char *name, i32 sfx) {
    if (sfx == -1 && name != NULL) {
        sfx = GetSfxId(name);
    }
    if (sfx == -1) {
        return;
    }

    const i32 count = world->level_sfx_count;
    i32 index;
    LEVELSFXENTRY_s *entry = world->level_sfx;
    for (index = 0; index < count; ++index, ++entry) {
        if (entry->id == sfx)
            break;
    }
    if (index == count) {
        if (index >= 64) {
            return;
        }
        if (position != NULL) {
            world->level_sfx[index].position = *position;
        } else {
            world->level_sfx[index].position = nuvec_zero;
        }
        world->level_sfx[index].id = static_cast<i16>(sfx);
        world->level_sfx[index].references = 1;
        world->level_sfx_count = index + 1;
    } else if (world->level_sfx[index].references != -1) {
        ++world->level_sfx[index].references;
    }
}

void ChatterSfx(GameObject_s *g, i32 a, float b) {
    if (chattersfxwait <= 0.0f && ParticlesPerSecond(2.0f, FRAMETIME) > 0 && g->apiobj.field_0x287 == 0 &&
        g->character_context == -1) {
        if (a == last_chatter_sfx && b > 0.0f) {
            chattersfxwait = b;
            last_chatter_sfx = -1;
        } else {
            GameAudio_PlaySfxById(a, &g->apiobj.collision_position, 0, 0);
            const i32 random = qrand();
            chattersfxwait = static_cast<f32>(random) * (1.0f / 65535.0f) * 2.0f + 3.0f;
            last_chatter_sfx = a;
        }
    }
}

void LevChatterSfx(char *name, nuvec_s *position) {
    if (chattersfxwait <= 0.0f && qrand() < 0x400) {
        PlaySfx(name, position);
        const i32 random = qrand();
        chattersfxwait = static_cast<f32>(random) * (1.0f / 65535.0f) * 2.0f + 3.0f;
    }
}

void EngineNoiseCode(GameObject_s *object, i32 silent) {
    GAMECHARACTERDATA_s *character = object->apiobj.character_data->game_character;
    i32 sfx_id = character->sfx_engine;
    if (sfx_id == -1) {
        return;
    }

    f32 target = 0.0f;
    if (silent == 0) {
        f32 speed;
        if (object->apiobj.player_controlled) {
            speed = __builtin_fabsf(object->field_0xdc8);
        } else {
            speed = object->apiobj.velocity_magnitude / character->movement_speed;
        }
        target = 1.0f < speed ? 1.0f : speed;
    }

    f32 &engine_level = object->force_use_volume;
    engine_level = SeekLinearF(engine_level, target, FRAMETIME * 0.5f);
    f32 volume = engine_level * 0.5f + 0.5f;
    if (!object->apiobj.player_controlled) {
        volume *= 0.6f;
    }
    f32 pitch = engine_level * 0.4f + 0.6f;
    pitch += (static_cast<f32>(static_cast<i32>(object->apiobj.field_0x289)) / 63.0f) * 0.05f - 0.025f;
    f32 variation = static_cast<f32>(qrand()) * 1.5259022e-5f * 0.03f + 0.985f;
    pitch *= variation;
    PlaySfxByIdAndSetVolumeAndPitch(sfx_id, &object->apiobj.collision_position, volume, pitch);
}

void PlayJumpSfx(GameObject_s *object, i32 type) {
    i32 sfx;
    const u32 flags = object->apiobj.character_data->model_flags;
    if ((flags & 0x40) != 0) {
        sfx = GameAudio->sfx_ids[1];
    } else if ((flags & 8) != 0) {
        switch (type) {
            case 0:
                sfx = GameAudio->sfx_ids[2];
                break;
            case 1:
                sfx = GameAudio->sfx_ids[3];
                break;
            case 2:
                sfx = GameAudio->sfx_ids[6];
                break;
            case 3:
                sfx = GameAudio->sfx_ids[4];
                break;
            case 4:
                sfx = GameAudio->sfx_ids[5];
                break;
            default:
                return;
        }
    } else {
        sfx = GameAudio->sfx_ids[0];
    }
    if (sfx != -1) {
        if (!object->apiobj.player_controlled && !object->is_boss) {
            PlaySfxByIdAndSetVolume(sfx, &object->apiobj.lower_position, 0.5f);
        } else {
            GameAudio_PlaySfxById(sfx, &object->apiobj.lower_position, 0, 1);
        }
    }
}

void PlayLandSfx(GameObject_s *object, i32 type, i32) {
    i32 sfx;
    if (type == 1) {
        if ((object->apiobj.character_data->model_flags & 8) == 0) {
            return;
        }
        sfx = GameAudio->sfx_ids[0xb];
    } else if (type == 2) {
        sfx = GameAudio->sfx_ids[0xc];
    } else if (type == 3) {
        sfx = GameAudio->sfx_ids[0xd];
    } else if (type == 4) {
        sfx = GameAudio->sfx_ids[0xe];
    } else {
        if (object->apiobj.is_underwater != 0 || object->apiobj.intersects_water != 0) {
            return;
        }
        const i32 alternate = WorldInfo_CurrentlyActive()->current_level->flags & 0x1000;
        if ((object->apiobj.character_data->model_flags & 0x10) == 0) {
            if (type != 0) {
                return;
            }
            if (alternate != 0) {
                sfx = GameAudio->sfx_ids[8];
            } else {
                sfx = GameAudio->sfx_ids[7];
            }
        } else {
            if (alternate != 0) {
                sfx = GameAudio->sfx_ids[10];
            } else {
                sfx = GameAudio->sfx_ids[9];
            }
        }
    }
    if (sfx != -1) {
        if (static_cast<i8>(object->apiobj.flags_low) < 0 || object->is_boss) {
            GameAudio_PlaySfxById(sfx, &object->apiobj.lower_position, 0, 1);
        } else {
            PlaySfxByIdAndSetVolume(sfx, &object->apiobj.lower_position, 0.5f);
        }
    }
}

void PlayFootStepSfx(GameObject_s *object) {
    i32 on_platform = 1;
    if (object->field_0x1078 == -1 && object->apiobj.supporting_platform_id == -1) {
        const i8 surface = static_cast<i8>(object->apiobj.field_0x281);
        if (surface == -1 || (TerSurface[surface].flags & 2) == 0) {
            on_platform = 0;
        }
    }

    i32 sfx;
    if (object->apiobj.character_data->game_character->sfx_footstep != -1) {
        sfx = object->apiobj.character_data->game_character->sfx_footstep;
    } else {
        sfx = -1;
        if (GameAudio->override_footstep_fn != NULL) {
            sfx = GameAudio->override_footstep_fn(object, on_platform);
        }
        if (sfx == -1) {
            if (object->apiobj.is_underwater == 0 && object->apiobj.intersects_water == 0) {
                const i32 alternate = WorldInfo_CurrentlyActive()->current_level->flags & 0x1000;
                if ((object->apiobj.character_data->model_flags & 0x10) != 0) {
                    if (alternate != 0) {
                        sfx = GameAudio->sfx_ids[0x12];
                    } else {
                        sfx = GameAudio->sfx_ids[0x11];
                    }
                } else {
                    if (alternate != 0) {
                        sfx = GameAudio->sfx_ids[0x10];
                    } else {
                        sfx = GameAudio->sfx_ids[0xf];
                    }
                }
            } else {
                sfx = GameAudio->sfx_ids[0x13];
            }
        }
    }

    if (sfx != -1) {
        if (object->apiobj.player_controlled || object->is_boss) {
            GameAudio_PlaySfxById(sfx, &object->apiobj.lower_position, 0, 0);
        } else {
            PlaySfxByIdAndSetVolume(sfx, &object->apiobj.lower_position, 0.5f);
        }
    }

    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if (packet.blending != 0 || packet.animation_index == -1)
        return;
    CHARACTERANIM_s *config =
        static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[packet.animation_index]);
    if (config == NULL)
        return;
    if ((config->flags & 0x20000) != 0) {
        GameCam_Judder(GameCam, -0.25f, 0, &object->apiobj.collision_position);
        NewRumbleAllPlayers(0.6f, 0.0f, 0, 0);
    } else if ((config->flags & 0x40000) != 0 && VehicleArea == 0) {
        GameCam_NewShake(GameCam, 0.5f, 0.5f, 1.0f);
        NewRumbleAllPlayers(0.5f, 0.0f, 0, 0);
    }
}

void PlayGruntSfx(GameObject_s *object) {
    CHARACTERDATA *character = object->apiobj.character_data;
    const i16 configured_sfx = character->game_character->sfx_grunt;
    i32 sfx = configured_sfx;
    if (configured_sfx == -1) {
        if ((character->model_flags & 0x44002010) != 0) {
            return;
        }
        if ((object->field_0xf01 & 8) != 0) {
            sfx = GameAudio->sfx_ids[0x14];
        } else {
            sfx = GameAudio->sfx_ids[0x15];
        }
    }
    if (sfx != -1) {
        GameAudio_PlaySfxById(sfx, &object->apiobj.collision_position, 0, 0);
    }
}

void PlayHurtSfx(GameObject_s *object) {
    CHARACTERDATA *character = object->apiobj.character_data;
    i32 sfx;
    {
        const i16 configured_sfx = character->game_character->sfx_hurt;
        if (configured_sfx != -1) {
            sfx = configured_sfx;
            goto selected;
        }
    }
    if ((character->model_flags & 0x44002010) != 0) {
        goto extra;
    }
    if ((object->field_0xf01 & 8) != 0) {
        sfx = GameAudio->sfx_ids[0x16];
    } else {
        sfx = GameAudio->sfx_ids[0x17];
    }
selected:
    if (sfx == -1) {
        goto extra;
    }
    GameAudio_PlaySfxById(sfx, &object->apiobj.collision_position, 0, 0);
extra:
    auto extra_sfx = ExtraHurtSfxFn;
    if (extra_sfx != NULL) {
        extra_sfx(object);
    }
}

void PlayDieSfx(GameObject_s *object) {
    CHARACTERDATA *character = object->apiobj.character_data;
    GAMECHARACTERDATA_s *config = character->game_character;
    i32 sfx;
    {
        const i16 configured_sfx = config->sfx_die;
        if (configured_sfx != -1) {
            sfx = configured_sfx;
            goto play;
        }
    }
    {
        sfx = -1;
        if ((config->flags_090 & 0x800) != 0) {
            sfx = GameAudio->sfx_ids[0x18];
        } else {
            const u32 flags = character->model_flags;
            if ((flags & 0x2000) != 0) {
                sfx = GameAudio->sfx_ids[0x18];
            } else if ((flags & 0x04000000) != 0) {
                sfx = GameAudio->sfx_ids[0x19];
            } else if ((flags & 0x10) != 0) {
                sfx = GameAudio->sfx_ids[0x1a];
            } else if ((flags & 0x40000000) == 0) {
                if ((object->field_0xf01 & 8) != 0) {
                    sfx = GameAudio->sfx_ids[0x1b];
                } else {
                    sfx = GameAudio->sfx_ids[0x1c];
                }
            }
        }
        if (sfx == -1) {
            goto extra;
        }
    }
play:
    GameAudio_PlaySfxById(sfx, &object->apiobj.collision_position, 0, 0);
extra:
    auto extra_sfx = ExtraDieSfxFn;
    if (extra_sfx != NULL) {
        extra_sfx(object);
    }
}

void PlaySabreSfx(char *name, GameObject_s *object, nuvec_s *position, i32) {
    const u8 player = static_cast<u8>(object->apiobj.field_0x27c);
    if (player == 0xff && WORLD->rooms_visible_ptr[object->room_id] == 0) {
        return;
    }
    u32 player_bits = 0;
    if (object->apiobj.player_controlled) {
        player_bits = 1u << (player & 0x1f);
    }
    if (name == NULL) {
        const i32 sfx = object->apiobj.character_data->game_character->sfx_sabre;
        if (sfx == -1) {
            GameAudio_PlaySfx(0x40, &object->apiobj.collision_position, 0, 1);
        } else {
            GameAudio_PlaySfxById(sfx, &object->apiobj.collision_position, player_bits, 1);
        }
    } else {
        const i32 named_sfx = GetSfxId(name);
        if (position == NULL) {
            position = &object->apiobj.collision_position;
        }
        GameAudio_PlaySfxById(named_sfx, position, 0, 1);
    }
}

void AddLevelSfxFromName(char *sfx_name, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx_count) {
    if (sfx_count == NULL || sfx_ids == NULL || *sfx_count >= max_sfx_count) {
        return;
    }

    i32 sfx_id = GetSfxId(sfx_name);
    if (sfx_id == -1) {
        return;
    }

    for (i32 i = 0; i < *sfx_count; i++) {
        if (sfx_ids[i] == sfx_id) {
            return;
        }
    }

    sfx_ids[*sfx_count] = sfx_id;
    ++*sfx_count;
}

void AddLevelSfxFromId(i32 sfx_id, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx) {
    if (sfx_count == NULL || sfx_ids == NULL || *sfx_count >= max_sfx || sfx_id == -1) {
        return;
    }
    for (i32 index = 0; index < *sfx_count; ++index) {
        if (sfx_ids[index] == sfx_id) {
            return;
        }
    }
    sfx_ids[*sfx_count] = sfx_id;
    ++*sfx_count;
}

void UpdateLevelSfx(WORLDINFO_s *world, i32 paused) {
    SfxBitsRestore(&CurrentSFXTAB);

    if (paused != 0) {
        goto disable_reverb_and_return;
    }
    if (GameAudio->check_reverb_fn != NULL) {
        if (GameAudio->check_reverb_fn() != 0) {
            goto enable_reverb;
        }
    }
    NuSound3SetReverb(0);

update_ambient:
    if (CUTSTOPGAME == 0 && world->current_level->unknown_0a2 != -1) {
        GameAudio_PlaySfxById(world->current_level->unknown_0a2, NULL, 0, 0);
    }
    return;

disable_reverb_and_return:
    NuSound3SetReverb(0);
    return;

enable_reverb:
    NuSound3SetReverb(1);
    goto update_ambient;
}

void TickTockSfx() {
    if (ticktock != 0) {
        GameAudio_PlaySfx(0x1d, NULL, 0, 0);
        ticktock = 0;
    } else {
        GameAudio_PlaySfx(0x1e, NULL, 0, 0);
        ticktock = 1;
    }
}

void ResetRepeatSfx() {
    repsfxcount = 0;
    memset(repsfxtab, 0, sizeof(repsfxtab));
    repsfxtab[0].sfx_id = -1;
}

void PlayRepeatSfx(char *name, i32 sfx_id, f32 initial_delay, char play_count, f32 interval, nuvec_s *position) {
    if (play_count == 1 && initial_delay == 0.0f) {
        if (sfx_id == -1) {
            PlaySfx(name, position);
        } else {
            GameAudio_PlaySfxById(sfx_id, position, 0, 0);
        }
        return;
    }

    const i32 sound_slot = repsfxcount;
    repsfxtab[sound_slot].state = REPEAT_SFX_PLAY;
    if (initial_delay > 0.0f) {
        repsfxtab[sound_slot].state = REPEAT_SFX_INITIAL_DELAY;
    }

    if (sfx_id == -1)
        sfx_id = GetSfxId(name);

    repsfxtab[sound_slot].sfx_id = static_cast<i16>(sfx_id);
    RepeatSfx &repeat = repsfxtab[repsfxcount];
    repsfxcount = (repsfxcount + 1) & 31;
    repeat.timer = initial_delay;
    repeat.plays_remaining = play_count;
    repeat.interval = interval;
    repeat.position = position;
}

void UpdateRepeatSfx() {
    for (RepeatSfx &repeat : repsfxtab) {
        switch (repeat.state) {
            case REPEAT_SFX_INITIAL_DELAY:
                if (repeat.timer > 0.0f) {
                    repeat.timer -= FRAMETIME;
                } else {
                    repeat.state = REPEAT_SFX_PLAY;
                }
                break;

            case REPEAT_SFX_PLAY:
                GameAudio_PlaySfxById(repeat.sfx_id, repeat.position, 0, 0);
                --repeat.plays_remaining;
                if (repeat.plays_remaining > 0) {
                    repeat.timer = repeat.interval;
                    repeat.state = REPEAT_SFX_INTERVAL;
                } else {
                    memset(&repeat, 0, sizeof(repeat));
                }
                break;

            case REPEAT_SFX_INTERVAL:
                if (repeat.timer > 0.0f) {
                    repeat.timer -= FRAMETIME;
                } else {
                    repeat.state = REPEAT_SFX_PLAY;
                }
                break;

            default:
                break;
        }
    }
}

void SetSoundFadeDist(WORLDINFO_s *world, OPTIONSSAVE_s *options) {
    options->field3_0x3 = 8;
    options->field4_0x4 = 6;
    if (SetSoundFadeDistCallBackFn != NULL && SetSoundFadeDistCallBackFn(world) != 0) {
        GameSetSoundVolume(options);
        GameSetMusicVolume(options);
        return;
    }
    if (VehicleArea == 0) {
        nusound_fade_start = 2.0f;
        nusound_fade_end = 15.0f;
    } else {
        nusound_fade_start = 10.0f;
        nusound_fade_end = 80.0f;
    }
    GameSetSoundVolume(options);
    GameSetMusicVolume(options);
}

void NewAreaMusicChanges(void) {
    BeenAttacked = 0;
}

i32 ActionFromQuiet(i32 idx) {
    if (idx != -1) {
        for (i32 entry = 0; ActionPairTab[entry * 14] != -1; entry++) {
            if (ActionPairTab[entry * 14] == idx) {
                return ActionPairTab[entry * 14 + 1];
            }
        }
    }
    return -1;
}

i32 AmbientFromQuiet(i32 idx) {
    if (idx != -1) {
        for (i32 entry = 0; AmbientPairTab[entry * 2] != -1; entry++) {
            if (AmbientPairTab[entry * 2] == idx) {
                return AmbientPairTab[entry * 2 + 1];
            }
        }
    }
    return -1;
}

i16 GetMusicIndex(char *name, nusound_filename_info_s *table, i32 def) {
    (void)name;
    (void)table;
    (void)def;
    return -1;
}

nusound_filename_info_s *ConfigureMusic(char *file, VARIPTR *bufferStart, VARIPTR *bufferEnd) {
    (void)file;
    nusound_filename_info_s *finfo;

    music_man.Initialise("audio\\music.cfg", NULL, bufferStart, *bufferEnd);
    music_man.GetSoundFiles(&finfo, NULL);

    audio_ps2_music_ext = ".mib";

    u8 *music_config = reinterpret_cast<u8 *>(ALIGN(bufferStart->addr, 4));
    *reinterpret_cast<i32 *>(music_config) = 0;
    ActionPairTab = reinterpret_cast<i16 *>(music_config + 0x20);
    *ActionPairTab = -1;
    AmbientPairTab = reinterpret_cast<i16 *>(music_config + 0x3c);
    *AmbientPairTab = -1;
    bufferStart->void_ptr = music_config + 0x40;

    return finfo;
}

i32 GamePlayMusic(LEVELDATA_s *level, i32 check, OPTIONSSAVE_s *options) {
    (void)options;
    i32 other = MusicOther;
    last_currentmusicpair_quiet = CurrentMusicPair_Quiet;
    MusicOther = 0;
    if (NOSOUND != 0) {
        last_currentmusicpair_quiet = CurrentMusicPair_Quiet;
        MusicOther = 0;
        return NOSOUND;
    }
    if (NOMUSIC != 0) {
        last_currentmusicpair_quiet = CurrentMusicPair_Quiet;
        MusicOther = 0;
        return NOMUSIC;
    }

    i32 music_other = 0;
    if (CheckMusicOtherFn != NULL) {
        music_other = CheckMusicOtherFn();
    }

    MusicOther = music_other;

    if (check != 0) {
        if (music_other == other)
            return music_other;
    } else {
        sticky_attack_time = 0;
        sticky_attack = PlayersUnderAttack;
    }

    // music_tracks is [class][pair]: the quiet slot of the selected pair,
    // plus the action/ambient slots of the MusicOther pair.
    music_man.SelectTrackByHandle(TRACK_CLASS_QUIET, level->music_tracks[0][music_other]);
    music_man.SelectTrackByHandle(TRACK_CLASS_ACTION, level->music_tracks[1][MusicOther]);
    music_man.SelectTrackByHandle(TRACK_CLASS_NOMUSIC, level->music_tracks[2][MusicOther]);

    if (SuperOptions.music_enabled == 0) {
        return music_man.PlayTrack(TRACK_CLASS_NOMUSIC);
    }

    if (sticky_attack != 0) {
        if (music_man.GetTrackHandle(TRACK_CLASS_ACTION, NULL) != -1) {
            return music_man.PlayTrack(TRACK_CLASS_ACTION);
        }
        if (music_man.GetTrackHandle(TRACK_CLASS_QUIET, NULL) != -1) {
            return music_man.PlayTrack(TRACK_CLASS_QUIET);
        }
        return music_man.PlayTrack(TRACK_CLASS_NOMUSIC);
    } else {
        if (music_man.GetTrackHandle(TRACK_CLASS_QUIET, NULL) != -1) {
            return music_man.PlayTrack(TRACK_CLASS_QUIET);
        }
        if (music_man.GetTrackHandle(TRACK_CLASS_ACTION, NULL) != -1) {
            return music_man.PlayTrack(TRACK_CLASS_ACTION);
        }
        return music_man.PlayTrack(TRACK_CLASS_NOMUSIC);
    }
}

i32 CheckMusicSwapInstant() {
    // Original Android build is a platform placeholder returning false.
    return 0;
}

void ProcessMusicChanges(LEVELDATA_s *level, OPTIONSSAVE_s *opts) {
    (void)opts;

    if (GameAudio_ActionMusicFn == NULL) {
        const auto player_under_attack = [](GameObject_s *player) {
            if (player == NULL)
                return false;
            if (player->ai.opponent != NULL)
                return true;
            GameObject_s *opponent = static_cast<GameObject_s *>(player->ai.nearest_opponent);
            return opponent != NULL && opponent->apiobj.field_0x287 == 0 && player->ai.nearest_opponent_metric < 3.0f;
        };
        if (DoubleScore != 0 || Cheat_PowerUpActive(-1) != 0 || player_under_attack(Player[0]) ||
            player_under_attack(Player[1]))
            PlayersUnderAttack = 1;
        else
            PlayersUnderAttack = 0;
    } else {
        PlayersUnderAttack = GameAudio_ActionMusicFn();
    }

    if (CheckMusicOtherFn != NULL) {
        MusicOther = CheckMusicOtherFn();
    } else {
        MusicOther = 0;
    }

    if (sticky_attack != PlayersUnderAttack) {
        if (Paused == 0) {
            sticky_attack_time -= FRAMETIME;
        }
        if (sticky_attack_time <= 0.0f) {
            sticky_attack = PlayersUnderAttack;
            sticky_attack_time = sticky_attack_timeout[sticky_attack];
        }
    }

    CUTINFO *cut_stop = static_cast<CUTINFO *>(CutStopInfo);
    if (cut_stop != NULL && cut_stop->skip_level != -1) {
        if ((LDataList[cut_stop->skip_level].flags & LEVEL_CONFIG_LOADED) != 0) {
            level = &LDataList[cut_stop->skip_level];
        }
    }

    music_man.SelectTrackByHandle(TRACK_CLASS_QUIET, level->music_tracks[0][MusicOther]);
    music_man.SelectTrackByHandle(TRACK_CLASS_ACTION, level->music_tracks[1][MusicOther]);
    music_man.SelectTrackByHandle(TRACK_CLASS_NOMUSIC, level->music_tracks[2][MusicOther]);

    if (SuperOptions.music_enabled == 0) {
        music_man.PlayTrack(TRACK_CLASS_NOMUSIC);
    } else if (sticky_attack != 0) {
        if (music_man.GetTrackHandle(TRACK_CLASS_ACTION, NULL) != -1)
            music_man.PlayTrack(TRACK_CLASS_ACTION);
        else if (music_man.GetTrackHandle(TRACK_CLASS_QUIET, NULL) != -1)
            music_man.PlayTrack(TRACK_CLASS_QUIET);
        else
            music_man.PlayTrack(TRACK_CLASS_NOMUSIC);
    } else {
        if (music_man.GetTrackHandle(TRACK_CLASS_QUIET, NULL) != -1)
            music_man.PlayTrack(TRACK_CLASS_QUIET);
        else if (music_man.GetTrackHandle(TRACK_CLASS_ACTION, NULL) != -1)
            music_man.PlayTrack(TRACK_CLASS_ACTION);
        else
            music_man.PlayTrack(TRACK_CLASS_NOMUSIC);
    }

    music_man.Process(FRAMETIME);
}

void MusicClearAll() {
    MusicPlrsUnderAttack = 0;
    MusicActive = -1;
    sticky_attack_time = 0.0f;
    MusicActiveReset = 0;
    MusicSwapDelay = 0.0f;
    MusicSeekIndex = 0;
    PlayersUnderAttack = 0;
    MusicInside = 0;
    MusicFindCut = 0;
    MusicRestoreTrack = 0;
    MusicOnFlag = 1;
    MusicPlrsHoldAttack = 0;
    CurrentMusicPair_Quiet = -1;
}

void SfxCheckMusicOnOff(OPTIONSSAVE_s *) {
    // Platform placeholder in the Android build.
}

void UpdateBackgroundMusic() {
}

void SetBackgroundMusic(i32 track) {
    // libTTapp.so 0x4df8b0: platform stub (eight NOPs and ret).
    // Title music is selected and started later by GamePlayMusic.
    (void)track;
}

