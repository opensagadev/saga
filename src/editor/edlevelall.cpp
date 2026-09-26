#include "decomp.h"
#include "globals.h"
#include "gameapi/edtools/edcam.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/customiser.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/render/fx/particles.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/render/light/lighting.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nucore/NuDynamicLight.h"
#if defined(__i386__) && defined(__SSE__)
#include <xmmintrin.h>
#endif
#if defined(__i386__) && defined(__SSE2__)
#include <emmintrin.h>
#endif
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nufile/nufile.h"
#include <string.h>
#include <new>
#include <float.h>

#include <stdio.h>

extern i32 EdType_String;
extern i32 EdType_Float;
extern i32 EdType_VuMtx;
extern i32 EdType_VuVec;
void EdDrawBegin(i32);
void EdDrawEnd();
void EdDrawLineSegment(VuVec const &, VuVec const &, i32);
void EdDrawLineSphere(VuVec const &, float, float, i32);
void EdDrawLineCross(VuVec const &, float, i32);
void EdDrawPolyArrow(VuVec const &, VuVec const &, i32, i32, float, float, float, float);
i32 EdTerrRay(VuVec &, VuVec &);
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
CursorTool theCursorTool;
extern SplineTool theSplineTool;
static i32 class_editor_create_pending = -1;
extern EdManipulator theDefaultManipulator;
extern EdManMove theMoveManipulator;
extern EdManRotate theRotateManipulator;
extern EdManScale theScaleManipulator;
extern MemoryManager theMemoryManager;
extern eduimenu_s *edLevelNextMenu;
void eduiSetPinnedMenu(eduimenu_s *);

i32 BaseEditor::blockDepth;
i32 BaseEditor::blockStart[8];
eduimenu_s *edLevelActiveMenu;
extern eduimenu_s *edLevelPinnedMenu;
i32 edLevelDestroyActiveMenu;
eduimenu_s *edLevelDestroyThisMenu;
eduimenu_s *edLevelDestroyThisMenu2;
i32 edlevel_mouseandkeyboard;
LevelEditor theLevelEditor;

extern "C" void eduiSetCameraEnabled(i32);
eduimenu_s *GetMenuActiveChild(eduimenu_s *);
extern "C" void NuFntSet(i32);
extern "C" void NuFntScale(i32, i32);
extern "C" void NuRndrRect2di(i32, i32, i32, i32, i32, NUMTL *);
extern "C" void eduiSetCursorColour(u32);
extern "C" void eduiSetFontScale(f32, f32);

PropertyTool thePropertyTool;
PropertyMenuMetrics menu_startmetrics __attribute__((aligned(16))) = {20, 5, 200, 400};
eduiiattr_s EdLevelAttr = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
i32 EdLevelFnt;
i32 EdLevelFntScale = 24;
NUMTL *edLevel2dMtl;
NUMTL *edLevel3dMtl;
NUMTL *edLevelTexMtl;

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

i32 CursorTool::Process(EdInputContext &input) {
    VuVec &origin = *reinterpret_cast<VuVec *>(input.reserved_00 + 0x20);
    VuVec &direction = *reinterpret_cast<VuVec *>(input.reserved_00 + 0x30);
    const bool extend_selection = static_cast<i32>(input.GetHold(16)) != 0;
    VuVec end = origin;
    end.x += direction.x;
    end.y += direction.y;
    end.z += direction.z;
    EdDrawBegin(0);
    EdDrawLineSegment(origin, end, NuRandInt());
    EdDrawEnd();
    if (theLevelEditor.field_0x30 == 0)
        return 0;

    ClassObject hover = {NULL, NULL, NULL};
    ClassObject after = {NULL, NULL, NULL};
    if (theClassEditor.selected_objects.first != NULL) {
        ClassObjectListEntry *first = theClassEditor.selected_objects.first;
        after = {first->ed_class, first->object, first->reference};
    }
    if (theClassEditor.FindNearestObject(origin, direction, hover, 1)) {
        theClassEditor.current_object = hover;
        if (input.GetPress(3) != 0.0f) {
            theClassEditor.FindNearestObject(origin, direction, hover, after, 1);
            theClassEditor.SelectObject(hover, extend_selection ? 1 : 0);
        }
        char name[128];
        if (!get_class_object_attribute(hover.ed_class, hover.object, hover.reference, 2, EdType_String, name,
                                        sizeof(name))) {
            theLevelEditor.AddInfoText(const_cast<char *>("object : no name"));
        } else {
            char text[136];
            sprintf(text, "object : %s", name);
            theLevelEditor.AddInfoText(text);
        }
    } else {
        theClassEditor.current_object = {NULL, NULL, NULL};
        if (input.GetPress(11) != 0.0f) {
            if (after.object != NULL)
                theClassEditor.CreateObject(after);
            else
                theClassEditor.CreateObject();
        }
    }
    if (input.GetPress(12) != 0.0f)
        theClassEditor.DestroySelectedObjects();
    if (hover.object == NULL && input.GetPress(3) != 0.0f) {
        ClassObject none = {NULL, NULL, NULL};
        theClassEditor.SelectObject(none, 0);
    }
    return 0;
}

void CursorTool::Render() {
    thePropertyTool.Render();
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

i32 ClassEditor::CreateObject(ClassObject &source) {
    ClassObject created = {source.ed_class, NULL, NULL};
    if (theLevelEditor.current_led_file == -1) {
        SelectLED(-1);
        return 1;
    }
    Placeable::CurrentLedFile = theLevelEditor.current_led_file;
    EdClass *ed_class = created.ed_class;
    EdClassInterface *interface = ed_class->interface;
    void *object = theRegistry.CreateObject(interface, (ed_class->flags & 0x04000000) ? source.object : NULL, 4, 0, 2);
    if (object == NULL)
        return 0;

    created.object = object;
    if (ed_class->flags & 0x04000000) {
        i32 flags = 0x04000000;
        if (created.reference == NULL || !created.reference->SetAttributeData(object, 1, EdType_Int, &flags, 0)) {
            EdMember member;
            if (ed_class->FindMember(&member, object, 1, 1))
                member.reference->SetAttributeData(member.object, 1, EdType_Int, &flags, 0);
        }
    } else {
        ed_class->CopyObject(object, source.object);
        interface->vtable->construct(interface, object, source.object);
    }
    InitialiseObject(created);
    theRegistry.NotifyCreateObject(object, ed_class, NULL, 0, 0, 0);
    SelectObject(created, 0);
    return 1;
}

i32 ClassEditor::CreateObject(EdClass *ed_class) {
    if (ed_class == NULL || ed_class->interface == NULL)
        return 0;
    void *object = theRegistry.CreateObject(ed_class->interface, NULL, 4, 0, 2);
    if (object == NULL)
        return 0;
    ClassObject created = {ed_class, object, NULL};
    InitialiseObject(created);
    theRegistry.NotifyCreateObject(object, ed_class, NULL, 0, 0, 0);
    SelectObject(created, 0);
    return 1;
}

i32 ClassEditor::CreateObject(i32 class_id) {
    if (theLevelEditor.current_led_file == -1) {
        SelectLED(class_id);
        return 1;
    }
    Placeable::CurrentLedFile = theLevelEditor.current_led_file;
    EdClass *ed_class = theRegistry.GetClass(class_id);
    if (ed_class == NULL || (ed_class->flags & 0x04000000))
        return 0;
    return CreateObject(ed_class);
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

void ClassEditor::Initialise(variptr_u &first, variptr_u &second, i32) {
    theRegistry.Initialise(first, second, 32, 32, 10, 1);
    theRegistry.RegisterBaseTypes();
    RegisterTool(theCursorTool);
    RegisterTool(theSplineTool);
    active_tool = &theCursorTool;
}

void ClassEditor::Process(EdInputContext &input) {
    UpdateSnapRay(*reinterpret_cast<VuVec *>(reinterpret_cast<u8 *>(&input) + 0x30));
    UpdateSelectedObjects(input);
    UpdateClassFilter(input);
    if (menu != NULL) {
        theLevelEditor.field_0x30 = 0;
        theLevelEditor.field_0x38 = 0;
        NuFntSet(EdLevelFnt);
        NuFntScale(EdLevelFntScale, EdLevelFntScale);
        eduiMenuProcess(menu, *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(&input) + 0x44),
                        *reinterpret_cast<nupad_s **>(reinterpret_cast<u8 *>(&input) + 0x40));
        if (menu != NULL)
            return;
    }
    if (input.GetHold(13) != 0.0f)
        return;
    field_3c = 0xff808080;
    theLevelEditor.field_0x30 = 1;
    theLevelEditor.field_0x38 = 1;
    if (thePropertyTool.Process(input) == 0) {
        if (input.GetPress(5) != 0.0f)
            SetMode(0);
        if (input.GetPress(6) != 0.0f)
            SetMode(3);
        if (input.GetPress(7) != 0.0f)
            SetMode(4);
        if (input.GetPress(8) != 0.0f)
            SetMode(5);
        if (input.GetPress(37) != 0.0f) {
            ClassObject nearest = {};
            VuVec &origin = *reinterpret_cast<VuVec *>(input.reserved_00 + 0x20);
            VuVec &direction = *reinterpret_cast<VuVec *>(input.reserved_00 + 0x30);
            if (FindNearestObject(origin, direction, nearest, 1) == 0)
                cbEdCopySelectedObject(input);
            else
                DestroySelectedObjects();
        }
        if (manipulator == NULL)
            manipulator = &theDefaultManipulator;
        if (manipulator->Process(input, selected_objects) == 0 && active_tool != NULL)
            active_tool->Process(input);
    }
    if (selected_objects.count > 0) {
        if (input.GetPress(9) != 0.0f)
            FocusSelected();
        if (input.GetPress(10) != 0.0f)
            ViewSelected();
        for (ClassObjectListEntry *entry = selected_objects.first; entry != NULL; entry = entry->next)
            theRegistry.ClassIFaceProcess(entry->ed_class, entry->object, input);
        if (input.GetPress(39) != 0.0f) {
            ClassObjectListEntry *entry = selected_objects.first;
            EdMember member;
            VuVec position __attribute__((aligned(16)));
            if (entry->reference == NULL ||
                !entry->reference->GetAttributeData(entry->object, 8, EdType_VuVec, &position, 0)) {
                if (entry->ed_class->FindMember(&member, entry->object, 8, 1))
                    member.reference->GetAttributeData(member.object, 8, EdType_VuVec, &position, 0);
            }
            theLevelEditor.background_colour[0] = position.x;
            theLevelEditor.background_colour[1] = position.y;
            theLevelEditor.background_colour[2] = position.z;
            theLevelEditor.background_colour[3] = position.w;
            edcamSetPos(reinterpret_cast<NUVEC *>(theLevelEditor.background_colour));
            thePropertyTool.Process(input);
            if (mode == 0)
                mode = 3;
            SetMode(mode);
            thePropertyTool.SetDefaultActiveMenu(NULL);
        }
    }
}

i32 ClassEditor::ReadBlock(DATAPTR *) {
    return 0;
}

void ClassEditor::Render() {
    thePropertyTool.Render();
    if (active_tool != NULL)
        active_tool->Render();
    if (current_object.object != NULL && !IsSelectedObject(current_object))
        DrawObjectSphere(current_object, field_3c);
    if (selected_objects.count > 0) {
        if (manipulator != NULL) {
            manipulator->Render(selected_objects);
        } else {
            for (ClassObjectListEntry *entry = selected_objects.first; entry != NULL; entry = entry->next) {
                DrawObjectSphere(*reinterpret_cast<ClassObject *>(&entry->ed_class), 0xff800000);
            }
        }
        for (ClassObjectListEntry *entry = selected_objects.first; entry != NULL; entry = entry->next)
            theRegistry.ClassIFaceRender(entry->ed_class, entry->object, 1);
    }

    for (i32 index = 0; index < theRegistry.class_count; ++index) {
        EdClass *ed_class = theRegistry.GetClass(index);
        if (!Editable(NULL, ed_class, index) || ed_class->interface == NULL || (ed_class->flags & 0x08000080) == 0)
            continue;

        EdClassInterface *interface = ed_class->interface;
        for (void *object = interface->vtable->get_next_object(interface, NULL); object != NULL;
             object = interface->vtable->get_next_object(interface, object)) {
            if (!Editable(object, ed_class, -1))
                continue;

            ClassObject selected = {ed_class, object, NULL};
            if ((ed_class->flags & 0x08000000) != 0 && selected_objects.IsInList(selected) == 0)
                theRegistry.ClassIFaceRender(ed_class, object, 0);
            if ((ed_class->flags & 0x80) != 0 && selected_objects.IsInList(selected) != 0 &&
                object != current_object.object)
                DrawObjectSphere(selected, 0xff000080);
        }
    }

    if (menu != NULL)
        eduiMenuRender(menu);
}

template <typename T> T *CreateObject(MemoryBuffer *buffer) {
    void *storage = buffer->Allocate(sizeof(T));
    if (storage == NULL)
        return NULL;
    return new (storage) T();
}

void ClassEditor::Serialise(EdStream &stream) {
    if (stream.mode == 2) {
        theRegistry.Serialise(stream);
        theRegistry.SerialiseObjects(stream, NULL);
    }
    if (stream.mode == 1) {
        MemoryBuffer *saved = stream.memory_buffer;
        stream.memory_buffer = stream.secondary_buffer;
        EdRegistry *source = ::CreateObject<EdRegistry>(stream.secondary_buffer);
        source->Initialise(*stream.secondary_buffer->position, *stream.secondary_buffer->end, 50, 50, 10, 1);
        source->Serialise(stream);
        stream.memory_buffer = saved;
        theRegistry.SerialiseObjects(stream, source);
    }
}

void ClassEditor::WriteBlock(i32) {
}

void ClassEditor::DestroySelectedObjects() {
    char title[136];
    NuStrCpy(title, "Destroy Selected Objects");
    f32 x, y;
    eduiGetCursorCoords(&x, &y);
    eduimenu_s *confirm_menu =
        eduiMenuCreate(static_cast<i32>(x * 640.0f), static_cast<i32>(y * 448.0f), 300, 50,
                       reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbDestroyMenu, title);
    if (confirm_menu == NULL)
        return;
    eduiMenuAddItem(confirm_menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbDestroyObject, const_cast<char *>("Yes")));
    eduiMenuAddItem(confirm_menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbDestroyObject, const_cast<char *>("No")));
    eduiMenuFitWidth(confirm_menu, 5);
    eduiMenuFitOnScreen(confirm_menu, 1);
    theLevelEditor.SetNextMenu(confirm_menu);
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

void ClassEditor::DrawObjectSphere(ClassObject &selected, i32 colour) {
    if (selected.object == NULL)
        return;
    f32 radius;
    EdMember member;
    i32 radius_type = EdType_Float;
    if (selected.reference == NULL ||
        selected.reference->GetAttributeData(selected.object, 0x40, radius_type, &radius, 0) == 0) {
        if (selected.ed_class->FindMember(&member, selected.object, 0x40, 1) == 0 ||
            member.reference->GetAttributeData(member.object, 0x40, radius_type, &radius, 0) == 0) {
            radius = 1.0f;
        }
    }
    // The original places this matrix on a 16-byte-aligned stack slot.
    VuMtx transform __attribute__((aligned(16)));
    i32 matrix_type = EdType_VuMtx;
    if ((selected.reference == NULL ||
         selected.reference->GetAttributeData(selected.object, 0x10, matrix_type, &transform, 0) == 0) &&
        (selected.ed_class->FindMember(&member, selected.object, 0x10, 1) == 0 ||
         member.reference->GetAttributeData(member.object, 0x10, matrix_type, &transform, 0) == 0)) {
        VuVec position;
        i32 position_type = EdType_VuVec;
        if ((selected.reference != NULL &&
             selected.reference->GetAttributeData(selected.object, 8, position_type, &position, 0) != 0) ||
            (selected.ed_class->FindMember(&member, selected.object, 8, 1) != 0 &&
             member.reference->GetAttributeData(member.object, 8, position_type, &position, 0) != 0)) {
            EdDrawBegin(0);
            EdDrawLineSphere(position, radius, 1.0f, colour);
            EdDrawEnd();
        }
        return;
    }

    VuVec const &position = *reinterpret_cast<VuVec const *>(&transform.matrix.m30);
    EdDrawBegin(0);
    EdDrawLineSphere(position, radius, 1.0f, colour);
    VuVec tip(transform.matrix.m30 - transform.matrix.m20 * radius,
              transform.matrix.m31 - transform.matrix.m21 * radius,
              transform.matrix.m32 - transform.matrix.m22 * radius, 0.0f);
    EdDrawPolyArrow(position, tip, 4, colour, 0.1f, 0.1f, 0.1f, 0.1f);
    EdDrawEnd();
}

i32 ClassEditor::Editable(void *object, EdClass *object_class, i32 index) {
    i16 scene = 0;
    if (object == NULL) {
        return (class_filter >> index) & 1;
    }
    i32 short_type = EdType_Short;
    EdMember member __attribute__((aligned(16)));
    if (object_class->FindMember(&member, object, 256, 1)) {
        member.reference->GetAttributeData(member.object, 256, short_type, &scene, 0);
    }
    return theLevelEditor.IsEditable(scene) != 0;
}

i32 ClassEditor::FindNearestObject(VuVec &point, ClassObject &result, ClassObject &after, i32 filter) {
    ClassObject candidates[16];
    i32 candidate_count = 0;
    for (i32 class_index = 0; class_index < theRegistry.class_count; ++class_index) {
        EdClass *ed_class = &theRegistry.classes[class_index];
        EdClassInterface *interface = ed_class->interface;
        if (!Editable(NULL, ed_class, class_index) || interface == NULL || (ed_class->flags & 8) == 0 ||
            interface->vtable->get_next_object == NULL)
            continue;
        for (void *object = interface->vtable->get_next_object(interface, NULL); object != NULL;
             object = interface->vtable->get_next_object(interface, object)) {
            if (!Editable(object, ed_class, -1))
                continue;
            EdMember member;
            if (!ed_class->FindMember(&member, object, 8, 1))
                continue;
            VuVec position;
            member.reference->GetAttributeData(member.object, 8, EdType_VuVec, &position, 0);
            f32 dx = position.x - point.x;
            f32 dy = position.y - point.y;
            f32 dz = position.z - point.z;
            f32 radius = 1.0f;
            if (ed_class->FindMember(&member, object, 0x40, 1))
                member.reference->GetAttributeData(member.object, 0x40, EdType_Float, &radius, 0);
            if (dx * dx + dy * dy + dz * dz < radius * radius && candidate_count < 16)
                candidates[candidate_count++] = {ed_class, object, NULL};
        }
    }
    if (candidate_count == 0)
        return 0;
    i32 choice = 0;
    if (after.object != NULL) {
        for (i32 index = 0; index < candidate_count; ++index) {
            if (candidates[index].object == after.object) {
                choice = (index + 1) % candidate_count;
                break;
            }
        }
    }
    result.ed_class = candidates[choice].ed_class;
    result.object = candidates[choice].object;
    return 1;
}

i32 ClassEditor::FindNearestObject(VuVec &point, ClassObject &result, i32 filter) {
    EdClass *nearest_class = NULL;
    void *nearest_object = NULL;
    f32 nearest_distance = FLT_MAX;
    for (i32 class_index = 0; class_index < theRegistry.class_count; ++class_index) {
        EdClass *ed_class = &theRegistry.classes[class_index];
        EdClassInterface *interface = ed_class->interface;
        if (!Editable(NULL, ed_class, class_index) || (ed_class->flags & 8) == 0)
            continue;
        for (void *object = interface->vtable->get_next_object(interface, NULL); object != NULL;
             object = interface->vtable->get_next_object(interface, object)) {
            EdMember member;
            if (!ed_class->FindMember(&member, object, 8, 1))
                continue;
            VuVec position;
            member.reference->GetAttributeData(member.object, 8, EdType_VuVec, &position, 0);
            f32 dx = position.x - point.x;
            f32 dy = position.y - point.y;
            f32 dz = position.z - point.z;
            f32 distance = dx * dx + dy * dy + dz * dz;
            if (distance < nearest_distance) {
                nearest_class = ed_class;
                nearest_object = object;
                nearest_distance = distance;
            }
        }
    }
    if (nearest_object != NULL && filter != 0) {
        f32 radius = 1.0f;
        if ((nearest_class->flags & 0x40) != 0) {
            EdMember member;
            if (nearest_class->FindMember(&member, nearest_object, 0x40, 1))
                member.reference->GetAttributeData(member.object, 0x40, EdType_Float, &radius, 0);
        }
        if (NuFsqrt(nearest_distance) > radius) {
            nearest_class = NULL;
            nearest_object = NULL;
        }
    }
    result.ed_class = nearest_class;
    result.object = nearest_object;
    return nearest_object != NULL;
}

i32 ClassEditor::FindNearestObject(VuVec &origin, VuVec &direction, ClassObject &result, ClassObject &after,
                                   i32 filter) {
    ClassObject candidates[16] __attribute__((aligned(16))) = {};
    i32 candidate_count = 0;
    for (i32 class_index = 0; class_index < theRegistry.class_count; ++class_index) {
        EdClass *ed_class = &theRegistry.classes[class_index];
        EdClassInterface *interface = ed_class->interface;
        if (!Editable(NULL, ed_class, class_index) || interface == NULL || (ed_class->flags & 8) == 0)
            continue;
        for (void *object = interface->vtable->get_next_object(interface, NULL); object != NULL;
             object = interface->vtable->get_next_object(interface, object)) {
            if (!Editable(object, ed_class, -1))
                continue;
            EdRef *reference = NULL;
            f32 distance = interface->vtable->distance_to_ray(interface, origin, direction, object, &reference);
            if (distance < 0.01f && candidate_count < 16)
                candidates[candidate_count++] = {ed_class, object, reference};
        }
    }
    if (candidate_count == 0)
        return 0;
    i32 choice = 0;
    if (after.object != NULL) {
        for (i32 index = 0; index < candidate_count; ++index) {
            if (candidates[index].object == after.object) {
                choice = (index + 1) % candidate_count;
                break;
            }
        }
    }
    result = candidates[choice];
    return 1;
}

i32 ClassEditor::FindNearestObject(VuVec &origin, VuVec &direction, ClassObject &result, i32 filter) {
    EdClass *nearest_class = NULL;
    void *nearest_object = NULL;
    EdRef *nearest_reference = NULL;
    f32 nearest_distance = FLT_MAX;
    for (i32 class_index = 0; class_index < theRegistry.class_count; ++class_index) {
        EdClass *ed_class = &theRegistry.classes[class_index];
        EdClassInterface *interface = ed_class->interface;
        if (!Editable(NULL, ed_class, class_index) || interface == NULL || (ed_class->flags & 8) == 0)
            continue;
        for (void *object = interface->vtable->get_next_object(interface, NULL); object != NULL;
             object = interface->vtable->get_next_object(interface, object)) {
            if (!Editable(object, ed_class, -1))
                continue;
            EdRef *reference = NULL;
            f32 distance = interface->vtable->distance_to_ray(interface, origin, direction, object, &reference);
            if (distance < nearest_distance) {
                nearest_class = ed_class;
                nearest_object = object;
                nearest_reference = reference;
                nearest_distance = distance;
            }
        }
    }
    if (filter != 0 && nearest_distance > 0.1f) {
        nearest_class = NULL;
        nearest_object = NULL;
    }
    result = {nearest_class, nearest_object, nearest_reference};
    return nearest_object != NULL;
}

void ClassEditor::FocusSelected() {
    VuVec position __attribute__((aligned(16)));
    f32 radius;
    if (selected_objects.GetAveragePosition(position, radius)) {
        theLevelEditor.background_colour[0] = position.x;
        theLevelEditor.background_colour[1] = position.y;
        theLevelEditor.background_colour[2] = position.z;
        theLevelEditor.background_colour[3] = position.w;
        edcamSetPos(reinterpret_cast<NUVEC *>(theLevelEditor.background_colour));
        NUCAMERA *camera = edmainGetCamera();
        i32 angle = static_cast<i32>(camera->fov * 0.5f * 10430.3779296875f);
        f32 cosine = NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff];
        f32 sine = NuTrigTable[(angle >> 1) & 0x7fff];
        f32 distance = 1.2f * radius * (cosine / sine);
        edcamSetDist(-distance);
    }
}

void ClassEditor::InitialiseObject(ClassObject &created) {
    EdMember member;
    if (created.ed_class->FindMember(&member, created.object, 2, 1)) {
        char name[128];
        char unique_name[128];
        member.reference->GetAttributeData(member.object, 2, EdType_String, name, sizeof(name));
        MakeUniqueName(name, unique_name, sizeof(unique_name));
        member.reference->SetAttributeData(member.object, 2, EdType_String, unique_name, 0);
    }
    if (created.ed_class->FindMember(&member, created.object, 8, 1)) {
        member.reference->SetAttributeData(member.object, 8, EdType_VuVec, reinterpret_cast<u8 *>(manipulator) + 0x50,
                                           0);
    }
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
        if (object == NULL)
            continue;
        do {
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
        } while (object != NULL);
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
        prefix_length = size + ~digit_count;
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

void ClassEditor::SelectLED(i32 class_id) {
    f32 x, y;
    eduiGetCursorCoords(&x, &y);
    class_editor_create_pending = class_id;
    eduimenu_s *led_menu = eduiMenuCreate(static_cast<i32>(x * 640.0f), static_cast<i32>(y * 448.0f), 300, 250,
                                          reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy,
                                          const_cast<char *>("Select LED File"));
    if (led_menu == NULL)
        return;
    for (i32 index = 0; index < theLevelEditor.reset_pending; ++index) {
        LevelEditorScene *scene = theLevelEditor.GetEdScene(index);
        if (scene != NULL && scene->editable)
            eduiMenuAddItem(led_menu, eduiItemSelCreate(index, &EdLevelAttr, 0, 0, cbFileSelected, scene->name));
    }
    eduiMenuFitWidth(led_menu, 5);
    eduiMenuFitOnScreen(led_menu, 1);
    theLevelEditor.SetNextMenu(led_menu);
}

i32 ClassEditor::SelectObject(ClassObject &object, i32 mode) {
    i32 selected = 1;
    if (mode == 1) {
        if (object.object == NULL) {
            selected = 0;
            goto update_property_selection;
        }
        for (ClassObjectListEntry *entry = selected_objects.first; entry != NULL; entry = entry->next) {
            if (entry->object == object.object && entry->reference == object.reference) {
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
                selected = 0;
                goto update_property_selection;
            }
        }
    } else if (mode == 2) {
        for (ClassObjectListEntry *entry = selected_objects.first; entry != NULL;) {
            ClassObjectListEntry *next = entry->next;
            if (entry->object == object.object) {
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
            entry = next;
        }
    } else {
        while (selected_objects.first != NULL) {
            ClassObjectListEntry *entry = selected_objects.first;
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

    if (object.object != NULL) {
        ClassObjectListEntry *entry =
            new (theMemoryManager.AllocPool(sizeof(ClassObjectListEntry), 1)) ClassObjectListEntry;
        memset(&entry->ed_class, 0, sizeof(object));
        entry->next = NULL;
        memcpy(&entry->ed_class, &object, sizeof(object));
        entry->previous = selected_objects.last;
        if (selected_objects.last != NULL) {
            selected_objects.last->next = entry;
        }
        ClassObjectListEntry *first = selected_objects.first;
        selected_objects.last = entry;
        if (first == NULL) {
            selected_objects.first = entry;
        }
        ++selected_objects.count;
    }
update_property_selection:
    thePropertyTool.SelectAttr(manipulator->selected_attribute);
    return selected;
}

void ClassEditor::SetMode(i32 new_mode) {
    mode = new_mode;
    switch (new_mode) {
        case 3:
            manipulator = &theMoveManipulator;
            break;
        case 4:
            manipulator = &theRotateManipulator;
            break;
        case 5:
            manipulator = &theScaleManipulator;
            break;
        default:
            manipulator = &theDefaultManipulator;
            break;
    }
    thePropertyTool.SelectAttr(manipulator->selected_attribute);
}

void ClassEditor::SetViewMenuHilight(eduimenu_s *menu) {
    for (eduiitem_s *item = menu->first; item != NULL; item = item->next) {
        i32 index = item->data - 3;
        if (static_cast<u32>(index) <= 60) {
            item->highlighted = (theClassEditor.class_filter >> index) & 1;
        }
    }
}

void ClassEditor::SnapPoint(VuVec &point) {
    if (static_cast<u32>(snap_mode - 1) > 1)
        return;
    VuVec ray = snap_ray;
    NuVecNorm(reinterpret_cast<NUVEC *>(&ray), reinterpret_cast<NUVEC *>(&ray));
    ray.x *= -snap_distance;
    ray.y *= -snap_distance;
    ray.z *= -snap_distance;
    ray.w = 0.0f;
    VuVec candidate = point;
    candidate.x += ray.x;
    candidate.y += ray.y;
    candidate.z += ray.z;
    if (EdTerrRay(candidate, snap_ray))
        point = candidate;
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

void ClassEditor::UpdateSelectedObjects(EdInputContext &input) {
    for (ClassObjectListEntry *entry = selected_objects.first; entry != NULL;) {
        ClassObjectListEntry *next_entry = entry->next;
        EdClassInterface *interface = entry->ed_class->interface;
        void *object = interface->vtable->get_next_object(interface, NULL);
        for (;;) {
            if (object == NULL)
                break;
            if (object == entry->object)
                break;
            object = interface->vtable->get_next_object(interface, object);
        }
        if (object == NULL) {
            if (entry->next != NULL)
                entry->next->previous = entry->previous;
            else
                selected_objects.last = entry->previous;
            if (entry->previous != NULL)
                entry->previous->next = entry->next;
            else
                selected_objects.first = entry->next;
            entry->next = NULL;
            entry->previous = NULL;
            --selected_objects.count;
            theMemoryManager.FreePool(entry, sizeof(*entry));
        }
        entry = next_entry;
    }
    if (selected_objects.first == NULL)
        return;
    ClassObjectListEntry *entry = selected_objects.first;
    EdClassInterface *interface = entry->ed_class->interface;
    void *object = NULL;
    if (input.GetPress(19) != 0.0f) {
        object = interface->vtable->get_next_object(interface, entry->object);
        if (object == NULL)
            object = interface->vtable->get_next_object(interface, NULL);
        for (i32 remaining = 4096; remaining != 0; --remaining) {
            if (Editable(object, entry->ed_class, -1))
                break;
            object = interface->vtable->get_next_object(interface, object);
            if (object == NULL)
                object = interface->vtable->get_next_object(interface, NULL);
        }
    }
    if (input.GetPress(20) != 0.0f) {
        void *current = entry->object;
        void *previous = NULL;
        for (i32 remaining = 4096; remaining != 0; --remaining) {
            if (Editable(current, entry->ed_class, -1))
                previous = current;
            current = interface->vtable->get_next_object(interface, current);
            if (current == NULL)
                current = interface->vtable->get_next_object(interface, NULL);
            if (current == entry->object)
                break;
        }
        object = previous;
    }
    if (object != NULL) {
        ClassObject selected = {entry->ed_class, object, NULL};
        theClassEditor.SelectObject(selected, 0);
        theClassEditor.ViewSelected();
    }
}

void ClassEditor::ViewSelected() {
    VuVec position;
    if (selected_objects.GetAveragePosition(position)) {
        theLevelEditor.background_colour[0] = position.x;
        theLevelEditor.background_colour[1] = position.y;
        theLevelEditor.background_colour[2] = position.z;
        theLevelEditor.background_colour[3] = position.w;
        edcamSetPos(reinterpret_cast<NUVEC *>(theLevelEditor.background_colour));
    }
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
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
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
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
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
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
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

void ClassEditor::cbEdClassNewObject(eduimenu_s *parent, eduiitem_s *item, u32) {
    i32 class_id = static_cast<i32>(item->data);
    EdClass *ed_class = theRegistry.GetClass(class_id);
    if (theClassEditor.cbEdCreateClassNewObject(class_id)) {
        theLevelEditor.CloseMenu();
        return;
    }
    eduimenu_s *error_menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                            reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy,
                                            const_cast<char *>("Error!"));
    if (error_menu == NULL)
        return;
    eduiMenuAddItem(error_menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect,
                                                  const_cast<char *>("Failed to create new object")));
    if (ed_class == NULL || (ed_class->flags & 0x04000000))
        eduiMenuAddItem(error_menu,
                        eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect,
                                          const_cast<char *>(ed_class == NULL ? "Unknown class id"
                                                                              : "No selected objects to clone")));
    eduiMenuFitWidth(error_menu, 5);
    eduiMenuFitOnScreen(error_menu, 1);
    eduiMenuAttach(parent, error_menu);
}

void ClassEditor::cbEdClassRemoveDuplicates(eduimenu_s *, eduiitem_s *, u32) {
    Placeable *objects[256];
    i32 count = 0;
    for (Placeable *object = static_cast<Placeable *>(thePlaceableHelper.GetNextObject(NULL));
         object != NULL && count < 256; object = static_cast<Placeable *>(thePlaceableHelper.GetNextObject(object))) {
        objects[count++] = object;
    }
    for (i32 first = 0; first < count; ++first) {
        Placeable *object = objects[first];
        if (object == NULL)
            continue;
        char const *name = object->GetName();
        VuVec const *position = object->GetInitialPosition();
        NUVEC first_position = {position->x, position->y, position->z};
        i32 object_type = 0;
        for (; object_type < thePlaceableHelper.object_type_count; ++object_type) {
            void *interface = thePlaceableHelper.object_types[object_type].interface;
            if (reinterpret_cast<i32 *>(interface)[2] == object->scene_id)
                break;
        }
        if (NuVecMag(&first_position) <= 0.1f)
            continue;
        for (i32 second = first + 1; second < count; ++second) {
            Placeable *other = objects[second];
            if (other == NULL)
                continue;
            char const *other_name = other->GetName();
            VuVec const *other_position = other->GetInitialPosition();
            i32 other_type = 0;
            for (; other_type < thePlaceableHelper.object_type_count; ++other_type) {
                void *interface = thePlaceableHelper.object_types[other_type].interface;
                if (reinterpret_cast<i32 *>(interface)[2] == other->scene_id)
                    break;
            }
            if (NuVecMag(&first_position) <= 0.1f)
                continue;
            NUVEC separation = {first_position.x - other_position->x, first_position.y - other_position->y,
                                first_position.z - other_position->z};
            if (NuVecMag(&separation) < 0.1f && object_type == other_type &&
                (strstr(name, other_name) != NULL || strstr(other_name, name) != NULL)) {
                EdClassInterface *interface =
                    reinterpret_cast<EdClassInterface *>(thePlaceableHelper.object_types[object_type].interface);
                interface->vtable->destroy_object(interface, other, 0);
                objects[second] = NULL;
            }
        }
    }
}

void ClassEditor::cbEdClassSelectClassMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
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
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (menu == NULL)
        return;
    EdClassInterface *interface = ed_class->interface;
    EdRef *name_ref = ed_class->FindTypeRef(2, 1);
    for (void *object = interface->vtable->get_next_object(interface, NULL); object != NULL;
         object = interface->vtable->get_next_object(interface, object)) {
        char name[128];
        if (name_ref == NULL || !name_ref->GetAttributeData(object, 2, EdType_String, name, sizeof(name)))
            NuStrCpy(name, ed_class->name);
        if (theClassEditor.Editable(object, ed_class, -1))
            eduiMenuAddItem(menu, eduiItemSelCreate(reinterpret_cast<usize>(object), &EdLevelAttr, 0, 0,
                                                    cbEdClassSelectObject, name));
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
        if (edLevelActiveMenu == menu) {
            edLevelActiveMenu = NULL;
        }
        eduiSetPinnedMenu(NULL);
    }
}

void ClassEditor::cbEdClassSetSnap(eduimenu_s *menu, eduiitem_s *item, u32) {
    theClassEditor.snap_mode = item->data;
    eduiMenuHighlight(menu, item);
}

void ClassEditor::cbEdClassSetView(eduimenu_s *menu, eduiitem_s *item, u32) {
    i32 index = item->data;
    if (index == 1) {
        theClassEditor.class_filter = 0;
    } else if (static_cast<u32>(index) < 1) {
        theClassEditor.class_filter = -1;
    } else if (index == 2) {
        theClassEditor.class_filter = ~theClassEditor.class_filter;
    } else if (index - 3 >= 0) {
        theClassEditor.class_filter ^= 1 << (index - 3);
    }
    SetViewMenuHilight(menu);
}

void ClassEditor::cbEdClassSnapMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
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
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
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
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
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

i32 ClassEditor::cbEdCopySelectedObject(EdInputContext &) {
    if (theClassEditor.selected_objects.first != NULL) {
        return theClassEditor.CreateObject(
            *reinterpret_cast<ClassObject *>(&theClassEditor.selected_objects.first->ed_class));
    }
    return 0;
}

i32 ClassEditor::cbEdCreateClassNewObject(i32 class_id) {
    EdClass *ed_class = theRegistry.GetClass(class_id);
    for (ClassObjectListEntry *entry = theClassEditor.selected_objects.first; entry != NULL; entry = entry->next) {
        if (entry->ed_class == ed_class) {
            return theClassEditor.CreateObject(*reinterpret_cast<ClassObject *>(&entry->ed_class));
        }
    }
    if (ed_class->flags & 0x04000000)
        return 0;
    return theClassEditor.CreateObject(class_id);
}

void ClassEditor::cbEdFilterLED(eduimenu_s *, eduiitem_s *item, u32) {
    if (item == NULL)
        return;
    LevelEditorScene *scene = theLevelEditor.GetEdScene(static_cast<u32>(item->data) >> 6);
    if (scene == NULL)
        return;
    theLevelEditor.current_led_file = -1;
    i32 editable = scene->editable + 1;
    scene->editable = editable;
    item->highlighted = editable;
}

void ClassEditor::cbEdLevelDeselectAll(eduimenu_s *menu, eduiitem_s *, u32) {
    // The original callback walks the first explicit argument as an item list.
    eduiitem_s *item = reinterpret_cast<eduiitem_s *>(menu);
#define EDLEVEL_DESELECT_SCENE(index)                                                                                  \
    do {                                                                                                               \
        LevelEditorScene *scene = theLevelEditor.GetEdScene(index);                                                    \
        if (scene != NULL && scene->active) {                                                                          \
            item = item->previous;                                                                                     \
            scene->editable = 0;                                                                                       \
            item->highlighted = 0;                                                                                     \
        }                                                                                                              \
    } while (0)
    EDLEVEL_DESELECT_SCENE(0);
    EDLEVEL_DESELECT_SCENE(1);
    EDLEVEL_DESELECT_SCENE(2);
    EDLEVEL_DESELECT_SCENE(3);
    EDLEVEL_DESELECT_SCENE(4);
    EDLEVEL_DESELECT_SCENE(5);
    EDLEVEL_DESELECT_SCENE(6);
    EDLEVEL_DESELECT_SCENE(7);
    EDLEVEL_DESELECT_SCENE(8);
    EDLEVEL_DESELECT_SCENE(9);
#undef EDLEVEL_DESELECT_SCENE
}

void ClassEditor::cbEdLevelSelectAll(eduimenu_s *menu, eduiitem_s *, u32) {
    // The original callback walks the first explicit argument as an item list.
    eduiitem_s *item = reinterpret_cast<eduiitem_s *>(menu);
#define EDLEVEL_SELECT_SCENE(index)                                                                                    \
    do {                                                                                                               \
        LevelEditorScene *scene = theLevelEditor.GetEdScene(index);                                                    \
        if (scene != NULL && scene->active) {                                                                          \
            item = item->previous;                                                                                     \
            scene->editable = 1;                                                                                       \
            item->highlighted = 1;                                                                                     \
        }                                                                                                              \
    } while (0)
    EDLEVEL_SELECT_SCENE(0);
    EDLEVEL_SELECT_SCENE(1);
    EDLEVEL_SELECT_SCENE(2);
    EDLEVEL_SELECT_SCENE(3);
    EDLEVEL_SELECT_SCENE(4);
    EDLEVEL_SELECT_SCENE(5);
    EDLEVEL_SELECT_SCENE(6);
    EDLEVEL_SELECT_SCENE(7);
    EDLEVEL_SELECT_SCENE(8);
    EDLEVEL_SELECT_SCENE(9);
#undef EDLEVEL_SELECT_SCENE
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

void ClassEditor::cbFileSelected(eduimenu_s *, eduiitem_s *item, u32) {
    if (item == NULL)
        return;
    theLevelEditor.current_led_file = static_cast<i16>(item->data);
    if (class_editor_create_pending != -1) {
        theClassEditor.CreateObject(class_editor_create_pending);
    } else if (theClassEditor.selected_objects.first != NULL) {
        ClassObjectListEntry *entry = theClassEditor.selected_objects.first;
        ClassObject object = {entry->ed_class, entry->object, entry->reference};
        theClassEditor.CreateObject(object);
    }
    theLevelEditor.CloseMenu();
}

__attribute__((force_align_arg_pointer)) void ClassObject::GetName(char *destination, i32 size) {
    if (object == NULL) {
        NuStrNCpy(destination, "None", size);
        return;
    }

    char name[128];
    EdMember member;
    if ((reference == NULL || !reference->GetAttributeData(object, 2, EdType_String, name, sizeof(name))) &&
        (!ed_class->FindMember(&member, object, 2, 1) ||
         !member.reference->GetAttributeData(member.object, 2, EdType_String, name, sizeof(name)))) {
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

void LevelEditor::BeginMultiLoad(variptr_u *buffer, variptr_u *buffer_end) {
    MemoryBuffer source = {buffer, buffer_end, 0, static_cast<u32>(buffer_end->addr - buffer->addr)};
    if (editor_buffer_from_front == 0) {
        editor_buffer_end = *buffer_end;
        editor_buffer_begin.addr = buffer_end->addr - 0x20000;
    }
    multi_load_active = 1;
    editor_buffer_cursor = editor_buffer_begin;
    MemoryBuffer scratch = {&editor_buffer_cursor, &editor_buffer_end, 0,
                            static_cast<u32>(editor_buffer_end.addr - editor_buffer_begin.addr)};
    theClassEditor.PreLoadInitialisation(&source, &scratch);
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
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy,
                                      const_cast<char *>("Editor List"));
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
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy,
                                      const_cast<char *>("Options"));
    if (menu == NULL)
        return;
    eduiMenuAddItem(menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdLevelSave, const_cast<char *>("Save Data")));
    theLevelEditor.settings.AddMenuItems(menu);
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
}

void LevelEditor::CreateMenu() {
    f32 x[4] __attribute__((aligned(16)));
    f32 y[4] __attribute__((aligned(16)));
    eduiGetCursorCoords(x, y);
    x[0] *= 640.0f;
    y[0] *= 448.0f;
    eduimenu_s *menu;
    if (NuKeyboard(0x0f) != 0) {
        menu = eduiMenuCreate(static_cast<i32>(x[0]), static_cast<i32>(y[0]), 100, 200,
                              reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy,
                              const_cast<char *>("Level Editor"));
        if (menu == NULL) {
            edLevelActiveMenu = NULL;
            return;
        }
        eduiMenuAddItem(
            menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdLevelEditorList, const_cast<char *>("Editors...")));
        eduiMenuAddItem(
            menu, eduiItemSelCreate(0, &EdLevelAttr, 0, 0, cbEdLevelSettingsMenu, const_cast<char *>("Options...")));
    } else {
        menu = eduiMenuCreate(static_cast<i32>(x[0]), static_cast<i32>(y[0]), 100, 200,
                              reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
        if (menu == NULL) {
            edLevelActiveMenu = NULL;
            return;
        }
        if (active_editor != NULL) {
            active_editor->AddMenuItems(menu);
        }
    }
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    edLevelActiveMenu = menu;
}

void LevelEditor::Display(ThingRenderData *) {
    f32 cursor_x __attribute__((aligned(16))) = 0.1f;
    f32 cursor_y __attribute__((aligned(16))) = 0.9f;
    if (!editors_entered) {
        return;
    }

    if (theLevelEditor.current_led_file != 0xff) {
        LevelEditorScene *scene = theLevelEditor.GetEdScene(theLevelEditor.current_led_file);
        if (scene != NULL) {
            char *name = scene->name;
            DrawInfoText(&name, 1, static_cast<i32>(640.0f * cursor_x), static_cast<i32>(448.0f * cursor_y), info_width,
                         info_height, info_colour, info_background);
        }
    }
    i32 draw_camera;
    // The original keeps the no-cursor path in the main render sequence.
    if (__builtin_expect(edmainGetCursorEnabled() == 0, 1)) {
        draw_camera = field_0x28;
    } else {
        eduiFlushInteracts();
        draw_camera = field_0x28;
    }
    if (draw_camera != 0) {
        NUCAMERA *camera = edmainGetCamera();
        const f32 distance = edcamGetDist();
        const f32 far_clip = distance + distance;
        if (global_camera.far_clip <= far_clip && global_camera.far_clip != far_clip) {
            camera->far_clip = far_clip;
        }
        edcamSet();
        EdDrawBegin(0);
        EdDrawLineCross(*reinterpret_cast<VuVec *>(background_colour), 1.0f, 0xff000000);
        EdDrawEnd();
    }
    if (active_editor != NULL) {
        active_editor->Render();
    }
    NuRndrLine3dDbgFlush();
    eduiGetCursorCoords(&cursor_x, &cursor_y);
    DrawInfoText(info_text, 32, static_cast<i32>((1.0f + cursor_x) * 640.0f),
                 static_cast<i32>((cursor_y + 1.0f) * 448.0f), info_width, info_height, info_colour, info_background);
    if (edLevelActiveMenu != NULL) {
        NuFntSet(EdLevelFnt);
        NuFntScale(EdLevelFntScale, EdLevelFntScale);
        eduiMenuRender(edLevelActiveMenu);
    }
    if (edLevelPinnedMenu != NULL) {
        NuFntSet(EdLevelFnt);
        NuFntScale(EdLevelFntScale, EdLevelFntScale);
        eduiMenuRender(edLevelPinnedMenu);
    }
    eduiSetCursorColour(static_cast<u32>(field_0x2c));
    if (edmainGetCursorEnabled()) {
        eduiRenderCursor();
    }
}

void LevelEditor::DrawInfoText(char **lines, i32 count, i32 x, i32 y, i32 available_width, i32, i32 text_colour,
                               i32 background) {
    NuQFntPushPrintMode(2);
    NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
    NUQFNT *font = system_qfont;
    NuQFntSet(font);
    const f32 line_height = NuQFntHeight(font) * 0.15625f;
    const f32 font_height = NuQFntHeight(font);
    const f32 baseline = NuQFntBaseline(font);
    i32 width = 0;
    i32 height = 0;
    for (i32 index = 0; index < count; ++index) {
        if (lines[index] != NULL) {
            const i32 line_width = static_cast<i32>(NuQFntPrintLenU(font, lines[index])) >> 4;
            if (line_width > width) {
                width = line_width;
            }
            height += static_cast<i32>(line_height);
        }
    }
    NuQFntPopCoordinateSystem();
    NuQFntPopPrintMode();
    if (height < 1) {
        return;
    }
    if (x < 0) {
        x = available_width - width - 5;
    }
    if (y < 0) {
        y = 5;
    }
    NuRndrRect2di(x << 4, y << 3, width << 4, height << 3, background, edLevel2dMtl);
    NuQFntPushPrintMode(2);
    NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
    NuQFntSet(font);
    NuQFntSetColour(font, text_colour);
    for (i32 index = 0; index < count; ++index) {
        if (lines[index] != NULL) {
            NuQFntPrintEx(font, x << 4, static_cast<i32>(font_height * 0.125f + baseline) + y * 8, 0x10, lines[index]);
            y += static_cast<i32>(line_height);
        }
    }
    NuQFntPopCoordinateSystem();
    NuQFntPopPrintMode();
}

void LevelEditor::EndMultiLoad(variptr_u *buffer, variptr_u *buffer_end) {
    MemoryBuffer source = {buffer, buffer_end, 0, static_cast<u32>(buffer_end->addr - buffer->addr)};
    if (editor_buffer_from_front == 0) {
        editor_buffer_end = *buffer_end;
        editor_buffer_begin.addr = buffer_end->addr - 0x20000;
    }
    editor_buffer_cursor = editor_buffer_begin;
    MemoryBuffer scratch = {&editor_buffer_cursor, &editor_buffer_end, 0,
                            static_cast<u32>(editor_buffer_end.addr - editor_buffer_begin.addr)};
    theClassEditor.PostLoadInitialisation(&source, &scratch);
    current_led_file = -1;
    multi_load_active = 0;
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
    if (NuStrICmp(scenes[0].name, name) == 0)
        return 0;
    if (NuStrICmp(scenes[1].name, name) == 0)
        return 1;
    if (NuStrICmp(scenes[2].name, name) == 0)
        return 2;
    if (NuStrICmp(scenes[3].name, name) == 0)
        return 3;
    if (NuStrICmp(scenes[4].name, name) == 0)
        return 4;
    if (NuStrICmp(scenes[5].name, name) == 0)
        return 5;
    if (NuStrICmp(scenes[6].name, name) == 0)
        return 6;
    if (NuStrICmp(scenes[7].name, name) == 0)
        return 7;
    if (NuStrICmp(scenes[8].name, name) == 0)
        return 8;
    // The original's unsigned carry test accepts only an exact match here.
    return (NuStrICmp(scenes[9].name, name) == 0) * 10 - 1;
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

void LevelEditor::Initalise(variptr_u &buffer, variptr_u &buffer_end, i32 from_front) {
    editor_buffer_from_front = from_front;
    if (from_front != 0) {
        editor_buffer_begin.addr = ALIGN(buffer.addr, 16);
        buffer.addr = ALIGN(buffer.addr, 16) + 0x20000;
        editor_buffer_end = buffer;
    }

    RegisterEditor(theClassEditor);
    for (BaseEditor *editor = first_editor; editor != NULL; editor = editor->next) {
        editor->Initialise(buffer, buffer_end, from_front);
    }
    active_editor = &theClassEditor;
    theEdSystem.Initalise(buffer, buffer_end, from_front);

    edLevel2dMtl = NuMtlCreate(1);
    edLevel2dMtl->diffuse_color.r = 0.5f;
    edLevel2dMtl->diffuse_color.g = 0.5f;
    edLevel2dMtl->diffuse_color.b = 0.5f;
    edLevel2dMtl->opacity = 1.0f;
    edLevel2dMtl->attribs.unknown_2_4 = 1;
    edLevel2dMtl->attribs.cull_mode = 2;
    edLevel2dMtl->attribs.z_mode = 3;
    edLevel2dMtl->attribs.alpha_mode = 1;
    NuMtlUpdate(edLevel2dMtl);

    edLevel3dMtl = NuMtlCreate(1);
    edLevel3dMtl->diffuse_color.r = 0.5f;
    edLevel3dMtl->diffuse_color.g = 0.5f;
    edLevel3dMtl->diffuse_color.b = 0.5f;
    edLevel3dMtl->opacity = 1.0f;
    edLevel3dMtl->attribs.alpha_mode = 0;
    edLevel3dMtl->attribs.unknown_2_1_2 = 2;
    edLevel3dMtl->attribs.unknown_2_4 = 1;
    NuMtlUpdate(edLevel3dMtl);

    edLevelTexMtl = NuMtlCreate(1);
    edLevelTexMtl->diffuse_color.r = 0.5f;
    edLevelTexMtl->diffuse_color.g = 0.5f;
    edLevelTexMtl->diffuse_color.b = 0.5f;
    edLevelTexMtl->opacity = 1.0f;
    edLevelTexMtl->attribs.alpha_mode = 0;
    edLevelTexMtl->attribs.cull_mode = 2;
    edLevelTexMtl->attribs.z_mode = 3;
    edLevelTexMtl->attribs.unknown_2_4 = 1;
    NuMtlUpdate(edLevelTexMtl);

    NuFntSet(EdLevelFnt);
    NuFntScale(EdLevelFntScale, EdLevelFntScale);
    EdDrawBegin(0);
    EdDrawEnd();
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

i32 LevelEditor::Load(char *filename, variptr_u *buffer, variptr_u *buffer_end, i32 flags) {
    MemoryBuffer source = {buffer, buffer_end, 0, static_cast<u32>(buffer_end->addr - buffer->addr)};
    if (editor_buffer_from_front == 0) {
        editor_buffer_end = *buffer_end;
        editor_buffer_begin.addr = buffer_end->addr - 0x20000;
    }

    char *basename = NuStrRChr(filename, '/');
    i32 basename_length = NuStrLen(basename + 1);
    char scene_name[32];
    char directory[128];
    NuStrNCpy(scene_name, basename + 1, basename_length - 3);
    NuStrNCpy(directory, filename, NuStrLen(filename) - basename_length);
    Placeable::CurrentLedFile = static_cast<i16>(FindSceneId(scene_name));
    if (Placeable::CurrentLedFile != -1) {
        NuStrCpy(scenes[Placeable::CurrentLedFile].directory, directory);
        current_led_file = Placeable::CurrentLedFile;
    } else {
        current_led_file = -1;
    }

    editor_buffer_cursor = editor_buffer_begin;
    MemoryBuffer scratch = {&editor_buffer_cursor, &editor_buffer_end, 0,
                            static_cast<u32>(editor_buffer_end.addr - editor_buffer_begin.addr)};
    SetSaveFilename(filename);
    if (!multi_load_active) {
        theClassEditor.PreLoadInitialisation(&source, &scratch);
    }

    i32 result = -1;
    void *file_data = editor_buffer_cursor.void_ptr;
    i32 file_size = NuFileLoadBuffer(save_filename, file_data, scratch.remaining);
    if (file_size > 0) {
        if (static_cast<u32>(file_size) < editor_buffer_end.addr - editor_buffer_cursor.addr) {
            editor_buffer_cursor.addr += file_size;
            scratch.used += file_size;
            scratch.remaining -= file_size;
        }
        NUFILE file = NuMemFileOpen(file_data, file_size, NUFILE_READ);
        if (file) {
            EdFileInputStream stream(&source, &scratch);
            if (flags) {
                stream.flags = 0x200000;
            }
            stream.Open(file, 4);
            i32 loaded = ReadStream(stream);
            stream.file = 0;
            NuFileClose(file);
            if (loaded) {
                result = Placeable::CurrentLedFile;
                scenes[result].editable = 1;
                Placeable::CurrentLedFile = -1;
            }
        }
    }
    if (!multi_load_active) {
        theClassEditor.PostLoadInitialisation(&source, &scratch);
    }
    return result;
}

i32 LevelEditor::LoadState(variptr_u *buffer, variptr_u *buffer_end, variptr_u *scratch_begin, variptr_u *scratch_end,
                           variptr_u *file_begin, variptr_u *file_end) {
    reserved_0x298 = 1;
    MemoryBuffer source = {buffer, buffer_end, 0, 0};
    MemoryBuffer scratch = {scratch_begin, scratch_end, 0, static_cast<u32>(scratch_end->addr - scratch_begin->addr)};
    if (!multi_load_active) {
        theClassEditor.PreLoadInitialisation(&source, &scratch);
    }
    i32 result = 0;
    NUFILE file = NuMemFileOpen(file_begin->void_ptr, file_end->addr - file_begin->addr, NUFILE_READ);
    if (file) {
        EdFileInputStream stream(&source, &scratch);
        stream.Open(file, 4);
        stream.flags = 0x400000;
        result = ReadStream(stream);
        stream.file = 0;
        NuFileClose(file);
    }
    if (!multi_load_active) {
        theClassEditor.PostLoadInitialisation(&source, &scratch);
    }
    reserved_0x298 = 0;
    return result;
}

void LevelEditor::ProcessEvenWhenPaused(ThingProcessData *data) {
    if (!editors_entered) {
        return;
    }
    f32 delta_time = data->t;
    nupad_s *pad = data->pads[0];
    text_length = 0;
    memset(info_text, 0, sizeof(info_text));
    memset(reserved_0xef8, 0, 0x80);
    memset(reserved_0xef8 + 0x80, 0, 0x80);
    field_0x2c = -1;
    editable_scene_count = 0;
    if (scenes[0].editable)
        ++editable_scene_count;
    if (scenes[1].editable)
        ++editable_scene_count;
    if (scenes[2].editable)
        ++editable_scene_count;
    if (scenes[3].editable)
        ++editable_scene_count;
    if (scenes[4].editable)
        ++editable_scene_count;
    if (scenes[5].editable)
        ++editable_scene_count;
    if (scenes[6].editable)
        ++editable_scene_count;
    if (scenes[7].editable)
        ++editable_scene_count;
    if (scenes[8].editable)
        ++editable_scene_count;
    if (scenes[9].editable)
        ++editable_scene_count;
    if (edLevelActiveMenu == NULL && field_0x38 != 0 && input.GetPress(4) != 0.0f) {
        eduiSetCameraEnabled(0);
        eduiSetDefaultActiveMenu(eduiGetActiveMenu());
        theLevelEditor.CreateMenu();
    }
    if (edLevelDestroyActiveMenu != 0) {
        eduiMenuDestroy(edLevelActiveMenu);
        edLevelActiveMenu = NULL;
        edLevelDestroyActiveMenu = 0;
    }
    if (edLevelDestroyThisMenu != NULL) {
        eduiMenuDestroy(edLevelDestroyThisMenu);
        edLevelDestroyThisMenu = NULL;
    }
    if (edLevelDestroyThisMenu2 != NULL) {
        eduiMenuDestroy(edLevelDestroyThisMenu2);
        edLevelDestroyThisMenu2 = NULL;
    }
    if (edLevelNextMenu != NULL) {
        if (edLevelActiveMenu != NULL) {
            eduiMenuDestroy(edLevelActiveMenu);
        }
        edLevelActiveMenu = edLevelNextMenu;
        edLevelNextMenu = NULL;
    }
    nucamera_s *camera = &global_camera;
    if (field_0x28 != 0) {
        camera = edmainGetCamera();
    }
    input.Update(camera, pad, delta_time, thePropertyTool.HasActiveMenu());
    if (edmainGetCursorEnabled()) {
        eduiProcessCursor(0.0f, pad);
    }
    if (field_0x28 != 0 && eduiGetCameraEnabled()) {
        if (edlevel_mouseandkeyboard == 0) {
            edcamMove(pad);
        } else {
            edcamMove(NULL);
        }
        edcamGetPosAng(reinterpret_cast<NUVEC *>(background_colour), &field_0x20, &field_0x24);
    }
    if (input.GetPress(17) != 0.0f) {
        overlay_alpha += 0.1f;
        eduiSetFontScale(overlay_alpha, overlay_alpha);
    }
    if (input.GetPress(18) != 0.0f && static_cast<f32>(EdLevelFntScale) > 0.1f) {
        overlay_alpha -= 0.1f;
        eduiSetFontScale(overlay_alpha, overlay_alpha);
    }
    if (edLevelActiveMenu != NULL) {
        NuFntSet(EdLevelFnt);
        NuFntScale(EdLevelFntScale, EdLevelFntScale);
        if (eduiMenuProcess(edLevelActiveMenu, delta_time, pad) != 0) {
            input.Clear(3);
        } else if (edLevelActiveMenu != edLevelPinnedMenu) {
            if (input.GetPress(3) != 0.0f) {
                eduiSetCameraEnabled(1);
                eduiMenuDestroy(edLevelActiveMenu);
                edLevelActiveMenu = NULL;
                eduiSetDefaultActiveMenu(NULL);
            }
            if (input.GetPress(4) != 0.0f) {
                eduiMenuDestroy(edLevelActiveMenu);
                edLevelActiveMenu = NULL;
                theLevelEditor.CreateMenu();
            }
        }
    } else if (edLevelPinnedMenu != NULL && !thePropertyTool.HasActiveMenu()) {
        NuFntSet(EdLevelFnt);
        NuFntScale(EdLevelFntScale, EdLevelFntScale);
        if (eduiMenuProcess(edLevelPinnedMenu, delta_time, pad) != 0) {
            return;
        }
    }
    if (active_editor != NULL) {
        active_editor->Process(input);
    }
    if (input.GetPress(23) != 0.0f) {
        theLevelEditor.Save();
    }
}

i32 LevelEditor::ReadStream(EdFileInputStream &stream) {
    if (stream.BeginBlock("FileInfo") != NULL) {
        stream.SerialiseBuffer(&file_version, sizeof(file_version), 1);
        stream.EndBlock();
    }
    if (stream.BeginBlock("Settings") != NULL) {
        settings.Serialise(stream);
        stream.EndBlock();
    }
    if (stream.BeginBlock("Editors") != NULL) {
        i32 count;
        stream.SerialiseBuffer(&count, sizeof(count), 1);
        for (i32 index = 0; index < count; ++index) {
            char const *name = stream.BeginBlock(NULL);
            for (BaseEditor *editor = first_editor; editor != NULL; editor = editor->next) {
                if (NuStrICmp(editor->GetName(), name) == 0) {
                    editor->Serialise(stream);
                    break;
                }
            }
            stream.EndBlock();
        }
        stream.EndBlock();
    }
    return 1;
}

void LevelEditor::Reset() {
    reset_pending = 0;
}

i32 __attribute__((optimize("no-partial-inlining"))) LevelEditor::Save() {
    i32 saved_any = 0;
    if (active) {
#define EDLEVEL_SAVE_SCENE(index)                                                                                      \
    do {                                                                                                               \
        LevelEditorScene &scene = scenes[index];                                                                       \
        theClassEditor.PreSaveInitialisation();                                                                        \
        sprintf(save_filename, "%s/%s.led", scene.directory, scene.name);                                              \
        if (scene.editable) {                                                                                          \
            editor_buffer_cursor = editor_buffer_begin;                                                                \
            NUFILE memory_file = NuMemFileOpen(editor_buffer_begin.void_ptr,                                           \
                                               editor_buffer_end.addr - editor_buffer_begin.addr, NUFILE_WRITE);       \
            if (memory_file) {                                                                                         \
                EdFileOutputStream stream;                                                                             \
                stream.Open(memory_file, 4);                                                                           \
                stream.unknown_10 = index;                                                                             \
                WriteStream(stream);                                                                                   \
                i32 size = NuFilePos(memory_file);                                                                     \
                stream.file = 0;                                                                                       \
                NuFileClose(memory_file);                                                                              \
                NUFILE output_file = NuFileOpen(save_filename, NUFILE_WRITE);                                          \
                if (!output_file) {                                                                                    \
                    scene.saved = 0;                                                                                   \
                } else {                                                                                               \
                    NuFileWrite(output_file, editor_buffer_begin.void_ptr, size);                                      \
                    NuFileClose(output_file);                                                                          \
                    scene.saved = 1;                                                                                   \
                    saved_any = 1;                                                                                     \
                }                                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)
        EDLEVEL_SAVE_SCENE(0);
        EDLEVEL_SAVE_SCENE(1);
        EDLEVEL_SAVE_SCENE(2);
        EDLEVEL_SAVE_SCENE(3);
        EDLEVEL_SAVE_SCENE(4);
        EDLEVEL_SAVE_SCENE(5);
        EDLEVEL_SAVE_SCENE(6);
        EDLEVEL_SAVE_SCENE(7);
        EDLEVEL_SAVE_SCENE(8);
        EDLEVEL_SAVE_SCENE(9);
#undef EDLEVEL_SAVE_SCENE
        theClassEditor.PostSaveInitialisation();
    }
    return saved_any;
}

i32 LevelEditor::SaveState(i32 scene_index, variptr_u *buffer, variptr_u *buffer_end) {
    i32 size = buffer_end->addr - buffer->addr;
    NUFILE file = NuMemFileOpen(buffer->void_ptr, size, NUFILE_WRITE);
    if (file) {
        reserved_0x298 = 1;
        EdFileOutputStream stream;
        stream.Open(file, 4);
        stream.flags = 0x400000;
        stream.unknown_10 = scene_index;
        WriteStream(stream);
        size = NuFilePos(file);
        stream.file = 0;
        NuFileClose(file);
    }
    reserved_0x298 = 0;
    return size;
}

i32 LevelEditor::SaveState(variptr_u *buffer, variptr_u *buffer_end) {
    i32 result = 0;
    NUFILE file = NuMemFileOpen(buffer->void_ptr, buffer_end->addr - buffer->addr, NUFILE_WRITE);
    if (file) {
        reserved_0x298 = 1;
        EdFileOutputStream stream;
        stream.Open(file, 4);
        stream.flags = 0x400000;
        result = WriteStream(stream);
        NuFilePos(file);
        stream.file = 0;
        NuFileClose(file);
    }
    reserved_0x298 = 0;
    return result;
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

i32 LevelEditor::WriteStream(EdFileOutputStream &stream) {
    ++file_version;
    stream.BeginBlock("FileInfo");
    stream.SerialiseBuffer(&file_version, sizeof(file_version), 1);
    stream.EndBlock();
    stream.BeginBlock("Settings");
    settings.Serialise(stream);
    stream.EndBlock();
    stream.BeginBlock("Editors");
    i32 count = editor_count;
    stream.SerialiseBuffer(&count, sizeof(count), 1);
    for (BaseEditor *editor = first_editor; editor != NULL; editor = editor->next) {
        stream.BeginBlock(editor->GetName());
        editor->Serialise(stream);
        stream.EndBlock();
    }
    stream.EndBlock();
    return 1;
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

void PropertyTool::AddPropertyMenuItems(eduimenu_s *menu, EdClass *ed_class, void *object, eduiitem_s *parent) {
    for (EdRef *member = ed_class->members; member != NULL; member = member->next) {
        if (member->attributes < 0) {
            eduiitem_s *expander = eduiItemExpanderCreate(0, &EdLevelAttr, 0, member->name);
            if (parent != NULL) {
                eduiItemExpanderAddChild(static_cast<edui_expander_s *>(parent), expander);
            } else {
                eduiMenuAddItem(menu, expander);
            }
            EdClass *child_class = theRegistry.GetClass(member->type_id);
            AddPropertyMenuItems(menu, child_class, member->GetMemberObject(object), expander);
        } else if (member->control != NULL) {
            eduiitem_s *last = menu->last;
            member->control->AddMenuItem(menu, member, object);
            if (parent != NULL) {
                eduiitem_s *item = last != NULL ? last->next : menu->first;
                while (item != NULL) {
                    eduiitem_s *next = item->next;
                    eduiMenuRemoveItem(menu, item);
                    eduiItemExpanderAddChild(static_cast<edui_expander_s *>(parent), item);
                    item = next;
                }
            }
        }
    }
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
    if (position != NULL && position->order < -1) {
        do {
            position = position->next;
        } while (position != NULL && position->order < -1);
    }
    if (position == NULL) {
        menu->previous = last_menu;
        if (last_menu != NULL) {
            last_menu->next = menu;
        }
        last_menu = menu;
        if (active_menu == NULL) {
            active_menu = menu;
        }
    } else {
        menu->next = position;
        menu->previous = position->previous;
        if (position->previous != NULL) {
            position->previous->next = menu;
        } else {
            active_menu = menu;
        }
        position->previous = menu;
    }
    ++menu_count;
}

PropertyMenu *PropertyTool::CreatePropertyMenu(ClassObject &object) {
    PropertyMenu *property_menu = new (theMemoryManager.AllocPool(sizeof(PropertyMenu), 1)) PropertyMenu();
    PropertyMenuMetrics metrics __attribute__((aligned(16))) = ediGetMenuStartMetrics();
    char name[64];
    char title[128];
    if (!get_class_object_attribute(object.ed_class, object.object, object.reference, 2, EdType_String, name,
                                    sizeof(name))) {
        NuStrCpy(name, const_cast<char *>("no name"));
    }
    sprintf(title, "%s - %s", object.ed_class->name, name);
    eduimenu_s *menu = eduiMenuCreate(metrics.x, metrics.y, metrics.width, metrics.height,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), NULL, title);
    if (menu != NULL) {
        AddPropertyMenuItems(menu, object.ed_class, object.object, NULL);
        menu->selected = menu->first;
        eduiMenuFitWidth(menu, 5);
    }
    eduiMenuSetAttr(menu, &selected_attr.background);
    property_menu->menu = menu;
    property_menu->objects[0] = object;
    property_menu->object_count = 1;
    property_menu->control = NULL;
    property_menu->SelectAttr(theClassEditor.manipulator != NULL ? theClassEditor.manipulator->selected_attribute : 0);
    return property_menu;
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

void PropertyTool::GetClassName(EdRef *reference, char *name) {
    if (show_type_names != 0) {
        EdClass *ed_class = theRegistry.GetClass(reference->type_id);
        if (ed_class != NULL) {
            sprintf(name, "%s %s", ed_class->name, reference->name);
            return;
        }
    }
    NuStrCpy(name, reference->name);
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

void PropertyTool::GetTypeName(EdRef *reference, char *name) {
    if (show_type_names != 0) {
        EdType *type = theRegistry.GetType(reference->type_id);
        if (type != NULL) {
            sprintf(name, "%s %s", type->name, reference->name);
            return;
        }
    }
    NuStrCpy(name, reference->name);
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

i32 PropertyTool::ProcessMenu(EdInputContext &input) {
    PropertyMenuList rebuilt = {};
    i32 order = 0;
    for (PropertyMenu *menu = active_menu; menu != NULL; menu = menu->next) {
        menu->order = order++;
    }
    for (ClassObjectListEntry *entry = theClassEditor.selected_objects.first; entry != NULL; entry = entry->next) {
        PropertyMenu *menu = FindItemMenu(active_menu, reinterpret_cast<ClassItem *>(entry));
        if (menu != NULL) {
            menu->ClearObjecs();
            menu->AddObject(*reinterpret_cast<ClassObject *>(&entry->ed_class));
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
        } else {
            menu = FindItemMenu(rebuilt.first, reinterpret_cast<ClassItem *>(entry));
            if (menu != NULL) {
                menu->AddObject(*reinterpret_cast<ClassObject *>(&entry->ed_class));
                continue;
            }
            for (PropertyMenu *other = active_menu; other != NULL; other = other->next) {
                ediMenuStoreMetrics(other->menu);
            }
            menu = RetrievePropertyMenu(reinterpret_cast<ClassObject *>(&entry->ed_class),
                                        reinterpret_cast<PropertyMenuList *>(&active_menu));
            ediMenuRetrieveMetrics(menu->menu);
            menu->order = -1;
        }

        PropertyMenu *position = rebuilt.first;
        while (position != NULL && position->order <= menu->order) {
            position = position->next;
        }
        if (position != NULL) {
            menu->next = position;
            menu->previous = position->previous;
            if (position->previous != NULL) {
                position->previous->next = menu;
            } else {
                rebuilt.first = menu;
            }
            position->previous = menu;
            ++rebuilt.count;
            continue;
        }
        menu->next = NULL;
        menu->previous = rebuilt.last;
        if (rebuilt.last != NULL) {
            rebuilt.last->next = menu;
        } else {
            rebuilt.first = menu;
        }
        rebuilt.last = menu;
        ++rebuilt.count;
    }
    while (active_menu != NULL) {
        PropertyMenu *menu = active_menu;
        PropertyMenu *next = menu->next;
        if (next != NULL) {
            next->previous = menu->previous;
        } else {
            last_menu = menu->previous;
        }
        if (menu->previous != NULL) {
            menu->previous->next = next;
        } else {
            active_menu = next;
        }
        menu->next = NULL;
        menu->previous = NULL;
        --menu_count;
        menu->Destroy();
        theMemoryManager.FreePool(menu, sizeof(PropertyMenu));
    }
    active_menu = rebuilt.first;
    last_menu = rebuilt.last;
    menu_count = rebuilt.count;
    if (edLevelActiveMenu != NULL) {
        return 0;
    }
    EdControl::Input = &input;
    PropertyMenu *active = GetActiveMenu(active_menu);
    if (active != NULL) {
        RefreshMenuControls(active);
        if (eduiMenuProcess(active->menu, input.delta_time, input.pad) != 0) {
            return 1;
        }
    }
    for (PropertyMenu *menu = active_menu; menu != NULL; menu = menu->next) {
        if (menu == active) {
            continue;
        }
        RefreshMenuControls(menu);
        if (eduiMenuProcess(menu->menu, input.delta_time, input.pad) != 0) {
            return 1;
        }
    }
    if ((input.pad->digital_buttons_pressed & 0x100) != 0) {
        ToggleActiveMenu();
    }
    EdControl::Input = NULL;
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

void PropertyTool::RenderMenu(PropertyMenu *property_menu) {
    if (property_menu->order == -1)
        AutoLocateMenu(property_menu);
    eduimenu_s *menu = property_menu->menu;
    VuVec start __attribute__((aligned(16)));
    VuVec end __attribute__((aligned(16)));
    NuCameraCalcRay((static_cast<f32>(menu->x) + static_cast<f32>(menu->width) * 0.5f) / 640.0f,
                    (static_cast<f32>(menu->y) + static_cast<f32>(menu->height) * 0.125f) / 448.0f, &start.xyz,
                    &end.xyz, NULL);
    ClassObject &object = property_menu->objects[0];
    if (object.reference == NULL || !object.reference->GetAttributeData(object.object, 8, EdType_VuVec, &end, 0)) {
        EdMember member;
        if (object.ed_class->FindMember(&member, object.object, 8, 1))
            member.reference->GetAttributeData(member.object, 8, EdType_VuVec, &end, 0);
    }
    EdDrawBegin(0);
    EdDrawLineSegment(start, end, static_cast<i32>(reinterpret_cast<usize>(&selected_attr)));
    EdDrawEnd();
    if (property_menu->menu != NULL)
        eduiMenuRender(menu);
    if (menu->selected != NULL) {
        EdControl *control = static_cast<EdControl *>(menu->selected->data_ptr);
        if (control != NULL)
            control->Render();
    }
}

PropertyMenu *PropertyTool::RetrievePropertyMenu(ClassObject *object, PropertyMenuList *) {
    return CreatePropertyMenu(*object);
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
#if defined(__i386__) && defined(__SSE2__)
    __m128i metrics = _mm_load_si128(reinterpret_cast<const __m128i *>(&menu_startmetrics));
    _mm_storeu_si128(reinterpret_cast<__m128i *>(&menu->x), metrics);
#else
    memcpy(&menu->x, &menu_startmetrics, sizeof(menu_startmetrics));
#endif
}

void PropertyTool::ediMenuStoreMetrics(eduimenu_s *menu) {
    menu_startmetrics.x = menu->x > 0 ? menu->x : 20;
    menu_startmetrics.y = menu->y > 0 ? menu->y : 5;
    menu_startmetrics.width = menu->width;
    menu_startmetrics.height = menu->height;
}

i32 ClassObjectList::GetAveragePosition(VuVec &average) {
    average = VuVec(0.0f, 0.0f, 0.0f, 1.0f);
    i32 position_count = 0;
    for (ClassObjectListEntry *entry = first; entry != NULL; entry = entry->next) {
        EdMember member;
        // The original's local vector is 16-byte aligned.
        VuVec position __attribute__((aligned(16)));
        i32 position_type = EdType_VuVec;
        bool found = entry->reference != NULL &&
                     entry->reference->GetAttributeData(entry->object, 8, position_type, &position, 0) != 0;
        if (!found) {
            found = entry->ed_class->FindMember(&member, entry->object, 8, 1) != 0 &&
                    member.reference->GetAttributeData(member.object, 8, position_type, &position, 0) != 0;
        }
        if (found) {
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
    average = VuVec(0.0f, 0.0f, 0.0f, 1.0f);
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

void DumpAreaData(i32 mode, i32) {
    if (big_icon_scene != NULL) {
        NuGScnRemove(big_icon_scene);
    }
    big_icon_scene = NULL;
    if (area_scene != NULL) {
        NuGScnRemove(area_scene);
    }
    area_scene = NULL;
    if (vehicle_scene != NULL) {
        NuGScnRemove(vehicle_scene);
    }
    vehicle_scene = NULL;
    if (Customiser_AccessoriesLoaded == 2) {
        Customiser_RestoreModelTextureIDs(reinterpret_cast<CUSTOMISER *>(Game_Customiser));
    } else if (Customiser_AccessoriesLoaded == 1) {
        Customiser_DumpAccessories(reinterpret_cast<CUSTOMISER *>(Game_Customiser));
    }
    Customiser_AccessoriesLoaded = 0;
    IconScenes_Dump();
    CharScenes_AreaDump();
    APIDumpCharacterModels(mode);
    Particles_DumpAreaPage();
}

void cbEdLevelSave(eduimenu_s *parent, eduiitem_s *item, u32) {
    theLevelEditor.Save();
    eduimenu_s *menu = eduiMenuCreate(item->x + parent->width, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy,
                                      const_cast<char *>("Save File"));
    char message[128];
    if (!theLevelEditor.active) {
        if (menu) {
            strcpy(message, "Saving not allowed in Debug Mode - change to Edit Mode");
            eduiMenuAddItem(menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect, message));
            eduiMenuFitWidth(menu, 5);
            eduiMenuFitOnScreen(menu, 1);
        }
    } else {
        for (i32 scene_index = 0; scene_index < theLevelEditor.reset_pending; ++scene_index) {
            LevelEditorScene *scene = theLevelEditor.GetEdScene(scene_index);
            if (!scene) {
                continue;
            }
            if (scene->active && scene->editable && scene->saved) {
                scene->saved = 0;
                if (menu) {
                    sprintf(message, "%s successfully saved", scene->name);
                    eduiMenuAddItem(menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect, message));
                    eduiMenuFitWidth(menu, 5);
                    eduiMenuFitOnScreen(menu, 1);
                }
            } else if (menu && scene->active && scene->editable) {
                sprintf(message, "%s : Save Failed, have you got the lock?", scene->name);
                eduiMenuAddItem(menu, eduiItemSelCreate(1, &EdLevelAttr, 0, 0, cbEdLevelDestroyOnSelect, message));
                eduiMenuFitWidth(menu, 5);
                eduiMenuFitOnScreen(menu, 1);
            }
        }
    }
    eduiMenuFitWidth(menu, 5);
    eduiMenuFitOnScreen(menu, 1);
    eduiMenuAttach(parent, menu);
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

void cbEdLevelSetText(eduimenu_s *, eduiitem_s *item, u32) {
    strcpy(static_cast<char *>(item->data_ptr), static_cast<edui_textpicker_s *>(item)->value);
}

void cbEdLevelToggleInt(eduimenu_s *, eduiitem_s *item, u32) {
    i32 *value = static_cast<i32 *>(item->data_ptr);
    *value ^= 1;
    item->highlighted = *value;
}

void cbCEDeleteConfirmed(eduimenu_s *menu, eduiitem_s *item, u32 buttons) {
    theClassEditor.DestroySelectedObjectsNow();
    cbEdLevelDestroyOnSelect(menu, item, buttons);
}

void LightEverythingInEditor(void *light_set) {
    rtlProcessLights(light_set, FRAMETIME);
    GameObject_s *object = Obj;
    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index, ++object) {
        if ((object->apiobj.field_0x1f8 & 0x1001) == 0x1001) {
            object->field_0xefc |= 0x80;
            LightGameObject(object, light_set);
        }
    }
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
    i32 count __attribute__((aligned(16)));
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
    } else if (stream.mode == 1) {
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
    EdRef *member = members;
    if (member == NULL)
        return NULL;
    if (recursive == 0) {
        do {
            if (member->attributes >= 0 && (member->attributes & attributes) != 0)
                return member;
            member = member->next;
        } while (member != NULL);
        return NULL;
    }
    do {
        if (member->attributes >= 0) {
            if ((member->attributes & attributes) != 0)
                return member;
        } else {
            EdRef *reference = theRegistry.GetClass(member->type_id)->FindTypeRef(attributes, 1);
            if (reference != NULL)
                return reference;
        }
        member = member->next;
    } while (member != NULL);
    return NULL;
}

i32 EdClass::FindMember(EdMember *result, void *object, i32 attributes, i32 recursive) {
    EdRef *member = members;
    if (member == NULL)
        return 0;
    if (recursive == 0) {
        do {
            if (member->attributes >= 0 && (member->attributes & attributes) != 0)
                goto found;
            member = member->next;
        } while (member != NULL);
        return 0;
    }
    do {
        if (member->attributes < 0) {
            EdClass *member_class = theRegistry.GetClass(member->type_id);
            void *member_object = member->GetMemberObject(object);
            if (member_class->FindMember(result, member_object, attributes, 1) != 0)
                return 1;
        } else if ((member->attributes & attributes) != 0)
            goto found;
        member = member->next;
    } while (member != NULL);
    return 0;
found:
    result->object = object;
    result->reference = member;
    return 1;
}

void *EdClass::FindObject(char *object_name) {
    void *object = interface->vtable->get_next_object(interface, NULL);
    while (object != NULL) {
        EdMember member;
        i32 string_type = EdType_String;
        if (FindMember(&member, object, 2, 1) != 0) {
            // The original aligns the stack for this name buffer.
            char name_buffer[256] __attribute__((aligned(16)));
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

void EditorSettings::Serialise(EdStream &stream) {
    stream.SerialiseBuffer(&cursor_radius, sizeof(cursor_radius), 1);
    stream.SerialiseBuffer(&snap_terrain, sizeof(snap_terrain), 1);
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

__attribute__((force_align_arg_pointer)) void ClassEditor::UpdateSnapRay(VuVec &position) {
    if (snap_mode == 1) {
        snap_ray.x = 0.0f;
        snap_ray.y = -1000.0f;
        snap_ray.z = 0.0f;
    } else if (snap_mode == 2) {
#if defined(__i386__) && defined(__SSE__)
        __m128 lanes = _mm_loadl_pi(_mm_setzero_ps(), reinterpret_cast<const __m64 *>(&position));
        lanes = _mm_loadh_pi(lanes, reinterpret_cast<const __m64 *>(&position.z));
        _mm_storel_pi(reinterpret_cast<__m64 *>(&snap_ray), lanes);
        _mm_storeh_pi(reinterpret_cast<__m64 *>(&snap_ray.z), lanes);
#else
        snap_ray = position;
#endif
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

i32 PropertyMenu::ContainsObject(void *object) {
    for (i32 i = 0; i < object_count; ++i) {
        if (objects[i].object == object) {
            return 1;
        }
    }
    return 0;
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
