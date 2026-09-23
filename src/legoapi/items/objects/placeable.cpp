#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "nu2api/nucore/NuDynamicLight.h"

#include <new>

i16 Placeable::CurrentLedFile;
PlaceableHelper thePlaceableHelper;

template <typename Ref>
static void add_placeable_reference(EdClass *object_class, char *type, char *name, i32 offset, i32 size, i32 attributes,
                                    EdControl *control = NULL) {
    void *memory = theMemoryManager.AllocPool(sizeof(Ref), 1);
    Ref *reference = new (memory) Ref();
    static_cast<EdRef &>(*reference) = EdRef(type, name, offset, size, attributes, control, 0);
    object_class->AddType(reference);
}

void PlaceableHelper::Find(char *) {
    STUBBED();
}

void PlaceableHelper::Find(char *, Placeable **, i32) {
    STUBBED();
}

void PlaceableHelper::FindObject(char *) {
    STUBBED();
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

void PlaceableHelper::IsEditorObject(ClassObject &) {
    STUBBED();
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

void PlaceableInterface::DebugOutputObjects() {
    STUBBED();
}

void PlaceableInterface::Find(char *) {
    STUBBED();
}

void PlaceableInterface::Find(char *, Placeable **, i32) {
    STUBBED();
}

void PlaceableNameControl::AddMenuItem(eduimenu_s *, EdRef *, void *) {
    STUBBED();
}

PlaceableNameControl::PlaceableNameControl() {
    STUBBED();
}

void PlaceableNameControl::Process(EdInputContext &) {
    STUBBED();
}

void PlaceableNameControl::Render() {
    STUBBED();
}

void PlaceableNameControl::cbButton(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void PlaceableNameControl::cbChanged(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void PlaceableNameControl::cbSelectObject(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
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
