#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"

void BaseEditor::ReadBuffer(void **, void *, i32) {
    STUBBED();
}

void BaseEditor::WriteBeginBlock(i32, i32) {
    STUBBED();
}

void BaseEditor::WriteEndBlock(i32) {
    STUBBED();
}

void BaseEditor::WriteMetaData(i32, i32, i32, i32) {
    STUBBED();
}

void CursorTool::Initialise(variptr_u &, variptr_u &, i32) {
    STUBBED();
}

void CursorTool::Process(EdInputContext &) {
    STUBBED();
}

void CursorTool::Render() {
    STUBBED();
}

ClassEditor::ClassEditor() {
    STUBBED();
}

void ClassEditor::AddMenuItems(eduimenu_s *) {
    STUBBED();
}

void ClassEditor::ClearLevel(i32) {
    STUBBED();
}

void ClassEditor::CreateObject() {
    STUBBED();
}

void ClassEditor::CreateObject(ClassObject &) {
    STUBBED();
}

void ClassEditor::CreateObject(EdClass *) {
    STUBBED();
}

void ClassEditor::CreateObject(i32) {
    STUBBED();
}

void ClassEditor::Enter() {
    STUBBED();
}

void ClassEditor::Exit() {
    STUBBED();
}

void ClassEditor::Flush() {
    STUBBED();
}

void ClassEditor::Initialise(variptr_u &, variptr_u &, i32) {
    STUBBED();
}

void ClassEditor::Process(EdInputContext &) {
    STUBBED();
}

void ClassEditor::ReadBlock(DATAPTR *) {
    STUBBED();
}

void ClassEditor::Render() {
    STUBBED();
}

void ClassEditor::Serialise(EdStream &) {
    STUBBED();
}

void ClassEditor::WriteBlock(i32) {
    STUBBED();
}

void ClassEditor::DestroySelectedObjects() {
    STUBBED();
}

void ClassEditor::DestroySelectedObjectsNow() {
    STUBBED();
}

void ClassEditor::DrawObjectSphere(ClassObject &, i32) {
    STUBBED();
}

void ClassEditor::Editable(void *, EdClass *, i32) {
    STUBBED();
}

void ClassEditor::FindNearestObject(VuVec &, ClassObject &, ClassObject &, i32) {
    STUBBED();
}

void ClassEditor::FindNearestObject(VuVec &, ClassObject &, i32) {
    STUBBED();
}

void ClassEditor::FindNearestObject(VuVec &, VuVec &, ClassObject &, ClassObject &, i32) {
    STUBBED();
}

void ClassEditor::FindNearestObject(VuVec &, VuVec &, ClassObject &, i32) {
    STUBBED();
}

void ClassEditor::FocusSelected() {
    STUBBED();
}

void ClassEditor::InitialiseObject(ClassObject &) {
    STUBBED();
}

void ClassEditor::IsSelectedClass(EdClass *) {
    STUBBED();
}

void ClassEditor::IsSelectedObject(ClassObject &) {
    STUBBED();
}

void ClassEditor::IsSelectedObject(void *, EdRef *) {
    STUBBED();
}

void ClassEditor::IsUniqueName(char *) {
    STUBBED();
}

void ClassEditor::MakeUniqueName(char const *, char *, i32) {
    STUBBED();
}

void ClassEditor::PostLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
    STUBBED();
}

void ClassEditor::PostSaveInitialisation() {
    STUBBED();
}

void ClassEditor::PreLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
    STUBBED();
}

void ClassEditor::PreSaveInitialisation() {
    STUBBED();
}

void ClassEditor::SelectLED(i32) {
    STUBBED();
}

void ClassEditor::SelectObject(ClassObject &, i32) {
    STUBBED();
}

void ClassEditor::SetMode(i32) {
    STUBBED();
}

void ClassEditor::SetViewMenuHilight(eduimenu_s *) {
    STUBBED();
}

void ClassEditor::SnapPoint(VuVec &) {
    STUBBED();
}

void ClassEditor::UpdateClassFilter(EdInputContext &) {
    STUBBED();
}

void ClassEditor::UpdateLists(MemoryBuffer *, MemoryBuffer *) {
    STUBBED();
}

void ClassEditor::UpdateSelectedObjects(EdInputContext &) {
    STUBBED();
}

void ClassEditor::ViewSelected() {
    STUBBED();
}

void ClassEditor::cbDestroyMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

void ClassEditor::cbDestroyObject(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassDeleteObject(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassExportMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassFileMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassImportMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassModeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassNewMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassNewObject(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassRemoveDuplicates(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassSelectClassMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassSelectObject(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassSelectObjectMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassSetMode(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassSetPinned(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassSetSnap(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassSetView(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassSnapMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassToolsMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassViewMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdCopySelectedObject(EdInputContext &) {
    STUBBED();
}

void ClassEditor::cbEdCreateClassNewObject(i32) {
    STUBBED();
}

void ClassEditor::cbEdFilterLED(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdLevelDeselectAll(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdLevelSelectAll(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdPadSetManipulatorMode(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbFileSelected(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassObject::GetName(char *, i32) {
    STUBBED();
}

void ClassObject::Set(char *) {
    STUBBED();
}

void LevelEditor::AddInfoText(char *) {
    STUBBED();
}

void LevelEditor::AddScene(char *, nugscn_s *, i32) {
    STUBBED();
}

void LevelEditor::AddText(char *) {
    STUBBED();
}

void LevelEditor::BeginMultiLoad(variptr_u *, variptr_u *) {
    STUBBED();
}

void LevelEditor::ClearLevel(i32) {
    STUBBED();
}

void LevelEditor::CloseMenu() {
    STUBBED();
}

void LevelEditor::CreateEditorList(eduimenu_s *, eduiitem_s *) {
    STUBBED();
}

void LevelEditor::CreateMenu() {
    STUBBED();
}

void LevelEditor::Display(ThingRenderData *) {
    STUBBED();
}

void LevelEditor::DrawInfoText(char **, i32, i32, i32, i32, i32, i32, i32) {
    STUBBED();
}

void LevelEditor::EndMultiLoad(variptr_u *, variptr_u *) {
    STUBBED();
}

void LevelEditor::Enter() {
    STUBBED();
}

void LevelEditor::Exit() {
    STUBBED();
}

void LevelEditor::FindSceneId(char *) {
    STUBBED();
}

void LevelEditor::Flush() {
    STUBBED();
}

LevelEditorScene *LevelEditor::GetEdScene(i32 index) {
    if (static_cast<u32>(index) >= 10) {
        return NULL;
    }
    return &scenes[index];
}

void LevelEditor::GetScene(char *) {
    STUBBED();
}

void LevelEditor::Initalise(variptr_u &, variptr_u &, i32) {
    STUBBED();
}

void LevelEditor::IsActiveScene(nugscn_s *) {
    STUBBED();
}

void LevelEditor::IsEditable(i32) {
    STUBBED();
}

LevelEditor::LevelEditor() {
    STUBBED();
}

void LevelEditor::Load(char *, variptr_u *, variptr_u *, i32) {
    STUBBED();
}

void LevelEditor::LoadState(variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *) {
    STUBBED();
}

void LevelEditor::ProcessEvenWhenPaused(ThingProcessData *) {
    STUBBED();
}

void LevelEditor::ReadStream(EdFileInputStream &) {
    STUBBED();
}

void LevelEditor::Reset() {
    reset_pending = 0;
}

void LevelEditor::Save() {
    STUBBED();
}

void LevelEditor::SaveState(i32, variptr_u *, variptr_u *) {
    STUBBED();
}

void LevelEditor::SaveState(variptr_u *, variptr_u *) {
    STUBBED();
}

void LevelEditor::SetPadText(i32, char *) {
    STUBBED();
}

void LevelEditor::SetSaveFilename(char *) {
    STUBBED();
}

void LevelEditor::WriteStream(EdFileOutputStream &) {
    STUBBED();
}

void PropertyMenu::AddObject(ClassObject &) {
    STUBBED();
}

void PropertyMenu::ClearObjecs() {
    object_count = 0;
}

bool PropertyMenu::ContainsObject(ClassObject &object) {
    for (i32 i = 0; i < object_count; ++i) {
        if (objects[i].object == object.object && objects[i].reference == object.reference) {
            return true;
        }
    }
    return false;
}

void PropertyMenu::Destroy() {
    STUBBED();
}

void PropertyMenu::SelectAttr(i32) {
    STUBBED();
}

void PropertyTool::AddPropertyMenuItems(eduimenu_s *, EdClass *, void *, eduiitem_s *) {
    STUBBED();
}

void PropertyTool::AutoLocateMenu(PropertyMenu *) {
    STUBBED();
}

void PropertyTool::BringToFront(PropertyMenu *) {
    STUBBED();
}

void PropertyTool::CreatePropertyMenu(ClassObject &) {
    STUBBED();
}

void PropertyTool::FindItemMenu(PropertyMenu *, ClassItem *) {
    STUBBED();
}

void PropertyTool::GetActiveMenu(PropertyMenu *) {
    STUBBED();
}

void PropertyTool::GetClassName(EdRef *, char *) {
    STUBBED();
}

void PropertyTool::GetNextActiveMenu() {
    STUBBED();
}

void PropertyTool::GetNextDefaultActiveMenu(eduimenu_s *) {
    STUBBED();
}

void PropertyTool::GetTypeName(EdRef *, char *) {
    STUBBED();
}

void PropertyTool::Initialise(variptr_u &, variptr_u &, i32) {
    STUBBED();
}

void PropertyTool::Process(EdInputContext &) {
    STUBBED();
}

void PropertyTool::ProcessControls(EdInputContext &) {
    STUBBED();
}

void PropertyTool::ProcessMenu(EdInputContext &) {
    STUBBED();
}

PropertyTool::PropertyTool() {
    STUBBED();
}

void PropertyTool::RefreshMenuControls(PropertyMenu *) {
    STUBBED();
}

void PropertyTool::Render() {
    STUBBED();
}

void PropertyTool::RenderMenu(PropertyMenu *) {
    STUBBED();
}

void PropertyTool::RetrievePropertyMenu(ClassObject *, PropertyMenuList *) {
    STUBBED();
}

void PropertyTool::SelectAttr(i32) {
    STUBBED();
}

void PropertyTool::SetDefaultActiveMenu(PropertyMenu *) {
    STUBBED();
}

void PropertyTool::ToggleActiveMenu() {
    STUBBED();
}

void PropertyTool::ediGetMenuStartMetrics() {
    STUBBED();
}

void PropertyTool::ediMenuRetrieveMetrics(eduimenu_s *) {
    STUBBED();
}

void PropertyTool::ediMenuStoreMetrics(eduimenu_s *) {
    STUBBED();
}

void ClassObjectList::GetAveragePosition(VuVec &) {
    STUBBED();
}

void ClassObjectList::GetAveragePosition(VuVec &, float &) {
    STUBBED();
}

bool ClassObjectList::IsInList(void *object, EdRef *reference) {
    ClassObjectListEntry *entry = first;
    if (entry == NULL) {
        return false;
    }

    if (reference != NULL) {
        do {
            if (entry->object == object && entry->reference == reference) {
                return true;
            }
            entry = entry->next;
        } while (entry != NULL);
    } else {
        do {
            if (entry->object == object) {
                return true;
            }
            entry = entry->next;
        } while (entry != NULL);
    }
    return false;
}

void DumpAreaData(i32, i32) {
    STUBBED();
}

void cbEdLevelSave(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void areaEditor_Enter() {
    STUBBED();
}

void cbEdLevelDestroy(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

void cbEdLevelSetText(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void areaEditor_Render(i32, i32, float, float) {
    STUBBED();
}

void areaEditor_Process(nupad_s *) {
    STUBBED();
}

void cbEdLevelToggleInt(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void cbCEDeleteConfirmed(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void LightEverythingInEditor(void *) {
    STUBBED();
}

void cbEdLevelDestroyOnSelect(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void EdClass::SerialiseObject(EdStream &, void *) {
    STUBBED();
}

void EdClass::SerialiseObjectHeader(EdStream &, void *) {
    STUBBED();
}

void EdClass::CopyObject(void *, void *) {
    STUBBED();
}

void EdClass::AddType(EdRef *) {
    STUBBED();
}

void EdClass::Serialise(EdStream &, i32 *) {
    STUBBED();
}

void EdClass::GetStreamClasses(EdStream &, i32 *, i32 &, i32) {
    STUBBED();
}

void EdClass::FindTypeRef(char *, i32) {
    STUBBED();
}

void EdClass::SerialiseObject(EdStream &, void *, EdClass *, EdRegistry *) {
    STUBBED();
}

void EdClass::FindTypeRef(i32, i32) {
    STUBBED();
}

void EdClass::FindMember(EdMember *, void *, i32, i32) {
    STUBBED();
}

void EdClass::FindObject(char *) {
    STUBBED();
}

EditorSettings::EditorSettings() {
    STUBBED();
}

void EditorSettings::AddMenuItems(eduimenu_s *) {
    STUBBED();
}

void EditorSettings::Serialise(EdStream &) {
    STUBBED();
}

eduimenu_s *edLevelNextMenu;

void BaseEditor::Initialise(variptr_u &, variptr_u &, i32 value) {
    field_0x0c = value;
}

void cbEdLevelSetSliderFloat(eduimenu_s *, eduiitem_s *item, u32) {
    *static_cast<f32 *>(item->data_ptr) = static_cast<edui_slider_s *>(item)->value;
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

nugscn_s *LevelEditor::GetScene(i32 index) {
    if (static_cast<u32>(index) < 10) {
        return scenes[index].scene;
    }
    return NULL;
}

void cbEdLevelSetSliderInt(eduimenu_s *, eduiitem_s *item, u32) {
    *static_cast<i32 *>(item->data_ptr) = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
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

void PropertyTool::SetMenuControl(eduimenu_s *menu, EdControl *control) {
    for (PropertyMenu *property_menu = active_menu; property_menu != NULL; property_menu = property_menu->next) {
        if (property_menu->menu == menu) {
            property_menu->control = control;
            return;
        }
    }
}

bool PropertyTool::HasActiveMenu() {
    return active_menu != NULL;
}
