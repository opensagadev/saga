#include "decomp.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspline.h"

static TELEPORT_s *Tel_teleport;
static WORLDINFO_s *Tel_worldinfo;

static __used__ void Tel_1_way(nufpar_s *) {
    Tel_teleport->flags |= 1;
}
static __used__ void Tel_crawl(nufpar_s *) {
    Tel_teleport->flags |= 4;
}
static __used__ void Tel_duration(nufpar_s *parser) {
    TELEPORT_s *teleport = Tel_teleport;
    teleport->duration = NuFParGetFloat(parser);
}
static __used__ void Tel_flap(nufpar_s *parser) {
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
static __used__ void Tel_flip_flap(nufpar_s *) {
    Tel_teleport->flags |= 8;
}
static __used__ void Tel_Hacky_Cam(nufpar_s *) {
    Tel_teleport->flags |= 0x10;
}
static __used__ void Tel_name(nufpar_s *parser) {
    if (NuFParGetWord(parser) != 0) {
        GizmoGetUniqueName(WORLD->gizmo_sys, "Teleport_", parser->word_buf, Tel_teleport->name,
                           sizeof(Tel_teleport->name));
    }
}
static __used__ void Tel_obj(nufpar_s *parser) {
    if (NuFParGetWord(parser) != 0) {
        NuSpecialFind(Tel_worldinfo->current_gscn, &Tel_teleport->blocking_special, parser->word_buf, 1);
    }
}
static __used__ void Tel_range(nufpar_s *parser) {
    f32 range = NuFParGetFloat(parser);
    Tel_teleport->flags |= 2;
    Tel_teleport->range_squared = range * range;
}
static __used__ void Tel_spline(nufpar_s *parser) {
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

    NuStrCpy(Tel_teleport->name, "Teleport_");
    NuStrCat(Tel_teleport->name, Tel_teleport->path->name);
    GizmoGetUniqueName(Tel_worldinfo->gizmo_sys, "Teleport_", Tel_teleport->name, Tel_teleport->name,
                       sizeof(Tel_teleport->name));
}
