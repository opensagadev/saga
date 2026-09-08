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

// The -1 length uses NuStrNICmp's prefix-comparison convention.
i32 NuSplineFindAllBeg(NUGSCN *scene, char *name, NUGSPLINE **results, i32 capacity) {
    if (capacity <= 0 || scene == NULL)
        return 0;
    i32 count = 0;
    NUGSPLINE *spline = scene->splines;
    for (i32 i = 0; i < scene->numsplines; i++, spline++) {
        if (NuStrNICmp(name, spline->name, -1) == 0) {
            results[count++] = spline;
            if (count >= capacity)
                break;
        }
    }
    return count;
}
