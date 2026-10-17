#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/render/core/terrain.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include <string.h>

struct AIROW_s;
struct nunativegscene_s;

void CharPlatforms_Reset(CHARPLATFORMSYS_s *system) {
    if (system == NULL) {
        return;
    }

    GameObject_s *object_cursor = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++object_cursor) {
        if ((object_cursor->apiobj.field_0x1f8 & 1) != 0) {
            object_cursor->field_0x107c = -1;
        }
    }

    for (i32 i = 0; i < system->platform_count; ++i) {
        NuSpecialSetVisibility(&system->platforms[i].special, 0);
        system->platforms[i].platform_id = FindPlatInst(NuSpecialGetInstanceix(&system->platforms[i].special));
        system->platforms[i].object = NULL;
        if (system->platforms[i].platform_id != -1) {
            GameObject_s *object = FindGameObject(system->platforms[i].object_id, 0, 1, 0, 1);
            if (object != NULL) {
                object->field_0x107c = system->platforms[i].platform_id;
                system->platforms[i].object = object;
            }
        }
    }
}

void CharPlatforms_Update(CHARPLATFORMSYS_s *system) {
    if (system == NULL)
        return;
    for (i32 i = 0; i < system->platform_count; ++i) {
        if (system->platforms[i].object_id == -1)
            continue;
        i32 visible = 0;
        GameObject_s *object = Obj;
        for (i32 j = 0; j < HIGHGAMEOBJECT; ++j, ++object) {
            if ((object->apiobj.field_0x1f8 & 0x1001) == 0x1001 && object->apiobj.field_0x287 == 0 &&
                object->field_0x107c == system->platforms[i].platform_id) {
                nuhspecial_s *special = &system->platforms[i].special;
                NUMTX *matrix = NuSpecialGetDrawMtx(special);
                if (matrix != NULL) {
                    *matrix = object->apiobj.field_0xb8;
                    if (GameTimer.time_elapsed * 2.0f < 1.0f) {
                        matrix->m31 -= 1.0f - GameTimer.time_elapsed * 2.0f;
                    }
                    NuSpecialUpdate(special);
                }
                visible = 1;
                break;
            }
        }
        NuSpecialSetVisibility(&system->platforms[i].special, visible);
    }
}

void CharPlatforms_Configure(WORLDINFO_s *world, char *config) {
    world->char_platform_sys = NULL;
    if (world->current_gscn == NULL) {
        return;
    }

    NUFPAR *parser = NuFParCreateMem(const_cast<char *>("CharPlatforms"), config, 0xffff);
    if (parser == NULL) {
        return;
    }

    world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
    CHARPLATFORMSYS_s *system = reinterpret_cast<CHARPLATFORMSYS_s *>(world->giz_buffer.void_ptr);
    world->char_platform_sys = system;
    system->field_0x00 = static_cast<i32>(reinterpret_cast<usize>(world->current_gscn));
    system->platform_count = 0;

    while (NuFParGetLine(parser) != 0 && NuFParGetWord(parser) != 0) {
        if (NuStrICmp(parser->word_buf, const_cast<char *>("char_platform")) != 0 || NuFParGetWord(parser) == 0) {
            continue;
        }
        CHARPLATFORM_s *platform = &world->char_platform_sys->platforms[world->char_platform_sys->platform_count];
        platform->object_id = CharIDFromName(parser->word_buf);
        if (platform->object_id == -1 || NuFParGetWord(parser) == 0) {
            continue;
        }
        if (NuSpecialFind(world->current_gscn, &platform->special, parser->word_buf, 1) != 0) {
            platform->platform_id = -1;
            platform->object = NULL;
            ++world->char_platform_sys->platform_count;
        }
    }

    NuFParDestroy(parser);
    system = world->char_platform_sys;
    if (system->platform_count > 0) {
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr + sizeof(CHARPLATFORMSYS_s) +
                                           (system->platform_count - 1) * sizeof(CHARPLATFORM_s),
                                       4);
    } else {
        world->char_platform_sys = NULL;
    }
}

GameObject_s *CharPlatform_FindObjFromPlatID(CHARPLATFORMSYS_s *system, i32 platform_id) {
    if (system == NULL) {
        return NULL;
    }
    for (i32 i = 0; i < system->platform_count; ++i) {
        if (system->platforms[i].object != NULL && system->platforms[i].platform_id == platform_id) {
            return system->platforms[i].object;
        }
    }
    return NULL;
}
