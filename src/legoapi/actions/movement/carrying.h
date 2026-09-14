#pragma once

#include "nu2api/nucore/common.h"

struct GameObject_s;
struct GIZMOBLOWUP_s;
struct WORLDINFO_s;
struct nuvec_s;

void SuperCarry_DrawObject(GameObject_s *object);
void SuperCarry_GetObjectPos(GameObject_s *object, nuvec_s *position, nuvec_s *secondary_position);
void SuperCarry_Throw(GameObject_s *object, i32 mode);
i32 SuperCarry_Possible(GameObject_s *object, i32 require_grounded);
void SuperCarry_Start(GameObject_s *object, GIZMOBLOWUP_s *blowup, i32 immediate);
void SuperCarry_MoveCode(WORLDINFO_s *world, GameObject_s *object);
void SuperCarry_Release(GameObject_s *object);
i32 SuperCarry_SetTargetMom(GameObject_s *object, f32 input_speed);
i32 SuperCarry_YRotation(GameObject_s *object, u16 input_angle);
i32 SuperCarry_Carrying(GameObject_s *object);
