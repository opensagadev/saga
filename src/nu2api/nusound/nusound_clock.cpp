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
    typedef NuEListOffset<Callback, 4> CallbackList;
    CallbackList::Link *link = callbacks.FrontLink();
    CallbackList::Link *end = callbacks.EndLink();
    for (; link != end; link = CallbackList::NextLink(link)) {
        CallbackList::EntryOf(link)->OnCallback(elapsed, frequency);
    }
    previous_ticks = ticks;
}

NuSoundClock::NuSoundClock() : previous_ticks(0) {
}

void NuSoundClock::RemoveCallback(NuSoundClock::Callback *callback) {
    callbacks.Remove(callback);
}

NuSoundClock::~NuSoundClock() {
    while (callbacks.length != 0) {
        callbacks.PopFront();
    }
}
