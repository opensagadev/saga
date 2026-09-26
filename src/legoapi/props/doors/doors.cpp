#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numath/nuplane.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void Hub_ActivateDoorMenu(LEVELDATA_s **level);

namespace {
    const f32 DOOR_BLOCK_VELOCITY = 10.0f;
} // namespace

// Original doors.cpp data and BSS, in original declaration order.
i32 Door_NextSock = -1;
static char door_gizmotype_id = -1;

char Door_ExitName[64];
char Door_ExitCameraSplineName[32];
i32 Door_Start;
DOOR_s *Door_Last;
NUGSPLINE *Door_CutSpl;
i32 Door_UseCutCam;
NUVEC Door_CutCamPos0;
NUVEC Door_CutCamPos1;
f32 Door_CutCamWait;
f32 Door_CutCamWaitTime;
f32 Door_CutCamBlendTime;
i32 Door_CutLookAtPlayers;
i32 gone_through_door_to_new_level;
i32 gone_through_door_to_new_mode;
DOOR_s *setlastdoor_last;
void (*Door_GoThrough_ExtraCodeFn)(WORLDINFO_s *, DOOR_s *);

static WORLDINFO_s *D_worldinfo;
static DOOR_s *D_door;
static i32 door_cutscenesnap;

static void Door_AddGizmos(GIZMOSYS *gizmo_sys, i32 type_id, void *world_ptr, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (world != NULL && world->doors != NULL && world->door_count > 0) {
        i32 i = 0;
        do {
            GizmoGetUniqueName(world->gizmo_sys, const_cast<char *>("Door_"), world->doors[i].name,
                               world->doors[i].gizmo_name, sizeof(world->doors[i].gizmo_name));
            AddGizmo(gizmo_sys, type_id, NULL, &world->doors[i]);
            ++i;
        } while (world->door_count > i);
    }
}

static void D_cam_lookatplayers(NUFPAR *) {
    D_door->flags |= DOOR_FLAG_CAMERA_LOOK_AT_PLAYERS;
}

static void D_spline(NUFPAR *parser) {
    if (NuFParGetWord(parser) == 0 || NuStrLen(parser->word_buf) > 63) {
        return;
    }

    NuStrCpy(D_door->name, parser->word_buf);
    D_door->spline = NuSplineFind(D_worldinfo->current_gscn, D_door->name);
    if (D_door->spline == NULL || D_door->spline->length < 4) {
        D_door->spline = NULL;
        D_door->name[0] = '\0';
        return;
    }

    for (i32 i = 0; i < D_worldinfo->door_count; i++) {
        DOOR_s *other = &D_worldinfo->doors[i];
        if (other->spline == D_door->spline) {
            D_door->spline = NULL;
            D_door->name[0] = '\0';
            return;
        }
    }
}

static void D_level(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        i32 index;
        Level_FindByName(parser->word_buf, &index);
        if (index != -1) {
            D_door->level = static_cast<i16>(index);
        }
    }
}

static void D_level_freeplay(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        i32 index;
        Level_FindByName(parser->word_buf, &index);
        if (index != -1 && (LDataList[index].flags & (LEVEL_INTRO | LEVEL_MIDTRO | LEVEL_OUTRO)) == 0) {
            D_door->freeplay_level = static_cast<i16>(index);
        }
    }
}

static void D_cam_spline(NUFPAR *parser) {
    if (NuFParGetWord(parser) == 0 || NuStrLen(parser->word_buf) > 31) {
        return;
    }
    NuStrCpy(D_door->camera_spline_name, parser->word_buf);
    D_door->camera_spline = NuSplineFind(D_worldinfo->current_gscn, D_door->camera_spline_name);
    if (D_door->camera_spline != NULL && D_door->camera_spline->length != 2) {
        D_door->camera_spline_name[0] = '\0';
        D_door->camera_spline = NULL;
    }
}

static void D_cam_wait(NUFPAR *parser) {
    D_door->camera_wait = NuFParGetFloat(parser);
}

static void D_cam_blend_time(NUFPAR *parser) {
    D_door->camera_blend_time = NuFParGetFloat(parser);
}

static void D_one_way(NUFPAR *) {
    D_door->flags |= DOOR_FLAG_ONE_WAY;
}

static void D_two_player_only(NUFPAR *) {
    D_door->flags |= DOOR_FLAG_TWO_PLAYER_ONLY;
}

static void D_do_not_use(NUFPAR *) {
    D_door->flags |= DOOR_FLAG_DO_NOT_USE;
}

static void D_next_sock(NUFPAR *parser) {
    i32 next_sock = NuFParGetInt(parser);
    if (static_cast<u32>(next_sock) < 64) {
        D_door->next_sock = static_cast<u8>(next_sock);
    }
}

static void D_vehicle(NUFPAR *parser) {
    // Vehicle names are consumed here exactly as a list on the current
    // line. Their name-to-type callback is registered by the game layer.
    while (NuFParGetWord(parser) != 0) {
    }
}

static void D_use_as_start(NUFPAR *) {
    D_door->flags |= DOOR_FLAG_USE_AS_START;
}

static i32 Door_GetMaxGizmos(void *door) {
    WORLDINFO *world = static_cast<WORLDINFO *>(door);
    if (world == NULL) {
        return 0;
    }
    return world->current_level->max_doors;
}

static char *Door_GetGizmoName(GIZMO *gizmo) {
    if (gizmo == NULL || gizmo->object == NULL) {
        return NULL;
    }
    DOOR_s *door = static_cast<DOOR_s *>(gizmo->object);
    return door->gizmo_name;
}

static i32 Door_GetOutput(GIZMO *gizmo, i32, i32) {
    if (gizmo == NULL || gizmo->object == NULL) {
        return 0;
    }
    DOOR_s *door = static_cast<DOOR_s *>(gizmo->object);
    return door->active == 0;
}

static i32 Door_GetNumOutputs(GIZMO *) {
    return 1;
}

char *Door_GetOutputName(GIZMO *, i32) {
    return const_cast<char *>("Active");
}

void Door_Activate(GIZMO *gizmo, i32 value) {
    if (gizmo == NULL || gizmo->object == NULL) {
        return;
    }
    DOOR_s *door = static_cast<DOOR_s *>(gizmo->object);
    door->active = value == 0;
}

static void D_cut_scene(NUFPAR *parser) {
    if (NuFParGetWord(parser) != 0) {
        WORLDINFO_s *world = WorldInfo_CurrentlyLoading();
        D_door->cutscene = CutScene_Find(world->cutscene_sys, parser->word_buf);
    }
}

static NUFPCOMJMP Door_ConfigKeywords[] = {
    {const_cast<char *>("spline"), D_spline},
    {const_cast<char *>("level"), D_level},
    {const_cast<char *>("level_freeplay"), D_level_freeplay},
    {const_cast<char *>("cam_spline"), D_cam_spline},
    {const_cast<char *>("cam_wait"), D_cam_wait},
    {const_cast<char *>("cam_blend_time"), D_cam_blend_time},
    {const_cast<char *>("cam_lookatplayers"), D_cam_lookatplayers},
    {const_cast<char *>("1_way"), D_one_way},
    {const_cast<char *>("one_way"), D_one_way},
    {const_cast<char *>("2_player_only"), D_two_player_only},
    {const_cast<char *>("2_players_only"), D_two_player_only},
    {const_cast<char *>("two_player_only"), D_two_player_only},
    {const_cast<char *>("two_players_only"), D_two_player_only},
    {const_cast<char *>("do_not_use"), D_do_not_use},
    {const_cast<char *>("next_sock"), D_next_sock},
    {const_cast<char *>("vehicle"), D_vehicle},
    {const_cast<char *>("use_as_start"), D_use_as_start},
    {const_cast<char *>("cut_scene"), D_cut_scene},
    {const_cast<char *>("cutscene"), D_cut_scene},
    {NULL, NULL},
};

void Doors_Configure(WORLDINFO_s *world, char *config) {
    world->doors = NULL;
    if (world->current_gscn == NULL) {
        return;
    }

    NUFPAR *parser = NuFParCreateMem(const_cast<char *>("doors"), config, 0xffff);
    if (parser == NULL) {
        return;
    }

    world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
    DOOR_s *doors = reinterpret_cast<DOOR_s *>(world->giz_buffer.void_ptr);
    world->doors = doors;
    NuFParPushCom(parser, Door_ConfigKeywords);

    i32 in_door = 0;
    DOOR_s *door = doors;
    while (NuFParGetLine(parser) != 0) {
        if (NuFParGetWord(parser) == 0) {
            continue;
        }

        if (!in_door) {
            if (NuStrICmp(parser->word_buf, const_cast<char *>("door_start")) != 0) {
                continue;
            }

            D_worldinfo = world;
            D_door = door;
            door->name[0] = '\0';
            door->camera_spline_name[0] = '\0';
            door->spline = NULL;
            door->pos = v000;
            door->radius = 1.0f;
            door->normal = v001;
            door->level = -1;
            door->freeplay_level = -1;
            door->next_sock = 0xff;
            door->flags = 0;
            door->vehicle = 0xff;
            door->active = 0;
            door->camera_spline = NULL;
            door->camera_wait = 0.0f;
            door->camera_blend_time = 1.0f;
            door->vehicle_mask = 0;
            door->vehicle_mode = 0;
            door->cutscene = NULL;
            in_door = 1;
            continue;
        }

        if (NuStrICmp(parser->word_buf, const_cast<char *>("door_end")) != 0) {
            NuFParInterpretWord(parser);
            continue;
        }

        if (door->spline != NULL && door->level != -1) {
            if (door->freeplay_level == -1) {
                door->freeplay_level = door->level;
            }
            world->door_count++;
            door++;
        }
        in_door = 0;
    }

    NuFParDestroy(parser);
    if (world->door_count > 0) {
        world->giz_buffer.addr = ALIGN(reinterpret_cast<usize>(door), 16);
    } else {
        world->doors = NULL;
    }
}

void Door_Reset() {
    Door_ExitName[0] = '\0';
    Door_Start = 0;
    Door_NextSock = -1;
}

void Doors_Init(WORLDINFO_s *world) {
    setlastdoor_last = NULL;
    world->start_door = NULL;

    DOOR_s *door = world->doors;
    for (i32 i = 0; door != NULL && i < world->door_count; i++, door++) {
        NUVEC *points = door->spline->pts;

        NuVecAdd(&door->pos, &points[0], &points[1]);
        NuVecAdd(&door->pos, &door->pos, &points[2]);
        NuVecAdd(&door->pos, &door->pos, &points[3]);
        NuVecScale(&door->pos, &door->pos, 0.25f);

        door->radius = NuVecDist(&door->pos, &points[0], NULL);
        for (i32 point = 1; point < 4; point++) {
            f32 distance = NuVecDist(&door->pos, &points[point], NULL);
            if (distance > door->radius) {
                door->radius = distance;
            }
        }
        door->radius *= 1.01f;

        NuVecSurfaceNormal(&door->normal, &points[0], &points[1], &points[3]);
        door->point0 = points[0];
        door->point1 = points[1];
        door->point3 = points[3];
        NuVecAdd(&door->opposite_midpoint, &door->point1, &door->point3);
        NuVecScale(&door->opposite_midpoint, &door->opposite_midpoint, 0.5f);
        NuVecSub(&door->opposite_midpoint, &door->opposite_midpoint, &door->point0);
        NuVecScale(&door->opposite_midpoint, &door->opposite_midpoint, 2.0f);
        NuVecAdd(&door->opposite_midpoint, &door->opposite_midpoint, &door->point0);
        NuPlnEqn(&door->plane, &door->point0, &door->point1, &door->point3);

        if (world->start_door == NULL && (door->flags & DOOR_FLAG_USE_AS_START) != 0 && door->spline->length > 5) {
            world->start_door = door;
        }
    }
}

DOOR_s *Door_FindByName(WORLDINFO_s *world, char *name) {
    if (name != NULL && world->doors != NULL && world->door_count > 0) {
        i32 i = 0;
        DOOR_s *b = world->doors;
        do {
            if (NuStrICmp(name, (char *)b) == 0) {
                return b;
            }
            i++;
            b++;
        } while (i < world->door_count);
    }
    return NULL;
}

void Door_SetCutCam(DOOR_s *door) {
    Door_UseCutCam = 0;
    if (door->camera_spline == NULL) {
        if (door->camera_spline_name[0] == '\0' || door->level == WORLD->level_idx) {
            return;
        }
        Door_UseCutCam = 1;
        NuStrCpy(Door_ExitCameraSplineName, door->camera_spline_name);
    } else {
        Door_UseCutCam = 1;
        Door_CutCamPos0 = door->camera_spline->pts[0];
        Door_CutCamPos1 = door->camera_spline->pts[1];
    }
    Door_CutCamWaitTime = door->camera_wait;
    Door_CutCamWait = door->camera_wait;
    Door_CutCamBlendTime = door->camera_blend_time;
    Door_CutLookAtPlayers = door->flags & DOOR_FLAG_CAMERA_LOOK_AT_PLAYERS;
}

void StartDoorPositions(void) {
    Door_Start = 0;
    if (Door_ExitName[0] == '\0') {
        return;
    }
    DOOR_s *a = WORLD->doors;
    if (a != NULL && WORLD->door_count > 0) {
        i32 i = 0;
        do {
            if (a->spline != NULL && NuStrICmp(a->name, Door_ExitName) == 0) {
                if (a->spline->length < 6) {
                    PlayerStart[0].pos = OldPlrSPos[7].door_fallback.position;
                    PlayerStart[0].angle = OldPlrSPos[7].door_fallback.angle;
                } else {
                    NUVEC tmp;
                    NUVEC *arr = a->spline->pts;
                    NuVecSub(&tmp, &arr[5], &arr[4]);
                    PlayerStart[0].pos = &arr[4];
                    PlayerStart[0].angle = NuAtan2D(tmp.x, tmp.z);
                }
                if (a->spline->length < 8) {
                    PlayerStart[1].pos = PlayerStart[0].pos;
                    PlayerStart[1].angle = PlayerStart[0].angle;
                } else {
                    NUVEC tmp;
                    NUVEC *arr = a->spline->pts;
                    NuVecSub(&tmp, &arr[7], &arr[6]);
                    PlayerStart[1].pos = &arr[6];
                    PlayerStart[1].angle = NuAtan2D(tmp.x, tmp.z);
                }
                if (a->spline->length < 10) {
                    PlayerStart[2].pos = PlayerStart[1].pos;
                    PlayerStart[2].angle = PlayerStart[1].angle;
                } else {
                    NUVEC tmp;
                    NUVEC *arr = a->spline->pts;
                    NuVecSub(&tmp, &arr[9], &arr[8]);
                    PlayerStart[2].pos = &arr[8];
                    PlayerStart[2].angle = NuAtan2D(tmp.x, tmp.z);
                }
                if (a->spline->length < 12) {
                    PlayerStart[3].pos = PlayerStart[2].pos;
                    PlayerStart[3].angle = PlayerStart[2].angle;
                } else {
                    NUVEC tmp;
                    NUVEC *arr = a->spline->pts;
                    NuVecSub(&tmp, &arr[11], &arr[10]);
                    PlayerStart[3].pos = &arr[10];
                    PlayerStart[3].angle = NuAtan2D(tmp.x, tmp.z);
                }
                if (a->spline->length < 14) {
                    PlayerStart[4].pos = PlayerStart[3].pos;
                    PlayerStart[4].angle = PlayerStart[3].angle;
                } else {
                    NUVEC tmp;
                    NUVEC *arr = a->spline->pts;
                    NuVecSub(&tmp, &arr[13], &arr[12]);
                    PlayerStart[4].pos = &arr[12];
                    PlayerStart[4].angle = NuAtan2D(tmp.x, tmp.z);
                }
                if (a->spline->length < 16) {
                    PlayerStart[5].pos = PlayerStart[4].pos;
                    PlayerStart[5].angle = PlayerStart[4].angle;
                } else {
                    NUVEC tmp;
                    NUVEC *arr = a->spline->pts;
                    NuVecSub(&tmp, &arr[15], &arr[14]);
                    PlayerStart[5].pos = &arr[14];
                    PlayerStart[5].angle = NuAtan2D(tmp.x, tmp.z);
                }
                if (a->spline->length < 18) {
                    PlayerStart[6].pos = PlayerStart[5].pos;
                    PlayerStart[6].angle = PlayerStart[5].angle;
                } else {
                    NUVEC tmp;
                    NUVEC *arr = a->spline->pts;
                    NuVecSub(&tmp, &arr[17], &arr[16]);
                    PlayerStart[6].pos = &arr[16];
                    PlayerStart[6].angle = NuAtan2D(tmp.x, tmp.z);
                }
                if (a->spline->length < 20) {
                    PlayerStart[7].pos = PlayerStart[6].pos;
                    PlayerStart[7].angle = PlayerStart[6].angle;
                } else {
                    NUVEC tmp;
                    NUVEC *arr = a->spline->pts;
                    NuVecSub(&tmp, &arr[19], &arr[18]);
                    PlayerStart[7].pos = &arr[18];
                    PlayerStart[7].angle = NuAtan2D(tmp.x, tmp.z);
                }
                Door_Start = 1;
                return;
            }
            i++;
            a++;
        } while (i < WORLD->door_count);
    }
    Door_ExitName[0] = '\0';
}

void Doors_SetLastDoor(DOOR_s *door) {
    if (door != NULL && door != setlastdoor_last) {
        NuStrCpy(Door_ExitName, door->name);
        StartDoorPositions();
        setlastdoor_last = door;
        Door_Last = door;
    }
}

void Door_GoThrough(WORLDINFO_s *world, DOOR_s *door, i32 player_triggered) {
    (void)player_triggered;

    if (player != NULL) {
        player->KillTasks();
    }
    if (NewLData != NULL) {
        return;
    }
    if (door->level == -1) {
        ResetBits |= RESETBIT_DOOR_TRANSITION;
        Door_Last = door;
        if (Door_GoThrough_ExtraCodeFn != NULL) {
            Door_GoThrough_ExtraCodeFn(world, door);
        }
        return;
    }

    NuStrCpy(Door_ExitName, door->name);
    if (door->cutscene == NULL || CutScene_HasPlayed(static_cast<CUTINFO *>(door->cutscene))) {
        door_cutscenesnap = 0;
        const i32 destination_level = InStory() != 0 ? door->level : door->freeplay_level;
        if (destination_level == world->level_idx) {
            StartDoorPositions();
            NewGameMode();
            for (i32 index = 0; index < 8; ++index) {
                GameObject_s *current_player = Player[index];
                if (current_player == NULL) {
                    continue;
                }
                PlayerProgress[index].field_0xb = current_player->field_0xe31 == 1;
                PlayerProgress[index].field_0x9 =
                    current_player->apiobj.field_0x287 == 0 ? current_player->field_0xe38 : 4;
            }
            gone_through_door_to_new_mode = 1;
        } else {
            NewLData = &LDataList[destination_level];
            if (FreePlay != 0 && (NewLData->flags & LEVEL_OUTRO) != 0) {
                LEVELDATA_s *status_level = Area_FindStatusLevel(WORLD->area, NULL);
                if (status_level != NULL) {
                    NewLData = status_level;
                    if (ChallengeMode != 0) {
                        ChallengeMode = 4;
                    }
                }
            }
        }
    } else if (door_cutscenesnap == 0) {
        newmode_cutinfo = static_cast<CUTINFO *>(door->cutscene);
        StartDoorPositions();
        NewGameMode();
    } else {
        WORLDINFO_s *active_world = WorldInfo_CurrentlyActive();
        NewCutScene(static_cast<CUTINFO *>(door->cutscene), active_world->cutscene_sys, NULL, 0);
    }

    Door_NextSock = static_cast<i8>(door->next_sock);
    Door_SetCutCam(door);
    ResetBits |= RESETBIT_DOOR_TRANSITION;
    if (NewLData != NULL && NewLData != world->current_level) {
        if (world->area == HUB_ADATA) {
            Hub_ActivateDoorMenu(&NewLData);
        } else if (NewLData->area_index == world->level_sub_id) {
            gone_through_door_to_new_level = 1;
        }
    }

    Door_Last = door;
    if (Door_GoThrough_ExtraCodeFn != NULL) {
        Door_GoThrough_ExtraCodeFn(world, door);
    }
}

void Doors_Check(WORLDINFO_s *world, GameObject_s *object) {
    if (object->apiobj.field_0x287 != 0 ||
        (LEGOCONTEXT_DOOMED != -1 && LEGOCONTEXT_DOOMED == static_cast<i8>(object->field_0x7a5)) || NewLData != NULL) {
        return;
    }

    DOOR_s *doors = WORLD->doors;
    if (doors == NULL || (object->apiobj.field_0x1f4 & APIOBJECT_STATE_FLAG_IGNORE_DOORS) != 0) {
        return;
    }

    const f32 centre_height = (object->character_bottom + object->character_top) * object->apiobj.field_0xa8 * 0.5f;
    NUVEC previous_position = object->apiobj.start_position;
    previous_position.y += centre_height;
    NUVEC current_position = object->apiobj.position;
    current_position.y += centre_height;

    for (i32 index = 0; index < WORLD->door_count; ++index) {
        DOOR_s *door = &doors[index];
        if ((door->flags & DOOR_FLAG_DO_NOT_USE) != 0 || door->active != 0 ||
            (Mission_Active(NULL) != NULL && door->level != world->level_idx)) {
            continue;
        }

        NUVEC previous_offset;
        NUVEC current_offset;
        NuVecSub(&previous_offset, &previous_position, &door->pos);
        NuVecSub(&current_offset, &current_position, &door->pos);
        const f32 previous_distance = NuVecDot(&door->normal, &previous_offset);
        const f32 current_distance = NuVecDot(&door->normal, &current_offset);

        i32 crossing_direction = 0;
        if (previous_distance < 0.0f && current_distance >= 0.0f) {
            crossing_direction = 1;
        } else if ((door->flags & DOOR_FLAG_ONE_WAY) == 0 && previous_distance >= 0.0f && current_distance < 0.0f) {
            crossing_direction = 2;
        } else {
            continue;
        }

        const f32 interpolation = NuFabs(previous_distance) / (NuFabs(current_distance) + NuFabs(previous_distance));
        NUVEC intersection;
        intersection.x = previous_position.x + (current_position.x - previous_position.x) * interpolation;
        intersection.y = previous_position.y + (current_position.y - previous_position.y) * interpolation;
        intersection.z = previous_position.z + (current_position.z - previous_position.z) * interpolation;

        if (NuPtInPoly(&intersection, &door->point0, &door->point1, &door->point3, &door->plane) == 0 &&
            NuPtInPoly(&intersection, &door->point1, &door->opposite_midpoint, &door->point3, &door->plane) == 0) {
            continue;
        }

        if ((object->apiobj.flags_low & APIOBJECT_FLAG_PLAYER_ACTIVE) != 0) {
            Door_GoThrough(world, door, 1);
            return;
        }

        if (NuFabs(door->normal.y) >= NuTrigTable[0xaaa]) {
            return;
        }

        object->apiobj.position = object->apiobj.start_position;
        NUVEC block_normal = door->normal;
        if (crossing_direction == 2) {
            NuVecRotateY(&block_normal, &block_normal, 0x8000);
        }
        object->apiobj.field_0x1fc -= block_normal.x * DOOR_BLOCK_VELOCITY;
        object->apiobj.field_0x204 -= block_normal.z * DOOR_BLOCK_VELOCITY;
        return;
    }
}

DOOR_s *Door_FindByIndex(WORLDINFO_s *world, i32 a, i32 b, NUVEC *c) {
    DOOR_s *d = world->doors;
    DOOR_s *out;
    out = NULL;
    if (d != NULL) {
        i32 count = world->door_count;
        if (count > 0) {
            if (a == -1) {
                if (b != -1) {
                    if (c != NULL) {
                        f32 best = 1000000.0f;
                        struct DOOR_s *bestDoor = NULL;
                        i32 i = 0;
                        do {
                            if (((d->flags & 4) == 0) && d->level != -1 && d->level == b) {
                                f32 dist = NuVecDistSqr((NUVEC *)c, &d->pos, NULL);
                                count = world->door_count;
                                if (dist < best) {
                                    best = dist;
                                    bestDoor = d;
                                }
                            }
                            i++;
                            d++;
                        } while (i < count);
                        return bestDoor;
                    }
                    i32 i = 0;
                    do {
                        if (((d->flags & 4) == 0) && d->level != -1 && d->level == b) {
                            return d;
                        }
                        i++;
                        d++;
                    } while (i != count);
                }
            } else {
                if (c != NULL) {
                    if (b != -1) {
                        f32 best = 1000000.0f;
                        struct DOOR_s *bestDoor = NULL;
                        i32 i = 0;
                        do {
                            i16 s = d->level;
                            if (((d->flags & 4) == 0) && s != -1 && (s == b || LDataList[s].area_index == a)) {
                                f32 dist = NuVecDistSqr((NUVEC *)c, &d->pos, NULL);
                                count = world->door_count;
                                if (dist < best) {
                                    best = dist;
                                    bestDoor = d;
                                }
                            }
                            i++;
                            d++;
                        } while (i < count);
                        return bestDoor;
                    }
                    f32 best = 1000000.0f;
                    struct DOOR_s *bestDoor = NULL;
                    i32 i = 0;
                    do {
                        if (((d->flags & 4) == 0) && d->level != -1 && LDataList[d->level].area_index == a) {
                            f32 dist = NuVecDistSqr((NUVEC *)c, &d->pos, NULL);
                            count = world->door_count;
                            if (dist < best) {
                                best = dist;
                                bestDoor = d;
                            }
                        }
                        i++;
                        d++;
                    } while (i < count);
                    return bestDoor;
                }
                if (b == -1) {
                    i32 i = 0;
                    do {
                        if (((d->flags & 4) == 0) && d->level != -1 && LDataList[d->level].area_index == a) {
                            return d;
                        }
                        i++;
                        d++;
                    } while (i != count);
                } else {
                    i32 i = 0;
                    do {
                        i16 s = d->level;
                        if ((d->flags & 4) == 0 && s != -1) {
                            if (s == b) {
                                return d;
                            }
                            if (LDataList[s].area_index == a) {
                                return d;
                            }
                        }
                        i++;
                        d++;
                    } while (i != count);
                }
            }
        }
    }
    return out;
}

ADDGIZMOTYPE *Door_RegisterGizmo(i32 type_id) {
    static ADDGIZMOTYPE addtype;

    addtype = Default_ADDGIZMOTYPE;
    addtype.name = "Door";
    addtype.prefix = "";
    addtype.fns.unknown1 = 0;
    addtype.fns.early_update_fn = NULL;
    addtype.fns.panel_draw_fn = NULL;
    addtype.fns.get_visibility_fn = NULL;
    addtype.fns.get_max_gizmos_fn = Door_GetMaxGizmos;
    addtype.fns.get_pos_fn = NULL;
    addtype.fns.using_special_fn = NULL;
    addtype.fns.add_gizmos_fn = Door_AddGizmos;
    addtype.fns.bolt_hit_plat_fn = NULL;
    addtype.fns.get_best_bolt_target_fn = NULL;
    addtype.fns.late_update_fn = NULL;
    addtype.fns.bolt_hit_fn = NULL;
    addtype.fns.draw_fn = NULL;
    addtype.fns.get_gizmo_name_fn = Door_GetGizmoName;
    addtype.fns.get_output_fn = Door_GetOutput;
    addtype.fns.get_output_name_fn = Door_GetOutputName;
    addtype.fns.get_num_outputs_fn = Door_GetNumOutputs;
    addtype.fns.activate_fn = Door_Activate;
    addtype.fns.activate_rev_fn = NULL;
    addtype.fns.set_visibility_fn = NULL;
    addtype.fns.allocate_progress_data_fn = NULL;
    addtype.fns.clear_progress_fn = NULL;
    addtype.fns.store_progress_fn = NULL;
    addtype.fns.reset_fn = NULL;
    addtype.fns.reserve_buffer_space_fn = NULL;
    addtype.fns.load_fn = NULL;
    addtype.fns.post_load_fn = NULL;
    addtype.fns.add_level_sfx_fn = NULL;
    door_gizmotype_id = (char)type_id;

    return &addtype;
}
