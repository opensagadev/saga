// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "globals.h"
#include "gameapi/edtools/edfile.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/world/mission.h"
#include "legoapi/props/system/socksys.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nusound/nusound.h"
#include "nu2api/nufile/nufile.h"
#include <stdio.h>
#include <string.h>
#include "legoapi/items/base/apiobject.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameframework/saveload.h"
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
#include "nu2api/nucore/nuhgobj.h"
#include "legoapi/render/fx/edsplines.h"
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
#include "legoapi/actions/combat/shovesys.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/audio/specialsfx.h"
#include "legoapi/core/config/cheats.h"

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

struct nuhspecial_s;

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

// Game-specific pre-load limits. These must be applied before WorldInfo
// reserves each level's fixed-size gameplay systems.

i32 SpecialSfxLoad(char *path, WORLDINFO_s *world) {
    if (world == NULL) {
        return 0;
    }

    EdFileSetMedia(1);
    if (EdFileOpen(path, NUFILE_READ) == 0) {
        return 0;
    }

    i32 version = EdFileReadInt();
    if (version > 2) {
        EdFileClose();
        return 0;
    }

    i32 count = EdFileReadInt();
    if (count > 0xc0) {
        count = 0xc0;
    }
    EdFileReadInt();
    world->special_sfx_count = 0;

    char name[72];
    for (i32 i = 0; i < count; i++) {
        specialsfx_s *entry = &world->special_sfx[i];
        i16 length = EdFileReadShort();
        EdFileRead(name, length);
        name[length] = '\0';
        NuSpecialFind(world->current_gscn, &entry->special, name, 1);
        entry->event_count = EdFileReadChar();
        if (version >= 2) {
            entry->type = EdFileReadChar();
        }
        world->special_sfx_count++;
    }

    SPECIALSFXEVENT_s *event = world->special_sfx_events;
    for (i32 i = 0; i < count; i++) {
        specialsfx_s *entry = &world->special_sfx[i];
        for (i32 j = 0; j < entry->event_count; j++, event++) {
            i16 length = EdFileReadShort();
            EdFileRead(name, length);
            name[length] = '\0';
            event->sfx_id = static_cast<i16>(GetSfxId(name));
            event->flags = static_cast<u8>(EdFileReadChar());
            event->looping = IsSfxLooping(event->sfx_id);
            event->trigger_frame = EdFileReadFloat();
            event->previous_frame = EdFileReadFloat();
            world->special_sfx_event_count++;
        }
    }

    event = world->special_sfx_events;
    if (world->special_sfx_count > 0) {
        SPECIALSFXEVENT_s *event_end = event + world->special_sfx_event_count;
        specialsfx_s *entry = world->special_sfx;
        specialsfx_s *entry_end = entry + world->special_sfx_count;
        while (entry < entry_end) {
            SPECIALSFXEVENT_s *next = event == event_end
                                          ? NULL
                                          : reinterpret_cast<SPECIALSFXEVENT_s *>(reinterpret_cast<uintptr_t>(event) +
                                                                                  sizeof(SPECIALSFXEVENT_s));
            entry->events = event;
            if (!(entry->event_count > 1)) {
                if (event != NULL)
                    event->next = NULL;
                event = next;
            } else {
                SPECIALSFXEVENT_s *previous = event;
                for (i32 j = 0; j < entry->event_count - 1; ++j) {
                    previous->next = next;
                    event = reinterpret_cast<SPECIALSFXEVENT_s *>(reinterpret_cast<uintptr_t>(next) +
                                                                  sizeof(SPECIALSFXEVENT_s));
                    previous = next;
                    next = event;
                }
                if (previous != NULL)
                    previous->next = NULL;
            }
            ++entry;
        }
    }

    EdFileClose();
    return 1;
}

void LoadSpecialSfxFile(WORLDINFO *world) {
    char path[268];
    sprintf(path, "%s.sfx", world->config_file);
    if (NuFileExists(path) != 0) {
        if (SpecialSfxLoad(path, world) != 0) {
            i32 count = world->special_sfx_count;
            specialsfx_s *entry = world->special_sfx;
            if (count > 0) {
                i32 i = 0;
                do {
                    if (entry->type == 1) {
                        SetupBlowupSfx(world, entry);
                    }
                    ++i;
                    count = world->special_sfx_count;
                    ++entry;
                } while (i < count);
            }
        }
    }
}

i32 SpecialSfxAdd(i32 special_index) {
    if (WORLD->special_sfx_count < 0xc0) {
        nuhspecial_s special;
        NuGScnGetSpecial(&special, WORLD->current_gscn, special_index);
        if (NuSpecialExistsFn(&special) != 0) {
            specialsfx_s *entry = &WORLD->special_sfx[WORLD->special_sfx_count];
            entry->type = 0;
            entry->special = special;
            entry->events = NULL;
            entry->event_count = 0;
            return ++WORLD->special_sfx_count;
        }
    }
    return -1;
}

void UpdateSpecialSfx(WORLDINFO_s *world) {
    for (i32 i = 0; i < world->special_sfx_count; i++) {
        specialsfx_s *entry = &world->special_sfx[i];
        SPECIALSFXEVENT_s *event = entry->events;
        if (NuSpecialExistsFn(&entry->special) == 0 || NuSpecialGetVisibilityFn(&entry->special) == 0) {
            continue;
        }

        nuinstanim_s *animation = NuSpecialGetInstAnim(&entry->special);
        if (animation == NULL) {
            continue;
        }
        nuanimdata_s *animation_data = entry->special.scene->instance_animation_data[animation->anim_ix];
        if (animation_data == NULL) {
            continue;
        }

        f32 end_frame = NuAnimEndFrameOld(animation_data);
        f32 frame = animation->ltime;
        if ((animation->flags & NUINSTANIM_FLAG_PLAYING) == 0 && entry->animation_playing == 0) {
            for (i32 j = 0; j < entry->event_count; j++) {
                if (event != NULL) {
                    event->triggered = 0;
                    if (frame < end_frame && !event->looping) {
                        event->previous_frame = 0.0f;
                    }
                }
                event = event->next;
            }
            entry->animation_playing = 0;
            continue;
        }

        NUVEC *position = NuSpecialGetDrawPos(&entry->special);
        i32 direction = GetAnimDirection(animation);
        for (i32 j = 0; j < entry->event_count; j++) {
            if (event != NULL) {
                bool play = false;
                if (event->looping) {
                    if ((direction == 0 && event->forward) || (event->forward && event->reverse)) {
                        if (event->trigger_frame <= frame && frame <= event->previous_frame) {
                            play = true;
                        }
                    } else if (direction == 1 && event->reverse && frame <= event->trigger_frame &&
                               frame >= event->previous_frame) {
                        play = true;
                    }
                } else if (event->forward && event->reverse) {
                    if ((event->previous_frame < event->trigger_frame && event->trigger_frame <= frame) ||
                        (event->trigger_frame < event->previous_frame && frame <= event->trigger_frame)) {
                        play = true;
                    }
                } else if (event->forward) {
                    if ((event->previous_frame < event->trigger_frame && event->trigger_frame <= frame) ||
                        (event->previous_frame == 0.0f && event->trigger_frame == 1.0f)) {
                        play = true;
                    }
                } else if (event->reverse) {
                    if ((event->trigger_frame < event->previous_frame && frame <= event->trigger_frame) ||
                        (end_frame == event->previous_frame && end_frame == event->trigger_frame)) {
                        play = true;
                    }
                }

                if (play) {
                    GameAudio_PlaySfxById(event->sfx_id, position, 0, 0);
                }
                if (!event->looping) {
                    event->previous_frame = frame;
                }
            }
            event = event->next;
        }
        entry->animation_playing = (animation->flags & NUINSTANIM_FLAG_PLAYING) != 0;
    }
}

void SetSpecialSfxBits(i32 *sfx_ids, i32 *sfx_count, WORLDINFO_s *world) {
    if (world == NULL) {
        return;
    }

    for (i32 i = 0; i < world->special_sfx_event_count; ++i) {
        i32 id = world->special_sfx_events[i].sfx_id;
        if (id != -1) {
            sfx_ids[*sfx_count] = id;
            ++*sfx_count;
        }
    }
}

