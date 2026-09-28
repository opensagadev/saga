#include "decomp.h"
#include "nu2api_nusound_types.h"

void NuSoundClock::Callback::OnCallback(u64, u64) {
}

void NuSoundClock::AddCallback(NuSoundClock::Callback *callback) {
    callbacks.PushBack(callback);
}

u64 NuSoundClock::GetClockFrequency() const {
    return clock_frequency;
}

u64 NuSoundClock::GetTicks() const {
    return 0;
}

void NuSoundClock::HandleCallbacks() {
    u64 ticks = GetTicks();
    u64 frequency = GetClockFrequency();
    u64 elapsed = ticks - previous_ticks;
    for (Callback *callback = callbacks.Front(); callback != callbacks.End(); callback = callback->intrusive_next) {
        callback->OnCallback(elapsed, frequency);
    }
    previous_ticks = ticks;
}

NuSoundClock::NuSoundClock() : previous_ticks(0) {
}

void NuSoundClock::RemoveCallback(NuSoundClock::Callback *callback) {
    NuSoundClock *self = this;
    Callback *next = callback->intrusive_next;
    Callback *previous;
    if (next != NULL) {
        previous = callback->intrusive_prev;
        self->callbacks.length--;
        Callback **next_links = reinterpret_cast<Callback **>(reinterpret_cast<usize>(next) + 4);
        if (previous != NULL) {
            if (next_links == NULL)
                goto clear_previous;
            previous->intrusive_next = next;
        }
        if (next_links != NULL)
            next_links[0] = previous;
        goto clear_entry;
    } else {
        previous = callback->intrusive_prev;
        if (previous == NULL)
            return;
        self->callbacks.length--;
    }
clear_previous:
    previous->intrusive_next = NULL;
clear_entry:
    callback->intrusive_next = NULL;
    callback->intrusive_prev = NULL;
}

NuSoundClock::~NuSoundClock() {
    while (callbacks.length != 0) {
        callbacks.Remove(callbacks.Front());
    }
}
