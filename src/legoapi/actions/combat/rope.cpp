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
    f32 px;
    volatile f32 py;
    f32 pz;
    px = (points[0].x - start->x) * 0.5f + start->x;
    py = (points[0].y - start->y) * 0.5f + start->y;
    pz = (points[0].z - start->z) * 0.5f + start->z;
    vertices[0].position = *start;
    vertices[1].position.x = px;
    vertices[1].position.y = py;
    vertices[1].position.z = pz;
    NuRndrLine3d(vertices, SolidMtl3D, NULL);

    nuvec_s *next = points + 1;
    for (i32 i = 0; i < count - 1; ++i, ++next) {
        NUVEC middle;
        middle.x = (next->x - points[i].x) * 0.5f + points[i].x;
        middle.y = (next->y - points[i].y) * 0.5f + points[i].y;
        middle.z = (next->z - points[i].z) * 0.5f + points[i].z;

        vertices[0].position = vertices[1].position;
        vertices[1].position.x = (0.4444443881511688f * px + 0.444444477558136f * points[i].x) +
                                 0.11111113429069519f * middle.x;
        vertices[1].position.y = (0.4444443881511688f * py + 0.444444477558136f * points[i].y) +
                                 0.11111113429069519f * middle.y;
        vertices[1].position.z = (0.4444443881511688f * pz + 0.444444477558136f * points[i].z) +
                                 0.11111113429069519f * middle.z;
        NuRndrLine3d(vertices, SolidMtl3D, NULL);

        memcpy(&vertices[0], &vertices[1], sizeof(vertices[0]));
        vertices[1].position.x = (0.11111107468605042f * px + 0.4444444179534912f * points[i].x) +
                                 0.44444453716278076f * middle.x;
        vertices[1].position.y = (0.11111107468605042f * py + 0.4444444179534912f * points[i].y) +
                                 0.44444453716278076f * middle.y;
        vertices[1].position.z = (0.11111107468605042f * pz + 0.4444444179534912f * points[i].z) +
                                 0.44444453716278076f * middle.z;
        NuRndrLine3d(vertices, SolidMtl3D, NULL);

        memcpy(&vertices[0], &vertices[1], sizeof(vertices[0]));
        vertices[1].position.x = (3.552713678800501e-15f * px - 1.1920928955078125e-7f * points[i].x) + middle.x;
        vertices[1].position.y = (3.552713678800501e-15f * py - 1.1920928955078125e-7f * points[i].y) + middle.y;
        vertices[1].position.z = (3.552713678800501e-15f * pz - 1.1920928955078125e-7f * points[i].z) + middle.z;
        NuRndrLine3d(vertices, SolidMtl3D, NULL);
        memcpy(&vertices[0], &vertices[1], sizeof(vertices[0]));
        px = middle.x;
        py = middle.y;
        pz = middle.z;
    }
    vertices[1].position = points[count - 1];
    NuRndrLine3d(vertices, SolidMtl3D, NULL);
}
