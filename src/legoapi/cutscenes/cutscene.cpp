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

static i32 CS_area = 0;
CUTSYS *CS_cutsys = NULL;
static WORLDINFO *CS_worldinfo = NULL;
static CUTINFO *CS_CutInfo;
static VARIPTR *CS_buffptr;
static VARIPTR *CS_buffend;
static i32 CS_texanimcount;
static i32 CS_fadefogcount;

static void CS_play_sfx(NUFPAR *fp) {
    if (NuFParGetWord(fp) == 0) {
        return;
    }

    i32 sfx_id = GetSfxId(fp->word_buf);
    if (sfx_id == -1) {
        return;
    }

    i32 slot;
    if (CS_CutInfo->sfx[0].id == -1) {
        slot = 0;
    } else if (CS_CutInfo->sfx[1].id == -1) {
        slot = 1;
    } else if (CS_CutInfo->sfx[2].id == -1) {
        slot = 2;
    } else if (CS_CutInfo->sfx[3].id == -1) {
        slot = 3;
    } else if (CS_CutInfo->sfx[4].id == -1) {
        slot = 4;
    } else if (CS_CutInfo->sfx[5].id == -1) {
        slot = 5;
    } else {
        return;
    }

    CS_CutInfo->sfx[slot].id = static_cast<i16>(sfx_id);
    CS_CutInfo->sfx[slot].flags &= ~1U;
    while (NuFParGetWord(fp) != 0) {
        if (NuStrICmp(fp->word_buf, "frame") == 0) {
            CS_CutInfo->sfx[slot].frame = NuFParGetFloat(fp);
            if (CS_CutInfo->sfx[slot].frame < 1.0f) {
                CS_CutInfo->sfx[slot].frame = 1.0f;
            }
        } else if (NuStrICmp(fp->word_buf, "pos") == 0) {
            if (NuFParGetFloat(fp) == 0.0f) {
                continue;
            }
            CS_CutInfo->sfx[slot].position.x = NuAToF(fp->word_buf);
            if (NuFParGetFloat(fp) == 0.0f) {
                continue;
            }
            CS_CutInfo->sfx[slot].position.y = NuAToF(fp->word_buf);
            if (NuFParGetFloat(fp) == 0.0f) {
                continue;
            }
            CS_CutInfo->sfx[slot].position.z = NuAToF(fp->word_buf);
            CS_CutInfo->sfx[slot].flags |= 1;
        }
    }
}

static void copyAnims(NUGCUTSCENE_s *, NUGCUTSCENE_s *);
void NewCopyAnims(instNUGCUTSCENE_s *);
i32 instNuGCutSceneSwapBuffers(instNUGCUTSCENE_s *, i32);
NUGCUTSCENE_s *RelocateCutScene(NUGCUTSCENE_s *, VARIPTR *);
extern "C" void instNuGCutSceneDestroy(instNUGCUTSCENE_s *);

static void CS_sfx(NUFPAR *fp) {
    if (NuFParGetWord(fp) != 0) {
        CS_CutInfo->legacy_music_index = GetMusicIndex(fp->word_buf, MusicInfo, -1);
        CS_CutInfo->music_handle = music_man.GetTrackHandle(TRACK_CLASS_CUTSCENE, fp->word_buf);
    }
}

i32 CUTCOUNT = 0;
CutSceneListEntry *CutList = NULL;
void (*CutScene_OverrideConfigFileNameFn)(char *, i32, i32) = NULL;
i32 ACTIVECUTCOUNT = 0;
f32 CutSceneScale = 1.0f;
extern "C" {
    u8 CUTSUBTITLEDEFAULT_R = 0xff;
    u8 CUTSUBTITLEDEFAULT_G = 0xff;
    u8 CUTSUBTITLEDEFAULT_B = 0xff;
    u8 CUTSUBTITLEDEFAULT_A = 0x80;
    u8 CUTSUBTITLEDEFAULT_ALIGN = 0;
    f32 CUTSUBTITLEDEFAULT_X = 0.0f;
    f32 CUTSUBTITLEDEFAULT_Y = -0.5f;
    f32 CUTSUBTITLEDEFAULT_SCALEX = 1.0f;
    f32 CUTSUBTITLEDEFAULT_SCALEY = 1.0f;
    f32 CUTSUBTITLEDEFAULT_FITWIDTH = 1.7f;
}
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

static void CS_no_fog(NUFPAR *) {
    CS_CutInfo->flags |= 4;
}

static void CS_lowend_lowbits(NUFPAR *) {
    CS_CutInfo->flags |= 0x10000;
}

static void CS_is_outro(NUFPAR *) {
    CS_CutInfo->flags |= 0x20000;
}

static void CS_unskippable_in_story(NUFPAR *) {
    CS_CutInfo->flags |= 0x40000;
}

static void CS_skip_use_goto(NUFPAR *) {
    CS_CutInfo->flags |= 0x80000;
}

static void CS_in_game(NUFPAR *) {
    CS_CutInfo->flags &= ~3U;
    CS_CutInfo->flags |= 0x800;
}

static void CS_snap_out(NUFPAR *) {
    CS_CutInfo->flags |= 0x10;
}

static void CS_cam_only(NUFPAR *) {
    CS_CutInfo->flags |= 0x20;
}

static void CS_wipe_out(NUFPAR *) {
    CS_CutInfo->flags |= 0x100;
}

static void CS_new_mode(NUFPAR *) {
    CS_CutInfo->flags |= 0x400;
}

static void CS_replace_players(NUFPAR *) {
    CS_CutInfo->flags |= 0x40;
}

static void CS_looping(NUFPAR *) {
    CS_CutInfo->flags |= 0x200;
}

static void CS_start_cam(NUFPAR *) {
    CS_CutInfo->flags |= 0x2000;
}

static void CS_super_widescreen(NUFPAR *) {
    CS_CutInfo->flags |= 0x4000;
}

static void CS_level_intro(NUFPAR *) {
    CS_CutInfo->flags |= 0x1000;
}

static void CS_hold_audio(NUFPAR *) {
    CS_CutInfo->linked_audio = 1;
}

static void CS_playonce(NUFPAR *) {
    CS_CutInfo->end_flags |= 1;
}

static void CS_nextcutscene_inplayablelevel(NUFPAR *) {
    CS_CutInfo->end_flags |= 2;
}

static void CS_farclip(NUFPAR *fp) {
    CS_CutInfo->camera_far_clip = static_cast<u16>(NuFParGetInt(fp));
}

static void CS_render_group(NUFPAR *fp) {
    CS_CutInfo->debris_render_group = static_cast<i8>(NuFParGetInt(fp));
}

static void CS_reflect_range(NUFPAR *fp) {
    CS_CutInfo->reflection_range = static_cast<u8>(NuFParGetInt(fp));
}

static void CS_blobshadow_fadefar(NUFPAR *fp) {
    CS_CutInfo->blob_shadow_fade_far = static_cast<u8>(NuFParGetInt(fp));
}

static void CS_blobshadow_fadenear(NUFPAR *fp) {
    CS_CutInfo->blob_shadow_fade_near = static_cast<u8>(NuFParGetInt(fp));
}

static void CS_blobshadow_alpha(NUFPAR *fp) {
    i32 alpha = NuFParGetInt(fp);
    alpha = alpha < 0 ? 0 : alpha;
    alpha = alpha > 0xfe ? 0xfe : alpha;
    CS_CutInfo->blob_shadow_alpha = static_cast<u8>(alpha);
}

static void CS_nearclip(NUFPAR *fp) {
    CS_CutInfo->camera_near_clip = NuFParGetFloat(fp);
}

static void CS_burnout_flare(NUFPAR *fp) {
    CS_CutInfo->burnout_flare = NuFParGetFloat(fp);
    CS_CutInfo->flags |= 0x80;
}

static void CS_burnout_intensity(NUFPAR *fp) {
    CS_CutInfo->burnout_intensity = NuFParGetFloat(fp);
    CS_CutInfo->flags |= 0x80;
}

static void CS_burnout_threshold(NUFPAR *fp) {
    CS_CutInfo->burnout_threshold = NuFParGetFloat(fp);
    CS_CutInfo->flags |= 0x80;
}

static void CS_fpsec(NUFPAR *fp) {
    CS_CutInfo->frames_per_second = NuFParGetFloat(fp);
}

static void CS_lowend_disthack(NUFPAR *fp) {
    if (NuFParGetWord(fp) != 0) {
        f32 distance = NuAToF(fp->word_buf);
        if (distance > 0.0f) {
            CS_CutInfo->low_end_distance = distance;
        }
    }
}

static void CS_draw_gizmo_sys(NUFPAR *fp) {
    if (NuFParGetWord(fp) == 0) {
        return;
    }
    if (NuStrICmp(fp->word_buf, "on") == 0) {
        CS_CutInfo->flags |= 0x8000;
    } else if (NuStrICmp(fp->word_buf, "off") == 0) {
        CS_CutInfo->flags &= ~0x8000U;
    }
}

static void CS_deb_page(NUFPAR *fp) {
    if (NuFParGetWord(fp) == 0) {
        return;
    }
    if (NuStrICmp(fp->word_buf, "level") == 0) {
        CS_CutInfo->flags |= 8;
    } else {
        CS_CutInfo->flags &= ~8U;
    }
}

static void CS_draw_world(NUFPAR *fp) {
    if (NuFParGetWord(fp) == 0) {
        return;
    }
    if (NuStrICmp(fp->word_buf, "on") == 0) {
        CS_CutInfo->flags |= 2;
    } else if (NuStrICmp(fp->word_buf, "off") == 0) {
        CS_CutInfo->flags &= ~2U;
    }
}

static void CS_next_cut_scene(NUFPAR *fp) {
    if (NuFParGetWord(fp) != 0 && NuStrLen(fp->word_buf) <= 0x3f) {
        NuStrCpy(CS_CutInfo->next_cutscene, fp->word_buf);
        CS_CutInfo->linked_audio = 1;
    }
}

static void CS_go_through_door(NUFPAR *fp) {
    if (NuFParGetWord(fp) != 0 && NuStrLen(fp->word_buf) <= 0xf) {
        NuStrCpy(CS_CutInfo->door_name, fp->word_buf);
    }
}

static void CS_tex_anim(NUFPAR *fp) {
    if (CS_texanimcount > 3) {
        return;
    }
    // Retail re-reads the cut info and slot for each store.
    if ((CS_CutInfo->texture_animations[CS_texanimcount].index = NuFParGetInt(fp)) == -1) {
        return;
    }
    if ((CS_CutInfo->texture_animations[CS_texanimcount].frame = static_cast<f32>(NuFParGetInt(fp))) >= 0.0f) {
        ++CS_texanimcount;
    }
}

static void CS_fadefog(NUFPAR *fp) {
    if (CS_fadefogcount > 1) {
        return;
    }
    f32 value;
    CS_CutInfo->fade_fog[CS_fadefogcount].frame = value = NuFParGetFloat(fp);
    if (value >= 0.0f) {
        CS_CutInfo->fade_fog[CS_fadefogcount].near_distance = value = NuFParGetFloat(fp);
        if (value >= 0.0f) {
            CS_CutInfo->fade_fog[CS_fadefogcount].far_distance = value = NuFParGetFloat(fp);
            if (value >= 0.0f) {
                CS_CutInfo->fade_fog[CS_fadefogcount].value = value = NuFParGetFloat(fp);
                if (value >= 0.0f) {
                    ++CS_fadefogcount;
                }
            }
        }
    }
}

static void CS_cutsceneplayerobj(NUFPAR *fp) {
    if (CS_CutInfo->state_count > 0x1f || NuFParGetWord(fp) == 0) {
        return;
    }

    if (NuSpecialFind(CS_worldinfo->current_gscn, &CS_CutInfo->state_entries[CS_CutInfo->state_count].special,
                      fp->word_buf, 1) == 0) {
        return;
    }
    // Every flag update re-indexes the entry: the u8 store may alias state_count.
#define CS_STATE_FLAGS CS_CutInfo->state_entries[CS_CutInfo->state_count].flags
    CS_STATE_FLAGS &= ~CLIP_OBJECT_SHOW;
    CS_STATE_FLAGS &= ~CLIP_OBJECT_HIDE;
    while (NuFParGetWord(fp) != 0) {
        if (NuStrICmp(fp->word_buf, "on") == 0) {
            CS_STATE_FLAGS |= CLIP_OBJECT_SHOW;
            CS_STATE_FLAGS &= ~CLIP_OBJECT_HIDE;
        } else if (NuStrICmp(fp->word_buf, "off") == 0) {
            CS_STATE_FLAGS &= ~CLIP_OBJECT_SHOW;
            CS_STATE_FLAGS |= CLIP_OBJECT_HIDE;
        } else if (NuStrICmp(fp->word_buf, "end_anim") == 0 || NuStrICmp(fp->word_buf, "endanim") == 0 ||
                   NuStrICmp(fp->word_buf, "anim_end") == 0 || NuStrICmp(fp->word_buf, "animend") == 0) {
            CS_STATE_FLAGS &= ~CLIP_OBJECT_SHOW;
            CS_STATE_FLAGS &= ~CLIP_OBJECT_HIDE;
            CS_STATE_FLAGS |= CLIP_OBJECT_ANIM_END;
        }
    }
#undef CS_STATE_FLAGS
    ++CS_CutInfo->state_count;
}

static void CS_goto_level(NUFPAR *fp) {
    if (NuFParGetWord(fp) == 0) {
        return;
    }
    // Retail realigns the frame (and esp,-16) and keeps both out-parameters in 16-byte aligned slots.
    i32 level_index __attribute__((aligned(16)));
    Level_FindByName(fp->word_buf, &level_index);
    if (level_index != -1 && &LDataList[level_index] == HUB_LDATA && CS_area != -1) {
        i32 status_index __attribute__((aligned(16)));
        Area_FindStatusLevel(&ADataList[CS_area], &status_index);
        if (status_index != -1) {
            level_index = status_index;
        }
    }
    CS_CutInfo->skip_level = static_cast<i16>(level_index);
}

static inline void CS_subtitle(NUFPAR *fp) {
    if (CS_CutInfo->subtitle_data == NULL) {
        CS_CutInfo->subtitle_data = reinterpret_cast<CUTSCENESUBTITLE *>(CS_buffptr->addr);
    }

    CUTSCENESUBTITLE *subtitle = &CS_CutInfo->subtitle_data[CS_CutInfo->subtitle_count];
    subtitle->text_id = -1;
    subtitle->start_frame = 0.0f;
    subtitle->end_frame = 0.0f;
    subtitle->red = CUTSUBTITLEDEFAULT_R;
    subtitle->green = CUTSUBTITLEDEFAULT_G;
    subtitle->blue = CUTSUBTITLEDEFAULT_B;
    subtitle->alpha = CUTSUBTITLEDEFAULT_A;
    subtitle->alignment = CUTSUBTITLEDEFAULT_ALIGN;
    subtitle->x = CUTSUBTITLEDEFAULT_X;
    subtitle->y = CUTSUBTITLEDEFAULT_Y;
    subtitle->x_scale = CUTSUBTITLEDEFAULT_SCALEX;
    subtitle->y_scale = CUTSUBTITLEDEFAULT_SCALEY;
    subtitle->max_width = CUTSUBTITLEDEFAULT_FITWIDTH;
    subtitle->fade_time = 0.0f;

    while (NuFParGetWord(fp) != 0) {
        if (NuStrICmp(fp->word_buf, "text_id") == 0) {
            subtitle->text_id = static_cast<i16>(NuFParGetInt(fp));
        } else if (NuStrICmp(fp->word_buf, "colour") == 0) {
            subtitle->red = static_cast<u8>(NuFParGetFloat(fp) * 255.0f);
            subtitle->green = static_cast<u8>(NuFParGetFloat(fp) * 255.0f);
            subtitle->blue = static_cast<u8>(NuFParGetFloat(fp) * 255.0f);
            subtitle->alpha = static_cast<u8>(NuFParGetFloat(fp) * 128.0f);
        } else if (NuStrICmp(fp->word_buf, "r") == 0) {
            subtitle->red = static_cast<u8>(NuFParGetFloat(fp) * 255.0f);
        } else if (NuStrICmp(fp->word_buf, "g") == 0) {
            subtitle->green = static_cast<u8>(NuFParGetFloat(fp) * 255.0f);
        } else if (NuStrICmp(fp->word_buf, "b") == 0) {
            subtitle->blue = static_cast<u8>(NuFParGetFloat(fp) * 255.0f);
        } else if (NuStrICmp(fp->word_buf, "a") == 0) {
            subtitle->alpha = static_cast<u8>(NuFParGetFloat(fp) * 128.0f);
        } else if (NuStrICmp(fp->word_buf, "align") == 0) {
            if (NuStrICmp(fp->word_buf, "centre") == 0) {
                subtitle->alignment = 0;
            } else if (NuStrICmp(fp->word_buf, "top") == 0) {
                subtitle->alignment = 1;
            } else if (NuStrICmp(fp->word_buf, "top_right") == 0) {
                subtitle->alignment = 9;
            } else if (NuStrICmp(fp->word_buf, "right") == 0) {
                subtitle->alignment = 8;
            } else if (NuStrICmp(fp->word_buf, "bottom_right") == 0) {
                subtitle->alignment = 12;
            } else if (NuStrICmp(fp->word_buf, "bottom") == 0) {
                subtitle->alignment = 4;
            } else if (NuStrICmp(fp->word_buf, "bottom_left") == 0) {
                subtitle->alignment = 6;
            } else if (NuStrICmp(fp->word_buf, "left") == 0) {
                subtitle->alignment = 2;
            } else if (NuStrICmp(fp->word_buf, "top_left") == 0) {
                subtitle->alignment = 3;
            }
        } else if (NuStrICmp(fp->word_buf, "start_time") == 0) {
            subtitle->start_frame = NuFabs(NuFParGetFloat(fp));
        } else if (NuStrICmp(fp->word_buf, "end_time") == 0) {
            subtitle->end_frame = NuFabs(NuFParGetFloat(fp));
        } else if (NuStrICmp(fp->word_buf, "pos") == 0) {
            subtitle->x = NuFParGetFloat(fp);
            subtitle->y = NuFParGetFloat(fp);
        } else if (NuStrICmp(fp->word_buf, "x") == 0) {
            subtitle->x = NuFParGetFloat(fp);
        } else if (NuStrICmp(fp->word_buf, "y") == 0) {
            subtitle->y = NuFParGetFloat(fp);
        }

        if (NuStrICmp(fp->word_buf, "scale") == 0) {
            subtitle->x_scale = NuFParGetFloat(fp);
            subtitle->y_scale = subtitle->x_scale;
        } else if (NuStrICmp(fp->word_buf, "x_scale") == 0) {
            subtitle->x_scale = NuFParGetFloat(fp);
        } else if (NuStrICmp(fp->word_buf, "y_scale") == 0) {
            subtitle->y_scale = NuFParGetFloat(fp);
        } else if (NuStrICmp(fp->word_buf, "fit_width") == 0) {
            subtitle->max_width = NuFabs(NuFParGetFloat(fp));
        } else if (NuStrICmp(fp->word_buf, "fade_time") == 0) {
            subtitle->fade_time = NuFabs(NuFParGetFloat(fp));
        }
    }

    i16 text_id = subtitle->text_id;
    if (text_id >= 0 && text_id < Text_GetMaxOverallStrings() && TTab[text_id] != NULL &&
        subtitle->start_frame < subtitle->end_frame) {
        if (subtitle->fade_time > 0.0f && subtitle->end_frame - subtitle->start_frame < subtitle->fade_time * 2.0f) {
            subtitle->fade_time = 0.0f;
        }
        CS_buffptr->addr += sizeof(CUTSCENESUBTITLE);
        ++CS_CutInfo->subtitle_count;
    }
}

static NUFPCOMJMP CutScene_ConfigKeywords[] = {
    {const_cast<char *>("fpsec"), CS_fpsec},
    {const_cast<char *>("no_fog"), CS_no_fog},
    {const_cast<char *>("draw_world"), CS_draw_world},
    {const_cast<char *>("lowend_lowbits"), CS_lowend_lowbits},
    {const_cast<char *>("lowend_disthack"), CS_lowend_disthack},
    {const_cast<char *>("is_outro"), CS_is_outro},
    {const_cast<char *>("unskippable_in_story"), CS_unskippable_in_story},
    {const_cast<char *>("skip_use_goto"), CS_skip_use_goto},
    {const_cast<char *>("in_game"), CS_in_game},
    {const_cast<char *>("blobshadow_alpha"), CS_blobshadow_alpha},
    {const_cast<char *>("blobshadow_fadenear"), CS_blobshadow_fadenear},
    {const_cast<char *>("blobshadow_fadefar"), CS_blobshadow_fadefar},
    {const_cast<char *>("reflect_range"), CS_reflect_range},
    {const_cast<char *>("render_group"), CS_render_group},
    {const_cast<char *>("deb_page"), CS_deb_page},
    {const_cast<char *>("snap_out"), CS_snap_out},
    {const_cast<char *>("wipe_out"), CS_wipe_out},
    {const_cast<char *>("new_mode"), CS_new_mode},
    {const_cast<char *>("cam_only"), CS_cam_only},
    {const_cast<char *>("replace_players"), CS_replace_players},
    {const_cast<char *>("burnout_threshold"), CS_burnout_threshold},
    {const_cast<char *>("burnout_intensity"), CS_burnout_intensity},
    {const_cast<char *>("burnout_flare"), CS_burnout_flare},
    {const_cast<char *>("looping"), CS_looping},
    {const_cast<char *>("start_cam"), CS_start_cam},
    {const_cast<char *>("super_widescreen"), CS_super_widescreen},
    {const_cast<char *>("go_through_door"), CS_go_through_door},
    {const_cast<char *>("sfx"), CS_sfx},
    {const_cast<char *>("play_sfx"), CS_play_sfx},
    {const_cast<char *>("goto_level"), CS_goto_level},
    {const_cast<char *>("next_cut_scene"), CS_next_cut_scene},
    {const_cast<char *>("holdaudio"), CS_hold_audio},
    {const_cast<char *>("hold_audio"), CS_hold_audio},
    {const_cast<char *>("level_intro"), CS_level_intro},
    {const_cast<char *>("tex_anim"), CS_tex_anim},
    {const_cast<char *>("fadefog"), CS_fadefog},
    {const_cast<char *>("subtitle"), CS_subtitle},
    {const_cast<char *>("play_once"), CS_playonce},
    {const_cast<char *>("nextcutscene_inplayablelevel"), CS_nextcutscene_inplayablelevel},
    {const_cast<char *>("cutsceneplayerobj"), CS_cutsceneplayerobj},
    {const_cast<char *>("cutsceneplayer_obj"), CS_cutsceneplayerobj},
    {const_cast<char *>("nearclip_360"), CS_nearclip},
    {const_cast<char *>("farclip_360"), CS_farclip},
    {const_cast<char *>("nearclip_pc"), CS_nearclip},
    {const_cast<char *>("farclip_pc"), CS_farclip},
    {const_cast<char *>("nearclip"), CS_nearclip},
    {const_cast<char *>("farclip"), CS_farclip},
    {const_cast<char *>("drawgizmosys"), CS_draw_gizmo_sys},
    {NULL, NULL},
};

static void CutScene_Configure(CUTINFO *cut, char *name, VARIPTR *buf, VARIPTR *buf_end) {
    CUTSCENEPLAYEROBJ state_entries[32];

    CS_CutInfo = cut;
    CS_buffptr = buf;
    CS_buffend = buf_end;
    cut->state_count = 0;
    cut->flags = 3;
    cut->frames_per_second = 30.0f;
    cut->burnout_threshold = 1.0f;
    cut->burnout_intensity = 0.0f;
    cut->burnout_flare = 0.0f;
    cut->camera_near_clip = 0.0f;
    cut->camera_far_clip = 0;
    cut->legacy_music_index = -1;
    cut->skip_level = -1;
    cut->debris_render_group = 2;
    cut->linked_audio = 0;
    cut->door_name[0] = '\0';
    cut->next_cutscene[0] = '\0';
    cut->music_handle = -1;

    for (i32 i = 0; i < 6; ++i) {
        cut->sfx[i].id = -1;
    }
    cut->blob_shadow_alpha = 0xff;
    cut->blob_shadow_fade_near = 0xff;
    cut->blob_shadow_fade_far = 0xff;
    cut->reflection_range = 0xff;
    for (i32 i = 0; i < 4; ++i) {
        cut->texture_animations[i].index = -1;
    }
    for (i32 i = 0; i < 2; ++i) {
        cut->fade_fog[i].frame = 0.0f;
        cut->fade_fog[i].near_distance = 0.0f;
        cut->fade_fog[i].far_distance = 0.0f;
        cut->fade_fog[i].value = 0.0f;
    }
    cut->subtitle_count = 0;
    cut->subtitle_data = NULL;
    cut->end_flags &= ~3U;
    cut->pad_18b = 0;
    cut->low_end_distance = 0.0f;
    cut->state_entries = state_entries;

    NUFPAR *fp = NuFParCreate(name);
    if (fp == NULL) {
        return;
    }
    CS_texanimcount = 0;
    CS_fadefogcount = 0;
    NuFParPushCom(fp, CutScene_ConfigKeywords);
    while (NuFParGetLine(fp) != 0) {
        if (NuFParGetWord(fp) == 0) {
            continue;
        }
        NuFParInterpretWord(fp);
    }
    NuFParDestroy(fp);

    CUTINFO *info = CS_CutInfo;
    if (info->blob_shadow_fade_near > info->blob_shadow_fade_far) {
        info->blob_shadow_fade_near = info->blob_shadow_fade_far;
    }
    if (info->subtitle_data != NULL && info->subtitle_count == 0) {
        info->subtitle_data = NULL;
    }
    if (info->state_count == 0) {
        info->state_entries = NULL;
    } else {
        buf->void_ptr = reinterpret_cast<void *>(ALIGN(buf->addr, 4));
        info->state_entries = reinterpret_cast<CUTSCENEPLAYEROBJ *>(buf->void_ptr);
        const i32 size = info->state_count * sizeof(CUTSCENEPLAYEROBJ);
        memmove(info->state_entries, state_entries, size);
        buf->void_ptr = reinterpret_cast<char *>(buf->void_ptr) + size;
    }
}

static CUTSYS *CutScenes_LoadFromList(NUGSCN *gscn1, NUGSCN *gscn2, i32 param1, VARIPTR *buf, VARIPTR *buf_end,
                                      i32 param2, i32 param3, WORLDINFO *world) {
    CUTSYS *sys;
    void *initial;
    CUTINFO *cut;
    CUTINFO *entries[32];
    char full_path[128];
    char config_name[128];

    CS_area = param2;
    initial = buf->void_ptr;
    sys = (CUTSYS *)ALIGN((usize)initial, 4);
    buf->void_ptr = sys + 1;
    CS_cutsys = sys;
    CS_worldinfo = world;
    sys->character_bits = reinterpret_cast<u32 *>(buf->void_ptr);
    buf->void_ptr = reinterpret_cast<char *>(buf->void_ptr) + ((CHARCOUNT + 0x1f) / 32) * 4;
    for (i32 i = 0; i < CUTCOUNT; ++i) {
        if (CutList[i].level != param3) {
            continue;
        }
        void *saved = buf->void_ptr;
        entries[sys->count] = reinterpret_cast<CUTINFO *>(saved);
        buf->addr = ALIGN(buf->addr + sizeof(*cut), 4);
        cut = entries[sys->count];
        NuStrCpy(full_path, "cut\\");
        NuStrCat(full_path, CutList[i].directory);
        NuStrCat(full_path, "\\");
        NuStrCat(full_path, CutList[i].filename);
        NuStrCpy(config_name, full_path);
        NuStrCat(config_name, ".txt");
        CutScene_Configure(cut, config_name, buf, buf_end);
        if ((cut->flags & 0x800) != 0 || InStory() != 0 ||
            (LDataList[param3].flags & (LEVEL_INTRO | LEVEL_MIDTRO | LEVEL_OUTRO)) != 0) {
            NuStrCpy(config_name, full_path);
            NuStrCat(config_name, ".cu2");
            cut->scene = NuGCutSceneLoad(config_name, buf, buf_end, 0);
            if (cut->scene != NULL) {
                NuStrCpy(cut->name, CutList[i].filename);
                NuGCutSceneFixUp(reinterpret_cast<NUGCUTSCENE_s *>(cut->scene), gscn1, 0, param1);
                NuGCutSceneFixUpExtra(reinterpret_cast<NUGCUTSCENE_s *>(cut->scene), gscn2);
                cut->instance = instNuGCutSceneCreate(reinterpret_cast<NUGCUTSCENE_s *>(cut->scene), NULL, NULL,
                                                      CutList[i].filename, buf, 0);
                if (cut->instance != NULL) {
                    reinterpret_cast<instNUGCUTSCENE_s *>(cut->instance)->rate =
                        cut->frames_per_second * DEFAULTFRAMETIME;
                    buf->addr = ALIGN(buf->addr, 0x10);
                }
                ++sys->count;
                continue;
            }
        }
        buf->void_ptr = saved;
    }
    if (sys->count <= 0) {
        buf->void_ptr = initial;
        return NULL;
    }
    sys->cuts = reinterpret_cast<CUTINFO **>(buf->void_ptr);
    const usize table_size = sys->count * sizeof(CUTINFO *);
    memmove(sys->cuts, entries, table_size);
    buf->addr = ALIGN(buf->addr + table_size * sys->count, 4);
    return sys;
}

void *CutScenes_Load(char *config, NUGSCN *gscn1, NUGSCN *gscn2, i32 param1, VARIPTR *buf, VARIPTR *buf_end, i32 param2,
                     i32 param3, WORLDINFO *world) {
    NUFPAR *fp;
    CUTSYS *sys;
    void *initial;
    CUTINFO *cut;
    // Retail realigns the frame (and esp,-16) and keeps the name buffer in a 16-byte aligned slot.
    char name[128] __attribute__((aligned(16)));
    CUTINFO *entries[32];
    char full_path[128];
    char config_name[128];

    if (CutList != NULL && CUTCOUNT > 0) {
        return CutScenes_LoadFromList(gscn1, gscn2, param1, buf, buf_end, param2, param3, world);
    }
    {
        if (config == NULL) {
            return NULL;
        }

        fp = NuFParCreateMem((char *)"cutscenes", config, 0xffff);
        if (fp == NULL) {
            return NULL;
        }

        CS_area = param2;
        initial = buf->void_ptr;
        sys = (CUTSYS *)ALIGN((usize)initial, 4);
        buf->void_ptr = sys + 1;
        CS_cutsys = sys;
        CS_worldinfo = world;
        sys->character_bits = reinterpret_cast<u32 *>(buf->void_ptr);
        buf->void_ptr = reinterpret_cast<char *>(buf->void_ptr) + ((CHARCOUNT + 0x1f) / 32) * 4;

        while (NuFParGetLine(fp) != 0) {
            if (NuFParGetWord(fp) == 0 || NuStrICmp(fp->word_buf, "cutscene") != 0 || sys->count > 0x1f ||
                NuFParGetWord(fp) == 0) {
                continue;
            }

            cut = (CUTINFO *)buf->void_ptr;
            entries[sys->count] = cut;
            buf->addr = ALIGN(buf->addr + sizeof(*cut), 4);
            NuStrLwr(name, fp->word_buf);
            i32 dot = NuStrLen(name) - 1;
            while (dot > 0 && name[dot] != '.') {
                --dot;
            }
            if (name[dot] == '.') {
                name[dot] = '\0';
            }

            if (name[0] == 'c' && name[1] == 'u' && name[2] == 't' && name[3] == '\\') {
                NuStrCpy(name, name + 4);
            }
            NuStrCpy(config_name, name);
            if (CutScene_OverrideConfigFileNameFn != NULL) {
                CutScene_OverrideConfigFileNameFn(config_name, param2, param3);
            }
            NuStrCpy(full_path, "cut\\");
            NuStrCat(full_path, config_name);
            NuStrCat(full_path, ".txt");
            CutScene_Configure(cut, full_path, buf, buf_end);
            NuStrCpy(full_path, "cut\\");
            NuStrCat(full_path, name);
            NuStrCat(full_path, ".txt");

            if ((cut->flags & 0x800) == 0 && InStory() == 0 &&
                (LDataList[param3].flags & (LEVEL_INTRO | LEVEL_MIDTRO | LEVEL_OUTRO)) == 0) {
                buf->void_ptr = cut;
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
                    buf->void_ptr = cut;
                    continue;
                }
            }
            i32 separator = NuStrLen(name) - 1;
            while (separator >= 0 && name[separator] != '\\') {
                --separator;
            }
            NuStrCpy(cut->name, name + separator + 1);
            NuGCutSceneFixUp(reinterpret_cast<NUGCUTSCENE_s *>(cut->scene), gscn1, 0, param1);
            NuGCutSceneFixUpExtra(reinterpret_cast<NUGCUTSCENE_s *>(cut->scene), gscn2);
            cut->instance =
                instNuGCutSceneCreate(reinterpret_cast<NUGCUTSCENE_s *>(cut->scene), NULL, NULL, name, buf, 0);
            if (cut->instance != NULL) {
                reinterpret_cast<instNUGCUTSCENE_s *>(cut->instance)->rate = cut->frames_per_second * DEFAULTFRAMETIME;
                buf->void_ptr = (char *)ALIGN(buf->addr, 0x10);
            }
            sys->count++;
        }
        NuFParDestroy(fp);
        if (sys->count <= 0) {
            buf->void_ptr = initial;
            return NULL;
        }
        sys->cuts = reinterpret_cast<CUTINFO **>(buf->void_ptr);
        const usize table_size = sys->count * sizeof(CUTINFO *);
        memmove(sys->cuts, entries, table_size);
        // Retail reserves count pointer-table rows after copying the count entries.
        buf->addr = ALIGN(buf->addr + table_size * sys->count, 4);
        return sys;
    }
}

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

// Name table passed as NuGCutSceneFixUp's third argument: count at +0x08 and 0x34-byte entries at +0x0c
// whose first field is the trigger target name (offsets from the original's trigger fix-up loop).
struct CutSceneTriggerNameEntry {
    char *name;
    u8 unknown_04[0x30];
};
struct CutSceneTriggerNameTable {
    u32 unknown_00[2];
    i32 count;
    CutSceneTriggerNameEntry *entries;
};
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

    instNUGCUTSCENE_s *instNuGCutSceneFind(const char *name) {
        for (instNUGCUTSCENE_s *instance = active_cutscene_instances; instance != NULL; instance = instance->next) {
            if (NuStrICmp(name, instance->name) == 0) {
                return instance;
            }
        }
        return NULL;
    }

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
static void NuGCutSceneFixPtrsAnims(NUGCUTSCENE_s *cutscene, isize anim_delta) {
    NuGCutSceneFixPtrs_Title(cutscene, anim_delta);
}
static NUGCUT_FIXPTRS_ATTR(1) void NuGCutSceneFixPtrsNoAnims(NUGCUTSCENE_s *cutscene)
    __asm__("_ZL18NuGCutSceneFixPtrsP13NUGCUTSCENE_sii.constprop.43");
static void NuGCutSceneFixPtrsNoAnims(NUGCUTSCENE_s *cutscene) {
    NuGCutSceneFixPtrs_Title(cutscene, 0);
}

extern "C" void NuGCutSceneSysInit(NUGCUTLOCATORFNENTRY_s *locator_functions) {
    background_cutscene_instances = NULL;
    active_cutscene_instances = NULL;
    locatorfns = locator_functions;
}

void NuGCutSceneSysInitVfx(NUGCUTLOOKUPLOCATORVFXFN lookup, NUGCUTTRIGGERLOCATORVFXFN trigger,
                           NUGCUTRELEASELOCATORVFXFN release, NUGCUTUPDATELOCATORVFXFN update) {
    LookupLocatorVfxFn = lookup;
    TriggerLocatorVfxFn = trigger;
    ReleaseLocatorVfxFn = release;
    UpdateLocatorVfxFn = update;
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

    i32 NuGCutDebFixUp_SearchAllPages = 0;
    NUGCUTLOCATORFNENTRY_s *locatorfns = NULL;
    NUGCUTUPDATELOCATORVFXFN UpdateLocatorVfxFn = NULL;
    NUGCUTRELEASELOCATORVFXFN ReleaseLocatorVfxFn = NULL;
    NUGCUTTRIGGERLOCATORVFXFN TriggerLocatorVfxFn = NULL;
    NUGCUTLOOKUPLOCATORVFXFN LookupLocatorVfxFn = NULL;
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

    void instNuGCutScenePause(instNUGCUTSCENE_s *instance, u8 paused) {
        instance->paused = paused;
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

    void NuGCutSceneSysBackgroundFlush(void) {
        background_cutscene_instances = NULL;
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

// Retail only keeps the IPA-SRA clone, which drops the unused frame argument.
#if defined(__i386__)
#define INST_LOCATOR_SYS_END_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define INST_LOCATOR_SYS_END_ATTR __attribute__((noinline, noclone))
#endif
static INST_LOCATOR_SYS_END_ATTR void instNuGCutLocatorSysEnd(instNUGCUTLOCATORSYS_s *instance,
                                                              NUGCUTLOCATORSYS_s *system)
    __asm__("_ZL23instNuGCutLocatorSysEndP22instNUGCUTLOCATORSYS_sP18NUGCUTLOCATORSYS_sf.isra.8");
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

static void instNuGCutSceneClipTest(instNUGCUTSCENE_s *instance) {
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

static __used__ void instNuGCutTriggerSysUpdate(instNUGCUTSCENE_s *instance, float frame) {
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

static __used__ void CutScene_OverrideConfigFileName_LSW(char *filename, int, int) {
    if (PODSPRINT_ADATA == NULL || WorldInfo_CurrentlyActive()->area != PODSPRINT_ADATA) {
        return;
    }
    if (NuStrIStr(filename, const_cast<char *>("episodei\\ep1_podrace_")) != filename) {
        return;
    }
    char *suffix = filename + NuStrLen(const_cast<char *>("episodei\\ep1_podrace_"));
    if (NuStrICmp(suffix, "arrival1") == 0 || NuStrICmp(suffix, "arrival2") == 0 ||
        NuStrICmp(suffix, "arrival3") == 0 || NuStrICmp(suffix, "arrival4") == 0 || NuStrICmp(suffix, "intro") == 0 ||
        NuStrICmp(suffix, "tuskenraiders") == 0 || NuStrICmp(suffix, "outro1") == 0 ||
        NuStrICmp(suffix, "outro2") == 0) {
        NuStrCat(filename, "_sprint");
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
