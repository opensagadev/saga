#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct GameObject_s;
struct nuvec_s;
extern i32 TagCharacter(GameObject_s *source, GameObject_s *target, i32 mode);
extern i32 players_cannot_exit_speeder;
extern void (*Tag_DrawIconFn)(GameObject_s *);
i32 TagCode(GameObject_s *, GameObject_s *, i32, i32, i32);
i32 TakeOverYoda(GameObject_s *rider, GameObject_s *vehicle, i32 blend_camera, i32);
void GetTakeOverPos(GameObject_s *, nuvec_s *);
void Tag_Check(GameObject_s *object);
void Tag_SetMode(i32 mode);
void TakeOver2GetIn(GameObject_s *, GameObject_s *);
void TakeOverCode(GameObject_s *object, i32 tag_pressed);
void TakeOverGameObject2(GameObject_s *, GameObject_s *, i32);
