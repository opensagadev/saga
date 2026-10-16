#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/cutscenes/cutscene_defrag.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/nusound/nusound.h"

#include <cfloat>
#include <string.h>

extern "C" i32 NuGCutLocatorIsVisble(NUGCUTLOCATOR_s *, f32, nuanimtime_s *, f32 *, f32 *);
extern "C" f32 *NuGCutLocatorCalcMtx(NUGCUTLOCATOR_s *, f32, NUMTX *, nuanimtime_s *);
void instNuGCutSceneEndButNotSystems(instNUGCUTSCENE_s *instance);
void instNuGCutSceneResetCamLock(instNUGCUTSCENE_s *);

extern "C" {
    CutSceneCleanUpEntry *DefragCutSceneList;
    CutSceneCleanUpEntry *DefragCutSceneListBase;
    void *DefragCutSceneBaseMem;
    void *DefragCutSceneEndMem;
    void *DefragInstBaseMem;
    void *DefragInstEndMem;
    instNUGCUTSCENE_s *(*DefragGetInstFn)(void *);
    void *(*DefragCreateInstFn)(void *, NUGCUTSCENE_s *, VARIPTR *);
    void (*DefragInstDestroyedFn)(void *);
    i32 DefragCutSceneListSize;
    extern debinftype **debtab;
    extern NUGCUTLOCATORFNENTRY_s *locatorfns;
    extern f32 timeincrement;
    extern i32 processdeb;
    extern f32 glyntestha;

    void NuAnimData2CalcTime(nuanimdata2_s *, f32, nuanimtime_s *);
    void NuMtxPreTranslate(NUMTX *, NUVEC *);
    void NuMtxMul(NUMTX *, NUMTX *, NUMTX *);
    i32 NuStrICmp(const char *, const char *);
    void instNuGCutSceneStart(instNUGCUTSCENE_s *);

    i32 AddScaledVariableShotDebrisEffect2(i32, NUVEC *, i32, f32, NUMTX *, NUMTX *, f32);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisPopulateInstance(i32, f32);
    void DebrisEmitterPos(i32, f32, f32, f32);
    void DebrisOrientationMtx(i32, NUMTX *);
    void DebrisPosOrientationMtx(i32, NUMTX *);
    void AddScaledFiniteShotDebrisEffect(i32 *, i32, NUVEC *, NUVEC *, NUVEC *, i32, f32);
    void DebrisEmitterOrientationMtx(i32, NUMTX *);
    void DebFreeInstantly(i32 *);
}

extern "C" {

    i32 CheckStreamFileID(void) {
        return 0;
    }

    void DisplayCutSceneMemory(void) {
        if (DefragCutSceneEndMem == NULL || DefragCutSceneListSize == 0 || DefragCutSceneBaseMem == NULL) {
            return;
        }

#define CUT_MEMORY_VERTEX(x, y, normal_colour, bright_colour)                                                          \
    do {                                                                                                               \
        if (g_NuPrim_NeedsOverbrightening == 0) {                                                                      \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = (normal_colour);                                                    \
        } else {                                                                                                       \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = (bright_colour);                                                    \
        }                                                                                                              \
        NuPrim2DAddXYZ((x), (y), 0.0f);                                                                                \
    } while (0)

        ++NuPrimCSPos;
        NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_PS2);
        NuPrim2DBegin(4, 5, NULL);

        if (DefragInstBaseMem != NULL) {
            const u32 cut_span = static_cast<u32>(reinterpret_cast<usize>(DefragCutSceneEndMem) -
                                                  reinterpret_cast<usize>(DefragCutSceneBaseMem));
            const f32 cut_scale = 580.0f / static_cast<f32>(cut_span);
            CUT_MEMORY_VERTEX(30.0f, 206.0f, 0x40000040u, 0x40000080u);
            CUT_MEMORY_VERTEX(610.0f, 208.0f, 0x40000040u, 0x40000080u);

            CutSceneCleanUpEntry *const list_end = DefragCutSceneListBase + DefragCutSceneListSize;
            for (CutSceneCleanUpEntry *entry = DefragCutSceneListBase; entry < list_end; ++entry) {
                if ((entry->flags & 4) == 0) {
                    continue;
                }
                instNUGCUTSCENE_s *instance = DefragGetInstFn(entry->handle);
                NUGCUTSCENE_s *scene = instance->cutscene;
                const u32 start =
                    static_cast<u32>(reinterpret_cast<usize>(scene) - reinterpret_cast<usize>(DefragCutSceneBaseMem));
                const u32 end = start + scene->loaded_size;
                const f32 start_x = static_cast<f32>(start) * cut_scale + 30.0f;
                const f32 end_x = static_cast<f32>(end) * cut_scale + 30.0f;
                CUT_MEMORY_VERTEX(start_x, 206.0f, 0x40004040u, 0x40008080u);
                CUT_MEMORY_VERTEX(end_x, 208.0f, 0x40004040u, 0x40008080u);
            }

            const u32 inst_span = static_cast<u32>(reinterpret_cast<usize>(DefragInstEndMem) -
                                                   reinterpret_cast<usize>(DefragInstBaseMem));
            const f32 inst_scale = 580.0f / static_cast<f32>(inst_span);
            CUT_MEMORY_VERTEX(30.0f, 209.0f, 0x40000030u, 0x40000060u);
            CUT_MEMORY_VERTEX(610.0f, 211.0f, 0x40000030u, 0x40000060u);

            for (CutSceneCleanUpEntry *entry = DefragCutSceneListBase; entry < list_end; ++entry) {
                if ((entry->flags & 4) == 0) {
                    continue;
                }
                instNUGCUTSCENE_s *instance = DefragGetInstFn(entry->handle);
                const u32 start =
                    static_cast<u32>(reinterpret_cast<usize>(instance) - reinterpret_cast<usize>(DefragInstBaseMem));
                const u32 end = start + instance->allocation_size;
                const f32 start_x = static_cast<f32>(start) * inst_scale + 30.0f;
                const f32 end_x = static_cast<f32>(end) * inst_scale + 30.0f;
                CUT_MEMORY_VERTEX(start_x, 209.0f, 0x40400040u, 0x40800080u);
                CUT_MEMORY_VERTEX(end_x, 211.0f, 0x40400040u, 0x40800080u);
            }
        } else {
            const u32 cut_span = static_cast<u32>(reinterpret_cast<usize>(DefragCutSceneEndMem) -
                                                  reinterpret_cast<usize>(DefragCutSceneBaseMem));
            const f32 cut_scale = 580.0f / static_cast<f32>(cut_span);
            CUT_MEMORY_VERTEX(30.0f, 206.0f, 0x40000040u, 0x40000080u);
            CUT_MEMORY_VERTEX(610.0f, 210.0f, 0x40000040u, 0x40000080u);

            CutSceneCleanUpEntry *const list_end = DefragCutSceneListBase + DefragCutSceneListSize;
            for (CutSceneCleanUpEntry *entry = DefragCutSceneListBase; entry < list_end; ++entry) {
                if ((entry->flags & 4) == 0) {
                    continue;
                }
                instNUGCUTSCENE_s *instance = DefragGetInstFn(entry->handle);
                NUGCUTSCENE_s *scene = instance->cutscene;
                const u32 scene_start =
                    static_cast<u32>(reinterpret_cast<usize>(scene) - reinterpret_cast<usize>(DefragCutSceneBaseMem));
                const u32 scene_end = scene_start + scene->loaded_size;
                const f32 scene_start_x = static_cast<f32>(scene_start) * cut_scale + 30.0f;
                const f32 scene_end_x = static_cast<f32>(scene_end) * cut_scale + 30.0f;
                CUT_MEMORY_VERTEX(scene_start_x, 206.0f, 0x40004040u, 0x40008080u);
                CUT_MEMORY_VERTEX(scene_end_x, 210.0f, 0x40004040u, 0x40008080u);

                const u32 inst_start = static_cast<u32>(reinterpret_cast<usize>(instance) -
                                                        reinterpret_cast<usize>(DefragCutSceneBaseMem));
                const u32 inst_end = inst_start + instance->allocation_size;
                const f32 inst_start_x = static_cast<f32>(inst_start) * cut_scale + 30.0f;
                const f32 inst_end_x = static_cast<f32>(inst_end) * cut_scale + 30.0f;
                CUT_MEMORY_VERTEX(inst_start_x, 206.0f, 0x40400040u, 0x40800080u);
                CUT_MEMORY_VERTEX(inst_end_x, 210.0f, 0x40400040u, 0x40800080u);
            }
        }

        NuPrim2DEnd();
        NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[--NuPrimCSPos]);

#undef CUT_MEMORY_VERTEX
    }

    void PauseGameCut(void) {
        if (NOMUSIC != 0 || NOSOUND != 0 || NUSOUND_STREAM_3 == -1) {
            return;
        }

        MusicPlayback *music = &Music;
        if (music->state != MUSIC_PLAYBACK_DUAL_STREAM && music->state != MUSIC_PLAYBACK_DUAL_STREAM_PENDING) {
            return;
        }
        NuSound3CancelCheckStereo();
        NuSound3PauseStereoStream(1 - music->primary_stream);
    }

    void RestoreGameCut(void) {
        MusicPlayback *music = &Music;

        if (NOMUSIC != 0 && NOSOUND != 0) {
            return;
        }
        if (NUSOUND_STREAM_3 == -1) {
            return;
        }
        if (music->state != MUSIC_PLAYBACK_DUAL_STREAM && music->state != MUSIC_PLAYBACK_DUAL_STREAM_PENDING) {
            return;
        }
        NuSound3ResumeStereoStream(1 - music->primary_stream);
    }

    u8 ForceScenePlayBack;

    void SetForceScenePlayBack(i32 enabled) {
        ForceScenePlayBack = static_cast<u8>(enabled);
    }

    f32 instCutSceneTimeElapsed(instNUGCUTSCENE_s *instance) {
        if (instance == NULL || instance->cutscene == NULL) {
            return 0.0f;
        }
        f32 rate = instance->rate;
        f32 accumulated = instance->accumulated_stream_duration;
        f32 frame = instance->current_frame;
        if (rate == 0.0f) {
            return 0.0f;
        }
        f32 elapsed = accumulated + frame - 1.0f;
        if (elapsed == 0.0f) {
            return 0.0f;
        }
        return elapsed / rate;
    }

    i32 instNuGCutSceneAddCamTgt(instNUGCUTSCENE_s *instance, NUVEC *target, f32 start_frame, f32 duration,
                                 i8 target_index) {
        instNUGCUTSCENECAMERA_s *camera = instance->camera_instance;
        if (camera == NULL || camera->target_count >= camera->target_capacity) {
            return 0;
        }
        instNUGCUTCAMTGT_s *entry = &camera->targets[camera->target_count++];
        entry->target = target;
        entry->start_frame = start_frame;
        entry->duration = duration;
        entry->target_index = target_index;
        return 1;
    }

    void instNuGCutSceneAddCleanUpItem(void *handle, i32 enabled) {
        instNUGCUTSCENE_s *instance = DefragGetInstFn(handle);
        CutSceneCleanUpEntry *entry = DefragCutSceneList++;
        entry->handle = handle;
        entry->cutscene = instance->cutscene;
        entry->accumulated_duration = instance->accumulated_stream_duration;
        entry->flags = enabled ? 7 : 0;
        ++DefragCutSceneListSize;
    }

    void instNuGCutSceneCalculateAverageCentre(instNUGCUTSCENE_s *instance, NUMTX *matrix, NUVEC *out) {
        out->x = 0.0f;
        out->y = 0.0f;
        out->z = 0.0f;
        if (instance == NULL || instance->cutscene == NULL || instance->cutscene->rigid_system == NULL ||
            instance->rigid_instance->rigids == NULL) {
            return;
        }
        f32 count = 0.0f;
        for (i32 i = 0; i < instance->cutscene->rigid_system->count; ++i) {
            instNUGCUTRIGID_s *rigid = &instance->rigid_instance->rigids[i];
            if (NuSpecialExistsFn(rigid) != 0) {
                NUVEC *position = NuSpecialGetPos(rigid);
                out->x += position->x;
                out->y += position->y;
                out->z += position->z;
                count += 1.0f;
            }
        }
        if (count > 0.0f) {
            f32 inverse = 1.0f / count;
            out->x *= inverse;
            out->y *= inverse;
            out->z *= inverse;
        }
        if (matrix != NULL) {
            NuVecMtxTransform(&instance->transformed_bounds_center, &instance->transformed_bounds_center, matrix);
        }
    }

    void instNuGCutSceneChain(instNUGCUTSCENE_s *instance, instNUGCUTSCENE_s *next) {
        instance->chained_instance = next;
    }

    i32 instNuGCutSceneCharGetStartMtx(instNUGCUTSCENE_s *instance, const char *name, NUMTX *out) {
        NUGCUTCHARSYS_s *system = instance->cutscene->character_system;
        for (i32 i = 0; i < system->character_count; ++i) {
            NUGCUTCHAR_s *character = &system->characters[i];
            if (NuStrICmp(name, character->name) == 0) {
                *out = character->base_matrix;
                return 1;
            }
        }
        return 0;
    }

    void instNuGCutSceneCreateCamTgtArray(instNUGCUTSCENE_s *instance, i32 count, VARIPTR *buf) {
        if (count == 0 || instance->camera_instance == NULL) {
            return;
        }
        instNUGCUTSCENECAMERA_s *camera = instance->camera_instance;
        camera->target_capacity = static_cast<u8>(count);
        buf->addr = ALIGN(buf->addr, 0x10);
        camera->targets = reinterpret_cast<instNUGCUTCAMTGT_s *>(buf->void_ptr);
        buf->void_ptr = camera->targets + count;
        memset(camera->targets, 0, count * sizeof(instNUGCUTCAMTGT_s));
    }

    void instNuGCutSceneDisable(instNUGCUTSCENE_s *instance) {
        instance->flags_89 |= 0x8;
    }

    void instNuGCutSceneEnable(instNUGCUTSCENE_s *instance) {
        instance->flags_89 &= ~0x8;
    }

    i32 instNuGCutSceneIsFinished(instNUGCUTSCENE_s *instance) {
        return instance->finished;
    }

    void instNuGCutScenePlay(instNUGCUTSCENE_s *instance, i32 forward) {
        if ((instance->flags_88 & 2) != 0) {
            if (forward != 0) {
                if (instance->rate < 0.0f) {
                    instance->rate = -instance->rate;
                }
            } else if (instance->rate > 0.0f) {
                instance->rate = -instance->rate;
            }
            return;
        }
        if ((instance->flags_89 & 0x10) != 0) {
            if (forward != 0) {
                return;
            }
            instance->flags_89 &= ~0x10;
            instance->flags_88 |= 2;
            instance->current_frame = instance->cutscene->duration - 1.0f;
            if (instance->rate > 0.0f) {
                instance->rate = -instance->rate;
            }
            return;
        }
        if (forward != 0) {
            if (instance->rate < 0.0f) {
                instance->rate = -instance->rate;
            }
            instNuGCutSceneStart(instance);
        }
    }

    void instNuGCutSceneResetCleanUp(void *list_storage, void *cut_base, void *cut_end, void *inst_base, void *inst_end,
                                     instNUGCUTSCENE_s *(*get_inst)(void *),
                                     void *(*create_inst)(void *, NUGCUTSCENE_s *, VARIPTR *),
                                     void (*inst_destroyed)(void *)) {
        DefragCutSceneList = reinterpret_cast<CutSceneCleanUpEntry *>(ALIGN(reinterpret_cast<usize>(list_storage), 4));
        DefragCutSceneListBase = DefragCutSceneList;
        DefragCutSceneBaseMem = cut_base;
        DefragCutSceneEndMem = cut_end;
        DefragInstBaseMem = inst_base;
        DefragInstEndMem = inst_end;
        DefragGetInstFn = get_inst;
        DefragCreateInstFn = create_inst;
        DefragInstDestroyedFn = inst_destroyed;
        DefragCutSceneListSize = 0;
    }

    void instNuGCutSceneRotateY(instNUGCUTSCENE_s *instance, NUANG angle) {
        instance->flags_88 |= 0x80;
        NuMtxRotateY(&instance->matrix, angle);
        NUMTX *matrix = &instance->matrix;
        NUVEC *bounds = static_cast<NUVEC *>(instance->cutscene->bounds);
        if (bounds != NULL) {
            instance->transformed_bounds_center.x = (bounds[1].x + bounds[0].x) * 0.5f;
            instance->transformed_bounds_center.y = (bounds[1].y + bounds[0].y) * 0.5f;
            instance->transformed_bounds_center.z = (bounds[1].z + bounds[0].z) * 0.5f;
        } else {
            instance->transformed_bounds_center.x = 0.0f;
            instance->transformed_bounds_center.y = 0.0f;
            instance->transformed_bounds_center.z = 0.0f;
        }
        NuVecMtxTransform(&instance->transformed_bounds_center, &instance->transformed_bounds_center, matrix);
    }

    void instNuGCutSceneSetEndCallback(instNUGCUTSCENE_s *instance, void (*callback)(instNUGCUTSCENE_s *)) {
        instance->end_callback = callback;
    }

    void instNuGCutSceneSetMtx(instNUGCUTSCENE_s *instance, NUMTX *matrix) {
        instance->flags_88 |= 0x80;
        instance->matrix = *matrix;

        NUMTX *instance_matrix = &instance->matrix;
        NUVEC *bounds = static_cast<NUVEC *>(instance->cutscene->bounds);
        if (bounds != NULL) {
            instance->transformed_bounds_center.x = (bounds[1].x + bounds[0].x) * 0.5f;
            instance->transformed_bounds_center.y = (bounds[1].y + bounds[0].y) * 0.5f;
            instance->transformed_bounds_center.z = (bounds[1].z + bounds[0].z) * 0.5f;
        } else {
            instance->transformed_bounds_center.x = 0.0f;
            instance->transformed_bounds_center.y = 0.0f;
            instance->transformed_bounds_center.z = 0.0f;
        }
        NuVecMtxTransform(&instance->transformed_bounds_center, &instance->transformed_bounds_center, instance_matrix);
    }

    void instNuGCutSceneSetPos(instNUGCUTSCENE_s *instance, NUVEC *pos) {
        instance->flags_88 |= 0x80;
        NuMtxSetTranslation(&instance->matrix, pos);
        NUMTX *matrix = &instance->matrix;
        NUVEC *bounds = static_cast<NUVEC *>(instance->cutscene->bounds);
        if (bounds != NULL) {
            instance->transformed_bounds_center.x = (bounds[1].x + bounds[0].x) * 0.5f;
            instance->transformed_bounds_center.y = (bounds[1].y + bounds[0].y) * 0.5f;
            instance->transformed_bounds_center.z = (bounds[1].z + bounds[0].z) * 0.5f;
        } else {
            instance->transformed_bounds_center.x = 0.0f;
            instance->transformed_bounds_center.y = 0.0f;
            instance->transformed_bounds_center.z = 0.0f;
        }
        NuVecMtxTransform(&instance->transformed_bounds_center, &instance->transformed_bounds_center, matrix);
    }

    void instNuGCutSceneSetRepeat(instNUGCUTSCENE_s *instance, i32 repeat) {
        instance->repeat_count = repeat <= 31 ? repeat : 31;
    }

    void instNuGCutSceneStop(instNUGCUTSCENE_s *instance) {
        instance->flags_88 &= ~6;
        instance->flags_89 &= ~0x10;
        if ((instance->flags_8a & 4) == 0) {
            instNUGCUTCHARSYS_s *runtime = instance->character_instance;
            if (runtime != NULL) {
                NUGCUTCHARSYS_s *system = instance->cutscene->character_system;
                for (i32 i = 0; i < system->character_count; ++i) {
                    instNUGCUTCHAR_s *character = &runtime->characters[i];
                    if (character->character != NULL && NuCutSceneCharacterRelease != NULL)
                        NuCutSceneCharacterRelease(character, &system->characters[i]);
                }
            }
        } else {
            instNUGCUTCHARSYS_s *runtime = instance->character_instance;
            if (runtime != NULL) {
                NUGCUTCHARSYS_s *system = instance->cutscene->character_system;
                for (i32 i = 0; i < system->character_count; ++i) {
                    instNUGCUTCHAR_s *character = &runtime->characters[i];
                    if (character->character != NULL && NuCutSceneCharacterRelease != NULL)
                        NuCutSceneCharacterRelease(character, &system->characters[i]);
                }
            }
        }
        instNuGCutSceneResetCamLock(instance);
    }

    f32 instNuGCutSceneTimeLeft(instNUGCUTSCENE_s *instance) {
        if (instance == NULL || instance->cutscene == NULL) {
            return 0.0f;
        }

        NUGCUTSCENE_s *cutscene = instance->cutscene;
        f32 frames_left;
        if (cutscene->version > 9) {
            frames_left = static_cast<f32>(static_cast<i32>(cutscene->total_stream_frames)) - instance->current_frame;
        } else {
            if (cutscene->version > 1 && (cutscene->flags & 1) != 0 && cutscene->last_stream != 0) {
                return FLT_MAX;
            }
            frames_left = cutscene->duration - 1.0f - instance->current_frame;
        }

        f32 time_left = 0.0f;
        if (frames_left > 0.0f) {
            if (instance->rate == 0.0f) {
                return FLT_MAX;
            }
            time_left = NuFdiv(frames_left, instance->rate);
        }
        return time_left;
    }

    f32 instNuGCutSceneTotalTime(instNUGCUTSCENE_s *instance) {
        if (instance == NULL || instance->cutscene == NULL || instance->cutscene->version <= 9) {
            return 0.0f;
        }

        f32 total_frames = instance->cutscene_copy->total_stream_frames;
        if (instance->rate != 0.0f) {
            return NuFdiv(total_frames, instance->rate);
        }
        return 0.0f;
    }

    void instNuGCutSceneTranslate(instNUGCUTSCENE_s *instance, NUVEC *translation) {
        if ((instance->flags_88 & 0x80) == 0) {
            instance->flags_88 |= 0x80;
            NuMtxSetIdentity(&instance->matrix);
        }
        NuMtxTranslate(&instance->matrix, translation);
        NUMTX *matrix = &instance->matrix;
        NUVEC *bounds = static_cast<NUVEC *>(instance->cutscene->bounds);
        if (bounds != NULL) {
            instance->transformed_bounds_center.x = (bounds[1].x + bounds[0].x) * 0.5f;
            instance->transformed_bounds_center.y = (bounds[1].y + bounds[0].y) * 0.5f;
            instance->transformed_bounds_center.z = (bounds[1].z + bounds[0].z) * 0.5f;
        } else {
            instance->transformed_bounds_center.x = 0.0f;
            instance->transformed_bounds_center.y = 0.0f;
            instance->transformed_bounds_center.z = 0.0f;
        }
        NuVecMtxTransform(&instance->transformed_bounds_center, &instance->transformed_bounds_center, matrix);
    }

    void instNuGCutSceneWaitAtEnd(instNUGCUTSCENE_s *instance, u8 enabled) {
        instance->wait_at_end = enabled;
    }

    void instNuGCutSoundStream(void) {
    }

} // extern "C"

void instGetLookAtLocatorInfo(instNUGCUTSCENE_s *instance, instNUGCUTLOOKAT_s *state);

// Steers a cutscene character's eyes towards a look-at locator and blinks its eyelids.
// Only version-3 locator animations carry the look and blink channels.
static void instNuGCutLookAtLocatorUpdate(NUGCUTLOCATORSYS_s *system, instNUGCUTLOCATOR_s *inst_locator,
                                          NUGCUTLOCATOR_s *locator, f32 frame, NUMTX *parent_mtx,
                                          instNUGCUTSCENE_s *instance) {
    if (*reinterpret_cast<u32 *>(locator->animation) - 0x414e4934U > 1) {
        return;
    }
    // The locator's instance slot holds the look-at record pointer.
    GCutLookAtState *state = reinterpret_cast<GCutLookAtState *>(static_cast<uintptr_t>(inst_locator->effect_handle));
    instGetLookAtLocatorInfo(instance, reinterpret_cast<instNUGCUTLOOKAT_s *>(state));
    if (state->character_index == 0 || state->scene == NULL) {
        return;
    }
    NUGSCN *scene = state->scene;

    f32 *values;
    f32 position_x, position_y, position_z;
    {
        NUMTX matrix;
        values = NuGCutLocatorCalcMtx(locator, frame, &matrix, NULL);
        if ((locator->flags & 4) != 0) {
            NuMtxPreTranslate(&matrix, &locator->pivot);
        }
        if (parent_mtx != NULL) {
            NuMtxMul(&matrix, &matrix, parent_mtx);
        }
        position_x = matrix.m30;
        position_y = matrix.m31;
        position_z = matrix.m32;
    }
    const i32 look = static_cast<i32>(values[3] < 0.0f ? values[3] - 0.5f : values[3] + 0.5f);
    const i32 blink = static_cast<i32>(values[4] < 0.0f ? values[4] - 0.5f : values[4] + 0.5f);

    i32 closed = 0;
    if (state->secondary_material_index != 0) {
        f32 look_x = 0.0f;
        f32 look_y = 0.0f;
        if (look != 0) {
            const i32 index = state->character_index - 1;
            NUGCUTCHAR_s *character = &instance->cutscene->character_system->characters[index];
            instNUGCUTCHAR_s *inst_character = &instance->character_instance->characters[index];
            NUMTX character_matrix;
            i32 visible;
            u32 animation_index;
            f32 animation_rate;
            f32 blend_time;
            f32 animation_start_frame;
            i32 layer_mask;
            NuGCutCharAnimProcess(character, inst_character->animation_frame_a, &character_matrix, &visible,
                                  &animation_index, &animation_rate, &blend_time, &animation_start_frame, &layer_mask);
            if (visible == 0) {
                return;
            }
            NUMTX_ALIGNED16 joints[256];
            NuHGobjEvalAnim2(reinterpret_cast<nuhgobj_s *>(scene),
                             reinterpret_cast<ani3_animheader_s *>(character->face_animation),
                             inst_character->animation_frame_a, 0, NULL, joints);
            NUMTX joint_matrix;
            EvaluateJointOrientationMtx(scene, state->joint_override_index - 1, &joint_matrix);
            NUMTX head;
            NuMtxMulVU0(&head, &joint_matrix, &joints[state->joint_override_index - 1]);
            NuMtxMulVU0(&head, &head, &character_matrix);
            NUVEC4 direction = {head.m30 - position_x, head.m31 - position_y, head.m32 - position_z, 0.0f};
            NUMTX inverse;
            NuMtxTranspose(&inverse, &head);
            NUVEC4 local;
            NuVec4MtxRotateVU0(&local, &direction, &inverse);
            NuVecNorm(reinterpret_cast<NUVEC *>(&local), reinterpret_cast<NUVEC *>(&local));
            if (local.z > 0.1f) {
                look_x = local.x * 0.5f;
                look_y = local.y * 0.5f;
            }
            if (look_x > 0.24f) {
                look_x = 0.24f;
            } else if (look_x < -0.24f) {
                look_x = -0.24f;
            }
        }
        numtl_s *material = scene->mtls[state->secondary_material_index - 1];
        // The vertical range narrows as the eyes turn sideways.
        const f32 limit = (NU_COS_LUT(NuFabs(look_x) / 0.24f * 8192.0f) - 0.7f) * 0.18f / 0.3f;
        look_y = look_y < limit ? look_y : limit;
        look_y = -limit > look_y ? -limit : look_y;
        material->delta_u += (look_x - material->delta_u) * 0.125f;
        material->delta_v += (-look_y - material->delta_v) * 0.125f;
        closed = material->delta_v < -0.15f;
    }

    if (state->texture_animation_index == 0) {
        return;
    }
    if (blink != 0) {
        state->lid_frame += instance->elapsed * 30.0f;
        if (state->lid_frame > 3.0f) {
            state->lid_frame = 3.0f;
        }
    } else {
        state->lid_frame -= instance->elapsed * 10.0f;
        if (state->lid_frame < 0.0f) {
            state->lid_frame = 0.0f;
        }
    }
    i32 lid = static_cast<i32>(state->lid_frame + 0.5f);
    if (lid < closed) {
        lid = closed;
    }
    numtl_s *material = scene->mtls[state->material_index - 1];
    nutexanim_s *animation = &static_cast<nutexanim_s *>(scene->texture_anims)[state->texture_animation_index - 1];
    if (lid < animation->texture_count) {
        material->tex_id = static_cast<i16>(animation->texture_ids[lid]);
        NuMtlUpdate(material);
    }
}

extern "C" {
    // Aligned four-float row used for retail's movaps matrix copies.
    typedef f32 LocatorMtxRow __attribute__((vector_size(16)));

    void instNuGCutLocatorUpdate(instNUGCUTSCENE_s *instance, NUGCUTLOCATORSYS_s *system,
                                 instNUGCUTLOCATOR_s *inst_locator, NUGCUTLOCATOR_s *locator, f32 frame,
                                 NUMTX *parent_mtx, i32 paused) {
        NUMTX matrix;
        NUGCUTLOCATORTYPE_s *type = &system->types[locator->type_index];
        const u8 type_flags = type->flags;

        if ((type_flags & 1) != 0) {
            if (paused != 0) {
                return;
            }
            const u16 effect_index = type->function_index;
            if (effect_index == 0xffff || debtab[effect_index] == NULL) {
                return;
            }
            nuanimtime_s time;
            f32 scale;
            f32 rate;
            if (locator->animation != NULL) {
                NuAnimData2CalcTime(locator->animation, frame, &time);
            }
            i32 visible = NuGCutLocatorIsVisble(locator, frame, &time, &scale, &rate);
            if (visible != 0 && scale < 0.0f && (locator->flags & 0x10) == 0) {
                scale = static_cast<f32>(debtab[effect_index]->frequency) * -scale;
            }
            if (visible != 0 && scale > 0.1f) {
                if ((locator->flags & 0x10) != 0 && inst_locator->effect_handle != 0) {
                    return;
                }

                NuGCutLocatorCalcMtx(locator, frame, &matrix, &time);
                if ((locator->flags & 4) != 0) {
                    NuMtxPreTranslate(&matrix, &locator->pivot);
                }
                if (parent_mtx != NULL) {
                    NuMtxMul(&matrix, &matrix, parent_mtx);
                }
                if ((locator->flags & 0x10) == 0) {
                    if ((locator->flags & 0x20) == 0) {
                        AddScaledVariableShotDebrisEffect2(effect_index, reinterpret_cast<NUVEC *>(&matrix.m30),
                                                           static_cast<i32>(scale), timeincrement, &matrix, 0, rate);
                    } else {
                        if (inst_locator->effect_handle == -1) {
                            AddDebrisEffect(&inst_locator->effect_handle, effect_index, matrix.m30, matrix.m31,
                                            matrix.m32);
                            DebrisPopulateInstance(inst_locator->effect_handle, 0.0f);
                        }
                        if (inst_locator->effect_handle >= 0) {
                            if (processdeb == 1) {
                                DebrisEmitterPos(inst_locator->effect_handle, matrix.m30, matrix.m31, matrix.m32);
                            } else if (processdeb == 2) {
                                DebrisOrientationMtx(inst_locator->effect_handle, &matrix);
                            } else {
                                DebrisPosOrientationMtx(inst_locator->effect_handle, &matrix);
                            }
                        }
                        if (glyntestha > 0.0f) {
                            NuRndrAxisArrowsMtx(&matrix, glyntestha, 0);
                        }
                    }
                } else {
                    i32 finite_handle = -1;
                    AddScaledFiniteShotDebrisEffect(&finite_handle, effect_index,
                                                    reinterpret_cast<NUVEC *>(&matrix.m30), 0, 0, 1, rate);
                    DebrisEmitterOrientationMtx(finite_handle, &matrix);
                    inst_locator->effect_handle = 1;
                }
                return;
            }
            if ((locator->flags & 0x20) != 0) {
                if (inst_locator->effect_handle >= 0) {
                    DebFreeInstantly(&inst_locator->effect_handle);
                }
            } else {
                inst_locator->effect_handle = 0;
            }
            inst_locator->field_00 = 0;
            return;
        }

        if ((type_flags & 2) != 0) {
            const u16 function_index = type->function_index;
            if (function_index != 0xffff) {
                NUGCUTLOCATORFN fn = locatorfns[function_index].function;
                if (fn != NULL) {
                    fn(instance, system, inst_locator, locator, frame, parent_mtx, paused);
                }
            }
        } else if ((type_flags & 0x10) == 0) {
            if ((type_flags & 4) != 0) {
                if (NuCutSceneSFXUpdate != NULL && instance->rate != 0.0f) {
                    NuCutSceneSFXUpdate(system, inst_locator, locator, frame, parent_mtx, paused);
                }
            } else if ((type_flags & 8) != 0 && paused == 0) {
                instNuGCutLookAtLocatorUpdate(system, inst_locator, locator, frame, parent_mtx, instance);
            }
        } else {
            if (paused != 0) {
                return;
            }
            const u16 effect_index = type->function_index;
            f32 opacity = 0.0f;
            if (TriggerLocatorVfxFn == NULL || ReleaseLocatorVfxFn == NULL) {
                return;
            }
            nuanimtime_s time;
            if (locator->animation != NULL) {
                NuAnimData2CalcTime(locator->animation, frame, &time);
            }
            if (NuGCutLocatorIsVisble(locator, frame, &time, &opacity, NULL) == 0 || !(opacity > 0.1f)) {
                if (inst_locator->effect_handle >= 0) {
                    ReleaseLocatorVfxFn(inst_locator->effect_handle);
                }
                inst_locator->effect_handle = -1;
            } else {
                // Retail realigns the frame and copies the locator matrix into the VFX matrix with movaps.
                VuMtx vfx_matrix __attribute__((aligned(16)));
                NUMTX locator_mtx __attribute__((aligned(16)));
                NuGCutLocatorCalcMtx(locator, frame, &locator_mtx, &time);
                if ((locator->flags & 4) != 0) {
                    NuMtxPreTranslate(&locator_mtx, &locator->pivot);
                }
                if (parent_mtx != NULL) {
                    NuMtxMul(&locator_mtx, &locator_mtx, parent_mtx);
                }
                const LocatorMtxRow *rows = reinterpret_cast<const LocatorMtxRow *>(&locator_mtx);
                LocatorMtxRow *vfx_rows = reinterpret_cast<LocatorMtxRow *>(&vfx_matrix.matrix);
                vfx_rows[0] = rows[0];
                vfx_rows[1] = rows[1];
                vfx_rows[2] = rows[2];
                vfx_rows[3] = rows[3];
                if (inst_locator->effect_handle >= 0 && UpdateLocatorVfxFn != NULL) {
                    UpdateLocatorVfxFn(inst_locator->effect_handle, &vfx_matrix);
                } else {
                    inst_locator->effect_handle = TriggerLocatorVfxFn(effect_index, &vfx_matrix);
                }
            }
        }
    }

} // extern "C"
