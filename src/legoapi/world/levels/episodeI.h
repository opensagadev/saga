#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct AIPACKET_s;
struct AISCRIPTPROCESS_s;
struct AISYS_s;
struct ANIMPACKET_s;
struct GameObject_s;
struct PART_s;
struct WORLDINFO_s;
struct nugspline_s;
i32 Action_CreatePod(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count, i32 first_time, float);
i32 Action_NewSebulba(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count, i32 first_time, float);
i32 Action_Sebulba(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count, i32 first_time, float);
i32 Action_SetLapTime(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count, i32 first_time, float);
extern void PodSprint_GetIAlongVals(nugspline_s *spline, i16 *first_point, i16 *last_point);
f32 PodSprint_InStartCountdown(WORLDINFO_s *);
i32 Action_MushroomCollapse(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count, i32 first_time, float);
i32 PodRace_InStartCountdown(WORLDINFO_s *world);
void Mine_Kill(PART_s *part, i32);
void PodDust(WORLDINFO_s *, GameObject_s *);
void PodRaceReset();
void PodRaceUpdate(WORLDINFO_s *world, float dt);
void PodRace_IncreaseLap();
void SetPodMergeAnims(ANIMPACKET_s *packet, i32 index);
extern "C" {
    extern f32 PODRACE_SPLINEINC;
}
