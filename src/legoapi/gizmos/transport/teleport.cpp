#include "legoapi/gizmos/transport/teleport.h"

#include "decomp.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nufile/nufpar.h"

i32 teleport_gizmotype_id = -1;

static TELEPORT_s *Tel_teleport;
static WORLDINFO_s *Tel_worldinfo;

static void Tel_1_way(nufpar_s *) {
    Tel_teleport->flags |= 1;
}
static void Tel_crawl(nufpar_s *) {
    Tel_teleport->flags |= 4;
}
static void Tel_duration(nufpar_s *parser) {
    TELEPORT_s *teleport = Tel_teleport;
    teleport->duration = NuFParGetFloat(parser);
}
static void Tel_flap(nufpar_s *parser) {
    if (NuFParGetWord(parser) == 0) {
        return;
    }

    if (NuStrIStr(parser->line_buf, "flap1") != NULL) {
        if (NuSpecialFind(Tel_worldinfo->current_gscn, &Tel_teleport->flap1_special, parser->word_buf, 1) != 0) {
            Tel_teleport->flap1_matrix = *NuSpecialGetDrawMtx(&Tel_teleport->flap1_special);
        }
    } else {
        if (NuSpecialFind(Tel_worldinfo->current_gscn, &Tel_teleport->flap2_special, parser->word_buf, 1) != 0) {
            Tel_teleport->flap2_matrix = *NuSpecialGetDrawMtx(&Tel_teleport->flap2_special);
        }
    }
}
static void Tel_flip_flap(nufpar_s *) {
    Tel_teleport->flags |= 8;
}
static void Tel_Hacky_Cam(nufpar_s *) {
    Tel_teleport->flags |= 0x10;
}
static void Tel_name(nufpar_s *parser) {
    if (NuFParGetWord(parser) != 0) {
        GizmoGetUniqueName(WORLD->gizmo_sys, "TLT_", parser->word_buf, Tel_teleport->name, sizeof(Tel_teleport->name));
    }
}
static void Tel_obj(nufpar_s *parser) {
    if (NuFParGetWord(parser) != 0) {
        NuSpecialFind(Tel_worldinfo->current_gscn, &Tel_teleport->blocking_special, parser->word_buf, 1);
    }
}
static void Tel_range(nufpar_s *parser) {
    f32 range = NuFParGetFloat(parser);
    Tel_teleport->flags |= 2;
    Tel_teleport->range_squared = range * range;
}
static void Tel_spline(nufpar_s *parser) {
    if (NuFParGetWord(parser) == 0) {
        return;
    }

    Tel_teleport->path = edSpline_SplineFind(Tel_worldinfo->current_gscn, parser->word_buf);
    if (Tel_teleport->path == NULL || Tel_teleport->path->length != 4) {
        Tel_teleport->path = NULL;
        return;
    }

    for (i32 i = 0; i < Tel_worldinfo->teleport_count; ++i) {
        if (Tel_worldinfo->teleports[i].path == Tel_teleport->path) {
            Tel_teleport->path = NULL;
            return;
        }
    }

    NuStrCpy(Tel_teleport->name, "TLT_");
    NuStrCat(Tel_teleport->name, Tel_teleport->path->name);
    GizmoGetUniqueName(Tel_worldinfo->gizmo_sys, "TLT_", Tel_teleport->name, Tel_teleport->name,
                       sizeof(Tel_teleport->name));
}

static NUFPCOMJMP Teleport_ConfigKeywords[] = {
    {"spline", Tel_spline}, {"duration", Tel_duration},   {"range", Tel_range},        {"1_way", Tel_1_way},
    {"crawl", Tel_crawl},   {"flip_flap", Tel_flip_flap}, {"hackycam", Tel_Hacky_Cam}, {"obj", Tel_obj},
    {"flap1", Tel_flap},    {"flap2", Tel_flap},          {"name", Tel_name},          {NULL, NULL}};

void Teleports_Configure(WORLDINFO_s *world, char *config) {
    world->teleports = NULL;
    if (world->current_gscn == NULL) {
        return;
    }

    NUFPAR *parser = NuFParCreateMem("teleports", config, 0xffff);
    if (parser == NULL) {
        return;
    }

    world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
    TELEPORT_s *teleport = reinterpret_cast<TELEPORT_s *>(world->giz_buffer.void_ptr);
    world->teleports = teleport;
    NuFParPushCom(parser, Teleport_ConfigKeywords);

    i32 active = 0;
    while (NuFParGetLine(parser) != 0) {
        if (NuFParGetWord(parser) == 0) {
            continue;
        }

        if (active) {
            if (NuStrICmp(parser->word_buf, "teleport_end") != 0) {
                NuFParInterpretWord(parser);
                active = 1;
                continue;
            }

            if (NuStrLen(Tel_teleport->name) == 0) {
                NuStrCpy(Tel_teleport->name, "TLT_");
                NuStrCat(Tel_teleport->name, "TeleportNoSpline!");
                GizmoGetUniqueName(WORLD->gizmo_sys, "TLT_", Tel_teleport->name, Tel_teleport->name,
                                   sizeof(Tel_teleport->name));
            }

            active = 0;
            if (teleport->path != NULL) {
                ++world->teleport_count;
                ++teleport;
            }
            continue;
        }

        if (NuStrICmp(parser->word_buf, "teleport_start") != 0) {
            continue;
        }

        Tel_worldinfo = world;
        Tel_teleport = teleport;
        NuStrCpy(teleport->name, "");
        teleport->enabled = 1;
        teleport->path = NULL;
        teleport->duration = 5.0f;
        teleport->range_squared = 0.0f;
        teleport->flags = 0;
        teleport->active = 0;
        teleport->blocking_special = {};
        teleport->flap1_special = {};
        teleport->flap2_special = {};
        active = 1;
    }

    NuFParDestroy(parser);
    if (world->teleport_count > 0) {
        world->giz_buffer.addr = ALIGN(reinterpret_cast<usize>(teleport), 16);
    } else {
        world->teleports = NULL;
    }
}

static i32 Teleport_ActivateRev(GIZMO *gizmo, i32 activate, i32 flags) {
    if (gizmo == NULL || gizmo->object == NULL) {
        return 0;
    }
    TELEPORT_s *teleport = static_cast<TELEPORT_s *>(gizmo->object);
    if ((flags & 1) != 0 && teleport->enabled != activate) {
        return 0;
    }
    if (activate != 0) {
        teleport->enabled = 0;
    } else if (teleport->enabled == 0) {
        teleport->enabled = 1;
    }
    return 1;
}

static void Teleport_Activate(GIZMO *gizmo, i32 activate) {
    TELEPORT_s *teleport = static_cast<TELEPORT_s *>(gizmo->object);
    if (activate != 0) {
        teleport->active = 0;
        teleport->enabled = 1;
    } else {
        teleport->enabled = 0;
    }
}

static i32 Teleport_GetMaxGizmos(void *world_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    return world != NULL ? world->current_level->max_teleports : 0;
}

static void Teleport_AddGizmos(GIZMOSYS *gizmo_sys, i32 type_id, void *world_ptr, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (world != NULL && world->teleports != NULL) {
        for (i32 index = 0; index < world->teleport_count; ++index) {
            AddGizmo(gizmo_sys, type_id, NULL, &world->teleports[index]);
        }
    }
}

static char *Teleport_GetGizmoName(GIZMO *gizmo) {
    return gizmo != NULL ? static_cast<char *>(gizmo->object) : NULL;
}

static i32 Teleport_GetOutput(GIZMO *gizmo, i32, i32) {
    if (gizmo != NULL && gizmo->object != NULL) {
        return static_cast<TELEPORT_s *>(gizmo->object)->active;
    }
    return 0;
}

char *Teleport_GetOutputName(GIZMO *, i32) {
    return const_cast<char *>("Occupied");
}

static i32 Teleport_GetNumOutputs(GIZMO *) {
    return 1;
}

ADDGIZMOTYPE *Teleport_RegisterGizmo(i32 type_id) {
    static ADDGIZMOTYPE addtype;

    addtype = Default_ADDGIZMOTYPE;
    addtype.name = "Teleport";
    addtype.prefix = "TLT_";
    addtype.fns.unknown1 = 0;
    addtype.fns.early_update_fn = NULL;
    addtype.fns.panel_draw_fn = NULL;
    addtype.fns.get_visibility_fn = NULL;
    addtype.fns.get_max_gizmos_fn = Teleport_GetMaxGizmos;
    addtype.fns.get_pos_fn = NULL;
    addtype.fns.using_special_fn = NULL;
    addtype.fns.add_gizmos_fn = Teleport_AddGizmos;
    addtype.fns.bolt_hit_plat_fn = NULL;
    addtype.fns.get_best_bolt_target_fn = NULL;
    addtype.fns.late_update_fn = NULL;
    addtype.fns.bolt_hit_fn = NULL;
    addtype.fns.draw_fn = NULL;
    addtype.fns.get_gizmo_name_fn = Teleport_GetGizmoName;
    addtype.fns.get_output_fn = Teleport_GetOutput;
    addtype.fns.get_output_name_fn = Teleport_GetOutputName;
    addtype.fns.get_num_outputs_fn = Teleport_GetNumOutputs;
    addtype.fns.activate_fn = Teleport_Activate;
    addtype.fns.activate_rev_fn = Teleport_ActivateRev;
    addtype.fns.set_visibility_fn = NULL;
    addtype.fns.allocate_progress_data_fn = NULL;
    addtype.fns.clear_progress_fn = NULL;
    addtype.fns.store_progress_fn = NULL;
    addtype.fns.reset_fn = NULL;
    addtype.fns.reserve_buffer_space_fn = NULL;
    addtype.fns.load_fn = NULL;
    addtype.fns.post_load_fn = NULL;
    addtype.fns.add_level_sfx_fn = NULL;
    teleport_gizmotype_id = type_id;

    return &addtype;
}
