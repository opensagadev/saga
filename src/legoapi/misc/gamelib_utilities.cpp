#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nusound/nusound.h"

#include <string.h>

struct SoundGroup {
    i16 first_sample;
    i16 sample_count;
    i16 field_0x4;
    i16 field_0x6;
};

i32 g_numGroups;
SoundGroup g_groups[128];
i32 g_lenGroupBuffer;
i16 g_groupBuffer[512];
static NUTIME frameStartTime;
static u32 frameStartMilliseconds;

DECOMP_ASSERT(sizeof(SoundGroup) == 8, "SoundGroup size");

i32 GroupBuffer_InGroup(i32 group_id, i32 sample_id) {
    i32 index = g_groups[group_id].first_sample;
    i32 end = index + g_groups[group_id].sample_count;
    while (index < end) {
        if (g_groupBuffer[index] == sample_id) {
            return 1;
        }
        index++;
    }
    return 0;
}

i32 GroupBuffer_GetSample(i32 group_id, i32 sequential) {
    i32 loaded_samples[32];
    i16 sample_count = g_groups[group_id].sample_count;
    i16 first_sample = g_groups[group_id].first_sample;
    memset(loaded_samples, 0, sizeof(loaded_samples));
    i32 loaded_count = 0;

    i16 *sample = &g_groupBuffer[first_sample];
    i16 *end = sample + sample_count;
    while (sample != end) {
        i32 sample_id = *sample;
        if (NuSound3IsSampleLoaded(g_soundInfo[sample_id].index)) {
            loaded_samples[loaded_count++] = *sample;
        }
        sample++;
    }

    if (loaded_count == 0) {
        return -1;
    }

    i32 index;
    if (sequential == 0) {
        static u32 seed;
        seed = NuRandIntSeeded(&seed);
        index = static_cast<i32>((seed >> 16) % static_cast<u32>(loaded_count));
    } else {
        index = g_groups[group_id].field_0x4++;
        if (g_groups[group_id].field_0x4 >= loaded_count) {
            g_groups[group_id].field_0x4 = 0;
        }
    }

    return loaded_samples[index];
}

i32 GroupBuffer_MakeGroup(i32 sample_id) {
    if (g_numGroups == 128 || g_lenGroupBuffer == 512) {
        return -1;
    }

    i32 group_id = g_numGroups;
    g_groups[group_id].first_sample = static_cast<i16>(g_lenGroupBuffer);
    g_groups[group_id].sample_count = 1;
    g_groups[group_id].field_0x4 = 0;
    g_soundInfo[sample_id].group = static_cast<i16>(group_id);
    g_groupBuffer[g_lenGroupBuffer++] = static_cast<i16>(sample_id);
    g_numGroups++;
    return group_id;
}

void GroupBuffer_MoveToEnd(i32 group_id) {
    i32 first_sample = g_groups[group_id].first_sample;
    i16 sample_count = g_groups[group_id].sample_count;
    i32 group_end = first_sample + sample_count;
    if (g_lenGroupBuffer == group_end) {
        return;
    }

    g_groups[group_id].first_sample = static_cast<i16>(g_lenGroupBuffer);
    memmove(&g_groupBuffer[g_lenGroupBuffer], &g_groupBuffer[first_sample],
            static_cast<usize>(sample_count) * sizeof(i16));
    memmove(&g_groupBuffer[first_sample], &g_groupBuffer[group_end],
            static_cast<usize>(g_lenGroupBuffer - first_sample) * sizeof(i16));

    for (i32 i = 0; i < g_numGroups; i++) {
        if (g_groups[i].first_sample > first_sample) {
            g_groups[i].first_sample -= sample_count;
        }
    }
}

void GroupBuffer_AddToGroup(i32 group_id, i32 sample_id) {
    if (g_lenGroupBuffer == 512) {
        return;
    }

    g_groupBuffer[g_lenGroupBuffer++] = static_cast<i16>(sample_id);
    g_groups[group_id].sample_count++;
    g_groups[group_id].field_0x4 = 0;
    g_soundInfo[sample_id].group = static_cast<i16>(group_id);
}

void GroupBuffer_RemoveGroup(i32 group_id) {
    if (group_id == -1) {
        return;
    }

    i32 first_sample = g_groups[group_id].first_sample;
    i16 sample_count = g_groups[group_id].sample_count;
    memmove(&g_groupBuffer[first_sample], &g_groupBuffer[first_sample + sample_count],
            static_cast<usize>(g_lenGroupBuffer - first_sample - sample_count) * sizeof(i16));
    g_lenGroupBuffer -= sample_count;

    for (i32 i = 0; i < g_numGroups; i++) {
        if (g_groups[i].first_sample > first_sample) {
            g_groups[i].first_sample -= sample_count;
        }
    }

    memmove(&g_groups[group_id], &g_groups[group_id + 1], static_cast<usize>(g_numGroups - 1) * sizeof(SoundGroup));
    g_numGroups--;

    for (i32 i = 0; i < SFX_MUSIC_COUNT; i++) {
        if (g_soundInfo[i].group > group_id) {
            g_soundInfo[i].group--;
        } else if (g_soundInfo[i].group == group_id) {
            g_soundInfo[i].group = -1;
        }
    }
}

i32 GroupBuffer_GetNumInGroup(i32 group_id) {
    return g_groups[group_id].sample_count;
}

void GroupBuffer_RemoveFromGroup(i32 group_id, i32 sample_id) {
    i32 first_sample = g_groups[group_id].first_sample;
    i16 sample_count = g_groups[group_id].sample_count;
    i32 end = first_sample + sample_count;
    i32 index = first_sample;
    while (index < end && g_groupBuffer[index] != sample_id) {
        index++;
    }
    if (index == end) {
        return;
    }

    memmove(&g_groupBuffer[index], &g_groupBuffer[index + 1],
            static_cast<usize>(g_lenGroupBuffer - index - 1) * sizeof(i16));
    g_lenGroupBuffer--;
    g_groups[group_id].sample_count = sample_count - 1;
    g_groups[group_id].field_0x4 = 0;
}

i32 GroupBuffer_GetSampleByIndex(i32 group_id, i32 sample_index) {
    return g_groupBuffer[g_groups[group_id].first_sample + sample_index];
}

extern "C" {

    u32 UtilGetTime(void) {
        NUTIME now;
        NuTimeGet(&now);
        return static_cast<u32>(NuTimeMilliSeconds(&now));
    }

    u32 UtilGetFrameStartTime(void) {
        return frameStartMilliseconds;
    }

    void UtilFrameStart(void) {
        NuTimeGet(&frameStartTime);
        frameStartMilliseconds = static_cast<u32>(NuTimeMilliSeconds(&frameStartTime));
    }

    // Original @0x286a8b. This is the VU-friendly approximation used while
    // blending sampled animation quaternions. The polynomial is an even
    // approximation of cos(), evaluated after an inexpensive acos estimate.
    void VuQuatSlerpFast(NUQUAT *out, NUQUAT *from, NUQUAT *to, f32 t) {
        f32 dot = NuQuatDot(from, to);
        NUQUAT target;
        if (dot < 0.0f) {
            dot = -dot;
            NuQuatNeg2(&target, to);
        } else {
            target = *to;
        }

        if (dot > 0.85f) {
            NuQuatLerp2(out, from, &target, t);
            NuQuatNormalise(out, out);
            return;
        }

        if (dot > 0.995f) {
            dot = 0.995f;
        }

        const f32 estimate = dot * 0.22340366f * dot + 2.2184808f;
        const f32 linear = dot * 2.4418843f;
        const f32 angle = NuFsqrt(estimate - linear) - NuFsqrt(estimate + linear) + 1.5707964f + dot * 0.63912874f;
        const f32 inverse_sine = 1.0f / NuFsqrt(1.0f - dot * dot);

        f32 from_angle = (1.0f - t) * angle - 1.5707964f;
        from_angle *= from_angle;
        f32 to_angle = t * angle - 1.5707964f;
        to_angle *= to_angle;

        const f32 from_weight =
            ((((from_angle * 2.3154014e-5f - 0.0013853709f) * from_angle + 0.041663583f) * from_angle - 0.49999905f) *
                 from_angle +
             0.99999994f) *
            inverse_sine;
        const f32 to_weight =
            ((((to_angle * 2.3154014e-5f - 0.0013853709f) * to_angle + 0.041663583f) * to_angle - 0.49999905f) *
                 to_angle +
             0.99999994f) *
            inverse_sine;
        NuQuatBlend(out, from, &target, from_weight, to_weight);
    }

    void buildBitCountTable(void) {
        BitCountTable[0] = 0;
        for (u32 value = 0; value < 0x100; ++value) {
            if ((value & 2) != 0) {
                ++BitCountTable[value];
            }
            if ((value & 4) != 0) {
                ++BitCountTable[value];
            }
            if ((value & 8) != 0) {
                ++BitCountTable[value];
            }
            if ((value & 0x10) != 0) {
                ++BitCountTable[value];
            }
            if ((value & 0x20) != 0) {
                ++BitCountTable[value];
            }
            if ((value & 0x40) != 0) {
                ++BitCountTable[value];
            }
            if ((value >> 7) != 0) {
                ++BitCountTable[value];
            }
            if (value + 1 < 0x100) {
                BitCountTable[value + 1] = static_cast<u8>((value + 1) & 1);
            }
        }
        isBitCountTable = 1;
    }

} // extern "C"
