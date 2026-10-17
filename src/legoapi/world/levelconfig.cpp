#include "decomp.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/world/level.h"
#include "legoapi/audio/audio.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/numusic/numusic.h"
#include "globals.h"

static __used__ void LC_AL_backb(nufpar_s *fp) {
    levelconfig_ldata->data_display.bg_blue_top =
        levelconfig_ldata->data_display.bg_blue_bottom = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_backb_bottom(nufpar_s *fp) {
    levelconfig_ldata->data_display.bg_blue_bottom = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_backb_top(nufpar_s *fp) {
    levelconfig_ldata->data_display.bg_blue_top = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_backg(nufpar_s *fp) {
    levelconfig_ldata->data_display.bg_green_top =
        levelconfig_ldata->data_display.bg_green_bottom = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_backg_bottom(nufpar_s *fp) {
    levelconfig_ldata->data_display.bg_green_bottom = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_backg_top(nufpar_s *fp) {
    levelconfig_ldata->data_display.bg_green_top = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_backr(nufpar_s *fp) {
    levelconfig_ldata->data_display.bg_red_top =
        levelconfig_ldata->data_display.bg_red_bottom = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_backr_bottom(nufpar_s *fp) {
    levelconfig_ldata->data_display.bg_red_bottom = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_backr_top(nufpar_s *fp) {
    levelconfig_ldata->data_display.bg_red_top = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_blobshadow_alpha(nufpar_s *fp) {
    levelconfig_ldata->blob_shadow_alpha = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_blobshadow_fadefar(nufpar_s *fp) {
    levelconfig_ldata->blob_shadow_fade_far = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_blobshadow_fadenear(nufpar_s *fp) {
    levelconfig_ldata->blob_shadow_fade_near = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_cam_tilt(nufpar_s *fp) {
    levelconfig_ldata->cam_tilt = NuFParGetFloat(fp);
}

static __used__ void LC_AL_camang_seek(nufpar_s *fp) {
    levelconfig_ldata->cam_angle_seek = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_campos_seek(nufpar_s *fp) {
    levelconfig_ldata->cam_pos_seek = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_conveyor(nufpar_s *fp) {
    while (NuFParGetWord(fp) != 0) {
        if (NuStrICmp(fp->word_buf, "xspeed") == 0) {
            levelconfig_ldata->conveyor_x_speed = NuFParGetFloat(fp);
        } else if (NuStrICmp(fp->word_buf, "zspeed") == 0) {
            levelconfig_ldata->conveyor_z_speed = NuFParGetFloat(fp);
        }
    }
}

static __used__ void LC_AL_hover_height(nufpar_s *fp) {
    levelconfig_ldata->hover_height = NuFParGetFloat(fp);
}

static __used__ void LC_AL_plat_scan_dist(nufpar_s *fp) {
    levelconfig_ldata->unknown_0db = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_raycaststep(nufpar_s *fp) {
    levelconfig_ldata->unknown_0da = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_reflect_range(nufpar_s *fp) {
    levelconfig_ldata->camera_judder_distance = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_reflect_y(nufpar_s *fp) {
    levelconfig_ldata->unknown_0cc = NuFParGetFloat(fp);
}

static __used__ void LC_AL_sfx_ambient(nufpar_s *fp) {
    if (NuFParGetWord(fp) != 0) {
        levelconfig_ldata->unknown_0a2 = (i16)GetSfxId(fp->word_buf);
    }
}

static __used__ void LC_AL_waterripple_endcol_a(nufpar_s *fp) {
    levelconfig_ldata->unknown_0bf = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_waterripple_endcol_b(nufpar_s *fp) {
    levelconfig_ldata->unknown_0be = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_waterripple_endcol_g(nufpar_s *fp) {
    levelconfig_ldata->unknown_0bd = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_waterripple_endcol_r(nufpar_s *fp) {
    levelconfig_ldata->unknown_0bc = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_waterripple_life(nufpar_s *fp) {
    levelconfig_ldata->unknown_0c0 = NuFParGetFloat(fp);
}

static __used__ void LC_AL_waterripple_startcol_a(nufpar_s *fp) {
    levelconfig_ldata->unknown_0bb = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_waterripple_startcol_b(nufpar_s *fp) {
    levelconfig_ldata->unknown_0ba = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_waterripple_startcol_g(nufpar_s *fp) {
    levelconfig_ldata->unknown_0b9 = (u8)NuFParGetInt(fp);
}

static __used__ void LC_AL_waterripple_startcol_r(nufpar_s *fp) {
    levelconfig_ldata->unknown_0b8 = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_attractos(nufpar_s *fp);

static __used__ void LC_BL_max_bombgen_objects(nufpar_s *fp);

static __used__ void LC_BL_max_bombgens(nufpar_s *fp);

static __used__ void LC_BL_max_bridges(nufpar_s *fp);

static __used__ void LC_BL_max_buildit_objects(nufpar_s *fp);

static __used__ void LC_BL_max_buildits(nufpar_s *fp);

static __used__ void LC_BL_max_climb_objects(nufpar_s *fp);

static __used__ void LC_BL_max_doors(nufpar_s *fp);

static __used__ void LC_BL_max_force(nufpar_s *fp);

static __used__ void LC_BL_max_force_objects(nufpar_s *fp);

static __used__ void LC_BL_max_gameantinodes(nufpar_s *fp);

static __used__ void LC_BL_max_gizmoblowups(nufpar_s *fp);

static __used__ void LC_BL_max_gizmoblowuptypes(nufpar_s *fp);

static __used__ void LC_BL_max_gizpanels(nufpar_s *fp);

static __used__ void LC_BL_max_gizrandoms(nufpar_s *fp);

static __used__ void LC_BL_max_gizspecials(nufpar_s *fp);

static __used__ void LC_BL_max_grapples(nufpar_s *fp);

static __used__ void LC_BL_max_guidelines(nufpar_s *fp);

static __used__ void LC_BL_max_hatmachines(nufpar_s *fp);

static __used__ void LC_BL_max_ledges(nufpar_s *fp);

static __used__ void LC_BL_max_levers(nufpar_s *fp);

static __used__ void LC_BL_max_minicutParts(nufpar_s *fp);

static __used__ void LC_BL_max_minicuts(nufpar_s *fp);

static __used__ void LC_BL_max_obstacle_objects(nufpar_s *fp);

static __used__ void LC_BL_max_obstacles(nufpar_s *fp);

static __used__ void LC_BL_max_pickups(nufpar_s *fp);

static __used__ void LC_BL_max_plugs(nufpar_s *fp);

static __used__ void LC_BL_max_pushblock_endpos(nufpar_s *fp);

static __used__ void LC_BL_max_pushblocks(nufpar_s *fp);

static __used__ void LC_BL_max_securitydoors(nufpar_s *fp);

static __used__ void LC_BL_max_shards(nufpar_s *fp);

static __used__ void LC_BL_max_signals(nufpar_s *fp);

static __used__ void LC_BL_max_spinneranim_objs(nufpar_s *fp);

static __used__ void LC_BL_max_spinners(nufpar_s *fp);

static __used__ void LC_BL_max_technos(nufpar_s *fp);

static __used__ void LC_BL_max_teleports(nufpar_s *fp);

static __used__ void LC_BL_max_ter_groups(nufpar_s *fp) {
    levelconfig_ldata->max_ter_groups = (i16)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_ter_platforms(nufpar_s *fp) {
    levelconfig_ldata->max_ter_platforms = (i16)NuFParGetInt(fp);
}

static __used__ void LC_BL_max_tightropes(nufpar_s *fp);

static __used__ void LC_BL_max_timers(nufpar_s *fp);

static __used__ void LC_BL_max_torpmachines(nufpar_s *fp);

static __used__ void LC_BL_max_tubes(nufpar_s *fp);

static __used__ void LC_BL_max_turrets(nufpar_s *fp);

static __used__ void LC_BL_max_zipups(nufpar_s *fp);

static __used__ void LC_BL_mipmapmode(nufpar_s *fp) {
    levelconfig_ldata->mipmap_mode = (u8)NuFParGetInt(fp);
}

static __used__ void LC_BL_wind_size(nufpar_s *fp);

static __used__ void LC_BL_wind_speed(nufpar_s *fp);

// Game-specific pre-load limits. These must be applied before WorldInfo
// reserves each level's fixed-size gameplay systems.
extern NUFPCOMJMP LevelConfigKeywords_BeforeLoad[45];

static u8 load_conditionParam;

static void loadSumBox(nufpar_s *fp) {
    load_conditionParam = (u8)NuFParGetInt(fp);
}
static __used__ void LC_AL_metal(nufpar_s *fp) {
    levelconfig_ldata->flags |= LEVEL_METAL;
    if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "off") == 0) {
        levelconfig_ldata->flags &= ~LEVEL_METAL;
    }
}
static __used__ void LC_AL_music(nufpar_s *fp) {
    if (NuFParGetWord(fp) != 0) {
        levelconfig_ldata->music_index = (i16)GetMusicIndex(fp->word_buf, MusicInfo, -1);
        levelconfig_ldata->music_tracks[0][0] = music_man.GetTrackHandle(TRACK_CLASS_QUIET, fp->word_buf);
        levelconfig_ldata->music_tracks[1][0] = music_man.GetTrackHandle(TRACK_CLASS_ACTION, fp->word_buf);
        levelconfig_ldata->music_tracks[2][0] = music_man.GetTrackHandle(TRACK_CLASS_NOMUSIC, fp->word_buf);
    }
}
static __used__ void LC_AL_music_other(nufpar_s *fp) {
    if (NuFParGetWord(fp) != 0) {
        levelconfig_ldata->unknown_0a8 = (i16)GetMusicIndex(fp->word_buf, MusicInfo, -1);
        levelconfig_ldata->music_tracks[0][1] = music_man.GetTrackHandle(TRACK_CLASS_QUIET, fp->word_buf);
        levelconfig_ldata->music_tracks[1][1] = music_man.GetTrackHandle(TRACK_CLASS_ACTION, fp->word_buf);
        levelconfig_ldata->music_tracks[2][1] = music_man.GetTrackHandle(TRACK_CLASS_NOMUSIC, fp->word_buf);
    }
}
static __used__ void LC_AL_farclip(nufpar_s *fp) {
    i32 v = NuFParGetInt(fp);
    i32 farclip;
    if (v > 9) {
        farclip = (v <= 20000) ? v : 20000;
    } else {
        farclip = 10;
    }
    levelconfig_ldata->data_display.unknown_14 = (i16)(
        NuIOS_IsLowEndDevice()
            ? ((f32)farclip < levelconfig_ldata->data_display.far_clip
                   ? (f32)farclip
                   : levelconfig_ldata->data_display.far_clip)
            : (f32)farclip);
}
static __used__ void LC_AL_camera_rain(nufpar_s *fp) {
    levelconfig_ldata->flags |= LEVEL_CAMERA_RAIN;
    if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "off") == 0) {
        levelconfig_ldata->flags &= ~LEVEL_CAMERA_RAIN;
    }
}
static __used__ void LC_AL_in_space(nufpar_s *fp) {
    levelconfig_ldata->flags |= LEVEL_IN_SPACE;
    if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "off") == 0) {
        levelconfig_ldata->flags &= ~LEVEL_IN_SPACE;
    }
}
static __used__ void LC_AL_flat_terrain(nufpar_s *fp) {
    levelconfig_ldata->flags |= LEVEL_FLAT_TERRAIN;
    if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "off") == 0) {
        levelconfig_ldata->flags &= ~LEVEL_FLAT_TERRAIN;
    }
}
static __used__ void LC_AL_hidden_icons(nufpar_s *fp) {
    if (NuFParGetWord(fp) != 0) {
        if (NuStrICmp(fp->word_buf, "on") == 0) {
            levelconfig_ldata->flags &= ~LEVEL_HIDE_ICONS;
            return;
        }
        if (NuStrICmp(fp->word_buf, "off") == 0) {
            levelconfig_ldata->flags |= LEVEL_HIDE_ICONS;
        }
    }
}
static __used__ void LC_AL_double_score(nufpar_s *fp) {
    levelconfig_ldata->flags |= LEVEL_DOUBLE_SCORE;
    if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "off") == 0) {
        levelconfig_ldata->flags &= ~LEVEL_DOUBLE_SCORE;
    }
}
static __used__ void LC_AL_narrow_socks(nufpar_s *fp) {
    levelconfig_ldata->flags |= LEVEL_NARROW_SOCKS;
    if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "off") == 0) {
        levelconfig_ldata->flags &= ~LEVEL_NARROW_SOCKS;
    }
}
static __used__ void LC_AL_terrain_rain(nufpar_s *fp) {
    levelconfig_ldata->flags |= LEVEL_TERRAIN_RAIN;
    if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "off") == 0) {
        levelconfig_ldata->flags &= ~LEVEL_TERRAIN_RAIN;
    }
}
static __used__ void LC_AL_lowendfarclip(nufpar_s *fp) {
    f32 v = NuFParGetFloat(fp);
    if (v < 2.0f) {
        v = 2.0f;
    } else if (v > 20000.0f) {
        v = 20000.0f;
    }
    levelconfig_ldata->data_display.far_clip = v;
    if (NuIOS_IsLowEndDevice()) {
        levelconfig_ldata->data_display.unknown_14 = (i16)v;
    }
}
static __used__ void LC_AL_cam_lateral_dist(nufpar_s *fp) {
    f32 v = NuFParGetFloat(fp);
    if (v < 0.0f) {
        v = 0.0f;
    }
    levelconfig_ldata->cam_lateral_dist = v;
}
static __used__ void LC_AL_cam_pullback_dist(nufpar_s *fp) {
    f32 v = NuFParGetFloat(fp);
    if (v < 0.0f) {
        v = 0.0f;
    }
    levelconfig_ldata->cam_pullback_dist = v;
}
static __used__ void LC_AL_lowendcharclip(nufpar_s *fp) {
    f32 v = NuFParGetFloat(fp);
    if (NuIOS_IsLowEndDevice()) {
        levelconfig_ldata->unknown_11c = v;
    } else {
        levelconfig_ldata->unknown_11c = 0.0f;
    }
}
static __used__ void LC_AL_lowendfogstart(nufpar_s *fp) {
    f32 v = NuFParGetFloat(fp);
    if (v < 2.0f) {
        v = 2.0f;
    } else if (v > 20000.0f) {
        v = 20000.0f;
    }
    levelconfig_ldata->data_display.fog_start = v;
}
static __used__ void LC_AL_nearclip(nufpar_s *fp) {
    f32 v = NuFParGetFloat(fp);
    if (v < 0.001f) {
        v = 0.001f;
    } else if (v > 1.0f) {
        v = 1.0f;
    }
    levelconfig_ldata->data_display.unknown_04 = v;
}
static __used__ void LC_AL_farclip_hack(nufpar_s *fp) {
    f32 v = NuFParGetFloat(fp);
    if (v < 0.1f) {
        v = 0.1f;
    } else if (v > 50.0f) {
        v = 50.0f;
    }
    levelconfig_ldata->data_display.unknown_00 = v;
}
static __used__ void LC_AL_lowendcamerazoom(nufpar_s *fp) {
    f32 v = NuFParGetFloat(fp);
    if (NuIOS_IsLowEndDevice()) {
        levelconfig_ldata->unknown_120 = v;
    } else {
        levelconfig_ldata->unknown_120 = 1.0f;
    }
}
static __used__ void LC_AL_lowendparticlethin(nufpar_s *fp) {
    f32 v = NuFParGetFloat(fp);
    if (NuIOS_IsLowEndDevice()) {
        levelconfig_ldata->data_display.particle_thin = v;
    } else {
        levelconfig_ldata->data_display.particle_thin = 1.0f;
    }
}
static __used__ void LC_AL_forget_takeovers(nufpar_s *fp) {
    levelconfig_ldata->flags |= LEVEL_FORGET_TAKEOVERS;
    if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "off") == 0) {
        levelconfig_ldata->flags &= ~LEVEL_FORGET_TAKEOVERS;
    }
}
static __used__ void LC_AL_pickups_to_panel(nufpar_s *fp) {
    levelconfig_ldata->flags |= LEVEL_PICKUPS_TO_PANEL;
    if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "off") == 0) {
        levelconfig_ldata->flags &= ~LEVEL_PICKUPS_TO_PANEL;
    }
}
static __used__ void LC_AL_override_nopickupgravity(nufpar_s *fp) {
    levelconfig_ldata->flags |= LEVEL_OVERRIDE_NO_PICKUP_GRAVITY;
    if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "off") == 0) {
        levelconfig_ldata->flags &= ~LEVEL_OVERRIDE_NO_PICKUP_GRAVITY;
    }
}
static __used__ void LC_BL_fix_strobing_anims(nufpar_s *fp) {
    (void)fp;
    levelconfig_ldata->flags |= LEVEL_FIX_STROBING_ANIMS;
}

static NUFPCOMJMP LevelConfig_BeforeLoad_GenericKeywords[] = {
    {const_cast<char *>("mipmapmode"), LC_BL_mipmapmode},
    {const_cast<char *>("fix_strobing_anims"), LC_BL_fix_strobing_anims},
    {const_cast<char *>("max_ter_groups"), LC_BL_max_ter_groups},
    {const_cast<char *>("max_ter_platforms"), LC_BL_max_ter_platforms},
    {NULL, NULL},
};

static NUFPCOMJMP LevelConfig_AfterLoad_GenericKeywords[] = {
    {const_cast<char *>("backr"), LC_AL_backr},
    {const_cast<char *>("backg"), LC_AL_backg},
    {const_cast<char *>("backb"), LC_AL_backb},
    {const_cast<char *>("backr_top"), LC_AL_backr_top},
    {const_cast<char *>("backg_top"), LC_AL_backg_top},
    {const_cast<char *>("backb_top"), LC_AL_backb_top},
    {const_cast<char *>("backr_bottom"), LC_AL_backr_bottom},
    {const_cast<char *>("backg_bottom"), LC_AL_backg_bottom},
    {const_cast<char *>("backb_bottom"), LC_AL_backb_bottom},
    {const_cast<char *>("farclip_hack"), LC_AL_farclip_hack},
    {const_cast<char *>("lowendfarclip"), LC_AL_lowendfarclip},
    {const_cast<char *>("lowendfogstart"), LC_AL_lowendfogstart},
    {const_cast<char *>("lowendparticlethin"), LC_AL_lowendparticlethin},
    {const_cast<char *>("farclip"), LC_AL_farclip},
    {const_cast<char *>("nearclip"), LC_AL_nearclip},
    {const_cast<char *>("pspcharclip"), LC_AL_lowendcharclip},
    {const_cast<char *>("lowendcharclip"), LC_AL_lowendcharclip},
    {const_cast<char *>("lowendcamerazoom"), LC_AL_lowendcamerazoom},
    {const_cast<char *>("farclip_pc"), LC_AL_farclip},
    {const_cast<char *>("nearclip_pc"), LC_AL_nearclip},
    {const_cast<char *>("hover_height"), LC_AL_hover_height},
    {const_cast<char *>("blobshadow_alpha"), LC_AL_blobshadow_alpha},
    {const_cast<char *>("blobshadow_fadenear"), LC_AL_blobshadow_fadenear},
    {const_cast<char *>("blobshadow_fadefar"), LC_AL_blobshadow_fadefar},
    {const_cast<char *>("reflect_range"), LC_AL_reflect_range},
    {const_cast<char *>("reflect_y"), LC_AL_reflect_y},
    {const_cast<char *>("metal"), LC_AL_metal},
    {const_cast<char *>("in_space"), LC_AL_in_space},
    {const_cast<char *>("override_nopickupgravity"), LC_AL_override_nopickupgravity},
    {const_cast<char *>("hidden_icons"), LC_AL_hidden_icons},
    {const_cast<char *>("pickups_to_panel"), LC_AL_pickups_to_panel},
    {const_cast<char *>("forget_takeovers"), LC_AL_forget_takeovers},
    {const_cast<char *>("narrow_socks"), LC_AL_narrow_socks},
    {const_cast<char *>("camera_rain"), LC_AL_camera_rain},
    {const_cast<char *>("terrain_rain"), LC_AL_terrain_rain},
    {const_cast<char *>("double_score"), LC_AL_double_score},
    {const_cast<char *>("flat_terrain"), LC_AL_flat_terrain},
    {const_cast<char *>("cam_tilt"), LC_AL_cam_tilt},
    {const_cast<char *>("raycaststep"), LC_AL_raycaststep},
    {const_cast<char *>("plat_scan_dist"), LC_AL_plat_scan_dist},
    {const_cast<char *>("waterripple_startcol_r"), LC_AL_waterripple_startcol_r},
    {const_cast<char *>("waterripple_startcol_g"), LC_AL_waterripple_startcol_g},
    {const_cast<char *>("waterripple_startcol_b"), LC_AL_waterripple_startcol_b},
    {const_cast<char *>("waterripple_startcol_a"), LC_AL_waterripple_startcol_a},
    {const_cast<char *>("waterripple_endcol_r"), LC_AL_waterripple_endcol_r},
    {const_cast<char *>("waterripple_endcol_g"), LC_AL_waterripple_endcol_g},
    {const_cast<char *>("waterripple_endcol_b"), LC_AL_waterripple_endcol_b},
    {const_cast<char *>("waterripple_endcol_a"), LC_AL_waterripple_endcol_a},
    {const_cast<char *>("waterripple_life"), LC_AL_waterripple_life},
    {const_cast<char *>("cam_pullback_dist"), LC_AL_cam_pullback_dist},
    {const_cast<char *>("cam_lateral_dist"), LC_AL_cam_lateral_dist},
    {const_cast<char *>("campos_seek"), LC_AL_campos_seek},
    {const_cast<char *>("camang_seek"), LC_AL_camang_seek},
    {const_cast<char *>("music"), LC_AL_music},
    {const_cast<char *>("music_other"), LC_AL_music_other},
    {const_cast<char *>("sfx_ambient"), LC_AL_sfx_ambient},
    {const_cast<char *>("conveyor"), LC_AL_conveyor},
    {NULL, NULL},
};

void LevelConfig_BeforeLoad(LEVELDATA *level, char *buffer, nufpcomjmp_s *keywords) {
    NUFPAR *parser = NuFParCreateMem("levelbeforeload", buffer, 0xffff);
    if (parser == NULL) {
        return;
    }

    levelconfig_ldata = level;
    NuFParPushCom2(parser, LevelConfig_BeforeLoad_GenericKeywords, keywords);
    while (NuFParGetLine(parser) != 0) {
        if (NuFParGetWord(parser) != 0) {
            NuFParInterpretWord(parser);
        }
    }
    NuFParDestroy(parser);
}

void LevelConfig_AfterLoad(LEVELDATA *level, char *buffer, nufpcomjmp_s *keywords) {
    NUFPAR *parser = NuFParCreateMem("levelafterload", buffer, 0xffff);
    if (parser == NULL) {
        return;
    }

    levelconfig_ldata = level;
    NuFParPushCom2(parser, LevelConfig_AfterLoad_GenericKeywords, keywords);
    while (NuFParGetLine(parser) != 0) {
        if (NuFParGetWord(parser) != 0) {
            NuFParInterpretWord(parser);
        }
    }
    NuFParDestroy(parser);

    if (level->blob_shadow_fade_near > level->blob_shadow_fade_far) {
        level->blob_shadow_fade_near = level->blob_shadow_fade_far;
    }
    level->field91_0x118 = level->data_display.unknown_14;
    level->flags |= LEVEL_CONFIG_LOADED;
}
