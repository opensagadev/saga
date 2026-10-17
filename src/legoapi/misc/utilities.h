#pragma once

#include "nu2api/nucore/common.h"

struct nuvec_s;
struct numtx_s;
struct VuVec;

i32 charToInt(char const *text);
char *IToX(char *output, i32 value);
char *I64ToX(char *output, i64 value);
void CatIToX(char *output, i32 value);
void CatI64ToX(char *output, i64 value);

f32 XZLinesClosest(nuvec_s *first_start, nuvec_s *first_end, nuvec_s *second_start, nuvec_s *second_end,
                   f32 *first_fraction, f32 *second_fraction);
f32 LineToPointDistance(VuVec &origin, VuVec &direction, VuVec &point, VuVec *closest);
i32 LineToPlaneIntersecion(VuVec &origin, VuVec &direction, VuVec &plane, VuVec *intersection);
i32 LineToSphereIntersection(VuVec &origin, VuVec &direction, VuVec &center, f32 radius, VuVec *far_intersection,
                             VuVec *near_intersection);

void MakeThrowVector(nuvec_s *result, nuvec_s *origin, nuvec_s *target, nuvec_s *target_velocity, f32 speed,
                     f32 gravity);
i32 MatrixReflection(numtx_s *matrix, i32 axis, f32 plane, f32 height, numtx_s *result);
i32 MatrixReflectionVU0_AXISY(numtx_s *matrix, f32 plane, f32 scale, numtx_s *result);
void FindAnglesXY(nuvec_s *direction, u16 *x_rotation, u16 *y_rotation);
void GetRotationAngles(nuvec_s *direction, u16 *z_rotation, u16 *y_rotation);
void FindAnglesZX(nuvec_s *normal, u16 *x_rotation, u16 *z_rotation);
void CalculateInterceptVector(nuvec_s *origin, nuvec_s *target, nuvec_s *velocity, f32 speed, nuvec_s *direction,
                              nuvec_s *intercept_velocity);
i32 SphereSphereOverlapScaleY(nuvec_s *position_a, f32 radius_a, f32 y_radius_a, nuvec_s *position_b, f32 radius_b,
                              f32 y_radius_b);
i32 SphereSphereOverlap(nuvec_s *position_a, f32 radius_a, nuvec_s *position_b, f32 radius_b);

// Declared here for the other units that use them.
struct CHARACTERMODEL_s;
struct GAMECHARACTERDATA_s;
struct nugscn_s;
struct nuhspecial_s;
struct numtl_s;
struct numtx_s;
struct nuvec_s;
bool LineIntersectCircle(nuvec_s *, nuvec_s *, nuvec_s *, f32);
extern CHARACTERMODEL_s *dco_cmodel;
extern GAMECHARACTERDATA_s *dco_gcdata;
extern f32 CameraZoom;
extern f32 dco_reflectcoord;
extern i32 LineIntersectSphere(nuvec_s *, nuvec_s *, nuvec_s *, f32, f32 *);
extern i32 dco_id;
extern i32 dco_reflectaxis;
extern i32 dco_wearinghat;
extern i32 i_temp_xrot;
extern i32 qseed;
extern numtl_s *SolidMtl3D;
extern u16 dco_prerotatez;
f32 DistanceToLineXZ(nuvec_s *position, nuvec_s *first, nuvec_s *second);
f32 FindReflectionNoPlatforms(nuvec_s *position);
i32 DrawPanel3DObjectMtxNoAlpha(nuhspecial_s *special, numtx_s *matrix);
i32 LineIntersectXY(nuvec_s *a, nuvec_s *b, nuvec_s *c, nuvec_s *d, nuvec_s *first, nuvec_s *second);
i32 OnOrInsidePlane(nuvec_s *point, nuvec_s *plane_point, nuvec_s *plane_normal, nuvec_s *corrected_point, f32 normal_offset, f32 *distance);
void CreateUsefulMaterials();
void InitTexAnimScripts(char **names);
void InterpolateRotationMatrix(numtx_s *result, numtx_s *first, numtx_s *second, float fraction);
void ResetSeeds(void);
void RotateGameMatrix(numtx_s *matrix, i32 order, u16 x, u16 y, u16 z);
void SaveSceneObjectAnimTFactors(nugscn_s *gscn);
void VecRotateAxis(nuvec_s *, u16, nuvec_s *);
