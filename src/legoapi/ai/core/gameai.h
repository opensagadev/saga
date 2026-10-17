#pragma once

#include "decomp.h"

extern "C" i32 adaptivedifficulty[3];
extern i8 (*adtab)[4];

// Declared here for the other units that use them.
struct AIAREA_s;
struct AIPATHCNXCONTROLLER_s;
struct AIPATHCNXCONTROLSYS_s;
struct AIPATHINFO_s;
struct AIPATH_s;
struct AISYS_s;
struct APIOBJECT_s;
struct GIZMO_s;
struct GameObject_s;
struct nuvec_s;
char *AI_GetOutputName(GIZMO_s *, i32);
extern AIPATHCNXCONTROLLER_s *AIPathCnxControllerCreate(AIPATHCNXCONTROLSYS_s *control_system, AISYS_s *ai_system, AIPATH_s *path, char *from, char *to, i32 target_type, char *target_name, i32 fake_animation_id, i32 gizmo_output);
extern GameObject_s *alert_obj;
extern f32 alert_timer;
extern f32 draw_attention_distance;
extern f32 oneAtOnce_GetHoldRange(GameObject_s *object);
extern f32 timetogetlos;
extern i32 CheckPosAIArea(AIAREA_s *area, nuvec_s *position, f32 tolerance);
extern i32 WALKSTEALTH;
extern i32 active_goody_count;
extern i32 debug_ignore_players;
extern i32 drop_in_teleport;
extern i32 oneAtOnce_CanAttack(GameObject_s *object, GameObject_s *opponent);
extern i32 teleport_all_freeplay_modes;
extern i32 test_new_interaction;
extern nuvec_s alert_pos;
extern void AIPathCnxControllerSetOnRange(AIPATHCNXCONTROLLER_s *controller, i32 start_frame, i32 end_frame);
extern void oneAtOnce_MaintainArray();
extern void oneAtOnce_SetAttackersPerRow(i32);
extern void oneAtOnce_SetDistPerRow(f32);
extern void oneAtOnce_SetInitDistPerRow(f32 distance);
extern void oneAtOnce_SetNumAttackers(i32);
i32 CanFightLikeAJedi(GameObject_s *object);
void AIPathCnxControlSysReset(AIPATHCNXCONTROLSYS_s *system);
void AISysGetPathPos2(AISYS_s *, nuvec_s *, AIPATHINFO_s *, nuvec_s *, AIPATH_s *, i32);
void AI_Activate(GIZMO_s *gizmo, i32 active);
void AlertSurroundingCreatures(GameObject_s *object, nuvec_s *position);
void GameAIProcess();
void GameCreatureOpponentSelection(AISYS_s *, i32, APIOBJECT_s **, i32, APIOBJECT_s **, i32, APIOBJECT_s **, u64, f32);
void MakeBaddiesForgetAboutParty(i32);
extern "C" {
    extern f32 default_max_t_respawn;
    extern f32 default_min_t_respawn;
    extern f32 default_path_heighttol;
    extern u8 default_activate_difficulty;
    extern u8 default_max_n_respawns;
    extern u8 default_min_n_respawns;
}
