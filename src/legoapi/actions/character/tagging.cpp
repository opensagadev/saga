#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/core/input/qrand.h"

#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/world/world.h"
#include "gameapi/gui/apimenu.h"

struct TAGTRANSFER_s {
    GameObject_s *source;
    NUVEC position[3];
    f32 height[3];
    f32 time;
};
DECOMP_ASSERT(sizeof(TAGTRANSFER_s) == 0x38, "tag transfer size");
static TAGTRANSFER_s Tag_Transfer[2];

static const f32 Tag_TransferResetTimer = 0.5f;

void TagCharacter(GameObject_s *, GameObject_s *, i32) {
}

void Tag_UpdateHint(HINT_s *) {
}

void Tag_NewTransfer(GameObject_s *source, GameObject_s *target) {
    const i8 player_index = target->apiobj.field_0x27c;
    if (static_cast<u8>(player_index) < 2) {
        Tag_Transfer[player_index].time = 0.0f;
        Tag_Transfer[player_index].source = source;
        Tag_Transfer[player_index].height[0] = static_cast<f32>(qrand()) * 1.5259022e-05f * 0.4f + 0.4f;
        TAGTRANSFER_s &first = Tag_Transfer[target->apiobj.field_0x27c];
        first.position[0].x = source->apiobj.collision_position.x;
        first.position[0].z = source->apiobj.collision_position.z;
        first.position[0].y = first.height[0] * (source->apiobj.collision_max.y - source->apiobj.collision_min.y) +
                              source->apiobj.collision_min.y;

        const i8 second_index = target->apiobj.field_0x27c;
        Tag_Transfer[second_index].height[1] = static_cast<f32>(qrand()) * 1.5259022e-05f * 0.4f + 0.4f;
        TAGTRANSFER_s &second = Tag_Transfer[target->apiobj.field_0x27c];
        second.position[1].x = source->apiobj.collision_position.x;
        second.position[1].z = source->apiobj.collision_position.z;
        second.position[1].y = second.height[1] * (source->apiobj.collision_max.y - source->apiobj.collision_min.y) +
                               source->apiobj.collision_min.y;

        const i8 third_index = target->apiobj.field_0x27c;
        Tag_Transfer[third_index].height[2] = static_cast<f32>(qrand()) * 1.5259022e-05f * 0.4f + 0.4f;
        TAGTRANSFER_s &third = Tag_Transfer[target->apiobj.field_0x27c];
        third.position[2].x = source->apiobj.collision_position.x;
        third.position[2].z = source->apiobj.collision_position.z;
        third.position[2].y = third.height[2] * (source->apiobj.collision_max.y - source->apiobj.collision_min.y) +
                              source->apiobj.collision_min.y;
    }
    if (static_cast<i8>(target->apiobj.flags_low) < 0) {
        if (Tag_DoneFirst == 0) {
            Tag_DoneFirst = 1;
        } else if (Tag_DoneFirst == 1) {
            Tag_DoneFirst = 2;
        }
        Tag_DoneAny = 1;
    }
}

void Tag_DrawIcon_LSW(GameObject_s *) {
}

void Tag_ResetTransfers() {
    Tag_Transfer[0].time = Tag_TransferResetTimer;
    Tag_Transfer[1].time = Tag_TransferResetTimer;
}

void Tag_DrawIcon_Batman(GameObject_s *) {
}

void Tag_UpdateTransfers(i32, i32, i32) {
}
