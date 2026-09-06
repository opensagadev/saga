#include "decomp.h"
#include "globals.h"
#include "legoapi/characters/motion.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"

#include <string.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void setnextpoint(float, float) {
}

void BezierLinePos(VuVec &, VuVec &, VuVec &, VuVec &, VuVec &, float) {
}

void nugraph_blend(i32, i32, i32 *, float) {
}

void BezierLineEval(VuVec &, VuVec &, VuVec &, VuVec &, VuVec &, float) {
}

void nugraph_linear(i32, nuvec_s *, nuvec_s *, i32) {
}

void CalcSplinePoint(flightspline_s *, _vuv_s *, float) {
}

void nugraph_bspline(i32, i32, nuvec_s *, nuvec_s *, i32) {
}

void BezierLineLength(VuVec &, VuVec &, VuVec &, VuVec &) {
}

void BezierLineLength(VuVec &, VuVec &, VuVec &, VuVec &, float) {
}

void LoadShelfSplines() {
    splshelf = NuSplineFind(WORLD->current_gscn, const_cast<char *>("shelf_top1"));
    splcharshelf = NuSplineFind(WORLD->current_gscn, const_cast<char *>("shelf_bottom"));
    splcodes = NuSplineFind(WORLD->current_gscn, const_cast<char *>("shelf_6"));

    if (splshelf == NULL || splcharshelf == NULL || splcodes == NULL) {
        return;
    }

    memset(SubShelfPos, 0, sizeof(SubShelfPos));
    memset(ShelfPos, 0, sizeof(ShelfPos));
    memset(CodePos, 0, 6 * sizeof(*CodePos));

    for (i32 i = 0; i < splshelf->length; ++i) {
        ShelfPos[i] = splshelf->pts[i];
    }
    for (i32 i = 0; i < splcharshelf->length; ++i) {
        SubShelfPos[i] = splcharshelf->pts[i];
    }
    for (i32 i = 0; i < splcodes->length; ++i) {
        CodePos[i] = splcodes->pts[i];
    }

    ShelfPos[2].x += (ShelfPos[3].x - ShelfPos[2].x) * 0.5f;
    ShelfPos[2].z += (ShelfPos[3].z - ShelfPos[2].z) * 0.5f;

    NUVEC diff;
    NuVecSub(&diff, &CodePos[5], &CodePos[4]);
    NuVecAdd(&CodePos[6], &CodePos[5], &diff);
    CodePos[6].y += 0.02f;
    NuVecScale(&diff, &diff, 0.5f);
    NuVecSub(&CodePos[0], &CodePos[0], &diff);
    NuVecSub(&CodePos[1], &CodePos[1], &diff);
    NuVecSub(&CodePos[2], &CodePos[2], &diff);
    NuVecSub(&CodePos[3], &CodePos[3], &diff);
    NuVecSub(&CodePos[4], &CodePos[4], &diff);
    NuVecSub(&CodePos[5], &CodePos[5], &diff);
    NuVecSub(&CodePos[6], &CodePos[6], &diff);

    NUVEC start = splshelf->pts[0];
    NUVEC end = splshelf->pts[splshelf->length];
    NUVEC direction;
    NuVecSub(&direction, &end, &start);
    shelfang = static_cast<u16>(NuAtan2D(direction.x, direction.z));
}

static const NUVEC *SplinePoint(const NUGSPLINE *spline, i32 index) {
    return reinterpret_cast<const NUVEC *>(reinterpret_cast<const u8 *>(spline->pts) + index * spline->pt_size);
}

static void SplinePointAngles(NUGSPLINE *spline, i32 point, i32 looping, u16 *yaw, u16 *pitch) {
    const NUVEC *current = SplinePoint(spline, point);
    NUVEC direction = {};

    i32 previous = point - 1;
    if (previous < 0 && looping != 0) {
        previous = spline->length - 1;
    }
    if (previous >= 0) {
        const NUVEC *value = SplinePoint(spline, previous);
        direction.x += current->x - value->x;
        direction.y += current->y - value->y;
        direction.z += current->z - value->z;
    }

    i32 next = point + 1;
    if (next >= spline->length && looping != 0) {
        next = 0;
    }
    if (next < spline->length) {
        const NUVEC *value = SplinePoint(spline, next);
        direction.x += value->x - current->x;
        direction.y += value->y - current->y;
        direction.z += value->z - current->z;
    }

    if (pitch != NULL) {
        *pitch = NuAtan2D(direction.y, NuFsqrt(direction.x * direction.x + direction.z * direction.z));
    }
    if (yaw != NULL) {
        *yaw = NuAtan2D(direction.x, direction.z);
    }
}

void PointAlongSpline(nugspline_s *spline, float position, nuvec_s *result, u16 *yaw, u16 *pitch, i32 looping) {
    if (yaw != NULL) {
        *yaw = 0;
    }
    if (pitch != NULL) {
        *pitch = 0;
    }
    if (spline == NULL || result == NULL || spline->length <= 0) {
        return;
    }

    if (position < 0.0f) {
        position = 0.0f;
    } else if (position > 1.0f) {
        position = 1.0f;
    }

    const i32 span_count = looping != 0 ? spline->length : spline->length - 1;
    const f32 point_position = static_cast<f32>(span_count) * position;
    i32 point = static_cast<i32>(point_position);
    if (point >= spline->length) {
        point = spline->length - 1;
    }
    const NUVEC *from = SplinePoint(spline, point);
    *result = *from;
    SplinePointAngles(spline, point, looping, yaw, pitch);

    i32 next = point + 1;
    if (next >= spline->length) {
        if (looping == 0) {
            return;
        }
        next = 0;
    }

    const f32 ratio = point_position - static_cast<f32>(point);
    if (ratio == 0.0f) {
        return;
    }
    const NUVEC *to = SplinePoint(spline, next);
    result->x += (to->x - from->x) * ratio;
    result->y += (to->y - from->y) * ratio;
    result->z += (to->z - from->z) * ratio;

    u16 next_yaw = 0;
    u16 next_pitch = 0;
    SplinePointAngles(spline, next, looping, yaw != NULL ? &next_yaw : NULL, pitch != NULL ? &next_pitch : NULL);
    if (yaw != NULL) {
        *yaw = static_cast<u16>(*yaw + static_cast<i32>(static_cast<f32>(RotDiff(*yaw, next_yaw)) * ratio));
    }
    if (pitch != NULL) {
        *pitch = static_cast<u16>(*pitch + static_cast<i32>(static_cast<f32>(RotDiff(*pitch, next_pitch)) * ratio));
    }
}

void getnextdatapoint(float *, i32 *) {
}

void FlightSpline_Init(WORLDINFO_s *, flightspline_s *, i32) {
}

void OutSideSplineArea(nuvec_s *, nugspline_s *, nuvec_s *, nuvec_s *, i32) {
}

void InitSplinePosition(SPLINEPOS_s *position, nugspline_s *spline, float distance, i32 looping) {
    if (position == NULL) {
        return;
    }

    SPLINEPOSITION_RUNTIME_s *runtime = reinterpret_cast<SPLINEPOSITION_RUNTIME_s *>(position);
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
        runtime->normalized_position = 0.0f;
    }
}

void nugraphGetXatIndex(nugraph_s *, i32) {
}

void nugraphGetYatIndex(nugraph_s *, i32) {
}

void nugraph_catmullrom(i32, nuvec_s *, nuvec_s *, i32) {
}

void GetNearestSplinePos(nuvec_s *point, SPLINEPOS_s *position, nugspline_s *spline, i32 looping, i16 first_point,
                         i16 last_point) {
    if (position == NULL) {
        return;
    }
    SPLINEPOSITION_RUNTIME_s *runtime = reinterpret_cast<SPLINEPOSITION_RUNTIME_s *>(position);
    memset(runtime, 0, sizeof(*runtime));
    if (spline == NULL || point == NULL || spline->length <= 1) {
        return;
    }

    runtime->spline = spline;
    runtime->looping = static_cast<u8>(looping);
    const i32 segment_count = spline->length + (looping != 0 ? 1 : 0);
    i32 begin = first_point < 0 ? 0 : first_point;
    if (begin >= segment_count) {
        return;
    }
    i32 end = last_point < 0 ? spline->length : last_point;
    if (end > spline->length) {
        end = spline->length;
    }

    f32 nearest_distance = 1.0e9f;
    for (i32 index = begin; index < end; ++index) {
        const f32 distance = NuVecDistSqr(point, const_cast<NUVEC *>(SplinePoint(spline, index)), NULL);
        if (distance < nearest_distance) {
            runtime->segment = static_cast<i16>(index);
            nearest_distance = distance;
        }
    }

    const i32 next = (runtime->segment + 1) % spline->length;
    const NUVEC *current_point = SplinePoint(spline, runtime->segment);
    runtime->distance = 0.0f;
    runtime->segment_length =
        NuVecDist(const_cast<NUVEC *>(SplinePoint(spline, next)), const_cast<NUVEC *>(current_point), NULL);
    runtime->position = *current_point;
    runtime->normalized_position = static_cast<f32>(runtime->segment) / static_cast<f32>(segment_count - 1);
}

void nugraph_compute_point(i32 *, i32, i32, float, nuvec_s *, nuvec_s *) {
}

void CalcSplinePointFromDist(flightspline_s *, _vuv_s *, float) {
}

static LEVELSPLINE *LevSplList;
static i32 LEVELSPLINECOUNT;
static i32 levspl_i_start = -1;
static i32 levspl_i_startcam = -1;

void LevelSplines_InitForGame(LEVELSPLINE *splines) {
    LevSplList = splines;
    LEVELSPLINECOUNT = 0;
    levspl_i_start = -1;
    levspl_i_startcam = -1;

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

void nugraph_compute_intervals(i32 *, i32, i32) {
}

void EvaluateSplineXZIntersection(nugspline_s *, i32, SPLINEPOS_s *, nugspline_s *, i32, SPLINEPOS_s *) {
}

void nugraph_compute_linear_point(i32, float, nuvec_s *, nuvec_s *) {
}

void nugraph_compute_catmull_point(i32, float, nuvec_s *, nuvec_s *) {
}

void setpoint(float) {
}

static __used__ f32 SplineLength(nugspline_s *, i32) {
    return 0.0f;
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

        NUGSCN *scene = entry->scene != NULL ? *entry->scene : world->current_gscn;
        if (scene == NULL) {
            continue;
        }

        NUGSPLINE *spline = NuSplineFind(scene, const_cast<char *>(entry->name));
        world->portal_places[i] = reinterpret_cast<PORTALPOS *>(spline);
        if (spline == NULL) {
            continue;
        }

        const i32 point_count = spline->length;
        if ((entry->min_points != 0 && point_count < entry->min_points) ||
            (entry->max_points != 0 && entry->min_points <= entry->max_points && point_count > entry->max_points)) {
            world->portal_places[i] = NULL;
        }
    }
}
