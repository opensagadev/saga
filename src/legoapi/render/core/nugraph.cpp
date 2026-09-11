#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/numemory.h"

static i32 curve_points;
static nuvec_s *curve;
static nuvec_s *control;

extern "C" void nugraphFreeTempCurveData();

f32 nugraph_blend(i32 interval, i32 degree, i32 *knots, f32 parameter);
void nugraph_compute_point(i32 *knots, i32 segment_count, i32 degree, f32 parameter, nuvec_s *control_points,
                           nuvec_s *point);
void nugraph_compute_catmull_point(i32 segment, f32 parameter, nuvec_s *control_points, nuvec_s *point);
void nugraph_compute_linear_point(i32 segment, f32 parameter, nuvec_s *control_points, nuvec_s *point);
void nugraph_compute_intervals(i32 *knots, i32 segment_count, i32 degree);
void nugraph_bspline(i32 segment_count, i32 degree, nuvec_s *control_points, nuvec_s *curve_points, i32 curve_count);
void nugraph_catmullrom(i32 segment_count, nuvec_s *control_points, nuvec_s *curve_points, i32 curve_count);
void nugraph_linear(i32 segment_count, nuvec_s *control_points, nuvec_s *curve_points, i32 curve_count);

extern "C" {

    i32 nugraphAddPoint(nugraph_s *graph, f32 x, f32 y) {
        i32 index;
        i32 insert_index = 0;
        if (graph == NULL) {
            return 0;
        }
        if (graph->point_count > 7) {
            return 0;
        }
        if (graph->point_count <= 1) {
            return 0;
        }

        for (index = 0; index < graph->point_count; ++index) {
            if (graph->x[index] == x) {
                return 0;
            }
            if (graph->x[index] > x) {
                insert_index = index;
                break;
            }
        }

        for (index = graph->point_count; index >= insert_index; --index) {
            graph->x[index] = graph->x[index - 1];
            graph->y[index] = graph->y[index - 1];
        }
        ++graph->point_count;
        graph->x[insert_index] = x;
        graph->y[insert_index] = y;
        return 1;
    }

    i32 nugraphCalcCurve(nugraph_s *graph, i32 point_count) {
        if (graph == NULL) {
            return 0;
        }

        nugraphFreeTempCurveData();
        if (curve == NULL) {
            curve = static_cast<nuvec_s *>(NU_ALLOC(100 * sizeof(nuvec_s), 4, 1, "", 0));
        }
        if (control == NULL) {
            control = static_cast<nuvec_s *>(NU_ALLOC(8 * sizeof(nuvec_s), 4, 1, "", 0));
        }
        if (curve == NULL || control == NULL) {
            return 0;
        }

        if (point_count > 100) {
            point_count = 100;
        }
        curve_points = point_count;

        for (i32 i = 0; i < graph->point_count; ++i) {
            control[i].x = graph->x[i];
            control[i].y = graph->y[i];
            control[i].z = 0.0f;
        }

        switch (graph->interpolation) {
            case 0:
                nugraph_linear(graph->point_count - 1, control, curve, curve_points);
                break;
            case 1:
                nugraph_catmullrom(graph->point_count - 1, control, curve, curve_points);
                break;
            case 2:
                nugraph_bspline(graph->point_count - 1, 3, control, curve, curve_points);
                break;
        }
        return 1;
    }

    i32 nugraphDeletePoint(nugraph_s *graph, i32 index) {
        if (graph == NULL) {
            return 0;
        }
        if (graph->point_count - 1 <= index) {
            return 0;
        }
        if (index <= 0) {
            return 0;
        }

        --graph->point_count;
        for (i32 i = index; i < graph->point_count; ++i) {
            graph->x[i] = graph->x[i + 1];
            graph->y[i] = graph->y[i + 1];
        }
        return 1;
    }

    void nugraphFileLoadGraph(void) {
    }

    void nugraphFileLoadGraphOnly(void) {
    }

    void nugraphFileLoadGraphOnlyToFile(void) {
    }

    void nugraphFileLoadTable(void) {
    }

    void nugraphFileSave(void) {
    }

    void nugraphFileSaveGraphOnly(void) {
    }

    void nugraphFileSaveGraphOnlyToFile(void) {
    }

    void nugraphFreeTempCurveData() {
        if (curve != NULL) {
            NU_FREE(curve);
        }
        curve = NULL;
        if (control != NULL) {
            NU_FREE(control);
        }
        control = NULL;
    }

    void nugraphGenerateLookupTable(void) {
    }

    f32 nugraphGetXatT(nugraph_s *, f32 t) {
        t *= (f32)curve_points;
        i32 index = (i32)(t + 0.5f);
        if (index >= curve_points) {
            index = curve_points - 1;
        }
        if (curve == NULL) {
            return 0.0f;
        }
        return curve[index].x;
    }

    f32 nugraphGetYatT(nugraph_s *, f32 t) {
        t *= (f32)curve_points;
        i32 index = (i32)(t + 0.5f);
        if (index >= curve_points) {
            index = curve_points - 1;
        }
        if (curve == NULL) {
            return 0.0f;
        }
        return curve[index].y;
    }

    void nugraphGetYatX(void) {
    }

    void nugraphGetYatXScaled(void) {
    }

    void nugraphInit(nugraph_s *graph) {
        graph->interpolation = 1;
        graph->point_count = 2;
        graph->x[0] = 0.0f;
        graph->y[0] = 0.0f;
        graph->x[1] = 1.0f;
        graph->y[1] = 1.0f;
        graph->x_scale = 1.0f;
        graph->y_scale = 1.0f;
        graph->x_extent = 10.0f;
        graph->y_extent = 10.0f;
    }

} // extern "C"

f32 nugraphGetXatIndex(nugraph_s *graph, i32 index) {
    if (graph == NULL) {
        return 0.0f;
    }
    if (index >= 0 && graph->point_count > index) {
        return graph->x[index];
    }
    if (index == -1) {
        return graph->x[0] + (graph->x[0] - graph->x[1]);
    }
    if (graph->point_count == index) {
        return graph->x[graph->point_count] +
               (graph->x[graph->point_count] - graph->x[graph->point_count - 1]);
    }
    return 0.0f;
}

f32 nugraphGetYatIndex(nugraph_s *graph, i32 index) {
    if (graph == NULL) {
        return 0.0f;
    }
    if (index >= 0 && graph->point_count > index) {
        return graph->y[index];
    }
    if (index == -1) {
        return graph->y[0] + (graph->y[0] - graph->y[1]);
    }
    if (graph->point_count == index) {
        return graph->y[graph->point_count] +
               (graph->y[graph->point_count] - graph->y[graph->point_count - 1]);
    }
    return 0.0f;
}

f32 nugraph_blend(i32 interval, i32 degree, i32 *knots, f32 parameter) {
    f32 result;
    if (degree == 1) {
        if ((f32)knots[interval] <= parameter && parameter < (f32)knots[interval + 1]) {
            result = 1.0f;
        } else {
            result = 0.0f;
        }
    } else {
        if (knots[interval + degree - 1] == knots[interval] &&
            knots[interval + degree] == knots[interval + 1]) {
            result = 0.0f;
        } else if (knots[interval + degree - 1] == knots[interval]) {
            f32 weight = ((f32)knots[interval + degree] - parameter) /
                         (f32)(knots[interval + degree] - knots[interval + 1]);
            result = weight * nugraph_blend(interval + 1, degree - 1, knots, parameter);
        } else if (knots[interval + degree] == knots[interval + 1]) {
            f32 weight = (parameter - (f32)knots[interval]) /
                         (f32)(knots[interval + degree - 1] - knots[interval]);
            result = weight * nugraph_blend(interval, degree - 1, knots, parameter);
        } else {
            f32 weight_a = (parameter - (f32)knots[interval]) /
                           (f32)(knots[interval + degree - 1] - knots[interval]);
            f32 blend_a = weight_a * nugraph_blend(interval, degree - 1, knots, parameter);
            f32 weight_b = ((f32)knots[interval + degree] - parameter) /
                           (f32)(knots[interval + degree] - knots[interval + 1]);
            result = blend_a + weight_b * nugraph_blend(interval + 1, degree - 1, knots, parameter);
        }
    }
    return result;
}

void nugraph_compute_point(i32 *knots, i32 segment_count, i32 degree, f32 parameter, nuvec_s *control_points,
                           nuvec_s *point) {
    point->x = 0.0f;
    point->y = 0.0f;
    point->z = 0.0f;
    for (i32 i = 0; i <= segment_count; ++i) {
        f32 weight = nugraph_blend(i, degree, knots, parameter);
        point->x += control_points[i].x * weight;
        point->y += control_points[i].y * weight;
    }
}

void nugraph_compute_catmull_point(i32 segment_count, f32 parameter, nuvec_s *control_points, nuvec_s *point) {
    if (segment_count <= 0) {
        return;
    }

    nuvec_s first;
    first.x = control_points[0].x + (control_points[0].x - control_points[1].x) * 0.1f;
    first.y = control_points[0].y + (control_points[0].y - control_points[1].y) * 0.1f;

    nuvec_s last;
    last.x = control_points[segment_count].x +
             (control_points[segment_count].x - control_points[segment_count - 1].x) * 0.1f;
    last.y = control_points[segment_count].y +
             (control_points[segment_count].y - control_points[segment_count - 1].y) * 0.1f;

    i32 segment = 1;
    nuvec_s *p0;
    nuvec_s *p3;
    f32 start;
    f32 fraction;
    nuvec_s *p1;
    nuvec_s *p2;
    for (; segment <= segment_count; ++segment) {
        if ((f32)segment / (f32)segment_count > parameter) {
            break;
        }
    }

    start = (f32)(segment - 1) / (f32)segment_count;
    fraction = parameter - start;
    fraction *= (f32)segment_count;

    p0 = segment == 1 ? &first : &control_points[segment - 2];
    p1 = &control_points[segment - 1];
    p2 = &control_points[segment];
    p3 = segment >= segment_count ? &last : &control_points[segment + 1];

    point->x = 0.5f *
               (2.0f * p1->x + (p2->x - p0->x) * fraction +
                (2.0f * p0->x - 5.0f * p1->x + 4.0f * p2->x - p3->x) * (fraction * fraction) +
                (3.0f * p1->x - p0->x - 3.0f * p2->x + p3->x) *
                    (fraction * fraction * fraction));
    point->y = 0.5f *
               (2.0f * p1->y + (p2->y - p0->y) * fraction +
                (2.0f * p0->y - 5.0f * p1->y + 4.0f * p2->y - p3->y) * (fraction * fraction) +
                (3.0f * p1->y - p0->y - 3.0f * p2->y + p3->y) *
                    (fraction * fraction * fraction));
    point->z = 0.0f;
}

void nugraph_compute_linear_point(i32 segment_count, f32 parameter, nuvec_s *control_points, nuvec_s *point) {
    i32 segment = 1;
    for (; segment <= segment_count; ++segment) {
        if ((f32)segment / (f32)segment_count > parameter) {
            break;
        }
    }

    f32 start = (f32)(segment - 1) / (f32)segment_count;
    f32 fraction = parameter - start;
    fraction *= (f32)segment_count;
    point->x = control_points[segment - 1].x +
               (control_points[segment].x - control_points[segment - 1].x) * fraction;
    point->y = control_points[segment - 1].y +
               (control_points[segment].y - control_points[segment - 1].y) * fraction;
    point->z = 0.0f;
}

void nugraph_compute_intervals(i32 *knots, i32 segment_count, i32 degree) {
    for (i32 i = 0; i <= segment_count + degree; ++i) {
        if (i < degree) {
            knots[i] = 0;
        } else if (degree <= i && i <= segment_count) {
            knots[i] = i - degree + 1;
        } else if (i > segment_count) {
            knots[i] = segment_count - degree + 2;
        }
    }
}

void nugraph_bspline(i32 segment_count, i32 degree, nuvec_s *control_points, nuvec_s *output, i32 output_count) {
    i32 knots[16];
    nugraph_compute_intervals(knots, segment_count, degree);
    f32 increment = (f32)(segment_count - degree + 2) / (f32)(output_count - 1);
    f32 parameter = 0.0f;
    for (i32 i = 0; i < output_count - 1; ++i) {
        nuvec_s point;
        nugraph_compute_point(knots, segment_count, degree, parameter, control_points, &point);
        output[i].x = point.x;
        output[i].y = point.y;
        output[i].z = point.z;
        parameter += increment;
    }
    output[output_count - 1].x = control_points[segment_count].x;
    output[output_count - 1].y = control_points[segment_count].y;
    output[output_count - 1].z = control_points[segment_count].z;
}

void nugraph_catmullrom(i32 segment_count, nuvec_s *control_points, nuvec_s *output, i32 output_count) {
    f32 increment = 1.0f / (f32)(output_count - 1);
    f32 parameter = 0.0f;
    for (i32 i = 0; i < output_count - 1; ++i) {
        nuvec_s point;
        nugraph_compute_catmull_point(segment_count, parameter, control_points, &point);
        output[i].x = point.x;
        output[i].y = point.y;
        output[i].z = point.z;
        parameter += increment;
    }
    output[output_count - 1].x = control_points[segment_count].x;
    output[output_count - 1].y = control_points[segment_count].y;
    output[output_count - 1].z = control_points[segment_count].z;
}

void nugraph_linear(i32 segment_count, nuvec_s *control_points, nuvec_s *output, i32 output_count) {
    f32 increment = 1.0f / (f32)(output_count - 1);
    f32 parameter = 0.0f;
    for (i32 i = 0; i < output_count - 1; ++i) {
        nuvec_s point;
        nugraph_compute_linear_point(segment_count, parameter, control_points, &point);
        output[i].x = point.x;
        output[i].y = point.y;
        output[i].z = point.z;
        parameter += increment;
    }
    output[output_count - 1].x = control_points[segment_count].x;
    output[output_count - 1].y = control_points[segment_count].y;
    output[output_count - 1].z = control_points[segment_count].z;
}
