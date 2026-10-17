// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/items/collect/minikits.h"
#include "legoapi/world/areas.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/render.h"

#include "batman.h"
#include "decomp.h"
#include "gameapi/edtools/edfile.h"
#include "globals.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nurndr.h"
#include <string.h>
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx/game_deb.h"
#include <stdio.h>
#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/ai/game/creature.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/props/system/socksys.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nusound/nusound.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/edtools/edgra.h"
#include <stdlib.h>
#include "legoapi/props/doors/door.h"
#include "legoapi/render/fx/edsplines.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nucore/nupad.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/core/input/gamepads.h"
#include "nu2api/nu3d/nurndrstat.h"
#include <float.h>
#include <math.h>
#include "legoapi/render/core/terrain_internal.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include <stddef.h>
#include "legoapi/core/startup/text.h"

f32 COINMAGNETSCALE = 3.0f;
f32 COINMSGTIME = 1.0f;
f32 (*GizmoPickups_Collide2DFn)(GameObject_s *) = NULL;
static GIZMOPICKUPSYS_s *GizmoPickupSys = &GizmoPickupSys_Game;

namespace {

    enum : i32 {
        GIZMOPICKUP_PROGRESS_CAPACITY = 512,
        GIZMOPICKUP_PROGRESS_WORDS = GIZMOPICKUP_PROGRESS_CAPACITY / 32,
        GIZMOPICKUP_TEMPORARY_CAPACITY = 64,
    };

    struct GIZMOPICKUPPROGRESS_s {
        u32 collected[GIZMOPICKUP_PROGRESS_WORDS];
        u32 enabled[GIZMOPICKUP_PROGRESS_WORDS];
        u32 visible[GIZMOPICKUP_PROGRESS_WORDS];
        u32 activated[GIZMOPICKUP_PROGRESS_WORDS];
    };
    DECOMP_ASSERT(sizeof(GIZMOPICKUPPROGRESS_s) == 0x100, "GIZMOPICKUP progress ABI");

    i32 FindPickupTypeIndex(char type_code) {
        for (i32 index = 0; index < GizmoPickupSys->type_count; ++index) {
            if (GizmoPickupSys->types[index].type_code == type_code) {
                return index;
            }
        }
        return 0;
    }

    // Sets AreaPickupScale from the current area's flags and returns it.
    f32 UpdateAreaPickupScale(const WORLDINFO *world) {
        AreaPickupScale = 1.0f;
        if (world->level_sub_id >= 0 && world->level_sub_id < AREACOUNT) {
            if ((ADataList[world->level_sub_id].flags & AREAFLAG_NOPICKUPGRAVITY) != 0) {
                AreaPickupScale = 6.0f;
            } else if ((ADataList[world->level_sub_id].flags & AREAFLAG_VEHICLE_AREA) != 0) {
                if (BonusArea != 0) {
                    AreaPickupScale = 3.0f;
                } else {
                    AreaPickupScale = 5.0f;
                }
            }
        }
        return AreaPickupScale;
    }

    void ClearPickupProgress(GIZMOPICKUPPROGRESS_s *progress) {
        if (progress == NULL) {
            return;
        }
        memset(progress->visible, 0xff, sizeof(progress->visible));
        memset(progress->enabled, 0xff, sizeof(progress->enabled));
        memset(progress->collected, 0, sizeof(progress->collected));
        memset(progress->activated, 0, sizeof(progress->activated));
    }

} // namespace

static __attribute__((noinline, noclone)) void GizmoPickups_UpdateListBody(WORLDINFO *world, GIZMOPICKUP_s *pickups, i32 count, i32 play_nearby_sfx)
    __asm__("_ZL23GizmoPickups_UpdateListP11WORLDINFO_sP13GIZMOPICKUP_sii.part.9");

static inline void GizmoPickups_UpdateList(WORLDINFO *world, GIZMOPICKUP_s *pickups, i32 count, i32 play_nearby_sfx) {
    if (world == NULL || pickups == NULL) {
        return;
    }
    GizmoPickups_UpdateListBody(world, pickups, count, play_nearby_sfx);
}

i32 gizmopickup_typeid = -1;
f32 AreaPickupScale;
i32 PickUpFlickerTest = 3;
i32 PickUpFlickerFrames = 6;
i32 PickupFlickerFrame;

static void GizmoPickups_Update(void *world_ptr, void *, float);

extern WORLDINFO WorldInfo[2];

struct GIZMOPICKUP_s;
struct PART_s;

static __used__ void Collection_GetSelectingPlayerIDs(i16 *ids);

namespace {

} // namespace

// Retail only has the constant-propagated clone of
// DrawHitPoints(GameObject_s *, float, float, float, float, i32, float, i32); every caller passes 0.0f and 0
// for the two trailing (unused) arguments, so the clone drops them.
static __attribute__((noinline, noclone, hot, section(".text"))) void DrawHitPoints(GameObject_s *object, float x, float y, float scale,
                                                             float alpha, i32 alignment)
    __asm__("_ZL13DrawHitPointsP12GameObject_sffffifi.constprop.39");

static __used__ void PartImpact_Coin(PART_s *part);
static __used__ void PartStolen_Coin(PART_s *part);
static __used__ void PartExtra_BlueCoin(PART_s *part);
static __used__ void PartExtra_PurpleCoin(PART_s *part);
static __used__ i32 PowerUp_DrawPart(PART_s *part);
static __used__ void PowerUp_ImpactPart(PART_s *part);
static __used__ void PowerUp_UpdatePart(PART_s *part);
static __used__ void PowerUp_EndMsg(GAMEMESSAGE_s *message);
static __used__ void PowerUp_UpdateMsg(GAMEMESSAGE_s *message);
static __used__ i32 PartDraw_VehicleHeart(PART_s *part);
static __used__ void PartMove_VehicleHeart(PART_s *part, f32);
static __used__ void PartMove_VehiclePickup(PART_s *part, f32);

// NuFileExists @nufile (nucore_plain.cpp).

static i32 GizmoPickups_GetMaxGizmos(void *pickup) {
    WORLDINFO *world = static_cast<WORLDINFO *>(pickup);
    return world != NULL ? world->current_level->max_pickups : 0;
}

static char *GizmoPickup_GetGizmoName(GIZMO *gizmo) {
    GIZMOPICKUP_s *pickup = gizmo != NULL ? static_cast<GIZMOPICKUP_s *>(gizmo->object) : NULL;
    return pickup != NULL ? pickup->name : NULL;
}

static i32 GizmoPickup_GetOutput(GIZMO *gizmo, i32, i32) {
    GIZMOPICKUP_s *pickup = static_cast<GIZMOPICKUP_s *>(gizmo->object);
    return (pickup->state_flags & GIZMOPICKUP_STATE_COLLECTED) != 0;
}

static char *GizmoPickup_GetOutputName(GIZMO *, i32) {
    return const_cast<char *>("Collected");
}

static i32 GizmoPickup_GetNumOutputs(GIZMO *) {
    return 1;
}

static void GizmoPickup_SetVisibility(GIZMO *gizmo, i32 visible) {
    if (gizmo == NULL) {
        return;
    }
    GIZMOPICKUP_s *pickup = static_cast<GIZMOPICKUP_s *>(gizmo->object);
    pickup->state_visible = visible != 0;
}

static NUVEC *GizmoPickup_GetPos(GIZMO *gizmo) {
    if (gizmo != NULL) {
        GIZMOPICKUP_s *pickup = static_cast<GIZMOPICKUP_s *>(gizmo->object);
        return &pickup->position;
    }
    return NULL;
}

void GizmoPickups_PostLoad(void *, void *) {
}

static void GizmoPickups_ClearProgress(void *, void *progress_ptr) {
    ClearPickupProgress(static_cast<GIZMOPICKUPPROGRESS_s *>(progress_ptr));
}

static void GameMsg_Draw_MiniKitDetector(GAMEMESSAGE_s *message, nuvec_s *position, float scale) {
    NUVEC screen = *position;
    i32 clipped = 0;
    if (message->field_0xfb != 0) {
        NuVecNeg(&screen, &screen);
        screen.x = screen.x < 0.0f ? -0.825f : 0.825f;
        if (screen.y < 0.0f)
            screen.y = -0.825f;
        else
            screen.y = 0.825f;
        clipped = 1;
    }

    f32 excess = 1.0e9f;
    if (screen.x >= 0.825f) {
        excess = screen.x - 0.825f;
        screen.x = 0.825f;
        NuStrCpy(message->text_buffer, ">");
        clipped = 1;
    } else if (screen.x <= -0.825f) {
        excess = -0.825f - screen.x;
        screen.x = -0.825f;
        NuStrCpy(message->text_buffer, "<");
        clipped = 1;
    }
    if (screen.y >= 0.825f) {
        const f32 y = screen.y;
        screen.y = 0.825f;
        if (clipped == 0 || (excess != 1.0e9f && y - 0.825f > excess))
            NuStrCpy(message->text_buffer, LEGOASCII_UP != NULL ? LEGOASCII_UP : "?");
        clipped = 1;
    } else if (screen.y <= -0.825f) {
        const f32 y = screen.y;
        screen.y = -0.825f;
        if (clipped == 0 || (excess != 1.0e9f && -0.825f - y > excess))
            NuStrCpy(message->text_buffer, LEGOASCII_DOWN != NULL ? LEGOASCII_DOWN : "?");
        clipped = 1;
    }

    f32 distance = NuVecDist(&message->position, &GameCam->pos, NULL) /
                   static_cast<f32>(static_cast<u32>(
                       static_cast<u8>(WorldInfo_CurrentlyActive()->current_level->camera_judder_distance)));
    if (distance > 1.0f)
        distance = 1.0f;
    i32 alpha = 64;
    if (distance < 0.25f)
        alpha = 128;
    else if (distance < 0.75f)
        alpha = static_cast<i32>((1.0f - 2.0f * (distance - 0.25f)) * 64.0f + 64.0f);

    if (clipped != 0) {
        message->red = 0xff;
        message->green = message->field_0xfb == 0 ? 0xff : 0;
        message->blue = message->green;
        message->alpha = static_cast<u8>(alpha);
        message->field_0xfc = 0;
    } else {
        message->red = 0;
        message->green = 0xff;
        message->blue = 0;
        message->alpha = static_cast<u8>(alpha);
        message->field_0xfc = 4;
    }

    if (message->icon != static_cast<u16>(-1)) {
        LEVEL_OBJECT_RUNTIME *icon = &WorldInfo_CurrentlyActive()->lev_objs[static_cast<i16>(message->icon)];
        if (icon->active != 0) {
            f32 opacity = 1.0f;
            if ((message->flags & 0x10000) != 0)
                opacity = static_cast<f32>(static_cast<u32>(message->alpha)) * (1.0f / 128.0f);
            DrawPanel3DObject(screen.x, screen.y, screen.z, scale, scale, scale, 0, 0, 0, &icon->special, 2, opacity);
        }
        return;
    }
    if (message->field_0xe4 != 0) {
        DrawCharIcon(message->field_0xe4, screen.x, screen.y, 0.0f, ICONSIZE, LEGOOBJ_ICON_FRAME_NEUTRAL, 1.0f, 1.0f, 1,
                     NULL);
        return;
    }
    Text3DEx(message->text != NULL ? message->text : message->text_buffer, screen.x, screen.y, screen.z, scale, scale,
             scale, message->field_0xfc, message->red, message->green, message->blue, message->alpha);
}

static GIZMOPICKUP_s *GizmoPickups_CollideList(GameObject_s *object, GIZMOPICKUP_s *pickups, i32 count) {
    if (pickups == NULL) {
        return NULL;
    }

    i32 collide_2d = 0;
    if ((object->apiobj.character_data->model_flags & 0x2000) != 0) {
        collide_2d = VehicleArea != 0;
    }

    f32 collide_scale = 0.0f;
    if (GizmoPickups_Collide2DFn != NULL) {
        collide_scale = GizmoPickups_Collide2DFn(object);
        if (collide_scale != 0.0f) {
            collide_2d = 1;
        }
    }

    const i32 coin_magnet = Cheats_CheckFlags(0x8000) != 0 || object->field_0xdec > 0.0f;
    f32 scaled_pickup = coin_magnet ? COINMAGNETSCALE * AreaPickupScale : AreaPickupScale;
    const f32 normal_pickup = AreaPickupScale;
    if (collide_scale != 0.0f) {
        scaled_pickup *= collide_scale;
    }
    if (collide_2d && VehicleArea != 0) {
        scaled_pickup += scaled_pickup;
    }

    for (i32 index = 0; index < count; ++index) {
        GIZMOPICKUP_s *pickup = &pickups[index];
        GIZMO_PICKUP_TYPE *type = &GizmoPickupSys->types[pickup->type_index];
        if ((type->flags & GIZMOPICKUP_TYPE_COLLISION_FILTER) != 0 ||
            (pickup->state_flags &
             (GIZMOPICKUP_STATE_ACTIVE | GIZMOPICKUP_STATE_ENABLED | GIZMOPICKUP_STATE_COLLECTED)) !=
                (GIZMOPICKUP_STATE_ACTIVE | GIZMOPICKUP_STATE_ENABLED) ||
            ((pickup->state_flags & GIZMOPICKUP_STATE_DRAWN) == 0 && (pickup->config_flags & 4) == 0)) {
            continue;
        }

        if ((pickup->state_flags & GIZMOPICKUP_STATE_ALTERNATE_TYPE) != 0 && GizmoPickupSys->alternate_type != -1) {
            type = &GizmoPickupSys->types[GizmoPickupSys->alternate_type];
        }
        if (type->field_0x0f != 0 && pickups != WorldInfo_CurrentlyActive()->gizmo_pickup_sys->temporary_pickups) {
            continue;
        }
        if ((pickup->config_flags & 4) != 0) {
            return pickup;
        }

        f32 scale = type->score == 0 ? normal_pickup : scaled_pickup;
        f32 radius_x = type->shadow_extent_x * scale;
        f32 radius_y = scale * (coin_magnet ? type->shadow_extent_z : type->shadow_radius_z);
        if (object->apiobj.collision_min.x > pickup->position.x + radius_x ||
            pickup->position.x - radius_x > object->apiobj.collision_max.x ||
            object->apiobj.collision_min.z > pickup->position.z + radius_x ||
            pickup->position.z - radius_x > object->apiobj.collision_max.z) {
            continue;
        }

        if (collide_2d) {
            f32 dx = pickup->position.x - object->apiobj.collision_position.x;
            f32 dz = pickup->position.z - object->apiobj.collision_position.z;
            f32 radius = radius_x + object->apiobj.field_0x1dc;
            if (!(dx * dx + dz * dz >= radius * radius)) {
                return pickup;
            }
        } else if (!(object->apiobj.collision_min.y > pickup->position.y + radius_y) &&
                   !(pickup->position.y - radius_y > object->apiobj.collision_max.y)) {
            if (object->field_0xcc0 == NULL) {
                if (SphereSphereOverlapScaleY(&pickup->position, radius_x, radius_y, &object->apiobj.collision_position,
                                              object->apiobj.collision_radius, object->apiobj.field_0x1e0)) {
                    return pickup;
                }
            } else {
                f32 dx = pickup->position.x - object->apiobj.collision_position.x;
                f32 dz = pickup->position.z - object->apiobj.collision_position.z;
                f32 radius = radius_x + object->apiobj.field_0x1dc;
                if (!(dx * dx + dz * dz >= radius * radius)) {
                    return pickup;
                }
            }
        }
    }
    return NULL;
}

static void *GizmoPickups_AllocateProgressData(VARIPTR *buffer, VARIPTR *buffer_end) {
    return GizmoBufferAlloc(buffer, buffer_end, sizeof(GIZMOPICKUPPROGRESS_s));
}

static void GizmoPickup_Activate(GIZMO *gizmo, i32 activate) {
    WORLDINFO *world = WorldInfo_CurrentlyActive();
    if (gizmo == NULL) {
        return;
    }

    GIZMOPICKUP_s *pickup = static_cast<GIZMOPICKUP_s *>(gizmo->object);
    if (activate == 0) {
        pickup->state_enabled = 0;
    } else {
        pickup->state_enabled = 1;
        SuperCounter_ActivateGizmoPickup(gizmo, pickup);
        if (pickup->state_enabled) {
            pickup->state_activated = 1;
            GIZMO_PICKUP_TYPE *type;
            if (!pickup->state_alternate_type || GizmoPickupSys->alternate_type == -1) {
                type = &GizmoPickupSys->types[pickup->type_index];
            } else {
                type = &GizmoPickupSys->types[GizmoPickupSys->alternate_type];
            }
            if ((ChallengeMode != 0 ? (type->flags & GIZMOPICKUP_TYPE_CHALLENGE_MODE_FILTER) != 0
                                    : (type->flags & GIZMOPICKUP_TYPE_CHALLENGE_MODE_FILTER) == 0) &&
                Mission_Active(NULL) == NULL) {
                if (type->activation_sfx_name != NULL) {
                    PlaySfx(type->activation_sfx_name, &pickup->position);
                }
                if (type->debris_id != -1) {
                    if (pickup->state_alternate_type) {
                        AddGameDebris(world->debris_sys, GizmoPickupSys->types[GizmoPickupSys->gizmo_type_id].debris_id,
                                      &pickup->position);
                    } else {
                        AddGameDebris(world->debris_sys, type->debris_id, &pickup->position);
                    }
                }
            }
        }
    }

    const u8 group = pickup->activation_group;
    if (group == 0) {
        return;
    }
    GIZMOPICKUP_s *group_pickup = world->gizmo_pickup_sys->pickups;
    for (i32 index = 0; index < world->gizmo_pickup_sys->pickup_count; ++index, ++group_pickup) {
        if (group_pickup->activation_group == group) {
            group_pickup->state_enabled = activate;
            group_pickup->state_visible = activate;
        }
    }
}

static void GizmoPickups_AddGizmos(GIZMOSYS *gizmo_sys, i32 type_id, void *data, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(data);
    for (i32 index = 0; index < world->gizmo_pickup_sys->pickup_count; ++index) {
        GIZMOPICKUP_s &pickup = world->gizmo_pickup_sys->pickups[index];
        if ((pickup.config_flags & GIZMOPICKUP_CONFIG_REGISTER_GIZMO) != 0) {
            AddGizmo(gizmo_sys, type_id, NULL, &pickup);
        }
    }
}

static void GizmoPickups_Reset(void *world_ptr, void *, void *progress_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    GIZMOPICKUPPROGRESS_s *progress = static_cast<GIZMOPICKUPPROGRESS_s *>(progress_ptr);

    for (i32 index = 0; index < GizmoPickupSys->type_count; ++index) {
        GIZMO_PICKUP_TYPE &type = GizmoPickupSys->types[index];
        type.shadow_extent_x = type.shadow_radius_x * type.draw_distance;
        type.shadow_extent_z = type.shadow_radius_z * type.draw_distance;
    }
    if (world == NULL) {
        return;
    }

    GIZMOPICKUP_s *pickup_cursor = world->gizmo_pickup_sys->pickups;
    if (pickup_cursor != NULL) {
        for (i32 index = 0; index < world->gizmo_pickup_sys->pickup_count; ++index, ++pickup_cursor) {
            GIZMOPICKUP_s &pickup = *pickup_cursor;
            pickup.type_index = static_cast<u8>(FindPickupTypeIndex(pickup.type_code));

            NewTerrPlatformsOff();
            pickup.floor_height = GameShadow(NULL, &pickup.position, 5.0f, -1);
            if (pickup.floor_height != 2000000.0f) {
                if (!(pickup.floor_height >= pickup.position.y)) {
                    FindAnglesZX(&ShadNorm, &pickup.shadow_x_rotation, &pickup.shadow_z_rotation);
                } else {
                    pickup.floor_height = 2000000.0f;
                }
            }

            pickup.state_active = 1;
            pickup.state_enabled = 1;
            pickup.state_visible = 1;
            pickup.state_collected = 0;
            pickup.state_drawn = 0;
            if (GizmoPickupSys->types[pickup.type_index].random_model_count != 0) {
                pickup.model_variant = static_cast<u8>(
                    qrand() / (0xffff / GizmoPickupSys->types[pickup.type_index].random_model_count + 1));
            } else {
                pickup.model_variant = 0;
            }
            pickup.room_index = static_cast<i8>(
                world->current_gscn != NULL ? NuPortalWhichRoom(world->current_gscn, &pickup.position) : -1);
            pickup.draw_rotation = static_cast<u16>(qrand());
            pickup.state_draw_visible = 1;
            pickup.state_alternate_type = 0;
            pickup.remaining_visible_time = 0.0f;

            if (index < GIZMOPICKUP_PROGRESS_CAPACITY && progress != NULL) {
                const i32 word = index >> 5;
                const u32 bit = 1u << (index & 31);
                pickup.state_visible = (progress->visible[word] & bit) != 0;
                pickup.state_enabled = (progress->enabled[word] & bit) != 0;
                pickup.state_collected = (progress->collected[word] & bit) != 0;
                pickup.state_activated = (progress->activated[word] & bit) != 0;
                if ((pickup.config_flags & GIZMOPICKUP_CONFIG_REQUIRES_ACTIVATION) != 0 && !pickup.state_activated) {
                    pickup.state_enabled = 0;
                    pickup.state_visible = 0;
                }
            } else if ((pickup.config_flags & GIZMOPICKUP_CONFIG_REQUIRES_ACTIVATION) != 0) {
                pickup.state_enabled = 0;
                pickup.state_visible = 0;
            }
            if (GizmoPickupSys->gizmo_type_id != -1 && pickup.type_index == GizmoPickupSys->gizmo_type_id &&
                Game_LevelSave != NULL) {
                if (SuperStory != 0) {
                    pickup.state_flags |= GIZMOPICKUP_STATE_ALTERNATE_TYPE;
                } else {
                    // The original reads the save entry through Game_LevelSave on every pass.
                    for (i32 saved_index = 0;
                         saved_index < reinterpret_cast<LEVELSAVE_s *>(Game_LevelSave)[world->level_idx].minikit_count;
                         ++saved_index) {
                        if (NuStrICmp(pickup.name, reinterpret_cast<LEVELSAVE_s *>(Game_LevelSave)[world->level_idx]
                                                       .minikit_names[saved_index]) == 0) {
                            pickup.state_alternate_type = 1;
                            break;
                        }
                    }
                }
            }
        }
    }

    GIZMOPICKUPRUNTIMESYS_s *pickup_sys = world->gizmo_pickup_sys;
    if (pickup_sys->temporary_pickups != NULL) {
        memset(pickup_sys->temporary_pickups, 0, sizeof(GIZMOPICKUP_s) * GIZMOPICKUP_TEMPORARY_CAPACITY);
    }
}

static void GizmoPickups_DrawList(WORLDINFO *world, GIZMOPICKUP_s *pickups, i32 count) {
    NUMTX matrix;
    if (world == NULL || pickups == NULL) {
        return;
    }

    NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f);
    f32 maximum_distance = world->gizmo_pickup_sys->draw_distance;
    f32 shadow_distance_squared;
    if (g_lowEndLevelBehaviour != 0) {
        shadow_distance_squared = 0.0f;
        if (__builtin_expect(CharClipToBlobShadows != 0 && maximum_distance > world->current_level->blob_shadow_fade_far, 0)) {
            maximum_distance = static_cast<f32>(world->current_level->blob_shadow_fade_far);
        }
    } else {
        const f32 shadow_distance = maximum_distance * 0.5f;
        shadow_distance_squared = shadow_distance * shadow_distance;
    }
    const f32 maximum_distance_squared = maximum_distance * maximum_distance;
    auto world_current_level = world->current_level;
    for (i32 index = 0; index < count; ++index) {
        GIZMOPICKUP_s &pickup = pickups[index];
        if ((pickup.state_flags & GIZMOPICKUP_STATE_ACTIVE) == 0) {
            continue;
        }
        pickup.state_flags &= static_cast<u8>(~GIZMOPICKUP_STATE_DRAWN);
        if ((pickup.state_flags & (GIZMOPICKUP_STATE_VISIBLE | GIZMOPICKUP_STATE_COLLECTED)) !=
                GIZMOPICKUP_STATE_VISIBLE ||
            (pickup.state_flags & GIZMOPICKUP_STATE_DRAW_VISIBLE) == 0) {
            continue;
        }
        if (pickup.room_index >= 0 && world->rooms_visible_ptr != NULL &&
            world->rooms_visible_ptr[pickup.room_index] == 0) {
            continue;
        }

        i32 type_index = pickup.type_index;
        if ((pickup.state_flags & GIZMOPICKUP_STATE_ALTERNATE_TYPE) != 0 && GizmoPickupSys->alternate_type >= 0) {
            type_index = GizmoPickupSys->alternate_type;
        }
        if (type_index < 0 || type_index >= GizmoPickupSys->type_count) {
            type_index = 0;
        }
        GIZMO_PICKUP_TYPE *type = &GizmoPickupSys->types[type_index];
        if (type->field_0x0f != 0 && pickups != WorldInfo_CurrentlyActive()->gizmo_pickup_sys->temporary_pickups) {
            continue;
        }
        const f32 camera_z = pickup.position.z - GameCam->pos.z;
        const f32 camera_x = pickup.position.x - GameCam->pos.x;
        if (camera_x * camera_x + camera_z * camera_z > maximum_distance_squared) {
            continue;
        }

        const i32 model_index = type->first_model_id + pickup.model_variant;
        LEVEL_OBJECT_RUNTIME_s &model = world->lev_objs[model_index];
        if (model.active != 0) {

            if ((type->flags & GIZMOPICKUP_TYPE_DRAW_TUMBLING) != 0) {
                const u16 y_rotation = pickup.draw_rotation;
                const i32 x_rotation = static_cast<i32>(NuTrigTable[y_rotation & 0x7fff] * 1820.0f);
                const f32 cos_x = NU_COS_LUT(x_rotation);
                const f32 sin_x = NU_SIN_LUT(x_rotation);
                const f32 cos_y = NU_COS_LUT(y_rotation);
                const f32 sin_y = NU_SIN_LUT(y_rotation);

                matrix.m00 = cos_y + sin_y * 0.0f;
                matrix.m01 = 0.0f;
                matrix.m02 = cos_y * 0.0f - sin_y;
                matrix.m03 = 0.0f;
                matrix.m11 = cos_x;
                matrix.m10 = sin_x * sin_y + cos_y * 0.0f;
                matrix.m12 = sin_x * cos_y - sin_y * 0.0f;
                matrix.m13 = 0.0f;
                matrix.m21 = -sin_x;
                matrix.m20 = sin_y * cos_x + cos_y * 0.0f;
                matrix.m22 = cos_y * cos_x - sin_y * 0.0f;
                matrix.m23 = 0.0f;
                matrix.m30 = sin_y * 0.0f + cos_y * 0.0f;
                matrix.m31 = 0.0f;
                matrix.m32 = cos_y * 0.0f - sin_y * 0.0f;
                matrix.m33 = 1.0f;
                NuMtxTranslate(&matrix, &pickup.position);
            } else if ((type->flags & GIZMOPICKUP_TYPE_DRAW_Y_ROTATION) != 0) {
                const u16 y_rotation = pickup.draw_rotation;
                matrix.m01 = 0.0f;
                matrix.m03 = 0.0f;
                matrix.m10 = 0.0f;
                matrix.m11 = 1.0f;
                matrix.m12 = 0.0f;
                matrix.m13 = 0.0f;
                matrix.m21 = 0.0f;
                matrix.m23 = 0.0f;
                matrix.m30 = 0.0f;
                matrix.m31 = 0.0f;
                matrix.m32 = 0.0f;
                matrix.m33 = 1.0f;
                const f32 cos_y = NU_COS_LUT(y_rotation);
                const f32 sin_y = NU_SIN_LUT(y_rotation);
                matrix.m00 = cos_y;
                matrix.m02 = -sin_y;
                matrix.m20 = sin_y;
                matrix.m22 = cos_y;
                NuMtxTranslate(&matrix, &pickup.position);
            } else {
                NuMtxSetTranslation(&matrix, &pickup.position);
            }
            if (AreaPickupScale != 1.0f) {
                NuMtxPreScaleU(&matrix, AreaPickupScale);
            }

            if (__builtin_expect((pickup.config_flags & GIZMOPICKUP_CONFIG_DISABLE_SHADOW_MAP) != 0, 1)) {
                ResetShadowMapRendering();
            }
            const i32 drawn = NuSpecialDrawAt(&model.special, &matrix);
            pickup.state_flags = static_cast<u8>((pickup.state_flags & ~GIZMOPICKUP_STATE_DRAWN) |
                                                 (drawn != 0 ? GIZMOPICKUP_STATE_DRAWN : 0));

            if (drawn != 0 && type->overlay_model_id != -1) {
                LEVEL_OBJECT_RUNTIME_s &overlay = world->lev_objs[type->overlay_model_id];
                if (overlay.active != 0) {
                    ResetShadowMapRendering();
                    NuSpecialDrawAt(&overlay.special, &matrix);
                    EnableShadowMapRendering(0);
                }
            }
            if ((pickup.config_flags & GIZMOPICKUP_CONFIG_DISABLE_SHADOW_MAP) != 0) {
                EnableShadowMapRendering(0);
            }
        }
        const f32 distance_squared = camera_x * camera_x + camera_z * camera_z;
        if (VehicleArea == 0 && pickup.floor_height != 2000000.0f && maximum_distance_squared > 0.0f &&
            distance_squared < shadow_distance_squared) {
            const f32 distance_ratio = distance_squared / shadow_distance_squared;
            NUVEC shadow_position = pickup.position;
            shadow_position.y = pickup.floor_height + 0.005f;
            const i32 opacity = static_cast<i32>(
                (1.0f - distance_ratio) * static_cast<f32>(static_cast<u8>(world_current_level->blob_shadow_alpha)));
            NuRndrAddShadow(&shadow_position, type->shadow_radius_x, opacity, pickup.shadow_x_rotation, 0,
                            pickup.shadow_z_rotation);
        }
    }
}

static void GizmoPickups_Draw(void *world_ptr, void *, float) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (world == NULL) {
        return;
    }

    EnableShadowMapRendering(0);
    if (TimingBarSet == 5) {
        TBOPENFN(const_cast<char *>("Coins"), 5);
    }

    GIZMOPICKUP_s *pickups = world->gizmo_pickup_sys->pickups;
    const i32 pickup_count = world->gizmo_pickup_sys->pickup_count;
    if (pickups != NULL) {
        if (Missions_PickupsOff(MissionSys) != 0) {
            // Mission pickups are hidden.
        } else {
            GizmoPickups_DrawList(world, pickups, pickup_count);
        }
    }
    GIZMOPICKUP_s *temporary_pickups = world->gizmo_pickup_sys->temporary_pickups;
    if (temporary_pickups != NULL && Missions_PickupsOff(MissionSys) == 0) {
        GizmoPickups_DrawList(world, temporary_pickups, GIZMOPICKUP_TEMPORARY_CAPACITY);
    }

    if (TimingBarSet == 5) {
        TBCLOSEFN(const_cast<char *>("Coins"), 5);
    }
    ResetShadowMapRendering();
}

static void GizmoPickups_StoreProgress(void *world_ptr, void *, void *progress_ptr) {
    GIZMOPICKUPPROGRESS_s *progress = static_cast<GIZMOPICKUPPROGRESS_s *>(progress_ptr);
    if (progress != NULL) {
        memset(progress->visible, 0xff, sizeof(progress->visible));
        memset(progress->enabled, 0xff, sizeof(progress->enabled));
        memset(progress->collected, 0, sizeof(progress->collected));
        memset(progress->activated, 0, sizeof(progress->activated));
    }
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (progress == NULL || world == NULL || world->gizmo_pickup_sys == NULL ||
        world->gizmo_pickup_sys->pickups == NULL) {
        return;
    }

    const i32 count = world->gizmo_pickup_sys->pickup_count;
    const GIZMOPICKUP_s *pickup = world->gizmo_pickup_sys->pickups;
    for (i32 index = 0; index < count && index < GIZMOPICKUP_PROGRESS_CAPACITY; ++index, ++pickup) {
        const i32 word = index >> 5;
        const u32 bit = 1u << (index & 31);
        if ((pickup->state_flags & GIZMOPICKUP_STATE_VISIBLE) == 0) {
            progress->visible[word] &= ~bit;
        }
        if ((pickup->state_flags & GIZMOPICKUP_STATE_ENABLED) == 0) {
            progress->enabled[word] &= ~bit;
        }
        if ((pickup->state_flags & GIZMOPICKUP_STATE_COLLECTED) != 0) {
            progress->collected[word] |= bit;
        }
    }
}

void GizmoPickups_InitSys(GIZMOPICKUPSYS_s *pickup_sys) {
    GizmoPickupSys = pickup_sys;
}

GIZMOPICKUP_s *GizmoPickup_FindNearest(WORLDINFO_s *world, NUVEC *position, f32 *distance) {
    GIZMOPICKUPRUNTIMESYS_s *runtime = world->gizmo_pickup_sys;
    GIZMOPICKUP_s *pickup = runtime->pickups;
    GIZMOPICKUP_s *nearest = NULL;
    f32 best_distance = 1.0e9f;
    for (i32 index = 0; index < world->gizmo_pickup_sys->pickup_count; ++index, ++pickup) {
        f32 candidate = NuVecDistSqr(position, &pickup->position, NULL);
        if (candidate < best_distance) {
            best_distance = candidate;
            nearest = pickup;
        }
    }
    if (distance != NULL) {
        *distance = best_distance;
    }
    return nearest;
}

// --- World-module helpers (kept with the WorldInfo API) ---

void SetAreaPickupGravity(i32 area, i32 level) {
    AreaPickupGravity = -6.0f;
    if (area < 0 || area >= AREACOUNT) {
        return;
    }
    AREADATA *ad = &ADataList[area];
    if (ad->flags & AREAFLAG_NOPICKUPGRAVITY) {
        if ((*(u8 *)((char *)LDataList + level * 0x144 + 0x66) & 0x40) == 0) {
            AreaPickupGravity = 0.0f;
            return;
        }
    }
    if (ad->flags & AREAFLAG_VEHICLE_AREA) {
        AreaPickupGravity = -20.0f;
    }
}

static i32 GizmoPickups_Load(void *world_ptr, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    GIZMOPICKUPRUNTIMESYS_s *pickup_sys = world->gizmo_pickup_sys;
    if (pickup_sys->pickup_count != 0) {
        return 0;
    }

    const i32 version = EdFileReadInt();
    pickup_sys->pickup_count = EdFileReadInt();
    if (version >= 3) {
        pickup_sys->total_score = EdFileReadInt();
    }
    if (version >= 5) {
        pickup_sys->draw_distance = EdFileReadFloat();
        pickup_sys->pickup_scale = EdFileReadFloat();
        if (version == 5) {
            pickup_sys->pickup_scale = UpdateAreaPickupScale(world);
        }
        AreaPickupScale = pickup_sys->pickup_scale;
    } else {
        pickup_sys->pickup_scale = 1.0f;
        AreaPickupScale = 1.0f;
    }

    if (version < 6 && pickup_sys->draw_distance < 10.0f) {
        pickup_sys->draw_distance = 10.0f;
    }
    if (version <= 6 && world->level_sub_id >= 0 && world->level_sub_id < AREACOUNT &&
        (ADataList[world->level_sub_id].flags & AREAFLAG_VEHICLE_AREA) != 0 && pickup_sys->draw_distance < 100.0f) {
        pickup_sys->draw_distance = 100.0f;
    }
    SetAreaPickupGravity(world->level_sub_id, world->level_idx);

    GIZMOPICKUP_s *pickup = world->gizmo_pickup_sys->pickups;
    for (i32 index = 0; index < world->gizmo_pickup_sys->pickup_count; ++index, ++pickup) {
        EdFileRead(pickup->name, sizeof(pickup->name));
        EdFileReadNuVec(&pickup->position);
        pickup->type_code = static_cast<char>(EdFileReadChar());
        if (version >= 2) {
            pickup->config_flags = static_cast<u8>(EdFileReadChar());
        }
        if (version >= 4) {
            pickup->activation_group = static_cast<u8>(EdFileReadChar());
        }
    }
    return 1;
}

static void *GizmoPickups_ReserveBufferSpace(void *world_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
    world->gizmo_pickup_sys = reinterpret_cast<GIZMOPICKUPRUNTIMESYS_s *>(world->giz_buffer.addr);
    world->giz_buffer.addr += sizeof(GIZMOPICKUPRUNTIMESYS_s);
    memset(world->gizmo_pickup_sys, 0, sizeof(GIZMOPICKUPRUNTIMESYS_s));

    GIZMOPICKUPRUNTIMESYS_s *pickup_sys = world->gizmo_pickup_sys;
    pickup_sys->draw_distance = 10.0f;
    pickup_sys->pickup_scale = UpdateAreaPickupScale(world);
    SetAreaPickupGravity(world->level_sub_id, world->level_idx);

    pickup_sys = world->gizmo_pickup_sys;
    if (world->current_level->max_pickups != 0) {
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
        pickup_sys->pickups = reinterpret_cast<GIZMOPICKUP_s *>(world->giz_buffer.addr);
        world->giz_buffer.addr += world->current_level->max_pickups * sizeof(GIZMOPICKUP_s);
    }
    world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
    pickup_sys->temporary_pickups = reinterpret_cast<GIZMOPICKUP_s *>(world->giz_buffer.addr);
    world->giz_buffer.addr += GIZMOPICKUP_TEMPORARY_CAPACITY * sizeof(GIZMOPICKUP_s);
    return pickup_sys;
}

u32 GizmoPickups_TotalScore(void *world) {
    GIZMOPICKUPRUNTIMESYS_s *system = static_cast<WORLDINFO_s *>(world)->gizmo_pickup_sys;
    GIZMOPICKUP_s *pickup = system->pickups;
    u32 score = 0;
    if (pickup != NULL) {
        for (i32 i = 0; i < system->pickup_count; ++i, ++pickup)
            score += GizmoPickupSys->types[pickup->type_index].score;
    }
    return score;
}

ADDGIZMOTYPE *GizmoPickups_RegisterGizmo(i32 type_id) {
    static ADDGIZMOTYPE addtype;

    addtype = Default_ADDGIZMOTYPE;
    addtype.name = "GizmoPickup";
    addtype.prefix = "";
    addtype.fns.unknown1 = 0x100;
    addtype.fns.early_update_fn = NULL;
    addtype.fns.panel_draw_fn = NULL;
    addtype.fns.get_visibility_fn = NULL;
    addtype.fns.get_max_gizmos_fn = GizmoPickups_GetMaxGizmos;
    addtype.fns.get_pos_fn = GizmoPickup_GetPos;
    addtype.fns.using_special_fn = NULL;
    addtype.fns.add_gizmos_fn = GizmoPickups_AddGizmos;
    addtype.fns.bolt_hit_plat_fn = NULL;
    addtype.fns.get_best_bolt_target_fn = NULL;
    addtype.fns.late_update_fn = GizmoPickups_Update;
    addtype.fns.bolt_hit_fn = NULL;
    addtype.fns.draw_fn = GizmoPickups_Draw;
    addtype.fns.get_gizmo_name_fn = GizmoPickup_GetGizmoName;
    addtype.fns.get_output_fn = GizmoPickup_GetOutput;
    addtype.fns.get_output_name_fn = GizmoPickup_GetOutputName;
    addtype.fns.get_num_outputs_fn = GizmoPickup_GetNumOutputs;
    addtype.fns.activate_fn = GizmoPickup_Activate;
    addtype.fns.activate_rev_fn = NULL;
    addtype.fns.set_visibility_fn = GizmoPickup_SetVisibility;
    addtype.fns.allocate_progress_data_fn = GizmoPickups_AllocateProgressData;
    addtype.fns.clear_progress_fn = GizmoPickups_ClearProgress;
    addtype.fns.store_progress_fn = GizmoPickups_StoreProgress;
    addtype.fns.reserve_buffer_space_fn = GizmoPickups_ReserveBufferSpace;
    addtype.fns.reset_fn = GizmoPickups_Reset;
    addtype.fns.load_fn = GizmoPickups_Load;
    addtype.fns.post_load_fn = GizmoPickups_PostLoad;
    addtype.fns.add_level_sfx_fn = NULL;
    gizmopickup_typeid = type_id;

    return &addtype;
}

i32 GizmoPickup_NumberOfType(WORLDINFO_s *world, i32 type_index, char type_code) {
    GIZMOPICKUPRUNTIMESYS_s *runtime = world->gizmo_pickup_sys;
    GIZMOPICKUP_s *pickup = runtime->pickups;
    if (type_code == 0 && type_index == -1) {
        return 0;
    }

    i32 count = 0;
    const i32 pickup_count = runtime->pickup_count;
    if (pickup_count > 0) {
        if (type_code != 0) {
            for (i32 index = 0; index != pickup_count; ++index, ++pickup)
                count = pickup->type_code == type_code ? count + 1 : count;
        } else {
            for (i32 index = 0; index != pickup_count; ++index, ++pickup)
                count = pickup->type_index == type_index ? count + 1 : count;
        }
    }
    return count;
}

GIZMOPICKUP_s *GizmoPickup_FindByName(WORLDINFO_s *world, char *name) {
    if (name != NULL && world != NULL) {
        GIZMOPICKUP_s *pickup = world->gizmo_pickup_sys->pickups;
        if (pickup != NULL) {
            for (i32 index = 0; index < world->gizmo_pickup_sys->pickup_count; ++index, ++pickup) {
                if (NuStrICmp(pickup->name, name) == 0)
                    return pickup;
            }
        }
    }
    return NULL;
}

void GizmoPickup_TurnOnPickup(GIZMOPICKUP_s *pickup) {
    if (pickup != NULL) {
        pickup->state_flags |= GIZMOPICKUP_STATE_ENABLED | GIZMOPICKUP_STATE_VISIBLE | GIZMOPICKUP_STATE_ACTIVATED;
    }
}

i32 GizmoPickup_BeenTurnedOn(GIZMOPICKUP_s *pickup) {
    return pickup != NULL ? pickup->state_activated : 0;
}

GIZMOPICKUP_s *GizmoPickup_InBox(WORLDINFO_s *world, i32 type_index, NUVEC *minimum, NUVEC *maximum) {
    GIZMOPICKUPRUNTIMESYS_s *runtime = world->gizmo_pickup_sys;
    GIZMOPICKUP_s *pickup = runtime->pickups;
    if (pickup == NULL) {
        return NULL;
    }

    GIZMO_PICKUP_TYPE *type = &GizmoPickupSys->types[type_index];
    if (type->field_0x0f != 0) {
        return NULL;
    }
    const f32 radius = type->shadow_extent_x * AreaPickupScale;
    const i32 count = runtime->pickup_count;
    const f32 low_x = minimum->x - radius;
    const f32 high_x = maximum->x + radius;
    const f32 low_y = minimum->y - radius;
    const f32 high_y = maximum->y + radius;
    const f32 low_z = minimum->z - radius;
    const f32 high_z = maximum->z + radius;
    for (i32 index = 0; index < count; ++index, ++pickup) {
        if (pickup->type_index != type_index) {
            continue;
        }
        if ((pickup->state_flags & 6) != 6) {
            continue;
        }
        if ((pickup->state_flags & GIZMOPICKUP_STATE_COLLECTED) != 0) {
            continue;
        }
        if (low_x > pickup->position.x || pickup->position.x > high_x || low_z > pickup->position.z ||
            pickup->position.z > high_z || low_y > pickup->position.y || pickup->position.y > high_y) {
            continue;
        }
        return pickup;
    }
    return NULL;
}

GIZMOPICKUP_s *GizmoPickups_Collide(WORLDINFO_s *world, GameObject_s *object, i32) {
    if (world == NULL || (object->apiobj.field_0x1f4 & 0x00040000) != 0) {
        return NULL;
    }

    GIZMOPICKUP_s *pickup = GizmoPickups_CollideList(object, world->gizmo_pickup_sys->temporary_pickups, 64);
    if (pickup != NULL) {
        pickup->state_flags =
            static_cast<u8>((pickup->state_flags | GIZMOPICKUP_STATE_COLLECTED) & ~GIZMOPICKUP_STATE_ACTIVE);
    } else {
        pickup =
            GizmoPickups_CollideList(object, world->gizmo_pickup_sys->pickups, world->gizmo_pickup_sys->pickup_count);
        if (pickup == NULL) {
            return NULL;
        }
        pickup->state_flags |= GIZMOPICKUP_STATE_COLLECTED;
    }

    i32 type_index = pickup->type_index;
    if ((pickup->state_flags & GIZMOPICKUP_STATE_ALTERNATE_TYPE) != 0 && GizmoPickupSys->alternate_type != -1) {
        type_index = GizmoPickupSys->alternate_type;
    }
    if (GizmoPickupSys->types[type_index].collection_sfx_name != NULL) {
        PlaySfx(GizmoPickupSys->types[type_index].collection_sfx_name, &pickup->position);
    }
    if (GizmoPickupSys->types[type_index].collect_fn != NULL) {
        GizmoPickupSys->types[type_index].collect_fn(world, pickup, type_index, object, 0);
    }
    if ((GizmoPickupSys->types[type_index].flags & GIZMOPICKUP_TYPE_FLAG_40) != 0) {
        ++AreaGlobals.values.field_0x18;
    }
    return pickup;
}

void MiniKitDetector(nuvec_s *position) {
    ADDGAMEMSG message = AddGameMsg_Default;
    message.text = LEGOASCII_BIGARROW != NULL ? LEGOASCII_BIGARROW : txt_UNKNOWN;
    message.position = position;
    message.flags = 0x40083;
    message.scale = 0.6f;
    message.field_0x44 = reinterpret_cast<void *>(GameMsg_Draw_MiniKitDetector);
    message.field_0x4f = 4;
    AddGameMsg(&message);
}

static void GizmoPickups_UpdateListBody(WORLDINFO *world, GIZMOPICKUP_s *pickups, i32 count, i32 play_nearby_sfx) {

    const bool detector_phase =
        play_nearby_sfx && FadeSys.fade == 0.0f && NuFmod(GameTimer.time_elapsed_mod_seconds, 0.2f) < 0.1f;
    const bool minikit_detector = detector_phase && Cheats_CheckFlags(0x200) != 0;
    const bool red_brick_detector = detector_phase && Cheats_CheckFlags(0x40000) != 0;
    NUVEC *nearby_pickup = NULL;
    f32 nearest_distance_squared = 1.0f;

    SuperCounters_ResetProcessed(world);
    for (i32 index = 0; index < count; ++index) {
        GIZMOPICKUP_s &pickup = pickups[index];
        if ((pickup.state_flags & GIZMOPICKUP_STATE_ACTIVE) == 0) {
            continue;
        }

        pickup.state_flags |= GIZMOPICKUP_STATE_DRAW_VISIBLE;
        pickup.draw_rotation = static_cast<u16>(pickup.draw_rotation + 16384.0f * FRAMETIME);
        i32 type_index = pickup.type_index;
        if ((pickup.state_flags & GIZMOPICKUP_STATE_ALTERNATE_TYPE) != 0 && GizmoPickupSys->alternate_type >= 0) {
            type_index = GizmoPickupSys->alternate_type;
        }
        if (type_index < 0 || type_index >= GizmoPickupSys->type_count) {
            type_index = 0;
        }
        GIZMO_PICKUP_TYPE *type = &GizmoPickupSys->types[type_index];

        const u8 effect_flags = type->flags;
        bool show_pickup_effects = false;
        if ((pickup.state_flags & GIZMOPICKUP_STATE_COLLECTED) == 0 && type->field_0x0f == 0 &&
            (effect_flags & 0x2c) != 0) {
            SUPERCOUNTERPICKUP *counter_pickup = NULL;
            SUPERCOUNTER *counter = SuperCounter_FindFromNameAndLevel(pickup.name, world, &counter_pickup);
            if (counter == NULL) {
                show_pickup_effects = true;
            } else {
                bool gizmo_complete = false;
                if ((pickup.state_flags & 6) != 6 && (counter->processed_flags & 1) == 0 &&
                    counter_pickup->position_gizmo != NULL) {
                    GIZMO *gizmo = counter_pickup->position_gizmo;
                    if (gizmo->type_id == turret_gizmotype_id) {
                        gizmo_complete = (static_cast<GIZTURRET_s *>(gizmo->object)->flags & 0x20) != 0;
                    } else if (gizmo->type_id == blowup_gizmotype_id) {
                        gizmo_complete = (static_cast<GIZMOBLOWUP_s *>(gizmo->object)->output_flags & 1) != 0;
                    } else if (gizmo->type_id == gizbuildit_gizmotype_id) {
                        gizmo_complete = static_cast<GIZBUILDIT_s *>(gizmo->object)->build_state == 2;
                    }
                }
                if (!gizmo_complete && SuperCounter_AnyCollected(counter, world) == 0) {
                    const u8 previous_flags = counter->processed_flags;
                    counter->processed_flags = previous_flags | 2;
                    show_pickup_effects = (previous_flags & 3) != 3;
                } else {
                    counter->processed_flags |= 2;
                }
            }
        }

        if (show_pickup_effects) {
            if ((minikit_detector && (effect_flags & GIZMOPICKUP_TYPE_MINIKIT_DETECTOR) != 0) ||
                (red_brick_detector && (effect_flags & GIZMOPICKUP_TYPE_RED_BRICK_DETECTOR) != 0)) {
                NUVEC detector_position = pickup.position;
                detector_position.y += 0.5f;
                MiniKitDetector(&detector_position);
            }

            if (play_nearby_sfx && GameCam != NULL) {
                const f32 distance_squared = NuVecDistSqr(&pickup.position, &GameCam->pos, NULL);
                if (distance_squared < nearest_distance_squared) {
                    nearest_distance_squared = distance_squared;
                    nearby_pickup = &pickup.position;
                }
            }
        }

        if (pickup.remaining_visible_time > 0.0f) {
            if (pickup.remaining_visible_time <= 0.5f && PickUpFlickerFrames > 0 &&
                PickUpFlickerTest <= PickupFlickerFrame % PickUpFlickerFrames) {
                pickup.state_flags &= static_cast<u8>(~GIZMOPICKUP_STATE_DRAW_VISIBLE);
            }
            if (MiniCutCam != 2) {
                pickup.remaining_visible_time -= FRAMETIME;
                if (pickup.remaining_visible_time < 0.0f) {
                    pickup.state_flags &= static_cast<u8>(~GIZMOPICKUP_STATE_ACTIVE);
                }
            }
        }

        if ((pickup.state_flags &
             (GIZMOPICKUP_STATE_ENABLED | GIZMOPICKUP_STATE_VISIBLE | GIZMOPICKUP_STATE_COLLECTED)) ==
                (GIZMOPICKUP_STATE_ENABLED | GIZMOPICKUP_STATE_VISIBLE) &&
            type->field_0x0f == 0 && type->update_fn != NULL) {
            type->update_fn(world, &pickup);
        }
    }

    if (nearby_pickup != NULL) {
        GameAudio_PlaySfx(0x25, nearby_pickup, 0, 0);
    }
}

static void GizmoPickups_Update(void *world_ptr, void *, float) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (world == NULL) {
        return;
    }

    // Retail does not test gizmo_pickup_sys for NULL here.
    GIZMOPICKUPRUNTIMESYS_s *pickup_sys = world->gizmo_pickup_sys;
    if (pickup_sys->pickups != NULL && Missions_PickupsOff(MissionSys) == 0) {
        GizmoPickups_UpdateList(world, pickup_sys->pickups, pickup_sys->pickup_count, 1);
    }
    if (!(pickup_sys->temporary_pickups != NULL && Missions_PickupsOff(MissionSys) == 0))
        return;
    GizmoPickups_UpdateList(world, pickup_sys->temporary_pickups, GIZMOPICKUP_TEMPORARY_CAPACITY, 0);
}

void ResetCoinPacket(COINPACKET_s *packet) {
    if (packet != NULL) {
        packet->scale = 1.0f;
        packet->double_score_timer = 0.0f;
        packet->active = 1;
    }
}

void UpdateCoinPacket(COINPACKET_s *packet, i32 active, i32 player_index) {
    if (packet == NULL) {
        return;
    }
    if (active == 0) {
        packet->active = 1;
        packet->scale = 1.0f;
        packet->double_score_timer = 0.0f;
        if (BonusArea != 0) {
            if (packet->coins < 10) {
                packet->coins = 0;
                return;
            }
            u32 decrement = static_cast<u32>(static_cast<i32>(20000.0f * FRAMETIME));
            decrement = (decrement / 10) * 10;
            if (decrement < 10) {
                decrement = 10;
            } else if (packet->coins < decrement) {
                packet->coins = 0;
                return;
            }
            packet->coins -= decrement;
        }
        return;
    }

    packet->scale = SeekLinearF(packet->scale, 1.0f, 3.0f * FRAMETIME);
    if (((DoubleScore >> player_index) & 1) != 0) {
        if (packet->double_score_timer > 0.0f) {
            packet->double_score_timer -= FRAMETIME;
            if (packet->double_score_timer <= 0.0f) {
                packet->active = 1;
            }
        }
    } else {
        packet->double_score_timer = 0.0f;
        packet->active = 1;
    }
}

i32 CoinsGoToMainTotal() {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if ((HUB_ADATA != NULL && HUB_ADATA == world->area) || SuperStory != 0 ||
        (world->area != NULL && (world->area->flags & (AREAFLAG_SUPER_BONUS_AREA & ~AREAFLAG_BONUS_AREA)) != 0) ||
        (world->current_level != NULL && (world->current_level->flags & LEVEL_STATUS) != 0)) {
        return 1;
    }
    return 0;
}

void PartUpdate_Heart(PART_s *part) {
    NUVEC position = part->position;
    part->rotation_y += 16384.0f * FRAMETIME;
    NuMtxSetRotationY(&part->transform, part->rotation_y);
    NuMtxTranslate(&part->transform, &position);
}

void SpecialMiniKits_Configure(WORLDINFO_s *world, char *config) {
    world->special_minikits = NULL;
    if (world->current_gscn == NULL) {
        return;
    }
    NUFPAR *parser = NuFParCreateMem(const_cast<char *>("specialminikits"), config, 0xffff);
    if (parser == NULL) {
        return;
    }

    world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
    SPECIALMINIKIT_s *items = static_cast<SPECIALMINIKIT_s *>(world->giz_buffer.void_ptr);
    SPECIALMINIKIT_s *next = items;
    i32 count = 0;

    while (NuFParGetLine(parser) != 0 && NuFParGetWord(parser) != 0) {
        if (NuStrICmp(parser->word_buf, const_cast<char *>("specialminikit")) != 0) {
            continue;
        }

        SPECIALMINIKIT_s &item = *next;
        item = {};
        item.flags = 1;
        while (NuFParGetWord(parser) != 0) {
            if (NuStrICmp(parser->word_buf, const_cast<char *>("pickup")) == 0) {
                if (NuFParGetWord(parser) != 0 && NuStrLen(parser->word_buf) < sizeof(item.pickup_name)) {
                    NuStrCpy(item.pickup_name, parser->word_buf);
                }
            } else if (NuStrICmp(parser->word_buf, const_cast<char *>("at_object")) == 0) {
                if (NuFParGetWord(parser) != 0) {
                    NuSpecialFind(world->current_gscn, &item.special, parser->word_buf, 1);
                    item.flags &= ~0x20;
                }
            } else if (NuStrICmp(parser->word_buf, const_cast<char *>("at_gizmo")) == 0) {
                if (NuFParGetWord(parser) != 0 && NuStrLen(parser->word_buf) < sizeof(item.special_name)) {
                    NuStrCpy(item.special_name, parser->word_buf);
                    item.flags |= 0x20;
                }
            } else if (NuStrICmp(parser->word_buf, const_cast<char *>("draw_at_start")) == 0) {
                item.flags = (item.flags & ~1) | 2;
            } else if (NuStrICmp(parser->word_buf, const_cast<char *>("draw_while_animating")) == 0) {
                item.flags = (item.flags & ~1) | 4;
            } else if (NuStrICmp(parser->word_buf, const_cast<char *>("draw_at_end")) == 0) {
                item.flags = (item.flags & ~1) | 8;
            } else if (NuStrICmp(parser->word_buf, const_cast<char *>("orientate")) == 0) {
                item.flags |= 0x10;
            }
        }

        if (item.pickup_name[0] == '\0') {
            continue;
        }
        if ((item.flags & 0x20) != 0) {
            if (item.special_name[0] == '\0') {
                continue;
            }
        } else if (NuSpecialExistsFn(&item.special) == 0) {
            continue;
        }

        if (NuSpecialExistsFn(&item.special) != 0) {
            item.inst_anim = NuSpecialGetInstAnim(&item.special);
            if (item.inst_anim != NULL) {
                nuinstanim_s *animation = static_cast<nuinstanim_s *>(item.inst_anim);
                item.anim_data = item.special.scene->instance_animation_data[animation->anim_ix];
                item.end_frame = NuAnimEndFrameOld(item.anim_data);
            }
        }
        ++next;
        ++count;
    }
    NuFParDestroy(parser);

    if (count == 0) {
        return;
    }
    world->giz_buffer.addr = reinterpret_cast<usize>(next);
    SPECIALMINIKITSYS_s *system = static_cast<SPECIALMINIKITSYS_s *>(world->giz_buffer.void_ptr);
    system->items = items;
    system->count = count;
    world->special_minikits = system;
    world->giz_buffer.addr = ALIGN(world->giz_buffer.addr + sizeof(*system), 4);
}

void SpecialMiniKits_Reset(WORLDINFO_s *world) {
    WORLDINFO_s *world_info = world;
    SPECIALMINIKITSYS_s *system = world_info->special_minikits;
    if (system == NULL || GizmoPickupSys->gizmo_type_id == -1) {
        return;
    }

    i32 count = system->count;
    SPECIALMINIKIT_s *item = system->items;
    if (count <= 0) {
        return;
    }

    i32 index = 0;
    for (;;) {
        item->pickup_gizmo = GizmoFindByName(world_info->gizmo_sys, gizmopickup_typeid, item->pickup_name);
        if ((item->flags & 0x20) != 0) {
            item->special_gizmo = GizmoFindByName(world_info->gizmo_sys, -1, item->special_name);
        }
        ++index;
        ++item;
        if (world_info->special_minikits->count <= index) {
            break;
        }
    }
}

void SpecialMiniKits_Draw(WORLDINFO_s *world) {
    NUVEC scale = {AreaPickupScale, AreaPickupScale, AreaPickupScale};
    SPECIALMINIKITSYS_s *system = world->special_minikits;
    if (system == NULL || GizmoPickupSys->gizmo_type_id == -1 ||
        GizmoPickupSys->types[GizmoPickupSys->gizmo_type_id].field_0x0f != 0) {
        return;
    }

    const u16 y_rotation = static_cast<u16>(static_cast<i32>(NuFmod(GameTimer.time_elapsed, 4.0f) * 0.25f * 65536.0f));
    const u16 x_rotation =
        static_cast<u16>(static_cast<i32>(NuTrigTable[static_cast<i32>(y_rotation) & 0x7fff] * 1820.0f));

    SPECIALMINIKIT_s *item = world->special_minikits->items;
    for (i32 index = 0; index < world->special_minikits->count; ++index, ++item) {
        NUMTX_ALIGNED16 matrix;
        NUVEC *position;

        if ((item->flags & 0x20) != 0) {
            if (item->special_gizmo == NULL || GizmoGetVisibility(world->gizmo_sys, item->special_gizmo) == 0) {
                continue;
            }
            position = GizmoGetPos(world->gizmo_sys, item->special_gizmo);
            if (__builtin_expect(position == NULL, 1)) {
                continue;
            }
            NuMtxSetScale(&matrix, &scale);
            NuMtxTranslate(&matrix, position);
        } else {
            if (NuSpecialGetVisibilityFn(&item->special) == 0) {
                continue;
            }

            nuinstanim_s *animation = static_cast<nuinstanim_s *>(item->inst_anim);
            bool draw = (item->flags & 1) != 0 || animation == NULL || item->anim_data == NULL;
            if (__builtin_expect(!draw && (animation->flags & NUINSTANIM_FLAG_REPEATING) == 0, 1)) {
                draw = ((item->flags & 2) != 0 && animation->ltime <= 1.0f) ||
                       ((item->flags & 8) != 0 && item->end_frame <= animation->ltime);
            }
            if (!draw && (item->flags & 4) != 0 && (animation->flags & NUINSTANIM_FLAG_PLAYING) != 0) {
                draw = true;
            }
            if (__builtin_expect(!draw, 0)) {
                continue;
            }

            position = NuSpecialGetDrawPos(&item->special);
            matrix = *NuSpecialGetDrawMtx(&item->special);
            NuVecNorm(NUMTX_GET_ROW_VEC(&matrix, 0), NUMTX_GET_ROW_VEC(&matrix, 0));
            NuVecNorm(NUMTX_GET_ROW_VEC(&matrix, 1), NUMTX_GET_ROW_VEC(&matrix, 1));
            NuVecNorm(NUMTX_GET_ROW_VEC(&matrix, 2), NUMTX_GET_ROW_VEC(&matrix, 2));
            NuMtxPreScale(&matrix, &scale);
        }

        if (item->pickup_gizmo != NULL) {
            GIZMOPICKUP_s *pickup = static_cast<GIZMOPICKUP_s *>(item->pickup_gizmo->object);
            i32 type_index = pickup->type_index;
            if ((pickup->state_flags & GIZMOPICKUP_STATE_ALTERNATE_TYPE) != 0 && GizmoPickupSys->alternate_type != -1) {
                type_index = GizmoPickupSys->alternate_type;
            }
            GIZMO_PICKUP_TYPE *type = &GizmoPickupSys->types[type_index];

            if ((item->flags & 0x10) == 0) {
                NuMtxSetScale(&matrix, &scale);
                NuMtxRotateY(&matrix, y_rotation);
                NuMtxRotateX(&matrix, x_rotation);
                NuMtxTranslate(&matrix, position);
            }
            NuSpecialDrawAt(&world->lev_objs[static_cast<i16>(type->first_model_id)].special, &matrix);
        }
    }
}

