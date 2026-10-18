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
        if (holders[index].touch_id == static_cast<i32>(touch.unknown_14)) {
            return &holders[index];
        }
    }
    for (i32 index = 0; index < 10; ++index) {
        if (holders[index].touch_id == -1) {
            holders[index].touch_id = touch.unknown_14;
            return &holders[index];
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
                if (!(tracker != NULL && tracker->OnDoubleClick(object, holder))) {
                    continue;
                }
                break;
            }
        } else if (first_recent || later_recent) {
            for (i32 priority = 0; priority < 10; ++priority) {
                MechInputTouchGestureTracker *tracker = entries[priority].tracker;
                if (!(tracker != NULL && tracker->OnClick(object, holder))) {
                    continue;
                }
                break;
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
                elapsed += holder.swipe_samples[previous_index].time - sample.time;
                f32 dy = sample.position.y - holder.swipe_samples[previous_index].position.y;
                f32 dx = sample.position.x - holder.swipe_samples[previous_index].position.x;
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
        holder.field_0x6 = holder.is_down;
        holder.is_down = 0;
        holder.held_time += FRAMETIME;
        holder.sample_countdown += FRAMETIME;
    }

    for (i32 index = 0; index < data.touch_count; ++index) {
        f32 x, y;
        MechInputTouchSystem::ConvertToScreenCoords(data.touch_events[index].unknown_04,
                                                    data.touch_events[index].unknown_08, x, y);
        TouchHolder *holder = GetTouch(data.touch_events[index]);
        if (!holder->field_0x6) {
            holder->click_candidate = 0;
            holder->down_position.x = x;
            holder->down_position.y = y;
            holder->sample_countdown = TimeSampleDelta;
            VuVec screen_position(x, y, 1.0f, 1.0f);
            if (player != NULL && NewMode == 0 && NewLData == NULL && GetMenuID() == -1 && Paused == 0 &&
                TouchHacks::TouchControlsActive) {
                MechObjectInterface *target =
                    MechInputTouchSystem::FindTargetObject(object, screen_position, 0x77f, NULL, NULL);
                holder->target_object = target;
            }
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
                TouchSwipeSample &dst = holder->swipe_samples[sample];
                const TouchSwipeSample &src = holder->swipe_samples[sample - 1];
                dst.position = src.position;
                dst.time = src.time;
                dst.object_position.x = src.object_position.x;
                dst.object_position.y = src.object_position.y;
                dst.object_position.z = src.object_position.z;
                dst.object_position.w = src.object_position.w;
                dst.object_velocity.x = src.object_velocity.x;
                dst.object_velocity.y = src.object_velocity.y;
                dst.object_velocity.z = src.object_velocity.z;
                dst.object_velocity.w = src.object_velocity.w;
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

    bool has_vacancy = false;
    for (i32 index = 0; index < 9; ++index) {
        has_vacancy |= entries[index].tracker == NULL;
    }
    if (!has_vacancy && entries[9].tracker != NULL) {
        return;
    }

    i32 insertion_index;
    for (insertion_index = 0; insertion_index < 9; ++insertion_index) {
        if (entries[insertion_index].tracker == NULL || priority < entries[insertion_index].priority) {
            break;
        }
    }
    if (insertion_index == 9) {
        return;
    }
    for (GestureTrackerRegistration *entry = &entries[9]; entry != &entries[insertion_index]; --entry) {
        entry[0] = entry[-1];
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
    GameObject_s *object = player;
    if (object == NULL) {
        object = Obj;
    }
    Process(*object, *data);
}

MechInputTouchGestureTrackingSystem::~MechInputTouchGestureTrackingSystem() {
}
