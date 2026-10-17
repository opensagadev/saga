#pragma once

#include "decomp.h"

#include "nu2api/nucore/common.h"

struct GameObject_s;
struct ADDPART_s;
struct AISYS_s;
struct AISCRIPTPROCESS_s;
struct AIPACKET_s;

struct SPEEDERCHASEVALUES {
    f32 distance;
    f32 ahead_range;
    f32 ahead_speed;
    f32 ahead_seek;
    f32 behind_range;
    f32 behind_speed;
    f32 behind_seek;
};

extern SPEEDERCHASEVALUES speedervals[4];
extern f32 zoom_ahead_extra;
extern f32 zoom_ahead_extra_time;
extern f32 speeder_level_height_lerpf;
extern f32 speeder_level_height;
extern f32 speederfirerange;
extern f32 speeder_shootrate;
extern f32 speeder_kill_dist;
extern f32 speeder_midrange_time;
extern f32 speeder_ahead_time;
extern f32 speeder_mode_ahead_timer;
extern i32 speeder_hitpoints_lost;
extern i32 players_going_forward;

i32 Action_SpeederBeingChased(AISYS_s *system, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                              i32 param_count, i32 first_time, f32 elapsed);

extern i32 objopponent_ignoreaiopponent;
i32 ObjOpponentStillThere(GameObject_s *object, GameObject_s *opponent, f32 gap);
i32 ObjIsTargetSpeeder(GameObject_s *object);
void InitBikeParts();
void KillParts_SpeederBike(ADDPART_s *params, i32 animation, i32 variant, GameObject_s *object);

// Declared here for the other units that use them.
struct GIZMOBLOWUP_s;
struct nuvec_s;
extern i32 CanPunchGirls;
extern i32 test_ai_combo;
extern u8 troopercannons_beenReset;
f32 SpeederChaseATATInOutMul(nuvec_s *, nuvec_s *);
f32 getPodRoll(i32 index);
i32 SpeederBlowupHack(GIZMOBLOWUP_s *blowup, i32);

DECOMP_ASSERT(sizeof(SPEEDERCHASEVALUES) == 0x1c, "Speeder chase tuning stride");
