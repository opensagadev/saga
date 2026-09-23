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
#include "host/platform/keyboard.hpp"
#include "host/platform/mouse.hpp"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/misc/androidbatman.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nuplatform/nuplatform.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

extern eduimenu_s *edLevelActiveMenu;
extern eduimenu_s *edLevelPinnedMenu;
extern i32 edLevelDestroyActiveMenu;
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
                this->scene_objects.clear();
                theClassEditor.selected_objects = {};
                theClassEditor.current_object = {};
                this->requested_view = EditorView::modules;
                this->active_view = EditorView::game;
                this->module_menu = nullptr;
                this->previous_editor_buttons = 0;
                this->last_selection_count = -1;
                this->last_property_menu_count = -1;
                this->toggle_requests.store(0, std::memory_order_relaxed);
                this->editor_buttons.store(0, std::memory_order_relaxed);
                this->capture_game_input.store(false, std::memory_order_relaxed);
                this->free_camera_enabled = false;
                this->free_camera_ready.store(false, std::memory_order_relaxed);
                saga::host::set_editor_mouse_enabled(false);
                this->exit_status.store(no_exit_requested, std::memory_order_relaxed);
                set_frame_callback(&EditorSession::update);
                LOG_INFO("editor: F1 modules, F2 Level Editor, F3 free camera; Enter selects, Escape goes back");
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
                if (!instance().capture_game_input.load(std::memory_order_acquire))
                    return;
                char input = 0;
                if (event.key.key >= SDLK_A && event.key.key <= SDLK_Z)
                    input = static_cast<char>(event.key.key);
                else if (event.key.key >= SDLK_0 && event.key.key <= SDLK_9)
                    input = static_cast<char>(event.key.key);
                else if (event.key.key == SDLK_SPACE)
                    input = ' ';
                else if (event.key.key == SDLK_MINUS)
                    input = (event.key.mod & SDL_KMOD_SHIFT) ? '_' : '-';
                else if (event.key.key == SDLK_PERIOD)
                    input = '.';
                else if (event.key.key == SDLK_BACKSPACE)
                    input = '\b';
                if (input) {
                    std::lock_guard lock{instance().filter_input_mutex};
                    instance().pending_filter_input += input;
                }
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
            static constexpr int no_exit_requested = -1;
            static constexpr int editor_width = 640;
            static constexpr int editor_height = 448;
            static constexpr int menu_margin = 1;
            static constexpr int main_menu_top = 16;
            static constexpr f32 menu_movement_speed = 240.0f;
            // The editor checks bit 4 directly; the game remaps GAMEPAD_MENUCANCEL to bit 5 at startup.
            static constexpr u32 editor_cancel_button = 1u << 4;

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
                    this->update_scene_filter();
                    nupad_s *pads[]{&pad, nullptr};
                    ThingProcessData process_data{FRAMETIME, static_cast<u32>(Paused), pads, 2};
                    theLevelEditor.ProcessEvenWhenPaused(&process_data);
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
                const i32 process_result = edmainProcess(FRAMETIME, &pad);
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
                const i32 scene_id = theLevelEditor.AddScene(const_cast<char *>("GAME"), scene, 1);
                std::snprintf(theLevelEditor.scenes[scene_id].directory,
                              sizeof(theLevelEditor.scenes[scene_id].directory), "%s", this->save_directory.c_str());
                theLevelEditor.scenes[scene_id].editable = 1;
                const i32 special_count = NuGScnNumSpecials(scene);
                if (theClassEditor.selected_objects.first) {
                    ClassObject empty{};
                    theClassEditor.SelectObject(empty, 0);
                }
                theClassEditor.current_object = {};
                if (this->registered_scene_id >= 0 && this->registered_scene_id < 10) {
                    theSceneObjectHelper.scenes[this->registered_scene_id] = nullptr;
                    theSceneObjectHelper.scene_counts[this->registered_scene_id] = 0;
                }
                while (theSceneObjectHelper.owned_first)
                    theSceneObjectHelper.DestroyObject(theSceneObjectHelper.owned_first, 0);
                this->scene_objects.clear();
                this->scene_objects.resize(special_count);
                for (i32 index = 0; index < special_count; ++index) {
                    HostSceneObject &object = this->scene_objects[index];
                    NuGScnGetSpecial(&object.special, scene, index);
                    object.attributes = 0x12400000;
                    object.reserved_0x28 = 0;
                }
                theSceneObjectHelper.scenes[scene_id] = this->scene_objects.data();
                theSceneObjectHelper.scene_counts[scene_id] = special_count;
                theSceneObjectHelper.scene_object_count = special_count;
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
                // The host currently registers the scene specials directly, so draw the
                // hidden subset here while the Level Editor's 3D scene is active.
                if (theSceneObjectHelper.show_hidden_solid != 0 || theSceneObjectHelper.show_hidden_wire != 0) {
                    for (const HostSceneObject &object : this->scene_objects) {
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

                // Draw selection feedback until the original sphere primitive is available.
                const ClassObjectList &selected = theClassEditor.selected_objects;
                if (selected.count <= 0)
                    return;
                i32 remaining = selected.count;
                for (ClassObjectListEntry *entry = selected.first; entry && remaining > 0;
                     entry = entry->next, --remaining) {
                    bool found = false;
                    for (const HostSceneObject &object : this->scene_objects) {
                        if (entry->object == &object && object.Exists()) {
                            this->draw_object_wire_sphere(object, 0xff800000);
                            found = true;
                            break;
                        }
                    }
                    if (found)
                        continue;
                    for (SceneInstance *object = theSceneObjectHelper.owned_first; object; object = object->next) {
                        if (entry->object == object) {
                            this->draw_object_wire_sphere(*object, 0xff800000);
                            break;
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

            static i32 process_scene_filter(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *) {
                auto *filter = static_cast<edui_filter_s *>(item);
                char *query = filter->property_text;
                if (!query)
                    return 0;

                bool changed = false;
                for (eduiitem_s *candidate = menu->first; candidate;) {
                    eduiitem_s *next = candidate->next;
                    if (candidate->type != 0x12 && candidate->type != 0x14 && candidate->text &&
                        !NuStrIStr(candidate->text, query)) {
                        eduiMenuRemoveItem(menu, candidate);
                        eduiItemFilterAddItem(filter, candidate);
                        changed = true;
                    }
                    candidate = next;
                }
                for (eduiitem_s *candidate = filter->first_child; candidate;) {
                    eduiitem_s *next = candidate->next;
                    if (!query[0] || (candidate->text && NuStrIStr(candidate->text, query))) {
                        eduiItemFilterRemoveItem(filter, candidate);
                        eduiMenuAddItem(menu, candidate);
                        changed = true;
                    }
                    candidate = next;
                }
                if (changed)
                    eduiMenuSortItemsByTxt(menu);
                return 0;
            }

            static i32 render_scene_filter(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width) {
                auto &session = instance();
                if (!session.scene_object_item_renderer)
                    return 0;
                auto *filter = static_cast<edui_filter_s *>(item);
                std::string label = "FILTER: ";
                if (filter->property_text)
                    label += filter->property_text;
                char *original = item->text;
                item->text = label.data();
                const i32 height = session.scene_object_item_renderer(menu, item, x, y, width);
                item->text = original;
                return height;
            }

            void update_scene_filter() {
                std::string input;
                {
                    std::lock_guard lock{this->filter_input_mutex};
                    input.swap(this->pending_filter_input);
                }
                eduimenu_s *menu = eduiGetActiveMenu();
                if (!menu || !menu->first || menu->first->type != 0x14)
                    return;
                auto *filter = static_cast<edui_filter_s *>(menu->first);
                if (filter->next && filter->next->render && !this->scene_object_item_renderer)
                    this->scene_object_item_renderer = filter->next->render;
                filter->process = &EditorSession::process_scene_filter;
                filter->render = &EditorSession::render_scene_filter;
                for (eduiitem_s *candidate = filter->next; candidate; candidate = candidate->next) {
                    if (candidate->type == 0)
                        static_cast<edui_sel_s *>(candidate)->selected = &EditorSession::select_scene_object;
                }
                for (eduiitem_s *candidate = filter->first_child; candidate; candidate = candidate->next) {
                    if (candidate->type == 0)
                        static_cast<edui_sel_s *>(candidate)->selected = &EditorSession::select_scene_object;
                }
                if (menu->selected != filter || input.empty())
                    return;
                std::string query = filter->property_text ? filter->property_text : "";
                for (const char character : input) {
                    if (character == '\b') {
                        if (!query.empty())
                            query.pop_back();
                    } else if (query.size() < 63) {
                        query += character;
                    }
                }
                eduiItemPropSetText(filter, query.data());
            }

            static void select_scene_object(eduimenu_s *, eduiitem_s *item, u32) {
                auto *object = static_cast<HostSceneObject *>(item->data_ptr);
                if (!object)
                    return;
                ClassObject selection{theClassEditor.pending_object.ed_class, object, nullptr};
                theClassEditor.pending_object = selection;
                theClassEditor.current_object = selection;
                theClassEditor.SelectObject(selection, 0);
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

            void destroy_level_menu() {
                std::vector<eduimenu_s *> children;
                for (eduimenu_s *menu = edLevelActiveMenu ? edLevelActiveMenu->child : nullptr; menu;
                     menu = menu->child)
                    children.push_back(menu);
                if (edLevelActiveMenu)
                    edLevelActiveMenu->child = nullptr;
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

            [[nodiscard]] nupad_s make_editor_pad() {
                nupad_s pad{};
                pad.analog_left_x = 0x80;
                pad.analog_left_y = 0x80;
                pad.analog_right_x = 0x80;
                pad.analog_right_y = 0x80;

                const u32 buttons = this->editor_buttons.load(std::memory_order_acquire);
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
            u32 previous_editor_buttons = 0;
            i32 last_selection_count = -1;
            i32 last_property_menu_count = -1;
            std::atomic<unsigned> toggle_requests{0};
            std::atomic<u32> editor_buttons{0};
            std::atomic<bool> capture_game_input{false};
            bool free_camera_enabled = false;
            std::atomic<bool> free_camera_ready{false};
            std::atomic<int> exit_status{no_exit_requested};
            alignas(16) std::array<u8, 64 * 1024> font_storage{};
            alignas(16) std::array<u8, 256 * 1024> level_editor_storage{};
            alignas(16) std::array<u8, editor_pool_size> editor_pool{};
            bool editor_pool_installed = false;
            std::vector<HostSceneObject> scene_objects;
            std::mutex filter_input_mutex;
            std::string pending_filter_input;
            decltype(eduiitem_s::render) scene_object_item_renderer = nullptr;
        };

        void print_editor_usage(std::string_view executable) {
            std::cout << "Usage: " << executable << " editor [level-or-area]\n\n"
                      << "With no destination, the game follows its normal startup flow.\n"
                      << "A destination may be a gameplay level name or an area file name.\n"
                      << "F1 toggles module editors; F2 toggles the Level Editor.\n"
                      << "F3 toggles free camera in the Level Editor; numpad 4/5/6/8 rotate, Shift moves.\n"
                      << "Enter selects; Escape goes back.\n";
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
            return run_window(window);
        }

    } // namespace

    const Program &editor_program() {
        static constexpr Program program{"editor", "Run the game with the editor shell", run_editor_program};
        return program;
    }

} // namespace saga::host::harness
