#include "nu2api_nusound_types.h"

void NuSoundClock::AddCallback(NuSoundClock::Callback *callback) {
    Links *tail_links = GetLinks(tail);
    Callback *previous = tail_links->prev;
    tail_links->prev = callback;
    callback->prev = previous;
    GetLinks(previous)->next = callback;
    callback->next = tail;
    ++callback_count;
}

u64 NuSoundClock::GetClockFrequency() const {
    return frequency;
}

u64 NuSoundClock::GetTicks() const {
    return 0;
}

void NuSoundClock::HandleCallbacks() {
    u64 ticks = GetTicks();
    u64 clock_frequency = GetClockFrequency();
    u64 elapsed = ticks - last_ticks;
    for (Callback *callback = GetLinks(head)->next; callback != tail; callback = callback->next) {
        callback->OnCallback(elapsed, clock_frequency);
    }
    last_ticks = ticks;
}

NuSoundClock::NuSoundClock() {
    head = reinterpret_cast<Callback *>(reinterpret_cast<char *>(&start) - sizeof(void *));
    tail = reinterpret_cast<Callback *>(reinterpret_cast<char *>(&end) - sizeof(void *));
    start.prev = NULL;
    start.next = tail;
    end.prev = head;
    end.next = NULL;
    callback_count = 0;
    last_ticks = 0;
}

void NuSoundClock::RemoveCallback(NuSoundClock::Callback *callback) {
    if (callback->next != NULL || callback->prev != NULL) {
        --callback_count;
        Callback *next = callback->next;
        Callback *prev = callback->prev;
        if (prev != NULL) GetLinks(prev)->next = next;
        if (next != NULL) GetLinks(next)->prev = prev;
        callback->next = NULL;
        callback->prev = NULL;
    }
}

NuSoundClock::~NuSoundClock() {
    while (callback_count != 0) RemoveCallback(GetLinks(head)->next);
}
