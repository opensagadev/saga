#include "host/harness/programs/editor.hpp"

#include <SDL3/SDL.h>

#include "batman.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edui.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "host/harness/frame_hook.hpp"
#include "host/harness/programs/window.hpp"
#include "host/harness/startup.hpp"
#include "host/platform/free_camera.hpp"
#include "host/platform/keyboard.hpp"
#include "host/platform/mouse.hpp"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/misc/androidbatman.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nuplatform/nuplatform.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

extern eduimenu_s *edLevelActiveMenu;
extern eduimenu_s *edLevelPinnedMenu;
extern i32 edLevelDestroyActiveMenu;
extern i32 delete_menu_active;
extern "C" i32 rtled_menu_active;
extern "C" eduimenu_s *edrtl_active_menu;
extern "C" void rtlSetUndoBuffer(VARIPTR *buffer, VARIPTR end, i32 count);
extern ClassEditor theClassEditor;
extern PropertyTool thePropertyTool;
void EdDrawBegin(i32 material);
void EdDrawEnd();
void EdDrawLineCircleX(VuVec const &centre, float radius, i32 colour, i32 segments);
void EdDrawLineCircleY(VuVec const &centre, float radius, i32 colour, i32 segments);
void EdDrawLineCircleZ(VuVec const &centre, float radius, i32 colour, i32 segments);

namespace saga::host::harness {
    namespace {

        struct EditorOptions final {
            std::optional<std::string> destination;
        };

        using HostSceneObject = SceneObject;
        DECOMP_ASSERT(sizeof(HostSceneObject) == 0x2c, "HostSceneObject size");

        class PlatformSelection final {
          public:
            explicit PlatformSelection(PLATFORMS_SUPPORTED selected)
                : platform{NuPlatform::Get()}, previous{this->platform->GetCurrentPlatform()} {
                this->platform->SetCurrentPlatform(selected);
            }

            ~PlatformSelection() {
                this->platform->SetCurrentPlatform(this->previous);
            }

            PlatformSelection(const PlatformSelection &) = delete;
            PlatformSelection &operator=(const PlatformSelection &) = delete;

          private:
            NuPlatform *platform;
            PLATFORMS_SUPPORTED previous;
        };

        class EditorSession final {
          public:
            static EditorSession &instance() {
                static EditorSession session;
                return session;
            }

            void start(EditorOptions options) {
                this->options = std::move(options);
                this->lifecycle = Lifecycle::waiting_for_game;
                this->destination_state = this->options.destination ? DestinationState::waiting_for_metadata
                                                                    : DestinationState::not_requested;
                this->destination = nullptr;
                this->registered_scene = nullptr;
                this->registered_scene_id = -1;
                this->save_directory.clear();
                theClassEditor.selected_objects = {};
                theClassEditor.current_object = {};
                this->requested_view = EditorView::modules;
                this->active_view = EditorView::game;
                this->module_menu = nullptr;
                this->detached_level_menus.clear();
                this->previous_editor_buttons = 0;
                this->previous_rtl_escape_held = false;
                this->last_selection_count = -1;
                this->last_property_menu_count = -1;
                this->toggle_requests.store(0, std::memory_order_relaxed);
                this->editor_buttons.store(0, std::memory_order_relaxed);
                this->editor_keys.store(0, std::memory_order_relaxed);
                this->capture_game_input.store(false, std::memory_order_relaxed);
                this->free_camera_enabled = false;
                this->free_camera_ready.store(false, std::memory_order_relaxed);
                saga::host::set_editor_mouse_enabled(false);
                this->exit_status.store(no_exit_requested, std::memory_order_relaxed);
                set_frame_callback(&EditorSession::update);
                LOG_INFO("editor: F1 modules, F2 Level Editor, F3 free camera; Enter selects, Escape goes back");
                LOG_INFO("editor RTL default keys: M options, N add, R drag, I inspect, Delete remove, Tab next mode; "
                         "WASD move, Q/E lower/raise, Z/X zoom, numpad 4/6/8/5 look");
                LOG_INFO("editor RTL keyboard actions follow the selected Ralph/Steve control table");
            }

            static void handle_event(const SDL_Event &event) {
                if (event.type != SDL_EVENT_KEY_DOWN)
                    return;
                if (!event.key.repeat && event.key.scancode == SDL_SCANCODE_F1)
                    instance().toggle_requests.fetch_or(1u, std::memory_order_release);
                else if (!event.key.repeat && event.key.scancode == SDL_SCANCODE_F2)
                    instance().toggle_requests.fetch_or(2u, std::memory_order_release);
                else if (!event.key.repeat && event.key.scancode == SDL_SCANCODE_F3)
                    instance().toggle_requests.fetch_or(4u, std::memory_order_release);
            }

            static int requested_exit_status() {
                return instance().exit_status.load(std::memory_order_acquire);
            }

            static bool is_free_camera_ready() {
                return instance().free_camera_ready.load(std::memory_order_acquire);
            }

            static u32 filter_game_input(u32 buttons) {
                EditorSession &session = instance();
                const bool *keyboard = SDL_GetKeyboardState(nullptr);
                u32 keys = 0;
                for (const auto &[scancode, bit] : editor_key_bindings) {
                    if (keyboard[scancode])
                        keys |= bit;
                }
                session.editor_keys.store(keys, std::memory_order_release);
                // The game binds WASD and E/F to pad actions. Editor keyboard
                // shortcuts use those keys directly, so only dedicated menu
                // keys may enter the synthetic editor pad.
                u32 editor_buttons = 0;
                if (keyboard[SDL_SCANCODE_UP])
                    editor_buttons |= GAMEPAD_DUP;
                if (keyboard[SDL_SCANCODE_DOWN])
                    editor_buttons |= GAMEPAD_DDOWN;
                if (keyboard[SDL_SCANCODE_RETURN] || keyboard[SDL_SCANCODE_KP_ENTER])
                    editor_buttons |= GAMEPAD_MENUSELECT;
                if (keyboard[SDL_SCANCODE_ESCAPE])
                    editor_buttons |= editor_cancel_button | GAMEPAD_START;
                session.editor_buttons.store(editor_buttons, std::memory_order_release);
                return session.capture_game_input.load(std::memory_order_acquire) ? 0 : buttons;
            }

          private:
            static constexpr std::size_t font_working_headroom = 16 * 1024;
            static constexpr std::size_t editor_pool_size = 1024 * 1024;
            static constexpr i32 rtl_undo_snapshots = 16;
            static constexpr std::size_t rtl_undo_bytes_per_snapshot =
                128 * sizeof(rtl_s) + 3 * sizeof(rtl_s *) + sizeof(NUVEC);
            static_assert(sizeof(void *) != 4 || rtl_undo_bytes_per_snapshot == 0x4618);
            static constexpr std::size_t rtl_undo_bytes = rtl_undo_bytes_per_snapshot * rtl_undo_snapshots;
            static constexpr int no_exit_requested = -1;
            static constexpr int editor_width = 640;
            static constexpr int editor_height = 448;
            static constexpr int menu_margin = 1;
            static constexpr int main_menu_top = 16;
            static constexpr f32 menu_movement_speed = 240.0f;
            // The editor checks bit 4 directly; the game remaps GAMEPAD_MENUCANCEL to bit 5 at startup.
            static constexpr u32 editor_cancel_button = 1u << 4;
            static constexpr u32 rtl_control_buttons[2][8] = {
                {0x80, 0x10, 0x40, 0x1000, 0x100, 0x20, 0x8000, 0x2000},
                {0x40, 0x10, 0x20, 0x1000, 0x80, 0x100, 0x4, 0x8},
            };

            struct RtlControlItem {
                eduiitem_s *item = nullptr;
                EdUiItemCallback selected = nullptr;
            };

            enum EditorKey : u32 {
                key_up = 1u << 0,
                key_down = 1u << 1,
                key_left = 1u << 2,
                key_right = 1u << 3,
                key_enter = 1u << 4,
                key_escape = 1u << 5,
                key_tab = 1u << 6,
                key_menu = 1u << 7,
                key_add = 1u << 8,
                key_delete = 1u << 9,
                key_drag = 1u << 10,
                key_inspect = 1u << 11,
                key_lock = 1u << 12,
                key_group = 1u << 13,
                key_group_next = 1u << 14,
                key_previous = 1u << 15,
                key_next = 1u << 16,
                key_burn_previous = 1u << 17,
                key_forward = 1u << 18,
                key_strafe_left = 1u << 19,
                key_backward = 1u << 20,
                key_strafe_right = 1u << 21,
                key_lower = 1u << 22,
                key_raise = 1u << 23,
                key_zoom_out = 1u << 24,
                key_zoom_in = 1u << 25,
                key_look_left = 1u << 26,
                key_look_right = 1u << 27,
                key_look_up = 1u << 28,
                key_look_down = 1u << 29,
            };

            static constexpr std::array editor_key_bindings{
                std::pair{SDL_SCANCODE_UP, key_up},
                std::pair{SDL_SCANCODE_DOWN, key_down},
                std::pair{SDL_SCANCODE_LEFT, key_left},
                std::pair{SDL_SCANCODE_RIGHT, key_right},
                std::pair{SDL_SCANCODE_RETURN, key_enter},
                std::pair{SDL_SCANCODE_KP_ENTER, key_enter},
                std::pair{SDL_SCANCODE_ESCAPE, key_escape},
                std::pair{SDL_SCANCODE_TAB, key_tab},
                std::pair{SDL_SCANCODE_M, key_menu},
                std::pair{SDL_SCANCODE_N, key_add},
                std::pair{SDL_SCANCODE_DELETE, key_delete},
                std::pair{SDL_SCANCODE_R, key_drag},
                std::pair{SDL_SCANCODE_I, key_inspect},
                std::pair{SDL_SCANCODE_L, key_lock},
                std::pair{SDL_SCANCODE_G, key_group},
                std::pair{SDL_SCANCODE_H, key_group_next},
                std::pair{SDL_SCANCODE_LEFTBRACKET, key_previous},
                std::pair{SDL_SCANCODE_RIGHTBRACKET, key_next},
                std::pair{SDL_SCANCODE_COMMA, key_burn_previous},
                std::pair{SDL_SCANCODE_W, key_forward},
                std::pair{SDL_SCANCODE_A, key_strafe_left},
                std::pair{SDL_SCANCODE_S, key_backward},
                std::pair{SDL_SCANCODE_D, key_strafe_right},
                std::pair{SDL_SCANCODE_Q, key_lower},
                std::pair{SDL_SCANCODE_E, key_raise},
                std::pair{SDL_SCANCODE_Z, key_zoom_out},
                std::pair{SDL_SCANCODE_X, key_zoom_in},
                std::pair{SDL_SCANCODE_KP_4, key_look_left},
                std::pair{SDL_SCANCODE_KP_6, key_look_right},
                std::pair{SDL_SCANCODE_KP_8, key_look_up},
                std::pair{SDL_SCANCODE_KP_5, key_look_down},
            };

            struct MenuPosition final {
                eduimenu_s *menu;
                i32 x;
            };

            enum class Lifecycle {
                waiting_for_game,
                in_game,
                in_editor,
            };

            enum class EditorView {
                game,
                modules,
                level,
            };

            enum class DestinationState {
                not_requested,
                waiting_for_metadata,
                loading,
                loaded,
                failed,
            };

            static void update() {
                instance().on_frame();
            }

            void on_frame() {
                const unsigned requests = this->toggle_requests.exchange(0, std::memory_order_acq_rel);
                if (requests & 1u)
                    this->requested_view =
                        this->requested_view == EditorView::modules ? EditorView::game : EditorView::modules;
                if (requests & 2u)
                    this->requested_view =
                        this->requested_view == EditorView::level ? EditorView::game : EditorView::level;
                if ((requests & 4u) && this->active_view == EditorView::level) {
                    this->free_camera_enabled = !this->free_camera_enabled;
                    this->free_camera_ready.store(this->free_camera_enabled, std::memory_order_release);
                    this->configure_level_camera();
                    LOG_INFO("editor: free camera %s (numpad 4/5/6/8, hold Shift to move)",
                             this->free_camera_enabled ? "on" : "off");
                }

                this->update_destination();
                if (this->destination_state == DestinationState::failed)
                    return;

                if (this->lifecycle == Lifecycle::waiting_for_game) {
                    if (!this->game_is_ready())
                        return;
                    if (!this->initialize_editor())
                        return;
                    this->lifecycle = Lifecycle::in_game;
                }

                this->refresh_scene_objects();

                if (this->active_view != this->requested_view) {
                    if (this->active_view != EditorView::game)
                        this->return_to_game();
                    if (this->requested_view != EditorView::game)
                        this->enter_editor(this->requested_view);
                }

                if (this->lifecycle != Lifecycle::in_editor)
                    return;

                nupad_s pad = this->make_editor_pad();
                if (this->active_view == EditorView::level) {
                    saga::host::set_editor_arrow_selection_enabled(!thePropertyTool.HasActiveMenu());
                    if (edLevelDestroyActiveMenu) {
                        edLevelDestroyActiveMenu = 0;
                        this->destroy_level_menu();
                    }
                    const auto menu_positions = this->capture_menu_positions();
                    this->bind_scene_object_selection();
                    if (this->free_camera_enabled)
                        edcamSet();
                    nupad_s *pads[]{&pad, nullptr};
                    ThingProcessData process_data{FRAMETIME, static_cast<u32>(Paused), pads, 2};
                    HostFreeCameraSuppressEditorCameraInput(this->free_camera_enabled);
                    theLevelEditor.ProcessEvenWhenPaused(&process_data);
                    HostFreeCameraSuppressEditorCameraInput(false);
                    if (this->last_selection_count != theClassEditor.selected_objects.count ||
                        this->last_property_menu_count != thePropertyTool.menu_count) {
                        this->last_selection_count = theClassEditor.selected_objects.count;
                        this->last_property_menu_count = thePropertyTool.menu_count;
                        LOG_INFO("editor: selected objects=%d property panels=%d", this->last_selection_count,
                                 this->last_property_menu_count);
                        for (PropertyMenu *menu = thePropertyTool.active_menu; menu; menu = menu->next)
                            LOG_INFO("editor: property panel=%p title=%s at (%d,%d)", menu->menu,
                                     menu->menu && menu->menu->title ? menu->menu->title : "",
                                     menu->menu ? menu->menu->x : 0, menu->menu ? menu->menu->y : 0);
                        LOG_INFO("editor: level menu=%p pinned=%p class menu=%p", edLevelActiveMenu, edLevelPinnedMenu,
                                 theClassEditor.menu);
                    }
                    this->constrain_menus(menu_positions);
                    if (NuRndrBeginSceneEx(-1, -2, 0)) {
                        eduiFlushInteracts();
                        this->render_scene_object_debug();
                        ThingRenderData render_data{};
                        theLevelEditor.Display(&render_data);
                        NuRndrEndSceneEx(0);
                    }
                    return;
                }

                const auto menu_positions = this->capture_menu_positions();
                eduimenu_s *main_menu = edGetMainMenu();
                const bool at_main_menu =
                    main_menu && !main_menu->child && !edmainCurrent() && eduiGetActiveMenu() == main_menu;
                const bool requested_return =
                    at_main_menu &&
                    ((pad.digital_buttons_pressed & editor_cancel_button) ||
                     ((pad.digital_buttons_pressed & GAMEPAD_MENUSELECT) && main_menu->selected == main_menu->last));
                if (edmainCurrent() == &edrtldesc)
                    this->track_rtl_control_menu();
                const i32 process_result = edmainProcess(FRAMETIME, &pad);
                if (edmainCurrent() == &edrtldesc)
                    this->track_rtl_control_menu();
                if (process_result) {
                    if (requested_return) {
                        main_menu->selected = main_menu->first;
                        this->requested_view = EditorView::game;
                        this->return_to_game();
                        return;
                    }
                    LOG_WARN("editor: ignored unexpected return outside the main menu");
                    if (main_menu)
                        eduiSetActiveMenu(main_menu);
                }
                this->constrain_menus(menu_positions);

                if (NuRndrBeginSceneEx(-1, -2, 0)) {
                    edmainRender();
                    NuRndrEndSceneEx(0);
                }
            }

            void update_destination() {
                if (this->destination_state == DestinationState::waiting_for_metadata) {
                    // The permanent-data loader publishes these collections before
                    // it finishes fixing them up. Do not resolve a destination
                    // until every record is stable and safe to hand to the game.
                    if (!PermDataLoaded || LEVELCOUNT <= 0 || !LDataList || !ADataList)
                        return;

                    this->destination = this->find_destination(*this->options.destination);
                    if (!this->destination) {
                        LOG_ERR("editor: unknown gameplay level or area '%s'", this->options.destination->c_str());
                        this->destination_state = DestinationState::failed;
                        this->exit_status.store(2, std::memory_order_release);
                        return;
                    }

                    memcard_autosaveenabled = 0;
                    HostEnterLevel(*this->destination);
                    this->destination_state = DestinationState::loading;
                    LOG_INFO("editor: loading %s (area %d)", this->destination->name, this->destination->area_index);
                }

                if (this->destination_state == DestinationState::loading && WORLD && WORLD->loaded &&
                    WORLD->current_level == this->destination && NewMode == 0 && !NewLData) {
                    this->destination_state = DestinationState::loaded;
                    LOG_INFO("editor: destination ready: %s", this->destination->name);
                }
            }

            [[nodiscard]] LEVELDATA_s *find_destination(const std::string &name) const {
                for (i32 index = 0; index < LEVELCOUNT; ++index) {
                    LEVELDATA_s &level = LDataList[index];
                    if (this->is_playable(level) && SDL_strcasecmp(level.name, name.c_str()) == 0)
                        return &level;
                }

                for (i32 index = 0; index < AREACOUNT; ++index) {
                    AREADATA &area = ADataList[index];
                    if (SDL_strcasecmp(area.file, name.c_str()) != 0)
                        continue;

                    for (i32 level_index = 0; level_index < area.level_count; ++level_index) {
                        const i32 candidate = area.levels[level_index];
                        if (candidate >= 0 && candidate < LEVELCOUNT && this->is_playable(LDataList[candidate]))
                            return &LDataList[candidate];
                    }
                    return nullptr;
                }
                return nullptr;
            }

            [[nodiscard]] bool game_is_ready() const {
                if (this->destination_state == DestinationState::waiting_for_metadata ||
                    this->destination_state == DestinationState::loading)
                    return false;

                return DEFAULTFRAMETIME > 0.0f && Game_NuPad && Game_NuPad[0] && WORLD && WORLD->loaded &&
                       WORLD->current_level;
            }

            [[nodiscard]] bool initialize_editor() {
                if (this->save_directory.empty()) {
                    std::string directory_template = "/tmp/saga-editor-XXXXXX";
                    if (!::mkdtemp(directory_template.data())) {
                        LOG_ERR("editor: could not create a temporary directory for level saves");
                        this->exit_status.store(1, std::memory_order_release);
                        return false;
                    }
                    this->save_directory = std::move(directory_template);
                    LOG_INFO("editor: level saves will be written to %s", this->save_directory.c_str());
                }
                if (!this->editor_pool_installed) {
                    MemoryManager &manager = theMemoryManager;
                    manager.cursor = reinterpret_cast<usize>(this->editor_pool.data());
                    manager.end = manager.cursor + this->editor_pool.size();
                    manager.cursor_cell = &manager.cursor;
                    manager.end_cell = &manager.end;
                    manager.high_water = manager.cursor;
                    manager.remaining += this->editor_pool.size();
                    this->editor_pool_installed = true;
                    LOG_INFO("editor: supplied %zu bytes of persistent host memory for UI objects",
                             this->editor_pool.size());
                }
                if (!this->rtl_undo_installed) {
                    VARIPTR cursor{};
                    cursor.u8_ptr = this->rtl_undo_storage.data();
                    VARIPTR end{};
                    end.u8_ptr = this->rtl_undo_storage.data() + this->rtl_undo_storage.size();
                    rtlSetUndoBuffer(&cursor, end, rtl_undo_snapshots);
                    if (cursor.u8_ptr != end.u8_ptr) {
                        LOG_ERR("editor: RTL undo buffer size does not match the original allocator");
                        this->exit_status.store(1, std::memory_order_release);
                        return false;
                    }
                    this->rtl_undo_installed = true;
                }
                if (!system_qfont) {
                    VARIPTR cursor;
                    cursor.u8_ptr = this->font_storage.data() + font_working_headroom;

                    VARIPTR end;
                    end.u8_ptr = this->font_storage.data() + this->font_storage.size();

                    // The embedded editor font carries the original PVR texture,
                    // independently of the ETC1 assets selected for the host game.
                    const PlatformSelection font_platform{ANDROID_PVRTC_PLATFORM};
                    NuQFntInit(&cursor, end);
                }
                if (!system_qfont) {
                    LOG_ERR("editor: failed to initialize the embedded system font");
                    this->exit_status.store(1, std::memory_order_release);
                    return false;
                }

                eduiSetFont(system_qfont);
                this->register_original_modules();
                // edmainInit rebuilds RTL's menus; their previous item addresses may be reused.
                this->rtl_control_items.fill({});
                edmainInit(system_qfont, nullptr);
                edmainSetCursorEnabled(0);

                VARIPTR editor_cursor{};
                editor_cursor.u8_ptr = this->level_editor_storage.data();
                VARIPTR editor_end{};
                editor_end.u8_ptr = this->level_editor_storage.data() + this->level_editor_storage.size();
                theLevelEditor.Initalise(editor_cursor, editor_end, 1);
                this->initialize_menu_classes();

                eduimenu_s *menu = edGetMainMenu();
                if (menu)
                    menu->y = main_menu_top;
                LOG_INFO("editor: shell initialized menu=%p items=%p..%p size=%dx%d", menu,
                         menu ? menu->first : nullptr, menu ? menu->last : nullptr, menu ? menu->width : 0,
                         menu ? menu->height : 0);
                return true;
            }

            void refresh_scene_objects() {
                if (!WORLD->loaded) {
                    this->registered_scene = nullptr;
                    return;
                }
                nugscn_s *scene = WORLD->current_gscn;
                if (!scene || scene == this->registered_scene)
                    return;

                if (this->active_view == EditorView::level) {
                    this->requested_view = EditorView::game;
                    this->return_to_game();
                }
                theLevelEditor.reset_pending = 0;
                if (this->registered_scene_id >= 0) {
                    if (theClassEditor.selected_objects.first) {
                        ClassObject empty{};
                        theClassEditor.SelectObject(empty, 0);
                    }
                    theClassEditor.current_object = {};
                    theLevelEditor.ClearLevel(this->registered_scene_id);
                }
                const i32 scene_id = theLevelEditor.AddScene(const_cast<char *>("GAME"), scene, 1);
                std::snprintf(theLevelEditor.scenes[scene_id].directory,
                              sizeof(theLevelEditor.scenes[scene_id].directory), "%s", this->save_directory.c_str());
                theLevelEditor.scenes[scene_id].editable = 1;
                const i32 special_count = NuGScnNumSpecials(scene);
                theSceneObjectHelper.PreLoadInitialisation(nullptr, nullptr);
                this->registered_scene_id = scene_id;
                this->registered_scene = scene;
                i32 enumerated = 0;
                HostSceneObject *first = nullptr;
                for (void *object = this->next_scene_object(nullptr); object != nullptr;
                     object = this->next_scene_object(object)) {
                    if (!first)
                        first = static_cast<HostSceneObject *>(object);
                    ++enumerated;
                }
                const char *first_name = first ? first->GetName() : nullptr;
                LOG_INFO("editor: registered %d specials, enumerated %d (first: %s)", special_count, enumerated,
                         first_name ? first_name : "none");
            }

            void render_scene_object_debug() const {
                // The original helper renders its editor-owned objects from a linked list.
                // Draw hidden scene specials separately while the Level Editor is active.
                if (this->registered_scene_id >= 0 &&
                    (theSceneObjectHelper.show_hidden_solid != 0 || theSceneObjectHelper.show_hidden_wire != 0)) {
                    const HostSceneObject *objects = theSceneObjectHelper.scenes[this->registered_scene_id];
                    const i32 count = theSceneObjectHelper.scene_counts[this->registered_scene_id];
                    for (i32 index = 0; index < count; ++index) {
                        const HostSceneObject &object = objects[index];
                        if (object.reserved_0x28 || object.GetVisibility() != 0 || !object.Exists())
                            continue;

                        if (theSceneObjectHelper.show_hidden_solid != 0) {
                            EdDrawBegin(0);
                            object.Render(nullptr);
                            EdDrawEnd();
                        } else {
                            this->draw_object_wire_sphere(object, 0x80808080);
                        }
                    }
                }
            }

            void draw_object_wire_sphere(const HostSceneObject &object, i32 colour) const {
                const VuVec *position = object.GetCurrentPosition();
                if (!position)
                    return;
                const float radius = std::max(object.GetRadius(), 0.01f);
                EdDrawBegin(0);
                EdDrawLineCircleX(*position, radius, colour, 16);
                EdDrawLineCircleY(*position, radius, colour, 16);
                EdDrawLineCircleZ(*position, radius, colour, 16);
                EdDrawEnd();
            }

            void *next_scene_object(void *previous) {
                return theSceneObjectHelper.GetNextObject(previous);
            }

            void bind_scene_object_selection() {
                eduimenu_s *menu = eduiGetActiveMenu();
                if (!menu || !menu->first || menu->first->type != 0x14)
                    return;
                auto *filter = static_cast<edui_filter_s *>(menu->first);
                for (eduiitem_s *candidate = filter->next; candidate; candidate = candidate->next) {
                    if (candidate->type == 0)
                        static_cast<edui_sel_s *>(candidate)->selected = &EditorSession::select_scene_object;
                }
                for (eduiitem_s *candidate = filter->first_child; candidate; candidate = candidate->next) {
                    if (candidate->type == 0)
                        static_cast<edui_sel_s *>(candidate)->selected = &EditorSession::select_scene_object;
                }
            }

            static void select_scene_object(eduimenu_s *, eduiitem_s *item, u32) {
                auto *object = static_cast<HostSceneObject *>(item->data_ptr);
                if (!object)
                    return;
                // CloseMenu detaches the submenu but defers destroying the root.
                // Retain the detached branch until the next frame, after this
                // selection callback has returned to the menu processor.
                auto &session = instance();
                if (edLevelActiveMenu && edLevelActiveMenu->child)
                    session.detached_level_menus.push_back(edLevelActiveMenu->child);
                ClassObject selection{theClassEditor.pending_object.ed_class, object, nullptr};
                theClassEditor.pending_object = selection;
                theClassEditor.current_object = selection;
                theClassEditor.SelectObject(selection, 0);
                // The original list callback focuses the camera on selection.
                // Mouse selection in the host editor should only select; keep
                // the original focus behavior for keyboard activation.
                if (NuMouseReadButtons() == 0)
                    theClassEditor.FocusSelected();
                LOG_INFO("editor: selected scene object %s", object->GetName());
                theLevelEditor.CloseMenu();
            }

            void enter_editor(EditorView view) {
                eduiSetUsingMenuFocus(1);
                editor_active = 1;
                this->previous_editor_buttons = this->editor_buttons.load(std::memory_order_acquire);
                this->capture_game_input.store(true, std::memory_order_release);
                if (view == EditorView::level) {
                    this->configure_level_camera();
                    edmainSetCursorEnabled(1);
                    saga::host::set_editor_mouse_enabled(true);
                    const auto [mouse_x, mouse_y] = saga::host::editor_mouse_position();
                    eduiSetCursorCoords(std::clamp(mouse_x, 0.0f, 1.0f), std::clamp(mouse_y, 0.0f, 1.0f));
                    edLevelDestroyActiveMenu = 0;
                    theLevelEditor.Enter();
                    theLevelEditor.CreateMenu();
                    eduiSetActiveMenu(edLevelActiveMenu);
                    eduiSetDefaultActiveMenu(edLevelActiveMenu);
                } else {
                    edmainSetCursorEnabled(0);
                    eduiSetActiveMenu(this->module_menu ? this->module_menu : edGetMainMenu());
                    eduiSetDefaultActiveMenu(edGetMainMenu());
                }
                this->active_view = view;
                this->lifecycle = Lifecycle::in_editor;
                this->free_camera_ready.store(view == EditorView::level && this->free_camera_enabled,
                                              std::memory_order_release);
                LOG_INFO("editor: entered %s", view == EditorView::level ? "Level Editor" : "module editors");
            }

            void return_to_game() {
                saga::host::set_editor_arrow_selection_enabled(true);
                this->free_camera_ready.store(false, std::memory_order_release);
                saga::host::set_editor_mouse_enabled(false);
                edmainSetCursorEnabled(0);
                if (this->active_view == EditorView::level) {
                    edLevelDestroyActiveMenu = 0;
                    theLevelEditor.Exit();
                    edmainExtCamera(nullptr);
                    theLevelEditor.field_0x28 = 1;
                    this->destroy_level_menu();
                } else if (this->active_view == EditorView::modules) {
                    this->module_menu = eduiGetActiveMenu();
                }
                eduiSetActiveMenu(nullptr);
                this->capture_game_input.store(false, std::memory_order_release);
                editor_active = 0;
                eduiSetUsingMenuFocus(0);
                this->active_view = EditorView::game;
                this->lifecycle = Lifecycle::in_game;
                LOG_INFO("editor: returned to game");
            }

            void configure_level_camera() {
                // The game remains visible while the editor runs. Its scenery and
                // editor geometry must use the same projection in free-camera mode.
                edmainExtCamera(this->free_camera_enabled ? &global_camera : nullptr);
                theLevelEditor.field_0x28 = this->free_camera_enabled ? 1 : 0;
            }

            void destroy_level_menu() {
                std::vector<eduimenu_s *> children;
                const auto collect_branch = [&children](eduimenu_s *menu) {
                    for (; menu && std::find(children.begin(), children.end(), menu) == children.end();
                         menu = menu->child)
                        children.push_back(menu);
                };
                collect_branch(edLevelActiveMenu ? edLevelActiveMenu->child : nullptr);
                for (eduimenu_s *menu : this->detached_level_menus)
                    collect_branch(menu);
                this->detached_level_menus.clear();
                if (edLevelActiveMenu)
                    edLevelActiveMenu->child = nullptr;
                eduiSetActiveMenu(nullptr);
                if (edLevelPinnedMenu == edLevelActiveMenu ||
                    std::find(children.begin(), children.end(), edLevelPinnedMenu) != children.end())
                    edLevelPinnedMenu = nullptr;
                for (auto menu = children.rbegin(); menu != children.rend(); ++menu) {
                    (*menu)->parent = nullptr;
                    (*menu)->child = nullptr;
                    eduiMenuDestroy(*menu);
                }
                if (edLevelActiveMenu) {
                    eduiMenuDestroy(edLevelActiveMenu);
                    edLevelActiveMenu = nullptr;
                }
            }

            void register_original_modules() {
                const std::array modules{&edptldesc, &edgradesc,  &edbridesc,   &edanimdesc,
                                         &edrtldesc, &edpartdesc, &edTimingDesc};
                for (auto module = modules.rbegin(); module != modules.rend(); ++module)
                    edmainRegister(*module);
            }

            void initialize_menu_classes() {
                if (theRegistry.class_count != 0)
                    return;

                thePlaceableHelper.Initialise();
                theSceneObjectHelper.Initialise();

                theSplineHelper.Initialise();
                LOG_INFO("editor: initialized original Placeable, SceneObject, Spline, and Knot classes");
            }

            static void rtl_control_selected(eduimenu_s *menu, eduiitem_s *item, u32 value) {
                auto &session = instance();
                for (const RtlControlItem &watched : session.rtl_control_items) {
                    if (watched.item != item)
                        continue;
                    watched.selected(menu, item, value);
                    session.rtl_control_index = item->data;
                    return;
                }
            }

            void track_rtl_control_menu() {
                eduimenu_s *menu = this->active_root_menu();
                if (!menu || !menu->title ||
                    (std::strcmp(menu->title, "Light Editor Options") != 0 &&
                     std::strcmp(menu->title, "Fog Editor Options") != 0))
                    return;
                for (eduiitem_s *item = menu->first; item; item = item->next) {
                    if (item->type != 2 || item->selection_group != 4 || item->data < 0 || item->data > 1 ||
                        !item->text ||
                        (std::strcmp(item->text, "Ralph Controls") != 0 &&
                         std::strcmp(item->text, "Steve Controls") != 0))
                        continue;
                    auto found = std::find_if(this->rtl_control_items.begin(), this->rtl_control_items.end(),
                                              [item](const RtlControlItem &watched) { return watched.item == item; });
                    if (found != this->rtl_control_items.end())
                        continue;
                    auto free = std::find_if(this->rtl_control_items.begin(), this->rtl_control_items.end(),
                                             [](const RtlControlItem &watched) { return watched.item == nullptr; });
                    if (free == this->rtl_control_items.end())
                        return;
                    auto *selection = static_cast<edui_sel_s *>(item);
                    *free = {item, selection->selected};
                    selection->selected = &EditorSession::rtl_control_selected;
                }
            }

            void configure_rtl_pad(nupad_s &pad, u32 &buttons) {
                const u32 keys = this->editor_keys.load(std::memory_order_acquire);
                const auto held = [keys](EditorKey key) { return (keys & key) != 0; };
                const bool menu_open = rtled_menu_active || delete_menu_active || edrtl_active_menu;
                const bool escape_held = held(key_escape);
                const bool escape_pressed = escape_held && !this->previous_rtl_escape_held;
                this->previous_rtl_escape_held = escape_held;
                buttons = 0;
                pad.is_valid = 1;
                pad.has_analog_sticks = 1;
                pad.has_analog_buttons = 1;
                if (menu_open) {
                    if (held(key_up))
                        buttons |= GAMEPAD_DUP;
                    if (held(key_down))
                        buttons |= GAMEPAD_DDOWN;
                    if (held(key_left))
                        buttons |= GAMEPAD_DLEFT;
                    if (held(key_right))
                        buttons |= GAMEPAD_DRIGHT;
                    if (held(key_enter))
                        buttons |= GAMEPAD_MENUSELECT;
                    if (escape_pressed)
                        buttons |= editor_cancel_button;
                    return;
                }

                // Match the control table selected in the original RTL menu.
                const u32 *ctl = rtl_control_buttons[this->rtl_control_index];
                if (escape_pressed)
                    buttons |= GAMEPAD_START;
                if (held(key_tab))
                    buttons |= 0x4000;
                if (held(key_menu))
                    buttons |= ctl[4];
                if (held(key_add))
                    buttons |= ctl[0];
                if (held(key_delete))
                    buttons |= ctl[1];
                if (held(key_drag))
                    buttons |= ctl[2];
                if (held(key_inspect))
                    buttons |= ctl[5];
                if (held(key_lock))
                    buttons |= ctl[3];
                if (held(key_group) || held(key_previous))
                    buttons |= ctl[6];
                if (held(key_group_next) || held(key_next))
                    buttons |= ctl[7];
                if (held(key_burn_previous))
                    buttons |= 0x2;

                pad.analog_left_x = held(key_strafe_left) ? 0 : held(key_strafe_right) ? 0xff : 0x80;
                pad.analog_left_y = held(key_forward) ? 0 : held(key_backward) ? 0xff : 0x80;
                pad.analog_right_x = held(key_look_left) ? 0xff : held(key_look_right) ? 0 : 0x80;
                pad.analog_right_y = held(key_look_up) ? 0 : held(key_look_down) ? 0xff : 0x80;
                pad.analog_l1 = held(key_raise) ? 0xff : 0;
                pad.analog_l2 = held(key_lower) ? 0xff : 0;
                pad.analog_r1 = held(key_zoom_in) ? 0xff : 0;
                pad.analog_r2 = held(key_zoom_out) ? 0xff : 0;
            }

            [[nodiscard]] nupad_s make_editor_pad() {
                nupad_s pad{};
                pad.analog_left_x = 0x80;
                pad.analog_left_y = 0x80;
                pad.analog_right_x = 0x80;
                pad.analog_right_y = 0x80;

                u32 buttons = this->editor_buttons.load(std::memory_order_acquire);
                if (this->active_view == EditorView::modules && edmainCurrent() == &edrtldesc)
                    this->configure_rtl_pad(pad, buttons);
                pad.digital_buttons = buttons;
                pad.digital_buttons_prev = this->previous_editor_buttons;
                pad.digital_buttons_pressed = buttons & ~this->previous_editor_buttons;
                pad.digital_buttons_released = ~buttons & this->previous_editor_buttons;
                this->previous_editor_buttons = buttons;
                return pad;
            }

            [[nodiscard]] std::vector<MenuPosition> capture_menu_positions() const {
                std::vector<MenuPosition> positions;
                for (eduimenu_s *menu = this->active_root_menu(); menu; menu = menu->child)
                    positions.push_back({menu, menu->x});
                return positions;
            }

            void constrain_menus(const std::vector<MenuPosition> &previous_positions) const {
                const i32 maximum_step = std::max(1, static_cast<i32>(std::ceil(FRAMETIME * menu_movement_speed)));
                for (eduimenu_s *menu = this->active_root_menu(); menu; menu = menu->child) {
                    const auto previous =
                        std::find_if(previous_positions.begin(), previous_positions.end(),
                                     [menu](const MenuPosition &position) { return position.menu == menu; });
                    if (previous != previous_positions.end()) {
                        const i32 movement = std::clamp(menu->x - previous->x, -maximum_step, maximum_step);
                        menu->x = previous->x + movement;
                    }

                    const i32 width = std::clamp(menu->field_24 > 0 ? menu->field_24 : menu->width, 0,
                                                 editor_width - menu_margin * 2);
                    const i32 height = std::clamp(menu->field_28 > 0 ? menu->field_28 : menu->height, 0,
                                                  editor_height - menu_margin * 2);
                    menu->x = std::clamp(menu->x, menu_margin, editor_width - menu_margin - width);
                    menu->y = std::clamp(menu->y, menu_margin, editor_height - menu_margin - height);
                }
            }

            [[nodiscard]] eduimenu_s *active_root_menu() const {
                if (this->active_view == EditorView::level)
                    return edLevelActiveMenu;
                if (eduimenu_s *active = eduiGetActiveMenu())
                    return eduiGetTopLevelParent(active);
                return edGetMainMenu();
            }

            [[nodiscard]] static bool is_playable(const LEVELDATA_s &level) {
                return (level.flags & LEVEL_GAMEPLAY) != 0 &&
                       (level.flags & (LEVEL_INTRO | LEVEL_MIDTRO | LEVEL_OUTRO | LEVEL_STATUS)) == 0;
            }

            EditorOptions options;
            Lifecycle lifecycle = Lifecycle::waiting_for_game;
            DestinationState destination_state = DestinationState::not_requested;
            LEVELDATA_s *destination = nullptr;
            nugscn_s *registered_scene = nullptr;
            i32 registered_scene_id = -1;
            std::string save_directory;
            EditorView requested_view = EditorView::modules;
            EditorView active_view = EditorView::game;
            eduimenu_s *module_menu = nullptr;
            std::vector<eduimenu_s *> detached_level_menus;
            u32 previous_editor_buttons = 0;
            bool previous_rtl_escape_held = false;
            i32 last_selection_count = -1;
            i32 last_property_menu_count = -1;
            std::atomic<unsigned> toggle_requests{0};
            std::atomic<u32> editor_buttons{0};
            std::atomic<u32> editor_keys{0};
            std::atomic<bool> capture_game_input{false};
            bool free_camera_enabled = false;
            std::atomic<bool> free_camera_ready{false};
            std::atomic<int> exit_status{no_exit_requested};
            alignas(16) std::array<u8, 64 * 1024> font_storage{};
            alignas(16) std::array<u8, 256 * 1024> level_editor_storage{};
            alignas(16) std::array<u8, editor_pool_size> editor_pool{};
            alignas(16) std::array<u8, rtl_undo_bytes> rtl_undo_storage{};
            bool editor_pool_installed = false;
            bool rtl_undo_installed = false;
            i32 rtl_control_index = 1;
            std::array<RtlControlItem, 4> rtl_control_items{};
        };

        void print_editor_usage(std::string_view executable) {
            std::cout << "Usage: " << executable << " editor [level-or-area]\n\n"
                      << "With no destination, the game follows its normal startup flow.\n"
                      << "A destination may be a gameplay level name or an area file name.\n"
                      << "F1 toggles module editors; F2 toggles the Level Editor.\n"
                      << "F3 toggles free camera in the Level Editor; numpad 4/5/6/8 rotate, Shift moves.\n"
                      << "Enter selects; Escape goes back.\n"
                      << "Realtime Light Editor default keys: M options, N add, R drag, I inspect, Delete remove, "
                         "Tab next mode; WASD move, Q/E lower/raise, Z/X zoom, numpad 4/6/8/5 look.\n";
        }

        using EditorParseResult = std::variant<EditorOptions, int>;

        EditorParseResult parse_editor_options(const Invocation &invocation) {
            if (invocation.arguments.size() == 1 &&
                (invocation.arguments[0] == "--help" || invocation.arguments[0] == "-h")) {
                print_editor_usage(invocation.executable);
                return 0;
            }
            if (invocation.arguments.size() > 1) {
                std::cerr << "editor accepts at most one level or area name\n\n";
                print_editor_usage(invocation.executable);
                return 2;
            }

            EditorOptions options;
            if (!invocation.arguments.empty())
                options.destination.emplace(invocation.arguments[0]);
            return options;
        }

        int run_editor_program(const Invocation &invocation) {
            auto parsed = parse_editor_options(invocation);
            if (const auto *status = std::get_if<int>(&parsed))
                return *status;

            EditorSession &editor = EditorSession::instance();
            editor.start(std::get<EditorOptions>(std::move(parsed)));

            WindowOptions window;
            window.hooks.handle_event = &EditorSession::handle_event;
            window.hooks.filter_game_input = &EditorSession::filter_game_input;
            window.hooks.free_camera_ready = &EditorSession::is_free_camera_ready;
            window.hooks.requested_exit_status = &EditorSession::requested_exit_status;
            window.camera_free = true;
            window.hide_system_cursor = true;
            return run_window(window);
        }

    } // namespace

    const Program &editor_program() {
        static constexpr Program program{"editor", "Run the game with the editor shell", run_editor_program};
        return program;
    }

} // namespace saga::host::harness
