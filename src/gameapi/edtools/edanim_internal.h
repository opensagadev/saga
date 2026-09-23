#pragma once

#include "gameapi/edtools/gameapi_edtools_types.h"

struct debinftype;
struct numtl_s;
typedef numtl_s NUMTL;
struct nugscn_s;
typedef nugscn_s NUGSCN;

// Animation-editor state shared by the reconstructed editor and sound owners.
extern "C" {
    extern edanim_param_s AnimParams[64];
    extern nugscn_s *edanim_page_scene[8];
    extern i32 edanim_page_used[8];
    extern i32 edanim_page_on[8];
    extern i32 edanim_next_param;
    extern i32 edanim_params_used;
    extern i32 edbits_anim_page;
    extern i32 edanim_nearest;
    extern i32 edanim_nearest_param_id;
    extern i32 edanim_nearest_sound;
    extern i32 edanim_nearest_particle;
    extern i32 edanim_sound_type;
    extern i32 edanim_particle_type;
    extern i32 edanim_particle_mode;
    extern i32 edanim_sound_mode;
    extern i32 edanim_emitroty;
    extern i32 edanim_emitrotz;
    extern NUVEC edanim_cam_pos;
    extern i32 edanim_cam_ax;
    extern i32 edanim_cam_ay;
    extern eduimenu_s *edanim_active_menu;
    extern eduimenu_s *edanim_options_menu;
    extern eduimenu_s *edanim_mctb_menu;
    extern eduimenu_s *edanim_switchtype_menu;
    extern eduimenu_s *edanim_switch_menu;
    extern eduimenu_s *edanim_bouncy_menu;
    extern eduimenu_s *edanim_localsoundtype_menu;
    extern eduimenu_s *edanim_localsound_menu;
    extern eduimenu_s *edanim_soundtype_menu;
    extern eduimenu_s *edanim_sound_menu;
    extern eduimenu_s *edanim_localparticletype_menu;
    extern eduimenu_s *edanim_localparticle_menu;
    extern eduimenu_s *edanim_particletype_menu;
    extern eduimenu_s *edanim_particle_menu;
    extern void *ed_fnt;
    extern i32 edbits_numsounds;
    extern i32 EDPP_MAX_TYPES;
    extern debinftype **debtab;
    extern NUMTL *edanim_mtl;
    extern NUMTL *edanim_mtl_zoff;
    extern NUGSCN *edbits_base_scene;
    extern char edbits_level_save_directory[256];
    extern char edbits_level_save_name[256];
    extern char edbits_level_save_extension[256];
    i32 edanimLoadPage(char *path, NUGSCN *scene);
    void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
    void PlatInstBounce(i32 index, f32 impulse, f32 spring, f32 damping);
    i32 FindPlatInst(i32 instance_index);
    void edanimParticleDestroy(i32 parameter_index, i32 particle_index);
    void edanimSoundDestroy(i32 parameter_index, i32 sound_index);
    char *edbitsGetSoundName(i32 sound_type);
    i32 edbitsStartCubemapDump();
    i32 edbitsProcessCubemapDump();
}

void edanimDoInput(nupad_s *pad);
i32 edanimParamCreate(i32 instance_id);
void edanimParamDestroy(i32 parameter_index);
void edanimParticlePlace(i32 particle_index, NUVEC *position);
void edanimParticleCreate(NUVEC *position);
void edanimSoundPlace(i32 sound_index, NUVEC *position);
void edanimSoundCreate(NUVEC *position);
void edanimDetermineNearestAnim(f32 radius);
void edanimDetermineNearestSound(f32 radius);
void edanimDetermineNearestParticle(f32 radius);
void edanimDrawCursor();
void edanimRenderSoundEmitters(i32 parameter_index);
void edanimRenderParticleEmitters(i32 parameter_index);
extern "C" void edanimParamReset();
void edanimStartAllPages();
i32 edanimFileSave(char *path);
