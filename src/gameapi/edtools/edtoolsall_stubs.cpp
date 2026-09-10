#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edcam.h"

i32 edGetPadDisabled() {
    extern i32 pad_disabled;
    return pad_disabled;
}

void edSetPadDisabled(i32 disabled) {
    extern i32 pad_disabled;
    pad_disabled = disabled;
}

void eduiSetPinnedMenu(eduimenu_s *menu) {
    extern eduimenu_s *edLevelPinnedMenu;
    edLevelPinnedMenu = menu;
}

void EdRegistry::Flush() {
    type_count = 0;
    class_count = 0;
    object_count = 0;
}

EdClass *EdRegistry::GetClass(i32 index) {
    if (index < 0 || index >= class_count) {
        return nullptr;
    }
    return &classes[index];
}

i32 EdRegistry::GetClassId(EdClass *object_class) {
    return object_class - classes;
}

EdType *EdRegistry::GetType(i32 index) {
    if (index < 0 || index >= type_count) {
        return nullptr;
    }
    return &types[index];
}

f32 EdInputContext::GetRelease(i32 input) {
    if (static_cast<u32>(input) < 40 && released[input] != 0) {
        return values[input];
    }
    return 0.0f;
}

f32 EdInputContext::GetRepeat(i32 input) {
    if (static_cast<u32>(input) < 40 && repeated[input] != 0) {
        return values[input];
    }
    return 0.0f;
}
