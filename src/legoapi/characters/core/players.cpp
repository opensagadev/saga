#include "decomp.h"
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
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/world/level.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/characters/motion.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/core/input/timer.h"
#include "legogame/game.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuhgobj.h"
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

extern "C" NUVEC plr_lastpos;

extern void GetTopBot(GameObject_s *obj);
extern f32 FindGunshipHoverHeight(GameObject_s *obj);
extern f32 GetVehicleSpeedMul(GameObject_s *obj, f32 input);
extern i32 ObjInNarrowSock(GameObject_s *obj, SOCKSYS *socks, i32 level);
extern i32 complexsockposition_forcesock;
extern i32 movegamecamera_forcesock;
extern AREADATA *BONUS_GUNSHIP_ADATA;
extern AREADATA *BATTLEOVERCORUSCANT_ADATA;
extern i32 LevFlag[4];
extern void GameObjectDimensions(GameObject_s *obj);
extern void GameObjectOrigin(GameObject_s *obj);
extern void ResetRumble(RUMBLEPACKET *packet);
extern void CurrentStart(GameObject_s *obj, i32 mode, i32 start);
extern void InitSurfaceInfo(GameObject_s *obj);
extern i32 SetObjOnSurface(GameObject_s *obj, i32 mode);
extern void GizForce_ResetLOS(GameObject_s *obj);
void Arcade_PlayerKilled(i32 player, i32 active);
void ResetPlayerAI(GameObject_s *obj);
void ResetPlayerMoves(GameObject_s *obj);
void SetProtocolDroidDeactivatedAction(GameObject_s *);
i32 TagCharacter(GameObject_s *source, GameObject_s *target, i32 mode);
void NewBuzz(nupad_s *, f32, i32);
void GameAudio_PlaySfxById(i32 sfx_id, nuvec_s *position, i32 flags, i32 volume);
f32 VehicleTurnOrLoopOffset(GameObject_s *object);
void Player_ClearContext(GameObject_s *object, i32 mode);
void Player_ResetContexts(PLAYERPACKET_s *packet);
extern "C" f32 chattersfxwait;

void Players_Init(void);

// --- Helpers moved from world.cpp ---


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
    if (!(world->start_door != NULL && bonus == 0)) {
        if (LEGOSPL_START != -1) {
            A = world->portal_places[LEGOSPL_START];
            ninit = 0;
        }
    } else {
        A = reinterpret_cast<PORTALPOS *>(world->start_door->spline);
    }

    if (A != NULL) {
        NUVEC tmp;
        i32 n = ninit;
        for (i32 i = 0; i < 8; i++) {
            const i32 m = n * 2;
            PlayerStart[i].pos = (NUVEC *)&A->positions[m * 3];
            NuVecSub(&tmp, (NUVEC *)&A->positions[m * 3 + 3], PlayerStart[i].pos);
            PlayerStart[i].angle = NuAtan2D(tmp.x, tmp.z);
            if (n * 2 + 4 > A->count)
                n = ninit;
            else
                n = n + 1;
        }
        if (bonus != 0 && A->count > 3) {
            i16 nc = A->count >> 2;
            i32 r = qrand() / (i32)(0xffff / nc + 1);
            PlayerStart[0].pos = (NUVEC *)&A->positions[12 * r];
            NUVEC *target = (NUVEC *)&A->positions[12 * r + 3];
            PlayerStart[0].angle = NuAtan2D(target->x - PlayerStart[0].pos->x, target->z - PlayerStart[0].pos->z);
            PlayerStart[1].pos = (NUVEC *)&A->positions[12 * r + 6];
            target = (NUVEC *)&A->positions[12 * r + 9];
            PlayerStart[1].angle = NuAtan2D(target->x - PlayerStart[0].pos->x, target->z - PlayerStart[1].pos->z);
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
                void *door = Door_FindByName(world, const_cast<char *>("mission_door"));
                if (door != NULL) {
                    NuStrCpy(Door_ExitName, (char *)door);
                }
            } else if (hub_from_arcade != -1) {
                void *door = Door_FindByName(world, const_cast<char *>("door_to_network"));
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
            Door_CutCamPos1 = spl->pts[1];
            Door_CutCamPos0 = spl->pts[0];
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

static PLAYERITEMTYPE_s *PlayerItemType = NULL;
static i32 PLAYERITEMTYPECOUNT = 0;

extern i8 BoltType_FindIDByName(char *name, WORLDINFO *world);

void PlayerItemTypes_Reset(WORLDINFO *world) {
    if (PlayerItemType != 0 && PLAYERITEMTYPECOUNT > 0) {
        for (i32 i = 0; i < PLAYERITEMTYPECOUNT; i++) {
            PlayerItemType[i].field_0x9 = (u8)BoltType_FindIDByName(PlayerItemType[i].name, world);
        }
    }
}

GameObject_s *AddCreature(i32 id, i32 param);

static f32 sPreResetMulA = 1.0f / 65535.0f;
static f32 sPreResetMulB = 0.03f;
static f32 sPreResetSubC = 0.015f;
static f32 sPreResetD18Scale = 0.25f;
static f32 sPreReset1048Scale = 0.5f;
static f32 sPreResetDivF = 0.21f;
static f32 sPreResetMulG = 0.035f;

void PreResetCode(GameObject_s *obj) {
    u8 t23 = (u8)(obj->field_0xe23 & 0xf8);
    obj->field_0xe20 &= 0xef;
    u8 t22 = (u8)(obj->field_0xe22 & 0x3f);
    u8 t25 = (u8)(obj->movement_runtime_flags & 0xaf);
    obj->field_0xe24 &= 0xcf;
    obj->field_0xf03 &= 0xdf;
    obj->quick_shoot_bolt_id = 0xff;
    obj->slam_debris_effect = 0xff;
    obj->reserved_e3c = 0x00;
    obj->attack_locator = 0xff;
    obj->movement_runtime_flags = t25;
    obj->field_0xe22 = t22;
    obj->field_0xe23 = t23;

    DrawOffsetCode(obj, 0);

    if ((obj->field_0xf00 & 8) != 0) {
        obj->field_0xe22 &= 0xf7;
        obj->weapon_trail_offset.x = (f32)qrand() * sPreResetMulA * sPreResetMulB - sPreResetSubC;
        obj->weapon_trail_offset.y = (f32)qrand() * sPreResetMulA * sPreResetMulB - sPreResetSubC;
        obj->weapon_trail_offset.z = (f32)qrand() * sPreResetMulA * sPreResetMulB - sPreResetSubC;
        obj->field_0xe20 &= 0xfb;
        obj->incoming_bolt = NULL;
        obj->incoming_melee = NULL;
        obj->incoming_special = NULL;
        obj->incoming_part = NULL;
        if (obj->field_0x7a5 != 0xa) {
            obj->field_0xe21 &= 0xf7;
        }
        obj->field_0xe21 &= 0xdf;
        obj->sabre_flags = 0x01;
        obj->field_0xe22 &= 0xfd;
        obj->blade_states[3] = 0xff;
        obj->blade_states[2] = 0xff;
        obj->blade_states[1] = 0xff;
        obj->blade_states[0] = 0xff;
        obj->force_glow_candidate = NULL;
        obj->apiobj.anim_packet.overlay_animation = -1;
        obj->terrain_origin_floor_offset = 0.0f;
        if ((obj->apiobj.character_data->model_flags & 0x8040) == 0) {
            obj->field_0xe31 = 0;
        }

        {
            i32 e04 = obj->dynamic_light_id;
            obj->field_0xe23 &= 0x7f;
            if (e04 != -1) {
                rtlDynamicEnable(e04, 0);
            }
        }

        {
            u8 al = obj->apiobj.field_0x27d;
            obj->movement_runtime_flags &= 0xfd;
            obj->field_0xe24 &= 0xbf;
            obj->field_0x1089 = 0;

            if (al != 0) {
                obj->ground_contact_grace_timer = 0.2f;
            } else if (obj->ground_contact_grace_timer > 0.0f) {
                obj->ground_contact_grace_timer -= FRAMETIME;
            }

            if ((i8) static_cast<u8>(obj->apiobj.field_0x1f8) >= 0 || VehicleArea != 0) {
                goto finish_dfd;
            }

            if (obj->field_0x7a5 == 0 && sPreReset1048Scale * obj->apiobj.character_data->player_config->reset_scale >
                                             obj->context_animation_timer) {
                goto finish_dfd;
            }

            {
                f32 f28v = obj->pad_gamepad->input_magnitude;
                if (f28v > 0.0f) {
                    if ((al & 2) != 0) {
                        goto finish_dfd;
                    }
                    if (obj->field_0x1084 == 0) {
                        goto finish_dfd;
                    }
                } else {
                    if (obj->apiobj.velocity.x == 0.0f && obj->apiobj.velocity.z == 0.0f) {
                        goto finish_dfd;
                    }
                    if ((al & 2) != 0) {
                        goto finish_dfd;
                    }
                    if (obj->field_0x1084 == 0) {
                        goto finish_dfd;
                    }
                }

                {
                    f32 t1 = obj->contact_normal.y;
                    if (!(t1 > NuTrigTable[0x4000])) {
                        goto finish_dfd;
                    }
                    f32 t2 = NuTrigTable[0x3000];
                    if (!(t2 > t1)) {
                        goto finish_dfd;
                    }
                    t1 = obj->apiobj.field_0x1e0 / sPreResetDivF;
                    f32 diff = obj->apiobj.collision_min.y - obj->apiobj.field_0x218;
                    t1 *= sPreResetMulG;
                    if (!(diff > t1)) {
                        goto finish_dfd;
                    }
                    NUVEC_ALIGNED16 dir;
                    if (!(f28v > 0.0f)) {
                        dir.x = obj->apiobj.velocity.x;
                        dir.y = 0.0f;
                        dir.z = obj->apiobj.velocity.z;
                        NuVecNorm(&dir, &dir);
                    } else {
                        NuVecRotateY(&dir, &v001, obj->apiobj.movement_facing_angle);
                    }
                    if (!((obj->contact_position.x - obj->apiobj.position.x) * dir.x +
                              (obj->contact_position.z - obj->apiobj.position.z) * dir.z >
                          0.0f)) {
                        goto finish_dfd;
                    }
                    obj->fall_acceleration_timer = 0.1f;
                }
            }
            goto finish_e1e;

        finish_dfd:
            if (obj->fall_acceleration_timer > 0.0f) {
                obj->fall_acceleration_timer -= FRAMETIME;
            }
            goto finish_e1e;

        finish_e1e:
            GameObjectNearFloor(obj, 1.0f, &obj->nearby_floor_distance);
            obj->pre_reset_hover_height = GetHoverPosY(obj);
            {
                u8 v = obj->apiobj.character_data->player_config->variant;
                if (v != 0xff) {
                    if (static_cast<u8>(obj->apiobj.field_0x27c) != 0xff) {
                        if (Cheat_IsOn(0x19) != 0) {
                            obj->blade_index = 0x00;
                        } else if (static_cast<u8>(obj->apiobj.field_0x27c) != 0xff &&
                                   Player_HasPurpleForce(obj) != 0) {
                            obj->blade_index = 0x03;
                        } else {
                            obj->blade_index = obj->apiobj.character_data->player_config->variant;
                        }
                    } else {
                        obj->blade_index = v;
                    }
                } else {
                    obj->blade_index = 0xff;
                }

                {
                    if ((obj->pad_gamepad->buttons_held & GAMEPAD_ACTION) != 0) {
                        obj->hold_timer += FRAMETIME;
                    } else {
                        if (obj->hold_timer > 0.0f && sPreResetD18Scale > obj->hold_timer) {
                            obj->field_0xe24 |= 0x40;
                        }
                        obj->hold_timer = 0.0f;
                    }
                }
            }
            obj->movement_runtime_flags &= 0x5f;
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

void SetHitPoints(GameObject_s *obj, i32 hp);

void RememberPlayerIDs(i32 a, i32 b, i32 c) {
    if (VehicleArea != 0 || GAMEDEMO != 0) {
        return;
    }
    if (a == 0 && (WORLD->current_level->flags & 0x4e2) != 2) {
        return;
    }
    const i32 ids[2] = {b, c};
    for (i32 i = 0; i < 2; ++i) {
        const i32 id = ids[i];
        if (id != -1 && (CDataList[id].model_flags & 0x2000) == 0 && PlayerID[i] != id && Collection_Got(id) == 1 &&
            GCDataList[id].field275_0x116 != 0 && PlayerID[1 - i] != id) {
            PlayerID[i] = id;
        }
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
        if (a == last_chatter_sfx && b > 0.0f) {
            chattersfxwait = b;
            last_chatter_sfx = -1;
        } else {
            GameAudio_PlaySfxById(a, &g->apiobj.collision_position, 0, 0);
            const i32 random = qrand();
            chattersfxwait = static_cast<f32>(random) * (1.0f / 65535.0f) * 2.0f + 3.0f;
            last_chatter_sfx = a;
        }
    }
}

void DrawOffsetCode(GameObject_s *obj, i32 param) {
    NUVEC delta;
    NUVEC_ALIGNED16 target = v000;
    u8 *bytes = reinterpret_cast<u8 *>(obj);
    void *route = *reinterpret_cast<void **>(bytes + 0xcac);
    if (route != NULL) {
        const u16 flags = *reinterpret_cast<u16 *>(static_cast<u8 *>(route) + 0xc);
        if ((flags & 0x10) != 0) {
            target.y = 0.0076f;
        } else if ((flags & 0x40) != 0) {
            target.y = MAGNETOFFSET;
        }
    }
    if ((obj->field_0x7a5 == 0x44 && static_cast<u16>(obj->context_animation - 5) >= 2) ||
        (obj->movement_context_state & 0xffff00) == 0x54300) {
        target = obj->context_position_offset;
    }
    if (param == 0) {
        if (obj->render_offset.x == target.x && obj->render_offset.y == target.y && obj->render_offset.z == target.z) {
            return;
        }

        const f32 distance = NuVecDist(&target, &obj->render_offset, &delta);
        const f32 step = 0.4f * FRAMETIME;
        if (!(distance <= step)) {
            NuVecNorm(&delta, &delta);
            NuVecScale(&delta, &delta, step);
            NuVecAdd(&obj->render_offset, &obj->render_offset, &delta);
            return;
        }
    }
    obj->render_offset = target;
}

float GetHoverPosY(GameObject_s *obj) {
    if ((obj->apiobj.character_data->model_flags & 0x2000) == 0 ||
        (WorldInfo_CurrentlyActive()->current_level->flags & LEVEL_IN_SPACE) != 0) {
        return 2000000.0f;
    }

    const f32 vehicle_offset = VehicleTurnOrLoopOffset(obj);
    if (obj->apiobj.field_0x27f == 7 || obj->apiobj.field_0x27f == 0x10)
        return vehicle_offset + obj->apiobj.water_height;

    f32 height = obj->apiobj.field_0x218;
    if (height != 2000000.0f) {
        if (__builtin_expect(obj->apiobj.water_height != 2000000.0f, 1))
            height = MAX(obj->apiobj.water_height, height);

        f32 hover_height = WORLD->current_level->hover_height;
        if (hover_height != 0.0f)
            return hover_height + height + vehicle_offset;
        return height + static_cast<GAMECHARACTERDATA *>(obj->apiobj.character_data->field11_0x24)->field_0x28 +
               vehicle_offset;
    }
    return 2000000.0f;
}

void PlayerItem_Set(PLAYERITEM_s *item, PLAYERITEMTYPE_s *type) {
    item->type = type;
    if (type != NULL) {
        item->ammunition = type->field_0x2;
        item->field_0x5 = type->field_0x3;
    } else {
        item->ammunition = 0;
        item->field_0x5 = 0;
    }
}

GameObject_s *Player_FindByID(i32 id) {
    for (i32 i = 0; i < 8; i++) {
        GameObject_s *object = Player[i];
        if (object != NULL && (object->apiobj.field_0x1f8 & 0x1001) == 0x1001 && object->id == id)
            return object;
    }
    return NULL;
}

NUVEC *Player_StartPos(GameObject_s *obj) {
    i32 index = obj->apiobj.field_0x27c;
    if (static_cast<u8>(index) > 7) {
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
    if (item != NULL && item->type != NULL && item->type->field_0x8 == 2)
        return item->ammunition != 0;
    return 1;
}

i32 Players_AveragePos(nuvec_s *position, SOCKPOSITION_s *socket_position) {
    NUVEC total = {0.0f, 0.0f, 0.0f};
    f32 player_count = 0.0f;

    if (Player[0] != NULL && Player[0]->apiobj.player_controlled) {
        NuVecAdd(&total, &total, &Player[0]->apiobj.position);
        player_count += 1.0f;
    }
    if (Player[1] != NULL && Player[1]->apiobj.player_controlled) {
        NuVecAdd(&total, &total, &Player[1]->apiobj.position);
        player_count += 1.0f;
    }

    if (player_count > 0.0f) {
        NuVecScale(position, &total, 1.0f / player_count);
        if (socket_position != NULL) {
            ComplexSockPosition(WORLD->sock_sys, position, -1, -1, socket_position);
        }
        return 1;
    }

    if (socket_position != NULL) {
        socket_position->location.sock = -1;
    }
    return 0;
}

i32 Players_BothActive() {
    return Player[0] != NULL && Player[0]->apiobj.player_controlled && Player[1] != NULL &&
           Player[1]->apiobj.player_controlled;
}

PLAYERITEMTYPE_s *PlayerItemType_Find(i32 id) {
    PLAYERITEMTYPE_s *type = PlayerItemType;
    if (type != NULL) {
        for (i32 i = 0; i < PLAYERITEMTYPECOUNT; i++) {
            if (type[i].id == id)
                return &type[i];
        }
    }
    return NULL;
}

void (*Player_ClearContextFn)(GameObject_s *, i32);
void Whip_Release(GameObject_s *);
i32 SpecialMove_ReleaseVictim(GameObject_s *);

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

void PlayerItemTypes_Init(PLAYERITEMTYPE_s *types) {
    PlayerItemType = NULL;
    PLAYERITEMTYPECOUNT = 0;
    if (types != NULL) {
        PlayerItemType = types;
        if (types->id != -1) {
            i32 count = 0;
            do {
                types++;
                count++;
            } while (types->id != -1);
            PLAYERITEMTYPECOUNT = count;
        } else {
            PlayerItemType = NULL;
        }
    }
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
    packet->gamepad->allocated_5a &= static_cast<u8>(~0x04);
    packet->gamepad->allocated_5a &= static_cast<u8>(~0x10);
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

void Player_CopyEssentials(GameObject_s *source, GameObject_s *destination);

i32 Player_HasDeflectBolts(GameObject_s *object) {
    return Cheats_CheckFlags(0x80000) != 0 || (object != NULL && object->field_0xdec > 0.0f);
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
        right = direction > 0;
        left = direction <= 0;
    } else {
        u32 right_mask = GAMEPAD_TOGGLERIGHT;
        u32 left_mask = GAMEPAD_TOGGLELEFT;
        if (LIFTPLAYER != 0) {
            right_mask &= ~GAMEPAD_LIFT;
            left_mask &= ~GAMEPAD_LIFT;
        }
        GAMEPAD_s *pad = object->pad_gamepad;
        right = pad->buttons_pressed & right_mask;
        left = pad->buttons_pressed & left_mask;
        if ((right != 0 && left != 0) || (left | right) == 0) {
            if ((object->apiobj.flags_low & 0x80) == 0) {
                return;
            }
            right = pad->buttons_held & right_mask;
            left = pad->buttons_held & left_mask;
            if ((right != 0 && left != 0) || (left | right) == 0) {
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
    i32 index = 0;
    while (index < apicharsys->loaded_model_count && object->id != apicharsys->models[index].model_id) {
        ++index;
    }
    if (index == apicharsys->loaded_model_count) {
        return;
    }
    i32 attempts = 0;
    i32 id;
    do {
        if (left != 0) {
            if (--index == -1) {
                index = apicharsys->loaded_model_count - 1;
            }
        } else if (++index == apicharsys->loaded_model_count) {
            index = 0;
        }
        id = apicharsys->models[index].model_id;
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
    next_character:;
    } while (attempts <= apicharsys->loaded_model_count && id != object->id);
}

i32 Player_HasInvincibility(GameObject_s *object) {
    return Cheats_CheckFlags(0x80) != 0 || (object != NULL && object->field_0xdec > 0.0f);
}

i32 Player_HasDoubleBoltDamage(GameObject_s *object) {
    return Cheats_CheckFlags(2) != 0 || (object != NULL && object->field_0xdec > 0.0f);
}

void PlayerButton_OnHold_Callback(MechTouchUIElement &element, TouchHolder &) {
    if (player != NULL && (player->field_0xcc0 == NULL || player->field_0xcc0->id == id_YODA)) {
        static_cast<MechTouchUIPlayerButton &>(element).ShowChooser();
        GameAudio_PlaySfx(0x30, NULL, 0, 0);
    } else {
        GameAudio_PlaySfx(0x32, NULL, 0, 0);
    }
}

i32 Player_HasDoubleWeaponDamage(GameObject_s *object) {
    return Cheats_CheckFlags(0x400) != 0 || (object != NULL && object->field_0xdec > 0.0f);
}

void PlayerButton_OnLeave_Callback(MechTouchUIElement &element, TouchHolder &holder) {
    MechTouchUIPlayerButton &button = static_cast<MechTouchUIPlayerButton &>(element);
    if (player != NULL && (player->field_0xcc0 == NULL || player->field_0xcc0->id == id_YODA) &&
        button.selector == NULL && holder.touch_position.y <= button.position.y &&
        holder.touch_position.y != button.position.y) {
        button.ShowChooser();
        GameAudio_PlaySfx(0x30, NULL, 0, 0);
    }
}

i32 Player_HasDoubleBoltDamage_FromBolt(BOLT_s *bolt) {
    i32 player;
    if (bolt->flags & 1)
        player = 0;
    else if (bolt->flags & 2)
        player = 1;
    else
        player = -1;
    if (player != -1)
        return Player_HasDoubleBoltDamage(Player[player]);
    return 0;
}

void PlayerButton_OnClick_Callback_NextButton(MechTouchUIElement &element, TouchHolder &) {
    if (player != NULL && (player->field_0xcc0 == NULL || player->field_0xcc0->id == id_YODA)) {
        static_cast<MechTouchUIPlayerButton &>(element).TriggerTagNext();
    } else {
        GameAudio_PlaySfx(0x32, NULL, 0, 0);
    }
}

u32 (*CanPushObstaclesFn)(GameObject_s *) = NULL;

namespace {



} // namespace

void ResetPlayer(GameObject_s *obj, i32 reset_moves, nuvec_s *position, i32 snap_to_surface);

void StarWars_AutoSetAICapabilities(GameObject_s *object);
i32 CanPullLevers(i32 id);
extern f32 DEFAULT_MOVE_RANGE;

void SetFlicker(GameObject_s *object, float duration);

#include "legoapi/characters/core/players_types.h"


void InitPlayerAI(GameObject_s *object);

void ResetPlayerAI(GameObject_s *object);

void StarWars_AutoSetAICapabilities(GameObject_s *object);

i32 MakePlayerList(i32 count);

void ResetPlayerPacket(PLAYERPACKET_s *, CHARACTERDATA_s *);
void ResetCharacterIdle(GameObject_s *, i32, i32);
void SetGameObjectCharacterData(GameObject_s *);
void ResetCoinPacket(COINPACKET_s *);

void ResetPlayerMoves(GameObject_s *object);

void SetToLastSafePos(GameObject_s *object) {
    NUVEC position = {object->apiobj.last_safe_position.x, object->apiobj.last_safe_position.y,
                      object->apiobj.last_safe_position.z};
    object->apiobj.start_position = position;
    object->saved_position = object->apiobj.position = object->apiobj.start_position;
}

i32 AvailableToPlayer(u32 character_flags, i32 weapon_action, i32 context, i32 require_all);

i32 GetNumLocalPlayers() {
    i32 count = 0;
    if (Player[0] != NULL)
        count = static_cast<i8>(Player[0]->apiobj.field_0x1f8) < 0;
    if (Player[1] != NULL && Player[1]->apiobj.player_controlled)
        count++;
    return count;
}

i32 UnderPlayerControl(GameObject_s *object) {
    return object->apiobj.player_controlled ||
           (object->field_0xcc0 != NULL && object->field_0xcc0->apiobj.player_controlled);
}

i32 ActivePlayerInRange(nuvec_s *position, float range_squared, float *distance_squared);

GameObject_s *GetOtherActivePlayer(GameObject_s *object);


DECOMP_ASSERT(sizeof(SPECIAL_LAYER_s) == 0xc, "SPECIAL_LAYER_s size");


i32 Player_HasPurpleForce(GameObject_s *obj);

void FixUpLayers();

u32 AdjustLayerBits(u32 mask, GameObject_s *object);

void SetPlayerGroupPosition(f32 x, f32 y, f32 z);

bool FindNearestPlayerToVec(nuvec_s *position, GameObject_s **nearest_player, float &distance_squared,
                            bool require_character_flags, u32 character_flags) {
    *nearest_player = NULL;
    distance_squared = 0.0f;

#define CONSIDER_PLAYER_DISTANCE(index, filtered)                                                                      \
    do {                                                                                                               \
        GameObject_s *candidate = Player[index];                                                                       \
        if (candidate != NULL && static_cast<i8>(candidate->apiobj.flags_low) < 0) {                                   \
            const f32 candidate_distance = NuVecDistSqr(&candidate->apiobj.position, position, NULL);                  \
            if (candidate_distance < distance_squared || *nearest_player == NULL) {                                    \
                candidate = Player[index];                                                                             \
                if (!(filtered) ||                                                                                     \
                    (candidate->apiobj.character_data->game_character->flags_090 & character_flags) != 0) {            \
                    distance_squared = candidate_distance;                                                             \
                    *nearest_player = candidate;                                                                       \
                }                                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)

    for (i32 i = 0; i < 8; i++) {
        CONSIDER_PLAYER_DISTANCE(i, require_character_flags);
    }

#undef CONSIDER_PLAYER_DISTANCE

    return *nearest_player != NULL;
}

i32 (*LastSafePosExtraFn)(GameObject_s *) = NULL;
void (*GameObjectDimensionsExtraFn)(GameObject_s *) = NULL;

void CheckForPlayersTurnedOff() {
    for (i32 source_index = 0; source_index < 2; ++source_index) {
        GameObject_s *source = Player[source_index];
        if (source != NULL && (source->apiobj.flags_high & APIOBJECT_HIGH_FLAG_CHARACTER) == 0) {
            for (i32 target_index = 2; target_index < 8; ++target_index) {
                GameObject_s *target = Player[target_index];
                if (target == NULL || (target->tag_flags & 2) != 0 || (target->field_0xefb & 4) != 0 ||
                    target->character_context == 0x24 || target->character_context == 0x1f) {
                    continue;
                }

                if (target->apiobj.field_0x27d != 0 ||
                    (target->field_0xe31 == 1 && target->apiobj.character_data->game_character->field_0x28 > 0.0f)) {
                    TagCharacter(Player[source_index], target, 1);
                }
            }
        }
    }
}

bool FindFurthestPlayerFromVec(nuvec_s *position, GameObject_s **furthest_player, float &distance_squared,
                               bool require_character_flags, u32 character_flags) {
    *furthest_player = NULL;
    distance_squared = 0.0f;

#define CONSIDER_PLAYER_DISTANCE(index, filtered)                                                                      \
    do {                                                                                                               \
        GameObject_s *candidate = Player[index];                                                                       \
        if (candidate != NULL && static_cast<i8>(candidate->apiobj.flags_low) < 0) {                                   \
            const f32 candidate_distance = NuVecDistSqr(&candidate->apiobj.position, position, NULL);                  \
            if (candidate_distance > distance_squared || *furthest_player == NULL) {                                   \
                candidate = Player[index];                                                                             \
                if (!(filtered) ||                                                                                     \
                    (candidate->apiobj.character_data->game_character->flags_090 & character_flags) != 0) {            \
                    distance_squared = candidate_distance;                                                             \
                    *furthest_player = candidate;                                                                      \
                }                                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)

    for (i32 i = 0; i < 8; i++) {
        CONSIDER_PLAYER_DISTANCE(i, require_character_flags);
    }

#undef CONSIDER_PLAYER_DISTANCE

    return *furthest_player != NULL;
}

void AveragePlayerCurrentSpeedMul() {
    i32 count = 0;
    avg_currentspeed_mul = 0.0f;
    f32 total = 0.0f;
    if (Player[0] != NULL && Player[0]->apiobj.player_controlled) {
        total += Player[0]->current_speed_mul;
        count = 1;
    }
    if (Player[1] == NULL || !Player[1]->apiobj.player_controlled) {
        avg_currentspeed_mul = total;
    } else {
        total += Player[1]->current_speed_mul;
        avg_currentspeed_mul = total;
        if (__builtin_expect(count == 1, 1)) {
            avg_currentspeed_mul = total * 0.5f;
        }
    }
}

void SetPlayer() {
    if (Player[0] != NULL && Player[0]->apiobj.player_controlled) {
        player = Player[0];
        if (Player[1] != NULL && Player[1]->apiobj.player_controlled) {
            player2 = Player[1];
        } else {
            player2 = NULL;
        }
    } else if (Player[1] != NULL && Player[1]->apiobj.player_controlled) {
        player = Player[1];
        player2 = NULL;
    } else {
        player = NULL;
    }
}
