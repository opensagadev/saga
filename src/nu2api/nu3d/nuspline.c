#include "nu2api/nu3d/nuspline.h"

#include "nu2api/nucore/nustring.h"

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

i32 NuSplineFindAllBeg(NUGSCN *scene, char *prefix, NUGSPLINE **results, i32 max_results) {
    if (scene == NULL || max_results <= 0) {
        return 0;
    }

    i32 result_count = 0;
    for (i32 i = 0; i < scene->numsplines; ++i) {
        NUGSPLINE *spline = &scene->splines[i];
        if (NuStrNICmp(prefix, spline->name, -1) == 0) {
            results[result_count++] = spline;
            if (result_count >= max_results) {
                break;
            }
        }
    }
    return result_count;
}
