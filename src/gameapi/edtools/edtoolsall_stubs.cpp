#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edcam.h"

void edGetPadDisabled() {
}

void edSetPadDisabled(i32) {
}

void eduiSetPinnedMenu(eduimenu_s *) {
}

void EdRegistry::Flush() {
}

EdClass *EdRegistry::GetClass(i32 index) {
    if (index < 0 || index >= class_count) {
        return nullptr;
    }
    return &classes[index];
}

void EdRegistry::GetClassId(EdClass *) {
}

EdType *EdRegistry::GetType(i32 index) {
    if (index < 0 || index >= type_count) {
        return nullptr;
    }
    return &types[index];
}

void EdInputContext::GetRelease(i32) {
}

void EdInputContext::GetRepeat(i32) {
}
