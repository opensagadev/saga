#include "nu2api/nusound/nusound_bus.hpp"

#include "nu2api/nucore/nustring.h"
#include "nu2api/nusound/nusound_system.hpp"

#include <string.h>

const char *NuSoundBus::GetName() {
    return this->name;
}

NuSoundBus *NuSoundSystem::GetBus(const char *name) {
    for (NuSoundBus *bus = bus_list.Front(); bus != bus_list.End(); bus = reinterpret_cast<NuSoundBus **>(bus)[1]) {
        if (NuStrICmp(bus->GetName(), name) == 0) {
            return bus;
        }
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

NuSoundBus::~NuSoundBus() {
    NuList<NuSoundEffect *> &effects = *reinterpret_cast<NuList<NuSoundEffect *> *>(&this->effect_begin_prev);
    effects.~NuList<NuSoundEffect *>();
}

bool NuSoundBus::AddEffect(NuSoundEffect *effect) {
    NuList<NuSoundEffect *> &effects = *reinterpret_cast<NuList<NuSoundEffect *> *>(&this->effect_begin_prev);

    for (NuListNodeBase *node = effects.Head(); node != effects.Tail(); node = node->GetNext()) {
        if (static_cast<NuListNode<NuSoundEffect *> *>(node)->value == effect) {
            return false;
        }
    }

    bool attached = effect->AttachBus(this);
    if (attached) {
        NuSoundMemory::PushNuListNode(effects, effect);
    }
    return attached;
}

void NuSoundBus::RemoveEffect(NuSoundEffect *effect) {
    NuList<NuSoundEffect *> &effects = *reinterpret_cast<NuList<NuSoundEffect *> *>(&this->effect_begin_prev);

    bool found = false;
    for (NuListNodeBase *node = effects.Head(); node != effects.Tail(); node = node->GetNext()) {
        if (static_cast<NuListNode<NuSoundEffect *> *>(node)->value == effect) {
            found = true;
            break;
        }
    }
    if (!found) {
        return;
    }

    effect->DetachBus(this);
    NuListNodeBase *node = effects.Head();
    while (node != effects.Tail()) {
        NuListNodeBase *next = node->GetNext();
        if (static_cast<NuListNode<NuSoundEffect *> *>(node)->value == effect) {
            effects.Remove(node);
        }
        node = next;
    }
}

void NuSoundBus::ApplyFinalMix(float *mix) {
    NuSoundBus *bus = this;
    do {
        f32 effect_mix = 1.0f;
        NuListNodeBase *node = reinterpret_cast<NuList<NuSoundEffect *> *>(&bus->effect_begin_prev)->Head();
        NuListNodeBase *end = reinterpret_cast<NuList<NuSoundEffect *> *>(&bus->effect_begin_prev)->Tail();
        while (node != end) {
            NuSoundEffect *effect = static_cast<NuListNode<NuSoundEffect *> *>(node)->value;
            effect_mix *= effect->output_mix;
            node = node->GetNext();
        }

        for (i32 i = 0; i < 8; i++) {
            mix[i] *= bus->output_mix[i] * effect_mix;
        }
        bus = bus->parent_bus;
    } while (bus != NULL);
}

void NuSoundBus::GetOutputMix(float *mix) {
    memmove(mix, this->output_mix, sizeof(this->output_mix));
}

void NuSoundBus::SetOutputMix(float mix) {
    for (i32 i = 0; i < 8; i++) {
        this->output_mix[i] = mix;
    }
}

void NuSoundBus::SetOutputMix(float *mix) {
    memmove(this->output_mix, mix, sizeof(this->output_mix));
}

void NuSoundBus::SetOutputBus(NuSoundBus *bus) {
    this->parent_bus = bus != NULL ? bus : NuSoundSystem::sMasterBus;
}
