#include "legoapi/legoapi_types.h"

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

static i32 Tag_Mode = 2;

void (*Tag_DrawIconFn)(GameObject_s *) = NULL;
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
        first.position[0].y = first.height[0] *
            (source->apiobj.collision_max.y - source->apiobj.collision_min.y) + source->apiobj.collision_min.y;

        const i8 second_index = target->apiobj.field_0x27c;
        Tag_Transfer[second_index].height[1] = static_cast<f32>(qrand()) * 1.5259022e-05f * 0.4f + 0.4f;
        TAGTRANSFER_s &second = Tag_Transfer[target->apiobj.field_0x27c];
        second.position[1].x = source->apiobj.collision_position.x;
        second.position[1].z = source->apiobj.collision_position.z;
        second.position[1].y = second.height[1] *
            (source->apiobj.collision_max.y - source->apiobj.collision_min.y) + source->apiobj.collision_min.y;

        const i8 third_index = target->apiobj.field_0x27c;
        Tag_Transfer[third_index].height[2] = static_cast<f32>(qrand()) * 1.5259022e-05f * 0.4f + 0.4f;
        TAGTRANSFER_s &third = Tag_Transfer[target->apiobj.field_0x27c];
        third.position[2].x = source->apiobj.collision_position.x;
        third.position[2].z = source->apiobj.collision_position.z;
        third.position[2].y = third.height[2] *
            (source->apiobj.collision_max.y - source->apiobj.collision_min.y) + source->apiobj.collision_min.y;
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

i32 TagCode(GameObject_s *source, GameObject_s *target, i32 takeover, i32, i32) {
    if (CUTSKIPLOCK != 0) {
        return 0;
    }

    GAMEPAD_s *target_gamepad = target->pad_gamepad;
    const f32 source_field_da8 = source->field_0xda8;
    GAMEPAD_s *source_gamepad = source->pad_gamepad;
    u8 source_hitpoints = source->current_hp;
    const u8 target_hitpoints = target->current_hp;
    const u8 source_active = source->apiobj.flags_low >> 7;
    const u8 target_active = target->apiobj.flags_low >> 7;

    ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(source->player_packet));
    ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(target->player_packet));

    target->pad_gamepad = source_gamepad;
    source->pad_gamepad = target_gamepad;
    source->apiobj.flags_low = (source->apiobj.flags_low & 0x7f) | (target_active << 7);
    target->apiobj.flags_low = (target->apiobj.flags_low & 0x7f) | (source_active << 7);

    source->field_0xda8 = target->field_0xda8;
    target->field_0xda8 = source_field_da8;

    COINPACKET *coinpacket = source->coinpacket;
    source->coinpacket = target->coinpacket;
    target->coinpacket = coinpacket;

    const i8 source_player = source->apiobj.field_0x27c;
    source->apiobj.field_0x27c = target->apiobj.field_0x27c;
    target->apiobj.field_0x27c = source_player;

    const f32 source_field_dec = source->field_0xdec;
    source->field_0xdec = target->field_0xdec;
    target->field_0xdec = source_field_dec;

    GizForceLOSState_s *los_info = source->gizforce_los_info;
    source->gizforce_los_info = target->gizforce_los_info;
    target->gizforce_los_info = los_info;

    source->KillTasks();
    target->KillTasks();
    GizForce_ResetLOS(source);
    GizForce_ResetLOS(target);

    const u32 source_flags = source->apiobj.field_0x1f4;
    const u32 source_mask = (-static_cast<u32>(takeover == 0) & 0x10005) - 0x146fe;
    const u32 target_mask = (-static_cast<u32>(takeover == 0) & 0xfffefffb) + 0x146fd;
    const u32 source_kept_flags = source_flags & source_mask;
    source->apiobj.field_0x1f4 = source_kept_flags;

    const u32 target_flags = target->apiobj.field_0x1f4;
    const f32 source_spawn_protection = source->spawn_protection_timer;
    source->spawn_protection_timer = target->spawn_protection_timer;
    target->spawn_protection_timer = source_spawn_protection;

    source->apiobj.field_0x1f4 = (target_flags & target_mask) | source_kept_flags;
    target->apiobj.field_0x1f4 = (source_flags & target_mask) | (target_flags & source_mask);

    const u32 source_field_eb4 = source->field_0xeb4;
    source->field_0xeb4 = target->field_0xeb4;
    target->field_0xeb4 = source_field_eb4;
    AICreatureResumeScript(source);

    if (takeover == 0) {
        source->tag_state = 0.0f;
        source->field_0xef0 = 0;
        source->pause_context_state = 0;
        source->field_0xefc |= 0x80;
        source->input_toggle_hold_time = TOGGLEHOLDTIME;
        source->field_0xefe &= 0xe7;

        target->tag_state = 2.0f;
        target->input_toggle_hold_time = TOGGLEHOLDTIME;
        target->field_0xefc |= 0x80;
        NewBuzzFrames(target->pad_gamepad->pad, 2, 0);

        i8 player_index = source->apiobj.field_0x27c;
        if (player_index == -1) {
            source->hitpoints = source->apiobj.character_data->game_character->hitpoints;
        } else {
            Player[player_index] = source;
            if (source->field_0xcc0 == NULL) {
                if (WORLD->current_level != VADERC_LDATA) {
                    source->hitpoints = static_cast<u8>(DEFAULT_PLAYERHITPOINTS);
                }
            } else {
                source->hitpoints = source->apiobj.character_data->game_character->hitpoints;
            }
        }

        player_index = target->apiobj.field_0x27c;
        if (player_index == -1) {
            target->hitpoints = target->apiobj.character_data->game_character->hitpoints;
        } else {
            Player[player_index] = target;
            if (target->field_0xcc0 == NULL) {
                if (WORLD->current_level != VADERC_LDATA) {
                    target->hitpoints = static_cast<u8>(DEFAULT_PLAYERHITPOINTS);
                }
            } else {
                target->hitpoints = target->apiobj.character_data->game_character->hitpoints;
            }
            GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
        }

        if (static_cast<i8>(source_hitpoints) > static_cast<i8>(target->hitpoints)) {
            source_hitpoints = target->hitpoints;
        }
        target->current_hp = source_hitpoints;
        if (source_hitpoints == 0 && target->hitpoints != 0) {
            target->current_hp = 1;
        }

        if (WORLD->current_level == VADERC_LDATA || static_cast<i8>(source->apiobj.flags_low) < 0) {
            source->current_hp = target_hitpoints;
        } else if ((source->apiobj.character_data->model_flags & 0x20) == 0) {
            source->current_hp = source->hitpoints;
        } else {
            source->current_hp = source->field_0xe38;
        }
    } else {
        const i8 source_index = source->apiobj.field_0x27c;
        if (source_index != -1) {
            Player[source_index] = source;
        }
        const i8 target_index = target->apiobj.field_0x27c;
        if (target_index != -1) {
            Player[target_index] = target;
        }
    }

    target->field_0xefe &= 0xe7;
    target->field_0xef0 = 0;
    target->pause_context_state = 0;
    target->tag_context_timer = 1.0f;

    if (source == player_tag_to && target != player_tag_from) {
        player_tag_to = NULL;
        player_tag_timer = 0.0f;
        player_tag_from = NULL;
        do_player_tag = 0;
    }

    return 1;
}

void Tag_Check(GameObject_s *object) {
    i32 tag_result = 0;
    GameObject_s *target = NULL;
    const u8 context_flags = object->tag_context_flags;
    const u16 required_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_PLAYER_CHARACTER;

    if ((context_flags & 1) != 0 || object->character_context == 0x51) {
        object->field_0xefe &= 0xe7;
        object->tag_state = 0.0f;
        object->field_0xef0 = 0;
        object->pause_context_state = 0;
        if (Arcade == 0) {
            return;
        }
    } else if (Arcade != 0) {
        object->field_0xef0 = 0;
        object->pause_context_state = 0;
        object->field_0xefe &= 0xe7;
    }

    if (object->apiobj.field_0x27c == -1) {
        return;
    }

    if ((context_flags & 4) == 0) {
        object->tag_target_player = -1;

        if (static_cast<i8>(object->apiobj.flags_low) < 0 && object->apiobj.field_0x287 == 0 &&
            (CInfo[object->character_context].flags & 0x800) == 0 && (object->apiobj.flags_high & 1) == 0 &&
            object->tag_context_timer <= 0.0f) {
            if (VehicleArea == 0 && (object->pad_gamepad->buttons_pressed & GAMEPAD_TAG) != 0) {
                if (CUTSKIPLOCK == 0 && Arcade == 0) {
                    if (Tag_Mode == 1 || Tag_Mode == 3) {
                        const i8 context = object->character_context;
                        if (context == 0x17 || context == -1 || (CInfo[context].parameter & 0x10) != 0 ||
                            (CInfo[context].flags & 4) != 0) {
                            target = Tag_FindGameObject_TRANSFER(object);
                        }
                        if (Tag_Mode == 3 && target != NULL) {
                            if (InCollectList_Index(target->id, NULL, 0) == -1) {
                                if (InCollectList_Index(target->id, NULL, 0) == -1) {
                                    GameAudio_PlaySfx(0x32, &object->apiobj.collision_position, 0, 0);
                                    NewRumble(object->pad_gamepad->pad, 0.5f, 0);
                                    AISCRIPTPROCESS *processor = reinterpret_cast<AISCRIPTPROCESS *>(&target->ai);
                                    if (AIScriptSetBaseScriptStateByName(processor, "MapRunAway") != 0) {
                                        AIScriptProcess(WORLD->ai_sys, &target->apiobj, &target->ai, processor, FRAMETIME);
                                    }
                                    target = NULL;
                                }
                            } else {
                                const i32 pack = Store_FindPack(target->id, NULL);
                                if (pack != -1 && static_cast<i32>(target->apiobj.field_0x1f4) < 0 &&
                                    !Store_IsPackUnlocked(pack)) {
                                    Hint_CancelCurrent();
                                    if (NuIOS_AreInAppPurchasesAvailable() == 0 || NuIOS_CanMakeInAppPurchases() == 0) {
                                        GameAudio_PlaySfx(0x32, NULL, 0, 0);
                                        GameCam_HitRoll();
                                    } else {
                                        GameAudio_PlaySfx(0x30, NULL, 0, 0);
                                        menu_i_pack = pack;
                                        NewMenu(0x14, -1, -1);
                                    }
                                    goto draw_icon;
                                }
                            }
                        } else if (Tag_Mode == 1 && target != NULL && target->apiobj.field_0x27c != -1) {
                            object->tag_target_player = target->apiobj.field_0x27c;
                        }
                    } else {
                        u32 object_index = static_cast<u8>(object->apiobj.field_0x289);
                        i32 remaining = HIGHGAMEOBJECT - 1;
                        while (remaining > 0) {
                            ++object_index;
                            if (object_index == static_cast<u32>(HIGHGAMEOBJECT)) {
                                object_index = 0;
                            }
                            target = &Obj[object_index];
                            --remaining;
                            if (target != object && (target->apiobj.field_0x1f8 & required_flags) == required_flags &&
                                target->apiobj.field_0x287 == 0 && target->apiobj.field_0x27c != -1 &&
                                (target->tag_context_flags & 2) == 0 && (target->field_0xf00 & 2) == 0 &&
                                (CInfo[target->character_context].flags & 0x800) == 0) {
                                object->tag_target_player = target->apiobj.field_0x27c;
                                break;
                            }
                        }
                    }

                    if (Tag_Mode != 3) {
                        const i8 target_player = object->tag_target_player;
                        target = target_player >= 0 ? Player[target_player] : NULL;
                    }
                    if (target != NULL) {
                        if ((target->field_0xf00 & 2) != 0) {
                            TakeOver2GetIn(target, object);
                            goto draw_icon;
                        }

                        if (static_cast<i8>(object->apiobj.flags_low) < 0 &&
                            static_cast<i8>(target->apiobj.flags_low) < 0) {
                            if (player_tag_timer <= 0.0f || object != player_tag_to || player_tag_from != target) {
                                player_tag_from = object;
                                player_tag_timer = 0.5f;
                                player_tag_to = target;
                            } else {
                                do_player_tag = 1;
                            }
                            goto draw_icon;
                        }

                        tag_result = TagCode(object, target, 0, 0, 1);
                        if (tag_result == 2) {
                            object->tag_target = target;
                            object->tag_context_timer = 1.0f;
                            object->tag_context_flags = (object->tag_context_flags & 0xf7) | 4;
                            goto draw_icon;
                        }
                        goto tag_complete;
                    }
                } else if (CUTSKIPLOCK != 0) {
                    object->tag_state = 2.0f;
                    object->input_toggle_hold_time = TOGGLEHOLDTIME;
                }
                if (object->field_0xcc0 == NULL) {
                    object->tag_state = 2.0f;
                }
            } else if ((object->pad_gamepad->buttons_pressed & GAMEPAD_TAG) != 0) {
                object->tag_state = 2.0f;
            }
        }
    } else {
        object->tag_context_timer -= FRAMETIME;
        if (object->tag_context_timer > 0.0f && object->tag_target != NULL) {
            target = object->tag_target;
            if ((target->apiobj.field_0x1f8 & required_flags) == required_flags) {
                if ((object->field_0xf00 & 2) == 0) {
                    tag_result = TagCode(object, target, (context_flags >> 3) & 1, 0, 1);
                } else {
                    TakeOverYoda(object, target, 0, 1);
                    tag_result = 0;
                }
                if (tag_result != 0) {
                tag_complete:
                    if (tag_result == 1) {
                        object->tag_context_flags &= 0xfb;
                        GameAudio_PlaySfx(0x21, &object->apiobj.collision_position, 0, 0);
                        if (target->apiobj.model_draw_result == 0) {
                            GameCam_Reset(GameCam);
                        } else {
                            GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
                        }
                        const i32 player_1_id = Player[1] == NULL ? -1 : Player[1]->id;
                        const i32 player_0_id = Player[0] == NULL ? -1 : Player[0]->id;
                        RememberPlayerIDs(0, player_0_id, player_1_id);
                        if ((Tag_Mode & 0xfffffffd) == 1) {
                            Tag_NewTransfer(object, target);
                        }
                        if (static_cast<u8>(object->apiobj.field_0x27c) < 2 &&
                            static_cast<u8>(target->apiobj.field_0x27c) < 2) {
                            ResetTimer(&JoinInTimer, 0.0f);
                        }
                    }
                    goto draw_icon;
                }
            } else {
                object->tag_target = NULL;
            }
        }
        object->tag_context_flags &= 0xfb;
    }

draw_icon:
    if (Tag_DrawIconFn != NULL) {
        Tag_DrawIconFn(object);
    }
}
