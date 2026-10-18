// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include <string.h>
#include <stdio.h>

#include "decomp.h"
#include "globals.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/level.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/render/fx/parts.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/world/world.h"
#include <new>
#include "legoapi/props/doors/door.h"
#include "legoapi/world/mission.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nufile/nufile.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edstubs.h"
#include "nu2api/numusic/sfx.h"
#include "legoapi/render/core/gameliball.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/nurndr.h"
#include <math.h>
#include <stddef.h>
#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/items/collect/torpedo.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "legoapi/render/fx/edsplines.h"
#include <float.h>
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include <stdlib.h>
#include "gameframework/saveload.h"
#include <cstdio>
#include "nu2api/nu3d/nuspline.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/numath/nuvec4.h"
#include <stdarg.h>
#include <GLES2/gl2.h>
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/numath/nuang.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nu3d/nupostparams.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/characters/motion/move.h"
#include "legoapi/lib/camera.h"

extern "C" void *AIPAthFindPathCnx(AISYS_s *, AIPATH_s *, char *, char *, i32 *);
extern "C" AIPATHNODE_s *AIPathFindNode(AISYS_s *, AIPATH_s *, char *);
extern "C" void AIPathNodeUpdatePos(AISYS_s *, AIPATH_s *, AIPATHNODE_s *);
void TrenchMove(GameObject_s *) asm("_ZL10TrenchMoveP12GameObject_s") __attribute__((visibility("hidden")));
void TrenchKilledCallback(GameObject_s *) asm("_ZL20TrenchKilledCallbackP12GameObject_s")
    __attribute__((visibility("hidden")));
extern "C" {
    extern i16 id_STORMTROOPER;
    extern i16 id_IMPERIALSHUTTLEPILOT;
}

struct BLOCKADERUNNERD_LEVFLAG_s {
    u8 obstacle15_active;
    u8 obstacle14_active;
    u8 reserved_02[0x0e];
};

static_assert(sizeof(BLOCKADERUNNERD_LEVFLAG_s) == 0x10, "LevFlag size");
extern i32 LevFlag[4];
#define LEV_FLAG_BR (*reinterpret_cast<BLOCKADERUNNERD_LEVFLAG_s *>(LevFlag))
i32 test_tb = 1;
void *deathstarescapeb_netpacket;
u8 tatooineA_nodesNeedUpdating = 1;
u8 mosEisleyB_nodesNeedUpdating = 1;
f32 trench_spawn_height = 40.0f;
f32 trench_spawn_distance = 20.0f;

// Death Star escape (DeathStarEscape_A / B / C / D)

__attribute__((force_align_arg_pointer)) void DeathStarEscapeB_Update(WORLDINFO_s *world);

struct GameObject_s;
struct WORLDINFO_s;
struct debinftype;

f32 trench_max_height_move = 5.0f;
f32 trench_roll_f = 5000.0f;
f32 trench_seek_z = 0.5f;
f32 trench_seek_y = 0.5f;
f32 trench_seek_x = 4.0f;
f32 LOOPTIME = 1.5f;

__used__ void TrenchMove(GameObject_s *object);

__used__ void TrenchKilledCallback(GameObject_s *object);

extern "C" {
    void *AIPAthFindPathCnx(AISYS_s *, AIPATH_s *, char *, char *, i32 *);
}

extern speedup_s DogFightSpeedList[9];

extern "C" {
    extern "C++" void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
}

extern "C" {
void Debris(i32 update_panel_time);
} // extern "C"

extern "C" {
extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId();
} // extern "C"

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

extern f32 HUB_EPISODETITLEY;
extern i16 tSELECT, tSELECTED, tSELECTING;

extern void (*drawslotinfofn)(f32, f32, i32, i32);
extern void (*drawslotsfn)(MENU *, f32);

void MenuInitialiseEx(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                      void (*draw_save_slots_info_fn)(f32, f32, i32, i32), i32 is_fade_enabled, i32 is_shadow_enabled);

void MenuInitialise(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                    void (*draw_save_slots_fn)(MENU *, f32), i32 is_fade_enabled, i32 is_shadow_enabled);

void NuPadRecordEndFrame(void);                      // nupad_interface.cpp

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

extern i16 id_STORMTROOPER;

extern "C" i16 id_LANDSPEEDER;
extern i32 CUTCAMONLY, GUNSHIPAHACK, netcamera, complexsockposition_forcesock, movegamecamera_forcesock;
GameObject_s *FindNearestGameObject(NUVEC *, GameObject_s *, u32, f32, f32, i32, i32, i32, f32 *, i32,
                                    i32 (*)(GameObject_s *), bool);

extern void (*FS_Callback)(char *, char *);

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

extern i16 tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);

extern i16 id_ANAKINJEDISCARRED, id_WOOKIEE, id_TWOFACE;

extern "C" {
extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

}

void NuPadRecordEndFrame(void);                      // nupad_interface.cpp

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

static i32 PartKill_DrawCreature(PART_s *) {
    return false;
}

__used__ void TrenchKilledCallback(GameObject_s *object) {
    for (i32 i = 0; i < 3; ++i) {
        if (trenchrun.objects[i] == object) {
            trenchrun.objects[i] = NULL;
            break;
        }
    }
}

static void PartKill_EjectedCreature(PART_s *part, i32) {
    for (i32 i = 0; i < 8; ++i) {
        if (LevGamePart[i] == part) {
            if (LevGameObject[i] != NULL) {
                KillGameObject(LevGameObject[i], 4, 0);
                LevGameObject[i] = NULL;
            }
            LevGamePart[i] = NULL;
        }
    }
}

__used__ void TrenchMove(GameObject_s *object) {
    APIOBJECT_s &api = object->apiobj;
    api.field_0x214 = api.field_0x218;
    api.start_position = api.position;
    api.initial_position = api.collision_position;
    api.field_0x27e = api.field_0x27d;
    object->pad_gamepad->previous_input_angle = object->pad_gamepad->input_angle;
    object->pad_gamepad->previous_input_magnitude = object->pad_gamepad->input_magnitude;
    object->previous_movement_angle = api.field_0x276;
    object->field_0xefd &= ~0x40;
    object->field_0x1086 = 0;
    if (api.model_draw_result != 0)
        object->field_0xf1c = 0.0f;
    else
        object->field_0xf1c += FRAMETIME;
    if (object->character_context == 0x2b)
        object->turn_braking += FRAMETIME;

    if (object->character_context != 0x2a) {
        if ((api.movement_facing_angle > 0x8000) != (player->apiobj.movement_facing_angle > 0x8000) ||
            (player->character_context == 0x2a &&
             (api.movement_facing_angle <= 0x8000) != (player->apiobj.movement_facing_angle > 0x8000) &&
             player->context_animation_timer < 0.5f * TURNTIME)) {
            StartTurn(object);
        } else if (object->character_context != 0x36 && player->character_context == 0x36 &&
                   player->context_animation_timer < 0.75f * LOOPTIME) {
            object->character_context = 0x36;
            object->context_animation = 1;
            object->airborne_action_duration = LOOPTIME;
            object->context_animation_timer = LOOPTIME;
        }
    }

    f32 target_x;
    if (object->character_context == 0x2a) {
        const f32 phase = object->context_animation_timer / TURNTIME;
        if (api.movement_facing_angle > 0x8000)
            target_x = phase * 10.0f + (1.0f - phase) * -10.0f + trenchrun.position.x;
        else
            target_x = phase * -10.0f + (1.0f - phase) * 10.0f + trenchrun.position.x;
    } else if (api.movement_facing_angle > 0x8000) {
        target_x = trenchrun.position.x + 10.0f;
    } else {
        target_x = trenchrun.position.x - 10.0f;
    }
    api.position.x = SeekValF(api.position.x, target_x, trench_seek_x);
    f32 target_y = trenchrun.position.y + object->movement_spline_offset.y + VehicleTurnOrLoopOffset(object);
    if (target_y - api.position.y > trench_max_height_move)
        target_y = api.position.y + trench_max_height_move;
    api.position.y = SeekValF(api.position.y, target_y, trench_seek_y);
    api.position.z = SeekValF(api.position.z, trenchrun.position.z + object->movement_spline_offset.z, trench_seek_z);
    NuVecSub(&api.velocity, &api.position, &api.start_position);
    NuVecScale(&api.velocity, &api.velocity, 1.0f / FRAMETIME);

    if ((api.character_data->game_character->flags_090 & 1) != 0) {
        i32 roll = 0;
        if (object->character_context != 0x36 && object->character_context != 0x2a &&
            object->character_context != 0x3a) {
            roll = static_cast<i32>(api.velocity.z * trench_roll_f);
        }
        if (static_cast<i16>(player->apiobj.movement_facing_angle) >= 0)
            roll = -roll;
        i32 target_roll;
        if (roll < -0x10000) {
            target_roll = -0x2000;
        } else if (roll > 0x10000) {
            target_roll = 0x2000;
        } else {
            target_roll = roll / 4;
            if (target_roll < -0x2000)
                target_roll = -0x2000;
            else if (target_roll > 0x2000)
                target_roll = 0x2000;
        }
        object->movement_lean_angle = SeekRot(object->movement_lean_angle, target_roll, 8.0f);
    }
    APIObjectVelocities(object);
    GameObjectOrigin(object);
}

static void BlockadeRunnerD_EjectCreature(i32 eject_index) {
    static NUVEC eject_vec[2] = {{13.0f, 1.6f, 7.93f}, {13.0f, 1.6f, 6.55f}};
    static NUVEC eject_mom = {-0.5f, 0.0f, 0.0f};
    i32 models[4] = {id_STORMTROOPER, id_BEACHTROOPER, id_STORMTROOPER, id_IMPERIALSHUTTLEPILOT};

    if (static_cast<u32>(eject_index) > 1)
        return;

    for (i32 i = 0; i < 8; ++i) {
        if (LevGamePart[i] == NULL) {
            NUMTX matrix;
            NuMtxSetTranslation(&matrix, &eject_vec[eject_index]);
            i32 model = models[qrand() / 0x4000];
            LevGameObject[i] =
                AddDynamicCreature(model, &eject_vec[eject_index], 0, "default", NULL, NULL, 0, NULL, NULL, 0, 0);
            if (LevGameObject[i] != NULL) {
                LevGameObject[i]->ai.animation_override_from = 0xe9;
                LevGameObject[i]->ai.animation_override_to = 5;
                LevGameObject[i]->dont_move = 1;
                LevGameObject[i]->field_0xf00 = (LevGameObject[i]->field_0xf00 & ~0x20) | ((test_tb & 1) << 5);
                LevGameObject[i]->hide_shadow = 1;

                ADDPART_s params = Default_ADDPART;
                params.matrix = &matrix;
                params.velocity = &eject_mom;
                params.field_14 = 0.15f;
                params.field_18 = 0.15f;
                params.gravity = 0.0f;
                params.special = &WORLD->lev_objs[254].special;
                params.flags = 0x8698;
                params.field_40 = PartCollide_3D;
                params.field_44 = PartKill_EjectedCreature;
                params.draw_fn = PartKill_DrawCreature;
                params.time_step = FRAMETIME;
                LevGamePart[i] = AddPart(&params);
                LevGamePart[i]->field_100 = 30.0f;
                return;
            }
        }
    }
}

// ===========================================================================
// Tatooine (Tatooine_A / B / C / D)
// ===========================================================================

void TatooineA_Init(WORLDINFO_s *world) {
    NUGSPLINE *spline = edSpline_SplineFind(world->current_gscn, "teleport_01");
    if (spline != NULL)
        spline->pts[0].x += 0.3f;

    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "thermaldet_011");
    if (blowup != NULL)
        blowup->draw_flags |= 2;
    blowup = GizmoBlowUp_FindByName(world, "thermaldet_021");
    if (blowup != NULL)
        blowup->draw_flags |= 2;
    blowup = GizmoBlowUp_FindByName(world, "bhblock_011");
    if (blowup != NULL)
        blowup->draw_flags |= 2;
    blowup = GizmoBlowUp_FindByName(world, "bhblock_021");
    if (blowup != NULL)
        blowup->draw_flags |= 2;

    LevGizForce[0] = GizForce_FindByName(world->giz_force_sys, "force13");
    LevGizForce[1] = GizForce_FindByName(world->giz_force_sys, "force14");
    LevGizForce[2] = GizForce_FindByName(world->giz_force_sys, "force15");
    LevAIPathNode[0] = AIPathFindNode(world->ai_sys, NULL, "force1_b");

    i32 direction;
    LevPathCnx[0] = AIPAthFindPathCnx(world->ai_sys, NULL, "force1_b", "force1_a", &direction);
    LevPathCnx[1] = AIPAthFindPathCnx(world->ai_sys, NULL, "force1_b", "force1_c", &direction);

    GIZFORCE_s *force = GizForce_FindByName(world->giz_force_sys, "force16");
    if (force != NULL)
        force->field_0xaa |= 0x80;
    tatooineA_nodesNeedUpdating = 1;
}

void TatooineA_Update(WORLDINFO_s *world) {
    GIZFORCE_s **forces = LevGizForce;
    GIZFORCE_s *first = forces[0];
    if (first == NULL)
        return;
    GIZFORCE_s *second = forces[1];
    if (second == NULL)
        return;
    GIZFORCE_s *third = forces[2];
    if (third == NULL)
        return;

    GIZFORCEGROUP_s *group = first->group;
    if (group != NULL && (group->field_0x24 & 2) != 0) {
        if (tatooineA_nodesNeedUpdating != 0)
            return;
        tatooineA_nodesNeedUpdating = 1;
        AIPATHCNX_s *connection = static_cast<AIPATHCNX_s *>(LevPathCnx[0]);
        if (connection != NULL) {
            connection->traversal_flags[0] &= ~0x80000000;
            connection->traversal_flags[1] &= ~0x80000000;
        }
        connection = static_cast<AIPATHCNX_s *>(LevPathCnx[1]);
        if (connection != NULL) {
            connection->traversal_flags[0] &= ~0x80000000;
            connection->traversal_flags[1] &= ~0x80000000;
        }

        AIPATHNODE_s *node = static_cast<AIPATHNODE_s *>(LevAIPathNode[0]);
        if (node == NULL)
            return;
        GIZFORCE_s *selected = group->forces[2];
        if (selected == first) {
            node->position.x = -16.44f;
            node->position.z = -0.16f;
        } else if (selected == second) {
            node->position.x = -16.58f;
            node->position.z = -0.21f;
        } else if (selected == third) {
            node->position.x = -16.71f;
            node->position.z = -0.25f;
        }
        if (world->ai_sys->path_sys != NULL && world->ai_sys->path_sys->active_path != NULL)
            AIPathNodeUpdatePos(world->ai_sys, world->ai_sys->path_sys->active_path, node);
        return;
    }

    if (tatooineA_nodesNeedUpdating == 0)
        return;
    tatooineA_nodesNeedUpdating = 0;
    AIPATHCNX_s *connection = static_cast<AIPATHCNX_s *>(LevPathCnx[0]);
    if (connection != NULL) {
        connection->traversal_flags[0] |= 0x80000000;
        connection->traversal_flags[1] |= 0x80000000;
    }
    connection = static_cast<AIPATHCNX_s *>(LevPathCnx[1]);
    if (connection != NULL) {
        connection->traversal_flags[0] |= 0x80000000;
        connection->traversal_flags[1] |= 0x80000000;
    }
}

void TatooineB_Init(WORLDINFO_s *world) {
    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "blowup_barrel14");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_barrel13");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_barrel15");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_barrel16");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_barrel17");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_barrel18");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_barrel19");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
}

void TatooineC_Init(WORLDINFO_s *world) {
    GIZMOBLOWUP_s *b = GizmoBlowUp_FindByName(world, "boxblowup_3");
    if (b != NULL)
        b->field_0xa0 |= 0x10000;
}

// ===========================================================================
// Mos Eisley (MosEisley_A / B / D / E)
// ===========================================================================

void MosEisleyA_Init(WORLDINFO_s *world) {
    char name[32];
    for (i32 i = 1; i <= 5; ++i) {
        sprintf(name, "big_bin_lid_gr%d", i);
        GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, name);
        if (blowup != NULL)
            blowup->field_0x124 = 1;
    }
    for (i32 i = 1; i <= 8; ++i) {
        sprintf(name, "big_bin_lid%d", i);
        GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, name);
        if (blowup != NULL)
            blowup->field_0x124 = 1;
    }

    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "evap_091");
    if (blowup != NULL) {
        blowup->field_0x128 = 0.5f;
        blowup->field_0x124 = 1;
    }
    blowup = GizmoBlowUp_FindByName(world, "evap_061");
    if (blowup != NULL) {
        blowup->field_0x128 = 0.5f;
        blowup->field_0x124 = 1;
        blowup->draw_flags |= 2;
    }

    GIZFORCE_s *force = GizForce_FindByName(world->giz_force_sys, "force1");
    if (force != NULL)
        force->strength_0x6c = 0.75f;
    force = GizForce_FindByName(world->giz_force_sys, "obstacle1");
    if (force != NULL)
        force->strength_0x6c = 0.75f;
}

void MosEisleyB_Init(WORLDINFO_s *world) {
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "land_speeder_terrain1", 1);
    LevGizObst[0] = GizObstacle_FindByName(world->giz_obstacle_sys, "Wash");
    LevGizForce[0] = GizForce_FindByName(world->giz_force_sys, "force1");
    LevGizForce[1] = GizForce_FindByName(world->giz_force_sys, "force2");
    LevGizForce[2] = GizForce_FindByName(world->giz_force_sys, "force3");

    LevAIPathNode[0] = AIPathFindNode(world->ai_sys, NULL, "stack_a");
    LevAIPathNode[1] = AIPathFindNode(world->ai_sys, NULL, "stack_b");
    LevAIPathNode[2] = AIPathFindNode(world->ai_sys, NULL, "stack_c");
    LevAIPathNode[3] = AIPathFindNode(world->ai_sys, NULL, "stack_d");
    i32 direction;
    LevPathCnx[0] = AIPAthFindPathCnx(world->ai_sys, NULL, "stack_a", "stack_b", &direction);
    LevPathCnx[1] = AIPAthFindPathCnx(world->ai_sys, NULL, "stack_b", "stack_c", &direction);
    LevPathCnx[2] = AIPAthFindPathCnx(world->ai_sys, NULL, "stack_c", "stack_d", &direction);
    LevPathCnx[3] = AIPAthFindPathCnx(world->ai_sys, NULL, "stack_d", "stack_e", &direction);
    mosEisleyB_nodesNeedUpdating = 1;

    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "junk_071");
    if (blowup != NULL) {
        blowup->field_0x125[0] = 1;
        blowup->draw_flags |= 0x10000;
    }
    blowup = GizmoBlowUp_FindByName(world, "junk_081");
    if (blowup != NULL) {
        blowup->field_0x125[0] = 1;
        blowup->draw_flags |= 0x10000;
    }
    blowup = GizmoBlowUp_FindByName(world, "evap_082");
    if (blowup != NULL) {
        blowup->field_0x124 = 1;
        blowup->draw_flags |= 0x18000;
    }

    if (NuSpecialExistsFn(&LevHSpecial[0])) {
        char *names[5] = {"arm_1_null1", "arm_1_null2", "arm_1_null3", "arm_1_null4", "arm_2_null1"};
        for (i32 i = 0; i < 5; ++i) {
            blowup = GizmoBlowUp_FindByName(world, names[i]);
            if (blowup != NULL && blowup->type != NULL) {
                blowup->field_0x124 = 1;
                blowup->override_special = &LevHSpecial[0];
                blowup->draw_flags |= 0xc00000;
                GizBlowup_InitSingleTerrain(blowup);
            }
        }
    }

    NuSpecialFind(world->current_gscn, &LevHSpecial[1], "heater_3_1_1", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[2], "heater_3_1_2", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[3], "heater_3_1_3", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[4], "heater_3_1_4", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[5], "heater_3_1_5", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[6], "heater_3_1_6", 1);

    blowup = GizmoBlowUp_FindByName(world, "null_pop1");
    if (blowup != NULL) {
        blowup->field_0x128 = 0.7f;
        blowup->field_0x124 = 1;
        blowup->override_special = &LevHSpecial[3];
        blowup->draw_flags |= 0xc10000;
        GizBlowup_InitSingleTerrain(blowup);
    }
    blowup = GizmoBlowUp_FindByName(world, "null_pop2");
    if (blowup != NULL) {
        blowup->field_0x128 = 0.7f;
        blowup->field_0x124 = 1;
        blowup->override_special = &LevHSpecial[4];
        blowup->draw_flags |= 0xc10000;
        GizBlowup_InitSingleTerrain(blowup);
    }
    blowup = GizmoBlowUp_FindByName(world, "null_pop3");
    if (blowup != NULL) {
        blowup->field_0x128 = 0.7f;
        blowup->field_0x124 = 1;
        blowup->override_special = &LevHSpecial[5];
        blowup->draw_flags |= 0xc10000;
        GizBlowup_InitSingleTerrain(blowup);
    }
    blowup = GizmoBlowUp_FindByName(world, "null_pop11");
    if (blowup != NULL) {
        blowup->field_0x128 = 0.7f;
        blowup->field_0x124 = 1;
        blowup->override_special = &LevHSpecial[6];
        blowup->draw_flags |= 0xc10000;
        GizBlowup_InitSingleTerrain(blowup);
    }
}

void MosEisleyB_Update(WORLDINFO_s *world) {
    GIZFORCE_s **forces = LevGizForce;
    GIZFORCE_s *first = forces[0];
    if (first == NULL)
        return;
    GIZFORCE_s *second = forces[1];
    if (second == NULL)
        return;
    GIZFORCE_s *third = forces[2];
    if (third == NULL)
        return;

    GIZFORCEGROUP_s *group = first->group;
    if (group != NULL && (group->field_0x24 & 2) != 0) {
        if (mosEisleyB_nodesNeedUpdating != 0)
            return;
        mosEisleyB_nodesNeedUpdating = 1;
        for (i32 i = 0; i < 4; ++i) {
            AIPATHCNX_s *connection = static_cast<AIPATHCNX_s *>(LevPathCnx[i]);
            if (connection != NULL) {
                connection->traversal_flags[0] &= ~0x80000000;
                connection->traversal_flags[1] &= ~0x80000000;
            }
        }

        AIPATHNODE_s *node0 = static_cast<AIPATHNODE_s *>(LevAIPathNode[0]);
        if (node0 == NULL)
            return;
        AIPATHNODE_s *node1 = static_cast<AIPATHNODE_s *>(LevAIPathNode[1]);
        if (node1 == NULL)
            return;
        AIPATHNODE_s *node2 = static_cast<AIPATHNODE_s *>(LevAIPathNode[2]);
        if (node2 == NULL)
            return;
        AIPATHNODE_s *node3 = static_cast<AIPATHNODE_s *>(LevAIPathNode[3]);
        if (node3 == NULL)
            return;

#define SET_STACK_NODE_POSITIONS(x1, z1, x2, z2, x3, z3)                                                               \
    do {                                                                                                               \
        node1->position.x = x1;                                                                                        \
        node1->position.z = z1;                                                                                        \
        node2->position.x = x2;                                                                                        \
        node2->position.z = z2;                                                                                        \
        node3->position.x = x3;                                                                                        \
        node3->position.z = z3;                                                                                        \
    } while (0)

        GIZFORCE_s *selected = group->forces[0];
        if (selected != first) {
            if (selected == second) {
                node0->position.x = 22.75f;
                node0->position.z = -15.12f;
                if (group->forces[1] == first)
                    SET_STACK_NODE_POSITIONS(23.27f, -14.93f, 23.72f, -14.78f, 23.32f, -14.61f);
                else
                    SET_STACK_NODE_POSITIONS(23.27f, -14.93f, 23.27f, -14.62f, 23.66f, -14.73f);
            } else if (selected == third) {
                node0->position.x = 22.79f;
                node0->position.z = -14.62f;
                if (group->forces[1] == first)
                    SET_STACK_NODE_POSITIONS(23.29f, -14.58f, 23.73f, -14.70f, 23.34f, -14.87f);
                else
                    SET_STACK_NODE_POSITIONS(23.29f, -14.58f, 23.27f, -14.85f, 23.63f, -14.76f);
            }
        } else {
            node0->position.x = 24.12f;
            node0->position.z = -14.69f;
            if (group->forces[1] == second)
                SET_STACK_NODE_POSITIONS(23.75f, -14.78f, 23.32f, -14.91f, 23.32f, -14.63f);
            else
                SET_STACK_NODE_POSITIONS(23.75f, -14.78f, 23.34f, -14.57f, 23.34f, -14.84f);
        }
#undef SET_STACK_NODE_POSITIONS

        if (world->ai_sys->path_sys != NULL && world->ai_sys->path_sys->active_path != NULL) {
            AIPathNodeUpdatePos(world->ai_sys, world->ai_sys->path_sys->active_path,
                                static_cast<AIPATHNODE_s *>(LevAIPathNode[0]));
            AIPathNodeUpdatePos(world->ai_sys, world->ai_sys->path_sys->active_path,
                                static_cast<AIPATHNODE_s *>(LevAIPathNode[1]));
            AIPathNodeUpdatePos(world->ai_sys, world->ai_sys->path_sys->active_path,
                                static_cast<AIPATHNODE_s *>(LevAIPathNode[2]));
            AIPathNodeUpdatePos(world->ai_sys, world->ai_sys->path_sys->active_path,
                                static_cast<AIPATHNODE_s *>(LevAIPathNode[3]));
        }
        return;
    }

    if (mosEisleyB_nodesNeedUpdating == 0)
        return;
    mosEisleyB_nodesNeedUpdating = 0;
    for (i32 i = 0; i < 4; ++i) {
        AIPATHCNX_s *connection = static_cast<AIPATHCNX_s *>(LevPathCnx[i]);
        if (connection != NULL) {
            connection->traversal_flags[0] |= 0x80000000;
            connection->traversal_flags[1] |= 0x80000000;
        }
    }
}

i32 MosEisleyC_PastBarrier(GameObject_s *object) {
    if (WORLD->current_level == MOSEISLEYC_LDATA)
        return object->apiobj.position.z > 5.85f;
    return 0;
}

void MosEisleyD_Init(WORLDINFO_s *world) {
    char name[32];
    char *special_names[6] = {"big_gate_1a", "big_gate_1b", "big_gate_2a", "big_gate_2b", "big_gate_3a", "big_gate_3b"};
    for (i32 i = 1; i <= 6; ++i) {
        sprintf(name, "NULL_door_pop%d", i);
        NuSpecialFind(world->current_gscn, &LevHSpecial[i - 1], special_names[i - 1], 1);
        GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, name);
        if (blowup != NULL) {
            blowup->field_0x128 = 0.7f;
            blowup->field_0x124 = 1;
            blowup->override_special = &LevHSpecial[i - 1];
            blowup->draw_flags |= 0xc00000;
            GizBlowup_InitSingleTerrain(blowup);
        }
    }

    for (i32 i = 1; i <= 8; ++i) {
        sprintf(name, "big_bin_lid%d", i);
        GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, name);
        if (blowup != NULL) {
            blowup->field_0x124 = 1;
            blowup->draw_flags |= 0x10000;
        }
    }
    for (i32 i = 1; i <= 8; ++i) {
        sprintf(name, "big_bin_lid_gr%d", i);
        GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, name);
        if (blowup != NULL) {
            blowup->field_0x124 = 1;
            blowup->draw_flags |= 0x10000;
        }
    }

    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "evap_041");
    if (blowup != NULL)
        blowup->field_0x124 = 1;
    blowup = GizmoBlowUp_FindByName(world, "evap_031");
    if (blowup != NULL) {
        blowup->field_0x128 = 0.5f;
        blowup->field_0x124 = 1;
    }
}

void MosEisleyD_AlwaysUpdate(WORLDINFO_s *world) {
    LevelStreaming_DoorOverride(world, MOSEISLEYE_LDATA, 7.5f, NULL);
}

void MosEisleyE_Init(WORLDINFO_s *world) {
    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, force_gizmotype_id, "force1");
    GIZFORCE_s *force = world->giz_force_sys->forces;
    for (i32 i = 0; i < world->giz_force_sys->count; i++, force++)
        force->state_flags |= 0x40;
}

void MosEisleyE_Reset(WORLDINFO_s *) {
    texanimbits &= ~2;
}

void MosEisleyE_Update(WORLDINFO_s *) {
    if ((texanimbits & 2) == 0 && LevGizmo[0] != NULL && LevGizmo[0]->object != NULL &&
        GizForce_Complete(static_cast<GIZFORCE_s *>(LevGizmo[0]->object)))
        texanimbits |= 2;
}

void TatooineD_Init(WORLDINFO_s *world) {
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "final_bpush", 1);
    LevAIPathNode[0] = AIPathFindNode(world->ai_sys, NULL, "box1_c");

    i32 direction;
    LevPathCnx[0] = AIPAthFindPathCnx(world->ai_sys, NULL, "box1_c", "box1_a", &direction);
    if (direction)
        LevPathCnxDir |= 1;

    LevPathCnx[1] = AIPAthFindPathCnx(world->ai_sys, NULL, "box1_c", "box1_b", &direction);
    if (direction)
        LevPathCnxDir |= 2;
}

void TatooineD_Update(WORLDINFO_s *world) {
    if (LevAIPathNode[0] != NULL && NuSpecialExistsFn(&LevHSpecial[0])) {
        NUVEC *position = NuSpecialGetDrawPos(&LevHSpecial[0]);
        if (position != NULL) {
            AIPATHNODE_s *node = static_cast<AIPATHNODE_s *>(LevAIPathNode[0]);
            node->position.x = position->x - 0.75f;
            node->position.z = position->z;
            if (world->ai_sys->path_sys != NULL && world->ai_sys->path_sys->active_path != NULL)
                AIPathNodeUpdatePos(world->ai_sys, world->ai_sys->path_sys->active_path, node);
        }
    }

    if (LevPathCnx[0] != NULL && LevPathCnx[1] != NULL) {
        AIPATHCNX_s *first = static_cast<AIPATHCNX_s *>(LevPathCnx[0]);
        AIPATHCNX_s *second = static_cast<AIPATHCNX_s *>(LevPathCnx[1]);
        u32 direction = LevPathCnxDir;
        if (second->traversal_flags[(direction >> 1) & 1] & 0x08000000)
            first->traversal_flags[direction & 1] |= 0x80000000;
        else
            first->traversal_flags[direction & 1] &= ~0x80000000;
    }
}

// ===========================================================================
// A New Hope (ANewHope_A)
// ===========================================================================

void ANewHopeA_Init(WORLDINFO_s *world) {
    GIZOBSTACLE_s *g;
    if ((g = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle6")) != NULL)
        g->field_a1_0xa1 |= 1;
    if ((g = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle7")) != NULL)
        g->field_a1_0xa1 |= 1;
    if ((g = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle8")) != NULL)
        g->field_a1_0xa1 |= 1;
    if ((g = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle9")) != NULL)
        g->field_a1_0xa1 |= 1;
    if ((g = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle10")) != NULL)
        g->field_a1_0xa1 |= 1;
    if ((g = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle11")) != NULL)
        g->field_a1_0xa1 |= 1;
    if ((g = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle12")) != NULL)
        g->field_a1_0xa1 |= 1;
    if ((g = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle13")) != NULL)
        g->field_a1_0xa1 |= 1;
}

// ===========================================================================
// Blockade runner (BlockadeRunner_B / BlockadeRunner_C / BlockadeRunner_D)
// ===========================================================================

void BlockadeRunnerB_Init(WORLDINFO_s *world) {
    LevBlowUp[0] = GizmoBlowUp_FindByName(world, "thermo_041");
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "arm_pop_1_2", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[1], "arm_pop_2_2", 1);

    if (NuSpecialExistsFn(&LevHSpecial[0])) {
        char *names[5] = {"arm_1_null1", "arm_1_null2", "arm_1_null3", "arm_1_null4", "arm_2_null1"};
        for (i32 i = 0; i < 5; ++i) {
            GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, names[i]);
            if (blowup != NULL && blowup->type != NULL) {
                blowup->field_0x124 = 1;
                blowup->override_special = &LevHSpecial[0];
                blowup->draw_flags |= 0xc00000;
                GizBlowup_InitSingleTerrain(blowup);
            }
        }
    }
    if (NuSpecialExistsFn(&LevHSpecial[1])) {
        char *names[5] = {"arm_1_null5", "arm_1_null6", "arm_1_null7", "arm_1_null8", "arm_2_null2"};
        for (i32 i = 0; i < 5; ++i) {
            GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, names[i]);
            if (blowup != NULL && blowup->type != NULL) {
                blowup->field_0x124 = 1;
                blowup->override_special = &LevHSpecial[1];
                blowup->draw_flags |= 0xc00000;
                GizBlowup_InitSingleTerrain(blowup);
            }
        }
    }

    GIZBUILDIT_s *buildit = GizBuildIt_Find(world, "buildit5");
    if (buildit != NULL)
        buildit->radius_scale = 2.0f;
    buildit = GizBuildIt_Find(world, "buildit6");
    if (buildit != NULL)
        buildit->radius_scale = 2.0f;
    buildit = GizBuildIt_Find(world, "buildit7");
    if (buildit != NULL)
        buildit->radius_scale = 2.0f;
}

void BlockadeRunnerB_Update(WORLDINFO_s *) {
    if (LevBlowUp[0] != NULL && InStory())
        LevBlowUp[0]->state_flags &= ~0x80;
}

void BlockadeRunnerC_Init(WORLDINFO_s *world) {
    LevGizObst[0] = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle8");
}

void BlockadeRunnerD_Reset(WORLDINFO_s *world) {
    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, "obstacle15");
    LevGizmo[1] = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, "obstacle14");
    LEV_FLAG_BR.obstacle15_active = 0;
    LEV_FLAG_BR.obstacle14_active = 0;
}

void BlockadeRunnerD_Update(WORLDINFO_s *world) {
    if (netclient != 0)
        return;

    for (i32 i = 0; i < 8; i++) {
        GameObject_s *game_object = LevGameObject[i];
        if (game_object == nullptr)
            continue;
        PART_s *part = LevGamePart[i];
        if (part == nullptr)
            continue;

        if ((part->active & 1) == 1) {
            game_object->field_0x1086 = 5;
            game_object->vehicle_orientation = part->transform;
            game_object->saved_position = game_object->apiobj.position = part->position;
        }

        if ((part->active & 1) == 0 || part->field_1c0 != &PartKill_EjectedCreature) {
            KillGameObject(game_object, 4, 0);
            LevGameObject[i] = nullptr;
        }
    }

    if (GizmoGetOutput(world->gizmo_sys, LevGizmo[0], 1, 1) != 0) {
        if (LEV_FLAG_BR.obstacle15_active == 0) {
            LEV_FLAG_BR.obstacle15_active = 1;
            BlockadeRunnerD_EjectCreature(0);
        }
    } else {
        LEV_FLAG_BR.obstacle15_active = 0;
    }

    if (GizmoGetOutput(world->gizmo_sys, LevGizmo[1], 1, 1) != 0) {
        if (LEV_FLAG_BR.obstacle14_active == 0) {
            LEV_FLAG_BR.obstacle14_active = 1;
            BlockadeRunnerD_EjectCreature(1);
        }
    } else {
        LEV_FLAG_BR.obstacle14_active = 0;
    }
}

// ===========================================================================
// Death Star rescue (DeathStarRescue_B / C)
// ===========================================================================

void DeathStarRescueB_Init(WORLDINFO_s *world) {
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "reactor_1", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[1], "reactor_2", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[2], "reactor_3", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[3], "reactor_4", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[4], "reactor_5", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[5], "reactor_6", 1);

    volatile u8 *flags = reinterpret_cast<volatile u8 *>(LevFlag);
    flags[5] = 0;
    flags[4] = 0;
    flags[3] = 0;
    flags[2] = 0;
    flags[1] = 0;
    flags[0] = 0;

    LevSfxId[0] = GetSfxId("env_tractorbeam_lp");
    LevSfxId[1] = GetSfxId("env_tractorbeam_off");

    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "dummy_exp2");
    if (blowup != NULL) {
        blowup->field_0x128 = 0.2f;
        blowup->field_0x124 = 1;
    }
    blowup = GizmoBlowUp_FindByName(world, "blowup_block_1");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_block_2");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_block_3");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_block_4");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_block_5");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_block_6");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
}

void DeathStarRescueB_Update(WORLDINFO_s *world) {
    u8 *flags = reinterpret_cast<u8 *>(LevFlag);
    for (i32 i = 0; i < 6; i++) {
        if (flags[i] == 0) {
            NUVEC *position = NuSpecialGetDrawPos(&LevHSpecial[i]);
            nuinstanim_s *animation = NuSpecialGetInstAnim(&LevHSpecial[i]);
            if (animation != NULL) {
                f32 frame = animation->ltime;
                f32 end = NuAnimEndFrameOld(world->current_gscn->instance_animation_data[animation->anim_ix]);
                PlaySfxByIdAndSetVolume(LevSfxId[0], position, 0.25f);
                if (frame >= end) {
                    PlaySfxByIdAndSetVolume(LevSfxId[1], NULL, 0.4f);
                    flags[i] = 1;
                }
            }
        }
    }
}

void DeathStarRescueB_AlwaysUpdate(WORLDINFO_s *world) {
    LevelStreaming_DoorOverride(world, DEATHSTARRESCUED_LDATA, 8.5f, NULL);
}

void DeathStarRescueC_Init(WORLDINFO_s *world) {
#define SETUP_RESCUE_PANEL(name)                                                                                       \
    do {                                                                                                               \
        GIZMOBLOWUP_s *panel = GizmoBlowUp_FindByName(world, name);                                                    \
        if (panel != NULL) {                                                                                           \
            panel->field_0x128 = 0.3f;                                                                                 \
            panel->field_0x124 = 1;                                                                                    \
        }                                                                                                              \
    } while (0)
    SETUP_RESCUE_PANEL("panel_11");
    SETUP_RESCUE_PANEL("panel_21");
    SETUP_RESCUE_PANEL("panel_31");
    SETUP_RESCUE_PANEL("panel_41");
    SETUP_RESCUE_PANEL("panel_51");
    SETUP_RESCUE_PANEL("panel_61");
    SETUP_RESCUE_PANEL("panel_71");
    SETUP_RESCUE_PANEL("panel_81");
#undef SETUP_RESCUE_PANEL
}

void DeathStarRescueC_AlwaysUpdate(WORLDINFO_s *world) {
    LevelStreaming_DoorOverride(world, DEATHSTARRESCUEE_LDATA, 8.5f, NULL);
}

void DeathStarEscapeA_Init(WORLDINFO_s *world) {
    GIZOBSTACLE_s *obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle8");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
    obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle9");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
    obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle10");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
    obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle11");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
    obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle12");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
    obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle1");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
}

void DeathStarEscapeB_Init(WORLDINFO_s *world) {
    deathstarescapeb_netpacket = SetLevelHack(4);
    LevAIMessage[0] = CheckGizAIMessage(gizaimessagesys, "WindowClean", NULL);
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "lift_up", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[1], "lift_down", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[2], "sponge", 1);

    fakeanimendframe[0] = 0.0f;
    nuinstanim_s *animation = NuSpecialGetInstAnim(&LevHSpecial[0]);
    if (animation != NULL) {
        nuanimdata_s *data = LevHSpecial[0].scene->instance_animation_data[animation->anim_ix];
        if (data != NULL)
            fakeanimendframe[0] += NuAnimEndFrameOld(data);
    }
    animation = NuSpecialGetInstAnim(&LevHSpecial[1]);
    if (animation != NULL) {
        nuanimdata_s *data = LevHSpecial[1].scene->instance_animation_data[animation->anim_ix];
        if (data != NULL)
            fakeanimendframe[0] += NuAnimEndFrameOld(data);
    }
}

__attribute__((force_align_arg_pointer)) void DeathStarEscapeB_Update(WORLDINFO_s *world) {
    nuanimdata_s *volatile data1 = NULL;
    nuanimdata_s *volatile data0 = NULL;
    if (netclient == 0)
        *static_cast<u8 *>(deathstarescapeb_netpacket) = static_cast<u8>(static_cast<i32>(LevAIMessage[0]->value));

    if (LevGameObject[0] == NULL && *static_cast<u8 *>(deathstarescapeb_netpacket) == 0)
        LevGameObject[0] = GetNamedGameObject(world->ai_sys, "WASHER_1");
    if (LevGameObject[1] == NULL && *static_cast<u8 *>(deathstarescapeb_netpacket) == 0)
        LevGameObject[1] = GetNamedGameObject(world->ai_sys, "WASHER_2");
    nuinstanim_s *animation0 = NuSpecialGetInstAnim(&LevHSpecial[0]);
    if (animation0 != NULL)
        data0 = LevHSpecial[0].scene->instance_animation_data[animation0->anim_ix];
    nuinstanim_s *animation1 = NuSpecialGetInstAnim(&LevHSpecial[1]);
    if (animation1 != NULL)
        data1 = LevHSpecial[1].scene->instance_animation_data[animation1->anim_ix];
    if (data0 == NULL || data1 == NULL)
        return;

    if (NuSpecialGetVisibilityFn(&LevHSpecial[0])) {
        fakeanimframe[0] = animation0->ltime;
    } else if (NuSpecialGetVisibilityFn(&LevHSpecial[1])) {
        fakeanimframe[0] = NuAnimEndFrameOld(data0) + animation1->ltime;
    } else {
        fakeanimframe[0] = 0.0f;
    }
}

void DeathStarEscapeB_AlwaysUpdate(WORLDINFO_s *) {
    NUVEC position;
    SOCKPOSITION socket_position;
    if (Players_AveragePos(&position, &socket_position) && socket_position.location.sock != -1) {
        if (DEATHSTARESCAPEA_LDATA != NULL && socket_position.location.sock < 6)
            other_level_override = DEATHSTARESCAPEA_LDATA->idx;
        else if (DEATHSTARESCAPEC_LDATA != NULL && socket_position.location.sock > 6)
            other_level_override = DEATHSTARESCAPEC_LDATA->idx;
    }
}

void DeathStarEscapeB_Draw(WORLDINFO_s *) {
    NUMTX_ALIGNED16 matrix;
    if (LevGameObject[0] != NULL && *static_cast<u8 *>(deathstarescapeb_netpacket) == 0) {
        matrix = LevGameObject[0]->joint_matrices[1];
        NuSpecialDrawAt(&LevHSpecial[2], &matrix);
    }
    if (LevGameObject[1] != NULL && *static_cast<u8 *>(deathstarescapeb_netpacket) == 0) {
        matrix = LevGameObject[1]->joint_matrices[1];
        NuSpecialDrawAt(&LevHSpecial[2], &matrix);
    }
}

void DeathStarEscapeA_Update(WORLDINFO_s *) {
    if (GameCam->sock_position.location.sock == 2) {
        NUVEC position = {0.0f, -5.5f, 0.0f};
        position.x = QRAND_FLOAT() * 2.0f + 36.5f;
        position.z = QRAND_FLOAT() * 2.0f + 8.5f;
        LevChatterSfx("Dianoga_Groan", &position);
    }
}

void DeathStarEscapeC_Init(WORLDINFO_s *world) {
    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "blowup_door_31");
    if (blowup != NULL)
        blowup->field_0x124 = 1;
    blowup = GizmoBlowUp_FindByName(world, "cup_built1");
    if (blowup != NULL)
        blowup->draw_flags |= 2;
    NuSpecialFind(world->current_gscn, &LevHSpecial[2], "door_push", 1);
    LevPathCnx[0] = AIPAthFindPathCnx(world->ai_sys, world->ai_sys->path_sys->active_path, "droid_rescue_a",
                                      "droid_rescue_b", &LevPathCnxDir);
}

void DeathStarEscapeC_Reset(WORLDINFO_s *) {
    LevAIMessage[0] = CheckGizAIMessage(gizaimessagesys, "RescuedDroids", NULL);
}

void DeathStarEscapeC_Update(WORLDINFO_s *) {
    if (LevPathCnx[0] != NULL) {
        NUVEC *position;
        if (NuSpecialGetVisibilityFn(&LevHSpecial[2]) && (position = NuSpecialGetDrawPos(&LevHSpecial[2])) != NULL &&
            position->x < 77.25f) {
            AIPATHCNX_s *connection = static_cast<AIPATHCNX_s *>(LevPathCnx[0]);
            connection->traversal_flags[0] &= ~0x80000000;
            connection->traversal_flags[1] &= ~0x80000000;
            if (LevAIMessage[0] != NULL)
                LevAIMessage[0]->value = 1.0f;
        } else {
            AIPATHCNX_s *connection = static_cast<AIPATHCNX_s *>(LevPathCnx[0]);
            connection->traversal_flags[0] |= 0x80000000;
            connection->traversal_flags[1] |= 0x80000000;
        }
    }
}

void DeathStarEscapeD_Update(WORLDINFO_s *) {
    NUVEC position = {0.0f, -5.5f, 0.0f};
    position.x = QRAND_FLOAT() * 4.0f + 32.5f;
    position.z = QRAND_FLOAT() * 4.0f + 12.5f;
    LevChatterSfx("Dianoga_Groan", &position);
}

// ===========================================================================
// Death Star battle (DeathStarBattle_C / D)
// ===========================================================================

void DeathStarBattleC_AlwaysUpdate(WORLDINFO_s *) {
    if (DEATHSTARBATTLEMIDTRO_LDATA != NULL)
        other_level_override = DEATHSTARBATTLEMIDTRO_LDATA->idx;
}

// ===========================================================================
// Death Star escape (DeathStarEscape_A / B / C / D)
// ===========================================================================

i32 DeathStarShieldDown() {
    if (LevGizmo[0] == NULL)
        LevGizmo[0] = GizmoFindByName(WORLD->gizmo_sys, blowup_gizmotype_id, "bigbang1");
    if (LevGizmo[0] == NULL || LevGizmo[0]->object == NULL)
        return 0;

    GIZMOBLOWUP_s *blowup = static_cast<GIZMOBLOWUP_s *>(LevGizmo[0]->object);
    if (blowup->output_flags & 1)
        return 1;
    if (__builtin_expect(static_cast<i8>(blowup->state_flags) < 0, 1)) {
        if (Player[0] != NULL && (WORLD->current_level != DEATHSTARBATTLED_LDATA ||
                                  !ObjInNarrowSock(Player[0], WORLD->sock_sys, WORLD->level_idx))) {
            TORPEDOPACKET_s *packet = Player[0]->torpedo;
            if (packet != NULL && (packet->count != 0 || (packet->field_0x1 & 2) != 0))
                return 1;
        }
        if (Player[1] != NULL && (WORLD->current_level != DEATHSTARBATTLED_LDATA ||
                                  !ObjInNarrowSock(Player[1], WORLD->sock_sys, WORLD->level_idx))) {
            TORPEDOPACKET_s *packet = Player[1]->torpedo;
            if (packet != NULL && (packet->count != 0 || (packet->field_0x1 & 2) != 0))
                return 1;
        }
    }
    return 0;
}

void DeathStarBattleDInit(WORLDINFO_s *) {
}

void DeathStarBattleDReset(WORLDINFO_s *) {
    memset(&trenchrun, 0, sizeof(trenchrun));
}

void DeathStarBattleDUpdate(WORLDINFO_s *world) {
    if (netclient == 0) {
        trenchrun.position.y = -10.5f;
        trenchrun.position.x = player->apiobj.collision_position.x;
        if (__builtin_expect(trenchrun.position.x < -500.0f, 1)) {
            trenchrun.position.y = trench_spawn_height - 10.5f;
            i32 &midtro = *reinterpret_cast<i32 *>(&trenchrun.reserved_0x18[0]);
            if (__builtin_expect(FreePlay == 0 && midtro == 0, 0)) {
                if (trenchrun.objects[0] != NULL) {
                    KillGameObject(trenchrun.objects[0], 4, 0);
                    trenchrun.objects[0] = NULL;
                }
                if (trenchrun.objects[1] != NULL) {
                    KillGameObject(trenchrun.objects[1], 4, 0);
                    trenchrun.objects[1] = NULL;
                }
                if (trenchrun.objects[2] != NULL) {
                    KillGameObject(trenchrun.objects[2], 4, 0);
                    trenchrun.objects[2] = NULL;
                }
                NewCutScene(NULL, world->cutscene_sys, "deathstarbattle_midtro_ingame", 1);
                midtro = 1;
            }
        }

        trenchrun.position.z = player->apiobj.collision_position.z;
        f32 lower = trenchrun.objects[0] != NULL ? 62.0f : 60.0f;
        f32 upper = trenchrun.objects[2] != NULL ? 70.0f : 72.0f;
        if (trenchrun.position.z < lower)
            trenchrun.position.z = lower;
        else if (trenchrun.position.z > upper)
            trenchrun.position.z = upper;

        if (__builtin_expect(aicreature_sets_alive[0] == 0, 1)) {
            f32 &timer = *reinterpret_cast<f32 *>(&trenchrun.reserved_0x18[4]);
            timer -= FRAMETIME;
            if (timer <= 0.0f) {
                timer = 5.0f;
#define SPAWN_TRENCH_SHIP(slot, model, ox, oy, oz)                                                                     \
    do {                                                                                                               \
        NUVEC_ALIGNED16 spawn = {player->apiobj.collision_position.x + trench_spawn_distance,                          \
                                 player->apiobj.collision_position.y + trench_spawn_height,                            \
                                 player->apiobj.collision_position.z};                                                 \
        NUVEC_ALIGNED16 offset = {ox, oy, oz};                                                                         \
        NuVecAdd(&spawn, &spawn, &offset);                                                                             \
        GameObject_s *ship = AddDynamicCreature(model, &spawn, 0xc000, "TrenchBaddie", &player->ai.path_info, NULL, 0, \
                                                NULL, NULL, 0, 1);                                                     \
        if (ship != NULL) {                                                                                            \
            ship->field_0xf04 |= 4;                                                                                    \
            ship->field_0xeb4 = TrenchKilledCallback;                                                                  \
            ship->move_override = TrenchMove;                                                                          \
            ship->movement_spline_offset = offset;                                                                     \
            trenchrun.objects[slot] = ship;                                                                            \
        }                                                                                                              \
    } while (0)
                SPAWN_TRENCH_SHIP(0, id_TIEFIGHTER, 0.0f, 0.0f, 2.0f);
                i32 model =
                    *reinterpret_cast<i32 *>(&trenchrun.reserved_0x18[0]) != 0 ? id_TIEFIGHTER : id_TIEFIGHTERDARTH;
                SPAWN_TRENCH_SHIP(1, model, 0.0f, 1.0f, 0.0f);
                SPAWN_TRENCH_SHIP(2, id_TIEFIGHTER, 0.0f, 0.0f, -2.0f);
#undef SPAWN_TRENCH_SHIP
            }
        }
    }

    volatile u8 *flags = reinterpret_cast<volatile u8 *>(LevFlag);
    if (flags[4] != 0)
        return;
    if (NuSpecialGetVisibilityFn(&LevHSpecial[0])) {
        PlaySfx("ffield", NuSpecialGetDrawPos(&LevHSpecial[0]));
    } else {
        PlaySfx("ffieldoff", NuSpecialGetDrawPos(&LevHSpecial[0]));
        flags[4] = 1;
    }
}

void DeathStarBattleDDraw(WORLDINFO_s *) {
}

