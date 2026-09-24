#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nustring.h"

#include <new>

i16 Placeable::CurrentLedFile;
PlaceableHelper thePlaceableHelper;
extern ClassEditor theClassEditor;
extern eduiiattr_s EdLevelAttr;
extern i32 EdLevelFnt;
void cbEdLevelDestroy(eduimenu_s *, eduimenu_s *);
static EdStringControl *edObjectNameControl;

template <typename Ref>
static __attribute__((always_inline)) void add_placeable_reference(EdClass *object_class, char *type, char *name,
                                                                   i32 offset, i32 size, i32 attributes,
                                                                   EdControl *control = NULL) {
    void *memory = theMemoryManager.AllocPool(sizeof(Ref), 1);
    Ref *reference = new (memory) Ref(type, name, offset, size, attributes, control, 0);
    object_class->AddType(reference);
}

Placeable *PlaceableHelper::Find(char *name) {
    for (i32 index = 0; index < object_type_count; ++index) {
        Placeable *object = object_types[index].interface->Find(name);
        if (object != NULL)
            return object;
    }
    return NULL;
}

i32 PlaceableHelper::Find(char *name, Placeable **objects, i32 capacity) {
    i32 count = 0;
    for (i32 index = 0; index < object_type_count; ++index)
        count += object_types[index].interface->Find(name, objects + count, capacity - count);
    return count;
}

Placeable *PlaceableHelper::FindObject(char *name) {
    for (i32 index = 0; index < object_type_count; ++index) {
        EdClassInterface *interface = reinterpret_cast<EdClassInterface *>(object_types[index].interface);
        for (Placeable *object = static_cast<Placeable *>(interface->vtable->get_next_object(interface, NULL));
             object != NULL; object = static_cast<Placeable *>(interface->vtable->get_next_object(interface, object))) {
            if (NuStrICmp(object->GetName(), name) == 0)
                return object;
        }
    }
    return NULL;
}

void *PlaceableHelper::GetNextObject(void *object) {
    if (object == NULL)
        iteration_object_type = 0;
    while (iteration_object_type < object_type_count) {
        EdClassInterface *interface =
            reinterpret_cast<EdClassInterface *>(object_types[iteration_object_type].interface);
        void *next = interface->vtable->get_next_object(interface, object);
        if (next != NULL)
            return next;
        ++iteration_object_type;
        object = NULL;
    }
    return NULL;
}

void *PlaceableHelper::GetNextObject(void *object, i32 (*filter)(void *)) {
    for (object = GetNextObject(object); object != NULL; object = GetNextObject(object)) {
        if (filter(object) != 0)
            return object;
    }
    return NULL;
}

void PlaceableHelper::Initialise() {
    EdClass *object_class = theRegistry.RegisterClass("Placeable", NULL, 0);
    if (object_class == NULL)
        return;
    add_placeable_reference<EdRefPlaceable>(object_class, "Short", "LEDFile", static_cast<i32>(0x80000001), 0,
                                            0x10000100);
    add_placeable_reference<EdRefPlaceable>(object_class, "Int", "Attributes", static_cast<i32>(0x80000002), 0, 1);
    void *name_storage = theMemoryManager.AllocPool(sizeof(EdStringControl), 1);
    EdStringControl *name_control = new (name_storage) EdStringControl();
    add_placeable_reference<EdRefPlaceable>(object_class, "String", "Name", static_cast<i32>(0x80000003), 250, 2,
                                            name_control);
    void *params_storage = theMemoryManager.AllocPool(sizeof(EdStringControl), 1);
    EdStringControl *params_control = new (params_storage) EdStringControl();
    add_placeable_reference<EdRefPlaceable>(object_class, "String", "Params", static_cast<i32>(0x80000005), 250, 0,
                                            params_control);
    void *matrix_storage = theMemoryManager.AllocPool(sizeof(EdMatrixControl), 1);
    EdMatrixControl *matrix_control = new (matrix_storage) EdMatrixControl();
    add_placeable_reference<EdRefPlaceable>(object_class, "VuMtx", "Mtx", static_cast<i32>(0x80000006), 0, 0x38,
                                            matrix_control);
}

i32 PlaceableHelper::IsEditorObject(ClassObject &object) {
    if (object.object != NULL) {
        for (i32 index = 0; index < object_type_count; ++index) {
            EdClassInterface *interface = reinterpret_cast<EdClassInterface *>(object_types[index].interface);
            if (interface->object_class == object.ed_class)
                return 1;
        }
    }
    return 0;
}

PlaceableHelper::PlaceableHelper() {
    object_type_count = 0;
}

void PlaceableHelper::RegisterObjectType(char *name, PlaceableInterface *interface) {
    i32 index = object_type_count++;
    object_types[index].name = name;
    object_types[index].interface = interface;
    *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(interface) + 8) = object_type_count;
}

i32 PlaceableInterface::DebugOutputObjects() {
    EdClassInterface *interface = reinterpret_cast<EdClassInterface *>(this);
    i32 count = 0;
    for (void *object = interface->vtable->get_next_object(interface, NULL); object != NULL;
         object = interface->vtable->get_next_object(interface, object))
        ++count;
    return count;
}

Placeable *PlaceableInterface::Find(char *name) {
    EdClassInterface *interface = reinterpret_cast<EdClassInterface *>(this);
    for (Placeable *object = static_cast<Placeable *>(interface->vtable->get_next_object(interface, NULL));
         object != NULL; object = static_cast<Placeable *>(interface->vtable->get_next_object(interface, object))) {
        if (NuStrICmp(name, object->GetName()) == 0)
            return object;
    }
    return NULL;
}

i32 PlaceableInterface::Find(char *name, Placeable **objects, i32 capacity) {
    EdClassInterface *interface = reinterpret_cast<EdClassInterface *>(this);
    i32 count = 0;
    Placeable *object = static_cast<Placeable *>(interface->vtable->get_next_object(interface, NULL));
    while (object != NULL && count < capacity) {
        if (NuStrIStr(const_cast<char *>(object->GetName()), name) != NULL)
            objects[count++] = object;
        object = static_cast<Placeable *>(interface->vtable->get_next_object(interface, object));
    }
    return count;
}

void PlaceableNameControl::AddMenuItem(eduimenu_s *menu, EdRef *reference, void *object) {
    PlaceableNameControl *control =
        new (theMemoryManager.AllocPool(sizeof(PlaceableNameControl), 1)) PlaceableNameControl();
    if (control == NULL)
        return;
    control->reference = reference;
    control->object = object;
    char value[128];
    control->GetVal(value, sizeof(value));
    control->item = eduiItemPropCreate(reinterpret_cast<usize>(control), &EdLevelAttr, EdControl::cbSelected, cbChanged,
                                       cbButton, 1, reference->name, value);
    eduiMenuAddItem(menu, control->item);
}

PlaceableNameControl::PlaceableNameControl() : reserved_10(0) {
}

PlaceableNameControl::~PlaceableNameControl() {
}

inline void PlaceableNameControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(PlaceableNameControl));
}

void PlaceableNameControl::Process(EdInputContext &input) {
    if (input.GetHold(0x16) != 0.0f) {
        ClassObject object = theClassEditor.current_object;
        if (thePlaceableHelper.IsEditorObject(object) == 0) {
            theClassEditor.field_3c = static_cast<i32>(0xff000080);
        } else {
            theClassEditor.field_3c = static_cast<i32>(0xff008000);
            if (input.GetPress(0x19) != 0.0f)
                SetVal(static_cast<Placeable *>(object.object)->GetName());
        }
    }
}

void PlaceableNameControl::Render() {
}

void PlaceableNameControl::cbButton(eduimenu_s *parent, eduiitem_s *item, u32) {
    edObjectNameControl = static_cast<EdStringControl *>(item->data_ptr);
    eduimenu_s *menu = eduiMenuCreate(parent->x + item->x, item->y, 180, 250,
                                      reinterpret_cast<void *>(static_cast<usize>(EdLevelFnt)), cbEdLevelDestroy, NULL);
    if (menu == NULL)
        return;

    char value[128];
    edObjectNameControl->GetVal(value, sizeof(value));
    eduiMenuAddItem(menu, eduiItemSelCreate(0, item->colours, 0, 0, cbSelectObject, const_cast<char *>("None")));
    for (Placeable *object = static_cast<Placeable *>(thePlaceableHelper.GetNextObject(NULL)); object != NULL;
         object = static_cast<Placeable *>(thePlaceableHelper.GetNextObject(object))) {
        eduiMenuAddItem(menu, eduiItemSelCreate(reinterpret_cast<usize>(object), item->colours, 0, 0, cbSelectObject,
                                                const_cast<char *>(object->GetName())));
        if (NuStrICmp(value, object->GetName()) == 0)
            menu->selected = edui_last_item;
    }
    menu->flags |= 1;
    eduiMenuAttach(parent, menu);
    eduiMenuFitWidth(menu, 5);
    static_cast<edui_prop_s *>(item)->unknown_property_flags &= ~8;
}

void PlaceableNameControl::cbChanged(eduimenu_s *, eduiitem_s *item, u32) {
    EdStringControl *control = static_cast<EdStringControl *>(item->data_ptr);
    for (Placeable *object = static_cast<Placeable *>(thePlaceableHelper.GetNextObject(NULL)); object != NULL;
         object = static_cast<Placeable *>(thePlaceableHelper.GetNextObject(object))) {
        if (NuStrICmp(static_cast<edui_prop_s *>(item)->property_text, object->GetName()) == 0) {
            eduiItemPropSetText(static_cast<edui_prop_s *>(item), const_cast<char *>(object->GetName()));
            control->SetVal(object->GetName());
            return;
        }
    }
}

void PlaceableNameControl::cbSelectObject(eduimenu_s *, eduiitem_s *item, u32) {
    if (edObjectNameControl != NULL) {
        Placeable *object = static_cast<Placeable *>(item->data_ptr);
        char const *name = object != NULL ? object->GetName() : "None";
        eduiItemPropSetText(static_cast<edui_prop_s *>(edObjectNameControl->item), const_cast<char *>(name));
        edObjectNameControl->SetVal(name);
    }
}

VuVec const *Placeable::GetCurrentPosition() const {
    VuMtx const *matrix = GetCurrentTransform();
    return matrix ? reinterpret_cast<VuVec const *>(&matrix->matrix.m30) : NULL;
}

VuVec const *Placeable::GetInitialPosition() const {
    VuMtx const *matrix = GetInitialTransform();
    return matrix ? reinterpret_cast<VuVec const *>(&matrix->matrix.m30) : NULL;
}

void Placeable::SetCurrentPosition(VuVec const *position) {
    NUMTX matrix;
    VuMtx const *current = GetCurrentTransform();
    // The original updates only a temporary matrix.
    if (current) {
        matrix = current->matrix;
        NuMtxTranslate(&matrix, const_cast<NUVEC *>(&position->xyz));
    } else {
        NuMtxSetTranslation(&matrix, const_cast<NUVEC *>(&position->xyz));
    }
}

void Placeable::SetInitialPosition(VuVec const *position) {
    NUMTX matrix;
    VuMtx const *initial = GetInitialTransform();
    if (initial) {
        matrix = initial->matrix;
        NuMtxTranslate(&matrix, const_cast<NUVEC *>(&position->xyz));
    } else {
        NuMtxSetTranslation(&matrix, const_cast<NUVEC *>(&position->xyz));
    }
}
