// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "nu2api/nusound/nusound3_include.hpp"
#include "decomp.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nucore.hpp"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/nuvector.hpp"
#include "nu2api/numusic/numusic.h"
#include "nu2api/nusound/nusound.h"
#include "nu2api/nusound/nusound_android.hpp"
#include "nu2api/nusound/nusound_buffer.hpp"
#include "nu2api/nusound/nusound_loader.hpp"
#include "nu2api/nusound/nusound_loader_ogg.hpp"
#include "nu2api/nusound/nusound_streamer.hpp"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nusound/nusound_voice.hpp"
#include "nu2api/nusound/nusound_decoder.hpp"
#include <new>
#include <stdarg.h>
#include <string.h>
#include "nu2api/nusound/nusound3_include_types.h"
#include <stdlib.h>
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nusound/nusound_system.hpp"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"
#include <cfloat>
#include "legoapi/core/config/cheat.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/world/mission.h"
#include "legoapi/props/system/socksys.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nufile/nufile.h"
#include <stdio.h>
#include "legoapi/items/base/apiobject.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/world/level.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuvideo.h"
#include "legoapi/items/collect/torpedo.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edui.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/core/input/timer.h"
#include "nu2api/nu3d/nuportal.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/world/levels/levels.h"
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
#include "nu2api/nu3d/nupostparams.h"
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
#include "legoapi/render/fx/game_deb.h"
#include <stddef.h>
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edgra.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include <cstdio>
#include <GLES2/gl2.h>
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "java/java.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "legoapi/world/areas.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/menus/screens/movies.h"
#include "legoapi/misc/androidbatman.h"
#include <limits.h>
#include "legoapi/render/fx/particles.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include <cstring>
#include "nu2api/nusound/nusound3_include.h"

// The original focused the listener on the player object; the title screen
// runs before gameplay, where it is NULL and the focus stays disabled.
static nuvec_s g_NuSoundFocusPosition;
static NuVector<nusound_filename_info_s> g_NuSoundSamples;

static NuSoundStream *g_NuSoundStreams[2] = {0};
static NuThreadEvent g_NuSoundLoadTrigger;
static NuSoundCriticalSection g_NuSoundLoadCriticalSection;

// Defined out of line: the original emitted this constructor on its own and
// called it from the static initialiser rather than inlining it.
NuEList<NuSound3Voice, DefaultElist>::NuEList() {
    begin = reinterpret_cast<NuSound3Voice *>(&begin_sentinel);
    end = reinterpret_cast<NuSound3Voice *>(&end_sentinel);
    begin_sentinel.next = reinterpret_cast<NuSound3Voice *>(&end_sentinel);
    end_sentinel.prev = reinterpret_cast<NuSound3Voice *>(&begin_sentinel);
    length = 0;
}

DECOMP_ASSERT(sizeof(NuEList<NuSound3Voice>) == 0x1c, "NuSound3Voice list size");
namespace {

    void StreamListPushBack(NuEList<NuSound3Voice> *list, NuSound3Voice *voice) {
        NuSound3Voice *end = list->end;
        NuSound3Voice *previous = end->intrusive_prev;
        end->intrusive_prev = voice;
        voice->intrusive_prev = previous;
        previous->intrusive_next = voice;
        voice->intrusive_next = end;
        list->length++;
    }

    void StreamListRemove(NuEList<NuSound3Voice> *list, NuSound3Voice *voice) {
        NuSound3Voice *previous = voice->intrusive_prev;
        NuSound3Voice *next = voice->intrusive_next;
        if (previous == NULL && next == NULL) {
            return;
        }
        list->length--;
        if (previous != NULL) {
            previous->intrusive_next = next;
        }
        if (next != NULL) {
            next->intrusive_prev = previous;
        }
        voice->intrusive_next = NULL;
        voice->intrusive_prev = NULL;
    }

} // namespace
static NuEList<NuSound3Voice> g_NuSoundVoicesPendingPlayback{};
static NuEList<NuSound3Voice> g_NuSoundVoicesActive{};
static NuEList<NuSound3Voice> g_NuSoundVoicesPendingDestruction{};
static i32 g_NuSoundNumReplaceableVoices;
NuSoundWeakPtr<NuSoundVoice> NuSound3Stream::mVoice;
static NuSoundBuffer g_NuSoundStreamBuffers[4];
static NuSoundListener g_NuSoundListener;
static NuSoundStreamer *g_NuSoundStreamer = NULL;
static u16 *g_NuSoundLoadBits = NULL;
static u16 *g_NuSoundLoadBitsCache = NULL;
static i32 g_NuSoundNumLoadBitShorts = 0;
static NuThread *g_NuSoundLoadThread = NULL;

// NuMusic - the music player (nu2api.2013/numusic/numusic.cpp).
//
// Decompressed from the original libTTapp.so. The player owns a parsed
// music.cfg "album" table (albums -> tracks), a shared entry-time pool and two
// stereo-stream voices. The boot path for the title music is:
//
//   LoadPermData -> ConfigureMusic -> NuMusic::Initialise("audio\\music.cfg")
//   NuMain -> GamePlayMusic(titles level) -> SelectTrackByHandle x3
//           -> PlayTrack(0x20) -> PlayTrackI -> Voice::Load -> Voice::Play
//           -> NuSound3PlayStereoV (host nusound streamer)
//   per-frame: NuMusic::Process (fades, ducking, volume mixing)
//
// Track handles pack (albumIndex << 11) | trackIndexInAlbum. The original
// constructs/validates them through reciprocal multiplies because
// sizeof(Album) == 36 == 4 * 9 and sizeof(Track) == 60 == 4 * 15; those
// literal forms are kept so the arithmetic matches the original binary.

static NuMusic *the_music_player = NULL;

NUFPCOMJMPCTX NuMusic::top_jmp_tab[11] = {
    {"ALBUM", NuMusic::xsAlbum},
    {"ACTION", NuMusic::xsAction},
    {"QUIET", NuMusic::xsQuiet},
    {"OVERLAY", NuMusic::xsOverlay},
    {"SIGNATURE", NuMusic::xsSignature},
    {"CUTSCENE", NuMusic::xsCutscene},
    {"NOMUSIC", NuMusic::xsNoMusicC},
    {"GLOBALATTENUATION", NuMusic::xsGlobalAttenuation},
    {"PATH", NuMusic::xsPath},
    {"STRICT", NuMusic::xsStrict},
    {NULL, NULL},
};

NUFPCOMJMPCTX NuMusic::track_jmp_tab[15] = {
    {"IDENT", NuMusic::xsIdent},
    {"ID", NuMusic::xsIdent},
    {"INDEX", NuMusic::xsIndex},
    {"IX", NuMusic::xsIndex},
    {"NOMUSIC", NuMusic::xsNoMusic},
    {"STARTPOINT", NuMusic::xsIndex},
    {"NODUCK", NuMusic::xsNoDuck},
    {"DUCK", NuMusic::xsDuck},
    {"LOOPING", NuMusic::xsLooping},
    {"LOOP", NuMusic::xsLooping},
    {"NONLOOPING", NuMusic::xsNonLooping},
    {"NOLOOP", NuMusic::xsNonLooping},
    {"ATTENUATION", NuMusic::xsAttenuation},
    {NULL, NULL},
};

// Globals driving GamePlayMusic. The original keeps these in the batman TU.
i32 NOMUSIC = 0;

// i32 GamePlayMusic(LEVELDATA_s *level, i32 check, OPTIONSSAVE_s *options)
//
// Picks the level's music tracks and starts playback. `check` is 0 on the
// boot/init call and 1 when re-invoked from the main loop (where it bails
// out early if the "other" pair hasn't changed). Classes: 1 = quiet,
// 2 = action, 0x20 = plain theme (the title music lives here).

static const char *voice_status_txt[] = {
    "IDLE_PENDING", "IDLE", "PAUSED_PENDING", "PAUSED", "SEEKING_PENDING", "SEEKING", "PLAYPENDING", "PLAYING",
};

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

struct VuVec;

struct nuvec_s;

struct EDCREATURE_s;

typedef isize __attribute__((may_alias)) NuGCutWord;

extern AIACTIONDEF api_aiactiondefs[55];
extern AICONDITIONDEF api_aiconditiondefs[50];

DECOMP_ASSERT(sizeof(api_aiactiondefs) == 0x294, "API action registry size");

DECOMP_ASSERT(sizeof(api_aiconditiondefs) == 0x258, "API condition registry size");

struct nupad_s;

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

#include "nu2api/nucore/nucore_plain_types.h"

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

#undef NUGCUT_CURVE_VALUE

    extern "C++" {
        static __attribute__((noinline, noclone)) void
        ClipInstTree(NuInstanceTree_s *tree,
                     i32 tree_index) __asm__("_ZL12ClipInstTreeP16NuInstanceTree_siP16nudisplayscene_sPc.isra.92");
    }
} // extern "C"

void NuSound3SampleLoadThread(void *arg) {
    (void)arg;

    while (true) {
        pthread_mutex_lock(&g_NuSoundLoadTrigger.mutex);
        while (!g_NuSoundLoadTrigger.a) {
            pthread_cond_wait(&g_NuSoundLoadTrigger.cond, &g_NuSoundLoadTrigger.mutex);
        }
        g_NuSoundLoadTrigger.a = g_NuSoundLoadTrigger.b;
        pthread_mutex_unlock(&g_NuSoundLoadTrigger.mutex);

        if (__builtin_expect(g_NuSoundLoadBits != NULL, 0)) {
            g_NuSoundLoadCriticalSection.Lock();

            for (u32 i = 0; i < g_NuSoundSamples.length; i++) {
                nusound_filename_info_s &info = g_NuSoundSamples.data[i];
                if (!(info.index <= 0xfff)) {
                    u16 request = g_NuSoundLoadBits[i >> 4] & static_cast<u16>(1 << (i & 0xf));
                    NuSoundSample::LoadState load_state =
                        reinterpret_cast<NuSoundSample *>(info.sample)->GetLoadState();
                    reinterpret_cast<NuSoundSample *>(info.sample)->GetLastErrorState();

                    if (request == 0 && load_state != NuSoundSample::LoadState::NOT_LOADED && info.sample != NULL &&
                        reinterpret_cast<NuSoundSample *>(info.sample)->field_0x18 == 0) {
                        reinterpret_cast<NuSoundSample *>(info.sample)->Release();
                        reinterpret_cast<NuSoundSample *>(info.sample)->Unload();
                    }
                }
            }

            for (u32 i = 0; i < g_NuSoundSamples.length; i++) {
                nusound_filename_info_s &info = g_NuSoundSamples.data[i];
                if (!(info.index <= 0xfff)) {
                    u16 request = g_NuSoundLoadBits[i >> 4] & static_cast<u16>(1 << (i & 0xf));
                    NuSoundSample::LoadState load_state =
                        reinterpret_cast<NuSoundSample *>(info.sample)->GetLoadState();
                    NuSoundSample::ErrorState error =
                        reinterpret_cast<NuSoundSample *>(info.sample)->GetLastErrorState();

                    if (request != 0 && error != NuSoundSample::ErrorState::FILE_NOT_FOUND &&
                        load_state == NuSoundSample::LoadState::NOT_LOADED && info.sample != NULL) {
                        reinterpret_cast<NuSoundSample *>(info.sample)->Reference();
                        reinterpret_cast<NuSoundSample *>(info.sample)->Load(NULL, 0, NULL);
                    }
                }
            }

            g_NuSoundLoadCriticalSection.Unlock();
        }
    }
}

NuMusic *NuMusic::GetPlayer() {
    return the_music_player;
}

// GamePlayMusic consults this option byte (original: SuperOptions field 0x14).
// Non-zero selects the quiet/action attack tracks instead of the plain theme.
NuMusic::NuMusic() {
    this->albums = NULL;
    this->album_count = 0;
    this->fileinfo = NULL;
    this->file_count = 0;
    this->track_index = 0;
    for (i32 i = 0; i < 6; i++) {
        this->class_volumes[i] = 1.0f;
    }
}

NuMusic::~NuMusic() {
}

void NuMusic::NoMusic(i32 no_music) {
    if (this != NULL && the_music_player != NULL) {
        this->track_index = no_music != 0;
    }
}

void NuMusic::InitVoiceManager() {
    // Original memsets each 0x34-byte voice, then sets stream index, status,
    // the fade state and clears the fade-out flag.
    memset(&this->voices[0], 0, sizeof(Voice));
    this->voices[0].stream_index = 0;
    this->voices[0].status = VOICE_STATUS_READY;
    this->voices[0].fade_rate = 0.0f;
    this->voices[0].gain = 1.0f;
    this->voices[0].last_volume = -1.0f;
    this->voices[0].play_time = 0.0f;
    this->voices[0].flags &= ~1u;

    memset(&this->voices[1], 0, sizeof(Voice));
    this->voices[1].stream_index = 1;
    this->voices[1].status = VOICE_STATUS_READY;
    this->voices[1].fade_rate = 0.0f;
    this->voices[1].gain = 1.0f;
    this->voices[1].last_volume = -1.0f;
    this->voices[1].play_time = 0.0f;
    this->voices[1].flags &= ~1u;
}

NuMusic::Voice *NuMusic::FindVoiceByTrack(Track *track) {
    if (track == NULL)
        return NULL;
    for (i32 index = 0; index < 2; ++index)
        if (voices[index].tracks[voices[index].track_index] == track)
            return &voices[index];
    return NULL;
}

NuMusic::Voice *NuMusic::FindVoiceByClass(TRACK_CLASS clazz) {
    for (i32 index = 0; index < 2; ++index) {
        Track *track = voices[index].tracks[voices[index].track_index];
        if (track != NULL && track->clazz == clazz)
            return &voices[index];
    }
    return NULL;
}

NuMusic::Voice *NuMusic::FindVoiceByClassAndStatus(TRACK_CLASS clazz, VOICE_STATUS status) {
    for (i32 index = 0; index < 2; ++index) {
        Track *track = voices[index].tracks[voices[index].track_index];
        if (track != NULL && track->clazz == clazz && voices[index].status == status)
            return &voices[index];
    }
    return NULL;
}

NuMusic::Voice *NuMusic::FindIdleVoice() {
    for (i32 index = 0; index < 2; ++index)
        if (voices[index].status == VOICE_STATUS_READY)
            return &voices[index];
    for (i32 index = 0; index < 2; ++index)
        if (voices[index].status == VOICE_STATUS_STOPPED)
            return &voices[index];
    return NULL;
}

void NuMusic::Voice::SetStatusFn(i32 status, i32 tag) {
    (void)tag; // original passes a source-line constant for error reporting
    if (this->status != (VOICE_STATUS)status) {
        this->play_time = 0.0f;
        this->status = (VOICE_STATUS)status;
    }
}

void NuMusic::Track::ManageEntryTime() {
    // Advance to the next INDEX entry; the value becomes this track's
    // start offset the next time it is played.
    f32 value = 0.0f;
    if (this->entry_count != 0) {
        i32 index = (this->entry_index + 1) % this->entry_count;
        value = this->entry_times[index];
        this->entry_index = index;
    }
    this->start_offset = value;
}

void NuMusic::Track::SetEntryTime(f32 entry_time) {
    this->start_offset = entry_time;
}

i32 NuMusic::ClassToIX(u32 i) {
    if (i == TRACK_CLASS_QUIET)
        return 0;
    if (i == TRACK_CLASS_ACTION)
        return 1;
    if (i == TRACK_CLASS_4)
        return 2;
    if (i == TRACK_CLASS_8)
        return 3;
    if (__builtin_expect(i == TRACK_CLASS_CUTSCENE, 0))
        return 4;
    if (i == TRACK_CLASS_NOMUSIC)
        return 5;
    return -1;
}

i32 NuMusic::SetTrackEntryTimeByClass(u32 clazz, f32 entry_time) {
    if (this == NULL || the_music_player == NULL || this->album == NULL) {
        return 0;
    }
    i32 index = ClassToIX(clazz);
    if (index == -1) {
        return 0;
    }
    this->album->tracks[index]->SetEntryTime(entry_time < 0.0f ? 0.0f : entry_time);
    return 1;
}

NuMusic::Track *NuMusic::Album::GetTrack(u32 clazz) {
    i32 index = ClassToIX(clazz);
    if (index == -1) {
        return NULL;
    } else {
        return this->tracks[index];
    }
}

i32 NuMusic::Album::GetTracks(u32 class_mask, Track **out_tracks) {
    i32 count = 0;
    for (i32 index = 0; index < 6; ++index) {
        if (class_mask & (1 << index)) {
            out_tracks[index] = tracks[index];
            ++count;
        } else {
            out_tracks[index] = NULL;
        }
    }
    return count;
}

void NuMusic::Album::Initialise() {
    i32 count = tracks_count;
    for (i32 index = 0; index < 6; ++index) {
        tracks[index] = NULL;
        if (count > 0) {
            Track *next = tracks_source;
            TRACK_CLASS clazz = 1 << index;
            for (u32 scanned = 0; scanned < static_cast<u32>(count); ++scanned) {
                Track *track = next++;
                if (track->clazz == clazz) {
                    tracks[index] = &tracks_source[scanned];
                    break;
                }
            }
        }
    }
}

const char *NuMusic::ClassToName(u32) {
    return "UNKNOWN";
}

bool NuMusic::SelectTrackByHandle(TRACK_CLASS clazz, i32 trackHandle) {
    if (this != NULL && the_music_player != NULL && this->album != NULL) {
        u32 index = (u32)ClassToIX(clazz);
        if (index < 6) {
            if (trackHandle != -1) {
                Album *album = this->album;
                // Validate the album encoded in the handle's high bits against
                // the album being selected (see header note on handle packing).
                if ((((i32)((char *)album - (char *)this->albums) >> 2) * (u32)0x38e38e39 - (trackHandle >> 0xb) ==
                     0) &&
                    ((trackHandle & 0x7ff) < album->tracks_count)) {
                    album->tracks[index] = &album->tracks_source[trackHandle & 0x7ff];
                    return true;
                }
            } else {
                this->album->tracks[index] = NULL;
            }
        }
    }

    return false;
}

i32 NuMusic::GetTrackHandle(TRACK_CLASS clazz, const char *name) {
    if (this != NULL && the_music_player != NULL && this->album != NULL) {
        if (name != NULL) {
            LOG_DEBUG("Searching for track name '%s' of class %d, tracks_count=%d", name, clazz,
                      this->album->tracks_count);
            for (i32 i = 0; i < this->album->tracks_count; i++) {
                if (this->album->tracks_source[i].clazz == clazz && this->album->tracks_source[i].ident != NULL) {
                    if (NuStrICmp(this->album->tracks_source[i].ident, name) == 0) {
                        return ((((i32)((char *)this->album - (char *)this->albums) >> 2) * (u32)0x38e38e39) << 11) | i;
                    }
                }
            }
        } else {
            i32 index = ClassToIX(clazz);
            if (index != -1) {
                Track *track = this->album->tracks[index];
                if (track != NULL) {
                    return (((i32)((char *)track - (char *)this->album->tracks_source) >> 2) * (u32)0xEEEEEEEF) |
                           ((((i32)((char *)this->album - (char *)this->albums) >> 2) * (u32)0x38e38e39) << 11);
                }
            }
        }
    }

    return -1;
}

bool NuMusic::SelectTrack(TRACK_CLASS clazz, const char *name) {
    if (this != NULL && the_music_player != NULL) {
        return SelectTrackByHandle(clazz, GetTrackHandle(clazz, name));
    }
    return false;
}

int NuMusic::GetStatus(u32 clazz, i32 *class_status) {
    if (this == NULL || the_music_player == NULL) {
        return 0;
    }

    if (class_status != NULL) {
        class_status[0] = 0;
        class_status[1] = 0;
        class_status[2] = 0;
        class_status[3] = 0;
        class_status[4] = 0;
        class_status[5] = 0;
    }

    i32 result = 0;
    Track *track = voices[0].tracks[voices[0].track_index];
    if (track != NULL && (track->clazz & clazz) != 0) {
        if (voices[0].status <= VOICE_STATUS_PLAYING_LOADED) {
            u32 status_bit = 1u << voices[0].status;
            if ((status_bit & 0x75) != 0) {
                result = 1;
            } else if ((status_bit & 0x80) != 0) {
                result = 4;
            } else if ((status_bit & 8) != 0) {
                result = 2;
            } else {
                result = 0;
            }
        }
        i32 class_index = ClassToIX(track->clazz);
        if (class_status != NULL) {
            if (class_index != -1) {
                class_status[class_index] |= result;
            }
        }
    }

    track = voices[1].tracks[voices[1].track_index];
    if (track != NULL && (track->clazz & clazz) != 0) {
        i32 status = result;
        switch (voices[1].status) {
            case VOICE_STATUS_NONE:
            case VOICE_STATUS_STOPPING:
            case VOICE_STATUS_CUED:
            case VOICE_STATUS_ENDED:
            case VOICE_STATUS_PLAYING:
                status = 1;
                break;
            case VOICE_STATUS_READY:
                status = 0;
                break;
            case VOICE_STATUS_STOPPED:
                status = 2;
                break;
            case VOICE_STATUS_PLAYING_LOADED:
                status = 4;
                break;
        }
        result |= status;
        i32 class_index = ClassToIX(track->clazz);
        if (class_status != NULL && class_index != -1) {
            class_status[class_index] |= status;
        }
    }
    return result;
}

void NuMusic::xIndex(nufpar_s *fpar) {
    this->indexes[this->index_count++] = NuFParGetFloatRDP(fpar);
    this->current_track->entry_count++;
}

void NuMusic::xsIndex(nufpar_s *fpar, void *thisptr) {
    ((NuMusic *)thisptr)->xIndex(fpar);
}

void NuMusic::xNoDuck(nufpar_s *fpar) {
    (void)fpar;
    ((u8 *)&this->current_track->flags)[0] |= 1;
}

void NuMusic::xsNoDuck(nufpar_s *fpar, void *thisptr) {
    ((NuMusic *)thisptr)->xNoDuck(fpar);
}

void NuMusic::xLooping(nufpar_s *fpar) {
    (void)fpar;
    ((u8 *)&this->current_track->flags)[0] |= 2;
}

void NuMusic::xsLooping(nufpar_s *fpar, void *thisptr) {
    ((NuMusic *)thisptr)->xLooping(fpar);
}

void NuMusic::xNonLooping(nufpar_s *fpar) {
    (void)fpar;
    ((u8 *)&this->current_track->flags)[0] &= 0xfd;
}

void NuMusic::xsNonLooping(nufpar_s *fpar, void *thisptr) {
    ((NuMusic *)thisptr)->xNonLooping(fpar);
}

char *NuMusic::RemovePath(char *str) {
    char *str_;
    char c;

    c = *str;
    if (c != '\0') {
        char *last_sep = NULL;
        str_ = str;

        do {
            u8 separator_count = (c == '\\') + (c == '/');
            if (separator_count != 0) {
                last_sep = str_;
            }
            str_ = str_ + 1;
            c = *str_;
        } while (c != '\0');

        if (last_sep != NULL) {
            str = last_sep + 1;
        }
    }

    return str;
}

void NuMusic::xPath(nufpar_s *fpar) {
    NuFParGetWord(fpar);
    NuStrCpy(this->current_path, fpar->word_buf);
}

void NuMusic::xsPath(nufpar_s *fpar, void *thisptr) {
    ((NuMusic *)thisptr)->xPath(fpar);
}

void NuMusic::xStrict(nufpar_s *fpar) {
    fpar->line_buf[28] = '\x01';
    NuFParSetInterpreterErrorHandler(GlobalParseErrorFn);
}

void NuMusic::xsStrict(nufpar_s *fpar, void *thisptr) {
    ((NuMusic *)thisptr)->xStrict(fpar);
}

char *NuMusic::FindString(const char *str) {
    for (char *ptr = this->string_pool_start; ptr < this->string_pool_end; ptr += NuStrLen(ptr) + 1) {
        if (NuStrICmp(ptr, str) == 0) {
            return ptr;
        }
    }

    return NULL;
}

char *NuMusic::AllocString(const char *str) {
    char *ptr = FindString(str);

    if (ptr == NULL) {
        ptr = this->string_pool_end;
        this->string_pool_end += NuStrLen(str) + 1;
        NuStrCpy(ptr, str);
    }

    return ptr;
}

void NuMusic::xAlbum(nufpar_s *fpar) {
    this->current_album = &this->albums[this->album_count++];
    this->current_album->tracks_source = &this->tracks[this->track_count];
    this->current_album->tracks_count = 0;

    NuFParGetWord(fpar);
    this->current_album->name = AllocString(fpar->word_buf);
    LOG_DEBUG("Parsing album '%s'", this->current_album->name);
}

void NuMusic::xsAlbum(nufpar_s *fpar, void *thisptr) {
    ((NuMusic *)thisptr)->xAlbum(fpar);
}

void NuMusic::xIdent(nufpar_s *fpar) {
    NuFParGetWord(fpar);
    this->current_track->ident = AllocString(fpar->word_buf);
}

void NuMusic::xsIdent(nufpar_s *fpar, void *thisptr) {
    ((NuMusic *)thisptr)->xIdent(fpar);
}

void NuMusic::SubstituteString(char *dst, char *src, char *find, char *subst) {
    while (true) {
        char *hit = NuStrIStr(src, find);
        if (hit == NULL)
            break;

        i32 pos = (usize)hit - (usize)src;
        NuStrNCpy(dst, src, pos + 1);
        NuStrCpy(dst + pos, subst);

        src = hit + NuStrLen(find);
        dst += pos + NuStrLen(subst);
    }

    NuStrCpy(dst, src);
}

void NuMusic::ParseTrack(u32 category, nufpar_s *fpar) {
    Track *track = &this->tracks[this->track_count++];
    this->current_track = track;
    if (this->current_album != NULL) {
        this->current_album->tracks_count++;
    }

    memset(track, 0, sizeof(Track));

    track = this->current_track;

    // Entry times carve sequential slices out of the shared INDEX pool.
    track->clazz = category;

    track->entry_times = this->indexes + this->index_count;
    track->pitch = this->pitch_default;
    track->duck_volume = 1.0f;
    track->duck_fade = 1.0f;
    track->attenuation = 1.0f;

    // Signature (4), overlay (8) and cutscene (16) tracks default to
    // non-looping; everything else loops until NONLOOPING says otherwise.
    switch (category) {
        case 4:
        case 8:
        case 16:
            ((u8 *)&track->flags)[0] &= 0xfd;
            break;
        default:
            ((u8 *)&track->flags)[0] |= 2;
            break;
    }

    char buf[256];
    char buf2[264];
    NuFParGetWord(fpar);
    NuStrCpy(buf, this->current_path);
    NuStrCat(buf, fpar->word_buf);
    SubstituteString(buf2, buf, "$lang", this->language);

    Track *path_track = this->current_track;
    path_track->path = AllocString(buf2);
    track = this->current_track;
    track->ident = RemovePath(track->path);

    nufpcomfn *prev_handler = NULL;
    if (this->strict_mode) {
        prev_handler = NuFParSetInterpreterErrorHandler(TrackParseErrorFn);
    }

    NuFParPushComCTX(fpar, track_jmp_tab);
    if (*fpar->word_buf != '\0') {
        if (*fpar->word_buf != ';') {
            do {
                NuFParGetWord(fpar);
                NuFParInterpretWordCTX(fpar, this);
                if (*fpar->word_buf == ';')
                    break;
            } while (*fpar->word_buf != '\0');
        }
    }
    NuFParPopCom(fpar);

    if (this->strict_mode) {
        NuFParSetInterpreterErrorHandler(prev_handler);
    }

    LOG_DEBUG("Parsed track: class=%d, path='%s', ident='%s'", track->clazz, track->path, track->ident);
}

void NuMusic::xNoMusicC(nufpar_s *fpar) {
    ParseTrack(TRACK_CLASS_NOMUSIC, fpar);
}

void NuMusic::xsNoMusicC(nufpar_s *fpar, void *thisptr) {
    ((NuMusic *)thisptr)->xNoMusicC(fpar);
}

void NuMusic::xCutscene(nufpar_s *fpar) {
    ParseTrack(TRACK_CLASS_CUTSCENE, fpar);
}

void NuMusic::xsCutscene(nufpar_s *fpar, void *thisptr) {
    ((NuMusic *)thisptr)->xCutscene(fpar);
}

void NuMusic::xSignature(nufpar_s *fpar) {
    ParseTrack(TRACK_CLASS_4, fpar);
}

void NuMusic::xsSignature(nufpar_s *fpar, void *thisptr) {
    ((NuMusic *)thisptr)->xSignature(fpar);
}

void NuMusic::xOverlay(nufpar_s *fpar) {
    ParseTrack(TRACK_CLASS_8, fpar);
}

void NuMusic::xsOverlay(nufpar_s *fpar, void *thisptr) {
    ((NuMusic *)thisptr)->xOverlay(fpar);
}

void NuMusic::xQuiet(nufpar_s *fpar) {
    ParseTrack(TRACK_CLASS_QUIET, fpar);
}

void NuMusic::xsQuiet(nufpar_s *fpar, void *thisptr) {
    ((NuMusic *)thisptr)->xQuiet(fpar);
}

void NuMusic::xAction(nufpar_s *fpar) {
    ParseTrack(TRACK_CLASS_ACTION, fpar);
}

void NuMusic::xsAction(nufpar_s *fpar, void *thisptr) {
    ((NuMusic *)thisptr)->xAction(fpar);
}

void NuMusic::xNoMusic(nufpar_s *fpar) {
    NuFParGetWord(fpar);
    char buf[256];
    SubstituteString(buf, fpar->word_buf, "$lang", this->language);
    this->current_track->name = AllocString(buf);
}

void NuMusic::xsNoMusic(nufpar_s *fpar, void *thisptr) {
    ((NuMusic *)thisptr)->xNoMusic(fpar);
}

i32 NuMusic::FindOrCreateSoundFile(nusound_filename_info_s *files, i32 *count, const char *filename, i32 param_4,
                                   i32 unused)

{
    if (filename == NULL) {
        return -1;
    }

    i32 index = 0;
    for (; index < *count; index++) {
        if (NuStrICmp(files[index].filename, filename) == 0) {
            return index;
        }
    }

    nusound_filename_info_s *puVar1 = &files[index];
    puVar1->filename = filename;
    puVar1->index = index;
    puVar1->field3_0xc = 0;
    puVar1->field4_0x4 = reinterpret_cast<void *>(static_cast<usize>(param_4 == 0));

    (*count)++;

    return index;
}

void NuMusic::BuildSoundTable(variptr_u *buffer_start, variptr_u buffer_end) {
    nusound_filename_info_s *finfo = (nusound_filename_info_s *)buffer_start->void_ptr;
    this->fileinfo = finfo;

    i32 count __attribute__((aligned(16))) = 0;

    i32 i = 0;
    for (; i < this->track_count; i++) {
        Track *track = &this->tracks[i];

        // Track flags bit1 (looping) decides whether the file registers as a
        // streaming sample; the pitch rides along for the loader.
        i32 streaming = static_cast<i8>(static_cast<u8>(track->flags) << 6) >> 7;
        track->file_indexes[0] = FindOrCreateSoundFile(this->fileinfo, &count, track->path, streaming, track->pitch);
        track = &this->tracks[i];
        streaming = static_cast<i8>(static_cast<u8>(track->flags) << 6) >> 7;
        track->file_indexes[1] = FindOrCreateSoundFile(this->fileinfo, &count, track->name, streaming, track->pitch);
    }

    finfo = this->fileinfo;
    nusound_filename_info_s *puVar1 = &finfo[count];
    puVar1->filename = NULL;
    puVar1->field4_0x4 = NULL;
    puVar1->index = -1;

    buffer_start->void_ptr = &finfo[count + 1];
}

void NuMusic::InitData(const char *file, VARIPTR *buffer_start, VARIPTR buffer_end) {
    VARIPTR buffer_original = *buffer_start;

    this->string_pool_start = buffer_start->char_ptr;
    this->string_pool_end = this->string_pool_start;
    buffer_start->addr += 0x10000;

    this->albums = reinterpret_cast<Album *>(ALIGN(buffer_start->addr, 4));
    buffer_start->addr = ALIGN(buffer_start->addr, 4) + 512 * sizeof(Album);
    this->tracks = reinterpret_cast<Track *>(ALIGN(buffer_start->addr, 4));
    buffer_start->addr = ALIGN(buffer_start->addr, 4) + 2048 * sizeof(Track);
    this->indexes = reinterpret_cast<f32 *>(ALIGN(buffer_start->addr, 4));
    buffer_start->addr = ALIGN(buffer_start->addr, 4) + 2048 * sizeof(f32);
    this->album_count = 0;
    this->track_count = 0;
    this->index_count = 0;
    LOG_DEBUG("this->albums=%p, this->tracks=%p, this->indexes=%p", this->albums, this->tracks, this->indexes);

    NuFParSetInterpreterErrorHandler(0);
    nufpar_s *fpar = NuFParCreate(const_cast<char *>(file));
    if (__builtin_expect(fpar != NULL, 1)) {
        NuFParPushComCTX(fpar, top_jmp_tab);

        while (NuFParGetLine(fpar), *fpar->line_buf != '\0') {
            NuFParGetWord(fpar);
            NuFParInterpretWordCTX(fpar, this);
        }

        NuFParDestroy(fpar);
    }
    NuFParSetInterpreterErrorHandler(0);
    LOG_DEBUG("Loaded %d albums, %d tracks, %d indexes", this->album_count, this->track_count, this->index_count);
    LOG_DEBUG("this->albums=%p, this->tracks=%p, this->indexes=%p", this->albums, this->tracks, this->indexes);

    i32 count = this->album_count;
    if (count == 0 || (track_count = this->track_count, track_count == 0)) {
        *buffer_start = buffer_original;
        this->albums = NULL;
        this->album_count = 0;
        this->tracks = NULL;
        this->track_count = 0;
        return;
    }

    // Compact the parse workspace down to just what was filled in, and rebase
    // the intra-pool pointers (album->tracks_source, track->entry_times).
    Track *tracksPtr;
    Track *alloced;
    Track *pTVar3;

    Album *albumPtr = this->albums;
    Album *album = (Album *)ALIGN((usize)this->string_pool_end, 4);
    if (__builtin_expect(albumPtr == album, 0)) {
        tracksPtr = this->tracks;
        alloced = (Track *)ALIGN((usize)&albumPtr[count].name, 4);
        pTVar3 = tracksPtr;
        if (tracksPtr == alloced)
            goto LAB_003203e6;
    LAB_003204e0:
        memmove(alloced, pTVar3, track_count * sizeof(Track));
        track_count = this->track_count;
        count = this->album_count;
        tracksPtr = alloced;
    } else {
        memmove(album, albumPtr, count * sizeof(Album));
        count = this->album_count;
        this->albums = album;
        track_count = this->track_count;
        pTVar3 = this->tracks;
        tracksPtr = (Track *)(album + count);
        if (pTVar3 == tracksPtr)
            goto LAB_003203e6;
        alloced = tracksPtr;
        if (__builtin_expect(track_count != 0, 0))
            goto LAB_003204e0;
    }

    this->tracks = tracksPtr;
    if (0 < count) {
        const isize track_rebase = reinterpret_cast<char *>(tracksPtr) - reinterpret_cast<char *>(pTVar3);
        Album *album = this->albums;
        Album *end = album + count;
        do {
            if (album->tracks_source != NULL) {
                album->tracks_source =
                    reinterpret_cast<Track *>(reinterpret_cast<char *>(album->tracks_source) + track_rebase);
            }
            ++album;
        } while (album != end);
        tracksPtr = this->tracks;
    }

LAB_003203e6:
    f32 *pfVar2 = this->indexes;
    f32 *__dest = (f32 *)ALIGN((usize)&tracksPtr[track_count].path, 4);
    i32 local_20;
    if (pfVar2 == __dest) {
        local_20 = this->index_count;
    } else {
        local_20 = 0;

        if (this->index_count != 0) {
            memmove(__dest, pfVar2, this->index_count << 2);
            track_count = this->track_count;
            local_20 = this->index_count;
        }
        this->indexes = __dest;

        if (0 < track_count) {
            const isize index_rebase = reinterpret_cast<char *>(__dest) - reinterpret_cast<char *>(pfVar2);
            tracksPtr = this->tracks;
            pTVar3 = tracksPtr + track_count;
            do {
                if (tracksPtr->entry_times != NULL) {
                    tracksPtr->entry_times =
                        reinterpret_cast<f32 *>(reinterpret_cast<char *>(tracksPtr->entry_times) + index_rebase);
                }
                tracksPtr = tracksPtr + 1;
            } while (tracksPtr != pTVar3);
            __dest = this->indexes;
        }
    }

    buffer_start->void_ptr = __dest + local_20;
    LOG_DEBUG("this->albums=%p, this->tracks=%p, this->indexes=%p", this->albums, this->tracks, this->indexes);
    for (i32 i = 0; i < this->album_count; i++) {
        this->albums[i].Initialise();
    }

    BuildSoundTable(buffer_start, buffer_end);
}

i32 NuMusic::GetAlbumHandle(char const *name) {
    if (this != NULL && the_music_player != NULL && this->album_count > 0) {
        for (i32 i = 0; i < this->album_count; i++) {
            if (NuStrICmp(this->albums[i].name, name) == 0) {
                return i + 0x12345678;
            }
        }
    }
    return 0;
}

i32 NuMusic::SetAlbum(i32 handle) {
    if (this == NULL || the_music_player == NULL || handle < 0x12345678) {
        return 0;
    }

    i32 index = handle - 0x12345678;
    if (index >= this->album_count) {
        return 0;
    }

    this->album = &this->albums[index];
    return 1;
}

i32 NuMusic::SetAlbum(char const *name) {
    if (this == NULL || the_music_player == NULL) {
        return 0;
    }

    i32 handle = GetAlbumHandle(name);
    if (handle == 0) {
        return 0;
    }
    SetAlbum(handle);
    return handle;
}

i32 NuMusic::Initialise(const char *file, char *null, VARIPTR *buffer_start, VARIPTR buffer_end) {
    the_music_player = this;
    this->language = null;
    this->pitch_default = 0x1000; // unity pitch in the game's fixed-point format
    this->current_path[0] = '\0';
    this->strict_mode = 0;
    InitVoiceManager();
    InitData(file, buffer_start, buffer_end);
    this->master_volume = 1.0f;
    this->fader_rate = 0.0f;
    this->duck_rate = 0.5f;
    this->album = this->albums;
    this->fader_current = 1.0f;
    this->fader_target = 1.0f;
    this->duck_gain = 1.0f;
    this->duck_current = 1.0f;
    this->global_attenuation = 1.0f;
    return 1;
}

void NuMusic::SetMasterVolume(f32 volume) {
    if (this == NULL || the_music_player == NULL) {
        return;
    }
    if (volume > 1.0f)
        volume = 1.0f;
    if (volume < 0.0f)
        volume = 0.0f;
    if (volume > 1.0f)
        volume = 1.0f;
    this->master_volume = volume;
}

void NuMusic::SetFader(f32 target, f32 fade_seconds) {
    if (this == NULL || the_music_player == NULL) {
        return;
    }
    if (fade_seconds != 0.0f) {
        this->fader_rate = 1.0f / fade_seconds;
        this->fader_target = target;
        return;
    }
    // Zero fade time snaps both ends of the fader.
    this->fader_target = target;
    this->fader_current = target;
    this->fader_rate = 0.0f;
}

void NuMusic::SetClassVolume(u32 class_mask, f32 volume) {
    if (this == NULL || the_music_player == NULL) {
        return;
    }
    if (volume > 1.0f)
        volume = 1.0f;
    if (volume < 0.0f)
        volume = 0.0f;
    if (volume > 1.0f)
        volume = 1.0f;

    if ((class_mask & TRACK_CLASS_QUIET) != 0)
        this->class_volumes[0] = volume;
    if ((class_mask & TRACK_CLASS_ACTION) != 0)
        this->class_volumes[1] = volume;
    if ((class_mask & TRACK_CLASS_4) != 0)
        this->class_volumes[2] = volume;
    if ((class_mask & TRACK_CLASS_8) != 0)
        this->class_volumes[3] = volume;
    if ((class_mask & TRACK_CLASS_CUTSCENE) != 0)
        this->class_volumes[4] = volume;
    if ((class_mask & TRACK_CLASS_NOMUSIC) != 0)
        this->class_volumes[5] = volume;
}

void NuMusic::GetSoundFiles(nusound_filename_info_s **finfo, i32 *null) {
    if (this == NULL) {
        if (finfo != NULL) {
            *finfo = NULL;
        }
        if (null != NULL) {
            *null = 0;
        }
    } else {
        if (finfo != NULL) {
            *finfo = this->fileinfo;
        }
        if (null != NULL) {
            *null = this->file_count;
        }
    }
}

extern "C" {
extern "C" f32 numusicGetDuckVolume(void) {
    if (the_music_player != NULL) {
        return the_music_player->duck_current;
    }
    return 1.0f;
}

}

// PS2 volume (0..16383) to the engine's linear scalar.
f32 PS2VolumeToScalar(i32 volume) {
    return (f32)volume / 16383.0f;
}

void NuSound3Init(i32 zero) {
    bool is_crappy = NuIOS_IsLowEndDevice();

    NuCore::Initialize();

    NuSound.Initialise(0x633333 + (is_crappy ? 0 : 0x1ccccd));
    // libTTapp.so 0x3109af: NuSound3Init brings up the decoder singleton
    // thread right after the sound system.
    NuSoundDecoder::Initialise();

    // NuMemoryGet()->GetThreadMem()->_BlockAlloc(0xa48, 4, 1, "", 7);
    NuSoundStreamer *streamer = NU_ALLOC_T(NuSoundStreamer, 1, "", NUMEMORY_CATEGORY_NUSOUND);
    if (streamer != NULL) {
        new (streamer) NuSoundStreamer{};
    }
    g_NuSoundStreamer = streamer;

    // g_NuSoundLoadThread = NuThreadManager::CreateThread(NuCore::m_threadManager, NuSound3SampleLoadThread, (void
    // *)0x0,
    // 0, "NuSoundLoadThread", 0, 1, 1);
    g_NuSoundLoadThread =
        NuCore::m_threadManager->CreateThread(NuSound3SampleLoadThread, NULL, 0, "NuSoundLoadThread", 0,
                                              NUTHREADCAFECORE_UNKNOWN_1, NUTHREADXBOX360CORE_UNKNOWN_1);

    // NuSound3Init registers the single 3D listener against the live global
    // camera matrix.  The camera object is updated in place for every view,
    // so positional audio follows it without a per-frame listener call.
    NuSound.AddListener(&g_NuSoundListener);
    g_NuSoundListener.SetHeadMatrix((const VuMtx *)&global_camera.mtx);
    g_NuSoundListener.Enable();

    g_NuSoundStreamBuffers[0].Allocate(NuSoundSystem::GetStreamBufferSize() / 2,
                                       NuSoundSystem::MemoryDiscipline::SAMPLE);
    g_NuSoundStreamBuffers[1].Allocate(NuSoundSystem::GetStreamBufferSize() / 2,
                                       NuSoundSystem::MemoryDiscipline::SAMPLE);
    g_NuSoundStreamBuffers[2].Allocate(NuSoundSystem::GetStreamBufferSize() / 2,
                                       NuSoundSystem::MemoryDiscipline::SAMPLE);
    g_NuSoundStreamBuffers[3].Allocate(NuSoundSystem::GetStreamBufferSize() / 2,
                                       NuSoundSystem::MemoryDiscipline::SAMPLE);
}

extern "C" {
    i32 NuSound3InitEx(void) {
        NuSound3Init(0);
        return 1;
    }

}

i32 NuSound3InitV(VARIPTR *bufferStart, VARIPTR bufferEnd, i32 zero1, i32 zero2) {
    NuSound3Init(0);
    return 1;
}

extern "C" {
    void NuSound3UpdateEx(void) {
    }

    void NuSound3UpdateV(void) {
    }

}

void NuSound3SetSampleTable(nusound_filename_info_s *info, variptr_u *buffer_start, variptr_u buffer_end) {
    for (; info != NULL && info->index != -1; info++) {
        // TODO: dont cast classes
        if (!(info->index < 0x1000)) {
            info->sample = (NuSoundStreamingSample *)NuSound.AddSample(info->filename, NuSoundSystem::FileType::WAV,
                                                                       NuSoundSource::FeedType::ZERO);
        } else {
            info->sample = (NuSoundStreamingSample *)NuSound.AddSample(info->filename, NuSoundSystem::FileType::OGG,
                                                                       NuSoundSource::FeedType::STREAMING);
        }

        g_NuSoundSamples.PushBack(*info);
    }
}

extern "C" {
extern "C" void NuSound3SetRequestTable(u16 *request_bits, i32 short_count) {
    if (g_NuSoundLoadBitsCache != NULL && g_NuSoundNumLoadBitShorts != short_count) {
        delete g_NuSoundLoadBitsCache;
    }

    g_NuSoundLoadBitsCache = new u16[short_count];
    g_NuSoundNumLoadBitShorts = short_count;
    g_NuSoundLoadBits = request_bits;
}

extern "C" bool NuSound3IsSampleLoaded(i32 sample_index) {
    NuSoundSample *sample = reinterpret_cast<NuSoundSample *>(g_NuSoundSamples.data[sample_index].sample);
    return sample->GetLoadState() == NuSoundSample::LoadState::LOADED;
}

extern "C" i32 NuSound3CountVoices(i32 sample_index) {
    NuSoundSample *sample = reinterpret_cast<NuSoundSample *>(g_NuSoundSamples.data[sample_index].sample);
    if (sample == NULL) {
        return 0;
    }

    i32 count = sample->field_0x18;
    for (NuSound3Voice *entry = g_NuSoundVoicesPendingPlayback.Front(); entry != g_NuSoundVoicesPendingPlayback.End();
         entry = entry->intrusive_next) {
        if (entry->source == sample) {
            count++;
        }
    }
    return count;
}

extern "C" NuSoundVoice *NuSound3FindOldestVoice(i32 sample_index, f32 *playback_position) {
    NuSoundSample *sample = reinterpret_cast<NuSoundSample *>(g_NuSoundSamples.data[sample_index].sample);
    if (sample == NULL) {
        return NULL;
    }
    return NuSound.GetOldestVoice(sample, *playback_position);
}

extern "C" NuSoundVoice *NuSound3FindQuietestVoice(i32 sample_index, f32 *playback_position) {
    NuSoundSample *sample = reinterpret_cast<NuSoundSample *>(g_NuSoundSamples.data[sample_index].sample);
    if (sample == NULL) {
        return NULL;
    }
    return NuSound.GetQuietestVoice(sample, *playback_position);
}

extern "C" void NuSound3StopVoice(NuSoundVoice *voice) {
    if (voice == NULL) {
        return;
    }

    voice->Stop(false);
    NuSound3Voice *entry = NULL;
    for (NuSound3Voice *it = g_NuSoundVoicesActive.Front(); it != g_NuSoundVoicesActive.End() && entry == NULL;
         it = it->intrusive_next) {
        if (it->weak_ptr.obj == reinterpret_cast<NuSoundWeakPtrObj<NuSoundVoice> *>(voice)) {
            entry = it;
        }
    }
    if (entry == NULL) {
        return;
    }

    StreamListRemove(&g_NuSoundVoicesActive, entry);
    g_NuSoundNumReplaceableVoices++;
    StreamListPushBack(&g_NuSoundVoicesPendingDestruction, entry);
}

}

// NuSound3CreateVoice wraps a source in the original intrusive NuSound3Voice
// node and queues it for NuSound3Update. The final argument is the source loop
// flag; positional setup is selected independently by source_position.
void NuSound3CreateVoice(nuvec_s *pos, i32 index, f32 falloff_a, f32 falloff_b, i32 volume_left, i32 volume_right,
                         f32 pitch, bool loop) {
    f32 oldest_time;
    (void)volume_right;
    if (__builtin_expect(!(NuSound.GetNumAvailableOutputDevices() < 1 || index < 0 ||
                           index >= static_cast<i32>(g_NuSoundSamples.length)),
                         1)) {
        NuSoundSource *source = g_NuSoundSamples.data[index].sample;
        NuSoundSample *sample = (NuSoundSample *)source;
        if (__builtin_expect(sample == NULL || sample->GetLoadState() != NuSoundSample::LoadState::LOADED ||
                                 sample->GetResourceCount() < 1,
                             0)) {
            return;
        }

        i32 in_flight = g_NuSoundVoicesPendingPlayback.length + g_NuSoundVoicesActive.length;
        i32 source_voice_count = sample->field_0x18;
        for (NuSound3Voice *entry = g_NuSoundVoicesPendingPlayback.Front();
             entry != g_NuSoundVoicesPendingPlayback.End(); entry = entry->intrusive_next) {
            if (entry->source == source) {
                source_voice_count++;
            }
        }
        if (__builtin_expect(source_voice_count > 2 || in_flight > 15, 0)) {
            if (loop || g_NuSoundSamples.data[index].field7_0x1c == 0 || source_voice_count <= 2) {
                return;
            }

            NuSoundVoice *oldest_voice = NuSound.GetOldestVoice(sample, oldest_time);
            if (__builtin_expect(oldest_voice != NULL, 0)) {
                NuSound3StopVoice(oldest_voice);
            }
        }

        NuSound3Voice *voice = new NuSound3Voice();
        voice->source = source;
        voice->pitch = pitch;
        voice->volume = volume_left;
        voice->falloff_b = falloff_b;
        voice->falloff_a = falloff_a;
        voice->source_position = pos;
        voice->loop = loop;
        if (pos != NULL) {
            voice->position = *pos;
        }
        voice->pause_counter = 0;

        StreamListPushBack(&g_NuSoundVoicesPendingPlayback, voice);
    }
}

extern "C" {
extern "C" void NuSound3Play3dLoopSfx(nuvec_s *position, i32 sample_index, f32 falloff_near, f32 falloff_far,
                                      i32 volume_left, i32 volume_right, f32 pitch, f32, i32, f32, f32) {
    NuSound3Voice *entry = g_NuSoundVoicesActive.Front();
    NuSound3Voice *end = g_NuSoundVoicesActive.End();
    while (entry != end && entry->source_position != position) {
        entry = entry->intrusive_next;
    }

    if (entry == end) {
        NuSound3CreateVoice(position, sample_index, falloff_near, falloff_far, volume_left, volume_right, pitch, true);
        return;
    }

    entry->pause_counter = 0;
    if (__builtin_expect(entry->weak_ptr.obj != NULL, 1)) {
        reinterpret_cast<NuSoundVoice *>(entry->weak_ptr.obj)->SetPosition(reinterpret_cast<VuVec *>(position));
        reinterpret_cast<NuSoundVoice *>(entry->weak_ptr.obj)->SetVolume(PS2VolumeToScalar(volume_left));
        reinterpret_cast<NuSoundVoice *>(entry->weak_ptr.obj)->SetPitch(pitch);
        reinterpret_cast<NuSoundVoice *>(entry->weak_ptr.obj)
            ->SetFalloff(falloff_near, falloff_far, NuSoundSystem::FalloffType::LINEAR);
    }
}

extern "C" void NuSound3Play3d(nuvec_s *position, i32 sample_index, f32 falloff_near, f32 falloff_far, i32 volume_left,
                               i32 volume_right, f32 pitch, f32, i32, f32, f32) {
    NuSound3CreateVoice(position, sample_index, falloff_near, falloff_far, volume_left, volume_right, pitch, false);
}

extern "C" void NuSound3Play3dPri(nuvec_s *position, i32 sample_index, f32, f32, i32 volume_left, i32 volume_right,
                                  f32 pitch, f32, i32, f32, f32, i32) {
    NuSound3CreateVoice(position, sample_index, 0.0f, 0.0f, volume_left, volume_right, pitch, false);
}

extern "C" void NuSound3PlayPri(i32 sample_index, i32 volume_left, i32 volume_right, f32 pitch, f32, i32, f32, f32,
                                i32) {
    NuSound3CreateVoice(NULL, sample_index, 0.0f, 0.0f, volume_left, volume_right, pitch, false);
}

extern "C" void NuSound3Play(i32 sample_index, i32 volume_left, i32 volume_right, f32 pitch, f32, i32, f32, f32) {
    NuSound3CreateVoice(NULL, sample_index, 0.0f, 0.0f, volume_left, volume_right, pitch, false);
}

extern "C" void NuSound3Listener(VuMtx *matrix) {
    g_NuSoundListener.SetHeadMatrix(matrix);
}

extern "C" const VuMtx *NuSound3GetListener(void) {
    return g_NuSoundListener.GetHeadMatrix();
}

}

void NuSound3PauseStereoStream(i32 stream_index) {
    NuSoundStream *stream = g_NuSoundStreams[stream_index];
    if (stream != NULL && NuSound3Stream::mVoice.obj != NULL) {
        ((NuSoundVoice *)NuSound3Stream::mVoice.obj)->Pause();
    }
}

i32 NuMusic::PauseTrack(u32 clazz) {
    if (this != NULL && the_music_player != NULL && this->album != NULL) {
        Track *tracks[6];
        if (this->album->GetTracks(clazz, tracks) == 0) {
            return 0;
        }

        i32 result = 0;
        for (i32 i = 0; i < 6; i++) {
            Track *track = tracks[i];
            if (track == NULL) {
                continue;
            }
            Voice *voice = FindVoiceByTrack(track);
            if (voice != NULL && static_cast<u32>(voice->status - VOICE_STATUS_PLAYING) <= 1 &&
                (voice->flags & 2) == 0) {
                NuSound3PauseStereoStream(voice->stream_index);
                voice->flags |= 2u;
                result = 1;
            }
        }

        return result;
    }
    return 0;
}

void NuSound3ResumeStereoStream(i32 stream_index) {
    NuSoundStream *stream = g_NuSoundStreams[stream_index];
    if (stream != NULL && NuSound3Stream::mVoice.obj != NULL) {
        ((NuSoundVoice *)NuSound3Stream::mVoice.obj)->Resume();
    }
}

i32 NuMusic::ResumeTrack(u32 clazz) {
    if (this != NULL && the_music_player != NULL && this->album != NULL) {
        Track *tracks[6];
        if (this->album->GetTracks(clazz, tracks) == 0) {
            return 0;
        }

        i32 result = 0;
        for (i32 i = 0; i < 6; i++) {
            Track *track = tracks[i];
            if (track == NULL) {
                continue;
            }
            Voice *voice = FindVoiceByTrack(track);
            if (voice != NULL && static_cast<u32>(voice->status - VOICE_STATUS_PLAYING) <= 1 &&
                (voice->flags & 2) != 0) {
                NuSound3ResumeStereoStream(voice->stream_index);
                voice->flags &= ~2u;
                result = 1;
            }
        }

        return result;
    }
    return 0;
}

// Volume arrives as the PS2-style 0..16383 fixed point computed by
// NuMusic::Process. The streamer applies it when mixing.
void NuSound3SetStereoStreamVolume(i32 stream_index, i32 volume) {
    NuSoundStream *stream = g_NuSoundStreams[stream_index];
    if (stream != NULL) {
        stream->ps2volume = volume;
        if (stream->field_0xd != 0 && NuSound3Stream::mVoice.obj != NULL) {
            ((NuSoundVoice *)NuSound3Stream::mVoice.obj)->SetVolume(PS2VolumeToScalar(stream->ps2volume));
        }
    }
}

extern "C" {
    i32 NuSound3SetStreamVolume(i32 stream_index, i32 volume) {
        NuSound3SetStereoStreamVolume(stream_index, volume);
        return 1;
    }

}

// Reports the state of one stereo stream slot. A ready stream is only
// considered playing after its shared voice has actually started.
i32 NuSound3GetStereoStreamStatus(i32 stream_index) {
    NuSoundStream *stream = g_NuSoundStreams[stream_index];
    if (stream != NULL && stream->stream != NULL) {
        if (stream->stream->GetLoadState() == NuSoundSample::LoadState::STREAM_READY &&
            NuSound3Stream::mVoice.obj != NULL && stream->field_0xd != 0) {
            return NUSOUND_STEREO_STREAM_PLAYING;
        }
        const i32 finished_status = NUSOUND_STEREO_STREAM_FINISHED;
        const i32 resource_count = stream->stream->GetResourceCount();
        if (resource_count <= 0) {
            return NUSOUND_STEREO_STREAM_INACTIVE;
        }
        return finished_status;
    }
    return NUSOUND_STEREO_STREAM_INACTIVE;
}

extern "C" {
extern "C" f32 NuSound3GetStreamPlaybackTime(i32 stream_index) {
    if (g_NuSoundStreams[stream_index] != NULL && NuSound3Stream::mVoice.obj != NULL) {
        return reinterpret_cast<NuSoundVoice *>(NuSound3Stream::mVoice.obj)->GetPlaybackPositionSeconds();
    }
    return 0.0f;
}

}

f32 NuMusic::GetPlaybackTime(u32 clazz) {
    for (i32 i = 0; i < 2; ++i) {
        Voice *voice = &voices[i];
        Track *track = voice->tracks[voice->track_index];
        if (track != NULL && (track->clazz & clazz) != 0 && voice->status == VOICE_STATUS_PLAYING_LOADED)
            return NuSound3GetStreamPlaybackTime(voice->stream_index);
    }
    return 0.0f;
}

// Stream key status for the NuMusic voice state machine: 0 = slot empty,
// 1 = stream loaded / playing, 2 = stream finished (the voice stopped or is
// already gone).
i32 NuSound3StreamKeyStatus(i32 stream_index) {
    return NuSound3GetStereoStreamStatus(stream_index);
}

// Decibel attenuation from music.cfg (DUCK/ATTENUATION/GLOBALATTENUATION with
// negative values): -100 dB and below is silence, 0 dB and above is unity.
f32 NuSound3dBToAmplitude(f32 db) {
    return NuSoundSystem::dBToAmplitude(db);
}

void NuMusic::xGlobalAttenuation(nufpar_s *fpar) {
    this->global_attenuation = NuFParGetFloatRDP(fpar);
    if (this->global_attenuation < 0.0f) {
        // Negative values are decibels.
        this->global_attenuation = NuSound3dBToAmplitude(this->global_attenuation);
    }
}

void NuMusic::xsGlobalAttenuation(nufpar_s *fpar, void *thisptr) {
    ((NuMusic *)thisptr)->xGlobalAttenuation(fpar);
}

void NuMusic::xAttenuation(nufpar_s *fpar) {
    this->current_track->attenuation = NuFParGetFloatRDP(fpar);
    if (this->current_track->attenuation < 0.0f) {
        this->current_track->attenuation = NuSound3dBToAmplitude(this->current_track->attenuation);
    }
}

void NuMusic::xsAttenuation(nufpar_s *fpar, void *thisptr) {
    ((NuMusic *)thisptr)->xAttenuation(fpar);
}

void NuMusic::xDuck(nufpar_s *fpar) {
    this->current_track->duck_volume = NuFParGetFloatRDP(fpar);
    if (this->current_track->duck_volume < 0.0f) {
        this->current_track->duck_volume = NuSound3dBToAmplitude(this->current_track->duck_volume);
    }
    this->current_track->duck_fade = NuFParGetFloatRDP(fpar);
}

void NuMusic::xsDuck(nufpar_s *fpar, void *thisptr) {
    ((NuMusic *)thisptr)->xDuck(fpar);
}

extern "C" {
    f32 NuSound3AmplitudeTodB(f32 amplitude) {
        return NuSoundSystem::AmplitudeTodB(amplitude);
    }

    i32 NuSound3FClose(void) {
        return 0;
    }

    i32 NuSound3FOpen(void) {
        return 0;
    }

    i32 NuSound3FOpenSize(void) {
        return 0;
    }

    i32 NuSound3FRead(void) {
        return 0;
    }

    i64 NuSound3FSeek(void) {
        return 0;
    }

    i32 CheckStreamFileID(void) {
        return 0;
    }

    i32 NuSound3StopStream(void) {
        return 0;
    }

    void NuSound3RemoveStreamID(void) {
    }

    i32 NuSound3AddStreamEx(void) {
        return 0;
    }

    i32 NuSound3AddStream(void) {
        return 0;
    }

    void NuSound3InitLoopInfo(void) {
    }

    void NuSound3SetDat(void) {
    }

    void SOUND_SFXRequest_Table(void) {
        // Platform placeholder in the Android build.
    }

    i32 NuSound3FindFree(void) {
        return 0;
    }

    i32 NuSound3GetSize(void) {
        return 0;
    }

    void NuSound3CancelCheckStereo(void) {
    }

    void NuSound3SetLoopHoldTime(float t) {
        (void)t;
    }

    i32 NuSound3PlayStream(void) {
        return 0;
    }

    void NuSound3PlayInterleavedStereo(void) {
    }

    void NuSound3KillAllAudio(void) {
    }

    void NuSound3KillAllAudioWaitEx(void) {
    }

    void NuSound3KillAllAudioWait(void) {
    }

    void NuSound3HoldOffMusic(void) {
    }

    i32 NuSound3UpdatePending(void) {
        return 0;
    }

    void NuSound3BeginWaitUpdate(void) {
    }

    i32 NuSound3CheckWaitUpdate(void) {
        return 0;
    }

    i32 NuSound3SetStreamPitch(void) {
        return 0;
    }

    void NuSound3GetStreamInfo(i32, NuSoundStreamInfo *) {
    }

}

void NuMusic::Debug(i32, i32 y) {
    const char *class_names[] = {"INVALID", "QUIET", "ACTION", "SIGNATURE", "OVERLAY", "CUTSCENE", "NOMUSIC"};
    NuSoundStreamInfo stream_info;
    NuQFntPushPrintMode(2);
    NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
    NuQFntSet(system_qfont);
    NuQFntSetPointSize(system_qfont, 1.0f, 1.0f);
    NuQFntSetScale(system_qfont, 0.8f, 0.8f);
    NuQFntSetColour(system_qfont, 0x80ffffff);

#define DEBUG_MUSIC_VOICE(i)                                                                                           \
    do {                                                                                                               \
        Voice *voice = &voices[i];                                                                                     \
        NuSound3GetStreamInfo(voice->stream_index, &stream_info);                                                      \
        NuQFntPrintEx(system_qfont, 4800, y, 0x10, "VOICE:%d", i);                                                     \
        y += (i32)NuQFntHeight(system_qfont);                                                                          \
        if (voice->tracks[voice->track_index] == NULL) {                                                               \
            NuQFntPrintEx(system_qfont, 4800, y, 0x10, "TRACK:NONE");                                                  \
            y += (i32)NuQFntHeight(system_qfont);                                                                      \
            NuQFntPrintEx(system_qfont, 4800, y, 0x10, "CLASS:NONE");                                                  \
        } else {                                                                                                       \
            NuQFntPrintEx(system_qfont, 4800, y, 0x10, "TRACK:%s",                                                     \
                          voice->tracks[voice->track_index]->filenames[voice->track_sub[voice->track_index]]);         \
            y += (i32)NuQFntHeight(system_qfont);                                                                      \
            NuQFntPrintEx(system_qfont, 4800, y, 0x10, "CLASS:%s",                                                     \
                          class_names[ClassToIX(voice->tracks[voice->track_index]->clazz) + 1]);                       \
        }                                                                                                              \
        y += (i32)NuQFntHeight(system_qfont);                                                                          \
        NuQFntPrintEx(system_qfont, 4800, y, 0x10, "STATUS:%s", voice_status_txt[voice->status]);                      \
        y += (i32)NuQFntHeight(system_qfont);                                                                          \
        NuQFntPrintEx(system_qfont, 4800, y, 0x10, "VOLUME:%f (%f)", voice->gain, voice->last_volume);                 \
        y += (i32)NuQFntHeight(system_qfont);                                                                          \
        NuQFntPrintEx(system_qfont, 4800, y, 0x10, "FADE:%f", voice->fade_rate);                                       \
        y += (i32)NuQFntHeight(system_qfont);                                                                          \
        NuQFntPrintEx(system_qfont, 4800, y, 0x10, "POS:%f", stream_info.playback_position);                           \
        y += (i32)NuQFntHeight(system_qfont);                                                                          \
    } while (0)
    DEBUG_MUSIC_VOICE(0);
    DEBUG_MUSIC_VOICE(1);
#undef DEBUG_MUSIC_VOICE
    NuQFntPopPrintMode();
}

extern "C" {
    void NuSound3PlayChan(void) {
    }

    i32 NuSound3RestoreStreamPitch(void) {
        return 0;
    }

    i32 NuSound3LoadingSfx(void) {
        return 0;
    }

    void NuSound3FlushLoops(void) {
    }

    void NuSound3ClearLoopHold(NUVEC *, i32) {
    }

    void NuSound3DrawMem(void) {
    }

    void NuSound3StopSFX(void) {
    }

    void NuSound3SetSFXPitch(i32) {
    }

    // The Android original is eight NOPs followed by RET; the callers still
    // supply the full rumble request ABI.
    void NuSound3AddRumble(nupad_s *, f32, i32, i32, f32) {
    }

    void NuSound3SetRumblePads(nupad_s *, nupad_s *) {
    }

    void NuSound3StopRumble(void) {
    }

    void NuSound3UpdateRumble(f32) {
    }

    i32 NuSound3SetReverb(i32) {
        return 0;
    }

    void NuSound3SetReverbVol(void) {
    }

    void NuSound3SetDPL(i32, i32) {
    }

    void NuSound3Close(void) {
    }

    i32 NuSound3StreamOpen(void) {
        return 0;
    }

    i32 NuSound3ReadStream(void) {
        return 0;
    }

    i32 NuSound3CheckStream(void) {
        return 0;
    }

    i32 NuSound3StreamClose(void) {
        return 0;
    }

    i32 NuSound3FlushBG(void) {
        return 0;
    }

    i32 NuSound3FlushFG(void) {
        return 0;
    }

    i32 NuSound3ReserveStream(void) {
        return 0;
    }

    void NuSound3InitThreadSafeHackyMess(void) {
    }

    i32 StreamRead(void) {
        return 0;
    }

    void StreamSeek(void) {
    }

    void StreamCache(void) {
    }

    i32 StreamCacheCheckComplete(void) {
        return 0;
    }

    // ---------------------------------------------------------------------------
    // Rendering extras
    // ---------------------------------------------------------------------------

    i32 NuStreamInit(void) {
        // Retail returns zero.
        return 0;
    }

    void NuSound3SetDataIopBufferSize(void) {
    }

    void NuSound3SetMonoIopBufferSize(void) {
    }

    void NuSound3SetStereoIopBufferSize(void) {
    }

    void NuSound3SetLoadCallback(void) {
    }

    void NuSound3LoadAllSpotFX(void) {
    }

    void NuSound3SetSampleTableFromPakFile(void) {
    }

    void NuSound3SetDuckVol(void) {
    }

    void NuSound3DisplayVoiceInfo(void) {
    }

}

void NuSound3ExitThreads() {
}

// NuSound3StopStereoStream tears down the stream node in a stereo-stream slot.
// The slot's node itself is deleted by NuSound3Update once the sample has
// fully unloaded.
void NuSound3StopStereoStream(i32 stream_index) {
    NuSoundStream *stream = g_NuSoundStreams[stream_index];
    if (stream == NULL) {
        return;
    }

    if (NuSound3Stream::mVoice.obj != NULL && stream->field_0xd != 0) {
        ((NuSoundVoice *)NuSound3Stream::mVoice.obj)->Stop(true);
        NuSound.ReleaseVoice((NuSoundVoice *)NuSound3Stream::mVoice.obj);
        NuSound3Stream::mVoice.Assign(NULL);
        stream->field_0xd = 0;
    }

    NuSoundStreamingSample *sample = stream->stream;
    if (sample != NULL && sample->GetResourceCount() > 0) {
        sample->Release();
        g_NuSoundStreamer->RequestClose(sample);
    }
}

// NuSound3PlayStereoV is the original varargs play entry. The music player
// pushes every argument as a raw dword EXCEPT STARTOFFSET, which is a true
// 8-byte double: VOL is float bits in an int (always 0 from NuMusic, which
// drives volume via NuSound3SetStereoStreamVolume instead), PITCH is a raw
// fixed-point dword the original reads and ignores. The sample's two stream
// buffers are carved out of g_NuSoundStreamBuffers[stream_index * 2] and
// [stream_index * 2 + 1].
i32 NuSound3PlayStereoV(NUSOUNDPLAYTOK token, ...) {
    va_list args;
    va_start(args, token);

    i32 stream_index = 0;
    i32 sample_index = 0;
    i32 volume_bits = 0;
    f32 start_offset = 0.0f;
    bool voice_loop = false;
    bool start_voice = true;

    while (token != NUSOUNDPLAYTOK_END) {
        switch (token) {
            case NUSOUNDPLAYTOK_STEREOSTREAM: {
                stream_index = va_arg(args, i32);
                break;
            }
            case NUSOUNDPLAYTOK_SAMPLE: {
                sample_index = va_arg(args, i32);
                break;
            }
            case NUSOUNDPLAYTOK_VOL: {
                volume_bits = va_arg(args, i32);
                break;
            }
            case NUSOUNDPLAYTOK_PITCH: {
                (void)va_arg(args, i32);
                break;
            }
            case NUSOUNDPLAYTOK_STARTOFFSET: {
                start_offset = (f32)va_arg(args, double);
                break;
            }
            case NUSOUNDPLAYTOK_LOOPTYPE: {
                voice_loop = va_arg(args, i32) != 0;
                break;
            }
            case NUSOUNDPLAYTOK_ONESHOT: {
                start_voice = false;
                break;
            }
            default: {
                break;
            }
        }

        token = (NUSOUNDPLAYTOK)va_arg(args, u32);
    }

    NuSoundStreamingSample *streaming_sample = g_NuSoundSamples.data[sample_index].sample;
    if (streaming_sample->GetThreadQueueCount() < 1) {
        NuSoundStream *stream_ptr = g_NuSoundStreams[stream_index];
        if (stream_ptr != NULL) {
            if (__builtin_expect(stream_ptr->stream != streaming_sample, 1)) {
                NuSound3StopStereoStream(stream_index);
            }

            NuSoundSample::LoadState load_state = stream_ptr->stream->GetLoadState();
            if ((load_state == NuSoundSample::LoadState::NOT_LOADED) && stream_ptr->stream->GetResourceCount() == 0) {
                delete stream_ptr;
                g_NuSoundStreams[stream_index] = NULL;
            } else if (__builtin_expect(g_NuSoundStreams[stream_index] != NULL, 0)) {
                return 0;
            }
        }

        if (streaming_sample != NULL && streaming_sample->GetResourceCount() == 0) {
            NuSoundStream *node = new NuSoundStream;

            node->field_0xd = 0;
            g_NuSoundStreams[stream_index] = node;
            node->field_0xc = voice_loop;
            node->stream = streaming_sample;
            node->ps2volume = volume_bits;
            node->pending_start = start_voice;

            streaming_sample->sound_buffers[0] = &g_NuSoundStreamBuffers[stream_index * 2];
            streaming_sample->sound_buffers[1] = &g_NuSoundStreamBuffers[stream_index * 2 + 1];

            streaming_sample->Reference();
            g_NuSoundStreamer->RequestCue(streaming_sample, voice_loop, start_offset, false);

            return 1;
        }
    }

    return 0;
}

i32 NuMusic::Voice::Play() {
    if ((u32)(this->status - VOICE_STATUS_PLAYING) <= 1) {
        return 1; // already playing (status 6/7)
    }

    i32 idx = this->track_index;
    Track *track = this->tracks[idx];
    if (track == NULL) {
        return -4;
    }
    i32 sub = this->track_sub[idx];
    if (track->file_indexes[sub] == -1) {
        return -4;
    }

    // The other voice decides whether this voice needs a fade-in.
    Voice *other = &the_music_player->voices[1 - this->stream_index];
    Track *otherTrack = other->tracks[other->track_index];

    this->flags &= ~1u; // clear fade-out request

    f32 target_gain;
    f32 fade_rate;
    if (track->clazz == TRACK_CLASS_CUTSCENE || ((u8 *)&track->flags)[1] == 0) {
        // Cutscene tracks and first plays start at full gain.
        this->gain = 1.0f;
        target_gain = 1.0f;
        fade_rate = 0.0f;
    } else {
        // Restart of a looping track: fade in from silence.
        target_gain = 0.0f;
        fade_rate = 1.0f;
    }
    this->fade_rate = fade_rate;

    if (track->clazz != TRACK_CLASS_8 && (otherTrack == NULL || otherTrack->clazz != TRACK_CLASS_8)) {
        if (other->status == VOICE_STATUS_PLAYING_LOADED) {
            // The other voice holds a loaded overlay stream: crossfade - fade
            // this voice in to its existing gain while the other fades out.
            other->flags &= ~1u;
            other->fade_rate = -2.0f;
            target_gain = this->gain;
            this->fade_rate = 1.0f;
        } else if (track->start_offset != 0.0f) {
            this->fade_rate = 1.0f;
            target_gain = 0.0f;
        }
    }

    i32 key_status = NuSound3StreamKeyStatus(this->stream_index);
    if (key_status != NUSOUND_STEREO_STREAM_INACTIVE) {
        if (this->status == VOICE_STATUS_STOPPED) {
            NuSound3ResumeStereoStream(this->stream_index);
            SetStatusFn(VOICE_STATUS_PLAYING, 0x1ca);
            return 1;
        }
    } else if (this->status == VOICE_STATUS_STOPPED) {
        this->status = VOICE_STATUS_READY;
    }
    if ((u32)(this->status - VOICE_STATUS_PLAYING) <= 1) {
        return 1;
    }

    this->gain = target_gain;

    Track *play_track = this->tracks[this->track_index];
    i32 res = NuSound3PlayStereoV(NUSOUNDPLAYTOK_STEREOSTREAM, this->stream_index, //
                                  NUSOUNDPLAYTOK_SAMPLE,
                                  play_track->file_indexes[this->track_sub[this->track_index]], //
                                  NUSOUNDPLAYTOK_PITCH, play_track->pitch,                      //
                                  NUSOUNDPLAYTOK_VOL, 0,                                        //
                                  NUSOUNDPLAYTOK_STARTOFFSET, (f64)play_track->start_offset,    //
                                  NUSOUNDPLAYTOK_LOOPTYPE, (u32)(play_track->clazz - 1U < 2),   //
                                  NUSOUNDPLAYTOK_END);
    this->last_volume = 0.0f;
    if (res == 0) {
        return -6;
    }
    SetStatusFn(VOICE_STATUS_PLAYING, 0x1e3);
    this->tracks[this->track_index]->ManageEntryTime();
    return 1;
}

void NuMusic::Voice::Cue() {
    Play();
}

i32 NuMusic::StopTrack(u32 clazz, i32 fade) {
    if (this == NULL || the_music_player == NULL || this->album == NULL) {
        return 0;
    }

    Track *tracks[6];
    if (__builtin_expect(this->album->GetTracks(clazz, tracks) == 0, 0)) {
        return 0;
    }

    i32 result = 0;
    for (i32 i = 0; i < 6; i++) {
        Track *track = tracks[i];
        if (track == NULL) {
            continue;
        }
        Voice *voice = FindVoiceByTrack(track);
        if (voice == NULL) {
            continue;
        }
        if (__builtin_expect(static_cast<u32>(voice->status - VOICE_STATUS_STOPPING) <= 1, 0)) {
            NuSound3StopStereoStream(voice->stream_index);
            voice->SetStatusFn(VOICE_STATUS_NONE, 0x46a);
            result = 1;
        } else if (__builtin_expect(static_cast<u32>(voice->status - VOICE_STATUS_PLAYING) <= 1, 1)) {
            if (!(fade & 1)) {
                NuSound3StopStereoStream(voice->stream_index);
                voice->SetStatusFn(VOICE_STATUS_NONE, 0x464);
            } else {
                voice->fade_rate = 2.0f;
                voice->flags |= 1u;
            }
            result = 1;
        }
    }

    return result;
}

i32 NuMusic::StopAll(i32 toggle) {
    Voice *voice = &this->voices[0];
    if ((toggle & 2) == 0) {
        // Stop both voices outright, cutscene tracks included.
        NuSound3StopStereoStream(voice->stream_index);
        voice->SetStatusFn(VOICE_STATUS_READY, 0x432);
        voice->fade_rate = 0.0f;
        voice->gain = 0.0f;
        voice->track_index = 0;
        voice->tracks[0] = NULL;
        voice->tracks[1] = NULL;
        NuSound3StopStereoStream(this->voices[1].stream_index);
        this->voices[1].SetStatusFn(VOICE_STATUS_READY, 0x432);
        this->voices[1].fade_rate = 0.0f;
        this->voices[1].gain = 0.0f;
        this->voices[1].track_index = 0;
        this->voices[1].tracks[0] = NULL;
        this->voices[1].tracks[1] = NULL;
        return 1;
    } else {
        // Keep playing cutscene tracks; stop everything else.
        Track *track0 = voice->tracks[voice->track_index];
        if (track0 == NULL || track0->clazz != TRACK_CLASS_CUTSCENE) {
            NuSound3StopStereoStream(voice->stream_index);
            voice->SetStatusFn(VOICE_STATUS_READY, 0x432);
            voice->fade_rate = 0.0f;
            voice->gain = 0.0f;
            voice->track_index = 0;
            voice->tracks[0] = NULL;
            voice->tracks[1] = NULL;
        }

        voice = &this->voices[1];
        Track *track1 = voice->tracks[voice->track_index];
        if (track1 != NULL && track1->clazz == TRACK_CLASS_CUTSCENE) {
            return 1;
        }
        NuSound3StopStereoStream(voice->stream_index);
        voice->SetStatusFn(VOICE_STATUS_READY, 0x432);
    }

    this->voices[1].fade_rate = 0.0f;
    this->voices[1].gain = 0.0f;
    this->voices[1].track_index = 0;
    this->voices[1].tracks[0] = NULL;
    this->voices[1].tracks[1] = NULL;
    return 1;
}

void NuMusic::Process(f32 delta) {
    if (this == NULL || the_music_player == NULL) {
        return;
    }

    // Ramp the fader toward its target, snapping when it arrives.
    if (this->fader_current != this->fader_target) {
        f32 step = delta * this->fader_rate;
        if (this->fader_target > this->fader_current) {
            this->fader_current += step;
            if (this->fader_target <= this->fader_current) {
                this->fader_rate = 0.0f;
                this->fader_current = this->fader_target;
            }
        } else {
            this->fader_current -= step;
            if (this->fader_current <= this->fader_target) {
                this->fader_rate = 0.0f;
                this->fader_current = this->fader_target;
            }
        }
    }

    // Ducking: while a loaded track with a DUCK volume plays, the duck gain
    // sinks to min(1, duck_volume) and ramps at max(1, 1/duck_fade) per second.
    this->duck_gain = 1.0f;
    this->duck_rate = 1.0f;

    f32 duck_target = 1.0f;
    for (i32 vi = 0; vi < 2; ++vi) {
        Track *track = this->voices[vi].tracks[this->voices[vi].track_index];
        if (track != NULL && this->voices[vi].status == VOICE_STATUS_PLAYING_LOADED) {
            duck_target = MIN(duck_target, track->duck_volume);
            this->duck_gain = duck_target;

            f32 rate = 1.0f / track->duck_fade;
            this->duck_rate = MAX(rate, this->duck_rate);
        }
    }

    // Ramp duck_current toward duck_gain without overshooting.
    f32 duck_current = this->duck_current;
    if (duck_current > duck_target) {
        duck_current -= delta * this->duck_rate;
        this->duck_current = MAX(duck_target, duck_current);
    } else if (duck_current < duck_target) {
        duck_current += delta * this->duck_rate;
        this->duck_current = MIN(duck_target, duck_current);
    }

    for (i32 vi = 0; vi < 2; ++vi) {
        Voice *voice = &this->voices[vi];
        Track *track = this->voices[vi].tracks[this->voices[vi].track_index];
        i32 key_status = NuSound3StreamKeyStatus(voice->stream_index);
        if (key_status == NUSOUND_STEREO_STREAM_INACTIVE) {
            NuSound3StopStereoStream(voice->stream_index);
            voice->SetStatusFn(VOICE_STATUS_READY, 0x246);
        } else if (key_status == NUSOUND_STEREO_STREAM_FINISHED) {
            voice->SetStatusFn(VOICE_STATUS_ENDED, 0x24a);
        } else if (key_status == NUSOUND_STEREO_STREAM_PLAYING) {
            voice->SetStatusFn(VOICE_STATUS_PLAYING_LOADED, 0x24e);
        }
        if (voice->status == VOICE_STATUS_PLAYING_LOADED && voice->fade_rate != 0.0f) {
            f32 gain = voice->fade_rate * delta + voice->gain;
            voice->gain = MAX(0.0f, MIN(gain, 1.0f));
            if (voice->gain == 0.0f) {
                voice->fade_rate = 0.0f;
                NuSound3StopStereoStream(voice->stream_index);
                voice->SetStatusFn(VOICE_STATUS_READY, 0x25e);
                voice->fade_rate = 1.0f;
                Track *cur = this->voices[vi].tracks[this->voices[vi].track_index];
                ((u8 *)&cur->flags)[1] = 1;
            }
        }
        f32 duck = this->duck_current;
        f32 class_volume = 1.0f;
        f32 attenuation = 1.0f;
        if (track != NULL) {
            if (((track->flags & 1) != 0) || track->duck_volume != 1.0f) {
                duck = 1.0f;
            }
            i32 index = ClassToIX(track->clazz);
            attenuation = track->attenuation;
            class_volume = this->class_volumes[index];
        }
        f32 volume = duck * this->master_volume * voice->gain * class_volume * this->fader_current * attenuation *
                     this->global_attenuation;
        voice->volume = volume;
        if (volume != voice->last_volume) {
            NuSound3SetStereoStreamVolume(voice->stream_index, (i32)(volume * 16383.0f));
            voice->last_volume = voice->volume;
        }
        voice->play_time = delta + voice->play_time;
    }
}

i32 NuMusic::Voice::Unload() {
    i32 idx = this->track_index;
    if (this->tracks[idx] == NULL || idx == 0) {
        return 0;
    }

    if (this->tracks[idx]->clazz == TRACK_CLASS_8) {
        // Step the class-8 voice back down to its base sub-stream.
        this->track_index = idx - 1;
        if (this->tracks[this->track_index] == NULL) {
            SetStatusFn(VOICE_STATUS_NONE, 0x14b);
            this->tracks[this->track_index] = NULL;
        } else {
            this->gain = 0.0f;
            NuSound3StopStereoStream(this->stream_index);
            Track *track = this->tracks[this->track_index];
            i32 res = NuSound3PlayStereoV(NUSOUNDPLAYTOK_STEREOSTREAM, this->stream_index, //
                                          NUSOUNDPLAYTOK_SAMPLE,
                                          track->file_indexes[this->track_sub[this->track_index]], //
                                          NUSOUNDPLAYTOK_PITCH, 0,                                 //
                                          NUSOUNDPLAYTOK_VOL, 0,                                   //
                                          NUSOUNDPLAYTOK_STARTOFFSET, (f64)track->start_offset,    //
                                          NUSOUNDPLAYTOK_LOOPTYPE, (u32)(track->clazz - 1U < 2),   //
                                          NUSOUNDPLAYTOK_END);
            this->last_volume = 0.0f;
            if (res != 0) {
                SetStatusFn(VOICE_STATUS_CUED, 0x147);
            }
        }
    }

    return 1;
}

i32 NuMusic::Voice::Load(Track *track, i32 trackIndex) {
    if (this->tracks[this->track_index] == track) {
        return 0;
    }

    // Class-8 (overlay) tracks use the voice's second sub-stream so the
    // previous track keeps playing underneath.
    this->track_index = (track->clazz == TRACK_CLASS_8) ? 1 : 0;
    NuSound3StopStereoStream(this->stream_index);
    i32 index = this->track_index;
    this->tracks[index] = track;
    this->track_sub[index] = trackIndex;
    this->flags &= ~2u; // clear paused
    SetStatusFn(VOICE_STATUS_READY, 0x123);
    return 1;
}

i32 NuMusic::PlayTrackI(TRACK_CLASS clazz, u32 unused) {
    (void)unused;
    if (this == NULL || the_music_player == NULL) {
        return -1;
    }

    if (this->album == NULL) {
        return -2;
    }

    Track *track = this->album->GetTrack(clazz);
    if (track == NULL || track->file_indexes[this->track_index] == -1) {
        return -3;
    }

    // A playing cutscene track mutes everything else.
    if (track->clazz != TRACK_CLASS_CUTSCENE) {
        if (FindVoiceByClassAndStatus(TRACK_CLASS_CUTSCENE, VOICE_STATUS_PLAYING) != NULL) {
            return -5;
        }
        if (FindVoiceByClassAndStatus(TRACK_CLASS_CUTSCENE, VOICE_STATUS_PLAYING_LOADED) != NULL) {
            return -5;
        }
    }

    Voice *voice = FindVoiceByTrack(track);
    if (voice != NULL) {
        return voice->Play();
    }

    Voice *target;
    if (track->clazz == TRACK_CLASS_8) {
        // Overlays reuse the class-8 voice if it is idle or stopped.
        target = FindVoiceByClass(TRACK_CLASS_8);
        if (target != NULL) {
            if ((target->status & 0xfffffffdU) != 1) {
                return -5;
            }
            goto load_and_play;
        }
    }

    if (track->clazz == TRACK_CLASS_CUTSCENE) {
        StopAll(2);
    }

    target = FindIdleVoice();
    if (target == NULL) {
        return -5;
    }

load_and_play:
    if (target->Load(track, this->track_index) == 0) {
        return -5;
    }

    return target->Play();
}

i32 NuMusic::PlayTrack(u32 track, u32 unused) {
    return PlayTrackI((TRACK_CLASS)track, unused);
}

i32 NuMusic::PlayTrack(TRACK_CLASS track) {
    return PlayTrack(static_cast<u32>(track), 0);
}

i32 NuMusic::CueTrack(u32 clazz) {
    if (this == NULL || the_music_player == NULL) {
        return -1;
    }
    if (this->album == NULL) {
        return -2;
    }
    Track *track = this->album->GetTrack(clazz);
    if (__builtin_expect(track == NULL || track->file_indexes[this->track_index] == -1, 0)) {
        return -3;
    }
    Voice *voice = FindVoiceByTrack(track);
    if (voice != NULL) {
        return 1;
    }
    if (track->clazz == TRACK_CLASS_8) {
        voice = FindVoiceByClass(TRACK_CLASS_8);
        if (voice != NULL) {
            if ((u32)(voice->status - VOICE_STATUS_PLAYING) < 2) {
                return -5;
            }
            if (voice->Load(track, this->track_index) != 0) {
                voice->Cue();
                return 1;
            }
        }
    }
    voice = FindIdleVoice();
    if (voice == NULL || voice->Load(track, this->track_index) == 0) {
        return -5;
    }
    voice->Cue();
    return 1;
}

void NuSound3Update(void) {
    if (NuSound.GetNumAvailableOutputDevices() < 1) {
        NuSound.Update(NuTimeGetFrameTime());
        return;
    }

    // The listener focus follows the player (NULL on the title screen).
    if (player != NULL) {
        g_NuSoundFocusPosition.x = player->apiobj.position.x;
        g_NuSoundFocusPosition.y = player->apiobj.position.y;
        g_NuSoundFocusPosition.z = player->apiobj.position.z;
        g_NuSoundListener.SetFocusPosition((const VuVec *)&g_NuSoundFocusPosition);
        g_NuSoundListener.EnableFocusPosition();
    } else {
        g_NuSoundListener.DisableFocusPosition();
    }

    NuSound.mutex.Lock();

    // (a) Voices queued for destruction: release and unlink.
    NuSound3Voice *destruction_end = g_NuSoundVoicesPendingDestruction.End();
    for (NuSound3Voice *entry = g_NuSoundVoicesPendingDestruction.Front(); entry != destruction_end;) {
        NuSound3Voice *next = entry->intrusive_next;
        NuSound.ReleaseVoice((NuSoundVoice *)entry->weak_ptr.obj);
        StreamListRemove(&g_NuSoundVoicesPendingDestruction, entry);
        delete entry;
        entry = next;
    }

    // (b) Active sweep: release stopped voices and long-paused ones.
    g_NuSoundNumReplaceableVoices = 0;
    NuSound3Voice *active_end = g_NuSoundVoicesActive.End();
    for (NuSound3Voice *entry = g_NuSoundVoicesActive.Front(); entry != active_end;) {
        NuSound3Voice *next = entry->intrusive_next;
        NuSoundVoice *voice = (NuSoundVoice *)entry->weak_ptr.obj;

        if (voice != NULL) {
            NuSoundVoice::PlayState state = voice->GetState();
            if (state != NuSoundVoice::PLAYSTATE_STOPPED && entry->pause_counter < 16) {
                if ((((NuSoundVoice *)entry->weak_ptr.obj)->flags2 & 8) != 0) {
                    entry->pause_counter++;
                }
                entry = next;
                continue;
            }
            ((NuSoundVoice *)entry->weak_ptr.obj)->Stop(true);
            NuSound.ReleaseVoice((NuSoundVoice *)entry->weak_ptr.obj);
        }

        StreamListRemove(&g_NuSoundVoicesActive, entry);
        delete entry;
        entry = next;
    }

    // (c) Pending playback. The target drains this list every update: entries
    // over the hardware-voice budget, and entries whose CreateVoice fails,
    // are discarded instead of remaining queued for a later frame.
    NuSound3Voice *playback_end = g_NuSoundVoicesPendingPlayback.End();
    for (NuSound3Voice *entry = g_NuSoundVoicesPendingPlayback.Front(); entry != playback_end;) {
        NuSound3Voice *next = entry->intrusive_next;
        StreamListRemove(&g_NuSoundVoicesPendingPlayback, entry);

        NuSoundVoice *voice = NULL;
        if (NuSound.GetVoiceCount() < 30) {
            voice = NuSound.CreateVoice(entry->source, entry->loop);
        }
        if (voice == NULL) {
            delete entry;
            entry = next;
            continue;
        }

        entry->weak_ptr.Set(voice);
        voice->SetAutoDelete(true);
        voice->SetVolume(PS2VolumeToScalar(entry->volume));
        voice->SetPitch(entry->pitch);
        if (entry->source_position != NULL) {
            voice->SetFalloff(entry->falloff_a, entry->falloff_b, NuSoundSystem::FalloffType::LINEAR);
            voice->SetPosition((VuVec *)&entry->position);
            voice->SetSurroundMode(NuSoundSystem::SurroundMode::ZERO);
            voice->SetListeners(NuSound.GetListeners());
        }
        voice->Play();
        StreamListPushBack(&g_NuSoundVoicesActive, entry);
        entry = next;
    }

    NuSound.mutex.Unlock();

    // (d) The two stereo-stream slots: create the pending voice, keep its
    // volume in sync, and tear the stream down once the voice stopped.
    for (i32 i = 0; i < 2; i++) {
        NuSoundStream *stream = g_NuSoundStreams[i];
        if (stream == NULL) {
            continue;
        }

        if (stream->pending_start != 0 && NuSound3Stream::mVoice.obj == NULL) {
            if (stream->stream != NULL && stream->stream->GetLoadState() == NuSoundSample::LoadState::STREAM_READY &&
                stream->stream->GetResourceCount() > 0 && stream->stream->GetThreadQueueCount() == 0) {
                NuSound3Stream::mVoice.Set(NuSound.CreateVoice(stream->stream, stream->field_0xc != 0));
                if (NuSound3Stream::mVoice.obj != NULL) {
                    ((NuSoundVoice *)NuSound3Stream::mVoice.obj)->SetAutoDelete(false);
                    ((NuSoundVoice *)NuSound3Stream::mVoice.obj)->SetVolume(PS2VolumeToScalar(stream->ps2volume));
                    ((NuSoundVoice *)NuSound3Stream::mVoice.obj)->Play();
                    stream->field_0xd = 1;
                    stream->pending_start = 0;
                }
            }
        } else if (NuSound3Stream::mVoice.obj != NULL) {
            if (stream->field_0xd != 0) {
                ((NuSoundVoice *)NuSound3Stream::mVoice.obj)->SetVolume(PS2VolumeToScalar(stream->ps2volume));
                if (((NuSoundVoice *)NuSound3Stream::mVoice.obj)->GetState() == NuSoundVoice::PLAYSTATE_STOPPED) {
                    NuSound3StopStereoStream(i);
                }
            }
        }

        if (stream->field_0xd != 0 && NuSound3Stream::mVoice.obj == NULL) {
            NuSound3StopStereoStream(i);
        }

        // Slot cleanup: fully unloaded streams free their node.
        if (stream->stream->GetLoadState() == NuSoundSample::LoadState::NOT_LOADED &&
            stream->stream->GetResourceCount() == 0 && stream->stream->GetThreadQueueCount() == 0) {
            delete stream;
            g_NuSoundStreams[i] = NULL;
        }
    }

    // (e) Wake the sample loader when the registered request table changes.
    for (i32 i = 0; i < g_NuSoundNumLoadBitShorts; i++) {
        if (g_NuSoundLoadBitsCache[i] != g_NuSoundLoadBits[i]) {
            memmove(g_NuSoundLoadBitsCache, g_NuSoundLoadBits,
                    static_cast<usize>(g_NuSoundNumLoadBitShorts) * sizeof(u16));

            pthread_mutex_lock(&g_NuSoundLoadTrigger.mutex);
            if (!g_NuSoundLoadTrigger.a) {
                bool broadcast = g_NuSoundLoadTrigger.b;
                g_NuSoundLoadTrigger.a = true;
                if (broadcast) {
                    pthread_cond_broadcast(&g_NuSoundLoadTrigger.cond);
                } else {
                    pthread_cond_signal(&g_NuSoundLoadTrigger.cond);
                }
            }
            pthread_mutex_unlock(&g_NuSoundLoadTrigger.mutex);
        }
    }

    NuSound.Update(NuTimeGetFrameTime());
}

