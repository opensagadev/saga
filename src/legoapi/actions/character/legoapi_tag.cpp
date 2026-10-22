// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/render/core/render.h"
#include "legoapi/audio/audio.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/world/mission.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/store.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/core/input/timer.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/nucamera.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuhgobj.h"
#include <stdio.h>
#include <string.h>
#include "legoapi/actions/character/legoapi_tag.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/actions/character/tagging.h"
#include "legoapi/actions/movement/climbing.h"
#include "legoapi/actions/movement/pushing.h"
#include "legoapi/ai/game/creature.h"
#include "legoapi/characters/motion/move.h"
#include "nu2api/nucore/nu2api_nucore_misc.h"
#include "legoapi/gizmo/object/takeoverobjects.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/lib/camera.h"
#include "legoapi/menus/core/hint.h"

// Character takeover and tag swapping (original legoapi_tag.cpp translation unit).

i32 players_cannot_exit_speeder;

static NUVEC SpeederChaseATATExitLandPos = {-159.0f, 6.869999885559082f, -17.600000381469727f};

extern i16 id_BUGGY, id_GYROCOPTER, id_BOMARRMONK;
extern i16 id_LANDSPEEDER;
extern i16 id_HEAVYREPEATINGCANNON, id_TROOPERCANNON;
extern GameObject_s *player_tag_to;
extern GameObject_s *player_tag_from;
static i32 Tag_Mode = 2;
void (*Tag_DrawIconFn)(GameObject_s *);

struct TAGSCRIPTSTATE {
    AISCRIPTPROCESS processor;
    void *field_c8;
};
DECOMP_ASSERT(sizeof(TAGSCRIPTSTATE) == 0xcc, "Tag script state ABI");
DECOMP_ASSERT(offsetof(GameObject_s, tag_cooldown) == 0x7b0, "Tag cooldown ABI");
DECOMP_ASSERT(offsetof(GameObject_s, field_0xef0) == 0xef0, "Tag reset state ABI");
DECOMP_ASSERT(offsetof(GameObject_s, takeover_target) == 0xeb0, "AI takeover target ABI");
DECOMP_ASSERT(offsetof(GameObject_s, field_0xeb4) == 0xeb4, "Object callback ABI");
DECOMP_ASSERT(offsetof(GameObject_s, current_hp) == 0x108b, "Tag signed health ABI");

extern "C" i32 menu_i_pack;
struct NuIOS_InAppProduct;
extern i16 tBUNDLESAVINGSAVAILABLE;
extern f32 text3d_height;

// GCC otherwise rewrites this once-called helper; the original has a separate local body.
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

static i32 Tag_FindNextPlayer(GameObject_s *object) {
    i16 index = object->apiobj.field_0x289;
    for (i32 remaining = HIGHGAMEOBJECT - 1; remaining > 0; --remaining) {
        if (++index == HIGHGAMEOBJECT)
            index = 0;
        GameObject_s *candidate = &Obj[index];
        if (candidate == object || candidate == NULL || (candidate->apiobj.field_0x1f8 & 0x1001) != 0x1001 ||
            candidate->apiobj.field_0x287 != 0 || candidate->apiobj.field_0x27c == -1 ||
            (candidate->tag_flags & 2) != 0 || (candidate->field_0xf00 & 2) != 0 ||
            (CInfo[candidate->character_context].flags & 0x800) != 0)
            continue;
        return candidate->apiobj.field_0x27c;
    }
    return -1;
}

static void DrawPackButton(GAMEMESSAGE_s *message, nuvec_s *position, float scale) {
    struct InAppProductData {
        u8 field_0x0[0x300];
        f32 price;
    } product;
    char text[128];

    const f32 phase = NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f);
    const i32 angle = static_cast<i32>(phase * 2.0f * 65536.0f);
    const f32 pulse = NuTrigTable[(angle >> 1) & 0x7fff] * 0.2f + 0.8f;
    i32 alpha = static_cast<i32>(pulse * 128.0f);
    const f32 label_y = position->y + 0.1f;

    if (!NuIOS_AreInAppPurchasesAvailable())
        goto purchases_unavailable;
    if (!NuIOS_CanMakeInAppPurchases())
        goto purchases_disabled;
    {
        product.price = 0.0f;
        NuIOS_GetInAppProductByID(StorePack[static_cast<i8>(message->field_0xfe)].product_id,
                                  reinterpret_cast<NuIOS_InAppProduct *>(&product));
        sprintf(text, "%s ~0%.2f~~", TTab[StorePack[static_cast<i8>(message->field_0xfe)].message_text_index],
                static_cast<double>(product.price));
        Text3DEx(text, position->x, label_y, position->z, scale, scale, scale, 4, message->red, message->green,
                 message->blue, alpha & 0xff);

        const f32 savings_height = text3d_height;
        uintptr_t cursor = reinterpret_cast<uintptr_t>(&StoreBundle[0].pack_mask);
        for (i32 count = 3; count != 0; --count, cursor += sizeof(STOREBUNDLE)) {
            u32 *bundle_mask = reinterpret_cast<u32 *>(cursor);
            if ((*bundle_mask & (1 << static_cast<i8>(message->field_0xfe))) == 0)
                continue;
            product.price = 0.0f;
            NuIOS_GetInAppProductByID(reinterpret_cast<STOREBUNDLE *>(cursor - offsetof(STOREBUNDLE, pack_mask))->name,
                                      reinterpret_cast<NuIOS_InAppProduct *>(&product));
            const f32 bundle_price = product.price;
            f32 pack_total = 0.0f;
            for (i32 index = 0; index < 11; ++index) {
                if (__builtin_expect(Store_IsPackUnlocked(index), 0) == 0 &&
                    __builtin_expect((*bundle_mask & (1u << index)) != 0, 1) &&
                    NuIOS_GetInAppProductByID(StorePack[index].product_id,
                                              reinterpret_cast<NuIOS_InAppProduct *>(&product)))
                    pack_total += product.price;
            }
            if (!(pack_total > bundle_price))
                continue;
            Text3DEx(TTab[tBUNDLESAVINGSAVAILABLE], position->x, label_y - savings_height, position->z, scale, scale,
                     scale, 4, 0, 191, 255, alpha & 0xff);
            break;
        }
    }

draw_button:
    DrawPanel3DObject(position->x, position->y, position->z, ICONSIZE, ICONSIZE, ICONSIZE, 0, 0, 0,
                      &WORLD->lev_objs[165].special, 0, pulse);
    Text3DEx(">", position->x + 0.00625f, position->y, position->z, 0.78f, 0.6f, 0.6f, 0, 255, 255, 255, alpha & 0xff);

    MechInputTouchMenuController::PackButtonActive = true;
    MechInputTouchMenuController::PackButtonX = position->x;
    MechInputTouchMenuController::PackButtonY = position->y;
    MechInputTouchMenuController::PackButtonW = ICONSIZE * 0.5f;
    MechInputTouchMenuController::PackButtonID = static_cast<i8>(message->field_0xfe);
    return;

purchases_disabled:
    Text3DEx(TTab[StorePack[static_cast<i8>(message->field_0xfe)].message_text_index], position->x, label_y,
             position->z, scale, scale, scale, 4, message->red, message->green, message->blue, alpha & 0xff);
    Text3DEx(TTab[0x610], position->x, label_y - text3d_height, position->z, scale, scale, scale, 4, 255, 31, 0,
             alpha & 0xff);
    goto draw_button;

purchases_unavailable:
    Text3DEx(TTab[StorePack[static_cast<i8>(message->field_0xfe)].message_text_index], position->x, label_y,
             position->z, scale, scale, scale, 4, message->red, message->green, message->blue, alpha & 0xff);
    Text3DEx(TTab[0x613], position->x, label_y - text3d_height, position->z, scale, scale, scale, 4, 255, 31, 0,
             alpha & 0xff);
    goto draw_button;
}

static void TakeOver_SetAction(GameObject_s *rider, GameObject_s *vehicle) {
    i32 action;
    if (vehicle->id == id_BUGGY)
        action = 0x70;
    else if (vehicle->id == id_GYROCOPTER)
        action = 0x83;
    else if (static_cast<GAMECHARACTERDATA *>(vehicle->apiobj.character_data->field11_0x24)->field275_0x116 == 12) {
        if (CurrentAnim(&vehicle->apiobj.anim_packet) == 3 || CurrentAnim(&vehicle->apiobj.anim_packet) == 0x17) {
            action = 0xc4;
        } else {
            action = 0xc3;
        }
    } else if (vehicle->id == id_BANTHA || vehicle->id == id_BOMARRMONK)
        action = 0x70;
    else if (vehicle->id == id_DEWBACK)
        action = 0x83;
    else if (vehicle->id == id_LANDSPEEDER || vehicle->id == id_FLASHSPEEDER)
        action = 0x84;
    else if (vehicle->id == id_TAUNTAUN)
        action = 0x86;
    else if (vehicle->id == id_SPEEDERBIKE)
        action = 0xc0;
    else if (vehicle->id == id_HEAVYREPEATINGCANNON || vehicle->id == id_BIGGUN)
        action = 0xc1;
    else if (vehicle->id == id_TROOPERCANNON) {
        action = rider->apiobj.character_model->model_data_b[0xc2] != NULL ? 0xc2 : 0xc1;
    } else if (vehicle->id == id_STAP2)
        action = 0xc1;
    else if (vehicle->id == id_CLONEWALKER)
        action = 0xc0;
    else {
        rider->context_animation = 0x6c;
        return;
    }
    rider->context_animation = rider->apiobj.character_model->model_data_b[action] != NULL ? action : 0x6c;
}

void Tag_SetMode(i32 mode) {
    Tag_Mode = mode;
}

i32 TagCode(GameObject_s *source, GameObject_s *destination, i32 takeover, i32 blend_camera, i32) {
    if (CUTSKIPLOCK != 0)
        return 0;
    GAMEPAD_s *source_pad = source->pad_gamepad;
    f32 source_da8 = source->field_0xda8;
    GAMEPAD_s *destination_pad = destination->pad_gamepad;
    i8 destination_hp = destination->current_hp;
    i8 source_hp = source->current_hp;
    u8 source_player = source->apiobj.player_controlled;
    u8 destination_player = destination->apiobj.player_controlled;
    TAGSCRIPTSTATE source_script, destination_script;
    u8 source_set, destination_set;
    i32 source_ai = 0, destination_ai = 0;
    if (takeover != 0) {
        memcpy(&source_script, &source->ai, sizeof(source_script));
        source_set = source->ai.creature_set;
        memcpy(&destination_script, &destination->ai, sizeof(destination_script));
        source_ai = source->apiobj.script_enabled;
        destination_ai = destination->apiobj.script_enabled;
        destination_set = destination->ai.creature_set;
    }
    ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(source->player_packet));
    ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(destination->player_packet));
    destination->pad_gamepad = source_pad;
    source->pad_gamepad = destination_pad;
    source->apiobj.player_controlled = destination_player;
    destination->apiobj.player_controlled = source_player;
    source->field_0xda8 = destination->field_0xda8;
    destination->field_0xda8 = source_da8;
    COINPACKET *coins = source->coinpacket;
    source->coinpacket = destination->coinpacket;
    destination->coinpacket = coins;
    u8 player_index = source->apiobj.field_0x27c;
    source->apiobj.field_0x27c = destination->apiobj.field_0x27c;
    destination->apiobj.field_0x27c = player_index;
    f32 dec = source->field_0xdec;
    source->field_0xdec = destination->field_0xdec;
    destination->field_0xdec = dec;
    GizForceLOSState_s *los = source->gizforce_los_info;
    source->gizforce_los_info = destination->gizforce_los_info;
    destination->gizforce_los_info = los;
    source->KillTasks();
    destination->KillTasks();
    GizForce_ResetLOS(source);
    GizForce_ResetLOS(destination);
    u32 mask = takeover == 0 ? 0x46f8 : 0x146fd;
    u32 source_state = source->apiobj.field_0x1f4;
    source->apiobj.field_0x1f4 &= ~mask;
    source->apiobj.field_0x1f4 |= destination->apiobj.field_0x1f4 & mask;
    destination->apiobj.field_0x1f4 = (destination->apiobj.field_0x1f4 & ~mask) | (source_state & mask);
    f32 protection = source->spawn_protection_timer;
    source->spawn_protection_timer = destination->spawn_protection_timer;
    destination->spawn_protection_timer = protection;
    void (*eb4)(GameObject_s *) = source->field_0xeb4;
    source->field_0xeb4 = destination->field_0xeb4;
    destination->field_0xeb4 = eb4;
    AICreatureResumeScript(source);
    if (takeover != 0) {
        if (static_cast<i8>(source->apiobj.field_0x27c) != -1)
            Player[static_cast<i8>(source->apiobj.field_0x27c)] = source;
        if (static_cast<i8>(destination->apiobj.field_0x27c) != -1)
            Player[static_cast<i8>(destination->apiobj.field_0x27c)] = destination;
    } else {
        source->hud_icon_timer = 0.0f;
        source->field_0xef0 = 0;
        source->pause_context_state = 0;
        source->input_toggle_hold_time = TOGGLEHOLDTIME;
        source->reset_lighting = 1;
        source->field_0xefe &= ~0x18;
        destination->hud_icon_timer = 2.0f;
        destination->input_toggle_hold_time = TOGGLEHOLDTIME;
        destination->reset_lighting = 1;
        NewBuzzFrames(destination->pad_gamepad->pad, 2, 0);
        if (static_cast<i8>(source->apiobj.field_0x27c) != -1) {
            Player[static_cast<i8>(source->apiobj.field_0x27c)] = source;
            if (source->field_0xcc0 != NULL)
                source->hitpoints =
                    static_cast<GAMECHARACTERDATA *>(source->apiobj.character_data->field11_0x24)->hitpoints;
            else if (WORLD->current_level != VADERC_LDATA)
                source->hitpoints = DEFAULT_PLAYERHITPOINTS;
        } else
            source->hitpoints =
                static_cast<GAMECHARACTERDATA *>(source->apiobj.character_data->field11_0x24)->hitpoints;
        if (static_cast<i8>(destination->apiobj.field_0x27c) != -1) {
            Player[static_cast<i8>(destination->apiobj.field_0x27c)] = destination;
            if (destination->field_0xcc0 != NULL)
                destination->hitpoints =
                    static_cast<GAMECHARACTERDATA *>(destination->apiobj.character_data->field11_0x24)->hitpoints;
            else if (WORLD->current_level != VADERC_LDATA)
                destination->hitpoints = DEFAULT_PLAYERHITPOINTS;
            GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
        } else
            destination->hitpoints =
                static_cast<GAMECHARACTERDATA *>(destination->apiobj.character_data->field11_0x24)->hitpoints;
        if (source_hp > destination->hitpoints)
            source_hp = destination->hitpoints;
        destination->current_hp = source_hp;
        if (destination->current_hp == 0 && destination->hitpoints != 0)
            destination->current_hp = 1;
        if (WORLD->current_level == VADERC_LDATA || source->apiobj.player_controlled)
            source->current_hp = destination_hp;
        else if ((source->apiobj.character_data->model_flags & 0x20) != 0)
            source->current_hp = source->field_0xe38;
        else
            source->current_hp = source->hitpoints;
    }
    destination->field_0xefe &= ~0x18;
    destination->field_0xef0 = 0;
    destination->pause_context_state = 0;
    destination->tag_cooldown = 1.0f;
    if (source == player_tag_to && destination != player_tag_from) {
        player_tag_to = NULL;
        player_tag_timer = 0.0f;
        player_tag_from = NULL;
        do_player_tag = 0;
    }
    if (takeover != 0) {
        AISCRIPTPROCESS *processor = reinterpret_cast<AISCRIPTPROCESS *>(&destination->ai);
        if (__builtin_expect(AIScriptSetBaseScriptStateByName(processor, const_cast<char *>("JustBeenTakenOver")) != 0, 1))
            AIScriptProcess(WORLD->ai_sys, &destination->apiobj, &destination->ai, processor, FRAMETIME);
        destination_script.processor.unknown_b0 = destination_set;
        memcpy(&source->ai, &destination_script, sizeof(destination_script));
        source_script.processor.unknown_b0 = source_set;
        memcpy(&destination->ai, &source_script, sizeof(source_script));
        source->ai.creature_set = source_set;
        destination->ai.creature_set = destination_set;
        AICreatureResumeScript(source);
        source->apiobj.script_enabled = destination_ai;
        destination->apiobj.script_enabled = source_ai;
        ReleaseForce(source, 0);
        ReleaseBuildIt(source, 0);
        ReleasePush(source);
        Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(source->player_packet));
        if (__builtin_expect((source->field_0xef8 & 0x10) == 0, 1))
            SetWeaponIn(source);
        source->field_0xcc0 = destination;
        source->takeover_target = NULL;
        destination->field_0xcc0 = source;
        source->takeover_source = destination;
        destination->takeover_target = NULL;
        source->character_context = 0x3b;
        TakeOver_SetAction(source, source->field_0xcc0);
        source->external_force = source->apiobj.position;
        source->takeover_timer = 1.0f;
        if (blend_camera != 0)
            GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
        if (static_cast<GAMECHARACTERDATA *>(destination->apiobj.character_data->field11_0x24)->field_0x28 > 0.0f)
            destination->apiobj.velocity.y -=
                0.5f * static_cast<GAMECHARACTERDATA *>(source->apiobj.character_data->field11_0x24)->jump_speed;
    }
    return 1;
}

i32 TagCharacter(GameObject_s *source, GameObject_s *target, i32 mode) {
    const auto available = [source](GameObject_s *candidate) {
        return candidate != NULL && (candidate->apiobj.field_0x1f8 & 0x1001) == 0x1001 && candidate != source &&
               candidate->apiobj.field_0x287 == 0 && !candidate->apiobj.player_controlled &&
               (candidate->tag_context_flags & 2) == 0;
    };
    if (!available(target)) {
        if (available(Player[0]))
            target = Player[0];
        else if (available(Player[1]))
            target = Player[1];
        else if (__builtin_expect(!!(available(Player[2])), 0))
            target = Player[2];
        else if (available(Player[3]))
            target = Player[3];
        else if (__builtin_expect(!!(available(Player[4])), 0))
            target = Player[4];
        else if (__builtin_expect(!!(available(Player[5])), 0))
            target = Player[5];
        else if (__builtin_expect(available(Player[6]) != 0, 0))
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
    if (__builtin_expect(result == 0, 0))
        return 0;
    target->apiobj.flags_low = (target->apiobj.flags_low & 0x7f) | (source_active << 7);
    source->apiobj.flags_low = (source->apiobj.flags_low & 0x7f) | (target_active << 7);
    return 1;
}

void GetTakeOverPos(GameObject_s *object, nuvec_s *position) {
    i32 locator = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->ride_locator;
    if (locator != -1 && object->apiobj.character_model->points_of_interest[locator] != NULL) {
        NUMTX *matrix = &object->joint_matrices[locator];
        *position = *NUMTX_GET_ROW_VEC(matrix, 3);
    } else {
        position->x = object->apiobj.position.x;
        position->y = object->apiobj.field_0x194;
        position->z = object->apiobj.position.z;
    }
}

void ReleaseTakeOver(GameObject_s *object, i32) {
    GameObject_s *rider = object->field_0xcc0;
    if (rider == NULL)
        return;
    if (object->character_context == 0x3b) {
        GameObject_s *vehicle = rider;
        rider = object;
        object = vehicle;
    }
    struct ScriptSnapshot {
        AISCRIPTPROCESS process;
        void *field_c8;
    } rider_script, object_script;
    DECOMP_ASSERT(sizeof(ScriptSnapshot) == 0xcc, "Takeover script snapshot ABI");
    memcpy(&rider_script.process, &rider->ai.script_process, sizeof(rider_script.process));
    rider_script.field_c8 = rider->ai.field_0xc8;
    u8 rider_set = rider->ai.creature_set;
    memcpy(&object_script.process, &object->ai.script_process, sizeof(object_script.process));
    object_script.field_c8 = object->ai.field_0xc8;
    u16 rider_flag = rider->apiobj.field_0x1f8 & 0x100;
    u16 object_flag = object->apiobj.field_0x1f8 & 0x100;
    u8 object_set = object->ai.creature_set;
    if ((rider->field_0xf00 & 2) == 0 && TagCode(rider, object, 1, 0, 0) == 0)
        return;
    memcpy(&rider->ai.script_process, &object_script.process, sizeof(object_script.process));
    rider->ai.field_0xc8 = object_script.field_c8;
    memcpy(&object->ai.script_process, &rider_script.process, sizeof(rider_script.process));
    object->ai.field_0xc8 = rider_script.field_c8;
    rider->ai.creature_set = rider_set;
    object->ai.creature_set = object_set;
    AICreatureResumeScript(object);
    rider->apiobj.field_0x1f8 = (rider->apiobj.field_0x1f8 & ~0x100) | object_flag;
    object->apiobj.field_0x1f8 = (object->apiobj.field_0x1f8 & ~0x100) | rider_flag;
    if (rider->field_0xcc0 != NULL) {
        GetTakeOverPos(rider->field_0xcc0, &rider->apiobj.position);
        rider->apiobj.pitch_angle = 0;
        rider->apiobj.roll_angle = 0;
        rider->apiobj.facing_angle = rider->field_0xcc0->apiobj.facing_angle;
        rider->apiobj.field_0x276 = rider->apiobj.facing_angle;
        rider->apiobj.movement_facing_angle = rider->field_0xcc0->apiobj.facing_angle;
    }
    GameObjectOrigin(rider);
    rider->character_context = -1;
    rider->field_0xcc0 = NULL;
    if (object->id == id_ATAT && WORLD->current_level == SPEEDERCHASEA_LDATA) {
        f32 multiplier = SpeederChaseATATInOutMul(&rider->apiobj.position, &SpeederChaseATATExitLandPos);
        StartBigJump(rider, &SpeederChaseATATExitLandPos, 0, multiplier * 4.0f, 2.5f * multiplier, 0, 0);
        GameCam_Blend(GameCam, 2.0f, 0.0f, 1);
    } else {
        Buck_StartRiderJump(rider, object);
    }
    GAMECHARACTERDATA *character = object->apiobj.character_data->game_character;
    if (character->field_0x28 > 0.0f) {
        object->apiobj.velocity.y -= 0.5f * rider->apiobj.velocity.y;
    } else if ((character->flags_094[2] & 2) != 0) {
        Buck_Start(object, rider);
    }
    object->field_0xcc0 = NULL;
    rider->tag_highlighted = 0;
    object->tag_highlighted = 0;
    rider->ai.path_info.flags &= ~1;
    object->saved_position = object->apiobj.position;
    if (rider->apiobj.field_0x27c != -1 || object->apiobj.field_0x27c != -1) {
        GameCam_Blend(GameCam, 0.3f, 0.0f, 1);
    }
    AISCRIPTPROCESS *rider_process = reinterpret_cast<AISCRIPTPROCESS *>(&rider->ai);
    AISCRIPTPROCESS *object_process = reinterpret_cast<AISCRIPTPROCESS *>(&object->ai);
    if (AIScriptSetBaseScriptStateByName(rider_process, const_cast<char *>("ReleasedTakeOver")) != 0) {
        AIScriptProcess(WORLD->ai_sys, &rider->apiobj, &rider->ai, rider_process, FRAMETIME);
    }
    if (AIScriptSetBaseScriptStateByName(object_process, const_cast<char *>("ReleasedTakeOver")) != 0) {
        AIScriptProcess(WORLD->ai_sys, &object->apiobj, &object->ai, object_process, FRAMETIME);
    }
}

void ReleaseAllTakeOvers() {
    for (i32 index = 0; index < 8; ++index) {
        if (Player[index] != NULL) {
            ReleaseTakeOver(Player[index], 0);
        }
    }
}

i32 TakeOverYoda(GameObject_s *rider, GameObject_s *vehicle, i32 blend_camera, i32) {
    ReleaseForce(rider, 0);
    ReleaseBuildIt(rider, 0);
    ReleasePush(rider);
    Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(rider->player_packet));
    if ((rider->field_0xef8 & 0x10) == 0)
        SetWeaponIn(rider);
    rider->field_0xcc0 = vehicle;
    vehicle->field_0xcc0 = rider;
    rider->takeover_target = NULL;
    rider->takeover_source = vehicle;
    vehicle->takeover_target = NULL;
    rider->character_context = 0x3b;
    TakeOver_SetAction(rider, rider->field_0xcc0);
    rider->external_force = rider->apiobj.position;
    rider->takeover_timer = 1.0f;
    if (blend_camera != 0)
        GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
    NewBuzz(vehicle->pad_gamepad->pad, 0.1f, 0);
    GameCam_Judder(GameCam, 0.1f, 0, NULL);
    Hint_SetComplete(0x28c);
    return 1;
}

void TakeOverGameObject2(GameObject_s *rider, GameObject_s *vehicle, i32 blend_camera) {
    if (rider == NULL || (rider->apiobj.flags_low & 1) == 0 || vehicle == NULL ||
        (vehicle->apiobj.flags_low & 1) == 0 || rider->field_0xcc0 != NULL || vehicle->field_0xcc0 != NULL) {
        return;
    }
    if ((rider->field_0xf00 & 2) != 0) {
        if (TakeOverYoda(rider, vehicle, blend_camera, 1) != 2)
            return;
    } else {
        if (WORLD->current_level == SPEEDERCHASEA_LDATA && disable_narrow_socks == 0 &&
            rider->apiobj.player_controlled) {
            blend_camera = 1;
        }
        RegisterTakeOverObject(vehicle);
        if (TagCode(rider, vehicle, 1, blend_camera, 1) != 2)
            return;
    }
    rider->tag_pending = 1;
    rider->pending_tag_target = vehicle;
    rider->tag_cooldown = 1.0f;
    rider->tag_blend_camera = 1;
}

void TakeOver2GetIn(GameObject_s *rider, GameObject_s *vehicle) {
    GameObject_s *object = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++object) {
        if ((object->apiobj.field_0x1f8 & 0x1001) == 0x1001 && object->character_context == 0x3c &&
            object->field_0x780 == vehicle)
            return;
    }
    if (rider == NULL || (rider->apiobj.flags_low & 1) == 0 || vehicle == NULL ||
        (vehicle->apiobj.flags_low & 1) == 0 || rider->field_0xcc0 != NULL || vehicle->field_0xcc0 != NULL)
        return;
    rider->character_context = 0x3c;
    if (rider->apiobj.character_model->model_data_b[6] != NULL) {
        ResetAnimPacket(&rider->apiobj.anim_packet, -1);
        rider->context_animation = 6;
        rider->airborne_action_duration = AnimDuration(rider->id, 6, 0.0f, 0.0f, 1);
    } else {
        rider->context_animation = 5;
        rider->airborne_action_duration = 0.5f;
    }
    if (vehicle->id == id_ATAT && WORLD->current_level == SPEEDERCHASEA_LDATA) {
        NUVEC position;
        GetTakeOverPos(vehicle, &position);
        rider->airborne_action_duration *= 2.0f * SpeederChaseATATInOutMul(&rider->apiobj.position, &position);
    }
    rider->context_animation_timer = 0.0f;
    rider->takeover_target = NULL;
    rider->external_force = rider->apiobj.position;
    rider->takeover_start_angle = rider->apiobj.field_0x276;
    rider->field_0x780 = vehicle;
    vehicle->takeover_target = NULL;
    PlayJumpSfx(rider, 0);
}

void Tag_Check(GameObject_s *object) {
    NUVEC difference;
    DECOMP_ASSERT(offsetof(GameObject_s, tag_player_index) == 0x7b4, "Tag player index ABI");
    DECOMP_ASSERT(offsetof(GameObject_s, tag_flags) == 0x7b5, "Tag flags ABI");
    DECOMP_ASSERT(offsetof(GameObject_s, pending_tag_target) == 0x7b8, "Pending tag target ABI");
    GameObject_s *target = NULL;
    i32 result = 0;
    const i32 arcade = Arcade;
    const bool disabled = (object->tag_flags & 1) != 0 || object->character_context == 0x51;
    if (disabled) {
        object->field_0xefe &= ~0x18;
        object->hud_icon_timer = 0.0f;
        object->field_0xef0 = 0.0f;
        object->pause_context_state = 0.0f;
        if (arcade == 0)
            return;
    } else if (arcade != 0) {
        object->field_0xef0 = 0.0f;
        object->pause_context_state = 0.0f;
        object->field_0xefe &= ~0x18;
    }
    if (object->apiobj.field_0x27c == -1)
        return;
    if ((object->tag_flags & 4) != 0) {
        object->tag_cooldown -= FRAMETIME;
        if (object->tag_cooldown > 0.0f && object->pending_tag_target != NULL) {
            target = object->pending_tag_target;
            if ((target->apiobj.field_0x1f8 & 0x1001) != 0x1001) {
                object->pending_tag_target = NULL;
            } else {
                result = (object->field_0xf00 & 2) != 0 ? TakeOverYoda(object, target, 0, 1)
                                                        : TagCode(object, target, (object->tag_flags >> 3) & 1, 0, 1);
            }
        }
        if (result == 0)
            object->tag_flags &= ~4;
    } else {
        object->tag_player_index = -1;
        if (object->apiobj.player_controlled && object->apiobj.field_0x287 == 0 &&
            (CInfo[object->character_context].flags & 0x800) == 0 && !object->apiobj.script_enabled &&
            !(object->tag_cooldown > 0.0f)) {
            if (VehicleArea != 0) {
                if ((object->pad_gamepad->buttons_pressed & GAMEPAD_TAG) != 0)
                    object->hud_icon_timer = 2.0f;
            } else {
                if (__builtin_expect(Arcade == 0 && Tag_Mode == 3 && GetMenuID() == -1, 1)) {
                    if (__builtin_expect(FindGameMsgsWithID(-1, 0, -1, NULL) == 0, 1)) {
                        i32 pack_index = -1;
                        f32 nearest = 0.7f * 0.7f * 2.0f;

                        for (i32 i = 0; i < 11; ++i) {
                            if (!Store_IsPackUnlocked(i) && StorePack[i].id != NULL) {
                                f32 distance = NuVecDistSqr(&object->apiobj.position, &StorePack[i].custodian_position,
                                                            &difference);
                                if (distance < nearest) {
                                    nearest = distance;
                                    pack_index = i;
                                }
                            }
                        }
                        if (pack_index != -1) {
                            STOREPACK *pack = &StorePack[pack_index];
                            NUVEC position = pack->custodian_position;
                            position.y += CDataList[*pack->id].field16_0x38 + 0.075f;
                            ADDGAMEMSG message = AddGameMsg_Default;
                            message.position = &position;
                            message.red = 255;
                            message.text = TTab[pack->message_text_index];
                            message.green = 191;
                            message.blue = 0;
                            message.flags = 0x1083;
                            message.field_0x4e = pack_index;
                            message.field_0x44 = reinterpret_cast<void *>(DrawPackButton);
                            message.scale = 0.4f;
                            AddGameMsg(&message);
                            Hint_CancelCurrent();
                        }
                    } else {
                        Hint_CancelCurrent();
                    }
                }
                if ((object->pad_gamepad->buttons_pressed & GAMEPAD_TAG) != 0) {
                    i32 no_target = 1;
                    if (CUTSKIPLOCK != 0) {
                        object->hud_icon_timer = 2.0f;
                        object->input_toggle_hold_time = TOGGLEHOLDTIME;
                    } else if (Arcade == 0) {
                        if (Tag_Mode == 3) {
                            if (object->character_context == 0x17 || object->character_context == -1 ||
                                (CInfo[object->character_context].flags & (0x10ull << 32)) != 0 ||
                                (CInfo[object->character_context].flags & 4) != 0) {
                                target = Tag_FindGameObject_TRANSFER(object);
                            }
                            if (target != NULL) {
                                if (InCollectList_Index(target->id, NULL, 0) != -1) {
                                    i32 pack = Store_FindPack(target->id, NULL);
                                    if (pack != -1 && static_cast<i32>(target->apiobj.field_0x1f4) < 0 &&
                                        !Store_IsPackUnlocked(pack)) {
                                        Hint_CancelCurrent();
                                        if (NuIOS_AreInAppPurchasesAvailable() && NuIOS_CanMakeInAppPurchases()) {
                                            GameAudio_PlaySfx(0x30, NULL, 0, 0);
                                            menu_i_pack = pack;
                                            NewMenu(0x14, -1, -1);
                                        } else {
                                            GameAudio_PlaySfx(0x32, NULL, 0, 0);
                                            GameCam_HitRoll();
                                        }
                                        no_target = 0;
                                        target = NULL;
                                    }
                                } else if (InCollectList_Index(target->id, NULL, 0) == -1) {
                                    GameAudio_PlaySfx(0x32, &object->apiobj.collision_position, 0, 0);
                                    NewRumble(object->pad_gamepad->pad, 0.5f, 0);
                                    AISCRIPTPROCESS *processor = reinterpret_cast<AISCRIPTPROCESS *>(&target->ai);
                                    if (AIScriptSetBaseScriptStateByName(processor, const_cast<char *>("MapRunAway")) !=
                                        0)
                                        AIScriptProcess(WORLD->ai_sys, &target->apiobj, &target->ai, processor,
                                                        FRAMETIME);
                                    target = NULL;
                                }
                            }
                        } else {
                            if (Tag_Mode == 1) {
                                if (object->character_context == 0x17 ||
                                    __builtin_expect(object->character_context == -1, 0) ||
                                    (CInfo[object->character_context].flags & (0x10ull << 32)) != 0 ||
                                    (CInfo[object->character_context].flags & 4) != 0) {
                                    GameObject_s *found = Tag_FindGameObject_TRANSFER(object);
                                    if (found != NULL && found->apiobj.field_0x27c != -1)
                                        object->tag_player_index = found->apiobj.field_0x27c;
                                }
                            } else {
                                object->tag_player_index = Tag_FindNextPlayer(object);
                            }
                            if (object->tag_player_index >= 0)
                                target = Player[object->tag_player_index];
                        }
                        if (target != NULL) {
                            no_target = 0;
                            if (__builtin_expect((target->field_0xf00 & 2) != 0, 0)) {
                                TakeOver2GetIn(target, object);
                            } else if (!object->apiobj.player_controlled || !target->apiobj.player_controlled) {
                                result = TagCode(object, target, 0, 0, 1);
                                if (result == 2) {
                                    object->pending_tag_target = target;
                                    object->tag_cooldown = 1.0f;
                                    object->tag_flags = (object->tag_flags | 4) & ~8;
                                }
                            } else if (player_tag_timer > 0.0f && object == player_tag_to &&
                                       target == player_tag_from) {
                                do_player_tag = 1;
                            } else {
                                player_tag_to = target;
                                player_tag_from = object;
                                player_tag_timer = 0.5f;
                            }
                        }
                    }
                    if (no_target && __builtin_expect(object->field_0xcc0 == NULL, 1))
                        object->hud_icon_timer = 2.0f;
                }
            }
        }
    }
    if (result == 1) {
        object->tag_flags &= ~4;
        GameAudio_PlaySfx(0x21, &object->apiobj.collision_position, 0, 0);
        if (target->apiobj.model_draw_result != 0)
            GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
        else
            GameCam_Reset(GameCam);
        RememberPlayerIDs(0, Player[0] != NULL ? Player[0]->id : -1, Player[1] != NULL ? Player[1]->id : -1);
        if (__builtin_expect((Tag_Mode & ~2) == 1, 0))
            Tag_NewTransfer(object, target);
        if (static_cast<u8>(object->apiobj.field_0x27c) < 2 && static_cast<u8>(target->apiobj.field_0x27c) < 2)
            ResetTimer(&JoinInTimer, 0.0f);
    }
    if (__builtin_expect(Tag_DrawIconFn != NULL, 0))
        Tag_DrawIconFn(object);
}

void TakeOverGameObject(GameObject_s *rider, GameObject_s *vehicle, i32 blend_camera, i32 immediate) {
    if (immediate != 0)
        TakeOverGameObject2(rider, vehicle, blend_camera);
    else
        TakeOver2GetIn(rider, vehicle);
}

__attribute__((force_align_arg_pointer)) void TakeOverCode(GameObject_s *object, i32 tag_pressed) {
    APIOBJECT &api = object->apiobj;
    if (object->character_context == 0x3c) {
        GameObject_s *target = object->takeover_entry_target;
        if (target == NULL) {
            object->character_context = -1;
            return;
        }
        object->context_animation_timer += FRAMETIME;
        if (object->context_animation_timer >= object->airborne_action_duration) {
            object->character_context = -1;
            if (api.player_controlled) {
                if (target->id == id_GRABCONTROL || target->id == id_GRABR2CONTROL) {
                    Hint_SetComplete(0x28a);
                } else if (target->apiobj.character_data->move_fn == Move_BEAST) {
                    Hint_SetComplete(0x263);
                } else if (object->id == id_ATAT) {
                    Hint_SetComplete(0x28d);
                } else {
                    Hint_SetComplete(0x25d);
                }
            }
            if (WORLD->grabber != NULL && (object->takeover_entry_target->id == id_GRABCONTROL ||
                                           object->takeover_entry_target->id == id_GRABR2CONTROL)) {
                PlaySfx("env_crane_in", &api.collision_position);
            } else {
                PlayLandSfx(object, 0, 0);
            }
            NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
            TakeOverGameObject2(object, object->takeover_entry_target, 1);
            return;
        }
        const f32 fraction = object->context_animation_timer / object->airborne_action_duration;
        NUVEC destination;
        GetTakeOverPos(target, &destination);
        api.position.x = (destination.x - object->external_force.x) * fraction + object->external_force.x;
        api.position.y = (destination.y - object->external_force.y) * fraction + object->external_force.y;
        api.position.z = (destination.z - object->external_force.z) * fraction + object->external_force.z;
        target = object->takeover_entry_target;
        f32 height = target->apiobj.field_0x1e0;
        if (object->context_animation == 0xc9 || object->context_animation == 6) {
            const f32 jump_height = api.character_data->game_character->field_0x34;
            height = jump_height > height ? jump_height : height;
        }
        if (target->id == id_ATAT && WORLD->current_level == SPEEDERCHASEA_LDATA) {
            height *= SpeederChaseATATInOutMul(&destination, &object->external_force) * 0.75f;
            target = object->takeover_entry_target;
        }
        const f32 phase = fraction * 32768.0f;
        api.position.y += height * NU_SIN_LUT(static_cast<i32>(phase));
        const i32 start_angle = object->takeover_start_angle;
        const i32 difference = RotDiff(start_angle, target->apiobj.field_0x276);
        const f32 blend = 1.0f - (NU_SIN_LUT(static_cast<i32>(phase + 16384.0f)) + 1.0f) * 0.5f;
        const u16 angle = static_cast<u16>(static_cast<f32>(start_angle) + static_cast<f32>(difference) * blend);
        api.field_0x276 = angle;
        api.movement_facing_angle = angle;
        api.facing_angle = angle;
        return;
    }
    object->tag_flags &= static_cast<u8>(~1u);
    GameObject_s *target = object->field_0xcc0;
    if (target != NULL) {
        if (object->character_context == 0x3b) {
            if (!(players_cannot_exit_speeder != 0 && target->id == id_SPEEDERBIKE)) {
                u32 buttons = target->pad_gamepad->buttons_pressed;
                if ((object->field_0xf00 & 2) != 0)
                    buttons &= ~GAMEPAD_JUMP;
                if (target->character_context != 0x2a && (buttons & (GAMEPAD_JUMP | GAMEPAD_TAG)) != 0) {
                    ReleaseTakeOver(object, 1);
                    if (api.player_controlled)
                        Hint_SetComplete(0x25e);
                    return;
                }
            }
            GetTakeOverPos(target, &api.position);
            target = object->field_0xcc0;
            api.pitch_angle = 0;
            api.roll_angle = 0;
            api.field_0x276 = target->apiobj.facing_angle;
            api.facing_angle = target->apiobj.facing_angle;
            api.movement_facing_angle = target->apiobj.facing_angle;
            GameObjectOrigin(object);
            object->takeover_timer = 1.0f;
        } else if (api.character_data->game_character->field275_0x116 == 12) {
            i16 animation = 0xc3;
            if (CurrentAnim(&api.anim_packet) == 3 || CurrentAnim(&api.anim_packet) == 0x17)
                animation = 0xc4;
            target = object->field_0xcc0;
            if (target->apiobj.character_model->model_data_b[animation] != NULL)
                target->context_animation = animation;
        }
        return;
    }
    if (object->takeover_timer >= 0.0f) {
        object->takeover_timer -= FRAMETIME;
        return;
    }
    if (MiniCutCam != 0 || !api.player_controlled || api.field_0x287 != 0 ||
        __builtin_expect(!(object->takeover_timer <= 0.0f), 0) ||
        (object->character_context != -1 && (object->character_context != 0 || object->action_movement_state == 3 ||
                                             object->action_movement_state == 4)) ||
        api.field_0x27d == 0) {
        return;
    }
    f32 nearest_distance = 9.0f;
    GameObject_s *nearest = NULL;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
        GameObject_s *candidate = &Obj[i];
        const APIOBJECT &other = candidate->apiobj;
        if (__builtin_expect((other.field_0x1f8 & 0x1001) != 0x1001 || other.field_0x287 != 0 ||
                                 other.field_0x27c != -1 || (CInfo[candidate->character_context].flags & 0x8000) != 0 ||
                                 candidate->field_0xcc0 != NULL ||
                                 (other.character_data->game_character->flags_090 & 0x40) == 0 ||
                                 ((other.character_data->model_flags & 0x40000000) != 0 &&
                                  (candidate->character_context == 0x3e || candidate->character_context == 0x17)) ||
                                 (candidate->tag_flags & 2) != 0,
                             1))
            continue;
        if (candidate->id == id_ATAT && WORLD->current_level == SPEEDERCHASEA_LDATA) {
            if (api.field_0x281 == 0x13)
                nearest = candidate;
            continue;
        }
        if (__builtin_expect(api.collision_min.y > other.collision_max.y || other.collision_min.y > api.collision_max.y,
                             1))
            continue;
        const f32 dz = other.collision_position.z - api.collision_position.z;
        const f32 dx = other.collision_position.x - api.collision_position.x;
        const f32 range = (other.field_0x1dc + other.field_0x1dc) + api.field_0x1dc;
        const f32 distance = dx * dx + dz * dz;
        if (object->facing_direction.x * dx + object->facing_direction.z * dz > 0.0f && range * range > distance &&
            nearest_distance > distance) {
            nearest_distance = distance;
            nearest = candidate;
        }
    }
    if (nearest != NULL) {
        if (tag_pressed != 0 || (nearest->id == id_ATAT && WORLD->current_level == SPEEDERCHASEA_LDATA)) {
            TakeOver2GetIn(object, nearest);
        } else {
            object->tag_flags |= 1;
            nearest->tag_highlighted = 1;
        }
    }
}

