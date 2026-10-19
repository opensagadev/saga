#include "nu2api/nucore/nuvuvec.hpp"
#include "decomp.h"
#include <float.h>
#include "nu2api/nucore/nucore.hpp"
#include "nu2api/nucore/nuapi.h"
#include <new>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nucore/NuCopyFilter.h"
#include "nu2api/nucore/NuDataPortManager.h"
#include "nu2api/nucore/NuDeferredFilter.h"
#include "nu2api/nucore/NuDeferredFilterGen.h"
#include "nu2api/nucore/NuDeviceSpecs.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nucore/NuMainFilter.h"
#include "nu2api/nucore/NuMainFilterGen.h"
#include "nu2api/nucore/NuMotionAccumFilter.h"
#include "nu2api/nucore/NuMotionAccumFilterGen.h"
#include "nu2api/nucore/NuMotionFilter.h"
#include "nu2api/nucore/NuMotionFilterGen.h"
#include "nu2api/nucore/NuNetEmu.h"
#include "nu2api/nucore/NuPlatform.h"
#include "nu2api/nucore/NuPostFilter.h"
#include "nu2api/nucore/NuPostFilterGen.h"
#include "nu2api/nucore/NuSpeedBlurFilter.h"
#include "nu2api/nucore/NuSpeedBlurFilterGen.h"
#include "nu2api/nucore/NuVoiceAndroid.h"
#include "gamelib/util/Refpack.h"
#include "gamelib/util/Utilities.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/nushader.h"
#include "nu2api/nu3d/nupostdraw.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nupostshaders.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nucore/nuvuvectype.hpp"
#include "nu2api/nucore/unref.h"
#include "nu2api/nu3d/nuqfnt.h"

#include <stdio.h>

// Network condition emulator (retail nunetemu.cpp): packet batching,
// splitting and stats for the emulated link.

NuNetEmu theNuNetEmu;

typedef i32 (*NuNetSessionSendToFn)(NetSession *, void *, i32, nunetaddr_s *);
typedef i32 (*NuNetSessionRecvFromFn)(NetSession *, void *, i32, nunetaddr_s *);

static inline i32 NuNetSessionSendTo(void *data, i32 size, nunetaddr_s *address) {
    void **vtable = *reinterpret_cast<void ***>(theSession);
    return reinterpret_cast<NuNetSessionSendToFn>(vtable[22])(theSession, data, size, address);
}

static inline i32 NuNetSessionRecvFrom(void *data, i32 size, nunetaddr_s *address) {
    void **vtable = *reinterpret_cast<void ***>(theSession);
    return reinterpret_cast<NuNetSessionRecvFromFn>(vtable[23])(theSession, data, size, address);
}

struct NuNetEmuSegment {
    u16 offset;
    u16 size;
};

// The retained emulator uses this sixteen-bit unsigned conversion closure.

static i32 cbSortSeg(void const *left, void const *right) {
    u16 right_size = static_cast<NuNetEmuSegment const *>(right)->size;
    u16 left_size = static_cast<NuNetEmuSegment const *>(left)->size;
    if (left_size < right_size) {
        return 1;
    }
    if (left_size > right_size) {
        return -1;
    }
    return 0;
}

void NuNetEmu::PackStats::Draw(float x, float y, float width, float height, NetSmallStats::eInfo info) const {
    NetStats::Draw(x, y, width, height, info);

    NuQFntPushCoordinateSystem(NUQFNT_CSMODE_NORMALISED);
    NuQFntPushPrintMode(2);
    NUQFNT *font = system_qfont;
    const float font_height = NuQFntHeight(font);
    y += font_height;
    y += font_height;
    y += font_height;
    y += font_height;
    y += font_height;
    NuQFntSetColour(font, 0x80808080);
    NuQFntSetScale(font, 0.75f, 0.75f);

    char text[128];
    y += font_height;
    NuQFntMove(font, x, y, 0.0f);
    sprintf(text, "Pack Ratio %.3f%%", pack_ratio);
    NuQFntPrintU(font, text);

    y += font_height;
    float first_time = 0.0f;
    float second_time = 0.0f;
    if (total.values[2] != 0) {
        first_time = static_cast<float>(packed_values[0]) / static_cast<float>(total.values[2]);
    }
    if (total.values[3] != 0) {
        second_time = static_cast<float>(packed_values[1]) / static_cast<float>(total.values[3]);
    }
    NuQFntMove(font, x, y, 0.0f);
    sprintf(text, "Pack Time %.2f : %.2f", first_time, second_time);
    NuQFntPrintU(font, text);

    y += font_height;
    NuQFntMove(font, x, y, 0.0f);
    sprintf(text, "Ave Packet %.1f", average_packet_size);
    NuQFntPrintU(font, text);

    y += font_height;
    NuQFntMove(font, x, y, 0.0f);
    sprintf(text, "Split Packets %d", split_packets);
    NuQFntPrintU(font, text);

    y += font_height;
    NuQFntMove(font, x, y, 0.0f);
    sprintf(text, "Held Packets %d", held_packets);
    NuQFntPrintU(font, text);

    NuQFntPopPrintMode();
    NuQFntPopCoordinateSystem();
}

NuNetEmu::EmuPacket *NuNetEmu::FindPacket(nunetaddr_s *, i32 size) {
    EmuPacket *packet = field_04;
    u32 now = UtilGetFrameStartTime();
    size += 2;
    while (packet != NULL) {
        if (size <= (i32)(0xbb8 - packet->payload_size) &&
            (packet->flush_time >= packet->send_time || now <= packet->flush_time)) {
            break;
        }
        packet = packet->next;
    }
    return packet;
}

i32 NuNetEmu::RecvFrom(void *data, i32 size, nunetaddr_s &address) {
    if (field_10 == 0) {
        return NuNetSessionRecvFrom(data, size, &address);
    }

    i32 received = 0;
    if (__builtin_expect(field_17b0 >= field_17b4, 0)) {
        received = NuNetSessionRecvFrom(packed_buffer, 0xbb8, &address);
        if (received > 0) {
            packet_stats.total.values[1] += received;
            packet_stats.total.values[3]++;
            field_17b0 = 0;
            field_17b8 = received;
            u32 start_time = UtilGetTime();
            field_17b4 = unref(packed_buffer, unpacked_buffer);
            packet_stats.packed_values[1] += UtilGetTime() - start_time;
            raw_stats.total.values[3]++;
            raw_stats.total.values[1] += field_17b4;
        }
    }

    if (field_17b0 < field_17b4 && received >= 0) {
        i32 payload_size = unpacked_buffer[field_17b0] | (unpacked_buffer[field_17b0 + 1] << 8);
        i32 copy_size = payload_size <= size ? payload_size : size;
        memmove(data, unpacked_buffer + field_17b0 + 2, copy_size);
        field_17b0 += payload_size + 2;
        return copy_size;
    }
    return received;
}

NuNetEmu::EmuPacket::EmuPacket(nunetaddr_s *net_address) {
    if (this != NULL) {
        next = NULL;
        previous = NULL;
    }
    payload = static_cast<u8 *>(theMemoryManager.AllocPool(0xbb8, 1));
    payload_size = 0;
    payload_count = 0;
    address = *reinterpret_cast<u32 *>(net_address);
}

NuNetEmu::EmuPacket::~EmuPacket() {
    theMemoryManager.FreePool(payload, 0xbb8);
}

i32 NuNetEmu::SplitSendPacket(NuNetEmu::EmuPacket *packet) {
    u32 start_time = UtilGetTime();
    field_17b8 = refpack(packet->payload, packet->payload_size, packed_buffer);
    packet_stats.packed_values[0] += UtilGetTime() - start_time;

    i32 packed_size = field_17b8;
    if (packed_size <= 0x4f0) {
        packet_stats.total.values[0] += packed_size;
        packet_stats.total.values[2]++;
        NuNetSessionSendTo(packed_buffer, packed_size, reinterpret_cast<nunetaddr_s *>(&packet->address));
        return packed_size;
    }

    packet_stats.split_packets++;
    NuNetEmuSegment segments[256];
    u32 segment_count = 0;
    u32 offset = 0;
    while (offset < packet->payload_size && segment_count < 256) {
        i32 payload_size = packet->payload[offset] | (packet->payload[offset + 1] << 8);
        segments[segment_count].offset = offset;
        segments[segment_count].size = payload_size + 2;
        segment_count++;
        offset += payload_size + 2;
    }
    qsort(segments, segment_count, sizeof(NuNetEmuSegment), cbSortSeg);

    EmuPacket *first = new (theMemoryManager.AllocPool(sizeof(EmuPacket), 1))
        EmuPacket(reinterpret_cast<nunetaddr_s *>(&packet->address));
    EmuPacket *second = new (theMemoryManager.AllocPool(sizeof(EmuPacket), 1))
        EmuPacket(reinterpret_cast<nunetaddr_s *>(&packet->address));

    EmuPacket *packets[2] = {first, second};
    for (u32 i = 0; i < segment_count; i++) {
        EmuPacket *destination = packets[second->payload_size < first->payload_size];
        memmove(destination->payload + destination->payload_size, packet->payload + segments[i].offset,
                segments[i].size);
        destination->payload_size += segments[i].size;
    }

    i32 sent = SplitSendPacket(first) + SplitSendPacket(second);
    if (first != NULL) {
        first->~EmuPacket();
        theMemoryManager.FreePool(first, sizeof(EmuPacket));
    }
    if (second != NULL) {
        second->~EmuPacket();
        theMemoryManager.FreePool(second, sizeof(EmuPacket));
    }
    return sent;
}

static inline f32 NetEmuUnsignedFloat(u32 value) {
    return static_cast<f32>(value);
}

void NuNetEmu::Update() {
    u32 now = UtilGetFrameStartTime();
    if (now > static_cast<u32>(field_17bc)) {
        u32 bandwidth = field_0c > 2 ? field_17c4 : field_17c0;
        u32 budget = bandwidth / 30;
        u32 sent = 0;
        EmuPacket *packet = field_04;
        while (packet != NULL && sent < budget) {
            if (now >= packet->send_time && (now >= packet->flush_time || packet->payload_size >= field_38)) {
                EmuPacket *next = packet->next;
                raw_stats.total.values[0] += packet->payload_size;
                raw_stats.total.values[2]++;
                sent += SplitSendPacket(packet);

                if (packet->next != NULL) {
                    packet->next->previous = packet->previous;
                } else {
                    field_08 = packet->previous;
                }
                if (packet->previous != NULL) {
                    packet->previous->next = packet->next;
                } else {
                    field_04 = packet->next;
                }
                packet->next = NULL;
                packet->previous = NULL;
                field_0c--;
                packet->~EmuPacket();
                theMemoryManager.FreePool(packet, sizeof(EmuPacket));
                packet = next;
            } else {
                packet = packet->next;
            }
        }
        field_17bc = now + static_cast<i32>(NetEmuUnsignedFloat(sent) / (NetEmuUnsignedFloat(bandwidth) / 1000.0f));
    }
    field_1c = 0;
    for (EmuPacket *packet = field_04; packet != NULL; packet = packet->next) {
        if (packet->flush_time >= packet->send_time || now <= packet->flush_time) {
            field_1c += packet->payload_size;
        }
    }

    raw_stats.Update();
    packet_stats.Update();
    {
        f32 ratio = 0.0f;
        f32 raw_size = NetEmuUnsignedFloat(raw_stats.total.values[0]);
        if (raw_size > 0.0f) {
            ratio = NetEmuUnsignedFloat(packet_stats.total.values[0]) / raw_size;
        }
        packet_stats.pack_ratio = ratio;

        f32 average = 0.0f;
        if (packet_stats.total.values[2] > 0) {
            average =
                NetEmuUnsignedFloat(packet_stats.total.values[0]) / NetEmuUnsignedFloat(packet_stats.total.values[2]);
        }
        packet_stats.average_packet_size = average;
    }
    packet_stats.held_packets = field_0c;
}

void NuNetEmu::EmuPacket::AddPayload(void *data, i32 size) {
    payload[payload_size] = size;
    payload[payload_size + 1] = size >> 8;
    memmove(payload + payload_size + 2, data, size);
    payload_count++;
    payload_size = payload_size + size + 2;
}

i32 NuNetEmu::SendTo(void *data, i32 size, nunetaddr_s *address, i32) {
    if (field_10 == 0) {
        return NuNetSessionSendTo(data, size, address);
    }

    if (field_24 != 1) {
        if (field_24 == 2) {
            if (field_30 == 0) {
                f32 random = NuRandFloat();
                if (field_34 > random) {
                    f32 range = static_cast<f32>(field_2c - field_28);
                    f32 random = NuRandFloat();
                    field_30 = static_cast<i32>(random * range) + field_28;
                }
            }
            if (field_30 > 0) {
                field_30--;
                return size;
            }
        }
    } else {
        f32 random = NuRandFloat();
        if (field_34 > random) {
            return size;
        }
    }

    if (field_0c >= field_17c8) {
        return 0;
    }

    EmuPacket *packet = FindPacket(address, size);
    if (packet == NULL) {
        packet = new (theMemoryManager.AllocPool(sizeof(EmuPacket), 1)) EmuPacket(address);
        u32 now = UtilGetFrameStartTime();
        if (field_18 > 0) {
            f32 range = static_cast<f32>(field_18 - field_14);
            f32 random = NuRandFloat();
            packet->send_time = now + (static_cast<u32>(static_cast<i32>(random * range)) + static_cast<u32>(field_14));
        } else {
            packet->send_time = 0;
        }
        packet->creation_time = now;
        packet->flush_time = now + field_3c;

        packet->next = NULL;
        packet->previous = field_08;
        if (field_08 != NULL) {
            field_08->next = packet;
        }
        field_08 = packet;
        if (field_04 == NULL) {
            field_04 = packet;
        }
        field_0c++;
    }
    packet->AddPayload(data, size);
    field_1c += size;
    return size;
}

void NuNetEmu::SetConditions(NuNetEmu::eConditions conditions) {
    field_20 = conditions;
    switch (conditions) {
        case CONDITIONS_NORMAL:
            field_24 = 0;
            field_14 = 0;
            field_18 = 0;
            break;
        case CONDITIONS_1:
            field_24 = 1;
            field_14 = 20;
            field_34 = 0.005f;
            field_18 = 40;
            break;
        case CONDITIONS_2:
            field_24 = 1;
            field_14 = 50;
            field_34 = 0.02f;
            field_18 = 70;
            break;
        case CONDITIONS_3:
            field_24 = 2;
            field_28 = 5;
            field_34 = 0.01f;
            field_2c = 15;
            field_14 = 100;
            field_18 = 120;
            break;
    }
}

NuNetEmu::NuNetEmu() : field_04(NULL), field_08(NULL), field_0c(0), raw_stats("EmuRaw"), packet_stats("EmuPack") {
    field_10 = 0;
    field_14 = 0;
    field_18 = 0;
    field_20 = 0;
    field_24 = 0;
    field_28 = 0;
    field_2c = 0;
    field_30 = 0;
    field_34 = 0.0f;
    field_38 = 0x320;
    field_3c = 0x32;
    field_17bc = 0;
    field_17c0 = 0x1800;
    field_17c4 = 0x2800;
    field_17c8 = 0x80;
    field_17b0 = 0;
    field_17b4 = 0;
    field_17b8 = 0;
    SetConditions(CONDITIONS_NORMAL);
    field_00 = 0x200;
}
