#include "nu2api/nusound/nusound_bus.hpp"

#include "nu2api/nucore/nustring.h"
#include "nu2api/nusound/nusound_system.hpp"
#include <string.h>

const char *NuSoundBus::GetName() {
    return this->name;
}

NuSoundBus *NuSoundSystem::GetBus(const char *name) {
    for (NuSoundBus *bus = bus_head->next; bus != bus_tail; bus = bus->next) {
        if (NuStrICmp(bus->GetName(), name) == 0) return bus;
    }
    return NULL;
}

NuSoundBus::NuSoundBus(const char *name, bool is_master) {
    effect_begin = &effect_begin_prev;
    effect_end_prev = &effect_begin_prev;
    intrusive_next = NULL;
    intrusive_prev = NULL;
    effect_begin_prev = NULL;
    effect_end_next = NULL;
    effect_count = 0;
    effect_end = &effect_end_prev;
    effect_begin_next = &effect_end_prev;
    parent_bus = NULL;

    NuStrCpy(this->name, name);
    for (i32 i = 0; i < 8; i++) {
        output_mix[i] = 1.0f;
    }

    if (!is_master) {
        parent_bus = NuSoundSystem::sMasterBus;
    }
}

NuSoundBus::NuSoundBus(const char *name, NuSoundBus *parent) {
    effect_begin = &effect_begin_prev;
    effect_end_prev = &effect_begin_prev;
    intrusive_next = NULL;
    intrusive_prev = NULL;
    effect_begin_prev = NULL;
    effect_end_next = NULL;
    effect_count = 0;
    effect_end = &effect_end_prev;
    effect_begin_next = &effect_end_prev;
    parent_bus = parent;

    NuStrCpy(this->name, name);
    for (i32 i = 0; i < 8; i++) {
        output_mix[i] = 1.0f;
    }

    if (parent == NULL) {
        parent_bus = NuSoundSystem::sMasterBus;
    }
}

NuSoundBus::~NuSoundBus() {}

bool NuSoundBus::AddEffect(NuSoundEffect *effect) {
    if (effects.length != 0) {
        NuListNodeBase *last = effects.tail->GetPrev();
        NuListNodeBase *node = effects.Head();
        for (;;) {
            if (static_cast<NuListNode<NuSoundEffect *> *>(node)->value == effect) return false;
            if (node == last) break;
            node = node->GetNext();
        }
    }
    bool attached = effect->AttachBus(this);
    if (attached) NuSoundMemory::PushNuListNode(effects, effect);
    return attached;
}

void NuSoundBus::RemoveEffect(NuSoundEffect *effect) {
    NuListNodeBase *found = effects.Head();
    while (found != effects.tail && static_cast<NuListNode<NuSoundEffect *> *>(found)->value != effect) found = found->next;
    if (found == effects.tail) return;
    effect->DetachBus(this);
    if (effects.length != 0) {
        NuListNodeBase *last = effects.tail->prev;
        NuListNodeBase *node = effects.Head();
        unsigned int removed = 0;
        for (;;) {
            if (static_cast<NuListNode<NuSoundEffect *> *>(node)->value == effect) {
                bool is_last = node == last;
                NuListNodeBase *next = node->next;
                NuListNodeBase *previous = node->prev;
                if (previous != NULL) previous->next = next;
                if (next != NULL) next->prev = previous;
                NuMemoryGet()->GetThreadMem()->BlockFree(node, 0);
                ++removed;
                if (is_last) break;
                node = next;
                if (node == last) break;
                // The original advances again after removing a non-final node.
                node = node->next;
            } else {
                if (node == last) break;
                node = node->next;
            }
        }
        effects.length -= removed;
    }
    NuSoundSystem::sAllocdMemory[0] -= sizeof(NuListNode<NuSoundEffect *>);
}

void NuSoundBus::ApplyFinalMix(float *mix) {
    NuSoundBus *bus = this;
    do {
        float attenuation = 1.0f;
        for (NuListNodeBase *node = bus->effects.Head(); node != bus->effects.tail; node = node->next) {
            attenuation *= static_cast<NuListNode<NuSoundEffect *> *>(node)->value->attenuation;
        }
        for (unsigned int i = 0; i < 8; ++i) mix[i] *= bus->output_mix[i] * attenuation;
        bus = bus->parent_bus;
    } while (bus != NULL);
}

void NuSoundBus::GetOutputMix(float *mix) {
    memmove(mix, output_mix, sizeof(output_mix));
}

void NuSoundBus::SetOutputMix(float mix) {
    for (unsigned int i = 0; i < 8; ++i) output_mix[i] = mix;
}

void NuSoundBus::SetOutputMix(float *mix) {
    memmove(output_mix, mix, sizeof(output_mix));
}

void NuSoundBus::SetOutputBus(NuSoundBus *bus) {
    parent_bus = bus != NULL ? bus : NuSoundSystem::sMasterBus;
}
