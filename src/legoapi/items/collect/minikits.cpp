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
struct nunativegscene_s;

HUBMINIKITPIECES_s **Char_MiniKit;
static VARIPTR minikits_savecharacterbufferptr;
extern "C" VARIPTR characterbuffer_ptr;
extern "C" VARIPTR characterbuffer_end;

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

void CollectMinikit(nuvec_s *position, char *name, i32);

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
i32 MatrixReflection(NUMTX *, i32, f32, f32, NUMTX *);
void CharMiniKit_Draw(i32 id, numtx_s *matrix, i32 reflection_axis, float reflection_plane, float reflection_height) {
    NUMTX_ALIGNED16 draw_matrix;
    if (Char_MiniKit == NULL)
        return;
    HUBMINIKITPIECES_s *kit = Char_MiniKit[id];
    if (kit != NULL && kit->piece_count != 0) {
        bool reflection = reflection_axis != 0 && reflection_plane != 2000000.0f;
        for (i32 i = 0; i < kit->piece_count; ++i) {
            HUBMINIKITPIECE_s *piece = &kit->pieces[i];
            if (NuSpecialExistsFn(&piece->special) == 0)
                continue;
            piece = &kit->pieces[i];
            draw_matrix = piece->matrix;
            NuMtxMul(&draw_matrix, &draw_matrix, matrix);
            piece = &kit->pieces[i];
            NuSpecialDrawAt(&piece->special, &draw_matrix);
            if (reflection) {
                NUMTX reflected;
                if (MatrixReflection(&draw_matrix, reflection_axis, reflection_plane, reflection_height, &reflected) !=
                    0) {
                    NuRndrStartReflectionRender(0);
                    piece = &kit->pieces[i];
                    NuSpecialDrawAt(&piece->special, &reflected);
                    NuRndrEndReflectionRender();
                }
            }
        }
    }
}

extern "C" void PlaySfx(char *, nuvec_s *);
void DrawMiniKitCount(f32, f32, i32, i32);

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

extern i32 LEGOOBJ_MINIKIT, LEGOOBJ_CHARKIT;
void EndChallenge(i32, i32);
void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);
void MiniKit_GameMsg_End(GAMEMESSAGE_s *message) {
    MiniKitScale = 2.0f;
    if (WorldInfo_CurrentlyActive()->area != NULL && static_cast<i16>(message->icon) != -1) {
        if (static_cast<i16>(message->icon) == LEGOOBJ_MINIKIT) {
            ++AreaGlobals.values.field_0x14;
            if (AreaGlobals.values.field_0x14 > AreaGlobals.values.field_0x0c)
                AreaGlobals.values.field_0x14 = AreaGlobals.values.field_0x0c;
        } else if (static_cast<i16>(message->icon) == LEGOOBJ_CHARKIT &&
                   AreaGlobals.values.field_0x20 < AreaGlobals.values.field_0x1c) {
            ++AreaGlobals.values.field_0x20;
            if (AreaGlobals.values.field_0x20 == AreaGlobals.values.field_0x1c && AreaGlobals.values.field_0x1c > 9)
                EndChallenge(2, 1);
        }
    }
    GameAudio_PlaySfx(0x26, NULL, 0, 0);
    NewRumbleAllPlayers(0.6f, 0.0f, 0, 0);
    GameCam_Judder(GameCam, -0.2f, 0, NULL);
}

void ResetMinikitCounter();

extern i16 id_SLAVE1, tALLMINIKITSBUILT;

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

    if (world->area != NULL && (world->area->flags & 5) == 5) {
        if (Char_MiniKit != NULL) {
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
    } else {
        for (i32 i = 0; i < AREACOUNT; ++i) {
            HUBMINIKITPIECES_s *entry = world->minikit_pieces_buf[i];
            if (entry != NULL && entry->scene != NULL) {
                NuGScnRemove(entry->scene);
                world->minikit_pieces_buf[i]->scene = NULL;
            }
        }
    }
}

void MiniKit_GameMsg_Update(GAMEMESSAGE_s *message) {
    i32 angle = static_cast<i32>((NuFmod(GlobalTimer.time_elapsed, 4.0f) * 0.25f) * 65536.0f);
    message->rotation_y = static_cast<u16>(angle);
    message->field_0xe0 = static_cast<u16>(NuTrigTable[angle & 0x7fff] * 1820.0f);
}

void IncrementMinikitCounter(GameObject_s *);

