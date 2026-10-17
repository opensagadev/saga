#pragma once

#include "decomp.h"
#include "nu2api/nu3d/nuspline.h"

struct flightspline_s;
struct _vuv_s;
void CalcSplinePoint(flightspline_s *spline, _vuv_s *position, f32 along);

struct SPLINEPOS_s {
    NUGSPLINE *spline; // 0x00
    i16 segment;       // 0x04
    i8 looping;        // 0x06
    union {
        u8 reached_end;
        u8 finished;
    }; // 0x07
    union {
        f32 segment_distance;
        f32 distance;
    }; // 0x08
    f32 segment_length; // 0x0c
    NUVEC position;     // 0x10
    union {
        f32 along;
        f32 normalized_position;
    }; // 0x1c
};
DECOMP_ASSERT(sizeof(SPLINEPOS_s) == 0x20, "SPLINEPOS size");
DECOMP_ASSERT(offsetof(SPLINEPOS_s, segment) == 4, "SPLINEPOS segment offset");
DECOMP_ASSERT(offsetof(SPLINEPOS_s, looping) == 6, "SPLINEPOS looping offset");
DECOMP_ASSERT(offsetof(SPLINEPOS_s, segment_distance) == 8, "SPLINEPOS segment distance offset");
DECOMP_ASSERT(offsetof(SPLINEPOS_s, segment_length) == 0xc, "SPLINEPOS segment length offset");
DECOMP_ASSERT(offsetof(SPLINEPOS_s, position) == 0x10, "SPLINEPOS position offset");
DECOMP_ASSERT(offsetof(SPLINEPOS_s, along) == 0x1c, "SPLINEPOS along offset");

void PointAlongSpline(NUGSPLINE *, f32, NUVEC *, u16 *, u16 *, i32);
void InitSplinePosition(SPLINEPOS_s *, NUGSPLINE *, f32, i32);
void MoveSplinePosition(SPLINEPOS_s *, f32);
void GetNearestSplinePos(NUVEC *, SPLINEPOS_s *, NUGSPLINE *, i32, i16, i16);
i32 OutSideSplineArea(NUVEC *position, NUGSPLINE *spline, NUVEC *edge_end, NUVEC *edge_start, i32 inside);

// Declared here for the other units that use them.
struct GAMEANTINODEDATA_s;
struct nugscn_s;
struct nugspline_s;
struct nuhspecial_s;
extern char * EDSPLINE_FILECHECK;
i32 edSpline_FindAllBeg(nugscn_s *scene, char *prefix, nugspline_s **results, i32 capacity);
i32 edSpline_FindAllSub(nugscn_s *scene, char *name, nugspline_s **results, i32 capacity);
void GameAntiNodeData_Init(GAMEANTINODEDATA_s *data, nuhspecial_s *special);
void GameAntiNodeData_Read(GAMEANTINODEDATA_s *data);
void edSpline_SplineList(nugscn_s *);
