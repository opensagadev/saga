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
    extern i32 edanim_nearest;
    extern i32 edanim_nearest_param_id;
    extern i32 edanim_nearest_sound;
    extern i32 edanim_nearest_particle;
    extern i32 edanim_sound_type;
    extern i32 edanim_particle_type;
    extern i32 edanim_particle_mode;
    extern i32 edanim_sound_mode;
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
    void PlatInstBounce(i32 index, f32 impulse, f32 spring, f32 damping);
    char *edbitsGetSoundName(i32 sound_type);
    i32 edbitsStartCubemapDump();
    i32 edbitsProcessCubemapDump();
}

void edanimDoInput(nupad_s *pad);
void edanimDetermineNearestAnim(f32 radius);
void edanimDetermineNearestSound(f32 radius);
void edanimDetermineNearestParticle(f32 radius);
void edanimDrawCursor();
void edanimRenderSoundEmitters(i32 parameter_index);
void edanimRenderParticleEmitters(i32 parameter_index);
extern "C" void edanimParamReset();
