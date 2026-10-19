// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/render/fx.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/world/level.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/config/cheat.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/numath/numtx_inline.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "legoapi/render/fx/edsplines.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "legoapi/render/core/gameliball.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include <stddef.h>
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/misc/utilities.h"

extern u16 TargetDeg_Near, TargetDeg_Mid, TargetDeg_Far;
extern f32 TargetDist_Near2, TargetDist_Mid2;
static i32 PDEBCOUNT;
static char **PDebNameList;

extern "C" f32 brickimpactwait;

// NuFileExists @nufile (nucore_plain.cpp).

void InitPartTable(char **names) {
    PDEBCOUNT = 0;
    PDebNameList = names;
    if (names != NULL) {
        while (names[PDEBCOUNT] != NULL) {
            ++PDEBCOUNT;
        }
    }
}

void *InitPartDebris(VARIPTR *buf, VARIPTR *, i32 capacity, i32 named_count, char **names, i32 page) {
    PARTDEBSYS_s *system = static_cast<PARTDEBSYS_s *>(BUFFER_ALLOC(buf, sizeof(PARTDEBSYS_s), 16));
    if (system == NULL)
        return system;
    system->entries = NULL;
    system->capacity = capacity;
    system->named_count = named_count;
    system->entries = static_cast<PARTDEBENTRY_s *>(BUFFER_ALLOC(buf, capacity * sizeof(PARTDEBENTRY_s), 16));
    if (system->entries == NULL)
        return NULL;
    memset(system->entries, -1, capacity * sizeof(PARTDEBENTRY_s));
    i32 i = 0;
    if (names != NULL) {
        for (; i < system->named_count; ++i) {
            NuStrCpy(system->entries[i].name, names[i]);
            system->entries[i].type_id = -1;
            system->entries[i].type_id = PARTLookupTypePageOnly(system->entries[i].name, page);
        }
    }
    for (i32 type = 0; type < 128 && i < system->capacity; ++type) {
        system->entries[i].type_id = -1;
        if (__builtin_expect(part_types[type].variant_count > 0, 1)) {
            NuStrCpy(system->entries[i].name, part_types[type].name);
            system->entries[i].type_id = PARTLookupTypePageOnly(system->entries[i].name, page);
            ++i;
        }
    }
    for (; i < system->capacity; ++i)
        system->entries[i].type_id = -1;
    return system;
}

i32 FindPartDebris(PARTDEBSYS_s *system, char *name) {
    if (system != NULL) {
        for (i32 i = system->named_count; i < system->capacity; ++i) {
            if (NuStrICmp(name, system->entries[i].name) == 0)
                return system->entries[i].type_id;
        }
    }
    return -1;
}

void LoadPartFile(WORLDINFO *world) {
    char path[256];
    world->page_part = -1;
    edpartSetParticlePage(world->page_pp);

    if ((world->current_level->flags & (LEVEL_OUTRO | LEVEL_MIDTRO | LEVEL_INTRO)) == 0) {
        sprintf(path, "%s.par", world->config_file);
        if (NuFileExists(path) > 0)
            world->page_part = edpartLoadPage(path, 1, world->current_gscn);
        world->part_debris_sys = static_cast<PARTDEBSYS_s *>(
            InitPartDebris(&world->giz_buffer, &world->unknown_0108, 0x40, PDEBCOUNT, PDebNameList, world->page_part));
    }
}

void Parts_Start(WORLDINFO_s *world) {
    if (world->page_part != -1) {
        edpartStartPage(world->page_part);
    }
}

void Parts_Stop(WORLDINFO_s *world) {
    if (world->page_part != -1) {
        edpartStopPage(world->page_part);
    }
}

void AddPartDebris(PARTDEBSYS_s *system, i32 index, nuvec_s *position) {
    if (index >= 0 && system != NULL && index < PDEBCOUNT) {
        const i32 type = system->entries[index].type_id;
        if (type != -1) {
            AddFiniteShotPART(type, position, 1);
        }
    }
}

void PartImpact_Brick(PART_s *part) {
    if (brickimpactwait <= 0.0f) {
        f32 speed = NuVecMag(&part->velocity);
        if (speed > 0.0f) {
            if (speed > 1.0f)
                speed = 1.0f;
            f32 volume = NU_SIN_LUT(static_cast<i32>((1.0f - speed) * 16384.0f + 16384.0f));
            PlaySfxByIdAndSetVolume(GetSfxId("LegoSingle"), &part->position, volume);
            brickimpactwait = 0.1f;
        }
    }
}

void PartStop_Flickerer(PART_s *part) {
    part->field_100 = 5.0f;
    if ((part->field_20a & 1) != 0)
        part->field_100 = 2.5f;
}

i32 PartDraw_Flickerer(PART_s *part) {
    if (part->field_100 > 0.0f) {
        if (PickupFlickerFrame % PickUpFlickerFrames < PickUpFlickerTest)
            return 1;
        f32 threshold = (part->field_20a & 1) == 0 ? 2.0f : 1.0f;
        return !(part->field_100 < threshold);
    }
    return 1;
}

PART_s *FindIncomingPart(void *owner, NUVEC *position, f32 radius, u32 flags, f32 range) {
    PART_s *part = Part;
    radius *= radius;
    PART_s *nearest = NULL;
    f32 nearest_distance = range > 0.0f ? range : 100.0f;
    for (i32 i = 0; i < MAXPARTS; ++i, ++part) {
        if ((part->active & 1) == 0 || part->owner == owner || (part->flags & flags) != flags)
            continue;
        if (range > 0.0f) {
            f32 distance = NuVecDistSqr(&part->position, position, NULL);
            if (distance < nearest_distance) {
                nearest_distance = distance;
                nearest = part;
            }
        } else {
            f32 speed = NuVecMag(&part->velocity);
            if (speed != 0.0f) {
                NUVEC direction;
                NuVecScale(&direction, &part->velocity, 1.0f / speed);
                f32 reach = speed * 0.5f;
                reach *= reach;
                f32 distance = NuVecDistSqr(&part->position, position, NULL);
                if (distance < reach && LineIntersectSphere(&part->position, &direction, position, radius, NULL)) {
                    nearest_distance = distance;
                    nearest = part;
                }
            }
        }
    }
    return nearest;
}

void MakePartVector(NUVEC *velocity, NUVEC *direction, f32 scale) {
    if (direction == NULL) {
        f32 random = static_cast<f32>(qrand()) * (1.0f / 65535.0f);
        velocity->x = random + random - 1.0f;
        velocity->y = AreaPickupGravity == 0.0f ? 0.0f : static_cast<f32>(qrand()) * (1.0f / 65535.0f) + 1.0f;
        random = static_cast<f32>(qrand()) * (1.0f / 65535.0f);
        velocity->z = random + random - 1.0f;
        NuVecScale(velocity, velocity, AreaPickupScale);
    } else {
        FindAnglesZX(direction, NULL, NULL);
        *velocity = v010;
        const i32 z_random = qrand();
        const i32 x_random = qrand();
        NuVecRotateZ(velocity, velocity, static_cast<u16>(temp_zrot + z_random / 6 - 0x1555));
        NuVecRotateX(velocity, velocity, static_cast<u16>(temp_xrot + x_random / 6 - 0x1555));
        NuVecScale(velocity, velocity, scale);
    }
}

PART_s *TargetPart(GameObject_s *object, nuvec_s *position, nuvec_s *direction, f32 radius, f32 range_squared,
                   i32 directional, i32 bolt_id) {
    NUVEC delta;
    NUVEC aim = *direction;
    BOLTTYPE_s *bolt = BoltType_FindByID(bolt_id, WORLD);
    PART_s *part = Part;
    f32 max_x = position->x + radius, min_x = position->x - radius;
    f32 min_z = position->z - radius, max_z = position->z + radius;
    if (directional != 0) {
        aim = *direction;
        if ((bolt->field_60 & 0x20000) != 0) {
            aim.y = 0.0f;
            NuVecNorm(&aim, &aim);
        }
    }
    PART_s *best = NULL;
    PART_s *previous = NULL;
    f32 nearest_distance = range_squared;
    for (i32 i = 0; i < MAXPARTS; ++i, ++part) {
        if ((part->active & 1) == 0 || ((part->flags & 0x8000000) == 0 && (part->flags & 0xa) != 0xa))
            continue;
        if (part->position.x < min_x || part->position.x > max_x || part->position.z < min_z ||
            part->position.z > max_z)
            continue;

        f32 distance = NuVecDistSqr(&part->position, position, &delta);
        if (!(range_squared > distance))
            continue;
        if (directional == 0)
            NuVecRotateY(&aim, &v001, NuAtan2D(part->position.x - position->x, part->position.z - position->z));
        if ((bolt->field_60 & 0x20000) != 0)
            delta.y = 0.0f;
        NuVecNorm(&delta, &delta);
        f32 dot = NuVecDot(&delta, &aim);
        u16 angle;
        if (TargetDist_Near2 > distance && directional != 0)
            angle = TargetDeg_Near;
        else if (TargetDist_Mid2 > distance)
            angle = TargetDeg_Mid;
        else
            angle = TargetDeg_Far;
        if (!(dot > NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff]) || !(nearest_distance > distance))
            continue;
        if (object->attack_part_target == part)
            previous = part;
        else {
            nearest_distance = distance;
            best = part;
        }
    }
    return best != NULL ? best : previous;
}

PART_s *Part_FindFromHSpecial(nuhspecial_s *special) {
    if (special != NULL) {
        PART_s *part = Part;
        for (i32 i = 0; i < MAXPARTS; ++i, ++part) {
            if ((part->active & 1) != 0 && NuSpecialCompare(&part->special, special))
                return part;
        }
    }
    return NULL;
}

