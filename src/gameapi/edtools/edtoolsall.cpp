// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edgra.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/render/fx.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numusic/sfx.h"
#include <stdio.h>
#include <string.h>
#include <new>
#if defined(__SSE__)
#include <xmmintrin.h>
#endif
#include "nu2api/numath/nurand.h"

extern EdRegistry theRegistry;
extern "C" VuVec SpherePos1;
extern "C" VuVec SpherePos2;
VuVec SpherePos1;
VuVec SpherePos2;
EdInputContext *EdControl::Input;
extern MemoryManager theMemoryManager;
extern LevelEditor theLevelEditor;
extern PropertyTool thePropertyTool;
extern eduiiattr_s EdLevelAttr;
extern i32 EdLevelFnt;
extern "C" void eduiSetCameraEnabled(i32);
extern "C" void eduicbItemDestroy(eduimenu_s *, eduiitem_s *);
extern "C" i32 edbri_nearest;
extern "C" NUVEC edbri_cam_pos;
extern "C" NUMTL *edbri_mtl, *edbri_mtl_zoff;
extern "C" f32 edbri_length, edbri_width;
extern "C" i32 edbri_rotz, edbri_roty, edbri_planks, edbri_post_interval;
extern "C" i32 edbri_plank_instance_type, edbri_post_instance_type, edbri_bridges_used;
extern "C" void NuRndrRect2di(i32, i32, i32, i32, i32, NUMTL *);
extern "C" void NuRndrLine2di(i32, i32, i32, i32, i32, NUMTL *);
extern "C" void edbitsDrawCircleTilted(NUVEC *, f32, i32, NUMTL *, i32, i32);
NUVEC NuFadeObjGetAngleTerrainValues(NUVEC *);
extern "C" void eduiAddPropTextPickEnt(eduimenu_s *, eduiitem_s *);
extern "C" char *GetSfxName(i32);
extern "C" void PlaySfxById(i32, nuvec_s *);
extern "C" eduiitem_s *eduiItemColourPickCreate(usize, const void *, EdUiItemCallback, char *);
extern "C" void eduiItemColourPickSetRGB(edui_colour_pick_s *, f32, f32, f32);
static i32 get_manipulator_attribute(ClassObjectListEntry *, i32, i32, void *);
static void set_manipulator_attribute(ClassObjectListEntry *, i32, i32, void *);
void cbEdLevelDestroy(eduimenu_s *, eduimenu_s *);
void cbEdLevelDestroyOnSelect(eduimenu_s *, eduiitem_s *, u32);
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
extern EdManipulator theDefaultManipulator;
extern EdManMove theMoveManipulator;
extern EdManRotate theRotateManipulator;
extern EdManScale theScaleManipulator;
extern SplineTool theSplineTool;
DECOMP_ASSERT(sizeof(EdManipulator) == 0x6c, "EdManipulator ABI");
extern SplineHelper theSplineHelper;
extern KnotHelper theKnotHelper;
extern ClassEditor theClassEditor;
i32 pad_disabled;
extern EdSystem theEdSystem;
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
void EdDrawPolyArrow(VuVec const &, VuVec const &, i32, i32, float, float, float, float);
void EdDrawPolyCylinder(VuMtx const &, float, float, float, i32, i32, i32, i32);
void EdDrawMtx(VuMtx const *);
char *EDSPLINE_FILECHECK = const_cast<char *>("EDSPLINE v. ");

extern "C" {
    char edgra_filter_string[16] = "GRASS";
extern i32 edgra_mode;
    extern NUGSCN *edbits_base_scene;
    extern NUGSCN *edbits_things_scene;
    extern part_type_s part_types[128];
    extern part_emit_s part_emits[40];
    extern i32 edpart_emitrotx, edpart_emitroty, edpart_emitrotz;
    extern NUVEC edanim_cam_pos;
extern i32 edgra_last_clump_in_buffer;
extern i32 edgra_copy_source;
extern f32 edgra_global_fadein;
extern f32 edgra_global_fadeout;
    void edgraInitAllClumps(void);
    void DebFreeInstantly(i32 *);
    void DebrisEmitterPos(i32, f32, f32, f32);
    void DebrisOrientation(i32, i16, i16);
    void DebrisEmitterOrientation(i32, i16, i16, i16);
    void DebrisReflectionOrientation(i32, i16, i16, f32, f32);
    void DebrisSetFacing(i32, u8, i16, i16);
    void DebrisStartOffset(i32, f32);
    void DebrisSetGroupID(i32, i16);
    void DebrisSetPriority(i32, u16, u8);
    void DebrisSetRoomID(i32, NUGSCN *);
    void DebrisSetDetailLevels(i32, i32);
    void DebrisSetTrigger(i32, i32, i32, f32);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    i32 AddPARTEffect(i32, NUVEC *);
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern debinftype *effecttypes;
    extern i32 EDPP_MAX_TYPES;
    extern i32 edpp_usememcard;
    extern i32 edpp_copy_mode, edpp_copyrotz, edpp_copyroty, edpp_copy_source_count, edpp_copy_enclosed;
    extern i32 edpp_readout, edpp_num_orphans;
    extern i32 globalframes;
    extern f32 edpp_copy_size;
    extern NUMTL *edpp_mtl, *edpp_boxmtl;
    void edppDrawSpheres(debinftype *, i32);
    void edppDrawTorus(debinftype *, i32);
    extern i32 part_page_used[8];
    extern i32 edanim_params_used;
    extern i32 edanim_particle_type;
    extern i32 edanim_emitrotz;
    extern i32 edanim_emitroty;
    extern f32 edpp_offset;
extern i32 edbits_particle_level_page;
}

void edpartDestroy(i32 index);

i32 edpartLookupObjectInScene(char *, NUGSCN *);

void EdTerrInit(void *, void *) {
}

void edDrawLine(nuvec_s *, nuvec_s *, unsigned char, unsigned char, unsigned char);

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

void edpartPlace(i32 index, nuvec_s *position);

void edppDetermineNearest(f32);
void edppMultipleCopyPaste();
void edppMultipleCopyCopy();
void edppMultipleCopyClear();
i32 edppPtlCreate(NUVEC *, i32);
i32 edppPtlPlace(i32, NUVEC *);
void edppPtlDestroy(i32);

void edppDoInput(nupad_s *pad);

void EdTerrShadow(nuvec_s *position, float height_above, float height_below, i32 terrain_mask) {
    reinterpret_cast<void (*)(nuvec_s *, f32, f32, i32)>(NewShadow)(position, height_above, height_below, terrain_mask);
}

void edpartCreate(nuvec_s *position, i32 type);

i32 edppPtlPlace(i32 index, NUVEC *position);

void EdDrawPolyTri(VuVec const &a, VuVec const &b, VuVec const &c, i32 colour) {
    if (__builtin_expect(NewPrim == 1, 0)) {
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

void edanimDoInput(nupad_s *pad);

i32 edbriFileSave(char *path);

i32 edppPtlCreate(NUVEC *position, i32 effect_index);

i32 edgraFileSave(char *path);

void edppPtlShelve(i32 index);

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

void EdDrawPolyAxis(VuMtx const &transform, float size, i32 opacity) {
    const NUMTX &matrix = transform.matrix;
    const VuVec &origin = *reinterpret_cast<const VuVec *>(&matrix.m30);
    VuVec tip(matrix.m30 + matrix.m00 * size, matrix.m31 + matrix.m01 * size, matrix.m32 + matrix.m02 * size, 0.0f);
    const float width = size * 0.02f;
    EdDrawPolyArrow(origin, tip, 8, static_cast<i32>(0xff000000 | (opacity & 0xff)), width, width, 0.02f, 0.0f);
    tip = VuVec(matrix.m30 + matrix.m10 * size, matrix.m31 + matrix.m11 * size, matrix.m32 + matrix.m12 * size, 0.0f);
    EdDrawPolyArrow(origin, tip, 8, static_cast<i32>(0xff000000 | ((static_cast<u32>(opacity) << 8) & 0xffff)), width,
                    width, 0.02f, 0.0f);
    tip = VuVec(matrix.m30 + matrix.m20 * size, matrix.m31 + matrix.m21 * size, matrix.m32 + matrix.m22 * size, 0.0f);
    EdDrawPolyArrow(origin, tip, 8, static_cast<i32>(0xff000000 | ((opacity & 0xff) << 16)), width, width, 0.02f, 0.0f);
}

i32 edanimFileSave(char *path);

void edpartInitType(i32 index);

void edppDrawCursor();

extern "C" {
    void DebFreeInstantly(i32 *);
}

void edppPtlDestroy(i32 index);

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

void EdDrawPolyArrow(VuVec const &start, VuVec const &end, i32 sides, i32 colour, float radius, float limit,
                     float radius_factor, float radius_offset) {
    VuVec displacement(end.x - start.x, end.y - start.y, end.z - start.z, 0.0f);
    const float length = NuVecMag(&displacement.xyz);
    if (!(length > 0.0f))
        return;
    const float inverse_length = 1.0f / length;
    VuVec direction(displacement.x * inverse_length, displacement.y * inverse_length, displacement.z * inverse_length,
                    0.0f);
    const float half_length = length * 0.4f;
    const float minimum_radius = radius_factor * half_length + radius_offset;

    NUANGVEC angles;
    if (direction.x == 0.0f && direction.z == 0.0f) {
        angles.x = -0x4000;
        angles.y = 0;
    } else {
        angles.y = NuAtan2D(direction.x, direction.z);
        NuVecRotateY(&direction.xyz, &direction.xyz, -angles.y);
        angles.x = -NuAtan2D(direction.y, direction.z);
    }
    angles.z = 0;
    const float draw_radius = radius > minimum_radius ? radius : minimum_radius;
    const float draw_limit = limit < draw_radius ? limit : draw_radius;

    VuMtx transform;
    VuVec center(start.x + displacement.x * 0.4f, start.y + displacement.y * 0.4f, start.z + displacement.z * 0.4f,
                 0.0f);
    NuMtxSetRotateXYZVU0(&transform.matrix, &angles);
    NuMtxTranslate(&transform.matrix, &center.xyz);
    EdDrawPolyCylinder(transform, half_length, draw_limit, draw_limit, sides, colour, 1, 0);
    NuMtxTranslateNeg(&transform.matrix, &center.xyz);
    center = VuVec(end.x - displacement.x * 0.1f, end.y - displacement.y * 0.1f, end.z - displacement.z * 0.1f, 0.0f);
    NuMtxTranslate(&transform.matrix, &center.xyz);
    EdDrawPolyCylinder(transform, half_length * 0.25f, draw_limit * 1.6f, 0.0f, sides, colour, 1, 0);
}

void edbriDrawCursor();

void edgraClumpPlace(i32 index, NUVEC *position);

void edgraDrawCursor();

void edpartPtlShelve(i32 index);

void edpartScaleType(i32 index, float scale);

i32 edppSaveEffects(char *filename, char page);

void EdDrawLineSphere(VuVec const &center, float radius, float scale, i32 colour) {
    const float radius_squared = radius * radius;
    VuVec first;
    VuVec second;
    for (i32 latitude = 0; latitude < 8; ++latitude) {
        float first_radius;
        float second_radius;
        if (latitude < 4)
            first_radius = radius * NU_SIN_LUT(latitude * 0x1000);
        else
            first_radius = radius * NU_SIN_LUT((8 - latitude) * 0x1000);
        if (latitude < 3)
            second_radius = radius * NU_SIN_LUT((latitude + 1) * 0x1000);
        else
            second_radius = radius * NU_SIN_LUT((7 - latitude) * 0x1000);
        float first_height = NuFsqrt(radius_squared - first_radius * first_radius) * scale;
        float second_height = NuFsqrt(radius_squared - second_radius * second_radius) * scale;
        if (latitude >= 5)
            first_height = -first_height;
        if (latitude >= 4)
            second_height = -second_height;
        first_height += center.y;
        second_height += center.y;
#define ED_DRAW_SPHERE_LONGITUDE(angle, next_angle)                                                                    \
    {                                                                                                                  \
        first = VuVec(center.x + first_radius * NU_COS_LUT(angle), first_height,                                       \
                      center.z + first_radius * NU_SIN_LUT(angle), 1.0f);                                              \
        second = VuVec(center.x + second_radius * NU_COS_LUT(angle), second_height,                                    \
                       center.z + second_radius * NU_SIN_LUT(angle), 1.0f);                                            \
        EdDrawLineSegment(first, second, colour);                                                                      \
        if (latitude != 0) {                                                                                           \
            second = VuVec(center.x + first_radius * NU_COS_LUT(next_angle), first_height,                             \
                           center.z + first_radius * NU_SIN_LUT(next_angle), 1.0f);                                    \
            EdDrawLineSegment(first, second, colour);                                                                  \
        }                                                                                                              \
    }
        ED_DRAW_SPHERE_LONGITUDE(0x0000, 0x2000);
        ED_DRAW_SPHERE_LONGITUDE(0x2000, 0x4000);
        ED_DRAW_SPHERE_LONGITUDE(0x4000, 0x6000);
        ED_DRAW_SPHERE_LONGITUDE(0x6000, 0x8000);
        ED_DRAW_SPHERE_LONGITUDE(0x8000, 0xa000);
        ED_DRAW_SPHERE_LONGITUDE(0xa000, 0xc000);
        ED_DRAW_SPHERE_LONGITUDE(0xc000, 0xe000);
        ED_DRAW_SPHERE_LONGITUDE(0xe000, 0x10000);
    }
}

void EdDrawPolySector(VuVec const &center, float radius, i32 axis, i32 first_angle, i32 last_angle, i32 colour,
                      i32 segments) {
    i32 step = 0x1000;
    if (segments != 0)
        step = 0x10000 / segments;
    i32 angle = first_angle;
    i32 remaining;
    if (first_angle < last_angle) {
        remaining = (last_angle - first_angle) & 0xffff;
        if (remaining >= 0x8000)
            return;
    } else {
        remaining = (first_angle - last_angle) & 0xffff;
        if (remaining >= 0x8000)
            return;
        angle = last_angle;
    }
    if (remaining <= 0)
        return;
    const float negative_radius = -radius;
    do {
        const i32 previous_angle = angle;
        if (remaining > step) {
            angle += step;
            remaining -= step;
        } else {
            angle += remaining;
            remaining = 0;
        }
        VuVec first;
        VuVec second;
        switch (axis) {
            case 1:
                first.w = 1.0f;
                first.x = negative_radius * 0.0f;
                first.y = negative_radius * 0.0f;
                first.z = negative_radius;
                NuVecRotateY(&first.xyz, &first.xyz, -previous_angle);
                second.w = 1.0f;
                second.y = negative_radius * 0.0f;
                second.x = negative_radius * 0.0f;
                second.z = negative_radius;
                NuVecRotateY(&second.xyz, &second.xyz, -angle);
                break;
            case 2:
                first.w = 1.0f;
                first.x = 0.0f * radius;
                first.y = radius;
                first.z = 0.0f * radius;
                NuVecRotateZ(&first.xyz, &first.xyz, -previous_angle);
                second.w = 1.0f;
                second.x = 0.0f * radius;
                second.y = radius;
                second.z = 0.0f * radius;
                NuVecRotateZ(&second.xyz, &second.xyz, -angle);
                break;
            case 0:
                first.w = 1.0f;
                first.x = 0.0f * radius;
                first.y = 0.0f * radius;
                first.z = radius;
                NuVecRotateX(&first.xyz, &first.xyz, -previous_angle);
                second.w = 1.0f;
                second.y = 0.0f * radius;
                second.x = 0.0f * radius;
                second.z = radius;
                NuVecRotateX(&second.xyz, &second.xyz, -angle);
                break;
        }
        first.x += center.x;
        first.y += center.y;
        first.z += center.z;
        second.x += center.x;
        second.y += center.y;
        second.z += center.z;
        EdDrawPolyTri(center, first, second, colour);
    } while (remaining > 0);
}

void edanimDrawCursor();

extern "C" i32 part_types_used;
extern "C" i32 part_emits_used;

void edpartDrawCursor();

i32 edgraClumpCreate(NUVEC *position);

template <i32 Axis> static inline void EdDrawCircle(VuVec const &centre, float radius, i32 colour, i32 segments) {
    VuVec base(0.0f, Axis == 2 ? radius : 0.0f, Axis == 2 ? 0.0f : radius, 0.0f);
    VuVec point(base.x + centre.x, base.y + centre.y, base.z + centre.z, 0.0f);
    for (i32 i = 1; i <= segments; i++) {
        VuVec previous(point.x, point.y, point.z, point.w);
        i32 angle = i * (0x10000 / segments);
        if (Axis == 0) {
            NuVecRotateX(&point.xyz, &base.xyz, angle);
        } else if (Axis == 1) {
            NuVecRotateY(&point.xyz, &base.xyz, angle);
        } else {
            NuVecRotateZ(&point.xyz, &base.xyz, angle);
        }
        point = VuVec(point.x + centre.x, point.y + centre.y, point.z + centre.z, 0.0f);
        EdDrawLineSegment(previous, point, colour);
    }
}

void EdDrawLineCircleX(VuVec const &centre, float radius, i32 colour, i32 segments) {
    EdDrawCircle<0>(centre, radius, colour, segments);
}

void EdDrawLineCircleZ(VuVec const &centre, float radius, i32 colour, i32 segments) {
    EdDrawCircle<2>(centre, radius, colour, segments);
}

void EdDrawLineCircleY(VuVec const &centre, float radius, i32 colour, i32 segments) {
    EdDrawCircle<1>(centre, radius, colour, segments);
}

void EdDrawLineSegment(VuVec const &a, VuVec const &b, i32 colour) {
    if (__builtin_expect(NewPrim == 1, 0)) {
        NuPrim3DBegin(2, 5, EdDrawMtl[NewMtl], NewMtx ? const_cast<NUMTX *>(&NewMtx->matrix) : NULL);
        NewPrim = 2;
    }
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(a.x, a.y, a.z);
    NuRndrPrimSetColour(colour);
    NuRndrPrimPosition(b.x, b.y, b.z);
}

i32 edanimParamCreate(i32 instance_id);

i32 edpartSaveEffects(char *filename, char page);

void edppPtlChangeType(i32 index, i32 effect_index);

i32 edppPtlCreateCopy(NUVEC *position, i32 source_index);

void EdDrawPolyCylinder(VuMtx const &transform, float half_length, float radius, float end_radius, i32 sides,
                        i32 colour, i32 cap_start, i32 cap_end) {
    const float taper = end_radius / radius;
    i32 shaded_colour = colour & 0xff000000;
    shaded_colour |= ((colour & 0xff) * 0xdc) >> 8;
    shaded_colour |= ((((colour >> 8) & 0xff) * 0xdc) & 0xff00) | (((((colour >> 16) & 0xff) * 0xdc) >> 8) << 16);
    VuVec first;
    VuVec second;
    VuVec third;
    EdDrawMtx(&transform);
    i32 segment_colour = 0;
    float previous_sine = NU_SIN_LUT(0) * radius;
    float previous_cosine = NU_COS_LUT(0) * radius;
    for (i32 segment = 0; segment < sides; ++segment) {
        segment_colour = (segment & 1) == 0 ? colour : shaded_colour;
        const i32 angle = (segment + 1) * 0x10000 / sides;
        const float sine = NU_SIN_LUT(angle) * radius;
        const float cosine = NU_COS_LUT(angle) * radius;
        first = VuVec(previous_sine * taper, previous_cosine * taper, half_length, 1.0f);
        second = VuVec(sine * taper, cosine * taper, half_length, 1.0f);
        third = VuVec(sine, cosine, -half_length, 1.0f);
        EdDrawPolyTri(first, second, third, segment_colour);
        first = VuVec(previous_sine * taper, previous_cosine * taper, half_length, 1.0f);
        second = VuVec(sine, cosine, -half_length, 1.0f);
        third = VuVec(previous_sine, previous_cosine, -half_length, 1.0f);
        EdDrawPolyTri(first, second, third, segment_colour);
        previous_sine = sine;
        previous_cosine = cosine;
    }
    if (cap_start != 0 || cap_end != 0) {
        float first_sine = NU_SIN_LUT(0) * radius;
        float first_cosine = NU_COS_LUT(0) * radius;
        const i32 first_angle = 0x10000 / sides;
        float previous_sine = NU_SIN_LUT(first_angle) * radius;
        float previous_cosine = NU_COS_LUT(first_angle) * radius;
        for (i32 segment = 0; segment < sides - 2; ++segment) {
            const i32 angle = (segment + 2) * 0x10000 / sides;
            const float sine = NU_SIN_LUT(angle) * radius;
            const float cosine = NU_COS_LUT(angle) * radius;
            if (cap_end != 0) {
                first = VuVec(first_sine * taper, first_cosine * taper, half_length, 1.0f);
                second = VuVec(sine * taper, cosine * taper, half_length, 1.0f);
                third = VuVec(previous_sine * taper, previous_cosine * taper, half_length, 1.0f);
                EdDrawPolyTri(first, second, third, segment_colour);
            }
            if (cap_start != 0) {
                first = VuVec(first_sine, first_cosine, -half_length, 1.0f);
                second = VuVec(previous_sine, previous_cosine, -half_length, 1.0f);
                third = VuVec(sine, cosine, -half_length, 1.0f);
                EdDrawPolyTri(first, second, third, segment_colour);
            }
            previous_sine = sine;
            previous_cosine = cosine;
        }
    }
    EdDrawMtx(NULL);
}

__attribute__((force_align_arg_pointer)) void EdDrawPolyCylinder(VuVec const &start, VuVec const &end, i32 sides,
                                                                 i32 colour, i32 cap_colour, float radius, float limit,
                                                                 float offset) {
    VuVec direction(end.x - start.x, end.y - start.y, end.z - start.z, 0.0f);
    const float length = NuVecMag(&direction.xyz);
    if (!(length > 0.0f))
        return;
    const float inverse_length = 1.0f / length;
    direction.x *= inverse_length;
    direction.y *= inverse_length;
    direction.z *= inverse_length;
    NUANGVEC angles;
    if (direction.x == 0.0f && direction.z == 0.0f) {
        angles.x = 0x2000;
        angles.y = 0;
    } else {
        angles.y = NuAtan2D(direction.x, direction.z);
        NuVecRotateY(&direction.xyz, &direction.xyz, -angles.y);
        angles.x = -NuAtan2D(direction.y, direction.z);
    }
    angles.z = 0;
    VuVec center((end.x + start.x) * 0.5f, (end.y + start.y) * 0.5f, (end.z + start.z) * 0.5f, 0.0f);
    VuMtx transform;
    NuMtxSetRotateXYZVU0(&transform.matrix, &angles);
    NuMtxTranslate(&transform.matrix, &center.xyz);
    const float half_length = length * 0.5f;
    const float minimum_radius = radius * half_length;
    float first_radius = limit > minimum_radius ? limit : minimum_radius;
    if (first_radius > offset)
        first_radius = offset;
    float second_radius = limit > minimum_radius * 0.1f ? limit : minimum_radius * 0.1f;
    if (second_radius > offset)
        second_radius = offset;
    EdDrawPolyCylinder(transform, half_length, first_radius, second_radius, sides, colour, 1, 1);
}

void edanimParamDestroy(i32 index);

void edbitsDoSingleDump(i32 face);

void edgraCalculatePage(char page, i32 calculate_vectors);

void edgraInstancePlace(i32 index, NUVEC *position);

i32 edpartLookupObject(char *name);

i32 edSpline_FindAllBeg(NUGSCN *scene, char *prefix, NUGSPLINE **results, i32 capacity) {
    i32 count = 0;
    if (capacity <= 0)
        return 0;
    if (splineStore != NULL) {
        NUGSPLINE *spline = splineStore;
        for (i32 i = 0; i < numSplinesLoaded; ++spline, ++i) {
            if (NuStrNICmp(prefix, spline->name, -1) == 0) {
                results[count++] = spline;
                if (count >= capacity)
                    return count;
            }
        }
    }
    if (scene != NULL) {
        NUGSPLINE *spline = scene->splines;
        for (i32 i = 0; i < scene->numsplines; ++spline, ++i) {
            if (NuStrNICmp(prefix, spline->name, -1) == 0) {
                results[count++] = spline;
                if (count >= capacity)
                    return count;
            }
        }
    }
    return count;
}

i32 LoadEditorSplines(char *path, VARIPTR *buf, VARIPTR *buf_end) {
    NUGSPLINE *store = reinterpret_cast<NUGSPLINE *>(buf->void_ptr);
    splineStore = store;
    buf->addr = (buf->addr + 3) & ~static_cast<usize>(3);

    EdFileSetMedia(1);
    if (EdFileOpen(path, NUFILE_READ) == 0) {
        splineStore = NULL;
        return 0;
    }

    const i32 file_check_length = NuStrLen(EDSPLINE_FILECHECK);
    for (i32 i = 0; i < file_check_length; ++i) {
        if (EDSPLINE_FILECHECK[i] != EdFileReadChar()) {
            EdFileClose();
            splineStore = NULL;
            return 0;
        }
    }

    EdFileReadInt();
    i32 spline_count = EdFileReadInt();
    i32 point_count = EdFileReadInt();
    i32 string_bytes = EdFileReadInt();

    char *name_cursor = reinterpret_cast<char *>(store + spline_count);
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
        splineStore[i].name = name_cursor;
        splineStore[i].pt_size = sizeof(NUVEC);
        splineStore[i].length = static_cast<i16>(EdFileReadInt());
        splineStore[i].pts = point_cursor;

        for (i32 j = 0; j < name_length; ++j) {
            splineStore[i].name[j] = EdFileReadChar();
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

        for (i32 j = 0; j < splineStore[i].length; ++j) {
            NUVEC *point = &splineStore[i].pts[j];
            point->x = EdFileReadFloat();
            point->y = EdFileReadFloat();
            point->z = EdFileReadFloat();
        }
        point_cursor += splineStore[i].length;
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

i32 edSpline_FindAllSub(NUGSCN *scene, char *name, NUGSPLINE **results, i32 capacity) {
    i32 count = 0;
    if (capacity <= 0)
        return 0;
    if (splineStore != NULL) {
        NUGSPLINE *spline = splineStore;
        for (i32 i = 0; i < numSplinesLoaded; ++spline, ++i) {
            if (NuStrIStr(spline->name, name) != NULL) {
                results[count++] = spline;
                if (count >= capacity)
                    return count;
            }
        }
    }
    if (scene != NULL) {
        NUGSPLINE *spline = scene->splines;
        for (i32 i = 0; i < scene->numsplines; ++spline, ++i) {
            if (NuStrIStr(spline->name, name) != NULL) {
                results[count++] = spline;
                if (count >= capacity)
                    return count;
            }
        }
    }
    return count;
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
}

void edanimParticlePlace(i32 index, NUVEC *position);

void edanimStartAllPages();

void edgraInstanceCreate(NUVEC *position);

i32 edpartPtlChangeType(i32 index, i32 type);

void edppDestroyAllPages();

void edanimParticleCreate(NUVEC *position);

void edgraInstanceDestroy(i32 index);

void edppDetermineNearest(float max_distance_squared);

void edppHighlightNearest();


void edbriDetermineNearest(float distance);

void edgraSortVectorBuffer(i32 index);

void edpartDestroyAllPages();



void edppStartSingleEffect(i32 index);

void edpartHighlightNearest();

void edpartMultipleCopyCopy();

void edpartMultipleCopyClear();

float edanimPlayerAnimDistance(i32 parameter_index);

void edanimRenderSoundEmitters(i32 parameter_index);

void edanimDetermineNearestAnim(float distance);

void edgraDetermineNearestClump(f32 distance);

eduiitem_s *eduiItemFileSelectorCreate(u32 data, eduiiattr_s *colours, EdUiItemCallback callback, char *text);

void edanimRenderParticleEmitters(i32 parameter_index);

void edanimDetermineNearestSound(float distance);

void edgraDetermineNearestInstance(f32 distance);

void edanimDetermineNearestParticle(float distance);

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

i32 EdTerrRay(VuVec &, VuVec &) {
    return 0;
}

EdManScale::EdManScale() {
    selected_attribute = 32;
}

// The original realigns the stack for its matrix and vector locals.
__attribute__((force_align_arg_pointer)) i32 EdManScale::Process(EdInputContext &input, ClassObjectList &selected) {
    VuVec projected;
    EdManipulator::Process(input, selected);
    VuVec average;
    if (selected.GetAveragePosition(average) == 0)
        return 0;

    VuMtx transform;
    EdMember member;
    i32 matrix_type = EdType_VuMtx;
    ClassObjectListEntry *first = selected.first;
    if (first->reference == NULL ||
        first->reference->GetAttributeData(first->object, 0x10, matrix_type, &transform, 0) == 0) {
        if (first->ed_class->FindMember(&member, first->object, 0x10, 1) != 0)
            member.reference->GetAttributeData(member.object, 0x10, matrix_type, &transform, 0);
    }
    VuVec second_axis;
    VuVec first_axis;
    i32 axis = SelectAxis(input, average, first_axis, second_axis, &transform);
    theLevelEditor.field_0x2c = AxisColour[axis];
    if (input.GetHold(3) != 0.0f) {
        VuVec const &delta = *reinterpret_cast<VuVec const *>(reinterpret_cast<u8 *>(this) + 0x40);
        for (ClassObjectListEntry *entry = selected.first; entry != NULL; entry = entry->next) {
            average = VuVec(0.0f, 0.0f, 0.0f, 1.0f);
            matrix_type = EdType_VuMtx;
            if (__builtin_expect(entry->reference == NULL ||
                entry->reference->GetAttributeData(entry->object, 0x20, matrix_type, &transform, 0) == 0, 0)) {
                if (__builtin_expect(entry->ed_class->FindMember(&member, entry->object, 0x20, 1) != 0, 1))
                    member.reference->GetAttributeData(member.object, 0x20, matrix_type, &transform, 0);
            }

            f32 scale_x = 1.0f;
            f32 scale_y = 1.0f;
            f32 scale_z = 1.0f;
            switch (axis) {
                case 1:
                case 2:
                case 3: {
                    NUMTX &matrix = transform.matrix;

                    projected.x = matrix.m00 * first_axis.x + matrix.m10 * first_axis.y + matrix.m20 * first_axis.z;
                    projected.y = matrix.m01 * first_axis.x + matrix.m11 * first_axis.y + matrix.m21 * first_axis.z;
                    projected.z = matrix.m02 * first_axis.x + matrix.m12 * first_axis.y + matrix.m22 * first_axis.z;
                    f32 magnitude = NuVecMag(reinterpret_cast<NUVEC *>(&projected));
                    f32 scaled_magnitude = Scale * magnitude;
                    f32 movement = delta.x * first_axis.x + delta.y * first_axis.y + delta.z * first_axis.z;
                    if (movement == 0.0f)
                        continue;
                    NuVecInvMtxRotate(reinterpret_cast<NUVEC *>(&first_axis), reinterpret_cast<NUVEC *>(&first_axis),
                                      &matrix);
                    NuVecNorm(reinterpret_cast<NUVEC *>(&first_axis), reinterpret_cast<NUVEC *>(&first_axis));
                    f32 change = (scaled_magnitude + movement) / scaled_magnitude - 1.0f;
                    scale_y = first_axis.y * change + 1.0f;
                    scale_x = first_axis.x * change + 1.0f;
                    scale_z = first_axis.z * change + 1.0f;
                    break;
                }
                case 4:
                case 5:
                case 6: {
                    NUMTX &matrix = transform.matrix;
                    VuVec projected;
                    projected.x = matrix.m00 * first_axis.x + matrix.m10 * first_axis.y + matrix.m20 * first_axis.z;
                    projected.y = matrix.m01 * first_axis.x + matrix.m11 * first_axis.y + matrix.m21 * first_axis.z;
                    projected.z = matrix.m02 * first_axis.x + matrix.m12 * first_axis.y + matrix.m22 * first_axis.z;
                    f32 magnitude = NuVecMag(reinterpret_cast<NUVEC *>(&projected));
                    f32 scaled_magnitude = Scale * magnitude;
                    f32 movement = delta.x * first_axis.x + delta.y * first_axis.y + delta.z * first_axis.z;
                    movement += delta.x * second_axis.x + delta.y * second_axis.y + delta.z * second_axis.z;
                    if (movement == 0.0f)
                        continue;
                    NuVecInvMtxRotate(reinterpret_cast<NUVEC *>(&first_axis), reinterpret_cast<NUVEC *>(&first_axis),
                                      &matrix);
                    NuVecNorm(reinterpret_cast<NUVEC *>(&first_axis), reinterpret_cast<NUVEC *>(&first_axis));
                    f32 change = (scaled_magnitude + movement) / scaled_magnitude - 1.0f;
                    scale_y = first_axis.y * change + second_axis.y * change + 1.0f;
                    scale_z = first_axis.z * change + second_axis.z * change + 1.0f;
                    scale_x = first_axis.x * change + second_axis.x * change + 1.0f;
                    break;
                }
                case 7: {
                    f32 movement = input.Get(1) - input.Get(0) + input.Get(2);
                    if (movement == 0.0f)
                        continue;
                    scale_x = scale_y = scale_z = movement * 0.005f + 1.0f;
                    break;
                }
                default:
                    continue;
            }
            NUMTX &matrix = transform.matrix;
            average = VuVec(matrix.m30, matrix.m31, matrix.m32, matrix.m33);
            const NUMTX original = matrix;
            const f32 zero = 0.0f;
            matrix.m02 = scale_x * original.m02 + zero * original.m12 + zero * original.m22;
            matrix.m00 = scale_x * original.m00 + zero * original.m10 + zero * original.m20;
            matrix.m10 = zero * original.m00 + scale_y * original.m10 + zero * original.m20;
            matrix.m01 = scale_x * original.m01 + zero * original.m11 + zero * original.m21;
            matrix.m11 = zero * original.m01 + scale_y * original.m11 + zero * original.m21;
            matrix.m12 = zero * original.m02 + scale_y * original.m12 + zero * original.m22;
            matrix.m20 = zero * original.m00 + zero * original.m10 + scale_z * original.m20;
            matrix.m21 = zero * original.m01 + zero * original.m11 + scale_z * original.m21;
            matrix.m22 = zero * original.m02 + zero * original.m12 + scale_z * original.m22;
            matrix.m03 = matrix.m13 = matrix.m23 = 0.0f;
            matrix.m30 = average.x;
            matrix.m31 = average.y;
            matrix.m32 = average.z;
            matrix.m33 = average.w;
            matrix_type = EdType_VuMtx;
            if (entry->reference == NULL ||
                entry->reference->SetAttributeData(entry->object, 0x20, matrix_type, &transform, 0) == 0) {
                if (entry->ed_class->FindMember(&member, entry->object, 0x20, 1) != 0)
                    member.reference->SetAttributeData(member.object, 0x20, matrix_type, &transform, 0);
            }
        }
    }
    return axis != 0;
}

__attribute__((force_align_arg_pointer)) void EdManScale::Render(ClassObjectList &selected) {
    if (selected.count > 2)
        EdManipulator::Render(selected);
    VuVec average;
    if (selected.GetAveragePosition(average) != 0) {
        VuMtx transform;
        ClassObjectListEntry *entry = selected.first;
        const i32 type = EdType_VuMtx;
        if (entry->reference == NULL ||
            entry->reference->GetAttributeData(entry->object, 0x20, type, &transform, 0) == 0) {
            EdMember member;
            if (entry->ed_class->FindMember(&member, entry->object, 0x20, 1) != 0)
                member.reference->GetAttributeData(member.object, 0x20, type, &transform, 0);
        }
        DrawAxis(average, &transform);
    }
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
    i32 loop_limit = class_count;
    for (i32 index = 0; index < loop_limit; ++index) {
        EdClass *object_class = &classes[index];
        i32 object_flags = object_class->flags;
        if (stream.flags & 0x400000) {
            if (object_flags & 0x400000) {
                continue;
            }
        } else if (object_flags & 0x10000000) {
            continue;
        }
        i32 stream_classes[64];
        i32 stream_class_count = 0;
        object_class->GetStreamClasses(stream, stream_classes, stream_class_count, 64);
        for (i32 stream_class = 0; stream_class < stream_class_count; ++stream_class) {
            used_classes[stream_classes[stream_class]] = 1;
        }
        loop_limit = class_count;
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
    types = reinterpret_cast<EdType *>(ALIGN(buffer.addr, 16));
    buffer.addr = reinterpret_cast<usize>(types) + sizeof(EdType) * max_types;
    memset(types, 0, sizeof(EdType) * max_types);
    classes = reinterpret_cast<EdClass *>(ALIGN(buffer.addr, 16));
    buffer.addr = reinterpret_cast<usize>(classes) + sizeof(EdClass) * class_capacity;
    memset(classes, 0, sizeof(EdClass) * class_capacity);
    mappings = reinterpret_cast<NameMapping *>(ALIGN(buffer.addr, 16));
    buffer.addr = reinterpret_cast<usize>(mappings) + sizeof(NameMapping) * mapping_capacity;
    memset(mappings, 0, sizeof(NameMapping) * mapping_capacity);
    notifiers = reinterpret_cast<EdObjectNotifier **>(ALIGN(buffer.addr, 16));
    buffer.addr = reinterpret_cast<usize>(notifiers) + sizeof(EdObjectNotifier *) * notifier_capacity;
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
        notifier->NotifyCreateObject(object, object_class, source, index, context, flags);
    }
}

void EdRegistry::NotifyDestroyObject(void *object, EdClass *object_class, i32 index, i32 flags) {
    for (i32 i = 0; i < notifier_count; ++i) {
        EdObjectNotifier *notifier = notifiers[i];
        notifier->NotifyDestroyObject(object, object_class, index, flags);
    }
}

void EdRegistry::NotifyDefunctObject(void *object, EdClass *object_class, i32 flags) {
    for (i32 i = 0; i < notifier_count; ++i) {
        EdObjectNotifier *notifier = notifiers[i];
        notifier->NotifyDefunctObject(object, object_class, flags);
    }
}

void EdRegistry::NotifyReviveObject(void *object, EdClass *object_class, i32 flags) {
    for (i32 i = 0; i < notifier_count; ++i) {
        EdObjectNotifier *notifier = notifiers[i];
        notifier->NotifyReviveObject(object, object_class, flags);
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
            i32 count = 0;
            i32 mapping[64];
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

    i32 filter_flags;
    i16 group;
    auto get_attribute = [&](EdClass *object_class, void *object, i32 attribute, i32 type, void *data, i32 size) {
        EdMember member;
        if (object_class->FindMember(&member, object, attribute, 1))
            member.reference->GetAttributeData(member.object, attribute, type, data, size);
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
            } else if (object_class->flags & 0x04000000) {
                stream.SerialiseString(object_name, sizeof(object_name));
                void *original = object_class->FindObject(object_name);
                if (original == NULL) {
                    object_class->SerialiseObject(stream, NULL, source_class, source_registry);
                    continue;
                }
                object = theRegistry.CreateObject(object_class->interface, original, 4, 0, 2);
            } else {
                object = theRegistry.CreateObject(object_class->interface, NULL, 4, 0, 2);
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
            if (object_class->interface == NULL)
                continue;
            if (stream.flags & 0x400000) {
                if (object_class->flags & 0x400000)
                    continue;
            } else if (object_class->flags & 0x10000000) {
                continue;
            }
            ++count;
        }
        stream.SerialiseBuffer(&count, sizeof(count), 1);
        for (i32 index = 0; index < class_count; ++index) {
            EdClass *object_class = &classes[index];
            if (object_class->interface == NULL)
                continue;
            if (stream.flags & 0x400000) {
                if (object_class->flags & 0x400000)
                    continue;
            } else if (object_class->flags & 0x10000000) {
                continue;
            }
            stream.BeginBlock("ObjectList");
            stream.SerialiseString(&object_class->name);
            i32 object_count = 0;
            for (void *object = object_class->interface->vtable->get_next_object(object_class->interface, NULL);
                 object != NULL;
                 object = object_class->interface->vtable->get_next_object(object_class->interface, object)) {
                group = 0;
                filter_flags = 0;
                get_attribute(object_class, object, 1, EdType_Int, &filter_flags, 0);
                get_attribute(object_class, object, 0x100, EdType_Short, &group, 0);
                if (stream.flags & 0x400000) {
                    if (filter_flags & 0x400000)
                        continue;
                } else if (filter_flags & 0x10000000) {
                    continue;
                }
                if (group != stream.unknown_10)
                    continue;
                ++object_count;
            }
            stream.SerialiseBuffer(&object_count, sizeof(object_count), 1);
            for (void *object = object_class->interface->vtable->get_next_object(object_class->interface, NULL);
                 object != NULL;
                 object = object_class->interface->vtable->get_next_object(object_class->interface, object)) {
                group = 0;
                filter_flags = 0;
                get_attribute(object_class, object, 1, EdType_Int, &filter_flags, 0);
                get_attribute(object_class, object, 0x100, EdType_Short, &group, 0);
                if (stream.flags & 0x400000) {
                    if (filter_flags & 0x400000)
                        continue;
                } else if (filter_flags & 0x10000000) {
                    continue;
                }
                if (group != stream.unknown_10)
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
        if (!(stream.version <= 3)) {
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
        } else {
            for (i32 index = 0; index < source_registry->class_count; ++index) {
                EdClass *source_class = source_registry->GetClass(index);
                EdClass *object_class = GetClass(MapName(source_class->name));
                if (object_class != NULL && object_class->interface != NULL) {
                    if (stream.BeginBlock("ObjectList") != NULL)
                        read_objects(object_class, source_class);
                    stream.EndBlock();
                }
            }
        }
    }
}

EdManRotate::EdManRotate() {
    selected_attribute = 16;
}

static i32 rotation_axis = 1;

i32 EdManRotate::Process(EdInputContext &input, ClassObjectList &selected) {
    EdManipulator::Process(input, selected);
    VuVec average;
    if (selected.GetAveragePosition(average) == 0)
        return 0;
    eduiSetCameraEnabled(1);
    if (input.pad != NULL && (input.pad->digital_buttons_pressed & 0x10) != 0) {
        if (++rotation_axis == 4)
            rotation_axis = 1;
    }
    if (input.GetHold(38) != 0.0f) {
        eduiSetCameraEnabled(0);
        i32 angle = static_cast<i32>(eduiGetAnalougePadValue(input.pad) * 400.0f);
        return RotateItem(input, selected, angle, rotation_axis);
    }
    if (input.GetHold(3) == 0.0f)
        return 0;
    VuVec axis;
    i32 selected_axis = SelectRotator(input, average, axis);
    theLevelEditor.field_0x2c = AxisColour[selected_axis];
    i32 angle = *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x68);
    return RotateItem(input, selected, angle, selected_axis);
}

void EdManRotate::Render(ClassObjectList &selected) {
    if (selected.count > 2)
        EdManipulator::Render(selected);
    VuVec average;
    if (selected.GetAveragePosition(average) != 0)
        DrawRotator(average);
}

// The original realigns the incoming stack for its matrix and vector locals.
__attribute__((force_align_arg_pointer)) i32 EdManRotate::RotateItem(EdInputContext &, ClassObjectList &objects,
                                                                     i32 angle, i32 axis) {
    ClassObjectListEntry *first = objects.first;
    VuVec average;
    objects.GetAveragePosition(average);
    if (angle == 0 || first == NULL)
        return axis;
    i32 cosine_index = ((angle + 0x4000) >> 1) & 0x7fff;
    i32 sine_index = (angle >> 1) & 0x7fff;
    for (ClassObjectListEntry *entry = first; entry != NULL; entry = entry->next) {
        NUMTX matrix;
        NuMtxSetIdentity(&matrix);
        i32 matrix_type = EdType_VuMtx;
        if (entry->reference == NULL ||
            entry->reference->GetAttributeData(entry->object, 0x10, matrix_type, &matrix, 0) == 0) {
            EdMember member;
            if (__builtin_expect(entry->ed_class->FindMember(&member, entry->object, 0x10, 1) == 0 ||
                member.reference->GetAttributeData(member.object, 0x10, matrix_type, &matrix, 0) == 0, 1))
                continue;
        }
        switch (axis) {
            case 1: {
                average = VuVec(matrix.m30, matrix.m31, matrix.m32, matrix.m33);
                f32 cosine = NuTrigTable[cosine_index];
                f32 sine = NuTrigTable[sine_index];
                f32 m01 = matrix.m01, m11 = matrix.m11, m21 = matrix.m21;
                matrix.m01 = cosine * m01 - sine * matrix.m02;
                matrix.m02 = m01 * sine + matrix.m02 * cosine;
                matrix.m11 = cosine * m11 - sine * matrix.m12;
                matrix.m12 = m11 * sine + matrix.m12 * cosine;
                matrix.m21 = cosine * m21 - sine * matrix.m22;
                matrix.m22 = m21 * sine + matrix.m22 * cosine;
                break;
            }
            case 2: {
                average = VuVec(matrix.m30, matrix.m31, matrix.m32, matrix.m33);
                f32 sine = NuTrigTable[sine_index];
                f32 cosine = NuTrigTable[cosine_index];
                f32 m00 = matrix.m00, m10 = matrix.m10, m20 = matrix.m20;
                matrix.m00 = cosine * m00 + sine * matrix.m02;
                matrix.m02 = matrix.m02 * cosine - m00 * sine;
                matrix.m10 = cosine * m10 + sine * matrix.m12;
                matrix.m12 = matrix.m12 * cosine - m10 * sine;
                matrix.m20 = cosine * m20 + sine * matrix.m22;
                matrix.m22 = matrix.m22 * cosine - m20 * sine;
                break;
            }
            case 3: {
                average = VuVec(matrix.m30, matrix.m31, matrix.m32, matrix.m33);
                f32 sine = NuTrigTable[sine_index];
                f32 cosine = NuTrigTable[cosine_index];
                f32 m00 = matrix.m00, m10 = matrix.m10, m20 = matrix.m20;
                matrix.m00 = cosine * m00 - sine * matrix.m01;
                matrix.m01 = m00 * sine + matrix.m01 * cosine;
                matrix.m10 = cosine * m10 - sine * matrix.m11;
                matrix.m11 = m10 * sine + matrix.m11 * cosine;
                matrix.m20 = cosine * m20 - sine * matrix.m21;
                matrix.m21 = m20 * sine + matrix.m21 * cosine;
                break;
            }
            default:
                continue;
        }
        matrix_type = EdType_VuMtx;
        if (entry->reference == NULL ||
            entry->reference->SetAttributeData(entry->object, 0x10, matrix_type, &matrix, 0) == 0) {
            EdMember member;
            if (entry->ed_class->FindMember(&member, entry->object, 0x10, 1) != 0)
                member.reference->SetAttributeData(member.object, 0x10, matrix_type, &matrix, 0);
        }
    }
    return axis;
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
    CheckType(type);
    switch (member_offset) {
        case static_cast<i32>(0x80000001):
            static_cast<SplineObject *>(object)->step = *static_cast<f32 *>(data);
            if (theSplineHelper.auto_generate_points)
                static_cast<SplineObject *>(object)->points.Clear();
            break;
        case static_cast<i32>(0x80000002):
            static_cast<SplineObject *>(object)->height = *static_cast<f32 *>(data);
            if (theSplineHelper.auto_generate_points)
                static_cast<SplineObject *>(object)->points.Clear();
            break;
        case static_cast<i32>(0x80000003):
            static_cast<SplineObject *>(object)->drop = *static_cast<i32 *>(data) != 0;
            if (theSplineHelper.auto_generate_points)
                static_cast<SplineObject *>(object)->points.Clear();
            break;
        case static_cast<i32>(0x80000004):
            static_cast<SplineObject *>(object)->closed = *static_cast<i32 *>(data);
            if (theSplineHelper.auto_generate_points)
                static_cast<SplineObject *>(object)->points.Clear();
            break;
        default:
            // The original setter delegates unrecognized members to the getter.
            EdRef::GetMemberData(object, type, data, data_size);
            break;
    }
}

static EdBitControl *edBitControl;
static edui_prop_s *edBitItem;
static i32 edBitIndex;

__attribute__((force_align_arg_pointer)) void EdBitControl::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    void *memory = theMemoryManager.AllocPool(sizeof(EdBitControl), 1);
    EdBitControl *control = new (memory) EdBitControl;
    control->reference = member;
    control->items = items;
    control->object = target;
    control->bit_mask = bit_mask;
    i32 value;
    member->GetMemberData(target, EdType_Int, &value, 0);
    control->item =
        eduiItemExpanderCreate(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected, member->name);
    eduiMenuAddItem(menu, control->item);
    for (i32 bit = 0; bit < 32; ++bit) {
        if ((control->bit_mask & (1u << bit)) == 0)
            continue;
        char bit_name[0x20];
        sprintf(bit_name, "%d", bit);
        char *text = GetEnumString((value >> bit) & 1);
        eduiitem_s *child = eduiItemPropCreateEx(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected,
                                                 cbChanged, cbButton, 1, bit_name, text, bit);
        eduiItemExpanderAddChild(reinterpret_cast<edui_expander_s *>(control->item), child);
    }
}

void EdBitControl::Refresh() {
}

void EdBitControl::cbButton(eduimenu_s *menu, eduiitem_s *item, u32) {
    edBitItem = reinterpret_cast<edui_prop_s *>(item);
    edBitControl = static_cast<EdBitControl *>(item->data_ptr);
    edBitIndex = edBitItem->extra_data;
    eduimenu_s *choices =
        eduiMenuCreate(item->x + menu->width, item->y, 180, 250,
                       reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (choices != NULL) {
        for (Item *entry = edBitControl->items; entry->name != NULL; ++entry) {
            eduiMenuAddItem(choices, eduiItemSelCreate(reinterpret_cast<usize>(entry), item->colours, 0, 0,
                                                       cbSelectItem, entry->name));
        }
        choices->flags |= 1;
        eduiMenuAttach(menu, choices);
        eduiMenuFitWidth(choices, 5);
        reinterpret_cast<u8 *>(item)[0x4c] &= ~8;
    }
}

__attribute__((force_align_arg_pointer)) void EdBitControl::cbChanged(eduimenu_s *, eduiitem_s *item, u32) {
    EdBitControl *control = static_cast<EdBitControl *>(item->data_ptr);
    for (Item *entry = control->items; entry->name != NULL; ++entry) {
        if (NuStrICmp(entry->name, reinterpret_cast<edui_prop_s *>(item)->property_text) == 0) {
            eduiItemPropSetText(reinterpret_cast<edui_prop_s *>(item), entry->name);
            return;
        }
    }
    i32 values[4];
    values[0] = NuAToI(reinterpret_cast<edui_prop_s *>(item)->property_text);
    Item *entry = control->items;
    char *text = entry->name;
    while (text != NULL) {
        if (entry->value == values[0])
            break;
        ++entry;
        text = entry->name;
    }
    char number[0x80];
    if (text == NULL) {
        control->reference->GetMemberData(control->object, EdType_Int, values, 0);
        sprintf(number, "%d", values[0]);
        text = number;
    }
    eduiItemPropSetText(reinterpret_cast<edui_prop_s *>(item), text);
}

__attribute__((force_align_arg_pointer)) void EdBitControl::cbSelectItem(eduimenu_s *menu, eduiitem_s *item,
                                                                         u32 flags) {
    if (edBitControl != NULL) {
        Item *choice = static_cast<Item *>(item->data_ptr);
        eduiItemPropSetText(edBitItem, edBitControl->GetEnumString(choice->value));
        u32 value[3] __attribute__((aligned(16)));
        edBitControl->reference->GetMemberData(edBitControl->object, EdType_Int, value, 0);
        u32 bits;
        if (choice->value != 0) {
            bits = (1u << edBitIndex) | value[0];
        } else {
            bits = ~(1u << edBitIndex) & value[0];
        }
        value[0] = bits;
        edBitControl->reference->SetMemberData(edBitControl->object, EdType_Int, value, 0, NULL);
        cbEdLevelDestroyOnSelect(menu, item, flags);
    }
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

void EdEnumControl::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    void *memory = theMemoryManager.AllocPool(sizeof(EdEnumControl), 1);
    EdEnumControl *control = new (memory) EdEnumControl;
    control->reference = member;
    control->object = target;
    control->items = items;
    i32 value;
    member->GetMemberData(target, EdType_Int, &value, 0);
    control->item = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected, cbChanged,
                                       cbButton, 1, member->name, GetEnumString(value));
    eduiMenuAddItem(menu, control->item);
}

EdEnumControl::Item EdEnumControl::OpenClosedItems[] = {{"Open", 1}, {"Closed", 0}, {NULL, 0}};
EdEnumControl::Item EdEnumControl::OnOffItems[] = {{"On", 1}, {"Off", 0}, {NULL, 0}};
EdEnumControl::Item EdEnumControl::YesNoItems[] = {{"Yes", 1}, {"No", 0}, {NULL, 0}};

char *EdEnumControl::GetEnumString(i32 value) {
    for (Item *entry = items; entry->name != NULL; ++entry) {
        if (entry->value == value)
            return entry->name;
    }
    return "Unknown";
}

i32 EdEnumControl::GetEnumValue(char *name) {
    for (Item *entry = items; entry->name != NULL; ++entry) {
        if (NuStrICmp(entry->name, name) != 0)
            return entry->value;
    }
    return 0;
}

void EdEnumControl::Refresh() {
    i32 value;
    reference->GetMemberData(object, EdType_Int, &value, 0);
    eduiItemPropSetText(reinterpret_cast<edui_prop_s *>(item), GetEnumString(value));
}

static EdEnumControl *active_enum_control;

void EdEnumControl::cbButton(eduimenu_s *menu, eduiitem_s *item, u32) {
    active_enum_control = static_cast<EdEnumControl *>(item->data_ptr);
    eduimenu_s *choices =
        eduiMenuCreate(item->x + menu->width, item->y, 180, 250,
                       reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (choices == NULL)
        return;
    for (Item *entry = active_enum_control->items; entry->name != NULL; ++entry)
        eduiMenuAddItem(
            choices, eduiItemSelCreate(reinterpret_cast<usize>(entry), item->colours, 0, 0, cbSelectItem, entry->name));
    choices->flags |= 1;
    eduiMenuAttach(menu, choices);
    eduiMenuFitWidth(choices, 5);
    eduiMenuFitOnScreen(choices, 30);
    static_cast<edui_prop_s *>(item)->unknown_property_flags &= ~8u;
}

void EdEnumControl::cbChanged(eduimenu_s *, eduiitem_s *item, u32) {
    EdEnumControl *control = static_cast<EdEnumControl *>(item->data_ptr);
    edui_prop_s *property = reinterpret_cast<edui_prop_s *>(item);
    for (Item *entry = control->items; entry->name != NULL; ++entry) {
        if (NuStrICmp(entry->name, property->property_text) == 0) {
            eduiItemPropSetText(property, entry->name);
            return;
        }
    }
    i32 value = NuAToI(property->property_text);
    for (Item *entry = control->items; entry->name != NULL; ++entry) {
        if (entry->value == value) {
            eduiItemPropSetText(property, entry->name);
            return;
        }
    }
    control->reference->GetMemberData(control->object, EdType_Int, &value, 0);
    char text[128];
    sprintf(text, "%d", value);
    eduiItemPropSetText(property, text);
}

void EdEnumControl::cbSelectItem(eduimenu_s *menu, eduiitem_s *item, u32 flags) {
    if (active_enum_control != NULL) {
        Item *choice = static_cast<Item *>(item->data_ptr);
        i32 value = choice->value;
        eduiItemPropSetText(static_cast<edui_prop_s *>(active_enum_control->item),
                            active_enum_control->GetEnumString(value));
        active_enum_control->reference->SetMemberData(active_enum_control->object, EdType_Int, &value, 0, NULL);
        cbEdLevelDestroyOnSelect(menu, item, flags);
    }
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

i32 EdManipulator::AxisColour[8] = {
    static_cast<i32>(0xffffffff), static_cast<i32>(0xff0000ff), static_cast<i32>(0xff00ff00),
    static_cast<i32>(0xffff0000), static_cast<i32>(0xff00ffff), static_cast<i32>(0xffff00ff),
    static_cast<i32>(0xffffff00), static_cast<i32>(0xffffffff),
};

void EdManipulator::DrawAxis(VuVec &origin, VuMtx *matrix) {
    // The original keeps the locator array in a 16-byte aligned slot of a realigned frame.
    VuVec points[8] __attribute__((aligned(16)));
    GetAxisLocators(origin, points, matrix);
    EdDrawBegin(1);
    EdDrawLineSphere(points[1], Scale * 0.25f, 1.0f,
                     *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0 ? AxisColour[1]
                                                                                     : static_cast<i32>(0xff808080));
    EdDrawLineSphere(points[2], Scale * 0.25f, 1.0f,
                     *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0 ? AxisColour[2]
                                                                                     : static_cast<i32>(0xff808080));
    EdDrawLineSphere(points[3], Scale * 0.25f, 1.0f,
                     *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0 ? AxisColour[3]
                                                                                     : static_cast<i32>(0xff808080));
    VuMtx box;
    NuMtxSetIdentity(&box.matrix);
    box.matrix.m30 = points[4].x;
    box.matrix.m31 = points[4].y;
    box.matrix.m32 = points[4].z;
    box.matrix.m33 = 1.0f;
    EdDrawLineCube(box, Scale * 0.1f,
                   *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0 ? AxisColour[4]
                                                                                   : static_cast<i32>(0xff808080));
    box.matrix.m30 = points[5].x;
    box.matrix.m31 = points[5].y;
    box.matrix.m32 = points[5].z;
    box.matrix.m33 = 1.0f;
    EdDrawLineCube(box, Scale * 0.1f,
                   *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0 ? AxisColour[5]
                                                                                   : static_cast<i32>(0xff808080));
    box.matrix.m30 = points[6].x;
    box.matrix.m31 = points[6].y;
    box.matrix.m32 = points[6].z;
    box.matrix.m33 = 1.0f;
    EdDrawLineCube(box, Scale * 0.1f,
                   *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0 ? AxisColour[6]
                                                                                   : static_cast<i32>(0xff808080));
    box.matrix.m30 = points[7].x;
    box.matrix.m31 = points[7].y;
    box.matrix.m32 = points[7].z;
    box.matrix.m33 = 1.0f;
    EdDrawLineCube(box, Scale * 0.1f,
                   *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0 ? AxisColour[7]
                                                                                   : static_cast<i32>(0xff808080));
    EdDrawEnd();
    EdDrawBegin(0);
    const float arrow_size = Scale * 0.25f;
    float arrow_radius;
    VuVec end;
    VuVec start;
    {
        const VuVec &point = points[1];
        const VuVec &center = points[7];
        const VuVec offset((point.x - center.x) * arrow_size * 0.5f, (point.y - center.y) * arrow_size * 0.5f,
                           (point.z - center.z) * arrow_size * 0.5f, 0.0f);
        start = VuVec(point.x - offset.x, point.y - offset.y, point.z - offset.z, 0.0f);
        end = VuVec(offset.x + point.x, offset.y + point.y, offset.z + point.z, 0.0f);
        arrow_radius = arrow_size * 0.2f;
        const i32 colour = *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0
                               ? AxisColour[1]
                               : static_cast<i32>(0xff808080);
        EdDrawPolyArrow(start, end, 8, colour, arrow_radius, arrow_radius, 0.5f, 0.0f);
    }
    {
        const VuVec &point = points[2];
        const VuVec &center = points[7];
        const VuVec offset((point.x - center.x) * arrow_size * 0.5f, (point.y - center.y) * arrow_size * 0.5f,
                           (point.z - center.z) * arrow_size * 0.5f, 0.0f);
        start = VuVec(point.x - offset.x, point.y - offset.y, point.z - offset.z, 0.0f);
        end = VuVec(offset.x + point.x, offset.y + point.y, offset.z + point.z, 0.0f);
        const i32 colour = *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0
                               ? AxisColour[2]
                               : static_cast<i32>(0xff808080);
        EdDrawPolyArrow(start, end, 8, colour, arrow_radius, arrow_radius, 0.2f, 0.0f);
    }
    {
        const VuVec &point = points[3];
        const VuVec &center = points[7];
        const VuVec offset((point.x - center.x) * arrow_size * 0.5f, (point.y - center.y) * arrow_size * 0.5f,
                           (point.z - center.z) * arrow_size * 0.5f, 0.0f);
        start = VuVec(point.x - offset.x, point.y - offset.y, point.z - offset.z, 0.0f);
        end = VuVec(offset.x + point.x, offset.y + point.y, offset.z + point.z, 0.0f);
        const i32 colour = *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0
                               ? AxisColour[3]
                               : static_cast<i32>(0xff808080);
        EdDrawPolyArrow(start, end, 8, colour, arrow_radius, arrow_radius, 0.2f, 0.0f);
    }
    EdDrawEnd();
}

void EdManipulator::DrawRotator(VuVec &origin) {
    f32 scale = Scale;
    EdDrawBegin(1);
    EdDrawLineCircleX(origin, scale,
                      *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0 ? AxisColour[1]
                                                                                      : static_cast<i32>(0xff808080),
                      32);
    EdDrawLineCircleY(origin, scale,
                      *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0 ? AxisColour[2]
                                                                                      : static_cast<i32>(0xff808080),
                      32);
    EdDrawLineCircleZ(origin, scale,
                      *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8) != 0 ? AxisColour[3]
                                                                                      : static_cast<i32>(0xff808080),
                      32);
    EdDrawEnd();
    const u8 *data = reinterpret_cast<const u8 *>(this);
    const i32 *selected_axis = reinterpret_cast<const i32 *>(data + 0x0c);
    const i32 *start_angle = reinterpret_cast<const i32 *>(data + 0x60);
    const i32 *end_angle = reinterpret_cast<const i32 *>(data + 0x64);
    if (*start_angle != *end_angle && *selected_axis != 0) {
        EdDrawBegin(1);
        EdDrawPolySector(origin, scale, *selected_axis - 1, *start_angle, *end_angle, static_cast<i32>(0x80808080), 32);
        EdDrawEnd();
    }
}

void EdManipulator::GetAxisLocators(VuVec &origin, VuVec *points, VuMtx *matrix) {
    const f32 scale = Scale + 0.0f;
    const f32 half = (scale + 0.0f) * 0.5f;
    points[3].x = 0.0f;
    points[3].y = 0.0f;
    points[3].w = 1.0f;
    points[2].x = 0.0f;
    points[2].z = 0.0f;
    points[2].w = 1.0f;
    points[1].y = 0.0f;
    points[1].z = 0.0f;
    points[1].w = 1.0f;
    points[7].x = 0.0f;
    points[7].y = 0.0f;
    points[7].z = 0.0f;
    points[7].w = 1.0f;
    points[0].x = 0.0f;
    points[0].y = 0.0f;
    points[0].z = 0.0f;
    points[0].w = 1.0f;
    points[1].x = scale;
    points[2].y = scale;
    points[3].z = scale;
    points[4].x = half;
    points[4].y = half;
    points[4].z = 0.0f;
    points[4].w = 0.0f;
    points[5].x = half;
    points[5].y = 0.0f;
    points[5].z = half;
    points[5].w = 0.0f;
    points[6].x = 0.0f;
    points[6].y = half;
    points[6].z = half;
    points[6].w = 0.0f;
    if (matrix != NULL) {
        for (i32 index = 0; index < 8; ++index) {
            VuVec &point = points[index];
            // The original reads the matrix rows as vectors, so they are reloaded after each locator store.
            const VuVec *rows = reinterpret_cast<const VuVec *>(&matrix->matrix);
            const f32 x = point.x;
            const f32 y = point.y;
            const f32 z = point.z;
            const f32 transformed_x = x * rows[0].x + y * rows[1].x + z * rows[2].x;
            const f32 transformed_y = x * rows[0].y + y * rows[1].y + z * rows[2].y;
            const f32 transformed_z = x * rows[0].z + y * rows[1].z + z * rows[2].z;
            point.x = transformed_x;
            point.y = transformed_y;
            point.z = transformed_z;
        }
    }
    for (i32 index = 0; index < 8; ++index) {
        points[index].x += origin.x;
        points[index].y += origin.y;
        points[index].z += origin.z;
    }
}

i32 EdManipulator::Process(EdInputContext &input, ClassObjectList &selected) {
    const f32 step = input.GetHold(21) != 0.0f ? 3.0f : 1.0f;
    if (input.GetHold(14) != 0.0f)
        Scale += step * input.delta_time;
    if (input.GetHold(15) != 0.0f) {
        if (Scale <= 0.1f)
            Scale = 0.1f;
        else
            Scale -= step * input.delta_time;
    }

    // The original keeps this vector in a 16-byte aligned slot of a realigned frame.
    VuVec average __attribute__((aligned(16)));
    f32 distance = 0.0f;
    VuVec *ray_origin = reinterpret_cast<VuVec *>(input.reserved_00 + 0x20);
    if (selected.GetAveragePosition(average) != 0) {
        VuVec displacement = {average.x - ray_origin->x, average.y - ray_origin->y, average.z - ray_origin->z, 0.0f};
        distance = NuVecMag(reinterpret_cast<NUVEC *>(&displacement));
    }
    VuVec direction = *reinterpret_cast<VuVec *>(input.reserved_00 + 0x30);
    NuVecNorm(reinterpret_cast<NUVEC *>(&direction), reinterpret_cast<NUVEC *>(&direction));
    VuVec *last_point = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x30);
    VuVec point = {ray_origin->x + direction.x * distance, ray_origin->y + direction.y * distance,
                   ray_origin->z + direction.z * distance, 0.0f};
    VuVec *delta = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x40);
    VuVec *cursor = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x50);
    if (input.GetPress(3) != 0.0f) {
        delta->x = 0.0f;
        delta->y = 0.0f;
        delta->z = 0.0f;
        delta->w = 1.0f;
        last_point->x = point.x;
        last_point->y = point.y;
        last_point->z = point.z;
        last_point->w = 0.0f;
    } else {
        delta->w = 0.0f;
        last_point->w = 0.0f;
        delta->x = point.x - last_point->x;
        delta->y = point.y - last_point->y;
        delta->z = point.z - last_point->z;
        last_point->x = point.x;
        last_point->y = point.y;
        last_point->z = point.z;
    }
    VuVec *ray_cursor = reinterpret_cast<VuVec *>(input.reserved_00);
    cursor->x = ray_cursor->x;
    cursor->y = ray_cursor->y;
    cursor->z = ray_cursor->z;
    cursor->w = ray_cursor->w;
    if (EdTerrRay(*cursor, *reinterpret_cast<VuVec *>(input.reserved_00 + 0x30)) == 0) {
        cursor->x = last_point->x;
        cursor->y = last_point->y;
        cursor->z = last_point->z;
        cursor->w = last_point->w;
    }
    last_point->w = 1.0f;
    delta->w = 1.0f;
    cursor->w = 1.0f;
    *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x08) = 0;
    for (ClassObjectListEntry *entry = selected.first; entry != NULL; entry = entry->next) {
        if (entry->ed_class->FindTypeRef(selected_attribute, 1) != NULL) {
            *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x08) = 1;
            break;
        }
    }
    return 0;
}

void EdManipulator::Render(ClassObjectList &selected) {
    for (ClassObjectListEntry *entry = selected.first; entry != NULL; entry = entry->next) {
        ClassObject &object = *reinterpret_cast<ClassObject *>(&entry->ed_class);
        theClassEditor.DrawObjectSphere(object, static_cast<i32>(0xff800000));
    }
}

i32 EdManipulator::SelectRotator(EdInputContext &input, VuVec &center, VuVec &plane) {
    // The original keeps the sphere hits and axis planes in one block whose address escapes.
    struct {
        VuVec far_point;
        VuVec near_point;
        VuVec normals[3];
    } hits;
    VuVec &near_point = hits.near_point;
    f32 scale = Scale;
    i32 *selected_axis = reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x0c);
    i32 *start_angle = reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x60);
    i32 *last_angle = reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x64);
    i32 *angle_delta = reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x68);
    VuVec *selected_plane = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x10);
    VuVec *secondary_plane = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x20);
    f32 pressed = input.GetPress(3);
    if (pressed != 0.0f || input.GetHold(3) == 0.0f) {
        VuVec &far_point = hits.far_point;
        VuVec *normals = hits.normals;
        normals[0] = VuVec(1.0f, 0.0f, 0.0f, -center.x);
        normals[1] = VuVec(0.0f, 1.0f, 0.0f, -center.y);
        normals[2] = VuVec(0.0f, 0.0f, 1.0f, -center.z);
        i32 axis = 0;
        VuVec chosen;
        f32 nearest_distance = __FLT_MAX__;
        i32 angle = 0;
        if (LineToSphereIntersection(*reinterpret_cast<VuVec *>(input.reserved_00 + 0x20),
                                     *reinterpret_cast<VuVec *>(input.reserved_00 + 0x30), center, scale + 0.01f,
                                     &far_point, &near_point) != 0) {
            SpherePos1.x = far_point.x;
            SpherePos1.y = far_point.y;
            SpherePos1.z = far_point.z;
            SpherePos1.w = far_point.w;
            SpherePos2.x = near_point.x;
            SpherePos2.y = near_point.y;
            SpherePos2.z = near_point.z;
            SpherePos2.w = near_point.w;
            VuVec screen_point;
            VuVec projection;
            VuVec difference;
#define CHECK_ROTATOR(point, normal, candidate)                                                                        \
    {                                                                                                                  \
        f32 distance_to_plane = point.x * normal.x + point.y * normal.y + point.z * normal.z + normal.w;               \
        projection = VuVec(point.x - normal.x * distance_to_plane, point.y - normal.y * distance_to_plane,             \
                           point.z - normal.z * distance_to_plane, 0.0f);                                              \
        NuCameraTransformScreenClip(&projection.xyz, &projection.xyz, 1, NULL);                                        \
        NuCameraTransformScreenClip(&screen_point.xyz, &point.xyz, 1, NULL);                                           \
        difference.x = screen_point.x - projection.x;                                                                  \
        difference.y = screen_point.y - projection.y;                                                                  \
        difference.z = screen_point.z - projection.z;                                                                  \
        f32 distance = NuVecMag(&difference.xyz);                                                                      \
        if (distance < 0.05f && distance < nearest_distance) {                                                         \
            nearest_distance = distance;                                                                               \
            chosen = point;                                                                                            \
            axis = candidate;                                                                                          \
        }                                                                                                              \
    }
            for (i32 candidate = 0; candidate < 3; ++candidate) {
                CHECK_ROTATOR(far_point, normals[candidate], candidate + 1);
                CHECK_ROTATOR(near_point, normals[candidate], candidate + 1);
            }
            if (axis != 0) {
                f32 x = chosen.x - center.x;
                f32 y = chosen.y - center.y;
                f32 z = chosen.z - center.z;
                switch (axis) {
                    case 1:
                        plane = VuVec(1.0f, 0.0f, 0.0f, 1.0f);
                        plane.w = -center.x;
                        angle = NuAtan2DA(y, z);
                        break;
                    case 2:
                        plane = VuVec(0.0f, 1.0f, 0.0f, 1.0f);
                        plane.w = -center.y;
                        angle = NuAtan2DA(x, -z);
                        break;
                    case 3:
                        plane = VuVec(0.0f, 0.0f, 1.0f, 1.0f);
                        plane.w = -center.z;
                        angle = NuAtan2DA(x, y);
                        break;
                }
            }
        }
        if (input.GetPress(3) != 0.0f) {
            *selected_axis = axis;
            selected_plane->x = plane.x;
            selected_plane->y = plane.y;
            selected_plane->z = plane.z;
            selected_plane->w = plane.w;
            *start_angle = angle;
            *last_angle = angle;
            *angle_delta = 0;
            return axis;
        }
        *selected_axis = 0;
        *start_angle = 0;
        *last_angle = 0;
        *angle_delta = 0;
        *selected_plane = VuVec_Zero;
        *secondary_plane = VuVec_Zero;
        return axis;
    }
    i32 axis = *selected_axis;
    plane.x = selected_plane->x;
    plane.y = selected_plane->y;
    plane.z = selected_plane->z;
    plane.w = selected_plane->w;
    VuVec intersection;
    if (LineToPlaneIntersecion(*reinterpret_cast<VuVec *>(input.reserved_00 + 0x20),
                               *reinterpret_cast<VuVec *>(input.reserved_00 + 0x30), plane, &intersection) == 0) {
        *angle_delta = 0;
        return axis;
    }
    f32 x = intersection.x - center.x;
    f32 y = intersection.y - center.y;
    f32 z = intersection.z - center.z;
    i32 angle = 0;
    switch (axis) {
        case 1:
            angle = NuAtan2DA(y, z);
            break;
        case 2:
            angle = NuAtan2DA(x, -z);
            break;
        case 3:
            angle = NuAtan2DA(x, y);
            break;
    }
    i32 delta = *last_angle - angle;
    *last_angle = angle;
    delta &= 0xffff;
    if (delta >= 0x8000)
        delta -= 0x10000;
    *angle_delta = delta;
    return axis;
}

// The original realigns the stack for its local locator vectors.
__attribute__((force_align_arg_pointer)) i32 EdManipulator::SelectAxis(EdInputContext &input, VuVec &origin,
                                                                       VuVec &first_axis, VuVec &second_axis,
                                                                       VuMtx *matrix) {
    VuVec locators[8];
    first_axis.y = 0.0f;
    first_axis.z = 0.0f;
    first_axis.w = 1.0f;
    first_axis.x = 0.0f;
    second_axis.x = 0.0f;
    second_axis.y = 0.0f;
    second_axis.w = 1.0f;
    second_axis.z = 0.0f;
    GetAxisLocators(origin, locators, matrix);

    VuVec *ray_direction = reinterpret_cast<VuVec *>(input.reserved_00 + 0x30);
    VuVec *ray_origin = reinterpret_cast<VuVec *>(input.reserved_00 + 0x20);
    i32 nearest = 0;
    f32 nearest_distance = __FLT_MAX__;
#define CHECK_AXIS(index, radius)                                                                                      \
    {                                                                                                                  \
        f32 distance = LineToPointDistance(*ray_origin, *ray_direction, locators[index], NULL);                        \
        if (distance < (radius) * Scale && distance < nearest_distance) {                                              \
            nearest_distance = distance;                                                                               \
            nearest = index;                                                                                           \
        }                                                                                                              \
    }
    CHECK_AXIS(1, 0.25f);
    CHECK_AXIS(2, 0.25f);
    CHECK_AXIS(3, 0.25f);
    CHECK_AXIS(4, 0.1f);
    CHECK_AXIS(5, 0.1f);
    CHECK_AXIS(6, 0.1f);
    CHECK_AXIS(7, 0.1f);

    i32 *selected_axis = reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x0c);

    switch (nearest) {
        case 1:
            first_axis = VuVec(1.0f, 0.0f, 0.0f, 1.0f);
            break;
        case 2:
            first_axis = VuVec(0.0f, 1.0f, 0.0f, 1.0f);
            break;
        case 3:
            first_axis = VuVec(0.0f, 0.0f, 1.0f, 1.0f);
            break;
        case 4:
            first_axis = VuVec(1.0f, 0.0f, 0.0f, 1.0f);
            second_axis = VuVec(0.0f, 1.0f, 0.0f, 1.0f);
            break;
        case 5:
            first_axis = VuVec(1.0f, 0.0f, 0.0f, 1.0f);
            second_axis = VuVec(0.0f, 0.0f, 1.0f, 1.0f);
            break;
        case 6:
            first_axis = VuVec(0.0f, 1.0f, 0.0f, 1.0f);
            second_axis = VuVec(0.0f, 0.0f, 1.0f, 1.0f);
            break;
        case 7:
            first_axis = VuVec(0.0f, 1.0f, 0.0f, 1.0f);
            second_axis = VuVec(0.0f, 0.0f, 1.0f, 1.0f);
            break;
        default:
            break;
    }
    if (matrix != NULL) {
        const NUMTX &transform = matrix->matrix;
        f32 first_x = first_axis.x;
        first_axis.z = first_x * transform.m02 + first_axis.y * transform.m12 + first_axis.z * transform.m22;
        first_axis.y = first_x * transform.m01 + first_axis.y * transform.m11 + first_axis.z * transform.m21;
        first_axis.x = first_x * transform.m00 + first_axis.y * transform.m10 + first_axis.z * transform.m20;

        f32 second_x = second_axis.x;
        f32 second_z = second_axis.z;
        second_axis.y = second_x * transform.m01 + second_axis.y * transform.m11 + second_z * transform.m21;
        second_axis.z = second_x * transform.m02 + second_axis.y * transform.m12 + second_z * transform.m22;
        second_axis.x = second_x * transform.m00 + second_axis.y * transform.m10 + second_z * transform.m20;

        NuVecNorm(reinterpret_cast<NUVEC *>(&first_axis), reinterpret_cast<NUVEC *>(&first_axis));
        NuVecNorm(reinterpret_cast<NUVEC *>(&second_axis), reinterpret_cast<NUVEC *>(&second_axis));
    }
    if (input.GetPress(3) != 0.0f) {
        *selected_axis = nearest;
        VuVec *selected_first = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x10);
        VuVec *selected_second = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x20);
        selected_first->x = first_axis.x;
        selected_first->y = first_axis.y;
        selected_first->z = first_axis.z;
        selected_first->w = first_axis.w;
        selected_second->x = second_axis.x;
        selected_second->y = second_axis.y;
        selected_second->z = second_axis.z;
        selected_second->w = second_axis.w;
    } else if (input.GetHold(3) != 0.0f) {
        nearest = *selected_axis;
        const VuVec *selected_first = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x10);
        const VuVec *selected_second = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x20);
        first_axis.z = selected_first->z;
        first_axis.y = selected_first->y;
        first_axis.x = selected_first->x;
        first_axis.w = selected_first->w;
        second_axis.x = selected_second->x;
        second_axis.y = selected_second->y;
        second_axis.z = selected_second->z;
        second_axis.w = selected_second->w;
    } else {
        *selected_axis = 0;
        *reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x10) = VuVec(0.0f, 0.0f, 0.0f, 1.0f);
        *reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x20) = VuVec(0.0f, 0.0f, 0.0f, 1.0f);
    }
    return nearest;
}

void EdInputContext::Clear(i32 input) {
    if (static_cast<u32>(input) < 40) {
        values[input] = 0.0f;
        cleared[input] = 1;
    }
}

EdInputContext::EdInputContext() {
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
        if (next_repeat >= repeat_threshold) {
            repeated[input] = 1;
            next_repeat = now;
        } else if (next_repeat == 0.0f) {
            repeated[input] = 1;
        }
        repeat_times[input] = next_repeat + repeat_delay;
        return;
    }

    values[input] = value;
    released[input] = held[input] != 0;
    repeat_times[input] = 0.0f;
    held[input] = 0;
}

void EdInputContext::Update(nucamera_s *camera, nupad_s *new_pad, float elapsed, bool) {
    static __used__ volatile u8 UseMouse;
    pad = new_pad;

    f32 *view = reinterpret_cast<f32 *>(reserved_00);
    view[0] = camera->mtx.m30;
    view[1] = camera->mtx.m31;
    view[2] = camera->mtx.m32;
    view[4] = camera->mtx.m10;
    view[5] = camera->mtx.m11;
    view[6] = camera->mtx.m12;
    view[4] *= 1000.0f;
    view[5] *= 1000.0f;
    view[6] *= 1000.0f;

    f32 cursor_x = 0.5f;
    f32 cursor_y = 0.5f;
    if (eduiUsedAlgPad(new_pad) > 0) {
        eduiSetCursorCoords(cursor_x, cursor_y);
        UseMouse = 0;
    } else {
        eduiGetCursorCoords(&cursor_x, &cursor_y);
    }
    NUVEC ray_end;
    NuCameraCalcRay(cursor_x, cursor_y, reinterpret_cast<NUVEC *>(reserved_00 + 0x20), &ray_end, camera);
    view[12] = ray_end.x - view[8];
    view[13] = ray_end.y - view[9];
    view[14] = ray_end.z - view[10];
    view[15] = 0.0f;
    delta_time = elapsed;

    // The original editor suppresses its input context while a property text
    // field is being edited, releasing held actions before returning.
    if (eduiPropTextPos >= 0) {
        for (i32 input_index = 0; input_index < 40; ++input_index)
            Set(input_index, 0.0f, elapsed);
        memset(cleared, 0, sizeof(cleared));
        return;
    }

    const i32 shift_or_s = NuKeyboard(0x2a) | NuKeyboard(0x36) | NuKeyboard(0x1f);
    const i32 alt_or_space = NuKeyboard(0x38) | NuKeyboard(0xb8) | NuKeyboard(0x39);
    const i32 control_or_c = NuKeyboard(0x1d) | NuKeyboard(0x9d) | NuKeyboard(0x2e);
    const i32 left_click = (NuMouseReadButtons() == 1 || (pad->digital_buttons & 0x800) != 0) && !alt_or_space;
    const f32 mouse_x = NuMouseReadXVel();
    const f32 mouse_y = NuMouseReadYVel();
    const f32 mouse_z = NuMouseReadZVel();
    u32 square = 0;
    i32 right_click;
    if (edGetPadDisabled() == 0) {
        if (eduiGetActiveMenu() == NULL) {
            const u32 buttons = pad->digital_buttons;
            square = buttons & 0x80;
            Set(38, static_cast<f32>(static_cast<i32>(buttons & 0x40)), elapsed);
        }
        right_click = NuMouseReadButtons() == 2 || (pad->digital_buttons & 0x20) != 0;
    } else {
        right_click = NuMouseReadButtons() == 2;
    }
    Set(0, mouse_x, elapsed);
    Set(1, mouse_y, elapsed);
    Set(2, mouse_z, elapsed);
    Set(3, static_cast<i32>(left_click && !control_or_c), elapsed);
    Set(4, static_cast<i32>(right_click && !alt_or_space && !control_or_c), elapsed);
    Set(5, static_cast<f32>(NuKeyboard(0x10)), elapsed);
    Set(6, static_cast<f32>(NuKeyboard(0x11)), elapsed);
    Set(7, static_cast<f32>(NuKeyboard(0x12)), elapsed);
    Set(8, static_cast<f32>(NuKeyboard(0x13)), elapsed);
    Set(9, static_cast<f32>(NuKeyboard(0x21)), elapsed);
    Set(10, static_cast<f32>(NuKeyboard(0x22)), elapsed);
    Set(11, static_cast<f32>(NuKeyboard(0xd2)), elapsed);
    Set(12, static_cast<f32>(NuKeyboard(0xd3)), elapsed);
    Set(13, static_cast<f32>(alt_or_space), elapsed);
    Set(14, static_cast<f32>(NuKeyboard(0x0d)), elapsed);
    Set(15, static_cast<f32>(NuKeyboard(0x0c)), elapsed);
    Set(16, static_cast<f32>(shift_or_s), elapsed);
    Set(17, static_cast<f32>(NuKeyboard(0x1b)), elapsed);
    Set(18, static_cast<f32>(NuKeyboard(0x1a)), elapsed);
    Set(19, static_cast<f32>(NuKeyboard(0xcd)), elapsed);
    Set(20, static_cast<f32>(NuKeyboard(0xcb)), elapsed);
    Set(21, static_cast<f32>(shift_or_s), elapsed);
    Set(22, static_cast<f32>(control_or_c), elapsed);
    Set(23, static_cast<i32>(NuKeyboard(0x1f) && control_or_c), elapsed);
    Set(24, static_cast<f32>(NuKeyboard(0x01)), elapsed);
    Set(25, static_cast<i32>(left_click && control_or_c), elapsed);
    Set(26, static_cast<f32>(NuKeyboard(2)), elapsed);
    Set(27, static_cast<f32>(NuKeyboard(3)), elapsed);
    Set(28, static_cast<f32>(NuKeyboard(4)), elapsed);
    Set(29, static_cast<f32>(NuKeyboard(5)), elapsed);
    Set(30, static_cast<f32>(NuKeyboard(6)), elapsed);
    Set(31, static_cast<f32>(NuKeyboard(7)), elapsed);
    Set(32, static_cast<f32>(NuKeyboard(8)), elapsed);
    Set(33, static_cast<f32>(NuKeyboard(9)), elapsed);
    Set(34, static_cast<f32>(NuKeyboard(10)), elapsed);
    Set(35, static_cast<f32>(NuKeyboard(11)), elapsed);
    Set(37, static_cast<f32>(static_cast<i32>(square)), elapsed);
    Set(39, static_cast<f32>(static_cast<i32>(pad->digital_buttons & 0x800)), elapsed);
    memset(cleared, 0, sizeof(cleared));
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

static EdColourControl *edColourControl;

static inline void set_colour_preview(eduiitem_s *item, const NUVEC &colour) {
    u32 red_alpha = 0xff000000 | (static_cast<i32>(colour.x * 255.0f) & 0xff);
    u32 green_blue = ((static_cast<u32>(static_cast<i32>(colour.y * 255.0f)) << 8) & 0xffff) |
                     ((static_cast<i32>(colour.z * 255.0f) & 0xff) << 16);
    u32 value = red_alpha | green_blue;
    *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(item) + 0x50) = value;
}

void EdColourControl::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    EdColourControl *control = new (theMemoryManager.AllocPool(sizeof(EdColourControl), 1)) EdColourControl();
    if (control == NULL)
        return;
    control->reference = member;
    control->object = target;
    NUVEC colour;
    member->GetMemberData(target, EdType_Colour3, &colour, 0);
    char name[128];
    strcpy(name, member->name);
    char value[128];
    sprintf(value, "%.2f %.2f %.2f", colour.x, colour.y, colour.z);
    control->item = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected, cbChanged,
                                       cbButton, 3, name, value);
    set_colour_preview(control->item, colour);
    eduiMenuAddItem(menu, control->item);
}

EdColourControl::EdColourControl() {
}

void EdColourControl::Refresh() {
    NUVEC colour;
    eduiitem_s *preview_item = item;
    reference->GetMemberData(object, EdType_Colour3, &colour, 0);
    char value[128];
    sprintf(value, "%.2f %.2f %.2f", colour.x, colour.y, colour.z);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), value);
    set_colour_preview(preview_item, colour);
}

void EdColourControl::cbButton(eduimenu_s *menu, eduiitem_s *item, u32) {
    edColourControl = static_cast<EdColourControl *>(item->data_ptr);
    eduimenu_s *picker_menu =
        eduiMenuCreate(item->x + menu->width, item->y, 180, 250,
                       reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (picker_menu == NULL)
        return;
    NUVEC colour;
    edColourControl->reference->GetMemberData(edColourControl->object, EdType_Colour3, &colour, 0);
    eduiitem_s *picker = eduiItemColourPickCreate(0, item->colours, cbColourSelected, const_cast<char *>("Colour"));
    eduiItemColourPickSetRGB(static_cast<edui_colour_pick_s *>(picker), colour.x, colour.y, colour.z);
    eduiMenuAddItem(picker_menu, picker);
    eduiMenuAttach(menu, picker_menu);
    static_cast<edui_prop_s *>(item)->unknown_property_flags &= ~8;
}

void EdColourControl::cbChanged(eduimenu_s *, eduiitem_s *item, u32) {
    EdColourControl *control = static_cast<EdColourControl *>(item->data_ptr);
    NUVEC colour;
    control->reference->GetMemberData(control->object, EdType_Colour3, &colour, 0);
    sscanf(static_cast<edui_prop_s *>(item)->property_text, "%f %f %f", &colour.x, &colour.y, &colour.z);
    control->reference->SetMemberData(control->object, EdType_Colour3, &colour, 0, NULL);
    char value[128];
    sprintf(value, "%.2f %.2f %.2f", colour.x, colour.y, colour.z);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), value);
    set_colour_preview(item, colour);
}

void EdColourControl::cbColourSelected(eduimenu_s *menu, eduiitem_s *item, u32 flags) {
    edui_colour_pick_s *picker = static_cast<edui_colour_pick_s *>(item);
    NUVEC colour = {picker->red, picker->green, picker->blue};
    eduiitem_s *preview_item = edColourControl->item;
    edColourControl->reference->SetMemberData(edColourControl->object, EdType_Colour3, &colour, 0, NULL);
    char value[128];
    sprintf(value, "%.2f %.2f %.2f", colour.x, colour.y, colour.z);
    eduiItemPropSetText(static_cast<edui_prop_s *>(edColourControl->item), value);
    set_colour_preview(preview_item, colour);
    cbEdLevelDestroyOnSelect(menu, item, flags);
}

void EdMatrixControl::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    EdMatrixControl *control = new (theMemoryManager.AllocPool(sizeof(EdMatrixControl), 1)) EdMatrixControl();
    if (!control)
        return;
    control->reference = member;
    control->object = target;
    // Keep the matrix at the 16-byte aligned stack address used by the original.
    VuMtx matrix __attribute__((aligned(16)));
    member->GetMemberData(target, EdType_VuMtx, &matrix, 0);
    control->item = eduiItemExpanderCreate(reinterpret_cast<usize>(control), &EdLevelAttr, cbSelected, member->name);
    eduiMenuAddItem(menu, control->item);
    char value[128];
#define ADD_MATRIX_COMPONENT(index, label, number)                                                                     \
    sprintf(value, "%.2f", number);                                                                                    \
    control->components[index] = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, cbSelected,        \
                                                    cbChanged, cbButton, 2, const_cast<char *>(label), value);         \
    control->components[index]->unknown_10 = index % 3 + 1;                                                            \
    eduiItemExpanderAddChild(static_cast<edui_expander_s *>(control->item), control->components[index])
    // These enabled groups are the common path in the original block layout.
    if (__builtin_expect(member->attributes & 8, 1)) {
        ADD_MATRIX_COMPONENT(0, "pos x", matrix.matrix.m30);
        ADD_MATRIX_COMPONENT(1, "pos y", matrix.matrix.m31);
        ADD_MATRIX_COMPONENT(2, "pos z", matrix.matrix.m32);
    }
    if (__builtin_expect(member->attributes & 0x10, 1)) {
        NUANG x, y, z;
        NuMtxGetEulerXYZ(&matrix.matrix, &x, &y, &z);
        ADD_MATRIX_COMPONENT(3, "rot x", static_cast<f32>(x) * (360.0f / 65536.0f));
        ADD_MATRIX_COMPONENT(4, "rot y", static_cast<f32>(y) * (360.0f / 65536.0f));
        ADD_MATRIX_COMPONENT(5, "rot z", static_cast<f32>(z) * (360.0f / 65536.0f));
    }
    if (member->attributes & 0x20) {
        f32 x = NuVecMag(reinterpret_cast<NUVEC *>(&matrix.matrix.m00));
        f32 y = NuVecMag(reinterpret_cast<NUVEC *>(&matrix.matrix.m10));
        f32 z = NuVecMag(reinterpret_cast<NUVEC *>(&matrix.matrix.m20));
        ADD_MATRIX_COMPONENT(6, "scale x", x);
        ADD_MATRIX_COMPONENT(7, "scale y", y);
        ADD_MATRIX_COMPONENT(8, "scale z", z);
    }
}

void EdMatrixControl::Destroy() {
    if (components[0])
        components[0]->data_ptr = nullptr;
    if (components[1])
        components[1]->data_ptr = nullptr;
    if (components[2])
        components[2]->data_ptr = nullptr;
    if (components[3])
        components[3]->data_ptr = nullptr;
    if (components[4])
        components[4]->data_ptr = nullptr;
    if (components[5])
        components[5]->data_ptr = nullptr;
    if (components[6])
        components[6]->data_ptr = nullptr;
    if (components[7])
        components[7]->data_ptr = nullptr;
    if (components[8])
        components[8]->data_ptr = nullptr;
}

EdMatrixControl::EdMatrixControl() {
}

inline EdMatrixControl::~EdMatrixControl() {
    Destroy();
}

inline void EdMatrixControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdMatrixControl));
}

void EdMatrixControl::Refresh() {
    VuMtx matrix;
    reference->GetMemberData(object, EdType_VuMtx, &matrix, 0);
    char value[128];
#define REFRESH_MATRIX_COMPONENT(index, number)                                                                        \
    if (components[index]) {                                                                                           \
        sprintf(value, "%.2f", number);                                                                                \
        eduiItemPropSetText(static_cast<edui_prop_s *>(components[index]), value);                                     \
    }
    REFRESH_MATRIX_COMPONENT(0, matrix.matrix.m30);
    REFRESH_MATRIX_COMPONENT(1, matrix.matrix.m31);
    REFRESH_MATRIX_COMPONENT(2, matrix.matrix.m32);
    if (components[3] || components[4] || components[5]) {
        NUANG x, y, z;
        NuMtxGetEulerXYZ(&matrix.matrix, &x, &y, &z);
        REFRESH_MATRIX_COMPONENT(3, static_cast<f32>(x) * (360.0f / 65536.0f));
        REFRESH_MATRIX_COMPONENT(4, static_cast<f32>(y) * (360.0f / 65536.0f));
        REFRESH_MATRIX_COMPONENT(5, static_cast<f32>(z) * (360.0f / 65536.0f));
    }
    REFRESH_MATRIX_COMPONENT(6, NuVecMag(reinterpret_cast<NUVEC *>(&matrix.matrix.m00)));
    REFRESH_MATRIX_COMPONENT(7, NuVecMag(reinterpret_cast<NUVEC *>(&matrix.matrix.m10)));
    REFRESH_MATRIX_COMPONENT(8, NuVecMag(reinterpret_cast<NUVEC *>(&matrix.matrix.m20)));
}

void EdMatrixControl::SetMenuItemAttr(i32 mask, eduiitem_s *menu_item, eduiiattr_s *selected, eduiiattr_s *unselected) {
    if (menu_item == components[0] || menu_item == components[1] || menu_item == components[2]) {
        if (mask & 8)
            memcpy(menu_item->colours, unselected, sizeof(*selected));
        else
            memcpy(menu_item->colours, selected, sizeof(*selected));
    }
    if (menu_item == components[3] || menu_item == components[4] || menu_item == components[5]) {
        if (mask && (mask & 0x10))
            memcpy(menu_item->colours, unselected, sizeof(*selected));
        else
            memcpy(menu_item->colours, selected, sizeof(*selected));
    }
    if (menu_item == components[6] || menu_item == components[7] || menu_item == components[8]) {
        if (mask && (mask & 0x20))
            memcpy(menu_item->colours, unselected, sizeof(*selected));
        else
            memcpy(menu_item->colours, selected, sizeof(*selected));
    }
}

void EdMatrixControl::cbButton(eduimenu_s *menu, eduiitem_s *item, u32 value) {
    static_cast<edui_prop_s *>(item)->unknown_property_flags |= 0x20;
    nupad_s *pad = EdControl::Input->pad;
    f32 changed = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
    f32 dx;
    f32 dy;
    eduiGetCursorDelta(&dx, &dy);
    changed -= dy * 100.0f;
    dy = eduiGetAnalougePadValue(pad);
    changed -= dy;
    char text[128];
    sprintf(text, "%.2f", changed);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), text);
    cbChanged(menu, item, value);
}

void EdMatrixControl::cbChanged(eduimenu_s *, eduiitem_s *item, u32) {
    NUMTX rebuilt;
    EdMatrixControl *control = static_cast<EdMatrixControl *>(item->data_ptr);
    // The original stores the source matrix at a 16-byte aligned stack address.
    VuMtx source __attribute__((aligned(16)));
    control->reference->GetMemberData(control->object, EdType_VuMtx, &source, 0);
    f32 changed_value = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
    NUMTX &matrix = source.matrix;
    if (item == control->components[0])
        matrix.m30 = changed_value;
    if (item == control->components[1])
        matrix.m31 = changed_value;
    if (item == control->components[2])
        matrix.m32 = changed_value;
    if (control->components[3] || control->components[4] || control->components[5] || control->components[6] ||
        control->components[7] || control->components[8]) {

        NuMtxSetIdentity(&rebuilt);
        if (control->components[6] && control->components[7] && control->components[8]) {
            // The original builds the scale as an aligned four-component vector with w set to one.
            VuVec scale __attribute__((aligned(16)));
            scale.x = NuAToF(static_cast<edui_prop_s *>(control->components[6])->property_text);
            scale.y = NuAToF(static_cast<edui_prop_s *>(control->components[7])->property_text);
            scale.z = NuAToF(static_cast<edui_prop_s *>(control->components[8])->property_text);
            scale.w = 1.0f;
            NuMtxScale(&rebuilt, reinterpret_cast<NUVEC *>(&scale));
        }
        {
            NUANG x = static_cast<NUANG>(NuAToF(static_cast<edui_prop_s *>(control->components[3])->property_text) *
                                         (65536.0f / 360.0f));
            NUANG z = static_cast<NUANG>(NuAToF(static_cast<edui_prop_s *>(control->components[5])->property_text) *
                                         (65536.0f / 360.0f));
            NUANG y = static_cast<NUANG>(NuAToF(static_cast<edui_prop_s *>(control->components[4])->property_text) *
                                         (65536.0f / 360.0f));
            f32 sine = NU_SIN_LUT(x), cosine = NU_COS_LUT(x);
            f32 saved = rebuilt.m01;
            rebuilt.m01 = saved * cosine - rebuilt.m02 * sine;
            rebuilt.m02 = saved * sine + rebuilt.m02 * cosine;
            saved = rebuilt.m11;
            rebuilt.m11 = saved * cosine - rebuilt.m12 * sine;
            rebuilt.m12 = saved * sine + rebuilt.m12 * cosine;
            saved = rebuilt.m21;
            rebuilt.m21 = saved * cosine - rebuilt.m22 * sine;
            rebuilt.m22 = saved * sine + rebuilt.m22 * cosine;

            sine = NU_SIN_LUT(y);
            cosine = NU_COS_LUT(y);
            saved = rebuilt.m00;
            rebuilt.m00 = saved * cosine + rebuilt.m02 * sine;
            rebuilt.m02 = rebuilt.m02 * cosine - saved * sine;
            saved = rebuilt.m10;
            rebuilt.m10 = saved * cosine + rebuilt.m12 * sine;
            rebuilt.m12 = rebuilt.m12 * cosine - saved * sine;
            saved = rebuilt.m20;
            rebuilt.m20 = saved * cosine + rebuilt.m22 * sine;
            rebuilt.m22 = rebuilt.m22 * cosine - saved * sine;

            sine = NU_SIN_LUT(z);
            cosine = NU_COS_LUT(z);
            saved = rebuilt.m00;
            rebuilt.m00 = saved * cosine - rebuilt.m01 * sine;
            rebuilt.m01 = saved * sine + rebuilt.m01 * cosine;
            saved = rebuilt.m10;
            rebuilt.m10 = saved * cosine - rebuilt.m11 * sine;
            rebuilt.m11 = saved * sine + rebuilt.m11 * cosine;
            saved = rebuilt.m20;
            rebuilt.m20 = saved * cosine - rebuilt.m21 * sine;
            rebuilt.m21 = saved * sine + rebuilt.m21 * cosine;
        }
        matrix.m00 = rebuilt.m00;
        matrix.m01 = rebuilt.m01;
        matrix.m02 = rebuilt.m02;
        matrix.m03 = rebuilt.m03;
        matrix.m10 = rebuilt.m10;
        matrix.m11 = rebuilt.m11;
        matrix.m12 = rebuilt.m12;
        matrix.m13 = rebuilt.m13;
        matrix.m20 = rebuilt.m20;
        matrix.m21 = rebuilt.m21;
        matrix.m22 = rebuilt.m22;
        matrix.m23 = rebuilt.m23;
    }
    matrix.m33 = 1.0f;
    control->reference->SetMemberData(control->object, EdType_VuMtx, &source, 0, nullptr);
    // The original keeps the text buffer at a 16-byte aligned stack address.
    char text[128] __attribute__((aligned(16)));
    sprintf(text, "%.2f", changed_value);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), text);
}

void EdMatrixControl::cbSelected(eduimenu_s *menu, eduiitem_s *item, u32 value) {
    EdMatrixControl *control = static_cast<EdMatrixControl *>(item->data_ptr);
    control->SelectSubObject();
    if (item == control->item || item == control->components[0] || item == control->components[1] ||
        item == control->components[2]) {
        theClassEditor.SetMode(3);
    }
    if (item == control->components[3] || item == control->components[4] || item == control->components[5]) {
        theClassEditor.SetMode(4);
    }
    if (item == control->components[6] || item == control->components[7] || item == control->components[8]) {
        theClassEditor.SetMode(5);
    }
    thePropertyTool.SetMenuControl(menu, control);
}

void EdStringControl::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    EdStringControl *control = new (theMemoryManager.AllocPool(sizeof(EdStringControl), 1)) EdStringControl();
    if (!control)
        return;
    control->reference = member;
    control->object = target;
    char value[128];
    control->GetVal(value, sizeof(value));
    control->item = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected, cbChanged,
                                       cbPress, 1, member->name, value);
    eduiMenuAddItem(menu, control->item);
}

EdStringControl::EdStringControl() {
}

inline void EdStringControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdStringControl));
}

void EdStringControl::GetVal(char *value, i32 capacity) {
    if (reference->type_id == EdType_String)
        reference->GetMemberData(object, EdType_String, value, capacity);
}

void EdStringControl::Refresh() {
    char value[128];
    GetVal(value, sizeof(value));
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), value);
}

void EdStringControl::SetVal(char const *value) {
    if (reference->type_id != EdType_String)
        return;
    char copy[128];
    NuStrNCpy(copy, value, reference->size);
    reference->SetMemberData(object, EdType_String, copy, 0, nullptr);
}

void EdStringControl::cbChanged(eduimenu_s *, eduiitem_s *item, u32) {
    static_cast<EdStringControl *>(item->data_ptr)->SetVal(static_cast<edui_prop_s *>(item)->property_text);
}

void EdStringControl::cbPress(eduimenu_s *menu, eduiitem_s *item, u32) {
    eduiAddPropTextPickEnt(menu, item);
}

template <> f32 EdValueControl<f32>::MouseScale = 100.0f;

template <> SAGA_HOST_LINKABLE_DTOR EdValueControl<f32>::~EdValueControl() {
}

template <> inline void EdValueControl<f32>::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdValueControl<f32>));
}

SAGA_HOST_LINKABLE_DTOR
EdFloatControl::~EdFloatControl() {
}

inline void EdFloatControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdFloatControl));
}

template <> void EdValueControl<f32>::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    EdValueControl<f32> *control =
        new (theMemoryManager.AllocPool(sizeof(EdValueControl<f32>), 1)) EdValueControl<f32>();
    control->format = format;
    control->value_type = value_type;
    control->minimum = minimum;
    control->maximum = maximum;
    control->reference = member;
    control->object = target;
    f32 value;
    member->GetMemberData(target, value_type, &value, 0);
    char text[128];
    sprintf(text, format, value);
    control->item = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected, cbChanged,
                                       cbButton, 2, member->name, text);
    eduiMenuAddItem(menu, control->item);
}

template <> void EdValueControl<f32>::Refresh() {
    f32 value;
    reference->GetMemberData(object, value_type, &value, 0);
    char text[128];
    sprintf(text, format, value);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), text);
}

template <> void EdValueControl<f32>::cbChanged(eduimenu_s *, eduiitem_s *item, u32) {
    EdValueControl<f32> *control = static_cast<EdValueControl<f32> *>(item->data_ptr);
    f32 value = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
    if (value < control->minimum)
        value = control->minimum;
    if (value > control->maximum)
        value = control->maximum;
    control->reference->SetMemberData(control->object, control->value_type, &value, 0, NULL);
    char text[128];
    sprintf(text, control->format, value);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), text);
}

template <>
__attribute__((force_align_arg_pointer)) void EdValueControl<f32>::cbButton(eduimenu_s *, eduiitem_s *item, u32) {
    EdValueControl<f32> *control = static_cast<EdValueControl<f32> *>(item->data_ptr);
    nupad_s *pad = EdControl::Input->pad;
    f32 sensitivity = 0.01f;
    if (control->maximum - control->minimum < 5.0f)
        sensitivity = 0.001f;
    static_cast<edui_prop_s *>(item)->unknown_property_flags |= 0x20;
    f32 initial_value = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
    f32 dx = 0.0f;
    // The original keeps the change, shared with the cursor delta, at a 16-byte aligned stack address.
    f32 change __attribute__((aligned(16))) = 0.0f;
    if (pad && (pad->digital_buttons & EDUI_CURSOR_PRIMARY)) {
        f32 left_y = static_cast<i32>(pad->analog_left_y);
        if (left_y > 128.0f)
            change = 10.0f * sensitivity * (left_y - 128.0f);
        if (pad->analog_left_y < 128)
            change = -10.0f * sensitivity * (128.0f - left_y);
        f32 right_y = static_cast<i32>(pad->analog_right_y);
        if (right_y > 128.0f)
            change = 0.1f * sensitivity * (right_y - 128.0f);
        if (pad->analog_right_y < 128)
            change = -0.1f * sensitivity * (128.0f - right_y);
    } else {
        eduiGetCursorDelta(&dx, &change);
        change *= MouseScale;
    }
    initial_value -= change;
    char text[128];
    sprintf(text, control->format, initial_value);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), text);
    control = static_cast<EdValueControl<f32> *>(item->data_ptr);
    f32 value = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
    if (value < control->minimum)
        value = control->minimum;
    if (value > control->maximum)
        value = control->maximum;
    control->reference->SetMemberData(control->object, control->value_type, &value, 0, NULL);
    char updated_text[128];
    sprintf(updated_text, control->format, value);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), updated_text);
}

void EdVectorControl::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    EdVectorControl *control = new (theMemoryManager.AllocPool(sizeof(EdVectorControl), 1)) EdVectorControl();
    if (!control)
        return;
    control->reference = member;
    control->object = target;
    VuVec vector;
    member->GetMemberData(target, EdType_VuVec, &vector, 0);
    control->item = eduiItemExpanderCreate(reinterpret_cast<usize>(control), &EdLevelAttr, cbSelected, member->name);
    eduiMenuAddItem(menu, control->item);
    char value[128];
    sprintf(value, "%.2f", vector.x);
    control->components[0] = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, cbSelected, cbChanged,
                                                cbButton, 2, const_cast<char *>("tx"), value);
    control->components[0]->unknown_10 = 1;
    eduiItemExpanderAddChild(static_cast<edui_expander_s *>(control->item), control->components[0]);
    sprintf(value, "%.2f", vector.y);
    control->components[1] = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, cbSelected, cbChanged,
                                                cbButton, 2, const_cast<char *>("ty"), value);
    control->components[1]->unknown_10 = 2;
    eduiItemExpanderAddChild(static_cast<edui_expander_s *>(control->item), control->components[1]);
    sprintf(value, "%.2f", vector.z);
    control->components[2] = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, cbSelected, cbChanged,
                                                cbButton, 2, const_cast<char *>("tz"), value);
    control->components[2]->unknown_10 = 3;
    eduiItemExpanderAddChild(static_cast<edui_expander_s *>(control->item), control->components[2]);
}

void EdVectorControl::Destroy() {
    if (components[0])
        components[0]->data_ptr = nullptr;
    if (components[1])
        components[1]->data_ptr = nullptr;
    if (components[2])
        components[2]->data_ptr = nullptr;
}

EdVectorControl::EdVectorControl() {
}

inline EdVectorControl::~EdVectorControl() {
    Destroy();
}

inline void EdVectorControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdVectorControl));
}

void EdVectorControl::Refresh() {
    VuVec vector;
    reference->GetMemberData(object, EdType_VuVec, &vector, 0);
    char value[128];
    sprintf(value, "%.2f", vector.x);
    eduiItemPropSetText(static_cast<edui_prop_s *>(components[0]), value);
    sprintf(value, "%.2f", vector.y);
    eduiItemPropSetText(static_cast<edui_prop_s *>(components[1]), value);
    sprintf(value, "%.2f", vector.z);
    eduiItemPropSetText(static_cast<edui_prop_s *>(components[2]), value);
}

void EdVectorControl::cbButton(eduimenu_s *menu, eduiitem_s *item, u32 value) {
    static_cast<edui_prop_s *>(item)->unknown_property_flags |= 0x20;
    nupad_s *pad = EdControl::Input->pad;
    f32 changed = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
    f32 dx;
    f32 dy;
    eduiGetCursorDelta(&dx, &dy);
    changed -= dy * 100.0f;
    dy = eduiGetAnalougePadValue(pad);
    changed -= dy;
    char text[128];
    sprintf(text, "%.2f", changed);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), text);
    cbChanged(menu, item, value);
}

void EdVectorControl::cbChanged(eduimenu_s *, eduiitem_s *item, u32) {
    EdVectorControl *control = static_cast<EdVectorControl *>(item->data_ptr);
    VuVec vector;
    control->reference->GetMemberData(control->object, EdType_VuVec, &vector, 0);
    f32 value = 0.0f;
    if (item == control->components[0]) {
        value = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
        vector.x = value;
    }
    if (item == control->components[1]) {
        value = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
        vector.y = value;
    }
    if (item == control->components[2]) {
        value = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
        vector.z = value;
    }
    vector.w = 1.0f;
    control->reference->SetMemberData(control->object, EdType_VuVec, &vector, 0, nullptr);
    char text[128];
    sprintf(text, "%.2f", value);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), text);
}

void EdVectorControl::cbSelected(eduimenu_s *menu, eduiitem_s *item, u32 value) {
    EdVectorControl *control = static_cast<EdVectorControl *>(item->data_ptr);
    control->item = item;
    if (control->SelectSubObject() != 0)
        theClassEditor.SetMode(3);
    else
        theClassEditor.SetMode(0);
    thePropertyTool.SetMenuControl(menu, control);
}

void EdClassInterface::ClearLevel(i32) {
}

void EdClassInterface::DefunctObject(void *) {
}

void EdClassInterface::ReviveObject(void *) {
}

void EdClassInterface::SetObjectGuid(void *, i32) {
}

i32 EdClassInterface::GetObjectGuid(void *) {
    return 0;
}

i32 EdClassInterface::GetConstructorData(void *, void *, i32) {
    return 0;
}

void EdClassInterface::Construct(void *, void *) {
}

void EdClassInterface::Process(void *, EdInputContext &) {
}

void EdClassInterface::Render(void *, i32) {
}

void EdClassInterface::EnterEditor() {
}

void EdClassInterface::ExitEditor() {
}

void EdClassInterface::EnterLevel() {
}

void EdClassInterface::ExitLevel() {
}

void EdClassInterface::UpdateLists(MemoryBuffer *, MemoryBuffer *) {
}

void EdClassInterface::PreLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
}

void EdClassInterface::PostLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
}

void EdClassInterface::PreSaveInitialisation() {
}

void EdClassInterface::PostSaveInitialisation() {
}

void EdClassInterface::SerialiseObject(EdStream &, void *) {
}

void EdClassInterface::AddMenuItems(eduimenu_s *) {
}

void EdClassInterface::Import() {
}

void SplineHelper::Flush() {
    last_object = NULL;
    first_object = NULL;
    object_count = 0;
}

void KnotHelper::Flush() {
}

f32 EdClassInterface::DistanceToObject(VuVec &origin, VuVec &direction, void *object, EdRef **reference) {
    VuVec position;
    f32 radius = 1.0f;
    EdMember member;
    f32 distance = 3.402823466e38f;
    if (object_class->FindMember(&member, object, 8, 1)) {

        member.reference->GetAttributeData(member.object, 8, EdType_VuVec, &position, 0);
        if (object_class->FindMember(&member, object, 0x40, 1))
            member.reference->GetAttributeData(member.object, 0x40, EdType_Float, &radius, 0);
        distance = LineToPointDistance(origin, direction, position, NULL) - radius;
        if (distance < 0.0f)
            distance = 0.0f;
    }
    if (reference != NULL)
        *reference = NULL;
    return distance;
}

f32 EdClassInterface::DistanceToObject(VuVec &point, void *object, EdRef **reference) {
    VuVec position;
    f32 radius = 0.0f;
    EdMember member;
    f32 distance = 3.402823466e38f;
    if (object_class->FindMember(&member, object, 8, 1)) {

        member.reference->GetAttributeData(member.object, 8, EdType_VuVec, &position, 0);
        if (object_class->FindMember(&member, object, 0x40, 1))
            member.reference->GetAttributeData(member.object, 0x40, EdType_Float, &radius, 0);
        VuVec delta;
        delta.x = point.x - position.x;
        delta.y = point.y - position.y;
        delta.z = point.z - position.z;
        distance = NuVecMag(&delta.xyz) - radius;
        if (distance < 0.0f)
            distance = 0.0f;
    }
    if (reference != NULL)
        *reference = NULL;
    return distance;
}

void *EdClassInterface::GetNextObject(void *current, i32 (*filter)(void *)) {
    void *next = vtable->get_next_object(this, current);
    while (next != NULL && filter(next) == 0)
        next = vtable->get_next_object(this, next);
    return next;
}

EdSfxNameControl *sfxNameControl;

void EdSfxNameControl::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    EdSfxNameControl *control = new (theMemoryManager.AllocPool(sizeof(EdSfxNameControl), 1)) EdSfxNameControl();
    if (control) {
        control->reference = member;
        control->object = target;
        char value[128];
        control->GetVal(value, sizeof(value));
        control->item = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected,
                                           cbChanged, cbButton, 1, member->name, value);
        eduiMenuAddItem(menu, control->item);
    }
}

EdSfxNameControl::EdSfxNameControl() {
}

void EdSfxNameControl::cbButton(eduimenu_s *menu, eduiitem_s *item, u32) {
    sfxNameControl = static_cast<EdSfxNameControl *>(item->data_ptr);
    eduimenu_s *choices =
        eduiMenuCreate(item->x + menu->width, item->y, 180, 250,
                       reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (choices) {
        char value[128];
        sfxNameControl->GetVal(value, sizeof(value));
        eduiMenuAddItem(choices, eduiItemSelCreate(static_cast<usize>(-1), item->colours, 0, 0, cbSelectSfx,
                                                   const_cast<char *>("None")));
        for (i32 id = 0;; ++id) {
            char *name = GetSfxName(id);
            if (!name)
                break;
            eduiMenuAddItem(choices, eduiItemSelCreate(static_cast<usize>(id), item->colours, 0, 0, cbSelectSfx, name));
        }
        choices->flags |= 1;
        eduiMenuAttach(menu, choices);
        eduiMenuFitWidth(choices, 5);
        reinterpret_cast<u8 *>(item)[0x4c] &= ~8;
    }
}

void EdSfxNameControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
}

void EdSfxNameControl::cbSelectSfx(eduimenu_s *menu, eduiitem_s *item, u32) {
    if (sfxNameControl) {
        PlaySfxById(static_cast<i32>(item->data), NULL);
        char *name = GetSfxName(static_cast<i32>(item->data));
        if (!name)
            name = const_cast<char *>("None");
        eduiItemPropSetText(static_cast<edui_prop_s *>(sfxNameControl->item), name);
        sfxNameControl->SetVal(name);
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }
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
        u8 *cursor = static_cast<u8 *>(data);
        for (i32 i = 0; i < count; i++) {
            switch (size) {
                case 2:
                    EdFileSwapEndianess16(cursor);
                    break;
                case 4:
                    EdFileSwapEndianess32(cursor);
                    break;
            }
            cursor += size;
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
        u8 *cursor = static_cast<u8 *>(data);
        for (i32 i = 0; i < count; i++) {
            switch (size) {
                case 2:
                    EdFileSwapEndianess16(cursor);
                    break;
                case 4:
                    EdFileSwapEndianess32(cursor);
                    break;
            }
            cursor += size;
        }
    }
    i32 result = NuFileWrite(file, data, size * count);
    if (swap_endianness && size > 1) {
        u8 *cursor = static_cast<u8 *>(data);
        for (i32 i = 0; i < count; i++) {
            switch (size) {
                case 2:
                    EdFileSwapEndianess16(cursor);
                    break;
                case 4:
                    EdFileSwapEndianess32(cursor);
                    break;
            }
            cursor += size;
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
    menu = NULL;
}

static EdSpecialObjectControl *active_special_object_control;

static i32 SpecialObjectFilter(void *object) {
    return theSceneObjectHelper.scene_id == static_cast<Placeable *>(object)->scene_id;
}

void EdSpecialObjectControl::cbButton(eduimenu_s *parent, eduiitem_s *item, u32) {
    EdSpecialObjectControl *control = static_cast<EdSpecialObjectControl *>(item->data_ptr);
    active_special_object_control = control;
    eduimenu_s *choices =
        eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                       reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (choices == NULL)
        return;
    nuhspecial_s selected;
    control->reference->GetMemberData(control->object, EdType_NuHSpecial, &selected, 0);
    eduiMenuAddItem(choices, eduiItemSelCreate(static_cast<usize>(-1), item->colours, 0, 0, cbSelectObject,
                                               const_cast<char *>("None")));
    for (Placeable *object = static_cast<Placeable *>(thePlaceableHelper.GetNextObject(NULL, SpecialObjectFilter));
         object != NULL;) {
        eduiMenuAddItem(choices, eduiItemSelCreate(reinterpret_cast<usize>(object), item->colours, 0, 0, cbSelectObject,
                                                   const_cast<char *>(object->GetName())));
        Placeable *next_object =
            static_cast<Placeable *>(thePlaceableHelper.GetNextObject(object, SpecialObjectFilter));
        if (NuSpecialCompare(&static_cast<SpecialObject *>(object)->special, &selected) != 0)
            choices->selected = edui_last_item;
        object = next_object;
    }
    eduiMenuSortItemsByTxt(choices);
    choices->flags |= 1;
    eduiMenuAttach(parent, choices);
    eduiMenuFitWidth(choices, 5);
    eduiMenuFitOnScreen(choices, 30);
    static_cast<edui_prop_s *>(item)->unknown_property_flags &= ~8;
}

void EdSpecialObjectControl::cbChanged(eduimenu_s *, eduiitem_s *item, u32) {
    EdSpecialObjectControl *control = static_cast<EdSpecialObjectControl *>(item->data_ptr);
    nuhspecial_s selected;
    NuSpecialClear(&selected);
    char *name = reinterpret_cast<edui_prop_s *>(item)->property_text;
    for (Placeable *object = static_cast<Placeable *>(thePlaceableHelper.GetNextObject(NULL, SpecialObjectFilter));
         object != NULL;
         object = static_cast<Placeable *>(thePlaceableHelper.GetNextObject(object, SpecialObjectFilter))) {
        if (NuStrICmp(name, const_cast<char *>(object->GetName())) == 0) {
            selected = *static_cast<SpecialObject *>(object)->GetNuHSpecial();
            break;
        }
    }
    if (NuSpecialExistsFn(&selected) != 0) {
        eduiItemPropSetText(reinterpret_cast<edui_prop_s *>(item), NuSpecialGetName(&selected));
        control->reference->SetMemberData(control->object, EdType_NuHSpecial, &selected, 0, NULL);
    }
}

void EdSpecialObjectControl::cbSelectObject(eduimenu_s *, eduiitem_s *item, u32) {
    if (active_special_object_control == NULL)
        return;
    nuhspecial_s selected;
    nuhspecial_s *choice;
    void *data = item->data_ptr;
    if (reinterpret_cast<uintptr_t>(data) == static_cast<uintptr_t>(-1)) {
        NuSpecialClear(&selected);
        choice = &selected;
    } else {
        choice = &static_cast<SpecialObject *>(data)->special;
        char *name = NuSpecialGetName(choice);
        if (name != NULL)
            eduiItemPropSetText(reinterpret_cast<edui_prop_s *>(active_special_object_control->item), name);
    }
    EdSpecialObjectControl *control = active_special_object_control;
    control->reference->SetMemberData(control->object, EdType_NuHSpecial, choice, 0, NULL);
}

void EdSpecialObjectControl::AddMenuItem(eduimenu_s *parent, EdRef *member, void *target) {
    EdSpecialObjectControl *control =
        new (theMemoryManager.AllocPool(sizeof(EdSpecialObjectControl), 1)) EdSpecialObjectControl();
    if (!control)
        return;
    control->reference = member;
    control->object = target;
    nuhspecial_s special;
    member->GetMemberData(target, EdType_NuHSpecial, &special, 0);
    char *name = NuSpecialGetName(&special);
    if (name == NULL)
        name = const_cast<char *>("no name");
    control->item = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected, cbChanged,
                                       cbButton, 1, member->name, name);
    eduiMenuAddItem(parent, control->item);
}

void EdSpecialObjectControl::Process(EdInputContext &) {
}

void EdSpecialObjectControl::Render() {
    if (menu != NULL) {
        Placeable *object = reinterpret_cast<Placeable *>(menu);
        VuVec center = *object->GetCurrentPosition();
        f32 radius = object->GetRadius();
        EdDrawBegin(0);
        EdDrawLineSphere(center, radius, 1.0f, static_cast<i32>(0x80808080));
        EdDrawEnd();
    }
}

static EdClassObjectNameControl *edClassObjectNameControl;
extern eduimenu_s *edLevelDestroyThisMenu;
extern eduimenu_s *edLevelDestroyThisMenu2;

void EdClassObjectNameControl::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    void *memory = theMemoryManager.AllocPool(sizeof(EdClassObjectNameControl), 1);
    EdClassObjectNameControl *control = new (memory) EdClassObjectNameControl();
    if (control == NULL)
        return;
    control->reference = member;
    control->object = target;
    char value[128];
    control->GetVal(value, sizeof(value));
    control->item = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected, cbChanged,
                                       cbButton, 1, member->name, value);
    control->item->flags |= 4;
    eduiMenuAddItem(menu, control->item);
}

EdClassObjectNameControl::EdClassObjectNameControl() : selected{NULL, NULL, NULL} {
}

inline EdClassObjectNameControl::~EdClassObjectNameControl() {
}

inline void EdClassObjectNameControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdClassObjectNameControl));
}

void EdClassObjectNameControl::Process(EdInputContext &input) {
    if (input.GetHold(0x16) != 0.0f) {
        ClassObject selected = theClassEditor.current_object;
        theClassEditor.field_3c = static_cast<i32>(0xff008000);
        if (input.GetPress(0x19) != 0.0f) {
            char name[128];
            selected.GetName(name, sizeof(name));
            SetVal(name);
        }
    }
}

void EdClassObjectNameControl::Render() {
}

void EdClassObjectNameControl::cbButton(eduimenu_s *parent, eduiitem_s *item, u32) {
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    edClassObjectNameControl = static_cast<EdClassObjectNameControl *>(item->data_ptr);
    if (menu == NULL)
        return;
    for (i32 index = 0; index < theRegistry.class_count; ++index) {
        EdClass *ed_class = theRegistry.GetClass(index);
        if ((ed_class->flags & 0x20000000) == 0 && ed_class->interface != NULL) {
            eduiMenuAddItem(menu, eduiItemSelCreate(index, &EdLevelAttr, 0, 0, cbSelectClass, ed_class->name));
        }
    }
    if (menu->first == NULL) {
        eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect,
                                                const_cast<char *>("No Registered Classes")));
    }
    menu->flags |= 1;
    eduiMenuAttach(parent, menu);
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 5);
    static_cast<edui_prop_s *>(item)->unknown_property_flags &= ~8;
}

void EdClassObjectNameControl::cbChanged(eduimenu_s *, eduiitem_s *item, u32) {
    EdClassObjectNameControl *control = static_cast<EdClassObjectNameControl *>(item->data_ptr);
    ClassObject selected{};
    selected.Set(static_cast<edui_prop_s *>(item)->property_text);
    char name[128];
    selected.GetName(name, sizeof(name));
    eduiItemPropSetText(static_cast<edui_prop_s *>(control->item), name);
    control->SetVal(name);
}

void EdClassObjectNameControl::cbSelectClass(eduimenu_s *parent, eduiitem_s *item, u32) {
    EdClass *ed_class = theRegistry.GetClass(item->data);
    edClassObjectNameControl->selected.ed_class = ed_class;
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (menu == NULL)
        return;
    EdRef *name_reference = ed_class->FindTypeRef(2, 1);
    if (name_reference != NULL) {
        for (void *object = ed_class->interface->vtable->get_next_object(ed_class->interface, NULL); object != NULL;
             object = ed_class->interface->vtable->get_next_object(ed_class->interface, object)) {
            char name[128];
            if (name_reference->GetAttributeData(object, 2, EdType_String, name, sizeof(name))) {
                eduiMenuAddItem(
                    menu, eduiItemSelCreate(reinterpret_cast<usize>(object), &EdLevelAttr, 0, 0, cbSelectObject, name));
            }
        }
    }
    if (menu->first == NULL) {
        eduiMenuAddItem(
            menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect, const_cast<char *>("No Object")));
    }
    menu->flags |= 1;
    eduiMenuAttach(parent, menu);
    eduiSetActiveMenu(menu);
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 5);
}

void EdClassObjectNameControl::cbSelectObject(eduimenu_s *menu, eduiitem_s *item, u32) {
    EdClassObjectNameControl *control = edClassObjectNameControl;
    if (control != NULL) {
        control->selected.object = item->data_ptr;
        char name[128];
        const char *value;
        if (__builtin_expect(control->selected.object != NULL, 1)) {
            control->selected.GetName(name, sizeof(name));
            value = name;
        } else {
            value = "None";
        }
        eduiItemPropSetText(static_cast<edui_prop_s *>(edClassObjectNameControl->item), const_cast<char *>(value));
        edClassObjectNameControl->SetVal(value);
    }
    edLevelDestroyThisMenu = menu;
    edLevelDestroyThisMenu2 = menu->parent;
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
    if (__builtin_expect((attributes & attribute) != 0, 0)) {
        if ((attribute & 8) && type_id == EdType_VuMtx) {
            NUMTX_ALIGNED16 matrix;
            GetMemberData(object, type_id, &matrix, 0);
            u32 *out = static_cast<u32 *>(data);
            const u32 *row = reinterpret_cast<const u32 *>(&matrix.m30);
            out[0] = row[0];
            out[1] = row[1];
            out[2] = row[2];
            out[3] = row[3];
            return 1;
        }
        if (attribute & 0x100) {
            i16 value;
            GetMemberData(object, EdType_Short, &value, 0);
            *static_cast<i16 *>(data) = value;
            return 1;
        }
        GetMemberData(object, requested_type, data, data_size);
        return 1;
    }
    return 0;
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

inline EdSubSystem::~EdSubSystem() {
}

__attribute__((weak)) void EdSubSystem::SubInitialise(variptr_u &, variptr_u &, i32) {
}

__attribute__((weak)) void EdSubSystem::SubReset() {
}

__attribute__((weak)) void EdSubSystem::SubProcess(float) {
}

__attribute__((weak)) void EdSubSystem::SubRender() {
}

SAGA_HOST_LINKABLE_DTOR
EdControl::~EdControl() {
}

inline void EdControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdControl));
}

// Match each control's allocation type, rather than inheriting the base size.
inline void EdEnumControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdEnumControl));
}

inline void EdBitControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdBitControl));
}

void EdControl::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    EdControl *control = new (theMemoryManager.AllocPool(sizeof(EdControl), 1)) EdControl;
    control->reference = member;
    control->object = target;
    control->item = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, cbSelected, nullptr, nullptr, 0,
                                       member->name, nullptr);
    eduiMenuAddItem(menu, control->item);
}

void EdControl::Process(EdInputContext &) {
}

void EdControl::Render() {
}

// The original realigns the stack for its local ClassObject.
__attribute__((force_align_arg_pointer)) i32 EdControl::SelectSubObject() {
    ClassObject selection = {};
    ClassObjectListEntry *entry;
    for (entry = theClassEditor.selected_objects.first; entry; entry = entry->next) {
        if (entry->object == object)
            break;
    }
    if (entry == NULL)
        return 1;
    selection.ed_class = entry->ed_class;
    selection.object = entry->object;
    selection.reference = entry->reference;
    if (selection.object != NULL) {
        i32 mode = static_cast<i32>(Input->GetHold(16)) != 0 ? 1 : 2;
        selection.reference = reference;
        theClassEditor.SelectObject(selection, mode);
        if (!theClassEditor.selected_objects.IsInList(selection.object, NULL))
            theClassEditor.SelectObject(selection, 1);
    }
    return 1;
}

void EdControl::Refresh() {
}

void EdControl::SetMenuItemAttr(i32 mask, eduiitem_s *menu_item, eduiiattr_s *selected, eduiiattr_s *unselected) {
    if (reference->attributes & mask) {
        ClassObjectListEntry *entry = theClassEditor.selected_objects.first;
        if (entry == NULL)
            goto use_unselected;
        volatile i32 subobject_count = 0;
        for (; entry; entry = entry->next) {
            if (entry->object != object || entry->reference == NULL)
                continue;
            ++subobject_count;
            if (entry->reference == reference)
                goto use_unselected;
        }
        if (subobject_count == 0)
            goto use_unselected;
    }
    memcpy(menu_item->colours, selected, sizeof(*selected));
    return;
use_unselected:
    memcpy(menu_item->colours, unselected, sizeof(*unselected));
}

void EdControl::cbSelected(eduimenu_s *menu, eduiitem_s *menu_item, u32) {
    EdControl *control = static_cast<EdControl *>(menu_item->data_ptr);
    i32 selected = control->SelectSubObject();
    theClassEditor.SetMode(0);
    if (selected != 0)
        memcpy(menu_item->colours, &thePropertyTool.unselected_attr, sizeof(eduiiattr_s));
    thePropertyTool.SetMenuControl(menu, control);
}

EdManMove::EdManMove() {
    selected_attribute = 8;
}

static i32 get_manipulator_attribute(ClassObjectListEntry *entry, i32 attribute, i32 type, void *data) {
    if (entry->reference != NULL && entry->reference->GetAttributeData(entry->object, attribute, type, data, 0))
        return 1;
    EdMember member;
    return entry->ed_class->FindMember(&member, entry->object, attribute, 1) &&
           member.reference->GetAttributeData(member.object, attribute, type, data, 0);
}

static void set_manipulator_attribute(ClassObjectListEntry *entry, i32 attribute, i32 type, void *data) {
    if (entry->reference != NULL && entry->reference->SetAttributeData(entry->object, attribute, type, data, 0))
        return;
    EdMember member;
    if (entry->ed_class->FindMember(&member, entry->object, attribute, 1))
        member.reference->SetAttributeData(member.object, attribute, type, data, 0);
}

__attribute__((force_align_arg_pointer)) i32 EdManMove::Process(EdInputContext &input, ClassObjectList &selected) {
    VuVec position;
    EdManipulator::Process(input, selected);
    VuVec average;
    if (selected.GetAveragePosition(average) == 0)
        return 0;
    VuVec first_axis;
    VuVec second_axis;
    i32 axis = SelectAxis(input, average, first_axis, second_axis, NULL);
    theLevelEditor.field_0x2c = AxisColour[axis];
    if (input.GetHold(3) == 0.0f && input.GetHold(38) == 0.0f)
        return axis != 0;
    const VuVec *delta = reinterpret_cast<VuVec const *>(reinterpret_cast<u8 *>(this) + 0x40);
    for (ClassObjectListEntry *entry = selected.first; entry != NULL; entry = entry->next) {
        position = VuVec_Zero;
        get_manipulator_attribute(entry, 8, EdType_VuVec, &position);
        if (axis == 7) {
            if (input.GetHold(38) == 0.0f)
                continue;
            memcpy(&position, theLevelEditor.background_colour, sizeof(position));
        } else {
            f32 amount = delta->x * first_axis.x + delta->y * first_axis.y + delta->z * first_axis.z;
            if (amount != 0.0f) {
                position.y += first_axis.y * amount;
                position.z += first_axis.z * amount;
                position.x += first_axis.x * amount;
            }
            if (axis >= 4) {
                f32 second_amount = delta->x * second_axis.x + delta->y * second_axis.y + delta->z * second_axis.z;
                if (second_amount != 0.0f) {
                    position.y += second_axis.y * second_amount;
                    position.x += second_axis.x * second_amount;
                    position.z += second_axis.z * second_amount;
                } else if (amount == 0.0f) {
                    continue;
                }
            } else if (amount == 0.0f) {
                continue;
            }
        }
        theClassEditor.SnapPoint(position);
        set_manipulator_attribute(entry, 8, EdType_VuVec, &position);
    }
    return axis != 0;
}

void EdManMove::Render(ClassObjectList &selected) {
    if (selected.count > 2)
        EdManipulator::Render(selected);
    VuVec average;
    if (selected.GetAveragePosition(average) != 0)
        DrawAxis(average, NULL);
}

__attribute__((force_align_arg_pointer)) void EdRefKnot::GetMemberData(void *object, i32 type, void *data,
                                                                       i32 data_size) {
    SplineKnot *knot = static_cast<SplineKnot *>(object);
    CheckType(type);
    switch (member_offset) {
        case static_cast<i32>(0x80000001):
            NuStrNCpy(static_cast<char *>(data), knot->spline->name, data_size);
            break;
        case static_cast<i32>(0x80000002):
            *static_cast<f32 *>(data) = 0.25f * EdManipulator::Scale;
            break;
        // The original copies each 16-byte vector with paired SSE half-loads and stores.
        case static_cast<i32>(0x80000003): {
#if defined(__SSE__)
            const __m64 *source = reinterpret_cast<const __m64 *>(&knot->position);
            __m128 lanes = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), source), source + 1);
            _mm_storel_pi(reinterpret_cast<__m64 *>(data), lanes);
            _mm_storeh_pi(reinterpret_cast<__m64 *>(static_cast<f32 *>(data) + 2), lanes);
#else
            *static_cast<VuVec *>(data) = knot->position;
#endif
            break;
        }
        case static_cast<i32>(0x80000004): {
#if defined(__SSE__)
            const __m64 *source = reinterpret_cast<const __m64 *>(&knot->in_tangent);
            __m128 lanes = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), source), source + 1);
            _mm_storel_pi(reinterpret_cast<__m64 *>(data), lanes);
            _mm_storeh_pi(reinterpret_cast<__m64 *>(static_cast<f32 *>(data) + 2), lanes);
#else
            *static_cast<VuVec *>(data) = knot->in_tangent;
#endif
            break;
        }
        case static_cast<i32>(0x80000005): {
#if defined(__SSE__)
            const __m64 *source = reinterpret_cast<const __m64 *>(&knot->out_tangent);
            __m128 lanes = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), source), source + 1);
            _mm_storel_pi(reinterpret_cast<__m64 *>(data), lanes);
            _mm_storeh_pi(reinterpret_cast<__m64 *>(static_cast<f32 *>(data) + 2), lanes);
#else
            *static_cast<VuVec *>(data) = knot->out_tangent;
#endif
            break;
        }
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
            f32 dy = position->y - knot->position.y;
            f32 dx = position->x - knot->position.x;
            f32 dz = position->z - knot->position.z;
            knot->position.y += dy;
            knot->position.x += dx;
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
        case static_cast<i32>(0x80000004): {
#if defined(__SSE__)
            const __m64 *source = reinterpret_cast<const __m64 *>(data);
            __m128 lanes = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), source), source + 1);
            _mm_storel_pi(reinterpret_cast<__m64 *>(&knot->in_tangent), lanes);
            _mm_storeh_pi(reinterpret_cast<__m64 *>(&knot->in_tangent) + 1, lanes);
#else
            knot->in_tangent = *static_cast<VuVec *>(data);
#endif
            break;
        }
        case static_cast<i32>(0x80000005): {
#if defined(__SSE__)
            const __m64 *source = reinterpret_cast<const __m64 *>(data);
            __m128 lanes = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), source), source + 1);
            _mm_storel_pi(reinterpret_cast<__m64 *>(&knot->out_tangent), lanes);
            _mm_storeh_pi(reinterpret_cast<__m64 *>(&knot->out_tangent) + 1, lanes);
#else
            knot->out_tangent = *static_cast<VuVec *>(data);
#endif
            break;
        }
        default:
            // The original setter delegates unrecognized members to the getter.
            EdRef::GetMemberData(object, type, data, data_size);
            return;
    }
    if (theSplineHelper.auto_generate_points && knot->spline) {
        knot->spline->points.Clear();
    }
}

// ==== src/gameapi/edtools/edtoolsall_plain.cpp: declarations ====
#include "decomp.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edui_colour.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/nupad.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nufile/nufile.h"

// Retail keeps an unreferenced partial-inlining clone of the old HSV2RGB
// helper (the non-grey path) in this unit.
#if defined(__i386__)
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone, regparm(2), sseregparm))
#else
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone))
#endif
static HSV2RGB_BODY_ATTR void
HSV2RGBBody(f32 hue, f32 saturation, f32 value, f32 *red, f32 *green, f32 *blue) __asm__("_ZL7HSV2RGBfffPfS_S_.part.18");
static void HSV2RGBBody(f32 hue, f32 saturation, f32 value, f32 *red, f32 *green, f32 *blue);

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// The original animation editor callbacks and lifecycle share file-local state.
// ---- begin src/gameapi/edtools/edanimall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"
#include "gameframework/saveload.h"

extern "C" i32 edSfxAllCount;

// Animation editor subsystem stubs (static, internal linkage).

static __attribute__((used)) void edanimcbCubeMap(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbFileLoad(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbFileSave(eduimenu_s *menu, eduiitem_s *, u32);
void edanimRegisterBaseScene(NUGSCN *scene);
static __attribute__((used)) void edanimcbMCTBMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSoundMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbBouncyMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSwitchMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSwitchId(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbMCTBCardType(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbParticleMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSoundType(eduimenu_s *menu, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSetSwitchVar(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbCancelOptMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSoundTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbCancelMCTBMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbLocalSoundMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbMCTBCardFormat(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSoundTiming(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSetSwitchDelay(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32);

// Remaining animation-editor UI/menu callbacks.

static __attribute__((used)) void edanimcbCancelSoundMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardPresent(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetParticleRate(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbSetParticleType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbToggleSoundType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelBouncyMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelSwitchMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardLoadSlot(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbMCTBCardSaveSlot(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbMCTBCardUnFormat(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbParticleTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetBouncyDamping(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbSetBouncyTension(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbLocalParticleMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardFreeSpace(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardSlotsUsed(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetLocalSoundType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelParticleMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbLocalSoundTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardDeleteSlot(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelSoundTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardCheckFormat(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetBouncyPlayerGrav(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelLocalSoundMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardCheckKeyCard(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardWriteKeyCard(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetLocalParticleType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbToggleParticleSwitch(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbLocalParticleTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbCancelParticleTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelLocalParticleMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelLocalSoundTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelLocalParticleTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbBouncyMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSoundMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbParticleMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSwitchMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalParticleMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSoundTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalSoundTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalParticleTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbParticleTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSwitchTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalSoundMenu(eduimenu_s *parent, eduiitem_s *, u32);

// ---- end src/gameapi/edtools/edanimall.cpp ----
// ---- begin src/gameapi/edtools/edmenucallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include <cstdio>

extern "C" i32 PS2_REZ_H;
extern "C" void cbCancelSubMenu(eduimenu_s *, eduimenu_s *);

// Editor UI callbacks retained in the original editor's menu code.
#pragma GCC push_options
#pragma GCC optimize("O2", "omit-frame-pointer")

static eduimenu_s *submenu1;
static eduimenu_s *submenu0;
static eduimenu_s *testmenu0;
static i32 selection_value;
static f32 cliph;
static f32 clips;
static f32 clipv;
static f32 clipg;
static f32 clipboard_r = 0.5f;
static f32 clipboard_g = 0.5f;
static f32 clipboard_b = 0.5f;

static eduimenu_s *colourmenu;
static u32 *cp_rgba;
static f32 cpt_r;
static f32 cpt_g;
static f32 cpt_b;
static f32 cpt_a;
static f32 *cp_r;
static f32 *cp_g;
static f32 *cp_b;
static f32 *cp_a;
static edui_colour_pick_s *cp_item;
static eduiitem_s *cp_info;
static i32 (*cp_process)(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
static eduiitem_s *cp_copy;
static eduiitem_s *cp_paste;

static __attribute__((always_inline)) inline u32 cpPackColour(f32 red, f32 green, f32 blue, f32 alpha) {
    return (static_cast<i32>(alpha * 255.0f) << 24) | ((static_cast<i32>(blue * 255.0f) & 0xff) << 16) |
           ((static_cast<i32>(green * 255.0f) & 0xff) << 8) | (static_cast<i32>(red * 255.0f) & 0xff);
}

static __attribute__((always_inline)) inline void cpUpdateInfo(eduiitem_s *info, edui_colour_pick_s *picker) {
    char text[256];
    if (info->data != 0) {
        sprintf(text, "R:%1.2f G:%1.2f B:%1.2f", picker->red, picker->green, picker->blue);
    } else {
        sprintf(text, "H:%1.2f S:%1.2f V:%1.2f", picker->hue, picker->saturation, picker->value);
    }
    eduiItemSetText(info, text);
}

static __used__ void cbColourPickSel(eduimenu_s *menu, eduiitem_s *, u32);
static __used__ void cbToggleIndicatorMode(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void cbCopy(eduimenu_s *, eduiitem_s *, u32);
static __used__ void cbPaste(eduimenu_s *, eduiitem_s *, u32);
static __used__ i32 cbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, float delta_time, nupad_s *pad);

extern "C" void CreateColourPicker(void);

extern "C" void AddColourPick(eduimenu_s *parent, eduiitem_s *item, f32 *red, f32 *green, f32 *blue, u32 *rgba);

extern "C" {

static __used__ void cbGradChange(void);

static __used__ void cbSel(eduimenu_s *, eduiitem_s *item);

static __used__ void cbSubMenu(eduimenu_s *menu, eduiitem_s *item);

static __used__ void cbgpcfgAdd(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCPPress(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCopy(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgDel(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgPaste(eduimenu_s *menu, eduiitem_s *item, u32 value);

} // extern "C"

#pragma GCC pop_options

// ---- end src/gameapi/edtools/edmenucallbacks.cpp ----

void edgraDoInput(nupad_s *pad);
extern "C" void edgraClumpsReset(void);
extern "C" void edbitsDrawCircleTilted(NUVEC *centre, f32 radius, i32 colour, NUMTL *material, i32 rotation_z,
                                       i32 rotation_y);
extern "C" i32 NuRndrDoingScreenGrab;
static i32 editor_return;
static i32 bShowCursor = 1;
static NUQFNT *edui_font;
static i32 edui_donotdraw;
static i32 item_width;
static void *dir_list;
static i32 dir_size;
static eduimenu_s *filepick_menu;
static edui_file_pick_s *filepick_item;

static char ed_levelfile[256];
static f32 edui_font_scale_x = 0.9f;
static f32 edui_font_scale_y = 0.9f;
static f32 edmain_menu_scale = 1.0f;
static u32 edui_cursor_colour = 0xff000000;
static char eduiPropTextEdit[256];
static char eduiPropTextStore[256];
static char *textrow[4];
static char TextPickCopyBuffer[0x40];
static i32 cursor_flash;


// ---- begin src/gameapi/edtools/edptlall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/fx.h"
#include "nu2api/numusic/sfx.h"
#include <stdio.h>

extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern debinftype *effecttypes;
    extern i32 debris_render_group;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern eduimenu_s *edptl_repeatbox_menu;
    extern i32 edpp_usememcard;
    extern i32 edbits_override_backups;
    extern i32 edbits_particle_general_page;
    extern i32 edbits_particle_level_page;
extern i32 edbits_particle_char_page;
    extern NUGSCN *edbits_base_scene;
    extern char edbits_datapath[256];
    extern char edbits_general_save_directory[256];
    extern char edbits_general_save_name[256];
    extern char edbits_general_save_extension[256];
    extern char edbits_level_save_directory[256];
    extern char edbits_level_save_name[256];
    extern char edbits_level_save_extension[256];
    void DebFreeInstantly(i32 *handle);
    void DebReAlloc(debkeydatatype_s *key, i32 particle_count);
    void DebrisSetDetailLevels(i32 handle, i32 detail_levels);
    void edppDeleteEffect(i32 index);
    i32 edppLoadPage(char *path, i32 flag, usize scene);
    void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
    void DebrisReScale(i32 effect_index, f32 scale);
    void edppRestartAllEffectsInLevel(void);
}

void cbFileSaveEffects(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbClipboardMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbOrphanListMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbDeleteOrphans(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbGhostMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbStarMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeDistortX(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeDistortY(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetScaleFactor(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSoundIDMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeGenRate(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeETime(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlRepeatBoxMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeRampTime(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchType(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCutClipboard(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbPasteClipboard(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbEmptyClipboard(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOn(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOnRan(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOffRan(eduimenu_s *, eduiitem_s *, u32);
static void UpdateTotalPtls(debinftype *effect);

// The original particle editor's private callbacks and menu state share this TU.
// ---- begin src/gameapi/edtools/edptlcallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/nu3d/numtl.h"
#include <stdio.h>
#include <string.h>
#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern NUMTL *DebMat[];
    extern i32 edpp_readout;
    extern eduimenu_s *sscalemenu;
    extern eduimenu_s *ptlgravmenu;
    extern eduimenu_s *ptlemitvelmenu;
    extern eduimenu_s *ptlreadoutmenu;
    extern eduimenu_s *edptl_damage_menu;
    extern eduimenu_s *edptl_damageflag_menu;
    extern eduimenu_s *namemenu;
}

static void cbPtlCancelSScaleMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelGravMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelEmitVelMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelReadoutMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelDamageMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelDamageFlagMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangeGrav(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeEmitVel(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeSScale(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSelReadout(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeDamageFlags(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDamageFlagMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlTorusMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelSoundControlMenu(eduimenu_s *, eduimenu_s *);
static void cbCancelChangeNameMenu(eduimenu_s *, eduimenu_s *);
static void cbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangeTextureSelect(eduimenu_s *, eduiitem_s *, u32);


extern "C" {
    extern eduimenu_s *edpp_active_menu;
    extern eduimenu_s *edptl_testdetail_menu;
    extern eduimenu_s *edptl_detail_menu;
    extern eduimenu_s *edptl_drawflag_menu;
    extern eduimenu_s *edptl_group_menu;
    extern eduimenu_s *edptl_switchtype_menu;
    extern eduimenu_s *edptl_switch_menu;
    extern eduimenu_s *dpadmodemenu;
    extern eduimenu_s *sscalemenu;
    extern eduimenu_s *edptl_scaleeffect_menu;
    extern eduimenu_s *edptl_instancesettings_menu;
    extern eduimenu_s *edptl_torus_menu;
    extern eduimenu_s *collmenu;
    extern eduimenu_s *edptl_damageflag_menu;
    extern eduimenu_s *edptl_damage_menu;
    extern eduimenu_s *edptl_soundcontrol_menu;
    extern eduimenu_s *edptl_soundid_menu;
    extern eduimenu_s *edptl_soundx_menu;
    extern eduimenu_s *edptl_sounds_menu;
    extern eduimenu_s *edptl_bounce_menu;
    extern eduimenu_s *ptljibmenu;
    extern eduimenu_s *ptlrotmenu;
    extern eduimenu_s *ptlsizemenu;
    extern eduimenu_s *edptl_ghost_menu;
    extern eduimenu_s *ptlcutoffmenu;
    extern eduimenu_s *emittimemenu;
    extern eduimenu_s *etimemenu;
    extern eduimenu_s *changegenratemenu;
    extern eduimenu_s *ptlvaremitmenu;
    extern eduimenu_s *ptlstartvelmenu;
    extern eduimenu_s *ptlvarstartmenu;
    extern eduimenu_s *ptlgravmenu;
    extern eduimenu_s *ptlemitvelmenu;
    extern eduimenu_s *ptlemitmenu;
    extern eduimenu_s *textureselectmenu;
    extern eduimenu_s *texturemenu;
    extern eduimenu_s *ptlcolmenu;
    extern eduimenu_s *ptlgsortmenu;
    extern eduimenu_s *ptlreadoutmenu;
    extern eduimenu_s *edptl_quickdel_menu;
    extern eduimenu_s *edptl_orphanlist_menu;
    extern eduimenu_s *ptlclipmenu;
    extern eduimenu_s *messagemenu;
    extern eduimenu_s *ptldatamenu;
    extern eduimenu_s *ptltypemenu;
    extern eduimenu_s *effectlistmenu;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern u32 edgrey[4];
    extern u32 eddarkred[4];
    extern i32 EDPP_MAX_TYPES;
    extern i32 edbits_particle_level_page;
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern DEBRISGENERATOR gensorttab[13];
    extern DEBRISMOMENTUMADJUSTER gencodetab[7];
    void edppDeleteEffect(i32 index);
    void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);
    void DebrisOrientation(i32 handle, i16 z, i16 y);
    void DebrisEmitterOrientation(i32 handle, i16 z, i16 y, i16 x);
    void DebrisStartOffset(i32 handle, f32 offset);
    void DebrisSetTrigger(i32 handle, i32 type, i32 id, f32 value);
    void DebrisReflectionOrientation(i32 handle, i16 z, i16 y, f32 offset, f32 bounce);
    void DebrisSetFacing(i32 handle, u8 enabled, i16 x, i16 y);
    void DebrisSetGroupID(i32 handle, i16 group);
    void DebrisSetRoomID(i32 handle, nugscn_s *scene);
}



extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern i32 edpp_readout;
    extern i32 edpp_showAllPlaced;
    extern i32 edpp_snap_enabled;
    extern eduimenu_s *ptlvaremitmenu;
    extern eduimenu_s *ptlvarstartmenu;
    extern eduimenu_s *ptlstartvelmenu;
}
static void cbPtlCancelGSortMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelTextureMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelQuickDeleteMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlQuickDeleteType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSelTextureType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlTextureSelectMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlAddEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCopyEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDeleteEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelDataMenu(eduimenu_s *, eduimenu_s *);
static void cbChangeNameMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlQuickDeleteMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlReadoutMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbFileLoadEffects(eduimenu_s *, eduiitem_s *, u32);
static void cbCancelEffectListMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelTypeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlSelType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangePriority(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlToggleDynamicPriority(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSetFacing(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbScaleEffectMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelEmitMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlEmitVelMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlGravMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlVarStartMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlStartVelMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlVarEmitMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeGenRateMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeETimeMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlEmitTimeMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCutOffMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelSizeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplySize(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCopySize(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelColMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplyGrad(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelJibMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelRotMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplyJib(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyRot(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelScaleEffectMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbApplyScaleFactor(eduimenu_s *, eduiitem_s *, u32);
static void cbCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *);
static void cbCancelChangeETimeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelCutOffMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangeRepeatFlag(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCutOn(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeDrawCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeSoundCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCameraCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelVarEmitMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelStartVelMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelVarStartMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelCollMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplyCollEnv(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeNumCollSpheres(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDefaultCollEnv(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelTorusMenu(eduimenu_s *, eduimenu_s *);
static void cbChangeTorusLife(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeTorusRad1(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeTorusRad2(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyTorusEnv1(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyTorusEnv2(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyTorusEnv3(eduimenu_s *, eduiitem_s *, u32);

// Particle-list editor UI/menu callbacks.

static void edptlcbApplyStarPoints(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelGhostMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelGroupMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbChangeCSDisable(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbScaleEffectMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void edptlcbSetDebrisDetail(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbSetSoundControl(eduimenu_s *menu, eduiitem_s *item, u32);

static void edptlcbApplyScaleFactor(eduimenu_s *menu, eduiitem_s *, u32);

static void edptlcbCancelBounceMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelDetailMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSoundXMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSoundsMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSwitchMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbSoundControlMenu(eduimenu_s *menu, eduiitem_s *item, u32);

static void edptlcbApplyBounceFactor(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbApplyBounceOffset(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelSoundIDMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbSetDebrisThinning(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelDpadModeMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelDrawflagMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbJumpToGameLocation(eduimenu_s *, eduiitem_s *item, u32);

static void edptlChangeRepeatBoxXZLock(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelClipboardMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelOrphanListMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelTestDetailMenu(eduimenu_s *, eduimenu_s *);



// Particle editor UI/menu callbacks.

static void cbChangeName(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeX(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlChangeY(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlChangeZ(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlColMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlJibMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlRotMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlSelType(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlShowAll(eduimenu_s *, eduiitem_s *, u32);



static void cbPtlCollMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlCopySize(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlDataMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlEmitMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlGravMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSelGCode(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlSelGSort(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlSizeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlAddEffect(eduimenu_s *menu, eduiitem_s *, u32);



static void cbPtlGSortMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSetFacing(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlTorusMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlChangeGrav(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlCopyEffect(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlCutOffMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlDamageMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSScaleMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSelReadout(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlSnapToggle(eduimenu_s *, eduiitem_s *item, u32);

static void cbSelEffectList(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbChangeNameMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbEffectListMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlChangeCutOn(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlEmitVelMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlReadoutMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSetXZFacing(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlTextureMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlVarEmitMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbChangeETimeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbChangeTorusLife(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeTorusRad1(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeTorusRad2(eduimenu_s *, eduiitem_s *item, u32);

static void cbFileLoadEffects(eduimenu_s *parent, eduiitem_s *, u32);


static void cbPtlChangeCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeSScale(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlDeleteEffect(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlEmitTimeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlStartVelMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlVarStartMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlChangeEmitVel(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeGenRateMenu(eduimenu_s *parent, eduiitem_s *, u32);




static void cbPtlChangePriority(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlDamageFlagMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlDefaultCollEnv(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSelTextureType(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlQuickDeleteMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlQuickDeleteType(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlChangeDrawCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeRepeatFlag(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeNumCollSpheres(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeDamageFlags(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeSoundCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlTextureSelectMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlChangeCameraCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeTextureSelect(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlInstanceSettingsMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlToggleDynamicPriority(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlCancel(eduimenu_s *, eduimenu_s *);
















static void cbCancelChangeNameMenu(eduimenu_s *menu, eduimenu_s *);














// ---- end src/gameapi/edtools/edptlcallbacks.cpp ----

void cbFileSaveEffects(eduimenu_s *parent, eduiitem_s *, u32);

static void edptlChangeRepeatBox(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbDrawflagMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyStarRatio(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyGhostTime(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyNumGhosts(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelStarMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbSetMasterGroup(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetMaxThin(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbTestDetailMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchId(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchVar(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelPageMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbStartPage(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbStopPage(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbClearPage(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlCancelRepeatBoxMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlRepeatBoxMenu(eduimenu_s *parent, eduiitem_s *, u32);


// Particle list editor subsystem stubs (static, internal linkage).

static void edptlcbPageMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetGroup(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbStarMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbStopPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbClearPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbGhostMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbGroupMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetDetail(eduimenu_s *, eduiitem_s *item, u32);
void edppStartPage(i32 page);
static void edptlcbStartPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbBounceMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbDetailMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetMaxThin(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetSoundID(eduimenu_s *menu, eduiitem_s *item, u32);
static void edptlcbSoundXMenu(eduimenu_s *parent, eduiitem_s *item, u32);
static void edptlcbSoundsMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSwitchMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetDpadMode(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetDrawflag(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetSwitchId(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSoundIDMenu(eduimenu_s *parent, eduiitem_s *item, u32);
static void edptlcbCutClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbDpadModeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbDrawflagMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetSwitchVar(eduimenu_s *, eduiitem_s *item, u32);
static void edptlChangeRepeatBox(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbClipboardMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbDeleteOrphans(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32);
static void edptlcbApplyGhostTime(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbApplyNumGhosts(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOffRan(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOff(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOnRan(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOn(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeETime(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeGenRate(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbApplyStarRatio(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbChangeDistortX(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbChangeDistortY(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbChangeRampTime(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbEmptyClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbOrphanListMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbPasteClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbResetParticles(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetMasterGroup(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetScaleFactor(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSwitchTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbTestDetailMenu(eduimenu_s *parent, eduiitem_s *, u32);

// ---- end src/gameapi/edtools/edptlall.cpp ----


void edppDetermineNearest(f32 distance);
extern "C" void DebrisStatusNormal(i32 *handle);

extern "C" {
    __attribute__((visibility("hidden"))) void tpentOnEnter(eduimenu_s *, eduiitem_s *, u32);
    __attribute__((visibility("hidden"))) void tpentPropOnEnter(eduimenu_s *, eduiitem_s *, u32);
    extern numtl_s *uimtls[5];
    extern i32 ui_bgmtl;
    extern i32 ui_outmtl;
    void NuRndrRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
    void NuRndrGradRect2di(i32 x, i32 y, i32 width, i32 height, i32 *colours, numtl_s *material);
    void NuRndrLineRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
    void NuRndrLine2di(i32 x0, i32 y0, i32 x1, i32 y1, i32 colour, numtl_s *material);
    void NuRndrLineStrip2di(i32 *points, f32 *uvs, i32 count, i32 colour, numtl_s *material);
    void NuRndrTriStrip2di(i32 *points, f32 *uvs, i32 count, i32 colour, numtl_s *material);
}

// The editor's draw helpers evaluate their arguments even during layout-only
// passes; only the renderer call is suppressed by edui_donotdraw.
static inline void eduiDrawRect(i32 x, i32 y, i32 width, i32 height, i32 colour, NUMTL *material) {
    if (!edui_donotdraw)
        NuRndrRect2di(x, y, width, height, colour, material);
}

static inline void eduiDrawLineRect(i32 x, i32 y, i32 width, i32 height, i32 colour, NUMTL *material) {
    if (!edui_donotdraw)
        NuRndrLineRect2di(x, y, width, height, colour, material);
}

static inline void eduiDrawGradRect(i32 x, i32 y, i32 width, i32 height, i32 *colours, NUMTL *material) {
    if (!edui_donotdraw)
        NuRndrGradRect2di(x, y, width, height, colours, material);
}

static inline void eduiDrawLine(i32 x0, i32 y0, i32 x1, i32 y1, i32 colour, NUMTL *material) {
    if (!edui_donotdraw)
        NuRndrLine2di(x0, y0, x1, y1, colour, material);
}

static inline void eduiDrawLineStrip(i32 *points, f32 *uvs, i32 count, i32 colour, NUMTL *material) {
    if (!edui_donotdraw)
        NuRndrLineStrip2di(points, uvs, count, colour, material);
}

static inline void eduiDrawTriStrip(i32 *points, f32 *uvs, i32 count, i32 colour, NUMTL *material) {
    if (!edui_donotdraw)
        NuRndrTriStrip2di(points, uvs, count, colour, material);
}

static inline void eduiDrawFontSet(NUQFNT *font) {
    if (!edui_donotdraw)
        NuQFntSet(font);
}

static inline void eduiSetFontColour(NUQFNT *font, u32 colour) {
    if (!edui_donotdraw)
        NuQFntSetColour(font, colour);
}

// Some original callers expect the draw to happen and keep the renderer call on the fallthrough path.
static inline void eduiDrawRectLikely(i32 x, i32 y, i32 width, i32 height, i32 colour, NUMTL *material) {
    if (__builtin_expect(!edui_donotdraw, 1))
        NuRndrRect2di(x, y, width, height, colour, material);
}

static inline void eduiDrawLineRectLikely(i32 x, i32 y, i32 width, i32 height, i32 colour, NUMTL *material) {
    if (__builtin_expect(!edui_donotdraw, 1))
        NuRndrLineRect2di(x, y, width, height, colour, material);
}

static inline void eduiDrawGradRectLikely(i32 x, i32 y, i32 width, i32 height, i32 *colours, NUMTL *material) {
    if (__builtin_expect(!edui_donotdraw, 1))
        NuRndrGradRect2di(x, y, width, height, colours, material);
}

static inline void eduiDrawLineLikely(i32 x0, i32 y0, i32 x1, i32 y1, i32 colour, NUMTL *material) {
    if (__builtin_expect(!edui_donotdraw, 1))
        NuRndrLine2di(x0, y0, x1, y1, colour, material);
}

static inline void eduiDrawLineStripLikely(i32 *points, f32 *uvs, i32 count, i32 colour, NUMTL *material) {
    if (__builtin_expect(!edui_donotdraw, 1))
        NuRndrLineStrip2di(points, uvs, count, colour, material);
}

static inline void eduiDrawTriStripLikely(i32 *points, f32 *uvs, i32 count, i32 colour, NUMTL *material) {
    if (__builtin_expect(!edui_donotdraw, 1))
        NuRndrTriStrip2di(points, uvs, count, colour, material);
}

static inline void eduiDrawFontSetLikely(NUQFNT *font) {
    if (__builtin_expect(!edui_donotdraw, 1))
        NuQFntSet(font);
}

static inline void eduiSetFontColourLikely(NUQFNT *font, u32 colour) {
    if (__builtin_expect(!edui_donotdraw, 1))
        NuQFntSetColour(font, colour);
}

// The original local font helpers read the same private draw state as the
// editor callbacks, so they belong to this translation unit.
static __used__ __attribute__((noclone)) void eduiFntPrintExGeneric(void *font, int x, int y,
                                                                                int alignment, char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz");
static __used__ void eduiFntPrintExGeneric(void *font, int x, int y, int alignment, char *format, ...);

// Left-aligned (16) specialisation of the integer print helper.
static __attribute__((noclone)) void eduiFntPrintExLeft(void *font, int x, int y, int alignment,
                                                                  char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.67");

// Centred (64) specialisation of the integer print helper.
static __attribute__((noclone)) void eduiFntPrintExCentre(void *font, int x, int y, int alignment,
                                                                    char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.68");

#if defined(__clang__)
// Clang has no __builtin_va_arg_pack; format here and print the result. The
// generic helper already owns this overload's mangled name, so rename it.
static void eduiFntPrintEx(void *font, int x, int y, int alignment, char *format, ...)
    __asm__("eduiFntPrintExFormatted");
static void eduiFntPrintEx(void *font, int x, int y, int alignment, char *format, ...) {
    char text[1024];
    va_list arguments;
    va_start(arguments, format);
    NuVSPrintf(text, format, arguments);
    va_end(arguments);
    eduiFntPrintExGeneric(font, x, y, alignment, const_cast<char *>("%s"), text);
}
#else
static inline __attribute__((always_inline)) void eduiFntPrintEx(void *font, int x, int y, int alignment,
                                                                 char *format, ...) {
    if (__builtin_constant_p(alignment) && alignment == 16)
        eduiFntPrintExLeft(font, x, y, alignment, format, __builtin_va_arg_pack());
    else if (__builtin_constant_p(alignment) && alignment == 64)
        eduiFntPrintExCentre(font, x, y, alignment, format, __builtin_va_arg_pack());
    else
        eduiFntPrintExGeneric(font, x, y, alignment, format, __builtin_va_arg_pack());
}
#endif

static void eduiFntPrintEx(void *font, float x, float y, int alignment, char *format, ...) {
    va_list arguments;
    va_start(arguments, format);
    eduiFntPrintEx(font, static_cast<i32>(x * 16.0f), static_cast<i32>(y * 8.0f), alignment, format, arguments);
    va_end(arguments);
}

// Only caller prints the property edit buffer left-aligned.
static __attribute__((noinline, noclone)) void eduiFntPrintExEditBuffer(void *font, float x, float y, int alignment,
                                                                        char *format, ...)
    __asm__("_ZL14eduiFntPrintExPvffiPcz.constprop.66");

static __used__ void eduiFntPrintClipEx(void *font, float x, float y, int alignment, float clip_x, float clip_width,
                                        char *format, ...);
// Only eduiMenuRender calls the integer clip helper, always left-aligned.
static __attribute__((noinline, noclone)) void eduiFntPrintClipEx(void *font, i32 x, i32 y, i32 alignment, i32 clip_x,
                                                                  i32 clip_width, char *format, ...)
    __asm__("_ZL18eduiFntPrintClipExPviiiiiPcz.constprop.70");
static void eduiFntPrintClipEx(void *font, i32 x, i32 y, i32, i32 clip_x, i32 clip_width, char *format, ...);
static i32 show_x_scale_help;
static i32 show_y_scale_help;

extern "C" i32 nugraphAddPoint(nugraph_s *graph, f32 x, f32 y);
extern "C" i32 nugraphDeletePoint(nugraph_s *graph, i32 index);
extern "C" i32 nugraphCalcCurve(nugraph_s *graph, i32 point_count);
extern "C" f32 nugraphGetYatX(nugraph_s *graph, f32 x, i32 mode);

#include "gameapi/edtools/edtools_types.h"

static void edgraInit();
static void edgraClose();
static void edgraEnter();
static i32 edgraProc(f32 delta_time, nupad_s *pad);
static void edgraRender();
static void edppInit();
static void edppClose();
static void edppEnter();
static void edppApply();
static i32 edppProc(f32 delta_time, nupad_s *pad);
static void edppRender();
static void edbriInit();
static void edbriClose();
static void edbriEnter();
static i32 edbriProc(f32 delta_time, nupad_s *pad);
static void edbriRender();
static void edanimInit();
static void edanimClose();
static void edanimEnter();
static i32 edanimProc(f32 delta_time, nupad_s *pad);
static void edanimRender();

extern "C" {
    void nugraphInit(nugraph_s *graph);
    i32 nugraphCalcCurve(nugraph_s *graph, i32 point_count);
    void eduicbItemDestroy(eduimenu_s *, eduiitem_s *);
    void eduicbItemDestroyProp(eduimenu_s *, eduiitem_s *);
    i32 eduicbInteractSlider(edui_interact_s *);
    static i32 eduicbProcessSel(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGradPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *);
    static i32 eduicbProcessSlider(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGraph(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessColourPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGreyPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessTexturePick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessColourSlider(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessExpander(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static void eduicbItemExpanderClose(edui_expander_s *);
    static i32 eduicbProcessFilter(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessFilePick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    // Retail only has IPA-SRA clones of these two helpers.
    static __attribute__((noinline, noclone)) void
    eduicbAttachDirectoryList(eduimenu_s *, edui_file_pick_s *) __asm__("eduicbAttachDirectoryList.isra.32");
    static void eduicbSelectDirectoryEntry(eduimenu_s *, eduiitem_s *, u32);
    static void eduicbSelectDirectoryExit(eduimenu_s *, eduiitem_s *, u32);
    static i32 eduicbProcessTextPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessProp(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static __attribute__((noinline, noclone)) i32
    eduicbProcessPropKeyboard(eduimenu_s *, edui_prop_s *) __asm__("eduicbProcessPropKeyboard.isra.27");
    static i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSelWithClipColour(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderGraph(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderColourPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTexturePick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderExpander(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderFilter(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderFilePick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderProp(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static void eduicbItemSliderDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemFilePickDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemTextPickDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemDestroyExpander(eduimenu_s *, eduiitem_s *);
    static void eduicbItemDestroyFilter(eduimenu_s *, eduiitem_s *);
    static i32 eduicbInteractSel(edui_interact_s *);
    static i32 eduicbInteractColourPick(edui_interact_s *);
    static i32 eduicbInteractExpander(edui_interact_s *);
    static i32 eduicbInteractFilter(edui_interact_s *);
    static i32 eduicbInteractProp(edui_interact_s *);
    void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
    typedef void (*EDBITSPLAYSOUNDCALLBACK)(NUVEC *, i32);
    typedef i32 (*EDBITSREQUESTSOUNDCALLBACK)(char *);

    // GCC 4.7 emits these file-scope objects in reverse declaration order, so
    // each group below mirrors the original linked order in reverse.
    i32 bCameraEnabled = 1;
    i32 edui_filepick_listsize = 8;
    u32 eduiitem_context_outline_colour = 0x0000ff00;
    u32 eduiitem_selected_outline_colour = 0x80ffffff;
    u32 eduimenu_selected_outline_colour = 0x80ffffff;
    u32 eduimenu_outline_colour = 0x80404040;
    f32 edmain_default_camera_speed = 1.0f;
    i32 ed_main_menu_h = 320;
    i32 ed_main_menu_w = 180;
    i32 ed_main_menu_y = 50;
    i32 ed_main_menu_x = 220;
    eduiiattr_s ed_attr = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    i32 eduiPropTextPos = -1;
    i32 edmain_render_interacts;
extern ed_module_s edbridesc;
extern ed_module_s edanimdesc;
extern ed_module_s edptldesc;
extern ed_module_s edgradesc;
extern i32 edpp_create_type;
extern f32 edpp_copy_size;
extern f32 edpp_scale_factor;
extern i32 edptl_repeatboxxzlock;
extern i32 edptl_clipboard_entry;

    eduimenu_s *edui_messagemenu;
extern u32 edblack[4];
extern u32 edgrey[4];
extern u32 eddarkred[4];
    NUMTL *uimtls[5];
    i32 ui_bgmtl;
    i32 ui_outmtl;

extern eduimenu_s * edgra_active_menu;
extern eduimenu_s * edgra_options_menu;
extern eduimenu_s * edgra_instance_menu;
extern eduimenu_s * edgra_changeinstance_menu;
extern eduimenu_s * edgra_clumpproperties_menu;
extern eduimenu_s * edgra_clumpsizes_menu;
extern eduimenu_s * edgra_clumparea_menu;
extern eduimenu_s * edgra_clumpdist_menu;
extern eduimenu_s * edgra_clumpfade_menu;
extern eduimenu_s * edgra_clumpterrain_menu;
extern eduimenu_s * edgra_clumpmode_menu;
extern eduimenu_s * edgra_dpadmode_menu;
extern eduimenu_s * edgra_sscale_menu;
extern eduimenu_s * edgra_globals_menu;
extern i32 edgra_filter;
extern struct numtl_s * edgra_mtl;
extern NUMTL * edgra_mtl_zoff;
extern NUVEC edgra_cam_pos;
extern f32 edgra_cam_dist;
extern i32 edgra_cam_ax;
extern i32 edgra_cam_ay;
extern f32 edgra_size;
extern i32 edgra_nearest;
extern i32 edgra_nearest_instance;
extern i32 edgra_instance_type;
extern i32 edgra_fadeunits_used;
extern i32 edgra_clumps_used;
extern i32 edgra_fadeclumps_used;
extern i32 edgra_clump_size;
extern i32 edgra_pageid;
extern i32 edgra_rotz;
extern i32 edgra_roty;
extern i32 edgra_dpadmode;
extern i32 edgra_editormode;
extern void * edgra_free_vecbuffer;
extern NUMTX * edgra_mtxbuffer;
extern NUVEC * edgra_vecbuffer;
extern NUGSCN * edgra_page_scene[8];
extern void * edgra_page_terrain[8];
extern NUMTX * edgra_page_matrix_stack[8];
extern i32 EDGRA_MAX_CLUMPS;
extern i32 EDGRA_MAX_INDIVIDUAL_CLUMPS;
extern i32 EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP;
extern edgra_clump_s * GrassClumps;
extern edgra_individual_s * IndGrassClumps;
extern i32 * IndGrassClumpsUsed;
extern i32 edgra_ind_clumps_used;

extern i32 edpp_page_used[8];
extern i32 edpp_page_on[8];
extern usize edpp_page_scene[8];
extern i32 edpp_types_used;
extern i32 edpp_copy_source[8];
extern i32 edpp_usememcard;
extern edpp_particle_s edpp_ptls[512];
extern NUVEC edpp_cam_pos;
extern f32 edpp_cam_dist;
extern i32 edpp_cam_ax;
extern i32 edpp_cam_ay;
extern i32 edpp_first_time_this_level;
extern i32 edpp_numparticles;
extern NUMTL * edpp_mtl;
extern NUMTL * edpp_boxmtl;
extern i32 edpp_nearest;
extern i32 edpp_curr;
extern i32 edpp_snap_enabled;
extern u8 edpp_effect_list;
extern i32 edpp_instances_used;
extern i32 edpp_copy_mode;
extern i32 edpp_copy_enclosed;
extern i32 edpp_copy_source_count;
extern NUVEC edpp_copy_source_vec;
extern i32 edpp_num_orphans;
extern i32 edpp_showAllPlaced;
extern i32 edpp_dpad_mode;
extern i32 edpp_readout;
extern eduimenu_s * edpp_active_menu;
extern eduimenu_s * ptltypemenu;
extern eduimenu_s * ptloptmenu;
extern eduimenu_s * ptlgsortmenu;
extern eduimenu_s * ptlgcodemenu;
extern eduimenu_s * ptlemitmenu;
extern eduimenu_s * ptlsizemenu;
extern eduimenu_s * ptlrotmenu;
extern eduimenu_s * ptljibmenu;
extern eduimenu_s * ptlcolmenu;
extern eduimenu_s * ptlemitvelmenu;
extern eduimenu_s * ptlgravmenu;
extern eduimenu_s * ptlvaremitmenu;
extern eduimenu_s * ptlvarstartmenu;
extern eduimenu_s * ptlstartvelmenu;
extern eduimenu_s * ptlcutoffmenu;
extern eduimenu_s * ptldatamenu;
extern eduimenu_s * ptlreadoutmenu;
extern eduimenu_s * memcardmenu;
extern eduimenu_s * messagemenu;
extern eduimenu_s * loadeffectsmenu;
extern eduimenu_s * mergeeffectsmenu;
extern eduimenu_s * deleteeffectsmenu;
extern eduimenu_s * filenamemenu;
extern eduimenu_s * namemenu;
extern eduimenu_s * confirmmenu;
extern eduimenu_s * changegenratemenu;
extern eduimenu_s * totalptlsmenu;
extern eduimenu_s * etimemenu;
extern eduimenu_s * emittimemenu;
extern eduimenu_s * sscalemenu;
extern eduimenu_s * texturemenu;
extern eduimenu_s * textureselectmenu;
extern eduimenu_s * collmenu;
extern eduimenu_s * effectlistmenu;
extern eduimenu_s * ptlclipmenu;
extern eduimenu_s * dpadmodemenu;
extern eduimenu_s * edptl_switch_menu;
extern eduimenu_s * edptl_switchtype_menu;
extern eduimenu_s * edptl_sounds_menu;
extern eduimenu_s * edptl_soundx_menu;
extern eduimenu_s * edptl_soundid_menu;
extern eduimenu_s * edptl_soundcontrol_menu;
extern eduimenu_s * edptl_bounce_menu;
extern eduimenu_s * edptl_group_menu;
extern eduimenu_s * edptl_ghost_menu;
extern eduimenu_s * edptl_star_menu;
extern eduimenu_s * edptl_page_menu;
extern eduimenu_s * edptl_drawflag_menu;
extern eduimenu_s * edptl_scaleeffect_menu;
extern eduimenu_s * edptl_orphanlist_menu;
extern eduimenu_s * edptl_instancesettings_menu;
extern eduimenu_s * edptl_torus_menu;
extern eduimenu_s * edptl_damage_menu;
extern eduimenu_s * edptl_damageflag_menu;
extern eduimenu_s * edptl_repeatbox_menu;
extern eduimenu_s * edptl_quickdel_menu;
extern eduimenu_s * edptl_detail_menu;
extern eduimenu_s * edptl_testdetail_menu;
extern i32 edpp_emitrotz;
extern i32 edpp_emitroty;
extern i32 edpp_emitrotx;
extern i32 edpp_rotz;
extern i32 edpp_roty;
extern f32 edpp_offset;
extern i32 edpp_refrotz;
extern i32 edpp_refroty;
extern i32 edpp_copyrotz;
extern i32 edpp_copyroty;
extern i32 edpp_facrotx;
extern i32 edpp_facroty;

    void edgraInitAllClumps(void);
}












void edbriDoInput(nupad_s *);
void edbriDetermineNearest(f32);
extern "C" {
    extern NUMTL *edbri_mtl, *edbri_mtl_zoff;
    extern eduimenu_s *edbri_options_menu, *edbri_active_menu;
    extern i32 edbri_nearest, edbri_plank_instance_type, edbri_post_instance_type;
    void edbriBridgesReset(void);
}









void edgraClumpReseed(i32 index);

edgra_individual_s *GetIndGrassClump(i32 clump, i32 element);

void edgraClumpDestroy(i32 index);
void edbriBridgeUpdate(i32 bridge, NUGSCN *scene);

static NUVEC *ed_loc;

extern "C" {
    extern debinftype *effecttypes;
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    i32 edbitsLookupSoundFX(char *name);
    void edbitsSoundPlay(NUVEC *position, i32 sound);
    void edanimSoundDestroy(i32 parameter_index, i32 sound_index);
    void AddVariableShotDebrisEffectTimed3(i32, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *);
    extern part_type_s part_types[128];
    extern i32 part_types_used;
    extern i32 part_emits_used;
    extern NUGSCN *part_scene[32];
    extern i32 part_scene_pageid[32];
    void PlatInstBounce(i32, f32, f32, f32);
    void CheckPartCount(void);
    void KillPartsByScene(NUGSCN *);
    extern i32 DEBPAGE_GENERAL;
    extern i32 DEBPAGE_CHARACTER;
    extern i32 DEBPAGE_AREA;
extern part_emit_s part_emits[40];
extern i32 part_page_on[8];
extern i32 part_page_used[8];
extern i32 edpart_instances_used;
extern edanim_param_s AnimParams[64];
extern eduimenu_s * edanim_active_menu;
extern eduimenu_s * edanim_options_menu;
extern eduimenu_s * edanim_particle_menu;
extern eduimenu_s * edanim_particletype_menu;
extern eduimenu_s * edanim_localparticletype_menu;
extern eduimenu_s * edanim_localparticle_menu;
extern eduimenu_s * edanim_switch_menu;
extern eduimenu_s * edanim_switchtype_menu;
extern eduimenu_s * edanim_sound_menu;
extern eduimenu_s * edanim_soundtype_menu;
extern eduimenu_s * edanim_localsoundtype_menu;
extern eduimenu_s * edanim_localsound_menu;
extern eduimenu_s * edanim_bouncy_menu;
extern eduimenu_s * edanim_mctb_menu;
extern NUGSCN * edanim_page_scene[8];
extern i32 edanim_page_used[8];
extern i32 edanim_page_on[8];
extern NUMTL * edanim_mtl;
extern NUMTL * edanim_mtl_zoff;
extern NUVEC edanim_cam_pos;
extern f32 edanim_cam_dist;
extern i32 edanim_cam_ax;
extern i32 edanim_cam_ay;
extern i32 edanim_nearest;
extern i32 edanim_nearest_param_id;
extern i32 edanim_nearest_particle;
extern i32 edanim_nearest_sound;
extern i32 edanim_next_param;
extern i32 edanim_particle_mode;
extern i32 edanim_particle_type;
extern i32 edanim_sound_mode;
extern i32 edanim_sound_type;
extern i32 edanim_params_used;
extern i32 edanim_emitrotz;
extern i32 edanim_emitroty;
}

extern u8 object_switches[128];

extern "C" {
extern u8 edbits_what_game;
extern char edbits_level_filename[256];
extern char edbits_general_save_directory[256];
extern char edbits_general_save_name[256];
extern char edbits_general_save_extension[256];
extern char edbits_level_save_directory[256];
extern char edbits_level_save_name[256];
extern char edbits_level_save_extension[256];
extern char edbits_datapath[256];
extern i32 edSfxAllCount;
extern NUGSCN * edbits_base_scene;
extern NUGSCN * edbits_things_scene;
extern void * edbits_base_terrain;
extern i32 edbits_particle_general_page;
extern i32 edbits_anim_page;
extern i32 edbits_editmode;
extern i32 edbits_part_general_page;
extern NUCAMERA * cubemapcam;
extern i32 edbits_types_error;
extern i32 edbits_duplicates_error;
extern EDBITS_GAME_SOUND edbitsGameSound[320];
extern i32 edbits_numsounds;
extern EDBITSPLAYSOUNDCALLBACK edbitsPlaySound;
extern EDBITSREQUESTSOUNDCALLBACK edbitsRequestSound;
extern i32 edbits_override_backups;
extern eduimenu_s * edbri_active_menu;
extern eduimenu_s * edbri_options_menu;
extern eduimenu_s * edbri_dpadmode_menu;
extern eduimenu_s * edbri_plankinstance_menu;
extern eduimenu_s * edbri_postinstance_menu;
extern eduimenu_s * edbri_plankcount_menu;
extern eduimenu_s * edbri_bridgeproperties_menu;
extern eduimenu_s * edbri_ropecolour_menu;
extern NUMTL * edbri_mtl;
extern NUMTL * edbri_mtl_zoff;
extern NUVEC edbri_cam_pos;
extern f32 edbri_cam_dist;
extern i32 edbri_cam_ax;
extern i32 edbri_cam_ay;
extern i32 edbri_nearest;
extern i32 edbri_plank_instance_type;
extern i32 edbri_post_instance_type;
extern i32 edbri_bridges_used;
extern i32 edbri_page_used[8];
extern i32 edbri_page_on[8];
extern i32 edbri_pageid;
extern i32 edbri_rotz;
extern i32 edbri_roty;
extern i32 edbri_mode;
extern NUGSCN * edbri_page_scene[8];
extern edbridge_s edBridges[64];
extern f32 edbri_length;
extern f32 edbri_width;
extern i32 edbri_planks;
extern i32 edbri_post_interval;
}



void FileLoadSingleEffectType(debinftype *, i32, char);
extern "C" void NuBridgeInit(void);
extern "C" void NuBridgeRemove(i32 bridge);
extern "C" i32 NuBridgeCreate(NUGSCN *, nuhspecial_s *, nuhspecial_s *, NUVEC *, NUVEC *, f32, i16, f32, f32, f32, f32,
                              i32, f32, f32, i32, u32);

void edbriBridgeUpdate(i32 index, NUGSCN *scene);

void edbriBridgePlace(i32 index, NUVEC *position);

i32 edbriBridgeCreate(NUVEC *position);

void edbriBridgeDestroy(i32 index);

void edanimDetermineNearestAnim(f32);
void edppDetermineNearest(float);
void edppPtlShelve(i32);
extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern i32 maxdebkeys;
    void DebFreeOrphansInstantly(debinftype *);
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisOrientation(i32, i16, i16);
    void DebrisEmitterOrientation(i32, i16, i16, i16);
    void DebrisStartOffset(i32, f32);
    void DebrisSetTrigger(i32, i32, i32, f32);
    void DebrisReflectionOrientation(i32, i16, i16, f32, f32);
    void DebrisSetFacing(i32, u8, i16, i16);
    void DebrisSetGroupID(i32, i16);
    void DebrisSetRoomID(i32, nugscn_s *);
}

extern "C" void do_Pad_Standard_camera(edcam_s *camera, f32 delta_time, nupad_s *pad);
extern "C" void do_maya_mouse_camera(edcam_s *camera);
extern "C" void do_mouse_flymode_camera(edcam_s *camera, f32 delta_time);
extern "C" i32 NuKeyboard(i32 key);

static edcam_s gp_cam = {
    {0.0f, 0.0f, 0.0f},
    0,
    0,
    -2.0f,
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 1.0f},
    1,
    1,
    {0.0f, 0.0f, 0.0f},
    0,
    0,
    {0.0015625f, 0.0015625f, 0.0015625f},
    2,
    2,
    0.0078125f,
    0.5f,
    0.2f,
    0.2f,
    4.0f,
    0.15f,
    0.2f,
    0.01f,
    0.1f,
    EDCAM_FREEDOM_POSITION_X | EDCAM_FREEDOM_POSITION_Y | EDCAM_FREEDOM_POSITION_Z | EDCAM_FREEDOM_PITCH |
        EDCAM_FREEDOM_YAW | EDCAM_FREEDOM_DISTANCE,
    {0, 0, 0},
};

void edcamSetContoller(i32 invert_pitch);

static NUCAMERA *edmaincam = NULL;
static NUCAMERA *edinternalcam = NULL;
static eduimenu_s *active_menu;
static eduimenu_s *default_active_menu;
static eduimenu_s *processing_menu;
static f32 edui_cursor_x = 50.0f;
static f32 edui_cursor_y = 50.0f;
static f32 edui_cursor_dx;
static f32 edui_cursor_dy;
static i32 edui_cursor_locked;
static u32 edui_cursor_buttons;
static u32 edui_cursor_buttons_old;
static u32 edui_cursor_buttons_db;
static f32 slider_acceleration;

static i32 numInteracts;
static eduimenu_s *ed_main_menu;
static eduimenu_s *ed_cfg_menu;
static i32 ed_init;
static ed_module_s *ed_list;
static i32 ed_module_active;
static ed_module_s *ed_curr;
static edui_interact_s eduiInteracts[64];
static edui_interact_s *eduiInteractLocked;

extern "C" {
    static void cbMMCancel(eduimenu_s *, eduimenu_s *);
    static void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);
    static void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *, u32);
    static void cbmcfgCancel(eduimenu_s *, eduimenu_s *);
    static void cbMMRegSel(eduimenu_s *, eduiitem_s *, u32);
    static void cbMMEditorConfig(eduimenu_s *, eduiitem_s *, u32);
    static i32 cbInteractMenuCancelChild(edui_interact_s *);
    extern i32 edmain_auto_speed;
    void cbSetAutoSpeed(eduimenu_s *, eduiitem_s *, u32);
}

extern "C" {
    i32 PadFlyMode = 0;
    NUMTX *ed_remap_mtx = NULL;
    i32 edmain_cursor_enabled = 0;

eduimenu_s *edGetMainMenu(void);
void edGraDisableTerrainSwap(void);
void edGraEnableTerrainSwap(void);
void edGraInitTerrainSwapProtection(void);
i32 edanimLoadPage(char *path, NUGSCN *scene);

void edanimClearPage(i32 page);
i32 edanimLookupSpecial(char *name, NUGSCN *scene);
void edanimParamReset(void);
void edanimParticleDestroy(i32 parameter_index, i32 particle_index);
void edanimRegisterCubeDumpInfo(void);
void edanimStartPage(i32 page);
void edanimStopPage(i32 page);
void edanimUpdateObjects(float elapsed);
void edbitsDrawBBox(NUVEC *minimum, NUVEC *maximum, i32 colour, numtl_s *material);
    void edbitsDrawOvalXY(NUVEC *, f32, f32, i32, NUMTL *);
void edbitsDrawCircleTilted(NUVEC *centre, f32 radius, i32 colour, NUMTL *material, i32 rotation_z,
                                i32 rotation_y);
void edbitsDrawCircleXY(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
void edbitsDrawOvalXY(NUVEC *centre, f32 radius_x, f32 radius_z, i32 colour, NUMTL *material);
void edbitsDrawSphere(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
char *edbitsGetSoundName(i32);
i32 edbitsLookupInstance(char *name, NUGSCN *scene);
i32 edbitsLookupSound(char *name);
i32 edbitsLookupSoundFX(char *);
i32 edbitsProcessCubemapDump(void);
void edbitsRegisterDataPath(char *path);
void edbitsRegisterBaseScene(NUGSCN *scene);
extern i32 * edbits_editor_enabled;

void edbitsRegisterEditMode(i32 mode);
void edbitsRegisterEditorEnabledFlag(i32 *enabled);
void edbitsRegisterLevel(char *filename, i32 game);
void edbitsRegisterPlaySound(EDBITSPLAYSOUNDCALLBACK callback);
void edbitsRegisterRequestSound(EDBITSREQUESTSOUNDCALLBACK callback);
void edbitsRegisterSaveFormat(char *general_save_directory, char *general_save_name, char *general_save_extension,
                                  char *level_save_directory, char *level_save_name, char *level_save_extension);
void edbitsRegisterSoundEffect(char *name, i32 id);
void edbitsRegisterThingsScene(NUGSCN *scene);
void edbitsRegisterBaseTerrain(void *terrain);
extern i32 edbitsSfxVol;
void edbitsSetSoundFxVolume(i32 volume);
i32 edbitsStartCubemapDump(void);
void edbitsVector2YZRot(i16 *rotation_y, i16 *rotation_z, f32 x, f32 y, f32 z);
void edbriBridgesReset(void);
    void edbriStopPage(i8 page);
    void NuBridgeRemove(i32 bridge);
void edbriClearPage(i8 page);
void edbriStartPage(i32 page_number);
void edbriStopPage(i8 page);
f32 edcamGetDist(void);
edcam_s *edcamGetEdCam(void);
void edcamGetOffset(NUVEC *offset);
void edcamGetPosAng(NUVEC *position, i32 *pitch, i32 *yaw);
void edcamGetPosAngSnap(NUVEC *position, i32 *pitch, i32 *yaw);
NUVEC *edcamGetPosPointer(void);
void edcamMove(nupad_s *pad);
void edcamMoveEx(nupad_s *pad, f32 delta_time);
void edcamMtx(NUMTX *matrix);
void edcamSet(void);
void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance);
void edcamSetAng(i32 pitch, i32 yaw);
void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale);
void edcamSetDist(f32 distance);
void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement);
void edcamSetOffset(NUVEC *offset);
void edcamSetPos(NUVEC *position);
void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw);
void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance);
void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z);
i32 edgraBufferUsage(void);
i32 edgraLoadPage(char *path, void *gscn, i32 terrain, void *buf, void *buf_end);
void edgraClearPage(i8 page);
void edgraClumpsReset(void);
void edgraSetMemoryBuffer(VARIPTR start, VARIPTR end);
void edgraSetThinning(i32 clumps, i32 elements);
    void NuWindSetup(VARIPTR *, VARIPTR, i32, i32);
void edgraSetup(VARIPTR *buffer, VARIPTR end, i32 clumps, i32 individual_clumps, i32 units_per_clump);
i32 edmainActivate(ed_module_s *module, i32 notify);
void edmainClose(void);
ed_module_s *edmainCurrent(void);
void edmainExtCamera(NUCAMERA *camera);
NUCAMERA *edmainGetCamera(void);
i32 edmainGetCursorEnabled(void);
    // Shared editor font; edmainInit receives and stores the font handle.
    void *ed_fnt;

void edmainInit(void *font, char *configuration_file);
void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height);
i32 edmainProcess(f32 delta_time, nupad_s *pad);
NUVEC *edmainQueryLocVec(void);
i32 edmainRegister(ed_module_s *module);
void edmainRegisterLocVec(NUVEC *position);
void edmainRender(void);
void edmainSetCamera(NUMTX *matrix);
void edmainSetCursorEnabled(i32 enabled);
f32 edmainSetMainMenuScale(f32 scale);
void edmainSetReturn(i32 result);
void edpartClearPage(i8 page);
void edpartDestroyAllParticles(void);
void edpartParticleReset(void);
void edpartRegisterPointerToGameCharLocation(NUVEC *position);
void edppClearPage(i8 page);
void edppDeleteEffect(i32 index);
void edppDestroyAllEffects(void);
void edppDestroyAllParticles(void);
    extern f32 globaltime;

void edppDrawSpheres(debinftype *effect, i32 key_index);
void edppDrawTorus(debinftype *effect, i32 key_index);
i32 edppFindAllSounds(i32 page, NUVEC *positions, i32 (*sounds)[4], i32 capacity, i32 skip);
i32 edppLoadPage(char *path, i32 flag, usize scene);
void edppRegisterPointerToGameCharLocation(NUVEC *position);
void edppRestartAllEffectsInLevel(void);
void edppSetSaveName(u32 page, char *name);
void edppStopPage(i32 page_id);
extern i32 edqseed;
void edqrand(void);
edui_textpicker_s *eduiAddTextPickEntEx(eduimenu_s *menu, eduiitem_s *source, EdUiItemCallback callback);
void eduiAddPropTextPickEnt(eduimenu_s *menu, eduiitem_s *source);
void eduiAddTextPickEnt(eduimenu_s *menu, eduiitem_s *source);
i32 eduiClearActiveMenu(void);
i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad);
void eduiCreate3LineMessageMenu(eduimenu_s *parent, char *first, char *second, char *third, i32 first_highlight,
                                    i32 second_highlight, i32 third_highlight);
void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
i32 eduiCursorOverMenu(eduimenu_s *menu);
i32 cbInteractMenuTitle(edui_interact_s *interact);
i32 cbInteractMenuScrollUp(edui_interact_s *interact);
i32 cbInteractMenuScrollDown(edui_interact_s *interact);
void cbInteractMenuScrollTo(eduimenu_s *menu, char *text);
void cbInteractMenuKeySelect(eduimenu_s *menu);
void eduiFlushInteracts(void);
eduimenu_s *eduiGetActiveMenu(void);
eduimenu_s *eduiGetActiveMenuParent(void);
f32 eduiGetAnalougePadValue(nupad_s *pad);
i32 eduiGetCameraEnabled(void);
void eduiGetCursorCoords(f32 *x, f32 *y);
void eduiGetCursorDelta(f32 *x, f32 *y);
eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu);
    i32 bUsingMenuFocus;
i32 eduiGetUsingMenuFocus(void);
i32 eduiGradPickRead(eduiitem_s *item, edui_gradient_stage_s *stages, i32 capacity);
edui_gradient_node_s *eduiGradStageAdd(edui_gradient_pick_s *item, f32 time, f32 hue, f32 saturation, f32 value);
edui_gradient_node_s *eduiGradStageAddRGB(edui_gradient_pick_s *item, f32 time, f32 red, f32 green, f32 blue);
void eduiGradStageDelete(edui_gradient_pick_s *item, edui_gradient_node_s *stage);
void eduiGradStageSetHSV(edui_gradient_node_s *stage, f32 hue, f32 saturation, f32 value);
void eduiGradStageSetRGB(edui_gradient_node_s *stage, f32 red, f32 green, f32 blue);
void eduiIitemExpanderSetDepth(edui_expander_s *item, i32 depth);
void eduiInit(void);
void eduiInitMaterials(void);
eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text);
eduiitem_s *eduiItemColourPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemColourPickSetHSV(edui_colour_pick_s *item, f32 hue, f32 saturation, f32 value);
void eduiItemColourPickSetRGB(edui_colour_pick_s *item, f32 red, f32 green, f32 blue);
eduiitem_s *eduiItemColourSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           f32 minimum, f32 range, f32 value, u8 red, u8 green, u8 blue, char *text);
eduiitem_s *eduiItemDataGradPickCreate(usize data, const void *colours, EdUiItemCallback callback,
                                           EdUiItemCallback press, EdUiItemCallback add, EdUiItemCallback remove,
                                           EdUiItemCallback copy, EdUiItemCallback paste, char *text);
void eduiItemExpanderAddChild(edui_expander_s *item, eduiitem_s *child);
eduiitem_s *eduiItemExpanderCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemFilePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemFilePickSetFmt(edui_file_pick_s *item, char *format);
void eduiItemFilterAddItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemFilterCreate(usize data, const void *colours, char *text, char *value);
void eduiItemFilterRemoveItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph);
eduiitem_s *eduiItemGraphCreate(usize data, const void *colours, EdUiItemCallback callback, nugraph_s *graph,
                                    i32 width, i32 height);
void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y);
void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title);
eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemGreyPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemNumberCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);
eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value);
eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data);
i32 eduiItemPropSetText(edui_prop_s *item, char *text);
i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected);
eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text);
eduiitem_s *eduiItemSelWithClipColourCreate(usize data, const void *colours, i32 selected, i32 group,
                                                EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemSeparatorCreate(usize data, const void *colours);
i32 eduiItemSetText(eduiitem_s *item, char *text);
eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);
eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 range, i32 value, char *text);
void eduiItemSliderSetFmt(edui_slider_s *item, char *format);
void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity);
void eduiItemSliderSetVal(edui_slider_s *item, f32 value);
void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify);
eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemTextPickSetFmt(edui_textpicker_s *item, char *format);
eduiitem_s *eduiItemTextSelectorCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           i32 count, i32 selected, char *text, char **options);
eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text);
    eduiitem_s *edui_last_item;
eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after);
void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before);
void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child);
eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback,
                               char *title);
void eduiMenuDestroy(eduimenu_s *menu);
i32 eduiMenuDetach(eduimenu_s *menu);
void eduiMenuEnsureSelection(eduimenu_s *menu);
void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding);
void eduiMenuFitWidth(eduimenu_s *menu, i32 padding);
void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuIsActive(eduimenu_s *menu);
i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result);
i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuRender(eduimenu_s *menu);
void eduiMenuSelectFirstEntry(eduimenu_s *menu);
void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours);
void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled);
void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours);
void eduiMenuSortItemsByTxt(eduimenu_s *menu);
void eduiProcessCursor(f32 delta_time, nupad_s *pad);
void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad);
i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad);
void eduiRenderCursor(void);
void eduiRenderInteracts(void);
void eduiSetActiveMenu(eduimenu_s *menu);
void eduiSetCameraEnabled(i32 enabled);
void eduiSetCursorColour(u32 colour);
void eduiSetCursorCoords(f32 x, f32 y);
void eduiSetDefaultActiveMenu(eduimenu_s *menu);
void eduiSetFont(NUQFNT *font);
void eduiSetFontScale(f32 x, f32 y);
void CreateTestMenu(void *font);
void TestMenu(nupad_s *pad);
void eduiSetGlobalSliderAccel(f32 acceleration);
void eduiSetRenderPlane(i8 plane);
void eduiSetUsingMenuFocus(i32 enabled);
void eduiShowCursor(i32 show);
i32 eduiUsedAlgPad(nupad_s *pad);
void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
i32 eduicbInteractSlider(edui_interact_s *interact);
void eduicbItemDestroy(eduimenu_s *menu, eduiitem_s *item);
void eduicbItemDestroyProp(eduimenu_s *menu, eduiitem_s *item);

    // The editor UI callbacks share this translation unit with their cursor,
    // menu, and interaction state in the original binary.

static __used__ void cbMMCancel(eduimenu_s *, eduimenu_s *);

static __used__ i32 cbInteractMenuCancelChild(edui_interact_s *interact);

static __used__ void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);

static __used__ void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *item, u32);

static __used__ void cbmcfgCancel(eduimenu_s *menu, eduimenu_s *);

static __used__ void cbMMRegSel(eduimenu_s *, eduiitem_s *selected, u32);
static __used__ void cbMMEditorConfig(eduimenu_s *menu, eduiitem_s *, u32);

static void eduiRenderGraphLine(nugraph_s *graph, i32 x, i32 y, i32 width, i32 height, u32 colour);

static __used__ void eduicbDestroyDirectoryList(eduimenu_s *menu, eduimenu_s *);

static void eduicbAttachDirectoryList(eduimenu_s *menu, edui_file_pick_s *picker);

static __used__ i32 eduicbInteractColourPick(edui_interact_s *interact);
static __used__ i32 eduicbInteractExpander(edui_interact_s *interact);
static __used__ i32 eduicbInteractProp(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractFilter(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractSel(edui_interact_s *interact);
static __used__ void eduicbItemDestroyExpander(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemDestroyFilter(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemFilePickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *item);
static __used__ void eduicbItemSliderDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemTextPickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbPickCFGCancel(eduimenu_s *menu, eduimenu_s *);

    static inline i32 eduiPickerDeadZone32(i32 raw_value) {
        i32 value = raw_value - 128;
        if (value > 0) {
            if (value < 32)
                return 0;
            return (value - 32) * 255 / 223;
        }
        if (value > -32)
            return 0;
        return (value + 32) * 255 / 223;
    }

static __used__ i32 eduicbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessColourSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessExpander(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilePick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilter(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGradPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGraph(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessGreyPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static i32 eduicbProcessPropKeyboard(eduimenu_s *menu, edui_prop_s *property);
static __used__ i32 eduicbProcessProp(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessSel(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
static __used__ i32 eduicbProcessSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTextPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTexturePick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
    // Retail keeps this as the constant-propagated clone of the generic four-step
    // gradient helper; only the hue/value colour picker band reaches it.
    static void eduiDrawHueValueBand(i32 x, i32 y, i32 width, i32 height, f32 start, f32 end, u32 first, u32 second)
        __asm__("_ZL19eduiDrawGrad4StepsViiiiffiiiii.constprop.59");
    static i32 edui_grad_steps = 8;
static void eduiDrawHueValueBand(i32 x, i32 y, i32 width, i32 height, f32 start, f32 end, u32 first, u32 second);
static __used__ i32 eduicbRenderColourPick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderExpander(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGraph(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32 selected);
static __used__ i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderProp(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilter(struct eduimenu_s *menu, struct eduiitem_s *item, i32 x, i32 y, i32 scale,
                                           i32 selected);
static __used__ i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSelWithClipColour(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width,
                                                      i32);
static __used__ i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTexturePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ void eduicbSelectDirectoryEntry(eduimenu_s *menu, eduiitem_s *item, u32 value);
static __used__ void eduicbSelectDirectoryExit(eduimenu_s *menu, eduiitem_s *, u32);
}

static void eduiFntPrintExLeft(void *font, int x, int y, int, char *format, ...);

static void eduiFntPrintExCentre(void *font, int x, int y, int, char *format, ...);

static void eduiFntPrintExEditBuffer(void *font, float x, float y, int, char *format, ...);

// Text-picker commit callbacks. Retail keeps them unmangled and local.
extern "C" {
__attribute__((visibility("hidden"), optimize("O3"))) void tpentOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                            unsigned int buttons);

__attribute__((visibility("hidden"), optimize("O3"))) void tpentPropOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                                unsigned int buttons);
}

// ==== src/legoapi/menus/screens/gamemenuall.cpp: declarations ====
#include "decomp.h"
#include "gameapi/edtools/edui.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/gui/apimenu_internal.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/config/fileselect.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/panel.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern MENUPACKET_s MenuPacket;
extern i32 NextArea_FreePlay;
void InitMission(MISSIONSYS *, i32);
extern f32 ICONSIZE, ICONX, DROPINALPHA, HUB_EPISODETITLEY;
extern i16 tSELECT, tSELECTED, tSELECTING, tEXIT, tCANCEL;
void DrawCharIcon(i32, f32, f32, f32, f32, i32, f32, f32, i32, nuhspecial_s *);
void Hub_DrawImportantBrick(i32, f32, f32, f32, i32, i32);
f32 GetAspectRatio();
extern "C" void BackupMenu(void);
extern "C" void BackupMenuNoFn(void);
extern "C" void PlaySfxById(i32 sfx_id, nuvec_s *position);
extern "C" void SmartText(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                          u8 green, u8 blue, f32 max_width, i32 max_lines);
extern "C" void NuIOS_RecordFlurryEvent(char *event_name);
extern "C" void DrawMenuButtonPrompts(i32 confirm_prompt, i32 cancel_prompt, i32 enabled, u8 red, u8 green, u8 blue,
                                      u8 alpha);
extern "C" void DrawMenuButtonPromptsEx(i32 confirm_prompt, i32 cancel_prompt, i32 flags, i32 enabled, u8 red, u8 green,
                                        u8 blue, u8 alpha);
extern i32 SAVESLOTS;
extern i32 MenuSFX;
extern i32 MENUSFX_MENUSELECT;
extern i32 MENUSFX_MENUBACK;
extern i32 MENUSFX_MENUNOENTRY;
extern i32 MENUSFX_MENUMOVE;
extern char *apitxt_LOADGAME;
extern char *apitxt_SAVEGAME;
extern char *apitxt_CONFIRMSAVE;
extern char *apitxt_CONFIRMOVERWRITE;
extern char *apitxt_CONFIRMLOAD;
extern char *apitxt_DOYOUWANTTOABORT;
extern char *apitxt_DOYOUWANTTOABORTLOAD;
extern char *apitxt_DOYOUWANTTOABORTFORMAT;
extern char *apitxt_CONFIRMDELETE;
extern char *apitxt_RETRY;
extern char *apitxt_SLOT;
extern char *apitxt_NEWSAVE;
extern char *apitxt_CANCEL;
extern char *apitxt_NODATAAVAILABLE;
extern char *apitxt_NOTENOUGHSPACE;
extern char *apitxt_DELETEGAME;
extern char *apitxt_DELETING;
extern char *apitxt_DELETECOMPLETE;
extern char *apitxt_FORMATTING;
extern char *apitxt_FORMATTINGCOMPLETE;
extern char *apiGameName;
extern char *apitxt_YES;
extern char *apitxt_NO;
extern char **TTab;
extern i16 tSELECTCONTROLS;
extern i16 tCONTROLSCANBECHANGED;
extern i16 tCLASSIC;
extern i16 tTOUCHSCREEN;
extern i16 tCONTROLLERCONNECTED;
extern i16 tCONTROLS;
extern i16 tCONSOLE;
extern i16 tTOUCH;
extern i16 tSURROUNDSOUND;
extern i16 tAUDIOVOLUME;
extern i16 tMUSIC;
extern i16 tWIDESCREEN;
extern i16 tON;
extern i16 tOFF;
extern i16 tACCEPT;
extern i16 tBACK;
extern i16 tCONTINUE;
extern i16 tCANCEL;
extern i16 tSTORY;
extern i16 tFREEPLAY;
extern i16 tREPLAYSTORY;
extern i16 tLOCKED;
extern i16 tAVAILABLETOBUY;
extern i16 tPOWERBRICK;
extern i16 tHOWTOPLAY;
extern "C" OPTIONSSAVE TempOptions;
extern i32 GAMEDEMO;
extern i32 menu_flash;
extern i16 tOUTOFTIME;
extern f32 minikittime;
extern "C" TIMER BonusTimer;
extern f32 text3d_height;
extern f32 text3d_width;
extern i32 Paused;
extern f32 PauseMenus_X;
extern i32 PauseMenus_Align;
void GameDrawMenuEntry(MENU *menu, char *text);
i32 GetParentMenuID(void);
f32 GameSetSoundVolume(OPTIONSSAVE *options);
f32 GameSetMusicVolume(OPTIONSSAVE *options);
void SfxCheckMusicOnOff(OPTIONSSAVE *options);
void NewGame(void);
void NuIOS_RestoreInAppPurchases(void);
extern i32 startnewgame;
extern i32 startnewgame_initiated;
extern f32 newgamecamtime;
extern i32 newgamecam;
extern "C" u8 MENUNORMALR;
extern "C" u8 MENUNORMALG;
extern "C" u8 MENUNORMALB;
extern "C" f32 sfx_wait;
extern "C" f32 MENUBOTY;
void Draw_AUTOSAVEWARNING();
void Draw_NODATAAVAILABLE();
void Draw_NOMEMORYCARD();
extern "C" void Draw_NOTENOUGHSPACE(void);
extern "C" void Draw_SPACENEEDED(void);
extern "C" void Draw_CHECKINGMEMORYCARD(void);
extern "C" void Draw_DONOTREMOVEMEMORYCARD(void);
void Draw_OK(MENU_s *menu);

extern i32 memcard_cardchanged;
extern i32 MenuASCancelFinished;
extern i32 MenuCardWarningState;
extern i32 ButtonScaleMode;
extern i32 Menu_InLoadFlow;
extern i32 Menu_InWarningFlow;
extern i32 Menu_LastFlow;
extern i32 MenuLoadStarted;
extern i32 memcard_slot;
extern i32 Menu_InSaveFlow;
extern i32 Menu_DisableCancel;
extern i32 Menu_InHeaderSave;
extern i32 Menu_Overwriteflag;
extern i32 memcard_slotsused;
extern i32 memcard_saveneeded;
extern i32 memcard_savestarted;
extern i32 memcard_savefailed;
extern i32 memcard_loadneeded;
extern i32 memcard_loadstarted;
extern i32 memcard_loadfailed;
extern i32 memcard_loadcorrupt;
extern i32 memcard_deleteneeded;
extern i32 memcard_deletestarted;
extern i32 memcard_deletefailed;
extern f32 memcard_deletemessage_delay;
extern f32 memcard_deleteresult_delay;
extern i32 memcard_formatting;
extern i32 memcard_formatme;
extern i32 memcard_formatfailed;
extern f32 memcard_formatmessage_delay;
extern f32 memcard_formatresult_delay;
extern f32 memcard_savemessage_delay;
extern f32 memcard_saveresult_delay;
extern f32 memcard_loadmessage_delay;
extern f32 memcard_loadresult_delay;
extern i32 SAVESIZE;

extern i32 memcard_autosave;
extern "C" i32 memcard_autosaveenabled;
extern "C" i32 memcard_autosavedisabled;
// Retail clears these "fast repeat" latches but never sets them.
extern void (*drawslotinfofn)(f32, f32, i32, i32);
extern void (*drawslotsfn)(MENU *, f32);

extern char * extrasavetext;
extern f32 extrasavealpha;
extern u8 RAP_WARNING_B;
extern u8 RAP_WARNING_G;
extern u8 RAP_WARNING_R;


extern i32 hub_new_level;
extern i32 hub_selectmode;
extern f32 selectmodetime;
extern i32 selectmodemode;
extern "C" i32 FinishLoop_On;
extern FadeSystem FadeSys;
void Hub_DrawFreePlaySelect();
void Hub_InitFreePlaySelect(i32 area, i32 first_model, i32 second_model);
void Hub_UpdateFreePlaySelect();
void WipeBackToHub();
void NewLevelFromMenu(LEVELDATA_s *level, i32 menu_id, i32 menu_y, i32 remember_hub);




void APIMenuDrawMemCardSlots(MENU *menu, f32 y);

void MenuDrawLoad(MENU_s *menu);

void MenuDrawSave(MENU_s *menu);


void MenuExitLoad(MENU_s *);

void MenuExitSave(MENU_s *);

void MenuDrawClips(MENU_s *);

void MenuEnterLoad(MENU_s *menu);

void MenuEnterSave(MENU_s *menu);

void MenuInitClips(MENU_s *);

void MenuStartLoad();

void MenuStartSave();




void MenuDrawNoData(MENU_s *menu);

void MenuDrawSaving(MENU_s *);

void MenuUpdateLoad(MENU_s *menu);

void MenuUpdateSave(MENU_s *menu);




void IconScenes_Dump();

void IconScenes_Init(char *path, variptr_u *buf, variptr_u *);

void IconScenes_Load(APICHARACTERMODELLIST_s *list, i32, variptr_u *buf, variptr_u *buf_end);

void MenuDrawLoading(MENU_s *);




i32 MenuIsAvailable();

void MenuUpdateClips(MENU_s *);

void MenuDrawDeleting(MENU_s *);

void MenuDrawFreePlay(MENU_s *);



void MenuInitFreePlay(MENU_s *menu);



void MenuUpdateNoData(MENU_s *menu);

void MenuUpdateLoading(MENU_s *);

void MenuUpdateSaving(MENU_s *);



NUGSCN *IconScene_FindById(i32 character_id);


void MenuDrawFormatting(MENU_s *);

void MenuDrawInsertCard(MENU_s *);

void MenuDrawLoadCancel(MENU_s *menu);

void MenuDrawSaveCancel(MENU_s *menu);

void MenuUpdateDeleting(MENU_s *);

void MenuUpdateFreePlay(MENU_s *);


void MenuDrawCardWarning(MENU_s *);

void MenuDrawFileCorrupt(MENU_s *);

void MenuDrawLoadConfirm(MENU_s *menu);

void MenuDrawSaveConfirm(MENU_s *menu);

void MenuEnterHeaderSave(MENU_s *);

void MenuEnterInsertCard(MENU_s *);

void MenuExitCardWarning(MENU_s *);

void MenuDrawEndChallenge(MENU_s *);

void MenuDrawFormatCancel(MENU_s *menu);

void MenuDrawNoMemoryCard(MENU_s *menu);

void MenuEnterCardWarning(MENU_s *);

void MenuEnterSaveConfirm(MENU_s *);


void MenuUpdateFormatting(MENU_s *);

void MenuUpdateLoadCancel(MENU_s *menu);

void MenuUpdateSaveCancel(MENU_s *menu);

void MenuUpdateInsertCard(MENU_s *menu);

void MenuDrawDeleteConfirm(MENU_s *menu);

void MenuDrawFormatConfirm(MENU_s *);

void MenuInitialiseEx(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                      void (*draw_save_slots_info_fn)(f32, f32, i32, i32), i32 is_fade_enabled, i32 is_shadow_enabled);

void MenuInitialise(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                    void (*draw_save_slots_fn)(MENU *, f32), i32 is_fade_enabled, i32 is_shadow_enabled);

void MenuEnterNoMemoryCard(MENU_s *);

void MenuEnterStartNewGame(MENU_s *);

void MenuUpdateCardWarning(MENU_s *menu);

void MenuUpdateFileCorrupt(MENU_s *menu);

void MenuUpdateLoadConfirm(MENU_s *menu);

void MenuUpdateSaveConfirm(MENU_s *menu);

void MenuDrawAutoSaveCancel(MENU_s *menu);

void MenuDrawNotEnoughSpace(MENU_s *);



void MenuUpdateEndChallenge(MENU_s *menu);

void MenuUpdateFormatCancel(MENU_s *menu);

void MenuUpdateNoMemoryCard(MENU_s *menu);

void MenuDrawAutoSaveWarning(MENU_s *menu);

void MenuDrawDoNotRemoveCard(MENU_s *);

void MenuEnterAutoSaveCancel(MENU_s *menu);

void MenuUpdateDeleteConfirm(MENU_s *menu);

void MenuUpdateFormatConfirm(MENU_s *menu);

void MenuEnterAutoSaveWarning(MENU_s *);

extern f32 memcard_autosavecanceldelay;
void MenuUpdateAutoSaveCancel(MENU_s *menu);

void MenuUpdateNotEnoughSpace(MENU_s *menu);



void MenuUpdateAutoSaveWarning(MENU_s *menu);

void MenuUpdateDoNotRemoveCard(MENU_s *menu);

extern "C" {

extern f32 MENUDY;
extern f32 MENUBOTY;
extern f32 MENUTOPY;
extern f32 dme_sx;
extern f32 dme_sy;
extern f32 MENUENTRYEXWIDTH;
extern u8 MENUFLASH1R;
extern u8 MENUFLASH1G;
extern u8 MENUFLASH1B;
extern u8 MENUFLASH0R;
extern u8 MENUFLASH0G;
extern u8 MENUFLASH0B;
extern u8 MENUWARNR;
extern u8 MENUWARNG;
extern u8 MENUWARNB;
extern u8 MENUENTRYR;
extern u8 MENUENTRYG;
extern u8 MENUENTRYB;
extern u8 MENUNORMALR;
extern u8 MENUNORMALG;
extern u8 MENUNORMALB;
extern u8 dme_r;
extern u8 dme_g;
extern u8 dme_b;
extern i32 dme_align;
extern i32 dme_rgb;
extern f32 menu_pulse;
extern f32 menu_pulse_speed;
extern i32 menu_pulse_angle;
extern f32 menu_pulsate;
extern f32 menu_pulsate_speed;
extern i32 menu_pulsate_angle;
extern void (*headerdrawfn)();
extern void (*predrawfn)(MENU *);


void MenuRememberCursor(MENU *menu);

void BackupMenu(void);

void BackupMenuNoFn(void);

void DrawMenu(i32 paused);

void DrawMenuBottomMessage(char *text, u8 red, u8 green, u8 blue);

void DrawMenuButtonPrompts(i32 confirm_prompt, i32 cancel_prompt, i32 enabled, u8 red, u8 green, u8 blue,
                               u8 alpha);

void DrawMenuButtonPromptsEx(i32, i32, i32, i32, u8, u8, u8, u8);

void DrawMenuEntry(MENU *menu, char *text);

void DrawMenuEntryEx(MENU *menu, char *text, i32 alpha);

void DrawMenuHeader(void);

void DrawMenuHeaderMessage(char *text, u8 red, u8 green, u8 blue);

void DrawMenuTopMessage(char *text, u8 red, u8 green, u8 blue);

void Draw_CANCEL(MENU *menu);

void Draw_CHECKINGMEMORYCARD(void);

void Draw_DONOTREMOVEMEMORYCARD(void);

void Draw_NOTENOUGHSPACE(void);

void Draw_SPACENEEDED(void);

void FlushMenuHighlights(eduimenu_s *menu);

i32 MenuCurrentID(void);

void MenuDrawBackground(void);

i32 MenuGetSlotNum(void);

i32 MenuInCriticalMemoryCard(void);

i32 MenuInMemoryCard(void);

i32 MenuInMemoryCardLoad(void);

i32 MenuInMemoryCardWarning(void);

void MenuRegisterSoundFX(i32 move, i32 select, i32 back, i32 no_entry);

void MenuRepeat(i32 *held, i32 *repeat, f32 *repeat_time, u8 *repeat_count, f32 interval, f32 elapsed);

void MenuSetColours(u8 normal_r, u8 normal_g, u8 normal_b, u8 entry_r, u8 entry_g, u8 entry_b, u8 header_r,
                        u8 header_g, u8 header_b, u8 warn_r, u8 warn_g, u8 warn_b, u8 flash0_r, u8 flash0_g,
                        u8 flash0_b, u8 flash1_r, u8 flash1_g, u8 flash1_b);

void MenuSetPulsateSpeed(f32 speed);

void MenuSetTopBottom(f32 top, f32 bottom);

void MenuSetPreDrawFn(void (*draw_fn)(MENU *));

void MenuSetHeaderDrawFn(void (*draw_fn)(void));

void MenuReset(void);

void RemapAddr(void *new_base, void *old_base, void **address);

void SetButtonScaleMode(i32 mode);

i32 UpdateMenu(u32 primary_held, u32 primary_pressed, u32 alternate_held, u32 alternate_pressed, f32 elapsed,
                   u32 confirm_mask, u32 cancel_mask, u32 start_mask, u32 select_mask);

void cbCancelSubMenu(eduimenu_s *menu, eduimenu_s *);

void cbCancelSubMenuFromItem(eduimenu_s *menu, eduiitem_s *, u32);

i32 cbCompateDirentByDateAsc(const void *first, const void *second);

i32 cbCompateDirentByDateDec(const void *first, const void *second);

i32 cbCompateDirentByNameAsc(const void *first, const void *second);

i32 cbCompateDirentByNameDec(const void *first, const void *second);

i32 cbCompateDirentBySizeAsc(const void *first, const void *second);

i32 cbCompateDirentBySizeDec(const void *first, const void *second);

void cbTriggerSubMenu(eduimenu_s *menu, eduiitem_s *item, u32);

} // extern "C"

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
// Temporary C-linkage compatibility surface for unresolved engine owners.
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
extern "C" {
extern NULGTARCMATERIAL NuLgtArcMtl[4];
extern i32 NuLgtLaserOldCnt;
}
#include "nu2api/nucore/nuonline.h"
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuqfnt.h"

void NuErrorPrint(char *);
void NuDebugMsgPrint(char *);
extern NUQFNT *system_qfont;
void NuLgtArcLaserEx(i32 type, NUVEC *start, NUVEC *end, NUVEC *control, f32 width, f32 segment_length, f32 wobble,
                     f32 duration, i32 colour, i32 flags);

#include "decomp.h"
#include "java/android.h"
#include "java/java.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/NuInputManager.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nuscreen.hpp"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/numath/numtx.h"
#include "globals.h"

extern NuInputManager *inputManager;
extern bool used_touch_IDs[10];

struct ani3_animheader_s;
struct nuanimdatachunk_s;
nuanimdatachunk_s *NuAnimDataChunkCreate(i32 curve_set_count);

extern "C" {

extern u8 CutSceneBoundingBoxTrackRoot;
extern i32 NuGCutAudioStream;
extern NUANIMBUFFEVALUATECB AnimBuffEvalCB;
extern void ** AnimBuffEvalData;
extern i32 * AnimBuffEvalJoint;
extern GLuint g_colorRenderbuffer;
extern GLuint g_depthRenderbuffer;
}


using NUHGOBJVIDEOMEMFN = void (*)(nuhgobj_s *);

extern NUHGOBJVIDEOMEMFN hgobj_to_video_mem;
extern NUHGOBJVIDEOMEMFN video_mem_to_hgobj;

extern "C" {
    extern NUGLOBALRNDRSTATE render_state;
    void RndrStateSetConstAlphaTint(i32 alpha_enabled, i32 tint_enabled, f32 alpha, const NUCOLOUR3 *tint, NUMTL *mtl);
}

// C++-linkage helpers defined in sibling TUs.
void NuPadRecordEndFrame(void);                      // nupad_interface.cpp
void bgSuspendMain(i32);                             // main.cpp
void NuAnimBuffInit(i32, VARIPTR *, VARIPTR);        // nu2api_nucore_misc.cpp
i32 NuCameraClipHGobj(nugscn_s *, NUMTX *, NUMTX *); // nu2api_nucore_misc.cpp
i32 findrange(nugscn_s *, i32);                      // utilities.cpp

extern "C" {

    // ---------------------------------------------------------------------------
    // Display-list bootstrap (original nucore TU file-statics)
    // ---------------------------------------------------------------------------

    i32 NuHasError(void);
    void NuMtlAnimate(f32 frame_time);
    void NuTexAnimProcess(f32 frame_time);
    void NuShaderManagerSetfv(i32 semantic, const f32 *values);

    // ---------------------------------------------------------------------------
    // Redirected symbols — real bodies live elsewhere (kept as comments)
    // ---------------------------------------------------------------------------

    // Nu360_dxClear(u32,u32) is implemented in ios_graphics.cpp (original 0x317070).
    // NuDisplayListSwapBuffersBeginFrame / EndFrame are transcribed in full
    // in nudlist.cpp (originals 0x2eb5d0 / 0x2eaef0).
    // NuHtmlBegin(void*) is transcribed in legoapi/misc/supportall.cpp (original 0x2d5ca0).
    // NuIOS_SetVertexFormat is transcribed in nuiosdl_gl.cpp (original 0x29c070).
    // NuIOS_WaitForRenderThreadCompletion is transcribed in ios_graphics.cpp (original 0xe34b0).
    // NuIOS_WakeRenderThread is transcribed in ios_graphics.cpp (original 0xe3590).
    // NuRenderContextSetZFunc is transcribed below (original 0x2a3860).

    // ---------------------------------------------------------------------------
    // Camera (thin wrappers; full matrix work is in NuCameraSetEx)
    // ---------------------------------------------------------------------------

f32 NuCameraCalcAperture(f32 focal_length, f32 root_fstop);
void NuCameraCalcClipMtx(NUCAMERACLIP *clip, NUCAMERA *camera, i32 use_cached_projection);
f32 NuCameraCalcRootFStop(f32 focal_length, f32 aperture);
void NuCameraClearStateBuffer(void);
i32 NuCameraClipTestExtentsGeneric(NUVEC *min, NUVEC *max, NUMTX *world_mtx, f32 far_clip, i32 flags,
                                       f32 *min_depth);
i32 NuCameraClipTestPointScissor(NUVEC *point);
i32 NuCameraClipTestPointVport(NUVEC *point);
i32 NuCameraClipTestPoints(NUVEC *points, i32 count, NUMTX *world_mtx);
f32 NuCameraFOVToFocalLen(f32 fov);
f32 NuCameraFocalLenToFOV(f32 focal_length);
void NuCameraForceFarclip(i32 enabled);
void NuCameraGetAxes(NUVEC *axes);
NUMTX *NuCameraGetClippingMtx(void);
void NuCameraGetClippingRatios(f32 *horizontal, f32 *vertical);
NUMTX *NuCameraGetPCMtx(void);
NUMTX *NuCameraGetPCSMtx(void);
NUMTX *NuCameraGetVPCSMtx(void);
i32 NuCameraIntersectsAABB(NUVEC *center, NUVEC *extent, f32 far_clip, i32 use_scissor);
extern i32 prev_lock;
extern NUCAMERA locked_camera;
extern NUCAMERA cam_copy;
extern i32 camfx;
extern NuCameraReflect global_reflect;
    DECOMP_ASSERT(sizeof(NuCameraReflect) == 0x84, "camera reflection state size");
    DECOMP_ASSERT(__builtin_offsetof(NuCameraReflect, mtx) == 4, "camera reflection matrix offset");

void NuCameraLock(i32 lock);
    extern nurenderscene_s currentScene;
extern i32 NuRndrDoingScreenGrab;
extern i32 motionBlurAccumActiveThisFrame;

void NuCameraMotionBlurEffect(const NUMTX *previous, const NUMTX *current, f32 scale, f32 maximum, f32 falloff);
void NuCameraMotionBlurParams(const NUMTX *current);
void NuCameraRelock(void);
i32 NuCameraSaveState(void);
void NuCameraSet(NUCAMERA *cam);
void NuCameraSetAxes(NUVEC *axes);
    extern i32 PS2_REZ_W;
    extern i32 PS2_REZ_H;
extern i32 PS2_SREZ_W;
extern i32 PS2_SREZ_H;
void NuCameraSetEx(NUCAMERA *cam, i32 fast);
void NuCameraSetReflect(NUCAMERA *camera, NuCameraReflect *reflect);
void NuCameraSetScissorClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraSetVPortClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraTransformScreen(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenClip(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenVU0(NUVEC4 *screen, NUVEC4 *world, i32 count, NUMTX *matrix);
void NuCameraTransformView(NUVEC *view, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraUnlock(void);

    // ---------------------------------------------------------------------------
    // Display list — faithful helpers + pending stubs
    // ---------------------------------------------------------------------------

    void NuDisplayListBeginCriticalSection(void);
    void NuDisplayListEndCriticalSection(void);

void NuDisplayListAnimateMtls(f32 frame_time);
void NuDisplayListBeginCriticalSection(void);
void NuDisplayListClipSpecials(i32 enabled);
    void DisplayListCreateFxItemPS(void *item, i32 type);
void DisplayListCreateFxList(VARIPTR *buffer, VARIPTR end, i32 count);
i32 NuDisplayListCreateFx(i32 type, i32 priority, i32 layer);
void NuDisplayListCreateMtl(NUMTL *mtl);
    void DisplayListDestroyFxItemPS(void *item);
void NuDisplayListDestroyFx(i32 handle);
void NuDisplayListDestroyMtl(NUMTL *mtl);
void NuDisplayListDrawAll(void);
void NuDisplayListEndCriticalSection(void);
} // extern "C"

// Local helpers matching original static display-list setters (t local symbols)

extern "C" {
void NuDisplayListLinkMtl(nudisplaylist_s *list, NUMTL *mtl);
void NuDisplayListLinkList(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *first, NUDISPLAYLISTITEM *last);
    void *DisplayListCreateFaceonTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *faceon);
    void *DisplayListCreateGeomTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *next, void *tx);

    extern "C++" {
        // Original local helper cloned as DisplayListProcessSkin.isra.36 by GCC.
        // It appends the world transform, skin palette, and geometry calls as one
        // contiguous run in the material display list.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        // Original local helper cloned as DisplayListProcessLightmapped.isra.34.
        // Lightmapped geometry carries an AE/AF/B0 setup command two entries before
        // its geometry command. Keep that setup command adjacent to the dynamic
        // transform and geometry entries when the special is submitted.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

    i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light);
    NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material);
    void WindShear(NUMTX *out, NUMTX *in, i32 wind_scale, i32 wind_speed);

i32 NuDisplayListRndrSpecial(nuhspecial_s *special_handle, NUMTX *mtx, i32 skinned, NUMTX *skin_matrices,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void DisplayListSetFxItemParamPS(void *item, i32 parameter, f32 value, i32 mode);
void NuDisplayListSetFxParam(i32 handle, i32 parameter, f32 value, i32 mode);

    // ---------------------------------------------------------------------------
    // Scene / render-scene
    // ---------------------------------------------------------------------------

void NuDisplaySceneDebug(NUDLDLISTSCENE *scene, i32 flags, i32 depth, i32 *state);

    // ---------------------------------------------------------------------------
    // Frame lifecycle (NuFrameEnd is faithful; neighbours are stubs)
    // ---------------------------------------------------------------------------

    // Per-frame animation/pad update hooks (originals take the frame time).
    void NuPad_Interface_Render(void);
    void NuPadUpdatePads(void);

    // Frame-end callbacks (original bss @0x6bdaec/0x6bdaf0/0x6bdad0).
    extern void (*preRenderFlashingHack)(void);
    extern void (*postRenderFlashingHack)(void);
    extern void (*nuapi_endframe_callbackfn)(void);

    // Faithful transcription of the original frame-end pump. Waits for the
    // target frame interval, ticks material/tex/wind anims, swaps screens
    // via NuRndrSwapScreenEx, then advances nuapi clocks and pad state.

    // ---------------------------------------------------------------------------
    // iOS / platform
    // ---------------------------------------------------------------------------

void NuIOS_AwardAchievement(void);
void NuIOS_CheckCurrentFramebuffer(void);
    void NuIOS_DeallocateSystemRenderbuffer(GLuint renderbuffer);
void NuIOS_DeallocateSystemFramebuffers(void);
void NuIOS_DeallocateSystemRenderbuffer(GLuint);
    // Locale string is owned by the Android/JNI TU; cache only its index here.
extern i32 g_languageIndex;

i32 NuIOS_GetDeviceLanguage(void);
i32 NuIOS_HardwareSupportsRetina(void);
i32 NuIOS_IsLowestEndDevice(void);
i32 NuIOS_IsMidRangeDevice(void);
i32 NuIOS_IsSmallScreen(void);
void NuIOS_RecordFlurryEvent(char *event_name);
void NuIOS_ShowAchievements(void);

    // ---------------------------------------------------------------------------
    // Animation / curves / data
    // ---------------------------------------------------------------------------

void NuAnimBuffAccumulate_3(nuanimbuff_s *buffer, ani3_animheader_s *animation, f32 time, i32 overwrite, f32 blend,
                                i32 first_joint, nuhgobj_s *object, NUVEC *root_translation);
void *NuAnimBuffCreate(i32 max_joints, VARIPTR *buf, VARIPTR);
void NuAnimBuffCreateScratch(nuanimbuff_s *buffer);
void NuAnimBuffDestroyScratch(nuanimbuff_s *buffer);
void NuAnimBuffEvaluateCallback(NUANIMBUFFEVALUATECB callback, void **data, i32 *joints);
void NuAnimBuffEvaluate_3(nuanimbuff_s *buffer, nuhgobj_s *object, NUMTX *matrices, ani3_animheader_s *animation,
                              NUHGOBJROOTFN root_fn, NUVEC *root_translation, void *root_data);
void NuAnimBuffProceduralAnimation(nuanimbuff_s *buffer, nuhgobj_s *object, i32 override_count,
                                       NUJOINTANIM_s *overrides);
void NuAnimCurve2SetApplyToJointTransLoc(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                             nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                             NUJOINTANIM_s *override_anim, NUVEC *root_translation,
                                             NUVEC *locator_translation);
void NuAnimCurve2SetApplyToJoint(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                     nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                     NUJOINTANIM_s *override_anim);
void NuAnimCurve2SetApplyToMatrix_3(ani3_animheader_s *animation, i32 node, f32 frame, NUMTX *matrix);
void *NuAnimCurveCreate(void);
void NuAnimCurveDestroy(void *curve);
f32 *NuAnimCurveExtractAllNodeCurves_3(ani3_animheader_s *anim, i32 node, f32 frame, char *curve_mask);
void NuAnimCurveSetApplyBlendToJoint2(nuanimcurveset_s *first, nuanimtime_s *first_time, nuanimcurveset_s *second,
                                          nuanimtime_s *second_time, f32 blend, nuhgobjjoint_s *joint, NUVEC *scale,
                                          NUVEC *parent_scale, NUMTX *matrix, NUJOINTANIM_s *override_anim);
void NuAnimCurveSetApplyToMatrix(nuanimcurveset_s *set, nuanimtime_s *time, NUMTX *matrix);
void *NuAnimCurveSetCreate(i32 curve_count);
void NuAnimCurveSetDestroy(void *curve_set, i32 destroy_curves);
void NuAnimData2CalcMatrix(nuanimdata_s *animation, i32 node, f32 frame, numtx_s *matrix);
void NuAnimData2CalcTime(nuanimdata2_s *anim, f32 frame, nuanimtime_s *time);
void NuAnimDataCalcTime(void *animation, f32 frame, nuanimtime_s *time);
void *NuAnimDataCreate(i32 chunk_count);
    extern "C++" void NuAnimDataChunkDestroy(nuanimdatachunk_s *chunk);

void NuAnimDataDestroy(void *animation);
i32 NuAnimDataFindVersion(char *path);
void *NuAnimDataLoadBuff(char *path, VARIPTR *buf, VARIPTR *buf_end);
void *NuAnimGetAnimLOD(void *animation, i32 lod);
i32 NuAnimGetUseQuatsFlag(void);
i32 NuAnimNumNodes(void *animation);
i32 NuAnimPopUseQuatsFlag(void);
i32 NuAnimPushSetUseQuatsFlag(i32 enabled);
i32 NuAnimSetUseQuatsFlag(i32 enabled);

    // ---------------------------------------------------------------------------
    // Bridge / file / heap / memory
    // ---------------------------------------------------------------------------

void *NuMemAllocFn(u32 size);
void NuMemFreeFn(void *ptr);
void *NuMemReAllocFn(void *ptr, u32 size);


i32 NuSysDirClose(i32 index);
i32 NuSysDirOpen(void);
i32 NuSysDirRead(void);

    // ---------------------------------------------------------------------------
    // Math / geometry
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Quick-font platform rendering (the generic font run lives in nuqfnt.cpp)
    // ---------------------------------------------------------------------------

VUFNT *NuQFntCreate(NUFNT *font, i32 flags, i32 render_options, VARIPTR *buf, VARIPTR *);
void NuQFntDestroy(VUFNT *font);
void NuQFntEncodeUnicodeString(NUQFNT *font, u16 *text);
NUQFNT_CSMODE NuQFntGetCoordinateSystem(void);
u32 NuQFntGetPrintMode(void);
f32 NuQFntHeightScale(void);
f32 NuQFntLenScale(void);
void NuQFntMove2d(NUQFNT *font, f32 x, f32 y, f32 z);
void NuQFntPopCoordinateSystem(void);
void NuQFntPrint2dU(NUQFNT *font, char *text);
void NuQFntPrint2dW(NUQFNT *font, u16 *text);
void NuQFntPrint3DU(NUQFNT *font, char *text);
void NuQFntPrint3DW(NUQFNT *font, u16 *text);
void NuQFntPrintEx(NUQFNT *font, i32 x, i32 y, i32 alignment, const char *format, ...);
f32 NuQFntPrintLenV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPrintV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPushCoordinateSystem(NUQFNT_CSMODE mode);
void NuQFntSet2d(NUQFNT *font);
void NuQFntSetColour2d(NUQFNT *font, u32 colour);
    void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);
void NuQFntSetPointSize(NUQFNT *font, f32 width, f32 height);
void NuQFntSetPrintMode(u32 mode);
void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);

    // ---------------------------------------------------------------------------
    // Rendering / materials / effects (host has GL paths elsewhere)
    // ---------------------------------------------------------------------------

void NuAccumulationMotionBlurEffect(i32 frames, f32 blend, i32 mode);
void NuAccumulationMotionBlurParams(i32 frames, f32 blend, i32 mode);
    extern nurenderscene_s currentScene;
void NuBackbufferCopy(i32 texture_id);
void NuDeferredShadingRender(void);
void NuDeferredShadingSetParameterf(i32 parameter, f32 value);
void NuDepthOfFieldEffect(f32 strength, f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect1(f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect2(f32 near_distance, f32 far_distance, f32 strength);
void NuDepthOfFieldEffectEx(const NuDepthOfFieldParameters *parameters);
extern i32 speedblur_enabled;
extern f32 NuLightsx;
extern f32 NuLightsy;
void NuLightSpeedBlur(i32 reuse_camera, f32 scale);
void NuLightSpeedBlurOldCameraPos(const NUMTX *camera);
void NuLightSpeedBlurScale(f32 x, f32 y);
extern NULGTARCLASER NuLgtArcLaserData[16];
extern i32 NuLgtArcLaserOldCnt;
void NuLgtArcLaser(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                       f32 bend_amount, i32 colour);
extern i32 NuLgtLaserCnt;
extern i32 NuLgtArcLaserCnt;
extern i32 NuLgtArcLaserFrame;
extern NULGTLASER NuLgtLaserData[64];

void NuLgtLaser(i32 type, f32 width, f32 segment_length, f32 width_wobble, NUVEC *start, NUVEC *delta, u32 colour,
                    f32 end_width, f32 length);
void NuLgtLaserDraw(i32 paused);

void NuLgtSetArcMat(NUMTL *material, f32 u0, f32 v0, f32 u1, f32 v1);
void NuPostBloom(i32, const NuBloomParameters *parameters);
void NuSpecialBurstDrawAt(nuhspecial_s *special, u32 count, NUMTX *matrices, i32 clip);
i32 NuSpecialDrawAt(void *special, NUMTX *mtx);
i32 NuSpecialDrawSmoothSkin(void *special, NUMTX *skin_matrices, NUMTX *world_matrix);
i32 NuSpecialDrawSmoothSkinDwa(void *special, NUMTX *skin_matrices, NUMTX *world_matrix,
                                   DEFORMERWEIGHTSARRAY *blend_values);
i32 NuSpecialDrawWith(void *special, NUMTX *mtx);
void NuSpecialSetDrawMtx(void *special, NUMTX *mtx);

    // ---------------------------------------------------------------------------
    // Light / wind / particles / debris
    // ---------------------------------------------------------------------------

void NuDynamicLightAddRenderScene(NuDynamicLight *light, i32 index, i32 scene_id);
void NuDynamicLightAddShadowCasterScene(NuDynamicLight *light, nugscn_s *scene);
void NuDynamicLightBeginCapture(NuDynamicLight *light, i32 render_set);
NuDynamicLight *NuDynamicLightClone(NuDynamicLight *light, VARIPTR *arena, VARIPTR end);
NuDynamicLight *NuDynamicLightCreate();
void NuDynamicLightDestroy(NuDynamicLight *light);
void NuDynamicLightEndCapture(void);
i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light);
NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material);
f32 NuDynamicLightGetParameterf(NuDynamicLight *light, i32 parameter);
i32 NuDynamicLightGetParameteri(NuDynamicLight *light, i32 parameter);
NUMTX *NuDynamicLightGetProjection(NuDynamicLight *light);
NUMTX *NuDynamicLightGetView(NuDynamicLight *light);
i32 NuDynamicLightIsUsedOnSpecials(NuDynamicLight *light);
void NuDynamicLightLookAt(NuDynamicLight *light, NUVEC *eye, NUVEC *target, NUVEC *up);
void NuDynamicLightResetGeometry(NuDynamicLight *light);
void NuDynamicLightSetDirectional(NuDynamicLight *light, f32 left, f32 right, f32 bottom, f32 top, f32 near_plane,
                                      f32 far_plane);
void NuDynamicLightSetEnabled(NuDynamicLight *light, i32 enabled);
void NuDynamicLightSetParameterf(NuDynamicLight *light, i32 parameter, f32 value);
void NuDynamicLightSetParameteri(NuDynamicLight *light, i32 parameter, i32 value);
void NuDynamicLightSetUsedOnSpecials(NuDynamicLight *light, i32 enabled);
void NuDynamicLightSetupCustomCameraFrustum(NuDynamicLight *light, NUCAMERA *camera, const f32 *splits, i32 count);
i32 NuDynamicLightTestShadowExtrusionExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent,
                                                i32 render_set);
void NuDynamicLightTestShadowExtrusions(NuDynamicLight *light, const VuVec *first, const VuVec *second);
void NuDynamicLightTestShadowExtrusionsExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent);

i32 NuWindLoad(NUWIND *wind, i32 index, char *name, VARIPTR *buffer, VARIPTR buffer_end);

void NuWindSetCurrent(NUWIND *wind, i32 index);

void NuWindUnload(NUWIND *wind, i32 index);
    void NuWindUpdateArray(NUVEC **);

    extern "C" f32 partglobaltime;
void NuPartResetGlobalTime(void);
    // ---------------------------------------------------------------------------
    // Gobj / hierarchy / scene graph
    // ---------------------------------------------------------------------------

void NuHGobjDestroy(nuhgobj_s *object);
void NuHGobjEval(nuhgobj_s *object, i32 override_count, nuhgobjjointoverride_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim(void);
void NuHGobjEvalAnim2(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                          NUJOINTANIM_s *overrides, NUMTX *matrices);
extern i32 ddmaxjoints;
void NuHGobjEvalAnim2Root(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                              NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);

void NuHGobjEvalAnim2Root_3(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                                NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
#include "nu2api/nucore/nucore_plain_types.h"
void NuHGobjEvalAnimBlend(nuhgobj_s *object, void *animation_a, f32 frame_a, void *animation_b, f32 frame_b,
                              f32 blend, i32 override_count, NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                               ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                               NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2Root(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                                   ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                                   NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
void **NuHGobjEvalDwa(i32 render_count, i16 *render_indices, void *animation, f32 frame);
void **NuHGobjEvalDwa2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation, f32 frame);
void **NuHGobjEvalDwaBlend(i32 render_count, i16 *render_indices, void *animation_a, f32 frame_a, void *animation_b,
                               f32 frame_b, f32 blend);
void **NuHGobjEvalDwaBlend2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation_a, f32 frame_a,
                                nuanimdata2_s *animation_b, f32 frame_b, f32 blend);
i32 NuHGobjForceShadowsOnCharacters(i32 enabled);
void NuHGobjFromVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuHGobjJointMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
void NuHGobjPOIMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
i32 NuHGobjReversibleCharacters(i32 enabled);
i32 NuHGobjRndrMtxDwa(nuhgobj_s *object, NUMTX *world_matrix, i32 render_count, i16 *render_indices,
                          NUMTX *joint_matrices, void **blend_values, i32);
    u32 NuWindRand(void);
i32 NuHGobjRndrRandShadowSurfacePoints(nuhgobj_s *object, NUMTX *world_matrix, NUMTX *joint_matrices, i32 count,
                                           NUVEC *positions, i32 exclusion_mask);
void NuHGobjSetClippingRootTrackerOverride(i32 enabled);
void NuHGobjToVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuGCutCharAnimProcess(NUGCUTCHAR_s *character, f32 frame, NUMTX *matrix, i32 *visible, u32 *animation_index,
                               f32 *animation_rate, f32 *blend_time, f32 *animation_start_frame, i32 *layer_mask);
void NuGCutSetCutAudioStream(i32 stream);
    void NuTexAnimRemoveList(void *texture_anims);
    void NuGHGPreRelocateFixupPS(NUGSCN *scene);
    void NuGHGPostRelocateFixupPS(NUGSCN *scene, i32 delta);
NUGSCN *NuGHGRelocate(NUGSCN *scene, VARIPTR *destination);

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

    NUKEYCHAR NuKeyChar[65] = {
        {0x2, 0x31, 0x21, {0, 0}},  {0x3, 0x32, 0x22, {0, 0}},  {0x4, 0x33, 0xa3, {0, 0}},  {0x5, 0x34, 0x24, {0, 0}},
        {0x6, 0x35, 0x25, {0, 0}},  {0x7, 0x36, 0x5e, {0, 0}},  {0x8, 0x37, 0x26, {0, 0}},  {0x9, 0x38, 0x2a, {0, 0}},
        {0xa, 0x39, 0x28, {0, 0}},  {0xb, 0x30, 0x29, {0, 0}},  {0xc, 0x2d, 0x5f, {0, 0}},  {0xd, 0x3d, 0x2b, {0, 0}},
        {0xf, 0x09, 0x09, {0, 0}},  {0x10, 0x71, 0x51, {0, 0}}, {0x11, 0x77, 0x57, {0, 0}}, {0x12, 0x65, 0x45, {0, 0}},
        {0x13, 0x72, 0x52, {0, 0}}, {0x14, 0x74, 0x54, {0, 0}}, {0x15, 0x79, 0x59, {0, 0}}, {0x16, 0x75, 0x55, {0, 0}},
        {0x17, 0x69, 0x49, {0, 0}}, {0x18, 0x6f, 0x4f, {0, 0}}, {0x19, 0x70, 0x50, {0, 0}}, {0x1a, 0x5b, 0x7b, {0, 0}},
        {0x1b, 0x5d, 0x7d, {0, 0}}, {0x1e, 0x61, 0x41, {0, 0}}, {0x1f, 0x73, 0x53, {0, 0}}, {0x20, 0x64, 0x44, {0, 0}},
        {0x21, 0x66, 0x46, {0, 0}}, {0x22, 0x67, 0x47, {0, 0}}, {0x23, 0x68, 0x48, {0, 0}}, {0x24, 0x6a, 0x4a, {0, 0}},
        {0x25, 0x6b, 0x4b, {0, 0}}, {0x26, 0x6c, 0x4c, {0, 0}}, {0x27, 0x3b, 0x3a, {0, 0}}, {0x28, 0x27, 0x40, {0, 0}},
        {0x2b, 0x5c, 0x3f, {0, 0}}, {0x2c, 0x7a, 0x5a, {0, 0}}, {0x2d, 0x78, 0x58, {0, 0}}, {0x2e, 0x63, 0x43, {0, 0}},
        {0x2f, 0x76, 0x56, {0, 0}}, {0x30, 0x62, 0x42, {0, 0}}, {0x31, 0x6e, 0x4e, {0, 0}}, {0x32, 0x6d, 0x4d, {0, 0}},
        {0x33, 0x2c, 0x3c, {0, 0}}, {0x34, 0x2e, 0x3e, {0, 0}}, {0x35, 0x2f, 0x00, {0, 0}}, {0x37, 0x2a, 0x2a, {0, 0}},
        {0x39, 0x20, 0x20, {0, 0}}, {0x47, 0x37, 0x37, {0, 0}}, {0x48, 0x38, 0x38, {0, 0}}, {0x49, 0x39, 0x39, {0, 0}},
        {0x4a, 0x2d, 0x2d, {0, 0}}, {0x4b, 0x34, 0x34, {0, 0}}, {0x4c, 0x35, 0x35, {0, 0}}, {0x4d, 0x36, 0x36, {0, 0}},
        {0x4e, 0x2b, 0x2b, {0, 0}}, {0x4f, 0x31, 0x31, {0, 0}}, {0x50, 0x32, 0x32, {0, 0}}, {0x51, 0x33, 0x33, {0, 0}},
        {0x52, 0x30, 0x30, {0, 0}}, {0x53, 0x2e, 0x2e, {0, 0}}, {0x8d, 0x3d, 0x3d, {0, 0}}, {0xb5, 0x2f, 0x2f, {0, 0}},
        {0x0, 0x00, 0x00, {0, 0}},
    };
i32 NuKeyToAscii(u32 key, i32 shifted);
void NuPad_Interface_TouchScreenInput(i32 x, i32 y, i32 prior_x, i32 prior_y, i32 is_down, i32 is_up, i32 is_move,
                                          i32 is_cancelled);
    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    i32 clipTestSphere(NUPORTALSPHERE *sphere, NUFRUSTRUM *frustum);

i32 NuPortalClipTest(NUGSCN *scene, NUVEC *position, f32 radius, i16 room_id);
i32 NuPortalEnabled(i32 enabled);
i32 NuPortalNumRooms(NUGSCN *scene);
void NuPortalResetActive(NUGSCN *scene);
i32 NuPortalRoomClipTest(NUGSCN *scene, i16 room_id);
void NuPortalRoomClipTestAll(NUGSCN *scene, u8 *room_visibility);
i32 NuPortalWhichRoom(NUGSCN *scene, NUVEC *position);
extern i32 VisiSysCameraLock;
extern i32 LoadedOcclusionData;
extern i32 UsingOcclusionData;
extern i32 CurrentViewBox;
extern u8 OcclusionBitArray[800];
extern i32 do_InstTree;
extern i32 do_occlusion;
extern i32 do_visibility;
extern i32 do_octree;

    void NuVisiInstTree(void *, NUGSCN *);
    void NuVisiOcclusion(NuVisibilityResult *);

void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
    DECOMP_ASSERT(sizeof(NuInstanceTreeNode) == 0x0c, "instance tree node stride");
    DECOMP_ASSERT(sizeof(NuInstanceTreeBounds) == 0x20, "instance tree bounds stride");
    DECOMP_ASSERT(sizeof(NuInstanceTree_s) == 0x28, "instance tree size");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, nodes) == 0x14, "instance tree nodes offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, roots) == 0x20, "instance tree roots offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, far_clips) == 0x24, "instance tree far clips offset");
extern NuVisibilityResult * myvisi;
    extern "C++" {
        // Retail only has the IPA-SRA clone (the unused scene and bits arguments dropped).
        static __attribute__((noinline, noclone)) void
        ClipInstTree(NuInstanceTree_s *tree,
                     i32 tree_index) __asm__("_ZL12ClipInstTreeP16NuInstanceTree_siP16nudisplayscene_sPc.isra.92");
    }
void NuVisiInstTree(void *visibility_result, NUGSCN *scene);

void NuVisiOcclusion(NuVisibilityResult *result);
    extern "C++" void OctreeRndr(u8 *visibility, nuoctreenode_s *root, i32 enabled);
void NuVisiOctree(NuVisibilityResult *result);
void NuOcclusionManagerAddOccluderOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix);
void NuOcclusionManagerAddOccluderQuad(const NUVEC *a, const NUVEC *b, const NUVEC *c, const NUVEC *d);
void NuOcclusionManagerAddOccluderSphere(const NUVEC *center, f32 radius);
void NuOcclusionManagerEndFrame(void);
void NuOcclusionManagerInit(u32 capacity, VARIPTR *buffer, VARIPTR buffer_end);
bool NuOcclusionManagerIsEnabled(void);
bool NuOcclusionManagerIsInitialised(void);
i32 NuOcclusionManagerIsOccludedOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix);
i32 NuOcclusionManagerIsOccludedSphere(const NUVEC *center, f32 radius);
void NuOcclusionManagerOnCameraSet(void);
void NuOcclusionManagerRenderStats(void);
void NuOcclusionManagerRenderZPass(void);
void NuOcclusionManagerSetEnabled(i32 enabled);
void NuOcclusionManagerSetOccluderDotProductThreshold(f32 threshold);
void NuOcclusionManagerSetOccluderScreenSpaceThreshold(f32 threshold);
    // ---------------------------------------------------------------------------
    // Viewport
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Strings / conversion / Unicode
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Containers / lists / params
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Debug / error / profiling
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Thread / misc OS
    // ---------------------------------------------------------------------------

#ifndef ANDROID
    void NuGetCurrentThreadId(void) {
        STUBBED();
    }
#endif
#ifndef ANDROID
    void NuThreadCreate(void) {
        STUBBED();
    }
#endif
void NuPause(i32 paused);
    // ---------------------------------------------------------------------------
    // Spline / online / net / other gameplay support
    // ---------------------------------------------------------------------------

void NuSplineList(void);
    extern void (*nuapi_endframe_callbackfn)(void);
void NuRegisterEndFrameCallBackFn(void (*callback)(void));

    // ---------------------------------------------------------------------------
    // Rendering extras
    // ---------------------------------------------------------------------------

i32 NuStreamInit(void);

    // ---------------------------------------------------------------------------
    // PS2 / Xbox legacy shims
    // ---------------------------------------------------------------------------

} // extern "C"

struct nupad_s;
struct nuframebuffer_s;
struct nushaderobject_s;
union variptr_u;

void NuLgtSetArcMatEx(i32 type, numtl_s *material, f32 u0, f32 v0, f32 u1, f32 v1);

void NuLgtArcLaserEx(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                     f32 bend_amount, i32 colour, i32 flags);

// ==== src/gameapi/edtools/edui.cpp: all remaining content ====
#include "gameapi/edtools/edui.h"

extern "C" {
    static void eduicbItemExpanderClose(edui_expander_s *expander) {
        for (eduiitem_s *item = expander->first_child; item; item = item->next) {
            if (item->type == EDUI_ITEM_EXPANDER)
                eduicbItemExpanderClose(static_cast<edui_expander_s *>(item));
            if (item == expander->last_child)
                break;
        }
        if (expander->first_child && expander->open) {
            expander->next = expander->last_child->next;
            if (expander->last_child->next)
                expander->last_child->next->previous = expander;
            expander->first_child->previous = NULL;
            expander->open = 0;
        }
    }

void eduicbMenuCloseAllexpanders(eduimenu_s *menu);

void eduicbMenuOpenAllexpanders(eduimenu_s *menu);
}

extern "C" void eduiMenuDestroyItems(eduimenu_s *menu);

// ==== src/legoapi/actions/character/speederchase.cpp: declarations ====
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/world/levels/levels.h"
#include "legogame/game.h"
#include "decomp.h"
#include "legoapi/actions/character/speederchase.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/items/collect/spacelevel.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edui.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nurand.h"
#include "legoapi/items/objects/gameobjects.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nu3d/nutex.h"
#include <string.h>


// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

#include "legoapi/actions/character/speederchase_types.h"

extern SPEEDERCHASEANETPACKET_s * speederchasea_netpacket;

extern u8 troopercannons_beenReset;
extern i32 players_going_forward;
extern i32 players_cannot_exit_speeder;

extern f32 zoom_ahead_extra;
extern f32 zoom_ahead_extra_time;
extern f32 speeder_level_height_lerpf;
extern f32 speeder_level_height;
extern SPEEDERCHASEVALUES speedervals[4];
extern f32 speederfirerange;
extern f32 speeder_shootrate;
extern f32 speeder_kill_dist;
extern f32 speeder_midrange_time;
extern f32 speeder_ahead_time;
extern f32 speeder_mode_ahead_timer;
DECOMP_ASSERT(sizeof(SPEEDERCHASEVALUES) == 0x1c, "Speeder chase tuning stride");
DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

extern i32 objopponent_ignoreaiopponent;
extern i32 test_ai_combo;
extern i32 CanPunchGirls;
extern i16 id_GAMORREANGUARD;
extern i16 id_STORMTROOPER;
i32 SpecialMove_Check(GameObject_s *, GameObject_s *);
u32 SpecialMove_GetFlags(i32, u32);

void PodKeyReset();

void TakeHitRumble(GameObject_s *, f32);

void PodLoseSpeed(GameObject_s *object, i32 hit, i32 rumble);

i32 SpeederBlowupHack(GIZMOBLOWUP_s *blowup, i32);

f32 FindPodHoverHeight(GameObject_s *object);

extern i32 ObjInNarrowSock(GameObject_s *, SOCKSYS *, i32);
extern i32 IDLESPEEDINNARROWSOCKSONLY;

i32 ObjIsTargetSpeeder(GameObject_s *object);
f32 GetVehicleSpeedMul(GameObject_s *object, f32 speed);

void SetLevelExBlowupFunc(i32 (*callback)(GIZMOBLOWUP_s *, i32));
void InitTrooperCannons(WORLDINFO_s *world);
void ResetTrooperCannons(WORLDINFO_s *world, i32 trooper_id);
void UpdateTrooperCannons(WORLDINFO_s *world);
i32 GoingForwardsAlongNarrowSock(GameObject_s *object);

void SpeederChaseA_Init(WORLDINFO_s *world);

void ProcessCurrentSpeed(WORLDINFO_s *world, speedup_s *speedup);

void SpeederChaseA_Panel(WORLDINFO_s *);

void SpeederChaseA_Reset(WORLDINFO_s *);

i32 SpeedersDroppedBack();

i32 Action_SpeederBeingChased(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **, i32, i32 first_time,
                              f32 elapsed);

void SpeederChaseA_Update(WORLDINFO_s *world);

i32 ObjOpponentStillThere(GameObject_s *object, GameObject_s *opponent, f32 gap);

f32 SpeederChaseATATInOutMul(nuvec_s *start, nuvec_s *end);

f32 GetVehicleAreaRememberSpeed();

i32 SpeederChase_ObjIsAGroundTroop(GameObject_s *object);

extern "C" {

    i32 edmain_auto_speed = 1;

void cbSetAutoSpeed(eduimenu_s *, eduiitem_s *item, u32);

} // extern "C"

extern "C" void AddDebrisEffect(i32 *, i32, f32, f32, f32);
extern "C" void DebrisPosOrientationMtx(i32, NUMTX *);
extern "C" void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

void PodDust(WORLDINFO_s *world, GameObject_s *object);

extern "C" f32 pod_roll[2];
f32 getPodRoll(i32 index);

// ==== src/legoapi/characters/motion/camera.cpp: declarations ====
#include "decomp.h"
#include "legoapi/world/area.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "globals.h"
#include <stdlib.h>
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/cutscenes/minicamcut.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"

void Minicam_InitSystem(void);
extern "C" void NuLightSpeedBlurOldCameraPos(const NUMTX *camera);
void GameCam_ResetLookRot(GAMECAMERA_s *camera);
i32 GetMenuID(void);
i32 OnOrInsidePlane(NUVEC *point, NUVEC *plane_point, NUVEC *plane_normal, NUVEC *corrected_point, f32 normal_offset,
                    f32 *distance_out);
void Surface_Deflect(NUVEC *normal, NUVEC *movement, NUVEC *result, i32 mode);
void SpecialMove_Cancel(GameObject_s *object);
void Hint_CancelCurrent(void);
extern AREADATA_s *PODSPRINT_ADATA;
extern AREADATA_s *BATTLEOVERCORUSCANT_ADATA;

#include "legoapi/characters/motion/camera_types.h"
extern VIEWCAM_s ViewCam;
// Original data symbol testmovef is float0.005 at 0x617cb0.
extern f32 testmovef;

DECOMP_ASSERT(sizeof(VIEWCAM_s) == 0x28, "ViewCam size");
DECOMP_ASSERT(offsetof(VIEWCAM_s, pitch) == 0x10, "ViewCam pitch offset");
DECOMP_ASSERT(offsetof(VIEWCAM_s, gamepad) == 0x24, "ViewCam gamepad offset");

void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);

void GameCam_Reset(GAMECAMERA_s *camera);

void GameCam_Judder(GAMECAMERA_s *camera, float amount, i32 axis, nuvec_s *source);

void GameCam_HitRoll();

void GameCam_NewShake(GAMECAMERA_s *camera, float amount, float duration, float speed);

void GameCam_HitJudder();

void GameCam_UpdateShake(GAMECAMERA_s *camera, float ambient_amount);


void GameCam_UpdateLookRot(GAMECAMERA_s *camera);

void GameCameraMakeMiniCut(nugspline_s *spline, f32 start, f32 end, f32 blend_in, f32 blend_out, i32 borders,
                           i32 hold_until_players_move);

extern nugspline_s ObstacleCamCutSpline;
extern i32 ObstacleCamTargetGuid;
extern NUVEC ObstacleCamCutPts[2];

void GameCameraMakeMiniCut2(nuvec_s *camera, nuvec_s *target, i32 target_guid, float start, float end, float blend_in,
                            float blend_out, i32 follow_target, i32 follow_camera, i32 borders);

void GameCameraMakeMiniCut3(u32 flags, float distance, i32 pitch, i32 yaw, i32 roll, void *focus, i32 target_guid,
                            nuvec_s *position, float start_time, float blend_in_time, float end_time,
                            float blend_out_time, float blend_time, float hold_time, float max_time, i32 borders,
                            nugspline_s *spline, char mode, char easing);

u16 GameCam_GetAdjustedYRot(GAMECAMERA_s *camera);

extern "C" {

    extern NUVEC *CutoffCameraVec;

f32 CameraEmitterDistance(NUVEC *position);

} // extern "C"

extern "C" i16 id_GRABCONTROL, id_GRABR2CONTROL, id_LANDSPEEDER, id_ATST, id_DEWBACK;
extern "C" f32 MainRenderTime;
extern f32 newgamecamtime, cutscenecam_focalLength, CamStopBlend, EMPERORFIGHTA_CAMDYHACK;
extern "C" i32 NewMode;
extern i32 CUTCAMONLY, CUTCAM, GUNSHIPAHACK, netcamera, complexsockposition_forcesock, movegamecamera_forcesock;
extern i32 LevFlag[4];
extern NUMTX cutscenecammtx, CutCamMtx;
extern u8 set_cutscenecammtx;
extern NUGSPLINE *hub_minikitviewer_camspl;
extern AREADATA_s *PODRACE_ADATA;
f32 getPodRoll(i32);
f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);
f32 PodSprint_InStartCountdown(WORLDINFO_s *);
i32 PodRace_InStartCountdown(WORLDINFO_s *);
extern "C" i32 editor_active;
extern "C" void NuSpeedBlurSetMotionFactors(f32 pan, f32 pull, f32 clamp);
extern "C" void NuLightSpeedBlur(i32 reuse_camera, f32 scale);
i32 Players_AveragePos(NUVEC *, SOCKPOSITION_s *);
GameObject_s *FindNearestGameObject(NUVEC *, GameObject_s *, u32, f32, f32, i32, i32, i32, f32 *, i32,
                                    i32 (*)(GameObject_s *), bool);
void Minicam_Update();
void Customiser_GetActiveWeirdoIndex(i32 *, i32 *);
i32 ObjInNarrowSock(GameObject_s *, SOCKSYS *, i32);
void MakePlayPlanes(GAMECAMERA_s *);
i32 GameRayCast(NUVEC *, NUVEC *, f32, i32);

extern NUVEC CustomisePos[2];
extern f32 HUB_MINIKITVIEWER_CAMDY;
extern f32 PodCamDist;
extern f32 EMPERORFIGHTA_CAMDYHACK;
extern i32 movegamecamera_forcesock;
extern NUMTX CutCamMtx;
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, path) == 0x40, "Camera context path offset");
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, flags) == 0x4c, "Camera context flags offset");
NUVEC *Technos_TgtPos(TECHNO_s *);
i32 Grapple_LookAtPos(GameObject_s *, NUVEC *);
void CentreTwoPlayerCamera(NUVEC *, NUVEC *, NUVEC *, NUVEC *);


void MoveGameCamera(GAMECAMERA_s *camera);

void ViewCamDraw();

void KeepOnScreen(GameObject_s *object);

NUVEC *ViewCamGetTgt();

i32 ViewCamGetMode();

void SetDepthOfField();

void SpeedBlur_Apply(WORLDINFO_s *world);

void SpeedBlur_Update();

void ViewCamSetActive(i32 mode, GAMEPAD_s *gamepad);

void KeepPointOnScreen(NUVEC *position, NUVEC *velocity);

void SetCameraMatrices();

GAMEPAD_s *ViewCamGetGamePad();

void CentreTwoPlayerCamera(nuvec_s *center, nuvec_s *player_a, nuvec_s *player_b, nuvec_s *reference);

void KeepVehicleOnScreen(GameObject_s *object, i32 sides, i32 top, i32 bottom);

void do_Pad_flymode_camera(edcam_s *camera, float delta_time, nupad_s *pad);

void InitCameraTargetMaterial();


void GetTopBot(GameObject_s *object);

extern "C" {

    i32 near_clip_at_cursor;

void cbNearClipAtCursor(eduimenu_s *, eduiitem_s *item, u32);

void do_Pad_Standard_camera(edcam_s *camera, f32 delta_time, nupad_s *pad);

void do_maya_mouse_camera(edcam_s *camera);

void do_mouse_flymode_camera(edcam_s *camera, f32 delta_time);

} // extern "C"

// ==== src/gameapi/edtools/edfile.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/edtools/edfile.h"

#include <string.h>

#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufilepak.h"

static i32 edfile_media;
i32 edfile_handle = -1;
i32 edfile_write_flag;

static void *edfile_pakfile;

char edfile_buffer[0x1000];
i32 edfile_buffer_pointer;
i32 edfile_lock[2];

i32 edfile_readwrongendianess;
i32 edfile_mcresult;

void EdFileSwapEndianess16(void *data);

void EdFileSwapEndianess32(void *data);

void EdFileSetReadWrongEndianess(i32 value);

void EdFileSetMedia(i32 media);

void EdFileSetPakFile(void *pak);

void EdFileResetBuffers();

void EdFileFillBuffer();

i32 EdFileOpen(char *filepath, NUFILEMODE mode);

void EdFileFlushBuffer();

i32 EdFileClose();

void EdFileWrite(void *data, i32 len);

void EdFileWriteFloat(f32 value);

void EdFileWriteInt(i32 value);

void EdFileWriteUnsignedInt(u32 value);

void EdFileWriteShort(i16 value);

void EdFileWriteUnsignedShort(u16 value);

void EdFileWriteChar(char value);

void EdFileWriteUnsignedChar(u8 value);

void EdFileWriteNuVec(NUVEC *value);

void EdFileRead(void *buf, i32 len);

f32 EdFileReadFloat();

i32 EdFileReadInt();

u32 EdFileReadUnsignedInt();

i16 EdFileReadShort();

u16 EdFileReadUnsignedShort();

char EdFileReadChar();

unsigned char EdFileReadUnsignedChar();

void EdFileReadNuVec(NUVEC *out);

i32 EdFileWriteMemCard(char *filename, i32 size, void *data);

i32 EdFileReadMemCard(char *filename, i32 size, void *data);

i32 EdFileBackup(char *source, char *destination);

// ==== src/legoapi/core/config/saveload.cpp: declarations ====
#include "decomp.h"
#include <stdio.h>
#include <string.h>

#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/core/config/fileselect.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"

extern i32 memcard_deleteneeded, memcard_deletestarted, memcard_deletefailed;
extern i32 memcard_formatme, memcard_formatting, memcard_formatfailed;
extern i16 (*memcard_hashfn)(void);
i32 numfilteroutblocks;
char filteroutblocks[8][12];
i32 numfilterblocks;
char filterblocks[8][96];

char FS_FileList[0x10000];
char FS_LastFileName[64];
char FS_Path[256];
i32 FS_NumFiles;
i32 FS_CurrentPosFileNum;
i32 FS_CursorLine;
char FS_ParentEntry[8] = "D######";
i32 FS_SortMode = 3;
char *FS_CurrentCursorPos = FS_FileList;
char *FS_CurrentPos = FS_FileList;
char *FS_FileListEnd = FS_FileList;
char FS_Title[64] = "Title";
char FS_Filter[256] = "*.nup | *.hgp   ";
char FS_FilterOut[256] = "_PC. | _360. | _PS3.";
f32 FS_X = 50.0f;
f32 FS_Y = 40.0f;
f32 FS_Width = 248.0f;
f32 FS_W;
f32 FS_H;
u8 FS_Active;
u8 FS_RefreshDir;
u8 FS_ShowVolumes;
void (*FS_Callback)(char *, char *);

void FS_BuildFilterBlocks(char *);
void FS_BuildFilterOutBlocks(char *);
void FS_SortStrings(char *, char *, i32);
i32 FS_FileNameFilter(char *);
void FS_MakeDateTimeString(FS_FILEENTRYHDR *, char *);

extern f32 memcard_createmessage_delay;
extern f32 memcard_createresult_delay;
extern f32 memcard_message_delay;
extern f32 memcard_result_delay;
extern i32 memcard_justformatted;
extern i32 saveload_autosavedisabled;

void InitMemCard();

i32 SaveGizmoSys(GIZMOSYS_s *, char *, char *);

void SerialiseInt(EdStream &stream, void *data, i32);

void FS_GetDirList(char *path, char *filter, char *filter_out);

void SerialiseChar(EdStream &stream, void *data, i32);

i32 FS_PrevNameLen(char *name);

static inline u32 FS_EncodedDateKey(const char *name) {
    u32 key = static_cast<u8>(name[6]) - 'A';
    key = key * 13 + static_cast<u8>(name[5]) - 'A';
    key = key * 33 + static_cast<u8>(name[4]) - 'A';
    key = key + ((key + (key << 1)) << 3) + static_cast<u8>(name[3]) - 'A';
    key *= 61;
    key += static_cast<u8>(name[2]);
    key -= 'A';
    key *= 61;
    return key + static_cast<u8>(name[1]) - 'A';
}

void FS_SortStrings(char *start, char *end, i32 mode);

void SerialiseFloat(EdStream &stream, void *data, i32);

void SerialiseNuMtx(EdStream &stream, void *data, i32);

void SerialiseNuVec(EdStream &stream, void *data, i32);

void SerialiseShort(EdStream &stream, void *data, i32);

void SerialiseVuMtx(EdStream &stream, void *data, i32);

void SerialiseVuVec(EdStream &stream, void *data, i32);

void FS_MoveCursorUp(i32 steps);

void SerialiseString(EdStream &stream, void *data, i32 count);

void SerialiseColour3(EdStream &stream, void *data, i32);

i32 FS_FileNameFilter(char *name);

void FS_MakeDateString(FS_FILEENTRYHDR *entry, char *output);

void FS_MakeTimeString(FS_FILEENTRYHDR *entry, char *output);

extern "C" void NuRndrRect2d(f32 x, f32 y, f32 z, f32 width, f32 height, i32 colour, NUMTL *material);

char *SortStrings[4] = {const_cast<char *>("A to Z"), const_cast<char *>("Z to A"), const_cast<char *>("Old to New"),
                        const_cast<char *>("New To Old")};

// Retail keeps the whole body of RenderFileSel3 as the split-out .part clone;
// the exported function only tests FS_Active.
static __attribute__((noinline, noclone)) void RenderFileSel3Body(i32 draw_title)
    __asm__("_Z14RenderFileSel3i.part.38");

static void RenderFileSel3Body(i32 draw_title);

void RenderFileSel3(i32 draw_title);

void RenderFileSel();

void FS_MoveCursorDown(i32 steps);

f32 FS_GetDirTextWidth();

char *FS_GetFilterString(char *input, char *output);

i32 getsaveload_status();

i32 FS_GetPadWithRepeat(nupad_s *pad, float repeat, float elapsed);

void SerialiseNuHSpecial(EdStream &stream, void *data, i32);

void SerialiseStringAddr(EdStream &stream, void *data, i32 count);

void FS_BuildFilterBlocks(char *input);

void FS_MakeDateTimeString(FS_FILEENTRYHDR *entry, char *output);

void FS_BuildFilterOutBlocks(char *input);

void FS_SetCursorToLastFileName();

i32 ProcessFileSel3(f32 elapsed, nupad_s *pad);

extern "C" i32 ProcessFileSel2(f32 elapsed, nupad_s *pad);

extern "C" void FileSelKill(void);

void ProcessFileSel(f32 elapsed, nupad_s *pad);

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

extern "C" void RenderFileSel2(i32 x, i32 y, i32 *width, i32 *height);

i32 LoadState(i32, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *);

extern "C" {
extern void (*savesuccessfn)();
extern void * memcard_headerdata;
extern i32 memcard_headerdatasize;
extern void * memcard_headerdatabuffer;

void FS_SetFileSelPathFromName(char *name);

void SaveSystemInitialiseEx(i32 slots, void *makeSaveHash, void *save, i32 saveSize, void *header, i32 headerSize,
                                i32 autosave, void (*drawSaveIcon)(void));

void SetSaveSuccessFn(void (*callback)(void));

void UpdateSaveSlots(void);

void loadsaveCallEachFrame(void);

i32 TriggerAutoSave(void);

} // extern "C"

// ==== src/legoapi/render/core/render.cpp: declarations ====
#include "decomp.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/render/core/render.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/character/transform.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/actions/character/snake.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/core/gamehint.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/arcade.h"
#include "legoapi/render/light/fade_material.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/android/nuportal_android.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/cutscenes/cutscenes.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

void DrawSubItems();

extern "C" void edbriStartAllPages(void);

#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nutrig.h"

extern "C" void AiRndrLine3dDbg(f32, f32, f32, f32, f32, f32, u32);

extern "C" void edDrawCross(NUVEC *centre, u8 red, u8 green, u8 blue);

extern "C" void LocaledbitsDrawSolidEllipseXY(NUVEC *centre, f32 radius_x, f32 radius_z, i32 rotation, f32 lower_y,
                                              f32 upper_y, u32 colour, i32, i32 segments);

extern "C" void LocaledbitsDrawCircleXY(NUVEC *centre, f32 radius, u32 colour, i32, i32 segments);

extern "C" void LocaledbitsDrawSolidCircleXY(NUVEC *centre, f32 radius, f32 lower_y, f32 upper_y, u32 colour, i32,
                                             i32 segments);

extern "C" void RndrOSquare(NUVEC *centre, f32 radius, i32 colour);

extern "C" void RndrOSphere(NUVEC *centre, f32 radius, i32 colour, i32 segments, i32);

extern "C" void RndrCircleXZ(NUVEC *centre, f32 radius, i32 colour, i32 segments);

#include "gameapi/gui/apimenu.h"
#include "gameframework/saveload.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/world/level.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world_shared.h"
#include "globals.h"
#include "nu2api/nu3d/nutexanm.h"
struct starfighter_s;
struct rtl_s;
struct rtlidata_s;

#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/numath.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

#include <string.h>

extern i32 VehicleArea;
extern f32 FORCEGLOWTIME;
extern i16 tHINTS;
extern i16 tCHARACTERS;
extern i16 tEXTRAS;
extern i16 tGOLDBRICKS;
extern i16 tSTORYCLIPS;
extern i16 tENTERCODE;
i32 AnakinGreenSabre(GameObject_s *object);
i32 MatrixReflection(NUMTX *matrix, i32 axis, f32 plane, f32 height, NUMTX *result);

extern f32 HUB_EPISODESUBTITLEY;
extern f32 HUB_EPISODETITLEY;
extern i32 draw_para;

void DrawGameObjectsDraw(i32 pass);
void EnableShadowMapRendering(i32 enable);
void ResetShadowMapRendering();
static void DrawParaphernalia(GameObject_s *object);

struct GAMEMESSAGE_s;
struct HINT_s;
struct MENU_s;
struct NuBloomParameters;
struct STATUSPACKET_s;
struct TouchHolder;
struct _vum_s;
struct _vuv_s;
struct nuhspecial_s;
struct ripple_node_s;
struct ripple_set_s;
struct VuVec;

extern nuhspecial_s *(*GameMsg_GetExtraObjFn)(GAMEMESSAGE_s *);

extern "C" i32 StateAnimEvaluate2(StateAnim *state, u8 *index, char *value, f32 frame);
extern "C" i32 NuRndrBeginScene(i32);
extern "C" void NuRndrEndScene(void);
extern "C" void NuRndrGradRect2di(i32, i32, i32, i32, i32 *, numtl_s *);
extern "C" void NuRndrRect2di(i32, i32, i32, i32, i32, numtl_s *);
extern "C" void NuCameraUnlock(void);
extern "C" void NuCameraRelock(void);
extern char *apitxt_EMPTY;
extern char *apitxt_PRESENT;
extern char *apitxt_AUTOSAVE_WARNING;
extern char *apitxt_LOADING;
extern char *apitxt_SAVING;
extern char *apitxt_OK;
extern char *apitxt_GAMELOADED;
extern char *apitxt_GAMELOADING;
extern char *apitxt_FAILEDTOLOAD;
extern char *apitxt_CORRUPTLOAD;
extern char *ASCII_DOWN;
extern char *ASCII_UP;
extern i16 tCURRENTGAME;
extern i16 tEMPTY;
extern i16 tGAME;
extern i16 tNOSPACE;
extern f32 MENUTEXTSCALE;
extern f32 MENUBOTY;
extern f32 AUTOSAVEICONY;
extern f32 AUTOSAVEICONX;
extern f32 AUTOSAVEICONSIZE;
extern f32 MenuAlpha;
extern i32 MenuA;
extern u8 MENUNORMALR;
extern u8 MENUNORMALG;
extern u8 MENUNORMALB;
extern u8 MENUENTRYR, MENUENTRYG, MENUENTRYB;
extern u8 MENUFLASH0R;
extern u8 MENUFLASH0G;
extern u8 MENUFLASH0B;
extern u8 MENUFLASH1R;
extern u8 MENUFLASH1G;
extern u8 MENUFLASH1B;
extern f32 menu_pulse;
extern f32 menu_pulsate;
extern f32 cointotaltime;
extern "C" i32 screendump;
extern f32 pause_fade;
extern "C" i32 PANELOFF;
extern i32 noscenespecials;
extern void RotateGameMatrix(numtx_s *matrix, i32 order, u16 x, u16 y, u16 z);
extern void SetLevelLights(void *set, f32 scale);
extern f32 ICONX;
extern f32 ICONSIZE;
extern f32 DROPINALPHA;
extern f32 statstime;
void Hint_Draw(i32 player_index);
extern i32 CutScenePlayer_CanStart(i32 cutscene_id);

// Camera zoom state
extern f32 CameraZoom;

// Graphics loading flags
extern i32 RemoveDirectionalMaps;
extern i32 RemoveNormalMaps;

extern NUVIDEORESHEADER g_VideoResHeader;

extern "C" {
    i32 NuDisplayListRndrSpecial(nuhspecial_s *special, NUMTX *mtx, i32 skinned, NUMTX *skin_mtx,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
}

void SetCameraZoom(f32 zoom);

extern "C" void NuGScnUpdate(NUGSCN *gscn, f32 frame_delta);

// --- NuGScn graphics-data reader ---
// NuReadGraphicsData is a C++ static function in the original (GCC clones it,
// hence the `.isra.NNN` suffix in the ROM symbol table).

extern "C" void NuSevereWarning(const char *format, ...);

// Retail only keeps the IPA-SRA clone, which drops the unused fourth argument.
#if defined(__i386__)
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone))
#endif
static NU_READ_GRAPHICS_DATA_ATTR NUGSCN *NuReadGraphicsData(VARIPTR *buf, VARIPTR *buf_end, char *path,
                                                             char *scene_data)
    __asm__("_ZL18NuReadGraphicsDataP9variptr_uS0_PcS1_S1_.isra.104");

// --- Extern "C": NuGScn functions have C linkage in original ---
extern "C" {
NUGSCN *NuGScnReadForMultiRender(VARIPTR *buf, VARIPTR buf_end, char *path, i32 render_count);

NUGSCN *NuGScnRead(VARIPTR *buf, VARIPTR buf_end, char *path);
void NuGScnReadFromMemory(NUGSCN *scene);
NUGSCN *NuGHGFixup(NUGSCN *scene, void *);
nuhgobj_s *NuGHGRead(char *path, VARIPTR *buf, VARIPTR buf_end);
} // extern "C"

extern i32 solid_cable;
extern i32 nsegments_drawn;
extern f32 slack_factor;
extern f32 cable_slack;
extern f32 nsegments_per_unit;
extern CABLE_s cables[8];
extern f32 tow_length;
extern NUMTL *SolidMtl3D;

void DrawCables();

void DrawRipple(ripple_node_s *node);

void DrawAreaBox(nuvec_s *position, nuvec_s *size, i32 rotation, i32 colour);

void DrawBox_Now(_vuv_s *, _vuv_s *, i32, i32);

extern "C" {
extern f32 aiEditor_DrawYOffset;
extern f32 aiEditor_LocatorWidth;
    void AiRndrLine3d(NURND_VERTEX3D *, NUMTL *, NUMTX *);
}

void DrawLocator(nuvec_s *position, float radius, i32 rotation, i32 colour);

void Draw_LOADED();

i32 Draw3DObject(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                 u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order);

void DrawCharIcon(i32 character_id, float x, float y, float z, float scale, i32 frame_object_id, float character_alpha,
                  float frame_alpha, i32 draw_character, nuhspecial_s *override_special);

extern const u8 HintRGB[5][3];

void DrawLine_Now(_vuv_s *, _vuv_s *, i32, i32);

void DrawParallax(nuhspecial_s *special);

void DrawQuestion(nuvec_s *position, float scale_value, float y_push);

void DrawRectRGBA(float x, float y, float width, float height, u32 colour, numtl_s *material, i32 alignment,
                  float scale);

void DrawItem(nuhspecial_s *special, nuvec_s *position, float scale_value, float unused, float y_push, u16 x_rot,
              u16 y_rot, u16 z_rot);


void Draw_LOADING();

#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nupostparams.h"

#define ALPHA_GRID_VERTEX(vx, vy)                                                                                      \
    do {                                                                                                               \
        direction.x = (vx);                                                                                            \
        direction.y = -(vy) * camera.aspect;                                                                           \
        direction.z = adjacent;                                                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        NuVecMtxTransform(&direction, &direction, &camera.mtx);                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        brightness = direction.y * 0.5f + 0.5f;                                                                        \
        if (brightness <= near_angle)                                                                                  \
            brightness = near_scale * parameters->intensity;                                                           \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale * parameters->intensity;                                                            \
        else                                                                                                           \
            brightness = ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity; \
        if (parameters->directional) {                                                                                 \
            f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;       \
            if (angle < parameters->direction_near_angle) {                                                            \
                brightness += 128.0f * parameters->direction_far_scale;                                                \
            } else if (!(angle > parameters->direction_far_angle)) {                                                   \
                f32 blend = NuPowFast((angle - parameters->direction_near_angle) /                                     \
                                          (parameters->direction_far_angle - parameters->direction_near_angle),        \
                                      parameters->direction_bias);                                                     \
                brightness +=                                                                                          \
                    128.0f * ((1.0f - blend) * (parameters->direction_far_scale - parameters->direction_near_scale) +  \
                              parameters->direction_near_scale);                                                       \
            }                                                                                                          \
        }                                                                                                              \
        u32 colour = (static_cast<u32>(static_cast<u8>(128)) << 24) |                                                  \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 16) |                   \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 8) |                    \
                     static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness))));                            \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] =                                                                     \
                ((static_cast<i32>(colour) >> 1) & 0x7f7f7f) | (colour & 0xff000000);                                  \
        else                                                                                                           \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = colour;                                                             \
        NuPrim2DAddXYZ((vx), (vy), 0.0f);                                                                              \
    } while (0)

void DrawAlphaGrid(i32 rows, i32 cols, NuBloomParameters *parameters);

void DrawArrow_Now(_vum_s *, float, i32, i32);

extern i16 tNONEWBESTTIME, tNEWBESTTIME;
void Text_MakeTime(f32, i32, i32, i32, char *);

void DrawBonusTime(STATUSPACKET_s *packet, float position, i32 alpha);

void DrawCross_Now(_vuv_s *, float, i32, i32);

void DrawPauseFade();


void DrawRippleSet(ripple_set_s *set);



void ClearScreen();



void DrawAlphaImage(i32 rows, i32 cols, numtl_s *material, i32 use_pixel_offsets, NuBloomParameters *parameters);

void DrawBezierLine(VuVec &start, VuVec &start_control, VuVec &end, VuVec &end_control, numtl_s *material, i32 colour);

void DrawBoxMtx_Now(_vum_s *, _vuv_s *, i32, i32);

void *AddGameMessage(char *, NUVEC *, f32, NUVEC *, f32, u8, u8, u8, u32, f32);

void DrawCutBorders(i32 widescreen);

void DrawExplosions();

void DrawItemMenu2D();

void DrawStatusText(char *text, u16 angle, float x, float y, float scale, u32 colour, i32 alignment);

i32 Draw3DObjectMtx(WORLDINFO_s *world, i32 object_index, numtx_s *mtx);

void DrawGameObjects();

extern nuhspecial_s green_light;
extern nuhspecial_s painttargetcolour[3];
extern nuhspecial_s paintlights[3];

void DrawPaintLights();

void DrawStatusIcons(STATUSPACKET_s *status, float y, float alpha);

void DrawTouchPrompt(char *prompt, char *unused_label, bool hovered, bool large);

void Draw_LOADFAILED();

void DrawAreaCylinder(nuvec_s *centre, nuvec_s *size, i32 colour);

void DrawCameraTarget(nuvec_s *);

void DrawGameMessages();

extern f32 KITPOSX;
extern f32 KITPOSY;
extern f32 KITPOS2X;
extern f32 KITPOS2Y;
extern f32 PANEL_MINIKITSCALE;
extern f32 PANEL_REDBRICKSCALE;
extern f32 PANEL_MINIKITY;
extern f32 PANEL_MINIKITCOUNTSCALE;
extern f32 PANEL_MINIKITCOUNTY;
// DrawPanel HUD globals referenced by the cantina-bar-patrons WIP. Initial
// red-brick positions come from the Game init values in legogame/game.cpp.
extern i32 DRAWBGLOAD;
extern u16 PowerUp_PanelYRot[2];
extern f32 PowerUp_PanelPosMul[2];
extern f32 POWERUPOBJSIZE;
extern f32 REDBRICKPOSX;
extern f32 REDBRICKPOSY;
extern f32 REDBRICKPOS2X;
extern f32 REDBRICKPOS2Y;
extern i32 Arcade_Points[2];
extern f32 BOSSICONY;
extern i32 FPSDISPLAY;
extern i32 ShowPlayerCoordinate;
extern i32 LEGOOBJ_CHARKIT;
extern i32 LEGOOBJ_MINIKIT;
i32 DrawPanel3DObjectNoAlpha(f32, f32, f32, f32, f32, f32, u16, u16, u16, nuhspecial_s *, i32);
extern "C" void Text3D(char *, f32, f32, f32, f32, f32, f32, u32, u8, u8, u8);

void DrawMiniKitCount(float position, float scale, i32 count, i32 maximum);

void DrawStatusBG_LSW(STATUSPACKET_s *status);

void Draw_LOADCORRUPT();

i32 Draw3DObjectAlpha(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                      u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order, float alpha);

void DrawCameraTarget2(nuvec_s *);

i32 DrawPanel3DObject(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                      u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order, float alpha);

extern STATUSMINIKITPART_s KitPart[10];
extern i32 currentminikit;
extern f32 slideseek;

i32 DrawPanel3DObjectMtxNoAlpha(nuhspecial_s *special, numtx_s *matrix);
void DrawStatusMiniKit(float x, float y, float z, float built_scale, float new_scale, i32 count, STATUSPACKET_s *packet,
                       float slide);

extern i16 tUNKNOWN, tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern "C" void PlaySfx(char *, NUVEC *);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);

void DrawSubItemMenu2D();

void DrawSubItemMenu3D();


void DrawFadeScreenWipe();


void DrawForceGlowSprite(nuvec_s *position, float radius, i32 model, float alpha, GameObject_s *);

void DrawGameObjectsDraw(i32);

void Draw_AUTOSAVECANCEL();

void DrawPanel3DObjectMtx(nuhspecial_s *special, numtx_s *matrix, float alpha);



extern i32 dco_locatorposonly;
extern i32 dco_id;
extern i32 dco_reflectaxis;
extern i32 dco_wearinghat;
extern u16 dco_prerotatez;
extern f32 dco_reflectcoord;
extern GAMECHARACTERDATA_s * dco_gcdata;
extern CHARACTERMODEL_s * dco_cmodel;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);
void DrawObjectOnCharacter(WORLDINFO_s *world, GameObject_s *object, i32 object_id, nuhspecial_s *special, i32 locator,
                           i32 second_locator, NUMTX *joints, i32 reflect, u32 layers, NUMTX *rotation,
                           NUVEC *translation, f32 alpha, f32 scale);

extern f32 DropInOutScale(GameObject_s *object);
extern void ApplyExtraRotation(GameObject_s *object, NUMTX *matrix);
extern AREADATA *DEATHSTARBATTLE2_ADATA;
extern AREADATA *PODSPRINT_ADATA;
extern "C" {
    extern i16 id_ATST;
    extern i16 id_ATST_LOWRES;
    extern i16 id_ATAT;
}
extern u8 CharClipToBlobShadows;


i32 DrawGameObjectsProcess();

void DrawStatusTextFraction(i32 current, i32 total, float x, float y, u16 angle, float scale, u32 colour,
                            float animation, float animation_duration);

void DrawGameMessage_Targets(GAMEMESSAGE_s *message, nuvec_s *position, float scale);

i32 DrawPanel3DObjectNoAlpha(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                             u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order);


extern void DrawMenuEntry(MENU_s *, char *);

void DrawAABox(_vuv_s *position, _vuv_s *size, i32 colour);

void DrawArrow(nuhspecial_s *special, float scale_value);

void DrawCross(nuvec_s *centre, float radius, numtl_s *material, i32 colour);

extern f32 SwipeDecalRenderer_MaxWidth;
extern f32 SwipeDecalRenderer_MinWidth;


#define SWIPE_PRIM_UV(U, V)                                                                                            \
    do {                                                                                                               \
        PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);                      \
        if (!g_NuPrim_NeedsHalfUVs) {                                                                                  \
            vertex->float_uv[0] = (U);                                                                                 \
            vertex->float_uv[1] = (V);                                                                                 \
        } else {                                                                                                       \
            vertex->half_uv[0] = NuRndrFloatToHalf(U);                                                                 \
            vertex->half_uv[1] = NuRndrFloatToHalf(V);                                                                 \
        }                                                                                                              \
    } while (false)




extern i32 GetMenuID(void);





void CharScene_Draw(WORLDINFO_s *, i32, NUMTX *, NUMTX *);
void CharMiniKit_Draw(i32, NUMTX *, i32, f32, f32);
void Customiser_DrawAccessories(CUSTOMISER *, GameObject_s *, NUMTX *);
i32 Batarang_GetObjectFromCharID(i32);
void Grapple_DrawLine(GameObject_s *);
void Transform_DrawTarget(NUVEC *, f32, f32);
extern i16 id_ANAKINJEDISCARRED, id_WEIRDO1, id_WEIRDO2, id_CATAPULT, id_CHEWBACCA, id_WOOKIEE, id_TWOFACE;
extern i32 PickUpFlickerTest, PickUpFlickerFrames, PickupFlickerFrame;


#include "legoapi/legoapi_types.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nurndr.h"

extern i32 qrand(void);
extern f32 SeekLinearF(f32, f32, f32);




extern f32 backdrop_top_r;
extern f32 backdrop_top_g;
extern f32 backdrop_top_b;
extern f32 backdrop_bot_r;
extern f32 backdrop_bot_g;
extern f32 backdrop_bot_b;


extern i32 backdrop_black;

extern void (*BackDrop_AlphaFn)(float *);


void BackDrop_Init(char *path, variptr_u *buf, variptr_u *buf_end);

void BackDrop_Dump();

void BackDrop_Update(float dt);

void BackDrop_ResetColours();

void BackDrop_UpdateColours(i32 instant);

void BackDrop_Draw(float alpha, i32 flags);

extern "C" {
    extern nurenderscene_s currentScene;
void DisplaySceneRndrSpecials(NUDLDLISTSCENE *scene, i32, void *visibility_context);
}

// ==== functions in original address order ====

extern "C" {
    static __used__ void cbMMRegSel(eduimenu_s *, eduiitem_s *selected, u32) {
        ed_curr = static_cast<ed_module_s *>(selected->data_ptr);
        if (__builtin_expect(!ed_curr->reserved, 1)) {
            ed_module_active = 1;
            if (ed_curr->activate)
                ed_curr->activate();
        }
    }

    static __used__ i32 eduicbInteractSel(edui_interact_s *interact) {
        eduimenu_s *menu = interact->menu;
        edui_sel_s *item = static_cast<edui_sel_s *>(interact->item);
        if (interact->callback) {
            if (edui_cursor_buttons_db & EDUI_CURSOR_PRIMARY)
                item->selected(menu, item, edui_cursor_buttons);
            else if ((edui_cursor_buttons & EDUI_CURSOR_PRIMARY) && item->held)
                item->held(menu, item, edui_cursor_buttons);
        }
        return 0;
    }

    static __used__ i32 eduicbProcessTexturePick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad) {
        edui_texture_pick_s *pick = static_cast<edui_texture_pick_s *>(item);
        i32 stick_x = static_cast<i32>(pad->analog_left_x) - 128;
        i32 stick_y = static_cast<i32>(pad->analog_left_y) - 128;
        if (stick_x > 0) {
            if (stick_x < 32)
                stick_x = 0;
            else
                stick_x = (stick_x - 32) * 255 / 223;
        } else if (stick_x > -32)
            stick_x = 0;
        else
            stick_x = (stick_x + 32) * 255 / 223;
        if (stick_y > 0) {
            if (stick_y < 32)
                stick_y = 0;
            else
                stick_y = (stick_y - 32) * 255 / 223;
        } else if (stick_y > -32)
            stick_y = 0;
        else
            stick_y = (stick_y + 32) * 255 / 223;
        f32 x = static_cast<f32>(stick_x);
        f32 y = static_cast<f32>(stick_y);
        i32 corner = pick->selected_corner;
        x = pick->uv_x[corner] + x / (8192.0f * pick->zoom);
        y = pick->uv_y[corner] + y / (8192.0f * pick->zoom);
        x = MIN(1.0f, (MAX(0.0f, x)));
        y = MIN(1.0f, (MAX(0.0f, y)));
        pick->uv_x[corner] = x;
        pick->uv_y[corner] = y;
        i32 left = pad->analog_l1 ? pad->analog_l1 : pad->analog_l2;
        i32 right = pad->analog_r1 ? pad->analog_r1 : pad->analog_r2;
        if (__builtin_expect(!!(left), 0)) {
            f32 zoom = pick->zoom - left / 4192.0f;
            if (!(zoom < 1.0f)) {
                pick->zoom = zoom;
            } else {
                pick->zoom = 1.0f;
            }
            return 1;
        }
        if (__builtin_expect(!!(right), 1)) {
            f32 zoom = pick->zoom + right / 4192.0f;
            pick->zoom = zoom > 16.0f ? 16.0f : zoom;
            return 1;
        }
        if (pad->digital_buttons_pressed & 0x100) {
            pick->uv_x[corner] = static_cast<i32>(pick->uv_x[corner] * 256.0f + 0.5f) * (1.0f / 256.0f);
            pick->uv_y[corner] = static_cast<i32>(pick->uv_y[corner] * 256.0f + 0.5f) * (1.0f / 256.0f);
        }
        if (pad->digital_buttons_pressed & 0x80)
            pick->selected_corner = corner != 1;
        if ((pad->digital_buttons_pressed & 0x40) && pick->changed)
            pick->changed(menu, item, pad->digital_buttons);
        return 0;
    }

    static __used__ i32 eduicbProcessSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad) {
        static i32 rep_timer = 30;
        static f32 accel = 1.0f;
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        if ((pad->digital_buttons & 0x0c) == 0x0c) {
            f32 value;
            if (slider->minimum <= 0.0f && slider->minimum + slider->range >= 0.0f)
                value = 0.0f;
            else if (slider->minimum <= 1.0f && slider->minimum + slider->range >= 1.0f)
                value = 1.0f;
            else {
                rep_timer = 30;
                return 0;
            }
            slider->value = value;
            slider->normalized_value = (value - slider->minimum) / slider->range;
            slider->change_timer = 60;
            if (slider->changed)
                slider->changed(menu, item, pad->digital_buttons);
            rep_timer = 30;
        } else if (pad->digital_buttons & 0x04) {
            if (rep_timer > 0)
                --rep_timer;
            else
                rep_timer = 0;
            if (!rep_timer || (pad->digital_buttons_pressed & 0x04)) {
                f32 step = slider->granularity * accel;
                f32 value = slider->value - step;
                if (value < slider->minimum)
                    value = slider->minimum;
                value = static_cast<i32>((value > 0.0f ? value + step * 0.5f : value - step * 0.5f) / step) * step;
                slider->value = value;
                slider->normalized_value = (value - slider->minimum) / slider->range;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
                accel += slider_acceleration;
            }
        } else if (pad->digital_buttons & 0x08) {
            if (rep_timer > 0)
                --rep_timer;
            else
                rep_timer = 0;
            if (!rep_timer || (pad->digital_buttons_pressed & 0x08)) {
                f32 step = slider->granularity * accel;
                f32 value = slider->value + step;
                if (value > slider->minimum + slider->range)
                    value = slider->minimum + slider->range;
                value = static_cast<i32>((value > 0.0f ? value + step * 0.5f : value - step * 0.5f) / step) * step;
                slider->value = value;
                slider->normalized_value = (value - slider->minimum) / slider->range;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
                accel += slider_acceleration;
            }
        } else {
            rep_timer = 30;
            accel = 1.0f;
        }
        return 0;
    }

    static __used__ i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *) {
        return 0;
    }

    static __used__ void eduicbItemDestroyExpander(eduimenu_s *menu, eduiitem_s *item) {
        edui_expander_s *expander = static_cast<edui_expander_s *>(item);
        if (expander->last_child)
            expander->last_child->next = NULL;
        while (expander->first_child) {
            eduiitem_s *next = expander->first_child->next;
            expander->first_child->destroy(menu, expander->first_child);
            expander->first_child = next;
        }
    }

    static __used__ void cbSel(eduimenu_s *, eduiitem_s *item) {
        selection_value = item->data;
    }

    static __used__ void cbGradChange(void) {
    }

    i32 cbInteractMenuTitle(edui_interact_s *interact) {
        static f32 offsetx;
        static f32 offsety;
        i32 result = (edui_cursor_buttons & EDUI_CURSOR_PRIMARY) != 0;
        eduimenu_s *menu = interact->menu;
        if (!(menu->flags & 1)) {
            if (!eduiInteractLocked) {
                offsetx = 0.0f - menu->x;
                offsety = 0.0f - menu->y;
            }
            menu->x = static_cast<i32>(0.0f - offsetx);
            menu->y = static_cast<i32>(0.0f - offsety);
        }
        return result;
    }

    i32 cbInteractMenuScrollUp(edui_interact_s *interact) {
        eduimenu_s *menu = interact->menu;
        if (menu->field_0c) {
            if (menu->field_0c->previous)
                menu->field_0c = menu->field_0c->previous;
            menu->selected = menu->field_0c;
        }
        return 0;
    }

    i32 cbInteractMenuScrollDown(edui_interact_s *interact) {
        eduimenu_s *menu = interact->menu;
        if (menu->field_10) {
            if (menu->field_10->next)
                menu->field_10 = menu->field_10->next;
            menu->selected = menu->field_10;
        }
        return 0;
    }

    static __used__ i32 cbInteractMenuCancelChild(edui_interact_s *interact) {
        eduimenu_s *menu = interact->menu;
        if (menu && menu->child && menu->child->callback)
            menu->child->callback(menu->child, menu);
        return 0;
    }

}

static __used__ void cbCopy(eduimenu_s *, eduiitem_s *, u32) {
    f32 red = *cp_r;
    f32 green = *cp_g;
    f32 blue = *cp_b;
    clipboard_r = red;
    clipboard_g = green;
    clipboard_b = blue;
    i32 red_byte = static_cast<i32>(red * 255.0f) & 0xff;
    i32 green_byte = static_cast<i32>(green * 255.0f) & 0xff;
    i32 blue_byte = static_cast<i32>(blue * 255.0f) & 0xff;
    u32 colour = 0x80000000u | red_byte;
    colour |= green_byte << 8;
    colour |= blue_byte << 16;
    cp_paste->colours[2] = colour;
}

extern "C" {
    void eduicbItemDestroyProp(eduimenu_s *menu, eduiitem_s *item) {
        edui_prop_s *prop = static_cast<edui_prop_s *>(item);
        if (prop->text) {
            NU_FREE(prop->text);
            prop->text = NULL;
        }
        if (prop->property_text) {
            NU_FREE(prop->property_text);
            prop->property_text = NULL;
        }
        NU_FREE(prop);
        menu->field_0c = menu->first;
    }

    static __used__ void eduicbItemDestroyFilter(eduimenu_s *menu, eduiitem_s *item) {
        edui_filter_s *filter = static_cast<edui_filter_s *>(item);
        while (filter->first_child) {
            eduiitem_s *next = filter->first_child->next;
            filter->first_child->destroy(menu, filter->first_child);
            filter->first_child = next;
        }
        eduicbItemDestroyProp(menu, item);
    }

    void eduicbItemDestroy(eduimenu_s *menu, eduiitem_s *item) {
        edui_interact_s *interact = eduiInteracts;
        for (i32 i = 0; i < numInteracts; ++i, ++interact) {
            if (menu == interact->menu && item == interact->item) {
                interact->menu = NULL;
                interact->item = NULL;
            }
        }
        if (item->text) {
            NU_FREE(item->text);
            item->text = NULL;
        }
        NU_FREE(item);
        menu->field_0c = menu->first;
    }

    static __used__ void eduicbItemSliderDestroy(eduimenu_s *menu, eduiitem_s *item) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        if (slider->format) {
            NU_FREE(slider->format);
            slider->format = NULL;
        }
        eduicbItemDestroy(menu, item);
    }

    static __used__ void eduicbItemFilePickDestroy(eduimenu_s *menu, eduiitem_s *item) {
        edui_file_pick_s *file_pick = static_cast<edui_file_pick_s *>(item);
        if (file_pick->format) {
            NU_FREE(file_pick->format);
            file_pick->format = NULL;
        }
        eduicbItemDestroy(menu, item);
    }

    static __used__ void eduicbItemTextPickDestroy(eduimenu_s *menu, eduiitem_s *item) {
        edui_textpicker_s *text_pick = static_cast<edui_textpicker_s *>(item);
        if (text_pick->format) {
            NU_FREE(text_pick->format);
            text_pick->format = NULL;
        }
        eduicbItemDestroy(menu, item);
    }

}

static __used__ void eduiFntPrintExGeneric(void *font, int x, int y, int alignment, char *format, ...) {
    if (edui_donotdraw) {
        return;
    }
    char text[1024] __attribute__((aligned(16)));
    NuQFntPushPrintMode(2);
    va_list arguments;
    va_start(arguments, format);
    NuVSPrintf(text, format, arguments);
    va_end(arguments);
    i32 width = static_cast<i32>(NuQFntPrintLenU(font, text));
    i32 left = x;
    switch (alignment) {
    case 32:
        left = x - width;
        break;
    case 64:
        left = x - width / 2;
        break;
    }
    NuQFntMove(font, left, y, 0.0f);
    NuQFntPrintU(font, text);
    NuQFntPopPrintMode();
}

static __used__ void eduiFntPrintClipEx(void *font, float x, float y, int alignment, float clip_x, float clip_width,
                                        char *format, ...) {
    va_list arguments;
    if (__builtin_expect(!edui_donotdraw, 1)) {
        char text[1024];
        i32 screen_x = static_cast<i32>(x * 16.0f);
        i32 screen_y = static_cast<i32>(y * 8.0f);
        i32 screen_clip_x = static_cast<i32>(clip_x * 16.0f);
        i32 screen_clip_width = static_cast<i32>(clip_width * 16.0f);
        NuQFntPushPrintMode(2);

        va_start(arguments, format);
        NuVSPrintf(text, format, arguments);
        va_end(arguments);
        i32 width = static_cast<i32>(NuQFntPrintLenU(font, text));
        if (alignment == 32)
            screen_x -= width;
        else if (alignment == 64)
            screen_x -= width / 2;
        if (screen_x < screen_clip_x)
            screen_x = screen_clip_x;
        i32 length = NuStrLen(text);
        if (width > screen_clip_width && length > 1) {
            char *end = text + length - 1;
            do {
                *end-- = '\0';
                width = static_cast<i32>(NuQFntPrintLenU(font, text));
            } while (width > screen_clip_width && end != text);
        }
        NuQFntMove(font, screen_x, screen_y, 0.0f);
        NuQFntPrintU(font, text);
        NuQFntPopPrintMode();
    }
}

extern "C" {
    static __used__ i32 eduicbProcessGraph(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad) {
        edui_graph_s *graph_item = static_cast<edui_graph_s *>(item);
        show_y_scale_help = show_y_scale_help > 1 ? show_y_scale_help - 1 : 0;
        show_x_scale_help = show_x_scale_help > 1 ? show_x_scale_help - 1 : 0;
        f32 x = eduiPickerDeadZone32(pad->analog_left_x) * (1.0f / 8192.0f);
        f32 y = -eduiPickerDeadZone32(pad->analog_left_y) * (1.0f / 8192.0f);
        x += graph_item->cursor_x;
        y += graph_item->cursor_y;
        x = MIN(1.0f, (MAX(0.0f, x)));
        y = MIN(1.0f, (MAX(0.0f, y)));
        graph_item->cursor_x = x;
        graph_item->cursor_y = y;

        u32 left = pad->analog_l2;
        u32 right = pad->analog_r2;
        if (left) {
            if (right) {
                show_x_scale_help = 60;
                graph_item->graph->x_scale = 1.0f;
                return 1;
            }
            if (left > 55) {
                show_x_scale_help = 60;
                f32 scale = graph_item->graph->x_scale - left * 0.02f / 255.0f;
                graph_item->graph->x_scale = scale < 0.01f ? 0.01f : scale;
                return 1;
            }
        } else if (__builtin_expect(right > 55, 1)) {
            show_x_scale_help = 60;
            graph_item->graph->x_scale = right * 0.02f / 255.0f + graph_item->graph->x_scale;
            return 1;
        }
        left = pad->analog_l1;
        right = pad->analog_r1;
        if (left) {
            if (__builtin_expect(!!(right), 0)) {
                show_y_scale_help = 60;
                graph_item->graph->y_scale = 1.0f;
                return 1;
            }
            if (__builtin_expect(!!(left), 0)) {
                show_y_scale_help = 60;
                f32 scale = graph_item->graph->y_scale - left * 0.02f / 255.0f;
                graph_item->graph->y_scale = scale < 0.01f ? 0.01f : scale;
                return 1;
            }
        } else if (right) {
            show_y_scale_help = 60;
            graph_item->graph->y_scale = right * 0.02f / 255.0f + graph_item->graph->y_scale;
            return 1;
        }
        u32 pressed = pad->digital_buttons_pressed;
        if ((pressed & 0x80) && graph_item->selected_point >= 0) {
            nugraphDeletePoint(graph_item->graph, graph_item->selected_point);
            nugraphCalcCurve(graph_item->graph, 100);
            return 1;
        }
        if (pressed & 0x20) {
            nugraphAddPoint(graph_item->graph, graph_item->cursor_x, graph_item->cursor_y);
            nugraphCalcCurve(graph_item->graph, 100);
            return 1;
        }
        if (__builtin_expect((pressed & 0x40) && graph_item->selected_point >= 0, 0)) {
            graph_item->cursor_x = graph_item->graph->x[graph_item->selected_point];
            graph_item->cursor_y = graph_item->graph->y[graph_item->selected_point];
        }
        if ((pad->digital_buttons & 0x40) && graph_item->selected_point >= 0) {
            i32 index = graph_item->selected_point;
            nugraph_s *graph = graph_item->graph;
            graph->x[index] = graph_item->cursor_x;
            graph->y[index] = graph_item->cursor_y;
            if (index == 0) {
                graph->x[index] = 0.0f;
            } else if (index == graph->point_count - 1) {
                graph->x[index] = 1.0f;
            } else if (graph->x[index] <= graph->x[index - 1]) {
                graph->x[index] = graph->x[index - 1] + 0.01f;
            } else if (graph->x[index] >= graph->x[index + 1]) {
                graph->x[index] = graph->x[index + 1] - 0.01f;
            }
            nugraphCalcCurve(graph, 100);
            return 1;
        }
        if ((pressed & 0x10) && graph_item->changed)
            graph_item->changed(menu, item, pressed);
        return 0;
    }

    static __used__ i32 eduicbProcessGreyPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad) {
        edui_colour_pick_s *pick = static_cast<edui_colour_pick_s *>(item);
        i32 left = pad->analog_l1 ? pad->analog_l1 : pad->analog_l2;
        i32 right = pad->analog_r1 ? pad->analog_r1 : pad->analog_r2;
        if (left) {
            f32 value = pick->value - left / 4192.0f;
            pick->value = value < 0.0f ? 0.0f : value;
            return 1;
        }
        if (right) {
            f32 value = pick->value + right / 4192.0f;
            pick->value = value > 1.0f ? 1.0f : value;
            return 1;
        }
        pick->saturation = 0.0f;
        pick->hue = 0.0f;
        if ((pad->digital_buttons_pressed & 0x40) && pick->changed)
            pick->changed(menu, item, pad->digital_buttons);
        return 0;
    }

}

static void HSV2RGBBody(f32 hue, f32 saturation, f32 value, f32 *red, f32 *green, f32 *blue) {
    if (hue == 360.0f)
        hue = 0.0f;
    hue /= 60.0f;
    i32 sector = static_cast<i32>(NuFloor(hue));
    f32 fraction = hue - sector;
    f32 low = (1.0f - saturation) * value;
    f32 falling = (1.0f - fraction * saturation) * value;
    f32 rising = (1.0f - (1.0f - fraction) * saturation) * value;
    switch (sector) {
    case 0:
        *red = value;
        *green = rising;
        *blue = low;
        break;
    case 1:
        *red = falling;
        *green = value;
        *blue = low;
        break;
    case 2:
        *red = low;
        *green = value;
        *blue = rising;
        break;
    case 3:
        *red = low;
        *green = falling;
        *blue = value;
        break;
    case 4:
        *red = rising;
        *green = low;
        *blue = value;
        break;
    case 5:
        *red = value;
        *green = low;
        *blue = falling;
        break;
    }
}

static void eduiFntPrintExLeft(void *font, int x, int y, int, char *format, ...) {
    if (edui_donotdraw) {
        return;
    }
    char text[1024] __attribute__((aligned(16)));
    NuQFntPushPrintMode(2);
    va_list arguments;
    va_start(arguments, format);
    NuVSPrintf(text, format, arguments);
    va_end(arguments);
    NuQFntPrintLenU(font, text);
    NuQFntMove(font, x, y, 0.0f);
    NuQFntPrintU(font, text);
    NuQFntPopPrintMode();
}

static void eduiFntPrintExEditBuffer(void *font, float x, float y, int, char *format, ...) {
    va_list arguments;
    va_start(arguments, format);
    eduiFntPrintExLeft(font, static_cast<i32>(x * 16.0f), static_cast<i32>(y * 8.0f), 16, eduiPropTextEdit,
                       arguments);
    va_end(arguments);
}

static void eduiFntPrintExCentre(void *font, int x, int y, int, char *format, ...) {
    if (edui_donotdraw) {
        return;
    }
    char text[1024] __attribute__((aligned(16)));
    NuQFntPushPrintMode(2);
    va_list arguments;
    va_start(arguments, format);
    NuVSPrintf(text, format, arguments);
    va_end(arguments);
    i32 width = static_cast<i32>(NuQFntPrintLenU(font, text));
    x -= width / 2;
    NuQFntMove(font, x, y, 0.0f);
    NuQFntPrintU(font, text);
    NuQFntPopPrintMode();
}

static void eduiFntPrintClipEx(void *font, i32 x, i32 y, i32, i32 clip_x, i32 clip_width, char *format, ...) {
    va_list arguments;
    if (!edui_donotdraw) {
        char text[1024] __attribute__((aligned(16)));
        NuQFntPushPrintMode(2);

        va_start(arguments, format);
        NuVSPrintf(text, format, arguments);
        va_end(arguments);
        i32 width = static_cast<i32>(NuQFntPrintLenU(font, text));
        if (x < clip_x)
            x = clip_x;
        i32 length = NuStrLen(text);
        if (width > clip_width) {
            char *end = text + length - 1;
            if (end > text) {
                do {
                    *end-- = '\0';
                    width = static_cast<i32>(NuQFntPrintLenU(font, text));
                } while (width > clip_width && end != text);
            }
        }
        NuQFntMove(font, x, y, 0.0f);
        NuQFntPrintU(font, text);
        NuQFntPopPrintMode();
    }
}

extern "C" {
    // Retail keeps this out of line; the unit's -O3 would otherwise inline it.
    static __attribute__((noinline)) void eduiRenderGraphLine(nugraph_s *graph, i32 x, i32 y, i32 width, i32 height,
                                                              u32 colour) {
        i32 previous_x = 0;
        i32 previous_y = height - static_cast<i32>(height * graph->y[0]);
        f32 fraction = 0.0f;
        for (i32 sample = 0; sample < 101; ++sample) {
            i32 current_x = static_cast<i32>(width * fraction);
            i32 current_y =
                height -
                static_cast<i32>(height * nugraphGetYatX(graph, fraction * graph->x_extent * graph->x_scale, 2));
            eduiDrawLine((x + previous_x) << 4, (y + previous_y) << 3, (x + current_x) << 4, (y + current_y) << 3,
                         colour, uimtls[0]);
            previous_x = current_x;
            previous_y = current_y;
            fraction += 0.01f;
        }
    }

    static void eduiDrawHueValueBand(i32 x, i32 y, i32 width, i32 height, f32 start, f32 end, u32 first, u32 second) {
        i32 left = (x << 4) + static_cast<i32>(start * static_cast<f32>(width << 4));
        i32 band_width = static_cast<i32>(end * static_cast<f32>(width << 4)) -
                         static_cast<i32>(start * static_cast<f32>(width << 4));
        i32 strip_height = (height << 3) / 8;
        i32 first_red = first & 0xff;
        i32 first_green = (first >> 8) & 0xff;
        i32 first_blue = (first >> 16) & 0xff;
        i32 first_alpha = static_cast<i32>(first >> 24) - 0x80;
        i32 second_red = second & 0xff;
        i32 second_green = (second >> 8) & 0xff;
        i32 second_blue = (second >> 16) & 0xff;
        i32 second_alpha = static_cast<i32>(second >> 24) - 0x80;
        struct {
            i32 top_left;
            i32 top_right;
            u8 bottom_left[4];
            u8 bottom_right[4];
        } colours;
        colours.top_left = static_cast<i32>(0x80000000u);
        colours.top_right = static_cast<i32>(0x80000000u);
#define EDUI_DRAW_VALUE_STRIP(strip)                                                                                   \
    {                                                                                                                  \
        u8 *bottom_left = colours.bottom_left;                                                                         \
        u8 *bottom_right = colours.bottom_right;                                                                       \
        i32 first_red_step = first_red * (strip);                                                                      \
        i32 first_green_step = first_green * (strip);                                                                  \
        i32 first_blue_step = first_blue * (strip);                                                                    \
        i32 first_alpha_step = first_alpha * (strip);                                                                  \
        i32 second_red_step = second_red * (strip);                                                                    \
        i32 second_green_step = second_green * (strip);                                                                \
        i32 second_blue_step = second_blue * (strip);                                                                  \
        i32 second_alpha_step = second_alpha * (strip);                                                                \
        bottom_left[0] = first_red_step / edui_grad_steps;                                                                    \
        bottom_left[1] = first_green_step / edui_grad_steps;                                                                  \
        bottom_left[2] = first_blue_step / edui_grad_steps;                                                                   \
        bottom_left[3] = 0x80 + first_alpha_step / edui_grad_steps;                                                           \
        bottom_right[0] = second_red_step / edui_grad_steps;                                                                  \
        bottom_right[1] = second_green_step / edui_grad_steps;                                                                \
        bottom_right[2] = second_blue_step / edui_grad_steps;                                                                 \
        bottom_right[3] = 0x80 + second_alpha_step / edui_grad_steps;                                                         \
        eduiDrawGradRect(left, (y << 3) + (strip) * strip_height, band_width, strip_height,                            \
                         reinterpret_cast<i32 *>(&colours), uimtls[ui_bgmtl]);                                         \
        colours.top_left = *reinterpret_cast<i32 *>(colours.bottom_left);                                              \
        colours.top_right = *reinterpret_cast<i32 *>(colours.bottom_right);                                            \
    }
        EDUI_DRAW_VALUE_STRIP(0);
        EDUI_DRAW_VALUE_STRIP(1);
        EDUI_DRAW_VALUE_STRIP(2);
        EDUI_DRAW_VALUE_STRIP(3);
        EDUI_DRAW_VALUE_STRIP(4);
        EDUI_DRAW_VALUE_STRIP(5);
        EDUI_DRAW_VALUE_STRIP(6);
        EDUI_DRAW_VALUE_STRIP(7);
#undef EDUI_DRAW_VALUE_STRIP
    }

    static __used__ i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        NuQFntHeight(edui_font);
        NuQFntBaseline(edui_font);
        numtl_s *background_material = uimtls[ui_bgmtl];
        item->y = y;
        item->x = x;
        eduiDrawRect(x << 4, y << 3, width << 4, 64, item->colours[2 + item->highlighted], background_material);
        eduiDrawLine((x + 4) << 4, (y << 3) + 32, ((x + width) << 4) - 64, (y << 3) + 32, 0xff000000, uimtls[ui_bgmtl]);
        return 8;
    }

    static __used__ i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        edui_gradient_pick_s *gradient = static_cast<edui_gradient_pick_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 2;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        eduiDrawRect(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted], uimtls[ui_bgmtl]);
        eduiDrawFontSet(edui_font);
        eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        if (gradient->change_timer && gradient->selected_stage) {
            eduiFntPrintEx(edui_font, (width + x * 2) << 3, (y << 3) + baseline, 64, "%1.03f",
                           gradient->selected_stage->time);
            --gradient->change_timer;
        } else
            eduiFntPrintEx(edui_font, (width + x * 2) << 3, (y << 3) + baseline, 64, item->text);

        i32 half_height = height >> 1;
        edui_gradient_node_s *stage = gradient->first_stage;
        if (!stage)
            return height;
        f32 stage_time = stage->time;
        u32 stage_colour = stage->colour;
        edui_gradient_node_s *next = stage->next;
        while (next) {
            i32 colours[4] = {static_cast<i32>(stage_colour), static_cast<i32>(next->colour),
                              static_cast<i32>(stage_colour), static_cast<i32>(next->colour)};
            i32 left = static_cast<i32>(stage_time * (width << 4));
            i32 right = static_cast<i32>(next->time * (width << 4));
            eduiDrawGradRectLikely((x << 4) + left, (y + half_height) << 3, right - left, half_height << 3, colours,
                             uimtls[ui_bgmtl]);
            stage_time = next->time;
            stage_colour = next->colour;
            next = next->next;
        }
        for (edui_gradient_node_s *stage = gradient->first_stage; stage; stage = stage->next) {
            i32 marker_x = (x - 2 + static_cast<i32>(stage->time * width)) << 4;
            i32 marker_y = ((y + half_height) << 3) - 16;
            i32 marker_height = (half_height << 3) + 16;
            eduiDrawRect(marker_x, marker_y, 80, marker_height, stage->colour, uimtls[0]);
            u32 outline = gradient->selected_stage == stage ? 0x80ffffff : 0x80000000;
            eduiDrawLineRect(marker_x, marker_y, 80, marker_height, outline, uimtls[ui_outmtl]);
        }
        return height;
    }

    static __used__ i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 0.15625f);
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        eduiDrawRect(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted], uimtls[ui_bgmtl]);
        eduiDrawFontSetLikely(edui_font);
        eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        if (item->text_alignment == 16)
            eduiFntPrintExGeneric(edui_font, x << 4, (y << 3) + baseline, 16, item->text);
        else if (item->text_alignment == 32)
            eduiFntPrintExGeneric(edui_font, (x + width) << 4, (y << 3) + baseline, 32, item->text);
        else
            eduiFntPrintExGeneric(edui_font, (x * 2 + width) << 3, (y << 3) + baseline, item->text_alignment, item->text);
        return height;
    }

    static __used__ i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        eduiDrawRect(x << 4, y << 3, width << 4, height << 4, item->colours[2 + item->highlighted], uimtls[0]);
        eduiDrawFontSet(edui_font);
        eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        char format[512];
        NuStrCpy(format, item->text);
        NuStrCat(format, slider->format);
        eduiFntPrintEx(edui_font, (x * 2 + width) << 3, (y << 3) + baseline, 64, format,
                       static_cast<i32>(slider->value));
        i32 bottom = (y + height + height - 1) << 3;
        i32 top = (y + height) << 3;
        f32 span = width;
        i32 marker_x = static_cast<i32>(x + 1 + slider->normalized_value * span) << 4;
        eduiDrawLine(marker_x, top, marker_x, bottom, item->colours[item->highlighted], uimtls[0]);
        marker_x = static_cast<i32>(x + slider->normalized_value * span) << 4;
        eduiDrawLine(marker_x, top, marker_x, bottom, item->colours[item->highlighted], uimtls[0]);
        marker_x = static_cast<i32>(x - 1 + slider->normalized_value * span) << 4;
        eduiDrawLine(marker_x, top, marker_x, bottom, item->colours[item->highlighted], uimtls[0]);
        return height * 2;
    }

    static __used__ i32 eduicbRenderTexturePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        u32 colours[2] = {0x807f7f7f, 0x807f7f7f};
        (void)menu;
        edui_texture_pick_s *picker = static_cast<edui_texture_pick_s *>(item);
        item->x = x;
        item->y = y;
        f32 size = static_cast<f32>(width);
        f32 uv_span = 1.0f / picker->zoom;
        f32 origin_u = picker->uv_x[picker->selected_corner] - 0.5f / picker->zoom;
        f32 origin_v = picker->uv_y[picker->selected_corner] - 0.5f / picker->zoom;
        f32 maximum = 1.0f - uv_span;
        if (origin_u < 0.0f)
            origin_u = maximum < 0.0f ? maximum : 0.0f;
        else if (maximum < origin_u)
            origin_u = maximum;
        if (origin_v < 0.0f)
            origin_v = maximum < 0.0f ? maximum : 0.0f;
        else if (maximum < origin_v)
            origin_v = maximum;
        f32 point_u[2] = {
            (picker->uv_x[0] - origin_u) / uv_span,
            (picker->uv_x[1] - origin_u) / uv_span,
        };
        f32 point_v[2] = {
            (picker->uv_y[0] - origin_v) / uv_span,
            (picker->uv_y[1] - origin_v) / uv_span,
        };
        for (i32 corner = 0; corner < 2; ++corner) {
            if (point_u[corner] < 0.0f)
                point_u[corner] = 0.0f;
            else if (point_u[corner] > 1.0f)
                point_u[corner] = 1.0f;
            if (point_v[corner] < 0.0f)
                point_v[corner] = 0.0f;
            else if (point_v[corner] > 1.0f)
                point_v[corner] = 1.0f;
        }
        colours[picker->selected_corner] = 0x80ffffff;
        if (!edui_donotdraw) {
            NuRndrRectUV2di(x << 4, y << 3, width << 4, width << 3, origin_u, origin_v, origin_u + uv_span,
                            origin_v + uv_span, 0x80808080, picker->material);
            for (i32 corner = 0; corner < 2; ++corner) {
                i32 vertical = static_cast<i32>(x + size * point_u[corner]) << 4;
                i32 horizontal = static_cast<i32>(y + size * point_v[corner]) << 3;
                NuRndrLine2di(x << 4, horizontal, (x + width - 1) << 4, horizontal, colours[corner], uimtls[0]);
                NuRndrLine2di(vertical, y << 3, vertical, (y + width) * 8 - 8, colours[corner], uimtls[0]);
            }
        }
        i32 label_y = y + width;
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        if (!edui_donotdraw) {
            NuRndrRect2di(x << 4, label_y << 3, width << 4, height << 3, item->colours[2 + item->highlighted],
                          uimtls[ui_bgmtl]);
            NuQFntSet(edui_font);
            NuQFntSetColour(edui_font, item->colours[item->highlighted]);
        }
        eduiFntPrintEx(edui_font, (x * 2 + width) << 3, (label_y << 3) + baseline, 64, "%1.03f,%1.03f",
                       picker->uv_x[picker->selected_corner] * 256.0f, picker->uv_y[picker->selected_corner] * 256.0f);
        return width + height;
    }

    static __used__ i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        edui_text_selector_s *selector = static_cast<edui_text_selector_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        eduiDrawRect(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted], uimtls[ui_bgmtl]);
        eduiDrawFontSet(edui_font);
        eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        char format[512];
        NuStrCpy(format, item->text);
        NuStrCat(format, ": %s");
        eduiFntPrintEx(edui_font, x << 4, (y << 3) + baseline, 16, format,
                       selector->options[static_cast<i32>(selector->value)]);
        return height;
    }

    static __used__ i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        edui_slider_s *number = static_cast<edui_slider_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->y = y;
        item->x = x;
        eduiDrawRect(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted], uimtls[ui_bgmtl]);
        eduiDrawFontSet(edui_font);
        eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        char format[512];
        NuStrCpy(format, item->text);
        NuStrCat(format, number->format);
        eduiFntPrintEx(edui_font, x << 4, (y << 3) + baseline, 16, format, number->value);
        return height;
    }

    static __used__ i32 eduicbRenderGraph(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32 selected) {
        (void)menu;
        edui_graph_s *picker = static_cast<edui_graph_s *>(item);
        f32 cursor_y = picker->cursor_y;
        f32 cursor_x = picker->cursor_x;
        item->x = x;
        item->y = y;
        i32 total_width = picker->width;
        i32 graph_width = total_width - 60;
        i32 total_height = picker->height;
        i32 graph_height = total_height - 60;
        i32 plot_x = x + 30;
        i32 plot_y = y + 30;
        i32 plot_bottom = plot_y + graph_height;
        eduiDrawRect(x << 4, y << 3, total_width << 4, total_height << 3, 0x80000000, uimtls[0]);
        eduiDrawRect(plot_x << 4, plot_y << 3, graph_width << 4, graph_height << 3, 0x80100000, uimtls[0]);
        if (!picker->graph) {
            eduiDrawFontSet(edui_font);
            eduiSetFontColour(edui_font, 0x80000080);
            eduiFntPrintEx(edui_font, (x + total_width) << 3, (y + total_height) << 2, 64, "Invalid Graph Pointer");
            return total_height;
        }
        eduiDrawFontSetLikely(edui_font);
        eduiSetFontColour(edui_font, 0x80808080);
        if (picker->x_scale > 0.0f) {
            f32 tick_count = picker->graph->x_extent * picker->graph->x_scale / picker->x_scale;
            i32 label_interval = static_cast<i32>(tick_count / 10.0f) + 1;
            for (i32 tick = 1; tick <= static_cast<i32>(tick_count); ++tick) {
                f32 tick_offset = static_cast<f32>(graph_width * tick) / tick_count;
                u32 colour = 0x80260000;
                if (tick % label_interval == 0) {
                    eduiDrawFontSet(edui_font);
                    eduiFntPrintEx(edui_font, static_cast<i32>(plot_x + tick_offset) << 4,
                                   (plot_bottom << 3) - 48 + static_cast<i32>(NuQFntHeight(edui_font)), 64, "%1.01f",
                                   static_cast<f32>(tick) * picker->x_scale);
                    colour = 0x80340000;
                }
                i32 line_x = static_cast<i32>(plot_x + tick_offset) << 4;
                eduiDrawLine(line_x, plot_y << 3, line_x, (plot_bottom << 3) - 8, colour, uimtls[0]);
            }
        }
        if (picker->y_scale > 0.0f) {
            f32 tick_count = picker->graph->y_extent * picker->graph->y_scale / picker->y_scale;
            i32 label_interval = static_cast<i32>(tick_count / 10.0f) + 1;
            for (i32 tick = 1; tick <= static_cast<i32>(tick_count); ++tick) {
                f32 tick_offset = static_cast<f32>(graph_height * tick) / tick_count;
                u32 colour = 0x80260000;
                if (tick % label_interval == 0) {
                    eduiDrawFontSet(edui_font);
                    eduiFntPrintEx(edui_font, (x + 28) << 4,
                                   (static_cast<i32>(plot_bottom - tick_offset) << 3) +
                                       (static_cast<i32>(NuQFntHeight(edui_font)) >> 1),
                                   32, "%1.01f", static_cast<f32>(tick) * picker->y_scale);
                    colour = 0x80340000;
                }
                i32 line_y = static_cast<i32>(plot_bottom - tick_offset) << 3;
                eduiDrawLine(plot_x << 4, line_y, (plot_x + graph_width - 1) << 4, line_y, colour, uimtls[0]);
            }
        }
        eduiDrawFontSet(edui_font);
        eduiFntPrintEx(edui_font, (plot_x + graph_width) << 4, (y << 3) + static_cast<i32>(NuQFntHeight(edui_font)), 32,
                       picker->x_label);
        eduiFntPrintEx(edui_font, (x + total_width - 3) << 4, (y + total_height) << 3, 32, picker->y_label);
        eduiFntPrintEx(edui_font, plot_x << 4, (y << 3) + static_cast<i32>(NuQFntHeight(edui_font)), 16, picker->title);
        eduiFntPrintEx(edui_font, (x + 28) << 4, (plot_bottom << 3) - 48 + static_cast<i32>(NuQFntHeight(edui_font)),
                       32, "0.0");
        u32 cursor_colour = selected ? 0x80707070 : 0x80303030;
        i32 cursor_line_y = static_cast<i32>(plot_y + (1.0f - cursor_y) * graph_height) << 3;
        eduiDrawLine(plot_x << 4, cursor_line_y, (plot_x + graph_width - 1) << 4, cursor_line_y, cursor_colour,
                     uimtls[0]);
        i32 cursor_line_x = static_cast<i32>(plot_x + cursor_x * graph_width) << 4;
        eduiDrawLine(cursor_line_x, plot_y << 3, cursor_line_x, (plot_bottom << 3) - 8, cursor_colour, uimtls[0]);
        picker->selected_point = -1;
        f32 closest = 1.0f;
        for (i32 point = 0; point < picker->graph->point_count; ++point) {
            i32 px = plot_x + static_cast<i32>(picker->graph->x[point] * graph_width);
            i32 py = plot_bottom - static_cast<i32>(picker->graph->y[point] * graph_height);
            eduiDrawLine((px - 10) << 4, py << 3, (px + 10) << 4, py << 3, 0x80505050, uimtls[0]);
            eduiDrawLine(px << 4, (py << 3) - 80, px << 4, (py << 3) + 80, 0x80505050, uimtls[0]);
            f32 distance =
                fabsf(picker->cursor_x - picker->graph->x[point]) + fabsf(picker->cursor_y - picker->graph->y[point]);
            if (distance < closest && distance < 0.05f) {
                picker->selected_point = point;
                closest = distance;
            }
        }
        if (picker->onion_skins[0])
            eduiRenderGraphLine(picker->onion_skins[0], plot_x, plot_y, graph_width, graph_height, 0x80402000);
        if (picker->onion_skins[1])
            eduiRenderGraphLine(picker->onion_skins[1], plot_x, plot_y, graph_width, graph_height, 0x80402000);
        if (picker->onion_skins[2])
            eduiRenderGraphLine(picker->onion_skins[2], plot_x, plot_y, graph_width, graph_height, 0x80402000);
        if (picker->onion_skins[3])
            eduiRenderGraphLine(picker->onion_skins[3], plot_x, plot_y, graph_width, graph_height, 0x80402000);
        if (picker->onion_skins[4])
            eduiRenderGraphLine(picker->onion_skins[4], plot_x, plot_y, graph_width, graph_height, 0x80402000);
        if (picker->onion_skins[5])
            eduiRenderGraphLine(picker->onion_skins[5], plot_x, plot_y, graph_width, graph_height, 0x80402000);
        if (picker->onion_skins[6])
            eduiRenderGraphLine(picker->onion_skins[6], plot_x, plot_y, graph_width, graph_height, 0x80402000);
        if (picker->onion_skins[7])
            eduiRenderGraphLine(picker->onion_skins[7], plot_x, plot_y, graph_width, graph_height, 0x80402000);
        eduiRenderGraphLine(picker->graph, plot_x, plot_y, graph_width, graph_height, 0x80800000);
        if (picker->selected_point >= 0 && !edui_donotdraw) {
            i32 point = picker->selected_point;
            NuRndrLineRect2di((plot_x + static_cast<i32>(picker->graph->x[point] * graph_width) - 6) << 4,
                              ((plot_bottom - static_cast<i32>(picker->graph->y[point] * graph_height)) << 3) - 48, 192,
                              96, 0x80ffffff, uimtls[ui_outmtl]);
        }
        if (show_x_scale_help || show_y_scale_help) {
            eduiDrawFontSet(edui_font);
            i32 help_x = (plot_x + (graph_width >> 1)) << 4;
            i32 help_y = plot_y << 3;
            help_y += static_cast<i32>(NuQFntHeight(edui_font));
            eduiSetFontColour(edui_font, 0x80808080);
            eduiFntPrintEx(edui_font, help_x, help_y, 64, "Scaling %c Axis", show_x_scale_help ? 'X' : 'Y');
            help_y += static_cast<i32>(NuQFntHeight(edui_font));
            eduiFntPrintEx(edui_font, help_x, help_y, 64, "%1.03f",
                           show_x_scale_help ? picker->graph->x_scale : picker->graph->y_scale);
            help_y += static_cast<i32>(NuQFntHeight(edui_font));
        }
        i32 label_height = static_cast<i32>(NuQFntHeight(edui_font) * 1.125f) >> 3;
        i32 label_y = (y + total_height) << 3;
        f32 label_font_height = NuQFntHeight(edui_font);
        i32 label_baseline = static_cast<i32>(NuQFntBaseline(edui_font)) +
                             (((label_height << 3) - static_cast<i32>(label_font_height)) >> 1);
        eduiDrawRect(x << 4, label_y, total_width << 4, label_height << 3, item->colours[2 + item->highlighted],
                     uimtls[ui_bgmtl]);
        eduiDrawFontSet(edui_font);
        eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        if (picker->selected_point >= 0) {
            i32 point = picker->selected_point;
            eduiFntPrintEx(edui_font, (x + 2) << 4, label_y + label_baseline, 16, "Point %d = %1.02f, %1.02f", point,
                           picker->graph->x[point] * picker->graph->x_scale * picker->graph->x_extent,
                           picker->graph->y[point] * picker->graph->y_scale * picker->graph->y_extent);
        }
        eduiFntPrintEx(edui_font, (x + total_width - 4) << 4, label_y + label_baseline, 32,
                       "Cursor pos = %1.02f, %1.02f",
                       picker->cursor_x * picker->graph->x_scale * picker->graph->x_extent,
                       picker->cursor_y * picker->graph->y_scale * picker->graph->y_extent);
        return total_height + label_height;
    }

    static __used__ i32 eduicbRenderFilePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        (void)menu;
        edui_file_pick_s *picker = static_cast<edui_file_pick_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        if (picker->reserved_54) {
            eduiDrawRect(x << 4, y << 3, width << 4, height << 4, item->colours[2 + item->highlighted],
                         uimtls[ui_bgmtl]);
            eduiDrawFontSet(edui_font);
            eduiSetFontColour(edui_font, item->colours[item->highlighted]);
            i32 centre = (x * 2 + width) << 3;
            eduiFntPrintEx(edui_font, centre, (y << 3) + baseline, 64, item->text);
            eduiFntPrintEx(edui_font, centre, ((y + height) << 3) + baseline, 64, picker->format, picker->name);
            return height * 2;
        }
        eduiDrawRect(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted], uimtls[ui_bgmtl]);
        char formatted[264];
        sprintf(formatted, picker->format, picker->name);
        eduiDrawFontSet(edui_font);
        eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        eduiFntPrintEx(edui_font, (x * 2 + width) << 3, (y << 3) + baseline, 64, "%s %s", item->text, formatted);
        return height;
    }

    __attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32) {
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->y = y;
        item->x = x;
        eduiDrawRect(x << 4, y << 3, width << 4, height << 3, item->colours[2], uimtls[ui_bgmtl]);
        eduiDrawFontSetLikely(edui_font);
        eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        eduiFntPrintEx(edui_font, x << 4, (y << 3) + baseline, 16, item->text);

        i32 top = (y << 3) + 16;
        i32 left = ((x - height + width - 2) << 4) + 16;
        i32 right = left + ((height - 2) << 4) - 16;
        i32 bottom = top + ((height - 2) << 3) - 8;
        if (item->highlighted) {
            i32 middle_y = (top + bottom) >> 1;
            i32 quarter_x = (left + ((left + right) >> 1)) >> 1;
            i32 lower_y = (bottom + middle_y) >> 1;
            i32 points[8] = {left, middle_y, quarter_x, lower_y, quarter_x, bottom, right, top};
            eduiDrawTriStrip(points, NULL, 4, item->colours[0], uimtls[0]);
        } else {
            i32 middle_y = (top + bottom) >> 1;
            i32 quarter_x = (left + ((left + right) >> 1)) >> 1;
            i32 lower_y = (bottom + middle_y) >> 1;
            i32 points[10] = {left, middle_y, quarter_x, lower_y, right, top, quarter_x, bottom, left, middle_y};
            eduiDrawLineStrip(points, NULL, 5, item->colours[0], uimtls[0]);
        }
        return height;
    }

    static __used__ i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        edui_colour_pick_s *pick = static_cast<edui_colour_pick_s *>(item);
        i32 colours[4] = {static_cast<i32>(0x80000000), static_cast<i32>(0x80ffffff), static_cast<i32>(0x80000000),
                          static_cast<i32>(0x80ffffff)};
        i32 height = width / 4;
        item->x = x;
        item->y = y;
        {
            i32 left = static_cast<i32>(0.0f * (width << 4));
            i32 right = static_cast<i32>(1.0f * (width << 4));
            eduiDrawGradRect((x << 4) + left, y << 3, right - left, height << 3, colours, uimtls[ui_bgmtl]);
        }
        i32 cursor_x = static_cast<i32>(x + 1 + (width - 2) * pick->value) << 4;
        eduiDrawLine(cursor_x, y << 3, cursor_x, (y + (height >> 1) - 1) << 3, 0x80ffffff, uimtls[0]);
        cursor_x = static_cast<i32>(x + (width - 2) * pick->value) << 4;
        eduiDrawLine(cursor_x, y << 3, cursor_x, (y + (height >> 1) - 1) << 3, 0x80ffffff, uimtls[0]);
        cursor_x = static_cast<i32>(x - 1 + (width - 2) * pick->value) << 4;
        eduiDrawLine(cursor_x, y << 3, cursor_x, (y + (height >> 1) - 1) << 3, 0x80ffffff, uimtls[0]);
        cursor_x = static_cast<i32>(x + 1 + (width - 2) * pick->value) << 4;
        eduiDrawLine(cursor_x, (y + (height >> 1)) << 3, cursor_x, (y + height - 1) << 3, 0x80000000, uimtls[0]);
        cursor_x = static_cast<i32>(x + (width - 2) * pick->value) << 4;
        eduiDrawLine(cursor_x, (y + (height >> 1)) << 3, cursor_x, (y + height - 1) << 3, 0x80000000, uimtls[0]);
        cursor_x = static_cast<i32>(x - 1 + (width - 2) * pick->value) << 4;
        eduiDrawLine(cursor_x, (y + (height >> 1)) << 3, cursor_x, (y + height - 1) << 3, 0x80000000, uimtls[0]);
        return height;
    }

    static __used__ i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        edui_colour_slider_s *slider = static_cast<edui_colour_slider_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        eduiDrawRect(x << 4, y << 3, width << 4, height << 4, item->colours[2 + item->highlighted], uimtls[0]);
        eduiDrawFontSet(edui_font);
        eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        eduiFntPrintEx(edui_font, (x + 64) << 4, (y << 3) + baseline, 64, item->text);
        eduiFntPrintEx(edui_font, (x + 116) << 4, (y << 3) + baseline, 64, slider->format,
                       static_cast<i32>(slider->value));

        i32 colour = slider->red + (slider->green << 8) + (slider->blue << 16);
        i32 colours[4] = {static_cast<i32>(0xff000000), colour, static_cast<i32>(0xff000000), colour};
        i32 top = (y + height) << 3;
        {
            i32 left = static_cast<i32>(0.0f * (width << 4));
            i32 right = static_cast<i32>(1.0f * (width << 4));
            eduiDrawGradRect((x << 4) + left, top, right - left, height << 3, colours, uimtls[ui_bgmtl]);
        }
        i32 bottom = (y + height + height - 1) << 3;
        {
            i32 marker = static_cast<i32>((x + 1) + slider->normalized_value * width) << 4;
            eduiDrawLine(marker, top, marker, bottom, item->colours[item->highlighted], uimtls[0]);
        }
        {
            i32 marker = static_cast<i32>(x + slider->normalized_value * width) << 4;
            eduiDrawLine(marker, top, marker, bottom, item->colours[item->highlighted], uimtls[0]);
        }
        {
            i32 marker = static_cast<i32>((x - 1) + slider->normalized_value * width) << 4;
            eduiDrawLine(marker, top, marker, bottom, item->colours[item->highlighted], uimtls[0]);
        }
        return height * 2;
    }

    static __used__ i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        auto *picker = static_cast<edui_textpicker_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->y = y;
        item->x = x;

        char letter[2] = "0";
        if (picker->editing) {
            item_width = width > height * 10 ? width : height * 10;
            i32 total_height = (picker->keyboard_flags & 2) ? height * 6 : height * 5;
            eduiDrawRect(x << 4, y << 3, item_width << 4, total_height << 3, item->colours[2], uimtls[ui_bgmtl]);
            for (i32 row = 0; row < 4; ++row) {
                auto draw_key = [&](i32 column) {
                    if (row == picker->keyboard_row && column == picker->keyboard_column) {
                        eduiDrawFontSet(edui_font);
                        eduiSetFontColour(edui_font, item->colours[1]);
                    } else {
                        eduiDrawFontSet(edui_font);
                        eduiSetFontColour(edui_font, item->colours[0]);
                    }
                    letter[0] = textrow[row][column];
                    eduiFntPrintEx(edui_font, (x + height * column) << 4, ((y + row * height) << 3) + baseline, 16,
                                   letter);
                };
                draw_key(0);
                draw_key(1);
                draw_key(2);
                draw_key(3);
                draw_key(4);
                draw_key(5);
                draw_key(6);
                draw_key(7);
                draw_key(8);
                draw_key(9);
            }
            eduiDrawFontSetLikely(edui_font);
            eduiSetFontColour(edui_font, item->colours[1]);
            i32 value_y = ((y + height * 4) << 3) + baseline;
            eduiFntPrintEx(edui_font, x << 4, value_y, 16, picker->value);
            if (--cursor_flash < 1)
                cursor_flash = 32;
            if (cursor_flash >= 16 && !edui_donotdraw) {
                char prefix[256];
                i32 position = picker->cursor;
                i32 offset = 0;
                if (position) {
                    NuStrNCpy(prefix, picker->value, position + 1);
                    offset = static_cast<i32>(NuQFntPrintLenU(edui_font, prefix));
                }
                char at_cursor[2] = {position < NuStrLen(picker->value) ? picker->value[position] : 'A', '\0'};
                NuRndrRect2di((x << 4) + offset, value_y - static_cast<i32>(NuQFntBaseline(edui_font)),
                              static_cast<i32>(NuQFntPrintLenU(edui_font, at_cursor)),
                              static_cast<i32>(NuQFntHeight(edui_font)), item->colours[1] ^ 0xffffff, NULL);
            }
            if (picker->keyboard_flags & 2) {
                eduiDrawFontSet(edui_font);
                eduiSetFontColour(edui_font, item->colours[0]);
                eduiFntPrintEx(edui_font, x << 4, ((y + height * 5) << 3) + baseline, 16,
                               TextPickCopyBuffer[0] ? TextPickCopyBuffer
                                                     : const_cast<char *>("Copy L2+R2 Paste L1+R1"));
            }
            return total_height;
        } else {
            if (!(picker->keyboard_flags & 1)) {
                eduiDrawRect(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted],
                             uimtls[ui_bgmtl]);
                char formatted[256];
                sprintf(formatted, picker->format, picker->value);
                eduiDrawFontSet(edui_font);
                eduiSetFontColour(edui_font, item->colours[item->highlighted]);
                eduiFntPrintEx(edui_font, x << 4, (y << 3) + baseline, 16, const_cast<char *>("%s %s"), item->text,
                               formatted);
                return height;
            }
            {
                eduiDrawRectLikely(x << 4, y << 3, width << 4, height << 4, item->colours[2 + item->highlighted],
                             uimtls[ui_bgmtl]);
            }
            eduiDrawFontSetLikely(edui_font);
            eduiSetFontColourLikely(edui_font, item->colours[item->highlighted]);
            i32 centre = (x * 2 + width) << 3;
            eduiFntPrintEx(edui_font, centre, (y << 3) + baseline, 64, item->text);
            eduiFntPrintEx(edui_font, centre, ((y + height) << 3) + baseline, 64, picker->format, picker->value);
            return height * 2;
        }
    }

    static __used__ i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        eduiDrawRect(x << 4, y << 3, width << 4, height << 4, item->colours[2 + item->highlighted], uimtls[ui_bgmtl]);
        eduiDrawFontSet(edui_font);
        eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        char format[512];
        NuStrCpy(format, item->text);
        NuStrCat(format, slider->format);
        eduiFntPrintEx(edui_font, (x * 2 + width) << 3, (y << 3) + baseline, 64, format, slider->value);
        if (slider->normalized_value >= 0.0f && slider->normalized_value <= 1.0f) {
            i32 top = (y + height) << 3;
            i32 bottom = (y + height + height - 1) << 3;
            f32 span = width - 2;
            i32 marker_x = static_cast<i32>(x + 1 + slider->normalized_value * span) << 4;
            eduiDrawLine(marker_x, top, marker_x, bottom, item->colours[item->highlighted], uimtls[0]);
            marker_x = static_cast<i32>(x + slider->normalized_value * span) << 4;
            eduiDrawLine(marker_x, top, marker_x, bottom, item->colours[item->highlighted], uimtls[0]);
            marker_x = static_cast<i32>(x - 1 + slider->normalized_value * span) << 4;
            eduiDrawLine(marker_x, top, marker_x, bottom, item->colours[item->highlighted], uimtls[0]);
        }
        return height * 2;
    }

    static __used__ i32 eduicbProcessExpander(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad) {
        (void)delta_time;
        edui_expander_s *expander = static_cast<edui_expander_s *>(item);
        if (pad->digital_buttons_pressed & EDUI_CURSOR_PRIMARY) {
            if (expander->changed)
                expander->changed(menu, item, 0);
            if (expander->first_child) {
                if (!expander->open) {
                    expander->last_child->next = item->next;
                    if (item->next)
                        item->next->previous = expander->last_child;
                    expander->first_child->previous = item;
                    item->next = expander->first_child;
                    expander->open = 1;
                } else {
                    eduicbItemExpanderClose(expander);
                }
            }
        }
        return 0;
    }

    static __used__ i32 eduicbInteractExpander(edui_interact_s *interact) {
        eduimenu_s *menu = interact->menu;
        edui_expander_s *expander = static_cast<edui_expander_s *>(interact->item);
        if (__builtin_expect(edui_cursor_buttons_db & EDUI_CURSOR_PRIMARY, 1)) {
            if (expander->changed)
                expander->changed(menu, expander, 0);
            if (edui_cursor_x >= expander->button_x && edui_cursor_y >= expander->button_y * 0.5f &&
                edui_cursor_x < expander->button_x + expander->button_size &&
                edui_cursor_y < (expander->button_y + expander->button_size) * 0.5f && expander->first_child) {
                if (!expander->open) {
                    expander->last_child->next = expander->next;
                    if (expander->next)
                        expander->next->previous = expander->last_child;
                    expander->first_child->previous = expander;
                    expander->next = expander->first_child;
                    expander->open = 1;
                } else {
                    eduicbItemExpanderClose(expander);
                }
            }
        }
        return 0;
    }

    static __used__ i32 eduicbInteractProp(struct edui_interact_s *interact) {
        eduimenu_s *menu = interact->menu;
        edui_prop_s *property = static_cast<edui_prop_s *>(interact->item);
        u8 &property_flags = reinterpret_cast<u8 *>(property)[0x4c];
        bool handled = false;
        if (__builtin_expect(edui_cursor_buttons & EDUI_CURSOR_PRIMARY, 0)) {
            if (!(property_flags & 0x0a)) {
                if (__builtin_expect(property_flags & 1, 0)) {
                    if (edui_cursor_x >= interact->x + property->label_width + 1.0f && edui_cursor_y >= interact->y &&
                        edui_cursor_x < property->button_x && edui_cursor_y < interact->y + interact->height) {
                        property_flags |= 1;
                        eduiPropTextPos = -1;
                        handled = true;
                    }
                } else {
                    if (__builtin_expect(edui_cursor_x >= interact->x, 0) && edui_cursor_y >= interact->y &&
                        edui_cursor_x < interact->x + property->label_width - 1.0f &&
                        edui_cursor_y < interact->y + interact->height && property->selected) {
                        property->selected(menu, property, 0);
                    }
                    if (edui_cursor_x >= interact->x + property->label_width - 1.0f &&
                        edui_cursor_y >= interact->y &&
                        edui_cursor_x < interact->x + property->label_width + 1.0f &&
                        edui_cursor_y < interact->y + interact->height) {
                        property_flags |= 0x06;
                    }
                    if (edui_cursor_x >= static_cast<edui_prop_s *>(interact->item)->label_width + interact->x + 1.0f &&
                        edui_cursor_y >= interact->y &&
                        edui_cursor_x < static_cast<edui_prop_s *>(interact->item)->button_x &&
                        edui_cursor_y < interact->y + interact->height && !(property_flags & 1)) {
                        if (property->selected)
                            property->selected(menu, property, 0);
                        NuKeyFlush();
                        property_flags |= 1;
                        NuStrCpy(eduiPropTextStore, property->property_text);
                        NuStrCpy(eduiPropTextEdit, property->property_text);
                        eduiPropTextPos = NuStrLen(property->property_text);
                    }
                    if (((edui_cursor_x >= property->button_x && edui_cursor_y >= property->button_y * 0.5f &&
                          edui_cursor_x < property->button_x + property->button_size &&
                          edui_cursor_y < (property->button_y + property->button_size) * 0.5f) ||
                         eduiInteractLocked) &&
                        property->button) {
                        property->button(menu, property, 0);
                        if (property_flags & 0x20)
                            handled = true;
                    }
                }
            }
        }
        return handled || (property_flags & 0x0b);
    }

    static __used__ i32 eduicbInteractFilter(struct edui_interact_s *interact) {
        return eduicbInteractProp(interact);
    }

    static __used__ i32 eduicbProcessColourSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad) {
        static i32 rep_timer = 30;
        static f32 dynascale;
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        if ((pad->digital_buttons & 0x0c) == 0x0c) {
            if (slider && slider->minimum <= 0.0f && slider->minimum + slider->range >= 0.0f) {
                slider->value = 0.0f;
                slider->normalized_value = (slider->value - slider->minimum) / slider->range;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
            }
            rep_timer = 30;
        } else if ((pad->digital_buttons & 0x03) == 0x03) {
            if (slider && slider->minimum <= 0.0f && slider->minimum + slider->range >= 255.0f) {
                slider->value = 255.0f;
                slider->normalized_value = (slider->value - slider->minimum) / slider->range;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
            }
            rep_timer = 30;
        } else if (pad->digital_buttons & 0x04) {
            if (rep_timer > 0)
                --rep_timer;
            else
                rep_timer = 0;
            if (!rep_timer || (pad->digital_buttons_pressed & 0x04)) {
                f32 value = slider->value - slider->granularity;
                if (value < slider->minimum)
                    value = slider->minimum;
                f32 rounded = value > 0.0f ? value + slider->granularity * 0.5f : value - slider->granularity * 0.5f;
                slider->value = static_cast<i32>(rounded / slider->granularity) * slider->granularity;
                slider->normalized_value = (slider->value - slider->minimum) / slider->range;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
            }
        } else if (pad->digital_buttons & 0x08) {
            if (rep_timer > 0)
                --rep_timer;
            else
                rep_timer = 0;
            if (!rep_timer || (pad->digital_buttons_pressed & 0x08)) {
                f32 value = slider->value + slider->granularity;
                f32 maximum = slider->minimum + slider->range;
                value = maximum < value ? maximum : value;
                f32 rounded = value > 0.0f ? value + slider->granularity * 0.5f : value - slider->granularity * 0.5f;
                slider->value = static_cast<i32>(rounded / slider->granularity) * slider->granularity;
                slider->normalized_value = (slider->value - slider->minimum) / slider->range;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
            }
        } else {
            rep_timer = 30;
            if (pad->analog_l2) {
                f32 scale;
                if (!pad->unknown_a9) {
                    dynascale = 1.627604291343232e-7f;
                    scale = 1.627604251552839e-5f;
                } else {
                    dynascale *= 1.05f;
                    scale = dynascale;
                }
                f32 value = slider->normalized_value - static_cast<u32>(pad->analog_l2) * scale;
                if (value < 0.0f)
                    value = 0.0f;
                slider->normalized_value = value;
                slider->value = value * slider->range + slider->minimum;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
            } else if (pad->analog_r2) {
                f32 scale;
                if (!pad->unknown_a9) {
                    dynascale = 1.627604291343232e-7f;
                    scale = 1.627604251552839e-5f;
                } else {
                    dynascale *= 1.05f;
                    scale = dynascale;
                }
                f32 value = slider->normalized_value + static_cast<u32>(pad->analog_r2) * scale;
                if (value > 1.0f)
                    value = 1.0f;
                slider->normalized_value = value;
                slider->value = value * slider->range + slider->minimum;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
            } else {
                dynascale = 1.627604291343232e-7f;
            }
        }
        return 0;
    }

    static __used__ i32 eduicbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad) {
        edui_colour_pick_s *pick = static_cast<edui_colour_pick_s *>(item);
        if (!edui_cursor_buttons) {
            pick->dragging_colour = 0;
            pick->dragging_saturation = 0;
        }
        f32 dx = eduiPickerDeadZone32(pad->analog_left_x) * 0.001f;
        f32 dy = eduiPickerDeadZone32(pad->analog_left_y) * 0.001f;
        pick->cursor_x += dx;
        pick->cursor_y += dy;
        pick->cursor_x = MIN(1.0f, (MAX(0.0f, pick->cursor_x)));
        pick->cursor_y = MIN(1.0f, (MAX(0.0f, pick->cursor_y)));
        if (pad->analog_l2) {
            pick->saturation = MAX(0.0f, pick->saturation - static_cast<u32>(pad->analog_l2) * delta_time * 0.005f);
            return 1;
        }
        if (pad->analog_r2) {
            pick->saturation = MIN(1.0f, pick->saturation + static_cast<u32>(pad->analog_r2) * delta_time * 0.005f);
            return 1;
        }
        pick->hue = pick->cursor_x * 360.0f;
        pick->value = pick->cursor_y;
        eduiHSVToRGB(pick->hue, pick->saturation, pick->value, pick->red, pick->green, pick->blue);
        if (((pad->digital_buttons_pressed & EDUI_CURSOR_PRIMARY) || pick->confirm) && pick->changed)
            pick->changed(menu, item, pad->digital_buttons);
        return 0;
    }

    static __used__ i32 eduicbInteractColourPick(edui_interact_s *interact) {
        edui_colour_pick_s *pick = static_cast<edui_colour_pick_s *>(interact->item);
        if (((edui_cursor_x >= interact->x && edui_cursor_y >= interact->y &&
              edui_cursor_x < interact->x + interact->width &&
              edui_cursor_y < interact->y + interact->height * 0.75f) ||
             pick->dragging_colour) &&
            !pick->dragging_saturation) {
            pick->cursor_y = (edui_cursor_y - interact->y) / (interact->height * 0.75f);
            pick->cursor_x = (edui_cursor_x - interact->x) / interact->width;
            pick->dragging_colour = (edui_cursor_buttons & EDUI_CURSOR_PRIMARY) != 0;
        }
        if (((edui_cursor_x >= interact->x && edui_cursor_y >= interact->y + interact->height * 0.75f &&
              edui_cursor_x < interact->x + interact->width &&
              edui_cursor_y < interact->y + interact->height * 0.75f + interact->height * 0.125f) ||
             pick->dragging_saturation) &&
            !pick->dragging_colour) {
            pick->saturation = (edui_cursor_x - interact->x) / interact->width;
            pick->dragging_saturation = (edui_cursor_buttons & EDUI_CURSOR_PRIMARY) != 0;
        }
        pick->cursor_x = MIN(1.0f, (MAX(0.0f, pick->cursor_x)));
        pick->cursor_y = MIN(1.0f, (MAX(0.0f, pick->cursor_y)));
        pick->saturation = MIN(1.0f, (MAX(0.0f, pick->saturation)));
        pick->hue = pick->cursor_x * 360.0f;
        pick->value = pick->cursor_y;
        eduiHSVToRGB(pick->hue, pick->saturation, pick->value, pick->red, pick->green, pick->blue);
        if (edui_cursor_x >= interact->x &&
            edui_cursor_y >= interact->y + interact->height * 0.75f + interact->height * 0.125f &&
            edui_cursor_x < interact->x + interact->width && edui_cursor_y < interact->y + interact->height &&
            (edui_cursor_buttons_db & EDUI_CURSOR_PRIMARY)) {
            pick->dragging_colour = 0;
            pick->dragging_saturation = 0;
            pick->confirm = 1;
        }
        return pick->dragging_colour || pick->dragging_saturation;
    }

    static __used__ i32 eduicbRenderColourPick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        (void)menu;
        edui_colour_pick_s *picker = static_cast<edui_colour_pick_s *>(item);
        item->y = y;
        item->x = x;
        i32 main_height = (width * 3) / 4;
        eduiDrawHueValueBand(x, y, width, main_height, 0.0f, 1.0f / 6.0f, 0x800000ffu, 0x8000ffffu);
        eduiDrawHueValueBand(x, y, width, main_height, 1.0f / 6.0f, 2.0f / 6.0f, 0x8000ffffu, 0x8000ff00u);
        eduiDrawHueValueBand(x, y, width, main_height, 2.0f / 6.0f, 0.5f, 0x8000ff00u, 0x80ffff00u);
        eduiDrawHueValueBand(x, y, width, main_height, 0.5f, 4.0f / 6.0f, 0x80ffff00u, 0x80ff0000u);
        eduiDrawHueValueBand(x, y, width, main_height, 4.0f / 6.0f, 5.0f / 6.0f, 0x80ff0000u, 0x80ff00ffu);
        eduiDrawHueValueBand(x, y, width, main_height, 5.0f / 6.0f, 1.0f, 0x80ff00ffu, 0x800000ffu);
        if (!edui_donotdraw) {
            i32 value_y = static_cast<i32>(y + main_height * picker->cursor_y) << 3;
            NuRndrLine2di(x << 4, value_y, (x + width - 1) << 4, value_y, 0x80ffffff, uimtls[0]);
        }
        if (!edui_donotdraw) {
            i32 hue_x = static_cast<i32>(x + width * picker->cursor_x) << 4;
            NuRndrLine2di(hue_x, y << 3, hue_x, ((y + main_height) << 3) - 8, 0x80ffffff, uimtls[0]);
        }
        i32 bar_height = width / 8;
        f32 full_red, full_green, full_blue;
        eduiHSVToRGB(picker->hue, 1.0f, picker->value, full_red, full_green, full_blue);
        i32 grey_byte = static_cast<i32>(picker->value * 255.0f);
        i32 grey = static_cast<i32>(0x80000000u | (grey_byte * 0x10101));
        i32 full = static_cast<i32>(0x80000000u | (static_cast<i32>(full_blue * 255.0f) << 16) |
                                    (static_cast<i32>(full_green * 255.0f) << 8) | static_cast<i32>(full_red * 255.0f));
        i32 saturation_colours[4] = {grey, full, grey, full};
        i32 saturation_y = y + main_height;
        if (!edui_donotdraw)
            NuRndrGradRect2di(x << 4, saturation_y << 3, width << 4, bar_height << 3, saturation_colours,
                              uimtls[ui_bgmtl]);
        if (!edui_donotdraw) {
            i32 marker_mid = saturation_y + (bar_height >> 1);
            for (i32 offset = 1; offset >= -1 && !edui_donotdraw; --offset) {
                i32 marker_x = static_cast<i32>(static_cast<f32>(x + offset) + picker->saturation * (width - 2)) << 4;
                NuRndrLine2di(marker_x, saturation_y << 3, marker_x, (marker_mid << 3) - 8, 0x80ffffff, uimtls[0]);
            }
            for (i32 offset = 1; offset >= -1 && !edui_donotdraw; --offset) {
                i32 marker_x = static_cast<i32>(static_cast<f32>(x + offset) + picker->saturation * (width - 2)) << 4;
                NuRndrLine2di(marker_x, marker_mid << 3, marker_x, ((saturation_y + bar_height) << 3) - 8, 0x80000000,
                              uimtls[0]);
            }
        }
        f32 red, green, blue;
        eduiHSVToRGB(picker->hue, picker->saturation, picker->value, red, green, blue);
        i32 selected = static_cast<i32>(0x80000000u | (static_cast<i32>(blue * 255.0f) << 16) |
                                        (static_cast<i32>(green * 255.0f) << 8) | static_cast<i32>(red * 255.0f));
        i32 selected_colours[4] = {selected, selected, selected, selected};
        if (__builtin_expect(!edui_donotdraw, 0))
            NuRndrGradRect2di(x << 4, (saturation_y + bar_height) << 3, width << 4, bar_height << 3, selected_colours,
                              uimtls[ui_bgmtl]);
        return width;
    }

    static __used__ i32 eduicbRenderSelWithClipColour(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width,
                                                      i32) {
        (void)menu;
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        {
            eduiDrawRect(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted],
                         uimtls[ui_bgmtl]);
            eduiDrawFontSet(edui_font);
            eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        }
        eduiFntPrintEx(edui_font, (x * 2 + width) << 3, (y << 3) + baseline, 64, item->text);
        f32 green = clipg;
        f32 red = clipg;
        f32 blue = clipg;
        if (item->type == 7)
            eduiHSVToRGB(cliph, clips, clipv, red, green, blue);
        u32 colour = 0x80000000u | (static_cast<i32>(blue * 255.0f) << 16) | (static_cast<i32>(green * 255.0f) << 8) |
                     static_cast<i32>(red * 255.0f);
        i32 colours[4] = {static_cast<i32>(colour), static_cast<i32>(colour), static_cast<i32>(colour),
                          static_cast<i32>(colour)};
        eduiDrawGradRect((x + width - 45) << 4, y << 3, 40 << 4, height << 3, colours, uimtls[ui_bgmtl]);
        return height;
    }

    void edmainSetReturn(i32 result) {
        editor_return = result;
    }

    static __used__ void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32) {
        edmainSetReturn(1);
    }

    static __used__ void cbMMCancel(eduimenu_s *, eduimenu_s *) {
        edmainSetReturn(1);
    }

    ed_module_s *edmainCurrent(void) {
        return ed_module_active ? ed_curr : NULL;
    }

    i32 edmainActivate(ed_module_s *module, i32 notify) {
        if (module) {
            for (ed_module_s *entry = ed_list; entry; entry = entry->next) {
                if (entry == module) {
                    ed_curr = module;
                    ed_module_active = 1;
                    if (module->activate && notify)
                        module->activate();
                    return 1;
                }
            }
        } else if (ed_curr) {
            if (ed_curr->deactivate && notify)
                ed_curr->deactivate();
            ed_curr = NULL;
            ed_module_active = 0;
        }
        return 0;
    }

    void edmainSetCamera(NUMTX *matrix) {
        edmaincam->mtx = *matrix;
        NuCameraSet(edmaincam);
    }

    NUCAMERA *edmainGetCamera(void) {
        return edmaincam;
    }

    void edmainExtCamera(NUCAMERA *camera) {
        edmaincam = camera != NULL ? camera : edinternalcam;
    }

    void edmainSetCursorEnabled(i32 enabled) {
        edmain_cursor_enabled = enabled;
    }

    i32 edmainGetCursorEnabled(void) {
        return edmain_cursor_enabled;
    }

    eduimenu_s *edGetMainMenu(void) {
        return ed_main_menu;
    }

    void edmainRegisterLocVec(NUVEC *position) {
        ed_loc = position;
    }

    NUVEC *edmainQueryLocVec(void) {
        return ed_loc;
    }

    void eduiSetGlobalSliderAccel(f32 acceleration) {
        if (acceleration < 0.0f)
            acceleration = 0.0f;
        else if (acceleration > 1.0f)
            acceleration = 1.0f;
        slider_acceleration = acceleration;
    }

    void eduiInitMaterials(void) {
        uimtls[0] = NuMtlCreate(1);
        uimtls[0]->opacity = 1.0f;
        uimtls[0]->attribs.alpha_mode = 0;
        uimtls[0]->attribs.cull_mode = 2;
        uimtls[0]->attribs.z_mode = 3;
        NuMtlUpdate(uimtls[0]);
        uimtls[1] = NuMtlCreate(1);
        uimtls[1]->opacity = 0.999f;
        uimtls[1]->diffuse_color.r = 0.2f;
        uimtls[1]->diffuse_color.g = 0.2f;
        uimtls[1]->diffuse_color.b = 0.2f;
        uimtls[1]->attribs.cull_mode = 2;
        uimtls[1]->attribs.z_mode = 3;
        uimtls[1]->attribs.alpha_mode = 3;
        NuMtlUpdate(uimtls[1]);
        uimtls[2] = NuMtlCreate(1);
        uimtls[2]->opacity = 0.999f;
        uimtls[2]->diffuse_color.r = 0.2f;
        uimtls[2]->diffuse_color.g = 0.2f;
        uimtls[2]->diffuse_color.b = 0.2f;
        uimtls[2]->attribs.cull_mode = 2;
        uimtls[2]->attribs.z_mode = 3;
        uimtls[2]->attribs.alpha_mode = 2;
        NuMtlUpdate(uimtls[2]);
        uimtls[3] = NuMtlCreate(1);
        uimtls[3]->opacity = 0.999f;
        uimtls[3]->diffuse_color.r = 0.2f;
        uimtls[3]->diffuse_color.g = 0.2f;
        uimtls[3]->diffuse_color.b = 0.2f;
        uimtls[3]->attribs.cull_mode = 2;
        uimtls[3]->attribs.z_mode = 3;
        uimtls[3]->attribs.alpha_mode = 1;
        NuMtlUpdate(uimtls[3]);
    }

    void eduiSetRenderPlane(i8 plane) {
        NuMtlSetRenderPlane(uimtls[0], plane);
        NuMtlSetRenderPlane(uimtls[1], plane);
        NuMtlSetRenderPlane(uimtls[2], plane);
        NuMtlSetRenderPlane(uimtls[3], plane);
    }

    eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item) {
        if (menu->last)
            menu->last->next = item;
        else
            menu->first = item;
        item->previous = menu->last;
        item->next = NULL;
        menu->field_0c = menu->first;
        menu->field_10 = 0;
        menu->last = item;
        edui_last_item = item;
        return item;
    }

    void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item) {
        if (menu->first)
            menu->first->previous = item;
        else
            menu->last = item;
        item->next = menu->first;
        item->previous = NULL;
        menu->first = item;
        menu->field_10 = 0;
        menu->field_0c = item;
        edui_last_item = item;
    }

    void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item) {
        if (menu->last)
            menu->last->next = item;
        else
            menu->first = item;
        item->previous = menu->last;
        item->next = NULL;
        menu->field_10 = 0;
        menu->last = item;
        menu->field_0c = item;
        edui_last_item = item;
    }

    void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after) {
        if (after) {
            item->next = after->next;
            item->previous = after;
            after->next = item;
            if (item->next)
                item->next->previous = item;
            else
                menu->last = item;
            menu->field_10 = 0;
            menu->field_0c = menu->first;
            edui_last_item = item;
        } else
            eduiMenuAddItemFirst(menu, item);
    }

    void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before) {
        if (before) {
            item->previous = before->previous;
            item->next = before;
            before->previous = item;
            // The original does not update the preceding item's next link.
            if (!item->previous)
                menu->first = item;
            menu->field_0c = menu->first;
            menu->field_10 = 0;
            edui_last_item = item;
        } else
            eduiMenuAddItemLast(menu, item);
    }

    i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item) {
        if (menu->first == item)
            return 0;
        eduiitem_s *previous = item->previous;
        eduiitem_s *next = item->next;
        item->next = previous;
        item->previous = previous->previous;
        if (item->previous)
            item->previous->next = item;
        else
            menu->first = item;
        if (next)
            next->previous = previous;
        else
            menu->last = item->next;
        previous->previous = item;
        previous->next = next;
        menu->field_0c = menu->first;
        menu->field_10 = 0;
        return 1;
    }

    i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item) {
        if (menu->last == item)
            return 0;
        eduiitem_s *next = item->next;
        eduiitem_s *previous = item->previous;
        item->previous = next;
        item->next = next->next;
        if (item->next)
            item->next->previous = item;
        else
            menu->last = item;
        if (previous)
            previous->next = next;
        else
            menu->first = next;
        next->next = item;
        next->previous = previous;
        menu->field_0c = menu->first;
        menu->field_10 = 0;
        return 1;
    }

    void eduiMenuSortItemsByTxt(eduimenu_s *menu) {
        eduiitem_s *item = menu->first;
        if (item) {
            i32 sorted;
            do {
                eduiitem_s *first = item;
                sorted = 1;
                while (item->next && item->text && item->next->text) {
                    if (item->type != 20 && (item->next->type == 20 || NuStrICmp(item->text, item->next->text) > 0)) {
                        eduiMenuItemMoveDown(menu, item);
                        if (item == first)
                            first = item->previous;
                        sorted = 0;
                    } else {
                        item = item->next;
                    }
                }
                item = first;
            } while (!sorted);
        }
    }

    void eduiMenuSelectFirstEntry(eduimenu_s *menu) {
        menu->selected = NULL;
    }

    void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item) {
        if (menu->first == item)
            menu->first = item->next;
        if (menu->last == item)
            menu->last = item->previous;
        if (menu->selected == item)
            menu->selected = item->next ? item->next : item->previous;
        menu->field_0c = menu->first;
        menu->field_10 = 0;
        if (item->next)
            item->next->previous = item->previous;
        if (item->previous)
            item->previous->next = item->next;
        item->next = NULL;
        item->previous = NULL;
    }

    f32 eduiGetAnalougePadValue(nupad_s *pad) {
        f32 value = 0.0f;
        if (pad && (pad->digital_buttons & EDUI_CURSOR_PRIMARY)) {
            if (pad->analog_left_x > 192)
                value = (pad->analog_left_y - 128.0f) * 0.01f;
            if (pad->analog_left_x < 64)
                value = (128.0f - pad->analog_left_y) * -0.01f;
            if (pad->analog_right_x > 192)
                value = (pad->analog_right_y - 128.0f) * 0.001f;
            if (pad->analog_right_x < 64)
                value = (128.0f - pad->analog_right_y) * -0.001f;
        }
        return value;
    }

    eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback,
                               char *title) {
        u32 bytes = sizeof(eduimenu_s) + 1;
        if (title)
            bytes += NuStrLen(title);
        eduimenu_s *menu = static_cast<eduimenu_s *>(NU_ALLOC(bytes, 4, 1, "", 0));
        if (menu) {
            memset(menu, 0, bytes);
            menu->field_24 = -1;
            menu->x = x;
            menu->y = y;
            menu->width = width;
            menu->height = height;
            menu->field_28 = -1;
            menu->font = font;
            menu->callback = callback;
            menu->flags = 0;
            if (title) {
                menu->title = reinterpret_cast<char *>(menu + 1);
                NuStrCpy(menu->title, title);
            }
            active_menu = menu;
        }
        edui_last_item = NULL;
        return menu;
    }

    void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours) {
        do {
            for (eduiitem_s *item = menu->first; item; item = item->next) {
                item->colours[0] = colours[0];
                item->colours[1] = colours[1];
                item->colours[2] = colours[2];
                item->colours[3] = colours[3];
            }
            menu = menu->child;
        } while (menu);
    }

    void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours) {
        for (eduiitem_s *item = menu->first; item; item = item->next) {
            item->colours[0] &= 0xffffff;
            item->colours[0] |= colours[0] & 0xff000000;
            item->colours[1] &= 0xffffff;
            item->colours[1] |= colours[1] & 0xff000000;
            item->colours[2] &= 0xffffff;
            item->colours[2] |= colours[2] & 0xff000000;
            item->colours[3] &= 0xffffff;
            item->colours[3] |= colours[3] & 0xff000000;
        }
    }

    void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled) {
        do {
            for (eduiitem_s *item = menu->first; item; item = item->next)
                item->disabled = disabled;
            menu = menu->child;
        } while (menu);
    }

    void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item) {
        if (item->selection_group) {
            for (eduiitem_s *entry = menu->first; entry; entry = entry->next) {
                if (entry == item) {
                    if (entry->type == EDUI_ITEM_TOGGLE)
                        entry->highlighted = 1 - entry->highlighted;
                    else
                        entry->highlighted = 1;
                } else if (entry->selection_group == item->selection_group) {
                    entry->highlighted = 0;
                }
            }
        }
    }

    static __used__ i32 eduicbProcessSel(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad) {
        edui_sel_s *selection = static_cast<edui_sel_s *>(item);
        if (pad->digital_buttons_pressed & EDUI_CURSOR_PRIMARY) {
            eduiMenuHighlight(menu, item);
            if (selection->selected)
                selection->selected(menu, item, pad->digital_buttons);
        } else if ((pad->digital_buttons & EDUI_CURSOR_PRIMARY) && selection->held) {
            selection->held(menu, item, pad->digital_buttons);
        }
        return 0;
    }

    i32 eduiItemSetText(eduiitem_s *item, char *text) {
        if (item->text && NuStrLen(item->text) < NuStrLen(text)) {
            NU_FREE(item->text);
            item->text = NULL;
        }
        if (!item->text) {
            u32 size = NuStrLen(text) + 1;
            item->text = static_cast<char *>(NU_ALLOC(size, 4, 1, "", 0));
        }
        if (item->text) {
            NuStrCpy(item->text, text);
            return 1;
        }
        return 0;
    }

}

static __used__ void cbToggleIndicatorMode(eduimenu_s *, eduiitem_s *item, u32) {
    edui_colour_pick_s *picker = cp_item;
    item->data = 1 - item->data;
    cpUpdateInfo(item, picker);
}

static __used__ i32 cbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, float delta_time, nupad_s *pad) {
    edui_colour_pick_s *picker = cp_item;
    i32 result = cp_process(menu, item, delta_time, pad);
    char text[256];
    i32 mode = cp_info->data;
    if (mode != 0)
        sprintf(text, "R:%1.2f G:%1.2f B:%1.2f", picker->red, picker->green, picker->blue);
    else
        sprintf(text, "H:%1.2f S:%1.2f V:%1.2f", picker->hue, picker->saturation, picker->value);
    eduiItemSetText(cp_info, text);
    return result;
}

extern "C" {
    eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text) {
        edui_sel_s *item = static_cast<edui_sel_s *>(NU_ALLOC(sizeof(edui_sel_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 0;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessSel;
        item->render = eduicbRenderSel;
        item->destroy = eduicbItemDestroy;
        item->interact = eduicbInteractSel;
        item->highlighted = selected;
        item->selection_group = group;
        item->text = NULL;
        item->text_alignment = 0x40;
        eduiItemSetText(item, text ? text : (char *)"<no name>");
        item->selected = callback;
        item->held = NULL;
        return item;
    }

    eduiitem_s *eduiItemSelWithClipColourCreate(usize data, const void *colours, i32 selected, i32 group,
                                                EdUiItemCallback callback, char *text) {
        eduiitem_s *item = eduiItemSelCreate(data, colours, selected, group, callback, text);
        if (item)
            item->render = eduicbRenderSelWithClipColour;
        return item;
    }

    eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text) {
        eduiitem_s *item = eduiItemSelCreate(data, colours, selected, group, callback, text);
        if (item) {
            item->type = 2;
            item->process = eduicbProcessSel;
            item->render = eduicbRenderCheck;
        }
        return item;
    }

    eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text) {
        eduiitem_s *item = eduiItemSelCreate(data, colours, selected, group, callback, text);
        if (item) {
            item->type = 3;
            item->process = eduicbProcessSel;
            item->render = eduicbRenderCheck;
        }
        return item;
    }

    void eduiItemTextPickSetFmt(edui_textpicker_s *item, char *format) {
        if (item->format) {
            i32 old_length = NuStrLen(item->format);
            i32 new_length = NuStrLen(format);
            if (old_length >= new_length)
                goto copy;
            NU_FREE(item->format);
        }
        item->format = static_cast<char *>(NU_ALLOC(NuStrLen(format) + 1, 4, 1, "", 0));
    copy:
        NuStrCpy(item->format, format);
    }

    eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        auto *item = static_cast<edui_textpicker_s *>(NU_ALLOC(sizeof(edui_textpicker_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 1;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessTextPick;
        item->render = eduicbRenderTextPick;
        item->destroy = eduicbItemTextPickDestroy;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        item->max_length = 255;
        item->callback = callback;
        item->format = NULL;
        item->cursor = 0;
        eduiItemTextPickSetFmt(item, const_cast<char *>("\"%s\""));
        textrow[0] = const_cast<char *>("1234567890");
        textrow[1] = const_cast<char *>("QWERTYUIOP");
        textrow[2] = const_cast<char *>("ASDFGHJKL_");
        textrow[3] = const_cast<char *>("ZXCVBNM<>?");
        return item;
    }

    i32 cbCompateDirentByDateAsc(const void *first, const void *second) {
        return static_cast<const FilePickDirectoryEntry *>(first)->year -
               static_cast<const FilePickDirectoryEntry *>(second)->year;
    }

    i32 cbCompateDirentByDateDec(const void *first, const void *second) {
        return static_cast<const FilePickDirectoryEntry *>(second)->year -
               static_cast<const FilePickDirectoryEntry *>(first)->year;
    }

    i32 cbCompateDirentBySizeAsc(const void *first, const void *second) {
        i32 first_size = static_cast<const FilePickDirectoryEntry *>(first)->size;
        i32 second_size = static_cast<const FilePickDirectoryEntry *>(second)->size;
        if (first_size < second_size)
            return -1;
        if (first_size > second_size)
            return 1;
        return 0;
    }

    i32 cbCompateDirentBySizeDec(const void *first, const void *second) {
        i32 first_size = static_cast<const FilePickDirectoryEntry *>(first)->size;
        i32 second_size = static_cast<const FilePickDirectoryEntry *>(second)->size;
        if (first_size < second_size)
            return 1;
        if (first_size > second_size)
            return -1;
        return 0;
    }

    i32 cbCompateDirentByNameAsc(const void *first, const void *second) {
        return NuStrCmp(static_cast<const FilePickDirectoryEntry *>(first)->name,
                        static_cast<const FilePickDirectoryEntry *>(second)->name);
    }

    i32 cbCompateDirentByNameDec(const void *first, const void *second) {
        return NuStrCmp(static_cast<const FilePickDirectoryEntry *>(second)->name,
                        static_cast<const FilePickDirectoryEntry *>(first)->name);
    }

    void eduiItemFilePickSetFmt(edui_file_pick_s *item, char *format) {
        if (item->format) {
            i32 old_length = NuStrLen(item->format);
            i32 new_length = NuStrLen(format);
            if (old_length >= new_length)
                goto copy;
            NU_FREE(item->format);
        }
        item->format = static_cast<char *>(NU_ALLOC(NuStrLen(format) + 1, 4, 1, "", 0));
    copy:
        NuStrCpy(item->format, format);
    }

    eduiitem_s *eduiItemFilePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        auto *item = static_cast<edui_file_pick_s *>(NU_ALLOC(sizeof(edui_file_pick_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 1;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessFilePick;
        item->render = eduicbRenderFilePick;
        item->destroy = eduicbItemFilePickDestroy;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        item->changed = callback;
        item->format = NULL;
        item->name[0] = '\0';
        item->directory[0] = '\0';
        item->filename[0] = '\0';
        item->compare_entries = NULL;
        eduiItemFilePickSetFmt(item, const_cast<char *>("\"%s\""));
        return item;
    }

    void eduiGradStageSetRGB(edui_gradient_node_s *stage, f32 red, f32 green, f32 blue) {
        f32 hue = 0.0f, saturation = 0.0f, value;
        eduiRGBToHSV(red, green, blue, hue, saturation, value);
        stage->hue = hue;
        stage->value = value;
        stage->saturation = saturation;
        stage->colour = 0x80000000u | static_cast<u32>(static_cast<i32>(red)) |
                        (static_cast<u32>(static_cast<i32>(green)) << 8) |
                        (static_cast<u32>(static_cast<i32>(blue)) << 16);
    }

    void eduiGradStageSetHSV(edui_gradient_node_s *stage, f32 hue, f32 saturation, f32 value) {
        // Left uninitialised as in the original: the out-of-range sector path
        // reads undefined values, which GCC materialises as zero.
        f32 red, green, blue;
        eduiHSVToRGB(hue, saturation, value, red, green, blue);
        stage->saturation = saturation;
        stage->hue = hue;
        stage->value = value;
        stage->colour = 0x80000000u | static_cast<u32>(static_cast<i32>(red * 255.0f)) |
                        (static_cast<u32>(static_cast<i32>(green * 255.0f)) << 8) |
                        (static_cast<u32>(static_cast<i32>(blue * 255.0f)) << 16);
    }

    edui_gradient_node_s *eduiGradStageAdd(edui_gradient_pick_s *item, f32 time, f32 hue, f32 saturation, f32 value) {
        edui_gradient_node_s *stage = static_cast<edui_gradient_node_s *>(NU_ALLOC(sizeof(*stage), 4, 1, "", 0));
        if (!stage)
            return NULL;
        memset(stage, 0, sizeof(*stage));
        edui_gradient_node_s *previous = NULL;
        edui_gradient_node_s *next = item->first_stage;
        while (next && !(next->time > time)) {
            previous = next;
            next = next->next;
        }
        if (previous)
            previous->next = stage;
        else
            item->first_stage = stage;
        if (next)
            next->previous = stage;
        stage->previous = previous;
        stage->time = time;
        stage->next = next;
        eduiGradStageSetHSV(stage, hue, saturation, value);
        item->selected_stage = stage;
        return stage;
    }

    edui_gradient_node_s *eduiGradStageAddRGB(edui_gradient_pick_s *item, f32 time, f32 red, f32 green, f32 blue) {
        f32 hue = 0.0f, saturation = 0.0f, value;
        eduiRGBToHSV(red, green, blue, hue, saturation, value);
        return eduiGradStageAdd(item, time, hue, saturation, value);
    }

    void eduiGradStageDelete(edui_gradient_pick_s *item, edui_gradient_node_s *stage) {
        edui_gradient_node_s *previous = stage->previous;
        edui_gradient_node_s *next = stage->next;
        if (previous)
            previous->next = next;
        else
            item->first_stage = next;
        if (next)
            next->previous = previous;
        if (next) {
            item->selected_stage = next;
        } else {
            item->selected_stage = previous;
        }
        NU_FREE(stage);
    }

    static __used__ void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *item) {
        edui_gradient_pick_s *gradient = static_cast<edui_gradient_pick_s *>(item);
        while (gradient->first_stage)
            eduiGradStageDelete(gradient, gradient->first_stage);
        NU_FREE(gradient);
    }

    i32 eduiGradPickRead(eduiitem_s *item, edui_gradient_stage_s *stages, i32 capacity) {
        edui_gradient_node_s *node = static_cast<edui_gradient_pick_s *>(item)->first_stage;
        f32 red, green, blue;
        i32 count = 0;
        for (; node; node = node->next, ++count) {
            if (count < capacity) {
                edui_gradient_stage_s *stage = &stages[count];
                stage->time = node->time;
                stage->hue = node->hue;
                stage->value = node->value;
                stage->saturation = node->saturation;
                eduiHSVToRGB(node->hue, node->saturation, node->value, red, green, blue);
                stage->red = red;
                stage->green = green;
                stage->blue = blue;
                stage->colour = 0x80000000u + static_cast<i32>(red * 255.0f) + (static_cast<i32>(green * 255.0f) << 8) +
                                (static_cast<i32>(blue * 255.0f) << 16);
                stage->metadata = node->metadata;
            }
        }
        return count;
    }

    eduiitem_s *eduiItemGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        edui_gradient_pick_s *item =
            static_cast<edui_gradient_pick_s *>(NU_ALLOC(sizeof(edui_gradient_pick_s), 4, 1, "", 0));
        if (__builtin_expect(item != NULL, 1)) {
            memset(item, 0, sizeof(*item));
            item->type = 7;
            item->data = data;
            memcpy(item->colours, colours, sizeof(item->colours));
            item->process = eduicbProcessGradPick;
            item->render = eduicbRenderGradPick;
            item->destroy = eduicbItemGradPickDestroy;
            item->text_alignment = 0x40;
            item->selection_group = 0;
            eduiItemSetText(item, text);
            item->change_timer = 0;
            item->changed = callback;
        }
        return item;
    }

    eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        eduiitem_s *item = eduiItemGradPickCreate(data, colours, callback, text);
        if (item)
            item->type = 8;
        return item;
    }

    eduiitem_s *eduiItemDataGradPickCreate(usize data, const void *colours, EdUiItemCallback callback,
                                           EdUiItemCallback press, EdUiItemCallback add, EdUiItemCallback remove,
                                           EdUiItemCallback copy, EdUiItemCallback paste, char *text) {
        edui_gradient_pick_s *item =
            static_cast<edui_gradient_pick_s *>(eduiItemGradPickCreate(data, colours, callback, text));
        if (item) {
            item->type = 9;
            item->press = press;
            item->add = add;
            item->remove = remove;
            item->copy = copy;
            item->paste = paste;
        }
        return item;
    }

    void eduiItemColourPickSetHSV(edui_colour_pick_s *item, f32 hue, f32 saturation, f32 value) {
        item->hue = hue;
        item->saturation = saturation;
        item->value = value;
        item->cursor_x = hue / 360.0f;
        item->cursor_y = value;
    }

    void eduiItemColourPickSetRGB(edui_colour_pick_s *item, f32 red, f32 green, f32 blue) {
        f32 maximum = MAX(red, MAX(green, blue));
        f32 minimum = MIN(red, MIN(green, blue));
        item->value = maximum;
        item->saturation = maximum != 0.0f ? (maximum - minimum) / maximum : 0.0f;
        if (item->saturation != 0.0f) {
            f32 delta = maximum - minimum;
            f32 hue;
            if (red == maximum)
                hue = (green - blue) / delta;
            else if (green == maximum)
                hue = 2.0f + (blue - red) / delta;
            else if (blue == maximum)
                hue = 4.0f + (red - green) / delta;
            else
                hue = item->hue;
            hue *= 60.0f;
            if (hue < 0.0f)
                hue += 360.0f;
            item->hue = hue;
            item->cursor_y = maximum;
            item->cursor_x = hue / 360.0f;
        } else {
            item->hue = 0.0f;
            item->cursor_x = 0.0f;
            item->cursor_y = maximum;
        }
    }

}

static __used__ void cbPaste(eduimenu_s *, eduiitem_s *, u32) {
    *cp_r = clipboard_r;
    *cp_g = clipboard_g;
    *cp_b = clipboard_b;
    if (cp_rgba != nullptr)
        *cp_rgba = cpPackColour(*cp_r, *cp_g, *cp_b, *cp_a);
    eduiItemColourPickSetRGB(cp_item, clipboard_r, clipboard_g, clipboard_b);
}

extern "C" {
    eduiitem_s *eduiItemColourPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        edui_colour_pick_s *item = static_cast<edui_colour_pick_s *>(NU_ALLOC(sizeof(edui_colour_pick_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 10;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessColourPick;
        item->render = eduicbRenderColourPick;
        item->interact = eduicbInteractColourPick;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        item->destroy = eduicbItemDestroy;
        eduiItemSetText(item, text);
        item->cursor_x = 0.5f;
        item->cursor_y = 0.5f;
        item->hue = 180.0f;
        item->value = 0.5f;
        item->saturation = 1.0f;
        item->changed = callback;
        return item;
    }

    eduiitem_s *eduiItemGreyPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        eduiitem_s *item = eduiItemColourPickCreate(data, colours, callback, text);
        if (item) {
            item->type = 11;
            item->process = eduicbProcessGreyPick;
            item->render = eduicbRenderGreyPick;
        }
        return item;
    }

    eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        edui_texture_pick_s *item =
            static_cast<edui_texture_pick_s *>(NU_ALLOC(sizeof(edui_texture_pick_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 12;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessTexturePick;
        item->render = eduicbRenderTexturePick;
        item->destroy = eduicbItemDestroy;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        item->uv_x[0] = 0.4f;
        item->uv_y[0] = 0.4f;
        item->uv_x[1] = 0.5f;
        item->uv_y[1] = 0.5f;
        item->zoom = 1.0f;
        item->selected_corner = 0;
        item->changed = callback;
        return item;
    }

    eduiitem_s *eduiItemGraphCreate(usize data, const void *colours, EdUiItemCallback callback, nugraph_s *graph,
                                    i32 width, i32 height) {
        edui_graph_s *item = static_cast<edui_graph_s *>(NU_ALLOC(sizeof(edui_graph_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 15;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessGraph;
        item->render = eduicbRenderGraph;
        item->destroy = eduicbItemDestroy;
        item->changed = callback;
        item->graph = graph;
        item->width = width;
        item->height = height;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        item->selected_point = -1;
        item->cursor_x = 0.5f;
        item->cursor_y = 0.5f;
        item->x_scale = 1.0f;
        item->y_scale = 1.0f;
        for (i32 i = 0; i != 8; ++i)
            item->onion_skins[i] = NULL;
        if (graph->point_count <= 1)
            nugraphInit(graph);
        nugraphCalcCurve(graph, 100);
        return item;
    }

    void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph) {
        if (item->type == 15) {
            i32 slot;
            if (!item->onion_skins[0])
                slot = 0;
            else if (!item->onion_skins[1])
                slot = 1;
            else if (!item->onion_skins[2])
                slot = 2;
            else if (!item->onion_skins[3])
                slot = 3;
            else if (!item->onion_skins[4])
                slot = 4;
            else if (!item->onion_skins[5])
                slot = 5;
            else if (!item->onion_skins[6])
                slot = 6;
            else if (!item->onion_skins[7])
                slot = 7;
            else
                return;
            item->onion_skins[slot] = graph;
        }
    }

    void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y) {
        if (item->type == 15) {
            item->cursor_x = MIN(1.0f, (MAX(0.0f, x)));
            item->cursor_y = MIN(1.0f, (MAX(0.0f, y)));
        }
    }

    void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title) {
        if (item->type == 15) {
            if (x)
                NuStrCpy(item->x_label, x);
            if (y)
                NuStrCpy(item->y_label, y);
            if (title)
                NuStrCpy(item->title, title);
        }
    }

    void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify) {
        item->normalized_value = (value - item->minimum) / item->range;
        item->value = value;
        if (reset_timer)
            item->change_timer = 60;
        if (notify && item->changed)
            item->changed(NULL, item, 0);
    }

    void eduiItemSliderSetVal(edui_slider_s *item, f32 value) {
        eduiItemSliderSetValEx(item, value, 1, 1);
    }

    i32 eduicbInteractSlider(edui_interact_s *interact) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(interact->item);
        f32 value = (edui_cursor_x - interact->x) / interact->width;
        value = CLAMP(value, 0.0f, 1.0f);
        eduiItemSliderSetVal(slider, value * slider->range);
        return (edui_cursor_buttons & EDUI_CURSOR_PRIMARY) != 0;
    }

    void eduiItemSliderSetFmt(edui_slider_s *item, char *format) {
        if (!item->format) {
            u32 size = NuStrLen(format) + 1;
            item->format = static_cast<char *>(NU_ALLOC(size, 4, 1, "", 0));
        } else if (NuStrLen(item->format) < NuStrLen(format)) {
            NU_FREE(item->format);
            u32 size = NuStrLen(format) + 1;
            item->format = static_cast<char *>(NU_ALLOC(size, 4, 1, "", 0));
        }
        NuStrCpy(item->format, format);
    }

    void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity) {
        item->granularity = granularity;
    }

    eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text) {
        edui_slider_s *item = static_cast<edui_slider_s *>(NU_ALLOC(sizeof(edui_slider_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 6;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessSlider;
        item->interact = eduicbInteractSlider;
        item->render = eduicbRenderSlider;
        item->destroy = eduicbItemSliderDestroy;
        item->selection_group = group;
        item->text_alignment = 0x40;
        eduiItemSetText(item, text);
        item->granularity = 0.001f;
        item->changed = callback;
        item->minimum = minimum;
        item->range = range;
        item->format = NULL;
        eduiItemSliderSetFmt(item, (char *)"(%1.03f)");
        eduiItemSliderSetVal(item, value);
        return item;
    }

    eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 range, i32 value, char *text) {
        edui_slider_s *item = static_cast<edui_slider_s *>(
            eduiItemSliderCreate(data, colours, group, callback, minimum, range, value, text));
        if (item) {
            eduiItemSliderSetFmt(item, (char *)"(%d)");
            item->granularity = 1.0f;
            item->render = eduicbRenderSliderInt;
        }
        return item;
    }

    eduiitem_s *eduiItemNumberCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text) {
        eduiitem_s *item = eduiItemSliderCreate(data, colours, group, callback, minimum, range, value, text);
        if (item)
            item->render = eduicbRenderNumber;
        return item;
    }

    eduiitem_s *eduiItemTextSelectorCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           i32 count, i32 selected, char *text, char **options) {
        edui_text_selector_s *item =
            static_cast<edui_text_selector_s *>(NU_ALLOC(sizeof(edui_text_selector_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 6;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessSlider;
        item->interact = eduicbInteractSlider;
        item->render = eduicbRenderTextSelector;
        item->destroy = eduicbItemSliderDestroy;
        item->selection_group = group;
        item->text_alignment = 0x40;
        eduiItemSetText(item, text);
        item->format = NULL;
        item->changed = callback;
        item->minimum = 0.0f;
        item->range = static_cast<f32>(count - 1);
        item->granularity = 1.0f;
        eduiItemSliderSetVal(item, static_cast<f32>(selected));
        item->options = options;
        return item;
    }

    i32 eduiItemPropSetText(edui_prop_s *item, char *text) {
        if (item->property_text && NuStrLen(item->property_text) < NuStrLen(text)) {
            NU_FREE(item->property_text);
            item->property_text = NULL;
        }
        if (!item->property_text) {
            u32 size = NuStrLen(text) + 1;
            item->property_text = static_cast<char *>(NU_ALLOC(size, 4, 1, "", 0));
        }
        if (item->property_text) {
            NuStrCpy(item->property_text, text);
            return 1;
        }
        return 0;
    }

    i32 eduiGetCameraEnabled(void) {
        return bCameraEnabled;
    }

    void eduiSetCameraEnabled(i32 enabled) {
        bCameraEnabled = enabled;
    }

    i32 NuKeyToAscii(u32 key, i32 shifted) {
        for (NUKEYCHAR *entry = NuKeyChar; entry->key != 0; ++entry) {
            if (entry->key == key)
                return shifted ? entry->shifted : entry->normal;
        }
        return 0;
    }

    static i32 eduicbProcessPropKeyboard(eduimenu_s *menu, edui_prop_s *property) {
        if (!(property->unknown_property_flags & 1))
            return 0;
        i32 length = NuStrLen(eduiPropTextEdit);
        if (eduiPropTextPos < 0)
            eduiPropTextPos = 0;
        if (eduiPropTextPos > length)
            eduiPropTextPos = length;
        u32 modifiers = 0;
        i32 key = NuKeyGet(&modifiers);
        if (key == -1)
            return 0;
        if (key == 0x1c) {
            eduiItemPropSetText(property, eduiPropTextEdit);
            if (property->changed)
                property->changed(menu, property, 0);
            property->unknown_property_flags &= ~1;
            eduiPropTextPos = -1;
            return 0;
        }
        if (key < 0x1d) {
            if (key == 1) {
                property->unknown_property_flags &= ~1;
                eduiPropTextPos = -1;
                return 0;
            }
            if (key == 0xe) {
                if (eduiPropTextPos < 1)
                    return 0;
                char *cursor = eduiPropTextEdit + eduiPropTextPos - 1;
                --eduiPropTextPos;
                char character = *cursor;
                while (character) {
                    character = cursor[1];
                    *cursor++ = character;
                }
                return 0;
            }
        } else {
            if (key == 0xcd) {
                if (eduiPropTextPos < length)
                    ++eduiPropTextPos;
                return 0;
            }
            if (key == 0xd3) {
                if (eduiPropTextPos >= length)
                    return 0;
                char *cursor = eduiPropTextEdit + eduiPropTextPos;
                char character = *cursor;
                while (character) {
                    character = cursor[1];
                    *cursor++ = character;
                }
                return 0;
            }
            if (key == 0xcb) {
                if (eduiPropTextPos > 0)
                    --eduiPropTextPos;
                return 0;
            }
        }
        char letter = NuKeyToAscii(key, modifiers & 1);
        if (letter) {
            eduiPropTextEdit[length + 1] = '\0';
            char *cursor = eduiPropTextEdit + length;
            char *insertion = eduiPropTextEdit + eduiPropTextPos;
            if (insertion < cursor) {
                do {
                    *cursor = cursor[-1];
                    --cursor;
                } while (cursor != insertion);
            }
            *cursor = letter;
            ++eduiPropTextPos;
        }
        return 0;
    }

    static __used__ i32 eduicbProcessProp(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad) {
        (void)delta_time;
        edui_prop_s *property = static_cast<edui_prop_s *>(item);
        u8 *property_flags = reinterpret_cast<u8 *>(property) + 0x4c;
        if (__builtin_expect(!!(*property_flags & 2), 1)) {
            property->label_width = edui_cursor_x - static_cast<f32>(item->x);
            if (property->label_width < 1.0f)
                property->label_width = 1.0f;
            for (eduiitem_s *other = menu->first; other; other = other->next) {
                if (other->type == 17 && other != item)
                    static_cast<edui_prop_s *>(other)->label_width = property->label_width;
            }
            if (!(edui_cursor_buttons & EDUI_CURSOR_PRIMARY))
                *property_flags &= ~2;
        }
        eduicbProcessPropKeyboard(menu, property);
        eduiSetCameraEnabled(1);
        if ((property->unknown_property_flags & 8) ||
            (item->type == 17 && (pad->digital_buttons & EDUI_CURSOR_PRIMARY))) {
            if (property->button) {
                eduiSetCameraEnabled(0);
                property->button(menu, item, edui_cursor_buttons);
                property->unknown_property_flags &= ~8;
            }
            if (!(edui_cursor_buttons & EDUI_CURSOR_PRIMARY))
                property->unknown_property_flags &= ~8;
        }
        return 0;
    }

    eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data) {
        edui_prop_s *item = static_cast<edui_prop_s *>(NU_ALLOC(sizeof(edui_prop_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 17;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessProp;
        item->render = eduicbRenderProp;
        item->destroy = eduicbItemDestroyProp;
        item->interact = eduicbInteractProp;
        item->highlighted = 0;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        eduiItemPropSetText(item, value);
        item->button_type = button_type;
        item->label_width = 50.0f;
        item->selected = selected;
        item->changed = changed;
        item->button = button;
        item->extra_data = extra_data;
        return item;
    }

    eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value) {
        return eduiItemPropCreateEx(data, colours, selected, changed, button, button_type, text, value, 0);
    }

    eduiitem_s *eduiItemSeparatorCreate(usize data, const void *colours) {
        eduiitem_s *item = static_cast<eduiitem_s *>(NU_ALLOC(sizeof(eduiitem_s), 4, 1, "", 0));
        if (item) {
            memset(item, 0, sizeof(*item));
            item->type = 18;
            item->data = data;
            memcpy(item->colours, colours, sizeof(item->colours));
            item->process = eduicbProcessSeparator;
            item->render = eduicbRenderSeparator;
            item->text_alignment = 0x40;
            item->highlighted = 0;
            item->destroy = eduicbItemDestroy;
            item->selection_group = 0;
        }
        return item;
    }

    void eduicbMenuOpenAllexpanders(eduimenu_s *menu) {
        for (eduiitem_s *item = menu->first; item; item = item->next) {
            if (item->type == EDUI_ITEM_EXPANDER) {
                edui_expander_s *expander = static_cast<edui_expander_s *>(item);
                eduiitem_s *child = expander->first_child;
                if (child && !expander->open) {
                    expander->last_child->next = expander->next;
                    if (expander->next)
                        expander->next->previous = expander->last_child;
                    child->previous = expander;
                    expander->next = child;
                    expander->open = 1;
                }
            }
        }
    }

    void eduicbMenuCloseAllexpanders(eduimenu_s *menu) {
        for (eduiitem_s *item = menu->last; item; item = item->previous) {
            if (item->type == EDUI_ITEM_EXPANDER)
                eduicbItemExpanderClose(static_cast<edui_expander_s *>(item));
        }
    }

    void eduiMenuFitWidth(eduimenu_s *menu, i32 padding) {
        eduiitem_s *first = menu->first;
        eduicbMenuOpenAllexpanders(menu);
        NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
        NuQFntSetScale(edui_font, edui_font_scale_x, edui_font_scale_y);

        i32 width = 0;
        if (menu->title) {
            i32 title_width = static_cast<i32>(NuQFntPrintLenU(edui_font, menu->title)) >> 4;
            if (title_width > width)
                width = title_width;
        }
        i32 property_label_width = 0;
        i32 property_value_width = 0;

        for (eduiitem_s *item = first; item; item = item->next) {
            i32 item_text_width = 0;
            switch (item->type) {
                case 1:
                    item_text_width = (static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3) * 10;
                    break;
                case 2:
                case 3:
                    if (item->text) {
                        item_text_width = (static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3) +
                                          (static_cast<i32>(NuQFntPrintLenU(edui_font, item->text)) >> 4);
                    }
                    break;
                case 17: {
                    edui_prop_s *property = static_cast<edui_prop_s *>(item);
                    i32 label_width = item->text ? static_cast<i32>(NuQFntPrintLenU(edui_font, item->text)) >> 4 : 30;
                    if (label_width > property_label_width)
                        property_label_width = label_width;
                    i32 value_width = property->property_text
                                          ? static_cast<i32>(NuQFntPrintLenU(edui_font, property->property_text)) >> 4
                                          : 30;
                    if (value_width > property_value_width)
                        property_value_width = value_width;
                    item_text_width = value_width;
                    break;
                }
                case 6: {
                    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
                    char format[512];
                    char text[512];
                    NuStrCpy(format, item->text);
                    NuStrCat(format, slider->format);
                    sprintf(text, format, 0, 0x80000000);
                    item_text_width = static_cast<i32>(NuQFntPrintLenU(edui_font, text)) >> 4;
                    break;
                }
                default:
                    if (item->text)
                        item_text_width = static_cast<i32>(NuQFntPrintLenU(edui_font, item->text)) >> 4;
                    break;
            }
            if (item_text_width > width)
                width = item_text_width;
        }

        i32 property_width =
            property_label_width + property_value_width + (static_cast<i32>(NuQFntHeight(edui_font)) >> 3);
        if (property_width > width)
            width = property_width;

        for (eduiitem_s *item = menu->first; item; item = item->next) {
            if (item->type == 17)
                static_cast<edui_prop_s *>(item)->label_width = static_cast<f32>(property_label_width + 1);
        }
        menu->width = width + padding;

        NuQFntPopCoordinateSystem();
        eduicbMenuCloseAllexpanders(menu);
    }

    f32 edmainSetMainMenuScale(f32 scale) {
        f32 previous = edmain_menu_scale;
        edmain_menu_scale = scale;
        if (ed_main_menu)
            eduiMenuFitWidth(ed_main_menu, 8);
        return previous;
    }

extern "C" void eduiMenuDestroyItems(eduimenu_s *menu) {
    if (menu) {
        eduicbMenuCloseAllexpanders(menu);
        while (menu->first) {
            eduiitem_s *next = menu->first->next;
            menu->first->destroy(menu, menu->first);
            menu->first = next;
        }
        menu->last = NULL;
        menu->selected = NULL;
        menu->field_0c = NULL;
        menu->field_10 = NULL;
    }
}

    void eduiMenuDestroy(eduimenu_s *menu) {
        if (menu) {
            if (eduiInteractLocked && eduiInteractLocked->menu == menu)
                eduiInteractLocked = NULL;
            if (processing_menu == menu)
                processing_menu = NULL;
            if (menu->parent) {
                if (active_menu == menu)
                    active_menu = menu->parent;
                menu->parent->child = NULL;
            }
            if (default_active_menu == menu)
                default_active_menu = NULL;
            if (active_menu == menu)
                active_menu = NULL;
            if (menu->child)
                menu->child->parent = NULL;
            eduiMenuDestroyItems(menu);
            NU_FREE(menu);
        }
    }

    void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *) {
        eduiMenuDestroy(edui_messagemenu);
        edui_messagemenu = NULL;
    }

    static __used__ void eduicbPickCFGCancel(eduimenu_s *menu, eduimenu_s *) {
        eduiMenuDestroy(menu);
    }

    void edmainClose(void) {
        if (ed_init) {
            NuCameraDestroy(edinternalcam);
            while (ed_list) {
                if (ed_list->close)
                    ed_list->close();
                ed_list = ed_list->next;
            }
            if (ed_main_menu) {
                eduiMenuDestroy(ed_main_menu);
                ed_main_menu = NULL;
            }
            if (ed_cfg_menu) {
                eduiMenuDestroy(ed_cfg_menu);
                ed_cfg_menu = NULL;
            }
            ed_list = NULL;
            ed_curr = NULL;
            ed_init = 0;
        }
    }

    i32 edmainRegister(ed_module_s *module) {
        if (ed_init)
            edmainClose();
        if (ed_list)
            ed_list->previous = module;
        module->next = ed_list;
        ed_list = module;
        module->previous = NULL;
        return 1;
    }

    void eduiIitemExpanderSetDepth(edui_expander_s *item, i32 depth) {
        item->depth = depth;
        for (eduiitem_s *child = item->first_child; child; child = child->next) {
            if (child->type == 19)
                eduiIitemExpanderSetDepth(static_cast<edui_expander_s *>(child), depth + 1);
            if (child->type == 17)
                static_cast<edui_prop_s *>(child)->depth = depth + 1;
            if (child == item->last_child)
                break;
        }
    }

    void eduiItemExpanderAddChild(edui_expander_s *item, eduiitem_s *child) {
        child->previous = item->last_child;
        if (item->last_child)
            item->last_child->next = child;
        item->last_child = child;
        if (!item->first_child)
            item->first_child = child;
        child->next = NULL;
        if (child->type == 19)
            eduiIitemExpanderSetDepth(static_cast<edui_expander_s *>(child), item->depth + 1);
        if (child->type == 17)
            static_cast<edui_prop_s *>(child)->depth = item->depth + 1;
    }

    eduiitem_s *eduiItemExpanderCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        edui_expander_s *item = static_cast<edui_expander_s *>(NU_ALLOC(sizeof(edui_expander_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 19;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessExpander;
        item->render = eduicbRenderExpander;
        item->interact = eduicbInteractExpander;
        item->destroy = eduicbItemDestroyExpander;
        item->highlighted = 0;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        item->changed = callback;
        return item;
    }

    void eduiItemFilterAddItem(edui_filter_s *item, eduiitem_s *child) {
        eduiitem_s *first = item->first_child;
        child->previous = NULL;
        child->next = first;
        if (first)
            first->previous = child;
        item->first_child = child;
    }

    void eduiItemFilterRemoveItem(edui_filter_s *item, eduiitem_s *child) {
        if (item->first_child == child)
            item->first_child = child->next;
        if (child->next)
            child->next->previous = child->previous;
        if (child->previous)
            child->previous->next = child->next;
        child->next = NULL;
        child->previous = NULL;
    }

    static __used__ i32 eduicbProcessFilter(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad) {
        auto *filter = static_cast<edui_filter_s *>(item);
        (void)delta_time;
        u8 *filter_flags = reinterpret_cast<u8 *>(filter) + 0x4c;
        if (__builtin_expect(!!(*filter_flags & 2), 1)) {
            filter->label_width = edui_cursor_x - static_cast<f32>(item->x);
            if (filter->label_width < 5.0f)
                filter->label_width = 5.0f;
            for (eduiitem_s *other = menu->first; other; other = other->next) {
                if (__builtin_expect(other->type == 17 && other != item, 0))
                    static_cast<edui_prop_s *>(other)->label_width = filter->label_width;
            }
            if (!(edui_cursor_buttons & EDUI_CURSOR_PRIMARY))
                *filter_flags &= ~2;
        }
        eduicbProcessPropKeyboard(menu, filter);
        eduiSetCameraEnabled(1);
        if ((filter->unknown_property_flags & 8) ||
            (pad && item->type == 17 && (pad->digital_buttons & EDUI_CURSOR_PRIMARY))) {
            if (filter->button) {
                eduiSetCameraEnabled(0);
                filter->button(menu, item, edui_cursor_buttons);
                filter->unknown_property_flags &= ~8;
            }
            if (!(edui_cursor_buttons & EDUI_CURSOR_PRIMARY))
                filter->unknown_property_flags &= ~8;
        }
        char *query = (filter->unknown_property_flags & 1) ? eduiPropTextEdit : filter->property_text;
        i32 changed = 0;
        if (!(!*query)) {
            for (eduiitem_s *candidate = menu->first; candidate;) {
                eduiitem_s *next = candidate->next;
                if (candidate->type != 18 && candidate->type != 20 && !NuStrIStr(candidate->text, query)) {
                    eduiMenuRemoveItem(menu, candidate);
                    eduiItemFilterAddItem(filter, candidate);
                    changed = 1;
                }
                candidate = next;
            }
            for (eduiitem_s *candidate = filter->first_child; candidate;) {
                eduiitem_s *next = candidate->next;
                if (NuStrIStr(candidate->text, query)) {
                    eduiItemFilterRemoveItem(filter, candidate);
                    eduiMenuAddItem(menu, candidate);
                    changed = 1;
                }
                candidate = next;
            }
        } else {
            for (eduiitem_s *child = filter->first_child; child;) {
                eduiitem_s *next = child->next;
                eduiItemFilterRemoveItem(filter, child);
                eduiMenuAddItem(menu, child);
                child = next;
                changed = 1;
            }
        }
        if (changed || !(filter->unknown_84[4] & 1)) {
            eduiMenuSortItemsByTxt(menu);
            filter->unknown_84[4] |= 1;
        }
        return 0;
    }

    eduiitem_s *eduiItemFilterCreate(usize data, const void *colours, char *text, char *value) {
        edui_filter_s *item = static_cast<edui_filter_s *>(NU_ALLOC(sizeof(edui_filter_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 20;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessFilter;
        item->render = eduicbRenderFilter;
        item->interact = eduicbInteractFilter;
        item->destroy = eduicbItemDestroyFilter;
        item->highlighted = 0;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        eduiItemPropSetText(item, value);
        item->label_width = 50.0f;
        return item;
    }

    i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected) {
        item_width = width;
        i32 height = item->render(menu, item, x, y, width, selected);
        if (selected) {
            u32 colour = (item->flags & 4) ? eduiitem_context_outline_colour : eduiitem_selected_outline_colour;
            if (__builtin_expect(!edui_donotdraw, 1))
                NuRndrLineRect2di(x << 4, (y << 3) + 8, item_width << 4, (height << 3) - 16, colour, uimtls[ui_outmtl]);
        }
        if (item->disabled) {
            i32 left = x << 4;
            i32 top = y << 3;
            i32 span = width << 4;
            i32 bottom = top + (height << 3);
            eduiDrawLine(left, top, left + span / 4, bottom, item->colours[0], uimtls[ui_bgmtl]);
            eduiDrawLine(left + span / 4, top, left + span / 2, bottom, item->colours[0], uimtls[ui_bgmtl]);
            eduiDrawLine(left + span / 2, top, left + span * 3 / 4, bottom, item->colours[0], uimtls[ui_bgmtl]);
            eduiDrawLine(left + span * 3 / 4, top, left + span, bottom, item->colours[0], uimtls[ui_bgmtl]);
        }
        return height;
    }

    void eduiSetUsingMenuFocus(i32 enabled) {
        bUsingMenuFocus = enabled;
    }

    i32 eduiGetUsingMenuFocus(void) {
        return bUsingMenuFocus;
    }

    void eduiSetDefaultActiveMenu(eduimenu_s *menu) {
        default_active_menu = menu;
    }

    void eduiSetActiveMenu(eduimenu_s *menu) {
        if (eduiGetUsingMenuFocus()) {
            if (menu) {
                while (__builtin_expect(menu->child != NULL, 0))
                    menu = menu->child;
                active_menu = menu;
            } else {
                menu = default_active_menu;
                if (menu)
                    while (menu->child)
                        menu = menu->child;
                active_menu = menu;
            }
        }
    }

    i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child) {
        if (child->parent)
            return 0;
        while (menu->child)
            menu = menu->child;
        child->parent = menu;
        menu->child = child;
        eduiSetActiveMenu(child);
        return 1;
    }

    static __used__ void cbSubMenu(eduimenu_s *menu, eduiitem_s *item) {
        eduiMenuAttach(menu, reinterpret_cast<eduimenu_s *>(item->data_ptr));
    }

    void eduiCreate3LineMessageMenu(eduimenu_s *parent, char *first, char *second, char *third, i32 first_highlight,
                                    i32 second_highlight, i32 third_highlight) {
        eduiiattr_s normal{0x800000c0, 0x80ff0000, 0x80808080, 0x80404040};
        eduiiattr_s highlighted{0x8000c000, 0x80ff0000, 0x80808080, 0x80404040};
        edui_messagemenu =
            eduiMenuCreate(70, 70, 300, 250, parent->font, eduicbCancelMessageMenu, const_cast<char *>("Message"));
        if (edui_messagemenu) {
            if (first)
                eduiMenuAddItem(edui_messagemenu,
                                eduiItemSelCreate(1, first_highlight ? &highlighted : &normal, 0, 0, NULL, first));
            if (second)
                eduiMenuAddItem(edui_messagemenu,
                                eduiItemSelCreate(1, second_highlight ? &highlighted : &normal, 0, 0, NULL, second));
            if (third)
                eduiMenuAddItem(edui_messagemenu,
                                eduiItemSelCreate(1, third_highlight ? &highlighted : &normal, 0, 0, NULL, third));
            eduiMenuAttach(parent, edui_messagemenu);
            edui_messagemenu->x = parent->x + 10;
            edui_messagemenu->y = parent->y + 40;
        }
    }

    void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted) {
        static const u32 initial_colours[4]
            __attribute__((aligned(16))) = {0x800000c0u, 0x80ff0000, 0x80808080, 0x80404040};
        u32 colours[4] __attribute__((aligned(16)));
        memcpy(colours, initial_colours, sizeof(colours));
        if (highlighted == 1)
            colours[0] = 0x8000c000u;
        edui_messagemenu =
            eduiMenuCreate(70, 70, 180, 250, parent->font, eduicbCancelMessageMenu, const_cast<char *>("Message"));
        if (!edui_messagemenu)
            return;
        eduiMenuAddItem(edui_messagemenu, eduiItemSelCreate(1, colours, 0, 0, NULL, message));
        eduiMenuAttach(parent, edui_messagemenu);
        edui_messagemenu->x = parent->x + 10;
        edui_messagemenu->y = parent->y + 40;
    }

    static __used__ i32 eduicbProcessGradPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad) {
        (void)delta_time;
        edui_gradient_pick_s *gradient = static_cast<edui_gradient_pick_s *>(item);
        if (gradient->changed)
            gradient->changed(menu, item, pad->digital_buttons);
        if (pad->digital_buttons_pressed & 0x40) {
            static eduimenu_s *gpcfg;
            gpcfg = eduiMenuCreate(menu->x + 10, menu->y + 40, 180, 250, menu->font, eduicbPickCFGCancel,
                                   "GradPick Adjust");
            if (!gpcfg)
                return 0;
            eduiitem_s *pick;
            if (item->type == 8) {
                pick = eduiItemGreyPickCreate(reinterpret_cast<usize>(item), &ed_attr, cbgpcfgCPPress, "Colourpick");
                if (gradient->selected_stage)
                    eduiItemColourPickSetHSV(static_cast<edui_colour_pick_s *>(pick), gradient->selected_stage->hue,
                                             gradient->selected_stage->saturation, gradient->selected_stage->value);
            } else if (item->type == 9)
                pick = eduiItemSelCreate(reinterpret_cast<usize>(item), &ed_attr, 0, 0, cbgpcfgCPPress, "Edit");
            else {
                pick = eduiItemColourPickCreate(reinterpret_cast<usize>(item), &ed_attr, cbgpcfgCPPress, "Colourpick");
                if (gradient->selected_stage)
                    eduiItemColourPickSetHSV(static_cast<edui_colour_pick_s *>(pick), gradient->selected_stage->hue,
                                             gradient->selected_stage->saturation, gradient->selected_stage->value);
            }
            eduiMenuAddItem(gpcfg, pick);
            eduiMenuAddItem(gpcfg, eduiItemSelCreate(reinterpret_cast<usize>(item), &ed_attr, 0, 0, cbgpcfgAdd, "Add"));
            eduiMenuAddItem(gpcfg, eduiItemSelCreate(reinterpret_cast<usize>(item), &ed_attr, 0, 0, cbgpcfgDel, "Del"));
            eduiMenuAddItem(gpcfg,
                            eduiItemSelCreate(reinterpret_cast<usize>(item), &ed_attr, 0, 0, cbgpcfgCopy, "Copy"));
            edui_last_item->type = item->type;
            eduiMenuAddItem(gpcfg, eduiItemSelWithClipColourCreate(reinterpret_cast<usize>(item), &ed_attr, 0, 0,
                                                                   cbgpcfgPaste, "Paste"));
            edui_last_item->type = item->type;
            eduiMenuAttach(menu, gpcfg);
            return 0;
        }
        if (!gradient->selected_stage)
            gradient->selected_stage = gradient->first_stage;
        edui_gradient_node_s *stage = gradient->selected_stage;
        if (!stage)
            return 0;
        if (pad->digital_buttons_pressed & 0x8000) {
            if (stage->previous)
                gradient->selected_stage = stage->previous;
            return 1;
        }
        if (pad->digital_buttons_pressed & 0x2000) {
            if (stage->next)
                gradient->selected_stage = stage->next;
            return 1;
        }
        if (pad->digital_buttons & 0xa000)
            return 1;
        f32 lower = stage->previous ? stage->previous->time : 0.0f;
        f32 upper = stage->next ? stage->next->time : 1.0f;
        u8 right2 = pad->analog_r2;
        u8 left2 = pad->analog_l2;
        u8 left1 = pad->analog_l1;
        u8 right1 = pad->analog_r1;
        i32 decrease = left1 ? left1 : left2;
        i32 increase = right1 ? right1 : right2;
        if (!(increase && !decrease)) {
            if (decrease) {
                f32 time = stage->time - static_cast<f32>(decrease) / 15360.0f;
                stage->time = lower > time ? lower : time;
                gradient->change_timer = 60;
            }
        } else {
            f32 time = static_cast<f32>(increase) / 15360.0f + stage->time;
            stage->time = upper < time ? upper : time;
            gradient->change_timer = 60;
        }
        return 0;
    }

    static void eduicbAttachDirectoryList(eduimenu_s *menu, edui_file_pick_s *picker) {
        if (!dir_list)
            return;

        picker->reopen_directory = 0;
        filepick_menu = menu;
        filepick_item = picker;
        eduimenu_s *directory_menu = eduiMenuCreate(menu->x + 20, menu->y + 20, 250, 300, menu->font,
                                                    eduicbDestroyDirectoryList, const_cast<char *>("Directory"));
        if (!directory_menu)
            return;

        char text[1032];
        sprintf(text, "%s%s", picker->directory, picker->filename);
        NUFILE directory = NuFileOpenDir(text);
        if (directory) {
            dir_size = 0;
            auto *entries = static_cast<FilePickDirectoryEntry *>(dir_list);
            while (dir_size < edui_filepick_listsize && NuFileReadDir(directory, &entries[dir_size]) > 0)
                ++dir_size;
            if (picker->compare_entries)
                qsort(entries, dir_size, sizeof(FilePickDirectoryEntry), picker->compare_entries);

            if (picker->filename[0]) {
                eduiitem_s *parent = eduiItemSelCreate(9999, picker->colours, 0, 1, eduicbSelectDirectoryEntry,
                                                       const_cast<char *>(".."));
                parent->text_alignment = 0x10;
                eduiMenuAddItem(directory_menu, parent);
            }

            for (i32 index = 0; index < dir_size; ++index) {
                FilePickDirectoryEntry &entry = entries[index];
                if (!(entry.flags & 8))
                    continue;
                sprintf(text, "<%s>", entry.name);
                eduiitem_s *item = eduiItemSelCreate(index, picker->colours, 0, 1, eduicbSelectDirectoryEntry, text);
                item->text_alignment = 0x10;
                eduiMenuAddItem(directory_menu, item);
            }

            for (i32 index = 0; index < dir_size; ++index) {
                FilePickDirectoryEntry &entry = entries[index];
                if (entry.flags & 8 || (picker->filename[0x100] && !NuStrIStr(entry.name, picker->filename + 0x100)))
                    continue;
                sprintf(text, "%s  ( size=%d  date=%d/%d/%d %d:%d )", entry.name, entry.size, entry.month, entry.day,
                        entry.year, entry.hour, entry.minute);
                eduiitem_s *item = eduiItemSelCreate(index, picker->colours, 0, 1, eduicbSelectDirectoryEntry, text);
                item->text_alignment = 0x10;
                eduiMenuAddItem(directory_menu, item);
            }

            if (!directory_menu->first)
                eduiMenuAddItem(directory_menu,
                                eduiItemSelCreate(dir_size, picker->colours, 0, 1, eduicbSelectDirectoryExit,
                                                  const_cast<char *>("No Files")));
            NuFileCloseDir(directory);
        }
        eduiMenuFitWidth(directory_menu, 8);
        eduiMenuAttach(menu, directory_menu);
    }

    static __used__ i32 eduicbProcessFilePick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad) {
        (void)delta_time;
        auto *picker = static_cast<edui_file_pick_s *>(item);
        if ((pad->digital_buttons_pressed & EDUI_CURSOR_PRIMARY) || picker->reopen_directory) {
            eduiMenuHighlight(menu, item);
            eduicbAttachDirectoryList(menu, picker);
        }
        return 0;
    }

    edui_textpicker_s *eduiAddTextPickEntEx(eduimenu_s *menu, eduiitem_s *source, EdUiItemCallback callback) {
        static eduimenu_s *tpent;
        tpent =
            eduiMenuCreate(source->x + 10, source->y + 10, 180, 50, menu->font, NULL, const_cast<char *>("Enter Text"));
        tpent->flags |= 2;
        auto *picker = static_cast<edui_textpicker_s *>(
            eduiItemTextPickCreate(reinterpret_cast<usize>(source), &ed_attr, callback, const_cast<char *>("")));
        eduiMenuAddItem(tpent, picker);
        eduiMenuAttach(menu, tpent);
        picker->editing = 1;
        eduiMenuFitWidth(tpent, 8);
        picker->cursor = NuStrLen(picker->value);
        return picker;
    }

    void eduiAddPropTextPickEnt(eduimenu_s *menu, eduiitem_s *source) {
        auto *picker = eduiAddTextPickEntEx(menu, source, tpentPropOnEnter);
        if (!picker)
            return;
        NuStrCpy(picker->value, static_cast<edui_prop_s *>(source)->property_text);
        picker->cursor = NuStrLen(picker->value);
        memcpy(picker->colours, source->colours, sizeof(picker->colours));
    }

    void eduiAddTextPickEnt(eduimenu_s *menu, eduiitem_s *source) {
        auto *picker = eduiAddTextPickEntEx(menu, source, tpentOnEnter);
        if (!picker)
            return;
        NuStrCpy(picker->value, static_cast<edui_textpicker_s *>(source)->value);
        picker->cursor = NuStrLen(picker->value);
        memcpy(picker->colours, source->colours, sizeof(picker->colours));
        picker->max_length = static_cast<edui_textpicker_s *>(source)->max_length;
        u8 *picker_flags = reinterpret_cast<u8 *>(&picker->keyboard_flags);
        const u8 *source_flags =
            reinterpret_cast<const u8 *>(&static_cast<edui_textpicker_s *>(source)->keyboard_flags);
        *picker_flags = (*picker_flags & ~2u) | (*source_flags & 2u);
    }

    static __used__ i32 eduicbProcessTextPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad) {
        auto *picker = static_cast<edui_textpicker_s *>(item);
        u32 modifiers;
        i32 key = NuKeyGet(&modifiers);
        NuStrLen(picker->value);

        auto delete_previous = [&]() {
            if (__builtin_expect(picker->cursor == 0, 0))
                return;
            if (picker->value[picker->cursor]) {
                --picker->cursor;
                for (i32 index = picker->cursor; index < 0xff; ++index)
                    picker->value[index] = picker->value[index + 1];
            } else {
                picker->value[--picker->cursor] = '\0';
            }
        };
        auto insert_character = [&](char character) {
            if (__builtin_expect(NuStrLen(picker->value) >= picker->max_length, 0))
                return;
            // The original includes the cursor slot in the shift before
            // overwriting it. Read the enclosing object's bytes so cursor zero
            // accesses the preceding object byte rather than value[-1].
            const char *picker_bytes = reinterpret_cast<const char *>(picker);
            for (i32 index = 0xff; index >= picker->cursor; --index)
                picker->value[index] = picker_bytes[offsetof(edui_textpicker_s, value) + index - 1];
            picker->value[picker->cursor++] = character;
        };
        auto next_row = [&]() {
            if (++picker->keyboard_row > ((picker->keyboard_flags & 2) ? 5 : 3))
                picker->keyboard_row = 0;
        };
        auto previous_row = [&]() {
            if (__builtin_expect(--picker->keyboard_row < 0, 1))
                picker->keyboard_row = (picker->keyboard_flags & 2) ? 5 : 3;
        };
        auto next_column = [&]() {
            if (__builtin_expect(picker->keyboard_row >= 4, 0))
                return;
            if (++picker->keyboard_column >= NuStrLen(textrow[picker->keyboard_row]))
                picker->keyboard_column = 0;
        };
        auto previous_column = [&]() {
            if (picker->keyboard_row >= 4)
                return;
            if (--picker->keyboard_column < 0)
                picker->keyboard_column = NuStrLen(textrow[picker->keyboard_row]) - 1;
        };

        if (__builtin_expect(key != -1, 1)) {
            switch (key) {
                case 0xc8:
                    previous_row();
                    break;
                case 0xd0:
                    next_row();
                    break;
                case 0xcb:
                    previous_column();
                    break;
                case 0xcd:
                    next_column();
                    break;
                case 0x0e:
                case 0xd3:
                    delete_previous();
                    break;
                case 0x1c:
                case 1:
                    if (__builtin_expect(!!(picker->callback), 0))
                        picker->callback(menu, item, pad->digital_buttons);
                    break;
                default: {
                    char character = NuKeyToAscii(key, modifiers & 1);
                    if (character)
                        insert_character(character);
                } break;
            }
        }

        if (__builtin_expect(!picker || !picker->editing, 0)) {
            if (pad->digital_buttons_pressed & 0x40)
                eduiAddTextPickEnt(menu, item);
            return 0;
        }

        if (__builtin_expect(!!(pad->digital_buttons_pressed & 0x4000), 0))
            next_row();
        if (__builtin_expect(!!(pad->digital_buttons_pressed & 0x1000), 0))
            previous_row();
        if (__builtin_expect(!!(pad->digital_buttons_pressed & 0x8000), 1))
            previous_column();
        if (pad->digital_buttons_pressed & 0x2000)
            next_column();
        if ((pad->digital_buttons_pressed & 0x10) && picker->callback)
            picker->callback(menu, item, pad->digital_buttons);
        if ((pad->digital_buttons & 0xc) == 0xc) {
            picker->cursor = 0;
            picker->value[0] = '\0';
            if (__builtin_expect((picker->keyboard_flags & 2) && TextPickCopyBuffer[0], 0)) {
                NuStrNCpy(picker->value, TextPickCopyBuffer, picker->max_length - 1);
                picker->value[picker->max_length - 1] = '\0';
                picker->cursor = NuStrLen(picker->value);
            }
        }
        if ((pad->digital_buttons & 3) == 3 && (picker->keyboard_flags & 2)) {
            NuStrNCpy(TextPickCopyBuffer, picker->value, 0x40);
            picker->value[0x3f] = '\0';
        }
        if (pad->digital_buttons_pressed & 4)
            picker->cursor = picker->cursor > 0 ? picker->cursor - 1 : 0;
        if (pad->digital_buttons_pressed & 8) {
            if (__builtin_expect(picker->cursor < NuStrLen(picker->value), 0))
                ++picker->cursor;
            else
                picker->cursor = NuStrLen(picker->value);
        }
        if (pad->digital_buttons_pressed & 0x40)
            insert_character(textrow[picker->keyboard_row][picker->keyboard_column]);
        if (__builtin_expect(!!(pad->digital_buttons_pressed & 0x80), 1))
            delete_previous();
        if (__builtin_expect(!!(pad->digital_buttons_pressed & 0x20), 0)) {
            if (textrow[1] == const_cast<char *>("QWERTYUIOP")) {
                textrow[0] = const_cast<char *>("1234567890");
                textrow[1] = const_cast<char *>("qwertyuiop");
                textrow[2] = const_cast<char *>("asdfghjkl_");
                textrow[3] = const_cast<char *>("zxcvbnm<>?");
            } else {
                textrow[0] = const_cast<char *>("1234567890");
                textrow[1] = const_cast<char *>("QWERTYUIOP");
                textrow[2] = const_cast<char *>("ASDFGHJKL_");
                textrow[3] = const_cast<char *>("ZXCVBNM<>?");
            }
        }
        return 0;
    }

    static __used__ void cbMMEditorConfig(eduimenu_s *menu, eduiitem_s *, u32) {
        ed_cfg_menu->x = menu->x + 10;
        ed_cfg_menu->y = menu->y + 10;
        eduiMenuAttach(menu, ed_cfg_menu);
    }

    eduimenu_s *eduiGetActiveMenu(void) {
        return active_menu;
    }

    void eduiMenuRender(eduimenu_s *menu) {
        const auto font_set = [](void *font) { eduiDrawFontSet(font); };
        const auto font_colour = [](void *font, u32 colour) { eduiSetFontColour(font, colour); };
        const auto rect = [](i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material) {
            eduiDrawRect(x, y, width, height, colour, material);
        };
        const auto line = [](i32 x0, i32 y0, i32 x1, i32 y1, i32 colour, numtl_s *material) {
            eduiDrawLine(x0, y0, x1, y1, colour, material);
        };
        const auto outline_rect = [](i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material) {
            eduiDrawLineRect(x, y, width, height, colour, material);
        };
        const auto interact = [](i32 x, i32 y, i32 width, i32 height, eduimenu_s *menu, eduiitem_s *item,
                                 i32 (*callback)(edui_interact_s *), u32 buttons, i32 field_20) {
            if (__builtin_expect(numInteracts < 64 && !eduiInteractLocked, 0)) {
                edui_interact_s *entry = &eduiInteracts[numInteracts];
                entry->x = static_cast<f32>(x);
                entry->menu = menu;
                entry->item = item;
                entry->callback = callback;
                entry->buttons = buttons;
                entry->y = static_cast<f32>(y) * 0.5f;
                entry->width = static_cast<f32>(width);
                entry->height = static_cast<f32>(height) * 0.5f;
                entry->field_20 = field_20;
                ++numInteracts;
            }
        };
        ++NuPrimCSPos;
        i32 x = menu->x;
        i32 y = menu->y;
        NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_PS2);
        NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
        NuQFntPushPrintMode(2);
        font_set(edui_font);
        NuQFntSetScale(edui_font, edui_font_scale_x, edui_font_scale_y);

        const f32 font_height = NuQFntHeight(edui_font);
        const f32 title_font_height = NuQFntHeight(edui_font);
        const f32 baseline = NuQFntBaseline(edui_font);
        if (menu->title && menu->title[0]) {
            const i32 title_height = static_cast<i32>(font_height * 0.15625f);
            if (!menu->child)
                interact(x, y, menu->width, title_height, menu, NULL, cbInteractMenuTitle, 0x40, 0);
            rect(x << 4, y << 3, menu->width << 4, title_height << 3, 0x80000000, uimtls[ui_bgmtl]);
            font_set(edui_font);
            font_colour(edui_font, 0x807f7f7f);
            eduiFntPrintClipEx(edui_font, (x + 1) << 4,
                               static_cast<i32>(title_font_height * 0.125f + baseline) + (y << 3), 0x10, x << 4,
                               menu->width << 4, menu->title);
            y += title_height;
        }

        eduiitem_s *item = menu->field_0c;
        i32 max_item_height = 0;
        if (item) {
            i32 find_selected = menu->selected != NULL;

            if (item != menu->first) {
                const i32 scroll_height = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f);
                if (!menu->child)
                    interact(x, y, menu->width, scroll_height, menu, NULL, cbInteractMenuScrollUp, 0x40, 0);
                rect(x << 4, y << 3, menu->width << 4, scroll_height << 3, 0x80000000, uimtls[ui_bgmtl]);
                const i32 top = (y << 3) + 8;
                const i32 bottom = ((y + scroll_height) << 3) - 16;
                i32 centre = (x * 2 + menu->width) << 3;
                line(centre, top, centre - ((scroll_height - 2) << 4), bottom, 0x80ffffff, uimtls[0]);
                centre = (x * 2 + menu->width) << 3;
                line(centre, top, centre + ((scroll_height - 2) << 4), bottom, 0x80ffffff, uimtls[0]);
                y += scroll_height;
            }

            while (item) {
                menu->field_10 = item;
                i32 is_selected;
                if (__builtin_expect(item != menu->selected, 0)) {
                    is_selected = 0;
                } else {
                    find_selected = 0;
                    if (menu->child)
                        is_selected = 0;
                    else
                        is_selected = 1;
                }

                const i32 item_height = eduiItemRender(item, menu, x, y, menu->width, is_selected);
                if (item_height > max_item_height)
                    max_item_height = item_height;

                if (!menu->child)
                    interact(x, y, menu->width, item_height, menu, item, reinterpret_cast<edui_sel_s *>(item)->interact,
                             0x40, 0);

                item = item->next;
                if (!(menu->flags & 2))
                    y += item_height;
                else
                    x += menu->width;

                if (!item)
                    break;
                if (!(menu->y + menu->height <= y)) {
                    continue;
                }
                const i32 scroll_height = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f);
                if (__builtin_expect(!menu->child, 1))
                    interact(x, y, menu->width, scroll_height, menu, NULL, cbInteractMenuScrollDown, 0x40, 0);
                rect(x << 4, y << 3, menu->width << 4, scroll_height << 3, 0x80000000, uimtls[ui_bgmtl]);
                const i32 bottom = ((y + scroll_height - 2) << 3);
                i32 centre = (x * 2 + menu->width) << 3;
                const i32 top = (y << 3) + 8;
                line(centre, bottom, centre - ((scroll_height - 2) << 4), top, 0x80ffffff, uimtls[0]);
                centre = (x * 2 + menu->width) << 3;
                line(centre, bottom, centre + ((scroll_height - 2) << 4), top, 0x80ffffff, uimtls[0]);
                break;
            }

            if (find_selected && menu->field_0c->next)
                menu->field_0c = menu->field_0c->next;
        }

        if (!(menu->flags & 2))
            x = menu->x + menu->width;
        else
            y += max_item_height;
        menu->field_24 = x - menu->x;
        menu->field_28 = y - menu->y;

        const u32 outline = eduiGetActiveMenu() == menu ? eduimenu_selected_outline_colour : eduimenu_outline_colour;
        outline_rect((menu->x - 1) << 4, (menu->y << 3) - 8, (menu->field_24 + 2) << 4, (menu->field_28 << 3) + 16,
                     outline, uimtls[ui_outmtl]);

        if (menu->child) {
            interact(menu->x, menu->y, menu->field_24, menu->field_28, menu, NULL, cbInteractMenuCancelChild, 0x40, 1);
            eduiMenuRender(menu->child);
        }

        NuQFntPopPrintMode();
        NuQFntPopCoordinateSystem();
        --NuPrimCSPos;
        NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[NuPrimCSPos]);
    }

    void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding) {
        if (menu->field_24 < 0) {
            i32 saved = edui_donotdraw;
            edui_donotdraw = 1;
            eduiMenuRender(menu);
            edui_donotdraw = saved;
        }
        if (menu->x + menu->field_24 > 640 - padding)
            menu->x -= menu->field_24;
        if (menu->x < padding)
            menu->x = padding;
        if (menu->y + menu->field_28 > 448 - padding)
            menu->y -= menu->field_28;
        if (menu->y < padding)
            menu->y = padding;
    }

    static __used__ i32 eduicbRenderProp(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        char prefix[256];
        edui_prop_s *property = static_cast<edui_prop_s *>(item);
        f32 draw_x = x;
        f32 draw_y = y;
        f32 draw_width = width;
        u32 divider_colours[2] = {0x0ff00000, 0xffffffff};
        u32 button_colours[2] = {0xff000000, 0xffffffff};
        item->x = x;
        item->y = y;
        u8 *flags = reinterpret_cast<u8 *>(property) + 0x4c;
        u32 highlighted;
        if (edui_cursor_x >= property->button_x && edui_cursor_y >= property->button_y * 0.5f &&
            edui_cursor_x < property->button_x + property->button_size &&
            edui_cursor_y < (property->button_y + property->button_size) * 0.5f && !eduiInteractLocked)
            highlighted = 1;
        else
            highlighted = (*flags >> 3) & 1;
        *flags = (*flags & ~16u) | (highlighted << 4);
        if (__builtin_expect(!edui_donotdraw, 1))
            NuQFntSet(edui_font);
        if (__builtin_expect(item->unknown_10 == 2, 0)) {
            if (!edui_donotdraw)
                NuQFntSetColour(edui_font, 0xff006000);
        } else if (__builtin_expect(item->unknown_10 == 3, 0)) {
            if (!edui_donotdraw)
                NuQFntSetColour(edui_font, 0xff600000);
        } else if (item->unknown_10 == 1) {
            if (__builtin_expect(!edui_donotdraw, 0))
                NuQFntSetColour(edui_font, 0xff000060);
        } else if (!edui_donotdraw) {
            NuQFntSetColour(edui_font, item->colours[item->highlighted]);
        }
        f32 height = NuQFntHeight(edui_font) * 1.25f * 0.125f;
        f32 baseline = (NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font)) * 0.125f;
        f32 indent_width = height - 2.0f;
        f32 indentation = property->depth * indent_width;
        if (property->unknown_property_flags & 1) {
            f32 text_width = NuQFntPrintLenU(edui_font, eduiPropTextEdit) * 0.0625f;
            f32 edit_width = draw_width - property->label_width - 4.0f;
            edit_width = MAX(edit_width, text_width);
            if (!edui_donotdraw)
                NuRndrRect2di(static_cast<i32>(draw_x * 16.0f), static_cast<i32>(draw_y * 8.0f),
                              static_cast<i32>(draw_width * 16.0f), static_cast<i32>(height * 8.0f),
                              item->colours[2 + item->highlighted], uimtls[0]);
            if (!edui_donotdraw)
                NuRndrRect2di(static_cast<i32>((draw_x + property->label_width + 2.0f) * 16.0f),
                              static_cast<i32>((draw_y + 2.0f) * 8.0f), static_cast<i32>(edit_width * 16.0f),
                              static_cast<i32>((height - 4.0f) * 8.0f), 0xffffffff, uimtls[0]);
            for (i32 depth = 0; depth < property->depth; ++depth) {
                f32 guide_x = draw_x + (depth + 0.5f) * indent_width;
                if (__builtin_expect(!edui_donotdraw, 1))
                    NuRndrLine2di(static_cast<i32>(guide_x * 16.0f), static_cast<i32>(draw_y * 8.0f),
                                  static_cast<i32>(guide_x * 16.0f), static_cast<i32>((draw_y + height) * 8.0f),
                                  0xff000000, uimtls[0]);
            }
            u32 divider_colour = divider_colours[(property->unknown_property_flags >> 2) & 1];
            if (!edui_donotdraw)
                NuRndrLine2di(static_cast<i32>((draw_x + property->label_width) * 16.0f),
                              static_cast<i32>(draw_y * 8.0f),
                              static_cast<i32>((draw_x + property->label_width) * 16.0f),
                              static_cast<i32>((draw_y + height) * 8.0f), divider_colour, uimtls[0]);
            eduiFntPrintClipEx(edui_font, draw_x + indentation, draw_y + baseline, 16, draw_x - indentation,
                               property->label_width, item->text);
            eduiFntPrintExEditBuffer(edui_font, draw_x + property->label_width + 2.0f, draw_y + baseline, 16, eduiPropTextEdit);

            NuStrNCpy(prefix, eduiPropTextEdit, eduiPropTextPos + 1);
            prefix[eduiPropTextPos + 1] = '\0';
            f32 caret_width = NuQFntPrintLenU(edui_font, prefix) * 0.0625f;
            f32 caret_x = draw_x + property->label_width + 2.0f + caret_width;
            if (!edui_donotdraw)
                NuRndrLine2di(static_cast<i32>(caret_x * 16.0f), static_cast<i32>(draw_y * 8.0f),
                              static_cast<i32>(caret_x * 16.0f), static_cast<i32>((draw_y + height) * 8.0f), 0xff000000,
                              uimtls[0]);
        } else {
            property->button_size = indent_width;
            property->button_y = draw_y + 1.0f;
            property->button_x = draw_x + draw_width - (1.0f + indent_width);
            for (eduimenu_s *descendant = menu; descendant; descendant = descendant->child) {
                if (descendant == eduiGetActiveMenu()) {
                    if (!edui_donotdraw)
                        NuRndrRect2di(static_cast<i32>(draw_x * 16.0f), static_cast<i32>(draw_y * 8.0f),
                                      static_cast<i32>(draw_width * 16.0f), static_cast<i32>(height * 8.0f),
                                      item->colours[2 + item->highlighted], uimtls[ui_bgmtl]);
                    break;
                }
            }
            u32 divider_colour = divider_colours[(property->unknown_property_flags >> 2) & 1];
            if (__builtin_expect(!edui_donotdraw, 1))
                NuRndrLine2di(static_cast<i32>((draw_x + property->label_width) * 16.0f),
                              static_cast<i32>(draw_y * 8.0f),
                              static_cast<i32>((draw_x + property->label_width) * 16.0f),
                              static_cast<i32>((draw_y + height) * 8.0f), divider_colour, uimtls[0]);
            for (i32 depth = 0; depth < property->depth; ++depth) {
                f32 guide_x = draw_x + (depth + 0.5f) * indent_width;
                if (!edui_donotdraw)
                    NuRndrLine2di(static_cast<i32>(guide_x * 16.0f), static_cast<i32>(draw_y * 8.0f),
                                  static_cast<i32>(guide_x * 16.0f), static_cast<i32>((draw_y + height) * 8.0f),
                                  0xff000000, uimtls[0]);
            }
            eduiFntPrintClipEx(edui_font, draw_x + indentation, draw_y + baseline, 16, draw_x - indentation,
                               property->label_width, item->text);
            f32 value_x = property->button_x - 1.0f;
            f32 clip_x = draw_x + property->label_width;
            eduiFntPrintClipEx(edui_font, value_x, draw_y + baseline, 32, clip_x, value_x - clip_x,
                               property->property_text);
            if (property->button_type == 3) {
                u32 fill_colour;
                memcpy(&fill_colour, property->unknown_4e + 2, sizeof(fill_colour));
                if (!edui_donotdraw)
                    NuRndrRect2di(static_cast<i32>(property->button_x * 16.0f),
                                  static_cast<i32>(property->button_y * 8.0f),
                                  static_cast<i32>(property->button_size * 16.0f),
                                  static_cast<i32>(property->button_size * 8.0f), fill_colour, uimtls[0]);
            }
            eduiDrawLineRect(static_cast<i32>(property->button_x * 16.0f), static_cast<i32>(property->button_y * 8.0f),
                             static_cast<i32>(property->button_size * 16.0f),
                             static_cast<i32>(property->button_size * 8.0f), 0xff000000, uimtls[ui_outmtl]);
            i32 points[6];
            if (__builtin_expect(property->button_type == 1, 0)) {
                points[0] = static_cast<i32>((property->button_x + property->button_size * 0.1f) * 16.0f);
                points[1] = static_cast<i32>((property->button_y + property->button_size * 0.1f) * 8.0f);
                points[2] = static_cast<i32>((property->button_x + property->button_size * 0.9f) * 16.0f);
                points[3] = static_cast<i32>((property->button_y + property->button_size * 0.5f) * 8.0f);
                points[4] = static_cast<i32>((property->button_x + property->button_size * 0.1f) * 16.0f);
                points[5] = static_cast<i32>((property->button_y + property->button_size * 0.9f) * 8.0f);
                u32 colour = button_colours[(property->unknown_property_flags >> 4) & 1];
                if (!edui_donotdraw)
                    NuRndrTriStrip2di(points, NULL, 3, colour, uimtls[0]);
            }
            if (property->button_type == 2) {
                points[0] = static_cast<i32>((property->button_x + property->button_size * 0.1f) * 16.0f);
                points[1] = static_cast<i32>((property->button_y + property->button_size * 0.45f) * 8.0f);
                points[2] = static_cast<i32>((property->button_x + property->button_size * 0.5f) * 16.0f);
                points[3] = static_cast<i32>((property->button_y + property->button_size * 0.1f) * 8.0f);
                points[4] = static_cast<i32>((property->button_x + property->button_size * 0.9f) * 16.0f);
                points[5] = static_cast<i32>((property->button_y + property->button_size * 0.45f) * 8.0f);
                u32 colour = button_colours[(property->unknown_property_flags >> 4) & 1];
                if (__builtin_expect(!edui_donotdraw, 0))
                    NuRndrTriStrip2di(points, NULL, 3, colour, uimtls[0]);
                points[0] = static_cast<i32>((property->button_x + property->button_size * 0.1f) * 16.0f);
                points[1] = static_cast<i32>((property->button_y + property->button_size * 0.55f) * 8.0f);
                points[2] = static_cast<i32>((property->button_x + property->button_size * 0.5f) * 16.0f);
                points[3] = static_cast<i32>((property->button_y + property->button_size * 0.9f) * 8.0f);
                points[4] = static_cast<i32>((property->button_x + property->button_size * 0.9f) * 16.0f);
                points[5] = static_cast<i32>((property->button_y + property->button_size * 0.55f) * 8.0f);
                colour = button_colours[(property->unknown_property_flags >> 4) & 1];
                if (!edui_donotdraw)
                    NuRndrTriStrip2di(points, NULL, 3, colour, uimtls[0]);
            }
        }
        return static_cast<i32>(height);
    }

    static __used__ i32 eduicbRenderFilter(struct eduimenu_s *menu, struct eduiitem_s *item, i32 x, i32 y, i32 scale,
                                           i32 selected) {
        return eduicbRenderProp(menu, item, x, y, scale, selected);
    }

    i32 eduiMenuDetach(eduimenu_s *menu) {
        if (menu->parent) {
            if (menu->child)
                eduiMenuDetach(menu->child);
            if (eduiGetActiveMenu() == menu)
                active_menu = menu->parent;
            menu->parent->child = NULL;
            menu->parent = NULL;
            return 1;
        }
        eduiSetActiveMenu(NULL);
        return 0;
    }

}

static __used__ void cbColourPickSel(eduimenu_s *menu, eduiitem_s *, u32) {
    *cp_r = cp_item->red;
    *cp_g = cp_item->green;
    *cp_b = cp_item->blue;
    if (cp_rgba != nullptr)
        *cp_rgba = cpPackColour(*cp_r, *cp_g, *cp_b, *cp_a);
    eduiMenuDetach(menu);
}

extern "C" {
    void cbCancelSubMenu(eduimenu_s *menu, eduimenu_s *) {
        eduiMenuDetach(menu);
    }

    static __used__ void cbgpcfgPaste(eduimenu_s *menu, eduiitem_s *item, u32 value) {
        edui_gradient_pick_s *picker = reinterpret_cast<edui_gradient_pick_s *>(item->data_ptr);
        edui_gradient_node_s *stage = picker->selected_stage;
        if (stage != nullptr) {
            if (item->type == 7)
                eduiGradStageSetHSV(stage, cliph, clips, clipv);
            if (item->type == 8)
                eduiGradStageSetHSV(stage, 0.0f, 0.0f, clipg);
            if (picker->paste != nullptr)
                picker->paste(menu, item, value);
        }
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    static __used__ void cbgpcfgCopy(eduimenu_s *menu, eduiitem_s *item, u32 value) {
        edui_gradient_pick_s *picker = reinterpret_cast<edui_gradient_pick_s *>(item->data_ptr);
        edui_gradient_node_s *stage = picker->selected_stage;
        if (stage != nullptr) {
            if (item->type == 7) {
                cliph = stage->hue;
                clips = stage->saturation;
                clipv = stage->value;
            } else if (item->type == 8) {
                clipg = stage->value;
            }
            if (picker->copy != nullptr)
                picker->copy(menu, item, value);
        }
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    static __used__ void cbgpcfgDel(eduimenu_s *menu, eduiitem_s *item, u32 value) {
        edui_gradient_pick_s *picker = reinterpret_cast<edui_gradient_pick_s *>(item->data_ptr);
        if (picker->remove != nullptr)
            picker->remove(menu, item, value);
        if (picker->selected_stage != nullptr)
            eduiGradStageDelete(picker, picker->selected_stage);
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    static __used__ void cbgpcfgAdd(eduimenu_s *menu, eduiitem_s *item, u32 value) {
        edui_gradient_pick_s *picker = reinterpret_cast<edui_gradient_pick_s *>(item->data_ptr);
        edui_gradient_node_s *stage = picker->selected_stage;
        f32 time = 0.0f;
        f32 hue = 0.0f;
        f32 saturation = 0.0f;
        f32 intensity = 1.0f;
        if (stage != nullptr) {
            time = ((stage->next != nullptr ? stage->next->time : 1.0f) + stage->time) * 0.5f;
            hue = stage->hue;
            saturation = stage->saturation;
            intensity = stage->value;
        }
        picker->selected_stage = eduiGradStageAdd(picker, time, hue, saturation, intensity);
        if (picker->add != nullptr)
            picker->add(menu, item, value);
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    static __used__ void cbgpcfgCPPress(eduimenu_s *menu, eduiitem_s *item, u32 value) {
        edui_gradient_pick_s *picker = reinterpret_cast<edui_gradient_pick_s *>(item->data_ptr);
        if (picker->selected_stage != nullptr && (item->type == 10 || item->type == 11)) {
            edui_colour_pick_s *colour = reinterpret_cast<edui_colour_pick_s *>(item);
            eduiGradStageSetHSV(picker->selected_stage, colour->hue, colour->saturation, colour->value);
        }
        if (picker->press != nullptr)
            picker->press(menu, item, value);
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    static __used__ void eduicbSelectDirectoryExit(eduimenu_s *menu, eduiitem_s *, u32) {
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    static __used__ void eduicbSelectDirectoryEntry(eduimenu_s *menu, eduiitem_s *item, u32 value) {
        if (!dir_list)
            return;
        if (item->data == 9999) {
            char *separator = NuStrRChr(filepick_item->filename, '/');
            if (separator)
                *separator = '\0';
            else
                filepick_item->filename[0] = '\0';
            filepick_item->reopen_directory = 1;
        }
        if (static_cast<FilePickDirectoryEntry *>(dir_list)[item->data].flags & 8) {
            if (filepick_item->filename[0])
                NuStrCat(filepick_item->filename, const_cast<char *>("/"));
            NuStrCat(filepick_item->filename, static_cast<FilePickDirectoryEntry *>(dir_list)[item->data].name);
            filepick_item->reopen_directory = 1;
        }
        if (!(static_cast<FilePickDirectoryEntry *>(dir_list)[item->data].flags & 8)) {
            NuStrCpy(filepick_item->name, static_cast<FilePickDirectoryEntry *>(dir_list)[item->data].name);
            if (filepick_item->changed)
                filepick_item->changed(filepick_menu, filepick_item, value);
        }
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    static __used__ void eduicbDestroyDirectoryList(eduimenu_s *menu, eduimenu_s *) {
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    __attribute__((visibility("hidden"), optimize("O3"))) void tpentPropOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                                unsigned int buttons) {
        void *original = *(void **)((char *)item + 0x0c);
        void (*callback)(void *, void *, unsigned int) =
            *(void (**)(void *, void *, unsigned int))((char *)original + 0x78);
        eduimenu_s *parent = *(eduimenu_s **)((char *)menu + 0x40);

        NuStrCpy(*(char **)((char *)original + 0x64), (char *)item + 0x4c);
        if (callback)
            callback(parent, original, buttons);
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
        if (parent)
            eduiSetActiveMenu(parent);
    }

    __attribute__((visibility("hidden"), optimize("O3"))) void tpentOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                            unsigned int buttons) {
        void *original = *(void **)((char *)item + 0x0c);
        void (*callback)(void *, void *, unsigned int) =
            *(void (**)(void *, void *, unsigned int))((char *)original + 0x15c);
        void *parent = *(void **)((char *)menu + 0x40);

        NuStrCpy((char *)original + 0x4c, (char *)item + 0x4c);
        if (callback)
            callback(parent, original, buttons);
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    static __used__ void cbmcfgCancel(eduimenu_s *menu, eduimenu_s *) {
        eduiMenuDetach(menu);
    }

    static __used__ i32 eduicbRenderExpander(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32) {
        edui_expander_s *expander = static_cast<edui_expander_s *>(item);
        u32 button_colours[2] = {0xff000000, 0xffffffff};
        const f32 item_x = static_cast<f32>(x);
        const f32 item_y = static_cast<f32>(y);
        const f32 item_width = static_cast<f32>(width);
        item->x = x;
        item->y = y;
        const bool over_button = edui_cursor_x >= expander->button_x && edui_cursor_y >= expander->button_y * 0.5f &&
                                 edui_cursor_x < expander->button_x + expander->button_size &&
                                 edui_cursor_y < (expander->button_y + expander->button_size) * 0.5f;
        // The original updates only the byte containing open and hover at offset 0x54.
        u8 *hover_flags = reinterpret_cast<u8 *>(expander) + 0x54;
        *hover_flags = (*hover_flags & ~2u) | static_cast<u8>(over_button) << 1;

        {
            eduiDrawFontSet(edui_font);
            eduiSetFontColour(edui_font, item->colours[item->highlighted]);
        }
        const f32 row_height = NuQFntHeight(edui_font) * 1.25f * 0.125f;
        const f32 baseline = (NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font)) * 0.125f;
        for (eduimenu_s *submenu = menu; submenu; submenu = submenu->child) {
            if (submenu == eduiGetActiveMenu()) {
                eduiDrawRect(static_cast<i32>(item_x * 16.0f), static_cast<i32>(item_y * 8.0f),
                             static_cast<i32>(item_width * 16.0f), static_cast<i32>(row_height * 8.0f),
                             item->colours[2 + item->highlighted], uimtls[ui_bgmtl]);
                break;
            }
        }
        const f32 button_size = row_height - 2.0f;
        for (i32 depth = 0; depth < expander->depth; ++depth) {
            if (__builtin_expect(!edui_donotdraw, 1)) {
                const i32 guide_x = static_cast<i32>((item_x + (static_cast<f32>(depth) + 0.5f) * button_size) * 16.0f);
                NuRndrLine2di(guide_x, static_cast<i32>(item_y * 8.0f), guide_x,
                              static_cast<i32>((item_y + row_height) * 8.0f), 0xff000000, uimtls[0]);
            }
        }
        expander->button_size = button_size;
        expander->button_x = item_x + static_cast<f32>(expander->depth) * button_size + 1.0f;
        expander->button_y = item_y + 1.0f;
        if (expander->first_child) {
            eduiDrawLineRect(static_cast<i32>(expander->button_x * 16.0f), static_cast<i32>(expander->button_y * 8.0f),
                             static_cast<i32>(expander->button_size * 16.0f),
                             static_cast<i32>(expander->button_size * 8.0f), 0xff000000, uimtls[ui_outmtl]);
            const u8 button_flags = *hover_flags;
            eduiDrawLine(static_cast<i32>((expander->button_x + 2.0f) * 16.0f),
                         static_cast<i32>((expander->button_y + expander->button_size * 0.5f) * 8.0f),
                         static_cast<i32>((expander->button_x + expander->button_size - 2.0f) * 16.0f),
                         static_cast<i32>((expander->button_y + expander->button_size * 0.5f) * 8.0f),
                         button_colours[(button_flags >> 1) & 1], uimtls[0]);
            if ((button_flags & 1) == 0 && !edui_donotdraw)
                NuRndrLine2di(static_cast<i32>((expander->button_x + expander->button_size * 0.5f) * 16.0f),
                              static_cast<i32>((expander->button_y + 2.0f) * 8.0f),
                              static_cast<i32>((expander->button_x + expander->button_size * 0.5f) * 16.0f),
                              static_cast<i32>((expander->button_y + expander->button_size - 2.0f) * 8.0f),
                              button_colours[(button_flags >> 1) & 1], uimtls[0]);
        }
        const f32 text_x = expander->button_x + expander->button_size + 2.0f;
        eduiFntPrintClipEx(edui_font, text_x, item_y + baseline, 16, text_x,
                           item_width - (expander->button_size - 2.0f), item->text);
        return static_cast<i32>(row_height);
    }

    eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu) {
        if (menu == NULL) {
            return NULL;
        }
        if (menu->parent == NULL) {
            return menu;
        }
        do {
            menu = menu->parent;
        } while (menu->parent != NULL);
        return menu;
    }

    eduimenu_s *eduiGetActiveMenuParent(void) {
        return eduiGetTopLevelParent(active_menu);
    }

    i32 eduiClearActiveMenu(void) {
        eduiSetActiveMenu(NULL);
        return 0;
    }

    i32 eduiMenuIsActive(eduimenu_s *menu) {
        if (__builtin_expect(!eduiGetUsingMenuFocus(), 1))
            return 1;
        if (eduiGetUsingMenuFocus() && active_menu == menu)
            return 1;
        return 0;
    }

    i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad) {
        if (menu && menu->selected && !(menu->selected->flags & EDUI_ITEM_DISABLED) && menu->selected->process)
            return menu->selected->process(menu, menu->selected, delta_time, pad);
        return 0;
    }

    void eduiMenuEnsureSelection(eduimenu_s *menu) {
        if (!menu->selected)
            menu->selected = menu->first;
    }

    i32 eduiCursorOverMenu(eduimenu_s *menu) {
        return edui_cursor_x >= menu->x && edui_cursor_y >= menu->y && edui_cursor_x < menu->x + menu->width &&
               edui_cursor_y < menu->y + menu->height;
    }

    i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad) {
        if (__builtin_expect(!pad || !(pad->digital_buttons_pressed & 0x10), 1))
            return 0;

        eduimenu_s *parent = menu->parent;
        if (!eduiGetUsingMenuFocus() && parent)
            eduiMenuDetach(menu);
        if (menu->callback)
            menu->callback(menu, parent);
        return 1;
    }

    void cbInteractMenuScrollTo(eduimenu_s *menu, char *text) {
        if (!text || !menu || !text[0])
            return;
        i32 comparison = menu->first && menu->first->text ? NuStrICmp(menu->first->text, text) : -1;
        menu->field_0c = menu->first;
        menu->selected = menu->first;
        while (menu->selected->next && comparison < 0) {
            menu->selected = menu->selected->next;
            if (menu->selected->text) {
                comparison = NuStrICmp(menu->selected->text, text);
            } else {
                comparison = -1;
            }
        }
        menu->field_10 = menu->selected;
    }

    void cbInteractMenuKeySelect(eduimenu_s *menu) {
        u32 modifiers;
        char text[16];
        i32 key = NuKeyGet(&modifiers);
        if (key >= 0) {
            text[0] = NuKeyToAscii(key, modifiers & 1);
            text[1] = 0;
            cbInteractMenuScrollTo(menu, text);
        }
    }

    i32 eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result) {
        static f32 up_rept;
        static f32 down_rept;
        cbInteractMenuKeySelect(menu);
        if (item_result || !pad)
            return 0;
        if (menu && menu->selected && menu->selected->input && !menu->selected->disabled && pad->digital_buttons) {
            if (menu->selected->input(menu, menu->selected, pad->digital_buttons, pad->digital_buttons_pressed))
                return 0;
        }
        u32 buttons = pad->digital_buttons;
        u32 pressed = pad->digital_buttons_pressed;
        u32 up_button = (menu->flags & 2) ? 0x8000 : 0x1000;
        u32 down_button = (menu->flags & 2) ? 0x2000 : 0x4000;
        i32 up = 0;
        if (pressed & up_button) {
            up_rept = 1.0f / 3.0f;
            up = 1;
        }
        if (__builtin_expect((buttons & up_button) && up_rept > 0.0f, 1)) {
            up_rept -= delta_time;
            if (up_rept <= 0.0f) {
                up_rept = 1.0f / 6.0f;
                up = 1;
            }
        }
        if (!(menu->flags & 6)) {
            if ((buttons & 0x8000) && !(menu->flags & 1)) {
                i32 distance = static_cast<i32>(delta_time * 1500.0f);
                if (distance < 1)
                    distance = 1;
                menu->x -= distance;
            }
            if ((buttons & 0x2000) && !(menu->flags & 1)) {
                i32 distance = static_cast<i32>(delta_time * 1500.0f);
                if (distance < 1)
                    distance = 1;
                menu->x += distance;
            }
        }
        i32 down = 0;
        if (pressed & down_button) {
            down_rept = 1.0f / 3.0f;
            down = 1;
        }
        if ((buttons & down_button) && down_rept > 0.0f) {
            down_rept -= delta_time;
            if (down_rept <= 0.0f) {
                down_rept = 1.0f / 6.0f;
                down = 1;
            }
        }
        i32 to_first = up && (buttons & 1);
        i32 page_up = up && (buttons & 2);
        i32 page_down = down && (buttons & 2);
        i32 to_last = down && (buttons & 1);
        if (page_down && menu->field_10) {
            i32 count = 1;
            for (eduiitem_s *item = menu->field_0c; item && item != menu->field_10; item = item->next)
                ++count;
            while (menu->selected->next && count--) {
                menu->selected = menu->selected->next;
            }
            return 0;
        }
        if (page_up && menu->field_10) {
            i32 count = 1;
            for (eduiitem_s *item = menu->field_0c; item && item != menu->field_10; item = item->next)
                ++count;
            menu->selected = menu->field_0c;
            while (menu->selected->previous && count--) {
                menu->selected = menu->selected->previous;
                menu->field_0c = menu->selected;
            }
            return 0;
        }
        if (up && menu->selected && menu->selected->previous) {
            do {
                if (menu->selected == menu->field_0c)
                    menu->field_0c = menu->selected->previous;
                eduiitem_s *item = menu->selected->previous;
                while (item->type == 18)
                    item = item->previous;
                menu->selected = item;
            } while (menu->selected->previous && to_first);
            return 0;
        }
        if (down && menu->selected && menu->selected->next) {
            do {
                eduiitem_s *item = menu->selected->next;
                while (item->type == 18)
                    item = item->next;
                menu->selected = item;
            } while (menu->selected->next && to_last);
            if (__builtin_expect(!!(to_last), 0)) {
                i32 count = 1;
                for (eduiitem_s *item = menu->field_0c; item && item != menu->field_10; item = item->next)
                    ++count;
                menu->field_0c = menu->selected;
                while (menu->field_0c->previous && count--)
                    menu->field_0c = menu->field_0c->previous;
            }
            return 0;
        }
        eduiCheckForPadMenuCancel(menu, pad);
        return 0;
    }

    void eduiFlushInteracts(void) {
        numInteracts = 0;
    }

    i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad) {
        i32 result = 0;
        if (__builtin_expect(!!(edmainGetCursorEnabled()), 0)) {
            if (eduiInteractLocked) {
                if (__builtin_expect((!eduiInteractLocked->menu || !eduiInteractLocked->item) && eduiInteractLocked->field_20, 0))
                    eduiInteractLocked = NULL;
            }
            if (eduiInteractLocked) {
                result = 1;
                if (__builtin_expect(menu == eduiInteractLocked->menu || menu->parent == eduiInteractLocked->menu, 0)) {
                    eduiInteractLocked = eduiInteractLocked->callback(eduiInteractLocked) ? eduiInteractLocked : NULL;
                    result = eduiInteractLocked != NULL;
                }
            } else {
                edui_interact_s *interact = eduiInteracts + numInteracts - 1;
                for (i32 i = 0; i < numInteracts; ++i, --interact) {
                    if (interact->menu == menu && (interact->buttons & edui_cursor_buttons_db) &&
                        edui_cursor_x >= interact->x && edui_cursor_y >= interact->y &&
                        edui_cursor_x < interact->x + interact->width &&
                        edui_cursor_y < interact->y + interact->height) {
                        if (menu && interact->item && interact->item->type != 0x12)
                            menu->selected = interact->item;
                        if (interact->field_20 || (interact->buttons & edui_cursor_buttons)) {
                            if (!interact->field_20 && eduiGetActiveMenu() != menu)
                                eduiSetActiveMenu(menu);
                            if (interact->callback && interact->callback(interact))
                                eduiInteractLocked = interact;
                        }
                        result = 1;
                        break;
                    }
                }
            }
        }
        return result;
    }

    i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad) {
        if (eduiMenuIsActive(menu)) {
            eduiMenuEnsureSelection(menu);
            i32 result = eduiMenuProcessSelectedItem(menu, delta_time, pad);
            if (eduiMenuIsActive(menu) && processing_menu == menu) {
                eduiMenuEnsureSelection(menu);
                if (eduiMenuIsActive(menu) && processing_menu == menu)
                    eduiMenuProcessInput(menu, delta_time, pad, result);
            }
        }
        return eduiProcessInteracts(menu, pad);
    }

    i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad) {
        processing_menu = menu;
        if (menu->child) {
            i32 result = eduiMenuProcess(menu->child, delta_time, pad);
            if (processing_menu) {
                if (result)
                    eduiSetActiveMenu(menu->child ? menu->child : menu);
                else
                    result = eduiProcessInteracts(menu, pad);
            }
            return result;
        }
        return eduiMenuProcessAux(menu, delta_time, pad);
    }

    void eduiRenderInteracts(void) {
        if (numInteracts < 1)
            return;

        ++NuPrimCSPos;
        NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_PS2);
        NuPrim2DBegin(2, 5, uimtls[0]);

        edui_interact_s *hovered = NULL;
        edui_interact_s *interact = eduiInteractLocked ? eduiInteractLocked : eduiInteracts;
        const i32 count = eduiInteractLocked ? 1 : numInteracts;
        for (i32 i = 0; i < count; ++i, ++interact) {
            const u32 colour = NuRandInt();
#define EDUI_INTERACT_VERTEX(x, y)                                                                                     \
    NuRndrPrimSetColour(colour);                                                                                       \
    NuPrim2DAddXYZ((x), (y), 0.0f)
            EDUI_INTERACT_VERTEX(interact->x, interact->y);
            EDUI_INTERACT_VERTEX(interact->x + interact->width, interact->y);
            EDUI_INTERACT_VERTEX(interact->x + interact->width, interact->y);
            EDUI_INTERACT_VERTEX(interact->x + interact->width, interact->y + interact->height);
            EDUI_INTERACT_VERTEX(interact->x + interact->width, interact->y + interact->height);
            EDUI_INTERACT_VERTEX(interact->x, interact->y + interact->height);
            EDUI_INTERACT_VERTEX(interact->x, interact->y + interact->height);
            EDUI_INTERACT_VERTEX(interact->x, interact->y);
#undef EDUI_INTERACT_VERTEX

            if (interact->x <= edui_cursor_x && interact->y <= edui_cursor_y &&
                edui_cursor_x < interact->x + interact->width && edui_cursor_y < interact->y + interact->height)
                hovered = interact;
        }
        NuPrim2DEnd();

        if (hovered) {
            NuPrim2DBegin(4, 5, uimtls[3]);
            const u32 colour = (NuRandInt() & 0x00ffffff) | 0x40000000;
            NuRndrPrimSetColour(colour);
            NuPrim2DAddXYZ(hovered->x, hovered->y, 0.0f);
            NuRndrPrimSetColour(colour);
            NuPrim2DAddXYZ(hovered->x + hovered->width, hovered->y + hovered->height, 0.0f);
            NuPrim2DEnd();
        }

        --NuPrimCSPos;
        NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[NuPrimCSPos]);
    }

    void eduiShowCursor(i32 show) {
        bShowCursor = show;
    }

    void eduiGetCursorCoords(f32 *x, f32 *y) {
        *x = edui_cursor_x / 640.0f;
        *y = edui_cursor_y / 224.0f;
    }

    void eduiSetCursorCoords(f32 x, f32 y) {
        edui_cursor_x = x * 640.0f;
        edui_cursor_y = y * 224.0f;
    }

    void eduiGetCursorDelta(f32 *x, f32 *y) {
        *x = edui_cursor_dx / 640.0f;
        *y = edui_cursor_dy / 224.0f;
    }

    i32 eduiUsedAlgPad(nupad_s *pad) {
        if (pad) {
            if (pad->analog_left_x < 64 || pad->analog_left_x > 192 || pad->analog_left_y < 64 ||
                pad->analog_left_y > 192)
                return EDUI_ANALOG_PAD_LEFT;
            if (pad->analog_right_x < 64 || pad->analog_right_x > 192 || pad->analog_right_y < 64 ||
                pad->analog_right_y > 192)
                return EDUI_ANALOG_PAD_RIGHT;
        }
        return EDUI_ANALOG_PAD_NONE;
    }

    void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad) {
        if (pad && eduiUsedAlgPad(pad) && !(pad->digital_buttons & EDUI_CURSOR_PRIMARY)) {
            eduiSetCursorCoords(0.5f, 0.5f);
        } else {
            edui_cursor_dx = -NuMouseReadXVel() * 0.1f;
            edui_cursor_dy = -NuMouseReadYVel() * 0.1f;
        }
        edui_cursor_locked = NuKeyboard(0x38) != 0;
        if (!edui_cursor_locked) {
            edui_cursor_x += edui_cursor_dx;
            edui_cursor_y += edui_cursor_dy;
        }
        if (edui_cursor_x < 0.0f)
            edui_cursor_x = 0.0f;
        if (edui_cursor_x > 640.0f)
            edui_cursor_x = 640.0f;
        if (edui_cursor_y < 0.0f)
            edui_cursor_y = 0.0f;
        if (edui_cursor_y > 224.0f)
            edui_cursor_y = 224.0f;
        edui_cursor_buttons_old = edui_cursor_buttons;
        edui_cursor_buttons = 0;
        edui_cursor_buttons |= NuMouseReadButtons() == 1 ? EDUI_CURSOR_PRIMARY : 0;
        edui_cursor_buttons |= NuMouseReadButtons() == 2 ? EDUI_CURSOR_SECONDARY : 0;
        edui_cursor_buttons_db = edui_cursor_buttons & ~edui_cursor_buttons_old;
    }

    void eduiProcessCursor(f32 delta_time, nupad_s *pad) {
        eduiProcessCursorDefault(delta_time, pad);
    }

    i32 edmainProcess(f32 delta_time, nupad_s *pad) {
        editor_return = 0;
        if (edmain_cursor_enabled)
            eduiProcessCursor(0.0f, NULL);

        if (ed_module_active && ed_curr) {
            if (!ed_curr->process) {
                ed_module_active = 0;
                return editor_return;
            }
            i32 result = ed_curr->process(delta_time, pad);
            if (!result)
                return editor_return;
            if (ed_curr->deactivate)
                ed_curr->deactivate();
            if (result == 2)
                editor_return = 1;
            ed_module_active = 0;
            return editor_return;
        }

        if (!ed_curr) {
            ed_curr = ed_list;
            eduiSetActiveMenu(ed_main_menu);
            eduiSetDefaultActiveMenu(ed_main_menu);
        }

        if (ed_main_menu) {
            for (eduiitem_s *item = ed_main_menu->first; item; item = item->next) {
                if (item->data_ptr) {
                    ed_module_s *module = static_cast<ed_module_s *>(item->data_ptr);
                    item->disabled = (reinterpret_cast<usize>(module->reserved) & 1) != 0;
                }
            }
            eduiMenuProcess(ed_main_menu, delta_time, pad);
        }
        return editor_return;
    }

    void eduiSetCursorColour(u32 colour) {
        edui_cursor_colour = colour;
    }

    void eduiRenderCursor(void) {
        if (edui_cursor_locked)
            return;
        NuPrim2DBegin(0, 7, uimtls[0]);
#define EDUI_CURSOR_VERTEX(xoff, yoff)                                                                                 \
    NuRndrPrimSetColour(edui_cursor_colour);                                                                           \
    NuRndrPrimUV(0.0f, 0.0f);                                                                                          \
    NuPrim2DAddXYZ(edui_cursor_x + (xoff), edui_cursor_y + (yoff), 0.0f)
        EDUI_CURSOR_VERTEX(0.0f, 0.0f);
        EDUI_CURSOR_VERTEX(8.0f, 2.0f);
        EDUI_CURSOR_VERTEX(4.0f, 4.0f);
        EDUI_CURSOR_VERTEX(0.0f, 0.0f);
        EDUI_CURSOR_VERTEX(12.0f, 5.4f);
        EDUI_CURSOR_VERTEX(10.8f, 6.0f);
#undef EDUI_CURSOR_VERTEX
        NuPrim2DEnd();
    }

    void edmainRender(void) {
        if (edmain_cursor_enabled)
            eduiFlushInteracts();

        if (ed_module_active && ed_curr) {
            if (ed_curr->render)
                ed_curr->render();
        } else if (ed_main_menu) {
            NuQFntPushPrintMode(2);
            NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
            NuQFntSet(system_qfont);
            NuQFntSetScale(system_qfont, edmain_menu_scale, edmain_menu_scale);
            eduiMenuRender(ed_main_menu);
            NuQFntPopCoordinateSystem();
            NuQFntPopPrintMode();
        }

        if (edmain_cursor_enabled) {
            eduiRenderCursor();
            if (edmain_render_interacts)
                eduiRenderInteracts();
        }
    }

    void eduiInit(void) {
        eduiInitMaterials();
        if (edui_filepick_listsize && !dir_list)
            dir_list = NU_ALLOC(static_cast<usize>(edui_filepick_listsize) * 0x118, 4, 1, "", 0);
    }

    void edmainInit(void *font, char *configuration_file) {
        if (ed_init)
            edmainClose();

        ed_module_active = 0;
        ed_fnt = font;
        for (ed_module_s *module = ed_list; module; module = module->next) {
            if (module->init)
                module->init();
        }

        edinternalcam = NuCameraCreate();
        edmainExtCamera(edinternalcam);
        eduiInit();

        NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
        NuQFntSetScale(system_qfont, edmain_menu_scale, edmain_menu_scale);

        ed_cfg_menu = eduiMenuCreate(220, 50, 180, 320, ed_fnt, cbmcfgCancel, const_cast<char *>("Editor Config"));
        if (ed_cfg_menu) {
            eduiMenuAddItem(ed_cfg_menu,
                            eduiItemSliderCreate(0, &ed_attr, 0, cbmcfgCameraSpeed, 1.0f, 20.0f,
                                                 edmain_default_camera_speed, const_cast<char *>("Camera Speed")));
            eduiMenuAddItem(ed_cfg_menu, eduiItemToggleCreate(0, &ed_attr, edmain_auto_speed, 1, cbSetAutoSpeed,
                                                              const_cast<char *>("Proportional Cursor Speed")));
            eduiMenuFitWidth(ed_cfg_menu, 8);
        }

        ed_main_menu = eduiMenuCreate(ed_main_menu_x, ed_main_menu_y, ed_main_menu_w, ed_main_menu_h, ed_fnt,
                                      cbMMCancel, const_cast<char *>("Editor Options"));
        if (ed_main_menu) {
            for (ed_module_s *module = ed_list; module; module = module->next) {
                eduiitem_s *item =
                    eduiMenuAddItem(ed_main_menu, eduiItemSelCreate(reinterpret_cast<usize>(module), &ed_attr, 0, 1,
                                                                    cbMMRegSel, const_cast<char *>(module->name)));
                if (module == ed_curr)
                    eduiMenuHighlight(ed_main_menu, item);
            }
            eduiMenuAddItem(ed_main_menu, eduiItemSelCreate(0, &ed_attr, 0, 0, cbMMEditorConfig,
                                                            const_cast<char *>("Editor Config")));
            eduiMenuAddItem(ed_main_menu,
                            eduiItemSelCreate(0, &ed_attr, 0, 0, cbMMReturnToApp, const_cast<char *>("Return To App")));
            eduiMenuFitWidth(ed_main_menu, 8);
        }

        NuQFntPopCoordinateSystem();

        if (configuration_file) {
            NuStrCpy(ed_levelfile, configuration_file);
            if (ed_levelfile[0]) {
                i32 file = NuFileOpen(ed_levelfile, NUFILE_READ);
                if (file) {
                    NuFileBeginBlkRead(file, 0x3032554e);
                    i32 block;
                    while ((block = NuFileBeginBlkRead(file, 0)) != 0) {
                        for (ed_module_s *module = ed_list; module; module = module->next) {
                            if (module->block_id == static_cast<u32>(block)) {
                                if (module->read)
                                    module->read(file);
                                break;
                            }
                        }
                        NuFileEndBlkRead(file);
                    }
                    NuFileEndBlkRead(file);
                    NuFileClose(file);
                }
            }
        } else {
            ed_levelfile[0] = '\0';
        }
        ed_init = 1;
    }

    void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height) {
        ed_attr = *colours;
        ed_main_menu_x = x;
        ed_main_menu_y = y;
        ed_main_menu_w = width;
        ed_main_menu_h = height;
        edmainInit(font, configuration_file);
    }

    void eduiSetFont(NUQFNT *font) {
        edui_font = font;
    }

    void eduiSetFontScale(f32 x, f32 y) {
        edui_font_scale_x = x;
        edui_font_scale_y = y;
    }

    void CreateTestMenu(void *font) {
        u32 colours[4] = {0x80000000u, 0x80ff0000u, 0x80808080u, 0x80404040u};
        eduiInitMaterials();

        submenu1 = eduiMenuCreate(90, 90, 180, 50, font, NULL, const_cast<char *>("SubMenu1"));
        if (submenu1 != NULL) {
            eduiMenuAddItem(submenu1, eduiItemSelCreate(0, colours, 0, 1, reinterpret_cast<EdUiItemCallback>(cbSel),
                                                        const_cast<char *>("Sub Sel 0")));
            eduiMenuAddItem(submenu1, eduiItemSelCreate(1, colours, 0, 1, reinterpret_cast<EdUiItemCallback>(cbSel),
                                                        const_cast<char *>("Sub Sel 1")));
            eduiMenuAddItem(submenu1, eduiItemSelCreate(2, colours, 0, 1, reinterpret_cast<EdUiItemCallback>(cbSel),
                                                        const_cast<char *>("Sub Sel 2")));
        }

        submenu0 = eduiMenuCreate(70, 70, 180, 50, font, NULL, const_cast<char *>("SubMenu0"));
        if (submenu0 != NULL) {
            eduiMenuAddItem(submenu0, eduiItemSelCreate(0, colours, 0, 1, reinterpret_cast<EdUiItemCallback>(cbSel),
                                                        const_cast<char *>("Sub Sel")));
            eduiMenuAddItem(submenu0, eduiItemSelCreate(reinterpret_cast<usize>(submenu1), colours, 0, 0,
                                                        reinterpret_cast<EdUiItemCallback>(cbSubMenu),
                                                        const_cast<char *>("Sub Menu 1..")));
            eduiMenuAddItem(submenu0, eduiItemSelCreate(2, colours, 0, 1, reinterpret_cast<EdUiItemCallback>(cbSel),
                                                        const_cast<char *>("Sub Sel")));
        }

        testmenu0 = eduiMenuCreate(50, 50, 180, 50, font, NULL, const_cast<char *>("Main Menu"));
        if (testmenu0 != NULL) {
            eduiMenuAddItem(testmenu0, eduiItemSelCreate(reinterpret_cast<usize>(submenu0), colours, 0, 0,
                                                         reinterpret_cast<EdUiItemCallback>(cbSubMenu),
                                                         const_cast<char *>("Sub Menu 0..")));

            edui_gradient_pick_s *gradient = static_cast<edui_gradient_pick_s *>(eduiItemGradPickCreate(
                1, colours, reinterpret_cast<EdUiItemCallback>(cbGradChange), const_cast<char *>("GradPick")));
            if (gradient != NULL) {
                eduiGradStageAdd(gradient, 0.0f, 0.0f, 0.0f, 0.0f);
                eduiGradStageAdd(gradient, 1.0f, 0.0f, 0.0f, 1.0f);
            }
            eduiMenuAddItem(testmenu0, gradient);

            edui_gradient_pick_s *grey_gradient = static_cast<edui_gradient_pick_s *>(eduiItemGreyGradPickCreate(
                1, colours, reinterpret_cast<EdUiItemCallback>(cbGradChange), const_cast<char *>("GreyGradPick")));
            if (grey_gradient != NULL) {
                eduiGradStageAdd(grey_gradient, 0.0f, 0.0f, 0.0f, 0.0f);
                eduiGradStageAdd(grey_gradient, 1.0f, 0.0f, 0.0f, 1.0f);
            }
            eduiMenuAddItem(testmenu0, grey_gradient);

            eduiMenuAddItem(testmenu0, eduiItemSelCreate(2, colours, 0, 1, reinterpret_cast<EdUiItemCallback>(cbSel),
                                                         const_cast<char *>("Sel")));
        }
    }

    void TestMenu(nupad_s *pad) {
        eduiMenuProcess(testmenu0, 0.016666668f, pad);
        eduiMenuRender(testmenu0);
    }

    eduiitem_s *eduiItemColourSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           f32 minimum, f32 range, f32 value, u8 red, u8 green, u8 blue, char *text) {
        edui_colour_slider_s *item =
            static_cast<edui_colour_slider_s *>(NU_ALLOC(sizeof(edui_colour_slider_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 13;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessColourSlider;
        item->selection_group = group;
        item->render = eduicbRenderColourSlider;
        item->text_alignment = 0x40;
        item->destroy = eduicbItemSliderDestroy;
        eduiItemSetText(item, text);
        item->granularity = 1.0f;
        item->format = NULL;
        item->changed = callback;
        item->minimum = minimum;
        item->range = range;
        item->red = red;
        item->green = green;
        item->blue = blue;
        eduiItemSliderSetFmt(item, (char *)"(%d)");
        eduiItemSliderSetVal(item, value);
        return item;
    }

    void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z) {
        gp_cam.position_speed = {position_x, position_y, position_z};
    }

    void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance) {
        gp_cam.position_speed = {position_x, position_y, position_z};
        gp_cam.distance_speed = distance;
    }

    static __used__ void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *item, u32) {
        f32 divisor = 512.0f / static_cast<edui_slider_s *>(item)->value;
        f32 speed = 0.2f / divisor;
        edcamSetSpeed(speed, speed, speed, 1.0f / divisor);
    }

    void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale) {
        gp_cam.auto_move_base = move_base;
        gp_cam.auto_move_dist_scale = move_distance_scale;
        gp_cam.auto_zoom_base = zoom_base;
        gp_cam.auto_zoom_dist_scale = zoom_distance_scale;
    }

    void cbSetAutoSpeed(eduimenu_s *, eduiitem_s *item, u32) {
        edmain_auto_speed = item->highlighted;
        if (edmain_auto_speed != 0) {
            edcamSetAutoSpeed(0.15f, 0.2f, 0.01f, 0.1f);
        } else {
            edcamSetAutoSpeed(0.0f, 0.0f, 0.0f, 0.0f);
        }
    }

    void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement) {
        gp_cam.mouse_pitch_speed = pitch;
        gp_cam.mouse_yaw_speed = yaw;
        gp_cam.mouse_move_speed = movement;
    }

    void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance) {
        gp_cam.allow_position_x = position_x;
        gp_cam.allow_position_y = position_y;
        gp_cam.allow_position_z = position_z;
        gp_cam.allow_pitch = pitch;
        gp_cam.allow_yaw = yaw;
        gp_cam.allow_distance = distance;
    }

}

void do_Pad_flymode_camera(edcam_s *camera, float delta_time, nupad_s *pad) {
    constexpr f32 kNominalFrameTime = 1.0f / 60.0f;
    constexpr f32 kPitchSpeedScale = 32.0f;
    constexpr f32 kYawSpeedScale = 64.0f;
    constexpr f32 kPositionSpeedScale = 0.01f;
    constexpr i32 kPadDeadZone = 32;
    constexpr i32 kPitchLimit = 0x4000;

    const f32 frame_scale = delta_time / kNominalFrameTime;
    const f32 move_speed = camera->auto_move_dist_scale == 0.0f
                               ? 1.0f
                               : camera->auto_move_base + NuFabs(camera->distance) * camera->auto_move_dist_scale;
    const f32 zoom_speed = camera->auto_zoom_dist_scale == 0.0f
                               ? 1.0f
                               : camera->auto_zoom_base + NuFabs(camera->distance) * camera->auto_zoom_dist_scale;

    NUVEC opposite_offset;
    opposite_offset.z = 0.0f;
    opposite_offset.y = 0.0f;
    opposite_offset.x = 0.0f;
    NUMTX rotation = numtx_identity;
    NuMtxRotateX(&rotation, camera->pitch);
    NuMtxRotateY(&rotation, camera->yaw);

    opposite_offset.z = -camera->distance;
    NuVecMtxRotate(&opposite_offset, &opposite_offset, &rotation);
    NUVEC old_opposite;
    NuVecAdd(&old_opposite, &camera->position, &opposite_offset);

    const i32 pad_yaw = NuPs2ApplyDeadZone(pad->analog_right_x, kPadDeadZone);
    const i32 yaw_speed = camera->pad_yaw_speed;
    const i32 pad_pitch = NuPs2ApplyDeadZone(pad->analog_right_y, kPadDeadZone);
    i32 pitch_delta = static_cast<i32>(static_cast<f32>(pad_pitch) * static_cast<f32>(camera->pad_pitch_speed) *
                                       delta_time * kPitchSpeedScale);
    if ((camera->freedoms & EDCAM_FREEDOM_INVERT_PAD_PITCH) != 0) {
        pitch_delta = -pitch_delta;
    }
    if ((camera->freedoms & EDCAM_FREEDOM_PITCH) != 0) {
        camera->pitch += pitch_delta;
        camera->pitch = MIN(kPitchLimit, camera->pitch);
        camera->pitch = MAX(-kPitchLimit, camera->pitch);
    }
    if ((camera->freedoms & EDCAM_FREEDOM_YAW) != 0) {
        camera->yaw +=
            static_cast<i32>(static_cast<f32>(pad_yaw) * static_cast<f32>(yaw_speed) * delta_time * kYawSpeedScale);
    }

    rotation = numtx_identity;
    NuMtxRotateX(&rotation, camera->pitch);
    NuMtxRotateY(&rotation, camera->yaw);
    opposite_offset.z = -camera->distance;
    opposite_offset.y = 0.0f;
    opposite_offset.x = 0.0f;
    NuVecMtxRotate(&opposite_offset, &opposite_offset, &rotation);
    NUVEC new_opposite;
    NuVecAdd(&new_opposite, &camera->position, &opposite_offset);
    NuVecAdd(&camera->position, &camera->position, &new_opposite);
    NuVecSub(&camera->position, &camera->position, &old_opposite);

    NUVEC movement = {0.0f, 0.0f, 0.0f};
    const i32 pad_move_x = NuPs2ApplyDeadZone(pad->analog_left_x, kPadDeadZone);
    movement.x -= static_cast<f32>(pad_move_x) * delta_time * camera->distance * kPositionSpeedScale;
    const i32 pad_move_z = NuPs2ApplyDeadZone(pad->analog_left_y, kPadDeadZone);
    movement.z += static_cast<f32>(pad_move_z) * delta_time * camera->distance * kPositionSpeedScale;

    movement.y += static_cast<f32>(pad->analog_l1) * camera->position_speed.y * move_speed * frame_scale * 0.5f;
    movement.y -= static_cast<f32>(pad->analog_l2) * camera->position_speed.y * move_speed * frame_scale * 0.5f;

    if (camera->allow_distance) {
        camera->distance += static_cast<f32>(pad->analog_r1) * camera->distance_speed * zoom_speed * frame_scale;
        camera->distance -= static_cast<f32>(pad->analog_r2) * camera->distance_speed * zoom_speed * frame_scale;
        camera->distance = MIN(-camera->minimum_distance, camera->distance);
    }

    NuVecMtxRotate(&movement, &movement, &rotation);
    if ((camera->freedoms & EDCAM_FREEDOM_POSITION_X) != 0) {
        camera->position.x += movement.x;
    }
    if ((camera->freedoms & EDCAM_FREEDOM_POSITION_Y) != 0) {
        camera->position.y += movement.y;
    }
    if ((camera->freedoms & EDCAM_FREEDOM_POSITION_Z) != 0) {
        camera->position.z += movement.z;
    }
}

extern "C" {
    void do_Pad_Standard_camera(edcam_s *camera, f32 delta_time, nupad_s *pad) {
        NUVEC movement = {0.0f, 0.0f, 0.0f};
        if (!pad->is_valid)
            return;

        const f32 frame_scale = delta_time / (1.0f / 60.0f);
        const f32 move_speed = camera->auto_move_dist_scale == 0.0f
                                   ? 1.0f
                                   : NuFabs(camera->distance) * camera->auto_move_dist_scale + camera->auto_move_base;
        const f32 zoom_speed = camera->auto_zoom_dist_scale == 0.0f
                                   ? 1.0f
                                   : NuFabs(camera->distance) * camera->auto_zoom_dist_scale + camera->auto_zoom_base;

        const u8 pad_yaw_axis = pad->analog_right_x;
        const u8 pad_pitch_axis = pad->analog_right_y;
        if (camera->freedoms & EDCAM_FREEDOM_DISTANCE) {
            f32 zoom_in = static_cast<f32>(pad->analog_r1) * camera->distance_speed * zoom_speed;
            if (zoom_in > 1.0f)
                zoom_in = 1.0f;
            camera->distance += zoom_in * frame_scale;
            f32 zoom_out = static_cast<f32>(pad->analog_r2) * camera->distance_speed * zoom_speed;
            if (zoom_out > 1.0f)
                zoom_out = 1.0f;
            camera->distance -= zoom_out * frame_scale;
            camera->distance = MIN(-camera->minimum_distance, camera->distance);
        }
        if (camera->freedoms & EDCAM_FREEDOM_PITCH) {
            const i32 pitch_input = NuPs2ApplyDeadZone(pad_pitch_axis, 32);
            camera->pitch -=
                static_cast<i32>(static_cast<f32>(pitch_input * camera->pad_pitch_speed) * delta_time * 64.0f);
            camera->pitch = MIN(0x4000, camera->pitch);
            camera->pitch = MAX(-0x4000, camera->pitch);
        }
        if (camera->freedoms & EDCAM_FREEDOM_YAW) {
            const i32 yaw_input = NuPs2ApplyDeadZone(pad_yaw_axis, 32);
            camera->yaw -= static_cast<i32>(static_cast<f32>(yaw_input * camera->pad_yaw_speed) * delta_time * 64.0f);
        }

        if (camera->freedoms & EDCAM_FREEDOM_POSITION_Y) {
            movement.y += static_cast<f32>(pad->analog_l1) * camera->position_speed.y * move_speed * frame_scale * 0.5f;
            movement.y -= static_cast<f32>(pad->analog_l2) * camera->position_speed.y * move_speed * frame_scale * 0.5f;
        }
        if (camera->freedoms & EDCAM_FREEDOM_POSITION_Z) {
            movement.z = -static_cast<f32>(NuPs2ApplyDeadZone(pad->analog_left_y, 32)) * camera->position_speed.y *
                         move_speed * frame_scale;
        }
        if (camera->freedoms & EDCAM_FREEDOM_POSITION_X) {
            movement.x = static_cast<f32>(NuPs2ApplyDeadZone(pad->analog_left_x, 32)) * camera->position_speed.y *
                         move_speed * frame_scale;
        }
        NuVecRotateY(&movement, &movement, camera->yaw);
        NuVecAdd(&camera->position, &camera->position, &movement);

        NUVEC snap_delta;
        NuVecSub(&snap_delta, &camera->position, &camera->snap_origin);
        snap_delta.x /= camera->snap_step.x;
        snap_delta.y /= camera->snap_step.y;
        snap_delta.z /= camera->snap_step.z;
        camera->snapped_position.x = NuFloor(snap_delta.x) * camera->snap_step.x + camera->snap_origin.x;
        camera->snapped_position.y = NuFloor(snap_delta.y) * camera->snap_step.y + camera->snap_origin.y;
        camera->snapped_position.z = NuFloor(snap_delta.z) * camera->snap_step.z + camera->snap_origin.z;
    }

    void do_maya_mouse_camera(edcam_s *camera) {
        static i16 TargetAng;
        static i16 MouseOldAng;

        f32 zoom_scale = camera->auto_zoom_dist_scale == 0.0f
                             ? 1.0f
                             : NuFabs(camera->distance) * camera->auto_zoom_dist_scale + camera->auto_zoom_base;
        NUVEC movement = {0.0f, 0.0f, 0.0f};
        if (NuKeyboard(0x38) != 0) {

            const f32 mouse_x = NuMouseReadXVel() * camera->mouse_pitch_speed;
            const f32 mouse_y = NuMouseReadYVel() * camera->mouse_yaw_speed;
            const f32 mouse_z = NuMouseReadZVel() * camera->mouse_move_speed;
            const u32 buttons = NuMouseReadButtons();

            if (buttons == 2 || buttons == 5) {
                if (camera->freedoms & EDCAM_FREEDOM_DISTANCE) {
                    if (mouse_y != 0.0f || mouse_x != 0.0f)
                        TargetAng = static_cast<i16>(NuAtan2D(-mouse_x, -mouse_y));
                    u16 angle = static_cast<u16>(TargetAng);
                    i32 difference = static_cast<u16>(angle - MouseOldAng);
                    if (difference > 0x7fff)
                        difference -= 0x10000;
                    if (abs(difference) > 20000)
                        angle = static_cast<u16>(TargetAng);
                    else
                        angle = static_cast<u16>(MouseOldAng + difference / 4);
                    MouseOldAng = angle;

                    f32 sine = NU_SIN_LUT(static_cast<u16>(angle) + 0x2000);
                    if (NuFabs(sine) > 0.1f) {
                        if (sine > 0.0f)
                            sine -= 0.1f;
                        else
                            sine += 0.1f;
                        f32 distance = NuFsqrt(mouse_x * mouse_x + mouse_y * mouse_y) * sine *
                                           (zoom_scale * camera->distance_speed) * camera->mouse_move_speed +
                                       camera->distance;
                        const f32 minimum = -camera->minimum_distance;
                        camera->distance = MIN(minimum, distance);
                    }
                }
            } else if (buttons == 3 || buttons == 4) {
                movement.x += mouse_x * 0.0078125f;
                movement.y -= mouse_y * 0.0078125f;
            } else if (buttons == 1) {
                if (camera->freedoms & EDCAM_FREEDOM_PITCH) {
                    camera->pitch -= static_cast<i32>(mouse_y * 16.0f);
                    if (camera->pitch > 0x4000)
                        camera->pitch = 0x4000;
                    if (camera->pitch < -0x4000)
                        camera->pitch = -0x4000;
                }
                if (camera->freedoms & EDCAM_FREEDOM_YAW)
                    camera->yaw -= static_cast<i32>(mouse_x * 16.0f);
            }

            const f32 distance = camera->distance - zoom_scale * camera->distance_speed * mouse_z;
            const f32 minimum = -camera->minimum_distance;
            camera->distance = MIN(minimum, distance);

            NUMTX rotation = numtx_identity;
            NuMtxRotateX(&rotation, camera->pitch);
            NuMtxRotateY(&rotation, camera->yaw);
            NuVecMtxRotate(&movement, &movement, &rotation);
            if (camera->freedoms & EDCAM_FREEDOM_POSITION_X)
                camera->position.x += movement.x;
            if (camera->freedoms & EDCAM_FREEDOM_POSITION_Y)
                camera->position.y += movement.y;
            if (camera->freedoms & EDCAM_FREEDOM_POSITION_Z)
                camera->position.z += movement.z;
        }
    }

    void do_mouse_flymode_camera(edcam_s *camera, f32 delta_time) {
        f32 zoom_scale = camera->auto_zoom_dist_scale == 0.0f
                             ? 1.0f
                             : NuFabs(camera->distance) * camera->auto_zoom_dist_scale + camera->auto_zoom_base;

        f32 mouse_y = NuMouseReadYRel();
        f32 mouse_x = NuMouseReadXRel();
        f32 mouse_z = NuMouseReadZRel();
        u32 buttons = NuMouseReadButtons();
        if (camera->allow_distance) {
            f32 distance = camera->distance - mouse_z * camera->distance_speed * zoom_scale;
            camera->distance = MIN(-camera->minimum_distance, distance);
        }

        NUVEC opposite_offset;
        opposite_offset.z = 0.0f;
        opposite_offset.y = 0.0f;
        opposite_offset.x = 0.0f;
        NUMTX rotation = numtx_identity;
        NuMtxRotateX(&rotation, camera->pitch);
        NuMtxRotateY(&rotation, camera->yaw);
        opposite_offset.z = -camera->distance;
        NuVecMtxRotate(&opposite_offset, &opposite_offset, &rotation);
        NUVEC opposite_positions[2];
        NUVEC &old_opposite = opposite_positions[0];
        NuVecAdd(&old_opposite, &camera->position, &opposite_offset);

        i32 pitch;
        if (!camera->allow_pitch)
            pitch = camera->pitch;
        else
            pitch = static_cast<i32>(mouse_y * 16.0f) + camera->pitch;
        pitch = MIN(0x4000, pitch);
        pitch = MAX(pitch, -0x4000);
        camera->pitch = pitch;
        if (camera->allow_yaw)
            camera->yaw += static_cast<i32>(mouse_x * 16.0f);

        rotation = numtx_identity;
        NuMtxRotateX(&rotation, camera->pitch);
        NuMtxRotateY(&rotation, camera->yaw);
        opposite_offset.z = -camera->distance;
        opposite_offset.y = 0.0f;
        opposite_offset.x = 0.0f;
        NuVecMtxRotate(&opposite_offset, &opposite_offset, &rotation);
        NUVEC &new_opposite = opposite_positions[1];
        NuVecAdd(&new_opposite, &camera->position, &opposite_offset);
        NuVecAdd(&camera->position, &camera->position, &new_opposite);
        NuVecSub(&camera->position, &camera->position, &old_opposite);

        NUVEC movement = {0.0f, 0.0f, 0.0f};
        if (buttons == 2)
            movement.z = delta_time * camera->distance;
        else if (buttons == 1)
            movement.z = -delta_time * camera->distance;
        NuVecMtxRotate(&movement, &movement, &rotation);
        if (camera->allow_position_x)
            camera->position.x += movement.x;
        if (camera->allow_position_y)
            camera->position.y += movement.y;
        if (camera->allow_position_z)
            camera->position.z += movement.z;
    }

    void edcamMoveEx(nupad_s *pad, f32 delta_time) {
        if (edmainGetCursorEnabled() != 0) {
            if (PadFlyMode == 0 || NuKeyboard(0x38) != 0) {
                do_maya_mouse_camera(&gp_cam);
            } else {
                do_mouse_flymode_camera(&gp_cam, delta_time);
            }
        }
        if (pad != NULL) {
            if (PadFlyMode == 0) {
                do_Pad_Standard_camera(&gp_cam, delta_time, pad);
            } else {
                do_Pad_flymode_camera(&gp_cam, delta_time, pad);
            }
        }
    }

    void edcamMove(nupad_s *pad) {
        edcamMoveEx(pad, NuTimeGetFrameTime());
    }

    void edcamSetPos(NUVEC *position) {
        gp_cam.position = *position;
        gp_cam.offset = {0.0f, 0.0f, 0.0f};
    }

    void edcamSetAng(i32 pitch, i32 yaw) {
        gp_cam.pitch = pitch;
        gp_cam.yaw = yaw;
    }

    void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw) {
        gp_cam.position = *position;
        gp_cam.offset = {0.0f, 0.0f, 0.0f};
        gp_cam.pitch = pitch;
        gp_cam.yaw = yaw;
    }

    void edcamGetPosAng(NUVEC *position, i32 *pitch, i32 *yaw) {
        if (position != NULL) {
            *position = gp_cam.position;
        }
        if (pitch != NULL) {
            *pitch = gp_cam.pitch;
        }
        if (yaw != NULL) {
            *yaw = gp_cam.yaw;
        }
    }

    void edcamGetPosAngSnap(NUVEC *position, i32 *pitch, i32 *yaw) {
        if (position != NULL) {
            *position = gp_cam.snapped_position;
        }
        if (pitch != NULL) {
            *pitch = gp_cam.snapped_pitch;
        }
        if (yaw != NULL) {
            *yaw = gp_cam.snapped_yaw;
        }
    }

}

void edcamSetContoller(i32 invert_pitch) {
    gp_cam.invert_pad_pitch = invert_pitch;
}

extern "C" {
    void edcamSetDist(f32 distance) {
        gp_cam.distance = distance;
    }

    f32 edcamGetDist(void) {
        return gp_cam.distance;
    }

    edcam_s *edcamGetEdCam(void) {
        return &gp_cam;
    }

    void edcamSetOffset(NUVEC *offset) {
        gp_cam.offset = *offset;
    }

    void edcamGetOffset(NUVEC *offset) {
        *offset = gp_cam.offset;
    }

    void edcamMtx(NUMTX *matrix) {
        NUVEC distance = {0.0f, 0.0f, gp_cam.distance};
        NuMtxSetTranslation(matrix, &distance);
        NuMtxRotateX(matrix, gp_cam.pitch);
        NuMtxRotateY(matrix, gp_cam.yaw);
        NuMtxTranslate(matrix, &gp_cam.position);
        NuMtxTranslate(matrix, &gp_cam.offset);
        if (ed_remap_mtx != NULL) {
            NuMtxMul(matrix, matrix, ed_remap_mtx);
            ed_remap_mtx = NULL;
        }
    }

    void edcamSet(void) {
        NUMTX matrix;
        edcamMtx(&matrix);
        edmainSetCamera(&matrix);
    }

    NUVEC *edcamGetPosPointer(void) {
        return &gp_cam.position;
    }

}

void EdFileSwapEndianess16(void *data) {
    char *bytes;
    char tmp;

    bytes = (char *)data;

    tmp = bytes[0];
    bytes[0] = bytes[1];
    bytes[1] = tmp;
}

void EdFileSwapEndianess32(void *data) {
    char *bytes;
    char tmp;

    bytes = (char *)data;

    tmp = bytes[0];
    bytes[0] = bytes[3];
    bytes[3] = tmp;

    tmp = bytes[1];
    bytes[1] = bytes[2];
    bytes[2] = tmp;
}

void EdFileSetReadWrongEndianess(i32 value) {
}

void EdFileSetMedia(i32 media) {
    edfile_media = media;
}

void EdFileSetPakFile(void *pak) {
    edfile_pakfile = pak;
}

void EdFileResetBuffers() {
    memset(edfile_buffer, 0, sizeof(edfile_buffer));
    edfile_buffer_pointer = 0;
    edfile_lock[1] = 0;
    edfile_lock[0] = 0;
}

void EdFileFillBuffer() {
    if (edfile_handle != -1) {
        memset(edfile_buffer, 0, sizeof(edfile_buffer));

        if (edfile_media == 1) {
            NuFileRead(edfile_handle, edfile_buffer, 0x1000);
            edfile_buffer_pointer = 0;
        }
    }
}

i32 EdFileOpen(char *filepath, NUFILEMODE mode) {
    i32 item_handle;
    void *item;
    i32 item_size;

    if (edfile_handle != -1) {
        return 0;
    }

    EdFileResetBuffers();

    edfile_handle = -1;
    edfile_write_flag = 0;

    if (edfile_media != 1) {
        return 0;
    }

    if (mode != NUFILE_READ) {
        if (mode != NUFILE_WRITE) {
            return 0;
        }

        edfile_write_flag = 1;
        edfile_handle = NuFileOpen(filepath, NUFILE_WRITE);

        if (edfile_handle > 0) {
            return 1;
        }

        edfile_handle = -1;

        return 0;
    }

    if (edfile_pakfile != NULL) {
        item_handle = NuFilePakGetItem(edfile_pakfile, filepath);

        if (item_handle != 0) {
            NuFilePakGetItemInfo(edfile_pakfile, item_handle, &item, &item_size);

            edfile_handle = NuMemFileOpen(item, item_size, NUFILE_READ);

            if (edfile_handle > 0) {
                EdFileFillBuffer();

                return 1;
            }

            edfile_handle = -1;
        }
    }

    edfile_handle = NuFileOpen(filepath, NUFILE_READ);

    if (edfile_handle > 0) {
        EdFileFillBuffer();

        return 1;
    }

    edfile_handle = -1;

    return 0;
}

void EdFileFlushBuffer() {
    if (edfile_handle == -1) {
        return;
    }

    if (edfile_buffer_pointer == 0 || edfile_media != 1) {
        return;
    }

    NuFileWrite(edfile_handle, edfile_buffer, edfile_buffer_pointer);
    edfile_buffer_pointer = 0;
}

i32 EdFileClose() {
    edfile_pakfile = NULL;

    if (edfile_handle == -1) {
        return 0;
    }

    if (__builtin_expect(edfile_write_flag != 0, 0)) {
        EdFileFlushBuffer();
    }

    if (edfile_media != 1) {
        return 0;
    }

    NuFileClose(edfile_handle);
    edfile_handle = -1;
    edfile_media = 0;

    return 1;
}

void EdFileWrite(void *data, i32 len) {
    i32 to_write;

    while (len > 0) {
        to_write = MIN(0x1000 - edfile_buffer_pointer, len);

        memcpy(edfile_buffer + edfile_buffer_pointer, data, to_write);

        edfile_buffer_pointer += to_write;
        len -= to_write;
        data = (void *)((usize)data + to_write);

        if (edfile_buffer_pointer == 0x1000) {
            EdFileFlushBuffer();
        }
    }
}

void EdFileWriteFloat(f32 value) {
    f32 data = value;
    if (edfile_readwrongendianess)
        EdFileSwapEndianess32(&data);
    EdFileWrite(&data, 4);
}

void EdFileWriteInt(i32 value) {
    i32 data = value;
    if (edfile_readwrongendianess)
        EdFileSwapEndianess32(&data);
    EdFileWrite(&data, 4);
}

void EdFileWriteUnsignedInt(u32 value) {
    u32 data = value;
    if (edfile_readwrongendianess)
        EdFileSwapEndianess32(&data);
    EdFileWrite(&data, 4);
}

void EdFileWriteShort(i16 value) {
    i16 data = value;
    if (edfile_readwrongendianess)
        EdFileSwapEndianess16(&data);
    EdFileWrite(&data, 2);
}

void EdFileWriteUnsignedShort(u16 value) {
    u16 data = value;
    if (edfile_readwrongendianess)
        EdFileSwapEndianess16(&data);
    EdFileWrite(&data, 2);
}

void EdFileWriteChar(char value) {
    EdFileWrite(&value, 1);
}

void EdFileWriteUnsignedChar(u8 value) {
    EdFileWrite(&value, 1);
}

void EdFileWriteNuVec(NUVEC *value) {
    EdFileWriteFloat(value->x);
    EdFileWriteFloat(value->y);
    EdFileWriteFloat(value->z);
}

void EdFileRead(void *buf, i32 len) {
    i32 to_read;

    while (len > 0) {
        to_read = MIN(0x1000 - edfile_buffer_pointer, len);
        memcpy(buf, edfile_buffer + edfile_buffer_pointer, to_read);

        edfile_buffer_pointer += to_read;
        len -= to_read;
        buf = (void *)((usize)buf + to_read);

        if (edfile_buffer_pointer == 0x1000) {
            EdFileFillBuffer();
        }
    }
}

f32 EdFileReadFloat() {
    f32 data;

    EdFileRead(&data, 4);

    if (edfile_readwrongendianess) {
        EdFileSwapEndianess32(&data);
    }

    return data;
}

i32 EdFileReadInt() {
    i32 data;

    EdFileRead(&data, 4);

    if (edfile_readwrongendianess) {
        EdFileSwapEndianess32(&data);
    }

    return data;
}

u32 EdFileReadUnsignedInt() {
    u32 data;

    EdFileRead(&data, 4);

    if (edfile_readwrongendianess) {
        EdFileSwapEndianess32(&data);
    }

    return data;
}

i16 EdFileReadShort() {
    i16 data;

    EdFileRead(&data, 2);

    if (edfile_readwrongendianess) {
        EdFileSwapEndianess16(&data);
    }

    return data;
}

u16 EdFileReadUnsignedShort() {
    u16 data;

    EdFileRead(&data, 2);

    if (edfile_readwrongendianess) {
        EdFileSwapEndianess16(&data);
    }

    return data;
}

char EdFileReadChar() {
    char data;

    EdFileRead(&data, 1);

    return data;
}

unsigned char EdFileReadUnsignedChar() {
    unsigned char data;

    EdFileRead(&data, 1);

    return data;
}

void EdFileReadNuVec(NUVEC *out) {
    out->x = EdFileReadFloat();
    out->y = EdFileReadFloat();
    out->z = EdFileReadFloat();
}

i32 EdFileWriteMemCard(char *filename, i32 size, void *data) {
    EdFileSetMedia(2);
    if (EdFileOpen(filename, static_cast<NUFILEMODE>(1))) {
        EdFileWrite(data, size);
        return EdFileClose();
    }
    return 0;
}

i32 EdFileReadMemCard(char *filename, i32 size, void *data) {
    EdFileSetMedia(2);
    if (EdFileOpen(filename, static_cast<NUFILEMODE>(0))) {
        EdFileRead(data, size);
        return EdFileClose();
    }
    return 0;
}

i32 EdFileBackup(char *source, char *destination) {
    i64 remaining = NuFileSize(source);
    if (remaining > 0) {
        NUFILE output = NuFileOpen(destination, NUFILE_WRITE);
        NUFILE input = NuFileOpen(source, NUFILE_READ);
        if (output > 0 && input > 0) {
            while (remaining > 0) {
                i32 size = MIN(static_cast<i32>(remaining), 0x1000);
                NuFileRead(input, edfile_buffer, size);
                NuFileWrite(output, edfile_buffer, size);
                remaining -= 0x1000;
            }
            NuFileClose(input);
            NuFileClose(output);
            return 1;
        }
        if (output > 0)
            NuFileClose(output);
        if (input > 0)
            NuFileClose(input);
    }
    return 0;
}

void FS_MakeDateTimeString(FS_FILEENTRYHDR *entry, char *output) {
    const u8 *date = reinterpret_cast<const u8 *>(entry);
    sprintf(output, " %.2d/%.2d/%d %.2d:%.2d:%.2d", date[4] - 'A', date[5] - 'A', date[6] + 1915, date[3] - 'A',
            date[2] - 'A', date[1] - 'A');
}

void FS_MakeTimeString(FS_FILEENTRYHDR *entry, char *output) {
    const u8 *date = reinterpret_cast<const u8 *>(entry);
    sprintf(output, " %.2d:%.2d:%.2d", date[3] - 'A', date[2] - 'A', date[1] - 'A');
}

void FS_MakeDateString(FS_FILEENTRYHDR *entry, char *output) {
    const u8 *date = reinterpret_cast<const u8 *>(entry);
    sprintf(output, " %.2d/%.2d/%d", date[4] - 'A', date[5] - 'A', date[6] + 1915);
}

static void RenderFileSel3Body(i32 draw_title) {
    char buffer[256];
    f32 x = FS_X;
    f32 y = FS_Y;
    const f32 width = FS_Width;
    NuQFntPushPrintMode(2);
    NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
    NuQFntSet(system_qfont);
    NuQFntSetPointSize(system_qfont, 1.0f, 1.0f);
    const f32 height = NuQFntHeight(system_qfont);
    const f32 baseline = NuQFntBaseline(system_qfont);
    f32 full_width = width;
    if (FS_NumFiles > 14) {
        full_width = width + 8.0f;
    }

    f32 text_x;
    if (draw_title) {
        const f32 box_width = full_width + 4.0f;
        const f32 box_height = height * 17.0f / 16.0f + 6.0f;
        NuRndrRect2d(x, y, 0.0f, box_width, box_height, 0xff808080, NULL);
        FS_W = box_width;
        FS_H = box_height;
        x += 2.0f;
        text_x = (x + 4.0f) * 16.0f;
        y += 1.0f;
        const f32 title_height = height / 16.0f;
        NuRndrRect2d(x, y, 0.0f, full_width, title_height, 0xff404040, NULL);
        NuRndrLine3dDbg(x, y, 0.0f, width + x, y, 0.0f, 0xff808080);
        const f32 title_y = y * 16.0f + baseline;
        NuQFntSet(system_qfont);
        NuQFntSetColour(system_qfont, 0x80808080);
        NuQFntMove(system_qfont, text_x, title_y, 0.0f);
        NuQFntPrintU(system_qfont, FS_Title);
        y += title_height + 2.0f;
    } else {
        const f32 box_width = full_width + 4.0f;
        const f32 box_height = height * 16.0f / 16.0f + 4.0f;
        NuRndrRect2d(x, y, 0.0f, box_width, box_height, 0xff808080, NULL);
        FS_W = box_width;
        FS_H = box_height;
        x += 2.0f;
        text_x = (x + 4.0f) * 16.0f;
        y += 1.0f;
    }

    const f32 line_height = height / 16.0f;
    const f32 path_y = y * 16.0f + baseline;
    NuRndrRect2d(x, y, 0.0f, full_width, line_height, 0x20202020, NULL);
    char *text;
    if (FS_ShowVolumes) {
        NuStrCpy(buffer, "Volumes:-");
        text = buffer;
    } else {
        text = FS_Path;
        const f32 available = full_width - 8.0f;
        f32 length = NuQFntPrintLenU(system_qfont, text) / 16.0f;
        if (length > available) {
            f32 scale = available / length;
            if (__builtin_expect(scale < 0.5f, 0)) {
                // Too long even at half width: keep the drive and first
                // directory, then "\....", then the last directory.
                char *first = NuStrChr(text, '\\');
                if (first != NULL) {
                    char *second = NuStrChr(first + 1, '\\');
                    if (second == NULL) {
                        second = first;
                    }
                    *second = '\0';
                    NuStrCpy(buffer, text);
                    *second = '\\';
                    NuStrCat(buffer, "\\....");
                    char *last;
                    const i32 tail_length = NuStrLen(second);
                    if (tail_length != 0 && second[tail_length - 1] == '\\') {
                        second[tail_length - 1] = '\0';
                        last = NuStrRChr(second, '\\');
                        second[tail_length - 1] = '\\';
                    } else {
                        last = NuStrRChr(second, '\\');
                    }
                    if (second < last) {
                        NuStrCat(buffer, last);
                        text = buffer;
                    }
                }
                length = NuQFntPrintLenU(system_qfont, text) / 16.0f;
                if (!(length > available)) {
                    goto path_done;
                }
                scale = available / length;
            }
            NuQFntSetPointSize(system_qfont, scale, 1.0f);
        }
    path_done:;
    }
    NuQFntSet(system_qfont);
    NuQFntMove(system_qfont, text_x, path_y, 0.0f);
    NuQFntSetColour(system_qfont, 0x80408080);
    NuQFntPrintU(system_qfont, text);
    NuQFntSetPointSize(system_qfont, 1.0f, 1.0f);

    y += line_height + 2.0f;
    const f32 list_height = height * 14.0f;
    const f32 list_box_height = list_height / 16.0f;
    NuRndrRect2d(x, y, 0.0f, width, list_box_height, 0x20202020, NULL);
    if (FS_NumFiles > 14) {
        NuRndrRect2d(width + x, y, 0.0f, 8.0f, list_box_height, 0xff646464, NULL);
        f32 thumb = list_box_height / FS_NumFiles * 14.0f;
        if (thumb < 5.0f) {
            thumb = 5.0f;
        }
        NuRndrRect2d(width + x, (list_box_height - thumb) * FS_CurrentPosFileNum / (FS_NumFiles - 14) + y, 0.0f, 8.0f,
                     thumb, 0xffc8c8c8, NULL);
    }

    f32 line_y = path_y + 32.0f;
    char *entry = FS_CurrentPos;
    for (i32 line = 0; line < 14; ++line) {
        if (entry >= FS_FileListEnd) {
            break;
        }
        line_y += height;
        if (entry == FS_CurrentCursorPos) {
            NuRndrRect2d(x, (line_y - baseline) / 16.0f, 0.0f, width, line_height, 0xff202060, NULL);
        }
        NuQFntSet(system_qfont);
        NuQFntMove(system_qfont, text_x, line_y, 0.0f);
        if (entry[0] == 'V') {
            NuQFntSetColour(system_qfont, 0x80204080);
        } else if (entry[0] == 'D') {
            NuQFntSetColour(system_qfont, 0x80804020);
        } else {
            NuQFntSetColour(system_qfont, 0x80408080);
        }
        NuQFntSetPointSize(system_qfont, 1.0f, 1.0f);
        char *name = entry + 7;
        NuQFntPrintU(system_qfont, name);
        if (entry[6] != '#') {
            NuQFntSetPointSize(system_qfont, 0.8f, 1.0f);
            FS_MakeTimeString(reinterpret_cast<FS_FILEENTRYHDR *>(entry), buffer);
            f32 length = NuQFntPrintLenU(system_qfont, buffer);
            NuQFntSetColour(system_qfont, 0x80208080);
            const f32 time_x = (width + x) * 16.0f - length;
            NuQFntMove(system_qfont, time_x, line_y, 0.0f);
            NuQFntPrintU(system_qfont, buffer);
            FS_MakeDateString(reinterpret_cast<FS_FILEENTRYHDR *>(entry), buffer);
            length = NuQFntPrintLenU(system_qfont, buffer);
            NuQFntSetColour(system_qfont, 0x80808020);
            NuQFntMove(system_qfont, time_x - length, line_y, 0.0f);
            NuQFntPrintU(system_qfont, buffer);
        }
        entry = name + NuStrLen(name) + 1;
    }

    const f32 footer_y = y * 16.0f + baseline + list_height;
    NuQFntSet(system_qfont);
    NuQFntSetPointSize(system_qfont, 1.0f, 1.0f);
    NuQFntSetColour(system_qfont, 0x80808080);
    NuQFntMove(system_qfont, x * 16.0f, footer_y, 0.0f);
    NuQFntPrintU(system_qfont, FS_Filter);
    NuQFntSetColour(system_qfont, 0x80000040);
    const f32 sort_length = NuQFntPrintLenU(system_qfont, SortStrings[FS_SortMode]);
    NuQFntMove(system_qfont, (full_width + x) * 16.0f - sort_length, footer_y, 0.0f);
    NuQFntPrintU(system_qfont, SortStrings[FS_SortMode]);
    NuQFntPopCoordinateSystem();
    NuQFntPopPrintMode();
}

void FS_MoveCursorDown(i32 steps) {
    char **currentPos = &FS_CurrentPos;
    char **cursorPos = &FS_CurrentCursorPos;
    while (steps > 0) {
        i32 currentPosLength = NuStrLen(*currentPos);
        i32 cursorLength = NuStrLen(*cursorPos);
        char *next = *cursorPos + cursorLength + 1;
        char *listEnd = FS_FileListEnd;

        if (next < listEnd) {
            i32 *cursorLine = &FS_CursorLine;
            i32 line = *cursorLine;
            if (line == 13) {
                char *nextTop = *currentPos + currentPosLength + 1;
                if (listEnd > nextTop) {
                    *currentPos = nextTop;
                    ++FS_CurrentPosFileNum;
                }
            } else {
                *cursorLine = line + 1;
            }
            *cursorPos = next;
        }
        --steps;
    }
}

i32 FS_PrevNameLen(char *name) {
    if (__builtin_expect(name == FS_FileList, 0)) {
        return 0;
    }
    char *previous = name - 2;
    if (__builtin_expect(*previous == '\0', 0)) {
        return 1;
    }
    if (__builtin_expect(previous == FS_FileList, 0)) {
        return 2;
    }
    i32 length = 0;
    for (;;) {
        --previous;
        ++length;
        if (*previous == '\0') {
            return length + 1;
        }
        if (previous == FS_FileList) {
            return length + 2;
        }
    }
}

void FS_MoveCursorUp(i32 steps) {
    while (steps > 0) {
        i32 currentPosLength = FS_PrevNameLen(FS_CurrentPos);
        i32 cursorLength = FS_PrevNameLen(FS_CurrentCursorPos);
        char *cursor = FS_CurrentCursorPos;

        if (cursor == FS_CurrentPos) {
            FS_CurrentPos = cursor - currentPosLength;
            if (FS_CurrentPosFileNum != 0) {
                --FS_CurrentPosFileNum;
            }
        } else {
            --FS_CursorLine;
        }

        FS_CurrentCursorPos = cursor - cursorLength;
        --steps;
    }
}

void RenderFileSel3(i32 draw_title) {
    if (FS_Active == 0) {
        return;
    }
    RenderFileSel3Body(draw_title);
}

extern "C" {
extern "C" void RenderFileSel2(i32 x, i32 y, i32 *width, i32 *height) {
    FS_X = static_cast<f32>(x);
    FS_Y = static_cast<f32>(y) * 0.5f;
    RenderFileSel3(0);
    *height = static_cast<i32>(FS_H + FS_H);
    *width = static_cast<i32>(FS_W);
}

}

void RenderFileSel() {
    RenderFileSel3(1);
}

f32 FS_GetDirTextWidth() {
    char date_time[64];
    f32 max_width = 0.0f;
    for (char *entry = FS_FileList; entry < FS_FileListEnd;) {
        char *name = entry + 7;
        NuQFntSetPointSize(system_qfont, 1.0f, 1.0f);
        f32 width = NuQFntPrintLenU(system_qfont, name);
        if (entry[6] != '#') {
            NuQFntSetPointSize(system_qfont, 0.8f, 1.0f);
            FS_MakeDateTimeString(reinterpret_cast<FS_FILEENTRYHDR *>(entry), date_time);
            width += NuQFntPrintLenU(system_qfont, date_time);
        }
        if (width > max_width) {
            max_width = width;
        }
        entry = name + NuStrLen(name) + 1;
    }
    return max_width;
}

char *FS_GetFilterString(char *input, char *output) {
    while (*input == ' ' || *input == '*') {
        ++input;
    }

    char *end = input;
    if (*end != '|' && *end != '\0') {
        do {
            ++end;
        } while (*end != '\0' && *end != '|' && *end != '*');

        char *trimmed = end;
        while (trimmed != input && trimmed[-1] == ' ') {
            --trimmed;
        }
        for (; input < trimmed; ++input) {
            *output++ = *input;
        }
    }
    *output = '\0';
    return end;
}

i32 FS_FileNameFilter(char *name) {
    char lower[256];
    NuStrCpy(lower, name);
    NuStrToLower(lower);

    for (i32 i = 0; i < numfilteroutblocks; ++i) {
        if (strstr(lower, filteroutblocks[i]) != NULL) {
            return 0;
        }
    }

    for (i32 i = 0; i < numfilterblocks; ++i) {
        const char *block = filterblocks[i];
        char *scan = lower;
        do {
            char *match;
            if (block[0] != '\0') {
                match = strstr(scan, block);
                if (match == NULL) {
                    break;
                }
                scan = match + 1;
            } else {
                match = scan;
                scan = NULL;
                goto eligible_filter;
            }

            if (block[12] == '\0') {
                goto eligible_filter;
            }
            match = strstr(match, block + 12);
            if (match == NULL) {
                continue;
            }
            if (block[24] == '\0') {
                goto eligible_filter;
            }
            match = strstr(match, block + 24);
            if (match == NULL) {
                continue;
            }
            if (block[36] == '\0') {
                goto eligible_filter;
            }
            match = strstr(match, block + 36);
            if (match == NULL) {
                continue;
            }
            if (block[48] == '\0') {
                goto eligible_filter;
            }
            match = strstr(match, block + 48);
            if (match == NULL) {
                continue;
            }
            if (block[60] == '\0') {
                goto eligible_filter;
            }
            match = strstr(match, block + 60);
            if (match == NULL) {
                continue;
            }
            if (block[72] != '\0') {
                continue;
            }

        eligible_filter:
            i32 dots = 0;
            for (char *p = match; *p != '\0'; ++p) {
                if (*p == '.') {
                    if (++dots > 1) {
                        break;
                    }
                }
            }
            if (dots < 2) {
                return 1;
            }
        } while (scan != NULL);
    }
    return 0;
}

void FS_BuildFilterBlocks(char *input) {
    numfilterblocks = 0;
    NuStrToLower(input);
    i32 slot = 0;
    while (*input != '\0') {
        input = FS_GetFilterString(input, &filterblocks[0][0] + numfilterblocks * 96 + slot * 12);
        if (*input == '|') {
            filterblocks[numfilterblocks][(slot + 1) * 12] = '\0';
            ++numfilterblocks;
            slot = 0;
            ++input;
        } else {
            ++slot;
        }
    }
    if (slot != 0) {
        ++numfilterblocks;
    }
}

void FS_BuildFilterOutBlocks(char *input) {
    numfilteroutblocks = 0;
    NuStrToLower(input);
    while (*input != '\0') {
        input = FS_GetFilterString(input, filteroutblocks[numfilteroutblocks]);
        if (filteroutblocks[numfilteroutblocks][0] != '\0') {
            ++numfilteroutblocks;
        }
        if (*input == '|') {
            ++input;
        }
    }
}

void FS_SortStrings(char *start, char *end, i32 mode) {
    char tmp[256];

    if (!(static_cast<u32>(mode) > 1)) {
        const i32 direction = mode == 0 ? 1 : -1;
        for (char *current = start; current != end;) {
            i32 current_length = NuStrLen(current);
            char *candidate = current + NuStrLen(current) + 1;
            while (candidate != end) {
                const i32 candidate_length = NuStrLen(candidate);
                if (NuStrICmp(current + 7, candidate + 7) == direction) {
                    NuStrCpy(tmp, candidate);
                    memmove(current + candidate_length + 1, current, candidate - current);
                    memcpy(current, tmp, candidate_length + 1);
                    current_length = candidate_length;
                }
                candidate += candidate_length + 1;
            }
            current += current_length + 1;
        }
    } else {
        const i32 direction = mode == 2 ? 1 : -1;
        for (char *current = start; current != end;) {
            u32 current_key = FS_EncodedDateKey(current);
            i32 current_length = NuStrLen(current);
            char *candidate = current + NuStrLen(current) + 1;
            while (candidate != end) {
                const u32 candidate_key = FS_EncodedDateKey(candidate);
                const i32 candidate_length = NuStrLen(candidate);
                const i32 compare = candidate_key < current_key ? 1 : (candidate_key > current_key ? -1 : 0);
                if (compare == direction) {
                    NuStrCpy(tmp, candidate);
                    memmove(current + candidate_length + 1, current, candidate - current);
                    memcpy(current, tmp, candidate_length + 1);
                    current_key = candidate_key;
                    current_length = candidate_length;
                }
                candidate += candidate_length + 1;
            }
            current += current_length + 1;
        }
    }
}

void FS_SetCursorToLastFileName() {
    char *last_name = FS_LastFileName;
    if (last_name[0] == '\0' || FS_NumFiles <= 0) {
        return;
    }
    char *entry = FS_FileList;
    for (i32 index = 0; index < FS_NumFiles; ++index) {
        if (NuStrICmp(entry + 7, last_name) == 0) {
            FS_CurrentCursorPos = entry;
            FS_CurrentPos = entry;
            FS_CursorLine = 0;
            FS_CurrentPosFileNum = index;
            if (index == 0 || index + 13 < FS_NumFiles) {
                return;
            }
            char *current = entry;
            i32 last_visible = index + 12;
            volatile i32 old_visible;
            do {
                i32 previous_length = FS_PrevNameLen(current);
                if (previous_length == 0) {
                    return;
                }
                current = FS_CurrentPos - previous_length;
                --FS_CurrentPosFileNum;
                FS_CurrentPos = current;
                ++FS_CursorLine;
                if (last_visible == 13) {
                    return;
                }
                old_visible = last_visible--;
            } while (old_visible >= FS_NumFiles);
            return;
        }
        entry += NuStrLen(entry) + 1;
    }
}

void FS_GetDirList(char *path, char *filter, char *filter_out) {
    FS_BuildFilterBlocks(filter);
    FS_BuildFilterOutBlocks(filter_out);

    FS_CurrentCursorPos = FS_FileList;
    FS_CurrentPos = FS_FileList;
    FS_NumFiles = 0;
    FS_CurrentPosFileNum = 0;
    FS_CursorLine = 0;

    NUFILE directory = NuFileOpenDir(path);
    __builtin_memcpy(FS_FileList, FS_ParentEntry, 7);
    NuStrCpy(FS_FileList + 7, "..");
    ++FS_NumFiles;

    char *volatile next;
    char entry[0x118] __attribute__((aligned(16)));
    volatile char header[7];
    if (directory != 0) {
        next = FS_FileList + 10;
        while (i32 name_length = NuFileReadDir(directory, entry)) {
            char *name = entry + 0x18;
            if (*reinterpret_cast<u16 *>(name) == '.' || (*reinterpret_cast<u32 *>(name) & 0xffffff) == 0x2e2e ||
                !(entry[0] & 8)) {
                continue;
            }
            if (next + name_length + 8 >= FS_FileList + sizeof(FS_FileList)) {
                break;
            }
            header[0] = 'D';
            header[4] = static_cast<u8>(entry[11]) + 'A';
            header[3] = static_cast<u8>(entry[10]) + 'A';
            header[2] = static_cast<u8>(entry[9]) + 'A';
            header[1] = static_cast<u8>(entry[8]) + 'A';
            header[5] = static_cast<u8>(entry[12]) + 'A';
            u8 encoded_year = static_cast<u8>(entry[14]) - 123;
            header[6] = encoded_year;
            __builtin_memcpy(next, const_cast<const char *>(header), 6);
            next[6] = encoded_year;
            char *name_dest = next + 7;
            NuStrCpy(name_dest, name);
            name_dest[name_length] = '\0';
            next = name_dest + name_length + 1;
            ++FS_NumFiles;
        }
        FS_SortStrings(FS_FileList + 10, next, FS_SortMode);
        NuFileCloseDir(directory);
    } else {
        next = FS_FileList + 10;
        FS_SortStrings(next, next, FS_SortMode);
    }

    char *file_begin = next;
    directory = NuFileOpenDir(path);
    if (directory != 0) {
        while (i32 name_length = NuFileReadDir(directory, entry)) {
            char *name = entry + 0x18;
            if ((entry[0] & 8) || !FS_FileNameFilter(name)) {
                continue;
            }
            if (next + name_length + 8 >= FS_FileList + sizeof(FS_FileList)) {
                break;
            }
            header[0] = 'F';
            header[4] = static_cast<u8>(entry[19]) + 'A';
            header[3] = static_cast<u8>(entry[18]) + 'A';
            header[2] = static_cast<u8>(entry[17]) + 'A';
            header[1] = static_cast<u8>(entry[16]) + 'A';
            header[5] = static_cast<u8>(entry[20]) + 'A';
            u8 encoded_year = static_cast<u8>(entry[22]) - 123;
            header[6] = encoded_year;
            __builtin_memcpy(next, const_cast<const char *>(header), 6);
            next[6] = encoded_year;
            char *name_dest = next + 7;
            NuStrCpy(name_dest, name);
            name_dest[name_length] = '\0';
            next = name_dest + name_length + 1;
            ++FS_NumFiles;
        }
        FS_SortStrings(file_begin, next, FS_SortMode);
        NuFileCloseDir(directory);
    } else {
        FS_SortStrings(file_begin, file_begin, FS_SortMode);
    }
    FS_FileListEnd = next;
}

i32 FS_GetPadWithRepeat(nupad_s *pad, float repeat, float elapsed) {
    static i32 LastPad;
    static float PadRepeat;

    if (pad->digital_buttons != LastPad) {
        LastPad = pad->digital_buttons;
        PadRepeat = 0.5f;
        return LastPad;
    }

    PadRepeat -= elapsed;
    if (PadRepeat < 0.0f) {
        PadRepeat += repeat;
        return LastPad;
    }
    return 0;
}

i32 ProcessFileSel3(f32 elapsed, nupad_s *pad) {
    if (FS_Active == 0)
        return 0;
    if (FS_RefreshDir != 0) {
        FS_GetDirList(FS_Path, FS_Filter, FS_FilterOut);
        NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
        NuQFntSet(system_qfont);
        NuQFntSetPointSize(system_qfont, 1.0f, 1.0f);
        f32 width = FS_GetDirTextWidth() * 0.0625f;
        NuQFntPopCoordinateSystem();
        if (width < 240.0f)
            width = 240.0f;
        FS_Width = width + 8.0f;
        FS_GetPadWithRepeat(pad, 0.05f, elapsed);
        if (FS_LastFileName[0] != '\0')
            FS_SetCursorToLastFileName();
        FS_RefreshDir = 0;
    }
    const i32 buttons = FS_GetPadWithRepeat(pad, 0.05f, elapsed);
    if ((buttons & 0x4000) != 0)
        FS_MoveCursorDown(1);
    if ((buttons & 0x1000) != 0)
        FS_MoveCursorUp(1);
    if ((buttons & 1) != 0)
        FS_MoveCursorDown(14);
    if ((buttons & 4) != 0)
        FS_MoveCursorUp(14);
    if ((buttons & 2) != 0)
        FS_MoveCursorDown(FS_NumFiles);
    if ((buttons & 8) != 0)
        FS_MoveCursorUp(FS_NumFiles);
    if ((buttons & 0x80) != 0) {
        FS_RefreshDir = 2;
        FS_SortMode = static_cast<i32>(static_cast<u32>(FS_SortMode) + 1);
        if (FS_SortMode == 4)
            FS_SortMode = 0;
        NuStrCpy(FS_LastFileName, FS_CurrentCursorPos + 7);
    }
    if ((buttons & 0x20) != 0) {
        const i32 length = NuStrLen(FS_Path);
        if (length != 0 && FS_Path[length - 1] == '\\')
            FS_Path[length - 1] = '\0';
        char *separator = NuStrRChr(FS_Path, '\\');
        if (separator != NULL)
            separator[1] = '\0';
        else
            NuStrCat(FS_Path, "\\");
        FS_RefreshDir = 1;
        FS_LastFileName[0] = '\0';
    }
    if ((buttons & 0x40) != 0) {
        if (FS_CurrentCursorPos[0] == 'V') {
            FS_ShowVolumes = 0;
            NuStrCpy(FS_Path, FS_CurrentCursorPos + 7);
            FS_RefreshDir = 1;
            FS_LastFileName[0] = '\0';
        } else if (FS_CurrentCursorPos[0] == 'D') {
            if (FS_CurrentCursorPos[7] == '.' && FS_CurrentCursorPos[8] == '.' && FS_CurrentCursorPos[9] == '\0') {
                const i32 length = NuStrLen(FS_Path);
                if (length != 0 && FS_Path[length - 1] == '\\')
                    FS_Path[length - 1] = '\0';
                char *separator = NuStrRChr(FS_Path, '\\');
                if (separator != NULL)
                    separator[1] = '\0';
                else
                    NuStrCat(FS_Path, "\\");
            } else {
                const i32 length = NuStrLen(FS_Path);
                if (length != 0 && FS_Path[length - 1] != '\\')
                    NuStrCat(FS_Path, "\\");
                NuStrCat(FS_Path, FS_CurrentCursorPos + 7);
                NuStrCat(FS_Path, "\\");
            }
            FS_RefreshDir = 1;
            FS_LastFileName[0] = '\0';
        } else {
            FS_Active = 0;
            NuStrCpy(FS_LastFileName, FS_CurrentCursorPos + 7);
            if (FS_Callback != NULL)
                FS_Callback(FS_Path, FS_CurrentCursorPos + 7);
            return 1;
        }
    }
    return 0;
}

extern "C" {
extern "C" i32 ProcessFileSel2(f32 elapsed, nupad_s *pad) {
    return ProcessFileSel3(elapsed, pad);
}

extern "C" void FileSelKill(void) {
    NuStrCpy(FS_LastFileName, FS_CurrentCursorPos + 7);
    FS_Active = 0;
}

}

void ProcessFileSel(f32 elapsed, nupad_s *pad) {
    ProcessFileSel3(elapsed, pad);
    RenderFileSel();
    if ((pad->digital_buttons & 0x10) != 0)
        FileSelKill();
}

extern "C" {
    void FS_SetFileSelPathFromName(char *name) {
        char file_name[256] = "";
        NuStrCpy(FS_Path, name);
        if (NuStrRChr(FS_Path, '.') != NULL) {
            char *separator = NuStrRChr(FS_Path, '\\');
            if (separator != NULL) {
                NuStrCpy(file_name, separator + 1);
                separator[1] = '\0';
            } else {
                NuStrCpy(file_name, name);
                FS_Path[0] = '\0';
            }
        }
        if (file_name[0] != '\0') {
            reinterpret_cast<i32 (*)(char *, char *, i32)>(&NuFileExtConvert)(FS_LastFileName, file_name, 255);
        } else {
            FS_LastFileName[0] = '\0';
        }
    }

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *)) {
    if (title != NULL)
        NuStrCpy(FS_Title, title);
    if (path != NULL)
        NuStrCpy(FS_Path, path);
    if (filter != NULL)
        NuStrCpy(FS_Filter, filter);
    else
        FS_Filter[0] = '\0';
    if (filter_out != NULL)
        NuStrCpy(FS_FilterOut, filter_out);
    else
        FS_FilterOut[0] = '\0';
    FS_Active = 1;
    FS_Callback = callback;
    FS_RefreshDir = 2;
}

}

eduiitem_s *eduiItemFileSelectorCreate(u32 data, eduiiattr_s *colours, EdUiItemCallback callback, char *text) {
    struct FileSelectorItem : eduiitem_s {
        i32 field_48;
        f32 field_4c;
        f32 field_50;
        f32 field_54;
        f32 field_58;
        f32 field_5c;
        EdUiItemCallback callback;
    };
    DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(FileSelectorItem) == 0x64, "file selector item ABI");

    auto *item = static_cast<FileSelectorItem *>(NU_ALLOC(sizeof(FileSelectorItem), 4, 1, "", 0));
    if (__builtin_expect(item != NULL, 1)) {
        memset(item, 0, sizeof(*item));
        item->type = 0x10;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->destroy = eduicbItemDestroy;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        item->field_4c = 0.5f;
        item->field_50 = 0.5f;
        item->field_54 = 180.0f;
        item->field_5c = 0.5f;
        item->field_58 = 1.0f;
        item->callback = callback;
    }
    return item;
}

extern "C" {
    void cbTriggerSubMenu(eduimenu_s *menu, eduiitem_s *item, u32) {
        eduimenu_s *submenu = static_cast<eduimenu_s *>(item->data_ptr);
        if (submenu != NULL) {
            eduiMenuAttach(menu, submenu);
            submenu->x = item->x + 10;
            submenu->y = MIN(50, PS2_REZ_H * 2 - submenu->height);
        }
        item->highlighted = 0;
    }

    void cbCancelSubMenuFromItem(eduimenu_s *menu, eduiitem_s *, u32) {
        eduiMenuDetach(menu);
    }

    void cbNearClipAtCursor(eduimenu_s *, eduiitem_s *item, u32) {
        near_clip_at_cursor = reinterpret_cast<const u8 *>(item)[0x11] & 1;
    }

extern "C" void edDrawCross(NUVEC *centre, u8 red, u8 green, u8 blue) {
    i32 colour = 0xff000000 | red | (green << 8) | (blue << 16);
    NuRndrLine3dDbg(centre->x - 0.5f, centre->y, centre->z, centre->x + 0.5f, centre->y, centre->z, colour);
    NuRndrLine3dDbg(centre->x, centre->y - 0.5f, centre->z, centre->x, centre->y + 0.5f, centre->z, colour);
    NuRndrLine3dDbg(centre->x, centre->y, centre->z - 0.5f, centre->x, centre->y, centre->z + 0.5f, colour);
}

}

void edDrawLine(nuvec_s *, nuvec_s *, unsigned char, unsigned char, unsigned char) {
}

extern "C" {
extern "C" void AddColourPick(eduimenu_s *parent, eduiitem_s *item, f32 *red, f32 *green, f32 *blue, u32 *rgba) {
    eduiMenuAttach(parent, colourmenu);
    colourmenu->x = (item != nullptr ? item->x : parent->x) + 10;
    i32 y = (item != nullptr ? item->y : parent->y) + 10;
    i32 bottom = PS2_REZ_H - 20 - colourmenu->height;
    colourmenu->y = y <= bottom ? y : bottom;
    cp_rgba = rgba;

    f32 red_value;
    f32 green_value;
    f32 blue_value;
    if (rgba != nullptr) {
        u32 colour = *rgba;
        red_value = static_cast<f32>(colour & 0xff) / 255.0f;
        green_value = static_cast<f32>((colour >> 8) & 0xff) / 255.0f;
        blue_value = static_cast<f32>((colour >> 16) & 0xff) / 255.0f;
        f32 alpha_value = static_cast<f32>(colour >> 24) / 255.0f;
        cpt_r = red_value;
        cpt_g = green_value;
        cpt_b = blue_value;
        cpt_a = alpha_value;
        cp_r = &cpt_r;
        cp_g = &cpt_g;
        cp_b = &cpt_b;
    } else {
        cp_r = red;
        cp_g = green;
        cp_b = blue;
        red_value = *red;
        green_value = *green;
        blue_value = *blue;
    }
    cp_a = &cpt_a;

    eduiItemColourPickSetRGB(cp_item, red_value, green_value, blue_value);
    char text[256];
    sprintf(text, "R:%1.2f G:%1.2f B:%1.2f", *cp_r, *cp_g, *cp_b);
    eduiItemSetText(cp_info, text);
}

extern "C" void CreateColourPicker(void) {
    static i32 initialised;
    const u32 colours[4] __attribute__((aligned(16))) = {0x80000000u, 0x800000ffu, 0x80808080u, 0x80f0f0f0u};
    if (initialised != 0)
        return;

    initialised = 1;
    colourmenu = eduiMenuCreate(200, 70, 180, 250, nullptr, cbCancelSubMenu, const_cast<char *>("Pick Colour"));
    if (colourmenu == nullptr)
        return;

    cp_item = static_cast<edui_colour_pick_s *>(
        eduiMenuAddItem(colourmenu, eduiItemColourPickCreate(0, colours, cbColourPickSel, const_cast<char *>(""))));
    cp_process = cp_item->process;
    cp_item->process = cbProcessColourPick;
    cp_info = eduiMenuAddItem(
        colourmenu, eduiItemSelCreate(1, colours, 0, 0, cbToggleIndicatorMode, const_cast<char *>("R:?? G:?? B:??")));
    cp_copy = eduiMenuAddItem(colourmenu,
                              eduiItemSelCreate(1, colours, 0, 0, cbCopy, const_cast<char *>("Copy To Clipboard")));
    cp_paste = eduiMenuAddItem(
        colourmenu, eduiItemSelCreate(1, colours, 0, 0, cbPaste, const_cast<char *>("Paste From Clipboard")));
}

extern "C" void RndrOSphere(NUVEC *centre, f32 radius, i32 colour, i32 segments, i32) {
    NUMTX_ALIGNED32 matrix = global_camera.mtx;
    matrix.m30 = centre->x;
    matrix.m31 = centre->y;
    matrix.m32 = centre->z;
    i32 step = 65536 / segments;
    NURND_VERTEX3D vertices[2];
    vertices[0].position.x = radius;
    vertices[0].position.y = 0.0f;
    vertices[0].position.z = 0.0f;
    vertices[0].colour = colour;
    vertices[1].colour = colour;
    NuVecRotateZ(&vertices[1].position, &vertices[0].position, step);
    for (i32 i = 0; i < segments; ++i) {
        NUVEC_ALIGNED16 start = vertices[0].position;
        NUVEC_ALIGNED16 end = vertices[1].position;
        NuVecMtxTransform(&start, &start, &matrix);
        NuVecMtxTransform(&end, &end, &matrix);
        NuRndrLine3dDbg(start.x, start.y, start.z, end.x, end.y, end.z, vertices[0].colour);
        vertices[0].position = vertices[1].position;
        NuVecRotateZ(&vertices[1].position, &vertices[1].position, step);
    }
}

extern "C" void RndrCircleXZ(NUVEC *centre, f32 radius, i32 colour, i32 segments) {
    NUMTX_ALIGNED32 matrix = numtx_identity;
    matrix.m30 = centre->x;
    matrix.m31 = centre->y;
    matrix.m32 = centre->z;
    i32 step = 65536 / segments;
    NURND_VERTEX3D vertices[2];
    vertices[0].position.x = radius;
    vertices[0].position.y = 0.0f;
    vertices[0].position.z = 0.0f;
    vertices[0].colour = colour;
    vertices[1].colour = colour;
    NuVecRotateY(&vertices[1].position, &vertices[0].position, step);
    for (i32 i = 0; i < segments; ++i) {
        NUVEC_ALIGNED16 start = vertices[0].position;
        NUVEC_ALIGNED16 end = vertices[1].position;
        NuVecMtxTransform(&start, &start, &matrix);
        NuVecMtxTransform(&end, &end, &matrix);
        NuRndrLine3dDbg(start.x, start.y, start.z, end.x, end.y, end.z, vertices[0].colour);
        vertices[0].position = vertices[1].position;
        NuVecRotateY(&vertices[1].position, &vertices[1].position, step);
    }
}

extern "C" void RndrOSquare(NUVEC *centre, f32 radius, i32 colour) {
    f32 corners[4][2] = {{-1.0f, -1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, 1.0f}};
    NUMTX_ALIGNED32 matrix = global_camera.mtx;
    matrix.m31 = centre->y;
    matrix.m30 = centre->x;
    matrix.m32 = centre->z;
    for (i32 remaining = 4; remaining > 0; --remaining) {
        const i32 i = 4 - remaining;
        NUVEC_ALIGNED16 start;
        NUVEC_ALIGNED16 end;
        if (i == 0) {
            start = NUVEC{-radius, -radius, 0.0f};
            end = NUVEC{radius, -radius, 0.0f};
        } else {
            const f32 start_y = corners[i][1] * radius;
            const f32 end_x = corners[(i + 1) & 3][0] * radius;
            const f32 end_y = corners[(i + 1) & 3][1] * radius;
            const f32 start_x = corners[i][0] * radius;
            start = NUVEC{start_x, start_y, 0.0f};
            end = NUVEC{end_x, end_y, 0.0f};
        }
        NuVecMtxTransform(&start, &start, &matrix);
        NuVecMtxTransform(&end, &end, &matrix);
        NuRndrLine3dDbg(start.x, start.y, start.z, end.x, end.y, end.z, colour);
    }
}

    void FlushMenuHighlights(eduimenu_s *menu) {
        for (eduiitem_s *item = menu->first; item != NULL; item = item->next) {
            if (item->type == 0) {
                item->highlighted = 0;
            }
        }
    }

}
