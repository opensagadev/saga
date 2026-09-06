#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/world/world.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/nusound/nusound.h"

#include <string.h>

extern i32 ACTIVECUTCOUNT;
extern "C" i32 CUTDRAWWORLD;
extern "C" i32 Paused;
extern i32 CutSceneWaiting;
extern i32 CUTCAMONLY;
extern i32 cut_waiting_for_new_level;
extern i32 waiting_for_level;
extern i32 newlevel_resumecutaudio;
extern CUTSYS *CS_cutsys;
extern FadeSystem FadeSys;
CUTSCENESYS *CutSceneSys;

static void CutScene_DrawCharacter(instNUGCUTSCENE_s *, NUGCUTSCENE_s *, instNUGCUTCHAR_s *, NUGCUTCHAR_s *, f32, i32);
static void CutScene_EvalCharacter(instNUGCUTSCENE_s *, NUGCUTSCENE_s *, instNUGCUTCHAR_s *, NUGCUTCHAR_s *, f32);
static void CutScene_FindCharacters(NUGCUTSCENE_s *);
static void CutScene_ResetCharacters(instNUGCUTSCENE_s *);
static void CutScene_RigidPostRender(NUGCUTRIGID_s *, instNUGCUTRIGID_s *, NUMTX *);
static void CutScene_CreateCharacterInstance(NUGCUTCHAR_s *, instNUGCUTCHAR_s *, variptr_u *);

extern "C" {
    void instNuGCutSceneEnd(instNUGCUTSCENE_s *instance);
    i32 instNuGCutSceneIsFinished(instNUGCUTSCENE_s *instance);
    void instNuGCutScenePause(instNUGCUTSCENE_s *, u8);
    void instNuGCutSceneReset(instNUGCUTSCENE_s *);
    void instNuGCutSceneStart(instNUGCUTSCENE_s *);
    void instNuGCutSceneStop(instNUGCUTSCENE_s *);
    void instNuGCutSceneDestroy(instNUGCUTSCENE_s *);
    void NuGCutSceneDestroy(NUGCUTSCENE_s *);
    void NuGCutSceneSysRender(f32);
    void NuGCutSceneSysUpdate(i32, i32, f32);
    void NuGCutSceneSysInit(NUGCUTLOCATORFNENTRY_s *);
    extern NUGCUTLOCATORFNENTRY_s cutscene_locatorfns[];
}

void SetLevelLights(void *, f32);
void NewLevelFromMenu(LEVELDATA_s *, i32, i32, i32);
void FindAndSetLights(NUVEC *, f32, void *);
void SetZeroLights(void);
void EnableShadowMapRendering(i32);
void ResetShadowMapRendering(void);
i32 CutScenePlayer_Active(void);
void CutScenePlayer_SetObjects(CUTINFO *);
void AddPartDebris(PARTDEBSYS_s *, i32, nuvec_s *);
extern "C" void DebrisSetRenderGroup(i32);
extern AREADATA_s *BONUS_GUNSHIP_ADATA;
extern AREADATA_s *GUNSHIP_ADATA;
extern AREADATA_s *BATTLEOVERCORUSCANT_ADATA;

extern "C" {
    extern i16 id_ANAKINSPODGREEN;
    extern i16 id_ANAKINSNEWPOD;
    extern i16 id_ANAKINSNEWPODGREEN;
    extern i16 id_REPUBLICGUNSHIP_GREEN;
    extern i16 id_NEW_REPUBLIC_GUNSHIP_GREEN;
    extern i16 id_JEDISTARFIGHTERYELLOWEP3;
}

i32 CutInstEndCount;
static instNUGCUTSCENE_s *CutInstEnd[4];
static i32 CutInstEndStop;
static i32 cutaudiopaused;
static CUTINFO *g_lastCutInfo;
static f32 g_lastCutsceneTime;
static f32 g_accumCutsceneTime;
i32 NewCutInfoCount;
static CUTINFO *NewCutInfo[8];
i32 CUTNOFOG;

extern "C" {
    void PauseGameAudio(void);
    void PauseGameCut(void);
    void SetLinkedCutSceneMusic(void *context, i32 state);
    void PlaySfxById(i32 sfx_id, nuvec_s *position);
}

static void CutScene_Start(WORLDINFO_s *world, CUTINFO *cut, i32) {
    instNUGCUTSCENE_s *instance = static_cast<instNUGCUTSCENE_s *>(cut->instance);
    if (cut->music_handle != -1) {
        g_lastCutsceneTime = 0.0f;
        g_accumCutsceneTime = 0.0f;
        g_lastCutInfo = NULL;
    }
    instNuGCutSceneReset(instance);
    if (CutScene_StartFn != NULL) {
        CutScene_StartFn(cut);
    }
    if (CutScenePlayer_Active() != 0) {
        CutScenePlayer_SetObjects(cut);
    }
    instance->rate = cut->frames_per_second * FRAMETIME;
    instNuGCutSceneStart(instance);
    if ((cut->flags & 0x200) != 0) {
        instance->flags_88 |= 8;
    } else {
        instance->flags_88 &= ~8U;
    }
    cut->previous_frame = 0.0f;
    cut->field_58 = 0;

    if (cut->music_handle != -1) {
        music_man.SelectTrackByHandle(TRACK_CLASS_CUTSCENE, cut->music_handle);
        i32 status = music_man.PlayTrack(TRACK_CLASS_CUTSCENE, 0);
        instance->rate = 0.0f;
        if ((cut->flags & 1) != 0 && status == 1) {
            CutSceneWaiting = 1;
            PauseGameAudio();
            cutaudiopaused = 1;
        }
    } else {
        PauseGameAudio();
        SetLinkedCutSceneMusic(instance, cut->linked_audio == 0 ? MUSIC_PLAYBACK_DUAL_STREAM
                                                                : MUSIC_PLAYBACK_DUAL_STREAM_PENDING);
        instance->rate = cut->frames_per_second * FRAMETIME;
        if ((cut->flags & 1) != 0) {
            CutSceneWaiting = 0;
            cutaudiopaused = 0;
        }
    }

    if ((cut->flags & 1) == 0) {
        return;
    }
    CutSceneWaiting = 0;
    cutaudiopaused = 0;
    for (i32 i = 0; i < world->cutscene_sys->count; ++i) {
        CUTINFO *other = world->cutscene_sys->cuts[i];
        if (other == NULL || other->instance == NULL) {
            continue;
        }
        instNUGCUTSCENE_s *other_instance = static_cast<instNUGCUTSCENE_s *>(other->instance);
        if ((other_instance->flags_88 & 2) != 0) {
            other_instance->rate = 0.0f;
            instNuGCutScenePause(other_instance, 1);
        }
    }
    CUTSTOPGAME = 1;
    CUTDRAWWORLD = cut->flags & 2;
    CutStopInfo = cut;
    DebrisSetRenderGroup(cut->debris_render_group);
    ACTIVECUTCOUNT = 1;
    CutBorderScale = 1.0f;
}

void CutScenes_End() {
    if (CutScenePlayer_Active() != 0 && NewLData != NULL && NewLData != HUB_LDATA) {
        return;
    }
    i32 i = 0;
    if (CutInstEndCount > 0) {
        do {
            instNuGCutSceneEnd(CutInstEnd[i]);
            if (i == CutInstEndStop) {
                CUTSTOPGAME = 0;
                CutStopInfo = NULL;
                GameCam_Reset(GameCam);
            }
            ++i;
        } while (CutInstEndCount > i);
    }
}

void CutScenes_Draw(WORLDINFO_s *world) {
    if (world->cutscene_sys != NULL && ACTIVECUTCOUNT > 0) {
        SetLevelLights(world->rtl_set, 1.0f);
        NuGCutSceneSysRender(static_cast<f32>(Paused));

        if (CUTDRAWWORLD != 0 && world->current_gscn != NULL) {
            SetLevelLights(world->rtl_set, 1.0f);
            NuGScnRndr3(world->current_gscn);
            CUTINFO *cut = static_cast<CUTINFO *>(CutStopInfo);
            if (world->gizmo_sys != NULL && cut != NULL && (cut->flags & 0x8000) != 0) {
                GizmoSysDraw(world->gizmo_sys, world, FRAMETIME);
            }
        }
    }
}

void CutScenes_Stop(CUTSYS *system) {
    if (system == NULL || system->count <= 0) {
        return;
    }
    for (i32 i = 0; i < system->count; ++i) {
        CUTINFO *cut = system->cuts[i];
        if (cut->instance != NULL) {
            instNuGCutSceneStop(static_cast<instNUGCUTSCENE_s *>(cut->instance));
        }
    }
}

void CutScenes_Reset(WORLDINFO_s *world) {
    ACTIVECUTCOUNT = 0;
    CUTSTOPGAME = 0;
    CUTDRAWWORLD = 0;
    cutaudiopaused = 0;
    CutStopInfo = NULL;
    CutSceneWaiting = 0;
    if (world == NULL || world->cutscene_sys == NULL) {
        return;
    }

    CUTSYS *system = world->cutscene_sys;
    for (i32 i = 0; i < system->count; ++i) {
        CUTINFO *cut = system->cuts[i];
        if (cut == NULL || cut->instance == NULL) {
            continue;
        }
        instNUGCUTSCENE_s *instance = reinterpret_cast<instNUGCUTSCENE_s *>(cut->instance);
        instNuGCutSceneReset(instance);
        if (reset_restart != 0 && (cut->flags & 0x1000) != 0) {
            if ((cut->flags & 0x200) != 0) {
                instance->flags_88 |= 8;
            } else {
                instance->flags_88 &= ~8U;
            }
            instNuGCutSceneStart(instance);
            reset_restart = 0;
            break;
        }
    }
}

void CutScenes_Start(WORLDINFO_s *world) {
    for (i32 i = 0; i < NewCutInfoCount; ++i) {
        CUTINFO *cut = NewCutInfo[i];
        if (cut == NULL || cut->instance == NULL) {
            continue;
        }
        i32 cutscene_index = -1;
        if (world->cutscene_sys != NULL) {
            for (i32 j = 0; j < world->cutscene_sys->count; ++j) {
                if (world->cutscene_sys->cuts[j] == cut) {
                    cutscene_index = j;
                }
            }
        }
        if (world->level_progress != NULL && cutscene_index != -1) {
            const u32 bit = 1U << (cutscene_index & 0x1f);
            if ((cut->end_flags & 1) != 0 && (world->level_progress->played_cutscene_mask & bit) != 0) {
                continue;
            }
            world->level_progress->played_cutscene_mask |= bit;
        }
        CutScene_Start(world, cut, cutscene_index);
    }
    NewCutInfoCount = 0;
}

void CutScenes_Update(WORLDINFO_s *world, i32 paused) {
    i32 active_before[32] = {};
    i32 stop_index = -1;
    CutInstEnd[0] = NULL;
    CutInstEndStop = -1;
    CutInstEndCount = 0;
    ACTIVECUTCOUNT = 0;
    CUTSTOPGAME = 0;
    CUTDRAWWORLD = 0;
    if (world == NULL || world->cutscene_sys == NULL) {
        return;
    }
    CUTSYS *system = world->cutscene_sys;
    for (i32 i = 0; i < system->count; ++i) {
        CUTINFO *cut = system->cuts[i];
        if (cut == NULL || cut->instance == NULL) {
            continue;
        }
        instNUGCUTSCENE_s *instance = reinterpret_cast<instNUGCUTSCENE_s *>(cut->instance);
        if ((instance->flags_88 & 2) == 0) {
            continue;
        }
        active_before[i] = 1;
        cut->previous_frame = instance->current_frame;
        cut->field_58 += FRAMETIME;
        if (cutaudiopaused != 0 && cut->field_58 >= 3.0f) {
            cutaudiopaused = 0;
        }

        instance->rate = cut->frames_per_second * FRAMETIME;
        ++ACTIVECUTCOUNT;
        if ((cut->flags & 1) != 0) {
            CUTSTOPGAME = 1;
            CutStopInfo = cut;
            stop_index = i;
        }
        if ((cut->flags & 2) != 0) {
            CUTDRAWWORLD = 1;
        }
    }

    for (i32 i = 0; i < system->count; ++i) {
        if (active_before[i] == 0) {
            continue;
        }
        CUTINFO *cut = system->cuts[i];
        instNUGCUTSCENE_s *instance = static_cast<instNUGCUTSCENE_s *>(cut->instance);
        if (stop_index != -1 && i != stop_index) {
            instance->rate = 0.0f;
            instNuGCutScenePause(instance, 1);
            continue;
        }

        if (i == stop_index && NOSOUND == 0 && NOMUSIC == 0) {
            i32 music_status = music_man.GetStatus(TRACK_CLASS_CUTSCENE, NULL);
            if (instance->current_frame == 0.0f && cut->music_handle != -1 && music_status != 4) {
                music_man.PlayTrack(TRACK_CLASS_CUTSCENE, 0);
            } else if (music_status == 4) {
                if (g_lastCutInfo != cut) {
                    g_accumCutsceneTime += g_lastCutsceneTime;
                    g_lastCutInfo = cut;
                    NUGCUTSCENE_s *scene = static_cast<NUGCUTSCENE_s *>(cut->scene);
                    g_lastCutsceneTime = scene->duration / cut->frames_per_second;
                }
                f32 audio_frame =
                    (music_man.GetPlaybackTime(TRACK_CLASS_CUTSCENE) - g_accumCutsceneTime) * cut->frames_per_second;
                if (audio_frame < 0.0f) {
                    audio_frame = 0.0f;
                }
                f32 rate = audio_frame - (instance->current_frame - 1.0f);
                instance->rate = rate > 0.0f ? rate : 0.0f;
            }
            if (CutSceneWaiting != 0) {
                CutSceneWaiting = 0;
                cutaudiopaused = 0;
            }
        }
        instNuGCutScenePause(instance, 0);
    }
    NuGCutSceneSysUpdate(paused, 0, 1.0f);

    if (paused == 0) {
        for (i32 i = 0; i < system->count; ++i) {
            if (active_before[i] == 0) {
                continue;
            }
            CUTINFO *cut = system->cuts[i];
            instNUGCUTSCENE_s *instance = static_cast<instNUGCUTSCENE_s *>(cut->instance);
            if (instance->rate <= 0.0f) {
                continue;
            }
            for (CUTSCENESFX &sfx : cut->sfx) {
                if (sfx.id != -1 && sfx.frame > cut->previous_frame && sfx.frame <= instance->current_frame) {
                    PlaySfxById(sfx.id, (sfx.flags & 1) != 0 ? &sfx.position : NULL);
                }
            }
        }
    }
    for (i32 i = 0; i < system->count; ++i) {
        CUTINFO *cut = system->cuts[i];
        if (cut == NULL || cut->instance == NULL || active_before[i] == 0) {
            continue;
        }
        instNUGCUTSCENE_s *instance = reinterpret_cast<instNUGCUTSCENE_s *>(cut->instance);
        if (i != stop_index && instNuGCutSceneIsFinished(instance) != 0 && CutInstEndCount < 4) {
            if (CutScene_StoppedFn != NULL) {
                CutScene_StoppedFn(cut);
            }
            if (cut->skip_level != -1) {
                NewLData = &LDataList[cut->skip_level];
            }
            if ((cut->flags & 1) != 0) {
                CutInstEndStop = CutInstEndCount;
                FADETYPE fade_type;
                fade_type.type = FADE_TYPE_STILL_WIPE;
                FadeSys.SetFade(fade_type, 0);
            }
            CutInstEnd[CutInstEndCount++] = instance;
            instance->flags_88 |= 2;
        }
    }
    if (stop_index == -1) {
        return;
    }

    CUTINFO *stop_cut = system->cuts[stop_index];
    instNUGCUTSCENE_s *stop_instance = static_cast<instNUGCUTSCENE_s *>(stop_cut->instance);
    if (instNuGCutSceneIsFinished(stop_instance) == 0) {
        return;
    }
    FADETYPE fade_type;
    fade_type.type = FADE_TYPE_STILL_WIPE;
    FadeSys.SetFade(fade_type, 0);
    if (CutScene_StoppedFn != NULL) {
        CutScene_StoppedFn(stop_cut);
    }

    if (stop_cut->skip_level == -1) {
        CUTINFO *next = NewCutScene(NULL, system, stop_cut->next_cutscene, 0);
        if (CutScenePlayer_Active() != 0 && next == NULL) {
            NewLevelFromMenu(HUB_LDATA, -1, -1, 1);
            hub_from_cutsceneplayer = 1;
        }
    } else {
        NewLData = &LDataList[stop_cut->skip_level];
        if (NewLData == HUB_LDATA && CutScenePlayer_Active() != 0) {
            NewLevelFromMenu(HUB_LDATA, -1, -1, 1);
            hub_from_cutsceneplayer = 1;
        }
    }

    if (waiting_for_level != -1) {
        cut_waiting_for_new_level = 1;
        PauseGameCut();
        music_man.PauseTrack(TRACK_CLASS_CUTSCENE);
        newlevel_resumecutaudio = 1;
    }
    CUTNOFOG = 0;

    if (CutInstEndCount < 4) {
        CutInstEndStop = CutInstEndCount;
        CutInstEnd[CutInstEndCount++] = stop_instance;
        stop_instance->flags_88 |= 2;
    }
}

void CutScene_FindInst(CUTSYS *, char *) {
}

void CutScenes_Destroy(CUTSYS *system) {
    if (system == NULL || system->count <= 0) {
        return;
    }
    for (i32 i = 0; i < system->count; ++i) {
        CUTINFO *cut = system->cuts[i];
        if (cut->instance != NULL) {
            instNuGCutSceneDestroy(static_cast<instNUGCUTSCENE_s *>(cut->instance));
            cut->instance = NULL;
        }
    }
    for (i32 i = 0; i < system->count; ++i) {
        CUTINFO *cut = system->cuts[i];
        if (cut->scene != NULL) {
            NuGCutSceneDestroy(static_cast<NUGCUTSCENE_s *>(cut->scene));
            cut->scene = NULL;
        }
    }
}

i32 CutScene_HasPlayed(CUTINFO *cut) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    CUTSYS *cutscene_system = world->cutscene_sys;
    if (cut == NULL || cutscene_system == NULL || cutscene_system->count <= 0) {
        return 0;
    }

    i32 cutscene_index = -1;
    for (i32 index = 0; index < cutscene_system->count; ++index) {
        if (cutscene_system->cuts[index] == cut) {
            cutscene_index = index;
        }
    }
    i32 has_played = 0;
    if (cutscene_index != -1) {
        world = WorldInfo_CurrentlyActive();
        has_played = (world->level_progress->played_cutscene_mask & (1u << (cutscene_index & 0x1f))) != 0;
    }
    return has_played;
}

void CutScene_SnapToEnd(CUTINFO *cut) {
    if (cut != NULL) {
        instNuGCutSceneEnd(static_cast<instNUGCUTSCENE_s *>(cut->instance));
    }
}

void CutScene_StartAudio() {
}

i32 CutScene_IsSkippable(CUTINFO *cut) {
    return FadeSys.fade == 0.0f && cut != NULL && (CutStopInfo != cut || CutSceneWaiting == 0 || cutaudiopaused == 0);
}

void CutScene_StartFn_LSW(CUTINFO *) {
}

void CutScenes_InitSystem(CUTSCENESYS *system) {
    CutSceneSys = system;
    NuGCutSceneSysInit(cutscene_locatorfns);
    NuSetCutSceneCharacterRenderFn(CutScene_DrawCharacter);
    NuSetCutSceneFindCharactersFn(CutScene_FindCharacters);
    NuSetCutSceneCharacterCreateDataFn(CutScene_CreateCharacterInstance);
    NuSetCutSceneResetCharactersFn(CutScene_ResetCharacters);
    NuSetCutSceneCharacterEvalFn(CutScene_EvalCharacter);
    NuSetCutSceneRigidPostRenderFn(CutScene_RigidPostRender);
}

static void CutScene_DrawCharacter(instNUGCUTSCENE_s *cutscene_instance, NUGCUTSCENE_s *, instNUGCUTCHAR_s *instance,
                                   NUGCUTCHAR_s *character, f32 frame, i32 paused) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if (world->cutscene_sys == NULL) {
        return;
    }

    // Characters found by CutScene_FindCharacters carry an ordinary loaded
    // APICHARACTERMODEL directly in their instance data (flags bit 1).
    CHARACTERMODEL_s *model = static_cast<CHARACTERMODEL_s *>(instance->character_model);
    if ((character->flags & 2) == 0 || model == NULL || model->hierarchy == NULL) {
        return;
    }
    const i32 character_id = model->model_id;
    CHARACTERDATA *character_data = &apicharsys->char_data[character_id];
    GAMECHARACTERDATA *game_data = static_cast<GAMECHARACTERDATA *>(character_data->field11_0x24);
    if (game_data == NULL || game_data->make_layer_list == NULL) {
        return;
    }

    NUMTX world_matrix;
    i32 visible = 0;
    u32 animation_index = 0;
    f32 animation_rate = 1.0f;
    f32 blend_time = 0.0f;
    f32 animation_start_frame = 0.0f;
    i32 layer_mask = -1;
    NuGCutCharAnimProcess(character, frame, &world_matrix, &visible, &animation_index, &animation_rate, &blend_time,
                          &animation_start_frame, &layer_mask);
    if (paused != 0) {
        animation_rate = 0.0f;
    }
    if (layer_mask == -1) {
        layer_mask = static_cast<i32>(reinterpret_cast<u8 *>(CutSceneSys)[7] == 0 ? game_data->layer_mask_special
                                                                                  : game_data->layer_mask);
    }
    if (static_cast<i8>(cutscene_instance->flags_88) < 0) {
        NuMtxMul(&world_matrix, &world_matrix, &cutscene_instance->matrix);
    }
    if (visible == 0) {
        return;
    }

    i16 render_indices[32];
    const i32 render_count = game_data->make_layer_list(model, render_indices, static_cast<u32>(layer_mask));
    if (render_count < 1) {
        return;
    }

    if (instance->field_15 != static_cast<u8>(animation_index)) {
        const u8 requested_animation = static_cast<u8>(animation_index);
        if (blend_time <= 0.0f || instance->field_15 == 0xff) {
            instance->field_16 = requested_animation;
            instance->animation_frame_a = animation_start_frame <= 1.0f ? 1.0f : animation_start_frame;
        } else {
            if ((instance->field_14 & 1) == 0) {
                if (instance->field_15 == 0) {
                    instance->animation_frame_a = frame;
                    instance->field_16 = requested_animation;
                } else {
                    instance->field_16 = instance->field_15;
                    animation_index = instance->field_15;
                }
            } else {
                instance->field_16 = instance->field_17;
                instance->animation_frame_a = instance->animation_frame_b;
                animation_index = instance->field_17;
            }
            if (animation_start_frame <= 1.0f) {
                animation_start_frame = 1.0f;
            }
            instance->field_17 = requested_animation;
            instance->animation_frame_b = animation_start_frame;
            if (requested_animation != static_cast<u8>(animation_index)) {
                instance->field_04 = 0;
                instance->field_14 |= 1;
            }
        }
        instance->field_15 = requested_animation;
    }

    bool blending = (instance->field_14 & 1) != 0;
    u32 animation_a_index = instance->field_16;
    if (blending) {
        if (blend_time <= 0.0f) {
            instance->field_04 = 0;
            animation_a_index = instance->field_17;
            instance->field_14 ^= 1;
            instance->field_16 = instance->field_17;
            instance->animation_frame_a = instance->animation_frame_b;
            blending = false;
        } else {
            instance->field_04 += (1.0f / blend_time) * FRAMETIME * 60.0f;
            if (instance->field_04 >= 1.0f) {
                instance->field_04 = 0;
                animation_a_index = instance->field_17;
                instance->field_14 ^= 1;
                instance->field_16 = instance->field_17;
                instance->animation_frame_a = instance->animation_frame_b;
                blending = false;
            }
        }
    }

    ani3_animheader_s *animation_a;
    if (animation_a_index == 0) {
        instance->animation_frame_a = frame;
        animation_a = reinterpret_cast<ani3_animheader_s *>(character->face_animation);
    } else {
        animation_a = static_cast<ani3_animheader_s *>(model->model_data_b[animation_a_index - 1]);
    }

    NUMTX joint_matrices[256];
    if (!blending) {
        if (animation_a == NULL) {
            NuHGobjEval(model->hierarchy, 0, NULL, joint_matrices);
        } else {
            NuHGobjEvalAnim2(model->hierarchy, animation_a, instance->animation_frame_a, 0, NULL, joint_matrices);
        }
    } else {
        ani3_animheader_s *animation_b;
        if (instance->field_17 == 0) {
            instance->animation_frame_b = frame;
            animation_b = reinterpret_cast<ani3_animheader_s *>(character->face_animation);
        } else {
            animation_b = static_cast<ani3_animheader_s *>(model->model_data_b[instance->field_17 - 1]);
        }
        if (animation_a != NULL && animation_b != NULL) {
            NuHGobjEvalAnimBlend2(model->hierarchy, animation_a, instance->animation_frame_a, animation_b,
                                  instance->animation_frame_b, instance->field_04, 0, NULL, joint_matrices);
        } else if (animation_b != NULL) {
            NuHGobjEvalAnim2(model->hierarchy, animation_b, instance->animation_frame_b, 0, NULL, joint_matrices);
        } else if (animation_a != NULL) {
            NuHGobjEvalAnim2(model->hierarchy, animation_a, instance->animation_frame_a, 0, NULL, joint_matrices);
        } else {
            NuHGobjEval(model->hierarchy, 0, NULL, joint_matrices);
        }
    }

    NUVEC locator_positions[16];
    NUMTX locator_matrices[16];
    StoreLocatorCoordinates(model, &world_matrix, joint_matrices, locator_positions, locator_matrices);

    if (animation_a != NULL && animation_a_index != 0 && animation_a_index != 0xff &&
        (CutStopInfo == NULL || CutSceneWaiting == 0)) {
        const f32 end_frame = AnimEndFrame(model, animation_a_index - 1);
        instance->animation_frame_a += FRAMETIME * 60.0f * animation_rate;
        if (instance->animation_frame_a > end_frame) {
            CHARACTERANIM_s *animation_info =
                static_cast<CHARACTERANIM_s *>(model->model_data_a[animation_a_index - 1]);
            instance->animation_frame_a = animation_info != NULL && (animation_info->flags & 2) != 0
                                              ? instance->animation_frame_a - end_frame + 1.0f
                                              : end_frame;
        }
    }

    EnableShadowMapRendering(0);
    FindAndSetLights(reinterpret_cast<NUVEC *>(&world_matrix.m30), 1.0f, world->rtl_set);
    NuHGobjRndrMtxDwa(model->hierarchy, &world_matrix, render_count, render_indices, joint_matrices, NULL,
                      character->flags & 8);
    ResetShadowMapRendering();
}

static void CutScene_EvalCharacter(instNUGCUTSCENE_s *cutscene_instance, NUGCUTSCENE_s *, instNUGCUTCHAR_s *instance,
                                   NUGCUTCHAR_s *character, f32 frame) {
    NUMTX matrix;
    i32 visible;
    u32 animation_index;
    f32 animation_rate;
    f32 blend_time;
    f32 animation_start_frame;
    i32 layer_mask = -1;
    NuGCutCharAnimProcess(character, frame, &matrix, &visible, &animation_index, &animation_rate, &blend_time,
                          &animation_start_frame, &layer_mask);
    if (static_cast<i8>(cutscene_instance->flags_88) < 0) {
        NuMtxMul(&matrix, &matrix, &cutscene_instance->matrix);
    }
    if ((character->flags & 2) == 0 && instance->character_model != NULL) {
        u8 *object = static_cast<u8 *>(instance->character_model);
        memcpy(object + 0xb8, &matrix, sizeof(matrix));
        memcpy(object + 0x5c, &matrix.m30, sizeof(NUVEC));
    }
}

static void CutScene_FindCharacters(NUGCUTSCENE_s *cutscene) {
    NUGCUTCHARSYS_s *system = cutscene->character_system;
    for (i32 i = 0; i < system->character_count; ++i) {
        NUGCUTCHAR_s *character = &system->characters[i];
        character->flags |= 2;
        character->character_model = NULL;

        for (i32 j = 0; j < apicharsys->loaded_model_count; ++j) {
            APICHARACTERMODEL *model = &apicharsys->models[j];
            i32 character_id = model->model_id;
            if (NuStrICmp(character->name, CDataList[character_id].file) != 0) {
                continue;
            }

            character->character_model = model;
            if (character_id != -1) {
                CS_cutsys->character_bits[character_id >> 5] |= 1U << (character_id & 0x1f);
            }
            break;
        }

        if (character->has_locator == 0 || reinterpret_cast<usize>(character->locator) > 0xfe) {
            character->locator_index = 0xff;
        } else {
            character->locator_index = static_cast<u8>(reinterpret_cast<usize>(character->locator));
            character->locator = &cutscene->locator_system->locators[character->locator_index];
        }
    }
}

static void CutScene_ResetCharacters(instNUGCUTSCENE_s *instance) {
    NUGCUTSCENE_s *cutscene = instance->cutscene;
    instNUGCUTCHARSYS_s *character_instance = instance->character_instance;
    NUGCUTCHARSYS_s *character_system = cutscene->character_system;

    i32 i = 0;
    while (i < character_system->character_count) {
        instNUGCUTCHAR_s *character = &character_instance->characters[i];
        character->field_04 = 0;
        character->field_14 = 0;
        character->field_15 = 0xff;
        ++i;
    }
}

static void CutScene_RigidPostRender(NUGCUTRIGID_s *, instNUGCUTRIGID_s *, NUMTX *) {
}

static void CutScene_CreateCharacterInstance(NUGCUTCHAR_s *character, instNUGCUTCHAR_s *instance, variptr_u *) {
    if ((character->flags & 2) != 0) {
        instance->character_model = character->character_model;
    }
}

void CutScene_DrawSubtitles() {
}

void CutScene_StoppedFn_LSW(CUTINFO *cut) {
    if (cut != game_cutscenes.cutscene) {
        return;
    }

    for (i32 i = 50; i <= 51; ++i) {
        nuhspecial_s *special = &LevHSpecial[i];
        if (NuSpecialExistsFn(special) != 0) {
            AddPartDebris(WORLD->part_debris_sys, 0x10, NuSpecialGetDrawPos(special));
            NuSpecialSetVisibility(special, 0);
        }
    }
}

void CutScenes_BGLoadManager() {
}

void CutScenes_ConfigureList(char *, variptr_u *, variptr_u) {
}

void CutScene_PreUpdateFn_LSW(CUTINFO *) {
}

void CutScene_PostUpdateFn_LSW() {
}

i32 CutScene_PlayingOrRequested(CUTINFO *cut) {
    if (cut == NULL) {
        return CutStopInfo != NULL || NewCutInfoCount != 0;
    }
    if (CutStopInfo == cut) {
        return 1;
    }
    for (i32 i = 0; i < NewCutInfoCount; ++i) {
        if (NewCutInfo[i] == cut) {
            return 1;
        }
    }
    return 0;
}

i32 CutScene_ReplaceCharacterModelFn_LSW(CUTINFO *cut, NUGCUTCHAR_s *character) {
    if (character == NULL || cut == NULL) {
        return -1;
    }

    if (PODRACE_ADATA != NULL && PODRACE_ADATA == WORLD->area) {
        if (cut != game_cutscenes.podrace_pod_explode && cut != game_cutscenes.podrace_out_of_time &&
            cut != game_cutscenes.podrace_sebulba) {
            return -1;
        }
        if (id_ANAKINSPOD == -1 || NuStrICmp(apicharsys->char_data[id_ANAKINSPOD].file, character->name) != 0) {
            return -1;
        }
        GameObject_s *vehicle = CutDeadVehiclePlayer != NULL ? CutDeadVehiclePlayer : player;
        if (vehicle != NULL && vehicle->id == id_ANAKINSPODGREEN) {
            return vehicle->id;
        }
        return -1;
    }

    if (WORLD->current_level == PODSPRINTA_LDATA) {
        if (cut != game_cutscenes.podsprint_out_of_time && cut != game_cutscenes.podsprint_sebulba) {
            return -1;
        }
        if (id_ANAKINSNEWPOD == -1 || NuStrICmp(apicharsys->char_data[id_ANAKINSNEWPOD].file, character->name) != 0) {
            return -1;
        }
        GameObject_s *vehicle = CutDeadVehiclePlayer != NULL ? CutDeadVehiclePlayer : player;
        if (vehicle != NULL && vehicle->id == id_ANAKINSNEWPODGREEN) {
            return vehicle->id;
        }
        return id_ANAKINSNEWPOD;
    }

    if (BONUS_GUNSHIP_ADATA != NULL && BONUS_GUNSHIP_ADATA == WORLD->area) {
        if (cut == game_cutscenes.bonus_gunship_cavalry_explode && player != NULL &&
            player->id == id_REPUBLICGUNSHIP_GREEN) {
            return player->id;
        }
        return -1;
    }

    if (GUNSHIP_ADATA != NULL && GUNSHIP_ADATA == WORLD->area) {
        if (cut != game_cutscenes.bonus_gunship_cavalry_explode) {
            return -1;
        }
        if (player != NULL && player->id == id_NEW_REPUBLIC_GUNSHIP_GREEN) {
            return player->id;
        }
        return id_NEW_REPUBLIC_GUNSHIP;
    }

    if (BATTLEOVERCORUSCANT_ADATA != NULL && BATTLEOVERCORUSCANT_ADATA == WORLD->area &&
        cut == game_cutscenes.dogfight_die && player != NULL && player->id == id_JEDISTARFIGHTERYELLOWEP3) {
        return player->id;
    }
    return -1;
}

void ResetScene(nugscn_s *, SCENEPROGRESS_s *) {
}

CUTINFO *NewCutScene(CUTINFO *cut, CUTSYS *system, char *name, i32) {
    if (cut == NULL && name != NULL && system != NULL) {
        cut = CutScene_Find(system, name);
    }
    if (cut == NULL || NewCutInfoCount >= 8) {
        return NULL;
    }
    NewCutInfo[NewCutInfoCount++] = cut;
    return cut;
}

void Exit_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, float) {
}

void Fade_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, float) {
}

void RelocateCutScene(NUGCUTSCENE_s *, variptr_u *) {
}

void GoldBrick_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32) {
}

void GoldBrick_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *) {
}

void GoldBrick_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, float) {
}

void LevelComplete_LSW_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32) {
}

void LevelComplete_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *) {
}

void LevelComplete_LSW_Update(STATUS_STAGE_s *, STATUSPACKET_s *, float) {
}

static __used__ void Titles_Draw(WORLDINFO_s *) {
}

static __used__ void Titles_Init(WORLDINFO_s *) {
}

static __used__ void Titles_Update(WORLDINFO_s *) {
}
