#include "decomp.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"

extern eduiiattr_s EdLevelAttr;

void SceneObject::Clone(i32) const {
    STUBBED();
}

SceneObject::SceneObject() {
    STUBBED();
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

void SceneObjectHelper::ClearLevel(i32) {
    STUBBED();
}

void SceneObjectHelper::CreateObject(void *, i32, i32) {
    STUBBED();
}

void SceneObjectHelper::DestroyObject(void *, i32) {
    STUBBED();
}

void SceneObjectHelper::Flush() {
    STUBBED();
}

void SceneObjectHelper::GetNextObject(void *) {
    STUBBED();
}

i32 SceneObjectHelper::GetNumObjects() {
    return scene_object_count + owned_object_count;
}

void SceneObjectHelper::Initialise() {
    STUBBED();
}

void SceneObjectHelper::PostLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
    STUBBED();
}

void SceneObjectHelper::PreLoadInitialisation(MemoryBuffer *, MemoryBuffer *) {
    STUBBED();
}

void SceneObjectHelper::SetSceneFilter(char *) {
    STUBBED();
}

void SceneObjectHelper::SubRender() {
    STUBBED();
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
