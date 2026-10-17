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
extern NUMTX cutscenecammtx;
extern i32 cutscenecamchange;
extern f32 cutscenecam_fstop;
extern u8 cutscenecam_usefocusloc;
extern NUVEC cutscenecam_focusloc;
extern f32 cutscenecam_focusDistance;
extern f32 cutscenecam_focalLength;
extern i32 CameraDOFHack;
extern u8 set_cutscenecammtx;

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

#include "legoapi/cutscenes/cutscene_types.h"
DECOMP_ASSERT(sizeof(CutSceneTriggerNameEntry) == 0x34, "cutscene trigger name entry stride");
DECOMP_ASSERT(offsetof(CutSceneTriggerNameTable, entries) == 0x0c, "cutscene trigger name table entries offset");

// Exported (default visibility) in the original library.
extern i32 cutscene_quit_prompt;
extern i32 cutscene_has_quit;
extern i32 cutscene_lock_player;
extern i32 termcutstream_hack;

extern "C" {
extern instNUGCUTSCENE_s * cutscene_load_instance;
extern i32 NumCommonStreamingBuffers;

instNUGCUTSCENE_s *instNuGCutSceneFind(const char *name);

void *instNuGCutSceneCleanUp(void);
}

// The relocation tables are patched as plain words, so every fixup aliases
// cutscene->string_delta and re-reads it.
typedef isize __attribute__((may_alias)) NuGCutWord;
#define NUGCUT_WORD(field) (*reinterpret_cast<NuGCutWord *>(&(field)))
#define NUGCUT_RELOCATE(field, delta) \
    (NUGCUT_WORD(field) = NUGCUT_WORD(field) != 0 ? NUGCUT_WORD(field) + (delta) : 0)


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

extern i32 NuGCutDebFixUp_SearchAllPages;
extern NUGCUTLOCATORFNENTRY_s * locatorfns;
extern NUGCUTUPDATELOCATORVFXFN UpdateLocatorVfxFn;
extern NUGCUTRELEASELOCATORVFXFN ReleaseLocatorVfxFn;
extern NUGCUTTRIGGERLOCATORVFXFN TriggerLocatorVfxFn;
extern NUGCUTLOOKUPLOCATORVFXFN LookupLocatorVfxFn;
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



static void instNuGCutSceneUpdate(instNUGCUTSCENE_s *instance, int paused, int skip, float elapsed);

static void instNuGCutCamSysUpdate(instNUGCUTSCENE_s *instance, float frame);

static void instNuGCutSceneClipTest(instNUGCUTSCENE_s *instance);

static void instNuGCutRigidSysRender(instNUGCUTSCENE_s *instance, float frame, int paused);

static void instNuGCutRigidSysUpdate(instNUGCUTSCENE_s *instance, float frame, int paused);

static __used__ void instNuGCutTriggerSysUpdate(instNUGCUTSCENE_s *instance, float frame);

extern "C" void NuGCutSceneSysUpdate(i32 paused, i32 skip, f32 elapsed);

extern "C" void NuGCutSceneSysRender(i32 paused);

static __used__ void CutScene_OverrideConfigFileName_LSW(char *filename, int, int);

void NewCopyAnims(instNUGCUTSCENE_s *instance);

static void copyAnims(NUGCUTSCENE_s *destination, NUGCUTSCENE_s *source);
