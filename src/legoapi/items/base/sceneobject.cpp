#include "decomp.h"
#include "globals.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nustring.h"

#include <new>

extern eduiiattr_s EdLevelAttr;
extern ClassEditor theClassEditor;

static EdClassInterfaceVTable scene_object_class_vtable;

static SceneObjectHelper *scene_helper(EdClassInterface *interface) {
    return reinterpret_cast<SceneObjectHelper *>(interface);
}

static void scene_clear_level(EdClassInterface *interface, i32 level) {
    scene_helper(interface)->ClearLevel(level);
}

static void scene_flush(EdClassInterface *interface) {
    scene_helper(interface)->Flush();
}

static i32 scene_get_num_objects(EdClassInterface *interface) {
    return scene_helper(interface)->GetNumObjects();
}

static void *scene_get_next_object(EdClassInterface *interface, void *object) {
    return scene_helper(interface)->GetNextObject(object);
}

static void *scene_create_object(EdClassInterface *interface, void *source, i32 index, i32 flags) {
    return scene_helper(interface)->CreateObject(source, index, flags);
}

static void scene_destroy_object(EdClassInterface *interface, void *object, i32 flags) {
    scene_helper(interface)->DestroyObject(object, flags);
}

static void scene_update_lists(EdClassInterface *interface, MemoryBuffer *first, MemoryBuffer *second) {
    scene_helper(interface)->UpdateLists(first, second);
}

static void scene_add_menu_items(EdClassInterface *interface, eduimenu_s *menu) {
    scene_helper(interface)->AddMenuItems(menu);
}

static void scene_noop_object(EdClassInterface *, void *) {
}
static void scene_noop_guid(EdClassInterface *, void *, i32) {
}
static i32 scene_get_object_guid(EdClassInterface *, void *) {
    return 0;
}
static i32 scene_get_constructor_data(EdClassInterface *, void *, void *, i32) {
    return 0;
}
static void scene_noop_construct(EdClassInterface *, void *, void *) {
}
static void scene_noop_process(EdClassInterface *, void *, EdInputContext &) {
}
static void scene_noop_render(EdClassInterface *, void *, i32) {
}
static void scene_noop(EdClassInterface *) {
}
static void scene_noop_serialise(EdClassInterface *, EdStream &, void *) {
}

static void *scene_get_next_filtered_object(EdClassInterface *interface, void *item, i32 (*filter)(void *)) {
    do {
        item = scene_helper(interface)->GetNextObject(item);
    } while (item != NULL && filter != NULL && filter(item) == 0);
    return item;
}

static f32 scene_distance_to_ray(EdClassInterface *, VuVec &, VuVec &, void *, EdRef **reference) {
    if (reference != NULL)
        *reference = NULL;
    return 1.0e10f;
}

static f32 scene_distance_to_point(EdClassInterface *, VuVec &, void *, EdRef **reference) {
    if (reference != NULL)
        *reference = NULL;
    return 1.0e10f;
}

static void scene_pre_load(EdClassInterface *interface, MemoryBuffer *first, MemoryBuffer *second) {
    scene_helper(interface)->PreLoadInitialisation(first, second);
}

static void scene_post_load(EdClassInterface *interface, MemoryBuffer *first, MemoryBuffer *second) {
    scene_helper(interface)->PostLoadInitialisation(first, second);
}

template <typename Ref>
static void add_scene_reference(EdClass *object_class, char *type, char *name, i32 offset, i32 size, i32 attributes,
                                EdControl *control = NULL) {
    void *memory = theMemoryManager.AllocPool(sizeof(Ref), 1);
    Ref *reference = new (memory) Ref();
    static_cast<EdRef &>(*reference) = EdRef(type, name, offset, size, attributes, control, 0);
    object_class->AddType(reference);
}

void EdDrawBegin(i32 material);
void EdDrawEnd();

void SceneObjectHelperSubSystem::SubRender() {
    SceneObjectHelper *helper = reinterpret_cast<SceneObjectHelper *>(reinterpret_cast<u8 *>(this) - 0x0c);
    helper->SubRender();
}

Placeable *SceneObject::Clone(i32 attributes) const {
    SceneInstance *copy = static_cast<SceneInstance *>(theSceneObjectHelper.CreateObject(NULL, 0, 0));
    if (copy == NULL)
        return NULL;
    EdClass *object_class = reinterpret_cast<EdClassInterface *>(&theSceneObjectHelper)->object_class;
    if (object_class != NULL)
        object_class->CopyObject(copy, const_cast<SceneObject *>(this));
    char unique_name[128];
    theClassEditor.MakeUniqueName(GetName(), unique_name, sizeof(unique_name));
    copy->SetName(unique_name);
    copy->attributes = attributes;
    return copy;
}

SceneObject::SceneObject() {
    editor_owned = 0;
    reserved_0x28 = 0;
    scene_id = static_cast<i16>(theSceneObjectHelper.scene_id);
}

void SceneObjectHelper::AddMenuItems(eduimenu_s *menu) {
    eduiMenuAddItem(menu, eduiItemSeparatorCreate(0, &EdLevelAttr));
    eduiMenuAddItem(menu, eduiItemCheckCreate(0, &EdLevelAttr, show_hidden_solid, 0, cbEdSceneObjectShowHiddenSolid,
                                              const_cast<char *>("Show Hidden Solid")));
    eduiMenuAddItem(menu, eduiItemCheckCreate(0, &EdLevelAttr, show_hidden_wire, 0, cbEdSceneObjectShowHiddenWire,
                                              const_cast<char *>("Show Hidden Wire")));
    eduiMenuAddItem(menu, eduiItemCheckCreate(0, &EdLevelAttr, show_owned_objects, 0, cbEdSceneObjectShowOwnedObjects,
                                              const_cast<char *>("Show Owned Obj")));
}

void SceneObjectHelper::ClearLevel(i32 level) {
    for (SceneObject *object = static_cast<SceneObject *>(GetNextObject(NULL)); object != NULL;) {
        SceneObject *next_object = static_cast<SceneObject *>(GetNextObject(object));
        if (object->led_file == level)
            DestroyObject(object, 0);
        object = next_object;
    }
    if (static_cast<u32>(level) < 10) {
        scenes[level] = NULL;
        scene_counts[level] = 0;
    }
}

void *SceneObjectHelper::CreateObject(void *, i32, i32) {
    void *storage = theMemoryManager.AllocPool(sizeof(SceneInstance), 1);
    if (storage == NULL)
        return NULL;
    SceneInstance *object = new (storage) SceneInstance();
    object->previous = owned_last;
    if (owned_last != NULL)
        owned_last->next = object;
    else
        owned_first = object;
    owned_last = object;
    ++owned_object_count;
    object->SetName("Object");
    return object;
}

void SceneObjectHelper::DestroyObject(void *item, i32) {
    theLevelEditor.destroying_objects = 1;
    SceneInstance *object = static_cast<SceneInstance *>(item);
    if (object == NULL || object->editor_owned == 0)
        return;
    if (object->previous != NULL)
        object->previous->next = object->next;
    else
        owned_first = object->next;
    if (object->next != NULL)
        object->next->previous = object->previous;
    else
        owned_last = object->previous;
    object->previous = NULL;
    object->next = NULL;
    --owned_object_count;
    object->~SceneInstance();
    theMemoryManager.FreePool(object, sizeof(SceneInstance));
}

void SceneObjectHelper::Flush() {
    memset(scenes, 0, sizeof(scenes));
    memset(scene_counts, 0, sizeof(scene_counts));
    owned_first = NULL;
    owned_last = NULL;
    owned_object_count = 0;
}

void *SceneObjectHelper::GetNextObject(void *item) {
    if (item != NULL && static_cast<SceneObject *>(item)->editor_owned != 0)
        return static_cast<SceneInstance *>(item)->next;

    if (item == NULL) {
        iteration_scene_index = 0;
        iteration_object_index = 0;
    } else {
        ++iteration_object_index;
    }
    for (; iteration_scene_index < 10; ++iteration_scene_index, iteration_object_index = 0) {
        for (; iteration_object_index < scene_counts[iteration_scene_index]; ++iteration_object_index) {
            SceneObject *object = reinterpret_cast<SceneObject *>(
                reinterpret_cast<u8 *>(scenes[iteration_scene_index]) + iteration_object_index * sizeof(SceneObject));
            char *filter = scene_filter;
            if (object->reserved_0x28 == 0 &&
                (filter[0] == 0 || NuStrIStr(const_cast<char *>(object->GetName()), filter) != NULL))
                return object;
        }
    }
    return item != NULL ? owned_first : NULL;
}

i32 SceneObjectHelper::GetNumObjects() {
    return scene_object_count + owned_object_count;
}

void SceneObjectHelper::Initialise() {
    scene_object_class_vtable.get_next_filtered_object = scene_get_next_filtered_object;
    scene_object_class_vtable.clear_level = scene_clear_level;
    scene_object_class_vtable.flush = scene_flush;
    scene_object_class_vtable.get_num_objects = scene_get_num_objects;
    scene_object_class_vtable.get_next_object = scene_get_next_object;
    scene_object_class_vtable.create_object = scene_create_object;
    scene_object_class_vtable.destroy_object = scene_destroy_object;
    scene_object_class_vtable.update_lists = scene_update_lists;
    scene_object_class_vtable.add_menu_items = scene_add_menu_items;
    scene_object_class_vtable.defunct_object = scene_noop_object;
    scene_object_class_vtable.revive_object = scene_noop_object;
    scene_object_class_vtable.set_object_guid = scene_noop_guid;
    scene_object_class_vtable.get_object_guid = scene_get_object_guid;
    scene_object_class_vtable.get_constructor_data = scene_get_constructor_data;
    scene_object_class_vtable.construct = scene_noop_construct;
    scene_object_class_vtable.process = scene_noop_process;
    scene_object_class_vtable.render = scene_noop_render;
    scene_object_class_vtable.enter_editor = scene_noop;
    scene_object_class_vtable.exit_editor = scene_noop;
    scene_object_class_vtable.enter_level = scene_noop;
    scene_object_class_vtable.exit_level = scene_noop;
    scene_object_class_vtable.pre_load_initialisation = scene_pre_load;
    scene_object_class_vtable.post_load_initialisation = scene_post_load;
    scene_object_class_vtable.pre_save_initialisation = scene_noop;
    scene_object_class_vtable.post_save_initialisation = scene_noop;
    scene_object_class_vtable.serialise_object = scene_noop_serialise;
    scene_object_class_vtable.distance_to_ray = scene_distance_to_ray;
    scene_object_class_vtable.distance_to_point = scene_distance_to_point;
    scene_object_class_vtable.import = scene_noop;
    class_interface.vtable = &scene_object_class_vtable;

    EdClass *object_class = theRegistry.RegisterClass("SceneObject", &class_interface, 0x100);
    if (object_class != NULL) {
        add_scene_reference<EdRef>(object_class, "Placeable", "Placeable", 0, 0, 0);

        void *control_memory = theMemoryManager.AllocPool(sizeof(EdSpecialObjectControl), 1);
        EdSpecialObjectControl *object_control = new (control_memory) EdSpecialObjectControl();
        add_scene_reference<EdRefSpecialObject>(object_class, "NuHSpecial", "Object", static_cast<i32>(0x80000008), 0,
                                                0, object_control);

        void *visible_memory = theMemoryManager.AllocPool(sizeof(EdEnumControl), 1);
        EdEnumControl *visible_control = new (visible_memory) EdEnumControl();
        visible_control->items = EdEnumControl::YesNoItems;
        add_scene_reference<EdRefSpecialObject>(object_class, "Int", "Visible", static_cast<i32>(0x80000009), 0, 0,
                                                visible_control);

        void *collision_memory = theMemoryManager.AllocPool(sizeof(EdEnumControl), 1);
        EdEnumControl *collision_control = new (collision_memory) EdEnumControl();
        collision_control->items = EdEnumControl::YesNoItems;
        add_scene_reference<EdRefSpecialObject>(object_class, "Int", "Collision", static_cast<i32>(0x8000000a), 0, 0,
                                                collision_control);

        add_scene_reference<EdRefPlaceable>(object_class, "Int", "Attributes", static_cast<i32>(0x80000002), 0,
                                            0x10400000);
        add_scene_reference<EdRefPlaceable>(object_class, "Short", "LEDFile", static_cast<i32>(0x80000001), 0,
                                            0x10400000);
        add_scene_reference<EdRefPlaceable>(object_class, "String", "Name", static_cast<i32>(0x80000003), 250,
                                            0x10400000);
        add_scene_reference<EdRefPlaceable>(object_class, "VuMtx", "Pos", static_cast<i32>(0x80000006), 0, 0x10400000);
    }
    thePlaceableHelper.RegisterObjectType("Scene Object", reinterpret_cast<PlaceableInterface *>(this));
    theEdSystem.RegisterSubSystem(&subsystem);
    memset(scenes, 0, sizeof(scenes));
}

void SceneObjectHelper::PostLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
    STUBBED();
}

void SceneObjectHelper::PreLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
    STUBBED();
}

void SceneObjectHelper::SetSceneFilter(char *filter) {
    if (filter != NULL)
        NuStrCpy(scene_filter, filter);
}

void SceneObjectHelper::SubRender() {
    // The original helper keeps editor-owned SceneObjects in a linked list at
    // 0x98. These members still need names in the public layout; use the
    // verified offsets here so the target render path remains available.
    auto *object = *reinterpret_cast<Placeable **>(reinterpret_cast<u8 *>(this) + 0x98);
    for (; object != nullptr; object = *reinterpret_cast<Placeable **>(reinterpret_cast<u8 *>(object) + 0x2c)) {
        if (*reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(object) + 0x28) != 0)
            continue;
        if (*reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(object) + 0xb4) != 0 ||
            (__builtin_expect(show_hidden_solid != 0, 1) && theLevelEditor.editors_entered != 0)) {
            EdDrawBegin(0);
            object->Render(nullptr);
            EdDrawEnd();
            continue;
        }
        if (__builtin_expect(show_hidden_wire != 0, 0) && theLevelEditor.editors_entered != 0) {
            auto *object_class = *reinterpret_cast<EdClass **>(reinterpret_cast<u8 *>(this) + 4);
            ClassObject selection{object_class, object, nullptr};
            theClassEditor.DrawObjectSphere(selection, 0x80808080);
        }
    }
}

void SceneObjectHelper::UpdateLists(MemoryBuffer *, MemoryBuffer *) {
    STUBBED();
}

void SceneObjectHelper::cbEdSceneObjectShowHiddenSolid(eduimenu_s *, eduiitem_s *item, u32) {
    theSceneObjectHelper.show_hidden_solid ^= 1;
    item->highlighted = theSceneObjectHelper.show_hidden_solid & 1;
}

void SceneObjectHelper::cbEdSceneObjectShowHiddenWire(eduimenu_s *, eduiitem_s *item, u32) {
    theSceneObjectHelper.show_hidden_wire ^= 1;
    item->highlighted = theSceneObjectHelper.show_hidden_wire & 1;
}

void SceneObjectHelper::cbEdSceneObjectShowOwnedObjects(eduimenu_s *, eduiitem_s *item, u32) {
    theSceneObjectHelper.show_owned_objects ^= 1;
    item->highlighted = theSceneObjectHelper.show_owned_objects & 1;
}
