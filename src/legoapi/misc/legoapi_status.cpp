// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"
#include <string.h>
#include <stdio.h>
#include "globals.h"
#include "gameframework/saveload.h"
#include "legoapi/characters/motion.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/numtx_inline.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/screens/gamestatus_lsw.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "gameapi/gui/apimenu.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/render/core/render.h"
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
#include "nu2api/nusound/nusound.h"
#include "legoapi/render/core/screen.h"
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
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include "nu2api/nu3d/nuportal.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/render/fx/parts.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nu3d/nugscn.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/menus/core/hint.h"
#include "legoapi/menus/screens/missions.h"
#include "legoapi/misc/legoapi_status.h"
#include "legogame/game.h"
#include "nu2api/nuandroid/ios_device.h"

extern "C" {
    i32 DEVCDDVDROM_Interrogate(NUFILE_DEVICE *device) {
        device->status = 1;
        return 1;
    }

    i32 DEVMEMORYCARD_Interrogate(NUFILE_DEVICE *device) {
        device->status = 1;
        return 1;
    }

    void DebugLog(void) {
    }
} // extern "C"

extern "C" STATUSPACKET_s StatusPacket;

static i32 gamedemo_option;
static i32 goldbrickmsgcount;

STATUS_STAGE_s *StatusStages;
static i32 DrawGoldBrick_Stage;
static i32 DrawGoldBrick_Phase;
extern "C" TIMER BonusTimer;
extern i16 tSUPERSTORYCOMPLETE, tNEWBESTTIME, tLEVELCOMPLETE, tMISSIONCOMPLETE;
extern i16 tNONEWBESTTIME;
extern i16 tCHALLENGECOMPLETE, tMINIKIT;
extern i32 newminikitcount;
extern i32 currentminikit;
static u32 collect_suit_bits;

NEWMINIPIECE_s NewMiniPiece[10];

static inline bool StatusRumbleTargetsPlayer(i32 player, i32 index) {
    return player == index || player == -1;
}

extern i16 tTRUEJEDI, tSTORY, tFREEPLAY;
extern "C" i32 reset_area;
extern i16 tMAP, tRESTART, tFINISHSTORY, tCONTINUESTORY, tFINALCHAPTERLOCKED, tNEXTCHAPTERLOCKED;

struct MENU_s;
struct STATUSPACKET_s;

extern i16 tNONEWBESTTIME, tNEWBESTTIME;

extern i16 tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);

extern i16 id_ANAKINJEDISCARRED, id_WOOKIEE, id_TWOFACE;

extern i16 tLEVELCOMPLETE, tFREEPLAYUNLOCKED, tSUPERSTORYCOMPLETE, tBONUSUNLOCKED;
extern i16 tEPISODEVUNLOCKED, tEPISODEVIUNLOCKED, tCHALLENGECOMPLETE, tOUTOFTIME;
extern i16 tYOUDIDNTCOLLECTALL10CANISTERS, tMISSIONCOMPLETE, tSTORYCLIPSUNLOCKED;
extern i32 currentminikit, newminikitcount;
extern i16 tMINIKIT;
extern i16 tALLMINIKITSBUILT;
extern NUCOLOUR3 panelcol[3], panelamb;

static STATUSMINIKITPART_s KitPart[10];

f32 slideseek;
f32 pop_timer;
f32 jibberlen = 0.1f;
f32 slidetime = 0.25f;

rtldata_s KitPartRTL;

void SetDrawGoldBrick(STATUSPACKET_s *packet, i32) {
    const i32 stage = packet->current_gold_brick;
    if (packet->gold_brick_enabled[stage] != 0) {
        DrawGoldBrick_Stage = stage;
        DrawGoldBrick_Phase = packet->stage->field_0x14;
    }
}

void DrawStatusText(char *text, u16 angle, float x, float y, float scale, u32 colour, i32 alignment) {
    static NUMTX status_mtx;

    NUVEC origin = {0.0f, 0.0f, 800.0f};
    NUVEC position = {x * 350.0f, y * 300.0f, 0.0f};
    u16 encoded[512];

    NuQFntSetJustifiedTolerances(1.2f, 1.2f);
    NuMtxSetIdentity(&status_mtx);
    NuMtxSetRotationX(&status_mtx, 0);
    NuMtxRotateY(&status_mtx, 0);
    NuMtxRotateZ(&status_mtx, angle);
    NuMtxTranslate(&status_mtx, &origin);
    NuMtxTranslate(&status_mtx, &position);
    NuMtxMul(&status_mtx, &status_mtx, NuCameraGetMtx());

    NuQFntSet(QFont3DZ);
    NuQFntSetMtx(QFont3DZ, &status_mtx);
    NuQFntPushPrintMode(NUQFNT_CSMODE_ABSOLUTE);
    NuQFntSetCoordinateSystem(NUQFNT_CSMODE_ABSOLUTE);
    NuQFntSetColour(QFont3DZ, colour);
    NuQFntSetScale(QFont3DZ, scale, scale);
    Text3DStringEncode(text, encoded);

    switch (alignment) {
        case 2: {
            const f32 height = NuQFntHeight(QFont3DZ);
            NuQFntMove(QFont3DZ, 0.0f, -height * 0.5f, 0.0f);
            break;
        }
        case 8: {
            const f32 height = NuQFntHeight(QFont3DZ);
            const f32 width = NuQFntPrintLenW(QFont3DZ, encoded);
            NuQFntMove(QFont3DZ, -width, -height * 0.5f, 0.0f);
            break;
        }
        default: {
            const f32 height = NuQFntHeight(QFont3DZ);
            const f32 width = NuQFntPrintLenW(QFont3DZ, encoded);
            NuQFntMove(QFont3DZ, -width * 0.5f, -height * 0.5f, 0.0f);
            break;
        }
    }
    NuQFntPrintW(QFont3DZ, encoded);
    NuQFntPopPrintMode();
    NuQFntSetCoordinateSystem(NUQFNT_CSMODE_NORMALISED);
}

void DrawStatusTextFraction(i32 current, i32 total, float x, float y, u16 angle, float scale, u32 colour,
                            float animation, float animation_duration) {
    char text[16];
    u16 encoded[64];
    NUMTX matrix;
    NUVEC origin = {0.0f, 0.0f, 800.0f};
    NUVEC position = {x * 400.0f, y * 250.0f, 0.0f};
    NuQFntSetJustifiedTolerances(1.2f, 1.2f);
    NuMtxSetIdentity(&matrix);
    NuMtxSetRotationX(&matrix, 0);
    NuMtxRotateY(&matrix, 0);
    NuMtxRotateZ(&matrix, angle);
    NuMtxTranslate(&matrix, &origin);
    NuMtxTranslate(&matrix, &position);
    NuMtxMul(&matrix, &matrix, NuCameraGetMtx());
    NuQFntSet(QFont3DZ);
    NuQFntSetMtx(QFont3DZ, &matrix);
    NuQFntPushPrintMode(NUQFNT_CSMODE_ABSOLUTE);
    NuQFntSetCoordinateSystem(NUQFNT_CSMODE_ABSOLUTE);
    NuQFntSetColour(QFont3DZ, colour);
    NuQFntSetScale(QFont3DZ, scale, scale);
    sprintf(text, "%i/%i", current, total);
    Text3DStringEncode(text, encoded);
    const f32 full_width = NuQFntPrintLenW(QFont3DZ, encoded);
    const f32 height = NuQFntHeight(QFont3DZ);
    NuQFntMove(QFont3DZ, -full_width * 0.5f, -height * 0.5f, 0.0f);
    if (animation > 0.0f) {
        const i32 wave_angle = static_cast<i32>((animation / animation_duration) * 32768.0f + 16384.0f);
        const f32 blend = 1.0f - (NuTrigTable[(wave_angle >> 1) & 0x7fff] + 1.0f) * 0.5f;
        const f32 wave = 1.0f - blend;
        const f32 small_scale = scale - 0.5f + wave * 0.5f;
        const f32 large_scale = 0.5f + scale - wave * 0.5f;
        sprintf(text, "/%i", total);
        Text3DStringEncode(text, encoded);
        const f32 suffix_width = NuQFntPrintLenW(QFont3DZ, encoded);
        const f32 suffix_offset = full_width - suffix_width;
        const f32 half_full_width = 0.5f * full_width;
        const f32 suffix_x = suffix_offset - half_full_width;
        const f32 suffix_height = NuQFntHeight(QFont3DZ);
        NuQFntMove(QFont3DZ, suffix_x, -suffix_height * 0.5f, 0.0f);
        NuQFntPrintW(QFont3DZ, encoded);
        sprintf(text, "%i", current);
        Text3DStringEncode(text, encoded);
        const f32 current_width = NuQFntPrintLenW(QFont3DZ, encoded);
        NuQFntSetScale(QFont3DZ, large_scale, large_scale);
        const f32 large_width = NuQFntPrintLenW(QFont3DZ, encoded);
        const f32 current_center = current_width * 0.5f - half_full_width;
        const f32 large_x = current_center - large_width * 0.5f;
        const f32 large_height = NuQFntHeight(QFont3DZ);
        NuQFntMove(QFont3DZ, large_x, -large_height * 0.5f, 0.0f);
        NuQFntPrintW(QFont3DZ, encoded);
        NuQFntSetScale(QFont3DZ, small_scale, small_scale);
        const f32 small_width = NuQFntPrintLenW(QFont3DZ, encoded);
        const f32 small_x = current_center - small_width * 0.5f;
        const f32 small_height = NuQFntHeight(QFont3DZ);
        NuQFntMove(QFont3DZ, small_x, -small_height * 0.5f, 0.0f);
        NuQFntPrintW(QFont3DZ, encoded);
    } else {
        sprintf(text, "%i", current);
        Text3DStringEncode(text, encoded);
        NuQFntPrintW(QFont3DZ, encoded);
        sprintf(text, "/%i", total);
        Text3DStringEncode(text, encoded);
        NuQFntPrintW(QFont3DZ, encoded);
    }
    NuQFntPopPrintMode();
    NuQFntSetCoordinateSystem(NUQFNT_CSMODE_NORMALISED);
}

STATUS_STAGE_s *FindStatusStage(i32 type) {
    for (i32 i = 0; StatusStages[i].type != -1; ++i) {
        if (StatusStages[i].type == type) {
            return &StatusStages[i];
        }
    }
    return NULL;
}

void StatusStage_Reset(STATUS_STAGE_s *stage) {
    if (stage != NULL) {
        stage->field_0x18 = 0;
        stage->field_0x1c = 1.0f;
        stage->field_0x14 = -1;
        stage->field_0x12 = 0;
    }
}

void NewStatusRumbleBuzz(i32 player, float amount, float buzz, i32 priority) {
    for (i32 i = 0; i < 2; ++i) {
        if (StatusPacket.player_active[i] != 0) {
            if (StatusRumbleTargetsPlayer(player, i)) {
                RUMBLEPACKET *rumble = &StatusPacket.rumble[i];
                if (amount > 0.0f && (__builtin_expect(rumble->time <= 0.0f, 0) ||
                                      __builtin_expect(amount > rumble->time / rumble->duration * rumble->amount, 0))) {
                    rumble->amount = amount;
                    rumble->duration = amount;
                    rumble->time = amount;
                }
                if (buzz > rumble->buzz_time) {
                    rumble->buzz_time = buzz;
                }
                if (priority > 0 && ++priority > rumble->priority) {
                    rumble->priority = static_cast<u8>(priority);
                }
            }
        }
    }
}

void NextStatusStage(STATUSPACKET_s *packet) {
    if (packet->stage != NULL) {
        if (packet->stage->field_0x14 == -1 && packet->field_0x68 > packet->stage->field_0x18) {
            packet->field_0xb0 |= 2;
            return;
        }
        packet->stage->field_0x12 = 1;
    }

    STATUS_STAGE_s *stage = packet->stage;
    ++packet->current_gold_brick;
    packet->previous_stage_2 = packet->previous_stage;
    packet->previous_stage = stage;
    packet->stage = packet->next_stage;
    packet->next_stage = FindStatusStage(packet->stage_types[packet->current_gold_brick + 1]);

    while (packet->stage == NULL || packet->next_stage == NULL) {
        if (packet->stage == NULL && packet->next_stage != NULL) {
            packet->stage = packet->next_stage;
            packet->next_stage = NULL;
        }

        ++packet->current_gold_brick;
        if (!(packet->current_gold_brick + 1 < packet->stage_count)) {
            packet->next_stage = FindStatusStage(10);
        } else {
            packet->next_stage = FindStatusStage(packet->stage_types[packet->current_gold_brick + 1]);
        }
    }

    StatusStage_Reset(packet->stage);
    packet->field_0xb0 &= ~2;
    status_prompt = 0;

    if ((packet->stage != NULL && packet->stage->type == 11) || packet->stage->type == 10) {
        ResetRumble(&packet->rumble[0]);
        ResetRumble(&packet->rumble[1]);
    }
    if (packet->stage != NULL && packet->stage->type == 11) {
        ReCalculateCompletionPoints();
    }
}

i32 AddGoldBrickMessage(STATUSPACKET_s *packet, i16 brick) {
    if (goldbrickmsgcount < 16) {
        reinterpret_cast<i16 *>(packet->field_0x12c)[goldbrickmsgcount] = brick;
        ++goldbrickmsgcount;
    }
    AddToGoldBricks();
    return 1;
}

void AddStatusStage(STATUSPACKET_s *packet, i32 type, i32 gold_brick_enabled) {
    const u8 index = packet->stage_count;
    packet->stage_types[index] = static_cast<i8>(type);
    packet->gold_brick_enabled[index] = static_cast<u8>(gold_brick_enabled);
    packet->stage_count = index + 1;
}

i32 newCharactersCollected(STATUSPACKET_s *) {
    i32 count = 0;
    for (i32 i = 0; Area_PlayerIDList[i] != -1 && count < 7; ++i) {
        const i16 player = Area_PlayerIDList[i];
        const i32 id = player;
        i32 previous = 0;
        while (previous < count && StatusCollectList.ids[previous] != id) {
            ++previous;
        }
        if (previous == count) {
            if (__builtin_expect(Game_CharacterSave != NULL && (Game_CharacterSave[id] & 1) != 0, 0)) {
                continue;
            }
            const i32 index = InCollectList_Index(id, MasterCollection.list, MasterCollection.count_y);
            if (__builtin_expect(index != -1 && MasterCollection.list[index].can_buy == 0, 0)) {
                StatusCollectList.ids[count++] = player;
                if (MasterCollection.list[index].field5_0x9 != 0) {
                    AddToCompletionPoints(POINTS_PER_CHARACTER);
                }
            }
        }
    }
    StatusCollectList.ids[count] = -1;
    return count;
}

void StatusPacketReset(STATUSPACKET_s *packet) {
    const STATUSPACKET_LSW_s *lsw_packet = packet->lsw_packet;
    auto init_callback = packet->init_callback;
    auto finish_callback = packet->finish_callback;
    void (*reset_callback)(STATUSPACKET_s *) = packet->reset_callback;
    void (*draw_background_callback)(STATUSPACKET_s *) = packet->draw_background_callback;
    const f32 field_0x68 = packet->field_0x68;

    reset_callback(packet);
    memset(packet, 0, sizeof(*packet));

    packet->reset_callback = reset_callback;
    packet->lsw_packet = const_cast<STATUSPACKET_LSW_s *>(lsw_packet);
    packet->init_callback = init_callback;
    packet->finish_callback = finish_callback;
    packet->draw_background_callback = draw_background_callback;
    packet->field_0x68 = field_0x68;
}

void InitStatusScreen(WORLDINFO_s *world) {
    STATUSPACKET_s &p = StatusPacket;
    char event[64];
    if (p.lsw_packet == NULL) {
        return;
    }
    for (i32 i = 0; StatusStages[i].type != -1; ++i) {
        StatusStage_Reset(&StatusStages[i]);
    }
    StatusPacketReset(&p);
    icon_y = STATSPOSY;
    newminikitcount = 0;
    currentminikit = 0;
    p.field_0xb0_0 = 0;
    ResetRumble(&p.rumble[0]);
    ResetRumble(&p.rumble[1]);
    CoinTotalScale = 1.0f;
    goldbrickmsgcount = 0;
    if (netclient == 0) {
        for (i32 i = 0; i < 2; ++i) {
            if (Player[i] == NULL) {
                p.player_model[i] = 0xffff;
            } else {
                p.player_model[i] = Player[i]->id;
                p.player_active[i] = Player[i]->apiobj.player_controlled;
                p.coins_remaining[i] = Player[i]->coinpacket != NULL ? Player[i]->coinpacket->coins : 0;
                ++p.status_flags;
            }
        }
    }
    if (p.status_flags == 0) {
        return;
    }
    p.stage_count = 0;
    p.super_story_mode = SuperStory;
    p.free_play = FreePlay;
    p.saved_and_exited = from_save_and_exit;
    p.continue_story = 0;
    p.mission = Mission_Active(NULL);
    if (p.mission == NULL)
        p.mission_state = 0;
    else
        p.mission_state = Mission_CurrentState(NULL);
    if (__builtin_expect(from_save_and_exit != 0, 1)) {
        from_save_and_exit = 0;
        level_already_loaded = -1;
    }
    p.next_area = -1;
    p.field_0xbe = -1;
    p.newly_completed = 0;
    p.field_0xbd = 0;
    p.challenge_state = ChallengeMode;
    const i32 area = static_cast<i8>(world->level_sub_id);
    p.next_area_unlocked = 0;
    p.score = &Game.coins;
    p.previous_completion = Game.completion;
    p.previous_gold_bricks = Game.field_0x7c26[0];
    p.area_id = area;
    p.displayed_gold_bricks = Game.field_0x7c26[0];
    p.vehicle_area = 0;
    p.bonus_mode = 0;
    p.super_bonus_mode = 0;
    i32 episode = -1;
    if (__builtin_expect(area == -1, 0)) {
        p.area = NULL;
        p.episode = NULL;
        p.chapter = -1;
    } else {
        p.area = ADataList + area;
        p.episode_id = p.area->episode_index;
        episode = p.episode_id;
        if (__builtin_expect(episode != -1, 1)) {
            p.episode = EDataList + static_cast<i8>(p.area->episode_index);
        }
        p.chapter = p.area->area_index;
        if ((p.area->flags & AREAFLAG_VEHICLE_AREA) != 0)
            p.vehicle_area = 1;
        if ((p.area->flags & AREAFLAG_BONUS_AREA) != 0)
            p.bonus_mode = 1;
        if ((p.area->flags & 0x100) != 0)
            p.super_bonus_mode = 1;
    }
    if (p.init_callback(world, &p) == 0) {
        if (p.super_story_mode) {
            if (NewLData == STATUS_LDATA || WORLD->current_level == STATUS_LDATA) {
                const u32 previous_score = Game.episode_save[episode].superstory_score_target;
                p.previous_best_score = previous_score;
                p.previous_best_time = Game.episode_save[episode].superstory_time_limit;
                p.superstory_time = SuperStoryTimer.time_elapsed;
                p.superstory_score = SuperStoryScore;
                i32 gold = 0;
                if (episode == -1) {
                    p.new_best_score = 0;
                    p.new_best_time = 0.0f;
                    p.time_reward_score = *p.score;
                } else {
                    f32 new_time = p.superstory_time;
                    if (new_time < p.previous_best_time)
                        Game.episode_save[episode].superstory_time_limit = new_time;
                    else
                        new_time = 0.0f;
                    p.new_best_time = new_time;
                    u32 new_score = p.superstory_score;
                    if (new_score > previous_score)
                        Game.episode_save[episode].superstory_score_target = new_score;
                    else
                        new_score = 0;
                    p.new_best_score = new_score;
                    if (Game.episode_save[episode].superstory_complete == 0) {
                        Game.episode_save[episode].superstory_complete = 1;
                        AddToCompletionPoints(POINTS_PER_SUPERSTORY);
                        if (GOLDBRICKFORSUPERSTORY != 0)
                            gold = AddGoldBrickMessage(&p, tSUPERSTORYCOMPLETE);
                    }
                    p.time_reward_score = *p.score;
                    if (__builtin_expect(p.new_best_time != 0.0f, 1)) {
                        u64 reward = static_cast<i64>(static_cast<i32>(p.previous_best_time - p.new_best_time) * 100);
                        if (reward < 100)
                            reward = 100;
                        IncreaseScore(&p.time_reward_score, reward, 0);
                    }
                }
                p.final_reward_score = p.time_reward_score;
                IncreaseScore(&p.final_reward_score, static_cast<u64>(p.superstory_score), 0);
                AddStatusStage(&p, 14, gold);
                AddStatusStage(&p, 15, 0);
                AddStatusStage(&p, 16, 0);
                if (Game.field_0x7c26[0] > p.previous_gold_bricks)
                    AddStatusStage(&p, 19, 0);
                if (netclient == 0 && (p.new_best_time != 0.0f || p.new_best_score != 0 || p.superstory_score != 0)) {
                    AddStatusStage(&p, 11, 0);
                }
                hub_from_superstory = static_cast<i8>(p.area->episode_index);
            }
            AddStatusStage(&p, 10, 0);
            p.current_gold_brick = -1;
            p.stage_types[p.stage_count] = -1;
            p.next_stage = FindStatusStage(p.stage_types[0]);
            NextStatusStage(&p);
            if (episode != -1 && p.chapter < EDataList[episode].regular_areas) {
                p.next_area = EDataList[episode].area_ids[p.chapter + 1];
                if (p.next_area != -1 && (ADataList[p.next_area].flags & 2) != 0)
                    hub_from_superstory = episode;
            }
            return;
        }
        if ((p.mode_flags & 1) != 0 && area != -1) {
            p.field_0xb0 = (p.field_0xb0 & 0xdf) | ((BonusWinner & 1) << 5);
            p.elapsed_time = BonusTimer.time_elapsed;
            p.previous_best_time = Game.area_save[area].challenge_trial_time;
            i32 gold = 0;
            if (p.previous_best_time <= 0.0f || p.elapsed_time < p.previous_best_time) {
                if (WORLD->area->challenge_trial_time != 0 &&
                    WORLD->area->challenge_trial_time <= p.previous_best_time) {
                    AddToCompletionPoints(POINTS_PER_TIMETRIAL);
                    gold = AddGoldBrickMessage(&p, tNEWBESTTIME);
                }
                p.new_best_time = p.elapsed_time;
                Game.area_save[area].challenge_trial_time = p.elapsed_time;
            }
            if ((p.mode_flags & 2) == 0) {
                AddStatusStage(&p, 20, 0);
                if (p.elapsed_time < p.previous_best_time)
                    AddStatusStage(&p, 3, 0);
            } else {
                if (Game.area_save[p.area_id].area_complete == 0) {
                    Game.area_save[p.area_id].area_complete = 1;
                    AddToCompletionPoints(POINTS_PER_SUPERBONUSCOMPLETE);
                    p.newly_completed = 1;
                    if (GOLDBRICKFORSUPERBONUS != 0)
                        gold = AddGoldBrickMessage(&p, tLEVELCOMPLETE);
                }
                AddStatusStage(&p, 22, gold);
                gold = 0;
            }
            AddStatusStage(&p, 21, gold);
            if (Game.field_0x7c26[0] > p.previous_gold_bricks)
                AddStatusStage(&p, 19, 0);
            AddStatusStage(&p, 9, 0);
            AddStatusStage(&p, 10, 0);
            if (netclient == 0)
                AddStatusStage(&p, 11, 0);
            AddStatusStage(&p, 12, 0);
            p.current_gold_brick = -1;
            p.stage_types[p.stage_count] = -1;
            p.next_stage = FindStatusStage(p.stage_types[0]);
            NextStatusStage(&p);
            draw_player_icons = 1;
            return;
        }
        if (p.mission_state != 0) {
            p.elapsed_time = MissionSys->timer.time_elapsed;
            p.newly_completed = 0;
            p.original_score = *p.score;
            p.reward_score = p.original_score;
            i32 gold = 0;
            if (p.mission_state == 2) {
                const i32 mission = static_cast<i8>(MissionSys->mission->count);
                if (Game_MissionSave == NULL || reinterpret_cast<u8 *>(Game_MissionSave)[0x50 + mission] == 0) {
                    if (Game_MissionSave != NULL)
                        reinterpret_cast<u8 *>(Game_MissionSave)[0x50 + mission] = 1;
                    AddToCompletionPoints(POINTS_PER_MISSION);
                    gold = AddGoldBrickMessage(&p, tMISSIONCOMPLETE);
                    sprintf(event, "bounty_mission_%i_complete", static_cast<i8>(MissionSys->mission->count) + 1);
                    NuIOS_RecordFlurryEvent(event);
                }
                u64 reward =
                    static_cast<i64>(static_cast<i32>(static_cast<u16>(p.mission->time) - p.elapsed_time) * 150);
                if (reward < 150)
                    reward = 150;
                IncreaseScore(&p.reward_score, reward, 0);
                if (Game_MissionSave != NULL) {
                    f32 &best = reinterpret_cast<f32 *>(Game_MissionSave)[static_cast<i8>(MissionSys->mission->count)];
                    if (best == 0.0f || p.elapsed_time < best)
                        best = p.elapsed_time;
                }
            }
            AddStatusStage(&p, 26, gold);
            if (Game.field_0x7c26[0] > p.previous_gold_bricks)
                AddStatusStage(&p, 19, 0);
            AddStatusStage(&p, 9, 0);
            AddStatusStage(&p, 10, 0);
            goto challenge_finish;
        }
        if (p.challenge_state != 0) {
            p.elapsed_time = ChallengeTimer.time_elapsed;
            p.original_score = *p.score;
            p.reward_score = p.original_score;
            if (p.challenge_state == 2) {
                i32 gold = 0;
                if (Game.area_save[p.area_id].field_0x5[2] == 0) {
                    AddToCompletionPoints(POINTS_PER_CHALLENGE);
                    if (__builtin_expect(GOLDBRICKFORCHALLENGE != 0, 1))
                        gold = AddGoldBrickMessage(&p, tCHALLENGECOMPLETE);
                    Game.area_save[p.area_id].field_0x5[2] = 1;
                    if (p.episode_id != -1 && p.chapter != -1) {
                        sprintf(event, "challenge_ep%i_ch%i_complete", p.episode_id + 1, p.chapter + 1);
                        NuIOS_RecordFlurryEvent(event);
                    }
                }
                u64 reward = static_cast<i64>(
                    static_cast<i32>(ADataList[p.area_id].challenge_trial_time - p.elapsed_time) * 500);
                if (reward < 500)
                    reward = 500;
                IncreaseScore(&p.reward_score, reward, 0);
                AddStatusStage(&p, 23, 0);
                if (p.challenge_state == 2 && gold != 0)
                    AddStatusStage(&p, 25, 0);
            } else
                AddStatusStage(&p, 23, 0);
            if (Game.field_0x7c26[0] > p.previous_gold_bricks)
                AddStatusStage(&p, 19, 0);
            AddStatusStage(&p, 9, 0);
            AddStatusStage(&p, 10, 0);
        challenge_finish:
            if (netclient == 0)
                AddStatusStage(&p, 11, 0);
            AddStatusStage(&p, 12, 0);
            p.current_gold_brick = -1;
            p.stage_types[p.stage_count] = -1;
            p.next_stage = FindStatusStage(p.stage_types[0]);
            NextStatusStage(&p);
            draw_player_icons = 1;
            cointotal_i_obj[0] = cointotal_i_obj[1] = 0xc3;
            return;
        }
        if (area != -1) {
            if (BuildUpDone != 0)
                p.field_0xb0 |= 4;
            p.minikit_count = Game.area_save[area].field_0x5[0] + AreaGlobals.values.field_0x10;
            p.minikit_max = (p.area->flags & 0x10) != 0 ? 10 : 0;
            if (p.minikit_count > p.minikit_max)
                p.minikit_count = p.minikit_max;
            p.true_hero_target = static_cast<u32>(p.area->true_hero_targets[p.free_play]);
        }
        p.area_time = AreaTimer.time_elapsed;
        const u64 total = static_cast<u64>(p.coins_remaining[0]) + p.coins_remaining[1];
        if (__builtin_expect(total >= 4000000000ULL, 0))
            p.collected_score = 4000000000.0f;
        else
            p.collected_score = static_cast<f32>(p.coins_remaining[0] + p.coins_remaining[1]);
        p.newly_completed = 0;
        if ((p.field_0xb0 & 4) == 0) {
            f32 numerator = p.collected_score * 100.0f;
            if (__builtin_expect(numerator != 0.0f && p.true_hero_target != 0.0f, 1)) {
                f32 percent = numerator / p.true_hero_target;
                p.true_hero_percent = MIN(percent, 99.0f);
            } else {
                p.true_hero_percent = 0.0f;
            }
        } else {
            p.true_hero_percent = 100.0f;
        }
        p.coins_collected[0] = p.coins_remaining[0];
        p.coins_collected[1] = p.coins_remaining[1];
        draw_player_icons = 0;
        if (area != -1) {
            if (!p.free_play && p.episode != NULL && p.chapter < p.episode->regular_areas) {
                p.next_area = p.episode->area_ids[p.chapter + 1];
                if (p.chapter < p.episode->regular_areas - 1)
                    p.field_0xb0 |= 8;
            }
            for (i32 i = 0; i < AreaGlobals.values.field_0x10; ++i) {
                if (Game.area_save[area].field_0x5[0] < 10)
                    ++Game.area_save[area].field_0x5[0];
                u8 *save = Game.level_save + NewMiniPiece[i].level * 0x54;
                if (save[0x50] < 10) {
                    NuStrCpy(reinterpret_cast<char *>(save + save[0x50] * 8), NewMiniPiece[i].name);
                    ++save[0x50];
                }
            }
            i32 gold;
            i32 completed_episode;
            if (!p.free_play && Game.area_save[area].area_complete == 0 && (p.area->flags & 0x26) == 0) {
                Game.area_save[area].area_complete = 1;
                p.newly_completed = 1;
                AddToCompletionPoints(POINTS_PER_STORY);
                gold = 0;
                if ((p.mode_flags & 1) == 0 && (p.area->flags & 0x800) == 0)
                    gold = AddGoldBrickMessage(&p, tLEVELCOMPLETE);
                if (p.episode != NULL && Episode_IsComplete(p.episode, NULL) != 0)
                    completed_episode = static_cast<i8>(p.area->episode_index);
                else
                    completed_episode = 0;
                if (p.episode_id != -1 && p.chapter != -1) {
                    sprintf(event, "story_ep%i_ch%i_complete", p.episode_id + 1, p.chapter + 1);
                    NuIOS_RecordFlurryEvent(event);
                }
                if (p.area == NEGOTIATIONS_ADATA) {
                    Game.area_save[EDataList[1].area_ids[0]].complete = 1;
                    Game.area_save[EDataList[2].area_ids[0]].complete = 1;
                    Game.area_save[EDataList[3].area_ids[0]].complete = 1;
                    Game.area_save[EDataList[4].area_ids[0]].complete = 1;
                    Game.area_save[EDataList[5].area_ids[0]].complete = 1;
                }
                if ((p.field_0xb0 & 8) != 0)
                    Game.area_save[p.next_area].complete = 1;
                else if (p.episode != NULL) {
                    const i32 bonus = Episode_FindAreaFromFlags(p.episode, 5, 4);
                    const i32 bonus2 = Episode_FindAreaFromFlags(p.episode, 5, 5);
                    if (bonus != -1)
                        Game.area_save[bonus].complete = 1;
                    if (bonus2 != -1)
                        Game.area_save[bonus2].complete = 1;
                    hub_startoutsidebonusdoor_area = bonus2;
                }
            } else {
                gold = 0;
                completed_episode = 0;
            }
            if (p.newly_completed != 0)
                AddStatusStage(&p, 13, gold);
            if (completed_episode != 0)
                AddStatusStage(&p, completed_episode + 29, 0);
        }
        if (p.area != NULL && static_cast<i8>(p.area->cheat) != -1 && Game.area_save[area].field_0x5[1] == 0 &&
            AreaGlobals.values.field_0x08 != 0) {
            AddToCompletionPoints(POINTS_PER_REDBRICK);
            Game.area_save[area].field_0x5[1] = 1;
            AddStatusStage(&p, 18, 0);
            if (p.episode_id != -1 && p.chapter != -1) {
                sprintf(event, "redbrick_ep%i_ch%i_awarded", p.episode_id + 1, p.chapter + 1);
                NuIOS_RecordFlurryEvent(event);
            }
        }
        if (p.newly_completed != 0 && newCharactersCollected(&p) > 0) {
            AddStatusStage(&p, 35, 0);
            AddStatusStage(&p, 36, 0);
            RememberPlayerIDs(1, static_cast<i16>(p.player_model[0]), static_cast<i16>(p.player_model[1]));
        }
        if (area != -1 && ((p.area->flags & 0x136) == 0x10 || (p.area->flags & 0x4000) != 0)) {
            if (BOTHTRUEJEDIGOLDBRICKS != 0) {
                u8 &complete = Game.area_save[area].true_hero_complete[p.free_play];
                if (complete == 0) {
                    if ((p.field_0xb0 & 4) != 0) {
                        complete = 1;
                        AddToCompletionPoints(POINTS_PER_TRUEJEDI);
                        AddStatusStage(&p, 1, AddGoldBrickMessage(&p, tTRUEHERO));
                    } else if (p.collected_score > 0.0f)
                        AddStatusStage(&p, 2, 0);
                }
            } else if (Game.area_save[area].true_hero_complete[0] == 0 &&
                       Game.area_save[area].true_hero_complete[1] == 0) {
                if ((p.field_0xb0 & 4) != 0) {
                    Game.area_save[area].true_hero_complete[0] = Game.area_save[area].true_hero_complete[1] = 1;
                    AddToCompletionPoints(POINTS_PER_TRUEJEDI);
                    AddStatusStage(&p, 1, AddGoldBrickMessage(&p, tTRUEHERO));
                    if (p.episode_id != -1 && p.chapter != -1) {
                        sprintf(event, "truejedi_ep%i_ch%i_awarded", p.episode_id + 1, p.chapter + 1);
                        NuIOS_RecordFlurryEvent(event);
                    } else {
                        sprintf(event, "truejedi_%s_awarded", p.area->file);
                        NuIOS_RecordFlurryEvent(event);
                    }
                } else if (p.collected_score > 0.0f && (p.mode_flags & 8) == 0)
                    AddStatusStage(&p, 2, 0);
            }
        }
        if (p.collected_score > 0.0f)
            AddStatusStage(&p, 3, 0);
        if (p.minikit_max != 0 && (p.area->flags & 0x36) == 0x10 &&
            Game.area_save[area].minikit_complete == SAVE_INCOMPLETE && AreaGlobals.values.field_0x10 > 0) {
            p.new_minikits = AreaGlobals.values.field_0x10;
            i32 gold = 0;
            if (p.minikit_max <= Game.area_save[area].field_0x5[0]) {
                Game.area_save[area].minikit_complete = SAVE_COMPLETE;
                p.field_0xb0 |= 0x10;
                AddToCompletionPoints(POINTS_PER_MINIKIT);
                gold = AddGoldBrickMessage(&p, tMINIKIT);
            }
            if (from_save_and_exit == 0 || p.new_minikits != 0)
                AddStatusStage(&p, 4, gold);
            if ((p.field_0xb0 & 0x10) != 0 && AllMiniKitsDone(Game.area_save) != 0) {
                AddToCollection(id_SLAVE1);
                AddStatusStage(&p, 17, 0);
            }
        }
        if (Game.field_0x7c26[0] > p.previous_gold_bricks)
            AddStatusStage(&p, 19, 0);
        if ((p.mode_flags & 8) == 0)
            AddStatusStage(&p, 9, 0);
        AddStatusStage(&p, 10, 0);
        if (__builtin_expect(netclient == 0, 1))
            AddStatusStage(&p, 11, 0);
        AddStatusStage(&p, 12, 0);
        p.current_gold_brick = -1;
        p.stage_types[p.stage_count] = -1;
        p.next_stage = FindStatusStage(p.stage_types[0]);
        NextStatusStage(&p);
        collect_suit_bits = Game.initial_store_pack_flags;
        Game.initial_store_pack_flags = areaSuitBits;
    }
}

void EndOfDemo(i32) {
    NewLData = TITLES_LDATA;
    new_level_from_menu = 1;
    NewGame();
}

void DrawStatusIcons(STATUSPACKET_s *status, float y, float alpha) {
    const f32 x = ICONX;
    const f32 icon_alpha = (status->player_active[0] == 0 ? 0.25f : 1.0f) * alpha;
    const f32 size = ICONSIZE;
    DrawCharIcon(static_cast<i16>(status->player_model[0]), -x, y, 0.0f, size, 0xa5, icon_alpha, icon_alpha, 1, NULL);
}

f32 StatusIconsOnOff(f32 progress) {
    return (STATSPOSY - STATSPOS2Y) * NuTrigTable[(static_cast<i32>(progress * 16384.0f) >> 1) & 0x7fff] + STATSPOS2Y;
}

i32 UpdateNewMiniKits(STATUSPACKET_s *, STATUS_STAGE_s *stage) {
    if (stage->field_0x18 > stage->field_0x1c) {
        stage->field_0x18 = 0.0f;
        ++currentminikit;
        pop_timer = 0.5f;
        slideseek = 1.0f;
        return 1;
    }
    if ((stage->field_0x1c - jibberlen) - slidetime < stage->field_0x18 &&
        stage->field_0x18 <= stage->field_0x1c - jibberlen) {
        slideseek = ((stage->field_0x1c - stage->field_0x18) - jibberlen) / slidetime;
    }
    return 0;
}

void DrawStatusMiniKit(float x, float y, float z, float built_scale, float new_scale, i32 count, STATUSPACKET_s *packet,
                       float slide) {
    if (WORLD->minikit.gscn == NULL || WORLD->minikit.field_0x4 == NULL)
        return;

    HUBMINIKITPIECE_s *pieces = static_cast<HUBMINIKITPIECE_s *>(WORLD->minikit.field_0x4);
    NUVEC built_size = {built_scale, built_scale, built_scale};
    NUVEC new_size = {new_scale, new_scale, new_scale};
    NUVEC offset = v000;
    if (__builtin_expect(pieces[currentminikit].direction <= 5, 0)) {
        switch (pieces[currentminikit].direction) {
            case 0:
                offset.x += 0.035f * NuTrigTable[(static_cast<i32>(slide * 32768.0f) >> 1) & 0x7fff];
                break;
            case 1:
                offset.x -= 0.035f * NuTrigTable[(static_cast<i32>(slide * 32768.0f) >> 1) & 0x7fff];
                break;
            case 2:
                offset.y += 0.035f * NuTrigTable[(static_cast<i32>(slide * 32768.0f) >> 1) & 0x7fff];
                break;
            case 3:
                offset.y -= 0.035f * NuTrigTable[(static_cast<i32>(slide * 32768.0f) >> 1) & 0x7fff];
                break;
            case 4:
                offset.z -= 0.035f * NuTrigTable[(static_cast<i32>(slide * 32768.0f) >> 1) & 0x7fff];
                break;
            case 5:
                offset.z += 0.035f * NuTrigTable[(static_cast<i32>(slide * 32768.0f) >> 1) & 0x7fff];
                break;
        }
    }

    if (__builtin_expect(packet->minikit_max != 0 && WORLD->minikit.field_0x8 != 0 && count > 0, 1)) {
        i32 i = 0;
        do {
            HUBMINIKITPIECE_s &piece = pieces[i];
            NUMTX_ALIGNED16 matrix = piece.matrix;
            if (i >= currentminikit) {
                NuMtxScale(&matrix, &new_size);
                i32 piece_angle = 0x2000;
                if (i == currentminikit) {
                    const f32 phase = slideseek * 16384.0f + 49152.0f + 16384.0f;
                    piece_angle = (static_cast<i32>(phase) >> 1) & 0x7fff;
                }
                const f32 oscillation = NuTrigTable[piece_angle];
                const HUBMINIKITPIECE_s *animated_pieces = static_cast<HUBMINIKITPIECE_s *>(WORLD->minikit.field_0x4);
                const HUBMINIKITPIECE_s &animated_piece = animated_pieces[i];
                if (__builtin_expect(animated_piece.direction <= 5, 1)) {
                    switch (animated_piece.direction) {
                        case 0:
                            matrix.m30 -=
                                (static_cast<f32>(static_cast<i32>(animated_piece.direction_index)) * 0.025f + 0.25f) *
                                oscillation;
                            break;
                        case 1:
                            matrix.m30 +=
                                (static_cast<f32>(static_cast<i32>(animated_piece.direction_index)) * 0.025f + 0.25f) *
                                oscillation;
                            break;
                        case 2:
                            matrix.m31 -=
                                (static_cast<f32>(static_cast<i32>(animated_piece.direction_index)) * 0.025f + 0.25f) *
                                oscillation;
                            break;
                        case 3:
                            matrix.m31 +=
                                (static_cast<f32>(static_cast<i32>(animated_piece.direction_index)) * 0.025f + 0.25f) *
                                oscillation;
                            break;
                        case 4:
                            matrix.m32 +=
                                (static_cast<f32>(static_cast<i32>(animated_piece.direction_index)) * 0.025f + 0.25f) *
                                oscillation;
                            break;
                        case 5:
                            matrix.m32 -=
                                (static_cast<f32>(static_cast<i32>(animated_piece.direction_index)) * 0.025f + 0.25f) *
                                oscillation;
                            break;
                    }
                }
            } else {
                NuMtxScale(&matrix, &built_size);
            }
            if (i <= currentminikit) {
                matrix.m32 += offset.z;
                matrix.m31 += offset.y;
                matrix.m30 += offset.x;
            }

            NuMtxRotateYInline(&matrix,
                               static_cast<u16>(static_cast<i32>(NuFmod(GameTimer.time_elapsed, 2.0f) * 0.5f * 65536.0f)));
            const u16 z_phase =
                static_cast<u16>(static_cast<i32>(NuFmod(GameTimer.time_elapsed, 2.24f) / 2.24f * 65536.0f));
            NuMtxRotateZInline(&matrix, static_cast<u16>(static_cast<i32>(2730.0f * NU_SIN_LUT(z_phase))));
            const u16 x_phase =
                static_cast<u16>(static_cast<i32>(NuFmod(GameTimer.time_elapsed, 1.87f) / 1.87f * 65536.0f));
            NuMtxRotateXInline(&matrix, static_cast<u16>(static_cast<i32>(2730.0f * NU_SIN_LUT(x_phase) - 5461.0f)));

            matrix.m31 += y * PANEL3DMULY;
            matrix.m32 += z;
            matrix.m30 += x * PANEL3DMULX;

            if (i < 10) {
                KitPart[i].matrix = matrix;
                KitPart[i].special = &static_cast<HUBMINIKITPIECE_s *>(WORLD->minikit.field_0x4)[i].special;
                KitPart[i].enabled = 1;
            }
            if (WORLD->lev_objs[206].active != 0)
                DrawPanel3DObjectMtxNoAlpha(&static_cast<HUBMINIKITPIECE_s *>(WORLD->minikit.field_0x4)[i].special,
                                            &matrix);
            ++i;
            if (i >= packet->minikit_max || i >= WORLD->minikit.field_0x8 || i == count)
                break;
            pieces = static_cast<HUBMINIKITPIECE_s *>(WORLD->minikit.field_0x4);
        } while (true);
    }
}

void AddStatusMiniKitParts() {
    NUVEC target_position;
    target_position.x = 0.0f;
    target_position.y = STATSPOSY;
    target_position.z = 1.0f;
    for (i32 i = 0; i < 3; ++i) {
        KitPartRTL.intensity[i] = panelcol[i];
        KitPartRTL.direction[i] = paneldir[i];
    }
    KitPartRTL.ambient_colour = panelamb;

    f32 delay = 0.0f;
    for (i32 i = 0; i < 10; ++i) {
        if (KitPart[i].enabled == 0) {
            continue;
        }
        NUVEC velocity;
        velocity.x = 0.0f;
        velocity.y = (static_cast<f32>(qrand()) * (1.0f / 65535.0f)) * 0.25f + 0.25f;
        velocity.z = 0.0f;
        NuVecRotateZ(&velocity, &velocity, qrand());
        ADDPART_s part = Default_ADDPART;
        part.matrix = &KitPart[i].matrix;
        part.velocity = &velocity;
        part.flags = 0x480;
        part.gravity = 0.0f;
        part.special = KitPart[i].special;
        part.lighting = reinterpret_cast<PARTLIGHTSOURCE_s *>(&KitPartRTL);
        part.time_step = FRAMETIME;
        AddPart(&part);

        for (i32 coin = 0; coin < 5; ++coin) {
            NUVEC position;
            position.x = 0.0f;
            position.y = (static_cast<f32>(qrand()) * (1.0f / 65535.0f)) * 0.1f;
            position.z = 1.0f;
            NuVecRotateZ(&position, &position, qrand());
            ADDGAMEMSG_ALIGNED16 message = AddGameMsg_Default;
            message.position = &position;
            message.target_scale = 0.75f;
            message.player_index = qrand() / 0x8000;
            target_position.x = cointotal_x[message.player_index];
            message.target_position = &target_position;
            message.target_scale = COINTOTAL_COINSIZE;
            message.icon = static_cast<i16>(GizmoPickupType[2].first_model_id);
            message.special = &WORLD->lev_objs[message.icon].special;
            message.flags = 0x112d;
            message.duration = COINMSGTIME;
            message.score = GizmoPickupType[2].score;
            message.end_fn = EndScoreMessage;
            message.field_0x20 = delay;
            message.update_fn = GameMsg_DrawAdjustNewPos_CoinToTotal;
            message.field_0x4e = 1;
            AddGameMsg(&message);
            if (delay == 0.0f) {
                NewStatusRumbleBuzz(-1, 0.0f, 0.0f, 1);
                AddGameDebris(WORLD->debris_sys, 0x38, &position);
            }
            delay += 0.1f;
        }
    }
    SetPanelLights(1.0f);
}

void Status_DrawPromptMenu(STATUSPACKET_s *packet, i32 selected, float alpha) {
    i32 labels[2];
    labels[1] = -1;
    MENU_s *menu = &GameMenu[GameMenuLevel];
    bool locked = false;
    if ((packet->mode_flags & 1) != 0 || packet->challenge_state != 0) {
        labels[0] = tRESTART;
        labels[1] = tMAP;
    } else if (packet->mission_state != 0) {
        labels[0] = tRESTART;
        labels[1] = tMAP;
    } else if (__builtin_expect((packet->field_0xb0 & 0x40) != 0 ||
                                    (packet->area_id != -1 && ADataList[packet->area_id].episode_index == 0xff),
                                0)) {
        labels[0] = tMAP;
    } else {
        labels[0] = tFINISHSTORY;
        if ((packet->field_0xb0 & 8) != 0) {
            if (packet->next_area != -1 && (ADataList[packet->next_area].flags & 2) == 0 &&
                Game.area_save[packet->next_area].complete == 0) {
                labels[0] = tMAP;
                const i16 text =
                    ADataList[packet->next_area].area_index == 5 ? tFINALCHAPTERLOCKED : tNEXTCHAPTERLOCKED;
                SmartTextEx(TTab[text], 0.0f, 0.325f, 1.0f, 0.5f, 0.5f, 0.5f, 0, 255, 63, 0, 1.7f, 1, NULL, 0,
                            static_cast<i32>(alpha * 128.0f));
                locked = true;
            } else {
                if (__builtin_expect(packet->next_area == -1 || (ADataList[packet->next_area].flags & 2) == 0, 1)) {
                    labels[0] = tCONTINUESTORY;
                }
                labels[1] = tMAP;
            }
        } else if (packet->next_area == -1 || (ADataList[packet->next_area].flags & 2) == 0) {
            labels[0] = tCONTINUESTORY;
        }
    }
    const i32 count = labels[1] == -1 ? 1 : 2;
    f32 y = count == 1 ? 0.125f : 0.1f;
    if (!locked && count == 2 && packet->newly_completed != 0 && packet->area != NULL &&
        packet->area == NEGOTIATIONS_ADATA) {
        Hint_SetHintFromId(0x26c, 0, 1);
        Hint_SetComplete(0x26c);
        Hint_Draw(-1);
        y = 0.48999998f;
    }
    if ((packet->mode_flags & 1) != 0) {
        y += 0.075f;
    }
    for (i32 i = 0; i < count; ++i) {
        i32 red, green, blue;
        if (selected != 0 && status_prompt == i && TestForController()) {
            if (__builtin_expect(!(menu_pulsate <= 0.0f), 1)) {
                const f32 inverse = 1.0f - menu_pulsate;
                red = static_cast<u32>(MENUFLASH0R) * menu_pulsate + static_cast<u32>(MENUFLASH1R) * inverse;
                green = static_cast<u32>(MENUFLASH0G) * menu_pulsate + static_cast<u32>(MENUFLASH1G) * inverse;
                blue = static_cast<u32>(MENUFLASH0B) * menu_pulsate + static_cast<u32>(MENUFLASH1B) * inverse;
            } else if (__builtin_expect(menu_flash == 0, 1)) {
                red = MENUFLASH1R;
                blue = MENUFLASH1B;
                green = MENUFLASH1G;
            } else {
                red = MENUFLASH0R;
                blue = MENUFLASH0B;
                green = MENUFLASH0G;
            }
        } else if (menu_pulse > 0.0f) {
            const f32 inverse = 1.0f - menu_pulse;
            red = static_cast<u32>(MENUFLASH0R) * menu_pulse + static_cast<u32>(MENUNORMALR) * inverse;
            green = static_cast<u32>(MENUFLASH0G) * menu_pulse + static_cast<u32>(MENUNORMALG) * inverse;
            blue = static_cast<u32>(MENUFLASH0B) * menu_pulse + static_cast<u32>(MENUNORMALB) * inverse;
        } else {
            red = MENUENTRYR;
            green = MENUENTRYG;
            blue = MENUENTRYB;
        }
        smarttextex_drawmessagebox = 1;
        SmartTextEx(TTab[labels[i]], 0.0f, y, 1.0f, 0.7f, 0.7f, 0.7f, 0, red, green, blue, 1.7f, 1, NULL, 0,
                    static_cast<i32>(alpha * 128.0f) & 0xff);
        menu->item_x[i] = 0.0f;
        menu->item_y[i] = y;
        menu->item_width[i] = text3d_width;
        menu->item_height[i] = text3d_height;
        menu->item_column[i] = 0;
        menu->item_row[i] = i;
        y -= 0.2f;
    }
}

void DrawBonusTime(STATUSPACKET_s *packet, float position, i32 alpha) {
    const f32 y = -0.5f * position;
    char *title;
    i32 red, green;
    if (packet->new_best_time != 0.0f) {
        title = TTab[tNEWBESTTIME];
        red = 0;
        green = 255;
    } else {
        title = TTab[tNONEWBESTTIME];
        red = 255;
        green = 0;
    }
    SmartTextEx(title, 0.0f, 0.15f + y, 1.0f, 0.7f, 0.7f, 0.7f, 0, red, green, 0, 1.7f, 1, NULL, 0,
                static_cast<u8>(alpha));
    char time[256];
    char previous[256];
    Text_MakeTime(packet->new_best_time != 0.0f ? packet->new_best_time : packet->elapsed_time, 1, 1, 1, time);
    Text3DEx(time, 0.0f, 0.0f + y, 1.0f, 0.7f, 0.7f, 0.7f, 0, 255, 255, 255, static_cast<u8>(alpha));
    Text_MakeTime(packet->previous_best_time, 1, 1, 1, time);
    NuStrCpy(previous, "(");
    NuStrCat(previous, time);
    NuStrCat(previous, ")");
    Text3DEx(previous, 0.0f, y - 0.15f, 1.0f, 0.7f, 0.7f, 0.7f, 0, 255, 255, 255,
             static_cast<u8>(static_cast<u8>(alpha) >> 1));
}

void DrawStatusScreen(WORLDINFO_s *) {
    iconalphaoverride = -1.0f;
    memset(KitPart, 0, sizeof(KitPart));

    if (GAMEDEMO != 0) {
        if (GAMEDEMO != 1 || FadeSys.fade != 0.0f)
            return;
        char *exit_text = TTab[tEXIT];
        char *freeplay_text = TTab[tFREEPLAY];
        f32 y = -0.75f - 0.5f * MENUDY;
        {
            const i32 option = 0;
            char *text = freeplay_text;
            i32 red, green, blue;
            if (gamedemo_option == option && TestForController() != 0) {
                if (menu_pulsate > 0.0f) {
                    red = static_cast<i32>(static_cast<u32>(MENUFLASH0R) * menu_pulsate +
                                           static_cast<u32>(MENUFLASH1R) * (1.0f - menu_pulsate));
                    green = static_cast<i32>(static_cast<u32>(MENUFLASH0G) * menu_pulsate +
                                             static_cast<u32>(MENUFLASH1G) * (1.0f - menu_pulsate));
                    blue = static_cast<i32>(static_cast<u32>(MENUFLASH0B) * menu_pulsate +
                                            static_cast<u32>(MENUFLASH1B) * (1.0f - menu_pulsate));
                } else if (menu_flash != 0) {
                    green = MENUFLASH0G;
                    red = MENUFLASH0R;
                    blue = MENUFLASH0B;
                } else {
                    blue = MENUFLASH1B;
                    green = MENUFLASH1G;
                    red = MENUFLASH1R;
                }
            } else if (menu_pulse > 0.0f) {
                green = static_cast<i32>(static_cast<u32>(MENUFLASH0G) * menu_pulse +
                                         static_cast<u32>(MENUNORMALG) * (1.0f - menu_pulse));
                red = static_cast<i32>(static_cast<u32>(MENUFLASH0R) * menu_pulse +
                                       static_cast<u32>(MENUNORMALR) * (1.0f - menu_pulse));
                blue = static_cast<i32>(static_cast<u32>(MENUFLASH0B) * menu_pulse +
                                        static_cast<u32>(MENUNORMALB) * (1.0f - menu_pulse));
            } else {
                green = MENUENTRYG;
                red = MENUENTRYR;
                blue = MENUENTRYB;
            }
            Text3D(text, 0.0f, y, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0, red & 255, green & 255,
                   blue & 255);
        }
        y += MENUDY;
        {
            const i32 option = 1;
            char *text = exit_text;
            i32 red, green, blue;
            if (gamedemo_option == option && TestForController() != 0) {
                if (menu_pulsate > 0.0f) {
                    green = static_cast<i32>(static_cast<u32>(MENUFLASH0G) * menu_pulsate +
                                             static_cast<u32>(MENUFLASH1G) * (1.0f - menu_pulsate));
                    red = static_cast<i32>(static_cast<u32>(MENUFLASH0R) * menu_pulsate +
                                           static_cast<u32>(MENUFLASH1R) * (1.0f - menu_pulsate));
                    blue = static_cast<i32>(static_cast<u32>(MENUFLASH0B) * menu_pulsate +
                                            static_cast<u32>(MENUFLASH1B) * (1.0f - menu_pulsate));
                } else if (menu_flash != 0) {
                    red = MENUFLASH0R;
                    green = MENUFLASH0G;
                    blue = MENUFLASH0B;
                } else {
                    green = MENUFLASH1G;
                    red = MENUFLASH1R;
                    blue = MENUFLASH1B;
                }
            } else if (menu_pulse > 0.0f) {
                red = static_cast<i32>(static_cast<u32>(MENUFLASH0R) * menu_pulse +
                                       static_cast<u32>(MENUNORMALR) * (1.0f - menu_pulse));
                green = static_cast<i32>(static_cast<u32>(MENUFLASH0G) * menu_pulse +
                                         static_cast<u32>(MENUNORMALG) * (1.0f - menu_pulse));
                blue = static_cast<i32>(static_cast<u32>(MENUFLASH0B) * menu_pulse +
                                        static_cast<u32>(MENUNORMALB) * (1.0f - menu_pulse));
            } else {
                red = MENUENTRYR;
                green = MENUENTRYG;
                blue = MENUENTRYB;
            }
            Text3D(text, 0.0f, y, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0, red & 255, green & 255,
                   blue & 255);
        }
        return;
    }
    if (!(FadeSys.fade <= 0.0f)) {
        return;
    }

    STATUSPACKET_s *status = &StatusPacket;
    if (status->status_flags != 0) {

        if (status->draw_background_callback != NULL) {
            status->draw_background_callback(status);
        }

        i32 stage_index = 1;
        for (STATUS_STAGE_s *stage = StatusStages; stage->type != -1; stage = StatusStages + stage_index++) {
            if (stage->draw_callback != NULL) {
                if (stage == status->stage)
                    stage->draw_callback(stage, status, 1);
                else
                    stage->draw_callback(stage, status, 0);
            }
        }

        STATUS_STAGE_s *stage = status->stage;
        f32 alpha;
        if (stage->type == 11) {
            return;
        } else if (stage->type == 12) {
            alpha = 0.0f;
        } else if (stage->type == 10) {
            alpha = !(stage->field_0x18 >= 1.0f) ? 1.0f - stage->field_0x18 : 0.0f;
        } else {
            alpha = 1.0f;
            if (stage->type == 19 && stage->field_0x14 != 0) {
                const f32 time = stage->field_0x18;
                if (time < 1.0f) {
                    alpha = 1.0f - time;
                } else {
                    const f32 fade_start = stage->field_0x1c - 1.0f;
                    alpha = 0.0f;
                    if (time >= fade_start)
                        alpha = (time - fade_start) / (stage->field_0x1c - fade_start);
                }
            }
        }

        if (draw_player_icons != 0) {
            DrawStatusIcons(status, icon_y, iconalphaoverride >= 0.0f ? iconalphaoverride : alpha);
        }
    } else {
        char time[256];
        const f32 remaining = MAX(0.0f, 6.0f - GameTimer.time_elapsed);
        Text_MakeTime(remaining, 0, 0, 0, time);
        const f32 y_phase = NuFmod(GameTimer.time_elapsed, 0.5f);
        const f32 y = 0.01f * NuTrigTable[static_cast<u16>(static_cast<i32>((y_phase + y_phase) * 65536.0f)) >> 1];
        const f32 x_phase = NuFmod(GameTimer.time_elapsed, 0.432f);
        const f32 x = 0.01f * NuTrigTable[static_cast<u16>(static_cast<i32>(x_phase / 0.432f * 65536.0f)) >> 1];
        Text3D(time, x, y, 1.0f, 1.0f, 1.0f, 1.0f, 0, 255, 191, 0);
    }
}

void FinishStatusPacket(i32 choice) {
    LOG_INFO("level results choice=%d flags=%x challenge=%d mission=%d", choice, StatusPacket.mode_flags,
             StatusPacket.challenge_state, StatusPacket.mission_state);
    RememberPlayerIDs(1, static_cast<i16>(StatusPacket.player_model[0]), static_cast<i16>(StatusPacket.player_model[1]));
    if (StatusPacket.finish_callback(WORLD, &StatusPacket, choice) != 0) {
        return;
    }
    NewLData = HUB_LDATA;
    if (choice == 0 &&
        ((StatusPacket.mode_flags & 1) != 0 || StatusPacket.challenge_state != 0 || StatusPacket.mission_state != 0)) {
        NewLData = Area_FindNextPlayLevel(ADataList[StatusPacket.area->index].levels[0]);
        ResetBits = 0xffffffbf;
        reset_area = 1;
        FadeSys.SetFade(FADETYPE{FADE_TYPE_STILL_WIPE}, 0);
        NeedScreenGrab(1);
        GrabStillScreen();
        if (StatusPacket.challenge_state != 0) {
            InitChallenge(StatusPacket.area->index);
        }
        if (StatusPacket.mission_state != 0) {
            InitMission(MissionSys, static_cast<i8>(MissionSys->mission->count));
            NewLData = &LDataList[MissionSys->mission->level];
        }
    } else if (choice == 0 && StatusPacket.next_area != -1) {
        NewLData = &LDataList[ADataList[StatusPacket.next_area].levels[0]];
        if ((ADataList[StatusPacket.next_area].flags & 2) != 0) {
            grab_screen_image = 1;
        } else {
            loadareacharacters_no_backdrop_reset = 1;
            if ((StatusPacket.mode_flags & 4) != 0) {
                finishloop_backdroponly = 1;
            }
        }
    } else if ((StatusPacket.mode_flags & 4) != 0 ||
               ((StatusPacket.area->flags & 4) != 0 && (StatusPacket.field_0xb0 & 0x40) == 0)) {
        NewLData = CREDITS_LDATA;
    }
    if ((StatusPacket.mode_flags & 8) != 0) {
        NewLData = HUB_LDATA;
    }
    if (NewLData->area_index != -1 && (ADataList[NewLData->area_index].flags & 0x40) != 0 &&
        StatusPacket.mission_state != 0 && MissionSys->mission != NULL) {
        hub_from_mission = static_cast<i8>(MissionSys->mission->count);
    }
    if (NewLData == HUB_LDATA) {
        OldBonusScore[0] = 0;
        OldBonusScore[1] = 0;
    } else {
        for (i32 i = 0; i < 2; ++i) {
            OldBonusScore[i] = BonusScore[i];
        }
    }
    NuSound3StopRumble();
}

void UpdateStatusScreen(WORLDINFO_s *) {
    const f32 elapsed = FRAMETIME;
    CoinTotalScale = SeekLinearF(CoinTotalScale, 1.0f, FRAMETIME * 3.0f);
    DrawGoldBrick_Phase = -1;
    DrawGoldBrick_Stage = -1;
    if (GAMEDEMO != 0) {
        if (GAMEDEMO == 1) {
            if (FadeSys.fade != 0.0f || NewLData != NULL) {
                return;
            }
            for (i32 player = 0; player < 2; ++player) {
                if (__builtin_expect(StatusPacket.player_active[player] == 0, 1)) {
                    continue;
                }
                if ((GamePad[player].buttons_pressed & GAMEPAD_MENUSELECT) != 0) {
                    GameAudio_PlaySfx(0x30, NULL, 0, 0);
                    if (gamedemo_option == 0) {
                        GAMEDEMO = 2;
                        NewLData = HUB_LDATA;
                        new_level_from_menu = 1;
                    } else if (gamedemo_option == 1) {
                        EndOfDemo(0);
                    }
                    return;
                }
                if (((GamePad[player].left_directions | GamePad[player].buttons_pressed) & GAMEPAD_DUP) != 0) {
                    if (gamedemo_option > 0) {
                        --gamedemo_option;
                        GameAudio_PlaySfx(0x2f, NULL, 0, 0);
                    }
                    return;
                }
                if ((GamePad[player].left_directions & GAMEPAD_DDOWN) == 0) {
                    if ((GamePad[player].buttons_pressed & GAMEPAD_DDOWN) == 0) {
                        continue;
                    }
                }
                if (gamedemo_option < 1) {
                    ++gamedemo_option;
                    GameAudio_PlaySfx(0x2f, NULL, 0, 0);
                }
                return;
            }
        } else if (__builtin_expect(GAMEDEMO == 2, 1)) {
            if (GameTimer.time_elapsed >= 8.0f) {
                EndOfDemo(0);
            }
        }
        return;
    }
    if (StatusPacket.status_flags == 0) {
        return;
    }
    if ((StatusPacket.field_0xb0 & 2) == 0) {
        bool skip = MechInputTouchMenuController::AnyTouchesThisFrame > 0;
        if (skip) {
            MechInputTouchMenuController::AnyTouchesThisFrame = 0;
        } else {
            const u32 button = Text_Language != 0 ? GAMEPAD_JUMP : GAMEPAD_MENUSELECT;
            skip = (StatusPacket.player_active[0] != 0 && (GamePad[0].buttons_pressed & button) != 0) ||
                   (StatusPacket.player_active[1] != 0 && (GamePad[1].buttons_pressed & button) != 0);
        }
        if (skip) {
            if (__builtin_expect(StatusPacket.stage == NULL, 0)) {
                return;
            }
            if (__builtin_expect(StatusPacket.stage->skip_callback != NULL, 1)) {
                StatusPacket.stage->skip_callback(StatusPacket.stage, &StatusPacket);
            }
        }
    }
    if (StatusPacket.stage != NULL && StatusPacket.stage->update_callback != NULL) {
        StatusPacket.stage->update_callback(StatusPacket.stage, &StatusPacket, elapsed);
    }
    if (StatusPacket.status_flags == 0 && GameTimer.time_elapsed >= 5.0f) {
        FinishStatusPacket(0);
    }
}

i32 UpdateAchievements(STATUSPACKET_s *) {
    STUBBED();
    return 0;
}

void RegisterStatusScreen(STATUS_STAGE_s *stages, i32 *, REGISTERSTATUSPACKET_s *registration) {
    StatusStages = stages;
    StatusPacket.init_callback = registration->init_callback;
    StatusPacket.finish_callback = registration->finish_callback;
    StatusPacket.reset_callback = registration->reset_callback;
    StatusPacket.lsw_packet = registration->lsw_packet;
    StatusPacket.draw_background_callback = registration->draw_background_callback;
    StatusPacket.field_0x68 = registration->stage_delay;
}

