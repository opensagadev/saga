#include "decomp.h"
#include "legoapi/legoapi_types.h"

void PlaceableHelper::Find(char *) {
    STUBBED();
}

void PlaceableHelper::Find(char *, Placeable **, i32) {
    STUBBED();
}

void PlaceableHelper::FindObject(char *) {
    STUBBED();
}

void PlaceableHelper::GetNextObject(void *) {
    STUBBED();
}

void PlaceableHelper::GetNextObject(void *, i32 (*)(void *)) {
    STUBBED();
}

void PlaceableHelper::Initialise() {
    STUBBED();
}

void PlaceableHelper::IsEditorObject(ClassObject &) {
    STUBBED();
}

PlaceableHelper::PlaceableHelper() {
    object_type_count = 0;
}

void PlaceableHelper::RegisterObjectType(char *, PlaceableInterface *) {
    STUBBED();
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

void Placeable::GetCurrentPosition() const {
    STUBBED();
}

__attribute__((weak)) void Placeable::Reset() {
    STUBBED();
}

void Placeable::GetInitialPosition() const {
    STUBBED();
}

void Placeable::SetCurrentPosition(VuVec const *) {
    STUBBED();
}

void Placeable::SetInitialPosition(VuVec const *) {
    STUBBED();
}
