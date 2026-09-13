#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nu2api_nucore_types.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/characters/motion/gameanim.h"

#include <string.h>
#include <float.h>

static const u8 KeyStructSizes[16] = {3, 4, 4, 3, 4, 3, 4, 8, 4, 8, 4, 0, 0, 0, 0, 0};
static const u8 CurveGroupMasks[3] = {2, 1, 8};

static inline f32 DecodeAni4V4Curve(const ani3_animheader_s *anim, u16 type, u32 quarter, f32 fraction, u8 *&keys,
                                    ani3_scalemin_s *&scale_min) {
    if (type != 6) {
        const u16 constant = reinterpret_cast<const u16 *>(anim->constants)[type - 16];
        return static_cast<f32>(constant) * anim->scale + anim->minimum;
    }

    const u32 first_word = *reinterpret_cast<const u32 *>(keys);
    const u32 next_word = *reinterpret_cast<const u32 *>(keys + anim->key_stride);
    const f32 first_value = static_cast<f32>(first_word & 0xff);
    const f32 next_value = static_cast<f32>(next_word & 0xff);
    const u32 tangents = first_word >> 8;
    const f32 tangent_scale = 0.01587302f;
    const f32 tangent0 = static_cast<f32>((tangents >> (quarter * 6)) & 0x3f) * tangent_scale;

    f32 packed_value;
    if (quarter == 3) {
        const f32 interpolated = (next_value - first_value) * tangent0 + first_value;
        const f32 tangent1 = static_cast<f32>((next_word >> 8) & 0x3f) * tangent_scale;
        const f32 after_value = static_cast<f32>(keys[anim->key_stride * 2]);
        const f32 next_interpolated = (after_value - next_value) * tangent1 + next_value;
        packed_value = (next_interpolated - interpolated) * fraction + interpolated;
    } else {
        const f32 tangent1 = static_cast<f32>((tangents >> (((quarter * 3 + 3) * 2) & 0x1f)) & 0x3f) * tangent_scale;
        packed_value = (next_value - first_value) * ((tangent1 - tangent0) * fraction + tangent0) + first_value;
    }

    const f32 value = packed_value * scale_min->scale + scale_min->minimum;
    keys += 4;
    ++scale_min;
    return value;
}

static inline f32 WrapAni4BlendRotation(f32 delta) {
    if (delta > 3.1415927f || delta < -3.1415927f) {
        i32 fixed_turn = static_cast<i32>(delta * 65536.0f / 6.2831855f) & 0xffff;
        if (fixed_turn >= 0x8000) {
            fixed_turn -= 0x10000;
        }
        delta = static_cast<f32>(fixed_turn) * 9.58738e-5f;
    }
    return delta;
}

static inline void SkipAni4V4Curve(u16 type, u8 *&keys, ani3_scalemin_s *&scale_min) {
    if (type < 16) {
        keys += KeyStructSizes[type];
        ++scale_min;
    }
}

static inline void GetAni4SamplePosition(const ani3_animheader_s *anim, f32 frame, u32 &quarter, f32 &fraction,
                                         i32 &key_offset) {
    if (anim->key_count == 1) {
        quarter = 0;
        fraction = 0.0f;
        key_offset = 0;
        return;
    }

    const f32 last_key = static_cast<f32>(anim->key_count - 1);
    f32 key = (frame - anim->first_frame) * last_key / static_cast<f32>(anim->frame_count - 1);
    if (key < 0.0f) {
        key = 0.0f;
    }
    if (last_key <= key) {
        key = last_key;
    }

    const i32 whole_key = static_cast<i32>(key);
    quarter = static_cast<u32>(whole_key) & 3;
    fraction = key - static_cast<f32>(whole_key);
    key_offset = (whole_key >> 2) * anim->key_stride;
}

static inline f32 DecodeAni4QuaternionScalar(const ani3_animheader_s *anim, u16 type, u32 quarter, f32 fraction,
                                             u8 *&keys, ani3_scalemin_s *&scale_min) {
    if (type == 7) {
        const f32 value = CalcValue1648(reinterpret_cast<char *>(keys), quarter, anim->key_stride, fraction, scale_min);
        keys += 8;
        ++scale_min;
        return value;
    }
    if (type == 9) {
        const u16 *samples = reinterpret_cast<const u16 *>(keys);
        const f32 first = static_cast<f32>(samples[quarter]);
        const f32 second = quarter == 3 ? static_cast<f32>(*reinterpret_cast<const u16 *>(keys + anim->key_stride))
                                        : static_cast<f32>(samples[quarter + 1]);
        const f32 value = ((second - first) * fraction + first) * scale_min->scale + scale_min->minimum;
        keys += 8;
        ++scale_min;
        return value;
    }
    return DecodeAni4V4Curve(anim, type, quarter, fraction, keys, scale_min);
}

static inline void DecodeAni4QuaternionPair(const ani3_animheader_s *anim, u16 type, u32 quarter, u8 *&keys,
                                            ani3_scalemin_s *&scale_min, f32 &first, f32 &second) {
    if (type == 7) {
        CalcValue1648Get2Values(reinterpret_cast<char *>(keys), quarter, anim->key_stride, scale_min, &first, &second);
        keys += 8;
        ++scale_min;
        return;
    }
    if (type == 9) {
        const u16 *samples = reinterpret_cast<const u16 *>(keys);
        first = static_cast<f32>(samples[quarter]) * scale_min->scale + scale_min->minimum;
        const u16 next = quarter == 3 ? *reinterpret_cast<const u16 *>(keys + anim->key_stride) : samples[quarter + 1];
        second = static_cast<f32>(next) * scale_min->scale + scale_min->minimum;
        keys += 8;
        ++scale_min;
        return;
    }
    if (type == 6) {
        u8 *pair_keys = keys;
        ani3_scalemin_s *pair_scale_min = scale_min;
        first = DecodeAni4V4Curve(anim, type, quarter, 0.0f, pair_keys, pair_scale_min);
        pair_keys = keys;
        pair_scale_min = scale_min;
        second = DecodeAni4V4Curve(anim, type, quarter, 1.0f, pair_keys, pair_scale_min);
        keys += 4;
        ++scale_min;
        return;
    }

    first = second =
        static_cast<f32>(reinterpret_cast<const u16 *>(anim->constants)[type - 16]) * anim->scale + anim->minimum;
}

static inline NUQUAT DecodeAni4Quaternion(const ani3_animheader_s *anim, i32 component_count, const u16 *types,
                                          u32 quarter, f32 fraction, u8 *&keys, ani3_scalemin_s *&scale_min) {
    NUQUAT first = {0.0f, 0.0f, 0.0f, 0.0f};
    NUQUAT second = {0.0f, 0.0f, 0.0f, 0.0f};
    f32 *first_values = &first.x;
    f32 *second_values = &second.x;
    for (i32 component = 0; component < component_count; ++component) {
        DecodeAni4QuaternionPair(anim, types[component], quarter, keys, scale_min, first_values[component],
                                 second_values[component]);
    }

    if (component_count == 3) {
        first.w = NuFsqrt(1.0f - first.x * first.x - first.y * first.y - first.z * first.z);
        second.w = NuFsqrt(1.0f - second.x * second.x - second.y * second.y - second.z * second.z);
    }

    NuQuatHarmonize(&first, &second);
    NUQUAT result;
    NuQuatLerp2(&result, &first, &second, fraction);
    NuQuatNormalise(&result, &result);
    return result;
}

static inline void SkipAni4QuaternionJoint(const ani3_animheader_s *anim, i32 quaternion_components, i32 joint,
                                           const u16 *types, u8 *&keys, ani3_scalemin_s *&scale_min) {
    const u8 flags = anim->node_flags[joint];
    if ((flags & NUANIMBUFF_JOINT_TRANSLATION) != 0) {
        for (i32 component = 0; component < 3; ++component) {
            SkipAni4V4Curve(types[component], keys, scale_min);
        }
    }
    if ((flags & NUANIMBUFF_JOINT_ROTATION) != 0) {
        for (i32 component = 0; component < quaternion_components; ++component) {
            SkipAni4V4Curve(types[3 + component], keys, scale_min);
        }
    }
    if ((flags & NUANIMBUFF_JOINT_SCALE) != 0) {
        const i32 scale_offset = 3 + quaternion_components;
        for (i32 component = 0; component < 3; ++component) {
            SkipAni4V4Curve(types[scale_offset + component], keys, scale_min);
        }
    }
}

static inline f32 DecodeAni4Quat3Scalar(const ani3_animheader_s *anim, i32 key_stride, const u16 *constants, u16 type,
                                        u32 quarter, f32 fraction, u8 *&keys, ani3_scalemin_s *&scale_min) {
    if (type != 6) {
        const u32 constant = constants[type];
        return static_cast<f32>(constant) * anim->scale + anim->minimum;
    }

    const u32 first_word = *reinterpret_cast<const u32 *>(keys);
    const u32 next_word = *reinterpret_cast<const u32 *>(keys + key_stride);
    const f32 first_value = static_cast<f32>(first_word & 0xff);
    const f32 next_value = static_cast<f32>(next_word & 0xff);
    const u32 tangents = first_word >> 8;
    const f32 tangent_scale = 0.01587302f;
    const f32 tangent0 = static_cast<f32>((tangents >> (quarter * 6)) & 0x3f) * tangent_scale;

    f32 packed_value;
    if (quarter == 3) {
        const f32 interpolated = (next_value - first_value) * tangent0 + first_value;
        const f32 tangent1 = static_cast<f32>((next_word >> 8) & 0x3f) * tangent_scale;
        const f32 after_value = static_cast<f32>(keys[key_stride * 2]);
        const f32 next_interpolated = (after_value - next_value) * tangent1 + next_value;
        packed_value = (next_interpolated - interpolated) * fraction + interpolated;
    } else {
        const f32 tangent1 = static_cast<f32>((tangents >> (((quarter * 3 + 3) * 2) & 0x1f)) & 0x3f) * tangent_scale;
        packed_value = (next_value - first_value) * ((tangent1 - tangent0) * fraction + tangent0) + first_value;
    }

    const f32 value = packed_value * scale_min->scale + scale_min->minimum;
    keys += 4;
    ++scale_min;
    return value;
}

static inline void DecodeAni4Quat3Pair(const ani3_animheader_s *anim, i32 key_stride, const u16 *constants, u16 type,
                                       u32 quarter, u8 *&keys, ani3_scalemin_s *&scale_min, f32 &first, f32 &second) {
    if (type == 7) {
        CalcValue1648Get2Values(reinterpret_cast<char *>(keys), quarter, key_stride, scale_min, &first, &second);
        keys += 8;
        ++scale_min;
        return;
    }
    if (type == 9) {
        const u16 *samples = reinterpret_cast<const u16 *>(keys);
        first = static_cast<f32>(samples[quarter]) * scale_min->scale + scale_min->minimum;
        const u16 next = quarter == 3 ? *reinterpret_cast<const u16 *>(keys + key_stride) : samples[quarter + 1];
        second = static_cast<f32>(next) * scale_min->scale + scale_min->minimum;
        keys += 8;
        ++scale_min;
        return;
    }
    if (type == 6) {
        const u32 first_word = *reinterpret_cast<const u32 *>(keys);
        const u32 next_word = *reinterpret_cast<const u32 *>(keys + key_stride);
        const f32 start = static_cast<f32>(first_word & 0xff);
        const f32 next = static_cast<f32>(next_word & 0xff);
        const u32 tangents = first_word >> 8;
        const f32 tangent0 = static_cast<f32>((tangents >> (quarter * 6)) & 0x3f) * 0.01587302f;
        first = ((next - start) * tangent0 + start) * scale_min->scale + scale_min->minimum;
        if (quarter == 3) {
            const f32 tangent1 = static_cast<f32>((next_word >> 8) & 0x3f) * 0.01587302f;
            const f32 after = static_cast<f32>(keys[key_stride * 2]);
            second = ((after - next) * tangent1 + next) * scale_min->scale + scale_min->minimum;
        } else {
            const f32 tangent1 = static_cast<f32>((tangents >> (((quarter * 3 + 3) * 2) & 0x1f)) & 0x3f) * 0.01587302f;
            second = ((next - start) * tangent1 + start) * scale_min->scale + scale_min->minimum;
        }
        keys += 4;
        ++scale_min;
        return;
    }

    first = second = static_cast<f32>(static_cast<u32>(constants[type])) * anim->scale + anim->minimum;
}

static inline void DecodeAni4BlendQuat3Pair(const ani3_animheader_s *anim, i32 key_stride, const u16 *constants,
                                            u16 type, u32 quarter, u8 *&keys, ani3_scalemin_s *&scale_min, f32 &first,
                                            f32 &second) {
    if (type == 6) {
        const u32 first_word = *reinterpret_cast<const u32 *>(keys);
        const u32 next_word = *reinterpret_cast<const u32 *>(keys + key_stride);
        const f32 start = static_cast<f32>(first_word & 0xff);
        const f32 next = static_cast<f32>(next_word & 0xff);
        const u32 tangents = first_word >> 8;
        const f32 tangent0 = static_cast<f32>((tangents >> (quarter * 6)) & 0x3f) * 0.01587302f;
        first = ((next - start) * tangent0 + start) * scale_min->scale + scale_min->minimum;
        if (quarter == 3) {
            const f32 tangent1 = static_cast<f32>((next_word >> 8) & 0x3f) * 0.01587302f;
            const f32 after = static_cast<f32>(keys[key_stride * 2]);
            second = ((after - next) * tangent1 + next) * scale_min->scale + scale_min->minimum;
        } else {
            const f32 tangent1 = static_cast<f32>((tangents >> (((quarter * 3 + 3) * 2) & 0x1f)) & 0x3f) * 0.01587302f;
            second = ((next - start) * tangent1 + start) * scale_min->scale + scale_min->minimum;
        }
        keys += 4;
        ++scale_min;
        return;
    }

    if (type == 7) {
        CalcValue1648Get2Values(reinterpret_cast<char *>(keys), quarter, key_stride, scale_min, &first, &second);
        keys += 8;
        ++scale_min;
        return;
    }
    first = second = static_cast<f32>(static_cast<u32>(constants[type])) * anim->scale + anim->minimum;
}

static inline void DecodeAni4BlendQuat3WPair(const ani3_animheader_s *anim, i32 key_stride, const u16 *constants,
                                             u16 type, u32 quarter, u8 *&keys, ani3_scalemin_s *&scale_min, f32 &first,
                                             f32 &second) {
    if (type == 6) {
        const u32 first_word = *reinterpret_cast<const u32 *>(keys);
        const u32 next_word = *reinterpret_cast<const u32 *>(keys + key_stride);
        const f32 start = static_cast<f32>(first_word & 0xff);
        const f32 next = static_cast<f32>(next_word & 0xff);
        const u32 tangents = first_word >> 8;
        const f32 tangent0 = static_cast<f32>((tangents >> (quarter * 6)) & 0x3f) * 0.01587302f;
        first = ((next - start) * tangent0 + start) * scale_min->scale + scale_min->minimum;
        if (quarter == 3) {
            const f32 tangent1 = static_cast<f32>((next_word >> 8) & 0x3f) * 0.01587302f;
            const f32 after = static_cast<f32>(keys[key_stride * 2]);
            second = ((after - next) * tangent1 + next) * scale_min->scale + scale_min->minimum;
        } else {
            const f32 tangent1 = static_cast<f32>((tangents >> (((quarter * 3 + 3) * 2) & 0x1f)) & 0x3f) * 0.01587302f;
            second = ((next - start) * tangent1 + start) * scale_min->scale + scale_min->minimum;
        }
        keys += 4;
        ++scale_min;
        return;
    }

    if (type == 7) {
        CalcValue1648Get2Values(reinterpret_cast<char *>(keys), quarter, key_stride, scale_min, &first, &second);
        keys += 8;
        ++scale_min;
        return;
    }
    first = second = static_cast<f32>(static_cast<u32>(constants[type])) * anim->scale + anim->minimum;
}

float CalcValue1648(char *data, i32 quarter, i32 stride, float fraction, ani3_scalemin_s *scale_min) {
    u16 *next = reinterpret_cast<u16 *>(data + stride);
    u16 *keys = reinterpret_cast<u16 *>(data);

    switch (quarter) {
        case 0: {
            float tangent = static_cast<float>((keys[2] & 0xfff) - (keys[1] & 0xfff)) * fraction +
                            static_cast<float>(keys[1] & 0xfff);
            float value =
                static_cast<float>(static_cast<i32>(*next) - static_cast<i32>(keys[0])) * (tangent / 4095.0f) +
                static_cast<float>(static_cast<i32>(keys[0]));
            return value * scale_min->scale + scale_min->minimum;
        }
        case 1: {
            float tangent = static_cast<float>((keys[3] & 0xfff) - (keys[2] & 0xfff)) * fraction +
                            static_cast<float>(keys[2] & 0xfff);
            float value =
                static_cast<float>(static_cast<i32>(*next) - static_cast<i32>(keys[0])) * (tangent / 4095.0f) +
                static_cast<float>(static_cast<i32>(keys[0]));
            return value * scale_min->scale + scale_min->minimum;
        }
        case 2: {
            i32 start = keys[3] & 0xfff;
            i32 end = ((keys[2] & 0xf000) >> 8) | ((keys[3] & 0xf000) >> 4) | (keys[1] >> 12);
            float tangent = static_cast<float>(end - start) * fraction + static_cast<float>(start);
            float value =
                static_cast<float>(static_cast<i32>(*next) - static_cast<i32>(keys[0])) * (tangent / 4095.0f) +
                static_cast<float>(static_cast<i32>(keys[0]));
            return value * scale_min->scale + scale_min->minimum;
        }
        case 3: {
            i32 next_value = *next;
            i32 tangent = ((keys[2] & 0xf000) >> 8) | ((keys[3] & 0xf000) >> 4) | (keys[1] >> 12);
            float a =
                static_cast<float>(next_value - static_cast<i32>(keys[0])) * static_cast<float>(tangent) / 4095.0f +
                static_cast<float>(static_cast<i32>(keys[0]));
            float b = static_cast<float>(static_cast<i32>(reinterpret_cast<u16 *>(data)[stride]) - next_value) *
                          static_cast<float>(next[1] & 0xfff) / 4095.0f +
                      static_cast<float>(next_value);
            float value = (b - a) * fraction + a;
            return value * scale_min->scale + scale_min->minimum;
        }
    }

    return 0.0f;
}

void CalcValue1648Get2Values(char *data, i32 quarter, i32 stride, ani3_scalemin_s *scale_min, float *first,
                             float *second) {
    u16 *next = reinterpret_cast<u16 *>(data + stride);
    u16 *keys = reinterpret_cast<u16 *>(data);
    i32 start, delta, end, last;
    switch (quarter) {
        case 0: {
            start = keys[0];
            delta = static_cast<i32>(next[0]) - start;
            *first = (static_cast<f32>(keys[1] & 0xfff) * static_cast<f32>(delta) / 4095.0f + static_cast<f32>(start)) *
                         scale_min->scale +
                     scale_min->minimum;
            *second =
                (static_cast<f32>(keys[2] & 0xfff) * static_cast<f32>(delta) / 4095.0f + static_cast<f32>(start)) *
                    scale_min->scale +
                scale_min->minimum;
            break;
        }
        case 1: {
            start = keys[0];
            delta = static_cast<i32>(next[0]) - start;
            *first = (static_cast<f32>(keys[2] & 0xfff) * static_cast<f32>(delta) / 4095.0f + static_cast<f32>(start)) *
                         scale_min->scale +
                     scale_min->minimum;
            *second =
                (static_cast<f32>(keys[3] & 0xfff) * static_cast<f32>(delta) / 4095.0f + static_cast<f32>(start)) *
                    scale_min->scale +
                scale_min->minimum;
            break;
        }
        case 2: {
            start = keys[0];
            delta = static_cast<i32>(next[0]) - start;
            *first = (static_cast<f32>(keys[3] & 0xfff) * static_cast<f32>(delta) / 4095.0f + static_cast<f32>(start)) *
                         scale_min->scale +
                     scale_min->minimum;
            last = ((keys[2] & 0xf000) >> 8) | ((keys[3] & 0xf000) >> 4) | (keys[1] >> 12);
            *second = (static_cast<f32>(last) * static_cast<f32>(delta) / 4095.0f + static_cast<f32>(start)) *
                          scale_min->scale +
                      scale_min->minimum;
            break;
        }
        case 3: {
            last = ((keys[2] & 0xf000) >> 8) | ((keys[3] & 0xf000) >> 4) | (keys[1] >> 12);
            start = keys[0];
            end = next[0];
            *first = (static_cast<f32>(last) * static_cast<f32>(end - start) / 4095.0f + static_cast<f32>(start)) *
                         scale_min->scale +
                     scale_min->minimum;
            *second = (static_cast<f32>(static_cast<i32>(next[static_cast<u32>(stride) / sizeof(u16)]) - end) *
                           static_cast<f32>(next[1] & 0xfff) / 4095.0f +
                       static_cast<f32>(end)) *
                          scale_min->scale +
                      scale_min->minimum;
            break;
        }
    }
}

i32 ANI_SimpleAni3PlayerV4Joint_Quat3(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, i32 joint_count,
                                      i32 first_joint) {
    const u8 *node_flags = anim->node_flags;
    buffer->use_quaternions = 1;
    u32 quarter;
    f32 fraction;
    i32 key_offset;
    if (anim->key_count == 1) {
        quarter = 0;
        fraction = 0.0f;
        key_offset = 0;
    } else {
        const i32 last = anim->key_count - 1;
        f32 key = (frame - anim->first_frame) * static_cast<f32>(last) / static_cast<f32>(anim->frame_count - 1);
        if (key < 0.0f)
            key = 0.0f;
        i32 whole;
        if (static_cast<f32>(last) <= key) {
            whole = last;
            fraction = 0.0f;
        } else {
            whole = static_cast<i32>(key);
            fraction = key - static_cast<f32>(whole);
        }
        quarter = static_cast<u32>(whole) & 3;
        key_offset = (whole >> 2) * anim->key_stride;
    }
    u8 *keys = anim->keys + key_offset;
    i32 key_stride = anim->key_stride;
    const u16 *constants = reinterpret_cast<const u16 *>(anim->constants) - 16;
    ani3_scalemin_s *scale_min = anim->scale_min;
    const u16 *curve_types = anim->curve_types;
    i32 count = joint_count < anim->node_count ? joint_count : anim->node_count;
    const u8 *first_flags = node_flags + first_joint;
    for (const u8 *skip_flags = node_flags; skip_flags < first_flags; ++skip_flags) {
        if (*skip_flags & 2) {
            SkipAni4V4Curve(curve_types[0], keys, scale_min);
            SkipAni4V4Curve(curve_types[1], keys, scale_min);
            SkipAni4V4Curve(curve_types[2], keys, scale_min);
        }
        if (*skip_flags & 1) {
            SkipAni4V4Curve(curve_types[3], keys, scale_min);
            SkipAni4V4Curve(curve_types[4], keys, scale_min);
            SkipAni4V4Curve(curve_types[5], keys, scale_min);
        }
        if (*skip_flags & 8) {
            SkipAni4V4Curve(curve_types[6], keys, scale_min);
            SkipAni4V4Curve(curve_types[7], keys, scale_min);
            SkipAni4V4Curve(curve_types[8], keys, scale_min);
        }
        curve_types += 9;
    }
    node_flags += first_joint;
    const u8 *end_flags = node_flags + count;
    u8 *joint_flags = buffer->joint_flags + first_joint;
    nuanimbuffjoint_s *joint = buffer->joints + first_joint;
    for (; node_flags < end_flags; ++node_flags, ++joint) {
        const u8 flags = *node_flags;
        *joint_flags++ = flags;
        f32 *output = &joint->translation.x;
        for (i32 group = 0; group < 3; ++group, curve_types += 3, output += 4) {
            if (!(flags & CurveGroupMasks[group])) {
                if (group == 0) {
                    output[0] = output[1] = output[2] = 0.0f;
                } else if (group == 1) {
                    output[0] = output[1] = output[2] = 0.0f;
                    output[3] = 1.0f;
                } else {
                    output[0] = output[1] = output[2] = 1.0f;
                }
            } else if (group != 1) {
                output[0] = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[0], quarter, fraction, keys,
                                                  scale_min);
                output[1] = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[1], quarter, fraction, keys,
                                                  scale_min);
                output[2] = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[2], quarter, fraction, keys,
                                                  scale_min);
            } else {
                NUQUAT first, second;
                DecodeAni4Quat3Pair(anim, key_stride, constants, curve_types[0], quarter, keys, scale_min, first.x,
                                    second.x);
                DecodeAni4Quat3Pair(anim, key_stride, constants, curve_types[1], quarter, keys, scale_min, first.y,
                                    second.y);
                DecodeAni4Quat3Pair(anim, key_stride, constants, curve_types[2], quarter, keys, scale_min, first.z,
                                    second.z);
                first.w = NuFsqrt(1.0f - (first.x * first.x + first.y * first.y + first.z * first.z));
                second.w = NuFsqrt(1.0f - (second.x * second.x + second.y * second.y + second.z * second.z));
                if (first.x * second.x + first.y * second.y + first.z * second.z + first.w * second.w < 0.0f) {
                    second.x = -second.x;
                    second.y = -second.y;
                    second.z = -second.z;
                    second.w = -second.w;
                }
                NUQUAT result;
                result.x = first.x * (1.0f - fraction) + second.x * fraction;
                result.y = first.y * (1.0f - fraction) + second.y * fraction;
                result.z = first.z * (1.0f - fraction) + second.z * fraction;
                result.w = first.w * (1.0f - fraction) + second.w * fraction;
                f32 length =
                    NuFsqrt(result.w * result.w + result.x * result.x + result.y * result.y + result.z * result.z);
                f32 inverse_length = length == 0.0f ? 0.0f : 1.0f / length;
                output[0] = result.x * inverse_length;
                output[1] = result.y * inverse_length;
                output[2] = result.z * inverse_length;
                output[3] = result.w * inverse_length;
            }
        }
    }
    return 0;
}

i32 ANI_SimpleAni3PlayerV4Joint_Quat3W(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, i32 joint_count,
                                       i32 first_joint) {

    const u8 *node_flags = anim->node_flags;
    buffer->use_quaternions = 1;
    u32 quarter;
    f32 fraction;
    i32 key_offset;
    if (anim->key_count == 1) {
        quarter = 0;
        fraction = 0.0f;
        key_offset = 0;
    } else {
        const i32 last = anim->key_count - 1;
        f32 key = (frame - anim->first_frame) * static_cast<f32>(last) / static_cast<f32>(anim->frame_count - 1);
        if (key < 0.0f)
            key = 0.0f;
        i32 whole;
        if (static_cast<f32>(last) <= key) {
            whole = last;
            fraction = 0.0f;
        } else {
            whole = static_cast<i32>(key);
            fraction = key - static_cast<f32>(whole);
        }
        quarter = static_cast<u32>(whole) & 3;
        key_offset = (whole / 4) * anim->key_stride;
    }
    u8 *keys = anim->keys + key_offset;
    i32 key_stride = anim->key_stride;
    const u16 *constants = reinterpret_cast<const u16 *>(anim->constants) - 16;
    ani3_scalemin_s *scale_min = anim->scale_min;
    const u16 *curve_types = anim->curve_types;
    // The original clamps the count before applying first_joint.
    i32 count = joint_count < anim->node_count ? joint_count : anim->node_count;
    const u8 *first_flags = node_flags + first_joint;
    for (const u8 *skip_flags = node_flags; skip_flags < first_flags; ++skip_flags) {
        for (i32 group = 0; group < 3; ++group) {
            i32 components = group == 1 ? 4 : 3;
            if (*skip_flags & CurveGroupMasks[group]) {
                for (i32 component = 0; component < components; ++component) {
                    SkipAni4V4Curve(*curve_types++, keys, scale_min);
                }
            } else
                curve_types += components;
        }
    }
    node_flags += first_joint;
    const u8 *end_flags = node_flags + count;
    u8 *joint_flags = buffer->joint_flags + first_joint;
    nuanimbuffjoint_s *joint = buffer->joints + first_joint;
    for (; node_flags < end_flags; ++node_flags, ++joint) {
        const u8 flags = *node_flags;
        *joint_flags++ = flags;
        f32 *output = &joint->translation.x;
        for (i32 group = 0; group < 3; ++group, output += 4) {
            if (!(flags & CurveGroupMasks[group])) {
                if (group == 0) {
                    output[0] = output[1] = output[2] = 0.0f;
                } else if (group == 1) {
                    output[0] = output[1] = output[2] = 0.0f;
                    output[3] = 1.0f;
                } else {
                    output[0] = output[1] = output[2] = 1.0f;
                }
                curve_types += group == 1 ? 4 : 3;
            } else if (group != 1) {
                output[0] = DecodeAni4Quat3Scalar(anim, key_stride, constants, *curve_types++, quarter, fraction, keys,
                                                  scale_min);
                output[1] = DecodeAni4Quat3Scalar(anim, key_stride, constants, *curve_types++, quarter, fraction, keys,
                                                  scale_min);
                output[2] = DecodeAni4Quat3Scalar(anim, key_stride, constants, *curve_types++, quarter, fraction, keys,
                                                  scale_min);
            } else {
                // This format stores w explicitly and interpolates the stored signs.
                NUQUAT first, second;
                DecodeAni4Quat3Pair(anim, key_stride, constants, *curve_types++, quarter, keys, scale_min, first.x,
                                    second.x);
                DecodeAni4Quat3Pair(anim, key_stride, constants, *curve_types++, quarter, keys, scale_min, first.y,
                                    second.y);
                DecodeAni4Quat3Pair(anim, key_stride, constants, *curve_types++, quarter, keys, scale_min, first.z,
                                    second.z);
                DecodeAni4Quat3Pair(anim, key_stride, constants, *curve_types++, quarter, keys, scale_min, first.w,
                                    second.w);
                NUQUAT result;
                result.x = first.x * (1.0f - fraction) + second.x * fraction;
                result.y = first.y * (1.0f - fraction) + second.y * fraction;
                result.z = first.z * (1.0f - fraction) + second.z * fraction;
                result.w = first.w * (1.0f - fraction) + second.w * fraction;
                f32 length =
                    NuFsqrt(result.w * result.w + result.x * result.x + result.y * result.y + result.z * result.z);
                f32 inverse_length = length == 0.0f ? 0.0f : 1.0f / length;
                NUQUAT normalized = {result.x * inverse_length, result.y * inverse_length, result.z * inverse_length,
                                     result.w * inverse_length};
                *reinterpret_cast<NUQUAT *>(output) = normalized;
            }
        }
    }
    return 0;
}

extern "C" i32 ANI_SimpleAni3PlayerV4Joint_EulerQuat(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer,
                                                     i32 joint_count, i32 first_joint) {
    buffer->use_quaternions = 1;

    u32 quarter;
    f32 fraction;
    i32 key_offset;
    GetAni4SamplePosition(anim, frame, quarter, fraction, key_offset);

    u8 *keys = anim->keys + key_offset;
    ani3_scalemin_s *scale_min = anim->scale_min;
    const u16 *curve_types = anim->curve_types;
    for (i32 joint = 0; joint < first_joint; ++joint) {
        const u8 flags = anim->node_flags[joint];
        for (i32 group = 0; group < 3; ++group) {
            if ((flags & CurveGroupMasks[group]) != 0) {
                for (i32 component = 0; component < 3; ++component) {
                    SkipAni4V4Curve(curve_types[group * 3 + component], keys, scale_min);
                }
            }
        }
        curve_types += 9;
    }

    i32 end_joint = first_joint + joint_count;
    if (end_joint > anim->node_count) {
        end_joint = anim->node_count;
    }
    for (i32 joint_index = first_joint; joint_index < end_joint; ++joint_index) {
        const u8 flags = anim->node_flags[joint_index];
        buffer->joint_flags[joint_index] = flags;
        nuanimbuffjoint_s &joint = buffer->joints[joint_index];

        if ((flags & NUANIMBUFF_JOINT_TRANSLATION) != 0) {
            f32 *translation = &joint.translation.x;
            translation[0] = DecodeAni4V4Curve(anim, curve_types[0], quarter, fraction, keys, scale_min);
            translation[1] = DecodeAni4V4Curve(anim, curve_types[1], quarter, fraction, keys, scale_min);
            translation[2] = DecodeAni4V4Curve(anim, curve_types[2], quarter, fraction, keys, scale_min);
        } else {
            joint.translation = {0.0f, 0.0f, 0.0f};
        }

        NUQUAT *rotation = reinterpret_cast<NUQUAT *>(&joint.rotation);
        if ((flags & NUANIMBUFF_JOINT_ROTATION) != 0) {
            f32 euler[3];
            for (i32 component = 0; component < 3; ++component) {
                euler[component] =
                    DecodeAni4V4Curve(anim, curve_types[3 + component], quarter, fraction, keys, scale_min);
            }
            NuQuatFromEulerXYZ(rotation, static_cast<NUANG>(euler[0] * 10430.378f),
                               static_cast<NUANG>(euler[1] * 10430.378f), static_cast<NUANG>(euler[2] * 10430.378f));
        } else {
            *rotation = {0.0f, 0.0f, 0.0f, 1.0f};
        }

        if ((flags & NUANIMBUFF_JOINT_SCALE) != 0) {
            f32 *scale = &joint.scale.x;
            for (i32 component = 0; component < 3; ++component) {
                scale[component] =
                    DecodeAni4V4Curve(anim, curve_types[6 + component], quarter, fraction, keys, scale_min);
            }
        } else {
            joint.scale = {1.0f, 1.0f, 1.0f};
        }

        curve_types += 9;
    }
    return 0;
}

extern "C" i32 ANI_SimpleAni3PlayerV4Joint(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, i32 joint_count,
                                           i32 first_joint) {
    if ((anim->format_flags & ANI3_FORMAT_QUATERNION_ROTATION) != 0) {
        if ((anim->format_flags & ANI3_FORMAT_QUATERNION_STORES_W) != 0) {
            return ANI_SimpleAni3PlayerV4Joint_Quat3W(anim, frame, buffer, joint_count, first_joint);
        }
        return ANI_SimpleAni3PlayerV4Joint_Quat3(anim, frame, buffer, joint_count, first_joint);
    }
    if (ForceEulerToQuat != 0) {
        return ANI_SimpleAni3PlayerV4Joint_EulerQuat(anim, frame, buffer, joint_count, first_joint);
    }

    buffer->use_quaternions = 0;

    u32 quarter;
    f32 fraction;
    i32 key_offset;
    if (anim->key_count == 1) {
        quarter = 0;
        fraction = 0.0f;
        key_offset = 0;
    } else {
        const f32 last_key = static_cast<f32>(anim->key_count - 1);
        f32 key = (frame - anim->first_frame) * last_key / static_cast<f32>(anim->frame_count - 1);
        if (key < 0.0f) {
            key = 0.0f;
        }
        if (last_key <= key) {
            key = last_key;
        }

        const i32 whole_key = static_cast<i32>(key);
        fraction = key - static_cast<f32>(whole_key);
        quarter = static_cast<u32>(whole_key) & 3;
        key_offset = (whole_key >> 2) * anim->key_stride;
    }

    u8 *keys = anim->keys + key_offset;
    ani3_scalemin_s *scale_min = anim->scale_min;
    const u16 *curve_types = anim->curve_types;

    // Bring all three packed-data cursors to the requested first joint.
    for (i32 joint = 0; joint < first_joint; ++joint) {
        const u8 flags = anim->node_flags[joint];
        for (i32 group = 0; group < 3; ++group) {
            if ((flags & CurveGroupMasks[group]) == 0) {
                continue;
            }
            for (i32 component = 0; component < 3; ++component) {
                if (curve_types[group * 3 + component] < 16) {
                    keys += 4;
                    ++scale_min;
                }
            }
        }
        curve_types += 9;
    }

    const i32 decode_count = joint_count <= anim->node_count ? joint_count : anim->node_count;
    const i32 end_joint = first_joint + decode_count;
    for (i32 joint_index = first_joint; joint_index < end_joint; ++joint_index) {
        const u8 flags = anim->node_flags[joint_index];
        buffer->joint_flags[joint_index] = flags;
        f32 *group_values = reinterpret_cast<f32 *>(&buffer->joints[joint_index]);

        for (i32 group = 0; group < 3; ++group) {
            if ((flags & CurveGroupMasks[group]) == 0) {
                const f32 default_value = group == 2 ? 1.0f : 0.0f;
                group_values[0] = default_value;
                group_values[1] = default_value;
                group_values[2] = default_value;
            } else {
                group_values[0] = DecodeAni4V4Curve(anim, curve_types[0], quarter, fraction, keys, scale_min);
                group_values[1] = DecodeAni4V4Curve(anim, curve_types[1], quarter, fraction, keys, scale_min);
                group_values[2] = DecodeAni4V4Curve(anim, curve_types[2], quarter, fraction, keys, scale_min);
            }

            curve_types += 3;
            group_values += 4;
        }
    }
    return 0;
}

i32 ANI_SimpleAni3PlayerV4Joint_Blend_Quat3(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, f32 blend,
                                            i32 joint_count, i32 first_joint, NUVEC *root_translation) {
    const u8 *node_flags = anim->node_flags;
    const f32 inverse_blend = 1.0f - blend;
    f32 key =
        (frame - anim->first_frame) * static_cast<f32>(anim->key_count - 1) / static_cast<f32>(anim->frame_count - 1);
    if (key < 0.0f)
        key = 0.0f;
    if (static_cast<f32>(anim->key_count) <= key)
        key = static_cast<f32>(anim->key_count - 1);
    const i32 whole = static_cast<i32>(key);
    const u32 quarter = static_cast<u32>(whole) & 3;
    const f32 fraction = key - static_cast<f32>(whole);
    const i32 key_stride = anim->key_stride;
    u8 *keys = anim->keys + (whole / 4) * key_stride;
    ani3_scalemin_s *scale_min = anim->scale_min;
    const u16 *constants = reinterpret_cast<const u16 *>(anim->constants) - 16;
    const u16 *curve_types = anim->curve_types;
    const i32 count = joint_count < anim->node_count ? joint_count : anim->node_count;
    if (first_joint != 0)
        root_translation = NULL;
    const u8 *first_flags = node_flags + first_joint;
    for (const u8 *skip_flags = node_flags; skip_flags < first_flags; ++skip_flags) {
        if (*skip_flags & 2) {
            SkipAni4V4Curve(curve_types[0], keys, scale_min);
            SkipAni4V4Curve(curve_types[1], keys, scale_min);
            SkipAni4V4Curve(curve_types[2], keys, scale_min);
        }
        if (*skip_flags & 1) {
            SkipAni4V4Curve(curve_types[3], keys, scale_min);
            SkipAni4V4Curve(curve_types[4], keys, scale_min);
            SkipAni4V4Curve(curve_types[5], keys, scale_min);
        }
        if (*skip_flags & 8) {
            SkipAni4V4Curve(curve_types[6], keys, scale_min);
            SkipAni4V4Curve(curve_types[7], keys, scale_min);
            SkipAni4V4Curve(curve_types[8], keys, scale_min);
        }
        curve_types += 9;
    }
    node_flags += first_joint;
    const u8 *end_flags = node_flags + count;
    u8 *joint_flags = buffer->joint_flags + first_joint;
    f32 *output = &buffer->joints[first_joint].translation.x;
    for (; node_flags < end_flags; ++node_flags) {
        const u8 flags = *node_flags;
        *joint_flags++ |= flags;
        for (i32 group = 0; group < 3; ++group, curve_types += 3, output += 4) {
            if (!(flags & CurveGroupMasks[group])) {
                if (group == 0) {
                    output[0] *= inverse_blend;
                    output[1] *= inverse_blend;
                    output[2] *= inverse_blend;
                    if (root_translation) {
                        root_translation->x = root_translation->y = root_translation->z = 0.0f;
                        root_translation = NULL;
                    }
                } else if (group == 1) {
                    const f32 blended_w = output[3] * inverse_blend + blend;
                    output[0] *= inverse_blend;
                    output[1] *= inverse_blend;
                    output[3] = blended_w;
                    output[2] *= inverse_blend;
                    f32 length = NuFsqrt(output[3] * output[3] + output[0] * output[0] + output[1] * output[1] +
                                         output[2] * output[2]);
                    f32 inverse_length = length == 0.0f ? 0.0f : 1.0f / length;
                    output[3] *= inverse_length;
                    output[0] *= inverse_length;
                    output[1] *= inverse_length;
                    output[2] *= inverse_length;
                } else {
                    output[0] = output[0] * inverse_blend + blend;
                    output[1] = output[1] * inverse_blend + blend;
                    output[2] = output[2] * inverse_blend + blend;
                }
            } else if (group != 1) {
                f32 sampled = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[0], quarter, fraction,
                                                    keys, scale_min);
                f32 delta = sampled - output[0];
                if (root_translation)
                    root_translation->x = sampled;
                output[0] += delta * blend;
                sampled = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[1], quarter, fraction, keys,
                                                scale_min);
                delta = sampled - output[1];
                if (root_translation)
                    root_translation->y = sampled;
                output[1] += delta * blend;
                sampled = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[2], quarter, fraction, keys,
                                                scale_min);
                delta = sampled - output[2];
                if (root_translation)
                    root_translation->z = -sampled;
                output[2] += delta * blend;
                root_translation = NULL;
            } else {
                NUQUAT first, second, result;
                DecodeAni4BlendQuat3Pair(anim, key_stride, constants, curve_types[0], quarter, keys, scale_min, first.x,
                                         second.x);
                DecodeAni4BlendQuat3Pair(anim, key_stride, constants, curve_types[1], quarter, keys, scale_min, first.y,
                                         second.y);
                DecodeAni4BlendQuat3Pair(anim, key_stride, constants, curve_types[2], quarter, keys, scale_min, first.z,
                                         second.z);
                first.w = NuFsqrt(1.0f - (first.x * first.x + first.y * first.y + first.z * first.z));
                second.w = NuFsqrt(1.0f - (second.x * second.x + second.y * second.y + second.z * second.z));
                NuQuatHarmonize(&first, &second);
                result.x = first.x * (1.0f - fraction) + second.x * fraction;
                result.y = first.y * (1.0f - fraction) + second.y * fraction;
                result.z = first.z * (1.0f - fraction) + second.z * fraction;
                result.w = first.w * (1.0f - fraction) + second.w * fraction;
                f32 length =
                    NuFsqrt(result.w * result.w + result.x * result.x + result.y * result.y + result.z * result.z);
                f32 inverse_length = length == 0.0f ? 0.0f : 1.0f / length;
                result.w *= inverse_length;
                result.x *= inverse_length;
                result.y *= inverse_length;
                result.z *= inverse_length;
                VuQuatSlerpFast(reinterpret_cast<NUQUAT *>(output), reinterpret_cast<NUQUAT *>(output), &result, blend);
            }
        }
    }
    return 0;
}

i32 ANI_SimpleAni3PlayerV4Joint_Blend_Quat3W(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, f32 blend,
                                             i32 joint_count, i32 first_joint, NUVEC *root_translation) {
    const u8 *node_flags = anim->node_flags;
    const f32 inverse_blend = 1.0f - blend;
    f32 key =
        (frame - anim->first_frame) * static_cast<f32>(anim->key_count - 1) / static_cast<f32>(anim->frame_count - 1);
    if (key < 0.0f)
        key = 0.0f;
    // Preserve the fractional interval after the last integer key.
    if (static_cast<f32>(anim->key_count) <= key)
        key = static_cast<f32>(anim->key_count - 1);
    const i32 whole = static_cast<i32>(key);
    const u32 quarter = static_cast<u32>(whole) & 3;
    const f32 fraction = key - static_cast<f32>(whole);
    const i32 key_stride = anim->key_stride;
    u8 *keys = anim->keys + (whole / 4) * key_stride;
    ani3_scalemin_s *scale_min = anim->scale_min;
    const u16 *constants = reinterpret_cast<const u16 *>(anim->constants) - 16;
    const u16 *curve_types = anim->curve_types;
    const i32 count = joint_count < anim->node_count ? joint_count : anim->node_count;
    if (first_joint != 0)
        root_translation = NULL;
    const u8 *first_flags = node_flags + first_joint;
    for (const u8 *skip_flags = node_flags; skip_flags < first_flags; ++skip_flags) {
        for (i32 group = 0; group < 3; ++group) {
            i32 components = group == 1 ? 4 : 3;
            if (*skip_flags & CurveGroupMasks[group]) {
                for (i32 component = 0; component < components; ++component) {
                    SkipAni4V4Curve(*curve_types++, keys, scale_min);
                }
            } else
                curve_types += components;
        }
    }
    node_flags += first_joint;
    const u8 *end_flags = node_flags + count;
    u8 *joint_flags = buffer->joint_flags + first_joint;
    nuanimbuffjoint_s *joint = buffer->joints + first_joint;
    for (; node_flags < end_flags; ++node_flags, ++joint) {
        const u8 flags = *node_flags;
        *joint_flags++ |= flags;
        f32 *output = &joint->translation.x;
        for (i32 group = 0; group < 3; ++group, output += 4) {
            if (!(flags & CurveGroupMasks[group])) {
                if (group == 0) {
                    output[0] *= inverse_blend;
                    output[1] *= inverse_blend;
                    output[2] *= inverse_blend;
                    if (root_translation) {
                        root_translation->x = root_translation->y = root_translation->z = 0.0f;
                        root_translation = NULL;
                    }
                } else if (group == 1) {
                    output[3] = output[3] * inverse_blend + blend;
                    output[0] *= inverse_blend;
                    output[1] *= inverse_blend;
                    output[2] *= inverse_blend;
                    f32 length = NuFsqrt(output[3] * output[3] + output[0] * output[0] + output[1] * output[1] +
                                         output[2] * output[2]);
                    f32 inverse_length = length == 0.0f ? 0.0f : 1.0f / length;
                    output[3] *= inverse_length;
                    output[0] *= inverse_length;
                    output[1] *= inverse_length;
                    output[2] *= inverse_length;
                } else {
                    output[0] = output[0] * inverse_blend + blend;
                    output[1] = output[1] * inverse_blend + blend;
                    output[2] = output[2] * inverse_blend + blend;
                }
                curve_types += group == 1 ? 4 : 3;
            } else if (group != 1) {
                f32 sampled = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[0], quarter, fraction,
                                                    keys, scale_min);
                f32 delta_x = sampled - output[0];
                if (root_translation)
                    root_translation->x = sampled;
                output[0] += delta_x * blend;
                sampled = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[1], quarter, fraction, keys,
                                                scale_min);
                f32 delta_y = sampled - output[1];
                if (root_translation)
                    root_translation->y = sampled;
                output[1] += delta_y * blend;
                sampled = DecodeAni4Quat3Scalar(anim, key_stride, constants, curve_types[2], quarter, fraction, keys,
                                                scale_min);
                f32 delta_z = sampled - output[2];
                if (root_translation)
                    root_translation->z = -sampled;
                output[2] += delta_z * blend;
                root_translation = NULL;
                curve_types += 3;
            } else {
                NUQUAT first, second, result;
                DecodeAni4BlendQuat3WPair(anim, key_stride, constants, curve_types[0], quarter, keys, scale_min,
                                          first.x, second.x);
                DecodeAni4BlendQuat3WPair(anim, key_stride, constants, curve_types[1], quarter, keys, scale_min,
                                          first.y, second.y);
                DecodeAni4BlendQuat3WPair(anim, key_stride, constants, curve_types[2], quarter, keys, scale_min,
                                          first.z, second.z);
                DecodeAni4BlendQuat3WPair(anim, key_stride, constants, curve_types[3], quarter, keys, scale_min,
                                          first.w, second.w);
                curve_types += 4;
                result.x = first.x * (1.0f - fraction) + second.x * fraction;
                result.y = first.y * (1.0f - fraction) + second.y * fraction;
                result.z = first.z * (1.0f - fraction) + second.z * fraction;
                result.w = first.w * (1.0f - fraction) + second.w * fraction;
                f32 length =
                    NuFsqrt(result.w * result.w + result.x * result.x + result.y * result.y + result.z * result.z);
                f32 inverse_length = length == 0.0f ? 0.0f : 1.0f / length;
                result.w *= inverse_length;
                result.x *= inverse_length;
                result.y *= inverse_length;
                result.z *= inverse_length;
                // Original W blending writes this local result and discards it.
                VuQuatSlerpFast(&result, reinterpret_cast<NUQUAT *>(output), &result, blend);
            }
        }
    }
    return 0;
}

extern "C" void ANI_SimpleAni3PlayerV4Joint_Blend_EulerQuat(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer,
                                                            f32 blend, i32 joint_count, i32 first_joint,
                                                            NUVEC *root_translation) {
    u32 quarter;
    f32 fraction;
    i32 key_offset;
    GetAni4SamplePosition(anim, frame, quarter, fraction, key_offset);

    u8 *keys = anim->keys + key_offset;
    ani3_scalemin_s *scale_min = anim->scale_min;
    const u16 *curve_types = anim->curve_types;
    for (i32 joint = 0; joint < first_joint; ++joint) {
        const u8 flags = anim->node_flags[joint];
        for (i32 group = 0; group < 3; ++group) {
            if ((flags & CurveGroupMasks[group]) != 0) {
                for (i32 component = 0; component < 3; ++component) {
                    SkipAni4V4Curve(curve_types[group * 3 + component], keys, scale_min);
                }
            }
        }
        curve_types += 9;
    }

    i32 end_joint = first_joint + joint_count;
    if (end_joint > anim->node_count) {
        end_joint = anim->node_count;
    }
    const f32 inverse_blend = 1.0f - blend;
    NUVEC *root = first_joint == 0 ? root_translation : NULL;
    for (i32 joint_index = first_joint; joint_index < end_joint; ++joint_index) {
        const u8 flags = anim->node_flags[joint_index];
        buffer->joint_flags[joint_index] |= flags;
        nuanimbuffjoint_s &joint = buffer->joints[joint_index];

        if ((flags & NUANIMBUFF_JOINT_TRANSLATION) != 0) {
            f32 sampled[3];
            f32 *translation = &joint.translation.x;
            sampled[0] = DecodeAni4V4Curve(anim, curve_types[0], quarter, fraction, keys, scale_min);
            translation[0] += (sampled[0] - translation[0]) * blend;
            sampled[1] = DecodeAni4V4Curve(anim, curve_types[1], quarter, fraction, keys, scale_min);
            translation[1] += (sampled[1] - translation[1]) * blend;
            sampled[2] = DecodeAni4V4Curve(anim, curve_types[2], quarter, fraction, keys, scale_min);
            translation[2] += (sampled[2] - translation[2]) * blend;
            if (root != NULL) {
                root->x = sampled[0];
                root->y = sampled[1];
                root->z = -sampled[2];
            }
        } else {
            joint.translation.x *= inverse_blend;
            joint.translation.y *= inverse_blend;
            joint.translation.z *= inverse_blend;
            if (root != NULL) {
                root->x = 0.0f;
                root->y = 0.0f;
                root->z = 0.0f;
            }
        }

        NUQUAT sampled_rotation = {0.0f, 0.0f, 0.0f, 1.0f};
        if ((flags & NUANIMBUFF_JOINT_ROTATION) != 0) {
            f32 euler[3];
            for (i32 component = 0; component < 3; ++component) {
                euler[component] =
                    DecodeAni4V4Curve(anim, curve_types[3 + component], quarter, fraction, keys, scale_min);
            }
            NuQuatFromEulerXYZ(&sampled_rotation, static_cast<NUANG>(euler[0] * 10430.378f),
                               static_cast<NUANG>(euler[1] * 10430.378f), static_cast<NUANG>(euler[2] * 10430.378f));
        }
        NUQUAT *rotation = reinterpret_cast<NUQUAT *>(&joint.rotation);
        VuQuatSlerpFast(rotation, rotation, &sampled_rotation, blend);

        if ((flags & NUANIMBUFF_JOINT_SCALE) != 0) {
            f32 *scale = &joint.scale.x;
            for (i32 component = 0; component < 3; ++component) {
                const f32 sampled =
                    DecodeAni4V4Curve(anim, curve_types[6 + component], quarter, fraction, keys, scale_min);
                scale[component] += (sampled - scale[component]) * blend;
            }
        } else {
            joint.scale.x = joint.scale.x * inverse_blend + blend;
            joint.scale.y = joint.scale.y * inverse_blend + blend;
            joint.scale.z = joint.scale.z * inverse_blend + blend;
        }

        root = NULL;
        curve_types += 9;
    }
}

extern "C" void ANI_SimpleAni3PlayerV4Joint_Blend(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, f32 blend,
                                                  i32 joint_count, i32 first_joint, NUVEC *root_translation) {
    if ((anim->format_flags & ANI3_FORMAT_QUATERNION_ROTATION) != 0) {
        if ((anim->format_flags & ANI3_FORMAT_QUATERNION_STORES_W) != 0) {
            ANI_SimpleAni3PlayerV4Joint_Blend_Quat3W(anim, frame, buffer, blend, joint_count, first_joint,
                                                     root_translation);
        } else {
            ANI_SimpleAni3PlayerV4Joint_Blend_Quat3(anim, frame, buffer, blend, joint_count, first_joint,
                                                    root_translation);
        }
        return;
    }
    if (buffer->use_quaternions != 0) {
        ANI_SimpleAni3PlayerV4Joint_Blend_EulerQuat(anim, frame, buffer, blend, joint_count, first_joint,
                                                    root_translation);
        return;
    }

    const f32 last_key = static_cast<f32>(anim->key_count - 1);
    f32 key = (frame - anim->first_frame) * last_key / static_cast<f32>(anim->frame_count - 1);
    if (key < 0.0f) {
        key = 0.0f;
    }
    if (last_key <= key) {
        key = last_key;
    }

    const i32 whole_key = static_cast<i32>(key);
    const u32 quarter = static_cast<u32>(whole_key) & 3;
    const f32 fraction = key - static_cast<f32>(whole_key);
    u8 *keys = anim->keys + (whole_key >> 2) * anim->key_stride;
    ani3_scalemin_s *scale_min = anim->scale_min;
    const u16 *curve_types = anim->curve_types;

    // Advance all packed stream cursors to the first requested joint.
    for (i32 joint = 0; joint < first_joint; ++joint) {
        const u8 flags = anim->node_flags[joint];
        if ((flags & NUANIMBUFF_JOINT_TRANSLATION) != 0) {
            SkipAni4V4Curve(curve_types[0], keys, scale_min);
            SkipAni4V4Curve(curve_types[1], keys, scale_min);
            SkipAni4V4Curve(curve_types[2], keys, scale_min);
        }
        if ((flags & NUANIMBUFF_JOINT_ROTATION) != 0) {
            SkipAni4V4Curve(curve_types[3], keys, scale_min);
            SkipAni4V4Curve(curve_types[4], keys, scale_min);
            SkipAni4V4Curve(curve_types[5], keys, scale_min);
        }
        if ((flags & NUANIMBUFF_JOINT_SCALE) != 0) {
            SkipAni4V4Curve(curve_types[6], keys, scale_min);
            SkipAni4V4Curve(curve_types[7], keys, scale_min);
            SkipAni4V4Curve(curve_types[8], keys, scale_min);
        }
        curve_types += 9;
    }

    const i32 decode_count = joint_count <= anim->node_count ? joint_count : anim->node_count;
    const i32 end_joint = first_joint + decode_count;
    NUVEC *root = first_joint == 0 ? root_translation : NULL;
    const f32 inverse_blend = 1.0f - blend;

    for (i32 joint_index = first_joint; joint_index < end_joint; ++joint_index) {
        const u8 flags = anim->node_flags[joint_index];
        buffer->joint_flags[joint_index] |= flags;
        f32 *group_values = reinterpret_cast<f32 *>(&buffer->joints[joint_index]);

        for (i32 group = 0; group < 3; ++group) {
            if ((flags & CurveGroupMasks[group]) != 0) {
                f32 decoded = DecodeAni4V4Curve(anim, curve_types[0], quarter, fraction, keys, scale_min);
                f32 delta = decoded - group_values[0];
                if (group == 1) {
                    delta = WrapAni4BlendRotation(delta);
                }
                if (root != NULL) {
                    root->x = decoded;
                }
                group_values[0] += delta * blend;

                decoded = DecodeAni4V4Curve(anim, curve_types[1], quarter, fraction, keys, scale_min);
                delta = decoded - group_values[1];
                if (group == 1) {
                    delta = WrapAni4BlendRotation(delta);
                }
                if (root != NULL) {
                    root->y = decoded;
                }
                group_values[1] += delta * blend;

                decoded = DecodeAni4V4Curve(anim, curve_types[2], quarter, fraction, keys, scale_min);
                delta = decoded - group_values[2];
                if (group == 1) {
                    delta = WrapAni4BlendRotation(delta);
                }
                if (root != NULL) {
                    root->z = -decoded;
                }
                group_values[2] += delta * blend;
            } else if (group == 2) {
                group_values[0] = group_values[0] * inverse_blend + blend;
                group_values[1] = group_values[1] * inverse_blend + blend;
                group_values[2] = group_values[2] * inverse_blend + blend;
            } else {
                group_values[0] *= inverse_blend;
                group_values[1] *= inverse_blend;
                group_values[2] *= inverse_blend;
                if (root != NULL) {
                    root->x = 0.0f;
                    root->y = 0.0f;
                    root->z = 0.0f;
                }
            }

            if (group == 0) {
                root = NULL;
            }
            curve_types += 3;
            group_values += 4;
        }
    }
}

extern "C" void ANI_Ani3ExtractAllNodeCurves(ani3_animheader_s *anim, float frame, float *values, i32 node,
                                             char *curve_mask) {
    u32 curve_count = anim->curve_count;
    u32 quarter;
    u32 stride = anim->key_stride;
    float fraction;
    i32 key_offset;

    if (ForcePlayEndFrame == 0 || anim->end_frame == 0) {
        if (anim->key_count == 1) {
            quarter = 0;
            fraction = 0.0f;
            key_offset = 0;
        } else {
            float last_key = static_cast<float>(anim->key_count - 1);
            float key =
                (frame - static_cast<float>(anim->first_frame)) * last_key / static_cast<float>(anim->frame_count - 1);
            if (key < 0.0f) {
                key = 0.0f;
            }
            if (last_key <= key) {
                key = last_key;
            }
            i32 whole_key = static_cast<i32>(key);
            fraction = key - static_cast<float>(whole_key);
            quarter = static_cast<u32>(whole_key) & 3;
            key_offset = (whole_key >> 2) * stride;
        }
    } else {
        float key = static_cast<float>(anim->end_frame + anim->key_count - 4);
        i32 whole_key = static_cast<i32>(key);
        fraction = key - static_cast<float>(whole_key);
        quarter = static_cast<u32>(whole_key) & 3;
        key_offset = (whole_key >> 2) * stride;
    }

    u16 *types = anim->curve_types;
    u8 *force_zero = reinterpret_cast<u8 *>(types + anim->node_count * curve_count);
    ani3_scalemin_s *scale_min = anim->scale_min;
    i16 *constants = anim->constants;
    u8 *keys = anim->keys + key_offset;

    for (i32 n = 0; n < node; ++n) {
        for (u32 curve = 0; curve < curve_count; ++curve) {
            u16 type = *types++;
            if (type < 16) {
                ++scale_min;
                keys += KeyStructSizes[type];
            }
        }
        force_zero += curve_count;
    }

    i32 quarter_shift = static_cast<i32>(quarter) * 6;
    i32 next_quarter_shift = (static_cast<i32>(quarter) * 3 + 3) * 2;
    for (u32 curve = 0; curve < curve_count; ++curve, ++values) {
        u16 type = types[curve];
        bool evaluate = curve_mask == NULL || *curve_mask == static_cast<char>(curve);
        if (curve_mask != NULL && evaluate) {
            ++curve_mask;
        }
        if (!evaluate) {
            if (type == 7) {
                keys += 8;
                ++scale_min;
            } else if (type == 6) {
                keys += 4;
                ++scale_min;
            } else if (type == 8 || type == 10) {
                keys += 4;
            }
            continue;
        }

        float key_fraction = (force_zero[curve] & 1) != 0 ? 0.0f : fraction;
        if (type == 7) {
            *values = CalcValue1648(reinterpret_cast<char *>(keys), quarter, stride, key_fraction, scale_min);
            keys += 8;
            ++scale_min;
        } else if (type == 8) {
            *values = static_cast<float>(constants[keys[quarter]]);
            keys += 4;
        } else if (type == 10) {
            u32 index = keys[quarter];
            const u32 packed = static_cast<u32>(static_cast<i32>(constants[index + 1])) |
                               (static_cast<u32>(static_cast<i32>(constants[index])) << 16);
            memcpy(values, &packed, sizeof(packed));
            keys += 4;
        } else if (type == 6) {
            u32 first = *reinterpret_cast<u32 *>(keys);
            u32 next = *reinterpret_cast<u32 *>(keys + stride);
            float first_value = static_cast<float>(first & 0xff);
            float next_value = static_cast<float>(next & 0xff);
            u32 tangents = first >> 8;
            float tangent0 = static_cast<float>((tangents >> quarter_shift) & 0x3f) * 0.01587302f;
            float packed_value;
            if (quarter == 3) {
                float interpolated = (next_value - first_value) * tangent0 + first_value;
                float tangent1 = static_cast<float>((next >> 8) & 0x3f) * 0.01587302f;
                float after = static_cast<float>(keys[stride * 2]);
                packed_value =
                    (((after - next_value) * tangent1 + next_value) - interpolated) * key_fraction + interpolated;
            } else {
                float tangent1 = static_cast<float>((tangents >> (next_quarter_shift & 0x1f)) & 0x3f) * 0.01587302f;
                packed_value =
                    (next_value - first_value) * ((tangent1 - tangent0) * key_fraction + tangent0) + first_value;
            }
            *values = packed_value * scale_min->scale + scale_min->minimum;
            keys += 4;
            ++scale_min;
        } else {
            u16 constant = reinterpret_cast<u16 *>(constants)[anim->constant_index + type - 16];
            *values = static_cast<float>(constant) * anim->scale + anim->minimum;
        }
    }
}

static i32 PlayAni4Quaternion(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, i32 joint_count,
                              i32 first_joint, i32 quaternion_components) {
    buffer->use_quaternions = 1;

    u32 quarter;
    f32 fraction;
    i32 key_offset;
    GetAni4SamplePosition(anim, frame, quarter, fraction, key_offset);

    u8 *keys = anim->keys + key_offset;
    ani3_scalemin_s *scale_min = anim->scale_min;
    const u16 *curve_types = anim->curve_types;
    const i32 curves_per_joint = quaternion_components + 6;

    for (i32 joint = 0; joint < first_joint; ++joint) {
        SkipAni4QuaternionJoint(anim, quaternion_components, joint, curve_types, keys, scale_min);
        curve_types += curves_per_joint;
    }

    i32 end_joint = first_joint + joint_count;
    if (end_joint > anim->node_count) {
        end_joint = anim->node_count;
    }
    for (i32 joint_index = first_joint; joint_index < end_joint; ++joint_index) {
        const u8 flags = anim->node_flags[joint_index];
        buffer->joint_flags[joint_index] = flags;
        nuanimbuffjoint_s &joint = buffer->joints[joint_index];

        if ((flags & NUANIMBUFF_JOINT_TRANSLATION) != 0) {
            f32 *translation = &joint.translation.x;
            for (i32 component = 0; component < 3; ++component) {
                translation[component] =
                    DecodeAni4QuaternionScalar(anim, curve_types[component], quarter, fraction, keys, scale_min);
            }
        } else {
            joint.translation = {0.0f, 0.0f, 0.0f};
        }

        NUQUAT *rotation = reinterpret_cast<NUQUAT *>(&joint.rotation);
        if ((flags & NUANIMBUFF_JOINT_ROTATION) != 0) {
            *rotation =
                DecodeAni4Quaternion(anim, quaternion_components, curve_types + 3, quarter, fraction, keys, scale_min);
        } else {
            *rotation = {0.0f, 0.0f, 0.0f, 1.0f};
        }

        const i32 scale_offset = quaternion_components + 3;
        if ((flags & NUANIMBUFF_JOINT_SCALE) != 0) {
            f32 *scale = &joint.scale.x;
            for (i32 component = 0; component < 3; ++component) {
                scale[component] = DecodeAni4QuaternionScalar(anim, curve_types[scale_offset + component], quarter,
                                                              fraction, keys, scale_min);
            }
        } else {
            joint.scale = {1.0f, 1.0f, 1.0f};
        }

        curve_types += curves_per_joint;
    }
    return 0;
}

static void BlendAni4Quaternion(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, f32 blend, i32 joint_count,
                                i32 first_joint, NUVEC *root_translation, i32 quaternion_components) {
    u32 quarter;
    f32 fraction;
    i32 key_offset;
    GetAni4SamplePosition(anim, frame, quarter, fraction, key_offset);

    u8 *keys = anim->keys + key_offset;
    ani3_scalemin_s *scale_min = anim->scale_min;
    const u16 *curve_types = anim->curve_types;
    const i32 curves_per_joint = quaternion_components + 6;
    const f32 inverse_blend = 1.0f - blend;

    for (i32 joint = 0; joint < first_joint; ++joint) {
        SkipAni4QuaternionJoint(anim, quaternion_components, joint, curve_types, keys, scale_min);
        curve_types += curves_per_joint;
    }

    i32 end_joint = first_joint + joint_count;
    if (end_joint > anim->node_count) {
        end_joint = anim->node_count;
    }
    NUVEC *root = first_joint == 0 ? root_translation : NULL;
    for (i32 joint_index = first_joint; joint_index < end_joint; ++joint_index) {
        const u8 flags = anim->node_flags[joint_index];
        buffer->joint_flags[joint_index] |= flags;
        nuanimbuffjoint_s &joint = buffer->joints[joint_index];

        if ((flags & NUANIMBUFF_JOINT_TRANSLATION) != 0) {
            f32 sampled[3];
            f32 *translation = &joint.translation.x;
            for (i32 component = 0; component < 3; ++component) {
                sampled[component] =
                    DecodeAni4QuaternionScalar(anim, curve_types[component], quarter, fraction, keys, scale_min);
                translation[component] += (sampled[component] - translation[component]) * blend;
            }
            if (root != NULL) {
                root->x = sampled[0];
                root->y = sampled[1];
                root->z = -sampled[2];
            }
        } else {
            joint.translation.x *= inverse_blend;
            joint.translation.y *= inverse_blend;
            joint.translation.z *= inverse_blend;
            if (root != NULL) {
                root->x = 0.0f;
                root->y = 0.0f;
                root->z = 0.0f;
            }
        }

        NUQUAT sampled_rotation = {0.0f, 0.0f, 0.0f, 1.0f};
        if ((flags & NUANIMBUFF_JOINT_ROTATION) != 0) {
            sampled_rotation =
                DecodeAni4Quaternion(anim, quaternion_components, curve_types + 3, quarter, fraction, keys, scale_min);
        }
        NUQUAT *rotation = reinterpret_cast<NUQUAT *>(&joint.rotation);
        VuQuatSlerpFast(rotation, rotation, &sampled_rotation, blend);

        const i32 scale_offset = quaternion_components + 3;
        if ((flags & NUANIMBUFF_JOINT_SCALE) != 0) {
            f32 *scale = &joint.scale.x;
            for (i32 component = 0; component < 3; ++component) {
                const f32 sampled = DecodeAni4QuaternionScalar(anim, curve_types[scale_offset + component], quarter,
                                                               fraction, keys, scale_min);
                scale[component] += (sampled - scale[component]) * blend;
            }
        } else {
            joint.scale.x = joint.scale.x * inverse_blend + blend;
            joint.scale.y = joint.scale.y * inverse_blend + blend;
            joint.scale.z = joint.scale.z * inverse_blend + blend;
        }

        root = NULL;
        curve_types += curves_per_joint;
    }
}
