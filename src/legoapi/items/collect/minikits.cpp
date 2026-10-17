// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/items/collect/minikits.h"
#include "globals.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/render/core/render.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/collect/torpedo.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nu3d/nucamera.h"
#include "gameframework/saveload.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/world/mission.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"

HUBMINIKITPIECES_s **Char_MiniKit;
static VARIPTR minikits_savecharacterbufferptr;
extern "C" VARIPTR characterbuffer_ptr;
extern "C" VARIPTR characterbuffer_end;

char *LEGOASCII_BIGARROW = NULL;

extern i32 LEGOOBJ_MINIKIT, LEGOOBJ_CHARKIT;
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);

struct GAMEMESSAGE_s;
struct nuhspecial_s;

#if defined(__i386__)
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone))
#endif
static NU_READ_GRAPHICS_DATA_ATTR NUGSCN *NuReadGraphicsData(VARIPTR *buf, VARIPTR *buf_end, char *path,
                                                             char *scene_data)
    __asm__("_ZL18NuReadGraphicsDataP9variptr_uS0_PcS1_S1_.isra.104");

f32 KITPOSX = -0.725f;
f32 KITPOSY = -0.7f;
f32 KITPOS2X = -1.275f;
f32 KITPOS2Y = -1.3f;
f32 PANEL_MINIKITSCALE = 0.25f;
f32 PANEL_MINIKITY = 0.03f;
f32 PANEL_MINIKITCOUNTSCALE = 0.5f;
f32 PANEL_MINIKITCOUNTY = -0.1f;
// DrawPanel HUD globals referenced by the cantina-bar-patrons WIP. Initial
// red-brick positions come from the Game init values in legogame/game.cpp.
i32 LEGOOBJ_CHARKIT = -1;
i32 LEGOOBJ_MINIKIT = -1;

void MiniKits_Init(variptr_u *buffer, variptr_u *) {
    const usize aligned = (buffer->addr + 3) & ~static_cast<usize>(3);
    buffer->addr = aligned;
    Char_MiniKit = reinterpret_cast<HUBMINIKITPIECES_s **>(aligned);
    buffer->addr = aligned + static_cast<usize>(CHARCOUNT) * sizeof(*Char_MiniKit);
}

void MiniKit_Load(MINIKIT *minikit, i32 id, VARIPTR *buf, VARIPTR *buf_end, void *param) {
    (void)param;
    minikit->gscn = NULL;
    minikit->field_0x4 = NULL;
    minikit->field_0x8 = 0;
    minikit->field_0x9 = -1;
    minikit->id = static_cast<i16>(id);
    if (id == -1)
        return;
    char path[256];
    NuStrCpy(path, const_cast<char *>("chars\\minikits\\"));
    NuStrCat(path, CDataList[id].file);
    NuStrCat(path, const_cast<char *>("\\"));
    NuStrCat(path, CDataList[id].file);
    NuStrCat(path, const_cast<char *>(".gsc"));
    buf->addr = ALIGN(buf->addr, 4);
    minikit->gscn = NuGScnRead(buf, *buf_end, path);
}

void MiniKit_InitPieces(MINIKIT *minikit, i32 count, VARIPTR *buf, VARIPTR *buf_end) {
    (void)buf_end;
    if (minikit->gscn == NULL) {
        minikit->field_0x9 = -1;
        return;
    }

    static const char *const direction_names[] = {"NegX", "PosX", "NegY", "PosY", "NegZ", "PosZ"};
    i32 direction_counts[6] = {};
    char name[76];

    buf->addr = ALIGN(buf->addr, 4);
    minikit->field_0x8 = 0;
    minikit->field_0x4 = reinterpret_cast<void *>(buf->addr);

    for (i32 index = 0; index < count; ++index) {
        for (u8 direction = 0; direction < 6; ++direction) {
            sprintf(name, "%s_%s_%i", CDataList[minikit->id].file, direction_names[direction], index);
            HUBMINIKITPIECE_s *piece = &reinterpret_cast<HUBMINIKITPIECE_s *>(minikit->field_0x4)[minikit->field_0x8];
            if (NuSpecialFind(minikit->gscn, &piece->special, name, 1) == 0) {
                continue;
            }
            piece = &reinterpret_cast<HUBMINIKITPIECE_s *>(minikit->field_0x4)[minikit->field_0x8];
            piece->matrix = *NuSpecialGetInstanceMtx(&piece->special);
            reinterpret_cast<HUBMINIKITPIECE_s *>(minikit->field_0x4)[minikit->field_0x8].direction = direction;
            reinterpret_cast<HUBMINIKITPIECE_s *>(minikit->field_0x4)[minikit->field_0x8].direction_index =
                direction_counts[direction]++;
            ++minikit->field_0x8;
        }
    }

    sprintf(name, "%s_shadow", CDataList[minikit->id].file);
    NuSpecialFind(minikit->gscn, reinterpret_cast<nuhspecial_s *>(reinterpret_cast<u8 *>(minikit) + 0xc), name, 1);

    if (minikit->field_0x8 == 0) {
        minikit->field_0x4 = NULL;
        return;
    }
    buf->addr += static_cast<usize>(minikit->field_0x8) * sizeof(HUBMINIKITPIECE_s);
}

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

void DrawMiniKitCount(float position, float scale, i32 count, i32 maximum) {
    const i32 model = ChallengeMode != 0 ? LEGOOBJ_CHARKIT : LEGOOBJ_MINIKIT;
    if (model == -1 || position <= 0.0f)
        return;
    const f32 blend = NuTrigTable[(static_cast<i32>(position * 16384.0f) >> 1) & 0x7fff];
    const f32 x = (KITPOSX - KITPOS2X) * blend + KITPOS2X;
    const f32 y = (KITPOSY - KITPOS2Y) * blend + KITPOS2Y;
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if (world->lev_objs[model].active != 0) {
        const u16 rotation =
            static_cast<u16>(static_cast<i32>(NuFmod(GlobalTimer.time_elapsed, 4.0f) * 0.25f * 65536.0f));
        const i16 tilt = static_cast<i16>(1820.0f * NuTrigTable[rotation & 0x7fff]);
        const f32 size = scale * PANEL_MINIKITSCALE;
        DrawPanel3DObjectNoAlpha(x, PANEL_MINIKITY + y, 1.0f, size, size, size, tilt, rotation, 0,
                                 &world->lev_objs[model].special, 2);
    }
    char text[32];
    sprintf(text, "%i/%i", count, maximum);
    const f32 size = scale * PANEL_MINIKITCOUNTSCALE;
    Text3D(text, x, y + PANEL_MINIKITCOUNTY, 1.0f, size, size, size, 0, 255, 0, 127);
}

void MiniKit_GameMsg_Update(GAMEMESSAGE_s *message) {
    i32 angle = static_cast<i32>((NuFmod(GlobalTimer.time_elapsed, 4.0f) * 0.25f) * 65536.0f);
    message->rotation_y = static_cast<u16>(angle);
    message->field_0xe0 = static_cast<u16>(NuTrigTable[angle & 0x7fff] * 1820.0f);
}

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

