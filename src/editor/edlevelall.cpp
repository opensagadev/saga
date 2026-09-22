#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nufile/nufile.h"
#include <string.h>
#include <new>

#include <stdio.h>

extern i32 EdType_String;
extern eduiiattr_s EdLevelAttr;
void cbEdLevelDestroy(eduimenu_s *, eduimenu_s *);
void cbEdLevelSave(eduimenu_s *, eduiitem_s *, u32);
void cbEdLevelSetSliderFloat(eduimenu_s *, eduiitem_s *, u32);
void cbEdLevelDestroyOnSelect(eduimenu_s *, eduiitem_s *, u32);
static void cbEdLevelEditorList(eduimenu_s *, eduiitem_s *, u32);
static void cbEdLevelEditorSelect(eduimenu_s *, eduiitem_s *, u32);
static void cbEdLevelSettingsMenu(eduimenu_s *, eduiitem_s *, u32);

#define EDLEVEL_ADD_ACTIVE_SCENE(menu, scene_index)                                                                    \
    do {                                                                                                               \
        LevelEditorScene *scene = theLevelEditor.GetEdScene(scene_index);                                              \
        if (scene != NULL && scene->active)                                                                            \
            eduiMenuAddItem(menu, eduiItemToggleCreate((scene_index) << 6, &EdLevelAttr, scene->editable, 1,           \
                                                       ClassEditor::cbEdFilterLED, scene->name));                      \
    } while (0)

static inline i32 get_class_object_attribute(EdClass *ed_class, void *object, EdRef *reference, i32 attribute, i32 type,
                                             void *data, i32 size) {
    if (reference != NULL && reference->GetAttributeData(object, attribute, type, data, size)) {
        return 1;
    }
    EdMember member;
    return ed_class->FindMember(&member, object, attribute, 1) &&
           member.reference->GetAttributeData(member.object, attribute, type, data, size);
}

extern EdRegistry theRegistry;
extern ClassEditor theClassEditor;
extern eduimenu_s *edLevelNextMenu;
void eduiSetPinnedMenu(eduimenu_s *);

extern i32 EdType_String;

DECOMP_ASSERT(offsetof(EdClass, flags) == 0x04, "EdClass flags offset");
DECOMP_ASSERT(offsetof(EdClass, members) == 0x08, "EdClass members offset");
DECOMP_ASSERT(offsetof(EdClass, last_member) == 0x0c, "EdClass last member offset");
DECOMP_ASSERT(offsetof(EdClass, member_count) == 0x10, "EdClass member count offset");
DECOMP_ASSERT(offsetof(EdClass, interface) == 0x14, "EdClass interface offset");
DECOMP_ASSERT(sizeof(EdMember) == 0x08, "EdMember size");
DECOMP_ASSERT(offsetof(EdMember, reference) == 0x04, "EdMember reference offset");

extern EdRegistry theRegistry;
extern ClassEditor theClassEditor;
extern LevelEditor theLevelEditor;
ClassEditor theClassEditor;
extern EdManipulator theDefaultManipulator;
extern MemoryManager theMemoryManager;
extern eduimenu_s *edLevelNextMenu;
void eduiSetPinnedMenu(eduimenu_s *);

i32 BaseEditor::blockDepth;
i32 BaseEditor::blockStart[8];
eduimenu_s *edLevelActiveMenu;
extern eduimenu_s *edLevelPinnedMenu;
i32 edLevelDestroyActiveMenu;
LevelEditor theLevelEditor;

extern "C" void eduiSetCameraEnabled(i32);
eduimenu_s *GetMenuActiveChild(eduimenu_s *);
extern "C" void NuFntSet(i32);
extern "C" void NuFntScale(i32, i32);

PropertyTool thePropertyTool;
PropertyMenuMetrics menu_startmetrics = {20, 5, 200, 400};
eduiiattr_s EdLevelAttr = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
i32 EdLevelFnt;
i32 EdLevelFntScale = 24;

void BaseEditor::ReadBuffer(void **destination, void *source, i32 size) {
    if (field_0x0c != 0) {
        memcpy(*destination, source, size);
    } else {
        *destination = source;
    }
}

void BaseEditor::WriteBeginBlock(i32 file, i32 type) {
    blockStart[blockDepth++] = NuFilePos(file);
    NuFileWriteInt(file, type);
    NuFileWriteInt(file, 0);
    NuFileWriteInt(file, 0);
    NuFileWriteInt(file, 0);
}

void BaseEditor::WriteEndBlock(i32 file) {
    --blockDepth;
    NuFileAlign(file, 15);
    i32 end = NuFilePos(file);
    NuFileSeek(file, blockStart[blockDepth] + 4, NUFILE_SEEK_START);
    NuFileWriteInt(file, end - blockStart[blockDepth]);
    NuFileSeek(file, end, NUFILE_SEEK_START);
}

void BaseEditor::WriteMetaData(i32 file, i32 type, i32 version, i32 count) {
    NuFileWriteInt(file, 1);
    NuFileWriteInt(file, type);
    NuFileWriteInt(file, version);
    NuFileWriteInt(file, count);
}

void CursorTool::Initialise(variptr_u &, variptr_u &, i32) {
}

void CursorTool::Process(EdInputContext &) {
    STUBBED();
}

void CursorTool::Render() {
    STUBBED();
}

ClassEditor::ClassEditor() {
    next = NULL;
    previous = NULL;
    first_tool = NULL;
    last_tool = NULL;
    tool_count = 0;
    selected_objects.first = NULL;
    selected_objects.last = NULL;
    selected_objects.count = 0;
    current_object.ed_class = NULL;
    current_object.object = NULL;
    current_object.reference = NULL;
    pending_object.ed_class = NULL;
    pending_object.object = NULL;
    pending_object.reference = NULL;
    snap_distance = 10.0f;
    manipulator = &theDefaultManipulator;
    class_filter = -1;
}

void ClassEditor::AddMenuItems(eduimenu_s *menu) {
    eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdClassFileMenu, const_cast<char *>("File")));
    eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdClassToolsMenu, const_cast<char *>("Tools")));
    eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdClassSnapMenu, const_cast<char *>("Snap")));
    eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdClassModeMenu, const_cast<char *>("Mode")));
    eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdClassViewMenu, const_cast<char *>("Show/Hide")));
    eduiMenuAddItem(menu, eduiItemSeparatorCreate(0, &EdLevelAttr));
    eduiMenuAddItem(menu,
                    eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdClassSelectClassMenu, const_cast<char *>("Select")));
    eduiMenuAddItem(menu, eduiItemSeparatorCreate(0, &EdLevelAttr));

    if (selected_objects.count == 0) {
        eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdClassNewMenu, const_cast<char *>("New")));
        return;
    }

    eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdClassNewMenu, const_cast<char *>("New/Copy")));
    eduiMenuAddItem(menu,
                    eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdClassDeleteObject, const_cast<char *>("Delete")));

    EdClass *classes[32];
    i32 class_count = 0;
    for (ClassObjectListEntry *entry = selected_objects.first; entry != NULL; entry = entry->next) {
        i32 index = 0;
        while (index < class_count && classes[index] != entry->ed_class)
            ++index;
        if (index == class_count)
            classes[class_count++] = entry->ed_class;
    }
    for (i32 index = 0; index < class_count; ++index) {
        EdClassInterface *interface = classes[index]->interface;
        interface->vtable->add_menu_items(interface, menu);
    }
}

void ClassEditor::ClearLevel(i32 level) {
    for (i32 i = 0; i < theRegistry.class_count; ++i) {
        EdClassInterface *interface = theRegistry.classes[i].interface;
        if (interface != NULL) {
            interface->vtable->clear_level(interface, level);
        }
    }
}

void *ClassEditor::CreateObject() {
    return NULL;
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
    for (i32 i = 0; i < theRegistry.class_count; ++i) {
        EdClassInterface *interface = theRegistry.classes[i].interface;
        if (interface != NULL) {
            interface->vtable->enter_editor(interface);
        }
    }
}

void ClassEditor::Exit() {
    for (i32 i = 0; i < theRegistry.class_count; ++i) {
        EdClassInterface *interface = theRegistry.classes[i].interface;
        if (interface != NULL) {
            interface->vtable->exit_editor(interface);
        }
    }
}

void ClassEditor::Flush() {
    for (i32 i = 0; i < theRegistry.class_count; ++i) {
        EdClassInterface *interface = theRegistry.classes[i].interface;
        if (interface != NULL) {
            interface->vtable->flush(interface);
        }
    }
}

void ClassEditor::Initialise(variptr_u &, variptr_u &, i32) {
    STUBBED();
}

void ClassEditor::Process(EdInputContext &) {
    STUBBED();
}

i32 ClassEditor::ReadBlock(DATAPTR *) {
    return 0;
}

void ClassEditor::Render() {
    STUBBED();
}

void ClassEditor::Serialise(EdStream &) {
    STUBBED();
}

void ClassEditor::WriteBlock(i32) {
}

void ClassEditor::DestroySelectedObjects() {
    STUBBED();
}

void ClassEditor::DestroySelectedObjectsNow() {
    while (selected_objects.first != NULL) {
        ClassObjectListEntry *entry = selected_objects.first;
        if (current_object.object == entry->object && current_object.reference == entry->reference) {
            current_object.ed_class = NULL;
            current_object.object = NULL;
        }
        theRegistry.DestroyObject(entry->ed_class->interface, entry->object, 0, 0);
        if (entry->next != NULL) {
            entry->next->previous = entry->previous;
        } else {
            selected_objects.last = entry->previous;
        }
        if (entry->previous != NULL) {
            entry->previous->next = entry->next;
        } else {
            selected_objects.first = entry->next;
        }
        entry->next = NULL;
        entry->previous = NULL;
        --selected_objects.count;
        theMemoryManager.FreePool(entry, sizeof(*entry));
    }
}

void ClassEditor::DrawObjectSphere(ClassObject &, i32) {
    STUBBED();
}

i32 ClassEditor::Editable(void *object, EdClass *object_class, i32 index) {
    i16 scene = 0;
    if (object == NULL) {
        return (class_filter >> index) & 1;
    }
    EdMember member;
    if (object_class->FindMember(&member, object, 256, 1)) {
        member.reference->GetAttributeData(member.object, 256, EdType_Short, &scene, 0);
    }
    return theLevelEditor.IsEditable(scene) != 0;
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

i32 ClassEditor::IsSelectedClass(EdClass *object_class) {
    return selected_objects.IsInList(object_class);
}

i32 ClassEditor::IsSelectedObject(ClassObject &object) {
    return selected_objects.IsInList(object);
}

i32 ClassEditor::IsSelectedObject(void *object, EdRef *reference) {
    return selected_objects.IsInList(object, reference);
}

i32 ClassEditor::IsUniqueName(char *name) {
    for (i32 i = 0; i < theRegistry.class_count; ++i) {
        EdClass *ed_class = &theRegistry.classes[i];
        EdClassInterface *interface = ed_class->interface;
        if (interface == NULL || !(ed_class->flags & 2)) {
            continue;
        }

        void *object = interface->vtable->get_next_object(interface, NULL);
        while (object != NULL) {
            EdMember member;
            char candidate_name[128];
            if (ed_class->FindMember(&member, object, 2, 1)) {
                member.reference->GetAttributeData(member.object, 2, EdType_String, candidate_name, 128);
                if (NuStrICmp(name, candidate_name) == 0) {
                    return 0;
                }
            }
            interface = ed_class->interface;
            object = interface->vtable->get_next_object(interface, object);
        }
    }
    return 1;
}

void ClassEditor::MakeUniqueName(char const *name, char *destination, i32 size) {
    i32 prefix_length = NuStrLen(name);
    const char *digit = name + prefix_length - 1;
    i32 digit_count = 0;
    while (*digit >= '0' && *digit <= '9') {
        --digit;
        ++digit_count;
    }

    i32 suffix;
    if (digit_count != 0) {
        prefix_length -= digit_count;
        suffix = NuAToI(const_cast<char *>(name + prefix_length));
    } else {
        suffix = 0;
        digit_count = 2;
    }

    if (prefix_length + digit_count >= size) {
        prefix_length = size - digit_count - 1;
    }
    NuStrNCpy(destination, name, prefix_length + 1);
    char format[16];
    sprintf(format, "%%.%dd", digit_count);
    do {
        ++suffix;
        sprintf(destination + prefix_length, format, suffix);
    } while (!IsUniqueName(destination));
}

void ClassEditor::PostLoadInitialisation(MemoryBuffer *first, MemoryBuffer *second) {
    for (i32 i = 0; i < theRegistry.class_count; ++i) {
        EdClassInterface *interface = theRegistry.classes[i].interface;
        if (interface != NULL) {
            interface->vtable->post_load_initialisation(interface, first, second);
        }
    }
}

void ClassEditor::PostSaveInitialisation() {
    for (i32 i = 0; i < theRegistry.class_count; ++i) {
        EdClassInterface *interface = theRegistry.classes[i].interface;
        if (interface != NULL) {
            interface->vtable->post_save_initialisation(interface);
        }
    }
}

void ClassEditor::PreLoadInitialisation(MemoryBuffer *first, MemoryBuffer *second) {
    for (i32 i = 0; i < theRegistry.class_count; ++i) {
        EdClassInterface *interface = theRegistry.classes[i].interface;
        if (interface != NULL) {
            interface->vtable->pre_load_initialisation(interface, first, second);
        }
    }
}

void ClassEditor::PreSaveInitialisation() {
    for (i32 i = 0; i < theRegistry.class_count; ++i) {
        EdClassInterface *interface = theRegistry.classes[i].interface;
        if (interface != NULL) {
            interface->vtable->pre_save_initialisation(interface);
        }
    }
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

void ClassEditor::SetViewMenuHilight(eduimenu_s *menu) {
    for (eduiitem_s *item = menu->first; item != NULL; item = item->next) {
        i32 index = item->data - 3;
        if (static_cast<u32>(index) <= 60) {
            item->highlighted = (theClassEditor.class_filter >> index) & 1;
        }
    }
}

void ClassEditor::SnapPoint(VuVec &) {
    STUBBED();
}

void ClassEditor::UpdateClassFilter(EdInputContext &input) {
    i32 count = theRegistry.class_count;
    if (count > 10) {
        count = 10;
    }
    for (i32 i = 0; i < count; ++i) {
        if (input.GetPress(i + 26) != 0.0f) {
            if (input.GetHold(22) != 0.0f) {
                class_filter = 1 << i;
            } else {
                class_filter ^= 1 << i;
            }
        }
    }
    if (input.GetPress(36) != 0.0f) {
        if (input.GetHold(22) != 0.0f) {
            class_filter = -1;
        } else {
            class_filter = ~class_filter;
        }
    }
}

void ClassEditor::UpdateLists(MemoryBuffer *first, MemoryBuffer *second) {
    for (i32 i = 0; i < theRegistry.class_count; ++i) {
        EdClassInterface *interface = theRegistry.classes[i].interface;
        if (interface != NULL) {
            interface->vtable->update_lists(interface, first, second);
        }
    }
}

void ClassEditor::UpdateSelectedObjects(EdInputContext &) {
    STUBBED();
}

void ClassEditor::ViewSelected() {
    STUBBED();
}

void ClassEditor::cbDestroyMenu(eduimenu_s *menu, eduimenu_s *) {
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
    if (theClassEditor.menu == menu) {
        theClassEditor.menu = NULL;
    }
}

void ClassEditor::cbDestroyObject(eduimenu_s *, eduiitem_s *item, u32) {
    if (item->data != 0) {
        theClassEditor.DestroySelectedObjectsNow();
    }
    theLevelEditor.CloseMenu();
}

void ClassEditor::cbEdClassDeleteObject(eduimenu_s *, eduiitem_s *, u32) {
    theClassEditor.DestroySelectedObjects();
}

void ClassEditor::cbEdClassExportMenu(eduimenu_s *, eduiitem_s *, u32) {
}

void ClassEditor::cbEdClassFileMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    eduimenu_s *menu = eduiMenuCreate(parent->x + item->x, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (menu == NULL)
        return;
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 0);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 1);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 2);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 3);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 4);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 5);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 6);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 7);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 8);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 9);
    eduiMenuAddItem(menu, eduiItemSeparatorCreate(0, &EdLevelAttr));
    eduiMenuAddItem(menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdLevelSave, const_cast<char *>("Save")));
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
}

void ClassEditor::cbEdClassImportMenu(eduimenu_s *, eduiitem_s *, u32) {
}

void ClassEditor::cbEdClassModeMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    eduimenu_s *menu = eduiMenuCreate(parent->x + item->x, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (menu == NULL)
        return;
    eduiMenuAddItem(menu, eduiItemToggleCreate(0, &EdLevelAttr, theClassEditor.mode == 0, 1, cbEdClassSetMode,
                                               const_cast<char *>("Select")));
    eduiMenuAddItem(menu, eduiItemToggleCreate(1, &EdLevelAttr, theClassEditor.mode == 1, 1, cbEdClassSetMode,
                                               const_cast<char *>("Create")));
    eduiMenuAddItem(menu, eduiItemToggleCreate(2, &EdLevelAttr, theClassEditor.mode == 2, 1, cbEdClassSetMode,
                                               const_cast<char *>("Delete")));
    eduiMenuAddItem(menu, eduiItemToggleCreate(3, &EdLevelAttr, theClassEditor.mode == 3, 1, cbEdClassSetMode,
                                               const_cast<char *>("Move")));
    eduiMenuAddItem(menu, eduiItemToggleCreate(4, &EdLevelAttr, theClassEditor.mode == 4, 1, cbEdClassSetMode,
                                               const_cast<char *>("Rotate")));
    eduiMenuAddItem(menu, eduiItemToggleCreate(5, &EdLevelAttr, theClassEditor.mode == 5, 1, cbEdClassSetMode,
                                               const_cast<char *>("Scale")));
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
}

void ClassEditor::cbEdClassNewMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    eduimenu_s *menu = eduiMenuCreate(parent->x + item->x, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (menu == NULL)
        return;
    for (i32 index = 0; index < theRegistry.class_count; ++index) {
        EdClass *ed_class = &theRegistry.classes[index];
        if ((ed_class->flags & 0x20000000) == 0)
            eduiMenuAddItem(menu, eduiItemSelCreate(index, &EdLevelAttr, 0, 0, cbEdClassNewObject, ed_class->name));
    }
    if (menu->first == NULL)
        eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect,
                                                const_cast<char *>("No Registered Classes")));
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
}

void ClassEditor::cbEdClassNewObject(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassRemoveDuplicates(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdClassSelectClassMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    eduimenu_s *menu = eduiMenuCreate(parent->x + item->x, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (menu == NULL)
        return;
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 0);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 1);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 2);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 3);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 4);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 5);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 6);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 7);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 8);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 9);
    eduiMenuAddItem(menu, eduiItemSeparatorCreate(0, &EdLevelAttr));
    for (i32 index = 0; index < theRegistry.class_count; ++index) {
        EdClass *ed_class = &theRegistry.classes[index];
        if ((ed_class->flags & 0x20000000) == 0 && ed_class->interface != NULL)
            eduiMenuAddItem(menu,
                            eduiItemSelCreate(index, &EdLevelAttr, 0, 0, cbEdClassSelectObjectMenu, ed_class->name));
    }
    if (menu->first == NULL)
        eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect,
                                                const_cast<char *>("No Registered Classes")));
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
    eduiMenuSortItemsByTxt(menu);
}

void ClassEditor::cbEdClassSelectObject(eduimenu_s *, eduiitem_s *item, u32) {
    theClassEditor.pending_object.object = item->data_ptr;
    if (theClassEditor.pending_object.object != NULL) {
        theClassEditor.SelectObject(theClassEditor.pending_object, 0);
        theClassEditor.FocusSelected();
        theLevelEditor.CloseMenu();
    }
}

void ClassEditor::cbEdClassSelectObjectMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    EdClass *ed_class = theRegistry.GetClass(static_cast<i32>(item->data));
    theClassEditor.pending_object.ed_class = ed_class;
    eduimenu_s *menu = eduiMenuCreate(parent->x + item->x, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (menu == NULL)
        return;
    if (ed_class != NULL && ed_class->interface != NULL) {
        EdClassInterface *interface = ed_class->interface;
        EdRef *name_ref = ed_class->FindTypeRef(2, 1);
        if (interface->vtable->get_next_object != NULL) {
            for (void *object = interface->vtable->get_next_object(interface, NULL); object != NULL;
                 object = interface->vtable->get_next_object(interface, object)) {
                char name[128];
                if (name_ref == NULL || !name_ref->GetAttributeData(object, 2, EdType_String, name, sizeof(name)))
                    NuStrCpy(name, ed_class->name);
                if (theClassEditor.Editable(object, ed_class, -1))
                    eduiMenuAddItem(menu, eduiItemSelCreate(reinterpret_cast<usize>(object), &EdLevelAttr, 0, 0,
                                                            cbEdClassSelectObject, name));
            }
        }
    }
    if (menu->first == NULL) {
        eduiMenuAddItem(
            menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect, const_cast<char *>("No Object")));
    } else {
        eduiMenuAddItemFirst(
            menu, eduiItemFilterCreate(0, &EdLevelAttr, const_cast<char *>("FILTER"), const_cast<char *>("")));
    }
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
    eduiMenuSortItemsByTxt(menu);
}

void ClassEditor::cbEdClassSetMode(eduimenu_s *menu, eduiitem_s *item, u32) {
    theClassEditor.SetMode(item->data);
    eduiMenuHighlight(menu, item);
}

void ClassEditor::cbEdClassSetPinned(eduimenu_s *menu, eduiitem_s *item, u32) {
    menu->flags ^= 4;
    item->highlighted = !item->highlighted;
    if (menu->flags & 4) {
        eduiMenuDetach(menu);
        eduiSetPinnedMenu(menu);
    } else {
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
        if (edLevelNextMenu == menu) {
            edLevelNextMenu = NULL;
        }
        eduiSetPinnedMenu(NULL);
    }
}

void ClassEditor::cbEdClassSetSnap(eduimenu_s *menu, eduiitem_s *item, u32) {
    theClassEditor.snap_mode = item->data;
    eduiMenuHighlight(menu, item);
}

void ClassEditor::cbEdClassSetView(eduimenu_s *menu, eduiitem_s *item, u32) {
    switch (item->data) {
        case 0:
            theClassEditor.class_filter = -1;
            break;
        case 1:
            theClassEditor.class_filter = 0;
            break;
        case 2:
            theClassEditor.class_filter = ~theClassEditor.class_filter;
            break;
        default:
            if (item->data - 3 >= 0) {
                theClassEditor.class_filter ^= 1 << (item->data - 3);
            }
            break;
    }
    SetViewMenuHilight(menu);
}

void ClassEditor::cbEdClassSnapMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    eduimenu_s *menu = eduiMenuCreate(parent->x + item->x, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (menu == NULL)
        return;
    eduiMenuAddItem(menu, eduiItemToggleCreate(1, &EdLevelAttr, theClassEditor.snap_mode == 1, 1, cbEdClassSetSnap,
                                               const_cast<char *>("Height")));
    eduiMenuAddItem(menu, eduiItemToggleCreate(2, &EdLevelAttr, theClassEditor.snap_mode == 2, 1, cbEdClassSetSnap,
                                               const_cast<char *>("Ray")));
    eduiMenuAddItem(menu, eduiItemToggleCreate(0, &EdLevelAttr, theClassEditor.snap_mode == 0, 1, cbEdClassSetSnap,
                                               const_cast<char *>("Off")));
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
}

void ClassEditor::cbEdClassToolsMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    eduimenu_s *menu = eduiMenuCreate(parent->x + item->x, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (menu == NULL)
        return;
    eduiMenuAddItem(menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdClassRemoveDuplicates,
                                            const_cast<char *>("Remove Duplicates")));
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
}

void ClassEditor::cbEdClassViewMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    eduimenu_s *menu = eduiMenuCreate(parent->x + item->x, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (menu == NULL)
        return;
    eduiMenuAddItem(menu, eduiItemCheckCreate(0, &EdLevelAttr, 0, 0, cbEdClassSetPinned, const_cast<char *>("Pinned")));
    menu->flags &= ~4;
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 0);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 1);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 2);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 3);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 4);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 5);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 6);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 7);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 8);
    EDLEVEL_ADD_ACTIVE_SCENE(menu, 9);
    eduiMenuAddItem(menu, eduiItemSeparatorCreate(0, &EdLevelAttr));
    eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdClassSetView, const_cast<char *>("All")));
    eduiMenuAddItem(menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdClassSetView, const_cast<char *>("None")));
    eduiMenuAddItem(menu, eduiItemSelCreate(2, &EdLevelAttr, 0, 0, cbEdClassSetView, const_cast<char *>("Invert")));
    if (theRegistry.class_count < 1) {
        eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect,
                                                const_cast<char *>("No Registered Classes")));
    } else {
        const i32 shortcuts[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
        char label[128];
        for (i32 index = 0; index < theRegistry.class_count; ++index) {
            EdClass *ed_class = &theRegistry.classes[index];
            if ((ed_class->flags & 0x20000000) != 0)
                continue;
            if (index < 10)
                sprintf(label, "%d %s", shortcuts[index], ed_class->name);
            else
                sprintf(label, "   %s", ed_class->name);
            eduiMenuAddItem(menu, eduiItemCheckCreate(index + 3, &EdLevelAttr,
                                                      (theClassEditor.class_filter >> (index & 31)) & 1, 0,
                                                      cbEdClassSetView, label));
        }
    }
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
}

void ClassEditor::cbEdCopySelectedObject(EdInputContext &) {
    STUBBED();
}

void ClassEditor::cbEdCreateClassNewObject(i32) {
    STUBBED();
}

void ClassEditor::cbEdFilterLED(eduimenu_s *, eduiitem_s *item, u32) {
    if (item == NULL)
        return;
    LevelEditorScene *scene = theLevelEditor.GetEdScene(static_cast<u32>(item->data) >> 6);
    if (scene == NULL)
        return;
    theLevelEditor.current_led_file = 0xffff;
    i32 editable = scene->editable + 1;
    scene->editable = editable;
    item->highlighted = editable;
}

void ClassEditor::cbEdLevelDeselectAll(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdLevelSelectAll(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassEditor::cbEdPadSetManipulatorMode(eduimenu_s *, eduiitem_s *, u32) {
    if (theClassEditor.mode == 4) {
        theClassEditor.mode = 5;
    } else if (theClassEditor.mode == 5) {
        theClassEditor.mode = 3;
    } else if (theClassEditor.mode == 3) {
        theClassEditor.mode = 4;
    }
    theClassEditor.SetMode(theClassEditor.mode);
}

void ClassEditor::cbFileSelected(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void ClassObject::GetName(char *destination, i32 size) {
    if (object == NULL) {
        NuStrNCpy(destination, "None", size);
        return;
    }

    char name[128];
    if (!get_class_object_attribute(ed_class, object, reference, 2, EdType_String, name, 128)) {
        NuStrCpy(name, "NoName");
    }
    sprintf(destination, "%s.%s", ed_class->name, name);
}

void ClassObject::Set(char *name) {
    ed_class = NULL;
    object = NULL;
    char *separator = NuStrChr(name, '.');
    if (separator == NULL) {
        return;
    }

    char class_name[128];
    char object_name[128];
    char candidate_name[128];
    EdMember member;
    *separator = '\0';
    NuStrCpy(class_name, name);
    *separator = '.';
    NuStrCpy(object_name, separator + 1);

    ed_class = theRegistry.GetClass(class_name);
    if (ed_class == NULL || !(ed_class->flags & 2) || ed_class->interface == NULL) {
        return;
    }

    EdClassInterface *interface = ed_class->interface;
    object = interface->vtable->get_next_object(interface, NULL);
    while (object != NULL) {
        if (ed_class->FindMember(&member, object, 2, 1)) {
            member.reference->GetAttributeData(member.object, 2, EdType_String, candidate_name, 128);
            if (NuStrICmp(object_name, candidate_name) == 0) {
                break;
            }
        }
        interface = ed_class->interface;
        object = interface->vtable->get_next_object(interface, object);
    }
}

void LevelEditor::AddInfoText(char *text) {
    for (i32 i = 0; i < 32; ++i) {
        if (info_text[i] == NULL) {
            info_text[i] = AddText(text);
            return;
        }
    }
}

i32 LevelEditor::AddScene(char *name, nugscn_s *scene, i32 active) {
    for (i32 i = 0; i < reset_pending; ++i) {
        if (NuStrICmp(scenes[i].name, name) == 0) {
            scenes[i].scene = scene;
            scenes[i].active = active;
            return i;
        }
    }
    if (reset_pending < 10) {
        i32 index = reset_pending++;
        NuStrNCpy(scenes[index].name, name, 32);
        scenes[index].scene = scene;
        scenes[index].active = active;
        return index;
    }
    return 0;
}

char *LevelEditor::AddText(char *text) {
    i32 size = NuStrLen(text) + 1;
    char *result = text_buffer + text_length;
    NuStrCpy(result, text);
    text_length += size;
    return result;
}

void LevelEditor::BeginMultiLoad(variptr_u *, variptr_u *) {
    STUBBED();
}

void LevelEditor::ClearLevel(i32 index) {
    scenes[index].active = 0;
    scenes[index].scene = NULL;
    for (BaseEditor *editor = first_editor; editor != NULL; editor = editor->next) {
        editor->ClearLevel(index);
    }
}

void LevelEditor::CloseMenu() {
    eduiSetCameraEnabled(1);
    if (edLevelActiveMenu != NULL) {
        if (edLevelActiveMenu->parent != NULL) {
            edLevelActiveMenu->parent->child = NULL;
        }
        if (edLevelActiveMenu->child != NULL) {
            edLevelActiveMenu->child->parent = NULL;
        }
    }
    edLevelDestroyActiveMenu = 1;
}

void LevelEditor::CreateEditorList(eduimenu_s *parent, eduiitem_s *item) {
    eduimenu_s *menu =
        eduiMenuCreate(parent->x + item->x, item->y, 180, 250, reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)),
                       cbEdLevelDestroy, const_cast<char *>("Editor List"));
    if (menu == NULL)
        return;
    for (BaseEditor *editor = first_editor; editor != NULL; editor = editor->next) {
        eduiMenuAddItem(menu, eduiItemSelCreate(reinterpret_cast<usize>(editor), &EdLevelAttr, 0, 0,
                                                cbEdLevelEditorSelect, editor->GetName()));
    }
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
}

static void cbEdLevelEditorList(eduimenu_s *menu, eduiitem_s *item, u32) {
    theLevelEditor.CreateEditorList(menu, item);
}

static void cbEdLevelEditorSelect(eduimenu_s *, eduiitem_s *item, u32) {
    theLevelEditor.active_editor = static_cast<BaseEditor *>(item->data_ptr);
    edLevelDestroyActiveMenu = 1;
}

static void cbEdLevelSettingsMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    eduimenu_s *menu =
        eduiMenuCreate(parent->x + item->x, item->y, 180, 250, reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)),
                       cbEdLevelDestroy, const_cast<char *>("Options"));
    if (menu == NULL)
        return;
    eduiMenuAddItem(menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdLevelSave, const_cast<char *>("Save Data")));
    theLevelEditor.settings.AddMenuItems(menu);
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
}

void LevelEditor::CreateMenu() {
    f32 x, y;
    eduiGetCursorCoords(&x, &y);
    const i32 menu_x = static_cast<i32>(x * 640.0f);
    const i32 menu_y = static_cast<i32>(y * 448.0f);
    const bool keyboard_menu = NuKeyboard(0x0f) != 0;
    eduimenu_s *menu =
        eduiMenuCreate(menu_x, menu_y, 100, 200, reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)),
                       cbEdLevelDestroy, keyboard_menu ? const_cast<char *>("Level Editor") : NULL);
    if (menu != NULL) {
        if (keyboard_menu) {
            eduiMenuAddItem(
                menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdLevelEditorList, const_cast<char *>("Editors...")));
            eduiMenuAddItem(menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdLevelSettingsMenu,
                                                    const_cast<char *>("Options...")));
        } else if (active_editor != NULL) {
            active_editor->AddMenuItems(menu);
        }
        eduiMenuFitWidth(menu, 5);
        eduiMenuFitOnScreen(menu, 1);
    }
    edLevelActiveMenu = menu;
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
    edLevelActiveMenu = NULL;
    eduiSetUsingMenuFocus(1);
    editors_entered = 1;
    active = 1;
    for (BaseEditor *editor = first_editor; editor != NULL; editor = editor->next) {
        editor->Enter();
    }
}

void LevelEditor::Exit() {
    active = 0;
    eduiSetUsingMenuFocus(0);
    if (editors_entered != 0) {
        for (BaseEditor *editor = first_editor; editor != NULL; editor = editor->next) {
            editor->Exit();
        }
        editors_entered = 0;
    }
}

i32 LevelEditor::FindSceneId(char *name) {
    for (i32 i = 0; i < 10; ++i) {
        if (NuStrICmp(scenes[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void LevelEditor::Flush() {
    for (BaseEditor *editor = first_editor; editor != NULL; editor = editor->next) {
        editor->Flush();
    }
    reset_pending = 0;
    memset(scenes, 0, sizeof(scenes));
}

LevelEditorScene *LevelEditor::GetEdScene(i32 index) {
    if (static_cast<u32>(index) >= 10) {
        return NULL;
    }
    return &scenes[index];
}

nugscn_s *LevelEditor::GetScene(char *name) {
    for (i32 i = 0; i < reset_pending; ++i) {
        if (NuStrICmp(name, scenes[i].name) == 0) {
            return scenes[i].scene;
        }
    }
    return NULL;
}

void LevelEditor::Initalise(variptr_u &, variptr_u &, i32) {
    STUBBED();
}

i32 LevelEditor::IsActiveScene(nugscn_s *scene) {
    for (i32 i = 0; i < reset_pending; ++i) {
        if (scenes[i].scene == scene) {
            return scenes[i].active;
        }
    }
    return 0;
}

i32 LevelEditor::IsEditable(i32 index) {
    if (index == -1) {
        return 1;
    }
    if (static_cast<u32>(index) >= 10) {
        return 0;
    }
    return scenes[index].editable != 0 || scenes[index].active == 0;
}

LevelEditor::LevelEditor() {
    first_editor = NULL;
    last_editor = NULL;
    editor_count = 0;
    background_colour[3] = 1.0f;
    overlay_alpha = 0.6f;
    active = 0;
    background_colour[0] = 0.0f;
    reset_pending = 0;
    background_colour[1] = 0.0f;
    field_0x20 = 0;
    background_colour[2] = 0.0f;
    field_0x24 = 0;
    field_0x28 = 1;
    field_0x30 = 1;
    field_0x38 = 1;
    info_x = 0;
    info_y = 0;
    info_width = 640;
    info_height = 448;
    info_colour = 0x80808080;
    info_background = 0x40000000;
    pad_x = 0;
    pad_y = 0;
    pad_width = 640;
    pad_height = 448;
    pad_background = 0x80000000;
    pad_colour = 0x80808080;
    SetPadText(0x1000, const_cast<char *>("D-Up"));
    SetPadText(0x4000, const_cast<char *>("D-Down"));
    SetPadText(0x8000, const_cast<char *>("D-Left"));
    SetPadText(0x2000, const_cast<char *>("D-Right"));
    SetPadText(0x10, const_cast<char *>("Triangle"));
    SetPadText(0x40, const_cast<char *>("Cross"));
    SetPadText(0x80, const_cast<char *>("Square"));
    SetPadText(0x20, const_cast<char *>("Circle"));
    SetPadText(4, const_cast<char *>("L1"));
    SetPadText(1, const_cast<char *>("L2"));
    SetPadText(8, const_cast<char *>("R1"));
    SetPadText(2, const_cast<char *>("R2"));
    SetPadText(0x200, const_cast<char *>("L3"));
    SetPadText(0x400, const_cast<char *>("R1"));
    SetPadText(0x800, const_cast<char *>("START"));
    SetPadText(0x100, const_cast<char *>("SELECT"));
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

void LevelEditor::SetPadText(i32 buttons, char *text) {
    for (i32 i = 0; i < 32; ++i) {
        if ((static_cast<u32>(buttons) & (1u << i)) != 0) {
            pad_text[i] = text;
        }
    }
}

void LevelEditor::SetSaveFilename(char *name) {
    if (name != NULL) {
        NuStrCpy(save_filename, name);
        NuStrCpy(editor_filename, save_filename);
        char *extension = NuStrRChr(editor_filename, '.');
        if (extension != NULL) {
            *extension = '\0';
        }
        NuStrCat(editor_filename, ".led");
    } else {
        save_filename[0] = '\0';
        editor_filename[0] = '\0';
    }
}

void LevelEditor::WriteStream(EdFileOutputStream &) {
    STUBBED();
}

void PropertyMenu::AddObject(ClassObject &object) {
    if (object_count < 8) {
        objects[object_count++] = object;
    }
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
    eduicbMenuCloseAllexpanders(menu);
    if (menu != NULL) {
        for (eduiitem_s *item = menu->first; item != NULL; item = item->next) {
            delete static_cast<EdControl *>(item->data_ptr);
        }
    }
    eduiMenuDestroy(menu);
}

void PropertyMenu::SelectAttr(i32 selected) {
    for (eduiitem_s *item = menu->first; item != NULL; item = item->next) {
        EdControl *control = static_cast<EdControl *>(item->data_ptr);
        if (control != NULL) {
            control->SetMenuItemAttr(selected, item, &thePropertyTool.selected_attr, &thePropertyTool.unselected_attr);
        }
    }
}

void PropertyTool::AddPropertyMenuItems(eduimenu_s *, EdClass *, void *, eduiitem_s *) {
    STUBBED();
}

void PropertyTool::AutoLocateMenu(PropertyMenu *property_menu) {
    eduimenu_s *menu = property_menu->menu;
    menu->x = 70;
    menu->y = 30;
    bool use_stored_metrics = true;
    PropertyMenu *other = active_menu;
    while (other != NULL) {
        if (other->menu != menu) {
            if (menu->x + menu->width >= other->menu->x && other->menu->x + other->menu->width >= menu->x) {
                menu->x += 10;
                if (static_cast<float>(menu->x + menu->width) > 590.0f) {
                    menu->x = 20;
                    menu->y = 5;
                    return;
                }
                use_stored_metrics = false;
                other = active_menu;
                continue;
            }
            use_stored_metrics = false;
        }
        other = other->next;
    }
    if (use_stored_metrics) {
        ediMenuRetrieveMetrics(menu);
    }
    if (static_cast<float>(menu->x + menu->width) > 590.0f) {
        menu->x = 20;
        menu->y = 5;
    }
}

void PropertyTool::BringToFront(PropertyMenu *menu) {
    if (menu->next != NULL) {
        menu->next->previous = menu->previous;
    } else {
        last_menu = menu->previous;
    }
    if (menu->previous != NULL) {
        menu->previous->next = menu->next;
    } else {
        active_menu = menu->next;
    }
    menu->next = NULL;
    menu->previous = NULL;
    --menu_count;
    menu->order = -2;
    PropertyMenu *position = active_menu;
    while (position != NULL && position->order < -1) {
        position = position->next;
    }
    if (position != NULL) {
        menu->next = position;
        menu->previous = position->previous;
        if (position->previous != NULL) {
            position->previous->next = menu;
        } else {
            active_menu = menu;
        }
        position->previous = menu;
    } else {
        menu->previous = last_menu;
        if (last_menu != NULL) {
            last_menu->next = menu;
        }
        last_menu = menu;
        if (active_menu == NULL) {
            active_menu = menu;
        }
    }
    ++menu_count;
}

void PropertyTool::CreatePropertyMenu(ClassObject &) {
    STUBBED();
}

PropertyMenu *PropertyTool::FindItemMenu(PropertyMenu *menu, ClassItem *item) {
    for (; menu != NULL; menu = menu->next) {
        if (menu->ContainsObject(item->object)) {
            return menu;
        }
    }
    return NULL;
}

PropertyMenu *PropertyTool::GetActiveMenu(PropertyMenu *menu) {
    for (; menu != NULL; menu = menu->next) {
        if (menu->menu == eduiGetActiveMenuParent()) {
            return menu;
        }
    }
    return NULL;
}

void PropertyTool::GetClassName(EdRef *, char *) {
    STUBBED();
}

PropertyMenu *PropertyTool::GetNextActiveMenu() {
    PropertyMenu *menu = GetActiveMenu(active_menu);
    return menu != NULL ? menu->next : NULL;
}

eduimenu_s *PropertyTool::GetNextDefaultActiveMenu(eduimenu_s *menu) {
    if (menu == NULL) {
        return edLevelActiveMenu;
    }
    return menu == edLevelActiveMenu ? edLevelPinnedMenu : NULL;
}

void PropertyTool::GetTypeName(EdRef *, char *) {
    STUBBED();
}

void PropertyTool::Initialise(variptr_u &, variptr_u &, i32) {
}

i32 PropertyTool::Process(EdInputContext &input) {
    i32 result = ProcessMenu(input);
    ProcessControls(input);
    return result;
}

i32 PropertyTool::ProcessControls(EdInputContext &input) {
    for (PropertyMenu *menu = active_menu; menu != NULL; menu = menu->next) {
        if (menu->control != NULL) {
            menu->control->Process(input);
        }
    }
    return 0;
}

i32 PropertyTool::ProcessMenu(EdInputContext &) {
    STUBBED();
    return 0;
}

PropertyTool::PropertyTool() {
    next = NULL;
    previous = NULL;
    active_menu = NULL;
    last_menu = NULL;
    menu_count = 0;
    show_type_names = 1;
    menu_attr.background = 0x80000000;
    menu_attr.text = 0x80ff0000;
    menu_attr.highlight = 0x80804040;
    menu_attr.disabled = 0x80404040;
    selected_attr.background = 0x80000000;
    selected_attr.text = 0x80ff0000;
    selected_attr.highlight = 0x80406080;
    selected_attr.disabled = 0x80404040;
    unselected_attr.background = 0x80000000;
    unselected_attr.text = 0x80ff0000;
    unselected_attr.highlight = 0x80707080;
    unselected_attr.disabled = 0x80808080;
}

void PropertyTool::RefreshMenuControls(PropertyMenu *property_menu) {
    eduiitem_s *item = property_menu->menu->field_0c;
    NuFntSet(EdLevelFnt);
    NuFntScale(EdLevelFntScale, EdLevelFntScale);
    for (; item != NULL; item = item->next) {
        EdControl *control = static_cast<EdControl *>(item->data_ptr);
        if (control != NULL) {
            control->Refresh();
        }
        if (item == property_menu->menu->field_10) {
            break;
        }
    }
}

void PropertyTool::Render() {
    PropertyMenu *selected = NULL;
    for (PropertyMenu *menu = last_menu; menu != NULL; menu = menu->previous) {
        if (menu == GetActiveMenu(active_menu)) {
            selected = menu;
        } else {
            RenderMenu(menu);
        }
    }
    if (selected != NULL) {
        RenderMenu(selected);
    }
}

void PropertyTool::RenderMenu(PropertyMenu *) {
    STUBBED();
}

void PropertyTool::RetrievePropertyMenu(ClassObject *, PropertyMenuList *) {
    STUBBED();
}

void PropertyTool::SelectAttr(i32 selected) {
    for (PropertyMenu *menu = active_menu; menu != NULL; menu = menu->next) {
        menu->SelectAttr(selected);
    }
}

void PropertyTool::SetDefaultActiveMenu(PropertyMenu *menu) {
    if (menu != NULL && menu->menu != NULL) {
        eduiSetActiveMenu(GetMenuActiveChild(menu->menu));
    } else {
        eduiSetActiveMenu(edLevelActiveMenu != NULL ? edLevelActiveMenu : edLevelPinnedMenu);
    }
}

void PropertyTool::ToggleActiveMenu() {
    PropertyMenu *menu = GetActiveMenu(active_menu);
    if (menu != NULL) {
        if (menu == last_menu) {
            eduiSetActiveMenu(NULL);
        } else {
            SetDefaultActiveMenu(GetNextActiveMenu());
        }
    } else if (GetNextDefaultActiveMenu(eduiGetActiveMenu()) == NULL) {
        SetDefaultActiveMenu(active_menu);
    }
}

PropertyMenuMetrics PropertyTool::ediGetMenuStartMetrics() {
    return menu_startmetrics;
}

void PropertyTool::ediMenuRetrieveMetrics(eduimenu_s *menu) {
    menu->x = menu_startmetrics.x;
    menu->y = menu_startmetrics.y;
    menu->width = menu_startmetrics.width;
    menu->height = menu_startmetrics.height;
}

void PropertyTool::ediMenuStoreMetrics(eduimenu_s *menu) {
    menu_startmetrics.x = menu->x > 0 ? menu->x : 20;
    menu_startmetrics.y = menu->y > 0 ? menu->y : 5;
    menu_startmetrics.width = menu->width;
    menu_startmetrics.height = menu->height;
}

i32 ClassObjectList::GetAveragePosition(VuVec &average) {
    average = VuVec_Zero;
    i32 position_count = 0;
    for (ClassObjectListEntry *entry = first; entry != NULL; entry = entry->next) {
        VuVec position;
        if (get_class_object_attribute(entry->ed_class, entry->object, entry->reference, 8, EdType_VuVec, &position,
                                       0)) {
            average.x += position.x;
            average.y += position.y;
            average.z += position.z;
            ++position_count;
        }
    }
    if (position_count != 0) {
        float scale = 1.0f / position_count;
        average.x *= scale;
        average.y *= scale;
        average.z *= scale;
    }
    return position_count;
}

i32 ClassObjectList::GetAveragePosition(VuVec &average, float &radius) {
    average = VuVec_Zero;
    i32 position_count = 0;
    VuVec positions[64];
    float radii[64];
    for (ClassObjectListEntry *entry = first; entry != NULL && position_count < 64; entry = entry->next) {
        VuVec &position = positions[position_count];
        if (get_class_object_attribute(entry->ed_class, entry->object, entry->reference, 8, EdType_VuVec, &position,
                                       0)) {
            average.x += position.x;
            average.y += position.y;
            average.z += position.z;
            radii[position_count] = 1.0f;
            get_class_object_attribute(entry->ed_class, entry->object, entry->reference, 64, EdType_Float,
                                       &radii[position_count], 0);
            ++position_count;
        }
    }
    if (position_count != 0) {
        float scale = 1.0f / position_count;
        average.x *= scale;
        average.y *= scale;
        average.z *= scale;
        radius = 0.0f;
        for (i32 index = 0; index < position_count; ++index) {
            NUVEC difference;
            difference.x = average.x - positions[index].x;
            difference.y = average.y - positions[index].y;
            difference.z = average.z - positions[index].z;
            float extent = NuVecMag(&difference) + radii[index];
            radius = extent > radius ? extent : radius;
        }
    }
    return position_count;
}

i32 ClassObjectList::IsInList(void *object, EdRef *reference) {
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

void cbEdLevelDestroy(eduimenu_s *menu, eduimenu_s *) {
    if ((menu->flags & 4) != 0) {
        return;
    }
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
    eduiSetCameraEnabled(1);
    if (menu == edLevelActiveMenu) {
        edLevelActiveMenu = NULL;
    }
    if (menu == edLevelPinnedMenu) {
        edLevelPinnedMenu = NULL;
    } else if (edLevelPinnedMenu != NULL && edLevelActiveMenu == NULL) {
        edLevelActiveMenu = edLevelPinnedMenu;
    }
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

void cbEdLevelToggleInt(eduimenu_s *, eduiitem_s *item, u32) {
    i32 *value = static_cast<i32 *>(item->data_ptr);
    *value ^= 1;
    item->highlighted = *value;
}

void cbCEDeleteConfirmed(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void LightEverythingInEditor(void *) {
    STUBBED();
}

void cbEdLevelDestroyOnSelect(eduimenu_s *menu, eduiitem_s *, u32) {
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
    if (menu == edLevelActiveMenu) {
        edLevelActiveMenu = NULL;
    }
    if (menu == edLevelPinnedMenu) {
        edLevelPinnedMenu = NULL;
    }
}

void EdClass::SerialiseObject(EdStream &stream, void *object) {
    if (stream.BeginBlock("Object") == NULL) {
        return;
    }
    if (object != NULL) {
        u8 data[256];
        for (EdRef *member = members; member != NULL; member = member->next) {
            if ((stream.flags & 0x400000) != 0 ? (member->attributes & 0x400000) != 0
                                               : (member->attributes & 0x10000000) != 0) {
                continue;
            }
            if (member->attributes < 0) {
                EdClass *member_class = theRegistry.GetClass(member->type_id);
                member_class->SerialiseObject(stream, member->GetMemberObject(object));
            } else {
                EdType *type = theRegistry.GetType(member->type_id);
                i32 size = member->size > 0 ? member->size : type->size;
                if (stream.mode == 2) {
                    member->GetMemberData(object, member->type_id, data, sizeof(data));
                }
                type->serialise(stream, data, size);
                if (stream.mode == 1) {
                    member->SetMemberData(object, member->type_id, data, sizeof(data), NULL);
                }
            }
        }
        if (interface != NULL) {
            interface->vtable->serialise_object(interface, stream, object);
        }
    }
    stream.EndBlock();
}

i32 EdClass::SerialiseObjectHeader(EdStream &stream, void *object) {
    u8 present = 0;
    if (stream.mode == 2 && object != NULL) {
        present = 1;
    }
    stream.SerialiseBuffer(&present, 1, 1);
    return present;
}

void EdClass::CopyObject(void *destination, void *source) {
    for (EdRef *member = members; member != NULL; member = member->next) {
        if ((member->attributes & 0x1800000) != 0) {
            continue;
        }
        if (member->attributes < 0) {
            EdClass *member_class = theRegistry.GetClass(member->type_id);
            void *source_member = member->GetMemberObject(source);
            void *destination_member = member->GetMemberObject(destination);
            member_class->CopyObject(destination_member, source_member);
        } else {
            theRegistry.GetType(member->type_id);
            u8 data[256];
            member->GetMemberData(source, member->type_id, data, sizeof(data));
            member->SetMemberData(destination, member->type_id, data, sizeof(data), NULL);
        }
    }
}

void EdClass::AddType(EdRef *member) {
    member->next = NULL;
    member->previous = last_member;
    if (last_member != NULL) {
        last_member->next = member;
    }
    EdRef *first = members;
    last_member = member;
    if (first == NULL) {
        members = member;
    }
    ++member_count;
    flags |= member->attributes & 0x17a;
    if (member->attributes < 0) {
        flags |= theRegistry.GetClass(member->type_id)->flags & 0x17a;
    }
}

void EdClass::Serialise(EdStream &stream, i32 *class_mapping) {
    if (stream.BeginBlock("Class") == NULL) {
        return;
    }
    stream.SerialiseString(&name);
    i32 count;
    if (stream.version == 0) {
        stream.SerialiseBuffer(&count, sizeof(count), 1);
    }
    if (stream.mode == 2) {
        count = 0;
        for (EdRef *member = members; member != NULL; member = member->next) {
            if ((stream.flags & 0x400000) != 0) {
                if ((member->attributes & 0x400000) != 0) {
                    continue;
                }
            } else {
                if ((member->attributes & 0x10000000) != 0 ||
                    (class_mapping != NULL && member->attributes < 0 && class_mapping[member->type_id] == -1)) {
                    continue;
                }
            }
            ++count;
        }
        stream.SerialiseBuffer(&count, sizeof(count), 1);
        for (EdRef *member = members; member != NULL; member = member->next) {
            if ((stream.flags & 0x400000) != 0) {
                if ((member->attributes & 0x400000) != 0) {
                    continue;
                }
            } else {
                if ((member->attributes & 0x10000000) != 0 ||
                    (class_mapping != NULL && member->attributes < 0 && class_mapping[member->type_id] == -1)) {
                    continue;
                }
            }
            member->Serialise(stream, class_mapping);
        }
    }
    if (stream.mode == 1) {
        if (stream.version <= 2) {
            stream.SerialiseBuffer(&count, sizeof(count), 1);
            for (i32 i = 0; i < count; ++i) {
                EdRef *member = new (stream.secondary_buffer->Allocate(sizeof(EdRef))) EdRef;
                member->Serialise(stream, NULL);
                AddType(member);
            }
        }
        stream.SerialiseBuffer(&count, sizeof(count), 1);
        for (i32 i = 0; i < count; ++i) {
            EdRef *member = new (stream.secondary_buffer->Allocate(sizeof(EdRef))) EdRef;
            member->Serialise(stream, NULL);
            AddType(member);
        }
    }
    stream.EndBlock();
}

i32 EdClass::GetStreamClasses(EdStream &stream, i32 *classes, i32 &count, i32 capacity) {
    i32 result = 0;
    if (count < capacity) {
        classes[count] = theRegistry.GetClassId(this);
        ++count;
    }
    for (EdRef *member = members; member != NULL; member = member->next) {
        if (member->attributes >= 0) {
            continue;
        }
        if ((stream.flags & 0x400000) != 0 ? (member->attributes & 0x400000) != 0
                                           : (member->attributes & 0x10000000) != 0) {
            continue;
        }
        theRegistry.GetClass(member->type_id)->GetStreamClasses(stream, classes, count, capacity);
        result = 1;
    }
    return result;
}

EdRef *EdClass::FindTypeRef(char *member_name, i32 recursive) {
    for (EdRef *member = members; member != NULL; member = member->next) {
        if (recursive != 0 && member->attributes < 0) {
            EdRef *reference = theRegistry.GetClass(member->type_id)->FindTypeRef(member_name, 1);
            if (reference != NULL) {
                return reference;
            }
        } else if (NuStrICmp(member->name, member_name) == 0) {
            return member;
        }
    }
    return NULL;
}

void EdClass::SerialiseObject(EdStream &stream, void *object, EdClass *schema, EdRegistry *registry) {
    if (stream.BeginBlock("Object") == NULL) {
        return;
    }
    if (object != NULL) {
        u8 data[256];
        for (EdRef *source = schema->members; source != NULL; source = source->next) {
            EdRef *member = FindTypeRef(source->name, 0);
            if (member != NULL) {
                if (stream.mode != 1) {
                    if (stream.mode != 2) {
                        continue;
                    }
                    if ((stream.flags & 0x400000) != 0 ? (member->attributes & 0x400000) != 0
                                                       : (member->attributes & 0x10000000) != 0) {
                        continue;
                    }
                }
                if (member->attributes < 0) {
                    EdClass *source_class = registry->GetClass(source->type_id);
                    EdClass *member_class = theRegistry.GetClass(member->type_id);
                    member_class->SerialiseObject(stream, member->GetMemberObject(object), source_class, registry);
                } else {
                    registry->GetType(source->type_id);
                    EdType *type = theRegistry.GetType(member->type_id);
                    i32 size = member->size > 0 ? member->size : type->size;
                    if (stream.mode == 2) {
                        member->GetMemberData(object, member->type_id, data, sizeof(data));
                    }
                    type->serialise(stream, data, size);
                    if (stream.mode == 1) {
                        member->SetMemberData(object, member->type_id, data, sizeof(data), NULL);
                    }
                }
            } else {
                EdType *type = registry->GetType(source->type_id);
                if (source->size <= 0) {
                    stream.Eat(type->size, 1);
                } else if (NuStrICmp(type->name, "String") != 0) {
                    stream.Eat(source->size, 1);
                } else {
                    i32 length;
                    stream.SerialiseBuffer(&length, sizeof(length), 1);
                    stream.Eat(length, 1);
                }
            }
        }
        if (interface != NULL) {
            interface->vtable->serialise_object(interface, stream, object);
        }
    }
    stream.EndBlock();
}

EdRef *EdClass::FindTypeRef(i32 attributes, i32 recursive) {
    for (EdRef *member = members; member != NULL; member = member->next) {
        if (member->attributes < 0) {
            if (recursive != 0) {
                EdRef *reference = theRegistry.GetClass(member->type_id)->FindTypeRef(attributes, 1);
                if (reference != NULL) {
                    return reference;
                }
            }
        } else if ((member->attributes & attributes) != 0) {
            return member;
        }
    }
    return NULL;
}

i32 EdClass::FindMember(EdMember *result, void *object, i32 attributes, i32 recursive) {
    for (EdRef *member = members; member != NULL; member = member->next) {
        if (member->attributes < 0) {
            if (recursive != 0) {
                EdClass *member_class = theRegistry.GetClass(member->type_id);
                void *member_object = member->GetMemberObject(object);
                if (member_class->FindMember(result, member_object, attributes, 1) != 0) {
                    return 1;
                }
            }
        } else if ((member->attributes & attributes) != 0) {
            result->object = object;
            result->reference = member;
            return 1;
        }
    }
    return 0;
}

void *EdClass::FindObject(char *object_name) {
    void *object = interface->vtable->get_next_object(interface, NULL);
    while (object != NULL) {
        EdMember member;
        i32 string_type = EdType_String;
        if (FindMember(&member, object, 2, 1) != 0) {
            char name_buffer[256];
            if (member.reference->GetAttributeData(member.object, 2, string_type, name_buffer, sizeof(name_buffer)) !=
                    0 &&
                NuStrICmp(object_name, name_buffer) == 0) {
                return object;
            }
        }
        object = interface->vtable->get_next_object(interface, object);
    }
    return NULL;
}

EditorSettings::EditorSettings() {
    cursor_radius = 1.0f;
    snap_terrain = 1;
}

void EditorSettings::AddMenuItems(eduimenu_s *menu) {
    eduiMenuAddItem(menu, eduiItemSliderCreate(reinterpret_cast<usize>(&cursor_radius), &EdLevelAttr, 0,
                                               cbEdLevelSetSliderFloat, 1.0f, 50.0f, cursor_radius,
                                               const_cast<char *>("Cursor Radius")));
    eduiMenuAddItem(menu, eduiItemCheckCreate(reinterpret_cast<usize>(&snap_terrain), &EdLevelAttr, snap_terrain, 0,
                                              cbEdLevelToggleInt, const_cast<char *>("Snap Terrain")));
    edui_last_item->highlighted = snap_terrain & 1;
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

i32 ClassObjectList::IsInList(ClassObject object) {
    for (ClassObjectListEntry *entry = first; entry != NULL; entry = entry->next) {
        if (entry->object == object.object && entry->reference == object.reference) {
            return true;
        }
    }
    return false;
}

i32 ClassObjectList::IsInList(EdClass *ed_class) {
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
