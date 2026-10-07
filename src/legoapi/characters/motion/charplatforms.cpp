#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/light/surfaces.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numath/nufloat.h"
#include <string.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" void NewTerrPlatformsOff(void);
extern TERRSET *CurTerr;
f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);

i32 SkinFlipTab[8] = {0, 1, 2, 3, 0, 2, 1, 3};
f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);
extern "C" void NewTerrPlatformsOff();

void SkinPlatform(terrsitu_s *terrain_group, unsigned char *buffer, PLATSKININFO *info) {
    TERRAIN_GROUP *group = reinterpret_cast<TERRAIN_GROUP *>(terrain_group);
    if (CurTerr == NULL || static_cast<u32>(group->chunk_type) > 1)
        return;
    TERRAIN_SHAPE_BATCH *input = static_cast<TERRAIN_SHAPE_BATCH *>(group->data);
    TERRAIN_SHAPE_BATCH *output = reinterpret_cast<TERRAIN_SHAPE_BATCH *>(buffer);
    while (input->marker >= 0) {
        output->marker = input->marker;
        output->shape_count = input->shape_count;
        TERRAIN_SHAPE *source = reinterpret_cast<TERRAIN_SHAPE *>(input + 1);
        TERRAIN_SHAPE *destination = reinterpret_cast<TERRAIN_SHAPE *>(output + 1);
        f32 min_x = 123456792.0f, min_z = 123456792.0f;
        f32 max_x = -123456792.0f, max_z = -123456792.0f;
        TERRAIN_SHAPE *const first_shape = source;
        const i32 shape_count = input->shape_count;
        TERRAIN_SHAPE *const end_shape = shape_count > 0 ? source + shape_count : source;
        for (; source != end_shape; ++source, ++destination) {
            i32 last_vertex = source->normals[1].y > 65535.0f ? 2 : 3;
            memcpy(destination, source, sizeof(TERRAIN_SHAPE));
            f32 shape_min_x = 123456792.0f, shape_min_y = 123456792.0f, shape_min_z = 123456792.0f;
            f32 shape_max_x = -123456792.0f, shape_max_y = -123456792.0f, shape_max_z = -123456792.0f;
            for (i32 vertex = last_vertex; vertex >= 0; --vertex) {
                i32 source_vertex = SkinFlipTab[vertex + info->mirrored * 4];
                NUVEC &v = destination->vectors[vertex];
                v = TerrainSkin(info, &source->vectors[source_vertex], -1.0f, info->flags);
                v.x -= info->matrix->m30;
                v.y -= info->matrix->m31;
                v.z -= info->matrix->m32;
                min_x = MIN(v.x, min_x);
                max_x = MAX(v.x, max_x);
                min_z = MIN(v.z, min_z);
                max_z = MAX(v.z, max_z);
                shape_min_x = MIN(v.x, shape_min_x);
                shape_min_y = MIN(v.y, shape_min_y);
                shape_min_z = MIN(v.z, shape_min_z);
                shape_max_x = MAX(v.x, shape_max_x);
                shape_max_y = MAX(v.y, shape_max_y);
                shape_max_z = MAX(v.z, shape_max_z);
            }
            destination->min_x = shape_min_x - 0.05f;
            destination->min_y = shape_min_y - 0.05f;
            destination->min_z = shape_min_z - 0.05f;
            destination->max_x = shape_max_x + 0.05f;
            destination->max_y = shape_max_y + 0.05f;
            destination->max_z = shape_max_z + 0.05f;
            if (source->normals[1].y < 65535.0f) {
                NUVEC a, b;
                a.x = destination->vectors[1].x - destination->vectors[3].x;
                a.y = destination->vectors[1].y - destination->vectors[3].y;
                a.z = destination->vectors[1].z - destination->vectors[3].z;
                b.x = destination->vectors[2].x - destination->vectors[3].x;
                b.y = destination->vectors[2].y - destination->vectors[3].y;
                b.z = destination->vectors[2].z - destination->vectors[3].z;
                NUVEC &n = destination->normals[1];
                n = TerCrossProduct(&a, &b);
                f32 length = NuFsqrt((n.x * n.x + n.y * n.y) + n.z * n.z);
                f32 inverse = length == 0.0f ? 0.0f : 1.0f / length;
                n.x *= inverse;
                n.y *= inverse;
                n.z = inverse * n.z;
            }
            {
                NUVEC a, b;
                a.x = destination->vectors[2].x - destination->vectors[0].x;
                a.y = destination->vectors[2].y - destination->vectors[0].y;
                a.z = destination->vectors[2].z - destination->vectors[0].z;
                b.x = destination->vectors[1].x - destination->vectors[0].x;
                b.y = destination->vectors[1].y - destination->vectors[0].y;
                b.z = destination->vectors[1].z - destination->vectors[0].z;
                NUVEC &n = destination->normals[0];
                n = TerCrossProduct(&a, &b);
                f32 length = NuFsqrt((n.x * n.x + n.y * n.y) + n.z * n.z);
                f32 inverse = length == 0.0f ? 0.0f : 1.0f / length;
                n.x *= inverse;
                n.y *= inverse;
                n.z = inverse * n.z;
            }
        }
        if (source != first_shape) {
            min_x -= 0.05f;
            min_z -= 0.05f;
            max_x += 0.05f;
            max_z += 0.05f;
        }
        output->min_x = min_x;
        output->max_x = max_x;
        output->min_z = min_z;
        output->max_z = max_z;
        input = reinterpret_cast<TERRAIN_SHAPE_BATCH *>(source);
        output = reinterpret_cast<TERRAIN_SHAPE_BATCH *>(destination);
    }
    output->marker = -1;
    output->shape_count = -1;
    group->data = buffer;
}

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

    while (NuFParGetLine(parser) != 0) {
        if (NuFParGetWord(parser) == 0) {
            break;
        }
        if (NuStrICmp(parser->word_buf, const_cast<char *>("char_platform")) != 0 || NuFParGetWord(parser) == 0) {
            continue;
        }

        system = world->char_platform_sys;
        CHARPLATFORM_s *platform = &system->platforms[system->platform_count];
        platform->object_id = CharIDFromName(parser->word_buf);
        if (platform->object_id == -1 || NuFParGetWord(parser) == 0) {
            continue;
        }
        if (NuSpecialFind(world->current_gscn, &platform->special, parser->word_buf, 1) == 0) {
            continue;
        }

        system = world->char_platform_sys;
        platform->platform_id = -1;
        platform->object = NULL;
        ++system->platform_count;
    }

    NuFParDestroy(parser);
    system = world->char_platform_sys;
    if (system->platform_count > 0) {
        world->giz_buffer.addr = ALIGN(reinterpret_cast<usize>(&system->platforms[system->platform_count]), 4);
    } else {
        world->char_platform_sys = NULL;
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
            if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001 || object->apiobj.field_0x287 != 0 ||
                object->field_0x107c != system->platforms[i].platform_id)
                continue;
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
        NuSpecialSetVisibility(&system->platforms[i].special, visible);
    }
}

f32 FindReflectionNoPlatforms(nuvec_s *position) {
    NewTerrPlatformsOff();
    f32 height = GameShadow(NULL, position, 5.0f, -1);
    if (height != 2000000.0f) {
        u32 surface = static_cast<u32>(ShadowInfo());
        if (surface < 32 && (TerSurface[surface].flags & 2) != 0) {
            return height;
        }
    }
    return 2000000.0f;
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
