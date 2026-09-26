#include "legoapi/actions/combat/rope.h"
#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nustring.h"
#include <string.h>

extern numtl_s *SolidMtl3D;

void DrawRopeCurved(nuvec_s *start, nuvec_s *points, i32 count, i32, numtl_s *) {
    if (count <= 2)
        return;

    NURND_VERTEX3D vertices[2];
    vertices[0].colour = 0xffffffff;
    vertices[1].colour = 0xffffffff;
    NUVEC previous;
    previous.x = (points[0].x - start->x) * 0.5f + start->x;
    previous.y = (points[0].y - start->y) * 0.5f + start->y;
    previous.z = (points[0].z - start->z) * 0.5f + start->z;
    vertices[0].position = *start;
    vertices[1].position = previous;
    NuRndrLine3d(vertices, SolidMtl3D, NULL);

    for (i32 i = 0; i < count - 1; ++i) {
        NUVEC middle;
        middle.x = (points[i + 1].x - points[i].x) * 0.5f + points[i].x;
        middle.y = (points[i + 1].y - points[i].y) * 0.5f + points[i].y;
        middle.z = (points[i + 1].z - points[i].z) * 0.5f + points[i].z;

        memcpy(&vertices[0], &vertices[1], sizeof(vertices[0]));
        vertices[1].position.x = (0.4444443881511688f * previous.x + 0.444444477558136f * points[i].x) +
                                 0.11111113429069519f * middle.x;
        vertices[1].position.y = (0.4444443881511688f * previous.y + 0.444444477558136f * points[i].y) +
                                 0.11111113429069519f * middle.y;
        vertices[1].position.z = (0.4444443881511688f * previous.z + 0.444444477558136f * points[i].z) +
                                 0.11111113429069519f * middle.z;
        NuRndrLine3d(vertices, SolidMtl3D, NULL);

        memcpy(&vertices[0], &vertices[1], sizeof(vertices[0]));
        vertices[1].position.x = (0.11111107468605042f * previous.x + 0.4444444179534912f * points[i].x) +
                                 0.44444453716278076f * middle.x;
        vertices[1].position.y = (0.11111107468605042f * previous.y + 0.4444444179534912f * points[i].y) +
                                 0.44444453716278076f * middle.y;
        vertices[1].position.z = (0.11111107468605042f * previous.z + 0.4444444179534912f * points[i].z) +
                                 0.44444453716278076f * middle.z;
        NuRndrLine3d(vertices, SolidMtl3D, NULL);

        memcpy(&vertices[0], &vertices[1], sizeof(vertices[0]));
        vertices[1].position.x = (3.552713678800501e-15f * previous.x - 1.1920928955078125e-7f * points[i].x) + middle.x;
        vertices[1].position.y = (3.552713678800501e-15f * previous.y - 1.1920928955078125e-7f * points[i].y) + middle.y;
        vertices[1].position.z = (3.552713678800501e-15f * previous.z - 1.1920928955078125e-7f * points[i].z) + middle.z;
        NuRndrLine3d(vertices, SolidMtl3D, NULL);
        memcpy(&vertices[0], &vertices[1], sizeof(vertices[0]));
        previous = middle;
    }
    vertices[1].position = points[count - 1];
    NuRndrLine3d(vertices, SolidMtl3D, NULL);
}
