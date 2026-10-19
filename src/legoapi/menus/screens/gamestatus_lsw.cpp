// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include <string.h>
#include <stdio.h>

#include "decomp.h"
#include "globals.h"
#include "gameframework/saveload.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/screens/gamestatus_lsw.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "gameapi/gui/apimenu.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/core/gamemessage.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/props/doors/door.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/core/net/netplay.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/menus/core/gamehint.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/render/fx/edsplines.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include <math.h>
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include "nu2api/nu3d/nuportal.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nu3d/nugscn.h"
#include "legoapi/core/startup/text.h"
#include "legoapi/misc/legoapi_status.h"
#include "legoapi/misc/utilities.h"

extern "C" STATUSPACKET_s StatusPacket;

extern i16 tCOINTOTAL;
extern i16 tSELECT;
extern i16 tSELECTING;
f32 STATUS_TITLE_Y = 0.5f;

f32 iconalphaoverride;
f32 icon_y;
i32 draw_player_icons;
i32 status_prompt;

STATUSPACKET_LSW_s StatusPacket_LSW;
STATUS_STAGE_s StatusStages_LSW[] = {
    {RedBrick_LSW_Draw, RedBrick_LSW_Update, RedBrick_LSW_Skip, 18, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {CollectCharcters_Draw, CollectCharcters_Update, CollectCharcters_Skip, 35, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {CollectCharactersOff_Draw, CollectCharactersOff_Update, CollectCharactersOff_Skip, 36, 0xffff, 0, 0, 0, 0.0f,
     0.0f},
    {Coins_LSW_Draw, Coins_LSW_Update, Coins_LSW_Skip, 3, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {TrueHero_LSW_Draw, TrueHero_LSW_Update, TrueHero_LSW_Skip, 1, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {TrueHero_LSW_Draw, TrueHero_LSW_Update, TrueHero_LSW_Skip, 2, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {MiniKit_LSW_Draw, MiniKit_LSW_Update, MiniKit_LSW_Skip, 4, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {AllMiniKits_LSW_Draw, AllMiniKits_LSW_Update, AllMiniKits_LSW_Skip, 17, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {GoldBrick_LSW_Draw, GoldBrick_LSW_Update, GoldBrick_LSW_Skip, 19, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 13, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 8, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 28, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 29, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 30, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 31, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 32, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 33, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 34, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 14, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 23, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {BonusTime_LSW_Draw, BonusTime_LSW_Update, BonusTime_LSW_Skip, 24, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {ChallangeCash_Draw, ChallangeCash_Update, ChallangeCash_Skip, 25, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {LevelComplete_LSW_Draw, LevelComplete_LSW_Update, LevelComplete_LSW_Skip, 26, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {BonusTime_LSW_Draw, BonusTime_LSW_Update, BonusTime_LSW_Skip, 27, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {SuperStoryTime_LSW_Draw, SuperStoryTime_LSW_Update, SuperStoryTime_LSW_Skip, 15, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {SuperStoryScore_LSW_Draw, SuperStoryScore_LSW_Update, SuperStoryScore_LSW_Skip, 16, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {BonusComplete_LSW_Draw, BonusComplete_LSW_Update, BonusComplete_LSW_Skip, 22, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {BonusWin_LSW_Draw, BonusWin_LSW_Update, BonusWin_LSW_Skip, 20, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {BonusTime_LSW_Draw, BonusTime_LSW_Update, BonusTime_LSW_Skip, 21, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {Prompt_LSW_Draw, Prompt_LSW_Update, NULL, 9, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {NULL, Exit_LSW_Update, NULL, 10, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {NULL, Save_LSW_Update, NULL, 11, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {NULL, Fade_LSW_Update, NULL, 12, 0xffff, 0, 0, 0, 0.0f, 0.0f},
    {NULL, NULL, NULL, -1, 0xffff, 0, 0, 0, 0.0f, 0.0f},
};

extern i16 tSUPERSTORYCOMPLETE, tNEWBESTTIME, tLEVELCOMPLETE, tMISSIONCOMPLETE;
extern i16 tNONEWBESTTIME;
extern i16 tNEWHIGHSCORE, tNONEWHIGHSCORE;
extern i16 tCHALLENGECOMPLETE, tMINIKIT;
i32 newminikitcount;
i32 currentminikit;

extern i32 STATUS_R, STATUS_G, STATUS_B;
extern i16 tUNLOCKED;
extern i16 tTRUEJEDI, tSTORY, tFREEPLAY;
static f32 nextsoundpercent = 5.0f;

static __attribute__((noinline, noclone)) void SuperStoryTime_LSW_DrawBody(STATUS_STAGE_s *stage, STATUSPACKET_s *packet)
    __asm__("_Z23SuperStoryTime_LSW_DrawP14STATUS_STAGE_sP14STATUSPACKET_si.part.12");

static __attribute__((noinline, noclone)) void SuperStoryScore_LSW_DrawBody(STATUS_STAGE_s *stage, STATUSPACKET_s *packet)
    __asm__("_Z24SuperStoryScore_LSW_DrawP14STATUS_STAGE_sP14STATUSPACKET_si.part.8");

extern i16 tFREEPLAYUNLOCKED, tEPISODEVUNLOCKED, tEPISODEVIUNLOCKED, tBONUSUNLOCKED;
extern i16 tOUTOFTIME, tYOUDIDNTCOLLECTALL10CANISTERS, tSTORYCLIPSUNLOCKED;
extern i16 tEPISODEICOMPLETE, tEPISODEIICOMPLETE, tEPISODEIIICOMPLETE;
extern i16 tEPISODEIVCOMPLETE, tEPISODEVCOMPLETE, tEPISODEVICOMPLETE;

// Retail keeps the active-stage drawing as the split-out .part clone of
// LevelComplete_LSW_Draw; the inline head only tests `active`.
static __attribute__((noinline, noclone)) void LevelComplete_LSW_DrawActive(STATUS_STAGE_s *stage,
                                                                         STATUSPACKET_s *packet)
    __asm__("_Z22LevelComplete_LSW_DrawP14STATUS_STAGE_sP14STATUSPACKET_si.part.27");

extern i16 tWINNER;

struct STATUSPACKET_s;
struct nuhspecial_s;

extern i16 tNONEWBESTTIME, tNEWBESTTIME;

extern i32 currentminikit;
extern i16 tPOWERBRICK, tLOCKED, tGOLDBRICK;

extern i16 id_ANAKINJEDISCARRED, id_WOOKIEE, id_TWOFACE;

f32 COLLECTION_Y_STATUS = -0.3f;
static i32 playedappearsfx;
static i32 playedmovesfx;

extern i32 STATUS_R;
extern i32 STATUS_G;
extern i32 STATUS_B;
extern i16 tLEVELCOMPLETE, tFREEPLAYUNLOCKED, tSUPERSTORYCOMPLETE, tBONUSUNLOCKED;
extern i16 tEPISODEVUNLOCKED, tEPISODEVIUNLOCKED, tCHALLENGECOMPLETE, tOUTOFTIME;
extern i16 tYOUDIDNTCOLLECTALL10CANISTERS, tMISSIONCOMPLETE, tSTORYCLIPSUNLOCKED;
extern i32 currentminikit, newminikitcount;
extern i16 tMINIKIT;
extern i16 tALLMINIKITSBUILT;

i32 InitStatusScreen_LSW(WORLDINFO_s *, STATUSPACKET_s *) {
    STUBBED();
    return 0;
}

i32 FinishStatusPacket_LSW(WORLDINFO_s *, STATUSPACKET_s *, i32) {
    STUBBED();
    return 0;
}

void DrawStatusBG_LSW(STATUSPACKET_s *status) {
    STATUS_STAGE_s *stage = status->stage;
    if (stage->field_0x14 == -1) {
        stage->field_0x18 = 0;
        stage->field_0x14 = 0;
    }
}

void CollectCharactersOff_Skip(STATUS_STAGE_s *, STATUSPACKET_s *) {
}

void BonusTime_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *) {
    STUBBED();
}

void BonusWin_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *) {
    STUBBED();
}

void ChallangeCash_Skip(STATUS_STAGE_s *, STATUSPACKET_s *) {
    STUBBED();
}

void ResetStatusPacket_LSW(STATUSPACKET_s *packet) {
    packet->lsw_packet->field_0x00 = 0;
}

void SuperStoryScore_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet) {
    *packet->score = packet->final_reward_score;
    NextStatusStage(packet);
}

void SuperStoryTime_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet) {
    *packet->score = packet->time_reward_score;
    NextStatusStage(packet);
}

void BonusComplete_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet) {
    NextStatusStage(packet);
}

void LevelComplete_LSW_Skip(STATUS_STAGE_s *stage, STATUSPACKET_s *packet) {
    if (stage->type == 26 && packet->mission_state == 2) {
        *packet->score = packet->reward_score;
    }
    if (stage->type == 23 && packet->challenge_state == 2) {
        *packet->score = packet->reward_score;
    }
    NextStatusStage(packet);
}

void GoldBrick_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet) {
    if (Game.field_0x7c26[0] < 61)
        NextStatusStage(packet);
}

void RedBrick_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet) {
    NextStatusStage(packet);
}

void AllMiniKits_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet) {
    NextStatusStage(packet);
}

void TrueHero_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet) {
    NextStatusStage(packet);
}

void ChallangeCash_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 draw) {
    NUVEC position = {0.0f, -0.5f, 1.0f};
    if (draw != 0) {
        if (stage->field_0x14 != 0) {
            const f32 time = stage->field_0x18;
            iconalphaoverride = time >= 0.5f ? 1.0f : time * 2.0f;
            if (__builtin_expect(time < 2.0f, 0) && NuFmod(time, 0.2f) >= 0.1f) {
                const bool right = (packet->field_0xb0 & 0x20) != 0;
                SmartTextEx(TTab[tWINNER], right ? 0.675f : -0.675f, STATSPOSY, 1.0f, 0.7f, 0.7f, 0.7f, right ? 8 : 2,
                            0, 255, 0, 0.35f, 1, NULL, 0, static_cast<i32>(iconalphaoverride * 128.0f));
            }
            if (stage->field_0x14 == 1) {
                AddGameMessage(TTab[tCHALLENGECOMPLETE], &position, 0.7f, &position, 0.7f, 255, 0, 127, 0x4020, 2.0f);
            }
        } else {
            iconalphaoverride = 0.0f;
        }
    }
}

void ChallangeCash_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    NUVEC target = {0.0f, STATSPOSY, 1.0f};
    switch (stage->field_0x14) {
        case 0:
            stage->field_0x1c = 4.0f;
            stage->field_0x14 = 1;
            stage->field_0x18 = 0.0f;
            break;
        case 1: {
            f32 previous = stage->field_0x18;
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= 1.5f && FindGameMsgsWithID(1, 0, -1, NULL) == 0) {
                stage->field_0x14 = 2;
                stage->field_0x1c = 1.0f;
                stage->field_0x18 = 0.0f;
                break;
            }
            if (previous < 1.0f && stage->field_0x18 >= 1.0f) {
                f32 delay = 0.0f;
                for (i32 i = 0; i < 50; ++i) {
                    NUVEC position;
                    position.x = 0.0f;
                    position.y = QRAND_FLOAT() * 0.1f;
                    position.z = 1.0f;
                    NuVecRotateZ(&position, &position, qrand());
                    ADDGAMEMSG_ALIGNED16 message = AddGameMsg_Default;
                    message.position = &position;
                    message.target_scale = 0.75f;
                    message.player_index = qrand() / 0x8000;
                    message.target_position = &target;
                    target.x = cointotal_x[message.player_index];
                    message.target_scale = COINTOTAL_COINSIZE;
                    message.icon = static_cast<i16>(GizmoPickupType[2].first_model_id);
                    message.field_0x4e = 1;
                    message.flags = 0x112d;
                    message.special = &WORLD->lev_objs[message.icon].special;
                    message.duration = COINMSGTIME;
                    message.score = GizmoPickupType[2].score;
                    message.end_fn = EndScoreMessage;
                    message.field_0x20 = delay;
                    message.update_fn = GameMsg_DrawAdjustNewPos_CoinToTotal;
                    AddGameMsg(&message);
                    if (delay == 0.0f) {
                        NewStatusRumbleBuzz(-1, 0.0f, 0.0f, 1);
                        AddGameDebris(WORLD->debris_sys, 0x38, &position);
                    }
                    delay += 0.1f;
                }
            }
            break;
        }
        case 2:
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 > stage->field_0x1c) {
                PlaySfx(const_cast<char *>("Shop_BuyCheat"), NULL);
                NextStatusStage(packet);
            }
            break;
    }
}

void Coins_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet) {
    icon_y = StatusIconsOnOff(1.0f);
    IncreaseScore(packet->score, static_cast<u64>(packet->coins_remaining[0] + packet->coins_remaining[1]), 0);
    packet->coins_remaining[0] = 0;
    packet->coins_remaining[1] = 0;
    NextStatusStage(packet);
}

void MiniKit_LSW_Skip(STATUS_STAGE_s *stage, STATUSPACKET_s *packet) {
    currentminikit += newminikitcount;
    if (packet->new_minikit_complete && (stage->field_0x14 != 6 || !(stage->field_0x18 >= stage->field_0x1c)))
        IncreaseScore(packet->score, 50000, 0);
    NextStatusStage(packet);
}

void TrueHero_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    static f32 spdelay;
    switch (stage->field_0x14) {
        case 0:
            stage->field_0x14 = 1;
            stage->field_0x18 = 0.0f;
            stage->field_0x1c = 1.0f;
            break;
        case 1:
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c) {
                stage->field_0x14 = 2;
                stage->field_0x18 = 0.0f;
                stage->field_0x1c = 3.0f;
                nextsoundpercent = 5.0f;
                packet->true_hero_percent = 0.0f;
            }
            break;
        case 2: {
            SetDrawGoldBrick(packet, packet->current_gold_brick);
            f32 percent = 100.0f;
            if ((packet->field_0xb0 & 4) == 0) {
                percent = packet->collected_score * 100.0f / packet->true_hero_target;
                if (percent > 99.0f)
                    percent = 99.0f;
            }
            stage->field_0x18 += elapsed;
            packet->true_hero_percent += (100.0f / stage->field_0x1c) * elapsed;
            if (packet->true_hero_percent > percent)
                packet->true_hero_percent = percent;
            else if (packet->true_hero_percent >= nextsoundpercent) {
                PlaySfx(const_cast<char *>("Status_GoldBarDec"), NULL);
                nextsoundpercent += 10.0f;
            }
            if (stage->field_0x18 > stage->field_0x1c) {
                stage->field_0x18 = 0.0f;
                stage->field_0x1c = 1.0f;
                if (stage->type == 2) {
                    stage->field_0x14 = 6;
                    PlaySfx(const_cast<char *>("TrueJedi_NOT"), NULL);
                } else {
                    stage->field_0x14 = 3;
                    PlaySfx(const_cast<char *>("TrueJedi_100pc"), NULL);
                }
            }
            break;
        }
        case 3:
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c) {
                stage->field_0x18 = 0.0f;
                stage->field_0x1c = 1.0f;
                stage->field_0x14 = 6;
                spdelay = 0.0f;
            }
            break;
        case 6:
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c)
                NextStatusStage(packet);
            break;
    }
}

void CollectCharactersOff_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active) {
    COLLECTION_s *collection = packet->vehicle_area ? &VehicleCollection : &CharacterCollection;
    if (active == 0)
        return;
    f32 alpha = 1.0f;
    if (stage->field_0x14 > 0)
        alpha = 1.0f - NuFdiv(stage->field_0x18, stage->field_0x1c);
    Collection_Draw(collection, 0.0f, COLLECTION_Y_STATUS, collection->field_10, NULL, alpha, 0);
}

void CollectCharcters_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active) {
    COLLECTION_s *collection = packet->vehicle_area ? &VehicleCollection : &CharacterCollection;
    if (active == 0)
        return;
    if (stage->field_0x14 == 0) {
        const f32 offscreen = (1.0f - fabsf(COLLECTION_Y_STATUS)) + 1.0f;
        const f32 range = COLLECTION_Y_STATUS + offscreen;
        const f32 ratio = NuFdiv(stage->field_0x18, stage->field_0x1c);
        Collection_Draw(collection, 0.0f,
                        NuTrigTable[(static_cast<i32>(ratio * 16384.0f) >> 1) & 0x7fff] * range - offscreen,
                        collection->field_10, NULL, 1.0f, 0);
        return;
    }
    const i32 id = StatusCollectList.ids[stage->field_0x14 - 1];
    if (id == -1)
        return;
    Collection_Draw(collection, 0.0f, COLLECTION_Y_STATUS, collection->field_10, NULL, 1.0f, 0);
    f32 time = stage->field_0x18;
    f32 x = 0.0f, y = 0.7f, size = 0.4f, icon_alpha = 1.0f, text_alpha;
    if (time < 0.5f) {
        if (time > 0.25f && playedappearsfx == 0) {
            PlaySfx(const_cast<char *>("Char_Icon_App"), NULL);
            playedappearsfx = 1;
            time = stage->field_0x18;
        }
        text_alpha = NuTrigTable[(static_cast<i32>((time + time) * 16384.0f) >> 1) & 0x7fff];
        icon_alpha = time + time;
    } else if (time < 2.0f) {
        text_alpha = 1.0f;
        playedappearsfx = 0;
        playedmovesfx = 0;
    } else {
        text_alpha = 1.0f - NuTrigTable[(static_cast<i32>((time - 2.0f) * 16384.0f) >> 1) & 0x7fff];
        if (playedmovesfx == 0) {
            PlaySfx(const_cast<char *>("Char_Icon_Slide"), NULL);
            playedmovesfx = 1;
        }
        const f32 blend =
            1.0f - (NuTrigTable[(static_cast<i32>((time - 2.0f) * 32768.0f + 16384.0f) >> 1) & 0x7fff] + 1.0f) * 0.5f;
        if (InCollectList_Index(id, collection->list, collection->count_y) != -1) {
            f32 target_x, target_y;
            Collection_GetPos(collection, id, &target_x, &target_y);
            f32 target_size = collection->field_10 * ICONSIZE;
            if (Game_OptionsSave != NULL && Game_OptionsSave->field11_0xb != 0)
                target_size *= 0.875f;
            x = target_x * blend + 0.0f;
            y = (target_y - 0.7f) * blend + 0.7f;
            size = (target_size - 0.4f) * blend + 0.4f;
        }
    }
    DrawCharIcon(id, x, y, 0.0f, size, 0xa7, icon_alpha, icon_alpha, 1, NULL);
    SmartTextEx(TTab[CDataList[id].name_id], 0.0f, 0.35f, 1.0f, 0.6f, 0.6f, 0.6f, 0, STATUS_R, STATUS_G, STATUS_B, 1.7f,
                1, NULL, 0, static_cast<i32>(text_alpha * 128.0f));
}

void CollectCharcters_Skip(STATUS_STAGE_s *stage, STATUSPACKET_s *packet) {
    if (stage->field_0x14 == 0)
        stage->field_0x14 = 1;
    while (StatusCollectList.ids[stage->field_0x14 - 1] != -1) {
        AddToCollection(StatusCollectList.ids[stage->field_0x14 - 1]);
        ++stage->field_0x14;
    }
    NextStatusStage(packet);
    NextStatusStage(packet);
}

void GoldBrick_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    const i32 count = Game.field_0x7c26[0] - packet->previous_gold_bricks;
    const i32 last = count - 1;
    switch (stage->field_0x14) {
        case 0:
            stage->field_0x14 = 1;
            stage->field_0x18 = 0.0f;
            stage->field_0x1c = last * 2.5f + 2.0f + 3.0f + 1.0f + 1.0f;
            break;
        case 1: {
            const f32 previous = stage->field_0x18;
            stage->field_0x18 += elapsed;
            if (__builtin_expect(stage->field_0x18 >= stage->field_0x1c, 0))
                NextStatusStage(packet);
            else
                for (i32 i = 0; i <= last; ++i) {
                    const f32 at = i * 2.5f + 2.0f;
                    if (previous < at && stage->field_0x18 >= at) {
                        PlaySfx(const_cast<char *>("TrueJedi_100pc"), NULL);
                        NewStatusRumbleBuzz(-1, 0.6f, 0.0f, 0);
                        const i32 points = packet->previous_gold_bricks + i + 1;
                        char text[252];
                        sprintf(text, "%i/%i", points, GOLDBRICKPOINTS);
                        AddFancyMessage(text, 0.0f, 0.3f, 0.75f, 1.25f, 1, 1);
                        nuvec_s position = {0.0f, 0.0375f, 1.0f};
                        const i16 id = reinterpret_cast<i16 *>(packet->field_0x12c)[i];
                        char *label;
                        if (id == tTRUEJEDI) {
                            NuStrCpy(text, TTab[id]);
                            if (BOTHTRUEJEDIGOLDBRICKS != 0) {
                                NuStrCat(text, " - ");
                                NuStrCat(text, TTab[(packet->field_0xb0 & 0x40) != 0 ? tFREEPLAY : tSTORY]);
                            }
                            label = text;
                        } else
                            label = TTab[id];
                        AddGameMessage(label, &position, 0.7f, &position, 0.7f, 255, 0, 127, 0x4020, 2.0f);
                        if (points > 60) {
                            IncreaseScore(packet->score, 100000, 0);
                            CoinTotalScale = 1.5f;
                            PlaySfx(const_cast<char *>("Shop_BuyCheat"), NULL);
                            Text_MakeScore(100000, text);
                            AddFancyMessage(text, 0.0f, -STATSPOSY, 0.5f, 1.25f, 0, 0);
                        }
                    }
                }
            break;
        }
    }
}

void MiniKit_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    switch (stage->field_0x14) {
        case 0:
            newminikitcount = packet->new_minikits;
            if (newminikitcount > 0)
                currentminikit = Game.area_save[packet->area->index].field_0x5[0] - newminikitcount;
            else
                currentminikit = Game.area_save[packet->area->index].field_0x5[0];
            stage->field_0x18 = 0.0f;
            stage->field_0x1c = 1.0f;
            stage->field_0x14 = 1;
            break;
        case 1:
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c) {
                stage->field_0x18 = 0.0f;
                if (newminikitcount == 0) {
                    PlaySfx(const_cast<char *>("TrueJedi_NOT"), NULL);
                    stage->field_0x1c = 1.0f;
                    stage->field_0x14 = 4;
                } else {
                    stage->field_0x14 = 2;
                    stage->field_0x1c = 0.5f;
                    slideseek = 1.0f;
                }
            }
            break;
        case 2:
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c) {
                stage->field_0x18 = 0.0f;
                stage->field_0x1c = 0.4f;
                stage->field_0x14 = 3;
                PlaySfx(const_cast<char *>("Jp_Ana_Jump"), NULL);
            }
            break;
        case 3:
            if (newminikitcount < 1) {
                stage->field_0x18 = 0.0f;
                stage->field_0x1c = 3.0f;
                stage->field_0x14 = 4;
            } else {
                const f32 previous = stage->field_0x18;
                stage->field_0x18 += elapsed;
                if (previous < stage->field_0x1c - jibberlen && stage->field_0x18 >= stage->field_0x1c - jibberlen) {
                    NewStatusRumbleBuzz(-1, 0.0f, 0.1f, 0);
                    PlaySfx(const_cast<char *>("MK-Panel"), NULL);
                }
                if (UpdateNewMiniKits(packet, stage) == 1) {
                    stage->field_0x18 = 0.0f;
                    if (currentminikit < packet->minikit_count) {
                        stage->field_0x1c = 0.5f;
                        stage->field_0x14 = 2;
                        return;
                    }
                    stage->field_0x1c = 1.0f;
                    stage->field_0x14 = 4;
                } else if (stage->field_0x14 != 4)
                    return;
            }
            if (__builtin_expect((packet->field_0xb0 & 0x10) == 0, 0))
                PlaySfx(const_cast<char *>("TrueJedi_NOT"), NULL);
            else {
                stage->field_0x14 = 6;
                stage->field_0x1c = 1.0f;
            }
            break;
        case 4:
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c)
                NextStatusStage(packet);
            break;
        case 6: {
            SetDrawGoldBrick(packet, packet->current_gold_brick);
            const f32 previous = stage->field_0x18;
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c) {
                if (previous < stage->field_0x1c) {
                    AddStatusMiniKitParts();
                    NewStatusRumbleBuzz(-1, 1.0f, 0.1f, 0);
                    PlaySfx(const_cast<char *>("Explode1"), NULL);
                } else if (FindGameMsgsWithID(1, 0, -1, NULL) == 0) {
                    PlaySfx(const_cast<char *>("Shop_BuyCheat"), NULL);
                    NextStatusStage(packet);
                }
            } else {
                if (static_cast<u32>(GameTimer.update_count) % 6 < 3 && (GameTimer.update_count - 1U) % 6 > 2)
                    NewStatusRumbleBuzz(-1, 0.0f, 0.0f, 2);
            }
            break;
        }
    }
}

void Fade_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    switch (stage->field_0x14) {
        case 0:
            stage->field_0x18 = 0.0f;
            stage->field_0x1c = 0.6f;
            stage->field_0x14 = 1;
            break;
        case 1: {
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c) {
                FinishStatusPacket(packet->prompt_choice);
            }
            icon_y = -StatusIconsOnOff(NuFdiv(stage->field_0x18, stage->field_0x1c));
            break;
        }
    }
}

void Exit_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    if (stage->field_0x14 >= 0) {
        stage->field_0x18 += elapsed;
    }
    if (stage->field_0x18 >= stage->field_0x1c) {
        NextStatusStage(packet);
        if (packet->current_gold_brick < packet->stage_count - 1) {
            stage->field_0x1c = 0.1f;
        } else {
            FinishStatusPacket(packet->prompt_choice);
        }
    }
}

void CollectCharactersOff_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    switch (stage->field_0x14) {
        case 0:
            stage->field_0x18 = 0.0f;
            stage->field_0x1c = 1.0f;
            stage->field_0x14 = 1;
            break;
        case 1:
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c)
                NextStatusStage(packet);
            break;
    }
}

void Save_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    if (packet->save_state == 3) {
        stage->field_0x18 += elapsed;
        if (stage->field_0x18 >= stage->field_0x1c) {
            if ((packet->mode_flags & 4) != 0 || packet->prompt_choice != 0 || (packet->field_0xb0 & 0x40) != 0 ||
                packet->challenge_state != 0 || packet->mission_state != 0) {
                NextStatusStage(packet);
            } else {
                packet->mode_flags |= 0x10;
                NextStatusStage(packet);
            }
        }
    } else if (packet->save_state == 2) {
        if (MenuInMemoryCard() == 0)
            packet->save_state = 3;
    } else if (packet->save_state == 0) {
        packet->save_state = 1;
        Text_FillInExtendedSaveInfo();
        if (TriggerAutoSave() == 0) {
            packet->save_state = 2;
            NewMenu(1000, -1, -1);
        }
    } else if (memcard_autosaveinprogress == 0) {
        packet->save_state = 3;
    }
}

void BonusComplete_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 current) {
    if (current == 0)
        return;
    if (stage->field_0x14 > 0 && packet->newly_completed != 0) {
        const f32 time = stage->field_0x18;
        f32 alpha;
        if (time < 0.5f)
            alpha = time + time;
        else if (!(time < 3.5f))
            alpha = 1.0f - ((time - 3.5f) + (time - 3.5f));
        else
            alpha = 1.0f;
        if (alpha > 0.0f) {
            SmartTextEx(TTab[tLEVELCOMPLETE], 0.0f, 0.1f, 1.0f, 0.7f, 0.7f, 0.7f, 0, STATUS_R, STATUS_G, STATUS_B, 1.7f,
                        1, NULL, 0, static_cast<i32>(alpha * 128.0f));
        }
    }
    if (stage->field_0x14 == 0) {
        iconalphaoverride = 0.0f;
    } else if (packet->newly_completed == 0) {
        iconalphaoverride = stage->field_0x18 / stage->field_0x1c;
    } else if (stage->field_0x18 < stage->field_0x1c - 0.5f) {
        iconalphaoverride = 0.0f;
    } else {
        const f32 time = stage->field_0x18 - (stage->field_0x1c - 0.5f);
        iconalphaoverride = time + time;
    }
}

void BonusWin_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    switch (stage->field_0x14) {
        case 0:
            stage->field_0x14 = 1;
            stage->field_0x18 = 0.0f;
            stage->field_0x1c = 4.0f;
            draw_player_icons = 1;
            break;
        case 1: {
            const f32 previous = stage->field_0x18;
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c)
                NextStatusStage(packet);
            else if (previous < stage->field_0x1c * 0.5f && stage->field_0x18 >= stage->field_0x1c * 0.5f) {
                PlaySfx(const_cast<char *>("TrueJedi_100pc"), NULL);
                NewStatusRumbleBuzz(-1, 0.6f, 0.0f, 0);
                ++BonusScore[(packet->field_0xb0 & 0x20) != 0];
            }
            break;
        }
    }
}

void RedBrick_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    switch (stage->field_0x14) {
        case 0:
            stage->field_0x18 = 0.0f;
            stage->field_0x1c = 5.0f;
            stage->field_0x14 = 1;
            break;
        case 1: {
            const f32 previous = stage->field_0x18;
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c)
                NextStatusStage(packet);
            else if (previous < 0.75f && stage->field_0x18 >= 0.75f) {
                PlaySfx(const_cast<char *>("TrueJedi_100pc"), NULL);
                NewStatusRumbleBuzz(-1, 0.6f, 0.0f, 0);
            }
            break;
        }
    }
}

void AllMiniKits_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    switch (stage->field_0x14) {
        case 0:
            stage->field_0x18 = 0.0f;
            stage->field_0x1c = 6.5f;
            stage->field_0x14 = 1;
            break;
        case 1: {
            const f32 previous = stage->field_0x18;
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c)
                NextStatusStage(packet);
            else if (previous < 0.5f && stage->field_0x18 >= 0.5f) {
                PlaySfx(const_cast<char *>("StatusAward"), NULL);
                NewStatusRumbleBuzz(-1, 0.6f, 0.0f, 0);
            } else if (previous < 4.0f && stage->field_0x18 >= 4.0f)
                PlaySfx(const_cast<char *>("Char_Icon_App"), NULL);
            break;
        }
    }
}

static void SuperStoryScore_LSW_DrawBody(STATUS_STAGE_s *stage, STATUSPACKET_s *packet) {
    f32 coin_progress = 1.0f;
    if (stage->field_0x14 > 0) {
        char text[256], previous[256];
        const f32 time = stage->field_0x18;
        f32 alpha;
        if (!(time < 0.5f)) {
            alpha = 1.0f;
            if (time >= 4.5f)
                alpha = coin_progress = 1.0f - ((time - 4.5f) + (time - 4.5f));
        } else {
            alpha = time + time;
        }
        const i32 opacity = static_cast<i32>(alpha * 128.0f);
        const bool new_best = packet->new_best_score != 0;
        SmartTextEx(TTab[new_best ? tNEWHIGHSCORE : tNONEWHIGHSCORE], 0.0f, 0.2f, 1.0f, 0.7f, 0.7f, 0.7f, 0,
                    new_best ? 0 : 255, new_best ? 255 : 0, 0, 1.7f, 1, NULL, 0, opacity);
        const f32 draw_time = stage->field_0x18;
        f32 blend = 0.0f;
        if (!(draw_time < 0.5f))
            blend = draw_time < 4.0f ? (draw_time - 0.5f) / 3.5f : 1.0f;
        const u32 score = packet->superstory_score;
        const f32 interpolated_score =
            (static_cast<f32>(score >> 16) * 65536.0f + static_cast<f32>(score & 0xffff)) * blend;
        // The reference's unsigned SSE conversion wraps the f32-rounded 2^32
        // endpoint to zero. Avoid an out-of-range float-to-integer conversion.
        const u32 displayed_score = !(interpolated_score >= 4294967296.0f) ? static_cast<u32>(interpolated_score) : 0;
        Text_MakeScore(displayed_score, text);
        Text3DEx(text, 0.0f, 0.0f, 1.0f, 0.7f, 0.7f, 0.7f, 0, 255, 255, 255, static_cast<u8>(opacity));
        Text_MakeScore(packet->previous_best_score, text);
        NuStrCpy(previous, "(");
        NuStrCat(previous, text);
        NuStrCat(previous, ")");
        Text3DEx(previous, 0.0f, -0.2f, 1.0f, 0.7f, 0.7f, 0.7f, 0, 255, 255, 255, static_cast<u8>(opacity / 2));
    }
    CoinTotal_Draw(*packet->score,
                   (STATSPOSY - STATSPOS2Y) * NuTrigTable[(static_cast<i32>(coin_progress * 16384.0f) >> 1) & 0x7fff] +
                       STATSPOS2Y,
                   CoinTotalScale, 1, 1.0f, 255, 191, 0);
}

void SuperStoryScore_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 current) {
    if (__builtin_expect(current == 0, 0))
        return;
    SuperStoryScore_LSW_DrawBody(stage, packet);
}

void SuperStoryScore_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    switch (stage->field_0x14) {
        case 0:
            stage->field_0x14 = 1;
            stage->field_0x18 = 0.0f;
            stage->field_0x1c = 5.0f;
            packet->original_score = *packet->score;
            break;
        case 1: {
            const f32 previous = stage->field_0x18;
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c) {
                *packet->score = packet->final_reward_score;
                NextStatusStage(packet);
            } else if (previous < 0.5f && stage->field_0x18 >= 0.5f) {
                if (packet->new_best_score != 0)
                    PlaySfx(const_cast<char *>("StatusAward"), NULL);
                else
                    GameAudio_PlaySfx(0x32, NULL, 0, 0);
                NewStatusRumbleBuzz(-1, 0.6f, 0.0f, 0);
            }
            if (packet->final_reward_score != packet->original_score) {
                if (previous < 4.0f && stage->field_0x18 >= 4.0f) {
                    CoinTotalScale = 1.5f;
                    NewStatusRumbleBuzz(-1, 0.0f, 0.1f, 0);
                    PlaySfx(const_cast<char *>("Shop_BuyCheat"), NULL);
                }
                if (stage->field_0x18 >= 4.0f)
                    *packet->score = packet->final_reward_score;
                else if (stage->field_0x18 >= 0.5f && stage->field_0x18 < 4.0f) {
                    const u32 original = packet->original_score;
                    const f32 blend =
                        1.0f -
                        NuTrigTable[(static_cast<i32>(((stage->field_0x18 - 0.5f) / 3.5f) * 16384.0f + 16384.0f) >> 1) &
                                    0x7fff];
                    const f32 difference = static_cast<f32>(packet->final_reward_score - original);
                    *packet->score = original;
                    IncreaseScore(packet->score, static_cast<i64>(static_cast<i32>(difference * blend)), 0);
                    if (static_cast<i32>(previous / 0.2f) != static_cast<i32>(stage->field_0x18 / 0.2f))
                        PlaySfx(const_cast<char *>("PickupCoin"), NULL);
                }
            }
            break;
        }
    }
}

void SuperStoryTime_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    switch (stage->field_0x14) {
        case 0:
            stage->field_0x14 = 1;
            stage->field_0x18 = 0.0f;
            stage->field_0x1c = 5.0f;
            packet->original_score = *packet->score;
            break;
        case 1: {
            const f32 previous = stage->field_0x18;
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c) {
                *packet->score = packet->time_reward_score;
                NextStatusStage(packet);
            } else if (previous < 0.5f && stage->field_0x18 >= 0.5f) {
                if (packet->new_best_time != 0.0f)
                    PlaySfx(const_cast<char *>("StatusAward"), NULL);
                else
                    GameAudio_PlaySfx(0x32, NULL, 0, 0);
                NewStatusRumbleBuzz(-1, 0.6f, 0.0f, 0);
            }
            if (packet->new_best_time != 0.0f) {
                if (previous < 4.0f && stage->field_0x18 >= 4.0f) {
                    CoinTotalScale = 1.5f;
                    NewStatusRumbleBuzz(-1, 0.0f, 0.1f, 0);
                    PlaySfx(const_cast<char *>("Shop_BuyCheat"), NULL);
                }
                if (stage->field_0x18 >= 4.5f)
                    *packet->score = packet->time_reward_score;
                else if (stage->field_0x18 >= 0.5f && stage->field_0x18 < 4.0f) {
                    const i32 increase = static_cast<i32>(
                        NuTrigTable[(static_cast<i32>(((stage->field_0x18 - 0.5f) / 3.5f) * 16384.0f) >> 1) & 0x7fff] *
                        static_cast<f32>(packet->time_reward_score - packet->original_score));
                    *packet->score = packet->original_score;
                    IncreaseScore(packet->score, static_cast<i64>(increase), 0);
                    if (static_cast<i32>(previous / 0.2f) != static_cast<i32>(stage->field_0x18 / 0.2f))
                        PlaySfx(const_cast<char *>("PickupCoin"), NULL);
                }
            }
            break;
        }
    }
}

void Coins_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    switch (stage->field_0x14) {
        case 0:
            stage->field_0x14 = 1;
            stage->field_0x18 = 0.0f;
            stage->field_0x1c = 1.0f;
            draw_player_icons = 1;
            break;
        case 1:
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c) {
                stage->field_0x14 = 2;
            }
            break;
        case 2: {
            i32 finished = 0;
            for (i32 player = 0; player < 2; ++player) {
                u32 remaining = packet->coins_remaining[player];
                if (remaining != 0) {
                    u32 rate = 35;
                    for (u32 threshold = 100; threshold <= remaining; threshold *= 10) {
                        rate *= 10;
                    }
                    u32 amount = static_cast<i32>(static_cast<f32>(rate) * elapsed);
                    if (amount > remaining) {
                        amount = remaining;
                    } else if (amount == 0) {
                        amount = 1;
                    }
                    packet->coins_remaining[player] -= amount;
                    IncreaseScore(packet->score, static_cast<u64>(amount), 0);
                    if (packet->coins_remaining[player] == 0)
                        ++finished;
                } else {
                    ++finished;
                }
            }
            PlaySfx("PickupCoin", 0);
            if (finished == 2) {
                stage->field_0x14 = 3;
                stage->field_0x18 = 0.0f;
                stage->field_0x1c = 1.0f;
                if (packet->coins_collected[0] + packet->coins_collected[1] == 0) {
                    GameAudio_PlaySfx(0x32, NULL, 0, 0);
                } else {
                    PlaySfx("Shop_BuyCheat", 0);
                }
            }
            break;
        }
        case 3:
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c) {
                NextStatusStage(packet);
            }
            break;
    }
}

static void SuperStoryTime_LSW_DrawBody(STATUS_STAGE_s *stage, STATUSPACKET_s *packet) {
    char text[256], previous[256];
    const f32 time = stage->field_0x18;
    f32 alpha, coin_progress;
    if (time >= 0.5f) {
        alpha = coin_progress = 1.0f;
        if (time >= 4.5f)
            alpha = 1.0f - ((time - 4.5f) + (time - 4.5f));
    } else {
        alpha = coin_progress = time + time;
    }
    const i32 opacity = static_cast<i32>(alpha * 128.0f);
    CoinTotal_Draw(*packet->score,
                   (STATSPOSY - STATSPOS2Y) * NuTrigTable[(static_cast<i32>(coin_progress * 16384.0f) >> 1) & 0x7fff] +
                       STATSPOS2Y,
                   CoinTotalScale, 1, 1.0f, 255, 191, 0);
    const bool new_best = packet->new_best_time != 0.0f;
    SmartTextEx(TTab[new_best ? tNEWBESTTIME : tNONEWBESTTIME], 0.0f, 0.2f, 1.0f, 0.7f, 0.7f, 0.7f, 0,
                new_best ? 0 : 255, new_best ? 255 : 0, 0, 1.7f, 1, NULL, 0, opacity);
    f32 displayed_time;
    if (packet->new_best_time == 0.0f) {
        displayed_time = packet->superstory_time;
    } else {
        const f32 draw_time = stage->field_0x18;
        f32 blend = 0.0f;
        if (draw_time < 0.5f)
            blend = 0.0f;
        else
            blend = draw_time < 4.0f ? (draw_time - 0.5f) / 3.5f : 1.0f;
        displayed_time = (packet->new_best_time - packet->previous_best_time) * blend + packet->previous_best_time;
    }
    Text_MakeTime(displayed_time, 1, 1, 1, text);
    Text3DEx(text, 0.0f, 0.0f, 1.0f, 0.7f, 0.7f, 0.7f, 0, 255, 255, 255, static_cast<u8>(opacity));
    Text_MakeTime(packet->previous_best_time, 1, 1, 1, text);
    NuStrCpy(previous, "(");
    NuStrCat(previous, text);
    NuStrCat(previous, ")");
    Text3DEx(previous, 0.0f, -0.2f, 1.0f, 0.7f, 0.7f, 0.7f, 0, 255, 255, 255, static_cast<u8>(opacity / 2));
}

void SuperStoryTime_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 current) {
    if (current == 0 || stage->field_0x14 < 1)
        return;
    SuperStoryTime_LSW_DrawBody(stage, packet);
}

void BonusTime_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    switch (stage->field_0x14) {
        case 0:
            stage->field_0x18 = 0.0f;
            stage->field_0x1c = 4.0f;
            stage->field_0x14 = 1;
            break;
        case 1: {
            SetDrawGoldBrick(packet, packet->current_gold_brick);
            const f32 previous = stage->field_0x18;
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c)
                NextStatusStage(packet);
            else if (previous < 0.5f && stage->field_0x18 >= 0.5f) {
                if (packet->new_best_time != 0.0f)
                    PlaySfx(const_cast<char *>("StatusAward"), NULL);
                else
                    GameAudio_PlaySfx(0x32, NULL, 0, 0);
                NewStatusRumbleBuzz(-1, 0.6f, 0.0f, 0);
            }
            break;
        }
    }
}

void BonusComplete_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    switch (stage->field_0x14) {
        case 0:
            stage->field_0x18 = 0.0f;
            stage->field_0x14 = 1;
            stage->field_0x1c = packet->newly_completed == 0 ? 0.5f : 4.0f;
            draw_player_icons = 1;
            break;
        case 1: {
            SetDrawGoldBrick(packet, packet->current_gold_brick);
            const f32 previous = stage->field_0x18;
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c)
                NextStatusStage(packet);
            else if (packet->newly_completed != 0 && previous < 0.5f && stage->field_0x18 >= 0.5f) {
                PlaySfx(const_cast<char *>("StatusAward"), NULL);
                NewStatusRumbleBuzz(-1, 0.6f, 0.0f, 0);
            }
            break;
        }
    }
}

void LevelComplete_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    switch (stage->field_0x14) {
        case 0:
            stage->field_0x18 = 0.0f;
            stage->field_0x1c = 4.0f;
            stage->field_0x14 = 1;
            return;
        case 1: {
            SetDrawGoldBrick(packet, packet->current_gold_brick);
            const f32 previous = stage->field_0x18;
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c) {
                if (stage->type == 26 && packet->mission_state == 2) {
                    *packet->score = packet->reward_score;
                }
                if (stage->type == 23 && packet->challenge_state == 2) {
                    *packet->score = packet->reward_score;
                }
                NextStatusStage(packet);
                return;
            }
            if (previous < 0.5f && stage->field_0x18 >= 0.5f) {
                if ((stage->type == 23 && packet->challenge_state == 3) ||
                    (stage->type == 26 && packet->mission_state == 3)) {
                    GameAudio_PlaySfx(0x32, NULL, 0, 0);
                } else {
                    PlaySfx(const_cast<char *>("StatusAward"), NULL);
                }
                NewStatusRumbleBuzz(-1, 0.6f, 0.0f, 0);
                return;
            }
            if ((stage->type != 26 || packet->mission_state != 2) &&
                (stage->type != 23 || packet->challenge_state != 2)) {
                return;
            }
            const f32 reward_end = stage->field_0x1c - 0.75f;
            if (previous < reward_end && stage->field_0x18 >= reward_end) {
                CoinTotalScale = 1.5f;
                NewStatusRumbleBuzz(-1, 0.0f, 0.1f, 0);
                PlaySfx(const_cast<char *>("Shop_BuyCheat"), NULL);
            } else if (stage->field_0x18 >= 0.75f && stage->field_0x18 < reward_end) {
                PlaySfx(const_cast<char *>("PickupCoin"), NULL);
            }
            return;
        }
        default:
            return;
    }
}

void CollectCharcters_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    if (stage->field_0x14 == 0) {
        stage->field_0x1c = 1.0f;
        stage->field_0x18 += elapsed;
        if (stage->field_0x18 >= 1.0f) {
            stage->field_0x14 = 1;
            if (StatusCollectList.ids[0] == -1) {
                GameAudio_PlaySfx(0x32, NULL, 0, 0);
                NewStatusRumbleBuzz(-1, 0.6f, 0.0f, 0);
                NextStatusStage(packet);
            } else {
                stage->field_0x18 = 0.0f;
                stage->field_0x1c = 3.0f;
            }
        }
    } else {
        stage->field_0x18 += elapsed;
        if (stage->field_0x18 >= stage->field_0x1c) {
            AddToCollection(StatusCollectList.ids[stage->field_0x14 - 1]);
            PlaySfx(const_cast<char *>("LegoClicks"), NULL);
            ++stage->field_0x14;
            stage->field_0x18 = 0.0f;
            stage->field_0x1c = 3.0f;
            if (StatusCollectList.ids[stage->field_0x14 - 1] == -1)
                NextStatusStage(packet);
        }
    }
}

void Prompt_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active) {
    if (active != 0) {
        if (stage->field_0x18 < 0.5f) {
            f32 alpha =
                NuTrigTable[(static_cast<i32>((stage->field_0x18 + stage->field_0x18) * 16384.0f) >> 1) & 0x7fff];
            if (stage->field_0x14 == 1) {
                alpha = 1.0f - alpha;
            }
            Status_DrawPromptMenu(packet, 0, alpha);
            draw_player_icons = 1;
        } else {
            Status_DrawPromptMenu(packet, 1, 1.0f);
            DrawPlayerIconPrompts(packet->player_active[0], tSELECT, 1.0f, -1, -1, -1, tSELECTING, packet->player_active[1],
                                  tSELECT, 1.0f, -1, -1, -1, tSELECTING);
        }
    }
}

void Prompt_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    switch (stage->field_0x14) {
        case 0: {
            if (stage->field_0x18 == 0.0f) {
                NewMenu(0x400, 0, -1);
            }
            stage->field_0x18 += elapsed;
            const i32 menu_level = GameMenuLevel;
            if (stage->field_0x18 >= 0.5f) {
                MENU *menu = &GameMenu[menu_level];
                i32 player = 0;
                auto *pad = &GamePad[0];
                i32 selected;
                i32 moved_down;
                i32 moved_up;
                do {
                    selected = 0;
                    moved_down = 0;
                    moved_up = 0;
                    if (__builtin_expect(packet->player_active[player] != 0, 0)) {
                        if (GameMenuLevel > 0 && menu->queued_item != -1) {
                            status_prompt = menu->queued_item;
                            menu->queued_item = -1;
                            selected = 1;
                        }
                        const u32 buttons = pad->buttons_pressed;
                        if ((buttons & GAMEPAD_MENUSELECT) != 0) {
                            selected = 1;
                        } else if (((packet->field_0xb0 & 0x48) == 8 &&
                                    (packet->next_area == -1 || (packet->area->flags & 2) != 0 ||
                                     Game.area_save[packet->next_area].complete != 0)) ||
                                   (packet->mode_flags & 1) != 0 || packet->challenge_state != 0 ||
                                   packet->mission_state != 0) {
                            if (status_prompt < 1) {
                                if (((buttons | pad->left_directions) & GAMEPAD_DDOWN) != 0) {
                                    GameAudio_PlaySfx(0x2f, NULL, 0, 0);
                                    ++status_prompt;
                                    moved_down = 1;
                                }
                            } else if (__builtin_expect(((buttons | pad->left_directions) & GAMEPAD_DUP) != 0, 1)) {
                                GameAudio_PlaySfx(0x2f, NULL, 0, 0);
                                --status_prompt;
                                moved_up = 1;
                            }
                        }
                    }
                    ++player;
                    ++pad;
                    if (__builtin_expect(selected != 0, 1)) {
                        break;
                    }
                } while (player < 2 && (moved_down | moved_up) == 0);
                if (__builtin_expect(selected != 0, 0)) {
                    packet->prompt_choice = (packet->field_0xb0 & 8) != 0 && packet->next_area != -1 &&
                                                    (packet->area->flags & 2) == 0 &&
                                                    Game.area_save[packet->next_area].complete == 0
                                                ? 1
                                                : static_cast<u8>(status_prompt);
                    GameAudio_PlaySfx(0x30, NULL, 0, 0);
                    stage->field_0x14 = 1;
                    stage->field_0x18 = 0.0f;
                    stage->field_0x1c = 0.5f;
                }
            }
            break;
        }
        case 1: {
            if (__builtin_expect(stage->field_0x18 == 0.0f, 1)) {
                BackupMenu();
            }
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 > stage->field_0x1c) {
                NextStatusStage(packet);
            }
            break;
        }
    }
}

void GoldBrick_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active) {
    if (active == 0)
        return;
    if (stage->field_0x14 > 0) {
        const f32 time = stage->field_0x18;
        if (time >= 1.0f && time < stage->field_0x1c - 1.0f) {
            f32 blend = 1.0f;
            i32 count = Game.field_0x7c26[0];
            const i32 previous = packet->previous_gold_bricks;
            if (time < 2.0f) {
                blend = NuTrigTable[(static_cast<i32>((time - 1.0f) * 16384.0f) >> 1) & 0x7fff];
                count = previous;
            } else {
                const f32 end = stage->field_0x1c - 2.0f;
                if (!(time < end))
                    blend = NuTrigTable[(static_cast<i32>((1.0f - (time - end)) * 16384.0f) >> 1) & 0x7fff];
                if (!(time < 2.0f) && time < (count - previous - 1) * 2.5f + 2.0f)
                    count = (previous + 1) + (time - 2.0f) / 2.5f;
            }
            char text[256];
            sprintf(text, "%i/%i", count, GOLDBRICKPOINTS);
            SmartTextEx(text, 0.0f, blend * -0.99999994f + 1.3f, 1.0f, 1.5f, 1.5f, 1.5f, 0, 255, 255, 255, 1.7f, 1,
                        NULL, 0, static_cast<i32>(blend * 128.0f));
            const u16 angle = NuFmod(GlobalTimer.time_elapsed, 4.0f) * 0.25f * 65536.0f;
            DrawPanel3DObjectNoAlpha(0.0f, blend * 0.99999994f - 1.3f, 1.0f, 1.0f, 1.0f, 1.0f,
                                     static_cast<i32>(NuTrigTable[angle & 0x7fff] * 1820.0f), angle, 0,
                                     reinterpret_cast<nuhspecial_s *>(&WORLD->lev_objs[0xd3]), 2);
            if (Game.field_0x7c26[0] > 60)
                CoinTotal_Draw(*packet->score, (STATSPOSY - STATSPOS2Y) * blend + STATSPOS2Y, CoinTotalScale, 1, 1.0f,
                               255, 191, 0);
        }
    }
    if ((stage->field_0x14 == 0 || stage->field_0x18 < 1.0f) && packet->mission_state != 0 &&
        Game.field_0x7c26[0] > packet->previous_gold_bricks)
        iconalphaoverride = 0.0f;
}

void RedBrick_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active) {
    if (active == 0 || stage->field_0x14 <= 0)
        return;
    f32 blend;
    if (stage->field_0x18 < 1.0f)
        blend = NuTrigTable[(static_cast<i32>(stage->field_0x18 * 16384.0f) >> 1) & 0x7fff];
    else if (stage->field_0x18 < stage->field_0x1c - 1.0f)
        blend = 1.0f;
    else
        blend =
            NuTrigTable[(static_cast<i32>((1.0f - (stage->field_0x18 - (stage->field_0x1c - 1.0f))) * 16384.0f) >> 1) &
                        0x7fff];
    SmartTextEx(TTab[tUNLOCKED], 0.0f, 0.05f, 1.0f, 0.7f, 0.7f, 0.7f, 0, STATUS_R, STATUS_G, STATUS_B, 1.7f, 1, NULL, 0,
                static_cast<i32>(blend * 128.0f));
    const i32 angle = static_cast<i32>(NuFmod(GlobalTimer.time_elapsed, 4.0f) * 0.25f * 65536.0f);
    DrawPanel3DObjectNoAlpha(0.0f, blend - 1.5f, 1.0f, 1.0f, 1.0f, 1.0f,
                             static_cast<i32>(NuTrigTable[angle & 0x7fff] * 1820.0f), angle, 0,
                             reinterpret_cast<nuhspecial_s *>(&WORLD->lev_objs[0xd2]), 2);
    SmartTextEx(TTab[*Cheat[static_cast<i8>(packet->area->cheat)].text_id], 0.0f, 1.5f - blend, 1.0f, 0.7f, 0.7f, 0.7f,
                0, 255, 255, 255, 1.7f, 1, NULL, 0, 128);
}

void TrueHero_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active) {
    if (active == 0)
        return;
    char text[256];
    f32 alpha = 1.0f;
    switch (stage->field_0x14) {
        case 0:
            alpha = 0.0f;
            break;
        case 1: {
            alpha = stage->field_0x18;
            const f32 ratio = NuFdiv(stage->field_0x18, stage->field_0x1c);
            const f32 blend =
                1.0f - (NuTrigTable[(static_cast<i32>(ratio * 32768.0f + 16384.0f) >> 1) & 0x7fff] + 1.0f) * 0.5f;
            DrawBuildUpBar(0.0f,
                           (0.7f - STATSPOS2Y) * NuTrigTable[(static_cast<i32>(blend * 16384.0f) >> 1) & 0x7fff] +
                               STATSPOS2Y,
                           0, 100, 1.0f, blend * 0.75f + 1.0f, 1.0f, 0);
            break;
        }
        case 2: {
            f32 time = stage->field_0x18;
            if (time > 1.0f)
                time = 1.0f;
            const f32 blend = (NuTrigTable[(static_cast<i32>(time * 32768.0f + 16384.0f) >> 1) & 0x7fff] + 1.0f) * 0.5f;
            DrawBuildUpBar(0.0f, 0.7f, packet->true_hero_percent, 100, 1.0f, 1.75f, 1.0f, 0);
            sprintf(text, "%i%%", static_cast<i32>(packet->true_hero_percent));
            Text3DEx(text, 0.0f, 0.3f, 1.0f, 0.8f, 0.8f, 0.8f, 0, 255, 191, 0,
                     static_cast<u8>((1.0f - blend) * 128.0f));
            break;
        }
        case 3: {
            const f32 blend =
                1.0f - (NuTrigTable[stage->field_0x18 < 1.0f
                                        ? ((static_cast<i32>(stage->field_0x18 * 32768.0f + 16384.0f) >> 1) & 0x7fff)
                                        : 0x6000] +
                        1.0f) *
                           0.5f;
            if (blend < 1.0f) {
                const i32 opacity = static_cast<i32>((1.0f - blend) * 64.0f) & 255;
                f32 scale = (blend * 0.5f + 1.0f) * 0.8f;
                Text3DEx(const_cast<char *>("100%"), 0.0f, 0.3f, 1.0f, scale, scale, scale, 0, 255, 191, 0, opacity);
                scale = (1.0f - blend * 0.5f) * 0.8f;
                Text3DEx(const_cast<char *>("100%"), 0.0f, 0.3f, 1.0f, scale, scale, scale, 0, 255, 191, 0, opacity);
            }
            DrawBuildUpBar(0.0f, 0.7f, packet->true_hero_percent, 100, 1.0f, 1.75f, 1.0f, 0);
            break;
        }
        case 6: {
            alpha = 1.0f - stage->field_0x18;
            if (stage->type == 2 && alpha > 0.0f) {
                sprintf(text, "%i%%", static_cast<i32>(packet->true_hero_percent));
                Text3DEx(text, 0.0f, 0.3f, 1.0f, 0.8f, 0.8f, 0.8f, 0, 255, 191, 0,
                         static_cast<u8>(alpha * 128.0f));
            }
            const f32 ratio = stage->field_0x18 < 1.0f ? 1.0f - stage->field_0x18 : 0.0f;
            const f32 blend =
                1.0f - (NuTrigTable[(static_cast<i32>(ratio * 32768.0f + 16384.0f) >> 1) & 0x7fff] + 1.0f) * 0.5f;
            DrawBuildUpBar(0.0f,
                           (0.7f - STATSPOS2Y) * NuTrigTable[(static_cast<i32>(blend * 16384.0f) >> 1) & 0x7fff] +
                               STATSPOS2Y,
                           packet->true_hero_percent, 100, 1.0f, blend * 0.75f + 1.0f, 1.0f, 0);
            break;
        }
    }
    alpha = alpha < 0.0f ? 0.0f : alpha > 1.0f ? 1.0f : alpha;
    NuStrCpy(text, TTab[tTRUEJEDI]);
    if (BOTHTRUEJEDIGOLDBRICKS != 0) {
        NuStrCat(text, " - ");
        NuStrCat(text, TTab[(packet->field_0xb0 & 0x40) != 0 ? tFREEPLAY : tSTORY]);
    }
    Text3DEx(text, 0.0f, STATUS_TITLE_Y, 1.0f, 0.5f, 0.5f, 0.5f, 0, 255, 255, 255,
             static_cast<i32>(alpha * 128.0f) & 255);
}

static void LevelComplete_LSW_DrawActive(STATUS_STAGE_s *stage, STATUSPACKET_s *packet) {
    char text[256];
    if (stage->field_0x14 > 0) {
        f32 alpha = 1.0f;
        if (stage->field_0x18 < 0.5f)
            alpha = stage->field_0x18 + stage->field_0x18;
        else if (!(stage->field_0x18 < 3.5f))
            alpha = 1.0f - ((stage->field_0x18 - 3.5f) + (stage->field_0x18 - 3.5f));
        if (alpha > 0.0f) {
            const i32 opacity = alpha * 128.0f;
            if (stage->type == 14) {
                strcpy(text, TTab[EDataList[packet->episode_id].text_id]);
                SmartTextEx(text, 0.0f, 0.1f, 1.0f, 0.525f, 0.525f, 0.525f, 0, STATUS_R, STATUS_G, STATUS_B, 1.7f,
                            1, NULL, 0, opacity);
                SmartTextEx(TTab[tSUPERSTORYCOMPLETE], 0.0f, -0.1f, 1.0f, 0.7f, 0.7f, 0.7f, 0, STATUS_R, STATUS_G,
                            STATUS_B, 1.7f, 1, NULL, 0, opacity);
            } else if (stage->type == 13) {
                if ((PODRACE_ADATA != NULL && packet->area == PODRACE_ADATA) ||
                    (BONUS_GUNSHIP_ADATA != NULL && packet->area == BONUS_GUNSHIP_ADATA)) {
                    SmartTextEx(TTab[tLEVELCOMPLETE], 0.0f, 0.0f, 1.0f, 0.7f, 0.7f, 0.7f, 0, STATUS_R, STATUS_G,
                                STATUS_B, 1.7f, 1, NULL, 0, opacity);
                } else if (FreePlayUnlocked()) {
                    SmartTextEx(TTab[tLEVELCOMPLETE], 0.0f, 0.1f, 1.0f, 0.7f, 0.7f, 0.7f, 0, STATUS_R, STATUS_G,
                                STATUS_B, 1.7f, 1, NULL, 0, opacity);
                    SmartTextEx(TTab[tFREEPLAYUNLOCKED], 0.0f, -0.1f, 1.0f, 0.7f, 0.7f, 0.7f, 0, STATUS_R, STATUS_G,
                                STATUS_B, 1.7f, 1, NULL, 0, opacity);
                } else {
                    SmartTextEx(TTab[tLEVELCOMPLETE], 0.0f, 0.0f, 1.0f, 0.7f, 0.7f, 0.7f, 0, STATUS_R, STATUS_G,
                                STATUS_B, 1.7f, 1, NULL, 0, opacity);
                }
            } else if (stage->type == 8) {
                SmartTextEx(TTab[tEPISODEVUNLOCKED], 0.0f, 0.1f, 1.0f, 0.7f, 0.7f, 0.7f, 0, STATUS_R, STATUS_G,
                            STATUS_B, 1.7f, 1, NULL, 0, opacity);
                SmartTextEx(TTab[tEPISODEVIUNLOCKED], 0.0f, -0.1f, 1.0f, 0.7f, 0.7f, 0.7f, 0, STATUS_R, STATUS_G,
                            STATUS_B, 1.7f, 1, NULL, 0, opacity);
            } else if (stage->type == 28) {
                SmartTextEx(TTab[tBONUSUNLOCKED], 0.0f, 0.0f, 1.0f, 0.7f, 0.7f, 0.7f, 0, STATUS_R, STATUS_G,
                            STATUS_B, 1.7f, 1, NULL, 0, opacity);
            } else if (stage->type == 23) {
                bool complete = packet->challenge_state == 2;
                SmartTextEx(TTab[complete                         ? tCHALLENGECOMPLETE
                                 : packet->challenge_state == 3 ? tOUTOFTIME
                                                                : tYOUDIDNTCOLLECTALL10CANISTERS],
                            0.0f, 0.0f, 1.0f, 0.7f, 0.7f, 0.7f, 0, complete ? 255 : STATUS_R, complete ? 255 : STATUS_G,
                            complete ? 255 : STATUS_B, 1.7f, 1, NULL, 0, opacity);
            } else if (stage->type == 26) {
                bool complete = packet->mission_state == 2;
                SmartTextEx(TTab[complete ? tMISSIONCOMPLETE : tOUTOFTIME], 0.0f, 0.0f, 1.0f, 0.7f, 0.7f, 0.7f, 0,
                            complete ? 255 : STATUS_R, complete ? 255 : STATUS_G, complete ? 255 : STATUS_B, 1.7f, 1,
                            NULL, 0, opacity);
            } else {
                const i16 *label;
                if (stage->type == 29)
                    label = &tEPISODEICOMPLETE;
                else if (stage->type == 30)
                    label = &tEPISODEIICOMPLETE;
                else if (stage->type == 31)
                    label = &tEPISODEIIICOMPLETE;
                else if (stage->type == 32)
                    label = &tEPISODEIVCOMPLETE;
                else if (stage->type == 33)
                    label = &tEPISODEVCOMPLETE;
                else
                    label = &tEPISODEVICOMPLETE;
                SmartTextEx(TTab[*label], 0.0f, 0.1f, 1.0f, 0.7f, 0.7f, 0.7f, 0, STATUS_R, STATUS_G, STATUS_B, 1.7f,
                            1, NULL, 0, opacity);
                SmartTextEx(TTab[tSTORYCLIPSUNLOCKED], 0.0f, -0.1f, 1.0f, 0.7f, 0.7f, 0.7f, 0, STATUS_R, STATUS_G,
                            STATUS_B, 1.7f, 1, NULL, 0, opacity);
            }
        }
    }
    if (stage->type != 23 && stage->type != 26)
        return;
    if (stage->field_0x14 == 0 || stage->field_0x18 < stage->field_0x1c - 0.5f)
        iconalphaoverride = 0.0f;
    else
        iconalphaoverride = (stage->field_0x18 - (stage->field_0x1c - 0.5f)) * 2.0f;
    if (stage->type == 26) {
        if (packet->mission_state != 2)
            return;
        if (packet->previous_gold_bricks < Game.field_0x7c26[0])
            iconalphaoverride = 0.0f;
    } else if (stage->type != 23 || packet->challenge_state != 2) {
        return;
    }
    f32 progress, slide;
    if (stage->field_0x18 < 0.75f) {
        progress = 0.0f;
        slide = stage->field_0x18 / 0.7f;
    } else if (!(stage->field_0x18 < stage->field_0x1c - 0.75f)) {
        progress = 1.0f;
        slide = 1.0f - (stage->field_0x18 - (stage->field_0x1c - 0.75f)) / 0.75f;
    } else {
        progress = (stage->field_0x18 - 0.75f) / (stage->field_0x1c - 0.75f - 0.75f);
        slide = 1.0f;
    }
    f32 coin_slide = slide;
    if (!(stage->field_0x18 < 0.75f))
        coin_slide = 1.0f;
    u32 score = packet->original_score;
    IncreaseScore(&score,
                  static_cast<i64>(static_cast<i32>(static_cast<f32>(packet->reward_score - score) * progress)), 0);
    CoinTotal_Draw(score,
                   (STATSPOSY - STATSPOS2Y) * NuTrigTable[(static_cast<i32>(coin_slide * 16384.0f) >> 1) & 0x7fff] +
                       STATSPOS2Y,
                   CoinTotalScale, 1, 1.0f, 255, 191, 0);
    if (stage->type == 26) {
        const f32 remaining = MAX(0.0f, static_cast<u16>(packet->mission->time) - packet->elapsed_time);
        Text_MakeTime((1.0f - progress) * remaining, 0, 1, 1, text);
        Text3D(text, 0.0f,
               -((STATSPOSY - STATSPOS2Y) * NuTrigTable[(static_cast<i32>(slide * 16384.0f) >> 1) & 0x7fff] +
                 STATSPOS2Y),
               1.0f, 0.6f, 0.6f, 0.6f, 0, 255, 191, 0);
    }
    if (stage->type == 23) {
        const f32 remaining = MAX(0.0f, packet->area->challenge_trial_time - packet->elapsed_time);
        Text_MakeTime((1.0f - progress) * remaining, 0, 1, 1, text);
        Text3D(text, 0.0f,
               -((STATSPOSY - STATSPOS2Y) * NuTrigTable[(static_cast<i32>(slide * 16384.0f) >> 1) & 0x7fff] +
                 STATSPOS2Y),
               1.0f, 0.6f, 0.6f, 0.6f, 0, 255, 191, 0);
    }
}

f32 getFinishedStatusAlpha(STATUSPACKET_s *packet) {
    STATUS_STAGE_s *stage = packet->stage;
    if (stage->type == 11 || stage->type == 12) {
        return 0.0f;
    }
    if (stage->type == 10) {
        if (stage->field_0x18 < 1.0f) {
            return 1.0f - stage->field_0x18;
        }
        return 0.0f;
    }
    f32 alpha = 1.0f;
    if (stage->type == 19 && stage->field_0x14 != 0) {
        if (stage->field_0x18 < 1.0f) {
            alpha = 1.0f - stage->field_0x18;
        } else {
            const f32 fade_start = stage->field_0x1c - 1.0f;
            if (stage->field_0x18 >= fade_start) {
                alpha = (stage->field_0x18 - fade_start) / (stage->field_0x1c - fade_start);
            } else {
                alpha = 0.0f;
            }
        }
    }
    return alpha;
}

void BonusWin_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 current) {
    if (current != 0) {
        if (stage->field_0x14 == 0) {
            iconalphaoverride = 0.0f;
        } else {
            const f32 time = stage->field_0x18;
            if (time >= stage->field_0x1c - 0.1f) {
                iconalphaoverride = 0.0f;
            } else if (time >= stage->field_0x1c - 0.6f) {
                const f32 fade = time - (stage->field_0x1c - 0.6f);
                iconalphaoverride = 1.0f - (fade + fade);
            } else if (time >= 0.5f) {
                iconalphaoverride = 1.0f;
            } else {
                iconalphaoverride = time + time;
            }
            if (time < 2.0f && NuFmod(time, 0.2f) >= 0.1f) {
                const bool left = (packet->field_0xb0 & 0x20) == 0;
                SmartTextEx(TTab[tWINNER], left ? -0.675f : 0.675f, STATSPOSY, 1.0f, 0.7f, 0.7f, 0.7f, left ? 2 : 8, 0,
                            255, 0, 0.35f, 1, NULL, 0, static_cast<i32>(iconalphaoverride * 128.0f));
            }
            DrawBonusScore(STATSPOSY, StatusPacket.player_active[0], StatusPacket.player_active[1], iconalphaoverride,
                           BonusScore);
        }
    } else {
        if (stage->field_0x12 != 0) {
            const f32 alpha = getFinishedStatusAlpha(packet);
            if (packet->stage->type != 25) {
                DrawBonusScore(STATSPOSY, StatusPacket.player_active[0], StatusPacket.player_active[1], alpha, BonusScore);
            }
        }
    }
}

void BonusTime_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 current) {
    if (current != 0) {
        if (stage->field_0x14 > 0) {
            f32 alpha = stage->field_0x18;
            f32 time;
            if (alpha < 0.5f) {
                alpha += alpha;
                time = 0.0f;
            } else {
                if (alpha < 3.0f) {
                    time = 0.0f;
                } else {
                    time = alpha - 3.0f;
                }
                alpha = 1.0f;
            }
            DrawBonusTime(packet,
                          1.0f -
                              (NuTrigTable[(static_cast<i32>(time * 32768.0f + 16384.0f) >> 1) & 0x7fff] + 1.0f) * 0.5f,
                          static_cast<i32>(alpha * 128.0f));
        }
    } else {
        if (stage->field_0x12 != 0) {
            DrawBonusTime(packet, 1.0f, static_cast<i32>(getFinishedStatusAlpha(packet) * 128.0f));
        }
    }
}

void LevelComplete_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active) {
    if (active != 0) {
        LevelComplete_LSW_DrawActive(stage, packet);
    } else {
        if (stage->field_0x12 != 0) {
            const f32 alpha = getFinishedStatusAlpha(packet);
            if ((stage->type == 26 && packet->mission_state == 2) ||
                (stage->type == 23 && packet->challenge_state == 2)) {
                CoinTotal_Draw(*packet->score, STATSPOSY, CoinTotalScale, 1, alpha, 255, 191, 0);
            }
        }
    }
}

void Coins_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active) {
    if (active != 0) {
        f32 alpha = 1.0f;
        f32 coin_y = STATSPOS2Y;
        f32 total_y = STATSPOS2Y;
        f32 scale = 2.0f;
        switch (stage->field_0x14) {
            case 0:
                alpha = 0.0f;
                break;
            case 1: {
                alpha = stage->field_0x18;
                f32 ratio = stage->field_0x18 / stage->field_0x1c;
                const f32 blend =
                    1.0f - (NuTrigTable[(static_cast<i32>(ratio * 32768.0f + 16384.0f) >> 1) & 0x7fff] + 1.0f) * 0.5f;
                if (stage->field_0x1c == 0.0f || stage->field_0x18 == 0.0f) {
                    ratio = 0.0f;
                }
                icon_y = StatusIconsOnOff(ratio);
                total_y = (0.3f - STATSPOS2Y) * blend + STATSPOS2Y;
                coin_y = (STATSPOSY - STATSPOS2Y) * blend + STATSPOS2Y;
                break;
            }
            case 2:
                total_y = 0.3f;
                coin_y = STATSPOSY;
                break;
            case 3: {
                alpha = 1.0f - stage->field_0x18;
                const f32 blend =
                    1.0f -
                    (NuTrigTable[(static_cast<i32>((1.0f - stage->field_0x18 / stage->field_0x1c) * 32768.0f + 16384.0f) >>
                                  1) &
                                 0x7fff] +
                     1.0f) *
                        0.5f;
                total_y = (0.3f - STATSPOSY) * blend + STATSPOSY;
                coin_y = STATSPOS2Y + (STATSPOSY - STATSPOS2Y) * blend;
                scale = (2.0f - CoinTotalScale) * blend + CoinTotalScale;
                break;
            }
        }
        if (stage->field_0x14 >= 1) {
            const f32 player_alpha = packet->player_active[0] == 0 ? DROPINALPHA : 1.0f;
            const u32 coins = packet->coins_remaining[0];
            const f32 score_x = -PANEL_SCOREX;
            const i32 object_index = coins > 999 ? 0xc3 : coins > 99 ? 0xbb : 0xb3;
            if (__builtin_expect(WORLD->lev_objs[object_index].active != 0, 0)) {
                DrawPanel3DObject(-PANEL_COINX, coin_y, 1.0f, PANEL_COINSCALE_END, PANEL_COINSCALE_END, PANEL_COINSCALE_END,
                                  0, 0, 0, &WORLD->lev_objs[object_index].special, 0, player_alpha);
            }
            char text[256];
            Text_MakeScore(packet->coins_remaining[0], text);
            Text3DEx(text, score_x, coin_y, 1.0f, PANEL_SCORESCALE, PANEL_SCORESCALE, PANEL_SCORESCALE, 2, 255, 191, 0,
                     static_cast<u8>(static_cast<i32>(player_alpha * 128.0f)));
            CoinTotal_Draw(*packet->score, total_y, scale, 1, 1.0f, 255, 191, 0);
        }
        alpha = !(alpha >= 0.0f) ? 0.0f : alpha > 1.0f ? 1.0f : alpha;
        Text3DEx(TTab[tCOINTOTAL], 0.0f, STATUS_TITLE_Y, 1.0f, 0.5f, 0.5f, 0.5f, 0, 255, 255, 255,
                 static_cast<u8>(static_cast<i32>(alpha * 128.0f)));
    } else if (stage->field_0x12 != 0) {
        const f32 alpha = getFinishedStatusAlpha(packet);
        CoinTotal_Draw(*packet->score,
                       (STATSPOSY - STATSPOS2Y) * NuTrigTable[(static_cast<i32>(alpha * 16384.0f) >> 1) & 0x7fff] +
                           STATSPOS2Y,
                       CoinTotalScale, 1, 1.0f, 255, 191, 0);
    }
}

void AllMiniKits_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 current) {

    if (current != 0) {
        if (stage->field_0x14 < 1)
            return;
        const f32 time = stage->field_0x18;
        f32 title_alpha;
        f32 icon_alpha = 0.0f;
        f32 y = 0.225f;
        if (time < 0.5f)
            title_alpha = time + time;
        else if (time < 3.5f)
            title_alpha = 1.0f;
        else if (time < 4.0f) {
            icon_alpha = (time - 3.5f) + (time - 3.5f);
            y = (1.0f - NuTrigTable[(static_cast<i32>(icon_alpha * 16384.0f) >> 1) & 0x7fff]) * -0.5f + 0.225f;
            title_alpha = 1.0f - icon_alpha;
        } else if (time < 6.0f) {
            title_alpha = 0.0f;
            icon_alpha = 1.0f;
            y = (1.0f - NuTrigTable[0x2000]) * -0.5f + 0.225f;
        } else if (time < 6.5f) {
            title_alpha = 0.0f;
            icon_alpha = 1.0f - ((time - 6.0f) + (time - 6.0f));
            if (icon_alpha <= 0.0f)
                return;
        } else
            title_alpha = 0.0f;
        if (icon_alpha > 0.0f) {
            DrawCharIcon(id_SLAVE1, 0.0f, y, 0.0f, 0.4f, 0xa7, icon_alpha, icon_alpha, 1, NULL);
            SmartTextEx(TTab[CDataList[id_SLAVE1].name_id], 0.0f, -0.1f, 1.0f, 0.6f, 0.6f, 0.6f, 0, STATUS_R, STATUS_G,
                        STATUS_B, 1.7f, 1, NULL, 0, static_cast<i32>(icon_alpha * 128.0f));
        }
        if (title_alpha > 0.0f) {
            Text3DEx(TTab[tALLMINIKITSBUILT], 0.0f, 0.225f, 1.0f, 0.7f, 0.7f, 0.7f, 0, STATUS_R, STATUS_G, STATUS_B,
                     static_cast<u8>(title_alpha * 128.0f));
        }
        return;
    }
    if (stage->field_0x12 != 0) {
        const f32 alpha = getFinishedStatusAlpha(packet);
        const f32 kit_wobble = NuTrigTable[(static_cast<i32>(alpha * 16384.0f) >> 1) & 0x7fff];
        const u8 opacity = static_cast<u8>(static_cast<i32>(alpha * 128.0f));
        const i32 angle = GameTimer.time_elapsed_mod_seconds > 0.25f
                              ? 0x2000
                              : (static_cast<i32>(GameTimer.time_elapsed_mod_seconds * 32768.0f + 16384.0f) >> 1) &
                                    0x7fff;
        const f32 pulse = NuTrigTable[angle];
        if (Game.area_save[packet->area->index].field_0x5[0] != 0) {
            DrawStatusMiniKit(-0.6f, -0.5f, 1.1f, kit_wobble * 0.15f, 1.0f,
                              Game.area_save[packet->area->index].field_0x5[0], packet, 0.0f);
        } else {
            const f32 size = ((1.0f - fabsf(pulse)) + 1.0f) * 1.2f;
            Text3DEx("?", -0.6f, -0.6f, 1.1f, size, size, size, 0, 255, 255, 255, opacity);
        }
        if (Game.area_save[packet->area->index].field_0x5[0] != packet->minikit_max) {
            char text[60];
            sprintf(text, "%i/%i", Game.area_save[packet->area->index].field_0x5[0], packet->minikit_max);
            Text3DEx(text, -0.6f, -0.8f, 1.0f, 0.5f, 0.5f, 0.5f, 0, 255, 0, 127, opacity);
        } else {
            Text3DEx("$", -0.6f, -0.7f, 1.0f, 0.8f, 0.8f, 0.8f, 0, 255, 0, 127, opacity);
        }
    }
}

void MiniKit_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 current) {

    if (current == 0) {
        if (stage->field_0x12 == 0)
            return;
        const f32 alpha = getFinishedStatusAlpha(packet);
        const f32 spin = NuTrigTable[(static_cast<i32>(alpha * 16384.0f) >> 1) & 0x7fff];
        const i32 opacity = static_cast<i32>(alpha * 128.0f);
        i32 angle = 0x2000;
        if (!(GameTimer.time_elapsed_mod_seconds > 0.25f)) {
            angle = (static_cast<i32>(GameTimer.time_elapsed_mod_seconds * 32768.0f + 16384.0f) >> 1) & 0x7fff;
        }
        const f32 pulse = NuTrigTable[angle];
        if (Game.area_save[packet->area->index].field_0x5[0] == 0) {
            const f32 size = ((1.0f - fabsf(pulse)) + 1.0f) * 1.2f;
            Text3DEx("?", -0.6f, -0.6f, 1.1f, size, size, size, 0, 255, 255, 255, static_cast<u8>(opacity));
        } else if (alpha > 0.0f) {
            DrawStatusMiniKit(-0.6f, -0.5f, 1.1f, spin * 0.15f, 1.0f, Game.area_save[packet->area->index].field_0x5[0],
                              packet, 0.0f);
        }
        if (packet->minikit_max == Game.area_save[packet->area->index].field_0x5[0]) {
            Text3DEx("$", -0.6f, -0.7f, 1.0f, 0.8f, 0.8f, 0.8f, 0, 255, 0, 127, static_cast<u8>(opacity));
        } else {
            char text[60];
            sprintf(text, "%i/%i", Game.area_save[packet->area->index].field_0x5[0], packet->minikit_max);
            Text3DEx(text, -0.6f, -0.8f, 1.0f, 0.5f, 0.5f, 0.5f, 0, 255, 0, 127, static_cast<u8>(opacity));
        }
    } else {
        f32 title_alpha = 1.0f;
        switch (stage->field_0x14) {
            case 0:
                title_alpha = 0.0f;
                break;
            case 1: {
                title_alpha = stage->field_0x18;
                i32 angle = 0x6000;
                if (!(title_alpha >= 1.0f))
                    angle = (static_cast<i32>(title_alpha * 32768.0f + 16384.0f) >> 1) & 0x7fff;
                const f32 blend = 1.0f - (NuTrigTable[angle] + 1.0f) * 0.5f;
                if (title_alpha <= stage->field_0x1c)
                    DrawStatusMiniKit(0.0f, blend * -1.4f + 1.4f, 1.1f, 0.333f, 0.0f, currentminikit, packet, 0.0f);
                else
                    DrawStatusMiniKit(0.0f, 0.0f, 1.1f, 0.333f, 0.0f, currentminikit, packet, 0.0f);
                const i32 count = packet->new_minikits < 1 ? Game.area_save[packet->area->index].field_0x5[0]
                                  : currentminikit < 0     ? 0
                                                           : currentminikit;
                DrawMiniKitCount(blend, 1.0f, count, packet->minikit_max);
                break;
            }
            case 2: {
                const f32 duration = stage->field_0x1c - 0.25f;
                f32 size;
                if (__builtin_expect(stage->field_0x18 < duration, 0)) {
                    i32 angle = 0;
                    if (duration != 0.0f && stage->field_0x18 != 0.0f)
                        angle =
                            (static_cast<i32>((stage->field_0x18 / duration) * 16384.0f + 49152.0f + 16384.0f) >> 1) &
                            0x7fff;
                    size = NuTrigTable[angle] * 0.333f;
                } else
                    size = 0.333f;
                DrawStatusMiniKit(0.0f, 0.0f, 1.1f, 0.333f, size, currentminikit + 1, packet, 0.0f);
                const i32 count = packet->new_minikits < 1 ? Game.area_save[packet->area->index].field_0x5[0]
                                  : currentminikit < 0     ? 0
                                                           : currentminikit;
                DrawMiniKitCount(1.0f, 1.0f, count, packet->minikit_max);
                break;
            }
            case 3: {
                f32 blend = 0.0f;
                if (__builtin_expect(!(stage->field_0x1c - jibberlen >= stage->field_0x18), 0))
                    blend = (stage->field_0x18 - (stage->field_0x1c - jibberlen)) / jibberlen;
                DrawStatusMiniKit(0.0f, 0.0f, 1.1f, 0.333f, 0.333f, currentminikit + 1, packet, blend);
                i32 count =
                    packet->new_minikits < 1 ? Game.area_save[packet->area->index].field_0x5[0] : currentminikit;
                if (!(blend < 0.5f))
                    ++count;
                if (count < 0)
                    count = 0;
                const f32 size =
                    (1.0f - fabsf(NuTrigTable[(static_cast<i32>(blend * 32768.0f + 16384.0f) >> 1) & 0x7fff])) * 0.25f +
                    1.0f;
                DrawMiniKitCount(1.0f, size, count, packet->minikit_max);
                break;
            }
            case 4: {
                title_alpha = 1.0f - stage->field_0x18;
                f32 progress = 0.0f;
                if (stage->field_0x1c != 0.0f && stage->field_0x18 != 0.0f)
                    progress = stage->field_0x18 / stage->field_0x1c;
                const f32 blend =
                    1.0f -
                    (NuTrigTable[(static_cast<i32>(progress * 32768.0f + 16384.0f) >> 1) & 0x7fff] + 1.0f) * 0.5f;
                DrawStatusMiniKit(blend * -0.6f + 0.0f, blend * -0.5f + 0.0f, 1.1f, blend * -0.183f + 0.333f, 1.0f,
                                  Game.area_save[packet->area->index].field_0x5[0], packet, 0.0f);
                const f32 off = stage->field_0x18 < 1.0f ? 1.0f - stage->field_0x18 : 0.0f;
                DrawMiniKitCount(
                    1.0f - (NuTrigTable[(static_cast<i32>(off * 32768.0f + 16384.0f) >> 1) & 0x7fff] + 1.0f) * 0.5f,
                    1.0f, Game.area_save[packet->area->index].field_0x5[0], packet->minikit_max);
                break;
            }
            case 6: {
                title_alpha = 1.0f - stage->field_0x18;
                i32 angle = 0x2000;
                if (!(stage->field_0x18 >= 1.0f))
                    angle = (static_cast<i32>(title_alpha * 32768.0f + 16384.0f) >> 1) & 0x7fff;
                if (__builtin_expect(stage->field_0x18 < stage->field_0x1c, 0)) {
                    DrawMiniKitCount(1.0f - (NuTrigTable[angle] + 1.0f) * 0.5f, 1.0f,
                                     Game.area_save[packet->area->index].field_0x5[0], packet->minikit_max);
                    DrawStatusMiniKit(0.0f, 0.0f, 1.1f, 0.333f, 1.0f, Game.area_save[packet->area->index].field_0x5[0],
                                      packet, 0.0f);
                }
                break;
            }
            default:
                break;
        }
        title_alpha = title_alpha < 0.0f ? 0.0f : title_alpha > 1.0f ? 1.0f : title_alpha;
        Text3DEx(TTab[tMINIKIT], 0.0f, STATUS_TITLE_Y, 1.0f, 0.5f, 0.5f, 0.5f, 0, 255, 255, 255,
                 static_cast<u8>(title_alpha * 128.0f));
    }
}

void LSW_registerStatusScreen() {
    REGISTERSTATUSPACKET_s registration = {
        &StatusPacket_LSW, InitStatusScreen_LSW, FinishStatusPacket_LSW, ResetStatusPacket_LSW, DrawStatusBG_LSW, 0.5f,
    };
    RegisterStatusScreen(StatusStages_LSW, NULL, &registration);
}

