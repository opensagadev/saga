#pragma once

#include "nu2api/nucore/fixed_width.h"

struct WORLDINFO_s;
struct GIZMOBLOWUP_s;
struct GameObject_s;
struct nuvec_s;
struct nuinstanim_s;

extern i32 (*BlowupExFunc)(GIZMOBLOWUP_s *, i32);
extern i32 blowup_gizmotype_id;
extern u32 EXBLOWUPFLAGS;
extern i32 (*GizmoBlowUp_NoTargetFn)(WORLDINFO_s *, GameObject_s *);
extern void (*GizmoBlowUp_SfxFn)(GIZMOBLOWUP_s *, nuvec_s *);
extern void (*GizmoBlowup_TransformDrawFn)(GIZMOBLOWUP_s *);
extern void (*GameBlowUpBlownUpFn)(GIZMOBLOWUP_s *);

void SetLevelExBlowupFlags(u32 flags);
u32 GetLevelExBlowupFlags(void);

void GizmoBlowUpTypeBlowUp(WORLDINFO_s *world, i32 type, nuvec_s *position);
void GizmoBlowUp_AddEffects(nuvec_s *position, GIZMOBLOWUP_s *blowup, i32 count, i32 flags, GameObject_s *object);
void GizBlowup_Respawn(GIZMOBLOWUP_s *blowup);
void GizmoBlowupCreateStuff(GIZMOBLOWUP_s *blowup);
i32 GizmoBlowupBlowup(GIZMOBLOWUP_s *blowup, i32 effects, i32 hit_type, i32 damage, GameObject_s *object,
                      i32 hit_context);
void GizmoBlowupResetNameTable(void);
void UpdateMidPos(GIZMOBLOWUP_s *blowup);
void PlayAnim(nuinstanim_s *animation, float *playback, float speed, i32 backwards);
i32 GizmoBlowupGetNameTableId(char *name);
i32 GizmoBlowupGetTypeFromNameTableId(WORLDINFO_s *world, i32 name_table_id);

// Declared here for the other units that use them.
struct BOLT_s;
struct GIZMOBLOWUP_s;
struct GIZMO_s;
struct GameObject_s;
struct WORLDINFO_s;
struct numtx_s;
GIZMOBLOWUP_s *GizmoBlowUp_Hit(GameObject_s *object, nuvec_s *points, i32 point_count, float radius, nuvec_s *minimum,
                               nuvec_s *maximum, BOLT_s *bolt, u32 hit_type, u8 *flags);
i32 Blowup_GetVisibility(GIZMO_s *gizmo);
i32 GizBlowup_InitSingleTerrain(GIZMOBLOWUP_s *blowup);
i32 GizmoBlowupCheckProximity(WORLDINFO_s *world, GameObject_s *object);
i32 gizmoblowup_Load(void *world_ptr, void *);
void *gizmoblowup_reservebuffers(void *world_ptr);
void GizBlowup_DeleteSingleTerrain(GIZMOBLOWUP_s *blowup);
void GizBlowup_DeleteTerrain();
void GizBlowup_InitTerrain();
void GizmoBlowupBurstDraw(void *world_ptr, void *, float);
void GizmoBlowupEarlyUpdate(void *world_ptr, void *, float);
void GizmoBlowupGenDecalMatrix(GIZMOBLOWUP_s *, numtx_s *, i32);
void GizmoBlowupGenShadowMatrix(GIZMOBLOWUP_s *, numtx_s *);
void GizmoBlowupLateUpdate(void *world_ptr, void *, float);
void GizmoBlowupUpdateMatrix(GIZMOBLOWUP_s *blowup);
void GizmoBlowupVisibilityOverrides(WORLDINFO_s *);
void GizmoBlowupsFinalSetup(WORLDINFO_s *world);
void SetLevelExBlowupFunc(i32 (*callback)(GIZMOBLOWUP_s *, i32));
u32 GizmoBlowups_TotalScore(void *context);
GIZMOBLOWUP_s *FindNearestGizmoBlowUp(WORLDINFO_s *world, nuvec_s *position, float max_distance_squared);
