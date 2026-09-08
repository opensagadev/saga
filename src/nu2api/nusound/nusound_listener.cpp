#include "nu2api_nusound_types.h"

#include "nu2api/numath/nuvec.h"

#include <float.h>

void NuSoundListener::Disable() {
    enabled = false;
}

void NuSoundListener::DisableFocusPosition() {
    focus_position_enabled = false;
}

void NuSoundListener::Enable() {
    enabled = true;
}

void NuSoundListener::EnableFocusPosition() {
    focus_position_enabled = true;
}

const VuVec *NuSoundListener::Get2DScreenPosition() const {
    return screen_position;
}

f32 NuSoundListener::GetAttenuationDistance(VuVec const &position) const {
    const VuVec *attenuation_position = GetAttenuationPosition(position);
    return NuVecDist(reinterpret_cast<NUVEC *>(const_cast<VuVec *>(attenuation_position)),
                     reinterpret_cast<NUVEC *>(const_cast<VuVec *>(&position)), NULL);
}

const VuVec *NuSoundListener::GetAttenuationPosition(VuVec const &position) const {
    if (focus_position != NULL && focus_position_enabled) {
        f32 distance = NuVecDist(reinterpret_cast<NUVEC *>(const_cast<VuVec *>(focus_position)),
                                 reinterpret_cast<NUVEC *>(const_cast<VuVec *>(&position)), NULL);
        if (distance > GetHeadDistance(position)) {
            return reinterpret_cast<const VuVec *>(reinterpret_cast<const u8 *>(head_matrix) + 0x30);
        }
        return focus_position;
    }
    return reinterpret_cast<const VuVec *>(reinterpret_cast<const u8 *>(head_matrix) + 0x30);
}

const VuVec *NuSoundListener::GetFocusPosition() const {
    if (focus_position != NULL && focus_position_enabled) {
        return focus_position;
    }
    return head_matrix != NULL ? reinterpret_cast<const VuVec *>(reinterpret_cast<const u8 *>(head_matrix) + 0x30)
                               : NULL;
}

f32 NuSoundListener::GetHeadDistance(VuVec const &position) const {
    if (head_matrix != NULL) {
        return NuVecDist(reinterpret_cast<NUVEC *>(const_cast<u8 *>(reinterpret_cast<const u8 *>(head_matrix)) + 0x30),
                         reinterpret_cast<NUVEC *>(const_cast<VuVec *>(&position)), NULL);
    }
    return FLT_MAX;
}

const VuMtx *NuSoundListener::GetHeadMatrix() const {
    return head_matrix;
}

i32 NuSoundListener::GetOutputDevices() const {
    return output_devices;
}

f32 NuSoundListener::GetSensitivity() const {
    return sensitivity;
}

const VuVec *NuSoundListener::GetVelocity() const {
    return &velocity;
}

bool NuSoundListener::IsEnabled() const {
    return enabled;
}

bool NuSoundListener::IsFocusPositionEnabled() const {
    return focus_position_enabled;
}

NuSoundListener::NuSoundListener() {
    sensitivity = 1.0f;
    field_0x4 = NULL;
    field_0x0 = NULL;
    field_0x8 = NULL;
    head_matrix = NULL;
    focus_position = NULL;
    enabled = false;
    focus_position_enabled = true;
    output_devices = 1;
}

void NuSoundListener::Set2DScreenPosition(VuVec const *position) {
    screen_position = position;
}

void NuSoundListener::SetFocusPosition(VuVec const *position) {
    focus_position = position;
}

void NuSoundListener::SetHeadMatrix(VuMtx const *matrix) {
    head_matrix = matrix;
}

void NuSoundListener::SetOutputDevices(i32 devices) {
    output_devices = devices;
}

void NuSoundListener::SetSensitivity(float value) {
    if (value >= 0.0f) {
        sensitivity = value;
    }
}

void NuSoundListener::SetVelocity(VuVec const &value) {
    velocity = value;
}

NuSoundListener::~NuSoundListener() {
    // Every positional voice contributes one of its embedded three-word
    // listener links to this circular chain.  The target tears the chain down
    // from the predecessor of the anchor and clears the embedded links in the
    // voices; leaving this destructor empty leaves listener->field_0x8
    // pointing at detached/reused voice storage.
    struct ListenerVoiceLink {
        void *listener;
        ListenerVoiceLink *previous;
        ListenerVoiceLink *next;
    };

    ListenerVoiceLink *anchor = static_cast<ListenerVoiceLink *>(field_0x8);
    if (anchor == NULL) {
        return;
    }

    ListenerVoiceLink *link = anchor->previous;
    while (link != anchor) {
        ListenerVoiceLink *previous = link->previous;
        link->listener = NULL;
        anchor->previous = previous;
        link->next = NULL;
        link->previous = NULL;
        link = anchor->previous;
    }

    anchor->listener = NULL;
    anchor->next = NULL;
    anchor->previous = NULL;
    field_0x8 = NULL;
}
