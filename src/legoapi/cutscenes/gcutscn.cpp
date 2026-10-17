// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"

#include "globals.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/world.h"

#include <string.h>
#include <stdio.h>
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/render/core/render.h"
#include "gameapi/gui/apimenu.h"

i32 (*CutScenePlayer_AcceptFn)(CUTSCENEPLAYERCLIP *) = NULL;
extern i32 NextArea_FreePlay;
extern i16 tCHAPTER;
extern f32 ICONSIZE;
void NewLevelFromMenu(LEVELDATA_s *, i32, i32, i32);

void EvaluateJointOrientationMtx(nugscn_s *scene, i32 joint_index, numtx_s *matrix);

CUTSCENEPLAYER_s *CutScenePlayer = NULL;

void CutScenePlayer_Reset() {
    if (CutScenePlayer != NULL) {
        CutScenePlayer->active = 0;
    }
}

void CutScenePlayer_Start(i32 clip_id, i32 door) {
    if (CutScenePlayer != NULL && clip_id >= 0 && clip_id < CutScenePlayer->clip_count) {
        NewLevelFromMenu(&LDataList[CutScenePlayer->clips[clip_id].level_id], -1, -1, 1);
        CutScenePlayer->active = &CutScenePlayer->clips[clip_id];
        NextArea_FreePlay = 0;
        FreePlay = 0;
        CutScenePlayer->return_door = door;
    }
}

CUTSCENEPLAYERCLIP *CutScenePlayer_Active() {
    return CutScenePlayer != NULL ? CutScenePlayer->active : 0;
}

void CutScenePlayer_GetText(i32 id, char *title, char *subtitle, i32 mode) {
    if (CutScenePlayer == NULL) {
        if (title != NULL)
            NuStrCpy(title, "?");
        if (subtitle != NULL)
            NuStrCpy(subtitle, "?");
        return;
    }
    if (title != NULL) {
        if (mode <= 1) {
            if (CutScenePlayer->clip_text != NULL)
                sprintf(title, TTab[*CutScenePlayer->clip_text], id + 1);
            else
                sprintf(title, "Clip %i", id + 1);
        } else
            title[0] = 0;
        if (mode != 0) {
            char *type_text = NULL;
            switch (CutScenePlayer->clips[id].type) {
                case CLIP_INTRO:
                    if (CutScenePlayer->intro_text)
                        type_text = TTab[*CutScenePlayer->intro_text];
                    break;
                case CLIP_MIDTRO:
                    if (CutScenePlayer->midtro_text)
                        type_text = TTab[*CutScenePlayer->midtro_text];
                    break;
                case CLIP_OUTRO:
                    if (CutScenePlayer->outro_text)
                        type_text = TTab[*CutScenePlayer->outro_text];
                    break;
                case CLIP_ENDING:
                    if (CutScenePlayer->ending_text)
                        type_text = TTab[*CutScenePlayer->ending_text];
                    break;
            }
            if (mode <= 1)
                NuStrCat(title, " - ");
            else {
                title[0] = 0;
                if (mode != 2) {
                    const i32 area = LDataList[CutScenePlayer->clips[id].level_id].area_index;
                    if (area != -1 && !(ADataList[area].flags & 2)) {
                        const i32 chapter = static_cast<i8>(ADataList[area].area_index);
                        if (chapter != -1) {
                            sprintf(title, "%s %i", TTab[tCHAPTER], chapter + 1);
                            NuStrCat(title, " - ");
                        }
                    }
                }
            }
            NuStrCat(title, type_text);
        }
    }
    if (subtitle != NULL) {
        LEVELDATA_s *level = &LDataList[CutScenePlayer->clips[id].level_id];
        if (level->area_index != -1) {
            AREADATA_s *area = &ADataList[level->area_index];
            if (!(area->flags & 2) || level->episode_index != -1) {
                const i32 text_id = !(area->flags & 2) ? area->name_id : EDataList[level->episode_index].text_id;
                NuStrCpy(subtitle, TTab[text_id]);
                return;
            }
        }
        NuStrCpy(subtitle, "?");
    }
}

i32 CutScenePlayer_CanStart(i32 clip_id) {
    if (CutScenePlayer != NULL && clip_id >= 0 && clip_id < CutScenePlayer->clip_count) {
        const i32 area_id = static_cast<i8>(LDataList[CutScenePlayer->clips[clip_id].level_id].area_index);
        if (area_id != -1) {
            AREADATA_s *area = &ADataList[area_id];
            if ((area->flags & 2) == 0) {
                if (Game_AreaSave != NULL && Game_AreaSave[area_id].area_complete != 0)
                    return 1;
            } else {
                const i32 episode = static_cast<i8>(area->episode_index);
                if (episode != -1 && Episode_IsComplete(&EDataList[episode], NULL))
                    return 2;
            }
        }
    }
    return 0;
}

void CutScenePlayer_DrawGrid(COLLECTION_s *collection, i16 *ids, float x, float y, i32 selected, float alpha) {
    if (FadeSys.fade > 0.0f || !WORLD->lev_objs[167].active)
        return;
    if (alpha > 1.0f)
        alpha = 1.0f;
    const i32 rows = collection->count_y / collection->count_x;
    const f32 scale = ICONSIZE * 1.2f;
    NUVEC minimum, maximum;
    NuSpecialGetBounds(&WORLD->lev_objs[167].special, &minimum, &maximum);
    const f32 dx = (maximum.x - minimum.x) * scale / PANEL3DMULX;
    const f32 dy = -(maximum.y - minimum.y) * scale / PANEL3DMULY;
    y -= rows * dy * 0.5f;
    const f32 first_x = x - (collection->count_x - 1) * dx * 0.5f;
    CUTSCENEPLAYER_s *player = static_cast<CUTSCENEPLAYER_s *>(CutScenePlayer_Available());
    MENU *menu = &GameMenu[GameMenuLevel];
    i32 previous_area = -1, colour = -1;
    static const u8 colours[8][3] = {{255, 63, 0},  {255, 127, 0},  {255, 223, 0},  {0, 255, 63},
                                     {0, 127, 255}, {191, 31, 255}, {255, 31, 191}, {255, 255, 255}};
    for (i32 row = 0; row <= rows; ++row, y += dy) {
        x = first_x;
        for (i32 column = 0; column < collection->count_x; ++column, x += dx) {
            const i32 i = row * collection->count_x + column;
            if (i >= collection->count_y)
                continue;
            COLLECTID *item = &collection->list[i];
            item->grid_y = y;
            item->grid_x = x;
            if (__builtin_expect(alpha <= 0.0f, 0))
                continue;
            i32 object = i == selected ? (menu_flash ? 168 : 166) : 167;
            f32 opacity = CutScenePlayer_CanStart(ids[i]) ? alpha : 0.375f * alpha;
            DrawPanel3DObject(x, y, 1.0f, scale, scale, scale, 0, 0, 0, &WORLD->lev_objs[object].special, 0, opacity);
            menu->item_x[i] = x;
            menu->item_y[i] = y;
            menu->item_width[i] = scale * 0.5f;
            menu->item_height[i] = 0.0f;
            char text[32];
            sprintf(text, "%i", i + 1);
            i32 area = LDataList[player->clips[ids[i]].level_id].area_index;
            if (__builtin_expect(area != previous_area, 1)) {
                previous_area = area;
                if (++colour > 7)
                    colour = 7;
            }
            const f32 text_scale = scale * 3.5f;
            Text3DEx(text, x, y, 1.0f, text_scale * 0.925f, text_scale, text_scale, 0, colours[colour][0],
                     colours[colour][1], colours[colour][2], static_cast<u8>(static_cast<i32>(opacity * 128.0f)));
        }
    }
}

void CutScenePlayer_Configure(char *file, VARIPTR *buffer, VARIPTR *, i16 *clip_text, i16 *intro, i16 *midtro,
                              i16 *outro, i16 *ending) {
    NUFPAR *fp = NuFParCreate(file);
    if (fp == NULL)
        return;
    CUTSCENEPLAYER_s player;
    buffer->addr = (buffer->addr + 3) & ~3U;
    player.clips = reinterpret_cast<CUTSCENEPLAYERCLIP *>(buffer->addr);
    player.active = NULL;
    player.clip_count = 0;
    player.clip_text = clip_text;
    player.intro_text = intro;
    player.midtro_text = midtro;
    player.outro_text = outro;
    player.ending_text = ending;
    while (NuFParGetLine(fp)) {
        if (NuFParGetWord(fp) && !(NuStrICmp(fp->word_buf, "clip"))) {
            CUTSCENEPLAYERCLIP *clip = &player.clips[player.clip_count];
            clip->level_id = -1;
            clip->type = CLIP_INTRO;
            clip->name[0] = 0;
            clip->guest_episode = -1;
            while (NuFParGetWord(fp)) {
                if (NuStrICmp(fp->word_buf, "in_level") == 0) {
                    i32 level;
                    if (NuFParGetWord(fp) && Level_FindByName(fp->word_buf, &level))
                        clip->level_id = level;
                } else if (NuStrICmp(fp->word_buf, "name") == 0) {
                    if (NuFParGetWord(fp) && NuStrLen(fp->word_buf) <= 63)
                        NuStrCpy(clip->name, fp->word_buf);
                } else if (NuStrICmp(fp->word_buf, "intro") == 0)
                    clip->type = CLIP_INTRO;
                else if (NuStrICmp(fp->word_buf, "midtro") == 0)
                    clip->type = CLIP_MIDTRO;
                else if (NuStrICmp(fp->word_buf, "outro") == 0)
                    clip->type = CLIP_OUTRO;
                else if (NuStrICmp(fp->word_buf, "ending") == 0)
                    clip->type = CLIP_ENDING;
                else if (NuStrICmp(fp->word_buf, "guest_in_episode") == 0 && NuFParGetWord(fp))
                    clip->guest_episode = NuAToI(fp->word_buf);
            }
            if (clip->level_id != -1 && (CutScenePlayer_AcceptFn == NULL || CutScenePlayer_AcceptFn(clip)))
                ++player.clip_count;
        }
    }
    NuFParDestroy(fp);
    if (player.clip_count == 0) {
        return;
    }
    buffer->void_ptr = &player.clips[player.clip_count];
    memmove(buffer->void_ptr, &player, sizeof(player));
    CutScenePlayer = reinterpret_cast<CUTSCENEPLAYER_s *>(buffer->void_ptr);
    buffer->addr += sizeof(player);
}

void CutScenePlayer_SetObjects(CUTINFO *cut) {
    if (CutScenePlayer_Active() != NULL && cut->state_entries != NULL) {
        for (i32 i = 0; i < cut->state_count; ++i) {
            CUTSCENEPLAYEROBJ *entry = &cut->state_entries[i];
            const u8 flags = entry->flags;
            if (flags & CLIP_OBJECT_SHOW)
                NuSpecialSetVisibility(&entry->special, 1);
            else if (flags & CLIP_OBJECT_HIDE)
                NuSpecialSetVisibility(&entry->special, 0);
            else if (flags & CLIP_OBJECT_ANIM_END) {
                f32 end = NuSpecialGetAnimEndFrame(&entry->special);
                nuinstanim_s *anim = NuSpecialGetInstAnim(&cut->state_entries[i].special);
                if (anim != NULL) {
                    anim->ltime = end;
                    anim->playing = 1;
                }
            }
        }
    }
}

i32 CutScenePlayer_CountEpisodeClips(i32 episode, i32 include_guests, i16 *ids) {
    i32 count = 0;
    CUTSCENEPLAYER_s *player = CutScenePlayer;
    if (player != NULL) {
        i32 clip_count = player->clip_count;
        if (__builtin_expect(clip_count != 0, 1)) {
            LEVELDATA *levels = LDataList;
            CUTSCENEPLAYERCLIP *clip = player->clips;
            for (i32 i = 0; i < clip_count; ++i, ++clip) {
                if (__builtin_expect(levels[clip->level_id].episode_index == episode ||
                    (include_guests != 0 && clip->guest_episode == episode), 1)) {
                    if (ids != NULL) {
                        ids[count++] = i;
                        clip_count = player->clip_count;
                    } else {
                        ++count;
                    }
                }
            }
        }
    }
    return count;
}

void FindGameCutScenes() {
    memset(&game_cutscenes, 0, sizeof(game_cutscenes));

    game_cutscenes.podrace_pod_explode = CutScene_Find(WORLD->cutscene_sys, "ep1_podrace_podexplode");
    game_cutscenes.podrace_out_of_time = CutScene_Find(WORLD->cutscene_sys, "ep1_podrace_outoftime");
    game_cutscenes.bonus_gunship_cavalry_explode =
        CutScene_Find(WORLD->cutscene_sys, "ep2_bonus_gunshipcavalry_explode");
    game_cutscenes.droid_factory_conveyor = CutScene_Find(WORLD->cutscene_sys, "ep2_droidfactory_conveyor");
    game_cutscenes.podrace_avalanche = CutScene_Find(WORLD->cutscene_sys, "ep1_podrace_avalanche");
    game_cutscenes.dogfight_die = CutScene_Find(WORLD->cutscene_sys, "ep3_dogfight_die");
    game_cutscenes.podrace_sebulba = CutScene_Find(WORLD->cutscene_sys, "ep1_podrace_sebulba");
    game_cutscenes.cutscene = CutScene_Find(WORLD->cutscene_sys, "ep1_podsprint_avalanche");
    game_cutscenes.podsprint_out_of_time = CutScene_Find(WORLD->cutscene_sys, "ep1_podsprint_outoftime");
    game_cutscenes.podsprint_sebulba = CutScene_Find(WORLD->cutscene_sys, "ep1_podsprint_sebulba");
}

SCENESPECIALPROGRESS_s *FindSceneStateObj(nugscn_s *scene, SCENEPROGRESS_s *progress, nuhspecial_s *target);

i32 FindMtlInHGObj(nugscn_s *scene, i32 material_type);

i32 FindTexAnimFromMtl(nugscn_s *, numtl_s *);

void instGetLookAtLocatorInfo(instNUGCUTSCENE_s *instance, instNUGCUTLOOKAT_s *opaque_state);

i32 instNuGCutGetNextRigidInfo(instNUGCUTSCENE_s *instance, float frame, i32 index, numtx_s *matrix,
                               nuhspecial_s *special);

i32 instNuGCutSceneSwapBuffers(instNUGCUTSCENE_s *instance, i32 force);

void instNuGCutSceneResetCamLock(instNUGCUTSCENE_s *instance);

void instNuGCutSceneEndButNotSystems(instNUGCUTSCENE_s *instance);

i32 instNuGCutContainsInstancedRigids(instNUGCUTSCENE_s *instance);

// ==== src/legoapi/cutscenes/cutscene.cpp: declarations ====
#include "decomp.h"
#include "globals.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/audio/audio.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/cutscenes/cutscene_defrag.h"
#include "nu2api/nucore/nugcutscene.h"
#include "legoapi/world/world_shared.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "legoapi/characters/core/character.h"
#include "legoapi/render/fx.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numusic/sfx.h"
struct CUTSCENEPLAYERCLIP;
struct instNUGCUTCHAR_s;
struct NUGCUTCHAR_s;
struct NUGCUTRIGID_s;
struct instNUGCUTRIGID_s;

extern CUTSYS * CS_cutsys;


static void copyAnims(NUGCUTSCENE_s *, NUGCUTSCENE_s *);
void NewCopyAnims(instNUGCUTSCENE_s *);
i32 instNuGCutSceneSwapBuffers(instNUGCUTSCENE_s *, i32);
NUGCUTSCENE_s *RelocateCutScene(NUGCUTSCENE_s *, VARIPTR *);
extern "C" void instNuGCutSceneDestroy(instNUGCUTSCENE_s *);


extern i32 CUTCOUNT;
extern CutSceneListEntry * CutList;
extern void (*CutScene_OverrideConfigFileNameFn)(char *, i32, i32);
extern i32 ACTIVECUTCOUNT;
extern f32 CutSceneScale;
extern "C" {
extern u8 CUTSUBTITLEDEFAULT_R;
extern u8 CUTSUBTITLEDEFAULT_G;
extern u8 CUTSUBTITLEDEFAULT_B;
extern u8 CUTSUBTITLEDEFAULT_A;
extern u8 CUTSUBTITLEDEFAULT_ALIGN;
extern f32 CUTSUBTITLEDEFAULT_X;
extern f32 CUTSUBTITLEDEFAULT_Y;
extern f32 CUTSUBTITLEDEFAULT_SCALEX;
extern f32 CUTSUBTITLEDEFAULT_SCALEY;
extern f32 CUTSUBTITLEDEFAULT_FITWIDTH;
}
extern i32 CUTCAM;
extern i32 CUTCAMONLY;
NUMTX cutscenecammtx = {};
i32 cutscenecamchange = 0;
f32 cutscenecam_fstop = 0.0f;
u8 cutscenecam_usefocusloc = 0;
NUVEC cutscenecam_focusloc = {};
f32 cutscenecam_focusDistance = 0.0f;
f32 cutscenecam_focalLength = 0.0f;
i32 CameraDOFHack = 0;
u8 set_cutscenecammtx = 0;












































void *CutScenes_Load(char *config, NUGSCN *gscn1, NUGSCN *gscn2, i32 param1, VARIPTR *buf, VARIPTR *buf_end, i32 param2,
                     i32 param3, WORLDINFO *world);

// --- Extern "C" block: functions with confirmed C linkage in original libTTapp.so ---
extern "C" void *NuAnimData2FixPtrs(void *, isize, isize, i32);
extern "C" StateAnim *StateAnimFixPtrs(StateAnim *, isize);
extern "C" i32 StateAnimEvaluate(StateAnim *, u8 *, u8 *, f32);
void instNuGCutSceneResetCamLock(instNUGCUTSCENE_s *instance);
extern "C" void NuAnimCurve2SetApplyToMatrix_3(ani3_animheader_s *, i32, f32, NUMTX *);
extern "C" {
    extern i32 NuGCutDebFixUp_SearchAllPages;
    extern NUGCUTLOCATORFNENTRY_s *locatorfns;
}
void NuGCutRigidCalcMtx(NUGCUTRIGID_s *, f32, numtx_s *);

#include "legoapi/cutscenes/cutscene_types.h"
DECOMP_ASSERT(sizeof(CutSceneTriggerNameEntry) == 0x34, "cutscene trigger name entry stride");
DECOMP_ASSERT(offsetof(CutSceneTriggerNameTable, entries) == 0x0c, "cutscene trigger name table entries offset");

static instNUGCUTSCENE_s *background_cutscene_instances;
static instNUGCUTSCENE_s *active_cutscene_instances;
static i32 cutscene_synchro_counter;
// Exported (default visibility) in the original library.
i32 cutscene_quit_prompt;
i32 cutscene_has_quit;
i32 cutscene_lock_player;
i32 termcutstream_hack;

extern "C" {
    instNUGCUTSCENE_s *cutscene_load_instance;
    i32 NumCommonStreamingBuffers = 2;

instNUGCUTSCENE_s *instNuGCutSceneFind(const char *name);

void *instNuGCutSceneCleanUp(void);
}

// The relocation tables are patched as plain words, so every fixup aliases
// cutscene->string_delta and re-reads it.
typedef isize __attribute__((may_alias)) NuGCutWord;
#define NUGCUT_WORD(field) (*reinterpret_cast<NuGCutWord *>(&(field)))
#define NUGCUT_RELOCATE(field, delta) \
    (NUGCUT_WORD(field) = NUGCUT_WORD(field) != 0 ? NUGCUT_WORD(field) + (delta) : 0)

static inline __attribute__((always_inline)) void NuGCutSceneFixPtrs_Title(NUGCUTSCENE_s *cutscene, isize anim_delta) {
    NUGCUT_RELOCATE(cutscene->strings, cutscene->string_delta);
    if (cutscene->camera_system != NULL) {
        NUGCUT_WORD(cutscene->camera_system) += cutscene->string_delta;
        NUGCUTCAMERASYS_s *system = cutscene->camera_system;
        NUGCUT_RELOCATE(system->cameras, cutscene->string_delta);
        if (anim_delta != 0) {
            if (cutscene->version > 4) {
                if (system->focus_animation != NULL) {
                    system->focus_animation =
                        static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(system->focus_animation, anim_delta, 0, 0));
                }
                system->focus_state_animation = StateAnimFixPtrs(system->focus_state_animation, anim_delta);
            }
            if (system->animation != NULL) {
                system->animation =
                    static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(system->animation, anim_delta, 0, 0));
            }
            system->state_animation = StateAnimFixPtrs(system->state_animation, anim_delta);
        }
    }
    if (cutscene->locator_system != NULL) {
        NUGCUT_WORD(cutscene->locator_system) += cutscene->string_delta;
        NUGCUTLOCATORSYS_s *system = cutscene->locator_system;
        NUGCUT_RELOCATE(system->locators, cutscene->string_delta);
        if (system->locators != NULL && __builtin_expect(anim_delta != 0, 0)) {
            for (i32 i = 0; i < system->locator_count; ++i) {
                NUGCUTLOCATOR_s *locator = &system->locators[i];
                if (locator->animation != NULL) {
                    locator->animation =
                        static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(locator->animation, anim_delta, 0, 0));
                }
            }
        }
        NUGCUT_RELOCATE(system->types, cutscene->string_delta);
        if (system->types != NULL) {
            for (i32 i = 0; i < system->type_count; ++i) {
                if (system->types[i].name != NULL) {
                    system->types[i].name = reinterpret_cast<char *>(reinterpret_cast<isize>(system->types[i].name) +
                                                                     reinterpret_cast<isize>(cutscene->strings) - 1);
                }
            }
        }
    }
    if (cutscene->rigid_system != NULL) {
        NUGCUT_WORD(cutscene->rigid_system) += cutscene->string_delta;
        NUGCUTRIGIDSYS_s *system = cutscene->rigid_system;
        NUGCUT_RELOCATE(system->rigids, cutscene->string_delta);
        if (system->rigids != NULL) {
            // Retail unswitches this loop on anim_delta.
            if (__builtin_expect(anim_delta != 0, 1)) {
                for (i32 i = 0; i < system->count; ++i) {
                    NUGCUTRIGID_s *rigid = &system->rigids[i];
                    if (rigid->name != NULL) {
                        rigid->name = reinterpret_cast<char *>(reinterpret_cast<isize>(rigid->name) +
                                                               reinterpret_cast<isize>(cutscene->strings) - 1);
                    }
                    if (rigid->animation != NULL) {
                        rigid->animation =
                            static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(rigid->animation, anim_delta, 0, 0));
                    }
                    rigid->state_animation = StateAnimFixPtrs(rigid->state_animation, anim_delta);
                }
            } else {
                for (i32 i = 0; i < system->count; ++i) {
                    NUGCUTRIGID_s *rigid = &system->rigids[i];
                    if (rigid->name != NULL) {
                        rigid->name = reinterpret_cast<char *>(reinterpret_cast<isize>(rigid->name) +
                                                               reinterpret_cast<isize>(cutscene->strings) - 1);
                    }
                }
            }
        }
    }
    if (cutscene->character_system != NULL) {
        NUGCUT_WORD(cutscene->character_system) += cutscene->string_delta;
        NUGCUTCHARSYS_s *system = cutscene->character_system;
        NUGCUT_RELOCATE(system->characters, cutscene->string_delta);
        if (system->characters != NULL) {
            // Retail also unswitches the character loop on anim_delta.
            if (anim_delta == 0) {
                for (i32 i = 0; i < system->character_count; ++i) {
                    NUGCUTCHAR_s *character = &system->characters[i];
                    if (character->name != NULL) {
                        character->name = reinterpret_cast<char *>(reinterpret_cast<isize>(character->name) +
                                                                   reinterpret_cast<isize>(cutscene->strings) - 1);
                    }
                }
            } else {
                for (i32 i = 0; i < system->character_count; ++i) {
                    NUGCUTCHAR_s *character = &system->characters[i];
                    if (character->name != NULL) {
                        character->name = reinterpret_cast<char *>(reinterpret_cast<isize>(character->name) +
                                                                   reinterpret_cast<isize>(cutscene->strings) - 1);
                    }
                    if (character->animation != NULL) {
                        character->animation =
                            static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(character->animation, anim_delta, 0, 0));
                    }
                    if (character->face_animation != NULL) {
                        character->face_animation = static_cast<nuanimdata2_s *>(
                            NuAnimData2FixPtrs(character->face_animation, anim_delta, 0, 0));
                    }
                    if (character->extra_animation != NULL) {
                        character->extra_animation = static_cast<nuanimdata2_s *>(
                            NuAnimData2FixPtrs(character->extra_animation, anim_delta, 0, 0));
                    }
                }
            }
        }
        if (cutscene->version > 3) {
            NUGCUT_RELOCATE(cutscene->character_animations, cutscene->string_delta);
            if (system->characters != NULL && cutscene->character_animations != NULL) {
                for (i32 i = 0; i < system->character_count; ++i) {
                    NUGCUTCHARANIM_s *animation = &cutscene->character_animations[i];
                    if (animation->animation != NULL) {
                        animation->animation =
                            static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(animation->animation, anim_delta, 0, 0));
                    }
                }
            }
        }
    }
    if (cutscene->trigger_system != NULL) {
        const isize delta = cutscene->string_delta;
        NUGCUT_WORD(cutscene->trigger_system) += delta;
        NUGCUTTRIGGERSYS_s *system = cutscene->trigger_system;
        NUGCUT_RELOCATE(system->events, delta);
        if (system->events != NULL) {
            for (i32 i = 0; i < system->event_count; ++i) {
                NUGCUTTRIGGEREVENT_s *event = &system->events[i];
                NUGCUT_RELOCATE(event->field_04, delta);
                event->state_animation = StateAnimFixPtrs(event->state_animation, delta);
            }
        }
    }
    NUGCUT_RELOCATE(cutscene->bounds, cutscene->string_delta);
}

// Retail keeps NuGCutSceneFixPtrs only as two constant-propagated clones:
// .44 relocates animations by anim_delta, .43 is the anim_delta == 0 case.
#if defined(__i386__)
#define NUGCUT_FIXPTRS_ATTR(n) __attribute__((noinline, noclone, regparm(n)))
#else
#define NUGCUT_FIXPTRS_ATTR(n) __attribute__((noinline, noclone))
#endif
static NUGCUT_FIXPTRS_ATTR(2) void NuGCutSceneFixPtrsAnims(NUGCUTSCENE_s *cutscene, isize anim_delta)
    __asm__("_ZL18NuGCutSceneFixPtrsP13NUGCUTSCENE_sii.constprop.44");
static void NuGCutSceneFixPtrsAnims(NUGCUTSCENE_s *cutscene, isize anim_delta);
static NUGCUT_FIXPTRS_ATTR(1) void NuGCutSceneFixPtrsNoAnims(NUGCUTSCENE_s *cutscene)
    __asm__("_ZL18NuGCutSceneFixPtrsP13NUGCUTSCENE_sii.constprop.43");
static void NuGCutSceneFixPtrsNoAnims(NUGCUTSCENE_s *cutscene);

extern "C" void NuGCutSceneSysInit(NUGCUTLOCATORFNENTRY_s *locator_functions);

void NuGCutSceneSysInitVfx(NUGCUTLOOKUPLOCATORVFXFN lookup, NUGCUTTRIGGERLOCATORVFXFN trigger,
                           NUGCUTRELEASELOCATORVFXFN release, NUGCUTUPDATELOCATORVFXFN update);

void NuGCutSceneRemapFocusIdToLocaterNum(NUGCUTSCENE_s *cutscene, VARIPTR *buffer);

extern "C" {

    i32 NuGCutDebFixUp_SearchAllPages = 0;
    NUGCUTLOCATORFNENTRY_s *locatorfns = NULL;
    NUGCUTUPDATELOCATORVFXFN UpdateLocatorVfxFn = NULL;
    NUGCUTRELEASELOCATORVFXFN ReleaseLocatorVfxFn = NULL;
    NUGCUTTRIGGERLOCATORVFXFN TriggerLocatorVfxFn = NULL;
    NUGCUTLOOKUPLOCATORVFXFN LookupLocatorVfxFn = NULL;
NUGCUTSCENE_s *NuGCutSceneLoad(char *name, VARIPTR *buf, VARIPTR *buf_end, i32 flags);

NUGCUTSCENE_s *NuGCutSceneLoadAddr(NUGCUTSCENE_s *cutscene, i32 loaded_size, VARIPTR *buffer);

void NuGCutSceneDestroy(NUGCUTSCENE_s *cutscene);

void NuGCutSceneFixUp(NUGCUTSCENE_s *cutscene, NUGSCN *scene, i32 flags, i32 area);
void NuGCutSceneFixUpExtra(NUGCUTSCENE_s *cutscene, NUGSCN *area);
instNUGCUTSCENE_s *instNuGCutSceneCreate(NUGCUTSCENE_s *cutscene, NUGSCN *scene, void *extra, char *name,
                                             VARIPTR *buf, i32 flags);

void instNuGCutSceneReset(instNUGCUTSCENE_s *instance);

void instNuGCutSceneStart(instNUGCUTSCENE_s *instance);

void instNuGCutScenePause(instNUGCUTSCENE_s *instance, u8 paused);

void instNuGCutSceneDestroy(instNUGCUTSCENE_s *instance);

i32 NuGCutSceneIsBackgroundLoading(void);

i32 instNuGCutScenePreload(instNUGCUTSCENE_s *instance);

void instNuGCutSceneServiceLoad(void);

void NuGCutSceneSysBackgroundFlush(void);

void NuGCutSceneSysPostBackgroundLoad(void);

void PetesHackOfDeath(void);
} // extern "C"

struct instNUGCUTSCENE_s;
struct NUGCUTLOCATORSYS_s;
struct instNUGCUTLOCATOR_s;
struct NUGCUTLOCATOR_s;
struct numtx_s;
extern "C" f32 *NuGCutLocatorCalcMtx(NUGCUTLOCATOR_s *, f32, NUMTX *, nuanimtime_s *);
extern "C" i32 NuGCutLocatorIsVisble(NUGCUTLOCATOR_s *, f32, nuanimtime_s *, f32 *, f32 *);
extern "C" void NuAnimData2CalcTime(nuanimdata2_s *, f32, nuanimtime_s *);
extern "C" void instNuGCutLocatorUpdate(instNUGCUTSCENE_s *, NUGCUTLOCATORSYS_s *, instNUGCUTLOCATOR_s *,
                                        NUGCUTLOCATOR_s *, f32, NUMTX *, i32);
i32 Draw3DObjectMtx(WORLDINFO_s *, i32, numtx_s *);

void instNuGCutSceneEndButNotSystems(instNUGCUTSCENE_s *instance);
void instNuGCutSceneResetCamLock(instNUGCUTSCENE_s *instance);
extern "C" void DebFreeInstantly(i32 *handle);

static void instNuGCutRigidSysEnd(instNUGCUTSCENE_s *instance, float frame);

// Retail only keeps the IPA-SRA clone, which drops the unused frame argument.
#if defined(__i386__)
#define INST_LOCATOR_SYS_END_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define INST_LOCATOR_SYS_END_ATTR __attribute__((noinline, noclone))
#endif
static INST_LOCATOR_SYS_END_ATTR void instNuGCutLocatorSysEnd(instNUGCUTLOCATORSYS_s *instance,
                                                              NUGCUTLOCATORSYS_s *system)
    __asm__("_ZL23instNuGCutLocatorSysEndP22instNUGCUTLOCATORSYS_sP18NUGCUTLOCATORSYS_sf.isra.8");
static void instNuGCutLocatorSysEnd(instNUGCUTLOCATORSYS_s *instance, NUGCUTLOCATORSYS_s *system);

void instNuGCutSceneEndFirstFrame(instNUGCUTSCENE_s *instance);

extern "C" void instNuGCutSceneJumpToEnd(instNUGCUTSCENE_s *instance);

extern "C" void instNuGCutSceneJumpToLastFrame(instNUGCUTSCENE_s *instance);

extern "C" void instNuGCutSceneEnd(instNUGCUTSCENE_s *instance);

static void instNuGCutRigidSysUpdate(instNUGCUTSCENE_s *, float, int);
static void instNuGCutCamSysUpdate(instNUGCUTSCENE_s *, float);
static void instNuGCutTriggerSysUpdate(instNUGCUTSCENE_s *, float);
static void instNuGCutSceneClipTest(instNUGCUTSCENE_s *);

static inline u8 instNuGCutSceneLastStream(instNUGCUTSCENE_s *instance) {
    return instance->cutscene_copy->last_stream;
}

static inline NUGCUTSCENE_s *instNuGCutSceneCurrentStreamBuffer(instNUGCUTSCENE_s *instance) {
    // stream_buffer_0/1 are adjacent (asserted in nugcutscene.h); the original indexes them by flag bit.
    return (&instance->stream_buffer_0)[(static_cast<u32>(instance->flags_8b) >> 4) & 1];
}

static void instNuGCutSceneUpdate(instNUGCUTSCENE_s *instance, int paused, int skip, float elapsed);

static void instNuGCutCamSysUpdate(instNUGCUTSCENE_s *instance, float frame);

static void instNuGCutSceneClipTest(instNUGCUTSCENE_s *instance);

static void instNuGCutRigidSysRender(instNUGCUTSCENE_s *instance, float frame, int paused);

static void instNuGCutRigidSysUpdate(instNUGCUTSCENE_s *instance, float frame, int paused);

static __used__ void instNuGCutTriggerSysUpdate(instNUGCUTSCENE_s *instance, float frame);

extern "C" void NuGCutSceneSysUpdate(i32 paused, i32 skip, f32 elapsed);

extern "C" void NuGCutSceneSysRender(i32 paused);


void NewCopyAnims(instNUGCUTSCENE_s *instance);

static void copyAnims(NUGCUTSCENE_s *destination, NUGCUTSCENE_s *source);

// ==== src/legoapi/cutscenes/cutscene_stubs.cpp: declarations ====
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

i32 CheckStreamFileID(void);

void DisplayCutSceneMemory(void);

void PauseGameCut(void);

void RestoreGameCut(void);

extern u8 ForceScenePlayBack;

void SetForceScenePlayBack(i32 enabled);

f32 instCutSceneTimeElapsed(instNUGCUTSCENE_s *instance);

i32 instNuGCutSceneAddCamTgt(instNUGCUTSCENE_s *instance, NUVEC *target, f32 start_frame, f32 duration,
                                 i8 target_index);

void instNuGCutSceneAddCleanUpItem(void *handle, i32 enabled);

void instNuGCutSceneCalculateAverageCentre(instNUGCUTSCENE_s *instance, NUMTX *matrix, NUVEC *out);

void instNuGCutSceneChain(instNUGCUTSCENE_s *instance, instNUGCUTSCENE_s *next);

i32 instNuGCutSceneCharGetStartMtx(instNUGCUTSCENE_s *instance, const char *name, NUMTX *out);

void instNuGCutSceneCreateCamTgtArray(instNUGCUTSCENE_s *instance, i32 count, VARIPTR *buf);

void instNuGCutSceneDisable(instNUGCUTSCENE_s *instance);

void instNuGCutSceneEnable(instNUGCUTSCENE_s *instance);

i32 instNuGCutSceneIsFinished(instNUGCUTSCENE_s *instance);

void instNuGCutScenePlay(instNUGCUTSCENE_s *instance, i32 forward);

void instNuGCutSceneResetCleanUp(void *list_storage, void *cut_base, void *cut_end, void *inst_base, void *inst_end,
                                     instNUGCUTSCENE_s *(*get_inst)(void *),
                                     void *(*create_inst)(void *, NUGCUTSCENE_s *, VARIPTR *),
                                     void (*inst_destroyed)(void *));

void instNuGCutSceneRotateY(instNUGCUTSCENE_s *instance, NUANG angle);

void instNuGCutSceneSetEndCallback(instNUGCUTSCENE_s *instance, void (*callback)(instNUGCUTSCENE_s *));

void instNuGCutSceneSetMtx(instNUGCUTSCENE_s *instance, NUMTX *matrix);

void instNuGCutSceneSetPos(instNUGCUTSCENE_s *instance, NUVEC *pos);

void instNuGCutSceneSetRepeat(instNUGCUTSCENE_s *instance, i32 repeat);

void instNuGCutSceneStop(instNUGCUTSCENE_s *instance);

f32 instNuGCutSceneTimeLeft(instNUGCUTSCENE_s *instance);

f32 instNuGCutSceneTotalTime(instNUGCUTSCENE_s *instance);

void instNuGCutSceneTranslate(instNUGCUTSCENE_s *instance, NUVEC *translation);

void instNuGCutSceneWaitAtEnd(instNUGCUTSCENE_s *instance, u8 enabled);

void instNuGCutSoundStream(void);

} // extern "C"

void instGetLookAtLocatorInfo(instNUGCUTSCENE_s *instance, instNUGCUTLOOKAT_s *state);

// Steers a cutscene character's eyes towards a look-at locator and blinks its eyelids.
// Only version-3 locator animations carry the look and blink channels.
// Retail only keeps the IPA-SRA clone: the system is dropped and the instance
// locator is replaced by its effect handle slot.
#if defined(__i386__)
#define INST_LOOKAT_UPDATE_ATTR __attribute__((noinline, noclone, regparm(2), sseregparm))
#else
#define INST_LOOKAT_UPDATE_ATTR __attribute__((noinline, noclone))
#endif
static INST_LOOKAT_UPDATE_ATTR void instNuGCutLookAtLocatorUpdate(i32 *effect_handle, NUGCUTLOCATOR_s *locator,
                                                                  f32 frame, NUMTX *parent_mtx,
                                                                  instNUGCUTSCENE_s *instance)
    __asm__("_ZL29instNuGCutLookAtLocatorUpdateP18NUGCUTLOCATORSYS_sP19instNUGCUTLOCATOR_sP15NUGCUTLOCATOR_"
            "sfP7numtx_sP17instNUGCUTSCENE_s.isra.23");
static void instNuGCutLookAtLocatorUpdate(i32 *effect_handle, NUGCUTLOCATOR_s *locator, f32 frame,
                                          NUMTX *parent_mtx, instNUGCUTSCENE_s *instance);

extern "C" {
    // Aligned four-float row used for retail's movaps matrix copies.
    typedef f32 LocatorMtxRow __attribute__((vector_size(16)));

void instNuGCutLocatorUpdate(instNUGCUTSCENE_s *instance, NUGCUTLOCATORSYS_s *system,
                                 instNUGCUTLOCATOR_s *inst_locator, NUGCUTLOCATOR_s *locator, f32 frame,
                                 NUMTX *parent_mtx, i32 paused);

} // extern "C"

// ==== src/nu2api/nusound/nu2api_nusound_plain.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nusound/nusound.h"

#include <string.h>

typedef void (*SoundBitCallback)(i32 sound_id);

extern "C" void MusicPreSeek(i32 track);
extern "C" void RestoreGameMusic(void);

extern "C" {
    i32 g_NuSoundMaxVoicesPerSample = 3;
    f32 AUDIOFADELEVEL = 1.0f;
extern f32 MASTERVOLUME;
    i32 gcutSoundMusVol = 30;
    i32 gcutSoundVol = 100;

extern MusicPlayback Music;

    void NuSound3FlushLoops(void);
    void SoundStopMusic(void);

f32 GetSoundVolume(void);
void SetSoundBitsById(const i32 *sound_ids, SoundBitCallback set_bit);
void SetSoundBitsBySingleId(i32 sound_id, SoundBitCallback set_bit);
void SetSoundVolume(f32 volume);
void SoundKillAll(void);
void SoundStopMusic(void);

void edanimSoundDestroy(i32 parameter_index, i32 sound_index);
void edbitsSoundPlay(NUVEC *, i32);
void gcutSetSoundVol(i32 sound_volume, i32 music_volume);
}

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
// Temporary C-linkage compatibility surface for unresolved engine owners.
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
extern "C" {
extern NULGTARCMATERIAL NuLgtArcMtl[4];
extern i32 NuLgtLaserOldCnt;
}
#include "nu2api/nucore/nuonline.h"
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuqfnt.h"

void NuErrorPrint(char *);
void NuDebugMsgPrint(char *);
extern NUQFNT *system_qfont;
void NuLgtArcLaserEx(i32 type, NUVEC *start, NUVEC *end, NUVEC *control, f32 width, f32 segment_length, f32 wobble,
                     f32 duration, i32 colour, i32 flags);

#include "decomp.h"
#include "java/android.h"
#include "java/java.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/NuInputManager.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nuscreen.hpp"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/numath/numtx.h"
#include "globals.h"

extern NuInputManager *inputManager;
extern bool used_touch_IDs[10];

struct ani3_animheader_s;
struct nuanimdatachunk_s;
nuanimdatachunk_s *NuAnimDataChunkCreate(i32 curve_set_count);

extern "C" {

extern u8 CutSceneBoundingBoxTrackRoot;
    i32 NuGCutAudioStream = 0;
extern NUANIMBUFFEVALUATECB AnimBuffEvalCB;
extern void ** AnimBuffEvalData;
extern i32 * AnimBuffEvalJoint;
extern GLuint g_colorRenderbuffer;
extern GLuint g_depthRenderbuffer;
}


using NUHGOBJVIDEOMEMFN = void (*)(nuhgobj_s *);

extern NUHGOBJVIDEOMEMFN hgobj_to_video_mem;
extern NUHGOBJVIDEOMEMFN video_mem_to_hgobj;

extern "C" {
    extern NUGLOBALRNDRSTATE render_state;
    void RndrStateSetConstAlphaTint(i32 alpha_enabled, i32 tint_enabled, f32 alpha, const NUCOLOUR3 *tint, NUMTL *mtl);
}

// C++-linkage helpers defined in sibling TUs.
void NuPadRecordEndFrame(void);                      // nupad_interface.cpp
void bgSuspendMain(i32);                             // main.cpp
void NuAnimBuffInit(i32, VARIPTR *, VARIPTR);        // nu2api_nucore_misc.cpp
i32 NuCameraClipHGobj(nugscn_s *, NUMTX *, NUMTX *); // nu2api_nucore_misc.cpp
i32 findrange(nugscn_s *, i32);                      // utilities.cpp

extern "C" {

    // ---------------------------------------------------------------------------
    // Display-list bootstrap (original nucore TU file-statics)
    // ---------------------------------------------------------------------------

    i32 NuHasError(void);
    void NuMtlAnimate(f32 frame_time);
    void NuTexAnimProcess(f32 frame_time);
    void NuShaderManagerSetfv(i32 semantic, const f32 *values);

    // ---------------------------------------------------------------------------
    // Redirected symbols — real bodies live elsewhere (kept as comments)
    // ---------------------------------------------------------------------------

    // Nu360_dxClear(u32,u32) is implemented in ios_graphics.cpp (original 0x317070).
    // NuDisplayListSwapBuffersBeginFrame / EndFrame are transcribed in full
    // in nudlist.cpp (originals 0x2eb5d0 / 0x2eaef0).
    // NuHtmlBegin(void*) is transcribed in legoapi/misc/supportall.cpp (original 0x2d5ca0).
    // NuIOS_SetVertexFormat is transcribed in nuiosdl_gl.cpp (original 0x29c070).
    // NuIOS_WaitForRenderThreadCompletion is transcribed in ios_graphics.cpp (original 0xe34b0).
    // NuIOS_WakeRenderThread is transcribed in ios_graphics.cpp (original 0xe3590).
    // NuRenderContextSetZFunc is transcribed below (original 0x2a3860).

    // ---------------------------------------------------------------------------
    // Camera (thin wrappers; full matrix work is in NuCameraSetEx)
    // ---------------------------------------------------------------------------

f32 NuCameraCalcAperture(f32 focal_length, f32 root_fstop);
void NuCameraCalcClipMtx(NUCAMERACLIP *clip, NUCAMERA *camera, i32 use_cached_projection);
f32 NuCameraCalcRootFStop(f32 focal_length, f32 aperture);
void NuCameraClearStateBuffer(void);
i32 NuCameraClipTestExtentsGeneric(NUVEC *min, NUVEC *max, NUMTX *world_mtx, f32 far_clip, i32 flags,
                                       f32 *min_depth);
i32 NuCameraClipTestPointScissor(NUVEC *point);
i32 NuCameraClipTestPointVport(NUVEC *point);
i32 NuCameraClipTestPoints(NUVEC *points, i32 count, NUMTX *world_mtx);
f32 NuCameraFOVToFocalLen(f32 fov);
f32 NuCameraFocalLenToFOV(f32 focal_length);
void NuCameraForceFarclip(i32 enabled);
void NuCameraGetAxes(NUVEC *axes);
NUMTX *NuCameraGetClippingMtx(void);
void NuCameraGetClippingRatios(f32 *horizontal, f32 *vertical);
NUMTX *NuCameraGetPCMtx(void);
NUMTX *NuCameraGetPCSMtx(void);
NUMTX *NuCameraGetVPCSMtx(void);
i32 NuCameraIntersectsAABB(NUVEC *center, NUVEC *extent, f32 far_clip, i32 use_scissor);
extern i32 prev_lock;
extern NUCAMERA locked_camera;
extern NUCAMERA cam_copy;
extern i32 camfx;
extern NuCameraReflect global_reflect;
    DECOMP_ASSERT(sizeof(NuCameraReflect) == 0x84, "camera reflection state size");
    DECOMP_ASSERT(__builtin_offsetof(NuCameraReflect, mtx) == 4, "camera reflection matrix offset");

void NuCameraLock(i32 lock);
    extern nurenderscene_s currentScene;
extern i32 NuRndrDoingScreenGrab;
extern i32 motionBlurAccumActiveThisFrame;

void NuCameraMotionBlurEffect(const NUMTX *previous, const NUMTX *current, f32 scale, f32 maximum, f32 falloff);
void NuCameraMotionBlurParams(const NUMTX *current);
void NuCameraRelock(void);
i32 NuCameraSaveState(void);
void NuCameraSet(NUCAMERA *cam);
void NuCameraSetAxes(NUVEC *axes);
    extern i32 PS2_REZ_W;
    extern i32 PS2_REZ_H;
extern i32 PS2_SREZ_W;
extern i32 PS2_SREZ_H;
void NuCameraSetEx(NUCAMERA *cam, i32 fast);
void NuCameraSetReflect(NUCAMERA *camera, NuCameraReflect *reflect);
void NuCameraSetScissorClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraSetVPortClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraTransformScreen(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenClip(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenVU0(NUVEC4 *screen, NUVEC4 *world, i32 count, NUMTX *matrix);
void NuCameraTransformView(NUVEC *view, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraUnlock(void);

    // ---------------------------------------------------------------------------
    // Display list — faithful helpers + pending stubs
    // ---------------------------------------------------------------------------

    void NuDisplayListBeginCriticalSection(void);
    void NuDisplayListEndCriticalSection(void);

void NuDisplayListAnimateMtls(f32 frame_time);
void NuDisplayListBeginCriticalSection(void);
void NuDisplayListClipSpecials(i32 enabled);
    void DisplayListCreateFxItemPS(void *item, i32 type);
void DisplayListCreateFxList(VARIPTR *buffer, VARIPTR end, i32 count);
i32 NuDisplayListCreateFx(i32 type, i32 priority, i32 layer);
void NuDisplayListCreateMtl(NUMTL *mtl);
    void DisplayListDestroyFxItemPS(void *item);
void NuDisplayListDestroyFx(i32 handle);
void NuDisplayListDestroyMtl(NUMTL *mtl);
void NuDisplayListDrawAll(void);
void NuDisplayListEndCriticalSection(void);
} // extern "C"

// Local helpers matching original static display-list setters (t local symbols)

extern "C" {
void NuDisplayListLinkMtl(nudisplaylist_s *list, NUMTL *mtl);
void NuDisplayListLinkList(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *first, NUDISPLAYLISTITEM *last);
    void *DisplayListCreateFaceonTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *faceon);
    void *DisplayListCreateGeomTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *next, void *tx);

    extern "C++" {
        // Original local helper cloned as DisplayListProcessSkin.isra.36 by GCC.
        // It appends the world transform, skin palette, and geometry calls as one
        // contiguous run in the material display list.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        // Original local helper cloned as DisplayListProcessLightmapped.isra.34.
        // Lightmapped geometry carries an AE/AF/B0 setup command two entries before
        // its geometry command. Keep that setup command adjacent to the dynamic
        // transform and geometry entries when the special is submitted.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

    i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light);
    NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material);
    void WindShear(NUMTX *out, NUMTX *in, i32 wind_scale, i32 wind_speed);

i32 NuDisplayListRndrSpecial(nuhspecial_s *special_handle, NUMTX *mtx, i32 skinned, NUMTX *skin_matrices,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void DisplayListSetFxItemParamPS(void *item, i32 parameter, f32 value, i32 mode);
void NuDisplayListSetFxParam(i32 handle, i32 parameter, f32 value, i32 mode);

    // ---------------------------------------------------------------------------
    // Scene / render-scene
    // ---------------------------------------------------------------------------

void NuDisplaySceneDebug(NUDLDLISTSCENE *scene, i32 flags, i32 depth, i32 *state);

    // ---------------------------------------------------------------------------
    // Frame lifecycle (NuFrameEnd is faithful; neighbours are stubs)
    // ---------------------------------------------------------------------------

    // Per-frame animation/pad update hooks (originals take the frame time).
    void NuPad_Interface_Render(void);
    void NuPadUpdatePads(void);

    // Frame-end callbacks (original bss @0x6bdaec/0x6bdaf0/0x6bdad0).
    extern void (*preRenderFlashingHack)(void);
    extern void (*postRenderFlashingHack)(void);
    extern void (*nuapi_endframe_callbackfn)(void);

    // Faithful transcription of the original frame-end pump. Waits for the
    // target frame interval, ticks material/tex/wind anims, swaps screens
    // via NuRndrSwapScreenEx, then advances nuapi clocks and pad state.

    // ---------------------------------------------------------------------------
    // iOS / platform
    // ---------------------------------------------------------------------------

void NuIOS_AwardAchievement(void);
void NuIOS_CheckCurrentFramebuffer(void);
    void NuIOS_DeallocateSystemRenderbuffer(GLuint renderbuffer);
void NuIOS_DeallocateSystemFramebuffers(void);
void NuIOS_DeallocateSystemRenderbuffer(GLuint);
    // Locale string is owned by the Android/JNI TU; cache only its index here.
extern i32 g_languageIndex;

i32 NuIOS_GetDeviceLanguage(void);
i32 NuIOS_HardwareSupportsRetina(void);
i32 NuIOS_IsLowestEndDevice(void);
i32 NuIOS_IsMidRangeDevice(void);
i32 NuIOS_IsSmallScreen(void);
void NuIOS_RecordFlurryEvent(char *event_name);
void NuIOS_ShowAchievements(void);

    // ---------------------------------------------------------------------------
    // Animation / curves / data
    // ---------------------------------------------------------------------------

void NuAnimBuffAccumulate_3(nuanimbuff_s *buffer, ani3_animheader_s *animation, f32 time, i32 overwrite, f32 blend,
                                i32 first_joint, nuhgobj_s *object, NUVEC *root_translation);
void *NuAnimBuffCreate(i32 max_joints, VARIPTR *buf, VARIPTR);
void NuAnimBuffCreateScratch(nuanimbuff_s *buffer);
void NuAnimBuffDestroyScratch(nuanimbuff_s *buffer);
void NuAnimBuffEvaluateCallback(NUANIMBUFFEVALUATECB callback, void **data, i32 *joints);
void NuAnimBuffEvaluate_3(nuanimbuff_s *buffer, nuhgobj_s *object, NUMTX *matrices, ani3_animheader_s *animation,
                              NUHGOBJROOTFN root_fn, NUVEC *root_translation, void *root_data);
void NuAnimBuffProceduralAnimation(nuanimbuff_s *buffer, nuhgobj_s *object, i32 override_count,
                                       NUJOINTANIM_s *overrides);
void NuAnimCurve2SetApplyToJointTransLoc(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                             nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                             NUJOINTANIM_s *override_anim, NUVEC *root_translation,
                                             NUVEC *locator_translation);
void NuAnimCurve2SetApplyToJoint(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                     nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                     NUJOINTANIM_s *override_anim);
void NuAnimCurve2SetApplyToMatrix_3(ani3_animheader_s *animation, i32 node, f32 frame, NUMTX *matrix);
void *NuAnimCurveCreate(void);
void NuAnimCurveDestroy(void *curve);
f32 *NuAnimCurveExtractAllNodeCurves_3(ani3_animheader_s *anim, i32 node, f32 frame, char *curve_mask);
void NuAnimCurveSetApplyBlendToJoint2(nuanimcurveset_s *first, nuanimtime_s *first_time, nuanimcurveset_s *second,
                                          nuanimtime_s *second_time, f32 blend, nuhgobjjoint_s *joint, NUVEC *scale,
                                          NUVEC *parent_scale, NUMTX *matrix, NUJOINTANIM_s *override_anim);
void NuAnimCurveSetApplyToMatrix(nuanimcurveset_s *set, nuanimtime_s *time, NUMTX *matrix);
void *NuAnimCurveSetCreate(i32 curve_count);
void NuAnimCurveSetDestroy(void *curve_set, i32 destroy_curves);
void NuAnimData2CalcMatrix(nuanimdata_s *animation, i32 node, f32 frame, numtx_s *matrix);
void NuAnimData2CalcTime(nuanimdata2_s *anim, f32 frame, nuanimtime_s *time);
void NuAnimDataCalcTime(void *animation, f32 frame, nuanimtime_s *time);
void *NuAnimDataCreate(i32 chunk_count);
    extern "C++" void NuAnimDataChunkDestroy(nuanimdatachunk_s *chunk);

void NuAnimDataDestroy(void *animation);
i32 NuAnimDataFindVersion(char *path);
void *NuAnimDataLoadBuff(char *path, VARIPTR *buf, VARIPTR *buf_end);
void *NuAnimGetAnimLOD(void *animation, i32 lod);
i32 NuAnimGetUseQuatsFlag(void);
i32 NuAnimNumNodes(void *animation);
i32 NuAnimPopUseQuatsFlag(void);
i32 NuAnimPushSetUseQuatsFlag(i32 enabled);
i32 NuAnimSetUseQuatsFlag(i32 enabled);

    // ---------------------------------------------------------------------------
    // Bridge / file / heap / memory
    // ---------------------------------------------------------------------------

void *NuMemAllocFn(u32 size);
void NuMemFreeFn(void *ptr);
void *NuMemReAllocFn(void *ptr, u32 size);


i32 NuSysDirClose(i32 index);
i32 NuSysDirOpen(void);
i32 NuSysDirRead(void);

    // ---------------------------------------------------------------------------
    // Math / geometry
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Quick-font platform rendering (the generic font run lives in nuqfnt.cpp)
    // ---------------------------------------------------------------------------

VUFNT *NuQFntCreate(NUFNT *font, i32 flags, i32 render_options, VARIPTR *buf, VARIPTR *);
void NuQFntDestroy(VUFNT *font);
void NuQFntEncodeUnicodeString(NUQFNT *font, u16 *text);
NUQFNT_CSMODE NuQFntGetCoordinateSystem(void);
u32 NuQFntGetPrintMode(void);
f32 NuQFntHeightScale(void);
f32 NuQFntLenScale(void);
void NuQFntMove2d(NUQFNT *font, f32 x, f32 y, f32 z);
void NuQFntPopCoordinateSystem(void);
void NuQFntPrint2dU(NUQFNT *font, char *text);
void NuQFntPrint2dW(NUQFNT *font, u16 *text);
void NuQFntPrint3DU(NUQFNT *font, char *text);
void NuQFntPrint3DW(NUQFNT *font, u16 *text);
void NuQFntPrintEx(NUQFNT *font, i32 x, i32 y, i32 alignment, const char *format, ...);
f32 NuQFntPrintLenV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPrintV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPushCoordinateSystem(NUQFNT_CSMODE mode);
void NuQFntSet2d(NUQFNT *font);
void NuQFntSetColour2d(NUQFNT *font, u32 colour);
    void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);
void NuQFntSetPointSize(NUQFNT *font, f32 width, f32 height);
void NuQFntSetPrintMode(u32 mode);
void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);

    // ---------------------------------------------------------------------------
    // Rendering / materials / effects (host has GL paths elsewhere)
    // ---------------------------------------------------------------------------

void NuAccumulationMotionBlurEffect(i32 frames, f32 blend, i32 mode);
void NuAccumulationMotionBlurParams(i32 frames, f32 blend, i32 mode);
    extern nurenderscene_s currentScene;
void NuBackbufferCopy(i32 texture_id);
void NuDeferredShadingRender(void);
void NuDeferredShadingSetParameterf(i32 parameter, f32 value);
void NuDepthOfFieldEffect(f32 strength, f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect1(f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect2(f32 near_distance, f32 far_distance, f32 strength);
void NuDepthOfFieldEffectEx(const NuDepthOfFieldParameters *parameters);
extern i32 speedblur_enabled;
extern f32 NuLightsx;
extern f32 NuLightsy;
void NuLightSpeedBlur(i32 reuse_camera, f32 scale);
void NuLightSpeedBlurOldCameraPos(const NUMTX *camera);
void NuLightSpeedBlurScale(f32 x, f32 y);
extern NULGTARCLASER NuLgtArcLaserData[16];
extern i32 NuLgtArcLaserOldCnt;
void NuLgtArcLaser(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                       f32 bend_amount, i32 colour);
extern i32 NuLgtLaserCnt;
extern i32 NuLgtArcLaserCnt;
extern i32 NuLgtArcLaserFrame;
extern NULGTLASER NuLgtLaserData[64];

void NuLgtLaser(i32 type, f32 width, f32 segment_length, f32 width_wobble, NUVEC *start, NUVEC *delta, u32 colour,
                    f32 end_width, f32 length);
void NuLgtLaserDraw(i32 paused);

void NuLgtSetArcMat(NUMTL *material, f32 u0, f32 v0, f32 u1, f32 v1);
void NuPostBloom(i32, const NuBloomParameters *parameters);
void NuSpecialBurstDrawAt(nuhspecial_s *special, u32 count, NUMTX *matrices, i32 clip);
i32 NuSpecialDrawAt(void *special, NUMTX *mtx);
i32 NuSpecialDrawSmoothSkin(void *special, NUMTX *skin_matrices, NUMTX *world_matrix);
i32 NuSpecialDrawSmoothSkinDwa(void *special, NUMTX *skin_matrices, NUMTX *world_matrix,
                                   DEFORMERWEIGHTSARRAY *blend_values);
i32 NuSpecialDrawWith(void *special, NUMTX *mtx);
void NuSpecialSetDrawMtx(void *special, NUMTX *mtx);

    // ---------------------------------------------------------------------------
    // Light / wind / particles / debris
    // ---------------------------------------------------------------------------

void NuDynamicLightAddRenderScene(NuDynamicLight *light, i32 index, i32 scene_id);
void NuDynamicLightAddShadowCasterScene(NuDynamicLight *light, nugscn_s *scene);
void NuDynamicLightBeginCapture(NuDynamicLight *light, i32 render_set);
NuDynamicLight *NuDynamicLightClone(NuDynamicLight *light, VARIPTR *arena, VARIPTR end);
NuDynamicLight *NuDynamicLightCreate();
void NuDynamicLightDestroy(NuDynamicLight *light);
void NuDynamicLightEndCapture(void);
i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light);
NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material);
f32 NuDynamicLightGetParameterf(NuDynamicLight *light, i32 parameter);
i32 NuDynamicLightGetParameteri(NuDynamicLight *light, i32 parameter);
NUMTX *NuDynamicLightGetProjection(NuDynamicLight *light);
NUMTX *NuDynamicLightGetView(NuDynamicLight *light);
i32 NuDynamicLightIsUsedOnSpecials(NuDynamicLight *light);
void NuDynamicLightLookAt(NuDynamicLight *light, NUVEC *eye, NUVEC *target, NUVEC *up);
void NuDynamicLightResetGeometry(NuDynamicLight *light);
void NuDynamicLightSetDirectional(NuDynamicLight *light, f32 left, f32 right, f32 bottom, f32 top, f32 near_plane,
                                      f32 far_plane);
void NuDynamicLightSetEnabled(NuDynamicLight *light, i32 enabled);
void NuDynamicLightSetParameterf(NuDynamicLight *light, i32 parameter, f32 value);
void NuDynamicLightSetParameteri(NuDynamicLight *light, i32 parameter, i32 value);
void NuDynamicLightSetUsedOnSpecials(NuDynamicLight *light, i32 enabled);
void NuDynamicLightSetupCustomCameraFrustum(NuDynamicLight *light, NUCAMERA *camera, const f32 *splits, i32 count);
i32 NuDynamicLightTestShadowExtrusionExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent,
                                                i32 render_set);
void NuDynamicLightTestShadowExtrusions(NuDynamicLight *light, const VuVec *first, const VuVec *second);
void NuDynamicLightTestShadowExtrusionsExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent);

i32 NuWindLoad(NUWIND *wind, i32 index, char *name, VARIPTR *buffer, VARIPTR buffer_end);

void NuWindSetCurrent(NUWIND *wind, i32 index);

void NuWindUnload(NUWIND *wind, i32 index);
    void NuWindUpdateArray(NUVEC **);

    extern "C" f32 partglobaltime;
void NuPartResetGlobalTime(void);
    // ---------------------------------------------------------------------------
    // Gobj / hierarchy / scene graph
    // ---------------------------------------------------------------------------

void NuHGobjDestroy(nuhgobj_s *object);
void NuHGobjEval(nuhgobj_s *object, i32 override_count, nuhgobjjointoverride_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim(void);
void NuHGobjEvalAnim2(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                          NUJOINTANIM_s *overrides, NUMTX *matrices);
extern i32 ddmaxjoints;
void NuHGobjEvalAnim2Root(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                              NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);

void NuHGobjEvalAnim2Root_3(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                                NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
#include "nu2api/nucore/nucore_plain_types.h"
void NuHGobjEvalAnimBlend(nuhgobj_s *object, void *animation_a, f32 frame_a, void *animation_b, f32 frame_b,
                              f32 blend, i32 override_count, NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                               ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                               NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2Root(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                                   ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                                   NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
void **NuHGobjEvalDwa(i32 render_count, i16 *render_indices, void *animation, f32 frame);
void **NuHGobjEvalDwa2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation, f32 frame);
void **NuHGobjEvalDwaBlend(i32 render_count, i16 *render_indices, void *animation_a, f32 frame_a, void *animation_b,
                               f32 frame_b, f32 blend);
void **NuHGobjEvalDwaBlend2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation_a, f32 frame_a,
                                nuanimdata2_s *animation_b, f32 frame_b, f32 blend);
i32 NuHGobjForceShadowsOnCharacters(i32 enabled);
void NuHGobjFromVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuHGobjJointMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
void NuHGobjPOIMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
i32 NuHGobjReversibleCharacters(i32 enabled);
i32 NuHGobjRndrMtxDwa(nuhgobj_s *object, NUMTX *world_matrix, i32 render_count, i16 *render_indices,
                          NUMTX *joint_matrices, void **blend_values, i32);
    u32 NuWindRand(void);
i32 NuHGobjRndrRandShadowSurfacePoints(nuhgobj_s *object, NUMTX *world_matrix, NUMTX *joint_matrices, i32 count,
                                           NUVEC *positions, i32 exclusion_mask);
void NuHGobjSetClippingRootTrackerOverride(i32 enabled);
void NuHGobjToVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuGCutCharAnimProcess(NUGCUTCHAR_s *character, f32 frame, NUMTX *matrix, i32 *visible, u32 *animation_index,
                               f32 *animation_rate, f32 *blend_time, f32 *animation_start_frame, i32 *layer_mask);
void NuGCutSetCutAudioStream(i32 stream);
    void NuTexAnimRemoveList(void *texture_anims);
    void NuGHGPreRelocateFixupPS(NUGSCN *scene);
    void NuGHGPostRelocateFixupPS(NUGSCN *scene, i32 delta);
NUGSCN *NuGHGRelocate(NUGSCN *scene, VARIPTR *destination);

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

extern NUKEYCHAR NuKeyChar[65];
i32 NuKeyToAscii(u32 key, i32 shifted);
void NuPad_Interface_TouchScreenInput(i32 x, i32 y, i32 prior_x, i32 prior_y, i32 is_down, i32 is_up, i32 is_move,
                                          i32 is_cancelled);
    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    i32 clipTestSphere(NUPORTALSPHERE *sphere, NUFRUSTRUM *frustum);

i32 NuPortalClipTest(NUGSCN *scene, NUVEC *position, f32 radius, i16 room_id);
i32 NuPortalEnabled(i32 enabled);
i32 NuPortalNumRooms(NUGSCN *scene);
void NuPortalResetActive(NUGSCN *scene);
i32 NuPortalRoomClipTest(NUGSCN *scene, i16 room_id);
void NuPortalRoomClipTestAll(NUGSCN *scene, u8 *room_visibility);
i32 NuPortalWhichRoom(NUGSCN *scene, NUVEC *position);
extern i32 VisiSysCameraLock;
extern i32 LoadedOcclusionData;
extern i32 UsingOcclusionData;
extern i32 CurrentViewBox;
extern u8 OcclusionBitArray[800];
extern i32 do_InstTree;
extern i32 do_occlusion;
extern i32 do_visibility;
extern i32 do_octree;

    void NuVisiInstTree(void *, NUGSCN *);
    void NuVisiOcclusion(NuVisibilityResult *);

void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
    DECOMP_ASSERT(sizeof(NuInstanceTreeNode) == 0x0c, "instance tree node stride");
    DECOMP_ASSERT(sizeof(NuInstanceTreeBounds) == 0x20, "instance tree bounds stride");
    DECOMP_ASSERT(sizeof(NuInstanceTree_s) == 0x28, "instance tree size");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, nodes) == 0x14, "instance tree nodes offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, roots) == 0x20, "instance tree roots offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, far_clips) == 0x24, "instance tree far clips offset");
extern NuVisibilityResult * myvisi;
    extern "C++" {
        // Retail only has the IPA-SRA clone (the unused scene and bits arguments dropped).
        static __attribute__((noinline, noclone)) void
        ClipInstTree(NuInstanceTree_s *tree,
                     i32 tree_index) __asm__("_ZL12ClipInstTreeP16NuInstanceTree_siP16nudisplayscene_sPc.isra.92");
    }
void NuVisiInstTree(void *visibility_result, NUGSCN *scene);

void NuVisiOcclusion(NuVisibilityResult *result);
    extern "C++" void OctreeRndr(u8 *visibility, nuoctreenode_s *root, i32 enabled);
void NuVisiOctree(NuVisibilityResult *result);
void NuOcclusionManagerAddOccluderOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix);
void NuOcclusionManagerAddOccluderQuad(const NUVEC *a, const NUVEC *b, const NUVEC *c, const NUVEC *d);
void NuOcclusionManagerAddOccluderSphere(const NUVEC *center, f32 radius);
void NuOcclusionManagerEndFrame(void);
void NuOcclusionManagerInit(u32 capacity, VARIPTR *buffer, VARIPTR buffer_end);
bool NuOcclusionManagerIsEnabled(void);
bool NuOcclusionManagerIsInitialised(void);
i32 NuOcclusionManagerIsOccludedOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix);
i32 NuOcclusionManagerIsOccludedSphere(const NUVEC *center, f32 radius);
void NuOcclusionManagerOnCameraSet(void);
void NuOcclusionManagerRenderStats(void);
void NuOcclusionManagerRenderZPass(void);
void NuOcclusionManagerSetEnabled(i32 enabled);
void NuOcclusionManagerSetOccluderDotProductThreshold(f32 threshold);
void NuOcclusionManagerSetOccluderScreenSpaceThreshold(f32 threshold);
    // ---------------------------------------------------------------------------
    // Viewport
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Strings / conversion / Unicode
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Containers / lists / params
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Debug / error / profiling
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Thread / misc OS
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
void NuPause(i32 paused);
    // ---------------------------------------------------------------------------
    // Spline / online / net / other gameplay support
    // ---------------------------------------------------------------------------

void NuSplineList(void);
    extern void (*nuapi_endframe_callbackfn)(void);
void NuRegisterEndFrameCallBackFn(void (*callback)(void));

    // ---------------------------------------------------------------------------
    // Rendering extras
    // ---------------------------------------------------------------------------

i32 NuStreamInit(void);

    // ---------------------------------------------------------------------------
    // PS2 / Xbox legacy shims
    // ---------------------------------------------------------------------------

} // extern "C"

struct nupad_s;
struct nuframebuffer_s;
struct nushaderobject_s;
union variptr_u;

void NuLgtSetArcMatEx(i32 type, numtl_s *material, f32 u0, f32 v0, f32 u1, f32 v1);

void NuLgtArcLaserEx(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                     f32 bend_amount, i32 colour, i32 flags);

// ==== src/nu2api/nucore/nugcutscene_anim.cpp: all remaining content ====
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/numath/numtx.h"

f32 *NuGCutLocatorCalcMtx_3(NUGCUTLOCATOR_s *locator, numtx_s *mtx, float frame);

extern "C" f32 *NuGCutLocatorCalcMtx(NUGCUTLOCATOR_s *locator, float frame, NUMTX *mtx, nuanimtime_s *time);

char CurveFilterLOCATORScale[] = {6, 7, 8, static_cast<char>(0xff)};
char CurveFilterLOCATOR[] = {6, 7, static_cast<char>(0xff)};
char CurveFilterSFX[] = {3, static_cast<char>(0xff)};

i32 NuGCutLocatorIsVisble_3(NUGCUTLOCATOR_s *locator, float frame, float *scale, float *rate);

extern "C" i32 NuGCutLocatorIsVisble(NUGCUTLOCATOR_s *locator, float frame, nuanimtime_s *time, float *scale,
                                     float *rate);

// ==== src/legoapi/characters/motion/gameanim.cpp: declarations ====
#include "decomp.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "gameapi/edtools/edfile.h"
#include "globals.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/world.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numusic/sfx.h"

#include <float.h>
#include <string.h>

void EvalAnim(nuhspecial_s *special, f32 frame, numtx_s *matrix, i32 include_instance_translation);
i32 UseFallAnim(GameObject_s *object);
i32 GetDefaultIdle(GameObject_s *object);
i32 SetProtocolDroidFallAnim(GameObject_s *object);
// Retail uses translation-unit-local helpers with compiler-inferred private
// calling conventions; keep their shared animation-mode call sites intact.
static void MoveAnim_Manage(GameObject_s *object, f32 movement_speed, i32 allow_tiptoe, i32 weapon_variant);
static void MoveAnim_Check(GameObject_s *object);
static void JumpAnimCode(GameObject_s *object);
static bool JumpAnim_HasAction(const GameObject_s *object, i16 action);
static GAMECHARACTERDATA *GetGameCharacterData(GameObject_s *object);
void UpdateCharacterIdle(GameObject_s *object);
void AutoWeaponOnOff(GameObject_s *object);
void AddFootSteps(GameObject_s *object);
f32 PodSprint_InStartCountdown(WORLDINFO_s *world);
i32 PodRace_InStartCountdown(WORLDINFO_s *world);
void SetPodMergeAnims(ANIMPACKET_s *packet, i32 index);
extern "C" i16 id_ANAKINSPODGREEN;
extern "C" void PlaySfxByIdAndSetVolume(i32 sfx_id, NUVEC *position, f32 volume);
i32 MatrixReflection(NUMTX *matrix, i32 axis, f32 plane, f32 height, NUMTX *result);

extern i16 id_BODYGUARD;
extern i16 id_IMPERIALGUARD;
extern i16 id_YODA;
extern i16 id_YODAGHOST;
extern i16 id_GONKDROID;
extern i16 id_JUMBOHOMINGDROID;
extern i16 id_GAMORREANGUARD, id_PRINCESSLEIASLAVE, id_LAMASU, id_TAUNWE;
extern i16 id_PENGUIN, id_UMBRELLA, id_GLIDEPACK;
extern f32 hub_jabbaawake;

#include "legoapi/characters/motion/gameanim_types.h"






void Animate_ATAT(GameObject_s *object);

void Animate_POD(GameObject_s *object);


void Animate_JEDI(GameObject_s *object);


void AnimatePlayer(GameObject_s *object);

void Animate_BEAST(GameObject_s *object);

void Animate_BARMAN(GameObject_s *object);

void Animate_CANNON(GameObject_s *object);

void Animate_WALKER(GameObject_s *object);

void Animate_CRITTER(GameObject_s *object);

void Animate_WEIRDO(GameObject_s *object);

void Animate_DEFAULT(GameObject_s *object);

extern i32 NeedsPretendAnim(GameObject_s *object);

void Animate_VEHICLE(GameObject_s *object);

void Animate_PROTOCOL(GameObject_s *object);

void Animate_DROIDEKA(GameObject_s *object);

void GameAnimSet_Draw(GAMEANIMSET_s &set);

i32 GameAnimSet_Play(GAMEANIMSET_s *set, float speed, i32 evaluate_state);

i32 GameAnimSet_Stop(GAMEANIMSET_s *set);

void Animate_ASTROMECH(GameObject_s *object);

void Animate_CHARACTER(GameObject_s *object);

void Animate_GEONOSIAN(GameObject_s *object);

i32 GameAnimSet_Reset(GAMEANIMSET_s *set);

void Animate_HOVERDROID(GameObject_s *object);

GAMEANIMSET_s *GameAnimSet_Create(variptr_u *buf, variptr_u *buf_end, GAMEANIMOBJPOOL_s *object_pool,
                                  GAMEANIMSYS_s *system);

void Animate_BATTLEDROID(GameObject_s *object);

void Animate_SPEEDERBIKE(GameObject_s *object);

i32 GameAnimSet_Playing(GAMEANIMSET_s *set);

void GameAnimSet_EvalAnim(GAMEANIMSET_s *set);

GAMEANIMOBJ_s *GameAnimSet_AddObject(GAMEANIMSET_s *set, nuhspecial_s *special, float start_frame, float end_frame,
                                     i32 append);

i32 GameAnimSet_JumpToEnd(GAMEANIMSET_s *set);

void GameAnimSet_SetOffset(GAMEANIMSET_s *set, NUVEC *offset);

f32 GameAnimSet_GetAnimPos(GAMEANIMOBJ_s *object);

void GameAnimSet_SetAnimPos(GAMEANIMOBJ_s *object, float position);

void GameAnimSet_SetTFactor(GAMEANIMSET_s *set, float factor);

void Animate_REPUBLICGUNSHIP(GameObject_s *object);

i32 GameAnimSet_JumpToStart(GAMEANIMSET_s *set);

void Animate_SUPERBATTLEDROID(GameObject_s *object);

void GameAnimSet_RemoveObject(GAMEANIMSET_s *set, GAMEANIMOBJ_s *object);

void GameAnimSet_ScaleFParam1(GAMEANIMSET_s *set, float scale);

i32 GameAnimSet_SetRepeating(GAMEANIMSET_s *set, i32 repeating);

void GameAnimSet_EvaluateState(GAMEANIMSET_s *set);

i32 GameAnimSet_GetAveragePos(GAMEANIMSET_s *set, NUVEC *position, i32 frame_selection, i32 include_animated,
                              i32 include_static);

GAMEANIMSET_VISIBILITY GameAnimSet_GetVisibility(GAMEANIMSET_s *set);

void GameAnimSet_JumpToAnimPos(GAMEANIMSET_s *set, float position);

void GameAnimSet_RemoveSpecial(GAMEANIMSET_s *set, nuhspecial_s *special);

void GameAnimSet_SetVisibility(GAMEANIMSET_s *set, i32 visibility);

void GameAnimSet_DrawReflection(GAMEANIMSET_s *set, i32 axis, float offset, numtx_s *matrix);

GAMEANIMOBJ_s *GameAnimSet_AddObjectByName(GAMEANIMSET_s *set, nugscn_s *scene, char *name, float start_frame,
                                           float end_frame, i32 append, GIZMOSYS_s *gizmo_sys, char *prefix,
                                           char *suffix);

void GameAnimSet_AddToSystemList(GAMEANIMSET_s *set);

f32 GameAnimSet_AutoSetReflectY(GAMEANIMSET_s *set, nuvec_s *position, numtx_s *matrix);

f32 GameAnimSet_GetCurrentFrame(GAMEANIMSET_s *set);

GAMEANIMOBJPOOL_s *GameAnimSet_CreateObjectPool(variptr_u *buf, variptr_u *buf_end, i32 object_data_size,
                                                i32 capacity);

i32 GameAnimSet_IsAnimationReset(GAMEANIMSET_s *set);

void GameAnimSet_RemoveAllObjects(GAMEANIMSET_s *set);

i32 GameAnimSet_GetCentreAndRadius(GAMEANIMSET_s *set, NUVEC *centre, f32 *radius, i32 frame_selection,
                                   i32 include_animated, i32 include_static);

f32 GameAnimSet_GetCompletionRatio(GAMEANIMSET_s *set);

void GameAnimSet_RemoveFromSystemList(GAMEANIMSET_s *set);

i32 ANI_SimpleAni3PlayerV4Joint_Blend_Quat3(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, f32 blend,
                                            i32 joint_count, i32 first_joint, NUVEC *root_translation);
i32 ANI_SimpleAni3PlayerV4Joint_Blend_Quat3W(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, f32 blend,
                                             i32 joint_count, i32 first_joint, NUVEC *root_translation);

extern "C" {

    i32 ANI_SimpleAni3PlayerV4Joint_EulerQuat(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, i32 joint_count,
                                              i32 first_joint);
    i32 ANI_SimpleAni3PlayerV4Joint_Blend_EulerQuat(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, f32 blend,
                                                    i32 joint_count, i32 first_joint, NUVEC *root_translation);

void ANI_FixUpAddrs(ani3_animheader_s *anim, isize delta, i32);

    // Original @0x2c17d0. The non-quaternion ANI4 player uses a compact
    // four-samples-per-word curve stream. Only groups enabled by the node's
    // translation/rotation/scale flags occupy space in that stream.

f32 GetInstAnimEndFrame(nugscn_s *scene, nuinstanim_s *instance_animation);

extern NUJOINTPROCANIMFN JointProcAnimFn;

void SetProceduralAnimationFn(void *function);

i32 StateAnimEvaluate(StateAnim *state, u8 *index, u8 *value, f32 frame);

i32 StateAnimEvaluate2(StateAnim *state, u8 *index, char *value, f32 frame);

void NuSpecialSetInstAnimTime(nuhspecial_s *special, f32 frame);

StateAnim *StateAnimFixPtrs(StateAnim *state, isize delta);

} // extern "C"

void SetAnimFrame(nuhspecial_s *special, float frame);

i32 GetAnimDirection(nuinstanim_s *animation);

i32 FindTexAnimFromMtl(nugscn_s *scene, numtl_s *material);


void InitTexAnimScripts(char **names);

i32 GizmoFileReadGameAnimSet(GAMEANIMSET_s *set, void *world_ptr,
                             void (*read_object_data)(GAMEANIMOBJ_s *, unsigned char), unsigned char version,
                             char *prefix, char *suffix);

void EvalAnim(nuhspecial_s *special, float frame, numtx_s *matrix, i32 include_instance_translation);

void EvalAnim2(nuhspecial_s *special, float frame);

void GameAnimSys_AllocateLevelProgressData(variptr_u *buf, variptr_u *buf_end, i32 capacity, i32 level_count);

u8 *GameAnimSys_GetProgressData(i32 index);

void GameAnimSys_StoreProgress(GAMEANIMSYS_s *system, i32 index);

void GameAnimSys_ReStoreProgress(GAMEANIMSYS_s *system, i32 index);

GAMEANIMSYS_s *GameAnimSys_Create(variptr_u *buf, variptr_u *buf_end);

// ==== src/legoapi/ai/core/ai_sys_stubs.cpp: all remaining content ====
#include "editor/edpath.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "editor/aieditor_state.h"
#include "editor/antinode_editor_private.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "gameapi/edtools/edfile.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufilepak.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nuplane.h"

#include <stdio.h>
#include <float.h>
#include <string.h>


extern "C" f32 (*GetCharacterGoalSpeedFn)(APIOBJECT *);

static __used__ i32 RowMoveWander(AIGROUP *group, AIROW *row, AIROW *previous, APIOBJECT *leader);

static __used__ i32 RowMoveTowards(AIGROUP *group, AIROW *row, AIROW *previous, APIOBJECT *leader);

static void FormationMove(AIGROUP *group, i32 (*move)(AIGROUP *, AIROW *, AIROW *, APIOBJECT *));

static void AISysCheckAntinode_Rectangle(APIOBJECT *object, AIANTINODE *antinode, NUVEC *difference, f32 radius);

static void AISysCheckAntinode_Ellipse(APIOBJECT *object, AIANTINODE *antinode, NUVEC *difference, f32 radius);

static void AISysCheckAntinode_Circle(APIOBJECT *object, AIANTINODE *antinode, NUVEC *difference, f32 radius);

extern "C" {
extern void (*checkantinodefns[3])(APIOBJECT *, AIANTINODE *, NUVEC *, f32);
extern char * AiLevelPathName;
extern AISCRIPTPROCESS * pSetStateDebugee;
extern SCRIPTPROCESSFIRSTTIMEACTION * ScriptProcessFirstTimeActionFn;
    extern f32 default_path_heighttol;
    extern u8 default_activate_difficulty;
    extern u8 default_min_n_respawns;
    extern u8 default_max_n_respawns;
    extern f32 default_min_t_respawn;
    extern f32 default_max_t_respawn;
    extern f32 default_stagger_start;
    extern void (*checkantinodefns[3])(APIOBJECT_s *, AIANTINODE_s *, NUVEC *, f32);
}

void AIPathCalcExtents(AIPATH *path);
i32 AIPathCheckExtents(AIPATH *path, NUVEC *position);
void AIMoveToDestination(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);
void AIRetreatFromDestination(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);
void AICircle(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);
void AIWander(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);
void AIMoveToDestinationAvoidingCamera(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);
void AIMoveDirectlyToDestination(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);

typedef i32 MIDSPECIALMOVE(AISYS *system, AIPACKET *packet, APIOBJECT *object);
extern MIDSPECIALMOVE *MidSpecialMoveFn;

extern "C" i32 AISysSetLevelPath(AISYS *system, char *path_name);
extern "C" void (*GameAISYSRebuildFromEditorDataFn)(AISYS *, VARIPTR *, VARIPTR *);
extern "C" void AISysFindRoute(AIPACKET *packet);
extern "C" void AISysCharacterSetPath(AIPACKET *packet, AIPATH *path);
extern "C" void AISysCharacterSetPathCnx(AIPACKET *packet, NUVEC *position, AIPATHCNX *connection, i32 direction);

extern "C" void AIPathNodeUpdatePos(AISYS *system, AIPATH *path, AIPATHNODE *node);






#include "legoapi/ai/core/ai_pathgeometry.h"

static u32 AISysCharacterTestPathCnx(AISYS *system, APIOBJECT *object, AIPACKET *packet, AIPATHCNX *connection,
                                     i32 direction, f32 *nearest_distance);










extern "C" {

    AIPATHNODE *AIPathFindNode(AISYS *system, AIPATH *path, char *name);

void AIMoveInstruction(AIPACKET *packet, NUVEC *destination, f32 stopping_distance, AIPATHINFO *path_info, i32 mode,
                           f32 movement_parameter);

void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from_name, char *to_name, i32 *direction);

AILOCATOR *AIPathFindLocator(AISYS *sys, char *name);

AILOCATORSET *AIPathFindLocatorSet(AISYS *system, char *name);

AIPATHNODE *AIPathFindNode(AISYS *system, AIPATH *path, char *name);

void AIPathNodeUpdatePos(AISYS *system, AIPATH *path, AIPATHNODE *node);

#include "legoapi/ai/core/ai_sys_stubs_types.h"
    DECOMP_ASSERT(sizeof(RebuildEditorCreature) == 0xac, "editor creature rebuild stride");
    DECOMP_ASSERT(offsetof(RebuildEditorCreature, path_check) == 0x38, "editor creature path offset");
    DECOMP_ASSERT(offsetof(RebuildEditorCreature, activation_area) == 0x6c, "editor creature area offset");
    DECOMP_ASSERT(offsetof(RebuildEditorCreature, activation) == 0x8f, "editor creature activation offset");

void AISYSRebuildFromEditorData(void);

void AIScriptForceParamReEval(AISCRIPTPROCESS *processor);

char *AIScriptNameFromIx(AISYS *system, i32 index);

} // extern "C"


#if defined(__i386__)
#define AI_SYS_STUBS_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define AI_SYS_STUBS_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif

// Retail keeps only the IPA-SRA clone, without the unused object and elapsed time.
static AI_SYS_STUBS_CLONE_ATTR2 AICONDITION *AIScriptProcessStateConditions(AISYS *system, NULISTHDR *conditions,
                                                                           AIPACKET *packet,
                                                                           AISCRIPTPROCESS *processor,
                                                                           i32 stack_index)
    __asm__("_ZL30AIScriptProcessStateConditionsP7AISYS_sP11nulisthdr_sP11APIOBJECT_s"
            "P10AIPACKET_sP17AISCRIPTPROCESS_sfi.isra.77");

static AICONDITION *AIScriptProcessStateConditions(AISYS *system, NULISTHDR *conditions, AIPACKET *packet,
                                                   AISCRIPTPROCESS *processor, i32 stack_index);

extern "C" {

void AIScriptProcess(AISYS *system, APIOBJECT *object, AIPACKET *packet, AISCRIPTPROCESS *processor, f32 elapsed);

void AIScriptSetLevelPath(char *path);

void *AISysBufferAlloc(VARIPTR *cursor, VARIPTR *buf_end, u32 size);

void AISysCharacterMovement(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);

void AISysCreatureAntinodeInteraction(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *);

void AISysCreatureInteraction2D(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *immovable,
                                    f32 delta_time);

void AISysCreatureInteraction3D(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *immovable,
                                    f32 delta_time);

AIAREA *AISysFindArea(AISYS *sys, char *name);

AIPATH *AISysFindPath(AISYS *sys, char *name);

void AISysGetCharacterPathPos(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 checks, i32 ground);

u32 AISysGetPathColour(i32 index);

i32 AISysGetPathColourCount(void);

void AISysGetPathPos(AISYS *system, NUVEC *position, AIPATHINFO *info, AIPATH *path, i32 checks);

void AISysGetPathPosEx(AISYS *system, NUVEC *position, AIPATHINFO *info, AIPATH *path, i32 checks,
                           i32 search_all_paths, f32 *nearest_distance_squared);

void *AISysLoadEx(void *buffer, void *buffer_end, i32 storage_size, void *scene, char *directory, char *name,
                      char *parameter, char *load_directory);

void AISysProcess(AISYS *system, APIOBJECT *player_1, APIOBJECT *player_2);

void AISysProcessCharacter(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 checks, f32 elapsed,
                               i32 use_three_dimensions, i32 process_ai);

i32 AISysSetLevelPath(AISYS *system, char *path_name);

void AISysUpdateCharacterPathPos(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 checks, f32 elapsed);

void AddToAIGroup(AIGROUP *group, APIOBJECT *object);

extern f32 AiRndrCullDistance;
void SetAiRndrCullDistance(f32 distance);

extern AIEDITOR_RENDER_STATE * aieditor;
    void NuRndrLine3dDbg(f32, f32, f32, f32, f32, f32, i32);
void AiRndrLine3d(NURND_VERTEX3D *vertices, NUMTL *, NUMTX *matrix);
void AiRndrLine3dDbg(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, u32 colour);

void AiSysOnlyUsePakFile(i32 enabled);

void AiSysSetStateDebugee(AISCRIPTPROCESS *processor);

void AiSysUsePackFile(i32 enabled);

void CalculateLocatorDirection(i32 direction, numtx_s *matrix, NUVEC *out);

AIGROUP *CreateAIGroup(AISYS *system, i32 count_across, f32 x_spacing, f32 z_spacing, f32 max_speed);

void DestroyAIGroup(AIGROUP *group);

void FindAIDirectionedRandomPointOnNetwork2D(AIPACKET *packet, NUVEC *position, NUVEC *direction, f32 distance,
                                                 f32 offset, i32 flags, NUVEC *result);

void FollowAPIObject(APIOBJECT *object, APIOBJECT *target, i32 flags, f32 movement_parameter);

    typedef void AIEDITORCALLBACK(void);
    typedef void AIEDITORREBUILDCALLBACK(AISYS *, VARIPTR *, VARIPTR *);

extern AIEDITORCALLBACK * AIPathDeletedFn;
extern AIEDITORPATHNODECALLBACK * AIPathNodeDeletedFn;
extern AIEDITORPATHNODECALLBACK * AIPathNodeMovedFn;
extern AIEDITORREBUILDCALLBACK * GameAISYSRebuildFromEditorDataFn;
extern AIEDITORCALLBACK * GameAISaveFn;

void InitFn_AIPathDeleted(AIEDITORCALLBACK *function);

void InitFn_AIPathNodeDeleted(AIEDITORPATHNODECALLBACK *function);

void InitFn_AIPathNodeMoved(AIEDITORPATHNODECALLBACK *function);

void InitFn_GameAISYSRebuildFromEditorData(AIEDITORREBUILDCALLBACK *function);

void InitFn_GameAISave(AIEDITORCALLBACK *function);

void InitFn_GameParamToFloat(GAMEPARAMTOFLOAT *function);

void InitFn_ScriptProcessFirstTimeAction(SCRIPTPROCESSFIRSTTIMEACTION *function);

void LEGO_AISysCreatureInteraction2D(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *immovable,
                                         f32 delta_time);

void *QueryLocalMessage(void *context, void *message);

} // extern "C"

// ==== src/nu2api/nucore/nugcutscene.cpp: all remaining content ====
#include "nu2api/nucore/nugcutscene.h"

// Callback storage is declared in reverse linked-BSS order because the
// original GCC 4.7 -O3 object emits these definitions in reverse source order.
extern "C" {
    NUGCUTSCENEGETHGOBJFN NuCutSceneGetHGObj = NULL;
    NUGCUTSCENECHARACTERPROCESSFN NuCutSceneCharacterProcess = NULL;
    NUGCUTSCENECHARACTERRENDERFN NuCutSceneCharacterRender = NULL;
    NUGCUTSCENECHARACTERRELEASEFN NuCutSceneCharacterRelease = NULL;
    NUGCUTSCENECHARACTEREVALFN NuCutSceneCharacterEval = NULL;
    NUGCUTSCENEFINDCHARACTERSFN NuCutSceneFindCharacters = NULL;
    void (*NuCutSceneDestroyCharacters)(NUGCUTSCENE_s *) = NULL;
    NUGCUTSCENERESETCHARACTERSFN NuCutSceneResetCharactersFn = NULL;
    NUGCUTSCENECHARACTERCREATEDATAFN NuCutSceneCharacterCreateData = NULL;
    NUGCUTSCENECHARACTERDESTROYDATAFN NuCutSceneCharacterDestroyData = NULL;
    NUGCUTSCENERIGIDCOLLISIONCHECKFN NuCutSceneRigidCollisionCheck = NULL;
    NUGCUTSCENERIGIDPOSTRENDERFN NuCutSceneRigidPostRender = NULL;
    NUGCUTSCENESFXFIXUPFN NuCutSceneSFXFixUp = NULL;
    NUGCUTSCENESFXUPDATEFN NuCutSceneSFXUpdate = NULL;
    NUGCUTSCENEREQUESTSFXFN NuCutSceneRequestSFX = NULL;

void NuSetGetHGObjFromIndxFn(NUGCUTSCENEGETHGOBJFN function);

void NuSetCutSceneCharacterProcessFn(NUGCUTSCENECHARACTERPROCESSFN function);

void NuSetCutSceneCharacterRenderFn(NUGCUTSCENECHARACTERRENDERFN function);

void NuSetCutSceneCharacterEvalFn(NUGCUTSCENECHARACTEREVALFN function);

void NuSetCutSceneCharacterReleaseFn(NUGCUTSCENECHARACTERRELEASEFN function);

void NuSetCutSceneFindCharactersFn(NUGCUTSCENEFINDCHARACTERSFN function);

void NuSetCutSceneDestroyCharactersFn(void (*callback)(NUGCUTSCENE_s *));

void NuSetCutSceneResetCharactersFn(NUGCUTSCENERESETCHARACTERSFN function);

void NuSetCutSceneCharacterCreateDataFn(NUGCUTSCENECHARACTERCREATEDATAFN function);

void NuSetCutSceneCharacterDestroyDataFn(NUGCUTSCENECHARACTERDESTROYDATAFN function);

void NuSetCutSceneRigidCollisionCheckFn(NUGCUTSCENERIGIDCOLLISIONCHECKFN callback);

void NuSetCutSceneSFXFixUpFn(NUGCUTSCENESFXFIXUPFN function);

void NuSetCutSceneSFXUpdateFn(NUGCUTSCENESFXUPDATEFN function);

void NuSetCutSceneRequestSFXFn(NUGCUTSCENEREQUESTSFXFN function);

void NuSetCutSceneRigidPostRenderFn(NUGCUTSCENERIGIDPOSTRENDERFN function);
} // extern "C"

// ==== src/legoapi/cutscenes/cutscenes.cpp: declarations ====
#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/audio/sfx.h"
#include "globals.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/core/screen.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/nusound/nusound.h"

#include <string.h>
#include <stdio.h>
#include "legoapi/world/mission.h"
#include "legoapi/world/levels/episode.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"

extern i32 ACTIVECUTCOUNT;
extern "C" i32 CUTDRAWWORLD;
extern "C" i32 Paused;
extern i32 CUTCAM;
extern "C" i32 CutSceneWaiting;
extern i32 CUTCAMONLY;
extern i32 cut_waiting_for_new_level;
extern volatile i32 waiting_for_level;
extern "C" i32 newlevel_resumecutaudio;
extern CUTSYS *CS_cutsys;
extern FadeSystem FadeSys;
extern CUTSCENESYS * CutSceneSys;

static void CutScene_DrawCharacter(instNUGCUTSCENE_s *, NUGCUTSCENE_s *, instNUGCUTCHAR_s *, NUGCUTCHAR_s *, f32, i32);
static void CutScene_EvalCharacter(instNUGCUTSCENE_s *, NUGCUTSCENE_s *, instNUGCUTCHAR_s *, NUGCUTCHAR_s *, f32);
static void CutScene_FindCharacters(NUGCUTSCENE_s *);
static void CutScene_ResetCharacters(instNUGCUTSCENE_s *);
static void CutScene_RigidPostRender(NUGCUTRIGID_s *, instNUGCUTRIGID_s *, NUMTX *);
static void CutScene_CreateCharacterInstance(NUGCUTCHAR_s *, instNUGCUTCHAR_s *, variptr_u *);

extern "C" {
    i32 instNuGCutSceneIsFinished(instNUGCUTSCENE_s *instance);
    void instNuGCutScenePause(instNUGCUTSCENE_s *, u8);
    void instNuGCutSceneReset(instNUGCUTSCENE_s *);
    void instNuGCutSceneStart(instNUGCUTSCENE_s *);
    void instNuGCutSceneStop(instNUGCUTSCENE_s *);
    void instNuGCutSceneDestroy(instNUGCUTSCENE_s *);
    void NuGCutSceneSysRender(i32);
    void NuGCutSceneSysUpdate(i32, i32, f32);
}

void SetLevelLights(void *, f32);
void NewLevelFromMenu(LEVELDATA_s *, i32, i32, i32);
void FindAndSetLights(NUVEC *, f32, void *);
void SetZeroLights(void);
void GameFog_Reset(void);
void EnableShadowMapRendering(i32);
void ResetShadowMapRendering(void);
i32 MatrixReflection(NUMTX *, i32, f32, f32, NUMTX *);
f32 FindReflectionNoPlatforms(NUVEC *);
extern "C" i32 NewShadowOnPlatform(void);
void FindAnglesZX(NUVEC *, u16 *, u16 *);
void CharScene_Draw(WORLDINFO_s *, i32, NUMTX *, NUMTX *);
i32 qrand(void);
void NewRumbleAllPlayers(f32, f32, i32, i32);
f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);
extern "C" void APITransparentCharDraw(nuhgobj_s *, NUMTX *, i32, i16 *, NUMTX *, void **, i32);
extern "C" void instNuGCutLocatorUpdate(instNUGCUTSCENE_s *, NUGCUTLOCATORSYS_s *, instNUGCUTLOCATOR_s *,
                                        NUGCUTLOCATOR_s *, f32, NUMTX *, i32);
CUTSCENEPLAYERCLIP *CutScenePlayer_Active(void);
void CutScenePlayer_SetObjects(CUTINFO *);
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

extern i32 CutInstEndCount;
extern i32 NewCutInfoCount;
extern u8 cutskip_dontplaylevelintro;
extern i32 CUTNOFOG;
extern i32 CUTSKIPLOCK;
extern f32 LevelNameMul;
extern f32 LevelNameTime;

extern "C" i32 drawcharactermodel_nobsa;
extern "C" i32 NewMode;
void GameAudio_PlaySfx(i32, nuvec_s *, i32, i32);

extern "C" {
    void PauseGameCut(void);
    void PlaySfxById(i32 sfx_id, nuvec_s *position);
    extern instNUGCUTSCENE_s *cutscene_load_instance;
    void instNuGCutSceneServiceLoad(void);
    void NuGCutSceneSysPostBackgroundLoad(void);
}



// Retail only keeps the split-out body of the IPA-SRA clone (the unused
// cutscene index dropped), called with world and cut in registers.
#if defined(__i386__)
#define CUTSCENE_START_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define CUTSCENE_START_ATTR __attribute__((noinline, noclone))
#endif
static CUTSCENE_START_ATTR i32 CutScene_StartBody(WORLDINFO_s *world, CUTINFO *cut)
    __asm__("_ZL14CutScene_StartP11WORLDINFO_sP7CUTINFOi.isra.24.part.25");


void CutScenes_End();

void CutScenes_Draw(WORLDINFO_s *world);

void CutScenes_Stop(CUTSYS *system);

void CutScenes_Reset(WORLDINFO_s *world);

void CutScenes_Start(WORLDINFO_s *world);

void CutScenes_Update(WORLDINFO_s *world, i32 paused);

CUTINFO *CutScene_Find(CUTSYS *cutscene_system, char *name);

void CutScenes_Destroy(CUTSYS *system);

void *CutScene_FindInst(CUTSYS *system, char *name);

i32 CutScene_HasPlayed(CUTINFO *cut);

void CutScene_SnapToEnd(CUTINFO *cut);

void CutScene_StartAudio();

i32 CutScene_IsSkippable(CUTINFO *cut);

i32 CutScene_StartFn_LSW(CUTINFO *cut);

extern "C" f32 *NuGCutLocatorCalcMtx(NUGCUTLOCATOR_s *, f32, NUMTX *, nuanimtime_s *);
extern "C" i32 NuGCutLocatorIsVisble(NUGCUTLOCATOR_s *, f32, nuanimtime_s *, f32 *, f32 *);
extern "C" void NuAnimData2CalcTime(nuanimdata2_s *, f32, nuanimtime_s *);



void CutScenes_InitSystem(CUTSCENESYS *system);

extern i32 dco_id;
extern i32 dco_reflectaxis;
extern i32 dco_wearinghat;
extern f32 dco_reflectcoord;
extern GAMECHARACTERDATA_s *dco_gcdata;
extern CHARACTERMODEL_s *dco_cmodel;







// The loop re-reads the active cut through CutStopInfo after each SmartTextEx call.
#define CUT_STOP static_cast<CUTINFO *>(CutStopInfo)
void CutScene_DrawSubtitles();

void CutScene_StoppedFn_LSW(CUTINFO *cut);

void CutScenes_BGLoadManager();

void CutScenes_ConfigureList(char *filename, variptr_u *buffer, variptr_u);

void CutScene_PreUpdateFn_LSW(CUTINFO *cut);

void CutScene_PostUpdateFn_LSW();

i32 CutScene_PlayingOrRequested(CUTINFO *cut);

i32 CutScene_ReplaceCharacterModelFn_LSW(CUTINFO *cut, NUGCUTCHAR_s *character);

CUTINFO *NewCutScene(CUTINFO *cut, CUTSYS *system, char *name, i32);

void ResetScene(nugscn_s *scene, SCENEPROGRESS_s *progress);

void NextStatusStage(STATUSPACKET_s *);
void FinishStatusPacket(i32);
f32 StatusIconsOnOff(f32);
extern f32 icon_y;

void Exit_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed);

void Fade_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed);

extern "C" void *NuAnimData2FixPtrs(void *, isize, isize, i32);
extern "C" StateAnim *StateAnimFixPtrs(StateAnim *, isize);

NUGCUTSCENE_s *RelocateCutScene(NUGCUTSCENE_s *source, variptr_u *buffer);

extern i32 STATUS_R;
extern i32 STATUS_G;
extern i32 STATUS_B;
extern f32 iconalphaoverride;
extern i16 tTRUEJEDI, tSTORY, tFREEPLAY;
extern i16 tLEVELCOMPLETE, tFREEPLAYUNLOCKED, tSUPERSTORYCOMPLETE, tBONUSUNLOCKED;
extern i16 tEPISODEVUNLOCKED, tEPISODEVIUNLOCKED, tCHALLENGECOMPLETE, tOUTOFTIME;
extern i16 tYOUDIDNTCOLLECTALL10CANISTERS, tMISSIONCOMPLETE, tSTORYCLIPSUNLOCKED;
extern i16 tEPISODEICOMPLETE, tEPISODEIICOMPLETE, tEPISODEIIICOMPLETE;
extern i16 tEPISODEIVCOMPLETE, tEPISODEVCOMPLETE, tEPISODEVICOMPLETE;
f32 getFinishedStatusAlpha(STATUSPACKET_s *);
void IncreaseScore(u32 *, u64, i32);
void NewStatusRumbleBuzz(i32, f32, f32, i32);
void Text_MakeScore(u32, char *);
void AddFancyMessage(char *, f32, f32, f32, f32, i32, i32);
void *AddGameMessage(char *, nuvec_s *, f32, nuvec_s *, f32, u8, u8, u8, u32, f32);
extern "C" void PlaySfx(char *, nuvec_s *);
extern "C" void Text3D(char *, f32, f32, f32, f32, f32, f32, u32, u8, u8, u8);
i32 DrawPanel3DObjectNoAlpha(f32, f32, f32, f32, f32, f32, u16, u16, u16, nuhspecial_s *, i32);

void GoldBrick_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active);

void GoldBrick_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet);

void GoldBrick_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed);

void LevelComplete_LSW_Skip(STATUS_STAGE_s *stage, STATUSPACKET_s *packet);

void SetDrawGoldBrick(STATUSPACKET_s *, i32);
void NewStatusRumbleBuzz(i32, f32, f32, i32);
extern "C" void PlaySfx(char *, nuvec_s *);

void LevelComplete_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed);

// ==== functions in original address order ====

static void copyAnims(NUGCUTSCENE_s *destination, NUGCUTSCENE_s *source) {
    NUGCUTCAMERASYS_s *destination_camera = destination->camera_system;
    NUGCUTCAMERASYS_s *source_camera = source->camera_system;
    if (static_cast<u8>(destination->version) > 4) {
        destination_camera->focus_animation = source_camera->focus_animation;
        destination_camera->focus_state_animation = source_camera->focus_state_animation;
    }
    if (source_camera->animation != NULL) {
        destination_camera->animation = source_camera->animation;
    }
    destination_camera->state_animation = source_camera->state_animation;

    NUGCUTLOCATORSYS_s *source_locators = source->locator_system;
    if (source_locators != NULL) {
        NUGCUTLOCATORSYS_s *destination_locators = destination->locator_system;
        NUGCUTLOCATOR_s *source_locator = source_locators->locators;
        if (source_locator != NULL) {
            NUGCUTLOCATOR_s *destination_locator = destination_locators->locators;
            for (i32 i = 0; i < source_locators->locator_count; ++i) {
                if (source_locator[i].animation != NULL) {
                    destination_locator[i].animation = source_locator[i].animation;
                }
            }
        }
    }

    NUGCUTRIGIDSYS_s *source_rigids = source->rigid_system;
    if (source_rigids != NULL) {
        NUGCUTRIGIDSYS_s *destination_rigids = destination->rigid_system;
        NUGCUTRIGID_s *source_rigid = source_rigids->rigids;
        if (source_rigid != NULL) {
            NUGCUTRIGID_s *destination_rigid = destination_rigids->rigids;
            for (i32 i = 0; i < source_rigids->count; ++i) {
                if (source_rigid[i].animation != NULL) {
                    destination_rigid[i].animation = source_rigid[i].animation;
                }
                destination_rigid[i].state_animation = source_rigid[i].state_animation;
            }
        }
    }

    NUGCUTCHARSYS_s *source_characters = source->character_system;
    if (source_characters != NULL) {
        NUGCUTCHARSYS_s *destination_characters = destination->character_system;
        NUGCUTCHAR_s *source_character = source_characters->characters;
        if (source_character != NULL) {
            NUGCUTCHAR_s *destination_character = destination_characters->characters;
            for (i32 i = 0; i < source_characters->character_count; ++i) {
                if (source_character[i].animation != NULL) {
                    destination_character[i].animation = source_character[i].animation;
                }
                if (source_character[i].face_animation != NULL) {
                    destination_character[i].face_animation = source_character[i].face_animation;
                }
                if (source_character[i].extra_animation != NULL) {
                    destination_character[i].extra_animation = source_character[i].extra_animation;
                }
            }
        }
    }

    NUGCUTTRIGGERSYS_s *source_triggers = source->trigger_system;
    if (source_triggers != NULL) {
        NUGCUTTRIGGERSYS_s *destination_triggers = destination->trigger_system;
        NUGCUTTRIGGEREVENT_s *source_event = source_triggers->events;
        if (source_event != NULL) {
            NUGCUTTRIGGEREVENT_s *destination_event = destination_triggers->events;
            for (i32 i = 0; i < source_triggers->event_count; ++i) {
                destination_event[i].state_animation = source_event[i].state_animation;
            }
        }
    }
    destination->duration = source->duration;
}

// Retail keeps this out of line; the unit's -O3 would otherwise inline it.
static __used__ __attribute__((noinline)) void instNuGCutTriggerSysUpdate(instNUGCUTSCENE_s *instance, float frame) {
    instNUGCUTTRIGGERSYS_s *trigger_instance = instance->trigger_instance;
    NUGCUTTRIGGERSYS_s *system = instance->cutscene->trigger_system;
    for (i32 i = 0; i < system->event_count; ++i) {
        NUGCUTTRIGGEREVENT_s *event = &system->events[i];
        if (event->state_animation == NULL) {
            continue;
        }
        // Retail realigns the frame (and esp,-16) and keeps this out-parameter in a 16-byte aligned slot.
        u8 value __attribute__((aligned(16)));
        if (StateAnimEvaluate(event->state_animation, reinterpret_cast<u8 *>(&trigger_instance->event_states[i]),
                              &value, frame) == 0) {
            continue;
        }
        NUGCUTTRIGGEROWNER_s *trigger_owner = static_cast<NUGCUTTRIGGEROWNER_s *>(trigger_instance->owner);
        const i32 trigger_index = static_cast<i16>(event->field_00);
        if (value != 0) {
            trigger_owner->states[trigger_index].flags |= 1;
        } else {
            trigger_owner->states[trigger_index].flags &= ~1U;
        }
    }
}

// Retail keeps this out of line; the unit's -O3 would otherwise inline it.
static __attribute__((noinline)) void instNuGCutSceneClipTest(instNUGCUTSCENE_s *instance) {
    NUGCUTSCENE_s *cutscene = instance->cutscene;
    instance->flags_89 |= 4;
    if (cutscene->bounds == NULL) {
        return;
    }
    if ((instance->flags_89 & 2) != 0 &&
        NuCameraDistSqr(&instance->transformed_bounds_center) > instance->max_camera_distance_squared) {
        instance->flags_89 &= ~4U;
        return;
    }
    if ((instance->flags_89 & 1) != 0) {
        NUVEC *bounds = static_cast<NUVEC *>(cutscene->bounds);
        NUMTX *matrix = &instance->matrix;
        if (static_cast<i8>(instance->flags_88) >= 0) {
            matrix = &numtx_identity;
        }
        if (NuCameraClipTestExtents(&bounds[0], &bounds[1], matrix, 0.0f, 0) == 0) {
            instance->flags_89 &= ~4U;
        }
    }
}

static void instNuGCutLocatorSysEnd(instNUGCUTLOCATORSYS_s *instance, NUGCUTLOCATORSYS_s *system) {
    for (u32 i = 0; i < system->locator_count; ++i) {
        instNUGCUTLOCATOR_s *inst_locator = &instance->locators[i];
        inst_locator->field_00 = 0;
        NUGCUTLOCATOR_s *locator = &system->locators[i];
        NUGCUTLOCATORTYPE_s *type = &system->types[locator->type_index];
        if ((type->flags & 8) != 0) {
            u8 *sfx = reinterpret_cast<u8 *>(static_cast<usize>(static_cast<u32>(inst_locator->effect_handle)));
            if (*reinterpret_cast<i16 *>(sfx + 8) != 0 && *reinterpret_cast<void **>(sfx) != NULL) {
                u8 *owner = *reinterpret_cast<u8 **>(sfx);
                auto silence = [owner](u16 index) {
                    if (index != 0) {
                        u8 *table = *reinterpret_cast<u8 **>(owner + 0xc);
                        u8 *voice = *reinterpret_cast<u8 **>(table - 4 + index * 4);
                        *reinterpret_cast<i32 *>(voice + 0x80) = 0;
                        *reinterpret_cast<i32 *>(voice + 0x84) = 0;
                    }
                };
                silence(*reinterpret_cast<u16 *>(sfx + 0xe));
                silence(*reinterpret_cast<u16 *>(sfx + 0x10));
                silence(*reinterpret_cast<u16 *>(sfx + 0xa));
            }
        } else if ((type->flags & 1) != 0) {
            if ((locator->flags & 0x20) != 0 && inst_locator->effect_handle >= 0) {
                DebFreeInstantly(&inst_locator->effect_handle);
            }
        } else if ((type->flags & 0x10) != 0) {
            if (ReleaseLocatorVfxFn != NULL) {
                ReleaseLocatorVfxFn(inst_locator->effect_handle);
            }
            inst_locator->effect_handle = -1;
            inst_locator->field_00 = 0;
        }
    }
}

static void NuGCutSceneFixPtrsNoAnims(NUGCUTSCENE_s *cutscene) {
    NuGCutSceneFixPtrs_Title(cutscene, 0);
}

static void NuGCutSceneFixPtrsAnims(NUGCUTSCENE_s *cutscene, isize anim_delta) {
    NuGCutSceneFixPtrs_Title(cutscene, anim_delta);
}

extern "C" {
    void NuGCutSceneSysBackgroundFlush(void) {
        background_cutscene_instances = NULL;
    }

extern "C" void NuGCutSceneSysInit(NUGCUTLOCATORFNENTRY_s *locator_functions) {
    background_cutscene_instances = NULL;
    active_cutscene_instances = NULL;
    locatorfns = locator_functions;
}

}

void NuGCutSceneSysInitVfx(NUGCUTLOOKUPLOCATORVFXFN lookup, NUGCUTTRIGGERLOCATORVFXFN trigger,
                           NUGCUTRELEASELOCATORVFXFN release, NUGCUTUPDATELOCATORVFXFN update) {
    LookupLocatorVfxFn = lookup;
    TriggerLocatorVfxFn = trigger;
    ReleaseLocatorVfxFn = release;
    UpdateLocatorVfxFn = update;
}

void instNuGCutSceneResetCamLock(instNUGCUTSCENE_s *instance) {
    if (instance != NULL && instance->camera_instance != NULL && instance->camera_instance->camera_index >= 0) {
        CutSceneCameraCTRL = 0;
    }
}

void NuGCutSceneRemapFocusIdToLocaterNum(NUGCUTSCENE_s *cutscene, VARIPTR *buffer) {
    if (cutscene->version <= 4 || cutscene->camera_system == NULL ||
        cutscene->camera_system->focus_state_animation == NULL || cutscene->locator_system == NULL) {
        return;
    }

    buffer->addr = ALIGN(buffer->addr, 2);
    cutscene->focus_camera_indices = reinterpret_cast<u16 *>(buffer->void_ptr);
    NUGCUTLOCATORSYS_s *system = cutscene->locator_system;
    for (u32 i = 0; i < system->locator_count; ++i) {
        NUGCUTLOCATOR_s *locator = &system->locators[i];
        if ((system->types[locator->type_index].flags & 8) != 0) {
            *reinterpret_cast<u16 *>(buffer->void_ptr) = static_cast<u16>(i);
            buffer->void_ptr = reinterpret_cast<u16 *>(buffer->void_ptr) + 1;
        }
    }
}

extern "C" {
    NUGCUTSCENE_s *NuGCutSceneLoad(char *name, VARIPTR *buf, VARIPTR *buf_end, i32 flags) {
        char path[1036];
        usize available = buf_end->addr - buf->addr;
        strcpy(path, name);
        buf->addr = ALIGN(buf->addr, 0x10);
        NUGCUTSCENE_s *cutscene = reinterpret_cast<NUGCUTSCENE_s *>(buf->void_ptr);
        i32 bytes = NuFileLoadBuffer(path, cutscene, static_cast<i32>(available));
        if (__builtin_expect(bytes == 0, 0)) {
            return NULL;
        }

        if (cutscene->version > 9) {
            isize anim_delta = reinterpret_cast<isize>(cutscene) - cutscene->relocation_delta;
            cutscene->string_delta = reinterpret_cast<isize>(cutscene) - cutscene->string_delta;
            cutscene->loaded_size = bytes;
            cutscene->relocation_delta = anim_delta;
            if (__builtin_expect((cutscene->flags & 8) == 0, 1)) {
                NuGCutSceneFixPtrsAnims(cutscene, anim_delta);
            } else {
                NuGCutSceneFixPtrsNoAnims(cutscene);
            }
            if ((cutscene->flags & 1) != 0 && flags == 0) {
                usize stream_buffer = ALIGN(buf->addr + cutscene->field_4c, 0x10);
                cutscene->stream_buffer_0 = reinterpret_cast<NUGCUTSCENE_s *>(stream_buffer);
                stream_buffer += cutscene->stream_buffer_size;
                if (NumCommonStreamingBuffers != 0) {
                    stream_buffer += 0x400;
                } else {
                    stream_buffer = ALIGN(stream_buffer + 0x400, 0x10);
                    cutscene->stream_buffer_1 = reinterpret_cast<NUGCUTSCENE_s *>(stream_buffer);
                    stream_buffer += cutscene->stream_buffer_size + 0x400;
                }
                buf->addr = stream_buffer;
                char *filename = reinterpret_cast<char *>(buf->void_ptr);
                buf->addr += NuStrLen(name) + 4;
                NuStrCpy(filename, name);
                cutscene->filename = filename;
            } else {
                buf->addr += bytes;
            }
            NuGCutSceneRemapFocusIdToLocaterNum(cutscene, buf);
            return cutscene;
        }

        isize anim_delta = reinterpret_cast<isize>(cutscene) - cutscene->string_delta;
        cutscene->string_delta = anim_delta;
        if (cutscene->version > 1) {
            anim_delta = reinterpret_cast<isize>(cutscene) - cutscene->relocation_delta;
            cutscene->relocation_delta = anim_delta;
        }
        if ((cutscene->flags & 8) == 0) {
            NuGCutSceneFixPtrsAnims(cutscene, anim_delta);
        } else {
            NuGCutSceneFixPtrsNoAnims(cutscene);
        }
        if (cutscene->version > 1 && (cutscene->flags & 1) != 0 && flags == 0) {
            usize stream_buffer;
            if (cutscene->camera_system != NULL && cutscene->camera_system->animation != NULL) {
                stream_buffer = reinterpret_cast<usize>(cutscene->camera_system->animation);
            } else if (cutscene->rigid_system != NULL && cutscene->rigid_system->rigids[0].animation != NULL) {
                stream_buffer = reinterpret_cast<usize>(cutscene->rigid_system->rigids[0].animation);
            } else if (cutscene->character_system != NULL) {
                NUGCUTCHAR_s *character = cutscene->character_system->characters;
                stream_buffer = reinterpret_cast<usize>(character->animation);
                if (stream_buffer == 0) {
                    stream_buffer = reinterpret_cast<usize>(character->face_animation);
                }
            } else {
                stream_buffer = 0;
            }
            if (cutscene->field_4c != 0) {
                stream_buffer = buf->addr + cutscene->field_4c;
            }
            stream_buffer = ALIGN(stream_buffer, 0x10);
            cutscene->stream_buffer_0 = reinterpret_cast<NUGCUTSCENE_s *>(stream_buffer);
            stream_buffer += cutscene->stream_buffer_size;
            if (NumCommonStreamingBuffers != 0) {
                stream_buffer += 0x400;
            } else {
                stream_buffer = ALIGN(stream_buffer + 0x400, 0x10);
                cutscene->stream_buffer_1 = reinterpret_cast<NUGCUTSCENE_s *>(stream_buffer);
                stream_buffer += cutscene->stream_buffer_size + 0x400;
            }
            buf->addr = stream_buffer;
            char *filename = reinterpret_cast<char *>(buf->void_ptr);
            buf->addr += NuStrLen(name) + 4;
            NuStrCpy(filename, name);
            cutscene->filename = filename;
            return cutscene;
        }
        buf->addr += bytes;
        return cutscene;
    }

    // Original 0x433910: fix a version-10+ cutscene already resident at
    // its final address, then remap its focus-camera indices.
    NUGCUTSCENE_s *NuGCutSceneLoadAddr(NUGCUTSCENE_s *cutscene, i32 loaded_size, VARIPTR *buffer) {
        if (cutscene->version <= 9) {
            return NULL;
        }
        cutscene->loaded_size = loaded_size;
        isize anim_delta = reinterpret_cast<isize>(cutscene) - cutscene->relocation_delta;
        cutscene->string_delta = reinterpret_cast<isize>(cutscene) - cutscene->string_delta;
        cutscene->relocation_delta = anim_delta;
        if ((cutscene->flags & 8) == 0) {
            NuGCutSceneFixPtrsAnims(cutscene, anim_delta);
        } else {
            NuGCutSceneFixPtrsNoAnims(cutscene);
        }
        NuGCutSceneRemapFocusIdToLocaterNum(cutscene, buffer);
        cutscene->string_delta = 0;
        return cutscene;
    }

    void NuGCutSceneDestroy(NUGCUTSCENE_s *cutscene) {
        if (cutscene->character_system != NULL && NuCutSceneDestroyCharacters != NULL) {
            NuCutSceneDestroyCharacters(cutscene);
        }
    }

    void NuGCutSceneFixUp(NUGCUTSCENE_s *cutscene, NUGSCN *scene, i32 flags, i32 area) {
        if (cutscene == NULL) {
            return;
        }
        if (__builtin_expect(cutscene->version > 1, 0)) {
            cutscene->scene = scene;
            cutscene->extra_scene = reinterpret_cast<void *>(static_cast<usize>(flags));
        }
        NUGCUTRIGIDSYS_s *rigid_system = cutscene->rigid_system;
        if (rigid_system != NULL && scene != NULL) {
            NUGCUTLOCATORSYS_s *locator_system = cutscene->locator_system;
            for (i32 i = 0; i < rigid_system->count; ++i) {
                NUGCUTRIGID_s *rigid = &rigid_system->rigids[i];
                nuhspecial_s special __attribute__((aligned(16)));
                if (__builtin_expect(NuSpecialFind(scene, &special, rigid->name, 1) != 0, 1)) {
                    rigid->flags |= 4;
                    rigid->scene = special.scene;
                    if (special.display_special != NULL) {
                        rigid->special_object = special.display_special;
                    } else {
                        rigid->special_object = special.special;
                    }
                }
                if (rigid->locator_count != 0 && static_cast<i32>(rigid->locator) < 0xff) {
                    rigid->locator_index = static_cast<u8>(rigid->locator);
                    rigid->locator = reinterpret_cast<usize>(&locator_system->locators[rigid->locator]);
                } else {
                    rigid->locator_index = 0xff;
                }
            }
        }
        if (cutscene->character_system != NULL && NuCutSceneFindCharacters != NULL) {
            NuCutSceneFindCharacters(cutscene);
        }
        NUGCUTLOCATORSYS_s *system = cutscene->locator_system;
        if (system != NULL && system->type_count != 0) {
            for (u32 i = 0; i < system->type_count; ++i) {
                NUGCUTLOCATORTYPE_s *type = &system->types[i];
                if ((type->flags & 1) != 0) {
                    if (NuGCutDebFixUp_SearchAllPages == 0) {
                        type->function_index =
                            static_cast<u16>(LookupDebrisEffectPageOnly(type->name, static_cast<i8>(area)));
                    } else {
                        type->function_index =
                            static_cast<u16>(LookupDebrisEffectPage(type->name, static_cast<i8>(area)));
                    }
                } else if (__builtin_expect((type->flags & 2) != 0, 0)) {
                    i32 function_index = -1;
                    if (locatorfns != NULL && locatorfns[0].name != NULL) {
                        for (i32 j = 0; locatorfns[j].name != NULL; ++j) {
                            if (__builtin_expect(NuStrICmp(type->name, locatorfns[j].name) != 0, 1)) {
                                continue;
                            }
                            function_index = j;
                            break;
                        }
                    }
                    type->function_index = static_cast<u16>(function_index);
                } else if ((type->flags & 0x10) != 0) {
                    if (__builtin_expect(LookupLocatorVfxFn != NULL, 0)) {
                        char *underscore = NuStrRChr(type->name, '_');
                        if (underscore != NULL && underscore[1] > '/' && underscore[1] < ':') {
                            *underscore = '\0';
                        }
                        type->function_index = static_cast<u16>(LookupLocatorVfxFn(type->name));
                    }
                } else if ((type->flags & 4) != 0) {
                    if (NuCutSceneSFXFixUp != NULL) {
                        type->function_index =
                            static_cast<u16>(NuCutSceneSFXFixUp(reinterpret_cast<usize>(type->name)));
                        if (static_cast<i16>(type->function_index) != -1) {
                            cutscene->flags |= 4;
                        }
                    }
                }
            }
        }
        NUGCUTTRIGGERSYS_s *trigger_system = cutscene->trigger_system;
        if (trigger_system != NULL && flags != 0) {
            const CutSceneTriggerNameTable *table =
                reinterpret_cast<const CutSceneTriggerNameTable *>(static_cast<usize>(flags));
            for (i32 i = 0; i < trigger_system->event_count; ++i) {
                NUGCUTTRIGGEREVENT_s *event = &trigger_system->events[i];
                // The event's target index is the low 16 bits of field_00 (read back as i16 elsewhere).
                u16 *target_index = reinterpret_cast<u16 *>(&event->field_00);
                for (i32 j = 0; j < table->count; ++j) {
                    *target_index = 0xffff;
                    if (NuStrICmp(static_cast<char *>(event->field_04), table->entries[j].name) == 0) {
                        *target_index = static_cast<u16>(j);
                        break;
                    }
                }
            }
        }
    }

    void NuGCutSceneFixUpExtra(NUGCUTSCENE_s *cutscene, NUGSCN *area) {
        if (cutscene == NULL || cutscene->rigid_system == NULL || area == NULL) {
            return;
        }

        NUGCUTRIGIDSYS_s *system = cutscene->rigid_system;
        for (i32 i = 0; i < system->count; ++i) {
            NUGCUTRIGID_s *rigid = &system->rigids[i];
            if ((rigid->flags & 4) != 0) {
                continue;
            }

            // Retail realigns the frame (and esp,-16) and keeps this handle 16-byte aligned.
            nuhspecial_s special __attribute__((aligned(16)));
            if (NuSpecialFind(area, &special, rigid->name, 1) == 0) {
                continue;
            }
            rigid->flags |= 4;
            rigid->scene = special.scene;
            rigid->special_object = special.display_special != NULL ? special.display_special : special.special;
        }
    }

    void NuGCutSceneSysPostBackgroundLoad(void) {
        instNUGCUTSCENE_s *instance = background_cutscene_instances;
        instNUGCUTSCENE_s *active = active_cutscene_instances;
        if (instance == NULL) {
            return;
        }

        do {
            instNUGCUTSCENE_s *next = instance->next;
            instance->next = active;
            if (active != NULL) {
                active->previous = instance;
            }
            instance->previous = NULL;
            active = instance;
            instance = next;
        } while (instance != NULL);

        background_cutscene_instances = NULL;
        active_cutscene_instances = active;
    }

    void PetesHackOfDeath(void) {
        active_cutscene_instances = NULL;
    }

    instNUGCUTSCENE_s *instNuGCutSceneCreate(NUGCUTSCENE_s *cutscene, NUGSCN *scene, void *extra, char *name,
                                             VARIPTR *buf, i32 flags) {
        if (cutscene == NULL) {
            return NULL;
        }
        buf->addr = ALIGN(buf->addr, 0x10);
        instNUGCUTSCENE_s *instance = reinterpret_cast<instNUGCUTSCENE_s *>(buf->void_ptr);
        buf->addr += sizeof(*instance);
        if (instance == NULL) {
            return NULL;
        }
        memset(instance, 0, sizeof(*instance));
        instance->alpha = 1.0f;
        instance->field_c4 = scene;
        instance->field_c8 = extra;
        if (cutscene->version > 1) {
            const u8 cutscene_flags = static_cast<u8>(cutscene->flags);
            if ((cutscene_flags & 1) != 0) {
                instance->stream_buffer_1 = cutscene->stream_buffer_1;
                instance->stream_buffer_0 = cutscene->stream_buffer_0;
                instance->pending_stream_buffer = cutscene->stream_buffer_1;
                instance->flags_8b &= ~0x10U;
            }
            instance->flags_8b = (instance->flags_8b & ~0x20U) | ((cutscene_flags & 9) != 0 ? 0x20 : 0);
        }
        instance->cutscene = cutscene;
        instance->cutscene_copy = cutscene;
        if (name != NULL) {
            sprintf(instance->name, name);
        }

        NUGCUTCAMERASYS_s *camera_system = cutscene->camera_system;
        if (camera_system != NULL) {
            instNUGCUTSCENECAMERA_s *camera_instance = NULL;
            if (camera_system->camera_count != 0) {
                buf->addr = ALIGN(buf->addr, 0x10);
                camera_instance = reinterpret_cast<instNUGCUTSCENECAMERA_s *>(buf->void_ptr);
                buf->void_ptr = camera_instance + 1;
                memset(camera_instance, 0, sizeof(*camera_instance));
                buf->addr = ALIGN(buf->addr, 0x10);
                camera_instance->camera_states = reinterpret_cast<instNUGCUTCAMSTATE_s *>(buf->void_ptr);
                buf->void_ptr = camera_instance->camera_states + camera_system->camera_count;
                memset(camera_instance->camera_states, 0, camera_system->camera_count * sizeof(instNUGCUTCAMSTATE_s));
            }
            instance->camera_instance = camera_instance;
        }

        NUGCUTRIGIDSYS_s *rigid_system = cutscene->rigid_system;
        if (rigid_system != NULL) {
            instNUGCUTRIGIDSYS_s *rigid_instance = NULL;
            if (rigid_system->count != 0) {
                rigid_instance = reinterpret_cast<instNUGCUTRIGIDSYS_s *>(ALIGN(buf->addr, 0x10));
                buf->void_ptr = rigid_instance + 1;
                rigid_instance->rigids = NULL;
                buf->addr = ALIGN(buf->addr, 0x10);
                rigid_instance->rigids = reinterpret_cast<instNUGCUTRIGID_s *>(buf->void_ptr);
                buf->void_ptr = rigid_instance->rigids + rigid_system->count;
                memset(rigid_instance->rigids, 0, rigid_system->count * sizeof(instNUGCUTRIGID_s));
                for (i32 i = 0; i < rigid_system->count; ++i) {
                    NUGCUTRIGID_s *rigid = &rigid_system->rigids[i];
                    instNUGCUTRIGID_s *inst_rigid = &rigid_instance->rigids[i];
                    if (scene == NULL || scene == rigid->scene) {
                        inst_rigid->scene = scene == NULL ? rigid->scene : scene;
                        if (inst_rigid->scene != NULL && inst_rigid->scene->display_list != NULL) {
                            inst_rigid->special = NULL;
                            inst_rigid->display_special = rigid->special_object;
                        } else {
                            inst_rigid->special = rigid->special_object;
                            inst_rigid->display_special = NULL;
                        }
                    } else if (rigid->special_object != NULL) {
                        inst_rigid->scene = scene;
                        if (scene->display_list == NULL) {
                            const usize offset = (reinterpret_cast<usize>(rigid->special_object) -
                                                  reinterpret_cast<usize>(rigid->scene->specials)) &
                                                 ~static_cast<usize>(3);
                            inst_rigid->special = reinterpret_cast<u8 *>(scene->specials) + offset;
                            inst_rigid->display_special = NULL;
                        } else {
                            const usize offset = (reinterpret_cast<usize>(rigid->special_object) -
                                                  reinterpret_cast<usize>(rigid->scene->display_list->specials)) &
                                                 ~static_cast<usize>(0xf);
                            inst_rigid->special = NULL;
                            inst_rigid->display_special =
                                reinterpret_cast<u8 *>(scene->display_list->specials) + offset;
                        }
                        if ((rigid->flags & 2) != 0) {
                            inst_rigid->visible = rigid->flags & 1;
                        }
                    }
                }
            }
            instance->rigid_instance = rigid_instance;
        }

        NUGCUTCHARSYS_s *character_system = cutscene->character_system;
        if (character_system != NULL) {
            instNUGCUTCHARSYS_s *character_instance = NULL;
            if (character_system->character_count != 0) {
                character_instance = reinterpret_cast<instNUGCUTCHARSYS_s *>(ALIGN(buf->addr, 0x10));
                buf->void_ptr = character_instance + 1;
                character_instance->characters = NULL;
                buf->addr = ALIGN(buf->addr, 0x10);
                character_instance->characters = reinterpret_cast<instNUGCUTCHAR_s *>(buf->void_ptr);
                buf->void_ptr = character_instance->characters + character_system->character_count;
                memset(character_instance->characters, 0, character_system->character_count * sizeof(instNUGCUTCHAR_s));
                for (i32 i = 0; i < character_system->character_count; ++i) {
                    NUGCUTCHAR_s *character = &character_system->characters[i];
                    instNUGCUTCHAR_s *inst_character = &character_instance->characters[i];
                    inst_character->field_16 = 0xff;
                    inst_character->field_15 = 0xff;
                    if ((character->flags & 2) != 0) {
                        if (NuCutSceneCharacterCreateData != NULL) {
                            NuCutSceneCharacterCreateData(character, inst_character, buf);
                        }
                    } else {
                        inst_character->character_model = character->character_model;
                    }
                }
            }
            instance->character_instance = character_instance;
        }

        NUGCUTLOCATORSYS_s *locator_system = cutscene->locator_system;
        if (locator_system != NULL) {
            instNUGCUTLOCATORSYS_s *locator_instance = NULL;
            if (locator_system->locator_count != 0) {
                locator_instance = reinterpret_cast<instNUGCUTLOCATORSYS_s *>(ALIGN(buf->addr, 0x10));
                buf->void_ptr = locator_instance + 1;
                locator_instance->locators = NULL;
                buf->addr = ALIGN(buf->addr, 0x10);
                locator_instance->locators = reinterpret_cast<instNUGCUTLOCATOR_s *>(buf->void_ptr);
                buf->void_ptr = locator_instance->locators + locator_system->locator_count;
                memset(locator_instance->locators, 0, locator_system->locator_count * sizeof(instNUGCUTLOCATOR_s));
                for (i32 i = 0; i < locator_system->locator_count; ++i) {
                    NUGCUTLOCATOR_s *locator = &locator_system->locators[i];
                    NUGCUTLOCATORTYPE_s *type = &locator_system->types[locator->type_index];
                    instNUGCUTLOCATOR_s *inst_locator = &locator_instance->locators[i];
                    if ((type->flags & 1) != 0) {
                        if ((locator->flags & 0x20) != 0) {
                            inst_locator->effect_handle = -1;
                        }
                    } else if ((type->flags & 2) != 0) {
                    } else if ((type->flags & 0x10) != 0) {
                        inst_locator->effect_handle = -1;
                    } else if ((type->flags & 8) != 0) {
                        buf->addr = ALIGN(buf->addr, 4);
                        void *effect = buf->void_ptr;
                        inst_locator->effect_handle = static_cast<i32>(reinterpret_cast<usize>(effect));
                        buf->addr += 0x18;
                        memset(effect, 0, 0x18);
                        if (locator->field_5b != 0) {
                            *reinterpret_cast<NUGCUTLOCATOR_s **>(static_cast<u8 *>(effect) + 0x14) = locator;
                        }
                    } else if ((type->flags & 4) != 0) {
                        buf->addr = ALIGN(buf->addr, 0x10);
                        void *effect = buf->void_ptr;
                        inst_locator->effect_handle = static_cast<i32>(reinterpret_cast<usize>(effect));
                        buf->addr += 0x10;
                        memset(effect, 0, 0x10);
                    }
                }
            }
            instance->locator_instance = locator_instance;
        }

        if (cutscene->trigger_system != NULL && extra != NULL) {
            const usize state_size = static_cast<usize>(cutscene->trigger_system->event_count) * sizeof(u32);
            buf->addr = ALIGN(buf->addr, 0x10);
            instance->trigger_instance = reinterpret_cast<instNUGCUTTRIGGERSYS_s *>(buf->void_ptr);
            buf->void_ptr = instance->trigger_instance + 1;
            instance->trigger_instance->owner = extra;
            instance->trigger_instance->event_states = reinterpret_cast<u32 *>(buf->void_ptr);
            buf->addr += state_size;
            memset(instance->trigger_instance->event_states, 0, state_size);
        }

        NUVEC *bounds = static_cast<NUVEC *>(instance->cutscene->bounds);
        if (bounds == NULL) {
            instance->transformed_bounds_center.x = 0.0f;
            instance->transformed_bounds_center.y = 0.0f;
            instance->transformed_bounds_center.z = 0.0f;
        } else {
            instance->transformed_bounds_center.x = (bounds[1].x + bounds[0].x) * 0.5f;
            instance->transformed_bounds_center.y = (bounds[1].y + bounds[0].y) * 0.5f;
            instance->transformed_bounds_center.z = (bounds[1].z + bounds[0].z) * 0.5f;
        }
        instance->rate = 1.0f;

        (*NuThreadDisableThreadSwap)();
        if (flags == 0) {
            instance->next = active_cutscene_instances;
            if (active_cutscene_instances != NULL) {
                active_cutscene_instances->previous = instance;
            }
            active_cutscene_instances = instance;
        } else {
            instance->next = background_cutscene_instances;
            if (background_cutscene_instances != NULL) {
                background_cutscene_instances->previous = instance;
            }
            background_cutscene_instances = instance;
        }
        instance->allocation_size = static_cast<i32>(buf->addr - reinterpret_cast<usize>(instance));
        (*NuThreadEnableThreadSwap)();
        return instance;
    }

    void instNuGCutSceneStop(instNUGCUTSCENE_s *instance) {
        instance->flags_88 &= ~6;
        instance->flags_89 &= ~0x10;
        if ((instance->flags_8a & 4) != 0) {
            instNUGCUTCHARSYS_s *runtime = instance->character_instance;
            if (runtime != NULL) {
                NUGCUTCHARSYS_s *system = instance->cutscene->character_system;
                u16 count = system->character_count;
                for (i32 i = 0; i < count; ++i) {
                    NUGCUTCHAR_s *characters = system->characters;
                    instNUGCUTCHAR_s *character = &runtime->characters[i];
                    if (character->character != NULL && NuCutSceneCharacterRelease != NULL)
                        NuCutSceneCharacterRelease(character, &characters[i]);
                    count = system->character_count;
                }
            }
        } else {
            instNUGCUTCHARSYS_s *runtime = instance->character_instance;
            if (runtime != NULL) {
                NUGCUTCHARSYS_s *system = instance->cutscene->character_system;
                for (i32 i = 0; i < system->character_count; ++i) {
                    instNUGCUTCHAR_s *character = &runtime->characters[i];
                    NUGCUTCHAR_s *characters = system->characters;
                    if (character->character != NULL && NuCutSceneCharacterRelease != NULL)
                        NuCutSceneCharacterRelease(character, &characters[i]);
                }
            }
        }
        instNuGCutSceneResetCamLock(instance);
    }

    i32 instNuGCutSceneIsFinished(instNUGCUTSCENE_s *instance) {
        return instance->finished;
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

    f32 instCutSceneTimeElapsed(instNUGCUTSCENE_s *instance) {
        if (__builtin_expect(instance == NULL || instance->cutscene == NULL, 0)) {
            return 0.0f;
        }
        f32 rate = instance->rate;
        f32 frame = instance->current_frame;
        f32 accumulated = instance->accumulated_stream_duration;
        f32 result = 0.0f;
        if (rate != 0.0f) {
            f32 elapsed = frame + accumulated - 1.0f;
            if (elapsed != 0.0f) {
                result = elapsed / rate;
            }
        }
        return result;
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

    void instNuGCutSceneReset(instNUGCUTSCENE_s *instance) {
        if (instance == NULL) {
            return;
        }
        instance->current_frame = 1.0f;
        instance->render_frame = 1.0f;
        instance->flags_88 &= 0xf8;
        instance->flags_89 &= 0xef;
        instance->flags_8c &= 0xbf;
        instance->flags_8d &= 0xef;
        instance->chained_instance = NULL;
        instance->end_callback = NULL;
        if (instance->rate < 0.0f) {
            instance->rate = -instance->rate;
        }
        instance->cutscene = instance->cutscene_copy;
        instNUGCUTRIGIDSYS_s *rigid_instance = instance->rigid_instance;
        if (rigid_instance != NULL) {
            NUGCUTRIGIDSYS_s *system = instance->cutscene->rigid_system;
            for (u32 i = 0; i < system->count; ++i) {
                instNUGCUTRIGID_s *inst_rigid = &rigid_instance->rigids[i];
                NUGCUTRIGID_s *rigid = &system->rigids[i];
                inst_rigid->state_index = 0;
                const u8 flags = rigid->flags;
                if ((flags & 4) != 0) {
                    if ((flags & 2) != 0) {
                        inst_rigid->visible = flags & 1;
                    } else {
                        if ((flags & 1) != 0) {
                            NuSpecialSetVisibility(inst_rigid, 1);
                        } else {
                            NuSpecialSetVisibility(inst_rigid, 0);
                        }
                        NuSpecialSetDrawMtx(inst_rigid, &rigid->base_matrix);
                    }
                }
            }
        }
        instNUGCUTLOCATORSYS_s *locator_instance = instance->locator_instance;
        if (locator_instance != NULL) {
            instNUGCUTLOCATORSYS_s *inst_system = locator_instance;
            NUGCUTLOCATORSYS_s *system = instance->cutscene->locator_system;
            for (u32 i = 0; i < system->locator_count; ++i) {
                instNUGCUTLOCATOR_s *inst_locator = &inst_system->locators[i];
                NUGCUTLOCATOR_s *locator = &system->locators[i];
                NUGCUTLOCATORTYPE_s *type = &system->types[locator->type_index];
                if ((type->flags & 2) != 0 && type->function_index != 0xffff &&
                    locatorfns[type->function_index].reset != NULL) {
                    locatorfns[type->function_index].reset(instance, system, inst_locator, locator);
                }
                if ((type->flags & 0x10) != 0 && type->function_index != 0xffff) {
                    inst_locator->effect_handle = -1;
                }
            }
        }
        if (instance->trigger_instance != NULL) {
            instNUGCUTTRIGGERSYS_s *inst_system = instance->trigger_instance;
            NUGCUTTRIGGERSYS_s *system = instance->cutscene->trigger_system;
            for (i32 i = 0; i < system->event_count; ++i) {
                NUGCUTTRIGGEREVENT_s *event = &system->events[i];
                u8 *state_index = reinterpret_cast<u8 *>(&inst_system->event_states[i]);
                *state_index = 0;
                u8 value[12];
                if (StateAnimEvaluate(event->state_animation, state_index, value, 1.0f) != 0) {
                    NUGCUTTRIGGEROWNER_s *owner = static_cast<NUGCUTTRIGGEROWNER_s *>(inst_system->owner);
                    NUGCUTTRIGGERSTATE_s *state = &owner->states[static_cast<i16>(event->field_00)];
                    if (value[0] != 0) {
                        state->flags |= 1;
                    } else {
                        state->flags &= ~1U;
                    }
                }
            }
        }
        if (instance->character_instance != NULL && NuCutSceneResetCharactersFn != NULL) {
            NuCutSceneResetCharactersFn(instance);
        }
        instNuGCutSceneResetCamLock(instance);
    }

    void gcutSetSoundVol(i32 sound_volume, i32 music_volume) {
        gcutSoundVol = sound_volume;
        gcutSoundMusVol = music_volume;
    }

    void instNuGCutSceneStart(instNUGCUTSCENE_s *instance) {
        instance->current_frame = 1.0f;
        instance->render_frame = 1.0f;
        instance->flags_89 &= 0xef;
        instance->flags_8d &= 0xef;
        instance->flags_88 |= 2;
        instance->flags_88 &= ~1;
        if (instance->rate < 0.0f) {
            instance->rate = -instance->rate;
        }
        if (instance->camera_instance != NULL) {
            instNUGCUTSCENECAMERA_s *camera = instance->camera_instance;
            NUGCUTCAMERASYS_s *system = instance->cutscene->camera_system;
            camera->state_index = 0;
            camera->camera_index = static_cast<i8>(system->field_10);
            camera->next_target_index = 0;
            for (u32 i = 0; i < system->camera_count; ++i) {
                instNUGCUTCAMSTATE_s *state = &camera->camera_states[i];
                state->flags &= ~2U;
                state->event_index = 0;
            }
        }
        instNUGCUTRIGIDSYS_s *rigid_instance = instance->rigid_instance;
        if (rigid_instance != NULL) {
            NUGCUTRIGIDSYS_s *system = instance->cutscene->rigid_system;
            for (u32 i = 0; i < system->count; ++i) {
                instNUGCUTRIGID_s *inst_rigid = &rigid_instance->rigids[i];
                NUGCUTRIGID_s *rigid = &system->rigids[i];
                inst_rigid->state_index = 0;
                if ((rigid->flags & 6) == 6) {
                    inst_rigid->visible = rigid->flags & 1;
                }
            }
        }
        if (instance->locator_instance != NULL) {
            NUGCUTLOCATORSYS_s *system = instance->cutscene->locator_system;
            for (u32 i = 0; i < system->locator_count; ++i) {
                instNUGCUTLOCATOR_s *inst_locator = &instance->locator_instance->locators[i];
                inst_locator->field_00 = 0;
                if ((system->types[system->locators[i].type_index].flags & 0x10) != 0) {
                    inst_locator->effect_handle = -1;
                }
            }
        }
        instNUGCUTTRIGGERSYS_s *trigger_instance = instance->trigger_instance;
        if (trigger_instance != NULL) {
            NUGCUTTRIGGERSYS_s *system = instance->cutscene->trigger_system;
            for (i32 i = 0; i < system->event_count; ++i) {
                *reinterpret_cast<u8 *>(&trigger_instance->event_states[i]) = 0;
            }
        }
        if (instance->skip_countdown != 0 && instance->stream_index == 0) {
            instance->flags_8b |= 0x40;
            instance->skip_delay = 0xa;
        }
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

    void instNuGCutScenePause(instNUGCUTSCENE_s *instance, u8 paused) {
        instance->paused = paused;
    }

}

void instNuGCutSceneEndButNotSystems(instNUGCUTSCENE_s *instance) {
    instance->current_frame = instance->cutscene->duration;
    instance->render_frame = instance->cutscene->duration;
    instance->flags_88 &= ~2U;
    instance->flags_89 |= 0x10;
    instance->flags_8c &= ~0x40U;
    instNuGCutSceneResetCamLock(instance);
}

extern "C" {
    void instNuGCutSceneEnable(instNUGCUTSCENE_s *instance) {
        instance->flags_89 &= ~0x8;
    }

    void instNuGCutSceneDisable(instNUGCUTSCENE_s *instance) {
        instance->flags_89 |= 0x8;
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

    void instNuGCutSceneSetRepeat(instNUGCUTSCENE_s *instance, i32 repeat) {
        instance->repeat_count = repeat <= 31 ? repeat : 31;
    }

    void instNuGCutSceneWaitAtEnd(instNUGCUTSCENE_s *instance, u8 enabled) {
        instance->wait_at_end = enabled;
    }

    void instNuGCutSceneChain(instNUGCUTSCENE_s *instance, instNUGCUTSCENE_s *next) {
        instance->chained_instance = next;
    }

    void instNuGCutSceneSetEndCallback(instNUGCUTSCENE_s *instance, void (*callback)(instNUGCUTSCENE_s *)) {
        instance->end_callback = callback;
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

    void instNuGCutSceneCreateCamTgtArray(instNUGCUTSCENE_s *instance, i32 count, VARIPTR *buf) {
        instNUGCUTSCENECAMERA_s *camera = instance->camera_instance;
        if (count == 0 || camera == NULL) {
            return;
        }
        camera->target_capacity = static_cast<u8>(count);
        buf->addr = ALIGN(buf->addr, 0x10);
        camera->targets = reinterpret_cast<instNUGCUTCAMTGT_s *>(buf->void_ptr);
        buf->addr += count * sizeof(instNUGCUTCAMTGT_s);
        memset(camera->targets, 0, count * sizeof(instNUGCUTCAMTGT_s));
    }

    void instNuGCutSoundStream(void) {
    }

    instNUGCUTSCENE_s *instNuGCutSceneFind(const char *name) {
        for (instNUGCUTSCENE_s *instance = active_cutscene_instances; instance != NULL; instance = instance->next) {
            if (NuStrICmp(name, instance->name) == 0) {
                return instance;
            }
        }
        return NULL;
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

}

void NewCopyAnims(instNUGCUTSCENE_s *instance) {
    // stream_buffer_0/1 are adjacent (asserted in nugcutscene.h); the original indexes them by flag bit.
    NUGCUTSCENE_s *source = instance->cutscene_copy;
    NUGCUTSCENE_s *destination = (&instance->stream_buffer_0)[(static_cast<u32>(instance->flags_8b) >> 4) & 1];
    instance->cutscene = destination;

    destination->filename = source->filename;
    destination->stream_buffer_0 = source->stream_buffer_0;
    destination->stream_buffer_1 = source->stream_buffer_1;
    reinterpret_cast<u8 *>(&destination->flags)[0] =
        (reinterpret_cast<u8 *>(&source->flags)[0] & 2) | (reinterpret_cast<u8 *>(&destination->flags)[0] & ~2U);

    NUGCUTLOCATORSYS_s *destination_locators = destination->locator_system;
    NUGCUTLOCATORSYS_s *source_locators = source->locator_system;
    if (destination_locators != NULL) {
        for (i32 i = 0; i < destination_locators->locator_count; ++i) {
            nuanimdata2_s *animation = destination_locators->locators[i].animation;
            destination_locators->locators[i] = source_locators->locators[i];
            destination_locators->locators[i].animation = animation;
        }
        destination_locators->types = source_locators->types;
    }

    NUGCUTRIGIDSYS_s *destination_rigids = destination->rigid_system;
    NUGCUTRIGIDSYS_s *source_rigids = source->rigid_system;
    if (destination_rigids != NULL && destination_rigids->rigids != NULL && destination_rigids->count != 0) {
        for (i32 i = 0; i < destination_rigids->count; ++i) {
            nuanimdata2_s *animation = destination_rigids->rigids[i].animation;
            StateAnim *state_animation = destination_rigids->rigids[i].state_animation;
            destination_rigids->rigids[i] = source_rigids->rigids[i];
            destination_rigids->rigids[i].animation = animation;
            destination_rigids->rigids[i].state_animation = state_animation;
            if (destination_rigids->rigids[i].locator != 0 && destination_rigids->rigids[i].locator_count != 0 &&
                destination_rigids->rigids[i].locator_index != 0xff) {
                destination_rigids->rigids[i].locator = reinterpret_cast<usize>(
                    &destination->locator_system->locators[destination_rigids->rigids[i].locator_index]);
            }
        }
    }

    NUGCUTCHARSYS_s *destination_characters = destination->character_system;
    NUGCUTCHARSYS_s *source_characters = source->character_system;
    if (destination_characters != NULL && destination_characters->character_count != 0) {
        for (i32 i = 0; i < destination_characters->character_count; ++i) {
            nuanimdata2_s *animation = destination_characters->characters[i].animation;
            nuanimdata2_s *face_animation = destination_characters->characters[i].face_animation;
            nuanimdata2_s *extra_animation = destination_characters->characters[i].extra_animation;
            NUGCUTLOCATOR_s *locator = destination_characters->characters[i].locator;
            destination_characters->characters[i] = source_characters->characters[i];
            destination_characters->characters[i].animation = animation;
            destination_characters->characters[i].face_animation = face_animation;
            destination_characters->characters[i].extra_animation = extra_animation;
            if (locator != NULL && destination_characters->characters[i].has_locator != 0 &&
                destination_characters->characters[i].locator_index != 0xff) {
                destination_characters->characters[i].locator =
                    &destination->locator_system->locators[destination_characters->characters[i].locator_index];
            }
        }
    }

    NUGCUTTRIGGERSYS_s *source_triggers = source->trigger_system;
    NUGCUTTRIGGERSYS_s *destination_triggers = destination->trigger_system;
    if (destination_triggers != NULL) {
        for (i32 i = 0; i < destination_triggers->event_count; ++i) {
            destination_triggers->events[i].field_00 = source_triggers->events[i].field_00;
            destination_triggers->events[i].field_04 = source_triggers->events[i].field_04;
        }
    }
}

extern "C" {
    i32 NuGCutSceneIsBackgroundLoading(void) {
        return 1;
    }

    i32 instNuGCutScenePreload(instNUGCUTSCENE_s *instance) {
        if (cutscene_load_instance != NULL) {
            return 0;
        }
        cutscene_load_instance = instance;
        return 1;
    }

}

i32 instNuGCutSceneSwapBuffers(instNUGCUTSCENE_s *instance, i32 force) {
    if (__builtin_expect(static_cast<i8>(instance->flags_8c) >= 0, 0) || force != 0) {
        if (instance->pending_stream_buffer == NULL) {
            u8 flags = instance->flags_8b;
            if ((flags & 0x10) != 0) {
                flags &= ~0x10U;
                instance->flags_8b = flags;
                instance->pending_stream_buffer = instance->stream_buffer_1;
            } else {
                flags |= 0x10;
                instance->flags_8b = flags;
                instance->pending_stream_buffer = instance->stream_buffer_0;
            }
            return 1;
        }
    }
    return 0;
}

extern "C" {
    void instNuGCutSceneServiceLoad(void) {
        if (cutscene_load_instance == NULL || cutscene_load_instance->cutscene == NULL) {
            return;
        }

        char path[256];
        char extension[8];
        NuStrCpy(path, cutscene_load_instance->cutscene->filename);
        char *dot = strchr(path, '.');
        NuStrCpy(extension, dot);

        const i32 tens = (cutscene_load_instance->stream_index + 1) / 10;
        const i32 units = (cutscene_load_instance->stream_index + 1) % 10;
        if (tens != 0) {
            *dot++ = static_cast<char>('0' + tens);
        }
        *dot++ = static_cast<char>('0' + units);
        NuStrCpy(dot, extension);

        // Retail realigns the frame (and esp,-16) and keeps both buffer cursors in 16-byte aligned slots.
        VARIPTR buffer __attribute__((aligned(16)));
        buffer.void_ptr = cutscene_load_instance->pending_stream_buffer;
        cutscene_load_instance->pending_stream_buffer = NULL;
        if (buffer.void_ptr == NULL) {
            return;
        }

        cutscene_load_instance->stream_loading = 1;
        VARIPTR buffer_end __attribute__((aligned(16)));
        buffer_end.addr = buffer.addr + cutscene_load_instance->cutscene->stream_buffer_size;
        NuGCutSceneLoad(path, &buffer, &buffer_end, 1);
        cutscene_load_instance->stream_loaded = 1;
        const bool swap_buffers =
            cutscene_load_instance->stream_index == 0 && (cutscene_load_instance->cutscene->flags & 8) != 0;
        if (swap_buffers) {
            instNuGCutSceneSwapBuffers(cutscene_load_instance, 1);
            if (NumCommonStreamingBuffers > 1) {
                cutscene_load_instance->stream_common_copy = 1;
                NewCopyAnims(cutscene_load_instance);
            } else {
                copyAnims(cutscene_load_instance->cutscene,
                          static_cast<NUGCUTSCENE_s *>(cutscene_load_instance->cutscene->stream_buffer_1));
            }
            cutscene_load_instance->cutscene->swap_on_first_stream = 0;
            cutscene_load_instance->stream_loaded = 0;
            cutscene_load_instance->stream_index = 1;
        }
        cutscene_load_instance->stream_loading = 0;
        cutscene_load_instance = NULL;
    }

    void NuGCutSetCutAudioStream(i32 stream) {
        NuGCutAudioStream = stream;
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

}

f32 *NuGCutLocatorCalcMtx_3(NUGCUTLOCATOR_s *locator, numtx_s *mtx, float frame) {
    ani3_animheader_s *animation = reinterpret_cast<ani3_animheader_s *>(locator->animation);
    f32 *values = NuAnimCurveExtractAllNodeCurves_3(animation, 0, frame, NULL);
    if ((*animation->node_flags & 1) != 0) {
        NUANGVEC angles = {
            static_cast<NUANG>(values[3] * 10430.378f),
            static_cast<NUANG>(values[4] * 10430.378f),
            static_cast<NUANG>(values[5] * 10430.378f),
        };
        NuMtxSetRotateXYZ(mtx, &angles);
    } else {
        NuMtxSetIdentity(mtx);
    }
    NUVEC translation = {values[0], values[1], values[2]};
    NuMtxTranslate(mtx, &translation);
    mtx->m02 = -mtx->m02;
    mtx->m12 = -mtx->m12;
    mtx->m20 = -mtx->m20;
    mtx->m21 = -mtx->m21;
    mtx->m23 = -mtx->m23;
    mtx->m32 = -mtx->m32;
    NuMtxTranslate(mtx, reinterpret_cast<NUVEC *>(&locator->base_matrix.m30));
    return values;
}

extern "C" {
extern "C" f32 *NuGCutLocatorCalcMtx(NUGCUTLOCATOR_s *locator, float frame, NUMTX *mtx, nuanimtime_s *time) {
    if (locator->animation == NULL) {
        *mtx = locator->base_matrix;
        return NULL;
    }
    if (__builtin_expect(*reinterpret_cast<u32 *>(locator->animation) + 0xbeb1b6ccU < 2, 0)) {
        return NuGCutLocatorCalcMtx_3(locator, mtx, frame);
    }
    nuanimdata2_s *animation = locator->animation;
    u8 *curve_types = animation->curve_types;
    nuanimcurve2_s *curves = animation->curves;
    auto evaluate = [&](u32 curve) {
        const i8 type = static_cast<i8>(curve_types[curve]);
        return type == 0 ? curves[curve].data.constant
                         : NuAnimCurve2CalcValEx(&curves[curve], time, static_cast<u32>(type));
    };
    if ((*animation->node_flags & 1) == 0) {
        NuMtxSetIdentity(mtx);
    } else {
        const f32 rotation_x = evaluate(3);
        const f32 rotation_y = evaluate(4);
        const f32 rotation_z = evaluate(5);
        NUANGVEC angles = {
            static_cast<NUANG>(rotation_x * 10430.378f),
            static_cast<NUANG>(rotation_y * 10430.378f),
            static_cast<NUANG>(rotation_z * 10430.378f),
        };
        NuMtxSetRotateXYZ(mtx, &angles);
    }
    NUVEC translation = {evaluate(0), evaluate(1), evaluate(2)};
    NuMtxTranslate(mtx, &translation);
    mtx->m02 = -mtx->m02;
    mtx->m12 = -mtx->m12;
    mtx->m20 = -mtx->m20;
    mtx->m21 = -mtx->m21;
    mtx->m23 = -mtx->m23;
    mtx->m32 = -mtx->m32;
    NuMtxTranslate(mtx, reinterpret_cast<NUVEC *>(&locator->base_matrix.m30));
    return NULL;
}

}

static void instNuGCutCamSysUpdate(instNUGCUTSCENE_s *instance, float frame) {
    NUMTX focus_matrix;
    cutscenecam_focusDistance = 0.0f;
    cutscenecam_focalLength = 0.0f;
    cutscenecam_usefocusloc = 0;
    cutscenecam_fstop = 0.0f;

    instNUGCUTSCENECAMERA_s *camera_instance = instance->camera_instance;
    NUGCUTCAMERASYS_s *system = instance->cutscene->camera_system;
    u8 focus_index;
    u8 next_camera_index;
    if (static_cast<u8>(instance->cutscene->version) > 4 && system->focus_state_animation != NULL) {
        if (StateAnimEvaluate(system->focus_state_animation, &camera_instance->focus_state_index, &focus_index,
                              frame) != 0) {
            if (focus_index == 0xff) {
                camera_instance->focus_index = -1;
            } else {
                camera_instance->focus_index = static_cast<i8>(instance->cutscene->focus_camera_indices[focus_index]);
            }
        }
    }

    if (system->state_animation != NULL) {
        if (StateAnimEvaluate(system->state_animation, &camera_instance->state_index, &next_camera_index, frame) != 0) {
            camera_instance->camera_index = static_cast<i8>(next_camera_index);
            instance->flags_8d |= 0x10;
            cutscenecamchange = 1;
        }
        const u8 state_index = camera_instance->state_index;
        if (state_index < system->state_animation->count) {
            const f32 next_state_frame = system->state_animation->times[state_index];
            if (next_state_frame - frame < 1.0f &&
                system->state_animation->values[state_index] != camera_instance->camera_index) {
                instance->render_frame = next_state_frame - 1.0f;
                if ((instance->flags_8a & 4) != 0) {
                    instance->render_frame = instance->cutscene->duration - instance->render_frame;
                }
                if (instance->render_frame < 1.0f) {
                    instance->render_frame = 1.0f;
                }
            }
        }
    }

    while (camera_instance->next_target_index < camera_instance->target_count &&
           frame >= camera_instance->targets[camera_instance->next_target_index].start_frame) {
        const i8 mapped_camera =
            system->target_camera_map[camera_instance->targets[camera_instance->next_target_index].target_index];
        instNUGCUTCAMSTATE_s *state = &camera_instance->camera_states[mapped_camera];
        state->flags |= 2;
        state->event_index = camera_instance->next_target_index;
        ++camera_instance->next_target_index;
    }
    while (camera_instance->next_target_index != 0 &&
           frame < camera_instance->targets[camera_instance->next_target_index - 1].start_frame) {
        --camera_instance->next_target_index;
        const i8 mapped_camera =
            system->target_camera_map[camera_instance->targets[camera_instance->next_target_index].target_index];
        instNUGCUTCAMSTATE_s *state = &camera_instance->camera_states[mapped_camera];
        state->flags |= 2;
        state->event_index = camera_instance->next_target_index;
    }

    i32 camera_index = camera_instance->camera_index;
    if (camera_index < 0) {
        CutSceneCameraCTRL = 0;
        return;
    }

    CameraDOFHack = 2;
    NUGCUTCAMERA_s *camera = &system->cameras[camera_index];
    instNUGCUTCAMSTATE_s *camera_state = &camera_instance->camera_states[camera_index];
    CutSceneCameraCTRL = 1;
    if ((camera->flags & 1) == 0 || camera->animation_node >= NuAnimNumNodes(system->animation)) {
        cutscenecammtx = camera->base_matrix;
    } else {
        if (static_cast<u8>(instance->cutscene->version) > 4 && system->focus_animation != NULL &&
            *reinterpret_cast<u32 *>(system->focus_animation) - ANI3_MAGIC_VERSION_4 < 2) {
            f32 *values =
                NuAnimCurveExtractAllNodeCurves_3(reinterpret_cast<ani3_animheader_s *>(system->focus_animation),
                                                  camera->animation_node, instance->render_frame, NULL);
            cutscenecam_fstop = values[2];
            cutscenecam_focalLength = values[0] * 1.3f;
            if ((camera->field_43 & 2) != 0) {
                cutscenecam_focusDistance = values[1];
            } else {
                if (system->focus_state_animation != NULL && camera_instance->focus_index >= 0) {
                    NUGCUTLOCATOR_s *focus_locator =
                        &instance->cutscene->locator_system->locators[camera_instance->focus_index];
                    if (focus_locator->animation != NULL) {

                        NuGCutLocatorCalcMtx(focus_locator, frame, &focus_matrix, NULL);
                        if ((focus_locator->flags & 4) != 0) {
                            NuMtxPreTranslate(&focus_matrix, &focus_locator->pivot);
                        }
                        if (static_cast<i8>(instance->flags_88) < 0) {
                            NuMtxMul(&focus_matrix, &focus_matrix, &instance->matrix);
                        }
                        NuMtxGetTranslation(&focus_matrix, &cutscenecam_focusloc);
                        cutscenecam_usefocusloc = 1;
                    }
                }
            }
        }
        const u32 animation_magic = *reinterpret_cast<u32 *>(system->animation);
        if (animation_magic - ANI3_MAGIC_VERSION_4 < 2) {
            NuAnimCurve2SetApplyToMatrix_3(reinterpret_cast<ani3_animheader_s *>(system->animation),
                                           camera->animation_node, instance->render_frame, &cutscenecammtx);
        }
    }

    if (static_cast<i8>(instance->flags_88) < 0) {
        NuMtxMul(&cutscenecammtx, &cutscenecammtx, &instance->matrix);
    }
    set_cutscenecammtx = 1;
    if ((camera_state->flags & 2) != 0) {

        instNUGCUTCAMTGT_s *target = &camera_instance->targets[camera_state->event_index];
        f32 duration = fabsf(target->duration);
        i32 reverse = 0;
        if (NuFsign(target->duration) < 0.0f) {
            if (duration + target->start_frame <= frame) {
                camera_state->flags &= ~2U;
                return;
            }
            reverse = 1;
        }

        const NUVEC translation = *NUMTX_GET_ROW_VEC(&cutscenecammtx, 3);
        NUMTX look_at_matrix = cutscenecammtx;
        NuMtxLookAtZ(&look_at_matrix, target->target);

        f32 blend = 1.0f;
        if (duration > 0.01f) {
            blend = (frame - target->start_frame) / duration;
        }
        if (blend > 1.0f) {
            blend = 1.0f - blend;
        } else if (!(blend < 0.0f) && reverse != 0) {
            blend = 1.0f - blend;
        }

        NUQUAT look_at_rotation;
        NUQUAT camera_rotation;
        NUQUAT blended_rotation;
        NuMtxToQuat(&look_at_matrix, &look_at_rotation);
        NuMtxToQuat(&cutscenecammtx, &camera_rotation);
        NuQuatSlerp(&blended_rotation, &camera_rotation, &look_at_rotation, blend);
        NuQuatToMtx(&blended_rotation, &cutscenecammtx);
        *NUMTX_GET_ROW_VEC(&cutscenecammtx, 3) = translation;
    }
}

i32 NuGCutLocatorIsVisble_3(NUGCUTLOCATOR_s *locator, float frame, float *scale, float *rate) {
    // The original rereads the locator's animation header for every test.
    const u16 curve_count = reinterpret_cast<ani3_animheader_s *>(locator->animation)->curve_count;
    char *filter = curve_count == 4 ? CurveFilterSFX : (curve_count == 8 ? CurveFilterLOCATOR : CurveFilterLOCATORScale);
    f32 *values = NuAnimCurveExtractAllNodeCurves_3(reinterpret_cast<ani3_animheader_s *>(locator->animation), 0,
                                                    frame, filter);
    if (reinterpret_cast<ani3_animheader_s *>(locator->animation)->curve_count == 4) {
        f32 visible = values[3];
        return static_cast<i32>(visible < 0.0f ? visible - 0.5f : visible + 0.5f);
    }
    f32 visible = values[7];
    i32 result = static_cast<i32>(visible < 0.0f ? visible - 0.5f : visible + 0.5f);
    if (result != 0 && scale != NULL) {
        *scale = values[6];
    }
    if (rate != NULL) {
        *rate = reinterpret_cast<ani3_animheader_s *>(locator->animation)->curve_count < 9 ? 1.0f : values[8];
    }
    return result;
}

extern "C" {
extern "C" i32 NuGCutLocatorIsVisble(NUGCUTLOCATOR_s *locator, float frame, nuanimtime_s *time, float *scale,
                                     float *rate) {
    if (locator->animation == NULL) {
        const i32 visible = locator->flags & 8;
        if (scale != NULL && visible != 0) {
            *scale = locator->locator_scale;
        }
        return visible;
    }
    if (*reinterpret_cast<u32 *>(locator->animation) + 0xbeb1b6ccU < 2) {
        return NuGCutLocatorIsVisble_3(locator, frame, scale, rate);
    }
    nuanimdata2_s *animation = locator->animation;
    if (rate != NULL) {
        *rate = 1.0f;
    }
    nuanimcurve2_s *curves = animation->curves;
    u8 *curve_types = animation->curve_types;
    if (animation->curve_count == 4) {
        const i8 type = curve_types[3];
        if (type == 0) {
            return static_cast<i32>(curves[3].data.constant);
        }
        return static_cast<i32>(NuAnimCurve2CalcValEx(&curves[3], time, static_cast<u32>(type)));
    }
    const i8 visible_type = curve_types[7];
    i32 visible;
    if (visible_type == 0) {
        visible = static_cast<i32>(curves[7].data.constant);
    } else {
        visible = static_cast<i32>(NuAnimCurve2CalcValEx(&curves[7], time, static_cast<u32>(visible_type)));
    }
    if (visible != 0 && scale != NULL) {
        const i8 scale_type = curve_types[6];
        *scale = scale_type == 0 ? curves[6].data.constant
                                 : NuAnimCurve2CalcValEx(&curves[6], time, static_cast<u32>(scale_type));
    }
    return visible;
}

}

i32 FindMtlInHGObj(nugscn_s *scene, i32 material_type) {
    for (i32 index = 0; index < scene->nummtl; ++index) {
        if (scene->mtls[index]->unknown_9a[0] == material_type) {
            return index + 1;
        }
    }
    return 0;
}

i32 FindTexAnimFromMtl(nugscn_s *scene, numtl_s *material) {
    nutexanim_s *animations = static_cast<nutexanim_s *>(scene->texture_anims);
    for (i32 animation_index = 0; animation_index < scene->num_texture_anims; ++animation_index) {
        if (animations[animation_index].material == material) {
            return animation_index + 1;
        }
    }
    return 0;
}

void instGetLookAtLocatorInfo(instNUGCUTSCENE_s *instance, instNUGCUTLOOKAT_s *opaque_state) {
    GCutLookAtState *state = reinterpret_cast<GCutLookAtState *>(opaque_state);
    NUGCUTLOCATOR_s *locator = state->locator;
    if (locator == NULL || NuCutSceneGetHGObj == NULL)
        return;

    u8 character_id = locator->field_5b;
    state->locator = NULL;
    NUGCUTSCENE_s *cutscene = instance->cutscene;
    NUGCUTCHARANIM_s *animations = cutscene->character_animations;
    for (i32 i = 0; i < cutscene->character_system->character_count; ++i) {
        if (static_cast<u8>(animations[i].field_00) == character_id) {
            state->character_index = i + 1;
            break;
        }
    }

    u16 character_index = state->character_index;
    if (character_index == 0)
        return;
    NUGSCN *scene = NuCutSceneGetHGObj(instance, character_index - 1);
    state->scene = scene;
    if (scene == NULL)
        return;

    nuhgobj_s *object = reinterpret_cast<nuhgobj_s *>(scene);
    u8 joint_index = locator->pad_5c[0];
    if (joint_index < object->joint_override_map_count)
        state->joint_override_index = object->joint_override_map[joint_index] + 1;

    u16 material_index = FindMtlInHGObj(scene, locator->pad_5c[1]);
    state->material_index = material_index;
    if (material_index != 0)
        state->texture_animation_index = FindTexAnimFromMtl(scene, object->materials[material_index - 1]);
    state->secondary_material_index = FindMtlInHGObj(scene, locator->pad_5c[2]);
    state->tertiary_material_index = FindMtlInHGObj(scene, locator->pad_5c[3]);
}

extern "C" {
    void CalculateLocatorDirection(i32 direction, numtx_s *matrix, NUVEC *out) {
        switch (direction) {
            case 0:
            case 1:
                out->z = 0.0f;
                out->x = 0.0f;
                out->y = 1.0f;
                return;
            case 2:
                out->z = 0.0f;
                out->x = 0.0f;
                out->y = -1.0f;
                return;
            case 3:
                out->x = -1.0f;
                out->y = 0.0f;
                out->z = 0.0f;
                break;
            case 4:
                out->x = 1.0f;
                out->y = 0.0f;
                out->z = 0.0f;
                break;
            case 5:
                out->x = 0.0f;
                out->y = -1.0f;
                out->z = 0.0f;
                break;
            case 6:
                out->x = 0.0f;
                out->y = 1.0f;
                out->z = 0.0f;
                break;
            case 7:
                out->x = 0.0f;
                out->y = 0.0f;
                out->z = 1.0f;
                break;
            case 8:
                out->x = 0.0f;
                out->y = 0.0f;
                out->z = -1.0f;
                break;
            default:
                return;
        }
        NuVecMtxRotate(out, out, matrix);
    }

}

void EvaluateJointOrientationMtx(nugscn_s *scene, i32 joint_index, numtx_s *matrix) {
    nuhgobj_s *object = reinterpret_cast<nuhgobj_s *>(scene);
    nuhgobjjoint_s *joint = &object->joints[joint_index];
    *matrix = numtx_identity;

    while (true) {
        NUMTX *bind_matrix = &joint->animation_bind_matrix;
        if (bind_matrix->m00 != 1.0f || bind_matrix->m11 != 1.0f || bind_matrix->m22 != 1.0f) {
            NuMtxMulVU0(matrix, bind_matrix, matrix);
        }

        if (joint->parent_index == 0xff) {
            break;
        }
        joint = &object->joints[joint->parent_index];
    }

    matrix->m02 = -matrix->m02;
    matrix->m12 = -matrix->m12;
    matrix->m20 = -matrix->m20;
    matrix->m21 = -matrix->m21;
    matrix->m23 = -matrix->m23;
    matrix->m32 = -matrix->m32;
    NuMtxTransposeR(matrix, matrix);
}

static void instNuGCutRigidSysEnd(instNUGCUTSCENE_s *instance, float frame) {
    NUGCUTRIGIDSYS_s *system = instance->cutscene->rigid_system;
    instNUGCUTRIGIDSYS_s *rigid_instance = instance->rigid_instance;
    u8 visible;

    for (i32 i = 0; i < system->count; ++i) {
        NUGCUTRIGID_s *rigid = &system->rigids[i];
        instNUGCUTRIGID_s *inst_rigid = &rigid_instance->rigids[i];
        if ((rigid->flags & 4) == 0) {
            continue;
        }
        if ((rigid->flags & 2) != 0) {
            continue;
        }

        if (rigid->state_animation != NULL &&
            StateAnimEvaluate(rigid->state_animation, &inst_rigid->state_index, &visible, frame) != 0) {
            if (visible != 0) {
                NuSpecialSetVisibility(inst_rigid, 1);
            } else {
                NuSpecialSetVisibility(inst_rigid, 0);
            }
        }

        if (NuSpecialGetVisibilityFn(inst_rigid) == 0) {
            continue;
        }

        NUMTX_ALIGNED16 matrix;
        NuGCutRigidCalcMtx(rigid, frame, &matrix);
        if ((instance->flags_88 & 0x80) != 0) {
            NuMtxMul(&matrix, &matrix, &instance->matrix);
        }
        NuSpecialSetDrawMtx(inst_rigid, &matrix);
    }
}

extern "C" {
extern "C" void instNuGCutSceneJumpToEnd(instNUGCUTSCENE_s *instance) {
    NUGCUTSCENE_s *cutscene = instance->cutscene;
    f32 duration = cutscene->duration;
    instance->flags_88_bit0 = 1;
    instance->flags_88_bit1 = 0;
    instance->flags_89 |= 0x10;
    instance->flags_8c &= ~0x40;
    instance->current_frame = duration;
    ForcePlayEndFrame = 1;
    if ((instance->flags_8a & 4) != 0) {
        if (cutscene->rigid_system != NULL) {
            instNuGCutRigidSysEnd(instance, cutscene->duration - instance->current_frame);
        }
    } else {
        if (cutscene->rigid_system != NULL) {
            instNuGCutRigidSysEnd(instance, instance->current_frame);
        }
    }
    if (instance->locator_instance != NULL) {
        instNuGCutLocatorSysEnd(instance->locator_instance, cutscene->locator_system);
    }
    ForcePlayEndFrame = 0;
    instNuGCutSceneResetCamLock(instance);
}

}

void instNuGCutSceneEndFirstFrame(instNUGCUTSCENE_s *instance) {
    NUGCUTSCENE_s *cutscene = instance->cutscene;
    instance->current_frame = 1.0f;
    instance->render_frame = 1.0f;
    instance->flags_88 &= ~2U;
    instance->flags_89 &= ~0x10U;
    instance->flags_8c &= ~0x40U;

    if ((instance->flags_8a & 4) != 0) {
        if (cutscene->rigid_system != NULL) {
            instNuGCutRigidSysEnd(instance, cutscene->duration - instance->current_frame);
        }
        if (instance->character_instance != NULL) {
            NUGCUTSCENE_s *scene = instance->cutscene;
            const f32 frame = scene->duration - instance->current_frame;
            NUGCUTCHARSYS_s *system = scene->character_system;
            for (i32 i = 0; i < system->character_count; ++i) {
                instNUGCUTCHAR_s *inst_character = &instance->character_instance->characters[i];
                NUGCUTCHAR_s *character = &system->characters[i];
                if (inst_character->character_model != NULL) {
                    if ((character->flags & 2) == 0 && NuCutSceneCharacterEval != NULL) {
                        NuCutSceneCharacterEval(instance, scene, inst_character, character, frame);
                    }
                    if (nu_current_thread_id == 0 && NuCutSceneCharacterRelease != NULL) {
                        NuCutSceneCharacterRelease(inst_character, character);
                    }
                }
            }
        }
    } else {
        if (cutscene->rigid_system != NULL) {
            instNuGCutRigidSysEnd(instance, instance->current_frame);
        }
        if (instance->character_instance != NULL) {
            NUGCUTSCENE_s *scene = instance->cutscene;
            const f32 frame = instance->current_frame;
            NUGCUTCHARSYS_s *system = scene->character_system;
            for (i32 i = 0; i < system->character_count; ++i) {
                instNUGCUTCHAR_s *inst_character = &instance->character_instance->characters[i];
                NUGCUTCHAR_s *character = &system->characters[i];
                if (inst_character->character_model != NULL) {
                    if ((character->flags & 2) == 0 && NuCutSceneCharacterEval != NULL) {
                        NuCutSceneCharacterEval(instance, scene, inst_character, character, frame);
                    }
                    if (nu_current_thread_id == 0 && NuCutSceneCharacterRelease != NULL) {
                        NuCutSceneCharacterRelease(inst_character, character);
                    }
                }
            }
        }
    }
    if (instance->locator_instance != NULL) {
        instNuGCutLocatorSysEnd(instance->locator_instance, cutscene->locator_system);
    }
    instNuGCutSceneResetCamLock(instance);
}

extern "C" {
extern "C" void instNuGCutSceneEnd(instNUGCUTSCENE_s *instance) {
    NUGCUTSCENE_s *cutscene = instance->cutscene;
    instNuGCutSceneEndButNotSystems(instance);

    const f32 end_frame = cutscene->duration;
    instance->flags_88 &= ~2U;
    instance->flags_89 |= 0x10;
    instance->current_frame = end_frame;
    instance->flags_8c &= ~0x40U;
    ForcePlayEndFrame = 1;

    if ((instance->flags_8a & 4) != 0) {
        if (cutscene->rigid_system != NULL) {
            instNuGCutRigidSysEnd(instance, instance->cutscene->duration - instance->current_frame);
        }
        if (instance->character_instance != NULL) {
            NUGCUTSCENE_s *scene = instance->cutscene;
            const f32 frame = scene->duration - instance->current_frame;
            NUGCUTCHARSYS_s *system = scene->character_system;
            for (i32 i = 0; i < system->character_count; ++i) {
                NUGCUTCHAR_s *character = &system->characters[i];
                instNUGCUTCHAR_s *inst_character = &instance->character_instance->characters[i];
                if (inst_character->character_model != NULL) {
                    if ((character->flags & 2) == 0 && NuCutSceneCharacterEval != NULL) {
                        NuCutSceneCharacterEval(instance, scene, inst_character, character, frame);
                    }
                    if (nu_current_thread_id == 0 && NuCutSceneCharacterRelease != NULL) {
                        NuCutSceneCharacterRelease(inst_character, character);
                    }
                }
            }
        }
    } else {
        if (cutscene->rigid_system != NULL) {
            instNuGCutRigidSysEnd(instance, instance->current_frame);
        }
        if (instance->character_instance != NULL) {
            NUGCUTSCENE_s *scene = instance->cutscene;
            const f32 frame = instance->current_frame;
            NUGCUTCHARSYS_s *system = scene->character_system;
            for (i32 i = 0; i < system->character_count; ++i) {
                NUGCUTCHAR_s *character = &system->characters[i];
                instNUGCUTCHAR_s *inst_character = &instance->character_instance->characters[i];
                if (inst_character->character_model != NULL) {
                    if ((character->flags & 2) == 0 && NuCutSceneCharacterEval != NULL) {
                        NuCutSceneCharacterEval(instance, scene, inst_character, character, frame);
                    }
                    if (nu_current_thread_id == 0 && NuCutSceneCharacterRelease != NULL) {
                        NuCutSceneCharacterRelease(inst_character, character);
                    }
                }
            }
        }
    }

    if (instance->locator_instance != NULL) {
        instNuGCutLocatorSysEnd(instance->locator_instance, cutscene->locator_system);
    }
    ForcePlayEndFrame = 0;
    instNuGCutSceneResetCamLock(instance);
}

    void instNuGCutSceneDestroy(instNUGCUTSCENE_s *instance) {
        instance->cutscene = instance->cutscene_copy;
        if ((instance->flags_88 & 2) != 0) {
            instNuGCutSceneEnd(instance);
        }
        instNUGCUTCHARSYS_s *character_instance = instance->character_instance;
        if (character_instance != NULL) {
            NUGCUTCHARSYS_s *system = instance->cutscene->character_system;
            if (system != NULL) {
                i32 count = system->character_count;
                for (i32 i = 0; i < count; ++i) {
                    NUGCUTCHAR_s *character = &system->characters[i];
                    instNUGCUTCHAR_s *inst_character = &character_instance->characters[i];
                    if ((character->flags & 2) != 0 && NuCutSceneCharacterDestroyData != NULL) {
                        NuCutSceneCharacterDestroyData(character, inst_character);
                        count = system->character_count;
                    }
                }
            }
        }
        if (instance->next != NULL) {
            instance->next->previous = instance->previous;
        }
        if (instance->previous == NULL) {
            active_cutscene_instances = instance->next;
        } else {
            instance->previous->next = instance->next;
        }
    }

extern "C" void instNuGCutSceneJumpToLastFrame(instNUGCUTSCENE_s *instance) {
    NUGCUTSCENE_s *cutscene = instance->cutscene;
    f32 duration = cutscene->duration;
    instance->flags_88 |= 3;
    instance->flags_89 |= 0x10;
    instance->flags_8c |= 0x40;
    instance->current_frame = duration;
    ForcePlayEndFrame = 1;
    if ((instance->flags_8a & 4) != 0) {
        if (cutscene->rigid_system != NULL) {
            instNuGCutRigidSysEnd(instance, cutscene->duration - instance->current_frame);
        }
    } else {
        if (cutscene->rigid_system != NULL) {
            instNuGCutRigidSysEnd(instance, instance->current_frame);
        }
    }
    if (instance->locator_instance != NULL) {
        instNuGCutLocatorSysEnd(instance->locator_instance, cutscene->locator_system);
    }
    ForcePlayEndFrame = 0;
    instNuGCutSceneResetCamLock(instance);
}

}

i32 instNuGCutContainsInstancedRigids(instNUGCUTSCENE_s *instance) {
    NUGCUTRIGIDSYS_s *rigid_system = instance->cutscene->rigid_system;
    for (i32 i = 0; i < rigid_system->count; ++i) {
        if ((rigid_system->rigids[i].flags & 6) == 6)
            return 1;
    }
    return 0;
}

i32 instNuGCutGetNextRigidInfo(instNUGCUTSCENE_s *instance, float frame, i32 index, numtx_s *matrix,
                               nuhspecial_s *special) {
    NUGCUTRIGIDSYS_s *rigid_system = instance->cutscene->rigid_system;
    instNUGCUTRIGIDSYS_s *instance_rigid_system = instance->rigid_instance;
    if (rigid_system == NULL || index >= rigid_system->count)
        return 0;
    NUGCUTRIGID_s *rigid = &rigid_system->rigids[index];
    instNUGCUTRIGID_s *instance_rigids = instance_rigid_system->rigids;
    if ((rigid->flags & 6) != 0) {
        extern void NuGCutRigidCalcMtx(NUGCUTRIGID_s *, float, numtx_s *);
        NuGCutRigidCalcMtx(rigid, frame, matrix);
        if ((instance->flags_88 & 0x80) != 0)
            NuMtxMul(matrix, matrix, &instance->matrix);
        instNUGCUTRIGID_s *instance_rigid = &instance_rigids[index];
        special->scene = instance_rigid->scene;
        special->special = instance_rigid->special;
        special->display_special = reinterpret_cast<NUDISPLAYSPECIAL *>(instance_rigid->display_special);
    }
    return index + 1;
}

extern "C" {
    void NuGCutCharAnimProcess(NUGCUTCHAR_s *character, f32 frame, NUMTX *matrix, i32 *visible, u32 *animation_index,
                               f32 *animation_rate, f32 *blend_time, f32 *animation_start_frame, i32 *layer_mask) {
        nuanimdata2_s *animation = character->animation;
        if (animation != NULL) {
            const u32 magic = *reinterpret_cast<u32 *>(animation);
            if (magic == ANI3_MAGIC_VERSION_4 || magic == ANI3_MAGIC_VERSION_5) {
                NuGCutCharAnimProcess_3(character, frame, matrix, visible, animation_index, animation_rate, blend_time,
                                        animation_start_frame, layer_mask);
                return;
            }

            nuanimtime_s time;
            NuAnimData2CalcTime(animation, frame, &time);
            animation = character->animation;
            nuanimcurve2_s *curves = animation->curves;
            i8 *types = reinterpret_cast<i8 *>(animation->curve_types);
            const u8 node_flags = animation->node_flags[0];
#define NUGCUT_CURVE_VALUE(curve)                                                                                      \
    (types[curve] == 0 ? curves[curve].data.constant : NuAnimCurve2CalcValEx(&curves[curve], &time, types[curve]))

            if (static_cast<i16>(character->animation->curve_count) > 6) {
                *visible = static_cast<i32>(NUGCUT_CURVE_VALUE(6));
            } else {
                *visible = character->flags & 1;
            }
            if (animation_index != NULL) {
                if (static_cast<i16>(character->animation->curve_count) > 7) {
                    const f32 value = NUGCUT_CURVE_VALUE(7);
                    if (value < 0.0f) {
                        *animation_index = 0xff;
                    } else {
                        *animation_index = static_cast<i32>(value);
                    }
                } else {
                    *animation_index = character->animation_index;
                }
                if (animation_start_frame != NULL) {
                    if (*animation_index != 0 && *animation_index != 0xff) {
                        if (static_cast<i16>(character->animation->curve_count) > 10) {
                            *animation_start_frame = NUGCUT_CURVE_VALUE(10);
                        } else {
                            *animation_start_frame = static_cast<f32>(character->animation_start_frame);
                        }
                    } else {
                        *animation_start_frame = 0.0f;
                    }
                }
            } else if (animation_start_frame != NULL) {
                *animation_start_frame = 0.0f;
            }
            if (*visible == 0) {
                return;
            }
            if (layer_mask != NULL) {
                if (static_cast<i16>(character->animation->curve_count) > 11) {
                    *layer_mask = static_cast<i32>(NUGCUT_CURVE_VALUE(11));
                } else {
                    *layer_mask = -1;
                }
            }

            if ((node_flags & NUANIM_NODE_HAS_ROTATION) != 0) {
                const f32 rx = NUGCUT_CURVE_VALUE(3);
                const f32 ry = NUGCUT_CURVE_VALUE(4);
                const f32 rz = NUGCUT_CURVE_VALUE(5);
                NUANGVEC rotation = {
                    static_cast<NUANG>(rx * 10430.378f),
                    static_cast<NUANG>(ry * 10430.378f),
                    static_cast<NUANG>(rz * 10430.378f),
                };
                NuMtxSetRotateXYZ(matrix, &rotation);
            } else {
                NuMtxSetIdentity(matrix);
            }
            NUVEC translation = {NUGCUT_CURVE_VALUE(0), NUGCUT_CURVE_VALUE(1), NUGCUT_CURVE_VALUE(2)};
            NuMtxTranslate(matrix, &translation);
            matrix->m02 = -matrix->m02;
            matrix->m12 = -matrix->m12;
            matrix->m20 = -matrix->m20;
            matrix->m21 = -matrix->m21;
            matrix->m23 = -matrix->m23;
            matrix->m32 = -matrix->m32;
            NUVEC scale;
            scale = NuMtxGetScale(&character->base_matrix);
            NuMtxPreScale(matrix, &scale);
            if (animation_rate != NULL) {
                if (static_cast<i16>(character->animation->curve_count) > 9) {
                    *animation_rate = NUGCUT_CURVE_VALUE(9);
                } else {
                    *animation_rate = character->animation_rate;
                }
            }
            if (blend_time != NULL) {
                if (static_cast<i16>(character->animation->curve_count) > 8) {
                    *blend_time = NUGCUT_CURVE_VALUE(8);
                } else {
                    *blend_time = static_cast<f32>(character->blend_time);
                }
            }
        } else {
            *visible = character->flags & 1;
            if (animation_index != NULL) {
                *animation_index = character->animation_index;
            }
            *matrix = character->base_matrix;
            if (animation_rate != NULL) {
                *animation_rate = character->animation_rate;
            }
            if (blend_time != NULL) {
                *blend_time = static_cast<f32>(character->blend_time);
            }
        }
    }

}

static void instNuGCutLookAtLocatorUpdate(i32 *effect_handle, NUGCUTLOCATOR_s *locator, f32 frame,
                                          NUMTX *parent_mtx, instNUGCUTSCENE_s *instance) {
    if (*reinterpret_cast<u32 *>(locator->animation) - 0x414e4934U > 1) {
        return;
    }
    // The locator's instance slot holds the look-at record pointer.
    GCutLookAtState *state = reinterpret_cast<GCutLookAtState *>(static_cast<uintptr_t>(*effect_handle));
    instGetLookAtLocatorInfo(instance, reinterpret_cast<instNUGCUTLOOKAT_s *>(state));
    if (__builtin_expect(state->character_index == 0 || state->scene == NULL, 0)) {
        return;
    }
    NUGSCN *scene = state->scene;

    f32 *values;
    f32 position_x, position_y, position_z;
    {
        NUMTX matrix;
        values = NuGCutLocatorCalcMtx(locator, frame, &matrix, NULL);
        if (__builtin_expect((locator->flags & 4) != 0, 0)) {
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
                instNuGCutLookAtLocatorUpdate(&inst_locator->effect_handle, locator, frame, parent_mtx, instance);
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

}

static void instNuGCutRigidSysRender(instNUGCUTSCENE_s *instance, float frame, int paused) {
    NUGCUTSCENE_s *cutscene = instance->cutscene;
    NUGCUTRIGIDSYS_s *system = cutscene->rigid_system;
    instNUGCUTRIGIDSYS_s *rigid_instance = instance->rigid_instance;
    u8 visible;
    const u16 count = system->count;

    for (i32 i = 0; i < count; ++i) {
        NUGCUTRIGID_s *rigid = &system->rigids[i];
        instNUGCUTRIGID_s *inst_rigid = &rigid_instance->rigids[i];
        if ((rigid->flags & 2) == 0) {
            continue;
        }
        if ((rigid->flags & 4) == 0) {
            continue;
        }
        if (rigid->state_animation != NULL &&
            StateAnimEvaluate(rigid->state_animation, &inst_rigid->state_index, &visible, frame) != 0) {
            if (visible != 0) {
                inst_rigid->visible = 1;
            } else {
                inst_rigid->visible = 0;
                continue;
            }
        } else if (__builtin_expect(inst_rigid->visible == 0, 1)) {
            continue;
        }

        NUMTX_ALIGNED16 matrix;
        NuGCutRigidCalcMtx(rigid, frame, &matrix);
        if ((instance->flags_88 & 0x80) != 0) {
            NuMtxMul(&matrix, &matrix, &instance->matrix);
        }
        if (instance->alpha == 1.0f) {
            NuSpecialDrawAt(inst_rigid, &matrix);
        } else {
            NuSpecialDrawAtAlpha(inst_rigid, &matrix, instance->alpha);
        }
        if (NuCutSceneRigidPostRender != NULL && (rigid->flags & 0x18) != 0) {
            NuCutSceneRigidPostRender(rigid, inst_rigid, &matrix);
        }
        if (NuCutSceneRigidCollisionCheck != NULL) {
            NuCutSceneRigidCollisionCheck(rigid, &matrix);
        }
        if (__builtin_expect(rigid->locator_index != 0xff, 0)) {
            NUGCUTLOCATORSYS_s *locator_system = cutscene->locator_system;
            instNUGCUTLOCATORSYS_s *locator_instance = instance->locator_instance;
            for (i32 locator_offset = 0; locator_offset < rigid->locator_count; ++locator_offset) {
                i32 locator_index = rigid->locator_index + locator_offset;
                instNuGCutLocatorUpdate(instance, locator_system, &locator_instance->locators[locator_index],
                                        &locator_system->locators[locator_index], frame, &matrix, paused);
            }
        }
    }
}

static void instNuGCutRigidSysUpdate(instNUGCUTSCENE_s *instance, float frame, int paused) {
    NUGCUTSCENE_s *cutscene = instance->cutscene;
    NUGCUTRIGIDSYS_s *system = cutscene->rigid_system;
    instNUGCUTRIGIDSYS_s *rigid_instance = instance->rigid_instance;
    u8 visible;

    for (i32 i = 0; i < system->count; ++i) {
        NUGCUTRIGID_s *rigid = &system->rigids[i];
        instNUGCUTRIGID_s *inst_rigid = &rigid_instance->rigids[i];
        if ((rigid->flags & 2) != 0) {
            continue;
        }
        if ((rigid->flags & 4) == 0) {
            continue;
        }

        if (rigid->state_animation != NULL &&
            StateAnimEvaluate(rigid->state_animation, &inst_rigid->state_index, &visible, frame) != 0) {
            if (visible != 0) {
                NuSpecialSetVisibility(inst_rigid, 1);
            } else {
                NuSpecialSetVisibility(inst_rigid, 0);
            }
        }

        if (NuSpecialGetVisibilityFn(inst_rigid) == 0) {
            continue;
        }

        NUMTX_ALIGNED16 matrix;
        NuGCutRigidCalcMtx(rigid, frame, &matrix);
        if ((instance->flags_88 & 0x80) != 0) {
            NuMtxMul(&matrix, &matrix, &instance->matrix);
        }
        NuSpecialSetDrawMtx(inst_rigid, &matrix);

        if (NuSpecialGetVisibilityFn(inst_rigid) != 0 && rigid->locator_index != 0xff) {
            NUGCUTLOCATORSYS_s *locator_system = cutscene->locator_system;
            instNUGCUTLOCATORSYS_s *locator_instance = instance->locator_instance;
            for (i32 locator_offset = 0; locator_offset < rigid->locator_count; ++locator_offset) {
                i32 locator_index = rigid->locator_index + locator_offset;
                instNuGCutLocatorUpdate(instance, locator_system, &locator_instance->locators[locator_index],
                                        &locator_system->locators[locator_index], frame, &matrix, paused);
            }
        }
    }
}

static void instNuGCutSceneUpdate(instNUGCUTSCENE_s *instance, int paused, int skip, float elapsed) {
    NUGCUTSCENE_s *cutscene = instance->cutscene;
    instance->flags_8d &= ~0x10U;
    u16 *sync_flags = reinterpret_cast<u16 *>(&instance->flags_8a);
    *sync_flags = static_cast<u16>((*sync_flags & 0xf807U) | (static_cast<u8>(cutscene_synchro_counter) << 3));
    if ((instance->flags_89 & 8) != 0 || (cutscene->flags & 8) != 0) {
        return;
    }
    instance->elapsed = elapsed;
    if (NuCutSceneRequestSFX != NULL && (cutscene->flags & 4) != 0 && (instance->flags_8c & 0x20) != 0) {
        NUMTX *camera = NuCameraGetMtx();
        if (camera != NULL && camera->m30 < instance->bounds_max.x && camera->m30 > instance->bounds_min.x &&
            camera->m31 < instance->bounds_max.y && camera->m31 > instance->bounds_min.y &&
            camera->m32 < instance->bounds_max.z && camera->m32 > instance->bounds_min.z) {
            NuCutSceneRequestSFX(instance);
        }
    }
    instance->flags_8b &= ~0x40U;
    if (instance->skip_countdown != 0 && (instance->flags_88 & 1) == 0 && instance->skip_delay != 0) {
        instance->skip_delay--;
        instance->flags_8b |= 0x40;
        paused = 1;
    }
    if ((instance->flags_88 & 2) == 0) {
        if ((instance->flags_88 & 0x41) != 0x40) {
            return;
        }
        instNuGCutSceneStart(instance);
    }
    if ((cutscene->flags & 8) != 0 && (instance->flags_8b & 8) == 0) {
        return;
    }
    if (instance->cutscene->version > 1) {
        if ((instance->flags_8b & 0x20) != 0 && cutscene_load_instance == NULL && (instance->flags_8b & 8) == 0 &&
            instance->stream_index != instNuGCutSceneLastStream(instance)) {
            cutscene_load_instance = instance;
        }
        if (instance->queued_stream_instance != NULL && cutscene_load_instance == NULL &&
            instance->stream_index == instNuGCutSceneLastStream(instance)) {
            instNUGCUTSCENE_s *queued_instance = instance->queued_stream_instance;
            instance->queued_stream_instance = NULL;
            cutscene_load_instance = queued_instance;
        }
    }
    instNuGCutSceneClipTest(instance);
    if ((instance->flags_89 & 4) == 0) {
        return;
    }
    if ((instance->flags_88 & 4) != 0) {
        paused = 1;
    }
    if (termcutstream_hack != 0 || (instance->flags_8c & 0x10) != 0) {
        if (cutscene->version > 1 && (cutscene->flags & 1) != 0) {
            if (skip != 0) {
                instance->flags_8d |= 4;
            } else {
                cutscene_quit_prompt = 1;
            }
        } else if ((instance->flags_8c & 0x10) != 0) {
            if (skip == 0) {
                cutscene_quit_prompt = 1;
            } else {
                instance->current_frame = cutscene->duration - 1.0f - instance->rate * elapsed;
                cutscene_has_quit = 1;
            }
        }
    }
    if ((instance->flags_8d & 2) != 0) {
        cutscene_lock_player = 1;
    }
    if ((instance->flags_88 & 1) != 0) {
        if (paused == 0) {
            const f32 frame_delta = instance->rate * elapsed;
            instance->current_frame += frame_delta;
            if ((instance->flags_8d & 4) != 0) {
                const u8 last_stream = instNuGCutSceneLastStream(instance);
                if (instance->stream_index == last_stream) {
                    instance->current_frame = instance->cutscene->duration - 1.0f;
                }
                instance->current_frame -= frame_delta;
                if (cutscene_load_instance == NULL) {
                    if (instance->stream_index == last_stream - 1) {
                        instNuGCutSceneSwapBuffers(instance, 0);
                        if (NumCommonStreamingBuffers > 1) {
                            NewCopyAnims(instance);
                        } else {
                            copyAnims(instance->cutscene, instNuGCutSceneCurrentStreamBuffer(instance));
                        }
                        instance->stream_index = instance->cutscene->last_stream;
                        instNuGCutSceneStart(instance);
                        instance->current_frame = instance->cutscene->duration - 1.0f;
                    } else {
                        instance->stream_index = instance->cutscene->last_stream - 1;
                        if ((instance->flags_8b & 0x10) != 0) {
                            instance->pending_stream_buffer = static_cast<void *>(instance->stream_buffer_0);
                        } else {
                            instance->pending_stream_buffer = static_cast<void *>(instance->stream_buffer_1);
                        }
                        cutscene_load_instance = instance;
                    }
                }
            }
            if (instance->current_frame < 1.0f) {
                if ((instance->flags_88 & 0x10) != 0) {
                    if ((instance->flags_88 & 8) != 0) {
                        if (instance->rate < 0.0f) {
                            instance->rate = -instance->rate;
                        }
                        instance->current_frame = 2.0f - instance->current_frame;
                    } else {
                        const i32 repeat_count = instance->repeat_count;
                        if (repeat_count > 0) {
                            const f32 overflow = 1.0f - instance->current_frame;
                            --instance->repeat_count;
                            instNuGCutSceneStart(instance);
                            instance->current_frame += overflow;
                            instance->flags_88 |= 1;
                        } else if (instance->chained_instance != NULL) {
                            const f32 overflow = 1.0f - instance->current_frame;
                            instNuGCutSceneEndFirstFrame(instance);
                            instNuGCutSceneStart(instance->chained_instance);
                            instance->chained_instance->current_frame += overflow;
                            instNuGCutSceneClipTest(instance->chained_instance);
                            instNuGCutSceneUpdate(instance->chained_instance, 0, skip, elapsed);
                            instance->chained_instance = NULL;
                            return;
                        } else {
                            instNuGCutSceneEndFirstFrame(instance);
                            return;
                        }
                    }
                } else if ((instance->flags_88 & 0x20) != 0) {
                    instance->current_frame = 1.0f;
                } else {
                    instNuGCutSceneEndFirstFrame(instance);
                }
            } else {
                const f32 end_frame = cutscene->duration - 1.0f;
                if (instance->current_frame >= end_frame) {
                    if (!(instance->cutscene->version > 1 && (instance->cutscene->flags & 1) != 0)) {
                        if ((instance->flags_88 & 0x10) != 0) {
                            instance->current_frame -= end_frame;
                            instance->current_frame = end_frame - instance->current_frame;
                            if (instance->rate > 0.0f) {
                                instance->rate = -instance->rate;
                            }
                        } else if ((instance->flags_8c & 0x40) != 0) {
                            instance->current_frame = end_frame;
                            instance->flags_89 |= 0x10;
                        } else {
                            if (instance->end_callback != NULL) {
                                void (*callback)(instNUGCUTSCENE_s *) = instance->end_callback;
                                instance->end_callback = NULL;
                                callback(instance);
                            }
                            const i32 repeat_count = instance->repeat_count;
                            if (repeat_count > 0) {
                                f32 overflow = instance->current_frame - (cutscene->duration - 1.0f);
                                if (overflow > cutscene->duration) {
                                    overflow = 0.0f;
                                }
                                --instance->repeat_count;
                                instNuGCutSceneStart(instance);
                                instance->current_frame += overflow;
                                instance->flags_88 |= 1;
                            } else if (instance->chained_instance != NULL) {
                                f32 overflow = instance->current_frame - (cutscene->duration - 1.0f);
                                if (overflow > cutscene->duration) {
                                    overflow = 0.0f;
                                }
                                instNuGCutSceneEnd(instance);
                                instNuGCutSceneStart(instance->chained_instance);
                                instance->chained_instance->current_frame += overflow;
                                instNuGCutSceneClipTest(instance->chained_instance);
                                instNuGCutSceneUpdate(instance->chained_instance, 0, skip, elapsed);
                                instance->chained_instance = NULL;
                                return;
                            } else if ((instance->flags_88 & 8) != 0) {
                                f32 overflow = instance->current_frame - (cutscene->duration - 1.0f);
                                if (overflow > cutscene->duration) {
                                    overflow = 0.0f;
                                }
                                instNuGCutSceneStart(instance);
                                instance->current_frame += overflow;
                                instance->flags_88 |= 1;
                            } else {
                                instNuGCutSceneEnd(instance);
                            }
                        }
                    } else {
                        const u8 last_stream = instNuGCutSceneLastStream(instance);
                        if (instance->stream_index < last_stream) {
                            if ((instance->flags_8b & 8) != 0) {
                                f32 overflow = instance->current_frame - (instance->cutscene->duration - 1.0f);
                                if (overflow > instance->cutscene->duration) {
                                    overflow = 0.0f;
                                }
                                instance->accumulated_stream_duration += instance->cutscene->duration;
                                ++instance->stream_index;
                                if (instNuGCutSceneSwapBuffers(instance, 0) != 0) {
                                    if (NumCommonStreamingBuffers > 1) {
                                        NewCopyAnims(instance);
                                    } else {
                                        copyAnims(instance->cutscene, instNuGCutSceneCurrentStreamBuffer(instance));
                                    }
                                    instance->flags_8b &= ~8U;
                                    instNuGCutSceneStart(instance);
                                    instance->current_frame += overflow;
                                    instance->flags_88 |= 1;
                                } else {
                                    --instance->stream_index;
                                    instance->current_frame = instance->cutscene->duration - 1.0f;
                                }
                            } else {
                                instance->current_frame = instance->cutscene->duration - 1.0f;
                            }
                        } else if ((instance->flags_8c & 0x40) != 0) {
                            instance->current_frame = end_frame;
                            instance->flags_89 |= 0x10;
                        } else {
                            instNuGCutSceneEnd(instance);
                            instance->accumulated_stream_duration = 0.0f;
                        }
                    }
                }
            }
        }
    } else {
        if (paused == 0) {
            instance->flags_88 |= 1;
        }
    }
    if ((instance->flags_88 & 2) == 0) {
        return;
    }
    instance->render_frame = instance->current_frame;
    if ((instance->flags_8a & 4) != 0) {
        const f32 render_frame = cutscene->duration - instance->current_frame;
        instNUGCUTCHARSYS_s *character_system = instance->character_instance;
        if (character_system != NULL) {
            NUGCUTSCENE_s *current_cutscene = instance->cutscene;
            NUGCUTCHARSYS_s *system = current_cutscene->character_system;
            if (NuCutSceneCharacterProcess != NULL && system->character_count != 0) {
                for (i32 i = 0; i < system->character_count; ++i) {
                    instNUGCUTCHAR_s *character_instance = &character_system->characters[i];
                    if (character_instance->character_model != NULL) {
                        NuCutSceneCharacterProcess(instance, current_cutscene, character_instance,
                                                   &system->characters[i], render_frame, paused);
                    }
                }
            }
        }
        if (instance->camera_instance != NULL) {
            instNuGCutCamSysUpdate(instance, cutscene->duration - instance->current_frame);
        }
        if (instance->trigger_instance != NULL) {
            instNuGCutTriggerSysUpdate(instance, cutscene->duration - instance->render_frame);
        }
        if (instance->rigid_instance != NULL) {
            instNuGCutRigidSysUpdate(instance, cutscene->duration - instance->render_frame, paused);
        }
    } else {
        const f32 render_frame = instance->render_frame;
        instNUGCUTCHARSYS_s *character_system = instance->character_instance;
        if (character_system != NULL) {
            NUGCUTSCENE_s *current_cutscene = instance->cutscene;
            NUGCUTCHARSYS_s *system = current_cutscene->character_system;
            if (NuCutSceneCharacterProcess != NULL && system->character_count != 0) {
                for (i32 i = 0; i < system->character_count; ++i) {
                    instNUGCUTCHAR_s *character_instance = &character_system->characters[i];
                    if (character_instance->character_model != NULL) {
                        NuCutSceneCharacterProcess(instance, current_cutscene, character_instance,
                                                   &system->characters[i], render_frame, paused);
                    }
                }
            }
        }
        if (instance->camera_instance != NULL) {
            instNuGCutCamSysUpdate(instance, instance->current_frame);
        }
        if (instance->trigger_instance != NULL) {
            instNuGCutTriggerSysUpdate(instance, instance->render_frame);
        }
        if (instance->rigid_instance != NULL) {
            instNuGCutRigidSysUpdate(instance, instance->render_frame, paused);
        }
    }
}

extern "C" {
extern "C" void NuGCutSceneSysUpdate(i32 paused, i32 skip, f32 elapsed) {
    if (__builtin_expect(cutscene_synchro_counter + 1 > 0xff, 0)) {
        cutscene_synchro_counter = 1;
    } else {
        cutscene_synchro_counter++;
    }
    cutscene_quit_prompt = 0;
    cutscene_has_quit = 0;
    cutscene_lock_player = 0;
    cutscenecamchange = 0;
    if (paused != 0) {
        skip = 0;
    }
    for (instNUGCUTSCENE_s *instance = active_cutscene_instances; instance != NULL; instance = instance->next) {
        const i8 instance_sync = static_cast<i8>(static_cast<u32>(*reinterpret_cast<u16 *>(&instance->flags_8a)) >> 3);
        if (instance_sync != cutscene_synchro_counter) {
            instNuGCutSceneUpdate(instance, paused, skip, elapsed);
        }
    }
}

extern "C" void NuGCutSceneSysRender(i32 paused) {
    for (instNUGCUTSCENE_s *instance = active_cutscene_instances; instance != NULL; instance = instance->next) {
        if ((instance->flags_89 & 8) != 0 || (instance->flags_88 & 2) == 0 || (instance->flags_89 & 4) == 0) {
            continue;
        }
        if ((instance->flags_8a & 4) != 0) {
            if (instance->rigid_instance != NULL) {
                instNuGCutRigidSysRender(instance, instance->cutscene->duration - instance->render_frame, paused);
            }

            instNUGCUTCHARSYS_s *character_instance = instance->character_instance;
            if (__builtin_expect(character_instance != NULL, 1)) {
                NUGCUTSCENE_s *cutscene = instance->cutscene;
                const f32 frame = cutscene->duration - instance->render_frame;
                NUGCUTCHARSYS_s *system = cutscene->character_system;
                if (NuCutSceneCharacterRender != NULL) {
                    for (i32 i = 0; i < system->character_count; ++i) {
                        instNUGCUTCHAR_s *character = &character_instance->characters[i];
                        if (character->character_model != NULL) {
                            NuCutSceneCharacterRender(instance, cutscene, character, &system->characters[i], frame,
                                                      paused);
                        }
                    }
                }
            }

            if (__builtin_expect((instance->flags_8c & 0x40) != 0 && instance->current_frame == instance->cutscene->duration - 1.0f, 0)) {
                continue;
            }
            if ((instance->flags_8b & 0x40) != 0) {
                continue;
            }
            instNUGCUTLOCATORSYS_s *locator_instance = instance->locator_instance;
            if (locator_instance != NULL) {
                NUGCUTSCENE_s *cutscene = instance->cutscene;
                const f32 frame = cutscene->duration - instance->render_frame;
                NUGCUTLOCATORSYS_s *system = cutscene->locator_system;
                for (u32 i = 0; i < system->locator_count; ++i) {
                    NUGCUTLOCATOR_s *locator = &system->locators[i];
                    if ((locator->flags & 3) == 0) {
                        if (static_cast<i8>(instance->flags_88) < 0) {
                            instNuGCutLocatorUpdate(instance, system, &locator_instance->locators[i], locator, frame,
                                                    &instance->matrix, paused);
                        } else {
                            instNuGCutLocatorUpdate(instance, system, &locator_instance->locators[i], locator, frame,
                                                    NULL, paused);
                        }
                    }
                }
            }
        } else {
            if (instance->rigid_instance != NULL) {
                instNuGCutRigidSysRender(instance, instance->render_frame, paused);
            }

            instNUGCUTCHARSYS_s *character_instance = instance->character_instance;
            if (character_instance != NULL) {
                NUGCUTSCENE_s *cutscene = instance->cutscene;
                const f32 frame = instance->render_frame;
                NUGCUTCHARSYS_s *system = cutscene->character_system;
                if (NuCutSceneCharacterRender != NULL) {
                    for (i32 i = 0; i < system->character_count; ++i) {
                        instNUGCUTCHAR_s *character = &character_instance->characters[i];
                        if (character->character_model != NULL) {
                            NuCutSceneCharacterRender(instance, cutscene, character, &system->characters[i], frame,
                                                      paused);
                        }
                    }
                }
            }

            if ((instance->flags_8c & 0x40) != 0 && instance->current_frame == instance->cutscene->duration - 1.0f) {
                continue;
            }
            if ((instance->flags_8b & 0x40) != 0) {
                continue;
            }
            instNUGCUTLOCATORSYS_s *locator_instance = instance->locator_instance;
            if (locator_instance != NULL) {
                NUGCUTSCENE_s *cutscene = instance->cutscene;
                const f32 frame = instance->render_frame;
                NUGCUTLOCATORSYS_s *system = cutscene->locator_system;
                for (u32 i = 0; i < system->locator_count; ++i) {
                    NUGCUTLOCATOR_s *locator = &system->locators[i];
                    if ((locator->flags & 3) == 0) {
                        if (static_cast<i8>(instance->flags_88) < 0) {
                            instNuGCutLocatorUpdate(instance, system, &locator_instance->locators[i], locator, frame,
                                                    &instance->matrix, paused);
                        } else {
                            instNuGCutLocatorUpdate(instance, system, &locator_instance->locators[i], locator, frame,
                                                    NULL, paused);
                        }
                    }
                }
            }
        }
    }
}

    void NuSetGetHGObjFromIndxFn(NUGCUTSCENEGETHGOBJFN function) {
        NuCutSceneGetHGObj = function;
    }

    void NuSetCutSceneCharacterProcessFn(NUGCUTSCENECHARACTERPROCESSFN function) {
        NuCutSceneCharacterProcess = function;
    }

    void NuSetCutSceneCharacterRenderFn(NUGCUTSCENECHARACTERRENDERFN function) {
        NuCutSceneCharacterRender = function;
    }

    void NuSetCutSceneCharacterEvalFn(NUGCUTSCENECHARACTEREVALFN function) {
        NuCutSceneCharacterEval = function;
    }

    void NuSetCutSceneCharacterReleaseFn(NUGCUTSCENECHARACTERRELEASEFN function) {
        NuCutSceneCharacterRelease = function;
    }

    void NuSetCutSceneFindCharactersFn(NUGCUTSCENEFINDCHARACTERSFN function) {
        NuCutSceneFindCharacters = function;
    }

    void NuSetCutSceneDestroyCharactersFn(void (*callback)(NUGCUTSCENE_s *)) {
        NuCutSceneDestroyCharacters = callback;
    }

    void NuSetCutSceneResetCharactersFn(NUGCUTSCENERESETCHARACTERSFN function) {
        NuCutSceneResetCharactersFn = function;
    }

    void NuSetCutSceneCharacterCreateDataFn(NUGCUTSCENECHARACTERCREATEDATAFN function) {
        NuCutSceneCharacterCreateData = function;
    }

    void NuSetCutSceneCharacterDestroyDataFn(NUGCUTSCENECHARACTERDESTROYDATAFN function) {
        NuCutSceneCharacterDestroyData = function;
    }

    void NuSetCutSceneRigidCollisionCheckFn(NUGCUTSCENERIGIDCOLLISIONCHECKFN callback) {
        NuCutSceneRigidCollisionCheck = callback;
    }

    void NuSetCutSceneSFXFixUpFn(NUGCUTSCENESFXFIXUPFN function) {
        NuCutSceneSFXFixUp = function;
    }

    void NuSetCutSceneSFXUpdateFn(NUGCUTSCENESFXUPDATEFN function) {
        NuCutSceneSFXUpdate = function;
    }

    void NuSetCutSceneRequestSFXFn(NUGCUTSCENEREQUESTSFXFN function) {
        NuCutSceneRequestSFX = function;
    }

    void NuSetCutSceneRigidPostRenderFn(NUGCUTSCENERIGIDPOSTRENDERFN function) {
        NuCutSceneRigidPostRender = function;
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

    void instNuGCutSceneAddCleanUpItem(void *handle, i32 enabled) {
        instNUGCUTSCENE_s *instance = DefragGetInstFn(handle);
        CutSceneCleanUpEntry *entry = DefragCutSceneList++;
        entry->handle = handle;
        entry->cutscene = instance->cutscene;
        entry->accumulated_duration = instance->accumulated_stream_duration;
        entry->flags = enabled ? 7 : 0;
        ++DefragCutSceneListSize;
    }

}

NUGCUTSCENE_s *RelocateCutScene(NUGCUTSCENE_s *source, variptr_u *buffer) {
    buffer->addr = ALIGN(buffer->addr, 4);
    NUGCUTSCENE_s *cutscene = reinterpret_cast<NUGCUTSCENE_s *>(buffer->void_ptr);
    isize delta = reinterpret_cast<isize>(cutscene) - reinterpret_cast<isize>(source);
    if (delta != 0) {
        source->string_delta = delta;
        source->relocation_delta = delta;
        memmove(cutscene, source, source->loaded_size);

        cutscene->strings =
            cutscene->strings != NULL
                ? reinterpret_cast<char *>(reinterpret_cast<isize>(cutscene->strings) + cutscene->string_delta)
                : NULL;
        if (cutscene->camera_system != NULL) {
            isize data_delta = cutscene->string_delta;
            const i32 version = cutscene->version & 0xff;
            cutscene->camera_system =
                reinterpret_cast<NUGCUTCAMERASYS_s *>(reinterpret_cast<isize>(cutscene->camera_system) + data_delta);
            NUGCUTCAMERASYS_s *system = cutscene->camera_system;
            system->cameras =
                system->cameras != NULL
                    ? reinterpret_cast<NUGCUTCAMERA_s *>(reinterpret_cast<isize>(system->cameras) + data_delta)
                    : NULL;
            if (__builtin_expect(version > 4, 1)) {
                if (system->focus_animation != NULL) {
                    system->focus_animation =
                        static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(system->focus_animation, delta, 1, 0));
                }
                system->focus_state_animation = StateAnimFixPtrs(system->focus_state_animation, delta);
            }
            if (system->animation != NULL) {
                system->animation = static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(system->animation, delta, 1, 0));
            }
            system->state_animation = StateAnimFixPtrs(system->state_animation, delta);
        }

        if (cutscene->locator_system != NULL) {
            isize data_delta = cutscene->string_delta;
            cutscene->locator_system =
                reinterpret_cast<NUGCUTLOCATORSYS_s *>(reinterpret_cast<isize>(cutscene->locator_system) + data_delta);
            NUGCUTLOCATORSYS_s *system = cutscene->locator_system;
            if (system->locators != NULL) {
                system->locators =
                    reinterpret_cast<NUGCUTLOCATOR_s *>(reinterpret_cast<isize>(system->locators) + data_delta);
            } else {
                system->locators = NULL;
            }
            if (system->locators != NULL) {
                for (i32 i = 0; i < system->locator_count; ++i) {
                    NUGCUTLOCATOR_s *locator = &system->locators[i];
                    if (locator->animation != NULL) {
                        locator->animation =
                            static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(locator->animation, delta, 1, 0));
                    }
                }
            }
            system->types =
                system->types != NULL
                    ? reinterpret_cast<NUGCUTLOCATORTYPE_s *>(reinterpret_cast<isize>(system->types) + data_delta)
                    : NULL;
            if (system->types != NULL) {
                for (i32 i = 0; i < system->type_count; ++i) {
                    if (system->types[i].name != NULL) {
                        system->types[i].name =
                            reinterpret_cast<char *>(reinterpret_cast<isize>(system->types[i].name) + data_delta);
                    }
                }
            }
        }

        if (cutscene->rigid_system != NULL) {
            isize data_delta = cutscene->string_delta;
            cutscene->rigid_system =
                reinterpret_cast<NUGCUTRIGIDSYS_s *>(reinterpret_cast<isize>(cutscene->rigid_system) + data_delta);
            NUGCUTRIGIDSYS_s *system = cutscene->rigid_system;
            if (system->rigids != NULL) {
                system->rigids =
                    reinterpret_cast<NUGCUTRIGID_s *>(reinterpret_cast<isize>(system->rigids) + data_delta);
            } else {
                system->rigids = NULL;
            }
            if (system->rigids != NULL) {
                for (i32 i = 0; i < system->count; ++i) {
                    NUGCUTRIGID_s *rigid = &system->rigids[i];
                    if (rigid->name != NULL) {
                        rigid->name = reinterpret_cast<char *>(reinterpret_cast<isize>(rigid->name) + data_delta);
                    }
                    if (rigid->animation != NULL) {
                        rigid->animation =
                            static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(rigid->animation, delta, 1, 0));
                    }
                    rigid->state_animation = StateAnimFixPtrs(rigid->state_animation, delta);
                }
            }
        }

        if (cutscene->character_system != NULL) {
            isize data_delta = cutscene->string_delta;
            cutscene->character_system =
                reinterpret_cast<NUGCUTCHARSYS_s *>(reinterpret_cast<isize>(cutscene->character_system) + data_delta);
            NUGCUTCHARSYS_s *system = cutscene->character_system;
            system->characters =
                system->characters != NULL
                    ? reinterpret_cast<NUGCUTCHAR_s *>(reinterpret_cast<isize>(system->characters) + data_delta)
                    : NULL;
            if (system->characters != NULL) {
                for (i32 i = 0; i < system->character_count; ++i) {
                    NUGCUTCHAR_s *character = &system->characters[i];
                    if (character->name != NULL) {
                        character->name =
                            reinterpret_cast<char *>(reinterpret_cast<isize>(character->name) + data_delta);
                    }
                    if (character->animation != NULL) {
                        character->animation =
                            static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(character->animation, delta, 1, 0));
                    }
                    if (character->face_animation != NULL) {
                        character->face_animation =
                            static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(character->face_animation, delta, 1, 0));
                    }
                    if (character->extra_animation != NULL) {
                        character->extra_animation =
                            static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(character->extra_animation, delta, 1, 0));
                    }
                }
            }
            if (static_cast<u8>(cutscene->version) > 3 && cutscene->character_animations != NULL) {
                cutscene->character_animations = reinterpret_cast<NUGCUTCHARANIM_s *>(
                    reinterpret_cast<isize>(cutscene->character_animations) + data_delta);
                if (system->characters != NULL && cutscene->character_animations != NULL) {
                    for (i32 i = 0; i < system->character_count; ++i) {
                        NUGCUTCHARANIM_s *animation = &cutscene->character_animations[i];
                        if (animation->animation != NULL) {
                            animation->animation =
                                static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(animation->animation, delta, 1, 0));
                        }
                    }
                }
            }
        }

        if (cutscene->trigger_system != NULL) {
            isize data_delta = cutscene->string_delta;
            cutscene->trigger_system =
                reinterpret_cast<NUGCUTTRIGGERSYS_s *>(reinterpret_cast<isize>(cutscene->trigger_system) + data_delta);
            NUGCUTTRIGGERSYS_s *system = cutscene->trigger_system;
            if (system->events != NULL) {
                system->events =
                    reinterpret_cast<NUGCUTTRIGGEREVENT_s *>(reinterpret_cast<isize>(system->events) + data_delta);
            } else {
                system->events = NULL;
            }
            if (system->events != NULL) {
                auto system_event_count = system->event_count;
                for (i32 i = 0; i < system_event_count; ++i) {
                    NUGCUTTRIGGEREVENT_s *event = &system->events[i];
                    event->field_04 =
                        event->field_04 != NULL
                            ? reinterpret_cast<void *>(reinterpret_cast<isize>(event->field_04) + data_delta)
                            : NULL;
                    event->state_animation = StateAnimFixPtrs(event->state_animation, delta);
                }
            }
        }
        cutscene->bounds =
            cutscene->bounds != NULL
                ? reinterpret_cast<void *>(reinterpret_cast<isize>(cutscene->bounds) + cutscene->string_delta)
                : NULL;
    }
    buffer->addr += cutscene->loaded_size;
    return cutscene;
}

extern "C" {
    void *instNuGCutSceneCleanUp(void) {
        CutSceneCleanUpEntry *end = DefragCutSceneListBase + DefragCutSceneListSize;

        for (CutSceneCleanUpEntry *entry = DefragCutSceneListBase; entry < end; ++entry) {
            if ((entry->flags & 1) != 0) {
                continue;
            }
            NUGCUTSCENE_s *scene = entry->cutscene;
            CutSceneCleanUpEntry *other;
            for (other = entry; other < end; ++other) {
                if ((other->flags & 1) != 0 && other->cutscene == scene) {
                    break;
                }
            }
            if (other < end) {
                for (other = entry; other < end; ++other) {
                    if ((other->flags & 1) != 0 && other->cutscene == scene) {
                        other->flags |= 1;
                    }
                }
            } else if (*reinterpret_cast<u32 *>(scene) != 0x44454144u) {
                NuGCutSceneDestroy(scene);
                *reinterpret_cast<u32 *>(entry->cutscene) = 0x44454144u;
            }
        }

        for (CutSceneCleanUpEntry *entry = DefragCutSceneListBase; entry < end; ++entry) {
            instNuGCutSceneDestroy(DefragGetInstFn(entry->handle));
        }

        VARIPTR cursor __attribute__((aligned(16)));
        cursor.void_ptr = DefragCutSceneBaseMem;
        for (CutSceneCleanUpEntry *first = NULL; DefragCutSceneListBase < end;) {
            u32 lowest = 0xffffffffu;
            for (CutSceneCleanUpEntry *entry = DefragCutSceneListBase; entry < end; ++entry) {
                u32 address = static_cast<u32>(reinterpret_cast<usize>(entry->cutscene));
                if ((entry->flags & 1) != 0 && address < lowest) {
                    lowest = address;
                    first = entry;
                }
            }
            if (lowest == 0xffffffffu) {
                break;
            }
            NUGCUTSCENE_s *old_scene = reinterpret_cast<NUGCUTSCENE_s *>(static_cast<usize>(lowest));
            NUGCUTSCENE_s *new_scene = RelocateCutScene(old_scene, &cursor);
            for (CutSceneCleanUpEntry *entry = first; entry < end; ++entry) {
                if ((entry->flags & 1) != 0 && entry->cutscene == old_scene) {
                    entry->cutscene = new_scene;
                    entry->flags ^= 1;
                }
            }
        }
        if (DefragInstBaseMem != NULL) {
            cursor.void_ptr = DefragInstBaseMem;
        }
        for (CutSceneCleanUpEntry *entry = DefragCutSceneListBase; entry < end; ++entry) {
            if (entry->flags == 0) {
                DefragInstDestroyedFn(entry->handle);
            } else {
                instNUGCUTSCENE_s *instance =
                    static_cast<instNUGCUTSCENE_s *>(DefragCreateInstFn(entry->handle, entry->cutscene, &cursor));
                instance->accumulated_stream_duration = entry->accumulated_duration;
            }
        }
        if (background_cutscene_instances != NULL) {
            active_cutscene_instances = background_cutscene_instances;
            background_cutscene_instances = NULL;
        }
        return cursor.void_ptr;
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

}
