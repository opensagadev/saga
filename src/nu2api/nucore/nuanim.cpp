#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nu2api_nucore_types.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numath/nufloat.h"

#include <string.h>
#include <float.h>

extern "C" f32 NuAnimEndFrame(void *animation_data) {
    ani3_animheader_s *animation = static_cast<ani3_animheader_s *>(animation_data);
    if (animation->magic != ANI3_MAGIC_VERSION_4 && animation->magic != ANI3_MAGIC_VERSION_5) {
        return *static_cast<f32 *>(animation_data);
    }

    return static_cast<f32>(animation->frame_count) + static_cast<f32>(animation->first_frame);
}

extern "C" f32 NuAnimEndFrameOld(void *animation_data) {
    ani3_animheader_s *animation = static_cast<ani3_animheader_s *>(animation_data);
    if (animation->magic != ANI3_MAGIC_VERSION_4 && animation->magic != ANI3_MAGIC_VERSION_5) {
        return *static_cast<f32 *>(animation_data);
    }

    const u32 declared_end_frame = animation->declared_end_frame;
    if (declared_end_frame == 0) {
        return static_cast<f32>(animation->frame_count + animation->first_frame);
    }
    return static_cast<f32>(declared_end_frame);
}

void NuAnimBuffInit(i32 max_joints, variptr_u *buf, variptr_u) {
    MaxAnimJoints = max_joints;
    globalbuffer = NuAnimBuffCreate(max_joints, buf);
}

nuanimdatachunk_s *NuAnimDataChunkCreate(i32 curve_set_count) {
    NuMemoryManager *memory = NuMemoryGet()->GetThreadMem();
    u8 *data = static_cast<u8 *>(memory->_BlockAlloc(0x14, 4, 1, "", 0));
    *reinterpret_cast<i32 *>(data) = curve_set_count;
    *reinterpret_cast<void **>(data + 4) = NULL;
    *reinterpret_cast<void **>(data + 8) = NULL;
    *reinterpret_cast<void **>(data + 0xc) = NULL;
    *reinterpret_cast<void **>(data + 0x10) = NULL;

    const u32 array_size = static_cast<u32>(curve_set_count) * sizeof(void *);
    void **curve_sets = static_cast<void **>(memory->_BlockAlloc(array_size, 4, 1, "", 0));
    *reinterpret_cast<void ***>(data + 8) = curve_sets;
    memset(curve_sets, 0, array_size);
    return reinterpret_cast<nuanimdatachunk_s *>(data);
}

void NuAnimDataChunkDestroy(nuanimdatachunk_s *chunk) {
    u8 *data = reinterpret_cast<u8 *>(chunk);
    const i32 curve_set_count = *reinterpret_cast<i32 *>(data);
    void **curve_sets = *reinterpret_cast<void ***>(data + 8);
    const i32 destroy_curves = *reinterpret_cast<void **>(data + 0x10) == NULL;

    for (i32 index = 0; index < curve_set_count; ++index) {
        if (curve_sets[index] != NULL) {
            NuAnimCurveSetDestroy(curve_sets[index], destroy_curves);
        }
    }

    void *curve_data = *reinterpret_cast<void **>(data + 0xc);
    if (curve_data != NULL) {
        NuMemoryGet()->GetThreadMem()->BlockFree(curve_data, 0);
    }
    void *shared_data = *reinterpret_cast<void **>(data + 0x10);
    if (shared_data != NULL) {
        NuMemoryGet()->GetThreadMem()->BlockFree(shared_data, 0);
    }
    if (curve_sets != NULL) {
        NuMemoryGet()->GetThreadMem()->BlockFree(curve_sets, 0);
    }
    NuMemoryGet()->GetThreadMem()->BlockFree(chunk, 0);
}

void NuAnimRelocatePtrsANI3(ani3_animheader_s *animation, i32 offset) {
    ani3_animheader_s *block = animation;
    do {
        if (block->scale_min != NULL) {
            block->scale_min = reinterpret_cast<ani3_scalemin_s *>(reinterpret_cast<usize>(block->scale_min) + offset);
        }
        if (block->constants != NULL) {
            block->constants = reinterpret_cast<i16 *>(reinterpret_cast<usize>(block->constants) + offset);
        }
        if (block->curve_types != NULL) {
            block->curve_types = reinterpret_cast<u16 *>(reinterpret_cast<usize>(block->curve_types) + offset);
        }
        if (block->keys != NULL) {
            block->keys = reinterpret_cast<u8 *>(reinterpret_cast<usize>(block->keys) + offset);
        }
        if (block->node_flags != NULL) {
            block->node_flags = reinterpret_cast<u8 *>(reinterpret_cast<usize>(block->node_flags) + offset);
        }
        if (block->field_38 != NULL) {
            block->field_38 = reinterpret_cast<void *>(reinterpret_cast<usize>(block->field_38) + offset);
        }
        if (block->next_block == 0) {
            break;
        }
        block = reinterpret_cast<ani3_animheader_s *>(reinterpret_cast<u8 *>(block) + block->next_block);
    } while (true);
}

u32 NuAnimGetAnimDataSizeANI3(ani3_animheader_s *animation) {
    ani3_animheader_s *block = animation;
    while (block->next_block != 0) {
        block = reinterpret_cast<ani3_animheader_s *>(reinterpret_cast<u8 *>(block) + block->next_block);
    }
    if (block->node_flags == NULL) {
        return 0;
    }
    return reinterpret_cast<usize>(block->node_flags) + block->node_count - reinterpret_cast<usize>(animation);
}

u8 BitCountTable[256] = {};

extern "C" {
static i32 isBitCountTable;
void buildBitCountTable(void) {
    for (u32 value = 0; value < 256; ++value) {
        BitCountTable[value] = 0;
        for (u32 bit = 0; bit < 8; ++bit) {
            if ((value >> bit) & 1)
                ++BitCountTable[value];
        }
    }
    isBitCountTable = 1;
}

f32 NuAnimCurveCalcVal2(nuanimcurve_s *curve, nuanimtime_s *time) {
    i32 key = 0;
    switch (time->time_byte) {
        case 0:
            key = BitCountTable[curve->key_mask[0] & time->time_mask];
            break;
        case 1:
            key = BitCountTable[curve->key_mask[0]] + BitCountTable[curve->key_mask[1] & time->time_mask];
            break;
        case 2:
            key = BitCountTable[curve->key_mask[0]];
            key += BitCountTable[curve->key_mask[1]];
            key += BitCountTable[curve->key_mask[2] & time->time_mask];
            break;
        case 3:
            key = BitCountTable[curve->key_mask[0]];
            key += BitCountTable[curve->key_mask[1]];
            key += BitCountTable[curve->key_mask[2]];
            key += BitCountTable[curve->key_mask[3] & time->time_mask];
            break;
    }
    nuanimkey_s *first = &curve->keys[key - 1];
    if (curve->flags & 1) {
        if ((curve->flags & 2) && key <= curve->key_count && time->time - first->time > first[1].time - time->time)
            return first[1].value;
        return first->value;
    }
    f32 value = first->value;
    f32 span = first[1].time - first->time;
    f32 delta = value - first[1].value;
    f32 t = (time->time - first->time) * first->reciprocal_span;
    f32 tangent0 = first->tangent * span;
    f32 tangent1 = first[1].tangent * span;
    return (((((delta + delta + tangent0 + tangent1) * t + delta * -3.0f) - (tangent0 + tangent0) - tangent1) * t +
             tangent0) *
            t) +
           value;
}

f32 NuAnimCurve2CalcValEx(nuanimcurve2_s *curve, nuanimtime_s *time, u32 type) {
    nuanimcurvedata_s *data = curve->data.curvedata;
    u32 *key_mask = data->key_mask + time->chunk;
    if (type == 4) {
        i32 frame = static_cast<i32>(NuFloor(time->time_offset));
        return static_cast<f32>((*key_mask >> ((frame - 1) & 0x1f)) & 1);
    }

    u32 key = 0;
    switch (time->time_byte) {
        case 0:
            key = BitCountTable[reinterpret_cast<u8 *>(key_mask)[0] & time->time_mask];
            break;
        case 1:
            key = BitCountTable[reinterpret_cast<u8 *>(key_mask)[0]] +
                  BitCountTable[reinterpret_cast<u8 *>(key_mask)[1] & time->time_mask];
            break;
        case 2:
            key = BitCountTable[reinterpret_cast<u8 *>(key_mask)[0]] +
                  BitCountTable[reinterpret_cast<u8 *>(key_mask)[1]] +
                  BitCountTable[reinterpret_cast<u8 *>(key_mask)[2] & time->time_mask];
            break;
        case 3:
            key = BitCountTable[reinterpret_cast<u8 *>(key_mask)[0]] +
                  BitCountTable[reinterpret_cast<u8 *>(key_mask)[1]] +
                  BitCountTable[reinterpret_cast<u8 *>(key_mask)[2]] +
                  BitCountTable[reinterpret_cast<u8 *>(key_mask)[3] & time->time_mask];
            break;
    }
    u32 key_offset = data->key_offsets[time->chunk];
    u8 *key_data = static_cast<u8 *>(data->key_data);

    switch (type) {
        case 1: {
            f32 *first = reinterpret_cast<f32 *>(key_data + (key + key_offset - 1) * 0x10);
            f32 span = first[4] - first[0];
            f32 value_delta = first[2] - first[6];
            f32 t = (time->time - first[0]) * first[1];
            f32 tangent0 = first[3] * span;
            f32 tangent1 = first[7] * span;
            return (((((value_delta * 2.0f + tangent0 + tangent1) * t + value_delta * -3.0f) - tangent0 * 2.0f) -
                     tangent1) *
                        t +
                    tangent0) *
                       t +
                   first[2];
        }
        case 2: {
            f32 *header = reinterpret_cast<f32 *>(key_data);
            u8 *first = key_data + (key + key_offset + 1) * 4;
            f32 first_time = static_cast<f32>(static_cast<u32>(first[3]));
            f32 span = static_cast<f32>(static_cast<u32>(first[7])) - first_time;
            f32 inverse_span = span == 0.0f ? 0.0f : 1.0f / span;
            f32 tangent0 = static_cast<f32>(*reinterpret_cast<i8 *>(first + 2)) * header[0] * span;
            f32 tangent1 = static_cast<f32>(*reinterpret_cast<i8 *>(first + 6)) * header[0] * span;
            f32 value0 = static_cast<f32>(*reinterpret_cast<i16 *>(first)) * header[1];
            f32 value_delta = value0 - static_cast<f32>(*reinterpret_cast<i16 *>(first + 4)) * header[1];
            f32 t = ((time->time - 1.0f) - first_time) * inverse_span;
            return (((((value_delta * 2.0f + tangent0 + tangent1) * t - value_delta * 3.0f) - tangent0 * 2.0f) -
                     tangent1) *
                        t +
                    tangent0) *
                       t +
                   value0;
        }
        case 3:
            return *reinterpret_cast<f32 *>(key_data + (key + key_offset - 1) * 8);
        case 5: {
            f32 *header = reinterpret_cast<f32 *>(key_data);
            f32 header_0 = header[0];
            f32 header_1 = header[1];
            f32 header_2 = header[2];
            i16 *first = reinterpret_cast<i16 *>(key_data + (key + key_offset) * 6 + 0x0c);
            f32 span = static_cast<f32>(static_cast<u32>(static_cast<u16>(first[5]))) -
                       static_cast<f32>(static_cast<u32>(static_cast<u16>(first[2])));
            f32 tangent0 = static_cast<f32>(static_cast<i8>(first[1])) * header_0 * span;
            f32 tangent1 = static_cast<f32>(static_cast<i8>(first[4])) * header_0 * span;
            f32 value0 = static_cast<f32>(first[0]) * header_1 + header_2;
            f32 value_delta = value0 - (static_cast<f32>(first[3]) * header_1 + header_2);
            f32 inverse_span = 1.0f / span;
            f32 t = ((time->time - 1.0f) - static_cast<f32>(static_cast<u32>(static_cast<u16>(first[2])))) *
                    inverse_span;
            return (((((value_delta * 2.0f + tangent0 + tangent1) * t - value_delta * 3.0f) - tangent0 * 2.0f) -
                     tangent1) *
                        t +
                    tangent0) *
                       t +
                   value0;
        }
        case 6: {
            f32 *header = reinterpret_cast<f32 *>(key_data);
            f32 header_3 = header[3];
            f32 header_2 = header[2];
            f32 header_0 = header[0];
            f32 header_1 = header[1];
            u8 *first = key_data + (key + key_offset + 3) * 4;
            f32 first_time = static_cast<f32>(static_cast<u32>(first[3])) * header_3;
            f32 next_time = static_cast<f32>(static_cast<u32>(first[7])) * header_3;
            if (next_time == first_time) {
                next_time = first_time + 1.0f;
            }
            f32 span = next_time - first_time;
            f32 tangent0 = static_cast<f32>(*reinterpret_cast<i8 *>(first + 2)) * header_0 * span;
            f32 tangent1 = static_cast<f32>(*reinterpret_cast<i8 *>(first + 6)) * header_0 * span;
            f32 value0 = static_cast<f32>(*reinterpret_cast<i16 *>(first)) * header_1 + header_2;
            f32 value_delta =
                value0 - (static_cast<f32>(*reinterpret_cast<i16 *>(first + 4)) * header_1 + header_2);
            f32 inverse_span = 1.0f / span;
            f32 t = ((time->time - 1.0f) - first_time) * inverse_span;
            return (((((value_delta * 2.0f + tangent0 + tangent1) * t - value_delta * 3.0f) - tangent0 * 2.0f) -
                     tangent1) *
                        t +
                    tangent0) *
                       t +
                   value0;
        }
        default:
            return 0.0f;
    }
}

void *NuAnimData2FixPtrs(void *data, isize delta, isize external_delta, i32 flags) {

    if (isBitCountTable == 0)
        buildBitCountTable();

    if (data == NULL) {
        return NULL;
    }
    nuanimdata2_s *anim = reinterpret_cast<nuanimdata2_s *>(reinterpret_cast<usize>(data) + delta);
    if (anim == NULL) {
        return NULL;
    }
    if (*reinterpret_cast<u32 *>(&anim->duration) + 0xbeb1b6ccU < 2) {
        ANI_FixUpAddrs(reinterpret_cast<ani3_animheader_s *>(anim),
                       external_delta == 0 ? static_cast<isize>(reinterpret_cast<usize>(anim)) : delta, flags);
        return anim;
    }

    if (anim->curves != NULL) {
        anim->curves = reinterpret_cast<nuanimcurve2_s *>(reinterpret_cast<usize>(anim->curves) + delta);
    }
    if (anim->curve_types != NULL) {
        anim->curve_types = reinterpret_cast<u8 *>(reinterpret_cast<usize>(anim->curve_types) + delta);
    }
    if (anim->node_flags != NULL) {
        anim->node_flags = reinterpret_cast<u8 *>(reinterpret_cast<usize>(anim->node_flags) + delta);
    }
    i32 curve_count = static_cast<i32>(anim->curve_count) * static_cast<i32>(anim->node_count);
    for (i32 i = 0; i < curve_count; ++i) {
        if (anim->curve_types[i] != 0) {
            nuanimcurvedata_s *curve = anim->curves[i].data.curvedata;
            curve = reinterpret_cast<nuanimcurvedata_s *>(reinterpret_cast<usize>(curve) + delta);
            anim->curves[i].data.curvedata = curve;
            if (curve->key_mask != NULL) {
                curve->key_mask = reinterpret_cast<u32 *>(reinterpret_cast<usize>(curve->key_mask) + delta);
            }
            if (curve->key_offsets != NULL) {
                curve->key_offsets = reinterpret_cast<u16 *>(reinterpret_cast<usize>(curve->key_offsets) + delta);
            }
            if (curve->key_data != NULL) {
                curve->key_data = reinterpret_cast<u8 *>(reinterpret_cast<usize>(curve->key_data) + delta);
            }
        }
    }
    return anim;
}

static inline void *NuLegacyRelocatePointer(void *pointer, isize delta) {
    return pointer == NULL ? NULL : reinterpret_cast<void *>(reinterpret_cast<usize>(pointer) + delta);
}
void *NuAnimDataFixPtrs(void *animation, isize delta) {

    if (isBitCountTable == 0)
        buildBitCountTable();
    animation = NuLegacyRelocatePointer(animation, delta);
    u8 *data = static_cast<u8 *>(animation);
    void *&name = *reinterpret_cast<void **>(data + 4);
    name = NuLegacyRelocatePointer(name, delta);
    void **&chunks = *reinterpret_cast<void ***>(data + 0xc);
    chunks = static_cast<void **>(NuLegacyRelocatePointer(chunks, delta));
    if (chunks != NULL) {
        i32 chunk_count = *reinterpret_cast<i32 *>(data + 8);
        for (i32 i = 0; i < chunk_count; ++i) {
            chunks[i] = NuLegacyRelocatePointer(chunks[i], delta);
            u8 *chunk = static_cast<u8 *>(chunks[i]);
            if (chunk != NULL) {
                void **&sets = *reinterpret_cast<void ***>(chunk + 8);
                sets = static_cast<void **>(NuLegacyRelocatePointer(sets, delta));
                if (sets != NULL) {
                    i32 set_count = *reinterpret_cast<i32 *>(chunk);
                    for (i32 j = 0; j < set_count; ++j) {
                        sets[j] = NuLegacyRelocatePointer(sets[j], delta);
                        u8 *set = static_cast<u8 *>(sets[j]);
                        if (set != NULL) {
                            void *&constants = *reinterpret_cast<void **>(set + 4);
                            constants = NuLegacyRelocatePointer(constants, delta);
                            void **&curves = *reinterpret_cast<void ***>(set + 8);
                            curves = static_cast<void **>(NuLegacyRelocatePointer(curves, delta));
                            if (curves != NULL) {
                                for (i32 k = 0; k < *reinterpret_cast<i8 *>(set + 0xc); ++k) {
                                    curves[k] = NuLegacyRelocatePointer(curves[k], delta);
                                    u8 *curve = static_cast<u8 *>(curves[k]);
                                    if (curve != NULL) {
                                        void *&keys = *reinterpret_cast<void **>(curve + 4);
                                        keys = NuLegacyRelocatePointer(keys, delta);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return animation;
}

void *NuAnimDataRead(NUFILE file) {

    if (isBitCountTable == 0) {
        buildBitCountTable();
    }

    char *name = NULL;
    const i32 name_size = NuFileReadInt(file);
    if (name_size != 0) {
        name = static_cast<char *>(NuMemoryGet()->GetThreadMem()->_BlockAlloc(name_size, 4, 1, "", 0));
        NuFileRead(file, name, name_size);
    }

    const f32 duration = NuFileReadFloat(file);
    const i32 chunk_count = NuFileReadInt(file);
    u8 *animation = static_cast<u8 *>(NuAnimDataCreate(chunk_count));
    *reinterpret_cast<f32 *>(animation) = duration;
    *reinterpret_cast<char **>(animation + 4) = name;

    for (i32 chunk_index = 0; chunk_index < *reinterpret_cast<i32 *>(animation + 8); ++chunk_index) {
        const i32 curve_set_count = NuFileReadInt(file);
        void **chunk_slot = *reinterpret_cast<void ***>(animation + 0xc) + chunk_index;
        u8 *chunk = reinterpret_cast<u8 *>(NuAnimDataChunkCreate(curve_set_count));
        *chunk_slot = chunk;
        *reinterpret_cast<i32 *>(chunk) = curve_set_count;

        u8 *curve_data = NULL;
        const i32 curve_data_count = NuFileReadInt(file);
        if (curve_data_count != 0) {
            const u32 curve_data_size = static_cast<u32>(curve_data_count) * 0x10;
            curve_data = static_cast<u8 *>(NuMemoryGet()->GetThreadMem()->_BlockAlloc(curve_data_size, 4, 1, "", 0));
            *reinterpret_cast<void **>(chunk + 0xc) = curve_data;
            NuFileRead(file, curve_data, curve_data_size);
            curve_data = *reinterpret_cast<u8 **>(chunk + 0xc);
        } else {
            *reinterpret_cast<void **>(chunk + 0xc) = NULL;
        }

        u8 *shared_curves = NULL;
        const i32 shared_curve_count = NuFileReadInt(file);
        if (shared_curve_count != 0) {
            const u32 shared_curve_size = static_cast<u32>(shared_curve_count) * 0x10;
            shared_curves = static_cast<u8 *>(NuMemoryGet()->GetThreadMem()->_BlockAlloc(shared_curve_size, 4, 1, "", 0));
            *reinterpret_cast<void **>(chunk + 0x10) = shared_curves;
            NuFileRead(file, shared_curves, shared_curve_size);
        } else {
            *reinterpret_cast<void **>(chunk + 0x10) = NULL;
        }

        for (i32 set_index = 0; set_index < curve_set_count; ++set_index) {
            const i32 curve_count = NuFileReadChar(file);
            if (curve_count == 0) {
                continue;
            }

            void **set_slot = *reinterpret_cast<void ***>(chunk + 8) + set_index;
            *set_slot = NuAnimCurveSetCreate(curve_count);
            u8 *curve_set = static_cast<u8 *>((*reinterpret_cast<void ***>(chunk + 8))[set_index]);
            *reinterpret_cast<i32 *>(curve_set) = NuFileReadInt(file);
            for (i32 curve_index = 0;
                 curve_index < *reinterpret_cast<i8 *>(
                                   static_cast<u8 *>((*reinterpret_cast<void ***>(chunk + 8))[set_index]) + 0xc);
                 ++curve_index) {
                curve_set = static_cast<u8 *>((*reinterpret_cast<void ***>(chunk + 8))[set_index]);
                f32 *value = *reinterpret_cast<f32 **>(curve_set + 4) + curve_index;
                *value = NuFileReadFloat(file);
            }
        }

        u8 *shared_cursor = *reinterpret_cast<u8 **>(chunk + 0x10);
        u8 *curve_data_cursor = curve_data;
        for (i32 set_index = 0; set_index < *reinterpret_cast<i32 *>(chunk); ++set_index) {
            u8 *curve_set = static_cast<u8 *>((*reinterpret_cast<void ***>(chunk + 8))[set_index]);
            if (curve_set == NULL) {
                continue;
            }
            for (i32 curve_index = 0;
                 curve_index < *reinterpret_cast<i8 *>(
                                   static_cast<u8 *>((*reinterpret_cast<void ***>(chunk + 8))[set_index]) + 0xc);
                 ++curve_index) {
                curve_set = static_cast<u8 *>((*reinterpret_cast<void ***>(chunk + 8))[set_index]);
                if ((*reinterpret_cast<f32 **>(curve_set + 4))[curve_index] == FLT_MAX) {
                    (*reinterpret_cast<void ***>(curve_set + 8))[curve_index] = shared_cursor;
                    ++*reinterpret_cast<i32 *>(chunk + 4);
                    *reinterpret_cast<void **>(shared_cursor + 4) = curve_data_cursor;
                    curve_data_cursor += *reinterpret_cast<i32 *>(shared_cursor + 8) << 4;
                    shared_cursor += 0x10;
                }
            }
        }
    }
    return animation;
}

void NuAnimInit(i32 max_joints, VARIPTR *buf, VARIPTR buf_end) {

    buildBitCountTable();
    NuAnimBuffInit(max_joints, buf, buf_end);
}
}

extern "C" void *NuAnimData2Relocate(void **data, VARIPTR *buf) {
    void *source = *data;
    u32 magic = *static_cast<u32 *>(source);
    if (magic == ANI3_MAGIC_VERSION_4 || magic == ANI3_MAGIC_VERSION_5) {
        u32 size = NuAnimGetAnimDataSizeANI3(static_cast<ani3_animheader_s *>(source));
        buf->addr = ALIGN(buf->addr, 16);
        void *destination = buf->void_ptr;
        buf->addr += size;
        *data = destination;
        memcpy(destination, source, size);
        NuAnimRelocatePtrsANI3(static_cast<ani3_animheader_s *>(destination),
                               reinterpret_cast<usize>(destination) - reinterpret_cast<usize>(source));
        return destination;
    }

    buf->addr = ALIGN(buf->addr, 16);
    u32 *header = static_cast<u32 *>(source);
    memcpy(buf->void_ptr, source, header[0]);
    *data = buf->void_ptr;
    buf->addr += *static_cast<u32 *>(*data);
    header = static_cast<u32 *>(*data);
    header[2] = reinterpret_cast<usize>(NuAnimData2FixPtrs(
        reinterpret_cast<void *>(static_cast<usize>(header[2])),
        reinterpret_cast<usize>(header) - static_cast<usize>(header[1]), 0, 0));
    header[1] = reinterpret_cast<usize>(*data);
    return reinterpret_cast<void *>(static_cast<usize>(header[2]));
}

extern "C" void *NuAnimData2Fixup(i32 file_size, void **data) {
    u32 *header = static_cast<u32 *>(*data);
    u32 magic = header[0];
    if (static_cast<i32>(header[1]) > static_cast<i32>(0x414e4934)) {
        return NuPtrBlockFix(header);
    }
    if (magic == 0x414e4933 || magic == 0x414e4934) {
        ANI_FixUpAddrs(reinterpret_cast<ani3_animheader_s *>(header),
                       static_cast<isize>(reinterpret_cast<usize>(header)), 0);
        return header;
    }
    header[0] = static_cast<u32>(file_size);
    const usize relocation_delta = reinterpret_cast<usize>(header) - static_cast<usize>(header[1]);
    header[2] = reinterpret_cast<usize>(
        NuAnimData2FixPtrs(reinterpret_cast<void *>(static_cast<usize>(header[2])), (isize)relocation_delta, 0, 0));
    header = static_cast<u32 *>(*data);
    header[1] = reinterpret_cast<usize>(header);
    return reinterpret_cast<void *>(static_cast<usize>(header[2]));
}

extern "C" void *NuAnimData2LoadBuffEx(char *path, VARIPTR *buf, VARIPTR *buf_end, void **result) {
    buf->addr = ALIGN(buf->addr, 0x10);
    const i32 file_size = NuFileLoadBuffer(path, buf->void_ptr, static_cast<i32>(buf_end->addr - buf->addr));
    if (file_size == 0) {
        if (NuFileGetLastError() == -1) {
            *buf = *buf_end;
        }
        *result = NULL;
        return NULL;
    }
    void *data = buf->void_ptr;
    if (static_cast<i32>(static_cast<u32 *>(data)[1]) > static_cast<i32>(0x414e4934)) {
        data = NuPtrBlockFix(data);
        buf->void_ptr = data;
        buf->addr += static_cast<usize>(file_size);
        *result = data;
        return data;
    }
    *result = data;
    buf->addr += static_cast<usize>(file_size);
    return NuAnimData2Fixup(file_size, result);
}

extern "C" void *NuAnimData2LoadBuff(char *path, VARIPTR *buf, VARIPTR *buf_end) {
    void *result;
    return NuAnimData2LoadBuffEx(path, buf, buf_end, &result);
}

extern "C" void *NuAnimData2LoadBuffFromPAK(void *data, i32 file_size) {
    if (file_size == 0) {
        return NULL;
    }
    if (static_cast<i32>(static_cast<u32 *>(data)[1]) > static_cast<i32>(0x414e4934)) {
        return NuPtrBlockFix(data);
    }
    return NuAnimData2Fixup(file_size, &data);
}

void NuAnimBuffEvaluate_3_QuatB(numtx_s *base, nuanimbuff_s *buffer, nugscn_s *scene, numtx_s *matrices,
                                ani3_animheader_s *animation, NUHGOBJROOTFN root_fn, nuvec_s *root_translation,
                                void *root_data) {
    nuhgobj_s *object = reinterpret_cast<nuhgobj_s *>(scene);
    i32 count = animation->node_count < object->joint_count ? animation->node_count : object->joint_count;
    NUQUAT *quaternions = static_cast<NUQUAT *>(NuScratchAlloc32((count + 1) * 16));
    quaternions = reinterpret_cast<NUQUAT *>(ALIGN(reinterpret_cast<usize>(quaternions), 16));
    NUVEC *scales = static_cast<NUVEC *>(NuScratchAlloc32(count * sizeof(NUVEC)));
    if (!buffer)
        buffer = static_cast<nuanimbuff_s *>(globalbuffer);
    void *callback_data[256];
    if (AnimBuffEvalData && AnimBuffEvalJoint) {
        memset(callback_data, 0, object->joint_count * sizeof(void *));
        for (i32 index = 0; AnimBuffEvalData[index]; ++index) {
            i32 mapped = AnimBuffEvalJoint[index];
            if (mapped >= 0 && mapped < object->joint_override_map_count) {
                u8 joint_index = object->joint_override_map[mapped];
                if (joint_index != 255)
                    callback_data[joint_index] = AnimBuffEvalData[index];
            }
        }
    }
    // Slot255 and the root/nonroot scale selection follow the original evaluator.
    NUVEC root_scale;
    if (base) {
        root_scale = NuMtxGetScale(base);
        NuMtxToQuat(base, &quaternions[255]);
    } else {
        root_scale.x = root_scale.y = root_scale.z = 1.0f;
        quaternions[255] = NUQUAT{0, 0, 0, 1};
    }
    NUVEC root_values = {0, 0, 0};
    nuanimbuffjoint_s *joint = buffer->joints;
    NUQUAT *rotation = quaternions;
    NUMTX *matrix = matrices;
    NUVEC *scale = scales;
    const u8 *joint_flags = buffer->joint_flags;
    for (i32 index = 0; index < count; ++index, ++joint, ++rotation, ++matrix, ++scale) {
        u8 parent = object->joints[index].parent_index;
        u8 flags = *joint_flags++;
        if (flags & 1)
            *rotation = *reinterpret_cast<NUQUAT *>(&joint->rotation);
        else
            *rotation = NUQUAT{0, 0, 0, 1};
        NUVEC *parent_scale;
        if (parent != 255) {
            const NUQUAT q = *rotation, p = quaternions[parent];
            rotation->z = (p.w * q.z + q.w * p.z + p.x * q.y) - q.x * p.y;
            rotation->x = (p.w * q.x + q.w * p.x + p.y * q.z) - p.z * q.y;
            rotation->y = (p.w * q.y + q.w * p.y + p.z * q.x) - p.x * q.z;
            rotation->w = ((p.w * q.w - q.x * p.x) - q.y * p.y) - q.z * p.z;
            parent_scale = &root_scale;
        } else
            parent_scale = &scales[parent];
        NuQuatToMtx(rotation, matrix);
        if (flags & 8) {
            scale->x = joint->scale.x * parent_scale->x;
            scale->y = joint->scale.y * parent_scale->y;
            scale->z = joint->scale.z * parent_scale->z;
            NuMtxPreScaleVU0(matrix, scale);
        } else
            *scale = *parent_scale;
        if (flags & 16)
            scale->x = scale->y = scale->z = 1.0f;
        if (flags & 2) {
            root_values.x = joint->translation.x;
            root_values.y = joint->translation.y;
            root_values.z = -joint->translation.z;
            if (parent != 255)
                NuVecMtxTransform(reinterpret_cast<NUVEC *>(&matrix->m30), &joint->translation, &matrices[parent]);
            else if (base)
                NuVecMtxTransform(reinterpret_cast<NUVEC *>(&matrix->m30), &joint->translation, base);
            else {
                matrix->m30 = joint->translation.x;
                matrix->m31 = joint->translation.y;
                matrix->m32 = joint->translation.z;
            }
        } else if (parent != 255) {
            matrix->m30 = matrices[parent].m30;
            matrix->m31 = matrices[parent].m31;
            matrix->m32 = matrices[parent].m32;
            matrix->m33 = matrices[parent].m33;
        } else if (base) {
            matrix->m30 = base->m30;
            matrix->m31 = base->m31;
            matrix->m32 = base->m32;
            matrix->m33 = base->m33;
        }
        if (parent == 255) {
            if (root_fn)
                root_fn(matrix, root_data, &root_values, &root_values, root_translation, 0.0f);
            root_fn = NULL;
            if (flags & 64)
                scale->x = scale->y = scale->z = 1.0f;
        }
        if (AnimBuffEvalCB && callback_data[index])
            AnimBuffEvalCB(matrix, callback_data[index], rotation);
    }
    // Callbacks observe the engine coordinates; reflect the completed hierarchy afterward.
    for (i32 index = 0; index < count; ++index) {
        NUMTX *matrix = &matrices[index];
        matrix->m02 = -matrix->m02;
        matrix->m12 = -matrix->m12;
        matrix->m20 = -matrix->m20;
        matrix->m21 = -matrix->m21;
        matrix->m23 = -matrix->m23;
        matrix->m32 = -matrix->m32;
    }
    for (i32 index = count; index < object->joint_count; ++index)
        NuMtxSetIdentity(&matrices[index]);
    AnimBuffEvalCB = NULL;
    AnimBuffEvalData = NULL;
    AnimBuffEvalJoint = NULL;
    NuScratchRelease();
    NuScratchRelease();
}
