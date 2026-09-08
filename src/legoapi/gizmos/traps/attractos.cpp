#include "legoapi/gizmos/traps/attractos.h"

#include "decomp.h"
#include "gameapi/edtools/edfile.h"
#include <string.h>
#include <stdio.h>
#include "globals.h"
#include "legoapi/items/objects/gameobjects.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/characters/core/players.h"
extern "C" {
    i16 FindPlatInst(i32);
    i32 DeletePlatinst(i32);
    i16 NewPlatPickupInst(void *, i32);
}
extern "C" void NewTerrPlatformsOff();
void FindAnglesZX(NUVEC *, u16 *, u16 *);

struct ATTRACTOPROGRESS_s {
    u8 counts[32];
    u32 visible;
    u32 active;
    u32 filled;
};
DECOMP_ASSERT(sizeof(ATTRACTOPROGRESS_s) == 0x2c, "Attractor progress ABI");
#include "legoapi/world/level.h"

static i32 Attractos_GetMaxGizmos(void *world_info) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_info);
    return world != NULL ? world->current_level->max_attractos : 0;
}

static void Attractos_AddGizmos(GIZMOSYS *gizmo_sys, i32 type, void *context, void *) {
    WORLDINFO_s *world = static_cast<WORLDINFO_s *>(context);
    for (i32 i = 0; i < world->attracto_count; ++i) {
        ATTRACTO *attracto = &static_cast<ATTRACTO *>(world->attractos)[i];
        if (NuStrLen(attracto->name) != 0)
            AddGizmo(gizmo_sys, type, NULL, &static_cast<ATTRACTO *>(world->attractos)[i]);
    }
}

void *AddGameMessage(char *, NUVEC *, f32, NUVEC *, f32, u8, u8, u8, u32, f32);
static void Attractos_Update(void *context, void *, float) {
    WORLDINFO_s *world = static_cast<WORLDINFO_s *>(context);
    if (world == NULL || world->attractos == NULL)
        return;
    i32 alpha = (0.2f * game_pulse + 0.6f) * 128.0f;
    ATTRACTO *attracto = static_cast<ATTRACTO *>(world->attractos);
    for (i32 i = 0; i < world->attracto_count; ++i, ++attracto) {
        if ((attracto->state_flags & 7) != 3)
            continue;
        char text[32];
        sprintf(text, "%i/%i", attracto->collected_count, attracto->capacity);
        NUVEC position;
        position.x = attracto->position.x;
        position.y = 0.7f + attracto->position.y;
        position.z = attracto->position.z;
        GAMEMESSAGE_s *message = static_cast<GAMEMESSAGE_s *>(
            AddGameMessage(text, &position, 2.3f, NULL, 0.0f, 255, 255, 255, 0x1087, 0.0f));
        if (message != NULL)
            message->alpha = alpha;
    }
}

static void Attractos_Draw(void *context, void *, float) {
    WORLDINFO_s *world = static_cast<WORLDINFO_s *>(context);
    GameObject_s *nearest_player = NULL;
    if (world->lev_objs[72].active == 0 || world->attractos == NULL)
        return;
    u16 spin = static_cast<i32>(NuFmod(GameTimer.time_elapsed, 5.0f) / 5.0f * 65536.0f);
    f32 phase = NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f) * 2.0f * 65536.0f;
    f32 pulse = 0.2f * NU_SIN_LUT(phase) + 0.8f;
    ATTRACTO *attracto = static_cast<ATTRACTO *>(world->attractos);
    for (i32 i = 0; i < world->attracto_count; ++i, ++attracto) {
        attracto->state_flags &= ~8;
        if ((attracto->state_flags & 2) == 0)
            continue;
        NUMTX matrix = attracto->transform;
        i32 drawn = NuSpecialDrawAt(&world->lev_objs[72].special, &matrix);
        attracto->state_flags = (attracto->state_flags & ~8) | ((drawn & 1) << 3);
        if ((attracto->state_flags & 5) != 1 || world->lev_objs[85].active == 0)
            continue;
        NuMtxSetRotationY(&matrix, spin);
        if (attracto->ground_angle_x != 0)
            NuMtxRotateZ(&matrix, attracto->ground_angle_x);
        if (attracto->ground_angle_z != 0)
            NuMtxRotateX(&matrix, attracto->ground_angle_z);
        NuMtxTranslate(&matrix, &attracto->active_position);
        f32 distance;
        if (FindNearestPlayerToVec(&attracto->position, &nearest_player, distance, false, 0)) {
            f32 phase = NuFmin(distance / 6.0f, 1.0f) * 16384.0f + 49152.0f + 16384.0f;
            f32 alpha = pulse - NU_SIN_LUT(phase);
            NuSpecialDrawAtAlpha(&world->lev_objs[85].special, &matrix, alpha);
        }
    }
}

static char *Attracto_GetGizmoName(GIZMO *gizmo) {
    return gizmo != NULL ? static_cast<ATTRACTO *>(gizmo->object)->name : NULL;
}

static i32 Attracto_GetOutput(GIZMO *gizmo, i32, i32) {
    return (static_cast<ATTRACTO *>(gizmo->object)->state_flags >> 2) & 1;
}

static char *Attracto_GetOutputName(GIZMO *, i32) {
    return "Filled";
}

static i32 Attracto_GetNumOutputs(GIZMO *gizmo) {
    return 1;
}

static void Attracto_Activate(GIZMO *gizmo, i32 active) {
    if (gizmo != NULL) {
        ATTRACTO *attracto = static_cast<ATTRACTO *>(gizmo->object);
        attracto->state_flags = (attracto->state_flags & ~1) | (active != 0);
    }
}

static void Attracto_SetVisibility(GIZMO *gizmo, i32 visible) {
    if (gizmo == NULL || gizmo->object == NULL)
        return;
    ATTRACTO *attracto = static_cast<ATTRACTO *>(gizmo->object);
    attracto->state_flags = (attracto->state_flags & ~2) | (visible != 0 ? 2 : 0);
    if ((attracto->state_flags & 2) != 0 && attracto->platform_id == -1) {
        FindPlatInst(NuSpecialGetInstanceix(&WORLD->lev_objs[72].special));
        NUMTX matrix;
        NuMtxSetIdentity(&matrix);
        NuMtxSetRotationY(&matrix, 0);
        NuMtxRotateY(&matrix, attracto->angle);
        NuMtxTranslate(&matrix, &attracto->position);
        attracto->transform = matrix;
        attracto->platform_id = NewPlatPickupInst(&attracto->transform, 4);
    } else if ((attracto->state_flags & 2) == 0 && attracto->platform_id != -1) {
        DeletePlatinst(attracto->platform_id);
        attracto->platform_id = -1;
    }
}

static NUVEC *Attracto_GetPos(GIZMO *gizmo) {
    return gizmo != NULL ? &static_cast<ATTRACTO *>(gizmo->object)->position : NULL;
}

static void *Attractos_AllocateProgressData(VARIPTR *start, VARIPTR *end) {
    return GizmoBufferAlloc(start, end, sizeof(ATTRACTOPROGRESS_s));
}

static void Attractos_ClearProgress(void *, void *data) {
    if (data != NULL) {
        ATTRACTOPROGRESS_s *progress = static_cast<ATTRACTOPROGRESS_s *>(data);
        memset(progress->counts, 0, sizeof(progress->counts));
        progress->visible = 0xffffffff;
        progress->active = 0xffffffff;
        progress->filled = 0;
    }
}

static void Attractos_StoreProgress(void *context, void *, void *data) {
    if (data == NULL)
        return;
    ATTRACTOPROGRESS_s *progress = static_cast<ATTRACTOPROGRESS_s *>(data);
    Attractos_ClearProgress(NULL, progress);
    WORLDINFO_s *world = static_cast<WORLDINFO_s *>(context);
    if (world != NULL && world->attractos != NULL) {
        ATTRACTO *attracto = static_cast<ATTRACTO *>(world->attractos);
        for (i32 i = 0; i < world->attracto_count && i < 32; ++i, ++attracto) {
            progress->counts[i] = attracto->collected_count;
            u32 mask = 1u << i;
            if ((attracto->state_flags & 2) == 0)
                progress->visible &= ~mask;
            if ((attracto->state_flags & 1) == 0)
                progress->active &= ~mask;
            if ((attracto->state_flags & 4) != 0)
                progress->filled |= mask;
        }
    }
}

static void Attractos_Reset(void *context, void *, void *data) {
    WORLDINFO_s *world = static_cast<WORLDINFO_s *>(context);
    ATTRACTOPROGRESS_s *progress = static_cast<ATTRACTOPROGRESS_s *>(data);
    if (world == NULL || world->attractos == NULL)
        return;
    ATTRACTO *attracto = static_cast<ATTRACTO *>(world->attractos);
    for (i32 i = 0; i < world->attracto_count; ++i, ++attracto) {
        attracto->active_position.y = 0.0f;
        attracto->active_position.x = 0.0f;
        attracto->active_position.z = 0.5f;
        NuVecRotateY(&attracto->active_position, &attracto->active_position, attracto->angle);
        NuVecAdd(&attracto->active_position, &attracto->active_position, &attracto->position);
        NewTerrPlatformsOff();
        f32 ground = GameShadow(NULL, &attracto->active_position, 5.0f, -1);
        if (ground != 2000000.0f) {
            attracto->active_position.y = ground + 0.005f;
            FindAnglesZX(&ShadNorm, &attracto->ground_angle_z, &attracto->ground_angle_x);
        } else
            attracto->active_position.y = 2000000.0f;
        attracto->state_flags = (attracto->state_flags | 3) & ~12;
        NuMtxSetRotationY(&attracto->transform, attracto->angle);
        NuMtxTranslate(&attracto->transform, &attracto->position);
        if (i < 32 && progress != NULL) {
            u32 mask = 1u << i;
            attracto->collected_count = progress->counts[i];
            attracto->state_flags = (attracto->state_flags & ~2) | ((progress->visible & mask) != 0 ? 2 : 0);
            attracto->state_flags = (attracto->state_flags & ~1) | ((progress->active & mask) != 0);
            attracto->state_flags = (attracto->state_flags & ~4) | ((progress->filled & mask) != 0 ? 4 : 0);
        }
    }
}

static void *Attractos_ReserveBufferSpace(void *context) {
    WORLDINFO_s *world = static_cast<WORLDINFO_s *>(context);
    world->attractos = NULL;
    world->attracto_count = 0;
    if (world->current_level->max_attractos != 0) {
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
        world->attractos = world->giz_buffer.void_ptr;
        world->giz_buffer.addr += world->current_level->max_attractos * sizeof(ATTRACTO);
    }
    return world->attractos;
}

static i32 Attractos_Load(void *context, void *) {
    WORLDINFO_s *world = static_cast<WORLDINFO_s *>(context);
    if (world->attracto_count != 0)
        return 0;
    i32 version = EdFileReadInt();
    world->attracto_count = EdFileReadInt();
    ATTRACTO *attracto = static_cast<ATTRACTO *>(world->attractos);
    for (i32 i = 0; i < world->attracto_count; ++i, ++attracto) {
        EdFileRead(attracto->name, 0x10);
        EdFileReadNuVec(&attracto->position);
        attracto->angle = EdFileReadShort();
        u8 capacity = EdFileReadUnsignedChar();
        attracto->capacity = capacity != 0 ? capacity : 1;
        if (version == 2) {
            char legacy_name[0x10];
            i32 length = static_cast<i8>(EdFileReadChar());
            EdFileRead(legacy_name, length);
        }
    }
    return 1;
}

ADDGIZMOTYPE *Attractos_RegisterGizmo(i32 type_id) {
    static ADDGIZMOTYPE addtype;

    addtype = Default_ADDGIZMOTYPE;
    addtype.name = "Attracto";
    addtype.prefix = "";
    addtype.fns.unknown1 = 0x2c;
    addtype.fns.early_update_fn = NULL;
    addtype.fns.panel_draw_fn = NULL;
    addtype.fns.get_visibility_fn = NULL;
    addtype.fns.get_max_gizmos_fn = Attractos_GetMaxGizmos;
    addtype.fns.get_pos_fn = Attracto_GetPos;
    addtype.fns.using_special_fn = NULL;
    addtype.fns.add_gizmos_fn = Attractos_AddGizmos;
    addtype.fns.bolt_hit_plat_fn = NULL;
    addtype.fns.get_best_bolt_target_fn = NULL;
    addtype.fns.late_update_fn = Attractos_Update;
    addtype.fns.bolt_hit_fn = NULL;
    addtype.fns.draw_fn = Attractos_Draw;
    addtype.fns.get_gizmo_name_fn = Attracto_GetGizmoName;
    addtype.fns.get_output_fn = Attracto_GetOutput;
    addtype.fns.get_output_name_fn = Attracto_GetOutputName;
    addtype.fns.get_num_outputs_fn = Attracto_GetNumOutputs;
    addtype.fns.activate_fn = Attracto_Activate;
    addtype.fns.activate_rev_fn = NULL;
    addtype.fns.set_visibility_fn = Attracto_SetVisibility;
    addtype.fns.allocate_progress_data_fn = Attractos_AllocateProgressData;
    addtype.fns.clear_progress_fn = Attractos_ClearProgress;
    addtype.fns.store_progress_fn = Attractos_StoreProgress;
    addtype.fns.reset_fn = Attractos_Reset;
    addtype.fns.reserve_buffer_space_fn = Attractos_ReserveBufferSpace;
    addtype.fns.load_fn = Attractos_Load;
    addtype.fns.post_load_fn = NULL;
    addtype.fns.add_level_sfx_fn = NULL;

    return &addtype;
}
