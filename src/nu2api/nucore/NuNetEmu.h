#pragma once

#include "gamelib/util/gamelib_util_types.h"

struct nunetaddr_s;

struct NuNetEmu {
    struct EmuPacket {
        void AddPayload(void *, i32);
        EmuPacket(nunetaddr_s *);
        ~EmuPacket();
    };
    struct PackStats : NetStats {
        explicit PackStats(char const *name) : NetStats(name) {
        }
        void Draw(float, float, float, float, NetSmallStats::eInfo) const;
    };
    enum eConditions {
        CONDITIONS_NORMAL = 0,
        CONDITIONS_1 = 1,
        CONDITIONS_2 = 2,
        CONDITIONS_3 = 3,
    };
    void FindPacket(nunetaddr_s *, i32);
    NuNetEmu();
    void RecvFrom(void *, i32, nunetaddr_s &);
    void SendTo(void *, i32, nunetaddr_s *, i32);
    void SetConditions(NuNetEmu::eConditions);
    void SplitSendPacket(NuNetEmu::EmuPacket *);
    void Update();

    i32 field_00;
    i32 field_04;
    i32 field_08;
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
    u8 reserved_40[0x1770];
    i32 field_17b0;
    i32 field_17b4;
    i32 field_17b8;
    i32 field_17bc;
    i32 field_17c0;
    i32 field_17c4;
    i32 field_17c8;
    NetStats raw_stats;
    PackStats packet_stats;
    u8 reserved_1c0c[0x10];
    float field_1c1c;
    u8 reserved_1c20[4];
};

DECOMP_ASSERT(sizeof(NuNetEmu) == 0x1c24, "NuNetEmu ABI");
DECOMP_ASSERT(offsetof(NuNetEmu, field_1c) == 0x1c, "NuNetEmu field_1c offset");
DECOMP_ASSERT(offsetof(NuNetEmu, raw_stats) == 0x17cc, "NuNetEmu raw stats offset");
DECOMP_ASSERT(offsetof(NuNetEmu, packet_stats) == 0x19ec, "NuNetEmu packet stats offset");
DECOMP_ASSERT(offsetof(NuNetEmu, field_1c1c) == 0x1c1c, "NuNetEmu field_1c1c offset");

extern NuNetEmu theNuNetEmu;
