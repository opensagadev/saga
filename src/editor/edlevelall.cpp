#include "decomp.h"
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
