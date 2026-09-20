// Lightweight microsecond profiling slots used by the render thread.

#include "nu2api/nu3d/android/nutimebar_plain.h"

#include "decomp.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nutime.h"

#include <cstddef>
#include <cstring>

// ---------------------------------------------------------------------------
struct TimeBarSet {
    i32 uses_buffer;
    i32 slot_count;
    i32 *accumulators[2];
    NUTIME *start_times;
    i32 *field_14;
    i32 *field_18;
    i32 *colours;
    i32 *toggle_flags;
    const char **slot_names;
    i32 field_28;
};

DECOMP_ASSERT(sizeof(TimeBarSet) == 0x2c, "TimeBarSet size");
DECOMP_ASSERT(offsetof(TimeBarSet, slot_count) == 0x04, "TimeBarSet slot count offset");
DECOMP_ASSERT(offsetof(TimeBarSet, accumulators) == 0x08, "TimeBarSet accumulator offset");
DECOMP_ASSERT(offsetof(TimeBarSet, start_times) == 0x10, "TimeBarSet start-time offset");
DECOMP_ASSERT(offsetof(TimeBarSet, toggle_flags) == 0x20, "TimeBarSet toggle offset");
DECOMP_ASSERT(offsetof(TimeBarSet, slot_names) == 0x24, "TimeBarSet name offset");

// ---------------------------------------------------------------------------
static constexpr i32 kMaxTimeBarSets = 16;
static TimeBarSet *NuTimeBar_SetList[kMaxTimeBarSets];
static NUMTL *NuTimeBar_FrameOutMtl;
static i32 NuTimeBar_Initialised;
static i32 NuTimeBar_PeakReset;
static i32 NuTimeBar_GpuFrameOutEnabled;
static i32 NuTimeBar_EngineEnabled;
static i32 NuTimeBar_RenderPeakReset = 30;
static const u32 NuTimeBar_DefaultColours[kMaxTimeBarSets] = {
    0x00000040, 0x22222240, 0x00004440, 0x00008840, 0x44000040, 0x88000040, 0x44004440, 0x88008840,
    0x00440040, 0x00880040, 0x00444440, 0x00888840, 0x44440040, 0x88880040, 0x44444440, 0x88888840,
};

// ---------------------------------------------------------------------------
static void *timebar_allocate(VARIPTR *buffer, usize size, usize alignment) {
    if (buffer == NULL) {
        return NU_ALLOC(size, alignment, 1, "Main", 0);
    }

    usize address = ALIGN(buffer->addr, alignment);
    buffer->addr = address + size;
    return reinterpret_cast<void *>(address);
}

template <typename T> static T *timebar_allocate_array(VARIPTR *buffer, i32 count) {
    return static_cast<T *>(timebar_allocate(buffer, static_cast<usize>(count) * sizeof(T), alignof(T)));
}

static i32 CreateTimeBar(VARIPTR *buffer, VARIPTR unused_buffer, i32 *colours, i32 slot_count) {
    (void)unused_buffer;

    TimeBarSet *timebar = timebar_allocate_array<TimeBarSet>(buffer, 1);
    std::memset(timebar, 0, sizeof(*timebar));
    timebar->uses_buffer = buffer != NULL;
    timebar->accumulators[0] = timebar_allocate_array<i32>(buffer, slot_count);
    timebar->accumulators[1] = timebar_allocate_array<i32>(buffer, slot_count);
    timebar->start_times = timebar_allocate_array<NUTIME>(buffer, slot_count);
    timebar->field_14 = timebar_allocate_array<i32>(buffer, slot_count);
    timebar->field_18 = timebar_allocate_array<i32>(buffer, slot_count);
    timebar->colours = timebar_allocate_array<i32>(buffer, slot_count);
    timebar->toggle_flags = timebar_allocate_array<i32>(buffer, slot_count);
    timebar->slot_names = timebar_allocate_array<const char *>(buffer, slot_count);

    std::memset(timebar->accumulators[0], 0, static_cast<usize>(slot_count) * sizeof(i32));
    std::memset(timebar->accumulators[1], 0, static_cast<usize>(slot_count) * sizeof(i32));
    std::memset(timebar->start_times, 0, static_cast<usize>(slot_count) * sizeof(NUTIME));
    std::memset(timebar->field_14, 0, static_cast<usize>(slot_count) * sizeof(i32));
    std::memset(timebar->field_18, 0, static_cast<usize>(slot_count) * sizeof(i32));
    std::memset(timebar->colours, 0, static_cast<usize>(slot_count) * sizeof(i32));
    std::memset(timebar->toggle_flags, 0, static_cast<usize>(slot_count) * sizeof(i32));
    std::memset(timebar->slot_names, 0, static_cast<usize>(slot_count) * sizeof(const char *));

    i32 list_index;
    for (list_index = 0; list_index < kMaxTimeBarSets; ++list_index) {
        if (NuTimeBar_SetList[list_index] == NULL) {
            NuTimeBar_SetList[list_index] = timebar;
            break;
        }
    }
    if (list_index == kMaxTimeBarSets) {
        list_index = kMaxTimeBarSets - 1;
        NuTimeBar_SetList[list_index] = timebar;
    }

    if (colours != NULL) {
        std::memcpy(timebar->colours, colours, static_cast<usize>(slot_count) * sizeof(i32));
    } else {
        std::memcpy(timebar->colours, NuTimeBar_DefaultColours, static_cast<usize>(slot_count) * sizeof(i32));
    }
    timebar->slot_count = slot_count;

    i32 set = list_index - 1;
    timebar->field_28 = set > 0;
    return set;
}

extern "C" i32 NuTimeBarCreateSetEx(VARIPTR *buffer, VARIPTR unused_buffer, i32 *colours) {
    return CreateTimeBar(buffer, unused_buffer, colours, 16);
}

extern "C" i32 NuTimeBarCreateSetEx2(VARIPTR *buffer, VARIPTR unused_buffer, i32 field_28) {
    i32 set = NuTimeBarCreateSetEx(buffer, unused_buffer, NULL);
    NuTimeBar_SetList[set + 1]->field_28 = field_28;
    return set;
}

extern "C" i32 NuTimeBarCreateSet(i32 *colours) {
    return NuTimeBarCreateSetEx(NULL, VARIPTR{}, colours);
}

extern "C" void NuTimeBarInitEx(VARIPTR *buffer, VARIPTR unused_buffer) {
    std::memset(NuTimeBar_SetList, 0, sizeof(NuTimeBar_SetList));
    CreateTimeBar(buffer, unused_buffer, NULL, 7);
    NuTimeBarCreateSetEx(buffer, unused_buffer, NULL);

    NuTimeBar_FrameOutMtl = NuMtlCreate(1);
    u8 *attributes = reinterpret_cast<u8 *>(&NuTimeBar_FrameOutMtl->attribs);
    attributes[0] = static_cast<u8>((attributes[0] & 0xf0) | 2);
    NuMtlUpdate(NuTimeBar_FrameOutMtl);
    NuTimeBar_Initialised = 1;
}

extern "C" void NuTimeBarInit(void) {
    VARIPTR unused = {};
    NuTimeBarInitEx(NULL, unused);
}

// ---------------------------------------------------------------------------
// Public API — original addresses noted per function
// ---------------------------------------------------------------------------

// original 0x2d7630
// Reset the accumulator for a single slot, or — when called as
// Reset(-1, 0) — reset every slot in set 0.  The per-slot toggle is
// flipped so the next End/SlotSet lands in the other buffer, then the
// newly-inactive accumulator is zeroed.
extern "C" void NuTimeBarSlotReset(i32 set, i32 slot) {
    if (NuTimeBar_Initialised == 0) {
        return;
    }

    if (slot == 0 && set == -1) {
        TimeBarSet *timebar = NuTimeBar_SetList[0];
        if (timebar->slot_count > 0) {
            do {
                timebar->toggle_flags[slot] ^= 1;
                timebar->accumulators[timebar->toggle_flags[slot] == 0][slot] = 0;
                ++slot;
            } while (slot < timebar->slot_count);
        }
    } else {
        TimeBarSet *timebar = NuTimeBar_SetList[set + 1];
        timebar->toggle_flags[slot] ^= 1;
        timebar->accumulators[timebar->toggle_flags[slot] == 0][slot] = 0;
    }
}

// original 0x2d76b0
// Mark the beginning of a timed region.  Records the current tick and
// the label pointer for the slot; the matching End computes the delta.
extern "C" void _NuTimeBarSlotBegin(i32 set, i32 slot, const char *name) {
    const i32 initialised = NuTimeBar_Initialised;
    if (initialised == 0) {
        return;
    }

    TimeBarSet *timebar = NuTimeBar_SetList[set + 1];
    NuTimeGet(&timebar->start_times[slot]);
    timebar->slot_names[slot] = name;
}

// original 0x2d7710
// Close the timed region opened by _NuTimeBarSlotBegin, accumulate the
// elapsed microseconds into the double-buffered slot, and return the
// updated accumulator value.  Returns 0 when profiling is disabled.
extern "C" u32 _NuTimeBarSlotEnd(i32 set, i32 slot) {
    u32 value = 0;
    if (NuTimeBar_Initialised != 0) {
        TimeBarSet *timebar = NuTimeBar_SetList[set + 1];
        NUTIME now;
        NuTimeGet(&now);

        NUTIME delta;
        NuTimeSub(&delta, &now, &timebar->start_times[slot]);
        f32 elapsed_us = NuTimeMicroSeconds(&delta);

        i32 accumulator = 1 - timebar->toggle_flags[slot];
        timebar->accumulators[accumulator][slot] += static_cast<i32>(elapsed_us);
        value = static_cast<u32>(timebar->accumulators[accumulator][slot]);
    }
    return value;
}

// original 0x2d77c0
// Directly overwrite the accumulator for a slot (used by the render
// thread to publish GPU timings that come from GL queries rather than
// CPU wall-clock).  Respects the same double-buffer selection as End.
extern "C" void NuTimeBarSlotSet(i32 set, i32 slot, i32 value) {
    if (NuTimeBar_Initialised != 0) {
        TimeBarSet *timebar = NuTimeBar_SetList[set + 1];
        i32 accumulator = 1 - timebar->toggle_flags[slot];
        timebar->accumulators[accumulator][slot] = value;
    }
}

// original 0x2d7820
// Update the display name for a slot without touching its timing.
extern "C" void NuTimeBarSlotSetName(i32 set, i32 slot, const char *name) {
    if (NuTimeBar_Initialised != 0) {
        TimeBarSet *timebar = NuTimeBar_SetList[set + 1];
        timebar->slot_names[slot] = name;
    }
}

extern "C" void NuTimeBarSlotSetEx(i32 set, i32 slot, i32 value, const char *name) {
    if (NuTimeBar_Initialised == 0) {
        return;
    }
    if (slot == 6 && set == -1) {
        value =
            static_cast<i32>(static_cast<u32>(static_cast<f64>(static_cast<f32>(static_cast<u32>(value))) * 63.556));
    }
    TimeBarSet *timebar = NuTimeBar_SetList[set + 1];
    i32 accumulator = 1 - timebar->toggle_flags[slot];
    timebar->accumulators[accumulator][slot] = value;
    timebar->slot_names[slot] = name;
}

extern "C" i32 NuTimeBarSlotLastValueMicroseconds(i32 set, i32 slot) {
    TimeBarSet *timebar = NuTimeBar_SetList[set + 1];
    return timebar->accumulators[timebar->toggle_flags[slot]][slot];
}

extern "C" i32 NuTimeBarSlotLastValue(i32 set, i32 slot) {
    u32 elapsed = static_cast<u32>(NuTimeBarSlotLastValueMicroseconds(set, slot));
    return static_cast<i32>((static_cast<f32>(elapsed) * 60.0f / 1000000.0f) * 262.5f);
}

extern "C" void NuTimeBarResetPeaks(void) {
    NuTimeBar_PeakReset = 1;
}

extern "C" void NuTimeBarSetScaleY(void) {
    STUBBED();
}

extern "C" void NuTimeBarSetRenderHorizontal(void) {
    STUBBED();
}

extern NUQFNT *system_qfont;
extern "C" i32 NuRndrBeginScene(i32 flags);
extern "C" void NuRndrRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, NUMTL *material);
extern "C" void NuQFntPrintEx(NUQFNT *font, i32 x, i32 y, i32 alignment, const char *format, ...);

static f32 timebar_unsigned_float(u32 value) {
    // Preserve the retail conversion for values whose sign bit is set.
    return static_cast<f32>(value & 0xffff) + static_cast<f32>(value >> 16) * 65536.0f;
}

extern "C" void NuTimeBarSetRender(i32 set) {
    if (set < -1 || set >= kMaxTimeBarSets - 1) {
        return;
    }
    TimeBarSet *timebar = NuTimeBar_SetList[set + 1];
    if (timebar == NULL) {
        return;
    }

    f32 gpu_frames = 0.0f;
    if (set == -1) {
        if (NuTimeBar_GpuFrameOutEnabled != 0 && timebar->slot_count > 1) {
            i32 buffer = timebar->toggle_flags[1];
            u32 gpu_time = static_cast<u32>(timebar->accumulators[buffer][1]);
            gpu_frames = timebar_unsigned_float(gpu_time) * nuapi.fps / 1000000.0f;
        }
        if (NuTimeBar_EngineEnabled == 0 && gpu_frames <= 1.0f) {
            return;
        }
    }

    NuRndrBeginScene(-1);
    if (set >= 0 || NuTimeBar_EngineEnabled != 0) {
        const f32 vertical_scale = 120.0f / static_cast<f32>(PS2_VREZ_H);
        const i32 screen_width = PS2_VREZ_W << 4;
        const i32 screen_height = PS2_VREZ_H << 4;

        for (i32 slot = 0; slot < timebar->slot_count; ++slot) {
            i32 buffer = timebar->toggle_flags[slot];
            u32 value = static_cast<u32>(timebar->accumulators[buffer][slot]);
            u32 &maximum = reinterpret_cast<u32 *>(timebar->field_14)[slot];
            u32 &recent_peak = reinterpret_cast<u32 *>(timebar->field_18)[slot];
            if (value > maximum) {
                maximum = value;
            }
            if (NuTimeBar_RenderPeakReset == 0) {
                recent_peak = 0;
            }
            if (value > recent_peak) {
                recent_peak = value;
            }

            if (recent_peak != 0) {
                const f32 x_fraction = static_cast<f32>(slot + 1) * 0.025f * 3.0f;
                const i32 x = static_cast<i32>(x_fraction * screen_width);
                const i32 y = static_cast<i32>(0.05f * screen_height);
                const i32 width = static_cast<i32>(0.025f * screen_width);
                const f32 frame_fraction = timebar_unsigned_float(recent_peak) * 60.0f / 1000000.0f;
                const i32 height = static_cast<i32>(frame_fraction * vertical_scale * screen_height);
                NuRndrRect2di(x, y, width, height, timebar->colours[slot], NULL);

                NuQFntPushPrintMode(2);
                NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
                NuQFntSet(system_qfont);
                NuQFntSetScale(system_qfont, 1.0f, 1.0f);
                NuQFntSetColour(system_qfont, 0xe0e0e0e0);

                const i32 centre_x = x + width / 2;
                const i32 recent_value = static_cast<i32>(timebar_unsigned_float(recent_peak) * 15734.0f /
                                                          1000000.0f);
                const i32 maximum_value = static_cast<i32>(timebar_unsigned_float(maximum) * 15734.0f /
                                                           1000000.0f);
                NuQFntPrintEx(system_qfont, centre_x, y + height, 0x40, "%d", MIN(recent_value, 9999));
                NuQFntPrintEx(system_qfont, centre_x, screen_height - static_cast<i32>(NuQFntHeight(system_qfont) * 2.0f),
                              0x40, "%d", maximum_value);
                if (timebar->slot_names[slot] != NULL) {
                    NuQFntPrintEx(system_qfont, centre_x,
                                  screen_height - static_cast<i32>(NuQFntHeight(system_qfont) * 3.0f), 0x40, "%s",
                                  timebar->slot_names[slot]);
                }
                NuQFntPopPrintMode();
                NuQFntPopCoordinateSystem();
            }

            if (timebar->field_28 != 0) {
                NuTimeBarSlotReset(set, slot);
            }
            if (NuTimeBar_PeakReset != 0) {
                maximum = 0;
            }
        }

        NuTimeBar_PeakReset = 0;
        --NuTimeBar_RenderPeakReset;
        if (NuTimeBar_RenderPeakReset < 0) {
            NuTimeBar_RenderPeakReset = 30;
        }
    }

    if (gpu_frames > 1.0f) {
        NuRndrRect2di(0, 0, 0x3c0, 0xe00, 0x80000064, NuTimeBar_FrameOutMtl);
        NuRndrRect2di(0x2440, 0, 0x3c0, 0xe00, 0x80000064, NuTimeBar_FrameOutMtl);
    }
    NuRndrEndScene();
}

extern "C" void NuTimeBarEnable(i32 enabled) {
    NuTimeBar_EngineEnabled = enabled;
}

extern "C" void NuTimeBarIndicateGpuFrameOut(i32 enabled) {
    NuTimeBar_GpuFrameOutEnabled = enabled;
}

extern "C" void NuTimeBarDestroySet(i32 set) {
    TimeBarSet *timebar = NuTimeBar_SetList[set + 1];
    if (timebar->uses_buffer == 0) {
        NU_FREE(timebar->accumulators[0]);
        NU_FREE(timebar->accumulators[1]);
        NU_FREE(timebar->start_times);
        NU_FREE(timebar->field_14);
        NU_FREE(timebar->field_18);
        NU_FREE(timebar->colours);
        NU_FREE(timebar->toggle_flags);
        NU_FREE(timebar->slot_names);
        NU_FREE(timebar);
    }
    NuTimeBar_SetList[set + 1] = NULL;
}
