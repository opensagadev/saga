#include "legoapi/legoapi_types.h"

VuVec const *SceneInstance::GetCurrentPosition() const {
    return reinterpret_cast<VuVec const *>(&current_transform.m30);
}

VuMtx const *SceneInstance::GetCurrentTransform() const {
    return reinterpret_cast<VuMtx const *>(&current_transform);
}

VuVec const *SceneInstance::GetInitialPosition() const {
    return reinterpret_cast<VuVec const *>(&initial_transform.m30);
}

VuMtx const *SceneInstance::GetInitialTransform() const {
    return reinterpret_cast<VuMtx const *>(&initial_transform);
}

int SceneInstance::GetVisibility() const {
    return visibility;
}

void SceneInstance::Render(VuMtx const *) const {
}

SceneInstance::SceneInstance() {
}

void SceneInstance::SetCurrentPosition(VuVec const *position) {
    *reinterpret_cast<VuVec *>(&current_transform.m30) = *position;
}

void SceneInstance::SetCurrentTransform(VuMtx const *transform) {
    current_transform = *reinterpret_cast<NUMTX const *>(transform);
}

void SceneInstance::SetInitialPosition(VuVec const *position) {
    *reinterpret_cast<VuVec *>(&current_transform.m30) = *position;
}

void SceneInstance::SetInitialTransform(VuMtx const *transform) {
    initial_transform = *reinterpret_cast<NUMTX const *>(transform);
}

void SceneInstance::SetVisibility(i32 visible) {
    visibility = visible;
}
