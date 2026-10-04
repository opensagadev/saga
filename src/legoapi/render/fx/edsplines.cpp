#include "decomp.h"
#include "globals.h"
#include "gameapi/edtools/edfile.h"
#include "legoapi/characters/motion.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/world/world.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/podrace.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"

#include <string.h>
#include <stdio.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

// The original translation unit exports this internal counter under its unmangled C name.
static i32 bezierline_depth asm("bezierline_depth");

extern "C" {
    char *FSP_Extension = ".FSP";
    extern f32 PODRACE_SPLINEINC;
}

f32 BezierLineLength(VuVec &, VuVec &, VuVec &, VuVec &);

i32 BezierLinePos(VuVec &result, VuVec &start, VuVec &first_control, VuVec &end, VuVec &second_control, f32 distance) {
    f32 step = 0.25f;
    f32 t = 0.5f;
    for (i32 count = 256; count != 0; --count) {
        f32 complement = 1.0f - t;
        VuVec middle{second_control.x * complement + first_control.x * t,
                     second_control.y * complement + first_control.y * t,
                     second_control.z * complement + first_control.z * t, 0.0f};
        VuVec first{first_control.x * complement + start.x * t, first_control.y * complement + start.y * t,
                    first_control.z * complement + start.z * t, 0.0f};
        VuVec last{end.x * complement + second_control.x * t, end.y * complement + second_control.y * t,
                   end.z * complement + second_control.z * t, 0.0f};
        VuVec first_middle{middle.x * complement + first.x * t, middle.y * complement + first.y * t,
                           middle.z * complement + first.z * t, 0.0f};
        VuVec middle_last{last.x * complement + middle.x * t, last.y * complement + middle.y * t,
                          last.z * complement + middle.z * t, 0.0f};
        VuVec point{middle_last.x * complement + first_middle.x * t, middle_last.y * complement + first_middle.y * t,
                    middle_last.z * complement + first_middle.z * t, 1.0f};
        f32 length = BezierLineLength(start, first, point, first_middle);
        f32 difference = distance - length;
        if (difference < 0.0f)
            difference = -difference;
        if (!(difference > 0.01f)) {
            result = point;
            return 1;
        }
        if (distance < length)
            t += step;
        else
            t -= step;
        step *= 0.5f;
        if (count == 1) {
            result = point;
            return 1;
        }
    }
    return 1;
}

void BezierLineEval(VuVec &result, VuVec &start, VuVec &first_control, VuVec &end, VuVec &second_control, float along) {
    const f32 complement = 1.0f - along;
    const f32 complement_squared = complement * complement;
    const f32 squared = along * along;
    const f32 start_weight = complement_squared * complement;
    const f32 first_weight = (along * 3.0f) * complement_squared;
    const f32 second_weight = (3.0f * squared) * complement;
    const f32 end_weight = along * squared;
    const VuVec point{((first_control.x * first_weight + start.x * start_weight) + second_control.x * second_weight) +
                          end.x * end_weight,
                      ((first_control.y * first_weight + start.y * start_weight) + second_control.y * second_weight) +
                          end.y * end_weight,
                      ((first_control.z * first_weight + start.z * start_weight) + second_control.z * second_weight) +
                          end.z * end_weight,
                      0.0f};
    result = point;
}

void CalcSplinePoint(flightspline_s *spline, _vuv_s *result, float along) {
    const f32 scaled = (spline->point_count - 1) * along;
    i32 index = static_cast<i32>(scaled);
    const f32 fraction = scaled - index;
    // The endpoint vectors occupy aligned stack slots in the retail evaluator.
    _vuv_s first __attribute__((aligned(16)));
    _vuv_s last __attribute__((aligned(16)));
    NUVEC_ALIGNED16 direction;
    _vuv_s *previous;
    if (!(index <= 0)) {
        // Retail retains the pre-clamp fraction and permits index == count;
        // callers must supply the corresponding neighboring point storage.
        if (index > spline->point_count)
            index = spline->point_count;
        previous = &spline->points[index - 1];
    } else {
        direction.x = spline->points[0].x - spline->points[1].x;
        direction.y = spline->points[0].y - spline->points[1].y;
        direction.z = spline->points[0].z - spline->points[1].z;
        NuVecNorm(&direction, &direction);
        direction.x *= 10.0f;
        direction.y *= 10.0f;
        direction.z *= 10.0f;
        first.x = spline->points[0].x + direction.x;
        first.y = spline->points[0].y + direction.y;
        first.z = spline->points[0].z + direction.z;
        first.w = spline->points[0].w;
        previous = &first;
        index = 0;
    }

    _vuv_s *current = &spline->points[index];
    _vuv_s *next = &spline->points[index + 1];
    _vuv_s *following;
    if (!(index < spline->point_count - 2)) {
        const i32 end = spline->point_count - 1;
        direction.x = spline->points[end].x - spline->points[end - 1].x;
        direction.y = spline->points[end].y - spline->points[end - 1].y;
        direction.z = spline->points[end].z - spline->points[end - 1].z;
        NuVecNorm(&direction, &direction);
        direction.x *= 10.0f;
        direction.y *= 10.0f;
        direction.z *= 10.0f;
        _vuv_s *end_point = &spline->points[spline->point_count - 1];
        last.x = end_point->x + direction.x;
        last.y = end_point->y + direction.y;
        last.z = end_point->z + direction.z;
        last.w = end_point->w;
        following = &last;
    } else {
        following = &spline->points[index + 2];
    }

    const _vuv_s after = *following;
    const _vuv_s before = *previous;
    const f32 squared = fraction * fraction;
    const f32 cubed = fraction * squared;
    result->x = (((current->x - next->x) * 3.0f - before.x + after.x) * cubed +
                 ((before.x * 2.0f - current->x * 5.0f + next->x * 4.0f - after.x) * squared +
                  (current->x * 2.0f + (next->x - before.x) * fraction))) *
                0.5f;
    result->y = (((current->y - next->y) * 3.0f - before.y + after.y) * cubed +
                 ((before.y * 2.0f - current->y * 5.0f + next->y * 4.0f - after.y) * squared +
                  (current->y * 2.0f + (next->y - before.y) * fraction))) *
                0.5f;
    result->z = (((current->z - next->z) * 3.0f - before.z + after.z) * cubed +
                 ((before.z * 2.0f - current->z * 5.0f + next->z * 4.0f - after.z) * squared +
                  (current->z * 2.0f + (next->z - before.z) * fraction))) *
                0.5f;
    result->w = current->w + (next->w - current->w) * fraction;
}

f32 BezierLineLength(VuVec &start, VuVec &first_control, VuVec &end, VuVec &second_control) {
    VuVec first{(start.x + first_control.x) * 0.5f, (start.y + first_control.y) * 0.5f,
                (start.z + first_control.z) * 0.5f, 0.0f};
    VuVec middle{(first_control.x + second_control.x) * 0.5f, (first_control.y + second_control.y) * 0.5f,
                 (first_control.z + second_control.z) * 0.5f, 0.0f};
    VuVec last{(second_control.x + end.x) * 0.5f, (second_control.y + end.y) * 0.5f, (second_control.z + end.z) * 0.5f,
               0.0f};
    VuVec first_middle{(first.x + middle.x) * 0.5f, (first.y + middle.y) * 0.5f, (first.z + middle.z) * 0.5f, 0.0f};
    VuVec middle_last{(middle.x + last.x) * 0.5f, (middle.y + last.y) * 0.5f, (middle.z + last.z) * 0.5f, 0.0f};
    VuVec midpoint{(first_middle.x + middle_last.x) * 0.5f, (first_middle.y + middle_last.y) * 0.5f,
                   (first_middle.z + middle_last.z) * 0.5f, 0.0f};
    NUVEC error{(start.x + end.x) * 0.5f - midpoint.x, (start.y + end.y) * 0.5f - midpoint.y,
                (start.z + end.z) * 0.5f - midpoint.z};
    if (NuVecMag(&error) >= 0.01f && bezierline_depth < 2) {
        ++bezierline_depth;
        f32 first_length = BezierLineLength(start, first, midpoint, first_middle);
        f32 second_length = BezierLineLength(midpoint, middle_last, end, last);
        --bezierline_depth;
        return first_length + second_length;
    }
    NUVEC chord{start.x - end.x, start.y - end.y, start.z - end.z};
    return NuVecMag(&chord);
}

f32 BezierLineLength(VuVec &start, VuVec &first_control, VuVec &end, VuVec &second_control, f32 t) {
    f32 complement = 1.0f - t;
    VuVec middle{second_control.x * complement + first_control.x * t,
                 second_control.y * complement + first_control.y * t,
                 second_control.z * complement + first_control.z * t, 0.0f};
    VuVec first{first_control.x * complement + start.x * t, first_control.y * complement + start.y * t,
                first_control.z * complement + start.z * t, 0.0f};
    VuVec last{end.x * complement + second_control.x * t, end.y * complement + second_control.y * t,
               end.z * complement + second_control.z * t, 0.0f};
    VuVec first_middle{middle.x * complement + first.x * t, middle.y * complement + first.y * t,
                       middle.z * complement + first.z * t, 0.0f};
    VuVec middle_last{last.x * complement + middle.x * t, last.y * complement + middle.y * t,
                      last.z * complement + middle.z * t, 0.0f};
    VuVec point{middle_last.x * complement + first_middle.x * t, middle_last.y * complement + first_middle.y * t,
                middle_last.z * complement + first_middle.z * t, 0.0f};
    return BezierLineLength(start, first, point, first_middle);
}

static void SplinePointAngles(NUGSPLINE *spline, i32 index, i32 looping, u16 *pitch, u16 *angle) {
    NUVEC *current = &spline->pts[index];
    i32 previous_index = index - 1;
    NUVEC *previous = NULL;
    if (looping && previous_index < 0)
        previous = &spline->pts[spline->length - 1];
    else if (previous_index >= 0)
        previous = &spline->pts[previous_index];
    NUVEC direction = {0.0f, 0.0f, 0.0f};
    if (previous != NULL) {
        direction.x += current->x - previous->x;
        direction.y += current->y - previous->y;
        direction.z += current->z - previous->z;
    }
    NUVEC *next = NULL;
    if (index + 1 < spline->length)
        next = &spline->pts[index + 1];
    else if (looping)
        next = spline->pts;
    if (next != NULL) {
        direction.x += next->x - current->x;
        direction.y += next->y - current->y;
        direction.z += next->z - current->z;
    }
    if (pitch != NULL)
        *pitch = NuAtan2D(direction.y, NuFsqrt(direction.x * direction.x + direction.z * direction.z));
    if (angle != NULL)
        *angle = NuAtan2D(direction.x, direction.z);
}

void PointAlongSpline(NUGSPLINE *spline, f32 along, NUVEC *position, u16 *angle, u16 *pitch, i32 looping) {
    if (angle != NULL)
        *angle = 0;
    if (pitch != NULL)
        *pitch = 0;
    if (spline == NULL)
        return;
    if (along > 1.0f)
        along = 1.0f;
    else if (along < 0.0f)
        along = 0.0f;
    u32 extent = (u32)(looping ? spline->length : spline->length - 1) << 16;
    u32 fixed = (i32)((f32)extent * along);
    i32 index = fixed >> 16;
    NUVEC *current = &spline->pts[index];
    *position = *current;
    if (pitch != NULL || angle != NULL)
        SplinePointAngles(spline, index, looping, pitch, angle);
    index++;
    if (index >= spline->length) {
        if (!looping)
            return;
        index = 0;
    }
    fixed &= 0xffff;
    if (fixed == 0)
        return;
    f32 fraction = fixed * (1.0f / 65536.0f);
    NUVEC *next = &spline->pts[index];
    position->x += (next->x - current->x) * fraction;
    position->y += (next->y - current->y) * fraction;
    position->z += (next->z - current->z) * fraction;
    if (angle != NULL || pitch != NULL) {
        u16 next_angle, next_pitch;
        SplinePointAngles(spline, index, looping, pitch != NULL ? &next_pitch : NULL,
                          angle != NULL ? &next_angle : NULL);
        if (angle != NULL)
            *angle += (i32)(RotDiff(*angle, next_angle) * fraction);
        if (pitch != NULL)
            *pitch += (i32)(RotDiff(*pitch, next_pitch) * fraction);
    }
}

void FlightSpline_Init(WORLDINFO_s *world, flightspline_s *splines, i32 capacity) {
    char filename[256];
    sprintf(filename, "%s%s", world->config_file, FSP_Extension);
    EdFileSetMedia(1);
    if (!EdFileOpen(filename, NUFILE_READ))
        return;

    const i32 version = EdFileReadInt();
    const i32 count = EdFileReadInt();
    // The file is trusted to fit the supplied storage, as in the retail loader.
    for (i32 i = 0; i < count; ++i) {
        flightspline_s *spline = &splines[i];
        spline->point_count = EdFileReadInt();
        spline->field_0x408 = EdFileReadFloat();
        spline->id = EdFileReadInt();
        spline->unknown_528 = 1;
        if (version > 1) {
            spline->field_0x40c = EdFileReadFloat();
            spline->field_0x514 = EdFileReadInt();
        } else {
            spline->field_0x40c = 0.0f;
            spline->field_0x514 = 0;
        }
        if (version > 2) {
            spline->field_0x51c = EdFileReadInt();
            spline->field_0x520 = EdFileReadInt();
        } else {
            spline->field_0x51c = -1;
            spline->field_0x520 = i;
        }
        for (i32 point = 0; point < spline->point_count; ++point) {
            spline->points[point].x = EdFileReadFloat();
            spline->points[point].y = EdFileReadFloat();
            spline->points[point].z = EdFileReadFloat();
            spline->points[point].w = EdFileReadFloat();
        }
    }

    i32 i;
    if (version > 3) {
        for (i = 0; i < count; ++i) {
            flightspline_s *spline = &splines[i];
            spline->length = EdFileReadFloat();
            if (version == 4) {
                f32 distance = 0.0f;
                _vuv_s previous, current;
                NUVEC difference;
                for (i32 point = 0; point < spline->point_count; ++point) {
                    CalcSplinePoint(spline, &previous, static_cast<f32>(point) / spline->point_count);
                    for (i32 sample = 1; sample <= 10; ++sample) {
                        CalcSplinePoint(spline, &current, (sample / 10.0f + point) / spline->point_count);
                        difference.x = current.x - previous.x;
                        difference.y = current.y - previous.y;
                        difference.z = current.z - previous.z;
                        distance += NuVecMag(&difference);
                        previous = current;
                    }
                    spline->cumulative_distances[point] = distance;
                }
                spline->length = distance;
            } else {
                for (i32 point = 0; point < spline->point_count; ++point)
                    spline->cumulative_distances[point] = EdFileReadFloat();
            }
        }
    } else {
        for (i = 0; i < count; ++i) {
            flightspline_s *spline = &splines[i];
            f32 distance = 0.0f;
            if (spline->point_count != 0) {
                _vuv_s current;
                CalcSplinePoint(spline, &current, 1.0f);
                f32 along = 1.0f;
                do {
                    const _vuv_s previous = current;
                    if (PODRACE_ADATA != NULL && PODRACE_ADATA == WORLD->area)
                        along -= PODRACE_SPLINEINC;
                    else
                        along -= 0.01f;
                    if (along < 0.0f)
                        along = 0.0f;
                    CalcSplinePoint(spline, &current, along);
                    NUVEC difference{current.x - previous.x, current.y - previous.y, current.z - previous.z};
                    distance += NuVecMag(&difference);
                } while (along > 0.0f);
            }
            spline->length = distance;
        }
    }
    for (; i < capacity; ++i)
        splines[i].point_count = 0;
    EdFileClose();
}

i32 LineIntersectXY(NUVEC *, NUVEC *, NUVEC *, NUVEC *, NUVEC *, NUVEC *);

i32 OutSideSplineArea(nuvec_s *position, nugspline_s *spline, nuvec_s *edge_end, nuvec_s *edge_start, i32 inside) {
    if (spline == NULL || position == NULL)
        return 0;
    NUVEC ray_start = {position->x, -position->z, position->y};
    NUVEC ray_end = {position->x, 100000.0f, position->y};
    NUVEC end = spline->pts[0];
    f32 end_y = end.y;
    end.y = -end.z;
    end.z = end_y;
    i32 intersections = 0;
    for (i32 i = 1; i < spline->length; ++i) {
        NUVEC start = end;
        end = spline->pts[i];
        end_y = end.y;
        end.y = -end.z;
        end.z = end_y;
        intersections += LineIntersectXY(&ray_start, &ray_end, &start, &end, NULL, NULL);
    }
    if ((intersections & 1) != 0 ? inside == 0 : inside != 0)
        return 0;
    i32 closest_index = 0;
    NUVEC closest = spline->pts[0];
    f32 best = 1000000000.0f;
    for (i32 i = 0; i < spline->length - 1; ++i) {
        f32 dx = spline->pts[i].x - position->x;
        f32 dz = spline->pts[i].z - position->z;
        f32 distance = dx * dx + dz * dz;
        if (distance < best) {
            closest_index = i;
            closest = spline->pts[i];
            best = distance;
        }
    }
    i32 previous = closest_index == 0 ? spline->length - 2 : closest_index - 1;
    i32 next = closest_index == spline->length - 2 ? 0 : closest_index + 1;
    NUVEC before = spline->pts[previous];
    NUVEC after = spline->pts[next];
    f32 ax = after.x - position->x;
    f32 az = after.z - position->z;
    f32 bx = before.x - position->x;
    f32 bz = before.z - position->z;
    if (bx * bx + bz * bz > ax * ax + az * az) {
        if (edge_end != NULL)
            *edge_end = after;
        if (edge_start != NULL)
            *edge_start = closest;
    } else {
        if (edge_end != NULL)
            *edge_end = closest;
        if (edge_start != NULL)
            *edge_start = before;
    }
    return 1;
}

void InitSplinePosition(SPLINEPOS_s *position, nugspline_s *spline, float distance, i32 looping) {
    if (position == NULL) {
        return;
    }

    SPLINEPOS_s *runtime = position;
    memset(runtime, 0, sizeof(*runtime));
    if (spline == NULL || spline->length < 2) {
        return;
    }

    runtime->spline = spline;
    runtime->looping = static_cast<u8>(looping);
    const NUVEC *first = reinterpret_cast<const NUVEC *>(reinterpret_cast<const u8 *>(spline->pts));
    const NUVEC *second = reinterpret_cast<const NUVEC *>(reinterpret_cast<const u8 *>(spline->pts) + spline->pt_size);
    runtime->segment_length = NuVecDist(const_cast<NUVEC *>(second), const_cast<NUVEC *>(first), NULL);
    if (distance > 0.0f) {
        MoveSplinePosition(position, distance);
    } else {
        runtime->position = *first;
        runtime->along = 0.0f;
    }
}

void GetNearestSplinePos(NUVEC *origin, SPLINEPOS_s *result, NUGSPLINE *spline, i32 looping, i16 first_point,
                         i16 last_point) {
    if (result == NULL)
        return;
    memset(result, 0, sizeof(*result));
    if (!(spline == NULL || origin == NULL || spline->length <= 1)) {
        result->spline = spline;
        result->looping = (i8)looping;
        i32 point_count = spline->length;
        i32 logical_count = result->looping != 0 ? point_count + 1 : point_count;
        i32 index = first_point;
        if (index < 0)
            index = 0;
        else if (index >= logical_count)
            return;
        i32 end = point_count;
        if (last_point >= 0 && last_point < end)
            end = last_point;
        NUVEC *point = (NUVEC *)((u8 *)spline->pts + index * (i16)spline->pt_size);
        f32 nearest = 1000000000.0f;
        NUVEC offset;
        do {
            f32 distance = NuVecDistSqr(origin, point, &offset);
            if (distance < nearest) {
                nearest = distance;
                result->segment = index;
            }
            point = (NUVEC *)((u8 *)point + (i16)result->spline->pt_size);
            index++;
        } while (index < end);
        spline = result->spline;
        i32 stride = (i16)spline->pt_size;
        NUVEC *current = (NUVEC *)((u8 *)spline->pts + result->segment * stride);
        NUVEC *next = (NUVEC *)((u8 *)spline->pts + ((result->segment + 1) % spline->length) * stride);
        result->segment_distance = 0.0f;
        result->segment_length = NuVecDist(next, current, &offset);
        result->position = *current;
        result->along = (result->segment_distance / result->segment_length + result->segment) / (logical_count - 1);
    }
}

void CalcSplinePointFromDist(flightspline_s *spline, _vuv_s *result, float distance) {
    if (distance >= spline->length) {
        distance = 1.0f;
    } else {
        for (i32 index = 0; index < spline->point_count; ++index) {
            const f32 end = spline->cumulative_distances[index];
            if (end > distance) {
                const f32 start = index == 0 ? 0.0f : spline->cumulative_distances[index - 1];
                distance = ((distance - start) / (end - start) + index) / spline->point_count;
                break;
            }
        }
    }
    CalcSplinePoint(spline, result, distance);
}

LEVELSPLINE *LevSplList;
static i32 levspl_i_start = -1;
static i32 levspl_i_startcam = -1;

void LevelSplines_InitForGame(LEVELSPLINE *splines) {
    LevSplList = splines;
    LEVELSPLINECOUNT = 0;

    if (splines == NULL) {
        return;
    }

    for (LEVELSPLINE *spline = splines; spline->name != NULL; ++spline) {
        if (levspl_i_start == -1 && NuStrICmp(spline->name, "start") == 0) {
            levspl_i_start = LEVELSPLINECOUNT;
        }
        if (levspl_i_startcam == -1 && NuStrICmp(spline->name, "start_cam") == 0) {
            levspl_i_startcam = LEVELSPLINECOUNT;
        }
        ++LEVELSPLINECOUNT;
    }
}

void EvaluateSplineXZIntersection(nugspline_s *first, i32 first_looping, SPLINEPOS_s *first_position,
                                  nugspline_s *second, i32 second_looping, SPLINEPOS_s *second_position) {
    memset(first_position, 0, sizeof(*first_position));
    memset(second_position, 0, sizeof(*second_position));
    first_position->spline = first;
    first_position->looping = static_cast<i8>(first_looping);
    second_position->spline = second;
    second_position->looping = static_cast<i8>(second_looping);
    if (!(second == NULL || first == NULL || first->length == 0 || second->length == 0)) {
        const i32 first_count = first_looping != 0 ? first->length + 1 : first->length;
        const i32 second_count = second_looping != 0 ? second->length + 1 : second->length;
        f32 closest = 1000000000.0f;
        f32 first_fraction, second_fraction;
        for (i32 i = 0; i < first_count - 1; ++i) {
            const i32 first_segment = i % first->length;
            NUVEC *first_start = &first->pts[first_segment];
            NUVEC *first_end = &first->pts[(i + 1) % first->length];
            // Retail includes the wraparound segment of the second spline even
            // without looping, and only stops this inner scan at an intersection.
            for (i32 j = 0; j < second_count; ++j) {
                const i32 second_segment = j % second->length;
                const f32 distance =
                    XZLinesClosest(first_start, first_end, &second->pts[second_segment],
                                   &second->pts[(j + 1) % second->length], &first_fraction, &second_fraction);
                if (distance < closest) {
                    first_position->segment_distance = first_fraction;
                    first_position->segment = static_cast<i16>(first_segment);
                    second_position->segment_distance = second_fraction;
                    second_position->segment = static_cast<i16>(second_segment);
                    closest = distance;
                    if (distance == 0.0f)
                        break;
                }
            }
        }

        NUVEC direction;
        first_position->segment_length = NuVecDist(&first->pts[(first_position->segment + 1) % first->length],
                                                   &first->pts[first_position->segment], &direction);
        first_position->segment_distance *= first_position->segment_length;
        MoveSplinePosition(first_position, 0.00001f);
        second_position->segment_length = NuVecDist(&second->pts[(second_position->segment + 1) % second->length],
                                                    &second->pts[second_position->segment], &direction);
        second_position->segment_distance *= second_position->segment_length;
        MoveSplinePosition(second_position, 0.00001f);
    }
}

void LevelSplines_InitForLevel(WORLDINFO_s *world) {
    world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
    world->portal_places = reinterpret_cast<PORTALPOS **>(world->giz_buffer.void_ptr);
    world->giz_buffer.addr += LEVELSPLINECOUNT * sizeof(*world->portal_places);
    memset(world->portal_places, 0, LEVELSPLINECOUNT * sizeof(*world->portal_places));

    if (LevSplList == NULL) {
        return;
    }

    for (i32 i = 0; i < LEVELSPLINECOUNT; ++i) {
        LEVELSPLINE *entry = &LevSplList[i];
        if ((entry->area != -1 && world->level_sub_id != entry->area) ||
            (entry->level != -1 && world->level_idx != entry->level)) {
            continue;
        }

        NUGSCN *scene = *(entry->scene != NULL ? entry->scene : &world->current_gscn);
        if (scene != NULL) {
            NUGSPLINE *spline = NuSplineFind(scene, const_cast<char *>(entry->name));
            world->portal_places[i] = reinterpret_cast<PORTALPOS *>(spline);
            if (spline != NULL) {
                const i32 point_count = spline->length;
                if ((entry->min_points != 0 && point_count < entry->min_points) ||
                    (entry->max_points != 0 && entry->min_points <= entry->max_points &&
                     point_count > entry->max_points)) {
                    world->portal_places[i] = NULL;
                }
            }
        }

        if (scene == NULL) {
            continue;
        }

        if (levspl_i_start >= 0 && levspl_i_start < LEVELSPLINECOUNT) {
            char name[64];
            name[0] = '\0';
            if (Mission_Active(NULL) != NULL) {
                NuStrCpy(name, "bounty_start");
            } else if (world->level_sub_id != -1 && (ADataList[world->level_sub_id].flags & 0x40) != 0 &&
                       hub_from_cutsceneplayer != 0) {
                NuStrCpy(name, "shop_start");
                if (CutScenePlayer_Available() != NULL &&
                    static_cast<CUTSCENEPLAYER_s *>(CutScenePlayer_Available())->return_door != -1) {
                    name[0] = '\0';
                }
            }
            if (name[0] != '\0') {
                NUGSPLINE *start = NuSplineFind(scene, name);
                if (start != NULL && start->length > 1) {
                    world->portal_places[levspl_i_start] = reinterpret_cast<PORTALPOS *>(start);
                    if (levspl_i_startcam >= 0 && levspl_i_startcam < LEVELSPLINECOUNT) {
                        world->portal_places[levspl_i_startcam] = NULL;
                    }
                }
            }
        }
    }
}
