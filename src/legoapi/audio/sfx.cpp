// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "legoapi/world/world_shared.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/props/system/socksys.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nusound/nusound.h"
#include "nu2api/nufile/nufile.h"
#include "decomp.h"

#include <stdio.h>
#include <string.h>

#include "legoapi/items/base/apiobject.h"
#include "legoapi/characters/motion.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/gui/apimenu.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/world/level.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/items/collect/torpedo.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edui.h"
#include "gamelib/util/gamelib_util_types.h"
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
#include "nu2api/nucore/nugcutscene.h"
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
#include <stdlib.h>
#include "legoapi/gizmos/trigger/ai.h"
#include <stdint.h>
#include "nu2api/nucore/nulist.h"
#include "gameapi/ai/aisys/aiscript_types.h"
#include <float.h>
#include "editor/edpath.h"
#include "legoapi/actions/character/speederchase.h"
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
#include "legoapi/audio/specialsfx.h"
#include "legoapi/items/fx/pulses.h"

extern "C" {
    SoundTable CurrentSFXTAB;
    i32 CruiserD_LiftChase;
}

extern "C" i16 id_GAMORREANGUARD;

extern "C" {
    f32 sfx_wait;
} // extern "C"

void AddFootSteps(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if (packet.blending != 0 || packet.animation_index == -1)
        return;
    CHARACTERMODEL_s *model = object->apiobj.character_model;
    const i32 animation = packet.animation_index;
    if (model->model_data_b[animation] == NULL)
        return;
    CHARACTERANIM_s *config = static_cast<CHARACTERANIM_s *>(model->model_data_a[animation]);
    if ((config->flags & CHARACTER_ANIMATION_FLAG_FOOTSTEPS) == 0 || packet.previous_time == packet.current_time)
        return;
    const i32 advancing = (packet.flags & ANIMPACKET_FLAG_LOOPED) != 0 ? packet.previous_time > packet.current_time
                                                                       : packet.current_time > packet.previous_time;
    bool crossed = false;
    if ((config->flags & 0x10000) == 0) {
        if (!(packet.previous_time > packet.current_time)) {
            if (!advancing)
                return;
            for (i32 i = 2; i < 4; ++i) {
                const f32 frame = config->event_frames[i];
                if (frame >= 1.0f && packet.current_time >= frame && frame > packet.previous_time)
                    crossed = true;
            }
        } else {
            if (!advancing)
                return;
            for (i32 i = 2; i < 4; ++i) {
                const f32 frame = config->event_frames[i];
                if (frame >= 1.0f && (packet.current_time >= frame || frame > packet.previous_time))
                    crossed = true;
            }
        }
    } else {
        if (!advancing)
            return;
        if (!(packet.previous_time > packet.current_time)) {
            for (i32 i = 0; i < 4; ++i) {
                const f32 frame = config->event_frames[i];
                if (frame >= 1.0f && packet.current_time >= frame && frame > packet.previous_time)
                    crossed = true;
            }
        } else {
            for (i32 i = 0; i < 4; ++i) {
                const f32 frame = config->event_frames[i];
                if (!(frame < 1.0f) && (packet.current_time >= frame || frame > packet.previous_time))
                    crossed = true;
            }
        }
    }
    if (crossed)
        PlayFootStepSfx(object);
}

static i32 GameAudio_CheckReverb_LSW();
static inline i32 GameAudio_OverrideFootStep_LSW(GameObject_s *object, i32 alternate);

GAMEAUDIO GameAudio_LSW = {
    GameAudio_OverrideFootStep_LSW,
    GameAudio_CheckReverb_LSW,
    {
        "Jp_Ami_Jump",
        NULL,
        "Jp_OQ_Jump",
        "Jp_OQ_DJump",
        "JumpLunge",
        "JumpSlam",
        "Jp_OQ_BackF",
        "FS_JLandS",
        "FS_JLandM",
        "FS_DLandS",
        "FS_DLandM",
        "JediLunge",
        "JediSlam",
        "Grv_GrievAngryJump",
        NULL,
        "FS_JWalkS",
        "FS_JWalkM",
        "FS_DWalkS",
        "FS_DWalkM",
        "FS_WaterWade",
        "Leia_grunt",
        "Rebel_grunt",
        "Leia_Hurt",
        "Luke_Hurt",
        "Explode1",
        "exp_thermalDet",
        "DroidDie01",
        "Leia_Death",
        "Luke_Death",
        "CountdownTimerTick",
        "CountdownTimerTock",
        "SwChar",
        NULL,
        "SwChar",
        "SwChar",
        "ToggleChar",
        "ToggleChar",
        "MK-Loop",
        "MK-Panel",
        "PickupHeart",
        "BlastHit",
        "BlastRic",
        "SabDefBlas",
        "StatusAward",
        "TrueJedi_100pc",
        "WipeScreen",
        "NegC_MagnetCoil",
        "MenuMove",
        "MenuSelect",
        "MenuBack",
        "MenuNoEntry",
        "Explode1",
        "imp_punch",
        "ui_DoubleScoreText",
        "MenuSelect",
        "MenuBack",
        "fly_paddle_rotate_lp",
        "fly_paddle_stuck",
        "MK-Pickup",
        "LegoForm",
        "LegoSingle",
        "SaberOff",
        "SaberOn",
        "SaberSaber01",
        "SaberMove01",
        "SaberSmack01",
        "BlasterHolster",
        "BlasterDraw",
        NULL,
        NULL,
        "FS_WaterOut",
        "Block_Shove",
        "Block_Push_Lp",
        "Blaster",
        "wpn_punch",
        "Honk",
        "WhipSwish",
        "WhipHit",
        "WhipNowt",
        "FallApart01",
        "PowerUp_On",
        "PowerUp_Loop",
        "PowerUp_Off",
        "env_padLight_on",
        "Shop_BuyCheat",
    },
    {},
};

extern "C" i16 id_GAMORREANGUARD;

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

extern "C" i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER;

extern i16 tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);

extern "C" i16 id_ANAKINJEDISCARRED, id_WOOKIEE, id_TWOFACE;

struct GameObject_s;
struct nuvec_s;
struct BOLT_s;
struct debinftype;

extern "C" {
    extern i16 id_GAMORREANGUARD;
}

struct EDCREATURE_s;
struct APIOBJECT_s;

extern "C" {
extern i16 id_GAMORREANGUARD;

}

extern void (*CutScene_OverrideConfigFileNameFn)(char *, i32, i32);

typedef isize __attribute__((may_alias)) NuGCutWord;

extern i16 id_GAMORREANGUARD;

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER;

extern AIACTIONDEF api_aiactiondefs[55];
extern AICONDITIONDEF api_aiconditiondefs[50];

DECOMP_ASSERT(sizeof(api_aiactiondefs) == 0x294, "API action registry size");

DECOMP_ASSERT(sizeof(api_aiconditiondefs) == 0x258, "API condition registry size");

extern "C" {
extern void (*checkantinodefns[3])(APIOBJECT *, AIANTINODE *, NUVEC *, f32);
    extern void (*checkantinodefns[3])(APIOBJECT_s *, AIANTINODE_s *, NUVEC *, f32);
}

struct AIPATH_s;
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

extern i16 id_ANAKINJEDISCARRED, id_WOOKIEE, id_TWOFACE;

struct AISYS_s;

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

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

extern f32 HUB_EPISODETITLEY;
extern i16 tSELECT, tSELECTED, tSELECTING;

extern void (*drawslotinfofn)(f32, f32, i32, i32);
extern void (*drawslotsfn)(MENU *, f32);

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

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

extern i16 id_GAMORREANGUARD;

extern "C" i16 id_LANDSPEEDER;
extern i32 CUTCAMONLY, GUNSHIPAHACK, netcamera, complexsockposition_forcesock, movegamecamera_forcesock;

extern void (*FS_Callback)(char *, char *);

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

extern "C" {
    extern "C++" FadeSystem FadeSys;
}

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

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

extern i16 tCHALLENGECOMPLETE, tMINIKIT;

extern char codechars[37];
extern char usercode[6];
extern i16 tSELECT, tBACK, tBUY, tPLAY, tSELECTING;

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

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

#include "nu2api/nucore/nucore_plain_types.h"
} // extern "C"

extern i16 id_GAMORREANGUARD, id_PRINCESSLEIASLAVE, id_LAMASU, id_TAUNWE;

extern "C" {
extern void (*checkantinodefns[3])(APIOBJECT *, AIANTINODE *, NUVEC *, f32);
    extern void (*checkantinodefns[3])(APIOBJECT_s *, AIANTINODE_s *, NUVEC *, f32);
}

extern "C" {
#include "legoapi/ai/core/ai_sys_stubs_types.h"
} // extern "C"

// Callback storage is declared in reverse linked-BSS order because the
// original GCC 4.7 -O3 object emits these definitions in reverse source order.

// Game-specific pre-load limits. These must be applied before WorldInfo
// reserves each level's fixed-size gameplay systems.

static i32 GameAudio_CheckReverb_LSW() {
    LEVELDATA *level = WorldInfo_CurrentlyActive()->current_level;
    if (level == HOTHBATTLEB_LDATA || level == DAGOBAHD_LDATA) {
        return 1;
    }
    if (level == ASTEROIDCHASEB_LDATA && GameCam->sock_position.location.sock == 4) {
        return 1;
    }
    return 0;
}

static inline i32 GameAudio_OverrideFootStep_LSW(GameObject_s *object, i32 alternate) {
    WORLDINFO *world = WorldInfo_CurrentlyActive();
    AREADATA *area = world->area;

    if ((area == HOTHESCAPE_ADATA || world->current_level == JABBASPALACEE_LDATA) && alternate == 0) {
        return GetSfxId("fs_ice");
    }

    if (area == DAGOBAH_ADATA && alternate == 0) {
        if (world->current_level != DAGOBAHA_LDATA || object->apiobj.field_0x281 != 0x14) {
            if (world->current_level != DAGOBAHD_LDATA) {
                if (world->current_level != DAGOBAHE_LDATA ||
                    (GameCam->sock_position.location.sock != 4 && GameCam->sock_position.location.sock != 1)) {
                    return GetSfxId("fs_swamp");
                }
            }
        }
    }

    if (world->current_level == JABBASPALACEA_LDATA &&
        (object->apiobj.field_0x281 == 9 || object->apiobj.field_0x281 == 0x18)) {
        return GetSfxId("fs_ice");
    }

    if ((area == DEATHSTARRESCUE_ADATA || area == DEATHSTARESCAPE_ADATA || WORLD->area == JABBASPALACE_ADATA) &&
        object->apiobj.field_0x281 == 0x14) {
        return GetSfxId("FS_JWalkM");
    }

    return -1;
}

void SetLevelSfxBits(WORLDINFO *world) {
    i32 sfx_ids[1024];
    i32 sfx_count = 0;
    i32 sound_id;

#define ADD_SFX(name) AddLevelSfxFromName(name, sfx_ids, &sfx_count, 0x400)
#define ADD_GAME_SFX(id) GameAudio_AddSfx(id, sfx_ids, &sfx_count, 0x400)

    for (i32 i = 0; i < world->level_sfx_count; ++i) {
        sfx_ids[sfx_count++] = world->level_sfx[i].id;
    }

    if (world->spinner_count > 0) {
        ADD_SFX("fly_paddle_rotate_lp");
        ADD_SFX("fly_paddle_stuck");
    }
    if (world->teleport_count > 0) {
        ADD_SFX("env_door_flap");
    }

    ADD_GAME_SFX(0x50);
    ADD_GAME_SFX(0x51);
    ADD_GAME_SFX(0x52);
    ADD_SFX("Grv_GuardWeaponLp");

    if (world->grabber != NULL) {
        if (world->current_level == CLOUDCITYTRAPA_LDATA) {
            ADD_SFX("CarbonFreezeCraneLp");
            ADD_SFX("CarbonFreezeCrane");
        } else if (world->current_level == JABBASPALACEB_LDATA) {
            ADD_SFX("env_hover_box_lp");
        } else {
            ADD_SFX("env_crane_mvt_lp");
        }
        ADD_SFX("env_crane_in");
        ADD_SFX("env_grabber_down");
        ADD_SFX("env_grabber_pickup");
        ADD_SFX("env_grabber_up");
        ADD_SFX("Explode2");
        if (world->current_level == CLOUDCITYESCAPEC_LDATA) {
            ADD_SFX("imp_C3PO_magnet");
            ADD_SFX("imp_C3PO_magnet_drop");
        }
    }

    if (world->cutscene_sys != NULL) {
        for (i32 i = 0; i < world->cutscene_sys->count; ++i) {
            CUTINFO *cut = world->cutscene_sys->cuts[i];
            // The original visits the six cutscene cues as separate checks.
            sound_id = cut->sfx[0].id;
            if (sound_id != -1) {
                sfx_ids[sfx_count++] = sound_id;
            }
            sound_id = cut->sfx[1].id;
            if (sound_id != -1) {
                sfx_ids[sfx_count++] = sound_id;
            }
            sound_id = cut->sfx[2].id;
            if (sound_id != -1) {
                sfx_ids[sfx_count++] = sound_id;
            }
            sound_id = cut->sfx[3].id;
            if (sound_id != -1) {
                sfx_ids[sfx_count++] = sound_id;
            }
            sound_id = cut->sfx[4].id;
            if (sound_id != -1) {
                sfx_ids[sfx_count++] = sound_id;
            }
            sound_id = cut->sfx[5].id;
            if (sound_id != -1) {
                sfx_ids[sfx_count++] = sound_id;
            }
        }
    }

    Pulses_AddSfx(world->pulses_sys, sfx_ids, &sfx_count, 0x400);
    if (world->hat_machine_sys != NULL && world->hat_machine_sys->count > 0) {
        ADD_SFX("SwLever");
        ADD_SFX("HatOn");
    }
    if (world->nlevers > 0) {
        ADD_SFX("SwLever");
    }

    if (world->area != NULL && (world->area->flags & 1) != 0) {
        ADD_SFX("XWing_Torpedo");
        ADD_SFX("imp_proton_torp");
        ADD_SFX("XWing_LoopDeLoop");
    } else {
        ADD_SFX("Char_Slide_Lp");
        if (world->push_block_count > 0) {
            ADD_SFX("Block_Shove");
            ADD_SFX("Block_Push_Lp");
        }
        ADD_SFX("exp_thermalDet");
        ADD_SFX("imp_thermalDet_attach");
        if (world->area != NULL && world->area == SPEEDERCHASE_ADATA) {
            ADD_SFX("XWing_LoopDeLoop");
        }
    }
    ADD_SFX("Explode1");

    LEVELDATA *level = world->current_level;
    if (level != NULL) {
        if (level->unknown_0a2 != -1) {
            sfx_ids[sfx_count++] = level->unknown_0a2;
        }
    }
    if (level != NULL && (level == STATUS_LDATA || (level->flags & LEVEL_STATUS) != 0)) {
        ADD_SFX("StatusAward");
        ADD_SFX("Status_GoldBarDec");
        ADD_SFX("TrueJedi_100pc");
        ADD_SFX("TrueJedi_NOT");
        ADD_SFX("MK-Panel");
        ADD_SFX("Char_Icon_App");
        ADD_SFX("Char_Icon_Slide");
        ADD_SFX("LegoClicks");
        ADD_SFX("Shop_BuyCheat");
        ADD_SFX("Explode1");
        ADD_SFX("Jp_Ana_Jump");
        ADD_SFX("PickupCoin");
    } else if (world->area != NULL && (world->area->flags & 4) != 0) {
        ADD_SFX("Victory");
        ADD_SFX("exp_debris");
    } else if (level != NULL && level == CREDITS_LDATA) {
        ADD_SFX("StatusAward");
    }

    if (world->area != NULL) {
        if (world->area->super_counter_count != 0) {
            ADD_GAME_SFX(0x53);
        }
        if (world->area == PODRACE_ADATA || world->area == PODSPRINT_ADATA) {
            ADD_SFX("PodX_TuskenBlast");
            ADD_SFX("Pod_TuskHit");
            ADD_SFX("PodX_EngSebulba_Lp");
            ADD_SFX("PodX_EngAnakin_Lp");
            ADD_SFX("PodX_EngGeneric1_Lp");
            ADD_SFX("PodX_EngGeneric2_Lp");
            ADD_SFX("PodX_EngGeneric3_Lp");
            ADD_SFX("PodX_EngStartup1");
            ADD_SFX("PodX_EngStartup2");
            ADD_SFX("Pod_Race_Go");
            ADD_SFX("Pod_Race_Light");
            ADD_SFX("PodX_EngGeneric_Lp");
            ADD_SFX("PodX_Collide");
            ADD_SFX("PodX_Crash");
            ADD_SFX("PodX_Booster");
            ADD_SFX("CountdownTimerTick");
            ADD_SFX("CountdownTimerTock");
            ADD_SFX("PodX_PurpCrysHit");
        } else if (world->area == GUNSHIP_ADATA || world->area == BONUS_GUNSHIP_ADATA) {
            ADD_SFX("GC_GunshipBlasterFire");
            ADD_SFX("GC_GunshipEngineLp");
            ADD_SFX("GC_GunshipDeath");
            ADD_SFX("GC_LaserBeam");
            ADD_SFX("Explode1");
        } else if (world->area == HUB_ADATA) {
            ADD_SFX("Shop_BuyCheat");
            ADD_SFX("Shop_NotEnufMuny");
            ADD_SFX("ui_ZoomIn");
            ADD_SFX("ui_ZoomOut");
            ADD_SFX("ui_hover_lp");
        } else if (world->area == JABBASPALACE_ADATA) {
            ADD_SFX("swdisco");
        } else if (world->area == NEWTOWN_ADATA) {
            ADD_SFX("BBounce");
        }
    }

    level = world->current_level;
    if (level != NULL) {
        if (level == NEGOTIATIONSA_LDATA) {
            ADD_SFX("NegA_CoinMDoorUp");
            ADD_SFX("NegA_CoinMDoorDn");
        } else if (level == NEGOTIATIONSB_LDATA) {
            ADD_SFX("FField");
            ADD_SFX("NegC_Crate");
        } else if (level == NEGOTIATIONSC_LDATA) {
            ADD_SFX("NegC_MagnetCoil");
            ADD_SFX("NegC_MagnetCoilH");
            ADD_SFX("NegC_MagnetCoilS");
            ADD_SFX("FField");
            ADD_SFX("NegC_Crate");
        } else if (level == GUNGAN_A_LDATA) {
            ADD_SFX("GunA_TreeFall");
            ADD_SFX("FS_CreaRun");
            ADD_SFX("IkopiGrowl");
            ADD_SFX("KaaduBark");
            ADD_SFX("KaaduGrowl");
        } else if (level == RESCUEC_LDATA) {
            ADD_SFX("ResX_MarbColLp");
            ADD_SFX("ResX_PoleLift_Rise");
            ADD_SFX("ResX_PoleLift_Lower");
        } else if (level == RETAKEB_LDATA) {
            ADD_SFX("ResX_MarbColLp");
        } else if (level == RETAKED_LDATA) {
            ADD_SFX("ResX_PortCulLp");
            ADD_SFX("ResX_PortCulEnd");
            ADD_SFX("Ep1_5_OutdoorPlatform");
        } else if (level == RETAKEE_LDATA) {
            ADD_SFX("Ep1_5_OutdoorPlatform");
            ADD_SFX("ResX_PortCulLp");
            ADD_SFX("ResX_PortCulEnd");
        } else if (level == MAULA_LDATA) {
            ADD_SFX("Ep1_7_BDroidPlatf");
            ADD_SFX("Ep1_7_SFPowerUp");
            ADD_SFX("Ep1_7_SFHoverLp");
            ADD_SFX("Ep1_7_SFTakeOff");
        } else if (level == MAULD_LDATA) {
            ADD_SFX("Ep1_7_FloatPlatRise");
            ADD_SFX("NegC_Crate");
        } else if (level == KAMINOE_LDATA) {
            ADD_SFX("Kam_Slave1BlasterFire");
            ADD_SFX("Slave1_EngineLp");
        } else if (level == KAMINOC_LDATA) {
            ADD_SFX("Kam_DiscoFloorPanelOn");
            ADD_SFX("Kam_DiscoFloorPanelDone");
            ADD_SFX("Kam_SparkLp");
            ADD_SFX("Kam_SparkLp");
            ADD_SFX("Kam_ForceFieldLp");
            ADD_SFX("Kam_ForceFieldOff");
        } else if (level == FACTORYB_LDATA) {
            ADD_SFX("FacB_BeltLp");
            ADD_SFX("FacB_ConvStop");
            ADD_SFX("Fac_StampHydros");
            ADD_SFX("Fac_StampImpacts");
            ADD_SFX("Fac_BonusCylUp");
            ADD_SFX("Fac_BonusBeep");
        } else if (level == FACTORYD_LDATA) {
            ADD_SFX("Fac_BucketGearUD");
            ADD_SFX("Fac_BucketAwayFB");
            ADD_SFX("Fac_BucketLp");
            ADD_SFX("Fac_BucketArriveLock");
            ADD_SFX("Fac_BucketOpens");
        } else if (level == FACTORYF_LDATA) {
            ADD_SFX("Fac_LaserGateLoop");
        } else if (level == FACTORYG_LDATA) {
            ADD_SFX("Fac_ObiRestraintLoop");
        } else if (level == GUNSHIPB_LDATA) {
            ADD_SFX("CountdownTimerTick");
            ADD_SFX("CountdownTimerTock");
        } else if (level == DOOKUC_LDATA) {
            ADD_SFX("Dooku_LightningLp");
        } else if (level == DOGFIGHTA_LDATA) {
            ADD_SFX("Ep3_1_StarDestEngLp");
            ADD_SFX("Ep3_1_ProtoXWingMissile");
            ADD_SFX("Ep3_1_ExplosionM");
            ADD_SFX("Ep3_1_ExplosionXXL");
            ADD_SFX("Dog_TurretFire");
            ADD_SFX("Dog_CloneARC170Gun");
            ADD_SFX("Dog_JediStFighterGun");
            ADD_SFX("Dog_JediStFighterGun2");
            ADD_SFX("Dog_JediStFighterGun3");
            ADD_SFX("Dog_JediStFighterEngLp");
            ADD_SFX("Dog_CloneARC170EngLp");
            ADD_SFX("Dog_DroidFighterBlast");
            ADD_SFX("Dog_TriFighterGuns");
            ADD_SFX("Dog_TriFighterGuns2");
            ADD_SFX("Dog_DroidFighterEngLp");
            ADD_SFX("Dog_TriFighterEngLp");
            ADD_SFX("Dog_DroidFighterHit");
            ADD_SFX("Dog_TriFighterHit");
            ADD_SFX("Dog_HugeBeamGunFire");
            ADD_SFX("Dog_HugeBeamGunLp");
            ADD_SFX("Dog_SepShieldPwrDown");
            ADD_SFX("Dog_ShipBreakExplo");
            ADD_SFX("Dog_ShipDoorOpen");
            ADD_SFX("Dog_StDestTurretSpins");
            ADD_SFX("Dog_TowerBreakExplo");
        } else if (level == CRUISERD_LDATA) {
            ADD_SFX("Cru_HugeWallMoveLp");
        } else if (level == CRUISERG_LDATA) {
            ADD_SFX("NegA_CoinMDoorUp");
            ADD_SFX("NegA_CoinMDoorDn");
        } else if (level == KASHYYYKD_LDATA) {
            ADD_SFX("Kas_BoulderLoop");
            ADD_SFX("Kas_BoulderExplo");
        } else if (level == VADERA_LDATA) {
            ADD_SFX("CountdownTimerTick");
            ADD_SFX("CountdownTimerTock");
        } else if (level == TATOOINEA_LDATA) {
            ADD_SFX("env_shower_lp");
        } else if (level == TATOOINEB_LDATA) {
            ADD_SFX("greenlighton");
            ADD_SFX("env_suckerspit_3PO");
            ADD_SFX("env_suckerspit_gonk");
            ADD_SFX("conveyorlp");
        } else if (level == MOSEISLEYA_LDATA) {
            ADD_SFX("Landspeeder_EngineLp");
            ADD_SFX("env_jacuzzi_lp");
        } else if (level == MOSEISLEYC_LDATA) {
            ADD_SFX("Droid_BeamLp");
            ADD_SFX("FField");
            ADD_SFX("FFieldOff");
            ADD_SFX("SwPPad");
        } else if (level == DEATHSTARRESCUEB_LDATA) {
            ADD_SFX("env_tractorbeam_off");
            ADD_SFX("env_tractorbeam_lp");
        } else if (level == DEATHSTARRESCUEC_LDATA) {
            ADD_SFX("ffield");
        } else if (level == DEATHSTARRESCUEE_LDATA) {
            ADD_SFX("Turret_PitchLp");
            ADD_SFX("Turret_YawLp");
            ADD_SFX("Turbo_Fire");
        } else if (level == DEATHSTARESCAPEA_LDATA) {
            ADD_SFX("Dianoga_Groan");
            ADD_SFX("Dianoga_Roar");
        } else if (level == DEATHSTARESCAPED_LDATA) {
            ADD_SFX("Dianoga_Groan");
            ADD_SFX("Dianoga_Roar");
        } else if (level == DEATHSTARESCAPEB_LDATA) {
            ADD_SFX("SqueakWash");
        } else if (level == DEATHSTARESCAPEC_LDATA) {
            ADD_SFX("FField");
            ADD_SFX("FFieldOff");
        } else if (level == DEATHSTARBATTLEA_LDATA) {
            ADD_SFX("FField");
            ADD_SFX("FFieldOff");
        } else if (level == DEATHSTARBATTLEB_LDATA) {
            ADD_SFX("FField");
            ADD_SFX("FFieldOff");
        } else if (level == DEATHSTARBATTLEC_LDATA) {
            ADD_SFX("FField");
            ADD_SFX("FFieldOff");
        } else if (level == DEATHSTARBATTLED_LDATA) {
            ADD_SFX("FField");
            ADD_SFX("FFieldOff");
        } else if (level == HOTHESCAPEA_LDATA) {
            ADD_SFX("ThermalDet_Beep");
            ADD_SFX("env_padLight_on");
        } else if (level == HOTHESCAPEC_LDATA) {
            ADD_SFX("env_padLight_on");
        } else if (level == ASTEROIDCHASED_LDATA) {
            ADD_SFX("exp_asteroid");
        } else if (level == CLOUDCITYTRAPA_LDATA) {
            ADD_SFX("Env_Steam_Lp");
        } else if (level == CLOUDCITYESCAPEA_LDATA) {
            ADD_SFX("env_steam_lp");
            ADD_SFX("Env_ctrl_desk_on");
        } else if (level == CLOUDCITYESCAPEC_LDATA) {
            ADD_SFX("env_steam_lp");
        } else if (level == DEATHSTAR2BATTLEB_LDATA) {
            ADD_SFX("FField");
            ADD_SFX("FFieldOff");
        } else if (level == DEATHSTAR2BATTLED_LDATA) {
            ADD_SFX("ForceLightningLp");
        } else if (level == JABBASPALACEA_LDATA) {
            ADD_SFX("SwDisco");
            ADD_SFX("Leia_Blaster");
        } else if (level == JABBASPALACEB_LDATA) {
            ADD_SFX("SwDisco");
            ADD_SFX("Leia_Blaster");
        } else if (level == JABBASPALACED_LDATA) {
            ADD_SFX("SwDisco");
            ADD_SFX("Leia_Blaster");
        } else if (level == JABBASPALACEE_LDATA) {
            ADD_SFX("exp_minecart");
        } else if (level == SARLACCPITB_LDATA) {
            ADD_SFX("env_curtain_lp");
            ADD_SFX("Kam_DiscoFloorPanelOn");
            ADD_SFX("Kam_DiscoFloorPanelDone");
        } else if (level == SARLACCPITC_LDATA) {
            ADD_SFX("FField");
            ADD_SFX("FFieldOff");
            ADD_SFX("imp_c3po_magnet_drop");
            ADD_SFX("env_magnet_on");
        } else if (level == SPEEDERCHASEA_LDATA) {
            ADD_SFX("FField");
            ADD_SFX("FFieldOff");
        } else if (level == ENDORBATTLEA_LDATA) {
            ADD_SFX("env_lantern_lp");
        } else if (level == ENDORBATTLEB_LDATA) {
            ADD_SFX("waterfall");
            ADD_SFX("drd_r2_mvt_water_lp");
            ADD_SFX("env_lantern_lp");
        } else if (level == ENDORBATTLED_LDATA) {
            ADD_SFX("FField");
            ADD_SFX("FFieldOff");
        } else if (level == EMPERORFIGHTA_LDATA) {
            ADD_SFX("env_block_light_on");
            ADD_SFX("env_padlight_on");
        }
    }

    if (VehicleArea != 0) {
        ADD_SFX("TieDoorsOpen");
    }
    if (Mission_Active(NULL) != NULL) {
        ADD_SFX("Victory");
    }
    if (Arcade != 0) {
        ADD_SFX("env_padLight_on");
    }
    if (world->area != NULL) {
        if (world->area == HOTHESCAPE_ADATA || world->area == JABBASPALACE_ADATA) {
            ADD_SFX("fs_ice");
        } else if (world->area == DAGOBAH_ADATA) {
            ADD_SFX("fs_swamp");
        }
    }
    if (apicharsys->loaded_model_count > 0 && world->current_level != NULL && (world->current_level->flags & 2) != 0 &&
        VehicleArea == 0) {
        ADD_GAME_SFX(0x14);
        ADD_GAME_SFX(0x15);
    }

    for (i32 character_id = 0; character_id < apicharsys->character_count; ++character_id) {
        i16 model_id = apicharsys->playermodelids[character_id];
        if (model_id != -1 && (apicharsys->models[model_id].flags & 1) != 0) {
            CHARACTERDATA *character = &CDataList[character_id];
            GAMECHARACTERDATA *game_character = static_cast<GAMECHARACTERDATA *>(character->field11_0x24);

            {
                const i16 configured_sfx = game_character->sfx_die;
                if (configured_sfx != -1) {
                    sfx_ids[sfx_count++] = configured_sfx;
                } else if ((character->model_flags & 0x44002010) == 0) {
                    ADD_GAME_SFX((static_cast<i32>(game_character->flags_090 << 14) >> 31) + 0x1c);
                }
            }
            {
                const i16 configured_sfx = game_character->sfx_hurt;
                if (configured_sfx != -1) {
                    sfx_ids[sfx_count++] = configured_sfx;
                } else if ((character->model_flags & 0x44002010) == 0) {
                    ADD_GAME_SFX((static_cast<i32>(game_character->flags_090 << 14) >> 31) + 0x17);
                }
            }
            {
                const i32 configured_sfx = game_character->sfx_grunt;
                if (configured_sfx != -1) {
                    sfx_ids[sfx_count++] = configured_sfx;
                } else if ((character->model_flags & 0x44002010) == 0) {
                    ADD_GAME_SFX((static_cast<i32>(game_character->flags_090 << 14) >> 31) + 0x15);
                }
            }
            {
                const i16 configured_sfx = game_character->sfx_engine;
                if (configured_sfx != -1) {
                    sfx_ids[sfx_count++] = configured_sfx;
                }
            }
            {
                const i16 configured_sfx = game_character->sfx_shoot;
                if (configured_sfx != -1) {
                    sfx_ids[sfx_count++] = configured_sfx;
                }
            }
            {
                const i16 configured_sfx = game_character->sfx_footstep;
                if (configured_sfx != -1) {
                    sfx_ids[sfx_count++] = configured_sfx;
                }
            }
            {
                const i16 configured_sfx = game_character->sfx_chatter;
                if (configured_sfx != -1) {
                    sfx_ids[sfx_count++] = configured_sfx;
                }
            }
            {
                const i16 configured_sfx = game_character->sfx_sabre;
                if (configured_sfx != -1) {
                    sfx_ids[sfx_count++] = configured_sfx;
                }
            }
            for (i32 i = 0; i < 6 && game_character->sfx_misc[i] != -1; ++i) {
                sfx_ids[sfx_count++] = game_character->sfx_misc[i];
            }

            if ((character->model_flags & 0x40) != 0) {
                ADD_SFX("drd_r2_scope_up");
                ADD_SFX("drd_r2_scope_down");
                ADD_SFX("drd_r2_mvt_water_lp");
            }
            if ((game_character->flags_090 & 0x400) != 0) {
                ADD_SFX("TowCable_Fire");
                ADD_SFX("TowCable_Latch");
                ADD_SFX("TowCable_Detach");
                ADD_SFX("TowCable_Snap");
            }
            if ((character->model_flags & 0x2000) != 0) {
                ADD_SFX("XWing_Torpedo");
                ADD_SFX("env_tractorbeam_lp");
                if ((character->model_flags & 0x04000000) == 0) {
                    ADD_SFX("Explode1");
                }
            }
            if (character_id == id_CHEWBACCA) {
                ADD_SFX("C3_Hurt");
                ADD_SFX("C3_Death");
            }
            if (character->move_fn == Move_BEAST) {
                ADD_SFX("Lego_Poo");
                ADD_SFX("Lego_PLOP");
                ADD_SFX("FliesLp");
            }
            if (game_character->uses_weapon_action == 10) {
                ADD_SFX("veh_tie_by");
                ADD_SFX("Tie_Spins");
            }
            if (character_id == id_EWOK || character_id == id_WICKET) {
                ADD_SFX("wpn_bomb_drop");
                ADD_SFX("exp_bomb");
            }
            if ((game_character->flags_090 & 4) != 0) {
                ADD_SFX("ForceLightningLp");
            } else if ((game_character->flags_090 & 2) != 0) {
                ADD_SFX("ForceChokeCrunch");
            } else if ((character->model_flags & 8) != 0) {
                ADD_SFX("ForceMindTrick");
            }
            if (game_character->uses_weapon_action == 12 && (character->model_flags & 8) != 0 &&
                id_GAMORREANGUARD != -1 && apicharsys->playermodelids[id_GAMORREANGUARD] != -1) {
                ADD_SFX("ForceChokeCrunch");
            }
            if ((game_character->flags_094[3] & 0x20) != 0) {
                ADD_GAME_SFX(0x4c);
                ADD_GAME_SFX(0x4d);
                ADD_GAME_SFX(0x4e);
            }

            CHARACTER_EFFECT_s *effect = character->effects;
            if (effect != NULL) {
                while (effect->character_id != -1) {
                    if (effect->sound_id != -1) {
                        sfx_ids[sfx_count++] = effect->sound_id;
                    }
                    ++effect;
                }
            }
            if ((character->model_flags & 0x20) != 0) {
                ADD_SFX("TC14_VLA");
                ADD_SFX("TC14_VLN");
            }
            if ((character->model_flags & 0x40) != 0) {
                ADD_SFX("R2D2_VLA");
            }
        }
    }

    i32 double_score_sfx = Arcade != 0;
    if (!double_score_sfx) {
        level = world->current_level;
        double_score_sfx = level != NULL && (level->flags & 0x800) != 0;
    }
    if (!double_score_sfx) {
        i32 portal = 0x13;
        while (!double_score_sfx && portal <= 0x17) {
            if (world->portal_places[portal] != NULL) {
                double_score_sfx = 1;
            } else {
                ++portal;
            }
        }
    }
    if (double_score_sfx) {
        ADD_SFX("ui_DoubleScoreEntry");
        ADD_SFX("ui_DoubleScoreText");
    }

    AddLevelSfxGizmoSys(world->gizmo_sys, world, sfx_ids, &sfx_count, 0x400);
    SetSpecialSfxBits(sfx_ids, &sfx_count, world);
    sfx_ids[sfx_count] = -1;
    if (sfx_count > 0) {
        SetSoundBitsById(sfx_ids, SetSfxBit_On);
    }
    SfxBitsStore(&CurrentSFXTAB);

#undef ADD_GAME_SFX
#undef ADD_SFX
}

