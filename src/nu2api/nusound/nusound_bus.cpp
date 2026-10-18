#include "nu2api/nusound/nusound_bus.hpp"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nusound/nusound_system.hpp"
#include <string.h>
const char *NuSoundBus::GetName() {
    return this->name;
}

NuSoundBus::NuSoundBus(const char *name, bool is_master)
    : intrusive_prev(NULL), intrusive_next(NULL), parent_bus(NULL) {

    NuStrCpy(this->name, name);
    for (i32 i = 0; i < 8; i++) {
        output_mix[i] = 1.0f;
    }

    if (!is_master) {
        parent_bus = NuSoundSystem::sMasterBus;
    }
}

NuSoundBus::NuSoundBus(const char *name, NuSoundBus *parent)
    : intrusive_prev(NULL), intrusive_next(NULL), parent_bus(parent) {

    NuStrCpy(this->name, name);
    for (i32 i = 0; i < 8; i++) {
        output_mix[i] = 1.0f;
    }

    if (parent == NULL) {
        parent_bus = NuSoundSystem::sMasterBus;
    }
}

NuSoundBus::~NuSoundBus() {
}

bool NuSoundBus::AddEffect(NuSoundEffect *effect) {
    if (effects.Contains(effect)) {
        return false;
    }

    bool attached = effect->AttachBus(this);
    if (attached) {
        NuSoundMemory::PushNuListNode(effects, effect);
    }
    return attached;
}

void NuSoundBus::RemoveEffect(NuSoundEffect *effect) {
    if (!effects.Contains(effect)) {
        return;
    }

    effect->DetachBus(this);
    if (effects.Length() != 0) {
        effects.RemoveValue(effect);
    }
    NuSoundSystem::sAllocdMemory[static_cast<i32>(NuSoundSystem::MemoryDiscipline::SCRATCH)] -=
        sizeof(NuListNode<NuSoundEffect *>);
}

void NuSoundBus::ApplyFinalMix(float *mix) {
    NuSoundBus *bus = this;
    do {
        f32 effect_mix = 1.0f;
        NuListNodeBase *node = bus->effects.Head();
        NuListNodeBase *end = bus->effects.Tail();
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
