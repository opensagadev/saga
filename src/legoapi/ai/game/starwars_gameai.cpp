#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/characters/motion.h"
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/world/world.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"

struct AIROW_s;
struct AISYS_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

typedef i32 (*MIDSPECIALMOVEFN)(AISYS_s *, AIPACKET_s *, APIOBJECT_s *);
typedef void (*HUBCALLBACK)(WORLDINFO_s *);

extern "C" void InitFn_PreparingForSpecialMove(PREPARINGSPECIALMOVEFN);
extern "C" void InitFn_MidSpecialMove(MIDSPECIALMOVEFN);

extern i32 LEGO_AIPATHCNX_JUMP;
extern i32 LEGO_AIPATHCNX_DOUBLE_JUMP;
extern i32 LEGO_AIPATHCNX_HIGH_JUMP;
extern i32 LEGO_AIPATHCNX_R2D2GLIDE;
extern i32 LEGO_AIPATHCNX_FORGOODIES;
extern i32 LEGO_AIPATHCNX_FORBADDIES;
extern i32 LEGO_AIPATHCNX_BLOCKAGE;
extern i32 LEGO_AIPATHCNX_DONTTOGGLE;
extern i32 LEGO_AIPATHCNX_FULLTERRAIN;
extern i32 LEGO_AIPATHCNX_BIGJUMP;
extern i32 LEGO_AIPATHCNX_REQUIRESPERMISSION;
extern i32 LEGO_AIPATHCNX_NO_DESTINATION_CHECK;
extern i32 LEGO_AIPATHCNX_JUMP_NOW;
extern i32 LEGO_AIPATHCNX_DONT_JUMP_NOW;
extern i32 mechAutoJumpFlags;
extern i32 mechAutoJumpCantReachFlags;

extern HUBCALLBACK Hub_InitAIFn;
extern HUBCALLBACK Hub_ResetAIFn;
extern HUBCALLBACK Hub_UpdateAIFn;

extern void LSW_Hub_InitAI(WORLDINFO_s *);
extern void LSW_Hub_ResetAI(WORLDINFO_s *);
extern void LSW_Hub_UpdateAI(WORLDINFO_s *);
extern "C" i16 id_SNAKE;

enum STARWARS_AI_CAPABILITY : u32 {
    STARWARS_AI_CAPABILITY_DEFAULT = 0x20180040,
    STARWARS_AI_CAPABILITY_OBJECT_STATE_SET = 0x100,
    STARWARS_AI_CAPABILITY_OBJECT_STATE_CLEAR = 0x80,
    STARWARS_AI_CAPABILITY_MODEL_ATTACHMENT = 0x1,
    STARWARS_AI_CAPABILITY_MODEL_FLAG_8_EXCLUSIVE = 0x1000,
    STARWARS_AI_CAPABILITY_MODEL_FLAG_8 = 0x3,
    STARWARS_AI_CAPABILITY_EXTENDED_JUMP = 0x23,
    STARWARS_AI_CAPABILITY_MODEL_FLAG_40000 = 0x10,
    STARWARS_AI_CAPABILITY_MODEL_FLAG_40 = 0x4004,
    STARWARS_AI_CAPABILITY_MODEL_FLAG_100000 = 0x8,
    STARWARS_AI_CAPABILITY_MODEL_FLAG_2000 = 0x10000,
    STARWARS_AI_CAPABILITY_SPECIAL_MOVEMENT = 0x8000,
    STARWARS_AI_CAPABILITY_PLAYER_SLOT = 0x20000,
    STARWARS_AI_CAPABILITY_SNAKE = 0x4000,
};

extern void SetSpecialMove(GameObject_s *, AIPATHNODE_s *, AIPATHNODE_s *, char);
extern void ReleaseTakeOver(GameObject_s *, i32);

static i32 StarWars_PrepareTakeOverJump(AIPACKET_s *packet, APIOBJECT_s *object, i32) {
    GameObject_s *owner = object->objptr;
    i32 result = 0;
    if ((owner->apiobj.character_data->game_character->flags_090 & 0x40) != 0) {
        result = 1;
        GameObject_s *participant = owner->field_0xcc0;
        if (participant != NULL) {
            AIPATHNODE *destination =
                &packet->path_info.path
                     ->nodes[packet->path_info.connection->node_indices[packet->path_info.direction == 0]];
            ReleaseTakeOver(participant, 1);
            StartBigJump(participant, &destination->position, 0, 0.5f, 1.0f, 0, 0);
            SetSpecialMove(participant, destination, NULL, 3);
        }
    }
    return result;
}

static i32 StarWars_PrepareObstacle(AIPACKET_s *packet, APIOBJECT_s *object, i32) {
    i32 near_index = packet->path_info.dist > 0.5f;
    i32 far_index = near_index == 0;
    AIPATHCNX *connection = packet->path_info.connection;
    AIPATHNODE *near_node = &packet->path_info.path->nodes[connection->node_indices[near_index]];
    AIPATHNODE *far_node = &packet->path_info.path->nodes[connection->node_indices[far_index]];
    NUVEC offset;
    offset.z = far_node->position.z - near_node->position.z;
    offset.x = far_node->position.x - near_node->position.x;
    offset.y = 0.0f;
    f32 radius = near_node->radius - packet->mover_height;
    if (radius < 0.0f) {
        radius = 0.0f;
    }
    NuVecScale(&offset, &offset, radius / connection->horizontal_distance);
    NuVecAdd(&packet->movement_destination, &near_node->position, &offset);
    offset.x = packet->path_info.width;
    offset.y = 0.0f;
    offset.z = 0.0f;
    NuVecRotateY(&offset, &offset, packet->path_info.connection->rotation);
    NuVecAdd(&packet->movement_destination, &packet->movement_destination, &offset);
    packet->movement_stopping_distance = 0.0f;
    f32 distance = NuVecXZDistSqr(&object->position, &packet->movement_destination, &offset);
    if (distance < ai_moveradius * ai_moveradius) {
        AIPATHCNX *destination = packet->fallback_path_info.connection;
        AIPATHCNX *current = packet->path_info.connection;
        if (destination == current) {
            f32 edge = far_node->radius / destination->horizontal_distance;
            if (far_index != 0) {
                if (1.0f - edge > packet->fallback_path_info.dist) {
                    goto reset_connection;
                }
            } else if (packet->fallback_path_info.dist > edge) {
                goto reset_connection;
            }
        }
        if (packet->path_info.direction == far_index) {
            return 0;
        }
        if (packet->path_connection_state == 0) {
            if (current->open == 0) {
                packet->path_connection_state = 1;
            }
        } else if (packet->path_connection_state == 1 && current->open != 0) {
            SetSpecialMove(object->objptr, far_node, NULL, 0);
        }
    } else {
    reset_connection:
        packet->path_connection_state = 0;
    }
    object->collision_priority |= 0x2000;
    return 1;
}

static __used__ i32 StarWars_PrepareHoverTube(AIPACKET_s *packet, APIOBJECT_s *apiobject, i32) {
    GameObject_s *object = apiobject->objptr;
    AIPATH_s *path = packet->path_info.path;
    AIPATHCNX_s *connection = packet->path_info.connection;
    i32 from = connection->node_indices[packet->path_info.direction];
    i32 to = connection->node_indices[packet->path_info.direction == 0];
    AIPATHNODE_s *target = &path->nodes[to];
    packet->movement_destination = path->nodes[from].position;
    packet->movement_stopping_distance = 0.0f;
    if ((connection->traversal_flags[packet->path_info.direction] & 0x200) == 0 &&
        ((path->previous_inside_node_bits[to >> 3] >> (to & 7)) & 1) != 0) {
        if (TryToTeleportToNextNode(object, target, 0) != 0) {
            return 1;
        }
        packet->path_connection_state = 0;
    } else {
        if (packet->path_connection_state != 0 && TryToTeleportToNextNode(object, target, 0) != 0) {
            return 1;
        }
        switch (packet->path_connection_state) {
            case 0:
                if (ObjInTube(object) == 0) {
                    break;
                }
                packet->path_connection_state = 1;
                // Fall through.
            case 1:
                if (ObjInTube(object) != 0 &&
                    !(object->apiobj.collision_min.y > static_cast<TUBE *>(object->field_0x788)->top &&
                      object->apiobj.velocity.y > 0.0f)) {
                    break;
                }
                // Fall through.
            case 2:
                packet->path_connection_state = 0;
                SetSpecialMove(object, target, NULL, 0);
                packet->movement_destination = target->position;
                packet->movement_stopping_distance = 0.0f;
                break;
        }
    }
    apiobject->movement_request_flags |= 0x2000;
    return 1;
}

extern "C" AIPATHNODE *AIPathFindNode(AISYS *, AIPATH *, char *);

static i32 StarWars_PrepareBigJump(AIPACKET_s *packet, APIOBJECT_s *object, i32) {
    GameObject_s *owner = object->objptr;
    AIPATHNODE *nodes = packet->path_info.path->nodes;
    i32 source_index = packet->path_info.connection->node_indices[packet->path_info.direction];
    i32 destination_index = packet->path_info.connection->node_indices[packet->path_info.direction == 0];
    if (owner->ai.inside_path_node == destination_index || owner->character_context == 0x2b) {
        return 0;
    }
    if ((packet->path_info.connection->traversal_flags[packet->path_info.direction] & 0x800) != 0) {
        packet->movement_destination = nodes[source_index].position;
        packet->runtime_flags |= 0x80;
        packet->movement_stopping_distance = 0.0f;
        object->collision_priority |= 0x2000;
        return 1;
    }
    AIPATHNODE *destination = &nodes[destination_index];
    if (TryToTeleportToNextNode(owner, destination, 0) != 0) {
        return 1;
    }
    if (WORLD->current_level == TEMPLEB_LDATA && destination->name != NULL &&
        NuStrICmp(destination->name, "stairs_top") == 0) {
        AIPATHNODE *alternate = AIPathFindNode(WORLD->ai_sys, NULL, "stairs_top2");
        if (alternate != NULL) {
            destination = alternate;
        }
    }
    StartBigJump(owner, &destination->position, 0, 0.5f, 1.0f, 0, 0);
    SetSpecialMove(owner, destination, NULL, 3);
    return 0;
}

extern void GameObjectSetCanUse(GameObject_s *, void *, u8, u8, f32);

static i32 StarWars_PrepareHatch(AIPACKET_s *packet, APIOBJECT_s *object, i32) {
    GameObject_s *owner = object->objptr;
    if (!(owner->special_move_timer <= 0.0f)) {
        packet->movement_destination = object->position;
        packet->movement_stopping_distance = 0.0f;
        object->collision_priority |= 0x2000;
        return 1;
    }
    AIPATHNODE *destination =
        &packet->path_info.path->nodes[packet->path_info.connection->node_indices[packet->path_info.direction == 0]];
    if (owner->character_context == 0x47 || owner->character_context == 0x0f ||
        (owner->character_context == 0 && owner->action_movement_state == 6)) {
        packet->movement_destination = destination->position;
        packet->movement_stopping_distance = 0.0f;
        SetSpecialMove(owner, destination, NULL, 2);
        object->collision_priority |= 0x2000;
        return 1;
    }
    if (TryToTeleportToNextNode(owner, destination, 0) != 0) {
        return 1;
    }
    AIPATHCNX *connection = packet->path_info.connection;
    AIPATHNODE *source = &packet->path_info.path->nodes[connection->node_indices[packet->path_info.direction]];
    packet->movement_destination = source->position;
    packet->movement_stopping_distance = 0.0f;
    owner->pad_gamepad->buttons_held |= GAMEPAD_SPECIAL;
    owner->pad_gamepad->buttons_pressed |= GAMEPAD_SPECIAL;
    u32 capabilities = connection->traversal_flags[packet->path_info.direction] & packet->capabilities;
    if ((capabilities & 0x10) != 0) {
        GameObjectSetCanUse(owner, NULL, 3, 1, 0.0f);
    } else if ((capabilities & 8) != 0) {
        GameObjectSetCanUse(owner, NULL, 4, 1, 0.0f);
    }
    object->collision_priority |= 0x2000;
    return 1;
}

i32 StarWars_PrepareJump(AIPACKET_s *, APIOBJECT_s *, i32);
i32 StarWars_PrepareR2D2Glide(AIPACKET_s *, APIOBJECT_s *, i32);

struct STARWARS_PREPARE_SPECIAL_MOVE {
    u32 capabilities;
    PREPARINGSPECIALMOVEFN prepare;
};
DECOMP_ASSERT(sizeof(STARWARS_PREPARE_SPECIAL_MOVE) == 8, "special move preparation entry size");
DECOMP_ASSERT(offsetof(STARWARS_PREPARE_SPECIAL_MOVE, prepare) == 4, "special move preparation callback offset");

STARWARS_PREPARE_SPECIAL_MOVE starwars_prepareforspecialmove[] = {
    {0x23, StarWars_PrepareJump},           {4, StarWars_PrepareR2D2Glide},
    {0x18, StarWars_PrepareHatch},          {0x40, StarWars_PrepareHoverTube},
    {0x1000, StarWars_PrepareBigJump},      {0x8000, StarWars_PrepareTakeOverJump},
    {0x20000000, StarWars_PrepareObstacle}, {0, NULL},
};

static i32 StarWars_PreparingForSpecialMove(AIPACKET_s *packet, APIOBJECT_s *object, i32 checks) {
    GameObject_s *owner = object->objptr;
    if (owner->character_context == 0x5b) {
        owner->pad_gamepad->buttons_pressed |= GAMEPAD_JUMP;
    }
    u32 flags = packet->path_info.connection->traversal_flags[packet->path_info.direction];
    if (flags != 0 && (flags & 0x40000000) == 0) {
        u32 capabilities = flags & packet->capabilities;
        if ((flags & 0x8000) != 0 && (owner->apiobj.character_data->game_character->flags_090 & 0x40) == 0 &&
            owner->takeover_target == NULL && owner->character_context != 0x3c && (capabilities & 0xffff7fff) == 0) {
            AIScriptSetBaseScriptStateByName(&object->ai->script_process, "TakeOverJump");
            return 1;
        }
        if (capabilities != 0 && (flags & 0x98000000) == 0) {
            STARWARS_PREPARE_SPECIAL_MOVE *entry = starwars_prepareforspecialmove;
            for (; entry->prepare != NULL; ++entry) {
                if ((entry->capabilities & capabilities) != 0) {
                    if (entry->prepare(packet, object, checks) != 0) {
                        return 1;
                    }
                    break;
                }
            }
        } else {
            packet->movement_destination = object->position;
            packet->movement_stopping_distance = 0.0f;
            object->collision_priority |= 0x2000;
            return 1;
        }
    }
    if ((packet->movement_event_flags & 0x20) != 0 && (owner->tag_context_flags & 2) == 0 &&
        drop_back_in_timer > 0.0f &&
        (packet->path_info.connection->traversal_flags[packet->path_info.direction] & 0xd8000000) == 0) {
        TryToTeleportToNextNode(
            owner,
            &packet->path_info.path
                 ->nodes[packet->path_info.connection->node_indices[packet->path_info.direction == 0]],
            0);
    }
    return 0;
}

static i32 StarWars_MidSpecialMove_BigJump(AISYS_s *, AIPACKET_s *, APIOBJECT_s *object) {
    GameObject_s *owner = object->objptr;
    if (owner->character_context == 0x1f) {
        owner->context_destination = owner->ai.special_move_node->position;
        object->collision_priority |= 0x4000;
        return 1;
    }
    ClearSpecialMove(owner);
    return 0;
}

static i32 StarWars_MidSpecialMove_UseHatch(AISYS_s *, AIPACKET_s *, APIOBJECT_s *object) {
    GameObject_s *owner = object->objptr;
    switch (owner->field_0x1093) {
        case 0:
            if (owner->character_context == 0x47 || owner->character_context == 0x0f ||
                (owner->character_context == 0 && owner->action_movement_state == 6)) {
                owner->ai.movement_destination = owner->ai.special_move_node->position;
                owner->ai.movement_stopping_distance = 0.0f;
                object->collision_priority |= 0x4000;
                return 1;
            }
            owner->special_move_timer = 1.0f;
            owner->field_0x1093 = 1;
            // Continue with the exit-node check on the transition frame.
        case 1:
            owner->ai.movement_destination = owner->ai.special_move_node->position;
            owner->ai.movement_stopping_distance = 0.0f;
            if (owner->ai.inside_path_node == owner->ai.special_move_node - owner->ai.path_info.path->nodes ||
                0.0f >= owner->special_move_timer) {
                ClearSpecialMove(owner);
                return 0;
            }
            object->collision_priority |= 0x4000;
            return 1;
    }
    return 0;
}

static i32 StarWars_MidSpecialMove_StartJump(AISYS_s *, AIPACKET_s *, APIOBJECT_s *object) {
    GameObject_s *owner = object->objptr;
    if (owner->character_context == 0) {
        SetSpecialMove(owner, owner->ai.special_move_node, NULL, 0);
        owner->ai.movement_destination = owner->ai.special_move_node->position;
        owner->ai.movement_stopping_distance = 0.0f;
    } else {
        owner->pad_gamepad->buttons_pressed |= GAMEPAD_JUMP;
        object->collision_priority |= 0x4000;
    }
    return 1;
}

extern TERRSET *CurTerr;

static i32 StarWars_MidSpecialMove_Default(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object) {
    GameObject_s *owner = object->objptr;
    u32 capabilities =
        packet->path_info.connection->traversal_flags[packet->path_info.direction] & packet->capabilities;
    AIPATHNODE *nodes = packet->path_info.path->nodes;
    AIPATHNODE *source = &nodes[packet->path_info.connection->node_indices[packet->path_info.direction]];
    AIPATHNODE *destination = &nodes[packet->path_info.connection->node_indices[packet->path_info.direction == 0]];
    NuVecSub(&owner->special_move_look_position, &destination->position, &source->position);
    NuVecNorm(&owner->special_move_look_position, &owner->special_move_look_position);
    NuVecAdd(&owner->special_move_look_position, &owner->special_move_look_position, &destination->position);
    packet->movement_look_target = &owner->special_move_look_position;
    AIPATHNODE *node = owner->ai.special_move_node;
    i32 check_inside = 1;
    if (node->has_special != 0 && (node->runtime_flags & 2) != 0) {
        AIPATH *path = owner->ai.path_info.path;
        if (path != NULL) {
            i32 index = node - path->nodes;
            if (((path->updated_node_bits[index / 8] >> (index % 8)) & 1) == 0) {
                AIPathNodeUpdatePos(system, path, node);
            }
        }
        check_inside = 0;
        if (owner->apiobj.field_0x27d != 0 && owner->apiobj.supporting_platform_id != -1 && CurTerr != NULL) {
            i32 instance = NuSpecialGetInstanceix(&owner->ai.special_move_node->special_handle);
            check_inside = instance == static_cast<i16>(
                                           CurTerr->platforms[owner->apiobj.supporting_platform_id].scene_object_index);
        }
        node = owner->ai.special_move_node;
    }
    if (object->respawn_timer > 2.0f ||
        (check_inside != 0 && owner->ai.inside_path_node == node - owner->ai.path_info.path->nodes)) {
        ClearSpecialMove(owner);
    } else if (owner->character_context != 0 &&
               (node->min_height > owner->apiobj.position.y ||
                (packet->path_info.connection->traversal_flags[packet->path_info.direction] & 0x80000000) != 0 ||
                ((capabilities & 0x40) != 0 && owner->apiobj.field_0x27d != 0))) {
        ClearSpecialMove(owner);
    }
    node = owner->ai.special_move_node;
    if (node == NULL) {
        return 0;
    }
    if ((capabilities & 0x40) == 0) {
        if ((capabilities & 4) != 0) {
            if (packet->terrain_origin.y >= destination->min_height) {
                owner->pad_gamepad->buttons_held |= GAMEPAD_JUMP;
                if (owner->field_0xe31 == 3 || owner->field_0xe31 == 0) {
                    owner->pad_gamepad->buttons_pressed |= GAMEPAD_JUMP;
                }
            }
        } else if ((owner->field_0xef9 & 0x10) != 0 && 0.0f > owner->apiobj.velocity.y) {
            owner->pad_gamepad->buttons_pressed |= GAMEPAD_JUMP;
            owner->field_0xef9 &= ~0x10;
        }
    }
    owner->ai.movement_destination = node->position;
    owner->ai.movement_stopping_distance = 0.0f;
    object->collision_priority |= 0x4000;
    return 1;
}

MIDSPECIALMOVEFN starwars_midspecialmovefns[] = {
    StarWars_MidSpecialMove_Default,
    StarWars_MidSpecialMove_StartJump,
    StarWars_MidSpecialMove_UseHatch,
    StarWars_MidSpecialMove_BigJump,
};

static i32 StarWars_MidSpecialMove(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object) {
    GameObject_s *owner = object->objptr;
    owner->special_move_timer -= FRAMETIME;
    if (owner->ai.special_move_node != NULL) {
        if (TryToTeleportToNextNode(owner, owner->ai.special_move_node, 0) != 0) {
            ClearSpecialMove(owner);
        } else {
            MIDSPECIALMOVEFN function = starwars_midspecialmovefns[static_cast<i8>(owner->field_0x1092)];
            if (function != NULL) {
                return function(system, packet, object);
            }
        }
    }
    return 0;
}

f32 jump_stuck_time = 0.1f;

i32 StarWars_PrepareJump(AIPACKET_s *packet, APIOBJECT_s *object, i32 checks) {
    GameObject_s *owner = object->objptr;
    if ((owner->field_0xf03 & 0x40) != 0) {
        return StarWars_PrepareBigJump(packet, object, checks);
    }
    u32 flags = packet->path_info.connection->traversal_flags[packet->path_info.direction];
    u32 capabilities = packet->capabilities;
    if ((packet->path_info.flags & 1) == 0) {
        packet->path_connection_state = 0;
        return 0;
    }
    AIPATHNODE *nodes = packet->path_info.path->nodes;
    i32 source_index = packet->path_info.connection->node_indices[packet->path_info.direction];
    i32 destination_index = packet->path_info.connection->node_indices[packet->path_info.direction == 0];
    AIPATHNODE *destination = &nodes[destination_index];
    NUVEC offset;
    if (destination->radius_squared > NuVecXZDistSqr(&object->position, &destination->position, &offset)) {
        packet->path_connection_state = 0;
        return 0;
    }
    AIPATHNODE *source = &nodes[source_index];
    AIPATHCNX *connection = packet->path_info.connection;
    if ((connection->traversal_flags[packet->path_info.direction] & 0x200) == 0 &&
        packet->inside_path_node != destination_index &&
        ((packet->path_info.path->previous_inside_node_bits[destination_index >> 3] >> (destination_index & 7)) & 1) !=
            0) {
        if (TryToTeleportToNextNode(owner, destination, 0) != 0) {
            return 1;
        }
        packet->path_connection_state = 1;
        connection = packet->path_info.connection;
    } else if (packet->path_connection_state == 0) {
        f32 distance = connection->horizontal_distance * packet->path_info.dist;
        packet->path_connection_state = 1;
        if ((packet->path_info.direction != 0 && source->radius > distance) ||
            (packet->path_info.direction == 0 && distance > connection->horizontal_distance - destination->radius)) {
            packet->path_connection_state = 0;
            return 0;
        }
    }
    offset.x = destination->position.x - source->position.x;
    offset.y = 0.0f;
    offset.z = destination->position.z - source->position.z;
    f32 radius = source->radius;
    if (radius < 0.0f) {
        radius = 0.0f;
    }
    NuVecScale(&offset, &offset, radius / connection->horizontal_distance);
    NuVecAdd(&packet->movement_destination, &source->position, &offset);
    f32 distance = NuVecXZDistSqr(&object->position, &packet->movement_destination, &offset);
    i32 ready = 0;
    if (checks != 0 && (ai_moveradius * ai_moveradius > distance || owner->apiobj.respawn_timer > jump_stuck_time ||
                        owner->character_context == 0x44)) {
        ready = 1;
        packet->movement_destination = object->position;
    }
    packet->movement_stopping_distance = 0.0f;
    f32 jump_distance = ((flags & capabilities) & 0x20) != 0 ? 1.5f : (((flags & capabilities) & 2) != 0 ? 1.2f : 0.8f);
    connection = packet->path_info.connection;
    flags = connection->traversal_flags[packet->path_info.direction];
    if ((flags & 0x800) != 0) {
        packet->path_connection_state = 1;
        packet->runtime_flags |= 0x80;
    }
    switch (packet->path_connection_state) {
        case 1:
            if (ready != 0) {
                if ((flags & 0x200) == 0 && packet->inside_path_node != destination_index &&
                    ((packet->path_info.path->previous_inside_node_bits[destination_index >> 3] >>
                      (destination_index & 7)) &
                     1) != 0) {
                    if (TryToTeleportToNextNode(owner, destination, 0) != 0) {
                        return 1;
                    }
                } else if ((flags & 0x800) == 0) {
                    if ((flags & 0x400) != 0 || ((destination->runtime_flags | source->runtime_flags) & 2) == 0 ||
                        NuSpecialCompare(&source->special_handle, &destination->special_handle) != 0) {
                        packet->path_connection_state = 4;
                    } else {
                        packet->path_connection_state = 2;
                    }
                }
            }
            break;
        case 2:
            if ((flags & 0x400) != 0) {
                packet->path_connection_state = 4;
            } else {
                distance = NuVecDist(&object->position, &destination->position, &offset);
                if (((destination->runtime_flags | source->runtime_flags) & 2) == 0 ||
                    distance - destination->radius > jump_distance) {
                    packet->path_connection_state = 3;
                    owner->jump_destination_distance = (destination->runtime_flags & 2) != 0 ? 0.0f : distance;
                }
            }
            break;
        case 3:
            if ((flags & 0x400) != 0) {
                packet->path_connection_state = 4;
            } else {
                distance = NuVecDist(&object->position, &destination->position, &offset);
                if (((destination->runtime_flags | source->runtime_flags) & 2) == 0) {
                    owner->jump_destination_distance = 0.0f;
                } else if (!(jump_distance > distance - destination->radius)) {
                    break;
                }
                if (distance > owner->jump_destination_distance) {
                    packet->path_connection_state = 4;
                    owner->jump_destination_distance = 0.0f;
                } else {
                    owner->jump_destination_distance = distance;
                }
            }
            break;
        case 4:
            if ((flags & 0x22) != 0) {
                if (0.0f >= owner->jump_reentry_timer) {
                    packet->path_connection_state = 5;
                    owner->field_0xef9 |= 0x10;
                }
                break;
            }
            packet->path_connection_state = 5;
            owner->field_0xef9 &= ~0x10;
            // Single jumps immediately enter the launch state.
        case 5:
            if (TryToTeleportToNextNode(owner, destination, 0) != 0) {
                return 1;
            }
            connection = packet->path_info.connection;
            if ((connection->traversal_flags[packet->path_info.direction] & 0x200) == 0) {
                i32 object_count = HIGHGAMEOBJECT;
                GameObject_s *other = Obj;
                GameObject_s *current_player = player;
                f32 mover_radius = ai_moveradius;
                for (i32 index = 0; index < object_count; ++index, ++other) {
                    if ((other->apiobj.object_flags & 1) == 0 || other == owner ||
                        (other->apiobj.object_flags & 0x1000) == 0 || other->apiobj.field_0x287 != 0) {
                        continue;
                    }
                    if (other != current_player && other->ai.path_info.connection == connection &&
                        other->ai.special_move_node == NULL &&
                        other->ai.path_connection_state < packet->path_connection_state) {
                        continue;
                    }
                    f32 height = destination->position.y;
                    f32 tolerance = 0.5f * mover_radius;
                    if (owner->apiobj.collision_min.y > height + tolerance) {
                        if (other->apiobj.collision_min.y > height + tolerance) {
                            continue;
                        }
                    } else if (height - tolerance > owner->apiobj.collision_min.y &&
                               height - tolerance > other->apiobj.collision_min.y) {
                        continue;
                    }
                    f32 clearance = other->ai.mover_height + other->ai.mover_height + packet->mover_height;
                    offset.x = other->apiobj.position.x - destination->position.x;
                    if (!(clearance > offset.x && offset.x > -clearance)) {
                        continue;
                    }
                    offset.z = other->apiobj.position.z - destination->position.z;
                    if (!(clearance > offset.z) || !(other->apiobj.collision_max.y >= height) ||
                        !(height + owner->apiobj.scaled_height >= other->apiobj.collision_min.y)) {
                        continue;
                    }
                    if (clearance * clearance > offset.x * offset.x + offset.z * offset.z) {
                        packet->path_connection_state = 1;
                        object->collision_priority |= 0x2000;
                        return 1;
                    }
                }
            }
            SetSpecialMove(owner, destination, NULL, 1);
            owner->pad_gamepad->buttons_pressed |= GAMEPAD_JUMP;
            break;
    }
    object->collision_priority |= 0x2000;
    return 1;
}

void StarWars_GameAISysInit() {
    LEGO_AIPATHCNX_JUMP = 1;
    LEGO_AIPATHCNX_DOUBLE_JUMP = 2;
    LEGO_AIPATHCNX_HIGH_JUMP = 0x20;
    LEGO_AIPATHCNX_R2D2GLIDE = 4;
    LEGO_AIPATHCNX_FORGOODIES = 0x80;
    LEGO_AIPATHCNX_FORBADDIES = 0x100;
    LEGO_AIPATHCNX_BLOCKAGE = 0x40000;
    LEGO_AIPATHCNX_DONTTOGGLE = 0x80000;
    LEGO_AIPATHCNX_FULLTERRAIN = 0x100000;
    LEGO_AIPATHCNX_BIGJUMP = 0x1000;
    LEGO_AIPATHCNX_REQUIRESPERMISSION = 0x2000;
    LEGO_AIPATHCNX_NO_DESTINATION_CHECK = 0x200;
    LEGO_AIPATHCNX_JUMP_NOW = 0x400;
    LEGO_AIPATHCNX_DONT_JUMP_NOW = 0x800;

    InitFn_PreparingForSpecialMove(StarWars_PreparingForSpecialMove);
    InitFn_MidSpecialMove(StarWars_MidSpecialMove);

    Hub_InitAIFn = LSW_Hub_InitAI;
    Hub_ResetAIFn = LSW_Hub_ResetAI;
    Hub_UpdateAIFn = LSW_Hub_UpdateAI;
    mechAutoJumpFlags = 0xe00023;
    mechAutoJumpCantReachFlags = 0xffedfe7f;
}

extern "C" i16 id_GEONOSIAN;

i32 StarWars_PrepareR2D2Glide(AIPACKET_s *packet, APIOBJECT_s *object, i32) {
    GameObject_s *owner = object->objptr;
    AIPATHNODE *nodes = packet->path_info.path->nodes;
    AIPATHCNX *connection = packet->path_info.connection;
    AIPATHNODE *destination = &nodes[connection->node_indices[packet->path_info.direction == 0]];
    AIPATHNODE *source = &nodes[connection->node_indices[packet->path_info.direction]];
    NUVEC offset;
    if ((connection->traversal_flags[packet->path_info.direction] & 0x800) != 0) {
        offset.z = destination->position.z - source->position.z;
        offset.x = destination->position.x - source->position.x;
        offset.y = 0.0f;
        f32 radius = source->radius - packet->mover_height;
        if (radius < 0.0f) {
            radius = 0.0f;
        }
        NuVecScale(&offset, &offset, radius / connection->horizontal_distance);
        NuVecAdd(&packet->movement_destination, &source->position, &offset);
        packet->runtime_flags |= 0x80;
        packet->movement_stopping_distance = 0.0f;
        object->collision_priority |= 0x2000;
        return 1;
    }
    if (!(packet->terrain_origin.y >= destination->min_height)) {
        return 0;
    }
    if (!(NuVecXZDistSqr(&object->position, &destination->position, &offset) >= destination->radius_squared)) {
        return 0;
    }
    if (TryToTeleportToNextNode(owner, destination, 0) != 0) {
        return 0;
    }
    packet->path_connection_state = 0;
    SetSpecialMove(owner, destination, source, 0);
    owner->pad_gamepad->buttons_held |= GAMEPAD_JUMP;
    if (owner->id != id_GEONOSIAN || owner->field_0xe31 == 3 || owner->field_0xe31 == 0) {
        owner->pad_gamepad->buttons_pressed |= GAMEPAD_JUMP;
    }
    packet->movement_destination = packet->special_move_node->position;
    packet->movement_stopping_distance = 0.0f;
    object->collision_priority |= 0x2000;
    return 1;
}

u32 StarWars_ParseAIPathCnxFlag(char *name) {
    if (NuStrICmp(name, "DBLJUMP") == 0) {
        return 0x00000002;
    }
    if (NuStrICmp(name, "R2D2GLIDE") == 0) {
        return 0x00000004;
    }
    if (NuStrICmp(name, "ZIPUP") == 0) {
        return 0x00000008;
    }
    if (NuStrICmp(name, "USEHATCH") == 0) {
        return 0x00000010;
    }
    if (NuStrICmp(name, "JARJARJUMP") == 0) {
        return 0x00000020;
    }
    if (NuStrICmp(name, "HOVERTUBE") == 0) {
        return 0x00000040;
    }
    if (NuStrICmp(name, "SWAMP") == 0) {
        return 0x00004000;
    }
    if (NuStrICmp(name, "TAKEOVER") == 0) {
        return 0x00008000;
    }
    if (NuStrICmp(name, "VEHICLE") == 0) {
        return 0x00010000;
    }
    if (NuStrICmp(name, "PARTY") == 0) {
        return 0x00020000;
    }
    if (NuStrICmp(name, "FORGOODIES") == 0) {
        return 0x00000080;
    }
    if (NuStrICmp(name, "FORBADDIES") == 0) {
        return 0x00000100;
    }
    if (NuStrICmp(name, "OBSTACLE") == 0) {
        return 0x20000000;
    }
    if (NuStrICmp(name, "JUMP_NOW") == 0) {
        return 0x00000400;
    }
    if (NuStrICmp(name, "DONT_JUMP_NOW") == 0) {
        return 0x00000800;
    }
    if (NuStrICmp(name, "BLOCKAGE") == 0) {
        return 0x00040000;
    }
    if (NuStrICmp(name, "JUMP") == 0) {
        return 0x00000001;
    }
    if (NuStrICmp(name, "DONTTOGGLE") == 0) {
        return 0x00080000;
    }
    if (NuStrICmp(name, "FULLTERRAIN") == 0) {
        return 0x00100000;
    }
    if (NuStrICmp(name, "AUTOJUMP") == 0) {
        return 0x00200000;
    }
    if (NuStrICmp(name, "AUTODBLJUMP") == 0) {
        return 0x00400000;
    }
    if (NuStrICmp(name, "AUTOHIGHJUMP") == 0) {
        return 0x00800000;
    }
    return 0;
}

void StarWars_AutoSetAICapabilities(GameObject_s *object) {
    CHARACTERDATA *character = object->apiobj.character_data;
    object->ai.capabilities = character->ai_path_capabilities | STARWARS_AI_CAPABILITY_DEFAULT;
    if ((object->apiobj.field_0x1f4 & 1) != 0) {
        object->ai.capabilities =
            character->ai_path_capabilities | STARWARS_AI_CAPABILITY_DEFAULT | STARWARS_AI_CAPABILITY_OBJECT_STATE_SET;
    } else if ((object->apiobj.field_0x1f4 & 4) == 0) {
        object->ai.capabilities = character->ai_path_capabilities | STARWARS_AI_CAPABILITY_DEFAULT |
                                  STARWARS_AI_CAPABILITY_OBJECT_STATE_CLEAR;
    }

    if (object->apiobj.character_model->model_data_b[6] != NULL) {
        object->ai.capabilities |= STARWARS_AI_CAPABILITY_MODEL_ATTACHMENT;
    }

    const u32 model_flags = character->model_flags;
    if ((model_flags & 0x00200008) == 0x8) {
        object->ai.capabilities |= STARWARS_AI_CAPABILITY_MODEL_FLAG_8_EXCLUSIVE;
    }
    if ((model_flags & 0x8) != 0) {
        object->ai.capabilities |= STARWARS_AI_CAPABILITY_MODEL_FLAG_8;
    }

    const u32 game_flags = static_cast<GAMECHARACTERDATA *>(character->field11_0x24)->flags_090;
    if ((game_flags & 0x00400000) != 0) {
        object->ai.capabilities |= STARWARS_AI_CAPABILITY_EXTENDED_JUMP;
    }
    if ((model_flags & 0x00040000) != 0) {
        object->ai.capabilities |= STARWARS_AI_CAPABILITY_MODEL_FLAG_40000;
    }
    if ((model_flags & 0x40) != 0) {
        object->ai.capabilities |= STARWARS_AI_CAPABILITY_MODEL_FLAG_40;
    }
    if ((model_flags & 0x00100000) != 0) {
        object->ai.capabilities |= STARWARS_AI_CAPABILITY_MODEL_FLAG_100000;
    }
    if ((model_flags & 0x2000) != 0) {
        object->ai.capabilities |= STARWARS_AI_CAPABILITY_MODEL_FLAG_2000;
    }
    if ((model_flags & 0x88) != 0 || (game_flags & 0x40) != 0) {
        object->ai.capabilities |= STARWARS_AI_CAPABILITY_SPECIAL_MOVEMENT;
    }
    if (object->apiobj.field_0x27c != -1) {
        object->ai.capabilities |= STARWARS_AI_CAPABILITY_PLAYER_SLOT;
    }
    if (object->id == id_SNAKE) {
        object->ai.capabilities |= STARWARS_AI_CAPABILITY_SNAKE;
    }
}
