#pragma once

#include "decomp.h"
#include "nu2api/nu3d/nuhspecial.h"

struct GameObject_s;

struct EQUIVALENTOBJECTGROUP_s {
    i16 object_count;
    i16 byte_size;
    nuhspecial_s objects[];
};
DECOMP_ASSERT(offsetof(EQUIVALENTOBJECTGROUP_s, objects) == 4, "Equivalent-object entries offset");
DECOMP_ASSERT(sizeof(EQUIVALENTOBJECTGROUP_s) == 4, "Equivalent-object group header size");

void Buck_Start(GameObject_s *object, GameObject_s *rider);
void Buck_StartRiderJump(GameObject_s *rider, GameObject_s *mount);

// Declared here for the other units that use them.
struct PART_s;
struct eduimenu_s;
struct nugscn_s;
struct nupad_s;
struct nuvec_s;
struct part_emit_s;
struct part_typedesc_s;
i32 edpartLookupDebrisEffect(char *name);
i32 edpartLookupObjectInScene(char *name, nugscn_s *scene);
i32 edpartSaveEffects(char *path, char page);
void NewPartOrderedRotation(PART_s *);
void PartCleanupTypes();
void PartTimeSlip();
void UpdatePartEmits(f32);
void edpartCreate(nuvec_s *position, i32 type);
void edpartDestroy(i32 index);
void edpartDetermineNearest(f32 distance);
void edpartDoInput(nupad_s *pad);
void edpartDrawCursor();
void edpartHighlightNearest();
void edpartInitType(i32 type);
void edpartLoadSingleType(part_typedesc_s *type, i32 version, i32 page);
void edpartMultipleCopyClear();
void edpartMultipleCopyCopy();
void edpartPlace(i32 index, nuvec_s *position);
void edpartScaleType(i32 type, f32 scale);
extern "C" {
    extern eduimenu_s * confirmmenu;
    extern eduimenu_s * deleteeffectsmenu;
    extern eduimenu_s * filenamemenu;
    extern eduimenu_s * loadeffectsmenu;
    extern eduimenu_s * memcardmenu;
    extern eduimenu_s * mergeeffectsmenu;
    extern eduimenu_s * totalptlsmenu;
    extern eduimenu_s *edpart_active_menu;
    extern eduimenu_s *edpart_changegenrate_menu;
    extern eduimenu_s *edpart_cutoff_menu;
    extern eduimenu_s *edpart_debrisscale_menu;
    extern eduimenu_s *edpart_emittime_menu;
    extern eduimenu_s *edpart_instancescale_menu;
    extern eduimenu_s *edpart_name_menu;
    extern eduimenu_s *edpart_opt_menu;
    extern eduimenu_s *edpart_scaletype_menu;
    extern eduimenu_s *edpart_sscale_menu;
    extern eduimenu_s *edpart_type_menu;
    extern f32 edanim_cam_dist;
    extern f32 edbri_cam_dist;
    extern f32 edpart_scale_factor;
    extern f32 edpp_cam_dist;
    extern f32 edpp_offset;
    extern i32 edbits_duplicates_error;
    extern i32 edbits_part_level_page;
    extern i32 edpart_create_type;
    extern i32 edpart_instances_used;
    extern i32 edpart_nearest;
    extern i32 edpp_cam_ax;
    extern i32 edpp_cam_ay;
    extern i32 edpp_copy_source[8];
    extern i32 edpp_numparticles;
    extern i32 part_emits_used;
    extern i32 part_page_on[8];
    extern i32 part_page_used[8];
    extern i32 part_platimpactcnt;
    extern i32 part_scene_pageid[32];
    extern i32 part_types_used;
    extern nugscn_s *part_scene[32];
    extern nuvec_s edbri_cam_pos;
    extern nuvec_s edpart_cam_pos;
    extern nuvec_s edpp_copy_source_vec;
    extern part_emit_s *edpart_nearest_emit;
    extern part_emit_s part_emits[40];
    extern part_typedesc_s part_types[128];
    i32 AddPARTEffect(i32, nuvec_s *);
    i32 CreateScaledPARTEffect(i32 effect_index, f32 requested_scale);
    i32 PartPlatId();
    i32 PartRayCast(nuvec_s *position, nuvec_s *movement, f32 radius, i32 flags);
    i32 edpartLoadPageEx(char *path, i8 mode, nugscn_s **scenes, i32 scene_count);
    void CastPart(PART_s *part, f32 time);
    void CheckPartCount(void);
    void InitParts(i32, VARIPTR *, VARIPTR *);
    void KillPartsByScene(nugscn_s *);
    void NewPartRotation(PART_s *part);
    void NuPartResetGlobalTime(void);
    void NuPartSetSeed(i32);
    void PARTStartOffset(i32, f32);
    void PartTerrInit(void);
    void RemovePARTEffect(i32 index);
    void ResetParts(void);
    void edpartDestroyAllParticles(void);
    void edpartParticleReset(void);
    void edpartRegisterPointerToGameCharLocation(nuvec_s *position);
}
