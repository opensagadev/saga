#include "decomp.h"
#include "legoapi/legoapi_types.h"

void SceneObject::Clone(i32) const {
    STUBBED();
}

SceneObject::SceneObject() {
    STUBBED();
}

void SceneObjectHelper::AddMenuItems(eduimenu_s *) {
    STUBBED();
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

void SceneObjectHelper::cbEdSceneObjectShowHiddenSolid(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void SceneObjectHelper::cbEdSceneObjectShowHiddenWire(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

void SceneObjectHelper::cbEdSceneObjectShowOwnedObjects(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
