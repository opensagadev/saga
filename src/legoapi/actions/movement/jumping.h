#pragma once

#include "nu2api/nucore/fixed_width.h"

struct GameObject_s;

void StartJump(GameObject_s *object, i32 movement_state);
void StartEndOfJump(GameObject_s *object);
i32 StartFallLand(GameObject_s *object, i32 action);

// Declared here for the other units that use them.
struct GameObject_s;
struct nuvec_s;
extern f32 SLAMJUMPSPEED;
extern i32 DoubleJump_JediSlam;
i32 Slam_Start(GameObject_s *object, f32 speed);
i32 StartBackFlip(GameObject_s *object);
i32 UseFallAnim(GameObject_s *object);
void BackFlipCode(GameObject_s *object);
void BigJumpCode(GameObject_s *object);
void FindSlamOrigin(GameObject_s *, nuvec_s *, nuvec_s *);
void MakeJumpReachHeight(GameObject_s *object, float height, i32 force);
