#include "nu2api_nusound_types.h"
#include "nu2api/numath/nuvec.h"
#include <float.h>

void NuSoundListener::Disable() {
    enabled = false;
}

void NuSoundListener::DisableFocusPosition() {
    focus_enabled = false;
}

void NuSoundListener::Enable() {
    enabled = true;
}

void NuSoundListener::EnableFocusPosition() {
    focus_enabled = true;
}

const VuVec * NuSoundListener::Get2DScreenPosition() const {
    return screen_position;
}

f32 NuSoundListener::GetAttenuationDistance(VuVec const &position) const {
    const VuVec *origin = GetAttenuationPosition(position);
    return NuVecDist((NUVEC *)origin, (NUVEC *)&position, NULL);
}

const VuVec *NuSoundListener::GetAttenuationPosition(VuVec const &position) const {
    if (focus_position != NULL && focus_enabled) {
        f32 focus_distance = NuVecDist((NUVEC *)focus_position, (NUVEC *)&position, NULL);
        if (!(focus_distance > GetHeadDistance(position))) return focus_position;
    }
    return reinterpret_cast<const VuVec *>(reinterpret_cast<const char *>(head_matrix) + 0x30);
}

const VuVec *NuSoundListener::GetFocusPosition() const {
    if (focus_position != NULL && focus_enabled) return focus_position;
    if (head_matrix != NULL) {
        return reinterpret_cast<const VuVec *>(reinterpret_cast<const char *>(head_matrix) + 0x30);
    }
    return NULL;
}

f32 NuSoundListener::GetHeadDistance(VuVec const &position) const {
    if (head_matrix == NULL) return FLT_MAX;
    const char *translation = reinterpret_cast<const char *>(head_matrix) + 0x30;
    return NuVecDist((NUVEC *)translation, (NUVEC *)&position, NULL);
}

const VuMtx * NuSoundListener::GetHeadMatrix() const {
    return head_matrix;
}

i32 NuSoundListener::GetOutputDevices() const {
    return output_devices;
}

f32 NuSoundListener::GetSensitivity() const {
    return sensitivity;
}

const VuVec * NuSoundListener::GetVelocity() const {
    return &velocity;
}

bool NuSoundListener::IsEnabled() const {
    return enabled;
}

bool NuSoundListener::IsFocusPositionEnabled() const {
    return focus_enabled;
}

NuSoundListener::NuSoundListener() {
    sensitivity = 1.0f;
    next = NULL;
    prev = NULL;
    references = NULL;
    head_matrix = NULL;
    focus_position = NULL;
    enabled = false;
    focus_enabled = true;
    output_devices = 1;
}

void NuSoundListener::Set2DScreenPosition(VuVec const *value) {
    screen_position = value;
}

void NuSoundListener::SetFocusPosition(VuVec const *value) {
    focus_position = value;
}

void NuSoundListener::SetHeadMatrix(VuMtx const *value) {
    head_matrix = value;
}

void NuSoundListener::SetOutputDevices(i32 value) {
    output_devices = value;
}

void NuSoundListener::SetSensitivity(float value) {
    if (value >= 0.0f) sensitivity = value;
}

void NuSoundListener::SetVelocity(VuVec const &value) {
    velocity.x = value.x;
    velocity.y = value.y;
    velocity.z = value.z;
    velocity.w = value.w;
}

NuSoundListener::~NuSoundListener() {
    NuSoundEffect::ManagedReference *head = references;
    if (head != NULL) {
        while (head->next != head) {
            NuSoundEffect::ManagedReference *ref = head->next;
            NuSoundEffect::ManagedReference *next = ref->next;
            ref->object = NULL;
            head->next = next;
            ref->previous = NULL;
            ref->next = NULL;
        }
        head->object = NULL;
        head->previous = NULL;
        head->next = NULL;
        references = NULL;
    }
}
