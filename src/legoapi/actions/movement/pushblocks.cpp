// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "legoapi/actions/movement/pushblocks.h"

#include <math.h>
#include "nu2api/nu3d/nuspecial.h"
#include "decomp.h"
#include "nu2api/nucore/nuanim3.h"
#include "globals.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/world/level.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/actions/movement/pushblocks_types.h"
#include "legoapi/gizmos/door/push.h"
#include "gameapi/edtools/edfile.h"
#include "legoapi/world/world.h"
#include "legoapi/render/light/surfaces.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nufloat.h"
#include <string.h>
#include "legoapi/world/area.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/items/base/animpacket.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "nu2api/nu3d/nutex.h"
#include <stdlib.h>
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/characters/motion/action_info.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/world/mission.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/numtx.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/base/apiobject.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numusic/sfx.h"
#include <float.h>
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/ai/aisys/aipath.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "nu2api/nucore/numechptr.hpp"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nu3d/nuspline.h"
#include <stdio.h>
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nufile/nufile.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/core/input/timer.h"
#include "nu2api/nucore/numemory.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/actions/character/speederchase.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/gizmo/base/GizBuildItObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizLeverObjectInterface.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizPanelObjectInterface.h"
#include "legoapi/gizmo/base/GizTurretObjectInterface.h"
#include "legoapi/gizmo/base/HatMachineObjectInterface.h"
#include "legoapi/gizmo/base/TeleportObjectInterface.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "gameframework/saveload.h"
#include "nu2api/nu3d/nuhspecial.h"
#include "legoapi/world/levels/podrace.h"
#include <stdint.h>
#include "nu2api/nucore/nulist.h"
#include "gameapi/ai/aisys/aiscript_types.h"
#include "editor/edpath.h"
#include "batman.h"
#include "nu2api/nucore/common.h"
#include "editor/aieditor_state.h"
#include "nu2api/numath/nuplane.h"
#include "editor/aieditor_settings.h"
#include "nu2api/nu3d/numtl.h"
#include "editor/edpath_types.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include "legoapi/render/fx/game_deb.h"
#include <stddef.h>
#include <ctype.h>
#include "legoapi/world/areas.h"
#include "nu2api/numusic/numusic.h"
#include <stdarg.h>
#include <limits.h>
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include "nu2api/nucore/nuvideo.h"
#include <cstdio>
#include <new>
#include "nu2api/nu3d/nugscn.h"
#include <GLES2/gl2.h>
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "legoapi/actions/combat/shovesys.h"
#include "legoapi/ai/core/gameai.h"
#include "legoapi/misc/utilities.h"

pushblock_s *BlockInBlock(WORLDINFO_s *, pushblock_s *, i32, pushblock_s **);

u32 (*CanPushBlocksFn)(GameObject_s *) = NULL;
i32 pushposincrease;
i32 runoutofpostabspace;

void ResetPushProgress(WORLDINFO_s *, void *);
void ResetSinglePushBlockHeight(WORLDINFO_s *, pushblock_s *, i32);
i32 TerrainBlockOnBlock(WORLDINFO_s *, pushblock_s *, NUVEC *, f32 *);

pushblock_s *NearestPushBlock(WORLDINFO_s *world, nuvec_s *position, float range);

void PushSeekComplete(pushblock_s *block, i32 index);

i32 OtherBlockInRange(WORLDINFO_s *world, pushblock_s *block, nuvec_s *position, i32 excluded);

void ResetSinglePushBlock(WORLDINFO_s *, pushblock_s *block, i32);

pushblock_s *NearestFacingPushBlock(WORLDINFO_s *world, GameObject_s *object, float range);

void ResetSinglePushBlockHeight(WORLDINFO_s *world, pushblock_s *block, i32 index);

i32 GizPushBlock_EndFrameCompleted(pushblock_s *push_block, i32 output_index);

i32 PushBlock(GameObject_s *object);

DECOMP_ASSERT(sizeof(PUSHPROGRESS) == 0x24c, "push-block progress size");

enum PUSHBLOCK_CONFIG_FLAGS {
    PUSHBLOCK_FLAG_VISIBLE = 1 << 2,
    PUSHBLOCK_FLAG_STATE = 1 << 1,
};

i32 pushblock_gizmotype_id;

void ResetPushProgress(WORLDINFO_s *world, void *progress);
void ResetSinglePushBlock(WORLDINFO_s *world, pushblock_s *push_block, i32 index);
void MoveBlocks(WORLDINFO_s *world, pushblock_s *push_block, i32 index, NUVEC *velocity);
void PushSeekComplete(pushblock_s *push_block, i32 snap_index);

extern i32 LEGOHINT_PUSHBLOCKS;
static NUVEC hothbtestpos = {29.0f, 0.0f, 13.0f};
i32 SnapPosTaken(WORLDINFO_s *, pushblock_s *, NUVEC *, i32);
i32 OtherBlockInRange(WORLDINFO_s *, pushblock_s *, NUVEC *, i32);
void UpdatePushBlocks(void *world_ptr, void *, float);

i32 GizPush_GetNumOutputs(GIZMO *gizmo);

static char outputName[13] = "Any Complete";

char *GizPush_GetOutputName(GIZMO *gizmo, i32 output_index);

i32 GizPush_GetOutput(GIZMO *gizmo, i32 output_index, i32);

void ResetPushProgress(WORLDINFO_s *world, void *progress_data);

extern "C" i16 id_DODGEM;

extern i32 LevFlag[4];

extern "C" i16 id_LANDSPEEDER;

extern "C" {
    extern i16 id_MINIATST, id_MINIATTE;
}

extern i16 id_MINIATST, id_MINIATTE;

extern AILOCATOR_s * locator;

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

struct HINT_s;

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

extern "C" i16 id_LANDSPEEDER;

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

extern "C" {
    extern i16 id_LANDSPEEDER;
}

struct nuhspecial_s;
struct nuvec_s;

extern AIACTIONDEF api_aiactiondefs[55];
extern AICONDITIONDEF api_aiconditiondefs[50];

DECOMP_ASSERT(sizeof(api_aiactiondefs) == 0x294, "API action registry size");

DECOMP_ASSERT(sizeof(api_aiconditiondefs) == 0x258, "API condition registry size");

struct eduimenu_s;
struct eduiitem_s;

struct EDCREATURE_s;

extern "C" {
    struct EDCREATURE_s;
}

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

struct AIPACKET_s;
struct AISYS_s;

// Original writable flagremaptab at 0x668520, 32 pairs of 32-bit masks.
extern "C" {
extern u32 flagremaptab[32][2];
}

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

pushblock_s *BlockInBlock(WORLDINFO_s *world, pushblock_s *block, i32 excluded, pushblock_s **support);

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

void MoveBlocksOverBlock(WORLDINFO_s *, pushblock_s *, i32, nuvec_s *);
void MoveBlocks(WORLDINFO_s *world, pushblock_s *block, i32 index, nuvec_s *velocity);

void MoveBlocksOverBlock(WORLDINFO_s *world, pushblock_s *block, i32 excluded, nuvec_s *velocity);

extern "C" {
    extern i16 id_MINIATST, id_MINIATTE;
}

i32 SnapPosTaken(WORLDINFO_s *world, pushblock_s *, nuvec_s *position, i32 excluded_index);

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

extern "C" {
extern u32 flagremaptab[32][2];
}

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

struct pushblock_s;

i32 TerrainBlockOnBlock(WORLDINFO_s *world, pushblock_s *block, nuvec_s *points, float *heights);

// Original 0x378fb0: the optimized rotating scan shares the terrain engine state.

static void *PushBlocks_ReserveBufferSpace(void *world_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    world->push_blocks = NULL;
    world->push_block_count = 0;
    if (world->current_level->max_push_blocks != 0) {
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
        world->push_blocks = static_cast<pushblock_s *>(world->giz_buffer.void_ptr);
        world->giz_buffer.addr += world->current_level->max_push_blocks * sizeof(pushblock_s);
    }
    return world->push_blocks;
}

static i32 Push_GetMaxGizmos(void *world_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    return world != NULL ? world->current_level->max_push_blocks : 0;
}

static char *GizPush_GetGizmoName(GIZMO *gizmo) {
    if (gizmo == NULL) {
        return NULL;
    }
    return static_cast<pushblock_s *>(gizmo->object)->name;
}

i32 GizPush_GetNumOutputs(GIZMO *gizmo) {
    return static_cast<pushblock_s *>(gizmo->object)->output_count;
}

static void Push_ClearProgress(void *, void *progress_ptr) {
    PUSHPROGRESS *progress = static_cast<PUSHPROGRESS *>(progress_ptr);
    if (progress != NULL) {
        progress->state_mask[0] = 0;
        progress->visible_mask[0] = ~0u;
        progress->position_mask[0] = 0;
        NuMemSet128(progress->positions, 0, 0);
    }
}

static void *Push_AllocateProgressData(VARIPTR *buffer, VARIPTR *buffer_end) {
    return GizmoBufferAlloc(buffer, buffer_end, 0x24c);
}

static void Push_SetVisibility(GIZMO *gizmo, i32 visible) {
    pushblock_s *push_block = static_cast<pushblock_s *>(gizmo->object);
    push_block->push_visible = visible != 0;
    NuSpecialSetVisibility(&push_block->special, visible);
    for (i32 index = 0; index < push_block->end_position_count; ++index) {
        NuSpecialSetVisibility(&push_block->end_position_specials[index], push_block->push_visible);
    }
}

static void Push_AddGizmos(GIZMOSYS *gizmo_sys, i32 type_id, void *world_ptr, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    for (i32 index = 0; index < world->push_block_count; ++index) {
        if (NuStrLen(world->push_blocks[index].name) != 0) {
            AddGizmo(gizmo_sys, type_id, NULL, &world->push_blocks[index]);
        }
    }
}

static i32 edpush_Load(void *world_ptr, void *) {
    static i32 version = -1;

    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (world->push_block_count != 0) {
        return 0;
    }

    version = EdFileReadInt();
    world->push_block_count = EdFileReadInt();
    i32 valid_count = 0;
    char special_name[256];
    for (i32 file_index = 0; file_index < world->push_block_count; ++file_index) {
        pushblock_s *push_block = &world->push_blocks[valid_count];
        memset(special_name, 0, sizeof(special_name));
        EdFileRead(special_name, EdFileReadChar());

        i32 special_missing = 1;
        if (Gizmo_FindNuSpecial(world->current_gscn, &push_block->special, special_name, 1, world->gizmo_sys,
                                const_cast<char *>("PushBlock"), push_block->name) != 0) {
            NuStrCpy(push_block->name, NuSpecialGetName(&push_block->special));
            special_missing = 0;
        }

        push_block->snap_distance = EdFileReadFloat();
        push_block->flags_0cb &= ~0x04;
        if (EdFileReadChar() != 0) {
            push_block->flags_0cb |= 0x04;
        }
        push_block->flags_0cb &= ~0x08;
        if (EdFileReadChar() != 0) {
            push_block->flags_0cb |= 0x08;
        }
        push_block->flags_0ca &= ~0x10;
        if (EdFileReadChar() != 0) {
            push_block->flags_0ca |= 0x10;
        }
        push_block->flags_0ca &= ~0x20;
        if (EdFileReadChar() != 0) {
            push_block->flags_0ca |= 0x20;
        }

        if (version > 3) {
            push_block->flags_0cb &= ~0x20;
            if (EdFileReadChar() != 0) {
                push_block->flags_0cb |= 0x20;
            }
            push_block->flags_0cb &= ~0x40;
            if (EdFileReadChar() != 0) {
                push_block->flags_0cb |= 0x40;
            }
        }
        if (version > 4) {
            push_block->flags_0ca &= ~0x40;
            if (EdFileReadChar() != 0) {
                push_block->flags_0ca |= 0x40;
            }
            push_block->flags_0ca &= ~0x80;
            if (EdFileReadChar() != 0) {
                push_block->flags_0ca |= 0x80;
            }
        }

        push_block->end_position_count = 0;
        if (version > 2) {
            const u8 end_position_count = static_cast<u8>(EdFileReadChar());
            push_block->end_position_count = end_position_count;
            for (i32 end_index = 0; end_index < end_position_count; ++end_index) {
                memset(special_name, 0, sizeof(special_name));
                EdFileRead(special_name, EdFileReadChar());
                special_name[16] = '\0';
                if (Gizmo_FindNuSpecial(world->current_gscn, &push_block->end_position_specials[end_index],
                                        special_name, 1, world->gizmo_sys, const_cast<char *>("PushBlockLinkObject"),
                                        push_block->name) == 0) {
                    --push_block->end_position_count;
                }
            }
        }
        if (special_missing == 0) {
            ++valid_count;
        }
    }
    world->push_block_count = valid_count;
    return 1;
}

char *GizPush_GetOutputName(GIZMO *gizmo, i32 output_index) {

    pushblock_s *push_block = static_cast<pushblock_s *>(gizmo->object);
    if (output_index < 0 || output_index > push_block->output_count) {
        return NULL;
    }
    if (output_index == 0) {
        return const_cast<char *>("Any Complete");
    }
    NuIToA(output_index, outputName, 10);
    NuStrCat(outputName, " Complete");
    return outputName;
}

static void Push_StoreProgress(void *world_ptr, void *, void *progress_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    PUSHPROGRESS *progress = static_cast<PUSHPROGRESS *>(progress_ptr);
    if (progress == NULL) {
        return;
    }

    Push_ClearProgress(NULL, progress);
    if (world == NULL || world->push_blocks == NULL || world->push_block_count <= 0) {
        return;
    }

    pushblock_s *push_block = world->push_blocks;
    for (i32 index = 0; index < world->push_block_count; ++index, ++push_block) {
        if (index == 16) {
            break;
        }
        const i32 mask_index = index >> 5;
        const u32 mask = 1u << (index & 31);
        if ((push_block->flags_0ca & PUSHBLOCK_FLAG_VISIBLE) == 0) {
            progress->visible_mask[mask_index] &= ~mask;
        }
        if ((push_block->flags_0cb & PUSHBLOCK_FLAG_STATE) != 0) {
            progress->state_mask[mask_index] |= mask;
        }

        if (NuSpecialExistsFn(&push_block->special) != 0) {
            progress->positions[index] = *NUMTX_GET_ROW_VEC(NuSpecialGetInstanceMtx(&push_block->special), 3);
        } else {
            progress->positions[index] = v000;
        }

        for (i32 end_index = 0; end_index < push_block->end_position_count; ++end_index) {
            nuhspecial_s *special = &push_block->end_position_specials[end_index];
            if (NuSpecialExistsFn(special) != 0) {
                progress->end_positions[end_index][index] = *NUMTX_GET_ROW_VEC(NuSpecialGetInstanceMtx(special), 3);
            } else {
                progress->end_positions[end_index][index] = v000;
            }
        }
        progress->position_mask[mask_index] |= mask;
    }
}

void KnockPushBlock(pushblock_s *block, nuvec_s *direction) {
    if (block != NULL) {
        block->velocity.x = -direction->x;
        block->velocity.y = -direction->y;
        block->velocity.z = -direction->z;
        NuVecScale(&block->velocity, &block->velocity, 0.005f);
        block->runtime_flags_0c8 |= 0x80;
    }
}

static i32 Pushblocks_BoltHitPlat(void *world_ptr, void *push_blocks_ptr, BOLT *bolt, unsigned char *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    pushblock_s *push_block = static_cast<pushblock_s *>(push_blocks_ptr);
    if (push_block != NULL) {
        for (i32 index = 0; index < world->push_block_count; ++index, ++push_block) {
            if ((push_block->flags_0ca & PUSHBLOCK_FLAG_VISIBLE) == 0 || (push_block->flags_0cb & 1) == 0 ||
                (push_block->flags_0cb & PUSHBLOCK_FLAG_STATE) != 0) {
                continue;
            }
            const i32 platform = FindPlatInst(NuSpecialGetInstanceix(&push_block->special));
            if (!(platform == -1 || platform != bolt->hit_platform)) {
                if (bolt->owner != NULL) {
                    NUVEC direction;
                    NuVecSub(&direction, &bolt->owner->apiobj.collision_position, push_block->position);
                    if (direction.x == direction.z) {
                        if (direction.x < 0.0f) {
                            direction.x = -QRAND_FLOAT();
                            direction.z = -QRAND_FLOAT();
                        } else {
                            direction.x = QRAND_FLOAT();
                            direction.z = QRAND_FLOAT();
                        }
                    }
                    if (NuFabs(direction.x) > NuFabs(direction.z)) {
                        direction.z = 0.0f;
                    } else {
                        direction.x = 0.0f;
                    }
                    NuVecNorm(&direction, &direction);
                    KnockPushBlock(push_block, &direction);
                }
                return 1;
            }
        }
    }
    return 0;
}

void ResetSinglePushBlock(WORLDINFO_s *, pushblock_s *block, i32) {
    block->platform_id = FindPlatInst(NuSpecialGetInstanceix(&block->special));
    SetAnimFrame(&block->special, 1.0f);
    block->runtime_flags_0c8 &= 0xf2;
    block->runtime_flags_0c9 &= 0xf0;
    block->flags_0cb &= ~2u;
    block->completion_flags &= 0xf807;
    block->packed_state_flags &= 0xfffc7fff;
    block->pushing_object = NULL;

    if (NuSpecialExistsFn(&block->special) == 0) {
        block->bounds_min = v000;
        block->bounds_max = v000;
        block->position = &v000;
        return;
    }

    NuSpecialGetBounds(&block->special, &block->bounds_min, &block->bounds_max);
    NUMTX *matrix = NuSpecialGetInstanceMtx(&block->special);
    block->position = reinterpret_cast<NUVEC *>(&matrix->m30);
    if ((block->runtime_flags_0c9 & 0x70) != 0 && runoutofpostabspace == 0) {
        *block->position = block->snap_positions[0];
    }
}

static void Push_Activate(GIZMO *gizmo, i32 active) {
    if (gizmo != NULL) {
        pushblock_s *push_block = static_cast<pushblock_s *>(gizmo->object);
        if (active != 0) {
            push_block->flags_0cb |= 1;
            WORLDINFO *world = WORLD;
            pushblock_s *candidate = world->push_blocks;
            for (i32 index = 0; index < world->push_block_count; ++index, ++candidate) {
                if (candidate == push_block) {
                    ResetSinglePushBlock(world, push_block, index);
                    break;
                }
            }
            push_block->flags_0ca |= PUSHBLOCK_FLAG_VISIBLE;
        } else {
            push_block->flags_0cb &= ~1;
        }
    }
}

void ResetPushProgress(WORLDINFO_s *world, void *progress_data) {
    pushblock_s *block = world->push_blocks;
    if (block == NULL || world->push_block_count <= 0) {
        return;
    }

    PUSHPROGRESS *progress = static_cast<PUSHPROGRESS *>(progress_data);
    for (i32 index = 0; index < world->push_block_count; ++index, ++block) {
        if (index < 16 && progress != NULL) {
            const u32 bit = 1u << index;
            block->flags_0cb = (block->flags_0cb & ~2u) | (((progress->state_mask[0] & bit) != 0) << 1);
            block->push_visible = (progress->visible_mask[0] & bit) != 0;
            if (!block->push_visible) {
                NuSpecialSetVisibility(&block->special, 0);
                block->push_visible = 0;
                for (i32 output = 0; output < block->end_position_count; ++output) {
                    NuSpecialSetVisibility(&block->end_position_specials[output], 0);
                }
            }

            if ((progress->position_mask[0] & bit) == 0) {
                continue;
            }
            nuinstanim_s *animation = NuSpecialGetInstAnim(&block->special);
            if (block->push_visible || animation == NULL) {
                NUMTX *matrix = NuSpecialGetInstanceMtx(&block->special);
                *NUMTX_GET_ROW_VEC(matrix, 3) = progress->positions[index];
                NuSpecialUpdate(&block->special);
                if (block->end_position_count != 0) {
                    i32 output = 0;
                    do {
                        matrix = NuSpecialGetInstanceMtx(&block->end_position_specials[output]);
                        *NUMTX_GET_ROW_VEC(matrix, 3) = progress->end_positions[output][index];
                        NuSpecialUpdate(&block->end_position_specials[output]);
                    } while (block->end_position_count > ++output);
                }
            } else {
                NUMTX evaluated;
                NUMTX *matrix = NuSpecialGetInstanceMtx(&block->special);
                EvalAnim(&block->special, 1.0f, &evaluated, 0);
                *NUMTX_GET_ROW_VEC(matrix, 3) = *NUMTX_GET_ROW_VEC(&evaluated, 3);
                NuSpecialUpdate(&block->special);
                if (block->end_position_count != 0) {
                    i32 output = 0;
                    do {
                        nuhspecial_s *special = &block->end_position_specials[output];
                        matrix = NuSpecialGetInstanceMtx(special);
                        EvalAnim(special, 1.0f, &evaluated, 0);
                        *NUMTX_GET_ROW_VEC(matrix, 3) = *NUMTX_GET_ROW_VEC(&evaluated, 3);
                        NuSpecialUpdate(special);
                    } while (block->end_position_count > ++output);
                }
            }
        }
    }
}

static void Push_Reset(void *world, void *, void *progress) {
    ResetPushProgress(static_cast<WORLDINFO_s *>(world), progress);
}

pushblock_s *NearestFacingPushBlock(WORLDINFO_s *world, GameObject_s *object, float range) {
    pushblock_s *nearest = NULL;
    if (world == NULL)
        return nearest;
    pushblock_s *blocks = world->push_blocks;
    if (blocks == NULL)
        return nearest;
    if (world->push_block_count == 0)
        return nearest;
    if (LEGOCONTEXT_PUSH == -1)
        return nearest;
    if (object->character_context != LEGOCONTEXT_PUSH)
        return nearest;
    f32 upper = object->apiobj.upper_position.y;
    u16 facing = object->apiobj.movement_facing_angle;
    f32 lower = object->apiobj.lower_position.y;
    f32 adjust = (upper - lower) * 0.25f;
    f32 max_y = upper - adjust;
    f32 min_y = lower + adjust;
    i32 mode;
    if (facing == 0) {
        mode = 2;
    } else {
        u16 angle16 = facing;
        i32 angle = facing;
        if (angle > 0xfc72) {
            mode = 2;
        } else if (angle16 <= 0x38d) {
            mode = 2;
        } else if (angle16 == 0x4000) {
            mode = 0;
            goto have_mode;
        } else if (angle <= 0x3c72) {
            goto check_8000;
        } else if (angle > 0x438d) {
            goto check_8000;
        } else {
            mode = 0;
            goto have_mode;
        }
    check_8000:
        if (angle16 == 0x8000) {
            mode = 3;
            goto have_mode;
        } else if (angle <= 0x7c72) {
            goto check_c000;
        } else if (angle > 0x838d) {
            goto check_c000;
        } else {
            mode = 3;
            goto have_mode;
        }
    check_c000:
        if (angle16 == 0xc000) {
            mode = 1;
            goto have_mode;
        } else if (angle <= 0xbc72) {
            return nearest;
        } else if (angle > 0xc38d) {
            return nearest;
        } else {
            mode = 1;
        }
    have_mode:;
    }
    nearest = NULL;
    if (world->push_block_count <= 0)
        return nearest;
    f32 nearest_dist = 1000000000.0f;
    for (i32 i = 0; i < world->push_block_count; ++i, blocks = world->push_blocks) {
        pushblock_s *block = &blocks[i];
        if (NuSpecialExistsFn(&block->special) != 0) {
            if (NuSpecialGetOnScreenFn(&block->special) == 0)
                continue;
            if (NuSpecialGetVisibilityFn(&block->special) == 0)
                continue;
        }
        if ((block->packed_state_flags & 0x4000100) != 0)
            continue;
        if ((block->flags_0cb & 2) != 0)
            continue;
        NUVEC *pos = block->position;
        f32 top = pos->y + fabsf(block->bounds_max.y);
        f32 bottom = pos->y - fabsf(block->bounds_min.y);
        f32 min_z = block->bounds_min.z;
        f32 max_z = block->bounds_max.z;
        f32 min_x = block->bounds_min.x;
        if (top > max_y && bottom > max_y)
            continue;
        if (__builtin_expect(min_y > top, 0)) {
            if (min_y > bottom)
                continue;
        }
        f32 dx = pos->x - object->apiobj.collision_position.x;
        f32 dz = pos->z - object->apiobj.collision_position.z;
        f32 dy = pos->y - object->apiobj.collision_position.y;
        NUVEC delta;
        delta.x = dx;
        delta.y = dy;
        delta.z = dz;
        f32 gap;
        if (mode == 2) {
            goto mode2_geometry;
        }
        if (mode == 3) {
            goto mode3_geometry;
        }
        if (mode == 1) {
            goto mode1_geometry;
        }
    mode0_geometry: {
        f32 block_min_z = min_z + pos->z - 0.01f;
        f32 block_max_z = pos->z + max_z - 0.01f;
        if (!(block_min_z < object->apiobj.collision_position.z))
            continue;
        if (object->apiobj.collision_position.z >= block_max_z)
            continue;
        if ((block->flags_0ca & 0x20) != 0)
            continue;
        if ((block->flags_0ca & 0x90) == 0x90)
            continue;
        gap = dx - fabsf(min_x);
        goto have_gap;
    }
    mode1_geometry: {
        f32 block_max_z = max_z + pos->z - 0.01f;
        f32 block_min_z = pos->z + min_z - 0.01f;
        if (object->apiobj.collision_position.z >= block_max_z)
            continue;
        if (block_min_z >= object->apiobj.collision_position.z)
            continue;
        if ((block->flags_0ca & 0x20) != 0)
            continue;
        if ((block->flags_0ca & 0x50) == 0x50)
            continue;
        gap = fabsf(dx) - block->bounds_max.x;
        goto have_gap;
    }
    mode3_geometry: {
        f32 block_min_x = min_x + pos->x - 0.01f;
        f32 block_max_x = pos->x + block->bounds_max.x - 0.01f;
        if (block_min_x >= object->apiobj.collision_position.x)
            continue;
        if (object->apiobj.collision_position.x >= block_max_x)
            continue;
        if ((block->flags_0ca & 0x10) != 0)
            continue;
        if ((block->flags_0ca & 0x60) == 0x60)
            continue;
        gap = fabsf(dz) - max_z;
        goto have_gap;
    }
    mode2_geometry: {
        f32 block_max_x = block->bounds_max.x + pos->x - 0.01f;
        f32 block_min_x = pos->x + min_x - 0.01f;
        if (object->apiobj.collision_position.x >= block_max_x)
            continue;
        if (block_min_x >= object->apiobj.collision_position.x)
            continue;
        if ((block->flags_0ca & 0x10) != 0)
            continue;
        if ((block->flags_0ca & 0xa0) == 0xa0)
            continue;
        gap = dz - fabsf(min_z);
        goto have_gap;
    }
    have_gap: {
        if (!(gap <= (object->apiobj.field_0x1dc + 0.01f) * 2.0f))
            continue;
        i32 yaw = NuAtan2D(delta.x, delta.z);
        i32 rot = 0x4000 - (yaw & 0xffff);
        NuVecRotateY(&delta, &delta, rot);
        i32 pitch = NuAtan2D(delta.x, delta.y);
        i16 yaw_diff = (i16)(yaw - object->apiobj.movement_facing_angle);
        i32 yaw_abs = yaw_diff;
        yaw_abs = yaw_abs < 0 ? -yaw_abs : yaw_abs;
        if ((u16)yaw_abs > 0x2000)
            continue;
        i32 pitch_diff = 0x4000 - (u16)pitch;
        pitch_diff = pitch_diff < 0 ? -pitch_diff : pitch_diff;
        if ((u16)pitch_diff > 0x4000)
            continue;
        f32 dist2 = dx * dx + dz * dz;
        if (range <= dist2)
            continue;
        if (!(nearest_dist > dist2))
            continue;
        nearest_dist = dist2;
        nearest = block;
    }
    }
    return nearest;
}

i32 PushBlock(GameObject_s *object) {
    pushblock_s *block = NearestFacingPushBlock(WORLD, object, 2.0f);
    if (block != NULL) {
        block->pushing_object = object;
        block->runtime_flags_0c8 |= 1;
        AlertSurroundingCreatures(object, &object->apiobj.collision_position);
        return 1;
    }
    object->character_context = -1;
    return 0;
}

pushblock_s *BlockInBlock(WORLDINFO_s *world, pushblock_s *block, i32 excluded, pushblock_s **support) {
    NUVEC *position = block->position;
    if (support)
        *support = NULL;
    for (i32 i = 0; i < world->push_block_count; ++i) {
        if (i == excluded)
            continue;
        pushblock_s *other = &world->push_blocks[i];
        if (other->flags_0cb & 2)
            continue;
        if (!NuSpecialGetVisibilityFn(&other->special))
            continue;
        f32 x = position->x - other->position->x, y = position->y - other->position->y,
            z = position->z - other->position->z;
        bool overlapx = x > (other->bounds_min.x - block->bounds_max.x) + 0.01f &&
                        x < (other->bounds_max.x - block->bounds_min.x) - 0.01f;
        f32 ymax = other->bounds_max.y - block->bounds_min.y;
        bool overlapz = z > (other->bounds_min.z - block->bounds_max.z) + 0.01f &&
                        z < (other->bounds_max.z - block->bounds_min.z) - 0.01f;
        bool overlapy = y > other->bounds_min.y - block->bounds_max.y && y < ymax;
        if (support && overlapz && overlapx && y - ymax > 0.0f) {
            if (!*support)
                *support = other;
            else if ((position->y - (*support)->position->y) - ((*support)->bounds_max.y - block->bounds_min.y) >
                     y - ymax) {
                other->supporting_block = *support;
                *support = other;
            }
        }
        if (overlapx && overlapy && overlapz)
            return other;
    }
    return NULL;
}

void ResetSinglePushBlockHeight(WORLDINFO_s *world, pushblock_s *block, i32 index) {
    BlockInBlock(world, block, index, &block->block_below);

    const f32 epsilon = 0.01f;
    f32 support_height;
    if (block->block_below != NULL) {
        pushblock_s *support = block->block_below;
        support_height = support->bounds_max.y + support->position->y + epsilon;
        block->support_height = support_height;
    } else {
        support_height = block->support_height;
    }

    NUVEC *position = block->position;
    f32 position_y = position->y;
    f32 penetration = block->bounds_min.y + position_y - support_height;
    block->vertical_penetration = penetration;
    if (penetration > epsilon) {
        position->y = position_y - penetration;
        block->vertical_penetration = 0.0f;
    }
}

__attribute__((optimize("O2", "tree-vectorize"))) void MoveBlocksOverBlock(WORLDINFO_s *world, pushblock_s *block, i32 excluded, nuvec_s *velocity) {
    if (block->runtime_flags_0c9 & 4)
        return;
    f32 xmin = block->bounds_min.x + block->position->x, xmax = block->position->x + block->bounds_max.x;
    f32 zmin = block->bounds_min.z + block->position->z, zmax = block->position->z + block->bounds_max.z;
    for (i32 i = 0; i < world->push_block_count; ++i) {
        if (i == excluded)
            continue;
        pushblock_s *other = &world->push_blocks[i];
        if ((other->flags_0cb & 4) || !(other->flags_0ca & 4))
            continue;
        NUVEC *position = other->position;
        if (block->position->y >= position->y)
            continue;
        if (!(position->x >= (xmin - other->bounds_max.x) + 0.01f &&
              position->x <= (xmax - other->bounds_min.x) - 0.01f &&
              position->z >= (zmin - other->bounds_max.z) + 0.01f &&
              position->z <= (zmax - other->bounds_min.z) - 0.01f))
            continue;
        other->snap_origin = *position;
        position->x += velocity->x;
        position->y -= velocity->y;
        other->ground_offset -= velocity->y;
        position->z += velocity->z;
        other->runtime_flags_0c8 |= 4;
        if (BlockInBlock(world, other, i, &other->supporting_block))
            other->runtime_flags_0c9 |= 4;
        NuSpecialUpdate(&other->special);
    }
}

void MoveBlocks(WORLDINFO_s *world, pushblock_s *block, i32 index, nuvec_s *velocity) {
    NUVEC delta;
    delta.x = velocity->x;
    delta.y = 0.0f;
    delta.z = velocity->z;
    block->position->x += delta.x;
    block->position->z += delta.z;
    NuSpecialUpdate(&block->special);
    for (i32 i = 0; i < block->end_position_count; ++i) {
        NUMTX *matrix = NuSpecialGetInstanceMtx(&block->end_position_specials[i]);
        matrix->m30 = block->position->x;
        matrix->m32 = block->position->z;
        NuSpecialUpdate(&block->end_position_specials[i]);
    }
    if (BlockInBlock(world, block, index, &block->supporting_block))
        block->runtime_flags_0c9 |= 4;
    else {
        MoveBlocksOverBlock(world, block, index, &delta);
        if (block->pushing_object && (fabsf(delta.x) > 0.001f || fabsf(delta.z) > 0.001f) &&
            NuFmod(GameTimer.time_elapsed, 0.25f) < 0.1f)
            NewRumble(block->pushing_object->pad_gamepad->pad, ((f32)qrand() * 1.5259022e-5f) * 0.05f, 0);
    }
    if (block->runtime_flags_0c8 & 8) {
        delta.y = velocity->y;
        delta.x = delta.z = 0;
        block->position->y -= delta.y;
        block->ground_offset -= delta.y;
        pushblock_s *hit = BlockInBlock(world, block, index, &block->supporting_block);
        if (hit) {
            block->settled_height = hit->position->y + hit->bounds_max.y;
            block->runtime_flags_0c9 |= 8;
        } else
            MoveBlocksOverBlock(world, block, index, &delta);
        NuSpecialUpdate(&block->special);
    }
}

i32 TerrainBlockOnBlock(WORLDINFO_s *world, pushblock_s *block, nuvec_s *points, float *heights) {
    if (!heights || !points)
        return 0;
    heights[0] = heights[1] = heights[2] = heights[3] = -10000.0f;
    for (i32 i = 0; i < world->push_block_count; ++i) {
        pushblock_s *other = &world->push_blocks[i];
        if (other == block)
            continue;
        f32 xmin = other->position->x - fabsf(other->bounds_min.x),
            xmax = other->position->x + fabsf(other->bounds_max.x);
        f32 zmin = other->position->z - fabsf(other->bounds_min.z),
            zmax = other->position->z + fabsf(other->bounds_max.z);
        f32 height = fabsf(other->bounds_max.y) + other->position->y;
        for (i32 j = 0; j < 4; ++j)
            if (points[j].x > xmin && points[j].x < xmax && points[j].z > zmin && points[j].z < zmax)
                heights[j] = height;
    }
    return 1;
}

void GizmoPushBlockInitAndReset(WORLDINFO_s *world, void *progress) {
    world->push_block_position_count = 0;
    runoutofpostabspace = 0;
    pushposincrease = 0;
    world->push_block_positions = reinterpret_cast<NUVEC *>(world->giz_buffer.void_ptr);
    world->giz_buffer.addr =
        ALIGN(world->giz_buffer.addr + world->current_level->max_push_block_end_pos * sizeof(NUVEC), 4);
    pushblock_s *block = world->push_blocks;
    ResetPushProgress(world, progress);

    for (i32 index = 0; index < world->push_block_count; ++index, ++block) {
        block->runtime_flags_0c9 &= 0x8f;
        nuinstanim_s *animation = NuSpecialGetInstAnim(&block->special);
        if (animation != NULL) {
            i32 positions =
                static_cast<i32>(NuAnimEndFrameOld(block->special.scene->instance_animation_data[animation->anim_ix])) &
                7;
            block->output_count = positions;
            block->runtime_flags_0c9 = (block->runtime_flags_0c9 & 0x8f) | ((positions & 7) << 4);
            const i32 required = world->push_block_position_count + positions;
            if (!(required > world->current_level->max_push_block_end_pos)) {
                if (positions != 0 && runoutofpostabspace == 0) {
                    block->snap_positions = &world->push_block_positions[world->push_block_position_count];
                    f32 animation_frame = 1.0f;
                    for (i32 position = 0; position < ((block->runtime_flags_0c9 >> 4) & 7);
                         ++position, animation_frame += 1.0f) {
                        NUMTX evaluated;
                        EvalAnim(&block->special, animation_frame, &evaluated, 0);
                        block->snap_positions[position] = *reinterpret_cast<NUVEC *>(&evaluated.m30);
                        evaluated = *NuSpecialGetMtx(&block->special);
                        block->snap_positions[position].x += evaluated.m30;
                        block->snap_positions[position].y += evaluated.m31;
                        block->snap_positions[position].z += evaluated.m32;
                        ++world->push_block_position_count;
                    }
                }
            } else {
                pushposincrease +=
                    runoutofpostabspace == 0 ? required - world->current_level->max_push_block_end_pos : positions;
                runoutofpostabspace = 1;
            }
        }
        ResetSinglePushBlock(world, block, index);
    }

    block = world->push_blocks;
    auto world_push_block_count = world->push_block_count;
    for (i32 index = 0; index < world_push_block_count; ++index, ++block) {
        NUVEC centre = *block->position;
        block->velocity = v000;
        block->target_velocity = v000;
        block->snap_origin = centre;

        const f32 left = fabsf(block->bounds_min.x) - 0.006f;
        const f32 right = fabsf(block->bounds_max.x) - 0.006f;
        const f32 back = fabsf(block->bounds_min.z) - 0.006f;
        const f32 front = fabsf(block->bounds_max.z) - 0.006f;
        const f32 y = centre.y - fabsf(block->bounds_min.y) + 0.001f + 0.025f;
        NUVEC corners[4] = {centre, centre, centre, centre};
        corners[0].y = corners[1].y = corners[2].y = corners[3].y = y;
        corners[0].x -= left;
        corners[0].z -= back;
        corners[1].x += right;
        corners[1].z -= back;
        corners[2].x += right;
        corners[2].z += front;
        corners[3].x -= left;
        corners[3].z += front;
        PlatOnOff(block->platform_id, 0);
        NewTerrPlatformsOff();
        const f32 height0 = GameShadow(NULL, &corners[0], 0.1f, -1);
        block->terrain_info[0] = static_cast<i8>(ShadowInfo());
        block->extra_terrain_info[0] = static_cast<i8>(EShadowInfo());
        NewTerrPlatformsOff();
        const f32 height1 = GameShadow(NULL, &corners[1], 0.1f, -1);
        block->terrain_info[1] = static_cast<i8>(ShadowInfo());
        block->extra_terrain_info[1] = static_cast<i8>(EShadowInfo());
        NewTerrPlatformsOff();
        const f32 height2 = GameShadow(NULL, &corners[2], 0.1f, -1);
        block->terrain_info[2] = static_cast<i8>(ShadowInfo());
        block->extra_terrain_info[2] = static_cast<i8>(EShadowInfo());
        NewTerrPlatformsOff();
        const f32 height3 = GameShadow(NULL, &corners[3], 0.1f, -1);
        block->terrain_info[3] = static_cast<i8>(ShadowInfo());
        block->extra_terrain_info[3] = static_cast<i8>(EShadowInfo());
        PlatOnOff(block->platform_id, 1);

        if (height1 == height2 && height0 == height1 && height2 == height3) {
            NewTerrPlatformsOff();
            centre.y = GameShadow(NULL, &centre, 5.0f, -1);
            if (centre.y == 2000000.0f) {
                centre.y = 0.0f;
            }
            block->ground_height = centre.y;
        } else {
            block->ground_height = (height1 + height0 + height2 + height3) * 0.25f;
        }
        f32 support_heights[4];
        TerrainBlockOnBlock(world, block, corners, support_heights);
        block->previous_extra_terrain_info = *reinterpret_cast<i32 *>(block->extra_terrain_info);
    }

    for (i32 index = 0; index < world->push_block_count; ++index) {
        ResetSinglePushBlockHeight(world, &world->push_blocks[index], index);
    }
}

i32 OtherBlockInRange(WORLDINFO_s *world, pushblock_s *block, nuvec_s *position, i32 excluded) {
    NUVEC centre;
    f32 radius;
    NuSpecialGetRadius(&block->special, &centre, &radius);
    radius *= radius;
    for (i32 i = 0; i < world->push_block_count; ++i) {
        if (i == excluded)
            continue;
        pushblock_s *other = &world->push_blocks[i];
        if ((other->packed_state_flags & 0x01040000) != 0x01040000 || (other->flags_0cb & 4))
            continue;
        if (!NuSpecialGetVisibilityFn(&other->special))
            continue;
        NUVEC minimum, maximum;
        NuSpecialGetBounds(&other->special, &minimum, &maximum);
        NuFmax(fabsf(minimum.x), maximum.x);
        NuFmax(fabsf(minimum.z), maximum.z);
        f32 other_radius;
        NuSpecialGetRadius(&other->special, &centre, &other_radius);
        centre = *other->position;
        f32 x = centre.x - position->x, z = centre.z - position->z;
        f32 distance = (x * x + z * z) - other_radius * other_radius;
        if (radius >= distance || 0.0f >= distance)
            return 1;
    }
    return 0;
}

i32 SnapPosTaken(WORLDINFO_s *world, pushblock_s *, nuvec_s *position, i32 excluded_index) {
    for (i32 i = 0; i < world->push_block_count; ++i) {
        if (i == excluded_index) {
            continue;
        }
        pushblock_s *block = &world->push_blocks[i];
        if ((block->packed_state_flags & 0x02000300) == 0) {
            continue;
        }
        const f32 dx = block->position->x - position->x;
        const f32 dy = block->position->y - position->y;
        const f32 dz = block->position->z - position->z;
        if (dx * dx + dy * dy + dz * dz <= 0.05f * 0.05f) {
            return 1;
        }
    }
    return 0;
}

void PushSeekComplete(pushblock_s *block, i32 index) {
    block->completed_outputs |= 1 << index;
    block->runtime_flags_0c9 &= ~2;
    if (!(block->flags_0cb & 0x20)) {
        block->flags_0cb |= 2;
        block->runtime_flags_0c9 |= 1;
    }
    if (block->pushing_object)
        NewBuzz(block->pushing_object->pad_gamepad->pad, 0.1f, 0);
}

void UpdatePushBlocks(void *world_ptr, void *, float) {
    static f32 snap_time = 0.25f;
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (!world || !world->push_block_count || !world->push_blocks)
        return;
    snap_time = 0.25f;
    i32 target_seen = 0;
    for (i32 index = 0; index < world->push_block_count; ++index) {
        pushblock_s *p = &world->push_blocks[index];
        i32 visible = NuSpecialGetVisibilityFn(&p->special) & 1;
        p->flags_0ca = (p->flags_0ca & ~4) | (visible << 2);
        if (!visible)
            continue;
        GizObstacles_AddTrigger(p->position);
        if ((p->runtime_flags_0c8 & 0x81) == 0x81)
            p->runtime_flags_0c8 &= 0x3f;
        if ((i8)p->runtime_flags_0c8 < 0)
            p->runtime_flags_0c8 |= 0x40;
        if ((p->runtime_flags_0c9 & 1) || (p->flags_0cb & 2))
            continue;
        if (__builtin_expect(!!(p->snap_positions), 1)) {
            if (p->runtime_flags_0c9 & 2) {
                if (!(p->snap_timer > snap_time)) {
                    f32 fraction = NU_SIN_LUT((i32)((p->snap_timer / snap_time) * 16384.0f + 49152.0f + 16384.0f));
                    i32 ix = (p->packed_state_flags >> 15) & 7;
                    p->position->x = p->snap_origin.x + (p->snap_positions[ix].x - p->snap_origin.x) * fraction;
                    p->position->y = p->snap_origin.y + (p->snap_positions[ix].y - p->snap_origin.y) * fraction;
                    p->position->z = p->snap_origin.z + (p->snap_positions[ix].z - p->snap_origin.z) * fraction;
                    NuSpecialUpdate(&p->special);
                    for (i32 i = 0; i < p->end_position_count; ++i) {
                        NUMTX *m = NuSpecialGetInstanceMtx(&p->end_position_specials[i]);
                        NUVEC *target = &p->snap_positions[(p->packed_state_flags >> 15) & 7];
                        m->m32 = p->end_position_origins[i].z + (target->z - p->end_position_origins[i].z) * fraction;
                        m->m30 = p->end_position_origins[i].x + (target->x - p->end_position_origins[i].x) * fraction;
                        NuSpecialUpdate(&p->end_position_specials[i]);
                    }
                    p->snap_timer += FRAMETIME;
                    continue;
                }
                PushSeekComplete(p, (p->packed_state_flags >> 15) & 7);
            } else {
                for (i32 i = 1; i < ((p->runtime_flags_0c9 >> 4) & 7); ++i) {
                    NUVEC *target = &p->snap_positions[i];
                    NUVEC delta = {target->x - p->position->x, target->y - p->position->y, target->z - p->position->z};
                    f32 radius = p->snap_distance;
                    if (world->current_level == HOTHESCAPEB_LDATA &&
                        NuVecXZDistSqr(target, &hothbtestpos, NULL) < 0.0625f)
                        radius *= 2.5f;
                    else
                        radius += radius;
                    if (!(radius * radius > (delta.y * delta.y + delta.x * delta.x) + delta.z * delta.z))
                        continue;
                    if (SnapPosTaken(world, p, target, index))
                        continue;
                    if ((p->flags_0cb & 0x10) || OtherBlockInRange(world, p, &p->snap_positions[i], index)) {
                        target_seen = 1;
                        continue;
                    }
                    p->runtime_flags_0c9 |= 2;
                    if (p->flags_0cb & 0x20)
                        p->flags_0cb |= 0x10;
                    p->snap_origin.x = p->position->x;
                    p->snap_origin.z = p->position->z;
                    for (i32 j = 0; j < p->end_position_count; ++j)
                        p->end_position_origins[j] = *NuSpecialGetDrawPos(&p->end_position_specials[j]);
                    p->snap_index = i;
                    p->snap_timer = FRAMETIME;
                    target_seen = 1;
                    break;
                }
                if (!target_seen) {
                    if (p->flags_0cb & 0x20) {
                        p->flags_0cb &= ~0x10;
                        p->packed_state_flags &= 0xfffc7fff;
                    }
                    if (!(p->flags_0cb & 0x40))
                        p->completion_flags &= 0xf807;
                }
            }
        }
        if (!(p->runtime_flags_0c8 & 0x40) && ((p->runtime_flags_0c8 & 3) == 2 || p->snap_origin.x != p->position->x ||
                                               p->snap_origin.z != p->position->z)) {
            p->target_velocity.z = p->target_velocity.x = p->velocity.z = p->velocity.x = 0;
        }
        if (!(p->runtime_flags_0c8 & 0x49))
            continue;
        if (p->runtime_flags_0c8 & 0x41) {
            i32 surface = p->terrain_info[0];
            bool same = p->terrain_info[1] == surface && p->terrain_info[2] == surface && p->terrain_info[3] == surface;
            if (__builtin_expect(!!(same), 1)) {
                if ((u32)surface <= 31 && (TerSurface[surface].flags & 0x20000))
                    p->runtime_flags_0c8 |= 0x40;
                else
                    p->runtime_flags_0c8 &= ~0x40;
            }
            if (__builtin_expect(!!(p->pushing_object), 1)) {
                if (!(p->runtime_flags_0c8 & 0x40))
                    p->target_velocity.z = p->target_velocity.x = 0;
                f32 x = p->pushing_object->target_velocity.x, z = p->pushing_object->target_velocity.z;
                if (fabsf(x) > fabsf(z)) {
                    p->velocity.z = p->target_velocity.z = 0;
                    p->target_velocity.x = x * FRAMETIME;
                } else {
                    p->velocity.x = p->target_velocity.x = 0;
                    p->target_velocity.z = z * FRAMETIME;
                }
            } else if (same) {
                p->target_velocity.x *= TerSurface[surface].movement_scale;
                p->target_velocity.z *= TerSurface[surface].movement_scale;
            } else
                p->target_velocity.x = p->velocity.x = p->target_velocity.z = p->velocity.z = 0;
            p->velocity.x = SeekValF(p->velocity.x, p->target_velocity.x, 1.0f);
            p->velocity.z = SeekValF(p->velocity.z, p->target_velocity.z, 1.0f);
            if (p->velocity.x == 0) {
                if (fabsf(p->velocity.z) <= 0.0001f) {
                    p->runtime_flags_0c8 &= 0x3f;
                    p->velocity.z = p->target_velocity.z = 0;
                }
            } else {
                if (__builtin_expect(fabsf(p->velocity.x) <= 0.0001f, 0)) {
                    p->runtime_flags_0c8 &= 0x3f;
                    p->velocity.x = p->target_velocity.x = 0;
                }
            }
        }
        if (p->runtime_flags_0c8 & 8) {
            p->target_velocity.y = 10.0f * FRAMETIME;
            p->target_velocity.x = p->target_velocity.z = p->velocity.x = 0;
            p->velocity.y = SeekValF(p->velocity.y, p->target_velocity.y, 1.0f);
            p->velocity.z = 0;
        } else
            p->target_velocity.y = 0;
    }
    for (i32 index = 0; index < world->push_block_count; ++index) {
        pushblock_s *p = &world->push_blocks[index];
        if (__builtin_expect((p->flags_0ca & 4) && !(p->packed_state_flags & 0x04000100) && !(p->flags_0cb & 2) &&
            (p->runtime_flags_0c8 & 0x69), 0))
            MoveBlocks(world, p, index, &p->velocity);
    }
    for (i32 index = 0; index < world->push_block_count; ++index) {
        pushblock_s *p = &world->push_blocks[index];
        if (!(p->flags_0ca & 4) || (p->flags_0cb & 2))
            continue;
        if (!NuSpecialGetVisibilityFn(&p->special) || !(p->runtime_flags_0c8 & 0x4d))
            continue;
        f32 left = fabsf(p->bounds_min.x) - 0.006f, right = fabsf(p->bounds_max.x) - 0.006f;
        f32 back = fabsf(p->bounds_min.z) - 0.006f, front = fabsf(p->bounds_max.z) - 0.006f;
        NUVEC *position = p->position;
        f32 y = (position->y - fabsf(p->bounds_min.y) + 0.001f) + 0.025f;
        if (__builtin_expect(p->ground_height > y, 0))
            y = p->ground_height + 0.25f;
        NUVEC corners[4] = {
            {position->x - left, y, position->z - back},
            {position->x + right, y, position->z - back},
            {position->x + right, y, position->z + front},
            {position->x - left, y, position->z + front},
        };
        PlatOnOff(p->platform_id, 0);
        bool platforms_off = p->supporting_block || p->previous_supporting_block;
        f32 heights[4];
        for (i32 i = 0; i < 4; ++i) {
            if (platforms_off)
                NewTerrPlatformsOff();
            heights[i] = GameShadow(NULL, &corners[i], 0.1f, -1);
            p->terrain_info[i] = ShadowInfo();
            p->extra_terrain_info[i] = EShadowInfo();
        }
        PlatOnOff(p->platform_id, 1);
        BlockInBlock(world, p, index, &p->supporting_block);
        if (p->supporting_block || p->previous_supporting_block) {
            p->previous_supporting_block = p->supporting_block;
            f32 support[4];
            TerrainBlockOnBlock(world, p, corners, support);
            for (i32 i = 0; i < 4; ++i)
                if (support[i] != -10000.0f)
                    heights[i] = support[i];
        }
        f32 low = p->ground_height - 0.01f, high = p->ground_height + 0.01f;
        if ((low > heights[0] && low > heights[1] && low > heights[2] && low > heights[3]) ||
            (heights[0] > high && heights[1] > high && heights[2] > high && heights[3] > high)) {
            p->runtime_flags_0c8 |= 8;
            p->ground_height = ((heights[0] + heights[1]) + heights[2] + heights[3]) * 0.25f + 0.01f;
            p->ground_offset = (p->position->y + p->bounds_min.y) - p->ground_height;
        }
        if (p->supporting_block) {
            p->ground_height = p->position->y + p->supporting_block->bounds_max.y;
            p->ground_offset = (p->position->y + p->bounds_min.y) - p->ground_height;
        }
        if (p->ground_offset > 0.01f)
            p->runtime_flags_0c8 |= 8;
        else if (p->runtime_flags_0c8 & 8) {
            p->runtime_flags_0c9 |= 8;
            p->settled_height = p->ground_height;
            p->runtime_flags_0c8 &= ~8;
        }
        i32 safe = 0;
        for (i32 i = 0; i < 4; ++i)
            if (p->extra_terrain_info[i] == 8 || p->terrain_info[i] == 14)
                ++safe;
        bool reset = safe != 4;
        if (world->current_level == HOTHESCAPEB_LDATA && NuVecXZDistSqr(p->position, &hothbtestpos, NULL) < 1.0f) {
            if (!(corners[0].z < 13.2f && !(corners[1].z >= 13.2f) && !(corners[2].z >= 13.2f) &&
                  !(corners[3].z >= 13.2f)))
                reset = true;
        }
        if (reset)
            p->runtime_flags_0c9 |= 4;
    }
    for (i32 index = 0; index < world->push_block_count; ++index) {
        pushblock_s *p = &world->push_blocks[index];
        if (!(p->flags_0ca & 4) || (p->runtime_flags_0c9 & 1) || (p->flags_0cb & 2))
            continue;
        if (!(p->runtime_flags_0c9 & 4) && (p->runtime_flags_0c8 & 1))
            GameAudio_PlaySfx(72, p->position, GameAudio_GetPlrSfxBits(p->pushing_object), 0);
        if (p->runtime_flags_0c9 & 0xc) {
            if (p->runtime_flags_0c9 & 4) {
                p->position->x = p->snap_origin.x;
                p->position->z = p->snap_origin.z;
                p->target_velocity.x = p->target_velocity.z = p->velocity.x = p->velocity.z = 0;
                memcpy(p->extra_terrain_info, &p->previous_extra_terrain_info, 4);
                p->runtime_flags_0c8 &= 0x3f;
                NuSpecialUpdate(&p->special);
                for (i32 i = 0; i < p->end_position_count; ++i)
                    if (NuSpecialExistsFn(&p->end_position_specials[i])) {
                        NUMTX *m = NuSpecialGetInstanceMtx(&p->end_position_specials[i]);
                        if ((usize)m != 0xffffffd0u) {
                            m->m30 = p->snap_origin.x;
                            m->m32 = p->snap_origin.z;
                            NuSpecialUpdate(&p->end_position_specials[i]);
                        }
                    }
                p->runtime_flags_0c8 &= ~1;
                p->runtime_flags_0c9 &= ~4;
            }
            if (p->runtime_flags_0c9 & 8) {
                p->ground_height = p->settled_height;
                p->position->y = p->settled_height - p->bounds_min.y;
                p->target_velocity.y = p->velocity.y = 0;
                p->runtime_flags_0c8 &= ~8;
                p->runtime_flags_0c9 &= ~8;
                p->ground_offset = (p->position->y + p->bounds_min.y) - p->settled_height;
                NuSpecialUpdate(&p->special);
            }
        } else if (p->runtime_flags_0c8 & 1) {
            if ((p->runtime_flags_0c8 & 3) == 1 && p->pushing_object)
                GameAudio_PlaySfx(71, &p->pushing_object->apiobj.collision_position,
                                  GameAudio_GetPlrSfxBits(p->pushing_object), 0);
            if (LEGOHINT_PUSHBLOCKS != -1 && p->pushing_object &&
                (p->pushing_object->apiobj.object_flags & 0x81) == 0x81)
                Hint_SetComplete(LEGOHINT_PUSHBLOCKS);
        }
    }
    for (i32 index = 0; index < world->push_block_count; ++index) {
        pushblock_s *p = &world->push_blocks[index];
        if (!(p->flags_0ca & 4))
            continue;
        if (p->pushing_object) {
            if (p->platform_id != -1 && (p->runtime_flags_0c8 & 9)) {
                p->pushing_object->ignore_shove_system = 1;
                AddShoveObject(&p->special, (i16)p->platform_id);
            } else
                p->pushing_object->ignore_shove_system = 0;
        }
        if (!(p->packed_state_flags & 0x220)) {
            i32 safe = 0;
            for (i32 i = 0; i < 4; ++i)
                if (__builtin_expect(p->extra_terrain_info[i] == 8 || p->terrain_info[i] == 14, 0))
                    ++safe;
            if (safe == 4) {
                memcpy(&p->previous_extra_terrain_info, p->extra_terrain_info, 4);
                p->snap_origin = *p->position;
            }
        }
        p->pushing_object = NULL;
        p->was_pushed = p->pushed;
        p->runtime_flags_0c8 = (p->runtime_flags_0c8 & 0xea) | ((p->runtime_flags_0c8 << 1) & 0x10);
    }
}

pushblock_s *NearestPushBlock(WORLDINFO_s *world, nuvec_s *position, float range) {
    if (position == NULL || world == NULL)
        return NULL;
    const NUVEC minimum = {position->x - range, position->y - range, position->z - range};
    const NUVEC maximum = {position->x + range, position->y + range, position->z + range};
    pushblock_s *nearest = NULL;
    f32 nearest_distance = 1000000000.0f;
    pushblock_s *block = world->push_blocks;
    for (i32 i = 0; i < world->push_block_count; ++i, ++block) {
        if ((block->packed_state_flags & 0x1040100) != 0x1040000)
            continue;
        NUVEC *candidate = block->position;
        if (candidate->x < minimum.x || candidate->x > maximum.x || candidate->z < minimum.z ||
            candidate->z > maximum.z || candidate->y < minimum.y || candidate->y > maximum.y)
            continue;
        const f32 distance = NuVecDistSqr(position, candidate, NULL);
        if (distance < nearest_distance) {
            nearest = block;
            nearest_distance = distance;
        }
    }
    return nearest;
}

i32 Push_UpdateHints(HINT_s *) {
    if (LEGOACT_PUSH == -1)
        return 0;
    for (i32 i = 0; i < 8; ++i) {
        GameObject_s *object = Player[i];
        if (__builtin_expect(object != NULL && object->apiobj.player_controlled &&
                                 object->apiobj.character_model->model_data_b[LEGOACT_PUSH] != NULL &&
                                 NearestPushBlock(WORLD, &object->apiobj.collision_position, 2.0f) != NULL,
                             1))
            return 1;
    }
    return 0;
}

i32 GizPushBlock_EndFrameCompleted(pushblock_s *push_block, i32 output_index) {
    if (push_block == NULL) {
        return -1;
    }

    if (output_index != 0) {
        const u8 completed_outputs = push_block->completion_flags / PUSH_BLOCK_FIRST_OUTPUT_FLAG;
        return (completed_outputs >> output_index) & 1;
    }
    return (push_block->completion_flags & PUSH_BLOCK_ANY_OUTPUT_MASK) != 0;
}

i32 GizPush_GetOutput(GIZMO *gizmo, i32 output_index, i32) {
    pushblock_s *push_block = static_cast<pushblock_s *>(gizmo->object);
    if (push_block == NULL) {
        return 0;
    }
    return GizPushBlock_EndFrameCompleted(push_block, output_index) != 0;
}

ADDGIZMOTYPE *Push_RegisterGizmo(i32 type_id) {
    static ADDGIZMOTYPE addtype;

    addtype = Default_ADDGIZMOTYPE;
    addtype.name = "PushBlocks";
    addtype.prefix = "";
    addtype.fns.unknown1 = 0x24c;
    addtype.fns.get_max_gizmos_fn = Push_GetMaxGizmos;
    addtype.fns.add_gizmos_fn = Push_AddGizmos;
    addtype.fns.early_update_fn = NULL;
    addtype.fns.late_update_fn = UpdatePushBlocks;
    addtype.fns.draw_fn = NULL;
    addtype.fns.panel_draw_fn = NULL;
    addtype.fns.get_gizmo_name_fn = GizPush_GetGizmoName;
    addtype.fns.get_output_fn = GizPush_GetOutput;
    addtype.fns.get_output_name_fn = GizPush_GetOutputName;
    addtype.fns.get_num_outputs_fn = GizPush_GetNumOutputs;
    addtype.fns.activate_fn = Push_Activate;
    addtype.fns.set_visibility_fn = Push_SetVisibility;
    addtype.fns.get_visibility_fn = NULL;
    addtype.fns.get_pos_fn = NULL;
    addtype.fns.using_special_fn = NULL;
    addtype.fns.bolt_hit_plat_fn = Pushblocks_BoltHitPlat;
    addtype.fns.get_best_bolt_target_fn = NULL;
    addtype.fns.bolt_hit_fn = NULL;
    addtype.fns.allocate_progress_data_fn = Push_AllocateProgressData;
    addtype.fns.clear_progress_fn = Push_ClearProgress;
    addtype.fns.store_progress_fn = Push_StoreProgress;
    addtype.fns.reset_fn = Push_Reset;
    addtype.fns.reserve_buffer_space_fn = PushBlocks_ReserveBufferSpace;
    addtype.fns.load_fn = edpush_Load;
    addtype.fns.post_load_fn = NULL;
    addtype.fns.add_level_sfx_fn = NULL;
    pushblock_gizmotype_id = type_id;

    return &addtype;
}

