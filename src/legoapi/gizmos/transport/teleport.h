#pragma once

#include "legoapi/gizmo/base/gizmo.h"

extern i32 teleport_gizmotype_id;

typedef struct TELEPORT_s TELEPORT;
struct GameObject_s;
struct HINT_s;
struct WORLDINFO_s;
struct VuVec;

ADDGIZMOTYPE *Teleport_RegisterGizmo(i32 type_id);
void Teleports_Configure(WORLDINFO_s *world, char *config);
TELEPORT *Teleport_Find(GameObject_s *object, f32 range_squared, VuVec *position);
void Teleports_Reset(WORLDINFO_s *world);
void Teleports_UpdateBeforeGameObjects(WORLDINFO_s *world);
void Teleports_UpdateAfterGameObjects(WORLDINFO_s *world);
void Teleport_MoveCode(GameObject_s *object, i32 special_pressed);
void Teleport_NetMoveCode(GameObject_s *object);
i32 Teleport_UpdateHints(HINT_s *hint);
