#pragma once

#include "nu2api/nucore/fixed_width.h"

struct GIZMOBLOWUP_s;
struct GameObject_s;
struct nuvec_s;
struct numtx_s;

void Transform_DrawTarget(nuvec_s *position, float radius, float alpha);
void QuatInterpolateRotationMatrix(numtx_s *result, numtx_s *first, numtx_s *second, float fraction);
GameObject_s *Transform_TargettedByObj(void *object);
void GizmoBlowup_TransformDraw_Game(GIZMOBLOWUP_s *blowup);
