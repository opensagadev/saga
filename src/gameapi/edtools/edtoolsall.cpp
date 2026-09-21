#include "decomp.h"
#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edgra.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nufile/nufile.h"
#include <stdio.h>
#include <string.h>
#include "nu2api/numath/nurand.h"

EdRegistry theRegistry;
extern MemoryManager theMemoryManager;
extern LevelEditor theLevelEditor;
i32 EdType_Char;
i32 EdType_Short;
i32 EdType_Int;
i32 EdType_Float;
i32 EdType_VuVec;
i32 EdType_VuMtx;
i32 EdType_Enumeration;
i32 EdType_String;
i32 EdType_Colour3;
i32 EdType_NuHSpecial;
i32 EdType_NuVec;
i32 EdType_NuMtx;

void SerialiseChar(EdStream &, void *, i32);
void SerialiseShort(EdStream &, void *, i32);
void SerialiseInt(EdStream &, void *, i32);
void SerialiseFloat(EdStream &, void *, i32);
void SerialiseVuVec(EdStream &, void *, i32);
void SerialiseVuMtx(EdStream &, void *, i32);
void SerialiseString(EdStream &, void *, i32);
void SerialiseColour3(EdStream &, void *, i32);
void SerialiseNuHSpecial(EdStream &, void *, i32);
void SerialiseNuVec(EdStream &, void *, i32);
void SerialiseNuMtx(EdStream &, void *, i32);

f32 EdManipulator::Scale = 2.0f;
EdManipulator theDefaultManipulator;
DECOMP_ASSERT(sizeof(EdManipulator) == 0x6c, "EdManipulator ABI");
SplineHelper theSplineHelper;
KnotHelper theKnotHelper;
extern ClassEditor theClassEditor;
i32 pad_disabled;
eduimenu_s *edLevelPinnedMenu;

void eduiSetPinnedMenu(eduimenu_s *menu) {
    edLevelPinnedMenu = menu;
}

void edSetPadDisabled(i32 disabled) {
    pad_disabled = disabled;
}

i32 edGetPadDisabled() {
    return pad_disabled;
}

static NUGSPLINE *splineStore;
static i32 numSplinesLoaded;
NUMTL *EdDrawMtl[2];
static i32 NewPrim;
static i32 NewMtl;
static const VuMtx *NewMtx;
char *EDSPLINE_FILECHECK = const_cast<char *>("EDSPLINE v. ");

extern "C" {
    char edgra_filter_string[16] = "GRASS";
    i32 edgra_mode = 1;
    extern NUGSCN *edbits_base_scene;
    extern part_type_s part_types[128];
    extern NUVEC edanim_cam_pos;
    i32 edgra_last_clump_in_buffer = -1;
    i32 edgra_copy_source = -1;
    f32 edgra_global_fadein = 15.0f, edgra_global_fadeout = 25.0f;
    void edgraInitAllClumps(void);
    void DebFreeInstantly(i32 *);
    void DebrisEmitterPos(i32, f32, f32, f32);
    void DebrisOrientation(i32, i16, i16);
    void DebrisEmitterOrientation(i32, i16, i16, i16);
    void DebrisReflectionOrientation(i32, i16, i16, f32, f32);
    void DebrisSetFacing(i32, u8, i16, i16);
    void DebrisStartOffset(i32, f32);
    void DebrisSetGroupID(i32, i16);
    void DebrisSetPriority(i32, i16, u8);
    void DebrisSetRoomID(i32, NUGSCN *);
    void DebrisSetDetailLevels(i32, i32);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern i32 part_page_used[8];
    extern i32 edanim_params_used;
    extern i32 edanim_particle_type;
    extern i32 edanim_emitrotz;
    extern i32 edanim_emitroty;
    extern f32 edpp_offset;
    i32 edbits_particle_level_page;
}

i32 edpartLookupObjectInScene(char *, NUGSCN *);

void EdTerrInit(void *, void *) {
    STUBBED();
}

void edDrawLine(nuvec_s *, nuvec_s *, unsigned char, unsigned char, unsigned char) {
}

void EdDrawBegin(i32 material) {
    if (EdDrawMtl[0] == NULL) {
        EdDrawMtl[0] = NuMtlCreate3D(1);
        EdDrawMtl[0]->attribs.cull_mode = 2;
        EdDrawMtl[0]->attribs.alpha_mode = 1;
        NuMtlUpdate(EdDrawMtl[0]);
        EdDrawMtl[1] = NuMtlCreate3D(1);
        EdDrawMtl[1]->attribs.cull_mode = 2;
        EdDrawMtl[1]->attribs.z_mode = 3;
        EdDrawMtl[1]->attribs.alpha_mode = 1;
        NuMtlUpdate(EdDrawMtl[1]);
    }
    NewPrim = 1;
    NewMtl = material;
}

void edpartPlace(i32, nuvec_s *) {
    STUBBED();
}

void edppDoInput(nupad_s *) {
    STUBBED();
}

void EdTerrShadow(nuvec_s *, float, float, i32) {
    STUBBED();
}

void edbriDoInput(nupad_s *) {
    STUBBED();
}

void edgraDoInput(nupad_s *) {
    STUBBED();
}

void edpartCreate(nuvec_s *, i32) {
    STUBBED();
}

i32 edppPtlPlace(i32 index, NUVEC *position) {
    edpp_particle_s *particle = &edpp_ptls[index];
    DebrisEmitterPos(particle->instance_id, position->x, position->y, position->z);
    DebrisOrientation(particle->instance_id, edpp_rotz, edpp_roty);
    DebrisEmitterOrientation(particle->instance_id, edpp_emitrotz, edpp_emitroty, edpp_emitrotx);
    DebrisReflectionOrientation(particle->instance_id, edpp_refrotz, edpp_refroty, particle->reflection_offset,
                                particle->reflection_bounce);
    if (particle->facing_mode != 0) {
        particle->facing_rotation_x = edpp_facrotx;
        particle->facing_rotation_y = edpp_facroty;
        DebrisSetFacing(particle->instance_id, particle->facing_mode, particle->facing_rotation_x,
                        particle->facing_rotation_y);
    }
    edpp_ptls[index].position = *position;
    edpp_ptls[index].emitter_rotation_z = edpp_emitrotz;
    edpp_ptls[index].emitter_rotation_y = edpp_emitroty;
    edpp_ptls[index].emitter_rotation_x = edpp_emitrotx;
    edpp_ptls[index].rotation_z = edpp_rotz;
    edpp_ptls[index].rotation_y = edpp_roty;
    edpp_ptls[index].reflection_rotation_z = edpp_refrotz;
    edpp_ptls[index].reflection_rotation_y = edpp_refroty;
    return index;
}

void EdDrawPolyTri(VuVec const &a, VuVec const &b, VuVec const &c, i32 colour) {
    if (NewPrim == 1) {
        NuPrim3DBegin(0, 5, EdDrawMtl[NewMtl], NewMtx ? const_cast<NUMTX *>(&NewMtx->matrix) : NULL);
        NewPrim = 3;
    }
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(a.x, a.y, a.z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(b.x, b.y, b.z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(c.x, c.y, c.z);
}

void edanimDoInput(nupad_s *) {
    STUBBED();
}

void edbobsDrawBox(nuvec_s *, nuvec_s *, i32) {
    STUBBED();
}

void edbriFileSave(char *) {
    STUBBED();
}

void edgraFileSave(char *) {
    STUBBED();
}

void edpartDoInput(nupad_s *) {
    STUBBED();
}

i32 edppPtlCreate(NUVEC *position, i32 effect_index) {
    if (edpp_instances_used == 512)
        return -1;
    i32 index = 0;
    while (edpp_ptls[index].instance_id != -1)
        ++index;
    AddDebrisEffect(&edpp_ptls[index].instance_id, effect_index, position->x, position->y, position->z);
    edpp_particle_s *particle = &edpp_ptls[index];
    if (particle->instance_id == -1)
        return -1;
    debkeydata[particle->instance_id].field_2f9 = 0;
    particle->position = *position;
    particle->effect_index = effect_index;
    particle->rotation_z = edpp_rotz;
    particle->rotation_y = edpp_roty;
    particle->emitter_rotation_z = edpp_emitrotz;
    particle->emitter_rotation_y = edpp_emitroty;
    particle->emitter_rotation_x = edpp_emitrotx;
    particle->start_offset = edpp_offset;
    particle->switch_type = 0;
    particle->switch_id = -1;
    particle->switch_variable = 0.0f;
    particle->reflection_offset = 0.0f;
    particle->reflection_bounce = 0.9f;
    particle->render_group = 0;
    particle->page = edbits_particle_level_page;
    particle->detail_levels = 7;
    switch (debtab[effect_index]->particle_type) {
        case 0:
            particle->render_priority = 20000;
            break;
        case 2:
            particle->render_priority = static_cast<i16>(40000);
            break;
        case 3:
            particle->render_priority = 30000;
            break;
        case 7:
            particle->render_priority = 10000;
            break;
    }
    particle = &edpp_ptls[index];
    particle->dynamic_priority = 0;
    particle->facing_mode = 0;
    particle->facing_rotation_x = 0;
    particle->facing_rotation_y = 0;
    strcpy(particle->name, debtab[particle->effect_index]->name);
    DebrisOrientation(particle->instance_id, particle->rotation_z, particle->rotation_y);
    DebrisEmitterOrientation(particle->instance_id, particle->emitter_rotation_z, particle->emitter_rotation_y,
                             particle->emitter_rotation_x);
    DebrisStartOffset(particle->instance_id, particle->start_offset);
    DebrisReflectionOrientation(particle->instance_id, particle->reflection_rotation_z, particle->reflection_rotation_y,
                                particle->reflection_offset, particle->reflection_bounce);
    DebrisSetFacing(particle->instance_id, particle->facing_mode, particle->facing_rotation_x,
                    particle->facing_rotation_y);
    DebrisSetGroupID(particle->instance_id, particle->render_group);
    DebrisSetPriority(particle->instance_id, particle->render_priority, particle->dynamic_priority);
    DebrisSetRoomID(particle->instance_id, 0);
    DebrisSetDetailLevels(particle->instance_id, particle->detail_levels);
    ++edpp_instances_used;
    edpp_page_used[edbits_particle_level_page] = 1;
    edpp_page_on[edbits_particle_level_page] = 1;
    if (edpp_page_scene[edbits_particle_level_page] == 0)
        edpp_page_scene[edbits_particle_level_page] = reinterpret_cast<usize>(edbits_base_scene);
    return index;
}

void edppPtlShelve(i32 index) {
    edpp_particle_s *particle = &edpp_ptls[index];
    if (particle->instance_id != -1 && particle->instance_id != 99999) {
        DebFreeInstantly(&particle->instance_id);
        particle->instance_id = 99999;
    }
}

void EdDrawLineCube(VuMtx const &transform, float size, i32 colour) {
    VuVec points[9] = {
        VuVec(size, size, -size, 0.0f),  VuVec(size, size, size, 0.0f),    VuVec(-size, size, size, 0.0f),
        VuVec(-size, size, -size, 0.0f), VuVec(size, -size, -size, 0.0f),  VuVec(size, -size, size, 0.0f),
        VuVec(-size, -size, size, 0.0f), VuVec(-size, -size, -size, 0.0f), VuVec(0.0f, 0.0f, size, 0.0f),
    };
    NUMTX *matrix = const_cast<NUMTX *>(&transform.matrix);
    NuVecMtxTransform(&points[0].xyz, &points[0].xyz, matrix);
    NuVecMtxTransform(&points[1].xyz, &points[1].xyz, matrix);
    NuVecMtxTransform(&points[2].xyz, &points[2].xyz, matrix);
    NuVecMtxTransform(&points[3].xyz, &points[3].xyz, matrix);
    NuVecMtxTransform(&points[4].xyz, &points[4].xyz, matrix);
    NuVecMtxTransform(&points[5].xyz, &points[5].xyz, matrix);
    NuVecMtxTransform(&points[6].xyz, &points[6].xyz, matrix);
    NuVecMtxTransform(&points[7].xyz, &points[7].xyz, matrix);
    NuVecMtxTransform(&points[8].xyz, &points[8].xyz, matrix);
    EdDrawLineSegment(points[0], points[1], colour);
    EdDrawLineSegment(points[1], points[2], colour);
    EdDrawLineSegment(points[2], points[3], colour);
    EdDrawLineSegment(points[3], points[0], colour);
    EdDrawLineSegment(points[0], points[2], colour);
    EdDrawLineSegment(points[1], points[3], colour);
    EdDrawLineSegment(points[4], points[5], colour);
    EdDrawLineSegment(points[5], points[6], colour);
    EdDrawLineSegment(points[6], points[7], colour);
    EdDrawLineSegment(points[7], points[4], colour);
    EdDrawLineSegment(points[0], points[4], colour);
    EdDrawLineSegment(points[1], points[5], colour);
    EdDrawLineSegment(points[2], points[6], colour);
    EdDrawLineSegment(points[3], points[7], colour);
    EdDrawLineSegment(points[5], points[8], colour);
    EdDrawLineSegment(points[6], points[8], colour);
}

void EdDrawPolyAxis(VuMtx const &, float, i32) {
    STUBBED();
}

void edanimFileSave(char *) {
    STUBBED();
}

void edpartInitType(i32 index) {
    part_type_s *type = &part_types[index];
    for (i32 variant = 0; variant < 8; ++variant) {
        type->effect_ids[variant] = -1;
        type->effect_pages[variant] = 1;
    }
    type->variant_count = 0;
    type->lifetime = 0.0f;
    type->speed = 1.0f;
    type->gravity = 0.0f;
    type->emission_rate = 1.0f;
    type->emission_period = 1.0f;
    type->emission_period_random = 0.0f;
    type->emission_pause = 0.0f;
    type->emission_pause_random = 0.0f;
    type->rotation[0] = 0;
    type->rotation[1] = 0;
    type->rotation[2] = 0;
    type->flags = 0;
    type->trail_effects[0] = -1;
    type->trail_effects[1] = -1;
    type->attached_effect = -1;
    type->kill_effect = -1;
    type->impact_effect = -1;
    type->impact_part = -1;
    type->last_used_time = 0.0f;
    type->field_174 = -1;
    type->scale = 1.0f;
    for (i32 sound = 0; sound < 4; ++sound) {
        type->sounds[sound] = -1;
        type->sound_modes[sound] = 0;
    }
}

void edppDrawCursor() {
    STUBBED();
}

extern "C" {
    void DebFreeInstantly(i32 *);
}

void edppPtlDestroy(i32 index) {
    if (edpp_ptls[index].instance_id != -1) {
        if (edpp_ptls[index].instance_id != 99999)
            DebFreeInstantly(&edpp_ptls[index].instance_id);
        --edpp_instances_used;
        edpp_ptls[index].instance_id = -1;
    }
}

void EdDrawLineArrow(VuMtx const &transform, float size, i32 colour) {
    VuVec points[4] = {
        VuVec(0.0f, 0.0f, -size, 0.0f),
        VuVec(0.0f, 0.0f, size, 0.0f),
        VuVec(size * 0.5f, 0.0f, 0.0f, 0.0f),
        VuVec(-size * 0.5f, 0.0f, 0.0f, 0.0f),
    };
    NUMTX *matrix = const_cast<NUMTX *>(&transform.matrix);
    NuVecMtxTransform(&points[0].xyz, &points[0].xyz, matrix);
    NuVecMtxTransform(&points[1].xyz, &points[1].xyz, matrix);
    NuVecMtxTransform(&points[2].xyz, &points[2].xyz, matrix);
    NuVecMtxTransform(&points[3].xyz, &points[3].xyz, matrix);
    EdDrawLineSegment(points[0], points[1], colour);
    EdDrawLineSegment(points[1], points[2], colour);
    EdDrawLineSegment(points[1], points[3], colour);
    EdDrawLineSegment(points[2], points[3], colour);
}

void EdDrawLineCross(VuVec const &position, float size, i32 colour) {
    VuVec start;
    VuVec end;
    start.y = end.y = position.y;
    start.z = end.z = position.z;
    start.x = position.x - size;
    end.x = position.x + size;
    EdDrawLineSegment(start, end, colour);

    start.x = end.x = position.x;
    start.y = position.y - size;
    end.y = position.y + size;
    EdDrawLineSegment(start, end, colour);

    start.y = end.y = position.y;
    start.z = position.z - size;
    end.z = position.z + size;
    EdDrawLineSegment(start, end, colour);
}

void EdDrawPolyArrow(VuVec const &, VuVec const &, i32, i32, float, float, float, float) {
    STUBBED();
}

void edbriDrawCursor() {
    STUBBED();
}

void edgraClumpPlace(i32 index, NUVEC *position) {
    edgra_clump_s *clump = &GrassClumps[index];
    clump->position = *position;
    clump->size = edgra_size;
    clump->rotation_z = edgra_rotz;
    clump->rotation_y = edgra_roty;
    if (edgra_mode != 3) {
        if (edgra_units_used + edgra_clump_size - clump->element_count <= 0x3000)
            clump->element_count = edgra_clump_size;
    }
    edgra_free_vecbuffer = static_cast<NUVEC *>(clump->vector_buffer) + clump->element_count;
    edgraInitAllClumps();
}

void edgraDrawCursor() {
    STUBBED();
}

void edpartPtlShelve(i32) {
    STUBBED();
}

void edpartScaleType(i32, float) {
    STUBBED();
}

void edppSaveEffects(char *, char) {
    STUBBED();
}

void EdDrawLineSphere(VuVec const &, float, float, i32) {
    STUBBED();
}

void EdDrawPolySector(VuVec const &, float, i32, i32, i32, i32, i32) {
    STUBBED();
}

void edanimDrawCursor() {
    STUBBED();
}

i32 edgraClumpCreate(NUVEC *position) {
    if (edgra_clumps_used == EDGRA_MAX_CLUMPS)
        return -1;
    if (edgra_copy_source == -1 && edgra_mode == 3) {
        if (edgra_units_used == 0x3000 || edgra_ind_clumps_used == EDGRA_MAX_INDIVIDUAL_CLUMPS)
            return -1;
    } else if (edgra_units_used + edgra_clump_size > 0x3000)
        return -1;
    i32 index = 0;
    while (GrassClumps[index].element_count != 0)
        ++index;
    edgra_clump_s *clump = &GrassClumps[index];
    if (edgra_copy_source != -1) {
        clump->field_18 = GrassClumps[edgra_copy_source].field_18;
        clump->special_index = GrassClumps[edgra_copy_source].special_index;
        clump->field_20 = GrassClumps[edgra_copy_source].field_20;
        clump->element_count = edgra_clump_size;
        clump->flags = GrassClumps[edgra_copy_source].flags;
        clump->page = edgra_pageid;
        clump->seed = NuRand(NULL);
        GrassClumps[index].unknown_25 = GrassClumps[edgra_copy_source].unknown_25;
        GrassClumps[index].field_2c = GrassClumps[edgra_copy_source].field_2c;
        GrassClumps[index].unknown_26 = GrassClumps[edgra_copy_source].unknown_26;
        GrassClumps[index].field_30 = GrassClumps[edgra_copy_source].field_30;
        GrassClumps[index].kind = GrassClumps[edgra_copy_source].kind;
        GrassClumps[index].near_distance = GrassClumps[edgra_copy_source].near_distance;
        GrassClumps[index].field_42 = GrassClumps[edgra_copy_source].field_42;
        GrassClumps[index].far_distance = GrassClumps[edgra_copy_source].far_distance;
        GrassClumps[index].field_44 = GrassClumps[edgra_copy_source].field_44;
        GrassClumps[index].field_43 = GrassClumps[edgra_copy_source].field_43;
    } else {
        clump->special_index = edgra_instance_type;
        clump->element_count = edgra_mode == 3 ? 1 : edgra_clump_size;
        clump->field_18 = 0.2f;
        clump->field_20 = 1.0f;
        clump->flags = 1;
        clump->page = edgra_pageid;
        clump->seed = NuRand(NULL);
        GrassClumps[index].unknown_25 = 1;
        GrassClumps[index].unknown_26 = 1;
        GrassClumps[index].field_2c = 0.0f;
        GrassClumps[index].field_30 = 1.0f;
        GrassClumps[index].kind = edgra_mode;
        GrassClumps[index].near_distance = edgra_global_fadein;
        GrassClumps[index].far_distance = edgra_global_fadeout;
        GrassClumps[index].field_42 = 1;
        GrassClumps[index].field_44 = 0.0f;
        GrassClumps[index].field_43 = 1;
    }
    if (GrassClumps[index].kind == 3) {
        i32 individual = 0;
        while (IndGrassClumpsUsed[individual])
            ++individual;
        GrassClumps[index].individual_index = individual;
        IndGrassClumpsUsed[individual] = 1;
        GetIndGrassClump(individual, 0)->position.x = 0.0f;
        GetIndGrassClump(individual, 0)->position.y = 0.0f;
        GetIndGrassClump(individual, 0)->position.z = 0.0f;
        GetIndGrassClump(individual, 0)->field_0c = 1.0f;
        GetIndGrassClump(individual, 0)->field_10 = edgra_rotz;
        GetIndGrassClump(individual, 0)->field_12 = edgra_roty;
        ++edgra_ind_clumps_used;
    } else
        GrassClumps[index].individual_index = -1;
    ++edgra_clumps_used;
    if (!edgra_page_used[edgra_pageid]) {
        edgra_page_used[edgra_pageid] = 1;
        edgra_page_scene[edgra_pageid] = edbits_base_scene;
        edgra_page_terrain[edgra_pageid] = edbits_base_terrain;
        edgra_page_matrix_stack[edgra_pageid] = edgra_mtxbuffer;
    }
    GrassClumps[index].vector_buffer = edgra_free_vecbuffer;
    edgra_last_clump_in_buffer = index;
    edgraClumpPlace(index, position);
    return index;
}

void edpartDrawCursor() {
    STUBBED();
}

template <i32 Axis> static inline void EdDrawCircle(VuVec const &centre, float radius, i32 colour, i32 segments) {
    VuVec base(0.0f, Axis == 2 ? radius : 0.0f, Axis == 2 ? 0.0f : radius, 0.0f);
    VuVec point(base.x + centre.x, base.y + centre.y, base.z + centre.z, 0.0f);
    for (i32 i = 1; i <= segments; i++) {
        VuVec previous = point;
        i32 angle = i * (0x10000 / segments);
        if (Axis == 0) {
            NuVecRotateX(&point.xyz, &base.xyz, angle);
        } else if (Axis == 1) {
            NuVecRotateY(&point.xyz, &base.xyz, angle);
        } else {
            NuVecRotateZ(&point.xyz, &base.xyz, angle);
        }
        point.x += centre.x;
        point.y += centre.y;
        point.z += centre.z;
        point.w = 0.0f;
        EdDrawLineSegment(previous, point, colour);
    }
}

void EdDrawLineCircleX(VuVec const &centre, float radius, i32 colour, i32 segments) {
    EdDrawCircle<0>(centre, radius, colour, segments);
}

void EdDrawLineCircleY(VuVec const &centre, float radius, i32 colour, i32 segments) {
    EdDrawCircle<1>(centre, radius, colour, segments);
}

void EdDrawLineCircleZ(VuVec const &centre, float radius, i32 colour, i32 segments) {
    EdDrawCircle<2>(centre, radius, colour, segments);
}

void EdDrawLineSegment(VuVec const &a, VuVec const &b, i32 colour) {
    if (NewPrim == 1) {
        NuPrim3DBegin(2, 5, EdDrawMtl[NewMtl], NewMtx ? const_cast<NUMTX *>(&NewMtx->matrix) : NULL);
        NewPrim = 2;
    }
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(a.x, a.y, a.z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(b.x, b.y, b.z);
}

void edanimParamCreate(i32) {
    STUBBED();
}

void edpartSaveEffects(char *, char) {
    STUBBED();
}

void edppPtlChangeType(i32 index, i32 effect_index) {
    edpp_particle_s *particle = &edpp_ptls[index];
    if (particle->effect_index != effect_index) {
        edppPtlDestroy(index);
        AddDebrisEffect(&edpp_ptls[index].instance_id, effect_index, particle->position.x, particle->position.y,
                        particle->position.z);
        if (particle->instance_id != -1)
            debkeydata[particle->instance_id].field_2f9 = 0;
        edpp_ptls[index].effect_index = effect_index;
    }
}

i32 edppPtlCreateCopy(NUVEC *position, i32 source_index) {
    if (edpp_instances_used == 512)
        return -1;
    i32 index = 0;
    while (edpp_ptls[index].instance_id != -1)
        ++index;
    edpp_particle_s *source = &edpp_ptls[source_index];
    AddDebrisEffect(&edpp_ptls[index].instance_id, source->effect_index, position->x, position->y, position->z);
    edpp_particle_s *particle = &edpp_ptls[index];
    if (particle->instance_id == -1)
        return -1;
    debkeydata[particle->instance_id].field_2f9 = 0;
    particle->position = *position;
    particle->effect_index = source->effect_index;
    particle->rotation_z = source->rotation_z;
    particle->rotation_y = source->rotation_y;
    particle->emitter_rotation_z = source->emitter_rotation_z;
    particle->emitter_rotation_y = source->emitter_rotation_y;
    particle->start_offset = source->start_offset;
    particle->switch_type = source->switch_type;
    particle->switch_id = source->switch_id;
    particle->switch_variable = source->switch_variable;
    particle->reflection_offset = source->reflection_offset;
    particle->reflection_bounce = source->reflection_bounce;
    particle->render_group = source->render_group;
    particle->page = source->page;
    particle->detail_levels = source->detail_levels;
    particle->facing_mode = source->facing_mode;
    particle->facing_rotation_x = source->facing_rotation_x;
    particle->facing_rotation_y = source->facing_rotation_y;
    strcpy(edpp_ptls[index].name, debtab[particle->effect_index]->name);
    DebrisOrientation(particle->instance_id, particle->rotation_z, particle->rotation_y);
    DebrisEmitterOrientation(particle->instance_id, particle->emitter_rotation_z, particle->emitter_rotation_y,
                             particle->emitter_rotation_x);
    DebrisStartOffset(particle->instance_id, particle->start_offset);
    DebrisReflectionOrientation(particle->instance_id, particle->reflection_rotation_z, particle->reflection_rotation_y,
                                particle->reflection_offset, particle->reflection_bounce);
    DebrisSetFacing(particle->instance_id, particle->facing_mode, particle->facing_rotation_x,
                    particle->facing_rotation_y);
    DebrisSetGroupID(particle->instance_id, particle->render_group);
    DebrisSetRoomID(particle->instance_id, reinterpret_cast<NUGSCN *>(edpp_page_scene[particle->page]));
    DebrisSetDetailLevels(particle->instance_id, particle->detail_levels);
    ++edpp_instances_used;
    return index;
}

void EdDrawPolyCylinder(VuMtx const &, float, float, float, i32, i32, i32, i32) {
    STUBBED();
}

void EdDrawPolyCylinder(VuVec const &, VuVec const &, i32, i32, i32, float, float, float) {
    STUBBED();
}

void edanimParamDestroy(i32 index) {
    if (AnimParams[index].instance_id != -1)
        AnimParams[index].instance_id = -1;
    --edanim_params_used;
}

void edbitsDoSingleDump(i32 face) {
    char filename[32];
    i32 index;
    for (index = 0; index < 1000; ++index) {
        sprintf(filename, "pictures\\cub%03d_%d.bmp", index, face);
        if (NuFileSize(filename) <= 0)
            break;
        if (index == 999)
            break;
    }
    sprintf(filename, "pictures\\cub%03d_", index);
    NuPs2VideoScreenDump(filename, 1, 1.0f, 1.0f, face, 0, 0);
}

void edgraCalculatePage(char, i32) {
    STUBBED();
}

void edgraInstancePlace(i32 index, NUVEC *position) {
    GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, index)->position.x =
        position->x - GrassClumps[edgra_nearest].position.x;
    GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, index)->position.y =
        position->y - GrassClumps[edgra_nearest].position.y;
    GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, index)->position.z =
        position->z - GrassClumps[edgra_nearest].position.z;
    GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, index)->field_0c = 1.0f;
    GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, index)->field_10 = edgra_rotz;
    GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, index)->field_12 = edgra_roty;
    edgraInitAllClumps();
}

i32 edpartLookupObject(char *name) {
    if (edbits_base_scene != NULL)
        return edpartLookupObjectInScene(name, edbits_base_scene);
    return -1;
}

i32 edSpline_FindAllBeg(NUGSCN *scene, char *prefix, NUGSPLINE **results, i32 capacity) {
    i32 count = 0;
    if (capacity <= 0)
        return 0;
    if (splineStore != NULL) {
        for (i32 i = 0; i < numSplinesLoaded; ++i) {
            if (NuStrNICmp(prefix, splineStore[i].name, -1) == 0) {
                results[count++] = &splineStore[i];
                if (count >= capacity)
                    return count;
            }
        }
    }
    if (scene != NULL) {
        for (i32 i = 0; i < scene->numsplines; ++i) {
            if (NuStrNICmp(prefix, scene->splines[i].name, -1) == 0) {
                results[count++] = &scene->splines[i];
                if (count >= capacity)
                    return count;
            }
        }
    }
    return count;
}

i32 edSpline_FindAllSub(NUGSCN *scene, char *name, NUGSPLINE **results, i32 capacity) {
    i32 count = 0;
    if (capacity <= 0)
        return 0;
    if (splineStore != NULL) {
        for (i32 i = 0; i < numSplinesLoaded; ++i) {
            if (NuStrIStr(splineStore[i].name, name) != NULL) {
                results[count++] = &splineStore[i];
                if (count >= capacity)
                    return count;
            }
        }
    }
    if (scene != NULL) {
        for (i32 i = 0; i < scene->numsplines; ++i) {
            if (NuStrIStr(scene->splines[i].name, name) != NULL) {
                results[count++] = &scene->splines[i];
                if (count >= capacity)
                    return count;
            }
        }
    }
    return count;
}

i32 LoadEditorSplines(char *path, VARIPTR *buf, VARIPTR *buf_end) {
    splineStore = reinterpret_cast<NUGSPLINE *>(buf->void_ptr);
    buf->addr = (buf->addr + 3) & ~static_cast<usize>(3);

    EdFileSetMedia(1);
    if (EdFileOpen(path, NUFILE_READ) == 0) {
        splineStore = NULL;
        return 0;
    }

    const i32 file_check_length = NuStrLen(EDSPLINE_FILECHECK);
    for (i32 i = 0; i < file_check_length; ++i) {
        if (EdFileReadChar() != EDSPLINE_FILECHECK[i]) {
            EdFileClose();
            splineStore = NULL;
            return 0;
        }
    }

    EdFileReadInt();
    i32 spline_count = EdFileReadInt();
    i32 point_count = EdFileReadInt();
    i32 string_bytes = EdFileReadInt();

    char *name_cursor = reinterpret_cast<char *>(splineStore + spline_count);
    char *name_end = name_cursor + string_bytes;
    NUVEC *point_cursor = reinterpret_cast<NUVEC *>((reinterpret_cast<usize>(name_end) + 3) & ~static_cast<usize>(3));
    NUVEC *data_end = point_cursor + point_count;
    if (reinterpret_cast<usize>(data_end) > buf_end->addr) {
        EdFileClose();
        splineStore = NULL;
        return 0;
    }

    for (i32 i = 0; i < spline_count; ++i) {
        EdFileReadChar();
        i32 name_length = EdFileReadInt();
        NUGSPLINE *spline = &splineStore[i];
        spline->name = name_cursor;
        spline->pt_size = sizeof(NUVEC);
        spline->length = static_cast<i16>(EdFileReadInt());
        spline->pts = point_cursor;

        for (i32 j = 0; j < name_length; ++j) {
            spline->name[j] = EdFileReadChar();
        }
        name_cursor += name_length;
        if (name_cursor > name_end) {
            EdFileClose();
            splineStore = NULL;
            EdFileClose();
            numSplinesLoaded = spline_count;
            buf->void_ptr = data_end;
            return 0;
        }

        for (i32 j = 0; j < spline->length; ++j) {
            point_cursor[j].x = EdFileReadFloat();
            point_cursor[j].y = EdFileReadFloat();
            point_cursor[j].z = EdFileReadFloat();
        }
        point_cursor += spline->length;
        if (point_cursor > data_end) {
            EdFileClose();
            splineStore = NULL;
            EdFileClose();
            numSplinesLoaded = spline_count;
            buf->void_ptr = data_end;
            return 0;
        }
    }

    EdFileClose();
    numSplinesLoaded = spline_count;
    buf->void_ptr = data_end;
    return spline_count;
}

NUGSPLINE *edSpline_SplineFind(NUGSCN *scene, char *name) {
    if (splineStore != NULL) {
        for (i32 i = 0; i < numSplinesLoaded; ++i) {
            if (NuStrICmp(splineStore[i].name, name) == 0) {
                return &splineStore[i];
            }
        }
    }
    return NuSplineFind(scene, name);
}

void edSpline_SplineList(nugscn_s *) {
    STUBBED();
}

void edanimParticlePlace(i32 index, NUVEC *position) {
    nuhspecial_s special;
    NuGScnGetSpecial(&special, edbits_base_scene, edanim_nearest);
    NuVecSub(reinterpret_cast<NUVEC *>(AnimParams[edanim_nearest_param_id].effect_positions[index]), position,
             NuSpecialGetPos(&special));
    AnimParams[edanim_nearest_param_id].effect_angles[index] = edanim_emitrotz;
    AnimParams[edanim_nearest_param_id].effect_angle_ranges[index] = edanim_emitroty;
}

void edanimStartAllPages() {
    edanimStartPage(0);
    edanimStartPage(1);
    edanimStartPage(2);
    edanimStartPage(3);
    edanimStartPage(4);
    edanimStartPage(5);
    edanimStartPage(6);
    edanimStartPage(7);
}

void edgraInstanceCreate(NUVEC *position) {
    if (edgra_nearest != -1 && GrassClumps[edgra_nearest].element_count != EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP) {
        i32 index = GrassClumps[edgra_nearest].element_count++;
        edgraInstancePlace(index, position);
    }
}

void edpartPtlChangeType(i32, i32) {
    STUBBED();
}

void edppDestroyAllPages() {
    if (edpp_page_used[0])
        edppClearPage(0);
    if (edpp_page_used[1])
        edppClearPage(1);
    if (edpp_page_used[2])
        edppClearPage(2);
    if (edpp_page_used[3])
        edppClearPage(3);
    if (edpp_page_used[4])
        edppClearPage(4);
    if (edpp_page_used[5])
        edppClearPage(5);
    if (edpp_page_used[6])
        edppClearPage(6);
    if (edpp_page_used[7])
        edppClearPage(7);
}

void edanimParticleCreate(NUVEC *position) {
    i32 index = AnimParams[edanim_nearest_param_id].effect_count;
    if (index != 8 && edanim_particle_type != -1) {
        edanimParticlePlace(index, position);
        AnimParams[edanim_nearest_param_id].effect_ids[index] = edanim_particle_type;
        strcpy(AnimParams[edanim_nearest_param_id].effect_names[index], debtab[edanim_particle_type]->name);
        AnimParams[edanim_nearest_param_id].effect_intervals[index] = 60;
        AnimParams[edanim_nearest_param_id].effect_flags[index] = 0;
        ++AnimParams[edanim_nearest_param_id].effect_count;
    }
}

void edgraInstanceDestroy(i32 index) {
    if (edgra_nearest != -1 && GrassClumps[edgra_nearest].element_count != 1) {
        if (index != GrassClumps[edgra_nearest].element_count - 1) {
            for (i32 i = index; i < GrassClumps[edgra_nearest].element_count - 1; ++i) {
                GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, i)->position =
                    GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, i + 1)->position;
                GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, i)->field_0c =
                    GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, i + 1)->field_0c;
                GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, i)->field_10 =
                    GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, i + 1)->field_10;
                GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, i)->field_12 =
                    GetIndGrassClump(GrassClumps[edgra_nearest].individual_index, i + 1)->field_12;
            }
        }
        --GrassClumps[edgra_nearest].element_count;
        edgraInitAllClumps();
    }
}

void edppDetermineNearest(float max_distance_squared) {
    NUVEC delta;
    if (edpp_nearest != -1) {
        edpp_particle_s *particle = &edpp_ptls[edpp_nearest];
        if (particle->instance_id != 99999 && particle->instance_id != -1) {
            NuVecSub(&delta, &edpp_cam_pos, &particle->position);
            if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z == 0.0f) {
                return;
            }
        }
    }
    edpp_nearest = -1;
    for (i32 i = 0; i < 512; ++i) {
        if (edpp_ptls[i].instance_id == -1 || edpp_ptls[i].instance_id == 99999) {
            continue;
        }
        NuVecSub(&delta, &edpp_cam_pos, &edpp_ptls[i].position);
        float distance_squared = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
        if (max_distance_squared < 0.0f || max_distance_squared > distance_squared) {
            max_distance_squared = distance_squared;
            edpp_nearest = i;
        }
    }
}

void edppHighlightNearest() {
    STUBBED();
}

void edppMultipleCopyCopy() {
    STUBBED();
}

void edbriDetermineNearest(float) {
    STUBBED();
}

void edgraSortVectorBuffer(i32 index) {
    NUVEC temporary[256];
    if (index != -1 && index != edgra_last_clump_in_buffer && GrassClumps[index].vector_buffer) {
        u8 *buffer = static_cast<u8 *>(GrassClumps[index].vector_buffer);
        usize bytes = GrassClumps[index].element_count * sizeof(NUVEC);
        memcpy(temporary, buffer, bytes);
        memmove(buffer, buffer + bytes, static_cast<u8 *>(edgra_free_vecbuffer) - (buffer + bytes));
        memcpy(static_cast<u8 *>(edgra_free_vecbuffer) - bytes, temporary, bytes);
        edgra_last_clump_in_buffer = index;
        for (i32 i = 0; i < EDGRA_MAX_CLUMPS; ++i) {
            if (i != index && GrassClumps[i].element_count &&
                GrassClumps[i].vector_buffer > GrassClumps[index].vector_buffer) {
                GrassClumps[i].vector_buffer = static_cast<u8 *>(GrassClumps[i].vector_buffer) - bytes;
            }
        }
        GrassClumps[index].vector_buffer = static_cast<u8 *>(edgra_free_vecbuffer) - bytes;
        edgraInitAllClumps();
    }
}

void edpartDestroyAllPages() {
    if (part_page_used[0])
        edpartClearPage(0);
    if (part_page_used[1])
        edpartClearPage(1);
    if (part_page_used[2])
        edpartClearPage(2);
    if (part_page_used[3])
        edpartClearPage(3);
    if (part_page_used[4])
        edpartClearPage(4);
    if (part_page_used[5])
        edpartClearPage(5);
    if (part_page_used[6])
        edpartClearPage(6);
    if (part_page_used[7])
        edpartClearPage(7);
}

void edppMultipleCopyClear() {
    STUBBED();
}

void edppMultipleCopyPaste() {
    STUBBED();
}

void edppStartSingleEffect(i32) {
    STUBBED();
}

void edpartHighlightNearest() {
    STUBBED();
}

void edpartMultipleCopyCopy() {
    STUBBED();
}

void edpartMultipleCopyClear() {
    STUBBED();
}

float edanimPlayerAnimDistance(i32 parameter_index) {
    if (edmainQueryLocVec() != NULL) {
        nuhspecial_s special;
        NuGScnGetSpecial(&special, edanim_page_scene[AnimParams[parameter_index].page],
                         AnimParams[parameter_index].instance_id);
        NUVEC *position = edmainQueryLocVec();
        return NuVecDist(NuSpecialGetPos(&special), position, NULL);
    }
    return 0.0f;
}

void edanimRenderSoundEmitters(i32) {
    STUBBED();
}

void edbobs_DrawCoordinateInfo(nuvec_s *, i32, i32) {
    STUBBED();
}

void edanimDetermineNearestAnim(float distance) {
    if (edbits_base_scene == NULL) {
        return;
    }

    nuhspecial_s special;
    NUVEC delta;
    if (edanim_nearest != -1) {
        NuGScnGetSpecial(&special, edbits_base_scene, edanim_nearest);
        NuVecSub(&delta, &edanim_cam_pos, NuSpecialGetPos(&special));
        if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z == 0.0f) {
            return;
        }
    }

    edanim_nearest = -1;
    edanim_nearest_param_id = -1;
    const i32 special_count = NuGScnNumSpecials(edbits_base_scene);
    for (i32 i = 0; i < special_count; ++i) {
        NuGScnGetSpecial(&special, edbits_base_scene, i);
        NuVecSub(&delta, &edanim_cam_pos, NuSpecialGetPos(&special));
        const f32 candidate = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
        if (distance < 0.0f || candidate < distance) {
            distance = candidate;
            edanim_nearest = i;
        }
    }
    if (edanim_nearest != -1) {
        for (i32 i = 0; i < 64; ++i) {
            if (AnimParams[i].instance_id == edanim_nearest) {
                edanim_nearest_param_id = i;
                return;
            }
        }
    }
}

void edgraDetermineNearestClump(f32 distance) {
    NUVEC delta;
    if (edgra_nearest != -1) {
        NuVecSub(&delta, &edgra_cam_pos, &GrassClumps[edgra_nearest].position);
        if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z == 0.0f)
            return;
    }
    edgra_nearest = -1;
    for (i32 i = 0; i < EDGRA_MAX_CLUMPS; ++i) {
        if (GrassClumps[i].element_count) {
            NuVecSub(&delta, &edgra_cam_pos, &GrassClumps[i].position);
            f32 candidate = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
            if (distance < 0.0f || candidate < distance) {
                distance = candidate;
                edgra_nearest = i;
            }
        }
    }
    if (edgra_nearest != -1)
        edgraSortVectorBuffer(edgra_nearest);
}

void eduiItemFileSelectorCreate(u32, eduiiattr_s *, void (*)(eduimenu_s *, eduiitem_s *, u32), char *) {
    STUBBED();
}

void edanimDetermineNearestSound(float) {
    STUBBED();
}

void edanimRenderParticleEmitters(i32) {
    STUBBED();
}

void edgraDetermineNearestInstance(f32 distance) {
    NUVEC delta;
    if (edgra_nearest == -1) {
        edgra_nearest_instance = -1;
        return;
    }
    i32 individual = GrassClumps[edgra_nearest].individual_index;
    if (edgra_nearest_instance != -1) {
        NuVecAdd(&delta, &GrassClumps[edgra_nearest].position,
                 &GetIndGrassClump(individual, edgra_nearest_instance)->position);
        NuVecSub(&delta, &edgra_cam_pos, &delta);
        if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z == 0.0f)
            return;
    }
    edgra_nearest_instance = -1;
    for (i32 i = 0; i < GrassClumps[edgra_nearest].element_count; ++i) {
        NuVecAdd(&delta, &GrassClumps[edgra_nearest].position, &GetIndGrassClump(individual, i)->position);
        NuVecSub(&delta, &edgra_cam_pos, &delta);
        f32 candidate = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
        if (distance < 0.0f || candidate < distance) {
            distance = candidate;
            edgra_nearest_instance = i;
        }
    }
}

void edanimDetermineNearestParticle(float) {
    STUBBED();
}

void EdDrawEnd() {
    if (NewPrim) {
        if (NewPrim == 2 || NewPrim == 3) {
            NuPrim3DEnd();
        }
        NewPrim = 0;
    }
}

void EdDrawMtx(VuMtx const *matrix) {
    EdDrawEnd();
    EdDrawBegin(0);
    NewMtx = matrix;
}

void EdTerrRay(VuVec &, VuVec &) {
    STUBBED();
}

EdManScale::EdManScale() {
    STUBBED();
}

void EdManScale::Process(EdInputContext &, ClassObjectList &) {
    STUBBED();
}

void EdManScale::Render(ClassObjectList &) {
    STUBBED();
}

i32 EdRegistry::AddMapping(char *source, char *destination) {
    NameMapping *mapping = &mappings[object_count++];
    mapping->source = source;
    mapping->destination = destination;
    return 1;
}

void EdRegistry::AddObjectNotifier(EdObjectNotifier *notifier) {
    if (notifier_count < notifier_capacity) {
        notifiers[notifier_count++] = notifier;
    }
}

void EdRegistry::ClassIFaceProcess(EdClass *object_class, void *object, EdInputContext &context) {
    if (object_class && object_class->interface) {
        EdClassInterface *interface = object_class->interface;
        interface->vtable->process(interface, object, context);
    }
}

void EdRegistry::ClassIFaceProcess(i32 class_id, void *object, EdInputContext &context) {
    EdClass *object_class = GetClass(class_id);
    if (object_class && object_class->interface) {
        EdClassInterface *interface = object_class->interface;
        interface->vtable->process(interface, object, context);
    }
}

void EdRegistry::ClassIFaceRender(EdClass *object_class, void *object, i32 flags) {
    if (object_class && object_class->interface) {
        EdClassInterface *interface = object_class->interface;
        interface->vtable->render(interface, object, flags);
    }
}

void EdRegistry::ClassIFaceRender(i32 class_id, void *object, i32 flags) {
    EdClass *object_class = GetClass(class_id);
    if (object_class && object_class->interface) {
        EdClassInterface *interface = object_class->interface;
        interface->vtable->render(interface, object, flags);
    }
}

void *EdRegistry::CreateObject(EdClassInterface *interface, void *source, i32 index, i32 guid, i32 flags) {
    if (!guid && create_object_guid) {
        guid = create_object_guid();
    }
    void *object = interface->vtable->create_object(interface, source, index, flags);
    if (object) {
        interface->vtable->set_object_guid(interface, object, guid);
        if (!(flags & 2)) {
            NotifyCreateObject(object, interface->object_class, source, index, guid, flags);
        }
    }
    return object;
}

void EdRegistry::DefunctObject(EdClassInterface *interface, void *object, i32, i32 flags) {
    if (object) {
        if (!(flags & 2)) {
            NotifyDefunctObject(object, interface->object_class, flags);
        }
        interface->vtable->defunct_object(interface, object);
        EdDefunctListEntry *entry = new EdDefunctListEntry;
        entry->object_class = interface->object_class;
        entry->object = object;
        entry->next = nullptr;
        entry->previous = defunct_objects.last;
        if (defunct_objects.last) {
            defunct_objects.last->next = entry;
        }
        defunct_objects.last = entry;
        if (!defunct_objects.first) {
            defunct_objects.first = entry;
        }
        ++defunct_objects.count;
    }
}

void EdRegistry::DestroyObject(EdClassInterface *interface, void *object, i32 index, i32 flags) {
    theLevelEditor.destroying_objects = 1;
    if (object) {
        if (!(flags & 2)) {
            NotifyDestroyObject(object, interface->object_class, index, flags);
        }
        interface->vtable->destroy_object(interface, object, flags);
    }
    theLevelEditor.destroying_objects = 0;
}

void EdRegistry::Flush() {
    type_count = 0;
    class_count = 0;
    object_count = 0;
}

EdClass *EdRegistry::GetClass(char *name) {
    for (i32 index = 0; index < class_count; ++index) {
        if (NuStrICmp(classes[index].name, name) == 0) {
            return &classes[index];
        }
    }
    return nullptr;
}

EdClass *EdRegistry::GetClass(i32 index) {
    if (index < 0 || index >= class_count) {
        return nullptr;
    }
    return &classes[index];
}

i32 EdRegistry::GetClassId(char *name) {
    for (i32 index = 0; index < class_count; ++index) {
        if (NuStrICmp(classes[index].name, name) == 0) {
            return index;
        }
    }
    return -1;
}

i32 EdRegistry::GetClassId(EdClass *object_class) {
    return object_class - classes;
}

void EdRegistry::GetStreamClassMapping(EdStream &stream, i32 *mapping, i32 &count, i32) {
    i32 used_classes[64] = {};
    for (i32 index = 0; index < class_count; ++index) {
        EdClass *object_class = &classes[index];
        if (stream.flags & 0x400000) {
            if (object_class->flags & 0x400000) {
                continue;
            }
        } else if (object_class->flags & 0x10000000) {
            continue;
        }
        i32 stream_classes[64];
        i32 stream_class_count = 0;
        object_class->GetStreamClasses(stream, stream_classes, stream_class_count, 64);
        for (i32 stream_class = 0; stream_class < stream_class_count; ++stream_class) {
            used_classes[stream_classes[stream_class]] = 1;
        }
    }
    count = 0;
    for (i32 index = 0; index < class_count; ++index) {
        if (used_classes[index]) {
            mapping[index] = count++;
        } else {
            mapping[index] = -1;
        }
    }
}

EdType *EdRegistry::GetType(char *name) {
    for (i32 index = 0; index < type_count; ++index) {
        if (NuStrICmp(types[index].name, name) == 0) {
            return &types[index];
        }
    }
    return nullptr;
}

EdType *EdRegistry::GetType(i32 index) {
    if (index < 0 || index >= type_count) {
        return nullptr;
    }
    return &types[index];
}

i32 EdRegistry::GetTypeId(char *name) {
    for (i32 index = 0; index < type_count; ++index) {
        if (NuStrICmp(types[index].name, name) == 0) {
            return index;
        }
    }
    return -1;
}

void EdRegistry::Initialise(variptr_u &buffer, variptr_u &, i32 max_classes, i32 max_types, i32 max_mappings,
                            i32 max_notifiers) {
    class_capacity = max_classes;
    type_capacity = max_types;
    mapping_capacity = max_mappings;
    notifier_capacity = max_notifiers;
    type_count = 0;
    class_count = 0;
    object_count = 0;
    notifier_count = 0;
    types = static_cast<EdType *>(BUFFER_ALLOC(&buffer, sizeof(EdType) * max_types, 16));
    memset(types, 0, sizeof(EdType) * max_types);
    classes = static_cast<EdClass *>(BUFFER_ALLOC(&buffer, sizeof(EdClass) * class_capacity, 16));
    memset(classes, 0, sizeof(EdClass) * class_capacity);
    mappings = static_cast<NameMapping *>(BUFFER_ALLOC(&buffer, sizeof(NameMapping) * mapping_capacity, 16));
    memset(mappings, 0, sizeof(NameMapping) * mapping_capacity);
    notifiers =
        static_cast<EdObjectNotifier **>(BUFFER_ALLOC(&buffer, sizeof(EdObjectNotifier *) * notifier_capacity, 16));
    memset(notifiers, 0, sizeof(EdObjectNotifier *) * notifier_capacity);
    initialised = 1;
}

char *EdRegistry::MapName(char *name) {
    for (i32 index = 0; index < object_count; ++index) {
        if (NuStrICmp(mappings[index].source, name) == 0) {
            name = mappings[index].destination;
            break;
        }
    }
    return name;
}

void EdRegistry::NotifyCreateObject(void *object, EdClass *object_class, void *source, i32 index, i32 context,
                                    i32 flags) {
    for (i32 i = 0; i < notifier_count; ++i) {
        EdObjectNotifier *notifier = notifiers[i];
        notifier->vtable->create_object(notifier, object, object_class, source, index, context, flags);
    }
}

void EdRegistry::NotifyDefunctObject(void *object, EdClass *object_class, i32 flags) {
    for (i32 i = 0; i < notifier_count; ++i) {
        EdObjectNotifier *notifier = notifiers[i];
        notifier->vtable->defunct_object(notifier, object, object_class, flags);
    }
}

void EdRegistry::NotifyDestroyObject(void *object, EdClass *object_class, i32 index, i32 flags) {
    for (i32 i = 0; i < notifier_count; ++i) {
        EdObjectNotifier *notifier = notifiers[i];
        notifier->vtable->destroy_object(notifier, object, object_class, index, flags);
    }
}

void EdRegistry::NotifyReviveObject(void *object, EdClass *object_class, i32 flags) {
    for (i32 i = 0; i < notifier_count; ++i) {
        EdObjectNotifier *notifier = notifiers[i];
        notifier->vtable->revive_object(notifier, object, object_class, flags);
    }
}

void EdRegistry::RegisterBaseTypes() {
    EdType_Char = RegisterType("Char", 1, SerialiseChar);
    EdType_Short = RegisterType("Short", 2, SerialiseShort);
    EdType_Int = RegisterType("Int", 4, SerialiseInt);
    EdType_Float = RegisterType("Float", 4, SerialiseFloat);
    EdType_VuVec = RegisterType("VuVec", 16, SerialiseVuVec);
    EdType_VuMtx = RegisterType("VuMtx", 64, SerialiseVuMtx);
    EdType_Enumeration = RegisterType("Enum", 4, SerialiseInt);
    EdType_String = RegisterType("String", -1, SerialiseString);
    EdType_Colour3 = RegisterType("Colour3", 12, SerialiseColour3);
    EdType_NuHSpecial = RegisterType("NuHSpecial", 12, SerialiseNuHSpecial);
    EdType_NuVec = RegisterType("NuVec", 12, SerialiseNuVec);
    EdType_NuMtx = RegisterType("NuMtx", 64, SerialiseNuMtx);
}

EdClass *EdRegistry::RegisterClass(char *name, EdClassInterface *interface, i32 flags) {
    i32 index = class_count++;
    EdClass *object_class = &classes[index];
    object_class->name = name;
    object_class->interface = interface;
    object_class->flags = flags;
    if (interface != nullptr) {
        interface->object_class = object_class;
    }
    return object_class;
}

i32 EdRegistry::RegisterType(char *name, i32 size, void (*serialise)(EdStream &, void *, i32)) {
    i32 index = type_count++;
    EdType *type = &types[index];
    type->name = name;
    type->size = size;
    type->serialise = serialise;
    return index;
}

void EdRegistry::Serialise(EdStream &stream) {
    if (stream.BeginBlock("TypeList")) {
        stream.SerialiseBuffer(&type_count, sizeof(type_count), 1);
        for (i32 index = 0; index < type_count; ++index) {
            types[index].Serialise(stream);
        }
        stream.EndBlock();
    }
    if (stream.BeginBlock("ClassList")) {
        if (stream.mode == 2) {
            i32 mapping[64];
            i32 count = 0;
            GetStreamClassMapping(stream, mapping, count, 64);
            stream.SerialiseBuffer(&count, sizeof(count), 1);
            for (i32 index = 0; index < class_count; ++index) {
                if (mapping[index] != -1) {
                    classes[index].Serialise(stream, mapping);
                }
            }
        }
        if (stream.mode == 1) {
            stream.SerialiseBuffer(&class_count, sizeof(class_count), 1);
            for (i32 index = 0; index < class_count; ++index) {
                classes[index].Serialise(stream, nullptr);
            }
        }
        stream.EndBlock();
    }
}

void EdRegistry::SerialiseObjects(EdStream &stream, EdRegistry *source_registry) {
    char object_name[256];
    auto get_attribute = [](EdClass *object_class, void *object, i32 attribute, i32 type, void *data, i32 size) {
        EdMember member;
        if (object_class->FindMember(&member, object, attribute, 1))
            member.reference->GetAttributeData(member.object, attribute, type, data, size);
    };
    auto include_object = [&](EdClass *object_class, void *object) {
        i32 object_flags = 0;
        i16 group = 0;
        get_attribute(object_class, object, 1, EdType_Int, &object_flags, 0);
        get_attribute(object_class, object, 0x100, EdType_Short, &group, 0);
        return !(object_flags & (stream.flags & 0x400000 ? 0x400000 : 0x10000000)) && group == stream.unknown_10;
    };
    auto read_objects = [&](EdClass *object_class, EdClass *source_class) {
        i32 count;
        stream.SerialiseBuffer(&count, sizeof(count), 1);
        for (i32 index = 0; index < count; ++index) {
            i32 object_flags;
            stream.SerialiseBuffer(&object_flags, sizeof(object_flags), 1);
            void *object;
            if (object_flags & 0x02000000) {
                stream.SerialiseString(object_name, sizeof(object_name));
                object = object_class->FindObject(object_name);
            } else {
                void *original = NULL;
                if (object_class->flags & 0x04000000) {
                    stream.SerialiseString(object_name, sizeof(object_name));
                    original = object_class->FindObject(object_name);
                    if (original == NULL) {
                        object_class->SerialiseObject(stream, NULL, source_class, source_registry);
                        continue;
                    }
                }
                object = theRegistry.CreateObject(object_class->interface, original, 4, 0, 2);
            }
            object_class->SerialiseObject(stream, object, source_class, source_registry);
            if (object != NULL)
                theRegistry.NotifyCreateObject(object, object_class, NULL, 0, 0, 0);
        }
    };

    if (stream.mode == 2) {
        i32 count = 0;
        for (i32 index = 0; index < class_count; ++index) {
            EdClass *object_class = &classes[index];
            if (object_class->interface != NULL &&
                !(object_class->flags & (stream.flags & 0x400000 ? 0x400000 : 0x10000000)))
                ++count;
        }
        stream.SerialiseBuffer(&count, sizeof(count), 1);
        for (i32 index = 0; index < class_count; ++index) {
            EdClass *object_class = &classes[index];
            if (object_class->interface == NULL ||
                (object_class->flags & (stream.flags & 0x400000 ? 0x400000 : 0x10000000)))
                continue;
            stream.BeginBlock("ObjectList");
            stream.SerialiseString(&object_class->name);
            i32 object_count = 0;
            for (void *object = object_class->interface->vtable->get_next_object(object_class->interface, NULL);
                 object != NULL;
                 object = object_class->interface->vtable->get_next_object(object_class->interface, object)) {
                if (include_object(object_class, object))
                    ++object_count;
            }
            stream.SerialiseBuffer(&object_count, sizeof(object_count), 1);
            for (void *object = object_class->interface->vtable->get_next_object(object_class->interface, NULL);
                 object != NULL;
                 object = object_class->interface->vtable->get_next_object(object_class->interface, object)) {
                if (!include_object(object_class, object))
                    continue;
                i32 object_flags = 0;
                get_attribute(object_class, object, 1, EdType_Int, &object_flags, 0);
                stream.SerialiseBuffer(&object_flags, sizeof(object_flags), 1);
                if ((object_flags & 0x02000000) || (object_class->flags & 0x04000000)) {
                    get_attribute(object_class, object, 4, EdType_String, object_name, sizeof(object_name));
                    stream.SerialiseString(object_name, sizeof(object_name));
                }
                object_class->SerialiseObject(stream, object);
            }
            stream.EndBlock();
        }
    }

    if (stream.mode == 1) {
        if (stream.version <= 3) {
            for (i32 index = 0; index < source_registry->class_count; ++index) {
                EdClass *source_class = source_registry->GetClass(index);
                EdClass *object_class = GetClass(MapName(source_class->name));
                if (object_class != NULL && object_class->interface != NULL) {
                    if (stream.BeginBlock("ObjectList") != NULL)
                        read_objects(object_class, source_class);
                    stream.EndBlock();
                }
            }
        } else {
            i32 count;
            stream.SerialiseBuffer(&count, sizeof(count), 1);
            for (i32 index = 0; index < count; ++index) {
                if (stream.BeginBlock("ObjectList") != NULL) {
                    char class_name[128];
                    stream.SerialiseString(class_name, sizeof(class_name));
                    EdClass *source_class = source_registry->GetClass(class_name);
                    EdClass *object_class = GetClass(MapName(source_class->name));
                    if (object_class != NULL && object_class->interface != NULL)
                        read_objects(object_class, source_class);
                }
                stream.EndBlock();
            }
        }
    }
}

EdManRotate::EdManRotate() {
    STUBBED();
}

void EdManRotate::Process(EdInputContext &, ClassObjectList &) {
    STUBBED();
}

void EdManRotate::Render(ClassObjectList &) {
    STUBBED();
}

void EdManRotate::RotateItem(EdInputContext &, ClassObjectList &, i32, i32) {
    STUBBED();
}

void EdRefSpline::GetMemberData(void *object, i32 type, void *data, i32 data_size) {
    SplineObject *spline = static_cast<SplineObject *>(object);
    CheckType(type);
    switch (member_offset) {
        case static_cast<i32>(0x80000001):
            *static_cast<f32 *>(data) = spline->step;
            break;
        case static_cast<i32>(0x80000002):
            *static_cast<f32 *>(data) = spline->height;
            break;
        case static_cast<i32>(0x80000003):
            *static_cast<i32 *>(data) = spline->drop != 0;
            break;
        case static_cast<i32>(0x80000004):
            *static_cast<i32 *>(data) = spline->closed;
            break;
        default:
            EdRef::GetMemberData(object, type, data, data_size);
            break;
    }
}

void EdRefSpline::SetMemberData(void *object, i32 type, void *data, i32 data_size, i16 *) {
    SplineObject *spline = static_cast<SplineObject *>(object);
    CheckType(type);
    switch (member_offset) {
        case static_cast<i32>(0x80000001):
            spline->step = *static_cast<f32 *>(data);
            break;
        case static_cast<i32>(0x80000002):
            spline->height = *static_cast<f32 *>(data);
            break;
        case static_cast<i32>(0x80000003):
            spline->drop = *static_cast<i32 *>(data) != 0;
            break;
        case static_cast<i32>(0x80000004):
            spline->closed = *static_cast<i32 *>(data);
            break;
        default:
            // The original setter delegates unrecognized members to the getter.
            EdRef::GetMemberData(object, type, data, data_size);
            return;
    }
    if (theSplineHelper.auto_generate_points) {
        spline->points.Clear();
    }
}

void EdBitControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

void EdBitControl::Refresh() {
    STUBBED();
}

void EdBitControl::cbButton(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdBitControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdBitControl::cbSelectItem(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdDefunctList::ReviveAll(i32 flags) {
    while (first) {
        EdDefunctListEntry *entry = first;
        EdClassInterface *interface = entry->object_class->interface;
        interface->vtable->revive_object(interface, entry->object);
        if (!(flags & 2)) {
            theRegistry.NotifyReviveObject(entry->object, entry->object_class, flags);
        }
        if (entry->next) {
            entry->next->previous = entry->previous;
        } else {
            last = entry->previous;
        }
        if (entry->previous) {
            entry->previous->next = entry->next;
        } else {
            first = entry->next;
        }
        entry->next = nullptr;
        entry->previous = nullptr;
        --count;
        delete entry;
    }
}

void EdEnumControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

void EdEnumControl::GetEnumString(i32) {
    STUBBED();
}

void EdEnumControl::GetEnumValue(char *) {
    STUBBED();
}

void EdEnumControl::Refresh() {
    STUBBED();
}

void EdEnumControl::cbButton(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdEnumControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdEnumControl::cbSelectItem(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

i32 EdInputStream::SerialiseString(char **text) {
    i32 length;
    SerialiseBuffer(&length, sizeof(length), 1);
    char *allocated = (char *)memory_buffer->Allocate(length);
    *text = allocated;
    return SerialiseBuffer(allocated, 1, length);
}

i32 EdInputStream::SerialiseString(char **text, i32 capacity) {
    if (*text) {
        return SerialiseString(*text, capacity);
    }
    return SerialiseString(text);
}

i32 EdInputStream::SerialiseString(char *text, i32 capacity) {
    i32 length;
    char temporary[256];
    SerialiseBuffer(&length, sizeof(length), 1);
    if (length > capacity) {
        SerialiseBuffer(temporary, 1, sizeof(temporary));
        NuStrNCpy(text, temporary, capacity);
        text[capacity - 1] = 0;
        return capacity;
    }
    return SerialiseBuffer(text, 1, length);
}

void EdManipulator::DrawAxis(VuVec &, VuMtx *) {
    STUBBED();
}

void EdManipulator::DrawRotator(VuVec &) {
    STUBBED();
}

void EdManipulator::GetAxisLocators(VuVec &, VuVec *, VuMtx *) {
    STUBBED();
}

void EdManipulator::Process(EdInputContext &, ClassObjectList &) {
    STUBBED();
}

void EdManipulator::Render(ClassObjectList &) {
    STUBBED();
}

void EdManipulator::SelectAxis(EdInputContext &, VuVec &, VuVec &, VuVec &, VuMtx *) {
    STUBBED();
}

void EdManipulator::SelectRotator(EdInputContext &, VuVec &, VuVec &) {
    STUBBED();
}

void EdInputContext::Clear(i32 input) {
    if (static_cast<u32>(input) < 40) {
        values[input] = 0.0f;
        cleared[input] = 1;
    }
}

EdInputContext::EdInputContext() {
    STUBBED();
}

f32 EdInputContext::Get(i32 input) {
    if (static_cast<u32>(input) < 40) {
        return values[input];
    }
    return 0.0f;
}

f32 EdInputContext::GetHold(i32 input) {
    if (static_cast<u32>(input) < 40 && held[input] != 0) {
        return values[input];
    }
    return 0.0f;
}

f32 EdInputContext::GetPress(i32 input) {
    if (static_cast<u32>(input) < 40 && pressed[input] != 0) {
        return values[input];
    }
    return 0.0f;
}

f32 EdInputContext::GetRelease(i32 input) {
    if (static_cast<u32>(input) < 40 && released[input] != 0) {
        return values[input];
    }
    return 0.0f;
}

f32 EdInputContext::GetRepeat(i32 input) {
    if (static_cast<u32>(input) < 40 && repeated[input] != 0) {
        return values[input];
    }
    return 0.0f;
}

void EdInputContext::Set(i32 input, float value, float repeat_delay) {
    if (value != 0.0f) {
        float now = current_time;
        float repeat_threshold = repeat_window + now;
        values[input] = value;
        float next_repeat = repeat_times[input];
        pressed[input] = held[input] == 0;
        held[input] = 1;
        if (next_repeat >= repeat_threshold || next_repeat == 0.0f) {
            repeated[input] = 1;
        }
        repeat_times[input] = now + repeat_delay;
        return;
    }

    values[input] = value;
    released[input] = held[input] != 0;
    repeat_times[input] = 0.0f;
    held[input] = 0;
}

void EdInputContext::Update(nucamera_s *, nupad_s *, float, bool) {
    STUBBED();
}

i32 EdOutputStream::SerialiseString(char **text) {
    return SerialiseString(*text, 0);
}

i32 EdOutputStream::SerialiseString(char **text, i32) {
    return SerialiseString(*text, 0);
}

i32 EdOutputStream::SerialiseString(char *text, i32) {
    if (!text) {
        text = const_cast<char *>("(NULL)");
    }
    i32 length = NuStrLen(text) + 1;
    i32 written = SerialiseBuffer(&length, sizeof(length), 1);
    return written + SerialiseBuffer(text, 1, length);
}

void EdRefPlaceable::GetMemberData(void *object, i32 type, void *data, i32 data_size) {
    Placeable *placeable = static_cast<Placeable *>(object);
    CheckType(type);
    switch (member_offset) {
        case static_cast<i32>(0x80000001):
            *static_cast<i16 *>(data) = placeable->led_file;
            break;
        case static_cast<i32>(0x80000002):
            *static_cast<i32 *>(data) = placeable->attributes;
            break;
        case static_cast<i32>(0x80000003): {
            const char *object_name = placeable->GetName();
            NuStrNCpy(static_cast<char *>(data), object_name ? object_name : "", data_size);
            break;
        }
        case static_cast<i32>(0x80000005): {
            const char *params = placeable->params.data ? placeable->params.data + 1 : NULL;
            // The original tests the reference name before copying parameter text.
            NuStrNCpy(static_cast<char *>(data), name ? params : "", data_size);
            break;
        }
        case static_cast<i32>(0x80000006): {
            const VuMtx *matrix = placeable->GetCurrentTransform();
            if (matrix) {
                *static_cast<VuMtx *>(data) = *matrix;
            }
            break;
        }
        case static_cast<i32>(0x80000007):
            *static_cast<f32 *>(data) = placeable->GetRadius();
            break;
    }
}

void EdRefPlaceable::SetMemberData(void *object, i32 type, void *data, i32, i16 *) {
    Placeable *placeable = static_cast<Placeable *>(object);
    CheckType(type);
    switch (member_offset) {
        case static_cast<i32>(0x80000001):
            placeable->led_file = *static_cast<i16 *>(data);
            break;
        case static_cast<i32>(0x80000002):
            placeable->attributes = *static_cast<i32 *>(data);
            break;
        case static_cast<i32>(0x80000003):
            if (NuStrLen(static_cast<char *>(data)) > 0) {
                placeable->SetName(static_cast<char *>(data));
            } else {
                placeable->SetName(NULL);
            }
            break;
        case static_cast<i32>(0x80000005):
            if (NuStrLen(static_cast<char *>(data)) > 0) {
                placeable->params.Set(static_cast<char *>(data));
            } else {
                placeable->params.Set(NULL);
            }
            break;
        case static_cast<i32>(0x80000006):
            placeable->SetInitialTransform(static_cast<VuMtx *>(data));
            placeable->SetCurrentTransform(static_cast<VuMtx *>(data));
            break;
    }
}

void EdColourControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

EdColourControl::EdColourControl() {
    STUBBED();
}

void EdColourControl::Refresh() {
    STUBBED();
}

void EdColourControl::cbButton(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdColourControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdColourControl::cbColourSelected(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdMatrixControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

void EdMatrixControl::Destroy() {
    STUBBED();
}

EdMatrixControl::EdMatrixControl() {
    STUBBED();
}

void EdMatrixControl::Refresh() {
    STUBBED();
}

void EdMatrixControl::SetMenuItemAttr(i32, eduiitem_s *, eduiiattr_s *, eduiiattr_s *) {
    STUBBED();
}

void EdMatrixControl::cbButton(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdMatrixControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdMatrixControl::cbSelected(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdStringControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

EdStringControl::EdStringControl() {
    STUBBED();
}

void EdStringControl::GetVal(char *, i32) {
    STUBBED();
}

void EdStringControl::Refresh() {
    STUBBED();
}

void EdStringControl::SetVal(char const *) {
    STUBBED();
}

void EdStringControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdStringControl::cbPress(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdVectorControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

void EdVectorControl::Destroy() {
    STUBBED();
}

EdVectorControl::EdVectorControl() {
    STUBBED();
}

void EdVectorControl::Refresh() {
    STUBBED();
}

void EdVectorControl::cbButton(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdVectorControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdVectorControl::cbSelected(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdClassInterface::DistanceToObject(VuVec &, VuVec &, void *, EdRef **) {
    STUBBED();
}

void EdClassInterface::DistanceToObject(VuVec &, void *, EdRef **) {
    STUBBED();
}

void EdClassInterface::GetNextObject(void *, i32 (*)(void *)) {
    STUBBED();
}

void EdSfxNameControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

EdSfxNameControl::EdSfxNameControl() {
    STUBBED();
}

void EdSfxNameControl::cbButton(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdSfxNameControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdSfxNameControl::cbSelectSfx(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

char const *EdFileInputStream::BeginBlock(char const *name) {
    i32 position = NuFilePos(file);
    if (block_count > 0) {
        Block &block = blocks[block_count - 1];
        if (position >= block.position + block.size) {
            return NULL;
        }
    }
    char *block_name;
    if (pending) {
        block_name = block_names + pending_block.name_offset;
    } else {
        pending_block.position = position;
        pending_block.name_offset = name_length;
        pending = 1;
        SerialiseBuffer(&pending_block.size, sizeof(pending_block.size), 1);
        block_name = block_names + name_length;
        SerialiseString(block_name, sizeof(block_names) - name_length);
        name_length += NuStrLen(block_name) + 1;
    }
    if (name && NuStrICmp(name, block_name)) {
        return NULL;
    }
    if (block_count < 8) {
        blocks[block_count++] = pending_block;
    }
    pending = 0;
    return block_name;
}

i32 EdFileInputStream::Eat(i32 size, i32 count) {
    return NuFileSeek(file, size * count, NUFILE_SEEK_CURRENT);
}

void EdFileInputStream::EndBlock() {
    Block block = blocks[--block_count];
    i32 position = NuFilePos(file);
    if (position != block.position + block.size) {
        NuFileSeek(file, block.position + block.size, NUFILE_SEEK_START);
    }
    name_length = block.name_offset;
}

void EdFileInputStream::Open(i32 handle, i32) {
    file = handle;
    version = 0;
    if (BeginBlock("StreamInfo")) {
        SerialiseBuffer(&version, sizeof(version), 1);
        EndBlock();
    }
}

i32 EdFileInputStream::SerialiseBuffer(void *data, i32 size, i32 count) {
    i32 result = NuFileRead(file, data, size * count);
    if (swap_endianness && size > 1) {
        u8 *cursor = (u8 *)data;
        for (i32 i = 0; i < count; i++) {
            if (size == 2) {
                EdFileSwapEndianess16(cursor);
                cursor += 2;
            } else if (size == 4) {
                EdFileSwapEndianess32(cursor);
                cursor += 4;
            }
        }
    }
    return result;
}

char const *EdFileOutputStream::BeginBlock(char const *name) {
    i32 position = NuFilePos(file);
    if (block_count < 8) {
        block_positions[block_count++] = position;
    }
    SerialiseBuffer(&position, sizeof(position), 1);
    SerialiseString(const_cast<char *>(name), 0);
    return name;
}

i32 EdFileOutputStream::Eat(i32, i32) {
    return 0;
}

void EdFileOutputStream::EndBlock() {
    i32 position = block_positions[--block_count];
    i32 end = NuFilePos(file);
    i32 size = end - position;
    NuFileSeek(file, position, NUFILE_SEEK_START);
    SerialiseBuffer(&size, sizeof(size), 1);
    NuFileSeek(file, end, NUFILE_SEEK_START);
}

void EdFileOutputStream::Open(i32 handle, i32 stream_version) {
    file = handle;
    version = stream_version;
    if (BeginBlock("StreamInfo")) {
        SerialiseBuffer(&version, sizeof(version), 1);
        EndBlock();
    }
}

i32 EdFileOutputStream::SerialiseBuffer(void *data, i32 size, i32 count) {
    if (swap_endianness && size > 1) {
        u8 *cursor = (u8 *)data;
        for (i32 i = 0; i < count; i++) {
            if (size == 2) {
                EdFileSwapEndianess16(cursor);
                cursor += 2;
            } else if (size == 4) {
                EdFileSwapEndianess32(cursor);
                cursor += 4;
            }
        }
    }
    i32 result = NuFileWrite(file, data, size * count);
    if (swap_endianness && size > 1) {
        u8 *cursor = (u8 *)data;
        for (i32 i = 0; i < count; i++) {
            if (size == 2) {
                EdFileSwapEndianess16(cursor);
                cursor += 2;
            } else if (size == 4) {
                EdFileSwapEndianess32(cursor);
                cursor += 4;
            }
        }
    }
    return result;
}

void EdRefSpecialObject::GetMemberData(void *object, i32 type, void *data, i32) {
    SpecialObject *special_object = static_cast<SpecialObject *>(object);
    CheckType(type);
    switch (member_offset) {
        case static_cast<i32>(0x80000008):
            *static_cast<nuhspecial_s *>(data) = special_object->special;
            break;
        case static_cast<i32>(0x80000009):
            *static_cast<i32 *>(data) = NuSpecialGetVisibilityFn(&special_object->special);
            break;
        case static_cast<i32>(0x8000000a):
            *static_cast<i32 *>(data) = NuSpecialGetCollision(&special_object->special);
            break;
    }
}

void EdRefSpecialObject::SetMemberData(void *object, i32 type, void *data, i32, i16 *) {
    SpecialObject *special_object = static_cast<SpecialObject *>(object);
    CheckType(type);
    switch (member_offset) {
        case static_cast<i32>(0x80000008):
            special_object->special = *static_cast<nuhspecial_s *>(data);
            break;
        case static_cast<i32>(0x80000009):
            NuSpecialSetVisibility(&special_object->special, *static_cast<i32 *>(data));
            break;
        case static_cast<i32>(0x8000000a):
            NuSpecialSetCollision(&special_object->special, *static_cast<i32 *>(data));
            break;
    }
}

EdSpecialObjectControl::EdSpecialObjectControl() {
    STUBBED();
}

void EdSpecialObjectControl::cbButton(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdSpecialObjectControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdSpecialObjectControl::cbSelectObject(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdSpecialObjectControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

void EdSpecialObjectControl::Process(EdInputContext &) {
    STUBBED();
}

void EdSpecialObjectControl::Render() {
    STUBBED();
}

void EdClassObjectNameControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

EdClassObjectNameControl::EdClassObjectNameControl() {
    STUBBED();
}

void EdClassObjectNameControl::Process(EdInputContext &) {
    STUBBED();
}

void EdClassObjectNameControl::Render() {
    STUBBED();
}

void EdClassObjectNameControl::cbButton(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdClassObjectNameControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdClassObjectNameControl::cbSelectClass(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdClassObjectNameControl::cbSelectObject(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdRef::CheckType(i32 requested_type) {
    if (type_id == requested_type || (type_id == EdType_NuVec && requested_type == EdType_VuVec) ||
        (type_id == EdType_NuMtx && requested_type == EdType_VuMtx)) {
        return;
    }
    theRegistry.GetType(requested_type);
    theRegistry.GetType(type_id);
}

EdRef::EdRef(char *type_name, char *member_name, i32 offset, i32 member_size, i32 member_attributes,
             EdControl *member_control, i32 group)
    : next(NULL), previous(NULL), attributes(0) {
    char direct_type[32];
    i32 length = NuStrLen(type_name);
    if (length > 0 && type_name[length - 1] == '*') {
        NuStrCpy(direct_type, type_name);
        direct_type[length - 1] = '\0';
        type_name = direct_type;
        attributes |= 0x40000000;
    }
    type_id = theRegistry.GetTypeId(type_name);
    if (type_id < 0) {
        type_id = theRegistry.GetClassId(type_name);
        theRegistry.GetClass(type_id);
        attributes |= 0x80000000;
    } else {
        theRegistry.GetType(type_id);
    }
    name = member_name;
    member_offset = offset;
    size = member_size;
    attributes |= member_attributes;
    control = member_control;
    replication_group = group;
}

i32 EdRef::GetAttributeData(void *object, i32 attribute, i32 requested_type, void *data, i32 data_size) {
    if (!(attributes & attribute)) {
        return 0;
    }
    if ((attribute & 8) && type_id == EdType_VuMtx) {
        NUMTX_ALIGNED16 matrix;
        GetMemberData(object, type_id, &matrix, 0);
        VuVec *position = static_cast<VuVec *>(data);
        position->x = matrix.m30;
        position->y = matrix.m31;
        position->z = matrix.m32;
        position->w = matrix.m33;
        return 1;
    }
    if (attribute & 0x100) {
        i16 value;
        GetMemberData(object, EdType_Short, &value, 0);
        *static_cast<i16 *>(data) = value;
    } else {
        GetMemberData(object, requested_type, data, data_size);
    }
    return 1;
}

void EdRef::GetMemberData(void *object, i32 requested_type, void *data, i32 data_size) {
    void *member = static_cast<u8 *>(object) + member_offset;
    i32 type_size = GetTypeSize(requested_type, data_size);
    if (member != NULL) {
        if (attributes & 0x40000000) {
            member = *static_cast<void **>(member);
        }
        memmove(data, member, type_size);
    }
}

void *EdRef::GetMemberObject(void *object) {
    void *member = static_cast<u8 *>(object) + member_offset;
    if (member != NULL && (attributes & 0x40000000)) {
        member = *static_cast<void **>(member);
    }
    return member;
}

i32 EdRef::GetTypeSize(i32 requested_type, i32 data_size) {
    CheckType(requested_type);
    i32 type_size = size;
    if (type_size <= 0) {
        type_size = theRegistry.GetType(type_id)->size;
    }
    if (type_size > data_size && data_size > 0) {
        theRegistry.GetType(type_id);
    }
    return type_size;
}

void EdRef::Serialise(EdStream &stream, i32 *class_mapping) {
    if (stream.version != 0) {
        if (attributes < 0 && class_mapping != NULL) {
            stream.SerialiseBuffer(&class_mapping[type_id], sizeof(i32), 1);
        } else {
            stream.SerialiseBuffer(&type_id, sizeof(i32), 1);
        }
        stream.SerialiseString(&name);
        stream.SerialiseBuffer(&member_offset, sizeof(i32), 1);
        stream.SerialiseBuffer(&size, sizeof(i32), 1);
    } else {
        stream.SerialiseBuffer(&type_id, sizeof(i32), 1);
        stream.SerialiseString(&name);
        stream.SerialiseBuffer(&member_offset, sizeof(i32), 1);
    }
    stream.SerialiseBuffer(&attributes, sizeof(i32), 1);
}

i32 EdRef::SetAttributeData(void *object, i32 attribute, i32 requested_type, void *data, i32 data_size) {
    if (!(attributes & attribute)) {
        return 0;
    }
    if ((attribute & 8) && type_id == EdType_VuMtx) {
        NUMTX_ALIGNED16 matrix;
        GetMemberData(object, type_id, &matrix, 0);
        memmove(&matrix.m30, data, sizeof(VuVec));
        SetMemberData(object, EdType_VuMtx, &matrix, 0, NULL);
        return 1;
    }
    if (attribute & 0x100) {
        SetMemberData(object, EdType_Short, &data, sizeof(i16), NULL);
        return 0;
    }
    SetMemberData(object, requested_type, data, data_size, NULL);
    return 1;
}

void EdRef::SetMemberData(void *object, i32 requested_type, void *data, i32 data_size, i16 *) {
    void *member = static_cast<u8 *>(object) + member_offset;
    i32 type_size = GetTypeSize(requested_type, data_size);
    if (member != NULL) {
        if (attributes & 0x40000000) {
            member = *static_cast<void **>(member);
        }
        memmove(member, data, type_size);
    }
}

void EdType::Serialise(EdStream &stream) {
    if (stream.BeginBlock("Type")) {
        stream.SerialiseString(&name);
        stream.SerialiseBuffer(&size, sizeof(size), 1);
        stream.EndBlock();
    }
}

EdStream::EdStream() {
    memory_buffer = NULL;
    secondary_buffer = NULL;
    mode = 0;
    swap_endianness = 0;
    flags = 0;
}

EdStream::EdStream(MemoryBuffer *buffer) {
    memory_buffer = buffer;
    secondary_buffer = NULL;
    mode = 0;
    swap_endianness = 0;
    flags = 0;
}

EdStream::EdStream(MemoryBuffer *buffer, MemoryBuffer *secondary) {
    memory_buffer = buffer;
    secondary_buffer = secondary;
    mode = 0;
    swap_endianness = 0;
    flags = 0;
}

void EdString::Set(char const *value) {
    if (value == NULL) {
        if (data != NULL) {
            theMemoryManager.FreePool(data, data[0]);
            data = NULL;
        }
        return;
    }

    i32 length = NuStrLen(value);
    if (data != NULL && length + 2 > data[0]) {
        theMemoryManager.FreePool(data, data[0]);
        data = NULL;
    }
    if (data == NULL) {
        data = static_cast<char *>(theMemoryManager.AllocPool(length + 2, 1));
        data[0] = ((length + 1) / 32) * 32 + 32;
    }
    NuStrNCpy(data + 1, value, 250);
}

EdString::~EdString() {
    if (data != NULL) {
        theMemoryManager.FreePool(data, data[0]);
    }
}

void EdSystem::Initalise(variptr_u &buffer, variptr_u &buffer_end, i32 flags) {
    for (EdSubSystem *subsystem = first_subsystem; subsystem != NULL; subsystem = subsystem->next)
        subsystem->SubInitialise(buffer, buffer_end, flags);
}

void EdSystem::Process(float delta_time) {
    for (EdSubSystem *subsystem = first_subsystem; subsystem != NULL; subsystem = subsystem->next)
        subsystem->SubProcess(delta_time);
}

void EdSystem::RegisterSubSystem(EdSubSystem *subsystem) {
    subsystem->next = NULL;
    subsystem->previous = last_subsystem;
    if (last_subsystem != NULL)
        last_subsystem->next = subsystem;
    last_subsystem = subsystem;
    if (first_subsystem == NULL)
        first_subsystem = subsystem;
    ++subsystem_count;
}

void EdSystem::Render() {
    for (EdSubSystem *subsystem = first_subsystem; subsystem != NULL; subsystem = subsystem->next)
        subsystem->SubRender();
}

void EdSystem::Reset() {
    for (EdSubSystem *subsystem = first_subsystem; subsystem != NULL; subsystem = subsystem->next) {
        subsystem->SubReset();
    }
}

__attribute__((weak)) void EdSubSystem::SubInitialise(variptr_u &, variptr_u &, i32) {
    STUBBED();
}

__attribute__((weak)) void EdSubSystem::SubReset() {
    STUBBED();
}

__attribute__((weak)) void EdSubSystem::SubProcess(float) {
    STUBBED();
}

__attribute__((weak)) void EdSubSystem::SubRender() {
    STUBBED();
}

void EdControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

void EdControl::Process(EdInputContext &) {
    STUBBED();
}

void EdControl::Render() {
    STUBBED();
}

void EdControl::SelectSubObject() {
    STUBBED();
}

void EdControl::Refresh() {
    STUBBED();
}

void EdControl::SetMenuItemAttr(i32, eduiitem_s *, eduiiattr_s *, eduiiattr_s *) {
    STUBBED();
}

void EdControl::cbSelected(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

EdManMove::EdManMove() {
    STUBBED();
}

void EdManMove::Process(EdInputContext &, ClassObjectList &) {
    STUBBED();
}

void EdManMove::Render(ClassObjectList &) {
    STUBBED();
}

void EdRefKnot::GetMemberData(void *object, i32 type, void *data, i32 data_size) {
    SplineKnot *knot = static_cast<SplineKnot *>(object);
    CheckType(type);
    switch (member_offset) {
        case static_cast<i32>(0x80000001):
            NuStrNCpy(static_cast<char *>(data), knot->spline->name, data_size);
            break;
        case static_cast<i32>(0x80000002):
            *static_cast<f32 *>(data) = 0.25f * EdManipulator::Scale;
            break;
        case static_cast<i32>(0x80000003):
            *static_cast<VuVec *>(data) = knot->position;
            break;
        case static_cast<i32>(0x80000004):
            *static_cast<VuVec *>(data) = knot->in_tangent;
            break;
        case static_cast<i32>(0x80000005):
            *static_cast<VuVec *>(data) = knot->out_tangent;
            break;
        default:
            EdRef::GetMemberData(object, type, data, data_size);
            break;
    }
}

void EdRefKnot::SetMemberData(void *object, i32 type, void *data, i32 data_size, i16 *) {
    SplineKnot *knot = static_cast<SplineKnot *>(object);
    CheckType(type);
    switch (member_offset) {
        case static_cast<i32>(0x80000001):
        case static_cast<i32>(0x80000002):
            return;
        case static_cast<i32>(0x80000003): {
            VuVec *position = static_cast<VuVec *>(data);
            f32 dx = position->x - knot->position.x;
            f32 dy = position->y - knot->position.y;
            f32 dz = position->z - knot->position.z;
            knot->position.x += dx;
            knot->position.y += dy;
            knot->position.z += dz;
            knot->position.w = 1.0f;
            if (!theClassEditor.IsSelectedObject(knot, theKnotHelper.in_tangent_ref)) {
                knot->in_tangent.x += dx;
                knot->in_tangent.y += dy;
                knot->in_tangent.z += dz;
                theClassEditor.SnapPoint(knot->in_tangent);
            }
            if (!theClassEditor.IsSelectedObject(knot, theKnotHelper.out_tangent_ref)) {
                knot->out_tangent.x += dx;
                knot->out_tangent.y += dy;
                knot->out_tangent.z += dz;
                theClassEditor.SnapPoint(knot->out_tangent);
            }
            break;
        }
        case static_cast<i32>(0x80000004):
            knot->in_tangent = *static_cast<VuVec *>(data);
            break;
        case static_cast<i32>(0x80000005):
            knot->out_tangent = *static_cast<VuVec *>(data);
            break;
        default:
            // The original setter delegates unrecognized members to the getter.
            EdRef::GetMemberData(object, type, data, data_size);
            return;
    }
    if (theSplineHelper.auto_generate_points && knot->spline) {
        knot->spline->points.Clear();
    }
}
