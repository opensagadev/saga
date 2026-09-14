#include "legoapi/world/world.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include <string.h>
#include "legoapi/core/input/qrand.h"
#include "globals.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/render/fx.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/world/level.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/world/area.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/gamemessage.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/audio/audio.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "nu2api/nucore/nustring.h"

#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/gizmo/object/giztorpedo.h"
#include "legoapi/items/collect/torpedo.h"

void Tube_MoveCode(GameObject_s *object, WORLDINFO_s *world) {
    if (LEGOCONTEXT_GLIDE != -1 && object->character_context == LEGOCONTEXT_GLIDE && object->field_0x788 != NULL) {
        if (Tube_InCylinder(object, static_cast<TUBE *>(object->field_0x788), NULL, 0) == 0)
            object->field_0x788 = NULL;
        return;
    }

    if (LEGOCONTEXT_TUBE == -1)
        return;
    if (object->character_context == LEGOCONTEXT_TUBE) {
        if (Tube_InCylinder(object, static_cast<TUBE *>(object->field_0x788), NULL, 0) == 0)
            object->character_context = -1;
        return;
    }
    if ((CInfo[object->character_context].flags & 0x4000) != 0 || world->tubes == NULL)
        return;

    for (i32 index = 0; index < world->tube_count; ++index) {
        TUBE *tube = &world->tubes[index];
        if ((tube->flags & (TUBE_FLAG_ACTIVE | TUBE_FLAG_VISIBLE | TUBE_FLAG_DIRECTIONAL)) !=
            (TUBE_FLAG_ACTIVE | TUBE_FLAG_VISIBLE)) {
            continue;
        }
        if (Tube_InCylinder(object, tube, NULL, 0) == 0 &&
            (object->tube_entry_state != 5 || object->tube_entry_data == NULL)) {
            continue;
        }

        if (LEGOCONTEXT_GLIDE != -1 && object->character_context == LEGOCONTEXT_GLIDE) {
            object->field_0x788 = tube;
            return;
        }
        object->field_0x788 = tube;
        object->field_0xe31 = 0;
        object->character_context = LEGOCONTEXT_TUBE;
        if (static_cast<i8>(object->apiobj.flags_low) < 0 && tube->audio_cooldown == 0.0f) {
            tube->audio_cooldown = 4.0f;
            GameAudio_PlaySfx(6, &object->apiobj.collision_position, 0, 0);
        }
        return;
    }
}

void Tube_SetObjBit(TUBE *tube, i32 object_index) {
    tube->occupied_object_masks[object_index / 32] |= 1U << object_index;
}

TUBE *Tube_FindByName(WORLDINFO_s *world, char *name) {
    if (world == NULL || world->tubes == NULL)
        return NULL;
    for (i32 index = 0; index < world->tube_count; ++index) {
        if (NuStrICmp(world->tubes[index].name, name) == 0)
            return &world->tubes[index];
    }
    return NULL;
}

i32 Tube_InCylinder(GameObject_s *object, TUBE *tube, f32 *horizontal_distance_squared, i32 ignore_height) {
    if (tube == NULL || object == NULL) {
        return 0;
    }

    if (ignore_height == 0) {
        if (tube->position.y > object->apiobj.collision_max.y || object->apiobj.collision_min.y > tube->top) {
            return 0;
        }
    }

    const f32 delta_x = object->apiobj.collision_position.x - tube->position.x;
    const f32 delta_z = object->apiobj.collision_position.z - tube->position.z;
    const f32 distance_squared = delta_x * delta_x + delta_z * delta_z;

    f32 radius_squared = tube->radius_squared;
    if ((tube->flags & TUBE_FLAG_TOUCH_RADIUS) != 0 && TouchHacks::TouchControlsActive) {
        radius_squared *= 0.8f;
    }

    if (distance_squared > radius_squared) {
        return 0;
    }
    if (horizontal_distance_squared != NULL) {
        *horizontal_distance_squared = distance_squared;
    }
    return 1;
}

i32 Tube_IsObjBitSet(TUBE *tube, i32 object_index) {
    return tube->occupied_object_masks[object_index / 32] >> object_index & 1;
}

TUBE *Tube_InAnyCylinder(WORLDINFO_s *world, GameObject_s *object, i32 ignore_height) {
    TUBE *tube = world->tubes;
    if (tube != NULL) {
        for (i32 index = 0; index < world->tube_count; ++index, ++tube) {
            if ((tube->flags & (TUBE_FLAG_ACTIVE | TUBE_FLAG_VISIBLE | TUBE_FLAG_DIRECTIONAL)) ==
                    (TUBE_FLAG_ACTIVE | TUBE_FLAG_VISIBLE) &&
                Tube_InCylinder(object, tube, NULL, ignore_height) != 0) {
                return tube;
            }
        }
    }
    return NULL;
}
