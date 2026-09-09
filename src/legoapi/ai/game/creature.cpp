#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nuspecial.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/audio/sfx.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void FreeTorpedoPacket(TORPEDOPACKET_s **packet);
void RemoveGameObject(GameObject_s *object, i32 immediate);

void ClearAICreatures() {
    GameObject_s *object = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++object) {
        if ((object->apiobj.flags_low & 1) != 0 && (object->apiobj.field_0x1f4 & 0x400) != 0) {
            FreeTorpedoPacket(&object->torpedo);
            RemoveGameObject(object, 1);
        }
    }
}

void AICreatureResumeScript(GameObject_s *object) {
    AISCRIPT *script = object->ai.script_process.base_script;
    if (script != NULL) {
        AIScriptProcessorInit(WORLD->ai_sys, &object->ai, &object->ai.script_process, NULL, NULL, NULL, 0, script,
                              script->base_state);
        object->ai.script_process.active_ref_count = 0;
    }
}

extern "C" {
    extern NUVEC plr_lastpos;
    AIGROUP *CreateAIGroup(AISYS *system, i32 count_across, f32 x_spacing, f32 z_spacing, f32 max_speed);
    void AddToAIGroup(AIGROUP *group, APIOBJECT *object);
    void AISysCharacterSetPath(AIPACKET *packet, AIPATH *path);
    void AISysCharacterSetPathCnx(AIPACKET *packet, NUVEC *position, AIPATHCNX *connection, i32 direction);
    void AISysGetCharacterPathPos(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 route, i32 surface_flags);
    void ResetAnimPacket(void *packet, i32 enabled);
    void SetAnimTimeRandom(CHARACTERMODEL_s *model, ANIMPACKET_s *packet);
}

void InitPlayerAI(GameObject_s *object);
void ResetPlayerMoves(GameObject_s *object);
void Player_ClearContext(GameObject_s *object, i32 context);
void Player_ResetContexts(PLAYERPACKET_s *packet);
void InitSurfaceInfo(GameObject_s *object);
i32 SetObjOnSurface(GameObject_s *object, i32 mode);
void PortalGameObject(GameObject_s *object, i32 enable, i32 immediate, i16 portal, nugscn_s *scene);

enum AI_CREATURE_FLAGS : i32 {
    AI_CREATURE_FLAG_FORMATION_REVERSED = 0x01,
    AI_CREATURE_FLAG_SKIP_LOW_END = 0x20,
};

enum AI_CREATURE_RESET_MODE : u8 {
    AI_CREATURE_RESET_READY = 0,
    AI_CREATURE_RESET_STAGGERED = 1,
    AI_CREATURE_RESET_ACTIVE = 2,
    AI_CREATURE_RESET_DISABLED = 4,
};

void InitAICreatures(AISYS_s *system) {
    if (Mission_Active(NULL) != NULL || netclient != 0 || system == NULL) {
        return;
    }

    for (i32 creature_index = 0; creature_index < system->creature_count; ++creature_index) {
        AICREATURE &creature = system->creatures[creature_index];
        if ((creature.flags & AI_CREATURE_FLAG_SKIP_LOW_END) != 0 && g_lowEndLevelBehaviour != 0) {
            continue;
        }

        i32 count = creature.count;
        if (count == 0) {
            continue;
        }

        AIGROUP *group = NULL;
        for (i32 member = 0; member < count; ++member) {
            if (NOAICREATURES != 0 && (GCDataList[creature.type].flags_090 & 0x40) == 0) {
                continue;
            }
            if (((static_cast<u64>(creature.active_mask) >> member) & 1) == 0) {
                continue;
            }

            GameObject_s *object = AddCreature(creature.type, 1);
            if (object == NULL) {
                count = creature.count;
                continue;
            }

            object->apiobj.flags_high &= static_cast<u8>(~APIOBJECT_HIGH_FLAG_CHARACTER);
            object->apiobj.field_0x1f4 |= APIOBJECT_MOTION_FLAG_AI_CONTROLLED;

            const u32 model_flags = apicharsys->char_data[creature.type].model_flags;
            if ((model_flags & 0x200) != 0) {
                object->apiobj.field_0x1f4 |= 0x404;
            } else if ((model_flags & 0x4) != 0) {
                object->apiobj.field_0x1f4 |= 0x401;
            }
            object->field_0x1050 |= (model_flags & 0x1000) != 0 ? 5 : 1;
            object->ai.field_0x134 = static_cast<u8>(creature_index);

            if (member == 0 && creature.count > 1 && creature.start_stagger == 0.0f) {
                GAMECHARACTERDATA *character =
                    static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                group = CreateAIGroup(system, creature.count_across, creature.x_spacing, creature.z_spacing,
                                      character->movement_speed);
            }
            if (group != NULL) {
                AddToAIGroup(group, &object->apiobj);
            }

            object->ai.area = creature.area;
            object->ai.locator = creature.locator;
            object->ai.respawn_locator = creature.respawn_locator;
            object->ai.creature_set = creature.set;
            count = creature.count;
        }
    }

    system->has_done_reset = 0;
}

void ResetAICreature(GameObject_s *object, AISYS_s *system) {
    if (object == NULL) {
        return;
    }

    PreResetCode(object);
    PostResetCode(object);
    if (system == NULL) {
        return;
    }

    InitPlayerAI(object);
    ResetPlayerMoves(object);
    Player_ClearContext(object, 1);
    Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));

    GAMECHARACTERDATA *character = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    object->hitpoints = character->hitpoints;
    object->current_hp = character->hitpoints;
    object->field_0xe38 = 4;
    object->field_0xe37 = character->field_0xf5;

    AICREATURE &creature = system->creatures[object->ai.field_0x134];
    object->ai.creature_set = creature.set;
    object->ai.field_0x1e6 &= ~4u;
    object->ai.field_0x1e4 = static_cast<u8>((object->ai.field_0x1e4 & 0x7f) |
                                             ((creature.flags & AI_CREATURE_FLAG_FORMATION_REVERSED) << 7));
    object->apiobj.viewdistance = creature.view_distance;
    object->apiobj.heardistance = creature.hear_distance;
    object->apiobj.maxviewheight = creature.max_view_height;
    object->apiobj.minviewheight = creature.min_view_height;

    const u8 column = object->ai.group_column;
    const u8 row = object->ai.group_row;
    NUVEC offset = {
        static_cast<f32>((column + 1) / 2) * creature.x_spacing * ((column & 1) != 0 ? -1.0f : 1.0f),
        0.0f,
        -static_cast<f32>(row) * creature.z_spacing,
    };

    i32 angle;
    AIPATHINFO *path_info;
    if (object->ai_respawn_count != 0 && object->ai.respawn_locator != NULL) {
        AILOCATOR *locator = object->ai.respawn_locator;
        angle = locator->flags;
        NuVecRotateY(&offset, &offset, angle);
        NuVecAdd(&object->apiobj.position, &offset, &locator->position);
        path_info = &locator->path_info;
        object->ai.respawn_locator = NULL;
    } else {
        angle = creature.y_rot;
        NuVecRotateY(&offset, &offset, angle);
        NuVecAdd(&object->apiobj.position, &offset, &creature.pos);
        path_info = &creature.path_info;
    }

    object->apiobj.pos_x = object->apiobj.position.x;
    object->apiobj.pos_y = object->apiobj.position.y;
    object->apiobj.pos_z = object->apiobj.position.z;
    object->apiobj.start_position = object->apiobj.position;
    object->apiobj.initial_position = object->apiobj.position;
    plr_lastpos = object->apiobj.position;
    object->apiobj.velocity = v000;
    object->reset_velocity = v000;

    InitSurfaceInfo(object);
    SetObjOnSurface(object, 0);
    object->apiobj.field_0x276 = static_cast<u16>(angle);
    object->apiobj.facing_angle = static_cast<u16>(angle);
    object->apiobj.movement_facing_angle = static_cast<u16>(angle);
    NuVecRotateYValZ(&object->facing_direction, 1.0f, angle & 0xffff);

    AIPACKET *packet = reinterpret_cast<AIPACKET *>(&object->ai);
    AISysCharacterSetPath(packet, path_info->path);
    AISysCharacterSetPathCnx(packet, &object->apiobj.position, path_info->connection, path_info->direction);
    object->apiobj.field_0x287 = 0;

    AISCRIPTPROCESS *processor = &object->ai.script_process;
    AIScriptProcessorInit(WORLD->ai_sys, packet, processor, &creature, creature.script_name, NULL, 1, NULL, NULL);
    object->ai.field_0x138 = 0xff;
    object->ai.field_0x139 = 0;
    object->ai.field_0x124 = -1;
    AISysGetCharacterPathPos(WORLD->ai_sys, &object->apiobj, packet, 0xff, static_cast<i8>(object->apiobj.field_0x27d));

    ResetAnimPacket(&object->apiobj.anim_packet, 1);
    SetAnimTimeRandom(object->apiobj.character_model, &object->apiobj.anim_packet);
    object->ai.field_0x1e7 &= ~1u;
    object->field_0x1092 = 0;
    object->field_0x1093 = 0;
    object->ai.field_0x110 = 0;
    object->field_0x109c = 0;
    object->ai.reset_position = object->apiobj.position;
    object->ai.movement_target = NULL;
    PortalGameObject(object, 1, 1, -1, WORLD->current_gscn);

    object->apiobj.flags_high |= APIOBJECT_HIGH_FLAG_CHARACTER;
    ++object->ai_respawn_count;
    object->ai.reset_mode = AI_CREATURE_RESET_ACTIVE;

    if (object->ai.group != NULL) {
        AIGROUP *group = object->ai.group;
        if (object->ai.group_member_index < 32) {
            group->member_is_alive |= 1u << object->ai.group_member_index;
        }

        AIROW &group_row = group->rows[object->ai.group_row];
        const i32 member_column = object->ai.group_member_index - creature.count_across * object->ai.group_row;
        if (static_cast<u32>(member_column) < 8) {
            group_row.is_alive |= static_cast<u8>(1u << member_column);
        }
        if (object->ai.group_column == 0) {
            group_row.pos = object->apiobj.position;
            group_row.path_info = object->ai.path_info;
            group_row.y_rot = object->apiobj.field_0x276;
            group_row.is_turning = 0;
            group_row.next_connection = NULL;
        }
    }

    object->apiobj.previous_position[0] = object->apiobj.position.x;
    object->apiobj.previous_position[1] = object->apiobj.position.y;
    object->apiobj.previous_position[2] = object->apiobj.position.z;
    object->field_0x10c8 = object->apiobj.position.x;
    object->field_0x10cc = object->apiobj.position.y;
    object->field_0x10d0 = object->apiobj.position.z;
    if ((object->apiobj.character_data->model_flags & 0x20000000) != 0 &&
        object->apiobj.character_model->model_data_b[0x41] != NULL) {
        object->context_animation_timer = 1000000000.0f;
        object->field_0x7a5 = 0x17;
        object->context_animation = 0x41;
        ResetAnimPacket(&object->apiobj.anim_packet, 0x41);
    }
}

void SnapCreaturePos(GameObject_s *object, nuvec_s *position, i32 angle, AIPATHINFO_s *path_info, i32 set_on_surface) {
    object->apiobj.position = *position;
    object->apiobj.field_0x276 = angle;
    object->apiobj.facing_angle = angle;
    object->apiobj.movement_facing_angle = angle;
    object->apiobj.initial_position = object->apiobj.position;
    object->apiobj.collision_position = object->apiobj.position;
    plr_lastpos = object->apiobj.position;
    object->apiobj.start_position = object->apiobj.position;
    object->apiobj.respawn_position = object->apiobj.position;
    object->apiobj.last_safe_position = object->apiobj.position;
    object->ai_update_position = object->apiobj.position;
    object->reset_velocity = v000;
    object->apiobj.velocity = v000;
    InitSurfaceInfo(object);
    if (set_on_surface != 0) {
        SetObjOnSurface(object, 0);
    }
    if (path_info != NULL) {
        object->ai.path_info = *path_info;
    } else {
        AISysGetCharacterPathPos(WORLD->ai_sys, &object->apiobj, &object->ai, 0xff, 1);
    }
}

void ResetAICreatures(AISYS_s *system) {
    if (system == NULL || system->has_done_reset != 0) {
        return;
    }
    system->has_done_reset = 1;

    GameObject_s *objects = Obj;
    for (i32 object_index = 0; object_index < HIGHGAMEOBJECT; ++object_index, ++objects) {
        GameObject_s &object = *objects;
        if ((object.apiobj.field_0x1f8 & APIOBJECT_FLAG_IN_USE) == 0 ||
            (object.apiobj.field_0x1f4 & APIOBJECT_MOTION_FLAG_AI_CONTROLLED) == 0) {
            continue;
        }

        object.apiobj.flags_high &= static_cast<u8>(~APIOBJECT_HIGH_FLAG_CHARACTER);
        LEVEL_PROGRESS_s *progress = WORLD->level_progress;
        const bool permanently_removed =
            progress != NULL && object_index < 64 &&
            (progress->disabled_ai_object_mask[object_index / 32] & (1u << (object_index & 31))) != 0;
        if (permanently_removed) {
            object.ai.reset_mode = AI_CREATURE_RESET_DISABLED;
            object.apiobj.field_0x287 = 1;
            continue;
        }

        AICREATURE &creature = system->creatures[object.ai.field_0x134];
        object.ai_respawn_count = 0;
        object.ai.reset_mode = AI_CREATURE_RESET_READY;

        AIPACKET *packet = reinterpret_cast<AIPACKET *>(&object.ai);
        AISCRIPTPROCESS *processor = &object.ai.script_process;
        AIScriptProcessorInit(WORLD->ai_sys, packet, processor, &creature, creature.script_name, "InActive", 1, NULL, NULL);
        if (processor->state != NULL && processor->state->name != NULL && NuStrICmp(processor->state->name, "InActive") == 0) {
            creature.activate_type = 2;
        }

        if (creature.activate_type != 0) {
            continue;
        }
        if (Game.save_version < creature.activation_difficulty) {
            object.ai.reset_mode = AI_CREATURE_RESET_DISABLED;
            continue;
        }
        if (creature.count > 1 && creature.start_stagger > 0.0f && object.ai.group_member_index != 0) {
            object.ai.reset_mode = AI_CREATURE_RESET_STAGGERED;
            object.ai_spawn_delay = static_cast<f32>(static_cast<u32>(object.ai.group_member_index)) * creature.start_stagger;
            continue;
        }

        ResetAICreature(&object, system);
    }
}

void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);
void NewRumbleAllPlayers(f32, f32, i32, i32);
i32 ObjHitObj_Flags(GameObject_s *);

void CreatureCrate_Stop(PART_s *part) {
    GameObject_s *object = part->owner;
    if (object == NULL)
        return;
    i16 debris = FindGameDebris(WORLD->debris_sys, "CRATE_POP");
    if (debris != -1)
        AddGameDebris(WORLD->debris_sys, debris, &part->position);
    i32 type = PARTLookupType("CRATE_PART");
    if (type != -1)
        AddFiniteShotPART(type, &part->position, 1);
    object->ai.reset_mode = 2;
    object->apiobj.field_0x1f8 |= 0x1000;
    PlaySfx("Explode1", &part->position);
    GameCam_Judder(GameCam, 0.1f, 0, NULL);
    NewRumbleAllPlayers(0.0f, 0.0f, 2, 0);
}

i32 CreatureCrate_DrawFn(PART_s *part) {
    return !(part->crate_spawn_delay > 0.0f);
}

void CreatureCrate_MoveFn(PART_s *part, f32 elapsed) {
    part->crate_spawn_delay -= elapsed;
    if (part->crate_spawn_delay <= 0.0f) {
        part->move_callback = NULL;
        part->draw_callback = NULL;
    }
}

void SpawnCreatureFromCrate(GameObject_s *object, f32 height, f32 delay) {
    NUVEC velocity = {0.0f, 0.0f, 0.0f};
    if (!NuSpecialExistsFn(&WORLD->lev_objs[0xe6].special))
        return;
    NUMTX matrix;
    NuMtxSetTranslation(&matrix, &object->apiobj.collision_position);
    ADDPART_s parameters = Default_ADDPART;
    parameters.flags = 0x313;
    parameters.matrix = &matrix;
    parameters.velocity = &velocity;
    matrix.m31 += height;
    parameters.special = &WORLD->lev_objs[0xe6].special;
    parameters.owner = object;
    parameters.field_14 = 0.1f;
    parameters.field_18 = 0.1f;
    parameters.gravity = -2.0f;
    parameters.draw_fn = CreatureCrate_DrawFn;
    parameters.move_fn = CreatureCrate_MoveFn;
    parameters.stop_fn = CreatureCrate_Stop;
    parameters.time_step = FRAMETIME;
    PART_s *part = AddPart(&parameters);
    if (part == NULL)
        return;
    part->force_flags = static_cast<u16>(ObjHitObj_Flags(object));
    if (!(delay > 0.0f)) {
        part->move_callback = NULL;
        part->draw_callback = NULL;
        delay = 0.0f;
    }
    part->crate_spawn_delay = delay;
    object->ai.reset_mode = 3;
    object->apiobj.field_0x1f8 &= ~0x1000;
}

void SpawnMeleeCreatureType(i32) {
}

GameObject_s *alert_obj;
NUVEC alert_pos;
f32 alert_timer;
f32 alert_time = 10.0f;

void AlertSurroundingCreatures(GameObject_s *object, nuvec_s *position) {
    if (object != NULL &&
        ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001 || (object->apiobj.field_0x1f4 & 5) != 0 ||
         (static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_090 & 0x8000) != 0))
        return;
    alert_obj = object;
    alert_pos = *position;
    alert_timer = alert_time;
}
