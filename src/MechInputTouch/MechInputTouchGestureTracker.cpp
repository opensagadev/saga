#include "decomp.h"
#include "MechInputTouch_types.h"

struct GestureTrackerRegistration {
    MechInputTouchGestureTracker *tracker;
    i32 priority;
};

void MechInputTouchGestureTrackingSystem::GetTouch(NuInputTouch const &) {
    STUBBED();
}

void MechInputTouchGestureTrackingSystem::LookForClicks(GameObject_s &) {
    STUBBED();
}

void MechInputTouchGestureTrackingSystem::LookForDown(GameObject_s &) {
    STUBBED();
}

void MechInputTouchGestureTrackingSystem::LookForGestures(GameObject_s &) {
    STUBBED();
}

void MechInputTouchGestureTrackingSystem::LookForHold(GameObject_s &) {
    STUBBED();
}

void MechInputTouchGestureTrackingSystem::LookForRelease(GameObject_s &) {
    STUBBED();
}

void MechInputTouchGestureTrackingSystem::LookForSwipe(GameObject_s &) {
    STUBBED();
}

MechInputTouchGestureTrackingSystem::MechInputTouchGestureTrackingSystem() {
}

void MechInputTouchGestureTrackingSystem::Process(GameObject_s &, NuInputTouchData const &) {
    STUBBED();
}

void MechInputTouchGestureTrackingSystem::ReadData(GameObject_s &, NuInputTouchData const &) {
    STUBBED();
}

void MechInputTouchGestureTrackingSystem::RegisterGestureTracker(MechInputTouchGestureTracker &tracker, i32 priority) {
    GestureTrackerRegistration *entries =
        reinterpret_cast<GestureTrackerRegistration *>(reinterpret_cast<u8 *>(this) + 0x2588);

    bool has_vacancy = (entries[0].tracker == NULL) | (entries[1].tracker == NULL) | (entries[2].tracker == NULL) |
                       (entries[3].tracker == NULL) | (entries[4].tracker == NULL) | (entries[5].tracker == NULL) |
                       (entries[6].tracker == NULL) | (entries[7].tracker == NULL) | (entries[8].tracker == NULL);
    if (entries[9].tracker != NULL && !has_vacancy) {
        return;
    }

    i32 insertion_index = 0;
    while (insertion_index < 9 && entries[insertion_index].tracker != NULL &&
           entries[insertion_index].priority <= priority) {
        ++insertion_index;
    }
    if (insertion_index == 9) {
        return;
    }
    for (i32 index = 9; index > insertion_index; --index) {
        entries[index] = entries[index - 1];
    }
    entries[insertion_index].tracker = &tracker;
    entries[insertion_index].priority = priority;
}

void MechInputTouchGestureTrackingSystem::UnregisterGestureTracker(MechInputTouchGestureTracker &tracker) {
    i32 index = 0;
    while (index < 9 &&
           reinterpret_cast<GestureTrackerRegistration *>(reinterpret_cast<u8 *>(this) + 0x2588)[index].tracker !=
               &tracker)
        ++index;
    if (index == 9)
        return;
    do {
        reinterpret_cast<GestureTrackerRegistration *>(reinterpret_cast<u8 *>(this) + 0x2588)[index] =
            reinterpret_cast<GestureTrackerRegistration *>(reinterpret_cast<u8 *>(this) + 0x2588)[index + 1];
        ++index;
    } while (index != 9);
    reinterpret_cast<GestureTrackerRegistration *>(reinterpret_cast<u8 *>(this) + 0x2588)[9].tracker = NULL;
    reinterpret_cast<GestureTrackerRegistration *>(reinterpret_cast<u8 *>(this) + 0x2588)[9].priority = -1;
}

void MechInputTouchGestureTrackingSystem::Update(NuInputTouchData const *) {
    STUBBED();
}

MechInputTouchGestureTrackingSystem::~MechInputTouchGestureTrackingSystem() {
}
