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

    i32 insertion_index = 0;
    while (insertion_index < 10 && entries[insertion_index].tracker != NULL &&
           entries[insertion_index].priority <= priority) {
        ++insertion_index;
    }
    if (insertion_index == 10) {
        return;
    }
    for (i32 index = 9; index > insertion_index; --index) {
        entries[index] = entries[index - 1];
    }
    entries[insertion_index].tracker = &tracker;
    entries[insertion_index].priority = priority;
}

void MechInputTouchGestureTrackingSystem::UnregisterGestureTracker(MechInputTouchGestureTracker &tracker) {
    GestureTrackerRegistration *entries =
        reinterpret_cast<GestureTrackerRegistration *>(reinterpret_cast<u8 *>(this) + 0x2588);
    i32 index = 0;
    while (index < 9 && entries[index].tracker != &tracker)
        ++index;
    if (index == 9)
        return;
    do {
        entries[index] = entries[index + 1];
        ++index;
    } while (index != 9);
    entries[9].tracker = NULL;
    entries[9].priority = -1;
}

void MechInputTouchGestureTrackingSystem::Update(NuInputTouchData const *) {
    STUBBED();
}

MechInputTouchGestureTrackingSystem::~MechInputTouchGestureTrackingSystem() {
}
