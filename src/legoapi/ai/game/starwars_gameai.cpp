#include "decomp.h"
#include "globals.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/world/world.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nustring.h"

struct AIROW_s;
struct AISYS_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

typedef void (*PREPARINGSPECIALMOVEFN)(AIPACKET_s *, APIOBJECT_s *, i32);
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

static void StarWars_PreparingForSpecialMove(AIPACKET_s *, APIOBJECT_s *, i32) {
}

extern void ReleaseTakeOver(GameObject_s *, i32);
extern void SetSpecialMove(GameObject_s *, AIPATHNODE_s *, AIPATHNODE_s *, char);
extern i32 TryToTeleportToNextNode(GameObject_s *, AIPATHNODE_s *, i32);

static __used__ i32 StarWars_PrepareBigJump(AIPACKET_s *packet, APIOBJECT_s *apiobject, i32) {
    GameObject_s *object = apiobject->objptr;
    AIPATHNODE_s *nodes = packet->path_info.path->nodes;
    AIPATHCNX_s *connection = packet->path_info.connection;
    u8 from = connection->node_indices[packet->path_info.direction];
    u8 to = connection->node_indices[packet->path_info.direction == 0];
    if (object->ai.inside_path_node == to || object->character_context == 0x2b) {
        return 0;
    }
    if ((connection->traversal_flags[packet->path_info.direction] & 0x800) != 0) {
        packet->movement_destination = nodes[from].position;
        packet->runtime_flags |= 0x80;
        packet->movement_stopping_distance = 0.0f;
        apiobject->movement_request_flags |= 0x2000;
        return 1;
    }
    AIPATHNODE_s *target = &nodes[to];
    if (TryToTeleportToNextNode(object, target, 0) != 0) {
        return 1;
    }
    if (WORLD->current_level == TEMPLEB_LDATA && target->name != NULL && NuStrICmp(target->name, "stairs_top") == 0) {
        AIPATHNODE_s *alternate = AIPathFindNode(WORLD->ai_sys, NULL, "stairs_top2");
        if (alternate != NULL) {
            target = alternate;
        }
    }
    StartBigJump(object, &target->position, 0, 0.5f, 1.0f, 0, 0);
    SetSpecialMove(object, target, NULL, 3);
    return 0;
}

static __used__ i32 StarWars_PrepareTakeOverJump(AIPACKET_s *packet, APIOBJECT_s *apiobject, i32) {
    GameObject_s *object = apiobject->objptr;
    i16 result = 0;
    if ((static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_090 & 0x40) != 0) {
        GameObject_s *linked_object = object->field_0xcc0;
        result = 1;
        if (linked_object != NULL) {
            AIPATHNODE_s *node =
                &packet->path_info.path
                     ->nodes[packet->path_info.connection->node_indices[packet->path_info.direction == 0]];
            ReleaseTakeOver(linked_object, 1);
            StartBigJump(linked_object, &node->position, 0, 0.5f, 1.0f, 0, 0);
            SetSpecialMove(linked_object, node, NULL, 3);
        }
    }
    return result;
}

static i32 StarWars_MidSpecialMove(AISYS_s *, AIPACKET_s *, APIOBJECT_s *) {
    return 0;
}

void StarWars_PrepareJump(AIPACKET_s *, APIOBJECT_s *, i32) {
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

void StarWars_PrepareR2D2Glide(AIPACKET_s *, APIOBJECT_s *, i32) {
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
