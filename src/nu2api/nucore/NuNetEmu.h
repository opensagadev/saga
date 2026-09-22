#pragma once

#include "gamelib/util/gamelib_util_types.h"

struct nunetaddr_s;

struct NuNetEmu {
    struct EmuPacket {
        EmuPacket *next;
        EmuPacket *previous;
        u32 address;
        u8 *payload;
        u32 payload_size;
        u32 payload_count;
        u32 flush_time;
        u32 send_time;
        u32 creation_time;

        void AddPayload(void *, i32);
        EmuPacket(nunetaddr_s *);
        ~EmuPacket();
    };
    struct PackStats : NetStats {
        explicit PackStats(char const *name) : NetStats(name) {
        }
        void Draw(float, float, float, float, NetSmallStats::eInfo) const;

        u32 packed_values[2];
        u32 split_packets;
        u32 held_packets;
        f32 pack_ratio;
        f32 average_packet_size;
    };
    enum eConditions {
        CONDITIONS_NORMAL = 0,
        CONDITIONS_1 = 1,
        CONDITIONS_2 = 2,
        CONDITIONS_3 = 3,
    };
    EmuPacket *FindPacket(nunetaddr_s *, i32);
    NuNetEmu();
    i32 RecvFrom(void *, i32, nunetaddr_s &);
    i32 SendTo(void *, i32, nunetaddr_s *, i32);
    void SetConditions(NuNetEmu::eConditions);
    i32 SplitSendPacket(NuNetEmu::EmuPacket *);
    void Update();

    i32 field_00;
    EmuPacket *field_04;
    EmuPacket *field_08;
    i32 field_0c;
    u8 field_10;
    u8 reserved_11[3];
    i32 field_14;
    i32 field_18;
    i32 field_1c;
    i32 field_20;
    i32 field_24;
    i32 field_28;
    i32 field_2c;
    i32 field_30;
    float field_34;
    i32 field_38;
    i32 field_3c;
    u8 packed_buffer[0xbb8];
    u8 unpacked_buffer[0xbb8];
    i32 field_17b0;
    i32 field_17b4;
    i32 field_17b8;
    i32 field_17bc;
    i32 field_17c0;
    i32 field_17c4;
    i32 field_17c8;
    NetStats raw_stats;
    PackStats packet_stats;
};

DECOMP_ASSERT(sizeof(NuNetEmu::PackStats) == 0x238, "NuNetEmu::PackStats ABI");
DECOMP_ASSERT(sizeof(NuNetEmu::EmuPacket) == 0x24, "NuNetEmu::EmuPacket ABI");
DECOMP_ASSERT(offsetof(NuNetEmu::EmuPacket, payload) == 0xc, "NuNetEmu::EmuPacket payload offset");
DECOMP_ASSERT(offsetof(NuNetEmu::EmuPacket, payload_size) == 0x10, "NuNetEmu::EmuPacket payload size offset");
DECOMP_ASSERT(offsetof(NuNetEmu::EmuPacket, flush_time) == 0x18, "NuNetEmu::EmuPacket flush time offset");
DECOMP_ASSERT(offsetof(NuNetEmu::PackStats, packed_values) == 0x220, "NuNetEmu::PackStats packed values offset");
DECOMP_ASSERT(offsetof(NuNetEmu::PackStats, pack_ratio) == 0x230, "NuNetEmu::PackStats ratio offset");
DECOMP_ASSERT(sizeof(NuNetEmu) == 0x1c24, "NuNetEmu ABI");
DECOMP_ASSERT(offsetof(NuNetEmu, field_1c) == 0x1c, "NuNetEmu field_1c offset");
DECOMP_ASSERT(offsetof(NuNetEmu, raw_stats) == 0x17cc, "NuNetEmu raw stats offset");
DECOMP_ASSERT(offsetof(NuNetEmu, packet_stats) == 0x19ec, "NuNetEmu packet stats offset");
DECOMP_ASSERT(offsetof(NuNetEmu, packet_stats) + offsetof(NuNetEmu::PackStats, pack_ratio) == 0x1c1c,
              "NuNetEmu packet ratio offset");

extern NuNetEmu theNuNetEmu;
