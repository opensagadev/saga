#include "gameapi/edtools/edgra.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nucore/nuvideo.h"

#include <string.h>
#include <math.h>

float edanimPlayerAnimDistance(i32 parameter_index);
extern u8 object_switches[0x80];

struct edbridge_s {
    i32 instance_id;
    u8 reserved_04[0x18];
    u8 connection_index;
    u8 reserved_1d[0x44 - 0x1d];
};
DECOMP_ASSERT(sizeof(edbridge_s) == 0x44, "edbridge_s size");

static NUVEC *ed_loc;

extern "C" {
    extern debinftype *effecttypes;
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern i32 edpp_types_used;
    extern usize edpp_page_scene[8];
    extern i32 edpp_page_used[8];
    extern i32 edpp_page_on[8];
    extern PartHeader **DmaDebTypes;
    extern i32 freeDmaDebType;
    i32 LookupDebrisEffect(char *name);
    i32 edbitsLookupSoundFX(char *name);
    void edbitsSoundPlay(NUVEC *position, i32 sound);
    void edanimSoundDestroy(i32 parameter_index, i32 sound_index);
    void AddVariableShotDebrisEffectTimed3(i32, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *);
    extern part_type_s part_types[128];
    extern i32 part_types_used;
    extern i32 part_emits_used;
    extern NUGSCN *part_scene[32];
    extern i32 part_scene_pageid[32];
    i32 FindPlatInst(i32);
    void PlatInstBounce(i32, f32, f32, f32);
    void CheckPartCount(void);
    void KillPartsByScene(NUGSCN *);
    extern i32 DEBPAGE_GENERAL;
    extern i32 DEBPAGE_CHARACTER;
    extern i32 DEBPAGE_AREA;
    part_emit_s part_emits[512];
    i32 part_page_on[8];
    i32 part_page_used[8];
    i32 edpart_instances_used;
    edanim_param_s AnimParams[64];
    i32 edbits_anim_page;
    i32 edanim_particle_mode;
    i32 edanim_sound_mode;
    i32 edanim_next_param;
    i32 edanim_params_used;
    i32 edanim_page_on[8];
    i32 edanim_page_used[8];
    NUGSCN *edanim_page_scene[8];
    i32 edanim_nearest;
    i32 edanim_nearest_param_id;
    i32 edanim_sound_type;
    NUGSCN *edbits_base_scene;
    edbridge_s edBridges[64];
    i32 edbri_bridges_used;
    i32 edbri_page_on[8];
    i32 edbri_page_scene[8];
    i32 edbri_page_used[8];
}

void FileLoadSingleEffectType(debinftype *, i32, char);
extern "C" void NuBridgeInit(void);

void edanimDetermineNearestAnim(f32);
void edppDetermineNearest(float);
void edppPtlDestroy(i32);
extern "C" {
    extern edpp_particle_s edpp_ptls[512];
    extern i32 edpp_nearest;
    extern debkeydatatype_s *debkeydata;
    extern i32 maxdebkeys;
    void DebFreeOrphansInstantly(debinftype *);
    i32 LookupDebrisEffectPageIgnore(char *, i32, i32);
    void DebFreeInstantly(i32 *);
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

static NUCAMERA *edmaincam = NULL;
static NUCAMERA *edinternalcam = NULL;

extern "C" {
    i32 PadFlyMode = 0;
    NUMTX *ed_remap_mtx = NULL;
    i32 edmain_cursor_enabled = 0;

    void EdFileBackup(void) {
    }
    void EdFileReadMemCard(void) {
    }
    void EdFileWriteChar(void) {
    }
    void EdFileWriteFloat(void) {
    }
    void EdFileWriteInt(void) {
    }
    void EdFileWriteMemCard(void) {
    }
    void EdFileWriteNuVec(void) {
    }
    void EdFileWriteShort(void) {
    }
    void EdFileWriteUnsignedChar(void) {
    }
    void EdFileWriteUnsignedInt(void) {
    }
    void EdFileWriteUnsignedShort(void) {
    }
    void edDrawCross(void) {
    }
    void edGetMainMenu(void) {
    }
    void edGraDisableTerrainSwap(void) {
    }
    void edGraEnableTerrainSwap(void) {
    }
    void edGraInitTerrainSwapProtection(void) {
    }
    i32 edanimLoadPage(char *path, NUGSCN *scene) {
        i32 page;
        if (edanim_page_used[0] == 0)
            page = 0;
        else if (edanim_page_used[1] == 0)
            page = 1;
        else if (edanim_page_used[2] == 0)
            page = 2;
        else if (edanim_page_used[3] == 0)
            page = 3;
        else if (edanim_page_used[4] == 0)
            page = 4;
        else if (edanim_page_used[5] == 0)
            page = 5;
        else if (edanim_page_used[6] == 0)
            page = 6;
        else if (edanim_page_used[7] == 0)
            page = 7;
        else
            return -1;
        EdFileSetMedia(1);
        if (EdFileOpen(path, NUFILE_READ) == 0)
            return -1;
        EdFileSetReadWrongEndianess(1);
        i32 version = EdFileReadInt();
        if (version > 6) {
            EdFileSetReadWrongEndianess(0);
            EdFileClose();
            return -1;
        }
        i32 count = EdFileReadInt();
        if (count + edanim_params_used > 64)
            count = 64 - edanim_params_used;
        i32 slot = 0;
        for (i32 index = 0; index < count; ++index) {
            while (AnimParams[slot].instance_id != -1 && slot < 64)
                ++slot;
            if (slot >= 64)
                continue;
            char name[20];
            EdFileRead(name, 20);
            edanim_param_s *param = &AnimParams[slot];
            param->instance_id = edanimLookupSpecial(name, scene);
            param->effect_count = EdFileReadInt();
            param->sound_count = version > 1 ? EdFileReadInt() : 0;
            param->field_00c = EdFileReadInt();
            param->field_010 = EdFileReadInt();
            param->field_014 = EdFileReadFloat();
            param->field_018 = EdFileReadFloat();
            if (param->effect_count > 8)
                param->effect_count = 8;
            for (i32 effect = 0; effect < param->effect_count; ++effect) {
                EdFileRead(param->effect_names[effect], 16);
                param->effect_ids[effect] = -1;
                if (version > 4) {
                    param->effect_intervals[effect] = version == 5 ? static_cast<i32>(EdFileReadFloat()) : EdFileReadInt();
                } else {
                    i32 interval = EdFileReadInt();
                    param->effect_intervals[effect] = interval > 0 ? interval * 60 : interval == 0 ? 0 : -60 / interval;
                }
                param->effect_flags[effect] = EdFileReadInt();
                EdFileReadNuVec(reinterpret_cast<NUVEC *>(param->effect_positions[effect]));
                if (version > 2) {
                    param->effect_angles[effect] = EdFileReadShort();
                    param->effect_angle_ranges[effect] = EdFileReadShort();
                } else {
                    param->effect_angles[effect] = 0;
                    param->effect_angle_ranges[effect] = 0;
                }
            }
            param->field_17c = 0.99f;
            if (version > 1) {
                if (param->sound_count > 8)
                    param->sound_count = 8;
                for (i32 sound = 0; sound < param->sound_count; ++sound) {
                    EdFileRead(param->sound_names[sound], 16);
                    param->sound_ids[sound] = -1;
                    param->sound_flags[sound] = EdFileReadInt();
                    param->sound_values[sound] = EdFileReadFloat();
                    EdFileReadNuVec(reinterpret_cast<NUVEC *>(param->sound_positions[sound]));
                }
            }
            nuhspecial_s special;
            NuGScnGetSpecial(&special, scene, param->instance_id);
            param->platform_id = FindPlatInst(NuSpecialGetInstanceix(&special));
            if (version > 3) {
                param->bounce_impulse = EdFileReadFloat();
                param->bounce_spring = EdFileReadFloat();
                param->bounce_damping = EdFileReadFloat();
            } else {
                param->bounce_impulse = 0.0f;
                param->bounce_spring = 0.0f;
                param->bounce_damping = 0.0f;
            }
            if (param->platform_id != -1)
                PlatInstBounce(param->platform_id, param->bounce_impulse, param->bounce_spring, param->bounce_damping);
            param->page = static_cast<i8>(page);
            ++edanim_params_used;
        }
        EdFileSetReadWrongEndianess(0);
        EdFileClose();
        edanim_page_used[page] = 1;
        edanim_page_scene[page] = scene;
        edbits_anim_page = page;
        edanim_next_param = count;
        edanim_particle_mode = 0;
        edanim_nearest = -1;
        edanim_nearest_param_id = -1;
        edanimDetermineNearestAnim(1.0f);
        return page;
    }

    void edanimClearPage(i32 page) {
        if (edanim_page_on[page] != 0)
            edanimStopPage(page);
        for (i32 index = 0; index < 64; ++index) {
            if (AnimParams[index].page == page) {
                AnimParams[index].instance_id = -1;
                --edanim_params_used;
            }
        }
        edanim_page_used[page] = 0;
        edanim_page_scene[page] = NULL;
    }
    i32 edanimLookupSpecial(char *name, NUGSCN *scene) {
        if (scene != NULL) {
            nuhspecial_s special;
            for (i32 index = 0; index < NuGScnNumSpecials(scene); ++index) {
                NuGScnGetSpecial(&special, scene, index);
                if (NuStrNICmp(NuSpecialGetName(&special), name, 19) == 0)
                    return index;
            }
        }
        return -1;
    }
    void edanimParamReset(void) {
        for (i32 i = 0; i < 64; ++i) {
            AnimParams[i].instance_id = -1;
        }
        edanim_next_param = 0;
        edanim_params_used = 0;
        memset(edanim_page_used, 0, sizeof(edanim_page_used));
        memset(edanim_page_on, 0, sizeof(edanim_page_on));
    }
    void edanimParticleDestroy(i32 parameter_index, i32 particle_index) {
        edanim_param_s *parameters = AnimParams;
        for (i32 index = particle_index; index < parameters[parameter_index].effect_count - 1; ++index) {
            parameters[parameter_index].effect_ids[index] = parameters[parameter_index].effect_ids[index + 1];
            parameters[parameter_index].effect_intervals[index] = parameters[parameter_index].effect_intervals[index + 1];
            parameters[parameter_index].effect_flags[index] = parameters[parameter_index].effect_flags[index + 1];
            memcpy(parameters[parameter_index].effect_positions[index],
                   parameters[parameter_index].effect_positions[index + 1], sizeof(NUVEC));
            strcpy(parameters[parameter_index].effect_names[index], parameters[parameter_index].effect_names[index + 1]);
        }
        --parameters[parameter_index].effect_count;
    }
    void edanimRegisterCubeDumpInfo(void) {
    }
    void edanimStartPage(i32 page) {
        if (edanim_page_used[page] != 0 && edanim_page_scene[page] != NULL && edanim_page_on[page] == 0)
            edanim_page_on[page] = 1;
    }
    void edanimStopPage(i32 page) {
        edanim_page_on[page] = 0;
    }
    void edanimUpdateObjects(float elapsed) {
        static i32 localframecount;
        float seconds;
        if (NuVideoGetMode() == 3)
            seconds = elapsed / 50.0f;
        else
            seconds = elapsed / 60.0f;
        for (i32 index = 0; index < 64; ++index) {
            edanim_param_s *param = &AnimParams[index];
            if (param->instance_id == -1 || edanim_page_on[param->page] == 0)
                continue;
            nuhspecial_s special;
            nuinstanim_s *animation = NULL;
            if (edanim_page_scene[param->page] != NULL) {
                NuGScnGetSpecial(&special, edanim_page_scene[param->page], param->instance_id);
                animation = NuSpecialGetInstAnim(&special);
                NuSpecialGetInstanceix(&special);
            }
            if (animation != NULL) {
                switch (param->field_00c) {
                    case 1:
                        animation->playing = 0;
                        if (param->field_010 != -1 && object_switches[param->field_010] != 0)
                            animation->playing = 1;
                        break;
                    case 2:
                        animation->repeating = 0;
                        if (param->field_010 != -1 && object_switches[param->field_010] != 0 && !animation->playing) {
                            animation->ltime = 1.0f;
                            animation->playing = 1;
                            animation->backwards = 0;
                            animation->waiting = 0;
                        }
                        break;
                    case 3:
                        animation->repeating = 1;
                        if (param->field_010 != -1 && object_switches[param->field_010] != 0)
                            animation->playing = 1;
                        break;
                    case 4:
                        animation->playing = param->field_014 > edanimPlayerAnimDistance(index);
                        break;
                    case 5:
                        animation->repeating = 0;
                        if (param->field_014 > edanimPlayerAnimDistance(index) && !animation->playing) {
                            animation->playing = 1;
                            animation->backwards = 0;
                            animation->waiting = 0;
                            animation->ltime = 1.0f;
                        }
                        break;
                    case 6:
                        animation->repeating = 1;
                        if (param->field_014 > edanimPlayerAnimDistance(index))
                            animation->playing = 1;
                        break;
                    case 10:
                        animation->ltime = 1.0f;
                        animation->playing = 0;
                        break;
                    case 11:
                        animation->playing = 1;
                        break;
                    case 12:
                        animation->playing = 1;
                        animation->repeating = 1;
                        break;
                }
                if (index == edanim_nearest_param_id && (edanim_particle_mode != 0 || edanim_sound_mode != 0))
                    animation->ltime = 1.0f;
            }
            if (NuSpecialGetVisibilityFn(&special) != NULL) {
                i32 particle_index = 0;
                while (particle_index < param->effect_count) {
                    i32 effect = param->effect_ids[particle_index];
                    if (effect != -1 && debtab[effect] != NULL) {
                        if (param->effect_flags[particle_index] == 0 || (animation != NULL && animation->playing)) {
                            NUMTX matrix;
                            NuMtxInvRSS(&matrix, NuSpecialGetMtx(&special));
                            NuMtxMul(&matrix, &matrix, NuSpecialGetDrawMtx(&special));
                            NUVEC position = *reinterpret_cast<NUVEC *>(param->effect_positions[particle_index]);
                            NuVecMtxTransform(&position, &position, &matrix);
                            NUMTX orientation = numtx_identity;
                            NuMtxRotateZ(&orientation, param->effect_angles[particle_index]);
                            NuMtxRotateY(&orientation, param->effect_angle_ranges[particle_index]);
                            NuMtxMul(&matrix, &orientation, &matrix);
                            AddVariableShotDebrisEffectTimed3(param->effect_ids[particle_index], &position, &nuvec_zero,
                                                             param->effect_intervals[particle_index], seconds, &matrix,
                                                             &numtx_identity);
                        }
                    } else if (param->effect_names[particle_index][0] != 0) {
                        param->effect_ids[particle_index] = LookupDebrisEffect(param->effect_names[particle_index]);
                        if (param->effect_ids[particle_index] == -1) {
                            edanimParticleDestroy(index, particle_index);
                            continue;
                        }
                    }
                    ++particle_index;
                }
                for (i32 sound_index = 0; sound_index < param->sound_count; ++sound_index) {
                    i32 sound = param->sound_ids[sound_index];
                    if (sound == -1) {
                        if (param->sound_names[sound_index][0] != 0) {
                            param->sound_ids[sound_index] = edbitsLookupSoundFX(param->sound_names[sound_index]);
                            if (param->sound_ids[sound_index] == -1) {
                                edanimSoundDestroy(index, sound_index);
                                --sound_index;
                            }
                        }
                        continue;
                    }
                    i32 play = -1;
                    if (param->sound_flags[sound_index] == 1) {
                        i32 interval = static_cast<i32>(param->sound_values[sound_index]);
                        if (static_cast<i32>(static_cast<float>(localframecount) + elapsed) / interval > localframecount / interval)
                            play = sound;
                    } else if (animation != NULL) {
                        float frame = param->sound_values[sound_index];
                        if (animation->ltime >= frame && frame > param->field_17c)
                            play = sound;
                        if (animation->oscillate && frame >= animation->ltime && fabsf(param->field_17c) > frame)
                            play = sound;
                    }
                    if (play != -1) {
                        NUMTX matrix;
                        NuMtxInvRSS(&matrix, NuSpecialGetMtx(&special));
                        NuMtxMul(&matrix, &matrix, NuSpecialGetDrawMtx(&special));
                        // The original uses the completed particle-loop index here (0x35d332).
                        NUVEC position = *reinterpret_cast<NUVEC *>(param->sound_positions[particle_index]);
                        NuVecMtxTransform(&position, &position, &matrix);
                        edbitsSoundPlay(&position, play);
                    }
                }
            }
            if (animation != NULL)
                param->field_17c = animation->oscillate ? -animation->ltime : animation->ltime;
        }
        localframecount += static_cast<i32>(elapsed);
    }
    void edbitsDrawBBox(void) {
    }
    void edbitsDrawBasicCube(void) {
    }
    void edbitsDrawCircleTilted(void) {
    }
    void edbitsDrawCircleXY(void) {
    }
    void edbitsDrawCross(void) {
    }
    void edbitsDrawCube(void) {
    }
    void edbitsDrawDiagonalCross(void) {
    }
    void edbitsDrawOvalTilted(void) {
    }
    void edbitsDrawOvalXY(void) {
    }
    void edbitsDrawSphere(void) {
    }
    void edbitsDrawTorus(void) {
    }
    char *edbitsGetSoundName(i32) {
    }
    void edbitsLookupInstance(void) {
    }
    void edbitsLookupSound(void) {
    }
    i32 edbitsLookupSoundFX(char *) {
        return -1;
    }
    void edbitsProcessCubemapDump(void) {
    }
    void edbitsRegisterDataPath(void) {
    }
    i32 edbits_editmode;
    static i32 edbits_local_editor_enabled;
    i32 *edbits_editor_enabled = &edbits_local_editor_enabled;

    void edbitsRegisterEditMode(i32 mode) {
        edbits_editmode = mode;
    }
    void edbitsRegisterEditorEnabledFlag(i32 *enabled) {
        edbits_editor_enabled = enabled;
    }
    void edbitsRegisterLevel(void) {
    }
    void edbitsRegisterPlaySound(void) {
    }
    void edbitsRegisterRequestSound(void) {
    }
    void edbitsRegisterSaveFormat(void) {
    }
    void edbitsRegisterSoundEffect(void) {
    }
    void edbitsRegisterThingsScene(NUGSCN *) {
    }
    void edbitsSetSoundFxVolume(void) {
    }
    void edbitsStartCubemapDump(void) {
    }
    void edbitsVector2YZRot(void) {
    }
    void edbriBridgesReset(void) {
        NuBridgeInit();
        for (i32 i = 0; i < 64; ++i) {
            edBridges[i].instance_id = -1;
            edBridges[i].connection_index = 0xff;
        }
        memset(edbri_page_used, 0, sizeof(edbri_page_used));
        memset(edbri_page_scene, 0, sizeof(edbri_page_scene));
        memset(edbri_page_on, 0, sizeof(edbri_page_on));
        edbri_bridges_used = 0;
    }
    void edbriClearPage(i8) {
    }
    void edbriStartAllPages(void) {
    }
    void edbriStopPage(void) {
    }
    f32 edcamGetDist(void) {
        return gp_cam.distance;
    }
    edcam_s *edcamGetEdCam(void) {
        return &gp_cam;
    }
    void edcamGetOffset(NUVEC *offset) {
        *offset = gp_cam.offset;
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
    NUVEC *edcamGetPosPointer(void) {
        return &gp_cam.position;
    }
    void edcamMove(nupad_s *pad) {
        edcamMoveEx(pad, NuTimeGetFrameTime());
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
    void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance) {
        gp_cam.allow_position_x = position_x;
        gp_cam.allow_position_y = position_y;
        gp_cam.allow_position_z = position_z;
        gp_cam.allow_pitch = pitch;
        gp_cam.allow_yaw = yaw;
        gp_cam.allow_distance = distance;
    }
    void edcamSetAng(i32 pitch, i32 yaw) {
        gp_cam.pitch = pitch;
        gp_cam.yaw = yaw;
    }
    void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale) {
        gp_cam.auto_move_base = move_base;
        gp_cam.auto_move_dist_scale = move_distance_scale;
        gp_cam.auto_zoom_base = zoom_base;
        gp_cam.auto_zoom_dist_scale = zoom_distance_scale;
    }
    void edcamSetDist(f32 distance) {
        gp_cam.distance = distance;
    }
    void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement) {
        gp_cam.mouse_pitch_speed = pitch;
        gp_cam.mouse_yaw_speed = yaw;
        gp_cam.mouse_move_speed = movement;
    }
    void edcamSetOffset(NUVEC *offset) {
        gp_cam.offset = *offset;
    }
    void edcamSetPos(NUVEC *position) {
        gp_cam.position = *position;
        gp_cam.offset = {0.0f, 0.0f, 0.0f};
    }
    void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw) {
        gp_cam.position = *position;
        gp_cam.offset = {0.0f, 0.0f, 0.0f};
        gp_cam.pitch = pitch;
        gp_cam.yaw = yaw;
    }
    void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance) {
        gp_cam.position_speed = {position_x, position_y, position_z};
        gp_cam.distance_speed = distance;
    }
    void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z) {
        gp_cam.position_speed = {position_x, position_y, position_z};
    }
    void edgraBufferUsage(void) {
    }
    void edgraClearPage(i8) {
    }
    void edgraClumpsReset(void) {
    }
    void edgraInitAllClumps(void) {
    }
    void edgraSetMemoryBuffer(void) {
    }
    void edgraSetThinning(void) {
    }
    void edgraSetup(VARIPTR *, VARIPTR, i32, i32, i32) {
    }
    void edgraStopPage(i8) {
    }
    void edmainActivate(void) {
    }
    void edmainClose(void) {
    }
    void edmainCurrent(void) {
    }
    void edmainExtCamera(NUCAMERA *camera) {
        edmaincam = camera != NULL ? camera : edinternalcam;
    }
    NUCAMERA *edmainGetCamera(void) {
        return edmaincam;
    }
    i32 edmainGetCursorEnabled(void) {
        return edmain_cursor_enabled;
    }
    void edmainInit(void) {
    }
    void edmainInitEx(void) {
    }
    void edmainProcess(void) {
    }
    NUVEC *edmainQueryLocVec(void) {
        return ed_loc;
    }
    void edmainRegister(void) {
    }
    void edmainRegisterLocVec(NUVEC *position) {
        ed_loc = position;
    }
    void edmainRender(void) {
    }
    void edmainSetCamera(NUMTX *matrix) {
        edmaincam->mtx = *matrix;
        NuCameraSet(edmaincam);
    }
    void edmainSetCursorEnabled(void) {
    }
    void edmainSetMainMenuScale(void) {
    }
    void edmainSetReturn(void) {
    }
    void edpartClearPage(i8 page) {
        NuThreadDisableThreadSwap();
        CheckPartCount();
        if (part_page_on[page] != 0)
            edpartStopPage(page);
        for (i32 index = 0; index < 128; ++index) {
            if (part_types[index].page == page && part_types[index].name[0] != 0) {
                part_types[index].name[0] = 0;
                part_types[index].effect_ids[0] = -1;
                --part_types_used;
            }
        }
        for (i32 index = 0; index < 40; ++index) {
            if (part_emits[index].page == page && part_emits[index].effect_id != -1) {
                part_emits[index].effect_id = -1;
                --part_emits_used;
            }
        }
        CheckPartCount();
        for (i32 index = 0; index < 32; ++index) {
            if (part_scene_pageid[index] == page) {
                KillPartsByScene(part_scene[index]);
                part_scene_pageid[index] = -1;
                part_scene[index] = NULL;
            }
        }
        NuThreadEnableThreadSwap();
        part_page_used[page] = 0;
    }
    void edpartDestroyAllParticles(void) {
    }
    void edpartParticleReset(void) {
        part_emit_s *emit = part_emits;
        part_emit_s *const emit_end = part_emits + 512;
        do {
            emit->instance_id = -1;
        } while (++emit != emit_end);
        memset(part_page_used, 0, sizeof(part_page_used));
        memset(part_page_on, 0, sizeof(part_page_on));
        edpart_instances_used = 0;
    }
    void edpartRegisterPointerToGameCharLocation(NUVEC *position) {
        edmainRegisterLocVec(position);
    }
    void edppClearPage(i8 page) {
        edpp_page_on[page] = 0;
        edpp_page_used[page] = 0;
        for (i32 index = 0; index < 512; ++index) {
            if (edpp_ptls[index].page == page)
                edppPtlDestroy(index);
        }
        for (i32 index = 1; index < EDPP_MAX_TYPES; ++index) {
            if (debtab[index] == NULL || debtab[index]->page != static_cast<u8>(page))
                continue;
            debtab[index]->disabled = 1;
            for (i32 key = 0; key < maxdebkeys; ++key) {
                if (debkeydata[key].effect_index == index) {
                    i32 handle = key;
                    DebFreeInstantly(&handle);
                }
            }
            DebFreeOrphansInstantly(debtab[index]);
            if (debtab[index]->native_data != NULL) {
                DmaDebTypes[--freeDmaDebType] = debtab[index]->native_data;
                debtab[index]->native_data = NULL;
            }
            debtab[index] = NULL;
            --edpp_types_used;
        }
    }
    void edppDeleteEffect(i32 index) {
        if (edpp_ptls[edpp_nearest].effect_index == index)
            edpp_nearest = -1;
        DebFreeOrphansInstantly(debtab[index]);
        i32 replacement = LookupDebrisEffectPageIgnore(debtab[index]->name, 1, index);
        if (replacement != -1) {
            for (i32 i = 0; i < 512; ++i) {
                if (edpp_ptls[i].effect_index == index) {
                    i32 handle = edpp_ptls[i].instance_id;
                    if (handle != 99999 && handle != -1)
                        debkeydata[handle].effect_index = replacement;
                    edpp_ptls[i].effect_index = replacement;
                }
            }
            for (i32 i = 0; i < maxdebkeys; ++i)
                if (debkeydata[i].effect_index == index)
                    debkeydata[i].effect_index = replacement;
        } else {
            for (i32 i = 0; i < 512; ++i)
                if (edpp_ptls[i].effect_index == index)
                    edppPtlDestroy(i);
            for (i32 i = 0; i < maxdebkeys; ++i) {
                if (debkeydata[i].effect_index == index) {
                    i32 handle = i;
                    DebFreeInstantly(&handle);
                }
            }
        }
        debtab[index] = NULL;
        --edpp_types_used;
        edppDetermineNearest(1.0f);
    }
    void edppDestroyAllEffects(void) {
    }
    void edppDestroyAllParticles(void) {
    }
    void edppDrawSpheres(void) {
    }
    void edppDrawTorus(void) {
    }
    void edppFindAllSounds(void) {
    }
    // Parts-page loader (edppLoadPage @0x36c630).  The normal general (0) and
    // character (5) pages only contain effect-type records; instance records
    // are read by the page-1/0 branches in the original and are deliberately
    // not entered here.
    i32 edppLoadPage(char *path, i32 flag, usize scene) {
        (void)scene;
        u8 category;
        i32 page_index;
        if (flag == 0) {
            category = 0;
            page_index = 0;
        } else if (flag == 5) {
            category = 5;
            page_index = 1;
        } else {
            // The remaining page kinds have their own instance-record paths;
            // they are outside the general/character pages recovered here.
            return -1;
        }

        EdFileSetMedia(1);
        if (EdFileOpen(path, NUFILE_READ) == 0) {
            return -1;
        }
        EdFileSetReadWrongEndianess(1);

        i32 version = EdFileReadInt();
        if (version < 5 || version > 41) {
            EdFileSetReadWrongEndianess(0);
            EdFileClose();
            return -1;
        }

        edpp_page_used[page_index] = 1;
        edpp_page_scene[page_index] = scene;

        i32 requested = EdFileReadInt();
        i32 available = EDPP_MAX_TYPES - edpp_types_used;
        if (requested > available) {
            requested = available;
        }
        if (requested < 0) {
            requested = 0;
        }

        for (i32 n = 0; n < requested; n++) {
            i32 index = 1;
            while (index < EDPP_MAX_TYPES && debtab[index] != NULL) {
                index++;
            }
            if (index >= EDPP_MAX_TYPES) {
                break;
            }

            debinftype *effect = &effecttypes[index];
            FileLoadSingleEffectType(effect, version, static_cast<char>(category));
            effect->native_data = NULL;
            effect->last_render_time = 0.0f;
            effect->page = static_cast<u8>(page_index);
            debtab[index] = effect;
            edpp_types_used++;
        }

        EdFileSetReadWrongEndianess(0);
        EdFileClose();
        edppDetermineNearest(1.0f);

        if (flag == 0) {
            DEBPAGE_GENERAL = page_index;
        } else if (flag == 5) {
            DEBPAGE_CHARACTER = page_index;
        }
        return page_index;
    }
    void edppRegisterPointerToGameCharLocation(NUVEC *position) {
        edmainRegisterLocVec(position);
    }
    void edppRestartAllEffectsInLevel(void) {
    }
    void edppSetSaveName(void) {
    }
    void edppStopPage(i32) {
    }
    void edqrand(void) {
    }
    void edrtlBurnoutLoad(void) {
    }
    void edrtlCalculateBurnout(void) {
    }
    void edrtlCalculateBurnoutEx(void) {
    }
    void edrtlDrawFog(void) {
    }
    void edrtlDrawLight(void) {
    }
    void edrtlDrawLightEx(void) {
    }
    void edrtlGetFogSet(void) {
    }
    void eduiAddPropTextPickEnt(void) {
    }
    void eduiAddTextPickEnt(void) {
    }
    void eduiAddTextPickEntEx(void) {
    }
    void eduiCheckForPadMenuCancel(void) {
    }
    void eduiClearActiveMenu(void) {
    }
    void eduiCreate3LineMessageMenu(void) {
    }
    void eduiCreateMessageMenu(void) {
    }
    void eduiCursorOverMenu(void) {
    }
    void eduiFlushInteracts(void) {
    }
    void eduiGetActiveMenu(void) {
    }
    void eduiGetActiveMenuParent(void) {
    }
    void eduiGetAnalougePadValue(void) {
    }
    void eduiGetCameraEnabled(void) {
    }
    void eduiGetCursorCoords(void) {
    }
    void eduiGetCursorDelta(void) {
    }
    void eduiGetTopLevelParent(void) {
    }
    void eduiGetUsingMenuFocus(void) {
    }
    void eduiGradPickRead(void) {
    }
    void eduiGradStageAdd(void) {
    }
    void eduiGradStageAddRGB(void) {
    }
    void eduiGradStageDelete(void) {
    }
    void eduiGradStageSetHSV(void) {
    }
    void eduiGradStageSetRGB(void) {
    }
    void eduiIitemExpanderSetDepth(void) {
    }
    void eduiInit(void) {
    }
    void eduiInitMaterials(void) {
    }
    void eduiItemCheckCreate(void) {
    }
    void eduiItemColourPickCreate(void) {
    }
    void eduiItemColourPickSetHSV(void) {
    }
    void eduiItemColourPickSetRGB(void) {
    }
    void eduiItemColourSliderCreate(void) {
    }
    void eduiItemDataGradPickCreate(void) {
    }
    void eduiItemExpanderAddChild(void) {
    }
    void eduiItemExpanderCreate(void) {
    }
    void eduiItemFilePickCreate(void) {
    }
    void eduiItemFilePickSetFmt(void) {
    }
    void eduiItemFilterAddItem(void) {
    }
    void eduiItemFilterCreate(void) {
    }
    void eduiItemFilterRemoveItem(void) {
    }
    void eduiItemGradPickCreate(void) {
    }
    void eduiItemGraphAddOnionSkin(void) {
    }
    void eduiItemGraphCreate(void) {
    }
    void eduiItemGraphSetCursor(void) {
    }
    void eduiItemGraphSetLabels(void) {
    }
    void eduiItemGreyGradPickCreate(void) {
    }
    void eduiItemGreyPickCreate(void) {
    }
    void eduiItemNumberCreate(void) {
    }
    void eduiItemPropCreate(void) {
    }
    void eduiItemPropCreateEx(void) {
    }
    void eduiItemPropSetText(void) {
    }
    void eduiItemRender(void) {
    }
    void eduiItemSelCreate(void) {
    }
    void eduiItemSelWithClipColourCreate(void) {
    }
    void eduiItemSeparatorCreate(void) {
    }
    void eduiItemSetText(void) {
    }
    void eduiItemSliderCreate(void) {
    }
    void eduiItemSliderCreateInt(void) {
    }
    void eduiItemSliderSetFmt(void) {
    }
    void eduiItemSliderSetGranularity(void) {
    }
    void eduiItemSliderSetVal(void) {
    }
    void eduiItemSliderSetValEx(void) {
    }
    void eduiItemTextPickCreate(void) {
    }
    void eduiItemTextPickSetFmt(void) {
    }
    void eduiItemTextSelectorCreate(void) {
    }
    void eduiItemTexturePickCreate(void) {
    }
    void eduiItemToggleCreate(void) {
    }
    void eduiMenuAddItem(void) {
    }
    void eduiMenuAddItemAfter(void) {
    }
    void eduiMenuAddItemBefore(void) {
    }
    void eduiMenuAddItemFirst(void) {
    }
    void eduiMenuAddItemLast(void) {
    }
    void eduiMenuAttach(void) {
    }
    void eduiMenuCreate(void) {
    }
    void eduiMenuDestroy(void) {
    }
    void eduiMenuDestroyItems(void) {
    }
    void eduiMenuDetach(void) {
    }
    void eduiMenuEnsureSelection(void) {
    }
    void eduiMenuFitOnScreen(void) {
    }
    void eduiMenuFitWidth(void) {
    }
    void eduiMenuHighlight(void) {
    }
    void eduiMenuIsActive(void) {
    }
    void eduiMenuItemMoveDown(void) {
    }
    void eduiMenuItemMoveUp(void) {
    }
    void eduiMenuProcess(void) {
    }
    void eduiMenuProcessAux(void) {
    }
    void eduiMenuProcessInput(void) {
    }
    void eduiMenuProcessSelectedItem(void) {
    }
    void eduiMenuRemoveItem(void) {
    }
    void eduiMenuRender(void) {
    }
    void eduiMenuSelectFirstEntry(void) {
    }
    void eduiMenuSetAttr(void) {
    }
    void eduiMenuSetDisabled(void) {
    }
    void eduiMenuSetTransparency(void) {
    }
    void eduiMenuSortItemsByTxt(void) {
    }
    void eduiProcessCursor(void) {
    }
    void eduiProcessCursorDefault(void) {
    }
    void eduiProcessInteracts(void) {
    }
    void eduiRenderCursor(void) {
    }
    void eduiRenderInteracts(void) {
    }
    void eduiSetActiveMenu(void) {
    }
    void eduiSetCameraEnabled(void) {
    }
    void eduiSetCursorColour(void) {
    }
    void eduiSetCursorCoords(void) {
    }
    void eduiSetDefaultActiveMenu(void) {
    }
    void eduiSetFont(void) {
    }
    void eduiSetFontScale(void) {
    }
    void eduiSetGlobalSliderAccel(void) {
    }
    void eduiSetRenderPlane(void) {
    }
    void eduiSetUsingMenuFocus(void) {
    }
    void eduiShowCursor(void) {
    }
    void eduiUsedAlgPad(void) {
    }
    void eduicbCancelMessageMenu(void) {
    }
    void eduicbInteractSlider(void) {
    }
    void eduicbItemDestroy(void) {
    }
    void eduicbItemDestroyProp(void) {
    }
    void eduicbMenuCloseAllexpanders(void) {
    }
    void eduicbMenuOpenAllexpanders(void) {
    }
}
