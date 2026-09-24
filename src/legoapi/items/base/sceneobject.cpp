#include "decomp.h"
#include "globals.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nustring.h"

#include <new>

extern eduiiattr_s EdLevelAttr;
extern ClassEditor theClassEditor;

// The original helper has an EdClassInterface primary vtable and an EdSubSystem
// secondary vtable at +0xc. The field-based types retain those exact layouts.
#define SCENE_VIRTUAL(name, symbol) extern "C" void name() asm(symbol)
#define SCENE_SLOT(field, name) reinterpret_cast<decltype(EdClassInterfaceVTable::field)>(name)
SCENE_VIRTUAL(base_destroy, "_ZN16EdClassInterfaceD1Ev");
SCENE_VIRTUAL(base_delete, "_ZN16EdClassInterfaceD0Ev");
SCENE_VIRTUAL(base_clear, "_ZN16EdClassInterface10ClearLevelEi");
SCENE_VIRTUAL(base_next_filtered, "_ZN16EdClassInterface13GetNextObjectEPvPFiS0_E");
SCENE_VIRTUAL(base_defunct, "_ZN16EdClassInterface13DefunctObjectEPv");
SCENE_VIRTUAL(base_revive, "_ZN16EdClassInterface12ReviveObjectEPv");
SCENE_VIRTUAL(base_set_guid, "_ZN16EdClassInterface13SetObjectGuidEPvi");
SCENE_VIRTUAL(base_get_guid, "_ZN16EdClassInterface13GetObjectGuidEPv");
SCENE_VIRTUAL(base_constructor_data, "_ZN16EdClassInterface18GetConstructorDataEPvS0_i");
SCENE_VIRTUAL(base_construct, "_ZN16EdClassInterface9ConstructEPvS0_");
SCENE_VIRTUAL(base_process, "_ZN16EdClassInterface7ProcessEPvR14EdInputContext");
SCENE_VIRTUAL(base_render, "_ZN16EdClassInterface6RenderEPvi");
SCENE_VIRTUAL(base_enter_editor, "_ZN16EdClassInterface11EnterEditorEv");
SCENE_VIRTUAL(base_exit_editor, "_ZN16EdClassInterface10ExitEditorEv");
SCENE_VIRTUAL(base_enter_level, "_ZN16EdClassInterface10EnterLevelEv");
SCENE_VIRTUAL(base_exit_level, "_ZN16EdClassInterface9ExitLevelEv");
SCENE_VIRTUAL(base_update_lists, "_ZN16EdClassInterface11UpdateListsEP12MemoryBufferS1_");
SCENE_VIRTUAL(base_pre_load, "_ZN16EdClassInterface21PreLoadInitialisationEP12MemoryBufferS1_");
SCENE_VIRTUAL(base_post_load, "_ZN16EdClassInterface22PostLoadInitialisationEP12MemoryBufferS1_");
SCENE_VIRTUAL(base_pre_save, "_ZN16EdClassInterface21PreSaveInitialisationEv");
SCENE_VIRTUAL(base_post_save, "_ZN16EdClassInterface22PostSaveInitialisationEv");
SCENE_VIRTUAL(base_serialise, "_ZN16EdClassInterface15SerialiseObjectER8EdStreamPv");
SCENE_VIRTUAL(base_distance_ray, "_ZN16EdClassInterface16DistanceToObjectER5VuVecS1_PvPP5EdRef");
SCENE_VIRTUAL(base_distance_point, "_ZN16EdClassInterface16DistanceToObjectER5VuVecPvPP5EdRef");
SCENE_VIRTUAL(base_add_menu, "_ZN16EdClassInterface12AddMenuItemsEP10eduimenu_s");
SCENE_VIRTUAL(base_import, "_ZN16EdClassInterface6ImportEv");
SCENE_VIRTUAL(scene_destroy, "_ZN17SceneObjectHelperD1Ev");
SCENE_VIRTUAL(scene_delete, "_ZN17SceneObjectHelperD0Ev");
SCENE_VIRTUAL(scene_clear, "_ZN17SceneObjectHelper10ClearLevelEi");
SCENE_VIRTUAL(scene_flush, "_ZN17SceneObjectHelper5FlushEv");
SCENE_VIRTUAL(scene_count, "_ZN17SceneObjectHelper13GetNumObjectsEv");
SCENE_VIRTUAL(scene_next, "_ZN17SceneObjectHelper13GetNextObjectEPv");
SCENE_VIRTUAL(scene_create, "_ZN17SceneObjectHelper12CreateObjectEPvii");
SCENE_VIRTUAL(scene_destroy_object, "_ZN17SceneObjectHelper13DestroyObjectEPvi");
SCENE_VIRTUAL(scene_update_lists, "_ZN17SceneObjectHelper11UpdateListsEP12MemoryBufferS1_");
SCENE_VIRTUAL(scene_pre_load, "_ZN17SceneObjectHelper21PreLoadInitialisationEP12MemoryBufferS1_");
SCENE_VIRTUAL(scene_post_load, "_ZN17SceneObjectHelper22PostLoadInitialisationEP12MemoryBufferS1_");
SCENE_VIRTUAL(scene_add_menu, "_ZN17SceneObjectHelper12AddMenuItemsEP10eduimenu_s");
SCENE_VIRTUAL(scene_sub_render, "_ZN17SceneObjectHelper9SubRenderEv");
SCENE_VIRTUAL(scene_sub_destroy, "_ZThn12_N17SceneObjectHelperD1Ev");
SCENE_VIRTUAL(scene_sub_delete, "_ZThn12_N17SceneObjectHelperD0Ev");
SCENE_VIRTUAL(sub_initialise, "_ZN11EdSubSystem13SubInitialiseER9variptr_uS1_i");
SCENE_VIRTUAL(sub_reset, "_ZN11EdSubSystem8SubResetEv");
SCENE_VIRTUAL(sub_process, "_ZN11EdSubSystem10SubProcessEf");
SCENE_VIRTUAL(scene_sub_render_thunk, "_ZThn12_N17SceneObjectHelper9SubRenderEv");

extern const EdClassInterfaceVTableObject sceneBaseInterfaceVTable asm("_ZTV16EdClassInterface")
    __attribute__((weak)) = {NULL,
                             NULL,
                             {SCENE_SLOT(destroy, base_destroy),
                              SCENE_SLOT(delete_object, base_delete),
                              SCENE_SLOT(clear_level, base_clear),
                              NULL,
                              NULL,
                              NULL,
                              SCENE_SLOT(get_next_filtered_object, base_next_filtered),
                              NULL,
                              NULL,
                              SCENE_SLOT(defunct_object, base_defunct),
                              SCENE_SLOT(revive_object, base_revive),
                              SCENE_SLOT(set_object_guid, base_set_guid),
                              SCENE_SLOT(get_object_guid, base_get_guid),
                              SCENE_SLOT(get_constructor_data, base_constructor_data),
                              SCENE_SLOT(construct, base_construct),
                              SCENE_SLOT(process, base_process),
                              SCENE_SLOT(render, base_render),
                              SCENE_SLOT(enter_editor, base_enter_editor),
                              SCENE_SLOT(exit_editor, base_exit_editor),
                              SCENE_SLOT(enter_level, base_enter_level),
                              SCENE_SLOT(exit_level, base_exit_level),
                              SCENE_SLOT(update_lists, base_update_lists),
                              SCENE_SLOT(pre_load_initialisation, base_pre_load),
                              SCENE_SLOT(post_load_initialisation, base_post_load),
                              SCENE_SLOT(pre_save_initialisation, base_pre_save),
                              SCENE_SLOT(post_save_initialisation, base_post_save),
                              SCENE_SLOT(serialise_object, base_serialise),
                              SCENE_SLOT(distance_to_ray, base_distance_ray),
                              SCENE_SLOT(distance_to_point, base_distance_point),
                              SCENE_SLOT(add_menu_items, base_add_menu),
                              SCENE_SLOT(import, base_import)}};

extern const SceneObjectHelperVTableObject sceneObjectHelperVTable asm("_ZTV17SceneObjectHelper")
    __attribute__((weak)) = {
        NULL,
        NULL,
        {SCENE_SLOT(destroy, scene_destroy),
         SCENE_SLOT(delete_object, scene_delete),
         SCENE_SLOT(clear_level, scene_clear),
         SCENE_SLOT(flush, scene_flush),
         SCENE_SLOT(get_num_objects, scene_count),
         SCENE_SLOT(get_next_object, scene_next),
         SCENE_SLOT(get_next_filtered_object, base_next_filtered),
         SCENE_SLOT(create_object, scene_create),
         SCENE_SLOT(destroy_object, scene_destroy_object),
         SCENE_SLOT(defunct_object, base_defunct),
         SCENE_SLOT(revive_object, base_revive),
         SCENE_SLOT(set_object_guid, base_set_guid),
         SCENE_SLOT(get_object_guid, base_get_guid),
         SCENE_SLOT(get_constructor_data, base_constructor_data),
         SCENE_SLOT(construct, base_construct),
         SCENE_SLOT(process, base_process),
         SCENE_SLOT(render, base_render),
         SCENE_SLOT(enter_editor, base_enter_editor),
         SCENE_SLOT(exit_editor, base_exit_editor),
         SCENE_SLOT(enter_level, base_enter_level),
         SCENE_SLOT(exit_level, base_exit_level),
         SCENE_SLOT(update_lists, scene_update_lists),
         SCENE_SLOT(pre_load_initialisation, scene_pre_load),
         SCENE_SLOT(post_load_initialisation, scene_post_load),
         SCENE_SLOT(pre_save_initialisation, base_pre_save),
         SCENE_SLOT(post_save_initialisation, base_post_save),
         SCENE_SLOT(serialise_object, base_serialise),
         SCENE_SLOT(distance_to_ray, base_distance_ray),
         SCENE_SLOT(distance_to_point, base_distance_point),
         SCENE_SLOT(add_menu_items, scene_add_menu),
         SCENE_SLOT(import, base_import)},
        reinterpret_cast<decltype(SceneObjectHelperVTableObject::sub_render)>(scene_sub_render),
        -12,
        NULL,
        {scene_sub_destroy, scene_sub_delete, sub_initialise, sub_reset, sub_process, scene_sub_render_thunk}};

#undef SCENE_SLOT
#undef SCENE_VIRTUAL

struct EdSubSystemVTableObject {
    void *offset_to_top;
    void *type_info;
    void (*methods[6])();
};
extern const EdSubSystemVTableObject edSubSystemVTable asm("_ZTV11EdSubSystem");

extern "C" void baseInterfaceDestructor(EdClassInterface *object) asm("_ZN16EdClassInterfaceD1Ev")
    __attribute__((weak));
void baseInterfaceDestructor(EdClassInterface *object) {
    object->vtable = const_cast<EdClassInterfaceVTable *>(&sceneBaseInterfaceVTable.methods);
}
extern "C" void baseInterfaceCompleteDestructor(EdClassInterface *) asm("_ZN16EdClassInterfaceD2Ev")
    __attribute__((weak, alias("_ZN16EdClassInterfaceD1Ev")));
extern "C" void baseInterfaceDeletingDestructor(EdClassInterface *object) asm("_ZN16EdClassInterfaceD0Ev")
    __attribute__((weak));
void baseInterfaceDeletingDestructor(EdClassInterface *object) {
    object->vtable = const_cast<EdClassInterfaceVTable *>(&sceneBaseInterfaceVTable.methods);
    ::operator delete(object);
}

extern "C" void sceneHelperDestructor(SceneObjectHelper *object) asm("_ZN17SceneObjectHelperD1Ev")
    __attribute__((weak));
void sceneHelperDestructor(SceneObjectHelper *object) {
    *reinterpret_cast<void **>(&object->subsystem) =
        const_cast<void *>(static_cast<void const *>(&edSubSystemVTable.methods));
    object->class_interface.vtable = const_cast<EdClassInterfaceVTable *>(&sceneBaseInterfaceVTable.methods);
}
extern "C" void sceneHelperCompleteDestructor(SceneObjectHelper *) asm("_ZN17SceneObjectHelperD2Ev")
    __attribute__((weak, alias("_ZN17SceneObjectHelperD1Ev")));
extern "C" void sceneHelperDeletingDestructor(SceneObjectHelper *object) asm("_ZN17SceneObjectHelperD0Ev")
    __attribute__((weak));
void sceneHelperDeletingDestructor(SceneObjectHelper *object) {
    *reinterpret_cast<void **>(&object->subsystem) =
        const_cast<void *>(static_cast<void const *>(&edSubSystemVTable.methods));
    object->class_interface.vtable = const_cast<EdClassInterfaceVTable *>(&sceneBaseInterfaceVTable.methods);
}
// The original adjustment thunks tail-jump after subtracting the secondary
// subobject's +0xc offset. A C++ wrapper would add a call frame here.
#if defined(__i386__)
asm(".weak _ZThn12_N17SceneObjectHelperD1Ev\n"
    ".type _ZThn12_N17SceneObjectHelperD1Ev, @function\n"
    "_ZThn12_N17SceneObjectHelperD1Ev:\n"
    "subl $12, 4(%esp)\n"
    "jmp _ZN17SceneObjectHelperD1Ev\n"
    ".size _ZThn12_N17SceneObjectHelperD1Ev, .-_ZThn12_N17SceneObjectHelperD1Ev\n"
    ".weak _ZThn12_N17SceneObjectHelperD0Ev\n"
    ".type _ZThn12_N17SceneObjectHelperD0Ev, @function\n"
    "_ZThn12_N17SceneObjectHelperD0Ev:\n"
    "subl $12, 4(%esp)\n"
    "jmp _ZN17SceneObjectHelperD0Ev\n"
    ".size _ZThn12_N17SceneObjectHelperD0Ev, .-_ZThn12_N17SceneObjectHelperD0Ev\n");
#else
extern "C" void sceneHelperSubDestructor(EdSubSystem *object) asm("_ZThn12_N17SceneObjectHelperD1Ev");
void sceneHelperSubDestructor(EdSubSystem *object) {
    sceneHelperDestructor(reinterpret_cast<SceneObjectHelper *>(reinterpret_cast<u8 *>(object) - 12));
}
extern "C" void sceneHelperSubDeletingDestructor(EdSubSystem *object) asm("_ZThn12_N17SceneObjectHelperD0Ev");
void sceneHelperSubDeletingDestructor(EdSubSystem *object) {
    sceneHelperDeletingDestructor(reinterpret_cast<SceneObjectHelper *>(reinterpret_cast<u8 *>(object) - 12));
}
#endif

template <typename Ref>
static void add_scene_reference(EdClass *object_class, char *type, char *name, i32 offset, i32 size, i32 attributes,
                                EdControl *control = NULL) {
    void *memory = theMemoryManager.AllocPool(sizeof(Ref), 1);
    Ref *reference = new (memory) Ref(type, name, offset, size, attributes, control, 0);
    object_class->AddType(reference);
}

void EdDrawBegin(i32 material);
void EdDrawEnd();

#if defined(__i386__)
asm(".globl _ZThn12_N17SceneObjectHelper9SubRenderEv\n"
    ".type _ZThn12_N17SceneObjectHelper9SubRenderEv, @function\n"
    "_ZThn12_N17SceneObjectHelper9SubRenderEv:\n"
    "subl $12, 4(%esp)\n"
    "jmp _ZN17SceneObjectHelper9SubRenderEv\n"
    ".size _ZThn12_N17SceneObjectHelper9SubRenderEv, .-_ZThn12_N17SceneObjectHelper9SubRenderEv\n");
#else
extern "C" void sceneHelperSubRender(EdSubSystem *object) asm("_ZThn12_N17SceneObjectHelper9SubRenderEv");
void sceneHelperSubRender(EdSubSystem *object) {
    SceneObjectHelper *helper = reinterpret_cast<SceneObjectHelper *>(reinterpret_cast<u8 *>(object) - 12);
    helper->SubRender();
}
#endif

Placeable *SceneObject::Clone(i32 attributes) const {
    SceneInstance *copy = static_cast<SceneInstance *>(theSceneObjectHelper.CreateObject(NULL, 0, 0));
    if (copy == NULL)
        return NULL;
    EdClass *object_class = theSceneObjectHelper.class_interface.object_class;
    object_class->CopyObject(copy, const_cast<SceneObject *>(this));
    char unique_name[128];
    theClassEditor.MakeUniqueName(copy->GetName(), unique_name, sizeof(unique_name));
    copy->SetName(unique_name);
    copy->attributes = attributes;
    return copy;
}

SceneObject::SceneObject() {
    editor_owned = 0;
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
    for (SceneObject *object =
             static_cast<SceneObject *>(class_interface.vtable->get_next_object(&class_interface, NULL));
         object != NULL;) {
        SceneObject *next_object =
            static_cast<SceneObject *>(class_interface.vtable->get_next_object(&class_interface, object));
        if (object->led_file == level)
            class_interface.vtable->destroy_object(&class_interface, object, 0);
        object = next_object;
    }
    i32 scene_index;
    if (level == 0)
        scene_index = 0;
    else if (level == 1)
        scene_index = 1;
    else if (level == 2)
        scene_index = 2;
    else if (level == 3)
        scene_index = 3;
    else if (level == 4)
        scene_index = 4;
    else if (level == 5)
        scene_index = 5;
    else if (level == 6)
        scene_index = 6;
    else if (level == 7)
        scene_index = 7;
    else if (level == 8)
        scene_index = 8;
    else if (level == 9)
        scene_index = 9;
    else
        return;
    scenes[scene_index] = NULL;
    scene_counts[scene_index] = 0;
}

void *SceneObjectHelper::CreateObject(void *, i32, i32) {
    void *storage = theMemoryManager.AllocPool(sizeof(SceneInstance), 1);
    SceneInstance *object = new (storage) SceneInstance();
    if (object != NULL) {
        object->next = NULL;
        object->previous = owned_last;
        if (owned_last != NULL)
            owned_last->next = object;
        owned_last = object;
        if (owned_first == NULL)
            owned_first = object;
        ++owned_object_count;
        object->SetName("object");
    }
    return object;
}

void SceneObjectHelper::DestroyObject(void *item, i32) {
    theLevelEditor.destroying_objects = 1;
    SceneInstance *object = static_cast<SceneInstance *>(item);
    if (object->editor_owned == 0)
        return;
    if (object->next != NULL)
        object->next->previous = object->previous;
    else
        owned_last = object->previous;
    if (object->previous != NULL)
        object->previous->next = object->next;
    else
        owned_first = object->next;
    object->next = NULL;
    object->previous = NULL;
    --owned_object_count;
    delete object;
}

void SceneObjectHelper::Flush() {
    memset(scenes, 0, sizeof(scenes));
    memset(scene_counts, 0, sizeof(scene_counts));
    owned_last = NULL;
    owned_first = NULL;
    owned_object_count = 0;
}

void *SceneObjectHelper::GetNextObject(void *item) {
    if (item == NULL) {
        iteration_scene_index = 0;
        iteration_object_index = 0;
    } else {
        ++iteration_object_index;
        if (iteration_scene_index > 9)
            goto owned_objects;
    }
    while (iteration_scene_index < 10) {
        while (scene_counts[iteration_scene_index] > iteration_object_index) {
            SceneObject *object = reinterpret_cast<SceneObject *>(
                reinterpret_cast<u8 *>(scenes[iteration_scene_index]) + iteration_object_index * sizeof(SceneObject));
            if (object->reserved_0x28 == 0) {
                if (scene_filter[0] == 0 || NuStrIStr(const_cast<char *>(object->GetName()), scene_filter) == NULL)
                    return reinterpret_cast<SceneObject *>(reinterpret_cast<u8 *>(scenes[iteration_scene_index]) +
                                                           iteration_object_index * sizeof(SceneObject));
            }
            ++iteration_object_index;
        }
        ++iteration_scene_index;
        iteration_object_index = 0;
    }
    if (item == NULL)
        return NULL;

owned_objects:
    return static_cast<SceneObject *>(item)->editor_owned != 0 ? static_cast<SceneInstance *>(item)->next : owned_first;
}

i32 SceneObjectHelper::GetNumObjects() {
    return scene_object_count + owned_object_count;
}

void SceneObjectHelper::Initialise() {
    EdClass *object_class = theRegistry.RegisterClass("SceneObject", &class_interface, 0x100);
    if (object_class != NULL) {
        add_scene_reference<EdRef>(object_class, "Placeable", "Placeable", 0, 0, 0);

        void *control_memory = theMemoryManager.AllocPool(sizeof(EdSpecialObjectControl), 1);
        EdSpecialObjectControl *object_control = new (control_memory) EdSpecialObjectControl();
        add_scene_reference<EdRefSpecialObject>(object_class, "NuHSpecial", "Object", static_cast<i32>(0x80000008), 0,
                                                0, object_control);

        void *visible_memory = theMemoryManager.AllocPool(sizeof(EdEnumControl), 1);
        EdEnumControl *visible_control = new (visible_memory) EdEnumControl;
        visible_control->items = EdEnumControl::YesNoItems;
        add_scene_reference<EdRefSpecialObject>(object_class, "Int", "Visible", static_cast<i32>(0x80000009), 0, 0,
                                                visible_control);

        void *collision_memory = theMemoryManager.AllocPool(sizeof(EdEnumControl), 1);
        EdEnumControl *collision_control = new (collision_memory) EdEnumControl;
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
    memset(scene_counts, 0, sizeof(scene_counts));
}

void SceneObjectHelper::PostLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
}

void SceneObjectHelper::PreLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
    for (i32 level = 0; level < 10; ++level) {
        i32 *count_slot = &scene_counts[level];
        SceneObject **scene_slot = &scenes[level];
        nugscn_s *scene = theLevelEditor.GetScene(level);
        if (scene != NULL && *count_slot == 0) {
            *count_slot = NuGScnNumSpecials(scene);
            const usize bytes = *count_slot * sizeof(SceneObject);
            SceneObject *objects = NULL;
            if (bytes < *theMemoryManager.end_cell - *theMemoryManager.cursor_cell) {
                const usize aligned = ALIGN(*theMemoryManager.cursor_cell, 0x10);
                *theMemoryManager.cursor_cell = aligned + bytes;
                objects = reinterpret_cast<SceneObject *>(aligned);
                memset(objects, 0, bytes);
                theMemoryManager.allocated += bytes;
                theMemoryManager.remaining -= bytes;
                theMemoryManager.high_water = *theMemoryManager.cursor_cell;
            }
            *scene_slot = objects;
            for (i32 index = 0; index < *count_slot; ++index)
                new (&(*scene_slot)[index]) SceneObject();
        }
    }

    scene_object_count = 0;
    i32 *count_slot = scene_counts;
    for (i32 level = 0; level < 10; ++level, ++count_slot) {
        SceneObject *object = *reinterpret_cast<SceneObject **>(count_slot - 10);
        nugscn_s *scene = theLevelEditor.GetScene(level);
        if (scene != NULL) {
            const i32 count = *count_slot;
            for (i32 index = 0; index < count; ++index) {
                NuGScnGetSpecial(&object->special, scene, index);
                object->attributes = 0x12400000;
                object->led_file = level;
                char *name = NuSpecialGetName(&object->special);
                if (name != NULL)
                    NuStrICmp(name, "reflectport");
                ++object;
                ++scene_object_count;
            }
        }
    }
}

void SceneObjectHelper::SetSceneFilter(char *filter) {
    if (filter != NULL)
        NuStrCpy(scene_filter, filter);
}

void SceneObjectHelper::SubRender() {
    for (SceneInstance *object = owned_first; object != NULL; object = object->next) {
        if (object->reserved_0x28 != 0)
            continue;
        if (object->visibility == 0 &&
            (__builtin_expect(show_hidden_solid == 0, 0) || theLevelEditor.editors_entered == 0)) {
            if (__builtin_expect(show_hidden_wire != 0, 0) && theLevelEditor.editors_entered != 0) {
                ClassObject selection{class_interface.object_class, object, NULL};
                theClassEditor.DrawObjectSphere(selection, 0x80808080);
            }
            continue;
        }
        EdDrawBegin(0);
        object->Render(NULL);
        EdDrawEnd();
    }
}

void SceneObjectHelper::UpdateLists(MemoryBuffer *first, MemoryBuffer *second) {
    class_interface.vtable->pre_load_initialisation(&class_interface, first, second);
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
