#include "decomp.h"
#include "globals.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/items/base/apiobject.h"
#include "nu2api/nucore/nustring.h"

#include <string.h>

struct nupad_s;

void GameAudio_PlaySfx(i32, nuvec_s *, i32, i32);
void ConstantRumble(GameObject_s *, f32, f32);
void NewRumble(nupad_s *, f32, i32);
void GameCam_HitRoll();
void *AddGameMessage(char *, nuvec_s *, f32, nuvec_s *, f32, u8, u8, u8, u32, f32);
void NewRumbleAllPlayers(f32, f32, i32, i32);
i32 qrand();

struct CHEATSYSTEM {
    CHEAT *cheats;
    i32 cheats_count;
    i32 flags;
};
DECOMP_ASSERT(sizeof(CHEATSYSTEM) == 0x0c, "CHEATSYSTEM size");

// GCC 4.7 emits this source-defined BSS group in reverse declaration order.
// The declarations therefore reproduce POWERUP_TEXTID, CheatSystem and
// Cheat_PowerUpTime at the start of the original cheats.cpp BSS contribution.
static f32 Cheat_PowerUpTime;
static CHEATSYSTEM CheatSystem;
i32 POWERUP_TEXTID;

// The original initialized-data order is ONEPLAYERPOWERUPS followed by the
// duration used by both the per-player and shared power-up paths.
f32 CHEAT_POWERUPTIME = 20.0f;
i32 ONEPLAYERPOWERUPS = 1;

void Cheats_Init(CHEAT *cheats) {
    CheatSystem.cheats_count = 0;
    CheatSystem.cheats = cheats;
    if (cheats != NULL && cheats[0].name != NULL) {
        i32 count = 0;
        do {
            count++;
        } while (cheats[count].name != NULL);
        CheatSystem.cheats_count = count;
    }
}

i32 Cheat_FindByName(char *name) {
    for (i32 i = 0; i < CheatSystem.cheats_count; i++) {
        if (NuStrICmp(CheatSystem.cheats[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void Cheats_SetFlags() {
    i32 count = CheatSystem.cheats_count;
    CheatSystem.flags = 0;
    if (count > 0) {
        i32 flags = 0;
        f32 powerup_time = Cheat_PowerUpTime;
        i32 vehicle_area = VehicleArea;
        if (ONEPLAYERPOWERUPS == 0) {
            if (powerup_time > 0.0009765625f) {
                if (vehicle_area != 0) {
                    for (i32 i = 0; i < count; i++) {
                        if (CheatSystem.cheats[i].enabled || (CheatSystem.cheats[i].flag & 0x20000)) {
                            flags |= CheatSystem.cheats[i].flag;
                        }
                    }
                } else {
                    for (i32 i = 0; i < count; i++) {
                        if (CheatSystem.cheats[i].enabled || (CheatSystem.cheats[i].flag & 0x10000)) {
                            flags |= CheatSystem.cheats[i].flag;
                        }
                    }
                }
            } else {
                for (i32 i = 0; i < count; i++) {
                    if (CheatSystem.cheats[i].enabled) {
                        flags |= CheatSystem.cheats[i].flag;
                    }
                }
            }
        } else {
            for (i32 i = 0; i < count; i++) {
                if (CheatSystem.cheats[i].enabled) {
                    flags |= CheatSystem.cheats[i].flag;
                }
            }
        }
        CheatSystem.flags = flags;
    }
}

u32 Cheats_CheckFlags(u32 flag) {
    return CheatSystem.flags & flag;
}

u32 Cheat_CheckFlags(i32 cheat_index, u32 flag_mask) {
    if (cheat_index > -1 && cheat_index < CheatSystem.cheats_count) {
        CHEAT target_cheat = CheatSystem.cheats[cheat_index];
        return flag_mask & target_cheat.flag;
    }
    return 0;
}

void Cheat_SetOn(i32 cheat, i32 on, i32) {
    if (cheat < 0 || cheat >= CheatSystem.cheats_count) {
        return;
    }
    CheatSystem.cheats[cheat].enabled = on != 0;
    Cheats_SetFlags();
}

i32 Cheat_IsOn(i32 cheat) {
    if (cheat >= 0 && cheat < CheatSystem.cheats_count) {
        if (CheatSystem.cheats[cheat].enabled != 0) {
            return 1;
        }
        if (ONEPLAYERPOWERUPS == 0 && Cheat_PowerUpTime > 0.0f) {
            u8 vehicle_flag = reinterpret_cast<u8 *>(&CheatSystem.cheats[cheat].flag)[2];
            vehicle_flag &= VehicleArea == 0 ? 1U : 2U;
            if (vehicle_flag != 0) {
                return 1;
            }
        }
    }
    return 0;
}

void Cheat_GetOnOffBitfield(i32 *onoffs, i32 count) {
    memset(onoffs, 0, ((count + 31) / 32) << 2);
    for (i32 i = 0; i < count; i++) {
        if (CheatSystem.cheats[i].enabled) {
            onoffs[i >> 5] |= 1 << i;
        }
    }
}

void Cheat_SetOnOffBitfield(i32 *onoffs, i32 count) {
    for (i32 i = 0; i < count; i++) {
        CheatSystem.cheats[i].enabled = ((onoffs[i >> 5] >> i) & 1) ? 1 : 0;
    }
}

u32 Cheat_MultiplyScore(u32 score) {
    score = Cheats_CheckFlags(0x4) ? score * 2 : score;
    score = Cheats_CheckFlags(0x8) ? score * 4 : score;
    score = Cheats_CheckFlags(0x10) ? score * 6 : score;
    score = Cheats_CheckFlags(0x20) ? score * 8 : score;
    score = Cheats_CheckFlags(0x40) ? score * 10 : score;
    return score;
}

void Cheats_TurnOff(i32 cheat) {
    i32 count = CheatSystem.cheats_count;
    if (count > 0) {
        CHEAT *cheats = CheatSystem.cheats;
        if (cheat != 0) {
            for (i32 i = 0; i < count; i++) {
                if (cheats[i].flag & 0x200000) {
                    cheats[i].enabled = 0;
                }
            }
        } else {
            for (i32 i = 0; i < count; i++) {
                cheats[i].enabled = 0;
            }
        }
    }
    Cheats_SetFlags();
}

void Cheat_StartPowerUp(nuvec_s *position, GameObject_s *object) {
    if (ONEPLAYERPOWERUPS != 0) {
        if (object == NULL) {
            return;
        }
        object->field_0xdec = CHEAT_POWERUPTIME;
        void *controller = *reinterpret_cast<void **>(reinterpret_cast<char *>(object) + 0xc94);
        NewRumble(*reinterpret_cast<nupad_s **>(controller), 0.7f, 0);
        GameCam_HitRoll();
    } else {
        NewRumbleAllPlayers(0.7f, 0.0f, 0, 0);
        Cheat_PowerUpTime = CHEAT_POWERUPTIME;
    }

    char *message = TTab[POWERUP_TEXTID];
    void *display = AddGameMessage(message, position, 0.5f, position, 0.75f, 0xff, 0xff, 0xff, 0x4023, 1.0f);
    if (display != NULL) {
        *reinterpret_cast<f32 *>(static_cast<char *>(display) + 0xd4) = 0.75f;
    }
    display = AddGameMessage(message, position, 0.5f, position, 0.25f, 0xff, 0xff, 0xff, 0x4023, 1.0f);
    if (display != NULL) {
        *reinterpret_cast<f32 *>(static_cast<char *>(display) + 0xd4) = 0.75f;
    }
    GameAudio_PlaySfx(0x50, NULL, 0, 0);
}

i32 Cheat_PowerUpActive(i32 index) {
    if (ONEPLAYERPOWERUPS != 0) {
        if (static_cast<u32>(index) <= 1) {
            if (Player[0] != NULL && Player[0]->field_0xdec > 0.0009765625f && Player[0]->apiobj.field_0x27c == index) {
                return 1;
            }
            if (Player[1] != NULL && Player[1]->field_0xdec > 0.0009765625f && Player[1]->apiobj.field_0x27c == index) {
                return 1;
            }
        }
        return 0;
    }
    return Cheat_PowerUpTime > 0.0009765625f;
}

void Cheats_Reset() {
    Cheat_PowerUpTime = 0.0f;
    Cheats_SetFlags();
}

void Cheats_Update() {
    if (ONEPLAYERPOWERUPS == 0 && Cheat_PowerUpTime > 0.0f) {
        Cheat_PowerUpTime -= FRAMETIME;
        if (Cheat_PowerUpTime <= 0.0f) {
            GameAudio_PlaySfx(0x52, NULL, 0, 0);
        } else {
            GameAudio_PlaySfx(0x51, NULL, 0, 0);
            ConstantRumble(NULL, qrand() * 0.0009765625f * 0.03125f, 0.0f);
        }
    }
    Cheats_SetFlags();
}

void Cheat_SetArea(i32 cheat, i32 area_id) {
    if (cheat >= 0 && cheat < CheatSystem.cheats_count && area_id >= 0 && area_id < AREACOUNT) {
        CheatSystem.cheats[cheat].area = area_id;
    }
}
