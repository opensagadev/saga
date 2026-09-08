#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/render/fx.h"
#include "legoapi/gizmos/object/lever.h"
#include "legoapi/gizmos/object/technos.h"
#include "legoapi/world/level.h"
#include "legoapi/world/areas.h"
#include "legoapi/world/world.h"
#include "gameapi/edtools/edstubs.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuspecial.h"

#include <string.h>
#include <stdio.h>

// LoadPerm1 is one of the few game-level entry points which wires together
// otherwise C-linkage engine subsystems.  Keep these declarations local: the
// individual subsystem TUs intentionally expose their original plain names.
extern "C" {
    i32 rtlInitDynamic(VARIPTR *, VARIPTR, i32);
    void DebrisSetup(VARIPTR *, VARIPTR, char *, i32, i32, i32);
    void DebrisRegisterCutoffCameraVec(void *);
    void edgraSetup(VARIPTR *, VARIPTR, i32, i32, i32);
    void InitParts(i32, VARIPTR *, VARIPTR);
    void ParticleReset(void);
    i32 NuFileExists(char *);
    i32 NuStrCpy(char *, const char *);
    i32 edppLoadPage(char *, i32, usize);
    void NuRndrShadowInit(u8 *);
    void NuTexAnimProgSysInit(void);
    void terrainpickupinit(char *, void **);

    extern i32 Grass_Available;
    extern i32 DEBPAGE_GENERAL;
    extern i32 DEBPAGE_CHARACTER;
    extern void *perm_debrissys;
}

struct AISYS_s;
void AIScriptLoadAll(char *path, VARIPTR *buf, VARIPTR *buf_end, AISYS_s *sys);
void InitTexAnimScripts(char **names);
void BackDrop_Init(char *path, VARIPTR *buf, VARIPTR *buf_end);

i32 PARTPAGE_GENERAL = -1;

// The original table at 0x006281a0.  These are the material animation
// scripts made available before the permanent things scene is loaded.
char *TexAnimList_LSW[32] = {
    (char *)"arrow",          (char *)"blink_01",      (char *)"blink_02",    (char *)"blink_03",
    (char *)"coin",           (char *)"coin_pause",    (char *)"control",     (char *)"ds_esc_intro_1",
    (char *)"ds_esc_intro_2", (char *)"ds_1",          (char *)"helpmeobi",   (char *)"lightening1",
    (char *)"plasma",         (char *)"screen",        (char *)"Dso_screen",  (char *)"anakinbows",
    (char *)"anakinkilling",  (char *)"anakinspod",    (char *)"boost1",      (char *)"boost2",
    (char *)"boost3",         (char *)"boost4",        (char *)"gasganospod", (char *)"helpobi",
    (char *)"hologram1",      (char *)"hologramnoise", (char *)"play8",       (char *)"quidie",
    (char *)"sebulbaspod",    (char *)"sidious",       (char *)"crowd",       NULL,
};

void InitStreaks(VARIPTR *, VARIPTR, char *);
void InitRopeMtl(char *, VARIPTR *, VARIPTR *);
void InitRipples(ripple_set_s **, VARIPTR *, VARIPTR *, i32);
void CreateFadeMaterials();
void CreateUsefulMaterials();
extern ripple_set_s *ripples;
extern NUMTL *ShadowMat;

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void CharConfig_ConfigureAll(i32 permanent, nufpcomjmp_s *game_keywords);
void ExtraCharacterFixUpAfterConfig();
extern i32 CHARPAK;
extern i32 apiloadcharactermodels_nopakfile;
extern "C" i32 apiloadcharactermodels_append;

extern i32 GetMenuID(void);
extern i32 InCollectList_Index(i32 id, COLLECTID *list, i32 count);
extern i32 Collection_Got(i32 id);
extern void IconScenes_Load(APICHARACTERMODELLIST_s *list, i32 permanent, VARIPTR *buf, VARIPTR *buf_end);
extern NUGSCN *IconScene_FindById(i32 character_id);
extern void Customiser_SaveModelTextureIDs(CUSTOMISER *customiser, CHARACTERMODEL_s *model);
extern CUSTOMISER *CharacterCustomiser;
extern VARIPTR characterbuffer_ptr;
extern VARIPTR characterbuffer_end;
extern i32 waiting_for_character;
extern f32 WaitingForCharacterTime;

extern "C" {
    i32 hub_character_ready = -1;
}

APICHARACTERMODELLIST_s PermModelList[] = {{-1, 0}};

enum LegoObjectId : i16 {
    LEGO_OBJECT_FLOOR_TARGET = 0x55,
};

void LSW_SetIndy(i32) {
    LedgeTerrain_On = 0;
    Grapples_Available = 0;
    SuperCarry_Bash = 0;
    SuperCarry_Jump = 0;
    PUNCHGAP = 0.3f;
    PUNCHCHARGAP = 0.3f;
    HINTS_ON = 1;

    TechnoSys.interaction_time = 0.2f;
    TechnoSys.idle_offset = v000;
    TechnoSys.active_offset = v000;
    TechnoSys.active_effect_id = 0x2c;
    TechnoSys.success_effect_id = -1;
    TechnoSys.failure_effect_id = -1;
    TechnoSys.complete_offset = v000;
    TechnoSys.floor_target_object_id = LEGO_OBJECT_FLOOR_TARGET;
    TechnoSys.activation_effect_id = 0x2d;
    TechnoSys.completion_effect_id = 0x2e;

    LeverSys.floor_target_object_id = LEGO_OBJECT_FLOOR_TARGET;
    GameAudio_SetActionMusicTimes(1.0f, 6.0f);
}

void HairMovement(GameObject_s *) {
}

void HeadMovement(GameObject_s *) {
}

void TakeOverYoda(GameObject_s *, GameObject_s *, i32, i32) {
}

void fullcodename(i32) {
}

nuhspecial_s *CharScene_FindHSpecial(WORLDINFO_s *world, i32 character_id);

void CharScene_Draw(WORLDINFO_s *world, i32 character_id, numtx_s *matrix, numtx_s *reflection_matrix) {
    nuhspecial_s *special = CharScene_FindHSpecial(world, character_id);
    if (special != NULL) {
        if (matrix != NULL) {
            NuSpecialDrawAt(special, matrix);
        }
        if (reflection_matrix != NULL) {
            NuSpecialDrawAt(special, reflection_matrix);
        }
    }
}

void CharScenes_Init(variptr_u *buf, variptr_u *) {
    buf->addr = ALIGN(buf->addr, 4);
    CharScene_Area = reinterpret_cast<CHARSCENE_s *>(buf->void_ptr);
    buf->addr += static_cast<usize>(CHARCOUNT) * sizeof(*CharScene_Area);
    memset(CharScene_Area, 0, static_cast<usize>(CHARCOUNT) * sizeof(*CharScene_Area));
}

extern i16 tUNKNOWN;
void Move_CHARACTER(GameObject_s *);
void Animate_CHARACTER(GameObject_s *);

void FixUpCharacters(CHARFIXUP *fixup) {
    for (i32 i = 0; i < CHARCOUNT; ++i) {
        GCDataList[i] = GCDATA_DEFAULT;

        CHARACTERDATA &character = CDataList[i];
        if (character.field0_0x0 == -1) {
            character.field0_0x0 = tUNKNOWN;
        }
        if (character.move_fn == NULL) {
            character.move_fn = Move_CHARACTER;
        }
        if (character.animate_fn == NULL) {
            character.animate_fn = Animate_CHARACTER;
        }
    }

    if (fixup != NULL) {
        while (fixup->name != NULL) {
            if (fixup->id != NULL) {
                *fixup->id = (i16)CharIDFromName(fixup->name);
            }
            ++fixup;
        }
    }
}

void PostAnimate_FETT(GameObject_s *) {
}

void ActivateCharacter(char *, nuvec_s *, i32) {
}

void FinishWeirdoNames(i32) {
}


#include "legoapi/core/input/gamepads.h"
#include "legoapi/render/core/rtl.h"
#include "nu2api/numath/nuvec.h"
extern i32 addcreature_override_id_check;
extern f32 default_mover_extra;
extern void SetGameObjectCharacterData(GameObject_s *);
extern void GetTopBot(GameObject_s *);
extern void GameObjectDimensions(GameObject_s *);
extern void GameObjectOrigin(GameObject_s *);
extern i32 GetDefaultIdle(GameObject_s *);
extern void ResetCharacterIdle(GameObject_s *, i32, i32);
extern void *Suit_GetDefault(i32);
extern void ResetLights(NUVEC *, rtldata_s *, void *);
extern "C" void ResetAnimPacket(void *, i32);
extern void ResetPlayerPacket(PLAYERPACKET_s *, CHARACTERDATA_s *);
extern void Player_ClearContext(GameObject_s *, i32);
extern "C" {
extern i16 id_MOSEISLEYCITIZEN, id_CANTINAALIEN, id_CLOUDCITYCITIZEN, id_GEONOSIAN, id_BOB;
}
void Shards_HandleLostObj(WORLDINFO_s *, GameObject_s *);
void LoseHelmet(GameObject_s *, i32, i32);
void DestroySnakeBody(GameObject_s *);
void InitPlayerAI(GameObject_s *);
extern "C" void SetAnimTimeRandom(CHARACTERMODEL_s *, ANIMPACKET_s *);
void SetFlicker(GameObject_s *object, f32 duration) {
    object->flicker_timer = duration;
    object->flicker_flags &= ~7u;
}
void ResetCoinPacket(COINPACKET_s *);

void ResetPlayerMoves(GameObject_s *object) {
    ResetPlayerPacket(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet),
                      reinterpret_cast<CHARACTERDATA_s *>(object->apiobj.character_data));
    object->fall_acceleration_timer = 0.0f;
    object->tag_state = 0.0f;
    object->apiobj.movement_direction = v000;
    object->input_toggle_hold_time = TOGGLEHOLDTIME;
    object->field_0xefc |= 0x80;
    ResetCharacterIdle(object, 2, GetDefaultIdle(object));
    if (object->apiobj.character_model->model_data_b[1] != NULL) {
        ResetAnimPacket(&object->apiobj.anim_packet, 1);
        SetAnimTimeRandom(object->apiobj.character_model, &object->apiobj.anim_packet);
    } else {
        ResetAnimPacket(&object->apiobj.anim_packet, -1);
    }
    object->apiobj.field_0x27e = 0;
    object->apiobj.field_0x27d = 0;
    SetGameObjectCharacterData(object);
    object->spawn_protection_timer = 0.0f;
    SetFlicker(object, 0.0f);
    ResetCoinPacket(object->coinpacket);
    object->apiobj.respawn_timer = 0.0f;
    object->apiobj.flags_high &= ~0x20u;
    DrawOffsetCode(object, 1);
}
void Player_ResetContexts(PLAYERPACKET_s *);
void InitSurfaceInfo(GameObject_s *);
i32 SetObjOnSurface(GameObject_s *, i32);
void GizForce_ResetLOS(GameObject_s *);
i32 StartSlide(GameObject_s *, i32);

static u32 LayerBit(u8 layer) {
    return 1u << (layer & 31);
}

static void SetLayers_BOB(GameObject_s *object) {
    const i32 first_layers[6] = {1, 2, 3, 21, 22, 23};
    const i32 second_layers[8] = {5, 6, 7, 8, 9, 10, 16, 17};
    const i32 third_layers[3] = {11, 12, 15};
    const i32 fourth_layers[2] = {13, 14};
    const i32 fifth_layers[2] = {19, 20};

    const i32 random = qrand();
    object->field_0xefd = (object->field_0xefd & ~2u) | (random <= 0x7fff ? 2 : 0);
    // The original consumes this draw even though layer zero is always selected.
    qrand();
    object->field_0x1054 = 1;
    object->field_0x1054 |= 1u << first_layers[qrand() / 0x2aab];
    object->field_0x1054 |= 1u << second_layers[qrand() / 0x2000];
    object->field_0x1054 |= 1u << third_layers[qrand() / 0x5556];
    object->field_0x1054 |= 1u << fourth_layers[qrand() / 0x8000];
    object->field_0x1054 |= 1u << fifth_layers[qrand() / 0x8000];
}

static void SetLayers_MOSEISLEYCITIZEN(u32 *layers) {
    static const u8 hat_layers[5] = {0, 0, 0, 7, 14};
    static const u8 head_layers[5] = {20, 20, 8, 8, 4};
    static const u8 body_layers[3] = {3, 9, 19};
    static const u8 arm_layers[3] = {1, 12, 15};
    static const u8 hand_layers[3] = {2, 13, 16};
    static const u8 waist_layers[3] = {6, 10, 17};
    static const u8 leg_layers[3] = {5, 11, 18};

    *layers = LayerBit(hat_layers[qrand() / 0x3334]) | LayerBit(head_layers[qrand() / 0x3334]) |
              LayerBit(body_layers[qrand() / 0x5556]) | LayerBit(arm_layers[qrand() / 0x5556]) |
              LayerBit(hand_layers[qrand() / 0x5556]) | LayerBit(waist_layers[qrand() / 0x5556]) |
              LayerBit(leg_layers[qrand() / 0x5556]);
}

i32 InitCreature(GameObject_s *obj, i32 id, i32 param) {
    addcreature_override_id_check = 0;
    if (id < 0 || id > 0x153 || apicharsys->playermodelids[id] == -1) {
        return 0;
    }

    CHARACTERDATA *character_data = &apicharsys->char_data[id];
    GAMECHARACTERDATA *game_character_data = static_cast<GAMECHARACTERDATA *>(character_data->field11_0x24);
    obj->apiobj.character_data = character_data;
    obj->field_0x1054 = game_character_data->layer_mask;
    obj->apiobj.field_0x1f4 = 0;
    obj->field_0x107c = -1;
    memset(obj->player_packet, 0, 0x798);
    obj->apiobj.field_0x27c = -1;

    NUVEC *start_position = Player_StartPos(obj);
    if (param == 0) {
        obj->apiobj.field_0x1f4 |= 2;
    } else {
        obj->apiobj.field_0x1f4 |= 0x4002;
    }
    if ((WORLD->current_level->flags & 0x40000) == 0) {
        obj->apiobj.field_0x1f4 |= 0x40;
    }

    obj->pad_gamepad = GamePad_Allocate();
    obj->pad_gamepad->unknown_24 |= 0x100;
    obj->hitpoints = game_character_data->hitpoints;
    obj->current_hp = game_character_data->hitpoints;
    ResetPlayerPacket(reinterpret_cast<PLAYERPACKET_s *>(obj->player_packet),
                      reinterpret_cast<CHARACTERDATA_s *>(character_data));

    obj->apiobj.field_0x1fc = v000.x;
    obj->apiobj.field_0x200 = v000.y;
    obj->apiobj.field_0x204 = v000.z;
    obj->field_0xe38 = 4;
    obj->field_0xe37 = game_character_data->field_0xf5;
    obj->id = static_cast<i16>(id);
    obj->apiobj.character_model = &apicharsys->models[apicharsys->playermodelids[id]];
    obj->suit = Suit_GetDefault(id);
    obj->ai.field_0x134 = 0xff;
    SetGameObjectCharacterData(obj);

    obj->apiobj.start_position = *start_position;
    obj->apiobj.initial_position = *start_position;
    obj->apiobj.position = *start_position;
    obj->apiobj.pos_x = start_position->x;
    obj->apiobj.pos_y = start_position->y;
    obj->apiobj.pos_z = start_position->z;
    obj->field_0x1018 = 0.5f;

    GetTopBot(obj);
    GameObjectDimensions(obj);
    obj->apiobj.anim_packet.animation_index = 1;

    i32 reset_animation = 1;
    if (obj->apiobj.character_model != NULL) {
        void **animation_table = *reinterpret_cast<void ***>(reinterpret_cast<u8 *>(obj->apiobj.character_model) + 0xc);
        if (animation_table != NULL && animation_table[1] == NULL) {
            reset_animation = 0;
            for (i32 i = 0; i < 0xe9; i++) {
                if (animation_table[i] != NULL) {
                    obj->apiobj.anim_packet.animation_index = static_cast<u16>(i);
                    reset_animation = i;
                    break;
                }
            }
        }
    }
    ResetAnimPacket(&obj->apiobj.anim_packet, reset_animation);
    ResetCharacterIdle(obj, 2, GetDefaultIdle(obj));
    ResetLights(&obj->apiobj.position, &obj->light_data, WORLD->rtl_set);

    obj->ai.mover_height = obj->apiobj.field_0x1dc + default_mover_extra;
    GameObjectOrigin(obj);
    obj->apiobj.previous_position[0] = obj->apiobj.position.x;
    obj->apiobj.previous_position[1] = obj->apiobj.position.y;
    obj->apiobj.previous_position[2] = obj->apiobj.position.z;
    obj->field_0x10c8 = obj->apiobj.position.x;
    obj->field_0x10cc = obj->apiobj.position.y;
    obj->field_0x10d0 = obj->apiobj.position.z;

    obj->field_0xf01 = static_cast<u8>((obj->field_0xf01 & ~8u) | (((game_character_data->flags_090 >> 17) & 1) << 3));
    if (id == id_MOSEISLEYCITIZEN) {
        SetLayers_MOSEISLEYCITIZEN(&obj->field_0x1054);
    } else if (id == id_CANTINAALIEN) {
        static const u8 head_layers[3] = {0, 3, 6};
        static const u8 body_layers[3] = {1, 4, 7};
        static const u8 leg_layers[3] = {2, 5, 8};
        obj->field_0x1054 = LayerBit(head_layers[qrand() / 0x5556]) | LayerBit(body_layers[qrand() / 0x5556]) |
                            LayerBit(leg_layers[qrand() / 0x5556]);
    } else if (id == id_CLOUDCITYCITIZEN) {
        static const u8 head_layers[3] = {1, 5, 7};
        static const u8 body_layers[3] = {2, 3, 6};
        static const u8 leg_layers[3] = {0, 4, 8};
        obj->field_0x1054 = LayerBit(head_layers[qrand() / 0x5556]) | LayerBit(body_layers[qrand() / 0x5556]) |
                            LayerBit(leg_layers[qrand() / 0x5556]);
        obj->field_0xf01 = static_cast<u8>((obj->field_0xf01 & ~8u) | (((obj->field_0x1054 >> 7) & 1) << 3));
    } else if (id == id_BOB) {
        SetLayers_BOB(obj);
    } else if (id == id_GEONOSIAN) {
        obj->field_0xefd = static_cast<u8>((obj->field_0xefd & ~2u) | ((qrand() <= 0x7fff) ? 2 : 0));
    }

    if ((game_character_data->flags_090 & 0x8000u) != 0) {
        obj->apiobj.field_0x1f4 |= 0x20000;
    } else {
        obj->apiobj.field_0x1f4 &= ~0x20000u;
    }
    return 1;
}

i32 NewPlayerCharacter(GameObject_s *object, i32 new_id, i32 old_id, i32) {
    if (new_id == old_id || new_id < 0 || new_id >= CHARCOUNT || apicharsys->playermodelids[new_id] == -1)
        return 0;
    Shards_HandleLostObj(WORLD, object);
    if (VehicleArea == 0)
        AddGameDebris(WORLD->debris_sys, 0x5c, &object->apiobj.collision_position);
    const u8 context = object->field_0x7a5;
    const i16 lean = object->movement_lean_angle;
    const u32 old_model_flag = object->apiobj.character_data->model_flags & 0x2000;
    LoseHelmet(object, 0, 1);
    DestroySnakeBody(object);
    object->id = new_id;
    const NUVEC velocity = object->apiobj.velocity;
    const u8 current_hp = object->current_hp;
    const u8 hitpoints = object->hitpoints;
    const u32 saved_object_flag = object->apiobj.object_flags & 0x2000;
    object->apiobj.character_model = &apicharsys->models[apicharsys->playermodelids[new_id]];
    object->apiobj.character_data = &apicharsys->char_data[new_id];
    object->takeover_source = NULL;
    object->suit = Suit_GetDefault(new_id);
    const u32 saved_f14 = object->field_0xf14;
    const u32 saved_f20 = object->field_0xf20;
    const u8 current_route = object->ai.current_route;
    const u8 next_route = object->ai.next_route;
    const u8 saved_efe = object->field_0xefe & 0x20;
    const AIPATHINFO path_info = object->ai.path_info;
    const u32 frame_state = object->ai.frame_state;
    InitPlayerAI(object);
    object->field_0xefe = (object->field_0xefe & ~0x20u) | saved_efe;
    object->field_0xf20 = saved_f20;
    object->ai.next_route = next_route;
    object->ai.path_info = path_info;
    object->field_0xf14 = saved_f14;
    object->field_0x1004 = 1.0f;
    object->ai.current_route = current_route;
    object->ai.frame_state = frame_state;
    object->field_0x1054 = object->apiobj.character_data->game_character->layer_mask;
    object->field_0xe37 = object->apiobj.character_data->game_character->field_0xf5;
    object->field_0xe38 = 4;
    Player_ClearContext(object, 1);
    ResetPlayerMoves(object);
    SetGameObjectCharacterData(object);
    ResetAnimPacket(&object->apiobj.anim_packet, -1);
    Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    GetTopBot(object);
    GameObjectDimensions(object);
    object->tag_state = 2.0f;
    object->field_0xefc |= 0x80;
    object->input_toggle_hold_time = TOGGLEHOLDTIME;
    InitSurfaceInfo(object);
    const i32 on_surface = SetObjOnSurface(object, 1);
    GizForce_ResetLOS(object);
    object->field_0xefe &= ~8u;
    object->current_hp = current_hp;
    object->hitpoints = hitpoints;
    object->field_0xef0 = 0;
    object->pause_context_state = 0;
    object->apiobj.object_flags = (object->apiobj.object_flags & ~0x2000u) | saved_object_flag;
    ResetMiniAnimPacket(&object->mini_animation, -1);
    object->weapon_scale = (object->field_0xe22 & 1) ? 1.0f : 0.0f;
    object->field_0xe32 = 0;
    if (on_surface)
        object->ground_contact_grace_timer = 0.2f;
    else
        object->apiobj.velocity.y = velocity.y;
    if (context == 0x33 && StartSlide(object, 0)) {
        object->apiobj.velocity.x = velocity.x;
        object->apiobj.velocity.z = velocity.z;
    }
    if (object->apiobj.velocity.y == 0.0f) object->apiobj.velocity.y = -0.1f;
    if (old_model_flag && (object->apiobj.character_data->model_flags & 0x2000))
        object->movement_lean_angle = lean;
    GAMECHARACTERDATA *data = apicharsys->char_data[object->id].game_character;
    object->apiobj.viewdistance = data->viewdistance;
    object->apiobj.heardistance = data->heardistance;
    object->apiobj.maxviewheight = data->maxviewheight;
    object->apiobj.minviewheight = data->minviewheight;
    object->field_0xf01 = (object->field_0xf01 & ~8u) |
                        (((object->apiobj.character_data->game_character->flags_090 >> 17) & 1) << 3);
    if (object->id == id_MOSEISLEYCITIZEN) {
        SetLayers_MOSEISLEYCITIZEN(&object->field_0x1054);
    } else if (object->id == id_CANTINAALIEN) {
        static const u8 head[3] = {0, 3, 6}, body[3] = {1, 4, 7}, legs[3] = {2, 5, 8};
        object->field_0x1054 = 0;
        object->field_0x1054 |= 1u << head[qrand() / 0x5556];
        object->field_0x1054 |= 1u << body[qrand() / 0x5556];
        object->field_0x1054 |= 1u << legs[qrand() / 0x5556];
    } else if (object->id == id_CLOUDCITYCITIZEN) {
        static const u8 head[3] = {1, 5, 7}, body[3] = {2, 3, 6}, legs[3] = {0, 4, 8};
        object->field_0x1054 = 0;
        object->field_0x1054 |= 1u << head[qrand() / 0x5556];
        object->field_0x1054 |= 1u << body[qrand() / 0x5556];
        object->field_0x1054 |= 1u << legs[qrand() / 0x5556];
        object->field_0xf01 = (object->field_0xf01 & ~8u) | (((object->field_0x1054 >> 7) & 1) << 3);
    } else if (object->id == id_BOB) {
        SetLayers_BOB(object);
    } else if (object->id == id_GEONOSIAN) {
        object->field_0xefd = (object->field_0xefd & ~2u) | (qrand() <= 0x7fff ? 2 : 0);
    }
    if (object->apiobj.character_data->game_character->flags_090 & 0x8000)
        object->apiobj.field_0x1f4 |= 0x20000;
    else
        object->apiobj.field_0x1f4 &= ~0x20000u;
    return 1;
}

extern "C" f32 AnimDuration(i32 character_id, i32 animation, f32 start_frame, f32 end_frame, i32 subtract_frame_time);
i32 GetDefaultIdle(GameObject_s *object);

static i32 IdleRepetitionCount(u8 minimum, u8 maximum) {
    if (maximum <= minimum) {
        return minimum;
    }

    const i32 bucket_size = 0xffff / (maximum - minimum) + 1;
    return minimum + qrand() / bucket_size;
}

void ResetCharacterIdle(GameObject_s *object, i32 mode, i32 animation) {
    // Animation 151 belongs to the held-batarang override and deliberately
    // leaves the ordinary idle scheduler untouched.
    if (animation == 151) {
        return;
    }

    object->idle_animation = static_cast<i16>(animation);
    object->idle_animation_time = 0.0f;

    CHARACTERMODEL_s *model = object->apiobj.character_model;
    CHARACTERANIM_s *animation_info = NULL;
    if (model != NULL && model->model_data_b != NULL && model->model_data_b[animation] != NULL &&
        model->model_data_a != NULL) {
        animation_info = static_cast<CHARACTERANIM_s *>(model->model_data_a[animation]);
    }

    const u8 minimum = animation_info != NULL ? static_cast<u8>(animation_info->minimum_repetitions) : 0;
    if (minimum == 0) {
        object->idle_animation_limit = static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 7.0f + 8.0f;
    } else {
        const u8 maximum = static_cast<u8>(animation_info->maximum_repetitions);
        const i32 repetitions = IdleRepetitionCount(minimum, maximum);
        object->idle_animation_limit = AnimDuration(object->id, animation, 0.0f, 0.0f, 0) * repetitions - FRAMETIME;
    }

    if (mode > 0) {
        object->idle_total_time = 0.0f;
        if (mode != 1) {
            object->previous_idle_animation = -1;
        }
    }
}

static __attribute__((used, noinline)) void NewCharacterIdle(GameObject_s *object, i32 default_idle) {
    CHARACTERDATA *character = object->apiobj.character_data;
    GAMECHARACTERDATA *game_character = static_cast<GAMECHARACTERDATA *>(character->field11_0x24);
    const i32 alternate_idle = game_character->field275_0x116 == 0 && (character->model_flags & 0x80) != 0 ? 118 : 25;

    i32 candidates[233];
    i32 candidate_count = 0;
    CHARACTERMODEL_s *model = object->apiobj.character_model;
    const bool select_alternate_group = alternate_idle == default_idle;
    for (i32 animation = 0; animation < 233; ++animation) {
        if (model->model_data_b[animation] == NULL) {
            continue;
        }
        CHARACTERANIM_s *info = static_cast<CHARACTERANIM_s *>(model->model_data_a[animation]);
        if ((info->flags & 0x10) == 0) {
            continue;
        }
        const bool alternate_group = (info->flags & 0x800) != 0;
        if (alternate_group == select_alternate_group) {
            candidates[candidate_count++] = animation;
        }
    }

    if (candidate_count == 0) {
        ResetCharacterIdle(object, 0, default_idle);
        return;
    }

    i32 animation;
    if (candidate_count == 1) {
        animation = candidates[0];
    } else {
        do {
            const i32 bucket_size = 0xffff / candidate_count + 1;
            animation = candidates[qrand() / bucket_size];
            object->idle_animation = static_cast<i16>(animation);
        } while (animation == object->previous_idle_animation);
    }
    object->idle_animation = static_cast<i16>(animation);

    CHARACTERANIM_s *info = static_cast<CHARACTERANIM_s *>(model->model_data_a[animation]);
    i32 repetitions = static_cast<u8>(info->minimum_repetitions);
    const u8 maximum = static_cast<u8>(info->maximum_repetitions);
    object->previous_idle_animation = static_cast<i16>(animation);
    if (repetitions > 1 && (info->flags & 2) == 0) {
        repetitions = 1;
    }
    if (repetitions == 0) {
        repetitions = 1;
    } else if (maximum > repetitions) {
        repetitions = IdleRepetitionCount(static_cast<u8>(repetitions), maximum);
    }

    object->idle_animation_time = 0.0f;
    object->idle_animation_limit = AnimDuration(object->id, animation, 0.0f, 0.0f, 0) * repetitions - FRAMETIME;
}

void UpdateCharacterIdle(GameObject_s *object) {
    if (object->apiobj.character_model == NULL) {
        return;
    }

    CHARACTERDATA *character = object->apiobj.character_data;
    GAMECHARACTERDATA *game_character = static_cast<GAMECHARACTERDATA *>(character->field11_0x24);
    const i16 alternate_idle =
        static_cast<i16>(game_character->field275_0x116 == 0 && (character->model_flags & 0x80) != 0 ? 118 : 25);
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    const i16 requested = packet.requested_animation;

    if (requested != 1 && requested != alternate_idle) {
        const i32 default_idle = GetDefaultIdle(object);
        if (default_idle != -1) {
            ResetCharacterIdle(object, 1, default_idle);
            return;
        }
    }

    if (requested == 1) {
        if (packet.previous_animation == alternate_idle) {
            ResetCharacterIdle(object, 1, 1);
        } else {
            object->idle_total_time += FRAMETIME;
            object->idle_animation_time += FRAMETIME;
            if (object->idle_animation_time >= object->idle_animation_limit) {
                if (object->idle_animation == 1) {
                    NewCharacterIdle(object, 1);
                } else {
                    ResetCharacterIdle(object, 1, 1);
                }
            }
        }
        packet.requested_animation = object->idle_animation;
        return;
    }

    if (requested == alternate_idle) {
        if (packet.previous_animation == 1) {
            ResetCharacterIdle(object, 1, alternate_idle);
        } else {
            object->idle_total_time += FRAMETIME;
            object->idle_animation_time += FRAMETIME;
            if (object->idle_animation_time >= object->idle_animation_limit) {
                if (object->idle_animation == alternate_idle) {
                    NewCharacterIdle(object, alternate_idle);
                } else {
                    ResetCharacterIdle(object, 1, alternate_idle);
                }
            }
        }
        packet.requested_animation = object->idle_animation;
        return;
    }

    const i32 default_idle = GetDefaultIdle(object);
    if (default_idle != -1) {
        ResetCharacterIdle(object, 1, default_idle);
    }
}

void UpdateCharacterIDs() {
    shopitem_s *item = CharItems;
    COLLECTID *collect = ShopCollection.list;
    for (i32 i = 0; i < ShopCollection.count_y; ++i, ++item, ++collect) {
        item->item_id = collect->id;
        item->price = collect->field3_0x4;
    }
}

void CharScenes_AreaDump() {
    if (CharScene_Area == NULL) {
        return;
    }
    for (i32 i = 0; i < CHARCOUNT; ++i) {
        CHARSCENE_s &entry = CharScene_Area[i];
        if (entry.scene != NULL) {
            NuGScnRemove(entry.scene);
        }
        entry.scene = NULL;
    }
}

void CharScenes_AreaLoad(APICHARACTERMODELLIST_s *list, variptr_u *buf, variptr_u buf_end) {
    if (CharScene_Area == NULL || apicharsys->loaded_model_count == 0) {
        return;
    }

    for (i32 i = 0; i < apicharsys->loaded_model_count; ++i) {
        const i32 model_id = apicharsys->models[i].model_id;
        CHARSCENE_s &entry = CharScene_Area[model_id];
        if (entry.scene != NULL || (CDataList[model_id].flags & 1) == 0) {
            continue;
        }

        i32 list_index;
        if (InModelList(list, model_id, &list_index) == 0 || list[list_index].count == 0) {
            continue;
        }

        char path[128];
        sprintf(path, "chars\\%s\\%s.gsc", CDataList[model_id].dir, CDataList[model_id].file);
        entry.scene = NuGScnRead(buf, buf_end, path);
        if (entry.scene != NULL) {
            NuSpecialFind(entry.scene, &entry.special_scene, CDataList[model_id].file, 1);
        }
    }
}

void DeactivateCharacter(char *) {
}

void LoadSingleCharacter(bgprocinfo_s *) {
    APICHARACTERMODELLIST_s list[2] = {
        {static_cast<i16>(waiting_for_character), 1},
        {-1, 0},
    };

    apiloadcharactermodels_append = 1;
    apiloadcharactermodels_nopakfile = CHARPAK == 0;
    APILoadCharacterModels(list, 0, &characterbuffer_ptr, characterbuffer_end, 1);
    IconScenes_Load(list, 0, &characterbuffer_ptr, &characterbuffer_end);

    NUGSCN *icon_scene = IconScene_FindById(list[0].model_id);
    if (icon_scene != NULL && LEVELOBJECTCOUNT > 0) {
        for (i32 i = 0; i < LEVELOBJECTCOUNT; ++i) {
            if (ObjTab[i].kind != 3) {
                continue;
            }

            LEVEL_OBJECT_RUNTIME &object = WORLD->lev_objs[i];
            if (object.active != 0) {
                continue;
            }

            if (NuSpecialFind(icon_scene, &object.special, ObjTab[i].name, 1) != 0) {
                object.active = 1;
            }
        }
    }

    Customiser_SaveModelTextureIDs(CharacterCustomiser, APICharacterLoaded(list[0].model_id));
    hub_character_ready = waiting_for_character;
    waiting_for_character = -1;
    g_loadingCharacterInHub = 0;
}

void UpdateCharacterLoad() {
    const i32 menu_id = GetMenuID();
    if (menu_id == 13 || (menu_id >= 15 && menu_id <= 19)) {
        return;
    }

    if (LOADEROFF != 0 || BGLOAD == 0 || waiting_for_character != -1 || hub_character_ready != -1 ||
        bgGetProcActive() != NULL || global_dlist_manager.ndisplay_lists > 0xfd) {
        return;
    }

    const i32 required_buffer =
        (static_cast<i32>((static_cast<f32>(CHARACTERBUFFERSIZE) / 7077888.0f) * 1048576.0f) + 0x3ff) & ~0x3ff;
    if (required_buffer > static_cast<i32>(characterbuffer_end.addr - characterbuffer_ptr.addr)) {
        return;
    }

    i32 candidates[0x154] __attribute__((aligned(16)));
    i32 candidate_count = 0;
    i32 candidate = 0;
    i16 *fixed_candidate = NULL;
    i32 fixed_value = -1;

    if (id_BARMAN != -1 && APICharacterLoaded(id_BARMAN) == NULL) {
        fixed_candidate = &id_BARMAN;
        goto queue_fixed_character;
    }
    if (id_CANTINABAND != -1 && APICharacterLoaded(id_CANTINABAND) == NULL) {
        fixed_candidate = &id_CANTINABAND;
        goto queue_fixed_character;
    }
    if (id_WEIRDO1 != -1 && APICharacterLoaded(id_WEIRDO1) == NULL) {
        fixed_candidate = &id_WEIRDO1;
        goto queue_fixed_character;
    }
    if (id_WEIRDO2 != -1 && APICharacterLoaded(id_WEIRDO2) == NULL) {
        fixed_candidate = &id_WEIRDO2;
        goto queue_fixed_character;
    }
    if (id_JABBA != -1 && APICharacterLoaded(id_JABBA) == NULL) {
        fixed_candidate = &id_JABBA;
        goto queue_fixed_character;
    }
    if (id_MOSEISLEYCITIZEN != -1 && APICharacterLoaded(id_MOSEISLEYCITIZEN) == NULL) {
        fixed_candidate = &id_MOSEISLEYCITIZEN;
        goto queue_fixed_character;
    }
    if (id_CANTINAALIEN != -1 && APICharacterLoaded(id_CANTINAALIEN) == NULL) {
        fixed_candidate = &id_CANTINAALIEN;
        goto queue_fixed_character;
    }
    if (id_WOMPRAT != -1 && APICharacterLoaded(id_WOMPRAT) == NULL) {
        fixed_candidate = &id_WOMPRAT;
        goto queue_fixed_character;
    }

    for (i32 pack = 0; pack < 11; ++pack) {
        if (g_lowEndLevelBehaviour != 0 && Hub_LowEnd_IconsInsteadOfModels != 0) {
            continue;
        }
        if (Store_IsPackUnlocked(pack) != 0 || StorePack[pack].id == NULL) {
            continue;
        }

        const i32 id = *StorePack[pack].id;
        if (id != -1 && APICharacterLoaded(id) == NULL) {
            fixed_value = id;
            goto queue_fixed_value;
        }
    }

    for (i16 id = 0; id < CHARCOUNT; ++id) {
        if (id == id_DROIDEKA || (CDataList[id].model_flags & 0x04002000) != 0 || APICharacterLoaded(id) != NULL) {
            continue;
        }
        if (static_cast<i32>(CDataList[id].field5_0x14) + apicharsys->loaded_animation_count >
            apicharsys->animation_capacity) {
            continue;
        }
        if (InCollectList_Index(id, NULL, 0) == -1 || Collection_Got(id) == 0) {
            continue;
        }
        candidates[candidate_count++] = id;
    }

    if (candidate_count == 0) {
        return;
    }

    if (candidate_count != 1) {
        candidate = qrand() / ((0xffff / candidate_count) + 1);
    }
    goto queue_character;

queue_fixed_character:
    candidates[0] = *fixed_candidate;
    candidate = 0;
    goto queue_character;

queue_fixed_value:
    candidates[0] = fixed_value;
    candidate = 0;
queue_character:
    waiting_for_character = candidates[candidate];
    WaitingForCharacterTime = 0.0f;
    bgPostRequest(LoadSingleCharacter, NULL, NULL, 0);
}

void CharScenes_LevelDump(WORLDINFO_s *) {
}

void CollectAllCharacters(i32) {
}

extern VARIPTR characterbuffer_base;
extern i32 CHARACTERBUFFERSIZE;
extern i32 Area;
extern i32 last_area;

void ResetCharacterBuffer(i32 force_reset) {
    if (force_reset == 0 && Area != -1 && Area == last_area) {
        return;
    }

    characterbuffer_ptr = characterbuffer_base;
    memset(characterbuffer_base.void_ptr, 0, CHARACTERBUFFERSIZE);
    apicharsys->loaded_model_count = apicharsys->permanent_model_count;
    apicharsys->animation_load_attempts = apicharsys->area_animation_count;
    apicharsys->loaded_animation_count = apicharsys->area_animation_count;
}

void CollectCharcters_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32) {
}

void CollectCharcters_Skip(STATUS_STAGE_s *, STATUSPACKET_s *) {
}

void E1CharacterBonus_Init(WORLDINFO_s *) {
}

void LocalGetRandomLocator(AILOCATOR_s **, i32, float, nuvec_s *, float, i32, float, float) {
}

void PostAnimate_ASTROMECH(GameObject_s *) {
}

nuhspecial_s *CharScene_FindHSpecial(WORLDINFO_s *world, i32 character_id) {
    CHARSCENE_s *scene;
    if (CharScene_Area != NULL && CharScene_Area[character_id].scene != NULL) {
        scene = &CharScene_Area[character_id];
    } else {
        scene = &world->minikit.character_scenes[character_id];
        if (scene->scene == NULL) {
            scene = NULL;
        }
    }
    if (scene != NULL && NuSpecialExistsFn(&scene->special_scene) == 0) {
        scene = NULL;
    }
    return scene == NULL ? NULL : &scene->special_scene;
}

void LocalGetNearestLocator(AILOCATOR_s **, i32, float, nuvec_s *, float, i32, float, float) {
}

void newCharactersCollected(STATUSPACKET_s *) {
}

void CollectCharcters_Update(STATUS_STAGE_s *, STATUSPACKET_s *, float) {
}

void RegisterGizmoTypes_Indy(variptr_u *, variptr_u *) {
}

void Area_CharIDInCurrentList(i32) {
}

void SetProtocolDroidFallAnim(GameObject_s *object) {
    static const i16 fall_animations[] = {76, 75, 40};
    const u8 variant = object->field_0xe38;
    object->apiobj.anim_packet.requested_animation = variant >= 1 && variant <= 3 ? fall_animations[variant - 1] : 5;
}

void CollectCharactersOff_Draw(STATUS_STAGE_s *, STATUSPACKET_s *, i32) {
}

void CollectCharactersOff_Skip(STATUS_STAGE_s *, STATUSPACKET_s *) {
}

void ScaleGameObject(GameObject_s *obj);

void SetGameObjectCharacterData(GameObject_s *obj) {
    CHARACTERDATA *data = obj->apiobj.character_data;
    obj->apiobj.field_0xa8 = data->field17_0x3c;
    obj->field_0x1008 = data->field14_0x30;
    obj->field_0xffc = data->field15_0x34;
    obj->field_0x1000 = data->field16_0x38;
    ScaleGameObject(obj);
}

void CollectCharactersOff_Update(STATUS_STAGE_s *, STATUSPACKET_s *, float) {
}

void TakeOverYodaSeekDistanceHack(GameObject_s *, GameObject_s *, nuvec_s *) {
}

void SetProtocolDroidInterfaceAction(GameObject_s *object) {
    if (object->field_0xe38 == 3)
        object->context_animation = 0x45;
    else if (object->field_0xe38 == 2)
        object->context_animation = 0x46;
    else if (object->field_0xe38 == 1)
        object->context_animation = 0x47;
}

void SetProtocolDroidDeactivatedAction(GameObject_s *object) {
    if (object->field_0xe38 == 3)
        object->context_animation = 0x42;
    else if (object->field_0xe38 == 2)
        object->context_animation = 0x43;
    else if (object->field_0xe38 == 1)
        object->context_animation = 0x44;
}

void LoadPerm1() {
    char buf[0x100];

    rtlInitDynamic(&permbuffer_ptr, superbuffer_end, 0x40);
    permbuffer_ptr.addr = (permbuffer_ptr.addr + 3) & ~3u;
    DebrisSetup(&permbuffer_ptr, permbuffer_end, (char *)"stuff\\particle", 0x100, 0x200, 0x200);
    DebrisRegisterCutoffCameraVec(reinterpret_cast<NUVEC *>(&global_camera.mtx.m30));
    if (Grass_Available != 0) {
        edgraSetup(&permbuffer_ptr, permbuffer_end, 0x200, 0x20, 0x40);
    }
    InitParts(0x40, &permbuffer_ptr, permbuffer_end);
    ParticleReset();

    NuStrCpy(buf, (char *)"stuff\\general.ptl");
    if (NuFileExists(buf) != 0) {
        DEBPAGE_GENERAL = edppLoadPage(buf, 0, 0);
    }
    NuStrCpy(buf, (char *)"stuff\\char.ptl");
    if (NuFileExists(buf) != 0) {
        DEBPAGE_CHARACTER = edppLoadPage(buf, 5, 0);
    }
    perm_debrissys = InitGameDebris(&permbuffer_ptr, permbuffer_end, 0x190, 0x93, debris_name, 0);

    permbuffer_ptr.addr = (permbuffer_ptr.addr + 3) & ~3u;
    InitStreaks(&permbuffer_ptr, permbuffer_end, (char *)"stuff\\streak.pnt");
    permbuffer_ptr.addr = (permbuffer_ptr.addr + 3) & ~3u;
    InitRopeMtl((char *)"rope", &permbuffer_ptr, &permbuffer_end);
    InitRipples(&ripples, &permbuffer_ptr, &permbuffer_end, 0x40);
    permbuffer_ptr.addr = (permbuffer_ptr.addr + 0xff) & ~0xffu;

    ShadowMat = NuMtlCreate3D(1);
    ShadowMat->diffuse_color.r = 1.0f;
    ShadowMat->diffuse_color.g = 1.0f;
    ShadowMat->diffuse_color.b = 1.0f;
    ShadowMat->sort_pri = 0xff;
    ShadowMat->opacity = 0.999f;
    u8 *attributes = reinterpret_cast<u8 *>(&ShadowMat->attribs);
    attributes[1] = (attributes[1] & 0x0f) | 0x60;
    attributes[0] = (attributes[0] & 0xf0) | 1;
    attributes[2] = (attributes[2] & 0x8c) | 0x12;
    ShadowMat->tex_id = static_cast<i16>(NuTexRead((char *)"stuff\\gradient", &permbuffer_ptr, &permbuffer_end));
    NuMtlUpdate(ShadowMat);

    u8 shadow_random[0x800];
    for (i32 i = 0; i < 0x800; ++i) {
        shadow_random[i] = static_cast<u8>(qrand() >> 8);
    }
    NuRndrShadowInit(shadow_random);
    CreateFadeMaterials();
    CreateUsefulMaterials();

    edgraClumpsReset();
    edanimParamReset();
    NuTexAnimProgSysInit();
    InitTexAnimScripts(TexAnimList_LSW);

    permbuffer_ptr.addr = (permbuffer_ptr.addr + 3) & ~3u;
    things_scene = NuGScnRead(&permbuffer_ptr, permbuffer_end, (char *)"stuff\\things.gsc");
    NUGSCN *terrain_scene = NULL;
    if (things_scene != NULL) {
        edbitsRegisterThingsScene(things_scene);
        terrain_scene = things_scene;
        if (things_scene->display_list != NULL) {
            things_scene->display_list->flags |= NU_DISPLAYSCENE_FLAG_NEEDS_BUILD;
        }
    }

    things_scene_terrain = TerrainInitEx(-1, &permbuffer_ptr, permbuffer_end.void_ptr, 0, (char *)"stuff\\things",
                                         terrain_scene, 0, 0x14, 0x14, 0x14);
    terrainpickupinit((char *)"stuff\\things", &things_scene_terrain);
    BackDrop_Init((char *)"stuff\\starfield.gsc", &permbuffer_ptr, &permbuffer_end);

    NuMtlSetCurrentRenderPlane(0xf);
    edpartSetParticlePage(DEBPAGE_GENERAL);
    NuStrCpy(buf, (char *)"stuff\\general.par");
    if (NuFileExists(buf) != 0) {
        PARTPAGE_GENERAL = edpartLoadPage(buf, 0, things_scene);
    }
    NuMtlSetCurrentRenderPlane(0);
    AIScriptLoadAll((char *)"scripts", &permbuffer_ptr, &permbuffer_end, NULL);
}

void LoadPerm2() {
    extern i16 tALL;
    extern i16 tJEDI;
    extern i16 tBLASTER;
    extern i16 tBOUNTYHUNTERCHARACTERS;

    CharConfig_ConfigureAll(1, ConfigChar_GameKeywords);
    ExtraCharacterFixUpAfterConfig();
    apiloadcharactermodels_nopakfile = CHARPAK == 0;
    APILoadCharacterModels(PermModelList, 1, &permbuffer_ptr, permbuffer_end, 1);

    Collection_CreateMaster(const_cast<char *>("All"), NULL, &MasterCollection, 15, COLLECTION_DEFAULTSCALE);
    Collection_CreateCustom(const_cast<char *>("Characters"), &tALL, &CharacterCollection, 0, 0x04002000, 0, 0, 16,
                            &permbuffer_ptr, &permbuffer_end, 0, 0.725f);
    Collection_CreateCustom(const_cast<char *>("Vehicles"), NULL, &VehicleCollection, 0x2000, 0x04000000, 0, 0, 7,
                            &permbuffer_ptr, &permbuffer_end, 0, 1.0f);
    Collection_CreateCustom(const_cast<char *>("Minikits"), NULL, &MiniKitCollection, 0x04000000, 0, 0, 0, 12,
                            &permbuffer_ptr, &permbuffer_end, 0, COLLECTION_DEFAULTSCALE);
    Collection_CreateCustom(const_cast<char *>("Shop"), NULL, &ShopCollection, 0, 0, 0, 1, 10, &permbuffer_ptr,
                            &permbuffer_end, 0, COLLECTION_DEFAULTSCALE);
    Collection_CreateCustom(const_cast<char *>("Jedi"), &tJEDI, &JediCollection, 8, 0, 0, 0, 7, &permbuffer_ptr,
                            &permbuffer_end, 0, COLLECTION_DEFAULTSCALE);
    Collection_CreateCustom(const_cast<char *>("BlasterZipUps"), &tBLASTER, &BlasterCollection, 0x100080, 0x01000000, 0,
                            0, 11, &permbuffer_ptr, &permbuffer_end, 0, COLLECTION_DEFAULTSCALE);
    Collection_CreateCustom(const_cast<char *>("BountyHunters"), &tBOUNTYHUNTERCHARACTERS, &BountyHunterCollection,
                            0x01000000, 0, 0, 0, 4, &permbuffer_ptr, &permbuffer_end, 0, COLLECTION_DEFAULTSCALE);
    Areas_ConfigureResidents(&permbuffer_ptr, &permbuffer_end);
}

void MapToGrid(nuvec_s *, nuvec_s *, i32 *, i32 *, nuvec_s *, nutexmanager_s *) {
}
