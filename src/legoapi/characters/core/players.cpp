#include "legoapi/world/world.h"
struct HINT_s;

#include <stdio.h>
#include <string.h>

#include "gameapi/edtools/edstubs.h"
#include "gameapi/gui/apimenu.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/world/area.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/level.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/characters/motion.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx.h"
#include "legoapi/core/input/timer.h"
#include "legogame/game.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numath/nutrig.h"

struct MechTouchUIElement;
struct PLAYERITEM_s;
struct PLAYERITEMTYPE_s;
struct PLAYERPACKET_s;
struct SOCKPOSITION_s;
struct TouchHolder;

void CheckForPlayersTurnedOff();

extern NUVEC plr_lastpos;

extern void GetTopBot(GameObject_s *obj);
extern void GameObjectDimensions(GameObject_s *obj);
extern void GameObjectOrigin(GameObject_s *obj);
extern void ResetRumble(RUMBLEPACKET *packet);
extern void ResetLights(NUVEC *position, rtldata_s *data, void *set);
extern void CurrentStart(GameObject_s *obj, i32 mode, i32 start);
extern void InitSurfaceInfo(GameObject_s *obj);
extern i32 SetObjOnSurface(GameObject_s *obj, i32 mode);
extern void GizForce_ResetLOS(GameObject_s *obj);
extern void PortalGameObject(GameObject_s *obj, i32 enable, i32 immediate, i16 portal, nugscn_s *scene);

void ResetPlayerAI(GameObject_s *obj);
void ResetPlayerMoves(GameObject_s *obj);
void SetProtocolDroidDeactivatedAction(GameObject_s *);
void NewBuzz(nupad_s *, f32, i32);
void GameAudio_PlaySfxById(i32 sfx_id, nuvec_s *position, i32 flags, i32 volume);
extern "C" f32 AnimDuration(i32, i32, f32, f32, i32);
extern "C" f32 chattersfxwait;

void Players_Init(void) {
    memset(Player, 0, sizeof(Player));
    GameObject_s **p = Player;

    if (netclient != 0) {
        return;
    }

    PLAYERCOUNT = 0;

    if ((WORLD->current_level->flags & 2) != 0) {
        i16 *list;
        i16 list0;

        if (FreePlay == 0 && (Hub_UsePlayerList == 0 || HUB_ADATA == NULL || HUB_ADATA != WORLD->area) &&
            (UsePlayerList != 1 ||
             (PlayerList[0] != -1 && (apicharsys->playermodelids[PlayerList[0]] == -1 ||
                                      (PlayerList[1] != -1 && apicharsys->playermodelids[PlayerList[1]] == -1))))) {
            list = Area_PlayerIDList;
            list0 = Area_PlayerIDList[0];
        } else {
            list = PlayerList;
            list0 = PlayerList[0];
        }

        if (LevelChangesInArea == 0 && UsePlayerList != 0 && PlayerProgress[0].active == 0 &&
            PlayerProgress[1].active != 0 && FreePlay == 0 && (WORLD->area->flags & 5) == 1 && list0 != -1 &&
            list0 == Area_StoryModelList[0].model_id && list[1] != -1 && list[1] == Area_StoryModelList[2].model_id &&
            list0 != list[1]) {
            list[0] = list[1];
            list[1] = list0;
        }

        i32 idx = 0;
        while (list[idx] != -1 && idx != 8) {
            GameObject_s *g = AddCreature(list[idx], 1);
            if (g != NULL) {
                i32 pi;
                u8 slot;
                GAMECHARACTERDATA *cd;

                PreResetCode(g);
                PostResetCode(g);

                pi = PLAYERCOUNT;
                g->oldpos = &OldPlrSPos[PLAYERCOUNT];
                p[PLAYERCOUNT] = g;
                p[pi]->batarang = &Batarang[pi];
                g->apiobj.field_0x27c = (char)pi;
                p[pi]->torpedo = GetTorpedoPacket();

                g->field_0x1050 |= 1;
                g->field_0x108e = 0;
                PLAYERCOUNT = PLAYERCOUNT + 1;
                g->hitpoints = DEFAULT_PLAYERHITPOINTS;

                if (UsePlayerList == 0) {
                    slot = g->apiobj.field_0x289;
                    *(u8 *)&g->apiobj.field_0x1f8 = (u8)((*(u8 *)&g->apiobj.field_0x1f8 & 0x7f) | ((slot == 0) << 7));
                    PlayerProgress[slot].hitpoints = g->current_hp;
                    g->field_0x106e = 0;
                } else {
                    char c = g->apiobj.field_0x27c;
                    *(u8 *)&g->apiobj.field_0x1f8 =
                        (u8)((*(u8 *)&g->apiobj.field_0x1f8 & 0x7f) | (PlayerProgress[c].active << 7));

                    if (Area == last_area) {
                        if (UsePlayerList == 1) {
                            g->field_0xe22 = (u8)((g->field_0xe22 & 0xfe) | (PlayerProgress[c].field_0x7 & 1));
                            SetHitPoints(g, PlayerProgress[c].hitpoints);
                            c = g->apiobj.field_0x27c;
                            g->field_0x108e = PlayerProgress[c].field_0xa;
                            g->field_0x106e = PlayerProgress[c].field_0x4;
                            g->field_0xdec = PlayerProgress[c].field_0xc;
                            g->suit = PlayerSuit[c];
                            if (g->torpedo != NULL) {
                                g->torpedo->count = PlayerTorpedoCount[c];
                            }
                        }
                    } else {
                        g->field_0xe22 &= 0xfe;
                        g->field_0x106e = 0;
                    }
                }

                cd = (GAMECHARACTERDATA *)apicharsys->char_data[g->id].field11_0x24;
                g->apiobj.viewdistance = cd->viewdistance;
                g->apiobj.heardistance = cd->heardistance;
                g->apiobj.maxviewheight = cd->maxviewheight;
                g->apiobj.minviewheight = cd->minviewheight;

                {
                    g->ai.nearest_opponent = NULL;
                    g->ai.field_0xdc = 0;
                    g->ai.opponent = NULL;
                    g->ai.field_0xec = 0;
                    g->ai.field_0xe0 = 0x4e6e6b28;
                    g->ai.field_0xf0 = 0x4e6e6b28;
                    g->apiobj.field387_0x2a0 = 0;
                    g->apiobj.field388_0x2a4 = 0;
                    g->field_0xebc = 0;
                    g->field_0xec0 = 0;
                    g->opponent = NULL;
                    g->last_attacker = NULL;
                    g->field_0xecc = 0;
                    g->field_0xed0 = 0;
                    g->ai.antinode_timer = 0.0f;
                    g->field_0xec4 = 0;
                    g->field_0xec8 = 0;
                    g->field_0xed8 = 0;
                    g->ai.field_0x1e5 &= 0xaf;
                    g->field_0xef9 &= 0xf7;
                    g->field_0xef8 &= 0xfe;
                    g->field_0xf00 |= 0x40;
                }
            }
            idx++;
            if (bonusmodearcade != 0 || (HUB_ADATA != NULL && HUB_ADATA == WORLD->area) || VehicleArea != 0)
                break;
        }
    }

    UsePlayerList = 0;

    if (p[0] != NULL) {
        COINPACKET *cp = p[0]->coinpacket = CoinPacket;
        cp->lastcoin = LEGOOBJ_DEFAULTLASTCOIN;
        u32 coins = PlayerProgress[0].coins;
        if (Area != last_area) {
            coins = 0;
        }
        cp->coins = coins;
        p[0]->gizforce_los_info = GizForceLOSInfo;
        memcpy(BackUpPlayers, p[0], 0x439 * 4);
    }

    if (p[1] != NULL) {
        COINPACKET *cp = p[1]->coinpacket = CoinPacket + 1;
        cp->lastcoin = LEGOOBJ_DEFAULTLASTCOIN;
        u32 coins = 0;
        if (Area == last_area) {
            coins = PlayerProgress[1].coins;
        }
        cp->coins = coins;
        p[1]->gizforce_los_info = &GizForceLOSInfo[1];
        memcpy(BackUpPlayers + 0x439, p[1], 0x439 * 4);
    }

    i32 id0 = (p[0] != NULL) ? p[0]->id : -1;
    i32 id1 = (p[1] != NULL) ? p[1]->id : -1;
    RememberPlayerIDs(0, id0, id1);
}

// --- Helpers moved from world.cpp ---

static char sMissionStartDoor[] = "MissionStartDoor";
static char sArcadeStartDoor[] = "ArcadeStartDoor";

static NUVEC HubVehiclesDoorPos[2] = {{-24.21f, 0.0f, -25.36f}, {-23.63f, 0.0f, -25.68f}};
static NUVEC HubMinikitDoorPos[2] = {{-27.14f, 0.0f, -24.92f}, {-26.77f, 0.0f, -24.94f}};

void Players_InitPositions(WORLDINFO *world) {
    i32 bonus = 0;
    if (world->area != NULL) {
        bonus = (i32)((world->area->flags >> 2) & 1);
    }

    for (i32 i = 0; i < 8; i++) {
        PlayerStart[i].pos = &v000;
        PlayerStart[i].angle = 0;
    }

    PORTALPOS *A = NULL;
    i32 ninit = 2;
    DOOR_s *start_door = world->start_door;
    if (start_door != NULL && bonus == 0) {
        A = reinterpret_cast<PORTALPOS *>(start_door->spline);
    } else if (LEGOSPL_START != -1) {
        A = world->portal_places[LEGOSPL_START];
        ninit = 0;
    }

    if (A != NULL) {
        f32 *ps = A->positions;
        NUVEC tmp;
        i32 n = ninit;
        for (i32 i = 0; i < 8; i++) {
            PlayerStart[i].pos = (NUVEC *)&ps[6 * n];
            NuVecSub(&tmp, (NUVEC *)&ps[6 * n + 3], (NUVEC *)&ps[6 * n]);
            PlayerStart[i].angle = NuAtan2D(tmp.x, tmp.z);
            if (2 * n + 4 > A->count)
                n = ninit;
            else
                n = n + 1;
        }
        if (bonus != 0 && A->count > 3) {
            i32 nc = A->count >> 2;
            i32 r = qrand() / (i32)(0xffff / nc + 1);
            PlayerStart[0].pos = (NUVEC *)&ps[12 * r];
            NuVecSub(&tmp, (NUVEC *)&ps[12 * r + 3], (NUVEC *)&ps[12 * r]);
            PlayerStart[0].angle = NuAtan2D(tmp.x, tmp.z);
            PlayerStart[1].pos = (NUVEC *)&ps[12 * r + 6];
            tmp.z = ps[12 * r + 9] - ps[12 * r + 6];
            tmp.x = ps[12 * r + 9] - PlayerStart[0].pos->x;
            PlayerStart[1].angle = NuAtan2D(tmp.x, tmp.z);
        }
    }

    // --- hub exit-door lookup ---
    if (HUB_ADATA != NULL && HUB_ADATA == world->area) {
        if (world->level_sub_id != last_area && hub_from_cutsceneplayer == 0) {
            if (hub_from_superstory != -1) {
                i32 area =
                    Episode_FindAreaFromFlags((EPISODEDATA *)((char *)EDataList + hub_from_superstory * 0x1c), 5, 5);
                void *door = Door_FindByIndex(world, area, -1, NULL);
                if (door != NULL) {
                    NuStrCpy(Door_ExitName, (char *)door);
                }
            } else if (hub_from_mission != -1) {
                void *door = Door_FindByName(world, sMissionStartDoor);
                if (door != NULL) {
                    NuStrCpy(Door_ExitName, (char *)door);
                }
            } else if (hub_from_arcade != -1) {
                void *door = Door_FindByName(world, sArcadeStartDoor);
                if (door != NULL) {
                    NuStrCpy(Door_ExitName, (char *)door);
                }
            } else if (hub_startoutsidebonusdoor_area != -1) {
                void *door = Door_FindByIndex(world, hub_startoutsidebonusdoor_area, -1, NULL);
                if (door != NULL) {
                    NuStrCpy(Door_ExitName, (char *)door);
                }
            } else if (last_area != -1) {
                i32 area = last_area;
                if (VEHICLES_ADATA != NULL && VEHICLES_ADATA->index == area) {
                    PlayerStart[0].pos = (NUVEC *)&HubVehiclesDoorPos[0];
                    PlayerStart[0].angle = 0x9555;
                    PlayerStart[1].pos = (NUVEC *)&HubVehiclesDoorPos[1];
                    PlayerStart[1].angle = 0x9555;
                } else {
                    AREADATA *area_data = &ADataList[area];
                    if ((area_data->flags & 5) == 5) {
                        PlayerStart[0].pos = (NUVEC *)&HubMinikitDoorPos[0];
                        PlayerStart[0].angle = 0x6e38;
                        PlayerStart[1].pos = (NUVEC *)&HubMinikitDoorPos[1];
                        PlayerStart[1].angle = 0x6e38;
                    } else {
                        if ((area_data->flags & 4) != 0 && area_data->episode_index != AREA_EPISODE_NONE) {
                            i32 episode_area =
                                (i32)Episode_FindAreaFromFlags(&EDataList[area_data->episode_index], 5, 5);
                            if (episode_area != -1) {
                                area = episode_area;
                            }
                        }
                        void *door = Door_FindByIndex(world, area, -1, NULL);
                        if (door != NULL) {
                            NuStrCpy(Door_ExitName, (char *)door);
                        }
                    }
                }
            }
        }
    }
    StartDoorPositions();

    Door_CutSpl = NULL;
    if (Door_ExitCameraSplineName[0] != 0) {
        NUGSPLINE *spl = NuSplineFind(world->current_gscn, Door_ExitCameraSplineName);
        Door_CutSpl = spl;
        if (spl == NULL) {
            Door_UseCutCam = 0;
        } else {
            Door_CutCamPos0 = spl->pts[0];
            Door_CutCamPos1 = spl->pts[1];
        }
    }
    Door_ExitCameraSplineName[0] = 0;

    for (i32 i = 0; i < 8; i++) {
        SOCKPOSITION sp;
        ComplexSockPosition(world->sock_sys, PlayerStart[i].pos, -1, -1, &sp);
        PlayerStart[i].sock_location = sp.location;
        PlayerStart[i].sock_ratio = sp.ratio;
    }
    HubStartDoor = NULL;

    if (HUB_ADATA != NULL && HUB_ADATA == world->area) {
        hub_from_superstory = -1;
        hub_from_mission = -1;
        hub_from_arcade = -1;
        if (hub_from_cutsceneplayer != 0) {
            void *av = CutScenePlayer_Available();
            if (av != NULL && static_cast<CUTSCENEPLAYER_s *>(CutScenePlayer_Available())->return_door != -1) {
                void *door = Door_FindByIndex(
                    world, -1, static_cast<CUTSCENEPLAYER_s *>(CutScenePlayer_Available())->return_door, NULL);
                HubStartDoor = door;
                if (door != NULL) {
                    f32 *vps = *(f32 **)((char *)(*(void **)((char *)door + 0xa0)) + 0x8);
                    NUVEC v2;
                    NuVecSub(&v2, (NUVEC *)&vps[15], (NUVEC *)&vps[12]);
                    PlayerStart[0].pos = (NUVEC *)&vps[12];
                    PlayerStart[0].angle = NuAtan2D(v2.x, v2.z);
                    NuVecSub(&v2, (NUVEC *)&vps[21], (NUVEC *)&vps[18]);
                    PlayerStart[1].pos = (NUVEC *)&vps[18];
                    PlayerStart[1].angle = NuAtan2D(v2.x, v2.z);
                } else {
                    shop_from_cutsceneplayer = hub_from_cutsceneplayer;
                }
            } else {
                shop_from_cutsceneplayer = hub_from_cutsceneplayer;
            }
        }
        hub_from_cutsceneplayer = 0;
    }
}

typedef struct {
    i32 field_0;
    char *name;
    u8 field_0x8;
    u8 field_0x9;
    u8 field_0xa;
    u8 field_0xb;
} PlayerItemTypeEntry;

static PlayerItemTypeEntry *PlayerItemType = NULL;
static i32 PLAYERITEMTYPECOUNT = 0;

extern i8 BoltType_FindIDByName(char *name, WORLDINFO *world);

void PlayerItemTypes_Reset(WORLDINFO *world) {
    if (PlayerItemType != 0 && PLAYERITEMTYPECOUNT > 0) {
        for (i32 i = 0; i < PLAYERITEMTYPECOUNT; i++) {
            PlayerItemType[i].field_0x9 = (u8)BoltType_FindIDByName(PlayerItemType[i].name, world);
        }
    }
}

GameObject_s *AddCreature(i32 id, i32 param) {
    if ((u32)id >= 0x154) {
        return NULL;
    }
    if (apicharsys->playermodelids[id] == -1) {
        return NULL;
    }
    GameObject_s *g = AddGameObject(id);
    if (g == NULL) {
        return NULL;
    }
    InitCreature(g, id, param);
    return g;
}

static f32 sPreResetMulA = 0.0f;
static f32 sPreResetMulB = 0.0f;
static f32 sPreResetSubC = 0.0f;
static f32 sPreResetD18Scale = 0.0f;
static f32 sPreReset1048Scale = 0.0f;
static f32 sPreResetDivF = 0.0f;
static f32 sPreResetMulG = 0.0f;

void PreResetCode(GameObject_s *obj) {
    u8 *b = (u8 *)obj;

    u8 t23 = (u8)(b[0xe23] & 0xf8);
    b[0xe20] &= 0xef;
    u8 t22 = (u8)(b[0xe22] & 0x3f);
    u8 t25 = (u8)(b[0xe25] & 0xaf);
    b[0xe24] &= 0xcf;
    b[0xf03] &= 0xdf;
    b[0xe3e] = 0xff;
    b[0xe3f] = 0xff;
    b[0xe3c] = 0x00;
    b[0xe3d] = 0xff;
    b[0xe25] = t25;
    b[0xe22] = t22;
    b[0xe23] = t23;

    DrawOffsetCode(obj, 0);

    if ((*(u32 *)&b[0xf00] & 8) != 0) {
        b[0xe22] &= 0xf7;
        *(f32 *)&b[0xc40] = (f32)qrand() * sPreResetMulA * sPreResetMulB - sPreResetSubC;
        *(f32 *)&b[0xc44] = (f32)qrand() * sPreResetMulA * sPreResetMulB - sPreResetSubC;
        b[0xe20] &= 0xfb;
        *(f32 *)&b[0xc48] = (f32)qrand() * sPreResetMulA * sPreResetMulB - sPreResetSubC;
        *(u32 *)&b[0xcf4] = 0;
        *(u32 *)&b[0xcec] = 0;
        *(u32 *)&b[0xcf0] = 0;
        *(u32 *)&b[0xca0] = 0;
        if (b[0x7a5] == 0xa) {
            b[0xe21] &= 0xf7;
        }
        b[0xe21] &= 0xdf;
        b[0xe33] = 0x01;
        b[0xe22] &= 0xfd;
        b[0xe46] = 0xff;
        b[0xe45] = 0xff;
        b[0xe44] = 0xff;
        b[0xe43] = 0xff;
        *(u32 *)&b[0xd0c] = 0;
        *(u16 *)&b[0x4a] = 0xffff;
        *(u32 *)&b[0xd7c] = 0;
        if ((obj->apiobj.character_data->model_flags & 0x8040) == 0) {
            b[0xe31] = 0;
        }

        {
            i32 e04 = *(i32 *)&b[0xe04];
            b[0xe23] &= 0x7f;
            if (e04 != -1) {
                rtlDynamicEnable(e04, 0);
            }
        }

        {
            u8 al = b[0x27d];
            b[0xe25] &= 0xfd;
            b[0xe24] &= 0xbf;
            b[0x1089] = 0;

            if (al != 0) {
                *(u32 *)&b[0xd18] = 0x3e4ccccd; /* 0.2f */
            } else if (*(f32 *)&b[0xd18] > 0.0f) {
                *(f32 *)&b[0xd18] -= FRAMETIME;
            }

            if ((i8)b[0x1f8] >= 0 || VehicleArea != 0) {
                goto finish_dfd;
            }

            if (b[0x7a5] == 0 &&
                sPreReset1048Scale * obj->apiobj.character_data->player_config->reset_scale > *(f32 *)&b[0x76c]) {
                goto finish_dfd;
            }

            {
                f32 f28v = obj->pad_gamepad->input_magnitude;
                if (f28v > 0.0f) {
                    if ((al & 2) != 0) {
                        goto finish_dfd;
                    }
                    if (b[0x1084] == 0) {
                        goto finish_dfd;
                    }
                } else {
                    if (*(f32 *)&b[0x68] == 0.0f && *(f32 *)&b[0x70] == 0.0f) {
                        goto finish_dfd;
                    }
                    if ((al & 2) != 0) {
                        goto finish_dfd;
                    }
                    if (b[0x1084] == 0) {
                        goto finish_dfd;
                    }
                }

                {
                    f32 t1 = *(f32 *)&b[0x6a8];
                    if (t1 <= NuTrigTable[0x4000]) {
                        goto finish_dfd;
                    }
                    f32 t2 = NuTrigTable[0x3000];
                    if (t2 <= t1) {
                        goto finish_dfd;
                    }
                    t1 = *(f32 *)&b[0x1e0] / sPreResetDivF;
                    f32 diff = *(f32 *)&b[0x17c] - *(f32 *)&b[0x218];
                    t1 *= sPreResetMulG;
                    if (diff <= t1) {
                        goto finish_dfd;
                    }
                    if (f28v <= 0.0f) {
                        NUVEC dir;
                        dir.x = *(f32 *)&b[0x68];
                        dir.y = 0.0f;
                        dir.z = *(f32 *)&b[0x70];
                        NuVecNorm(&dir, &dir);
                        if ((*(f32 *)&b[0x698] - *(f32 *)&b[0x5c]) * dir.x +
                                (*(f32 *)&b[0x6a0] - *(f32 *)&b[0x64]) * dir.z <=
                            0.0f) {
                            goto finish_dfd;
                        }
                    } else {
                        NUVEC dir;
                        NuVecRotateY(&dir, &v001, *(u16 *)&b[0x5a]);
                        if ((*(f32 *)&b[0x698] - *(f32 *)&b[0x5c]) * dir.x +
                                (*(f32 *)&b[0x6a0] - *(f32 *)&b[0x64]) * dir.z <=
                            0.0f) {
                            goto finish_dfd;
                        }
                    }
                    *(u32 *)&b[0x1048] = 0x3dcccccd; /* 0.1f */
                }
            }
            goto finish_e1e;

        finish_dfd:
            if (*(f32 *)&b[0x1048] > 0.0f) {
                *(f32 *)&b[0x1048] -= FRAMETIME;
            }
            goto finish_e1e;

        finish_e1e:
            GameObjectNearFloor(obj, 1.0f, (f32 *)&b[0xda0]);
            *(f32 *)&b[0xdb8] = GetHoverPosY(obj);
            {
                u8 v = obj->apiobj.character_data->player_config->variant;
                if (v != 0xff) {
                    if (b[0x27c] == 0xff) {
                        b[0xe42] = v;
                    } else {
                        if (Cheat_IsOn(0x19) != 0) {
                            b[0xe42] = 0x00;
                        } else if (b[0x27c] != 0xff && Player_HasPurpleForce(obj) != 0) {
                            b[0xe42] = 0x03;
                        } else {
                            b[0xe42] = obj->apiobj.character_data->player_config->variant;
                        }
                    }
                } else {
                    b[0xe42] = 0xff;
                }

                {
                    u32 mask = GAMEPAD_ACTION;
                    f32 xmm0 = *(f32 *)&b[0xde4];
                    if ((obj->pad_gamepad->buttons_held & mask) != 0) {
                        *(f32 *)&b[0xde4] = xmm0 + FRAMETIME;
                    } else {
                        if (xmm0 > 0.0f && sPreResetD18Scale > xmm0) {
                            b[0xe24] |= 0x40;
                        }
                        *(f32 *)&b[0xde4] = 0.0f;
                    }
                }
            }
            b[0xe25] &= 0x5f;
        }
    }

    return;
}

void PostResetCode(GameObject_s *obj) {
    u8 *p = (u8 *)obj;
    void *pp = *(void **)(p + 0x54);
    *(u8 *)(p + 0x1091) = 0;
    void *q = *(void **)((u8 *)pp + 0x24);
    i16 v = *(i16 *)((u8 *)q + 0xe8);
    if (v != -1) {
        u8 b = *(u8 *)((u8 *)q + 0x11e);
        f32 f = (f32)(u32)b;
        ChatterSfx(obj, v, f);
    }
    if (*(void **)(*(u8 **)(p + 0x54) + 0x18) != (void *)&Move_VEHICLE && *(u16 *)(p + 0xe1c) != 0 &&
        (*(u8 *)(p + 0xe25) & 0x20) == 0) {
        *(u16 *)(p + 0xe1c) = 0;
    }
}

static TORPEDOPACKET TorpedoPackets[16];

TORPEDOPACKET *GetTorpedoPacket(void) {
    for (i32 i = 0; i < 16; i++) {
        if ((TorpedoPackets[i].field_0x1 & 1) == 0) {
            TorpedoPackets[i].field_0x1 |= 1;
            return &TorpedoPackets[i];
        }
    }
    return NULL;
}

void SetHitPoints(GameObject_s *obj, i32 hp) {
    obj->current_hp = (u8)hp;
    if ((i8)hp > (i32)(u32)obj->hitpoints) {
        obj->current_hp = obj->hitpoints;
    }
}

void RememberPlayerIDs(i32 a, i32 b, i32 c) {
    if (VehicleArea != 0 || GAMEDEMO != 0) {
        return;
    }
    if (a == 0) {
        if ((WORLD->current_level->flags & 0x4e2) != 2) {
            return;
        }
    }
    if (b != -1 && (CDataList[b].model_flags & 0x2000) == 0 && PlayerID[0] != b && Collection_Got(b) == 1 &&
        GCDataList[b].field275_0x116 != 0 && PlayerID[1] != b) {
        PlayerID[0] = b;
    }
    if (c != -1 && (CDataList[c].model_flags & 0x2000) == 0 && PlayerID[1] != c && Collection_Got(c) == 1 &&
        GCDataList[c].field275_0x116 != 0 && PlayerID[0] != c) {
        PlayerID[1] = c;
    }
    if (b != c && PlayerID[1] == b && PlayerID[0] == c) {
        PlayerID[0] = b;
        PlayerID[1] = c;
    }
}

// ---- Player start spawn entries ----
PLAYERSTARTENTRY PlayerStart[8];

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

void *CutScenePlayer_Available(void) {
    return CutScenePlayer;
}

void ChatterSfx(GameObject_s *g, i32 a, float b) {
    if (chattersfxwait <= 0.0f && ParticlesPerSecond(2.0f, FRAMETIME) > 0 && g->apiobj.field_0x287 == 0 &&
        g->character_context == -1) {
        if (a != last_chatter_sfx || b <= 0.0f) {
            GameAudio_PlaySfxById(a, &g->apiobj.collision_position, 0, 0);
            const i32 random = qrand();
            last_chatter_sfx = a;
            chattersfxwait = static_cast<f32>(random) * (1.0f / 65535.0f) * 2.0f + 3.0f;
        } else {
            chattersfxwait = b;
            last_chatter_sfx = -1;
        }
    }
}

void Move_VEHICLE(GameObject_s *g) {
    (void)g;
}

void DrawOffsetCode(GameObject_s *obj, i32 param) {
    (void)obj;
    (void)param;
}

i32 GameObjectNearFloor(GameObject_s *obj, f32 h, f32 *out) {
    // Target 0x46e7b0..0x46e862. GameShadow uses a large positive
    // sentinel when it does not find terrain, rather than -1.
    const f32 no_floor_height = 2000000.0f;
    const f32 floor_height = obj->apiobj.field_0x218;
    if (floor_height == no_floor_height) {
        if (out != NULL) {
            *out = no_floor_height;
        }
        return 0;
    }

    i32 height_steps = static_cast<i32>(h);
    if (height_steps < 0) {
        height_steps = 0;
    }
    f32 tolerance = static_cast<f32>(height_steps) * 0.025f;
    const f32 radius_tolerance = obj->apiobj.collision_radius / 0.225f * tolerance;
    if (radius_tolerance > tolerance) {
        tolerance = radius_tolerance;
    }

    const f32 floor_distance = obj->apiobj.collision_min.y - floor_height;
    if (out != NULL) {
        *out = floor_distance;
    }
    return tolerance > floor_distance;
}

float GetHoverPosY(GameObject_s *obj) {
    (void)obj;
    return 0.0f;
}

i32 Player_HasPurpleForce(GameObject_s *obj) {
    if (Cheat_IsOn(0x1c))
        return 1;
    return obj != NULL && obj->field_0xdec > 0.0f;
}

void PlayerTakeHit(GameObject_s *, GameObject_s *) {
}

void PlayerItem_Set(PLAYERITEM_s *, PLAYERITEMTYPE_s *) {
}

void Player_FindByID(i32) {
}

NUVEC *Player_StartPos(GameObject_s *obj) {
    i32 index = obj->apiobj.field_0x27c;
    if (index < 0 || index > 7) {
        index = obj->apiobj.field_0x289;
    }
    index &= 7;
    return PlayerStart[index].pos != NULL ? PlayerStart[index].pos : PlayerStart[0].pos;
}

i32 PlayersDropInOut() {
    CheckForPlayersTurnedOff();
    return 0;
}

i32 PlayerItem_GotAmmo(PLAYERITEM_s *item) {
    if (item != NULL && item->type != NULL && item->type[8] == 2)
        return item->ammunition != 0;
    return 1;
}

i32 Players_AveragePos(nuvec_s *, SOCKPOSITION_s *) {
    return 0;
}

i32 Players_BothActive() {
    return Player[0] != NULL && static_cast<i8>(Player[0]->apiobj.field_0x1f8) < 0 && Player[1] != NULL &&
           static_cast<i8>(Player[1]->apiobj.field_0x1f8) < 0;
}

void PlayerItemType_Find(i32) {
}

void (*Player_ClearContextFn)(GameObject_s *, i32);
void Whip_Release(GameObject_s *);
void SuperCarry_Release(GameObject_s *);
void SpecialMove_ReleaseVictim(GameObject_s *);

void Player_ClearContext(GameObject_s *object, i32 mode) {
    if (Player_ClearContextFn != NULL)
        Player_ClearContextFn(object, mode);
    Whip_Release(object);
    SuperCarry_Release(object);
    SpecialMove_ReleaseVictim(object);
    object->movement_runtime_flags &= ~0x0c;
}

i32 Player_HasFastBuild(GameObject_s *player) {
    return Cheats_CheckFlags(0x4000) != 0 || (player != NULL && player->field_0xdec > 0.0f);
}

void PlayerItemTypes_Init(PLAYERITEMTYPE_s *) {
}

void Player_ResetContexts(PLAYERPACKET_s *packet) {
    f32 context_blend = 0.0f;

    packet->context_animation_time = 0.0f;
    packet->context_target = -1;
    if ((packet->animation_flags & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) != 0) {
        context_blend = 1.0f;
    }

    packet->context_distance = 5.0f;
    packet->field_0x670 = 0;
    packet->field_0x678 = 0;
    packet->field_0x668 = 0;
    packet->field_0x77e = 0;
    packet->context_blend = context_blend;
    packet->build_context = -1;
    packet->action_movement_variant = 0;
    packet->action_movement_state = 0;
    packet->gamepad->allocated_5a &= static_cast<u8>(~(0x04 | 0x10));
    packet->input_state = 0;
    packet->secondary_flags &= static_cast<u8>(~0x04);
    packet->field_0x648 = 0;

    const i32 random_context = qrand();
    packet->field_0x6a0 = 0;
    packet->field_0x6a4 = 0;
    packet->field_0x674 = 0;
    packet->field_0x6e0 = 0;
    packet->field_0x6f8 = 0;
    packet->field_0x710 = 0;
    packet->field_0x724 = 0;
    packet->field_0x730 = 0;
    packet->field_0x734 = 0;
    packet->context_animation = 0;
    packet->movement_angle_0 = -1;
    packet->context_mode = 0;
    packet->movement_angle_2 = -1;
    packet->random_context = static_cast<u8>(random_context / 0x8000);
    packet->movement_angle_3 = -1;
    packet->field_0x77d = 0;
    packet->linked_object = NULL;
}

void Player_CopyEssentials(GameObject_s *source, GameObject_s *destination) {
    destination->pad_gamepad = source->pad_gamepad;
    destination->coinpacket = source->coinpacket;
    destination->gizforce_los_info = source->gizforce_los_info;
    destination->batarang = source->batarang;
    destination->torpedo = source->torpedo;
    destination->oldpos = source->oldpos;
    destination->hitpoints = source->hitpoints;
    destination->field_0xeb4 = source->field_0xeb4;
    destination->apiobj.field_0x1f4 = source->apiobj.field_0x1f4;
}

i32 Player_HasDeflectBolts(GameObject_s *object) {
    if (Cheats_CheckFlags(0x80000) != 0 || (object != NULL && object->field_0xdec > 0.0f))
        return 1;
    return 0;
}

i32 FULLDEBUGTOGGLE;
i32 LIFTPLAYER;
f32 TOGGLEREPEATTIME = 0.25f;
i32 (*Player_ToggleSubCharacterFn)(GameObject_s *, i32, i32);
void (*Player_ToggledCharacterFn)(GameObject_s *, i32);
extern FadeSystem FadeSys;
extern i32 LEGOHINT_FREEPLAYTOGGLE;
void Hint_SetComplete(i32 hint_id);
void Move_DEFAULT(GameObject_s *object);
void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);
void GameAudio_PlaySfx(i32 sfx, NUVEC *position, i32 flags, i32 volume);

void Player_ToggleCharacter(GameObject_s *object, i32 direction, i32 sound) {
    f32 hold_time = object->input_toggle_hold_time;
    WORLDINFO *world = WorldInfo_CurrentlyActive();
    FULLDEBUGTOGGLE = 0;
    if ((HUB_ADATA != NULL && HUB_ADATA == world->area) || FadeSys.fade != 0.0f ||
        (CInfo[object->character_context].flags & 0x100) != 0 || object->field_0xcc0 != NULL || FreePlay == 0) {
        return;
    }
    if ((MiniCutCam != 0 && (object->apiobj.field_0x1f8 & 0x180) == 0x80) || object->apiobj.field_0x287 != 0 ||
        object->pad_gamepad == ViewCamGetGamePad()) {
        object->input_toggle_hold_time = TOGGLEHOLDTIME;
        return;
    }
    i16 old_id = object->id;
    i32 left;
    i32 right;
    if (direction != 0) {
        left = direction <= 0;
        right = direction > 0;
    } else {
        u32 left_mask = GAMEPAD_TOGGLELEFT;
        u32 right_mask = GAMEPAD_TOGGLERIGHT;
        if (LIFTPLAYER != 0) {
            left_mask &= ~GAMEPAD_LIFT;
            right_mask &= ~GAMEPAD_LIFT;
        }
        GAMEPAD_s *pad = object->pad_gamepad;
        left = pad->buttons_pressed & left_mask;
        right = pad->buttons_pressed & right_mask;
        if ((left != 0 && right != 0) || (left | right) == 0) {
            if ((object->apiobj.flags_low & 0x80) == 0) {
                return;
            }
            left = pad->buttons_held & left_mask;
            right = pad->buttons_held & right_mask;
            if ((left != 0 && right != 0) || (left | right) == 0) {
                object->input_toggle_hold_time = TOGGLEHOLDTIME;
                return;
            }
            object->input_toggle_hold_time -= FRAMETIME;
            if (!(object->input_toggle_hold_time <= 0.0f)) {
                return;
            }
            object->input_toggle_hold_time = TOGGLEREPEATTIME;
        }
        hold_time = object->input_toggle_hold_time;
    }
    if (Player_ToggleSubCharacterFn != NULL && Player_ToggleSubCharacterFn(object, left, sound) != 0) {
        return;
    }
    i32 index;
    for (index = 0; index < apicharsys->loaded_model_count; ++index) {
        if (apicharsys->models[index].model_id == object->id) {
            break;
        }
    }
    if (index == apicharsys->loaded_model_count) {
        return;
    }
    i32 attempts = 0;
    for (;;) {
        if (left != 0) {
            if (--index == -1) {
                index = apicharsys->loaded_model_count - 1;
            }
        } else if (++index == apicharsys->loaded_model_count) {
            index = 0;
        }
        i32 id = apicharsys->models[index].model_id;
        ++attempts;
        i32 collected = InCollectList_Index(id, NULL, 0);
        if ((apicharsys->models[index].flags & 1) == 0) {
            goto next_character;
        }
        if (collected == -1 && (i32)GCDataList[id].flags_090 >= 0 && (GCDataList[id].flags_094[3] & 1) == 0 &&
            !(VehicleArea != 0 && BonusArea != 0 && Cheats_CheckFlags(0x100) != 0 &&
              (GCDataList[id].flags_094[3] & 2) != 0)) {
            goto next_character;
        }
        if (VehicleArea != 0) {
            if ((CDataList[id].model_flags & 0x2000) == 0 &&
                !(BonusArea != 0 && (CDataList[id].model_flags & 0x4000000) != 0) &&
                (i32)GCDataList[id].flags_090 >= 0 &&
                !(BonusArea != 0 && Cheats_CheckFlags(0x100) != 0 && (GCDataList[id].flags_094[3] & 2) != 0)) {
                goto next_character;
            }
            if (BonusArea != 0) {
                i32 area = AreaFromMiniKitID(id);
                if (area != -1) {
                    if (Game_AreaSave == NULL || Game_AreaSave[area].minikit_complete == 0) {
                        goto next_character;
                    }
                } else if ((i32)GCDataList[id].flags_090 >= 0 && !(BonusArea != 0 && Cheats_CheckFlags(0x100) != 0 &&
                                                                   (GCDataList[id].flags_094[3] & 2) != 0)) {
                    goto next_character;
                }
                if (object->apiobj.field_0x27f <= 16 && (TerLayer[(i8)object->apiobj.field_0x27f].flags & 1) != 0 &&
                    GCDataList[id].field_0x28 <= 0.0f) {
                    goto next_character;
                }
            }
        } else if ((CDataList[id].model_flags & 0x2000) != 0 || (GCDataList[id].flags_090 & 0x40) != 0) {
            goto next_character;
        }
        if (BonusArea == 0 || VehicleArea == 0) {
            if ((i32)GCDataList[id].flags_090 < 0) {
                if ((object->apiobj.flags_low & 0x80) == 0 || Cheats_CheckFlags(0x100) == 0) {
                    goto next_character;
                }
            } else if ((GCDataList[id].flags_094[3] & 1) == 0 && (collected == -1 || Collection_Got(id) == 0)) {
                goto next_character;
            }
        }
        if (object->apiobj.field_0x218 != 2000000.0f && object->apiobj.field_0x220 != 2000000.0f &&
            object->apiobj.character_data->move_fn != Move_DEFAULT &&
            CDataList[id].bounds_max_y - CDataList[id].bounds_min_y >=
                object->apiobj.field_0x220 - object->apiobj.field_0x218) {
            GameAudio_PlaySfx(0x32, &object->apiobj.collision_position, 0, 0);
            goto next_character;
        }
        if (attempts >= apicharsys->loaded_model_count) {
            return;
        }
        if ((object->apiobj.flags_low & 0x80) != 0) {
            Hint_SetComplete(LEGOHINT_FREEPLAYTOGGLE);
            if (VehicleArea == 0) {
                GameCam_Blend(GameCam, 1.0f, 0.0f, 1);
            }
        }
        NewPlayerCharacter(object, id, old_id, 1);
        RememberPlayerIDs(0, Player[0] != NULL ? Player[0]->id : -1, Player[1] != NULL ? Player[1]->id : -1);
        if (Player_ToggledCharacterFn != NULL) {
            Player_ToggledCharacterFn(object, left);
        }
        object->input_toggle_hold_time = hold_time;
        if (sound != 0) {
            if (left != 0) {
                GameAudio_PlaySfx(0x23, &object->apiobj.collision_position, 0, 0);
            } else if (right != 0) {
                GameAudio_PlaySfx(0x24, &object->apiobj.collision_position, 0, 0);
            }
        }
        return;
    next_character:
        if (attempts > apicharsys->loaded_model_count || id == object->id) {
            return;
        }
    }
}

i32 Player_HasInvincibility(GameObject_s *object) {
    if (Cheats_CheckFlags(0x80) != 0 || (object != NULL && object->field_0xdec > 0.0f))
        return 1;
    return 0;
}

i32 Player_HasDoubleBoltDamage(GameObject_s *object) {
    if (Cheats_CheckFlags(2) != 0 || (object != NULL && object->field_0xdec > 0.0f))
        return 1;
    return 0;
}

void PlayerButton_OnHold_Callback(MechTouchUIElement &, TouchHolder &) {
}

i32 Player_HasDoubleWeaponDamage(GameObject_s *object) {
    if (Cheats_CheckFlags(0x400) == 0 && (object == NULL || object->field_0xdec <= 0.0f)) {
        return 0;
    }
    return 1;
}

void PlayerButton_OnLeave_Callback(MechTouchUIElement &, TouchHolder &) {
}

i32 Player_HasDoubleBoltDamage_FromBolt(BOLT_s *bolt) {
    i32 player;
    if (bolt->flags & 1)
        player = 0;
    else if (bolt->flags & 2)
        player = 1;
    else
        return 0;
    return Player_HasDoubleBoltDamage(Player[player]);
}

void PlayerButton_OnClick_Callback_NextButton(MechTouchUIElement &, TouchHolder &) {
}

static __used__ i32 SelectOpponent(GameObject_s *, f32, f32, i32, i32) {
    return 0;
}

static __used__ void Player_ClearContext_Game(GameObject_s *, i32) {
}

static __used__ unsigned int CanStartHold_Game(GameObject_s *) {
    return {};
}

static __used__ unsigned int CanPushBlocks_Game(GameObject_s *) {
    return {};
}

static __used__ unsigned int CanPushObstacles_Game(GameObject_s *) {
    return {};
}

void KillPlayer(GameObject_s *, i32, i32, nuvec_s *) {
}

namespace {

    constexpr f32 kInvalidSurfaceHeight = 2000000.0f;

    void SyncPlayerSpawnPosition(GameObject_s *obj) {
        const NUVEC &position = obj->apiobj.position;

        obj->apiobj.pos_x = position.x;
        obj->apiobj.pos_y = position.y;
        obj->apiobj.pos_z = position.z;

        obj->apiobj.start_position = position;
        obj->apiobj.initial_position = position;

        plr_lastpos = position;
    }

} // namespace

void ResetPlayer(GameObject_s *obj, i32 reset_moves, nuvec_s *position, i32 snap_to_surface) {
    if (obj == NULL) {
        return;
    }

    const bool restore_hitpoints = obj->apiobj.field_0x27c != -1 && obj->field_0x7a5 == 0x2b;

    if (reset_moves != 0) {
        if (position == NULL) {
            position = Player_StartPos(obj);
        }
        if (position != NULL) {
            obj->apiobj.position = *position;
        }

        if (obj->apiobj.field_0x27c != -1) {
            obj->field_0xf00 |= 0x40;
        }

        ResetPlayerMoves(obj);
        SyncPlayerSpawnPosition(obj);

        obj->apiobj.velocity = v000;
        obj->reset_velocity = v000;

        GetTopBot(obj);
        GameObjectDimensions(obj);
        obj->field_0xe23 &= static_cast<u8>(~8u);
        obj->use_model_origin = 0;
        obj->apiobj.field_0x288 = 0;
        GameObjectOrigin(obj);

        ResetRumble(&obj->pad_gamepad->rumble_packet);
        ResetLights(&obj->apiobj.position, &obj->light_data, WORLD->rtl_set);
        ResetMiniAnimPacket(&obj->mini_animation, -1);

        obj->sock_position.location.sock = -1;
        obj->sock_position.location.segment = -1;
        if (WORLD->sock_sys != NULL) {
            ComplexSockPosition(WORLD->sock_sys, &obj->apiobj.position, -1, -1, &obj->sock_position);
            ComplexSockAngles(&obj->sock_angles);
        }

        u8 player_index = static_cast<u8>(obj->apiobj.field_0x27c);
        if (player_index > 7) {
            player_index = obj->apiobj.field_0x289;
        }
        player_index &= 7;

        const u16 facing = static_cast<u16>(PlayerStart[player_index].angle);
        obj->apiobj.pitch_angle = 0;
        obj->apiobj.roll_angle = 0;
        obj->apiobj.facing_angle = facing;
        obj->apiobj.movement_facing_angle = facing;
        obj->apiobj.field_0x276 = facing;
        NuVecRotateY(&obj->facing_direction, &v001, facing);

        obj->field_0xc34 = 0x3f800000;
        obj->field_0xc38 = 0.0f;
        CurrentStart(obj, 0, 1);
        obj->field_0xe23 &= static_cast<u8>(~8u);
        obj->field_0xefe &= static_cast<u8>(~4u);
        obj->field_0xeff &= static_cast<u8>(~2u);
        obj->apiobj.model_draw_result = 1;
        obj->use_model_origin = 0;
        obj->apiobj.field_0x288 = 0;
        obj->field_0x1084 = 0;

        InitSurfaceInfo(obj);
        f32 surface_y = GetHoverPosY(obj);
        if (snap_to_surface == 0 || (obj->apiobj.character_data->model_flags & 0x2000) != 0) {
            if (surface_y != kInvalidSurfaceHeight) {
                obj->apiobj.position.y = surface_y;
                SyncPlayerSpawnPosition(obj);
            }
        } else {
            SetObjOnSurface(obj, 0);
        }

        GizForce_ResetLOS(obj);
        PortalGameObject(obj, 1, 1, -1, WORLD->current_gscn);

        if ((ResetBits & 2) != 0 || obj->apiobj.field_0x287 != 0 || restore_hitpoints) {
            SetHitPoints(obj, DEFAULT_PLAYERHITPOINTS);
            obj->field_0xe38 = 4;
        }
        if ((ResetBits & 8) != 0) {
            const i32 progress_index = static_cast<i8>(obj->apiobj.field_0x27c);
            if (progress_index >= 0 && progress_index < 8) {
                if (PlayerProgress[progress_index].field_0xb != 0) {
                    obj->field_0xe31 = 1;
                }
                obj->field_0xe38 = PlayerProgress[progress_index].field_0x9;
            }
        }

        ResetPlayerAI(obj);
        obj->apiobj.previous_position[0] = obj->apiobj.position.x;
        obj->apiobj.previous_position[1] = obj->apiobj.position.y;
        obj->apiobj.previous_position[2] = obj->apiobj.position.z;
        obj->field_0x10c8 = obj->apiobj.position.x;
        obj->field_0x10cc = obj->apiobj.position.y;
        obj->field_0x10d0 = obj->apiobj.position.z;
        obj->field_0xdc8 = 0.0f;
    } else {
        obj->field_0xdc8 = 0.0f;
    }

    obj->field_0x1004 = 1.0f;
    obj->field_0x101c = 0.0f;
    obj->field_0xda8 = 0.0f;
    obj->field_0xd78 = 1.0f;
    obj->apiobj.field_0x214 = kInvalidSurfaceHeight;
    obj->field_0xdbc = 0.0f;
    obj->field_0xf1c = 0.0f;
    obj->field_0xc54 = -1.0f;
    obj->field_0xde0 = 0.0f;
    obj->apiobj.field_0x287 = 0;
    obj->field_0xe36 = 0;
    obj->apiobj.field_0x285 = 0;
    obj->apiobj.field_0x1f8 &= 0xfa83;
    obj->apiobj.field_0x1fa &= static_cast<u8>(~2u);
    obj->apiobj.field_0x1f4 &= ~0x100u;
    obj->field_0xefc |= 0x80;
    obj->field_0xe21 |= 0x80;
}

void StarWars_AutoSetAICapabilities(GameObject_s *object);
i32 CanPullLevers(i32 id);
extern f32 DEFAULT_MOVE_RANGE;

void InitPlayerAI(GameObject_s *object) {
    StarWars_AutoSetAICapabilities(object);
    u8 *b = reinterpret_cast<u8 *>(object);
    i32 can_pull_levers = CanPullLevers(object->id);
    b[0xefe] = (b[0xefe] & 0x7f) | (can_pull_levers << 7);
    b[0x4a6] &= 0xfe;
    object->ai.character_type_mask_low = 0;
    object->ai.character_type_mask_high = 0;
    if (FreePlay && !(object->apiobj.field_0x1f4 & 0x400)) {
        object->ai.character_type_mask_low = ~u32(0);
        object->ai.character_type_mask_high = ~u32(0);
    } else if (SpecialRouteCharacterTypeIDFn) {
        u8 *row = *reinterpret_cast<u8 **>(b + 0xcac);
        char *name = row ? *reinterpret_cast<char **>(row + 4) : object->apiobj.character_data->file;
        u8 type = SpecialRouteCharacterTypeIDFn(name);
        if (type != 0xff) {
            u64 mask = type <= 63 ? u64(1) << type : ~u64(0);
            object->ai.character_type_mask_low = static_cast<u32>(mask);
            object->ai.character_type_mask_high = static_cast<u32>(mask >> 32);
        }
    }
    b[0x370] = 0;
    b[0xef8] = ((b[0xef8] | 0xa) & 0x2a) | ((object->apiobj.character_data->model_flags >> 5) & 4);
    b[0xef9] &= 0xf4;
    b[0xefa] = (b[0xefa] & 0xc3) | ((object->apiobj.character_data->model_flags >> 23) & 0x10);
    b[0xefb] &= 0x20;
    b[0xefc] &= 0xc0;
    b[0xefd] &= 0xe6;
    b[0xeff] &= 0xc6;
    b[0xf00] &= 0x58;
    b[0xf01] &= 0xe9;
    b[0xf02] = (b[0xf02] & 0x53) | ((object->apiobj.field_0x1f4 & 0x400) ? 0 : 4);
    b[0xf03] &= 0x30;
    b[0xf04] &= 0xfc;
    object->run_speed_override = 1000000000.0f;
    object->walk_speed_override = 1000000000.0f;
    object->hover_height_override = 1000000000.0f;
    // The original scale-override sentinel; zero would hide the character.
    object->field_0x1038 = 1000000000.0f;
    *reinterpret_cast<f32 *>(b + 0x4ac) = DEFAULT_MOVE_RANGE;
    b[0x4a7] = (b[0x4a7] & 0xe3) | ((DEFAULT_MOVE_RANGE > 0.0f) << 2);
    b[0x7b5] &= 0xfd;
    b[0x4a5] &= 0xaf;
    // Scalar stores follow the original AI state layout.
    *reinterpret_cast<f32 *>(b + 0xed4) = 1.0f;
    *reinterpret_cast<f32 *>(b + 0x3a0) = 1000000000.0f;
    *reinterpret_cast<f32 *>(b + 0x3b0) = 1000000000.0f;
    object->doomed_escape_locator = NULL;
    *reinterpret_cast<u32 *>(b + 0x1058) = 0;
    *reinterpret_cast<u32 *>(b + 0xecc) = 0;
    *reinterpret_cast<u32 *>(b + 0x394) = 0;
    *reinterpret_cast<u32 *>(b + 0x39c) = 0;
    *reinterpret_cast<u32 *>(b + 0xed0) = 0;
    *reinterpret_cast<u32 *>(b + 0x3a4) = 0;
    *reinterpret_cast<u32 *>(b + 0x3ac) = 0;
    *reinterpret_cast<u32 *>(b + 0x2a0) = 0;
    *reinterpret_cast<u32 *>(b + 0x2a4) = 0;
    *reinterpret_cast<u32 *>(b + 0xebc) = 0;
    *reinterpret_cast<u32 *>(b + 0xec0) = 0;
    *reinterpret_cast<u32 *>(b + 0x10b0) = 0;
    *reinterpret_cast<u32 *>(b + 0x10b4) = 0;
    *reinterpret_cast<u32 *>(b + 0xec4) = 0;
    *reinterpret_cast<u32 *>(b + 0x448) = 0;
    *reinterpret_cast<u32 *>(b + 0xec8) = 0;
    *reinterpret_cast<u32 *>(b + 0xed8) = 0;
    memset(b + 0xf48, 0, 0x68);
    b[0x1089] = 0;
    *reinterpret_cast<u32 *>(b + 0xfe4) = 0;
    *reinterpret_cast<u32 *>(b + 0x3b4) = 0;
    *reinterpret_cast<u32 *>(b + 0x294) = 0;
    *reinterpret_cast<u32 *>(b + 0x298) = 0;
    *reinterpret_cast<u32 *>(b + 0x29c) = 0;
    bool special = (object->apiobj.character_data->model_flags & 0x4000) &&
                   (b[0x27c] == 0xff || *reinterpret_cast<void **>(b + 0xcc0));
    b[0x1f8] = (b[0x1f8] & 0xfd) | (special << 1);
    u8 *config = reinterpret_cast<u8 *>(object->apiobj.character_data->player_config);
    b[0x1f9] &= 0xf7;
    b[0x1fa] &= 0xe7;
    b[0xeff] = (b[0xeff] & 0x7b) | ((*reinterpret_cast<u32 *>(config + 0x90) >> 6) & 0x80);
    b[0xefe] &= 0xbf;
    b[0x108f] = 0;
    *reinterpret_cast<u32 *>(b + 0x10c0) = 0;
    *reinterpret_cast<u32 *>(b + 0x10d8) = 0;
    *reinterpret_cast<f32 *>(b + 0x1040) = 1.0f;
    *reinterpret_cast<u32 *>(b + 0x1044) = 0;
    b[0xf04] = (b[0xf04] & 0x73) | (config[0x98] & 0x80);
    ResetPlayerAI(object);
}

void ResetPlayerAI(GameObject_s *object) {
    object->field_0x109c = 0;
    object->ai.movement_flags &= 0x9f;
    object->field_0xefe &= 0xdf;
    object->ai.field_0x1e5 &= 0xfd;
    object->ai.intersection_connection = NULL;
    object->ai.intersection_target_connection = NULL;
    object->ai.animation_override_from = -1;
    object->ai.animation_override_to = -1;
    object->ai.field_0x180 = NULL;
    object->field_0x1092 = 0;
    object->field_0x1093 = 0;
    object->field_0x1098 = 0;
    object->field_0x1094 = 0;
    object->field_0xf08 = 0;
    object->field_0xf0c = 0;
    object->field_0xf10 = 0;
    object->ai.frame_state = 0;
    object->field_0xf14 = 0;
    memset(&object->ai.path_info, 0, sizeof(object->ai.path_info));
    object->ai.current_route = 0xff;
    object->ai.next_route = 0;
    object->ai.inside_path_node = -1;
    AISysGetCharacterPathPos(WORLD->ai_sys, &object->apiobj, &object->ai, 0xff, 1);
}

void StarWars_AutoSetAICapabilities(GameObject_s *object);

void ActivatePlayer(GameObject_s *object) {
    object->character_context = -1;
    if (object->apiobj.character_model->model_data_b[0x80] != NULL) {
        object->character_context = 0x41;
        object->context_animation = 0x80;
        f32 duration = AnimDuration(object->id, 0x80, 0.0f, 0.0f, 1);
        object->airborne_action_duration = 0.0f;
        object->context_animation_timer = duration;
    }
}

i32 MakePlayerList(i32 count) {
    i32 player_count = 0;

    for (i32 i = 0; i < count; i++) {
        GameObject_s *player = Player[i];
        if (player == NULL) {
            if (makeplayerlist_freeplay == 1 && player_count == 1 && WORLD != NULL &&
                WORLD->current_level == HUB_LDATA && HUB_LDATA != NewLData) {
                PlayerList[1] = FreePlayModelList[i].model_id;
                PlayerProgress[1].active = 0;
                player_count = 2;
            }
            continue;
        }

        if (makeplayerlist_freeplay == 1) {
            PlayerList[player_count] = FreePlayModelList[i].model_id;
        } else if (makeplayerlist_freeplay != 2) {
            PlayerList[player_count] = player->id;
        }

        PLAYERPROGRESS *progress = &PlayerProgress[player_count];
        progress->active = (u8)player->apiobj.field_0x1f8 >> 7;

        bool unavailable = false;
        if (player->field_0x7a5 != 6 && player->field_0xe32 != 2) {
            unavailable = true;
            if ((player->field_0xe22 & 1) == 0) {
                unavailable = player->field_0xe32 != 0;
            }
        }
        progress->field_0x7 = unavailable;
        progress->coins = player->coinpacket != NULL ? player->coinpacket->coins : 0;
        progress->field_0xb = player->field_0xe31 == 1;

        if (player->apiobj.field_0x287 != 0) {
            progress->hitpoints = player->hitpoints;
            progress->field_0x9 = 4;
        } else {
            progress->hitpoints = player->current_hp;
            progress->field_0x9 = player->field_0xe38;
        }
        progress->field_0xa = player->field_0x108e;
        progress->field_0x4 = player->field_0x106e;
        progress->field_0xc = player->field_0xdec;

        PlayerSuit[player_count] = player->suit;
        PlayerTorpedoCount[player_count] = player->torpedo != NULL ? player->torpedo->count : 0;
        player_count++;
    }

    for (i32 i = player_count; i < 8; i++) {
        PlayerList[i] = -1;
    }
    makeplayerlist_freeplay = 0;
    return player_count;
}

i32 DeactivatePlayer(GameObject_s *object, f32 duration, GameObject_s *source) {
    if (object->character_context == 0x17 && duration <= object->context_animation_timer)
        return 0;
    f32 model_state = object->field_0xd24;
    Player_ClearContext(object, 1);
    if (object->character_context == 0x3e)
        return 0;
    Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    object->character_context = 0x17;
    object->field_0xd24 = model_state;
    AISCRIPTPROCESS *process = reinterpret_cast<AISCRIPTPROCESS *>(&object->ai);
    if (AIScriptSetBaseScriptStateByName(process, "BeenDeactivated")) {
        AIScriptProcess(WORLD->ai_sys, &object->apiobj, &object->ai, process, FRAMETIME);
    }
    CHARACTERDATA *character = object->apiobj.character_data;
    CHARACTERMODEL_s *model = object->apiobj.character_model;
    GAMECHARACTERDATA *runtime = static_cast<GAMECHARACTERDATA *>(character->field11_0x24);
    if (source != NULL && (source->apiobj.character_data->model_flags & 0x40) != 0 && runtime->field275_0x116 == 1 &&
        model->model_data_b[0x62] != NULL) {
        duration = 4.0f;
        object->context_animation = 0x62;
    } else if (model->model_data_b[0x81] != NULL &&
               (static_cast<CHARACTERANIM_s *>(model->model_data_a[0x81])->flags & 2) == 0) {
        object->context_animation = 0x81;
    } else {
        object->context_animation = model->model_data_b[0x41] != NULL ? 0x41 : 1;
    }
    if ((character->model_flags & 0x20) != 0)
        SetProtocolDroidDeactivatedAction(object);
    object->action_movement_state = 0;
    object->context_animation_timer = duration;
    object->airborne_action_duration = 0.0f;
    object->field_0x768 = 0.0f;
    NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
    return 1;
}

void ResetPlayerPacket(PLAYERPACKET_s *, CHARACTERDATA_s *);
i32 GetDefaultIdle(GameObject_s *);
void ResetCharacterIdle(GameObject_s *, i32, i32);
void SetGameObjectCharacterData(GameObject_s *);
void SetFlicker(GameObject_s *, f32);
void ResetCoinPacket(COINPACKET_s *);

void ResetPlayerMoves(GameObject_s *object) {
    ResetPlayerPacket(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet),
                      reinterpret_cast<CHARACTERDATA_s *>(object->apiobj.character_data));
    object->fall_acceleration_timer = 0.0f;
    object->pause_input_state = 0;
    object->apiobj.movement_direction = v000;
    object->input_toggle_hold_time = TOGGLEHOLDTIME;
    object->field_0xefc |= 0x80;
    ResetCharacterIdle(object, 2, GetDefaultIdle(object));
    if (object->apiobj.character_model->model_data_b[1] != NULL) {
        ResetAnimPacket(&object->apiobj.anim_packet, 1);
        SetAnimTimeRandom(object->apiobj.character_model, &object->apiobj.anim_packet);
    } else {
        ResetAnimPacket(&object->apiobj.anim_packet, -1);
    }
    object->apiobj.field_0x27e = 0;
    object->apiobj.field_0x27d = 0;
    SetGameObjectCharacterData(object);
    object->spawn_protection_timer = 0.0f;
    SetFlicker(object, 0.0f);
    ResetCoinPacket(object->coinpacket);
    object->apiobj.respawn_timer = 0.0f;
    object->apiobj.flags_high &= 0xdf;
    DrawOffsetCode(object, 1);
}

void SetToLastSafePos(GameObject_s *object) {
    NUVEC position = {object->apiobj.last_safe_position.x, object->apiobj.last_safe_position.y,
                      object->apiobj.last_safe_position.z};
    object->apiobj.start_position = position;
    object->saved_position = object->apiobj.position = object->apiobj.start_position;
}

i32 AvailableToPlayer(u32 character_flags, i32 weapon_action, i32 context, i32 require_all) {
    for (i32 index = 0; index < 8; ++index) {
        GameObject_s *object = Player[index];
        if (object == NULL || object->apiobj.character_data == NULL)
            continue;
        if (require_all != 0) {
            if ((character_flags == 0 ||
                 (object->apiobj.character_data->model_flags & character_flags) == character_flags) &&
                (weapon_action == -1 ||
                 static_cast<i8>(object->apiobj.character_data->game_character->uses_weapon_action) == weapon_action) &&
                (context == 0 || object->field_0x108e == context))
                return 1;
        } else {
            if ((character_flags != 0 &&
                 (object->apiobj.character_data->model_flags & character_flags) == character_flags) ||
                (weapon_action != -1 &&
                 static_cast<i8>(object->apiobj.character_data->game_character->uses_weapon_action) == weapon_action) ||
                context == 0 || object->field_0x108e == context)
                return 1;
        }
    }
    if (FreePlay != 0) {
        for (i32 index = 0; index < apicharsys->character_count; ++index) {
            i32 model = apicharsys->playermodelids[index];
            if (model == -1 || FreePlay == 0 || (apicharsys->models[model].flags & 1) == 0)
                continue;
            if (!((Game_CharacterSave != NULL && (Game_CharacterSave[index] & 1) != 0) ||
                  (static_cast<i32>(GCDataList[index].flags_090) < 0 && Cheats_CheckFlags(0x100) != 0)))
                continue;
            if (character_flags != 0) {
                if ((CDataList[index].model_flags & character_flags) == character_flags) {
                    if (require_all == 0)
                        return 1;
                } else if (require_all != 0) {
                    continue;
                }
            }
            if (weapon_action == -1 || static_cast<i8>(GCDataList[index].uses_weapon_action) == weapon_action)
                return 1;
        }
    }
    return 0;
}

void GetNumLocalPlayers() {
}

i32 UnderPlayerControl(GameObject_s *object) {
    return static_cast<i8>(object->apiobj.flags_low) < 0 ||
           (object->field_0xcc0 != NULL && static_cast<i8>(object->field_0xcc0->apiobj.flags_low) < 0);
}

i32 ActivePlayerInRange(nuvec_s *position, float range_squared, float *distance_squared) {
    for (i32 i = 0; i < 8; ++i) {
        GameObject_s *object = Player[i];
        if (object != NULL && static_cast<i8>(object->apiobj.flags_low) < 0) {
            const f32 distance = NuVecDistSqr(&object->apiobj.collision_position, position, NULL);
            if (distance < range_squared) {
                if (distance_squared != NULL)
                    *distance_squared = distance;
                return 1;
            }
        }
    }
    return 0;
}

GameObject_s *GetOtherActivePlayer(GameObject_s *object) {
    GameObject_s *other;
    if (Player[0] == object)
        other = Player[1];
    else if (Player[1] == object)
        other = Player[0];
    else
        return NULL;
    if (other != NULL && static_cast<i8>(other->apiobj.field_0x1f8) < 0)
        return other;
    return NULL;
}

bool FindNearestPlayerToVec(nuvec_s *position, GameObject_s **nearest_player, float &distance_squared,
                            bool require_character_flags, u32 character_flags) {
    *nearest_player = NULL;
    distance_squared = 0.0f;

    for (i32 index = 0; index < 8; ++index) {
        GameObject_s *candidate = Player[index];
        if (candidate == NULL || static_cast<i8>(candidate->apiobj.flags_low) >= 0) {
            continue;
        }

        const f32 candidate_distance = NuVecDistSqr(&candidate->apiobj.position, position, NULL);
        if (*nearest_player != NULL && candidate_distance >= distance_squared) {
            continue;
        }

        if (require_character_flags) {
            CHARACTERDATA *character = candidate->apiobj.character_data;
            GAMECHARACTERDATA *game_character =
                character == NULL ? NULL : static_cast<GAMECHARACTERDATA *>(character->field11_0x24);
            if (game_character == NULL || (game_character->flags_090 & character_flags) == 0) {
                continue;
            }
        }

        distance_squared = candidate_distance;
        *nearest_player = candidate;
    }

    return *nearest_player != NULL;
}

void SetPlayerGroupPosition(float, float, float) {
}

i32 (*LastSafePosExtraFn)(GameObject_s *) = NULL;

void CheckForPlayersTurnedOff() {
}

void FindFurthestPlayerFromVec(nuvec_s *, GameObject_s **, float &, bool, u32) {
}

void AveragePlayerCurrentSpeedMul() {
    avg_currentspeed_mul = 0.0f;
    f32 total = 0.0f;
    i32 count = 0;
    if (Player[0] != NULL && (Player[0]->apiobj.flags_low & 0x80) != 0) {
        total += Player[0]->current_speed_mul;
        count = 1;
    }
    if (Player[1] != NULL && (Player[1]->apiobj.flags_low & 0x80) != 0) {
        total += Player[1]->current_speed_mul;
        ++count;
    }
    avg_currentspeed_mul = total;
    if (count == 2)
        avg_currentspeed_mul *= 0.5f;
}

void SetPlayer() {
    if (Player[0] != NULL && static_cast<i8>(Player[0]->apiobj.field_0x1f8) < 0) {
        player = Player[0];
        if (Player[1] != NULL && static_cast<i8>(Player[1]->apiobj.field_0x1f8) < 0) {
            player2 = Player[1];
            return;
        }
    } else if (Player[1] != NULL && static_cast<i8>(Player[1]->apiobj.field_0x1f8) < 0) {
        player = Player[1];
    } else {
        player = NULL;
        return;
    }

    player2 = NULL;
}
