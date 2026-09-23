#include "decomp.h"
#include "gameapi_edtools_types.h"
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
#include "nu2api/numath/nurand.h"

EdRegistry theRegistry;
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
EdManipulator theDefaultManipulator;
EdManMove theMoveManipulator;
EdManRotate theRotateManipulator;
EdManScale theScaleManipulator;
SplineTool theSplineTool;
DECOMP_ASSERT(sizeof(EdManipulator) == 0x6c, "EdManipulator ABI");
SplineHelper theSplineHelper;
KnotHelper theKnotHelper;
extern ClassEditor theClassEditor;
i32 pad_disabled;
EdSystem theEdSystem;
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
    i32 edgra_mode = 1;
    extern NUGSCN *edbits_base_scene;
    extern NUGSCN *edbits_things_scene;
    extern part_type_s part_types[128];
    extern part_emit_s part_emits[512];
    extern i32 edpart_emitrotx, edpart_emitroty, edpart_emitrotz;
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
    i32 edbits_particle_level_page;
}

void edpartDestroy(i32 index);

i32 edpartLookupObjectInScene(char *, NUGSCN *);

void EdTerrInit(void *, void *) {
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

void edpartPlace(i32 index, nuvec_s *position) {
    part_emit_s *emitter = &part_emits[index];
    emitter->position = *position;
    emitter->rotation_2c = static_cast<i16>(edpart_emitrotz);
    emitter->rotation_30 = static_cast<i16>(edpart_emitrotx);
    emitter->rotation_2e = static_cast<i16>(edpart_emitroty);
}

void edppDetermineNearest(f32);
void edppMultipleCopyPaste();
void edppMultipleCopyCopy();
void edppMultipleCopyClear();
i32 edppPtlCreate(NUVEC *, i32);
i32 edppPtlPlace(i32, NUVEC *);
void edppPtlDestroy(i32);

void edppDoInput(nupad_s *pad) {
    extern i32 edpp_copy_mode, edpp_copy_source_count, edpp_snap_enabled;
    extern i32 edpp_cam_ax, edpp_cam_ay;
    extern i32 edpp_copyroty;
    extern f32 edpp_offset;
    extern eduimenu_s *edpp_active_menu, *ptloptmenu;

    if ((pad->digital_buttons & 0x100) == 0)
        edcamMove(pad);
    if (pad->digital_buttons & 0x100) {
        if (pad->digital_buttons_pressed & 0x20) {
            edpp_copy_mode = !edpp_copy_mode;
            if (edpp_copy_mode)
                edpp_copy_source_count = 0;
        }
        if (edpp_nearest == -1) {
            edppDetermineNearest(-1.0f);
        } else {
            if (pad->digital_buttons_pressed & 8) {
                do {
                    ++edpp_nearest;
                    if (edpp_nearest == 512)
                        edpp_nearest = 0;
                } while (edpp_ptls[edpp_nearest].instance_id == 99999 || edpp_ptls[edpp_nearest].instance_id == -1);
            }
            if (pad->digital_buttons_pressed & 2) {
                do {
                    --edpp_nearest;
                    if (edpp_nearest == -1)
                        edpp_nearest = 511;
                } while (edpp_ptls[edpp_nearest].instance_id == 99999 || edpp_ptls[edpp_nearest].instance_id == -1);
            }
        }
        if (edpp_nearest != -1) {
            edpp_particle_s &particle = edpp_ptls[edpp_nearest];
            edcamSetPos(&particle.position);
            edpp_rotz = particle.rotation_z;
            edpp_roty = particle.rotation_y;
            edpp_emitrotz = particle.emitter_rotation_z;
            edpp_emitroty = particle.emitter_rotation_y;
            edpp_emitrotx = particle.emitter_rotation_x;
            edpp_offset = particle.start_offset;
            edpp_create_type = particle.effect_index;
            edpp_effect_list = debtab[particle.effect_index]->category;
        }
    }

    if (edpp_snap_enabled == 0)
        edcamGetPosAng(&edpp_cam_pos, &edpp_cam_ax, &edpp_cam_ay);
    else
        edcamGetPosAngSnap(&edpp_cam_pos, &edpp_cam_ax, &edpp_cam_ay);

    if ((pad->digital_buttons & 0x100) == 0) {
        if (pad->digital_buttons_pressed & 0x80)
            edpp_active_menu = ptloptmenu;
        if (pad->digital_buttons_pressed & 0x40) {
            if (edpp_copy_mode != 0)
                edppMultipleCopyPaste();
            else if (edpp_create_type != -1)
                edppPtlCreate(&edpp_cam_pos, edpp_create_type);
        }
        if (pad->digital_buttons & 0x20) {
            if (edpp_copy_mode != 0)
                edppMultipleCopyCopy();
            else if (edpp_nearest != -1)
                edppPtlPlace(edpp_nearest, &edpp_cam_pos);
        }
        if ((pad->digital_buttons & 0x400) && edpp_copy_mode == 0 && edpp_nearest != -1)
            edppPtlPlace(edpp_nearest, &edpp_cam_pos);
        if (pad->digital_buttons_pressed & 0x10) {
            if (edpp_copy_mode != 0)
                edppMultipleCopyClear();
            else if (edpp_nearest != -1) {
                edppPtlDestroy(edpp_nearest);
                edpp_nearest = -1;
            }
        }
    }

    const i32 right = pad->analog_left_pad_right;
    const i32 left = pad->analog_left_pad_left;
    const i32 up = pad->analog_left_pad_up;
    const i32 down = pad->analog_left_pad_down;
    if (edpp_copy_mode != 0) {
        edpp_copy_size += static_cast<f32>(up - down) / 5000.0f;
        if (edpp_copy_size < 0.05f)
            edpp_copy_size = 0.05f;
        if (edpp_copy_size > 2.0f)
            edpp_copy_size = 2.0f;
        edpp_copyroty += right - left;
        return;
    }

    switch (edpp_dpad_mode) {
        case 0:
            if (pad->digital_buttons & 0x200)
                edpp_emitrotz = edpp_emitroty = edpp_emitrotx = 0;
            if (pad->digital_buttons & 0x400) {
                edpp_emitrotx += right - left;
            } else {
                edpp_emitroty += right - left;
                i32 rotation = edpp_emitrotz + up;
                if (rotation > 0x8000)
                    rotation = 0x8000;
                rotation -= down;
                if (rotation < -0x8000)
                    rotation = -0x8000;
                edpp_emitrotz = rotation;
            }
            break;
        case 1: {
            if (pad->digital_buttons & 0x200)
                edpp_rotz = edpp_roty = 0;
            edpp_roty += right - left;
            i32 rotation = edpp_rotz + up;
            if (rotation > 0)
                rotation = 0;
            rotation -= down;
            if (rotation < -0x8000)
                rotation = -0x8000;
            edpp_rotz = rotation;
            break;
        }
        case 2:
            if (up == 255 || (pad->digital_buttons_pressed & 0x1000))
                edpp_offset += 1.25f;
            if (down == 255 || (pad->digital_buttons_pressed & 0x4000))
                edpp_offset -= 1.25f;
            if (edpp_offset < 0.0f)
                edpp_offset = 0.0f;
            break;
        case 3: {
            edpp_refroty += right - left;
            i32 rotation = edpp_refrotz + up;
            if (rotation > 0)
                rotation = 0;
            rotation -= down;
            if (rotation < -0x8000)
                rotation = -0x8000;
            edpp_refrotz = rotation;
            break;
        }
        case 4:
            edpp_facroty += right - left;
            edpp_facrotx -= up;
            if (edpp_facrotx < -0x4000)
                edpp_facrotx = -0x4000;
            edpp_facrotx += down;
            if (edpp_facrotx > 0x4000)
                edpp_facrotx = 0x4000;
            break;
    }
}

void EdTerrShadow(nuvec_s *position, float height_above, float height_below, i32 terrain_mask) {
    reinterpret_cast<void (*)(nuvec_s *, f32, f32, i32)>(NewShadow)(position, height_above, height_below, terrain_mask);
}

void edpartCreate(nuvec_s *position, i32 type) {
    AddPARTEffect(type, position);
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

void edanimDoInput(nupad_s *pad) {
    if ((pad->digital_buttons & 0x100) == 0)
        edcamMove(pad);
    const auto pressed = pad->digital_buttons_pressed;

    if ((pad->digital_buttons & 0x100) == 0) {
        if (edanim_nearest_param_id != -1) {
            auto &param = AnimParams[edanim_nearest_param_id];
            for (i32 effect = 0; effect < param.effect_count;) {
                if ((param.effect_ids[effect] == -1 || debtab[param.effect_ids[effect]] == nullptr) &&
                    param.effect_names[effect][0] != '\0') {
                    param.effect_ids[effect] = LookupDebrisEffect(param.effect_names[effect]);
                    if (param.effect_ids[effect] == -1) {
                        edanimParticleDestroy(edanim_nearest_param_id, effect);
                        continue;
                    }
                }
                ++effect;
            }
        }
    } else {
        bool particle_selection_started = false;
        bool sound_selection_started = false;
        if (edanim_sound_mode == 0 && edanim_particle_mode != 0 && (pressed & 0x80)) {
            edanim_particle_mode = 0;
        } else if (edanim_sound_mode != 0 && (pressed & 0x10)) {
            edanim_sound_mode = 0;
        } else if (edanim_particle_mode == 0 && edanim_sound_mode == 0 && edanim_nearest != -1 &&
                   edanim_nearest_param_id != -1) {
            if (pressed & 0x80) {
                edanim_particle_mode = 1;
                edanim_nearest_particle = -1;
                edanimDetermineNearestParticle(-1.0f);
                particle_selection_started = true;
            } else if (pressed & 0x10) {
                edanim_sound_mode = 1;
                edanim_nearest_sound = -1;
                edanimDetermineNearestSound(-1.0f);
                sound_selection_started = true;
            }
        }
        if (edanim_particle_mode != 0) {
            if (edanim_nearest_particle == -1) {
                if (!particle_selection_started)
                    edanimDetermineNearestParticle(-1.0f);
            } else {
                auto &param = AnimParams[edanim_nearest_param_id];
                if (pressed & 0x8) {
                    if (++edanim_nearest_particle == param.effect_count)
                        edanim_nearest_particle = 0;
                }
                if (pressed & 0x2) {
                    if (--edanim_nearest_particle == -1)
                        edanim_nearest_particle = param.effect_count - 1;
                }
            }
            if (edanim_nearest_particle != -1) {
                nuhspecial_s special;
                NuGScnGetSpecial(&special, edbits_base_scene, edanim_nearest);
                auto &param = AnimParams[edanim_nearest_param_id];
                NUVEC position;
                NuVecAdd(&position, NuSpecialGetPos(&special),
                         reinterpret_cast<NUVEC *>(param.effect_positions[edanim_nearest_particle]));
                edcamSetPos(&position);
                edanim_emitrotz = param.effect_angles[edanim_nearest_particle];
                edanim_emitroty = param.effect_angle_ranges[edanim_nearest_particle];
                edanim_particle_type = param.effect_ids[edanim_nearest_particle];
            }
        } else if (edanim_sound_mode != 0) {
            if (edanim_nearest_sound == -1) {
                if (!sound_selection_started)
                    edanimDetermineNearestSound(-1.0f);
            } else {
                auto &param = AnimParams[edanim_nearest_param_id];
                if (pressed & 0x8) {
                    if (++edanim_nearest_sound == param.sound_count)
                        edanim_nearest_sound = 0;
                }
                if (pressed & 0x2) {
                    if (--edanim_nearest_sound == -1)
                        edanim_nearest_sound = param.sound_count - 1;
                }
            }
            if (edanim_nearest_sound != -1) {
                nuhspecial_s special;
                NuGScnGetSpecial(&special, edbits_base_scene, edanim_nearest);
                auto &param = AnimParams[edanim_nearest_param_id];
                NUVEC position;
                NuVecAdd(&position, NuSpecialGetPos(&special),
                         reinterpret_cast<NUVEC *>(param.sound_positions[edanim_nearest_sound]));
                edcamSetPos(&position);
                edanim_sound_type = param.sound_ids[edanim_nearest_sound];
            }
        } else {
            if (edanim_nearest == -1) {
                edanimDetermineNearestAnim(-1.0f);
            } else {
                if (pressed & 0x8) {
                    if (++edanim_nearest == NuGScnNumSpecials(edbits_base_scene))
                        edanim_nearest = 0;
                }
                if (pressed & 0x2) {
                    if (--edanim_nearest == -1)
                        edanim_nearest = NuGScnNumSpecials(edbits_base_scene) - 1;
                }
            }
            if (edanim_nearest != -1) {
                nuhspecial_s special;
                NuGScnGetSpecial(&special, edbits_base_scene, edanim_nearest);
                edcamSetPos(NuSpecialGetPos(&special));
                edanim_nearest_param_id = -1;
                for (i32 index = 0; index < 64; ++index) {
                    if (AnimParams[index].instance_id == edanim_nearest) {
                        edanim_nearest_param_id = index;
                        break;
                    }
                }
            }
        }
    }

    edcamGetPosAng(&edanim_cam_pos, &edanim_cam_ax, &edanim_cam_ay);
    if ((pad->digital_buttons & 0x100) == 0) {
        if (pressed & 0x80)
            edanim_active_menu = edanim_options_menu;
        if (pressed & 0x40) {
            if (edanim_particle_mode != 0) {
                edanimParticleCreate(&edanim_cam_pos);
            } else if (edanim_sound_mode != 0) {
                edanimSoundCreate(&edanim_cam_pos);
            } else if (edanim_nearest != -1 && edanim_nearest_param_id == -1) {
                edanim_nearest_param_id = edanimParamCreate(edanim_nearest);
            }
        }
        if (pad->digital_buttons & 0x20) {
            if (edanim_particle_mode != 0 && edanim_nearest_particle != -1) {
                edanimParticlePlace(edanim_nearest_particle, &edanim_cam_pos);
            } else if (edanim_sound_mode != 0 && edanim_nearest_sound != -1) {
                edanimSoundPlace(edanim_nearest_sound, &edanim_cam_pos);
            }
        }
        if (pressed & 0x10) {
            if (edanim_particle_mode != 0) {
                if (edanim_nearest_particle != -1) {
                    edanimParticleDestroy(edanim_nearest_param_id, edanim_nearest_particle);
                    edanim_nearest_particle = -1;
                }
            } else if (edanim_sound_mode != 0) {
                if (edanim_nearest_sound != -1) {
                    edanimSoundDestroy(edanim_nearest_param_id, edanim_nearest_sound);
                    edanim_nearest_sound = -1;
                }
            } else {
                if (edanim_nearest_param_id != -1)
                    edanimParamDestroy(edanim_nearest_param_id);
                edanim_nearest_param_id = -1;
                edanim_nearest = -1;
            }
        }
    }
    if (edanim_particle_mode != 0) {
        edanim_emitroty += pad->analog_left_pad_right - pad->analog_left_pad_left;
        const i32 raised = edanim_emitrotz + pad->analog_left_pad_up;
        const i32 rotation = (raised < 0 ? raised : 0) - pad->analog_left_pad_down;
        edanim_emitrotz = rotation > -32768 ? rotation : -32768;
    }
}

void edbobsDrawBox(nuvec_s *minimum, nuvec_s *maximum, i32 colour) {
    NuRndrLine3dDbg(minimum->x, minimum->y, minimum->z, maximum->x, minimum->y, minimum->z, colour);
    NuRndrLine3dDbg(maximum->x, minimum->y, minimum->z, maximum->x, minimum->y, maximum->z, colour);
    NuRndrLine3dDbg(maximum->x, minimum->y, maximum->z, minimum->x, minimum->y, maximum->z, colour);
    NuRndrLine3dDbg(minimum->x, minimum->y, maximum->z, minimum->x, minimum->y, minimum->z, colour);
    NuRndrLine3dDbg(minimum->x, maximum->y, minimum->z, maximum->x, maximum->y, minimum->z, colour);
    NuRndrLine3dDbg(maximum->x, maximum->y, minimum->z, maximum->x, maximum->y, maximum->z, colour);
    NuRndrLine3dDbg(maximum->x, maximum->y, maximum->z, minimum->x, maximum->y, maximum->z, colour);
    NuRndrLine3dDbg(minimum->x, maximum->y, maximum->z, minimum->x, maximum->y, minimum->z, colour);
    NuRndrLine3dDbg(minimum->x, minimum->y, minimum->z, minimum->x, maximum->y, minimum->z, colour);
    NuRndrLine3dDbg(maximum->x, minimum->y, minimum->z, maximum->x, maximum->y, minimum->z, colour);
    NuRndrLine3dDbg(maximum->x, minimum->y, maximum->z, maximum->x, maximum->y, maximum->z, colour);
    NuRndrLine3dDbg(minimum->x, minimum->y, maximum->z, minimum->x, maximum->y, maximum->z, colour);
}

i32 edbriFileSave(char *path) {
    i32 count = 0;
    for (i32 i = 0; i < 64; ++i)
        if (edBridges[i].connection_index != 0xff)
            ++count;
    EdFileSetMedia(1);
    if (!EdFileOpen(path, NUFILE_WRITE))
        return 0;
    EdFileWriteInt(1);
    EdFileWriteInt(count);
    for (i32 i = 0; i < 64; ++i) {
        edbridge_s &bridge = edBridges[i];
        if (bridge.connection_index == 0xff)
            continue;
        EdFileWriteNuVec(&bridge.position);
        EdFileWriteFloat(bridge.length);
        EdFileWriteFloat(bridge.field_14);
        EdFileWriteShort(bridge.rotation_z);
        EdFileWriteShort(bridge.rotation_y);
        EdFileWriteChar(bridge.field_1d);
        EdFileWriteChar(bridge.field_1e);
        char name[20];
        if (bridge.special_20 == -1)
            name[0] = '\0';
        else {
            nuhspecial_s special;
            NuGScnGetSpecial(&special, edbits_base_scene, bridge.special_20);
            strncpy(name, NuSpecialGetName(&special), sizeof(name));
        }
        EdFileWrite(name, sizeof(name));
        if (bridge.special_24 == -1)
            name[0] = '\0';
        else {
            nuhspecial_s special;
            NuGScnGetSpecial(&special, edbits_base_scene, bridge.special_24);
            strncpy(name, NuSpecialGetName(&special), sizeof(name));
        }
        EdFileWrite(name, sizeof(name));
        EdFileWriteFloat(bridge.field_28);
        EdFileWriteFloat(bridge.field_2c);
        EdFileWriteFloat(bridge.field_30);
        EdFileWriteFloat(bridge.field_34);
        EdFileWriteFloat(bridge.field_38);
        EdFileWriteFloat(bridge.field_3c);
        EdFileWriteChar(bridge.red);
        EdFileWriteChar(bridge.green);
        EdFileWriteChar(bridge.blue);
        EdFileWriteChar(bridge.field_43);
    }
    EdFileClose();
    return 1;
}

i32 edgraFileSave(char *path) {
    i32 clump_count = 0;
    i32 element_count = 0;
    for (i32 i = 0; i < EDGRA_MAX_CLUMPS; ++i) {
        edgra_clump_s &clump = GrassClumps[i];
        if (clump.element_count)
            ++clump_count;
        element_count += clump.element_count;
    }

    EdFileSetMedia(1);
    if (!EdFileOpen(path, NUFILE_WRITE))
        return 0;
    EdFileSetReadWrongEndianess(1);
    EdFileWriteInt(9);
    EdFileWriteFloat(edgra_global_fadein);
    EdFileWriteFloat(edgra_global_fadeout);
    EdFileWriteInt(clump_count);
    EdFileWriteInt(element_count);

    for (i32 i = 0; i < EDGRA_MAX_CLUMPS; ++i) {
        edgra_clump_s &clump = GrassClumps[i];
        if (!clump.element_count)
            continue;
        nuhspecial_s special;
        NuGScnGetSpecial(&special, edbits_base_scene, clump.special_index);
        char name[20];
        strncpy(name, NuSpecialGetName(&special), sizeof(name));
        name[19] = '\0';
        EdFileWrite(name, sizeof(name));
        EdFileWriteInt(clump.element_count);
        EdFileWriteNuVec(&clump.position);
        EdFileWriteFloat(clump.size);
        EdFileWriteFloat(clump.field_18);
        EdFileWriteInt(clump.flags);
        EdFileWriteFloat(clump.field_20);
        EdFileWriteChar(clump.unknown_25);
        EdFileWriteChar(clump.unknown_26);
        EdFileWriteChar(clump.kind);
        EdFileWriteInt(clump.seed);
        EdFileWriteFloat(clump.field_2c);
        EdFileWriteFloat(clump.field_30);
        EdFileWriteShort(clump.rotation_z);
        EdFileWriteShort(clump.rotation_y);
        EdFileWriteFloat(clump.near_distance);
        EdFileWriteFloat(clump.far_distance);
        EdFileWriteChar(clump.field_42);
        EdFileWriteChar(clump.field_43);
        EdFileWriteFloat(clump.field_44);
        if (clump.kind == 3) {
            for (i32 j = 0; j < clump.element_count; ++j) {
                edgra_individual_s *individual = GetIndGrassClump(clump.individual_index, j);
                EdFileWriteNuVec(&individual->position);
                EdFileWriteFloat(individual->field_0c);
                EdFileWriteShort(individual->field_10);
                EdFileWriteShort(individual->field_12);
            }
        }
        NUVEC *vectors = static_cast<NUVEC *>(clump.vector_buffer);
        for (i32 j = 0; j < clump.element_count; ++j)
            EdFileWriteNuVec(&vectors[j]);
    }

    EdFileSetReadWrongEndianess(0);
    EdFileClose();
    return 1;
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

void EdDrawPolyAxis(VuMtx const &transform, float size, i32 opacity) {
    const NUMTX &matrix = transform.matrix;
    VuVec origin(matrix.m30, matrix.m31, matrix.m32, 0.0f);
    VuVec tip(matrix.m30 + matrix.m00 * size, matrix.m31 + matrix.m01 * size, matrix.m32 + matrix.m02 * size, 0.0f);
    const float width = size * 0.02f;
    EdDrawPolyArrow(origin, tip, 8, static_cast<i32>(0xff000000 | (opacity & 0xff)), width, width, 0.02f, 0.0f);
    tip.x = matrix.m30 + matrix.m10 * size;
    tip.y = matrix.m31 + matrix.m11 * size;
    tip.z = matrix.m32 + matrix.m12 * size;
    EdDrawPolyArrow(origin, tip, 8, static_cast<i32>(0xff000000 | ((opacity & 0xff) << 8)), width, width, 0.02f, 0.0f);
    tip.x = matrix.m30 + matrix.m20 * size;
    tip.y = matrix.m31 + matrix.m21 * size;
    tip.z = matrix.m32 + matrix.m22 * size;
    EdDrawPolyArrow(origin, tip, 8, static_cast<i32>(0xff000000 | ((opacity & 0xff) << 16)), width, width, 0.02f, 0.0f);
}

i32 edanimFileSave(char *path) {
    i32 parameter_count = 0;
    for (const auto &param : AnimParams) {
        if (param.instance_id != -1) {
            ++parameter_count;
        }
    }

    EdFileSetMedia(1);
    if (!EdFileOpen(path, NUFILE_WRITE)) {
        return 0;
    }
    EdFileSetReadWrongEndianess(1);
    EdFileWriteInt(6);
    EdFileWriteInt(parameter_count);

    for (const auto &param : AnimParams) {
        if (param.instance_id == -1) {
            continue;
        }
        nuhspecial_s special;
        NuGScnGetSpecial(&special, edbits_base_scene, param.instance_id);
        char name[20];
        strncpy(name, NuSpecialGetName(&special), sizeof(name));
        name[sizeof(name) - 1] = '\0';
        EdFileWrite(name, sizeof(name));
        EdFileWriteInt(param.effect_count);
        EdFileWriteInt(param.sound_count);
        EdFileWriteInt(param.field_00c);
        EdFileWriteInt(param.field_010);
        EdFileWriteFloat(param.field_014);
        EdFileWriteFloat(param.field_018);

        for (i32 effect = 0; effect < param.effect_count; ++effect) {
            EdFileWrite(const_cast<char *>(param.effect_names[effect]), sizeof(param.effect_names[effect]));
            EdFileWriteInt(param.effect_intervals[effect]);
            EdFileWriteInt(param.effect_flags[effect]);
            EdFileWriteNuVec(reinterpret_cast<NUVEC *>(const_cast<f32 *>(param.effect_positions[effect])));
            EdFileWriteShort(param.effect_angles[effect]);
            EdFileWriteShort(param.effect_angle_ranges[effect]);
        }
        for (i32 sound = 0; sound < param.sound_count; ++sound) {
            EdFileWrite(const_cast<char *>(param.sound_names[sound]), sizeof(param.sound_names[sound]));
            EdFileWriteInt(param.sound_flags[sound]);
            EdFileWriteFloat(param.sound_values[sound]);
            EdFileWriteNuVec(reinterpret_cast<NUVEC *>(const_cast<f32 *>(param.sound_positions[sound])));
        }
        EdFileWriteFloat(param.bounce_impulse);
        EdFileWriteFloat(param.bounce_spring);
        EdFileWriteFloat(param.bounce_damping);
    }

    EdFileClose();
    EdFileSetReadWrongEndianess(0);
    return 1;
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
    const i32 rotation_z = edpp_copy_mode == 0 ? edpp_rotz : edpp_copyrotz;
    const i32 rotation_y = edpp_copy_mode == 0 ? edpp_roty : edpp_copyroty;
    NURND_VERTEX3D line[2];
    auto rotate = [&](NUVEC &vector) {
        NuVecRotateZ(&vector, &vector, rotation_z);
        NuVecRotateY(&vector, &vector, rotation_y);
    };
    auto draw_axis = [&](NUVEC vector) {
        rotate(vector);
        line[0].position = {edpp_cam_pos.x - vector.x, edpp_cam_pos.y - vector.y, edpp_cam_pos.z - vector.z};
        line[0].colour = 0xffffffff;
        line[1].position = edpp_cam_pos;
        line[1].colour = 0xffffffff;
        NuRndrLine3d(line, edpp_mtl, NULL);
        line[0].position = edpp_cam_pos;
        line[0].colour = 0xff00ff00;
        line[1].position = {edpp_cam_pos.x + vector.x, edpp_cam_pos.y + vector.y, edpp_cam_pos.z + vector.z};
        line[1].colour = 0xff00ff00;
        NuRndrLine3d(line, edpp_mtl, NULL);
    };
    draw_axis({0.5f, 0.0f, 0.0f});
    draw_axis({0.0f, 0.5f, 0.0f});
    draw_axis({0.0f, 0.0f, 0.5f});
    auto draw_mark = [&](NUVEC start, NUVEC end) {
        rotate(start);
        rotate(end);
        line[0].position = {edpp_cam_pos.x + start.x, edpp_cam_pos.y + start.y, edpp_cam_pos.z + start.z};
        line[1].position = {edpp_cam_pos.x + end.x, edpp_cam_pos.y + end.y, edpp_cam_pos.z + end.z};
        line[0].colour = line[1].colour = 0xff00ff00;
        NuRndrLine3d(line, edpp_mtl, NULL);
    };
    draw_mark({0.55f, 0.05f, 0.0f}, {0.6f, -0.05f, 0.0f});
    draw_mark({0.6f, 0.05f, 0.0f}, {0.55f, -0.05f, 0.0f});
    draw_mark({0.0f, 0.65f, -0.025f}, {0.0f, 0.6f, 0.0f});
    draw_mark({0.0f, 0.65f, 0.025f}, {0.0f, 0.6f, 0.0f});
    draw_mark({0.0f, 0.6f, 0.0f}, {0.0f, 0.55f, 0.0f});
    draw_mark({0.0f, 0.05f, 0.55f}, {0.0f, 0.05f, 0.6f});
    draw_mark({0.0f, 0.05f, 0.6f}, {0.0f, -0.05f, 0.55f});
    draw_mark({0.0f, -0.05f, 0.55f}, {0.0f, -0.05f, 0.6f});

    if (edpp_copy_mode == 0) {
        auto draw_direction = [&](NUVEC direction, u32 colour) {
            rotate(direction);
            line[0].position = edpp_cam_pos;
            line[1].position = {edpp_cam_pos.x + direction.x, edpp_cam_pos.y + direction.y,
                                edpp_cam_pos.z + direction.z};
            line[0].colour = line[1].colour = colour;
            NuRndrLine3d(line, edpp_mtl, NULL);
        };
        NUVEC emitter_tip = {0.0f, 0.375f, 0.0f};
        NuVecRotateZ(&emitter_tip, &emitter_tip, edpp_emitrotz);
        NuVecRotateY(&emitter_tip, &emitter_tip, edpp_emitroty);
        NuVecRotateX(&emitter_tip, &emitter_tip, edpp_emitrotx);
        draw_direction(emitter_tip, 0xff0000ff);
        NUVEC emitter_side = {0.0f, 0.375f, 0.125f};
        NuVecRotateZ(&emitter_side, &emitter_side, edpp_emitrotz);
        NuVecRotateY(&emitter_side, &emitter_side, edpp_emitroty);
        NuVecRotateX(&emitter_side, &emitter_side, edpp_emitrotx);
        rotate(emitter_side);
        line[0].position = {edpp_cam_pos.x + emitter_side.x, edpp_cam_pos.y + emitter_side.y,
                            edpp_cam_pos.z + emitter_side.z};
        line[0].colour = 0xff0000ff;
        NuRndrLine3d(line, edpp_mtl, NULL);
        if (edpp_dpad_mode == 3) {
            NUVEC reflection = {0.0f, 0.25f, 0.0f};
            NuVecRotateZ(&reflection, &reflection, edpp_refrotz);
            NuVecRotateY(&reflection, &reflection, edpp_refroty);
            draw_direction(reflection, 0xffff0000);
            if (edpp_nearest != -1) {
                reflection = {0.0f, edpp_ptls[edpp_nearest].reflection_offset, 0.0f};
                NuVecRotateZ(&reflection, &reflection, edpp_refrotz);
                NuVecRotateY(&reflection, &reflection, edpp_refroty);
                rotate(reflection);
                edbitsDrawCube(edpp_cam_pos.x + reflection.x, edpp_cam_pos.y + reflection.y,
                               edpp_cam_pos.z + reflection.z, 0.25f, 0.0f, 0.25f, edpp_refrotz, edpp_refroty, 0,
                               rotation_z, rotation_y, 0xffff0000, edpp_mtl);
            }
        }
        if (edpp_dpad_mode == 4) {
            NUVEC facing = {0.0f, 0.0f, 0.25f};
            NuVecRotateX(&facing, &facing, edpp_facrotx);
            NuVecRotateY(&facing, &facing, edpp_facroty);
            line[0].position = edpp_cam_pos;
            line[1].position = {edpp_cam_pos.x + facing.x, edpp_cam_pos.y + facing.y, edpp_cam_pos.z + facing.z};
            line[0].colour = line[1].colour = 0xffff0000;
            NuRndrLine3d(line, edpp_mtl, NULL);
            edbitsDrawBasicCube(edpp_cam_pos.x, edpp_cam_pos.y, edpp_cam_pos.z, 0.25f, 0.25f, 0.0f, edpp_facrotx,
                                edpp_facroty, 0, 0xffff0000, edpp_mtl);
        }
    } else {
        edbitsDrawCube(edpp_cam_pos.x, edpp_cam_pos.y, edpp_cam_pos.z, edpp_copy_size, edpp_copy_size, edpp_copy_size,
                       0, 0, 0, 0, 0, 0xffffffff, edpp_mtl);
    }

    NuRndrRect2di(0x1680, 0x9b0, 4000, 0x410, 0x80808080, edpp_boxmtl);
    NuRndrRect2di(0x1670, 0x8f0, 0xfc0, 0xc0, 0x80000000, edpp_boxmtl);
    NuRndrLine2di(0x1670, 0x9b0, 0x1670, 0xdc8, 0x80000000, edpp_boxmtl);
    NuRndrLine2di(0x2630, 0x9b0, 0x2630, 0xdc8, 0x80000000, edpp_boxmtl);
    NuRndrLine2di(0x1670, 0xdc8, 0x2630, 0xdc8, 0x80000000, edpp_boxmtl);
    NuQFntPushPrintMode(2);
    NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
    NuQFntSet(system_qfont);
    if (edpp_readout == 0) {
        if (globalframes % 200 < 101 || edpp_num_orphans < 1) {
            NuQFntSetColour(system_qfont, 0xe0e0e0e0);
            NuQFntPrintEx(system_qfont, 0x1720, 0x988, 0x10, "Info Box");
        } else {
            NuQFntSetColour(system_qfont, 0x80202080);
            NuQFntPrintEx(system_qfont, 0x1720, 0x988, 0x10, "WARNING: %d Orphan(s)", edpp_num_orphans);
        }
    } else if (edpp_readout == 1) {
        NuQFntSetColour(system_qfont, 0x80808080);
        NuQFntPrintEx(system_qfont, 0x1720, 0x988, 0x10, "Co-ordinates");
    }
    NuQFntSetColour(system_qfont, 0x80000000);
    if (edpp_readout == 1) {
        if (edpp_nearest == -1) {
            NuQFntPrintEx(system_qfont, 0x1720, 0xa50, 0x10, "Highlight: <none>");
        } else {
            edpp_particle_s &particle = edpp_ptls[edpp_nearest];
            debkeydatatype_s &key = debkeydata[particle.instance_id];
            NuQFntPrintEx(system_qfont, 0x1720, 0xa50, 0x10, "Highlight: %s", debtab[key.effect_index]->name);
            NuQFntPrintEx(system_qfont, 0x1720, 0xaf0, 0x10, "XYZ: %0.2f %0.2f %0.2f", key.position.x, key.position.y,
                          key.position.z);
            NuQFntPrintEx(system_qfont, 0x1720, 0xb90, 0x10, "RotZ: %d", particle.rotation_z);
            NuQFntPrintEx(system_qfont, 0x1720, 0xc30, 0x10, "RotY: %d", particle.rotation_y);
            NuQFntPrintEx(system_qfont, 0x1720, 0xcd0, 0x10, "EmitRotZ: %d", particle.emitter_rotation_z);
            NuQFntPrintEx(system_qfont, 0x1720, 0xd70, 0x10, "EmitRotY: %d", particle.emitter_rotation_y);
        }
    } else if (edpp_readout == 0) {
        if (edpp_copy_mode == 0) {
            if (edpp_effect_list == 1)
                NuQFntPrintEx(system_qfont, 0x1720, 0xa50, 0x10, "Current List: Level");
            else if (edpp_effect_list == 0)
                NuQFntPrintEx(system_qfont, 0x1720, 0xa50, 0x10, "Current List: General");
            else if (edpp_effect_list == 5)
                NuQFntPrintEx(system_qfont, 0x1720, 0xa50, 0x10, "Current List: Character");
            if (edpp_create_type == -1)
                NuQFntPrintEx(system_qfont, 0x1720, 0xaf0, 0x10, "Current Type: <none>");
            else
                NuQFntPrintEx(system_qfont, 0x1720, 0xaf0, 0x10, "Current Type: %s", debtab[edpp_create_type]->name);
            if (edptl_clipboard_entry == -1)
                NuQFntPrintEx(system_qfont, 0x1720, 0xb90, 0x10, "Clipboard: <none>");
            else
                NuQFntPrintEx(system_qfont, 0x1720, 0xb90, 0x10, "Clipboard: %s", debtab[edptl_clipboard_entry]->name);
            if (edpp_nearest == -1) {
                NuQFntPrintEx(system_qfont, 0x1720, 0xc30, 0x10, "Highlight: <none>");
            } else {
                edpp_particle_s &particle = edpp_ptls[edpp_nearest];
                debkeydatatype_s &key = debkeydata[particle.instance_id];
                debinftype *effect = debtab[key.effect_index];
                if (effect->generator_type == 0) {
                    edbitsDrawCube(edpp_cam_pos.x, edpp_cam_pos.y, edpp_cam_pos.z, effect->field_058, effect->field_05c,
                                   effect->field_060, edpp_emitrotz, edpp_emitroty, edpp_emitrotx, rotation_z,
                                   rotation_y, 0xff0000ff, edpp_mtl);
                    edppDrawSpheres(effect, particle.instance_id);
                }
                edppDrawTorus(effect, particle.instance_id);
                NuQFntSet(system_qfont);
                NuQFntPrintEx(system_qfont, 0x1720, 0xc30, 0x10, "Highlight: %s", effect->name);
                NuQFntPrintEx(system_qfont, 0x1720, 0xcd0, 0x10, "Particles:");
                const i32 group = effect->particle_type == 7 ? 12 : 32;
                const i32 limit = effect->particle_type == 7 ? 384 : 1024;
                const i32 count = effect->max_particles;
                if (count > limit)
                    NuQFntSetColour(system_qfont, 0x80000080);
                const i32 rounded = ((count - 1) / group + 1) * group;
                NuQFntPrintEx(system_qfont, 0x1cc0, 0xcd0, 0x10, "%d (%d)", count, rounded);
                NuQFntSetColour(system_qfont, 0x80000000);
            }
        } else {
            if (edpp_copy_source_count == 0)
                NuQFntPrintEx(system_qfont, 0x1720, 0xb90, 0x10, "Clipboard: <none>");
            else
                NuQFntPrintEx(system_qfont, 0x1720, 0xb90, 0x10, "Clipboard: %d items", edpp_copy_source_count);
            if (edpp_copy_enclosed > 8)
                NuQFntSetColour(system_qfont, 0x80000080);
            NuQFntPrintEx(system_qfont, 0x1720, 0xc30, 0x10, "Enclosed: %d", edpp_copy_enclosed);
            NuQFntSetColour(system_qfont, 0x80000000);
        }
        const char *mode = edpp_copy_mode ? "Multiple Copy Mode" : nullptr;
        if (edpp_copy_mode == 0) {
            switch (edpp_dpad_mode) {
                case 0:
                    mode = "Dpad Mode: Emit Rotate";
                    break;
                case 1:
                    mode = "Dpad Mode: Grav Rotate";
                    break;
                case 2:
                    NuQFntPrintEx(system_qfont, 0x1720, 0xd70, 0x10, "Dpad Mode: Offset (%0.2f)", edpp_offset);
                    break;
                case 3:
                    mode = "Dpad Mode: Reflections";
                    break;
                case 4:
                    mode = "Dpad Mode: Facing";
                    break;
            }
        }
        if (mode)
            NuQFntPrintEx(system_qfont, 0x1720, 0xd70, 0x10, mode);
    }
    NuQFntPopPrintMode();
    NuQFntPopCoordinateSystem();
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

void EdDrawPolyArrow(VuVec const &start, VuVec const &end, i32 sides, i32 colour, float radius, float limit,
                     float radius_factor, float radius_offset) {
    VuVec direction(end.x - start.x, end.y - start.y, end.z - start.z, 0.0f);
    const float length = NuVecMag(&direction.xyz);
    if (length <= 0.0f)
        return;
    const float inverse_length = 1.0f / length;
    direction.x *= inverse_length;
    direction.y *= inverse_length;
    direction.z *= inverse_length;
    const float half_length = length * 0.4f;
    const float minimum_radius = radius_factor * half_length + radius_offset;
    if (radius < minimum_radius)
        radius = minimum_radius;
    if (limit > radius)
        limit = radius;

    NUANGVEC angles{};
    if (direction.x == 0.0f && direction.z == 0.0f) {
        angles.x = -0x4000;
    } else {
        angles.y = NuAtan2D(direction.x, direction.z);
        NuVecRotateY(&direction.xyz, &direction.xyz, -angles.y);
        angles.x = -NuAtan2D(direction.y, direction.z);
    }

    NUMTX transform;
    NuMtxSetRotateXYZVU0(&transform, &angles);
    NUVEC center{start.x + (end.x - start.x) * 0.4f, start.y + (end.y - start.y) * 0.4f,
                 start.z + (end.z - start.z) * 0.4f};
    NuMtxTranslate(&transform, &center);
    EdDrawPolyCylinder(*reinterpret_cast<VuMtx *>(&transform), half_length, limit, limit, sides, colour, 1, 0);
    NuMtxTranslateNeg(&transform, &center);
    center.x = end.x - (end.x - start.x) * 0.1f;
    center.y = end.y - (end.y - start.y) * 0.1f;
    center.z = end.z - (end.z - start.z) * 0.1f;
    NuMtxTranslate(&transform, &center);
    EdDrawPolyCylinder(*reinterpret_cast<VuMtx *>(&transform), half_length * 0.25f, limit * 1.6f, 0.0f, sides, colour,
                       1, 0);
}

void edbriDrawCursor() {
    NURND_VERTEX3D line[2] = {};
    line[0].colour = line[1].colour = 0xffffffff;
    line[0].position = line[1].position = edbri_cam_pos;
    line[0].position.x -= 0.5f;
    line[1].position.x += 0.5f;
    NuRndrLine3d(line, edbri_mtl, NULL);
    line[0].position = line[1].position = edbri_cam_pos;
    line[0].position.y -= 0.5f;
    line[1].position.y += 0.5f;
    NuRndrLine3d(line, edbri_mtl, NULL);
    line[0].position = line[1].position = edbri_cam_pos;
    line[0].position.z -= 0.5f;
    line[1].position.z += 0.5f;
    NuRndrLine3d(line, edbri_mtl, NULL);

    NUVEC extent = {edbri_length, 0.0f, 0.0f};
    NuVecRotateZ(&extent, &extent, edbri_rotz);
    NuVecRotateY(&extent, &extent, edbri_roty);
    line[0].position = edbri_cam_pos;
    NuVecAdd(&line[1].position, &edbri_cam_pos, &extent);
    line[0].colour = line[1].colour = 0xff0000ff;
    NuRndrLine3d(line, edbri_mtl, NULL);
    extent.x = extent.y = 0.0f;
    extent.z = edbri_width;
    NuVecRotateZ(&extent, &extent, edbri_rotz);
    NuVecRotateY(&extent, &extent, edbri_roty);
    NuVecSub(&line[0].position, &edbri_cam_pos, &extent);
    NuVecAdd(&line[1].position, &edbri_cam_pos, &extent);
    NuRndrLine3d(line, edbri_mtl, NULL);

    NuRndrRect2di(0x1720, 0x9b0, 0xdc0, 0x410, 0x80808080, edbri_mtl_zoff);
    NuRndrRect2di(0x1710, 0x8f0, 0xde0, 0xc0, 0x80000000, edbri_mtl_zoff);
    NuRndrLine2di(0x1710, 0x9b0, 0x1710, 0xdc8, 0x80000000, edbri_mtl_zoff);
    NuRndrLine2di(0x24f0, 0x9b0, 0x24f0, 0xdc8, 0x80000000, edbri_mtl_zoff);
    NuRndrLine2di(0x1710, 0xdc8, 0x24f0, 0xdc8, 0x80000000, edbri_mtl_zoff);
    NuQFntPushPrintMode(2);
    NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
    NuQFntSet(system_qfont);
    NuQFntSetColour(system_qfont, 0xe0e0e0e0);
    NuQFntPrintEx(system_qfont, 0x17c0, 0x988, 0x10, "Info Box");
    NuQFntSetColour(system_qfont, 0x80000000);
    if (edbri_plank_instance_type == -1) {
        NuQFntPrintEx(system_qfont, 0x17c0, 0xa50, 0x10, "Plank Inst: <none>");
    } else {
        nuhspecial_s special;
        NuGScnGetSpecial(&special, edbits_base_scene, edbri_plank_instance_type);
        NuQFntPrintEx(system_qfont, 0x17c0, 0xa50, 0x10, "Plank Inst: %s", NuSpecialGetName(&special));
    }
    if (edbri_post_instance_type == -1) {
        NuQFntPrintEx(system_qfont, 0x17c0, 0xaf0, 0x10, "Post Inst: <none>");
    } else {
        nuhspecial_s special;
        NuGScnGetSpecial(&special, edbits_base_scene, edbri_post_instance_type);
        NuQFntPrintEx(system_qfont, 0x17c0, 0xaf0, 0x10, "Post Inst: %s", NuSpecialGetName(&special));
    }
    NuQFntPrintEx(system_qfont, 0x17c0, 0xb90, 0x10, "Planks (Interval): %d (%d)", edbri_planks, edbri_post_interval);
    NuQFntPrintEx(system_qfont, 0x17c0, 0xc30, 0x10, "Used: %d/%d", edbri_bridges_used, 64);
    NuQFntPrintEx(system_qfont, 0x17c0, 0xd70, 0x10, "%5.2f", edbri_cam_pos.x);
    NuQFntPrintEx(system_qfont, 0x1c20, 0xd70, 0x10, "%5.2f", edbri_cam_pos.y);
    NuQFntPrintEx(system_qfont, 0x2080, 0xd70, 0x10, "%5.2f", edbri_cam_pos.z);
    NuQFntPopPrintMode();
    NuQFntPopCoordinateSystem();
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
    NURND_VERTEX3D line[2];
    line[0].colour = line[1].colour = 0xffffffff;
    line[0].position.x = edgra_cam_pos.x - 0.5f;
    line[1].position.x = edgra_cam_pos.x + 0.5f;
    line[0].position.y = line[1].position.y = edgra_cam_pos.y;
    line[0].position.z = line[1].position.z = edgra_cam_pos.z;
    NuRndrLine3d(line, edgra_mtl, NULL);
    line[0].position.x = line[1].position.x = edgra_cam_pos.x;
    line[0].position.y = edgra_cam_pos.y - 0.5f;
    line[1].position.y = edgra_cam_pos.y + 0.5f;
    line[0].position.z = line[1].position.z = edgra_cam_pos.z;
    line[0].colour = line[1].colour = 0xffffffff;
    NuRndrLine3d(line, edgra_mtl, NULL);
    line[0].position.x = line[1].position.x = edgra_cam_pos.x;
    line[0].position.y = line[1].position.y = edgra_cam_pos.y;
    line[0].position.z = edgra_cam_pos.z - 0.5f;
    line[1].position.z = edgra_cam_pos.z + 0.5f;
    line[0].colour = line[1].colour = 0xffffffff;
    NuRndrLine3d(line, edgra_mtl, NULL);
    NUVEC arrow = {0.0f, 0.5f, 0.0f};
    NuVecRotateZ(&arrow, &arrow, edgra_rotz);
    NuVecRotateY(&arrow, &arrow, edgra_roty);
    line[0].position = edgra_cam_pos;
    line[1].position.x = edgra_cam_pos.x + arrow.x;
    line[1].position.y = edgra_cam_pos.y + arrow.y;
    line[1].position.z = edgra_cam_pos.z + arrow.z;
    line[0].colour = line[1].colour = 0xff0000ff;
    NuRndrLine3d(line, edgra_mtl, NULL);

    if (edgra_nearest != -1 && GrassClumps[edgra_nearest].kind == 3) {
        edbitsDrawCircleTilted(&edgra_cam_pos, 0.5f, 0xff0000ff, edgra_mtl, edgra_rotz, edgra_roty);
    } else if (edgra_nearest != -1 && static_cast<u8>(GrassClumps[edgra_nearest].unknown_25 - 3) < 2) {
        edbitsDrawCube(edgra_cam_pos.x, edgra_cam_pos.y, edgra_cam_pos.z, edgra_size, 0.0f, edgra_size, 0, 0, 0,
                       edgra_rotz, edgra_roty, 0xff0000ff, edgra_mtl);
    } else {
        edbitsDrawCircleTilted(&edgra_cam_pos, edgra_size, 0xff0000ff, edgra_mtl, edgra_rotz, edgra_roty);
    }
    NuRndrRect2di(0x1720, 0x9b0, 0xdc0, 0x410, 0x80808080, edgra_mtl_zoff);
    NuRndrRect2di(0x1710, 0x8f0, 0xde0, 0xc0, 0x80000000, edgra_mtl_zoff);
    NuRndrLine2di(0x1710, 0x9b0, 0x1710, 0xdc8, 0x80000000, edgra_mtl_zoff);
    NuRndrLine2di(0x24f0, 0x9b0, 0x24f0, 0xdc8, 0x80000000, edgra_mtl_zoff);
    NuRndrLine2di(0x1710, 0xdc8, 0x24f0, 0xdc8, 0x80000000, edgra_mtl_zoff);
    NuQFntPushPrintMode(2);
    NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
    NuQFntSet(system_qfont);
    NuQFntSetColour(system_qfont, 0xe0e0e0e0);
    NuQFntPrintEx(system_qfont, 0x17c0, 0x988, 0x10, "Info Box");
    NuQFntSetColour(system_qfont, 0x80000000);
    if (edgra_copy_source != -1) {
        NuQFntPrintEx(system_qfont, 0x17c0, 0xa50, 0x10, "Copy Clump Mode");
    } else if (edgra_instance_type == -1) {
        NuQFntPrintEx(system_qfont, 0x17c0, 0xa50, 0x10, "Curr Inst: <none>");
    } else {
        nuhspecial_s special;
        NuGScnGetSpecial(&special, edbits_base_scene, edgra_instance_type);
        NuQFntPrintEx(system_qfont, 0x17c0, 0xa50, 0x10, "Curr Inst: %s", NuSpecialGetName(&special));
    }
    if (edgra_mode == 3) {
        if (edgra_nearest == -1) {
            NuQFntPrintEx(system_qfont, 0x17c0, 0xb90, 0x10, "Clump Size: -");
        } else {
            NuQFntPrintEx(system_qfont, 0x17c0, 0xb90, 0x10, "Clump Size: %d/%d",
                          GrassClumps[edgra_nearest].element_count, EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP);
        }
    } else {
        NuQFntPrintEx(system_qfont, 0x17c0, 0xaf0, 0x10, "Base Size: %0.2f", edgra_size);
        NuQFntPrintEx(system_qfont, 0x17c0, 0xb90, 0x10, "Clump Size: %d", edgra_clump_size);
    }
    NuQFntPrintEx(system_qfont, 0x17c0, 0xc30, 0x10, "Used: %d/%d,%d/%d", edgra_units_used, 0x3000, edgra_clumps_used,
                  EDGRA_MAX_CLUMPS);
    if (edgra_dpadmode == 0)
        NuQFntPrintEx(system_qfont, 0x17c0, 0xcd0, 0x10, "Dpad Mode: Size");
    else if (edgra_dpadmode == 1)
        NuQFntPrintEx(system_qfont, 0x17c0, 0xcd0, 0x10, "Dpad Mode: Tilt");
    NuQFntPrintEx(system_qfont, 0x17c0, 0xd70, 0x10, "%5.2f", edgra_cam_pos.x);
    NuQFntPrintEx(system_qfont, 0x1c20, 0xd70, 0x10, "%5.2f", edgra_cam_pos.y);
    NuQFntPrintEx(system_qfont, 0x2080, 0xd70, 0x10, "%5.2f", edgra_cam_pos.z);
    NuQFntPopPrintMode();
    NuQFntPopCoordinateSystem();
}

void edpartPtlShelve(i32 index) {
    if (part_emits[index].instance_id != -1 && part_emits[index].instance_id != 99999) {
        DebFreeInstantly(&part_emits[index].instance_id);
        part_emits[index].instance_id = 99999;
    }
}

void edpartScaleType(i32 index, float scale) {
    part_type_s *type = &part_types[index];
    type->particle_scale *= scale;
    type->effect_scale *= scale;
    type->speed *= scale;
    type->gravity *= scale;
    type->position_random.x *= scale;
    type->position_random.y *= scale;
    type->position_random.z *= scale;
    type->velocity_random.x *= scale;
    type->velocity_random.y *= scale;
    type->velocity_random.z *= scale;
    type->maximum_distance *= scale;
}

i32 edppSaveEffects(char *filename, char page) {
    const u8 category = page == 6 ? 1 : static_cast<u8>(page);
    i32 effect_count = 0;
    for (i32 index = 1; index < EDPP_MAX_TYPES; ++index) {
        if (debtab[index] == NULL)
            continue;
        const debinftype &effect = effecttypes[index];
        if (category == 2 ||
            (category == 1 && effect.category == 1 && static_cast<i8>(effect.page) == edbits_particle_level_page) ||
            (category != 1 && effect.category == category))
            ++effect_count;
    }

    EdFileSetMedia(edpp_usememcard == 0 ? 1 : 2);
    if (EdFileOpen(filename, NUFILE_WRITE) == 0)
        return 0;
    EdFileSetReadWrongEndianess(1);
    EdFileWriteInt(0x29);
    EdFileWriteInt(effect_count);

    for (i32 index = 1; index < EDPP_MAX_TYPES; ++index) {
        if (debtab[index] == NULL)
            continue;
        debinftype *effect = &effecttypes[index];
        if (category != 2 &&
            !(category == 1 && effect->category == 1 && static_cast<i8>(effect->page) == edbits_particle_level_page) &&
            !(category != 1 && effect->category == category))
            continue;

        u8 *bytes = reinterpret_cast<u8 *>(effect);
#define WRITE_FLOAT_AT(offset) EdFileWriteFloat(*reinterpret_cast<f32 *>(bytes + (offset)))
        EdFileWrite(effect->name, 16);
        EdFileWriteShort(effect->frequency);
        EdFileWriteShort(effect->max_particles);
        for (i32 offset = 0x18; offset <= 0x28; offset += 4)
            WRITE_FLOAT_AT(offset);
        EdFileWriteChar(effect->generator_type);
        EdFileWriteChar(effect->momentum_adjustment_type);
        EdFileWriteChar(effect->cutscene_only);
        EdFileWriteChar(effect->particle_type);
        EdFileWriteChar(effect->camera_facing);
        for (i32 offset = 0x30; offset <= 0x48; offset += 4)
            WRITE_FLOAT_AT(offset);
        EdFileWriteNuVec(reinterpret_cast<NUVEC *>(bytes + 0x4c));
        EdFileWriteNuVec(reinterpret_cast<NUVEC *>(bytes + 0x58));
        EdFileWriteNuVec(reinterpret_cast<NUVEC *>(bytes + 0x64));
        for (i32 offset = 0x70; offset <= 0xa4; offset += 4)
            WRITE_FLOAT_AT(offset);
        EdFileWriteShort(effect->field_0a8);
        EdFileWriteChar(effect->field_0aa);
        EdFileWriteChar(effect->field_0ab);
        for (i32 offset = 0xac; offset <= 0xbc; offset += 4)
            WRITE_FLOAT_AT(offset);
        for (i32 key = 0; key < 8; ++key) {
            EdFileWriteFloat(effect->colour_keys[key].time);
            EdFileWriteUnsignedChar(effect->colour_keys[key].red);
            EdFileWriteUnsignedChar(effect->colour_keys[key].green);
            EdFileWriteUnsignedChar(effect->colour_keys[key].blue);
            EdFileWriteUnsignedChar(effect->colour_keys[key].alpha);
        }
        for (i32 offset = 0x100; offset <= 0x2a4; offset += 4)
            WRITE_FLOAT_AT(offset);
        for (i32 offset = 0x2b0; offset <= 0x2ec; offset += 4)
            WRITE_FLOAT_AT(offset);
        EdFileWriteChar(effect->process_spheres);
        EdFileWriteChar(effect->time_group);
        EdFileWriteChar(effect->field_2f2);
        EdFileWriteChar(effect->use_explicit_clip_box);
        EdFileWriteNuVec(&effect->repeat_box);
        EdFileWriteFloat(effect->thinning);
        for (i32 offset = 0x304; offset <= 0x3cc; offset += 4)
            WRITE_FLOAT_AT(offset);
#undef WRITE_FLOAT_AT

        i32 sound_count = 0;
        for (i32 sound = 0; sound < 4; ++sound)
            sound_count += effect->sound_data[sound * 3] != -1;
        EdFileWriteInt(sound_count);
        for (i32 sound = 0; sound < 4; ++sound) {
            const i32 id = effect->sound_data[sound * 3];
            if (id == -1)
                continue;
            EdFileWrite(const_cast<char *>(g_soundInfo[id].sfx_name), 16);
            EdFileWriteInt(effect->sound_data[sound * 3 + 1]);
            EdFileWriteInt(effect->sound_data[sound * 3 + 2]);
        }
        EdFileWriteChar(effect->trail_count);
        EdFileWriteFloat(effect->trail_time);
        EdFileWriteChar(effect->radial_segments);
        EdFileWriteFloat(effect->radial_floor);
        EdFileWriteFloat(effect->scale_in_time);
    }

    if (page == 1 || page == 2) {
        i32 instance_count = 0;
        for (i32 index = 0; index < 512; ++index)
            instance_count += edpp_ptls[index].instance_id != -1;
        EdFileWriteInt(instance_count);
        for (i32 index = 0; index < 512; ++index) {
            edpp_particle_s *particle = &edpp_ptls[index];
            if (particle->instance_id == -1)
                continue;
            EdFileWriteNuVec(&particle->position);
            EdFileWriteShort(particle->rotation_z);
            EdFileWriteShort(particle->rotation_y);
            EdFileWriteShort(particle->emitter_rotation_z);
            EdFileWriteShort(particle->emitter_rotation_y);
            EdFileWriteShort(particle->emitter_rotation_x);
            EdFileWriteFloat(particle->start_offset);
            EdFileWrite(particle->effect_index == -1 ? particle->name : debtab[particle->effect_index]->name, 16);
            EdFileWriteInt(particle->switch_type);
            EdFileWriteInt(particle->switch_id);
            EdFileWriteFloat(particle->switch_variable);
            EdFileWriteShort(particle->reflection_rotation_z);
            EdFileWriteShort(particle->reflection_rotation_y);
            EdFileWriteFloat(particle->reflection_offset);
            EdFileWriteFloat(particle->reflection_bounce);
            EdFileWriteShort(particle->render_group);
            EdFileWriteUnsignedShort(particle->render_priority);
            EdFileWriteChar(particle->dynamic_priority);
            EdFileWriteChar(particle->detail_levels);
            EdFileWriteChar(particle->facing_mode);
            EdFileWriteShort(particle->facing_rotation_x);
            EdFileWriteShort(particle->facing_rotation_y);
        }
    }
    EdFileSetReadWrongEndianess(0);
    EdFileClose();
    return 1;
}

void EdDrawLineSphere(VuVec const &center, float radius, float scale, i32 colour) {
    const float radius_squared = radius * radius;
    for (i32 latitude = 0; latitude < 8; ++latitude) {
        float first_radius;
        float second_radius;
        if (latitude < 4) {
            first_radius = radius * NuTrigTable[latitude * 0x800];
            second_radius = radius * NuTrigTable[(latitude + 1) * 0x800];
        } else {
            first_radius = radius * NuTrigTable[(8 - latitude) * 0x800];
            second_radius = radius * NuTrigTable[(7 - latitude) * 0x800];
        }
        float first_height = NuFsqrt(radius_squared - first_radius * first_radius) * scale;
        float second_height = NuFsqrt(radius_squared - second_radius * second_radius) * scale;
        if (latitude >= 5)
            first_height = -first_height;
        if (latitude >= 4)
            second_height = -second_height;
#define ED_DRAW_SPHERE_LONGITUDE(angle, next_angle)                                                                    \
    {                                                                                                                  \
        const VuVec first(center.x + first_radius * NU_COS_LUT(angle), center.y + first_height,                        \
                          center.z + first_radius * NU_SIN_LUT(angle), 1.0f);                                          \
        const VuVec second(center.x + second_radius * NU_COS_LUT(angle), center.y + second_height,                     \
                           center.z + second_radius * NU_SIN_LUT(angle), 1.0f);                                        \
        EdDrawLineSegment(first, second, colour);                                                                      \
        if (latitude != 0) {                                                                                           \
            const VuVec next(center.x + first_radius * NU_COS_LUT(next_angle), center.y + first_height,                \
                             center.z + first_radius * NU_SIN_LUT(next_angle), 1.0f);                                  \
            EdDrawLineSegment(first, next, colour);                                                                    \
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
#undef ED_DRAW_SPHERE_LONGITUDE
    }
}

void EdDrawPolySector(VuVec const &center, float radius, i32 axis, i32 first_angle, i32 last_angle, i32 colour,
                      i32 segments) {
    const i32 step = segments != 0 ? 0x10000 / segments : 0x1000;
    i32 angle = first_angle < last_angle ? first_angle : last_angle;
    i32 remaining = (first_angle < last_angle ? last_angle - first_angle : first_angle - last_angle) & 0xffff;
    if (remaining == 0 || remaining >= 0x8000)
        return;
    do {
        const i32 delta = remaining < step ? remaining : step;
        VuVec first(0.0f, 0.0f, 0.0f, 1.0f);
        VuVec second(0.0f, 0.0f, 0.0f, 1.0f);
        if (axis == 0) {
            first.z = radius;
            second.z = radius;
            NuVecRotateX(&first.xyz, &first.xyz, -angle);
            NuVecRotateX(&second.xyz, &second.xyz, -angle - delta);
        } else if (axis == 1) {
            first.z = -radius;
            second.z = -radius;
            NuVecRotateY(&first.xyz, &first.xyz, -angle);
            NuVecRotateY(&second.xyz, &second.xyz, -angle - delta);
        } else if (axis == 2) {
            first.y = radius;
            second.y = radius;
            NuVecRotateZ(&first.xyz, &first.xyz, -angle);
            NuVecRotateZ(&second.xyz, &second.xyz, -angle - delta);
        }
        first.x += center.x;
        first.y += center.y;
        first.z += center.z;
        second.x += center.x;
        second.y += center.y;
        second.z += center.z;
        EdDrawPolyTri(center, first, second, colour);
        angle += delta;
        remaining -= delta;
    } while (remaining > 0);
}

void edanimDrawCursor() {
    NURND_VERTEX3D line[2] = {};
    line[0].colour = line[1].colour = 0xffffffff;
    line[0].position = line[1].position = edanim_cam_pos;
    line[0].position.x -= 0.5f;
    line[1].position.x += 0.5f;
    NuRndrLine3d(line, edanim_mtl, NULL);
    line[0].position = line[1].position = edanim_cam_pos;
    line[0].position.y -= 0.5f;
    line[1].position.y += 0.5f;
    NuRndrLine3d(line, edanim_mtl, NULL);
    line[0].position = line[1].position = edanim_cam_pos;
    line[0].position.z -= 0.5f;
    line[1].position.z += 0.5f;
    NuRndrLine3d(line, edanim_mtl, NULL);

    if (edanim_particle_mode != 0) {
        NUVEC direction{0.0f, 0.375f, 0.0f};
        NuVecRotateZ(&direction, &direction, edanim_emitrotz);
        NuVecRotateY(&direction, &direction, edanim_emitroty);
        line[0].position = edanim_cam_pos;
        NuVecAdd(&line[1].position, &edanim_cam_pos, &direction);
        line[0].colour = line[1].colour = 0xff0000ff;
        NuRndrLine3d(line, edanim_mtl, NULL);
    }

    NuRndrRect2di(0x1720, 0xa50, 0xdc0, 0x370, 0x80808080, edanim_mtl_zoff);
    NuRndrRect2di(0x1710, 0x990, 0xde0, 0xc0, 0x80000000, edanim_mtl_zoff);
    NuRndrLine2di(0x1710, 0xa50, 0x1710, 0xdc8, 0x80000000, edanim_mtl_zoff);
    NuRndrLine2di(0x24f0, 0xa50, 0x24f0, 0xdc8, 0x80000000, edanim_mtl_zoff);
    NuRndrLine2di(0x1710, 0xdc8, 0x24f0, 0xdc8, 0x80000000, edanim_mtl_zoff);
    NuQFntPushPrintMode(2);
    NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
    NuQFntSet(system_qfont);
    NuQFntSetColour(system_qfont, 0xe0e0e0e0);
    NuQFntPrintEx(system_qfont, 0x17c0, 0xa28, 0x10, "Info Box");
    NuQFntSetColour(system_qfont, 0x80000000);
    if (edanim_nearest == -1) {
        NuQFntPrintEx(system_qfont, 0x17c0, 0xaf0, 0x10, "Curr Spcl: <none>");
    } else {
        nuhspecial_s special;
        NuGScnGetSpecial(&special, edbits_base_scene, edanim_nearest);
        NuQFntPrintEx(system_qfont, 0x17c0, 0xaf0, 0x10, "Curr Spcl: %s", NuSpecialGetName(&special));
    }
    NuQFntPrintEx(system_qfont, 0x17c0, 0xb90, 0x10, edanim_nearest_param_id == -1 ? "Params: No" : "Params: Yes");
    if (edanim_nearest_param_id != -1) {
        auto &param = AnimParams[edanim_nearest_param_id];
        if (edanim_particle_mode != 0) {
            NuQFntPrintEx(system_qfont, 0x1810, 0xc30, 0x10, "Particles: %d (Max %d)", param.effect_count, 8);
            if (edanim_particle_type != -1) {
                NuQFntPrintEx(system_qfont, 0x1810, 0xcd0, 0x10, "Select Type: %s", debtab[edanim_particle_type]->name);
            } else {
                NuQFntPrintEx(system_qfont, 0x1810, 0xcd0, 0x10, "Select Type: <none>");
            }
        } else if (edanim_sound_mode != 0) {
            NuQFntPrintEx(system_qfont, 0x1810, 0xc30, 0x10, "Sounds: %d (Max %d)", param.sound_count, 8);
            if (edanim_sound_type != -1) {
                NuQFntPrintEx(system_qfont, 0x1810, 0xcd0, 0x10, "Select Type: %s",
                              edbitsGetSoundName(edanim_sound_type));
            } else {
                NuQFntPrintEx(system_qfont, 0x1810, 0xcd0, 0x10, "Select Type: <none>");
            }
        }
    }
    NuQFntPrintEx(system_qfont, 0x17c0, 0xd70, 0x10, "%5.2f", edanim_cam_pos.x);
    NuQFntPrintEx(system_qfont, 0x1c20, 0xd70, 0x10, "%5.2f", edanim_cam_pos.y);
    NuQFntPrintEx(system_qfont, 0x2080, 0xd70, 0x10, "%5.2f", edanim_cam_pos.z);
    NuQFntPopPrintMode();
    NuQFntPopCoordinateSystem();
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
    extern i32 edpart_copy_mode, edpart_copyrotz, edpart_copyroty, edpart_rotz, edpart_roty;
    extern i32 edpart_emitrotz, edpart_emitroty, edpart_emitrotx, edpart_refrotz, edpart_refroty;
    extern i32 edpart_dpad_mode, edpart_nearest, edpart_readout, edpart_num_orphans;
    extern i32 edpart_create_type, edpart_nearest_orphans, edpart_nearest_duplicates, edpart_copy_enclosed;
    extern i32 part_types_used, part_emits_used;
    extern i8 edpart_effect_list;
    extern f32 edpart_copy_size;
    extern NUVEC edpart_cam_pos;
    extern NUMTL *edpart_mtl, *edpart_boxmtl;
    extern part_emit_s *edpart_nearest_emit;
    extern part_typedesc_s *edpart_nearest_type;

    const i32 rotation_z = edpart_copy_mode == 0 ? edpart_rotz : edpart_copyrotz;
    const i32 rotation_y = edpart_copy_mode == 0 ? edpart_roty : edpart_copyroty;
    NURND_VERTEX3D line[2];
    auto rotate = [&](NUVEC &vector) {
        NuVecRotateZ(&vector, &vector, rotation_z);
        NuVecRotateY(&vector, &vector, rotation_y);
    };
    auto draw_axis = [&](NUVEC vector) {
        rotate(vector);
        line[0].position.x = edpart_cam_pos.x - vector.x;
        line[0].position.y = edpart_cam_pos.y - vector.y;
        line[0].position.z = edpart_cam_pos.z - vector.z;
        line[0].colour = 0xffffffff;
        line[1].position = edpart_cam_pos;
        line[1].colour = 0xffffffff;
        NuRndrLine3d(line, edpart_mtl, NULL);
        line[0].position = edpart_cam_pos;
        line[0].colour = 0xff00ff00;
        line[1].position.x = edpart_cam_pos.x + vector.x;
        line[1].position.y = edpart_cam_pos.y + vector.y;
        line[1].position.z = edpart_cam_pos.z + vector.z;
        line[1].colour = 0xff00ff00;
        NuRndrLine3d(line, edpart_mtl, NULL);
    };
    draw_axis({0.5f, 0.0f, 0.0f});
    draw_axis({0.0f, 0.5f, 0.0f});
    draw_axis({0.0f, 0.0f, 0.5f});
    auto draw_mark = [&](NUVEC start, NUVEC end) {
        rotate(start);
        rotate(end);
        line[0].position.x = edpart_cam_pos.x + start.x;
        line[0].position.y = edpart_cam_pos.y + start.y;
        line[0].position.z = edpart_cam_pos.z + start.z;
        line[1].position.x = edpart_cam_pos.x + end.x;
        line[1].position.y = edpart_cam_pos.y + end.y;
        line[1].position.z = edpart_cam_pos.z + end.z;
        line[0].colour = line[1].colour = 0xff00ff00;
        NuRndrLine3d(line, edpart_mtl, NULL);
    };
    draw_mark({0.55f, 0.05f, 0.0f}, {0.6f, -0.05f, 0.0f});
    draw_mark({0.6f, 0.05f, 0.0f}, {0.55f, -0.05f, 0.0f});
    draw_mark({0.0f, 0.65f, -0.025f}, {0.0f, 0.6f, 0.0f});
    draw_mark({0.0f, 0.65f, 0.025f}, {0.0f, 0.6f, 0.0f});
    draw_mark({0.0f, 0.6f, 0.0f}, {0.0f, 0.55f, 0.0f});
    draw_mark({0.0f, 0.05f, 0.55f}, {0.0f, 0.05f, 0.6f});
    draw_mark({0.0f, 0.05f, 0.6f}, {0.0f, -0.05f, 0.55f});
    draw_mark({0.0f, -0.05f, 0.55f}, {0.0f, -0.05f, 0.6f});

    if (edpart_copy_mode == 0) {
        NUVEC emitter_tip = {0.0f, 0.375f, 0.0f};
        NuVecRotateZ(&emitter_tip, &emitter_tip, edpart_emitrotz);
        NuVecRotateY(&emitter_tip, &emitter_tip, edpart_emitroty);
        NuVecRotateX(&emitter_tip, &emitter_tip, edpart_emitrotx);
        rotate(emitter_tip);
        line[0].position = edpart_cam_pos;
        line[1].position.x = edpart_cam_pos.x + emitter_tip.x;
        line[1].position.y = edpart_cam_pos.y + emitter_tip.y;
        line[1].position.z = edpart_cam_pos.z + emitter_tip.z;
        line[0].colour = line[1].colour = 0xff0000ff;
        NuRndrLine3d(line, edpart_mtl, NULL);
        NUVEC emitter_side = {0.0f, 0.375f, 0.125f};
        NuVecRotateZ(&emitter_side, &emitter_side, edpart_emitrotz);
        NuVecRotateY(&emitter_side, &emitter_side, edpart_emitroty);
        NuVecRotateX(&emitter_side, &emitter_side, edpart_emitrotx);
        rotate(emitter_side);
        line[0].position.x = edpart_cam_pos.x + emitter_side.x;
        line[0].position.y = edpart_cam_pos.y + emitter_side.y;
        line[0].position.z = edpart_cam_pos.z + emitter_side.z;
        line[0].colour = 0xff0000ff;
        NuRndrLine3d(line, edpart_mtl, NULL);
        if (edpart_dpad_mode == 3) {
            NUVEC reflection = {0.0f, 0.25f, 0.0f};
            NuVecRotateZ(&reflection, &reflection, edpart_refrotz);
            NuVecRotateY(&reflection, &reflection, edpart_refroty);
            rotate(reflection);
            line[0].position = edpart_cam_pos;
            line[1].position.x = edpart_cam_pos.x + reflection.x;
            line[1].position.y = edpart_cam_pos.y + reflection.y;
            line[1].position.z = edpart_cam_pos.z + reflection.z;
            line[0].colour = line[1].colour = 0xffff0000;
            NuRndrLine3d(line, edpart_mtl, NULL);
            if (edpart_nearest != -1) {
                reflection.x = 0.0f;
                reflection.y = *reinterpret_cast<f32 *>(&part_emits[edpart_nearest].trailing_state_words[4]);
                reflection.z = 0.0f;
                NuVecRotateZ(&reflection, &reflection, edpart_refrotz);
                NuVecRotateY(&reflection, &reflection, edpart_refroty);
                rotate(reflection);
                edbitsDrawCube(edpart_cam_pos.x + reflection.x, edpart_cam_pos.y + reflection.y,
                               edpart_cam_pos.z + reflection.z, 0.25f, 0.0f, 0.25f, edpart_refrotz, edpart_refroty, 0,
                               rotation_z, rotation_y, 0xffff0000, edpart_mtl);
            }
        }
    } else {
        edbitsDrawCube(edpart_cam_pos.x, edpart_cam_pos.y, edpart_cam_pos.z, edpart_copy_size, edpart_copy_size,
                       edpart_copy_size, 0, 0, 0, 0, 0, 0xffffffff, edpart_mtl);
    }

    NuRndrRect2di(0x1720, 0x9b0, 0xf00, 0x410, 0x80808080, edpart_boxmtl);
    NuRndrRect2di(0x1710, 0x8f0, 0xf20, 0xc0, 0x80000000, edpart_boxmtl);
    NuRndrLine2di(0x1710, 0x9b0, 0x1710, 0xdc8, 0x80000000, edpart_boxmtl);
    NuRndrLine2di(0x2630, 0x9b0, 0x2630, 0xdc8, 0x80000000, edpart_boxmtl);
    NuRndrLine2di(0x1710, 0xdc8, 0x2630, 0xdc8, 0x80000000, edpart_boxmtl);
    NuQFntPushPrintMode(2);
    NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
    NuQFntSet(system_qfont);
    NuQFntSetColour(system_qfont, 0x80808080);
    NuQFntPrintEx(system_qfont, 0x17c0, 0x988, 0x10, "Info Box");
    if (edpart_readout == 0) {
        if (edpart_num_orphans < 1) {
            NuQFntSetColour(system_qfont, 0x80808080);
            NuQFntPrintEx(system_qfont, 0x17c0, 0x988, 0x10, "Info Box");
        } else {
            NuQFntSetColour(system_qfont, 0x80202080);
            NuQFntPrintEx(system_qfont, 0x17c0, 0x988, 0x10, "WARNING: %d Orphans", edpart_num_orphans);
        }
    } else if (edpart_readout == 1) {
        NuQFntSetColour(system_qfont, 0x80808080);
        NuQFntPrintEx(system_qfont, 0x17c0, 0x988, 0x10, "Co-ordinates");
    }
    NuQFntSetColour(system_qfont, 0x80000000);
    if (edpart_readout == 1) {
        if (edpart_nearest == -1) {
            NuQFntPrintEx(system_qfont, 0x17c0, 0xa50, 0x10, "Highlight: <none>");
        } else {
            NuQFntPrintEx(system_qfont, 0x17c0, 0xa50, 0x10, "Highlight: %s", edpart_nearest_type->name);
            NuQFntPrintEx(system_qfont, 0x17c0, 0xaf0, 0x10, "XYZ: %0.2f %0.2f %0.2f", edpart_nearest_emit->position.x,
                          edpart_nearest_emit->position.y, edpart_nearest_emit->position.z);
            const i16 *rotation = reinterpret_cast<const i16 *>(&edpart_nearest_emit->trailing_state_words[0]);
            NuQFntPrintEx(system_qfont, 0x17c0, 0xb90, 0x10, "RotZ: %d", rotation[0]);
            NuQFntPrintEx(system_qfont, 0x17c0, 0xc30, 0x10, "RotY: %d", rotation[1]);
            NuQFntPrintEx(system_qfont, 0x17c0, 0xcd0, 0x10, "EmitRotZ: %d", edpart_nearest_emit->rotation_2c);
            NuQFntPrintEx(system_qfont, 0x17c0, 0xd70, 0x10, "EmitRotY: %d", edpart_nearest_emit->rotation_2e);
        }
    } else if (edpart_readout == 0) {
        if (edpart_copy_mode == 0) {
            if (edpart_create_type == -1)
                NuQFntPrintEx(system_qfont, 0x17c0, 0xa50, 0x10, "Current Type: <none>");
            else
                NuQFntPrintEx(system_qfont, 0x17c0, 0xa50, 0x10, "Current Type: %s",
                              part_types[edpart_create_type].name);
            if (edpart_create_type == -1)
                NuQFntPrintEx(system_qfont, 0x17c0, 0xaf0, 0x10, "Current List: <none>");
            else if (edpart_effect_list == 1)
                NuQFntPrintEx(system_qfont, 0x17c0, 0xaf0, 0x10, "Current List: Level");
            else if (edpart_effect_list == 0)
                NuQFntPrintEx(system_qfont, 0x17c0, 0xaf0, 0x10, "Current List: General");
            if (edpart_nearest == -1) {
                NuQFntPrintEx(system_qfont, 0x17c0, 0xc30, 0x10, "Highlight: <none>");
            } else {
                edbitsDrawCube(edpart_cam_pos.x, edpart_cam_pos.y, edpart_cam_pos.z,
                               edpart_nearest_type->position_random.x, edpart_nearest_type->position_random.y,
                               edpart_nearest_type->position_random.z, edpart_emitrotz, edpart_emitroty,
                               edpart_emitrotx, rotation_z, rotation_y, 0xff0000ff, edpart_mtl);
                NuQFntSet(system_qfont);
                NuQFntSetColour(system_qfont, 0x80000000);
                NuQFntPrintEx(system_qfont, 0x17c0, 0xc30, 0x10, "Highlight: %s", edpart_nearest_type->name);
                if (edpart_nearest_orphans == 0 && edpart_nearest_duplicates == 0) {
                    NuQFntPrintEx(system_qfont, 0x1860, 0xcd0, 0x10, "Instances: %d of %d",
                                  edpart_nearest_type->variant_count, 8);
                } else {
                    NuQFntSetColour(system_qfont, 0x80000080);
                    NuQFntPrintEx(system_qfont, 0x1860, 0xcd0, 0x10, "Instances: %d of %d (Or = %d, Du = %d)",
                                  edpart_nearest_type->variant_count, 8, edpart_nearest_orphans,
                                  edpart_nearest_duplicates);
                    NuQFntSetColour(system_qfont, 0x80000000);
                }
            }
        } else {
            if (edpart_copy_enclosed > 8)
                NuQFntSetColour(system_qfont, 0x80000080);
            NuQFntPrintEx(system_qfont, 0x17c0, 0xc30, 0x10, "Enclosed: %d", edpart_copy_enclosed);
            NuQFntSetColour(system_qfont, 0x80000000);
        }
        NuQFntPrintEx(system_qfont, 0x17c0, 0xd70, 0x10, "Types: %d/%d; Emits: %d/%d", part_types_used, 128,
                      part_emits_used, 40);
    }
    NuQFntPopPrintMode();
    NuQFntPopCoordinateSystem();
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

i32 edanimParamCreate(i32 instance_id) {
    if (edanim_params_used == 64) {
        return -1;
    }
    const i32 start = edanim_next_param;
    i32 index = start;
    while (AnimParams[index].instance_id != -1) {
        index = index + 1 >= 65 ? 0 : index + 1;
        if (index == start) {
            edanim_next_param = start;
            return -1;
        }
    }
    auto &param = AnimParams[index];
    edanim_next_param = index;
    param.effect_count = 0;
    param.instance_id = instance_id;
    nuhspecial_s special;
    NuGScnGetSpecial(&special, edbits_base_scene, instance_id);
    param.platform_id = FindPlatInst(NuSpecialGetInstanceix(&special));
    param.bounce_impulse = 0.0f;
    param.bounce_spring = 0.0f;
    param.bounce_damping = 0.0f;
    if (param.platform_id != -1) {
        PlatInstBounce(param.platform_id, 0.0f, 0.0f, 0.0f);
    }
    const i32 page = edbits_anim_page;
    param.page = static_cast<i8>(page);
    edanim_page_used[page] = 1;
    edanim_page_on[page] = 1;
    if (!edanim_page_scene[page]) {
        edanim_page_scene[page] = edbits_base_scene;
    }
    edanim_next_param = index + 1;
    ++edanim_params_used;
    return index;
}

i32 edpartSaveEffects(char *filename, char page) {
    i32 type_count = 0;
    for (i32 index = 0; index < 128; ++index) {
        const part_type_s &type = part_types[index];
        if (type.name[0] != '\0' && type.field_b3 == static_cast<u8>(page))
            ++type_count;
    }

    EdFileSetMedia(1);
    if (EdFileOpen(filename, NUFILE_WRITE) == 0)
        return 0;
    EdFileSetReadWrongEndianess(1);
    EdFileWriteInt(16);
    EdFileWriteInt(type_count);

    char empty_name[16] = {};
    static char null_instance_name[16] = "NULL instance";
    for (i32 index = 0; index < 128; ++index) {
        part_type_s &type = part_types[index];
        if (type.name[0] == '\0' || type.field_b3 != static_cast<u8>(page))
            continue;

        EdFileWrite(type.name, 16);
        EdFileWriteChar(type.effect_pages[0]);
        EdFileWriteChar(type.effect_pages[1]);
        EdFileWriteChar(type.effect_pages[2]);
        EdFileWriteChar(type.effect_pages[3]);
        EdFileWriteChar(type.effect_pages[4]);
        EdFileWriteChar(type.effect_pages[5]);
        EdFileWriteChar(type.effect_pages[6]);
        EdFileWriteChar(type.effect_pages[7]);
#define WRITE_PART_OBJECT_NAME(variant)                                                                                \
    do {                                                                                                               \
        char *name = empty_name;                                                                                       \
        i16 effect_id = type.effect_ids[variant];                                                                      \
        if (effect_id == 9999)                                                                                         \
            name = null_instance_name;                                                                                 \
        else if (effect_id == 9998)                                                                                    \
            name = type.object_names[variant];                                                                         \
        else if (effect_id != -1) {                                                                                    \
            if (type.effect_pages[variant] == 0 || type.effect_pages[variant] == 1) {                                  \
                NUGSCN *scene = type.effect_pages[variant] == 0 ? edbits_base_scene : edbits_things_scene;             \
                nuhspecial_s special;                                                                                  \
                NuGScnGetSpecial(&special, scene, effect_id);                                                          \
                name = NuSpecialGetName(&special);                                                                     \
                EdFileWrite(name, 16);                                                                                 \
                break;                                                                                                 \
            }                                                                                                          \
        }                                                                                                              \
        EdFileWrite(name, 16);                                                                                         \
    } while (0)
        WRITE_PART_OBJECT_NAME(0);
        WRITE_PART_OBJECT_NAME(1);
        WRITE_PART_OBJECT_NAME(2);
        WRITE_PART_OBJECT_NAME(3);
        WRITE_PART_OBJECT_NAME(4);
        WRITE_PART_OBJECT_NAME(5);
        WRITE_PART_OBJECT_NAME(6);
        WRITE_PART_OBJECT_NAME(7);
#undef WRITE_PART_OBJECT_NAME
        EdFileWriteChar(type.variant_mode);
        EdFileWriteFloat(type.particle_scale);
        EdFileWriteFloat(type.effect_scale);
        EdFileWriteFloat(type.lifetime);
        EdFileWriteFloat(type.lifetime_random);
        EdFileWriteFloat(type.speed);
        EdFileWriteFloat(type.gravity);
        EdFileWriteFloat(type.emission_rate);
        EdFileWriteFloat(type.bounce);
        EdFileWriteNuVec(&type.position_random);
        EdFileWriteNuVec(&type.velocity_random);
        EdFileWriteFloat(type.emission_period);
        EdFileWriteFloat(type.emission_period_random);
        EdFileWriteFloat(type.emission_pause);
        EdFileWriteFloat(type.emission_pause_random);
        EdFileWriteInt(type.rotation[0]);
        EdFileWriteInt(type.rotation[1]);
        EdFileWriteInt(type.rotation[2]);
        EdFileWriteInt(type.rotation_random[0]);
        EdFileWriteInt(type.rotation_random[1]);
        EdFileWriteInt(type.rotation_random[2]);
        EdFileWriteUnsignedInt(type.flags);

        EdFileWrite(type.trail_effects[0] == -1 ? empty_name : debtab[type.trail_effects[0]]->name, 16);
        EdFileWrite(type.trail_effects[1] == -1 ? empty_name : debtab[type.trail_effects[1]]->name, 16);
        EdFileWrite(type.attached_effect == -1 ? empty_name : debtab[type.attached_effect]->name, 16);
        EdFileWriteFloat(type.trail_rates[0]);
        EdFileWriteFloat(type.trail_rates[1]);
        EdFileWrite(type.kill_effect == -1 ? empty_name : debtab[type.kill_effect]->name, 16);
        EdFileWrite(type.impact_effect == -1 ? empty_name : debtab[type.impact_effect]->name, 16);
        EdFileWrite(type.impact_part == -1 ? empty_name : part_types[type.impact_part].name, 16);
        EdFileWrite(type.sounds[0] == -1 ? empty_name : const_cast<char *>(g_soundInfo[type.sounds[0]].sfx_name), 16);
        EdFileWrite(type.sounds[1] == -1 ? empty_name : const_cast<char *>(g_soundInfo[type.sounds[1]].sfx_name), 16);
        EdFileWrite(type.sounds[2] == -1 ? empty_name : const_cast<char *>(g_soundInfo[type.sounds[2]].sfx_name), 16);
        EdFileWrite(type.sounds[3] == -1 ? empty_name : const_cast<char *>(g_soundInfo[type.sounds[3]].sfx_name), 16);
        EdFileWriteChar(type.sound_modes[0]);
        EdFileWriteChar(type.sound_modes[1]);
        EdFileWriteChar(type.sound_modes[2]);
        EdFileWriteChar(type.sound_modes[3]);
        EdFileWriteFloat(type.maximum_distance);
        EdFileWriteFloat(type.field_160);
        EdFileWriteFloat(type.field_164);
        EdFileWriteFloat(type.field_168);
    }

    if (page == 1) {
        i32 emitter_count = 0;
        for (i32 index = 0; index < 40; ++index) {
            if (part_emits[index].effect_id != -1 && part_emits[index].shots_remaining == 0)
                ++emitter_count;
        }
        EdFileWriteInt(emitter_count);
        for (i32 index = 0; index < 40; ++index) {
            part_emit_s &emitter = part_emits[index];
            if (emitter.effect_id == -1)
                continue;
            EdFileWriteNuVec(&emitter.position);
            EdFileWrite(emitter.name, 16);
            EdFileWriteShort(emitter.rotation_30);
            EdFileWriteShort(emitter.rotation_2e);
            EdFileWriteShort(emitter.rotation_2c);
            EdFileWriteShort(emitter.field_44);
            EdFileWriteShort(emitter.field_46);
        }
    } else {
        EdFileWriteInt(0);
    }
    EdFileSetReadWrongEndianess(0);
    EdFileClose();
    return 1;
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

void EdDrawPolyCylinder(VuMtx const &transform, float half_length, float radius, float end_radius, i32 sides,
                        i32 colour, i32 cap_start, i32 cap_end) {
    const float taper = end_radius / radius;
    EdDrawMtx(&transform);
    i32 segment_colour = colour;
    if (sides > 0) {
        float previous_sine = NU_SIN_LUT(0) * radius;
        float previous_cosine = NU_COS_LUT(0) * radius;
        for (i32 segment = 1; segment <= sides; ++segment) {
            const i32 angle = segment * 0x10000 / sides;
            const float sine = NU_SIN_LUT(angle) * radius;
            const float cosine = NU_COS_LUT(angle) * radius;
            const VuVec previous_top(previous_sine * taper, previous_cosine * taper, half_length, 1.0f);
            const VuVec top(sine * taper, cosine * taper, half_length, 1.0f);
            const VuVec bottom(sine, cosine, -half_length, 1.0f);
            const VuVec previous_bottom(previous_sine, previous_cosine, -half_length, 1.0f);
            EdDrawPolyTri(previous_top, top, bottom, segment_colour);
            EdDrawPolyTri(previous_top, bottom, previous_bottom, segment_colour);
            previous_sine = sine;
            previous_cosine = cosine;
            if (segment != sides) {
                segment_colour = (segment & 1) == 0 ? colour
                                                    : static_cast<i32>((static_cast<u32>(colour) & 0xff000000) |
                                                                       (((colour & 0xff) * 0xdc) >> 8) |
                                                                       ((((colour >> 8) & 0xff) * 0xdc) & 0xff00) |
                                                                       (((((colour >> 16) & 0xff) * 0xdc) >> 8) << 16));
            }
        }
    }
    if (cap_start != 0 || cap_end != 0) {
        const float negative_half_length = -half_length;
        float first_sine = NU_SIN_LUT(0);
        float first_cosine = NU_COS_LUT(0);
        const i32 first_angle = 0x10000 / sides;
        float previous_sine = NU_SIN_LUT(first_angle) * radius;
        float previous_cosine = NU_COS_LUT(first_angle) * radius;
        for (i32 segment = 0; segment < sides - 2; ++segment) {
            const i32 angle = (segment + 2) * 0x10000 / sides;
            const float sine = NU_SIN_LUT(angle) * radius;
            const float cosine = NU_COS_LUT(angle) * radius;
            if (cap_end != 0) {
                const VuVec first(first_sine * radius * taper, first_cosine * radius * taper, half_length, 1.0f);
                const VuVec current(sine * taper, cosine * taper, half_length, 1.0f);
                const VuVec previous(previous_sine * taper, previous_cosine * taper, half_length, 1.0f);
                EdDrawPolyTri(first, current, previous, segment_colour);
            }
            if (cap_start != 0) {
                const VuVec first(first_sine * radius, first_cosine * radius, negative_half_length, 1.0f);
                const VuVec previous(previous_sine, previous_cosine, negative_half_length, 1.0f);
                const VuVec current(sine, cosine, negative_half_length, 1.0f);
                EdDrawPolyTri(first, previous, current, segment_colour);
            }
            previous_sine = sine;
            previous_cosine = cosine;
        }
    }
    EdDrawMtx(NULL);
}

void EdDrawPolyCylinder(VuVec const &start, VuVec const &end, i32 sides, i32 colour, i32 cap_colour, float radius,
                        float limit, float offset) {
    VuVec direction(end.x - start.x, end.y - start.y, end.z - start.z, 0.0f);
    const float length = NuVecMag(&direction.xyz);
    if (length <= 0.0f)
        return;
    const float inverse_length = 1.0f / length;
    direction.x *= inverse_length;
    direction.y *= inverse_length;
    direction.z *= inverse_length;
    NUANGVEC angles{};
    if (direction.x == 0.0f && direction.z == 0.0f) {
        angles.x = 0x2000;
    } else {
        angles.y = NuAtan2D(direction.x, direction.z);
        NuVecRotateY(&direction.xyz, &direction.xyz, -angles.y);
        angles.x = -NuAtan2D(direction.y, direction.z);
    }
    NUMTX transform;
    NuMtxSetRotateXYZVU0(&transform, &angles);
    NUVEC center{(start.x + end.x) * 0.5f, (start.y + end.y) * 0.5f, (start.z + end.z) * 0.5f};
    NuMtxTranslate(&transform, &center);
    const float minimum_radius = radius * 0.5f * length;
    float first_radius = limit > minimum_radius ? limit : minimum_radius;
    if (first_radius > offset)
        first_radius = offset;
    float second_radius = limit > minimum_radius * 0.1f ? limit : minimum_radius * 0.1f;
    if (second_radius > offset)
        second_radius = offset;
    EdDrawPolyCylinder(*reinterpret_cast<VuMtx *>(&transform), length * 0.5f, first_radius, second_radius, sides,
                       colour, 1, 1);
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

void edgraCalculatePage(char page, i32 calculate_vectors) {
    const i32 page_index = page;
    if (!edgra_page_used[page_index] || !edgra_page_scene[page_index] || !edgra_page_matrix_stack[page_index] ||
        edgra_page_on[page_index])
        return;
    NUMTX *matrix = edgra_page_matrix_stack[page_index];
    i32 max_clumps = EDGRA_MAX_CLUMPS;
    for (i32 clump_index = 0; clump_index < max_clumps; ++clump_index) {
        edgra_clump_s &clump = GrassClumps[clump_index];
        if (clump.element_count <= 0 || static_cast<i8>(clump.page) != page)
            continue;
        struct Sample {
            NUVEC position;
            f32 scale;
        } samples[256];
        u32 seed = clump.seed;
        const i32 count = clump.element_count;
        for (i32 element = 0; element < count; ++element) {
            Sample &sample = samples[element];
            f32 distance = 0.0f;
            if (clump.kind == 3) {
                sample.position.x = GetIndGrassClump(clump.individual_index, element)->position.x;
                sample.position.y = GetIndGrassClump(clump.individual_index, element)->position.y;
                sample.position.z = GetIndGrassClump(clump.individual_index, element)->position.z;
            } else {
                NUVEC offset = {};
                switch (clump.unknown_25) {
                    case 1: {
                        u32 angle = NuRandIntSeeded(&seed) & 0xffff;
                        f32 radius = NuRandFloatSeeded(&seed) * clump.size;
                        offset.x = NU_SIN_LUT(angle) * radius;
                        offset.z = NU_COS_LUT(angle) * radius;
                        distance = NuFsqrt(offset.x * offset.x + offset.z * offset.z);
                        offset.x *= distance + 1.5f;
                        offset.z *= distance + 1.5f;
                        break;
                    }
                    case 2: {
                        u32 angle = NuRandIntSeeded(&seed) & 0xffff;
                        f32 radius = NuRandFloatSeeded(&seed) * clump.size;
                        offset.x = radius * NU_SIN_LUT(angle);
                        offset.z = radius * NU_COS_LUT(angle);
                        distance = NuFsqrt(offset.x * offset.x + offset.z * offset.z);
                        break;
                    }
                    case 3:
                        offset.x = (NuRandFloatSeeded(&seed) * 2.0f - 1.0f) * clump.size;
                        offset.z = (NuRandFloatSeeded(&seed) * 2.0f - 1.0f) * clump.size;
                        distance = NuFsqrt(offset.x * offset.x + offset.z * offset.z);
                        break;
                    case 4: {
                        i32 rows = static_cast<i32>(NuFsqrt(static_cast<f32>(count)));
                        i32 columns = (count - 1 + rows) / rows;
                        offset.x =
                            static_cast<f32>(element / columns) * (2.0f * clump.size / static_cast<f32>(rows - 1)) -
                            clump.size;
                        offset.z =
                            static_cast<f32>(element % columns) * (2.0f * clump.size / static_cast<f32>(columns - 1)) -
                            clump.size;
                        distance = NuFsqrt(offset.x * offset.x + offset.z * offset.z);
                        break;
                    }
                }
                NuVecRotateZ(&offset, &offset, clump.rotation_z);
                NuVecRotateY(&offset, &offset, clump.rotation_y);
                sample.position = offset;
            }
            sample.position.x += clump.position.x;
            sample.position.y += clump.position.y;
            sample.position.z += clump.position.z;
            if (clump.kind == 3) {
                sample.scale =
                    (clump.field_30 - clump.field_2c) * GetIndGrassClump(clump.individual_index, element)->field_0c +
                    clump.field_2c;
            } else {
                sample.scale = distance;
                switch (clump.unknown_26) {
                    case 1: {
                        const f32 random = NuRandFloatSeeded(&seed);
                        f32 scale = ((1.25f - 0.1f * distance) + (random - 0.25f) * (random - 0.25f)) / 1.7625f *
                                    clump.field_30;
                        if (scale < clump.field_2c)
                            scale = clump.field_2c;
                        sample.scale = scale;
                        break;
                    }
                    case 2:
                        sample.scale = NuRandFloatSeeded(&seed) * (clump.field_30 - clump.field_2c) + clump.field_2c;
                        break;
                    case 3: {
                        f32 limited = distance < clump.size ? distance : clump.size;
                        sample.scale = clump.field_30 - (limited / clump.size) * (clump.field_30 - clump.field_2c);
                        break;
                    }
                    case 4: {
                        f32 limited = distance < clump.size ? distance : clump.size;
                        i32 angle = static_cast<i32>((limited / clump.size) * 16384.0f);
                        sample.scale = (clump.field_30 - clump.field_2c) * NU_COS_LUT(angle) + clump.field_2c;
                        break;
                    }
                }
            }
        }
        seed = clump.seed;
        NUMTX *matrix_start = matrix;
        NUVEC *terrain_values = static_cast<NUVEC *>(clump.vector_buffer);
        for (i32 element = 0; element < count; ++element) {
            Sample &sample = samples[element];
            if (clump.kind == 3) {
                if (clump.field_42 && calculate_vectors)
                    terrain_values[element] = NuFadeObjGetAngleTerrainValues(&sample.position);
                NuMtxSetIdentity(matrix);
                NuMtxRotateZ(matrix, GetIndGrassClump(clump.individual_index, element)->field_10);
                NuMtxRotateY(matrix, GetIndGrassClump(clump.individual_index, element)->field_12);
                if (clump.field_42 && clump.field_43) {
                    NuMtxRotateZ(matrix, static_cast<i32>(terrain_values[element].z));
                    NuMtxRotateX(matrix, static_cast<i32>(terrain_values[element].x));
                }
                NUVEC scale = {sample.scale, sample.scale, sample.scale};
                NuMtxScale(matrix, &scale);
                NUVEC position = sample.position;
                if (clump.field_42)
                    position.y = clump.field_44 + terrain_values[element].y;
                NuMtxTranslate(matrix, &position);
            } else {
                if (clump.field_42 && calculate_vectors)
                    terrain_values[element] = NuFadeObjGetAngleTerrainValues(&sample.position);
                NuMtxSetIdentity(matrix);
                NuMtxPreRotateY(matrix, static_cast<u16>(NuRandIntSeeded(&seed)));
                if (clump.kind != 1 && clump.field_42 && clump.field_43) {
                    NuMtxRotateZ(matrix, static_cast<i32>(terrain_values[element].z));
                    NuMtxRotateX(matrix, static_cast<i32>(terrain_values[element].x));
                }
                NUVEC scale = {sample.scale, sample.scale, sample.scale};
                NuMtxScale(matrix, &scale);
                NUVEC position = sample.position;
                if (clump.field_42)
                    position.y = clump.field_44 + terrain_values[element].y;
                NuMtxTranslate(matrix, &position);
                if (clump.kind == 1)
                    matrix->m33 = clump.field_18 * sample.scale;
            }
            ++matrix;
        }
        clump.matrices = matrix_start;
        max_clumps = EDGRA_MAX_CLUMPS;
    }
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

i32 edpartPtlChangeType(i32 index, i32 type) {
    if (part_emits[index].effect_id != type) {
        edpartDestroy(index);
        AddDebrisEffect(&part_emits[index].instance_id, type, part_emits[index].position.x,
                        part_emits[index].position.y, part_emits[index].position.z);
        part_emits[index].effect_id = type;
    }
    return index;
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
    extern i32 edpp_copy_mode, edpp_copy_enclosed, edpp_copy_source_count, edpp_copy_source[8], edpp_copyroty;
    extern i32 edpp_showAllPlaced;
    extern NUVEC edpp_copy_source_vec;
    extern NUMTL *edpp_mtl;

    if (edpp_copy_mode != 0) {
        edpp_copy_enclosed = 0;
        edpp_particle_s *particle = edpp_ptls;
        for (i32 index = 0; index < 512; ++index, ++particle) {
            if (particle->instance_id == 99999)
                continue;
            if (particle->instance_id == -1)
                continue;
            NUVEC delta;
            NuVecSub(&delta, &edpp_cam_pos, &particle->position);
            if (!(__builtin_fabsf(delta.x) <= edpp_copy_size) || !(__builtin_fabsf(delta.y) <= edpp_copy_size) ||
                !(__builtin_fabsf(delta.z) <= edpp_copy_size))
                continue;
            ++edpp_copy_enclosed;
            edbitsDrawDiagonalCross(particle->position.x, particle->position.y, particle->position.z, 0.05f, 0xffffffff,
                                    edpp_mtl);
        }
        for (i32 source = 0; source < edpp_copy_source_count; ++source) {
            NUVEC offset, position;
            NuVecSub(&offset, &edpp_ptls[edpp_copy_source[source]].position, &edpp_copy_source_vec);
            NuVecRotateY(&offset, &offset, edpp_copyroty);
            NuVecAdd(&position, &edpp_cam_pos, &offset);
            edbitsDrawDiagonalCross(position.x, position.y, position.z, 0.05f, 0xff808080, edpp_mtl);
        }
    } else {
        if (edpp_nearest != -1) {
            const NUVEC &position = edpp_ptls[edpp_nearest].position;
            edbitsDrawCube(position.x, position.y, position.z, 0.5f, 0.5f, 0.5f, 0, 0, 0, 0, 0, 0xffffffff, edpp_mtl);
        }
        if (edpp_showAllPlaced != 0) {
            const edpp_particle_s *particle = edpp_ptls;
            for (i32 index = 0; index < 512; ++index, ++particle) {
                if (particle->effect_index > 0 && index != edpp_nearest) {
                    edbitsDrawCube(particle->position.x, particle->position.y, particle->position.z, 0.5f, 0.5f, 0.5f,
                                   0, 0, 0, 0, 0, 0xff000000, edpp_mtl);
                }
            }
        }
    }
}

void edppMultipleCopyCopy() {
    extern i32 edpp_copy_source_count, edpp_copy_source[8], edpp_copyroty;
    extern NUVEC edpp_copy_source_vec;

    edpp_copy_source_count = 0;
    edpp_particle_s *particle = edpp_ptls;
    for (i32 index = 0; index < 512; ++index, ++particle) {
        if (particle->instance_id == 99999 || particle->instance_id == -1)
            continue;

        NUVEC delta;
        NuVecSub(&delta, &edpp_cam_pos, &particle->position);
        if (__builtin_fabsf(delta.x) <= edpp_copy_size && __builtin_fabsf(delta.y) <= edpp_copy_size &&
            __builtin_fabsf(delta.z) <= edpp_copy_size) {
            edpp_copy_source[edpp_copy_source_count++] = index;
            if (edpp_copy_source_count == 8)
                break;
        }
    }
    edpp_copy_source_vec = edpp_cam_pos;
    edpp_copyroty = 0;
}

void edbriDetermineNearest(float distance) {
    if (edbri_nearest != -1) {
        NUVEC delta;
        NuVecSub(&delta, &edbri_cam_pos, &edBridges[edbri_nearest].position);
        if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z == 0.0f)
            return;
    }
    edbri_nearest = -1;
    for (i32 index = 0; index < 64; ++index) {
        if (edBridges[index].instance_id == -1)
            continue;
        NUVEC delta;
        NuVecSub(&delta, &edbri_cam_pos, &edBridges[index].position);
        f32 length_squared = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
        if (distance < 0.0f || length_squared < distance) {
            edbri_nearest = index;
            distance = length_squared;
        }
    }
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
    extern i32 edpp_copy_source_count, edpp_copyroty;
    edpp_copy_source_count = 0;
    edpp_copyroty = 0;
}

void edppMultipleCopyPaste() {
    extern i32 edpp_copy_source_count, edpp_copy_source[8], edpp_copyroty;
    extern NUVEC edpp_copy_source_vec;

    for (i32 source = 0; source < edpp_copy_source_count; ++source) {
        NUVEC offset, position;
        const i32 source_index = edpp_copy_source[source];
        NuVecSub(&offset, &edpp_ptls[source_index].position, &edpp_copy_source_vec);
        NuVecRotateY(&offset, &offset, edpp_copyroty);
        NuVecAdd(&position, &edpp_cam_pos, &offset);
        const i32 index = edppPtlCreateCopy(&position, source_index);
        if (index != -1) {
            edpp_particle_s &particle = edpp_ptls[index];
            particle.emitter_rotation_y += static_cast<i16>(edpp_copyroty);
            DebrisEmitterOrientation(particle.instance_id, particle.emitter_rotation_z, particle.emitter_rotation_y,
                                     particle.emitter_rotation_x);
        }
    }
}

void edppStartSingleEffect(i32 index) {
    edpp_particle_s &particle = edpp_ptls[index];
    if (particle.instance_id != -1)
        return;
    AddDebrisEffect(&particle.instance_id, particle.effect_index, particle.position.x, particle.position.y,
                    particle.position.z);
    if (particle.instance_id == -1)
        return;

    debkeydata[particle.instance_id].field_2f9 = 0;
    DebrisOrientation(particle.instance_id, particle.rotation_z, particle.rotation_y);
    DebrisEmitterOrientation(particle.instance_id, particle.emitter_rotation_z, particle.emitter_rotation_y,
                             particle.emitter_rotation_x);
    DebrisStartOffset(particle.instance_id, particle.start_offset);
    DebrisSetTrigger(particle.instance_id, particle.switch_type, particle.switch_id, particle.switch_variable);
    DebrisReflectionOrientation(particle.instance_id, particle.reflection_rotation_z, particle.reflection_rotation_y,
                                particle.reflection_offset, particle.reflection_bounce);
    DebrisSetFacing(particle.instance_id, particle.facing_mode, particle.facing_rotation_x, particle.facing_rotation_y);
    DebrisSetGroupID(particle.instance_id, particle.render_group);
    DebrisSetRoomID(particle.instance_id, reinterpret_cast<NUGSCN *>(edpp_page_scene[particle.page]));
    DebrisSetDetailLevels(particle.instance_id, particle.detail_levels);
}

void edpartHighlightNearest() {
    extern i32 edpart_copy_mode, edpart_nearest, edpart_copy_enclosed;
    extern i32 edpart_copy_source_count, edpart_copy_source[8], edpart_copyroty;
    extern f32 edpart_copy_size;
    extern NUVEC edpart_cam_pos, edpart_copy_source_vec;
    extern NUMTL *edpart_mtl;
    if (edpart_copy_mode != 0) {
        edpart_copy_enclosed = 0;
        part_emit_s *emitter = part_emits;
        for (i32 index = 0; index < 512; ++index, ++emitter) {
            if (emitter->instance_id == 99999)
                continue;
            if (emitter->instance_id == -1)
                continue;
            NUVEC delta;
            NuVecSub(&delta, &edpart_cam_pos, &part_emits[index].position);
            if (edpart_copy_size < __builtin_fabsf(delta.x) || edpart_copy_size < __builtin_fabsf(delta.y) ||
                edpart_copy_size < __builtin_fabsf(delta.z))
                continue;
            ++edpart_copy_enclosed;
            edbitsDrawDiagonalCross(emitter->position.x, emitter->position.y, emitter->position.z, 0.05f, 0xffffffff,
                                    edpart_mtl);
        }
        for (i32 index = 0; index < edpart_copy_source_count; ++index) {
            NUVEC offset, destination;
            NuVecSub(&offset, &part_emits[edpart_copy_source[index]].position, &edpart_copy_source_vec);
            NuVecRotateY(&offset, &offset, edpart_copyroty);
            NuVecAdd(&destination, &edpart_cam_pos, &offset);
            edbitsDrawDiagonalCross(destination.x, destination.y, destination.z, 0.05f, 0xff808080, edpart_mtl);
        }
    } else if (edpart_nearest != -1) {
        const NUVEC &position = part_emits[edpart_nearest].position;
        edbitsDrawCube(position.x, position.y, position.z, 0.5f, 0.5f, 0.5f, 0, 0, 0, 0, 0, 0xffffffff, edpart_mtl);
    }
}

void edpartMultipleCopyCopy() {
    extern i32 edpart_copy_source_count, edpart_copy_source[8], edpart_copyroty;
    extern f32 edpart_copy_size;
    extern NUVEC edpart_cam_pos, edpart_copy_source_vec;

    edpart_copy_source_count = 0;
    for (i32 index = 0; index < 512; ++index) {
        part_emit_s &emitter = part_emits[index];
        if (emitter.instance_id == 99999 || emitter.instance_id == -1)
            continue;

        NUVEC delta;
        NuVecSub(&delta, &edpart_cam_pos, &emitter.position);
        if (__builtin_fabsf(delta.x) > edpart_copy_size || __builtin_fabsf(delta.y) > edpart_copy_size ||
            __builtin_fabsf(delta.z) > edpart_copy_size)
            continue;

        edpart_copy_source[edpart_copy_source_count++] = index;
        if (edpart_copy_source_count == 8)
            break;
    }
    edpart_copy_source_vec = edpart_cam_pos;
    edpart_copyroty = 0;
}

void edpartMultipleCopyClear() {
    extern i32 edpart_copy_source_count, edpart_copyroty;
    edpart_copy_source_count = 0;
    edpart_copyroty = 0;
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

void edanimRenderSoundEmitters(i32 parameter_index) {
    auto &param = AnimParams[parameter_index];
    nuhspecial_s special;
    NuGScnGetSpecial(&special, edbits_base_scene, param.instance_id);
    const NUVEC *base_position = NuSpecialGetPos(&special);
    for (i32 sound = 0; sound < param.sound_count; ++sound) {
        if (param.sound_ids[sound] == -1) {
            continue;
        }
        const i32 colour = edanim_sound_mode && edanim_nearest_sound == sound ? 0xffff0000 : 0xffffffff;
        const auto &offset = param.sound_positions[sound];
        edbitsDrawDiagonalCross(base_position->x + offset[0], base_position->y + offset[1],
                                base_position->z + offset[2], 0.25f, colour, edanim_mtl);
    }
}

void edbobs_DrawCoordinateInfo(nuvec_s *position, i32 x, i32 y) {
    const i32 row = 1000 + y * 8;
    NuQFntPrintEx(system_qfont, (x + 10) * 16, row, 0x10, "%.3f", position->x);
    NuQFntPrintEx(system_qfont, (x + 80) * 16, row, 0x10, "%.3f", position->y);
    NuQFntPrintEx(system_qfont, (x + 150) * 16, row, 0x10, "%.3f", position->z);
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
    if (item == NULL)
        return NULL;
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
    return item;
}

void edanimDetermineNearestSound(float distance) {
    if (edanim_nearest == -1 || edanim_nearest_param_id == -1) {
        edanim_nearest_sound = -1;
        return;
    }
    nuhspecial_s special;
    NuGScnGetSpecial(&special, edbits_base_scene, edanim_nearest);
    NUVEC *base_position = NuSpecialGetPos(&special);
    NUVEC world_position;
    NUVEC delta;
    auto &param = AnimParams[edanim_nearest_param_id];
    if (edanim_nearest_sound != -1) {
        NuVecAdd(&world_position, base_position,
                 reinterpret_cast<NUVEC *>(param.sound_positions[edanim_nearest_sound]));
        NuVecSub(&delta, &edanim_cam_pos, &world_position);
        if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z == 0.0f) {
            return;
        }
    }
    edanim_nearest_sound = -1;
    for (i32 sound = 0; sound < param.sound_count; ++sound) {
        NuVecAdd(&world_position, base_position, reinterpret_cast<NUVEC *>(param.sound_positions[sound]));
        NuVecSub(&delta, &edanim_cam_pos, &world_position);
        const f32 candidate = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
        if (distance < 0.0f || candidate < distance) {
            distance = candidate;
            edanim_nearest_sound = sound;
        }
    }
}

void edanimRenderParticleEmitters(i32 parameter_index) {
    auto &param = AnimParams[parameter_index];
    nuhspecial_s special;
    NuGScnGetSpecial(&special, edbits_base_scene, param.instance_id);
    const NUVEC *base_position = NuSpecialGetPos(&special);
    for (i32 particle = 0; particle < param.effect_count; ++particle) {
        const i32 effect_id = param.effect_ids[particle];
        if (effect_id == -1) {
            continue;
        }
        const auto &offset = param.effect_positions[particle];
        const f32 x = base_position->x + offset[0];
        const f32 y = base_position->y + offset[1];
        const f32 z = base_position->z + offset[2];
        i32 colour = 0xffffffff;
        if (edanim_particle_mode && edanim_nearest_particle == particle) {
            const debinftype *definition = debtab[effect_id];
            if (definition->generator_type == 0) {
                edbitsDrawCube(x, y, z, definition->field_058, definition->field_05c, definition->field_060,
                               edanim_emitrotz, edanim_emitroty, 0, 0, 0, 0xff008000, edanim_mtl);
            }
            colour = 0xff00ff00;
        }
        edbitsDrawDiagonalCross(x, y, z, 0.25f, colour, edanim_mtl);
    }
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

void edanimDetermineNearestParticle(float distance) {
    if (edanim_nearest == -1 || edanim_nearest_param_id == -1) {
        edanim_nearest_particle = -1;
        return;
    }
    nuhspecial_s special;
    NuGScnGetSpecial(&special, edbits_base_scene, edanim_nearest);
    NUVEC *base_position = NuSpecialGetPos(&special);
    NUVEC world_position;
    NUVEC delta;
    auto &param = AnimParams[edanim_nearest_param_id];
    if (edanim_nearest_particle != -1) {
        NuVecAdd(&world_position, base_position,
                 reinterpret_cast<NUVEC *>(param.effect_positions[edanim_nearest_particle]));
        NuVecSub(&delta, &edanim_cam_pos, &world_position);
        if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z == 0.0f) {
            return;
        }
    }
    edanim_nearest_particle = -1;
    for (i32 particle = 0; particle < param.effect_count; ++particle) {
        NuVecAdd(&world_position, base_position, reinterpret_cast<NUVEC *>(param.effect_positions[particle]));
        NuVecSub(&delta, &edanim_cam_pos, &world_position);
        const f32 candidate = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
        if (distance < 0.0f || candidate < distance) {
            distance = candidate;
            edanim_nearest_particle = particle;
        }
    }
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

i32 EdTerrRay(VuVec &, VuVec &) {
    return 0;
}

EdManScale::EdManScale() {
    selected_attribute = 32;
}

i32 EdManScale::Process(EdInputContext &input, ClassObjectList &selected) {
    EdManipulator::Process(input, selected);
    VuVec average;
    if (selected.GetAveragePosition(average) == 0)
        return 0;

    VuMtx orientation;
    get_manipulator_attribute(selected.first, 0x10, EdType_VuMtx, &orientation);
    VuVec first_axis;
    VuVec second_axis;
    i32 axis = SelectAxis(input, average, first_axis, second_axis, &orientation);
    theLevelEditor.field_0x2c = AxisColour[axis];
    if (input.GetHold(3) != 0.0f) {
        VuVec const &delta = *reinterpret_cast<VuVec const *>(reinterpret_cast<u8 *>(this) + 0x40);
        for (ClassObjectListEntry *entry = selected.first; entry != NULL; entry = entry->next) {
            VuMtx transform;
            get_manipulator_attribute(entry, 0x20, EdType_VuMtx, &transform);

            f32 scale_x = 1.0f;
            f32 scale_y = 1.0f;
            f32 scale_z = 1.0f;
            if (axis >= 1 && axis <= 6) {
                NUMTX &matrix = transform.matrix;
                VuVec projected;
                projected.x = matrix.m00 * first_axis.x + matrix.m10 * first_axis.y + matrix.m20 * first_axis.z;
                projected.y = matrix.m01 * first_axis.x + matrix.m11 * first_axis.y + matrix.m21 * first_axis.z;
                projected.z = matrix.m02 * first_axis.x + matrix.m12 * first_axis.y + matrix.m22 * first_axis.z;
                projected.w = 0.0f;
                f32 magnitude = NuVecMag(reinterpret_cast<NUVEC *>(&projected));
                f32 movement = delta.x * first_axis.x + delta.y * first_axis.y + delta.z * first_axis.z;
                if (axis >= 4)
                    movement += delta.x * second_axis.x + delta.y * second_axis.y + delta.z * second_axis.z;
                if (movement == 0.0f)
                    continue;
                VuVec local_axis = first_axis;
                NuVecInvMtxRotate(reinterpret_cast<NUVEC *>(&local_axis), reinterpret_cast<NUVEC *>(&local_axis),
                                  &matrix);
                NuVecNorm(reinterpret_cast<NUVEC *>(&local_axis), reinterpret_cast<NUVEC *>(&local_axis));
                f32 scaled_magnitude = Scale * magnitude;
                f32 change = (scaled_magnitude + movement) / scaled_magnitude - 1.0f;
                if (axis >= 4) {
                    scale_x = second_axis.x * change + local_axis.x * change + 1.0f;
                    scale_y = second_axis.y * change + local_axis.y * change + 1.0f;
                    scale_z = second_axis.z * change + local_axis.z * change + 1.0f;
                } else {
                    scale_x = local_axis.x * change + 1.0f;
                    scale_y = local_axis.y * change + 1.0f;
                    scale_z = local_axis.z * change + 1.0f;
                }
            } else if (axis == 7) {
                f32 movement = input.Get(1) - input.Get(0) + input.Get(2);
                if (movement == 0.0f)
                    continue;
                scale_x = scale_y = scale_z = movement * 0.005f + 1.0f;
            } else {
                continue;
            }
            NUMTX &matrix = transform.matrix;
            const NUMTX original = matrix;
            const f32 zero = 0.0f;
            matrix.m00 = scale_x * original.m00 + zero * original.m10 + zero * original.m20;
            matrix.m01 = scale_x * original.m01 + zero * original.m11 + zero * original.m21;
            matrix.m02 = scale_x * original.m02 + zero * original.m12 + zero * original.m22;
            matrix.m10 = zero * original.m00 + scale_y * original.m10 + zero * original.m20;
            matrix.m11 = zero * original.m01 + scale_y * original.m11 + zero * original.m21;
            matrix.m12 = zero * original.m02 + scale_y * original.m12 + zero * original.m22;
            matrix.m20 = zero * original.m00 + zero * original.m10 + scale_z * original.m20;
            matrix.m21 = zero * original.m01 + zero * original.m11 + scale_z * original.m21;
            matrix.m22 = zero * original.m02 + zero * original.m12 + scale_z * original.m22;
            matrix.m03 = matrix.m13 = matrix.m23 = 0.0f;
            set_manipulator_attribute(entry, 0x20, EdType_VuMtx, &transform);
        }
    }
    return axis != 0;
}

void EdManScale::Render(ClassObjectList &selected) {
    if (selected.count > 2)
        EdManipulator::Render(selected);
    VuVec average;
    if (selected.GetAveragePosition(average) == 0)
        return;
    VuMtx transform;
    ClassObjectListEntry *entry = selected.first;
    if (entry->reference == NULL ||
        entry->reference->GetAttributeData(entry->object, 0x20, EdType_VuMtx, &transform, 0) == 0) {
        EdMember member;
        if (entry->ed_class->FindMember(&member, entry->object, 0x20, 1) != 0)
            member.reference->GetAttributeData(member.object, 0x20, EdType_VuMtx, &transform, 0);
    }
    DrawAxis(average, &transform);
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
        notifier->NotifyCreateObject(object, object_class, source, index, context, flags);
    }
}

void EdRegistry::NotifyDefunctObject(void *object, EdClass *object_class, i32 flags) {
    for (i32 i = 0; i < notifier_count; ++i) {
        EdObjectNotifier *notifier = notifiers[i];
        notifier->NotifyDefunctObject(object, object_class, flags);
    }
}

void EdRegistry::NotifyDestroyObject(void *object, EdClass *object_class, i32 index, i32 flags) {
    for (i32 i = 0; i < notifier_count; ++i) {
        EdObjectNotifier *notifier = notifiers[i];
        notifier->NotifyDestroyObject(object, object_class, index, flags);
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

i32 EdManRotate::RotateItem(EdInputContext &, ClassObjectList &objects, i32 angle, i32 axis) {
    VuVec average;
    objects.GetAveragePosition(average);
    if (angle == 0 || objects.first == NULL)
        return axis;
    i32 sine_index = (angle >> 1) & 0x7fff;
    i32 cosine_index = ((angle + 0x4000) >> 1) & 0x7fff;
    for (ClassObjectListEntry *entry = objects.first; entry != NULL; entry = entry->next) {
        NUMTX matrix;
        NuMtxSetIdentity(&matrix);
        EdMember member;
        member.object = NULL;
        member.reference = NULL;
        i32 got_matrix = entry->reference != NULL &&
                         entry->reference->GetAttributeData(entry->object, 0x10, EdType_VuMtx, &matrix, 0) != 0;
        if (!got_matrix) {
            got_matrix = entry->ed_class->FindMember(&member, entry->object, 0x10, 1) != 0 &&
                         member.reference->GetAttributeData(member.object, 0x10, EdType_VuMtx, &matrix, 0) != 0;
        }
        if (!got_matrix)
            continue;
        f32 sine = NuTrigTable[sine_index];
        f32 cosine = NuTrigTable[cosine_index];
        switch (axis) {
            case 1: {
                f32 m01 = matrix.m01, m11 = matrix.m11, m21 = matrix.m21;
                matrix.m01 = m01 * cosine - matrix.m02 * sine;
                matrix.m02 = m01 * sine + matrix.m02 * cosine;
                matrix.m11 = m11 * cosine - matrix.m12 * sine;
                matrix.m12 = m11 * sine + matrix.m12 * cosine;
                matrix.m21 = m21 * cosine - matrix.m22 * sine;
                matrix.m22 = m21 * sine + matrix.m22 * cosine;
                break;
            }
            case 2: {
                f32 m00 = matrix.m00, m10 = matrix.m10, m20 = matrix.m20;
                matrix.m00 = m00 * cosine + matrix.m02 * sine;
                matrix.m02 = matrix.m02 * cosine - m00 * sine;
                matrix.m10 = m10 * cosine + matrix.m12 * sine;
                matrix.m12 = matrix.m12 * cosine - m10 * sine;
                matrix.m20 = m20 * cosine + matrix.m22 * sine;
                matrix.m22 = matrix.m22 * cosine - m20 * sine;
                break;
            }
            case 3: {
                f32 m00 = matrix.m00, m10 = matrix.m10, m20 = matrix.m20;
                matrix.m00 = m00 * cosine - matrix.m01 * sine;
                matrix.m01 = m00 * sine + matrix.m01 * cosine;
                matrix.m10 = m10 * cosine - matrix.m11 * sine;
                matrix.m11 = m10 * sine + matrix.m11 * cosine;
                matrix.m20 = m20 * cosine - matrix.m21 * sine;
                matrix.m21 = m20 * sine + matrix.m21 * cosine;
                break;
            }
            default:
                continue;
        }
        if (entry->reference != NULL &&
            entry->reference->SetAttributeData(entry->object, 0x10, EdType_VuMtx, &matrix, 0) != 0)
            continue;
        if (entry->ed_class->FindMember(&member, entry->object, 0x10, 1) != 0)
            member.reference->SetAttributeData(member.object, 0x10, EdType_VuMtx, &matrix, 0);
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

static EdBitControl *edBitControl;
static edui_prop_s *edBitItem;
static i32 edBitIndex;

__attribute__((force_align_arg_pointer)) void EdBitControl::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    void *memory = theMemoryManager.AllocPool(sizeof(EdBitControl), 1);
    EdBitControl *control = new (memory) EdBitControl();
    control->reference = member;
    control->object = target;
    control->items = items;
    control->bit_mask = bit_mask;
    i32 value = 0;
    member->GetMemberData(target, EdType_Int, &value, 0);
    control->item =
        eduiItemExpanderCreate(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected, member->name);
    eduiMenuAddItem(menu, control->item);
    for (i32 bit = 0; bit < 32; ++bit) {
        if ((control->bit_mask & (1u << bit)) == 0)
            continue;
        char bit_name[0x10];
        sprintf(bit_name, "%d", bit);
        char *text = control->GetEnumString((value >> bit) & 1);
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
        eduiMenuCreate(menu->x + item->x, item->y, 180, 250, reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)),
                       cbEdLevelDestroy, NULL);
    if (choices != NULL) {
        for (Item *entry = edBitControl->items; entry != NULL && entry->name != NULL; ++entry) {
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
    if (edBitControl == NULL)
        return;
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
    EdEnumControl *control = new (memory) EdEnumControl();
    control->reference = member;
    control->object = target;
    control->items = items;
    i32 value = 0;
    member->GetMemberData(target, EdType_Int, &value, 0);
    control->item = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected, cbChanged,
                                       cbButton, 1, member->name, control->GetEnumString(value));
    eduiMenuAddItem(menu, control->item);
}

EdEnumControl::Item EdEnumControl::OpenClosedItems[] = {{"Open", 1}, {"Closed", 0}, {NULL, 0}};
EdEnumControl::Item EdEnumControl::OnOffItems[] = {{"On", 1}, {"Off", 0}, {NULL, 0}};
EdEnumControl::Item EdEnumControl::YesNoItems[] = {{"Yes", 1}, {"No", 0}, {NULL, 0}};

char *EdEnumControl::GetEnumString(i32 value) {
    for (Item *entry = items; entry != NULL && entry->name != NULL; ++entry) {
        if (entry->value == value)
            return entry->name;
    }
    return "Unknown";
}

i32 EdEnumControl::GetEnumValue(char *name) {
    for (Item *entry = items; entry != NULL && entry->name != NULL; ++entry) {
        if (NuStrICmp(entry->name, name) != 0)
            return entry->value;
    }
    return 0;
}

void EdEnumControl::Refresh() {
    if (reference == NULL || item == NULL)
        return;
    i32 value = 0;
    reference->GetMemberData(object, EdType_Int, &value, 0);
    eduiItemPropSetText(reinterpret_cast<edui_prop_s *>(item), GetEnumString(value));
}

static EdEnumControl *active_enum_control;

void EdEnumControl::cbButton(eduimenu_s *menu, eduiitem_s *item, u32) {
    EdEnumControl *control = static_cast<EdEnumControl *>(item->data_ptr);
    active_enum_control = control;
    eduimenu_s *choices =
        eduiMenuCreate(menu->x + item->x, item->y, 180, 250, reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)),
                       cbEdLevelDestroy, NULL);
    if (choices == NULL)
        return;
    for (Item *entry = control->items; entry != NULL && entry->name != NULL; ++entry)
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
    VuVec points[8];
    GetAxisLocators(origin, points, matrix);
    const i32 active = *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8);
    EdDrawBegin(1);
    for (i32 axis = 1; axis <= 3; ++axis) {
        const i32 colour = active != 0 ? AxisColour[axis] : static_cast<i32>(0xff808080);
        EdDrawLineSphere(points[axis], Scale * 0.25f, 1.0f, colour);
    }
    VuMtx box;
    NuMtxSetIdentity(&box.matrix);
    for (i32 plane = 4; plane <= 7; ++plane) {
        box.matrix.m30 = points[plane].x;
        box.matrix.m31 = points[plane].y;
        box.matrix.m32 = points[plane].z;
        box.matrix.m33 = 1.0f;
        const i32 colour = active != 0 ? AxisColour[plane] : static_cast<i32>(0xff808080);
        EdDrawLineCube(box, Scale * 0.1f, colour);
    }
    EdDrawEnd();
    EdDrawBegin(0);
    const float arrow_half_size = Scale * 0.25f * 0.5f;
    const float arrow_radius = Scale * 0.25f * 0.2f;
    for (i32 axis = 1; axis <= 3; ++axis) {
        const VuVec &point = points[axis];
        const VuVec &center = points[7];
        const VuVec offset((point.x - center.x) * arrow_half_size, (point.y - center.y) * arrow_half_size,
                           (point.z - center.z) * arrow_half_size, 0.0f);
        const VuVec start(point.x - offset.x, point.y - offset.y, point.z - offset.z, 0.0f);
        const VuVec end(point.x + offset.x, point.y + offset.y, point.z + offset.z, 0.0f);
        const i32 colour = active != 0 ? AxisColour[axis] : static_cast<i32>(0xff808080);
        EdDrawPolyArrow(start, end, 8, colour, arrow_radius, arrow_radius, axis == 1 ? 0.5f : 0.2f, 0.0f);
    }
    EdDrawEnd();
}

void EdManipulator::DrawRotator(VuVec &origin) {
    i32 active = *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 8);
    EdDrawBegin(1);
    EdDrawLineCircleX(origin, Scale, active != 0 ? AxisColour[1] : static_cast<i32>(0xff808080), 32);
    EdDrawLineCircleY(origin, Scale, active != 0 ? AxisColour[2] : static_cast<i32>(0xff808080), 32);
    EdDrawLineCircleZ(origin, Scale, active != 0 ? AxisColour[3] : static_cast<i32>(0xff808080), 32);
    EdDrawEnd();
    const u8 *data = reinterpret_cast<const u8 *>(this);
    const i32 start_angle = *reinterpret_cast<const i32 *>(data + 0x60);
    const i32 end_angle = *reinterpret_cast<const i32 *>(data + 0x64);
    const i32 selected_axis = *reinterpret_cast<const i32 *>(data + 0x0c);
    if (start_angle != end_angle && selected_axis != 0) {
        EdDrawBegin(1);
        EdDrawPolySector(origin, Scale, selected_axis - 1, start_angle, end_angle, static_cast<i32>(0x80808080), 32);
        EdDrawEnd();
    }
}

void EdManipulator::GetAxisLocators(VuVec &origin, VuVec *points, VuMtx *matrix) {
    const f32 scale = Scale;
    const f32 half = scale * 0.5f;
    points[0] = VuVec(0.0f, 0.0f, 0.0f, 1.0f);
    points[1] = VuVec(scale, 0.0f, 0.0f, 1.0f);
    points[2] = VuVec(0.0f, scale, 0.0f, 1.0f);
    points[3] = VuVec(0.0f, 0.0f, scale, 1.0f);
    points[4] = VuVec(half, half, 0.0f, 0.0f);
    points[5] = VuVec(half, 0.0f, half, 0.0f);
    points[6] = VuVec(0.0f, half, half, 0.0f);
    points[7] = VuVec(0.0f, 0.0f, 0.0f, 1.0f);
    if (matrix != NULL) {
#define TRANSFORM_LOCATOR(index)                                                                                       \
    {                                                                                                                  \
        VuVec &point = points[index];                                                                                  \
        const NUMTX &transform = matrix->matrix;                                                                       \
        const f32 x = point.x;                                                                                         \
        const f32 y = point.y;                                                                                         \
        const f32 z = point.z;                                                                                         \
        point.x = x * transform.m00 + y * transform.m10 + z * transform.m20;                                           \
        point.y = x * transform.m01 + y * transform.m11 + z * transform.m21;                                           \
        point.z = x * transform.m02 + y * transform.m12 + z * transform.m22;                                           \
    }
        TRANSFORM_LOCATOR(0);
        TRANSFORM_LOCATOR(1);
        TRANSFORM_LOCATOR(2);
        TRANSFORM_LOCATOR(3);
        TRANSFORM_LOCATOR(4);
        TRANSFORM_LOCATOR(5);
        TRANSFORM_LOCATOR(6);
        TRANSFORM_LOCATOR(7);
#undef TRANSFORM_LOCATOR
    }
#define OFFSET_LOCATOR(index)                                                                                          \
    points[index].x += origin.x;                                                                                       \
    points[index].y += origin.y;                                                                                       \
    points[index].z += origin.z
    OFFSET_LOCATOR(0);
    OFFSET_LOCATOR(1);
    OFFSET_LOCATOR(2);
    OFFSET_LOCATOR(3);
    OFFSET_LOCATOR(4);
    OFFSET_LOCATOR(5);
    OFFSET_LOCATOR(6);
    OFFSET_LOCATOR(7);
#undef OFFSET_LOCATOR
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

    VuVec average;
    f32 distance = 0.0f;
    VuVec *ray_origin = reinterpret_cast<VuVec *>(input.reserved_00 + 0x20);
    VuVec *ray_direction = reinterpret_cast<VuVec *>(input.reserved_00 + 0x30);
    if (selected.GetAveragePosition(average) != 0) {
        VuVec displacement = {average.x - ray_origin->x, average.y - ray_origin->y, average.z - ray_origin->z, 0.0f};
        distance = NuVecMag(reinterpret_cast<NUVEC *>(&displacement));
    }
    VuVec direction = *ray_direction;
    NuVecNorm(reinterpret_cast<NUVEC *>(&direction), reinterpret_cast<NUVEC *>(&direction));
    VuVec point = {ray_origin->x + direction.x * distance, ray_origin->y + direction.y * distance,
                   ray_origin->z + direction.z * distance, 0.0f};
    VuVec *last_point = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x30);
    VuVec *delta = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x40);
    VuVec *cursor = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x50);
    if (input.GetPress(3) != 0.0f) {
        *delta = VuVec_Zero;
    } else {
        delta->x = point.x - last_point->x;
        delta->y = point.y - last_point->y;
        delta->z = point.z - last_point->z;
        delta->w = 1.0f;
    }
    *last_point = point;
    last_point->w = 0.0f;
    delta->w = 1.0f;
    *cursor = *reinterpret_cast<VuVec *>(input.reserved_00);
    if (EdTerrRay(*cursor, *last_point) == 0)
        *cursor = *last_point;
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
        ClassObject object = {entry->ed_class, entry->object, entry->reference};
        theClassEditor.DrawObjectSphere(object, static_cast<i32>(0xff800000));
    }
}

i32 EdManipulator::SelectAxis(EdInputContext &input, VuVec &origin, VuVec &first_axis, VuVec &second_axis,
                              VuMtx *matrix) {
    VuVec locators[8];
    GetAxisLocators(origin, locators, matrix);
    first_axis = VuVec_Zero;
    second_axis = VuVec_Zero;
    first_axis.w = 1.0f;
    second_axis.w = 1.0f;

    VuVec *ray_origin = reinterpret_cast<VuVec *>(input.reserved_00 + 0x20);
    VuVec *ray_direction = reinterpret_cast<VuVec *>(input.reserved_00 + 0x30);
    f32 nearest_distance = __FLT_MAX__;
    i32 nearest = 0;
    for (i32 index = 1; index < 8; ++index) {
        VuVec closest;
        f32 distance = LineToPointDistance(*ray_origin, *ray_direction, locators[index], &closest);
        f32 radius = (index <= 3 ? 0.25f : 0.1f) * Scale;
        if (distance < radius && distance < nearest_distance) {
            nearest_distance = distance;
            nearest = index;
        }
    }

    i32 *selected_axis = reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x0c);

    switch (nearest) {
        case 1:
            first_axis.x = 1.0f;
            break;
        case 2:
            first_axis.y = 1.0f;
            break;
        case 3:
            first_axis.z = 1.0f;
            break;
        case 4:
            first_axis.x = 1.0f;
            second_axis.y = 1.0f;
            break;
        case 5:
            first_axis.x = 1.0f;
            second_axis.z = 1.0f;
            break;
        case 6:
            first_axis.y = 1.0f;
            second_axis.z = 1.0f;
            break;
        case 7:
            first_axis.y = 1.0f;
            second_axis.z = 1.0f;
            break;
        default:
            break;
    }
    if (matrix != NULL) {
        const NUMTX &transform = matrix->matrix;
        VuVec *axes[2] = {&first_axis, &second_axis};
        for (i32 index = 0; index < 2; ++index) {
            VuVec &axis = *axes[index];
            f32 x = axis.x;
            f32 y = axis.y;
            f32 z = axis.z;
            axis.x = x * transform.m00 + y * transform.m10 + z * transform.m20;
            axis.y = x * transform.m01 + y * transform.m11 + z * transform.m21;
            axis.z = x * transform.m02 + y * transform.m12 + z * transform.m22;
            NuVecNorm(reinterpret_cast<NUVEC *>(&axis), reinterpret_cast<NUVEC *>(&axis));
        }
    }
    if (input.GetPress(3) != 0.0f) {
        *selected_axis = nearest;
        *reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x10) = first_axis;
        *reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x20) = second_axis;
    } else if (input.GetHold(3) == 0.0f) {
        *selected_axis = 0;
        *reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x10) = VuVec_Zero;
        *reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x20) = VuVec_Zero;
    } else {
        nearest = *selected_axis;
        first_axis = *reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x10);
        second_axis = *reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x20);
    }
    return nearest;
}

i32 EdManipulator::SelectRotator(EdInputContext &input, VuVec &center, VuVec &plane) {
    VuVec &ray_origin = *reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(&input) + 0x20);
    VuVec &ray_direction = *reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(&input) + 0x30);
    i32 *selected_axis = reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x0c);
    i32 *start_angle = reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x60);
    i32 *last_angle = reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x64);
    i32 *angle_delta = reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x68);
    VuVec *selected_plane = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x10);
    VuVec *secondary_plane = reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(this) + 0x20);
    f32 pressed = input.GetPress(3);
    if (pressed != 0.0f || input.GetHold(3) == 0.0f) {
        VuVec far_point;
        VuVec near_point;
        i32 axis = 0;
        VuVec chosen = VuVec_Zero;
        f32 nearest_distance = __FLT_MAX__;
        if (LineToSphereIntersection(ray_origin, ray_direction, center, Scale + 0.01f, &far_point, &near_point) != 0) {
            SpherePos1 = far_point;
            SpherePos2 = near_point;
            const VuVec points[2] = {far_point, near_point};
            for (i32 candidate = 1; candidate <= 3; ++candidate) {
                for (i32 side = 0; side < 2; ++side) {
                    const VuVec &point = points[side];
                    VuVec normal = {candidate == 1 ? 1.0f : 0.0f, candidate == 2 ? 1.0f : 0.0f,
                                    candidate == 3 ? 1.0f : 0.0f,
                                    candidate == 1   ? -center.x
                                    : candidate == 2 ? -center.y
                                                     : -center.z};
                    f32 distance_to_plane = point.x * normal.x + point.y * normal.y + point.z * normal.z + normal.w;
                    VuVec projection = {point.x - normal.x * distance_to_plane, point.y - normal.y * distance_to_plane,
                                        point.z - normal.z * distance_to_plane, 0.0f};
                    VuVec screen_point;
                    VuVec screen_projection;
                    NuCameraTransformScreenClip(reinterpret_cast<NUVEC *>(&screen_projection),
                                                reinterpret_cast<NUVEC *>(&projection), 1, NULL);
                    NuCameraTransformScreenClip(reinterpret_cast<NUVEC *>(&screen_point),
                                                reinterpret_cast<NUVEC *>(const_cast<VuVec *>(&point)), 1, NULL);
                    VuVec difference = {screen_point.x - screen_projection.x, screen_point.y - screen_projection.y,
                                        screen_point.z - screen_projection.z, 0.0f};
                    f32 distance = NuVecMag(reinterpret_cast<NUVEC *>(&difference));
                    if (distance < 0.05f && distance < nearest_distance) {
                        nearest_distance = distance;
                        chosen = point;
                        axis = candidate;
                    }
                }
            }
        }
        if (pressed != 0.0f) {
            *selected_axis = axis;
            *angle_delta = 0;
            if (axis == 0) {
                *selected_plane = plane;
                *start_angle = *last_angle = 0;
                return 0;
            }
            plane = VuVec(axis == 1 ? 1.0f : 0.0f, axis == 2 ? 1.0f : 0.0f, axis == 3 ? 1.0f : 0.0f,
                          axis == 1   ? -center.x
                          : axis == 2 ? -center.y
                                      : -center.z);
            *selected_plane = plane;
            f32 x = chosen.x - center.x;
            f32 y = chosen.y - center.y;
            f32 z = chosen.z - center.z;
            i32 angle = axis == 1 ? NuAtan2DA(y, z) : axis == 2 ? NuAtan2DA(x, -z) : NuAtan2DA(x, y);
            *start_angle = *last_angle = angle;
            return axis;
        }
        *selected_axis = 0;
        *start_angle = *last_angle = *angle_delta = 0;
        *selected_plane = VuVec_Zero;
        *secondary_plane = VuVec_Zero;
        return axis;
    }
    i32 axis = *selected_axis;
    plane = *selected_plane;
    VuVec intersection;
    if (LineToPlaneIntersecion(ray_origin, ray_direction, plane, &intersection) == 0) {
        *angle_delta = 0;
        return axis;
    }
    f32 x = intersection.x - center.x;
    f32 y = intersection.y - center.y;
    f32 z = intersection.z - center.z;
    i32 angle = axis == 1 ? NuAtan2DA(y, z) : axis == 2 ? NuAtan2DA(x, -z) : axis == 3 ? NuAtan2DA(x, y) : 0;
    i32 delta = (*last_angle - angle) & 0xffff;
    if (delta >= 0x8000)
        delta -= 0x10000;
    *last_angle = angle;
    *angle_delta = delta;
    return axis;
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
    delta_time = elapsed;

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
    const u32 buttons = new_pad->digital_buttons;
    const i32 left_click = (NuMouseReadButtons() == 1 || (buttons & 0x800) != 0) && !alt_or_space;
    const f32 mouse_x = NuMouseReadXVel();
    const f32 mouse_y = NuMouseReadYVel();
    const f32 mouse_z = NuMouseReadZVel();
    const bool pad_enabled = edGetPadDisabled() == 0;
    const bool menu_closed = pad_enabled && eduiGetActiveMenu() == NULL;
    const i32 right_click = NuMouseReadButtons() == 2 || (pad_enabled && (buttons & 0x20) != 0);
    Set(0, mouse_x, elapsed);
    Set(1, mouse_y, elapsed);
    Set(2, mouse_z, elapsed);
    Set(3, left_click && !control_or_c, elapsed);
    Set(4, right_click && !alt_or_space && !control_or_c, elapsed);
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
    Set(23, static_cast<f32>(NuKeyboard(0x1f) && control_or_c), elapsed);
    Set(24, static_cast<f32>(NuKeyboard(0x01)), elapsed);
    Set(25, static_cast<f32>(left_click && control_or_c), elapsed);
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
    Set(37, static_cast<f32>(menu_closed ? buttons & 0x80 : 0), elapsed);
    if (menu_closed)
        Set(38, static_cast<f32>(buttons & 0x40), elapsed);
    Set(39, static_cast<f32>(buttons & 0x800), elapsed);
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
    u32 value = 0xff000000 | (static_cast<i32>(colour.x * 255.0f) & 0xff) |
                ((static_cast<i32>(colour.y * 255.0f) & 0xff) << 8) |
                ((static_cast<i32>(colour.z * 255.0f) & 0xff) << 16);
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
    reference->GetMemberData(object, EdType_Colour3, &colour, 0);
    char value[128];
    sprintf(value, "%.2f %.2f %.2f", colour.x, colour.y, colour.z);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), value);
    set_colour_preview(item, colour);
}

void EdColourControl::cbButton(eduimenu_s *menu, eduiitem_s *item, u32) {
    edColourControl = static_cast<EdColourControl *>(item->data_ptr);
    eduimenu_s *picker_menu =
        eduiMenuCreate(menu->x + item->x, item->y, 180, 250, reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)),
                       cbEdLevelDestroy, NULL);
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
    edColourControl->reference->SetMemberData(edColourControl->object, EdType_Colour3, &colour, 0, NULL);
    char value[128];
    sprintf(value, "%.2f %.2f %.2f", colour.x, colour.y, colour.z);
    eduiItemPropSetText(static_cast<edui_prop_s *>(edColourControl->item), value);
    set_colour_preview(edColourControl->item, colour);
    cbEdLevelDestroyOnSelect(menu, item, flags);
}

void EdMatrixControl::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    EdMatrixControl *control = new (theMemoryManager.AllocPool(sizeof(EdMatrixControl), 1)) EdMatrixControl();
    if (!control)
        return;
    control->reference = member;
    control->object = target;
    VuMtx matrix;
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
    if (member->attributes & 8) {
        ADD_MATRIX_COMPONENT(0, "pos x", matrix.matrix.m30);
        ADD_MATRIX_COMPONENT(1, "pos y", matrix.matrix.m31);
        ADD_MATRIX_COMPONENT(2, "pos z", matrix.matrix.m32);
    }
    if (member->attributes & 0x10) {
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
#undef ADD_MATRIX_COMPONENT
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

EdMatrixControl::~EdMatrixControl() {
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
#undef REFRESH_MATRIX_COMPONENT
}

void EdMatrixControl::SetMenuItemAttr(i32 mask, eduiitem_s *menu_item, eduiiattr_s *selected, eduiiattr_s *unselected) {
    if (menu_item == components[0] || menu_item == components[1] || menu_item == components[2])
        memcpy(menu_item->colours, (mask & 8) ? unselected : selected, sizeof(*selected));
    if (menu_item == components[3] || menu_item == components[4] || menu_item == components[5])
        memcpy(menu_item->colours, (mask && (mask & 0x10)) ? unselected : selected, sizeof(*selected));
    if (menu_item == components[6] || menu_item == components[7] || menu_item == components[8])
        memcpy(menu_item->colours, (mask && (mask & 0x20)) ? unselected : selected, sizeof(*selected));
}

void EdMatrixControl::cbButton(eduimenu_s *menu, eduiitem_s *item, u32 value) {
    nupad_s *pad = EdControl::Input->pad;
    static_cast<edui_prop_s *>(item)->unknown_property_flags |= 0x20;
    f32 current = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
    f32 dx;
    f32 dy;
    eduiGetCursorDelta(&dx, &dy);
    f32 changed = current - dy * 100.0f - eduiGetAnalougePadValue(pad);
    char text[128];
    sprintf(text, "%.2f", changed);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), text);
    cbChanged(menu, item, value);
}

void EdMatrixControl::cbChanged(eduimenu_s *, eduiitem_s *item, u32) {
    EdMatrixControl *control = static_cast<EdMatrixControl *>(item->data_ptr);
    VuMtx source;
    control->reference->GetMemberData(control->object, EdType_VuMtx, &source, 0);
    f32 changed_value = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
    NUMTX &matrix = source.matrix;
    for (i32 index = 0; index < 3; ++index)
        if (item == control->components[index])
            (&matrix.m30)[index] = changed_value;
    if (control->components[3] || control->components[4] || control->components[5] || control->components[6] ||
        control->components[7] || control->components[8]) {
        NUMTX rebuilt;
        NuMtxSetIdentity(&rebuilt);
        if (control->components[6] && control->components[7] && control->components[8]) {
            NUVEC scale = {NuAToF(static_cast<edui_prop_s *>(control->components[6])->property_text),
                           NuAToF(static_cast<edui_prop_s *>(control->components[7])->property_text),
                           NuAToF(static_cast<edui_prop_s *>(control->components[8])->property_text)};
            NuMtxScale(&rebuilt, &scale);
        }
        {
            NUANG x = static_cast<NUANG>(NuAToF(static_cast<edui_prop_s *>(control->components[3])->property_text) *
                                         (65536.0f / 360.0f));
            NUANG y = static_cast<NUANG>(NuAToF(static_cast<edui_prop_s *>(control->components[4])->property_text) *
                                         (65536.0f / 360.0f));
            NUANG z = static_cast<NUANG>(NuAToF(static_cast<edui_prop_s *>(control->components[5])->property_text) *
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
        rebuilt.m30 = matrix.m30;
        rebuilt.m31 = matrix.m31;
        rebuilt.m32 = matrix.m32;
        matrix = rebuilt;
    }
    matrix.m33 = 1.0f;
    control->reference->SetMemberData(control->object, EdType_VuMtx, &source, 0, nullptr);
    char text[32];
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

EdStringControl::~EdStringControl() {
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

template <> EdValueControl<f32>::~EdValueControl() {
}

template <> inline void EdValueControl<f32>::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdValueControl<f32>));
}

EdFloatControl::~EdFloatControl() {
}

inline void EdFloatControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdFloatControl));
}

template <> void EdValueControl<f32>::AddMenuItem(eduimenu_s *menu, EdRef *member, void *target) {
    EdValueControl<f32> *control =
        new (theMemoryManager.AllocPool(sizeof(EdValueControl<f32>), 1)) EdValueControl<f32>();
    control->value_type = value_type;
    control->format = format;
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

template <> void EdValueControl<f32>::cbButton(eduimenu_s *, eduiitem_s *item, u32) {
    EdValueControl<f32> *control = static_cast<EdValueControl<f32> *>(item->data_ptr);
    f32 sensitivity = control->maximum - control->minimum < 5.0f ? 0.001f : 0.01f;
    static_cast<edui_prop_s *>(item)->unknown_property_flags |= 0x20;
    f32 value = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
    nupad_s *pad = EdControl::Input->pad;
    f32 change = 0.0f;
    if (pad && (pad->digital_buttons & EDUI_CURSOR_PRIMARY)) {
        if (pad->analog_right_y > 128)
            change = 10.0f * sensitivity * (pad->analog_right_y - 128.0f);
        else if (pad->analog_right_y < 128)
            change = -10.0f * sensitivity * (128.0f - pad->analog_right_y);
        if (pad->analog_left_y > 128)
            change = 0.1f * sensitivity * (pad->analog_left_y - 128.0f);
        else if (pad->analog_left_y < 128)
            change = -0.1f * sensitivity * (128.0f - pad->analog_left_y);
    } else {
        f32 dx = 0.0f;
        f32 dy = 0.0f;
        eduiGetCursorDelta(&dx, &dy);
        change = dy * MouseScale;
    }
    value -= change;
    char text[128];
    sprintf(text, control->format, value);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), text);
    value = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
    if (value < control->minimum)
        value = control->minimum;
    if (value > control->maximum)
        value = control->maximum;
    control->reference->SetMemberData(control->object, control->value_type, &value, 0, NULL);
    sprintf(text, control->format, value);
    eduiItemPropSetText(static_cast<edui_prop_s *>(item), text);
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
    f32 *values = &vector.x;
    static char *names[3] = {const_cast<char *>("tx"), const_cast<char *>("ty"), const_cast<char *>("tz")};
    for (i32 index = 0; index < 3; ++index) {
        sprintf(value, "%.2f", values[index]);
        control->components[index] = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, cbSelected,
                                                        cbChanged, cbButton, 2, names[index], value);
        control->components[index]->unknown_10 = index + 1;
        eduiItemExpanderAddChild(static_cast<edui_expander_s *>(control->item), control->components[index]);
    }
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

EdVectorControl::~EdVectorControl() {
    Destroy();
}

inline void EdVectorControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdVectorControl));
}

void EdVectorControl::Refresh() {
    VuVec vector;
    reference->GetMemberData(object, EdType_VuVec, &vector, 0);
    f32 *values = &vector.x;
    char value[128];
    for (i32 index = 0; index < 3; ++index) {
        sprintf(value, "%.2f", values[index]);
        eduiItemPropSetText(static_cast<edui_prop_s *>(components[index]), value);
    }
}

void EdVectorControl::cbButton(eduimenu_s *menu, eduiitem_s *item, u32 value) {
    nupad_s *pad = EdControl::Input->pad;
    static_cast<edui_prop_s *>(item)->unknown_property_flags |= 0x20;
    f32 current = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
    f32 dx;
    f32 dy;
    eduiGetCursorDelta(&dx, &dy);
    f32 changed = current - dy * 100.0f - eduiGetAnalougePadValue(pad);
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
    for (i32 index = 0; index < 3; ++index)
        if (item == control->components[index]) {
            value = NuAToF(static_cast<edui_prop_s *>(item)->property_text);
            (&vector.x)[index] = value;
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
    first_object = NULL;
    last_object = NULL;
    object_count = 0;
}

void KnotHelper::Flush() {
}

f32 EdClassInterface::DistanceToObject(VuVec &origin, VuVec &direction, void *object, EdRef **reference) {
    f32 radius = 1.0f;
    EdMember member;
    f32 distance = 3.402823466e38f;
    if (object_class->FindMember(&member, object, 8, 1)) {
        VuVec position;
        member.reference->GetAttributeData(member.object, 8, EdType_VuVec, &position, 0);
        if (object_class->FindMember(&member, object, 0x40, 1))
            member.reference->GetAttributeData(member.object, 0x40, EdType_Float, &radius, 0);
        f32 surface_distance = LineToPointDistance(origin, direction, position, NULL) - radius;
        distance = surface_distance >= 0.0f ? surface_distance : 0.0f;
    }
    if (reference != NULL)
        *reference = NULL;
    return distance;
}

f32 EdClassInterface::DistanceToObject(VuVec &point, void *object, EdRef **reference) {
    f32 radius = 0.0f;
    EdMember member;
    f32 distance = 3.402823466e38f;
    if (object_class->FindMember(&member, object, 8, 1)) {
        VuVec position;
        member.reference->GetAttributeData(member.object, 8, EdType_VuVec, &position, 0);
        if (object_class->FindMember(&member, object, 0x40, 1))
            member.reference->GetAttributeData(member.object, 0x40, EdType_Float, &radius, 0);
        NUVEC delta{point.x - position.x, point.y - position.y, point.z - position.z};
        f32 surface_distance = NuVecMag(&delta) - radius;
        distance = surface_distance >= 0.0f ? surface_distance : 0.0f;
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
    if (swap_endianness && size > 1 && count > 0) {
        if (size == 2) {
            for (i32 i = 0; i < count; ++i) {
                EdFileSwapEndianess16(data);
                data = static_cast<u8 *>(data) + 2;
            }
        } else if (size == 4) {
            for (i32 i = 0; i < count; ++i) {
                EdFileSwapEndianess32(data);
                data = static_cast<u8 *>(data) + 4;
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
        u8 *cursor = static_cast<u8 *>(data);
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
    if (swap_endianness) {
        for (i32 i = 0; i < count; i++) {
            if (size == 2) {
                EdFileSwapEndianess16(data);
                data = static_cast<u8 *>(data) + 2;
            } else if (size == 4) {
                EdFileSwapEndianess32(data);
                data = static_cast<u8 *>(data) + 4;
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
    menu = NULL;
}

static EdSpecialObjectControl *active_special_object_control;

static i32 SpecialObjectFilter(void *object) {
    return static_cast<Placeable *>(object)->scene_id == theSceneObjectHelper.scene_id;
}

void EdSpecialObjectControl::cbButton(eduimenu_s *parent, eduiitem_s *item, u32) {
    EdSpecialObjectControl *control = static_cast<EdSpecialObjectControl *>(item->data_ptr);
    active_special_object_control = control;
    eduimenu_s *choices =
        eduiMenuCreate(parent->x + item->x, item->y, 180, 250, reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)),
                       cbEdLevelDestroy, NULL);
    if (choices == NULL)
        return;
    nuhspecial_s selected;
    control->reference->GetMemberData(control->object, EdType_NuHSpecial, &selected, 0);
    eduiMenuAddItem(choices, eduiItemSelCreate(static_cast<usize>(-1), item->colours, 0, 0, cbSelectObject,
                                               const_cast<char *>("None")));
    for (Placeable *object = static_cast<Placeable *>(thePlaceableHelper.GetNextObject(NULL, SpecialObjectFilter));
         object != NULL;
         object = static_cast<Placeable *>(thePlaceableHelper.GetNextObject(object, SpecialObjectFilter))) {
        eduiMenuAddItem(choices, eduiItemSelCreate(reinterpret_cast<usize>(object), item->colours, 0, 0, cbSelectObject,
                                                   const_cast<char *>(object->GetName())));
        if (NuSpecialCompare(&static_cast<SpecialObject *>(object)->special, &selected) != 0)
            choices->selected = choices->last;
    }
    eduiMenuSortItemsByTxt(choices);
    choices->flags |= 1;
    eduiMenuAttach(parent, choices);
    eduiMenuFitWidth(choices, 5);
    eduiMenuFitOnScreen(choices, 30);
    item->flags &= ~8;
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
    if (item->data == -1) {
        NuSpecialClear(&selected);
        choice = &selected;
    } else {
        choice = &static_cast<SpecialObject *>(item->data_ptr)->special;
        char *name = NuSpecialGetName(choice);
        if (name != NULL)
            eduiItemPropSetText(reinterpret_cast<edui_prop_s *>(active_special_object_control->item), name);
    }
    EdSpecialObjectControl *control = active_special_object_control;
    control->reference->SetMemberData(control->object, EdType_NuHSpecial, choice, 0, NULL);
}

void EdSpecialObjectControl::AddMenuItem(eduimenu_s *parent, EdRef *member, void *target) {
    void *memory = theMemoryManager.AllocPool(sizeof(EdSpecialObjectControl), 1);
    EdSpecialObjectControl *control = new (memory) EdSpecialObjectControl();
    control->reference = member;
    control->object = target;
    nuhspecial_s special;
    memset(&special, 0, sizeof(special));
    member->GetMemberData(target, EdType_NuHSpecial, &special, 0);
    char *name = NuSpecialGetName(&special);
    if (name == NULL)
        name = const_cast<char *>("None");
    control->item = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected, cbChanged,
                                       cbButton, 1, member->name, name);
    eduiMenuAddItem(parent, control->item);
}

void EdSpecialObjectControl::Process(EdInputContext &) {
}

void EdSpecialObjectControl::Render() {
    if (menu == NULL)
        return;
    Placeable *object = reinterpret_cast<Placeable *>(menu);
    VuVec center = *object->GetCurrentPosition();
    f32 radius = object->GetRadius();
    EdDrawBegin(0);
    EdDrawLineSphere(center, radius, 1.0f, static_cast<i32>(0x80808080));
    EdDrawEnd();
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

EdClassObjectNameControl::EdClassObjectNameControl()
    : selected_class(NULL), selected_object(NULL), selected_reference(NULL) {
}

EdClassObjectNameControl::~EdClassObjectNameControl() {
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
    eduimenu_s *menu = eduiMenuCreate(parent->x + item->x, item->y, 180, 250,
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
    item->flags &= ~8;
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
    edClassObjectNameControl->selected_class = ed_class;
    eduimenu_s *menu = eduiMenuCreate(parent->x + item->x, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (menu == NULL)
        return;
    EdRef *name_reference = ed_class->FindTypeRef(2, 1);
    if (name_reference != NULL) {
        EdClassInterface *interface = ed_class->interface;
        for (void *object = interface->vtable->get_next_object(interface, NULL); object != NULL;
             object = interface->vtable->get_next_object(interface, object)) {
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
        control->selected_object = item->data_ptr;
        char name[128];
        const char *value = "None";
        if (control->selected_object != NULL) {
            ClassObject selected = {control->selected_class, control->selected_object, control->selected_reference};
            selected.GetName(name, sizeof(name));
            value = name;
        }
        eduiItemPropSetText(static_cast<edui_prop_s *>(control->item), const_cast<char *>(value));
        control->SetVal(value);
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

EdSubSystem::~EdSubSystem() {
}

__attribute__((weak)) void EdSubSystem::SubInitialise(variptr_u &, variptr_u &, i32) {
}

__attribute__((weak)) void EdSubSystem::SubReset() {
}

__attribute__((weak)) void EdSubSystem::SubProcess(float) {
}

__attribute__((weak)) void EdSubSystem::SubRender() {
}

EdControl::~EdControl() {
}

inline void EdControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdControl));
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

i32 EdControl::SelectSubObject() {
    for (ClassObjectListEntry *entry = theClassEditor.selected_objects.first; entry; entry = entry->next) {
        if (entry->object != object)
            continue;
        ClassObject selection = {entry->ed_class, entry->object, reference};
        if (selection.object != NULL) {
            i32 mode = static_cast<i32>(Input->GetHold(16)) >= 1 ? 1 : 2;
            theClassEditor.SelectObject(selection, mode);
            if (!theClassEditor.selected_objects.IsInList(selection.object, NULL))
                theClassEditor.SelectObject(selection, 1);
        }
        return 1;
    }
    return 1;
}

void EdControl::Refresh() {
}

void EdControl::SetMenuItemAttr(i32 mask, eduiitem_s *menu_item, eduiiattr_s *selected, eduiiattr_s *unselected) {
    if (reference->attributes & mask) {
        i32 subobject_count = 0;
        for (ClassObjectListEntry *entry = theClassEditor.selected_objects.first; entry; entry = entry->next) {
            if (entry->object != object || entry->reference == NULL)
                continue;
            ++subobject_count;
            if (entry->reference == reference) {
                memcpy(menu_item->colours, unselected, sizeof(*unselected));
                return;
            }
        }
        if (subobject_count == 0) {
            memcpy(menu_item->colours, unselected, sizeof(*unselected));
            return;
        }
    }
    memcpy(menu_item->colours, selected, sizeof(*selected));
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

i32 EdManMove::Process(EdInputContext &input, ClassObjectList &selected) {
    EdManipulator::Process(input, selected);
    VuVec average;
    if (selected.GetAveragePosition(average) == 0)
        return 0;
    VuVec first_axis;
    VuVec second_axis;
    i32 axis = SelectAxis(input, average, first_axis, second_axis, NULL);
    theLevelEditor.field_0x2c = AxisColour[axis];
    if (axis == 0)
        return 0;
    if (input.GetHold(3) == 0.0f && input.GetHold(38) == 0.0f)
        return 1;
    const VuVec *delta = reinterpret_cast<VuVec const *>(reinterpret_cast<u8 *>(this) + 0x40);
    for (ClassObjectListEntry *entry = selected.first; entry != NULL; entry = entry->next) {
        VuVec position = VuVec_Zero;
        get_manipulator_attribute(entry, 8, EdType_VuVec, &position);
        if (axis == 7) {
            if (input.GetHold(38) == 0.0f)
                continue;
            position.x = theLevelEditor.background_colour[0];
            position.y = theLevelEditor.background_colour[1];
            position.z = theLevelEditor.background_colour[2];
        } else {
            f32 amount = delta->x * first_axis.x + delta->y * first_axis.y + delta->z * first_axis.z;
            if (amount != 0.0f) {
                position.x += first_axis.x * amount;
                position.y += first_axis.y * amount;
                position.z += first_axis.z * amount;
            }
            if (axis >= 4) {
                f32 second_amount = delta->x * second_axis.x + delta->y * second_axis.y + delta->z * second_axis.z;
                if (second_amount != 0.0f) {
                    position.x += second_axis.x * second_amount;
                    position.y += second_axis.y * second_amount;
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
    return 1;
}

void EdManMove::Render(ClassObjectList &selected) {
    if (selected.count > 2)
        EdManipulator::Render(selected);
    VuVec average;
    if (selected.GetAveragePosition(average) != 0)
        DrawAxis(average, NULL);
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
