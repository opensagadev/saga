// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "globals.h"
#include "legoapi/render/fx/game_deb.h"

#include "nu2api/numath/nurand.h"

#include <stdlib.h>
#include <ctype.h>
#include "legoapi/characters/motion.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/world/world.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nucore/nustring.h"
#include <string.h>
#include <stdio.h>
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/characters/motion/action_info.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/world/level.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/items/base/apiobject.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numusic/sfx.h"
#include <float.h>
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/ai/aisys/aipath.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "nu2api/nucore/numechptr.hpp"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nufile/nufile.h"
#include <math.h>
#include "legoapi/items/objects/objectsall.h"
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
#include "legoapi/items/base/animpacket.h"
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
#include "legoapi/actions/character/transform.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include <stddef.h>
#include "legoapi/world/areas.h"
#include "nu2api/numusic/numusic.h"
#include <stdarg.h>
#include <limits.h>
#include <new>
#include "nu2api/nu3d/nugscn.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/menus/screens/movies.h"
#include "nu2api/nucore/nuvideo.h"
#include "legoapi/misc/androidbatman.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include <cstdio>
#include <GLES2/gl2.h>
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nusound/nusound.h"
#include "nu2api/nu3d/nu3d_includes.h"

i32 i_temp_xrot;

char *IToX(char *output, i32 value) {
    char hex[] = "0123456789abcdef";
    i8 byte = static_cast<i8>(value >> 24);
    output[0] = hex[(byte >> 4) & 15];
    output[1] = hex[byte & 15];
    byte = static_cast<i8>(value >> 16);
    output[2] = hex[(byte >> 4) & 15];
    output[3] = hex[byte & 15];
    byte = static_cast<i8>(value >> 8);
    output[4] = hex[(byte >> 4) & 15];
    output[5] = hex[byte & 15];
    byte = static_cast<i8>(value);
    output[6] = hex[(byte >> 4) & 15];
    output[7] = hex[byte & 15];
    return output + 8;
}

i32 XToI(char *input) {
    char digit = input[0];
    i32 decimal = digit - '0';
    i32 letter = digit - 'W';
    i32 result = digit >= ':' ? letter : decimal;
    digit = input[1];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[2];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[3];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[4];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[5];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[6];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[7];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    return result;
}

i32 IsTok(char const *text, char const *token) {
    return text[0] == token[0] && text[1] == token[1] && text[2] == token[2] && text[3] == token[3];
}

char *I64ToX(char *output, i64 value) {
    i32 high;
    __builtin_memcpy(&high, reinterpret_cast<const char *>(&value) + 4, sizeof(high));
    char hex[] = "0123456789abcdef";
    i8 byte = static_cast<i8>(high >> 24);
    output[0] = hex[(byte >> 4) & 15];
    output[1] = hex[byte & 15];
    byte = static_cast<i8>(high >> 16);
    output[2] = hex[(byte >> 4) & 15];
    output[3] = hex[byte & 15];
    byte = static_cast<i8>(high >> 8);
    output[4] = hex[(byte >> 4) & 15];
    output[5] = hex[byte & 15];
    byte = static_cast<i8>(high);
    output[6] = hex[(byte >> 4) & 15];
    output[7] = hex[byte & 15];
    i32 low;
    __builtin_memcpy(&low, &value, sizeof(low));
    byte = static_cast<i8>(low >> 24);
    output[8] = hex[(byte >> 4) & 15];
    output[9] = hex[byte & 15];
    byte = static_cast<i8>(low >> 16);
    output[10] = hex[(byte >> 4) & 15];
    output[11] = hex[byte & 15];
    byte = static_cast<i8>(low >> 8);
    output[12] = hex[(byte >> 4) & 15];
    output[13] = hex[byte & 15];
    byte = static_cast<i8>(low);
    output[14] = hex[(byte >> 4) & 15];
    output[15] = hex[byte & 15];
    return output + 16;
}

i64 XToI64(char *input) {
    char digit = input[0];
    i32 decimal = digit - '0';
    i32 letter = digit - 'W';
    i64 result = digit >= ':' ? letter : decimal;
    digit = input[1];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[2];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[3];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[4];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[5];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[6];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[7];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[8];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[9];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[10];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[11];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[12];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[13];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[14];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[15];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    return result;
}

typedef f32 RawClipVec4 __attribute__((vector_size(16), aligned(4)));

#if defined(__i386__)
#define SPLINE_POINT_ANGLES_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define SPLINE_POINT_ANGLES_ATTR __attribute__((noinline, noclone))
#endif
static SPLINE_POINT_ANGLES_ATTR void SplinePointAnglesBody(const i16 *length, NUVEC *const *pts, i32 index,
                                                           i32 looping, u16 *pitch, u16 *angle)
    __asm__("_ZL17SplinePointAnglesP11nugspline_siiPtS1_.isra.1.part.2");

static inline void SplinePointAngles(NUGSPLINE *spline, i32 index, i32 looping, u16 *pitch, u16 *angle) {
    SplinePointAnglesBody(&spline->length, &spline->pts, index, looping, pitch, angle);
}

i32 qseed = 0x3039;

extern i32 LevFlag[4];

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

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

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

extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);

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

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" u32 LAYER_HOVERIGNORE;

struct GameObject_s;
struct WORLDINFO_s;

struct numtx_s;

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

union variptr_u;

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
    extern i16 id_MINIATST, id_MINIATTE;
}

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

#if defined(__i386__) && defined(__SSE__)
#include <xmmintrin.h>
#endif

inline SplineObject::SplineObject()
    : next(NULL), previous(NULL), knots{}, points{}, led_file(Placeable::CurrentLedFile) {
}

inline SplineObject::~SplineObject() {
}

inline void SplineKnot::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(SplineKnot));
}

inline void SplineObject::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(SplineObject));
}

inline void SplinePointBlock::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(SplinePointBlock));
}

static char **TexAnimList;

extern WORLDINFO WorldInfo[2];

// Camera zoom state
f32 CameraZoom = 1.0f;

i32 dco_locatorposonly;
i32 dco_id = -1;
i32 dco_reflectaxis;
i32 dco_wearinghat;
u16 dco_prerotatez;
f32 dco_reflectcoord = 2000000.0f;
GAMECHARACTERDATA_s *dco_gcdata;
CHARACTERMODEL_s *dco_cmodel;
void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);

NUMTL *SolidMtl;
NUMTL *SolidMtl3D;

extern "C" {
extern u32 LAYER_HOVERIGNORE;
}

static void SplinePointAnglesBody(const i16 *length, NUVEC *const *pts, i32 index, i32 looping, u16 *pitch,
                                  u16 *angle) {
    NUVEC *current = &(*pts)[index];
    i32 previous_index = index - 1;
    NUVEC *previous = NULL;
    if (previous_index < 0 && looping)
        previous = &(*pts)[*length - 1];
    else if (previous_index >= 0)
        previous = &(*pts)[previous_index];
    NUVEC direction = {0.0f, 0.0f, 0.0f};
    if (previous != NULL) {
        direction.x += current->x - previous->x;
        direction.y += current->y - previous->y;
        direction.z += current->z - previous->z;
    }
    NUVEC *next = NULL;
    if (index + 1 < *length)
        next = &(*pts)[index + 1];
    else if (looping)
        next = *pts;
    if (next != NULL) {
        direction.x += next->x - current->x;
        direction.y += next->y - current->y;
        direction.z += next->z - current->z;
    }
    if (pitch != NULL)
        *pitch = NuAtan2D(direction.y, NuFsqrt(direction.x * direction.x + direction.z * direction.z));
    if (angle != NULL)
        *angle = NuAtan2D(direction.x, direction.z);
}

i32 qrand(void) {
    qseed = qseed * 0x24cd + 1 & 0xffff;

    return qseed;
}

i32 getqseed() {
    return qseed;
}

void ResetSeeds() {
    srand48(0);
    qseed = 0x3039;
    GAMERAND = 0x1f3ad27f;
    NuRandSeed(0);
    DebrisSetSeed(0);
    NuRandSetSeed(NULL, 0);
    NuPartSetSeed(0);
}

float SeekLinearF(float current, float target, float step) {
    if (current > target) {
        current -= step;
        if (current < target) {
            current = target;
        }
    } else if (current < target) {
        current += step;
        if (current > target) {
            current = target;
        }
    }
    return current;
}

f32 SeekValF(f32 current, f32 target, f32 rate) {
    const f32 blend = MIN(1.0f, rate * FRAMETIME);
    return current + (target - current) * blend;
}

void SeekVec(NUVEC *result, NUVEC *current, NUVEC *target, f32 rate) {
    const f32 blend = MIN(1.0f, rate * FRAMETIME);
    result->x = current->x + (target->x - current->x) * blend;
    result->y = current->y + (target->y - current->y) * blend;
    result->z = current->z + (target->z - current->z) * blend;
}

void CapVec(nuvec_s *input, float maximum, nuvec_s *output) {
    f32 length_squared = input->x * input->x + input->y * input->y + input->z * input->z;
    if (length_squared > maximum * maximum) {
        f32 factor = maximum / NuFsqrt(length_squared);
        output->x *= factor;
        output->y *= factor;
        output->z *= factor;
    }
}

i32 SphereSphereOverlap(NUVEC *a, f32 radius_a, NUVEC *b, f32 radius_b) {
    const f32 x = b->x - a->x;
    const f32 y = b->y - a->y;
    const f32 z = b->z - a->z;
    const f32 radius = radius_b + radius_a;
    return x * x + y * y + z * z <= radius * radius;
}

i32 SphereSphereOverlapScaleY(nuvec_s *position_a, float radius_a, float y_radius_a, nuvec_s *position_b,
                              float radius_b, float y_radius_b) {
    f32 dx = position_b->x - position_a->x;
    f32 dz = position_b->z - position_a->z;
    f32 dy = position_b->y - position_a->y;
    if (radius_b != y_radius_b || radius_a != y_radius_a) {
        dy *= (radius_b + radius_a) / (y_radius_b + y_radius_a);
    }
    f32 radius = radius_b + radius_a;
    return dx * dx + dy * dy + dz * dz <= radius * radius;
}

i32 OnOrInsidePlane(nuvec_s *point, nuvec_s *plane_point, nuvec_s *plane_normal, nuvec_s *corrected_point,
                    float normal_offset, float *distance_out) {
    NUVEC test_point;
    if (normal_offset == 0.0f) {
        test_point = *point;
    } else {
        NuVecScale(&test_point, plane_normal, normal_offset);
        NuVecAdd(&test_point, &test_point, point);
    }

    const f32 distance = (test_point.x - plane_point->x) * plane_normal->x +
                         (test_point.y - plane_point->y) * plane_normal->y +
                         (test_point.z - plane_point->z) * plane_normal->z;
    if (distance_out != NULL) {
        *distance_out = distance;
    }
    if (!(distance <= 0.0f)) {
        return 0;
    }

    if (corrected_point != NULL) {
        NuVecScale(corrected_point, plane_normal, -(distance - 0.5f));
        NuVecAdd(corrected_point, corrected_point, point);
    }
    return 1;
}

i32 OnOrOutsidePlane(nuvec_s *point, nuvec_s *plane_point, nuvec_s *normal) {
    f32 distance = (point->x - plane_point->x) * normal->x + (point->y - plane_point->y) * normal->y +
                   (point->z - plane_point->z) * normal->z;
    return distance >= 0.0f;
}

void FindAnglesZX(nuvec_s *normal, u16 *x_rotation, u16 *z_rotation) {
    const i32 x_angle = NuAtan2D(normal->z, normal->y);
    if (x_rotation != NULL) {
        *x_rotation = static_cast<u16>(x_angle);
    }
    temp_xrot = static_cast<i16>(x_angle);

    NUVEC rotated;
    const u16 truncated_x_angle = static_cast<u16>(x_angle);
    NuVecRotateX(&rotated, normal, static_cast<NUANG>(-static_cast<i32>(truncated_x_angle)));
    const i32 z_angle = -NuAtan2D(rotated.x, rotated.y);
    if (z_rotation != NULL) {
        *z_rotation = static_cast<u16>(z_angle);
    }
    temp_zrot = static_cast<i16>(z_angle);
}

void FindAnglesXY(nuvec_s *direction, u16 *x_rotation, u16 *y_rotation) {
    temp_yrot = NuAtan2D(direction->x, direction->z);
    if (y_rotation != NULL)
        *y_rotation = temp_yrot;
    i_temp_xrot = -NuAtan2D(direction->y, NuFsqrt(direction->x * direction->x + direction->z * direction->z));
    temp_xrot = i_temp_xrot;
    if (x_rotation != NULL)
        *x_rotation = temp_xrot;
}

void GetRotationAngles(nuvec_s *direction, u16 *x_rotation, u16 *y_rotation) {
    NUVEC rotated;
    NUVEC copy = *direction;
    u16 y_angle = -NuAtan2D(copy.z, copy.x);
    NuVecRotateY(&rotated, &copy, -y_angle);
    *x_rotation = -NuAtan2D(rotated.x, rotated.y);
    *y_rotation = y_angle;
}

i32 RotDiff(u16 current, u16 target) {
    i32 difference = static_cast<u32>(target) - static_cast<u32>(current);
    if (difference > 0x8000) {
        difference -= 0x10000;
    } else if (difference < -0x8000) {
        difference += 0x10000;
    }
    return difference;
}

u16 SeekRot(u16 current, u16 target, f32 rate) {
    const f32 blend = MIN(1.0f, rate * FRAMETIME);
    const i32 current_angle = current;
    i32 difference = static_cast<i32>(target) - current_angle;
    if (difference > 0x8000) {
        difference -= 0x10000;
    } else if (difference < -0x8000) {
        difference += 0x10000;
    }
    // Original 0x490a3d converts to signed i32 before retaining the low
    // 16 bits.  Interpolation across zero can produce a negative angle.
    const i32 angle = static_cast<i32>(static_cast<f32>(current_angle) + static_cast<f32>(difference) * blend);
    return static_cast<u16>(angle);
}

u16 TurnRot(u16 current, u16 target, i32 speed, i32 *difference_out) {
    if (target == current) {
        return target;
    }

    const i32 step = static_cast<i32>(static_cast<f32>(speed) * FRAMETIME);
    const i32 difference = RotDiff(current, target);
    if (difference_out != NULL) {
        *difference_out = difference;
    }
    if (difference > 0) {
        if (step < difference) {
            return static_cast<u16>(current + step);
        }
    } else if (difference < -step) {
        return static_cast<u16>(current - step);
    }
    return target;
}

f32 PushingTowardsAngle(u16 input_angle, u16 direction) {
    NUVEC input, target;
    NuVecRotateY(&input, &v001, input_angle);
    NuVecRotateY(&target, &v001, direction);
    return input.x * target.x + input.z * target.z;
}

void AnglesBetweenPoints(nuvec_s *from, nuvec_s *to, u16 *vertical, u16 *horizontal) {
    f32 x = to->x - from->x;
    f32 z = to->z - from->z;
    f32 y = to->y - from->y;
    *vertical = NuAtan2D(y, NuFsqrt(x * x + z * z));
    *horizontal = NuAtan2D(x, z);
}

void CalculateInterceptVector(NUVEC *origin, NUVEC *target, NUVEC *velocity, f32 speed, NUVEC *direction,
                              NUVEC *intercept) {
    const bool zero_speed = speed == 0.0f;
    NUVEC prediction = *target;
    {
        NuVecSub(direction, &prediction, origin);
        f32 distance = NuVecMag(direction);
        f32 time = (distance == 0.0f || zero_speed) ? 0.0f : distance / speed;
        NuVecAddScale(&prediction, target, velocity, time);
    }
    {
        NuVecSub(direction, &prediction, origin);
        f32 distance = NuVecMag(direction);
        f32 time = distance;
        if (distance == 0.0f || zero_speed)
            time = 0.0f;
        else
            time /= speed;
        NuVecAddScale(&prediction, target, velocity, time);
    }
    {
        NuVecSub(direction, &prediction, origin);
        f32 distance = NuVecMag(direction);
        f32 time = distance;
        if (distance == 0.0f || zero_speed)
            time = 0.0f;
        else
            time /= speed;
        NuVecAddScale(&prediction, target, velocity, time);
    }
    NuVecSub(direction, &prediction, origin);
    if (intercept != NULL)
        *intercept = prediction;
}

void RotateGameMatrix(numtx_s *matrix, i32 order, u16 x, u16 y, u16 z) {
    switch (order) {
        case 0:
            if (x != 0)
                NuMtxRotateX(matrix, x);
            if (y != 0)
                NuMtxRotateY(matrix, y);
            if (z != 0)
                NuMtxRotateZ(matrix, z);
            break;
        case 1:
            if (y != 0)
                NuMtxRotateY(matrix, y);
            if (x != 0)
                NuMtxRotateX(matrix, x);
            if (z != 0)
                NuMtxRotateZ(matrix, z);
            break;
        case 2:
            if (y != 0)
                NuMtxRotateY(matrix, y);
            if (z != 0)
                NuMtxRotateZ(matrix, z);
            if (x != 0)
                NuMtxRotateX(matrix, x);
            break;
        case 3:
            if (z != 0)
                NuMtxRotateZ(matrix, z);
            if (x != 0)
                NuMtxRotateX(matrix, x);
            if (y != 0)
                NuMtxRotateY(matrix, y);
            break;
    }
}

i32 MatrixReflection(numtx_s *matrix, i32 axis, f32 plane, f32 override_plane, numtx_s *result) {
    if (static_cast<u8>(Reflections_On) == 0)
        return 0;

    switch (axis) {
        case 1:
            *result = *matrix;
            result->m00 = -result->m00;
            result->m10 = -result->m10;
            result->m20 = -result->m20;
            result->m30 = plane - (result->m30 - plane);
            return 1;

        case 2:
            if (override_plane != 2000000.0f &&
                (MatrixReflection_CanOverrideFn == NULL || MatrixReflection_CanOverrideFn(plane) != 0)) {
                plane = override_plane;
            }
            *result = *matrix;
            result->m01 = -result->m01;
            result->m11 = -result->m11;
            result->m21 = -result->m21;
            result->m31 = plane - (result->m31 - plane);
            return 1;

        case 3:
            *result = *matrix;
            result->m02 = -result->m02;
            result->m12 = -result->m12;
            result->m22 = -result->m22;
            result->m32 = plane - (result->m32 - plane);
            return 1;

        default:
            return 0;
    }
}

i32 MatrixReflectionVU0_AXISY(numtx_s *matrix, f32 plane, f32 override_plane, numtx_s *result) {
    if (static_cast<u8>(Reflections_On) == 0)
        return 0;
    if (override_plane != 2000000.0f &&
        (MatrixReflection_CanOverrideFn == NULL || MatrixReflection_CanOverrideFn(plane) != 0)) {
        plane = override_plane;
    }
    *result = *matrix;
    result->m01 = -result->m01;
    result->m11 = -result->m11;
    result->m21 = -result->m21;
    result->m31 = plane - (result->m31 - plane);
    return 1;
}

void InterpolateRotationMatrix(numtx_s *result, numtx_s *first, numtx_s *second, float fraction) {
    SeekVec(NUMTX_GET_ROW_VEC(result, 0), NUMTX_GET_ROW_VEC(first, 0), NUMTX_GET_ROW_VEC(second, 0), fraction);
    SeekVec(NUMTX_GET_ROW_VEC(result, 1), NUMTX_GET_ROW_VEC(first, 1), NUMTX_GET_ROW_VEC(second, 1), fraction);
    SeekVec(NUMTX_GET_ROW_VEC(result, 2), NUMTX_GET_ROW_VEC(first, 2), NUMTX_GET_ROW_VEC(second, 2), fraction);
    NuVecNorm(NUMTX_GET_ROW_VEC(result, 0), NUMTX_GET_ROW_VEC(result, 0));
    NuVecNorm(NUMTX_GET_ROW_VEC(result, 1), NUMTX_GET_ROW_VEC(result, 1));
    NuVecNorm(NUMTX_GET_ROW_VEC(result, 2), NUMTX_GET_ROW_VEC(result, 2));
}

void QuatInterpolateRotationMatrix(NUMTX *result, NUMTX *first, NUMTX *second, f32 fraction) {
    NUQUAT a, b, interpolated;
    NuMtxToQuat(second, &b);
    NuMtxToQuat(first, &a);
    NuQuatSlerp(&interpolated, &a, &b, fraction);
    NuQuatToMtx(&interpolated, result);
}

i32 GameRayCast(NUVEC *position, NUVEC *displacement, f32 radius, i32 mask) {
    i32 hit = NewRayCastScaleYMask(position, displacement, radius, 1.0f, 0, mask);
    ++RAYCASTCALLS;
    return hit;
}

f32 GameShadow(GameObject_s *object, nuvec_s *position, f32 probe_height, i32 terrain_mask) {
    i32 disabled_platforms[16];
    i32 disabled_platform_count = 0;

    if (object != NULL) {
        WORLDINFO_s *world = WorldInfo_CurrentlyActive();
        if (object->field_0x107c != -1) {
            disabled_platforms[disabled_platform_count++] = object->field_0x107c;
        }

        if (world != NULL && world->char_platform_sys != NULL && VehicleArea == 0 &&
            !object->apiobj.player_controlled) {
            CHARPLATFORMSYS_s *system = world->char_platform_sys;
            for (i32 i = 0; i < system->platform_count; ++i) {
                if (system->platforms[i].object != NULL) {
                    disabled_platforms[disabled_platform_count++] = system->platforms[i].object->field_0x107c;
                }
            }
        }

        for (i32 i = 0; i < disabled_platform_count; ++i) {
            PlatOnOff(disabled_platforms[i], 0);
        }

        if (LAYER_HOVERIGNORE != 0xffffffff &&
            static_cast<GAMECHARACTERDATA_s *>(object->apiobj.character_data->field11_0x24)->field_0x28 == 0.0f) {
            terrain_mask &= ~LAYER_HOVERIGNORE;
        }
    }

    if (TimingBarSet == 2) {
        TBOPENFN("Ter", 2);
    }
    const f32 shadow_height = NewShadowEx(position, 0, probe_height, probe_height, terrain_mask);
    ++SHADOWCALLS;
    if (TimingBarSet == 2) {
        TBCLOSEFN("Ter", 2);
    }

    if (object != NULL && object->field_0x107c != -1) {
        PlatOnOff(object->field_0x107c, 1);
    }
    for (i32 i = 0; i < disabled_platform_count; ++i) {
        PlatOnOff(disabled_platforms[i], 1);
    }
    return shadow_height;
}

f32 FindReflectionNoPlatforms(nuvec_s *position) {
    NewTerrPlatformsOff();
    f32 height = GameShadow(NULL, position, 5.0f, -1);
    if (height != 2000000.0f) {
        u32 surface = static_cast<u32>(ShadowInfo());
        if (surface < 32 && (TerSurface[surface].flags & 2) != 0) {
            return height;
        }
    }
    return 2000000.0f;
}

f32 BlobShadowFade(NUVEC *position, f32 fade_start, f32 fade_end, f32 alpha) {
    if (alpha <= 0.0f) {
        return 0.0f;
    }

    NUVEC camera_delta;
    NuVecSub(&camera_delta, position, NUMTX_GET_ROW_VEC(&pNuCam->mtx, 3));
    const f32 distance_squared =
        camera_delta.x * camera_delta.x + camera_delta.y * camera_delta.y + camera_delta.z * camera_delta.z;

    if (distance_squared < fade_end * fade_end) {
        if (__builtin_expect(distance_squared > fade_start * fade_start, 0)) {
            const f32 distance = NuFsqrt(distance_squared);
            return (1.0f - (distance - fade_start) / (fade_end - fade_start)) * alpha;
        }
        return alpha;
    }
    return 0.0f;
}

void PointAlongSpline(NUGSPLINE *spline, f32 along, NUVEC *position, u16 *angle, u16 *pitch, i32 looping) {
    if (angle != NULL)
        *angle = 0;
    if (pitch != NULL)
        *pitch = 0;
    if (__builtin_expect(spline == NULL, 0))
        return;
    if (along > 1.0f)
        along = 1.0f;
    else if (along < 0.0f)
        along = 0.0f;
    u32 extent = (u32)(looping ? spline->length : spline->length - 1) << 16;
    u32 fixed = (i32)((f32)extent * along);
    i32 index = fixed >> 16;
    NUVEC *current = &spline->pts[index];
    *position = *current;
    if (__builtin_expect(angle != NULL, 1) || pitch != NULL)
        SplinePointAngles(spline, index, looping, pitch, angle);
    index++;
    if (index >= spline->length) {
        if (!looping)
            return;
        index = 0;
    }
    fixed &= 0xffff;
    if (fixed == 0)
        return;
    NUVEC *next = &spline->pts[index];
    f32 fraction = fixed * (1.0f / 65536.0f);
    position->x += (next->x - current->x) * fraction;
    position->y += (next->y - current->y) * fraction;
    position->z += (next->z - current->z) * fraction;
    if (angle != NULL || pitch != NULL) {
        u16 next_angle, next_pitch;
        SplinePointAngles(spline, index, looping, pitch != NULL ? &next_pitch : NULL,
                          angle != NULL ? &next_angle : NULL);
        if (angle != NULL)
            *angle += (i32)(RotDiff(*angle, next_angle) * fraction);
        if (pitch != NULL)
            *pitch += (i32)(RotDiff(*pitch, next_pitch) * fraction);
    }
}

f32 SplineLength(nugspline_s *spline, i32 closed) {
    if (spline == NULL || spline->length < 2)
        return 0.0f;
    i32 segment_count = closed != 0 ? spline->length : spline->length - 1;
    f32 length = 0.0f;
    NUVEC *previous = spline->pts;
    i32 segment = 0;
    do {
        NUVEC *next = &spline->pts[segment == spline->length - 1 ? 0 : segment + 1];
        NUVEC difference;
        NuVecSub(&difference, next, previous);
        length += NuVecMag(&difference);
        previous = next;
        ++segment;
    } while (segment < segment_count);
    return length;
}

i32 InsideLineXZ(f32 point_u, f32 point_v, f32 line_start_u, f32 line_start_v, f32 line_end_u, f32 line_end_v) {
    const f32 side =
        (point_u - line_start_u) * (line_end_v - line_start_v) + (point_v - line_start_v) * (line_start_u - line_end_u);
    return side >= 0.0f;
}

i32 LineCrossedXZ(f32 ax, f32 az, f32 bx, f32 bz, f32 cx, f32 cz, f32 dx, f32 dz) {
    i32 result = 0;
    if (!((bx - cx) * (dz - cz) + (bz - cz) * (cx - dx) >= 0.0f) &&
        (ax - cx) * (dz - cz) + (az - cz) * (cx - dx) >= 0.0f) {
        result = 1;
        if ((bx - ax) * (cz - az) + (bz - az) * (ax - cx) >= 0.0f &&
            (bx - dx) * (az - dz) + (bz - dz) * (dx - ax) >= 0.0f) {
            result = 2;
        }
    }
    return result;
}

f32 RatioAlongLineXZ(nuvec_s *point, nuvec_s *start, nuvec_s *end) {
    f32 dx = end->x - start->x;
    f32 dz = end->z - start->z;
    i32 angle = -NuAtan2D(dx, dz);
    f32 sine = NuTrigTable[static_cast<u16>(static_cast<u16>(angle) / 2)];
    f32 cosine = NuTrigTable[((static_cast<u16>(angle) + 0x4000) >> 1) & 0x7fff];
    f32 px = point->x - start->x;
    f32 pz = point->z - start->z;
    f32 along = pz * cosine - px * sine;
    if (along <= 0.0f)
        return 0.0f;
    f32 length = dz * cosine - dx * sine;
    if (along >= length)
        return 1.0f;
    return along / length;
}

f32 DistanceToLineXZ(NUVEC *position, NUVEC *first, NUVEC *second) {
    u16 angle = -NuAtan2D(second->x - first->x, second->z - first->z);
    return NuFabs((position->x - first->x) * NU_COS_LUT(angle) + (position->z - first->z) * NU_SIN_LUT(angle));
}

f32 RatioBetweenEdgesXZ(nuvec_s *point, nuvec_s *edge_a0, nuvec_s *edge_a1, nuvec_s *edge_b0, nuvec_s *edge_b1) {
    f32 distance_a = DistanceToLineXZ(point, edge_a0, edge_a1);
    f32 distance_b = DistanceToLineXZ(point, edge_b0, edge_b1);
    return distance_a / (distance_a + distance_b);
}

i32 LineIntersectXY(nuvec_s *a, nuvec_s *b, nuvec_s *c, nuvec_s *d, nuvec_s *first, nuvec_s *second) {
    const f32 ax = a->x, ay = a->y;
    const f32 dx = b->x - ax, dy = b->y - ay;
    const f32 ex = d->x - c->x, ey = d->y - c->y;
    const f32 denominator = ey * dx - ex * dy;
    if (denominator == 0.0f)
        return 0;
    const f32 ox = ax - c->x, oy = ay - c->y;
    const f32 t = (ex * oy - ey * ox) / denominator;
    const f32 u = (oy * dx - ox * dy) / denominator;
    if (first != NULL) {
        first->x = dx * t + ax;
        first->y = dy * t + ay;
    }
    if (second != NULL) {
        second->x = (d->x - c->x) * u + c->x;
        second->y = (d->y - c->y) * u + c->y;
    }
    return t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f;
}

i32 XZLinesIntersect(nuvec_s *a, nuvec_s *b, nuvec_s *c, nuvec_s *d, float *first, float *second) {
    NUVEC ac __attribute__((aligned(16)));
    NUVEC bc __attribute__((aligned(16)));
    NUVEC ca __attribute__((aligned(16)));
    NUVEC da __attribute__((aligned(16)));
    NUVEC direction;
    NuVecSub(&direction, b, a);
    i32 angle = NuAtan2D(direction.x, direction.z);
    NuVecSub(&ca, c, a);
    NuVecRotateY(&ca, &ca, -angle);
    NuVecSub(&da, d, a);
    NuVecRotateY(&da, &da, -angle);
    if (NuFsign(ca.x) == NuFsign(da.x))
        return 0;

    NuVecSub(&direction, d, c);
    angle = NuAtan2D(direction.x, direction.z);
    NuVecSub(&ac, a, c);
    NuVecRotateY(&ac, &ac, -angle);
    NuVecSub(&bc, b, c);
    NuVecRotateY(&bc, &bc, -angle);
    if (NuFsign(ac.x) == NuFsign(bc.x))
        return 0;

    if (first != NULL)
        *first = __builtin_fabsf(ac.x) / (__builtin_fabsf(ac.x) + __builtin_fabsf(bc.x));
    if (second != NULL)
        *second = __builtin_fabsf(ca.x) / (__builtin_fabsf(ca.x) + __builtin_fabsf(da.x));
    return 1;
}

f32 XZLinesClosest(nuvec_s *a, nuvec_s *b, nuvec_s *c, nuvec_s *d, f32 *first, f32 *second) {
    NUVEC ac __attribute__((aligned(16)));
    NUVEC bc __attribute__((aligned(16)));
    NUVEC ca __attribute__((aligned(16)));
    NUVEC da __attribute__((aligned(16)));
    NUVEC direction;
    NuVecSub(&direction, b, a);
    i32 angle = NuAtan2D(direction.x, direction.z);
    NuVecSub(&ca, c, a);
    NuVecRotateY(&ca, &ca, -angle);
    NuVecSub(&da, d, a);
    NuVecRotateY(&da, &da, -angle);
    NuVecSub(&direction, d, c);
    angle = NuAtan2D(direction.x, direction.z);
    NuVecSub(&ac, a, c);
    NuVecRotateY(&ac, &ac, -angle);
    NuVecSub(&bc, b, c);
    NuVecRotateY(&bc, &bc, -angle);

    if (NuFsign(ca.x) != NuFsign(da.x)) {
        if (NuFsign(ac.x) != NuFsign(bc.x)) {
            *first = NuFabs(ac.x) / (NuFabs(ac.x) + NuFabs(bc.x));
            *second = NuFabs(ca.x) / (NuFabs(ca.x) + NuFabs(da.x));
            return 0.0f;
        }
    }

    f32 ab_length = NuVecXZDist(b, a, NULL);
    f32 cd_length = NuVecXZDist(d, c, NULL);
    f32 closest = 1000000000.0f;
    f32 distance, fraction;
    if (ca.z < 0.0f) {
        distance = NuVecXZDist(a, &ca, NULL);
        fraction = 0.0f;
    } else if (ca.z > ab_length) {
        distance = NuVecXZDist(b, &ca, NULL);
        fraction = 1.0f;
    } else {
        fraction = ca.z / ab_length;
        distance = NuFabs(ca.x);
    }
    if (distance < closest) {
        *first = fraction;
        *second = 0.0f;
        closest = distance;
    }
    if (da.z < 0.0f) {
        distance = NuVecXZDist(a, &da, NULL);
        fraction = 0.0f;
    } else if (da.z > ab_length) {
        distance = NuVecXZDist(b, &da, NULL);
        fraction = 1.0f;
    } else {
        fraction = da.z / ab_length;
        distance = NuFabs(da.x);
    }
    if (distance < closest) {
        *first = fraction;
        *second = 1.0f;
        closest = distance;
    }
    if (!(ac.z < 0.0f)) {
        if (ac.z > cd_length) {
            distance = NuVecXZDist(d, &ac, NULL);
            fraction = 1.0f;
        } else {
            fraction = ac.z / cd_length;
            distance = NuFabs(ac.x);
        }
    } else {
        distance = NuVecXZDist(c, &ac, NULL);
        fraction = 0.0f;
    }
    if (distance < closest) {
        *second = fraction;
        *first = 0.0f;
        closest = distance;
    }
    if (!(bc.z < 0.0f)) {
        if (bc.z > cd_length) {
            distance = NuVecXZDist(d, &bc, NULL);
            fraction = 1.0f;
        } else {
            fraction = bc.z / cd_length;
            distance = NuFabs(bc.x);
        }
    } else {
        distance = NuVecXZDist(c, &bc, NULL);
        fraction = 0.0f;
    }
    if (distance < closest) {
        *second = fraction;
        *first = 1.0f;
        closest = distance;
    }
    return closest;
}

i32 LineIntersectSphere(NUVEC *origin, NUVEC *direction, NUVEC *center, f32 radius_squared, f32 *distance_squared) {
    f32 x = center->x - origin->x;
    f32 y = center->y - origin->y;
    f32 z = center->z - origin->z;
    f32 projection = direction->x * x + direction->y * y + direction->z * z;
    if (projection < 0.0f)
        return 0;
    f32 distance = x * x + y * y + z * z - projection * projection;
    if (distance <= radius_squared) {
        if (distance_squared != NULL)
            *distance_squared = distance;
        return 1;
    }
    return 0;
}

i32 LineIntersectCircle(NUVEC *origin, NUVEC *direction, NUVEC *center, f32 radius_squared) {
    f32 x = center->x - origin->x;
    f32 z = center->z - origin->z;
    f32 projection = direction->x * x + direction->z * z;
    if (projection < 0.0f)
        return 0;
    return x * x + z * z - projection * projection <= radius_squared;
}

void MoveSplinePosition(SPLINEPOS_s *position, f32 distance) {
    if (position != NULL && position->spline != NULL && position->spline->length > 1) {
        NUGSPLINE *spline = position->spline;
        i32 points = spline->length;
        if (position->looping)
            points++;
        i32 segments = points - 1;
        if (position->segment < segments) {
            NUVEC offset;
            if (distance >= 0.0f) {
                while (distance > 0.0f) {
                    position->segment_distance += distance;
                    if (__builtin_expect(!(position->segment_distance >= position->segment_length), 0))
                        goto interpolate;
                    distance = position->segment_distance - position->segment_length;
                    i16 previous = position->segment;
                    position->segment++;
                    if (position->segment >= segments) {
                        if (__builtin_expect(!position->looping, 0)) {
                            position->reached_end = 1;
                            position->position =
                                *(NUVEC *)((u8 *)spline->pts + position->segment * (i16)spline->pt_size);
                            position->segment = previous;
                            position->along = 1.0f;
                            position->segment_distance = position->segment_length;
                            return;
                        }
                        position->segment = 0;
                    }
                    NUVEC *current = (NUVEC *)((u8 *)spline->pts + position->segment * (i16)spline->pt_size);
                    NUVEC *next = (NUVEC *)((u8 *)spline->pts +
                                            ((position->segment + 1) % spline->length) * (i16)spline->pt_size);
                    position->segment_distance = 0.0f;
                    position->segment_length = NuVecDist(next, current, &offset);
                    if (distance == 0.0f)
                        position->position = *current;
                    spline = position->spline;
                }
                goto update_along;
            } else if (position->segment >= 0 && distance < 0.0f) {
                for (;;) {
                    position->segment_distance += distance;
                    if (__builtin_expect(position->segment_distance >= 0.0f, 0))
                        break;
                    distance = position->segment_distance;
                    i16 previous = position->segment;
                    position->segment--;
                    if (position->segment < 0) {
                        if (!position->looping) {
                            position->reached_end = 1;
                            position->position = *spline->pts;
                            position->segment = previous;
                            position->along = 0.0f;
                            position->segment_distance = 0.0f;
                            return;
                        }
                        position->segment = segments - 1;
                    }
                    NUVEC *current = (NUVEC *)((u8 *)spline->pts + position->segment * (i16)spline->pt_size);
                    NUVEC *next = (NUVEC *)((u8 *)spline->pts +
                                            ((position->segment + 1) % spline->length) * (i16)spline->pt_size);
                    position->segment_length = NuVecDist(next, current, &offset);
                    position->segment_distance = position->segment_length;
                    if (distance == 0.0f)
                        position->position = *next;
                    spline = position->spline;
                }
            } else
                goto update_along;
        interpolate: {
            NUVEC *current = (NUVEC *)((u8 *)spline->pts + position->segment * (i16)spline->pt_size);
            NUVEC *next =
                (NUVEC *)((u8 *)spline->pts + ((position->segment + 1) % spline->length) * (i16)spline->pt_size);
            NuVecSub(&offset, next, current);
            f32 fraction = 0.0f;
            if (position->segment_length != 0.0f)
                fraction = position->segment_distance / position->segment_length;
            NuVecScale(&offset, &offset, fraction);
            NuVecAdd(&position->position, current, &offset);
        }
        update_along:
            position->along = (position->segment_distance / position->segment_length + position->segment) / segments;
        }
    }
}

void InitSplinePosition(SPLINEPOS_s *position, nugspline_s *spline, float distance, i32 looping) {
    if (position == NULL) {
        return;
    }

    SPLINEPOS_s *runtime = position;
    memset(runtime, 0, sizeof(*runtime));
    if (spline == NULL || spline->length < 2) {
        return;
    }

    runtime->spline = spline;
    runtime->looping = static_cast<u8>(looping);
    const NUVEC *first = reinterpret_cast<const NUVEC *>(reinterpret_cast<const u8 *>(spline->pts));
    const NUVEC *second = reinterpret_cast<const NUVEC *>(reinterpret_cast<const u8 *>(spline->pts) + spline->pt_size);
    runtime->segment_length = NuVecDist(const_cast<NUVEC *>(second), const_cast<NUVEC *>(first), NULL);
    if (distance > 0.0f) {
        MoveSplinePosition(position, distance);
    } else {
        runtime->position = *first;
        runtime->along = 0.0f;
    }
}

void GetNearestSplinePos(NUVEC *origin, SPLINEPOS_s *result, NUGSPLINE *spline, i32 looping, i16 first_point,
                         i16 last_point) {
    NUVEC offset;
    if (result == NULL)
        return;
    memset(result, 0, sizeof(*result));
    if (!(spline == NULL || origin == NULL || spline->length <= 1)) {
        result->spline = spline;
        result->looping = (i8)looping;
        i16 point_count = spline->length;
        i32 logical_count = result->looping != 0 ? point_count + 1 : point_count;
        i32 index = first_point;
        if (index < 0)
            index = 0;
        else if (__builtin_expect(index >= logical_count, 0))
            return;
        i32 end = point_count;
        if (last_point >= 0 && last_point < end)
            end = last_point;
        NUVEC *point = (NUVEC *)((u8 *)spline->pts + index * (i16)spline->pt_size);
        f32 nearest = 1000000000.0f;

        do {
            f32 distance = NuVecDistSqr(origin, point, &offset);
            if (distance < nearest) {
                nearest = distance;
                result->segment = index;
            }
            point = (NUVEC *)((u8 *)point + (i16)result->spline->pt_size);
            index++;
        } while (index < end);
        spline = result->spline;
        i32 stride = (i16)spline->pt_size;
        NUVEC *current = (NUVEC *)((u8 *)spline->pts + result->segment * stride);
        NUVEC *next = (NUVEC *)((u8 *)spline->pts + ((result->segment + 1) % spline->length) * stride);
        result->segment_distance = 0.0f;
        result->segment_length = NuVecDist(next, current, &offset);
        result->position = *current;
        result->along = (result->segment_distance / result->segment_length + result->segment) / (logical_count - 1);
    }
}

void EvaluateSplineXZIntersection(nugspline_s *first, i32 first_looping, SPLINEPOS_s *first_position,
                                  nugspline_s *second, i32 second_looping, SPLINEPOS_s *second_position) {
    memset(first_position, 0, sizeof(*first_position));
    memset(second_position, 0, sizeof(*second_position));
    first_position->spline = first;
    first_position->looping = static_cast<i8>(first_looping);
    second_position->spline = second;
    second_position->looping = static_cast<i8>(second_looping);
    if (!(second == NULL || first == NULL || first->length == 0 || second->length == 0)) {
        const i32 second_count = second_looping != 0 ? second->length + 1 : second->length;
        const i32 first_count = first_looping != 0 ? first->length + 1 : first->length;
        f32 closest = 1000000000.0f;
        f32 first_fraction, second_fraction;
        for (i32 i = 0; i < first_count - 1; ++i) {
            const i32 first_segment = i % first->length;
            NUVEC *first_start = &first->pts[first_segment];
            NUVEC *first_end = &first->pts[(i + 1) % first->length];
            // Retail includes the wraparound segment of the second spline even
            // without looping, and only stops this inner scan at an intersection.
            for (i32 j = 0; j < second_count; ++j) {
                const i32 second_segment = j % second->length;
                const f32 distance =
                    XZLinesClosest(first_start, first_end, &second->pts[second_segment],
                                   &second->pts[(j + 1) % second->length], &first_fraction, &second_fraction);
                if (distance < closest) {
                    first_position->segment_distance = first_fraction;
                    first_position->segment = static_cast<i16>(first_segment);
                    second_position->segment_distance = second_fraction;
                    second_position->segment = static_cast<i16>(second_segment);
                    closest = distance;
                    if (distance == 0.0f)
                        break;
                }
            }
        }

        NUVEC direction;
        first_position->segment_length = NuVecDist(&first->pts[(first_position->segment + 1) % first->length],
                                                   &first->pts[first_position->segment], &direction);
        first_position->segment_distance *= first_position->segment_length;
        MoveSplinePosition(first_position, 0.00001f);
        second_position->segment_length = NuVecDist(&second->pts[(second_position->segment + 1) % second->length],
                                                    &second->pts[second_position->segment], &direction);
        second_position->segment_distance *= second_position->segment_length;
        MoveSplinePosition(second_position, 0.00001f);
    }
}

i32 OutSideSplineArea(nuvec_s *position, nugspline_s *spline, nuvec_s *edge_end, nuvec_s *edge_start, i32 inside) {
    if (__builtin_expect(spline == NULL || position == NULL, 0))
        return 0;
    NUVEC ray_start = {position->x, -position->z, position->y};
    NUVEC ray_end;
    ray_end.x = position->x;
    ray_end.y = 100000.0f;
    ray_end.z = position->y;
    NUVEC end = spline->pts[0];
    f32 end_y = end.y;
    end.y = -end.z;
    end.z = end_y;
    i32 intersections = 0;
    for (i32 i = 1; i < spline->length; ++i) {
        NUVEC start = end;
        end = spline->pts[i];
        end_y = end.y;
        end.y = -end.z;
        end.z = end_y;
        intersections += LineIntersectXY(&ray_start, &ray_end, &start, &end, NULL, NULL);
    }
    if ((intersections & 1) != 0 ? inside == 0 : inside != 0)
        return 0;
    i32 closest_index = 0;
    NUVEC closest = spline->pts[0];
    f32 best = 1000000000.0f;
    for (i32 i = 0; i < spline->length - 1; ++i) {
        f32 dx = spline->pts[i].x - position->x;
        f32 dz = spline->pts[i].z - position->z;
        f32 distance = dx * dx + dz * dz;
        if (__builtin_expect(distance < best, 1))
            closest_index = i;
        closest.x = distance < best ? spline->pts[i].x : closest.x;
        closest.y = distance < best ? spline->pts[i].y : closest.y;
        closest.z = distance < best ? spline->pts[i].z : closest.z;
        best = distance < best ? distance : best;
    }
    i32 previous = closest_index == 0 ? spline->length - 2 : closest_index - 1;
    i32 next = closest_index == spline->length - 2 ? 0 : closest_index + 1;
    f32 bx = spline->pts[previous].x - position->x;
    f32 ax = spline->pts[next].x - position->x;
    f32 az = spline->pts[next].z - position->z;
    f32 bz = spline->pts[previous].z - position->z;
    if (bx * bx + bz * bz > ax * ax + az * az) {
        if (edge_end != NULL)
            *edge_end = spline->pts[next];
        if (edge_start != NULL)
            *edge_start = closest;
    } else {
        if (edge_end != NULL)
            *edge_end = closest;
        if (edge_start != NULL)
            *edge_start = spline->pts[previous];
    }
    return 1;
}

void VecRotateAxis(nuvec_s *vector, u16 angle, nuvec_s *axis) {
    NuVecNorm(axis, axis);
    const f32 cosine = NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff];
    const f32 sine = NuTrigTable[static_cast<u16>(angle / 2)];
    const f32 complement = 1.0f - cosine;
    const f32 x = axis->x;
    const f32 y = axis->y;
    const f32 z = axis->z;
    const f32 ty = complement * y;
    const NUVEC source = *vector;
    const f32 tx = complement * x;
    const f32 tz = complement * z;
    const f32 xy = tx * y;
    const f32 yz = ty * z;
    const f32 sx = sine * x;
    const f32 xz = tx * z;
    const f32 sz = z * sine;
    const f32 sy = sine * y;
    vector->x = ((tx * x + cosine) * source.x + 0.0f) + (xy - sz) * source.y + (xz + sy) * source.z;
    vector->y = ((xy + sz) * source.x + 0.0f) + (y * ty + cosine) * source.y + (yz - sx) * source.z;
    vector->z = ((xz - sy) * source.x + 0.0f) + (sx + yz) * source.y + (tz * z + cosine) * source.z;
}

void *GameBufferAlloc(variptr_u *buf, variptr_u *buf_end, i32 size) {
    if (buf_end == NULL || buf == NULL || buf_end->addr <= buf->addr + size) {
        return NULL;
    }
    buf->addr = (buf->addr + 3) & ~usize(3);
    void *ptr = buf->void_ptr;
    buf->addr += size;
    if (ptr == NULL) {
        return NULL;
    }
    memset(ptr, 0, size);
    return ptr;
}

void EvalAnim(nuhspecial_s *special, float frame, numtx_s *matrix, i32 include_instance_translation) {
    if (matrix == NULL || special == NULL) {
        return;
    }

    nuinstanim_s *instance_animation = NuSpecialGetInstAnim(special);
    if (instance_animation == NULL) {
        if (include_instance_translation != 0) {
            NUMTX *instance_matrix = NuSpecialGetInstanceMtx(special);
            if (instance_matrix != NULL) {
                memcpy(matrix, instance_matrix, sizeof(NUMTX));
            }
        }
        return;
    }

    NUGSCN *scene = special->scene;
    nuanimdata_s *animation = scene->instance_animation_data[instance_animation->anim_ix];
    if (animation == NULL) {
        return;
    }

    NuAnimData2CalcMatrix(animation, 0, frame, matrix);
    if (include_instance_translation != 0) {
        NUMTX *instance_matrix = NuSpecialGetInstanceMtx(special);
        if (instance_matrix != NULL) {
            NUVEC *translation = NUMTX_GET_ROW_VEC(matrix, 3);
            const NUVEC *instance_translation = NUMTX_GET_ROW_VEC(instance_matrix, 3);
            translation->x += instance_translation->x;
            translation->y += instance_translation->y;
            translation->z += instance_translation->z;
        }
    }
}

void EvalAnim2(nuhspecial_s *special, float frame) {
    nuinstanim_s *instance_animation = NuSpecialGetInstAnim(special);
    if (instance_animation == NULL) {
        return;
    }

    NUGSCN *scene = special->scene;
    nuanimdata_s *animation = scene->instance_animation_data[instance_animation->anim_ix];
    if (animation == NULL || frame == instance_animation->prev_eval_time) {
        return;
    }

    NUMTX *instance_matrix = NuSpecialGetInstanceMtx(special);
    NuAnimData2CalcMatrix(animation, 0, frame, &instance_animation->mtx);

    instance_animation->prev_eval_time = frame;
    const i32 instance_index = instance_animation - scene->instance_animations;
    NUVEC &animation_translation = *NUMTX_GET_ROW_VEC(&instance_animation->mtx, 3);
    NUVEC &instance_translation = *NUMTX_GET_ROW_VEC(instance_matrix, 3);
    NUMTX *evaluated_matrix = &scene->instance_animation_matrices[instance_index];
    memcpy(evaluated_matrix, &instance_animation->mtx, sizeof(NUMTX));

    animation_translation.x += instance_translation.x;
    animation_translation.y += instance_translation.y;
    animation_translation.z += instance_translation.z;
    NuSpecialUpdate(special);
}

void SetAnimFrame(nuhspecial_s *special, float frame) {
    if (NuSpecialExistsFn(special) != 0) {
        NUMTX_ALIGNED16 matrix;
        NuMtxSetIdentity(&matrix);
        nuinstanim_s *instance_animation = NuSpecialGetInstAnim(special);
        if (instance_animation == NULL)
            return;
        nuanimdata_s *animation = special->scene->instance_animation_data[instance_animation->anim_ix];
        if (animation == NULL)
            return;
        // The animation header begins with its final frame; the remaining header is opaque here.
        f32 end_frame;
        memcpy(&end_frame, animation, sizeof(end_frame));
        if (frame == 1.0e9f)
            frame = end_frame;
        if (!(frame >= 1.0f) || frame > end_frame)
            return;
        NuAnimData2CalcMatrix(animation, 0, frame, &matrix);
        instance_animation->mtx = matrix;
        NUMTX *instance_matrix = NuSpecialGetInstanceMtx(special);
        instance_animation->mtx.m30 = instance_matrix->m30;
        instance_animation->mtx.m31 = instance_matrix->m31;
        instance_animation->mtx.m32 = instance_matrix->m32;
        instance_animation->ltime = frame;
    }
}

f32 NuSpecialGetAnimPos(nuhspecial_s *special) {
    nuinstanim_s *animation = NuSpecialGetInstAnim(special);
    if (animation == NULL || special->scene->instance_animation_data[animation->anim_ix] == NULL) {
        return 0.0f;
    }
    f32 end_frame = NuSpecialGetAnimEndFrame(special);
    f32 position = (animation->ltime - 1.0f) / (end_frame - 1.0f);
    if (position < 0.0f) {
        return 0.0f;
    }
    return position > 1.0f ? 1.0f : position;
}

void SaveSceneObjectAnimTFactors(NUGSCN *gscn) {
    i32 count;
    char *p;
    void *obj;

    if (gscn == NULL) {
        return;
    }
    if (*(void **)((char *)gscn + 0x110) != NULL) {
        return;
    }
    count = *(i32 *)((char *)gscn + 0x1c);
    p = *(char **)((char *)gscn + 0x20) + count * 0x50;
    while (count != 0) {
        p -= 0x50;
        obj = *(void **)(p + 0x48);
        if (obj != NULL) {
            *(f32 *)((char *)obj + 0x54) = *(f32 *)((char *)obj + 0x40);
        }
        count--;
    }
}

void DrawRectRGBA(float x, float y, float width, float height, u32 colour, numtl_s *material, i32 alignment,
                  float scale) {
    const float scaled_width = width * scale;
    const float scaled_height = height * scale;
    const i32 rect_width = static_cast<i32>(scaled_width * 10240.0f * 0.5f);
    const i32 rect_height = static_cast<i32>(scaled_height * 3584.0f * 0.5f);
    if ((alignment & 5) == 4)
        y += scaled_height;
    else if ((alignment & 5) == 0)
        y += scaled_height * 0.5f;
    if ((alignment & 10) == 8)
        x -= scaled_width;
    else if ((alignment & 10) == 0)
        x -= scaled_width * 0.5f;
    NuRndrRect2di(static_cast<i32>((x + 1.0f) * 0.5f * 10240.0f),
                  static_cast<i32>((2.0f - (y + 1.0f)) * 0.5f * 3584.0f), rect_width, rect_height, colour, material);
}

void InitTexAnimScripts(char **names) {
    TexAnimList = names;
    if (names == NULL)
        return;
    while (*names != NULL) {
        permbuffer_ptr.addr = ALIGN(permbuffer_ptr.addr, 4);
        char path[72];
        NuStrCpy(path, "stuff\\ats\\");
        NuStrCat(path, *names++);
        NuStrCat(path, ".ats");
        NuTexAnimProgReadScript(path, &permbuffer_ptr, permbuffer_end, static_cast<i32>(DEFAULTFPS));
    }
    permbuffer_ptr.addr = ALIGN(permbuffer_ptr.addr, 16);
}

void SetTexAnimSignals() {
    NuTexAnimSetSignals(texanimbits);
}

i32 Draw3DObjectAlpha(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                      u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order, float alpha) {
    if (object_index == -1) {
        return 1;
    }
    if (world == NULL) {
        world = WorldInfo_CurrentlyActive();
    }
    if (world == NULL) {
        return 1;
    }
    if (world->lev_objs[object_index].active != 0 && (scale_x != 0.0f || scale_y != 0.0f || scale_z != 0.0f)) {
        NUVEC scale = {scale_x, scale_y, scale_z};
        NUMTX_ALIGNED16 matrix;
        NuMtxSetScale(&matrix, &scale);
        RotateGameMatrix(&matrix, rotate_order, x_rotation, y_rotation, z_rotation);
        NuMtxTranslate(&matrix, position);
        return NuSpecialDrawAtAlpha(&world->lev_objs[object_index].special, &matrix, alpha);
    }
    return 0;
}

i32 Draw3DObject(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                 u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order) {
    if (object_index == -1) {
        return 1;
    }
    if (world == NULL) {
        world = WorldInfo_CurrentlyActive();
    }
    if (world == NULL) {
        return 1;
    }
    if (world->lev_objs[object_index].active != 0 && (scale_x != 0.0f || scale_y != 0.0f || scale_z != 0.0f)) {
        NUVEC scale = {scale_x, scale_y, scale_z};
        NUMTX_ALIGNED16 matrix;
        NuMtxSetScale(&matrix, &scale);
        RotateGameMatrix(&matrix, rotate_order, x_rotation, y_rotation, z_rotation);
        NuMtxTranslate(&matrix, position);
        return NuSpecialDrawAt(&world->lev_objs[object_index].special, &matrix);
    }
    return 0;
}

i32 Draw3DObjectMtx(WORLDINFO_s *world, i32 object_index, numtx_s *mtx) {
    if (object_index == -1) {
        return 1;
    }
    if (world == NULL) {
        world = WorldInfo_CurrentlyActive();
    }
    if (world == NULL) {
        return 1;
    }
    LEVEL_OBJECT_RUNTIME &object = world->lev_objs[object_index];
    if (object.active != 0) {
        return NuSpecialDrawAt(&object.special, mtx);
    }
    return 0;
}

i32 DrawPanel3DObject(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                      u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order, float alpha) {
    if (alpha > 0.0f && special != NULL && NuSpecialExistsFn(special) != 0 &&
        (scale_y != 0.0f || scale_x != 0.0f || scale_z != 0.0f)) {
        NUVEC scale = {scale_x / CameraZoom, scale_y / CameraZoom, scale_z / CameraZoom};
        NUMTX_ALIGNED16 matrix;
        NuMtxSetScale(&matrix, &scale);
        RotateGameMatrix(&matrix, rotate_order, rotate_x, rotate_y, rotate_z);
        matrix.m30 = x * PANEL3DMULX;
        matrix.m31 = y * PANEL3DMULY;
        matrix.m32 = z;
        NuMtxMulVU0(&matrix, &matrix, NuCameraGetMtx());
        NuSpecialDrawAtAlpha(special, &matrix, alpha);
    }
    return 0;
}

i32 DrawPanel3DObjectNoAlpha(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                             u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order) {
    if (special != NULL && NuSpecialExistsFn(special) != 0 && (scale_y != 0.0f || scale_x != 0.0f || scale_z != 0.0f)) {
        NUVEC scale = {scale_x / CameraZoom, scale_y / CameraZoom, scale_z / CameraZoom};
        NUMTX_ALIGNED16 matrix;
        NuMtxSetScale(&matrix, &scale);
        RotateGameMatrix(&matrix, rotate_order, rotate_x, rotate_y, rotate_z);
        matrix.m30 = x * PANEL3DMULX;
        matrix.m31 = y * PANEL3DMULY;
        matrix.m32 = z;
        NuMtxMulVU0(&matrix, &matrix, NuCameraGetMtx());
        NuSpecialDrawAt(special, &matrix);
    }
    return 0;
}

void DrawPanel3DObjectMtx(nuhspecial_s *special, numtx_s *matrix, float alpha) {
    if (!(alpha <= 0.0f)) {
        NUVEC scale = {1.0f / CameraZoom, 1.0f / CameraZoom, 1.0f / CameraZoom};
        NuMtxPreScale(matrix, &scale);
        if (special != NULL && NuSpecialExistsFn(special) != 0) {
            NuMtxMulVU0(matrix, matrix, NuCameraGetMtx());
            NuSpecialDrawAtAlpha(special, matrix, alpha);
        }
    }
}

i32 DrawPanel3DObjectMtxNoAlpha(nuhspecial_s *special, numtx_s *matrix) {
    if (special == NULL || NuSpecialExistsFn(special) == 0)
        return 0;
    NuMtxMulVU0(matrix, matrix, NuCameraGetMtx());
    return NuSpecialDrawAt(special, matrix);
}

void DrawObjectOnCharacter(WORLDINFO_s *world, GameObject_s *object, i32 object_id, nuhspecial_s *special, i32 locator,
                           i32 second_locator, NUMTX *joints, i32 reflect, u32 layers, NUMTX *rotation,
                           NUVEC *translation, f32 alpha, f32 scale) {
    i32 position_only = dco_locatorposonly;
    dco_locatorposonly = 0;
    u16 rotate_z = dco_prerotatez;
    dco_prerotatez = 0;
    i32 id, axis, hat;
    f32 plane;
    CHARACTERMODEL_s *model;
    if (object != NULL) {
        id = object->id;
        model = object->apiobj.character_model;
        hat = object->field_0x108e;
        axis = object->field_0x1087;
        plane = object->field_0x1020;
    } else {
        id = dco_id;
        if (id == -1 || dco_gcdata == NULL || dco_cmodel == NULL)
            return;
        model = dco_cmodel;
        axis = dco_reflectaxis;
        plane = dco_reflectcoord;
        hat = dco_wearinghat;
    }
    dco_gcdata = NULL;
    dco_cmodel = NULL;
    dco_id = -1;
    dco_reflectaxis = 0;
    dco_reflectcoord = 2000000.0f;
    dco_wearinghat = 0;
    if (world == NULL)
        world = WorldInfo_CurrentlyActive();
    if (object_id != -1) {
        if (!world->lev_objs[object_id].active)
            return;
    } else if (special == NULL || !NuSpecialExistsFn(special)) {
        return;
    }
    if (locator == -1 || model->points_of_interest[locator] == NULL)
        return;
    NUMTX matrix, reflected, first, second;
    if (position_only)
        NuMtxSetTranslation(&matrix, reinterpret_cast<NUVEC *>(&joints[locator].m30));
    else
        matrix = joints[locator];
    if (second_locator != -1 && model->points_of_interest[second_locator] != NULL) {
        NUVEC position = {matrix.m30, matrix.m31, matrix.m32};
        if (!position_only) {
            first = matrix;
            first.m30 = first.m31 = first.m32 = 0.0f;
            second = joints[second_locator];
            second.m30 = second.m31 = second.m32 = 0.0f;
            QuatInterpolateRotationMatrix(&matrix, &first, &second, 0.5f);
        }
        matrix.m30 = (position.x + joints[second_locator].m30) * 0.5f;
        matrix.m31 = (position.y + joints[second_locator].m31) * 0.5f;
        matrix.m32 = (position.z + joints[second_locator].m32) * 0.5f;
    }
    if (rotate_z != 0)
        NuMtxPreRotateZ(&matrix, rotate_z);
    if (object_id != -1 && CutSceneSys->field_04 == object_id && DisguiseAdjustFn != NULL) {
        NUVEC adjustment, offset;
        DisguiseAdjustFn(id, hat, &adjustment, &offset);
        if (adjustment.x != 1.0f || adjustment.y != 1.0f || adjustment.z != 1.0f)
            NuMtxPreScale(&matrix, &adjustment);
        if (offset.x != 0.0f || offset.y != 0.0f || offset.z != 0.0f)
            NuMtxPreTranslate(&matrix, &offset);
    }
    if (rotation != NULL) {
        NUVEC position = {matrix.m30, matrix.m31, matrix.m32};
        NuMtxMulR(&matrix, rotation, &matrix);
        matrix.m30 = position.x;
        matrix.m31 = position.y;
        matrix.m32 = position.z;
    }
    if (translation != NULL)
        NuMtxTranslate(&matrix, translation);
    i32 use_alpha = 0;
    if (object_id != -1) {
        special = &world->lev_objs[object_id].special;
        if (ObjTabList != NULL)
            use_alpha = ObjTabList[object_id].pad_01;
    }
    if (scale != 1.0f)
        NuMtxPreScaleU(&matrix, scale);
    if (!use_alpha)
        NuSpecialDrawAt(special, &matrix);
    else
        NuSpecialDrawAtAlpha(special, &matrix, alpha);
    if (reflect && MatrixReflection(&matrix, axis, plane, world->current_level->unknown_0cc, &reflected)) {
        NuRndrStartReflectionRender(0);
        if (use_alpha)
            NuSpecialDrawAtAlpha(special, &reflected, alpha);
        else
            NuSpecialDrawAt(special, &reflected);
        NuRndrEndReflectionRender();
    }
}

void CreateUsefulMaterials() {
    SolidMtl = NuMtlCreate(1);
    SolidMtl->attribs.z_mode = 0;
    SolidMtl->attribs.alpha_mode = 0;
    SolidMtl->attribs.unknown_2_4 = 1;
    NuMtlUpdate(SolidMtl);

    SolidMtl3D = NuMtlCreate3D(1);
    SolidMtl3D->attribs.z_mode = 0;
    SolidMtl3D->attribs.alpha_mode = 0;
    SolidMtl3D->attribs.unknown_2_4 = 1;
    NuMtlUpdate(SolidMtl3D);
}

void IncreaseScore(u32 *total, u64 amount, i32 apply_multiplier) {
    if (apply_multiplier != 0 && BonusArea == 0) {
        if (Cheats_CheckFlags(4) != 0) {
            amount *= 2;
        }
        if (Cheats_CheckFlags(8) != 0) {
            amount *= 4;
        }
        if (Cheats_CheckFlags(0x10) != 0) {
            amount *= 6;
        }
        if (Cheats_CheckFlags(0x20) != 0) {
            amount *= 8;
        }
        if (Cheats_CheckFlags(0x40) != 0) {
            amount *= 10;
        }
    }
    amount += *total;
    if (amount > 4000000000ULL) {
        amount = 4000000000ULL;
    }
    *total = static_cast<u32>(amount);
}

