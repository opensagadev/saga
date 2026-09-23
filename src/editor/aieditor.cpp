
#include "decomp.h"
#include "editor/edpath.h"
#include "editor/path_connections.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edfile.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/terrain_internal.h"
#include "globals.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuqfnt.h"

#include <string.h>
#include <stdio.h>

extern "C" {
    i32 aidata_version = 20;
}

extern "C" void aieditor_ClearMainMenu(void);
extern "C" void aieditor_SetMode(i32 mode);
extern "C" void AISYSRebuildFromEditorData(void);
extern "C" i32 aieditor_Register(const char *, void (*)(), void (*)(), void (*)(), void (*)());
extern "C" void aieditor_cbCancelMainMenu(eduimenu_s *, eduimenu_s *);
extern "C" f32 AITerrShadow(NUVEC *, f32, f32, i32);
extern "C" i32 AITerrShadowOnPlatform(void);
extern "C" i32 near_clip_at_cursor;
extern "C" void NuRndrRect2di(i32, i32, i32, i32, i32, NUMTL *);
extern "C" void NuRndrLine2di(i32, i32, i32, i32, i32, NUMTL *);
extern "C" void areaEditorSaveData(void);
extern "C" void locatorEditorSaveData(AIPATHSYS_s *);
extern "C" void creatureEditorSaveData(AIPATHSYS_s *);
extern "C" void antinodeEditorSaveData(void);
extern "C" void (*GameAISaveFn)(void);
void NuSpecialFindByPlatformID(nugscn_s *, nuhspecial_s *, i32);

struct nupad_s;
void pathEditor_Enter();
void pathEditor_Render(i32, i32, f32, f32);
eduimenu_s *routeEditor_Process(nupad_s *);
void routeEditor_Render(i32, i32, f32, f32);
void areaEditor_Enter();
eduimenu_s *areaEditor_Process(nupad_s *);
void areaEditor_Render(i32, i32, f32, f32);
void locatorEditor_Enter();
eduimenu_s *locatorEditor_Process(nupad_s *);
void locatorEditor_Render(i32, i32, f32, f32);
void creatureEditor_Enter();
eduimenu_s *creatureEditor_Process(nupad_s *);
void creatureEditor_Render(i32, i32, f32, f32);
void antinodeEditor_Enter();
eduimenu_s *antinodeEditor_Process(nupad_s *);
void antinodeEditor_Render(i32, i32, f32, f32);

extern "C" {
    i32 AIEDITOR_PATHS;
    i32 AIEDITOR_ROUTES = -1;
    i32 AIEDITOR_AREAS;
    i32 AIEDITOR_LOCATORS;
    i32 AIEDITOR_CREATURES;
    i32 AIEDITOR_ANTINODES;
}

aieditor_settings_s aieditorsettings;
static i16 disable_cylinder_check;
extern "C" {
    extern void *ed_fnt;
}
struct EditorItemColours {
    u32 normal;
    u32 selected;
    u32 disabled;
    u32 background;
};
static EditorItemColours attr = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
static __used__ void aieditor_cbSetEditorMode(eduimenu_s *, eduiitem_s *item, unsigned int) {
    if ((u32)item->data < (u32)aieditorsettings.mode_count) {
        aieditor_SetMode(item->data);
        aieditor_ClearMainMenu();
    }
}

static __used__ void aieditor_cbCancelSelectEditorMode(eduimenu_s *menu, eduimenu_s *) {
    eduiMenuDestroy(menu);
}

static __used__ void aieditor_cbCancelSaveMenu(eduimenu_s *, eduiitem_s *, unsigned int) {
    aieditor_ClearMainMenu();
}

extern "C" {

    typedef void AIEDITORMOVEPLAYERS(NUVEC *position);
    AIEDITORMOVEPLAYERS *AIEditorMovePlayersFn;

    void aieditor_SetCurrentScript(char *, const AIEditorScriptSelection *);

    void InitFn_AIEditorMovePlayers(AIEDITORMOVEPLAYERS *function) {
        AIEditorMovePlayersFn = function;
    }

    eduimenu_s *aieditor_AddMessage(char *title, char *message) {
        eduimenu_s *outer = eduiMenuCreate(0xc8, 0x46, 0xf0, 0x10e, ed_fnt, aieditor_cbCancelMainMenu, title);
        if (outer != nullptr) {
            eduimenu_s *inner = eduiMenuCreate(0x78, 0x5a, 0x1b8, 0xfa, ed_fnt, nullptr, title);
            if (inner != nullptr) {
                eduiitem_s *item = eduiItemSelCreate(1, &attr, 0, 0, aieditor_cbCancelSaveMenu, message);
                eduiMenuAddItem(inner, item);
                eduiMenuAttach(outer, inner);
            }
        }
        return outer;
    }

    void aieditor_ClearMainMenu(void) {
        if (aieditor->main_menu != nullptr) {
            eduiMenuDestroy(aieditor->main_menu);
            aieditor->main_menu = nullptr;
        }
    }

    void aieditor_Enter(AISYS_s *system, NUVEC *camera_position, void *context, void *owner, void *extra_1,
                        void *extra_2, void *extra_3) {
        AIEDITOR_RENDER_STATE *state = aieditor;
        state->flags |= 2;
        state->ai_system = system;
        state->enter_context = context;
        state->enter_owner = owner;
        state->enter_extra_1 = extra_1;
        state->enter_extra_2 = extra_2;
        state->enter_extra_3 = extra_3;
        if (camera_position != nullptr) {
            edcamSetPos(camera_position);
        }
        for (i32 index = 0; index < aieditorsettings.mode_count; ++index) {
            void (*enter)() = aieditorsettings.modes[index].enter;
            if (enter != nullptr) {
                enter();
            }
        }
        aieditorsettings.snap_height_display = 1;
    }

    void aieditor_Init(AIEDITOR_RENDER_STATE *state, void **external_display_a, void **external_display_b) {
        aieditor = state;
        memset(&aieditorsettings, 0, sizeof(aieditorsettings));

        NUMTL *path_material = NuMtlCreate3D(1);
        aieditorsettings.path_material = path_material;
        path_material->opacity = 1.0f;
        path_material->diffuse_color.r = 0.5f;
        path_material->diffuse_color.g = 0.5f;
        path_material->diffuse_color.b = 0.5f;
        path_material->attribs.alpha_mode = 0;
        path_material->attribs.cull_mode = 2;
        path_material->attribs.z_mode = 0;
        NuMtlUpdate(path_material);

        NUMTL *route_material = NuMtlCreate3D(1);
        aieditorsettings.route_material = route_material;
        route_material->opacity = 1.0f;
        route_material->diffuse_color.r = 0.5f;
        route_material->diffuse_color.g = 0.5f;
        route_material->diffuse_color.b = 0.5f;
        route_material->attribs.cull_mode = 2;
        route_material->attribs.z_mode = 0;
        route_material->attribs.alpha_mode = 2;
        NuMtlUpdate(route_material);

        NUMTL *overlay_material = NuMtlCreate(1);
        aieditorsettings.overlay_material = overlay_material;
        overlay_material->diffuse_color.r = 0.5f;
        overlay_material->diffuse_color.g = 0.5f;
        overlay_material->diffuse_color.b = 0.5f;
        overlay_material->attribs.alpha_mode = 0;
        overlay_material->attribs.cull_mode = 2;
        overlay_material->attribs.z_mode = 3;
        overlay_material->opacity = 1.0f;
        NuMtlUpdate(overlay_material);

        aieditorsettings.current_path_type = -1;
        aieditorsettings.current_script_flags = 0;
        aieditorsettings.draw_all_paths = 0;
        aieditorsettings.unknown_060_bit0 = 0;
        aieditorsettings.show_creatures_display = 1;
        aieditorsettings.snap_height_display = 1;
        aieditorsettings.solid_path_display = 0;
        aieditorsettings.unknown_060_bit5 = 0;
        aieditorsettings.solid_antinode_display = 1;
        aieditorsettings.draw_wallsplines = 1;
        aieditorsettings.show_creatures_set = 0;
        aieditorsettings.current_script_params[0] = 0.0f;
        aieditorsettings.current_script_params[1] = 0.0f;
        aieditorsettings.current_script_params[2] = 0.0f;
        aieditorsettings.current_script_params[3] = 0.0f;
        aieditorsettings.mode_count = 0;
        memset(aieditorsettings.modes, 0, sizeof(aieditorsettings.modes));
        aieditorsettings.path_height_offset = 0.01f;

        AIEDITOR_PATHS =
            aieditor_Register("AIEDITOR_PATHS", pathEditor_Enter, reinterpret_cast<void (*)()>(pathEditor_Process),
                              reinterpret_cast<void (*)()>(pathEditor_Render), nullptr);
        AIEDITOR_ROUTES =
            aieditor_Register("AIEDITOR_ROUTES", nullptr, reinterpret_cast<void (*)()>(routeEditor_Process),
                              reinterpret_cast<void (*)()>(routeEditor_Render), nullptr);
        AIEDITOR_AREAS =
            aieditor_Register("AIEDITOR_AREAS", areaEditor_Enter, reinterpret_cast<void (*)()>(areaEditor_Process),
                              reinterpret_cast<void (*)()>(areaEditor_Render), nullptr);
        AIEDITOR_LOCATORS = aieditor_Register("AIEDITOR_LOCATORS", locatorEditor_Enter,
                                              reinterpret_cast<void (*)()>(locatorEditor_Process),
                                              reinterpret_cast<void (*)()>(locatorEditor_Render), nullptr);
        AIEDITOR_CREATURES = aieditor_Register("AIEDITOR_CREATURES", creatureEditor_Enter,
                                               reinterpret_cast<void (*)()>(creatureEditor_Process),
                                               reinterpret_cast<void (*)()>(creatureEditor_Render), nullptr);
        AIEDITOR_ANTINODES = aieditor_Register("AIEDITOR_ANTINODES", antinodeEditor_Enter,
                                               reinterpret_cast<void (*)()>(antinodeEditor_Process),
                                               reinterpret_cast<void (*)()>(antinodeEditor_Render), nullptr);
        aieditor_RegisterDefaultPathCnxTypes();
        aieditorsettings.external_display_a = *external_display_a;
        aieditorsettings.external_display_b = *external_display_b;
    }

    void aieditor_Leave(void) {
        edmainGetCamera()->near_clip = 0.15f;
        for (i32 index = 0; index < aieditorsettings.mode_count; ++index) {
            void (*leave)() = aieditorsettings.modes[index].leave;
            if (leave != nullptr) {
                leave();
            }
        }
        AIEDITOR_RENDER_STATE *state = aieditor;
        if (state->mode_selection_36930 != nullptr) {
            strcpy(aieditorsettings.current_area_name, state->mode_selection_36930->name);
        }
        if (state->current_path != nullptr) {
            strcpy(aieditorsettings.current_path_name, state->current_path->name);
        }
        if (state->current_route != nullptr) {
            strcpy(aieditorsettings.current_route_name, state->current_route->name);
        }
        AISYSRebuildFromEditorData();
        AIScriptInitConditions(aieditor->ai_system);
        aieditor->flags &= ~u8(2);
    }

    bool aieditor_Proc(f32 menu_delta_time, nupad_s *pad, f32 frame_delta_time, u32 exit_buttons) {
        aieditor->pad_buttons = pad->digital_buttons;
        aieditor->pad_pressed = pad->digital_buttons_pressed;
        if (aieditor->main_menu != nullptr) {
            eduiMenuProcess(aieditor->main_menu, menu_delta_time, pad);
            return false;
        }
        if (NuStrLen(aieditor->warning) != 0) {
            aieditor->main_menu = aieditor_AddMessage((char *)"Warning", aieditor->warning);
            NuStrCpy(aieditor->warning, "");
            if (aieditor->main_menu != nullptr) {
                return false;
            }
        }

        aieditorsettings.elapsed_time = NuFmod(frame_delta_time + aieditorsettings.elapsed_time, 1.0f);
        if ((pad->digital_buttons & 0x100) == 0 && (aieditor->flags & 4) == 0) {
            edcamMoveEx(pad, frame_delta_time);
        }

        NUVEC previous_position = aieditor->cursor_position;
        edcamGetPosAng(&aieditor->cursor_position, &aieditor->camera_pitch, &aieditor->camera_yaw);
        NuVecSub(&aieditor->cursor_movement, &aieditor->cursor_position, &previous_position);
        aieditor->flags &= ~u8(4);
        memset(&aieditor->cursor_platform, 0, sizeof(aieditor->cursor_platform));
        aieditor->camera_position = aieditor->cursor_position;
        f32 terrain_height = AITerrShadow(&aieditor->cursor_position, 0.0f, 5.0f, -1);
        i32 platform = AITerrShadowOnPlatform();
        NuSpecialFindByPlatformID(static_cast<nugscn_s *>(aieditor->enter_context), &aieditor->cursor_platform,
                                  platform);

        if (aieditorsettings.snap_height_display) {
            const f32 absent_height = 2000000.0f;
            f32 closest = absent_height;
            if (terrain_height != absent_height) {
                aieditor->camera_position.y = terrain_height;
                closest = __builtin_fabsf(aieditor->cursor_position.y - terrain_height);
            }
            if (EShadY != absent_height) {
                f32 distance = __builtin_fabsf(aieditor->cursor_position.y - EShadY);
                if (distance < closest) {
                    aieditor->camera_position.y = EShadY;
                    closest = distance;
                }
            }
            if (ShadRoofY != absent_height && __builtin_fabsf(aieditor->cursor_position.y - ShadRoofY) < closest) {
                aieditor->camera_position.y = ShadRoofY;
            }
            if (disable_cylinder_check == 0 && aieditor->current_path != nullptr &&
                aieditor->current_path->current_node != nullptr &&
                aieditor->current_path->current_node == aieditor->current_path->nearest_node &&
                aieditor->camera_position.y < aieditor->cursor_position.y &&
                terrain_height == aieditor->camera_position.y) {
                NUVEC movement;
                NuVecSub(&movement, &aieditor->camera_position, &aieditor->cursor_position);
                union {
                    u32 bits;
                    f32 value;
                } ray_scale = {0x233877aa};
                i32 hit = NewRayCastScaleY(&aieditor->cursor_position, &movement,
                                           aieditor->current_path->current_node->radius, ray_scale.value, 0);
                if (hit != 0 && (hit & 0x10) == 0) {
                    NuVecAdd(&aieditor->camera_position, &aieditor->cursor_position, &movement);
                }
            }
            aieditor->camera_position.y += aieditorsettings.path_height_offset;
            if (aieditorsettings.unknown_060_bit5) {
                aieditor->cursor_position = aieditor->camera_position;
            }
        }
        auto process = reinterpret_cast<eduimenu_s *(*)(nupad_s *)>(
            aieditorsettings.modes[static_cast<i16>(aieditorsettings.current_mode)].callback_24);
        if (process != nullptr) {
            aieditor->main_menu = process(pad);
        }
        return aieditor->main_menu == nullptr && (pad->digital_buttons_pressed & exit_buttons) != 0;
    }

    i32 aieditor_Register(const char *name, void (*enter)(), void (*callback_24)(), void (*callback_28)(),
                          void (*leave)()) {
        if (aieditorsettings.mode_count > 7) {
            return -1;
        }
        i32 index = aieditorsettings.mode_count;
        ++aieditorsettings.mode_count;
        aieditor_settings_s::EditorMode &mode = aieditorsettings.modes[index];
        NuStrCpy(mode.name, name);
        mode.enter = enter;
        mode.callback_24 = callback_24;
        mode.callback_28 = callback_28;
        mode.leave = leave;
        return index;
    }

    void AISysSetPathCylinderCheck(i32 enabled) {
        disable_cylinder_check = enabled == 0;
    }

    void aieditor_Render(f32 x_scale, f32 y_scale) {
        NUCAMERA *camera = edmainGetCamera();
        if (near_clip_at_cursor == 0) {
            camera->near_clip = 0.15f;
        } else {
            camera->near_clip =
                NuVecDist(&aieditor->cursor_position, reinterpret_cast<NUVEC *>(&camera->mtx.m30), nullptr) - 5.0f;
        }
        edcamSet();

        f32 cursor_radius = 0.5f;
        i32 cursor_colour = -1;
        if (aieditorsettings.snap_height_display) {
            edbitsDrawCross(aieditor->cursor_position.x, aieditor->cursor_position.y, aieditor->cursor_position.z, 0.5f,
                            -1, aieditorsettings.path_material);
            cursor_radius = 0.25f;
            cursor_colour = 0xff00ff00;
        }
        edbitsDrawCross(aieditor->camera_position.x, aieditor->camera_position.y, aieditor->camera_position.z,
                        cursor_radius, cursor_colour, aieditorsettings.path_material);
        NuRndrRect2di(0x1680, 0x9b0, 0x1180, 0x488, 0x80808080, aieditorsettings.overlay_material);
        NuRndrRect2di(0x1670, 0x8f0, 0x11a0, 0xc0, 0x80000000, aieditorsettings.overlay_material);
        NuRndrLine2di(0x1670, 0x9b0, 0x1670, 0xe40, 0x80000000, aieditorsettings.overlay_material);
        NuRndrLine2di(0x2810, 0x9b0, 0x2810, 0xe40, 0x80000000, aieditorsettings.overlay_material);
        NuRndrLine2di(0x1670, 0xe40, 0x2810, 0xe40, 0x80000000, aieditorsettings.overlay_material);

        NuQFntPushPrintMode(2);
        NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
        NuQFntSet(system_qfont);
        NuQFntSetColour(system_qfont, 0x80808080);
        NuQFntSetScale(system_qfont, x_scale, y_scale);
        auto render = reinterpret_cast<void (*)(i32, i32, f32, f32)>(
            aieditorsettings.modes[static_cast<i16>(aieditorsettings.current_mode)].callback_28);
        if (render != nullptr) {
            render(0x168, 0x136, x_scale, y_scale);
        }
        NuQFntSet(system_qfont);
        NuQFntSetColour(system_qfont, 0x80000000);
        NuQFntSetScale(system_qfont, x_scale, y_scale);
        NuQFntPrintEx(system_qfont, 0x1720, 0xde8, 0x10, "%5.2f", static_cast<f64>(aieditor->camera_position.x));
        NuQFntPrintEx(system_qfont, 0x1b80, 0xde8, 0x10, "%5.2f", static_cast<f64>(aieditor->camera_position.y));
        NuQFntPrintEx(system_qfont, 0x1fe0, 0xde8, 0x10, "%5.2f", static_cast<f64>(aieditor->camera_position.z));
        if (aieditor->main_menu != nullptr) {
            NuQFntSetColour(system_qfont, 0x80000000);
            NuQFntSetScale(system_qfont, x_scale, y_scale);
            eduiMenuRender(aieditor->main_menu);
        }
        NuQFntPopPrintMode();
        NuQFntPopCoordinateSystem();
    }

    void aieditor_Reset(void) {
        aieditorsettings.current_path_type = -1;
        aieditorsettings.current_area_name[0] = 0;
        aieditorsettings.current_path_name[0] = 0;
        aieditorsettings.current_route_name[0] = 0;
        aieditor_SetCurrentScript((char *)"default", 0);
    }

    i32 aieditor_Save(void) {
        VARIPTR cursor;
        VARIPTR end;
        cursor.u8_ptr = aieditor->save_scratch;
        end.char_ptr = aieditor->warning;
        memset(cursor.u8_ptr, 0, sizeof(aieditor->save_scratch));
        AIPATHSYS_s *paths = pathEditorCreateData(&cursor, &end, &aieditorsettings.external_display_a,
                                                  &aieditorsettings.external_display_b);
        if (paths == nullptr) {
            return 1;
        }
        if (aieditor->enter_extra_2 == nullptr) {
            return 1;
        }

        char filename[128];
        sprintf(filename, "%sLevels\\%s\\%s\\%s.ai2", AiLevelPathName, static_cast<char *>(aieditor->enter_extra_1),
                static_cast<char *>(aieditor->enter_extra_3), static_cast<char *>(aieditor->enter_extra_2));
        if (NuFileExists(filename)) {
            NUFILE_INFO info;
            if (NuFileGetInfo(filename, &info)) {
                char *basename = filename;
                for (char *separator = NuStrIStr(basename, (char *)"\\"); separator != nullptr;
                     separator = NuStrIStr(basename, (char *)"\\")) {
                    basename = separator + 1;
                }
                char backup[128];
                sprintf(backup, "ai_backups\\%s.%d_%d_%d_%d_%d_%d", basename, info.year, info.month, info.day,
                        info.hour, info.minute, info.second);
                NuFileCopy(backup, filename);
            }
            char backup[128];
            sprintf(backup, "%s.bak", filename);
            NuFileCopy(backup, filename);
        }
        EdFileSetMedia(1);
        if (!EdFileOpen(filename, NUFILE_WRITE)) {
            return 0;
        }
        EdFileWriteInt(aidata_version);
        pathEditorSaveData(paths);
        areaEditorSaveData();
        locatorEditorSaveData(paths);
        creatureEditorSaveData(paths);
        antinodeEditorSaveData();
        if (GameAISaveFn != nullptr) {
            reinterpret_cast<void (*)(AIPATHSYS_s *)>(GameAISaveFn)(paths);
        }
        EdFileClose();
        return 1;
    }

    void aieditor_SetCurrentScript(char *name, const AIEditorScriptSelection *selection) {
        if (NuStrICmp(name, aieditorsettings.current_script_name) != 0) {
            strcpy(aieditorsettings.current_script_name, name);
        }
        if (selection != nullptr) {
            aieditorsettings.current_script_flags = selection->flags & 0x1e;
            memcpy(aieditorsettings.current_script_params, selection->script_params,
                   sizeof(aieditorsettings.current_script_params));
            return;
        }
        AISCRIPT *script = AIScriptFind(aieditor->ai_system, aieditorsettings.current_script_name, 1, 1, 1);
        if (script != nullptr) {
            for (i32 i = 0; i < 4; ++i) {
                aieditorsettings.current_script_params[i] = script->params[i].default_val;
            }
        } else {
            memset(aieditorsettings.current_script_params, 0, sizeof(aieditorsettings.current_script_params));
        }
        aieditorsettings.current_script_flags = 0;
    }

    void aieditor_SetMode(i32 mode) {
        u16 previous_mode = aieditorsettings.current_mode;
        aieditorsettings.current_mode = mode;
        AIEDITOR_RENDER_STATE *state = aieditor;
        if (state != nullptr && previous_mode != (u16)mode) {
            if (state->current_path != nullptr) {
                state->current_path->current_node = nullptr;
            }
            state->mode_selection_36930 = nullptr;
            state->mode_selection_37a48 = nullptr;
            state->mode_selection_3c260 = nullptr;
            state->mode_selection_42e9c = nullptr;
        }
    }

    void aieditor_cbCancelMainMenu(eduimenu_s *, eduimenu_s *) {
        aieditor_ClearMainMenu();
    }

    void aieditor_cbDrawAllToggle(eduimenu_s *, eduiitem_s *item, u32) {
        aieditorsettings.draw_all_paths = item->highlighted;
    }

    void aieditor_cbGoToPlayer(eduimenu_s *, eduiitem_s *, u32) {
        AISYS_s *system = aieditor->ai_system;
        if (system != nullptr && system->player_1 != nullptr) {
            edcamSetPos(&system->player_1->position);
        }
        aieditor_ClearMainMenu();
    }

    void aieditor_cbMovePlayer(eduimenu_s *, eduiitem_s *, u32) {
        if (AIEditorMovePlayersFn != nullptr) {
            AIEditorMovePlayersFn(&aieditor->camera_position);
        }
        aieditor_ClearMainMenu();
    }

    void aieditor_cbSave(eduimenu_s *parent, eduiitem_s *, u32) {
        if (aieditor_Save() == 0) {
            eduimenu_s *menu = eduiMenuCreate(0x78, 0x5a, 0x1b8, 0xfa, ed_fnt, nullptr, (char *)"AI Save FAILED");
            if (menu != nullptr) {
                char message[128];
                sprintf(message, "Make sure there is a folder called \"%s\" in your level directory",
                        static_cast<char *>(aieditor->enter_extra_3));
                eduiMenuAddItem(menu, eduiItemSelCreate(1, &attr, 0, 0, aieditor_cbCancelSaveMenu, message));
                eduiMenuAttach(parent, menu);
            }
        } else {
            aieditor_ClearMainMenu();
        }
    }

    void aieditor_cbShowCreaturesSetToggle(eduimenu_s *, eduiitem_s *item, u32) {
        aieditorsettings.show_creatures_set = item->highlighted;
    }

    void aieditor_cbShowCreaturesToggle(eduimenu_s *, eduiitem_s *item, u32) {
        aieditorsettings.show_creatures_display = item->highlighted;
    }

    void aieditor_cbSnapHeightToggle(eduimenu_s *, eduiitem_s *item, u32) {
        aieditorsettings.snap_height_display = item->highlighted;
    }

    void aieditor_cbSolidAntinodeDisplayToggle(eduimenu_s *, eduiitem_s *item, u32) {
        aieditorsettings.solid_antinode_display = item->highlighted;
    }

    void aieditor_cbSolidPathDisplayToggle(eduimenu_s *, eduiitem_s *item, u32) {
        aieditorsettings.solid_path_display = item->highlighted;
    }

    void aieditor_cbStopPlatformsToggle(eduimenu_s *, eduiitem_s *item, u32) {
        aieditorsettings.stop_platforms = item->highlighted;
    }

    void aieditor_cvSelectEditorMode(eduimenu_s *parent, eduiitem_s *, u32) {
        eduimenu_s *menu = eduiMenuCreate(0xdc, 0x46, 0xf0, 0xfa, ed_fnt, aieditor_cbCancelSelectEditorMode,
                                          (char *)"Select Editor Mode");
        if (menu == nullptr) {
            return;
        }
        for (i32 index = 0; index < aieditorsettings.mode_count; ++index) {
            if ((i16)aieditorsettings.current_mode == index) {
                eduiitem_s *item = eduiItemCheckCreate(index, &attr, 1, 1, aieditor_cbSetEditorMode,
                                                       aieditorsettings.modes[index].name);
                eduiMenuAddItem(menu, item);
                menu->selected = edui_last_item;
            } else {
                eduiitem_s *item = eduiItemCheckCreate(index, &attr, 0, 1, aieditor_cbSetEditorMode,
                                                       aieditorsettings.modes[index].name);
                eduiMenuAddItem(menu, item);
            }
            eduiMenuAttach(parent, menu);
        }
    }

} // extern "C"
