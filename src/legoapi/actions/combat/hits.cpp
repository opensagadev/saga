#include "decomp.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nufloat.h"

#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/render/core/terrain.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/audio/audio.h"
#include <stdio.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

i32 ForcePushed_SuperPush_Occurring(GameObject_s *first, GameObject_s *second);
void StartFlatten(GameObject_s *source, GameObject_s *target);

i32 objhitobj_nohurtsfx;
i32 objhitobj_noattackerrumble;
i32 objhitobj_throwkillpartsup;
extern i32 objhitobj_noimpactsfx;
extern i16 *objhitobj_killparts_yrot;
extern BOLT_s *objhitobj_bolt;
extern i32 ObstacleCamHoldUntilPlayersMove;
extern i32 disable_narrow_socks, players_cannot_exit_speeder, BuildUpDone;
extern f32 BuildUpScale, DrawBuildUpTime, builduptime;
extern i32 MiniCutCam;
f32 SHIELDFLICKERTIME = 0.1f;
void AlertSurroundingCreatures(GameObject_s *, NUVEC *);
i32 Hub_InMenu();
i32 RotDiff(u16, u16);
void NewRumble(nupad_s *, f32, i32);
i32 NewBlockAction(GameObject_s *);
void PlayerTakeHit(GameObject_s *, GameObject_s *);
void ReleaseForce(GameObject_s *, i32);
void LoseHelmet(GameObject_s *, i32, i32);
void TakeHitRumble(GameObject_s *, f32);
void KillRumble(GameObject_s *);
void PlayHurtSfx(GameObject_s *);
void SnakeBeenHit(GameObject_s *);
void PopBalloon(GameObject_s *);
i32 Player_HasFastBuild(GameObject_s *);
i32 Player_HasInvincibility(GameObject_s *);
i32 ReleaseHearts();
void KillParts(GameObject_s *, i32, i32, i32, f32, i32, u16 *);
void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);
void Arcade_Kill(i32, i32);
i32 qrand();
GAMEPAD_s *ViewCamGetGamePad();
void Player_ClearContext(GameObject_s *, i32);
extern "C" i32 AddGameDebris(APIDEBRISSYS_s *, i32, NUVEC *);
extern "C" void NuSpecialSetVisibility(void *, i32);

extern i16 id_ANAKINJEDI;
extern i16 id_ATAT;
extern i16 id_BODYGUARD;
extern i16 id_DRAGBOMB;
extern i16 id_GAMORREANGUARD;
extern i16 id_IMPERIALGUARD;
extern i16 id_OBIWANKENOBIEP3;
extern i16 id_ROYALGUARD;
extern i16 id_SNAKE;
extern i16 id_SPEEDERBIKE;
extern i16 id_WOOKIEE;
extern AREADATA_s *HOTHBATTLE_ADATA;
extern AREADATA_s *PODRACE_ADATA;
extern AREADATA_s *PODSPRINT_ADATA;
extern AREADATA_s *SPEEDERCHASE_ADATA;
extern LEVELDATA_s *CRUISERC_LDATA;
extern LEVELDATA_s *HUB_LDATA;
extern LEVELDATA_s *SPEEDERCHASEA_LDATA;
extern LEVELDATA_s *VADERC_LDATA;

i32 CannotKill(GameObject_s *object) {
    if ((CInfo[object->character_context].flags & 0x2000000) != 0) {
        return 1;
    }
    GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    return (data->field_0x94 & 0x800) != 0 && object->apiobj.field_0x27c == -1;
}

void KillRumble(GameObject_s *object) {
    if (object != NULL) {
        if (object->apiobj.player_controlled) {
            NewRumble(object->pad_gamepad->pad, 0.7f, 0);
            NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
        }
    }
}

void FloatRumble(GameObject_s *object) {
    if (object != NULL && object->apiobj.player_controlled) {
        const f32 strength = MIN(1.0f, NuFabs(object->apiobj.velocity.y) /
                                               object->apiobj.character_data->game_character->movement_speed * 0.15f +
                                           0.25f);
        NewRumble(object->pad_gamepad->pad, strength, 0);
        NewBuzzFrames(object->pad_gamepad->pad, 1, 0);
    }
}

i16 InsideLineF(f32 point_u, f32 point_v, f32 line_start_u, f32 line_start_v, f32 line_end_u, f32 line_end_v) {
    const f32 side =
        (point_u - line_start_u) * (line_end_v - line_start_v) + (point_v - line_start_v) * (line_start_u - line_end_u);
    if (side < 0.0f) {
        return 0;
    }
    return 1;
}

i16 InsideLineXZ(f32 point_u, f32 point_v, f32 line_start_u, f32 line_start_v, f32 line_end_u, f32 line_end_v) {
    const f32 side =
        (point_u - line_start_u) * (line_end_v - line_start_v) + (point_v - line_start_v) * (line_start_u - line_end_u);
    return side >= 0.0f;
}

i16 InsidePolLines(f32 point_x, f32 point_y, f32 point_z, f32 edge_a_x, f32 edge_a_y, f32 edge_a_z, f32 edge_b_x,
                   f32 edge_b_y, f32 edge_b_z, NUVEC *normal) {
    // Project onto the plane perpendicular to the dominant normal component.
    // The call order preserves the polygon winding in that projection.
    if (NuFabs(normal->y) >= NuFabs(normal->x) && NuFabs(normal->y) >= NuFabs(normal->z)) {
        if (0.0f > normal->y) {
            if (InsideLineF(point_x, point_z, 0.0f, 0.0f, edge_a_x, edge_a_z) == 0 ||
                InsideLineF(point_x, point_z, edge_b_x, edge_b_z, 0.0f, 0.0f) == 0) {
                return 0;
            }
            return InsideLineF(point_x, point_z, edge_a_x, edge_a_z, edge_b_x, edge_b_z) != 0;
        }

        if (InsideLineF(point_x, point_z, edge_a_x, edge_a_z, 0.0f, 0.0f) == 0 ||
            InsideLineF(point_x, point_z, 0.0f, 0.0f, edge_b_x, edge_b_z) == 0) {
            return 0;
        }
        return InsideLineF(point_x, point_z, edge_b_x, edge_b_z, edge_a_x, edge_a_z) != 0;
    }

    if (NuFabs(normal->y) > NuFabs(normal->x) || NuFabs(normal->z) > NuFabs(normal->x)) {
        if (0.0f > normal->z) {
            if (InsideLineF(point_y, point_x, 0.0f, 0.0f, edge_a_y, edge_a_x) == 0 ||
                InsideLineF(point_y, point_x, edge_b_y, edge_b_x, 0.0f, 0.0f) == 0) {
                return 0;
            }
            return InsideLineF(point_y, point_x, edge_a_y, edge_a_x, edge_b_y, edge_b_x) != 0;
        }

        if (InsideLineF(point_y, point_x, edge_a_y, edge_a_x, 0.0f, 0.0f) == 0 ||
            InsideLineF(point_y, point_x, 0.0f, 0.0f, edge_b_y, edge_b_x) == 0) {
            return 0;
        }
        return InsideLineF(point_y, point_x, edge_b_y, edge_b_x, edge_a_y, edge_a_x) != 0;
    }

    if (0.0f > normal->x) {
        if (InsideLineF(point_y, point_z, edge_a_y, edge_a_z, 0.0f, 0.0f) == 0 ||
            InsideLineF(point_y, point_z, 0.0f, 0.0f, edge_b_y, edge_b_z) == 0) {
            return 0;
        }
        return InsideLineF(point_y, point_z, edge_b_y, edge_b_z, edge_a_y, edge_a_z) != 0;
    }

    if (InsideLineF(point_y, point_z, 0.0f, 0.0f, edge_a_y, edge_a_z) == 0 ||
        InsideLineF(point_y, point_z, edge_b_y, edge_b_z, 0.0f, 0.0f) == 0) {
        return 0;
    }
    return InsideLineF(point_y, point_z, edge_a_y, edge_a_z, edge_b_y, edge_b_z) != 0;
}

i32 ObjHitObj_Flags(GameObject_s *object) {
    if (object == NULL)
        return 0;
    const bool player = object->apiobj.player_controlled;
    const u16 ordinary = player ? 0x80c : 0x00a;
    const u16 special = player ? 0x824 : 0x022;
    const u16 scripted = player ? 0x804 : 0x002;
    if ((object->apiobj.field_0x1f4 & 0x10001) != 0)
        return scripted | 0x10;
    return (object->apiobj.field_0x1f4 & 4) != 0 ? special : ordinary;
}

bool CalculateRayBoxIntersection(VuVec const &minimum, VuVec const &maximum, VuVec const &start, VuVec const &direction,
                                 float maximum_distance, float &distance) {
    f32 near_distance;
    f32 far_distance;
    if (direction.x >= 0.0f) {
        near_distance = (minimum.x - start.x) / direction.x;
        far_distance = (maximum.x - start.x) / direction.x;
    } else {
        near_distance = (maximum.x - start.x) / direction.x;
        far_distance = (minimum.x - start.x) / direction.x;
    }

    f32 axis_near_distance;
    f32 axis_far_distance;
    if (direction.y >= 0.0f) {
        axis_near_distance = (minimum.y - start.y) / direction.y;
        axis_far_distance = (maximum.y - start.y) / direction.y;
    } else {
        axis_near_distance = (maximum.y - start.y) / direction.y;
        axis_far_distance = (minimum.y - start.y) / direction.y;
    }

    if (near_distance > axis_far_distance || axis_near_distance > far_distance) {
        return false;
    }
    near_distance = MAX(axis_near_distance, near_distance);
    far_distance = MIN(axis_far_distance, far_distance);

    if (direction.z >= 0.0f) {
        axis_near_distance = (minimum.z - start.z) / direction.z;
        axis_far_distance = (maximum.z - start.z) / direction.z;
    } else {
        axis_near_distance = (maximum.z - start.z) / direction.z;
        axis_far_distance = (minimum.z - start.z) / direction.z;
    }

    if (near_distance > axis_far_distance || axis_near_distance > far_distance) {
        return false;
    }
    far_distance = MIN(axis_far_distance, far_distance);
    near_distance = MAX(axis_near_distance, near_distance);
    distance = near_distance;
    return maximum_distance > near_distance && far_distance > 0.0f;
}

f32 CalcCapsuleIntersectDistance(VuVec const &start, VuVec const &direction, f32 maximum_distance, VuVec const &centre,
                                 f32 radius) {
    const f32 delta_x = centre.x - start.x;
    const f32 delta_y = centre.y - start.y;
    const f32 delta_z = centre.z - start.z;
    f32 distance = direction.x * delta_x + direction.y * delta_y + direction.z * delta_z;

    if (distance >= 0.0f && distance < maximum_distance) {
        const f32 distance_from_axis_squared =
            delta_x * delta_x + delta_y * delta_y + delta_z * delta_z - distance * distance;
        if (radius * radius >= distance_from_axis_squared) {
            return distance;
        }
    }
    return 1.0e9f;
}

i32 CheckCol(nutex_s *, i32, i32, i32, i32) {
    return true;
}

void HitRumble(GameObject_s *object) {
    if (object != NULL) {
        if (object->apiobj.player_controlled) {
            NewRumble(object->pad_gamepad->pad, 0.5f, 0);
            NewBuzzFrames(object->pad_gamepad->pad, 2, 0);
        }
    }
}
