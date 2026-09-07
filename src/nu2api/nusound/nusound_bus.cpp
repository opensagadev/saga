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
    previous = next = NULL;

    // this->field15_0x3c = (undefined *)&this->field11_0x2c;
    // this->field13_0x34 = (undefined *)&this->field11_0x2c;
    // this->field1_0x4 = (i32 *)0x0;
    // this->field0_0x0 = 0;
    // this->field11_0x2c = 0;
    // this->field14_0x38 = 0;
    // this->field17_0x44 = 0;
    // this->field16_0x40 = &this->field13_0x34;
    // this->field12_0x30 = &this->field13_0x34;

    this->parent_bus = NULL;

    NuStrCpy(this->name, name);
    for (unsigned int i = 0; i < 8; ++i) output_mix[i] = 1.0f;
    // this->field2_0x8 = 0x3f800000;
    // this->field3_0xc = 0x3f800000;
    // this->field4_0x10 = 0x3f800000;
    // this->field5_0x14 = 0x3f800000;
    // this->field6_0x18 = 0x3f800000;
    // this->field7_0x1c = 0x3f800000;
    // this->field8_0x20 = 0x3f800000;
    // this->field9_0x24 = 0x3f800000;

    if (!is_master) {
        this->parent_bus = NuSoundSystem::sMasterBus;
    }
}

NuSoundBus::NuSoundBus(const char *name, NuSoundBus *parent) {
    previous = next = NULL;

    this->parent_bus = parent;
    NuStrCpy(this->name, name);
    for (unsigned int i = 0; i < 8; ++i) output_mix[i] = 1.0f;
    if (parent == NULL) parent_bus = NuSoundSystem::sMasterBus;
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
