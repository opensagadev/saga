#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct AILOCATOR_s;
struct AIPACKET_s;
struct AISCRIPTPROCESS_s;
struct AISYS_s;
struct nuvec_s;
AILOCATOR_s *LocalGetNearestLocator(AILOCATOR_s **, i32, f32, nuvec_s *, f32, i32, f32, f32);
AILOCATOR_s *LocalGetRandomLocator(AILOCATOR_s **, i32, f32, nuvec_s *, f32, i32, f32, f32);
extern f32 guided_life;
extern f32 guided_speed;
extern f32 guided_start_time;
extern f32 party_follow_offsets[8];
extern i32 guided_rotate_speed;
i16 GetGenericGoon(i32);
i32 Action_AssignLocatorInSet(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count, i32 first_time, float);
i32 Action_DynamicCameraCut(AISYS_s *system, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params, i32 param_count, i32 first_time, float);
i32 Action_GameFollowPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32);
i32 Action_GetLocatorFromSet(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params, i32 param_count, i32 first_time, float);
i32 Action_HelpWithTriggers(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32);
i32 Action_MoveForward(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params, i32 param_count, i32 first_time, f32);
i32 Action_PullLever(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32);
i32 Action_ReleaseLocator(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count, i32 first_time, float);
i32 Action_SetVisibility(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32);
i32 Action_UsePanel(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32);
i32 Action_UseTechno(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32);
i32 Action_UseTriggerSet(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32);
