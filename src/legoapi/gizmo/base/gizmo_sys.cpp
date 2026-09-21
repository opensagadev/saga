#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "globals.h"
#include "gameapi/edtools/edfile.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizflow.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/world/area.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuspecial.h"

#include <stdio.h>
#include <string.h>
struct FLOWBOX_s;

i32 gizmoerrorlogsize = 0x800;

GIZMOSYS *CreateGizmoSys(void *world, VARIPTR *buf, VARIPTR *buf_end) {
    GIZMOSYS *gizmo_sys = NULL;

    if (gizmotypes != NULL) {
        gizmo_sys = reinterpret_cast<GIZMOSYS *>(GizmoBufferAlloc(buf, buf_end, sizeof(GIZMOSYS)));
        if (gizmo_sys != NULL) {
            gizmo_sys->sets = reinterpret_cast<GIZMOSET *>(
                GizmoBufferAlloc(buf, buf_end, gizmotypes->count * static_cast<i32>(sizeof(GIZMOSET))));

            if (gizmo_sys->sets != NULL) {
                GIZMOSET *set = gizmo_sys->sets;
                GIZMOTYPE *type = gizmotypes->types;
                for (i32 type_index = 0; type_index < gizmotypes->count; ++type_index, ++type, ++set) {
                    set->type = type;

                    if (type->fns.get_max_gizmos_fn != NULL) {
                        set->max_count = type->fns.get_max_gizmos_fn(world);
                    }
                    if (set->max_count != 0) {
                        set->gizmos = reinterpret_cast<GIZMO *>(
                            GizmoBufferAlloc(buf, buf_end, set->max_count * static_cast<i32>(sizeof(GIZMO))));
                    }
                    if (type->fns.reserve_buffer_space_fn != NULL) {
                        set->unknown = type->fns.reserve_buffer_space_fn(world);
                    }
                }
            }

            if (gizmoerrorlogsize != 0) {
                gizmo_sys->error_log = reinterpret_cast<char *>(GizmoBufferAlloc(buf, buf_end, gizmoerrorlogsize));
            }
        }
    }

    return gizmo_sys;
}
void LoadGizmoSys(GIZMOSYS_s *gizmo_sys, void *world, char *config_file) {
    if (gizmo_sys != NULL) {
        gizmo_sys->flags |= GIZMOSYS_FLAG_LOADING;
        if (gizmo_sys->error_log != NULL) {
            memset(gizmo_sys->error_log, 0, gizmoerrorlogsize);
        }
        gizmo_sys->flags &= ~3;

        if (gizmotypes->count != 0) {
            char gizmo_name[32];
            char path[256];
            sprintf(path, "%s.giz", config_file);

            EdFileSetMedia(1);
            if (EdFileOpen(path, NUFILE_READ) != 0) {
                EdFileReadInt();
                i32 name_length = EdFileReadInt();
                while (name_length != 0) {
                    memset(gizmo_name, 0, sizeof(gizmo_name));
                    EdFileRead(gizmo_name, name_length);

                    i32 data_length = EdFileReadInt();
                    i32 type_id = GizmoGetTypeIDByName(gizmo_sys, gizmo_name);
                    if (data_length > 0 && type_id >= 0 && type_id < gizmotypes->count &&
                        gizmotypes->types[type_id].fns.load_fn != NULL &&
                        gizmotypes->types[type_id].fns.load_fn(world, gizmo_sys->sets[type_id].unknown) != 0) {
                        name_length = EdFileReadInt();
                        continue;
                    }

                    while (data_length != 0) {
                        EdFileReadChar();
                        --data_length;
                    }
                    name_length = EdFileReadInt();
                }
                EdFileClose();
            }

            GIZMOTYPE *type = gizmotypes->types;
            GIZMOSET *set = gizmo_sys->sets;
            for (i32 type_id = 0; type_id < gizmotypes->count; ++type_id, ++type, ++set) {
                if (type->fns.post_load_fn != NULL) {
                    type->fns.post_load_fn(world, set->unknown);
                }
            }
        }

        gizmo_sys->flags &= ~GIZMOSYS_FLAG_LOADING;
    }
}
void Hub_LoadAndFixUpMiniKits(WORLDINFO *world, VARIPTR *buf, VARIPTR *buf_end) {
    buf->addr = ALIGN(buf->addr, 4);
    world->hub_minikits = reinterpret_cast<HUBMINIKIT_s *>(buf->addr);
    buf->addr = ALIGN(buf->addr + static_cast<usize>(AREACOUNT) * sizeof(HUBMINIKIT_s), 4);
    world->minikit_pieces_buf = reinterpret_cast<HUBMINIKITPIECES_s **>(buf->addr);
    memset(world->minikit_pieces_buf, 0, static_cast<usize>(AREACOUNT) * sizeof(*world->minikit_pieces_buf));
    buf->addr += static_cast<usize>(AREACOUNT) * sizeof(*world->minikit_pieces_buf);

    for (i32 i = 0; i < AREACOUNT; ++i) {
        if ((ADataList[i].flags & AREAFLAG_MINIKIT) == 0 || ADataList[i].minikit_id == -1) {
            continue;
        }
        world->minikit_pieces_buf[i] = reinterpret_cast<HUBMINIKITPIECES_s *>(buf->addr);
        buf->addr += 0x18;
        MiniKit_Load(reinterpret_cast<MINIKIT *>(world->minikit_pieces_buf[i]), ADataList[i].minikit_id, buf, buf_end,
                     NULL);
        MINIKIT *minikit = reinterpret_cast<MINIKIT *>(world->minikit_pieces_buf[i]);
        if (minikit->gscn == NULL) {
            world->minikit_pieces_buf[i] = NULL;
            buf->addr -= 0x18;
        } else {
            MiniKit_InitPieces(minikit, 10, buf, buf_end);
            minikit->field_0x9 = static_cast<i8>(i);
        }
    }
}
void MiniKit_Load(MINIKIT *minikit, i32 id, VARIPTR *buf, VARIPTR *buf_end, void *param) {
    (void)param;
    minikit->gscn = NULL;
    minikit->field_0x4 = NULL;
    minikit->field_0x8 = 0;
    minikit->field_0x9 = -1;
    minikit->id = static_cast<i16>(id);
    if (id != -1) {
        char path[268];
        NuStrCpy(path, const_cast<char *>("chars\\minikits\\"));
        NuStrCat(path, CDataList[id].file);
        NuStrCat(path, const_cast<char *>("\\"));
        NuStrCat(path, CDataList[id].file);
        NuStrCat(path, const_cast<char *>(".gsc"));
        buf->addr = ALIGN(buf->addr, 4);
        minikit->gscn = NuGScnRead(buf, *buf_end, path);
    }
}
void MiniKit_InitPieces(MINIKIT *minikit, i32 count, VARIPTR *buf, VARIPTR *buf_end) {
    (void)buf_end;
    if (minikit->gscn == NULL) {
        minikit->field_0x9 = -1;
        return;
    }

    static const char *const direction_names[] = {"NegX", "PosX", "NegY", "PosY", "NegZ", "PosZ"};
    u8 direction_counts[6] = {};
    char name[76];

    buf->addr = ALIGN(buf->addr, 4);
    minikit->field_0x4 = reinterpret_cast<void *>(buf->addr);
    minikit->field_0x8 = 0;

    for (i32 index = 0; index < count; ++index) {
        for (u8 direction = 0; direction < 6; ++direction) {
            sprintf(name, "%s_%s_%i", CDataList[minikit->id].file, direction_names[direction], index);
            HUBMINIKITPIECE_s *piece =
                &reinterpret_cast<HUBMINIKITPIECE_s *>(minikit->field_0x4)[minikit->field_0x8];
            if (NuSpecialFind(minikit->gscn, &piece->special, name, 1) == 0) {
                continue;
            }
            piece->matrix = *NuSpecialGetInstanceMtx(&piece->special);
            piece->direction = direction;
            piece->direction_index = direction_counts[direction]++;
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
void GizmoSysAddGizmos(GIZMOSYS_s *gizmo_sys, GIZFLOW_s *giz_flow, void *world) {
    if (gizmotypes != NULL && gizmo_sys != NULL) {
        GIZMOTYPE *type = gizmotypes->types;
        GIZMOSET *set = gizmo_sys->sets;
        for (i32 type_id = 0; type_id < gizmotypes->count; ++type_id, ++type, ++set) {
            ResetGizmoType(gizmo_sys, type_id, NULL);
            if (type->fns.add_gizmos_fn != NULL) {
                type->fns.add_gizmos_fn(gizmo_sys, type_id, world, set->unknown);
            }
        }
    }

    if (giz_flow != NULL && giz_flow->pointers_need_reset != 0) {
        ResetGizFlowPointers(giz_flow);
    }
}
