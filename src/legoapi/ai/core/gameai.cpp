// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "globals.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/numtx.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/props/objects/tightrope.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/ai/core/legoai.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/menus/screens/arcade.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edui.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/world/mission.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/store.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/world/area.h"
#include "legoapi/core/input/timer.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include <stdio.h>
#include <string.h>
#include "legoapi/items/objects/gameobjects_types.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/ai/game/creature.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "nu2api/nucore/nuanim3.h"
#include <stdint.h>
#include "nu2api/nucore/nulist.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/items/base/apiobject.h"
#include "gameapi/ai/aisys/aiscript_types.h"
#include "legoapi/actions/character/simpletons_types.h"
#include <float.h>
#include <math.h>
#include "editor/edpath.h"
#include "batman.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufpar.h"
#include "editor/aieditor_state.h"
#include "nu2api/numath/nuplane.h"
#include "editor/aieditor_settings.h"
#include "legoapi/render/core/terrain_internal.h"
#include "nu2api/nu3d/numtl.h"
#include "editor/edpath_types.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/characters/motion/contexts.h"
#include <stdlib.h>
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/fx/edsplines.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "gameframework/saveload.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include "legoapi/render/fx/game_deb.h"
#include <stddef.h>
#include "gameapi/ai/aisys/gameaiscript.h"
#include "legoapi/ai/core/aisysall.h"
#include "legoapi/ai/core/gameai.h"
#include "legoapi/ai/game/aipathcnxhelper.h"
#include "legoapi/ai/game/aitrigger.h"
#include "legoapi/ai/game/starwars_gameai.h"
#include "legoapi/characters/motion/move.h"
#include "legoapi/gizmo/base/gizflow.h"
#include "legoapi/gizmos/traps/gizturret.h"
#include "legoapi/world/levels.h"
#include "legoapi/world/levels/hub.h"

i8 adtabentries[9][4] = {{-1, -1, -1, -1}, {-1, -1, -1, 0}, {-1, -1, 0, 0}, {-1, 0, 0, 0}, {0, 0, 0, 0},
                         {1, 0, 0, 0},     {1, 1, 0, 0},    {1, 1, 1, 0},   {1, 1, 1, 1}};
i8 (*adtab)[4] = &adtabentries[4];

i32 Suit_GetIndex(SUIT_s *);
extern i16 id_GAMORREANGUARD;
f32 DEFENDTIME = 4.0f;
i32 teleport_all_freeplay_modes = 1;
i32 drop_in_teleport = 3;

extern "C" {
    extern i16 id_LANDSPEEDER;
}

// Enabled in the target data image.  Ordinary background characters are
// staggered across frames; special movement states opt back into full-rate
// processing through the forced-update path.

static const f32 AI_RESPAWN_DELAY = 2.0f;

extern "C" {
extern i32 nbaddies_can_see_players;
}

extern "C" {
    f32 default_path_heighttol = 0.2f;
    u8 default_activate_difficulty = AI_DEFAULT_ACTIVATE_DIFFICULTY;
    u8 default_min_n_respawns;
    u8 default_max_n_respawns;
    f32 default_min_t_respawn;
    f32 default_max_t_respawn;

}

// The special-route list reserves the first ten ids for suit characters.  The
// remaining ids enumerate the current story list while omitting variants that
// are represented by those dedicated routes.
static const char *skip_chars[] = {"Batman", "Robin", "Glide_Pack", NULL};

f32 draw_attention_distance = 2.0f;
extern "C" i32 party_under_cover;
i32 test_new_interaction;
i32 debug_ignore_players;
i32 WALKSTEALTH;
i32 active_goody_count;
f32 timetogetlos;

#if defined(__i386__)
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone, regparm(2), sseregparm))
#else
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone))
#endif

static GAMEOBJECTS_CLONE_ATTR2_SSE void TestWalkAround(APIOBJECT *object, f32 *other_x, f32 *other_z,
                                                       NUVEC *difference, f32 radius)
    __asm__("_ZL14TestWalkAroundP11APIOBJECT_sS0_P7nuvec_sf.isra.10");

static void TestWalkAround(APIOBJECT *object, f32 *other_x, f32 *other_z, NUVEC *difference, f32 radius) {
    if (difference->x * difference->x + difference->z * difference->z < radius * radius) {
        difference->x = object->position.x - *other_x;
        difference->z = object->position.z - *other_z;
        i32 angle = NuAtan2D(difference->x, difference->z);
        difference->x = object->ai->movement_destination.x - *other_x;
        difference->z = object->ai->movement_destination.z - *other_z;
        i32 turn = NuAngSub(NuAtan2D(difference->x, difference->z), angle);
        i32 limit = static_cast<i32>((ai_moveradius / radius) * 10430.378f);
        if (turn > limit)
            turn = limit;
        else if (turn < -limit)
            turn = -limit;
        angle = NuAngAdd(angle, turn);
        difference->x = 0.0f;
        difference->y = 0.0f;
        difference->z = radius;
        NuVecRotateY(difference, difference, angle);
        object->ai->movement_position.x = difference->x + *other_x;
        object->ai->movement_position.y = object->ai->movement_destination.y;
        object->ai->movement_position.z = difference->z + *other_z;
    }
}

extern "C" {
void SetScriptErrorLevel(SCRIPT_ERROR_LEVEL level);
}

extern i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER;

extern "C" {
    i32 party_under_cover;
}

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

GameObject_s *FindNearestGameObject(NUVEC *position, GameObject_s *exclude, u32 required_flags, f32 radius,
                                    f32 extra_radius, i32 animation, i32 character_id, i32 player_index,
                                    f32 *distance_squared, i32 horizontal_only, i32 (*filter)(GameObject_s *),
                                    bool first_match);

GameObject_s *alert_obj;
NUVEC alert_pos;
f32 alert_timer;
f32 alert_time = 10.0f;

extern "C" void *AIPAthFindPathCnx(AISYS_s *, AIPATH_s *, char *, char *, i32 *);

struct nuhspecial_s;
struct nuvec_s;

static AtOnceAttacker AtOnce_attackingPlayer[8][17];
static i32 AtOnce_attackersPerRow = 32;
static f32 AtOnce_InitialRowDist = 0.75f;
static f32 AtOnce_RowDist = 0.75f;
static i32 AtOnce_maxAttackers = 1;

extern "C" void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from, char *to, i32 *direction);

extern AIACTIONDEF api_aiactiondefs[55];
extern AICONDITIONDEF api_aiconditiondefs[50];

DECOMP_ASSERT(sizeof(api_aiactiondefs) == 0x294, "API action registry size");

DECOMP_ASSERT(sizeof(api_aiconditiondefs) == 0x258, "API condition registry size");

extern "C" {
extern void (*checkantinodefns[3])(APIOBJECT *, AIANTINODE *, NUVEC *, f32);
    extern void (*checkantinodefns[3])(APIOBJECT_s *, AIANTINODE_s *, NUVEC *, f32);
}

extern "C" {
void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from_name, char *to_name, i32 *direction);
} // extern "C"

struct AIPATH_s;

struct EDCREATURE_s;

extern "C" {
    struct EDCREATURE_s;
}

extern f32 DEFENDTIME;

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern i32 nbaddies_can_see_players;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

extern "C" {
void SetScriptErrorLevel(SCRIPT_ERROR_LEVEL level);
}

extern "C" {
extern i32 party_under_cover;
}

extern i16 tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);

extern i16 id_ANAKINJEDISCARRED, id_WOOKIEE, id_TWOFACE;

struct APIOBJECT_s;
struct AISYS_s;
struct AIGROUP_s;

extern "C" {
    extern "C++" void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
}

extern "C" {
extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId();
} // extern "C"

extern "C" {
void SetScriptErrorLevel(SCRIPT_ERROR_LEVEL level);

}

extern "C" {
void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from_name, char *to_name, i32 *direction);

}

static char *LevelCharacterName(u8 character_index) {
    if (character_index == 0xff || CurrentStoryCList == NULL) {
        return NULL;
    }

    const i16 character_type = CurrentStoryCList[character_index].model_id;
    if (character_type == -1) {
        return NULL;
    }
    return apicharsys->char_data[character_type].file;
}

static i32 LevelCharacterGlobalID(u8 character_index) {
    if (character_index == 0xff || CurrentStoryCList == NULL) {
        return -1;
    }
    return CurrentStoryCList[character_index].model_id;
}

static char *GlobalCharacterName(i32 character_type) {
    if (character_type == -1 || character_type >= apicharsys->character_count) {
        return NULL;
    }
    return apicharsys->char_data[character_type].file;
}

static void *GlobalCharacterHGobj(i32 character_type) {
    if (character_type == -1) {
        return NULL;
    }

    const i16 model_index = apicharsys->playermodelids[character_type];
    if (model_index == -1) {
        return NULL;
    }
    return apicharsys->models[model_index].hierarchy;
}

static f32 GetViewRange(i32 character_type) {
    if (character_type == -1) {
        return 0.0f;
    }
    return static_cast<GAMECHARACTERDATA *>(apicharsys->char_data[character_type].field11_0x24)->viewdistance;
}

static f32 GetHearDistance(i32 character_type) {
    if (character_type == -1) {
        return 0.0f;
    }
    return static_cast<GAMECHARACTERDATA *>(apicharsys->char_data[character_type].field11_0x24)->heardistance;
}

static f32 GetMaxViewHeight(i32 character_type) {
    if (character_type == -1) {
        return 0.0f;
    }
    return static_cast<GAMECHARACTERDATA *>(apicharsys->char_data[character_type].field11_0x24)->maxviewheight;
}

static f32 GetMinViewHeight(i32 character_type) {
    if (character_type == -1) {
        return 0.0f;
    }
    return static_cast<GAMECHARACTERDATA *>(apicharsys->char_data[character_type].field11_0x24)->minviewheight;
}

APIOBJECT *GameAPIOBJECTFromObjID(u8 object_id) {
    if (object_id >= HIGHGAMEOBJECT) {
        return NULL;
    }

    GameObject_s *object = &Obj[object_id];
    if ((object->apiobj.flags_low & APIOBJECT_FLAG_IN_USE) == 0) {
        return NULL;
    }

    if ((object->apiobj.flags_high & APIOBJECT_HIGH_FLAG_PLAYER_CHARACTER) != 0 ||
        object->ai.reset_mode == AI_OBJECT_ROUTE_STATE_SCRIPT_VISIBLE) {
        if (object->apiobj.field_0x287 == 0 || object->field_0x101c > 0.0f) {
            return &object->apiobj;
        }
    }
    return NULL;
}

static i32 AI_GetMaxGizmos(void *) {
    return 32;
}

static i32 AI_GetOutput(GIZMO *gizmo, i32, i32) {
    if (gizmo == NULL || gizmo->object == NULL) {
        return 0;
    }
    LEVELSCRIPTPROCESS *process = static_cast<LEVELSCRIPTPROCESS *>(gizmo->object);
    return process->processor.is_disabled == 0;
}

static i32 AI_GetNumOutputs(GIZMO *) {
    return 1;
}

char *AI_GetOutputName(GIZMO *, i32) {
    static char name[64] = "Processing";
    return name;
}

void AI_Activate(GIZMO *gizmo, i32 active) {
    if (gizmo != NULL && gizmo->object != NULL) {
        LEVELSCRIPTPROCESS *process = static_cast<LEVELSCRIPTPROCESS *>(gizmo->object);
        process->processor.is_disabled = active == 0;
    }
}

static char *AI_GetGizmoName(GIZMO *gizmo) {
    static char name[16];
    if (gizmo == NULL || gizmo->object == NULL) {
        return NULL;
    }

    LEVELSCRIPTPROCESS *level_process = static_cast<LEVELSCRIPTPROCESS *>(gizmo->object);
    if (NuStrLen(level_process->name) != 0) {
        return level_process->name;
    }

    AISCRIPTPROCESS *process = &level_process->processor;
    if (process->script != NULL && process->script->name != NULL) {
        NuStrNCpy(name, process->script->name, sizeof(name));
    } else {
        NuStrCpy(name, "");
    }

    const i8 length = NuStrLen(name);
    if (process->unknown_a8 != NULL) {
        i8 total = length;
        total += NuStrLen("()");
        total += NuStrLen(process->unknown_a8->name);
        if (total <= 15) {
            NuStrCat(name, "(");
            NuStrCat(name, process->unknown_a8->name);
            NuStrCat(name, ")");
        }
    } else if (process->unknown_a4 != NULL) {
        i8 total = length;
        total += NuStrLen("()");
        total += NuStrLen(process->unknown_a4->name);
        if (total <= 15) {
            NuStrCat(name, "(");
            NuStrCat(name, process->unknown_a4->name);
            NuStrCat(name, ")");
        }
    } else if (process->unknown_a0 != NULL) {
        i8 total = length;
        total += NuStrLen("()");
        total += NuStrLen(process->unknown_a0->name);
        if (total <= 15) {
            NuStrCat(name, "(");
            NuStrCat(name, process->unknown_a0->name);
            NuStrCat(name, ")");
        }
    }
    return name;
}

static void AI_AddGizmos(GIZMOSYS *gizmo_sys, i32 type_id, void *world_ptr, void *unused) {
    WORLDINFO *world;
    i32 i;

    world = (WORLDINFO *)world_ptr;

    // ORIG_BUG: This comparison can never fail.
    if (world->processors != NULL) { // NOLINT
        for (i = 0; i < WORLD->processor_count; i++) {
            AddGizmo(gizmo_sys, type_id, NULL, &world->processors[i]);
        }
    }
}

static NUVEC *GetAICreatureOrigin(AISYS *system, AIPACKET *packet) {
    if (packet == NULL || system == NULL || packet->field_0x134 == 0xff)
        return NULL;
    if ((packet->navigation_flags & 8) == 0) {
        AICREATURE *creature = &system->creatures[packet->field_0x134];
        NUVEC offset;
        offset.x = ((packet->group_column + 1) >> 1) * creature->x_spacing;
        if ((packet->group_column & 1) != 0)
            offset.x = -offset.x;
        offset.y = 0.0f;
        offset.z = -creature->z_spacing * (f32)(u32)packet->group_row;
        NuVecRotateY(&offset, &offset, creature->y_rot);
        NuVecAdd(&packet->creature_origin, &offset, &creature->pos);
        packet->navigation_flags |= 8;
    }
    return &packet->creature_origin;
}

static APIOBJECT *GetNamedAPIObject(AISYS *system, char *name) {
    APIOBJECT *found = NULL;
    if (system != NULL) {
        for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
            if ((Obj[index].apiobj.field_0x1f8 & APIOBJECT_FLAG_IN_USE) == 0) {
                continue;
            }
            if ((Obj[index].apiobj.field_0x1f4 & 0x400) != 0 && Obj[index].ai.field_0x134 != 0xff) {
                auto *creature = &system->creatures[Obj[index].ai.field_0x134];
                if (creature != NULL && NuStrICmp(creature->name, name) == 0) {
                    found = &Obj[index].apiobj;
                    break;
                }
            }
            if (NuStrICmp(Obj[index].apiobj.character_data->file, name) == 0) {
                found = &Obj[index].apiobj;
                break;
            }
        }
    }
    if (found != NULL) {
        return found;
    }

    if (NuStrICmp(name, "player") == 0) {
        return player != NULL ? &player->apiobj : NULL;
    }
    if (NuStrICmp(name, "player_2") == 0) {
        GameObject_s *second = Player[0] == player ? Player[1] : Player[0];
        return second != NULL ? &second->apiobj : NULL;
    }
    for (i32 index = 0; index < 8; ++index) {
        if (Player[index] == NULL) {
            continue;
        }
        char player_name[72];
        sprintf(player_name, "Player%d", index);
        if (NuStrICmp(player_name, name) == 0) {
            return &Player[index]->apiobj;
        }
    }
    return NULL;
}

static i32 GlobalCharacterTypeID(char *name) {
    for (i32 character_type = 0; character_type < apicharsys->character_count; ++character_type) {
        if (NuStrICmp(name, apicharsys->char_data[character_type].file) == 0) {
            return character_type;
        }
    }
    return -1;
}

static i32 GameFindAlternativeSpecialObject(AISYS *, nuhspecial_s *special) {
    return EquivalentObject_Find(WorldInfo_CurrentlyActive(), special);
}

static void GameAILoad(AISYS *system, i32 version, NUGSCN *, VARIPTR *, VARIPTR *) {
    if (__builtin_expect(version < 12 && system->path_sys != NULL && system->path_sys->path_count != 0, 1)) {
        AIPATHSYS *path_system = system->path_sys;
        AIPATH **paths = path_system->paths;
        for (u32 path_index = 0; path_index < path_system->path_count; ++path_index) {
            AIPATH *path = paths[path_index];
            AIPATHCNX *entries = path->connections;
            const i32 connection_count = path->connection_count;
            for (i32 connection = 0; connection < connection_count; ++connection) {
                entries[connection].traversal_flags[0] = 0;
                entries[connection].original_traversal_flags[0] = 0;
                entries[connection].traversal_flags[1] = 0;
                entries[connection].original_traversal_flags[1] = 0;
            }
        }
    }

    i32 tag_length = static_cast<i8>(EdFileReadChar());
    if (__builtin_expect(tag_length <= 8 && tag_length != 0, 0)) {
        char tag[16];
        EdFileRead(tag, tag_length);
        if (NuStrICmp(tag, "LEGO") == 0) {
            EdFileReadInt();
            // Retail allocates through a fresh 16-aligned cursor against the
            // system's storage end instead of the caller's buffer.
            VARIPTR cursor __attribute__((aligned(16)));
            system->game_sys = AISysBufferAlloc(&cursor, &system->storage_end, 4);
        }
    }
}

static void GlobalCharacterRender(NUVEC *position, i16 angle, i32 character, i32, EDCREATURE_s *) {
    i16 layers[32];
    if (character < CHARCOUNT) {
        i16 model_id = apicharsys->playermodelids[character];
        if (model_id != -1) {
            CHARACTERMODEL_s *model = &apicharsys->models[model_id];

            i32 layer_count = GCDataList[model->model_id].make_layer_list(
                model, layers, apicharsys->char_data[character].game_character->layer_mask);
            if (layer_count > 0) {
                angle = NuAngAdd(angle, 0x8000);
                NUVEC scale;
                scale.x = scale.y = scale.z = CDataList[character].model_scale;
                NUMTX_ALIGNED16 matrix;
                NuMtxSetScale(&matrix, &scale);
                NuMtxTranslate(&matrix, position);
                NuMtxPreRotateY(&matrix, angle);
                NuHGobjRndr(model->hierarchy, &matrix, layer_count, layers);
            }
        }
    }
}

static f32 GetCharacterGoalSpeed(APIOBJECT *object) {
    if (object == NULL || object->ai == NULL) {
        return 0.0f;
    }

    switch (object->ai->goal_speed_mode) {
        case AI_ACTION_SPEED_RUN:
            return static_cast<GAMECHARACTERDATA *>(object->character_data->field11_0x24)->movement_speed * FRAMETIME;
        case AI_ACTION_SPEED_WALK:
            return static_cast<GAMECHARACTERDATA *>(object->character_data->field11_0x24)->field_0x18 * FRAMETIME;
        case AI_ACTION_SPEED_TIPTOE:
            return static_cast<GAMECHARACTERDATA *>(object->character_data->field11_0x24)->field_0x14 * FRAMETIME;
        default:
            return 0.0f;
    }
}

static i32 GameAIActionParseSpeed(char *name, u8 *speed) {
    if (NuStrICmp(name, "RUN") == 0) {
        *speed = AI_ACTION_SPEED_RUN;
        return 1;
    }
    if (NuStrICmp(name, "WALK") == 0) {
        *speed = AI_ACTION_SPEED_WALK;
        return 1;
    }
    if (NuStrICmp(name, "TIPTOE") == 0) {
        *speed = AI_ACTION_SPEED_TIPTOE;
        return 1;
    }
    return 0;
}

static char *SpecialRouteCharacterName(u8 route_id) {
    if (route_id == 0xff) {
        return NULL;
    }
    if (route_id < 10) {
        return Suit[route_id].suit_character_name;
    }

    i32 route_index = 0;
    i32 skipped_count = 0;
    while (CurrentStoryCList[route_index + skipped_count].model_id != -1 && route_index + skipped_count < 64) {
        char *name = apicharsys->char_data[CurrentStoryCList[route_index + skipped_count].model_id].file;
        bool skip = false;
        for (i32 skip_index = 0; skip_chars[skip_index] != NULL; ++skip_index) {
            if (NuStrICmp(name, skip_chars[skip_index]) == 0) {
                skip = true;
                break;
            }
        }
        if (skip) {
            ++skipped_count;
            continue;
        }
        if (route_index + 10 == route_id) {
            return apicharsys->char_data[CurrentStoryCList[route_index + skipped_count].model_id].file;
        }
        ++route_index;
    }
    return NULL;
}

static i32 LevelCharacterTypeID(char *name) {
    i32 result = 0x40;
    if (NuStrICmp(name, "Everyone") != 0) {
        result = -1;
        if (CurrentStoryCList != NULL) {
            for (i32 index = 0; CurrentStoryCList[index].model_id != -1 && index < 64; ++index) {
                if (NuStrICmp(name, apicharsys->char_data[CurrentStoryCList[index].model_id].file) == 0) {
                    result = index;
                    break;
                }
            }
        }
    }
    return result;
}

static u32 AIBigJumpToDestination(APIOBJECT *object, NUVEC *destination) {
    if (destination == NULL || object == NULL || object->objptr == NULL || object->field_0x287 != 0 ||
        object->objptr->field_0x7a5 == AI_GAME_OBJECT_TYPE_VEHICLE) {
        return 1;
    }

    GameObject_s *game_object = object->objptr;
    if (game_object->field_0xcc0 == NULL) {
        if ((object->character_data->model_flags & CHARACTER_AI_MODEL_FLAG_SNAP_ON_BIG_JUMP) != 0) {
            object->start_position = *destination;
            object->velocity = v000;
            object->position = *destination;
            ResetPlayerMoves(game_object);
            object->respawn_timer = 0.0f;
        } else {
            StartBigJump(game_object, destination, 0, 0.5f, 1.0f, 0, 0);
        }
    } else {
        SnapCreaturePos(game_object, destination, 0, NULL, 0);
    }

    return 1;
}

static u32 AIRespawnOnPath(APIOBJECT *object) {
    if (object->field_0x287 != 0 || (object->ai->path_info.flags & AI_RESPAWN_FLAG_DISABLED) != 0 ||
        (object->flags_high & APIOBJECT_HIGH_FLAG_RESPAWN_ENABLED) == 0) {
        return 0;
    }

    const u32 model_flags = object->character_data->model_flags;
    if ((model_flags & CHARACTER_AI_MODEL_FLAG_DISABLE_RESPAWN) != 0) {
        return 0;
    }

    GameObject_s *game_object = object->objptr;
    if (game_object->field_0x7a5 == AI_GAME_OBJECT_TYPE_VEHICLE || game_object->field_0xcc0 != NULL) {
        return 0;
    }

    if (object->respawn_timer > AI_RESPAWN_DELAY) {
        if ((model_flags & CHARACTER_AI_MODEL_FLAG_SNAP_ON_BIG_JUMP) == 0) {
            StartBigJump(game_object, &object->respawn_position, 0, 0.5f, 1.0f, 0, 0);
        } else {
            object->start_position = object->respawn_position;
            object->position = object->respawn_position;
            object->velocity = v000;
            ResetPlayerMoves(game_object);
            object->respawn_timer = 0.0f;
        }
    }

    return 0;
}

static i32 SpecialRouteCharacterTypeID(char *name) {
    if (NuStrICmp(name, "Everyone") == 0) {
        return 0x40;
    }

    for (i32 suit_index = 0; suit_index < 10; ++suit_index) {
        if (NuStrICmp(name, Suit[suit_index].suit_character_name) == 0) {
            return suit_index;
        }
    }

    i32 route_index = 0;
    i32 skipped_count = 0;
    if (CurrentStoryCList != NULL) {
        while (CurrentStoryCList[route_index].model_id != -1 && route_index < 64) {
            char *character_name = apicharsys->char_data[CurrentStoryCList[route_index + skipped_count].model_id].file;
            bool skip = false;
            for (i32 skip_index = 0; skip_chars[skip_index] != NULL; ++skip_index) {
                if (NuStrICmp(character_name, skip_chars[skip_index]) == 0) {
                    skip = true;
                    break;
                }
            }
            if (skip) {
                ++skipped_count;
                continue;
            }
            if (NuStrICmp(name, apicharsys->char_data[CurrentStoryCList[route_index + skipped_count].model_id].file) ==
                0) {
                return route_index + 10;
            }
            ++route_index;
        }
    }
    return -1;
}

void AlertSurroundingCreatures(GameObject_s *object, nuvec_s *position) {
    if (object != NULL &&
        ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001 || (object->apiobj.field_0x1f4 & 5) != 0 ||
         (static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_090 & 0x8000) != 0))
        return;
    alert_obj = object;
    alert_pos = *position;
    alert_timer = alert_time;
}

void GameCreatureOpponentSelection(AISYS_s *system, i32 count, APIOBJECT_s **objects, i32 goody_count,
                                   APIOBJECT_s **goodies, i32 baddy_count, APIOBJECT_s **baddies, u64 awareness,
                                   float) {
    NUVEC difference = {0.0f, 0.0f, 0.0f};
    if (system != NULL) {
        if (goody_count == 0)
            return;
        f32 distance = 0.0f;
        i32 complete = baddy_count == 0;
        if (alert_obj != NULL && (alert_obj->apiobj.flags_high & 0x10) != 0 &&
            (alert_obj->apiobj.field_0x1f4 & 5) == 0 &&
            (static_cast<GAMECHARACTERDATA *>(alert_obj->apiobj.character_data->field11_0x24)->flags_090 & 0x8000) ==
                0) {
            const u64 alert_mask = (u64)1 << alert_obj->apiobj.field_0x289;
            for (i32 i = 0; i < baddy_count; ++i) {
                APIOBJECT_s *object = baddies[i];
                if ((object->ai_awareness_mask & alert_mask) != 0) {
                    object->objptr->alert_target_timer = 5.0f;
                    object->objptr->alert_target = &alert_obj->apiobj;
                } else if (WORLD->rooms_visible_ptr[object->objptr->room_id] != 0) {
                    difference.x = alert_pos.x - object->collision_position.x;
                    if (object->heardistance > difference.x) {
                        difference.z = alert_pos.z - object->collision_position.z;
                        if (object->heardistance > difference.z) {
                            difference.y = alert_pos.y - object->collision_position.y;
                            if (object->maxviewheight > difference.y && difference.y > object->minviewheight) {
                                distance = difference.x * difference.x + difference.z * difference.z;
                                if (!(object->heardistance * object->heardistance > distance))
                                    continue;
                                if ((WORLD->api_object_sys->line_of_sight[object->field_0x289] & alert_mask) == 0) {
                                    const f32 dx =
                                        alert_obj->apiobj.collision_position.x - object->collision_position.x;
                                    const f32 dz =
                                        alert_obj->apiobj.collision_position.z - object->collision_position.z;
                                    if (!(dx * dx + dz * dz > object->viewdistance * object->viewdistance))
                                        continue;
                                }
                                object->objptr->alert_target = &alert_obj->apiobj;
                                object->ai_awareness_mask |= alert_mask;
                                awareness |= object->ai_awareness_mask;
                                object->objptr->alert_target_timer = 5.0f;
                            }
                        }
                    }
                }
            }
            alert_timer -= FRAMETIME;
            if (alert_timer <= 0.0f)
                alert_obj = NULL;
        }
        if (system->goody_idx >= goody_count)
            system->goody_idx = 0;
        i32 budget = 32;
        // Pending choices survive budget exhaustion. Publish only after a pass
        // finishes without changing the baddies' assignments.
        while (!complete && budget > 0) {
            complete = 0;
            while (system->goody_idx < goody_count && budget > 0) {
                APIOBJECT_s *goody = goodies[system->goody_idx];
                f32 best_distance = 1.0e9f;
                APIOBJECT_s *best_baddy = NULL;
                for (i32 i = 0; i < baddy_count; ++i) {
                    APIOBJECT_s *baddy = baddies[i];
                    if (baddy == goody || baddy->ai->pending_opponent == goody)
                        continue;
                    i32 have_distance = 0;
                    if (baddy->objptr->cannot_be_seen) {
                        goody->ai_awareness_mask &= ~((u64)1 << baddy->field_0x289);
                    } else if ((goody->ai_awareness_mask >> baddy->field_0x289) & 1) {
                        distance = NuVecDist(&baddy->position, &goody->position, &difference);
                        have_distance = 1;
                        if (distance >= goody->viewdistance + baddy->visibility_range_extension)
                            goody->ai_awareness_mask &= ~((u64)1 << baddy->field_0x289);
                    } else if ((WORLD->api_object_sys->line_of_sight[goody->field_0x289] >> baddy->field_0x289) & 1) {
                        distance = NuVecDist(&baddy->position, &goody->position, &difference);
                        have_distance = 1;
                        if ((!party_under_cover || ((goody->field_0x1f4 & 0x400) && (baddy->field_0x1f4 & 0x400))) &&
                            (((awareness >> baddy->field_0x289) & 1) ||
                             (((WORLD->api_object_sys->line_of_sight[goody->field_0x289] >> baddy->field_0x289) & 1) &&
                              ((goody->flags_high & 8) ||
                               difference.x * goody->objptr->facing_direction.x +
                                       difference.z * goody->objptr->facing_direction.z >=
                                   0.0f ||
                               (goody->heardistance > distance && (baddy->objptr->field_0xef9 & 8)))))) {
                            goody->ai_awareness_mask |= (u64)1 << baddy->field_0x289;
                            awareness |= goody->ai_awareness_mask;
                        }
                    }
                    if (goody->objptr->cannot_be_seen) {
                        baddy->ai_awareness_mask &= ~((u64)1 << goody->field_0x289);
                    } else if (baddy->objptr->alert_target == goody) {
                        baddy->ai_awareness_mask |= (u64)1 << goody->field_0x289;
                        awareness |= baddy->ai_awareness_mask;
                    } else if ((baddy->ai_awareness_mask >> goody->field_0x289) & 1) {
                        if (!have_distance)
                            distance = NuVecDist(&baddy->position, &goody->position, &difference);
                        have_distance = 1;
                        if (distance >= baddy->viewdistance + goody->visibility_range_extension)
                            baddy->ai_awareness_mask &= ~((u64)1 << goody->field_0x289);
                    } else if ((WORLD->api_object_sys->line_of_sight[baddy->field_0x289] >> goody->field_0x289) & 1) {
                        if (!have_distance)
                            distance = NuVecDist(&baddy->position, &goody->position, &difference);
                        have_distance = 1;
                        if ((!party_under_cover || ((baddy->field_0x1f4 & 0x400) && (goody->field_0x1f4 & 0x400))) &&
                            (((awareness >> goody->field_0x289) & 1) ||
                             (((WORLD->api_object_sys->line_of_sight[baddy->field_0x289] >> goody->field_0x289) & 1) &&
                              ((baddy->flags_high & 8) ||
                               -difference.z * baddy->objptr->facing_direction.z -
                                       difference.x * baddy->objptr->facing_direction.x >=
                                   0.0f ||
                               (baddy->heardistance > distance && (goody->objptr->field_0xef9 & 8)))))) {
                            baddy->ai_awareness_mask |= (u64)1 << goody->field_0x289;
                            awareness |= baddy->ai_awareness_mask;
                        }
                    }
                    if (!((goody->ai_awareness_mask >> baddy->field_0x289) & 1) &&
                        !((baddy->ai_awareness_mask >> goody->field_0x289) & 1))
                        continue;
                    if (!have_distance)
                        distance = NuVecDist(&baddy->position, &goody->position, &difference);
                    GameObject_s *baddy_object = baddy->objptr;
                    i16 baddy_id = baddy_object->id;
                    if (baddy_id != id_UGNAUGHT && baddy_id != id_JAWA) {
                        if (!(goody->character_data->model_flags & 0x80000)) {
                            if (baddy_object->only_attack_players && goody->objptr != player &&
                                goody->objptr != player2) {
                                baddy_object->ai_opponent_exclusion_mask |= (u64)1 << goody->field_0x289;
                            } else if (goody->objptr->id == id_DRAGBOMB && baddy_id != id_ATAT) {
                                baddy_object->ai_opponent_exclusion_mask |= (u64)1 << goody->field_0x289;
                            }
                        } else {
                            baddy_object->ai_opponent_exclusion_mask |= (u64)1 << goody->field_0x289;
                        }
                    } else {
                        if (!ZapTarget(goody->objptr))
                            baddy->objptr->ai_opponent_exclusion_mask |= (u64)1 << goody->field_0x289;
                        baddy_object = baddy->objptr;
                        baddy_id = baddy_object->id;
                    }
                    GameObject_s *goody_object = goody->objptr;
                    if (!((baddy_object->ai_opponent_exclusion_mask >> goody->field_0x289) & 1)) {
                        AIPACKET *packet = baddy->ai;
                        if (packet->pending_nearest_metric > distance) {
                            packet->pending_nearest_metric = distance;
                            packet->pending_nearest_opponent = goody;
                        }
                        f32 metric = distance;
                        if (packet->opponent_object == goody)
                            metric -= 0.5f;
                        if ((baddy->ai_awareness_mask >> goody->field_0x289) & 1) {
                            if (packet->pending_opponent == NULL) {
                                if (!(packet->runtime_flags & 2) ||
                                    ((WORLD->api_object_sys->line_of_sight[baddy->field_0x289] >> goody->field_0x289) &
                                     1) ||
                                    baddy_object->alert_target == goody) {
                                    if (best_distance > metric) {
                                        best_distance = distance;
                                        best_baddy = baddy;
                                    }
                                }
                            } else if (goody->player_controlled && baddy_object->prefers_players) {
                                if (!packet->pending_opponent->player_controlled ||
                                    packet->pending_opponent_metric > metric) {
                                    best_distance = distance;
                                    best_baddy = baddy;
                                }
                            }
                        }
                    }
                    if (baddy_id == id_JAWA || baddy_id == id_UGNAUGHT ||
                        (baddy->character_data->model_flags & 0x80000))
                        goody_object->ai_opponent_exclusion_mask |= (u64)1 << baddy->field_0x289;
                    if (!((goody_object->ai_opponent_exclusion_mask >> baddy->field_0x289) & 1)) {
                        AIPACKET *packet = goody->ai;
                        if (packet->pending_nearest_metric > distance) {
                            packet->pending_nearest_metric = distance;
                            packet->pending_nearest_opponent = baddy;
                        }
                        f32 metric = distance;
                        if (packet->opponent_object == baddy)
                            metric -= 0.5f;
                        if (((goody->ai_awareness_mask >> baddy->field_0x289) & 1) &&
                            (!(packet->runtime_flags & 2) ||
                             ((WORLD->api_object_sys->line_of_sight[goody->field_0x289] >> baddy->field_0x289) & 1))) {
                            if (packet->pending_opponent_metric > metric ||
                                (packet->pending_opponent != NULL && packet->pending_opponent->objptr != NULL &&
                                 packet->pending_opponent->objptr->character_context == 0x5a &&
                                 baddy_object->character_context != 0x5a)) {
                                packet->pending_opponent_metric = distance;
                                packet->pending_opponent = baddy;
                            }
                        }
                    }
                }
                if (baddy_count > 0) {
                    budget -= baddy_count;
                    if (best_baddy != NULL && best_baddy->ai->pending_opponent != goody) {
                        best_baddy->ai->pending_opponent = goody;
                        best_baddy->ai->pending_opponent_metric = best_distance;
                        system->unknown_flag_2 = 1;
                        if (!((goody->objptr->ai_opponent_exclusion_mask >> best_baddy->field_0x289) & 1) &&
                            ((goody->ai_awareness_mask >> best_baddy->field_0x289) & 1)) {
                            if (!(goody->ai->field_0x1e5 & 0x10) ||
                                goody->ai->pending_opponent_metric > best_distance) {
                                goody->ai->pending_opponent = best_baddy;
                                goody->ai->pending_opponent_metric = best_distance;
                                goody->ai->field_0x1e5 |= 0x10;
                            }
                        }
                    }
                }
                ++system->goody_idx;
                if (system->goody_idx >= goody_count) {
                    system->goody_idx = 0;
                    if (system->unknown_flag_2) {
                        system->unknown_flag_2 = 0;
                        complete = 0;
                    } else {
                        complete = 1;
                    }
                }
            }
        }
        if (!complete)
            return;
        system->goody_idx = 0;
        nbaddies_can_see_players = 0;
        // Losing LOS alone does not erase the seen history; losing awareness does.
        for (i32 i = 0; i < goody_count; ++i) {
            APIOBJECT_s *goody = goodies[i];
            for (i32 j = 0; j < baddy_count; ++j) {
                APIOBJECT_s *baddy = baddies[j];
                if (!((goody->ai_awareness_mask >> baddy->field_0x289) & 1)) {
                    if ((goody->objptr->ai_seen_mask >> baddy->field_0x289) & 1) {
                        goody->objptr->ai_seen_mask &= ~((u64)1 << baddy->field_0x289);
                    }
                } else {
                    if ((WORLD->api_object_sys->line_of_sight[goody->field_0x289] >> baddy->field_0x289) & 1)
                        goody->objptr->ai_seen_mask |= (u64)1 << baddy->field_0x289;
                }
                if ((baddy->ai_awareness_mask >> goody->field_0x289) & 1) {
                    if ((WORLD->api_object_sys->line_of_sight[baddy->field_0x289] >> goody->field_0x289) & 1)
                        baddy->objptr->ai_seen_mask |= (u64)1 << goody->field_0x289;
                } else if ((baddy->objptr->ai_seen_mask >> goody->field_0x289) & 1) {
                    baddy->objptr->ai_seen_mask &= ~((u64)1 << goody->field_0x289);
                }
            }
        }
        for (i32 i = 0; i < count; ++i) {
            APIOBJECT_s *object = objects[i];
            AIPACKET *packet = object->ai;
            packet->opponent_metric = packet->pending_opponent_metric;
            packet->opponent_object = packet->pending_opponent;
            packet->opponent_is_threat = packet->field_0x1e5_bit4;
            packet = object->ai;
            packet->nearest_opponent_metric = packet->pending_nearest_metric;
            packet->nearest_opponent_object = packet->pending_nearest_opponent;
            packet->pending_opponent_metric = 1.0e9f;
            packet->pending_nearest_metric = 1.0e9f;
            packet->field_0x1e5 &= ~0x10;
            packet->pending_nearest_opponent = NULL;
            packet->pending_opponent = NULL;
            if ((awareness >> object->field_0x289) & 1)
                object->ai->field_0x1e5 |= 0x40;
            else
                object->ai->field_0x1e5 &= ~0x40;
            object->objptr->field_0xef9 &= ~8;
            GameObject_s *game_object = object->objptr;
            game_object->ai_opponent_exclusion_mask = 0;
            if ((player != NULL && ((game_object->ai_seen_mask >> player->apiobj.field_0x289) & 1)) ||
                (player2 != NULL && ((game_object->ai_seen_mask >> player2->apiobj.field_0x289) & 1)))
                ++nbaddies_can_see_players;
            if (game_object->opponent != NULL) {
                packet = object->ai;
                packet->opponent_object = static_cast<APIOBJECT_s *>(game_object->opponent);
                packet->opponent_metric = NuVecDist(&packet->opponent_object->position, &object->position, &difference);
                packet = object->ai;
                if (packet->opponent_object->ai->opponent_object == object)
                    packet->field_0x1e5 |= 8;
                else
                    packet->field_0x1e5 &= ~8;
            }
        }
    }
}

void MakeBaddiesForgetAboutParty(i32 check_hostility) {
    GameObject_s *baddies[64];
    GameObject_s *party[64];
    i32 party_count = 0;
    GameObject_s *objects = Obj;
    i32 count = HIGHGAMEOBJECT;
    GameObject_s **next_baddie = baddies;
    GameObject_s **next_party = party;
    GameObject_s *object = objects;
    i32 baddie_count = 0;
    for (i32 i = 0; i < count; ++i, ++object) {
        if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001 || object->apiobj.field_0x287 != 0)
            continue;
        if ((object->apiobj.field_0x1f4 & 1) != 0) {
            *next_party++ = object;
            party_count++;
        } else if ((object->apiobj.field_0x1f4 & 4) == 0) {
            *next_baddie++ = object;
            baddie_count++;
        }
    }
    if (check_hostility != 0) {
        for (i32 i = 0; i < baddie_count; ++i) {
            for (i32 j = 0; j < party_count; ++j) {
                u64 mask = u64(1) << baddies[i]->apiobj.field_0x289;
                APIOBJECT *object = &party[j]->apiobj;
                u64 hostile = u64(object->field387_0x2a0) | (u64(object->field388_0x2a4) << 32);
                if ((hostile & mask) != 0) {
                    APIOBJECTSYS_s *api_object_sys = WORLD->api_object_sys;
                    const u8 slot = object->field_0x289;
                    if (((u64(api_object_sys->hostility_masks[slot][0]) |
                          (u64(api_object_sys->hostility_masks[slot][1]) << 32)) &
                         mask) != 0)
                        return;
                }
            }
        }
    }
    for (i32 i = 0; i < baddie_count; ++i) {
        APIOBJECT *baddie = &baddies[i]->apiobj;
        for (i32 j = 0; j < party_count; ++j) {
            u64 mask = ~(u64(1) << baddie->field_0x289);
            APIOBJECT *member = &party[j]->apiobj;
            member->field387_0x2a0 &= static_cast<u32>(mask);
            member->field388_0x2a4 &= static_cast<u32>(mask >> 32);
            u64 other_mask = ~(u64(1) << member->field_0x289);
            baddie->field387_0x2a0 &= static_cast<u32>(other_mask);
            baddie->field388_0x2a4 &= static_cast<u32>(other_mask >> 32);
        }
    }
    object = objects;
    for (i32 i = 0; i < count; ++i, ++object) {
        if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001 || object->apiobj.field_0x287 != 0)
            continue;
        object->ai.field_0x1e5 &= ~0x58;
        object->ai.opponent = NULL;
        object->ai.opponent_metric = 1000000000.0f;
        object->ai.nearest_opponent = NULL;
        object->ai.field_0xdc = 0;
        object->ai.nearest_opponent_metric = 1000000000.0f;
        object->ai.target_metric_e0 = 1000000000.0f;
        object->ai.field_0xec = 0;
        object->ai.target_metric_f0 = 1000000000.0f;
        object->apiobj.field387_0x2a0 = 0;
        object->apiobj.field388_0x2a4 = 0;
        object->field_0xebc = 0;
        object->field_0xec0 = 0;
        object->field_0xecc = 0;
    }
    party_under_cover = 0;
    alert_timer = 0.0f;
    alert_obj = NULL;
    if (VADER_ADATA != NULL && WORLD->area == VADER_ADATA)
        return;
    if (party_cant_be_under_cover != 0)
        return;
    i32 all_under_cover = 1;
    for (i32 i = 0; i < 8; ++i) {
        GameObject_s *player = Player[i];
        if (player == NULL || (player->apiobj.field_0x1f8 & 0x1001) != 0x1001 || player->apiobj.field_0x287 != 0 ||
            player->not_with_party)
            continue;
        if (player->field_0x108e == 5 ||
            ((FreePlay != 0 || WORLD->current_level == HUB_LDATA) &&
             (player->apiobj.character_data->model_flags & 0x204) != 0 && player->field_0xcc0 == NULL)) {
            party_under_cover = 1;
        }
        if ((player->apiobj.character_data->model_flags & 0x80000) == 0)
            all_under_cover = 0;
    }
    if (all_under_cover != 0)
        party_under_cover = 1;
}

void GameAttackInit() {
}

i32 CanFightLikeAJedi(GameObject_s *object) {
    return CharCategory_IsCategory(object, 0) != 0 || object->id == id_GRIEVOUS || object->id == id_BODYGUARD ||
           object->id == id_IMPERIALGUARD;
}

void GameAISysStartFrame(AISYS_s *system) {
    if (system == NULL || netclient != 0) {
        return;
    }

    if (system->path_sys != NULL && system->path_sys->path_count != 0) {
        for (i32 index = 0; index < system->path_sys->path_count; ++index) {
            memset(&system->path_sys->paths[index]->updated_node_bits[0], 0, 0x20);
            memmove(system->path_sys->paths[index]->previous_inside_node_bits,
                    system->path_sys->paths[index]->inside_node_bits, 0x20);
            memset(system->path_sys->paths[index]->inside_node_bits, 0,
                   sizeof(system->path_sys->paths[index]->inside_node_bits));
        }

        // Moving specials can carry path endpoints. Update both ends of each
        // connection; AIPathNodeUpdatePos uses updated_node_bits to ensure a
        // shared endpoint is transformed only once this frame.
        for (i32 path_index = 0; path_index < system->path_sys->path_count; ++path_index) {
            AIPATH *path = system->path_sys->paths[path_index];
            for (u32 connection_index = 0; connection_index < path->connection_count; ++connection_index) {
                AIPATHCNX *connection = &path->connections[connection_index];
                AIPATHNODE *node = &path->nodes[connection->direction_a];
                if (node->has_special != 0) {
                    AIPathNodeUpdatePos(system, path, node);
                }
                path = system->path_sys->paths[path_index];
                node = &path->nodes[connection->direction_b];
                if (node->has_special != 0) {
                    AIPathNodeUpdatePos(system, path, node);
                    path = system->path_sys->paths[path_index];
                }
            }
        }
    }

    // Spread the object/area overlap work across frames. Area runtime flags
    // summarize the kinds of live characters inside it, while every object
    // retains a 64-area occupancy mask for script queries.
    if (__builtin_expect(system->next_area_check < system->area_count, 0)) {
        AIAREA *area = &system->areas[system->next_area_check];
        const i32 area_index = static_cast<i32>(area - WORLD->ai_sys->areas);
        area->runtime_flags &= static_cast<u8>(
            ~(AIAREA_RUNTIME_PLAYER_PRESENT | AIAREA_RUNTIME_OBJECT_STATE_CLEAR | AIAREA_RUNTIME_OBJECT_STATE_SET));

        GameObject_s *object = Obj;
        for (i32 object_index = 0; object_index < HIGHGAMEOBJECT; ++object_index, ++object) {
            if ((object->apiobj.field_0x1f8 & (APIOBJECT_FLAG_CHARACTER | APIOBJECT_FLAG_IN_USE)) !=
                (APIOBJECT_FLAG_CHARACTER | APIOBJECT_FLAG_IN_USE)) {
                continue;
            }
            if (object->apiobj.field_0x287 != 0 && !(object->field_0x101c > 0.0f) &&
                !object->apiobj.player_controlled) {
                continue;
            }

            NUVEC local_position;
            NuVecSub(&local_position, &object->apiobj.position, &area->position);
            NuVecRotateY(&local_position, &local_position, -area->rotation);
            const bool is_inside = local_position.x >= -area->half_width && local_position.y >= -0.1f &&
                                   local_position.z >= -area->half_depth && local_position.x <= area->half_width &&
                                   local_position.y <= area->height && local_position.z <= area->half_depth;
            if (!is_inside) {
                // Outside removal addresses the full 64-bit area bit.
                const u64 area_bit = 1ULL << area_index;
                object->apiobj.ai_area_mask_low &= ~static_cast<u32>(area_bit);
                object->apiobj.ai_area_mask_high &= ~static_cast<u32>(area_bit >> 32);
                continue;
            }

            // Inside occupancy uses the signed, wrapping 32-bit area bit.
            const i64 inside_area_bit = static_cast<i32>(1u << (area_index & 31));
            object->apiobj.ai_area_mask_low |= static_cast<u32>(inside_area_bit);
            object->apiobj.ai_area_mask_high |= static_cast<u32>(inside_area_bit >> 32);
            if ((object->apiobj.flags_low & APIOBJECT_FLAG_PLAYER_ACTIVE) != 0) {
                area->runtime_flags |= AIAREA_RUNTIME_PLAYER_PRESENT;
            }
            if (object->apiobj.field_0x27c != -1) {
                area->runtime_flags |= AIAREA_RUNTIME_CHARACTER_SLOT_SEEN;
            }
            if ((object->apiobj.field_0x1f4 & 1) != 0) {
                area->runtime_flags |= AIAREA_RUNTIME_OBJECT_STATE_SET;
            } else if ((object->apiobj.field_0x1f4 & 4) == 0) {
                area->runtime_flags |= AIAREA_RUNTIME_OBJECT_STATE_CLEAR;
            }
        }

        ++system->next_area_check;
        if (__builtin_expect(system->next_area_check >= system->area_count, 0)) {
            system->next_area_check = 0;
        }
    }

    // Level scripts are ordinary AI processors without an object or packet.
    // Disabled processors retain their state but do not advance this frame.
    for (i32 index = 0; index < WORLD->processor_count; ++index) {
        if (!WORLD->processors[index].processor.is_disabled) {
            AIScriptProcess(system, NULL, NULL, &WORLD->processors[index].processor, FRAMETIME);
        }
    }
}

i32 CheckPosAIArea(AIAREA_s *area, nuvec_s *position, float tolerance) {
    if (position == NULL || area == NULL) {
        return 0;
    }

    NUVEC local_position;
    NuVecSub(&local_position, position, &area->position);
    NuVecRotateY(&local_position, &local_position, -area->rotation);

    return local_position.x + tolerance >= -area->half_width && local_position.y + tolerance >= -0.1f &&
           local_position.z + tolerance >= -area->half_depth && local_position.x - tolerance <= area->half_width &&
           local_position.y - tolerance <= area->height && local_position.z - tolerance <= area->half_depth;
}

void GameAISysSetGame() {
    AIPathCnxHelperSysInitFn = NULL;
    StarWars_GameAISysInit();
}

void GameAISysInit() {
    RegisterAIScriptActions(lego_aiactiondefs);
    RegisterAIScriptConditions(lego_aiconditiondefs);
    InitFn_LevelCharacterTypeID(LevelCharacterTypeID);
    InitFn_SpecialRouteCharacterTypeID(SpecialRouteCharacterTypeID);
    InitFn_LevelCharacterName(LevelCharacterName);
    InitFn_SpecialRouteCharacterName(SpecialRouteCharacterName);
    InitFn_LevelCharacterGlobalID(LevelCharacterGlobalID);
    InitFn_GlobalCharacterTypeID(GlobalCharacterTypeID);
    InitFn_GlobalCharacterName(GlobalCharacterName);
    InitFn_GlobalCharacterRender(GlobalCharacterRender);
    InitFn_GlobalCharacterHGobj(GlobalCharacterHGobj);
    InitFn_ClearAICreatures(ClearAICreatures);
    InitFn_GetCharacterGoalSpeedFn(GetCharacterGoalSpeed);
    InitFn_GetViewRange(GetViewRange);
    InitFn_GetHearDistance(GetHearDistance);
    InitFn_GlobalGetMaxViewHeight(GetMaxViewHeight);
    InitFn_GlobalGetMinViewHeight(GetMinViewHeight);
    InitFn_GameAILoad(GameAILoad);
    InitFn_AIActionParseSpeed(GameAIActionParseSpeed);
    InitFn_AIRespawnOnPath(AIRespawnOnPath);
    InitFn_AIBigJumpToDestination(AIBigJumpToDestination);
    InitFn_FindAlternativeSpecialObjectFn(GameFindAlternativeSpecialObject);
    InitFn_APIOBJECTFromObjIDFn(GameAPIOBJECTFromObjID);
    InitFn_GetNamedAPIObject(GetNamedAPIObject);
    InitFn_GetAICreatureOrigin(GetAICreatureOrigin);

    default_path_heighttol = 0.2f;
    default_activate_difficulty = AI_DEFAULT_ACTIVATE_DIFFICULTY;
    default_min_n_respawns = 0;
    default_max_n_respawns = 0;
    default_min_t_respawn = 0.0f;
    default_max_t_respawn = 0.0f;

    SetScriptErrorLevel(SCRIPT_ERROR_LEVEL_WARNING);
    GameAISysSetGame();
}

i32 TryToTeleportToNextNode(GameObject_s *object, AIPATHNODE_s *node, i32 tightrope) {
    if ((object->apiobj.field_0x1f4 & 0x400) != 0 || !object->ai.path_info.on_path) {
        return 0;
    }
    if (!(object->field_0xf1c >= OFFSCREEN_CATCHUP_TIME) &&
        (object->apiobj.model_draw_result != 0 || (object->tag_context_flags & 2) != 0 ||
         !(drop_back_in_timer > 0.0f))) {
        return 0;
    }
    NUVEC position = node->position;
    position.y += object->apiobj.field_0x1e0;
    if (NuCameraClipTestSphere(&position, object->apiobj.field_0x1e0, &numtx_identity) == 0) {
        return 0;
    }
    if (tightrope != 0) {
        if (TightRope_SnapTo(object, &node->position) == 0) {
            return 0;
        }
        object->field_0x109c = 0;
        object->field_0x1092 = 0;
        object->field_0x1093 = 0;
        object->ai.path_connection_state = 0;
        return 1;
    }
    object->apiobj.initial_position = node->position;
    object->apiobj.collision_position = object->apiobj.initial_position;
    object->apiobj.start_position = object->apiobj.initial_position;
    object->ai.owner->apiobj.position = object->apiobj.initial_position;
    plr_lastpos = object->apiobj.initial_position;
    object->apiobj.velocity = v000;
    InitSurfaceInfo(object);
    SetObjOnSurface(object, 0);
    object->field_0x1092 = 0;
    object->field_0x1093 = 0;
    object->field_0x109c = 0;
    object->ai.path_connection_state = 0;
    if (object->field_0xcc0 == NULL) {
        Player_ClearContext(object, 0);
        Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    }
    return 1;
}

void SetSpecialMove(GameObject_s *object, AIPATHNODE_s *target, AIPATHNODE_s *node, char move) {
    object->ai.field_0x180 = target;
    object->field_0x1092 = move;
    object->field_0x1093 = 0;
    object->special_move_node = node;
    object->special_move_progress = 0.0f;
}

void ClearSpecialMove(GameObject_s *object) {
    object->special_move_progress = 1.0f;
    object->ai.field_0x180 = NULL;
    object->field_0x1092 = 0;
    object->field_0x1093 = 0;
    object->special_move_node = NULL;
    object->field_0x1080 = 0.0f;
    object->field_0x107f = 0;
}

void AIPathCnxControlSysReset(AIPATHCNXCONTROLSYS_s *system) {
    if (system == NULL) {
        return;
    }

    NULISTHDR *available = &system->available_controllers;
    available->head = NULL;
    available->tail = NULL;
    system->active_controllers.head = NULL;
    system->active_controllers.tail = NULL;
    memset(system->controllers, 0, system->controller_count * sizeof(*system->controllers));
    for (i32 i = 0; i < system->controller_count; ++i) {
        NuLinkedListAppend(available, &system->controllers[i].links);
    }
}

void *AIPathCnxControlSysCreate(VARIPTR *buf, VARIPTR *buf_end, i32 count) {
    AIPATHCNXCONTROLSYS_s *system =
        static_cast<AIPATHCNXCONTROLSYS_s *>(AISysBufferAlloc(buf, buf_end, sizeof(AIPATHCNXCONTROLSYS_s)));
    if (system != NULL) {
        memset(system, 0, sizeof(*system));
        system->controllers = static_cast<AIPATHCNXCONTROLLER_s *>(
            AISysBufferAlloc(buf, buf_end, static_cast<u32>(count) * sizeof(AIPATHCNXCONTROLLER_s)));
        if (system->controllers != NULL) {
            system->controller_count = count;
            AIPathCnxControlSysReset(system);
        }
    }
    return system;
}

void GameAISysReset(AISYS_s *system) {
    if (system == NULL) {
        return;
    }

    AISysSetLevelPath(system, NULL);

    WORLD->processor_count = 0;
    for (i32 script_index = 0; script_index < 32; ++script_index) {
        char script_name[16];
        if (script_index != 0) {
            sprintf(script_name, "Level%d", script_index);
        } else {
            sprintf(script_name, "Level");
        }

        if (AIScriptFind(WORLD->ai_sys, script_name, 0, 1, 0) == NULL) {
            continue;
        }

        AIScriptProcessorInit(system, NULL, &WORLD->processors[WORLD->processor_count].processor, NULL, script_name,
                              NULL, 0, NULL, NULL);
        NuStrCpy(WORLD->processors[WORLD->processor_count].name, script_name);
        LevelScriptReStoreProgress(WORLD, &WORLD->processors[WORLD->processor_count]);
        ++WORLD->processor_count;
    }

    AIPATHSYS *path_system = system->path_sys;
    if (path_system != NULL) {
        for (i32 path_index = 0; path_index < path_system->path_count; ++path_index) {
            AIPATH *path = path_system->paths[path_index];
            AIPATHCNX *connection = path->connections;
            auto path_connection_count = path->connection_count;
            for (i32 connection_index = 0; connection_index < path_connection_count; ++connection_index, ++connection) {
                connection->node_a = connection->previous_node_a;
                connection->node_b = connection->previous_node_b;

                if (LEGO_AIPATHCNX_BLOCKAGE != 0) {
                    connection->node_a &= ~LEGO_AIPATHCNX_BLOCKAGE;
                    connection->node_b &= ~LEGO_AIPATHCNX_BLOCKAGE;
                }
            }
        }
    }

    AIPathCnxControlSysReset(WORLD->ai_path_cnx_control_sys);
    AIPathCnxHelperSysReset(WORLD, WORLD->ai_path_cnx_helper_sys);
    InitAICreatures(system);
    ResetAICreatures(system);

    if (WORLD->processor_count != 0) {
        GizmoSysAddGizmos(WORLD->gizmo_sys, WORLD->giz_flow, WORLD);
    }
}

AIPATHCNXCONTROLLER_s *AIPathCnxControllerCreate(AIPATHCNXCONTROLSYS_s *control_system, AISYS_s *ai_system,
                                                 AIPATH_s *path, char *from, char *to, i32 target_type,
                                                 char *target_name, i32 fake_animation_id, i32 gizmo_output) {
    AIPATHCNXCONTROLLER_s *controller = NULL;
    nuhspecial_s special = {};
    if (from != NULL) {
        if (control_system != NULL && to != NULL) {
            i32 direction;
            AIPATHCNX *connection = static_cast<AIPATHCNX *>(AIPAthFindPathCnx(ai_system, path, from, to, &direction));
            void *target = NULL;
            if (connection != NULL) {
                i32 gizmo_type;
                switch (target_type) {
                    case 0:
                        if (target_name != NULL) {
                            NuSpecialFind(ai_system->scene, &special, target_name, 1);
                        }
                        break;
                    case 1:
                        target = CutScene_FindInst(WORLD->cutscene_sys, target_name);
                        break;
                    case 2:
                        target = GizBuildIt_Find(WORLD, target_name);
                        break;
                    case 3:
                        target = GizmoFindByName(WORLD->gizmo_sys, -1, target_name);
                        break;
                    case 4:
                        gizmo_type = blowup_gizmotype_id;
                        goto find_typed_gizmo;
                    case 7:
                        gizmo_type = force_gizmotype_id;
                        goto find_typed_gizmo;
                    case 8:
                        gizmo_type = obstacle_gizmotype_id;
                        goto find_typed_gizmo;
                    case 9: {
                        gizmo_type = zipup_gizmotype_id;
                    find_typed_gizmo:
                        GIZMO *gizmo = GizmoFindByName(WORLD->gizmo_sys, gizmo_type, target_name);
                        target = gizmo != NULL ? gizmo->object : NULL;
                        break;
                    }
                    case 5:
                        if (fake_animation_id != 0) {
                            return NULL;
                        }
                        break;
                    case 6:
                        target = FlowBoxFindByName(WORLD->giz_flow, target_name);
                        break;
                    default:
                        break;
                }
            }
            if (target != NULL || NuSpecialExistsFn(&special) != 0 || target_type == 5) {
                NULISTLNK *node = NuLinkedListGetHead(&control_system->available_controllers);
                if (node != NULL) {
                    NuLinkedListRemove(&control_system->available_controllers, node);
                    NuLinkedListAppend(&control_system->active_controllers, node);

                    controller = reinterpret_cast<AIPATHCNXCONTROLLER_s *>(node);
                    AIPATHSYS *path_system = ai_system->path_sys;
                    const i32 path_count = path_system->path_count;
                    AIPATH **path_cursor = path_system->paths;
                    for (u8 index = 0; index < path_count; ++index, ++path_cursor) {
                        if (*path_cursor == path) {
                            controller->path_index = index;
                            break;
                        }
                    }
                    controller->flags = static_cast<u8>((controller->flags & ~1) | (direction & 1));
                    controller->connection = connection;
                    controller->gizmo_output = gizmo_output;
                    if (target != NULL) {
                        controller->target = target;
                    } else if (NuSpecialExistsFn(&special) != 0) {
                        controller->special = special;
                    } else {
                        controller->fake_animation_id = fake_animation_id;
                    }
                    controller->target_type = static_cast<u8>(target_type);
                }
            }
        }
    }
    return controller;
}

void AIPathCnxControllerDestroy(AIPATHCNXCONTROLSYS_s *system, AIPATHCNXCONTROLLER_s *controller) {
    if (system == NULL || controller == NULL) {
        return;
    }
    NuLinkedListRemove(&system->active_controllers, &controller->links);
    memset(controller, 0, sizeof(*controller));
    NuLinkedListAppend(&system->available_controllers, &controller->links);
}

void AIPathCnxControllerSetOnRange(AIPATHCNXCONTROLLER_s *controller, i32 start_frame, i32 end_frame) {
    if (controller == NULL) {
        return;
    }

    i32 animation_end = 1;
    switch (controller->target_type) {
        case 0: {
            nuinstanim_s *animation = NuSpecialGetInstAnim(&controller->special);
            if (animation != NULL) {
                nuanimdata_s *data = controller->special.scene->instance_animation_data[animation->anim_ix];
                if (data != NULL)
                    animation_end = static_cast<i32>(NuAnimEndFrameOld(data));
            }
            break;
        }
        case 1: {
            u8 *animation = *reinterpret_cast<u8 **>(static_cast<u8 *>(controller->target) + 0x58);
            animation_end = static_cast<i32>(*reinterpret_cast<f32 *>(animation + 8));
            break;
        }
        case 2:
        case 3:
        case 4:
        case 6:
            return;
        case 5:
            animation_end = static_cast<i32>(fakeanimendframe[controller->fake_animation_id]);
            break;
        default:
            animation_end = 1;
            break;
    }
    if (animation_end > 1024) {
        animation_end = 1024;
    }

    if (start_frame == -1) {
        start_frame = animation_end;
    } else if (start_frame == -2) {
        start_frame = animation_end - 1;
    }
    if (end_frame == -1) {
        end_frame = animation_end;
    } else if (end_frame == -2) {
        end_frame = animation_end - 1;
    }
    i32 first = start_frame > 0 ? start_frame - 1 : 0;
    i32 last = end_frame <= animation_end ? end_frame : animation_end;
    for (i32 frame = first; frame < last; ++frame) {
        controller->on_frames[frame >> 5] |= 1u << (frame & 31);
    }
}

void AIPathCnxControlSysUpdate(AIPATHCNXCONTROLSYS_s *system) {
    if (system == NULL)
        return;
    for (NULISTLNK *node = NuLinkedListGetHead(&system->active_controllers); node != NULL;
         node = NuLinkedListGetNext(&system->active_controllers, node)) {
        AIPATHCNXCONTROLLER_s *controller = reinterpret_cast<AIPATHCNXCONTROLLER_s *>(node);
        controller->flags &= ~0x20;
        i32 on = 1;
        switch (controller->target_type) {
            case 0: {
                if ((controller->flags & 4) != 0 && NuSpecialGetVisibilityFn(&controller->special) == NULL) {
                    on = 0;
                    break;
                }
                i32 frame = 0;
                nuinstanim_s *animation = NuSpecialGetInstAnim(&controller->special);
                if (animation != NULL) {
                    nuanimdata_s *data = controller->special.scene->instance_animation_data[animation->anim_ix];
                    if (data != NULL) {
                        float end = NuAnimEndFrameOld(data);
                        i32 current = static_cast<i32>(animation->ltime);
                        if (current > 0) {
                            frame = current < 1024 ? current : 1024;
                            i32 end_frame = static_cast<i32>(end);
                            frame = frame < end_frame ? frame : end_frame;
                            --frame;
                        }
                    }
                }
                if ((controller->on_frames[frame / 32] & (1u << (frame % 32))) == 0)
                    on = 0;
                break;
            }
            case 1: {
                const u8 *instance = static_cast<const u8 *>(controller->target);
                i32 frame = static_cast<i32>(*reinterpret_cast<const f32 *>(instance + 0x90));
                float end = *reinterpret_cast<f32 *>(*reinterpret_cast<u8 *const *>(instance + 0x58) + 8);
                if (frame > 0) {
                    i32 end_frame = static_cast<i32>(end);
                    frame = frame < end_frame ? frame : end_frame;
                    frame = frame < 1024 ? frame : 1024;
                    --frame;
                } else {
                    frame = 0;
                }
                if ((controller->on_frames[frame / 32] & (1u << (frame % 32))) == 0)
                    on = 0;
                break;
            }
            case 2:
                if (static_cast<GIZBUILDIT_s *>(controller->target)->build_state != 2)
                    on = 0;
                break;
            case 3:
                on = GizmoGetOutput(WORLD->gizmo_sys, static_cast<GIZMO *>(controller->target),
                                    controller->gizmo_output, 1) != 0;
                break;
            case 4:
                on = (static_cast<GIZMOBLOWUP_s *>(controller->target)->status_flags & 1) == 0;
                break;
            case 5: {
                float end = fakeanimendframe[controller->fake_animation_id];
                i32 frame = static_cast<i32>(fakeanimframe[controller->fake_animation_id]);
                if (frame > 0) {
                    i32 end_frame = static_cast<i32>(end);
                    frame = frame < end_frame ? frame : end_frame;
                    frame = frame < 1024 ? frame : 1024;
                    --frame;
                } else {
                    frame = 0;
                }
                if ((controller->on_frames[frame / 32] & (1u << (frame % 32))) == 0)
                    on = 0;
                break;
            }
            case 6:
                if ((static_cast<FLOWBOX_s *>(controller->target)->state_flags & 2) == 0)
                    on = 0;
                break;
            case 7:
                if (GizForce_Complete(static_cast<GIZFORCE_s *>(controller->target)) == 0)
                    on = 0;
                break;
            case 8:
                on = static_cast<GIZOBSTACLE_s *>(controller->target)->anim_set->state == GAMEANIMSET_STATE_AT_END;
                break;
            case 9:
                on = (static_cast<ZIPUP *>(controller->target)->flags & (ZIPUP_FLAG_ACTIVE | ZIPUP_FLAG_VISIBLE)) ==
                     (ZIPUP_FLAG_ACTIVE | ZIPUP_FLAG_VISIBLE);
                break;
        }
        if (on) {
            controller->flags |= 0x20;
            controller->connection->open = (controller->flags >> 3) & 1;
            AIPATHCNX *connection = controller->connection;
            connection->traversal_flags[controller->flags & 1] &= ~controller->off_flags;
            connection->traversal_flags[controller->flags & 1] |= controller->on_flags;
            if ((controller->flags & 2) != 0) {
                connection->traversal_flags[(~controller->flags) & 1] &= ~controller->off_flags;
                connection->traversal_flags[(~controller->flags) & 1] |= controller->on_flags;
            }
        } else {
            controller->connection->open = (controller->flags >> 4) & 1;
            AIPATHCNX *connection = controller->connection;
            connection->traversal_flags[controller->flags & 1] &= ~controller->on_flags;
            connection->traversal_flags[controller->flags & 1] |= controller->off_flags;
            if ((controller->flags & 2) != 0) {
                connection->traversal_flags[(~controller->flags) & 1] &= ~controller->on_flags;
                connection->traversal_flags[(~controller->flags) & 1] |= controller->off_flags;
            }
        }
    }
}

i32 oneAtOnce_CanAttack(GameObject_s *object, GameObject_s *opponent) {
    if (object == NULL) {
        return 0;
    }
    if ((object->field_0xf01 & 0x20) == 0) {
        return 1;
    }
    if (object->one_at_once_player == 0xff) {
        return 0;
    }
    return Player[object->one_at_once_player] == opponent;
}

f32 oneAtOnce_GetHoldRange(GameObject_s *object) {
    APIOBJECT *opponent = object->ai.opponent_object;
    const i32 player_index = opponent->field_0x27c;
    if (static_cast<u32>(player_index) > 8) {
        return 0.75f;
    }

    AtOnceAttacker *attackers = AtOnce_attackingPlayer[player_index];
    i32 unassigned_in_row = 0;
    i32 row = 0;
    i32 index = 0;
    GameObject_s *candidate = attackers[index].object;
    while (candidate != NULL && index < 17) {
        if (candidate == object) {
            return static_cast<f32>(row) * AtOnce_RowDist + AtOnce_InitialRowDist;
        }
        unassigned_in_row += candidate->one_at_once_player == 0xff;
        if (unassigned_in_row >= AtOnce_attackersPerRow) {
            ++row;
            unassigned_in_row = 0;
        }
        ++index;
        candidate = attackers[index].object;
    }
    return AtOnce_InitialRowDist;
}

void oneAtOnce_SetNumAttackers(i32 attackers) {
    if (attackers < 0) {
        attackers = 0;
    }
    if (attackers >= 5) {
        attackers = 4;
    }
    AtOnce_maxAttackers = attackers;
}

void oneAtOnce_SetAttackersPerRow(i32 attackers) {
    AtOnce_attackersPerRow = MAX(0, attackers);
}

void oneAtOnce_SetDistPerRow(float distance) {
    AtOnce_RowDist = MAX(0.0f, distance);
}

void oneAtOnce_SetInitDistPerRow(float distance) {
    AtOnce_InitialRowDist = MAX(0.0f, distance);
}

void oneAtOnce_MaintainArray() {
    GameObject_s *previous_attackers[8][4];
    i32 previous_count[8] = {};
    i32 attacker_count[8] = {};

    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
        GameObject_s *object = &Obj[index];
        const u16 character_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER;
        if (__builtin_expect(object == NULL || (object->apiobj.field_0x1f8 & character_flags) != character_flags ||
            object->ai.opponent == NULL || (object->field_0xf01 & 0x20) == 0, 1)) {
            continue;
        }

        if (__builtin_expect(object->one_at_once_player != 0xff, 1)) {
            previous_attackers[object->one_at_once_player][previous_count[object->one_at_once_player]] = object;
            ++previous_count[object->one_at_once_player];
        }
        object->one_at_once_player = 0xff;

        APIOBJECT *opponent_api = static_cast<APIOBJECT *>(object->ai.opponent);
        GameObject_s *opponent = opponent_api != NULL ? opponent_api->objptr : NULL;
        i32 player_index = -1;
        for (i32 player = 0; player < 8; ++player) {
            if (Player[player] != opponent) {
                continue;
            }
            player_index = player;
            break;
        }
        if (player_index < 0) {
            continue;
        }

        const i32 slot = attacker_count[player_index];
        AtOnce_attackingPlayer[player_index][slot].object = object;
        AtOnce_attackingPlayer[player_index][slot].distance =
            NuVecDistSqr(&opponent->apiobj.collision_position, &object->apiobj.collision_position, NULL);
        ++attacker_count[player_index];
    }

    for (i32 player = 0; player < 8; ++player) {
        const i32 count = attacker_count[player];
        AtOnce_attackingPlayer[player][count].object = NULL;
        AtOnce_attackingPlayer[player][count].distance = 1.0e9f;
    }

    for (i32 player = 0; player < 8; ++player) {
        if (Player[player] == NULL) {
            continue;
        }
        for (i32 slot = 0; AtOnce_attackingPlayer[player][slot].object != NULL; ++slot) {
            if (AtOnce_attackingPlayer[player][slot].object == previous_attackers[player][0]) {
                AtOnce_attackingPlayer[player][slot].distance *= 0.75f;
            }
            if (AtOnce_attackingPlayer[player][slot].object == previous_attackers[player][1]) {
                AtOnce_attackingPlayer[player][slot].distance *= 0.75f;
            }
            if (AtOnce_attackingPlayer[player][slot].object == previous_attackers[player][2]) {
                AtOnce_attackingPlayer[player][slot].distance *= 0.75f;
            }
            if (AtOnce_attackingPlayer[player][slot].object == previous_attackers[player][3]) {
                AtOnce_attackingPlayer[player][slot].distance *= 0.75f;
            }
            for (i32 adjacent = 1; adjacent <= attacker_count[player] - slot; ++adjacent) {
                if (AtOnce_attackingPlayer[player][adjacent].distance <
                    AtOnce_attackingPlayer[player][adjacent - 1].distance) {
                    GameObject_s *object = AtOnce_attackingPlayer[player][adjacent - 1].object;
                    const f32 distance = AtOnce_attackingPlayer[player][adjacent - 1].distance;
                    AtOnce_attackingPlayer[player][adjacent - 1].object =
                        AtOnce_attackingPlayer[player][adjacent].object;
                    AtOnce_attackingPlayer[player][adjacent - 1].distance =
                        AtOnce_attackingPlayer[player][adjacent].distance;
                    AtOnce_attackingPlayer[player][adjacent].object = object;
                    AtOnce_attackingPlayer[player][adjacent].distance = distance;
                }
            }
        }

        for (i32 slot = 0; slot < AtOnce_maxAttackers && AtOnce_attackingPlayer[player][slot].object != NULL; ++slot) {
            AtOnce_attackingPlayer[player][slot].object->one_at_once_player = static_cast<u8>(player);
        }
    }
}

void GameAIProcess() {
    if (TimingBarSet == 4)
        TBOPENFN("(Sys)", 4);
    ai_fighting = 0;
    AISysProcess(WORLD->ai_sys, reinterpret_cast<APIOBJECT *>(player), reinterpret_cast<APIOBJECT *>(player2));

    GameObject_s *object = Obj;
    u8 high_flags;
    i32 object_limit = HIGHGAMEOBJECT;
    // Keep the first-loop counter available in edx at the latch and body entry.
    for (i32 index = 0; index < object_limit; ++object, ++index) {
        f32 frame_time = FRAMETIME;
        high_flags = object->apiobj.flags_high;
        if (!__builtin_expect((high_flags & 0x10) != 0 && object->apiobj.field_0x287 == 0, 1)) {
            continue;
        }
        object->ai.capabilities &= ~0x00e00000u;
        if (object->apiobj.player_controlled && TouchHacks::TouchControlsActive) {
            if ((object->ai.capabilities & 1) != 0)
                object->ai.capabilities |= 0x00200000;
            if ((object->ai.capabilities & 2) != 0)
                object->ai.capabilities |= 0x00400000;
            if ((object->ai.capabilities & 0x20) != 0)
                object->ai.capabilities |= 0x00800000;
        }
        object->field_0xefd &= 0x7f;
        object->field_0xf01 &= 0x3f;
        object->field_0xf02 &= 0xfc;
        object->field_0x107e = 0;
        if (__builtin_expect(object->use_action_frames != 0, 1)) {
            --object->use_action_frames;
        } else if (object->use_action_parameter > 0.0f) {
            object->use_action_parameter -= frame_time;
        } else {
            object->can_use_object = NULL;
            object->use_action = 0;
        }
        if (object->field_0x1094 != 0)
            --object->field_0x1094;
        object->apiobj.visibility_range_extension =
            (object->ai.field_0x1e5 & 0x40) != 0 ? draw_attention_distance : 0.0f;
        i32 mini_cut_cam = MiniCutCam;
        if (mini_cut_cam != 0 && (high_flags & 1) != 0) {
            i32 gamepad_start = GAMEPAD_START;
            GAMEPAD_s *pad = object->pad_gamepad;
            pad->buttons_held &= gamepad_start;
            pad->buttons_pressed &= gamepad_start;
        }
        if (object->alert_target != NULL) {
            object->alert_target_timer -= frame_time;
            if (__builtin_expect(object->alert_target_timer <= 0.0f, 0)) {
                object->alert_target = NULL;
                object->alert_target_timer = 0.0f;
            }
        }
    }
    f32 follow_offset = 0.0f;
    active_neutral_count = 0;
    party_under_cover = 0;
    i32 all_under_cover = 1;
    i32 cover_result = 0;
    GameObject_s **player_slots = Player;
    for (i32 index = 0; index < 8; ++index) {
        GameObject_s *object = player_slots[index];
        if (object != NULL && (object->apiobj.field_0x1f8 & 0x1001) == 0x1001 &&
            (object->apiobj.field_0x287 == 0 || object->field_0x101c > 0.0f) && !object->not_with_party &&
            (VADER_ADATA == NULL || WORLD->area != VADER_ADATA) && party_cant_be_under_cover == 0) {
            if (object->field_0x108e == 5 ||
                ((FreePlay != 0 || WORLD->current_level == HUB_LDATA) &&
                 (object->apiobj.character_data->model_flags & 0x204) != 0 && object->field_0xcc0 == NULL)) {
                if (alert_obj == NULL || alert_obj->apiobj.field_0x27c == -1 || alert_obj->not_with_party)
                    cover_result = 1;
            }
            all_under_cover = (object->apiobj.character_data->model_flags & 0x80000) != 0 ? all_under_cover : 0;
        }
        party_follow_offsets[index] = 0.5f;
    }
    party_under_cover = cover_result;
    if ((VADER_ADATA == NULL || WORLD->area != VADER_ADATA) && (all_under_cover != 0) &&
        party_cant_be_under_cover == 0) {
        party_under_cover = 1;
    }
    AITriggerSetSysProcess(WORLD->ai_trigger_set_sys);

    f32 drop_timer = drop_back_in_timer;
    APIOBJECT *object_lists[4][64];
    // Keep the original stack frame and pointer-list offsets while scratch slots are identified.
    APIOBJECT **neutral_objects = object_lists[0];
    APIOBJECT **interactive_objects = object_lists[1];
    APIOBJECT **goodies = object_lists[2];
    APIOBJECT **baddies = object_lists[3];
    i32 neutral_count = 0;
    i32 interactive_count = 0;
    i32 goody_count = 0;
    i32 baddy_count = 0;
    GameObject_s *active_player = player;
    APIOBJECT **volatile neutral_cursor = neutral_objects;
    APIOBJECT **volatile goody_cursor = goodies;
    APIOBJECT **baddy_cursor = baddies;
    for (i32 index = 0; index < 8; ++index) {
        GameObject_s *object;
        if (index < 2 && (object = Player[1]) == active_player) {
            if (index != 0)
                object = player_slots[0];
        } else {
            object = player_slots[index];
        }
        if (object != NULL && (object->apiobj.field_0x1f8 & 0x1001) == 0x1001 &&
            (object->apiobj.field_0x287 == 0 || object->field_0x101c > 0.0f) && object->character_context != 0x17 &&
            object->character_context != 0x5a) {
            if (party_under_cover != 0 && (object->ai.field_0x1e5 & 0x40) != 0)
                party_under_cover = 0;
            if (!(object->camera_screen_position.x > -0.85f && object->camera_screen_position.x < 0.85f &&
                  object->camera_screen_position.y > -0.85f && object->camera_screen_position.y < 0.85f) &&
                (object->tag_flags & 2) == 0 && drop_timer > 0.0f) {
                object->ai.nearest_opponent = NULL;
                object->ai.pending_nearest_opponent = NULL;
                object->ai.pending_nearest_metric = 1.0e9f;
                object->ai.opponent = NULL;
                object->ai.pending_opponent = NULL;
                object->ai.pending_opponent_metric = 1.0e9f;
                object->ai.field_0x1e5 &= ~0x10;
            } else if ((object->apiobj.field_0x1f4 & 1) != 0) {
                ++baddy_count;
                *baddy_cursor++ = &object->apiobj;
            } else if ((object->apiobj.field_0x1f4 & 4) != 0) {
                ++neutral_count;
                *neutral_cursor++ = &object->apiobj;
            } else {
                ++goody_count;
                *goody_cursor++ = &object->apiobj;
            }
        }
        object = Player[index];
        if (object != NULL && (object->apiobj.field_0x1f8 & 0x1001) == 0x1001 &&
            (object->apiobj.field_0x287 == 0 || object->field_0x101c > 0.0f) && !object->not_with_party &&
            !object->apiobj.player_controlled) {
            party_follow_offsets[object->apiobj.field_0x27c] = follow_offset;
            follow_offset += 0.2f;
        }
    }
    u64 awareness = 0;
    NUVEC wall_direction;
    u8 shifted_flag;
    u8 cleared_flag;
    object = Obj;
    APIOBJECT **interactive_cursor = interactive_objects;
    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index, ++object) {
        NUVEC direction;
        i32 ground_checks;
        u8 ai_update_flags;
        GAMEPAD_s *pad;
        const u16 character_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER;
        object->active_trigger_set = NULL;
        if ((object->apiobj.field_0x1f8 & character_flags) != character_flags || object->apiobj.field_0x287 != 0 ||
            object->character_context == 0x3b) {
            goto interaction;
        }

        ground_checks = object->apiobj.field_0x27d != 0;
        if (ground_checks == 0) {
            ground_checks = (object->apiobj.character_data->model_flags >> 13) & 1;
        }

        if ((object->apiobj.field_0x1f4 & 0x400) != 0) {
            if ((object->apiobj.field_0x1f4 & 0x10000) != 0) {
                ++baddy_count;
                *baddy_cursor++ = &object->apiobj;
                ++goody_count;
                *goody_cursor++ = &object->apiobj;
            } else if ((object->apiobj.field_0x1f4 & 1) != 0) {
                ++baddy_count;
                *baddy_cursor++ = &object->apiobj;
            } else if ((object->apiobj.field_0x1f4 & 4) != 0) {
                ++neutral_count;
                *neutral_cursor++ = &object->apiobj;
            } else {
                ++goody_count;
                *goody_cursor++ = &object->apiobj;
            }
        }
        if ((object->apiobj.field_0x1f4 & 0x40000) != 0) {
            if ((object->apiobj.flags_high & 0x10) != 0 && object->apiobj.field_0x287 == 0 &&
                object->context_target_position == NULL &&
                (object->apiobj.character_data->game_character->flags_090 & 0x8000) == 0) {
                ++interactive_count;
                *interactive_cursor++ = &object->apiobj;
                object->apiobj.resolved_collision_priority = 0;
                object->apiobj.collision_priority = HIGHGAMEOBJECT - object->apiobj.field_0x289;
                object->ai.field_0x1e5 |= 0x20;
            } else if (object->field_0x101c > 0.0f) {
                object->ai.field_0x1e5 |= 0x20;
            } else {
                object->ai.nearest_opponent = NULL;
                object->ai.opponent = NULL;
                object->ai.field_0x1e5 &= ~0x20;
            }
            if (object->opponent != NULL && (static_cast<APIOBJECT *>(object->opponent)->flags_low & 1) == 0)
                object->opponent = NULL;
            if (object->ai.opponent_object != NULL && ((object->ai.opponent_object->flags_low & 1) == 0 ||
                                                       (object->ai.opponent_object->ai->field_0x1e5 & 0x20) == 0)) {
                object->ai.opponent_metric = 1.0e9f;
                object->ai.opponent = NULL;
            }
            if ((object->field_0xf00 & GAME_OBJECT_AI_UPDATE_PROCESS) != 0)
                AISysUpdateCharacterPathPos(WORLD->ai_sys, &object->apiobj, &object->ai, ground_checks,
                                            object->ai_elapsed_time);
            continue;
        }
        pad = object->pad_gamepad;
        if (pad->pad == NULL) {
            pad->buttons_held = 0;
            pad->buttons_pressed = 0;
            pad->buttons_released = 0;
            pad->left_directions = 0;
            pad->previous_left_directions = 0;
            pad->right_directions = 0;
            pad->previous_right_directions = 0;
            pad->unknown_20 = 0;
        }
        awareness |= object->apiobj.ai_awareness_mask;
        // Network-controlled objects use the direct field test above. The
        // scripted path snapshots its update flags after resetting the pad.
        ai_update_flags = object->field_0xf00;
        if ((ai_update_flags & GAME_OBJECT_AI_UPDATE_PROCESS) != 0) {
            object->script_fire_target = NULL;
            shifted_flag = (object->field_0xef9 << 1) & 4;
            cleared_flag = object->field_0xef9 & ~4;
            cleared_flag |= shifted_flag;
            object->field_0xef9 = cleared_flag;
            object->field_0xef8 &= ~0x20;
            if (__builtin_expect((cleared_flag & 0x80) != 0, 1)) {
                object->field_0xef9 &= ~0x80;
                object->field_0xe64 = object->field_0xe58;
            }
            object->field_0xefa &= ~3;
        }
        AISysProcessCharacter(WORLD->ai_sys, &object->apiobj, &object->ai, ground_checks, object->ai_elapsed_time, 0,
                              (ai_update_flags >> 3) & 1);
        goto ai_precombat;
    ai_combat:
        if ((object->apiobj.field_0x1f8 & 0x180) == 0x80) {
            object->field_0xefc &= ~4;
            object->apiobj.flags_high =
                (object->apiobj.flags_high & ~2) | ((object->apiobj.character_data->model_flags >> 27) & 2);
        } else {
            object->apiobj.flags_high &= ~2;
            i32 defend = false;
            if ((object->field_0xef8 & 0x80) != 0 && object->ai.opponent_object != NULL) {
                BOLTTYPE_s *bolt = BoltType_FindByID(0, WORLD);
                if (bolt->field_10 * bolt->field_14 > object->ai.opponent_metric) {
                    NUVEC forward;
                    NuVecSub(&direction, &object->ai.opponent_object->collision_position,
                             &object->apiobj.collision_position);
                    f32 scale = object->ai.opponent_metric == 0.0f ? 0.0f : 1.0f / object->ai.opponent_metric;
                    NuVecScale(&direction, &direction, scale);
                    NuVecRotateY(&forward, &v001, object->apiobj.movement_facing_angle);
                    f32 dot = NuVecDot(&direction, &forward);
                    u16 angle = object->ai.opponent_metric < 0.5f   ? 0x3aaa
                                : object->ai.opponent_metric < 1.0f ? 0x3000
                                                                    : 0x2555;
                    if (dot > NuTrigTable[angle]) {
                        object->pad_gamepad->buttons_pressed |= GAMEPAD_ACTION;
                        object->script_fire_target = *object->ai.action_target_ref;
                    }
                }
                defend = true;
            }
            if (FreePlay != 0 && (object->apiobj.field_0x1f4 & 0x400) == 0 && object->id == id_DROIDEKA)
                object->pad_gamepad->buttons_pressed |= GAMEPAD_TOGGLERIGHT;
            if ((object->field_0xef8 & 2) != 0 &&
                ((object->apiobj.field_0x1f4 & 0x400) != 0 || party_under_cover == 0 ||
                 object->not_with_party) &&
                (CanFightLikeAJedi(object) != 0 || object->id == id_GAMORREANGUARD)) {
                if ((object->field_0xef8 & 1) != 0 || object->ai.opponent_object != NULL ||
                    (object->field_0xe22 & 8) != 0)
                    object->field_0xed8 = DEFENDTIME;
                if (object->field_0xed8 > 0.0f) {
                    defend = true;
                    if ((object->field_0xe22 & 9) == 9)
                        object->pad_gamepad->buttons_pressed |= GAMEPAD_ACTION;
                }
            }
            if (object->use_weapon) {
                defend = true;
                if ((object->field_0xe22 & 1) != 0) {
                    object->pad_gamepad->buttons_pressed |= GAMEPAD_ACTION;
                    object->use_weapon = 0;
                }
            }
            if ((object->field_0xef8 & 8) != 0 &&
                (defend || (object->field_0xef8 & 0x30) != 0 ||
                 ((object->id == id_YODA || object->id == id_YODAGHOST) && !object->apiobj.player_controlled &&
                  object->apiobj.field_0x27c != -1))) {
                if ((object->field_0xe22 & 1) == 0 && object->character_context != 7 && object->field_0xe32 == 0)
                    object->pad_gamepad->buttons_pressed |= GAMEPAD_ACTION;
            } else if ((object->pad_gamepad->buttons_pressed & GAMEPAD_ACTION) == 0 && (object->field_0xe22 & 1) != 0 &&
                       object->character_context != 6 && object->field_0xe32 == 0) {
                object->pad_gamepad->buttons_pressed |= GAMEPAD_SPECIAL;
            }
        }
        if ((object->apiobj.character_data->game_character->flags_094[3] & 0x10) != 0 &&
            !object->apiobj.player_controlled) {
            f32 threshold = object->apiobj.character_data->game_character->walk_speed - 0.01f;
            if (object->field_0xe31 == 1) {
                if (threshold >= object->pad_gamepad->input_magnitude) {
                    object->ai_jump_timer += FRAMETIME;
                    if (object->ai_jump_timer > 0.1f) {
                        object->pad_gamepad->buttons_pressed |= GAMEPAD_JUMP;
                        object->ai_jump_timer = 0.0f;
                    }
                } else {
                    object->ai_jump_timer = 0.0f;
                }
            } else if (object->pad_gamepad->input_magnitude > threshold) {
                object->ai_jump_timer += FRAMETIME;
                if (object->ai_jump_timer > 1.0f) {
                    object->pad_gamepad->buttons_pressed |= GAMEPAD_JUMP;
                    object->ai_jump_timer = 0.0f;
                }
            } else {
                object->ai_jump_timer = 0.0f;
            }
        }
        if (__builtin_expect(object->ai.action_target_ref != NULL, 1))
            SetObjAsHeadTarget(object, *object->ai.action_target_ref, 2, 1.0f, 0.0f, 0.0f);
        goto interaction;
    ai_precombat:
        if (__builtin_expect(!object->apiobj.player_controlled, 1)) {
            if (object->character_context == 0x5d)
                SetBallooningHeight(object, object->ai.movement_destination.y);
            if ((WORLD->current_level == JEDI_B_LDATA) && object->id == id_JANGOFETT &&
                object->is_boss && object->hover_height_override != 1.0e9f && object->field_0xe31 == 0)
                object->field_0xe31 = 1;
            if (object->apiobj.field_0x27c != -1 && object->field_0xe31 != 0 &&
                (object->ai.capabilities & LEGO_AIPATHCNX_R2D2GLIDE) == 0)
                object->field_0xe31 = 0;
            if (object->character_context == 0x4b && object->suit != NULL &&
                (static_cast<SUIT_s *>(object->suit)->flags & 0x10) != 0)
                object->pad_gamepad->buttons_held |= GAMEPAD_SPECIAL;
            AIPATHCNX *connection = object->ai.path_info.connection;
            if (connection != NULL && (connection->traversal_flags[object->ai.path_info.direction] &
                                       object->ai.capabilities & LEGO_AIPATHCNX_WALLSHUFFLE) != 0)
                goto ai_wall_shuffle;
            if (!(object->ai.path_connection_state == 0 && (object->ai.path_info.flags & 1) != 0 &&
                  object->apiobj.movement_stuck_time > jump_stuck_time))
                goto ai_route;
            if (object->apiobj.supporting_platform_id != -1 && connection != NULL &&
                (connection->original_traversal_flags[0] & LEGO_AIPATHCNX_BLOCKAGE) != 0) {
                if ((object->apiobj.character_data->model_flags & 0x88) != 0) {
                    object->pad_gamepad->buttons_pressed |= GAMEPAD_ACTION;
                    object->apiobj.movement_stuck_time = 0.0f;
                    GameObjectSetCanUse(object, NULL, 5, 0, 1.0f);
                } else if (FreePlay != 0) {
                    object->input_toggle_hold_time -= FRAMETIME;
                    if (object->input_toggle_hold_time <= 0.0f) {
                        Player_ToggleCharacter(object, 1, 0);
                        if (object->id == id_DROIDEKA)
                            Player_ToggleCharacter(object, 1, 0);
                        if (object->apiobj.model_draw_result == 0)
                            object->input_toggle_hold_time = 0.0f;
                        else
                            object->input_toggle_hold_time = 0.25f;
                    }
                }
            } else if ((object->ai.capabilities & (LEGO_AIPATHCNX_R2D2GLIDE | LEGO_AIPATHCNX_JUMP)) != 0 &&
                       object->field_0xe31 == 0) {
                object->pad_gamepad->buttons_pressed |= GAMEPAD_JUMP;
                object->apiobj.movement_stuck_time = 0.0f;
            } else {
                object->ai.field_0x1e6 |= AIPACKET_RUNTIME_USING_PATH_WAYPOINT;
            }
            goto ai_route;
        ai_wall_shuffle: {
            NUVEC lateral, ray, movement;
            object->field_0xf02 |= 1;
            AIPATHNODE *nodes = object->ai.path_info.path->nodes;
            i32 direction = object->ai.path_info.direction;
            u8 other_node = connection->node_indices[direction == 0];
            u8 node = connection->node_indices[direction];
            u16 angle;
            if (object->character_context == 0x45) {
                angle = object->takeover_start_angle;
                wall_direction = v001;
                NuVecRotateY(&wall_direction, &wall_direction, angle);
            } else {
                NuVecSub(&lateral, &nodes[other_node].position, &nodes[node].position);
                lateral.y = 0.0f;
                NuVecNorm(&lateral, &lateral);
                NuVecRotateY(&lateral, &lateral, 0x4000);
                ray = lateral;
                f32 reverse_x = -lateral.x;
                f32 reverse_z = -lateral.z;
                if (GameRayCast(&nodes[node].position, &ray, 0.0f,
                                TERRAINMASK_NONWEAPON | TERRAINMASK_NONDROID | 0x5f) != 0) {
                    f32 first_distance = NuVecMag(&ray);
                    ray.x = reverse_x;
                    ray.y = 0.0f;
                    ray.z = reverse_z;
                    if (GameRayCast(&nodes[node].position, &ray, 0.0f,
                                    TERRAINMASK_NONWEAPON | TERRAINMASK_NONDROID | 0x5f) != 0) {
                        f32 second_distance = NuVecMag(&ray);
                        if (first_distance < 1.0e9f || second_distance < 1.0e9f) {
                            if (second_distance > first_distance) {
                                wall_direction = lateral;
                            } else {
                                wall_direction.x = -lateral.x;
                                wall_direction.y = 0.0f;
                                wall_direction.z = -lateral.z;
                            }
                        }
                    } else if (first_distance < 1.0e9f) {
                        wall_direction = lateral;
                    }
                } else {
                    ray.x = reverse_x;
                    ray.y = 0.0f;
                    ray.z = reverse_z;
                    if (GameRayCast(&nodes[node].position, &ray, 0.0f,
                                    TERRAINMASK_NONWEAPON | TERRAINMASK_NONDROID | 0x5f) != 0 &&
                        NuVecMag(&ray) < 1.0e9f) {
                        wall_direction.x = -lateral.x;
                        wall_direction.y = 0.0f;
                        wall_direction.z = -lateral.z;
                    }
                }
                angle = NuAtan2D(wall_direction.x, wall_direction.z);
            }
            movement.x = object->ai.movement_position.x - object->apiobj.position.x;
            movement.y = 0.0f;
            movement.z = object->ai.movement_position.z - object->apiobj.position.z;
            f32 distance = NuVecMag(&movement);
            if (__builtin_expect(distance > ai_moveradius, 1))
                NuVecScale(&movement, &movement, (ai_moveradius / distance) * 2.0f);
            NuVecRotateY(&movement, &movement, -static_cast<i32>(angle));
            movement.z = 0.0f;
            NuVecRotateY(&movement, &movement, angle);
            NuVecScale(&wall_direction, &wall_direction, ai_moveradius);
            NuVecAdd(&movement, &movement, &wall_direction);
            object->ai.movement_position.x = object->apiobj.position.x + movement.x;
            object->ai.movement_position.y = object->apiobj.position.y;
            object->ai.movement_position.z = object->apiobj.position.z + movement.z;
        }
        }
    ai_route:
        if (FreePlay != 0 && (object->apiobj.field_0x1f4 & 0x400) == 0) {
            if (!object->apiobj.player_controlled && object->character_context != 0x0b) {
                if (drop_in_teleport == 1 && VehicleArea == 0 && (Arcade != 0 || teleport_all_freeplay_modes != 0) &&
                    object->apiobj.model_draw_result == 0 && (object->tag_flags & 2) == 0 &&
                    drop_back_in_timer > 0.0f) {
                    NUVEC position = player->apiobj.last_safe_position;
                    if (position.x == player->apiobj.position.x && position.y == player->apiobj.position.y &&
                        position.z == player->apiobj.position.z)
                        position.z += 0.01f;
                    object->apiobj.start_position = position;
                    object->apiobj.position = object->apiobj.start_position;
                    object->apiobj.velocity = v000;
                    object->apiobj.field_0x276 = player->apiobj.field_0x276;
                    ResetPlayerMoves(object);
                    object->apiobj.movement_stuck_time = 0.0f;
                    InitSurfaceInfo(object);
                    SetObjOnSurface(object, 1);
                    GameObjectOrigin(object);
                    AddGameDebris(WORLD->debris_sys, 0x5c, &object->apiobj.collision_position);
                    goto ai_combat;
                }
                if ((object->ai.field_0x1e6 & AIPACKET_RUNTIME_ROUTE_SELECTED) != 0) {
                    object->ai.current_route = 0xff;
                    object->route_character_id = -1;
                    object->field_0xf14 = 0;
                } else if ((object->ai.field_0x1e6 & AIPACKET_RUNTIME_USING_PATH_WAYPOINT) != 0 &&
                           ((object->ai.field_0x1e6 & AIPACKET_RUNTIME_PATH_BLOCKED) != 0 ||
                            (object->ai.path_info.connection != NULL &&
                             object->ai.path_info.connection->route_mask != 0))) {
                    if ((object->field_0xefc & 0x40) != 0) {
                        if (object->field_0xf14 != object->ai.frame_state) {
                            object->route_character_id = -1;
                            object->field_0xefc &= ~0x40;
                            object->route_suit_index = -1;
                            object->field_0xf14 = 0;
                        }
                    } else if ((object->ai.frame_state & LEGO_AIPATHCNX_DONTTOGGLE) == 0) {
                        if (object->route_character_id == -1 || object->field_0xf14 != object->ai.frame_state ||
                            object->route_character_id == id_DROIDEKA) {
                            object->route_character_id = object->id;
                            object->route_suit_index =
                                object->suit != NULL ? Suit_GetIndex(static_cast<SUIT_s *>(object->suit)) : -1;
                            object->input_toggle_hold_time = 0.0f;
                            object->field_0xf14 = object->ai.frame_state;
                            object->route_search_index = object->ai.next_route;
                            object->route_start_index = object->ai.next_route;
                        }
                        object->input_toggle_hold_time -= FRAMETIME;
                        if (object->input_toggle_hold_time <= 0.0f) {
                            Player_ToggleCharacter(object, 1, 0);
                            if (object->id == id_DROIDEKA)
                                Player_ToggleCharacter(object, 1, 0);
                            object->input_toggle_hold_time = object->apiobj.model_draw_result != 0 ? 0.25f : 0.0f;
                            if (object->id == object->route_character_id) {
                                const i32 route_suit_index = object->route_suit_index;
                                if (Suit_GetIndex(static_cast<SUIT_s *>(object->suit)) == route_suit_index) {
                                    Player_ToggleCharacter(object, -1, 0);
                                    if (object->id == id_DROIDEKA)
                                        Player_ToggleCharacter(object, -1, 0);
                                    object->route_character_id = object->id;
                                    object->route_suit_index = Suit_GetIndex(static_cast<SUIT_s *>(object->suit));
                                    object->ai.next_route = object->route_search_index;
                                    AISysFindRoute(&object->ai);
                                    object->route_search_index = object->ai.next_route;
                                    if (object->route_search_index == object->route_start_index)
                                        object->field_0xefc |= 0x40;
                                }
                            }
                        }
                    }
                } else {
                    object->route_character_id = -1;
                    object->field_0xefc &= ~0x40;
                    object->route_suit_index = -1;
                    object->field_0xf14 = 0;
                    object->route_search_index = 0;
                }
            }
        } else if ((object->ai.field_0x1e6 & AIPACKET_RUNTIME_USING_PATH_WAYPOINT) != 0) {
            AISysFindRoute(&object->ai);
        }
        goto ai_combat;
    interaction:
        if ((object->apiobj.flags_high & 0x10) != 0 && object->apiobj.field_0x287 == 0 &&
            object->context_target_position == NULL &&
            (object->apiobj.character_data->game_character->flags_090 & 0x8000) == 0) {
            ++interactive_count;
            *interactive_cursor++ = &object->apiobj;
            object->apiobj.resolved_collision_priority = 0;
            object->apiobj.collision_priority = HIGHGAMEOBJECT - object->apiobj.field_0x289;
            if ((object->apiobj.flags_low & 2) != 0 || object->high_collision_priority) {
                object->apiobj.collision_priority = 0x8000;
            } else if (!object->apiobj.player_controlled) {
                if (__builtin_expect((object->ai.field_0x1e7 & 0x80) != 0, 1))
                    object->apiobj.collision_priority |= 0x4000;
                if ((object->ai.field_0x1e7 & 0x40) != 0)
                    object->apiobj.collision_priority |= 0x2000;
                if ((object->ai.navigation_flags & 1) != 0)
                    object->apiobj.collision_priority |= 0x1000;
                if ((object->pad_gamepad->allocated_5a & 2) != 0)
                    object->apiobj.collision_priority |= 0x800;
                if ((object->ai.path_info.flags & 1) == 0 || object->ai.path_info.connection == NULL)
                    object->apiobj.collision_priority |= 0x400;
                if (object->ai.movement_parameter == 0.0f) {
                    const f32 delta_x = object->ai.fallback_destination.x - object->apiobj.position.x;
                    const f32 delta_z = object->ai.fallback_destination.z - object->apiobj.position.z;
                    const f32 radius = object->ai.mover_height + 1.0f;
                    direction.x = delta_x;
                    direction.z = delta_z;
                    if (direction.x * direction.x + direction.z * direction.z < radius * radius)
                        object->apiobj.collision_priority |= 0x200;
                }
            }
            object->ai.field_0x1e5 |= 0x20;
            if (object->opponent != NULL && (static_cast<APIOBJECT *>(object->opponent)->flags_low & 1) == 0)
                object->opponent = NULL;
            if (object->ai.opponent_object != NULL && ((object->ai.opponent_object->flags_low & 1) == 0 ||
                                                       (object->ai.opponent_object->ai->field_0x1e5 & 0x20) == 0)) {
                object->ai.opponent = NULL;
                object->ai.opponent_metric = 1.0e9f;
            }
            if (object->ignore_players || debug_ignore_players != 0 || Hub_InMenu() != 0) {
                object->ai_opponent_exclusion_mask |= (u64)1 << player->apiobj.field_0x289;
                if (player2 != NULL)
                    object->ai_opponent_exclusion_mask |= (u64)1 << player2->apiobj.field_0x289;
            }
            if (object->apiobj.player_controlled && object->apiobj.field_0x27d != 0 &&
                object->pad_gamepad->input_magnitude >
                    (WALKSTEALTH != 0 ? object->apiobj.character_data->game_character->walk_speed
                                      : object->apiobj.character_data->game_character->tiptoe_speed))
                object->field_0xef9 |= 8;
        } else if (object->field_0x101c > 0.0f) {
            object->ai.field_0x1e5 |= 0x20;
        } else {
            object->ai.nearest_opponent = NULL;
            object->ai.opponent = NULL;
            object->ai.field_0x1e5 &= ~0x20;
        }
    }
    if (TimingBarSet == 4)
        TBCLOSEFN("(Sys)", 4);
    active_goody_count = goody_count;
    active_baddy_count = baddy_count;
    active_neutral_count = neutral_count;
    if (TimingBarSet == 4)
        TBOPENFN("(LOS)", 4);
    for (i32 index = 0; index < neutral_count; ++index)
        baddies[baddy_count + index] = neutral_objects[index];
    i32 los_count = baddy_count + neutral_count;
    timetogetlos = (f32)(los_count * los_count) * FRAMETIME * 0.5f;
    APIObjectLOSChecks(WORLD->api_object_sys, 2, goody_count, goodies, los_count, baddies,
                       (f32) static_cast<u8>(WORLD->current_level->unknown_0da));
    if (TimingBarSet == 4)
        TBCLOSEFN("(LOS)", 4);
    if (TimingBarSet == 4)
        TBOPENFN("(Avoid)", 4);
    if (test_new_interaction == 0) {
        LEGO_AISysCreatureInteraction2D(WORLD->ai_sys, interactive_count, interactive_objects, NULL, FRAMETIME);
    } else {
        for (i32 index = 0; index < interactive_count; ++index) {
            AIPACKET *packet = interactive_objects[index]->ai;
            packet->antinode_timer -= FRAMETIME;
            if (packet->antinode_timer < 0.0f)
                packet->antinode_timer = 0.0f;
        }
        for (i32 first_index = 0; first_index < interactive_count - 1; ++first_index) {
            APIOBJECT *first = interactive_objects[first_index];
            for (i32 second_index = first_index + 1; second_index < interactive_count; ++second_index) {
                APIOBJECT *second = interactive_objects[second_index];
                f32 radius = first->ai->mover_height + second->ai->mover_height;
                NUVEC difference;
                difference.x = first->ai->movement_position.x - second->ai->movement_position.x;
                if (difference.x > radius || difference.x < -radius)
                    continue;
                difference.z = first->ai->movement_position.z - second->ai->movement_position.z;
                if (difference.z > radius || difference.z < -radius)
                    continue;
                if (first->collision_min.y > second->collision_max.y ||
                    second->collision_min.y > first->collision_max.y || first->collision_link == second ||
                    second->collision_link == first)
                    continue;
                bool fixed_first = first->player_controlled;
                bool fixed_second = second->player_controlled;
                if (first->collision_priority > second->collision_priority)
                    fixed_first = true;
                if (first->collision_priority == second->collision_priority)
                    continue;
                if (second->collision_priority > first->collision_priority)
                    fixed_second = true;
                if (fixed_first && fixed_second)
                    continue;
                f32 distance = NuFsqrt(difference.x * difference.x + difference.z * difference.z);
                if (distance < radius) {
                    difference.y = 0.0f;
                    if (__builtin_expect(!fixed_first, 1)) {
                        if (!fixed_second) {
                            first->resolved_collision_priority = second->collision_priority;
                            difference.x = -difference.x;
                            difference.z = -difference.z;
                            TestWalkAround(second, &first->position.x, &first->position.z, &difference, radius);
                        } else if (second->collision_priority > first->resolved_collision_priority) {
                            first->resolved_collision_priority = second->collision_priority;
                            TestWalkAround(first, &second->position.x, &second->position.z, &difference, radius);
                        }
                    } else if (first->collision_priority > second->resolved_collision_priority) {
                        second->resolved_collision_priority = first->collision_priority;
                        difference.x = -difference.x;
                        difference.z = -difference.z;
                        TestWalkAround(second, &first->position.x, &first->position.z, &difference, radius);
                    }
                    AIPACKET *first_ai = first->ai;
                    f32 first_timer = first_ai->antinode_timer;
                    f32 second_timer = second->ai->antinode_timer;
                    if (__builtin_expect(first_timer > antinode_time && second_timer > antinode_time, 1)) {
                        if (first_timer > second_timer) {
                            second->ai->antinode_timer = first_timer;
                            second->ai->antinode_clockwise = first_ai->antinode_clockwise;
                        } else {
                            first_ai->antinode_timer = second_timer;
                            first_ai->antinode_clockwise = second->ai->antinode_clockwise;
                        }
                    } else if (second_timer > antinode_time) {
                        first_ai->antinode_timer = second_timer;
                        first_ai->antinode_clockwise = second->ai->antinode_clockwise;
                    } else if (first_timer > 0.0f) {
                        second->ai->antinode_timer = first_timer;
                        second->ai->antinode_clockwise = first_ai->antinode_clockwise;
                    }
                    if (((first->field_0x1f4 ^ second->field_0x1f4) & 1) != 0) {
                        first->ai->field_0x1e5 |= 0x40;
                        second->ai->field_0x1e5 |= 0x40;
                    }
                }
            }
        }
        AISysCreatureAntinodeInteraction(WORLD->ai_sys, interactive_count, interactive_objects, NULL);
    }
    if (TimingBarSet == 4)
        TBCLOSEFN("(Avoid)", 4);
    GameObject_s **cleanup_players = Player;
    if (party_under_cover != 0) {
        for (i32 index = 0; index < 8; ++index) {
            GameObject_s *cover_player = cleanup_players[index];
            if (cover_player != NULL)
                cover_player->ai.field_0x1e5 &= ~0x40;
        }
    }
    if (TimingBarSet == 4)
        TBOPENFN("(Opponent)", 4);
    GameCreatureOpponentSelection(WORLD->ai_sys, interactive_count, interactive_objects, goody_count, goodies,
                                  baddy_count, baddies, awareness, FRAMETIME);
    if (TimingBarSet == 4)
        TBCLOSEFN("(Opponent)", 4);
    if (TimingBarSet == 4)
        TBOPENFN("(Turret)", 4);
    GizTurrets_OpponentSelection(WORLD->giz_turret_sys, goody_count, goodies, baddy_count, baddies);
    if (TimingBarSet == 4)
        TBCLOSEFN("(Turret)", 4);
    oneAtOnce_MaintainArray();
}

GameObject_s *AddDynamicCreature(i32 model, nuvec_s *position, i32 angle, char *script_name, AIPATHINFO_s *path_info,
                                 AIGROUP_s *group, i32 set_on_surface, nugspline_s *spline, nuvec_s *spline_offset,
                                 i32 spline_mode, i32 creature_set) {
    u16 yaw, pitch;
    NUVEC offset, spline_point;
    const bool has_no_spline = spline == NULL;
    if (position == NULL && spline == NULL) {
        return NULL;
    }

    if (NOAICREATURES != 0 && model != id_DRAGBOMB && (GCDataList[model].flags_090 & 0x40) == 0) {
        return NULL;
    }

    u32 arcade_mode;
    Arcade_GetMode(&arcade_mode);
    if ((arcade_mode & 0x10) != 0) {
        return NULL;
    }

    if (static_cast<u32>(model) >= 0x154 || apicharsys->playermodelids[model] == -1) {
        return NULL;
    }

    GameObject_s *object = AddCreature(model, 0);
    if (object == NULL) {
        return NULL;
    }

    object->apiobj.field_0x1f4 |= APIOBJECT_MOTION_FLAG_AI_CONTROLLED;
    CHARACTERDATA *character = &apicharsys->char_data[model];
    const u32 model_flags = character->model_flags;
    if ((model_flags & 0x200) != 0) {
        object->apiobj.field_0x1f4 |= 0x404;
    } else if ((model_flags & 4) != 0) {
        object->apiobj.field_0x1f4 |= 0x401;
    }
    if ((model_flags & 0x1000) == 0)
        object->field_0x1050 |= 1;
    else
        object->field_0x1050 |= 5;

    GAMECHARACTERDATA &game_character = *(GAMECHARACTERDATA *)character->field11_0x24;
    object->apiobj.viewdistance = game_character.viewdistance;
    object->apiobj.heardistance = game_character.heardistance;
    object->apiobj.maxviewheight = game_character.maxviewheight;
    object->apiobj.minviewheight = game_character.minviewheight;
    object->field_0xef9 &= static_cast<u8>(~8u);
    object->ai.pending_nearest_metric = 1000000000.0f;
    object->ai.pending_opponent_metric = 1000000000.0f;
    object->field_0xef8 &= static_cast<u8>(~1u);
    object->ai.field_0x1e5_bit4 = 0;
    object->ai.nearest_opponent = NULL;
    object->ai.pending_nearest_opponent = NULL;
    object->ai.opponent = NULL;
    object->ai.pending_opponent = NULL;
    object->ai.antinode_timer = 0.0f;
    object->apiobj.ai_awareness_mask = 0;
    object->ai_seen_mask = 0;
    object->field_0xecc = 0;
    object->field_0xed0 = 0;
    object->field_0xed8 = 0;
    object->ai.field_0x1e5_bit6 = 0;
    InitPlayerAI(object);

    if (spline != NULL) {
        SPLINEPOS_s *spline_position = reinterpret_cast<SPLINEPOS_s *>(&object->movement_spline);
        InitSplinePosition(spline_position, spline, 0.0f, spline_mode);
        SPLINEPOSITION_RUNTIME_s *runtime = reinterpret_cast<SPLINEPOSITION_RUNTIME_s *>(spline_position);
        PointAlongSpline(runtime->spline, runtime->normalized_position, &spline_point, &yaw, &pitch, runtime->looping);
        object->apiobj.movement_facing_angle = yaw;
        object->apiobj.facing_angle = yaw;
        object->apiobj.field_0x276 = yaw;
        object->apiobj.pitch_angle = static_cast<u16>(-pitch);

        if (spline_offset != NULL) {
            object->movement_spline_offset = *spline_offset;
            if (object->movement_spline_offset.x != 0.0f || object->movement_spline_offset.y != 0.0f ||
                object->movement_spline_offset.z != 0.0f) {
                NuVecRotateX(&offset, &object->movement_spline_offset, static_cast<u16>(-pitch));
                NuVecRotateY(&offset, &offset, object->apiobj.field_0x276);
                NuVecAdd(&spline_point, &spline_point, &offset);
            }
        }
        object->apiobj.position = spline_point;
    } else {
        object->apiobj.position = *position;
        object->apiobj.movement_facing_angle = static_cast<u16>(angle);
        object->apiobj.facing_angle = static_cast<u16>(angle);
        object->apiobj.field_0x276 = static_cast<u16>(angle);
        if (group != NULL) {
            AddToAIGroup(group, &object->apiobj);
            offset.x = ((object->ai.group_column + 1) / 2) * group->x_spacing;
            if (object->ai.group_column & 1)
                offset.x = -offset.x;
            offset.z = -group->z_spacing * static_cast<u32>(object->ai.group_member);
            offset.y = 0.0f;
            NuVecRotateY(&offset, &offset, angle);
            NuVecAdd(&object->apiobj.position, &object->apiobj.position, &offset);
        }
    }

    ResetPlayerMoves(object);
    plr_lastpos = object->apiobj.start_position = object->apiobj.collision_position = object->apiobj.initial_position =
        object->apiobj.position;
    object->apiobj.velocity = v000;

    GetTopBot(object);
    GameObjectDimensions(object);
    ResetRumble(&object->pad_gamepad->rumble_packet);
    ResetLights(&object->apiobj.position, &object->light_data, WORLD->rtl_set);

    object->field_0x661 = 0xff;
    object->sock_segment = -1;
    if (WORLD->sock_sys != NULL) {
        extern i32 complexsockposition_forcesock;
        if (Door_NextSock != -1 && WORLD->sock_sys->sock[Door_NextSock].valid != 0)
            complexsockposition_forcesock = Door_NextSock;
        ComplexSockPosition(WORLD->sock_sys, &object->apiobj.position, -1, -1, &object->sock_position);
        ComplexSockAngles(&object->sock_angles);
    }
    NuVecRotateYValZ(&object->facing_direction, 1.0f, object->apiobj.field_0x276);
    object->apiobj.model_draw_result = 1;
    object->use_model_origin = 0;
    object->apiobj.field_0x288 = 0;
    object->field_0xefe &= ~4u;

    if (has_no_spline) {
        InitSurfaceInfo(object);
        if (set_on_surface != 0) {
            SetObjOnSurface(object, 0);
        }
    }
    PortalGameObject(object, 1, 1, -1, WORLD->current_gscn);
    if (reset_restart != 0) {
        CHARACTERDATA *character = object->apiobj.character_data;
        object->current_hp = object->hitpoints = ((GAMECHARACTERDATA *)character->field11_0x24)->hitpoints;
        object->field_0xe37 = ((GAMECHARACTERDATA *)character->field11_0x24)->field_0xf5;
        object->field_0xe38 = 4;
    }

    object->apiobj.field_0x1f4 &= ~0x100u;
    object->apiobj.field_0x1f8 &= static_cast<u16>(~4u);
    object->field_0x1004 = 1.0f;
    object->apiobj.field_0x287 = 0;
    object->field_0x7a5 = 0xff;
    object->apiobj.field_0x285 = 0;
    memset(&object->ai.path_info, 0, sizeof(object->ai.path_info));
    object->ai.field_0x124 = -1;
    object->ai.field_0x138 = 0xff;
    object->ai.field_0x139 = 0;
    if (has_no_spline) {
        if (path_info != NULL) {
            AISysCharacterSetPath(&object->ai, path_info->path);
            AISysCharacterSetPathCnx(&object->ai, &object->apiobj.position, path_info->connection,
                                     path_info->direction);
        }
        if (object->ai.path_info.connection == NULL) {
            AISysGetCharacterPathPos(WORLD->ai_sys, &object->apiobj, &object->ai, 0xff,
                                     static_cast<i8>(object->apiobj.field_0x27d));
        }
    }
    AIScriptProcessorInit(WORLD->ai_sys, &object->ai, &object->ai.script_process, NULL, script_name, NULL, 1, NULL,
                          NULL);
    object->apiobj.field_0x214 = 2000000.0f;
    if (object->ai.group != NULL) {
        AIGROUP *active_group = object->ai.group;
        u32 member = object->ai.group_member_index;
        active_group->member_is_alive |= 1ULL << member;
        AIROW *row = &active_group->rows[object->ai.group_row];
        u32 column = object->ai.group_member_index - object->ai.group_row * active_group->count_across;
        row->is_alive |= 1ULL << column;
        if (object->ai.group_column == 0) {
            row->pos = object->apiobj.position;
            row->path_info = object->ai.path_info;
            row->next_connection = NULL;
            row->y_rot = object->apiobj.field_0x276;
            row->is_turning = 0;
        }
    }
    PreResetCode(object);
    PostResetCode(object);
    GameObjectOrigin(object);
    object->ai_update_position = object->apiobj.last_safe_position = object->apiobj.position;
    if (static_cast<u32>(creature_set - 1) < 16) {
        object->ai.creature_set = static_cast<u8>(creature_set);
        ++aicreature_sets_alive[creature_set - 1];
    }
    return object;
}

void AISysGetPathPos2(AISYS_s *system, nuvec_s *position, AIPATHINFO_s *path_info, nuvec_s *path_position, AIPATH_s *,
                      i32) {
    if (position != NULL && system != NULL && path_info != NULL) {
        APIOBJECT object;
        AIPACKET packet;
        memset(&object, 0, sizeof(object));
        memset(&packet, 0, sizeof(packet));
        object.ai = &packet;
        object.position = *position;
        packet.terrain_origin = *position;
        AISysGetCharacterPathPos(system, &object, &packet, 0xff, 1);
        *path_info = packet.path_info;
        if (path_position != NULL) {
            *path_position = packet.last_path_position;
        }
    }
}

ADDGIZMOTYPE *AI_RegisterGizmo(i32 type_id) {
    static ADDGIZMOTYPE addtype;

    addtype = Default_ADDGIZMOTYPE;
    addtype.name = "AIProcessor";
    addtype.prefix = "ai_";
    addtype.fns.unknown1 = 0;
    addtype.fns.get_max_gizmos_fn = AI_GetMaxGizmos;
    addtype.fns.add_gizmos_fn = AI_AddGizmos;
    addtype.fns.early_update_fn = NULL;
    addtype.fns.late_update_fn = NULL;
    addtype.fns.draw_fn = NULL;
    addtype.fns.panel_draw_fn = NULL;
    addtype.fns.get_gizmo_name_fn = AI_GetGizmoName;
    addtype.fns.get_output_fn = AI_GetOutput;
    addtype.fns.get_output_name_fn = AI_GetOutputName;
    addtype.fns.get_num_outputs_fn = AI_GetNumOutputs;
    addtype.fns.activate_fn = AI_Activate;
    addtype.fns.activate_rev_fn = NULL;
    addtype.fns.set_visibility_fn = NULL;
    addtype.fns.get_visibility_fn = NULL;
    addtype.fns.get_pos_fn = NULL;
    addtype.fns.using_special_fn = NULL;
    addtype.fns.bolt_hit_plat_fn = NULL;
    addtype.fns.get_best_bolt_target_fn = NULL;
    addtype.fns.bolt_hit_fn = NULL;
    addtype.fns.allocate_progress_data_fn = NULL;
    addtype.fns.clear_progress_fn = NULL;
    addtype.fns.store_progress_fn = NULL;
    addtype.fns.reset_fn = NULL;
    addtype.fns.reserve_buffer_space_fn = NULL;
    addtype.fns.load_fn = NULL;
    addtype.fns.post_load_fn = NULL;
    addtype.fns.add_level_sfx_fn = NULL;

    return &addtype;
}

u32 GameAI_TotalScore() {
    GameObject_s *object = Obj;
    u32 total = 0;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++object) {
        if ((object->apiobj.flags_low & 1) == 0 || (object->apiobj.field_0x1f4 & 0x400) == 0 ||
            object->ai.field_0x134 == 0xff)
            continue;
        GAMECHARACTERDATA_s *config = static_cast<GAMECHARACTERDATA_s *>(object->apiobj.character_data->field11_0x24);
        if ((config->flags_090 & 0x40) == 0 && object->id != id_GONKDROID)
            total += static_cast<u16>(config->field_0xee);
    }
    return total;
}

