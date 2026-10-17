// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "legoapi/gizmos/transport/teleport.h"

#include "decomp.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nufile/nufpar.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/misc/supportall.h"
#include "globals.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/core/players.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/gizmo/base/TeleportObjectInterface.h"
#include "legoapi/ai/core/gameai.h"
#include "legoapi/audio/audio.h"
#include "legoapi/render/light/surfaces.h"

i32 teleport_gizmotype_id = -1;

static TELEPORT_s *Tel_teleport;
static WORLDINFO_s *Tel_worldinfo;
static void Tel_1_way(nufpar_s *);
static void Tel_crawl(nufpar_s *);
static void Tel_duration(nufpar_s *parser);
static void Tel_flap(nufpar_s *parser);
static void Tel_flip_flap(nufpar_s *);
static void Tel_Hacky_Cam(nufpar_s *);
static void Tel_name(nufpar_s *parser);
static void Tel_obj(nufpar_s *parser);
static void Tel_range(nufpar_s *parser);
static void Tel_spline(nufpar_s *parser);

static NUFPCOMJMP Teleport_ConfigKeywords[] = {
    {"spline", Tel_spline}, {"duration", Tel_duration},   {"range", Tel_range},        {"1_way", Tel_1_way},
    {"crawl", Tel_crawl},   {"flip_flap", Tel_flip_flap}, {"hackycam", Tel_Hacky_Cam}, {"obj", Tel_obj},
    {"flap1", Tel_flap},    {"flap2", Tel_flap},          {"name", Tel_name},          {NULL, NULL}};

static i32 FindTeleport_Direction;

static inline void Teleport_GetSegment(GameObject_s *object, TELEPORT_s *teleport, i32 *start, i32 *end) {
    const bool forwards = (object->context_variant_flags & 4) != 0;
    if ((teleport->flags & 4) != 0) {
        *start = forwards ? 0 : 3;
        *end = forwards ? 3 : 0;
    } else if (forwards) {
        if (object->field_0x7a4 == 0) {
            *start = 0;
        } else {
            *start = 2;
        }
        *end = object->field_0x7a4 == 0 ? 1 : 3;
    } else {
        *start = object->field_0x7a4 == 0 ? 3 : 1;
        if (object->field_0x7a4 == 0) {
            *end = 2;
        } else {
            *end = 0;
        }
    }
}

static inline f32 Teleport_GetSegmentDuration(GameObject_s *object, TELEPORT_s *teleport, i32 start, i32 end) {
    if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL) {
        const f32 animation_speed = AnimSpeed(object->apiobj.character_model, object->context_animation);
        teleport = static_cast<TELEPORT_s *>(object->field_0x788);
        if (animation_speed != 0.0f) {
            NUVEC *points = teleport->path->pts;
            return NuVecXZDist(&points[start], &points[end], NULL) / NuFabs(animation_speed);
        }
    }
    return (teleport->flags & 4) != 0 ? 3.0f : 1.0f;
}

static inline void Teleport_SetFacing(GameObject_s *object, NUVEC *start, NUVEC *end, bool all_angles) {
    const u16 facing = NuAtan2D(end->x - start->x, end->z - start->z);
    object->apiobj.movement_facing_angle = facing;
    if (all_angles) {
        object->apiobj.facing_angle = facing;
        object->apiobj.field_0x276 = facing;
    }
}

static void Tel_1_way(nufpar_s *) {
    Tel_teleport->flags |= 1;
}

static void Tel_crawl(nufpar_s *) {
    Tel_teleport->flags |= 4;
}

static void Tel_flip_flap(nufpar_s *) {
    Tel_teleport->flags |= 8;
}

static void Tel_Hacky_Cam(nufpar_s *) {
    Tel_teleport->flags |= 0x10;
}

static i32 Teleport_ActivateRev(GIZMO *gizmo, i32 activate, i32 flags) {
    if (gizmo == NULL || gizmo->object == NULL) {
        return 0;
    }
    TELEPORT_s *teleport = static_cast<TELEPORT_s *>(gizmo->object);
    if ((flags & 1) != 0 && teleport->enabled != activate) {
        return 0;
    }
    if (activate != 0) {
        teleport->enabled = 0;
    } else if (teleport->enabled == 0) {
        teleport->enabled = 1;
    }
    return 1;
}

static void Teleport_Activate(GIZMO *gizmo, i32 activate) {
    TELEPORT_s *teleport = static_cast<TELEPORT_s *>(gizmo->object);
    if (activate != 0) {
        teleport->active = 0;
        teleport->enabled = 1;
    } else {
        teleport->enabled = 0;
    }
}

static i32 Teleport_GetMaxGizmos(void *world_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    return world != NULL ? world->current_level->max_teleports : 0;
}

static char *Teleport_GetGizmoName(GIZMO *gizmo) {
    return gizmo != NULL ? static_cast<char *>(gizmo->object) : NULL;
}

static i32 Teleport_GetOutput(GIZMO *gizmo, i32, i32) {
    if (gizmo != NULL && gizmo->object != NULL) {
        return static_cast<TELEPORT_s *>(gizmo->object)->active;
    }
    return 0;
}

char *Teleport_GetOutputName(GIZMO *, i32) {
    return const_cast<char *>("Occupied");
}

static i32 Teleport_GetNumOutputs(GIZMO *) {
    return 1;
}

static void Teleport_AddGizmos(GIZMOSYS *gizmo_sys, i32 type_id, void *world_ptr, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (world != NULL && world->teleports != NULL) {
        for (i32 index = 0; index < world->teleport_count; ++index) {
            AddGizmo(gizmo_sys, type_id, NULL, &world->teleports[index]);
        }
    }
}

static void Tel_range(nufpar_s *parser) {
    f32 range = NuFParGetFloat(parser);
    Tel_teleport->flags |= 2;
    Tel_teleport->range_squared = range * range;
}

static void Tel_duration(nufpar_s *parser) {
    TELEPORT_s *teleport = Tel_teleport;
    teleport->duration = NuFParGetFloat(parser);
}

static void Tel_name(nufpar_s *parser) {
    if (NuFParGetWord(parser) != 0) {
        GizmoGetUniqueName(WORLD->gizmo_sys, "TLT_", parser->word_buf, Tel_teleport->name, sizeof(Tel_teleport->name));
    }
}

static void Tel_obj(nufpar_s *parser) {
    if (NuFParGetWord(parser) != 0) {
        NuSpecialFind(Tel_worldinfo->current_gscn, &Tel_teleport->blocking_special, parser->word_buf, 1);
    }
}

static void Tel_flap(nufpar_s *parser) {
    if (NuFParGetWord(parser) == 0) {
        return;
    }

    if (NuStrIStr(parser->line_buf, "flap1") != NULL) {
        if (NuSpecialFind(Tel_worldinfo->current_gscn, &Tel_teleport->flap1_special, parser->word_buf, 1) != 0) {
            Tel_teleport->flap1_matrix = *NuSpecialGetDrawMtx(&Tel_teleport->flap1_special);
        }
    } else {
        if (NuSpecialFind(Tel_worldinfo->current_gscn, &Tel_teleport->flap2_special, parser->word_buf, 1) != 0) {
            Tel_teleport->flap2_matrix = *NuSpecialGetDrawMtx(&Tel_teleport->flap2_special);
        }
    }
}

static void Tel_spline(nufpar_s *parser) {
    if (NuFParGetWord(parser) == 0) {
        return;
    }

    Tel_teleport->path = edSpline_SplineFind(Tel_worldinfo->current_gscn, parser->word_buf);
    if (Tel_teleport->path == NULL || Tel_teleport->path->length != 4) {
        Tel_teleport->path = NULL;
        return;
    }

    for (i32 i = 0; i < Tel_worldinfo->teleport_count; ++i) {
        if (Tel_worldinfo->teleports[i].path == Tel_teleport->path) {
            Tel_teleport->path = NULL;
            return;
        }
    }

    NuStrCpy(Tel_teleport->name, "TLT_");
    NuStrCat(Tel_teleport->name, Tel_teleport->path->name);
    GizmoGetUniqueName(Tel_worldinfo->gizmo_sys, "TLT_", Tel_teleport->name, Tel_teleport->name,
                       sizeof(Tel_teleport->name));
}

MechObjectInterface *TELEPORT_s::GetMechObjectInterface() {
    if (mech_object_interface == NULL) {
        new TeleportObjectInterface(*this, -1);
    }
    return mech_object_interface;
}

void TELEPORT_s::ClearMechObjectInterface() {
    delete mech_object_interface;
}

void Teleports_Configure(WORLDINFO_s *world, char *config) {
    world->teleports = NULL;
    if (world->current_gscn == NULL) {
        return;
    }

    NUFPAR *parser = NuFParCreateMem("teleports", config, 0xffff);
    if (parser == NULL) {
        return;
    }

    world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
    TELEPORT_s *teleport = reinterpret_cast<TELEPORT_s *>(world->giz_buffer.void_ptr);
    world->teleports = teleport;
    NuFParPushCom(parser, Teleport_ConfigKeywords);

    i32 active = 0;
    while (NuFParGetLine(parser) != 0) {
        if (NuFParGetWord(parser) != 0) {
            if (active) {
                if (NuStrICmp(parser->word_buf, "teleport_end") != 0) {
                    NuFParInterpretWord(parser);
                    active = 1;
                    continue;
                }

                if (NuStrLen(Tel_teleport->name) == 0) {
                    NuStrCpy(Tel_teleport->name, "TLT_");
                    NuStrCat(Tel_teleport->name, "TeleportNoSpline!");
                    GizmoGetUniqueName(WORLD->gizmo_sys, "TLT_", Tel_teleport->name, Tel_teleport->name,
                                       sizeof(Tel_teleport->name));
                }

                active = 0;
                if (teleport->path != NULL) {
                    ++world->teleport_count;
                    ++teleport;
                }
                continue;
            }

            if (NuStrICmp(parser->word_buf, "teleport_start") != 0) {
                continue;
            }

            Tel_worldinfo = world;
            Tel_teleport = teleport;
            NuStrCpy(teleport->name, "");
            teleport->enabled = 1;
            teleport->path = NULL;
            teleport->duration = 5.0f;
            teleport->range_squared = 0.0f;
            teleport->flags = 0;
            teleport->active = 0;
            teleport->blocking_special = {};
            teleport->flap1_special = {};
            teleport->flap2_special = {};
            active = 1;
        }
    }

    NuFParDestroy(parser);
    if (world->teleport_count > 0) {
        world->giz_buffer.addr = ALIGN(reinterpret_cast<usize>(teleport), 16);
    } else {
        world->teleports = NULL;
    }
}

TELEPORT_s *Teleport_Find(GameObject_s *object, float range_squared, VuVec *position) {
    if (WORLD->teleports == NULL)
        return NULL;
    f32 radius = 2.5f * object->apiobj.collision_radius;
    const f32 radius_squared = radius * radius;
    i32 nearest = -1;
    f32 nearest_distance = 100000000.0f;
    TELEPORT_s *teleport = WORLD->teleports;
    for (i32 i = 0; i < WORLD->teleport_count; ++i, ++teleport) {
        if (teleport->active != 0 || teleport->enabled == 0)
            continue;
        if (NuSpecialExistsFn(&teleport->blocking_special)) {
            nuinstanim_s *animation = NuSpecialGetInstAnim(&teleport->blocking_special);
            if (animation != NULL) {
                if (animation->playing || animation->ltime == 1.0f)
                    continue;
            } else if (NuSpecialGetVisibilityFn(&teleport->blocking_special)) {
                continue;
            }
        }
        const i32 range_flag = teleport->flags & 2;
        NUVEC *point = teleport->path->pts;
        f32 distance = NuVecDistSqr(&object->apiobj.collision_position, point, NULL);
        if (((range_squared != 0.0f && distance < range_squared) ||
             (range_flag == 0 ? distance < radius_squared : distance < teleport->range_squared)) &&
            (nearest == -1 || distance < nearest_distance)) {
            if (position != NULL) {
                point = teleport->path->pts;
                if (point != NULL)
                    position->xyz = *point;
            }
            FindTeleport_Direction = 0;
            nearest = i;
            nearest_distance = distance;
        }
        if ((teleport->flags & 1) != 0)
            continue;
        point = &teleport->path->pts[teleport->path->length - 1];
        distance = NuVecDistSqr(&object->apiobj.collision_position, point, NULL);
        if (((range_squared != 0.0f && distance < range_squared) ||
             (range_flag == 0 ? distance < radius_squared : distance < teleport->range_squared)) &&
            (nearest == -1 || distance < nearest_distance)) {
            if (position != NULL) {
                point = &teleport->path->pts[teleport->path->length - 1];
                if (point != NULL)
                    position->xyz = *point;
            }
            FindTeleport_Direction = 1;
            nearest = i;
            nearest_distance = distance;
        }
    }
    return nearest == -1 ? NULL : &WORLD->teleports[nearest];
}

void Teleports_Reset(WORLDINFO_s *world) {
    if (world->teleports == NULL || WORLD->teleport_count <= 0) {
        return;
    }

    TELEPORT_s *teleport = world->teleports;
    for (i32 i = 0; i < WORLD->teleport_count; ++i, ++teleport) {
        teleport->active = 0;
        teleport->enabled = 1;
        teleport->field_78 = 0;
        teleport->field_74 = 0;
        teleport->field_7a = 0;
        teleport->field_76 = 0;
    }
}

void Teleports_UpdateBeforeGameObjects(WORLDINFO_s *world) {
    if (world->teleports == NULL || WORLD->teleport_count <= 0)
        return;

    TELEPORT_s *teleport = world->teleports;
    for (i32 i = 0; i < WORLD->teleport_count; ++i, ++teleport) {
        teleport->field_78 = 0;
        teleport->field_7a = 0;
    }
}

void Teleports_UpdateAfterGameObjects(WORLDINFO_s *world) {
    NUMTX matrix;
    if (world->teleports == NULL || WORLD->teleport_count <= 0)
        return;

    TELEPORT_s *teleport = world->teleports;
    for (i32 i = 0; i < WORLD->teleport_count; ++i, ++teleport) {
        teleport->field_74 = SeekRot(teleport->field_74, teleport->field_78, 5.0f);
        if (NuSpecialExistsFn(&teleport->flap1_special)) {
            matrix = teleport->flap1_matrix;
            if ((teleport->flags & 8) != 0)
                NuMtxPreRotateX(&matrix, -teleport->field_74);
            else
                NuMtxPreRotateX(&matrix, teleport->field_74);
            NuSpecialSetDrawMtx(&teleport->flap1_special, &matrix);
            NuSpecialUpdate(&teleport->flap1_special);
        }

        teleport->field_76 = SeekRot(teleport->field_76, teleport->field_7a, 5.0f);
        if (NuSpecialExistsFn(&teleport->flap2_special)) {
            NUMTX matrix = teleport->flap2_matrix;
            if ((teleport->flags & 8) != 0)
                NuMtxPreRotateX(&matrix, -teleport->field_76);
            else
                NuMtxPreRotateX(&matrix, teleport->field_76);
            NuSpecialSetDrawMtx(&teleport->flap2_special, &matrix);
            NuSpecialUpdate(&teleport->flap2_special);
        }
    }
}

void Teleport_NetMoveCode(GameObject_s *object) {
    if (object->field_0x7a5 != 15 || object->field_0x7a3 != 0)
        return;

    TELEPORT_s *teleport = static_cast<TELEPORT_s *>(object->field_0x788);
    const bool forwards = (object->context_variant_flags & 4) != 0;
    const i16 flap1_rotation = forwards ? 0x4000 : -0x4000;
    const i16 flap2_rotation = forwards ? -0x4000 : 0x4000;

    if (NuSpecialExistsFn(&teleport->flap1_special) &&
        NuVecDistSqr(&object->apiobj.collision_position, NuSpecialGetDrawPos(&teleport->flap1_special), NULL) < 0.36f) {
        teleport->field_78 = flap1_rotation;
        if ((object->context_x_rotation & 1) == 0) {
            object->context_x_rotation |= 1;
            PlaySfx(const_cast<char *>("env_door_flap"), &object->apiobj.collision_position);
        }
    }

    if (NuSpecialExistsFn(&teleport->flap2_special) &&
        NuVecDistSqr(&object->apiobj.collision_position, NuSpecialGetDrawPos(&teleport->flap2_special), NULL) < 0.36f) {
        teleport->field_7a = flap2_rotation;
        if ((object->context_x_rotation & 2) == 0) {
            object->context_x_rotation |= 2;
            PlaySfx(const_cast<char *>("env_door_flap"), &object->apiobj.collision_position);
        }
    }
}

void Teleport_MoveCode(GameObject_s *object, i32 start_immediately) {
    if ((object->apiobj.character_data->model_flags & 0x40000) == 0 && SuperWeirdo(object) == 0)
        return;

    if (object->field_0x7a5 != 15) {
        if (object->apiobj.field_0x27d == 0 || object->apiobj.field_0x287 != 0)
            return;

        switch (object->field_0x7a5) {
            case 1:
            case 0xff:
            case 2:
            case 3:
            case 4:
                break;
            default:
                if (__builtin_expect(objInNetWaitContext(object, 15) == 0, 0))
                    return;
                break;
        }

        if (!object->apiobj.player_controlled && object->use_action != 3)
            return;
        if (object->touch_task != NULL &&
            object->touch_task->GetHashId().value != MechTouchTaskUseTeleport::HashId.value)
            return;

        TELEPORT_s *teleport = Teleport_Find(object, 0.0f, NULL);
        if (teleport == NULL)
            return;

        if (__builtin_expect(objInNetWaitContext(object, 15) != 0, 1)) {
            object->context_animation_timer -= FRAMETIME;
            if (object->context_animation_timer <= 0.0f) {
                object->field_0x7a5 = 0xff;
                object->big_jump_data = NULL;
            }
        }

        if (start_immediately == 0 && (objInNetWaitContext(object, 15) == 0 || object->big_jump_data == NULL)) {
            if (__builtin_expect(!!(object->apiobj.player_controlled), 0))
                object->field_0xe24 |= 0x20;
            return;
        }

        object->field_0x788 = teleport;
        object->field_0x7a5 = 15;
        teleport->active = 1;
        object->context_variant_flags = (object->context_variant_flags & ~4) | (FindTeleport_Direction == 0 ? 4 : 0);
        PlayJumpSfx(object, 0);
        if (object->apiobj.player_controlled) {
            Hint_SetComplete(0x268);
            Hint_SetComplete(0x622);
        }

        if (object->id == id_YODA || object->id == id_YODAGHOST)
            object->context_animation = 4;
        else
            object->context_animation = 30;
        object->field_0x7a3 = 0;
        object->field_0x7a4 = 0;

        i32 start_index;
        i32 end_index;
        teleport = static_cast<TELEPORT_s *>(object->field_0x788);
        Teleport_GetSegment(object, teleport, &start_index, &end_index);
        object->airborne_action_duration = Teleport_GetSegmentDuration(object, teleport, start_index, end_index);
        object->context_animation_timer = object->airborne_action_duration;
        teleport = static_cast<TELEPORT_s *>(object->field_0x788);
        Teleport_SetFacing(object, &teleport->path->pts[start_index], &teleport->path->pts[end_index], false);
        object->context_x_rotation = 0;
        object->apiobj.velocity = v000;
        return;
    }

    TELEPORT_s *teleport = static_cast<TELEPORT_s *>(object->field_0x788);

    if (object->field_0x7a3 == 0) {
        const bool forwards = (object->context_variant_flags & 4) != 0;
        const i16 flap1_rotation = forwards ? 0x4000 : -0x4000;
        const i16 flap2_rotation = forwards ? -0x4000 : 0x4000;
        if (NuSpecialExistsFn(&teleport->flap1_special) &&
            NuVecDistSqr(&object->apiobj.collision_position, NuSpecialGetDrawPos(&teleport->flap1_special), NULL) <
                0.36f) {
            teleport->field_78 = flap1_rotation;
            if ((object->context_x_rotation & 1) == 0) {
                object->context_x_rotation |= 1;
                PlaySfx(const_cast<char *>("env_door_flap"), &object->apiobj.collision_position);
            }
        }
        if (NuSpecialExistsFn(&teleport->flap2_special) &&
            NuVecDistSqr(&object->apiobj.collision_position, NuSpecialGetDrawPos(&teleport->flap2_special), NULL) <
                0.36f) {
            teleport->field_7a = flap2_rotation;
            if ((object->context_x_rotation & 2) == 0) {
                object->context_x_rotation |= 2;
                PlaySfx(const_cast<char *>("env_door_flap"), &object->apiobj.collision_position);
            }
        }
    }

    if (object->field_0x7a3 != 0) {
        if (object->field_0x7a3 != 1)
            return;
        object->context_animation_timer -= FRAMETIME;
        f32 progress = 1.0f - object->context_animation_timer * 0.5f;
        f32 shadow_height = 2000000.0f;
        bool has_surface = false;
        if (object->context_animation_timer <= 0.0f) {
            object->field_0x7a3 = 0;
            i32 start_index;
            i32 end_index;
            teleport = static_cast<TELEPORT_s *>(object->field_0x788);
            Teleport_GetSegment(object, teleport, &start_index, &end_index);
            object->airborne_action_duration = Teleport_GetSegmentDuration(object, teleport, start_index, end_index);
            object->context_animation_timer = object->airborne_action_duration;
            teleport = static_cast<TELEPORT_s *>(object->field_0x788);
            Teleport_SetFacing(object, &teleport->path->pts[start_index], &teleport->path->pts[end_index], true);

            shadow_height = GameShadow(NULL, &object->apiobj.position, 5.0f, -1);
            object->apiobj.field_0x218 = shadow_height;
            has_surface = shadow_height != 2000000.0f;
            GetSurfaceInfo(object, has_surface, shadow_height);
            progress = 1.0f;
        }
        teleport = static_cast<TELEPORT_s *>(object->field_0x788);
        NUVEC *points = teleport->path->pts;
        NUVEC *start = &points[(object->context_variant_flags & 4) != 0 ? 1 : 2];
        NUVEC *end = &points[(object->context_variant_flags & 4) != 0 ? 2 : 1];
        object->apiobj.position.x = (end->x - start->x) * progress + start->x;
        object->apiobj.position.y = (end->y - start->y) * progress + start->y;
        object->apiobj.position.z = (end->z - start->z) * progress + start->z;
        object->apiobj.velocity = v000;
        if (has_surface)
            object->apiobj.position.y = shadow_height - object->character_bottom * object->apiobj.field_0xa8;
        return;
    }

    bool completed = false;
    if (object->apiobj.character_model->model_data_b[object->context_animation] == NULL ||
        CurrentAnim(&object->apiobj.anim_packet) == object->context_animation) {
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer <= 0.0f) {
            object->field_0x7a5 = 0xff;
            object->context_animation_timer = 0.0f;
            teleport->active = 0;
            if ((teleport->flags & 0x10) != 0)
                GameCam_Blend(GameCam, 1.0f, 0.0f, 1);
            AlertSurroundingCreatures(object, &object->apiobj.collision_position);
            completed = true;
        }
    }

    const f32 progress = 1.0f - object->context_animation_timer / object->airborne_action_duration;
    i32 start_index;
    i32 end_index;
    teleport = static_cast<TELEPORT_s *>(object->field_0x788);
    Teleport_GetSegment(object, teleport, &start_index, &end_index);
    NUVEC *segment_start = &teleport->path->pts[start_index];
    NUVEC *segment_end = &teleport->path->pts[end_index];
    NUVEC target;
    target.x = (segment_end->x - segment_start->x) * progress + segment_start->x;
    target.y = (segment_end->y - segment_start->y) * progress + segment_start->y;
    target.z = (segment_end->z - segment_start->z) * progress + segment_start->z;
    SeekVec(&object->apiobj.position, &object->apiobj.position, &target, 10.0f);
    object->apiobj.velocity = v000;

    NUVEC shadow_position = object->apiobj.position;
    shadow_position.y += object->character_bottom * object->apiobj.field_0xa8;
    const f32 shadow_height = GameShadow(NULL, &shadow_position, 5.0f, -1);
    object->apiobj.field_0x218 = shadow_height;
    if (shadow_height != 2000000.0f) {
        object->apiobj.position.y = shadow_height - object->character_bottom * object->apiobj.field_0xa8;
        GetSurfaceInfo(object, 1, shadow_height);
    } else {
        GetSurfaceInfo(object, 0, shadow_height);
    }
    object->apiobj.position.y += progress * 0.0025f;

    if (!completed)
        return;
    teleport = static_cast<TELEPORT_s *>(object->field_0x788);
    if ((teleport->flags & 4) != 0 || object->field_0x7a4 != 0) {
        object->apiobj.velocity.y = -0.1f;
        object->apiobj.field_0x27d = 0;
        return;
    }

    object->field_0x7a5 = 15;
    object->field_0x7a4 = 1;
    object->field_0x7a3 = 1;
    object->context_animation_timer = 2.0f;
    NUVEC *points = teleport->path->pts;
    object->apiobj.start_position = points[(object->context_variant_flags & 4) != 0 ? 1 : 2];
    object->apiobj.position = object->apiobj.start_position;
    GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
}

i32 Teleport_UpdateHints(HINT_s *hint) {
    if (WORLD->teleport_count <= 0 || player->field_0x7a5 != 0xff)
        return 0;
    for (i32 i = 0; i < 2; ++i) {
        GameObject_s *object = Player[i];
        if (__builtin_expect(object == NULL || ((object->field_0xe24 & 0x20) == 0 && Teleport_Find(object, 1.5625f, NULL) == NULL), 1))
            continue;
        if (hint->control_mode_ids[0] == 0x268)
            return AvailableToPlayer(0x40000, -1, 0, 1) != 0;
        if (hint->control_mode_ids[0] == 0x622 && FreePlay != 0 && AvailableToPlayer(0x40000, -1, 0, 1) == 0)
            return 1;
        return 0;
    }
    return 0;
}

ADDGIZMOTYPE *Teleport_RegisterGizmo(i32 type_id) {
    static ADDGIZMOTYPE addtype;

    addtype = Default_ADDGIZMOTYPE;
    addtype.name = "Teleport";
    addtype.prefix = "TLT_";
    addtype.fns.unknown1 = 0;
    addtype.fns.get_max_gizmos_fn = Teleport_GetMaxGizmos;
    addtype.fns.add_gizmos_fn = Teleport_AddGizmos;
    addtype.fns.early_update_fn = NULL;
    addtype.fns.late_update_fn = NULL;
    addtype.fns.draw_fn = NULL;
    addtype.fns.panel_draw_fn = NULL;
    addtype.fns.get_gizmo_name_fn = Teleport_GetGizmoName;
    addtype.fns.get_output_fn = Teleport_GetOutput;
    addtype.fns.get_output_name_fn = Teleport_GetOutputName;
    addtype.fns.get_num_outputs_fn = Teleport_GetNumOutputs;
    addtype.fns.activate_fn = Teleport_Activate;
    addtype.fns.activate_rev_fn = Teleport_ActivateRev;
    addtype.fns.set_visibility_fn = NULL;
    addtype.fns.get_visibility_fn = NULL;
    addtype.fns.get_pos_fn = NULL;
    addtype.fns.using_special_fn = NULL;
    addtype.fns.allocate_progress_data_fn = NULL;
    addtype.fns.clear_progress_fn = NULL;
    addtype.fns.store_progress_fn = NULL;
    addtype.fns.reset_fn = NULL;
    addtype.fns.reserve_buffer_space_fn = NULL;
    addtype.fns.load_fn = NULL;
    addtype.fns.post_load_fn = NULL;
    addtype.fns.add_level_sfx_fn = NULL;
    teleport_gizmotype_id = type_id;

    return &addtype;
}

