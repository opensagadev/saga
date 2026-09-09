#pragma once

#include "nu2api/nucore/common.h"
#include "nu2api/numath/nuvec.h"

typedef struct nuplane_s {
    f32 a;
    f32 b;
    f32 c;
    f32 d;
} NUPLANE;

#ifdef __cplusplus
extern "C" {
#endif
    /// @brief Constructs a plane equation from three non-collinear points.
    /// @param out The constructed plane.
    /// @param pnt0 The first point in the plane.
    /// @param pnt1 The second point in the plane.
    /// @param pnt2 The third point in the plane.
    void NuPlnEqn(NUPLANE *out, NUVEC *pnt0, NUVEC *pnt1, NUVEC *pnt2);
    i32 NuPlnPlnIntersect(NUPLANE *first, NUPLANE *second, NUVEC *point, NUVEC *direction);
    void NuPlnEqnPn(NUPLANE *out, NUVEC *point, NUVEC *normal);
    f32 NuPlnDist(NUPLANE *plane, NUVEC *point);
    f32 NuPlnDist2(NUPLANE *plane, NUVEC *a, NUVEC *b);
    i32 NuPlnLineVU0(NUPLANE *plane, NUVEC *start, NUVEC *end, NUVEC *out);
    i32 NuPlnLine2(NUPLANE *plane, NUVEC *a, NUVEC *b, NUVEC *c, NUVEC *start, NUVEC *end, NUVEC *out, f32 *distance,
                   f32 *fraction);

    i32 NuPlnLine(NUPLANE *plane, NUVEC *s, NUVEC *e, NUVEC *out);
    f32 NuPlnLine3(NUPLANE *plane, NUVEC *pnt, NUVEC *v, NUVEC *out);

    i32 NuPtInPoly(NUVEC *pnt, NUVEC *t0, NUVEC *t1, NUVEC *t2, NUPLANE *plane);
    i32 NuPtInPolyXY(NUVEC *pnt, NUVEC *t0, NUVEC *t1, NUVEC *t2);
    i32 NuPtInPolyYX(NUVEC *pnt, NUVEC *t0, NUVEC *t1, NUVEC *t2);
    i32 NuPtInPolyXZ(NUVEC *pnt, NUVEC *t0, NUVEC *t1, NUVEC *t2);
    i32 NuPtInPolyZX(NUVEC *pnt, NUVEC *t0, NUVEC *t1, NUVEC *t2);
    i32 NuPtInPolyYZ(NUVEC *pnt, NUVEC *t0, NUVEC *t1, NUVEC *t2);
    i32 NuPtInPolyZY(NUVEC *pnt, NUVEC *t0, NUVEC *t1, NUVEC *t2);

    f32 NuLineToPointDistSqrEx(NUVEC *s, NUVEC *e, NUVEC *pnt, NUVEC *out);
    f32 NuLineToPointDistSqr(NUVEC *start, NUVEC *end, NUVEC *point);
    f32 NuInfiniteLineToPointDistSqr(NUVEC *start, NUVEC *end, NUVEC *point);
    f32 NuInfiniteLineToPointDistSqrEx(NUVEC *start, NUVEC *end, NUVEC *point, NUVEC *closest);
    f32 NuLineToLineDist(NUVEC *a, NUVEC *b, NUVEC *c, NUVEC *d);
#ifdef __cplusplus
}
#endif
