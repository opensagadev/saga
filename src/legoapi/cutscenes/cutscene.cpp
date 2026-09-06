#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nugcutscene.h"
#include "legoapi/world/world_shared.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "legoapi/characters/core/character.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
struct CUTSCENEPLAYERCLIP;
struct instNUGCUTCHAR_s;
struct NUGCUTCHAR_s;
struct NUGCUTRIGID_s;
struct instNUGCUTRIGID_s;
extern "C" void instNuGCutSceneEnd(instNUGCUTSCENE_s *instance);

i32 CUTCOUNT = 0;
CUTINFO *CutList = NULL;
i32 ACTIVECUTCOUNT = 0;
i32 CS_area = 0;
CUTSYS *CS_cutsys = NULL;
WORLDINFO *CS_worldinfo = NULL;
f32 CutSceneScale = 1.0f;
i32 CUTCAM = 0;
i32 CUTCAMONLY = 0;
NUMTX cutscenecammtx = {};
i32 cutscenecamchange = 0;
f32 cutscenecam_fstop = 0.0f;
u8 cutscenecam_usefocusloc = 0;
NUVEC cutscenecam_focusloc = {};
f32 cutscenecam_focusDistance = 0.0f;
f32 cutscenecam_focalLength = 0.0f;
i32 CameraDOFHack = 0;
u8 set_cutscenecammtx = 0;
CHARSCENE_s *CharScene_Area = NULL;

__attribute__((noinline)) static void CutScene_Configure_Load(CUTINFO *cut, char *name, VARIPTR *buf,
                                                              VARIPTR *buf_end) {
    (void)buf;
    (void)buf_end;
    cut->state_count = 0;
    cut->flags = 3;
    cut->frames_per_second = 30.0f;
    cut->field_60 = 1.0f;
    cut->camera_near_clip = 0.0f;
    cut->camera_far_clip = 0;
    cut->skip_level = -1;
    cut->linked_audio = 0;
    cut->debris_render_group = 2;
    cut->door_name[0] = '\0';
    cut->next_cutscene[0] = '\0';
    cut->end_flags = 0;
    cut->music_handle = -1;

    u8 *bytes = reinterpret_cast<u8 *>(cut);
    *reinterpret_cast<u16 *>(bytes + 0x70) = 0xffff;
    *reinterpret_cast<u16 *>(bytes + 0x84) = 0xffff;
    *reinterpret_cast<u16 *>(bytes + 0x98) = 0xffff;
    *reinterpret_cast<u16 *>(bytes + 0xac) = 0xffff;
    *reinterpret_cast<u16 *>(bytes + 0xc0) = 0xffff;
    *reinterpret_cast<u16 *>(bytes + 0xea) = 0xffff;
    bytes[0xef] = 0xff;
    bytes[0xf0] = 0xff;
    bytes[0xf1] = 0xff;
    bytes[0xf2] = 0xff;
    memset(bytes + 0x164, 0, 0x20);
    *reinterpret_cast<i32 *>(bytes + 0x148) = -1;
    *reinterpret_cast<i32 *>(bytes + 0x150) = -1;
    *reinterpret_cast<i32 *>(bytes + 0x158) = -1;
    *reinterpret_cast<i32 *>(bytes + 0x160) = -1;
    *reinterpret_cast<u32 *>(bytes + 0x184) = 0;
    *reinterpret_cast<u16 *>(bytes + 0x188) = 0;
    bytes[0x18b] = 0;
    memset(bytes + 0x190, 0, 8);

    NUFPAR *fp = NuFParCreate(name);
    if (fp == NULL) {
        return;
    }
    while (NuFParGetLine(fp) != 0) {
        if (NuFParGetWord(fp) == 0) {
            continue;
        }
        if (NuStrICmp(fp->word_buf, "level_intro") == 0) {
            cut->flags |= 0x1000;
        } else if (NuStrICmp(fp->word_buf, "in_game") == 0) {
            cut->flags = (cut->flags & ~3U) | 0x800;
        } else if (NuStrICmp(fp->word_buf, "goto_level") == 0 && NuFParGetWord(fp) != 0) {
            i32 level_index = -1;
            LEVELDATA *level = Level_FindByName(fp->word_buf, &level_index);
            if (level_index != -1 && level == HUB_LDATA && CS_area != -1) {
                i32 status_index = -1;
                Area_FindStatusLevel(&ADataList[CS_area], &status_index);
                if (status_index != -1) {
                    level_index = status_index;
                }
            }
            cut->skip_level = static_cast<i16>(level_index);
        } else if (NuStrICmp(fp->word_buf, "next_cut_scene") == 0 && NuFParGetWord(fp) != 0) {
            if (NuStrLen(fp->word_buf) < static_cast<i32>(sizeof(cut->next_cutscene))) {
                NuStrCpy(cut->next_cutscene, fp->word_buf);
                cut->linked_audio = 1;
            }
        } else if (NuStrICmp(fp->word_buf, "fpsec") == 0) {
            cut->frames_per_second = NuFParGetFloat(fp);
        } else if (NuStrICmp(fp->word_buf, "nearclip") == 0) {
            cut->camera_near_clip = NuFParGetFloat(fp);
        } else if (NuStrICmp(fp->word_buf, "farclip") == 0) {
            cut->camera_far_clip = static_cast<u16>(NuFParGetInt(fp));
        } else if (NuStrICmp(fp->word_buf, "reflect_range") == 0) {
            bytes[0xf2] = static_cast<i8>(NuFParGetInt(fp));
        } else if (NuStrICmp(fp->word_buf, "holdaudio") == 0) {
            cut->linked_audio = 1;
        } else if (NuStrICmp(fp->word_buf, "snap_out") == 0) {
            cut->flags |= 0x10;
        } else if (NuStrICmp(fp->word_buf, "nextcutscene_inplayablelevel") == 0) {
            cut->end_flags |= 2;
        } else if (NuStrICmp(fp->word_buf, "looping") == 0) {
            cut->flags |= 0x200;
        } else if (NuStrICmp(fp->word_buf, "draw_world") == 0 && NuFParGetWord(fp) != 0 &&
                   NuStrICmp(fp->word_buf, "off") == 0) {
            cut->flags &= ~2U;
        }
    }
    NuFParDestroy(fp);
}

void *CutScenes_Load(char *config, NUGSCN *gscn1, NUGSCN *gscn2, i32 param1, VARIPTR *buf, VARIPTR *buf_end, i32 param2,
                     i32 param3, WORLDINFO *world) {
    NUFPAR *fp;
    CUTSYS *sys;
    void *initial;
    CUTINFO *cut;
    char name[128];
    char full_path[128];
    CUTINFO *entries[32];

    if (CutList != NULL && CUTCOUNT > 0) {
        sys = (CUTSYS *)ALIGN(buf->addr, 4);
        sys->cuts = reinterpret_cast<CUTINFO **>(sys + 1);
        sys->count = CUTCOUNT;
        CS_area = param2;
        CS_worldinfo = world;
        CS_cutsys = sys;
        buf->void_ptr = reinterpret_cast<char *>(sys->cuts) + CUTCOUNT * sizeof(CUTINFO *);
        return sys;
    }
    if (config == NULL) {
        return NULL;
    }

    fp = NuFParCreateMem((char *)"cutscenes", config, 0xffff);
    if (fp == NULL) {
        return NULL;
    }

    initial = buf->void_ptr;
    sys = (CUTSYS *)ALIGN((usize)initial, 4);
    sys->cuts = reinterpret_cast<CUTINFO **>(sys + 1);
    sys->count = 0;
    sys->character_bits = reinterpret_cast<u32 *>(sys + 1);
    CS_area = param2;
    CS_worldinfo = world;
    CS_cutsys = sys;
    buf->void_ptr = reinterpret_cast<char *>(sys->character_bits) + ((CHARCOUNT + 0x1f) >> 5) * 4;

    while (NuFParGetLine(fp) != 0) {
        if (NuFParGetWord(fp) == 0 || NuStrICmp(fp->word_buf, "cutscene") != 0 || sys->count > 0x1f ||
            NuFParGetWord(fp) == 0) {
            continue;
        }

        cut = (CUTINFO *)ALIGN(buf->addr, 4);
        entries[sys->count] = cut;
        buf->void_ptr = cut + 1;
        NuStrCpy(name, fp->word_buf);
        for (char *lower = name; *lower != '\0'; ++lower) {
            *lower = (char)NuToLower((u8)*lower);
        }
        i32 len = NuStrLen(name);
        while (len > 0 && name[len - 1] != '.') {
            --len;
        }
        if (len > 0) {
            name[len - 1] = '\0';
        }

        if (name[0] == 'c' && name[1] == 'u' && name[2] == 't' && name[3] == '\\') {
            NuStrCpy(name, name + 4);
        }
        NuStrCpy(full_path, "cut\\");
        NuStrCat(full_path, name);
        NuStrCat(full_path, ".txt");
        CutScene_Configure_Load(cut, full_path, buf, buf_end);

        if ((reinterpret_cast<u8 *>(cut)[0x51] & 8) == 0 && !InStory()) {
            continue;
        }
        buf->void_ptr = (char *)ALIGN(buf->addr, 0x40);
        NuStrCpy(full_path, "cut\\");
        NuStrCat(full_path, name);
        NuStrCat(full_path, ".cu2");
        cut->scene = NuGCutSceneLoad(full_path, buf, buf_end, 0);
        if (cut->scene == NULL) {
            NuStrCpy(full_path, "cut\\");
            NuStrCat(full_path, name);
            NuStrCat(full_path, ".cut");
            cut->scene = NuGCutSceneLoad(full_path, buf, buf_end, 0);
            if (cut->scene == NULL) {
                continue;
            }
        }
        char *base_name = name;
        for (char *cursor = name; *cursor != '\0'; ++cursor) {
            if (*cursor == '\\') {
                base_name = cursor + 1;
            }
        }
        NuStrCpy(cut->name, base_name);
        NuGCutSceneFixUp(reinterpret_cast<NUGCUTSCENE_s *>(cut->scene), gscn1, 0, static_cast<i8>(param1));
        NuGCutSceneFixUpExtra(reinterpret_cast<NUGCUTSCENE_s *>(cut->scene), gscn2);
        cut->instance = instNuGCutSceneCreate(reinterpret_cast<NUGCUTSCENE_s *>(cut->scene), NULL, NULL, name, buf, 0);
        if (cut->instance != NULL) {
            reinterpret_cast<instNUGCUTSCENE_s *>(cut->instance)->rate = cut->frames_per_second * DEFAULTFRAMETIME;
            buf->void_ptr = (char *)ALIGN(buf->addr, 0x10);
        }
        sys->count++;
    }
    NuFParDestroy(fp);
    if (sys->count == 0) {
        buf->void_ptr = initial;
        return NULL;
    }
    buf->void_ptr = reinterpret_cast<void *>(ALIGN(buf->addr, 4));
    sys->cuts = reinterpret_cast<CUTINFO **>(buf->void_ptr);
    memmove(sys->cuts, entries, sys->count * sizeof(CUTINFO *));
    buf->void_ptr = reinterpret_cast<char *>(buf->void_ptr) + sys->count * sizeof(CUTINFO *);
    return sys;
}

void CharScenes_LevelLoad(WORLDINFO *world) {
    if (CHARCOUNT <= 0) {
        return;
    }

    for (i32 i = 0; i < CHARCOUNT; i++) {
        CHARSCENE_s *entry = &world->minikit.character_scenes[i];
        entry->scene = NULL;

        // Check if we should load this character scene
        if ((CharScene_Area == NULL || CharScene_Area[i].scene == NULL) && (CDataList[i].flags & 1) != 0 &&
            world->cutscene_sys != NULL) {
            // Check if this character is in a cutscene
            u32 *cutscene_flags = *(u32 **)((char *)world->cutscene_sys + 8);
            u32 flag = (cutscene_flags[i >> 5] >> (i & 0x1f)) & 1;
            if (flag != 0) {
                // Load the character scene
                char path[136];
                VARIPTR buf_end = world->unknown_0108;
                sprintf(path, "chars\\%s\\%s.gsc", CDataList[i].dir, CDataList[i].file);
                NUGSCN *scene = NuGScnRead(&world->giz_buffer, buf_end, path);
                entry->scene = scene;
                if (scene != NULL) {
                    NuSpecialFind(scene, &entry->special_scene, CDataList[i].file, 1);
                }
            }
        }
    }
}

// --- Extern "C" block: functions with confirmed C linkage in original libTTapp.so ---
extern "C" void *NuAnimData2FixPtrs(void *, isize, isize, i32);
extern "C" StateAnim *StateAnimFixPtrs(StateAnim *, isize);
extern "C" i32 StateAnimEvaluate(StateAnim *, u8 *, u8 *, f32);
extern "C" void NuAnimCurve2SetApplyToMatrix_3(ani3_animheader_s *, i32, f32, NUMTX *);
extern "C" i32 LookupDebrisEffectPage(char *, i32);
extern "C" i32 LookupDebrisEffectPageOnly(char *, i32);
extern "C" {
    extern i32 NuGCutDebFixUp_SearchAllPages;
    extern NUGCUTLOCATORFNENTRY_s *locatorfns;
    extern i32 (*LookupLocatorVfxFn)(char *);
    extern i32 (*NuCutSceneSFXFixUp)(usize);
}
void NuGCutRigidCalcMtx(NUGCUTRIGID_s *, f32, numtx_s *);

static instNUGCUTSCENE_s *active_cutscene_instances;

static void NuGCutSceneFixPtrs_Title(NUGCUTSCENE_s *cutscene, isize anim_delta) {
    usize data_delta = (usize)cutscene->string_delta;
    if (cutscene->strings != NULL) {
        cutscene->strings = reinterpret_cast<char *>(reinterpret_cast<usize>(cutscene->strings) + data_delta);
    }
    if (cutscene->camera_system != NULL) {
        cutscene->camera_system = reinterpret_cast<NUGCUTCAMERASYS_s *>(
            reinterpret_cast<usize>(cutscene->camera_system) + data_delta);
        NUGCUTCAMERASYS_s *system = cutscene->camera_system;
        if (system->cameras != NULL) {
            system->cameras =
                reinterpret_cast<NUGCUTCAMERA_s *>(reinterpret_cast<usize>(system->cameras) + data_delta);
        }
        if (anim_delta != 0) {
            system->animation =
                static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(system->animation, anim_delta, 0, 0));
            system->state_animation = StateAnimFixPtrs(system->state_animation, anim_delta);
            if (cutscene->version > 4) {
                system->focus_animation =
                    static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(system->focus_animation, anim_delta, 0, 0));
                system->focus_state_animation = StateAnimFixPtrs(system->focus_state_animation, anim_delta);
            }
        }
    }
    if (cutscene->rigid_system != NULL) {
        cutscene->rigid_system =
            reinterpret_cast<NUGCUTRIGIDSYS_s *>(reinterpret_cast<usize>(cutscene->rigid_system) + data_delta);
        NUGCUTRIGIDSYS_s *system = cutscene->rigid_system;
        if (system->rigids != NULL) {
            system->rigids = reinterpret_cast<NUGCUTRIGID_s *>(reinterpret_cast<usize>(system->rigids) + data_delta);
            for (u32 i = 0; i < system->count; ++i) {
                NUGCUTRIGID_s *rigid = &system->rigids[i];
                if (rigid->name != NULL) {
                    rigid->name = reinterpret_cast<char *>(reinterpret_cast<usize>(rigid->name) +
                                                           reinterpret_cast<usize>(cutscene->strings) - 1);
                }
                rigid->animation = static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(rigid->animation, anim_delta, 0, 0));
                rigid->state_animation = StateAnimFixPtrs(rigid->state_animation, anim_delta);
            }
        }
    }
    if (cutscene->character_system != NULL) {
        cutscene->character_system = reinterpret_cast<NUGCUTCHARSYS_s *>(
            reinterpret_cast<usize>(cutscene->character_system) + data_delta);
        NUGCUTCHARSYS_s *system = cutscene->character_system;
        if (system->characters != NULL) {
            system->characters =
                reinterpret_cast<NUGCUTCHAR_s *>(reinterpret_cast<usize>(system->characters) + data_delta);
            for (u32 i = 0; i < system->character_count; ++i) {
                NUGCUTCHAR_s *character = &system->characters[i];
                if (character->name != NULL) {
                    character->name = reinterpret_cast<char *>(reinterpret_cast<usize>(character->name) +
                                                               reinterpret_cast<usize>(cutscene->strings) - 1);
                }
                if (anim_delta != 0) {
                    character->animation =
                        static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(character->animation, anim_delta, 0, 0));
                    character->face_animation =
                        static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(character->face_animation, anim_delta, 0, 0));
                    character->extra_animation =
                        static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(character->extra_animation, anim_delta, 0, 0));
                }
            }
        }
    }
    if (cutscene->version > 3 && cutscene->character_animations != NULL) {
        cutscene->character_animations = reinterpret_cast<NUGCUTCHARANIM_s *>(
            reinterpret_cast<usize>(cutscene->character_animations) + data_delta);
        if (cutscene->character_system != NULL && cutscene->character_system->characters != NULL) {
            for (u32 i = 0; i < cutscene->character_system->character_count; ++i) {
                NUGCUTCHARANIM_s *animation = &cutscene->character_animations[i];
                if (animation->animation != NULL) {
                    animation->animation =
                        static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(animation->animation, anim_delta, 0, 0));
                }
            }
        }
    }
    if (cutscene->locator_system != NULL) {
        cutscene->locator_system =
            reinterpret_cast<NUGCUTLOCATORSYS_s *>(reinterpret_cast<usize>(cutscene->locator_system) + data_delta);
        NUGCUTLOCATORSYS_s *system = cutscene->locator_system;
        if (system->locators != NULL) {
            system->locators =
                reinterpret_cast<NUGCUTLOCATOR_s *>(reinterpret_cast<usize>(system->locators) + data_delta);
            for (u32 i = 0; i < system->locator_count; ++i) {
                NUGCUTLOCATOR_s *locator = &system->locators[i];
                locator->animation =
                    static_cast<nuanimdata2_s *>(NuAnimData2FixPtrs(locator->animation, anim_delta, 0, 0));
            }
        }
        if (system->types != NULL) {
            system->types =
                reinterpret_cast<NUGCUTLOCATORTYPE_s *>(reinterpret_cast<usize>(system->types) + data_delta);
            for (u32 i = 0; i < system->type_count; ++i) {
                if (system->types[i].name != NULL) {
                    system->types[i].name = reinterpret_cast<char *>(reinterpret_cast<usize>(system->types[i].name) +
                                                                     reinterpret_cast<usize>(cutscene->strings) - 1);
                }
            }
        }
    }
    if (cutscene->bounds != NULL) {
        cutscene->bounds = reinterpret_cast<void *>(reinterpret_cast<usize>(cutscene->bounds) + data_delta);
    }
    if (cutscene->trigger_system != NULL) {
        cutscene->trigger_system =
            reinterpret_cast<void *>(reinterpret_cast<usize>(cutscene->trigger_system) + data_delta);
    }
}

extern "C" {

    i32 NuGCutDebFixUp_SearchAllPages = 0;
    NUGCUTLOCATORFNENTRY_s *locatorfns = NULL;
    i32 (*LookupLocatorVfxFn)(char *) = NULL;
    i32 (*NuCutSceneSFXFixUp)(usize) = NULL;
    NUGCUTSCENE_s *NuGCutSceneLoad(char *name, VARIPTR *buf, VARIPTR *buf_end, i32 flags) {
        (void)flags;
        buf->addr = ALIGN(buf->addr, 0x10);
        NUGCUTSCENE_s *cutscene = reinterpret_cast<NUGCUTSCENE_s *>(buf->void_ptr);
        i32 bytes = NuFileLoadBuffer(name, cutscene, static_cast<i32>(buf_end->addr - buf->addr));
        if (bytes == 0) {
            return NULL;
        }
        usize anim_delta = reinterpret_cast<usize>(cutscene) - (usize)cutscene->relocation_delta;
        cutscene->string_delta = (isize)(reinterpret_cast<usize>(cutscene) - (usize)cutscene->string_delta);
        cutscene->relocation_delta = (isize)anim_delta;
        cutscene->loaded_size = bytes;
        NuGCutSceneFixPtrs_Title(cutscene, (isize)anim_delta);
        buf->addr += bytes;
        if (cutscene->version > 9) {
            NuGCutSceneRemapFocusIdToLocaterNum(cutscene, buf);
        }
        return cutscene;
    }
    void NuGCutSceneFixUp(NUGCUTSCENE_s *cutscene, NUGSCN *scene, i32 flags, i8 area) {
        if (cutscene == NULL) {
            return;
        }
        if (cutscene->version > 1) {
            cutscene->scene = scene;
            cutscene->extra_scene = reinterpret_cast<void *>(static_cast<usize>(flags));
        }
        if (scene != NULL && cutscene->rigid_system != NULL && cutscene->rigid_system->rigids != NULL) {
            for (u32 i = 0; i < cutscene->rigid_system->count; ++i) {
                NUGCUTRIGID_s *rigid = &cutscene->rigid_system->rigids[i];
                nuhspecial_s special;
                if (NuSpecialFind(scene, &special, rigid->name, 1) != 0) {
                    rigid->flags |= 4;
                    rigid->scene = special.scene;
                    rigid->special_object = special.display_special != NULL ? special.display_special : special.special;
                }
                if (rigid->locator_count != 0 && cutscene->locator_system != NULL && rigid->locator < 0xff) {
                    rigid->locator_index = static_cast<u8>(rigid->locator);
                    rigid->locator = reinterpret_cast<usize>(&cutscene->locator_system->locators[rigid->locator_index]);
                } else {
                    rigid->locator_index = 0xff;
                }
            }
        }
        if (cutscene->character_system != NULL && NuCutSceneFindCharacters != NULL) {
            NuCutSceneFindCharacters(cutscene);
        }
        NUGCUTLOCATORSYS_s *system = cutscene->locator_system;
        if (system == NULL || system->types == NULL || system->type_count == 0) {
            return;
        }
        for (u32 i = 0; i < system->type_count; ++i) {
            NUGCUTLOCATORTYPE_s *type = &system->types[i];
            if ((type->flags & 1) != 0) {
                if (NuGCutDebFixUp_SearchAllPages == 0) {
                    type->function_index = static_cast<u16>(LookupDebrisEffectPageOnly(type->name, area));
                } else {
                    type->function_index = static_cast<u16>(LookupDebrisEffectPage(type->name, area));
                }
            } else if ((type->flags & 2) != 0) {
                i32 function_index = -1;
                if (locatorfns != NULL && type->name != NULL && locatorfns[0].name != NULL) {
                    for (i32 j = 0; locatorfns[j].name != NULL; ++j) {
                        if (NuStrICmp(type->name, locatorfns[j].name) == 0) {
                            function_index = j;
                            break;
                        }
                    }
                }
                type->function_index = static_cast<u16>(function_index);
            } else if ((type->flags & 0x10) != 0 && LookupLocatorVfxFn != NULL && type->name != NULL) {
                char *underscore = NuStrRChr(type->name, '_');
                if (underscore != NULL && underscore[1] > '/' && underscore[1] < ':') {
                    *underscore = '\0';
                }
                type->function_index = static_cast<u16>(LookupLocatorVfxFn(type->name));
            } else if ((type->flags & 4) != 0 && NuCutSceneSFXFixUp != NULL && type->name != NULL) {
                type->function_index = static_cast<u16>(NuCutSceneSFXFixUp(reinterpret_cast<usize>(type->name)));
                if (static_cast<i16>(type->function_index) != -1) {
                    cutscene->flags |= 4;
                }
            }
        }
    }
    void NuGCutSceneFixUpExtra(NUGCUTSCENE_s *cutscene, NUGSCN *area) {
        if (cutscene == NULL || cutscene->rigid_system == NULL || area == NULL) {
            return;
        }

        NUGCUTRIGIDSYS_s *system = cutscene->rigid_system;
        for (u32 i = 0; i < system->count; ++i) {
            NUGCUTRIGID_s *rigid = &system->rigids[i];
            if ((rigid->flags & 4) != 0) {
                continue;
            }

            nuhspecial_s special;
            if (NuSpecialFind(area, &special, rigid->name, 1) == 0) {
                continue;
            }
            rigid->flags |= 4;
            rigid->scene = special.scene;
            rigid->special_object = special.display_special != NULL ? special.display_special : special.special;
        }
    }
    instNUGCUTSCENE_s *instNuGCutSceneCreate(NUGCUTSCENE_s *cutscene, NUGSCN *scene, void *extra, char *name,
                                             VARIPTR *buf, i32 flags) {
        (void)scene;
        (void)extra;
        (void)flags;
        if (cutscene == NULL) {
            return NULL;
        }
        buf->addr = ALIGN(buf->addr, 0x10);
        instNUGCUTSCENE_s *instance = reinterpret_cast<instNUGCUTSCENE_s *>(buf->void_ptr);
        buf->void_ptr = instance + 1;
        memset(instance, 0, sizeof(*instance));
        instance->alpha = 1.0f;
        instance->cutscene = cutscene;
        instance->cutscene_copy = cutscene;
        instance->current_frame = 1.0f;
        instance->render_frame = 1.0f;
        instance->rate = 1.0f;
        NuMtxSetIdentity(&instance->matrix);
        if (name != NULL) {
            snprintf(instance->name, sizeof(instance->name), "%s", name);
        }

        if (cutscene->camera_system != NULL && cutscene->camera_system->camera_count != 0) {
            buf->addr = ALIGN(buf->addr, 0x10);
            instance->camera_instance = reinterpret_cast<instNUGCUTSCENECAMERA_s *>(buf->void_ptr);
            buf->void_ptr = instance->camera_instance + 1;
            memset(instance->camera_instance, 0, sizeof(*instance->camera_instance));
            buf->addr = ALIGN(buf->addr, 0x10);
            instance->camera_instance->camera_states = reinterpret_cast<instNUGCUTCAMSTATE_s *>(buf->void_ptr);
            buf->void_ptr = instance->camera_instance->camera_states + cutscene->camera_system->camera_count;
            memset(instance->camera_instance->camera_states, 0,
                   cutscene->camera_system->camera_count * sizeof(instNUGCUTCAMSTATE_s));
        }

        if (cutscene->rigid_system != NULL && cutscene->rigid_system->count != 0) {
            instance->rigid_instance = reinterpret_cast<instNUGCUTRIGIDSYS_s *>(ALIGN(buf->addr, 4));
            buf->void_ptr = instance->rigid_instance + 1;
            buf->addr = ALIGN(buf->addr, 0x10);
            instance->rigid_instance->rigids = reinterpret_cast<instNUGCUTRIGID_s *>(buf->void_ptr);
            buf->void_ptr = instance->rigid_instance->rigids + cutscene->rigid_system->count;
            memset(instance->rigid_instance->rigids, 0, cutscene->rigid_system->count * sizeof(instNUGCUTRIGID_s));
            for (u32 i = 0; i < cutscene->rigid_system->count; ++i) {
                NUGCUTRIGID_s *rigid = &cutscene->rigid_system->rigids[i];
                instNUGCUTRIGID_s *inst_rigid = &instance->rigid_instance->rigids[i];
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
                        inst_rigid->special = reinterpret_cast<u8 *>(scene->specials) +
                                              (reinterpret_cast<u8 *>(rigid->special_object) -
                                               reinterpret_cast<u8 *>(rigid->scene->specials));
                        inst_rigid->display_special = NULL;
                    } else {
                        inst_rigid->special = NULL;
                        inst_rigid->display_special = reinterpret_cast<u8 *>(scene->display_list->specials) +
                                                      (reinterpret_cast<u8 *>(rigid->special_object) -
                                                       reinterpret_cast<u8 *>(rigid->scene->display_list->specials));
                    }
                    if ((rigid->flags & 2) != 0) {
                        inst_rigid->visible = rigid->flags & 1;
                    }
                }
            }
        }

        if (cutscene->character_system != NULL && cutscene->character_system->character_count != 0) {
            instance->character_instance = reinterpret_cast<instNUGCUTCHARSYS_s *>(ALIGN(buf->addr, 0x10));
            buf->void_ptr = instance->character_instance + 1;
            buf->addr = ALIGN(buf->addr, 0x10);
            instance->character_instance->characters = reinterpret_cast<instNUGCUTCHAR_s *>(buf->void_ptr);
            buf->void_ptr = instance->character_instance->characters + cutscene->character_system->character_count;
            memset(instance->character_instance->characters, 0,
                   cutscene->character_system->character_count * sizeof(instNUGCUTCHAR_s));
            for (u32 i = 0; i < cutscene->character_system->character_count; ++i) {
                NUGCUTCHAR_s *character = &cutscene->character_system->characters[i];
                instNUGCUTCHAR_s *inst_character = &instance->character_instance->characters[i];
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

        if (cutscene->locator_system != NULL && cutscene->locator_system->locator_count != 0) {
            instance->locator_instance = reinterpret_cast<instNUGCUTLOCATORSYS_s *>(ALIGN(buf->addr, 4));
            buf->void_ptr = instance->locator_instance + 1;
            buf->addr = ALIGN(buf->addr, 0x10);
            instance->locator_instance->locators = reinterpret_cast<instNUGCUTLOCATOR_s *>(buf->void_ptr);
            buf->void_ptr = instance->locator_instance->locators + cutscene->locator_system->locator_count;
            memset(instance->locator_instance->locators, 0,
                   cutscene->locator_system->locator_count * sizeof(instNUGCUTLOCATOR_s));
            for (u32 i = 0; i < cutscene->locator_system->locator_count; ++i) {
                NUGCUTLOCATOR_s *locator = &cutscene->locator_system->locators[i];
                NUGCUTLOCATORTYPE_s *type = &cutscene->locator_system->types[locator->type_index];
                if ((type->flags & 1) != 0 && (locator->flags & 0x20) != 0) {
                    instance->locator_instance->locators[i].effect_handle = -1;
                }
            }
        }

        instance->next = active_cutscene_instances;
        if (active_cutscene_instances != NULL) {
            active_cutscene_instances->previous = instance;
        }
        active_cutscene_instances = instance;
        instance->allocation_size = static_cast<i32>(buf->addr - reinterpret_cast<usize>(instance));
        return instance;
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
        if (instance->rate < 0.0f) {
            instance->rate = -instance->rate;
        }
        instance->cutscene = instance->cutscene_copy;
        if (instance->rigid_instance != NULL && instance->cutscene->rigid_system != NULL) {
            for (u32 i = 0; i < instance->cutscene->rigid_system->count; ++i) {
                NUGCUTRIGID_s *rigid = &instance->cutscene->rigid_system->rigids[i];
                instNUGCUTRIGID_s *inst_rigid = &instance->rigid_instance->rigids[i];
                inst_rigid->state_index = 0;
                if ((rigid->flags & 6) == 6) {
                    inst_rigid->visible = rigid->flags & 1;
                }
            }
        }
    }

    void instNuGCutSceneStart(instNUGCUTSCENE_s *instance) {
        instance->current_frame = 1.0f;
        instance->render_frame = 1.0f;
        instance->flags_89 &= 0xef;
        instance->flags_8d &= 0xef;
        instance->flags_88 = (instance->flags_88 & 0xfe) | 2;
        if (instance->rate < 0.0f) {
            instance->rate = -instance->rate;
        }
        if (instance->camera_instance != NULL && instance->cutscene->camera_system != NULL) {
            instNUGCUTSCENECAMERA_s *camera = instance->camera_instance;
            NUGCUTCAMERASYS_s *system = instance->cutscene->camera_system;
            camera->state_index = 0;
            camera->next_target_index = 0;
            camera->camera_index = static_cast<i8>(system->field_10);
            for (u32 i = 0; i < system->camera_count; ++i) {
                camera->camera_states[i].flags &= ~2U;
                camera->camera_states[i].event_index = 0;
            }
        }
        if (instance->rigid_instance != NULL && instance->cutscene->rigid_system != NULL) {
            for (u32 i = 0; i < instance->cutscene->rigid_system->count; ++i) {
                NUGCUTRIGID_s *rigid = &instance->cutscene->rigid_system->rigids[i];
                instNUGCUTRIGID_s *inst_rigid = &instance->rigid_instance->rigids[i];
                inst_rigid->state_index = 0;
                if ((rigid->flags & 6) == 6) {
                    inst_rigid->visible = rigid->flags & 1;
                }
            }
        }
    }

    void instNuGCutScenePause(instNUGCUTSCENE_s *instance, u8 paused) {
        instance->paused = paused;
    }

    void instNuGCutSceneDestroy(instNUGCUTSCENE_s *instance) {
        instance->cutscene = instance->cutscene_copy;
        if ((instance->flags_88 & 2) != 0) {
            instNuGCutSceneEnd(instance);
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
} // extern "C"

struct instNUGCUTSCENE_s;
struct NUGCUTLOCATORSYS_s;
struct instNUGCUTLOCATOR_s;
struct NUGCUTLOCATOR_s;
struct numtx_s;
extern "C" i32 NuGCutLocatorCalcMtx(NUGCUTLOCATOR_s *, f32, NUMTX *, nuanimtime_s *);
extern "C" i32 NuGCutLocatorIsVisble(NUGCUTLOCATOR_s *, f32, nuanimtime_s *, f32 *, f32 *);
extern "C" void NuAnimData2CalcTime(nuanimdata2_s *, f32, nuanimtime_s *);
extern "C" void instNuGCutLocatorUpdate(instNUGCUTSCENE_s *, NUGCUTLOCATORSYS_s *, instNUGCUTLOCATOR_s *,
                                        NUGCUTLOCATOR_s *, f32, NUMTX *, i32);
void Draw3DObjectMtx(WORLDINFO_s *, i32, numtx_s *);
extern CUTSCENESYS *CutSceneSys;

static __used__ void LocatorFunction_Blaster(instNUGCUTSCENE_s *, NUGCUTLOCATORSYS_s *, instNUGCUTLOCATOR_s *,
                                             NUGCUTLOCATOR_s *locator, float frame, numtx_s *parent_mtx, int) {
    nuanimtime_s time;
    NuAnimData2CalcTime(locator->animation, frame, &time);
    if (NuGCutLocatorIsVisble(locator, frame, &time, NULL, NULL) == 0) {
        return;
    }
    NUMTX matrix;
    NuGCutLocatorCalcMtx(locator, frame, &matrix, &time);
    if ((locator->flags & 4) != 0) {
        NuMtxPreTranslate(&matrix, &locator->pivot);
    }
    if (parent_mtx != NULL) {
        NuMtxMul(&matrix, &matrix, parent_mtx);
    }
    if (CutSceneSys != NULL) {
        Draw3DObjectMtx(NULL, CutSceneSys->blaster_object_0, &matrix);
        Draw3DObjectMtx(NULL, CutSceneSys->blaster_object_1, &matrix);
    }
}

extern "C" {
    __attribute__((weak)) NUGCUTLOCATORFNENTRY_s cutscene_locatorfns[] = {
        {"blaster", 0, 0, 0, LocatorFunction_Blaster},
        {NULL, 0, 0, 0, NULL},
    };
}

static __used__ void instNuGCutRigidSysEnd(instNUGCUTSCENE_s *, float) {
}

static void instNuGCutRigidSysUpdate(instNUGCUTSCENE_s *, float, int);
static void instNuGCutCamSysUpdate(instNUGCUTSCENE_s *, float);
extern "C" void instNuGCutSceneEnd(instNUGCUTSCENE_s *instance);

static __used__ void instNuGCutSceneUpdate(instNUGCUTSCENE_s *instance, int paused, int, float elapsed) {
    instance->flags_89 |= 4;
    if ((instance->flags_88 & 2) == 0) {
        return;
    }
    if ((instance->flags_88 & 1) == 0) {
        if (paused == 0) {
            instance->flags_88 |= 1;
        }
    } else if (paused == 0) {
        f32 frame = instance->current_frame + instance->rate * elapsed;
        f32 end_frame = instance->cutscene->duration - 1.0f;
        if (frame >= end_frame) {
            if ((instance->flags_88 & 8) != 0) {
                frame -= end_frame;
                instNuGCutSceneStart(instance);
                instance->flags_88 |= 1;
                instance->current_frame += frame;
            } else if ((instance->flags_8c & 0x40) != 0) {
                instance->current_frame = end_frame;
                instance->flags_89 |= 0x10;
            } else {
                instNuGCutSceneEnd(instance);
            }
        } else {
            instance->current_frame = frame;
        }
    }
    instance->render_frame = instance->current_frame;
    if (instance->camera_instance != NULL) {
        instNuGCutCamSysUpdate(instance, instance->render_frame);
    }
    if (instance->rigid_instance != NULL) {
        const f32 frame = (instance->flags_8a & 4) == 0
                              ? instance->render_frame
                              : instance->cutscene->duration - instance->render_frame;
        instNuGCutRigidSysUpdate(instance, frame, paused);
    }
}

static __used__ void instNuGCutCamSysUpdate(instNUGCUTSCENE_s *instance, float frame) {
    cutscenecam_focusDistance = 0.0f;
    cutscenecam_focalLength = 0.0f;
    cutscenecam_usefocusloc = 0;
    cutscenecam_fstop = 0.0f;

    NUGCUTCAMERASYS_s *system = instance->cutscene->camera_system;
    instNUGCUTSCENECAMERA_s *camera_instance = instance->camera_instance;
    if (system->focus_state_animation != NULL && instance->cutscene->version > 4) {
        u8 focus_index = 0xff;
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
        u8 camera_index = 0xff;
        if (StateAnimEvaluate(system->state_animation, &camera_instance->state_index, &camera_index, frame) != 0) {
            camera_instance->camera_index = static_cast<i8>(camera_index);
            instance->flags_8d |= 0x10;
            cutscenecamchange = 1;
        }
        const u8 state_index = camera_instance->state_index;
        if (state_index < system->state_animation->count) {
            const f32 next_state_frame = system->state_animation->times[state_index];
            if (next_state_frame - frame < 1.0f &&
                system->state_animation->values[state_index] != static_cast<u8>(camera_instance->camera_index)) {
                f32 render_frame = next_state_frame - 1.0f;
                if ((instance->flags_8a & 4) != 0) {
                    render_frame = instance->cutscene->duration - render_frame;
                }
                instance->render_frame = render_frame < 1.0f ? 1.0f : render_frame;
            }
        }
    }

    u8 target_index = camera_instance->next_target_index;
    while (target_index < camera_instance->target_count &&
           frame >= camera_instance->targets[target_index].start_frame) {
        const i8 mapped_camera =
            system->target_camera_map[camera_instance->targets[target_index].target_index];
        instNUGCUTCAMSTATE_s *state = &camera_instance->camera_states[mapped_camera];
        state->flags |= 2;
        state->event_index = target_index;
        camera_instance->next_target_index = ++target_index;
    }
    while (target_index != 0 && frame < camera_instance->targets[target_index - 1].start_frame) {
        camera_instance->next_target_index = --target_index;
        const i8 mapped_camera =
            system->target_camera_map[camera_instance->targets[target_index].target_index];
        instNUGCUTCAMSTATE_s *state = &camera_instance->camera_states[mapped_camera];
        state->flags |= 2;
        state->event_index = target_index;
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
    if ((camera->flags & 1) == 0 || system->animation == NULL ||
        NuAnimNumNodes(system->animation) <= camera->animation_node) {
        cutscenecammtx = camera->base_matrix;
    } else {
        const u32 focus_magic = system->focus_animation == NULL
                                    ? 0
                                    : *reinterpret_cast<u32 *>(system->focus_animation);
        if (instance->cutscene->version > 4 && system->focus_animation != NULL &&
            focus_magic - ANI3_MAGIC_VERSION_4 < 2) {
            f32 *values = NuAnimCurveExtractAllNodeCurves_3(
                reinterpret_cast<ani3_animheader_s *>(system->focus_animation), camera->animation_node,
                instance->render_frame, NULL);
            cutscenecam_fstop = values[2];
            cutscenecam_focalLength = values[0] * 1.3f;
            if ((camera->field_43 & 2) == 0) {
                if (system->focus_state_animation != NULL && camera_instance->focus_index >= 0) {
                    NUGCUTLOCATOR_s *focus_locator =
                        &instance->cutscene->locator_system->locators[camera_instance->focus_index];
                    if (focus_locator->animation != NULL) {
                        NUMTX focus_matrix;
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
            } else {
                cutscenecam_focusDistance = values[1];
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
    if ((camera_state->flags & 2) == 0) {
        return;
    }

    instNUGCUTCAMTGT_s *target = &camera_instance->targets[camera_state->event_index];
    f32 duration = fabsf(target->duration);
    const bool reverse = NuFsign(target->duration) < 0.0f;
    if (reverse && duration + target->start_frame <= frame) {
        camera_state->flags &= ~2U;
        return;
    }

    const NUVEC translation = *NUMTX_GET_ROW_VEC(&cutscenecammtx, 3);
    NUMTX look_at_matrix = cutscenecammtx;
    NuMtxLookAtZ(&look_at_matrix, target->target);

    f32 blend;
    if (duration > 0.01f) {
        blend = (frame - target->start_frame) / duration;
        if (blend > 1.0f || (blend >= 0.0f && reverse)) {
            blend = 1.0f - blend;
        }
    } else {
        blend = reverse ? 0.0f : 1.0f;
    }

    NUQUAT look_at_rotation;
    NUQUAT camera_rotation;
    NUQUAT blended_rotation;
    NuMtxToQuat(&look_at_rotation, &look_at_matrix);
    NuMtxToQuat(&camera_rotation, &cutscenecammtx);
    NuQuatSlerp(&blended_rotation, &camera_rotation, &look_at_rotation, blend);
    NuQuatToMtx(&blended_rotation, &cutscenecammtx);
    *NUMTX_GET_ROW_VEC(&cutscenecammtx, 3) = translation;
}

static __used__ void instNuGCutSceneClipTest(instNUGCUTSCENE_s *instance) {
    instance->flags_89 |= 4;
}

static __used__ void instNuGCutRigidSysRender(instNUGCUTSCENE_s *instance, float frame, int paused) {
    NUGCUTRIGIDSYS_s *system = instance->cutscene->rigid_system;
    if (system == NULL || instance->rigid_instance == NULL) {
        return;
    }
    for (u32 i = 0; i < system->count; ++i) {
        NUGCUTRIGID_s *rigid = &system->rigids[i];
        if ((rigid->flags & 6) != 6) {
            continue;
        }
        instNUGCUTRIGID_s *inst_rigid = &instance->rigid_instance->rigids[i];
        u8 visible = inst_rigid->visible;
        if (rigid->state_animation != NULL &&
            StateAnimEvaluate(rigid->state_animation, &inst_rigid->state_index, &visible, frame) != 0) {
            inst_rigid->visible = visible != 0;
        }
        if (inst_rigid->visible == 0) {
            continue;
        }
        NUMTX matrix;
        NuGCutRigidCalcMtx(rigid, frame, &matrix);
        if ((instance->flags_88 & 0x80) != 0) {
            NuMtxMul(&matrix, &matrix, &instance->matrix);
        }
        if (instance->alpha == 1.0f) {
            NuSpecialDrawAt(inst_rigid, &matrix);
        } else {
            NuSpecialDrawAtAlpha(inst_rigid, &matrix, instance->alpha);
        }
        if (rigid->locator_index != 0xff && rigid->locator_count != 0 && instance->cutscene->locator_system != NULL &&
            instance->locator_instance != NULL) {
            NUGCUTLOCATORSYS_s *locator_system = instance->cutscene->locator_system;
            for (u32 locator_offset = 0; locator_offset < rigid->locator_count; ++locator_offset) {
                u32 locator_index = rigid->locator_index + locator_offset;
                instNuGCutLocatorUpdate(instance, locator_system, &instance->locator_instance->locators[locator_index],
                                        &locator_system->locators[locator_index], frame, &matrix, paused);
            }
        }
    }
}

static __used__ void instNuGCutRigidSysUpdate(instNUGCUTSCENE_s *instance, float frame, int paused) {
    NUGCUTRIGIDSYS_s *system = instance->cutscene->rigid_system;
    if (system == NULL || instance->rigid_instance == NULL) {
        return;
    }

    for (u32 i = 0; i < system->count; ++i) {
        NUGCUTRIGID_s *rigid = &system->rigids[i];
        if ((rigid->flags & 4) == 0 || (rigid->flags & 2) != 0) {
            continue;
        }

        instNUGCUTRIGID_s *inst_rigid = &instance->rigid_instance->rigids[i];
        u8 visible;
        if (rigid->state_animation != NULL &&
            StateAnimEvaluate(rigid->state_animation, &inst_rigid->state_index, &visible, frame) != 0) {
            NuSpecialSetVisibility(inst_rigid, visible != 0);
        }

        if (NuSpecialGetVisibilityFn(inst_rigid) == 0) {
            continue;
        }

        NUMTX matrix;
        NuGCutRigidCalcMtx(rigid, frame, &matrix);
        if ((instance->flags_88 & 0x80) != 0) {
            NuMtxMul(&matrix, &matrix, &instance->matrix);
        }
        NuSpecialSetDrawMtx(inst_rigid, &matrix);

        if (rigid->locator_index != 0xff && rigid->locator_count != 0 && instance->cutscene->locator_system != NULL &&
            instance->locator_instance != NULL) {
            NUGCUTLOCATORSYS_s *locator_system = instance->cutscene->locator_system;
            for (u32 locator_offset = 0; locator_offset < rigid->locator_count; ++locator_offset) {
                u32 locator_index = rigid->locator_index + locator_offset;
                instNuGCutLocatorUpdate(instance, locator_system, &instance->locator_instance->locators[locator_index],
                                        &locator_system->locators[locator_index], frame, &matrix, paused);
            }
        }
    }
}

static __used__ void instNuGCutTriggerSysUpdate(instNUGCUTSCENE_s *, float) {
}

extern "C" void NuGCutSceneSysUpdate(i32 paused, i32 skip, f32 elapsed) {
    for (instNUGCUTSCENE_s *instance = active_cutscene_instances; instance != NULL; instance = instance->next) {
        instNuGCutSceneUpdate(instance, paused != 0, skip, elapsed);
    }
}

extern "C" void NuGCutSceneSysRender(f32 paused) {
    for (instNUGCUTSCENE_s *instance = active_cutscene_instances; instance != NULL; instance = instance->next) {
        const f32 frame = (instance->flags_8a & 4) == 0
                              ? instance->render_frame
                              : instance->cutscene->duration - instance->render_frame;
        if ((instance->flags_89 & 8) == 0 && (instance->flags_88 & 2) != 0 && (instance->flags_89 & 4) != 0 &&
            instance->rigid_instance != NULL) {
            instNuGCutRigidSysRender(instance, frame, static_cast<i32>(paused));
        }

        if ((instance->flags_89 & 8) == 0 && (instance->flags_88 & 2) != 0 && (instance->flags_89 & 4) != 0 &&
            instance->character_instance != NULL && instance->cutscene->character_system != NULL &&
            NuCutSceneCharacterRender != NULL) {
            NUGCUTCHARSYS_s *system = instance->cutscene->character_system;
            for (u32 i = 0; i < system->character_count; ++i) {
                instNUGCUTCHAR_s *inst_character = &instance->character_instance->characters[i];
                if (inst_character->character_model != NULL) {
                    NuCutSceneCharacterRender(instance, instance->cutscene, inst_character, &system->characters[i],
                                              frame, static_cast<i32>(paused));
                }
            }
        }

        if ((instance->flags_89 & 8) != 0 || (instance->flags_88 & 2) == 0 || (instance->flags_89 & 4) == 0 ||
            instance->locator_instance == NULL || instance->cutscene->locator_system == NULL) {
            continue;
        }
        if ((instance->flags_8c & 0x40) != 0 && instance->current_frame == instance->cutscene->duration - 1.0f) {
            continue;
        }
        if ((instance->flags_8b & 0x40) != 0) {
            continue;
        }

        NUGCUTLOCATORSYS_s *system = instance->cutscene->locator_system;
        NUMTX *parent_matrix = static_cast<i8>(instance->flags_88) < 0 ? &instance->matrix : NULL;
        for (u32 i = 0; i < system->locator_count; ++i) {
            NUGCUTLOCATOR_s *locator = &system->locators[i];
            if ((locator->flags & 3) == 0) {
                instNuGCutLocatorUpdate(instance, system, &instance->locator_instance->locators[i], locator, frame,
                                        parent_matrix, static_cast<i32>(paused));
            }
        }
    }
}

static __used__ void CutScene_Configure(CUTINFO *, char *, variptr_u *, variptr_u *) {
}

static __used__ void bgAckStreamCutScene(bgprocinfo_s *) {
}

static __used__ void bgLoadStreamCutScene(bgprocinfo_s *) {
}

static __used__ unsigned int CutScenePlayer_Accept(CUTSCENEPLAYERCLIP *) {
    return {};
}

static __used__ void CutScene_OverrideConfigFileName_LSW(char *, int, int) {
}

static __used__ void copyAnims(NUGCUTSCENE_s *, NUGCUTSCENE_s *) {
}
