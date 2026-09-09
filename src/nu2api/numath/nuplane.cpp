#include "decomp.h"
#include "nu2api/numath/nuplane.h"

#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"

i32 NuPlnPlnIntersect(NUPLANE *first, NUPLANE *second, NUVEC *point, NUVEC *direction) {
    f32 cross_y = first->c * second->a - first->a * second->c;
    f32 cross_z = first->a * second->b - first->b * second->a;
    NUVEC normal;
    normal.x = first->b * second->c - first->c * second->b;
    normal.y = cross_y;
    normal.z = cross_z;
    NuVecNorm(&normal, &normal);
    *direction = normal;
    normal.x = NuFabs(normal.x);
    normal.y = NuFabs(normal.y);
    normal.z = NuFabs(normal.z);
    if (normal.z >= normal.x && normal.z >= normal.y) {
        if (NuFabs(second->a) < 0.0001f) {
            point->y = (first->d * second->a / first->a - second->d) / (-second->a * first->b / first->a + second->b);
            point->x = (-first->d - first->b * point->y) / first->a;
            point->z = 0.0f;
        } else {
            point->y = (second->d * first->a / second->a - first->d) / (-first->a * second->b / second->a + first->b);
            point->x = (-second->d - second->b * point->y) / second->a;
            point->z = 0.0f;
        }
    } else if (normal.y >= normal.x && normal.y >= normal.z) {
        if (NuFabs(second->a) < 0.0001f) {
            point->z = (first->d * second->a / first->a - second->d) / (-second->a * first->c / first->a + second->c);
            point->x = (-first->d - first->c * point->z) / first->a;
            point->y = 0.0f;
        } else {
            point->z = (second->d * first->a / second->a - first->d) / (-first->a * second->c / second->a + first->c);
            point->x = (-second->d - second->c * point->z) / second->a;
            point->y = 0.0f;
        }
    } else {
        if (NuFabs(second->b) < 0.0001f) {
            point->z = (first->d * second->b / first->b - second->d) / (-second->b * first->c / first->b + second->c);
            point->y = (-first->d - first->c * point->z) / first->b;
            point->x = 0.0f;
        } else {
            point->z = (second->d * first->b / second->b - first->d) / (-first->b * second->c / second->b + first->c);
            point->y = (-second->d - second->c * point->z) / second->b;
            point->x = 0.0f;
        }
    }
    // The original evaluates this residual but returns zero on both paths.
    if (first->a * point->x + first->b * point->y + first->c * point->z + first->d > 0.0001f) {
        return 0;
    }
    return 0;
}

f32 NuInfiniteLineToPointDistSqrEx(NUVEC *start, NUVEC *end, NUVEC *point, NUVEC *closest) {
    f32 length;
    f32 projection;
    f32 distance;
    NUVEC direction;
    NUVEC relative;
    NUVEC projected;
    NUVEC delta;
    NuVecSub(&direction, end, start);
    length = NuVecMag(&direction);
    NuVecScale(&direction, &direction, 1.0f / length);
    NuVecSub(&relative, point, start);
    projection = NuVecDot(&direction, &relative);
    NuVecScale(&projected, &direction, projection);
    NuVecAdd(&projected, &projected, start);
    distance = NuVecDistSqr(&projected, point, &delta);
    if (closest != NULL)
        *closest = projected;
    return distance;
}

f32 NuInfiniteLineToPointDistSqr(NUVEC *start, NUVEC *end, NUVEC *point) {
    return NuInfiniteLineToPointDistSqrEx(start, end, point, NULL);
}

f32 NuLineToPointDistSqr(NUVEC *start, NUVEC *end, NUVEC *point) {
    return NuLineToPointDistSqrEx(start, end, point, NULL);
}

f32 NuLineToLineDist(NUVEC *a, NUVEC *b, NUVEC *c, NUVEC *d) {
    f32 distance;
    NUVEC offset;
    NUVEC first;
    NUVEC second;
    NUVEC between;
    NUVEC normal;
    NuVecSub(&offset, c, a);
    NuVecSub(&first, b, a);
    NuVecSub(&second, d, c);
    NuVecSub(&between, b, c);
    NuVecCross(&normal, &first, &second);
    distance = NuVecDot(&offset, &normal);
    distance /= NuVecMag(&normal);
    return distance;
}

f32 NuPlnDist(NUPLANE *plane, NUVEC *point) {
    f32 distance = NuVecDot(point, (NUVEC *)plane) + plane->d;
    return distance;
}

f32 NuPlnDist2(NUPLANE *plane, NUVEC *a, NUVEC *b) {
    f32 da = NuVecDot(a, (NUVEC *)plane) + plane->d;
    f32 db = NuVecDot(b, (NUVEC *)plane) + plane->d;
    if (da < 0.0f && db < 0.0f)
        return da > db ? da : db;
    if (da > 0.0f && db > 0.0f)
        return da < db ? da : db;
    return 0.0f;
}

i32 NuPlnLineVU0(NUPLANE *plane, NUVEC *start, NUVEC *end, NUVEC *out) {
    return NuPlnLine(plane, start, end, out);
}

i32 NuPlnLine2(NUPLANE *plane, NUVEC *a, NUVEC *b, NUVEC *c, NUVEC *start, NUVEC *end, NUVEC *out, f32 *distance,
               f32 *fraction) {
    if (NuPlnLine(plane, start, end, out)) {
        if (NuPtInPoly(out, a, b, c, plane)) {
            if (distance != NULL || fraction != NULL) {
                f32 hit_distance;
                f32 line_length;
                NUVEC delta;
                NUVEC line;
                NuVecSub(&delta, out, start);
                hit_distance = NuVecMag(&delta);
                if (distance != NULL)
                    *distance = hit_distance;
                if (fraction != NULL) {
                    NuVecSub(&line, end, start);
                    line_length = NuVecMag(&line);
                    *fraction = hit_distance / line_length;
                }
            }
            return 1;
        }
    }
    return 0;
}

void NuPlnEqnPn(NUPLANE *out, NUVEC *point, NUVEC *normal) {
    out->a = normal->x;
    out->b = normal->y;
    out->c = normal->z;
    out->d = -(out->a * point->x + out->b * point->y + out->c * point->z);
}

void NuPlnEqn(NUPLANE *out, NUVEC *pnt0, NUVEC *pnt1, NUVEC *pnt2) {
    NUVEC *pln;
    NUVEC v1_minus_0;
    NUVEC v2_minus_0;
    NUVEC normal;

    pln = (NUVEC *)out;

    NuVecSub(&v1_minus_0, pnt1, pnt0);
    NuVecSub(&v2_minus_0, pnt2, pnt0);

    NuVecCross(&normal, &v1_minus_0, &v2_minus_0);

    NuVecNorm(pln, &normal);

    out->d = -(out->a * pnt0->x + out->b * pnt0->y + out->c * pnt0->z);
}

i32 NuPlnLine(NUPLANE *plane, NUVEC *s, NUVEC *e, NUVEC *out) {
    f32 dot_sp;
    f32 dot_ep;
    NUVEC dir;
    f32 inv_magnitude;
    i32 is_acute;

    dot_sp = NuVecDot(s, (NUVEC *)plane) + plane->d;
    dot_ep = NuVecDot(e, (NUVEC *)plane) + plane->d;

    if (NuFsign(dot_sp) != NuFsign(dot_ep)) {
        NuVecSub(&dir, e, s);

        inv_magnitude = -dot_sp / (dot_ep - dot_sp);
        NuVecScale(out, &dir, inv_magnitude);

        NuVecAdd(out, out, s);

        return 1;
    }

    return 0;
}

f32 NuPlnLine3(NUPLANE *plane, NUVEC *pnt, NUVEC *v, NUVEC *out) {
    NUVEC *pln;
    f32 dist;
    f32 dot;

    NuVecNorm(v, v);

    // It's not entirely clear why the plane is stored in a separate variable,
    // or that this cast is what originally took place.
    pln = (NUVEC *)plane;
    dot = v->x * pln->x + v->y * pln->y + v->z * pln->z;

    dist = ((plane->a * -plane->d - pnt->x) * pln->x + (plane->b * -plane->d - pnt->y) * pln->y +
            (plane->c * -plane->d - pnt->z) * pln->z) /
           dot;

    out->x = pnt->x + v->x * dist;
    out->y = pnt->y + v->y * dist;
    out->z = pnt->z + v->z * dist;

    return dist;
}

i32 NuPtInPoly(NUVEC *pnt, NUVEC *t0, NUVEC *t1, NUVEC *t2, NUPLANE *plane) {
    f32 abs_a;
    f32 abs_b;
    f32 abs_c;
    i32 result;

    abs_a = NuFabs(plane->a);
    abs_b = NuFabs(plane->b);
    abs_c = NuFabs(plane->c);

    if (abs_a > abs_b) {
        if (abs_a > abs_c) {
            if (plane->a <= 0.0f) {
                result = NuPtInPolyYZ(pnt, t0, t1, t2);
            } else {
                result = NuPtInPolyZY(pnt, t0, t1, t2);
            }
        } else {
            if (plane->c <= 0.0f) {
                result = NuPtInPolyXY(pnt, t0, t1, t2);
            } else {
                result = NuPtInPolyYX(pnt, t0, t1, t2);
            }
        }
    } else {
        if (abs_b > abs_c) {
            if (plane->b <= 0.0f) {
                result = NuPtInPolyZX(pnt, t0, t1, t2);
            } else {
                result = NuPtInPolyXZ(pnt, t0, t1, t2);
            }
        } else {
            if (plane->c <= 0.0f) {
                result = NuPtInPolyXY(pnt, t0, t1, t2);
            } else {
                result = NuPtInPolyYX(pnt, t0, t1, t2);
            }
        }
    }

    return result;
}

i32 NuPtInPolyXY(NUVEC *pnt, NUVEC *t0, NUVEC *t1, NUVEC *t2) {
    if ((t1->x - t0->x) * (pnt->y - t1->y) - (pnt->x - t1->x) * (t1->y - t0->y) <= 0.0f &&
        (t2->x - t1->x) * (pnt->y - t2->y) - (pnt->x - t2->x) * (t2->y - t1->y) <= 0.0f &&
        (t0->x - t2->x) * (pnt->y - t0->y) - (pnt->x - t0->x) * (t0->y - t2->y) <= 0.0f) {
        if ((t0->x != t1->x || t0->y != t1->y) && (t0->x != t2->x || t0->y != t2->y) &&
            (t1->x != t2->x || t1->y != t2->y)) {
            return 1;
        }
    }

    return 0;
}

i32 NuPtInPolyYX(NUVEC *pnt, NUVEC *t0, NUVEC *t1, NUVEC *t2) {
    if ((t1->y - t0->y) * (pnt->x - t1->x) - (pnt->y - t1->y) * (t1->x - t0->x) <= 0.0f &&
        (t2->y - t1->y) * (pnt->x - t2->x) - (pnt->y - t2->y) * (t2->x - t1->x) <= 0.0f &&
        (t0->y - t2->y) * (pnt->x - t0->x) - (pnt->y - t0->y) * (t0->x - t2->x) <= 0.0f) {
        if ((t0->y != t1->y || t0->x != t1->x) && (t0->y != t2->y || t0->x != t2->x) &&
            (t1->y != t2->y || t1->x != t2->x)) {
            return 1;
        }
    }

    return 0;
}

i32 NuPtInPolyXZ(NUVEC *pnt, NUVEC *t0, NUVEC *t1, NUVEC *t2) {
    if ((t1->x - t0->x) * (pnt->z - t1->z) - (pnt->x - t1->x) * (t1->z - t0->z) <= 0.0f &&
        (t2->x - t1->x) * (pnt->z - t2->z) - (pnt->x - t2->x) * (t2->z - t1->z) <= 0.0f &&
        (t0->x - t2->x) * (pnt->z - t0->z) - (pnt->x - t0->x) * (t0->z - t2->z) <= 0.0f) {
        if ((t0->x != t1->x || t0->z != t1->z) && (t0->x != t2->x || t0->z != t2->z) &&
            (t1->x != t2->x || t1->z != t2->z)) {
            return 1;
        }
    }

    return 0;
}

i32 NuPtInPolyZX(NUVEC *pnt, NUVEC *t0, NUVEC *t1, NUVEC *t2) {
    if ((t1->z - t0->z) * (pnt->x - t1->x) - (pnt->z - t1->z) * (t1->x - t0->x) <= 0.0f &&
        (t2->z - t1->z) * (pnt->x - t2->x) - (pnt->z - t2->z) * (t2->x - t1->x) <= 0.0f &&
        (t0->z - t2->z) * (pnt->x - t0->x) - (pnt->z - t0->z) * (t0->x - t2->x) <= 0.0f) {
        if ((t0->z != t1->z || t0->x != t1->x) && (t0->z != t2->z || t0->x != t2->x) &&
            (t1->z != t2->z || t1->x != t2->x)) {
            return 1;
        }
    }

    return 0;
}

i32 NuPtInPolyYZ(NUVEC *pnt, NUVEC *t0, NUVEC *t1, NUVEC *t2) {
    if ((t1->y - t0->y) * (pnt->z - t1->z) - (pnt->y - t1->y) * (t1->z - t0->z) <= 0.0f &&
        (t2->y - t1->y) * (pnt->z - t2->z) - (pnt->y - t2->y) * (t2->z - t1->z) <= 0.0f &&
        (t0->y - t2->y) * (pnt->z - t0->z) - (pnt->y - t0->y) * (t0->z - t2->z) <= 0.0f) {
        if ((t0->y != t1->y || t0->z != t1->z) && (t0->y != t2->y || t0->z != t2->z) &&
            (t1->y != t2->y || t1->z != t2->z)) {
            return 1;
        }
    }

    return 0;
}

i32 NuPtInPolyZY(NUVEC *pnt, NUVEC *t0, NUVEC *t1, NUVEC *t2) {
    if ((t1->z - t0->z) * (pnt->y - t1->y) - (pnt->z - t1->z) * (t1->y - t0->y) <= 0.0f &&
        (t2->z - t1->z) * (pnt->y - t2->y) - (pnt->z - t2->z) * (t2->y - t1->y) <= 0.0f &&
        (t0->z - t2->z) * (pnt->y - t0->y) - (pnt->z - t0->z) * (t0->y - t2->y) <= 0.0f) {
        if ((t0->z != t1->z || t0->y != t1->y) && (t0->z != t2->z || t0->y != t2->y) &&
            (t1->z != t2->z || t1->y != t2->y)) {
            return 1;
        }
    }

    return 0;
}

f32 NuLineToPointDistSqrEx(NUVEC *s, NUVEC *e, NUVEC *pnt, NUVEC *out) {
    NUVEC s_to_e;
    f32 len;
    NUVEC pnt_to_s;
    f32 dot;
    NUVEC nearest;
    f32 dist;
    NUVEC dir;

    NuVecSub(&s_to_e, e, s);
    len = NuVecMag(&s_to_e);

    NuVecScale(&s_to_e, &s_to_e, 1.0f / len);

    NuVecSub(&pnt_to_s, pnt, s);
    dot = NuVecDot(&s_to_e, &pnt_to_s);

    if (dot <= 0.0f) {
        nearest = *s;
    } else if (dot >= len) {
        nearest = *e;
    } else {
        NuVecScale(&nearest, &s_to_e, dot);
        NuVecAdd(&nearest, &nearest, s);
    }

    dist = NuVecDistSqr(&nearest, pnt, &dir);

    if (out != NULL) {
        *out = nearest;
    }

    return dist;
}

static __used__ bool OnOrOutsidePlane(nuvec_s *, nuvec_s *, nuvec_s *) {
    return false;
}
