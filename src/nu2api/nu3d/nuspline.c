#include "nu2api/nu3d/nuspline.h"

#include "nu2api/nucore/nustring.h"
#include <stdlib.h>

i32 NuSplineFindAllSub(NUGSCN *scene, char *name, NUGSPLINE **results, i32 capacity) {
    if (capacity <= 0 || scene == NULL)
        return 0;
    i32 count = 0;
    NUGSPLINE *spline = scene->splines;
    for (i32 i = 0; i < scene->numsplines; spline++, i++) {
        if (NuStrIStr(spline->name, name) != NULL) {
            results[count++] = spline;
            if (count >= capacity)
                break;
        }
    }
    return count;
}

NUGSPLINE *NuSplineFindNextBeg(NUGSCN *scene, char *name, NUGSPLINE *previous) {
    if (scene == NULL)
        return NULL;
    NUGSPLINE *spline = previous;
    if (spline == NULL)
        spline = scene->splines;
    else
        spline++;
    NUGSPLINE *end = scene->splines + scene->numsplines;
    for (; spline < end; spline++) {
        if (NuStrNICmp(name, spline->name, -1) == 0)
            return spline;
    }
    return NULL;
}

void NuSplineGetRandomPoint(NUGSPLINE *spline, NUVEC *point) {
    i32 index = lrand48() % spline->length;
    i32 next_index = (index + 1) % spline->length;
    NUVEC *start = &spline->pts[index * (i16)spline->pt_size];
    NUVEC *end = &spline->pts[next_index * (i16)spline->pt_size];
    f32 fraction = (lrand48() & 0x7fff) * (1.0f / 32768.0f);
    NuVecLerp(point, start, end, fraction);
}

// Finds the spline named `name` in `scene`, or NULL if there is no such
// spline.
NUGSPLINE *NuSplineFind(NUGSCN *scene, char *name) {
    if (scene == NULL) {
        return NULL;
    }

    NUGSPLINE *spline = scene->splines;

    if (scene->numsplines <= 0) {
        return NULL;
    }

    for (i32 i = 0; i < scene->numsplines; i++, spline++) {
        if (NuStrICmp(name, spline->name) == 0) {
            return spline;
        }
    }

    return NULL;
}

// The -1 length uses NuStrNICmp's prefix-comparison convention.
i32 NuSplineFindAllBeg(NUGSCN *scene, char *name, NUGSPLINE **results, i32 capacity) {
    if (capacity <= 0 || scene == NULL)
        return 0;
    i32 count = 0;
    NUGSPLINE *spline = scene->splines;
    for (i32 i = 0; i < scene->numsplines; spline++, i++) {
        if (NuStrNICmp(name, spline->name, -1) == 0) {
            results[count++] = spline;
            if (count >= capacity)
                break;
        }
    }
    return count;
}
