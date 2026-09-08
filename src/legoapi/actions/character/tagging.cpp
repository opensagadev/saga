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
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
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

static i32 Tag_Mode = 2;

void (*Tag_DrawIconFn)(GameObject_s *) = NULL;
i32 (*Tag_NoHiddenIconFn)(GameObject_s *) = NULL;
char *LEGOASCII_UP = NULL;
u8 PlayerRGB[2][3] = {{0, 127, 255}, {0, 255, 0}};
extern ADDGAMEMSG AddGameMsg_Default;
GAMEMESSAGE_s *AddGameMsg(ADDGAMEMSG *message);
i32 do_player_tag = 0;
f32 player_tag_timer = 0.0f;
GameObject_s *player_tag_from = NULL;
GameObject_s *player_tag_to = NULL;

void ResetForceGlow(PLAYERPACKET_s *packet);
void AICreatureResumeScript(GameObject_s *object);
void GizForce_ResetLOS(GameObject_s *object);
void NewBuzzFrames(nupad_s *pad, i32 frames, i32 flags);
void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);
void GameAudio_PlaySfx(i32 sfx, NUVEC *position, i32 flags, i32 volume);
void TakeOver2GetIn(GameObject_s *source, GameObject_s *target);
void TakeOverYoda(GameObject_s *source, GameObject_s *target, i32 mode, i32 blend);
extern i32 CUTSKIPLOCK;
extern i16 id_LUKESKYWALKERDAGOBAH;
extern "C" i32 menu_i_pack;
void NewRumble(nupad_s *, f32, i32);
void Hint_CancelCurrent();
void GameCam_HitRoll();
i32 NuIOS_AreInAppPurchasesAvailable();
i32 NuIOS_CanMakeInAppPurchases();

// The original keeps this search out of line and passes the object in EAX.
static __attribute__((noinline)) GameObject_s *Tag_FindGameObject_TRANSFER(GameObject_s *object) {
    f32 nearest_distance = object->character_context == 0x17 ? 1.44f : 0.48999998f;
    const i32 count = Tag_Mode == 3 ? HIGHGAMEOBJECT : 8;
    GameObject_s *nearest = NULL;
    for (i32 index = 0; index < count; ++index) {
        GameObject_s *candidate = Tag_Mode == 3 ? &Obj[index] : Player[index];
        if (candidate == NULL || (candidate->apiobj.field_0x1f8 & 0x1001) != 0x1001 || candidate == object ||
            candidate->apiobj.field_0x287 != 0 || (candidate->tag_context_flags & 2) != 0) {
            continue;
        }
        const i8 context = candidate->character_context;
        if (context == 0x17 || context == 0x3d || (CInfo[context].flags & 0x8000) != 0 ||
            ((candidate->field_0xf00 & 2) != 0 && object->id != id_LUKESKYWALKERDAGOBAH)) {
            continue;
        }
        NUVEC direction;
        const f32 distance = NuVecDistSqr(&object->apiobj.position, &candidate->apiobj.position, &direction);
        if (distance < nearest_distance) {
            NuVecRotateY(&direction, &direction, -static_cast<u32>(object->apiobj.field_0x276));
            if (direction.z < 0.0f) {
                nearest_distance = distance;
                nearest = candidate;
            }
        }
    }
    return nearest;
}

void Tag_SetMode(i32 mode) {
    Tag_Mode = mode;
}

i32 TagCode(GameObject_s *, GameObject_s *, i32, i32, i32);

i32 TagCharacter(GameObject_s *source, GameObject_s *target, i32 mode) {
    const auto available = [source](GameObject_s *candidate) {
        return candidate != NULL && (candidate->apiobj.field_0x1f8 & 0x1001) == 0x1001 && candidate != source &&
               candidate->apiobj.field_0x287 == 0 && static_cast<i8>(candidate->apiobj.flags_low) >= 0 &&
               (candidate->tag_context_flags & 2) == 0;
    };
    if (!available(target)) {
        if (available(Player[0]))
            target = Player[0];
        else if (available(Player[1]))
            target = Player[1];
        else if (available(Player[2]))
            target = Player[2];
        else if (available(Player[3]))
            target = Player[3];
        else if (available(Player[4]))
            target = Player[4];
        else if (available(Player[5]))
            target = Player[5];
        else if (available(Player[6]))
            target = Player[6];
        else if (available(Player[7]))
            target = Player[7];
        else
            return 0;
    }
    const u8 target_active = target->apiobj.flags_low >> 7;
    const u8 source_active = source->apiobj.flags_low >> 7;
    const i32 result = TagCode(source, target, 0, 0, mode);
    if (result == 2) {
        const u8 flags = (source->tag_context_flags | 4) & ~8;
        source->tag_target = target;
        source->tag_context_timer = 3.0f;
        source->tag_context_flags = flags;
        return 0;
    }
    if (result == 0)
        return 0;
    target->apiobj.flags_low = (target->apiobj.flags_low & 0x7f) | (source_active << 7);
    source->apiobj.flags_low = (source->apiobj.flags_low & 0x7f) | (target_active << 7);
    return 1;
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

void Tag_DrawIcon_LSW(GameObject_s *object) {
    if (VehicleArea != 0 || FadeSys.fade != 0.0f || static_cast<i8>(object->apiobj.flags_low) >= 0 ||
        (WORLD->current_level->flags & LEVEL_HIDE_ICONS) != 0) {
        return;
    }
    if (Tag_NoHiddenIconFn != NULL && Tag_NoHiddenIconFn(object) != 0) {
        return;
    }
    if ((object->field_0xefe & 0x10) == 0) {
        f32 timer = object->hud_icon_timer;
        if (!(timer > 0.0f) || (timer < 2.0f && NuFmod(timer, 0.4f) < 0.2f)) {
            return;
        }
    }
    ADDGAMEMSG message = AddGameMsg_Default;
    message.field_0x4f = 1;
    message.scale = 3.0f;
    NUVEC position = object->apiobj.position;
    position.y += object->field_0xffc * object->apiobj.field_0xa8;
    message.text = LEGOASCII_UP;
    message.position = &position;
    message.red = PlayerRGB[0][0];
    message.green = PlayerRGB[0][1];
    message.blue = PlayerRGB[0][2];
    f32 phase = NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f);
    message.flags = 0x87;
    message.alpha = static_cast<i32>(48.0f * NU_SIN_LUT(static_cast<u16>((phase + phase) * 65536.0f)) + 80.0f);
    AddGameMsg(&message);
}

void Tag_ResetTransfers() {
    Tag_Transfer[0].time = Tag_TransferResetTimer;
    Tag_Transfer[1].time = Tag_TransferResetTimer;
}

void Tag_DrawIcon_Batman(GameObject_s *) {
}

extern "C" void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);

void Tag_UpdateTransfers(i32 effect0, i32 effect1, i32 count) {
    NUVEC position __attribute__((aligned(16)));
    if (Player[0] == NULL) {
        Tag_Transfer[0].time = Tag_TransferResetTimer;
        goto second_player;
    }
    if (!(Tag_Transfer[0].time < Tag_TransferResetTimer))
        goto second_player;
    Tag_Transfer[0].time += FRAMETIME;
    if (Tag_Transfer[0].time > Tag_TransferResetTimer) {
        Tag_Transfer[0].time = Tag_TransferResetTimer;
        goto second_player;
    }
    {
        f32 phase = Tag_Transfer[0].time + Tag_Transfer[0].time;
        {
            GameObject_s *source = Tag_Transfer[0].source;
            GameObject_s *target = Player[0];
            const f32 height = Tag_Transfer[0].height[0];
            NUVEC from = source->apiobj.collision_position;
            NUVEC to = target->apiobj.collision_position;
            from.y = (source->apiobj.collision_max.y - source->apiobj.collision_min.y) * height +
                     source->apiobj.collision_min.y;
            to.y = (target->apiobj.collision_max.y - target->apiobj.collision_min.y) * height +
                   target->apiobj.collision_min.y;
            position.x = (to.x - from.x) * phase + from.x;
            position.y = (to.y - from.y) * phase + from.y;
            position.z = (to.z - from.z) * phase + from.z;
            AddVariableShotDebrisEffectTimed1(effect0, &position, count, FRAMETIME, 0, 0, NULL);
            Tag_Transfer[0].position[0] = position;
        }
        {
            phase = NU_SIN_LUT(static_cast<i32>(phase * 16384.0f));
            GameObject_s *source = Tag_Transfer[0].source;
            GameObject_s *target = Player[0];
            const f32 height = Tag_Transfer[0].height[1];
            NUVEC from = source->apiobj.collision_position;
            NUVEC to = target->apiobj.collision_position;
            from.y = (source->apiobj.collision_max.y - source->apiobj.collision_min.y) * height +
                     source->apiobj.collision_min.y;
            to.y = (target->apiobj.collision_max.y - target->apiobj.collision_min.y) * height +
                   target->apiobj.collision_min.y;
            position.x = (to.x - from.x) * phase + from.x;
            position.y = (to.y - from.y) * phase + from.y;
            position.z = (to.z - from.z) * phase + from.z;
            position.y += 0.005f * NU_SIN_LUT(static_cast<i32>(phase * 65536.0f));
            AddVariableShotDebrisEffectTimed1(effect0, &position, count, FRAMETIME, 0, 0, NULL);
            Tag_Transfer[0].position[1] = position;
        }
        {
            phase = 1.0f - NU_SIN_LUT(static_cast<i32>(phase * 16384.0f + 16384.0f));
            GameObject_s *source = Tag_Transfer[0].source;
            GameObject_s *target = Player[0];
            const f32 height = Tag_Transfer[0].height[2];
            NUVEC from = source->apiobj.collision_position;
            NUVEC to = target->apiobj.collision_position;
            from.y = (source->apiobj.collision_max.y - source->apiobj.collision_min.y) * height +
                     source->apiobj.collision_min.y;
            to.y = (target->apiobj.collision_max.y - target->apiobj.collision_min.y) * height +
                   target->apiobj.collision_min.y;
            position.x = (to.x - from.x) * phase + from.x;
            position.y = (to.y - from.y) * phase + from.y;
            position.z = (to.z - from.z) * phase + from.z;
            position.y += 0.01f * NU_SIN_LUT(static_cast<i32>(phase * 32768.0f + 16384.0f));
            AddVariableShotDebrisEffectTimed1(effect0, &position, count, FRAMETIME, 0, 0, NULL);
            Tag_Transfer[0].position[2] = position;
        }
    }
second_player:
    if (Player[1] == NULL) {
        Tag_Transfer[1].time = Tag_TransferResetTimer;
        return;
    }
    if (!(Tag_Transfer[1].time < Tag_TransferResetTimer))
        return;
    Tag_Transfer[1].time += FRAMETIME;
    if (Tag_Transfer[1].time > Tag_TransferResetTimer) {
        Tag_Transfer[1].time = Tag_TransferResetTimer;
        return;
    }
    {
        f32 phase = Tag_Transfer[1].time + Tag_Transfer[1].time;
        {
            GameObject_s *source = Tag_Transfer[1].source;
            GameObject_s *target = Player[1];
            const f32 height = Tag_Transfer[1].height[0];
            NUVEC from = source->apiobj.collision_position;
            NUVEC to = target->apiobj.collision_position;
            from.y = (source->apiobj.collision_max.y - source->apiobj.collision_min.y) * height +
                     source->apiobj.collision_min.y;
            to.y = (target->apiobj.collision_max.y - target->apiobj.collision_min.y) * height +
                   target->apiobj.collision_min.y;
            position.x = (to.x - from.x) * phase + from.x;
            position.y = (to.y - from.y) * phase + from.y;
            position.z = (to.z - from.z) * phase + from.z;
            AddVariableShotDebrisEffectTimed1(effect1, &position, count, FRAMETIME, 0, 0, NULL);
            Tag_Transfer[1].position[0] = position;
        }
        {
            phase = NU_SIN_LUT(static_cast<i32>(phase * 16384.0f));
            GameObject_s *source = Tag_Transfer[1].source;
            GameObject_s *target = Player[1];
            const f32 height = Tag_Transfer[1].height[1];
            NUVEC from = source->apiobj.collision_position;
            NUVEC to = target->apiobj.collision_position;
            from.y = (source->apiobj.collision_max.y - source->apiobj.collision_min.y) * height +
                     source->apiobj.collision_min.y;
            to.y = (target->apiobj.collision_max.y - target->apiobj.collision_min.y) * height +
                   target->apiobj.collision_min.y;
            position.x = (to.x - from.x) * phase + from.x;
            position.y = (to.y - from.y) * phase + from.y;
            position.z = (to.z - from.z) * phase + from.z;
            position.y += 0.005f * NU_SIN_LUT(static_cast<i32>(phase * 65536.0f));
            AddVariableShotDebrisEffectTimed1(effect1, &position, count, FRAMETIME, 0, 0, NULL);
            Tag_Transfer[1].position[1] = position;
        }
        {
            phase = 1.0f - NU_SIN_LUT(static_cast<i32>(phase * 16384.0f + 16384.0f));
            GameObject_s *source = Tag_Transfer[1].source;
            GameObject_s *target = Player[1];
            const f32 height = Tag_Transfer[1].height[2];
            NUVEC from = source->apiobj.collision_position;
            NUVEC to = target->apiobj.collision_position;
            from.y = (source->apiobj.collision_max.y - source->apiobj.collision_min.y) * height +
                     source->apiobj.collision_min.y;
            to.y = (target->apiobj.collision_max.y - target->apiobj.collision_min.y) * height +
                   target->apiobj.collision_min.y;
            position.x = (to.x - from.x) * phase + from.x;
            position.y = (to.y - from.y) * phase + from.y;
            position.z = (to.z - from.z) * phase + from.z;
            position.y += 0.01f * NU_SIN_LUT(static_cast<i32>(phase * 32768.0f + 16384.0f));
            AddVariableShotDebrisEffectTimed1(effect1, &position, count, FRAMETIME, 0, 0, NULL);
            Tag_Transfer[1].position[2] = position;
        }
    }
}
