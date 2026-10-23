// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#if defined(__SSE__)
#include <xmmintrin.h>
#endif
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/actions/combat/hits.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include "decomp.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "globals.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/world/mission.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nupostparams.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"
#include "legoapi/world/world.h"
#include "legoapi/misc/supportall_types.h"
#include "legoapi/items/collect/torpedo.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edui.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/screens/store.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/core/input/timer.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nucore/nuhgobj.h"
#include "legoapi/items/objects/gameobjects.h"

MemoryManager theMemoryManager;
BIGBUFFER_s BigBuffer;
void *theThingManager = NULL;

void bgProcClose() {
}

void CloakMovement(GameObject_s *object) {
    const i32 index = object->field_0x1089;
    if (index > 2) {
        return;
    }
    GAMECHARACTERDATA_s *character = object->apiobj.character_data->game_character;
    if (character->cloak_joint == -1) {
        return;
    }
    u8 *state = reinterpret_cast<u8 *>(object) + index * 0x34;
    f32 &value = *reinterpret_cast<f32 *>(state + 0xf50);
    f32 phase = NuFsqrt((value - character->field_0x60) / (character->field_0x5c - character->field_0x60));
    f32 low_weight;
    f32 high_weight;
    if (LEGOACT_FALL != -1) {
        i16 animation;
        if (__builtin_expect(object->apiobj.anim_packet.blending == 0, 0)) {
            animation = object->apiobj.anim_packet.animation_index;
        } else {
            animation = object->apiobj.anim_packet.blend_animation_b;
        }
        if (animation == LEGOACT_FALL) {
            phase = FRAMETIME + FRAMETIME + phase;
            if (phase > 1.0f) {
                high_weight = 1.0f;
                low_weight = 0.0f;
            } else {
                high_weight = phase * phase;
                low_weight = 1.0f - high_weight;
            }
            goto apply_weights;
        }
    }

    high_weight = 0.0f;
    phase = phase - (FRAMETIME + FRAMETIME);
    low_weight = 1.0f;
    if (!(phase < 0.0f)) {
        high_weight = phase * phase;
        low_weight = 1.0f - high_weight;
    }

apply_weights:
    const u8 current_index = object->field_0x1089;
    const u32 state_index = current_index;
    state = reinterpret_cast<u8 *>(object) + state_index * 0x34;
    CHARACTERDATA *character_data = object->apiobj.character_data;
    state[0xf78] = static_cast<u8>(character_data->game_character->cloak_joint);
    GAMECHARACTERDATA_s *current_character = character_data->game_character;
    const f32 interpolated_value =
        high_weight * current_character->field_0x5c + low_weight * current_character->field_0x60;
    *reinterpret_cast<f32 *>(state + 0xf50) = interpolated_value;
    if (interpolated_value != 0.0f) {
        state[0xf79] = 0x21;
        if (current_character->field_0x60 <= current_character->field_0x5c) {
            *reinterpret_cast<i16 *>(state + 0xf70) = static_cast<i16>(current_character->field_0x5c * 32768.0f);
            *reinterpret_cast<i16 *>(state + 0xf76) = static_cast<i16>(current_character->field_0x60 * 32768.0f);
        } else {
            *reinterpret_cast<i16 *>(state + 0xf70) = static_cast<i16>(current_character->field_0x60 * 32768.0f);
            *reinterpret_cast<i16 *>(state + 0xf76) = static_cast<i16>(current_character->field_0x5c * 32768.0f);
        }
    } else {
        state[0xf79] = 0;
    }
    object->field_0x1089 = current_index + 1;
}

// Debug-capture output helpers consumed by NuDisplayListCaptureSortPriority.
// Transcribed from the original C-linkage symbols:
//   NuHtmlBegin    0x2d5ca0   NuHtmlFlush   0x2d5c30
//   NuHtmlWrite    0x2d5cd0   NuHtmlHeading1 0x2d5d40
extern "C" {
extern NUFILE hfh;
}

extern i16 id_GAMORREANGUARD;

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

extern i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER;

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

static eduimenu_s *edTimingMenu;
static u32 EdAttr[] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};

// BaseThing defaults @0x424bf0 (dtor) and 0x425990..0x425a20 (interface
// defaults); RemoveDependancies returns 1, the rest are no-ops. GetName holds
// a 0 slot in the original base vtable (pure) — see basething.h.
BaseThing::~BaseThing() {
}

// ThingManager::AddThing @0x424c10. Appends at count; the pending
// AddThingAfterThis reservation (field_0x14) is folded into the index and
// cleared here.
void ThingManager::AddThing(BaseThing *thing) {
    i32 index = this->count;
    if (thing != NULL) {
        if (index < this->max_things) {
            this->things[index] = thing;
            index = index + 1;
        }
    }
    index = index + this->field_0x14;
    this->field_0x14 = 0;
    this->count = index;
}

// ThingManager::AddThingAfterThis @0x424c40. Reserves the slot after the
// current tail: bumps field_0x14 and stores the thing at count+field_0x14;
// the next AddThing folds the reservation into count.
void ThingManager::AddThingAfterThis(BaseThing *thing) {
    if (thing != NULL) {
        i32 index = this->field_0x14 + 1;
        this->field_0x14 = index;
        index = index + this->count;
        if (index < this->max_things) {
            this->things[index] = thing;
        }
    }
}

void ThingManager::RemoveTemporaryThings() {
    i32 i = this->count - 1;
    if (i >= this->permanent_count) {
        do {
            delete this->things[i];
            this->things[i] = NULL;
            --i;
        } while (i >= this->permanent_count);
    }
    this->count = this->permanent_count;
}

i32 ThingManager::RemoveDependanciesThings(ThingRemoveData *data) {
    i32 removed = 1;
    for (i32 i = 0; i < this->count; ++i) {
        if (this->things[i] == NULL || (this->things[i]->flags & THING_FLAG_SKIP_REMOVE_DEPENDANCIES) != 0) {
            continue;
        }
        removed &= this->things[i]->RemoveDependancies(data);
    }
    return removed;
}

void ThingManager::EnterLevelThings(ThingLevelData *data) {
    for (i32 i = 0; i < this->count; ++i) {
        if (this->things[i] == NULL || (this->things[i]->flags & THING_FLAG_SKIP_ENTER_LEVEL) != 0) {
            continue;
        }
        this->things[i]->EnterLevel(data);
    }
}

void ThingManager::ExitLevelThings(ThingLevelData *data) {
    for (i32 i = 0; i < this->count; ++i) {
        if (this->things[i] == NULL || (this->things[i]->flags & THING_FLAG_SKIP_EXIT_LEVEL) != 0) {
            continue;
        }
        this->things[i]->ExitLevel(data);
    }
}

void ThingManager::edTimingInit() {
    static_cast<ThingManager *>(theThingManager)->ed_timing_state = 0;
}

void ThingManager::edTimingRender() {
    eduiMenuRender(edTimingMenu);
}

i32 ThingManager::edTimingProc(float delta_time, nupad_s *pad) {
    NuFntSet(0);
    NuFntScale(12, 12);
    eduiMenuProcess(edTimingMenu, delta_time, pad);
    return edTimingMenu == NULL ? 2 : 0;
}

void ThingManager::edTimingEnter() {
    edTimingMenu = eduiMenuCreate(30, 30, 250, 320, NULL, cbEdTrackCancel, "Timings");
    if (edTimingMenu != NULL) {
        eduiMenuAddItem(edTimingMenu, eduiItemSelCreate(0, EdAttr, 0, 1, cbEdTimingSelect, "Off"));
        eduiMenuAddItem(edTimingMenu, eduiItemSelCreate(1, EdAttr, 0, 2, cbEdTimingSelect, "System"));
        eduiMenuAddItem(edTimingMenu, eduiItemSelCreate(2, EdAttr, 0, 3, cbEdTimingSelect, "No Things"));
        eduiMenuAddItem(edTimingMenu, eduiItemSelCreate(3, EdAttr, 0, 4, cbEdTimingSelect, "All Things"));
        for (i32 i = 0; i < static_cast<ThingManager *>(theThingManager)->count; ++i) {
            eduiMenuAddItem(
                edTimingMenu,
                eduiItemToggleCreate(
                    i + 4, EdAttr, static_cast<ThingManager *>(theThingManager)->things[i]->profiling_0xc, i + 5,
                    cbEdTimingSelect,
                    const_cast<char *>(static_cast<ThingManager *>(theThingManager)->things[i]->GetName())));
        }
    }
}

void ThingManager::cbEdTrackCancel(eduimenu_s *menu, eduimenu_s *) {
    eduiMenuDestroy(menu);
    edTimingMenu = NULL;
}

void ThingManager::cbEdTimingSelect(eduimenu_s *menu, eduiitem_s *item, u32) {
    ThingManager *manager = static_cast<ThingManager *>(theThingManager);
    u32 selection = item->data;
    manager->ed_timing_state = selection;
    manager->field_0x1c = selection - 3;
    if (selection <= 3) {
        i32 enabled = selection == 3;
        for (i32 i = 0; i < manager->count; ++i) {
            manager->things[i]->profiling_0xc = enabled;
        }
        if (!(selection <= 1)) {
            i32 index = 0;
            for (eduiitem_s *entry = menu->first; entry != NULL; entry = entry->next) {
                if (index < 3) {
                    entry->highlighted = 0;
                } else {
                    entry->highlighted = enabled;
                }
                ++index;
            }
        } else {
            eduiMenuDestroy(menu);
            edTimingMenu = NULL;
        }
        NuTimeBarEnable(item->data != 0);
    } else {
        manager->things[selection - 4]->profiling_0xc ^= 1;
    }
}

void ThingManager::EffectsThings(ThingRenderData *data) {
    const char *name = "Fx";
    for (i32 i = 0; i < this->count; ++i) {
        if (this->things[i] == NULL || (this->things[i]->flags & THING_FLAG_SKIP_EFFECTS) != 0) {
            continue;
        }
        if (__builtin_expect(this->things[i]->profiling_0xc != 0, 1)) {
            _NuTimeBarSlotBegin(this->timebar, 5, name);
        }
        this->things[i]->Effects(data);
        if (this->things[i]->profiling_0xc != 0) {
            _NuTimeBarSlotEnd(this->timebar, 5);
        }
    }
}

// ThingManager::DisplayThings @0x4252c0. Single pass over Display,
// bracketed with timebar slot 3 ("Dis"). PanelRender uses this pass for the
// display-layer things that render on top of the gameplay panel.
void ThingManager::DisplayThings(ThingRenderData *data) {
    const char *name = "Dis"; // timebar slot name @0x5734db

    for (i32 i = 0; i < this->count; ++i) {
        BaseThing *thing = this->things[i];
        if (thing != NULL && (thing->flags & THING_FLAG_SKIP_DISPLAY) == 0) {
            if (__builtin_expect(thing->profiling_0xc != 0, 1)) {
                _NuTimeBarSlotBegin(this->timebar, 3, name);
                thing = this->things[i];
            }
            thing->Display(data);
            thing = this->things[i];
            if (thing->profiling_0xc != 0) {
                _NuTimeBarSlotEnd(this->timebar, 3);
            }
        }
    }
}

// ThingManager::RenderThings @0x425390. Single pass over Render,
// bracketed with timebar slot 1 ("Rnd").
void ThingManager::RenderThings(ThingRenderData *data) {
    const char *name = "Rnd"; // timebar slot name @0x5734df

    for (i32 i = 0; i < this->count; ++i) {
        BaseThing *thing = this->things[i];
        if (thing != NULL && (thing->flags & THING_FLAG_SKIP_RENDER) == 0) {
            if (__builtin_expect(thing->profiling_0xc != 0, 1)) {
                _NuTimeBarSlotBegin(this->timebar, 1, name);
                thing = this->things[i];
            }
            thing->Render(data);
            thing = this->things[i];
            if (thing->profiling_0xc != 0) {
                _NuTimeBarSlotEnd(this->timebar, 1);
            }
        }
    }
}

// ThingManager::ProcessThings @0x425460. Pass 1 always runs
// ProcessEvenWhenPaused first; then, per ThingProcessData.paused, either
// Process or ProcessOnlyWhenPaused. Each pass has its own opt-out flag. The count
// is re-read every iteration because thing Process calls may add things.
// Profiling flags select which things receive NuTimeBarSlotBegin/End calls.
void ThingManager::ProcessThings(ThingProcessData *data) {
    const char *name = "PROC";
    for (i32 i = 0; i < this->count; ++i) {
        if (this->things[i] != NULL && (this->things[i]->flags & THING_FLAG_SKIP_PROCESS_EVEN_WHEN_PAUSED) == 0) {
            if (__builtin_expect(this->things[i]->profiling_0xc != 0, 1)) {
                _NuTimeBarSlotBegin(this->timebar, 0, name);
            }
            this->things[i]->ProcessEvenWhenPaused(data);
            if (this->things[i]->profiling_0xc != 0) {
                _NuTimeBarSlotEnd(this->timebar, 0);
            }
        }
    }
    if (data->paused == 0) {
        for (i32 i = 0; i < this->count; ++i) {
            if (this->things[i] != NULL && (this->things[i]->flags & THING_FLAG_SKIP_PROCESS) == 0) {
                if (__builtin_expect(this->things[i]->profiling_0xc != 0, 0)) {
                    _NuTimeBarSlotBegin(this->timebar, 0, name);
                }
                this->things[i]->Process(data);
                if (this->things[i]->profiling_0xc != 0) {
                    _NuTimeBarSlotEnd(this->timebar, 0);
                }
            }
        }
    } else {
        for (i32 i = 0; i < this->count; ++i) {
            if (this->things[i] != NULL && (this->things[i]->flags & THING_FLAG_SKIP_PROCESS_ONLY_WHEN_PAUSED) == 0) {
                if (__builtin_expect(this->things[i]->profiling_0xc != 0, 1)) {
                    _NuTimeBarSlotBegin(this->timebar, 0, name);
                }
                this->things[i]->ProcessOnlyWhenPaused(data);
                if (this->things[i]->profiling_0xc != 0) {
                    _NuTimeBarSlotEnd(this->timebar, 0);
                }
            }
        }
    }
}

void ThingManager::ResetThings(ThingResetData *data) {
    const char *name = "Res";

    for (i32 i = 0; i < this->count; ++i) {
        if (this->things[i] != NULL && (this->things[i]->flags & 8) == 0) {
            if (__builtin_expect(this->things[i]->profiling_0xc != 0, 1)) {
                _NuTimeBarSlotBegin(this->timebar, 4, name);
            }
            this->things[i]->Reset(data);
            if (this->things[i]->profiling_0xc != 0) {
                _NuTimeBarSlotEnd(this->timebar, 4);
            }
        }
    }
}

// MemoryManager::AllocPool @0x425740.
//
// The manager is initialized by LoadPermData. Its pointer-bearing fields use
// native pointer width while retaining the original layout in the 32-bit build.
// Pools are 32-byte classes; a freed block is pushed on its class list and
// recycled by the pop path below (memset of the requested size only), while a
// fresh carve rounds up to the full 32-byte block and updates the counters.
void *MemoryManager::AllocPool(u32 size, i32 zero) {
    const i32 adjusted_size =
        static_cast<i32>(size - 1) < 0 ? static_cast<i32>(size + 0x1e) : static_cast<i32>(size - 1);
    const i32 size_class = adjusted_size >> 5;

    void **free_head = &free_lists[size_class];
    void *item = *free_head;
    if (item != NULL) {
        *free_head = *reinterpret_cast<void **>(item);
        if (zero != 0) {
            memset(item, 0, size);
        }
        return item;
    }

    const usize block = static_cast<usize>(size_class + 1) << 5;
    if (*end_cell - *cursor_cell <= block) {
        return NULL;
    }
    const usize aligned = ALIGN(*cursor_cell, 0x10);
    *cursor_cell = aligned;
    item = reinterpret_cast<void *>(aligned);
    *cursor_cell = aligned + block;
    memset(item, 0, block);
    allocated += block;
    remaining -= block;
    high_water = *cursor_cell;
    return item;
}

// MemoryManager::FreePool @0x425810: pushes the block onto its size class's
// free list (the next AllocPool of that class pops it, first word = next).
void MemoryManager::FreePool(void *item, u32 size) {
    const i32 adjusted_size =
        static_cast<i32>(size - 1) < 0 ? static_cast<i32>(size + 0x1e) : static_cast<i32>(size - 1);
    const i32 size_class = adjusted_size >> 5;

    void **free_head = &free_lists[size_class];
    *reinterpret_cast<void **>(item) = *free_head;
    *free_head = item;
}

// BaseThing::BaseThing @0x425840 zeroes the data fields after the vptr.
BaseThing::BaseThing() {
    this->field_0x4 = 0;
    this->flags = 0;
    this->profiling_0xc = 0;
}

// ThingManager::ThingManager @0x425870. Stores the manager in theThingManager
// and carves the thing-pointer array from the
// theMemoryManager linear pool. On allocation failure the array is NULL — the
// manager then simply never accepts things (AddThing's count < max check).
ThingManager::ThingManager(i32 max_things) {
    const usize need = static_cast<usize>(max_things) * sizeof(*things);

    BaseThing **array = NULL;
    if (*theMemoryManager.end_cell - *theMemoryManager.cursor_cell > need) {
        const usize aligned = ALIGN(*theMemoryManager.cursor_cell, 0x10);
        *theMemoryManager.cursor_cell = aligned + need;
        array = reinterpret_cast<BaseThing **>(aligned);
        memset(array, 0, need);
        theMemoryManager.allocated += need;
        theMemoryManager.remaining -= need;
        theMemoryManager.high_water = *theMemoryManager.cursor_cell;
    }
    this->things = array;
    this->max_things = max_things;
    // Profiling sets are a deferred subsystem; the handle is only ever passed
    // to the NuTimeBarSlotBegin/End stubs, so NULL behaves like the original
    // with profiling disabled.
    this->timebar = NuTimeBarCreateSet(0);
    theThingManager = this;
}

// ThingManager::EnableActions @0x425930. Finds the first thing whose 0x4 id
// matches and sets (invert==0) or clears (invert!=0) the given flags bits.
void ThingManager::EnableActions(i32 id, i32 flags, i32 invert) {
    i32 count = this->count;
    if (count <= 0) {
        return;
    }
    for (i32 i = 0; i < count; i++) {
        BaseThing *thing = this->things[i];
        if (thing == NULL) {
            continue;
        }
        if (thing->field_0x4 == (u32)id) {
            if (invert != 0) {
                thing->flags &= ~(u32)flags;
            } else {
                thing->flags |= (u32)flags;
            }
            return;
        }
    }
}

