#include "decomp.h"
#include "legoapi/items/base/apiobject.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

struct ShoveObject {
    nuhspecial_s *special;
    NUVEC position;
    f32 radius;
    i16 character_id;
    i16 padding;
};

struct ShoveObjectSystem {
    ShoveObject objects[16];
    i32 count;
};

static ShoveObjectSystem shovesys;

DECOMP_ASSERT(sizeof(ShoveObject) == 0x18, "ShoveObject size");
DECOMP_ASSERT(sizeof(ShoveObjectSystem) == 0x184, "ShoveObjectSystem size");

void AddShoveObject(nuhspecial_s *special, i16 character_id) {
    if (NuSpecialExistsFn(special) != 0) {
        if (shovesys.count < 16) {
            shovesys.objects[shovesys.count].special = special;
            shovesys.objects[shovesys.count].character_id = character_id;
            NuSpecialGetRadius(special, &shovesys.objects[shovesys.count].position,
                               &shovesys.objects[shovesys.count].radius);
            NuVecMtxTransform(&shovesys.objects[shovesys.count].position, &shovesys.objects[shovesys.count].position,
                              NuSpecialGetDrawMtx(special));
            shovesys.count++;
        }
    }
}

void ShoveObjectSysReset() {
    shovesys.count = 0;
}

// Original: 831 bytes.
void ShoveSystemCheckGameObject(GameObject_s *object) {
    if (object->field_0xefc & 1)
        return;
    f32 best_penetration = 0.0f;
    ShoveObject *shove = shovesys.objects;
    for (i32 i = 0; i < shovesys.count; i++, shove++) {
        if (shove->character_id != -1 && shove->character_id == object->apiobj.supporting_platform_id &&
            object->contact_normal.y > 0.707f)
            continue;
        f32 reach = object->apiobj.field_0x1dc + shove->radius + 0.1f;
        if (!(reach > fabsf(object->apiobj.collision_position.x - shove->position.x)))
            continue;
        if (!(reach > fabsf(object->apiobj.collision_position.z - shove->position.z)))
            continue;
        if (!(shove->position.y + shove->radius > object->apiobj.collision_min.y))
            continue;
        NUMTX *matrix = NuSpecialGetDrawMtx(shove->special);
        NUVEC minimum, maximum, point;
        NuSpecialGetBounds(shove->special, &minimum, &maximum);
        if (!BoundingBoxToLine(&minimum, &maximum, matrix, &object->apiobj.collision_min, &object->apiobj.collision_max,
                               object->apiobj.field_0x1dc + 0.1f, &point))
            continue;
        if (point.x == 0.0f && point.y == 0.0f && point.z == 0.0f) {
            point = object->apiobj.collision_position;
        } else {
            NuVecMtxTransform(&point, &point, matrix);
        }
        f32 dx = point.x - shove->position.x;
        f32 dz = point.z - shove->position.z;
        if (dx == 0.0f && dz == 0.0f) {
            object->apiobj.movement_direction.x = 2.0f;
            object->apiobj.movement_direction.z = 0.0f;
            object->field_0x107a = shove->character_id;
            return;
        }
        f32 distance = NuFsqrt(dx * dx + dz * dz);
        f32 penetration = (shove->radius + 0.1f) - distance;
        if (!(penetration > best_penetration))
            continue;
        f32 speed = penetration > 0.1f ? 2.0f : (penetration + penetration) / 0.1f;
        object->apiobj.movement_direction.x = (dx * speed) / distance;
        object->apiobj.movement_direction.z = (dz * speed) / distance;
        best_penetration = penetration;
        object->field_0x107a = shove->character_id;
    }
}
