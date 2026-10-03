#include "decomp.h"
#include "legoapi/items/collect/minikits.h"
#include "globals.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestatus_lsw.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/light/lighting.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

HUBMINIKITPIECES_s **Char_MiniKit;
static VARIPTR minikits_savecharacterbufferptr;
extern VARIPTR characterbuffer_ptr;
extern VARIPTR characterbuffer_end;

void MiniKits_Init(variptr_u *buffer, variptr_u *) {
    const usize aligned = (buffer->addr + 3) & ~static_cast<usize>(3);
    buffer->addr = aligned;
    Char_MiniKit = reinterpret_cast<HUBMINIKITPIECES_s **>(aligned);
    buffer->addr = aligned + static_cast<usize>(CHARCOUNT) * sizeof(*Char_MiniKit);
}

void CharacterMiniKits_Load(COLLECTION_s *collection, WORLDINFO *world, VARIPTR *buf, VARIPTR *buf_end) {
    if (Char_MiniKit == NULL)
        return;

    minikits_savecharacterbufferptr = characterbuffer_ptr;
    for (i32 i = 0; i < CHARCOUNT; ++i)
        Char_MiniKit[i] = NULL;

    buf->addr = ALIGN(buf->addr, 4);
    world->minikit_pieces_buf = reinterpret_cast<HUBMINIKITPIECES_s **>(buf->addr);
    memset(world->minikit_pieces_buf, 0, static_cast<usize>(CHARCOUNT) * sizeof(*world->minikit_pieces_buf));
    buf->addr += static_cast<usize>(CHARCOUNT) * sizeof(*world->minikit_pieces_buf);

    for (i32 index = 0; index < collection->count_y; ++index) {
        const i32 character_id = collection->list[index].id;
        if ((GCDataList[character_id].flags_094[1] & 4) != 0 || APICharacterLoaded(character_id) == NULL)
            continue;

        HUBMINIKITPIECES_s *minikit = reinterpret_cast<HUBMINIKITPIECES_s *>(buf->addr);
        world->minikit_pieces_buf[character_id] = minikit;
        buf->addr += 0x18;

        char path[256];
        NuStrCpy(path, const_cast<char *>("chars\\minikits\\"));
        NuStrCat(path, CDataList[character_id].file);
        NuStrCat(path, const_cast<char *>("\\"));
        NuStrCat(path, CDataList[character_id].file);
        NuStrCat(path, const_cast<char *>(".gsc"));

        buf->addr = ALIGN(buf->addr, 4);
        if (index <= 17)
            minikit->scene = NuGScnRead(buf, *buf_end, path);
        else
            minikit->scene = NuGScnRead(&characterbuffer_ptr, characterbuffer_end, path);

        MINIKIT *runtime = reinterpret_cast<MINIKIT *>(minikit);
        runtime->id = static_cast<i16>(character_id);
        if (minikit->scene == NULL) {
            world->minikit_pieces_buf[character_id] = NULL;
            buf->addr -= 0x18;
            continue;
        }

        MiniKit_InitPieces(runtime, 10, buf, buf_end);
        Char_MiniKit[character_id] = minikit;
    }
}

extern f32 KITPOSX, PANEL_MINIKITY, PANEL_MINIKITSCALE;
void MiniKit_GameMsg_Update(GAMEMESSAGE_s *message);
void MiniKit_GameMsg_End(GAMEMESSAGE_s *message);
void Hint_SetHintFromId(i32, i32, i32);

void CollectMinikit(nuvec_s *position, char *name, i32) {
    if (WORLD->lev_objs[0xce].active == 0) {
        return;
    }
    const i32 area_id = WORLD->level_sub_id;
    NUVEC target_position = {KITPOSX, KITPOSY + PANEL_MINIKITY, 1.0f};
    ADDGAMEMSG message = AddGameMsg_Default;
    message.position = position;
    message.target_position = &target_position;
    message.scale = AreaPickupScale;
    message.target_scale = PANEL_MINIKITSCALE;
    message.flags = 0x2112d;
    message.duration = 1.0f;
    message.icon = 0xce;
    message.special = &WORLD->lev_objs[0xce].special;
    message.tick_fn = MiniKit_GameMsg_Update;
    message.end_fn = MiniKit_GameMsg_End;
    message.field_0x4d = 1;
    AddGameMsg(&message);
    DrawMiniKitTime = 2.0f;

    if (area_id != -1 && Game.area_save[area_id].minikit_count < 10) {
        if (NuStrLen(name) <= 8) {
            NuStrCpy(NewMiniPiece[AreaGlobals.values.field_0x10].name, name);
        } else {
            NewMiniPiece[AreaGlobals.values.field_0x10].name[0] = '\0';
        }
        NewMiniPiece[AreaGlobals.values.field_0x10].level = WORLD->level_idx;
        ++AreaGlobals.values.field_0x10;
        ++AreaGlobals.values.field_0x0c;
        if (GAMEDEMO != 0) {
            if (AreaGlobals.values.field_0x0c == 1) {
                Hint_SetHintFromId(0x26d, 0, 0);
            } else if (AreaGlobals.values.field_0x0c == 10) {
                Hint_SetHintFromId(0x26e, 0, 0);
            }
        }
    }
    AddGameDebris(WORLD->debris_sys, 0x13, position);
    GameAudio_PlaySfxById(GetSfxId(const_cast<char *>("MK-Pickup")), position, 3, 0);
}

i32 AllMiniKitsDone(AREASAVE_s *save) {
    if (save == NULL) {
        return 1;
    }

    for (i32 i = 0; i < AREACOUNT; ++i, ++save) {
        if ((ADataList[i].flags & AREAFLAG_MINIKIT) != 0 && save->minikit_complete == SAVE_INCOMPLETE) {
            return 0;
        }
    }
    return 1;
}

char *LEGOASCII_BIGARROW = NULL;
void GameMsg_Draw_MiniKitDetector(GAMEMESSAGE_s *, nuvec_s *,
                                  float) __asm__("_ZL28GameMsg_Draw_MiniKitDetectorP13GAMEMESSAGE_sP7nuvec_sf")
    __attribute__((visibility("hidden")));
void MiniKitDetector(nuvec_s *position) {
    ADDGAMEMSG message = AddGameMsg_Default;
    message.text = LEGOASCII_BIGARROW != NULL ? LEGOASCII_BIGARROW : txt_UNKNOWN;
    message.position = position;
    message.flags = 0x40083;
    message.scale = 0.6f;
    message.field_0x44 = reinterpret_cast<void *>(GameMsg_Draw_MiniKitDetector);
    message.field_0x4f = 4;
    AddGameMsg(&message);
}

i32 MatrixReflection(NUMTX *, i32, f32, f32, NUMTX *);
void CharMiniKit_Draw(i32 id, numtx_s *matrix, i32 reflection_axis, float reflection_plane, float reflection_height) {
    if (Char_MiniKit == NULL)
        return;
    HUBMINIKITPIECES_s *kit = Char_MiniKit[id];
    if (kit == NULL || kit->piece_count == 0)
        return;
    bool reflection = reflection_axis != 0 && reflection_plane != 2000000.0f;
    for (i32 i = 0; i < kit->piece_count; ++i) {
        HUBMINIKITPIECE_s *piece = &kit->pieces[i];
        if (NuSpecialExistsFn(&piece->special) == 0)
            continue;
        NUMTX draw_matrix = piece->matrix;
        NuMtxMul(&draw_matrix, &draw_matrix, matrix);
        NuSpecialDrawAt(&piece->special, &draw_matrix);
        if (reflection) {
            NUMTX reflected;
            if (MatrixReflection(&draw_matrix, reflection_axis, reflection_plane, reflection_height, &reflected) != 0) {
                NuRndrStartReflectionRender(0);
                NuSpecialDrawAt(&piece->special, &reflected);
                NuRndrEndReflectionRender();
            }
        }
    }
}

extern i32 currentminikit, newminikitcount;
f32 slideseek;
f32 pop_timer;
f32 jibberlen = 0.1f;
f32 slidetime = 0.25f;
void NextStatusStage(STATUSPACKET_s *);
void SetDrawGoldBrick(STATUSPACKET_s *, i32);
void IncreaseScore(u32 *, u64, i32);
void NewStatusRumbleBuzz(i32, f32, f32, i32);
extern "C" void PlaySfx(char *, nuvec_s *);
void AddStatusMiniKitParts();
void DrawStatusMiniKit(f32, f32, f32, f32, f32, i32, STATUSPACKET_s *, f32);
void DrawMiniKitCount(f32, f32, i32, i32);
f32 getFinishedStatusAlpha(STATUSPACKET_s *);
extern f32 STATUS_TITLE_Y;
extern i16 tMINIKIT;

void MiniKit_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 current) {
    char text[60];
    if (current == 0) {
        if (stage->field_0x12 == 0)
            return;
        const f32 alpha = getFinishedStatusAlpha(packet);
        i32 angle = 0x2000;
        if (GameTimer.time_elapsed_mod_seconds <= 0.25f) {
            angle = (static_cast<i32>(GameTimer.time_elapsed_mod_seconds * 32768.0f + 16384.0f) >> 1) & 0x7fff;
        }
        if (Game.area_save[packet->area->index].field_0x5[0] == 0) {
            const f32 size = ((1.0f - fabsf(NuTrigTable[angle])) + 1.0f) * 1.2f;
            Text3DEx("?", -0.6f, -0.6f, 1.1f, size, size, size, 0, 255, 255, 255,
                     static_cast<u8>(static_cast<i32>(alpha * 128.0f)));
        } else if (alpha > 0.0f) {
            DrawStatusMiniKit(-0.6f, -0.5f, 1.1f,
                              NuTrigTable[(static_cast<i32>(alpha * 16384.0f) >> 1) & 0x7fff] * 0.15f, 1.0f,
                              Game.area_save[packet->area->index].field_0x5[0], packet, 0.0f);
        }
        if (packet->minikit_max == Game.area_save[packet->area->index].field_0x5[0]) {
            Text3DEx("$", -0.6f, -0.7f, 1.0f, 0.8f, 0.8f, 0.8f, 0, 255, 0, 127,
                     static_cast<u8>(static_cast<i32>(alpha * 128.0f)));
        } else {
            sprintf(text, "%i/%i", Game.area_save[packet->area->index].field_0x5[0], packet->minikit_max);
            Text3DEx(text, -0.6f, -0.8f, 1.0f, 0.5f, 0.5f, 0.5f, 0, 255, 0, 127,
                     static_cast<u8>(static_cast<i32>(alpha * 128.0f)));
        }
        return;
    }
    f32 title_alpha;
    switch (stage->field_0x14) {
        case 0:
            title_alpha = 0.0f;
            break;
        case 1: {
            title_alpha = stage->field_0x18;
            i32 angle = 0x6000;
            if (title_alpha < 1.0f)
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
            if (stage->field_0x18 < duration) {
                i32 angle = 0;
                if (duration != 0.0f && stage->field_0x18 != 0.0f)
                    angle = (static_cast<i32>((stage->field_0x18 / duration) * 16384.0f + 49152.0f + 16384.0f) >> 1) &
                            0x7fff;
                size = NuTrigTable[angle] * 0.333f;
            } else
                size = 0.333f;
            DrawStatusMiniKit(0.0f, 0.0f, 1.1f, 0.333f, size, currentminikit + 1, packet, 0.0f);
            const i32 count = packet->new_minikits < 1 ? Game.area_save[packet->area->index].field_0x5[0]
                              : currentminikit < 0     ? 0
                                                       : currentminikit;
            DrawMiniKitCount(1.0f, 1.0f, count, packet->minikit_max);
            title_alpha = 1.0f;
            break;
        }
        case 3: {
            f32 blend = 0.0f;
            if (stage->field_0x1c - jibberlen < stage->field_0x18)
                blend = (stage->field_0x18 - (stage->field_0x1c - jibberlen)) / jibberlen;
            DrawStatusMiniKit(0.0f, 0.0f, 1.1f, 0.333f, 0.333f, currentminikit + 1, packet, blend);
            i32 count = packet->new_minikits < 1 ? Game.area_save[packet->area->index].field_0x5[0] : currentminikit;
            if (blend >= 0.5f)
                ++count;
            if (count < 0)
                count = 0;
            const f32 size =
                (1.0f - fabsf(NuTrigTable[(static_cast<i32>(blend * 32768.0f + 16384.0f) >> 1) & 0x7fff])) * 0.25f +
                1.0f;
            DrawMiniKitCount(1.0f, size, count, packet->minikit_max);
            title_alpha = 1.0f;
            break;
        }
        case 4: {
            title_alpha = 1.0f - stage->field_0x18;
            f32 progress = 0.0f;
            if (stage->field_0x1c != 0.0f && stage->field_0x18 != 0.0f)
                progress = stage->field_0x18 / stage->field_0x1c;
            const f32 blend =
                1.0f - (NuTrigTable[(static_cast<i32>(progress * 32768.0f + 16384.0f) >> 1) & 0x7fff] + 1.0f) * 0.5f;
            DrawStatusMiniKit(blend * -0.6f + 0.0f, blend * -0.5f + 0.0f, 1.1f, blend * -0.183f + 0.333f, 1.0f,
                              Game.area_save[packet->area->index].field_0x5[0], packet, 0.0f);
            const f32 off = stage->field_0x18 < 1.0f ? 1.0f - stage->field_0x18 : 0.0f;
            DrawMiniKitCount(1.0f - (NuTrigTable[(static_cast<i32>(off * 32768.0f + 16384.0f) >> 1) & 0x7fff] + 1.0f) *
                                        0.5f,
                             1.0f, Game.area_save[packet->area->index].field_0x5[0], packet->minikit_max);
            break;
        }
        case 6: {
            title_alpha = 1.0f - stage->field_0x18;
            i32 angle = 0x2000;
            if (stage->field_0x18 < 1.0f)
                angle = (static_cast<i32>(title_alpha * 32768.0f + 16384.0f) >> 1) & 0x7fff;
            if (stage->field_0x18 < stage->field_0x1c) {
                DrawMiniKitCount(1.0f - (NuTrigTable[angle] + 1.0f) * 0.5f, 1.0f,
                                 Game.area_save[packet->area->index].field_0x5[0], packet->minikit_max);
                DrawStatusMiniKit(0.0f, 0.0f, 1.1f, 0.333f, 1.0f, Game.area_save[packet->area->index].field_0x5[0],
                                  packet, 0.0f);
            }
            break;
        }
        default:
            title_alpha = 1.0f;
            break;
    }
    title_alpha = title_alpha < 0.0f ? 0.0f : title_alpha > 1.0f ? 1.0f : title_alpha;
    Text3DEx(TTab[tMINIKIT], 0.0f, STATUS_TITLE_Y, 1.0f, 0.5f, 0.5f, 0.5f, 0, 255, 255, 255,
             static_cast<u8>(static_cast<i32>(title_alpha * 128.0f)));
}

void MiniKit_LSW_Skip(STATUS_STAGE_s *stage, STATUSPACKET_s *packet) {
    currentminikit += newminikitcount;
    if ((packet->field_0xb0 & 0x10) != 0 && (stage->field_0x14 != 6 || stage->field_0x18 < stage->field_0x1c))
        IncreaseScore(packet->score, 50000, 0);
    NextStatusStage(packet);
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

void CollectAllMiniKits(AREASAVE_s *save) {
    if (save == NULL)
        return;
    for (i32 i = 0; i < AREACOUNT; ++i) {
        if ((ADataList[i].flags & AREAFLAG_MINIKIT) != 0 && save[i].complete != SAVE_INCOMPLETE) {
            save[i].minikit_count = 10;
            save[i].minikit_complete = SAVE_COMPLETE;
        }
    }
}

void MiniKit_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    switch (stage->field_0x14) {
        case 0:
            newminikitcount = packet->new_minikits;
            currentminikit = Game.area_save[packet->area->index].field_0x5[0];
            if (newminikitcount > 0)
                currentminikit -= newminikitcount;
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
            if ((packet->field_0xb0 & 0x10) == 0)
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
            if (stage->field_0x18 < stage->field_0x1c) {
                if (static_cast<u32>(GameTimer.update_count) % 6 < 3 && (GameTimer.update_count - 1U) % 6 > 2)
                    NewStatusRumbleBuzz(-1, 0.0f, 0.0f, 2);
            } else if (previous < stage->field_0x1c) {
                AddStatusMiniKitParts();
                NewStatusRumbleBuzz(-1, 1.0f, 0.1f, 0);
                PlaySfx(const_cast<char *>("Explode1"), NULL);
            } else if (FindGameMsgsWithID(1, 0, -1, NULL) == 0) {
                PlaySfx(const_cast<char *>("Shop_BuyCheat"), NULL);
                NextStatusStage(packet);
            }
            break;
        }
    }
}

extern i32 LEGOOBJ_MINIKIT, LEGOOBJ_CHARKIT;
void EndChallenge(i32, i32);
void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);
void MiniKit_GameMsg_End(GAMEMESSAGE_s *message) {
    MiniKitScale = 2.0f;
    if (WorldInfo_CurrentlyActive()->area != NULL && message->icon != -1) {
        if (message->icon == LEGOOBJ_MINIKIT) {
            ++AreaGlobals.values.field_0x14;
            if (AreaGlobals.values.field_0x14 > AreaGlobals.values.field_0x0c)
                AreaGlobals.values.field_0x14 = AreaGlobals.values.field_0x0c;
        } else if (message->icon == LEGOOBJ_CHARKIT && AreaGlobals.values.field_0x20 < AreaGlobals.values.field_0x1c) {
            ++AreaGlobals.values.field_0x20;
            if (AreaGlobals.values.field_0x20 == AreaGlobals.values.field_0x1c && AreaGlobals.values.field_0x1c > 9)
                EndChallenge(2, 1);
        }
    }
    GameAudio_PlaySfx(0x26, NULL, 0, 0);
    NewRumbleAllPlayers(0.6f, 0.0f, 0, 0);
    GameCam_Judder(GameCam, -0.2f, 0, NULL);
}

void ResetMinikitCounter() {
    minikitCounter_C = 0;
    minikitCounter_A = 0;
}

extern i16 id_SLAVE1, tALLMINIKITSBUILT;
extern i32 STATUS_R, STATUS_G, STATUS_B;

void AllMiniKits_LSW_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 current) {
    char text[60];
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
            title_alpha = 1.0f - icon_alpha;
            y = (1.0f - NuTrigTable[(static_cast<i32>(icon_alpha * 16384.0f) >> 1) & 0x7fff]) * -0.5f + 0.225f;
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
                     static_cast<u8>(static_cast<i32>(title_alpha * 128.0f)));
        }
        return;
    }
    if (stage->field_0x12 == 0)
        return;
    const f32 alpha = getFinishedStatusAlpha(packet);
    const u8 opacity = static_cast<u8>(static_cast<i32>(alpha * 128.0f));
    i32 angle = 0x2000;
    if (GameTimer.time_elapsed_mod_seconds <= 0.25f)
        angle = (static_cast<i32>(GameTimer.time_elapsed_mod_seconds * 32768.0f + 16384.0f) >> 1) & 0x7fff;
    if (Game.area_save[packet->area->index].field_0x5[0] == 0) {
        const f32 size = ((1.0f - fabsf(NuTrigTable[angle])) + 1.0f) * 1.2f;
        Text3DEx("?", -0.6f, -0.6f, 1.1f, size, size, size, 0, 255, 255, 255, opacity);
    } else {
        DrawStatusMiniKit(-0.6f, -0.5f, 1.1f, NuTrigTable[(static_cast<i32>(alpha * 16384.0f) >> 1) & 0x7fff] * 0.15f,
                          1.0f, Game.area_save[packet->area->index].field_0x5[0], packet, 0.0f);
    }
    if (Game.area_save[packet->area->index].field_0x5[0] == packet->minikit_max) {
        Text3DEx("$", -0.6f, -0.7f, 1.0f, 0.8f, 0.8f, 0.8f, 0, 255, 0, 127, opacity);
    } else {
        sprintf(text, "%i/%i", Game.area_save[packet->area->index].field_0x5[0], packet->minikit_max);
        Text3DEx(text, -0.6f, -0.8f, 1.0f, 0.5f, 0.5f, 0.5f, 0, 255, 0, 127, opacity);
    }
}

void AllMiniKits_LSW_Skip(STATUS_STAGE_s *, STATUSPACKET_s *packet) {
    NextStatusStage(packet);
}

rtldata_s KitPartRTL;
extern NUCOLOUR3 panelcol[3], panelamb;
extern NUVEC paneldir[3];

void AddStatusMiniKitParts() {
    NUVEC target_position = {0.0f, STATSPOSY, 1.0f};
    for (i32 i = 0; i < 3; ++i) {
        KitPartRTL.intensity[i] = panelcol[i];
        KitPartRTL.direction[i] = paneldir[i];
    }
    KitPartRTL.ambient.x = panelamb.r;
    KitPartRTL.ambient.y = panelamb.g;
    KitPartRTL.ambient.z = panelamb.b;

    f32 delay = 0.0f;
    for (i32 i = 0; i < 10; ++i) {
        if (KitPart[i].enabled == 0) {
            continue;
        }
        NUVEC velocity = {0.0f, (static_cast<f32>(qrand()) * (1.0f / 65535.0f)) * 0.25f + 0.25f, 0.0f};
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
            NUVEC position = {0.0f, (static_cast<f32>(qrand()) * (1.0f / 65535.0f)) * 0.1f, 1.0f};
            NuVecRotateZ(&position, &position, qrand());
            ADDGAMEMSG message = AddGameMsg_Default;
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
            message.update_fn = GameMsg_DrawAdjustNewPos_CoinToTotal;
            message.field_0x20 = delay;
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

void AllMiniKits_LSW_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    if (stage->field_0x14 == 0) {
        stage->field_0x18 = 0.0f;
        stage->field_0x1c = 6.5f;
        stage->field_0x14 = 1;
    } else if (stage->field_0x14 == 1) {
        const f32 previous = stage->field_0x18;
        stage->field_0x18 += elapsed;
        if (stage->field_0x18 >= stage->field_0x1c)
            NextStatusStage(packet);
        else if (previous < 0.5f && stage->field_0x18 >= 0.5f) {
            PlaySfx(const_cast<char *>("StatusAward"), NULL);
            NewStatusRumbleBuzz(-1, 0.6f, 0.0f, 0);
        } else if (previous < 4.0f && stage->field_0x18 >= 4.0f)
            PlaySfx(const_cast<char *>("Char_Icon_App"), NULL);
    }
}

void CharacterMiniKits_Dump(WORLDINFO_s *world) {
    if (world->minikit.gscn != NULL) {
        NuGScnRemove(world->minikit.gscn);
        world->minikit.gscn = NULL;
        world->minikit.field_0x4 = NULL;
        world->minikit.field_0x8 = 0;
    }
    if (world->minikit_pieces_buf == NULL) {
        return;
    }

    if (world->area == NULL || (world->area->flags & 5) != 5) {
        for (i32 i = 0; i < AREACOUNT; ++i) {
            HUBMINIKITPIECES_s *entry = world->minikit_pieces_buf[i];
            if (entry != NULL && entry->scene != NULL) {
                NuGScnRemove(entry->scene);
                world->minikit_pieces_buf[i]->scene = NULL;
            }
        }
    } else if (Char_MiniKit != NULL) {
        for (i32 i = 0; i < CHARCOUNT; ++i) {
            HUBMINIKITPIECES_s *entry = world->minikit_pieces_buf[i];
            if (entry != NULL && entry->scene != NULL) {
                NuGScnRemove(entry->scene);
                world->minikit_pieces_buf[i]->scene = NULL;
            }
            Char_MiniKit[i] = 0;
        }
        characterbuffer_ptr = minikits_savecharacterbufferptr;
    }
}

void MiniKit_GameMsg_Update(GAMEMESSAGE_s *message) {
    i32 angle = static_cast<i32>((NuFmod(GlobalTimer.time_elapsed, 4.0f) * 0.25f) * 65536.0f);
    message->rotation_y = static_cast<u16>(angle);
    message->field_0xe0 = static_cast<u16>(NuTrigTable[angle & 0x7fff] * 1820.0f);
}

void SetEffectVisibility(char *, i32);

void EffectOffProgress_Reset(LEVEL_PROGRESS_s *progress) {
    if (progress == NULL)
        return;
    for (i32 i = 0; i < 12; ++i) {
        if (progress->disabled_effect_names[i][0] != '\0')
            SetEffectVisibility(progress->disabled_effect_names[i], 0);
    }
}

void IncrementMinikitCounter(GameObject_s *) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    NUVEC average;
    Players_AveragePos(&average, NULL);
    u8 *counter;
    i32 gizmo_index;
    NUVEC camera;
    if (world->current_level == HOTHBATTLEA_LDATA) {
        gizmo_index = 0;
        counter = &minikitCounter_A;
        camera = {0.0f, 0.0f, 10.0f};
    } else if (world->current_level == HOTHBATTLEC_LDATA) {
        gizmo_index = 1;
        counter = &minikitCounter_C;
        camera = {0.0f, 0.0f, 25.0f};
    } else {
        return;
    }
    GIZMO *gizmo = LevGizmo[gizmo_index];
    if (gizmo == NULL || gizmo->object == NULL) {
        return;
    }
    GIZMOPICKUP_s *pickup = static_cast<GIZMOPICKUP_s *>(gizmo->object);
    if ((pickup->state_flags & (GIZMOPICKUP_STATE_COLLECTED | GIZMOPICKUP_STATE_ALTERNATE_TYPE)) != 0 ||
        *counter >= 10) {
        return;
    }
    ++*counter;
    if (*counter == 10) {
        if (gizmo_index == 0) {
            NuVecRotateX(&camera, &camera, 0xf1c8);
            NuVecRotateY(&camera, &camera, 0xe000);
        } else {
            NuVecRotateX(&camera, &camera, 0xeaab);
            NuVecRotateY(&camera, &camera, 0x78e4);
        }
        NuVecAdd(&camera, &camera, &pickup->position);
        GameCameraMakeMiniCut2(&camera, &pickup->position, 0, 0.0f, 4.0f, 0.0f, 0.0f, 0, 0, 0);
        GizmoActivate(world->gizmo_sys, LevGizmo[gizmo_index], 1, 1);
    }
    // The reference's completion branch passes an uninitialized temporary.
    // Keep the message at the same valid averaged position as earlier counts.
    AddGameMsgCount(&average, *counter, 10, 200, 100, 30, 0.75f);
}

i32 EffectOffProgress_Update(LEVEL_PROGRESS_s *progress, char *name, i32 visible) {
    if (name == NULL || progress == NULL || NuStrLen(name) > 15)
        return 0;
    for (i32 i = 0; i < 12; ++i) {
        if (NuStrICmp(progress->disabled_effect_names[i], name) == 0) {
            if (visible != 0) {
                progress->disabled_effect_names[i][0] = '\0';
                return 2;
            }
            return 3;
        }
    }
    if (visible != 0)
        return 0;
    for (i32 i = 0; i < 12; ++i) {
        if (progress->disabled_effect_names[i][0] == '\0') {
            NuStrCpy(progress->disabled_effect_names[i], name);
            break;
        }
    }
    return 1;
}
