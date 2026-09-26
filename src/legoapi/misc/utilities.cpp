#include "decomp.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "globals.h"
#include "legoapi/render/fx/game_deb.h"

#include "nu2api/numath/nurand.h"

#include <stdlib.h>
#include <ctype.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern i32 qseed;
extern i32 GAMERAND;
extern "C" void NuPartSetSeed(i32);

static const u32 crctab[256] = {
    0x0000, 0xc0c1, 0xc181, 0x0140, 0xc301, 0x03c0, 0x0280, 0xc241, 0xc601, 0x06c0, 0x0780, 0xc741, 0x0500, 0xc5c1,
    0xc481, 0x0440, 0xcc01, 0x0cc0, 0x0d80, 0xcd41, 0x0f00, 0xcfc1, 0xce81, 0x0e40, 0x0a00, 0xcac1, 0xcb81, 0x0b40,
    0xc901, 0x09c0, 0x0880, 0xc841, 0xd801, 0x18c0, 0x1980, 0xd941, 0x1b00, 0xdbc1, 0xda81, 0x1a40, 0x1e00, 0xdec1,
    0xdf81, 0x1f40, 0xdd01, 0x1dc0, 0x1c80, 0xdc41, 0x1400, 0xd4c1, 0xd581, 0x1540, 0xd701, 0x17c0, 0x1680, 0xd641,
    0xd201, 0x12c0, 0x1380, 0xd341, 0x1100, 0xd1c1, 0xd081, 0x1040, 0xf001, 0x30c0, 0x3180, 0xf141, 0x3300, 0xf3c1,
    0xf281, 0x3240, 0x3600, 0xf6c1, 0xf781, 0x3740, 0xf501, 0x35c0, 0x3480, 0xf441, 0x3c00, 0xfcc1, 0xfd81, 0x3d40,
    0xff01, 0x3fc0, 0x3e80, 0xfe41, 0xfa01, 0x3ac0, 0x3b80, 0xfb41, 0x3900, 0xf9c1, 0xf881, 0x3840, 0x2800, 0xe8c1,
    0xe981, 0x2940, 0xeb01, 0x2bc0, 0x2a80, 0xea41, 0xee01, 0x2ec0, 0x2f80, 0xef41, 0x2d00, 0xedc1, 0xec81, 0x2c40,
    0xe401, 0x24c0, 0x2580, 0xe541, 0x2700, 0xe7c1, 0xe681, 0x2640, 0x2200, 0xe2c1, 0xe381, 0x2340, 0xe101, 0x21c0,
    0x2080, 0xe041, 0xa001, 0x60c0, 0x6180, 0xa141, 0x6300, 0xa3c1, 0xa281, 0x6240, 0x6600, 0xa6c1, 0xa781, 0x6740,
    0xa501, 0x65c0, 0x6480, 0xa441, 0x6c00, 0xacc1, 0xad81, 0x6d40, 0xaf01, 0x6fc0, 0x6e80, 0xae41, 0xaa01, 0x6ac0,
    0x6b80, 0xab41, 0x6900, 0xa9c1, 0xa881, 0x6840, 0x7800, 0xb8c1, 0xb981, 0x7940, 0xbb01, 0x7bc0, 0x7a80, 0xba41,
    0xbe01, 0x7ec0, 0x7f80, 0xbf41, 0x7d00, 0xbdc1, 0xbc81, 0x7c40, 0xb401, 0x74c0, 0x7580, 0xb541, 0x7700, 0xb7c1,
    0xb681, 0x7640, 0x7200, 0xb2c1, 0xb381, 0x7340, 0xb101, 0x71c0, 0x7080, 0xb041, 0x5000, 0x90c1, 0x9181, 0x5140,
    0x9301, 0x53c0, 0x5280, 0x9241, 0x9601, 0x56c0, 0x5780, 0x9741, 0x5500, 0x95c1, 0x9481, 0x5440, 0x9c01, 0x5cc0,
    0x5d80, 0x9d41, 0x5f00, 0x9fc1, 0x9e81, 0x5e40, 0x5a00, 0x9ac1, 0x9b81, 0x5b40, 0x9901, 0x59c0, 0x5880, 0x9841,
    0x8801, 0x48c0, 0x4980, 0x8941, 0x4b00, 0x8bc1, 0x8a81, 0x4a40, 0x4e00, 0x8ec1, 0x8f81, 0x4f40, 0x8d01, 0x4dc0,
    0x4c80, 0x8c41, 0x4400, 0x84c1, 0x8581, 0x4540, 0x8701, 0x47c0, 0x4680, 0x8641, 0x8201, 0x42c0, 0x4380, 0x8341,
    0x4100, 0x81c1, 0x8081, 0x4040,
};

u32 HashString(unsigned char *string) {
    u32 hash = crctab[string[0]];
    hash = (hash >> 8) ^ crctab[static_cast<u8>(hash ^ string[1])];
    return (hash >> 8) ^ crctab[static_cast<u8>(hash ^ string[2])];
}

void ResetSeeds() {
    srand48(0);
    qseed = 0x3039;
    GAMERAND = 0x1f3ad27f;
    NuRandSeed(0);
    DebrisSetSeed(0);
    NuRandSetSeed(NULL, 0);
    NuPartSetSeed(0);
}

i32 i_temp_xrot;

void FindAnglesXY(nuvec_s *direction, u16 *x_rotation, u16 *y_rotation) {
    temp_yrot = NuAtan2D(direction->x, direction->z);
    if (y_rotation != NULL)
        *y_rotation = temp_yrot;
    i_temp_xrot = -NuAtan2D(direction->y, NuFsqrt(direction->x * direction->x + direction->z * direction->z));
    temp_xrot = i_temp_xrot;
    if (x_rotation != NULL)
        *x_rotation = temp_xrot;
}

void FindAnglesZX(nuvec_s *normal, u16 *x_rotation, u16 *z_rotation) {
    const i32 x_angle = NuAtan2D(normal->z, normal->y);
    if (x_rotation != NULL) {
        *x_rotation = static_cast<u16>(x_angle);
    }
    temp_xrot = static_cast<i16>(x_angle);

    NUVEC rotated;
    const u16 truncated_x_angle = static_cast<u16>(x_angle);
    NuVecRotateX(&rotated, normal, static_cast<NUANG>(-static_cast<i32>(truncated_x_angle)));
    const i32 z_angle = -NuAtan2D(rotated.x, rotated.y);
    if (z_rotation != NULL) {
        *z_rotation = static_cast<u16>(z_angle);
    }
    temp_zrot = static_cast<i16>(z_angle);
}

i32 getNumDigits(i32 value) {
    if (__builtin_expect(value <= 9, 0))
        return 1;
    i32 threshold = 10;
    i32 digits = 1;
    do {
        threshold *= 10;
        ++digits;
    } while (value >= threshold);
    return digits;
}

i32 LineCrossedXZ(f32 ax, f32 az, f32 bx, f32 bz, f32 cx, f32 cz, f32 dx, f32 dz) {
    f32 first = (bx - cx) * (dz - cz) + (bz - cz) * (cx - dx);
    if (first >= 0.0f)
        return 0;
    f32 second = (ax - cx) * (dz - cz) + (az - cz) * (cx - dx);
    if (!(second >= 0.0f))
        return 0;
    f32 third = (bx - ax) * (cz - az) + (bz - az) * (ax - cx);
    if (!(third >= 0.0f))
        return 1;
    f32 az_to_dz = az;
    i32 result = 2;
    az_to_dz -= dz;
    f32 fourth = (bx - dx) * az_to_dz + (bz - dz) * (dx - ax);
    if (fourth >= 0.0f)
        return result;
    return 1;
}

i32 ScaleAndClamp(volatile i32 value) {
    i32 scaled = value << 7;
    scaled += scaled << 5;
    value = scaled / 1048576;
    if (value < -128)
        value = -128;
    if (value > 127)
        value = 127;
    return value + 128;
}

void VecRotateAxis(nuvec_s *vector, u16 angle, nuvec_s *axis) {
    NuVecNorm(axis, axis);
    const f32 cosine = NuTrigTable[((static_cast<u32>(angle) + 0x4000) >> 1) & 0x7fff];
    const f32 sine = NuTrigTable[angle >> 1];
    const f32 complement = 1.0f - cosine;
    const f32 x = axis->x;
    const f32 y = axis->y;
    const f32 z = axis->z;
    const NUVEC source = *vector;
    const f32 tx = complement * x;
    const f32 ty = complement * y;
    const f32 tz = complement * z;
    const f32 xy = tx * y;
    const f32 xz = tx * z;
    const f32 yz = ty * z;
    const f32 sx = sine * x;
    const f32 sy = sine * y;
    const f32 sz = z * sine;
    vector->x = ((tx * x + cosine) * source.x + 0.0f) + (xy - sz) * source.y + (xz + sy) * source.z;
    vector->y = ((xy + sz) * source.x + 0.0f) + (y * ty + cosine) * source.y + (yz - sx) * source.z;
    vector->z = ((xz - sy) * source.x + 0.0f) + (sx + yz) * source.y + (tz * z + cosine) * source.z;
}

i32 SolveQuadratic(f32 a, f32 b, f32 c, f32 *first, f32 *second) {
    if (a == 0.0f) {
        if (b == 0.0f)
            return 0;
        *first = *second = -c / b;
        return 1;
    }
    const f32 four_ac = 4.0f * a * c;
    const f32 b_squared = b * b;
    if (four_ac > b_squared)
        return 0;
    if (four_ac == b_squared) {
        *first = *second = -b / (a + a);
        return 1;
    }
    const f32 root = NuFsqrt(b_squared - four_ac);
    *first = (-b - root) / (a + a);
    *second = (root - b) / (a + a);
    return 1;
}

f32 XZLinesClosest(nuvec_s *a, nuvec_s *b, nuvec_s *c, nuvec_s *d, f32 *first, f32 *second) {
    NUVEC ac __attribute__((aligned(16)));
    NUVEC bc __attribute__((aligned(16)));
    NUVEC ca __attribute__((aligned(16)));
    NUVEC da __attribute__((aligned(16)));
    NUVEC direction;
    NuVecSub(&direction, b, a);
    i32 angle = NuAtan2D(direction.x, direction.z);
    NuVecSub(&ca, c, a);
    NuVecRotateY(&ca, &ca, -angle);
    NuVecSub(&da, d, a);
    NuVecRotateY(&da, &da, -angle);
    NuVecSub(&direction, d, c);
    angle = NuAtan2D(direction.x, direction.z);
    NuVecSub(&ac, a, c);
    NuVecRotateY(&ac, &ac, -angle);
    NuVecSub(&bc, b, c);
    NuVecRotateY(&bc, &bc, -angle);

    if (NuFsign(ca.x) != NuFsign(da.x) && NuFsign(ac.x) != NuFsign(bc.x)) {
        *first = NuFabs(ac.x) / (NuFabs(ac.x) + NuFabs(bc.x));
        *second = NuFabs(ca.x) / (NuFabs(ca.x) + NuFabs(da.x));
        return 0.0f;
    }

    f32 ab_length = NuVecXZDist(b, a, NULL);
    f32 cd_length = NuVecXZDist(d, c, NULL);
    f32 closest = 1000000000.0f;
    f32 distance, fraction;
    if (ca.z < 0.0f) {
        distance = NuVecXZDist(a, &ca, NULL);
        fraction = 0.0f;
    } else if (ca.z > ab_length) {
        distance = NuVecXZDist(b, &ca, NULL);
        fraction = 1.0f;
    } else {
        fraction = ca.z / ab_length;
        distance = NuFabs(ca.x);
    }
    if (distance < closest) {
        *first = fraction;
        *second = 0.0f;
        closest = distance;
    }
    if (da.z < 0.0f) {
        distance = NuVecXZDist(a, &da, NULL);
        fraction = 0.0f;
    } else if (da.z > ab_length) {
        distance = NuVecXZDist(b, &da, NULL);
        fraction = 1.0f;
    } else {
        fraction = da.z / ab_length;
        distance = NuFabs(da.x);
    }
    if (distance < closest) {
        *first = fraction;
        *second = 1.0f;
        closest = distance;
    }
    if (ac.z < 0.0f) {
        distance = NuVecXZDist(c, &ac, NULL);
        fraction = 0.0f;
    } else if (ac.z > cd_length) {
        distance = NuVecXZDist(d, &ac, NULL);
        fraction = 1.0f;
    } else {
        fraction = ac.z / cd_length;
        distance = NuFabs(ac.x);
    }
    if (distance < closest) {
        *second = fraction;
        *first = 0.0f;
        closest = distance;
    }
    if (bc.z < 0.0f) {
        distance = NuVecXZDist(c, &bc, NULL);
        fraction = 0.0f;
    } else if (bc.z > cd_length) {
        distance = NuVecXZDist(d, &bc, NULL);
        fraction = 1.0f;
    } else {
        fraction = bc.z / cd_length;
        distance = NuFabs(bc.x);
    }
    if (distance < closest) {
        *second = fraction;
        *first = 1.0f;
        closest = distance;
    }
    return closest;
}

i32 LineIntersectXY(nuvec_s *a, nuvec_s *b, nuvec_s *c, nuvec_s *d, nuvec_s *first, nuvec_s *second) {
    const f32 ax = a->x, ay = a->y;
    const f32 dx = b->x - ax, dy = b->y - ay;
    const f32 ex = d->x - c->x, ey = d->y - c->y;
    const f32 denominator = ey * dx - ex * dy;
    if (denominator == 0.0f)
        return 0;
    const f32 ox = ax - c->x, oy = ay - c->y;
    const f32 t = (ex * oy - ey * ox) / denominator;
    const f32 u = (oy * dx - ox * dy) / denominator;
    if (first != NULL) {
        first->x = dx * t + ax;
        first->y = dy * t + ay;
    }
    if (second != NULL) {
        second->x = (d->x - c->x) * u + c->x;
        second->y = (d->y - c->y) * u + c->y;
    }
    return t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f;
}

void MakeThrowVector(NUVEC *result, NUVEC *origin, NUVEC *target, NUVEC *target_velocity, f32 speed, f32 gravity) {
    NUVEC predicted = *target;
    NUVEC delta;
    NUVEC horizontal_origin;
    horizontal_origin.x = origin->x;
    horizontal_origin.y = 0.0f;
    horizontal_origin.z = origin->z;
    NuVecSub(&delta, &predicted, &horizontal_origin);
    NuVecAddScale(&predicted, target, target_velocity, NuVecMag(&delta) / speed);
    horizontal_origin.x = origin->x;
    horizontal_origin.y = 0.0f;
    horizontal_origin.z = origin->z;
    NuVecSub(&delta, &predicted, &horizontal_origin);
    NuVecAddScale(&predicted, target, target_velocity, NuVecMag(&delta) / speed);
    horizontal_origin.x = origin->x;
    horizontal_origin.y = 0.0f;
    horizontal_origin.z = origin->z;
    NuVecSub(&delta, &predicted, &horizontal_origin);
    NuVecAddScale(&predicted, target, target_velocity, NuVecMag(&delta) / speed);
    const f32 time = NuVecXZDist(&predicted, origin, NULL) / speed;
    const f32 vertical_speed = (predicted.y - origin->y) / (FRAMETIME + time) - (gravity * 0.5f) * time;
    const u16 angle = NuAtan2D(predicted.x - origin->x, predicted.z - origin->z);
    result->x = speed * NU_SIN_LUT(angle);
    result->y = vertical_speed;
    result->z = speed * NU_COS_LUT(angle);
}

i32 OnOrInsidePlane(nuvec_s *point, nuvec_s *plane_point, nuvec_s *plane_normal, nuvec_s *corrected_point,
                    float normal_offset, float *distance_out) {
    NUVEC test_point;
    if (normal_offset != 0.0f) {
        NuVecScale(&test_point, plane_normal, normal_offset);
        NuVecAdd(&test_point, &test_point, point);
    } else {
        test_point = *point;
    }

    const f32 distance = (test_point.x - plane_point->x) * plane_normal->x +
                         (test_point.y - plane_point->y) * plane_normal->y +
                         (test_point.z - plane_point->z) * plane_normal->z;
    if (distance_out != NULL) {
        *distance_out = distance;
    }
    if (!(distance <= 0.0f)) {
        return 0;
    }

    if (corrected_point != NULL) {
        NuVecScale(corrected_point, plane_normal, 0.5f - distance);
        NuVecAdd(corrected_point, corrected_point, point);
    }
    return 1;
}

i32 PackCharIntoInt(char a, char b, char c, char d) {
    return (static_cast<i32>(a) << 24) | ((static_cast<i32>(b) << 16) & 0xff0000) |
           ((static_cast<i32>(c) << 8) & 0xffff) | static_cast<u8>(d);
}

f32 DistanceToLineXZ(NUVEC *position, NUVEC *first, NUVEC *second) {
    u16 angle = -NuAtan2D(second->x - first->x, second->z - first->z);
    return NuFabs((position->x - first->x) * NU_COS_LUT(angle) + (position->z - first->z) * NU_SIN_LUT(angle));
}

i32 MatrixReflection(numtx_s *matrix, i32 axis, f32 plane, f32 override_plane, numtx_s *result) {
    if (static_cast<u8>(Reflections_On) == 0)
        return 0;

    switch (axis) {
        case 1:
            *result = *matrix;
            result->m00 = -result->m00;
            result->m10 = -result->m10;
            result->m20 = -result->m20;
            result->m30 = plane - (result->m30 - plane);
            return 1;

        case 2:
            if (override_plane != 2000000.0f) {
                if (MatrixReflection_CanOverrideFn != NULL && MatrixReflection_CanOverrideFn(plane) == 0)
                    return 0;
                plane = override_plane;
            }
            *result = *matrix;
            result->m01 = -result->m01;
            result->m11 = -result->m11;
            result->m21 = -result->m21;
            result->m31 = plane - (result->m31 - plane);
            return 1;

        case 3:
            *result = *matrix;
            result->m02 = -result->m02;
            result->m12 = -result->m12;
            result->m22 = -result->m22;
            result->m32 = plane - (result->m32 - plane);
            return 1;

        default:
            return 0;
    }
}

i32 OnOrOutsidePlane(nuvec_s *point, nuvec_s *plane_point, nuvec_s *normal) {
    f32 distance = (point->x - plane_point->x) * normal->x + (point->y - plane_point->y) * normal->y +
                   (point->z - plane_point->z) * normal->z;
    return distance >= 0.0f;
}

i32 PackShortIntoInt(i16 high, i16 low) {
    i32 packed = high;
    packed <<= 16;
    packed |= static_cast<u16>(low);
    return packed;
}

f32 RatioAlongLineXZ(nuvec_s *point, nuvec_s *start, nuvec_s *end) {
    f32 dx = end->x - start->x;
    f32 dz = end->z - start->z;
    i32 angle = -NuAtan2D(dx, dz);
    f32 sine = NuTrigTable[static_cast<u16>(angle) >> 1];
    f32 cosine = NuTrigTable[((static_cast<u16>(angle) + 0x4000) >> 1) & 0x7fff];
    f32 px = point->x - start->x;
    f32 pz = point->z - start->z;
    f32 along = pz * cosine - px * sine;
    if (along <= 0.0f)
        return 0.0f;
    f32 length = dz * cosine - dx * sine;
    if (along >= length)
        return 1.0f;
    return along / length;
}

i32 XZLinesIntersect(nuvec_s *a, nuvec_s *b, nuvec_s *c, nuvec_s *d, float *first, float *second) {
    NUVEC ac __attribute__((aligned(16)));
    NUVEC bc __attribute__((aligned(16)));
    NUVEC ca __attribute__((aligned(16)));
    NUVEC da __attribute__((aligned(16)));
    NUVEC direction;
    NuVecSub(&direction, b, a);
    i32 angle = NuAtan2D(direction.x, direction.z);
    NuVecSub(&ca, c, a);
    NuVecRotateY(&ca, &ca, -angle);
    NuVecSub(&da, d, a);
    NuVecRotateY(&da, &da, -angle);
    if (NuFsign(ca.x) == NuFsign(da.x))
        return 0;

    NuVecSub(&direction, d, c);
    angle = NuAtan2D(direction.x, direction.z);
    NuVecSub(&ac, a, c);
    NuVecRotateY(&ac, &ac, -angle);
    NuVecSub(&bc, b, c);
    NuVecRotateY(&bc, &bc, -angle);
    if (NuFsign(ac.x) == NuFsign(bc.x))
        return 0;

    if (first != NULL)
        *first = __builtin_fabsf(ac.x) / (__builtin_fabsf(ac.x) + __builtin_fabsf(bc.x));
    if (second != NULL)
        *second = __builtin_fabsf(ca.x) / (__builtin_fabsf(ca.x) + __builtin_fabsf(da.x));
    return 1;
}

void GetRotationAngles(nuvec_s *direction, u16 *x_rotation, u16 *y_rotation) {
    NUVEC rotated;
    NUVEC copy = *direction;
    i32 y_angle = -NuAtan2D(copy.z, copy.x);
    NuVecRotateY(&rotated, &copy, -static_cast<i32>(static_cast<u16>(y_angle)));
    *x_rotation = -NuAtan2D(rotated.x, rotated.y);
    *y_rotation = y_angle;
}

void UnpackCharFromInt(i32 value, char &a, char &b, char &c, char &d) {
    a = static_cast<u32>(value) >> 26;
    b = static_cast<u32>(value) >> 16;
    c = static_cast<u16>(value) >> 8;
    d = value;
}

void UnpackShortFromInt(i32 value, i16 &high, i16 &low) {
    high = static_cast<u32>(value) >> 16;
    low = value;
}

void AnglesBetweenPoints(nuvec_s *from, nuvec_s *to, u16 *vertical, u16 *horizontal) {
    f32 x = to->x - from->x;
    f32 z = to->z - from->z;
    f32 y = to->y - from->y;
    *vertical = NuAtan2D(y, NuFsqrt(x * x + z * z));
    *horizontal = NuAtan2D(x, z);
}

bool LineIntersectCircle(NUVEC *origin, NUVEC *direction, NUVEC *center, f32 radius_squared) {
    f32 x = center->x - origin->x;
    f32 z = center->z - origin->z;
    f32 projection = direction->x * x + direction->z * z;
    if (projection >= 0.0f)
        return x * x + z * z - projection * projection <= radius_squared;
    return false;
}

i32 LineIntersectSphere(NUVEC *origin, NUVEC *direction, NUVEC *center, f32 radius_squared, f32 *distance_squared) {
    f32 x = center->x - origin->x;
    f32 y = center->y - origin->y;
    f32 z = center->z - origin->z;
    f32 projection = direction->x * x + direction->y * y + direction->z * z;
    if (projection < 0.0f)
        return 0;
    f32 distance = x * x + y * y + z * z - projection * projection;
    if (radius_squared < distance)
        return 0;
    if (distance_squared != NULL)
        *distance_squared = distance;
    return 1;
}

f32 LineToPlaneDistance(VuVec &origin, VuVec &direction, VuVec &plane) {
    f32 first = origin.x * plane.x + origin.y * plane.y + origin.z * plane.z + plane.w;
    f32 second = (origin.x + direction.x) * plane.x + (origin.y + direction.y) * plane.y +
                 (origin.z + direction.z) * plane.z + plane.w;
    if (first < 0.0f && second < 0.0f) {
        return first > second ? first : second;
    }
    if (first > 0.0f && second > 0.0f) {
        return first < second ? first : second;
    }
    return 0.0f;
}

f32 LineToPointDistance(VuVec &origin, VuVec &direction, VuVec &point, VuVec *closest) {
    f32 length = NuVecMag(reinterpret_cast<NUVEC *>(&direction));
    f32 inverse_length = 1.0f / length;
    f32 nx = direction.x * inverse_length;
    f32 ny = direction.y * inverse_length;
    f32 nz = direction.z * inverse_length;
    f32 projection = (point.x - origin.x) * nx + (point.y - origin.y) * ny + (point.z - origin.z) * nz;

    VuVec on_segment = origin;
    if (projection > 0.0f) {
        if (projection >= length) {
            on_segment.x += direction.x;
            on_segment.y += direction.y;
            on_segment.z += direction.z;
        } else {
            on_segment.x += nx * projection;
            on_segment.y += ny * projection;
            on_segment.z += nz * projection;
        }
        on_segment.w = 0.0f;
    }

    VuVec difference = {on_segment.x - point.x, on_segment.y - point.y, on_segment.z - point.z, 0.0f};
    f32 distance = NuVecMag(reinterpret_cast<NUVEC *>(&difference));
    if (closest != NULL)
        *closest = on_segment;
    return distance;
}

f32 RatioBetweenEdgesXZ(nuvec_s *point, nuvec_s *edge_a0, nuvec_s *edge_a1, nuvec_s *edge_b0, nuvec_s *edge_b1) {
    f32 distance_a = DistanceToLineXZ(point, edge_a0, edge_a1);
    f32 distance_b = DistanceToLineXZ(point, edge_b0, edge_b1);
    return distance_a / (distance_a + distance_b);
}

bool SphereSphereOverlap(NUVEC *a, f32 radius_a, NUVEC *b, f32 radius_b) {
    const f32 x = b->x - a->x;
    const f32 y = b->y - a->y;
    const f32 z = b->z - a->z;
    const f32 radius = radius_b + radius_a;
    return x * x + y * y + z * z <= radius * radius;
}

i32 LineToPlaneIntersecion(VuVec &origin, VuVec &direction, VuVec &plane, VuVec *intersection) {
    f32 start_distance = origin.x * plane.x + origin.y * plane.y + origin.z * plane.z + plane.w;
    f32 end_distance = (origin.x + direction.x) * plane.x + (origin.y + direction.y) * plane.y +
                       (origin.z + direction.z) * plane.z + plane.w;
    if (!(start_distance * end_distance < 0.0f))
        return 0;
    if (intersection != NULL) {
        f32 t = -start_distance / (end_distance - start_distance);
        intersection->x = direction.x * t;
        intersection->y = direction.y * t;
        intersection->z = direction.z * t;
        intersection->w = 0.0f;
        intersection->x += origin.x;
        intersection->y += origin.y;
        intersection->z += origin.z;
    }
    return 1;
}

void CalculateInterceptVector(NUVEC *origin, NUVEC *target, NUVEC *velocity, f32 speed, NUVEC *direction,
                              NUVEC *intercept) {
    NUVEC prediction = *target;
    {
        NuVecSub(direction, &prediction, origin);
        f32 distance = NuVecMag(direction);
        f32 time = distance;
        if (distance == 0.0f || speed == 0.0f)
            time = 0.0f;
        else
            time /= speed;
        NuVecAddScale(&prediction, target, velocity, time);
    }
    {
        NuVecSub(direction, &prediction, origin);
        f32 distance = NuVecMag(direction);
        f32 time = distance;
        if (distance == 0.0f || speed == 0.0f)
            time = 0.0f;
        else
            time /= speed;
        NuVecAddScale(&prediction, target, velocity, time);
    }
    {
        NuVecSub(direction, &prediction, origin);
        f32 distance = NuVecMag(direction);
        f32 time = distance;
        if (distance == 0.0f || speed == 0.0f)
            time = 0.0f;
        else
            time /= speed;
        NuVecAddScale(&prediction, target, velocity, time);
    }
    NuVecSub(direction, &prediction, origin);
    if (intercept != NULL)
        *intercept = prediction;
}

i32 LineToSphereIntersection(VuVec &origin, VuVec &direction, VuVec &center, f32 radius, VuVec *far_intersection,
                             VuVec *near_intersection) {
    f32 a = direction.x * direction.x + direction.y * direction.y + direction.z * direction.z;
    if (a < 1.1920928955078125e-7f)
        return 0;
    f32 dx = origin.x - center.x;
    f32 dy = origin.y - center.y;
    f32 dz = origin.z - center.z;
    f32 b = 2.0f * (dx * direction.x + dy * direction.y + dz * direction.z);
    f32 c = dx * dx + dy * dy + dz * dz - radius * radius;
    f32 discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f)
        return 0;
    f32 root = NuFsqrt(discriminant);
    f32 denominator = a + a;
    if (far_intersection != NULL) {
        f32 t = (root - b) / denominator;
        *far_intersection =
            VuVec(origin.x + direction.x * t, origin.y + direction.y * t, origin.z + direction.z * t, 0.0f);
    }
    if (near_intersection != NULL) {
        f32 t = (-b - root) / denominator;
        *near_intersection =
            VuVec(origin.x + direction.x * t, origin.y + direction.y * t, origin.z + direction.z * t, 0.0f);
    }
    return 1;
}

i32 MatrixReflectionVU0_AXISY(numtx_s *matrix, f32 plane, f32 override_plane, numtx_s *result) {
    if (static_cast<u8>(Reflections_On) == 0)
        return 0;
    if (override_plane != 2000000.0f) {
        if (MatrixReflection_CanOverrideFn != NULL && MatrixReflection_CanOverrideFn(plane) == 0)
            return 0;
        plane = override_plane;
    }
    *result = *matrix;
    result->m01 = -result->m01;
    result->m11 = -result->m11;
    result->m21 = -result->m21;
    result->m31 = plane - (result->m31 - plane);
    return 1;
}

i32 SphereSphereOverlapScaleY(nuvec_s *position_a, float radius_a, float y_radius_a, nuvec_s *position_b,
                              float radius_b, float y_radius_b) {
    f32 dx = position_b->x - position_a->x;
    f32 dy = position_b->y - position_a->y;
    f32 dz = position_b->z - position_a->z;
    if (radius_b != y_radius_b || radius_a != y_radius_a) {
        dy *= (radius_b + radius_a) / (y_radius_b + y_radius_a);
    }
    f32 radius = radius_b + radius_a;
    return dx * dx + dy * dy + dz * dz <= radius * radius;
}

char *IToX(char *output, i32 value) {
    char hex[] = "0123456789abcdef";
    output[0] = hex[(static_cast<u32>(value) >> 28) & 15];
    output[1] = hex[(value >> 24) & 15];
    i32 shifted = value << 8;
    output[2] = hex[(static_cast<u32>(shifted) >> 28) & 15];
    output[3] = hex[(shifted >> 24) & 15];
    i8 byte = static_cast<i8>(value >> 8);
    output[4] = hex[(byte >> 4) & 15];
    output[5] = hex[byte & 15];
    output[6] = hex[(static_cast<u32>(value) >> 4) & 15];
    output[7] = hex[value & 15];
    return output + 8;
}

i32 XToI(char *input) {
    char digit = input[0];
    i32 decimal = digit - '0';
    i32 letter = digit - 'W';
    i32 result = digit >= ':' ? letter : decimal;
    digit = input[1];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[2];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[3];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[4];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[5];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[6];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[7];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    return result;
}

i32 IsTok(char const *text, char const *token) {
    return text[0] == token[0] && text[1] == token[1] && text[2] == token[2] && text[3] == token[3];
}

void CapVec(nuvec_s *input, float maximum, nuvec_s *output) {
    f32 length_squared = input->x * input->x + input->y * input->y + input->z * input->z;
    if (length_squared > maximum * maximum) {
        f32 factor = maximum / NuFsqrt(length_squared);
        output->x *= factor;
        output->y *= factor;
        output->z *= factor;
    }
}

char *I64ToX(char *output, i64 value) {
    i32 high;
    __builtin_memcpy(&high, reinterpret_cast<const char *>(&value) + 4, sizeof(high));
    char hex[] = "0123456789abcdef";
    output[0] = hex[(static_cast<u32>(high) >> 28) & 15];
    output[1] = hex[(high >> 24) & 15];
    i32 shifted_high = high << 8;
    output[2] = hex[(static_cast<u32>(shifted_high) >> 28) & 15];
    output[3] = hex[(shifted_high >> 24) & 15];
    i8 byte_high = static_cast<i8>(high >> 8);
    output[4] = hex[(byte_high >> 4) & 15];
    output[5] = hex[byte_high & 15];
    output[6] = hex[(static_cast<u32>(high) >> 4) & 15];
    output[7] = hex[high & 15];
    i32 low;
    __builtin_memcpy(&low, &value, sizeof(low));
    output[8] = hex[(static_cast<u32>(low) >> 28) & 15];
    output[9] = hex[(low >> 24) & 15];
    i32 shifted_low = low << 8;
    output[10] = hex[(static_cast<u32>(shifted_low) >> 28) & 15];
    output[11] = hex[(shifted_low >> 24) & 15];
    i8 byte_low = static_cast<i8>(low >> 8);
    output[12] = hex[(byte_low >> 4) & 15];
    output[13] = hex[byte_low & 15];
    output[14] = hex[(static_cast<u32>(low) >> 4) & 15];
    output[15] = hex[low & 15];
    return output + 16;
}

i64 XToI64(char *input) {
    char digit = input[0];
    i32 decimal = digit - '0';
    i32 letter = digit - 'W';
    i64 result = digit >= ':' ? letter : decimal;
    digit = input[1];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[2];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[3];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[4];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[5];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[6];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[7];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[8];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[9];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[10];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[11];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[12];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[13];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[14];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    digit = input[15];
    decimal = digit - '0';
    letter = digit - 'W';
    result = (result << 4) | (digit >= ':' ? letter : decimal);
    return result;
}

i32 RotDiff(u16 current, u16 target) {
    i32 difference = static_cast<u32>(target) - static_cast<u32>(current);
    if (difference > 0x8000) {
        difference -= 0x10000;
    } else if (difference < -0x8000) {
        difference += 0x10000;
    }
    return difference;
}

static const i32 cubeEdgeIndices[12][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7},
};

i32 rawClip(VuVec const *input, VuVec *output, i32, VuVec const &plane) {
    i32 count __attribute__((aligned(16))) = 0;
    for (i32 edge = 0; edge < 12; ++edge) {
        VuVec const &a = input[cubeEdgeIndices[edge][0]];
        VuVec const &b = input[cubeEdgeIndices[edge][1]];
        f32 da = plane.x * a.x + plane.y * a.y + plane.z * a.z + plane.w;
        f32 db = plane.x * b.x + plane.y * b.y + plane.z * b.z + plane.w;
        if (da > 0.0f) {
            output[count].x = a.x;
            output[count].y = a.y;
            output[count].z = a.z;
            output[count].w = a.w;
            if (db > 0.0f) {
                count += 2;
                output[count - 1] = b;
            } else {
                count += 2;
                f32 t = da / (da - db);
                output[count - 1].w = 0.0f;
                output[count - 1].y = a.y + (b.y - a.y) * t;
                output[count - 1].z = a.z + (b.z - a.z) * t;
                output[count - 1].x = a.x + (b.x - a.x) * t;
            }
        } else if (db > 0.0f) {
            output[count].x = b.x;
            output[count].y = b.y;
            output[count].z = b.z;
            output[count].w = b.w;
            count += 2;
            f32 t = -da / (db - da);
            output[count - 1].w = 0.0f;
            output[count - 1].y = a.y + (b.y - a.y) * t;
            output[count - 1].z = a.z + (b.z - a.z) * t;
            output[count - 1].x = a.x + (b.x - a.x) * t;
        }
    }
    return count;
}

i32 getqseed() {
    return qseed;
}

i32 findrange(nugscn_s *scene, i32 first_joint) {
    // This helper's original C++ ABI names nugscn_s, but animation callers
    // pass the hierarchy object whose joint table starts at the same offsets.
    nuhgobj_s *object = reinterpret_cast<nuhgobj_s *>(scene);
    const i32 joint_count = object->joint_count;
    nuhgobjjoint_s *joints = object->joints;
    const i32 parent_index = joints[first_joint].parent_index;
    i32 end_joint = first_joint + 1;
    while (end_joint < joint_count && joints[end_joint].parent_index != parent_index) {
        ++end_joint;
    }
    return end_joint - 1;
}

static __used__ i32 MatchExtension(char *candidate, char *extension, i32 remaining) {
    while (*candidate != 0) {
        --extension;
        if (remaining == 0)
            return 0;
        char upper = toupper(static_cast<unsigned char>(*extension));
        if (*candidate != upper)
            return 0;
        ++candidate;
        --remaining;
    }
    return 1;
}

static __used__ int icomp(const void *left, const void *right) {
    const i32 *left_value = *static_cast<const i32 *const *>(left);
    const i32 *right_value = *static_cast<const i32 *const *>(right);
    return *left_value - *right_value;
}

static __used__ i32 sort32a(void const *left, void const *right) {
    const u32 *left_value = *static_cast<const u32 *const *>(left);
    const u32 *right_value = *static_cast<const u32 *const *>(right);
    return (*left_value > *right_value) - (*left_value < *right_value);
}
