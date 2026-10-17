#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct nugraph_s;
struct nuvec_s;
f32 nugraph_blend(i32 interval, i32 degree, i32 *knots, f32 parameter);
void nugraph_bspline(i32 segment_count, i32 degree, nuvec_s *control_points, nuvec_s *curve_points, i32 curve_count);
void nugraph_catmullrom(i32 segment_count, nuvec_s *control_points, nuvec_s *curve_points, i32 curve_count);
void nugraph_compute_catmull_point(i32 segment, f32 parameter, nuvec_s *control_points, nuvec_s *point);
void nugraph_compute_intervals(i32 *knots, i32 segment_count, i32 degree);
void nugraph_compute_linear_point(i32 segment, f32 parameter, nuvec_s *control_points, nuvec_s *point);
void nugraph_compute_point(i32 *knots, i32 segment_count, i32 degree, f32 parameter, nuvec_s *control_points, nuvec_s *point);
void nugraph_linear(i32 segment_count, nuvec_s *control_points, nuvec_s *curve_points, i32 curve_count);
extern "C" {
    f32 nugraphGetXatT(nugraph_s *graph, f32 parameter);
    f32 nugraphGetYatT(nugraph_s *graph, f32 parameter);
    f32 nugraphGetYatX(nugraph_s *graph, f32 x, i32 mode);
    i32 nugraphAddPoint(nugraph_s *graph, f32 x, f32 y);
    i32 nugraphCalcCurve(nugraph_s *graph, i32 point_count);
    i32 nugraphDeletePoint(nugraph_s *graph, i32 index);
    i32 nugraphGenerateLookupTable(nugraph_s *graph, f32 *table, i32 table_count);
    u32 NuWindRand(void);
    void nugraphFreeTempCurveData();
    void nugraphInit(nugraph_s *graph);
}
