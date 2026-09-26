#include "decomp.h"
#include "MechInputTouch_types.h"
#include "globals.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/characters/core/character.h"
#include "gamelib/util/gamelib_util_types.h"

f32 TimeSampleDelta = 0.05f;
i32 GetMenuID();

TouchHolder *MechInputTouchGestureTrackingSystem::GetTouch(NuInputTouch const &touch) {
    for (i32 index = 0; index < 10; ++index) {
        TouchHolder *holder = &holders[index];
        if (holder->touch_id == static_cast<i32>(touch.unknown_14)) {
            return holder;
        }
    }
    for (i32 index = 0; index < 10; ++index) {
        TouchHolder *holder = &holders[index];
        if (holder->touch_id == -1) {
            holder->touch_id = touch.unknown_14;
            return holder;
        }
    }
    return holders;
}

void MechInputTouchGestureTrackingSystem::LookForClicks(GameObject_s &object) {
    GestureTrackerRegistration *entries = trackers;
    for (i32 index = 0; index < 10; ++index) {
        TouchHolder &holder = holders[index];
        if (holder.is_down || !holder.field_0x6) {
            continue;
        }
        holder.click_candidate = 1;
        f32 dx = holder.touch_position.x - holder.down_position.x;
        f32 dy = holder.touch_position.y - holder.down_position.y;
        if (dy * dy + dx * dx >= 0.0025f) {
            continue;
        }
        bool first_recent = holder.click_timer < 0.3f;
        bool later_recent = holder.double_click_timer < 0.3f && holder.release_timer < 0.3f;
        if (first_recent && later_recent) {
            for (i32 priority = 0; priority < 10; ++priority) {
                MechInputTouchGestureTracker *tracker = entries[priority].tracker;
                if (tracker != NULL && tracker->OnDoubleClick(object, holder)) {
                    break;
                }
            }
        } else if (first_recent || later_recent) {
            for (i32 priority = 0; priority < 10; ++priority) {
                MechInputTouchGestureTracker *tracker = entries[priority].tracker;
                if (tracker != NULL && tracker->OnClick(object, holder)) {
                    break;
                }
            }
        }
    }
}

void MechInputTouchGestureTrackingSystem::LookForDown(GameObject_s &object) {
    GestureTrackerRegistration *entries = trackers;
    for (i32 index = 0; index < 10; ++index) {
        TouchHolder &holder = holders[index];
        if (holder.is_down && !holder.field_0x6) {
            for (i32 priority = 0; priority < 10; ++priority) {
                MechInputTouchGestureTracker *tracker = entries[priority].tracker;
                if (tracker != NULL && tracker->OnDown(object, holder)) {
                    break;
                }
            }
        }
    }
}

void MechInputTouchGestureTrackingSystem::LookForGestures(GameObject_s &object) {
    LookForDown(object);
    LookForClicks(object);
    LookForRelease(object);
    LookForHold(object);
    LookForSwipe(object);
}

void MechInputTouchGestureTrackingSystem::LookForHold(GameObject_s &object) {
    GestureTrackerRegistration *entries = trackers;
    for (i32 index = 0; index < 10; ++index) {
        TouchHolder &holder = holders[index];
        if (holder.is_down && !holder.consumed && holder.held_time >= 0.2f) {
            for (i32 priority = 0; priority < 10; ++priority) {
                MechInputTouchGestureTracker *tracker = entries[priority].tracker;
                if (tracker != NULL && tracker->OnHold(object, holder)) {
                    break;
                }
            }
        }
    }
}

void MechInputTouchGestureTrackingSystem::LookForRelease(GameObject_s &object) {
    GestureTrackerRegistration *entries = trackers;
    for (i32 index = 0; index < 10; ++index) {
        TouchHolder &holder = holders[index];
        if (!holder.is_down && holder.field_0x6) {
            for (i32 priority = 0; priority < 10; ++priority) {
                MechInputTouchGestureTracker *tracker = entries[priority].tracker;
                if (tracker != NULL && tracker->OnRelease(object, holder)) {
                    break;
                }
            }
        }
    }
}

void MechInputTouchGestureTrackingSystem::LookForSwipe(GameObject_s &object) {
    GestureTrackerRegistration *entries = trackers;
    for (i32 index = 0; index < 10; ++index) {
        TouchHolder &holder = holders[index];
        if (holder.is_down || !holder.field_0x6) {
            continue;
        }
        f32 elapsed = 0.0f;
        f32 squared_path = 0.0f;
        i32 previous_index = 0;
        for (i32 sample_index = 0; sample_index < 20; ++sample_index) {
            TouchSwipeSample &sample = holder.swipe_samples[sample_index];
            if (sample.time < 0.0f) {
                break;
            }
            if (sample_index > 0 && elapsed < 0.2f) {
                previous_index = sample_index - 1;
                TouchSwipeSample &previous = holder.swipe_samples[previous_index];
                elapsed += previous.time - sample.time;
                f32 dx = sample.position.x - previous.position.x;
                f32 dy = sample.position.y - previous.position.y;
                squared_path += dx * dx + dy * dy;
            }
        }
        if (squared_path > 0.05f) {
            for (i32 priority = 0; priority < 10; ++priority) {
                MechInputTouchGestureTracker *tracker = entries[priority].tracker;
                if (tracker != NULL && tracker->OnSwipe(object, holder, previous_index)) {
                    break;
                }
            }
        }
    }
}

MechInputTouchGestureTrackingSystem::MechInputTouchGestureTrackingSystem()
    : NuTouchInputElement(static_cast<NuTouchInputElement::TYPE>(3), 0xffff00ff, 0) {
    for (i32 index = 0; index < 10; ++index) {
        trackers[index].tracker = NULL;
        trackers[index].priority = -1;
    }
}

void MechInputTouchGestureTrackingSystem::Process(GameObject_s &object, NuInputTouchData const &data) {
    ReadData(object, data);
    LookForGestures(object);
}

void MechInputTouchGestureTrackingSystem::ReadData(GameObject_s &object, NuInputTouchData const &data) {
    for (i32 index = 0; index < 10; ++index) {
        TouchHolder &holder = holders[index];
        holder.held_time += FRAMETIME;
        holder.sample_countdown += FRAMETIME;
        holder.field_0x6 = holder.is_down;
        holder.is_down = 0;
    }

    for (i32 index = 0; index < static_cast<i32>(data.touch_count); ++index) {
        NuInputTouch const &touch = data.touch_events[index];
        f32 x, y;
        MechInputTouchSystem::ConvertToScreenCoords(touch.unknown_08, touch.unknown_0c, x, y);
        TouchHolder *holder = GetTouch(touch);
        if (!holder->field_0x6) {
            holder->click_candidate = 0;
            holder->down_position.x = x;
            holder->down_position.y = y;
            holder->sample_countdown = TimeSampleDelta;
            VuVec screen_position(x, y, 1.0f, 1.0f);
            MechObjectInterface *target = NULL;
            if (player != NULL && NewMode == 0 && NewLData == NULL && GetMenuID() == -1 && Paused == 0 &&
                TouchHacks::TouchControlsActive) {
                target = MechInputTouchSystem::FindTargetObject(object, screen_position, 0x77f, NULL, NULL);
            }
            holder->target_object = target;
            holder->oldest_click_timer = holder->release_timer;
            holder->release_timer = holder->double_click_timer;
            holder->double_click_timer = holder->click_timer;
            holder->click_timer = holder->held_time;
            holder->held_time = 0.0f;
            for (i32 sample = 0; sample < 20; ++sample) {
                holder->swipe_samples[sample].position.x = 0.0f;
                holder->swipe_samples[sample].position.y = 0.0f;
                holder->swipe_samples[sample].time = -1.0f;
            }
        }
        holder->touch_position.x = x;
        holder->touch_position.y = y;
        holder->is_down = 1;
        if (holder->sample_countdown >= TimeSampleDelta) {
            holder->sample_countdown -= TimeSampleDelta;
            for (i32 sample = 19; sample > 0; --sample) {
                holder->swipe_samples[sample] = holder->swipe_samples[sample - 1];
            }
            TouchSwipeSample &latest = holder->swipe_samples[0];
            latest.position.x = x;
            latest.position.y = y;
            latest.time = holder->held_time;
            latest.object_position =
                VuVec(object.apiobj.position.x, object.apiobj.position.y, object.apiobj.position.z, 1.0f);
            latest.object_velocity =
                VuVec(object.apiobj.velocity.x, object.apiobj.velocity.y, object.apiobj.velocity.z, 1.0f);
        }
    }
    for (i32 index = 0; index < 10; ++index) {
        TouchHolder &holder = holders[index];
        if (holder.touch_id != -1 && !holder.is_down && holder.field_0x6) {
            holder.consumed = 0;
            holder.touch_id = -1;
            holder.oldest_click_timer = holder.release_timer;
            holder.release_timer = holder.double_click_timer;
            holder.double_click_timer = holder.click_timer;
            holder.click_timer = holder.held_time;
            holder.held_time = 0.0f;
        }
    }
}

void MechInputTouchGestureTrackingSystem::RegisterGestureTracker(MechInputTouchGestureTracker &tracker, i32 priority) {
    GestureTrackerRegistration *entries = trackers;

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
    while (index < 9 && trackers[index].tracker != &tracker)
        ++index;
    if (index == 9)
        return;
    do {
        trackers[index] = trackers[index + 1];
        ++index;
    } while (index != 9);
    trackers[9].tracker = NULL;
    trackers[9].priority = -1;
}

void MechInputTouchGestureTrackingSystem::Update(NuInputTouchData const *data) {
    if (data == NULL) {
        return;
    }
    GameObject_s *object = Player[0];
    if (object == NULL) {
        object = Obj;
    }
    Process(*object, *data);
}

MechInputTouchGestureTrackingSystem::~MechInputTouchGestureTrackingSystem() {
}
