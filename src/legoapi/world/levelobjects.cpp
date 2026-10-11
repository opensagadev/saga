#include "legoapi/world/level.h"
#include "legoapi/render/core/terrain.h"
#include "globals.h"
#include "nu2api/nucore/nustring.h"

#include <string.h>

extern "C" i32 NuSpecialExistsFn(void *);

static i32 LEVELOBJECTMAX;
static char *ExtraLevelObject_NameTable;
static i32 ExtraLevelObject_NameTableSize;
static i32 ExtraLevelObject_NameTableIndex;

void LevelObjects_InitForGame(LEVELOBJECT *tab, VARIPTR *buf, VARIPTR *buf_end, i32 max, i32 name_table_size) {
    LEVOBJREF_FIRSTOBJ = -1;
    LEVOBJREF_LASTOBJ = -1;
    LEVOBJREF_FIRSTREFOBJ = -1;
    LEVOBJREF_LASTREFOBJ = -1;
    LEVELOBJECTMAX = max;
    ObjTabList = tab;
    LEVELSPLINECOUNT = 0;

    for (; tab->kind != 0xff; ++tab, ++LEVELOBJECTCOUNT) {
        if (tab->reflection != 1) {
            if (tab->reflection == 2) {
                if (LEVOBJREF_FIRSTREFOBJ == -1) {
                    LEVOBJREF_FIRSTREFOBJ = LEVELOBJECTCOUNT;
                }
                LEVOBJREF_LASTREFOBJ = LEVELOBJECTCOUNT;
            }
        } else {
            if (LEVOBJREF_FIRSTOBJ == -1) {
                LEVOBJREF_FIRSTOBJ = LEVELOBJECTCOUNT;
            }
            LEVOBJREF_LASTOBJ = LEVELOBJECTCOUNT;
        }
    }

    const i32 object_range = LEVOBJREF_LASTOBJ - LEVOBJREF_FIRSTOBJ;
    const i32 reflection_range = LEVOBJREF_LASTREFOBJ - LEVOBJREF_FIRSTREFOBJ;
    if (object_range > reflection_range) {
        LEVOBJREF_LASTOBJ = LEVOBJREF_FIRSTOBJ + reflection_range;
    } else if (object_range < reflection_range) {
        LEVOBJREF_LASTREFOBJ = LEVOBJREF_FIRSTREFOBJ + object_range;
    }

    if (name_table_size > 0) {
        ExtraLevelObject_NameTable = (char *)buf->u8_ptr;
        ExtraLevelObject_NameTableSize = name_table_size;
        buf->addr += name_table_size;
    }
}

i32 LevelObject_AddExtra(char *name, i32 kind) {
    if (LEVELOBJECTCOUNT < LEVELOBJECTMAX && ExtraLevelObject_NameTable != NULL) {
        i32 nameLen = NuStrLen(name);
        if (nameLen + ExtraLevelObject_NameTableIndex + 1 < ExtraLevelObject_NameTableSize) {
            char *nameDest = ExtraLevelObject_NameTable + ExtraLevelObject_NameTableIndex;
            LEVELOBJECT *obj = &ObjTabList[LEVELOBJECTCOUNT];
            obj->kind = (u8)kind;
            obj->name = nameDest;
            LEVELOBJECTCOUNT++;
            EXTRALEVELOBJECTCOUNT++;
            NuStrCpy(nameDest, name);
            ExtraLevelObject_NameTableIndex += nameLen + 1;
            return 1;
        }
    }
    return 0;
}

void LevObj_FixUpPlatIDs(WORLDINFO_s *world) {
    i32 i;
    LEVEL_OBJECT_RUNTIME *obj;

    if (ObjTabList == NULL || LEVELOBJECTCOUNT <= 0) {
        return;
    }
    for (i = 0; i < LEVELOBJECTCOUNT; i++) {
        obj = &world->lev_objs[i];
        obj->platform_id = -1;
        if (world->terrain != NULL) {
            if (NuSpecialExistsFn(&obj->special)) {
                if (ObjTabList[i].kind == 1) {
                    obj = &world->lev_objs[i];
                    obj->platform_id = FindPlatInst(NuSpecialGetInstanceix(&obj->special));
                }
            }
        }
    }
}

void *LevObj_FindByPlatID(WORLDINFO_s *world, i32 platID) {
    LEVEL_OBJECT_RUNTIME *obj = world->lev_objs;
    for (i32 i = 0; i < LEVELOBJECTCOUNT; i++, obj++) {
        if (obj->platform_id == platID) {
            return obj;
        }
    }
    return NULL;
}
