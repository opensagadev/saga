#include "legoapi/legoapi_types.h"
#include "gameapi/edtools/edui.h"
#include "nu2api/nucore/nuvuvec.hpp"

eduimenu_s *edLevelNextMenu;

void BaseEditor::Initialise(variptr_u &, variptr_u &, i32 value) {
    field_0x0c = value;
}

void ClassEditor::RegisterTool(EdTool &tool) {
    tool.next = NULL;
    tool.previous = last_tool;
    if (last_tool != NULL) {
        last_tool->next = &tool;
    }
    EdTool *old_first = first_tool;
    last_tool = &tool;
    if (old_first == NULL) {
        first_tool = &tool;
    }
    ++tool_count;
}

void ClassEditor::UpdateSnapRay(VuVec &position) {
    if (snap_mode == 1) {
        snap_ray.x = 0.0f;
        snap_ray.y = -1000.0f;
        snap_ray.z = 0.0f;
    } else if (snap_mode == 2) {
        snap_ray = position;
    }
}

nugscn_s *LevelEditor::GetScene(i32 index) {
    if (static_cast<u32>(index) < 10) {
        return scenes[index].scene;
    }
    return NULL;
}

void LevelEditor::RegisterEditor(BaseEditor &editor) {
    editor.next = NULL;
    editor.previous = last_editor;
    if (last_editor != NULL) {
        last_editor->next = &editor;
    }
    BaseEditor *old_first = first_editor;
    last_editor = &editor;
    if (old_first == NULL) {
        first_editor = &editor;
    }
    ++editor_count;
}

void LevelEditor::SetNextMenu(eduimenu_s *menu) {
    edLevelNextMenu = menu;
}

bool PropertyMenu::ContainsObject(void *object) {
    for (i32 i = 0; i < object_count; ++i) {
        if (objects[i].object == object) {
            return true;
        }
    }
    return false;
}

bool PropertyTool::HasActiveMenu() {
    return active_menu != NULL;
}

void PropertyTool::SetMenuControl(eduimenu_s *menu, EdControl *control) {
    for (PropertyMenu *property_menu = active_menu; property_menu != NULL; property_menu = property_menu->next) {
        if (property_menu->menu == menu) {
            property_menu->control = control;
            return;
        }
    }
}

bool ClassObjectList::IsInList(ClassObject object) {
    for (ClassObjectListEntry *entry = first; entry != NULL; entry = entry->next) {
        if (entry->object == object.object && entry->reference == object.reference) {
            return true;
        }
    }
    return false;
}

bool ClassObjectList::IsInList(EdClass *ed_class) {
    for (ClassObjectListEntry *entry = first; entry != NULL; entry = entry->next) {
        if (entry->ed_class == ed_class) {
            return true;
        }
    }
    return false;
}

void cbEdLevelSetSliderInt(eduimenu_s *, eduiitem_s *item, u32) {
    *static_cast<i32 *>(item->data_ptr) = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}

void cbEdLevelSetSliderFloat(eduimenu_s *, eduiitem_s *item, u32) {
    *static_cast<f32 *>(item->data_ptr) = static_cast<edui_slider_s *>(item)->value;
}
